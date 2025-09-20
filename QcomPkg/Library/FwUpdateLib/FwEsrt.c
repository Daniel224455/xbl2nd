/** @file FwESRT.c

   Functions to maintain the ESRT , including ESRT header and ESRT entry .

  Copyright (c) 2012-2019 Qualcomm Technologies, Inc.  All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential.

**/
/*=============================================================================
                              EDIT HISTORY
  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.



 when           who         what, where, why
 --------       ---         -----------------------------------------------
 2019/06/06     rajb        Moving to EFICHIPINFO_FAMILY, which is capable of handling
                            different chip IDs of that family
 2018/08/31     prema       Changes for SysFwVersion Entry handling to update
                            SysFwVersion and ESRT entry
 2018/07/20     gparam      KW Fixes
 2018/05/31     prema       Change to use different format to print GUID
 2018/05/28     prema       FAT to NV migration for ESRT data storage
 2018/05/25     prema       Updated System Firmware Resource GUIDs for 8180 
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
#include <Library/BootConfig.h>

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

#define QC_SYSFW_VERSION_SIZE                 0x1C

static EFI_LOCK gSysFwVersionDataCacheLock;

/*global variables*/

const QC_SYSFW_VERSION_DATA gDefaultSysFwVerData = {
  SYSTEM_FW_VERSION_SIGNATURE,               // default signature                : 'S','Y','S','F','W','V','E','R'
  SYSTEM_FW_VERSION_REVISION,                // default Fwrevision               : 1.0 (0x00010000) 
  QC_SYSFW_VERSION_SIZE,                     // default datasize                 : QC_SYSFW_VERSION_SIZE
  0x0,                                       // default CrcValue                 : 0
  DEFAULT_SYSTEM_FW_VERSION,                 // default FwVersion                : 0
  DEFAULT_SYSTEM_FW_LOWEST_SUPPORTED_VERSION,// default LowestSupportedFwVersion : 0
};

/* The list of System Firmware Resource GUIDs that apply for each type of platform, the order must
   match the QCOM_FW_RESOURCE_TYPE. */
const EFI_GUID qcomFwResourceGUIDs_UFS[] =
{
    QCOM_FW_UPDATE_SYSTEM_FW_8180_MTP_UFS_GUID,
    QCOM_FW_UPDATE_SYSTEM_FW_8180_CLS_UFS_GUID,
    QCOM_FW_UPDATE_SYSTEM_FW_8180_QRD_UFS_GUID,
    QCOM_FW_UPDATE_SYSTEM_FW_UNKNOWN_GUID
};

const EFI_GUID qcomFwResourceGUIDs_SPINOR[] =
{
    QCOM_FW_UPDATE_SYSTEM_FW_8180_MTP_SPINOR_GUID,
    QCOM_FW_UPDATE_SYSTEM_FW_8180_CLS_SPINOR_GUID,
    QCOM_FW_UPDATE_SYSTEM_FW_8180_QRD_SPINOR_GUID,
    QCOM_FW_UPDATE_SYSTEM_FW_UNKNOWN_GUID
};
EFI_GUID  *qcomFwResourceGUIDs = NULL;

// Fill default with zeros so that the cache stays invalid during boot and
// gets poulated on the first query for the version details
static QC_SYSFW_VERSION_DATA gSysFwVerPartitionDataCache  = {0};

