/** @file
                        TrEEDxe.c

  TrEEDxe driver implements the protocol defined in Microsoft "Trusted
  Execution Environment(TrEE) EFI Protocol, Draft 0.72 – April 19th, 2011".

  Copyright (c) 2012-2019 Copyright Qualcomm Technologies, Inc.  All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential.

**/

/*=============================================================================
                EDIT HISTORY


 when       who    what, where, why
 --------   ---     ----------------------------------------------------------
 03/29/19   shl     Moved register ACPI call to TpmDxe
 12/20/18   shl     Moved out more TPM functions to TpmDxe.c
 09/19/18   pr      Enabled TpmDxe
 09/13/18   shl     Revert to previous version
 02/20/18   shl     Split TrEE and TPM Dxe to load TPM earlier for NV counter access
 07/25/18   pj      Fixed kw error - memory leak of pTCGPCREvent
 07/19/17   dp      TrEE Carveout Changes
 06/14/18   pj      TPM app separation from winsecapp
 05/03/18   pj      Ported SHMBridge changes to Poipu
 04/07/17   pr      Changes to pick TPM Control Area from static memory regions
 10/06/16   pr      Enablement of Winsecapp for MSM 8998 WP
 05/19/15   sk      Added support for TCG 2.0
 04/22/15   sm      Changed TpmReq and TpmRsp to be dynamically allocated.
 05/28/14   sm      Changed TPM Control Area to be dynamically allocated at runtime
 03/18/14   sm      Updated print statements
 12/27/13   sp      Updated to new fields of TPMControlAreaMemRegionInfo
 10/09/13   niting  Moved TPM control area
 08/07/13   sahn    Used the callback mechanism in AcpiPlatformDxe to fix the tables.
 05/15/13   shl     Used UEFI utility to convert command to avoid alignment crash.
 04/22/13  bmuthuku Send signal to fTPM to disable NV flushing.
 02/01/13   shl     Query ApplicationID first and no matter what return success,
                    so ApplicationId is always set. Added support for get app ID
                    Changed the data allocation to reduce image space.
 01/10/13   niting  Switch to new Scm Dxe
 01/02/13   shl     Reverted the change, will check in later once winsecapp is checked in
 12/10/12   shl     Switch to new Scm Dxe
 07/19/12   shl     Added flush tpm nv store support
 11/18/11   Eamonn  Initial version

=============================================================================*/
#include <Uefi.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UncachedMemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiLib.h>
#include <IndustryStandard/UefiTcgPlatform.h>
#include <Library/PeHashLib.h>
#include <Library/UefiCfgLib.h>
#include <Library/Tpm2DeviceLib.h>
#include <Library/QcomLib.h>
#include <Protocol/Hash.h>
#include <Protocol/EFITrEE.h>
#include <Protocol/EFITpm.h>
#include "TzfTPMCmd.h"


//  The following are required for the header event as per latest TCG 2.0 spec
//  The header event is the 1st event that goes into the log (not TPM)
//  This must only be done the first time
//  Spec ID Event03 
STATIC UINT8 signature_array[16] = { 0x53, 0x70, 0x65, 0x63, 0x20, 0x49, 0x44, 0x20, 0x45, 0x76, 0x65, 0x6e, 0x74, 0x30, 0x33, 0x00 }; 
STATIC UINT8 digests_array[20]   = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

// protocol pointers
STATIC EFI_HASH_PROTOCOL  *pEfiHashProtocol = NULL;
STATIC EFI_TPM_PROTOCOL   *pTpmProtocol     = NULL;

TREE_VERSION StructureVersion = { 1, 1 };
TREE_VERSION ProtocolVersion  = { 1, 1 };

#define  MAX_EVENT_LOG_SIZE  65536
#define  HASH_SHA256_LEN     32

#define  TPM_COMMAND_SIZE   1000
#define  TPM_RESPONSE_SIZE  1000

UINT8*   pTpmCommand  = NULL;
UINT8*   pTpmResponse = NULL;

BOOLEAN  TpmReady = FALSE;

