/** @file
  Additional Qualcomm functions for DXE capsule library.

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
 02/01/18   prema   Freed valid OsIndication variable and not clearing
                    OsIndiaction NV status in LoadMassStorageCapsules(), as
                    this operation will be handled post-update
 08/30/17   gparam  Fix for incorrect string comparison in capsule sorting code
 05/14/17   gparam  Changes to do the check on the mass-storage capsule flag even
                    before attempting to process the mass-storage capsules
 04/18/17   gparam  Absence of 'OsIndications' variable as well to be considered
                    as no mass-storage capsules + Removed the 'OsIndications'
                    pre-filling temporary hack used to process all mass-storage
                    capsules found during any boot
 03/24/17   gparam  New functions to abstract some more operations on capsule
                    persisting media + Restructuring of code between files +
                    Correction of function headers + Relocated storage pointer
                    initialization to start of runtime APIs instead of the
                    persist-capsule call
 03/15/17   gparam  Moved BdsFormalizeEfiGlobalVariable from QcomBds to
                    to DxeCapsuleLibFmp to avoid creating stubs for use in LA +
                    Change to auto-trigger ESRT lock call only if the lock protocol
                    is available in the system
 02/22/17   gparam  Initial revision

=============================================================================*/
#include <Protocol/Capsule.h>
#include <Protocol/EFICapsule.h>
#include <Guid/CapsuleVendor.h>
#include <Guid/FmpCapsule.h>
#include <Guid/FileInfo.h>
#include <Guid/GlobalVariable.h>

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
#include <Library/UefiCfgLib.h>
#include <Library/SortLib.h>
#include <Library/HobLib.h>
#include <Library/GenericBdsLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DevicePathLib.h>

#include <Protocol/SimpleFileSystem.h>
#include <Protocol/EsrtManagement.h>
#include <Protocol/FirmwareManagement.h>

#include <Guid/EventGroup.h>

#include "DxeCapsuleQcom.h"
#include "DxeCapsulePartition.h"
#include "DxeCapsuleRawFile.h"
#include "CommonHeader.h"
#include "ProcessCoalescedCapsules.h"

#define CAPSULE_HEADER_OFFSET                   0x3E2000
#define CAPSULE_HEADER_SIZE                     0x1000
#define SCATTERGATHERLIST_ARRAY_HEADER_REVISION 1

VOID                         *CapsuleStorageHeader = NULL;

extern VOID                 **mCapsulePtr;
extern UINT32                *mCapsuleOffsetArray;
extern CHAR16               **mCapsuleFileNameArray;
extern VOID                  *CapsuleStorageHeader;
extern UINT32                 mCapsuleTotalNumber;
extern EFI_STATUS            *mCapsuleStatusArray;

// RAM based Scatter Gather List
SCATTERGATHERLIST_ARRAY_HEADER *mScatterGatherHeader = NULL;
UINT32                          mScatterGatherHeaderSize = 0;

// Cached EFI partition handle list for processing mass-storage capsules
UINTN                           mNumMassStoragePartitionHandles = UINTN_MAX_VALUE;
EFI_HANDLE                     *mMassStoragePartitionHandleBuffer = NULL;

// TODO: Giri: Revalidate the sorting as this seems to be inconsistant
/**
  Function to compare strings per section 7.5.5 of the UEFI 2.4B specification

  @param[in] Buffer1                  The pointer to first buffer.
  @param[in] Buffer2                  The pointer to second buffer.

  @retval 0                           Buffer1 equal to Buffer2.
  @return <0                          Buffer1 is less than Buffer2.
  @return >0                          Buffer1 is greater than Buffer2.
**/
STATIC
INTN
EFIAPI
CapsuleStringCompare (
  IN CONST VOID                 *Buffer1,
  IN CONST VOID                 *Buffer2
  )
{
  CONST CHAR16 *String1 = Buffer1;
  CONST CHAR16 *String2 = Buffer2;

  do {
    BOOLEAN LastDot1 = ((*String1 == '.') && (NULL == StrStr(String1+1,L".")));
    BOOLEAN LastDot2 = ((*String2 == '.') && (NULL == StrStr(String2+1,L".")));
    CHAR16 Compare1;
    CHAR16 Compare2;

    if ((*String1 == '\0') && (*String2 == '\0')) {
      return 0;
    }

    if((LastDot1 && !LastDot2) || (*String1 == '\0')) {
      Compare1 = ' ';
    }
    else {
      if ((*String1 >= 'a') && (*String1 <= 'z')) {
        Compare1 = *String1 + 'A' - 'a';
      }
      else {
        Compare1 = *String1;
      }
    }
    if((LastDot2 && !LastDot1) || (*String2 == '\0')) {
      Compare2 = ' ';
    }
    else {
      if ((*String2 >= 'a') && (*String2 <= 'z')) {
        Compare2 = *String2 + 'A' - 'a';
      }
      else {
        Compare2 = *String2;
      }
    }

    if (Compare1 != Compare2) {
      return Compare1 - Compare2;
    }

    if ((*String1 != '\0') ||
        !LastDot1 ||
        (LastDot1 && LastDot2)) {
      String1++;
    }

    if ((*String2 != '\0') ||
        !LastDot2 ||
        (LastDot1 && LastDot2)) {
      String2++;
    }
  } while(1);
}


