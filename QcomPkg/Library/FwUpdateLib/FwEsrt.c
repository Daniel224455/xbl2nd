/** @file FwESRT.c

   Functions to maintain the ESRT , including ESRT header and ESRT entry .

  Copyright (c) 2012-2018 Qualcomm Technologies, Inc.  All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential.

**/
/*=============================================================================
                              EDIT HISTORY
  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.



 when           who         what, where, why
 2018/04/23     prema       Handled to update ESRT info when DPP is inaccessible
                            and DPP corruption, and not to bail out.
 2018/02/15     prema       Using ChipInfo protocol to get PlatformInfo
                            instead of harcoded chip information
 2018/02/13     prema       Added check to handle 850 chipset in CapsuleUpdate
 2018/01/31     prema       Updated System Firmware Resource GUIDs for 845
 2017/07/13     gpavithr    Changes to silent error logs when DPP media is not available
 2017/07/07     hhovale     Change to not skip SystemFmpCapsuleImageTypeIdGuid update 
                            when DPP not found.
 2017/06/01     gparam      Change to initialize cached ESRT data every time when
                            a system firmware update is attempted.
 2017/05/12     gparam      Removed the use of dynamic PCD and used boot-time variable instead
 2017/05/08     gparam      Changes to centralize ESRT update to DPP to the end of
                            SetImage API
 2017/04/18     gparam      Changes to centralize the recording of update status
                            in DPP for future use
 2017/04/18     gparam      Changed the update status from internal type to
                            standard ones defined in EDK headers + Removed
                            unused/redundant definitions/parameters
 2017/04/18     gparam      Change to update PCD containing SystemFirmware GUID list
                            with the currently active SystemFirmware GUID read from DPP
 2017/03/15     gparam      Corrected the ESRT read from DPP to honor the ESRT
                            provision backup file which will be present if device
                            resets/reboots between update of ESRT GUID.
 2017/02/27     pkn         Added changes required to support new update type - FWCLASS_GUID
 2017/02/24     gparam      Changes to correct the prefilled known SystemFirmware
                            GUIDs for the default Qualcomm form-factors
 2017/02/22     gparam      Added the default 8998 specific system firmware GUIDs and
                            removed 8996 GUID
 2017/02/13     gpavithr    Changes to use ESRTDxe inplace of local functions
                            to directly create and/or operate on ESRT table
 2017/02/01     pkn         Move ESRT system firmware GUID provisioned in DPP to a separate DPP file
 2016/08/29     ltg         Replace EFI_PLATFORMINFO_TYPE_MTP_MSM with EFI_PLATFORMINFO_TYPE_MTP
 2/15/04/16     mic         Use updated DppProtocol
 2015/03/31     mic         Added 8994 platform types
 2013/06/11     rsb         Added 8074 platform types
 2013/04/11     rsb         Added 8084 platform types
 2012/12/11     mic         Fixed KW warnings
 2012/11/30     mic         Added ESRT entry print function
 2012/11/09     jmb         Adding CBH platform
 2012/10/21     mic         Code review
 2012/10/03     ash         Fixed version matching for 8064 platforms.
 2012/09/19     rsb         Added 8974 platform types
 2012/08/14     ash         Added 8064 WB292 and V1 variants.
 2012/06/21     rs          Adding support for FPDT ACPI table
 2012/06/21     mic         Set the default ESRT FW ver/lowest support version/last attempt version to 0
 2012/06/05     rs          Enhanced lowest supported version check
 2012/05/09     rsb         Platform run-time type detection for FW Resource GUID
 2012/05/07     jd          Migrated to DPP Protocol.
 2012/02/01     mic         Initial version

=============================================================================*/
#include <Uefi.h>
#include <Protocol/EsrtManagement.h>
#include <Library/PcdLib.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>

#include <Include/Library/FwCommonLib.h>
#include <Include/Library/QcomLib.h>

#include <Pi/PiDxeCis.h>
#include <Protocol/SimpleFileSystem.h>
#include <Protocol/BlockIo.h>
#include <Protocol/FirmwareVolume2.h>
#include <Protocol/EFIChipInfo.h>
#include <Protocol/EFIPlatformInfo.h>
#include <Protocol/EFIDpp.h>

#include <Guid/GlobalVariable.h>

#include "FwEsrt.h"
#include "FwCapsule.h"
#include "FwPayloadEntry.h"

