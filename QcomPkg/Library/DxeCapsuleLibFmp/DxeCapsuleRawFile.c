/** @file
  Qualcomm functions for operations on capsule partition from DXE capsule library.

  Caution: This module requires additional review when modified.
  This module will have external input - capsule image.
  This external input must be validated carefully to avoid security issue like
  buffer overflow, integer overflow.

  Copyright (c) 2017-2018, Qualcomm Technologies, Inc. All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential.
**/
/*=============================================================================
                              EDIT HISTORY


 when       who     what, where, why
 --------   ---     -----------------------------------------------------------
 20/02/18   gparam  Fixed the ASSERT due to incorrect print of GUID
 02/01/18   prema   Handled capsule block size based on dynamic media block size
                    corresponding to given GUID instead of static block size
 03/15/17   gparam  Remove unused header file inclusions + Changes to block-align
                    capsules while persisting in capsule raw file + 
                    Restructuring of code between files
 02/22/17   gparam  Initial revision

=============================================================================*/
#include <Protocol/Capsule.h>
#include <Protocol/EFICapsule.h>
#include <Protocol/SimpleFileSystem.h>

#include <Guid/CapsuleVendor.h>
#include <Guid/FmpCapsule.h>
#include <Guid/FileInfo.h>
#include <Guid/GlobalVariable.h>
#include <Guid/EventGroup.h>

#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/CapsuleLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiRuntimeLib.h>
#include <Library/BaseLib.h>
#include <Library/PrintLib.h>
#include <Library/QcomLib.h>
#include <Library/QcomBaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/SortLib.h>
#include <Library/HobLib.h>
#include <Library/GenericBdsLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DevicePathLib.h>

#include "DxeCapsuleQcom.h"
#include "DxeCapsuleRawFile.h"

#define CAPSULE_RAW_FILE_GUID_TABLE_REVISION    0x00010001
#define CAPSULE_RAW_FILE_NAME                   L"Persisted_Capsules.bin"
#define CAPSULE_RAW_FILE_SIZE                   (UINT32)(50 * 1024 * 1024) // 50MB = 0x03200000 Bytes
#define CAPSULE_STORAGE_BLOCK_SIZE              4096

extern VOID                 **mCapsulePtr;
extern UINT32                *mCapsuleOffsetArray;
extern VOID                  *CapsuleStorageHeader;
extern UINT32                 mCapsuleTotalNumber;
extern EFI_STATUS            *mCapsuleStatusArray;
extern VOID                 **mCapsuleFileNameArray;

static UINT32                 RawFilePartitionBlockSize = CAPSULE_STORAGE_BLOCK_SIZE;
/**
  Validates the capsule GUID table in capsule raw file.
  
  @param[in]  CapsuleGuidTable Capsule GUID table buffer pointer to be
                               initialized and updated to the storage media.

  @retval EFI_SUCESS            Action completed successfully
  @retval EFI_INVALID_PARAMETER Invalid input parameter.

**/
EFI_STATUS
EFIAPI
ValidateCapsuleRawFileGuidTable (
  CAPSULE_RAW_FILE_GUID_TABLE *CapsuleGuidTable
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  UINT32 Crc32 = 0;
  do {
    if (CapsuleGuidTable == NULL) {
      Status = EFI_INVALID_PARAMETER;
      break;
    }

    Crc32 = CapsuleGuidTable->Crc32;
    CapsuleGuidTable->Crc32  = 0;
    Status = CalculateCrc32(CapsuleGuidTable,
                            sizeof(CAPSULE_RAW_FILE_GUID_TABLE),
                            &(CapsuleGuidTable->Crc32));
    if (EFI_ERROR (Status)) {
      break;
    }

    if ((Crc32 != CapsuleGuidTable->Crc32) ||
        (CapsuleGuidTable->Signature != CAPSULE_GUID_TABLE_SIGNATURE)) {
      // restore old CRC value
      CapsuleGuidTable->Crc32 = Crc32;
      Status = EFI_NOT_FOUND;
      break;
    }
  } while (0);

  return Status;
}

