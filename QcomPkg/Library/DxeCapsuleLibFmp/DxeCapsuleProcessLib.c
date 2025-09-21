/** @file
  DXE capsule process.

  Caution: This module requires additional review when modified.
  This module will have external input - capsule image.
  This external input must be validated carefully to avoid security issue like
  buffer overflow, integer overflow.

  ProcessCapsules(), ProcessTheseCapsules() will receive untrusted
  input and do basic validation.

  Copyright (c) 2017-2018, Qualcomm Technologies, Inc. All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential.
  Portions Copyright (c) 2016, Intel Corporation. All rights reserved.<BR>
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
 02/12/18   prema   Change to skip nested capsule header while deriving FMP header
 02/12/18   prema   During CapsuleUpdate, when no CAP found and EFI_NO_MEDIA return,
                    clear OsIndicationSupport NV variable then do reset if needed
 02/01/18   prema   Cleared valid OsIndicationSupport NV variable when all Capsule 
                    images are processed 
 08/24/17   gparam  Change to flush UEFI logs to LogFs partition just before rebooting
                    device after capsule update.
 05/08/17   gparam  Change to handle the new error code that indicate no capsules
                    are found in mass-storage media + Changes to centralize ESRT
                    update to DPP to the end of SetImage API + Minor tweaks to logging
 04/21/17   gparam  Check for battery level and AC power before applying each capsule
 04/18/17   gparam  Fix to flush capsule storage in all possible paths +
                    Moved the de-init of capsule pointers to a generic place
                    so that all code paths use this
 03/24/17   gparam  Changes to cache mass-storage handles for use repeated in
                    repeated query + Change to flush capsule storage medium before
                    attempting a reboot after capsule processing
 03/15/17   gparam  Moved the ESRT sync and lock function to ProcessCapsules
                    to avoid creating stubs for use in LA
 02/22/17   gparam  Initial revision

=============================================================================*/

#include <PiDxe.h>
#include <Protocol/EsrtManagement.h>

#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiLib.h>
#include <Library/PcdLib.h>
#include <Library/HobLib.h>
#include <Library/GenericBdsLib.h>
#include <Library/ReportStatusCodeLib.h>
#include <Library/CapsuleLib.h>
#include <Library/FwCommonLib.h>
#include <Library/FwUpdateLib/FwPowerCheck.h>
#include <Library/SerialPortShLib.h>

#include <IndustryStandard/WindowsUxCapsule.h>

#include <Guid/GlobalVariable.h>
#include <Guid/FmpCapsule.h>

#include "DxeCapsuleQcom.h"
#include "DxeCapsulePartition.h"
#include "DxeCapsuleRawFile.h"

extern VOID                     *CapsuleStorageHeader;
extern BOOLEAN                   mDxeCapsuleLibEndOfDxe;
extern UINTN                     mNumMassStoragePartitionHandles;
extern EFI_HANDLE               *mMassStoragePartitionHandleBuffer;


BOOLEAN                          mNeedReset = FALSE;

VOID                           **mCapsulePtr         = NULL;
UINT32                          *mCapsuleOffsetArray = NULL;
EFI_STATUS                      *mCapsuleStatusArray = NULL;
CHAR16                         **mCapsuleFileNameArray = NULL;
UINT32                           mCapsuleTotalNumber = 0;

EFI_STATUS EFIAPI WriteLogBufToPartition (VOID);

/**
  Return if this FMP is a system FMP or a device FMP, based upon CapsuleHeader.

  @param[in] CapsuleHeader A pointer to EFI_CAPSULE_HEADER

  @retval TRUE  It is a system FMP.
  @retval FALSE It is a device FMP.
**/
BOOLEAN
IsFmpCapsule (
  IN EFI_CAPSULE_HEADER  *CapsuleHeader
  );

