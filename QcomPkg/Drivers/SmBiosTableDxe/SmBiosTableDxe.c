/** @file
  Initializes SMBios table.

  Copyright (c) 2011,2012, 2014-2018 Qualcomm Technologies, Inc. All rights reserved.
  
**/

/*=============================================================================
                              EDIT HISTORY


 when       who     what, where, why
 --------   ---     -----------------------------------------------------------
 04/12/18   ltg     Add Raw File System support
 03/21/18   md      Add Spinor support 
 12/01/17   jt      Add NVMe support 
 07/05/16   bh      Update file system call after API change
 04/23/15   vk      Add UFS support
 10/16/14   vk      LoadFileFromPartition signature update
 09/11/12   niting  Fix status of init
 07/09/12   yg      Add filesize param
 04/25/12   yg      Revert Type 3 table removal 
 04/24/12   yg      Remove System Enclosure table
 03/20/12   kedara  Support config file based table loading.
 03/07/12   kedara  Added support for System Enclosure (Type 3) table
 05/25/11   niting  Initial revision

=============================================================================*/

#include <Library/BaseLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>

#include <Protocol/Smbios.h>

#include <Library/QcomLib.h>
#include <Library/EfiFileLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BootConfig.h>

#define CONFIG_FILE_PATH                         L"\\SMBIOS\\SMBIOS.CFG"
#define RAW_FS_CONFIG_FILE_PATH                  L"SMBIOS\\SMBIOS.CFG"
#define SMBIOS_PART_NAME                         L"SMBIOS"
#define DEFAULT_FS_BLOCKSIZE                     256  // 1/8 of TZ Block Size

static EFI_GUID gSmbiosPartitionTypeGuid =
{ 0x04A856B8, 0x84C1, 0x4075,{ 0x83, 0x91, 0x9A, 0x99, 0x42, 0x35, 0xE5, 0xF0 } };

#include <SmbiosCfgInfo.h>
#include <Protocol/EFIRawFileSystem.h>


EFI_STATUS UpdateSysEnclosureInfoTable( EFI_SMBIOS_PROTOCOL     *Smbios);
EFI_STATUS UpdateBiosInfoTable( EFI_SMBIOS_PROTOCOL     *Smbios);
EFI_STATUS UpdateProcessorInfoTable (EFI_SMBIOS_PROTOCOL     *Smbios);
EFI_STATUS UpdateSystemInfoTable (EFI_SMBIOS_PROTOCOL     *Smbios);
EFI_STATUS UpdateMemoryArrayInfoTable (EFI_SMBIOS_PROTOCOL     *Smbios);
EFI_STATUS UpdateCacheInfoTable (EFI_SMBIOS_PROTOCOL     *Smbios);

//Root Partition
extern EFI_GUID gEfiEmmcGppPartition1Guid;
extern EFI_GUID gEfiEmmcUserPartitionGuid;
extern EFI_GUID gEfiUfsLU4Guid;
extern EFI_GUID gEfiNvme0Guid;
extern EFI_GUID gEfiSpiNor0Guid;

//Partition Types
extern EFI_GUID gEfiPlatPartitionTypeGuid;
extern EFI_GUID gSmbiosPartitionTypeGuid;

/**
LoadCfgFileFromRawPartition
Loads a  file from a block-based partition specified by Root Device type GUID
Partition Type GUID and Partition Label

@param  RawFSProtocol     Handle to Raw File System Protocol
@param  CfgFilePath       Pointer to file path
@param  DiskTypeGuid      Pointer to the disk type guid
@param  PartitionType     Pointer to the partition type guid
@param  PartitionLabel    Pointer to partition label
@param  SmbiosCfgBuffPtr  Pointer to the buffer to store file contents
@param  FileSize          Pointer to size of contents copied to buffer

@retval EFI_SUCCESS           File read successfully
@retval EFI_OUT_OF_RESOURCES  Cannot allocate buffer
@retval EFI_VOLUME_CORRUPTED  Partition is corrupted

**/
EFI_STATUS
LoadCfgFileFromRawPartition(
  IN EFI_RAW_FILE_SYSTEM_PROTOCOL     *pRawFSProtocol,
  IN CHAR16                           *pCfgFilePath,
  IN EFI_GUID                         *pDiskTypeGuid,
  IN EFI_GUID                         *pPartitionType,
  IN CHAR16                           *pPartitionLabel,
  IN OUT UINT8                        **ppSmbiosCfgBuffPtr,
  IN OUT UINTN                        *pFileSize
)
{
  EFI_STATUS Status = EFI_SUCCESS;
  VOID *CfgFileBuffer = NULL;

  // get raw file system protocol handle
  Status = gBS->LocateProtocol(
    &gEfiRawFileSystemProtocolGuid,
    NULL,
    (VOID **)&pRawFSProtocol);

  if (Status != EFI_SUCCESS)
  {
    DEBUG((EFI_D_WARN, "[SmbiosTableDxe] Failed to locate RawFileSystemDxe Protocol.\n"));
    goto LoadCfgFileFromRawPartitionExit;
  }

  // get file size
  Status = pRawFSProtocol->FileRead(pRawFSProtocol, pCfgFilePath, NULL, pFileSize, pPartitionLabel, pPartitionType, pDiskTypeGuid);
  if (Status != EFI_SUCCESS)
  {
    DEBUG((EFI_D_WARN, "[SmbiosTableDxe] Failed to read config file size from raw file System.\n"));
    if (Status == EFI_VOLUME_CORRUPTED)
    {
      DEBUG((EFI_D_WARN, "[SmbiosTableDxe] Raw file system header is corrupted. Partition needs to be re-formatted.\n"));
    }

    // set file size to 0 so that we do not attempt to read the file
    *pFileSize = 0;
  }

  // read file size bytes from the file
  if (*pFileSize != 0)
  {
    CfgFileBuffer = (UINT8*)AllocateZeroPool(sizeof(UINT8) * (*pFileSize));

    if (CfgFileBuffer == NULL)
    {
      Status = EFI_OUT_OF_RESOURCES;
      DEBUG((EFI_D_WARN, "[SmbiosTableDxe] Failed to allocate buffer for config file. Status = 0x%x\n", Status));
      goto LoadCfgFileFromRawPartitionExit;
    }

    // read file
    Status = pRawFSProtocol->FileRead(pRawFSProtocol, pCfgFilePath, CfgFileBuffer, pFileSize, pPartitionLabel, pPartitionType, pDiskTypeGuid);

    if (Status != EFI_SUCCESS)
    {
      DEBUG((EFI_D_WARN, "[SmbiosTableDxe] Failed to read config file from raw file System.\n"));
    }

    if (CfgFileBuffer != NULL)
    {
      *ppSmbiosCfgBuffPtr = CfgFileBuffer;
    }
  }

LoadCfgFileFromRawPartitionExit:
  return Status;
}