extern EFI_GUID   gSystemFwVersionPartitionGuid;

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
    DEBUG ((EFI_D_ERROR, "FmpDxe: Allocating new SystemFmpCapsuleImageTypeIdGuid failed\n"));
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
    DEBUG ((EFI_D_ERROR, "FmpDxe: Setting of new GUID list in SystemFmpCapsuleImageTypeIdGuid failed with error %r\n", Status));
    return Status;
  }

  return EFI_SUCCESS;
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
  EFIChipInfoFamilyType                ChipFamily            = EFICHIPINFO_FAMILY_UNKNOWN;
  EFIChipInfoVersionType               nChipVersion          = 0;

  if (FwResourceGuid == NULL)
  {
     return EFI_INVALID_PARAMETER;
  }

  DEBUG((EFI_D_INFO,"Retrieving FW GUID for the current platform\n"));

  if (qcomFwResourceGUIDs == NULL)
  {
    if (boot_from_spi_nor())
	{
	  qcomFwResourceGUIDs = (EFI_GUID *)&qcomFwResourceGUIDs_SPINOR;
	}
	else if (boot_from_ufs())
	{
	  qcomFwResourceGUIDs = (EFI_GUID *)&qcomFwResourceGUIDs_UFS;
	}
	else
	{
	  DEBUG((EFI_D_WARN,"Error: Unsupported system memory type detected.\n"));
	  Status = EFI_UNSUPPORTED;
	  goto QcomGetFwGuidForPlatformExit;
	}
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

  Status = mChipInfoProtocol->GetChipFamily( mChipInfoProtocol,
                                                     &ChipFamily);
  HANDLE_ERROR_LABEL(Status,mChipInfoProtocol->GetChipFamily,QcomGetFwGuidForPlatformExit);

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

  if (EFICHIPINFO_FAMILY_SC8180X == ChipFamily)
  {
	EFI_STATUS         Status;
	CHAR8              DestBuffer[128];
	CHAR8*             PlatTypePtr;
	CHAR8*             ChipNamePtr;
	CHAR8*             ChipVerPtr;
  
	Status = GetPlatformStrings (DestBuffer, sizeof(DestBuffer),
                               &PlatTypePtr, &ChipNamePtr, &ChipVerPtr);

    DEBUG((EFI_D_WARN,"Platform: %a %a\n", ChipNamePtr, PlatTypePtr));

    /* Detected 8180 */
    if (platformInfo.platform == EFI_PLATFORMINFO_TYPE_MTP)
    {
      /* 8180 MTP platform */
      CopyMem(FwResourceGuid, &qcomFwResourceGUIDs[QCOM_FW_UPDATE_SYSTEM_FW_8180_MTP_TYPE], sizeof(EFI_GUID));
    }
    else if (platformInfo.platform == EFI_PLATFORMINFO_TYPE_CLS)
    {
      /* 8180 CLS platform */
      CopyMem(FwResourceGuid, &qcomFwResourceGUIDs[QCOM_FW_UPDATE_SYSTEM_FW_8180_CLS_TYPE], sizeof(EFI_GUID));
    }
    else if (platformInfo.platform == EFI_PLATFORMINFO_TYPE_QRD)
    {
      /* 8180 QRD platform */
      CopyMem(FwResourceGuid, &qcomFwResourceGUIDs[QCOM_FW_UPDATE_SYSTEM_FW_8180_QRD_TYPE], sizeof(EFI_GUID));
    }
    else
    {
      /* Unknown 8180 platform */
      DEBUG((EFI_D_WARN,"System firmware GUID not known for current platform (%a:%d). Using default firmware GUID.\n", PlatTypePtr, platformInfo.platform));
      CopyMem(FwResourceGuid, &qcomFwResourceGUIDs[QCOM_FW_UPDATE_SYSTEM_FW_UNKNOWN_TYPE], sizeof(EFI_GUID));
    }
  }
  else
  {
    /* Unknown chipset */
    DEBUG((EFI_D_WARN,"Platform: Unknown chipset: %d\n", ChipFamily));
    CopyMem(FwResourceGuid, &qcomFwResourceGUIDs[QCOM_FW_UPDATE_SYSTEM_FW_UNKNOWN_TYPE], sizeof(EFI_GUID));
  }

QcomGetFwGuidForPlatformExit:

  return Status;
}

/**
  Validate SysFwVersionData retrieved from SysFwVersion Patition

  @param[in]  QC_SYSFW_VERSION_DATA   SysFwVersionData buffer pointer to be validate for CRC32

  @retval EFI_SUCCESS                When valid CRC32 retrieved for given SysFwVerData
  @retval EFI_INVALID_PARAMETER      When NULL SysFwVersion data is passed
  @retval EFI_VOLUME_CORRUPTED       When invalid SysFwVer signature or Version or CRC found

**/

