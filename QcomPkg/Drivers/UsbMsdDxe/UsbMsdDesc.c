/**
* @file UsbMsdDesc.c
*
* Defines the USB descriptors used for the mass storage device enumeration.
*
* Copyright (c) 2014, QUALCOMM Technologies Inc. All rights reserved.
*/
/*=============================================================================
EDIT HISTORY

when       who     what, where, why
--------   ---     -----------------------------------------------------------
10/11/18   kameya  Support USB SLT loopback feature.
10/14/14   ck      Init version
=============================================================================*/
#include "UsbMsdDesc.h"          // header
#include "UsbMsdUtility.h"       // ENDPOINT_ADDR macro 


CONST
EFI_USB_DEVICE_DESCRIPTOR
DeviceDescriptor ={
  sizeof(EFI_USB_DEVICE_DESCRIPTOR),  // uint8  bLength;
  USB_DESC_TYPE_DEVICE,               // uint8  bDescriptorType;
  0x0200,                             // uint16 bcdUSB;
  0x00,                               // uint8  bDeviceClass;
  0x00,                               // uint8  bDeviceSubClass;
  0x00,                               // uint8  bDeviceProtocol;
  64,                                 // uint8  bMaxPacketSize0;
  0x05C6,                             // uint16 idVendor;
  0xF000,                             // uint16 idProduct;
  0x0,                                // uint16 bcdDevice;
  1,                                  // uint8  iManufacturer;
  2,                                  // uint8  iProduct;
  3,                                  // uint8  iSerialNumber;
  0x1                                 // uint8  bNumConfigurations;
};

CONST
EFI_USB_DEVICE_DESCRIPTOR
SSDeviceDescriptor = {
  sizeof(EFI_USB_DEVICE_DESCRIPTOR),  // uint8  bLength;
  USB_DESC_TYPE_DEVICE,               // uint8  bDescriptorType;
  0x0300,                             // uint16 bcdUSB;
  0x00,                               // uint8  bDeviceClass;
  0x00,                               // uint8  bDeviceSubClass;
  0x00,                               // uint8  bDeviceProtocol;
  9,                                  // uint8  bMaxPacketSize0;
  0x05C6,                             // uint16 idVendor;
  0xF000,                             // uint16 idProduct;
  0x0,                                // uint16 bcdDevice;
  1,                                  // uint8  iManufacturer;
  2,                                  // uint8  iProduct;
  3,                                  // uint8  iSerialNumber;
  0x1                                 // uint8  bNumConfigurations;
};

CONST
EFI_USB_DEVICE_QUALIFIER_DESCRIPTOR
DeviceQualifier ={
  sizeof(EFI_USB_DEVICE_QUALIFIER_DESCRIPTOR),  // uint8  bLength;
  USB_DESC_TYPE_DEVICE_QUALIFIER,               // uint8  bDescriptorType;
  0x0200,                                       // uint16 bcdUSB;
  0x00,                                         // uint8  bDeviceClass;
  0x00,                                         // uint8  bDeviceSubClass;
  0x00,                                         // uint8  bDeviceProtocol;
  64,                                           // uint8  bMaxPacketSize0;
  1,                                            // uint8  bNumConfigurations;
  0                                             // uint8  bReserved;
};

