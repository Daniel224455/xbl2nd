/** @file
  Qualcomm capsule header file.
 
  Copyright (c) 2017, Qualcomm Technologies, Inc. All Rights Reserved.
Portions Copyright (c) 2011, Intel Corporation. All rights 
reserved.<BR> This program and the accompanying materials 
are licensed and made available under the terms and conditions of the BSD License
which accompanies this distribution.  The full text of the license may be found at
http://opensource.org/licenses/bsd-license.php

THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/
/*=============================================================================
                              EDIT HISTORY


 when       who     what, where, why
 --------   ---     -----------------------------------------------------------
 05/08/17   gparam  Minor tweaks to logging
 03/24/17   gparam  New functions to abstract some more operations on capsule
                    persisting media + Restructuring of code between files +
                    Correction of function headers
 03/15/17   gparam  Moved BdsFormalizeEfiGlobalVariable from QcomBds to
                    to DxeCapsuleLibFmp to avoid creating stubs for use in LA
 02/22/17   gparam  Initial revision

=============================================================================*/

#ifndef _QCOM_CAPSULE_HEADER_H_
#define _QCOM_CAPSULE_HEADER_H_

#include <Uefi.h>
#include <Library/QcomLib.h>

/*
 * Capsule Guid Entry - stores the GUID and location of a capsule within
 * the partition.
 */
typedef enum {
  CAPSULE_STORAGE_TYPE_CAPSULE_RAW_FILE     = 0x0,
  CAPSULE_STORAGE_TYPE_CAPSULE_PARTITION    = 0x1,
  CAPSULE_STORAGE_TYPE_MASS_STORAGE         = 0x2,
  CAPSULE_STORAGE_TYPE_RAM                  = 0x3,
  CAPSULE_STORAGE_TYPE_UNKNOWN              = 0xFFFF
} CapsuleStorageType;

#define CAPSULESTORAGETYPE_TO_CAPSULESTORAGETYPE_STRING(x) \
    ((x == CAPSULE_STORAGE_TYPE_CAPSULE_RAW_FILE  ) ? L"EFI Raw file" : \
     (x == CAPSULE_STORAGE_TYPE_CAPSULE_PARTITION ) ? L"Capsule partition" : \
     (x == CAPSULE_STORAGE_TYPE_MASS_STORAGE      ) ? L"EFI\\UpdateCapsule folder" : \
     (x == CAPSULE_STORAGE_TYPE_RAM               ) ? L"RAM" : \
     (x == CAPSULE_STORAGE_TYPE_UNKNOWN           ) ? L"Unkown storage" : \
     L"INVALID"                                   )

/**
  Checks whether the currently used media to persist capsules exists.

  @param[in] CapsuleStorage     Type of storage to process capsule from. 

  @retval TRUE                  Update Capsule partition does exist.
  @retval FALSE                 Update Capsule partition does not exist.

**/
BOOLEAN
EFIAPI
CapsulePersistedMediaExists (
  IN CapsuleStorageType          CapsuleStorage
  );

/**
  Get the maximum capsule size supported in the current storage type.
  
  @param  MaxiumCapsuleSize     Maximum capsule size supported in the update
                                capsule partition.
   
  @retval EFI_SUCESS            Action completed successfully
  @retval EFI_INVALID_PARAMETER Invalid input parameter.

**/
EFI_STATUS
EFIAPI
GetMaxCapsuleSize (
  OUT UINT64             *MaxiumCapsuleSize
  );

/**
  Get the space needed to store the capsule in the current storage media
  
  @param [in]  CapsuleImageSize           Size of the capsule eceived in the capsule
                                          header array.
  @param [out] CapsuleSizeInStorageMedia  Size needed to store the capsule
                                          in the current storage media

  @retval EFI_SUCESS            Action completed successfully
  @retval EFI_INVALID_PARAMETER Invalid parameter.
  @retval EFI_UNSUPPORTED       Capsule image is not supported by the firmware.
  @retval EFI_NO_MEDIA          Capsule storage media not found.
**/
EFI_STATUS
EFIAPI
GetCapsuleSizeInStorageMedia (
  IN UINT32             CapsuleImageSize,
  OUT UINT32           *CapsuleSizeInStorageMedia
  );

