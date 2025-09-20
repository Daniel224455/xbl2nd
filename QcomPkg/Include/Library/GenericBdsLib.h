/** @file
  Generic BDS library defines general interfaces for a BDS driver, including:
    1) BDS boot policy interface.
    2) BDS boot device connect interface.
    3) BDS Misc interfaces for mainting boot variable, ouput string.

Copyright (c) 2019 Qualcomm Technologies, Inc All rights reserved.<BR>
Copyright (c) 2004 - 2013, Intel Corporation. All rights reserved.<BR>
This program and the accompanying materials are licensed and made available under 
the terms and conditions of the BSD License that accompanies this distribution.  
The full text of the license may be found at
http://opensource.org/licenses/bsd-license.php.                                          
    
THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,                     
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _GENERIC_BDS_LIB_H_
#define _GENERIC_BDS_LIB_H_

#include <Protocol/UserManager.h>

typedef struct {

  UINTN                     Signature;
  LIST_ENTRY                Link;

  EFI_DEVICE_PATH_PROTOCOL  *DevicePath;

  CHAR16                    *OptionName;
  UINTN                     OptionNumber;
  UINT16                    BootCurrent;
  UINT32                    Attribute;
  CHAR16                    *Description;
  VOID                      *LoadOptions;
  UINT32                    LoadOptionsSize;
  CHAR16                    *StatusString;

} BDS_COMMON_OPTION;

typedef struct {
  EFI_DEVICE_PATH_PROTOCOL  *DevicePath;
  UINTN                     ConnectType;
} BDS_CONSOLE_CONNECT_ENTRY;


/**
  This function reads the EFI variable (VendorGuid/Name) and returns a dynamically allocated
  buffer and the size of the buffer. If it fails, return NULL.

  @param  Name                  The string part of the  EFI variable name.
  @param  VendorGuid            The GUID part of the EFI variable name.
  @param  VariableSize          Returns the size of the EFI variable that was read.

  @return                       Dynamically allocated memory that contains a copy 
                                of the EFI variable. The caller is responsible for 
                                freeing the buffer.
  @retval NULL                  The variable was not read.

**/
VOID *
EFIAPI
BdsLibGetVariableAndSize (
  IN  CHAR16              *Name,
  IN  EFI_GUID            *VendorGuid,
  OUT UINTN               *VariableSize
  );

//
// Bds connect and disconnect driver lib funcions
//
/**
  This function connects all system drivers with the corresponding controllers. 

**/
VOID
EFIAPI
BdsLibConnectAllDriversToAllControllers (
  VOID
  );

/**
  This function connects all system drivers to controllers.

**/
VOID
EFIAPI
BdsLibConnectAll (
  VOID
  );

/**
  This function will create all handles associate with every device
  path node. If the handle associate with one device path node can not
  be created successfully, then still give chance to do the dispatch,
  which load the missing drivers if possible.

  @param  DevicePathToConnect   The device path to be connected. Can be
                                a multi-instance device path.

  @retval EFI_SUCCESS           All handles associates with every device path node
                                were created.
  @retval EFI_OUT_OF_RESOURCES  Not enough resources to create new handles.
  @retval EFI_NOT_FOUND         At least one handle could not be created.

**/
EFI_STATUS
EFIAPI
BdsLibConnectDevicePath (
  IN EFI_DEVICE_PATH_PROTOCOL  *DevicePathToConnect
  );

/**
  This function will connect all current system handles recursively.     
  gBS->ConnectController() service is invoked for each handle exist in system handler buffer.  
  If the handle is bus type handler, all childrens also will be connected recursively  by gBS->ConnectController().
  
  @retval EFI_SUCCESS           All handles and child handles have been
                                connected.  
  @retval EFI_STATUS            Return the status of gBS->LocateHandleBuffer().
**/
EFI_STATUS
EFIAPI
BdsLibConnectAllEfi (
  VOID
  );

/**
  This function will disconnect all current system handles.     
  gBS->DisconnectController() is invoked for each handle exists in system handle buffer.  
  If handle is a bus type handle, all childrens also are disconnected recursively by  gBS->DisconnectController().
  
  @retval EFI_SUCCESS           All handles have been disconnected.
  @retval EFI_STATUS            Error status returned by of gBS->LocateHandleBuffer().

**/
EFI_STATUS
EFIAPI
BdsLibDisconnectAllEfi (
  VOID
  );

//
// Bds console related lib functions
//
/**
  This function will search every simpletxt device in the current system,
  and make every simpletxt device a potential console device.

**/
VOID
EFIAPI
BdsLibConnectAllConsoles (
  VOID
  );


