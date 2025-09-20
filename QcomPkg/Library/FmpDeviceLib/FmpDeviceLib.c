/** @file
  Provides firmware device specific services to support updates of a firmware
  image stored in a firmware device.

  Copyright (c) 2019 Qualcomm Technologies, Inc. All Rights Reserved.
  Portions Copyright (c) 2016, Microsoft Corporation. All rights reserved.<BR>
  Portions Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:
  1. Redistributions of source code must retain the above copyright notice,
  this list of conditions and the following disclaimer.
  2. Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
  IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
  INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
  OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

**/

#include <PiDxe.h>
#include <Library/FmpDeviceLib.h>
#include <Library/FwUpdateLib/FwEsrt.h>
#include <Library/BaseMemoryLib.h>
#include <Library/FmpAuthenticationLib.h>
#include <Guid/WinCertificate.h>
#include <Protocol/FirmwareManagement.h>
#include <Library/FwCommonLib.h>
#include <Library/FwUpdateLib.h>
#include <Library/EdkiiSystemCapsuleLib.h>
#include <EdkiiSystemFmpCapsule.h>
#include <Library/UefiLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Protocol/VariablePolicy.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>

/*global variables*/

VARIABLE_POLICY_ENTRY PolicyTemplate = {
  .Version            = VARIABLE_POLICY_ENTRY_REVISION,
  .Size               = sizeof(VARIABLE_POLICY_ENTRY),
  .OffsetToName       = sizeof(VARIABLE_POLICY_ENTRY),
  .Namespace          = {0},
  .MinSize            = 0,
  .MaxSize            = MAX_UINT32,
  .AttributesMustHave = 0,
  .AttributesCantHave = 0,
  .LockPolicyType     = NoLock
};
VARIABLE_POLICY_PROTOCOL *vp = NULL;    /* Variable policy protocol */
EFI_EVENT                mFmpDeviceVarLockEvent;   /*Event for locking Fmp Variables */
UINT32                   mVariableListSize = 0;

const static EFI_GUID      gDefaultSysFwClassGuid = QCOM_FW_UPDATE_SYSTEM_FW_UNKNOWN_GUID;
static EFI_GUID            gSysFwClassGuid = QCOM_FW_UPDATE_SYSTEM_FW_UNKNOWN_GUID;
static EFI_GUID           *gSysFwClassGuidPtr = &gSysFwClassGuid;


extern EFI_FIRMWARE_MANAGEMENT_UPDATE_IMAGE_PROGRESS    mProgressFuncSysFmp;

