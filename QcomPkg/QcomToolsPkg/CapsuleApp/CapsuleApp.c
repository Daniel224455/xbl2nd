/** @file
  A shell application that triggers capsule update process.

  Copyright (c) 2017 - 2019 Qualcomm Technologies, Inc. All rights reserved.
  Portions Copyright (c) 2016 - 2017, Intel Corporation. All rights reserved.<BR>
  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/
/*=============================================================================
                              EDIT HISTORY
  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

 when       who     what, where, why
 --------   ---     -----------------------------------------------------------
 02/11/19   rajb    Added test hook support to reset the device at different phases
                    of capsule update and also to widen progress bar width
 02/05/19   akeshan Addition of capsule storage type as an input to CapsuleApp
 07/20/18   gparam  KW Fixes
 02/22/17   gparam  Initial revision from EDK2 with minor fixes

=============================================================================*/

#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/PrintLib.h>
#include <Library/QcomCapsuleHeader.h>
#include <Protocol/GraphicsOutput.h>
#include <Guid/GlobalVariable.h>
#include <Guid/CapsuleReport.h>
#include <Guid/SystemResourceTable.h>
#include <Guid/FmpCapsule.h>
#include <IndustryStandard/WindowsUxCapsule.h>
#include <ctype.h>
#include <Include/Library/FwUpdateLib.h>

#define CAPSULE_HEADER_SIZE  0x20

#define NESTED_CAPSULE_HEADER_SIZE  SIZE_4KB
#define SYSTEM_FIRMWARE_FLAG 0x50000
#define DEVICE_FIRMWARE_FLAG 0x78010

#define MAJOR_VERSION   1
#define MINOR_VERSION   0

#define MAX_CAPSULE_NUM 10

#define GUID_STRING_LENGTH  36

#define MAX_ALLOWED_FW_ENTRIES 255

extern UINTN  Argc;
extern CHAR16 **Argv;

//
// Define how many block descriptors we want to test with.
//
UINTN  NumberOfDescriptors = 1;
UINTN  CapsuleFirstIndex;
UINTN  CapsuleLastIndex;

/**
  Dump capsule information

  @param[in] CapsuleName  The name of the capsule image.

  @retval EFI_SUCCESS            The capsule information is dumped.
  @retval EFI_UNSUPPORTED        Input parameter is not valid.
**/
EFI_STATUS
DumpCapsule (
  IN CHAR16                                        *CapsuleName
  );

/**
  Dump capsule status variable.

  @retval EFI_SUCCESS            The capsule status variable is dumped.
  @retval EFI_UNSUPPORTED        Input parameter is not valid.
**/
EFI_STATUS
DmpCapsuleStatusVariable (
  VOID
  );

/**
  Dump FMP protocol info.
**/
VOID
DumpFmpData (
  VOID
  );

/**
  Dump FMP image data.

  @param[in]  ImageTypeId   The ImageTypeId of the FMP image.
                            It is used to identify the FMP protocol.
  @param[in]  ImageIndex    The ImageIndex of the FMP image.
                            It is the input parameter for FMP->GetImage().
  @param[in]  ImageName     The file name to hold the output FMP image.
**/
VOID
DumpFmpImage (
  IN EFI_GUID  *ImageTypeId,
  IN UINTN     ImageIndex,
  IN CHAR16    *ImageName
  );

/**
  Dump ESRT info.
**/
VOID
DumpEsrtData (
  VOID
  );

/**
  Read a file.

  @param[in]  FileName        The file to be read.
  @param[out] BufferSize      The file buffer size
  @param[out] Buffer          The file buffer

  @retval EFI_SUCCESS    Read file successfully
  @retval EFI_NOT_FOUND  Shell protocol or file not found
  @retval others         Read file failed
**/
EFI_STATUS
ReadFileToBuffer (
  IN  CHAR16                               *FileName,
  OUT UINTN                                *BufferSize,
  OUT VOID                                 **Buffer
  );

/**
  Write a file.

  @param[in] FileName        The file to be written.
  @param[in] BufferSize      The file buffer size
  @param[in] Buffer          The file buffer

  @retval EFI_SUCCESS    Write file successfully
  @retval EFI_NOT_FOUND  Shell protocol not found
  @retval others         Write file failed
**/
EFI_STATUS
WriteFileFromBuffer (
  IN  CHAR16                               *FileName,
  IN  UINTN                                BufferSize,
  IN  VOID                                 *Buffer
  );

/**
  Converts a string to GUID value.
  Guid Format is xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx

  @param[in]  Str              The registry format GUID string that contains the GUID value.
  @param[out] Guid             A pointer to the converted GUID value.

  @retval EFI_SUCCESS     The GUID string was successfully converted to the GUID value.
  @retval EFI_UNSUPPORTED The input string is not in registry format.
  @return others          Some error occurred when converting part of GUID value.

**/
EFI_STATUS
InternalStrToGuid (
  IN  CHAR16   *Str,
  OUT EFI_GUID *Guid
  );

/**

  This function parse application ARG.

  @return Status
**/
EFI_STATUS
GetArg (
  VOID
  );

  /**
  Read user supplied wide progress bar on/off configuration setting
**/
EFI_STATUS
ConfigureProgressBarLength (
);

/**
  Read user supplied reset data to a structure and set the NV variable with that data
**/
EFI_STATUS
ReadAndSetPhaseData (
);


