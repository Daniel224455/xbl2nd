/**
@file  UsbPwrCtrlLib.h
@brief Provide UsbPwrCtrlLib Related definition
*/
/*=============================================================================
Copyright (c) 2017 Qualcomm Technologies, Incorporated.
All rights reserved.
Qualcomm Technologies, Confidential and Proprietary.
=============================================================================*/

/*=============================================================================
EDIT HISTORY


when       who     what, where, why
--------   ---     -----------------------------------------------------------
04/06/18   dc      Remove Debug Message Header
02/20/18   dc      Update HW Description Data Structure for Optional Backup configuration
08/30/17   mr      Moved HW_INFO struct from EFIUsbPwrCtrl.h file
07/13/17   dc      Initial Release
=============================================================================*/
#ifndef __USBPWRCTRLLIB_H__
#define __USBPWRCTRLLIB_H__


/*===========================================================================*/
/*                  INCLUDE FILES                                            */
/*===========================================================================*/

#include <Protocol/EFIUsbPwrCtrl.h>

/*===========================================================================*/
/*                  TYPEDEF AND MACRO DEFINITIONS                            */
/*===========================================================================*/
#ifndef USBPWRCTRL_LIB_DEBUG
  #define USBPWRCTRL_LIB_DEBUG(Expression) \
  do{ \
    _DEBUG (Expression); \
  } while (FALSE)
#endif


/*
Data Sturcuture for HW information and capability
*/
typedef enum _EFI_USB_PWR_CTRL_PORTHW_CONNECTOR_TYPE
{
  EFI_USB_PWR_CTRL_PORTHW_CONNECTOR_NONE,
  EFI_USB_PWR_CTRL_PORTHW_CONNECTOR_uUSB,
  EFI_USB_PWR_CTRL_PORTHW_CONNECTOR_TYPEC,
  EFI_USB_PWR_CTRL_PORTHW_CONNECTOR_TYPEA,
  EFI_USB_PWR_CTRL_PORTHW_CONNECTOR_INVALID
}EFI_USB_PWR_CTRL_PORTHW_CONNECTOR_TYPE;

typedef enum _EFI_USB_PWR_CTRL_PORTHW_BUS_TYPE
{
  EFI_USB_PWR_CTRL_PORTHW_BUS_NONE,                   /*HW resource not accessible, not supported or controled by HW*/
  EFI_USB_PWR_CTRL_PORTHW_BUS_INTERNAL,               /*HW resource access through MSM directly*/
  EFI_USB_PWR_CTRL_PORTHW_BUS_SPMI,                   /*HW resource access through SPMI bus*/
  EFI_USB_PWR_CTRL_PORTHW_BUS_I2C,                    /*HW resource access through I2C*/
  EFI_USB_PWR_CTRL_PORTHW_BUS_INVALID,
}EFI_USB_PWR_CTRL_PORTHW_BUS_TYPE;

typedef enum _EFI_USB_PWR_CTRL_PORTHW_HW_TYPE
{
  EFI_USB_PWR_CTRL_PORTHW_HW_NONE,
  EFI_USB_PWR_CTRL_PORTHW_HW_AUTO,
  EFI_USB_PWR_CTRL_PORTHW_HW_PMSCHG,
  EFI_USB_PWR_CTRL_PORTHW_HW_PMGPIO,
  EFI_USB_PWR_CTRL_PORTHW_HW_ROHM,
  EFI_USB_PWR_CTRL_PORTHW_HW_ANX74XX,
  EFI_USB_PWR_CTRL_PORTHW_HW_PMUCSI,
  EFI_USB_PWR_CTRL_PORTHW_HW_INVALID,
}EFI_USB_PWR_CTRL_PORTHW_HW_TYPE;

typedef enum _EFI_USB_PWR_CTRL_TYPEC_ROLE
{
  EFI_USB_PWR_CTRL_TYPEC_ROLE_NONE,
  EFI_USB_PWR_CTRL_TYPEC_ROLE_DRP,
  EFI_USB_PWR_CTRL_TYPEC_ROLE_UFP,
  EFI_USB_PWR_CTRL_TYPEC_ROLE_DFP,
  EFI_USB_PWR_CTRL_TYPEC_ROLE_INVALID
}EFI_USB_PWR_CTRL_TYPEC_ROLE;