/**
  Validate Fmp capsules layout.

  Caution: This function may receive untrusted input.

  This function assumes the caller validated the capsule by using
  IsValidCapsuleHeader(), so that all fields in EFI_CAPSULE_HEADER are correct.
  The capsule buffer size is CapsuleHeader->CapsuleImageSize.

  This function validates the fields in EFI_FIRMWARE_MANAGEMENT_CAPSULE_HEADER
  and EFI_FIRMWARE_MANAGEMENT_CAPSULE_IMAGE_HEADER.

  This function need support nested FMP capsule.

  @param[in]   CapsuleHeader        Points to a capsule header.
  @param[out]  EmbeddedDriverCount  The EmbeddedDriverCount in the FMP capsule.

  @retval EFI_SUCESS             Input capsule is a correct FMP capsule.
  @retval EFI_INVALID_PARAMETER  Input capsule is not a correct FMP capsule.
**/
EFI_STATUS
ValidateFmpCapsule (
  IN EFI_CAPSULE_HEADER  *CapsuleHeader,
  OUT UINT16             *EmbeddedDriverCount OPTIONAL
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

/**
  Return if there is a FMP header below capsule header.

  @param[in] CapsuleHeader A pointer to EFI_CAPSULE_HEADER

  @retval TRUE  There is a FMP header below capsule header.
  @retval FALSE There is not a FMP header below capsule header
**/
BOOLEAN
IsNestedFmpCapsule (
  IN EFI_CAPSULE_HEADER         *CapsuleHeader
  );

/**
  This function initializes the mCapsulePtr, mCapsuleStatusArray,
  mCapsuleStatusArray, mCapsuleFileNameArray and mCapsuleTotalNumber
  from the Hobs created in PEI stage out of the capsule info in scattergather list

  @retval EFI_SUCESS            Action completed successfully
  @retval EFI_INVALID_PARAMETER Invalid input parameter.
  @retval EFI_END_OF_MEDIA      No persisted capsules found.

**/
EFI_STATUS
InitCapsulePtrFromHob (
  VOID
  )
{
  EFI_STATUS                  Status = EFI_SUCCESS;
  EFI_PEI_HOB_POINTERS        HobPointer;
  UINTN                       Index = 0;

  //
  // Find all capsule images from hob
  //
  HobPointer.Raw = GetHobList ();
  while ((HobPointer.Raw = GetNextHob (EFI_HOB_TYPE_UEFI_CAPSULE, HobPointer.Raw)) != NULL) {
    if (!IsValidCapsuleHeader((VOID *)(UINTN)HobPointer.Capsule->BaseAddress, HobPointer.Capsule->Length)) {
      HobPointer.Header->HobType = EFI_HOB_TYPE_UNUSED; // Mark this hob as invalid
    } else {
      mCapsuleTotalNumber++;
    }
    HobPointer.Raw = GET_NEXT_HOB (HobPointer);
  }

  DEBUG ((DEBUG_INFO, "mCapsuleTotalNumber - 0x%x\n", mCapsuleTotalNumber));

  if (mCapsuleTotalNumber == 0) {
    return EFI_END_OF_MEDIA;
  }

  //
  // Init temp Capsule Data table.
  //
  mCapsulePtr = (VOID **) AllocateZeroPool (sizeof (VOID *) * mCapsuleTotalNumber);
  if (mCapsulePtr == NULL) {
    DEBUG ((DEBUG_ERROR, "Allocate mCapsulePtr fail!\n"));
    mCapsuleTotalNumber = 0;
    return EFI_OUT_OF_RESOURCES;
  }
  mCapsuleOffsetArray = (UINT32 *) AllocateZeroPool (sizeof (UINT32) * mCapsuleTotalNumber);
  if (mCapsuleOffsetArray == NULL) {
    DEBUG ((DEBUG_ERROR, "Allocate mCapsuleOffsetArray fail!\n"));
    FreePool (mCapsulePtr);
    mCapsulePtr = NULL;
    mCapsuleTotalNumber = 0;
    return EFI_OUT_OF_RESOURCES;
  }
  mCapsuleStatusArray = (EFI_STATUS *) AllocateZeroPool (sizeof (EFI_STATUS) * mCapsuleTotalNumber);
  if (mCapsuleStatusArray == NULL) {
    DEBUG ((DEBUG_ERROR, "Allocate mCapsuleStatusArray fail!\n"));
    FreePool (mCapsulePtr);
    mCapsulePtr = NULL;
    FreePool (mCapsuleOffsetArray);
    mCapsuleOffsetArray = NULL;
    mCapsuleTotalNumber = 0;
    return EFI_OUT_OF_RESOURCES;
  }
  SetMemN (mCapsuleStatusArray, sizeof (EFI_STATUS) * mCapsuleTotalNumber, EFI_NOT_READY);

  mCapsuleFileNameArray = (CHAR16 **) AllocateZeroPool (sizeof (CHAR16 *) * mCapsuleTotalNumber);
  if (mCapsuleFileNameArray == NULL) {
    DEBUG ((DEBUG_ERROR, "Allocate mCapsuleFileNameArray fail!\n"));
    FreePool (mCapsulePtr);
    mCapsulePtr = NULL;
    FreePool (mCapsuleOffsetArray);
    mCapsuleOffsetArray = NULL;
    FreePool (mCapsuleStatusArray);
    mCapsuleStatusArray = NULL;
    mCapsuleTotalNumber = 0;
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // Find all capsule images from hob
  //
  HobPointer.Raw = GetHobList ();
  Index = 0;
  while ((HobPointer.Raw = GetNextHob (EFI_HOB_TYPE_UEFI_CAPSULE, HobPointer.Raw)) != NULL) {
    mCapsulePtr [Index++] = (VOID *) (UINTN) HobPointer.Capsule->BaseAddress;
    HobPointer.Raw = GET_NEXT_HOB (HobPointer);
  }

  return Status;
}

/**
  This function returns if all capsule images are processed.

  @retval TRUE   All capsule images are processed.
  @retval FALSE  Not all capsule images are processed.
**/
BOOLEAN
AreAllImagesProcessed (
  VOID
  )
{
  UINTN  Index;

  for (Index = 0; Index < mCapsuleTotalNumber; Index++) {
    if (mCapsuleStatusArray[Index] == EFI_NOT_READY) {
      return FALSE;
    }
  }

  return TRUE;
}

/**
  This function returns if the processing of capsule with flag
  CAPSULE_FLAGS_POPULATE_SYSTEM_TABLE has to be delayed till next boot.

  This is needed when we are persisting capsules media other than RAM.
  For medias other than RAM, the processing of FMP capsules will result
  in system reboot and the capsules installed in system configuration table
  are not guaranteed to be persisted across this reboot as they would have
  been lying in RAM when they are installed in system configuratoin table.
  For this reason, we do no want to process these type of capsules unless
  all FMP capsules are processed and only these type of capsules are left to
  be processed. In such a boot, there wont be any need of device reboot after
  the capsules with CAPSULE_FLAGS_POPULATE_SYSTEM_TABLE are processed and
  thus these capsules stored in RAM will stay safe for OS to use in later
  stages of the boot.

  @retval TRUE   All capsule images are processed.
  @retval FALSE  Not all capsule images are processed.
**/
BOOLEAN
DelaySystemTableCapsulePopulation (
  VOID
  )
{
  EFI_CAPSULE_HEADER          *CapsuleHeader = NULL;
  UINTN                        Index = 0;
  BOOLEAN                      PopulateSystemTable = FALSE;
  BOOLEAN                      NonSystemTableCapsuleFound = FALSE;

  for (Index = 0; Index < mCapsuleTotalNumber; Index++) {
    CapsuleHeader = (EFI_CAPSULE_HEADER*) mCapsulePtr [Index];
    if ((CapsuleHeader->Flags & CAPSULE_FLAGS_POPULATE_SYSTEM_TABLE) != 0) {
      PopulateSystemTable = TRUE;
    } else {
      NonSystemTableCapsuleFound = TRUE;
    }
  }

  // If we find capsules with and without CAPSULE_FLAGS_POPULATE_SYSTEM_TABLE flag,
  // delay the processing of capsules with flag CAPSULE_FLAGS_POPULATE_SYSTEM_TABLE
  if ((TRUE == PopulateSystemTable) &&
      (TRUE == NonSystemTableCapsuleFound)) {
    return TRUE;
  } else {
    return FALSE;
  }
}

/**
  This function initializes the mCapsulePtr, mCapsuleOffsetArray,
  mCapsuleStatusArray, mCapsuleFileNameArray  and mCapsuleTotalNumber.

  @param[in] CapsuleStorage       Type of storage to process capsule from.

  @retval EFI_SUCCESS             There is no error when processing capsules.
  @retval EFI_OUT_OF_RESOURCES    No enough resource to process capsules.
**/
EFI_STATUS
InitCapsulePtr (
  IN CapsuleStorageType          CapsuleStorage
  )
{
  EFI_STATUS                  Status = EFI_SUCCESS;

  switch (CapsuleStorage)
  {
    case CAPSULE_STORAGE_TYPE_CAPSULE_RAW_FILE:
    case CAPSULE_STORAGE_TYPE_CAPSULE_PARTITION:
    case CAPSULE_STORAGE_TYPE_MASS_STORAGE:
    {
      Status = InitCapsulePtrFromPersistedMedia(CapsuleStorage);

      if ((EFI_END_OF_MEDIA != Status) &&
          (EFI_NO_MEDIA != Status) &&
          EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "InitCapsulePtrFromPersistedMedia returned - %r!\n", Status));
      }
      break;
    }

    case CAPSULE_STORAGE_TYPE_RAM:
    {
      Status = InitCapsulePtrFromHob();

      if ((EFI_END_OF_MEDIA != Status) &&
          (EFI_NO_MEDIA != Status) &&
          EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "InitCapsulePtrFromHob returned - %r!\n", Status));
      }
      break;
    }

    default:
    {
      DEBUG ((DEBUG_ERROR, "Invalid CapsuleStorageType (%d) received for initializing capsule pointer!\n", CapsuleStorage));
      Status = EFI_INVALID_PARAMETER;
    }
  }

  // The below errors are ignorable. The check for mCapsuleTotalNumber in the
  // caller of this function will do the validation of the number of capsules
  // found an log the status and end the capsule processing
  if (EFI_NO_MEDIA == Status) {
    DEBUG ((DEBUG_WARN, "Current capsule storage medium not found!\n"));
    Status = EFI_SUCCESS;
  } else if (EFI_END_OF_MEDIA == Status) {
    // Keeping this log as INFO type as the same message is getting printed
    // at the caller of this function as well
    DEBUG ((DEBUG_INFO, "Current capsule storage medium does not contain any capsules to process!\n"));
    Status = EFI_SUCCESS;
  }

  return Status;
}