/**
  Locates the handle to a device on children of the device path that supports the specified protocol.

  @param  Protocol              Specifies the protocol to search for.
  @param  DevicePath            Pointer to a pointer to the device path.
  @param  HandleCount           A pointer to the number of handles returned.
  @param  Handles               A pointer to an array of handles.

  @retval EFI_SUCCESS           The resulting handle was returned.
  @retval EFI_NOT_FOUND         No handles match the search.
  @retval EFI_INVALID_PARAMETER Protocol is NULL.
  @retval EFI_INVALID_PARAMETER DevicePath is NULL.
  @retval EFI_INVALID_PARAMETER A handle matched the search and Device is NULL.

**/
EFI_STATUS
EFIAPI
LocateChildDevicePath (
  IN EFI_GUID                       *Protocol,
  IN EFI_DEVICE_PATH_PROTOCOL      **DevicePath,
  OUT UINTN                         *MatchHandleCount,
  OUT EFI_HANDLE                   **MatchHandles
  )
{
  INTN                        SourceSize;
  INTN                        Size;
  UINTN                       HandleCount;
  UINTN                       Index;
  EFI_STATUS                  Status;
  EFI_HANDLE                  *Handles;
  EFI_HANDLE                  Handle;
  EFI_DEVICE_PATH_PROTOCOL    *SourcePath;
  EFI_DEVICE_PATH_PROTOCOL    *TmpDevicePath;

  if (Protocol == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if ((DevicePath == NULL) || (*DevicePath == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  if ((MatchHandles == NULL) || (MatchHandleCount == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  Handles = NULL;
  SourcePath = *DevicePath;
  TmpDevicePath = SourcePath;
  while (!IsDevicePathEnd (TmpDevicePath)) {
    if (IsDevicePathEndInstance (TmpDevicePath)) {
      //
      // If DevicePath is a multi-instance device path,
      // the function will operate on the first instance
      //
      break;
    }
    TmpDevicePath = NextDevicePathNode (TmpDevicePath);
  }

  SourceSize = (UINTN) TmpDevicePath - (UINTN) SourcePath;

  //
  // Get a list of all handles that support the requested protocol
  //
  Status = gBS->LocateHandleBuffer (ByProtocol, Protocol, NULL, &HandleCount, &Handles);
  if (EFI_ERROR (Status) || HandleCount == 0) {
    return EFI_NOT_FOUND;
  }

  *MatchHandles = AllocatePool (HandleCount * sizeof(EFI_HANDLE));
  if (NULL == *MatchHandles) {
    return EFI_NOT_FOUND;
  }

  *MatchHandleCount = 0;
  for(Index = 0; Index < HandleCount; Index += 1) {
    Handle = Handles[Index];
    Status = gBS->HandleProtocol (Handle, &gEfiDevicePathProtocolGuid, (VOID **)&TmpDevicePath);
    if (EFI_ERROR (Status)) {
      //
      // If this handle doesn't support device path, then skip it
      //
      continue;
    }

    //
    // Check if DevicePath is first part of SourcePath
    //
    Size = GetDevicePathSize (TmpDevicePath) - sizeof(EFI_DEVICE_PATH_PROTOCOL);
    ASSERT (Size >= 0);
    if ((Size >= SourceSize) && CompareMem (SourcePath, TmpDevicePath, (UINTN) SourceSize) == 0) {
      *MatchHandles[*MatchHandleCount] = Handle;
      (*MatchHandleCount)++;
    }
  }

  FreePool (Handles);

  //
  // If there wasn't any match, then no parts of the device path was found.
  // Which is strange since there is likely a "root level" device path in the system.
  //
  if (*MatchHandleCount == 0) {
    return EFI_NOT_FOUND;
  }

  return EFI_SUCCESS;
}

/**
  Function to get the count of the capsules from mass storage per section 7.5.5 of the UEFI 2.4B specification

  @param[in, out]  NoHandles    The number of handles returned in Buffer.
  @param[out]      Buffer       A pointer to the buffer to return the requested array of handles that
                                support Protocol.

  @retval EFI_SUCCESS             UEFI Capsule Runtime Services are installed successfully.
  @retval EFI_INVALID_PARAMETER   Invalid paramter received
  @retval EFI_VOLUME_CORRUPTED    Capsule partition corrupted

**/
EFI_STATUS
EFIAPI
GetPartitionHandlesForMassStorageCapsules (
  IN OUT UINTN                        *NoHandles,
  OUT    EFI_HANDLE                   **Buffer
  )
{
  UINTN                     Index = 0;
  EFI_STATUS                Status = EFI_SUCCESS;
  UINTN                     Size;
  EFI_DEVICE_PATH_PROTOCOL *DevPath = NULL;
  UINT16                   *BootVar = 0;
  CHAR16                    BootVariableName[12] = {0};

  if ((NULL == NoHandles) ||
      (NULL == Buffer)) {
    DEBUG ((DEBUG_ERROR, "Invalid parameter\n"));
    return EFI_INVALID_PARAMETER;
  }

  // If the handle count is not holding the default value, this function was
  // called earlier and data is available in cache. Just return the cached data
  // instead of recreating the data
  if (UINTN_MAX_VALUE != mNumMassStoragePartitionHandles) {
    DEBUG ((DEBUG_INFO, "Returning cached EFI patition handles for mass-storage capsule processing\n"));

    if ((0 == mNumMassStoragePartitionHandles) ||
        (NULL == mMassStoragePartitionHandleBuffer)) {
      return EFI_NOT_FOUND;
    }

    *NoHandles = mNumMassStoragePartitionHandles;
    *Buffer = AllocatePool (sizeof(EFI_HANDLE) * mNumMassStoragePartitionHandles);

    if (NULL == *Buffer) {
      DEBUG ((DEBUG_WARN, "Failed to allocate memory for *Buffer\n"));
      return EFI_OUT_OF_RESOURCES;
    }
    CopyMem(*Buffer, mMassStoragePartitionHandleBuffer, sizeof(EFI_HANDLE) * mNumMassStoragePartitionHandles);
  }

  DEBUG ((DEBUG_INFO, "Looking for 'BootNext' variable to look for EFI partition with mass-storage capsules\n"));
  BootVar = BdsLibGetVariableAndSize (L"BootNext", &gEfiGlobalVariableGuid, &Size);
  if (NULL != BootVar) {
    CHAR8 *BootVariable;
    DevPath = NULL;
    UnicodeSPrint (BootVariableName, sizeof (BootVariableName), L"Boot%04x", BootVar[0]);
    BootVariable = BdsLibGetVariableAndSize (
                    BootVariableName,
                    &gEfiGlobalVariableGuid,
                    &Size
                    );
    if (BootVariable != NULL) {
      if ((*(UINT16*)(BootVariable+4)) != 0) {
        DEBUG ((DEBUG_WARN, "Trying to locate EFI partition in bootable device '%s' (%s).\n", (CHAR16*)(BootVariable+6), BootVariableName));

        DevPath = (EFI_DEVICE_PATH_PROTOCOL *)(BootVariable+6+StrSize((CHAR16*)(BootVariable+6)));
        Status = LocateChildDevicePath ( &gEfiPartTypeSystemPartGuid,
                                         &DevPath,
                                         NoHandles,
                                         Buffer);
        if (EFI_ERROR(Status)) {
          *NoHandles = 0;
          *Buffer = NULL;
          DEBUG ((DEBUG_WARN, "Failed to locate EFI partition in bootable device '%s'. Status - %r!\n", (CHAR16*)(BootVariable+6), Status));
        } else {
          DEBUG ((DEBUG_WARN, "Found %d EFI partitions in bootable device '%s'.\n", *NoHandles, (CHAR16*)(BootVariable+6)));
        }
      }
      FreePool(BootVariable);
    }
    FreePool(BootVar);
  }else {
    DEBUG ((DEBUG_INFO, "No 'BootNext' variable found to look for EFI partition with mass-storage capsules\n"));
  }

  if (NULL == *Buffer) {
    DEBUG ((DEBUG_INFO, "Looking for 'BootOrder' variable to find EFI partition with mass-storage capsules\n"));

    BootVar = BdsLibGetVariableAndSize (L"BootOrder", &gEfiGlobalVariableGuid, &Size);
    if (NULL != BootVar) {
      for (Index = 0; Index < Size/sizeof(UINT16); Index++) {
        CHAR8 *BootVariable;
        EFI_DEVICE_PATH_PROTOCOL  *DevPath = NULL;
        UnicodeSPrint (BootVariableName, sizeof (BootVariableName), L"Boot%04x", BootVar[Index]);
        BootVariable = BdsLibGetVariableAndSize (
                        BootVariableName,
                        &gEfiGlobalVariableGuid,
                        &Size
                        );

        if (BootVariable != NULL) {
          if ((*(UINT16*)(BootVariable+4)) != 0) {
            DEBUG ((DEBUG_WARN, "Trying to locate EFI partition in bootable device '%s' (%s).\n", (CHAR16*)(BootVariable+6), BootVariableName));

            DevPath = (EFI_DEVICE_PATH_PROTOCOL *)(BootVariable+6+StrSize((CHAR16*)(BootVariable+6)));
            Status = LocateChildDevicePath ( &gEfiPartTypeSystemPartGuid,
                                             &DevPath,
                                             NoHandles,
                                             Buffer);
            if (EFI_ERROR(Status)) {
              *NoHandles = 0;
              *Buffer = NULL;
              DEBUG ((DEBUG_WARN, "Failed to locate EFI partition in bootable device '%s'. Status - %r!\n", (CHAR16*)(BootVariable+6), Status));
            } else {
              DEBUG ((DEBUG_WARN, "Found %d EFI partitions in bootable device '%s'.\n", *NoHandles, (CHAR16*)(BootVariable+6)));
              break;
            }
          }
          FreePool(BootVariable);
        }
      }
      FreePool(BootVar);
    } else {
      DEBUG ((DEBUG_WARN, "Could not find 'BootOrder' variable to find EFI partition with mass-storage capsules!\n"));
    }
  }

  if (NULL == *Buffer) {
    DEBUG ((DEBUG_WARN, "Looking for local EFI partition variable to find EFI partition with mass-storage capsules\n"));
    //Get all ESP partitions on UFS
    Status = gBS->LocateHandleBuffer (
      ByProtocol,
      &gEfiPartTypeSystemPartGuid,
      NULL,
      NoHandles,
      Buffer
      );
    if (EFI_ERROR(Status)) {
      DEBUG ((DEBUG_ERROR, "Failed to locate EFI partition in local device. Status - %r!\n", Status));
      goto Exit;
    } else {
      DEBUG ((DEBUG_INFO, "Found %d EFI partitions in local device\n", *NoHandles));
    }
  }

Exit:
  // Cache the value if we have not cached earlier so that we can direclty use
  // this data in later calls
  if (UINTN_MAX_VALUE == mNumMassStoragePartitionHandles) {
    mNumMassStoragePartitionHandles = *NoHandles;
    mMassStoragePartitionHandleBuffer = AllocatePool (sizeof(EFI_HANDLE) * mNumMassStoragePartitionHandles);

    if (NULL == mMassStoragePartitionHandleBuffer) {
      DEBUG ((DEBUG_WARN, "Failed to allocate memory for mMassStoragePartitionHandleBuffer\n"));
      return EFI_OUT_OF_RESOURCES;
    }
    CopyMem(mMassStoragePartitionHandleBuffer, *Buffer, sizeof(EFI_HANDLE) * mNumMassStoragePartitionHandles);
  }

  return Status;
}


/**
  Function to load the capsules from mass storage per section 7.5.5 of the UEFI 2.4B specification

  @param [in] CapsuleFileName     Name of mass-storage capsules file to be deleted.

  @retval EFI_SUCCESS             UEFI Capsule Runtime Services are installed successfully.
  @retval EFI_INVALID_PARAMETER   Invalid paramter received
  @retval EFI_VOLUME_CORRUPTED    Capsule partition corrupted

**/
EFI_STATUS
EFIAPI
DeleteMassStorageCapsule (
  IN CHAR16         *CapsuleFileName
  )
{
  EFI_HANDLE *Handles = NULL;
  UINTN NoHandles = 0;
  UINTN Index;
  EFI_STATUS Status = EFI_SUCCESS;
  EFI_FILE_INFO *FileInfo = NULL;
  UINTN  Size;
  BOOLEAN FileFound = FALSE;

  if (NULL == CapsuleFileName) {
    DEBUG ((DEBUG_ERROR, "Invalid value received for parameter CapsuleFileName\n"));
    return EFI_INVALID_PARAMETER;
  }

  //Get all ESP partitions that can have mass-storage capsules
  Status = GetPartitionHandlesForMassStorageCapsules (
    &NoHandles,
    &Handles
    );
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "GetPartitionHandlesForMassStorageCapsules returned - %r!\n", Status));
    return Status;
  }

  if (NoHandles != 0) {
    FileInfo = AllocatePool (sizeof(EFI_FILE_INFO) + MAX_FILE_NAME_SIZE);
    if (FileInfo == NULL) {
      if(NULL != Handles) {
        FreePool(Handles);
      }
      return EFI_OUT_OF_RESOURCES;
    }
  }

  if (NULL == Handles) {
    return EFI_OUT_OF_RESOURCES;
  }

  for (Index = 0; (Index < NoHandles) && (FALSE == FileFound) ; Index++) {
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *SimpleFileSystem = NULL;
    EFI_FILE_PROTOCOL *RootVolume = NULL;
    EFI_FILE_PROTOCOL *Directory = NULL;
    EFI_FILE_PROTOCOL *FileHandle = NULL;

    Status = gBS->HandleProtocol (Handles[Index], &gEfiSimpleFileSystemProtocolGuid, (VOID **)&SimpleFileSystem);
    if (EFI_ERROR(Status)) {
      continue;
    }

    Status = SimpleFileSystem->OpenVolume(SimpleFileSystem, &RootVolume);
    if (EFI_ERROR(Status)) {
      continue;
    }

    Status = RootVolume->Open (RootVolume, &Directory, L"EFI\\UpdateCapsule", EFI_FILE_MODE_READ, 0);
    if (EFI_ERROR(Status)) {
      RootVolume->Close(RootVolume);
      continue;
    }
    RootVolume->Close(RootVolume);

    Size = sizeof(EFI_FILE_INFO) + MAX_FILE_NAME_SIZE;
    while (EFI_SUCCESS == Directory->Read (Directory, &Size, FileInfo)) {
      if(Size == 0) {
        break;
      }

      if ((FileInfo->Attribute & EFI_FILE_DIRECTORY) != 0) {
        Size = sizeof(EFI_FILE_INFO) + MAX_FILE_NAME_SIZE;
        continue;
      }

      if (0 == StrCmp(CapsuleFileName, FileInfo->FileName)) {
        FileFound = TRUE;

        Status = Directory->Open(Directory, &FileHandle, FileInfo->FileName, EFI_FILE_MODE_READ|EFI_FILE_MODE_WRITE, 0);
        if (EFI_ERROR(Status)) {
          // Try read only
          Status = Directory->Open(Directory, &FileHandle, FileInfo->FileName, EFI_FILE_MODE_READ, 0);
          if (EFI_ERROR(Status)) {
            Status = EFI_DEVICE_ERROR;
            break;
          }
        }

        DEBUG ((DEBUG_WARN, "Deleting mass-storage capsule file '%s'!\n", FileInfo->FileName));
        Status = FileHandle->Delete(FileHandle);

        if (EFI_ERROR(Status)) {
          Status = EFI_DEVICE_ERROR;
          DEBUG ((DEBUG_ERROR, "Failed to delete mass-storage capsule file '%s'!\n", FileInfo->FileName));
        }

        // Break the loop and exit as we have found the file and attempted a delete
        break;
      }

      Size = sizeof(EFI_FILE_INFO) + MAX_FILE_NAME_SIZE;
    }

    Directory->Close(Directory);
  }

  if (NULL != FileInfo) {
    FreePool(FileInfo);
  }

  if (NULL != Handles) {
    FreePool(Handles);
  }

  return Status;
}

/**
  Function to get the count of the capsules from mass storage per section 7.5.5 of the UEFI 2.4B specification

  @param [in] CapsulePtrArray     Array of capsules with array length CapsulePtrArrayLen.
  @param [in] CapsulePtrArrayLen  Length of the array CapsulePtrArray.

  @retval EFI_SUCCESS             UEFI Capsule Runtime Services are installed successfully.
  @retval EFI_INVALID_PARAMETER   Invalid paramter received
  @retval EFI_VOLUME_CORRUPTED    Capsule partition corrupted

**/
EFI_STATUS
EFIAPI
GetMassStorageCapsuleCount (
  OUT UINT32 *CapsuleCount
  )
{
  EFI_HANDLE     *Handles   = NULL;
  UINTN           NoHandles = 0;
  UINTN           Index     = 0;
  EFI_STATUS      Status    = EFI_SUCCESS;
  EFI_FILE_INFO  *FileInfo  = NULL;
  UINTN           Size      = 0;
  static UINT32   MassStorageCapsuleCount = 0xFFFFFFFF;

  if (CapsuleCount == NULL) {
    DEBUG ((DEBUG_ERROR, "Invalid parameter recevied in GetMassStorageCapsuleCount!\n"));
    return EFI_INVALID_PARAMETER;
  }

  if (0xFFFFFFFF != MassStorageCapsuleCount) {
    DEBUG ((DEBUG_INFO, "Using previously calculated capsule count value for mass-storage capsules!\n"));
    *CapsuleCount = MassStorageCapsuleCount;
    return EFI_SUCCESS;
  }

  // Initialize to zero as we are incrementing later
  *CapsuleCount = 0;
  MassStorageCapsuleCount = 0;

  //Get all ESP partitions
  Status = GetPartitionHandlesForMassStorageCapsules (
    &NoHandles,
    &Handles
    );
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "GetPartitionHandlesForMassStorageCapsules returned - %r!\n", Status));
    return Status;
  }

  if (NoHandles != 0) {
    FileInfo = AllocatePool (sizeof(EFI_FILE_INFO) + MAX_FILE_NAME_SIZE);
    if (FileInfo == NULL) {
      if(NULL != Handles) {
        FreePool(Handles);
      }
      DEBUG ((DEBUG_ERROR, "No memory to allocate for FileInfo in GetMassStorageCapsuleCount!\n"));
      return EFI_OUT_OF_RESOURCES;
    }
  }

  if (NULL == Handles) {
    DEBUG ((DEBUG_ERROR, "No EFI partition handles found in GetMassStorageCapsuleCount!\n"));
    return EFI_OUT_OF_RESOURCES;
  }

  for (Index = 0; Index < NoHandles; Index++) {
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *SimpleFileSystem;
    EFI_FILE_PROTOCOL *RootVolume;
    EFI_FILE_PROTOCOL *Directory;
    CHAR16 *FileNames = NULL;
    UINTN Files = 0;
    UINTN AllocatedFiles = 0;

    Status = gBS->HandleProtocol (Handles[Index], &gEfiSimpleFileSystemProtocolGuid, (VOID **)&SimpleFileSystem);
    if (EFI_ERROR(Status)) {
      continue;
    }

    Status = SimpleFileSystem->OpenVolume(SimpleFileSystem, &RootVolume);
    if (EFI_ERROR(Status)) {
      continue;
    }

    Status = RootVolume->Open (RootVolume, &Directory, L"EFI\\UpdateCapsule", EFI_FILE_MODE_READ, 0);
    if (EFI_ERROR(Status)) {
      RootVolume->Close(RootVolume);
      continue;
    }
    RootVolume->Close(RootVolume);

    Size = sizeof(EFI_FILE_INFO) + MAX_FILE_NAME_SIZE;
    while (EFI_SUCCESS == Directory->Read (Directory, &Size, FileInfo)) {
      if(Size == 0) {
        break;
      }

      if ((FileInfo->Attribute & EFI_FILE_DIRECTORY) != 0) {
        Size = sizeof(EFI_FILE_INFO) + MAX_FILE_NAME_SIZE;
        continue;
      }

      if (Files == AllocatedFiles) {
        CHAR16 *FileNamesNew = AllocatePool((AllocatedFiles + FILE_COUNT_STEP) * MAX_FILE_NAME_SIZE);
        if (NULL == FileNamesNew) {
          break;
        }
        if (NULL != FileNames) {
          CopyMem(FileNamesNew, FileNames, AllocatedFiles * MAX_FILE_NAME_SIZE);
          FreePool(FileNames);
        }
        AllocatedFiles += FILE_COUNT_STEP;
        FileNames = FileNamesNew;
      }
      CopyMem(FileNames + (Files * MAX_FILE_NAME_SIZE/sizeof(CHAR16)), FileInfo->FileName, StrSize(FileInfo->FileName));
      Files++;
      Size = sizeof(EFI_FILE_INFO) + MAX_FILE_NAME_SIZE;
    }

    if (Files != 0) {
      UINTN File;
      for (File = 0; File < Files; File++) {
        EFI_FILE_PROTOCOL *FileHandle;
        VOID *FileData;
        Status = Directory->Open(Directory, &FileHandle, &FileNames[File*(MAX_FILE_NAME_SIZE/sizeof(CHAR16))], EFI_FILE_MODE_READ|EFI_FILE_MODE_WRITE, 0);
        if (EFI_ERROR(Status)) {
          // Try read only
          Status = Directory->Open(Directory, &FileHandle, &FileNames[File*(MAX_FILE_NAME_SIZE/sizeof(CHAR16))], EFI_FILE_MODE_READ, 0);
          if (EFI_ERROR(Status)) {
            continue;
          }
        }

        Size = sizeof(EFI_FILE_INFO) + MAX_FILE_NAME_SIZE;
        Status = FileHandle->GetInfo(FileHandle, &gEfiFileInfoGuid, &Size, FileInfo);
        if (EFI_ERROR(Status)) {
          continue;
        }

        FileData = AllocatePool(FileInfo->FileSize);
        if (NULL == FileData) {
          continue;
        }

        Size = FileInfo->FileSize;
        Status = FileHandle->Read(FileHandle, &Size, FileData);

        if (!EFI_ERROR(Status)) {
          // Validate the capsule header and add the capsule count if header is valid

          if (EFI_SUCCESS == SupportCapsuleImage (FileData)) {
            (*CapsuleCount)++;
            MassStorageCapsuleCount = *CapsuleCount;
          }
        }
        FreePool(FileData);

        //FreePool(FileData);
      }
    }
    Directory->Close(Directory);
  }

  if (NULL != FileInfo) {
    FreePool(FileInfo);
  }

  if (NULL != Handles) {
    FreePool(Handles);
  }

  return Status;
}

