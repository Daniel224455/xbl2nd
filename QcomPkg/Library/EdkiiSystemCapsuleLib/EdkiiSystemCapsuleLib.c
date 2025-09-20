/** @file
  EDKII System Capsule library.

  EDKII System Capsule library instance.

  CapsuleAuthenticateSystemFirmware(), ExtractAuthenticatedImage() will receive
  untrusted input and do basic validation.

  Copyright (c) 2017, 2019 Qualcomm Technologies, Inc.  All Rights Reserved.
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



 when           who         what, where, why
 --------       ---         --------------------------------------------------
 2017/07/20     gparam      Corrected the version compatibility check + Converted
                            logs from error conditions to WARN/ERROR types
                            depending on the need to capture this in PROD/NON-PROD builds
 2017/04/22     gparam      New API to extract the metadata for using it in very
                            early stage of udpate to retrieve the LastAttemptVersion
                            for logging early errors + Changes to centralize
                            the recording of update status in DPP for future use +
                            Changes to check version matching before starting update
 2017/01/03     gparam      Initial version
*/

#include <PiDxe.h>

#include <Guid/SystemResourceTable.h>
#include <Guid/FirmwareContentsSigned.h>
#include <Guid/WinCertificate.h>
#include <EdkiiSystemFmpCapsule.h>
#include <Guid/WinCertificate.h>
#include <Guid/ImageAuthentication.h>

#include <Library/FwCommonLib.h>
#include <Library/FwUpdateLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/EdkiiSystemCapsuleLib.h>
#include <Library/FmpAuthenticationLib.h>

#include <Protocol/FirmwareManagement.h>

/**
  Check if a block of buffer is erased.

  @param[in] ErasePolarity  Erase polarity attribute of the firmware volume
  @param[in] InBuffer       The buffer to be checked
  @param[in] BufferSize     Size of the buffer in bytes

  @retval    TRUE           The block of buffer is erased
  @retval    FALSE          The block of buffer is not erased
**/
BOOLEAN
IsBufferErased (
  IN UINT8    ErasePolarity,
  IN VOID     *InBuffer,
  IN UINTN    BufferSize
  )
{
  UINTN   Count;
  UINT8   EraseByte;
  UINT8   *Buffer;

  if(ErasePolarity == 1) {
    EraseByte = 0xFF;
  } else {
    EraseByte = 0;
  }

  Buffer = InBuffer;
  for (Count = 0; Count < BufferSize; Count++) {
    if (Buffer[Count] != EraseByte) {
      return FALSE;
    }
  }

  return TRUE;
}

/**
  Get Section buffer pointer by SectionType and SectionInstance.

  @param[in]   SectionBuffer     The buffer of section
  @param[in]   SectionBufferSize The size of SectionBuffer in bytes
  @param[in]   SectionType       The SectionType of Section to be found
  @param[in]   SectionInstance   The Instance of Section to be found
  @param[out]  OutSectionBuffer  The section found, including SECTION_HEADER
  @param[out]  OutSectionSize    The size of section found, including SECTION_HEADER

  @retval TRUE  The FFS buffer is found.
  @retval FALSE The FFS buffer is not found.
**/
BOOLEAN
GetSectionByType (
  IN VOID                  *SectionBuffer,
  IN UINT32                SectionBufferSize,
  IN EFI_SECTION_TYPE      SectionType,
  IN UINTN                 SectionInstance,
  OUT VOID                 **OutSectionBuffer,
  OUT UINTN                *OutSectionSize
  )
{
  EFI_COMMON_SECTION_HEADER             *SectionHeader;
  UINTN                                 SectionSize;
  UINTN                                 Instance;

  DEBUG ((DEBUG_INFO, "GetSectionByType - Buffer: 0x%08x - 0x%08x\n", SectionBuffer, SectionBufferSize));

  //
  // Find Section
  //
  SectionHeader = SectionBuffer;

  Instance = 0;
  while ((UINTN)SectionHeader < (UINTN)SectionBuffer + SectionBufferSize) {
    DEBUG ((DEBUG_INFO, "GetSectionByType - Section: 0x%08x\n", SectionHeader));
    if (IS_SECTION2(SectionHeader)) {
      SectionSize = SECTION2_SIZE(SectionHeader);
    } else {
      SectionSize = SECTION_SIZE(SectionHeader);
    }

    if (SectionHeader->Type == SectionType) {
      if (Instance == SectionInstance) {
        *OutSectionBuffer = (UINT8 *)SectionHeader;
        *OutSectionSize = SectionSize;
        DEBUG((DEBUG_INFO, "GetSectionByType - 0x%x - 0x%x\n", *OutSectionBuffer, *OutSectionSize));
        return TRUE;
      } else {
        DEBUG((DEBUG_INFO, "GetSectionByType - find section instance %x\n", Instance));
        Instance++;
      }
    } else {
      //
      // Skip other section type
      //
      DEBUG ((DEBUG_INFO, "GetSectionByType - other section type 0x%x\n", SectionHeader->Type));
    }

    //
    // Next Section
    //
    SectionHeader = (EFI_COMMON_SECTION_HEADER *)((UINTN)SectionHeader + ALIGN_VALUE(SectionSize, 4));
  }

  return FALSE;
}

