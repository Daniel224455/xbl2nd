/** @file
  This file includes methods for loading and decompression of FV

  Copyright (c) 2013 - 2014, 2017-2018 Qualcomm Technologies, Inc. All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential.
  Portions copyright (c) 2004 - 2008, Intel Corporation. All rights reserved.
  
  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php
  
  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
 
**/

/*=============================================================================
                              EDIT HISTORY


 when       who      what, where, why
 --------   ---      ----------------------------------------------------------
 02/14/18   ps       Support for GUIDed FV loading
 09/08/17   bd       Support for authenticated FV loading
 08/12/16   bh       Get RAM loaded FV address from memmap
 06/22/16   bh       Add tools FV mounting from different lib
 07/09/15   bh       Remove assert
 12/11/14   bh       Moved location to QcomLib
 10/14/14   jb       Change FD PCD to 64bit
 04/03/14   vk       Fix 64 bit compiler warnings
 01/06/14   aus      Fixed 64 bit program header format
 11/28/13   aus      Added 64 bit elf support
 11/01/13   aus      Added support to load debug FV is uefi is an elf image
 11/01/13   vk       Fix 64 bit compiler warnings
 05/08/13   aus      Read FV info from RAM and if that fails read from EMMC
 04/08/13   aus      Created new file
=============================================================================*/


/*=========================================================================
      Include Files
==========================================================================*/
#include <Uefi.h>

#include <Library/IoLib.h>
#include <Library/PcdLib.h>
#include <Library/BaseMemoryLib.h>
#include <Pi/PiFirmwareVolume.h>
#include <Pi/PiFirmwareFile.h>
#include <Library/UncachedMemoryAllocationLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/ArmLib.h>
#include <Library/PcdLib.h>
#include <Library/QcomBaseLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DevicePathLib.h>
#include <Library/ExtractGuidedSectionLib.h>
#include <Protocol/FirmwareVolumeBlock.h>
#include <Guid/FileSystemVolumeLabelInfo.h>
#include <Guid/LzmaDecompress.h>
#include "QcomLib.h"
#include <Library/GuidedFvLib.h>
#include <Library/QcomBdsLib.h>
#include <MemRegionInfo.h>
#include <Library/UefiCfgLib.h>

#define FV_INFO_SECTOR_MAGIC_NUMBER 0x462E562E // F.V.
#define MAX_FV_COUNT                8
#define FVB_DEVICE_SIGNATURE        SIGNATURE_32('_','F','V','B')
#define GET_OCCUPIED_SIZE(ActualSize, Alignment) \
  (ActualSize) + (((Alignment) - ((ActualSize) & ((Alignment) - 1))) & ((Alignment) - 1))

// 
// MBN header structure
//  
typedef struct
{
  UINT32 ImageId;           /* Identifies the type of image this header
                               represents (OEM SBL, AMSS, Apps boot loader,
                               etc.). */
  UINT32 HeaderVersion;     /* Header version number. */
  UINT32 ImageSrc;          /* Location of image in flash: Address of
                               image in NOR or page/sector offset to image
                               from page/sector 0 in NAND/SUPERAND. */
  UINT32 ImageDest;         /* Pointer to location to store image in RAM.
                               Also, entry point at which image execution
                               begins. */
  UINT32 ImageSize;         /* Size of complete image in bytes */
  UINT32 CodeSize;          /* Size of code region of image in bytes */
  UINT32 SignaturePtr;      /* Pointer to images attestation signature */
  UINT32 SignatureSize;     /* Size of the attestation signature in
                               bytes */
  UINT32 CertChainPtr;      /* Pointer to the chain of attestation
                               certificates associated with the image. */
  UINT32 CertChainSize;     /* Size of the attestation chain in bytes */

} ImageHeaderType;

//
// ELF header structure
//

//
// Fields in the e_ident array.  The ELFINFO_*_INDEX macros are 
// indices into the array.  The macros under each ELFINFO_* macro
// is the values the byte may have.  
//
#define ELFINFO_MAGIC_SIZE    16
#define ELFINFO_MAG0_INDEX    0     /* File identification byte 0 index */
#define ELFINFO_MAG0          0x7f  /* Magic number byte 0              */
#define ELFINFO_MAG1_INDEX    1     /* File identification byte 1 index */
#define ELFINFO_MAG1         'E'    /* Magic number byte 1              */
#define ELFINFO_MAG2_INDEX    2     /* File identification byte 2 index */
#define ELFINFO_MAG2         'L'    /* Magic number byte 2              */
#define ELFINFO_MAG3_INDEX    3     /* File identification byte 3 index */
#define ELFINFO_MAG3         'F'    /* Magic number byte 3              */
#define ELFINFO_CLASS_INDEX   4     /* File class byte index            */
#define ELF_CLASS_32          1     /* 32-bit objects                   */
#define ELF_CLASS_64          2     /* 64-bit objects                   */
#define ELFINFO_VERSION_INDEX 6     /* File version byte index          */
#define ELF_VERSION_CURRENT   1     /* Current version                  */

typedef struct
{
  UINT8  e_ident[ELFINFO_MAGIC_SIZE]; /* Magic number and other info       */
  UINT16 e_type;                      /* Object file type                  */
  UINT16 e_machine;                   /* Architecture                      */
  UINT32 e_version;                   /* Object file version               */
  UINTN  e_entry;                     /* Entry point virtual address       */
  UINTN  e_phoff;                     /* Program header table file offset  */
  UINTN  e_shoff;                     /* Section header table file offset  */
  UINT32 e_flags;                     /* Processor-specific flags          */
  UINT16 e_ehsize;                    /* ELF header size in bytes          */
  UINT16 e_phentsize;                 /* Program header table entry size   */
  UINT16 e_phnum;                     /* Program header table entry count  */
  UINT16 e_shentsize;                 /* Section header table entry size   */
  UINT16 e_shnum;                     /* Section header table entry count  */
  UINT16 e_shstrndx;                  /* Section header string table index */
} Elf_Ehdr;

