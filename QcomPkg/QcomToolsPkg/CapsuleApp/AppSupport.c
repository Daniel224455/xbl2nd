/** @file
  A shell application that triggers capsule update process.

  Copyright (c) 2017 Qualcomm Technologies, Inc. All rights reserved.
  Portions Copyright (c) 2016 - 2017, Intel Corporation. All rights reserved.<BR>
  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/SimpleFileSystem.h>
#include <Protocol/ShellParameters.h>
#include <Protocol/Shell.h>
#include <Guid/FileInfo.h>

#define IS_HYPHEN(a)               ((a) == L'-')
#define IS_NULL(a)                 ((a) == L'\0')

UINTN  Argc;
CHAR16 **Argv;
EFI_SHELL_PROTOCOL      *mShellProtocol = NULL;

/**

  This function parse application ARG.

  @return Status
**/
EFI_STATUS
GetArg (
  VOID
  )
{
  EFI_STATUS                    Status;
  EFI_SHELL_PARAMETERS_PROTOCOL *ShellParameters;

  Status = gBS->HandleProtocol (
                  gImageHandle,
                  &gEfiShellParametersProtocolGuid,
                  (VOID**)&ShellParameters
                  );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  Argc = ShellParameters->Argc;
  Argv = ShellParameters->Argv;
  return EFI_SUCCESS;
}

#if defined (QCOM_EDK2_PATCH)
/**
  Converts a list of string to a specified buffer.

  @param[out] Buf             The output buffer that contains the string.
  @param[in]  BufferLength    The length of the buffer
  @param[in]  Str             The input string that contains the hex number

  @retval EFI_SUCCESS    The string was successfully converted to the buffer.

**/
EFI_STATUS
InternalStrToBuf (
  OUT UINT8    *Buf,
  IN  UINTN    BufferLength,
  IN  CHAR16   *Str
  )
{
  UINTN       Index;
  UINTN       StrLength;
  UINT8       Digit;
  UINT8       Byte;

  Digit = 0;

  //
  // Two hex char make up one byte
  //
  StrLength = BufferLength * sizeof (CHAR16);

  for(Index = 0; Index < StrLength; Index++, Str++) {

    if ((*Str >= L'a') && (*Str <= L'f')) {
      Digit = (UINT8) (*Str - L'a' + 0x0A);
    } else if ((*Str >= L'A') && (*Str <= L'F')) {
      Digit = (UINT8) (*Str - L'A' + 0x0A);
    } else if ((*Str >= L'0') && (*Str <= L'9')) {
      Digit = (UINT8) (*Str - L'0');
    } else {
      return EFI_INVALID_PARAMETER;
    }

    //
    // For odd characters, write the upper nibble for each buffer byte,
    // and for even characters, the lower nibble.
    //
    if ((Index & 1) == 0) {
      Byte = (UINT8) (Digit << 4);
    } else {
      Byte = Buf[Index / 2];
      Byte &= 0xF0;
      Byte = (UINT8) (Byte | Digit);
    }

    Buf[Index / 2] = Byte;
  }

  return EFI_SUCCESS;
}

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
  )
{
  //
  // Get the first UINT32 data
  //
  Guid->Data1 = (UINT32) StrHexToUint64  (Str);
  while (!IS_HYPHEN (*Str) && !IS_NULL (*Str)) {
    Str ++;
  }

  if (IS_HYPHEN (*Str)) {
    Str++;
  } else {
    return EFI_UNSUPPORTED;
  }

  //
  // Get the second UINT16 data
  //
  Guid->Data2 = (UINT16) StrHexToUint64  (Str);
  while (!IS_HYPHEN (*Str) && !IS_NULL (*Str)) {
    Str ++;
  }

  if (IS_HYPHEN (*Str)) {
    Str++;
  } else {
    return EFI_UNSUPPORTED;
  }

  //
  // Get the third UINT16 data
  //
  Guid->Data3 = (UINT16) StrHexToUint64  (Str);
  while (!IS_HYPHEN (*Str) && !IS_NULL (*Str)) {
    Str ++;
  }

  if (IS_HYPHEN (*Str)) {
    Str++;
  } else {
    return EFI_UNSUPPORTED;
  }

  //
  // Get the following 8 bytes data
  //
  InternalStrToBuf (&Guid->Data4[0], 2, Str);
  //
  // Skip 2 byte hex chars
  //
  Str += 2 * 2;

  if (IS_HYPHEN (*Str)) {
    Str++;
  } else {
    return EFI_UNSUPPORTED;
  }
  InternalStrToBuf (&Guid->Data4[2], 6, Str);

  return EFI_SUCCESS;
}
#endif // #if defined (QCOM_EDK2_PATCH)