/**
  Initialize the capsule GUID table in capsule raw file. This accepts the pointer to the memory
  that can be used as the buffer to initialize the default data and updates the same to the
  capsule raw file
  
  @param[in]  CapsuleGuidTable Capsule GUID table buffer pointer to be
                               initialized and updated to the storage media.

  @retval EFI_SUCESS            Action completed successfully
  @retval EFI_INVALID_PARAMETER Invalid input parameter.

**/
EFI_STATUS
EFIAPI
InitCapsuleRawFileGuidTable (
  CAPSULE_RAW_FILE_GUID_TABLE *CapsuleGuidTable
  )
{
  EFI_STATUS         Status = EFI_SUCCESS;
  UINTN              HeaderBytes = sizeof (*CapsuleGuidTable);
  UINTN              HeaderBytesBlockAligned = EFI_BLOCKS_TO_SIZE (
                                                 EFI_SIZE_TO_BLOCKS (
                                                   sizeof (*CapsuleGuidTable),
                                                   RawFilePartitionBlockSize
                                                 ),
                                                 RawFilePartitionBlockSize
                                               );

  if (NULL == CapsuleGuidTable) {
    return EFI_INVALID_PARAMETER;
  }

  if (!UpdateCapsuleRawFileExists()) {
    Status = EFI_NOT_FOUND;
    DEBUG ((EFI_D_ERROR, "Cannot initialize GUID table as capsule raw file does not exist\n"));
    return Status;
  }

  // This effectively deletes all capsules in the partition
  CapsuleGuidTable->Signature = CAPSULE_GUID_TABLE_SIGNATURE;
  CapsuleGuidTable->Revision = CAPSULE_RAW_FILE_GUID_TABLE_REVISION;
  CapsuleGuidTable->GuidCount = 0;
  CapsuleGuidTable->BlockSize = RawFilePartitionBlockSize;
  // Use the next block boundary for the next capsule as read from a non-block aligned addresses are failing
  CapsuleGuidTable->FreeBytes = CAPSULE_RAW_FILE_SIZE - HeaderBytesBlockAligned;

  // Clear out all Guid entries
  SetMem(&(CapsuleGuidTable->CapsuleGuidEntries[0]),
         sizeof(CapsuleGuidTable->CapsuleGuidEntries),
         0x0);

  CapsuleGuidTable->Crc32 = 0;
  Status = CalculateCrc32(CapsuleGuidTable,
                          sizeof(CAPSULE_RAW_FILE_GUID_TABLE),
                          &(CapsuleGuidTable->Crc32));
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to calculate CRC value after clearing capsule GUID table. Status: %r\n", Status));
    return Status;
  }

  // Copy back Capsule Guid Table to persisted storage
  Status = WriteFile (
    CAPSULE_RAW_FILE_NAME,
    NULL,
    &gEfiPartTypeSystemPartGuid,
    NULL,
    TRUE,
    NULL,
    &HeaderBytes,
    0,
    (UINT8*)CapsuleGuidTable,
    CAPSULE_RAW_FILE_SIZE // sizeof (*CapsuleGuidTable) TODO: Giri: Check with UEFI team on why this parameter is used
    );
    
  if (EFI_ERROR (Status) || (sizeof (*CapsuleGuidTable) != HeaderBytes)) {
    DEBUG ((EFI_D_ERROR, "Failed to initialize capsule raw file storage header. Status: %r\n", Status));
    return (EFI_ERROR (Status) ? Status : EFI_DEVICE_ERROR);
  }

  return Status;
}

/**
  Checks whether the Update Capsule partition exists.

  @retval TRUE                  Update Capsule partition does exist.
  @retval FALSE                 Update Capsule partition does not exist.

**/
BOOLEAN
EFIAPI
UpdateCapsuleRawFileExists (
  VOID
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  UINT32     FileSize = 0;

  Status = GetFileSize(
    CAPSULE_RAW_FILE_NAME,
    NULL,
    &gEfiPartTypeSystemPartGuid,
    NULL,
    TRUE,
    NULL,
    &FileSize
    );

  if (EFI_ERROR (Status)) {
    return FALSE;
  } else {
    if (CAPSULE_RAW_FILE_SIZE != FileSize) {
      // We do not consider this as fatal as this functions job is to just 
      // check if the file exists. The below warning log will just serve as a
      // hint to the developer that there is something wrong with the file size
      DEBUG ((EFI_D_WARN, "Capsule raw file exits but size 0x%x does not match with expected size 0x%x\n", FileSize, CAPSULE_RAW_FILE_SIZE));
    }
    return TRUE;
  }
}

