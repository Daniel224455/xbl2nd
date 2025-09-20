/** @file
*
*  Copyright (c) 2014-2015, 2017-2019 Qualcomm Technologies, Inc.  All Rights Reserved.
*  Portions Copyright (c) 2006 - 2016, Intel Corporation. All rights reserved.<BR>
*  
*  This program and the accompanying materials                          
*  are licensed and made available under the terms and conditions of the BSD License         
*  which accompanies this distribution.  The full text of the license may be found at        
*  http://opensource.org/licenses/bsd-license.php                                            
*
*  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,                     
*  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.             
*
**/


/*=============================================================================
                              EDIT HISTORY


 when       who     what, where, why
 --------   ---     -----------------------------------------------------------
 04/25/19   am      Added function to check for change in FW regions in UEFI memory map
 08/07/18   yw      disable hw wdog before image starts in case the image disables interrupt and cuases reset
 05/05/17   sj      Added status code reporting for OS load and start
 02/01/15   bh      Fix KW errors
 09/29/15   bh      Clean serial logs
 04/23/15   bh      Fix KW errors
 03/10/15   bh      Fix KW errors
 02/04/15   bh      Fix KW errors
 12/01/14   bh      Clean up comments
 11/06/14   bh      Add WorkingDevicePath for harddrive device paths in boot option
 10/24/14   bh      Initial branch from ArmPkg with edits

=============================================================================*/


#include "QcomBdsLibInternal.h"
#include <Protocol/DiskIo.h>
#include <Protocol/LoadedImage.h>
#include <Protocol/SimpleNetwork.h>
#include <Protocol/EFIQcomWDog.h>
#include <Library/GenericBdsLib.h>
#include <Library/QcomTargetLib.h>
#include <Library/ReportStatusCodeLib.h>
#include <Library/UefiCfgLib.h>

#define IS_DEVICE_PATH_NODE(node,type,subtype) (((node)->Type == (type)) && ((node)->SubType == (subtype)))
#define NUMBER_OF_EXTRA_DESCRIPTORS            0x4
#define IS_MEMTYPE_NONFW(MemType)              ((MemType == EfiLoaderCode)          ||\
                                                (MemType == EfiLoaderData)          ||\
                                                (MemType == EfiBootServicesCode)    ||\
                                                (MemType == EfiBootServicesData)    ||\
                                                (MemType == EfiConventionalMemory)  ||\
                                                (MemType == EfiPersistentMemory))

#define IS_MEMTYPE_FW(MemType)                 ((MemType == EfiRuntimeServicesCode)     ||\
                                                (MemType == EfiRuntimeServicesData)     ||\
                                                (MemType == EfiReservedMemoryType)      ||\
                                                (MemType == EfiACPIMemoryNVS)           ||\
                                                (MemType == EfiUnusableMemory)          ||\
                                                (MemType == EfiPalCode)                 ||\
                                                (MemType == EfiACPIReclaimMemory)       ||\
                                                (MemType == EfiMemoryMappedIOPortSpace) ||\
                                                (MemType == EfiMemoryMappedIO))

typedef struct {
  EFI_PHYSICAL_ADDRESS RegionBaseAddress;
  UINT32               RegionSize;
  UINT32               RegionType;
}RegionEntry;

// Extract the FilePath from the Device Path
CHAR16*
BdsExtractFilePathFromDevicePath (
  IN  CONST CHAR16    *StrDevicePath,
  IN  UINTN           NumberDevicePathNode
  )
{
  UINTN       Node;
  CHAR16      *Str;

  Str = (CHAR16*)StrDevicePath;
  Node = 0;
  while ((Str != NULL) && (*Str != L'\0') && (Node < NumberDevicePathNode)) {
    if ((*Str == L'/') || (*Str == L'\\')) {
        Node++;
    }
    Str++;
  }
  if (Str == NULL) {
    return NULL;
  }
  else if (*Str == L'\0') {
    return NULL;
  } else {
    return Str;
  }
}

BOOLEAN
BdsIsRemovableHd (
  IN  EFI_DEVICE_PATH*  DevicePath
  )
{
  return IS_DEVICE_PATH_NODE (DevicePath, MEDIA_DEVICE_PATH, MEDIA_HARDDRIVE_DP);
}

EFI_STATUS
BdsGetDeviceHd (
  IN  EFI_DEVICE_PATH*  RemovableDevicePath,
  OUT EFI_HANDLE*       DeviceHandle,
  OUT EFI_DEVICE_PATH** NewDevicePath
  )
{
  EFI_STATUS                    Status;
  UINTN                         Index;
  UINTN                         PartitionHandleCount;
  EFI_HANDLE                    *PartitionBuffer;
  EFI_DEVICE_PATH*              PartitionDevicePath;
  EFI_DEVICE_PATH*              TmpDevicePath;
  HARDDRIVE_DEVICE_PATH*        HardDriveDevicePath1;
  HARDDRIVE_DEVICE_PATH*        HardDriveDevicePath2;

  // Get all the DiskIo handles
  PartitionHandleCount = 0;
  Status = gBS->LocateHandleBuffer (ByProtocol, &gEfiDiskIoProtocolGuid, NULL, &PartitionHandleCount, &PartitionBuffer);
  if (EFI_ERROR (Status) || (PartitionHandleCount == 0)) {
    return Status;
  }

  // Check if one of the handles matches the Hard Disk Description
  for (Index = 0; Index < PartitionHandleCount; Index++) {
    Status = gBS->HandleProtocol (PartitionBuffer[Index], &gEfiDevicePathProtocolGuid, (VOID **) &PartitionDevicePath);
    if (!EFI_ERROR (Status)) {
      TmpDevicePath = PartitionDevicePath;
      while (!IsDevicePathEnd (TmpDevicePath)) {
        // Check if the Device Path node is a HD Removable device Path node
        if (BdsIsRemovableHd (TmpDevicePath)) {
          HardDriveDevicePath1 = (HARDDRIVE_DEVICE_PATH*)RemovableDevicePath;
          HardDriveDevicePath2 = (HARDDRIVE_DEVICE_PATH*)TmpDevicePath;
          if ((HardDriveDevicePath1->SignatureType == HardDriveDevicePath2->SignatureType) &&
              (CompareGuid ((EFI_GUID *)HardDriveDevicePath1->Signature, (EFI_GUID *)HardDriveDevicePath2->Signature) == TRUE) &&
              (HardDriveDevicePath1->PartitionNumber == HardDriveDevicePath2->PartitionNumber))
          {
            *DeviceHandle = PartitionBuffer[Index];
            // Add the additional original Device Path Nodes (eg: FilePath Device Path Node) to the new Device Path
            *NewDevicePath = AppendDevicePath (PartitionDevicePath, NextDevicePathNode (RemovableDevicePath));
            return EFI_SUCCESS;
          }
        }
        TmpDevicePath = NextDevicePathNode (TmpDevicePath);
      }

    }
  }

  return EFI_NOT_FOUND;
}

typedef BOOLEAN
(*BDS_IS_REMOVABLE) (
  IN  EFI_DEVICE_PATH*  DevicePath
  );

typedef EFI_STATUS
(*BDS_GET_DEVICE) (
  IN  EFI_DEVICE_PATH*  RemovableDevicePath,
  OUT EFI_HANDLE*       DeviceHandle,
  OUT EFI_DEVICE_PATH** DevicePath
  );

typedef struct {
  BDS_IS_REMOVABLE    IsRemovable;
  BDS_GET_DEVICE      GetDevice;
} BDS_REMOVABLE_DEVICE_SUPPORT;

BDS_REMOVABLE_DEVICE_SUPPORT  RemovableDeviceSupport[] = {
  { BdsIsRemovableHd, BdsGetDeviceHd },
};

