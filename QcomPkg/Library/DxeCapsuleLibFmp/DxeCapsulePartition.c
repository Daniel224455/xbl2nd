/** @file
  Qualcomm functions for operations on capsule partition from DXE capsule library.

  Caution: This module requires additional review when modified.
  This module will have external input - capsule image.
  This external input must be validated carefully to avoid security issue like
  buffer overflow, integer overflow.

  Copyright (c) 2017, Qualcomm Technologies, Inc. All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential.
**/
/*=============================================================================
                              EDIT HISTORY


 when       who     what, where, why
 --------   ---     -----------------------------------------------------------
 03/24/17   gparam  New functions to abstract some more operations on capsule
                    persisting media + Correction of function headers +
                    Changes to do memory free in a cleaner way
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
#include "DxeCapsulePartition.h"

EFI_BLOCK_IO_PROTOCOL        *CapsuleBlkIo = NULL;

extern VOID                 **mCapsulePtr;
extern UINT32                *mCapsuleOffsetArray;
extern CHAR16               **mCapsuleFileNameArray;
extern VOID                  *CapsuleStorageHeader;
extern UINT32                 mCapsuleTotalNumber;
extern EFI_STATUS            *mCapsuleStatusArray;


/**
  Validates the capsule GUID table in capsule partition.
  
  @param[in]  CapsuleGuidTable Capsule GUID table buffer pointer to be
                               initialized and updated to the storage media.

  @retval EFI_SUCESS            Action completed successfully
  @retval EFI_INVALID_PARAMETER Invalid input parameter.

**/
EFI_STATUS
EFIAPI
ValidateCapsulePartitionGuidTable (
  IN CAPSULE_PARTITION_GUID_TABLE *CapsuleGuidTable
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
                            sizeof(CAPSULE_PARTITION_GUID_TABLE),
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
  Initialize the capsule GUID table in capsule partition. This accepts the
  pointer to the memory that can be used as the buffer to initialize the
  default data and updates the same to the capsule partition
  
  @param[in]  CapsuleGuidTable Capsule GUID table buffer pointer to be
                               initialized and updated to the storage media.

  @retval EFI_SUCESS            Action completed successfully
  @retval EFI_INVALID_PARAMETER Invalid input parameter.

**/
EFI_STATUS
EFIAPI
InitCapsulePartitionGuidTable (
  CAPSULE_PARTITION_GUID_TABLE *CapsuleGuidTable
  )
{
  EFI_STATUS         Status = EFI_SUCCESS;
  UINTN              HeaderBlocks = EFI_SIZE_TO_BLOCKS (sizeof (*CapsuleGuidTable), CapsuleBlkIo->Media->BlockSize);

  if (NULL == CapsuleGuidTable) {
    return EFI_INVALID_PARAMETER;
  }

  if (!UpdateCapsulePartitionExists()) {
    Status = EFI_NOT_FOUND;
    DEBUG ((EFI_D_ERROR, "Cannot initialize GUID table as capsule partition does not exist\n"));
    return Status;
  }

  // This effectively deletes all capsules in the partition
  CapsuleGuidTable->Signature = CAPSULE_GUID_TABLE_SIGNATURE;
  CapsuleGuidTable->Revision = CAPSULE_PART_GUID_TABLE_REVISION;
  CapsuleGuidTable->GuidCount = 0;
  CapsuleGuidTable->BlockSize = CapsuleBlkIo->Media->BlockSize;
  CapsuleGuidTable->BlockCount = CapsuleBlkIo->Media->LastBlock + 1;
  CapsuleGuidTable->FreeBlocks = CapsuleGuidTable->BlockCount - HeaderBlocks ;

  // Clear out all Guid entries
  SetMem(&(CapsuleGuidTable->CapsuleGuidEntries[0]),
         sizeof(CapsuleGuidTable->CapsuleGuidEntries),
         0x0);

  CapsuleGuidTable->Crc32 = 0;
  Status = CalculateCrc32(CapsuleGuidTable,
                          sizeof(CAPSULE_PARTITION_GUID_TABLE),
                          &(CapsuleGuidTable->Crc32));
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Copy back Capsule Guid Table to persisted storage
  Status = CapsuleBlkIo->WriteBlocks (CapsuleBlkIo,
                                      CapsuleBlkIo->Media->MediaId,
                                      0,
                                      EFI_BLOCKS_TO_SIZE (HeaderBlocks, CapsuleBlkIo->Media->BlockSize),
                                      CapsuleGuidTable);
  if (EFI_ERROR (Status)) {
    return Status;
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
UpdateCapsulePartitionExists (
  VOID
  )
{
  if (NULL != CapsuleBlkIo) {
    return TRUE;
  } else {
    return FALSE;
  }
}

/**
  Delete the capsule matching the input parameter from persisted capsules in capsule partition.
  
  @param[in]  CapsuleGuid      File name of the mass-storage capsule.
  @param[in]  CapsuleOffset    Offset of the capsule in storage, if 0 capsule did
                               not come from storage

  @retval EFI_SUCESS            Action completed successfully
  @retval EFI_INVALID_PARAMETER Invalid input parameter.

**/
EFI_STATUS
EFIAPI
DeletePersistedCapsuleFromCapsulePartition(
  IN EFI_GUID           *CapsuleGuid,
  IN UINT32              CapsuleOffset
  )
{
  EFI_STATUS                   Status = EFI_SUCCESS;
  UINT32                       *MemLoc = NULL;
  UINT32                       ImgSize = 0;
  CAPSULE_PARTITION_GUID_TABLE *CapsuleGuidTable = NULL;
  UINT32                       Index = 0;

  do
  {
    if (NULL == CapsuleGuid) {
      DEBUG ((DEBUG_ERROR, "Invalid value received for parameter CapsuleGuid\n"));
      Status = EFI_INVALID_PARAMETER;
      break;
    }

    if (!UpdateCapsulePartitionExists())
    {
      Status = EFI_UNSUPPORTED;
      break;
    }
    /* Get the Capsule Guid Table - stored in first block */
    ImgSize = CapsuleBlkIo->Media->BlockSize;
    Status = gBS->AllocatePool(EfiBootServicesData,
                               ImgSize,
                               (VOID**)&MemLoc);

    if (Status != EFI_SUCCESS)
      break;

    // Load the Capsule Guid Table into memory
    Status = CapsuleBlkIo->ReadBlocks (CapsuleBlkIo, 
                                       CapsuleBlkIo->Media->MediaId, 
                                       0, 
                                       ImgSize, 
                                       MemLoc);
    if (EFI_ERROR (Status))
      break;

    CapsuleGuidTable = (CAPSULE_PARTITION_GUID_TABLE*)MemLoc;
    Status = ValidateCapsulePartitionGuidTable(CapsuleGuidTable);
    if (EFI_ERROR (Status))
      break;

    // Find Guid entry corresponding to capsule
    for (Index = 0; Index < CapsuleGuidTable->GuidCount; Index++)
    {
      if ((CompareGuid(&(CapsuleGuidTable->CapsuleGuidEntries[Index].Guid), 
                       CapsuleGuid) == TRUE) &&
          (CapsuleOffset == CapsuleGuidTable->CapsuleGuidEntries[Index].BlockOffset))
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
      CapsuleGuidTable->Revision = CAPSULE_PART_GUID_TABLE_REVISION;
      CapsuleGuidTable->GuidCount = 0; 
      CapsuleGuidTable->BlockSize = CAPSULE_PART_GUID_TABLE_BLOCK_SIZE;
      CapsuleGuidTable->BlockCount = CapsuleBlkIo->Media->LastBlock;
      CapsuleGuidTable->FreeBlocks = CapsuleBlkIo->Media->LastBlock - 1;

      // Clear out all Guid entries
      SetMem(&(CapsuleGuidTable->CapsuleGuidEntries[0]), 
             sizeof(CapsuleGuidTable->CapsuleGuidEntries),
             0x0);
    }

    CapsuleGuidTable->Crc32 = 0;
    Status = CalculateCrc32(CapsuleGuidTable, 
                            sizeof(CAPSULE_PARTITION_GUID_TABLE), 
                            &(CapsuleGuidTable->Crc32));
    if (EFI_ERROR (Status))
      break;

    // Copy back Capsule Guid Table to persisted storage
    Status = CapsuleBlkIo->WriteBlocks (CapsuleBlkIo, 
                                        CapsuleBlkIo->Media->MediaId, 
                                        0, 
                                        ImgSize, 
                                        MemLoc);
    if (EFI_ERROR (Status))
      break;

  }while(0);

  if (Status == EFI_SUCCESS)
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
LoadPersistedCapsulesFromCapsulePartition (
  IN OUT VOID **CapsulePtrArray,
  IN OUT UINT32 *CapsuleOffsetArray,
  IN UINT32 CapsuleArrayLen
  )
{
  EFI_STATUS                    Status = EFI_SUCCESS;
  EFI_GUID                      InvalidGuid = CAPSULE_INVALID_GUID;
  CAPSULE_PARTITION_GUID_TABLE  *CapsuleGuidTable = (CAPSULE_PARTITION_GUID_TABLE *) CapsuleStorageHeader;
  CAPSULE_GUID_ENTRY            *CapsuleGuidEntry = NULL;
  UINT32                        Index;

  UINT32                        *MemLoc = NULL;
  UINT32                        ImgSize = 0;

  do {
    if (NULL == CapsulePtrArray) {
        Status = EFI_INVALID_PARAMETER;
        DEBUG ((DEBUG_INFO, "Invalid value received for parameter CapsulePtrArray\n"));
        break;
    }

    if (NULL == CapsuleOffsetArray) {
        Status = EFI_INVALID_PARAMETER;
        DEBUG ((DEBUG_INFO, "Invalid value received for parameter CapsuleOffsetArray\n"));
        break;
    }

    if (CapsuleBlkIo == NULL) {
      Status = EFI_NOT_FOUND;
      DEBUG ((DEBUG_INFO, "Capsule partition read pointers not initialized. Error in loading capsules from partition\n"));
      break;
    }

    // Calculate CRC on table, if doesn't match or is uninitialized, reinitialize
    Status = ValidateCapsulePartitionGuidTable(CapsuleGuidTable);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, "ValidateCapsulePartitionGuidTable returned error - %r\n", Status));
      break;
    }

    if (CapsuleArrayLen != CapsuleGuidTable->GuidCount) {
      Status = EFI_INVALID_PARAMETER;
      DEBUG ((DEBUG_INFO, "ValidateCapsulePartitionGuidTable returned error - %r\n", Status));
      break;
    }
      
    // Load each capsule and store it to capsule pointer
    for (Index = 0; Index < CapsuleGuidTable->GuidCount; Index++) {
      CapsuleGuidEntry = &(CapsuleGuidTable->CapsuleGuidEntries[Index]);

      // Confirm GUID entry is valid
      if (CompareGuid(&(CapsuleGuidEntry->Guid), &InvalidGuid)) {
        // This should never be the case
        Status = EFI_VOLUME_CORRUPTED;
        DEBUG ((DEBUG_INFO, "Capsule partition corrupted. Invalid capsule table GUID\n"));
        break;
      }

      // Get the Capsule size. Account for one extra block in case not a
      // multiple of block size
      ImgSize = EFI_BLOCKS_TO_SIZE (EFI_SIZE_TO_BLOCKS (CapsuleGuidEntry->ImageSize, CapsuleBlkIo->Media->BlockSize), CapsuleBlkIo->Media->BlockSize);

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
        break;
      }

      // Load entire capsule into memory
      Status = CapsuleBlkIo->ReadBlocks (CapsuleBlkIo,
                                         CapsuleBlkIo->Media->MediaId,
                                         CapsuleGuidEntry->BlockOffset,
                                         ImgSize,
                                         MemLoc);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_INFO, "Capsule partition read for index %d reurned error - %r\n", Index, Status));
        break;
      }
        
      // Copy to pointer to Capsule pointer array
      CapsulePtrArray[Index] = MemLoc;
      
      // Copy to block offset to Capsule offset array
      CapsuleOffsetArray[Index] = CapsuleGuidEntry->BlockOffset;
    }
  } while(0);

  return Status;
}