/**
  Create UX capsule.

  @retval EFI_SUCCESS            The capsule header is appended.
  @retval EFI_UNSUPPORTED        Input parameter is not valid.
  @retval EFI_OUT_OF_RESOURCES   No enough resource to create UX capsule.
**/
EFI_STATUS
CreateBmpFmp (
  VOID
  )
{
  CHAR16                                        *OutputCapsuleName;
  VOID                                          *BmpBuffer;
  UINTN                                         FileSize;
  CHAR16                                        *BmpName;
  UINT8                                         *FullCapsuleBuffer;
  UINTN                                         FullCapsuleBufferSize;
  EFI_DISPLAY_CAPSULE                           *DisplayCapsule;
  EFI_STATUS                                    Status;
  EFI_GRAPHICS_OUTPUT_PROTOCOL                  *Gop;

  Status = gBS->LocateProtocol(&gEfiGraphicsOutputProtocolGuid, NULL, (VOID **)&Gop);
  if (EFI_ERROR(Status)) {
    Print(L"CapsuleApp: NO GOP is found.\n");
    return EFI_UNSUPPORTED;
  }
  Print(L"Current GOP: Mode - %d, ", Gop->Mode->Mode);
  Print(L"HorizontalResolution - %d, ", Gop->Mode->Info->HorizontalResolution);
  Print(L"VerticalResolution - %d\n", Gop->Mode->Info->VerticalResolution);
  // HorizontalResolution >= BMP_IMAGE_HEADER.PixelWidth
  // VerticalResolution   >= BMP_IMAGE_HEADER.PixelHeight

  if (Argc != 5) {
    Print(L"CapsuleApp: Invalid Parameter.\n");
    return EFI_UNSUPPORTED;
  }

  if (StrCmp(Argv[3], L"-O") != 0) {
    Print(L"CapsuleApp: NO output capsule name.\n");
    return EFI_UNSUPPORTED;
  }
  OutputCapsuleName = Argv[4];

  BmpBuffer = NULL;
  FileSize = 0;
  FullCapsuleBuffer = NULL;

  BmpName = Argv[2];
  Status = ReadFileToBuffer(BmpName, &FileSize, &BmpBuffer);
  if (EFI_ERROR(Status)) {
    Print(L"CapsuleApp: BMP image (%s) is not found.\n", BmpName);
    goto Done;
  }

  FullCapsuleBufferSize = sizeof(EFI_DISPLAY_CAPSULE) + FileSize;
  FullCapsuleBuffer = AllocatePool(FullCapsuleBufferSize);
  if (FullCapsuleBuffer == NULL) {
    Print(L"CapsuleApp: Capsule Buffer size (0x%x) too big.\n", FullCapsuleBufferSize);
    Status = EFI_OUT_OF_RESOURCES;
    goto Done;
  }

  DisplayCapsule = (EFI_DISPLAY_CAPSULE *)FullCapsuleBuffer;
  CopyGuid(&DisplayCapsule->CapsuleHeader.CapsuleGuid, &gWindowsUxCapsuleGuid);
  DisplayCapsule->CapsuleHeader.HeaderSize = sizeof(DisplayCapsule->CapsuleHeader);
  DisplayCapsule->CapsuleHeader.Flags = CAPSULE_FLAGS_PERSIST_ACROSS_RESET;
  DisplayCapsule->CapsuleHeader.CapsuleImageSize = (UINT32)FullCapsuleBufferSize;

  DisplayCapsule->ImagePayload.Version = 1;
  DisplayCapsule->ImagePayload.Checksum = 0;
  DisplayCapsule->ImagePayload.ImageType = 0; // BMP
  DisplayCapsule->ImagePayload.Reserved = 0;
  DisplayCapsule->ImagePayload.Mode = Gop->Mode->Mode;
  DisplayCapsule->ImagePayload.OffsetX = 0;
  DisplayCapsule->ImagePayload.OffsetY = 0;

  CopyMem((DisplayCapsule + 1), BmpBuffer, FileSize);

  DisplayCapsule->ImagePayload.Checksum = CalculateCheckSum8(FullCapsuleBuffer, FullCapsuleBufferSize);

  Status = WriteFileFromBuffer(OutputCapsuleName, FullCapsuleBufferSize, FullCapsuleBuffer);
  Print(L"CapsuleApp: Write %s %r\n", OutputCapsuleName, Status);

Done:
  if (BmpBuffer != NULL) {
    FreePool(BmpBuffer);
  }

  if (FullCapsuleBuffer != NULL) {
    FreePool(FullCapsuleBuffer);
  }

  return Status;
}

/**
  Get ImageTypeId in the FMP capsule header.

  @param[in] CapsuleHeader  The FMP capsule image header.

  @return ImageTypeId
**/
EFI_GUID *
GetCapsuleImageTypeId (
  IN EFI_CAPSULE_HEADER                            *CapsuleHeader
  )
{
  EFI_FIRMWARE_MANAGEMENT_CAPSULE_HEADER       *FmpCapsuleHeader;
  UINT64                                       *ItemOffsetList;
  EFI_FIRMWARE_MANAGEMENT_CAPSULE_IMAGE_HEADER *ImageHeader;

  FmpCapsuleHeader = (EFI_FIRMWARE_MANAGEMENT_CAPSULE_HEADER *)((UINT8 *)CapsuleHeader + CapsuleHeader->HeaderSize);
  ItemOffsetList = (UINT64 *)(FmpCapsuleHeader + 1);
  if (FmpCapsuleHeader->PayloadItemCount == 0) {
    return NULL;
  }
  ImageHeader = (EFI_FIRMWARE_MANAGEMENT_CAPSULE_IMAGE_HEADER *)((UINT8 *)FmpCapsuleHeader + ItemOffsetList[FmpCapsuleHeader->EmbeddedDriverCount]);
  return &ImageHeader->UpdateImageTypeId;
}

/**
  Get ESRT FwType according to ImageTypeId

  @param[in]  ImageTypeId   ImageTypeId of an FMP capsule.

  @return ESRT FwType
**/
UINT32
GetEsrtFwType (
  IN  EFI_GUID                                      *ImageTypeId
  )
{
  EFI_STATUS                 Status;
  EFI_SYSTEM_RESOURCE_TABLE  *Esrt;
  EFI_SYSTEM_RESOURCE_ENTRY  *EsrtEntry;
  UINTN                      Index;

  //
  // Check ESRT
  //
  Status = EfiGetSystemConfigurationTable(&gEfiSystemResourceTableGuid, (VOID **)&Esrt);
  if (!EFI_ERROR(Status)) {
    ASSERT(Esrt != NULL);
    EsrtEntry = (VOID *)(Esrt + 1);
    for (Index = 0; Index < Esrt->FwResourceCount; Index++, EsrtEntry++) {
      if (CompareGuid(&EsrtEntry->FwClass, ImageTypeId)) {
        return EsrtEntry->FwType;
      }
    }
  }

  return ESRT_FW_TYPE_UNKNOWN;
}