/**
  This function de-initializes mCapsulePtr, mCapsuleOffsetArray,
  mCapsuleStatusArray, mCapsuleFileNameArray and mCapsuleTotalNumber.

  @param[in] CapsuleStorage       Type of storage to process capsule from.

  @retval EFI_SUCCESS             There is no error when processing capsules.
  @retval EFI_OUT_OF_RESOURCES    No enough resource to process capsules.
**/
VOID
DeInitCapsulePtr (
  IN CapsuleStorageType          CapsuleStorage
  )
{
  UINT32            Index = 0;

  for (Index = 0; Index < mCapsuleTotalNumber; Index++) {
    if (NULL != mCapsulePtr) {
      /* Individual elements of this array are freed just after processing
         the capsules. If some error encountered earlier than processing the
         capsule resulted in exiting the whole capsule processing, we will
         try to de-init the memory here as well.

         We do not process the error as there is no logical action to take
         when we encounter an error */
      if ((NULL != mCapsulePtr [Index])) {
        DeInitCapsulePtrMemory (
          &(mCapsulePtr [Index]),
          CapsuleStorage
          );
        mCapsulePtr [Index] = NULL;
      }
    }

    if (NULL != mCapsuleFileNameArray) {
      if ((NULL != mCapsuleFileNameArray [Index])) {
        FreePool (mCapsuleFileNameArray[Index]);
        mCapsuleFileNameArray[Index] = NULL;
      }
    }
  }

  if (NULL != mCapsulePtr) {
    FreePool (mCapsulePtr);
    mCapsulePtr = NULL;
  }

  if (NULL != mCapsuleFileNameArray) {
    FreePool (mCapsuleFileNameArray);
    mCapsuleFileNameArray = NULL;
  }

  if (NULL != mCapsuleOffsetArray) {
    FreePool (mCapsuleOffsetArray);
    mCapsuleOffsetArray = NULL;
  }

  if (NULL != mCapsuleStatusArray) {
    FreePool (mCapsuleStatusArray);
    mCapsuleStatusArray = NULL;
  }

  mCapsuleTotalNumber = 0;

  if (NULL != mMassStoragePartitionHandleBuffer) {
    FreePool (mMassStoragePartitionHandleBuffer);
    mMassStoragePartitionHandleBuffer = NULL;
  }
  mNumMassStoragePartitionHandles = UINTN_MAX_VALUE;
}