STATIC
CONST
struct {
  EFI_USB_BOS_DESCRIPTOR                BosDescriptor;
  EFI_USB_USB_20_EXTENSION_DESCRIPTOR   Usb2ExtDescriptor;
  EFI_USB_SUPERSPEED_USB_DESCRIPTOR     SsUsbDescriptor;
  EFI_USB_SUPERSPEEDPLUS_USB_DESCRIPTOR SspUsbDescriptor;
} BinaryObjectStore = {
  // BOS Descriiptor
  {
    sizeof(EFI_USB_BOS_DESCRIPTOR),               // Descriptor Size
    USB_DESC_TYPE_BOS,                            // Descriptor Type
    sizeof(BinaryObjectStore),                    // Total Length
    3                                             // Number of device capabilities
  },
  // USB2 Extension Desc
  {
    sizeof(EFI_USB_USB_20_EXTENSION_DESCRIPTOR),  // Descriptor Size
    USB_DESC_TYPE_DEVICE_CAPABILITY,              // Device Capability Type descriptor
    USB_DEV_CAP_TYPE_USB_20_EXTENSION,            // USB 2.0 Extension Capability Type
    0x2                                           // Supported device level features
  },
  // Super Speed Device Capability Desc
  {
    sizeof(EFI_USB_SUPERSPEED_USB_DESCRIPTOR),    // Descriptor Size
    USB_DESC_TYPE_DEVICE_CAPABILITY,              // Device Capability Type descriptor
    USB_DEV_CAP_TYPE_SUPERSPEED_USB,              // SuperSpeed Device Capability Type
    0x00,                                         // Supported device level features
    0x0E,                                         // Speeds Supported by the device: SS, HS and FS
    0x01,                                         // Functionality support
    0x07,                                         // U1 Device Exit Latency
    0x65                                          // U2 Device Exit Latency
  },
  // Super Speed Plus Device Capability Desc
  {
    sizeof(EFI_USB_SUPERSPEEDPLUS_USB_DESCRIPTOR), // Descriptor Size
    USB_DESC_TYPE_DEVICE_CAPABILITY,               // Device Capability Type descriptor
    USB_DEV_CAP_TYPE_SUPERSPEEDPLUS_USB,           // SuperSpeedPlus Device Capability Type
    0x00,                                          // Reserved
    0x00000001,                                    // Attributes
    0x1100,                                        // Functionality Support - Minimum 1 RX and 1 TX lane.
    0x00,                                          // Reserved
    {0x000A4030, 0x000A40B0},                      // Sublink Speed Attribute - total 2 symmetric lanes(Rx/TX), 10 Gbps each.
  }
};

USB_CONFIGURATION_TREE TotalConfigDescriptor ={
  {
    sizeof(EFI_USB_CONFIG_DESCRIPTOR),      // uint8  bLength;
    USB_DESC_TYPE_CONFIG,                   // uint8  bDescriptorType;
    sizeof(TotalConfigDescriptor),          // uint16 wTotalLength;
    1,                                      // uint8  bNumInterfaces;
    1,                                      // uint8  bConfigurationValue;
    0,                                      // uint8  iConfiguration;
    0x80,                                   // uint8  bmAttributes;
    10                                      // uint8  bMaxPower;
  },
  {
    sizeof(EFI_USB_INTERFACE_DESCRIPTOR),   // uint8  bLength;
    USB_DESC_TYPE_INTERFACE,                // uint8  bDescriptorType;
    0,                                      // uint8  bInterfaceNumber;
    0,                                      // uint8  bAlternateSetting;
    2,                                      // uint8  bNumEndpoints;
    0x08,                                   // uint8  bInterfaceClass;
    0x06,                                   // uint8  bInterfaceSubClass;
    0x50,                                   // uint8  bInterfaceProtocol;
    0                                       // uint8  iInterface;
  },
  {
    sizeof(EFI_USB_ENDPOINT_DESCRIPTOR),                           // uint8  bLength;
    USB_DESC_TYPE_ENDPOINT,                                        // uint8  bDescriptorType;
    ENDPOINT_ADDR(USBMSD_BULK_EP, EfiUsbEndpointDirectionHostIn),  // uint8  bEndpointAddress;
    USB_ENDPOINT_BULK,                                             // uint8  bmAttributes;
    512,                                                           // uint16 wMaxPacketSize;
    0                                                              // uint8  bInterval;
  },
  {
    sizeof(EFI_USB_ENDPOINT_DESCRIPTOR),                           // uint8  bLength;
    USB_DESC_TYPE_ENDPOINT,                                        // uint8  bDescriptorType;
    ENDPOINT_ADDR(USBMSD_BULK_EP, EfiUsbEndpointDirectionHostOut),    // uint8  bEndpointAddress;
    USB_ENDPOINT_BULK,                                             // uint8  bmAttributes;
    512,                                                           // uint16 wMaxPacketSize;
    0                                                              // uint8  bInterval;
  },
};