/**
  Provide a function to install the Firmware Management Protocol instance onto a
  device handle when the device is managed by a driver that follows the UEFI
  Driver Model.  If the device is not managed by a driver that follows the UEFI
  Driver Model, then EFI_UNSUPPORTED is returned.

  @param[in] FmpInstaller  Function that installs the Firmware Management
                           Protocol.

  @retval EFI_SUCCESS      The device is managed by a driver that follows the
                           UEFI Driver Model.  FmpInstaller must be called on
                           each Driver Binding Start().
  @retval EFI_UNSUPPORTED  The device is not managed by a driver that follows
                           the UEFI Driver Model.
  @retval other            The Firmware Management Protocol for this firmware
                           device is not installed.  The firmware device is
                           still locked using FmpDeviceLock().

**/
EFI_STATUS
EFIAPI
RegisterFmpInstaller (
  IN FMP_DEVICE_LIB_REGISTER_FMP_INSTALLER  Function
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Returns the size, in bytes, of the firmware image currently stored in the
  firmware device.  This function is used to by the GetImage() and
  GetImageInfo() services of the Firmware Management Protocol.  If the image
  size can not be determined from the firmware device, then 0 must be returned.

  @param[out] Size  Pointer to the size, in bytes, of the firmware image
                    currently stored in the firmware device.

  @retval EFI_SUCCESS            The size of the firmware image currently
                                 stored in the firmware device was returned.
  @retval EFI_INVALID_PARAMETER  Size is NULL.
  @retval EFI_UNSUPPORTED        The firmware device does not support reporting
                                 the size of the currently stored firmware image.
  @retval EFI_DEVICE_ERROR       An error occurred attempting to determine the
                                 size of the firmware image currently stored in
                                 in the firmware device.

**/
EFI_STATUS
EFIAPI
FmpDeviceGetSize (
  OUT UINTN  *Size
  )
{
  if (Size == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  *Size = 0;
  return EFI_SUCCESS;
}

/**
  Returns the GUID value used to fill in the ImageTypeId field of the
  EFI_FIRMWARE_IMAGE_DESCRIPTOR structure that is returned by the GetImageInfo()
  service of the Firmware Management Protocol.  If EFI_UNSUPPORTED is returned,
  then the ImageTypeId field is set to gEfiCallerIdGuid.  If EFI_SUCCESS is
  returned, then ImageTypeId is set to the Guid returned from this function.

  @param[out] Guid  Double pointer to a GUID value that is updated to point to
                    to a GUID value.  The GUID value is not allocated and must
                    not be modified or freed by the caller.

  @retval EFI_SUCCESS      EFI_FIRMWARE_IMAGE_DESCRIPTOR ImageTypeId GUID is set
                           to the returned Guid value.
  @retval EFI_UNSUPPORTED  EFI_FIRMWARE_IMAGE_DESCRIPTOR ImageTypeId GUID is set
                           to gEfiCallerIdGuid.

**/
EFI_STATUS
EFIAPI
FmpDeviceGetImageTypeIdGuidPtr (
  OUT EFI_GUID  **Guid
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  
  /* Get the Firmware Resource GUID for the curent platform. */
  Status = QcomGetFwGuidForPlatform(&gSysFwClassGuid);
  if (EFI_ERROR (Status))
  {
    DEBUG((EFI_D_ERROR,"Getting System firmware GUID for current device failed with error: %r. Using default GUID.\n", Status));
    CopyGuid(&gSysFwClassGuid, &gDefaultSysFwClassGuid);
    Status = EFI_SUCCESS;
  }


  Status = UpdateVarSystemFmpCapsuleImageTypeIdGuid(gSysFwClassGuid);
  if (EFI_ERROR (Status))
  {
    DEBUG((EFI_D_ERROR,"Recording System firmware GUID in NV variable failed with error: %r. Ignoring and proceeding.\n", Status));
    Status = EFI_SUCCESS;
  }

  *Guid = gSysFwClassGuidPtr;

  return Status;
}

/**
  Returns values used to fill in the AttributesSupported and AttributesSettings
  fields of the EFI_FIRMWARE_IMAGE_DESCRIPTOR structure that is returned by the
  GetImageInfo() service of the Firmware Management Protocol.  The following
  bit values from the Firmware Management Protocol may be combined:
    IMAGE_ATTRIBUTE_IMAGE_UPDATABLE
    IMAGE_ATTRIBUTE_RESET_REQUIRED
    IMAGE_ATTRIBUTE_AUTHENTICATION_REQUIRED
    IMAGE_ATTRIBUTE_IN_USE
    IMAGE_ATTRIBUTE_UEFI_IMAGE

  @param[out] Supported  Attributes supported by this firmware device.
  @param[out] Setting    Attributes settings for this firmware device.

  @retval EFI_SUCCESS            The attributes supported by the firmware
                                 device were returned.
  @retval EFI_INVALID_PARAMETER  Supported is NULL.
  @retval EFI_INVALID_PARAMETER  Setting is NULL.

**/
EFI_STATUS
EFIAPI
FmpDeviceGetAttributes (
  OUT UINT64    *Supported,
  OUT UINT64    *Setting
  )
{
  if (Supported == NULL || Setting == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  *Supported = IMAGE_ATTRIBUTE_IMAGE_UPDATABLE |
               IMAGE_ATTRIBUTE_RESET_REQUIRED |
               IMAGE_ATTRIBUTE_AUTHENTICATION_REQUIRED |
               IMAGE_ATTRIBUTE_IN_USE |
               IMAGE_ATTRIBUTE_UEFI_IMAGE;

  *Setting = IMAGE_ATTRIBUTE_IMAGE_UPDATABLE |
             IMAGE_ATTRIBUTE_RESET_REQUIRED |
             IMAGE_ATTRIBUTE_AUTHENTICATION_REQUIRED |
             IMAGE_ATTRIBUTE_IN_USE |
             IMAGE_ATTRIBUTE_UEFI_IMAGE;

  return EFI_SUCCESS;
}

/**
  Returns the value used to fill in the LowestSupportedVersion field of the
  EFI_FIRMWARE_IMAGE_DESCRIPTOR structure that is returned by the GetImageInfo()
  service of the Firmware Management Protocol.  If EFI_SUCCESS is returned, then
  the firmware device supports a method to report the LowestSupportedVersion
  value from the currently stored firmware image.  If the value can not be
  reported for the firmware image currently stored in the firmware device, then
  EFI_UNSUPPORTED must be returned.  EFI_DEVICE_ERROR is returned if an error
  occurs attempting to retrieve the LowestSupportedVersion value for the
  currently stored firmware image.

  @note It is recommended that all firmware devices support a method to report
        the LowestSupportedVersion value from the currently stored firmware
        image.

  @param[out] LowestSupportedVersion  LowestSupportedVersion value retrieved
                                      from the currently stored firmware image.

  @retval EFI_SUCCESS       The lowest supported version of currently stored
                            firmware image was returned in LowestSupportedVersion.
  @retval EFI_UNSUPPORTED   The firmware device does not support a method to
                            report the lowest supported version of the currently
                            stored firmware image.
  @retval EFI_DEVICE_ERROR  An error occurred attempting to retrieve the lowest
                            supported version of the currently stored firmware
                            image.

**/
EFI_STATUS
EFIAPI
FmpDeviceGetLowestSupportedVersion (
  OUT UINT32  *LowestSupportedVersion
  )
{
  EFI_STATUS    Status  = EFI_SUCCESS;
  
  if (NULL == LowestSupportedVersion)
  {
    Status = EFI_DEVICE_ERROR;
    DEBUG((EFI_D_ERROR,"Invalid input parameter received in FmpDeviceGetLowestSupportedVersion. Returning %r\n", Status));
    goto Exit;
  }

  // Get version from partition
  Status = QcomGetSysFwVersion(NULL, LowestSupportedVersion);
  
  if (EFI_ERROR(Status)) {
    DEBUG((EFI_D_WARN,"Lowest Supported Version query failed with error: %r. Attempting to update partition with default value...\n", Status));

    // Try to create content with default value with versions as 1
    Status = QcomSetSysFwVersion(
      DEFAULT_SYSTEM_FW_VERSION,
      DEFAULT_SYSTEM_FW_LOWEST_SUPPORTED_VERSION
      );
    if (EFI_ERROR(Status)) {
      DEBUG((EFI_D_ERROR,"Update of version partition with default values failed with error %r. Returning %r\n", Status, EFI_DEVICE_ERROR));
      Status = EFI_DEVICE_ERROR;
      goto Exit;
    }

    *LowestSupportedVersion = DEFAULT_SYSTEM_FW_VERSION;
  }

Exit:
  return Status;
}

/**
  Returns the Null-terminated Unicode string that is used to fill in the
  VersionName field of the EFI_FIRMWARE_IMAGE_DESCRIPTOR structure that is
  returned by the GetImageInfo() service of the Firmware Management Protocol.
  The returned string must be allocated using EFI_BOOT_SERVICES.AllocatePool().

  @note It is recommended that all firmware devices support a method to report
        the VersionName string from the currently stored firmware image.

  @param[out] VersionString  The version string retrieved from the currently
                             stored firmware image.

  @retval EFI_SUCCESS            The version string of currently stored
                                 firmware image was returned in Version.
  @retval EFI_INVALID_PARAMETER  VersionString is NULL.
  @retval EFI_UNSUPPORTED        The firmware device does not support a method
                                 to report the version string of the currently
                                 stored firmware image.
  @retval EFI_DEVICE_ERROR       An error occurred attempting to retrieve the
                                 version string of the currently stored
                                 firmware image.
  @retval EFI_OUT_OF_RESOURCES   There are not enough resources to allocate the
                                 buffer for the version string of the currently
                                 stored firmware image.

**/
EFI_STATUS
EFIAPI
FmpDeviceGetVersionString (
  OUT CHAR16  **VersionString
  )
{
  if (VersionString == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  *VersionString = NULL;
  return EFI_UNSUPPORTED;
}

/**
  Returns the value used to fill in the Version field of the
  EFI_FIRMWARE_IMAGE_DESCRIPTOR structure that is returned by the GetImageInfo()
  service of the Firmware Management Protocol.  If EFI_SUCCESS is returned, then
  the firmware device supports a method to report the Version value from the
  currently stored firmware image.  If the value can not be reported for the
  firmware image currently stored in the firmware device, then EFI_UNSUPPORTED
  must be returned.  EFI_DEVICE_ERROR is returned if an error occurs attempting
  to retrieve the LowestSupportedVersion value for the currently stored firmware
  image.

  @note It is recommended that all firmware devices support a method to report
        the Version value from the currently stored firmware image.

  @param[out] Version  The version value retrieved from the currently stored
                       firmware image.

  @retval EFI_SUCCESS       The version of currently stored firmware image was
                            returned in Version.
  @retval EFI_UNSUPPORTED   The firmware device does not support a method to
                            report the version of the currently stored firmware
                            image.
  @retval EFI_DEVICE_ERROR  An error occurred attempting to retrieve the version
                            of the currently stored firmware image.

**/
EFI_STATUS
EFIAPI
FmpDeviceGetVersion (
  OUT UINT32  *Version
  )
{

  EFI_STATUS                  Status  = EFI_SUCCESS;

  if (NULL == Version)
  {
    Status = EFI_DEVICE_ERROR;
    DEBUG((EFI_D_ERROR,"Invalid input parameter received in FmpDeviceGetVersion. Returning %r\n", Status));
    goto Exit;
  }

  // Get version from partition
  Status = QcomGetSysFwVersion(Version, NULL);
  
  if (EFI_ERROR(Status)) {
    DEBUG((EFI_D_WARN,"Firmware Version query failed with error: %r. Attempting to update partition with default value...\n", Status));

    // Try to create content with default value with versions as 1
    Status = QcomSetSysFwVersion(
      DEFAULT_SYSTEM_FW_VERSION,
      DEFAULT_SYSTEM_FW_LOWEST_SUPPORTED_VERSION
      );
    if (EFI_ERROR(Status)) {
      DEBUG((EFI_D_ERROR,"Update of version partition with default values failed with error %r. Returning %r\n", Status, EFI_DEVICE_ERROR));
      Status = EFI_DEVICE_ERROR;
      goto Exit;
    }

    *Version = DEFAULT_SYSTEM_FW_VERSION;
  }
  
Exit:
  return Status;
}

/**
  Returns a copy of the firmware image currently stored in the firmware device.

  @note It is recommended that all firmware devices support a method to retrieve
        a copy currently stored firmware image.  This can be used to support
        features such as recovery and rollback.

  @param[out]     Image     Pointer to a caller allocated buffer where the
                            currently stored firmware image is copied to.
  @param[in, out] ImageSize Pointer the size, in bytes, of the Image buffer.
                            On return, points to the size, in bytes, of firmware
                            image currently stored in the firmware device.

  @retval EFI_SUCCESS            Image contains a copy of the firmware image
                                 currently stored in the firmware device, and
                                 ImageSize contains the size, in bytes, of the
                                 firmware image currently stored in the
                                 firmware device.
  @retval EFI_BUFFER_TOO_SMALL   The buffer specified by ImageSize is too small
                                 to hold the firmware image currently stored in
                                 the firmware device. The buffer size required
                                 is returned in ImageSize.
  @retval EFI_INVALID_PARAMETER  The Image is NULL.
  @retval EFI_INVALID_PARAMETER  The ImageSize is NULL.
  @retval EFI_UNSUPPORTED        The operation is not supported.
  @retval EFI_DEVICE_ERROR       An error occurred attempting to retrieve the
                                 firmware image currently stored in the firmware
                                 device.

**/
EFI_STATUS
EFIAPI
FmpDeviceGetImage (
  OUT    VOID   *Image,
  IN OUT UINTN  *ImageSize
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Checks if a new firmware image is valid for the firmware device.  This
  function allows firmware update operation to validate the firmware image
  before FmpDeviceSetImage() is called.

  @param[in]  Image           Points to a new firmware image.
  @param[in]  ImageSize       Size, in bytes, of a new firmware image.
  @param[out] ImageUpdatable  Indicates if a new firmware image is valid for
                              a firmware update to the firmware device.  The
                              following values from the Firmware Management
                              Protocol are supported:
                                IMAGE_UPDATABLE_VALID
                                IMAGE_UPDATABLE_INVALID
                                IMAGE_UPDATABLE_INVALID_TYPE
                                IMAGE_UPDATABLE_INVALID_OLD
                                IMAGE_UPDATABLE_VALID_WITH_VENDOR_CODE

  @retval EFI_SUCCESS            The image was successfully checked.  Additional
                                 status information is returned in
                                 ImageUpdatable.
  @retval EFI_INVALID_PARAMETER  Image is NULL.
  @retval EFI_INVALID_PARAMETER  ImageUpdatable is NULL.

**/
EFI_STATUS
EFIAPI
FmpDeviceCheckImage (
  IN  CONST VOID  *Image,
  IN  UINTN       ImageSize,
  OUT UINT32      *ImageUpdatable
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  UINT32                    LastAttemptStatus           = LAST_ATTEMPT_STATUS_ERROR_UNSUCCESSFUL;
  QCFW_UPDATE_FLAG_VALUE    QcResultFlag                = QCFW_UPDATE_INIT;
  QPAYLOAD_METADATA_HEADER_V3 PayloadHeaderV3           = {0};
  
  if (ImageUpdatable == NULL)
  {
      DEBUG((DEBUG_ERROR, "CheckImage - ImageUpdateable Pointer Parameter is NULL.\n"));
      Status = EFI_INVALID_PARAMETER;
      goto Exit;
  }
  
  //
  //Set to valid and then if any tests fail it will update this flag.
  //
  *ImageUpdatable = IMAGE_UPDATABLE_VALID;
  
  if (Image == NULL)
  {
    *ImageUpdatable = IMAGE_UPDATABLE_INVALID;
    DEBUG((DEBUG_ERROR, "CheckImage - Image Pointer Parameter is NULL.\n"));
    return EFI_INVALID_PARAMETER;
  }

  /* Try getting meta data here as getting this data as this will be an indirect
  check whether the payload contain the mandatory metadata section */
  Status = ExtractSystemFirmwareMetaDataHeader(
    (void *)Image,
    ImageSize,
    &PayloadHeaderV3,
    &LastAttemptStatus,
    &QcResultFlag
    );
  if (EFI_ERROR(Status)) {
    *ImageUpdatable = IMAGE_UPDATABLE_INVALID_TYPE;
    DEBUG((DEBUG_WARN, "ExtractSystemFirmwareMetaDataHeader failed with error: %r\n", Status));
    goto Exit;
  }


Exit:
  return Status;
}

/**
  Updates a firmware device with a new firmware image.  This function returns
  EFI_UNSUPPORTED if the firmware image is not updatable.  If the firmware image
  is updatable, the function should perform the following minimal validations
  before proceeding to do the firmware image update.
    - Validate that the image is a supported image for this firmware device.
      Return EFI_ABORTED if the image is not supported.  Additional details
      on why the image is not a supported image may be returned in AbortReason.
    - Validate the data from VendorCode if is not NULL.  Firmware image
      validation must be performed before VendorCode data validation.
      VendorCode data is ignored or considered invalid if image validation
      fails.  Return EFI_ABORTED if the VendorCode data is invalid.

  VendorCode enables vendor to implement vendor-specific firmware image update
  policy.  Null if the caller did not specify the policy or use the default
  policy.  As an example, vendor can implement a policy to allow an option to
  force a firmware image update when the abort reason is due to the new firmware
  image version is older than the current firmware image version or bad image
  checksum.  Sensitive operations such as those wiping the entire firmware image
  and render the device to be non-functional should be encoded in the image
  itself rather than passed with the VendorCode.  AbortReason enables vendor to
  have the option to provide a more detailed description of the abort reason to
  the caller.

  @param[in]  Image             Points to the new firmware image.
  @param[in]  ImageSize         Size, in bytes, of the new firmware image.
  @param[in]  VendorCode        This enables vendor to implement vendor-specific
                                firmware image update policy.  NULL indicates
                                the caller did not specify the policy or use the
                                default policy.
  @param[in]  Progress          A function used to report the progress of
                                updating the firmware device with the new
                                firmware image.
  @param[in]  CapsuleFwVersion  The version of the new firmware image from the
                                update capsule that provided the new firmware
                                image.
  @param[out] AbortReason       A pointer to a pointer to a Null-terminated
                                Unicode string providing more details on an
                                aborted operation. The buffer is allocated by
                                this function with
                                EFI_BOOT_SERVICES.AllocatePool().  It is the
                                caller's responsibility to free this buffer with
                                EFI_BOOT_SERVICES.FreePool().

  @retval EFI_SUCCESS            The firmware device was successfully updated
                                 with the new firmware image.
  @retval EFI_ABORTED            The operation is aborted.  Additional details
                                 are provided in AbortReason.
  @retval EFI_INVALID_PARAMETER  The Image was NULL.
  @retval EFI_UNSUPPORTED        The operation is not supported.

**/
EFI_STATUS
EFIAPI
FmpDeviceSetImage (
  IN  CONST VOID                                     *Image,
  IN  UINTN                                          ImageSize,
  IN  CONST VOID                                     *VendorCode,       OPTIONAL
  IN  EFI_FIRMWARE_MANAGEMENT_UPDATE_IMAGE_PROGRESS  Progress,          OPTIONAL
  IN  UINT32                                         CapsuleFwVersion,
  OUT CHAR16                                         **AbortReason
  )
{

  EFI_STATUS                Status                      = EFI_SUCCESS;
  UINT32					NewLowestSupportedVersion   = 0;
  UINT32                    NewVersion                  = 0;
  UINT32					LowestSupportedVersion      = 0;
  UINT32                    Version                     = 0;
  UINT32                    LastAttemptStatus           = LAST_ATTEMPT_STATUS_ERROR_UNSUCCESSFUL;
  QCFW_UPDATE_FLAG_VALUE	QcResultFlag	            = QCFW_UPDATE_INIT;
  EFI_STATUS                StatusExitPath              = EFI_SUCCESS;
  QPAYLOAD_METADATA_HEADER_V3 PayloadHeaderV3           = {0};

  // Initialize the Progress function , if defined.
  if (Progress != NULL) {
    mProgressFuncSysFmp = Progress;
  }

  /* Getting meta data here as getting this data as early in the update
  process will enable recording the last attempted version value in ESRT
  and capsule report variables */
  Status = ExtractSystemFirmwareMetaDataHeader((void *)Image, ImageSize, &PayloadHeaderV3, &LastAttemptStatus, &QcResultFlag);
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_WARN, "ExtractSystemFirmwareMetaDataHeader failed with error: %r\n", Status));
    goto Exit;
  }

  NewVersion                = PayloadHeaderV3.FirmwareVersion;
  NewLowestSupportedVersion = PayloadHeaderV3.LowestSupportedVersion;

  DEBUG((DEBUG_WARN, "    NewImage Version                                  - 0x%x\n", NewVersion));
  DEBUG((DEBUG_WARN, "    NewImage LowestSupportedImageVersion              - 0x%x\n", NewLowestSupportedVersion));

  Status = QcomGetSysFwVersion(&Version, &LowestSupportedVersion);
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "QcomGetSysFwVersion failed with error: %r. Trying to ignore and proceed...\n", Status));
    Status = EFI_SUCCESS;
  } else {
    DEBUG((DEBUG_WARN, "    Current Version (partition)                       - 0x%x\n", Version));
    DEBUG((DEBUG_WARN, "    Current LowestSupportedImageVersion (partition)   - 0x%x\n", LowestSupportedVersion));
  }

  DEBUG ((DEBUG_WARN, "Attempting to start: Firmware update\n"));

  Status = RunFwUpdateMain(
              Image,
              ImageSize,
              &LastAttemptStatus,
              &QcResultFlag,
              Progress
              );
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "RunFwUpdateMain failed with error: %r\n", Status));
    goto Exit;
  }

