/** @file PartitionHandles.c
   
  Handles core partition functions for FwCommonLib. 
   
  Maintains a registry of the partitions needed for FwUpdate 
    and provides lookup functions for these partitions and
    their appropriate read/write functions.

  Copyright (c) 2011-2012,2016-2019 Qualcomm Technologies, Inc.  All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential.

**/
/*=============================================================================
                              EDIT HISTORY
  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.



 when           who         what, where, why
 --------       ---         --------------------------------------------------
 2019/03/28     vkasilya    Added BLK_IO_SEL_STRING_CASE_INSENSITIVE to avoid No handles found failure
 2018/05/31     prema       Change to support capsule update for SPINOR-NVME devices
 2018/05/21     gparam      Changes to move all file based state-machine data to NV +
                            Cleanup of unused functions after the file to NV migration
 2017/04/18     gparam      Removed unused/redundant definitions/parameters
 2017/02/13     gpavithr    Removed the log message which prints ACTIVE LUN in GetBootLUN
 2016/12/07     ssumit      Replaced #define GUIDs with extern variables
 2016/06/27     edg         Modified FwUpdateGetEmmcPartitionTypeGuid function to be generic for UFS support
 2012/11/15     mic         Added FwUpdateGetEmmcPartitionTypeGuid function
 2012/10/17     rs          Changes in FwUpdateGetPartitionHandle to check for
                            Partition Name
 2012/07/10     rs          Changes in FindinFV to check all FWFile Enums
 2011/10/12     jthompso    Initial version

=============================================================================*/

#include <Uefi.h>
#include <Library/PcdLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/FwCommonLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/BootConfig.h>
#include <Library/DevicePathLib.h>
#include <Pi/PiDxeCis.h>
#include <Protocol/SimpleFileSystem.h>
#include <Protocol/BlockIo.h>
#include <Protocol/FirmwareVolume2.h>
#include <Include/Library/QcomLib.h>
#include <Include/api/storage/ufs_api.h>
#include <Include/Protocol/EFICardInfo.h>
#include "FwHandle_Partition.h"

/* UFS Device Path */
typedef struct {
   VENDOR_DEVICE_PATH  Ufs;
   EFI_DEVICE_PATH     End;
} QCUFS_DEVICE_PATH;

extern GUID gEfiPartitionTypeGuid;
extern EFI_GUID gQcomTokenSpaceGuid;
extern EFI_GUID gEfiPartitionRecordGuid;
extern EFI_GUID gEfiMemCardInfoProtocolGuid;
extern EFI_GUID gEfiEmmcUserPartitionGuid;
extern EFI_GUID gEfiEmmcBootPartition1Guid;
extern EFI_GUID gEfiEmmcBootPartition2Guid;
extern EFI_GUID gEfiEmmcRpmbPartitionGuid;
extern EFI_GUID gEfiEmmcGppPartition1Guid;
extern EFI_GUID gEfiEmmcGppPartition2Guid;
extern EFI_GUID gEfiEmmcGppPartition3Guid;
extern EFI_GUID gEfiEmmcGppPartition4Guid;
extern EFI_GUID gEfiUfsLU0Guid;
extern EFI_GUID gEfiUfsLU1Guid;
extern EFI_GUID gEfiUfsLU2Guid;
extern EFI_GUID gEfiUfsLU3Guid;
extern EFI_GUID gEfiUfsLU4Guid;
extern EFI_GUID gEfiUfsLU5Guid;
extern EFI_GUID gEfiUfsLU6Guid;
extern EFI_GUID gEfiUfsLU7Guid;
extern EFI_GUID gEfiSpiNor0Guid;
extern EFI_GUID gEfiNvme0Guid; 

CONST EFI_GUID *FwEntryDiskPartitionGuids[FWENTRY_DISK_PARTITION_MAX] = {
    &gEfiEmmcUserPartitionGuid,
    &gEfiEmmcBootPartition1Guid,
    &gEfiEmmcBootPartition2Guid,
    &gEfiEmmcRpmbPartitionGuid,
    &gEfiEmmcGppPartition1Guid,
    &gEfiEmmcGppPartition2Guid,
    &gEfiEmmcGppPartition3Guid,
    &gEfiEmmcGppPartition4Guid,
    &gEfiUfsLU0Guid,
    &gEfiUfsLU1Guid,
    &gEfiUfsLU2Guid,
    &gEfiUfsLU3Guid,
    &gEfiUfsLU4Guid,
    &gEfiUfsLU5Guid,
    &gEfiUfsLU6Guid,
    &gEfiUfsLU7Guid,
    &gEfiSpiNor0Guid,
    &gEfiNvme0Guid
};

/**
 * Function to get a disk partition type guid by a given
 * disk partition type enum
 *  
 * @param DiskType      - Partition type enum
 * @Param DiskTypeGuid  - Disk type GUID
 *
 * @return : EFI_STATUS
 */