USB_SS_CONFIGURATION_TREE TotalSSConfigDescriptor = {
  {
    sizeof(EFI_USB_CONFIG_DESCRIPTOR),    // uint8  bLength;
    USB_DESC_TYPE_CONFIG,                 // uint8  bDescriptorType;
    sizeof(TotalSSConfigDescriptor),      // uint16 wTotalLength;
    1,                                    // uint8  bNumInterfaces;
    1,                                    // uint8  bConfigurationValue;
    0,                                    // uint8  iConfiguration;
    0x80,                                 // uint8  bmAttributes;
    10                                    // uint8  bMaxPower;
  },
  {
    sizeof(EFI_USB_INTERFACE_DESCRIPTOR), // uint8  bLength;
    USB_DESC_TYPE_INTERFACE,              // uint8  bDescriptorType;
    0,                                    // uint8  bInterfaceNumber;
    0,                                    // uint8  bAlternateSetting;
    2,                                    // uint8  bNumEndpoints;
    0x08,                                 // uint8  bInterfaceClass;
    0x06,                                 // uint8  bInterfaceSubClass;
    0x50,                                 // uint8  bInterfaceProtocol;
    0                                     // uint8  iInterface;
  },
  {
    sizeof(EFI_USB_ENDPOINT_DESCRIPTOR),                           // uint8  bLength;
    USB_DESC_TYPE_ENDPOINT,                                        // uint8  bDescriptorType;
    ENDPOINT_ADDR(USBMSD_BULK_EP, EfiUsbEndpointDirectionHostIn),  // uint8  bEndpointAddress;
    USB_ENDPOINT_BULK,                                             // uint8  bmAttributes;
    1024,                                                          // uint16 wMaxPacketSize;
    0                                                              // uint8  bInterval;
  },
  {
    sizeof(EFI_USB_SS_ENDPOINT_COMPANION_DESCRIPTOR), // uint8  bLength;
    USB_DESC_TYPE_SS_ENDPOINT_COMPANION,              // uint8  bDescriptorType;
    4,                                                // uint8  bMaxBurst;
    0,                                                // uint8  bmAttributes;
    0                                                 // uint16 wBytesPerInterval;
  },
  {
    sizeof(EFI_USB_ENDPOINT_DESCRIPTOR),                              // uint8  bLength;
    USB_DESC_TYPE_ENDPOINT,                                           // uint8  bDescriptorType;
    ENDPOINT_ADDR(USBMSD_BULK_EP, EfiUsbEndpointDirectionHostOut),    // uint8  bEndpointAddress;
    USB_ENDPOINT_BULK,                                                // uint8  bmAttributes;
    1024,                                                             // uint16 wMaxPacketSize;
    0                                                                 // uint8  bInterval;
  },
  {
    sizeof(EFI_USB_SS_ENDPOINT_COMPANION_DESCRIPTOR), // uint8  bLength;
    USB_DESC_TYPE_SS_ENDPOINT_COMPANION,              // uint8  bDescriptorType;
    4,                                                // uint8  bMaxBurst;
    0,                                                // uint8  bmAttributes;
    0                                                 // uint16 wBytesPerInterval;
  },
};

CONST
EFI_USB_ENDPOINT_DESCRIPTOR
*EndpointDescriptorTable[] ={
  &TotalConfigDescriptor.EndpointDescriptor0,
  &TotalConfigDescriptor.EndpointDescriptor1,
};

CONST
EFI_USB_INTERFACE_INFO
InterfaceInfo ={
  (EFI_USB_INTERFACE_DESCRIPTOR *)&TotalConfigDescriptor.InterfaceDescriptor,
  (EFI_USB_ENDPOINT_DESCRIPTOR **)EndpointDescriptorTable
};

CONST
EFI_USB_INTERFACE_INFO
*InterfaceInfoTable[] ={
  &InterfaceInfo,
};

CONST
EFI_USB_CONFIG_INFO
ConfigInfo ={
  (EFI_USB_CONFIG_DESCRIPTOR *)&TotalConfigDescriptor.ConfigDescriptor,
  (EFI_USB_INTERFACE_INFO **)InterfaceInfoTable
};

CONST
EFI_USB_CONFIG_INFO
*ConfigInfoTable[] ={
  &ConfigInfo,
};

CONST
EFI_USB_DEVICE_INFO
UsbMsdDeviceInfo ={
  (EFI_USB_DEVICE_DESCRIPTOR *)&DeviceDescriptor,
  (EFI_USB_CONFIG_INFO **)ConfigInfoTable
};

