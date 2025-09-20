
/** 
                    TrEEApp.c

  TrEEApp.c is responsible for testing TrEEDxe driver.

  Copyright (c) 2011-2018 Qualcomm Technologies, Inc.  All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential.

**/

/*=============================================================================
                              EDIT HISTORY
  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.



 when       who      what, where, why
 --------   ---      ----------------------------------------------------------
 12/20/18   shl      Moved to TPM protocol and format change
 09/19/18   pr       Enabled TpmDxe
 09/13/18   shl      Revert TpmDxe
 07/14/18   shl      Split TreeDxe, added tpm MINK API test cases, cosmetic change
 05/24/17   pr       Changes to reset device after test app executes
 06/02/17   rp       Update of TCG Header length to travel Log
 10/06/16   rp       Enablement of Winsecapp for MSM 8998 WP
 05/19/15   sk       Added support for TCG 2.0
 12/10/13   sm       Updated print statements
 11/11/13   sm       Added Test Interface Macros
 09/10/13   shl      Hide GPT measurement data
 02/04/13   shl      Changed printing format
 12/10/12   shl      Fixed warning
 10/25/12   shl      Reorg test code for easy use for APT team
 07/03/12   aus      Replaced DEBUG with AsciiPrint
 08/28/11   shl      Initial version

=============================================================================*/

#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/QcomLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/TestInterface.h>
#include <Protocol/EFIScm.h>
#include <Protocol/EFITpm.h>
#include <Protocol/EFITrEE.h>
#include <IndustryStandard/UefiTcgPlatform.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UncachedMemoryAllocationLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <string.h>
#include <PeHashLib.h>
#include <Library/EfiFileLib.h>
#include <MemRegionInfo.h>
#include <Library/UefiCfgLib.h>

#define TZEXEC_SVC_TESTSRV_ID          0x0001000
#define TESTSRV_CREATE_CMD(x)          (TZEXEC_SVC_TESTSRV_ID | x)
#define TESTSRV_TPM_NV_COUNTER         TESTSRV_CREATE_CMD(16)
#define TESTSRV_TPM_MINK_API           TESTSRV_CREATE_CMD(17)

extern EFI_GUID gEfiACPITableLoadGuid;
EFI_EVENT ACPITableLoadEvent  = NULL;

enum log_cmd {
  LOG_PRINT,
  LOG_AVER_FORMAT,
  LOG_COMPARE
};

//pOutData tests pDataToHash output of HashLogExtendEvent
static UINT8 pDataToHash[] = {1, 2, 3, 4};
static UINT32 DataToHashLen = 4;
static UINT8 pOutData[] = {0x9F, 0x64, 0xA7, 0x47, 0xE1, 0xB9, 0x7F, 0x13, 0x1F, 0xAB, 0xB6, 0xB4, 0x47, 0x29, 0x6C, 0x9B};


#if 0  //For reference:
#define TPM_ST_NO_SESSIONS     (TPM_ST)(0x8001)
#define TPM_CC_GetRandom       (TPM_CC)(0x0000017B)

typedef struct _TPM2_COMMAND_HEADER {
    TPM_ST tag;
    UINT32 paramSize;
    TPM_CC commandCode;
} TPM2_COMMAND_HEADER;

typedef struct _TPM2_RESPONSE_HEADER {
    TPM_ST tag;
    UINT32 paramSize;
    TPM_RC responseCode;
} TPM2_RESPONSE_HEADER;
#endif

typedef struct _GET_RANDOM_PARAMS {
    UINT16 bytesRequested;
} GET_RANDOM_PARAMS, *PGET_RANDOM_PARAMS;
 
typedef struct _TPM_GET_RANDOM_REQ {
    TPM2_COMMAND_HEADER commandHeader;
    GET_RANDOM_PARAMS params;
} TPM_GET_RANDOM_REQ, *PTPM_GET_RANDOM_REQ;

typedef struct _TPM_GET_RANDOM_RSP {
    TPM2_RESPONSE_HEADER responseHeader;
    UINT32  RandomData; // Size this based on how much data you expect to get back
} TPM_GET_RANDOM_RSP, *PTPM_GET_RANDOM_RSP;