/**
  Get shell protocol.

  @return Pointer to shell protocol.
**/
EFI_SHELL_PROTOCOL *
GetShellProtocol (
  VOID
  )
{
  EFI_STATUS            Status;

  if (mShellProtocol == NULL) {
    Status = gBS->LocateProtocol (
                    &gEfiShellProtocolGuid,
                    NULL,
                    (VOID **) &mShellProtocol
                    );
    if (EFI_ERROR (Status)) {
      mShellProtocol = NULL;
    }
  }

  return mShellProtocol;
}

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
  )
{
  EFI_STATUS                        Status;
  EFI_SHELL_PROTOCOL                *ShellProtocol;
  SHELL_FILE_HANDLE                 Handle;
  UINT64                            FileSize;
  UINTN                             TempBufferSize;
  VOID                              *TempBuffer;

  ShellProtocol = GetShellProtocol();
  if (ShellProtocol == NULL) {
    return EFI_NOT_FOUND;
  }

  //
  // Open file by FileName.
  //
  Status = ShellProtocol->OpenFileByName (
                            FileName,
                            &Handle,
                            EFI_FILE_MODE_READ
                            );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Get the file size.
  //
  Status = ShellProtocol->GetFileSize (Handle, &FileSize);
  if (EFI_ERROR (Status)) {
    ShellProtocol->CloseFile (Handle);
    return Status;
  }

  TempBufferSize = (UINTN) FileSize;
  TempBuffer = AllocateZeroPool (TempBufferSize);
  if (TempBuffer == NULL) {
    ShellProtocol->CloseFile (Handle);
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // Read the file data to the buffer
  //
  Status = ShellProtocol->ReadFile (
                            Handle,
                            &TempBufferSize,
                            TempBuffer
                            );
  if (EFI_ERROR (Status)) {
    ShellProtocol->CloseFile (Handle);
    return Status;
  }

  ShellProtocol->CloseFile (Handle);

  *BufferSize = TempBufferSize;
  *Buffer     = TempBuffer;
  return EFI_SUCCESS;
}

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
  )
{
  EFI_STATUS                        Status;
  EFI_SHELL_PROTOCOL                *ShellProtocol;
  SHELL_FILE_HANDLE                 Handle;
  EFI_FILE_INFO                     *FileInfo;
  UINTN                             TempBufferSize;

  ShellProtocol = GetShellProtocol();
  if (ShellProtocol == NULL) {
    return EFI_NOT_FOUND;
  }

  //
  // Open file by FileName.
  //
  Status = ShellProtocol->OpenFileByName (
                            FileName,
                            &Handle,
                            EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE | EFI_FILE_MODE_CREATE
                            );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Empty the file contents.
  //
  FileInfo = ShellProtocol->GetFileInfo (Handle);
  if (FileInfo == NULL) {
    ShellProtocol->CloseFile (Handle);
    return EFI_DEVICE_ERROR;
  }

  //
  // If the file size is already 0, then it has been empty.
  //
  if (FileInfo->FileSize != 0) {
    //
    // Set the file size to 0.
    //
    FileInfo->FileSize = 0;
    Status = ShellProtocol->SetFileInfo (Handle, FileInfo);
    if (EFI_ERROR (Status)) {
      FreePool (FileInfo);
      ShellProtocol->CloseFile (Handle);
      return Status;
    }
  }
  FreePool (FileInfo);

  //
  // Write the file data from the buffer
  //
  TempBufferSize = BufferSize;
  Status = ShellProtocol->WriteFile (
                            Handle,
                            &TempBufferSize,
                            Buffer
                            );
  if (EFI_ERROR (Status)) {
    ShellProtocol->CloseFile (Handle);
    return Status;
  }

  ShellProtocol->CloseFile (Handle);

  return EFI_SUCCESS;
}