/**
  Append a capsule header on top of current image.
  This function follows Windows UEFI Firmware Update Platform document.

  @retval EFI_SUCCESS            The capsule header is appended.
  @retval EFI_UNSUPPORTED        Input parameter is not valid.
  @retval EFI_OUT_OF_RESOURCES   No enough resource to append capsule header.
**/
EFI_STATUS
CreateNestedFmp (
  VOID
  )
{
  CHAR16                                        *OutputCapsuleName;
  VOID                                          *CapsuleBuffer;
  UINTN                                         FileSize;
  CHAR16                                        *CapsuleName;
  UINT8                                         *FullCapsuleBuffer;
  UINTN                                         FullCapsuleBufferSize;
  EFI_CAPSULE_HEADER                            *NestedCapsuleHeader;
  EFI_GUID                                      *ImageTypeId;
  UINT32                                        FwType;
  EFI_STATUS                                    Status;

  if (Argc != 5) {
    Print(L"CapsuleApp: Invalid Parameter.\n");
    return EFI_UNSUPPORTED;
  }

  if (StrCmp(Argv[3], L"-O") != 0) {
    Print(L"CapsuleApp: NO output capsule name.\n");
    return EFI_UNSUPPORTED;
  }
  OutputCapsuleName = Argv[4];

  CapsuleBuffer = NULL;
  FileSize = 0;
  FullCapsuleBuffer = NULL;

  CapsuleName = Argv[2];
  Status = ReadFileToBuffer(CapsuleName, &FileSize, &CapsuleBuffer);
  if (EFI_ERROR(Status)) {
    Print(L"CapsuleApp: Capsule image (%s) is not found.\n", CapsuleName);
    goto Done;
  }

  ImageTypeId = GetCapsuleImageTypeId(CapsuleBuffer);
  if (ImageTypeId == NULL) {
    Print(L"CapsuleApp: Capsule ImageTypeId is not found.\n");
    goto Done;
  }
  FwType = GetEsrtFwType(ImageTypeId);
  if ((FwType != ESRT_FW_TYPE_SYSTEMFIRMWARE) && (FwType != ESRT_FW_TYPE_DEVICEFIRMWARE)) {
    Print(L"CapsuleApp: Capsule FwType is invalid.\n");
    goto Done;
  }

  FullCapsuleBufferSize = NESTED_CAPSULE_HEADER_SIZE + FileSize;
  FullCapsuleBuffer = AllocatePool(FullCapsuleBufferSize);
  if (FullCapsuleBuffer == NULL) {
    Print(L"CapsuleApp: Capsule Buffer size (0x%x) too big.\n", FullCapsuleBufferSize);
    Status = EFI_OUT_OF_RESOURCES;
    goto Done;
  }

  NestedCapsuleHeader = (EFI_CAPSULE_HEADER *)FullCapsuleBuffer;
  ZeroMem(NestedCapsuleHeader, NESTED_CAPSULE_HEADER_SIZE);
  CopyGuid(&NestedCapsuleHeader->CapsuleGuid, ImageTypeId);
  NestedCapsuleHeader->HeaderSize = NESTED_CAPSULE_HEADER_SIZE;
  NestedCapsuleHeader->Flags = (FwType == ESRT_FW_TYPE_SYSTEMFIRMWARE) ? SYSTEM_FIRMWARE_FLAG : DEVICE_FIRMWARE_FLAG;
  NestedCapsuleHeader->CapsuleImageSize = (UINT32)FullCapsuleBufferSize;

  CopyMem((UINT8 *)NestedCapsuleHeader + NestedCapsuleHeader->HeaderSize, CapsuleBuffer, FileSize);

  Status = WriteFileFromBuffer(OutputCapsuleName, FullCapsuleBufferSize, FullCapsuleBuffer);
  Print(L"CapsuleApp: Write %s %r\n", OutputCapsuleName, Status);

Done:
  if (CapsuleBuffer != NULL) {
    FreePool(CapsuleBuffer);
  }

  if (FullCapsuleBuffer != NULL) {
    FreePool(FullCapsuleBuffer);
  }

  return Status;
}


/**
  Clear capsule status variable.

  @retval EFI_SUCCESS            The capsule status variable is cleared.
**/
EFI_STATUS
ClearCapsuleStatusVariable (
  VOID
  )
{
  EFI_STATUS                          Status;
  UINT32                              Index;
  CHAR16                              CapsuleVarName[20] = L"Capsule";
  CHAR16                              *TempVarName;

  TempVarName = CapsuleVarName + StrLen (CapsuleVarName);
  Index = 0;

  while (TRUE) {
    UnicodeSPrint (TempVarName, 5 * sizeof(CHAR16), L"%04x", Index);

    Status = gRT->SetVariable (
                    CapsuleVarName,
                    &gEfiCapsuleReportGuid,
                    EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_BOOTSERVICE_ACCESS,
                    0,
                    (VOID *)NULL
                    );
    if (EFI_ERROR(Status)) {
      //
      // There is no capsule variables, quit
      //
      break;
    }

    Index++;
    if (Index > 0xFFFF) {
      break;
    }
  }

  return EFI_SUCCESS;
}

