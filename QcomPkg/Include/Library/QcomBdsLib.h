/** @file
  Platform BDS library definition. A platform can implement 
  instances to support platform-specific behavior.

Copyright (c) 2014, 2016 - 2019 Qualcomm Technologies, Inc. All Rights Reserved
Portions Copyright (c) 2008 - 2010, Intel Corporation. All rights reserved.<BR>
Portions Copyright (c) 2013-2015, ARM Limited. All rights reserved.<BR>
This program and the accompanying materials are licensed and made available under 
the terms and conditions of the BSD License that accompanies this distribution.  
The full text of the license may be found at
http://opensource.org/licenses/bsd-license.php.                                          
    
THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,                     
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

/*=============================================================================
                              EDIT HISTORY


 when       who     what, where, why
 --------   ---     -----------------------------------------------------------
 07/09/19   kpa     Rename PlatBdsLaunchOEMSetupApp and add platbds apis
 05/28/19   pa      Added api prototypes from CapsuleUpdate
 03/22/19   kpa     Added api prototypes from PlatformBdsLib
 01/18/19   kpa     Added PlatformPreConsoleInit, PlatformSecurityInit, SetupFwProvision
 12/09/18   kpa     Added QcomBdsDetectBootHotKey
 09/18/17   bd      Send SD Card detect event in case of Boot from removable 
 05/10/16   ai      Move QC changes from BdsLib.h, PlatformBdsLib.h to QcomBdsLib.h 

=============================================================================*/

#ifndef __QCOM_BDS_LIB_H_
#define __QCOM_BDS_LIB_H_

typedef enum {
  OptionNone = 0,
  BootFromRemovableMedia
} BDS_INIT_OPTION; 

typedef UINT8* EFI_QC_LOAD_OPTION;

/**
  This is defined by the UEFI specs, don't change it
**/
typedef struct {
  UINT16                      LoadOptionIndex;
  EFI_QC_LOAD_OPTION             LoadOption;
  UINTN                       LoadOptionSize;

  UINT32                      Attributes;
  UINT16                      FilePathListLength;
  CHAR16                      *Description;
  EFI_DEVICE_PATH_PROTOCOL    *FilePathList;

  VOID*                       OptionalData;
  UINTN                       OptionalDataSize;
} BDS_LOAD_OPTION;

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
  );

/**
  Connect all DXE drivers

  @retval EFI_SUCCESS           All drivers have been connected
  @retval EFI_NOT_FOUND         No handles match the search.
  @retval EFI_OUT_OF_RESOURCES  There is not resource pool memory to store the matching results.

**/
EFI_STATUS
BdsConnectAllDrivers (
  VOID
  );

EFI_STATUS
BootOptionFromLoadOptionIndex (
  IN  UINT16            LoadOptionIndex,
  OUT BDS_LOAD_OPTION** BdsLoadOption
  );

EFI_STATUS
BootOptionFromLoadOptionVariable (
  IN  CHAR16*           BootVariableName,
  OUT BDS_LOAD_OPTION** BdsLoadOption
  );

EFI_STATUS
BootOptionToLoadOptionVariable (
  IN BDS_LOAD_OPTION*   BdsLoadOption
  );

UINT16
BootOptionAllocateBootIndex (
  VOID
  );

/**
  Start a Linux kernel from a Device Path

  @param  LinuxKernel           Device Path to the Linux Kernel
  @param  Parameters            Linux kernel arguments

  @retval EFI_SUCCESS           All drivers have been connected
  @retval EFI_NOT_FOUND         The Linux kernel Device Path has not been found
  @retval EFI_OUT_OF_RESOURCES  There is not enough resource memory to store the matching results.

**/
EFI_STATUS
BdsBootLinuxAtag (
  IN  EFI_DEVICE_PATH_PROTOCOL* LinuxKernelDevicePath,
  IN  EFI_DEVICE_PATH_PROTOCOL* InitrdDevicePath,
  IN  CONST CHAR8*              Arguments
  );