typedef struct _TESTSRV_TPM_MINK_API_REQ {
    UINT32 cmdId;
}__attribute__ ((packed)) TESTSRV_TPM_MINK_API_REQ;

typedef struct _TESTSRV_TPM_MINK_API_RSP {
    INT32 status;
}__attribute__ ((packed)) TESTSRV_TPM_MINK_API_RSP;


static EFI_TREE_PROTOCOL *pEfiTrEEProtocol = NULL;
static EFI_TPM_PROTOCOL  *pTPMProtocol     = NULL;
static QCOM_SCM_PROTOCOL *pScmProtocol      = NULL;

VOID
EFIAPI
ACPITableLoadEmptyFuntion (
  IN EFI_EVENT                Event,
  IN VOID                    *Context
  )
{
  return;
}

/**
  The user Entry Point for Application. The user code starts with this function
  as the real entry point for the application.

  @param[in] ImageHandle    The firmware allocated handle for the EFI image.  
  @param[in] SystemTable    A pointer to the EFI System Table.
  
  cmd = LOG_PRINT           print log.
  cmd = LOG_AVER_FORMAT     send incorrect format.  
  cmd = LOG_COMPARE         test log entry to existing data 
  @retval EFI_SUCCESS       The entry point is executed successfully.
  @retval other             Some error occurs when executing this entry point.

**/
EFI_STATUS GetEventLog(int cmd)
{
  EFI_STATUS              Status;
  TREE_EVENT_LOG_FORMAT   EventLogFormat = TREE_EVENT_LOG_FORMAT_TCG_2; 
  EFI_PHYSICAL_ADDRESS    EventLogLocation;
  EFI_PHYSICAL_ADDRESS    LogLoc;
  EFI_PHYSICAL_ADDRESS    EventLogLastEntry;
  BOOLEAN                 EventLogTruncated;
  //UINT32                  FixedSize = sizeof(TCG_PCRINDEX)+sizeof(TCG_EVENTTYPE)+sizeof(TPML_DIGEST_VALUES)+sizeof(UINT32);
  TCG_PCR_EVENT2         **pEvent;                 
  UINT32                  i;
  UINT32                  EventSize;

  // New
  UINT32 NewEntrySize;
  UINT8  *digestPtr;
  UINT8  *p;
  UINT32 *eventSizePtr;

  if (LOG_AVER_FORMAT == cmd)
    EventLogFormat = TREE_EVENT_LOG_FORMAT_TCG_1_2;

  Status = pEfiTrEEProtocol->GetEventLog(
        pEfiTrEEProtocol,
        EventLogFormat, 
       &EventLogLocation,
       &EventLogLastEntry,
       &EventLogTruncated
      );
  if ( EFI_ERROR (Status) )
  {
    AsciiPrint("Status = 0x%x, exiting.\r\n", Status);
    goto ErrorExit;
  }

  if (LOG_COMPARE == cmd)
  {
    LogLoc = EventLogLocation + sizeof(TCG_PCRINDEX) + sizeof(TCG_EVENTTYPE) + sizeof(TCG_DIGEST) + sizeof(UINT32) + 
             sizeof(TCG_EfiSpecIDEventStruct) + sizeof(UINT32) + (sizeof(TCG_EfiSpecIdEventAlgorithmSize)) + sizeof(UINT8);
    pEvent = (TCG_PCR_EVENT2 **)&LogLoc;
    p=(*pEvent)->Digest.digests[0].digest.sha256;
    AsciiPrint("Start pDataToHash compare...\r\n");
    if (0==memcmp(p, pOutData, 16) ) 
        AsciiPrint("pDataToHash compare PASSED \r\n");
    else
        AsciiPrint("pDataToHash compare ***FAILED*** \r\n");
   goto ErrorExit;
  }
  // print result.
  AsciiPrint(" EventLogLocation  = 0x%0lx\r\n", EventLogLocation);
  AsciiPrint(" EventLogLastEntry = 0x%0lx\r\n", EventLogLastEntry);
  AsciiPrint(" EventLogTruncated = 0x%x\r\n", EventLogTruncated);
  AsciiPrint("\r\n");

  if( EventLogLocation != 0 )
  {
    // Ignore the header-event
    EventLogLocation += sizeof(TCG_PCRINDEX) + sizeof(TCG_EVENTTYPE) + sizeof(TCG_DIGEST) + sizeof(UINT32) +
                        sizeof(TCG_EfiSpecIDEventStruct) + sizeof(UINT32) + (sizeof(TCG_EfiSpecIdEventAlgorithmSize)) + sizeof(UINT8);

    while( EventLogLocation <= EventLogLastEntry )
    {
      pEvent = (TCG_PCR_EVENT2 **)&EventLogLocation;

      AsciiPrint(" PCRIndex  = 0x%x\r\n", (*pEvent)->PCRIndex);
      AsciiPrint(" EventType = 0x%x\r\n", (*pEvent)->EventType);
      AsciiPrint(" Digests.count = 0x%x\r\n", (*pEvent)->Digest.count);
      AsciiPrint(" Digests.digests[0].hashAlg = 0x%x\r\n", (*pEvent)->Digest.digests[0].hashAlg);
      AsciiPrint(" Digest    =");
      for(i=0; i<32; i++)   // SHA256 size is 32
      {
        AsciiPrint("0x%x",   (*pEvent)->Digest.digests[0].digest.sha256[i]);
      }
      AsciiPrint("\r\n");

      digestPtr = (UINT8*)&((*pEvent)->Digest.digests[0].digest.sha256);
      eventSizePtr = (UINT32*)(digestPtr + 32/*HASH_SHA256_LEN*/);
      //eventPtr = (UINT8*)eventSizePtr + sizeof(UINT32);
      EventSize = *(UINT32*)eventSizePtr;
      AsciiPrint(" EventSize = 0x%x\r\n", EventSize);
      
      NewEntrySize = sizeof(UINT32/*TCG_PCRINDEX*/) + sizeof(UINT32/*TCG_EVENTTYPE*/) + sizeof(UINT32) + 
                     sizeof(UINT16) + 32/*HASH_SHA256_LEN*/ + sizeof(UINT32) + EventSize;
      EventLogLocation += NewEntrySize;
    }
  }

ErrorExit:
  return Status;
}