UINT8   *header_eventLog     = NULL;
UINT8   *real_eventLog       = NULL;
UINT8   *lastEventLogEntry   = NULL;
UINT32   byteCounter         = 0;
UINT32   numOfEntries        = 0;
BOOLEAN  isEventLogTruncated = FALSE;
BOOLEAN  eventLogIsFull      = FALSE;
UINT32   qct_PCRMaskActive   = TREE_BOOT_HASH_ALG_SHA256;

// TrEE protocol implemetation
EFI_STATUS GetCapability(
    IN EFI_TREE_PROTOCOL                *This,
    IN OUT TREE_BOOT_SERVICE_CAPABILITY *ProtocolCapability
    );

EFI_STATUS GetEventLog(
    IN EFI_TREE_PROTOCOL       *This,
    IN TREE_EVENT_LOG_FORMAT    EventLogFormat,
    OUT EFI_PHYSICAL_ADDRESS   *EventLogLocation,
    OUT EFI_PHYSICAL_ADDRESS   *EventLogLastEntry,
    OUT BOOLEAN                *EventLogTruncated
    );

EFI_STATUS HashLogExtendEvent(
    IN EFI_TREE_PROTOCOL        *This,
    IN UINT64                    Flags,
    IN EFI_PHYSICAL_ADDRESS      DataToHash,
    IN UINT64                    DataToHashLen,
    IN TrEE_EVENT               *Event
    );

EFI_STATUS SubmitCommand(
    IN EFI_TREE_PROTOCOL      *This,
    IN UINT32                  InputParameterBlockSize,
    IN UINT8                  *InputParameterBlock,
    IN UINT32                  OutputParameterBlockSize,
    IN UINT8                  *OutputParameterBlock
    );

EFI_STATUS GetCurrentActivePCRBanks(
    IN EFI_TREE_PROTOCOL* This,
    OUT UINT32*     ActivePcrBanks
    );

EFI_STATUS SetCurrentActivePCRBanks(
    IN EFI_TREE_PROTOCOL* This,
    IN UINT32       ActivePcrBanks
    );


EFI_TREE_PROTOCOL TrEEProtocolImpl = {
   GetCapability,
   GetEventLog,
   HashLogExtendEvent,
   SubmitCommand,
   GetCurrentActivePCRBanks,
   SetCurrentActivePCRBanks
};


STATIC EFI_STATUS CreateTpmCmdHeader(TPM_CC id, TPM_ST tag, UINT32 paramSize, UINT8 *param)
{
   TPM2_COMMAND_HEADER header;

   header.tag = SwapBytes16(tag);
   header.paramSize = SwapBytes32(sizeof(TPM2_COMMAND_HEADER)+paramSize);
   header.commandCode = SwapBytes32(id);

   if ((sizeof(header)+paramSize) < TPM_COMMAND_SIZE)
   {
     CopyMem(pTpmCommand,  (UINT8 *)&header, sizeof(header));
     CopyMem(pTpmCommand + sizeof(header), param, paramSize);

     return EFI_SUCCESS;
   }
   else
     return EFI_INVALID_PARAMETER;
}