/**
  Initialize the pointers needed to acess the storage type used to persist capsules.
  
  @param[in] CapsuleStorage     Type of storage to process capsule from. 

  @retval EFI_SUCESS            Action completed successfully
  @retval EFI_INVALID_PARAMETER Invalid input parameter.

**/
EFI_STATUS
EFIAPI
InitCapsuleStoragePointers (
  IN CapsuleStorageType          CapsuleStorage
  );

/**

  This code loads capsules in boot time from the NV storage and processes them.

  Before calling this function, the modules should call InitCapsuleStoragePointers

  @param[in] CapsuleHeaderArray Virtual pointer to an array of virtual pointers to the capsules
                                being passed into update capsule.
  @param[in] CapsuleCount       Number of pointers to EFI_CAPSULE_HEADER in
                                CaspuleHeaderArray.
  @param[in] ScatterGatherList  Physical pointer to a set of
                                EFI_CAPSULE_BLOCK_DESCRIPTOR that describes the
                                location in physical memory of a set of capsules.

  @retval EFI_SUCCESS           Valid capsule was passed. If
                                CAPSULE_FLAGS_PERSIT_ACROSS_RESET is not set, the
                                capsule has been successfully processed by the firmware.
  @retval EFI_DEVICE_ERROR      The capsule update was started, but failed due to a device error.
  @retval EFI_OUT_OF_RESOURCES  When ExitBootServices() has been previously called this error indicates the capsule 
                                is compatible with this platform but is not capable of being submitted or processed 
                                in runtime. The caller may resubmit the capsule prior to ExitBootServices().
  @retval EFI_OUT_OF_RESOURCES  When ExitBootServices() has not been previously called then this error indicates 
                                the capsule is compatible with this platform but there are insufficient resources to process.

**/
EFI_STATUS
EFIAPI
PersistCapsules (
  IN EFI_CAPSULE_HEADER   **CapsuleHeaderArray,
  IN UINTN                  CapsuleCount,
  IN EFI_PHYSICAL_ADDRESS   ScatterGatherList OPTIONAL
  );

/**
  The firmware-specific implementation processes the capsule image
  if it recognized the format of this capsule image.
  
  @param  CapsuleHeader    Pointer to the UEFI capsule image to be processed. 
  @param  CapsuleOffset    Offset of the capsule in persistent storage, 0 if
                           capsule didn't come from storage
  @param  CapsuleFileName  In case of capsule loaded from disk, the zero-
                           terminated array containing file name of capsule
                           that was processed
  @param  ResetNeeded      Returns if a process capsule that needs a reset was
                           processed
   
  @retval EFI_SUCESS       Capsule Image processed successfully. 
  @retval EFI_UNSUPPORTED  Capsule image is not supported by the firmware.

**/
EFI_STATUS
EFIAPI
ProcessCapsuleImageEx (
  IN EFI_CAPSULE_HEADER *CapsuleHeader,
  IN UINT32              CapsuleOffset,
  IN CHAR16             *CapsuleFileName OPTIONAL,
  OUT BOOLEAN           *ResetNeeded OPTIONAL
  );
/**
  Formalize Bds global variables.

 1. For OsIndicationsSupported, Create a BS/RT/UINT64 variable to report caps
 2. Delete OsIndications variable if it is not NV/BS/RT UINT64
 Item 3 is used to solve case when OS corrupts OsIndications. Here simply delete this NV variable.

**/
VOID
BdsFormalizeEfiGlobalVariable (
  VOID
  );
#endif // _QCOM_CAPSULE_HEADER_H_
