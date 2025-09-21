/** @file
  Capsule Runtime Driver produces two UEFI capsule runtime services.
  (UpdateCapsule, QueryCapsuleCapabilities)
  It installs the Capsule Architectural Protocol defined in PI1.0a to signify 
  the capsule runtime services are ready.

Copyright (c) 2012-2017, Qualcomm Technologies, Inc. All Rights Reserved.
Qualcomm Technologies Proprietary and Confidential.
Portions Copyright (c) 2006 - 2015, Intel Corporation. All rights reserved.<BR>
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
 03/24/17   gparam  Correction in the code to validate if the capsules will fit
                    in to storage media used for persisting the same +
                    Moved the storage pointer initialization to runtime APIs
                    instead of driver initialization
 02/22/17   gparam  Initial revision.

=============================================================================*/
#include <Uefi.h>

#include <Protocol/Capsule.h>
#include <Guid/CapsuleVendor.h>
#include <Guid/FmpCapsule.h>

#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/CapsuleLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiRuntimeLib.h>
#include <Library/BaseLib.h>
#include <Library/PrintLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/QcomCapsuleHeader.h>
//
// Handle for the installation of Capsule Architecture Protocol.
//
EFI_HANDLE     mNewHandle = NULL;

UINT32         mMaxSizeNonPopulateCapsule  = 0;

STATIC BOOLEAN mStoragePointersInitialized = FALSE;