EFI_STATUS
EFIAPI
QcomValidateSysFwVersionData (
  IN QC_SYSFW_VERSION_DATA *SysFwVersionData
)
{
  EFI_STATUS Status = EFI_SUCCESS;
  UINT32 OldCrc32   = 0;
  UINT32 NewCrc32   = 0;

  if (NULL == SysFwVersionData)
  {
    Status = EFI_INVALID_PARAMETER;
    DEBUG((EFI_D_ERROR, "Invalid SysFw data found and return %r\n", Status));
    goto QcomValidateSysFwVersionDataExit;
  }
      
  //Check for valid SysFwVer Signature
  if(SYSTEM_FW_VERSION_SIGNATURE == SysFwVersionData->Signature)
  {
    //Check for valid SysFwVer Revision
    if (SYSTEM_FW_VERSION_REVISION != SysFwVersionData->Revision)
    {
      Status = EFI_VOLUME_CORRUPTED;
      DEBUG((EFI_D_ERROR, "Invalid SysFw revision 0x%x found and return %r\n", SysFwVersionData->Revision, Status));
      goto QcomValidateSysFwVersionDataExit;
    }
    
    OldCrc32 = SysFwVersionData->VersionDataCrc32;
    SysFwVersionData->VersionDataCrc32 = 0;

    NewCrc32 = CalculateCrc32( SysFwVersionData,
                               SysFwVersionData->VersionDataSize);
    
    //Check for valid SysFwVer CRC
    if(NewCrc32 != OldCrc32)
    {
      DEBUG((EFI_D_WARN, "NewCrc32 = 0x%x is mismatch with OldCrc32 = 0x%x ..\n", NewCrc32, OldCrc32));
      //Restore to old CRC32
      SysFwVersionData->VersionDataCrc32 = OldCrc32;
      Status = EFI_VOLUME_CORRUPTED;
      goto QcomValidateSysFwVersionDataExit;
    }
    else {
      // Update with same crc32 value
      SysFwVersionData->VersionDataCrc32 = NewCrc32;
      Status = EFI_SUCCESS;
      goto QcomValidateSysFwVersionDataExit;
    }
  }
  else
  {
    Status = EFI_VOLUME_CORRUPTED;
    goto QcomValidateSysFwVersionDataExit;
  }

QcomValidateSysFwVersionDataExit:

    return Status;
        
}

/**
  Get SystemFwVersion data from SysFwVersion partition.
  This function will get QC_SYSFW_VERSION_DATA block of memory by reading
  full SysFwVersion partition and copy only QC_SYSFW_VERSION_DATA size of memory.
  
  On successful returning of QC_SYSFW_VERSION_DATA size of memory, this block of memory
  will be validate for Signature, Version, CRC.
  
  @param [out] FwVersion                - Pointer to current firmware version.
  @param [out] LowestSupportedFwVersion - Pointer to current lowest supported version.
  
  @retval EFI_STATUS

**/

EFI_STATUS
EFIAPI
QcomGetSysFwVersion (
  OUT UINT32                   *FwVersion,
  OUT UINT32                   *LowestSupportedFwVersion
  )
{
  EFI_STATUS            Status      = EFI_SUCCESS;
  QC_SYSFW_VERSION_DATA *SysFwVerPartitionDataPtr  = NULL;

  EfiAcquireLock(&gSysFwVersionDataCacheLock);

  // Give error if both are not provided
  if ((NULL == FwVersion) &&
      (NULL == LowestSupportedFwVersion))
  {
    Status = EFI_INVALID_PARAMETER;
    DEBUG ((EFI_D_ERROR, "QcomGetSysFwVersion received invalid parameter\n"));
    goto Exit;
  }

  // Use the cached data if valid. Else query from partition and udpate cache
  Status = QcomValidateSysFwVersionData(&gSysFwVerPartitionDataCache);      
  if (EFI_ERROR (Status))
  {
    DEBUG ((EFI_D_INFO, "gSysFwVerPartitionDataCache not valid. Trying to populate from version partition\n"));

    Status = QcomReadSysFwVerPartition((VOID**)&SysFwVerPartitionDataPtr);
    HANDLE_ERROR_LABEL(Status,QcomValidateSysFwVersionData,Exit);

    // Validate read Buffer
    Status = QcomValidateSysFwVersionData(SysFwVerPartitionDataPtr);
    if (EFI_ERROR (Status))
    {
      if(Status == EFI_VOLUME_CORRUPTED && SysFwVerPartitionDataPtr != NULL)
      {
        DEBUG((EFI_D_WARN, "Invalid Signature %lx found with SysFwVer.\n", SysFwVerPartitionDataPtr->Signature));
      }
      goto Exit;
    }

    CopyMem(&gSysFwVerPartitionDataCache, SysFwVerPartitionDataPtr, sizeof (QC_SYSFW_VERSION_DATA));
  }

  if (NULL != FwVersion)
  {
    *FwVersion = gSysFwVerPartitionDataCache.FwVersion;
  }

  if (NULL != LowestSupportedFwVersion)
  {
    *LowestSupportedFwVersion = gSysFwVerPartitionDataCache.LowestSupportedFwVersion;
  }

Exit:

  if(NULL!= SysFwVerPartitionDataPtr)
  {
    gBS->FreePool (SysFwVerPartitionDataPtr);
    SysFwVerPartitionDataPtr = NULL;
  }

  EfiReleaseLock (&gSysFwVersionDataCacheLock);

  return Status;
}