/**
  Delete the capsule matching the input parameter from persisted capsules in capsule raw file.
  
  @param[in]  CapsuleGuid      File name of the mass-storage capsule.
  @param[in]  CapsuleOffset    Offset of the capsule in storage, if 0 capsule did
                               not come from storage

  @retval EFI_SUCESS            Action completed successfully
  @retval EFI_INVALID_PARAMETER Invalid input parameter.

**/
EFI_STATUS
EFIAPI
DeletePersistedCapsuleFromCapsuleRawFile(
  IN EFI_GUID           *CapsuleGuid,
  IN UINT32              CapsuleOffset
  )
{
  EFI_STATUS                    Status = EFI_SUCCESS;
  UINT8                        *MemLoc = NULL;
  UINTN                         ImgSize = 0;
  CAPSULE_RAW_FILE_GUID_TABLE  *CapsuleGuidTable = NULL;
  UINT32                        Index = 0;
  UINTN                         BytesToRead = 0;
  UINTN                         BytesToWrite = 0;

  do {
    if (NULL == CapsuleGuid) {
      DEBUG ((DEBUG_ERROR, "Invalid value received for parameter CapsuleGuid\n"));
      Status = EFI_INVALID_PARAMETER;
      break;
    }

    if (!UpdateCapsuleRawFileExists())
    {
      Status = EFI_UNSUPPORTED;
      break;
    }
    /* Get the Capsule Guid Table */
    ImgSize = sizeof (*CapsuleGuidTable);
    Status = gBS->AllocatePool(EfiBootServicesData,
                               ImgSize,
                               (VOID**)&MemLoc);

    if (EFI_ERROR(Status)) {
      DEBUG ((EFI_D_ERROR, "Allocate-memory for capsule storage header returned error - %r\n", Status));
      break;
    }

    BytesToRead = ImgSize;
    BytesToWrite = ImgSize;

    // Load the Capsule Guid Table into memory
    Status = ReadFile (
      CAPSULE_RAW_FILE_NAME,
      NULL,
      &gEfiPartTypeSystemPartGuid,
      NULL,
      TRUE,
      NULL,
      &BytesToRead,
      0,
      MemLoc,
      CAPSULE_RAW_FILE_SIZE // ImgSize // TODO: Giri: Check with UEFI team on why this parameter is used
      );
      
    if (EFI_ERROR (Status) || (ImgSize != BytesToRead)) {
      DEBUG ((EFI_D_ERROR, "Capsule storage header read during capsule delete failed. Status: %r\n", Status));
      Status = (EFI_ERROR (Status)) ? Status : EFI_DEVICE_ERROR;
      break;
    }

    CapsuleGuidTable = (CAPSULE_RAW_FILE_GUID_TABLE*)MemLoc;
    Status = ValidateCapsuleRawFileGuidTable(CapsuleGuidTable);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "Invalid capsule storage header found during capsule delete. Status: %r\n", Status));
      break;
    }

    // Find Guid entry corresponding to capsule
    for (Index = 0; Index < CapsuleGuidTable->GuidCount; Index++)
    {
      if ((CompareGuid(&(CapsuleGuidTable->CapsuleGuidEntries[Index].Guid), 
                      CapsuleGuid) == TRUE) &&
          (CapsuleOffset == CapsuleGuidTable->CapsuleGuidEntries[Index].FileOffset))
      {
        // Check if last entry in list;
        // Don't update CapsuleGuidTable->FreeBlocks unless all entries are 
        // deleted. Once all capsules are deleted it will clear out the entire 
        // table and thus free up the partition. If a new capsule is inserted,
        // it will be allocated from the remaining FreeBlocks and not reallocate
        // from the blocks taken from the deleted capsule.
        if (Index == CapsuleGuidTable->GuidCount - 1)
        {
          // If last entry, zero out last entry
          SetMem(&(CapsuleGuidTable->CapsuleGuidEntries[Index].Guid), 
                 sizeof(CAPSULE_GUID_ENTRY), 
                 0x0);
          CapsuleGuidTable->GuidCount--;
        }
        else
        {
          // If not last entry, shift all entries up one
          CopyMem(&(CapsuleGuidTable->CapsuleGuidEntries[Index]),
                  &(CapsuleGuidTable->CapsuleGuidEntries[Index + 1]),
                  (CapsuleGuidTable->GuidCount - Index - 1)*sizeof(CAPSULE_GUID_ENTRY));
    
          // Zero out last entry
          SetMem(&(CapsuleGuidTable->CapsuleGuidEntries[CapsuleGuidTable->GuidCount - 1]), 
                 sizeof(CAPSULE_GUID_ENTRY), 
                 0x0);
          CapsuleGuidTable->GuidCount--;
        }
      }
    }

    if (CapsuleGuidTable->GuidCount == 0)
    {
      // Reinitialize Capsule Guid Table 
      // This effectively deletes all capsules in the partition
      CapsuleGuidTable->Signature = CAPSULE_GUID_TABLE_SIGNATURE;
      CapsuleGuidTable->Revision = CAPSULE_RAW_FILE_GUID_TABLE_REVISION;
      CapsuleGuidTable->GuidCount = 0; 
      CapsuleGuidTable->FreeBytes = CAPSULE_RAW_FILE_SIZE - sizeof (*CapsuleGuidTable);

      // Clear out all Guid entries
      SetMem(&(CapsuleGuidTable->CapsuleGuidEntries[0]), 
             sizeof(CapsuleGuidTable->CapsuleGuidEntries),
             0x0);
    }

    CapsuleGuidTable->Crc32 = 0;
    Status = CalculateCrc32(CapsuleGuidTable, 
                            sizeof(CAPSULE_RAW_FILE_GUID_TABLE), 
                            &(CapsuleGuidTable->Crc32));
    if (EFI_ERROR (Status))
      break;

    // Copy back Capsule Guid Table to persisted storage
    Status = WriteFile (
      CAPSULE_RAW_FILE_NAME,
      NULL,
      &gEfiPartTypeSystemPartGuid,
      NULL,
      TRUE,
      NULL,
      &BytesToWrite,
      0,
      MemLoc,
      CAPSULE_RAW_FILE_SIZE // ImgSize // TODO: Giri: Check with UEFI team on why this parameter is used
      );
      
    if (EFI_ERROR (Status) || (ImgSize != BytesToWrite)) {
      DEBUG ((EFI_D_ERROR, "Capsule storage header update during capsule delete failed. Status: %r\n", Status));
      Status = (EFI_ERROR (Status)) ? Status : EFI_DEVICE_ERROR;
      break;
    }
  }while(0);

  if (NULL != MemLoc)
  {
    gBS->FreePool(MemLoc);
  }

  return Status;
}