/**
  This function will connect console device based on the console
  device variable ConIn, ConOut and ErrOut.

  @retval EFI_SUCCESS              At least one of the ConIn and ConOut devices have
                                   been connected.
  @retval EFI_STATUS               Return the status of BdsLibConnectConsoleVariable ().

**/
EFI_STATUS
EFIAPI
BdsLibConnectAllDefaultConsoles (
  VOID
  );


/**
  This function updates the console variable based on ConVarName. It can
  add or remove one specific console device path from the variable

  @param  ConVarName               The console-related variable name: ConIn, ConOut,
                                   ErrOut.
  @param  CustomizedConDevicePath  The console device path to be added to
                                   the console variable ConVarName. Cannot be multi-instance.
  @param  ExclusiveDevicePath      The console device path to be removed
                                   from the console variable ConVarName. Cannot be multi-instance.

  @retval EFI_UNSUPPORTED          The added device path is the same as a removed one.
  @retval EFI_SUCCESS              Successfully added or removed the device path from the
                                   console variable.

**/
EFI_STATUS
EFIAPI
BdsLibUpdateConsoleVariable (
  IN  CHAR16                    *ConVarName,
  IN  EFI_DEVICE_PATH_PROTOCOL  *CustomizedConDevicePath,
  IN  EFI_DEVICE_PATH_PROTOCOL  *ExclusiveDevicePath
  );

/**
  Connect the console device base on the variable ConVarName, if
  device path of the ConVarName is multi-instance device path and
  anyone of the instances is connected success, then this function
  will return success.
  If the handle associate with one device path node can not
  be created successfully, then still give chance to do the dispatch,
  which load the missing drivers if possible.

  @param  ConVarName               Console related variable name, ConIn, ConOut,
                                   ErrOut.

  @retval EFI_NOT_FOUND            There is not any console devices connected
                                   success
  @retval EFI_SUCCESS              Success connect any one instance of the console
                                   device path base on the variable ConVarName.

**/
EFI_STATUS
EFIAPI
BdsLibConnectConsoleVariable (
  IN  CHAR16                 *ConVarName
  );

//
// Bds device path related lib functions
//
/**
  Delete the instance in Multi that overlaps with Single. 

  @param  Multi                 A pointer to a multi-instance device path data
                                structure.
  @param  Single                A pointer to a single-instance device path data
                                structure.

  @return This function removes the device path instances in Multi that overlap
          Single, and returns the resulting device path. If there is no
          remaining device path as a result, this function will return NULL.

**/
EFI_DEVICE_PATH_PROTOCOL *
EFIAPI
BdsLibDelPartMatchInstance (
  IN     EFI_DEVICE_PATH_PROTOCOL  *Multi,
  IN     EFI_DEVICE_PATH_PROTOCOL  *Single
  );

/**
  This function compares a device path data structure to that of all the nodes of a
  second device path instance.

  @param  Multi                 A pointer to a multi-instance device path data
                                structure.
  @param  Single                A pointer to a single-instance device path data
                                structure.

  @retval TRUE                  If the Single device path is contained within a 
                                Multi device path.
  @retval FALSE                 The Single device path is not contained within a 
                                Multi device path.

**/
BOOLEAN
EFIAPI
BdsLibMatchDevicePaths (
  IN  EFI_DEVICE_PATH_PROTOCOL  *Multi,
  IN  EFI_DEVICE_PATH_PROTOCOL  *Single
  );

/**
  This function converts an input device structure to a Unicode string.

  @param DevPath                  A pointer to the device path structure.

  @return A newly allocated Unicode string that represents the device path.

**/
CHAR16 *
EFIAPI
DevicePathToStr (
  IN EFI_DEVICE_PATH_PROTOCOL     *DevPath
  );

/**
  Check whether an instance in BlockIoDevicePath has the same partition node as the HardDriveDevicePath device path.

  @param  BlockIoDevicePath      Multi device path instances to check.
  @param  HardDriveDevicePath    A device path starting with a hard drive media
                                 device path.

  @retval TRUE                   There is a matched device path instance.
  @retval FALSE                  There is no matched device path instance.

**/
BOOLEAN
EFIAPI
MatchPartitionDevicePathNode (
  IN  EFI_DEVICE_PATH_PROTOCOL   *BlockIoDevicePath,
  IN  HARDDRIVE_DEVICE_PATH      *HardDriveDevicePath
  );