/**
  This function populates capsule in the configuration table.
**/
VOID
PopulateCapsuleInConfigurationTable (
  VOID
  )
{
  VOID                        **CapsulePtrCache;
  EFI_GUID                    *CapsuleGuidCache;
  EFI_CAPSULE_HEADER          *CapsuleHeader;
  EFI_CAPSULE_TABLE           *CapsuleTable;
  UINT32                      CacheIndex;
  UINT32                      CacheNumber;
  UINT32                      CapsuleNumber;
  UINTN                       Index;
  UINTN                       Size;
  EFI_STATUS                  Status;

  if (mCapsuleTotalNumber == 0) {
    return ;
  }

  CapsulePtrCache     = NULL;
  CapsuleGuidCache    = NULL;
  CacheIndex          = 0;
  CacheNumber         = 0;

  CapsulePtrCache  = (VOID **) AllocateZeroPool (sizeof (VOID *) * mCapsuleTotalNumber);
  if (CapsulePtrCache == NULL) {
    DEBUG ((DEBUG_ERROR, "Allocate CapsulePtrCache fail!\n"));
    return ;
  }
  CapsuleGuidCache = (EFI_GUID *) AllocateZeroPool (sizeof (EFI_GUID) * mCapsuleTotalNumber);
  if (CapsuleGuidCache == NULL) {
    DEBUG ((DEBUG_ERROR, "Allocate CapsuleGuidCache fail!\n"));
    FreePool (CapsulePtrCache);
    return ;
  }

  //
  // Capsules who have CAPSULE_FLAGS_POPULATE_SYSTEM_TABLE always are used for operating
  // System to have information persist across a system reset. EFI System Table must
  // point to an array of capsules that contains the same CapsuleGuid value. And agents
  // searching for this type capsule will look in EFI System Table and search for the
  // capsule's Guid and associated pointer to retrieve the data. Two steps below describes
  // how to sorting the capsules by the unique guid and install the array to EFI System Table.
  // Firstly, Loop for all coalesced capsules, record unique CapsuleGuids and cache them in an
  // array for later sorting capsules by CapsuleGuid.
  //
  for (Index = 0; Index < mCapsuleTotalNumber; Index++) {
    CapsuleHeader = (EFI_CAPSULE_HEADER*) mCapsulePtr [Index];
    if ((CapsuleHeader->Flags & CAPSULE_FLAGS_POPULATE_SYSTEM_TABLE) != 0) {
      //
      // For each capsule, we compare it with known CapsuleGuid in the CacheArray.
      // If already has the Guid, skip it. Whereas, record it in the CacheArray as
      // an additional one.
      //
      CacheIndex = 0;
      while (CacheIndex < CacheNumber) {
        if (CompareGuid(&CapsuleGuidCache[CacheIndex],&CapsuleHeader->CapsuleGuid)) {
          break;
        }
        CacheIndex++;
      }
      if (CacheIndex == CacheNumber) {
        CopyMem(&CapsuleGuidCache[CacheNumber++],&CapsuleHeader->CapsuleGuid,sizeof(EFI_GUID));
      }
    }
  }

  //
  // Secondly, for each unique CapsuleGuid in CacheArray, gather all coalesced capsules
  // whose guid is the same as it, and malloc memory for an array which preceding
  // with UINT32. The array fills with entry point of capsules that have the same
  // CapsuleGuid, and UINT32 represents the size of the array of capsules. Then install
  // this array into EFI System Table, so that agents searching for this type capsule
  // will look in EFI System Table and search for the capsule's Guid and associated
  // pointer to retrieve the data.
  //
  for (CacheIndex = 0; CacheIndex < CacheNumber; CacheIndex++) {
    CapsuleNumber = 0;
    for (Index = 0; Index < mCapsuleTotalNumber; Index++) {
      CapsuleHeader = (EFI_CAPSULE_HEADER*) mCapsulePtr [Index];
      if ((CapsuleHeader->Flags & CAPSULE_FLAGS_POPULATE_SYSTEM_TABLE) != 0) {
        if (CompareGuid (&CapsuleGuidCache[CacheIndex], &CapsuleHeader->CapsuleGuid)) {
          //
          // Cache Caspuleheader to the array, this array is uniqued with certain CapsuleGuid.
          //
          CapsulePtrCache[CapsuleNumber++] = (VOID*)CapsuleHeader;
        }
      }
    }
    if (CapsuleNumber != 0) {
      Size = sizeof(EFI_CAPSULE_TABLE) + (CapsuleNumber - 1) * sizeof(VOID*);
      CapsuleTable = AllocateRuntimePool (Size);
      if (CapsuleTable == NULL) {
        DEBUG ((DEBUG_ERROR, "Allocate CapsuleTable (%g) fail!\n", &CapsuleGuidCache[CacheIndex]));
        continue;
      }
      CapsuleTable->CapsuleArrayNumber =  CapsuleNumber;
      CopyMem(&CapsuleTable->CapsulePtr[0], CapsulePtrCache, CapsuleNumber * sizeof(VOID*));
      Status = gBS->InstallConfigurationTable (&CapsuleGuidCache[CacheIndex], (VOID*)CapsuleTable);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "InstallConfigurationTable (%g) fail!\n", &CapsuleGuidCache[CacheIndex]));
      }

      // Search for the matching capsule in the pointer array and update the
      // status as the value received from InstallConfigurationTable
      for (Index = 0; Index < mCapsuleTotalNumber; Index++) {
        CapsuleHeader = (EFI_CAPSULE_HEADER*) mCapsulePtr [Index];
        if ((CapsuleHeader->Flags & CAPSULE_FLAGS_POPULATE_SYSTEM_TABLE) != 0) {
          if (CompareGuid (&CapsuleGuidCache[CacheIndex], &CapsuleHeader->CapsuleGuid)) {
            mCapsuleStatusArray [Index] = Status;
          }
        }
      }
    }
  }

  FreePool(CapsuleGuidCache);
  FreePool(CapsulePtrCache);
}