/**
  This function query the capability of TrEE

  @param  IN this                       An EFI_TREE_PROTOCOL instance.
  @param  IN OUT *ProtocolCapability    A pointer to a capability buffer where
                                        the capability structure is, this buffer
                                        will be filled if call is successful.

  @retval EFI_SUCCESS                   Call is successfully.
  @retval EFI_INVALID_PARAMETER         NULL parameter is passed
  @retval EFI_BUFFER_TOO_SMALL          passed capability buffer is too small.

**/
EFI_STATUS GetCapability(
    IN EFI_TREE_PROTOCOL                *This,
    IN OUT TREE_BOOT_SERVICE_CAPABILITY *ProtocolCapability
    )
{
    EFI_STATUS Status = EFI_SUCCESS;

    if (NULL == This || NULL == ProtocolCapability)
    {
        Status = EFI_INVALID_PARAMETER;
        goto Exit;
    }

    if (ProtocolCapability->Size < sizeof(TREE_BOOT_SERVICE_CAPABILITY))
    {
        ProtocolCapability->Size = sizeof(TREE_BOOT_SERVICE_CAPABILITY);
        Status = EFI_BUFFER_TOO_SMALL;
        goto Exit;
    }

    ProtocolCapability->StructureVersion.Major = StructureVersion.Major;
    ProtocolCapability->StructureVersion.Minor = StructureVersion.Minor;
    ProtocolCapability->ProtocolVersion.Major = ProtocolVersion.Major;
    ProtocolCapability->ProtocolVersion.Minor = ProtocolVersion.Minor;

    if(TpmReady)
    {
        ProtocolCapability->SupportedEventLogs = TREE_EVENT_LOG_FORMAT_TCG_2;
        ProtocolCapability->HashAlgorithmBitmap = TREE_BOOT_HASH_ALG_SHA256;
        ProtocolCapability->TrEEPresentFlag = TRUE;
        ProtocolCapability->MaxCommandSize = 4096;
        ProtocolCapability->MaxResponseSize = 4096;
        ProtocolCapability->ManufacturerID = SIGNATURE_32('Q', 'C', 'O', 'M');
    }
    else
    {
        ProtocolCapability->SupportedEventLogs  = 0;
        ProtocolCapability->HashAlgorithmBitmap = 0;
        ProtocolCapability->TrEEPresentFlag     = FALSE;
        ProtocolCapability->MaxCommandSize      = 0;
        ProtocolCapability->MaxResponseSize     = 0;
        ProtocolCapability->ManufacturerID      = 0;
    }

    ProtocolCapability->NumberOfPcrBanks = 1;                           // Only 1 PCR bank - SHA256
    ProtocolCapability->ActivePcrBanks = TREE_BOOT_HASH_ALG_SHA256;   // Denotes SHA256, same as HashAlgorithmBitmap

Exit:
    return Status;
}

/**
  This function query the event log of TrEE

  @param  *this                    An EFI_TREE_PROTOCOL instance.
  @param  EventLogFormat           Event log format.
  @param  *EventLogLocation        A pointer to event log location.
  @param  *EventLogLastEntry       A pointer to start of last event entry.
  @param  *EventLogTruncated       A flag indicating the event log is trucated or not.

  @retval EFI_SUCCESS              Call is successfully.
  @retval EFI_INVALID_PARAMETER    NULL parameter is passed

**/
EFI_STATUS GetEventLog(
    IN EFI_TREE_PROTOCOL       *This,
    IN TREE_EVENT_LOG_FORMAT    EventLogFormat,
    OUT EFI_PHYSICAL_ADDRESS   *EventLogLocation,
    OUT EFI_PHYSICAL_ADDRESS   *EventLogLastEntry,
    OUT BOOLEAN                *EventLogTruncated
    )
{
    EFI_STATUS Status = EFI_SUCCESS;

    if (NULL == This ||
        NULL == EventLogLocation ||
        NULL == EventLogLastEntry ||
        NULL == EventLogTruncated ||
        TREE_EVENT_LOG_FORMAT_TCG_2 != EventLogFormat)
    {
        Status = EFI_INVALID_PARAMETER;
        goto ErrorExit;
    }

    if(!TpmReady)
    {
        // not up, bail out.
        *EventLogLocation = 0;
        *EventLogLastEntry = 0;
        *EventLogTruncated = FALSE;
        goto ErrorExit;
    }

    *EventLogLocation = (EFI_PHYSICAL_ADDRESS)header_eventLog;

    if (numOfEntries == 0)
    {
        *EventLogLastEntry = 0;
    }
    else
    {
        *EventLogLastEntry = (EFI_PHYSICAL_ADDRESS)lastEventLogEntry;
    }

    *EventLogTruncated = eventLogIsFull;

ErrorExit:
    return Status;
}