EFI_STATUS
FwUpdateGetDiskPartitionTypeGuid(
    IN  FWENTRY_DISK_PARTITION_TYPE  DiskType,
    OUT EFI_GUID                    *DiskTypeGuid
    )
{
    if ((DiskType >=  FWENTRY_DISK_PARTITION_MAX) ||
        (DiskTypeGuid == NULL))
    {
        return EFI_INVALID_PARAMETER;
    }

    CopyMem((VOID*)DiskTypeGuid, (VOID*)FwEntryDiskPartitionGuids[DiskType], sizeof(EFI_GUID));
    
    return EFI_SUCCESS;
  
}

/**
 * Function to get a matching partition handle
 * after comparing the Disk type, Partition Name and 
 * Partition type GUID
 *  
 * @param PartHandle    - partition handle
 * @param DiskType      - Disk partition type
 * @Param PartTypeGuid  - Partition type GUID
 * @Param PartitionName - Partition name
 * @Param GetFATHandle  - TRUE  - Filter from FAT partition handles
 *                        FALSE - Filter from BLOCKIO handles
 * @return : EFI_STATUS
 */
EFI_STATUS
FwUpdateGetPartitionHandle(
    IN OUT EFI_HANDLE              *PartHandle,
    IN FWENTRY_DISK_PARTITION_TYPE  DiskType,
    IN EFI_GUID                    *PartTypeGuid,
    IN CHAR16                      *PartitionName,
    IN BOOLEAN                      GetFATHandle
    )
{
  EFI_STATUS                        Status           = EFI_SUCCESS;
  UINT32                            MaxHandles       = 0;
  UINT32                            Attrib           = 0;
  HandleInfo                        HandleInfoList[2];
  PartiSelectFilter                 HandleFilter;
  
  MaxHandles = sizeof(HandleInfoList)/sizeof(*HandleInfoList);

  Attrib |= BLK_IO_SEL_PARTITIONED_GPT;
  Attrib |= BLK_IO_SEL_MATCH_PARTITION_TYPE_GUID;
  Attrib |= BLK_IO_SEL_MATCH_ROOT_DEVICE;
  Attrib |= BLK_IO_SEL_MATCH_PARTITION_LABEL;
  Attrib |= BLK_IO_SEL_STRING_CASE_INSENSITIVE;
  
  if (TRUE == GetFATHandle) {
    Attrib |= BLK_IO_SEL_SELECT_MOUNTED_FILESYSTEM;
  }


  HandleFilter.PartitionType   = PartTypeGuid;
  HandleFilter.RootDeviceType  = (EFI_GUID*)FwEntryDiskPartitionGuids[DiskType];
  HandleFilter.VolumeName      = 0;
  HandleFilter.PartitionLabel  = PartitionName;


  Status = GetBlkIOHandles(Attrib, &HandleFilter, HandleInfoList, &MaxHandles);

  if(EFI_SUCCESS == Status) {
    if(MaxHandles == 0) {
      Status = EFI_NOT_FOUND;
      DEBUG(( EFI_D_WARN, "No handles found for the given criteria.\n"));
      goto FwUpdateGetPartitionHandleExit;
    }
    if(MaxHandles != 1) {
      Status = EFI_UNSUPPORTED;
      DEBUG(( EFI_D_WARN, "GetBlkIOHandles returned more than one handles. Returning %r\n", Status));
      goto FwUpdateGetPartitionHandleExit;
    }
  }

  *PartHandle = HandleInfoList[0].Handle;

FwUpdateGetPartitionHandleExit:

  return Status;

}