STATIC
BOOLEAN
IsRemovableDevice (
  IN  EFI_DEVICE_PATH*  DevicePath
  )
{
  UINTN             Index;
  EFI_DEVICE_PATH*  TmpDevicePath;

  TmpDevicePath = DevicePath;
  while (!IsDevicePathEnd (TmpDevicePath)) {
    for (Index = 0; Index < sizeof (RemovableDeviceSupport) / sizeof (BDS_REMOVABLE_DEVICE_SUPPORT); Index++) {
      if (RemovableDeviceSupport[Index].IsRemovable (TmpDevicePath)) {
        return TRUE;
      }
    }
    TmpDevicePath = NextDevicePathNode (TmpDevicePath);
  }

  return FALSE;
}

STATIC
EFI_STATUS
TryRemovableDevice (
  IN  EFI_DEVICE_PATH*  DevicePath,
  OUT EFI_HANDLE*       DeviceHandle,
  OUT EFI_DEVICE_PATH** NewDevicePath
  )
{
  EFI_STATUS        Status;
  UINTN             Index;
  EFI_DEVICE_PATH*  TmpDevicePath;
  BDS_REMOVABLE_DEVICE_SUPPORT* RemovableDevice;
  EFI_DEVICE_PATH* RemovableDevicePath;
  BOOLEAN         RemovableFound;

  RemovableDevice     = NULL;
  RemovableDevicePath = NULL;
  RemovableFound      = FALSE;
  TmpDevicePath       = DevicePath;

  while (!IsDevicePathEnd (TmpDevicePath) && !RemovableFound) {
    for (Index = 0; Index < sizeof (RemovableDeviceSupport) / sizeof (BDS_REMOVABLE_DEVICE_SUPPORT); Index++) {
      RemovableDevice = &RemovableDeviceSupport[Index];
      if (RemovableDevice->IsRemovable (TmpDevicePath)) {
        RemovableDevicePath = TmpDevicePath;
        RemovableFound = TRUE;
        break;
      }
    }
    TmpDevicePath = NextDevicePathNode (TmpDevicePath);
  }

  if (!RemovableFound) {
    return EFI_NOT_FOUND;
  }

  // Search into the current started drivers
  Status = RemovableDevice->GetDevice (RemovableDevicePath, DeviceHandle, NewDevicePath);
  if (Status == EFI_NOT_FOUND) {
    // Connect all the drivers
    BdsConnectAllDrivers ();

    // Search again into all the drivers
    Status = RemovableDevice->GetDevice (RemovableDevicePath, DeviceHandle, NewDevicePath);
  }

  return Status;
}

