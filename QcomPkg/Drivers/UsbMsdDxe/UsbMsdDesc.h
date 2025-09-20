/** @file UsbMsdDesc.h

The header file to provide the access to the USB descriptors

Copyright (c) 2014,2016 QUALCOMM Technologies Inc. All rights reserved.

**/

/*=============================================================================
EDIT HISTORY


when       who     what, where, why
--------   ---     -----------------------------------------------------------
10/11/18   kameya  Support USB SLT loopback feature.
01/25/16   ck      Separate EDK2 undefined USB data structure to EFIUsbEx.h
10/14/14   ck      Init Version
=============================================================================*/

#ifndef __USB_MSD_DESC_H__
#define __USB_MSD_DESC_H__

#include <Protocol/EFIUsbfnIo.h>  // EFI_USB_*_DESCRIPTOR and EFI_USB_DEVICE_INFO
#include <Protocol/EFIUsbEx.h>    // USB data structure

/* endpoint number for control and bulk pipes */
#define USBMSD_CONTROL_EP    0
#define USBMSD_BULK_EP       1

/* USB MSD USB device configuration */
typedef struct _USB_CONFIGURATION_TREE{
  EFI_USB_CONFIG_DESCRIPTOR    ConfigDescriptor;
  EFI_USB_INTERFACE_DESCRIPTOR InterfaceDescriptor;
  EFI_USB_ENDPOINT_DESCRIPTOR  EndpointDescriptor0;
  EFI_USB_ENDPOINT_DESCRIPTOR  EndpointDescriptor1;
} USB_CONFIGURATION_TREE;

typedef struct _USB_SS_CONFIGURATION_TREE{
  EFI_USB_CONFIG_DESCRIPTOR                 ConfigDescriptor;
  EFI_USB_INTERFACE_DESCRIPTOR              InterfaceDescriptor;
  EFI_USB_ENDPOINT_DESCRIPTOR               EndpointDescriptor0;
  EFI_USB_SS_ENDPOINT_COMPANION_DESCRIPTOR  EndpointCompanionDescriptor0;
  EFI_USB_ENDPOINT_DESCRIPTOR               EndpointDescriptor1;
  EFI_USB_SS_ENDPOINT_COMPANION_DESCRIPTOR  EndpointCompanionDescriptor1;
} USB_SS_CONFIGURATION_TREE;

extern USB_CONFIGURATION_TREE                     TotalConfigDescriptor;
extern USB_SS_CONFIGURATION_TREE                  TotalSSConfigDescriptor;

/* Device Info for USBfnIo protocol */
extern CONST  EFI_USB_DEVICE_INFO                 UsbMsdDeviceInfo;
extern CONST  EFI_USB_SUPERSPEED_DEVICE_INFO      UsbMsdSSDeviceInfo;


/* USB descriptors */
extern CONST  EFI_USB_DEVICE_DESCRIPTOR           DeviceDescriptor;
extern CONST  EFI_USB_DEVICE_DESCRIPTOR           SSDeviceDescriptor;


extern CONST  EFI_USB_DEVICE_QUALIFIER_DESCRIPTOR DeviceQualifier;

extern CONST  UINT8                               Str0Descriptor[4];

extern CONST  UINT8                               StrManufacturerDescriptor[18];

extern CONST  UINT8                               StrProductDescriptor[34];

extern CONST  UINT8                               StrSerialDescriptor[42];

#endif // #ifndef __USB_MSD_DESC_H__ 
