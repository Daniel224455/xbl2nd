/** @file
  Header for additional Qualcomm functions for DXE capsule library.

  Copyright (c) 2017-2019, 2021 Qualcomm Technologies, Inc. All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential.
**/
/*=============================================================================
                              EDIT HISTORY


 when         who     what, where, why
 --------     ---     -----------------------------------------------------------
 05/28/19     pameta  Adding NV variable to mark pending capsule processing
 06/28/18     prema   Fixes to update capsule partition using unaligned buffer
 03/24/17     gparam  New functions to abstract some more operations on capsule
                      persisting media + Restructuring of code between files +
                      Correction of function headers + Removed unused struct
                      definitions
 02/22/17     gparam  Initial revision

=============================================================================*/

#ifndef _DXE_CAPSULE_QCOM_H_
#define _DXE_CAPSULE_QCOM_H_

#include <Uefi.h>
#include <Library/QcomCapsuleHeader.h>

#define MAX_FILE_NAME_SIZE                      1024
#define FILE_COUNT_STEP                         5
#define UINTN_MAX_VALUE                         ((UINTN) (-1))

// based on header entries in CAPSULE_PARTITION_GUID_TABLE
#define CAPSULE_GUID_ENTRIES_MAX                20

#define CAPSULE_INVALID_GUID \
  { 0x00000000, 0x0000, 0x0000, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00} }

#define CAPSULE_GUID_TABLE_SIGNATURE            SIGNATURE_32('C','A','P','S')

/**
  Macro that returns the size aligned to the given block size.

  @param  OrigSize  A size in bytes.  This parameter is assumed to be type UINTN.  
                    Passing in a parameter that is larger than UINTN may produce 
                    unexpected results.

  @param  BlockSize Size of a block in bytes.

  @param  AlignedSize Aligned size in bytes. This parameter is assumed to be type UINTN.  
                      Passing in a parameter that is larger than UINTN may produce 
                      unexpected results.
**/
#define ALIGN_TO_BLOCK(OrigSize,BlockSize,AlignedSize)  \
  do { \
       AlignedSize = OrigSize + (BlockSize - 1);\
       AlignedSize = AlignedSize & ~(BlockSize -1);\
     \
  } while (0) 

/**
  Macro that converts a size, in bytes, to a number of blocks.

  @param  Size      A size in bytes.  This parameter is assumed to be type UINTN.  
                    Passing in a parameter that is larger than UINTN may produce 
                    unexpected results.

  @param  BlockSize Size of a block in bytes.

  @return  The number of blocks associated with the number of bytes specified
           by Size.

**/
#define EFI_SIZE_TO_BLOCKS(Size, BlockSize)  (((Size) / BlockSize) + (((Size) & (BlockSize - 1)) ? 1 : 0))

/**
  Macro that converts a number of blocks to a size in bytes.

  @param  Blocks    The number of blocks.  This parameter is assumed to be 
                    type UINTN.  Passing in a parameter that is larger than 
                    UINTN may produce unexpected results.

  @param  BlockSize Size of a block in bytes.

  @return  The number of bytes associated with the number of blocks specified 
           by Pages.
  
**/
#define EFI_BLOCKS_TO_SIZE(Blocks, BlockSize)  ((Blocks * BlockSize))

extern EFI_GUID gQcomScatterGatherListHeaderGuid;

/**
  This header describes an array of 
  EFI_PHYSICAL_ADDRESSes, each of which points 
  to a ScatterGatherList. 
   
  Header description: 
   
  +------------------------------------------+ 
  | Revision - Revision 1                    | 
  +------------------------------------------+ 
  |GUID - Signature                          | 
  +------------------------------------------+ 
  |PointerCount - the number of elements in  | 
  |  our ScatterGatherList array|            | 
  +------------------------------------------+ 
  |Offset - Offset from the beginning of     |
  | this header to the beginning of the      | 
  | ScatterGatherList array. This value is   |
  |  usually the size of this header         | 
  +------------------------------------------+ 
  |Crc32 - Header CRC                        | 
  +------------------------------------------+ 
  |(Beginning of Array)                      | 
  +------------------------------------------+ 
 
*/
typedef struct 
{
    UINT32                  Revision;
    EFI_GUID                ScatterGatherHeaderGuid;
    UINT32                  PointerCount;
    UINT32                  ArrayOffset;  //Offset from beginning of header
    UINT32                  Crc32;
    
}SCATTERGATHERLIST_ARRAY_HEADER;

/**
 * Capsule Guid Entry - stores the GUID and location of a capsule within
 * the partition.
 */
typedef struct {
  /// 
  /// Guid for the capsule. If set to CAPSULE_INVALID_GUID, then this capsule
  /// has been consumed and is no longer available for firmware update.
  /// 
  EFI_GUID            Guid;

  union {
    /// 
    /// For CAPSULE_PARTITION_GUID_TABLE
    /// Block offset from start of the partition to where the capsule 
    /// header/data is stored. Each capsule is stored on a block alignment.
    /// 
    UINT32            BlockOffset;

    /// 
    /// For CAPSULE_RAW_FILE_GUID_TABLE
    /// Offset from start of the EFI raw file to where the capsule 
    /// header/data is stored.
    /// 
    UINT32            FileOffset;
  };

  /// 
  /// Size in bytes of the capsule header/data
  /// 
  UINT32              ImageSize;

  ///
  /// Flags for the capsule
  ///
  UINT32              Flags;

}CAPSULE_GUID_ENTRY;

/**
  Return if this CapsuleGuid is a FMP capsule GUID or not.

  @param[in] CapsuleGuid A pointer to EFI_GUID

  @retval TRUE  It is a FMP capsule GUID.
  @retval FALSE It is not a FMP capsule GUID.
**/
BOOLEAN
IsFmpCapsuleGuid (
  IN EFI_GUID  *CapsuleGuid
  );