#define DEFAULT_ESRT_CAPSULE_FLAGS 0x0
#define ESRT_FWCLASS_PROVISIONING_FILE_NAME L"ESRT_SYS_FWClass.PROVISION"
#define ESRT_FWENTRY_PROVISIONING_FILE_NAME L"ESRT_SYS_FWEntry.PROVISION"
#define ESRT_FWCLASS_PROVISIONING_FILE_NAME_BKP L"ESRT_SYS_FWClass.PROVISION.bkp"

#define ESRT_PROVISIONING_DATA_TYPE L"QCOM"

/*global variable*/
UINT16                  *esrtfwclassfilename = ESRT_FWCLASS_PROVISIONING_FILE_NAME;
UINT16                  *esrtfwentryfilename = ESRT_FWENTRY_PROVISIONING_FILE_NAME;
UINT16                  *esrtfwclassfilenamebkp = ESRT_FWCLASS_PROVISIONING_FILE_NAME_BKP;

UINT16                  *ProvisiondataType = ESRT_PROVISIONING_DATA_TYPE;
BOOLEAN                  bIsESRTInDppAccessible  = TRUE;
const EFI_SYSTEM_RESOURCE_ENTRY DefaultEsrtEntry = {
  QCOM_FW_UPDATE_SYSTEM_FW_UNKNOWN_GUID,
  FW_TYPE_SYSTEM,
  0x0,
  0x0,
  DEFAULT_ESRT_CAPSULE_FLAGS,
  0x0,
  LAST_ATTEMPT_STATUS_SUCCESS
};
EFI_SYSTEM_RESOURCE_ENTRY esrt_ent = {
  QCOM_FW_UPDATE_SYSTEM_FW_UNKNOWN_GUID,
  FW_TYPE_SYSTEM,
  0x0,
  0x0,
  DEFAULT_ESRT_CAPSULE_FLAGS,
  0x0,
  LAST_ATTEMPT_STATUS_SUCCESS
};
EFI_GUID                  ESRT_guid = QCOM_FW_UPDATE_ESRT_GUID;

/* The list of System Firmware Resource GUIDs that apply for each type of platform, the order must
   match the QCOM_FW_RESOURCE_TYPE. */
const EFI_GUID qcomFwResourceGUIDs[] =
{
    QCOM_FW_UPDATE_SYSTEM_FW_845_MTP_GUID,
    QCOM_FW_UPDATE_SYSTEM_FW_845_CLS_GUID,
    QCOM_FW_UPDATE_SYSTEM_FW_845_QRD_GUID,
    QCOM_FW_UPDATE_SYSTEM_FW_UNKNOWN_GUID
};

/**
  Return if this FMP is a system FMP or a device FMP, based upon FmpImageInfo.

  @param[in] FmpImageInfo A pointer to EFI_FIRMWARE_IMAGE_DESCRIPTOR

  @return TRUE  It is a system FMP.
  @return FALSE It is a device FMP.
**/
EFI_STATUS 
UpdateVarSystemFmpCapsuleImageTypeIdGuid (
  IN EFI_GUID  SystemFwGuid
  )
{
  EFI_STATUS Status         = EFI_SUCCESS;
  GUID      *Guid           = NULL;
  UINTN      Count          = 0;
  UINTN      Index          = 0;
  GUID      *NewGuidList    = NULL;
  UINTN      NewGuidListSize = sizeof(GUID);
  VOID      *GuidBuffer     = NULL;
  UINTN      GuidBufferSize = 0;

  Status = GetEfiGlobalVariable2 (
             L"SystemFmpCapsuleImageTypeIdGuid",
             &GuidBuffer,
             &GuidBufferSize
             );
  if (!EFI_ERROR(Status) &&
      (GuidBuffer != NULL)) {

    Guid = (GUID *)GuidBuffer;
    Count = GuidBufferSize/sizeof(GUID);

    for (Index = 0; Index < Count; Index++) {
      if (CompareGuid(&SystemFwGuid, (Guid + Index))) {
        return EFI_SUCCESS;
      }
    }
  } else {
    Status = EFI_SUCCESS;
  }

  NewGuidListSize = (Count + 1) * sizeof(GUID);
  NewGuidList = AllocateZeroPool(NewGuidListSize);
  if (NewGuidList == NULL) {
    DEBUG ((EFI_D_ERROR, "FmpDxe: Allocating new PcdSystemFmpCapsuleImageTypeIdGuid failed\n"));
    return EFI_OUT_OF_RESOURCES;
  }

  CopyMem(
    NewGuidList,
    Guid,
    (Count * (sizeof(GUID)))
    );
  CopyMem(
    (NewGuidList + Count),
    &SystemFwGuid,
    sizeof(GUID)
    );

  //
  // Update New Repository
  //
  Status = gRT->SetVariable(
    L"SystemFmpCapsuleImageTypeIdGuid",
    &gEfiGlobalVariableGuid,
    EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS,
    NewGuidListSize,
    NewGuidList
    );

  if (EFI_ERROR(Status)) {
    DEBUG ((EFI_D_ERROR, "FmpDxe: Setting of new GUID list in PcdSystemFmpCapsuleImageTypeIdGuid failed with error %r\n", Status));
    return Status;
  }

  return EFI_SUCCESS;
}

