/** @file SystemFMPDxe.c

  Qualcomm system firmware update driver

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
 2017/07/20     gparam      Updated the default system-formware GUID to a generic
                            one instead of using MTP specific one.
 2017/04/18     gparam      Changes to centralize the recording of update status
                            in DPP for future use
 04/18/17       gparam      Removed unused/redundant definitions/parameters +
                            Change to record update status in private data so
                            that the same is available through subsequent
                            GetimageInfo calls
 03/01/17       ssumit      Initial version
*/

#include <Guid/FmpCapsule.h>
#include <Guid/SystemResourceTable.h>

#include <Protocol/EsrtManagement.h>
#include <Protocol/FirmwareManagement.h>

#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/QcomLib.h>
#include <Library/DevicePathLib.h>
#include <Library/HobLib.h>
#include <Library/UefiCfgLib.h>
#include <Library/UefiRuntimeLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/FwUpdateLib.h>
#include <Library/EdkiiSystemCapsuleLib.h>
#include <Library/FwCommonLib.h>

#include <EdkiiSystemFmpCapsule.h>

#define FMP_SIGNATURE                SIGNATURE_32('F', 'W', 'M', 'P')

/**
  Returns a pointer to the SYSTEM_FMP_PRIVATE_DATA structure from the input a as Fmp.

  If the signatures matches, then a pointer to the data structure that contains
  a specified field of that data structure is returned.

  @param  a              Pointer to the field specified by ServiceBinding within
                         a data structure of type SYSTEM_FMP_PRIVATE_DATA.

**/
#define SYSTEM_FMP_PRIVATE_DATA_FROM_FMP(a) \
  CR ( \
  (a), \
  SYSTEM_FMP_PRIVATE_DATA, \
  FmpProtocol, \
  FMP_SIGNATURE \
  )

typedef struct {
  //
  // Structure Identification
  //
  UINTN                                           Signature;
  //
  // Upper Level API
  //
  EFI_FIRMWARE_MANAGEMENT_PROTOCOL                FmpProtocol;

  //TODO: Giri: check the relevant variables
  EFI_HANDLE                                      Handle;
  UINT8                                           DescriptorCount;
  SYSTEM_FIRMWARE_IMAGE_DESCRIPTOR               *ImageDescriptor;
} SYSTEM_FMP_PRIVATE_DATA;

/**
  Authenticate and update System Firmware image.

  Caution: This function may receive untrusted input.

  @param[in]  Image                 System FMP capsule image.
  @param[in]  ImageSize             The size of the System FMP capsule image in bytes.
  @param[out] LastAttemptVersion    The last attempt version, which will be recorded in ESRT and FMP EFI_FIRMWARE_IMAGE_DESCRIPTOR.
  @param[out] LastAttemptStatus     The last attempt status, which will be recorded in ESRT and FMP EFI_FIRMWARE_IMAGE_DESCRIPTOR.
  @param[out] LowestSupportedVersion  Lowest supported version of the new image getting updated
  @param[out] QcResultFlag          QCFirmwareUpdate result flag

  @retval EFI_SUCCESS               System FMP capsule passes authentication and the System Firmware image is updated.
  @retval EFI_SECURITY_VIOLATION    System FMP capsule fails authentication and the System Firmware image is not updated.
  @retval EFI_WRITE_PROTECTED       The flash device is read only.
**/
EFI_STATUS
SystemFirmwareAuthenticatedUpdate (
  IN VOID                         *Image,
  IN UINTN                         ImageSize,
  OUT UINT32                      *LastAttemptVersion,
  OUT UINT32                      *LastAttemptStatus,
  OUT UINT32                      *LowestSupportedVersion,
  OUT QCFW_UPDATE_FLAG_VALUE      *QcResultFlag
  )
{
  EFI_STATUS                  Status                  = EFI_SUCCESS;
  VOID                       *AuthenticatedImage      = NULL;
  UINTN                       AuthenticatedImageSize  = 0;

  AuthenticatedImage     = NULL;
  AuthenticatedImageSize = 0;

  DEBUG((DEBUG_INFO, "SystemFirmwareAuthenticatedUpdate...\n"));

  Status = CapsuleAuthenticateSystemFirmware(
              Image,
              ImageSize,
              FALSE,
              LastAttemptVersion,
              LastAttemptStatus,
              LowestSupportedVersion,
              &AuthenticatedImage,
              &AuthenticatedImageSize,
              QcResultFlag
              );
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "Firmware authentication failed. Status - %r\n", Status));
    return Status;
  }

  DEBUG ((DEBUG_INFO, "Attempting to start: Firmware update\n"));

  Status = RunFwUpdateMain(
              AuthenticatedImage,
              AuthenticatedImageSize,
              LastAttemptStatus,
              QcResultFlag
              );
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "Firmware update failed. Status - %r\n", Status));
    return Status;
  }

  // TODO: Giri: Need to de-initialize all global variables used in
  // FmpDxe and the firmware update libs as two masstorage capsules 
  // pointing to system firmware should not use any data filled in 
  // during the last capsule update happened in the current boot itself
  
  DEBUG((DEBUG_INFO, "Firmware update Done\n"));

  return EFI_SUCCESS;
}

