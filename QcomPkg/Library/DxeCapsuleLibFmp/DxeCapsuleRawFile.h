/** @file
  Header for operations on capsule partition from DXE capsule library.

  Copyright (c) 2017, Qualcomm Technologies, Inc. All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential.
**/
/*=============================================================================
                              EDIT HISTORY


 when       who     what, where, why
 --------   ---     -----------------------------------------------------------
 03/24/17   gparam  New functions to abstract some more operations on capsule
                    persisting media + Restructuring of code between files
 02/22/17   gparam  Initial revision

=============================================================================*/

#ifndef _DXE_CAPSULE_RAW_FILE_H_
#define _DXE_CAPSULE_RAW_FILE_H_

#include <Uefi.h>
#include <Library/QcomCapsuleHeader.h>

/**
 * Firmware Update Table - stores information of this table along with the file
 * offset of each Record Table referenced by this table 
 */
typedef struct {
  ///
  /// Signature to validate the start of this structure in memory.
  ///
  UINT32              Signature;

  ///
  /// Version number of this header structure.
  /// Currently set to CAPSULE_PART_GUID_TABLE_REVISION.
  ///
  UINT32              Revision;

  ///
  /// Number of valid Capsule Guid Entries
  ///
  UINT32              GuidCount;

  ///
  /// Block size of partition. 
  ///
  UINT32              BlockSize;

  ///
  /// Number of available bytes in the raw file for capsules.
  ///
  UINT32              FreeBytes;

  ///
  /// Crc32 of the entire table.
  ///
  UINT32              Crc32;

  ///
  /// Entries for each Guid in the Capsule Guid Table.
  /// GuidCount in Header determines the number of valid entries
  /// in this array.
  ///
  CAPSULE_GUID_ENTRY CapsuleGuidEntries[CAPSULE_GUID_ENTRIES_MAX];

} CAPSULE_RAW_FILE_GUID_TABLE;

/**
  Checks whether the Update Capsule partition exists.

  @retval TRUE                  Update Capsule partition does exist.
  @retval FALSE                 Update Capsule partition does not exist.

**/
BOOLEAN
EFIAPI
UpdateCapsuleRawFileExists (
  VOID
  );

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
  );


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
  IN OUT VOID                **CapsulePtrArray,
  IN OUT UINT32               *CapsuleOffsetArray,
  IN UINT32                    CapsulePtrArrayLen
  );

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
  );

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
  OUT UINT64                  *MaxiumCapsuleSize
  );

/**
  This function also loads and validates the header of the capsule raw file

  @retval TRUE                  Update Capsule partition does exist.
  @retval FALSE                 Update Capsule partition does not exist.

**/
EFI_STATUS
EFIAPI
InitStoragePointersForCapsuleRawFile (
  VOID
  );

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
  );

/**
  De-initialize the capsule pointer memory for the capsule loaded from capsule partition
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
  );

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
  );
#endif // _DXE_CAPSULE_RAW_FILE_H_