/**
 * Function to print ESRT entry
 *
 * @param ent  - ESRT entry
 * @DebugLevel - Debug level
 *
 * @return : EFI_STATUS
 */
EFI_STATUS
QcomPrintEsrtEntry(
  IN EFI_SYSTEM_RESOURCE_ENTRY  *ent,
  IN UINTN                      DebugLevel
  )
{

  DEBUG((DebugLevel,"    Esrt info:\n"));
  DEBUG((DebugLevel,"    ====================================\n"));

  DEBUG((DebugLevel,"    GUID                     :"));
  FwCommonPrintGuid(ent->FwClass,DebugLevel);
  DEBUG((DebugLevel,"\n"));

  DEBUG((DebugLevel,"    FwType                   : 0x%x\n", ent->FwType));
  DEBUG((DebugLevel,"    FwVersion                : 0x%x\n", ent->FwVersion));
  DEBUG((DebugLevel,"    Lowest supported version : 0x%x\n", ent->LowestSupportedFwVersion));
  DEBUG((DebugLevel,"    Capsuleflag              : 0x%x\n", ent->CapsuleFlags));
  DEBUG((DebugLevel,"    Last attempt ver         : 0x%x\n", ent->LastAttemptVersion));
  DEBUG((DebugLevel,"    Last attempt status      : 0x%x\n", ent->LastAttemptStatus));

  DEBUG((DebugLevel,"    ====================================\n"));

  return EFI_SUCCESS;
}

/**
* Get the ESRT entry info
*
* @param  ent         - Copy of current ESRT entry
* @return EFI_STATUS
**/
EFI_STATUS
QcomGetEsrtEntryCache(
  OUT EFI_SYSTEM_RESOURCE_ENTRY  *ent
  )
{
  if(ent == NULL)
  {
    return EFI_INVALID_PARAMETER;
  }

  CopyMem ((VOID *)ent, (VOID*)&esrt_ent, sizeof (esrt_ent));

  return EFI_SUCCESS;
}

/**
 *  Check if ESRT header is installed in UEFI System table
 *
 * @param SystemTable -UEFI system table
 *
 * @return BOOLEAN
 */
BOOLEAN
QcomIsEsrtHeaderInstalledInSysTbl(
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_CONFIGURATION_TABLE *pcfg_table = NULL;
  UINTN                    cs_cnt     = 0;
  UINTN                    i          = 0;

  if (SystemTable == NULL)
  {
    return FALSE;
  }

  cs_cnt =SystemTable->NumberOfTableEntries;
  pcfg_table=SystemTable->ConfigurationTable;

  for ( i = 0; i < cs_cnt; i++)
  {
    if(CompareGuid((GUID*)&ESRT_guid,(GUID*)&pcfg_table->VendorGuid))
    {
      return TRUE;
    }
    pcfg_table++;
  }

    return FALSE;
}

/**
 * Read ESRT entry info from storage
 *
 * @param buf       - out buffer
 * @param esrtSize  - out buffer size
 *
 * @return EFI_STATUS
 */
