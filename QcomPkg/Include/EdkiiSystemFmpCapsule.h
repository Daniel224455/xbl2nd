/** @file
  Qualcomm system firmware update driver stuctures and GUIDs
  
  Copyright (c) 2017 Qualcomm Technologies, Inc.  All Rights Reserved.
  Portions Copyright (c) 2016, Intel Corporation. All rights reserved.<BR>
  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/


#ifndef __EDKII_SYSTEM_FMP_CAPSULE_GUID_H__
#define __EDKII_SYSTEM_FMP_CAPSULE_GUID_H__

#pragma pack(1)
typedef struct {
  // Below structure is similar as UEFI EFI_FIRMWARE_MANAGEMENT_PROTOCOL.GetPackageInfo()
  UINT32                                PackageVersion;
  UINT32                                PackageVersionNameStringOffset; // Offset from head, CHAR16 string including NULL terminate char
  // Below structure is similar as UEFI EFI_FIRMWARE_IMAGE_DESCRIPTOR
  UINT8                                 ImageIndex;
  EFI_GUID                              ImageTypeId;
  UINT64                                ImageId;
  UINT32                                ImageIdNameStringOffset; // Offset from head, CHAR16 string including NULL terminate char
  UINT32                                Version;
  UINT32                                VersionNameStringOffset; // Offset from head, CHAR16 string including NULL terminate char
  UINT64                                Size;
  UINT64                                AttributesSupported;
  UINT64                                AttributesSetting;
  UINT64                                Compatibilities;
  UINT32                                LowestSupportedImageVersion;
  UINT32                                LastAttemptVersion;
  UINT32                                LastAttemptStatus;
  UINT64                                HardwareInstance;
  // real string data
//CHAR16                                ImageIdNameStr[];        // CHAR16 string including NULL terminate char
//CHAR16                                VersionNameStr[];        // CHAR16 string including NULL terminate char
//CHAR16                                PackageVersionNameStr[]; // CHAR16 string including NULL terminate char
} SYSTEM_FIRMWARE_IMAGE_DESCRIPTOR;
#pragma pack()


#endif