/**
  Function to load the capsules from mass storage per section 7.5.5 of the UEFI 2.4B specification

  @param [in] CapsulePtrArray      Array of capsules with array length CapsulePtrArrayLen.
  @param [in] CapsuleFileNameArray Array of filenames from which the capsules was loaded.
  @param [in] CapsulePtrArrayLen   Length of the array CapsulePtrArray.

  @retval EFI_SUCCESS             UEFI Capsule Runtime Services are installed successfully.
  @retval EFI_INVALID_PARAMETER   Invalid paramter received
  @retval EFI_VOLUME_CORRUPTED    Capsule partition corrupted

**/
EFI_STATUS
EFIAPI
LoadMassStorageCapsules (
  IN OUT VOID           **CapsulePtrArray,
  IN OUT CHAR16         **CapsuleFileNameArray,
  IN UINT32               CapsulePtrArrayLen
  )
{
  EFI_STATUS                 Status = EFI_SUCCESS;
  EFI_HANDLE                *Handles = NULL;
  UINTN                      NoHandles = 0;
  UINTN                      Index = 0;
  EFI_FILE_INFO             *FileInfo = NULL;
  UINTN                      Size = 0;
  EFI_DEVICE_PATH_PROTOCOL  *DevPath = NULL;
  BOOLEAN                    ResetNeeded = FALSE;
  UINT32                     CapsuleCount = 0;
  UINT32                     CapsuleIndex = 0;

  UINT64                    *OsIndications = NULL;
  CHAR16                    *FileNamesNew = NULL;

  if (NULL == CapsulePtrArray) {
    DEBUG ((DEBUG_ERROR, "Invalid value received for parameter CapsulePtrArray\n"));
    return EFI_INVALID_PARAMETER;
  }

  Status = GetMassStorageCapsuleCount(&CapsuleCount);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "GetMassStorageCapsuleCount returned - %r!\n", Status));
    return Status;
  }

  if (CapsulePtrArrayLen != CapsuleCount) {
    Status = EFI_INVALID_PARAMETER;
    DEBUG ((DEBUG_ERROR, "ValidateCapsuleRawFileGuidTable returned error - %r\n", Status));
    return Status;
  }

  OsIndications = BdsLibGetVariableAndSize (L"OsIndications", &gEfiGlobalVariableGuid, &Size);

  if((OsIndications == NULL) || ((*OsIndications & EFI_OS_INDICATIONS_FILE_CAPSULE_DELIVERY_SUPPORTED) == 0)) {
    if (NULL != OsIndications) {
      FreePool (OsIndications);
      OsIndications = NULL;
    }
    return EFI_OUT_OF_RESOURCES;
  }

  /* Do no clear this NV variable as we should do this only post-update so
  that capsule files get reapplied if somebody reboot before all files
  are processed. This can happen either due to power failure or due to a
  reboot initiated as part of double update */
  FreePool (OsIndications);

  //Get all ESP partitions that can have mass-storage capsules
  Status = GetPartitionHandlesForMassStorageCapsules (
    &NoHandles,
    &Handles
    );
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "GetPartitionHandlesForMassStorageCapsules returned - %r!\n", Status));
    return Status;
  }

  if (NoHandles != 0) {
    FileInfo = AllocatePool (sizeof(EFI_FILE_INFO) + MAX_FILE_NAME_SIZE);
    if (FileInfo == NULL) {
      if(NULL != Handles) {
        FreePool(Handles);
      }
      return EFI_OUT_OF_RESOURCES;
    }
  }

  if (NULL == Handles) {
    return EFI_OUT_OF_RESOURCES;
  }

  for (Index = 0; Index < NoHandles; Index++) {
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *SimpleFileSystem = NULL;
    EFI_FILE_PROTOCOL               *RootVolume = NULL;
    EFI_FILE_PROTOCOL               *Directory = NULL;
    CHAR16                          *FileNames = NULL;
    UINTN                            Files = 0;
    UINTN                            AllocatedFiles = 0;

    Status = gBS->HandleProtocol (Handles[Index], &gEfiSimpleFileSystemProtocolGuid, (VOID **)&SimpleFileSystem);
    if (EFI_ERROR(Status)) {
      continue;
    }

    Status = gBS->HandleProtocol (Handles[Index], &gEfiDevicePathProtocolGuid, (VOID **)&DevPath);
    if (EFI_ERROR(Status)) {
      continue;
    }

    Status = SimpleFileSystem->OpenVolume(SimpleFileSystem, &RootVolume);
    if (EFI_ERROR(Status)) {
      continue;
    }

    Status = RootVolume->Open (RootVolume, &Directory, L"EFI\\UpdateCapsule", EFI_FILE_MODE_READ, 0);
    if (EFI_ERROR(Status)) {
      RootVolume->Close(RootVolume);
      continue;
    }
    RootVolume->Close(RootVolume);

    Size = sizeof(EFI_FILE_INFO) + MAX_FILE_NAME_SIZE;
    while (EFI_SUCCESS == Directory->Read (Directory, &Size, FileInfo)) {
      if(Size == 0) {
        break;
      }

      if ((FileInfo->Attribute & EFI_FILE_DIRECTORY) != 0) {
        Size = sizeof(EFI_FILE_INFO) + MAX_FILE_NAME_SIZE;
        continue;
      }

      if (Files == AllocatedFiles) {
        FileNamesNew = AllocateZeroPool((AllocatedFiles + FILE_COUNT_STEP) * MAX_FILE_NAME_SIZE);
        if (NULL == FileNamesNew) {
          break;
        }
        if (NULL != FileNames) {
          CopyMem(FileNamesNew, FileNames, AllocatedFiles * MAX_FILE_NAME_SIZE);
          FreePool(FileNames);
        }
        AllocatedFiles += FILE_COUNT_STEP;
        FileNames = FileNamesNew;
      }
      CopyMem(FileNames + (Files * MAX_FILE_NAME_SIZE/sizeof(CHAR16)), FileInfo->FileName, StrSize(FileInfo->FileName));
      Files++;
      Size = sizeof(EFI_FILE_INFO) + MAX_FILE_NAME_SIZE;
    }

    if (Files != 0) {
      UINTN File;
      PerformQuickSort( FileNames, Files, MAX_FILE_NAME_SIZE, CapsuleStringCompare );
      for (File = 0; File < Files; File++) {
        EFI_FILE_PROTOCOL *FileHandle = NULL;
        VOID              *FileData = NULL;
        CHAR16            *FileName = NULL;

        Status = Directory->Open(Directory, &FileHandle, &FileNames[File*(MAX_FILE_NAME_SIZE/sizeof(CHAR16))], EFI_FILE_MODE_READ|EFI_FILE_MODE_WRITE, 0);
        if (EFI_ERROR(Status)) {
          // Try read only
          Status = Directory->Open(Directory, &FileHandle, &FileNames[File*(MAX_FILE_NAME_SIZE/sizeof(CHAR16))], EFI_FILE_MODE_READ, 0);
          if (EFI_ERROR(Status)) {
            continue;
          }
        }

        DEBUG ((DEBUG_WARN, "Loading mass-storage capsule file '%s'!\n", &FileNames[File*(MAX_FILE_NAME_SIZE/sizeof(CHAR16))]));

        Size = sizeof(EFI_FILE_INFO) + MAX_FILE_NAME_SIZE;
        Status = FileHandle->GetInfo(FileHandle, &gEfiFileInfoGuid, &Size, FileInfo);
        if (EFI_ERROR(Status)) {
          FileHandle->Delete(FileHandle);
          continue;
        }

        FileData = AllocatePool(FileInfo->FileSize);
        if (NULL == FileData) {
          FileHandle->Delete(FileHandle);
          continue;
        }

        FileName = AllocateZeroPool(StrSize(FileInfo->FileName));
        if (NULL == FileName) {
          FileHandle->Delete(FileHandle);
          continue;
        }

        CopyMem(FileName, FileInfo->FileName, StrSize(FileInfo->FileName));

        Size = FileInfo->FileSize;
        Status = FileHandle->Read(FileHandle, &Size, FileData);
        ResetNeeded = TRUE;
        if (!EFI_ERROR(Status)) {

          if (CapsuleIndex >= CapsulePtrArrayLen) {
            DEBUG ((DEBUG_ERROR, "Not enough space to process more capsules. Skipping current capsule file!\n"));
            FileHandle->Delete(FileHandle);
            continue;
          }

          CapsulePtrArray [CapsuleIndex] = FileData;
          CapsuleFileNameArray [CapsuleIndex] = FileName;
          CapsuleIndex++;

          // We will not delete the file here unless we have encountered
          // an error File will be deleted after the processing is done
        } else {
          FileHandle->Delete(FileHandle);
        }
      }
    }
    Directory->Close(Directory);
  }

  if (NULL != FileInfo) {
    FreePool(FileInfo);
  }

  if (NULL != Handles) {
    FreePool(Handles);
  }

  if (NULL != FileNamesNew) {
    FreePool(FileNamesNew);
  }

  return Status;
}

