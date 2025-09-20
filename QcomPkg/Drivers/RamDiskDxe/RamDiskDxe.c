/******************************************************************************
*
* Copyright (c) 2014 Qualcomm Technologies, Inc.  All Rights Reserved.
* Qualcomm Technologies Proprietary and Confidential.
*
******************************************************************************/

#include <Uefi.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DxeServicesLib.h>
#include <Library/DxeServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/IoLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/RamDiskLib.h>
#include <Library/UefiCfgLib.h>

#include <Protocol/BlockIo.h>
#include <Protocol/DevicePath.h>

// --------------------------------------------------------------------------------

//  Forward definitions:

EFI_STATUS
EFIAPI
RamDiskReset (
  IN EFI_BLOCK_IO_PROTOCOL          *This,
  IN BOOLEAN                        ExtendedVerification
  );

EFI_STATUS
EFIAPI
RamDiskReadBlocks (
  IN EFI_BLOCK_IO_PROTOCOL          *This,
  IN UINT32                         MediaId,
  IN EFI_LBA                        Lba,
  IN UINTN                          BufferSize,
  OUT VOID                          *Buffer
  );

EFI_STATUS
EFIAPI
RamDiskWriteBlocks (
  IN EFI_BLOCK_IO_PROTOCOL          *This,
  IN UINT32                         MediaId,
  IN EFI_LBA                        Lba,
  IN UINTN                          BufferSize,
  IN VOID                           *Buffer
  );

EFI_STATUS
EFIAPI
RamDiskFlushBlocks (
  IN EFI_BLOCK_IO_PROTOCOL  *This
  );

EFI_STATUS
RamDiskInitialize (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  );

// --------------------------------------------------------------------------------

//  The EFI_BLOCK_IO_MEDIA structure pointed to by our EFI_BLOCK_IO_PROTOCOL

EFI_BLOCK_IO_MEDIA gRamDiskMedia = {
  SIGNATURE_32('r','a','m','d'),  // MediaId
  FALSE,                          // RemovableMedia
  TRUE,                           // MediaPresent
  FALSE,                          // PhysicalPartition
  FALSE,                          // ReadOnly
  FALSE,                          // WriteCaching
  RAMDISK_BLOCK_SIZE,             // BlockSize
  4,                              // IoAlign
  0,                              // Pad
  0                               // LastBlock (to be filled in)
};

// --------------------------------------------------------------------------------

//  The EFI_BLOCK_IO_PROTOCOL that we register for our RamDisk

EFI_BLOCK_IO_PROTOCOL gRamDiskBlockIoProtocol = {
  EFI_BLOCK_IO_INTERFACE_REVISION,   // Revision
  &gRamDiskMedia,                    // Media
  RamDiskReset,                      // Reset
  RamDiskReadBlocks,                 // ReadBlocks
  RamDiskWriteBlocks,                // WriteBlocks
  RamDiskFlushBlocks                 // FlushBlocks
};

// --------------------------------------------------------------------------------

//  The EFI_DEVICE_PATH_PROTOCOL that we register for our RamDisk

typedef struct {
  MEMMAP_DEVICE_PATH           RamDisk;
  EFI_DEVICE_PATH_PROTOCOL     End;
} RAMDISK_DEVICE_PATH;

RAMDISK_DEVICE_PATH gRamDiskDevicePathProtocol = 
{
  {
    {
      HARDWARE_DEVICE_PATH,
      HW_MEMMAP_DP,
      {
        (UINT8)(sizeof(MEMMAP_DEVICE_PATH)),
        (UINT8)((sizeof(MEMMAP_DEVICE_PATH)) >> 8)
      }
    },
    EfiBootServicesData,
    (EFI_PHYSICAL_ADDRESS) 0,
    (EFI_PHYSICAL_ADDRESS) 0
  },
  {
    END_DEVICE_PATH_TYPE,
    END_ENTIRE_DEVICE_PATH_SUBTYPE,
    {
      (UINT8)(sizeof (EFI_DEVICE_PATH_PROTOCOL)),
      0
    }
  }
};

// --------------------------------------------------------------------------------

// Other globals:

EFI_HANDLE gSystemHandle;
EFI_HANDLE gRamDiskHandle;             // handle to our RamDisk device

UINT8* gRamDiskBuffer;                  // ptr to content of the RAM disk
UINTN gRamDiskSizeBytes;               // size of the RAM disk, in bytes


// --------------------------------------------------------------------------------

