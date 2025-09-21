/** @file
  Null Dxe Capsule Library instance does nothing and returns unsupport status.

Copyright (c) 2017, Qualcomm Technologies, Inc. All Rights Reserved.
Qualcomm Technologies Proprietary and Confidential.
Portions Copyright (c) 2007 - 2016, Intel Corporation. All rights reserved.<BR>
This program and the accompanying materials
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
 02/22/17   gparam  Initial revision

=============================================================================*/

#include <Uefi.h>
#include <Library/CapsuleLib.h>
#include <Library/QcomCapsuleHeader.h>

/**
  The firmware checks whether the capsule image is supported 
  by the CapsuleGuid in CapsuleHeader or other specific information in capsule image.

  Caution: This function may receive untrusted input.

  @param  CapsuleHeader    Point to the UEFI capsule image to be checked.
  
  @retval EFI_UNSUPPORTED  Input capsule is not supported by the firmware.
**/
EFI_STATUS
EFIAPI
SupportCapsuleImage (
  IN EFI_CAPSULE_HEADER *CapsuleHeader
  )
{
  return EFI_UNSUPPORTED;
}

/**
  The firmware specific implementation processes the capsule image
  if it recognized the format of this capsule image.

  Caution: This function may receive untrusted input.

  @param  CapsuleHeader    Point to the UEFI capsule image to be processed. 
   
  @retval EFI_UNSUPPORTED  Capsule image is not supported by the firmware.
**/
EFI_STATUS
EFIAPI
ProcessCapsuleImage (
  IN EFI_CAPSULE_HEADER *CapsuleHeader
  )
{
  return EFI_UNSUPPORTED;
}

/**

  This routine is called to process capsules.

  Caution: This function may receive untrusted input.

  The capsules reported in EFI_HOB_UEFI_CAPSULE are processed.
  If there is no EFI_HOB_UEFI_CAPSULE, this routine does nothing.

  This routine should be called twice in BDS.
  1) The first call must be before EndOfDxe. The system capsules is processed.
     If device capsule FMP protocols are exposted at this time and device FMP
     capsule has zero EmbeddedDriverCount, the device capsules are processed.
     Each individual capsule result is recorded in capsule record variable.
     System may reset in this function, if reset is required by capsule and
     all capsules are processed.
     If not all capsules are processed, reset will be defered to second call.

  2) The second call must be after EndOfDxe and after ConnectAll, so that all
     device capsule FMP protocols are exposed.
     The system capsules are skipped. If the device capsules are NOT processed
     in first call, they are processed here.
     Each individual capsule result is recorded in capsule record variable.
     System may reset in this function, if reset is required by capsule
     processed in first call and second call.

  @retval EFI_SUCCESS             There is no error when processing capsules.
  @retval EFI_OUT_OF_RESOURCES    No enough resource to process capsules.

**/
EFI_STATUS
EFIAPI
ProcessCapsules (
  VOID
  )
{
  return EFI_UNSUPPORTED;
}

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
  )
{
  return FALSE;
}

/**
  Get the maximum capsule size supported in the update capsule partition.
  
  @param[out] MaxiumCapsuleSize Maximum capsule size supported by the current
                                capsule storage type.
   
  @retval EFI_SUCESS            Action completed successfully
  @retval EFI_INVALID_PARAMETER Invalid input parameter.

**/
EFI_STATUS
EFIAPI
GetMaxCapsuleSize (
  OUT UINT64             *MaxiumCapsuleSize
  )
{
  return EFI_UNSUPPORTED;
}

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
  )
{
  return EFI_UNSUPPORTED;
}

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
  )
{
  return EFI_UNSUPPORTED;
}

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
  )
{
  return EFI_UNSUPPORTED;
}

/**
  The firmware-specific implementation processes the capsule image
  if it recognized the format of this capsule image.
  
  @param  CapsuleHeader    Pointer to the UEFI capsule image to be processed. 
  @param  CapsuleOffset    Offset of the capsule in persistent storage, 0 if
                           capsule didn't come from storage
  @param  CapsuleReport    TRUE - Create a report, FALSE - Do not create report
  @param  CapsuleFileName  In case of capsule loaded from disk, the zero-
                           terminated array containing file name of capsule
                           that was processed
  @param  CapsuleTarget    This field will contain a zero-terminated string
                           containing the text representation of the device
                           path of device publishing Firmware Management Protocol
                           (if present)
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
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Formalize Bds global variables.

 1. For OsIndicationsSupported, Create a BS/RT/UINT64 variable to report caps
 2. Delete OsIndications variable if it is not NV/BS/RT UINT64
 Item 3 is used to solve case when OS corrupts OsIndications. Here simply delete this NV variable.

**/
VOID
BdsFormalizeEfiGlobalVariable (
  VOID
  )
{
}