typedef enum _EFI_USB_PWR_CTRL_PORTHW_EXT_HW_DEP_TYPE
{
  EFI_USB_PWR_CTRL_PORTHW_EXT_DEP_NC,             /* Not connected */
  EFI_USB_PWR_CTRL_PORTHW_EXT_DEP_MSM_RESOURCE,
  EFI_USB_PWR_CTRL_PORTHW_EXT_DEP_PMIC_RESOURCE,
  EFI_USB_PWR_CTRL_PORTHW_EXT_DEP_INVALID
}EFI_USB_PWR_CTRL_PORTHW_EXT_DEP_TYPE;

typedef enum _CHARGER_EXT_HW_EXT_HW_DEP_INIT_STATE
{
  EFI_USB_PWR_CTRL_PORTHW_EXT_DEP_ENABLE_HIZ,       /* TODO: HiZ case is not supported for now */
  EFI_USB_PWR_CTRL_PORTHW_EXT_DEP_ENABLE_HIGH,
  EFI_USB_PWR_CTRL_PORTHW_EXT_DEP_ENABLE_LOW,
  EFI_USB_PWR_CTRL_PORTHW_EXT_DEP_STATE_INVALID
} EFI_USB_PWR_CTRL_PORTHW_EXT_DEP_STATE;

typedef enum _EFI_USB_PWR_CTRL_PORTHW_HWTYPE_TYPE
{
  EFI_USB_PWR_CTRL_PORTHW_HWTYPE_PRIMARY,
  EFI_USB_PWR_CTRL_PORTHW_HWTYPE_SECONDARY,
  EFI_USB_PWR_CTRL_PORTHW_HWTYPE_MAX
}EFI_USB_PWR_CTRL_PORTHW_HWTYPE_TYPE;

///////////////////////////////////////////////////////////////////////////////

//General HW information
typedef struct _EFI_USB_PWR_CTRL_PORTHW_HW_INFO
{
  UINT32  ResourceIndex;
  UINT32  ResourceIDorAddr;
}EFI_USB_PWR_CTRL_PORTHW_HW_INFO;

//External HW Dependency
typedef struct _EFI_USB_PWR_CTRL_PORTHW_EXT_HW_DEP
{
  EFI_USB_PWR_CTRL_PORTHW_EXT_DEP_TYPE ExtDepResource;
  UINT32 ResourceID;
  EFI_USB_PWR_CTRL_PORTHW_EXT_DEP_STATE InitState;
}EFI_USB_PWR_CTRL_PORTHW_EXT_HW_DEP;

typedef struct _EFI_USB_PWR_CTRL_PORTHW_GENERAL_HW_INFO
{
  EFI_USB_PWR_CTRL_PORTHW_HW_TYPE                     HwType[EFI_USB_PWR_CTRL_PORTHW_HWTYPE_MAX];
  EFI_USB_PWR_CTRL_PORTHW_HW_INFO                     HwInfo;
}EFI_USB_PWR_CTRL_PORTHW_GENERAL_HW_INFO;

//USB3 Hardware Inforamtion
typedef struct _EFI_USB_PWR_CTRL_PORTHW_USB3_HW_INFO
{
  EFI_USB_PWR_CTRL_PORTHW_EXT_HW_DEP                  *SSCtrlExtDep;
  BOOLEAN                                             bSSSupported;       /**< Flag indicate if SuperSpeed USB (USB3.0) supported */
}EFI_USB_PWR_CTRL_PORTHW_USB3_HW_INFO;

//TypeC Hardware information
typedef struct _EFI_USB_PWR_CTRL_PORTHW_TYPEC_INFO
{
  EFI_USB_PWR_CTRL_PORTHW_HW_TYPE                     HwType[EFI_USB_PWR_CTRL_PORTHW_HWTYPE_MAX];
  EFI_USB_PWR_CTRL_PORTHW_HW_INFO                     HwInfo;
  EFI_USB_PWR_CTRL_PORTHW_EXT_HW_DEP                  *HwExtDep;
  BOOLEAN                                             bPDSupported;
}EFI_USB_PWR_CTRL_PORTHW_TYPEC_INFO;