EFI_STATUS
EFIAPI
RamDiskReset (
  IN EFI_BLOCK_IO_PROTOCOL          *This,
  IN BOOLEAN                        ExtendedVerification
  )
{
  return EFI_SUCCESS;
}

// --------------------------------------------------------------------------------

EFI_STATUS
EFIAPI
RamDiskReadBlocks (
  IN EFI_BLOCK_IO_PROTOCOL          *This,
  IN UINT32                         MediaId,
  IN EFI_LBA                        Lba,
  IN UINTN                          BufferSize,
  OUT VOID                          *Buffer
  )
{
  UINTN Offset;

  //DEBUG((EFI_D_ERROR,"RamDiskReadBlocks Lba=0x%x, BufferSize=0x%x\n", (UINTN)Lba, BufferSize));

  Offset = (UINTN)Lba * gRamDiskMedia.BlockSize;
  if ((Offset >= gRamDiskSizeBytes) || (BufferSize > gRamDiskSizeBytes - Offset)) {
    return EFI_BAD_BUFFER_SIZE;
  }

  CopyMem(Buffer, gRamDiskBuffer + Offset, BufferSize );

  return EFI_SUCCESS;
}

// --------------------------------------------------------------------------------

EFI_STATUS
EFIAPI
RamDiskWriteBlocks (
  IN EFI_BLOCK_IO_PROTOCOL          *This,
  IN UINT32                         MediaId,
  IN EFI_LBA                        Lba,
  IN UINTN                          BufferSize,
  OUT VOID                          *Buffer
  )
{
  UINTN Offset;

  //DEBUG((EFI_D_ERROR,"RamDiskWriteBlocks Lba=0x%x, BufferSize=0x%x\n", (UINTN)Lba, BufferSize));

  Offset = (UINTN)Lba * gRamDiskMedia.BlockSize;
  if ((Offset >= gRamDiskSizeBytes) || (BufferSize > gRamDiskSizeBytes - Offset)) {
    return EFI_BAD_BUFFER_SIZE;
  }

  CopyMem(gRamDiskBuffer + Offset, Buffer, BufferSize );

  return EFI_SUCCESS;
}

// --------------------------------------------------------------------------------

EFI_STATUS
EFIAPI
RamDiskFlushBlocks (
  IN EFI_BLOCK_IO_PROTOCOL  *This
  )
{
  return EFI_SUCCESS;
}

// --------------------------------------------------------------------------------

//  Main entry point for this driver
EFI_STATUS
RamDiskInitialize (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS                     Status;
  UINT64                         RamDiskBase;

  Status = GetConfigValue64 ("RamDiskBase", &RamDiskBase);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  gRamDiskBuffer = (VOID *)(UINTN)RamDiskBase;

  if (0 == gRamDiskBuffer) {
    return EFI_NOT_FOUND;
  }

  Status = ValidateRamDisk(gRamDiskBuffer, &gRamDiskSizeBytes);
  if (EFI_ERROR(Status)) {
    return Status;
  } 

  gRamDiskMedia.LastBlock = (gRamDiskSizeBytes / gRamDiskMedia.BlockSize) - 1;
  gRamDiskDevicePathProtocol.RamDisk.StartingAddress = (EFI_PHYSICAL_ADDRESS)(UINTN)gRamDiskBuffer;
  gRamDiskDevicePathProtocol.RamDisk.EndingAddress = gRamDiskDevicePathProtocol.RamDisk.StartingAddress + gRamDiskSizeBytes;

  Status = gBS->InstallMultipleProtocolInterfaces(&gRamDiskHandle,
                                                  &gEfiBlockIoProtocolGuid, &gRamDiskBlockIoProtocol,
                                                  &gEfiDevicePathProtocolGuid, &gRamDiskDevicePathProtocol,
                                                  NULL);
  if (EFI_ERROR(Status)) {
    DEBUG((EFI_D_ERROR, "RamDiskInitialize: Error 0x%x from InstallMultipleProtocolInterfaces\n", Status));
    return Status;
  }

  DEBUG((EFI_D_ERROR, "RamDiskInitialize: Handle=0x%x, BlockIo=0x%x, DevicePath=0x%x, Buffer=0x%x, Size = 0x%x\n",
                       gRamDiskHandle, &gRamDiskBlockIoProtocol, &gRamDiskDevicePathProtocol,
                       gRamDiskBuffer, gRamDiskSizeBytes));

  return EFI_SUCCESS;
}