//
// Program segment header.  
//
typedef struct
{
  UINT32 p_type;    /* Segment type */
  UINT32 p_offset;  /* Segment file offset */
  UINTN  p_vaddr;   /* Segment virtual address */
  UINTN  p_paddr;   /* Segment physical address */
  UINTN  p_filesz;  /* Segment size in file */
  UINTN  p_memsz;   /* Segment size in memory */
  UINTN  p_flags;   /* Segment flags */
  UINTN  p_align;   /* Segment alignment */
} Elf32_Phdr;


typedef struct
{
  UINT32 p_type;    /* Segment type */
  UINT32 p_flags;   /* Segment flags */  
  UINTN  p_offset;  /* Segment file offset */
  UINTN  p_vaddr;   /* Segment virtual address */
  UINTN  p_paddr;   /* Segment physical address */
  UINTN  p_filesz;  /* Segment size in file */
  UINTN  p_memsz;   /* Segment size in memory */
  UINTN  p_align;   /* Segment alignment */
} Elf64_Phdr;

// 
// FV info structure
//  
typedef struct
{
  UINT32 MagicNumber;                  /* Magic Number FV_INFO_SECTOR_MAGIC_NUMBER */
  UINT32 Version;                      /* Structure version number. */
  UINT32 NumberOfFvs;                  /* Number of FVs */
  UINT32 FvSizeInBytes [MAX_FV_COUNT];  /* Size of each FV that is appended */
} FvInfoSectorType;

// UEFI Partition GUID
EFI_GUID UefiPartitionGuid = { 0x400FFDCD, 0x22E0, 0x47E7, { 0x9A, 0x23, 0xF1, 0x6E, 0xD9, 0x38, 0x23, 0x88 } };
  
//
// UEFI Specification structures for FV LBA cache, device path and FVB device format
//  
typedef struct {
  UINTN                       Base;
  UINTN                       Length;
} LBA_CACHE;

typedef struct {
  MEMMAP_DEVICE_PATH          MemMapDevPath;
  EFI_DEVICE_PATH_PROTOCOL    EndDevPath;
} FV_MEMMAP_DEVICE_PATH;

typedef struct {
  MEDIA_FW_VOL_DEVICE_PATH    FvDevPath;
  EFI_DEVICE_PATH_PROTOCOL    EndDevPath;
} FV_PIWG_DEVICE_PATH;

typedef struct {
  UINTN                                 Signature;
  EFI_HANDLE                            Handle;
  EFI_DEVICE_PATH_PROTOCOL              *DevicePath;
  EFI_FIRMWARE_VOLUME_BLOCK_PROTOCOL    FwVolBlockInstance;
  UINTN                                 NumBlocks;
  LBA_CACHE                             *LbaCache;
  UINT32                                FvbAttributes;
  EFI_PHYSICAL_ADDRESS                  BaseAddress;
  UINT32                                AuthenticationStatus;
} EFI_FW_VOL_BLOCK_DEVICE;

// 
// Global data structures and variables
//
FV_MEMMAP_DEVICE_PATH mFvMemmapDevicePathTemplate = {
  {
    {
      HARDWARE_DEVICE_PATH,
      HW_MEMMAP_DP,
      {
        (UINT8)(sizeof (MEMMAP_DEVICE_PATH)),
        (UINT8)(sizeof (MEMMAP_DEVICE_PATH) >> 8)
      }
    },
    EfiMemoryMappedIO,
    (EFI_PHYSICAL_ADDRESS) 0,
    (EFI_PHYSICAL_ADDRESS) 0,
  },
  {
    END_DEVICE_PATH_TYPE,
    END_ENTIRE_DEVICE_PATH_SUBTYPE,
    {
      END_DEVICE_PATH_LENGTH,
      0
    }
  }
};

FV_PIWG_DEVICE_PATH mFvPIWGDevicePathTemplate = {
  {
    {
      MEDIA_DEVICE_PATH,
      MEDIA_PIWG_FW_VOL_DP,
      {
        (UINT8)(sizeof (MEDIA_FW_VOL_DEVICE_PATH)),
        (UINT8)(sizeof (MEDIA_FW_VOL_DEVICE_PATH) >> 8)
      }
    },
    { 0 }
  },
  {
    END_DEVICE_PATH_TYPE,
    END_ENTIRE_DEVICE_PATH_SUBTYPE,
    {
      END_DEVICE_PATH_LENGTH,
      0
    }
  }
};

EFI_FW_VOL_BLOCK_DEVICE  mFwVolBlock = {
  FVB_DEVICE_SIGNATURE,
  NULL,
  NULL,
  {
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL
  },
  0,
  NULL,
  0,
  0,
  0
};

EFI_FIRMWARE_VOLUME_BLOCK_PROTOCOL *FwVolBlkProtocol = NULL;
STATIC HandleInfo HandleInfoList[2];
STATIC UINT32 MaxHandles;
STATIC PartiSelectFilter HandleFilter;

// 
// Extern functions
//
extern RETURN_STATUS EFIAPI LzmaUefiDecompressGetInfo (
  IN  CONST VOID  *Source,
  IN  UINT32      SourceSize,
  OUT UINT32      *DestinationSize,
  OUT UINT32      *ScratchSize
  );
  
extern RETURN_STATUS EFIAPI LzmaUefiDecompress (
  IN CONST VOID  *Source,
  IN UINTN       SourceSize,
  IN OUT VOID    *Destination,
  IN OUT VOID    *Scratch
  );
  
extern RETURN_STATUS EFIAPI LzmaGuidedSectionGetInfo (
  IN  CONST VOID  *InputSection,
  OUT UINT32      *OutputBufferSize,
  OUT UINT32      *ScratchBufferSize,
  OUT UINT16      *SectionAttribute
  );
  