//Power Source or Sink Hardware information
typedef struct _EFI_USB_PWR_CTRL_PORTHW_PWR_SRC_SNK_INFO
{
  EFI_USB_PWR_CTRL_PORTHW_HW_TYPE                     HwType[EFI_USB_PWR_CTRL_PORTHW_HWTYPE_MAX];
  EFI_USB_PWR_CTRL_PORTHW_HW_INFO                     HwInfo;
  EFI_USB_PWR_CTRL_PORTHW_PWR_CAP                     PwrCapability;
}EFI_USB_PWR_CTRL_PORTHW_PWR_SRC_SNK_INFO;

typedef struct _EFI_USB_PWR_CTRL_PORTHW_INFO
{
  EFI_USB_PWR_CTRL_PORTHW_CONNECTOR_TYPE              ConnectorType;      /**< Describe connector type for this port */
  /*USB Hardware*/
  EFI_USB_PWR_CTRL_PORTHW_GENERAL_HW_INFO             Usb2ChgDetect;      /**< Describe How USB2.0 is supported for this port */
  EFI_USB_PWR_CTRL_PORTHW_USB3_HW_INFO                Usb3;               /**< Describe How USB3.0 is supported for this port */
  /*TYPEC & PD Hardware*/
  EFI_USB_PWR_CTRL_PORTHW_TYPEC_INFO                  TypeC;              /**< Describe How Type-C Port is supported for this port */
  /*Power Control Hardware*/
  EFI_USB_PWR_CTRL_PORTHW_GENERAL_HW_INFO             VbusCtrl;           /**< Describe how Vbus Control is supported for this port */
  EFI_USB_PWR_CTRL_PORTHW_GENERAL_HW_INFO             VconnCtrl;          /**< Describe how Vconn Control is supported for this port */
  /*Power Detect Hardware*/
  EFI_USB_PWR_CTRL_PORTHW_GENERAL_HW_INFO             VbusDetect;         /**< Describe how Vbus Detection is supported for this port */
  EFI_USB_PWR_CTRL_PORTHW_GENERAL_HW_INFO             VbusSrcOkDetect;    /**< Describe how Vbus SourceOK Detection is done for this port */
  EFI_USB_PWR_CTRL_PORTHW_GENERAL_HW_INFO             UsbIdDetect;        /**< Describe how USB ID detection is supported for this port */
  /* Power Source and Sink Hardware*/
  EFI_USB_PWR_CTRL_PORTHW_PWR_SRC_SNK_INFO            PowerSource;        /**< Describe How Power Source is supported for this port */
  EFI_USB_PWR_CTRL_PORTHW_PWR_SRC_SNK_INFO            PowerSink;          /**< Describe How Power Sink is supported for this port */
  /*Power Error Detect Hardware*/
  //EFI_USB_PWR_CTRL_PORTHW_GENERAL_HW_INFO             VbusOV;             /**< Describe how Vbus Over-Voltage detection is supported for this port */
  //EFI_USB_PWR_CTRL_PORTHW_GENERAL_HW_INFO             VbusOC;             /**< Describe how Vbus Over-Current detection is supported for this port */
  //EFI_USB_PWR_CTRL_PORTHW_GENERAL_HW_INFO             VconnOC;            /**< Describe how Vconn Over-Current detection is supported for this port */
}EFI_USB_PWR_CTRL_PORTHW_INFO;

/**
  Initialize UsbPwrCtrl Library functions

  @param none

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
  EFI_UNSUPPORTED:              The physical device does not support this function
*/
EFI_STATUS
UsbPwrCtrlLib_Init(VOID);


/**
  Get USB Port Hw information

  @param[OUT]   pUsbPortHwInfo  Refer EFI_USB_PWR_CTRL_USB_PORTHW_INFO for more details

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
  EFI_UNSUPPORTED:              The physical device does not support this function
*/
EFI_STATUS
UsbPwrCtrlLib_GetHwInfo(
  EFI_USB_PWR_CTRL_USB_PORTHW_INFO *pUsbPortHwInfo);