/**
  Connect a Device Path and return the handle of the driver that support this DevicePath

  @param  DevicePath            Device Path of the File to connect
  @param  Handle                Handle of the driver that support this DevicePath
  @param  RemainingDevicePath   Remaining DevicePath nodes that do not match the driver DevicePath

  @retval EFI_SUCCESS           A driver that matches the Device Path has been found
  @retval EFI_NOT_FOUND         No handles match the search.
  @retval EFI_INVALID_PARAMETER DevicePath or Handle is NULL

**/
EFI_STATUS
BdsConnectDevicePath (
  IN  EFI_DEVICE_PATH_PROTOCOL* DevicePath,
  OUT EFI_HANDLE                *Handle,
  OUT EFI_DEVICE_PATH_PROTOCOL  **RemainingDevicePath
  )
{
  EFI_DEVICE_PATH*            Remaining;
  EFI_DEVICE_PATH*            NewDevicePath;
  EFI_STATUS                  Status;

  if ((DevicePath == NULL) || (Handle == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  do {
    Remaining = DevicePath;
    // The LocateDevicePath() function locates all devices on DevicePath that support Protocol and returns
    // the handle to the device that is closest to DevicePath. On output, the device path pointer is modified
    // to point to the remaining part of the device path
    Status = gBS->LocateDevicePath (&gEfiDevicePathProtocolGuid, &Remaining, Handle);
    if (!EFI_ERROR (Status)) {
      // Recursive = FALSE: We do not want to start all the device tree
      Status = gBS->ConnectController (*Handle, NULL, Remaining, FALSE);
    }

    /*// We need to check if RemainingDevicePath does not point on the last node. Otherwise, calling
    // NextDevicePathNode () will return an undetermined Device Path Node
    if (!IsDevicePathEnd (RemainingDevicePath)) {
      RemainingDevicePath = NextDevicePathNode (RemainingDevicePath);
    }*/
  } while (!EFI_ERROR (Status) && !IsDevicePathEnd (Remaining));

  if (!EFI_ERROR (Status)) {
    // Now, we have got the whole Device Path connected, call again ConnectController to ensure all the supported Driver
    // Binding Protocol are connected (such as DiskIo and SimpleFileSystem)
    Remaining = DevicePath;
    Status = gBS->LocateDevicePath (&gEfiDevicePathProtocolGuid, &Remaining, Handle);
    if (!EFI_ERROR (Status)) {
      Status = gBS->ConnectController (*Handle, NULL, Remaining, FALSE);
      if (EFI_ERROR (Status)) {
        // If the last node is a Memory Map Device Path just return EFI_SUCCESS.
        if ((Remaining->Type == HARDWARE_DEVICE_PATH) && (Remaining->SubType == HW_MEMMAP_DP)) {
            Status = EFI_SUCCESS;
        }
      }
    }
  } else if (!IsDevicePathEnd (Remaining) && !IsRemovableDevice (Remaining)) {

    /*// If the remaining Device Path is a FilePath or MemoryMap then we consider the Device Path has been loaded correctly
    if ((Remaining->Type == MEDIA_DEVICE_PATH) && (Remaining->SubType == MEDIA_FILEPATH_DP)) {
      Status = EFI_SUCCESS;
    } else if ((Remaining->Type == HARDWARE_DEVICE_PATH) && (Remaining->SubType == HW_MEMMAP_DP)) {
      Status = EFI_SUCCESS;
    }*/

    //TODO: Should we just return success and leave the caller decide if it is the expected RemainingPath
    Status = EFI_SUCCESS;
  } else {
    Status = TryRemovableDevice (DevicePath, Handle, &NewDevicePath);
    if (!EFI_ERROR (Status)) {
      return BdsConnectDevicePath (NewDevicePath, Handle, RemainingDevicePath);
    }
  }

  if (RemainingDevicePath) {
    *RemainingDevicePath = Remaining;
  }

  return Status;
}

BOOLEAN
BdsFileSystemSupport (
  IN EFI_DEVICE_PATH *DevicePath,
  IN EFI_HANDLE Handle,
  IN EFI_DEVICE_PATH *RemainingDevicePath
  )
{
  EFI_STATUS  Status;
  EFI_SIMPLE_FILE_SYSTEM_PROTOCOL     *FsProtocol;

  Status = gBS->HandleProtocol (Handle, &gEfiSimpleFileSystemProtocolGuid, (VOID **)&FsProtocol);

  return (!EFI_ERROR (Status) && IS_DEVICE_PATH_NODE (RemainingDevicePath, MEDIA_DEVICE_PATH, MEDIA_FILEPATH_DP));
}

EFI_STATUS
BdsFileSystemLoadImage (
  IN     EFI_DEVICE_PATH *DevicePath,
  IN     EFI_HANDLE Handle,
  IN     EFI_DEVICE_PATH *RemainingDevicePath,
  IN     EFI_ALLOCATE_TYPE     Type,
  IN OUT EFI_PHYSICAL_ADDRESS* Image,
  OUT    UINTN                 *ImageSize
  )
{
  FILEPATH_DEVICE_PATH*             FilePathDevicePath;
  EFI_SIMPLE_FILE_SYSTEM_PROTOCOL     *FsProtocol;
  EFI_FILE_PROTOCOL                   *Fs;
  EFI_STATUS Status;
  EFI_FILE_INFO       *FileInfo;
  EFI_FILE_PROTOCOL   *File;
  UINTN               Size;

  ASSERT (IS_DEVICE_PATH_NODE (RemainingDevicePath, MEDIA_DEVICE_PATH, MEDIA_FILEPATH_DP));

  FilePathDevicePath = (FILEPATH_DEVICE_PATH*)RemainingDevicePath;

  Status = gBS->HandleProtocol (Handle, &gEfiSimpleFileSystemProtocolGuid, (VOID **)&FsProtocol);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Try to Open the volume and get root directory
  Status = FsProtocol->OpenVolume (FsProtocol, &Fs);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  File = NULL;
  Status = Fs->Open (Fs, &File, FilePathDevicePath->PathName, EFI_FILE_MODE_READ, 0);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Size = 0;
  File->GetInfo (File, &gEfiFileInfoGuid, &Size, NULL);
  FileInfo = AllocatePool (Size);
  if (FileInfo == NULL)
    return EFI_OUT_OF_RESOURCES;

  Status = File->GetInfo (File, &gEfiFileInfoGuid, &Size, FileInfo);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Get the file size
  Size = FileInfo->FileSize;
  if (ImageSize) {
    *ImageSize = Size;
  }
  FreePool (FileInfo);

  Status = gBS->AllocatePages (Type, EfiBootServicesCode, EFI_SIZE_TO_PAGES(Size), Image);
  // Try to allocate in any pages if failed to allocate memory at the defined location
  if ((Status == EFI_OUT_OF_RESOURCES) && (Type != AllocateAnyPages)) {
    Status = gBS->AllocatePages (AllocateAnyPages, EfiBootServicesCode, EFI_SIZE_TO_PAGES(Size), Image);
  }
  if (!EFI_ERROR (Status)) {
    Status = File->Read (File, &Size, (VOID*)(UINTN)(*Image));
  }

  return Status;
}

BOOLEAN
BdsMemoryMapSupport (
  IN EFI_DEVICE_PATH *DevicePath,
  IN EFI_HANDLE Handle,
  IN EFI_DEVICE_PATH *RemainingDevicePath
  )
{
  return IS_DEVICE_PATH_NODE (DevicePath, HARDWARE_DEVICE_PATH, HW_MEMMAP_DP) ||
         IS_DEVICE_PATH_NODE (RemainingDevicePath, HARDWARE_DEVICE_PATH, HW_MEMMAP_DP);
}

EFI_STATUS
BdsMemoryMapLoadImage (
  IN     EFI_DEVICE_PATH *DevicePath,
  IN     EFI_HANDLE Handle,
  IN     EFI_DEVICE_PATH *RemainingDevicePath,
  IN     EFI_ALLOCATE_TYPE     Type,
  IN OUT EFI_PHYSICAL_ADDRESS* Image,
  OUT    UINTN                 *ImageSize
  )
{
  EFI_STATUS            Status;
  MEMMAP_DEVICE_PATH*   MemMapPathDevicePath;
  UINTN                 Size;

  if (IS_DEVICE_PATH_NODE (RemainingDevicePath, HARDWARE_DEVICE_PATH, HW_MEMMAP_DP)) {
    MemMapPathDevicePath = (MEMMAP_DEVICE_PATH*)RemainingDevicePath;
  } else {
    ASSERT (IS_DEVICE_PATH_NODE (DevicePath, HARDWARE_DEVICE_PATH, HW_MEMMAP_DP));
    MemMapPathDevicePath = (MEMMAP_DEVICE_PATH*)DevicePath;
  }

  Size = MemMapPathDevicePath->EndingAddress - MemMapPathDevicePath->StartingAddress;
  if (Size == 0) {
      return EFI_INVALID_PARAMETER;
  }

  Status = gBS->AllocatePages (Type, EfiBootServicesCode, EFI_SIZE_TO_PAGES(Size), Image);
  // Try to allocate in any pages if failed to allocate memory at the defined location
  if ((Status == EFI_OUT_OF_RESOURCES) && (Type != AllocateAnyPages)) {
    Status = gBS->AllocatePages (AllocateAnyPages, EfiBootServicesCode, EFI_SIZE_TO_PAGES(Size), Image);
  }
  if (!EFI_ERROR (Status)) {
    CopyMem ((VOID*)(UINTN)(*Image), (CONST VOID*)(UINTN)MemMapPathDevicePath->StartingAddress, Size);

    if (ImageSize != NULL) {
        *ImageSize = Size;
    }
  }

  return Status;
}

BOOLEAN
BdsFirmwareVolumeSupport (
  IN EFI_DEVICE_PATH *DevicePath,
  IN EFI_HANDLE Handle,
  IN EFI_DEVICE_PATH *RemainingDevicePath
  )
{
  return IS_DEVICE_PATH_NODE (RemainingDevicePath, MEDIA_DEVICE_PATH, MEDIA_PIWG_FW_FILE_DP);
}

EFI_STATUS
BdsFirmwareVolumeLoadImage (
  IN     EFI_DEVICE_PATH *DevicePath,
  IN     EFI_HANDLE Handle,
  IN     EFI_DEVICE_PATH *RemainingDevicePath,
  IN     EFI_ALLOCATE_TYPE     Type,
  IN OUT EFI_PHYSICAL_ADDRESS* Image,
  OUT    UINTN                 *ImageSize
  )
{
  EFI_STATUS            Status;
  EFI_FIRMWARE_VOLUME2_PROTOCOL     *FwVol;
  EFI_GUID                          *FvNameGuid;
  EFI_SECTION_TYPE                  SectionType;
  EFI_FV_FILETYPE                   FvType;
  EFI_FV_FILE_ATTRIBUTES            Attrib;
  UINT32                            AuthenticationStatus;
  VOID* ImageBuffer;

  ASSERT (IS_DEVICE_PATH_NODE (RemainingDevicePath, MEDIA_DEVICE_PATH, MEDIA_PIWG_FW_FILE_DP));

  Status = gBS->HandleProtocol (Handle, &gEfiFirmwareVolume2ProtocolGuid, (VOID **)&FwVol);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  FvNameGuid = EfiGetNameGuidFromFwVolDevicePathNode ((CONST MEDIA_FW_VOL_FILEPATH_DEVICE_PATH *)RemainingDevicePath);
  if (FvNameGuid == NULL) {
    Status = EFI_INVALID_PARAMETER;
  }

  SectionType = EFI_SECTION_PE32;
  AuthenticationStatus = 0;
  //Note: ReadSection at the opposite of ReadFile does not allow to pass ImageBuffer == NULL to get the size of the file.
  ImageBuffer = NULL;
  Status = FwVol->ReadSection (
                    FwVol,
                    FvNameGuid,
                    SectionType,
                    0,
                    &ImageBuffer,
                    ImageSize,
                    &AuthenticationStatus
                    );
  if (!EFI_ERROR (Status)) {
#if 0
    // In case the buffer has some address requirements, we must copy the buffer to a buffer following the requirements
    if (Type != AllocateAnyPages) {
      Status = gBS->AllocatePages (Type, EfiBootServicesCode, EFI_SIZE_TO_PAGES(*ImageSize),Image);
      if (!EFI_ERROR (Status)) {
        CopyMem ((VOID*)(UINTN)(*Image), ImageBuffer, *ImageSize);
        FreePool (ImageBuffer);
      }
    }
#else
    // We must copy the buffer into a page allocations. Otherwise, the caller could call gBS->FreePages() on the pool allocation
    Status = gBS->AllocatePages (Type, EfiBootServicesCode, EFI_SIZE_TO_PAGES(*ImageSize), Image);
    // Try to allocate in any pages if failed to allocate memory at the defined location
    if ((Status == EFI_OUT_OF_RESOURCES) && (Type != AllocateAnyPages)) {
      Status = gBS->AllocatePages (AllocateAnyPages, EfiBootServicesCode, EFI_SIZE_TO_PAGES(*ImageSize), Image);
    }
    if (!EFI_ERROR (Status)) {
      CopyMem ((VOID*)(UINTN)(*Image), ImageBuffer, *ImageSize);
      FreePool (ImageBuffer);
    }
#endif
  } else {
    // Try a raw file, since a PE32 SECTION does not exist
    Status = FwVol->ReadFile (
                        FwVol,
                        FvNameGuid,
                        NULL,
                        ImageSize,
                        &FvType,
                        &Attrib,
                        &AuthenticationStatus
                        );
    if (!EFI_ERROR (Status)) {
      Status = gBS->AllocatePages (Type, EfiBootServicesCode, EFI_SIZE_TO_PAGES(*ImageSize), Image);
      // Try to allocate in any pages if failed to allocate memory at the defined location
      if ((Status == EFI_OUT_OF_RESOURCES) && (Type != AllocateAnyPages)) {
        Status = gBS->AllocatePages (AllocateAnyPages, EfiBootServicesCode, EFI_SIZE_TO_PAGES(*ImageSize), Image);
      }
      if (!EFI_ERROR (Status)) {
        Status = FwVol->ReadFile (
                                FwVol,
                                FvNameGuid,
                                (VOID*)(UINTN)(*Image),
                                ImageSize,
                                &FvType,
                                &Attrib,
                                &AuthenticationStatus
                                );
      }
    }
  }
  return Status;
}

BOOLEAN
BdsPxeSupport (
  IN EFI_DEVICE_PATH*           DevicePath,
  IN EFI_HANDLE                 Handle,
  IN EFI_DEVICE_PATH*           RemainingDevicePath
  )
{
  EFI_STATUS                  Status;
  EFI_PXE_BASE_CODE_PROTOCOL* PxeBcProtocol;

  if (!IsDevicePathEnd (RemainingDevicePath)) {
    return FALSE;
  }

  Status = gBS->HandleProtocol (Handle, &gEfiPxeBaseCodeProtocolGuid, (VOID **)&PxeBcProtocol);
  if (EFI_ERROR (Status)) {
    return FALSE;
  } else {
    return TRUE;
  }
}

EFI_STATUS
BdsPxeLoadImage (
  IN     EFI_DEVICE_PATH*       DevicePath,
  IN     EFI_HANDLE             Handle,
  IN     EFI_DEVICE_PATH*       RemainingDevicePath,
  IN     EFI_ALLOCATE_TYPE      Type,
  IN OUT EFI_PHYSICAL_ADDRESS   *Image,
  OUT    UINTN                  *ImageSize
  )
{
  EFI_STATUS              Status;
  EFI_LOAD_FILE_PROTOCOL  *LoadFileProtocol;
  UINTN                   BufferSize;
  EFI_PXE_BASE_CODE_PROTOCOL *Pxe;

  // Get Load File Protocol attached to the PXE protocol
  Status = gBS->HandleProtocol (Handle, &gEfiLoadFileProtocolGuid, (VOID **)&LoadFileProtocol);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = LoadFileProtocol->LoadFile (LoadFileProtocol, DevicePath, TRUE, &BufferSize, NULL);
  if (Status == EFI_BUFFER_TOO_SMALL) {
    Status = gBS->AllocatePages (Type, EfiBootServicesCode, EFI_SIZE_TO_PAGES(BufferSize), Image);
    if (EFI_ERROR (Status)) {
      return Status;
    }

    Status = LoadFileProtocol->LoadFile (LoadFileProtocol, DevicePath, TRUE, &BufferSize, (VOID*)(UINTN)(*Image));
    if (!EFI_ERROR (Status) && (ImageSize != NULL)) {
      *ImageSize = BufferSize;
    }
  }

  if (Status == EFI_ALREADY_STARTED) {
    Status = gBS->LocateProtocol (&gEfiPxeBaseCodeProtocolGuid, NULL, (VOID **)&Pxe);
    if (!EFI_ERROR(Status)) {
      // If PXE is already started, we stop it
      Pxe->Stop (Pxe);
      // And we try again
      return BdsPxeLoadImage (DevicePath, Handle, RemainingDevicePath, Type, Image, ImageSize);
    }
  }
  return Status;
}

BDS_FILE_LOADER FileLoaders[] = {
    { BdsFileSystemSupport, BdsFileSystemLoadImage },
    { BdsFirmwareVolumeSupport, BdsFirmwareVolumeLoadImage },
    { BdsMemoryMapSupport, BdsMemoryMapLoadImage },
    { BdsPxeSupport, BdsPxeLoadImage },
    { NULL, NULL }
};

EFI_STATUS
BdsLoadImage (
  IN     EFI_DEVICE_PATH       *DevicePath,
  IN     EFI_ALLOCATE_TYPE     Type,
  IN OUT EFI_PHYSICAL_ADDRESS* Image,
  OUT    UINTN                 *FileSize
  )
{
  EFI_STATUS      Status;
  EFI_HANDLE      Handle;
  EFI_DEVICE_PATH *RemainingDevicePath;
  BDS_FILE_LOADER*  FileLoader;
  EFI_DEVICE_PATH *FullDevicePath;
  EFI_DEVICE_PATH *FileNameDevicePath;
  
  FileNameDevicePath = FileDevicePath(NULL, EFI_REMOVABLE_MEDIA_FILE_NAME);
  if (!IS_DEVICE_PATH_NODE(DevicePath, MEDIA_DEVICE_PATH, MEDIA_FILEPATH_DP))
  {
    FullDevicePath = AppendDevicePath(DevicePath, FileNameDevicePath);
  }
  else {
    FullDevicePath = DevicePath;
  }

  Status = BdsConnectDevicePath (FullDevicePath, &Handle, &RemainingDevicePath);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  FileLoader = FileLoaders;
  while (FileLoader->Support != NULL) {
    if (FileLoader->Support (DevicePath, Handle, RemainingDevicePath)) {
      return FileLoader->LoadImage (DevicePath, Handle, RemainingDevicePath, Type, Image, FileSize);
    }
    FileLoader++;
  }
  FreePool(FullDevicePath);
  return EFI_UNSUPPORTED;
}

/* UEFI Memory Types */
char *MemType[] = {"Reserved",
                   "LoaderCode",
                   "LoaderData",
                   "BS_Code",
                   "BS_Data",
                   "RT_Code",
                   "RT_Data",
                   "ConvMemory",
                   "Unusable",
                   "ACPIRM",
                   "ACPIMNVS",
                   "MM_IO",
                   "MM_IOPort",
                   "PalCode",
                   "PersiMemory",
                   "MaxMemoryType"
                 };

/**
  Get the FW Regions from Memory Map.

  FW Regions:   EfiRuntimeServicesCode, EfiRuntimeServicesData,
                EfiReservedMemoryType, EfiACPIMemoryNVS,
                Holes, EfiUnusableMemory,
                EfiPalCode, EfiACPIReclaimMemory,
                EfiMemoryMappedIOPortSpace, EfiMemoryMappedIO
  
  @param  MemMap                Pointer to UEFI Memory Map
  @param  MemMapSize            UEFI Memory Map Size
  @param  DescriptorSize        Size of each descriptor in MemMap
  @param  FwGroup               FwGroup points to buffer where FW descriptors <physical_address,number_of_pages> are stored
  @param  FwGroupSize           FwGroupSize stores the size of FwGroup
  
  @retval EFI_SUCCESS           FwGroup buffer successfully filled
  @retval EFI_NOT_FOUND         MemMap not found
**/
EFI_STATUS
BdsGetFwRegionsFromMemMap (
  IN      EFI_MEMORY_DESCRIPTOR   *MemMap,
  IN      UINTN                   MemMapSize,
  IN      UINTN                   DescriptorSize,
  IN OUT  VOID                    *FwGroup,
  IN OUT  UINTN                   *FwGroupSize
  )
{
  EFI_STATUS            Status = EFI_SUCCESS;
  UINT32                Pages;
  UINT32                Type;
  UINT32                Index;
  VOID                  *pFwGroup = NULL;
  
  if (MemMap == NULL || MemMapSize == 0 || DescriptorSize == 0)
  {
    DEBUG ((EFI_D_ERROR, "[BdsGetFwRegionsFromMemMap] MemMap not found\n"));
    Status = EFI_NOT_FOUND;
    return Status;
  }

  pFwGroup = FwGroup;

  for (Index = 0; Index < (MemMapSize/DescriptorSize); Index++)
  {      
    if (IS_MEMTYPE_FW (MemMap->Type))
    {
      CopyMem (pFwGroup, (VOID*)(&(MemMap->PhysicalStart)), sizeof(UINTN));
      pFwGroup += sizeof (UINTN);
      Pages = (UINT32)MemMap->NumberOfPages;
      CopyMem (pFwGroup, (VOID*)(&Pages), sizeof (UINT32));
      pFwGroup += sizeof (UINT32);
      Type = (UINT32)MemMap->Type;
      CopyMem (pFwGroup, (VOID*)(&Type), sizeof (UINT32));
      pFwGroup += sizeof (UINT32);
      *FwGroupSize += (sizeof (UINTN) + sizeof (UINT32) + sizeof (UINT32));
    }
    MemMap = NEXT_MEMORY_DESCRIPTOR (MemMap, DescriptorSize);
  }

  return Status;
}

/**
  Calculate the CRC value for Firmware memory regions in memory map
  CRC calculated over the triplet <physical_address, size, type>

  FwGroup :     EfiRuntimeServicesCode, EfiRuntimeServicesData,
                EfiReservedMemoryType, EfiACPIMemoryNVS,
                Holes, EfiUnusableMemory,
                EfiPalCode, EfiACPIReclaimMemory,
                EfiMemoryMappedIOPortSpace, EfiMemoryMappedIO
  
  @param  MemMap                Pointer to UEFI Memory Map
  @param  MemMapSize            UEFI Memory Map Size
  @param  DescriptorSize        Descriptor size of each descriptor in MemMap
  @param  FwGroupCRC            Pointer to FwGroupCRC variable
  
  @retval EFI_SUCCESS           CRC calculated successfully
  @retval EFI_OUT_OF_RESOURCES  Failed to allocate pool for NonFwGroupCRC or FwGroupCRC
  @retval EFI_NOT_FOUND         MemMap not found
**/
EFI_STATUS
BdsCalculateMemMapCRC (
  IN  EFI_MEMORY_DESCRIPTOR  *MemMap,
  IN  UINTN                  MemMapSize,
  IN  UINTN                  DescriptorSize,
  OUT UINT32                 *FwGroupCRC
  )
{
  EFI_STATUS            Status = EFI_SUCCESS;
  RegionEntry           FwRegion;
  UINT32                Index = 0;
  VOID                  *FwGroupBase = NULL;
  VOID                  *pFwGroup = NULL;

  if (MemMap == NULL)
  {
    DEBUG ((EFI_D_ERROR, "[BdsCalculateMemMapCRC] MemMap not found\n"));
    Status = EFI_NOT_FOUND;
    return Status;
  }

  FwGroupBase = AllocatePool (sizeof (FwRegion) * (MemMapSize/DescriptorSize));
  if (FwGroupBase == NULL)
  {
    DEBUG ((EFI_D_ERROR, "[BdsCalculateMemMapCRC] Failed to allocate pool for FwGroup\n"));
    Status = EFI_OUT_OF_RESOURCES;
    return Status;
  }

  pFwGroup = FwGroupBase;
  
  for (Index = 0; Index < (MemMapSize/DescriptorSize); Index++)
  {      
    if (IS_MEMTYPE_FW (MemMap->Type))
    {
      FwRegion.RegionBaseAddress = MemMap->PhysicalStart;
      FwRegion.RegionSize = (UINT32)EFI_PAGES_TO_SIZE (MemMap->NumberOfPages);
      FwRegion.RegionType = (UINT32)MemMap->Type;
      CopyMem (pFwGroup, (VOID*)(&(FwRegion)), sizeof (FwRegion));
      pFwGroup += sizeof (FwRegion);
    }
    else if (!IS_MEMTYPE_NONFW (MemMap->Type))
    {
      DEBUG ((EFI_D_ERROR, "[BdsCalculateMemMapCRC] MemoryType of region at address : 0x%lx is not FW or NonFW (Type : %d)\n",\
                MemMap->PhysicalStart,\
                MemMap->Type));
    }
    MemMap = NEXT_MEMORY_DESCRIPTOR (MemMap, DescriptorSize);
  }

  *FwGroupCRC     = CalculateCrc32 (FwGroupBase, pFwGroup - FwGroupBase);

  if (FwGroupBase != NULL && pFwGroup != NULL)
  {
    FreePool (FwGroupBase);
    pFwGroup = NULL;
  }
  
  return Status;
}

/**
  Internal wrapper function for GetVariable()
  Note: Caller needs to free VariableData and DataSize pointers
  
  @param  VariableName          Variable Name, of which the value has to be retrieved
  @param  DataSize              Size of the Variable 
  @param  VariableData          Pointer to buffer where the retrieved value is stored
  
  @retval EFI_SUCCESS           If GetVariable() is successful
  @retval Other                 If GetVariable() is not success, it returns the respective value
**/
EFI_STATUS
BdsGetVariable(
  IN      CHAR16   *VariableName,
  IN  OUT UINTN    *DataSize,
  IN  OUT VOID     **VariableData
  )
{
  EFI_STATUS            Status = EFI_SUCCESS;

  // Check whether the passed Variable exists
  Status = gRT->GetVariable (VariableName, &gQcomTokenSpaceGuid, NULL, DataSize, *VariableData);

  // Check if the DataSize was not sufficient, call GetVariable() again
  if (Status == EFI_BUFFER_TOO_SMALL)
  {
    *VariableData = AllocatePool (*DataSize);
    if (*VariableData == NULL)
    {
      DEBUG ((EFI_D_ERROR, "[BdsGetVariable] Variable buffer allocation failed\n"));
      Status = EFI_BUFFER_TOO_SMALL;
      return Status; 
    }

    Status = gRT->GetVariable (VariableName, &gQcomTokenSpaceGuid, NULL, DataSize, *VariableData);
    if (EFI_ERROR (Status))
    {
      DEBUG ((EFI_D_ERROR, "[BdsGetVariable] GetVariable() failed, Status = 0x%lx\n", Status));    
    }
  }
  else
  {
    DEBUG ((EFI_D_ERROR, "[BdsGetVariable] GetVariable() failed, Status = 0x%lx\n", Status));
  }

  return Status;
}

/**
  Internal wrapper function for SetVariable()
  
  @param  VariableName          Variable Name, of which the value has to be set
  @param  DataSize              Size of the Variable 
  @param  Data                  Pointer to buffer which stores the value to be set.
  
  @retval EFI_SUCCESS           If SetVariable() is successful
  @retval Other                 If SetVariable() is not success, it returns the respective value
**/
EFI_STATUS
BdsSetVariable(
  IN      CHAR16   *VariableName,
  IN      UINTN    *DataSize,
  IN      VOID     *Data 
  )
{
  EFI_STATUS            Status = EFI_SUCCESS;

  Status = gRT->SetVariable (VariableName,\
                             &gQcomTokenSpaceGuid,\
                             (EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS),\
                             *DataSize,\
                             Data);
  if (EFI_ERROR (Status))
  {
    DEBUG ((EFI_D_ERROR, "[BdsSetVariable] Unable to set %s variable, Status = 0x%lx\n", VariableName, Status));
  }

  return Status;
}

/**
  Internal function to print the current FW region in UEFI memory map
  
  @param  MemMap                Pointer to UEFI memory map
  @param  MemMapSize            UEFI memory map size
  @param  DescriptorSize        Size of each descriptor in memory map
  
  @retval EFI_SUCCESS           If Memory Map is printed successfully
  @retval EFI_NOT_FOUND         If MemMap is NULL or MemMapSize or DescriptorSize is 0 
**/
EFI_STATUS
BdsPrintCurrentBootFwRegions(
  IN EFI_MEMORY_DESCRIPTOR  *MemMap,
  IN UINTN                  MemMapSize,
  IN UINTN                  DescriptorSize
  )
{
  EFI_STATUS            Status = EFI_SUCCESS;
  UINT32                Index = 0;

  if (MemMap == NULL || MemMapSize == 0 || DescriptorSize == 0)
  {
    Status = EFI_NOT_FOUND;
    return Status;
  }

  DEBUG ((EFI_D_WARN, "Current boot FW regions\n---------------------------------------------------------\n"));
  for (Index = 0; Index < MemMapSize/DescriptorSize; Index++)
  {
    if(IS_MEMTYPE_FW(MemMap->Type))
      DEBUG ((EFI_D_WARN, "%a    \tPhysicalStart = 0x%lx    \tPages = 0x%lx\n",\
                        MemType[MemMap->Type],\
                        MemMap->PhysicalStart,\
                        MemMap->NumberOfPages));

    MemMap = NEXT_MEMORY_DESCRIPTOR (MemMap, DescriptorSize);
  }
  DEBUG ((EFI_D_WARN, "---------------------------------------------------------\n"));
  
  return Status;
}

/**
  Wrapper function to retrieve the UEFI memory map
  Note: Caller needs to free the MemMap pointer
  
  @param  MemMapSize            Pointer to UEFI memory map size
  @param  MemMap                Pointer to UEFI memory map
  @param  MapKey                Pointer to UEFI memory map key
  @param  DescriptorSize        Pointer to variable which stores the size of each descriptor in memory map
  @param  DescriptorVersion     Pointer to DescriptorVersion
  
  @retval EFI_SUCCESS           If Memory Map is retrieved successfully
  @retval EFI_OUT_OF_RESOURCES  If MemMap buffer cannot be allocated
  @retval Other                 Status returned by gBS->GetMemoryMap() call 
**/
EFI_STATUS
BdsGetMemoryMap(
  IN OUT UINTN                  *MemMapSize,
  IN OUT EFI_MEMORY_DESCRIPTOR  **MemMap,
  IN OUT UINTN                  *MapKey,
  IN OUT UINTN                  *DescriptorSize,
  IN OUT UINT32                 *DescriptorVersion
  )
{
  EFI_STATUS            Status;

  // Retrieve the size of MemMap buffer required
  Status = gBS->GetMemoryMap (MemMapSize, *MemMap, MapKey, DescriptorSize, DescriptorVersion);
  if (Status == EFI_BUFFER_TOO_SMALL)
  {
    // In case the AllocatePool call adds memory map entries, add in extra descriptors
    *MemMapSize += ((*DescriptorSize) * NUMBER_OF_EXTRA_DESCRIPTORS);
    *MemMap = AllocatePool (*MemMapSize);
    if (*MemMap == NULL)
    {
      DEBUG ((EFI_D_ERROR, "[BdsGetMemoryMap] Unable to allocate buffer for UEFI Memory Map\n"));
      return EFI_OUT_OF_RESOURCES;
    }

    // Retrieve the Memory Map after successful buffer allocation
    Status = gBS->GetMemoryMap (MemMapSize, *MemMap, MapKey, DescriptorSize, DescriptorVersion);
    if (EFI_ERROR (Status))
    {
      DEBUG ((EFI_D_ERROR, "[BdsGetMemoryMap] Unable to get UEFI Memory Map, Error = 0x%lx\n", Status));
    }
  }
  else
  {
    DEBUG ((EFI_D_ERROR, "[BdsGetMemoryMap] Unable to get UEFI Memory Map, Error = 0x%lx\n", Status));
  }

  return Status;
}

/**
  Internal function to compare the CRC values of FW regions. If there is a change in CRC value, set the variable
  with the updated CRC value.
  
  @param  FwGroupCRC            Current FwGroupCRC value
  @param  OldFwGroupCRC         Previous boot FwGroupCRC value
  @param  NoChangeInCRC         Pointer to a Boolean variable to keep track for change in CRC value of FwGroup
  
  @retval EFI_SUCCESS           If function returns without error
  @retval Other                 Based on Status returned by gRT->SetVariable() call 
**/
EFI_STATUS
BdsCompareAndUpdateCRC(
  IN      UINT32             FwGroupCRC,
  IN      UINT32             OldFwGroupCRC,
  IN OUT  BOOLEAN            *NoChangeInCRC
  )
{
  EFI_STATUS            Status = EFI_SUCCESS;

  if (FwGroupCRC == OldFwGroupCRC)
  {
    DEBUG ((EFI_D_ERROR, "[BdsCompareAndUpdateCRC] No Change in FwGroup CRC values\n"));
    *NoChangeInCRC = TRUE;
    return Status;
  }

  DEBUG ((EFI_D_WARN, "OldFwGroupCRC = %u\tFwGroupCRC = %u\n", OldFwGroupCRC, FwGroupCRC));
  DEBUG ((EFI_D_WARN, "Updating FwGroupCRCVar variable\n"));
  Status = gRT->SetVariable (L"FwGroupCRCVar",\
                              &gQcomTokenSpaceGuid,\
                              (EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS),\
                              sizeof (FwGroupCRC),\
                              &FwGroupCRC);
  if (EFI_ERROR (Status))
  {
    DEBUG ((EFI_D_ERROR, "[SetVariable] Unable to set FwGroupCRCVar variable, Error = 0x%lx\n", Status));
  }

  return Status; 
}

/**
  Internal function to update CRC and firmware version string variable values.
  
  @param  FwGroupCRC            Current FwGroupCRC value
  @param  FwVerStr              Current FwVerStr value
  @param  FwVerStrSize          Current FwVerStr's size
  
  @retval EFI_SUCCESS           If function returns without error
  @retval Other                 Based on Status returned by gRT->SetVariable() call
**/
EFI_STATUS
BdsUpdateMemMapVariables(
  IN      UINT32             FwGroupCRC,
  IN      CHAR8              *FwVerStr,
  IN      UINTN              FwVerStrSize
  )
{
  EFI_STATUS            Status = EFI_SUCCESS;

  DEBUG ((EFI_D_WARN, "Setting FwGroupCRCVar variable to %u\n", FwGroupCRC));
  Status = gRT->SetVariable (L"FwGroupCRCVar",\
                              &gQcomTokenSpaceGuid,\
                              (EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS),\
                              sizeof (FwGroupCRC),\
                              &FwGroupCRC);
  if (EFI_ERROR (Status))
  {
    DEBUG ((EFI_D_ERROR, "[SetVariable] Unable to set FwGroupCRCVar variable, Error = 0x%lx\n", Status));
  }

  DEBUG ((EFI_D_WARN, "Setting OldBootFwVerStr variable to %a\n", FwVerStr));
  Status = gRT->SetVariable (L"OldBootFwVerStr",\
                              &gQcomTokenSpaceGuid,\
                              (EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS),\
                              FwVerStrSize,\
                              FwVerStr);
  if (EFI_ERROR (Status))
  {
    DEBUG ((EFI_D_ERROR, "[SetVariable] Unable to set OldBootFwVerStr variable, Error = 0x%lx\n", Status));
  }

  return Status; 
}

/**
  Internal function to store the FW region entries of memory map in a NV BS RT variable.
  
  @param  MemMap                Pointer to Memory Map
  @param  MemMapSize            Memory map size
  @param  DescriptorSize        Descriptor size
  
  @retval EFI_SUCCESS           If FwRegions are stored successfully in a variable
  @retval EFI_OUT_OF_RESOURCES  If FwRegion buffer cannot be allocated
  @retval Other                 Based on status returned by BdsGetFwRegionsFromMemMap() and/or SetVariable() call
**/
EFI_STATUS
BdsStoreFwRegion(
  IN EFI_MEMORY_DESCRIPTOR  *MemMap,
  IN UINTN                  MemMapSize,
  IN UINTN                  DescriptorSize
  )
{
  EFI_STATUS            Status = EFI_SUCCESS;
  VOID                  *FwGroup = NULL;
  UINTN                 FwGroupSize = 0;

  DEBUG ((EFI_D_WARN, "Storing memory map values of FW Regions\n"));

  FwGroup = AllocatePool ((sizeof (UINTN) + sizeof (UINT32) + sizeof (UINT32)) * (MemMapSize/DescriptorSize));
  if (FwGroup == NULL)
  {
    DEBUG ((EFI_D_ERROR, "[AllocatePool] Failed to allocate buffer for FwGroup regions\n"));
    return EFI_OUT_OF_RESOURCES;
  }

  Status = BdsGetFwRegionsFromMemMap (MemMap, MemMapSize, DescriptorSize, FwGroup, &FwGroupSize);
  if (EFI_ERROR (Status))
  {
    DEBUG ((EFI_D_ERROR, "[BdsGetFwRegionsFromMemMap] Failed, Error = 0x%lx\n", Status));
    goto BdsStoreFwRegionEnd;
  }

  Status = gRT->SetVariable (L"FwGroupVar",\
                              &gQcomTokenSpaceGuid,\
                              (EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS),\
                              FwGroupSize,\
                              FwGroup);
  if (EFI_ERROR (Status))
  {
    DEBUG ((EFI_D_ERROR, "[SetVariable] Unable to set FwGroupVar variable, Error = 0x%lx\n", Status));
  }

BdsStoreFwRegionEnd:
  if (FwGroup != NULL)
    FreePool (FwGroup);

  return Status;
}

/**
  Internal function to print the FW region entries from previous boot using FwGroupVar variable
  
  @retval EFI_SUCCESS           If previous boot FW regions were printed properly
  @retval EFI_NOT_FOUND         If FwGroupVar was not found
  @retval Other
**/
EFI_STATUS
BdsPrintPreviousBootFwRegions(VOID)
{
  EFI_STATUS            Status = EFI_SUCCESS;
  VOID                  *FwGroup = NULL;
  VOID                  *pFwGroup = NULL;
  UINTN                 FwGroupSize = 0;
  UINTN                 Index_line = 0;
  UINTN                 SizeofTriplet = sizeof (UINTN) + (2 * sizeof (UINT32));
  UINTN                 PhysicalStartAddress = 0;
  UINT32                NumberOfPages = 0;
  UINT32                RegionType = 0;

  Status = BdsGetVariable(L"FwGroupVar", &FwGroupSize, &FwGroup);
  if (EFI_ERROR (Status))
  {
    DEBUG ((EFI_D_ERROR, "[BdsGetVariable] Unable to get FwGroupVar variable, Error = 0x%lx\n", Status));
    if (FwGroup != NULL)
      FreePool (FwGroup);
    return Status;
  }

  pFwGroup = FwGroup;

  DEBUG ((EFI_D_WARN, "Previous boot FW regions\n---------------------------------------------------------\n"));
  for (Index_line = 0; Index_line < (FwGroupSize / SizeofTriplet); Index_line++)
  {
    CopyMem ((VOID*)&PhysicalStartAddress, pFwGroup, sizeof (UINTN));
    pFwGroup += sizeof (UINTN);
    CopyMem ((VOID*)&NumberOfPages, pFwGroup, sizeof (UINT32));
    pFwGroup += sizeof (UINT32);
    CopyMem ((VOID*)&RegionType, pFwGroup, sizeof (UINT32));
    pFwGroup += sizeof (UINT32);

    DEBUG ((EFI_D_WARN, "%a    \tPhysicalStart = 0x%lx    \tPages = 0x%lx\n",\
                        MemType[RegionType],
                        PhysicalStartAddress,
                        NumberOfPages));
  }
  DEBUG ((EFI_D_WARN, "---------------------------------------------------------\n"));

  if (FwGroup != NULL)
    FreePool (FwGroup);

  return Status;
}
                
/**
  Function to detect changes in FW regions in memory map.
  See macro IS_MEMTYPE_FW for info on which region falls in this category.
  Function will check for change in CRC for FW region only if FwVerStr is same across successive boots
  and print the UEFI Memory Map only if change in FW region's CRC value.
  If change in CRC, print the previous boot FW regions and store the current boot FW regions
  <physical_address, number_of_pages, region_type> in a NV BS RT variable.
**/
VOID
BdsDetectMemoryMapChanges (VOID)
{
  EFI_STATUS            Status;
  BOOLEAN               NoChangeInCRC = FALSE;
  EFI_MEMORY_DESCRIPTOR *MemMap = NULL;
  UINTN                 MemMapSize = 0;
  UINTN                 MapKey = 0;
  UINTN                 DescriptorSize = 0;
  UINT32                DescriptorVersion = 0;
  CHAR8                 *OldBootFwVerStr = NULL;
  CHAR8                 *FwVerStr = NULL;
  UINTN                 VarSize = 0;
  UINTN                 FwVerStrSize = 0;
  UINT32                FwGroupCRC = 0;
  UINT32                *OldFwGroupCRC = NULL;

  // Get the Memory Map
  Status = BdsGetMemoryMap (&MemMapSize, &MemMap, &MapKey, &DescriptorSize, &DescriptorVersion);
  if (EFI_ERROR (Status))
  {
    DEBUG ((EFI_D_ERROR, "[BdsDetectMemoryMapChanges] BdsGetMemoryMap Failed, Exiting function\n"));
    goto FunctionExit;
  }

  // Calculate the CRC for FwGroup
  Status = BdsCalculateMemMapCRC (MemMap, MemMapSize, DescriptorSize, &FwGroupCRC);
  if (EFI_ERROR (Status))
  {
   DEBUG ((EFI_D_ERROR, "[BdsDetectMemoryMapChanges] BdsCalculateMemMapCRC Failed, Exiting function\n"));
   goto FunctionExit;
  }

  Status = BdsGetVariable (L"FwGroupCRCVar", &VarSize, (VOID*)&OldFwGroupCRC);
  if (EFI_ERROR (Status))
  {
    if (VarSize == 0)
      VarSize = sizeof (*OldFwGroupCRC);

    Status = BdsSetVariable (L"FwGroupCRCVar", &VarSize, &FwGroupCRC);
    if (EFI_ERROR (Status))
      goto FunctionExit;
  }
  if (OldFwGroupCRC != NULL)
  {
    DEBUG ((EFI_D_INFO, "CRC values\tOldFwGroupCRC = %u\tFwGroupCRC = %u\n", *OldFwGroupCRC, FwGroupCRC));
  }
  else
    DEBUG ((EFI_D_ERROR, "First boot: FwGroupCRC = %u\n", FwGroupCRC));

  Status = BdsGetVariable (L"FwVerStr", &FwVerStrSize, (VOID*)&FwVerStr);
  if (EFI_ERROR (Status))
  {
    // No call to BdsSetVariable() because FwVerStr should already be set
    goto FunctionExit;
  }
  DEBUG ((EFI_D_INFO, "FwVerStr = %a\n", FwVerStr));

  VarSize = 0;
  Status = BdsGetVariable (L"OldBootFwVerStr", &VarSize, (VOID*)&OldBootFwVerStr);
  if (EFI_ERROR (Status))
  {
    if (VarSize == 0)
      VarSize = FwVerStrSize;

      Status = BdsSetVariable (L"OldBootFwVerStr", &VarSize, (VOID*)FwVerStr);
    if (EFI_ERROR (Status))
      goto FunctionExit;
  }
  if (OldBootFwVerStr != NULL)
    DEBUG ((EFI_D_INFO, "OldBootFwVerStr = %a\n", OldBootFwVerStr));
  
  if (OldBootFwVerStr == NULL || AsciiStrnCmp (FwVerStr, OldBootFwVerStr, AsciiStrSize (FwVerStr)) != 0)
  {
    /** 
    If FwVerStr and OldBootFwVerStr are different,
    then no need to check for CRC value change as 
    different builds can have different memory map
    **/
    DEBUG ((EFI_D_WARN, "FwVerStr changed or First boot, not checking for change in CRC\n"));
    DEBUG ((EFI_D_WARN, "Updating FwGroupCRC and OldBootFwVerStr variable values to current values\n"));
    Status = BdsUpdateMemMapVariables (FwGroupCRC, FwVerStr, FwVerStrSize);
    goto PrintandStoreFwRegion;
  }
  DEBUG ((EFI_D_INFO, "OldBootFwVerStr and FwVerStr values match\n"));

  if (OldFwGroupCRC == NULL)
  {
    DEBUG ((EFI_D_ERROR, "[BdsDetectMemoryMapChanges] OldFwGroupCRC is NULL, Exiting function\n"));
    goto FunctionExit;
  }
  
  Status = BdsCompareAndUpdateCRC (FwGroupCRC, *OldFwGroupCRC, &NoChangeInCRC);
  if (EFI_ERROR (Status))
  {
    DEBUG ((EFI_D_ERROR, "[BdsDetectMemoryMapChanges] BdsCompareAndUpdateCRC Failed, Exiting function\n"));
    goto FunctionExit;
  }

  if (NoChangeInCRC)
  {
    //No change in FwGroup CRC, don't print the map and don't save the FW regions in a variable
    goto FunctionExit;
  }

PrintandStoreFwRegion:
  // Print the current boot FW regions
  Status = BdsPrintCurrentBootFwRegions (MemMap, MemMapSize, DescriptorSize);
  if (EFI_ERROR (Status))
  {
    DEBUG ((EFI_D_ERROR, "[BdsDetectMemoryMapChanges] BdsPrintCurrentBootFwRegions Failed, Exiting function\n")); 
    goto FunctionExit; 
  }

  Status = BdsPrintPreviousBootFwRegions();
  if (EFI_ERROR (Status))
    DEBUG ((EFI_D_ERROR, "[BdsDetectMemoryMapChanges] BdsPrintPreviousBootFwRegions Failed, Error = %d\n", Status));

  Status = BdsStoreFwRegion (MemMap, MemMapSize, DescriptorSize);
  if (EFI_ERROR (Status))
    DEBUG ((EFI_D_ERROR, "[BdsDetectMemoryMapChanges] BdsStoreFwRegion Failed, Exiting function\n"));

  // ASSERT in Debug mode
  //ASSERT (RETAIL);
FunctionExit:
  if (MemMap != NULL)
    FreePool (MemMap);
  
  if (FwVerStr != NULL)
    FreePool (FwVerStr);

  if (OldBootFwVerStr != NULL)
    FreePool (OldBootFwVerStr);

  if (OldFwGroupCRC != NULL)
    FreePool (OldFwGroupCRC);

}

/**
  Start an EFI Application from a Device Path

  @param  ParentImageHandle     Handle of the calling image
  @param  DevicePath            Location of the EFI Application

  @retval EFI_SUCCESS           All drivers have been connected
  @retval EFI_NOT_FOUND         The Linux kernel Device Path has not been found
  @retval EFI_OUT_OF_RESOURCES  There is not enough resource memory to store the matching results.

**/
EFI_STATUS
BdsStartEfiApplication (
  IN EFI_HANDLE                  ParentImageHandle,
  IN EFI_DEVICE_PATH_PROTOCOL   *DevicePath,
  IN UINTN                       LoadOptionsSize,
  IN VOID*                       LoadOptions
  )
{
  EFI_STATUS                   Status;
  EFI_HANDLE                   ImageHandle;
  EFI_HANDLE                   Handle;
  EFI_DEVICE_PATH_PROTOCOL    *FilePath;
  EFI_LOADED_IMAGE_PROTOCOL   *LoadedImage;
  EFI_DEVICE_PATH_PROTOCOL    *WorkingDevicePath;
  EFI_QCOM_WATCHDOG_PROTOCOL  *WdogProt;
  UINTN                        VarSize;
  UINT8                        Flag = 0;
  
  //if the device path starts with a hard drive path, it needs to be 
  //made into a full device path
  WorkingDevicePath = NULL;
  if ((DevicePathType (DevicePath) == MEDIA_DEVICE_PATH) &&
      (DevicePathSubType (DevicePath) == MEDIA_HARDDRIVE_DP)) {
    WorkingDevicePath = BdsExpandPartitionPartialDevicePathToFull (
                          (HARDDRIVE_DEVICE_PATH *)DevicePath
                          );
    if (WorkingDevicePath != NULL) {
      DevicePath = WorkingDevicePath;
    }
  }
 
    //
  // Report status code for OS Loader LoadImage.
  //
  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, PcdGet32 (PcdProgressCodeOsLoaderLoad));

 
  //Try loading image with working device path 
  Status = gBS->LoadImage (TRUE, ParentImageHandle, DevicePath, NULL, 0, &ImageHandle);
  if (EFI_ERROR (Status)) {
    Handle = BdsLibGetBootableHandle(DevicePath);
    if (Handle == NULL) {
      DEBUG ((DEBUG_ERROR, "[QcomBds] Could not find bootable handle\r\n"));
      return Status;
    }
    //
    // Load the default boot file \EFI\BOOT\boot{machinename}.EFI from removable Media
    //  machinename is ia32, ia64, x64, ...
    //
    FilePath = FileDevicePath (Handle, EFI_REMOVABLE_MEDIA_FILE_NAME);
    if (FilePath != NULL)
   {
      Status = gBS->LoadImage (TRUE, ParentImageHandle, FilePath, NULL, 0, &ImageHandle);
   }
    if (EFI_ERROR(Status)) {
     REPORT_STATUS_CODE (EFI_ERROR_CODE | EFI_ERROR_MINOR,(EFI_SOFTWARE_DXE_BS_DRIVER | EFI_SW_DXE_BS_EC_BOOT_OPTION_LOAD_ERROR));
      return Status;
    }

  }

  // Passed LoadOptions to the EFI Application
  if (LoadOptionsSize != 0) {
    Status = gBS->HandleProtocol (ImageHandle, &gEfiLoadedImageProtocolGuid, (VOID **) &LoadedImage);
    if (EFI_ERROR (Status)) {
      return Status;
    }

    LoadedImage->LoadOptionsSize  = LoadOptionsSize;
    LoadedImage->LoadOptions      = LoadOptions;
  }
  
  VarSize = sizeof(Flag);
  Status = gRT->GetVariable (L"EnableMemMap", &gQcomTokenSpaceGuid,
                             NULL, &VarSize, &Flag);

  if ((Status == EFI_SUCCESS) && (Flag))
    BdsDetectMemoryMapChanges();

  // Before calling the image, enable the Watchdog Timer for  the 5 Minute period
  gBS->SetWatchdogTimer (5 * 60, 0x0000, 0x00, NULL);
  
  // disable HW wdog to prevent wdog reset if the app disable interrupt and pet timer 
  Status = gBS->LocateProtocol (
                  &gEfiQcomWDogProtocolGuid,
                  NULL,
                  (VOID **) &WdogProt); 
  if (EFI_ERROR(Status)) {
      DEBUG((DEBUG_ERROR, "Failed to locate wdog protocol\n"));
  }
  else {
      WdogProt->Disable();
  }
  
  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, PcdGet32 (PcdProgressCodeOsLoaderStart));
  // Start the image
  Status = gBS->StartImage (ImageHandle, NULL, NULL);
  
    if (EFI_ERROR (Status)) {
    //
    // Report Status Code to indicate that boot failure
    //
    REPORT_STATUS_CODE (
      EFI_ERROR_CODE | EFI_ERROR_MINOR,
      (EFI_SOFTWARE_DXE_BS_DRIVER | EFI_SW_DXE_BS_EC_BOOT_OPTION_FAILED)
      );
  }

  // Clear the Watchdog Timer after the image returns
  gBS->SetWatchdogTimer (0x0000, 0x0000, 0x0000, NULL);

  return Status;
}