EFI_STATUS
QcomRead_EsrtEntryFromStorage(
  IN OUT EFI_SYSTEM_RESOURCE_ENTRY  **buf,
  IN OUT UINTN                      *esrtSize
  )
{
  UINTN                      esrtFwClassSize        = 0;
  UINTN                      esrtFwEntrySize        = 0;
  EFI_STATUS                 Status                 = EFI_SUCCESS;
  EFI_DPP_PROTOCOL          *DppProtocol            = NULL;
  UINT16                    *esrtfwclassfilenamePtr = NULL;

  if((buf == NULL) ||
     (esrtSize == NULL))
  {
    return EFI_INVALID_PARAMETER;
  }

  *buf = NULL;
  Status = gBS->LocateProtocol(&gEfiDppProtocolGuid,
                               NULL,
                               (VOID**)&DppProtocol);
  if(EFI_ERROR(Status))
  {
    DEBUG((EFI_D_ERROR,"LocateProtocol(DPP) returned : %r. Returning alternative error %r\n", Status, EFI_NO_MEDIA));
    Status = EFI_NO_MEDIA;
    goto QcomRead_EsrtEntryFromStorageExit;
  }

  /* If ESRT bkp file exists, then read the GUID from bkp and use as this can
    happen only when the firmware update was interrupted in between and device
    rebooted before the payload is completely updated.

    If bkp file doesn't exist, no need to throw error */
  Status = DppProtocol->GetDPP(DppProtocol,ProvisiondataType,esrtfwclassfilenamebkp,NULL,&esrtFwClassSize);
  if(EFI_ERROR(Status))
  {
    DEBUG((EFI_D_WARN,"Using Fw GUID from default file %s\n", esrtfwclassfilename));

    esrtFwClassSize = 0;
    Status = DppProtocol->GetDPP(DppProtocol,ProvisiondataType,esrtfwclassfilename,NULL,&esrtFwClassSize);
    HANDLE_ERROR_LABEL(Status,DppProtocol->GetDPP,QcomRead_EsrtEntryFromStorageExit);

    esrtfwclassfilenamePtr = esrtfwclassfilename;
  }
  else
  {
    DEBUG((EFI_D_WARN,"System Fw GUID backup file %s available.\n", esrtfwclassfilenamebkp));
    DEBUG((EFI_D_WARN,"Using Fw GUID from backup file\n"));

    esrtfwclassfilenamePtr = esrtfwclassfilenamebkp;
  }

  Status = DppProtocol->GetDPP(DppProtocol,ProvisiondataType,esrtfwentryfilename,NULL,&esrtFwEntrySize);
  HANDLE_ERROR_LABEL(Status,DppProtocol->GetDPP,QcomRead_EsrtEntryFromStorageExit);

  if((esrtFwClassSize + esrtFwEntrySize) != sizeof(EFI_SYSTEM_RESOURCE_ENTRY))
  {
    Status = EFI_BAD_BUFFER_SIZE;
    DEBUG((EFI_D_ERROR,"Incorrect provision file size (%d + %d)!. Expected: %d\n", esrtFwClassSize, esrtFwEntrySize, sizeof(EFI_SYSTEM_RESOURCE_ENTRY)));
    goto QcomRead_EsrtEntryFromStorageExit;
  }

  *esrtSize = esrtFwClassSize + esrtFwEntrySize;

  Status = gBS->AllocatePool(EfiBootServicesData,*esrtSize,(VOID **)buf);
  HANDLE_ERROR_LABEL(Status,AllocatePool (EfiBootServicesData),QcomRead_EsrtEntryFromStorageExit);

  Status = DppProtocol->GetDPP(DppProtocol,ProvisiondataType,esrtfwclassfilenamePtr,*buf,&esrtFwClassSize);
  HANDLE_ERROR_LABEL(Status,GetDPP (Fw GUID file),QcomRead_EsrtEntryFromStorageExit);

  Status = DppProtocol->GetDPP(DppProtocol,ProvisiondataType,esrtfwentryfilename,((char *)*buf + esrtFwClassSize),&esrtFwEntrySize);
  HANDLE_ERROR_LABEL(Status,GetDPP (Fw Entry File),QcomRead_EsrtEntryFromStorageExit);

  return EFI_SUCCESS;

QcomRead_EsrtEntryFromStorageExit:

  if(*buf != NULL)
  {
    gBS->FreePool(*buf);
    *buf = NULL;
  }
  return Status;
}

/**
 * Write ESRT entry info into storage.
 *
 * @param inBuf           - buffer
 * @param esrtSize        - buffer size
 * @param cleanBackupFile - TRUE indicates to delete the backup file if found
 *                          instead of processing the file
 *                          FALSE indicates to process the file if found
 *
 * @return EFI_STATUS
 */