/**
  Get FFS buffer pointer by FileName GUID and FileType.

  @param[in]   FdStart          The System Firmware FD image
  @param[in]   FdSize           The size of System Firmware FD image
  @param[in]   FileName         The FileName GUID of FFS to be found
  @param[in]   Type             The FileType of FFS to be found
  @param[out]  OutFfsBuffer     The FFS buffer found, including FFS_FILE_HEADER
  @param[out]  OutFfsBufferSize The size of FFS buffer found, including FFS_FILE_HEADER

  @retval TRUE  The FFS buffer is found.
  @retval FALSE The FFS buffer is not found.
**/
BOOLEAN
GetFfsByName (
  IN VOID                  *FdStart,
  IN UINTN                  FdSize,
  IN EFI_GUID              *FileName,
  IN EFI_FV_FILETYPE        Type,
  OUT VOID                **OutFfsBuffer,
  OUT UINTN                *OutFfsBufferSize
  )
{
  UINTN                                     FvSize;
  EFI_FIRMWARE_VOLUME_HEADER                *FvHeader;
  EFI_FIRMWARE_VOLUME_EXT_HEADER            *FvExtHeader;
  EFI_FFS_FILE_HEADER                       *FfsHeader;
  UINT32                                    FfsSize;
  UINTN                                     TestLength;
  BOOLEAN                                   FvFound;

  DEBUG ((DEBUG_INFO, "GetFfsByName - FV: 0x%08x - 0x%08x\n", (UINTN)FdStart, (UINTN)FdSize));

  FvFound = FALSE;
  FvHeader = (EFI_FIRMWARE_VOLUME_HEADER *)FdStart;
  while ((UINTN)FvHeader < (UINTN)FdStart + FdSize - 1) {
    FvSize = (UINTN)FdStart + FdSize - (UINTN)FvHeader;

    if (FvHeader->Signature != EFI_FVH_SIGNATURE) {
      FvHeader = (EFI_FIRMWARE_VOLUME_HEADER *)((UINTN)FvHeader + SIZE_4KB);
      continue;
    }
    DEBUG((DEBUG_INFO, "checking FV....0x%08x - 0x%x\n", FvHeader, FvHeader->FvLength));
    FvFound = TRUE;
    if (FvHeader->FvLength > FvSize) {
      DEBUG((DEBUG_ERROR, "GetFfsByName - FvSize: 0x%08x, MaxSize - 0x%08x\n", (UINTN)FvHeader->FvLength, (UINTN)FvSize));
      return FALSE;
    }
    FvSize = (UINTN)FvHeader->FvLength;

    //
    // Find FFS
    //
    if (FvHeader->ExtHeaderOffset != 0) {
      FvExtHeader = (EFI_FIRMWARE_VOLUME_EXT_HEADER *)((UINT8 *)FvHeader + FvHeader->ExtHeaderOffset);
      FfsHeader = (EFI_FFS_FILE_HEADER *)((UINT8 *)FvExtHeader + FvExtHeader->ExtHeaderSize);
    } else {
      FfsHeader = (EFI_FFS_FILE_HEADER *)((UINT8 *)FvHeader + FvHeader->HeaderLength);
    }
    FfsHeader = (EFI_FFS_FILE_HEADER *)((UINTN)FvHeader + ALIGN_VALUE((UINTN)FfsHeader - (UINTN)FvHeader, 8));

    while ((UINTN)FfsHeader < (UINTN)FvHeader + FvSize - 1) {
      DEBUG((DEBUG_INFO, "GetFfsByName - FFS: 0x%08x\n", FfsHeader));
      TestLength = (UINTN)((UINTN)FvHeader + FvSize - (UINTN)FfsHeader);
      if (TestLength > sizeof(EFI_FFS_FILE_HEADER)) {
        TestLength = sizeof(EFI_FFS_FILE_HEADER);
      }
      if (IsBufferErased(1, FfsHeader, TestLength)) {
        break;
      }

      if (IS_FFS_FILE2(FfsHeader)) {
        FfsSize = FFS_FILE2_SIZE(FfsHeader);
      } else {
        FfsSize = FFS_FILE_SIZE(FfsHeader);
      }

      if (CompareGuid(FileName, &FfsHeader->Name) &&
          ((Type == EFI_FV_FILETYPE_ALL) || (FfsHeader->Type == Type))) {
        //
        // Check section
        //
        *OutFfsBuffer = FfsHeader;
        *OutFfsBufferSize = FfsSize;
        return TRUE;
      } else {
        //
        // Any other type is not allowed
        //
        DEBUG((DEBUG_INFO, "GetFfsByName - other FFS type 0x%x, name %g\n", FfsHeader->Type, &FfsHeader->Name));
      }

      //
      // Next File
      //
      FfsHeader = (EFI_FFS_FILE_HEADER *)((UINTN)FfsHeader + ALIGN_VALUE(FfsSize, 8));
    }

    //
    // Next FV
    //
    FvHeader = (VOID *)(UINTN)((UINTN)FvHeader + FvHeader->FvLength);
    DEBUG((DEBUG_ERROR, "Next FV....0x%08x - 0x%x\n", FvHeader, FvHeader->FvLength));
  }

  if (!FvFound) {
    DEBUG((DEBUG_ERROR, "GetFfsByName - NO FV Found\n"));
  }
  return FALSE;
}

/**
  The constructor function.

  @retval EFI_SUCCESS   The constructor successfully .
**/
EFI_STATUS
EFIAPI
EdkiiSystemCapsuleLibConstructor (
  VOID
  )
{
  return EFI_SUCCESS;
}