/**

  Persists capsules received in the form of capsule array in RAM itself
  and save the pointer of the scattergatherlist to UEFI RAM carve-out for
  capsule processing.

  @param [in] CapsuleHeaderArray Virtual pointer to an array of virtual pointers
                                 to the capsules being passed into update capsule.
  @param [in] CapsuleCount       Number of pointers to EFI_CAPSULE_HEADER in
                                 CaspuleHeaderArray.
  @param [in] ScatterGatherList  Physical pointer to a set of
                                 EFI_CAPSULE_BLOCK_DESCRIPTOR that describes the
                                 location in physical memory of a set of capsules.

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
PersistCapsulesToRAM (
  IN EFI_CAPSULE_HEADER   **CapsuleHeaderArray,
  IN UINTN                  CapsuleCount,
  IN EFI_PHYSICAL_ADDRESS   ScatterGatherList OPTIONAL
  )
{
  EFI_STATUS                   Status = EFI_SUCCESS;
  UINT32                       SavedCrc32;
  MemRegionInfo                UefiFdMemRegionInfo;
  STATIC CHAR8*                MemLabel = "UEFI FD";
  EFI_PHYSICAL_ADDRESS        *mScatterGatherArray = NULL;

  // Get the Capsule Header region to be used in the implementation
  // that use ScatterGatherList.
  Status = GetMemRegionInfoByName(MemLabel, &UefiFdMemRegionInfo);
  if (Status != EFI_SUCCESS)
  {
    DEBUG((EFI_D_ERROR, "Could not find the Capsule Header region.\r\n"));
    return Status;
  }

  mScatterGatherHeader = (SCATTERGATHERLIST_ARRAY_HEADER*)((UINTN)UefiFdMemRegionInfo.MemBase + CAPSULE_HEADER_OFFSET);
  mScatterGatherHeaderSize = CAPSULE_HEADER_SIZE;

  // If at runtime or Update Capsule partition does not exist,
  // store address of ScatterGatherList in RAM version

  // If the ScatterGatherHeader is not available, return.
  if(mScatterGatherHeader == NULL)
  {
    return EFI_UNSUPPORTED;
  }

  //Check CRC
  SavedCrc32 = mScatterGatherHeader->Crc32;
  mScatterGatherHeader->Crc32 = 0x0;

  Status = CalculateCrc32(mScatterGatherHeader,
                          sizeof(SCATTERGATHERLIST_ARRAY_HEADER),
                          &mScatterGatherHeader->Crc32);

  if(EFI_ERROR(Status))
  {
    //Restore SavedCrc
    mScatterGatherHeader->Crc32 = SavedCrc32;
    return Status;
  }

  if(SavedCrc32 != mScatterGatherHeader->Crc32)
  {
    //Restore SavedCrc
    mScatterGatherHeader->Crc32 = SavedCrc32;
    return EFI_CRC_ERROR;
  }

  mScatterGatherHeader->Crc32 = SavedCrc32;

  //
  // ScatterGatherList is only referenced if the capsules are defined to persist across
  // system reset. Add this ScatterGatherList pointer to the array
  // of ScatterGatherList pointers in memory and let pre-boot driver to pick it up
  // after coming through a system reset.
  //
  mScatterGatherArray =
    (EFI_PHYSICAL_ADDRESS*)((UINT8*)mScatterGatherHeader +
                            mScatterGatherHeader->ArrayOffset);

  // Check to ensure overflow does not occur - account for NULL pointer at end
  if ((mScatterGatherHeader->PointerCount+1)*sizeof(EFI_PHYSICAL_ADDRESS) >=
      mScatterGatherHeaderSize - sizeof(SCATTERGATHERLIST_ARRAY_HEADER))
  {
    return EFI_OUT_OF_RESOURCES;
  }

  mScatterGatherArray[mScatterGatherHeader->PointerCount] = ScatterGatherList;
  mScatterGatherHeader->PointerCount++;

  //Ensure that last entry in ScatterGatherArray is NULL.
  mScatterGatherArray[mScatterGatherHeader->PointerCount] = (UINT64)NULL;

  //Calculate new Header CRC
  mScatterGatherHeader->Crc32 = 0x0;

  Status = CalculateCrc32(mScatterGatherHeader,
                          sizeof(SCATTERGATHERLIST_ARRAY_HEADER),
                          &mScatterGatherHeader->Crc32);

  return Status;
}

/**

  This code loads capsules in boot time from the NV storage and processes them

  Before calling this function, the modules should call InitCapsuleStoragePointers

  @param [in] CapsuleHeaderArray Virtual pointer to an array of virtual pointers
                                 to the capsules being passed into update capsule.
  @param [in] CapsuleCount       Number of pointers to EFI_CAPSULE_HEADER in
                                 CaspuleHeaderArray.
  @param [in] ScatterGatherList  Physical pointer to a set of
                                 EFI_CAPSULE_BLOCK_DESCRIPTOR that describes the
                                 location in physical memory of a set of capsules.
  @param [in] CapsuleStorage     Type of storage to process capsule from.


  @retval EFI_SUCCESS           Valid capsule was passed. If
                                CAPSULE_FLAGS_PERSIT_ACROSS_RESET is not set, the
                                capsule has been successfully processed by the firmware.
**/
EFI_STATUS
EFIAPI
PersistCapsules (
  IN EFI_CAPSULE_HEADER   **CapsuleHeaderArray,
  IN UINTN                  CapsuleCount,
  IN EFI_PHYSICAL_ADDRESS   ScatterGatherList OPTIONAL
  )
{
  EFI_STATUS                  Status = EFI_SUCCESS;
  CapsuleStorageType          CapsuleStorage = CAPSULE_STORAGE_TYPE_UNKNOWN;

  //
  // Get the current capsule storage type to be used for persisting capsules
  //
  CapsuleStorage = (CapsuleStorageType)PcdGet16(PcdCapsuleStorageType);

  switch (CapsuleStorage)
  {
    case CAPSULE_STORAGE_TYPE_CAPSULE_RAW_FILE:
    {
      Status = PersistCapsulesToCapsuleRawFile(CapsuleHeaderArray,
                                               CapsuleCount);

      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "PersistCapsulesToCapsuleRawFile returned - %r!\n", Status));
      }
      break;
    }

    case CAPSULE_STORAGE_TYPE_CAPSULE_PARTITION:
    {
      Status = PersistCapsulesToCapsulePartition(CapsuleHeaderArray,
                                                 CapsuleCount);

      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "PersistCapsulesToCapsulePartition returned - %r!\n", Status));
      }
      break;
    }

    case CAPSULE_STORAGE_TYPE_RAM:
    {
      Status = PersistCapsulesToRAM(CapsuleHeaderArray,
                                    CapsuleCount,
                                    ScatterGatherList);

      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "PersistCapsulesToRAM returned - %r!\n", Status));
      }
      break;
    }

    case CAPSULE_STORAGE_TYPE_MASS_STORAGE:
    {
      Status = EFI_UNSUPPORTED;
      DEBUG ((DEBUG_ERROR, "Unsupported CapsuleStorageType (%d) received for persisting!\n", CapsuleStorage));

      break;
    }

    default:
    {
      Status = EFI_INVALID_PARAMETER;
      DEBUG ((DEBUG_ERROR, "Invalid CapsuleStorageType (%d) received for persisting!\n", CapsuleStorage));
    }
  }

  return Status;
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
  EFI_STATUS          Status = EFI_SUCCESS;
  CapsuleStorageType  CapsuleStorage= CAPSULE_STORAGE_TYPE_UNKNOWN;

  if (NULL == MaxiumCapsuleSize) {
    return EFI_INVALID_PARAMETER;
  }

  CapsuleStorage = (CapsuleStorageType)PcdGet16(PcdCapsuleStorageType);

  switch (CapsuleStorage)
  {
    case CAPSULE_STORAGE_TYPE_CAPSULE_RAW_FILE:
    {
      Status = GetMaxCapsuleSizeInCapsuleRawFile(MaxiumCapsuleSize);

      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "GetMaxCapsuleSizeInCapsuleRawFile returned - %r!\n", Status));
      }
      break;
    }

    case CAPSULE_STORAGE_TYPE_CAPSULE_PARTITION:
    {
      Status = GetMaxCapsuleSizeInCapsulePartition(MaxiumCapsuleSize);

      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "GetMaxCapsuleSizeInCapsulePartition returned - %r!\n", Status));
      }
      break;
    }

    case CAPSULE_STORAGE_TYPE_MASS_STORAGE:
    {
      Status = EFI_UNSUPPORTED;
      DEBUG ((DEBUG_ERROR, "Unsupported CapsuleStorageType (%d) received for max capsule-size query!\n", CapsuleStorage));

      break;
    }

    case CAPSULE_STORAGE_TYPE_RAM:
    {
      //
      //Check if the platform supports update capsule across a system reset
      //
      if (!FeaturePcdGet(PcdSupportUpdateCapsuleReset)) {
        Status = EFI_UNSUPPORTED;
      } else {
        *MaxiumCapsuleSize = PcdGet32(PcdMaxSizePopulateCapsule);
      }
      break;
    }

    default:
    {
      Status = EFI_INVALID_PARAMETER;
      DEBUG ((DEBUG_ERROR, "Invalid CapsuleStorageType (%d) received for max capsule-size query!\n", CapsuleStorage));
    }
  }

  return Status;
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
  EFI_STATUS                  Status = EFI_SUCCESS;

  switch (CapsuleStorage)
  {
    case CAPSULE_STORAGE_TYPE_CAPSULE_RAW_FILE:
    {
      Status = InitStoragePointersForCapsuleRawFile ();

      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "InitStoragePointersForCapsuleRawFile returned - %r!\n", Status));
      }
      break;
    }

    case CAPSULE_STORAGE_TYPE_CAPSULE_PARTITION:
    {
      Status = InitStoragePointersForCapsulePartition ();

      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "InitStoragePointersForCapsulePartition returned - %r!\n", Status));
      }
      break;
    }

    case CAPSULE_STORAGE_TYPE_MASS_STORAGE:
    case CAPSULE_STORAGE_TYPE_RAM:
    {
      Status = EFI_UNSUPPORTED;
      DEBUG ((DEBUG_ERROR, "Unsupported CapsuleStorageType (%d) received for initializing capsule storage pointers!\n", CapsuleStorage));

      break;
    }

    default:
    {
      Status = EFI_INVALID_PARAMETER;
      DEBUG ((DEBUG_ERROR, "Invalid CapsuleStorageType (%d) received for initializing capsule storage pointers!\n", CapsuleStorage));
    }
  }

  return Status;
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
  switch (CapsuleStorage)
  {
    case CAPSULE_STORAGE_TYPE_CAPSULE_RAW_FILE:
    {
      return UpdateCapsuleRawFileExists();
    }

    case CAPSULE_STORAGE_TYPE_CAPSULE_PARTITION:
    {
      return UpdateCapsulePartitionExists();
    }

    case CAPSULE_STORAGE_TYPE_MASS_STORAGE:
    {
      // EFI partition will always be there. We will detect the presence of capsules in later functions
      return TRUE;
    }

    case CAPSULE_STORAGE_TYPE_RAM:
    {
      DEBUG ((DEBUG_ERROR, "Unsupported CapsuleStorageType (%d) received for deleting a capsule!\n", CapsuleStorage));

      return FALSE;
    }

    default:
    {
      DEBUG ((DEBUG_ERROR, "Invalid CapsuleStorageType (%d) received for deleting a capsule!\n", CapsuleStorage));

      return FALSE;
    }
  }
}