/**
  This function sets SysFwVersion data into SysFwVersion partition.
  
  @param [IN] FwVersion                - New firmware version.
  @param [IN] LowestSupportedFwVersion - New lowest supported version.
  
  @retval EFI_STATUS

**/

EFI_STATUS
QcomSetSysFwVersion (
  IN UINT32                 FwVersion,
  IN UINT32                 LowestSupportedFwVersion
 )
{
  EFI_STATUS                Status                  = EFI_SUCCESS;
  QC_SYSFW_VERSION_DATA     SysFwVerPartitionData;
  UINT32                    OldCrc32                = 0;
  UINT32                    NewCrc32                = 0;

  EfiAcquireLock(&gSysFwVersionDataCacheLock);

  CopyMem(&SysFwVerPartitionData, &gDefaultSysFwVerData, sizeof(QC_SYSFW_VERSION_DATA));

  SysFwVerPartitionData.FwVersion = FwVersion;
  SysFwVerPartitionData.LowestSupportedFwVersion = LowestSupportedFwVersion;

  // Calculate CRC and update
  OldCrc32 = SysFwVerPartitionData.VersionDataCrc32;
  SysFwVerPartitionData.VersionDataCrc32  = 0;

  NewCrc32 = CalculateCrc32(
    &SysFwVerPartitionData,
    SysFwVerPartitionData.VersionDataSize
    );

  SysFwVerPartitionData.VersionDataCrc32 = NewCrc32;

  DEBUG ((DEBUG_WARN, "Writing updated SysFwVer details into SysFwVer partition...\n"));
  Status = QcomWriteSysFwPartition((VOID*)&SysFwVerPartitionData, sizeof(QC_SYSFW_VERSION_DATA));
  HANDLE_ERROR_LABEL(Status,QcomWriteSysFwPartition, Exit);

  // Cache new content after updating to partition
  CopyMem(&gSysFwVerPartitionDataCache, &SysFwVerPartitionData, sizeof (QC_SYSFW_VERSION_DATA));

Exit:

  EfiReleaseLock (&gSysFwVersionDataCacheLock);

  return Status;
}

/**
  Reading SystemFwVersion partion based on SysFwVersion partition guid
  
  This function expects size of QC_SYSFW_VERSION_DATA memory on.
  And this function will read full SysFwVersion partition and return full
  size value. On reading full data, only QC_SYSFW_VERSION_DATA size of data
  will be copy to destination buffer and same buffer will be return to caller.
  
  @param  VOID***             : **Buffer
  
  @retval EFI_STATUS
  @retval EFI_SUCESS            Action completed successfully

**/