/**

  This routine is called to process capsules.

  Caution: This function may receive untrusted input.

  Each individual capsule result is recorded in capsule record variable.

  @param [in]  FirstRound         TRUE:  First round. Need skip the FMP capsules with non zero EmbeddedDriverCount.
                                  FALSE: Process rest FMP capsules.
  @param [in]  CapsuleStorage     Type of storage to process capsule from.


  @retval EFI_SUCCESS             There is no error when processing capsules.
  @retval EFI_OUT_OF_RESOURCES    No enough resource to process capsules.
  @retval EFI_END_OF_MEDIA        No persisted capsules found.

**/
EFI_STATUS
ProcessTheseCapsules (
  IN BOOLEAN            FirstRound,
  IN CapsuleStorageType CapsuleStorage
  )
{
  EFI_STATUS                  Status = EFI_SUCCESS, StatusEsrt = EFI_SUCCESS;
  EFI_CAPSULE_HEADER         *CapsuleHeader = NULL;
  EFI_CAPSULE_HEADER         *NestedCapsuleHeader = NULL;
  UINT32                      CapsuleOffset = 0;
  CHAR16                     *CapsuleFileName = NULL;
  UINT32                      Index = 0;
  BOOLEAN                     DisplayCapsuleExist;
  ESRT_MANAGEMENT_PROTOCOL   *EsrtManagement = NULL;
  UINT16                      EmbeddedDriverCount = 0;
  BOOLEAN                     ResetNeeded = FALSE;
  BOOLEAN                     bPowerGood = TRUE;
  UINT32                      MsftAttemptStatus = LAST_ATTEMPT_STATUS_SUCCESS;
  QCFW_UPDATE_FLAG_VALUE      QcResultFlag = QCFW_UPDATE_INIT;
  EFI_SYSTEM_RESOURCE_ENTRY   EsrtEntry;
  EFI_FIRMWARE_MANAGEMENT_CAPSULE_HEADER       *FmpCapsuleHeader = NULL;
  EFI_FIRMWARE_MANAGEMENT_CAPSULE_IMAGE_HEADER *ImageHeader = NULL;
  UINT64                                       *ItemOffsetList = NULL;

  REPORT_STATUS_CODE(EFI_PROGRESS_CODE, (EFI_SOFTWARE | PcdGet32(PcdStatusCodeSubClassCapsule) | PcdGet32(PcdCapsuleStatusCodeProcessCapsulesBegin)));

  if (FirstRound) {

    // Need to initialize these pointers only for these types
    if ((CapsuleStorage == CAPSULE_STORAGE_TYPE_CAPSULE_RAW_FILE) ||
        (CapsuleStorage == CAPSULE_STORAGE_TYPE_CAPSULE_PARTITION)) {
      Status = InitCapsuleStoragePointers (CapsuleStorage);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "InitCapsuleStoragePointers returned - %r.\n", Status));
        return Status;
      }
    }

    Status = InitCapsulePtr (CapsuleStorage);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "InitCapsulePtr returned - %r.\n", Status));
      return Status;
    }
  }

  if (mCapsuleTotalNumber == 0) {
    //
    // We didn't find capsules to process. Report back to the caller so that caller can
    // understand that there is no capsule processing done.
    //
    DEBUG ((DEBUG_ERROR, "No pending capsules found in %s\n", CAPSULESTORAGETYPE_TO_CAPSULESTORAGETYPE_STRING(CapsuleStorage)));
    return EFI_END_OF_MEDIA;
  }

  if (AreAllImagesProcessed ()) {
    return EFI_SUCCESS;
  }

  //
  // Check the capsule flags,if contains CAPSULE_FLAGS_POPULATE_SYSTEM_TABLE, install
  // capsuleTable to configure table with EFI_CAPSULE_GUID
  //
  if (FirstRound) {
    // Check if we need to delay this till next reboot. For more info, check
    // the below function description
    if (FALSE == DelaySystemTableCapsulePopulation()) {
      PopulateCapsuleInConfigurationTable ();
    }
  }

  REPORT_STATUS_CODE(EFI_PROGRESS_CODE, (EFI_SOFTWARE | PcdGet32(PcdStatusCodeSubClassCapsule) | PcdGet32(PcdCapsuleStatusCodeUpdatingFirmware)));

  //
  // If Windows UX capsule exist, process it first
  //
  DisplayCapsuleExist = FALSE;
  for (Index = 0; Index < mCapsuleTotalNumber; Index++) {
    CapsuleHeader = (EFI_CAPSULE_HEADER*) mCapsulePtr [Index];
    CapsuleOffset = mCapsuleOffsetArray [Index];
    CapsuleFileName = mCapsuleFileNameArray [Index];
    if (CompareGuid (&CapsuleHeader->CapsuleGuid, &gWindowsUxCapsuleGuid)) {
      DEBUG ((DEBUG_INFO, "ProcessCapsuleImageEx (Ux) - 0x%x\n", CapsuleHeader));
      DisplayCapsuleExist = TRUE;
      DEBUG ((DEBUG_INFO, "Display logo capsule is found\n"));
      Status = ProcessCapsuleImageEx (CapsuleHeader, CapsuleOffset, CapsuleFileName, &ResetNeeded);
      mCapsuleStatusArray [Index] = EFI_SUCCESS;
      DEBUG((DEBUG_INFO, "ProcessCapsuleImageEx (Ux) - %r\n", Status));
      /*
        Continue looking for more display capsules to make sure that the
        same doesn't get missed in processing and then updating its
        mCapsuleStatusArray entry with status. This is to handle following
        corner cases during engineering testing (though not a possibility
        in an HLOS triggered end-user usecase)
        1. Capsule update using capsuleapp from UEFI shell:
           a. Device encounters an abnormal reboot just after all the
              non-display capsules are processed but just before the
              persisted capsule storage is flushed.
           b. In the immediate boot, user again uses the capsuleapp to apply
              a UX capsule.
        2. Mass-storage capsules: Uses pushes two UX capsules before rebooting
           to apply update

        This will result in persisted capsule storage to have two UX capsules.
        If the above logic breaks after the first UX capsule, the second
        UX capsule will get processed only in the subsequent reboot. To avoid
        this, we should process all the UX capsules that are found at this
        point.

        Note: We do not delete UX capsule at this point to make sure that UX
        capsule is available for processing till all the capsules are processed
        successfully. This is needed to handle any abnormal resets between
        capsule processing which will result in resuming the update from last
        undeleted capsule lying in persisted storage
      */
    }
  }

  if (!DisplayCapsuleExist) {
    //
    // Display Capsule not found. Display the default string.
    //
    Print (L"Updating the firmware ......\r\n");
  }

  //
  // All capsules left are recognized by platform.
  //
  for (Index = 0; Index < mCapsuleTotalNumber; Index++) {
    if (mCapsuleStatusArray [Index] != EFI_NOT_READY) {
      // already processed
      continue;
    }
    CapsuleHeader = (EFI_CAPSULE_HEADER*) mCapsulePtr [Index];
    CapsuleOffset = mCapsuleOffsetArray [Index];
    CapsuleFileName = mCapsuleFileNameArray [Index];
    if (!CompareGuid (&CapsuleHeader->CapsuleGuid, &gWindowsUxCapsuleGuid)) {
      //
      // Call capsule library to process capsule image.
      //
      EmbeddedDriverCount = 0;
      if (IsFmpCapsule(CapsuleHeader)) {
        Status = ValidateFmpCapsule (CapsuleHeader, &EmbeddedDriverCount);
        if (EFI_ERROR(Status)) {
          DEBUG((DEBUG_ERROR, "ValidateFmpCapsule failed. Ignore!\n"));
          mCapsuleStatusArray [Index] = EFI_UNSUPPORTED;
          // continue execution as we need to delete capsule and free capsule pointers below
        }
      } else {
        mCapsuleStatusArray [Index] = EFI_UNSUPPORTED;
        // continue execution as we need to delete capsule and free capsule pointers below
      }

      // Skips the processing of capsules with errors in capsule/FMP headers
      if (((!FirstRound) || (EmbeddedDriverCount == 0)) &&
          (mCapsuleStatusArray [Index] == EFI_NOT_READY) &&
          (bPowerGood != FALSE)) {

        /* LastAttemptStatus is set to an appropriate power event type if
           bPowerGood is FALSE and the function returns success. */
        Status = FwUpdateCheckPowerLevel(&bPowerGood, &MsftAttemptStatus, &QcResultFlag);
        if (EFI_ERROR(Status)) {
          /* Hard-code an error value of insufficient battery if failure retrieving power level. */
          MsftAttemptStatus = LAST_ATTEMPT_STATUS_ERROR_PWR_EVT_BATT;
          QcResultFlag = QCFW_UPDATE_ERR_RESOUCE_POWER_INSUFFICEIENT_BATTERY;

          bPowerGood = FALSE;
          DEBUG((DEBUG_ERROR, "Aborting update of further capsules due to failure in battery status check!\n"));

          mCapsuleStatusArray [Index] = EFI_ABORTED;
        } else if (bPowerGood == FALSE) {
          DEBUG((DEBUG_ERROR, "Aborting update of further capsules due to insufficient battery!\n"));

          mCapsuleStatusArray [Index] = EFI_ABORTED;
        } else {
          DEBUG((DEBUG_INFO, "ProcessCapsuleImage - 0x%x\n", CapsuleHeader));
          Status = ProcessCapsuleImageEx (CapsuleHeader, CapsuleOffset, CapsuleFileName, &ResetNeeded);
          mCapsuleStatusArray [Index] = Status;
          DEBUG((DEBUG_INFO, "ProcessCapsuleImage - %r\n", Status));

          if (Status != EFI_NOT_READY) {
            if (EFI_ERROR(Status)) {
              REPORT_STATUS_CODE(EFI_ERROR_CODE, (EFI_SOFTWARE | PcdGet32(PcdStatusCodeSubClassCapsule) | PcdGet32(PcdCapsuleStatusCodeUpdateFirmwareFailed)));
              DEBUG ((DEBUG_ERROR, "Capsule process failed!\n"));
              Print (L"Firmware update failed...\r\n");
            } else {
              REPORT_STATUS_CODE(EFI_PROGRESS_CODE, (EFI_SOFTWARE | PcdGet32(PcdStatusCodeSubClassCapsule) | PcdGet32(PcdCapsuleStatusCodeUpdateFirmwareSuccess)));
            }
          }
        }

        if ((CapsuleHeader->Flags & PcdGet16(PcdSystemRebootAfterCapsuleProcessFlag)) != 0 ||
            IsFmpCapsule(CapsuleHeader) ||
            (TRUE == ResetNeeded)) {
          mNeedReset = TRUE;
        }

        //
        // Sync ESRT from FMP if there is a battery error and if we haven't
        // synced it already in previous iteration of this loop
        //
        if ((NULL == EsrtManagement) &&
            (bPowerGood == FALSE)) {
          Status = gBS->LocateProtocol(&gEsrtManagementProtocolGuid, NULL, (VOID **)&EsrtManagement);

          if (!EFI_ERROR(Status)) {
            EsrtManagement->SyncEsrtFmp();
          }
        }

        //
        // If the power criteria is not good, update the ESRT using non-FMP method
        // as SyncEsrtFmp() only can query from internal data structures of the FMP
        // protocol and this cannot be updated by this FMP framework in any way.
        // So we directly try to record the failure in ESRT so that data will be used
        // by FMP Dxe and OS will consume it on next boot.
        //
        if ((NULL != EsrtManagement) &&
            (bPowerGood == FALSE)) {
          if (IsNestedFmpCapsule(CapsuleHeader)) {
            NestedCapsuleHeader = (EFI_CAPSULE_HEADER *)((UINT8 *)CapsuleHeader + CapsuleHeader->HeaderSize);
            FmpCapsuleHeader = (EFI_FIRMWARE_MANAGEMENT_CAPSULE_HEADER *) ((UINT8 *) NestedCapsuleHeader + NestedCapsuleHeader->HeaderSize);
          } else {
            FmpCapsuleHeader = (EFI_FIRMWARE_MANAGEMENT_CAPSULE_HEADER *) ((UINT8 *) CapsuleHeader + CapsuleHeader->HeaderSize);
          }

          ItemOffsetList = (UINT64 *)(FmpCapsuleHeader + 1);
          ImageHeader  = (EFI_FIRMWARE_MANAGEMENT_CAPSULE_IMAGE_HEADER *)((UINT8 *)FmpCapsuleHeader + ItemOffsetList[0]);

          StatusEsrt = EsrtManagement->GetEsrtEntry(&ImageHeader->UpdateImageTypeId, &EsrtEntry);
          if (!EFI_ERROR(StatusEsrt)){
            EsrtEntry.LastAttemptStatus = MsftAttemptStatus;
            EsrtEntry.LastAttemptVersion = 0;

            EsrtManagement->UpdateEsrtEntry(&EsrtEntry);
          }
        }
      }

      //
      // Delete capsule and de-initialize capsule pointer if the capsule is
      // already attempted (SUCCESS/FAILURE) for processing
      //
      if (mCapsuleStatusArray [Index] != EFI_NOT_READY) {
        Status = DeleteCapsule (
          CapsuleHeader,
          CapsuleOffset,
          CapsuleFileName,
          CapsuleStorage
          );
        if (EFI_ERROR(Status)) {
          DEBUG((DEBUG_ERROR, "DeleteCapsule returned error - %r\n", Status));
          return Status;
        }

        Status = DeInitCapsulePtrMemory (
          &(mCapsulePtr [Index]),
          CapsuleStorage
          );
        if (EFI_ERROR(Status)) {
          DEBUG((DEBUG_WARN, "DeInitCapsulePtrMemory returned error - %r. Ignoring and proceeding.\n", Status));
          Status = EFI_SUCCESS;
        }
      }
    }
  }

  //
  // Sync here only if we haven't synced this above due to battery error
  //
  if (NULL == EsrtManagement) {
    Status = gBS->LocateProtocol(&gEsrtManagementProtocolGuid, NULL, (VOID **)&EsrtManagement);
    //
    // Always sync ESRT Cache from FMP Instance
    //
    if (!EFI_ERROR(Status)) {
      EsrtManagement->SyncEsrtFmp();
    }
  }

  Status = EFI_SUCCESS;

  REPORT_STATUS_CODE(EFI_PROGRESS_CODE, (EFI_SOFTWARE | PcdGet32(PcdStatusCodeSubClassCapsule) | PcdGet32(PcdCapsuleStatusCodeProcessCapsulesEnd)));

  return Status;
}