/**
  Returns the capsule count in the current media used to persist capsules.

  @param[in] CapsuleCount       Type of the storage from which the capsules
                                are to be processed
  @param[in] CapsuleStorage     Type of storage to process capsule from.


  @retval EFI_SUCESS            Action completed successfully
  @retval EFI_INVALID_PARAMETER Invalid input parameter.

**/
EFI_STATUS
EFIAPI
GetPersistedCapsuleCount(
  IN UINT32                     *CapsuleCount,
  IN CapsuleStorageType          CapsuleStorage
  )
{
  EFI_STATUS                    Status = EFI_SUCCESS;

  if (NULL == CapsuleCount) {
    return EFI_INVALID_PARAMETER;
  }

  if (NULL == CapsuleStorageHeader) {
    DEBUG ((DEBUG_ERROR, "GetPersistedCapsuleCount called without initializing capsule storage pointers!\n"));
    return EFI_NOT_READY;
  }

  switch (CapsuleStorage)
  {
    case CAPSULE_STORAGE_TYPE_CAPSULE_RAW_FILE:
    {
      CAPSULE_RAW_FILE_GUID_TABLE  *CapsuleGuidTable = (CAPSULE_RAW_FILE_GUID_TABLE *) CapsuleStorageHeader;

      *CapsuleCount = CapsuleGuidTable->GuidCount;
      break;
    }

    case CAPSULE_STORAGE_TYPE_CAPSULE_PARTITION:
    {
      CAPSULE_PARTITION_GUID_TABLE  *CapsuleGuidTable = (CAPSULE_PARTITION_GUID_TABLE *) CapsuleStorageHeader;

      *CapsuleCount = CapsuleGuidTable->GuidCount;
      break;
    }

    case CAPSULE_STORAGE_TYPE_MASS_STORAGE:
    {
      Status = GetMassStorageCapsuleCount(CapsuleCount);

      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "GetMassStorageCapsuleCount returned - %r!\n", Status));
      }
      break;
    }

    case CAPSULE_STORAGE_TYPE_RAM:
    {
      // TODO: Giri: Can use the logic from HOB processing to create a function for this
      Status = EFI_UNSUPPORTED;
      DEBUG ((DEBUG_ERROR, "Unsupported CapsuleStorageType (%d) received in GetPersistedCapsuleCount!\n", CapsuleStorage));

      break;
    }

    default:
    {
      Status = EFI_INVALID_PARAMETER;
      DEBUG ((DEBUG_ERROR, "Invalid CapsuleStorageType (%d) received in GetPersistedCapsuleCount!\n", CapsuleStorage));

      break;
    }
  }

  return Status;
}