extern RETURN_STATUS EFIAPI LzmaGuidedSectionExtraction (
  IN CONST  VOID    *InputSection,
  OUT       VOID    **OutputBuffer,
  OUT       VOID    *ScratchBuffer,        OPTIONAL
  OUT       UINT32  *AuthenticationStatus
  );

/**
  This routine produces a firmware volume block protocol on a given
  buffer. 
  Function used from /MdeModulePkg/Core/Dxe/FwVolBlock.c

  @param  BaseAddress            base address of the firmware volume image
  @param  Length                 length of the firmware volume image
  @param  ParentHandle           handle of parent firmware volume, if this image
                                 came from an FV image file and section in another firmware
                                 volume (ala capsules)
  @param  AuthenticationStatus   Authentication status inherited, if this image
                                 came from an FV image file and section in another firmware volume.

  @retval EFI_VOLUME_CORRUPTED   Volume corrupted.
  @retval EFI_OUT_OF_RESOURCES   No enough buffer to be allocated.
  @retval EFI_SUCCESS            Successfully produced a FVB protocol on given
                                 buffer.

**/
EFI_STATUS
ProduceFVBProtocolOnBuffer (
  IN EFI_PHYSICAL_ADDRESS   BaseAddress,
  IN UINT64                 Length,
  IN EFI_HANDLE             ParentHandle,
  IN UINT32                 AuthenticationStatus
  )
{
  EFI_FW_VOL_BLOCK_DEVICE       *FvbDev;
  EFI_FIRMWARE_VOLUME_HEADER    *FwVolHeader;
  UINTN                         BlockIndex;
  UINTN                         BlockIndex2;
  UINTN                         LinearOffset;
  UINT32                        FvAlignment;
  EFI_FV_BLOCK_MAP_ENTRY        *PtrBlockMapEntry;
  EFI_STATUS                    Status;

  FvAlignment = 0;
  FwVolHeader = (EFI_FIRMWARE_VOLUME_HEADER *)(UINTN) BaseAddress;
  //
  // Validate FV Header, if not as expected, return
  //
  if (FwVolHeader->Signature != EFI_FVH_SIGNATURE) {
    return EFI_VOLUME_CORRUPTED;
  }
  //
  // Get FvHeader alignment
  //
  FvAlignment = 1 << ((FwVolHeader->Attributes & EFI_FVB2_ALIGNMENT) >> 16);
  //
  // FvAlignment must be greater than or equal to 8 bytes of the minimum FFS alignment value. 
  //
  if (FvAlignment < 8) {
    FvAlignment = 8;
  }
  if ((UINTN)BaseAddress % FvAlignment != 0) {
    //
    // FvImage buffer is not at its required alignment.
    //
    return EFI_VOLUME_CORRUPTED;
  }
    
  Status = gBS->LocateProtocol(&gEfiFirmwareVolumeBlockProtocolGuid, 
                               NULL,
                               (VOID**) &FwVolBlkProtocol);  
  ASSERT_EFI_ERROR (Status);
  
  CopyMem(&mFwVolBlock.FwVolBlockInstance, FwVolBlkProtocol, sizeof(EFI_FIRMWARE_VOLUME_BLOCK_PROTOCOL));

  //
  // Allocate EFI_FW_VOL_BLOCK_DEVICE
  //
  FvbDev = AllocateCopyPool (sizeof (EFI_FW_VOL_BLOCK_DEVICE), &mFwVolBlock);
  if (FvbDev == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }  
  
  FvbDev->BaseAddress   = BaseAddress;
  FvbDev->FvbAttributes = FwVolHeader->Attributes;
  FvbDev->FwVolBlockInstance.ParentHandle = ParentHandle;
  if (ParentHandle != NULL) {
    FvbDev->AuthenticationStatus = AuthenticationStatus;
  }

  //
  // Init the block caching fields of the device
  // First, count the number of blocks
  //
  FvbDev->NumBlocks = 0;
  for (PtrBlockMapEntry = FwVolHeader->BlockMap;
       PtrBlockMapEntry->NumBlocks != 0;
       PtrBlockMapEntry++) {
    FvbDev->NumBlocks += PtrBlockMapEntry->NumBlocks;
  }
  //
  // Second, allocate the cache
  //
  FvbDev->LbaCache = AllocatePool (FvbDev->NumBlocks * sizeof (LBA_CACHE));
  if (FvbDev->LbaCache == NULL) {
    FreePool (FvbDev);
    return EFI_OUT_OF_RESOURCES;
  }
  
  //
  // Last, fill in the cache with the linear address of the blocks
  //
  BlockIndex = 0;
  LinearOffset = 0;
  for (PtrBlockMapEntry = FwVolHeader->BlockMap;
       PtrBlockMapEntry->NumBlocks != 0; PtrBlockMapEntry++) {
    for (BlockIndex2 = 0; BlockIndex2 < PtrBlockMapEntry->NumBlocks; BlockIndex2++) {
      FvbDev->LbaCache[BlockIndex].Base = LinearOffset;
      FvbDev->LbaCache[BlockIndex].Length = PtrBlockMapEntry->Length;
      LinearOffset += PtrBlockMapEntry->Length;
      BlockIndex++;
    }
  }

  //
  // Judge whether FV name guid is produced in Fv extension header
  //
  if (FwVolHeader->ExtHeaderOffset == 0) {
    //
    // FV does not contains extension header, then produce MEMMAP_DEVICE_PATH
    //
    FvbDev->DevicePath = (EFI_DEVICE_PATH_PROTOCOL *) AllocateCopyPool (sizeof (FV_MEMMAP_DEVICE_PATH), &mFvMemmapDevicePathTemplate);
    if (FvbDev->DevicePath == NULL) {
      FreePool (FvbDev);
      return EFI_OUT_OF_RESOURCES;
    }
    ((FV_MEMMAP_DEVICE_PATH *) FvbDev->DevicePath)->MemMapDevPath.StartingAddress = BaseAddress;
    ((FV_MEMMAP_DEVICE_PATH *) FvbDev->DevicePath)->MemMapDevPath.EndingAddress   = BaseAddress + FwVolHeader->FvLength - 1;
  } else {
    //
    // FV contains extension header, then produce MEDIA_FW_VOL_DEVICE_PATH
    //
    FvbDev->DevicePath = (EFI_DEVICE_PATH_PROTOCOL *) AllocateCopyPool (sizeof (FV_PIWG_DEVICE_PATH), &mFvPIWGDevicePathTemplate);
    if (FvbDev->DevicePath == NULL) {
      FreePool (FvbDev);
      return EFI_OUT_OF_RESOURCES;
    }
    CopyGuid (
      &((FV_PIWG_DEVICE_PATH *)FvbDev->DevicePath)->FvDevPath.FvName, 
      (GUID *)(UINTN)(BaseAddress + FwVolHeader->ExtHeaderOffset)
      );
  }
  
  //
  //
  // Attach FvVolBlock Protocol to new handle
  //
  Status = gBS->InstallMultipleProtocolInterfaces (
             &FvbDev->Handle,
             &gEfiFirmwareVolumeBlockProtocolGuid,     &FvbDev->FwVolBlockInstance,
             &gEfiDevicePathProtocolGuid,              FvbDev->DevicePath,
             NULL
             );

  return Status;
  
}