/**
  Build Gather list for a list of capsule images.

  @param[in]  CapsuleBuffer    An array of pointer to capsule images
  @param[in]  FileSize         An array of UINTN to capsule images size
  @param[in]  CapsuleNum       The count of capsule images
  @param[out] BlockDescriptors The block descriptors for the capsule images

  @retval EFI_SUCCESS The block descriptors for the capsule images are constructed.
**/
EFI_STATUS
BuildGatherList (
  IN VOID                          **CapsuleBuffer,
  IN UINTN                         *FileSize,
  IN UINTN                         CapsuleNum,
  OUT EFI_CAPSULE_BLOCK_DESCRIPTOR **BlockDescriptors
  )
{
  EFI_STATUS                    Status;
  EFI_CAPSULE_BLOCK_DESCRIPTOR  *BlockDescriptors1;
  EFI_CAPSULE_BLOCK_DESCRIPTOR  *BlockDescriptors2;
  EFI_CAPSULE_BLOCK_DESCRIPTOR  *BlockDescriptorPre;
  EFI_CAPSULE_BLOCK_DESCRIPTOR  *BlockDescriptorsHeader;
  EFI_CAPSULE_BLOCK_DESCRIPTOR  *TempBlockPtr;
  UINT8                         *TempDataPtr;
  UINTN                         SizeLeft;
  UINTN                         Size;
  INT32                         Count;
  INT32                         Number;
  UINTN                         Index;

  TempBlockPtr           = NULL;
  BlockDescriptors1      = NULL;
  BlockDescriptors2      = NULL;
  BlockDescriptorPre     = NULL;
  BlockDescriptorsHeader = NULL;

  for (Index = 0; Index < CapsuleNum; Index++) {
    //
    // Allocate memory for the descriptors.
    //
    if (NumberOfDescriptors == 1) {
      Count = 2;
    } else {
      Count = (INT32)(NumberOfDescriptors + 2) / 2;
    }

    Size               = Count * sizeof (EFI_CAPSULE_BLOCK_DESCRIPTOR);
    BlockDescriptors1  = AllocateRuntimeZeroPool (Size);
    if (BlockDescriptors1 == NULL) {
      Print (L"CapsuleApp: failed to allocate memory for descriptors\n");
      Status = EFI_OUT_OF_RESOURCES;
      goto ERREXIT;
    } else {
      Print (L"CapsuleApp: creating capsule descriptors at 0x%X\n", (UINTN) BlockDescriptors1);
      Print (L"CapsuleApp: capsule data starts          at 0x%X with size 0x%X\n", (UINTN) CapsuleBuffer[Index], FileSize[Index]);
    }

    //
    // Record descirptor header
    //
    if (Index == 0) {
      BlockDescriptorsHeader = BlockDescriptors1;
    }

    if (BlockDescriptorPre != NULL) {
      BlockDescriptorPre->Union.ContinuationPointer = (UINTN) BlockDescriptors1;
      BlockDescriptorPre->Length = 0;
    }

    //
    // Fill them in
    //
    TempBlockPtr  = BlockDescriptors1;
    TempDataPtr   = CapsuleBuffer[Index];
    SizeLeft      = FileSize[Index];
    for (Number = 0; (Number < Count - 1) && (SizeLeft != 0); Number++) {
      //
      // Divide remaining data in half
      //
      if (NumberOfDescriptors != 1) {
        if (SizeLeft == 1) {
          Size = 1;
        } else {
          Size = SizeLeft / 2;
        }
      } else {
        Size = SizeLeft;
      }
      TempBlockPtr->Union.DataBlock    = (UINTN)TempDataPtr;
      TempBlockPtr->Length  = Size;
      Print (L"CapsuleApp: capsule block/size              0x%X/0x%X\n", (UINTN) TempDataPtr, Size);
      SizeLeft -= Size;
      TempDataPtr += Size;
      TempBlockPtr++;
    }

    //
    // Allocate the second list, point the first block's last entry to point
    // to this one, and fill this one in. Worst case is that the previous
    // list only had one element that pointed here, so we need at least two
    // elements -- one to point to all the data, another to terminate the list.
    //
    if ((NumberOfDescriptors != 1) && (SizeLeft != 0)) {
      Count = (INT32)(NumberOfDescriptors + 2) - Count;
      if (Count == 1) {
        Count++;
      }

      Size              = Count * sizeof (EFI_CAPSULE_BLOCK_DESCRIPTOR);
      BlockDescriptors2 = AllocateRuntimeZeroPool (Size);
      if (BlockDescriptors2 == NULL) {
        Print (L"CapsuleApp: failed to allocate memory for descriptors\n");
        Status = EFI_OUT_OF_RESOURCES;
        goto ERREXIT;
      }

      //
      // Point the first list's last element to point to this second list.
      //
      TempBlockPtr->Union.ContinuationPointer   = (UINTN) BlockDescriptors2;

      TempBlockPtr->Length  = 0;
      TempBlockPtr = BlockDescriptors2;
      for (Number = 0; Number < Count - 1; Number++) {
        //
        // If second-to-last one, then dump rest to this element
        //
        if (Number == (Count - 2)) {
          Size = SizeLeft;
        } else {
          //
          // Divide remaining data in half
          //
          if (SizeLeft == 1) {
            Size = 1;
          } else {
            Size = SizeLeft / 2;
          }
        }

        TempBlockPtr->Union.DataBlock    = (UINTN)TempDataPtr;
        TempBlockPtr->Length  = Size;
        Print (L"CapsuleApp: capsule block/size              0x%X/0x%X\n", (UINTN) TempDataPtr, Size);
        SizeLeft -= Size;
        TempDataPtr += Size;
        TempBlockPtr++;
        if (SizeLeft == 0) {
          break;
        }
      }
    }

    BlockDescriptorPre = TempBlockPtr;
    BlockDescriptors1  = NULL;
  }

  //
  // Null-terminate.
  //
  if (TempBlockPtr != NULL) {
    TempBlockPtr->Union.ContinuationPointer    = (UINTN)NULL;
    TempBlockPtr->Length  = 0;
    *BlockDescriptors = BlockDescriptorsHeader;
  }

  return EFI_SUCCESS;

ERREXIT:
  if (BlockDescriptors1 != NULL) {
    FreePool(BlockDescriptors1);
  }

  if (BlockDescriptors2 != NULL) {
    FreePool(BlockDescriptors2);
  }

  return Status;
}

/**
  Clear the Gather list for a list of capsule images.

  @param[in]  BlockDescriptors The block descriptors for the capsule images
  @param[in]  CapsuleNum       The count of capsule images
**/
VOID
CleanGatherList (
  IN EFI_CAPSULE_BLOCK_DESCRIPTOR   *BlockDescriptors,
  IN UINTN                          CapsuleNum
  )
{
  EFI_CAPSULE_BLOCK_DESCRIPTOR   *TempBlockPtr;
  EFI_CAPSULE_BLOCK_DESCRIPTOR   *TempBlockPtr1;
  EFI_CAPSULE_BLOCK_DESCRIPTOR   *TempBlockPtr2;
  UINTN                          Index;

  if (BlockDescriptors != NULL) {
    TempBlockPtr1 = BlockDescriptors;
    while (1){
      TempBlockPtr = TempBlockPtr1;
      for (Index = 0; Index < CapsuleNum; Index++) {
        if (TempBlockPtr[Index].Length == 0) {
          break;
        }
      }

      if (TempBlockPtr[Index].Union.ContinuationPointer == (UINTN)NULL) {
        break;
      }

      TempBlockPtr2 = (VOID *) ((UINTN) TempBlockPtr[Index].Union.ContinuationPointer);
      FreePool(TempBlockPtr1);
      TempBlockPtr1 = TempBlockPtr2;
    }
  }
}