EFI_STATUS
EFIAPI
QcomReadSysFwVerPartition (
  IN OUT VOID        **Buffer
  )
{
  EFI_STATUS                  Status             = EFI_SUCCESS;
  UINT32                      Attribs            = 0;
  PartiSelectFilter           HandleFilter;
  EFI_BLOCK_IO_PROTOCOL      *SysFwVerPartBlkIo  = NULL;
  VOID                       *SysFwBuffer        = NULL;
  UINTN                       SysFwBufferSize    = 0;  
  VOID                       *BlockAlignedBuffer = NULL;
  UINTN                       BlockAlignedSize   = 0;
  // Use size as 2 to detect existance of more than one SystemFwVersion partition while query
  HandleInfo                  HandleInfoList [2];
  UINT32                      NoHandles          = sizeof(HandleInfoList) / sizeof(*HandleInfoList);      

  SysFwBufferSize = sizeof(QC_SYSFW_VERSION_DATA);
  *Buffer = NULL;

  Attribs |= BLK_IO_SEL_MEDIA_TYPE_NON_REMOVABLE;
  Attribs |= BLK_IO_SEL_MATCH_PARTITION_TYPE_GUID;

  HandleFilter.VolumeName = 0;
  HandleFilter.PartitionType = &gSystemFwVersionPartitionGuid;

  if (boot_from_spi_nor())
  {
    Attribs |= BLK_IO_SEL_MATCH_ROOT_DEVICE;
    HandleFilter.RootDeviceType = &gEfiSpiNor0Guid;
  }
  else if (boot_from_ufs())
  {
    Attribs |= BLK_IO_SEL_MATCH_ROOT_DEVICE;
    HandleFilter.RootDeviceType = &gEfiUfsLU4Guid;
  }
  
  Status = GetBlkIOHandles(Attribs, &HandleFilter, HandleInfoList, &NoHandles);

  if (EFI_ERROR (Status))
  {
    DEBUG ((EFI_D_ERROR, "GetBlkIOHandles returned Status: %r\n", Status));
    goto QcomReadSysFwVerPartitionExit;
  }
  
  if (0 == NoHandles)
  {
    Status = EFI_NOT_FOUND;
    DEBUG ((EFI_D_ERROR, "SystemFwVersion partition not found. Status: %r\n", Status));
    return Status;
  }

  if (NoHandles != 1)
  {
    Status = EFI_DEVICE_ERROR;
    DEBUG ((EFI_D_ERROR, "More than 1 SystemFwVersion partition found. Status=%r\n"));    
    goto QcomReadSysFwVerPartitionExit;
  }

  Status = gBS->HandleProtocol (
                    HandleInfoList[0].Handle,
                    &gEfiBlockIoProtocolGuid,
                    (VOID **)&SysFwVerPartBlkIo);
  if (EFI_ERROR (Status))
  {
    DEBUG ((EFI_D_ERROR, "SystemFwVersion partition does not support block io. Status: %r\n", Status));
    goto QcomReadSysFwVerPartitionExit;
  }

  if (NULL == SysFwVerPartBlkIo)
  {
    Status = EFI_NOT_FOUND;
    DEBUG ((EFI_D_ERROR, "SysFwVersion partition blockIo is NULL : Status=%r\n", Status));
    goto QcomReadSysFwVerPartitionExit;
  }

  // Allocate buffer to read full partition data
  ALIGN_TO_BLOCK (SysFwBufferSize, SysFwVerPartBlkIo->Media->BlockSize, BlockAlignedSize);
  Status = AllocateAndZero(EfiBootServicesData,BlockAlignedSize,&BlockAlignedBuffer);
  if (EFI_ERROR (Status) || (BlockAlignedBuffer == NULL))
  {
    DEBUG ((EFI_D_ERROR, "Failed to allocate memory for BlockAlignedBuffer. Status: %r\n", Status));
    Status = EFI_OUT_OF_RESOURCES;
    goto QcomReadSysFwVerPartitionExit;
  }

  Status = SysFwVerPartBlkIo->ReadBlocks(SysFwVerPartBlkIo,
                                         SysFwVerPartBlkIo->Media->MediaId,
                                         0,
                                         BlockAlignedSize,
                                         BlockAlignedBuffer);
  HANDLE_ERROR_LABEL(Status,SysFwVerPartBlkIo->ReadBlocks,QcomReadSysFwVerPartitionExit);
  
  // Allocate buffer to copy valid QC_SYSFW_VERSION_DATA data after read from partition
  Status = AllocateAndZero(EfiBootServicesData,SysFwBufferSize,&SysFwBuffer);
  if (EFI_ERROR (Status) || (SysFwBuffer == NULL))
  {
      DEBUG ((EFI_D_ERROR, "ReadBlock success but failed to allocate memory for SysFwBuffer. Status: %r\n", Status));
      Status = (EFI_ERROR (Status)) ? Status : EFI_OUT_OF_RESOURCES;
      HANDLE_ERROR_LABEL(Status,AllocateAndZero,QcomReadSysFwVerPartitionExit);
  }

  // Copy QC_SYSFW_VERSION_DATA size data from AlignedBuffer to new buffer and validate
  CopyMem(SysFwBuffer, BlockAlignedBuffer, SysFwBufferSize);
  *Buffer = SysFwBuffer;

QcomReadSysFwVerPartitionExit:

  if(EFI_ERROR(Status))
  {
    if (NULL!=SysFwBuffer)
    {
      gBS->FreePool (SysFwBuffer);
      SysFwBuffer = NULL;
    }
  }

  if (NULL!=BlockAlignedBuffer)
  {
    gBS->FreePool (BlockAlignedBuffer);
    BlockAlignedBuffer = NULL;
  }

  return Status;
}