// UsbMsd SuperSpeed Device Info Structures
STATIC
CONST
EFI_USB_SUPERSPEED_ENDPOINT_DESCRIPTOR
SSEndpointDescriptor0 = {
  (EFI_USB_ENDPOINT_DESCRIPTOR *)&TotalSSConfigDescriptor.EndpointDescriptor0,
  (EFI_USB_SS_ENDPOINT_COMPANION_DESCRIPTOR *)&TotalSSConfigDescriptor.EndpointCompanionDescriptor0
};

STATIC
CONST
EFI_USB_SUPERSPEED_ENDPOINT_DESCRIPTOR
SSEndpointDescriptor1 = {
  (EFI_USB_ENDPOINT_DESCRIPTOR *)&TotalSSConfigDescriptor.EndpointDescriptor1,
  (EFI_USB_SS_ENDPOINT_COMPANION_DESCRIPTOR *)&TotalSSConfigDescriptor.EndpointCompanionDescriptor1
};

STATIC
CONST
EFI_USB_SUPERSPEED_ENDPOINT_DESCRIPTOR
*SSEndpointDescriptorTable[] = {
  &SSEndpointDescriptor0,
  &SSEndpointDescriptor1,
};

STATIC
CONST
EFI_USB_SUPERSPEED_INTERFACE_INFO
SSInterfaceInfo = {
  (EFI_USB_INTERFACE_DESCRIPTOR *)&TotalSSConfigDescriptor.InterfaceDescriptor,
  (EFI_USB_SUPERSPEED_ENDPOINT_DESCRIPTOR **)SSEndpointDescriptorTable
};

STATIC
CONST
EFI_USB_SUPERSPEED_INTERFACE_INFO
*SSInterfaceInfoTable[] = {
  &SSInterfaceInfo,
};

STATIC
CONST
EFI_USB_SUPERSPEED_CONFIG_INFO
SSConfigInfo = {
  (EFI_USB_CONFIG_DESCRIPTOR *)&TotalSSConfigDescriptor.ConfigDescriptor,
  (EFI_USB_SUPERSPEED_INTERFACE_INFO **)SSInterfaceInfoTable
};

STATIC
CONST
EFI_USB_SUPERSPEED_CONFIG_INFO
*SSConfigInfoTable[] = {
  &SSConfigInfo,
};

CONST
EFI_USB_SUPERSPEED_DEVICE_INFO
UsbMsdSSDeviceInfo = {
  (EFI_USB_DEVICE_DESCRIPTOR *)&SSDeviceDescriptor,
  (EFI_USB_SUPERSPEED_CONFIG_INFO **)SSConfigInfoTable,
  (EFI_USB_BOS_DESCRIPTOR *)&BinaryObjectStore
};

// String Descriptors
CONST
UINT8
Str0Descriptor[4] ={
  sizeof(Str0Descriptor),
  USB_DESC_TYPE_STRING,
  0x09, 0x04 // Langid : US_EN.
};

CONST
UINT8
StrManufacturerDescriptor[18] ={
  sizeof(StrManufacturerDescriptor),
  USB_DESC_TYPE_STRING,
  'Q', 0,
  'U', 0,
  'A', 0,
  'L', 0,
  'C', 0,
  'O', 0,
  'M', 0,
  'M', 0,
};

CONST
UINT8
StrProductDescriptor[34] ={
  sizeof(StrProductDescriptor),
  USB_DESC_TYPE_STRING,
  'U', 0,
  'S', 0,
  'B', 0,
  ' ', 0,
  'M', 0,
  'A', 0,
  'S', 0,
  'S', 0,
  ' ', 0,
  'S', 0,
  'T', 0,
  'O', 0,
  'R', 0,
  'A', 0,
  'G', 0,
  'E', 0,
};


/*
* If USBFn is not able to retrieve the serial number from the system, 
* use the hardcoded one. 
*/
CONST
UINT8
StrSerialDescriptor[42] ={
  sizeof(StrProductDescriptor),
  USB_DESC_TYPE_STRING,
  '0', 0,
  '5', 0,
  'C', 0,
  '6', 0,
  'F', 0,
  '0', 0,
  '0', 0,
  '0', 0,
  'D', 0,
  '3', 0,
  '5', 0,
  '3', 0,
  '9', 0,
  '5', 0,
  '4', 0,
  '7', 0,
  '6', 0,
  '3', 0,
  'A', 0,
  'F', 0,
};