/**
  This code loads capsules in boot time from the NV storage and processes them 
  in the same way as capsules in RAM.

  @param [in/out] CapsulePtrArray     Array of capsules with array length CapsuleArrayLen.
  @param [in/out] CapsuleOffsetArray  Array of capsules offset within the capsule storage.
  @param [in] CapsuleArrayLen     Length of the arrays CapsulePtrArray and CapsuleOffsetArray.

  @retval EFI_SUCCESS             UEFI Capsule Runtime Services are installed successfully. 
  @retval EFI_INVALID_PARAMETER   Invalid paramter received
  @retval EFI_VOLUME_CORRUPTED    Capsule partition corrupted

**/
EFI_STATUS
EFIAPI
LoadPersistedCapsulesFromCapsuleRawFile (
  IN OUT VOID **CapsulePtrArray,
  IN OUT UINT32 *CapsuleOffsetArray,
  IN UINT32 CapsulePtrArrayLen
  )
{
  EFI_STATUS                    Status = EFI_SUCCESS;
  EFI_GUID                      InvalidGuid = CAPSULE_INVALID_GUID;
  CAPSULE_RAW_FILE_GUID_TABLE  *CapsuleGuidTable = (CAPSULE_RAW_FILE_GUID_TABLE *) CapsuleStorageHeader;
  CAPSULE_GUID_ENTRY           *CapsuleGuidEntry = NULL;
  UINT32                        Index;

  UINT8                        *MemLoc = NULL;
  UINTN                         ImgSize = 0;
  UINTN                         BytesToRead = 0;

  do {
    if (NULL == CapsulePtrArray) {
        Status = EFI_INVALID_PARAMETER;
        DEBUG ((DEBUG_ERROR, "Invalid value received for parameter CapsulePtrArray\n"));
        break;
    }

    if (NULL == CapsuleOffsetArray) {
        Status = EFI_INVALID_PARAMETER;
        DEBUG ((DEBUG_ERROR, "Invalid value received for parameter CapsuleOffsetArray\n"));
        break;
    }

    if (CapsuleStorageHeader == NULL) {
      Status = EFI_NOT_FOUND;
      DEBUG ((DEBUG_ERROR, "Capsule raw file read pointers not initialized. Error in loading capsules from capsule raw file\n"));
      break;
    }

    // Calculate CRC on table, if doesn't match or is uninitialized, reinitialize
    Status = ValidateCapsuleRawFileGuidTable(CapsuleGuidTable);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "ValidateCapsuleRawFileGuidTable returned error - %r\n", Status));
      break;
    }

    if (CapsulePtrArrayLen != CapsuleGuidTable->GuidCount) {
      Status = EFI_INVALID_PARAMETER;
      DEBUG ((DEBUG_ERROR, "ValidateCapsuleRawFileGuidTable returned error - %r\n", Status));
      break;
    }
      
    // Load each capsule and store it to capsule pointer
    for (Index = 0; Index < CapsuleGuidTable->GuidCount; Index++) {
      CapsuleGuidEntry = &(CapsuleGuidTable->CapsuleGuidEntries[Index]);

      // Confirm GUID entry is valid
      if (CompareGuid(&(CapsuleGuidEntry->Guid), &InvalidGuid)) {
        // This should never be the case
        Status = EFI_VOLUME_CORRUPTED;
        DEBUG ((DEBUG_ERROR, "Capsule partition corrupted. Invalid capsule table GUID\n"));
        break;
      }

      // Get the Capsule size. Account for one extra block in case not a
      // multiple of block size
      ImgSize = CapsuleGuidEntry->ImageSize;

      if ((CapsuleGuidEntry->Flags & CAPSULE_FLAGS_POPULATE_SYSTEM_TABLE) != 0) {
        Status = gBS->AllocatePool(EfiRuntimeServicesData,
                                   ImgSize,
                                   (VOID**)&MemLoc);
      } else {
        Status = gBS->AllocatePool(EfiBootServicesData,
                                   ImgSize,
                                   (VOID**)&MemLoc);
      }
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "Failed to allocate memory to load persisted capsule\n"));
        break;
      }

      BytesToRead = ImgSize;

      // Copy back Capsule Guid Table to persisted storage
      Status = ReadFile (
        CAPSULE_RAW_FILE_NAME,
        NULL,
        &gEfiPartTypeSystemPartGuid,
        NULL,
        TRUE,
        NULL,
        &BytesToRead,
        CapsuleGuidEntry->FileOffset,
        MemLoc,
        CAPSULE_RAW_FILE_SIZE // ImgSize // TODO: Giri: Check with UEFI team on why this parameter is used
        );
        
      if (EFI_ERROR (Status) || (ImgSize != BytesToRead)) {
        DEBUG ((EFI_D_ERROR, "Capsule raw file read for index %d returned error - %r\n", Index, Status));
        Status = (EFI_ERROR (Status)) ? Status : EFI_DEVICE_ERROR;
        break;
      }

      // Copy to pointer to Capsule pointer array
      CapsulePtrArray[Index] = MemLoc;
      CapsuleOffsetArray[Index] = CapsuleGuidEntry->FileOffset;
    }
  } while(0);

  return Status;
}