/**
  Start a Linux kernel from a Device Path

  @param[in]  LinuxKernelDevicePath  Device Path to the Linux Kernel
  @param[in]  InitrdDevicePath       Device Path to the Initrd
  @param[in]  Arguments              Linux kernel arguments

  @retval EFI_SUCCESS           All drivers have been connected
  @retval EFI_NOT_FOUND         The Linux kernel Device Path has not been found
  @retval EFI_OUT_OF_RESOURCES  There is not enough resource memory to store the matching results.

**/
EFI_STATUS
BdsBootLinuxFdt (
  IN  EFI_DEVICE_PATH_PROTOCOL* LinuxKernelDevicePath,
  IN  EFI_DEVICE_PATH_PROTOCOL* InitrdDevicePath,
  IN  CONST CHAR8*              Arguments
  );

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
  IN EFI_DEVICE_PATH_PROTOCOL    *DevicePath,
  IN UINTN                       LoadOptionsSize,
  IN VOID*                       LoadOptions
  );

/**
  Start an EFI Application from any Firmware Volume

  @param  EfiApp                EFI Application Name

  @retval EFI_SUCCESS           All drivers have been connected
  @retval EFI_NOT_FOUND         The Linux kernel Device Path has not been found
  @retval EFI_OUT_OF_RESOURCES  There is not enough resource memory to store the matching results.

**/
EFI_STATUS
BdsLoadApplication (
  IN EFI_HANDLE                  ParentImageHandle,
  IN CHAR16*                     EfiApp,
  IN UINTN                       LoadOptionsSize,
  IN VOID*                       LoadOptions
  );

EFI_STATUS
BdsLoadImage (
  IN     EFI_DEVICE_PATH       *DevicePath,
  IN     EFI_ALLOCATE_TYPE     Type,
  IN OUT EFI_PHYSICAL_ADDRESS* Image,
  OUT    UINTN                 *FileSize
  );

/**
 * Call BS.ExitBootServices with the appropriate Memory Map information
 */
EFI_STATUS
ShutdownUefiBootServices (
  VOID
  );

/**
  Platform Bds Api's for Pre-Console Config
**/
EFI_STATUS
EFIAPI
PlatformBdsPreConsoleInit (VOID);

/** 
  Platform Bds init with support for specifying initialization options

**/
VOID
EFIAPI
PlatformBdsPostConsoleInit ( VOID );

/** 
  Platform security initializations

**/
EFI_STATUS
EFIAPI
PlatformSecurityInit ( VOID );

/**
  Capsule Update
**/
EFI_STATUS
EFIAPI
PlatBdsProcessFwCapsules (VOID);


/**
  Setup Firmware Provision
**/
EFI_STATUS
EFIAPI
PlatBdsSocPostProc (VOID);

/**
  Connect consoles and drivers
**/
EFI_STATUS
EFIAPI
PlatBdsConnectCon (VOID);

/**
  Process Hotkeys
**/
EFI_STATUS
EFIAPI
PlatformBdsDetectHotKey (VOID);

/**
  Security Configs after Firmware Provision
**/
EFI_STATUS
EFIAPI
PlatBdsPostFwConfigSec (VOID);

/**
 * Launch default BDS Boot App.
 * If default BDS Boot App is specified then this function should NOT return,
 * on failure to launch or if the launched app returns, launch the shell app
 * if the device is NOT in retail mode as determined by PRODMODE Build and
 * fuse blown status.
*/
EFI_STATUS
EFIAPI
PlatBdsLaunchDefaultApps (VOID);

/**
 * Launch default OEM Setup App.
*/
EFI_STATUS
EFIAPI
PlatBdsProcessOsIndicationsForOemSetupApp (VOID);

/** 
  Get Boot Hotkey Option

**/
EFI_STATUS
EFIAPI
QcomBdsDetectBootHotKey (BDS_INIT_OPTION *InitOption);

/**
  This is called after the BDS exhuasts all options, either 
  through enumeration or attempts to boot
**/
VOID
PlatformBdsBootHalt(
  VOID
  );

/**
  Send SD Card detected event notification  
**/
VOID
SendEventDetectSdCard(
  VOID
  );  

EFI_STATUS
EFIAPI
RegisterEBSCallback
(
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
);
#endif