/**
* Depending on the device flash type, EMMC or UFS or SPINOR-NVME, returns
* the GUID for the BOOT/LUN1/LUN2/SPINOR and GPP/LUN4/NVME partition
*
* @param BOOTGUID - GUID number for the BOOT/LUN1/LUN2/SPINOR partition.
* @param GPPGUID  - GUID number for the GPP/LUN4/NVME partition.
*
* @return EFI_STATUS
*/
EFI_STATUS
GetPartitionGUIDs(
   OUT EFI_GUID  *BOOTGUID,
   OUT EFI_GUID  *GPPGUID
   )
{
    EFI_STATUS   Status        = EFI_SUCCESS;
    UINT32       BootLunNumber = 0;
    UINT32       LunNumber1    = 1;
    UINT32       LunNumber2    = 2;
    EFI_GUID     LUN1GUID      = gEfiUfsLU1Guid;
    EFI_GUID     LUN2GUID      = gEfiUfsLU2Guid;
    EFI_GUID     LUN4GUID      = gEfiUfsLU4Guid;
    EFI_GUID     emmcBootGUID  = gEfiEmmcBootPartition1Guid;
    EFI_GUID     emmcGPPGUID   = gEfiEmmcGppPartition1Guid;
    EFI_GUID     SPINOR_GUID   = gEfiSpiNor0Guid;
    EFI_GUID     NVME_GUID     = gEfiNvme0Guid;

    if (BOOTGUID == NULL || GPPGUID == NULL)
    {
        return EFI_INVALID_PARAMETER;
    }

    /* Getting BOOTGUID and GPPGUID depending on device's flash type: eMMC or UFS */
    /* UFS device requires extra check to determine bootable LUN: LUN1 or LUN2    */
    if (boot_from_ufs())
    {
        Status = GetBootLUN(&BootLunNumber);
        HANDLE_ERROR_LABEL(Status, GetBootLUN, GetPartitionGUIDsExit);

        if (BootLunNumber == LunNumber1)
        {
            CopyMem((VOID*)BOOTGUID, (VOID*)&LUN1GUID, sizeof (EFI_GUID));
            CopyMem((VOID*)GPPGUID,  (VOID*)&LUN4GUID, sizeof (EFI_GUID));
            Status = EFI_SUCCESS;
        }
        else if (BootLunNumber == LunNumber2)
        {
            CopyMem((VOID*)BOOTGUID, (VOID*)&LUN2GUID, sizeof (EFI_GUID));
            CopyMem((VOID*)GPPGUID,  (VOID*)&LUN4GUID, sizeof (EFI_GUID));
            Status = EFI_SUCCESS;
        }
        else
        {
            Status = EFI_NOT_FOUND;
            DEBUG_ERR_LABEL("Could not determine Boot LUN", GetPartitionGUIDsExit);
        }
    }
    else if (boot_from_emmc())
    {
        CopyMem((VOID*)BOOTGUID, (VOID*)&emmcBootGUID, sizeof (EFI_GUID));
        CopyMem((VOID*)GPPGUID,  (VOID*)&emmcGPPGUID, sizeof (EFI_GUID));
        Status = EFI_SUCCESS;
    }
    else if (boot_from_spi_nor())
    {
        CopyMem((VOID*)BOOTGUID, (VOID*)&SPINOR_GUID, sizeof (EFI_GUID));
        CopyMem((VOID*)GPPGUID,  (VOID*)&NVME_GUID, sizeof (EFI_GUID));
        Status = EFI_SUCCESS;
    }
    else
    {
        Status = EFI_INCOMPATIBLE_VERSION;
        DEBUG_ERR_LABEL("Could not determine Boot flash type", GetPartitionGUIDsExit);
    }

GetPartitionGUIDsExit:

    return Status;
}

/**
* It searches for the MemCardInfo handler using gEfiMemCardInfoProtocolGuid protocol
*
* @param MemCardInfo  - Handler obtained by gEfiMemCardInfoProtocolGuid protocol.
*
* @return EFI_STATUS
*/
EFI_STATUS
GetMemCardInfoHandle(
    OUT EFI_MEM_CARDINFO_PROTOCOL **MemCardInfo
   )
{
    EFI_STATUS            Status           = EFI_SUCCESS;
    UINTN                 NumHandles       = 0;
    EFI_HANDLE            *HandleBuffer    = NULL;
    UINTN                 HandleCounter;
    QCUFS_DEVICE_PATH     *DevicePath       = NULL;


    if (MemCardInfo == NULL)
    {
        return EFI_INVALID_PARAMETER;
    }

    Status = gBS->LocateHandleBuffer(ByProtocol,
                                     &gEfiMemCardInfoProtocolGuid,
                                     NULL,
                                     &NumHandles,
                                     &HandleBuffer);
    ERR_HANDLE(Status);
    
    for(HandleCounter = 0; HandleCounter < NumHandles; HandleCounter++)
    {
        // Get MemCardInfo
        Status = gBS->HandleProtocol(HandleBuffer[HandleCounter],
                                     &gEfiMemCardInfoProtocolGuid,
                                    (VOID**)MemCardInfo);  

        if(EFI_ERROR(Status))
        {
           continue;
        }

        // Compare MemCardInfo to UFS LUN1
        DevicePath = (QCUFS_DEVICE_PATH *) DevicePathFromHandle(HandleBuffer[HandleCounter]);
        
        if(DevicePath == NULL)
        {
            continue;
        }

        if (CompareGuid(&DevicePath->Ufs.Guid, (FwEntryDiskPartitionGuids[FWENTRY_UFS_LUN1]))) 
        {
            Status = EFI_SUCCESS;
            goto GetMemCardInfoHandleExit;
        }
    }

    Status = EFI_NOT_FOUND;

GetMemCardInfoHandleExit:
    if(HandleBuffer != NULL)
    {
        gBS->FreePool(HandleBuffer);
        HandleBuffer = NULL;
    }

    return Status;
}

/**
* Identifies the boot LUN number in a UFS device
*
* @param LUN  - UFS LUN number.
*
* @return EFI_STATUS
*/
EFI_STATUS
GetBootLUN(
    OUT UINT32  *LUN
    )
{
    EFI_STATUS                Status           = EFI_SUCCESS;
    EFI_MEM_CARDINFO_PROTOCOL *MemCardInfo     = NULL;
    UINT32                    BootLUN          = 0;       

    if (LUN == NULL)
    {
        return EFI_INVALID_PARAMETER;
    }

    Status = GetMemCardInfoHandle(&MemCardInfo);
    HANDLE_ERROR_LABEL(Status, GetMemCardInfoHandle, GetBootLUNExit);

    Status = MemCardInfo->GetBootLU(MemCardInfo, &BootLUN);
    HANDLE_ERROR_LABEL(Status, MemCardInfo->GetBootLU, GetBootLUNExit);

    *LUN = BootLUN;

GetBootLUNExit:

    return Status;

}