/**

  Persists capsules received in the form of capsule array to capsule raw file.

  @param [in] CapsuleHeaderArray Virtual pointer to an array of virtual pointers
                                 to the capsules being passed into update capsule.
  @param [in] CapsuleCount       Number of pointers to EFI_CAPSULE_HEADER in
                                 CaspuleHeaderArray.

  @retval EFI_SUCCESS           Successfully persisted given capsules.
  @retval EFI_DEVICE_ERROR      The capsule update was started, but failed
                                due to a device error.
  @retval EFI_OUT_OF_RESOURCES  When ExitBootServices() has been previously
                                called this error indicates the capsule 
                                is compatible with this platform but is not
                                capable of being submitted or processed 
                                in runtime. The caller may resubmit the capsule
                                prior to ExitBootServices().
  @retval EFI_OUT_OF_RESOURCES  When ExitBootServices() has not been previously
                                called then this error indicates the capsule is
                                compatible with this platform but there are
                                insufficient resources to process.

**/
EFI_STATUS
EFIAPI
PersistCapsulesToCapsuleRawFile (
  IN EFI_CAPSULE_HEADER      **CapsuleHeaderArray,
  IN UINTN                   CapsuleCount
  )
{
  UINTN                         ArrayNumber = 0;
  EFI_STATUS                    Status = EFI_SUCCESS;
  EFI_CAPSULE_HEADER           *CapsuleHeader = NULL;
  UINTN                         BlockAlignedSize = 0;
  CAPSULE_RAW_FILE_GUID_TABLE  *CapsuleGuidTable = (CAPSULE_RAW_FILE_GUID_TABLE *) CapsuleStorageHeader;
  UINTN                         BytesToWrite = 0;
  EFI_GUID                      UniqueCapsuleGuid;

  //
  // Walk through all capsules, store capsules to persist
  //
  for (ArrayNumber = 0; ArrayNumber < CapsuleCount ; ArrayNumber++) {
    CapsuleHeader = CapsuleHeaderArray[ArrayNumber];
    if ((CapsuleHeader->Flags & CAPSULE_FLAGS_PERSIST_ACROSS_RESET) != 0) {
      if ((CapsuleGuidTable->GuidCount >= CAPSULE_GUID_ENTRIES_MAX) ||
          (CapsuleGuidTable->FreeBytes < CapsuleHeader->CapsuleImageSize)) {
        DEBUG ((EFI_D_ERROR, "Failed to persist capsule to EFI raw file. Not enough resources.\n"));
        return EFI_OUT_OF_RESOURCES;
      }

      Status = GetUniqueCapsuleGuid (CapsuleHeader, &UniqueCapsuleGuid);
      if (EFI_ERROR (Status)) {
        DEBUG ((EFI_D_ERROR, "Failed to get unique GUID of capsule to store in capsule RAW file. Returning alternative error %r\n", Status));
        return Status;
      }

      CopyMem (&CapsuleGuidTable->CapsuleGuidEntries[CapsuleGuidTable->GuidCount].Guid, &UniqueCapsuleGuid, sizeof(EFI_GUID));
      CapsuleGuidTable->CapsuleGuidEntries[CapsuleGuidTable->GuidCount].FileOffset = CAPSULE_RAW_FILE_SIZE - CapsuleGuidTable->FreeBytes;
      CapsuleGuidTable->CapsuleGuidEntries[CapsuleGuidTable->GuidCount].ImageSize = CapsuleHeader->CapsuleImageSize;
      BlockAlignedSize = EFI_BLOCKS_TO_SIZE (EFI_SIZE_TO_BLOCKS (CapsuleHeader->CapsuleImageSize, CapsuleGuidTable->BlockSize), CapsuleGuidTable->BlockSize);
      // Use the next block boundary for the next capsule as read from a non-block aligned addresses are failing
      CapsuleGuidTable->FreeBytes -= BlockAlignedSize;
      CapsuleGuidTable->CapsuleGuidEntries[CapsuleGuidTable->GuidCount].Flags = CapsuleHeader->Flags;

      BytesToWrite = (UINTN)CapsuleHeader->CapsuleImageSize;

      Status = WriteFile (
        CAPSULE_RAW_FILE_NAME,
        NULL,
        &gEfiPartTypeSystemPartGuid,
        NULL,
        TRUE,
        NULL,
        &BytesToWrite,
        CapsuleGuidTable->CapsuleGuidEntries[CapsuleGuidTable->GuidCount].FileOffset,
        (UINT8 *)CapsuleHeader,
        CAPSULE_RAW_FILE_SIZE // CapsuleHeader->CapsuleImageSize // TODO: Giri: Check with UEFI team on why this parameter is used
        );
        
      CapsuleGuidTable->GuidCount++;
      if (EFI_ERROR (Status) || ((UINTN)CapsuleHeader->CapsuleImageSize != BytesToWrite)) {
        DEBUG ((EFI_D_ERROR, "Failed to persist capsule to EFI raw file. Status: %r. Returning alternative error %r\n", Status, EFI_DEVICE_ERROR));
        return EFI_DEVICE_ERROR;
      }
    }
  }

  //Update CRC and store header
  CapsuleGuidTable->Crc32 = 0;
  Status = CalculateCrc32(CapsuleGuidTable,
                          sizeof(CAPSULE_RAW_FILE_GUID_TABLE),
                          &(CapsuleGuidTable->Crc32));
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to calculate CRC after persisting capsules. Status: %r. Returning alternative error %r\n", Status, EFI_DEVICE_ERROR));
    return EFI_DEVICE_ERROR;
  }

  BytesToWrite = sizeof (CAPSULE_RAW_FILE_GUID_TABLE);

  Status = WriteFile (
    CAPSULE_RAW_FILE_NAME,
    NULL,
    &gEfiPartTypeSystemPartGuid,
    NULL,
    TRUE,
    NULL,
    &BytesToWrite,
    0,
    (UINT8 *)CapsuleGuidTable,
    CAPSULE_RAW_FILE_SIZE // sizeof (*CapsuleGuidTable) // TODO: Giri: Check with UEFI team on why this parameter is used
    );
    
  if (EFI_ERROR (Status) || (sizeof (*CapsuleGuidTable) != BytesToWrite)) {
    DEBUG ((EFI_D_ERROR, "Failed to add storage header to EFI raw file. Status: %r. Returning alternative error %r\n", Status, EFI_DEVICE_ERROR));
    return EFI_DEVICE_ERROR;
  }

  return Status;
}