/**
  Get Vbus Detect result from Library

  @param[IN]    PortIndex       Specify which port the request is for
  @param[OUT]   bVbusDetect     Return the result of Vbus Detect

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
  EFI_UNSUPPORTED:              The physical device does not support this function
*/
EFI_STATUS
UsbPwrCtrlLib_GetVbusDetect(
  UINT8 PortIndex,
  BOOLEAN *bVbusDetect);

/**
  Get Vbus Source OK result from Library

  @param[IN]    PortIndex       Specify which port the request is for
  @param[OUT]   bVbusSourceOK   Return the result of Vbus SourceOK

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
  EFI_UNSUPPORTED:              The physical device does not support this function
*/
EFI_STATUS
UsbPwrCtrlLib_GetVbusSourceOK(
  UINT8 PortIndex,
  BOOLEAN *bVbusSourceOK);

/**
  Get HSUSB Charger Port Type

  @param[IN]    PortIndex       Specify which port the request is for
  @param[OUT]   ChgPortType     Return Charger Port Tpye

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
  EFI_UNSUPPORTED:              The physical device does not support this function
*/
EFI_STATUS
UsbPwrCtrlLib_GetHSUsbChgPortType(
  UINT8 PortIndex,
  EFI_USB_PWR_CTRL_HSUSB_CHGPORT_TYPE *ChgPortType);

/**
  Get USB ID result from Library

  @param[IN]    PortIndex       Specify which port the request is for
  @param[OUT]   bUsbID          Return USB ID Status

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
  EFI_UNSUPPORTED:              The physical device does not support this function
*/
EFI_STATUS
UsbPwrCtrlLib_GetUsbIDStatus(
  UINT8 PortIndex,
  BOOLEAN *bUsbID);

/**
  Get TypeC Port Status

  @param[IN]    PortIndex       Specify which port the request is for
  @param[OUT]   TypeCPortStatus Return TypeC port Status;

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
  EFI_UNSUPPORTED:              The physical device does not support this function
*/
EFI_STATUS
UsbPwrCtrlLib_GetTypeCPortStatus(
  UINT8 PortIndex,
  EFI_USB_PWR_CTRL_TYPEC_PORT_STATUS *TypeCPortStatus);

/**
  Get Port PD Status

  @param[IN]    PortIndex       Specify which port the request is for
  @param[OUT]   PDStatus        Return PD Status;

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
  EFI_UNSUPPORTED:              The physical device does not support this function
*/
EFI_STATUS
UsbPwrCtrlLib_GetPDStatus(
    UINT8 PortIndex,
    EFI_USB_PWR_CTRL_PD_STATUS  *PDStatus);

/**
  Set Sink Max Input Current

  @param[IN]    PortIndex       Specify which port the request is for
  @param[IN]    MaxCurrentMA    Value to set Max Input Current

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
  EFI_UNSUPPORTED:              The physical device does not support this function
*/
EFI_STATUS
UsbPwrCtrlLib_SetSnkMaxInputCurrent(
  UINT8 PortIndex,
  UINT32 MaxCurrentMA);

/**
  Set Source Max Output Current

  @param[IN]    PortIndex       Specify which port the request is for
  @param[IN]    MaxCurrentMA    Value to set Max Output Current

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
  EFI_UNSUPPORTED:              The physical device does not support this function
*/
EFI_STATUS
UsbPwrCtrlLib_SetSrcMaxOutputCurrent(
  UINT8 PortIndex,
  UINT32 MaxCurrentMA);

/**
  Enable/Disable Vbus

  @param[IN]    PortIndex       Specify which port the request is for
  @param[IN]    bVbusEn         Enable/Dsiable Vbus

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
  EFI_UNSUPPORTED:              The physical device does not support this function
*/
EFI_STATUS
UsbPwrCtrlLib_SetVbusEn(
  UINT8 PortIndex,
  BOOLEAN bVbusEn);

/**
  Enable/Disable Vconn

  @param[IN]    PortIndex       Specify which port the request is for
  @param[IN]    bVconnEn        Enable/Dsiable Vbus

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
  EFI_UNSUPPORTED:              The physical device does not support this function
*/
EFI_STATUS
UsbPwrCtrlLib_SetVconnEn(
  UINT8 PortIndex,
  BOOLEAN bVconnEn);

#endif  /* __USBPWRCTRLLIB_H__ */