EFI_STATUS
QcomWrite_EsrtEntryToStorage(
  IN EFI_SYSTEM_RESOURCE_ENTRY  *inBuf,
  IN UINTN                       esrtSize,
  IN BOOLEAN                     cleanBackupFile
  )
{
  UINTN              esrtFwClassSize          = 0;
  UINTN              esrtFwEntrySize          = 0;
  UINTN              esrtFwClassBkpSize       = 0;
  UINT16            *esrtFwClassFilenameInUse = esrtfwclassfilename;
  EFI_STATUS         Status                   = EFI_SUCCESS;
  VOID              *EsrtBuffer               = NULL;
  EFI_DPP_PROTOCOL  *DppProtocol              = NULL;
  BOOLEAN            SkipFwGUIDUpdate         = FALSE;

  if(inBuf == NULL)
  {
    return EFI_INVALID_PARAMETER;
  }

  /*Create the new ESRT entry*/
  Status=gBS->AllocatePool(EfiBootServicesData,esrtSize,&EsrtBuffer);
  HANDLE_ERROR_LABEL(Status,gBS->AllocatePool,QcomWrite_EsrtEntryToStorageExit);

  /*Initialize the ESRT entry*/
  ZeroMem(EsrtBuffer,esrtSize);
  CopyMem(EsrtBuffer,(VOID*)inBuf,esrtSize);

  /*write the ESRT entry to DPP*/
  Status = gBS->LocateProtocol(&gEfiDppProtocolGuid,
                               NULL,
                               (VOID**)&DppProtocol);
  if(EFI_ERROR(Status))
  {
    DEBUG((EFI_D_ERROR,"LocateProtocol(DPP) returned : %r. Returning alternative error %r\n", Status, EFI_NO_MEDIA));
    Status = EFI_NO_MEDIA;
    goto QcomWrite_EsrtEntryToStorageExit;
  }

  Status = DppProtocol->GetDPP(DppProtocol,ProvisiondataType,esrtfwclassfilename,NULL,&esrtFwClassSize);
  if(0 == esrtFwClassSize)
  {
    esrtFwClassSize = sizeof(EFI_GUID);
  }

  Status = DppProtocol->GetDPP(DppProtocol,ProvisiondataType,esrtfwentryfilename,NULL,&esrtFwEntrySize);
  if(0 == esrtFwEntrySize)
  {
    esrtFwEntrySize = esrtSize - sizeof(EFI_GUID);
  }

  if((esrtFwClassSize + esrtFwEntrySize) != esrtSize)
  {
    Status = EFI_BAD_BUFFER_SIZE;
    HANDLE_ERROR_LABEL(Status,esrtSize,QcomWrite_EsrtEntryToStorageExit);
  }

  /*Write to bkp file during the update process*/
  Status = DppProtocol->GetDPP(DppProtocol,ProvisiondataType,esrtfwclassfilenamebkp,NULL,&esrtFwClassBkpSize);
  if((EFI_ERROR(Status)))
  {
    DEBUG((EFI_D_WARN,"Updating Fw GUID to default file %s\n", esrtfwclassfilename));
  }
  else
  {
    if (cleanBackupFile == TRUE)
    {
      DEBUG((EFI_D_WARN,"System Fw GUID backup file %s available.\n", esrtfwclassfilenamebkp));
      DEBUG((EFI_D_WARN,"Deleting the same and skipping Fw GUID write due to clean request\n"));

      Status = DppProtocol->DeleteItemDPP(DppProtocol,ProvisiondataType,esrtfwclassfilenamebkp);
      HANDLE_ERROR_LABEL(Status, DppProtocol->DeleteItemDPP, QcomWrite_EsrtEntryToStorageExit);

      SkipFwGUIDUpdate = TRUE;
    }
    else
    {
      DEBUG((EFI_D_WARN,"System Fw GUID backup file %s available.\n", esrtfwclassfilenamebkp));
      DEBUG((EFI_D_WARN,"Updating Fw GUID to backup file\n"));

      esrtFwClassFilenameInUse = esrtfwclassfilenamebkp;
    }
  }

  if (SkipFwGUIDUpdate == FALSE)
  {
    // We should not update the Fw GUID to FW CLass provision file if we find
    // a backup fw class file and a delete request has come as this might be
    // the point where the system is updating the FW GUID to a new value through
    // firmware update and the current call is part of the cleanup process of
    // the backup file generated before staring the update. Writing to the
    // default file with the FW GUID value will result in overwriting the new
    // GUID lying in the new copy of the default file brought-in by the
    // ongoing update.
    Status = DppProtocol->SetDPP(DppProtocol,ProvisiondataType,esrtFwClassFilenameInUse,EsrtBuffer,esrtFwClassSize);
    HANDLE_ERROR_LABEL(Status,DppProtocol->SetDPP,QcomWrite_EsrtEntryToStorageExit);\
  }

  Status = DppProtocol->SetDPP(DppProtocol,ProvisiondataType,esrtfwentryfilename,((char *)EsrtBuffer + esrtFwClassSize),esrtFwEntrySize);
  HANDLE_ERROR_LABEL(Status,DppProtocol->SetDPP,QcomWrite_EsrtEntryToStorageExit);

QcomWrite_EsrtEntryToStorageExit:

  if(EsrtBuffer != NULL)
  {
    gBS->FreePool(EsrtBuffer);
    EsrtBuffer = NULL;
  }

  return Status;

}