/**
  Get the maximum capsule size supported in the update capsule partition.
  
  @param  MaxiumCapsuleSize     Maximum capsule size supported in the update
                                capsule partition.
   
  @retval EFI_SUCESS            Action completed successfully
  @retval EFI_INVALID_PARAMETER Invalid input parameter.

**/
EFI_STATUS
EFIAPI
GetMaxCapsuleSizeInCapsuleRawFile (
  OUT UINT64             *MaxiumCapsuleSize
  )
{
  EFI_STATUS Status = EFI_SUCCESS;

  if (NULL != MaxiumCapsuleSize) {
    CAPSULE_RAW_FILE_GUID_TABLE *CapsuleGuidTable = (CAPSULE_RAW_FILE_GUID_TABLE *)CapsuleStorageHeader;

    *MaxiumCapsuleSize = CapsuleGuidTable->FreeBytes;
  } else {
    Status = EFI_INVALID_PARAMETER;
  }
  
  return Status;
}

/**
  Creates Capsule raw file if not existing.

**/
EFI_STATUS
EFIAPI
CreateCapsuleRawFile (
  VOID
  )
{
  EFI_STATUS                   Status = EFI_SUCCESS;
  UINTN                        BytesToWrite = CAPSULE_RAW_FILE_SIZE;
  VOID                        *FileBuffer = NULL;
  UINT32                       FileSize = 0;
  CAPSULE_RAW_FILE_GUID_TABLE *CapsuleGuidTable = NULL;

  Status = GetFileSize(
    CAPSULE_RAW_FILE_NAME,
    NULL,
    &gEfiPartTypeSystemPartGuid,
    NULL,
    TRUE,
    NULL,
    &FileSize
    );

  if ((EFI_NOT_FOUND == Status) ||
      ((EFI_SUCCESS == Status) &&
       (CAPSULE_RAW_FILE_SIZE != FileSize))) {

    if (EFI_NOT_FOUND != Status) {
      DEBUG ((EFI_D_WARN, "Capsule raw file exits but size 0x%x does not match with expected 0x%x\n", FileSize, CAPSULE_RAW_FILE_SIZE));
      DEBUG ((EFI_D_WARN, "Trying to reset the file size to the expected size\n"));
    }

    // TODO: Giri: Add code to make sure that we have minimum size for EFI partition
    // (like 260MB) so that we dont eat-up partition space if its is of very
    // small size and cause system instability. For capsule update to work,
    // this minimum EFI size should be met

    // Capsule raw file not present. Reserve space for the same by creating
    // a new file of size CAPSULE_RAW_FILE_SIZE
    //
    // **NOTE: TPM needs minimum 350 MB in EFI system partition**

    Status = gBS->AllocatePool (EfiBootServicesData, CAPSULE_RAW_FILE_SIZE, &FileBuffer);
    if (EFI_ERROR (Status) || (FileBuffer == NULL)) {
      DEBUG ((EFI_D_ERROR, "Failed to allocate empty buffer for capsule raw file. Status: %r\n", Status));
      return (EFI_ERROR (Status) ? Status : EFI_OUT_OF_RESOURCES);
    }

    ZeroMem (FileBuffer, CAPSULE_RAW_FILE_SIZE);

    CapsuleGuidTable = (CAPSULE_RAW_FILE_GUID_TABLE *)FileBuffer;
    
    // This effectively deletes all capsules in the partition
    CapsuleGuidTable->Signature = CAPSULE_GUID_TABLE_SIGNATURE;
    CapsuleGuidTable->Revision = CAPSULE_RAW_FILE_GUID_TABLE_REVISION;
    CapsuleGuidTable->GuidCount = 0;
    CapsuleGuidTable->FreeBytes = CAPSULE_RAW_FILE_SIZE - sizeof (CAPSULE_RAW_FILE_GUID_TABLE);
    
    // Clear out all Guid entries
    SetMem(&(CapsuleGuidTable->CapsuleGuidEntries[0]),
           sizeof(CapsuleGuidTable->CapsuleGuidEntries),
           0x0);
    
    CapsuleGuidTable->Crc32 = 0;
    Status = CalculateCrc32(CapsuleGuidTable,
                            sizeof(CAPSULE_RAW_FILE_GUID_TABLE),
                            &(CapsuleGuidTable->Crc32));
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "Failed to calculate CRC value after clearing capsule GUID table. Status: %r\n", Status));
      return Status;
    }    

    BytesToWrite = CAPSULE_RAW_FILE_SIZE;

    Status = WriteFile (
      CAPSULE_RAW_FILE_NAME,
      NULL,
      &gEfiPartTypeSystemPartGuid,
      NULL,
      TRUE,
      NULL,
      &BytesToWrite,
      0,
      FileBuffer,
      CAPSULE_RAW_FILE_SIZE
      );
      
    if (EFI_ERROR (Status) || (CAPSULE_RAW_FILE_SIZE != BytesToWrite)) {
      gBS->FreePool (FileBuffer);
      FileBuffer = NULL;    

      DEBUG ((EFI_D_ERROR, "Failed to create '%s' for persisting capsules. Status: %r\n", CAPSULE_RAW_FILE_NAME, Status));
      return (EFI_ERROR (Status) ? Status :  EFI_OUT_OF_RESOURCES);
    }
    else {
      DEBUG ((EFI_D_WARN, "Successfully created file capsule raw file '%s'.\n", CAPSULE_RAW_FILE_NAME));
      DEBUG ((EFI_D_WARN, "Size of file capsule raw file: 0x%x bytes.\n", BytesToWrite));

      gBS->FreePool (FileBuffer);
      FileBuffer = NULL;    
    }
  } else if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to get size of capsule raw file. Status: %r\n", Status));
    return Status;
  } else {
    DEBUG ((EFI_D_WARN, "Warning: Capsule raw file already exist. Ignoring the create request.\n"));
  }

  return Status;
}