#define dTPM   0x6454504D
#define fTPM   0x6654504D

EFI_STATUS TestTpmMinkApi()
{
   EFI_STATUS   Status;

   UINT8   AppName[] = "qcom.tz.winsecapp";
   UINT32  AppId     = 0;
   UINT32  TpmType   = 0;

   TESTSRV_TPM_MINK_API_REQ  Request;
   TESTSRV_TPM_MINK_API_RSP  Response;

   Request.cmdId   = TESTSRV_TPM_MINK_API;
   Response.status = -0xFFFF;

   // get TPM type, either fTPM or dTPM
   Status = pTPMProtocol->GetTpmType(pTPMProtocol, &TpmType);
   if (EFI_ERROR(Status))
   {
      AsciiPrint( " GetTpmType() failed, Status = 0x%x \r\n", Status );
      return Status;
   }  
   else if( TpmType != dTPM  && TpmType != fTPM )
   {
      AsciiPrint( " Not dTPM, Not fTPM,  TpmType = 0x%x \r\n", TpmType );
      return Status;
   }

   //get AppId
   Status = pScmProtocol->ScmSendCommand( pScmProtocol, APP_QUERY_APPID_CMD, &AppId, AppName, sizeof(AppName), NULL,  0 );
   if (EFI_ERROR(Status))
   {
      AsciiPrint("winsecapp app id fialed \n" );
      return Status;
   }

   Status = pScmProtocol->ScmSendCommand(pScmProtocol,
                                         APP_SEND_DATA_CMD,
                                         &AppId,
                                         &Request,
                                         sizeof(TESTSRV_TPM_MINK_API_REQ),
                                         &Response,
                                         sizeof(TESTSRV_TPM_MINK_API_RSP));

   if (EFI_ERROR(Status) || Response.status != 0 )
   {
      AsciiPrint(" Mink API test failed, Status = 0x%x  Response.status %d \n" , Status, Response.status );
      return (Status != 0)? Status:Response.status;
   }
   else
      AsciiPrint(" Mink API test pass\n");

   if( TpmType != dTPM )
      goto END;
 
   Request.cmdId   = TESTSRV_TPM_NV_COUNTER;
   Response.status = -0xFFFF;

   Status = pScmProtocol->ScmSendCommand(pScmProtocol,
                                         APP_SEND_DATA_CMD,
                                         &AppId,
                                         &Request,
                                         sizeof(TESTSRV_TPM_MINK_API_REQ),
                                         &Response,
                                         sizeof(TESTSRV_TPM_MINK_API_RSP));

   if (EFI_ERROR(Status) || Response.status != 0 )
   {
      AsciiPrint(" NV Counter test failed, Status = 0x%x  Response.status %d \n" , Status, Response.status );
      return (Status != 0)? Status:Response.status;
   }
   else
      AsciiPrint(" NV Counter test pass\n");
       
END:
   return Status;
}