/**
  Change the capsule storage based on user input.

  @param[in]  argc      Number of args passed into the app
  @param[in]  argv      Array of char arguments
**/
EFI_STATUS 
ChangeCapsuleStorage(UINTN  Argc, CHAR16 **Argv) {
    EFI_STATUS    Status;
    CapsuleStorageType CapsuleStorage = CAPSULE_STORAGE_TYPE_CAPSULE_RAW_FILE;
    UINTN         VarSize;
    VarSize = sizeof(CapsuleStorage);
        
    AsciiPrint("CapsuleApp.c Attempting to set capsule storage type\r\n");
    
    if (Argc == 3) {
      if (StrCmp(Argv[2], L"P") == 0) {
        CapsuleStorage = CAPSULE_STORAGE_TYPE_CAPSULE_PARTITION;
        Status = gRT->SetVariable (L"EnableCapsulePartition", &gQcomTokenSpaceGuid,
                                   EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
                                   VarSize, &CapsuleStorage);

        if(Status == EFI_SUCCESS)
          AsciiPrint("CapsuleApp.c Setting capsule storage type as RAW PARTITION\r\n");
        else
          AsciiPrint("ERROR: CapsuleApp.c Unable to change/store setting\r\n");
        return Status;
      } 
      else {
        CapsuleStorage = CAPSULE_STORAGE_TYPE_CAPSULE_RAW_FILE;
        Status = gRT->SetVariable (L"EnableCapsulePartition", &gQcomTokenSpaceGuid,
                                   EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
                                   VarSize, &CapsuleStorage);

        if(Status == EFI_SUCCESS)
          AsciiPrint("CapsuleApp.c Setting capsule storage type as EFIESP\r\n");
        else
          AsciiPrint("ERROR: CapsuleApp.c Unable to change/store setting\r\n");
        return Status;
      }
    }
    else {
        CapsuleStorage = CAPSULE_STORAGE_TYPE_CAPSULE_RAW_FILE;
        Status = gRT->SetVariable (L"EnableCapsulePartition", &gQcomTokenSpaceGuid,
                                   EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
                                   VarSize, &CapsuleStorage);

        if(Status == EFI_SUCCESS)
          AsciiPrint("CapsuleApp.c Setting capsule storage type as EFIESP\r\n");
        else
          AsciiPrint("ERROR: CapsuleApp.c Unable to change/store setting\r\n");
        return Status;
    }
}


/**
  Print APP usage.
**/
VOID
PrintUsage (
  VOID
  )
{
  Print(L"CapsuleApp:  usage\n");
  Print(L"  CapsuleApp <Capsule...> [-NR]\n");
  Print(L"  CapsuleApp -S\n");
  Print(L"  CapsuleApp -C\n");
  Print(L"  CapsuleApp -P\n");
  Print(L"  CapsuleApp -E\n");
  Print(L"  CapsuleApp -G <BMP> -O <Capsule>\n");
  Print(L"  CapsuleApp -N <Capsule> -O <NestedCapsule>\n");
  Print(L"  CapsuleApp -D <Capsule>\n");
  Print(L"  CapsuleApp -P GET <ImageTypeId> <Index> -O <FileName>\n");
  Print(L"  CapsuleApp -cM <P/F>\n");
  Print(L"  CapsuleApp -rP <B[1-255]/U[1-255]/D[1-255]/R[1-255]/V>\n");
  Print(L"  CapsuleApp -wPB <ON/OFF>\n");
  Print(L"Parameter:\n");
  Print(L"  -NR: No reset will be triggered for the capsule\n");
  Print(L"       with CAPSULE_FLAGS_PERSIST_ACROSS_RESET and without CAPSULE_FLAGS_INITIATE_RESET.\n");
  Print(L"  -S:  Dump capsule report variable (EFI_CAPSULE_REPORT_GUID),\n");
  Print(L"       which is defined in UEFI specification.\n");
  Print(L"  -C:  Clear capsule report variable (EFI_CAPSULE_RPORT_GUID),\n");
  Print(L"       which is defined in UEFI specification.\n");
  Print(L"  -P:  Dump UEFI FMP protocol info.\n");
  Print(L"  -E:  Dump UEFI ESRT table info.\n");
  Print(L"  -G:  Convert a BMP file to be a UX capsule,\n");
  Print(L"       according to Windows Firmware Update document\n");
  Print(L"  -N:  Append a Capsule Header to an existing capsule image,\n");
  Print(L"       according to Windows Firmware Update document\n");
  Print(L"  -O:  Output new Capsule file name\n");
  Print(L"  -D:  Dump Capsule image header information and FMP header information,\n");
  Print(L"       if it is an FMP capsule.\n");
  Print(L"  -cM/capMedia: Used to change the capsule media type. Default will be EFIESP. \n");
  Print(L"       Sub params:\n");
  Print(L"           F: Set the storage type to aw file in EFIESP\n");
  Print(L"           P: Set the storage type to capsule raw partition:\n");
  Print(L"       Eg: -cM F \n");
  Print(L"           -cM P \n");
  Print(L"  -rP/resetPhase: Used to reset the device at different phase. \n");
  Print(L"       Sub params:\n");
  Print(L"           B[1-254]: To reset the device during backup phase, before processing [1-254]'th Fw entry:\n");
  Print(L"           U[1-254]: To reset the device during update phase, before processing [1-254]'th Fw entry:\n");
  Print(L"           D[1-254]: To reset the device during delete phase, before processing [1-254]'th Fw entry:\n");
  Print(L"           R[1-254]: To reset the device during rollback phase, before processing [1-254]'th Fw entry:\n");
  Print(L"           V       : To view the current configuration of reset phase test hook:\n");
  Print(L"       Notes:\n");
  Print(L"           - Reset values '1 to 254' indicates the firmware entry before which reset should be induced\n");
  Print(L"           - 'Empty or 255' is to clear the respective reset phase variable\n");
  Print(L"           - '0' and > '255' are invalid entries\n");
  Print(L"           - Reset phases can be set individual or as group except option 'V' which shall be \n");
  Print(L"             individual to display all the phase value configured. All literals are case sensitive \n");
  Print(L"       Eg: CapsuleApp.efi -rP B1 \n");
  Print(L"           CapsuleApp.efi -rP B33 U32 \n");
  Print(L"           CapsuleApp.efi -rP R34 D20 B U\n");
  Print(L"           CapsuleApp.efi -rP B U R D\n");
  Print(L"           CapsuleApp.efi -rP V\n");
  Print(L"  -wPB/wideProgBar: Used to display different lengths of progress bar\n");
  Print(L"       Sub params:\n");
  Print(L"           ON : Centered progress bar of width equal to (screen-width / 2)\n");
  Print(L"           OFF: Centered progress bar of width equal to boot logo width\n");
  Print(L"       Eg: -wPB ON\n");
  Print(L"           -wPB OFF\n\n");
}