/**
  This code loads capsules in boot time from the NV storage and processes them
  in the same way as capsules in RAM.

  @param [in/out] CapsulePtrArray     Array of capsules with array length CapsuleArrayLen.
  @param [in/out] CapsuleOffsetArray  Array of capsules offset within the capsule storage.
  @param [in] CapsulePtrArray         Array of filenames from which the capsules was loaded.
  @param [in] CapsuleArrayLen         Length of the arrays CapsulePtrArray and CapsuleOffsetArray.
  @param [in] CapsuleStorage          Type of storage to process capsule from.


  @retval EFI_SUCCESS             UEFI Capsule Runtime Services are installed successfully.
  @retval EFI_INVALID_PARAMETER   Invalid paramter received
  @retval EFI_VOLUME_CORRUPTED    Capsule partition corrupted

**/
EFI_STATUS
EFIAPI
LoadCapsulesFromPersistedMedia (
  IN OUT VOID           **CapsulePtrArray,
  IN OUT UINT32          *CapsuleOffsetArray,
  IN OUT CHAR16         **CapsuleFileNameArray,
  IN UINT32               CapsulePtrArrayLen,
  IN CapsuleStorageType   CapsuleStorage
  )
{
  EFI_STATUS                    Status = EFI_SUCCESS;

  if (NULL == CapsulePtrArray) {
      Status = EFI_INVALID_PARAMETER;
      DEBUG ((DEBUG_ERROR, "Invalid value received for parameter CapsulePtrArray in LoadCapsulesFromPersistedMedia\n"));
      return Status;
  }

  if (NULL == CapsuleOffsetArray) {
      Status = EFI_INVALID_PARAMETER;
      DEBUG ((DEBUG_ERROR, "Invalid value received for parameter CapsuleOffsetArray in LoadCapsulesFromPersistedMedia\n"));
      return Status;
  }

  switch (CapsuleStorage)
  {
    case CAPSULE_STORAGE_TYPE_CAPSULE_RAW_FILE:
    {
      Status = LoadPersistedCapsulesFromCapsuleRawFile (
        CapsulePtrArray,
        CapsuleOffsetArray,
        CapsulePtrArrayLen
        );
      if (EFI_ERROR(Status)) {
        DEBUG ((DEBUG_ERROR, "LoadPersistedCapsulesFromCapsuleRawFile returned error - %r\n", Status));
        return Status;
      }

      break;
    }

    case CAPSULE_STORAGE_TYPE_CAPSULE_PARTITION:
    {
      Status = LoadPersistedCapsulesFromCapsulePartition (
        CapsulePtrArray,
        CapsuleOffsetArray,
        CapsulePtrArrayLen
        );
      if (EFI_ERROR(Status)) {
        DEBUG ((DEBUG_ERROR, "LoadPersistedCapsulesFromCapsuleRawFile returned error - %r\n", Status));
        return Status;
      }

      break;
    }

    case CAPSULE_STORAGE_TYPE_MASS_STORAGE:
    {
      Status = LoadMassStorageCapsules(
        CapsulePtrArray,
        CapsuleFileNameArray,
        CapsulePtrArrayLen
        );
      if (EFI_ERROR(Status)) {
        DEBUG ((DEBUG_ERROR, "LoadMassStorageCapsules returned error - %r\n", Status));
        return Status;
      }

      break;
    }

    case CAPSULE_STORAGE_TYPE_RAM:
    {
      Status = EFI_UNSUPPORTED;
      DEBUG ((DEBUG_ERROR, "Unsupported CapsuleStorageType (%d) received in LoadCapsulesFromPersistedMedia!\n", CapsuleStorage));

      break;
    }

    default:
    {
      Status = EFI_INVALID_PARAMETER;
      DEBUG ((DEBUG_ERROR, "Invalid CapsuleStorageType (%d) received in LoadCapsulesFromPersistedMedia!\n", CapsuleStorage));

      break;
    }
  }

  return Status;
}

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
  )
{
  EFI_STATUS                    Status = EFI_SUCCESS;

  if (!CapsulePersistedMediaExists(CapsuleStorage)) {
    return EFI_NO_MEDIA;
  }

  Status = GetPersistedCapsuleCount(&mCapsuleTotalNumber, CapsuleStorage);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "GetPersistedCapsuleCount returned - %r\n", Status));
    return Status;
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
  // Find all capsule images from capsule partition, copy it to local buffer and update capsule pointer
  //
  Status = LoadCapsulesFromPersistedMedia (
    mCapsulePtr,
    mCapsuleOffsetArray,
    mCapsuleFileNameArray,
    mCapsuleTotalNumber,
    CapsuleStorage
    );
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "LoadCapsulesFromPersistedMedia returned error - %r\n", Status));
    FreePool (mCapsulePtr);
    mCapsulePtr = NULL;
    FreePool (mCapsuleOffsetArray);
    mCapsuleOffsetArray = NULL;
    FreePool (mCapsuleStatusArray);
    mCapsuleStatusArray = NULL;
    FreePool (mCapsuleFileNameArray);
    mCapsuleFileNameArray = NULL;
    mCapsuleTotalNumber = 0;
    return Status;
  }

  return Status;
}

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
  )
{
  EFI_STATUS                    Status = EFI_SUCCESS;
  EFI_GUID                      UniqueCapsuleGuid;

  if (NULL == CapsuleHeader) {
      DEBUG ((DEBUG_ERROR, "Invalid value received for parameter CapsuleHeader in DeleteCapsule\n"));
      return EFI_INVALID_PARAMETER;
  }

  switch (CapsuleStorage)
  {
    case CAPSULE_STORAGE_TYPE_CAPSULE_RAW_FILE:
    {
      Status = GetUniqueCapsuleGuid (CapsuleHeader, &UniqueCapsuleGuid);
      if (EFI_ERROR (Status)) {
        DEBUG ((EFI_D_ERROR, "Failed to get unique GUID of capsule to delete the capsule from capsule raw file. Returning error %r\n", Status));
        return Status;
      }

      Status = DeletePersistedCapsuleFromCapsuleRawFile (
        &UniqueCapsuleGuid,
        CapsuleOffset
        );
      if (EFI_ERROR(Status)) {
        DEBUG ((DEBUG_ERROR, "DeleteCapsuleFromCapsuleRawFile returned error - %r\n", Status));
        return Status;
      }

      break;
    }

    case CAPSULE_STORAGE_TYPE_CAPSULE_PARTITION:
    {
      Status = GetUniqueCapsuleGuid (CapsuleHeader, &UniqueCapsuleGuid);
      if (EFI_ERROR (Status)) {
        DEBUG ((EFI_D_ERROR, "Failed to get unique GUID of capsule to delete the capsule from capsule partition. Returning error %r\n", Status));
        return Status;
      }

      Status = DeletePersistedCapsuleFromCapsulePartition (
        &UniqueCapsuleGuid,
        CapsuleOffset
        );
      if (EFI_ERROR(Status)) {
        DEBUG ((DEBUG_ERROR, "DeleteCapsuleFromCapsulePartition returned error - %r\n", Status));
        return Status;
      }

      break;
    }

    case CAPSULE_STORAGE_TYPE_MASS_STORAGE:
    {
      Status = DeleteMassStorageCapsule(
        CapsuleFileName
        );
      if (EFI_ERROR(Status)) {
        DEBUG ((DEBUG_ERROR, "DeleteMassStorageCapsule returned error - %r\n", Status));
        return Status;
      }

      // TODO: Giri: UEFI Spec says that failure to delete a mass-storage capsule is not a reporable error. Confirm how we should handle errors in this and other capsule-storage types
      break;
    }

    case CAPSULE_STORAGE_TYPE_RAM:
    {
      Status = EFI_UNSUPPORTED;
      DEBUG ((DEBUG_ERROR, "Unsupported CapsuleStorageType (%d) received in DeleteCapsule!\n", CapsuleStorage));

      break;
    }

    default:
    {
      Status = EFI_INVALID_PARAMETER;
      DEBUG ((DEBUG_ERROR, "Invalid CapsuleStorageType (%d) received in DeleteCapsule!\n", CapsuleStorage));

      break;
    }
  }

  return Status;
}