/**
  Passes capsules to the firmware with both virtual and physical mapping. Depending on the intended
  consumption, the firmware may process the capsule immediately. If the payload should persist
  across a system reset, the reset value returned from EFI_QueryCapsuleCapabilities must
  be passed into ResetSystem() and will cause the capsule to be processed by the firmware as
  part of the reset process.

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
  @retval EFI_INVALID_PARAMETER CapsuleSize is NULL, or an incompatible set of flags were
                                set in the capsule header.
  @retval EFI_INVALID_PARAMETER CapsuleCount is Zero.
  @retval EFI_INVALID_PARAMETER For across reset capsule image, ScatterGatherList is NULL.
  @retval EFI_UNSUPPORTED       CapsuleImage is not recognized by the firmware.
  @retval EFI_OUT_OF_RESOURCES  When ExitBootServices() has been previously called this error indicates the capsule 
                                is compatible with this platform but is not capable of being submitted or processed 
                                in runtime. The caller may resubmit the capsule prior to ExitBootServices().
  @retval EFI_OUT_OF_RESOURCES  When ExitBootServices() has not been previously called then this error indicates 
                                the capsule is compatible with this platform but there are insufficient resources to process.

**/
EFI_STATUS
EFIAPI
UpdateCapsule (
  IN EFI_CAPSULE_HEADER      **CapsuleHeaderArray,
  IN UINTN                   CapsuleCount,
  IN EFI_PHYSICAL_ADDRESS    ScatterGatherList OPTIONAL
  )
{
  UINTN                     ArrayNumber = 0;
  EFI_STATUS                Status = EFI_SUCCESS;
  EFI_CAPSULE_HEADER        *CapsuleHeader = NULL;
  BOOLEAN                   NeedReset = FALSE;
  BOOLEAN                   InitiateReset = FALSE;
  CapsuleStorageType        CapsuleStorage = CAPSULE_STORAGE_TYPE_UNKNOWN;

  //
  // Capsule Count can't be less than one.
  //
  if (CapsuleCount < 1) {
    return EFI_INVALID_PARAMETER;
  }

  CapsuleStorage = (CapsuleStorageType)PcdGet16(PcdCapsuleStorageType);

  if (FALSE == mStoragePointersInitialized) {
    Status = InitCapsuleStoragePointers (CapsuleStorage);
    
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "InitCapsuleStoragePointers returned - %r!\n", Status));
      return Status;
    }
    mStoragePointersInitialized = TRUE;
  }

  for (ArrayNumber = 0; ArrayNumber < CapsuleCount; ArrayNumber++) {
    //
    // A capsule which has the CAPSULE_FLAGS_POPULATE_SYSTEM_TABLE flag must have
    // CAPSULE_FLAGS_PERSIST_ACROSS_RESET set in its header as well.
    //
    CapsuleHeader = CapsuleHeaderArray[ArrayNumber];
    if ((CapsuleHeader->Flags & (CAPSULE_FLAGS_PERSIST_ACROSS_RESET | CAPSULE_FLAGS_POPULATE_SYSTEM_TABLE)) == CAPSULE_FLAGS_POPULATE_SYSTEM_TABLE) {
      return EFI_INVALID_PARAMETER;
    }
    //
    // A capsule which has the CAPSULE_FLAGS_INITIATE_RESET flag must have
    // CAPSULE_FLAGS_PERSIST_ACROSS_RESET set in its header as well.
    //
    if ((CapsuleHeader->Flags & (CAPSULE_FLAGS_PERSIST_ACROSS_RESET | CAPSULE_FLAGS_INITIATE_RESET)) == CAPSULE_FLAGS_INITIATE_RESET) {
      return EFI_INVALID_PARAMETER;
    }

    //
    // Check FMP capsule flag 
    //
    if (CompareGuid(&CapsuleHeader->CapsuleGuid, &gEfiFmpCapsuleGuid)
     && (CapsuleHeader->Flags & CAPSULE_FLAGS_POPULATE_SYSTEM_TABLE) != 0 ) {
       return EFI_INVALID_PARAMETER;
    }

    //
    // Check Capsule image without populate flag by firmware support capsule function  
    //
    if ((CapsuleHeader->Flags & CAPSULE_FLAGS_POPULATE_SYSTEM_TABLE) == 0) {
      Status = SupportCapsuleImage (CapsuleHeader);
      if (EFI_ERROR(Status)) {
        return Status;
      }
    }
  }

  //
  // Walk through all capsules, record whether there is a capsule needs reset
  // or initiate reset. And then process capsules which has no reset flag directly.
  //
  for (ArrayNumber = 0; ArrayNumber < CapsuleCount ; ArrayNumber++) {
    CapsuleHeader = CapsuleHeaderArray[ArrayNumber];
    //
    // Here should be in the boot-time for non-reset capsule image
    // Platform specific update for the non-reset capsule image.
    //
    if ((CapsuleHeader->Flags & CAPSULE_FLAGS_PERSIST_ACROSS_RESET) == 0) {
      if (EfiAtRuntime ()) { 
        // At runtime, processing of capsules that don't have PERSIST_ACROSS_RESET
        // is not supported
        Status = EFI_OUT_OF_RESOURCES;
      } else {
        // Processing of capsules that don't have CAPSULE_FLAGS_PERSIST_ACROSS_RESET
        // is only supported at boot time - currently do nothing
        Status = ProcessCapsuleImageEx(CapsuleHeader, 0, NULL, NULL);
      }
      if (EFI_ERROR(Status)) {
        return Status;
      }
    } else {
      NeedReset = TRUE;
      if ((CapsuleHeader->Flags & CAPSULE_FLAGS_INITIATE_RESET) != 0) {
        InitiateReset = TRUE;
      }
    }
  }
  
  //
  // After launching all capsules who has no reset flag, if no more capsules claims
  // for a system reset just return.
  //
  if (!NeedReset) {
    return EFI_SUCCESS;
  }

  //
  // ScatterGatherList is only referenced if the capsules are defined to persist across
  // system reset. 
  //
  if (ScatterGatherList == (EFI_PHYSICAL_ADDRESS) (UINTN) NULL) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Check if the platform supports update capsule across a system reset
  //
  if (!FeaturePcdGet(PcdSupportUpdateCapsuleReset)) {
    return EFI_UNSUPPORTED;
  }

  Status = PersistCapsules(CapsuleHeaderArray,
                           CapsuleCount,
                           ScatterGatherList);
  if(EFI_ERROR(Status))
  {
    return Status;
  }

  if(InitiateReset) {
    //
    // Firmware that encounters a capsule which has the CAPSULE_FLAGS_INITIATE_RESET Flag set in its header
    // will initiate a reset of the platform which is compatible with the passed-in capsule request and will 
    // not return back to the caller.
    //
    EfiResetSystem (EfiResetWarm, EFI_SUCCESS, 0, NULL);
  }

  return Status;
}