/**
 * This Function try to update ESRT in both DPP and system table
 *
 * @param  ent            - Copy of new ESRT entry to update in cache and DPP
 * @param cleanBackupFile - TRUE indicates to delete the backup file if found
 *                          instead of processing the file
 *                          FALSE indicates to process the file if found
 *
 * @return EFI_STATUS
 */

EFI_STATUS
QcomUpdateEsrtEntry(
  IN EFI_SYSTEM_RESOURCE_ENTRY  *ent,
  IN BOOLEAN                     cleanBackupFile
  )
{

  EFI_STATUS Status   = EFI_SUCCESS;
  UINTN      esrtSize = 0;

  /* Do not update the current version in ESRT cache to the new version
     as it needs a reboot to really take effect. Till then, any query to
     cache should return the current version as the version of the
     firmware which processed the current update */
  esrt_ent.LastAttemptVersion = ent->LastAttemptVersion;
  esrt_ent.LowestSupportedFwVersion = ent->LowestSupportedFwVersion;
  esrt_ent.LastAttemptStatus  = ent->LastAttemptStatus;

  DEBUG((EFI_D_INFO,"Updated cached ESRT:\n"));
  QcomPrintEsrtEntry(&esrt_ent,EFI_D_WARN);

  /*update ESRT in DPP*/
  esrtSize = sizeof(EFI_SYSTEM_RESOURCE_ENTRY);
  if (TRUE == bIsESRTInDppAccessible)
  {
    DEBUG((EFI_D_WARN,"Writing System ESRT Entry to DPP...\n"));
    Status = QcomWrite_EsrtEntryToStorage(ent, esrtSize, cleanBackupFile);
    if(EFI_ERROR(Status))
    {
     DEBUG((EFI_D_WARN,"QcomWrite_EsrtEntryToStorage failed. Returning: %r\n", Status));
    }
  }

  return Status;
}

/**
 *  Find if the given Firmware Resource GUID matches a known type
 *  used for Qualcomm platforms.
 *
 * @param FwResourceGuid - Pointer to GUID to match against known platform types
 * @param bFound         - Pointer to output boolean set to TRUE if a match is found,
 *                         or FALSE otherwise.
 *
 * @return EFI_STATUS
 */

EFI_STATUS
QcomIsFwGuidKnownPlatformType(
  IN EFI_GUID *FwResourceGuid,
  OUT BOOLEAN *bFound
  )
{
   EFI_STATUS Status     = EFI_SUCCESS;
   UINTN      numFwGuids = 0;
   UINTN      i          = 0;

   if ((FwResourceGuid == NULL) || (bFound == NULL))
   {
      return EFI_INVALID_PARAMETER;
   }

   numFwGuids = sizeof(qcomFwResourceGUIDs)/sizeof(qcomFwResourceGUIDs[0]);

   *bFound = FALSE;

   for (i = 0; i < numFwGuids; i++)
   {
      if (CompareGuid((GUID *)FwResourceGuid, (GUID *)&qcomFwResourceGUIDs[i]) == TRUE)
      {
         *bFound = TRUE;
         break;
      }
   }

   return Status;
}

/**
 *  Find the Firmware Resource GUID for the device platform.
 *
 * @param FwResourceGuid - Pointer to GUID that is modified to correspond to the current
 *                         platform type.
 *
 * @return EFI_STATUS
 */