/**
  Returns information about the current firmware image(s) of the device.

  This function allows a copy of the current firmware image to be created and saved.
  The saved copy could later been used, for example, in firmware image recovery or rollback.

  @param[in]      This               A pointer to the EFI_FIRMWARE_MANAGEMENT_PROTOCOL instance.
  @param[in, out] ImageInfoSize      A pointer to the size, in bytes, of the ImageInfo buffer.
                                     On input, this is the size of the buffer allocated by the caller.
                                     On output, it is the size of the buffer returned by the firmware
                                     if the buffer was large enough, or the size of the buffer needed
                                     to contain the image(s) information if the buffer was too small.
  @param[in, out] ImageInfo          A pointer to the buffer in which firmware places the current image(s)
                                     information. The information is an array of EFI_FIRMWARE_IMAGE_DESCRIPTORs.
  @param[out]     DescriptorVersion  A pointer to the location in which firmware returns the version number
                                     associated with the EFI_FIRMWARE_IMAGE_DESCRIPTOR.
  @param[out]     DescriptorCount    A pointer to the location in which firmware returns the number of
                                     descriptors or firmware images within this device.
  @param[out]     DescriptorSize     A pointer to the location in which firmware returns the size, in bytes,
                                     of an individual EFI_FIRMWARE_IMAGE_DESCRIPTOR.
  @param[out]     PackageVersion     A version number that represents all the firmware images in the device.
                                     The format is vendor specific and new version must have a greater value
                                     than the old version. If PackageVersion is not supported, the value is
                                     0xFFFFFFFF. A value of 0xFFFFFFFE indicates that package version comparison
                                     is to be performed using PackageVersionName. A value of 0xFFFFFFFD indicates
                                     that package version update is in progress.
  @param[out]     PackageVersionName A pointer to a pointer to a null-terminated string representing the
                                     package version name. The buffer is allocated by this function with
                                     AllocatePool(), and it is the caller's responsibility to free it with a call
                                     to FreePool().

  @retval EFI_SUCCESS                The device was successfully updated with the new image.
  @retval EFI_BUFFER_TOO_SMALL       The ImageInfo buffer was too small. The current buffer size
                                     needed to hold the image(s) information is returned in ImageInfoSize.
  @retval EFI_INVALID_PARAMETER      ImageInfoSize is NULL.
  @retval EFI_DEVICE_ERROR           Valid information could not be returned. Possible corrupted image.

**/
EFI_STATUS
EFIAPI
FmpGetImageInfo (
  IN        EFI_FIRMWARE_MANAGEMENT_PROTOCOL *This,
  IN OUT    UINTN                            *ImageInfoSize,
  IN OUT    EFI_FIRMWARE_IMAGE_DESCRIPTOR    *ImageInfo,
  OUT       UINT32                           *DescriptorVersion,
  OUT       UINT8                            *DescriptorCount,
  OUT       UINTN                            *DescriptorSize,
  OUT       UINT32                           *PackageVersion,
  OUT       CHAR16                          **PackageVersionName
  )
{
  SYSTEM_FMP_PRIVATE_DATA          *SystemFmpPrivate  = NULL;
  SYSTEM_FIRMWARE_IMAGE_DESCRIPTOR *ImageDescriptor   = NULL;

  SystemFmpPrivate = SYSTEM_FMP_PRIVATE_DATA_FROM_FMP(This);

  if(ImageInfoSize == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (*ImageInfoSize < sizeof(EFI_FIRMWARE_IMAGE_DESCRIPTOR) * SystemFmpPrivate->DescriptorCount) {
    *ImageInfoSize = sizeof(EFI_FIRMWARE_IMAGE_DESCRIPTOR) * SystemFmpPrivate->DescriptorCount;
    return EFI_BUFFER_TOO_SMALL;
  }

  if (ImageInfo == NULL ||
      DescriptorVersion == NULL ||
      DescriptorCount == NULL ||
      DescriptorSize == NULL ||
      PackageVersion == NULL ||
      PackageVersionName == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  *ImageInfoSize      = sizeof(EFI_FIRMWARE_IMAGE_DESCRIPTOR) * SystemFmpPrivate->DescriptorCount;
  *DescriptorSize     = sizeof(EFI_FIRMWARE_IMAGE_DESCRIPTOR);
  *DescriptorCount    = SystemFmpPrivate->DescriptorCount;
  *DescriptorVersion  = EFI_FIRMWARE_IMAGE_DESCRIPTOR_VERSION;

  //
  // supports 1 ImageInfo descriptor
  //
  ImageDescriptor = SystemFmpPrivate->ImageDescriptor;
  ImageInfo->ImageIndex = ImageDescriptor->ImageIndex;
  CopyGuid (&ImageInfo->ImageTypeId, &ImageDescriptor->ImageTypeId);
  ImageInfo->ImageId = ImageDescriptor->ImageId;
  if (ImageDescriptor->ImageIdNameStringOffset != 0) {
    ImageInfo->ImageIdName = (CHAR16 *)((UINTN)ImageDescriptor + ImageDescriptor->ImageIdNameStringOffset);
  } else {
    ImageInfo->ImageIdName = NULL;
  }
  ImageInfo->Version = ImageDescriptor->Version;
  if (ImageDescriptor->VersionNameStringOffset != 0) {
    ImageInfo->VersionName = (CHAR16 *)((UINTN)ImageDescriptor + ImageDescriptor->VersionNameStringOffset);
  } else {
    ImageInfo->VersionName = NULL;
  }
  ImageInfo->Size = (UINTN)ImageDescriptor->Size;
  ImageInfo->AttributesSupported = ImageDescriptor->AttributesSupported;
  ImageInfo->AttributesSetting = ImageDescriptor->AttributesSetting;
  ImageInfo->Compatibilities = ImageDescriptor->Compatibilities;
  ImageInfo->LowestSupportedImageVersion = ImageDescriptor->LowestSupportedImageVersion;
  ImageInfo->LastAttemptVersion = ImageDescriptor->LastAttemptVersion;
  ImageInfo->LastAttemptStatus = ImageDescriptor->LastAttemptStatus;
  ImageInfo->HardwareInstance = ImageDescriptor->HardwareInstance;

  //
  // package version
  //
  *PackageVersion = ImageDescriptor->PackageVersion;
  if (ImageDescriptor->PackageVersionNameStringOffset != 0) {
    *PackageVersionName = (VOID *)((UINTN)ImageDescriptor + ImageDescriptor->PackageVersionNameStringOffset);
    *PackageVersionName = AllocateCopyPool(StrSize(*PackageVersionName), *PackageVersionName);
  } else {
    *PackageVersionName = NULL;
  }

  return EFI_SUCCESS;
}

/**
  Retrieves a copy of the current firmware image of the device.

  This function allows a copy of the current firmware image to be created and saved.
  The saved copy could later been used, for example, in firmware image recovery or rollback.

  @param[in]  This               A pointer to the EFI_FIRMWARE_MANAGEMENT_PROTOCOL instance.
  @param[in]  ImageIndex         A unique number identifying the firmware image(s) within the device.
                                 The number is between 1 and DescriptorCount.
  @param[out] Image              Points to the buffer where the current image is copied to.
  @param[out] ImageSize          On entry, points to the size of the buffer pointed to by Image, in bytes.
                                 On return, points to the length of the image, in bytes.

  @retval EFI_SUCCESS            The device was successfully updated with the new image.
  @retval EFI_BUFFER_TOO_SMALL   The buffer specified by ImageSize is too small to hold the
                                 image. The current buffer size needed to hold the image is returned
                                 in ImageSize.
  @retval EFI_INVALID_PARAMETER  The Image was NULL.
  @retval EFI_NOT_FOUND          The current image is not copied to the buffer.
  @retval EFI_UNSUPPORTED        The operation is not supported.
  @retval EFI_SECURITY_VIOLATIO  The operation could not be performed due to an authentication failure.

**/
EFI_STATUS
EFIAPI
FmpGetImage (
  IN  EFI_FIRMWARE_MANAGEMENT_PROTOCOL  *This,
  IN  UINT8                             ImageIndex,
  IN  OUT  VOID                         *Image,
  IN  OUT  UINTN                        *ImageSize
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Updates the firmware image of the device.

  This function updates the hardware with the new firmware image.
  This function returns EFI_UNSUPPORTED if the firmware image is not updatable.
  If the firmware image is updatable, the function should perform the following minimal validations
  before proceeding to do the firmware image update.
  - Validate the image authentication if image has attribute
    IMAGE_ATTRIBUTE_AUTHENTICATION_REQUIRED.  The function returns
    EFI_SECURITY_VIOLATION if the validation fails.
  - Validate the image is a supported image for this device.  The function returns EFI_ABORTED if
    the image is unsupported.  The function can optionally provide more detailed information on
    why the image is not a supported image.
  - Validate the data from VendorCode if not null.  Image validation must be performed before
    VendorCode data validation.  VendorCode data is ignored or considered invalid if image
    validation failed.  The function returns EFI_ABORTED if the data is invalid.

  VendorCode enables vendor to implement vendor-specific firmware image update policy.  Null if
  the caller did not specify the policy or use the default policy.  As an example, vendor can implement
  a policy to allow an option to force a firmware image update when the abort reason is due to the new
  firmware image version is older than the current firmware image version or bad image checksum.
  Sensitive operations such as those wiping the entire firmware image and render the device to be
  non-functional should be encoded in the image itself rather than passed with the VendorCode.
  AbortReason enables vendor to have the option to provide a more detailed description of the abort
  reason to the caller.

  @param[in]  This               A pointer to the EFI_FIRMWARE_MANAGEMENT_PROTOCOL instance.
  @param[in]  ImageIndex         A unique number identifying the firmware image(s) within the device.
                                 The number is between 1 and DescriptorCount.
  @param[in]  Image              Points to the new image.
  @param[in]  ImageSize          Size of the new image in bytes.
  @param[in]  VendorCode         This enables vendor to implement vendor-specific firmware image update policy.
                                 Null indicates the caller did not specify the policy or use the default policy.
  @param[in]  Progress           A function used by the driver to report the progress of the firmware update.
  @param[out] AbortReason        A pointer to a pointer to a null-terminated string providing more
                                 details for the aborted operation. The buffer is allocated by this function
                                 with AllocatePool(), and it is the caller's responsibility to free it with a
                                 call to FreePool().

  @retval EFI_SUCCESS            The device was successfully updated with the new image.
  @retval EFI_ABORTED            The operation is aborted.
  @retval EFI_INVALID_PARAMETER  The Image was NULL.
  @retval EFI_UNSUPPORTED        The operation is not supported.
  @retval EFI_SECURITY_VIOLATIO  The operation could not be performed due to an authentication failure.

**/
EFI_STATUS
EFIAPI
FmpSetImage (
  IN  EFI_FIRMWARE_MANAGEMENT_PROTOCOL                *This,
  IN  UINT8                                            ImageIndex,
  IN  CONST VOID                                      *Image,
  IN  UINTN                                            ImageSize,
  IN  CONST VOID                                      *VendorCode,
  IN  EFI_FIRMWARE_MANAGEMENT_UPDATE_IMAGE_PROGRESS    Progress,
  OUT CHAR16                                         **AbortReason
  )
{
  SYSTEM_FMP_PRIVATE_DATA    *SystemFmpPrivate = NULL;
  EFI_STATUS                  Status           = EFI_SUCCESS;
  EFI_STATUS                  StatusExitPath   = EFI_SUCCESS;
  EFI_SYSTEM_RESOURCE_ENTRY   ent;
  QCFW_UPDATE_FLAG_VALUE      QcResultFlag     = QCFW_UPDATE_INIT;
  UINT32                      LowestSupportedVersion = 0;

  if (This == NULL ||Image == NULL || ImageSize == 0 || AbortReason == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  SystemFmpPrivate = SYSTEM_FMP_PRIVATE_DATA_FROM_FMP (This);

  SystemFmpPrivate->ImageDescriptor->LastAttemptVersion = 0;
  SystemFmpPrivate->ImageDescriptor->LastAttemptStatus = LAST_ATTEMPT_STATUS_ERROR_UNSUCCESSFUL;

  if(ImageIndex != 1) {
    Status = EFI_ABORTED;
    goto FmpSetImageExit;
  }

  //Get Block I/O and File Handles if the handle for EFI partition is found empty
  Status = UpdateHandleRegistry();
  if (Status != EFI_SUCCESS) {
    DEBUG ((DEBUG_ERROR, "Partition Handle registering Failed\n"));
    goto FmpSetImageExit;
  }

  Status = SystemFirmwareAuthenticatedUpdate(
    (VOID *)Image,
    ImageSize,
    &SystemFmpPrivate->ImageDescriptor->LastAttemptVersion,
    &SystemFmpPrivate->ImageDescriptor->LastAttemptStatus,
    &LowestSupportedVersion,
    &QcResultFlag
    );
  if (Status != EFI_SUCCESS) {
    DEBUG ((DEBUG_WARN, "Image authentication and/or update Failed\n"));
    goto FmpSetImageExit;
  }

  DEBUG((DEBUG_INFO, "SetImage - LastAttempt Version - 0x%x, Status - 0x%x\n",
    SystemFmpPrivate->ImageDescriptor->LastAttemptVersion,
    SystemFmpPrivate->ImageDescriptor->LastAttemptStatus));

  //Progress reporting not supported
  if (Progress != NULL) {
    Progress(0);
  }

FmpSetImageExit:

  QcomGetEsrtEntryCache (&ent);

  if (SystemFmpPrivate->ImageDescriptor->LastAttemptStatus == LAST_ATTEMPT_STATUS_SUCCESS)
  {
    //Update the lowest supported version only if it's greater than what is currently set
    if(LowestSupportedVersion > ent.LowestSupportedFwVersion)
    {
      ent.LowestSupportedFwVersion = LowestSupportedVersion;
    }
    ent.FwVersion = SystemFmpPrivate->ImageDescriptor->LastAttemptVersion;
  }
  ent.LastAttemptVersion = SystemFmpPrivate->ImageDescriptor->LastAttemptVersion;
  ent.LastAttemptStatus = SystemFmpPrivate->ImageDescriptor->LastAttemptStatus;

  StatusExitPath = RecordFwUpdateStatus(
    &ent,
    QcResultFlag
    );
  HANDLE_ERR_CONTINUE(StatusExitPath,RecordFwUpdateStatus);

  return (EFI_ERROR (Status) ? Status : StatusExitPath);
}

/**
  Checks if the firmware image is valid for the device.

  This function allows firmware update application to validate the firmware image without
  invoking the SetImage() first.

  @param[in]  This               A pointer to the EFI_FIRMWARE_MANAGEMENT_PROTOCOL instance.
  @param[in]  ImageIndex         A unique number identifying the firmware image(s) within the device.
                                 The number is between 1 and DescriptorCount.
  @param[in]  Image              Points to the new image.
  @param[in]  ImageSize          Size of the new image in bytes.
  @param[out] ImageUpdatable     Indicates if the new image is valid for update. It also provides,
                                 if available, additional information if the image is invalid.

  @retval EFI_SUCCESS            The image was successfully checked.
  @retval EFI_INVALID_PARAMETER  The Image was NULL.
  @retval EFI_UNSUPPORTED        The operation is not supported.
  @retval EFI_SECURITY_VIOLATIO  The operation could not be performed due to an authentication failure.

**/
EFI_STATUS
EFIAPI
FmpCheckImage (
  IN  EFI_FIRMWARE_MANAGEMENT_PROTOCOL  *This,
  IN  UINT8                             ImageIndex,
  IN  CONST VOID                        *Image,
  IN  UINTN                             ImageSize,
  OUT UINT32                            *ImageUpdatable
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Returns information about the firmware package.

  This function returns package information.

  @param[in]  This                     A pointer to the EFI_FIRMWARE_MANAGEMENT_PROTOCOL instance.
  @param[out] PackageVersion           A version number that represents all the firmware images in the device.
                                       The format is vendor specific and new version must have a greater value
                                       than the old version. If PackageVersion is not supported, the value is
                                       0xFFFFFFFF. A value of 0xFFFFFFFE indicates that package version
                                       comparison is to be performed using PackageVersionName. A value of
                                       0xFFFFFFFD indicates that package version update is in progress.
  @param[out] PackageVersionName       A pointer to a pointer to a null-terminated string representing
                                       the package version name. The buffer is allocated by this function with
                                       AllocatePool(), and it is the caller's responsibility to free it with a
                                       call to FreePool().
  @param[out] PackageVersionNameMaxLen The maximum length of package version name if device supports update of
                                       package version name. A value of 0 indicates the device does not support
                                       update of package version name. Length is the number of Unicode characters,
                                       including the terminating null character.
  @param[out] AttributesSupported      Package attributes that are supported by this device. See 'Package Attribute
                                       Definitions' for possible returned values of this parameter. A value of 1
                                       indicates the attribute is supported and the current setting value is
                                       indicated in AttributesSetting. A value of 0 indicates the attribute is not
                                       supported and the current setting value in AttributesSetting is meaningless.
  @param[out] AttributesSetting        Package attributes. See 'Package Attribute Definitions' for possible returned
                                       values of this parameter

  @retval EFI_SUCCESS                  The package information was successfully returned.
  @retval EFI_UNSUPPORTED              The operation is not supported.

**/
EFI_STATUS
EFIAPI
FmpGetPackageInfo (
  IN  EFI_FIRMWARE_MANAGEMENT_PROTOCOL *This,
  OUT UINT32                           *PackageVersion,
  OUT CHAR16                           **PackageVersionName,
  OUT UINT32                           *PackageVersionNameMaxLen,
  OUT UINT64                           *AttributesSupported,
  OUT UINT64                           *AttributesSetting
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Updates information about the firmware package.

  This function updates package information.
  This function returns EFI_UNSUPPORTED if the package information is not updatable.
  VendorCode enables vendor to implement vendor-specific package information update policy.
  Null if the caller did not specify this policy or use the default policy.

  @param[in]  This               A pointer to the EFI_FIRMWARE_MANAGEMENT_PROTOCOL instance.
  @param[in]  Image              Points to the authentication image.
                                 Null if authentication is not required.
  @param[in]  ImageSize          Size of the authentication image in bytes.
                                 0 if authentication is not required.
  @param[in]  VendorCode         This enables vendor to implement vendor-specific firmware
                                 image update policy.
                                 Null indicates the caller did not specify this policy or use
                                 the default policy.
  @param[in]  PackageVersion     The new package version.
  @param[in]  PackageVersionName A pointer to the new null-terminated Unicode string representing
                                 the package version name.
                                 The string length is equal to or less than the value returned in
                                 PackageVersionNameMaxLen.

  @retval EFI_SUCCESS            The device was successfully updated with the new package
                                 information.
  @retval EFI_INVALID_PARAMETER  The PackageVersionName length is longer than the value
                                 returned in PackageVersionNameMaxLen.
  @retval EFI_UNSUPPORTED        The operation is not supported.
  @retval EFI_SECURITY_VIOLATIO  The operation could not be performed due to an authentication failure.

**/
EFI_STATUS
EFIAPI
FmpSetPackageInfo (
  IN  EFI_FIRMWARE_MANAGEMENT_PROTOCOL   *This,
  IN  CONST VOID                         *Image,
  IN  UINTN                              ImageSize,
  IN  CONST VOID                         *VendorCode,
  IN  UINT32                             PackageVersion,
  IN  CONST CHAR16                       *PackageVersionName
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Initialize SystemFmpDriver private data structure.

  @param[in] SystemFmpPrivate  private data structure to be initialized.

  @return EFI_SUCCESS private data is initialized.
**/
EFI_STATUS
InitializePrivateData (
  IN SYSTEM_FMP_PRIVATE_DATA  *SystemFmpPrivate
  )
{
  EFI_STATUS                        Status = EFI_SUCCESS;
  EFI_GUID                          gSystemFirmwareGuid = QCOM_FW_UPDATE_SYSTEM_FW_UNKNOWN_GUID;
  SYSTEM_FIRMWARE_IMAGE_DESCRIPTOR *ImageDescriptor = NULL;
  EFI_SYSTEM_RESOURCE_ENTRY         ent;

  SystemFmpPrivate->Signature       = FMP_SIGNATURE;

  //
  // Build the FMP protocol for the current controller
  //
  SystemFmpPrivate->FmpProtocol.GetImageInfo        = FmpGetImageInfo;
  SystemFmpPrivate->FmpProtocol.GetImage            = FmpGetImage;
  SystemFmpPrivate->FmpProtocol.SetImage            = FmpSetImage;
  SystemFmpPrivate->FmpProtocol.CheckImage          = FmpCheckImage;
  SystemFmpPrivate->FmpProtocol.GetPackageInfo      = FmpGetPackageInfo;
  SystemFmpPrivate->FmpProtocol.SetPackageInfo      = FmpSetPackageInfo;

  SystemFmpPrivate->Handle          = NULL;

  // Only single image is supported
  SystemFmpPrivate->DescriptorCount = 1;

  SystemFmpPrivate->ImageDescriptor = (SYSTEM_FIRMWARE_IMAGE_DESCRIPTOR *) AllocateZeroPool (sizeof (SYSTEM_FIRMWARE_IMAGE_DESCRIPTOR));

  if (SystemFmpPrivate->ImageDescriptor == NULL) {
    DEBUG ((DEBUG_ERROR, "SystemFMPDxe: there is not enough memory to allocate ImageDescriptor.\n"));
    Status = EFI_OUT_OF_RESOURCES;
    goto Exit;
  }

  ImageDescriptor = SystemFmpPrivate->ImageDescriptor;

  ImageDescriptor->PackageVersion = 0xFFFFFFFF;
  ImageDescriptor->PackageVersionNameStringOffset = 0;
  ImageDescriptor->ImageIndex = 1;
  CopyMem (&ImageDescriptor->ImageTypeId, &gSystemFirmwareGuid, sizeof (EFI_GUID));
  ImageDescriptor->ImageId = 0;
  ImageDescriptor->ImageIdNameStringOffset = 0;
  ImageDescriptor->Version = 0; // Updated below to the correct value with the data from ESRT entry
  ImageDescriptor->VersionNameStringOffset = 0;

  /* Use lasrge non-zero size temporarily. FMP processing not using this
     as we dont know this value as it can vary depending on every update
     payload as the payload contents are highly constomizable to have
     all OR one OR a sub-set of all known payload components */
  ImageDescriptor->Size = 0xFFFFFFFF;

  ImageDescriptor->AttributesSupported = IMAGE_ATTRIBUTE_IMAGE_UPDATABLE |
                                         IMAGE_ATTRIBUTE_RESET_REQUIRED |
                                         IMAGE_ATTRIBUTE_AUTHENTICATION_REQUIRED |
                                         IMAGE_ATTRIBUTE_IN_USE |
                                         IMAGE_ATTRIBUTE_UEFI_IMAGE;

  ImageDescriptor->AttributesSetting = IMAGE_ATTRIBUTE_IMAGE_UPDATABLE |
                                       IMAGE_ATTRIBUTE_RESET_REQUIRED |
                                       IMAGE_ATTRIBUTE_AUTHENTICATION_REQUIRED |
                                       IMAGE_ATTRIBUTE_IN_USE |
                                       IMAGE_ATTRIBUTE_UEFI_IMAGE;
  ImageDescriptor->Compatibilities = 0;

  ImageDescriptor->LowestSupportedImageVersion = 0x0;  // Updated below to the correct value with the data from ESRT entry
  ImageDescriptor->LastAttemptVersion = 0; // Updated below to the correct value with the data from ESRT entry
  ImageDescriptor->LastAttemptStatus = 0; // Updated below to the correct value with the data from ESRT entry

  Status = QcomESRTPhaseMain();
  HANDLE_ERROR_LABEL(Status,QcomESRTPhaseMain,Exit);

  Status = QcomGetEsrtEntryCache(&ent);

  if (EFI_ERROR(Status)) {
    DEBUG((EFI_D_WARN, "SystemFMPDxe: Failed to get system firmware ESRT entry. Status = %r\n", Status));
  }
  else {
    CopyMem (&ImageDescriptor->ImageTypeId, &ent.FwClass, sizeof (EFI_GUID));
    ImageDescriptor->Version = ent.FwVersion;
    ImageDescriptor->LowestSupportedImageVersion = ent.LowestSupportedFwVersion;
    ImageDescriptor->LastAttemptVersion = ent.LastAttemptVersion;
    ImageDescriptor->LastAttemptStatus = ent.LastAttemptStatus;
  }

  DEBUG((DEBUG_INFO, "SystemFMPDxe: GetLastAttemp Version - 0x%x, State - 0x%x\n", ImageDescriptor->LastAttemptVersion, ImageDescriptor->LastAttemptStatus));

  ImageDescriptor->HardwareInstance = 0;

Exit:

  return EFI_SUCCESS;
}

/**
  Entry point of the driver.
  Initializes the state information for the Firmware Management Protocol
  and install the protocol.

  @param  ImageHandle   of the loaded driver
  @param  SystemTable   Pointer to the System Table

  @retval EFI_SUCCESS           Protocol registered
  @retval EFI_OUT_OF_RESOURCES  Cannot allocate protocol data structures
  @retval EFI_DEVICE_ERROR      Hardware problems

**/
EFI_STATUS
EFIAPI
SystemFMPEntryPoint (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS                Status = EFI_SUCCESS;
  SYSTEM_FMP_PRIVATE_DATA  *SystemFmpPrivate = NULL;

  //
  // Allocate the FMP Context structure
  //
  SystemFmpPrivate = AllocateZeroPool (sizeof (SYSTEM_FMP_PRIVATE_DATA));
  if (SystemFmpPrivate == NULL) {
    DEBUG ((DEBUG_ERROR, "SystemFMPDxe: There is not enough memory to allocate.\n"));
    Status = EFI_OUT_OF_RESOURCES;
    goto Exit;
  }

  Status = InitializePrivateData(SystemFmpPrivate);
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "SystemFMPDxe: Failed to initialize driver private data. Status - %r.\n", Status));
    goto Exit;
  }

  Status = gBS->InstallMultipleProtocolInterfaces (
    &SystemFmpPrivate->Handle,
    &gEfiFirmwareManagementProtocolGuid,
    &SystemFmpPrivate->FmpProtocol,
    NULL
    );

Exit:
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "SystemFMPDxe: Start() function failed, Status = %r\n", Status));

    //
    //  Release the context structure upon failure
    //
    if (SystemFmpPrivate != NULL) {
      if (SystemFmpPrivate->ImageDescriptor != NULL) {
        FreePool (SystemFmpPrivate->ImageDescriptor);
        SystemFmpPrivate->ImageDescriptor= NULL;
      }
      FreePool (SystemFmpPrivate);
      SystemFmpPrivate = NULL;
    }
  }

  //
  //  Return the operation status.
  //
  return Status;
}