/**
  Returns if the capsule can be supported via UpdateCapsule().

  @param  CapsuleHeaderArray    Virtual pointer to an array of virtual pointers to the capsules
                                being passed into update capsule.
  @param  CapsuleCount          Number of pointers to EFI_CAPSULE_HEADER in
                                CaspuleHeaderArray.
  @param  MaxiumCapsuleSize     On output the maximum size that UpdateCapsule() can
                                support as an argument to UpdateCapsule() via
                                CapsuleHeaderArray and ScatterGatherList.
  @param  ResetType             Returns the type of reset required for the capsule update.

  @retval EFI_SUCCESS           Valid answer returned.
  @retval EFI_UNSUPPORTED       The capsule image is not supported on this platform, and
                                MaximumCapsuleSize and ResetType are undefined.
  @retval EFI_INVALID_PARAMETER MaximumCapsuleSize is NULL, or ResetTyep is NULL,
                                Or CapsuleCount is Zero, or CapsuleImage is not valid.

**/
EFI_STATUS
EFIAPI
QueryCapsuleCapabilities (
  IN  EFI_CAPSULE_HEADER  **CapsuleHeaderArray,
  IN  UINTN                 CapsuleCount,
  OUT UINT64               *MaxiumCapsuleSize,
  OUT EFI_RESET_TYPE       *ResetType
  )
{
  EFI_STATUS              Status = EFI_SUCCESS;
  UINTN                   ArrayNumber;
  EFI_CAPSULE_HEADER     *CapsuleHeader;
  BOOLEAN                 NeedReset;
  UINTN                   NeededSize = 0;
  UINT32                  CapsuleSizeInStorageMedia = 0;
  CapsuleStorageType      CapsuleStorage = CAPSULE_STORAGE_TYPE_UNKNOWN;
  BOOLEAN                 StorageMediaChecked = FALSE;

  //
  // Capsule Count can't be less than one.
  //
  if (CapsuleCount < 1) {
    return EFI_INVALID_PARAMETER;
  }
  
  //
  // Check whether input parameter is valid
  //
  if ((MaxiumCapsuleSize == NULL) ||(ResetType == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  CapsuleStorage = (CapsuleStorageType)PcdGet16(PcdCapsuleStorageType);

  CapsuleHeader = NULL;
  NeedReset     = FALSE;

  if (FALSE == mStoragePointersInitialized) {
    Status = InitCapsuleStoragePointers (CapsuleStorage);
    
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "InitCapsuleStoragePointers returned - %r!\n", Status));
      return Status;
    }
    mStoragePointersInitialized = TRUE;
  }

  for (ArrayNumber = 0; ArrayNumber < CapsuleCount; ArrayNumber++) {
    CapsuleHeader = CapsuleHeaderArray[ArrayNumber];
    //
    // A capsule which has the CAPSULE_FLAGS_POPULATE_SYSTEM_TABLE flag must have
    // CAPSULE_FLAGS_PERSIST_ACROSS_RESET set in its header as well.
    //
    if ((CapsuleHeader->Flags & (CAPSULE_FLAGS_PERSIST_ACROSS_RESET | CAPSULE_FLAGS_POPULATE_SYSTEM_TABLE)) == CAPSULE_FLAGS_POPULATE_SYSTEM_TABLE) {
      return EFI_INVALID_PARAMETER;
    }
    //
    // A capsule which has the CAPSULE_FLAGS_INITIATE_RESET flag must have
    // CAPSULE_FLAGS_PERSIST_ACROSS_RESET set in its header as well.
    //
    if ((CapsuleHeader->Flags & (CAPSULE_FLAGS_PERSIST_ACROSS_RESET | CAPSULE_FLAGS_INITIATE_RESET)) == CAPSULE_FLAGS_INITIATE_RESET) {
      return EFI_INVALID_PARAMETER;
    }

    //
    // Check FMP capsule flag 
    //
    if (CompareGuid(&CapsuleHeader->CapsuleGuid, &gEfiFmpCapsuleGuid)
     && (CapsuleHeader->Flags & CAPSULE_FLAGS_POPULATE_SYSTEM_TABLE) != 0 ) {
       return EFI_INVALID_PARAMETER;
    }

    //
    // Check Capsule image without populate flag is supported by firmware
    //
    if ((CapsuleHeader->Flags & CAPSULE_FLAGS_POPULATE_SYSTEM_TABLE) == 0) {
      Status = SupportCapsuleImage (CapsuleHeader);
      if (EFI_ERROR(Status)) {
        return Status;
      }
    }

    //
    // Calculate the total memory needed to store all the capsules that has
    // CAPSULE_FLAGS_PERSIST_ACROSS_RESET flag
    //
    if ((CapsuleHeader->Flags & CAPSULE_FLAGS_PERSIST_ACROSS_RESET) != 0) {
      // If we havent checked for the availablity of media to persist
      // capsules check it now
      if (FALSE == StorageMediaChecked) {
        StorageMediaChecked = TRUE;
        
        if (!CapsulePersistedMediaExists(CapsuleStorage)) {
          return EFI_OUT_OF_RESOURCES;
        }
      }

      Status = GetCapsuleSizeInStorageMedia (
        CapsuleHeader->CapsuleImageSize,
        &CapsuleSizeInStorageMedia
        );
      if (EFI_ERROR(Status)) {
        return Status;
      }

      NeededSize += CapsuleSizeInStorageMedia;
    }
  }

  //
  // Find out whether there is any capsule defined to persist across system reset. 
  //
  for (ArrayNumber = 0; ArrayNumber < CapsuleCount ; ArrayNumber++) {
    CapsuleHeader = CapsuleHeaderArray[ArrayNumber];
    if ((CapsuleHeader->Flags & CAPSULE_FLAGS_PERSIST_ACROSS_RESET) != 0) {
      NeedReset = TRUE;
      break;
    }
  }

  if (NeedReset) {
    //
    //Check if the platform supports update capsule across a system reset
    //
    if (!FeaturePcdGet(PcdSupportUpdateCapsuleReset)) {
      return EFI_UNSUPPORTED;
    }

    *ResetType = EfiResetWarm;

    Status = GetMaxCapsuleSize (MaxiumCapsuleSize);
    if (EFI_ERROR(Status)) {
      return Status;
    }

    //
    //Check if the the current capsules fit into the memory available for
    //persisting the capsules
    //
    if (NeededSize > *MaxiumCapsuleSize) {
      return EFI_OUT_OF_RESOURCES;
    }
  } else {
    //
    // For non-reset capsule image.
    //
    *ResetType = EfiResetCold;
    *MaxiumCapsuleSize = (UINT64) mMaxSizeNonPopulateCapsule;
  }

  return Status;
}

/**

  This code installs UEFI capsule runtime service.

  @param  ImageHandle    The firmware allocated handle for the EFI image.  
  @param  SystemTable    A pointer to the EFI System Table.

  @retval EFI_SUCCESS    UEFI Capsule Runtime Services are installed successfully. 

**/
EFI_STATUS
EFIAPI
CapsuleServiceInitialize (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS  Status = EFI_SUCCESS;

  mMaxSizeNonPopulateCapsule = PcdGet32(PcdMaxSizeNonPopulateCapsule);

  //
  // Install capsule runtime services into UEFI runtime service tables.
  //
  gRT->UpdateCapsule                    = UpdateCapsule;
  gRT->QueryCapsuleCapabilities         = QueryCapsuleCapabilities;

  //
  // Install the Capsule Architectural Protocol on a new handle
  // to signify the capsule runtime services are ready.
  //
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &mNewHandle,
                  &gEfiCapsuleArchProtocolGuid,
                  NULL,
                  NULL
                  );
  ASSERT_EFI_ERROR (Status);

  return Status;
}