EFI_STATUS
EFIAPI
TrEEAppMain (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
   EFI_STATUS         Status, st = 0;

   TREE_BOOT_SERVICE_CAPABILITY    Capability;
   TrEE_EVENT                     *pTreeEvent = NULL;
   UINT64 TreeFlags = 0;
   UINT32 EventDataSize=DataToHashLen+4;
   UINT32 EventSize;

   UINT32 TpmAppId;
   int i=0;

   TPM_GET_RANDOM_REQ tpmReq;
   TPM_GET_RANDOM_RSP tpmRsp;
   UINT32 rndNum;
   
   #define TPM_ACPI_TABLE_SIGNATURE    0x324D5054
   #define TPM_COMMAND_SIZE            0x1000
   #define TPM_RESPONSE_SIZE           0x1000
   #define TPM_BUFFER_SIZE             ((sizeof(TPM2_ACPI_CONTROL_AREA)) + (TPM_COMMAND_SIZE) + (TPM_RESPONSE_SIZE))
    #define MAX_EVENT_LOG_SIZE 65536

   TEST_START("TrEEApp");

   Status = gBS->LocateProtocol(&gEfiTrEEProtocolGuid, NULL, (VOID **) &pEfiTrEEProtocol);
   if ( EFI_ERROR (Status) )
   {
      AsciiPrint("ERROR: Locate TrEE protocol failed, Status = 0x%x\r\n", Status);
      goto ErrorExit;
   }

   Status = gBS->LocateProtocol ( &gEfiTpmProtocolGuid, NULL, (VOID **)&pTPMProtocol);
   if ( EFI_ERROR (Status) )
   {
      DEBUG(( EFI_D_ERROR, " Locate TPM Protocol failed, Status = 0x%x\r\n", Status));
      goto ErrorExit;
   }

   Status = gBS->LocateProtocol (&gQcomScmProtocolGuid, NULL, (VOID**)&pScmProtocol);
   if ( EFI_ERROR (Status) )
   {
      DEBUG(( EFI_D_ERROR, " Locate Scm Protocol failed, Status =  0x%x\r\n", Status));
      goto ErrorExit;
   }

   Status = TestTpmMinkApi();
   if ( EFI_ERROR (Status) )
   {
      DEBUG(( EFI_D_ERROR, " TPM Mink Api test failed, Status =  0x%x\r\n", Status));
//      goto ErrorExit;
   }

   Capability.Size = sizeof(TREE_BOOT_SERVICE_CAPABILITY);
   Status = pEfiTrEEProtocol->GetCapability(pEfiTrEEProtocol, &Capability);
   if ( EFI_ERROR (Status) )
   {
      AsciiPrint("ERROR: GetCapability failed, Status = 0x%x\r\n", Status);
      goto ErrorExit;
   }

   // print capability
   AsciiPrint("Size = (%d)\r\n", Capability.Size);
   AsciiPrint("StructureVersion Major = (%d)\r\n",   Capability.StructureVersion.Major);
   AsciiPrint("StructureVersion Minor = (%d)\r\n",   Capability.StructureVersion.Minor);
   AsciiPrint("ProtocolVersion Major  = (%d)\r\n",   Capability.ProtocolVersion.Major);
   AsciiPrint("ProtocolVersion Minor  = (%d)\r\n",   Capability.ProtocolVersion.Minor);
   AsciiPrint("HashAlgorithmBitmap    = (%d)\r\n",   Capability.HashAlgorithmBitmap);
   AsciiPrint("SupportedEventLogs     = (%d)\r\n",   Capability.SupportedEventLogs);
   AsciiPrint("TrEEPresentFlag        = (%d)\r\n",   Capability.TrEEPresentFlag);
   AsciiPrint("MaxCommandSize         = (%d)\r\n",   Capability.MaxCommandSize);
   AsciiPrint("MaxResponseSize        = (%d)\r\n",   Capability.MaxResponseSize);
   AsciiPrint("ManufacturerID         = (0x%x)\r\n", Capability.ManufacturerID);
   AsciiPrint("NumberOfPcrBanks       = (0x%x)\r\n", Capability.NumberOfPcrBanks);
   AsciiPrint("ActivePcrBanks         = (0x%x)\r\n", Capability.ActivePcrBanks);

   //add HashLogExtendEvent setup
   // allocate a pool for this TreeEvent
   EventSize = sizeof(UINT32) + sizeof(TrEE_EVENT_HEADER) + EventDataSize;
   pTreeEvent = (TrEE_EVENT *)AllocatePool( EventSize);
   if( pTreeEvent == NULL )
   {
      Status = EFI_OUT_OF_RESOURCES;
      goto ErrorExit;
   }
   // populate TreeEventHeader
   pTreeEvent->Header.HeaderSize = sizeof(TrEE_EVENT_HEADER);
   pTreeEvent->Header.HeaderVersion = 1;
   pTreeEvent->Header.PCRIndex = 0;
   pTreeEvent->Header.EventType = EV_EFI_HANDOFF_TABLES;
   // populate TreeEvent.Event
   SetMem( pTreeEvent->Event, EventDataSize, 0);
   pTreeEvent->Size = EventSize;

   // now run HashLogExtendEvent
   AsciiPrint("Start HashLogExtendEvent...\r\n");
   Status = pEfiTrEEProtocol->HashLogExtendEvent(
             pEfiTrEEProtocol, 
             TreeFlags,
             ( EFI_PHYSICAL_ADDRESS ) pDataToHash,
             DataToHashLen,
             pTreeEvent );
 
   AsciiPrint("End HashLogExtendEvent Status Status = \r\n", Status);
   AsciiPrint("Start GetEventLog...\r\n");

   Status = GetEventLog(LOG_PRINT);
   AsciiPrint("End GetEventLog, Status = 0x%x \r\n", Status);
   if ( EFI_ERROR (Status) )
   {
      AsciiPrint("ERROR: GetEventLog failed, Status = 0x%x\r\n", Status);
      goto ErrorExit;
   }
   // Commpare pDataToHash data
   st=GetEventLog(LOG_COMPARE);
 
   //Send wrong format to log
   AsciiPrint("Start GetEventLog Adversarial...\r\n");
   Status = GetEventLog(LOG_AVER_FORMAT);
   if( EFI_INVALID_PARAMETER == Status )
      AsciiPrint("GetEventLog Adversarial PASSED, Status = 0x%x \r\n", Status);
   else
   {
      AsciiPrint("GetEventLog Adversarial ***FAILED***, Status = 0x%x \r\n", Status);
      st=Status;
   }

   //GetTpmAppId
   AsciiPrint("Start GetTpmAppId...\r\n");
   Status = pTPMProtocol->GetTpmAppId(pTPMProtocol, &TpmAppId);
   if (EFI_SUCCESS == Status)
      AsciiPrint("GetTpmAppId = 0x%x PASSED.\r\n", TpmAppId);
   else 
   {
      AsciiPrint("GetTpmAppId ***FAILED*** Status Status = 0x%x, TpmAppId= 0x%x \r\n", Status, TpmAppId);
      st=Status;
   }

   //SubmitCommand for random number command
   memset(&tpmReq,0,sizeof(tpmReq) );
   memset(&tpmRsp,0,sizeof(tpmRsp) );
   tpmReq.commandHeader.tag = SwapBytes16(TPM_ST_NO_SESSIONS);
   tpmReq.commandHeader.paramSize = SwapBytes32(sizeof(tpmReq));
   tpmReq.commandHeader.commandCode = SwapBytes32(TPM_CC_GetRandom);
   tpmReq.params.bytesRequested = SwapBytes16(sizeof(tpmRsp.RandomData));

   Status = pEfiTrEEProtocol->SubmitCommand(pEfiTrEEProtocol, sizeof(tpmReq), (UINT8*)&tpmReq,   sizeof(tpmRsp), (UINT8*)&tpmRsp);
   i = SwapBytes32(tpmRsp.responseHeader.responseCode);
   rndNum=tpmRsp.RandomData;
   if (Status)
      AsciiPrint("SubmitCommand for random number Status = 0x%x, response code= 0x%x #= 0x%x ***FAILED*** \r\n", Status, i, tpmRsp.RandomData);
   else
      AsciiPrint("SubmitCommand for random number, Status = 0x%x,response code= 0x%x   rand #= 0x%x \r\n", Status, i, tpmRsp.RandomData);
	
   Status = pEfiTrEEProtocol->SubmitCommand(pEfiTrEEProtocol, sizeof(tpmReq), (UINT8*)&tpmReq,  sizeof(tpmRsp), (UINT8*)&tpmRsp);
   i = SwapBytes32(tpmRsp.responseHeader.responseCode);
   if (Status)
      AsciiPrint("SubmitCommand for random number Status = 0x%x, response code= 0x%x #= 0x%x ***FAILED*** \r\n", Status, i, tpmRsp.RandomData);
   else
      AsciiPrint("SubmitCommand for random number, Status = 0x%x,response code= 0x%x   rand #= 0x%x \r\n", Status, i, tpmRsp.RandomData);

   if (tpmRsp.RandomData == rndNum){
      AsciiPrint("SubmitCommand for random number not rand    ***FAIL*** \r\n");
      st=1;
   }
   else
      AsciiPrint("SubmitCommand for random number PASSED \r\n");

#if 1
   //Do Hash a few more times so the log fills up: 
   //add back in 
   AsciiPrint("Start Full Log test... \r\n");
   for (i=0; i<(MAX_EVENT_LOG_SIZE+2); i++)
   {
         pDataToHash[0]=i;
         pDataToHash[1]=i+1;
         pDataToHash[2]=i+2;
         pDataToHash[3]=i+3;
         Status = pEfiTrEEProtocol->HashLogExtendEvent(pEfiTrEEProtocol, TreeFlags, 
                                          (EFI_PHYSICAL_ADDRESS) pDataToHash, DataToHashLen, pTreeEvent); 
         //AsciiPrint("%d, End HashLogExtendEvent Status = 0x%x \r\n", i, Status);
         if (EFI_VOLUME_FULL == Status ) 
         break;
   }

   if ((MAX_EVENT_LOG_SIZE+2) == i)
   {
      AsciiPrint("Full Log test ***FAILED***, Status = 0x%x, entries=%d \r\n", Status, i);
      st = EFI_VOLUME_FULL;
   }
   else
   {
      AsciiPrint("Full Log test PASSED, Status = 0x%x, entries=%d \r\n", Status, i);
      st = EFI_SUCCESS;
   }
//maybe add a reset.
#else
   AsciiPrint("Full Log test not run, Status = 0x%x, entries=%d \r\n", Status, i);
#endif

   Status = st;

ErrorExit:
   if( pTreeEvent != NULL )
   {
      FreePool(pTreeEvent);
      pTreeEvent = NULL;
   }

   TestStatus("TrEEApp", Status);
   TEST_STOP("TrEEApp");
   
   ReadAnyKey (NULL, 0);
   gRT->ResetSystem(EfiResetCold, EFI_SUCCESS, 0, NULL);

   return Status;
}