/**
  Write SystemFwVersion data into SysFwVersion partion.
  This function recalculate and update CRC for given inputdata
  as and when data is received
  
  @param  VOID***             : *Buffer   Buffer to write
  @param  UINTN               : BuffSize  Size of buffer to write
  
  @retval EFI_STATUS

**/

EFI_STATUS
EFIAPI
QcomWriteSysFwPartition (
  IN  VOID         *Buffer,
  IN  UINTN         BufferSize
  )
{
  EFI_STATUS                  Status             = EFI_SUCCESS;
  UINT32                      Attribs            = 0;
  PartiSelectFilter           HandleFilter;
  EFI_BLOCK_IO_PROTOCOL      *SysFwVerPartBlkIo  = NULL;
  VOID                       *BlockAlignedBuffer = NULL;
  UINTN                       BlockAlignedSize   = 0;
  // Use size as 2 to detect existance of more than one SystemFwVersion partition while query
  HandleInfo                  HandleInfoList [2];
  UINT32                      NoHandles          = sizeof(HandleInfoList) / sizeof(*HandleInfoList);      

  Attribs |= BLK_IO_SEL_MEDIA_TYPE_NON_REMOVABLE;
  Attribs |= BLK_IO_SEL_MATCH_PARTITION_TYPE_GUID;

  if(NULL == Buffer)
  {
      DEBUG ((EFI_D_ERROR, "Buffer is NULL before attempt to write to partition\n"));
      Status = EFI_INVALID_PARAMETER;
      goto QcomWriteSysFwPartitionExit;      
  }

  HandleFilter.VolumeName = 0;
  HandleFilter.PartitionType = &gSystemFwVersionPartitionGuid;

  if (boot_from_spi_nor())
  {
    Attribs |= BLK_IO_SEL_MATCH_ROOT_DEVICE;
    HandleFilter.RootDeviceType = &gEfiSpiNor0Guid;
  }
  else if (boot_from_ufs())
  {
    Attribs |= BLK_IO_SEL_MATCH_ROOT_DEVICE;
    HandleFilter.RootDeviceType = &gEfiUfsLU4Guid;
  }
  
  Status = GetBlkIOHandles(Attribs, &HandleFilter, HandleInfoList, &NoHandles);

  if (EFI_ERROR (Status))
  {
    DEBUG ((EFI_D_ERROR, "GetBlkIOHandles returned Status: %r\n", Status));
    goto QcomWriteSysFwPartitionExit;
  }
  
  if (0 == NoHandles)
  {
    Status = EFI_NOT_FOUND;
    DEBUG ((EFI_D_ERROR, "SystemFwVersion partition not found. Status: %r\n", Status));
    goto QcomWriteSysFwPartitionExit;
  }

  if (NoHandles != 1)
  {
    Status = EFI_DEVICE_ERROR;
    DEBUG ((EFI_D_ERROR, "More than 1 SystemFwVersion partition found. Status:%r\n", Status));
    goto QcomWriteSysFwPartitionExit;
  }

  Status = gBS->HandleProtocol (HandleInfoList[0].Handle,
                                &gEfiBlockIoProtocolGuid,
                                (VOID **)&SysFwVerPartBlkIo);
  if ((EFI_ERROR (Status)) || (NULL == SysFwVerPartBlkIo))
  {  
    Status = (EFI_ERROR(Status)) ? Status : EFI_OUT_OF_RESOURCES;
    DEBUG ((EFI_D_ERROR, "HandleProtocol failed. Status: %r\n", Status));
    goto QcomWriteSysFwPartitionExit;
  }

  // Copy QC_SYSFW_VERSION_DATA size data from valid AlignedBuffer to new buffer
  ALIGN_TO_BLOCK (BufferSize, SysFwVerPartBlkIo->Media->BlockSize, BlockAlignedSize);
  
  if(BufferSize != BlockAlignedSize)
  {
    Status = AllocateAndZero(EfiBootServicesData,BlockAlignedSize,&BlockAlignedBuffer);
    if (EFI_ERROR (Status) || (BlockAlignedBuffer == NULL))
    {
      Status = (EFI_ERROR (Status)) ? Status : EFI_OUT_OF_RESOURCES;
      DEBUG ((EFI_D_ERROR, "Failed to allocate memory for BlockAlignedBuffer. Status: %r\n", Status));
      goto QcomWriteSysFwPartitionExit;
    }
    CopyMem(BlockAlignedBuffer, Buffer, BufferSize);
  }
  else
  {
    BlockAlignedBuffer = Buffer;
  }

  Status = SysFwVerPartBlkIo->WriteBlocks(SysFwVerPartBlkIo,
                                          SysFwVerPartBlkIo->Media->MediaId,
                                          0,
                                          BlockAlignedSize,
                                          BlockAlignedBuffer);
  HANDLE_ERROR_LABEL(Status,QcomWriteSysFwPartition,QcomWriteSysFwPartitionExit);
  
  Status = SysFwVerPartBlkIo->FlushBlocks(SysFwVerPartBlkIo);
  Status = (EFI_ERROR (Status) ? EFI_SUCCESS : Status);

QcomWriteSysFwPartitionExit:

  if((NULL != BlockAlignedBuffer) && 
     (BufferSize != BlockAlignedSize))
  {
      gBS->FreePool (BlockAlignedBuffer);            
      BlockAlignedBuffer = NULL;
  }

  return Status;
}

