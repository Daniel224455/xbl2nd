/** @file
  EDKII System Capsule library.

Copyright (c) 2017 Qualcomm Technologies, Inc.  All Rights Reserved.
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
  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.



 when           who         what, where, why
 --------       ---         --------------------------------------------------
 04/18/17       gparam      Changes to centralize the recording of update status
                            in DPP for future use
 03/01/17       gparam      Initial version
*/


#ifndef __EDKII_SYSTEM_CAPSULE_LIB_H__
#define __EDKII_SYSTEM_CAPSULE_LIB_H__

#include <EdkiiSystemFmpCapsule.h>

/**
  Extract ImageFmpInfo from system firmware.

  @param[in]  SystemFirmwareImage     The System Firmware image.
  @param[in]  SystemFirmwareImageSize The size of the System Firmware image in bytes.
  @param[out] ImageFmpInfo            The ImageFmpInfo.
  @param[out] ImageFmpInfoSize        The size of the ImageFmpInfo in bytes.

  @retval TRUE  The ImageFmpInfo is extracted.
  @retval FALSE The ImageFmpInfo is not extracted.
**/
BOOLEAN
EFIAPI
ExtractSystemFirmwareImageFmpInfo (
  IN VOID                                      *SystemFirmwareImage,
  IN UINTN                                      SystemFirmwareImageSize,
  OUT SYSTEM_FIRMWARE_IMAGE_DESCRIPTOR        **ImageFmpInfo,
  OUT UINTN                                    *ImageFmpInfoSize
  );

/**
  Extract the driver FV from an authenticated image.

  @param[in]  AuthenticatedImage      The authenticated capsule image.
  @param[in]  AuthenticatedImageSize  The size of the authenticated capsule image in bytes.
  @param[out] DriverFvImage           The driver FV image.
  @param[out] DriverFvImageSize       The size of the driver FV image in bytes.

  @retval TRUE  The driver Fv is extracted.
  @retval FALSE The driver Fv is not extracted.
**/
BOOLEAN
EFIAPI
ExtractDriverFvImage (
  IN VOID                         *AuthenticatedImage,
  IN UINTN                        AuthenticatedImageSize,
  OUT VOID                        **DriverFvImage,
  OUT UINTN                       *DriverFvImageSize
  );

/**
  Extract the config image from an authenticated image.

  @param[in]  AuthenticatedImage      The authenticated capsule image.
  @param[in]  AuthenticatedImageSize  The size of the authenticated capsule image in bytes.
  @param[out] ConfigImage             The config image.
  @param[out] ConfigImageSize         The size of the config image in bytes.

  @retval TRUE  The config image is extracted.
  @retval FALSE The config image is not extracted.
**/
BOOLEAN
EFIAPI
ExtractConfigImage (
  IN VOID                         *AuthenticatedImage,
  IN UINTN                        AuthenticatedImageSize,
  OUT VOID                        **ConfigImage,
  OUT UINTN                       *ConfigImageSize
  );

/**
  Extract the System Firmware image from an authenticated image.

  @param[in]  AuthenticatedImage      The authenticated capsule image.
  @param[in]  AuthenticatedImageSize  The size of the authenticated capsule image in bytes.
  @param[out] SystemFirmwareImage     The System Firmware image.
  @param[out] SystemFirmwareImageSize The size of the System Firmware image in bytes.

  @retval TRUE  The System Firmware image is extracted.
  @retval FALSE The System Firmware image is not extracted.
**/
BOOLEAN
EFIAPI
ExtractSystemFirmwareImage (
  IN VOID                         *AuthenticatedImage,
  IN UINTN                        AuthenticatedImageSize,
  OUT VOID                        **SystemFirmwareImage,
  OUT UINTN                       *SystemFirmwareImageSize
  );

/**
  Extract the authenticated image from an FMP capsule image.

  @param[in]  Image                   The FMP capsule image, including EFI_FIRMWARE_IMAGE_AUTHENTICATION.
  @param[in]  ImageSize               The size of FMP capsule image in bytes.
  @param[out] LastAttemptStatus       The last attempt status, which will be recorded in ESRT and FMP EFI_FIRMWARE_IMAGE_DESCRIPTOR.
  @param[out] AuthenticatedImage      The authenticated capsule image, excluding EFI_FIRMWARE_IMAGE_AUTHENTICATION.
  @param[out] AuthenticatedImageSize  The size of the authenticated capsule image in bytes.
  @param[out] PayloadHeaderV3         Payload header structure (optional)
  @param[out] QcResultFlag            QCFirmwareUpdate result flag

  @retval TRUE  The authenticated image is extracted.
  @retval FALSE The authenticated image is not extracted.
**/
BOOLEAN
EFIAPI
ExtractAuthenticatedImage (
  IN VOID                          *Image,
  IN UINTN                          ImageSize,
  OUT UINT32                       *LastAttemptStatus,
  OUT VOID                        **AuthenticatedImage,
  OUT UINTN                        *AuthenticatedImageSize,
  OUT QPAYLOAD_METADATA_HEADER_V3  *PayloadHeaderV3,
  OUT QCFW_UPDATE_FLAG_VALUE       *QcResultFlag
  );

/**
  Authenticated system firmware FMP capsule image.

  Caution: This function may receive untrusted input.

  @param[in]  Image                   The FMP capsule image, including EFI_FIRMWARE_IMAGE_AUTHENTICATION.
  @param[in]  ImageSize               The size of FMP capsule image in bytes.
  @param[in]  ForceVersionMatch       TRUE: The version of capsule must be as same as the version of current image.
                                      FALSE: The version of capsule must be as same as greater than the lowest
                                             supported version of current image.
  @param[out] LastAttemptVersion      The last attempt version, which will be recorded in ESRT and FMP EFI_FIRMWARE_IMAGE_DESCRIPTOR.
  @param[out] LastAttemptStatus       The last attempt status, which will be recorded in ESRT and FMP EFI_FIRMWARE_IMAGE_DESCRIPTOR.
  @param[out] LowestSupportedVersion  Lowest supported version of the new image getting updated
  @param[out] AuthenticatedImage      The authenticated capsule image, excluding EFI_FIRMWARE_IMAGE_AUTHENTICATION.
  @param[out] AuthenticatedImageSize  The size of the authenticated capsule image in bytes.
  @param[out] QcResultFlag            QCFirmwareUpdate result flag

  @retval TRUE  Authentication passes and the authenticated image is extracted.
  @retval FALSE Authentication fails and the authenticated image is not extracted.
**/
EFI_STATUS
EFIAPI
CapsuleAuthenticateSystemFirmware (
  IN VOID                         *Image,
  IN UINTN                         ImageSize,
  IN BOOLEAN                       ForceVersionMatch,
  OUT UINT32                      *LastAttemptVersion,
  OUT UINT32                      *LastAttemptStatus,
  OUT UINT32                      *LowestSupportedVersion,
  OUT VOID                       **AuthenticatedImage,
  OUT UINTN                       *AuthenticatedImageSize,
  OUT QCFW_UPDATE_FLAG_VALUE      *QcResultFlag
  );

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
  );
#endif