/**
  Update Capsule image.

  @param[in]  ImageHandle     The image handle.
  @param[in]  SystemTable     The system table.

  @retval EFI_SUCCESS            Command completed successfully.
  @retval EFI_INVALID_PARAMETER  Command usage error.
  @retval EFI_NOT_FOUND          The input file can't be found.
**/
EFI_STATUS
EFIAPI
UefiMain (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS                    Status;
  RETURN_STATUS                 RStatus;
  UINTN                         FileSize[MAX_CAPSULE_NUM];
  VOID                          *CapsuleBuffer[MAX_CAPSULE_NUM];
  EFI_CAPSULE_BLOCK_DESCRIPTOR  *BlockDescriptors;
  EFI_CAPSULE_HEADER             *CapsuleHeaderArray[MAX_CAPSULE_NUM + 1];
  UINT64                         MaxCapsuleSize;
  EFI_RESET_TYPE                 ResetType;
  BOOLEAN                        NeedReset;
  BOOLEAN                        NoReset;
  CHAR16                         *CapsuleName;
  UINTN                          CapsuleNum;
  UINTN                          Index;

  Status = GetArg();
  if (EFI_ERROR(Status)) {
    Print(L"Please use UEFI SHELL to run this application!\n", Status);
    return Status;
  }
  if (Argc < 2) {
    PrintUsage();
    return EFI_INVALID_PARAMETER;
  }
  if (StrCmp(Argv[1], L"-D") == 0) {
    Status = DumpCapsule(Argv[2]);
    return Status;
  }
  if (StrCmp(Argv[1], L"-G") == 0) {
    Status = CreateBmpFmp();
    return Status;
  }
  if (StrCmp(Argv[1], L"-N") == 0) {
    Status = CreateNestedFmp();
    return Status;
  }
  if (StrCmp(Argv[1], L"-S") == 0) {
    Status = DmpCapsuleStatusVariable();
    return EFI_SUCCESS;
  }
  if (StrCmp(Argv[1], L"-C") == 0) {
    Status = ClearCapsuleStatusVariable();
    return Status;
  }
  if (StrCmp(Argv[1], L"-P") == 0) {
    if (Argc == 2) {
      DumpFmpData();
    }
    if (Argc >= 3) {
      if (StrCmp(Argv[2], L"GET") == 0) {
        EFI_GUID  ImageTypeId;
        UINTN     ImageIndex;
        //
        // FMP->GetImage()
        //
        RStatus = InternalStrToGuid (Argv[3], &ImageTypeId);
        if (RETURN_ERROR (RStatus) || (Argv[3][GUID_STRING_LENGTH] != L'\0')) {
          Print (L"Invalid ImageTypeId - %s\n", Argv[3]);
          return EFI_INVALID_PARAMETER;
        }
        ImageIndex = StrDecimalToUintn(Argv[4]);
        if (StrCmp(Argv[5], L"-O") == 0) {
          DumpFmpImage(&ImageTypeId, ImageIndex, Argv[6]);
        }
      }
    }
    return EFI_SUCCESS;
  }
  if (StrCmp(Argv[1], L"-E") == 0) {
    DumpEsrtData();
    return EFI_SUCCESS;
  }
  
  if (StrCmp(Argv[1], L"-cM") == 0 || StrCmp(Argv[1], L"-capMedia") == 0) {
    Status = ChangeCapsuleStorage(Argc, Argv);
    return Status;
  }
  
  if ((StrCmp(Argv[1], L"-rP") == 0) || 
      (StrCmp(Argv[1], L"-resetPhase") == 0)) {

    if ((Argc - 1) < 2) {
      PrintUsage();
      return EFI_INVALID_PARAMETER;
    }

    Status = ReadAndSetPhaseData ();
    return Status;
  }
  if ((StrCmp(Argv[1], L"-wPB") == 0) || 
      (StrCmp(Argv[1], L"-wideProgBar") == 0)) {

    if ((Argc - 1) < 2) {
      PrintUsage();
      return EFI_INVALID_PARAMETER;
    }

    Status = ConfigureProgressBarLength ();
    return Status;
  }

  CapsuleFirstIndex = 1;
  NoReset = FALSE;
  if ((StrCmp(Argv[Argc - 1], L"-NR") == 0)) {
    NoReset = TRUE;
    CapsuleLastIndex = Argc - 2;
  } else {
    CapsuleLastIndex = Argc - 1;
  }
  CapsuleNum = CapsuleLastIndex - CapsuleFirstIndex + 1;

  if (CapsuleFirstIndex > CapsuleLastIndex) {
    Print(L"CapsuleApp: NO capsule image.\n");
    return EFI_UNSUPPORTED;
  }
  if (CapsuleNum > MAX_CAPSULE_NUM) {
    Print(L"CapsuleApp: Too many capsule images.\n");
    return EFI_UNSUPPORTED;
  }

  ZeroMem(&CapsuleBuffer, sizeof(CapsuleBuffer));
  ZeroMem(&FileSize, sizeof(FileSize));
  BlockDescriptors = NULL;

  for (Index = 0; Index < CapsuleNum; Index++) {
    CapsuleName = Argv[CapsuleFirstIndex + Index];
    Status = ReadFileToBuffer(CapsuleName, &FileSize[Index], &CapsuleBuffer[Index]);
    if (EFI_ERROR(Status)) {
      Print(L"CapsuleApp: capsule image (%s) is not found.\n", CapsuleName);
      goto Done;
    }
  }

  //
  // Every capsule use 2 descriptor 1 for data 1 for end
  //
  Status = BuildGatherList(CapsuleBuffer, FileSize, CapsuleNum, &BlockDescriptors);
  if (EFI_ERROR(Status)) {
    goto Done;
  }

  //
  // Call the runtime service capsule.
  //
  NeedReset = FALSE;
  for (Index = 0; Index < CapsuleNum; Index++) {
    CapsuleHeaderArray[Index] = (EFI_CAPSULE_HEADER *) CapsuleBuffer[Index];
    if ((CapsuleHeaderArray[Index]->Flags & CAPSULE_FLAGS_PERSIST_ACROSS_RESET) != 0) {
      NeedReset = TRUE;
    }
  }
  CapsuleHeaderArray[CapsuleNum] = NULL;

  //
  // Inquire platform capability of UpdateCapsule.
  //
  Status = gRT->QueryCapsuleCapabilities (CapsuleHeaderArray, CapsuleNum, &MaxCapsuleSize, &ResetType);
  if (EFI_ERROR(Status)) {
    Print (L"CapsuleApp: failed to query capsule capability - %r\n", Status);
    goto Done;
  }

  for (Index = 0; Index < CapsuleNum; Index++) {
    if (FileSize[Index] > MaxCapsuleSize) {
      Print (L"CapsuleApp: capsule is too large to update, %ld is allowed\n", MaxCapsuleSize);
      Status = EFI_UNSUPPORTED;
      goto Done;
    }
  }

  //
  // Check whether the input capsule image has the flag of persist across system reset.
  //
  if (NeedReset) {
    Status = gRT->UpdateCapsule(CapsuleHeaderArray,CapsuleNum,(UINTN) BlockDescriptors);
    if (Status != EFI_SUCCESS) {
      Print (L"CapsuleApp: failed to update capsule - %r\n", Status);
      goto Done;
    }
    //
    // For capsule with CAPSULE_FLAGS_PERSIST_ACROSS_RESET + CAPSULE_FLAGS_INITIATE_RESET,
    // a system reset should have been triggered by gRT->UpdateCapsule() calling above.
    //
    // For capsule with CAPSULE_FLAGS_PERSIST_ACROSS_RESET and without CAPSULE_FLAGS_INITIATE_RESET,
    // check if -NR (no-reset) has been specified or not.
    //
    if (!NoReset) {
      //
      // For capsule who has reset flag and no -NR (no-reset) has been specified, after calling UpdateCapsule service,
      // trigger a system reset to process capsule persist across a system reset.
      //
      gRT->ResetSystem (ResetType, EFI_SUCCESS, 0, NULL);
    }
  } else {
    //
    // For capsule who has no reset flag, only call UpdateCapsule Service without a
    // system reset. The service will process the capsule immediately.
    //
    Status = gRT->UpdateCapsule (CapsuleHeaderArray,CapsuleNum,(UINTN) BlockDescriptors);
    if (Status != EFI_SUCCESS) {
      Print (L"CapsuleApp: failed to update capsule - %r\n", Status);
    }
  }

  Status = EFI_SUCCESS;

Done:
  for (Index = 0; Index < CapsuleNum; Index++) {
    if (CapsuleBuffer[Index] != NULL) {
      FreePool (CapsuleBuffer[Index]);
    }
  }

  CleanGatherList(BlockDescriptors, CapsuleNum);

  return Status;
}