/**
  Go through the file to search SectionType section,
  when meeting an encapsuled section. 
  Function used from EmbeddedPkg/Library/PrePiLib/FwVol.c
  
  @param  SectionType  - Filter to find only section of this type.
  @param  Section      - From where to search.
  @param  SectionSize  - The file size to search.
  @param  OutputBuffer - Pointer to the section to search.
                      
  @retval EFI_SUCCESS
**/
EFI_STATUS
FfsProcessSection (
  IN EFI_SECTION_TYPE           SectionType,
  IN EFI_COMMON_SECTION_HEADER  *Section,
  IN UINTN                      SectionSize,
  OUT VOID                      **OutputBuffer
  )
{
  EFI_STATUS                              Status;
  UINT32                                  SectionLength = 0;
  UINT32                                  ParsedLength;
  UINT32                                  DstBufferSize = 0;
  VOID                                    *ScratchBuffer = NULL;
  UINT32                                  ScratchBufferSize = 0;
  VOID                                    *DstBuffer = NULL;
  UINT16                                  SectionAttribute;
  UINT32                                  AuthenticationStatus;


  *OutputBuffer = NULL;
  ParsedLength  = 0;
  Status        = EFI_NOT_FOUND;
  while (ParsedLength < SectionSize) {
    if (Section->Type == SectionType) {
      *OutputBuffer = (VOID *)(Section + 1);

      return EFI_SUCCESS;
    } else if (Section->Type == EFI_SECTION_GUID_DEFINED) {
    
      if (Section->Type == EFI_SECTION_GUID_DEFINED) {
        Status = ExtractGuidedSectionGetInfo (
                   Section,
                   &DstBufferSize,
                   &ScratchBufferSize,
                   &SectionAttribute
                   );
      }
      
      if (EFI_ERROR (Status)) {
        //
        // GetInfo failed
        //
        DEBUG ((EFI_D_ERROR, "Decompress GetInfo Failed - %r\n", Status));
        return EFI_NOT_FOUND;
      }
      //
      // Allocate scratch buffer
      //
      ScratchBuffer = (VOID *)(UINTN)AllocatePages (EFI_SIZE_TO_PAGES (ScratchBufferSize));
      if (ScratchBuffer == NULL) {
        return EFI_OUT_OF_RESOURCES;
      }
      //
      // Allocate destination buffer, extra one page for adjustment 
      //
      DstBuffer = (VOID *)(UINTN)AllocatePages (EFI_SIZE_TO_PAGES (DstBufferSize) + 1);
      if (DstBuffer == NULL) {
        return EFI_OUT_OF_RESOURCES;
      }
      //
      // DstBuffer still is one section. Adjust DstBuffer offset, skip EFI section header
      // to make section data at page alignment.
      //
      DstBuffer = (UINT8 *)DstBuffer + EFI_PAGE_SIZE - sizeof (EFI_COMMON_SECTION_HEADER);
      //
      // Call decompress function
      //
      if (Section->Type == EFI_SECTION_GUID_DEFINED) {
        Status = ExtractGuidedSectionDecode (
                    Section,
                    &DstBuffer,
                    ScratchBuffer,
                    &AuthenticationStatus
                    );
      }
      
      if (EFI_ERROR (Status)) {
        //
        // Decompress failed
        //
        DEBUG ((EFI_D_ERROR, "Decompress Failed - %r\n", Status));
        return EFI_NOT_FOUND;
      } else {
        return FfsProcessSection (
                SectionType, 
                DstBuffer, 
                DstBufferSize, 
                OutputBuffer 
                );
       }        
    }

    //
    // Size is 24 bits wide so mask upper 8 bits. 
    // SectionLength is adjusted it is 4 byte aligned.
    // Go to the next section
    //
    SectionLength = *(UINT32 *)Section->Size & 0x00FFFFFF;
    SectionLength = GET_OCCUPIED_SIZE (SectionLength, 4);
    if (SectionLength == 0)
      return EFI_NOT_FOUND;
    
    ParsedLength += SectionLength;
    Section = (EFI_COMMON_SECTION_HEADER *)((UINT8 *)Section + SectionLength);
  }
  
  return EFI_NOT_FOUND;
}