/**
  Get the space needed to store the capsule in the current raw file
  
  @param  CapsuleImageSize      Size of the capsule eceived in the capsule
                                header array.
   
  @retval Size needed to store the capsule in the current storage media

**/
UINTN
EFIAPI
GetCapsuleSizeInCapsuleRawFile (
  IN UINT32             CapsuleImageSize
  )
{
  // We have to consume the whole block at the end of the capsule as the next
  // capsule can be saved only at block-aligned addresses. Otherwise, read
  // from a non-block aligned addresses are fails
  return EFI_BLOCKS_TO_SIZE (
           EFI_SIZE_TO_BLOCKS (
             CapsuleImageSize,
             RawFilePartitionBlockSize
           ),
           RawFilePartitionBlockSize
         );
}

/**
  This function returns the blocksize of the partition corresponding to the 
  given  GUID. 

  Limitations:
  1. This API returns error if there are more than one partition
     with the given GUID.
  2. Searches only for non-removable partitions
  3. Supports only GPT partitions

  @retval EFI_SUCESS            Action completed successfully
  @retval EFI_INVALID_PARAMETER Invalid parameter.
  @retval others                Other EFI error codes.

**/
EFI_STATUS
EFIAPI
GetEfiPartitionBlockSize (
  IN EFI_GUID          PartitionGUID,
  OUT UINT32          *PartitionBlockSize
  )
{
  UINT32                  Attribs = 0;
  EFI_STATUS              Status = EFI_SUCCESS;
  HandleInfo              HandleInfoList[10];
  UINT32                  MaxHandles = 0;
  PartiSelectFilter       HandleFilter;
  if (NULL == PartitionBlockSize) {
    return EFI_INVALID_PARAMETER;
  }
  Attribs |= BLK_IO_SEL_MEDIA_TYPE_NON_REMOVABLE;
  Attribs |= BLK_IO_SEL_PARTITIONED_GPT;
  Attribs |= BLK_IO_SEL_MATCH_PARTITION_TYPE_GUID;
  MaxHandles = sizeof(HandleInfoList)/sizeof(*HandleInfoList);
  HandleFilter.PartitionType = &PartitionGUID;
  HandleFilter.VolumeName = 0;
  Status = GetBlkIOHandles (Attribs, &HandleFilter, HandleInfoList, &MaxHandles);
  if (Status != EFI_SUCCESS) {
    DEBUG ((EFI_D_ERROR, "Locating block IO handles of partition /'%g/' failed. Status: %r\n", &PartitionGUID, Status));
    goto GetEfiPartitionBlockSizeExit;
  } else if (MaxHandles == 0) {
    DEBUG ((EFI_D_ERROR, "Block IO handles of partition /'%g/' not found\n", &PartitionGUID));
    Status = EFI_NOT_FOUND;
    goto GetEfiPartitionBlockSizeExit;
  }
  if (MaxHandles > 1)
  {
    DEBUG ((EFI_D_ERROR, "More than 1 partition found for the given GUID\n"));
    Status = EFI_DEVICE_ERROR;
    goto GetEfiPartitionBlockSizeExit;
  }
  *PartitionBlockSize = HandleInfoList[0].BlkIo->Media->BlockSize;
  GetEfiPartitionBlockSizeExit:
  return Status;
}