/**
  Convert a Null-terminated Unicode decimal string to a value of type UINTN.

  This function outputs a value of type UINTN by interpreting the contents of
  the Unicode string specified by String as a decimal number. The format of the
  input Unicode string String is:

                  [spaces] [decimal digits].

  The valid decimal digit character is in the range [0-9]. The function will
  ignore the pad space, which includes spaces or tab characters, before
  [decimal digits]. The running zero in the beginning of [decimal digits] will
  be ignored. Then, the function stops at the first character that is a not a
  valid decimal character or a Null-terminator, whichever one comes first.

  If String has no valid decimal digits in the above format, then RETURN_UNSUPPORTED
  is returned.

  @param  String                   Pointer to a Null-terminated Unicode string.
  @param  Data                     Pointer to the converted value.

  @retval RETURN_SUCCESS           Value is translated from String.
  @retval RETURN_UNSUPPORTED       If the number represented by String cannot
                                   be converted to UNINT for any reason.

**/
RETURN_STATUS
EFIAPI
ConvertStringToUInt (
  IN  CONST CHAR16             *String,
  OUT       UINTN              *ConvertedValue
  )
{
    EFI_STATUS                    Status = EFI_SUCCESS;
    CHAR16                       *EndPointer = NULL;

    Status = StrDecimalToUintnS (String, &EndPointer, ConvertedValue);
    // TODO: Check status

    if ((*EndPointer != L'\0'))
    {
      /* If the string was a pure number, successful conversion 
      should make EndPointer point to the NULL terminator of the string*/
      // Error: Found characters and numbers mixed
      *ConvertedValue = 0;
      Status = EFI_UNSUPPORTED;
      goto Exit;
    }
    else if (*ConvertedValue == 0)
    {
      /* User has either FwEntry count as 0 or has given and invalid
      number that resulted in string to integer coversion */
      Status = EFI_UNSUPPORTED;
      goto Exit;
    }

Exit:
    return Status;
}