EFI_STATUS
QcomGetFwGuidForPlatform(
  OUT EFI_GUID *FwResourceGuid
  )
{
  EFI_STATUS                           Status                = EFI_SUCCESS;
  EFI_CHIPINFO_PROTOCOL               *mChipInfoProtocol     = NULL;
  EFI_PLATFORMINFO_PROTOCOL           *pPlatformInfoProtocol = NULL;
  EFI_PLATFORMINFO_PLATFORM_INFO_TYPE  platformInfo;
  EFIChipInfoIdType                    eId                   = EFICHIPINFO_ID_UNKNOWN;
  EFIChipInfoVersionType               nChipVersion          = 0;

  if (FwResourceGuid == NULL)
  {
     return EFI_INVALID_PARAMETER;
  }

  Status = gBS->LocateProtocol (&gEfiChipInfoProtocolGuid,
                                NULL,
                                (VOID **) &mChipInfoProtocol
                               );
  if(EFI_ERROR(Status))
  {
    DEBUG((EFI_D_ERROR,"LocateProtocol(CHIPINFO_PROTOCOL) returned Status:%r\n", Status));
    goto QcomGetFwGuidForPlatformExit;
  }

  Status = mChipInfoProtocol->GetChipId( mChipInfoProtocol,
                                                     &eId);
  HANDLE_ERROR_LABEL(Status,mChipInfoProtocol->GetChipId,QcomGetFwGuidForPlatformExit);

  Status = mChipInfoProtocol->GetChipVersion( mChipInfoProtocol,
                                                 &nChipVersion);
  HANDLE_ERROR_LABEL(Status,mChipInfoProtocol->GetChipVersion,QcomGetFwGuidForPlatformExit);

  // Get the platform info protocol.  It is used to map rows and scan columns to
  // specific keys which can vary per platform.
  Status = gBS->LocateProtocol ( &gEfiPlatformInfoProtocolGuid,
                                 NULL,
                                 (VOID **)&pPlatformInfoProtocol
                                );
  if(EFI_ERROR(Status))
  {
    DEBUG((EFI_D_ERROR,"LocateProtocol(PLATFORMINFO_PROTOCOL) returned Status:%r\n", Status));
    goto QcomGetFwGuidForPlatformExit;
  }

  ZeroMem(&platformInfo, sizeof(platformInfo));

  Status = pPlatformInfoProtocol->GetPlatformInfo( pPlatformInfoProtocol,
                                                  &platformInfo);
  HANDLE_ERROR_LABEL(Status,pPlatformInfoProtocol->GetPlatformInfo,QcomGetFwGuidForPlatformExit);

  if ( (EFICHIPINFO_ID_SDM845 == eId) || (EFICHIPINFO_ID_SDM850 == eId) )
  {
	EFI_STATUS         Status;
	CHAR8              DestBuffer[128];
	CHAR8*             PlatTypePtr;
	CHAR8*             ChipNamePtr;
	CHAR8*             ChipVerPtr;
  
	Status = GetPlatformStrings (DestBuffer, sizeof(DestBuffer),
                               &PlatTypePtr, &ChipNamePtr, &ChipVerPtr);

    DEBUG((EFI_D_WARN,"Platform: %a %a\n", ChipNamePtr, PlatTypePtr));

    /* Detected 845/850 */
    if (platformInfo.platform == EFI_PLATFORMINFO_TYPE_MTP)
    {
      /* 845/850 MTP platform */
      CopyMem(FwResourceGuid, &qcomFwResourceGUIDs[QCOM_FW_UPDATE_SYSTEM_FW_845_MTP_TYPE], sizeof(EFI_GUID));
    }
    else if (platformInfo.platform == EFI_PLATFORMINFO_TYPE_CLS)
    {
      /* 845/850 CLS platform */
      CopyMem(FwResourceGuid, &qcomFwResourceGUIDs[QCOM_FW_UPDATE_SYSTEM_FW_845_CLS_TYPE], sizeof(EFI_GUID));
    }
    else if (platformInfo.platform == EFI_PLATFORMINFO_TYPE_QRD)
    {
      /* 845/850 QRD platform */
      CopyMem(FwResourceGuid, &qcomFwResourceGUIDs[QCOM_FW_UPDATE_SYSTEM_FW_845_QRD_TYPE], sizeof(EFI_GUID));
    }
    else
    {
      /* Unknown 845/850 platform */
      DEBUG((EFI_D_WARN,"System firmware GUID not known for current platform (%a:%d). Using default firmware GUID.\n", PlatTypePtr, platformInfo.platform));
      CopyMem(FwResourceGuid, &qcomFwResourceGUIDs[QCOM_FW_UPDATE_SYSTEM_FW_UNKNOWN_TYPE], sizeof(EFI_GUID));
    }
  }
  else
  {
    /* Unknown chipset */
    DEBUG((EFI_D_WARN,"Platform: Unknown chipset: %d\n", eId));
    CopyMem(FwResourceGuid, &qcomFwResourceGUIDs[QCOM_FW_UPDATE_SYSTEM_FW_UNKNOWN_TYPE], sizeof(EFI_GUID));
  }

QcomGetFwGuidForPlatformExit:

  return Status;
}

/**
 * Entry function for ESRT maintainence.
 *
 * @param SystemTable
 *
 * @return EFI_STATUS
 **/