/**
  Expand a device path that starts with a hard drive media device path node to be a
  full device path that includes the full hardware path to the device. This function enables the device to boot. 
  To avoid requiring a connect on every boot, the front match is saved in a variable (the part point
  to the partition node. E.g. ACPI() /PCI()/ATA()/Partition() ).
  All successful history device paths
  that point to the front part of the partition node will be saved.

  @param  HardDriveDevicePath    EFI Device Path to boot, if it starts with a hard
                                 drive media device path.
  @return A Pointer to the full device path, or NULL if a valid Hard Drive devic path
          cannot be found.

**/
EFI_DEVICE_PATH_PROTOCOL *
EFIAPI
BdsExpandPartitionPartialDevicePathToFull (
  IN  HARDDRIVE_DEVICE_PATH      *HardDriveDevicePath
  );
  
/**
  Return the bootable media handle.
  First, check whether the device is connected.
  Second, check whether the device path points to a device that supports SimpleFileSystemProtocol.
  Third, detect the the default boot file in the Media, and return the removable Media handle.

  @param  DevicePath             The Device Path to a  bootable device.

  @return  The bootable media handle. If the media on the DevicePath is not bootable, NULL will return.

**/
EFI_HANDLE
EFIAPI
BdsLibGetBootableHandle (
  IN  EFI_DEVICE_PATH_PROTOCOL      *DevicePath
  );
  

/**
  Checks whether the Device path in a boot option points to a valid bootable device, and if the device
  is ready to boot now.

  @param  DevPath     The Device path in a boot option.
  @param  CheckMedia  If true, check whether the device is ready to boot now.

  @retval TRUE        The Device path is valid.
  @retval FALSE       The Device path is invalid.

**/
BOOLEAN
EFIAPI
BdsLibIsValidEFIBootOptDevicePath (
  IN EFI_DEVICE_PATH_PROTOCOL     *DevPath,
  IN BOOLEAN                      CheckMedia
  );

/**
  Checks whether the Device path in a boot option points to a valid bootable device, and if the device
  is ready to boot now.
  If Description is not NULL and the device path points to a fixed BlockIo
  device, this function checks whether the description conflicts with other auto-created
  boot options.

  @param  DevPath     The Device path in a boot option.
  @param  CheckMedia  If true, checks if the device is ready to boot now.
  @param  Description The description of a boot option.

  @retval TRUE        The Device path is valid.
  @retval FALSE       The Device path is invalid.

**/
BOOLEAN
EFIAPI
BdsLibIsValidEFIBootOptDevicePathExt (
  IN EFI_DEVICE_PATH_PROTOCOL     *DevPath,
  IN BOOLEAN                      CheckMedia,
  IN CHAR16                       *Description
  );

/**
  This function checks if a Fv file device path is valid, according to a file GUID. If it is invalid,
  it tries to return the valid device path.
  FV address maybe changes for memory layout adjust from time to time, use this funciton
  could promise the Fv file device path is right.

  @param  DevicePath             On input, the Fv file device path to check. On
                                 output, the updated valid Fv file device path
  @param  FileGuid               the Fv file GUID.

  @retval EFI_INVALID_PARAMETER  The input DevicePath or FileGuid is invalid.
  @retval EFI_UNSUPPORTED        The input DevicePath does not contain an Fv file
                                 GUID at all.
  @retval EFI_ALREADY_STARTED    The input DevicePath has pointed to the Fv file and is
                                 valid.
  @retval EFI_SUCCESS            Successfully updated the invalid DevicePath
                                 and returned the updated device path in DevicePath.

**/
EFI_STATUS
EFIAPI
BdsLibUpdateFvFileDevicePath (
  IN  OUT EFI_DEVICE_PATH_PROTOCOL      ** DevicePath,
  IN  EFI_GUID                          *FileGuid
  );


/**
  Connect the specific USB device that matches the RemainingDevicePath,
  and whose bus is determined by Host Controller (Uhci or Ehci).

  @param  HostControllerPI      Uhci (0x00) or Ehci (0x20) or Both uhci and ehci
                                (0xFF).
  @param  RemainingDevicePath   A short-form device path that starts with the first
                                element being a USB WWID or a USB Class device
                                path.

  @retval EFI_SUCCESS           The specific Usb device is connected successfully.
  @retval EFI_INVALID_PARAMETER Invalid HostControllerPi (not 0x00, 0x20 or 0xFF) 
                                or RemainingDevicePath is not the USB class device path.
  @retval EFI_NOT_FOUND         The device specified by device path is not found.

**/
EFI_STATUS
EFIAPI
BdsLibConnectUsbDevByShortFormDP(
  IN UINT8                      HostControllerPI,
  IN EFI_DEVICE_PATH_PROTOCOL   *RemainingDevicePath
  );
  

#endif