Exit:
  //All test hook data should be cleared as
  //we do not want device use this in future capsule update
  ResetHookEntryToDefault();
  
  if (EFI_ERROR(Status)) {
    if (LastAttemptStatus == LAST_ATTEMPT_STATUS_SUCCESS) {
        LastAttemptStatus = LAST_ATTEMPT_STATUS_ERROR_UNSUCCESSFUL;
    }
    DEBUG ((DEBUG_ERROR, "Firmware update failed with error: %r. Last attempt status: 0x%x\n", Status, LastAttemptStatus));
  } else {
    DEBUG ((DEBUG_WARN, "Firmware update completed successfully\n"));
  }

  if (LastAttemptStatus == LAST_ATTEMPT_STATUS_SUCCESS)
  {
    //Update the lowest supported version only if it's greater than what is currently set
    if(NewLowestSupportedVersion > LowestSupportedVersion)
    {
      LowestSupportedVersion = NewLowestSupportedVersion;
    }
    Version = NewVersion;
  }

  DEBUG ((DEBUG_WARN, "Attempting to update SysFw Partition Entry\n"));

  StatusExitPath = RecordFwUpdateStatus(
    Version,
    LowestSupportedVersion,
    LastAttemptStatus,
    QcResultFlag
    );
  HANDLE_ERR_CONTINUE(StatusExitPath,RecordFwUpdateStatus);

  DEBUG ((DEBUG_WARN, "Updated SysFw Partition Entry\n"));

  if (Progress != NULL) {
    Progress(PROGRESS_CAPSULE_COMPLEATED);
  }

  return (EFI_ERROR (Status) ? Status : StatusExitPath);;
}