/**
  This function also loads and validates the header of the capsule raw file

  @retval TRUE                  Update Capsule partition does exist.
  @retval FALSE                 Update Capsule partition does not exist.

**/
EFI_STATUS
EFIAPI
InitStoragePointersForCapsuleRawFile (
  VOID
  )
{
  EFI_STATUS  Status = EFI_SUCCESS;
  UINTN       BytesToRead = CAPSULE_RAW_FILE_SIZE;
  UINTN       HeaderSize = 0;
  Status = GetEfiPartitionBlockSize (gEfiPartTypeSystemPartGuid,
                                     &RawFilePartitionBlockSize);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to get block size of EFI partition. Status: %r\n", Status));
    return Status;
  }
  DEBUG ((EFI_D_WARN, "EFI partition block size: %d\n", RawFilePartitionBlockSize));

  if (!UpdateCapsuleRawFileExists()) {
    DEBUG ((EFI_D_WARN, "Capsule raw file not found in EFI partition. Trying to create one to reserve space for future updates\n"));

    Status = CreateCapsuleRawFile ();

    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "Failed to create capsule raw file. Status: %r\n", Status));
      return Status;
    }
  }

  HeaderSize = sizeof (CAPSULE_RAW_FILE_GUID_TABLE);

  Status = gBS->AllocatePool (EfiRuntimeServicesData, HeaderSize, &CapsuleStorageHeader);
  if (EFI_ERROR (Status) || (CapsuleStorageHeader == NULL)) {
    DEBUG ((EFI_D_ERROR, "Failed to allocate capsule raw file storage header. Status: %r\n", Status));
    return (EFI_ERROR (Status) ? Status : EFI_OUT_OF_RESOURCES);
  }

  BytesToRead = HeaderSize;
  
  Status = ReadFile(
    CAPSULE_RAW_FILE_NAME,
    NULL,
    &gEfiPartTypeSystemPartGuid,
    NULL,
    TRUE,
    NULL,
    &BytesToRead,
    0,
    CapsuleStorageHeader,
    CAPSULE_RAW_FILE_SIZE // HeaderSize // TODO: Giri: Check with UEFI team on why this parameter is used
    );

  if (EFI_ERROR (Status) || (BytesToRead != HeaderSize)) {
    gBS->FreePool (CapsuleStorageHeader);
    DEBUG ((EFI_D_ERROR, "Failed to read header. Status: %r\n", Status));
    return (EFI_ERROR (Status) ? Status : EFI_VOLUME_CORRUPTED);
  }

  Status = ValidateCapsuleRawFileGuidTable ((CAPSULE_RAW_FILE_GUID_TABLE *)CapsuleStorageHeader);
  if (EFI_ERROR (Status)) {
    Status = InitCapsuleRawFileGuidTable ((CAPSULE_RAW_FILE_GUID_TABLE *)CapsuleStorageHeader);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "Failed to initilize capsule raw file storage header. Status: %r\n", Status));
      return Status;
    }
  }

  return Status;
}

/**
  De-initialize the capsule pointer memory for the capsule loaded from capsule raw file
  This function clears the pointer after de-initialization

  @param[in]  CapsulePtr       Points to the pointer to the capsule to be deinitialized. 

  @retval EFI_SUCESS            Action completed successfully
  @retval EFI_INVALID_PARAMETER Invalid parameter.
  @retval EFI_UNSUPPORTED       Capsule image is not supported by the firmware.
**/
EFI_STATUS
EFIAPI
DeInitCapsulePtrMemoryForCapsuleRawFile (
  IN OUT VOID               **CapsulePtr
  )
{
  return DeInitCapsulePtrMemoryGeneric (CapsulePtr);
}