/**
  Persists capsules received in the form of capsule array to capsule partition.

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
PersistCapsulesToCapsulePartition (
  IN EFI_CAPSULE_HEADER    **CapsuleHeaderArray,
  IN UINTN                   CapsuleCount
  )
{
  UINTN                         ArrayNumber = 0;
  EFI_STATUS                    Status = EFI_SUCCESS;
  EFI_CAPSULE_HEADER           *CapsuleHeader = NULL;
  UINTN                         BufferBlocks = 0;
  CAPSULE_PARTITION_GUID_TABLE *CapsuleGuidTable = (CAPSULE_PARTITION_GUID_TABLE *) CapsuleStorageHeader;
  EFI_GUID                      UniqueCapsuleGuid;

  //
  // Walk through all capsules, store capsules to persist
  //
  for (ArrayNumber = 0; ArrayNumber < CapsuleCount ; ArrayNumber++) {
    CapsuleHeader = CapsuleHeaderArray[ArrayNumber];
    if ((CapsuleHeader->Flags & CAPSULE_FLAGS_PERSIST_ACROSS_RESET) != 0) {
      if ((CapsuleGuidTable->GuidCount >= CAPSULE_GUID_ENTRIES_MAX) ||
          ((CapsuleGuidTable->FreeBlocks * CapsuleGuidTable->BlockSize) < CapsuleHeader->CapsuleImageSize)) {
        return EFI_OUT_OF_RESOURCES;
      }

      Status = GetUniqueCapsuleGuid (CapsuleHeader, &UniqueCapsuleGuid);
      if (EFI_ERROR (Status)) {
        DEBUG ((EFI_D_ERROR, "Failed to get unique GUID of capsule to store in capsule partition. Returning error %r\n", Status));
        return Status;
      }

      CopyMem (&CapsuleGuidTable->CapsuleGuidEntries[CapsuleGuidTable->GuidCount].Guid, &UniqueCapsuleGuid, sizeof(EFI_GUID));
      CapsuleGuidTable->CapsuleGuidEntries[CapsuleGuidTable->GuidCount].BlockOffset = CapsuleGuidTable->BlockCount - CapsuleGuidTable->FreeBlocks;
      CapsuleGuidTable->CapsuleGuidEntries[CapsuleGuidTable->GuidCount].ImageSize = CapsuleHeader->CapsuleImageSize;
      BufferBlocks = EFI_SIZE_TO_BLOCKS (CapsuleHeader->CapsuleImageSize, CapsuleGuidTable->BlockSize);
      CapsuleGuidTable->FreeBlocks -= BufferBlocks;
      CapsuleGuidTable->CapsuleGuidEntries[CapsuleGuidTable->GuidCount].Flags = CapsuleHeader->Flags;
      Status = CapsuleBlkIo->WriteBlocks (CapsuleBlkIo,
                                          CapsuleBlkIo->Media->MediaId,
                                          CapsuleGuidTable->CapsuleGuidEntries[CapsuleGuidTable->GuidCount].BlockOffset,
                                          EFI_BLOCKS_TO_SIZE (BufferBlocks, CapsuleGuidTable->BlockSize),
                                          CapsuleHeader);
      CapsuleGuidTable->GuidCount++;
      if (EFI_ERROR (Status)) {
        return EFI_DEVICE_ERROR;
      }
    }
  }

  //Update CRC and store header
  CapsuleGuidTable->Crc32 = 0;
  Status = CalculateCrc32(CapsuleGuidTable,
                          sizeof(CAPSULE_PARTITION_GUID_TABLE),
                          &(CapsuleGuidTable->Crc32));
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  BufferBlocks = EFI_SIZE_TO_BLOCKS (sizeof(CAPSULE_PARTITION_GUID_TABLE), CapsuleGuidTable->BlockSize);

  Status = CapsuleBlkIo->WriteBlocks (CapsuleBlkIo,
                                      CapsuleBlkIo->Media->MediaId,
                                      0,
                                      EFI_BLOCKS_TO_SIZE (BufferBlocks, CapsuleGuidTable->BlockSize),
                                      CapsuleGuidTable);
  if (EFI_ERROR (Status)) {
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
GetMaxCapsuleSizeInCapsulePartition (
  OUT UINT64             *MaxiumCapsuleSize
  )
{
  EFI_STATUS Status = EFI_SUCCESS;

  if (NULL != MaxiumCapsuleSize) {
    CAPSULE_PARTITION_GUID_TABLE *CapsuleGuidTable = (CAPSULE_PARTITION_GUID_TABLE *)CapsuleStorageHeader;

    *MaxiumCapsuleSize = EFI_BLOCKS_TO_SIZE (CapsuleGuidTable->FreeBlocks,
                                             CapsuleBlkIo->Media->BlockSize);    
  } else {
    Status = EFI_INVALID_PARAMETER;
  }
  
  return Status;
}

/**
  Get the space needed to store the capsule in the current partition
  
  @param  CapsuleImageSize      Size of the capsule eceived in the capsule
                                header array.
   
  @retval Size needed to store the capsule in the current storage media

**/
UINTN
EFIAPI
GetCapsuleSizeInCapsulePartition (
  IN UINT32             CapsuleImageSize
  )
{
  return EFI_BLOCKS_TO_SIZE (EFI_SIZE_TO_BLOCKS (CapsuleImageSize,
                                                 CapsuleBlkIo->Media->BlockSize),
                             CapsuleBlkIo->Media->BlockSize);
}