EFI_STATUS
QcomESRTPhaseMain(
  )
{
  EFI_STATUS                  Status             = EFI_SUCCESS;
  EFI_SYSTEM_RESOURCE_ENTRY  *buf                = NULL;
  UINTN                       esrtSize           = 0;
  BOOLEAN                     bIsQcomFwGuid      = FALSE;
  EFI_GUID                    QcomFwPlatformGuid;

  CopyMem(&esrt_ent, &DefaultEsrtEntry, sizeof(esrt_ent));

  /*Read ESRT entry from DPP*/
  esrtSize = sizeof(EFI_SYSTEM_RESOURCE_ENTRY);
  DEBUG((EFI_D_WARN,"\nReading System ESRT Entry from DPP...\n"));
  Status = QcomRead_EsrtEntryFromStorage(&buf, &esrtSize);

  if(Status != EFI_SUCCESS)
  {
    if(Status == EFI_NO_MEDIA)
    {
      bIsESRTInDppAccessible = FALSE;
    }

    if (Status == EFI_NO_MEDIA || Status == EFI_NOT_FOUND)
    {
      DEBUG((EFI_D_WARN,"System ESRT Entry not found.\n"));
      Status = EFI_SUCCESS;
    }

    if(buf != NULL)
    {
      gBS->FreePool(buf);
      buf = NULL;
    }
    /* Write the initial DPP ESRT entry using the correct Firmware Resource GUID assigned to the
       given platform type. If it fails getting the platform type, the esrt_ent already contains
       UNKNOWN platform GUID. */
    DEBUG((EFI_D_WARN,"Detecting platform type\n"));
    if (QcomGetFwGuidForPlatform(&QcomFwPlatformGuid) == EFI_SUCCESS)
    {
       CopyMem(&(esrt_ent.FwClass), &QcomFwPlatformGuid, sizeof(EFI_GUID));
    }
    else
    {
      DEBUG((EFI_D_WARN,"Set platform type to default\n"));
    }

    if (TRUE == bIsESRTInDppAccessible)
    {
       DEBUG((EFI_D_WARN,"Writing System ESRT Entry to DPP...\n"));
       Status = QcomWrite_EsrtEntryToStorage(&esrt_ent, esrtSize, FALSE);
       if(Status != EFI_SUCCESS)
       {
         DEBUG((EFI_D_WARN,"Writing newly generated System ESRT Entry to DPP failed. Continuing execution marking DPP as inaccessible...\n"));
         Status = EFI_SUCCESS;
         bIsESRTInDppAccessible = FALSE;
       }
    }
  }
  else
  {
    bIsESRTInDppAccessible = TRUE;

    /* Check if the Firmware Resource GUID in storage is a known Qualcomm firmware resource GUID. */
    if (QcomIsFwGuidKnownPlatformType(&(buf->FwClass), &bIsQcomFwGuid) == EFI_SUCCESS)
    {
      if (bIsQcomFwGuid == TRUE)
      {
         /* Verify that the Firmware Resource GUID is correct for the platform. */
         if (QcomGetFwGuidForPlatform(&QcomFwPlatformGuid) == EFI_SUCCESS)
         {
            if (CompareGuid((GUID *)&(buf->FwClass), (GUID *)&QcomFwPlatformGuid) == FALSE)
            {
               /* If the storage contains an ESRT firmware resource GUID that is a Qualcomm type,
                  but the type doesn't match the current platform, then reset the GUID to the
                  correct GUID for the platform and write it back to storage. */
               CopyMem(&(buf->FwClass), &QcomFwPlatformGuid, sizeof(EFI_GUID));
               DEBUG((EFI_D_WARN,"Writing System ESRT Entry to DPP...\n"));
               Status = QcomWrite_EsrtEntryToStorage(buf, esrtSize, FALSE);
               if(Status != EFI_SUCCESS)
               {
                 DEBUG((EFI_D_WARN,"Writing updated System ESRT Entry to DPP failed. Continuing execution marking DPP as inaccessible...\n"));
                 Status = EFI_SUCCESS;
                 bIsESRTInDppAccessible = FALSE;
               }
            }
         }
      }
    }
    CopyMem(&esrt_ent,(VOID*)buf,esrtSize);
  }

  Status = UpdateVarSystemFmpCapsuleImageTypeIdGuid(esrt_ent.FwClass);
  if(Status != EFI_SUCCESS)
  {
    DEBUG((EFI_D_WARN,"Failed to Update PcdSystemFmpCapsuleImageTypeIdGuid \n"));
    Status = EFI_SUCCESS;
  }
  else
  {
    DEBUG((EFI_D_INFO,"PcdSystemFmpCapsuleImageTypeIdGuid Update Success or not required\n"));
  } 

  if(buf != NULL)
  {
    gBS->FreePool(buf);
    buf = NULL;
  }

  return Status;
}