/**
  This function hash, log and extend event

  @param  *this                    An EFI_TREE_PROTOCOL instance.
  @param  Flags                    Bitmap indicating extend only and data format.
  @param  DataToHash               Start address of data to be hashed.
  @param  DataToHashLen            Length of data to be hashed.
  @param  *Event                   A pointer to the Event to be logged and extended.

  @retval EFI_SUCCESS              Call is successfully.
  @retval EFI_INVALID_PARAMETER    NULL parameter is passed
  @retval EFI_DEVICE_ERROR         SCM call failed.
  @retval EFI_VOLUME_FULL          Failed to log in an event due to log is full,
                                   extend is successful.
**/
EFI_STATUS HashLogExtendEvent(
    IN EFI_TREE_PROTOCOL        *This,
    IN UINT64                    Flags,
    IN EFI_PHYSICAL_ADDRESS      DataToHash,
    IN UINT64                    DataToHashLen,
    IN TrEE_EVENT               *Event
    )
{
    EFI_STATUS        Status = EFI_SUCCESS;
    UINT32            NewEntrySize;
    TCG_PCR_EVENT2   *ptrEventEntry;
    EFI_SHA256_HASH   Sha256HashBuffer;
    EFI_HASH_OUTPUT   HashOutput;
    UINT16            SubSystem;
    UINT32            pTpmCommandSize;
    UINT32            pTpmResponseSize = TPM_RESPONSE_SIZE;
    UINT8            *buffer;
    UINT8            *authSizeOffset;

    // New
    UINT8  *digestPtr;
    UINT8  *eventPtr;
    UINT32 *eventSizePtr;

    //  The following are required for the header event as per latest TCG 2.0 spec
    //  The header event is the 1st event that goes into the log (not TPM)
    //  This must only be done the first time
    STATIC BOOLEAN    first_time = TRUE;
    TCG_PCR_EVENT    *pTCGPCREvent = NULL;
    UINT32            loopindex;
    UINT32            EventDataSize = sizeof(TCG_EfiSpecIDEventStruct) + sizeof(UINT32) + (sizeof(TCG_EfiSpecIdEventAlgorithmSize)) + sizeof(UINT8);   // Size of the header-event structure = 33 bytes
    UINT32            EventSize = sizeof(TCG_PCRINDEX) + sizeof(TCG_EVENTTYPE) + sizeof(TCG_DIGEST) + sizeof(UINT32) + EventDataSize;

    UINT8                           *pTCG_EfiSpecIDEventStruct;
    TCG_EfiSpecIDEventStruct        *pIDEventStruct;
    TCG_EfiSpecIdEventAlgorithmSize *pIdEventAlgorithmSize;

    if (first_time)
    {
        //  The first event recorded to log must be a header event as per latest spec.
        //  This header event need not be recorded to TPM.

        //  Clear the first_time flag
        first_time = FALSE;

        //  Allocate memory for event
        pTCGPCREvent = (TCG_PCR_EVENT *)AllocatePool(EventSize);
        if (pTCGPCREvent == NULL)
        {
            Status = EFI_OUT_OF_RESOURCES;
            goto ErrorExit;
        }

        // Fill up the header TCG_EfiSpecIDEventStruct structure
        pTCG_EfiSpecIDEventStruct = (UINT8 *)AllocatePool(EventDataSize);
        if (pTCG_EfiSpecIDEventStruct == NULL)
        {
            Status = EFI_OUT_OF_RESOURCES;
            goto ErrorExit;
        }

        pIDEventStruct = (TCG_EfiSpecIDEventStruct *)pTCG_EfiSpecIDEventStruct;

        for (loopindex = 0; loopindex < 16; loopindex++)
        {
          pIDEventStruct->signature[loopindex] = signature_array[loopindex];
        }
        //TCG_EfiSpecIDEventStructobj.signature[0] = 0x53;//, 0x70, 0x65, 0x63, 0x20, 0x49, 0x44, 0x20, 0x45, 0x76, 0x65, 0x6e, 0x74, 0x30, 0x33, 0x00}; // “Spec ID Event03”
        pIDEventStruct->platformClass = 0;
        pIDEventStruct->specVersionMinor = 2;  //Should be 0
        pIDEventStruct->specVersionMajor= 1;   //Should be 2
        pIDEventStruct->specErrata = 0;
        pIDEventStruct->uintnSize = 2;         // UINTN fields should be 64 bit size

        *(UINT32 *)(pTCG_EfiSpecIDEventStruct + sizeof(TCG_EfiSpecIDEventStruct)) = 1;   // numberOfAlgorithms
        pIdEventAlgorithmSize = (TCG_EfiSpecIdEventAlgorithmSize *)(pTCG_EfiSpecIDEventStruct + sizeof(TCG_EfiSpecIDEventStruct) + sizeof(UINT32));
        pIdEventAlgorithmSize->algorithmId = TPM_ALG_SHA256;   //
        pIdEventAlgorithmSize->digestSize  = SHA256_DIGEST_SIZE;

        *(pTCG_EfiSpecIDEventStruct + sizeof(TCG_EfiSpecIDEventStruct) + sizeof(UINT32) + sizeof(TCG_EfiSpecIdEventAlgorithmSize)) = 0;   // vendorInfoSize = 0;

        // Fill up the TCG_PCR_EVENT fields
        pTCGPCREvent->PCRIndex = 0;
        pTCGPCREvent->EventType = EV_NO_ACTION;
        for (loopindex = 0; loopindex < 20; loopindex++)
        {
            pTCGPCREvent->Digest.digest[loopindex] = digests_array[loopindex];
        }
        //pTCGPCREvent->Digest.digest[TPM_SHA1_160_HASH_LEN] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }; // 20 bytes of zero
        pTCGPCREvent->EventSize = EventDataSize;
        CopyMem(pTCGPCREvent->Event, pTCG_EfiSpecIDEventStruct, EventDataSize);

        // Copy this TCG_PCR_EVENT structire to eventLog
        CopyMem(header_eventLog, pTCGPCREvent, EventSize);
        //Free pTCGPCREvent since the variable and it's memory will not be used anymore
        FreePool(pTCGPCREvent);

        real_eventLog = header_eventLog + EventSize;
    }

    if (NULL == This || 0 == DataToHash || NULL == (VOID *)Event)
    {
        Status = EFI_INVALID_PARAMETER;
        goto ErrorExit;
    }

    if (Event->Size < (Event->Header.HeaderSize + sizeof(UINT32)))
    {
        Status = EFI_INVALID_PARAMETER;
        goto ErrorExit;
    }

    if (Event->Header.PCRIndex > 23)
    {
        Status = EFI_INVALID_PARAMETER;
        goto ErrorExit;
    }

    HashOutput.Sha256Hash = &Sha256HashBuffer;

    // Check flag to decide how to hash
    if (Flags & PE_COFF_IMAGE)
    {
        // Hash a PE/COFF file, use SHA256 as in specs.
        Status = ComputePeImageHash(
            (VOID *)(UINTN)DataToHash,
            DataToHashLen,
            &gEfiHashAlgorithmSha256Guid,
            (UINT8 *)HashOutput.Sha256Hash
            );
    }
    else
    {
        // gEfiHashAlgorithmSha256Guid is hardcoded here
        Status = pEfiHashProtocol->Hash(
            pEfiHashProtocol,
            &gEfiHashAlgorithmSha256Guid,
            FALSE,
            (UINT8 *)(UINTN)DataToHash,
            DataToHashLen,
            &HashOutput
            );
    }
    if (EFI_ERROR(Status))
    {
        DEBUG(( EFI_D_ERROR, " Hash() failed, Status =  0x%x\r\n", Status));
        goto ErrorExit;
    }

    Status = CreateTpmCmdHeader(TPM_CC_PCR_Extend, TPM_ST_SESSIONS, 0, NULL);
    if (EFI_ERROR(Status))
    {
        DEBUG(( EFI_D_ERROR, " CreateTpmCmdHeader failed, Status =  0x%x\r\n", Status));
        goto ErrorExit;
    }

    buffer = ((UINT8*)pTpmCommand) + sizeof(TPM2_COMMAND_HEADER); // pR->input;

    // pcrHandle
    WriteUnaligned32((UINT32 *)buffer, SwapBytes32(Event->Header.PCRIndex));
    buffer += sizeof(UINT32);

    // Add in Auth session
    authSizeOffset = buffer;
    WriteUnaligned32((UINT32 *)buffer, 0);
    buffer += sizeof(UINT32);

    // authHandle
    WriteUnaligned32((UINT32 *)buffer, SwapBytes32(TPM_RS_PW));
    buffer += sizeof(TPM_RH);

    // nonce = nullNonce
    WriteUnaligned16((UINT16 *)buffer, 0);
    buffer += sizeof(UINT16);

    // sessionAttributes = 0
    *(UINT8 *)buffer = 0;
    buffer += sizeof(UINT8);

    // auth = nullAuth
    WriteUnaligned16((UINT16 *)buffer, 0);
    buffer += sizeof(UINT16);

    // authorizationSize
    WriteUnaligned32((UINT32 *)authSizeOffset, SwapBytes32((UINT32)(buffer - authSizeOffset - sizeof(UINT32))));

    // digests
    // Digest.count
    WriteUnaligned32((UINT32 *)buffer, SwapBytes32(1));
    buffer += sizeof(UINT32);

    // hashAlg
    WriteUnaligned16((UINT16 *)buffer, SwapBytes16(TPM_ALG_SHA256));
    buffer += sizeof(TPM_ALG_ID);

    CopyMem(buffer, HashOutput.Sha256Hash, HASH_SHA256_LEN);
    buffer += HASH_SHA256_LEN;

    pTpmCommandSize = (UINT32)(buffer - (((UINT8*)pTpmCommand)));
    ((TPM2_COMMAND_HEADER*)pTpmCommand)->paramSize = SwapBytes32(pTpmCommandSize);

    Status = pTpmProtocol->SubmitCommand( pTpmProtocol,
                                          pTpmCommandSize,
                                          pTpmCommand,
                                         &pTpmResponseSize,
                                          pTpmResponse );
    if (EFI_ERROR(Status))
    {
       DEBUG(( EFI_D_INFO, " Tpm SubmitCommand failed, Status =  0x%x\r\n", Status));
    }

    // check flag
    if (Flags & TREE_EXTEND_ONLY)
    {
        // bail out here.
        goto ErrorExit;
    }

    // calculate new entry size
    // New
    //NewEntrySize = sizeof(TCG_PCR_EVENT2) - sizeof(UINT8 /*Event[1]*/) + (Event->Size - sizeof(UINT32) - Event->Header.HeaderSize);
    NewEntrySize = sizeof(UINT32/*TCG_PCRINDEX*/) + sizeof(UINT32/*TCG_EVENTTYPE*/) + sizeof(UINT32) + sizeof(UINT16) + HASH_SHA256_LEN + sizeof(UINT32) + Event->Size - sizeof(UINT32) - Event->Header.HeaderSize;

    if ((byteCounter + NewEntrySize) >= MAX_EVENT_LOG_SIZE)
    {
        eventLogIsFull = TRUE;
        // bail out here.
        Status = EFI_VOLUME_FULL;
        goto ErrorExit;
    }

    ptrEventEntry = (TCG_PCR_EVENT2 *)(real_eventLog + byteCounter);
    // log this event it.
    if (Flags & PE_COFF_IMAGE)
    {
        // handle different PE case here.
        GetPeImageSubSystem(&SubSystem);
        switch (SubSystem)
        {
        case EFI_IMAGE_SUBSYSTEM_EFI_APPLICATION:
            ptrEventEntry->PCRIndex = 4;
            ptrEventEntry->EventType = EV_EFI_BOOT_SERVICES_APPLICATION;
            break;

        case EFI_IMAGE_SUBSYSTEM_EFI_BOOT_SERVICE_DRIVER:
            ptrEventEntry->PCRIndex = 2;
            ptrEventEntry->EventType = EV_EFI_BOOT_SERVICES_DRIVER;
            break;

        case EFI_IMAGE_SUBSYSTEM_EFI_RUNTIME_DRIVER:
        case EFI_IMAGE_SUBSYSTEM_SAL_RUNTIME_DRIVER:
            ptrEventEntry->PCRIndex = 2;
            ptrEventEntry->EventType = EV_EFI_RUNTIME_SERVICES_DRIVER;
            break;
        }
    }
    else
    {
        ptrEventEntry->PCRIndex = Event->Header.PCRIndex;
        ptrEventEntry->EventType = Event->Header.EventType;
    }
    ptrEventEntry->Digest.count = 1;                               // sha256 is the only supported algorithm by Qualcomm, hence count is 1
    ptrEventEntry->Digest.digests[0].hashAlg = TPM_ALG_SHA256;     // sha256 is the only one in the list of digests, hence digests[0]

    // New
    digestPtr = (UINT8*)&(ptrEventEntry->Digest.digests[0].digest.sha256);
    CopyMem(digestPtr, HashOutput.Sha256Hash, HASH_SHA256_LEN);
    //CopyMem( &(ptrEventEntry->Digests.digests[0].digest.sha256), HashOutput.Sha256Hash, HASH_SHA256_LEN );

    // New
    eventSizePtr = (UINT32*)(digestPtr + HASH_SHA256_LEN);
    *eventSizePtr = Event->Size - sizeof(UINT32) - Event->Header.HeaderSize;
    //ptrEventEntry->EventSize = Event->Size - sizeof(UINT32) - Event->Header.HeaderSize;

    // New
    eventPtr = (UINT8*)eventSizePtr + sizeof(UINT32);
    CopyMem(eventPtr, Event->Event, *eventSizePtr);
    //CopyMem( ptrEventEntry->Event, Event->Event, ptrEventEntry->EventSize);

    // update the lastEventEntry
    lastEventLogEntry = real_eventLog + byteCounter;
    // update the byteCounter
    byteCounter += NewEntrySize;
    // update numOfntries
    numOfEntries++;

ErrorExit:
    return Status;
}