/**
  Process the buffer to obtain the decompressed buffer and use that to produce the 
  FVB protocol on the buffer.
  Parts of the function used from MdeModulePkg/Core/Dxe/Dispatcher/Dispatcher.c
  Parts of the function used from EmbeddedPkg/Library/PrePiLib/FwVol.c

  @param  Buffer                The FV buffer to be processed

  @retval EFI_OUT_OF_RESOURCES  No enough memory or other resource.
  @retval EFI_VOLUME_CORRUPTED  Corrupted volume.
  @retval EFI_SUCCESS           Function successfully returned.
  @retval EFI_INVALID_PARAMETER Invalid parameters

**/
EFI_STATUS
ProcessFvImageFile (
  IN  VOID                            *Buffer
  )
{
  EFI_STATUS                          Status = EFI_SUCCESS;
  VOID                                *AlignedBuffer = NULL;
  EFI_FIRMWARE_VOLUME_HEADER          *FvHeader;
  UINT32                              FvAlignment;
  VOID                                *OutputBuffer = NULL;
  EFI_FFS_FILE_HEADER                 *FfsFileHeader;
  UINT32                              FileSize;
  EFI_COMMON_SECTION_HEADER           *Section;
  
  if (Buffer == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  
  //
  // Read the first (and only the first) firmware volume section
  //
  FvHeader = (EFI_FIRMWARE_VOLUME_HEADER *) Buffer;  
  
  // FfsFile Header is located after FV Header
  FfsFileHeader = (EFI_FFS_FILE_HEADER *)( (UINTN)FvHeader + (UINTN)FvHeader->HeaderLength);

  //
  // Size is 24 bits wide so mask upper 8 bits. 
  // Does not include FfsFileHeader header size
  // FileSize is adjusted to FileOccupiedSize as it is 8 byte aligned.
  //
  Section = (EFI_COMMON_SECTION_HEADER *)(FfsFileHeader + 1);
  FileSize = *(UINT32 *)(FfsFileHeader->Size) & 0x00FFFFFF;
  FileSize -= sizeof (EFI_FFS_FILE_HEADER);

  // Decompress the Volume image, if required
  Status = FfsProcessSection (  EFI_SECTION_FIRMWARE_VOLUME_IMAGE,
                                Section,
                                FileSize,
                                &OutputBuffer);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  

  if (OutputBuffer != NULL) {
    
    //
    // FvImage should be at its required alignment.
    //
    FvHeader = (EFI_FIRMWARE_VOLUME_HEADER *) OutputBuffer;
    //
    // Get FvHeader alignment
    //
    FvAlignment = 1 << ((FvHeader->Attributes & EFI_FVB2_ALIGNMENT) >> 16);
    //
    // FvAlignment must be greater than or equal to 8 bytes of the minimum FFS alignment value. 
    //
    if (FvAlignment < 8) {
      FvAlignment = 8;
    }
    //
    // Allocate the aligned buffer for the FvImage.
    //
    AlignedBuffer = AllocateAlignedPages (EFI_SIZE_TO_PAGES (FvHeader->FvLength), (UINTN) FvAlignment);
    if (AlignedBuffer == NULL) {
      Status = EFI_OUT_OF_RESOURCES;
    } else {
      //
      // Move FvImage into the aligned buffer and release the original buffer.
      //
      CopyMem (AlignedBuffer, OutputBuffer, FvHeader->FvLength);

      //
      // Produce a FVB protocol for the file
      //
      Status = ProduceFVBProtocolOnBuffer (
                 (EFI_PHYSICAL_ADDRESS) (UINTN) AlignedBuffer,
                 (UINT64)FvHeader->FvLength,
                 NULL,
                 0
                 ); 
    }
  }

  if (EFI_ERROR (Status)) {
    if (AlignedBuffer != NULL) {
      FreeAlignedPages (AlignedBuffer, EFI_SIZE_TO_PAGES (FvHeader->FvLength));
    }

    if(OutputBuffer != NULL) {
      FreePool(OutputBuffer);
    }
  }

  return Status;
}

/**
  This function verifies the ELF Header by sanity checking various fields
  within the ELF Header structure 

  @retval TRUE      If ELF Header is valid
  @retval FALSE     If ELF header is invalid

**/
BOOLEAN
VerifyElfHeader(Elf_Ehdr *ehdr)
{
  return ( ehdr->e_ident[ELFINFO_MAG0_INDEX] == ELFINFO_MAG0 && 
           ehdr->e_ident[ELFINFO_MAG1_INDEX] == ELFINFO_MAG1 &&
           ehdr->e_ident[ELFINFO_MAG2_INDEX] == ELFINFO_MAG2 &&
           ehdr->e_ident[ELFINFO_MAG3_INDEX] == ELFINFO_MAG3 &&
	       ehdr->e_ehsize == sizeof(Elf_Ehdr) &&
           ehdr->e_ident[ELFINFO_VERSION_INDEX] == ELF_VERSION_CURRENT &&
          ((ehdr->e_ident[ELFINFO_CLASS_INDEX] == ELF_CLASS_32 && 
		    ehdr->e_phentsize == sizeof(Elf32_Phdr)) || 
	       (ehdr->e_ident[ELFINFO_CLASS_INDEX] == ELF_CLASS_64 &&  
		    ehdr->e_phentsize == sizeof(Elf64_Phdr))) );
} 

/**
  Load the Debug FV image by parsing the MBN header to locate the end of the UEFI image. 
  Parse the FV info sector to obtain the number of FVs appended. Then load and process 
  every FV that is appended.

  @retval EFI_NOT_FOUND         Couldn't locate the MBN imformation
  @retval EFI_OUT_OF_RESOURCES  Out of memory.
  @retval EFI_SUCCESS           Function successfully returned.
  @retval EFI_INVALID_PARAMETER Invalid parameter.

**/
EFI_STATUS
LoadDebugFv( VOID )
{
  UINT32                  Attribs = 0;
  EFI_STATUS              Status = EFI_SUCCESS;
  EFI_BLOCK_IO_PROTOCOL   *BlkIo;
  ImageHeaderType         *MbnHeaderPtr = NULL;
  FvInfoSectorType        *FvInfoPtr = NULL;
  UINT8                   *SectorBuff = NULL, *FvBuffer = NULL, *TempFvBuffer;
  UINT32                  FvInfoSector, FvSectorStart, BlkSize = 0;
  UINT32                  CurrentFvCount, FvTotalSizeInBytes = 0;
  UINTN                   TempRamBuff = 0;
  BOOLEAN                 ReadFromRAM = TRUE;
  UINT32                  HeaderSize = 0, ImageSize = 0;
  Elf_Ehdr                *ElfHeaderPtr = NULL;
  Elf32_Phdr              *Phdr32Ptr = NULL;
  Elf64_Phdr              *Phdr64Ptr = NULL;
  
  // Select the BlkIo handle that represents the partition by the
  // referenced GUID type in GPT partition on Non removable media
  // ie eMMC device 
  Attribs |= BLK_IO_SEL_MEDIA_TYPE_NON_REMOVABLE;
  Attribs |= BLK_IO_SEL_PARTITIONED_GPT;
  Attribs |= BLK_IO_SEL_MATCH_PARTITION_TYPE_GUID;

  MaxHandles = sizeof(HandleInfoList)/sizeof(*HandleInfoList);
  HandleFilter.PartitionType = &UefiPartitionGuid;
  HandleFilter.VolumeName = 0;

  Status = GetBlkIOHandles (Attribs, &HandleFilter, HandleInfoList, &MaxHandles);
  if (Status != EFI_SUCCESS)
  {
    goto LoadFvImageExit;
  }

  // Having more than one partition is considered ambiguity, return error 
  if (MaxHandles > 1) { 
    Status = EFI_NO_MAPPING;
    goto LoadFvImageExit;
  }
  
  // If no partition is found, return error
  if (MaxHandles == 0) { 
    Status = EFI_NOT_FOUND;
    goto LoadFvImageExit;
  }  
  
  // Get the Block IO protocol instance from the handle 
  BlkIo = HandleInfoList[0].BlkIo;
  BlkSize = BlkIo->Media->BlockSize;
  
  SectorBuff = AllocatePool(BlkSize);
  if (SectorBuff == NULL)
  {
    Status = EFI_OUT_OF_RESOURCES;
    goto LoadFvImageExit;
  }
  
  // Get the image header to know the end of the UEFI image 
  Status = BlkIo->ReadBlocks (BlkIo, BlkIo->Media->MediaId, 0, BlkSize, SectorBuff);
  if (Status != EFI_SUCCESS)
  {
    goto LoadFvImageExit;
  }

  // Check if UEFI image is an ELF image
  ElfHeaderPtr = (Elf_Ehdr *)SectorBuff;

  if (VerifyElfHeader(ElfHeaderPtr)) 
  {
     // UEFI compiled as ELF image 
	 // The program header corresponding to the actual data segment is the last one. Hence 
	 // we get the headersize to point to the last program header.
     HeaderSize = sizeof(Elf_Ehdr) + ((ElfHeaderPtr->e_phnum-1) * ElfHeaderPtr->e_phentsize);  	 
	 
     // If it is a 32 bit ELF header
	 if (ElfHeaderPtr->e_ident[ELFINFO_CLASS_INDEX] == ELF_CLASS_32) {	 
        Phdr32Ptr = (Elf32_Phdr *) &SectorBuff[HeaderSize];
        ImageSize = Phdr32Ptr->p_memsz + Phdr32Ptr->p_offset;	 
	    HeaderSize = 0;		
	 } 
	 
	 // Else if it is a 64 bit ELF header
	 else {
        Phdr64Ptr = (Elf64_Phdr *) &SectorBuff[HeaderSize];
        ImageSize = Phdr64Ptr->p_memsz + Phdr64Ptr->p_offset;	 
	    HeaderSize = 0;
	 }
  }
  else
  {
     // UEFI compiled as MBN image
     MbnHeaderPtr = (ImageHeaderType *)SectorBuff;
     HeaderSize = sizeof(ImageHeaderType);
     ImageSize = MbnHeaderPtr->ImageSize;
  }
  
  // ImageSize will point to the end of the image. It also points to the beginning of FV information table. 
  // Get the FV information table 
  FvInfoSector = DivU64x32(ImageSize, BlkSize);
  
  if (((ImageSize + HeaderSize)%BlkSize) != 0)
     FvInfoSector += 1;

  //  FV info sector format  
  //===========================================================================
  // magic number: uint32
  // version     : uint32
  // num of FVs  : uint32
  // 
  // FV0 size: 32 bits 
  // FV1 size: 32 bits
  // ....
  // 
  // Sector aligned FV0 data : user defined length 
  // Sector aligned FV1 data : user defined length 
  // ....
  //
  // ==========================================================================
  
  // Try reading the FV info from RAM 
  if (ReadFromRAM) {
      TempRamBuff = (UINTN)PcdGet64(PcdEmbeddedFdBaseAddress) + (UINTN)PcdGet64(PcdEmbeddedFdSize); 
      FvInfoPtr = (FvInfoSectorType *)TempRamBuff;
  
      // Validate the FV info sector 
      // Check Magic number and total number of FVs appended
      if ((FvInfoPtr->MagicNumber != FV_INFO_SECTOR_MAGIC_NUMBER) ||
          (FvInfoPtr->NumberOfFvs >= MAX_FV_COUNT)){
         // Read from RAM failed, hence read from EMMC
         ReadFromRAM = FALSE;
      }  
   }
   
   // Read from EMMC in case read from RAM fails
   if (!ReadFromRAM) {
      Status = BlkIo->ReadBlocks (BlkIo, BlkIo->Media->MediaId, FvInfoSector, BlkSize, SectorBuff);
      if (Status != EFI_SUCCESS) {
         goto LoadFvImageExit;
      }  
  
      FvInfoPtr = (FvInfoSectorType *)SectorBuff;
  
      // Validate the FV info sector 
      // Magic number check 
      if (FvInfoPtr->MagicNumber != FV_INFO_SECTOR_MAGIC_NUMBER) {
         Status = EFI_NOT_FOUND;  
         goto LoadFvImageExit;
      }  
  
      // Check total number of FVs appended
      if (FvInfoPtr->NumberOfFvs >= MAX_FV_COUNT) {
         Status = EFI_INVALID_PARAMETER;  
         goto LoadFvImageExit;
      }  
  }
  
  // Get the total size of all the FVs that need to be read
  CurrentFvCount = 0;
  while (FvInfoPtr->NumberOfFvs != CurrentFvCount) {
    FvTotalSizeInBytes += FvInfoPtr->FvSizeInBytes[CurrentFvCount];
    CurrentFvCount++;
  }

  // Read the FV
  FvBuffer = AllocatePool(FvTotalSizeInBytes);
  if (FvBuffer == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    goto LoadFvImageExit;
  }
  
  if (ReadFromRAM) {
    // FV info sector is sector aligned. Appended FVs start at the sector boundary.
    TempRamBuff += BlkSize;  
    CopyMem (FvBuffer, (VOID *) TempRamBuff, FvTotalSizeInBytes);
  }
  else {
    // FV info sector is sector aligned. Appended FVs start at the sector boundary.
    FvSectorStart = FvInfoSector + 1;  
    Status = BlkIo->ReadBlocks (BlkIo, BlkIo->Media->MediaId, FvSectorStart, FvTotalSizeInBytes, FvBuffer);
    if (Status != EFI_SUCCESS) {
      goto LoadFvImageExit;
    }
  }
  
  // Process each FV 
  CurrentFvCount = 0;
  TempFvBuffer = FvBuffer;
  while (FvInfoPtr->NumberOfFvs != CurrentFvCount) {
    Status = ProcessFvImageFile (TempFvBuffer);
    if (Status != EFI_SUCCESS)
    {
      goto LoadFvImageExit;
    }

    // Script aligns the FVs to sectors and the final size of FV (including the padding to make it sector aligned)
    // is written into the info sector header. Thus the FvSizeInBytes is expected to be sector aligned. 
    // Hence we can obtain the pointer to the next FV by incrementing the current FV Buffer pointer 
    // by the FV size mentioned in the header. 
    TempFvBuffer = TempFvBuffer + FvInfoPtr->FvSizeInBytes[CurrentFvCount];
    CurrentFvCount++; 
  }

LoadFvImageExit:  
  if (SectorBuff != NULL)
    FreePool (SectorBuff);

  if (FvBuffer != NULL)
    FreePool (FvBuffer);

  return Status;
}

#define MAX_PARTI_NAME_LEN 16

EFI_STATUS ReadFromPartition (CHAR16* PartitionLabel, VOID **ReadBuffer, UINTN *ImageSize)
{
  EFI_STATUS             Status = EFI_NOT_FOUND;
  HandleInfo             HandleInfoList[2];
  PartiSelectFilter      FilterData;
  UINT32                 HandleCount, Attrib;
  EFI_BLOCK_IO_PROTOCOL *BlkIo;
  UINTN                  ReadSize = 0;

  Attrib = BLK_IO_SEL_MATCH_PARTITION_LABEL;
  Attrib |= BLK_IO_SEL_STRING_CASE_INSENSITIVE;
  Attrib |= BLK_IO_SEL_MEDIA_TYPE_NON_REMOVABLE;

  FilterData.PartitionLabel = PartitionLabel;

  HandleCount = sizeof(HandleInfoList)/sizeof(*HandleInfoList);

  Status = GetBlkIOHandles(Attrib,
                           &FilterData,
                           HandleInfoList,
                           &HandleCount);
  if (EFI_ERROR(Status) || (HandleCount != 1))
  {
    FilterData.RootDeviceType = &gEfiEmmcUserPartitionGuid;
    Status = GetBlkIOHandles(Attrib,
                             &FilterData,
                             HandleInfoList,
                             &HandleCount);
    if (EFI_ERROR(Status) || (HandleCount != 1))
      return EFI_NOT_FOUND;
  }

  BlkIo = HandleInfoList[0].BlkIo;

  ReadSize = (BlkIo->Media->LastBlock+1)*(BlkIo->Media->BlockSize);

  *ReadBuffer = AllocatePages(ReadSize/EFI_PAGE_SIZE);
  if (*ReadBuffer == NULL)
    return EFI_OUT_OF_RESOURCES;

  Status = BlkIo->ReadBlocks(BlkIo,
                             BlkIo->Media->MediaId,
                             0,
                             ReadSize,
                             *ReadBuffer);

  *ImageSize = ReadSize;
  return Status;
}

EFI_STATUS ReadFromPartitionUsingGuid (EFI_GUID *PartitionType, VOID **ReadBuffer, UINTN *ImageSize)
{
  EFI_STATUS             Status = EFI_NOT_FOUND;
  HandleInfo             HandleInfoList[2];
  PartiSelectFilter      FilterData;
  UINT32                 HandleCount, Attrib;
  EFI_BLOCK_IO_PROTOCOL  *BlkIo;
  UINTN                  ReadSize = 0;

  Attrib  = BLK_IO_SEL_MEDIA_TYPE_NON_REMOVABLE;

  /* Set Partition Type using GUID from caller */
  if (PartitionType != NULL)
  {
    Attrib                         = BLK_IO_SEL_MATCH_PARTITION_TYPE_GUID;
    FilterData.PartitionType       = PartitionType;
    FilterData.VolumeName = 0;
  }

  HandleCount = (UINT32)sizeof(HandleInfoList)/sizeof(*HandleInfoList);

  Status = GetBlkIOHandles(Attrib,
                           &FilterData,
                           HandleInfoList,
                           &HandleCount);
  if (EFI_ERROR(Status) || (HandleCount != 1))
  {
    DEBUG((EFI_D_ERROR, "GetBlkIOHandles failed trying again\n"));
    FilterData.RootDeviceType = &gEfiEmmcUserPartitionGuid;
    Status = GetBlkIOHandles(Attrib,
                             &FilterData,
                             HandleInfoList,
                             &HandleCount);
    if (EFI_ERROR(Status) || (HandleCount != 1)){
      DEBUG((EFI_D_ERROR, "GetBlkIOHandles failed again\n"));
      return EFI_NOT_FOUND;
    }
  }

  BlkIo = HandleInfoList[0].BlkIo;

  ReadSize = (BlkIo->Media->LastBlock+1)*(BlkIo->Media->BlockSize);

  *ReadBuffer = AllocatePages(ReadSize/EFI_PAGE_SIZE);
  if (*ReadBuffer == NULL)
    return EFI_OUT_OF_RESOURCES;

  Status = BlkIo->ReadBlocks(BlkIo,
                             BlkIo->Media->MediaId,
                             0,
                             ReadSize,
                             *ReadBuffer);

  *ImageSize = ReadSize;
  return Status;
}

EFI_STATUS ReadFromToolsPartition (VOID **ReadBuffer)
{
  EFI_STATUS             Status = EFI_NOT_FOUND;
  CHAR8                  ToolsFvPartition[MAX_PARTI_NAME_LEN];
  UINTN                  Size = MAX_PARTI_NAME_LEN;
  CHAR16                 *PartitionLabel = NULL;
  UINTN                  ImageSize = 0;
  
  Status = GetConfigString ("ToolsFvPartition", ToolsFvPartition, &Size);
  if (Status == EFI_SUCCESS)
  {
    PartitionLabel = AllocatePool(Size);
    AsciiStrToUnicodeStr(ToolsFvPartition, PartitionLabel);
  }
  else
    PartitionLabel = L"toolsfv";
  
  Status = ReadFromPartition (PartitionLabel, ReadBuffer, &ImageSize);
  
  return Status;
}

#define TOOLS_FV_FILENAME "tools.fv"

#define EXTRACT_SECTION_UI_SIZE(SectionUi) \
  ((UINTN) ((SectionUi->CommonHeader.Size[0] & 0xFF) | \
           ((SectionUi->CommonHeader.Size[1] << 8) & 0xFF00)| \
           ((SectionUi->CommonHeader.Size[2] << 16) & 0xFF0000)))

EFI_STATUS MountFvFromBuffer (VOID *Buffer, EFI_GUID *DesiredGuid)
{
  EFI_STATUS                  Status;
  VOID                       *FvBuffer;
  VOID                       *FvBufferTry;
  UINTN                       FvSize, SectionUiSize;
  EFI_USER_INTERFACE_SECTION *SectionUi;

  FvBuffer = NULL;
  FvBufferTry = NULL;
  FvSize = 0;
  SectionUi = NULL;
  SectionUiSize = 0;

  Status = ProcessFvImageFile (Buffer);
  if (EFI_ERROR(Status))
    DEBUG((EFI_D_WARN, "Could not process FV in buffer\n"));

  //check if tools fv is mounted
  if (GuidedFvIsMounted(DesiredGuid))
    return EFI_SUCCESS;

  BdsConnectAllDrivers ();

  // Search for Tools FV in all mounted FVs
  Status = LoadFileFromGuidedFv(NULL,
                                TOOLS_FV_FILENAME,
                                (UINT8 **) &FvBuffer,
                                &FvSize);

  if (EFI_ERROR(Status) || (FvBuffer == NULL))
  {
    DEBUG((EFI_D_WARN, "%a not found in any loaded FVs\n", TOOLS_FV_FILENAME));
    return Status;
  }

  SectionUi = (EFI_USER_INTERFACE_SECTION *) FvBuffer;

  while ((SectionUi->CommonHeader.Type != EFI_SECTION_FIRMWARE_VOLUME_IMAGE) &&
         (SectionUi->CommonHeader.Type != 0))
  {
    if (SectionUi->CommonHeader.Type == EFI_SECTION_USER_INTERFACE)
      SectionUiSize = sizeof(*SectionUi) + StrSize(SectionUi->FileNameString);
    else 
      SectionUiSize = EXTRACT_SECTION_UI_SIZE(SectionUi);

    SectionUi = (EFI_USER_INTERFACE_SECTION *) ((UINTN) SectionUi + SectionUiSize);
  }

  FvBufferTry = (VOID *) ((UINTN) SectionUi + SectionUiSize);

  Status = ProcessFvImageFile(FvBufferTry);
  if (EFI_ERROR(Status))
    DEBUG((EFI_D_WARN, "Could not mount tools FV from FV\n"));

  return Status;
}

/* Load the debug FV from the partition into memory and mount */
EFI_STATUS LoadDebugToolsFv (VOID)
{
  EFI_STATUS Status = EFI_NOT_FOUND;
  VOID *ReadBuffer = NULL;
  MemRegionInfo RamFv; 

  Status = GetMemRegionInfoByName("FV Region", &RamFv);
  if (Status == EFI_SUCCESS)
  {
    Status = MountFvFromBuffer ( (VOID *) RamFv.MemBase, &gToolsFvGuid);
    if (Status == EFI_SUCCESS)
    {
      DEBUG((EFI_D_ERROR, "FV mounted from memory\n"));
      return Status;
    }
  }

  Status = ReadFromToolsPartition (&ReadBuffer);
  if (EFI_ERROR(Status))
  {
    DEBUG((EFI_D_WARN, "Failed to read from specified tools partition"));
    return Status;
  }

  Status = MountFvFromBuffer (ReadBuffer, &gToolsFvGuid);
  if (EFI_ERROR(Status))
  {
    DEBUG((EFI_D_WARN, "Failed to mount FV from partition\n"));
    return Status;
  }

  BdsConnectAllDrivers ();
  DEBUG((EFI_D_ERROR, "Successfully loaded Tools FV\n"));
  return EFI_SUCCESS;
}