/**
  Do reset system.
**/
VOID
DoResetSystem (
  VOID
  )
{
  UINTN                         Index = 0;
  EFI_STATUS                    Status = EFI_SUCCESS;

  REPORT_STATUS_CODE(EFI_PROGRESS_CODE, (EFI_SOFTWARE | PcdGet32(PcdStatusCodeSubClassCapsule) | PcdGet32(PcdCapsuleStatusCodeResettingSystem)));

  Print(L"Capsule Request Cold Reboot.\n");
  DEBUG((DEBUG_INFO, "Capsule Request Cold Reboot."));

  for (Index = 5; Index > 0; Index--) {
    Print(L"Resetting system in %d seconds ...\n", Index);
    DEBUG((DEBUG_INFO, "Resetting system in %d seconds ...\n", Index));
    gBS->Stall(1000000);
  }

  /* Dump Log Buffer to File before rebooting after capsule update */
  Status = WriteLogBufToPartition ();
  if (EFI_ERROR(Status))
  {
     DEBUG ((EFI_D_WARN, "Write Log Buffer to FAT partition after capsule update failed, Status = (%r)\r\n", Status));
  }

  gRT->ResetSystem(EfiResetCold, EFI_SUCCESS, 0, NULL);

  CpuDeadLoop();
}