/**
  This function submit a command to TZ, this assume caller know the command
  stream and how to read output block. Assume this is calling to fTPM in TZ.

  @param  *this                     An EFI_TREE_PROTOCOL instance.
  @param  InputParameterBlockSize   Size of input parameter block.
  @param  *InputParameterBlock      A pointer to input parameter block.
  @param  OutputParameterBlockSize  Size of output parameter block.
  @param  *OutputParameterBlock     A pointer to output parameter block.

  @retval EFI_SUCCESS              Call is successfully.
  @retval EFI_INVALID_PARAMETER    NULL parameter is passed
  @retval EFI_DEVICE_ERROR         SCM call failed.

**/
EFI_STATUS SubmitCommand(
    IN EFI_TREE_PROTOCOL       *This,
    IN UINT32                   InputParameterBlockSize,
    IN UINT8                   *InputParameterBlock,
    IN UINT32                   OutputParameterBlockSize,
    IN UINT8                   *OutputParameterBlock
    )
{
  UINT32 OutputSize = OutputParameterBlockSize;
/*
  // real use case
  return pTpmProtocol->SubmitCommand( pTpmProtocol,
                                      InputParameterBlockSize,
                                      InputParameterBlock,
                                     &OutputSize,
                                      OutputParameterBlock
                                    );
*/
  // test case for Tpm2DeviceLib, keep it so this API is always tested.
  return Tpm2SubmitCommand ( InputParameterBlockSize,
                             InputParameterBlock,
                            &OutputSize,
                             OutputParameterBlock
                           );

}