/**
  De-initialize the capsule pointer memory for the given storage media.
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
  )
{
  EFI_STATUS                    Status = EFI_SUCCESS;

  if (NULL == CapsulePtr) {
      Status = EFI_INVALID_PARAMETER;
      DEBUG ((DEBUG_ERROR, "Invalid value received for parameter CapsulePtr in DeInitCapsulePtrMemory\n"));
      return Status;
  }

  switch (CapsuleStorage)
  {
    case CAPSULE_STORAGE_TYPE_CAPSULE_RAW_FILE:
    {
      Status = DeInitCapsulePtrMemoryForCapsuleRawFile (
        CapsulePtr
        );
      if (EFI_ERROR(Status)) {
        DEBUG ((DEBUG_ERROR, "DeInitCapsulePtrMemoryForCapsuleRawFile returned error - %r\n", Status));
        return Status;
      }

      break;
    }

    case CAPSULE_STORAGE_TYPE_CAPSULE_PARTITION:
    {
      Status = DeInitCapsulePtrMemoryForCapsulePartition (
        CapsulePtr
        );
      if (EFI_ERROR(Status)) {
        DEBUG ((DEBUG_ERROR, "DeInitCapsulePtrMemoryForCapsulePartition returned error - %r\n", Status));
        return Status;
      }

      break;
    }

    case CAPSULE_STORAGE_TYPE_MASS_STORAGE:
    {
      Status = DeInitCapsulePtrMemoryGeneric (
        CapsulePtr
        );
      if (EFI_ERROR(Status)) {
        DEBUG ((DEBUG_ERROR, "DeInitCapsulePtrMemoryGeneric returned error - %r\n", Status));
        return Status;
      }

      break;
    }

    case CAPSULE_STORAGE_TYPE_RAM:
    {
      // Nothing to do than clearing the pointer as the memory points to a
      // HOB content and we cannot deinitialize the HOB memory as we dont own it
      *CapsulePtr = NULL;

      break;
    }

    default:
    {
      Status = EFI_INVALID_PARAMETER;
      DEBUG ((DEBUG_ERROR, "Invalid CapsuleStorageType (%d) received in DeleteCapsule!\n", CapsuleStorage));

      break;
    }
  }

  return Status;
}

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
  )
{
  if (NULL == CapsulePtr) {
    DEBUG ((DEBUG_ERROR, "Invalid value received for parameter CapsulePtr in DeInitCapsulePtrMemoryGeneric\n"));
    return EFI_INVALID_PARAMETER;
  }

  if (NULL == *CapsulePtr) {
    return EFI_SUCCESS;
  }

  FreePool(*CapsulePtr);
  *CapsulePtr = NULL;

  return EFI_SUCCESS;
}

/******************************************************************
 * Below functions are to be used incase we use RAM in future to  *
 * persist capsule                                                *
 ******************************************************************/

/**
 *
 * Check if the ScatterGatherHeader
 *  was written by a previous call
 *  to UpdateCapsule by running
 *  a CRC32 check.
 *
 */
EFI_STATUS
EFIAPI
ValidateSGHeader(
  IN SCATTERGATHERLIST_ARRAY_HEADER *ScatterGatherArrayHeader
  )
{
  EFI_STATUS Status;

  UINT32 SavedCrc32;

  if(ScatterGatherArrayHeader == NULL)
  {
    return EFI_INVALID_PARAMETER;
  }

  SavedCrc32 = ScatterGatherArrayHeader->Crc32;
  ScatterGatherArrayHeader->Crc32 = 0x0;

  Status = CalculateCrc32(ScatterGatherArrayHeader,
                          sizeof(SCATTERGATHERLIST_ARRAY_HEADER),
                          &ScatterGatherArrayHeader->Crc32);

  if(EFI_ERROR(Status))
  {
    goto DoneValidating;
  }

  if(SavedCrc32 != ScatterGatherArrayHeader->Crc32)
  {
    Status = EFI_CRC_ERROR;
    goto DoneValidating;
  }

  //See if there are any ScatterGatherLists to process
  // and Make sure the ScatterGather array offset is valid.
  if(ScatterGatherArrayHeader->PointerCount < 1 ||
     ScatterGatherArrayHeader->ArrayOffset < sizeof(SCATTERGATHERLIST_ARRAY_HEADER))
  {
    Status = EFI_UNSUPPORTED;
  }

DoneValidating:

  //Restore SavedCrc32.
  ScatterGatherArrayHeader->Crc32 = SavedCrc32;
  return Status;
}

/**
 * Check to see if previous
 *  OS called UpdateCapsule
 *  to persist a capsule across
 *  reset.
 *
 *
 * @return VOID
 */
VOID CheckScatterGatherListArray(
  IN SCATTERGATHERLIST_ARRAY_HEADER *ScatterGatherArrayHeader,
  IN BOOLEAN                        PersistCapsule
  )
{
  EFI_PHYSICAL_ADDRESS *ScatterGatherArray;

  EFI_STATUS Status;

  VOID *MemBase = NULL;
  UINTN MemSize = 0;

  DEBUG((EFI_D_INFO,"+CheckScatterGatherListArray\n"));

  if(ScatterGatherArrayHeader!= NULL)
  {

    if(!EFI_ERROR(ValidateSGHeader(ScatterGatherArrayHeader)))
    {

      ScatterGatherArray =
        (EFI_PHYSICAL_ADDRESS*)((UINT8*)ScatterGatherArrayHeader +
                                ScatterGatherArrayHeader->ArrayOffset);


      //First find out the size of the capsules
      // we have been passed.
      Status = CapsuleDataCoalesce(NULL,
                                   ScatterGatherArray,
                                   &MemBase,
                                   &MemSize);

      if(Status == EFI_BUFFER_TOO_SMALL)
      {
        // Allocate Memory for coalescing the capsules.
        if (PersistCapsule)
        {
          Status = gBS->AllocatePool(EfiBootServicesData,
                                     MemSize,
                                     (VOID**)&MemBase);
        }
        else
        {
          Status = gBS->AllocatePool(EfiRuntimeServicesData,
                                     MemSize,
                                     (VOID**)&MemBase);
        }

        if(!EFI_ERROR(Status))
        {

          DEBUG((EFI_D_INFO,"ScatterGatherList is 0x%llx, preparing to coalesce capsules.\n",
                 (UINT64)*ScatterGatherArray));

          Status = CapsuleDataCoalesce(NULL,
                                       ScatterGatherArray,
                                       &MemBase,
                                       &MemSize);

          if(EFI_ERROR(Status))
          {
            DEBUG((EFI_D_ERROR,"Could not coalesce capsule, Status: %r\n",Status));
            goto InitializeHeader;
          }

          // Process coalesced capsules
          if (PersistCapsule)
          {
            Status = PersistCoalescedCapsules((VOID*)MemBase);
          }
          else
          {
            Status = ProcessCoalescedCapsules((VOID*)MemBase);
          }

          if(EFI_ERROR(Status))
          {
            DEBUG((EFI_D_ERROR,"Could not process coalesced capsule(s), Status: %r",Status));
            goto InitializeHeader;
          }

          DEBUG((EFI_D_INFO,"Capsules processsed successfully!\n"));

        }
        else
        {
          DEBUG((EFI_D_ERROR,"Could not allocate memory for capsule coalescing, Status: %r",Status));
        }

      }

    }
    else
    {
      DEBUG((EFI_D_INFO,"ScatterGather header CRC failed. Will not process capsules in RAM.\n"));
    }

InitializeHeader:
    //Done with the ScatterGatherListArray, re-initialize the header.
    DEBUG((EFI_D_INFO,"Initializing ScatterGather header.\n"));
    ScatterGatherArrayHeader->Revision = SCATTERGATHERLIST_ARRAY_HEADER_REVISION;
    ScatterGatherArrayHeader->PointerCount = 0x0;
    ScatterGatherArrayHeader->ArrayOffset = sizeof(SCATTERGATHERLIST_ARRAY_HEADER);

    CopyMem(&(ScatterGatherArrayHeader->ScatterGatherHeaderGuid),
            (VOID*)&gQcomScatterGatherListHeaderGuid,
            sizeof(EFI_GUID));

    ScatterGatherArrayHeader->Crc32 = 0x0;
    Status = CalculateCrc32(ScatterGatherArrayHeader,
                            sizeof(SCATTERGATHERLIST_ARRAY_HEADER),
                            &ScatterGatherArrayHeader->Crc32);

  }

  DEBUG((EFI_D_INFO,"-CheckScatterGatherListArray\n"));

  return;
}