/**
  Checks if any of the image descriptors have attribute IMAGE_ATTRIBUTE_RESET_REQUIRED.

  @param[in]  Handle              A FMP handle.
  @param[out] ResetRequired       TRUE - If any of the image descriptors have attribute
                                    IMAGE_ATTRIBUTE_RESET_REQUIRED.
                                  FALSE - If none of the image descriptors have attribute
                                    IMAGE_ATTRIBUTE_RESET_REQUIRED.

  @retval EFI_SUCESS            Action completed successfully
  @retval EFI_INVALID_PARAMETER Invalid input parameter.

**/
EFI_STATUS
GetFmpImageInfoImageAttribResetRequired (
  IN EFI_HANDLE                                   Handle,
  OUT BOOLEAN                                     *ResetRequired
  );

/**
  This function initializes the mCapsulePtr, mCapsuleOffsetArray,
  mCapsuleStatusArray, mCapsuleFileNameArray and mCapsuleTotalNumber
  from the media used to persist capsules

  @param[in] CapsuleStorage       Type of storage to process capsule from. 

  @retval EFI_SUCESS            Action completed successfully
  @retval EFI_INVALID_PARAMETER Invalid input parameter.
  @retval EFI_END_OF_MEDIA      No persisted capsules found.

**/
EFI_STATUS
InitCapsulePtrFromPersistedMedia(
  IN CapsuleStorageType          CapsuleStorage
  );

/**
  Returns the unique GUID of the capsule.

  @param[in] CapsuleHeader      A pointer to EFI_CAPSULE_HEADER
  @param[in] UniqueCapsuleGuid  Unique GUID of the capsule

  @retval TRUE  It is a system FMP.
  @retval FALSE It is a device FMP.
**/
EFI_STATUS
GetUniqueCapsuleGuid (
  IN EFI_CAPSULE_HEADER         *CapsuleHeader,
  OUT EFI_GUID                  *UniqueCapsuleGuid
  );

/**
  Delete given capsule from given storage media

  @param[in]  CapsuleHeader    Points to the UEFI capsule image to be processed. 
  @param[in]  CapsuleOffset    Offset of the capsule in storage, if 0 capsule did
                               not come from storage
  @param[in]  CapsuleFileName  File name of the mass-storage capsule.
  @param[in]  CapsuleStorage   Type of storage to process capsule from. 

  @retval EFI_UNSUPPORTED  Capsule image is not supported by the firmware.
  **/
EFI_STATUS
EFIAPI
DeleteCapsule (
    IN EFI_CAPSULE_HEADER *CapsuleHeader,
    IN UINT32              CapsuleOffset,
    IN CHAR16             *CapsuleFileName,
    IN CapsuleStorageType  CapsuleStorage
  );

/**
  De-initialize the capsule pointer memory for the given storage media
  This function clears the pointer after de-initialization

  @param[in]  CapsulePtr       Points to the pointer to the capsule to be deinitialized. 
  @param[in]  CapsuleStorage   Type of storage to process capsule from. 

  @retval EFI_UNSUPPORTED  Capsule image is not supported by the firmware.
  **/
EFI_STATUS
EFIAPI
DeInitCapsulePtrMemory (
    IN VOID               **CapsulePtr,
    IN CapsuleStorageType   CapsuleStorage
  );

/**
  Default function to de-initialize the capsule pointer memory through simple FreePool.
  This can be used unless there is any other storage media specific de-initialization needed

  This function clears the pointer after de-initialization

  @param[in]  CapsulePtr       Points to the pointer to the capsule to be deinitialized. 

  @retval EFI_SUCESS            Action completed successfully
  @retval EFI_INVALID_PARAMETER Invalid parameter.
  @retval EFI_UNSUPPORTED       Capsule image is not supported by the firmware.
**/
EFI_STATUS
EFIAPI
DeInitCapsulePtrMemoryGeneric (
  IN OUT VOID               **CapsulePtr
  );

/**
  Flush persisted capsules in the given storage media

  This function clears the pointer after de-initialization

  @param[in]  CapsuleStorage   Type of storage to process capsule from. 

  @retval EFI_SUCESS            Action completed successfully
  @retval EFI_INVALID_PARAMETER Invalid parameter.
  @retval EFI_UNSUPPORTED       Capsule image is not supported by the firmware.
**/
EFI_STATUS
EFIAPI
FlushPersistedCapsules (
  IN CapsuleStorageType   CapsuleStorage
  );

/**
  Returns if OsIndications variable has EFI_OS_INDICATIONS_FILE_CAPSULE_DELIVERY_SUPPORTED
  flag enabled.

  @retval TRUE  It is a system FMP.
  @retval FALSE It is a device FMP.
**/
BOOLEAN
IsMassStorageCapsuleFlagEnabled (
  );

/**
  Mark persisted media capsule processing pending
  This flag read by MSFT enterprise code. .
  @retval VOID.
**/
VOID
MarkCapsulePendingInPersistedMedia (
  VOID
  );

/**
  Validate if it is valid capsule header

  This function assumes the caller provided correct CapsuleHeader pointer
  and CapsuleSize.

  This function validates the fields in EFI_CAPSULE_HEADER.

  @param[in]  CapsuleHeader    Points to a capsule header.
  @param[in]  CapsuleSize      Size of the whole capsule image.

**/
BOOLEAN
IsValidCapsuleHeader (
  IN EFI_CAPSULE_HEADER  *CapsuleHeader,
  IN UINT64              CapsuleSize
  );
#endif // _DXE_CAPSULE_QCOM_H_