/**
  Read user supplied wide progress bar on/off configuration setting
**/
EFI_STATUS
ConfigureProgressBarLength (
)
{
  EFI_STATUS        Status = EFI_SUCCESS;
  BOOLEAN           EnableWideProgressBar = FALSE;
  UINTN             ProgressBarVarSize = sizeof(EnableWideProgressBar);

  if ((Argc < 2) ||
    (Argc > 3)) {
    Print(L"Invalid argument count detected for progress bar configuration, aborting...\n");
    return EFI_INVALID_PARAMETER;
  }

  if (Argc == 2)
  {
    Print(L"\nCapsuleApp is clearing wide progress bar setting\n");
    // Make the size as zero so that the NV variable gets deleted
    // during set operation below
    ProgressBarVarSize = 0;
  }
  else if (StrCmp(Argv[2], L"ON") == 0) {
    Print(L"\nCapsuleApp is switching ON wide progress bar\n");
    EnableWideProgressBar = TRUE;
  }
  else if (StrCmp(Argv[2], L"OFF") == 0) {
    Print(L"\nCapsuleApp is switching OFF wide progress bar\n");
    EnableWideProgressBar = FALSE;
  }
  else {
    Status = EFI_INVALID_PARAMETER;
    Print(L"\nInvalid/no parameter is sent, aborting\n");
    goto Exit;
  }

  Status = gRT->SetVariable (
    PROGRESS_BAR_CONFIG_NV,
    &gQcomTokenSpaceGuid,
    (EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS),
    ProgressBarVarSize,
    &EnableWideProgressBar
    );
  if (EFI_ERROR(Status)) {
    Print(L"ERROR: CapsuleApp failed to record new progress bar width setting\n");
    goto Exit;
  }

Exit:
  //Clearing the allocated memory before exit

  return Status;
}

/**
  Read user supplied reset data to a structure and set the NV variable with that data
**/
EFI_STATUS
ReadAndSetPhaseData (
)
{
  EFI_STATUS   Status = EFI_SUCCESS;
  UINT8        ResetPhaseData[QCOM_TESTHOOK_MAX] = {0};
  UINTN        ResetPhaseDataSize = sizeof (ResetPhaseData);
  UINTN        ArgumentIndex = 0;
  UINTN        FwEntryToReset = 0;
  CHAR16       *FwEntryToResetStrPtr = NULL;

  if ((Argc - 1) < 2) {
    Print(L"Invalid argument count detected for reset phase configuration, aborting...\n");
    return EFI_INVALID_PARAMETER;
  }

  //
  //Try to get the variable before we proceed to set with new content.
  //Return status is will not be considererd here as the first instance will
  //not contain such variable.
  //
  Status = gRT->GetVariable (
              RESET_PHASE_DATA_NV,
              &gQcomTokenSpaceGuid,
              NULL,
              &ResetPhaseDataSize,
              &ResetPhaseData
              );

  if ((EFI_ERROR (Status)) ||
    (ResetPhaseDataSize != sizeof (ResetPhaseData)))
  {
    SetMem (&ResetPhaseData, sizeof (ResetPhaseData), NO_RESET_IN_CURRENT_PHASE);
    Status = EFI_SUCCESS;
  }

  if (Argv[2][0] == L'V') {
     if (Argv[2][1] != L'\0' || Argc > 3) {     
       Print(L"Invalid argument count detected for View option, aborting...\n");
       Status = EFI_INVALID_PARAMETER;
       goto Exit;
     }
  } else {
    for (ArgumentIndex = 2; ArgumentIndex < Argc; ArgumentIndex++) {

      FwEntryToResetStrPtr = &Argv[ArgumentIndex][1];
      if (*FwEntryToResetStrPtr == L'\0')
      {
        // No Fw entry mentioned for the current phase. Assume it a request to
        // clear the setting for the current phase
        FwEntryToReset = NO_RESET_IN_CURRENT_PHASE;
      }
      else
      {
        Status = ConvertStringToUInt (FwEntryToResetStrPtr, &FwEntryToReset);
        if (EFI_ERROR(Status))
        {
          Print(L"Invalid FW entry count detected, aborting...\n");
          goto Exit;
        }

        if (FwEntryToReset > MAX_ALLOWED_FW_ENTRIES) {
          Print(L"Fw entry count to reset exceeded maximum allowed value, aborting...\n");
          Status = EFI_INVALID_PARAMETER;
          goto Exit;
        }
      }

      if (Argv[ArgumentIndex][0] == L'B') {
        ResetPhaseData[QCOM_TESTHOOK_BACKUP_PHASE]   = FwEntryToReset;
      }
      else if (Argv[ArgumentIndex][0] == L'U') {
        ResetPhaseData[QCOM_TESTHOOK_UPDATE_PHASE]   = FwEntryToReset;
      }
      else if (Argv[ArgumentIndex][0] == L'D') {
        ResetPhaseData[QCOM_TESTHOOK_DELETE_PHASE]   = FwEntryToReset;
      }
      else if (Argv[ArgumentIndex][0] == L'R') {
        ResetPhaseData[QCOM_TESTHOOK_ROLLBACK_PHASE] = FwEntryToReset;
      }
      else {
        Status = EFI_UNSUPPORTED;
        Print(L"Supplied input parameters are unsupported, aborting...\n");
        goto Exit;
      }
    }

    Status = gRT->SetVariable (
                    RESET_PHASE_DATA_NV,
                    &gQcomTokenSpaceGuid,
                    (EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS),
                    sizeof (ResetPhaseData),
                    &ResetPhaseData
                    );
  }
    
  if(Status == EFI_SUCCESS) {
    Print(L"\n");
    Print(L"###################################\n");
    Print(L"####      Reset phase data     ####\n");
    Print(L"###################################\n");
    Print(L"Fw entry count for Backup    : %d\n",   ResetPhaseData[QCOM_TESTHOOK_BACKUP_PHASE]);
    Print(L"Fw entry count for Update    : %d\n",   ResetPhaseData[QCOM_TESTHOOK_UPDATE_PHASE]);
    Print(L"Fw entry count for Delete    : %d\n",   ResetPhaseData[QCOM_TESTHOOK_DELETE_PHASE]);
    Print(L"Fw entry count for Rollback  : %d\n\n", ResetPhaseData[QCOM_TESTHOOK_ROLLBACK_PHASE]);
  }
  else {
    Print(L"ERROR: CapsuleApp failed to record new reset phase data\n");
    goto Exit;
  }

Exit:

  return Status;
}