/**
  The EFI_TREE_PROTOCOL GetCurrentActivePCRBanks function provides information
  about the active platform PCRs. In addition if a request has been made in this
  boot sequence for changing the active PCRs but will be in effect at the next
  boot, that information is also returned.

  @param[in]    This                    An EFI_TREE_PROTOCOL instance.

  @param[out]   ActivePcrBanks          Pointer to the variable receiving the bitmap of currently active PCR banks.

  @retval       EFI_SUCCESS             Success.
                EFI_INVALID_PARAMETER   Null pointer passed in

**/
EFI_STATUS GetCurrentActivePCRBanks(
    IN EFI_TREE_PROTOCOL* This,
    OUT UINT32*     ActivePcrBanks
    )
{
    EFI_STATUS Status = EFI_SUCCESS;

    if (NULL == This)
    {
        Status = EFI_INVALID_PARAMETER;
        goto ErrorExit;
    }

    // Currently Qualcomm only needs to support SHA256 for TCG 2.0 spec
    *ActivePcrBanks = qct_PCRMaskActive;

ErrorExit:

    return Status;
}

/**
  The EFI_TREE_PROTOCOL SetCurrentActivePCRBanks function call makes the request
  to update the PCR banks that are to be used. This will get into effect at the
  next platform reboot.

  @param[in]  This                    An EFI_TREE_PROTOCOL instance.

  @param[in]  ActivePcrBanks          Bitmap of the requested active PCR banks.

  @retval     EFI_SUCCESS             Success.
              EFI_INVALID_PARAMETER   Null pointer passed in

**/
EFI_STATUS SetCurrentActivePCRBanks(
    IN EFI_TREE_PROTOCOL* This,
    IN UINT32       ActivePcrBanks
    )
{
    EFI_STATUS Status = EFI_SUCCESS;

    if (NULL == This)
    {
        Status = EFI_INVALID_PARAMETER;
        goto ErrorExit;
    }

    // Currently Qualcomm only needs to support SHA256 for TCG 2.0 spec. Just return SUCCESS

ErrorExit:

    return Status;
}