/**
  Flush persisted capsules in the given storage media

  This function clears the pointer after de-initialization

  @param[in]  CapsuleStorage   Type of storage to process capsule from.

  @retval EFI_SUCESS            Action completed successfully
  @retval EFI_INVALID_PARAMETER Invalid parameter.
  @retval EFI_UNSUPPORTED       Capsule image is not supported by the firmware.
  @retval EFI_NO_MEDIA          Capsule storage media not found.
**/
EFI_STATUS
EFIAPI
FlushPersistedCapsules (
  IN CapsuleStorageType   CapsuleStorage
  )
{
  EFI_STATUS                    Status = EFI_SUCCESS;
  EFI_STATUS                    RetStatus = EFI_SUCCESS;
  UINT32                        Index = 0;

  if (!CapsulePersistedMediaExists(CapsuleStorage)) {
    return EFI_NO_MEDIA;
  }

  switch (CapsuleStorage)
  {
    case CAPSULE_STORAGE_TYPE_CAPSULE_RAW_FILE:
    {
      Status = InitCapsuleRawFileGuidTable ((CAPSULE_RAW_FILE_GUID_TABLE *)CapsuleStorageHeader);
      if (EFI_ERROR (Status)) {
        DEBUG ((EFI_D_ERROR, "Failed to flush capsule raw file GUID table. Status: %r\n", Status));
        return Status;
      }

      return Status;
    }

    case CAPSULE_STORAGE_TYPE_CAPSULE_PARTITION:
    {
      Status = InitCapsulePartitionGuidTable ((CAPSULE_PARTITION_GUID_TABLE *)CapsuleStorageHeader);
      if (EFI_ERROR (Status)) {
        DEBUG ((EFI_D_ERROR, "Failed to flush capsule raw file GUID table. Status: %r\n", Status));
        return Status;
      }

      return Status;
    }

    case CAPSULE_STORAGE_TYPE_MASS_STORAGE:
    {
      for (Index = 0; Index < mCapsuleTotalNumber; Index++) {
        if ((NULL != mCapsuleFileNameArray [Index])) {
          RetStatus = DeleteMassStorageCapsule(
            mCapsuleFileNameArray [Index]
            );
          if (EFI_ERROR(Status)) {
            DEBUG ((DEBUG_WARN, "Failed to delete capsule file %s. Status: %r. Ignoring and proceeding.\n", mCapsuleFileNameArray [Index], RetStatus));
            // We will record and report the first error encountered
            Status = (!EFI_ERROR(Status) ? RetStatus : Status);
          }
        }
      }

      return Status;
    }

    case CAPSULE_STORAGE_TYPE_RAM:
    {
      DEBUG ((DEBUG_ERROR, "Unsupported CapsuleStorageType (%d) received in FlushPersistedCapsules!\n", CapsuleStorage));

      return EFI_UNSUPPORTED;
    }

    default:
    {
      DEBUG ((DEBUG_ERROR, "Invalid CapsuleStorageType (%d) received in FlushPersistedCapsules!\n", CapsuleStorage));

      return EFI_INVALID_PARAMETER;
    }
  }
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
  CapsuleStorageType CapsuleStorage = CAPSULE_STORAGE_TYPE_UNKNOWN;

  if (NULL == CapsuleSizeInStorageMedia) {
    return EFI_INVALID_PARAMETER;
  }

  CapsuleStorage = (CapsuleStorageType)PcdGet16(PcdCapsuleStorageType);

  if (!CapsulePersistedMediaExists(CapsuleStorage)) {
    return EFI_NO_MEDIA;
  }

  switch (CapsuleStorage)
  {
    case CAPSULE_STORAGE_TYPE_CAPSULE_RAW_FILE:
    case CAPSULE_STORAGE_TYPE_MASS_STORAGE:
    {
      *CapsuleSizeInStorageMedia = GetCapsuleSizeInCapsuleRawFile (CapsuleImageSize);

      return EFI_SUCCESS;
    }

    case CAPSULE_STORAGE_TYPE_CAPSULE_PARTITION:
    {
      *CapsuleSizeInStorageMedia = GetCapsuleSizeInCapsulePartition (CapsuleImageSize);

      return EFI_SUCCESS;
    }

    case CAPSULE_STORAGE_TYPE_RAM:
    {
      DEBUG ((DEBUG_ERROR, "Unsupported CapsuleStorageType (%d) received in FlushPersistedCapsules!\n", CapsuleStorage));

      return EFI_UNSUPPORTED;
    }

    default:
    {
      DEBUG ((DEBUG_ERROR, "Invalid CapsuleStorageType (%d) received in FlushPersistedCapsules!\n", CapsuleStorage));

      return EFI_INVALID_PARAMETER;
    }
  }
}

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
  IN EFI_HANDLE         Handle,
  OUT BOOLEAN          *ResetRequired
  )
{
  EFI_STATUS                                    Status = EFI_SUCCESS;
  EFI_FIRMWARE_MANAGEMENT_PROTOCOL             *Fmp = NULL;
  UINTN                                         ImageInfoSize = 0;
  EFI_FIRMWARE_IMAGE_DESCRIPTOR                *FmpImageInfoBuf = NULL;
  EFI_FIRMWARE_IMAGE_DESCRIPTOR                *FmpImageInfoPtr = NULL;
  UINT32                                        FmpImageInfoDescriptorVer = 0;
  UINT8                                         FmpImageInfoCount = 0;
  UINTN                                         DescriptorSize = 0;
  UINT32                                        PackageVersion = 0;
  CHAR16                                       *PackageVersionName = NULL;
  UINT8                                         Index = 0;

  if ((NULL == Handle) ||
      (NULL == ResetRequired))
  {
    return EFI_INVALID_PARAMETER;
  }

  Status = gBS->HandleProtocol(
                  Handle,
                  &gEfiFirmwareManagementProtocolGuid,
                  (VOID **)&Fmp
                  );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  ImageInfoSize = 0;
  Status = Fmp->GetImageInfo (
                  Fmp,
                  &ImageInfoSize,
                  NULL,
                  NULL,
                  NULL,
                  NULL,
                  NULL,
                  NULL
                  );
  if (Status != EFI_BUFFER_TOO_SMALL) {
    return Status;
  }

  FmpImageInfoBuf = AllocateZeroPool (ImageInfoSize);
  if (FmpImageInfoBuf == NULL) {
    return Status;
  }

  PackageVersionName = NULL;
  Status = Fmp->GetImageInfo (
                  Fmp,
                  &ImageInfoSize,               // ImageInfoSize
                  FmpImageInfoBuf,              // ImageInfo
                  &FmpImageInfoDescriptorVer,   // DescriptorVersion
                  &FmpImageInfoCount,           // DescriptorCount
                  &DescriptorSize,              // DescriptorSize
                  &PackageVersion,              // PackageVersion
                  &PackageVersionName           // PackageVersionName
                  );
  if (EFI_ERROR(Status)) {
    FreePool(FmpImageInfoBuf);
    return Status;
  }

  // Fill default value
  *ResetRequired = FALSE;

  for (Index = 0; Index < FmpImageInfoCount; Index++)
  {
    FmpImageInfoPtr = (FmpImageInfoBuf + (Index * DescriptorSize));

    if ((FmpImageInfoPtr->AttributesSupported & IMAGE_ATTRIBUTE_RESET_REQUIRED) &&
        (FmpImageInfoPtr->AttributesSetting & IMAGE_ATTRIBUTE_RESET_REQUIRED)) {
      *ResetRequired = TRUE;
      break;
    }
  }

  return Status;
}

/**
  Returns the unique GUID of the capsule.

  If there are multiple payloads in the capsule, the GUID of the first payload
  will be returned. In Windows system, a capsule can only contain payloads
  orresponding to one type of firmware.

  @param[in] CapsuleHeader      A pointer to EFI_CAPSULE_HEADER
  @param[in] UniqueCapsuleGuid  Unique GUID of the capsule

  @retval EFI_SUCESS            Action completed successfully
  @retval EFI_INVALID_PARAMETER Invalid input parameter.
**/
EFI_STATUS
GetUniqueCapsuleGuid (
  IN EFI_CAPSULE_HEADER         *CapsuleHeader,
  OUT EFI_GUID                  *UniqueCapsuleGuid
  )
{
  EFI_STATUS                                    Status = EFI_SUCCESS;
  EFI_FIRMWARE_MANAGEMENT_CAPSULE_HEADER       *FmpCapsuleHeader = NULL;
  EFI_FIRMWARE_MANAGEMENT_CAPSULE_IMAGE_HEADER *ImageHeader = NULL;
  UINT64                                       *ItemOffsetList = NULL;
  UINTN                                         PayloadItemIndex = 0;

  if (NULL == UniqueCapsuleGuid) {
    return EFI_INVALID_PARAMETER;
  }

  if (NULL == CapsuleHeader) {
    return EFI_INVALID_PARAMETER;
  }

  if (IsFmpCapsuleGuid(&CapsuleHeader->CapsuleGuid)) {
    FmpCapsuleHeader = (EFI_FIRMWARE_MANAGEMENT_CAPSULE_HEADER *) ((UINT8 *) CapsuleHeader + CapsuleHeader->HeaderSize);

    if (FmpCapsuleHeader->PayloadItemCount == 0) {
      return EFI_NOT_FOUND;
    }

    PayloadItemIndex = FmpCapsuleHeader->EmbeddedDriverCount;

    ItemOffsetList = (UINT64 *)(FmpCapsuleHeader + 1);
    ImageHeader  = (EFI_FIRMWARE_MANAGEMENT_CAPSULE_IMAGE_HEADER *)((UINT8 *)FmpCapsuleHeader + ItemOffsetList[PayloadItemIndex]);

    CopyMem(UniqueCapsuleGuid, &ImageHeader->UpdateImageTypeId, sizeof(EFI_GUID));
  } else {
    CopyMem(UniqueCapsuleGuid, &CapsuleHeader->CapsuleGuid, sizeof(EFI_GUID));
  }

  return Status;
}

/**
  Returns if OsIndications variable has EFI_OS_INDICATIONS_FILE_CAPSULE_DELIVERY_SUPPORTED
  flag enabled.

  @retval TRUE  It is a system FMP.
  @retval FALSE It is a device FMP.
**/
BOOLEAN
IsMassStorageCapsuleFlagEnabled (
  )
{
  UINT64 *OsIndications = NULL;
  UINTN   Size          = 0;

  OsIndications = BdsLibGetVariableAndSize (L"OsIndications", &gEfiGlobalVariableGuid, &Size);

  if (OsIndications == NULL) {
    return FALSE;
  }

  if ((*OsIndications & EFI_OS_INDICATIONS_FILE_CAPSULE_DELIVERY_SUPPORTED) == 0) {
    FreePool (OsIndications);
    return FALSE;
  }

  // Do not clear the variable here. Will be cleared while processing the capsules

  FreePool (OsIndications);

  return TRUE;
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
  EFI_STATUS Status;
  UINT64     OsIndicationSupport;
  UINT64     OsIndication;
  UINTN      DataSize;
  UINT32     Attributes;

  //
  // OS indicater support variable
  //
  OsIndicationSupport =
    (EFI_OS_INDICATIONS_FILE_CAPSULE_DELIVERY_SUPPORTED |
     EFI_OS_INDICATIONS_FMP_CAPSULE_SUPPORTED |
     EFI_OS_INDICATIONS_CAPSULE_RESULT_VAR_SUPPORTED);
  Status = gRT->SetVariable (
                  L"OsIndicationsSupported",
                  &gEfiGlobalVariableGuid,
                  EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
                  sizeof(UINT64),
                  &OsIndicationSupport
                  );
  ASSERT_EFI_ERROR (Status);

  //
  // If OsIndications is invalid, remove it.
  // Invalid case
  //   1. Data size != UINT64
  //   2. OsIndication value inconsistence
  //   3. OsIndication attribute inconsistence
  //
  OsIndication = 0;
  Attributes = 0;
  DataSize = sizeof(UINT64);
  Status = gRT->GetVariable (
                  L"OsIndications",
                  &gEfiGlobalVariableGuid,
                  &Attributes,
                  &DataSize,
                  &OsIndication
                  );

  if (!EFI_ERROR(Status)) {
    if (DataSize != sizeof(UINT64) ||
        (OsIndication & ~OsIndicationSupport) != 0 ||
        Attributes != (EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_NON_VOLATILE)){

      DEBUG ((EFI_D_ERROR, "Unformalized OsIndications variable exists. Delete it\n"));
      Status = gRT->SetVariable (
                      L"OsIndications",
                      &gEfiGlobalVariableGuid,
                      Attributes,
                      0,
                      &OsIndication
                      );
      ASSERT_EFI_ERROR (Status);
    }
  }
}

