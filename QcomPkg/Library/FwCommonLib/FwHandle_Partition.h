/** @file PartitionHandles.h
   
  Defines types for PartionHandles.c
  Copyright (c) 2011-2012,2016-2018 Qualcomm Technologies, Inc.  All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential.

**/
/*=============================================================================
                              EDIT HISTORY
  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.



 when           who         what, where, why
 --------       ---         --------------------------------------------------
 2018/05/31     prema       Change to support capsule update for SPINOR-NVME devices
 2018/05/21     gparam      Changes to move all file based state-machine data to NV +
                            Cleanup of unused functions after the file to NV migration
 2017/04/18     gparam      Removed unused/redundant definitions/parameters
 2017/02/13     gpavithr    Adding list of LUN indexes in a UFS device
 2016/12/07     ssumit      Removed duplicate #define GUIDs
 2016/06/27     edg         Modified FwUpdateGetEmmcPartitionTypeGuid function to be generic for UFS support
 2012/11/15     mic         Added FwUpdateGetEmmcPartitionTypeGuid function
 2012/10/08     mic         Removed unused Guids and comments
 2012/08/08     mic         Added maximum update attempts support
 2012/08/08     rsb         Removed unused file enum types
 2012/07/10     rs          Changes in FindinFV to check all FWFile Enums
 2012/06/22     mic         Added support for OPM keys 
 2012/06/21     rs          Adding support for FPDT ACPI table
 2012/06/12     rs          Added support for BreakChangeNumber
 2011/10/12     jthompso    Initial version

=============================================================================*/
#ifndef __QCOM_FW_HANDLES_PARTITION_H__
#define __QCOM_FW_HANDLES_PARTITION_H__

#include <Library/FwCommonLib.h>
#include <Include/Protocol/EFICardInfo.h>

#define UFS_LUN_0                  0x00
#define UFS_LUN_1                  0x01
#define UFS_LUN_2                  0x02
#define UFS_LUN_3                  0x03
#define UFS_LUN_4                  0x04
#define UFS_LUN_5                  0x05
#define UFS_LUN_6                  0x06
#define UFS_LUN_7                  0x07

#define PARTITION_NAME_MAX_CHAR16_SIZE 36

#define TYPE_MEDIA_DEVICE_PATH      4

#define TYPE_END_DEVICE_PATH        0x7F

typedef enum
{
  FWENTRY_EMMC_PARTITION_USER_DATA = 0x00000000,
  FWENTRY_EMMC_PARTITION_BOOT1     = 0x00000001,
  FWENTRY_EMMC_PARTITION_BOOT2     = 0x00000002,
  FWENTRY_EMMC_PARTITION_RPMB      = 0x00000003,
  FWENTRY_EMMC_PARTITION_GPP1      = 0x00000004,
  FWENTRY_EMMC_PARTITION_GPP2      = 0x00000005,
  FWENTRY_EMMC_PARTITION_GPP3      = 0x00000006,
  FWENTRY_EMMC_PARTITION_GPP4      = 0x00000007,
  FWENTRY_UFS_LUN0                 = 0x00000008,
  FWENTRY_UFS_LUN1                 = 0x00000009,
  FWENTRY_UFS_LUN2                 = 0x0000000A,
  FWENTRY_UFS_LUN3                 = 0x0000000B,
  FWENTRY_UFS_LUN4                 = 0x0000000C,
  FWENTRY_UFS_LUN5                 = 0x0000000D,
  FWENTRY_UFS_LUN6                 = 0x0000000E,
  FWENTRY_UFS_LUN7                 = 0x0000000F,
  FWENTRY_SPINOR                   = 0x00000010,
  FWENTRY_NVME                     = 0x00000011,
  FWENTRY_DISK_PARTITION_MAX       = 0x00000012,
  FWENTRY_DISK_PARTITION_DONT_CARE = 0x0000ffff

} FWENTRY_DISK_PARTITION_TYPE;


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
    );

/**
 * Function to get  partition handle
 * 
 * @param PartHandle    - partition handle
 * @param EmmcType      - Emmc partition type
 * @Param PartTypeGuid  - Partition type GUID
 * @Param PartitionName - Partition name
 * @Param GetFATHandle  - TRUE  - Filter from FAT partition handles
 *                        FALSE - Filter from BLOCKIO handles
 * @return : EFI_STATUS
 */
EFI_STATUS
FwUpdateGetPartitionHandle(
  IN OUT EFI_HANDLE                *PartHandle,
    IN FWENTRY_DISK_PARTITION_TYPE  DiskType,
    IN EFI_GUID                    *PartTypeGuid,
    IN CHAR16                      *PartitionName,
    IN BOOLEAN                      GetFATHandle
    );

/**
* Depending on the device flash type, EMMC or UFS or SPINOR-NVME, returns
* the GUID for the BOOT/LUN1/LUN2/SPINOR and GPP/LUN4/NVME partition
*
* @param BOOTGUID - GUID number for the BOOT/LUN1/LUN2/SPINOR partition.
* @param GPPGUID  - GUID number for the GPP/LUN4/NVME partition.
*
* @return EFI_STATUS
*/
EFI_STATUS GetPartitionGUIDs(
  OUT EFI_GUID  *BOOTGUID,
  OUT EFI_GUID  *GPPGUID
  );

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
   );
   
/**
* Identifies the boot LUN number in a UFS device
*
* @param LUN  - UFS LUN number.
*
* @return EFI_STATUS
*/
EFI_STATUS GetBootLUN(
  OUT UINT32  *LUN
  );

#endif