/**
  The Entry Point for TrEEDxe.

  @param[in] ImageHandle  The firmware allocated handle for the EFI image.
  @param[in] SystemTable  A pointer to the EFI System Table.

  @retval EFI_SUCCESS     The entry point is executed successfully.
  @retval other           Some error occurs when executing this entry point.

**/
EFI_STATUS TrEEDxeInitialize
(
   IN EFI_HANDLE        ImageHandle,
   IN EFI_SYSTEM_TABLE *SystemTable
)
{
    EFI_STATUS  Status = EFI_SUCCESS;
    UINT32      ConfigFlag = 0;

    Status = GetConfigValue("SecurityFlag", &ConfigFlag);
    if ( EFI_ERROR(Status)) 
    {
       DEBUG((EFI_D_INFO, " get SecurityFlag failed, Status =  0x%x\r\n", Status));
       goto ErrorExit;
    }  

    if ((ConfigFlag & ENABLE_TCG_DXE_FLAG) == ENABLE_TCG_DXE_FLAG )
    {
       DEBUG((EFI_D_INFO, "TcgDxe is used \r\n"));
       goto ErrorExit;
    }

    // locate Tpm protocol
    Status = gBS->LocateProtocol ( &gEfiTpmProtocolGuid,  NULL,  (VOID **)&pTpmProtocol );
    if (EFI_ERROR(Status))
    {
       DEBUG(( EFI_D_ERROR, " Locate TPM Protocol failed, Status =  (0x%x)\r\n", Status));
       goto ErrorExit;
    }
    else
       TpmReady = TRUE;

    // Allocate mem for event log
    header_eventLog = AllocatePool(MAX_EVENT_LOG_SIZE);
    if (header_eventLog == NULL)
    {
        Status = EFI_OUT_OF_RESOURCES;
        goto ErrorExit;
    }

    pTpmCommand = AllocateZeroPool(TPM_COMMAND_SIZE);
    if (pTpmCommand == NULL)
    {
       Status = EFI_OUT_OF_RESOURCES;
        goto ErrorExit;
    }

    pTpmResponse = AllocateZeroPool(TPM_RESPONSE_SIZE);
    if (pTpmResponse == NULL)
    {
       Status = EFI_OUT_OF_RESOURCES;
        goto ErrorExit;
    }

    Status = gBS->LocateProtocol( &gEfiHashProtocolGuid, NULL, (VOID **)&pEfiHashProtocol );
    if (EFI_ERROR(Status))
    {
       DEBUG((EFI_D_INFO, " Locate hash Protocol failed, Status =  0x%x\r\n", Status));
        goto ErrorExit;
    }

    // Install the TrEE protocol implemented here
    Status = gBS->InstallMultipleProtocolInterfaces( &ImageHandle,
                                                     &gEfiTrEEProtocolGuid,  
                                                     &TrEEProtocolImpl,
                                                      NULL );
    if (EFI_ERROR(Status))
    {
        DEBUG((EFI_D_ERROR, " InstallMultipleProtocolInterfaces() failed, Status = (0x%p)\r\n", Status));
        goto ErrorExit;
    }

ErrorExit:

    return Status;
}