/**
 * This function prints current SysFwVersion entry info
 * by reading entry from SysFwVersion partition.
 * 
 * @DebugLevel - Debug level
 *
 * @return : EFI_STATUS
**/

EFI_STATUS
QcomPrintSysFwVerPartitionData(
  IN UINTN                      DebugLevel
  )
{
  EFI_STATUS Status        = EFI_SUCCESS;
  VOID      *SysFwVerData  = NULL;

  Status = QcomReadSysFwVerPartition(&SysFwVerData);
  if((EFI_SUCCESS == Status) && (NULL != SysFwVerData))
  {
      DEBUG((DebugLevel,"\n"));
      DEBUG((DebugLevel,"    Current SysFwVersion Entry info:\n"));
      DEBUG((DebugLevel,"    ====================================\n"));
      DEBUG((DebugLevel,"    Signature                 : %lx\n",   ((QC_SYSFW_VERSION_DATA*)SysFwVerData)->Signature));
      DEBUG((DebugLevel,"    Revision                  : 0x%x\n", ((QC_SYSFW_VERSION_DATA*)SysFwVerData)->Revision));
      DEBUG((DebugLevel,"    VersionDataCrc32          : 0x%x\n", ((QC_SYSFW_VERSION_DATA*)SysFwVerData)->VersionDataCrc32));
      DEBUG((DebugLevel,"    FwVersion                 : 0x%x\n", ((QC_SYSFW_VERSION_DATA*)SysFwVerData)->FwVersion));
      DEBUG((DebugLevel,"    LowestSupportedFwVersion  : 0x%x\n", ((QC_SYSFW_VERSION_DATA*)SysFwVerData)->LowestSupportedFwVersion));
      DEBUG((DebugLevel,"    ====================================\n"));
      goto QcomPrintSysFwVersionEntryExit;
    
  }
  else
  {
    DEBUG((DebugLevel,"Read SysFwVersion partiont failed. Status : %r", Status));
    goto QcomPrintSysFwVersionEntryExit;
  }

QcomPrintSysFwVersionEntryExit:

  if(SysFwVerData != NULL)
  {
    gBS->FreePool(SysFwVerData);
    SysFwVerData = NULL;
  }  

  return Status;
}

/**
 * Initialize lock used to handle cache buffer
 * 
 * @return : EFI_STATUS
**/
EFI_STATUS
EFIAPI
QcomInitSysFwVersionContext (
  VOID
  )
{
  EFI_STATUS Status = EFI_SUCCESS;

  EfiInitializeLock (&gSysFwVersionDataCacheLock, TPL_CALLBACK);

  return Status;
}