/**
  Checks whether the Update Capsule partition exists.
  This function also loads and validates the header of the capsule partition

  @retval EFI_SUCESS            Action completed successfully
  @retval EFI_INVALID_PARAMETER Invalid input parameter.
**/
EFI_STATUS
EFIAPI
InitStoragePointersForCapsulePartition (
  VOID
  )
{
  EFI_STATUS  Status = EFI_SUCCESS;
  UINTN       NoHandles = 0;
  EFI_HANDLE *HandleBuffer = NULL;
  UINTN       HeaderSize = 0;

  if (NULL != CapsuleBlkIo) {
    return EFI_SUCCESS;
  }

  Status = gBS->LocateHandleBuffer (ByProtocol
                                  , &gUpdateCapsulePartitionGuid,
                                    NULL,
                                    &NoHandles,
                                    &HandleBuffer);
  if ((EFI_ERROR (Status)) || (HandleBuffer == NULL)) {
    DEBUG ((EFI_D_ERROR, "Update Capsule partition not found. Status: %r\n", Status));
    return (EFI_ERROR (Status) ? Status : EFI_NOT_FOUND);
  }

  if (NoHandles != 1) {
    gBS->FreePool (HandleBuffer);
    DEBUG ((EFI_D_ERROR, "More than 1 update Capsule partition found\n"));
    return EFI_DEVICE_ERROR;
  }

  Status = gBS->HandleProtocol (HandleBuffer[0], &gEfiBlockIoProtocolGuid, (VOID **)&CapsuleBlkIo);
  if (EFI_ERROR (Status)) {
    gBS->FreePool (HandleBuffer);
    DEBUG ((EFI_D_ERROR, "Update Capsule partition does not support block io. Status: %r\n", Status));
    return Status;
  }

  gBS->FreePool (HandleBuffer);
  HandleBuffer = NULL;

  if (NULL == CapsuleBlkIo) {
    DEBUG ((EFI_D_ERROR, "Capsule partition block io is NULL\n"));
    return EFI_DEVICE_ERROR;
  }

  HeaderSize = EFI_BLOCKS_TO_SIZE (EFI_SIZE_TO_BLOCKS (sizeof (CAPSULE_PARTITION_GUID_TABLE), CapsuleBlkIo->Media->BlockSize), CapsuleBlkIo->Media->BlockSize);

  Status = gBS->AllocatePool (EfiRuntimeServicesData, HeaderSize, &CapsuleStorageHeader);
  if (EFI_ERROR (Status) || (CapsuleStorageHeader == NULL)) {
    DEBUG ((EFI_D_ERROR, "Failed to allocate capsule partition storage header. Status: %r\n", Status));
    return (EFI_ERROR (Status) ? Status : EFI_OUT_OF_RESOURCES);
  }

  Status = CapsuleBlkIo->ReadBlocks (CapsuleBlkIo,
                                     CapsuleBlkIo->Media->MediaId,
                                     0,
                                     HeaderSize,
                                     CapsuleStorageHeader);
  if (EFI_ERROR (Status)) {
    gBS->FreePool (CapsuleStorageHeader);
    DEBUG ((EFI_D_ERROR, "Failed to read header. Status: %r\n", Status));
    return Status;
  }

  Status = ValidateCapsulePartitionGuidTable ((CAPSULE_PARTITION_GUID_TABLE *)CapsuleStorageHeader);
  if (EFI_ERROR (Status)) {
    Status = InitCapsulePartitionGuidTable ((CAPSULE_PARTITION_GUID_TABLE *)CapsuleStorageHeader);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "Failed to initilize capsule partition storage header. Status: %r\n", Status));
      return Status;
    }
  }

  return Status;
}

/**
  De-initialize the capsule pointer memory for the capsule loaded from
  capsule partition. This function clears the pointer after de-initialization

  @param[in]  CapsulePtr       Points to the pointer to the capsule to be
                               deinitialized. 

  @retval EFI_SUCESS            Action completed successfully
  @retval EFI_INVALID_PARAMETER Invalid parameter.
  @retval EFI_UNSUPPORTED       Capsule image is not supported by the firmware.

**/
EFI_STATUS
EFIAPI
DeInitCapsulePtrMemoryForCapsulePartition (
  IN OUT VOID               **CapsulePtr
  )
{
  return DeInitCapsulePtrMemoryGeneric (CapsulePtr);
}