/**

  This routine is called to process capsules.

  Caution: This function may receive untrusted input.

  The capsules reported in the given CapsuleStorage are processed.
  If there is no capsules found in the given storage type, this routine does nothing.

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
  @retval EFI_END_OF_MEDIA        No persisted capsules found.

**/
EFI_STATUS
EFIAPI
ProcessCapsules (
  )
{
  EFI_STATUS                     UpdateStatus = EFI_SUCCESS;
  EFI_STATUS                     Status = EFI_SUCCESS;
  CapsuleStorageType             CapsuleStorage = CAPSULE_STORAGE_TYPE_UNKNOWN;
  ESRT_MANAGEMENT_PROTOCOL      *EsrtProtocol = NULL;
  UINT64                        *OsIndications = NULL;
  UINTN                          Size = 0;

  Status = gBS->LocateProtocol(&gEsrtManagementProtocolGuid, NULL, (VOID **) &EsrtProtocol);
  if (EFI_ERROR(Status)) {
    DEBUG ((EFI_D_WARN, "Failed to locate protocol 'gEsrtManagementProtocolGuid'!. Status - %r\n", Status));
    Status = EFI_SUCCESS;
  } else {
    /* Sync cached ESRT with FMP instance data */
    Status = EsrtProtocol->SyncEsrtFmp();
    if (Status != EFI_SUCCESS) {
      DEBUG ((EFI_D_WARN, "Warning! Failed to sync FMP ESRTs before processing capsules. Status - %r\n", Status));
      Status = EFI_SUCCESS;
    }
  }

  //
  // Get the current capsule storage type to be used for persisting capsules
  //
  CapsuleStorage = (CapsuleStorageType)PcdGet16(PcdCapsuleStorageType);

  if (!mDxeCapsuleLibEndOfDxe) {
    UpdateStatus = ProcessTheseCapsules(TRUE, CapsuleStorage);

    if (AreAllImagesProcessed()) {
      FlushPersistedCapsules(CapsuleStorage);
    }

    //
    // Reboot System if and only if all capsule processed.
    // If not, defer reset to 2nd process.
    //
    if (mNeedReset && AreAllImagesProcessed()) {
      DoResetSystem();
    }
  } else {
    UpdateStatus = ProcessTheseCapsules(FALSE, CapsuleStorage);

    if (AreAllImagesProcessed()) {
      FlushPersistedCapsules(CapsuleStorage);
    }

    //
    // Reboot System if required after all capsule processed
    //
    if (mNeedReset) {
      DoResetSystem();
    }
  }

  // Deinitialize the pointers so that there is no side-effect on mass-storage capsules
  DeInitCapsulePtr (CapsuleStorage);

  // If there are no capsule found but no other error, then try looking for
  // mass-storage capsules.
  if ((EFI_END_OF_MEDIA != UpdateStatus) &&
      EFI_ERROR (UpdateStatus)) {
    goto Exit;
  }

  ////////////////////////////////////////////////////////////////
  // Process Mass-Storage capsules that might be present in the //
  // EFI\UpdateCapsule folder of EFI System partition           //
  ////////////////////////////////////////////////////////////////

  // Check if there are any mass-storage capsules present and process if any

  if (TRUE == IsMassStorageCapsuleFlagEnabled()) {
    UpdateStatus = ProcessTheseCapsules(TRUE, CAPSULE_STORAGE_TYPE_MASS_STORAGE);

    // Update OsIndication status when NoMedia found, AllImagesProcess for MassStorageEnabled
    if (AreAllImagesProcessed()) {
      FlushPersistedCapsules(CAPSULE_STORAGE_TYPE_MASS_STORAGE);
      OsIndications = BdsLibGetVariableAndSize (L"OsIndications", &gEfiGlobalVariableGuid, &Size);

      if (OsIndications != NULL) {
         if ((*OsIndications & EFI_OS_INDICATIONS_FILE_CAPSULE_DELIVERY_SUPPORTED) != 0) {
           *OsIndications &= ~(UINT64)EFI_OS_INDICATIONS_FILE_CAPSULE_DELIVERY_SUPPORTED;
           Status = gRT->SetVariable(L"OsIndications",
                                     &gEfiGlobalVariableGuid,
                                     EFI_VARIABLE_NON_VOLATILE|EFI_VARIABLE_BOOTSERVICE_ACCESS|EFI_VARIABLE_RUNTIME_ACCESS,
                                     sizeof(UINT64),
                                     OsIndications);
         }
         FreePool (OsIndications);
         OsIndications = NULL;

      }
    }

    if (mNeedReset) {
      Status = gRT->SetVariable(
        L"BootNext",
        &gEfiGlobalVariableGuid,
        EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
        0,
        NULL
        );
      if (EFI_ERROR(Status)) {
        DEBUG((EFI_D_ERROR, "!!Warning!! Could not clear 'BootNext' variable after processing mass-storage capsules. Status - %r!\r\n", Status));

        // Ignore the error as we might not always find a BootNext variable.
        Status = EFI_SUCCESS;
      }

      DoResetSystem();
    }


    // Deinitialize the pointers so that there is no unused memory if at
    // all we are proceeding without reset
    DeInitCapsulePtr (CAPSULE_STORAGE_TYPE_MASS_STORAGE);
  } else {
    DEBUG ((DEBUG_WARN, "Mass-storage capsule flag is not enabled. Processing of mass-storage capsules not attempted!\n"));
  }

Exit:

  if (NULL != EsrtProtocol) {
    // Lock the ESRT repository as we dont expect any more update to the ESRT
    // data base maintained through ESRT management protocol
    Status = EsrtProtocol->LockEsrtRepository();
    if (Status != EFI_SUCCESS) {
      DEBUG ((EFI_D_INFO, "Warning! Failed to lock FMP ESRTs. Status - %r\n", Status));
      Status = EFI_SUCCESS;
    }
  }

  return UpdateStatus;
}