/**
  Lock the firmware device that contains a firmware image.  Once a firmware
  device is locked, any attempts to modify the firmware image contents in the
  firmware device must fail.

  @note It is recommended that all firmware devices support a lock method to
        prevent modifications to a stored firmware image.

  @note A firmware device lock mechanism is typically only cleared by a full
        system reset (not just sleep state/low power mode).

  @retval  EFI_SUCCESS      The firmware device was locked.
  @retval  EFI_UNSUPPORTED  The firmware device does not support locking

**/
EFI_STATUS
EFIAPI
FmpDeviceLock (
  VOID
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Prepare for the subsequent calls from FmpDxe by keeping the necessary contents
  initialized during start of driver

  @retval  EFI_SUCCESS      The firmware device was successfully initialized.

**/
EFI_STATUS
EFIAPI
FmpDeviceInit (
  VOID
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  UINT32     Version = 0;
    
  Status = CheckAndCreateNVData() ;
  HANDLE_ERROR_LABEL(Status,CheckAndCreateNVData,Exit);

  Status = QcomInitSysFwVersionContext();
  HANDLE_ERROR_LABEL(Status,QcomInitSysFwVersionContext,Exit);

  // Just query the version so that 1) internal version cache gets updated
  // for future usage 2) To make sure that partition content is valid before
  // its printed below during every boot.
  Status = FmpDeviceGetVersion (&Version);
  if (EFI_ERROR(Status)) {
    DEBUG((EFI_D_WARN,"Attempt to get SysFwVer partition content failed with error: %r. Ignoring and continuing...\n", Status));
    // Clear out the error value if any as we dont consider this as fatal
    // as FmpDxe has a fall-back mechanism to handle this scenario by
    // using its itnernal NV variable as the version storage container 
    Status = EFI_SUCCESS;
  }

  // Print current SysFwVersion entry info here to record this once in UEFI log
  Status = QcomPrintSysFwVerPartitionData(EFI_D_WARN);
  if (EFI_ERROR(Status)) {
    DEBUG((EFI_D_WARN,"Attempt to print SysFwVer partition content failed with error: %r. Ignoring and continuing...\n", Status));
    Status = EFI_SUCCESS;
  }

Exit:
  return Status;
}

/**
  This routine will lock fmp variable, which will be lock after creation of that variable.

  UEFI Variable accessed: GUID = gEfiCallerIdGuid, Name = L"LastAttemptVersion"

  @param[in] VARIABLE_POLICY_PROTOCOL  variable policy protocol instance

  @param[in] CHAR16 *   name of variable which caller want to lock

**/
EFI_STATUS RequestToLockFmpVariable( VARIABLE_POLICY_PROTOCOL *vp, const CHAR16  *VarName)
{
	EFI_STATUS Status = EFI_SUCCESS;
	UINTN      BuffSz = 0;
	BuffSz = sizeof(VARIABLE_POLICY_ENTRY) + StrSize(VarName);
	VARIABLE_POLICY_ENTRY *PolLockOnCreate = (VARIABLE_POLICY_ENTRY *)AllocateZeroPool(BuffSz);
	if (PolLockOnCreate == NULL) {
      DEBUG ((DEBUG_ERROR, " RequestToLockFmpVariable: AllocateZeroPool failed\n"));
	  return EFI_OUT_OF_RESOURCES;
    }
	
	CopyMem(PolLockOnCreate, &PolicyTemplate, sizeof(VARIABLE_POLICY_ENTRY));
	CopyMem((UINT8*)PolLockOnCreate + sizeof(VARIABLE_POLICY_ENTRY), VarName, StrSize(VarName));
	PolLockOnCreate->Size = BuffSz;
	PolLockOnCreate->Namespace = gEfiCallerIdGuid;
	PolLockOnCreate->LockPolicyType = LockNow; // this will lock variable immediately.

	Status = vp->RegisterVariablePolicy(PolLockOnCreate);
	if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "RequestToLockFmpVariable: RegisterVariablePolicy failed\n"));
	  return EFI_NOT_FOUND;
    }
	return Status;
}
/**
  Event notification function that is invoked when the event GUID specified by
  LockGuid is signaled.
  Locks all the UEFI FMP Variables used by this module.

  @retval  EFI_SUCCESS      All UEFI variables are locked.
  @retval  EFI_UNSUPPORTED  Variable Lock Protocol not found.
  @retval  Other            One of the UEFI variables could not be locked.

**/
VOID
EFIAPI
FmpDeviceLockVarEventNotify (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
	EFI_STATUS      Status = EFI_SUCCESS;
	UINTN           Index = 0;
	BOOLEAN         State  = FALSE;
	const CHAR16    **FmpVariableLockOnEventList;

	if (Context == NULL)
	{
	DEBUG ((DEBUG_ERROR, " FmpDeviceLockVarEventNotify: Context not found. No Variable list available to Lock\n"));
	return;
	}
	FmpVariableLockOnEventList =(const CHAR16 **)Context;
  
	Status = gBS->LocateProtocol(
	  &gVariablePolicyProtocolGuid,
	  NULL,
	  (VOID **) &vp);
	DEBUG ((DEBUG_INFO, "[LockAllFmpVariables] gVariablePolicyProtocolGuid =%r\n", Status));
  
	Status = vp->IsVariablePolicyEnabled(&State);
	if (EFI_ERROR (Status) || (State != TRUE)) {
	  DEBUG ((DEBUG_ERROR, "FmpDeviceLockVarEventNotify: IsVariablePolicyEnabled  Status = %r, State=%x \n", Status, State));
	  return;
	}
	
	for (Index = 0; Index < mVariableListSize; Index++) {
	Status = RequestToLockFmpVariable(vp,FmpVariableLockOnEventList[Index]);
	DEBUG ((DEBUG_INFO, "[LockAllFmpVariables] RequestToLockFmpVariables=%r..Varname=%s\n", Status, FmpVariableLockOnEventList[Index]));
	if (EFI_ERROR (Status)) {
	  DEBUG ((DEBUG_ERROR, "FmpDxe: Failed to lock variable  %s.  Status = %r\n",
	    FmpVariableLockOnEventList[Index],
	    Status
	    ));
	}
   }
}
/**
 It will register event on passed guid

  @param[in] LockGuid   Guid of event.
  
**/
EFI_STATUS FmpDeviceVarLockOnEvent(EFI_GUID *LockGuid,const CHAR16  **FmpVariableList, UINT32 VariableListSize) {

	EFI_STATUS Status = EFI_SUCCESS;
	mVariableListSize = VariableListSize;
 
	Status = gBS->CreateEventEx (
	                EVT_NOTIFY_SIGNAL,
	                TPL_CALLBACK,
	                FmpDeviceLockVarEventNotify,
	                (VOID *)FmpVariableList,
	                LockGuid,
	                &mFmpDeviceVarLockEvent
	                );
	if (EFI_ERROR (Status)) {
	  DEBUG ((DEBUG_ERROR, "FmpDeviceVarLockOnEvent: Failed to register notification.  Status = %r\n", Status));
	}

	return Status;
}