/**
Set up the SMBios table

  @param  ImageHandle   of the loaded driver
  @param  SystemTable   Pointer to the System Table

  @retval EFI_SUCCESS           Protocol registered
  @retval EFI_OUT_OF_RESOURCES  Cannot allocate protocol data structure
  @retval EFI_DEVICE_ERROR      Hardware problems

**/
EFI_STATUS
SmBiosTableDxeInitialize (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_SMBIOS_PROTOCOL     *Smbios;
  EFI_STATUS              Status;
  UINT8                   *SmbiosCfgBuffPtr = NULL;
  UINTN                   FileSize = 0;
  UINT32                  BlkIOAttrib = 0;
  EFI_RAW_FILE_SYSTEM_PROTOCOL *RawFSProtocol = NULL;
  
  // Pointer to list of smbios tables to be added
  SMBIOS_TABLE_NODE *SmbiosListHeadPtr;
  
  Status = gBS->LocateProtocol (
                  &gEfiSmbiosProtocolGuid,
                  NULL,
                  (VOID **) &Smbios);

  ASSERT_EFI_ERROR (Status);

  // Initialize Smbios Table List struct
  SmbiosConfigInit(&SmbiosListHeadPtr);
    // Initialize the Head Node;
  SetSmbiosListHeadNode (SmbiosListHeadPtr);
  // Set attributes for fetching Block IO handle
  BlkIOAttrib = BLK_IO_SEL_PARTITIONED_GPT;
  BlkIOAttrib |= BLK_IO_SEL_MEDIA_TYPE_NON_REMOVABLE;
  BlkIOAttrib |= BLK_IO_SEL_MATCH_ROOT_DEVICE;
  BlkIOAttrib |= BLK_IO_SEL_MATCH_PARTITION_TYPE_GUID;
  
  // Load Smbios file into buffer

  if (boot_from_emmc())
  {
    //Attempt to load from Plat partition in eMMC GPP
    Status = LoadFileFromPartition(BlkIOAttrib,
                     CONFIG_FILE_PATH,
                     &gEfiEmmcGppPartition1Guid,
                     &gEfiPlatPartitionTypeGuid,
                     NULL,
                     &SmbiosCfgBuffPtr,
                     &FileSize
                     );
    //Attempt to load from User Partition
    if (Status != EFI_SUCCESS)
    {
      Status = LoadFileFromPartition(BlkIOAttrib,
                       CONFIG_FILE_PATH,
                       &gEfiEmmcUserPartitionGuid,
                       &gEfiPlatPartitionTypeGuid,
                       NULL,
                       &SmbiosCfgBuffPtr,
                       &FileSize
                       );
    }
  }
  else if (boot_from_ufs())
  {
     //Search in only SMBIOS partition for the config file, PLAT partition should not have SMBIOS config as it may get wiped away during an update.
      Status = LoadCfgFileFromRawPartition(
        RawFSProtocol,
        RAW_FS_CONFIG_FILE_PATH,
        &gEfiUfsLU4Guid,
        &gSmbiosPartitionTypeGuid,
        SMBIOS_PART_NAME,
        &SmbiosCfgBuffPtr,
        &FileSize
      );
  }
  else if (boot_from_nvme())
  {
     //Search in only SMBIOS partition for the config file, PLAT partition should not have SMBIOS config as it may get wiped away during an update.
    Status = LoadCfgFileFromRawPartition(
      RawFSProtocol,
      RAW_FS_CONFIG_FILE_PATH,
      &gEfiSpiNor0Guid,
      &gSmbiosPartitionTypeGuid,
      SMBIOS_PART_NAME,
      &SmbiosCfgBuffPtr,
      &FileSize
    );
    
  }

  if (Status == EFI_SUCCESS)
  {
    // Parse config file and update tables if it exists.
    UpdateCfgBasedInfoTable(SmbiosCfgBuffPtr, FileSize, SmbiosListHeadPtr);
    AddSmbiosTables(Smbios,SmbiosListHeadPtr);
  }

  Status = EFI_SUCCESS;
  
  // Override Config file changes if any with preset values.
  UpdateBiosInfoTable(Smbios);
  UpdateCacheInfoTable(Smbios);
  UpdateProcessorInfoTable(Smbios);
  UpdateSystemInfoTable(Smbios);
  UpdateMemoryArrayInfoTable(Smbios);
  UpdateSysEnclosureInfoTable(Smbios);

  return Status;
}

