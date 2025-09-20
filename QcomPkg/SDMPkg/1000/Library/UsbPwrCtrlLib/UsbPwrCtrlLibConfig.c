/**
@file  UsbPwrCtrlLibConfig.c
@brief Provide Access to UsbPWrCtrlLibrary API
*/
/*=============================================================================
Copyright (c) 2017-2019 Qualcomm Technologies, Incorporated.
All rights reserved.
Qualcomm Technologies, Confidential and Proprietary.
=============================================================================*/

/*=============================================================================

                    EDIT HISTORY

when       who     what, where, why
--------   ---     -----------------------------------------------------------
02/06/19   vn      Update the configuration to use SchgChg only for HSUSB Port Type
10/10/18   ivy     Updated pmicglink to polling-based and support fetching usb info from charger FW
06/18/18   dc      Update CLS Platform configuration to 2 ports for new HW
06/18/18   dc      Add port indexes support in GetHwInfo API 
05/11/18   dc      Update Configuration for CLS platform
04/17/18   dc      Update Non-Error Debug Message Level to INFO
04/17/18   dc      Enable Support for Type-A Port
04/06/18   dc      Add 2nd port configuration 
11/14/17   ra      Remove support for ANX74XX HW
08/03/17   mr      Added support for ANX and ROHM Modules
07/13/17   dc      Initial Release
=============================================================================*/

/*===========================================================================*/
/*                  INCLUDE FILES                                            */
/*===========================================================================*/

#include <Uefi.h>
/**
  Library Dependencies
*/
#include <Library/QcomLib.h>
#include <Library/QcomBaseLib.h>
#include <Library/DebugLib.h>

/**
  Protocol Dependencies
*/

/**
  Internal Dependencies
*/
#include "UsbPwrCtrlLibConfig.h"
#include "UsbPwrCtrlLibPmSchg.h"
#include "UsbPwrCtrlLibPmUcsi.h"
#include "UsbPwrCtrlLibPmGpio.h"
#include "UsbPwrCtrlLibUsbSSCtrl.h"

/*===========================================================================*/
/*                  TYPEDEF AND MACRO DEFINITIONS                            */
/*===========================================================================*/

/*===========================================================================*/
/*                  FUNCTIONS PROTOTYPES                                     */
/*===========================================================================*/
EFI_STATUS UsbPwrCtrlLibConfigInit_default(VOID);
EFI_STATUS UsbPwrCtrlLibConfigInit_cls(VOID);

/*===========================================================================*/
/*                  LOCAL VARIABLE DECLARATIONS                              */
/*===========================================================================*/
/** USB Power Control Config for Default (MTP) Platform */
static EFI_USB_PWR_CTRL_HW_INFO sUsbPwrCtrlHwInfo_default =
{
  .NoOfPorts = 4,
  .PortIndexes = {0,1,2,3},

  /*1st Port Configuration*/
  .Ports[0].ConnectorType = EFI_USB_PWR_CTRL_PORTHW_CONNECTOR_TYPEC,

  .Ports[0].Usb2ChgDetect.HwType = {EFI_USB_PWR_CTRL_PORTHW_HW_PMSCHG, EFI_USB_PWR_CTRL_PORTHW_HW_NONE},
  .Ports[0].Usb2ChgDetect.HwInfo = {1, 0},

  .Ports[0].Usb3.SSCtrlExtDep = NULL,
  .Ports[0].Usb3.bSSSupported = TRUE,

  .Ports[0].TypeC.HwType = {EFI_USB_PWR_CTRL_PORTHW_HW_PMUCSI, EFI_USB_PWR_CTRL_PORTHW_HW_PMSCHG},
  .Ports[0].TypeC.HwInfo = {1, 0},
  .Ports[0].TypeC.HwExtDep = NULL,
  .Ports[0].TypeC.bPDSupported = FALSE,

  .Ports[0].VbusCtrl.HwType = {EFI_USB_PWR_CTRL_PORTHW_HW_AUTO, EFI_USB_PWR_CTRL_PORTHW_HW_NONE},
  .Ports[0].VbusCtrl.HwInfo = {1, 0},

  .Ports[0].VconnCtrl.HwType = {EFI_USB_PWR_CTRL_PORTHW_HW_AUTO, EFI_USB_PWR_CTRL_PORTHW_HW_NONE},
  .Ports[0].VconnCtrl.HwInfo = {1, 0},

  .Ports[0].VbusDetect.HwType = {EFI_USB_PWR_CTRL_PORTHW_HW_PMUCSI, EFI_USB_PWR_CTRL_PORTHW_HW_PMSCHG},
  .Ports[0].VbusDetect.HwInfo = {1, 0},

  .Ports[0].VbusSrcOkDetect.HwType = {EFI_USB_PWR_CTRL_PORTHW_HW_PMUCSI, EFI_USB_PWR_CTRL_PORTHW_HW_PMSCHG},
  .Ports[0].VbusSrcOkDetect.HwInfo = {1, 0},

  .Ports[0].UsbIdDetect.HwType = {EFI_USB_PWR_CTRL_PORTHW_HW_NONE, EFI_USB_PWR_CTRL_PORTHW_HW_NONE},
  .Ports[0].UsbIdDetect.HwInfo = {0, 0},

  .Ports[0].PowerSink.HwType = {EFI_USB_PWR_CTRL_PORTHW_HW_AUTO, EFI_USB_PWR_CTRL_PORTHW_HW_NONE},
  .Ports[0].PowerSink.HwInfo = {1, 0},
  .Ports[0].PowerSink.PwrCapability.MaxCurrentMA = 3000,
  .Ports[0].PowerSink.PwrCapability.MaxVoltageMV = 12000,

  .Ports[0].PowerSource.HwType = {EFI_USB_PWR_CTRL_PORTHW_HW_AUTO, EFI_USB_PWR_CTRL_PORTHW_HW_NONE},
  .Ports[0].PowerSource.HwInfo = {1, 0},
  .Ports[0].PowerSource.PwrCapability.MaxCurrentMA = 3000,
  .Ports[0].PowerSource.PwrCapability.MaxVoltageMV = 5000,

  /*2nd Port Configuration*/
  .Ports[1].ConnectorType = EFI_USB_PWR_CTRL_PORTHW_CONNECTOR_TYPEC,

  .Ports[1].Usb2ChgDetect.HwType = {EFI_USB_PWR_CTRL_PORTHW_HW_PMSCHG, EFI_USB_PWR_CTRL_PORTHW_HW_NONE},
  .Ports[1].Usb2ChgDetect.HwInfo = {5, 0},

  .Ports[1].Usb3.SSCtrlExtDep = NULL,
  .Ports[1].Usb3.bSSSupported = TRUE,

  .Ports[1].TypeC.HwType = {EFI_USB_PWR_CTRL_PORTHW_HW_PMUCSI, EFI_USB_PWR_CTRL_PORTHW_HW_PMSCHG},
  .Ports[1].TypeC.HwInfo = {5, 0},
  .Ports[1].TypeC.HwExtDep = NULL,
  .Ports[1].TypeC.bPDSupported = FALSE,

  .Ports[1].VbusCtrl.HwType = {EFI_USB_PWR_CTRL_PORTHW_HW_AUTO, EFI_USB_PWR_CTRL_PORTHW_HW_NONE},
  .Ports[1].VbusCtrl.HwInfo = {5, 0},

  .Ports[1].VconnCtrl.HwType = {EFI_USB_PWR_CTRL_PORTHW_HW_AUTO, EFI_USB_PWR_CTRL_PORTHW_HW_NONE},
  .Ports[1].VconnCtrl.HwInfo = {5, 0},

  .Ports[1].VbusDetect.HwType = {EFI_USB_PWR_CTRL_PORTHW_HW_PMUCSI, EFI_USB_PWR_CTRL_PORTHW_HW_PMSCHG},
  .Ports[1].VbusDetect.HwInfo = {5, 0},

  .Ports[1].VbusSrcOkDetect.HwType = {EFI_USB_PWR_CTRL_PORTHW_HW_PMUCSI, EFI_USB_PWR_CTRL_PORTHW_HW_PMSCHG},
  .Ports[1].VbusSrcOkDetect.HwInfo = {5, 0},

  .Ports[1].UsbIdDetect.HwType = {EFI_USB_PWR_CTRL_PORTHW_HW_NONE, EFI_USB_PWR_CTRL_PORTHW_HW_NONE},
  .Ports[1].UsbIdDetect.HwInfo = {0, 0},

  .Ports[1].PowerSink.HwType = {EFI_USB_PWR_CTRL_PORTHW_HW_AUTO, EFI_USB_PWR_CTRL_PORTHW_HW_NONE},
  .Ports[1].PowerSink.HwInfo = {5, 0},
  .Ports[1].PowerSink.PwrCapability.MaxCurrentMA = 3000,
  .Ports[1].PowerSink.PwrCapability.MaxVoltageMV = 12000,

  .Ports[1].PowerSource.HwType = {EFI_USB_PWR_CTRL_PORTHW_HW_AUTO, EFI_USB_PWR_CTRL_PORTHW_HW_NONE},
  .Ports[1].PowerSource.HwInfo = {5, 0},
  .Ports[1].PowerSource.PwrCapability.MaxCurrentMA = 3000,
  .Ports[1].PowerSource.PwrCapability.MaxVoltageMV = 5000,

  /*3rd Port Configuration*/
  .Ports[2].ConnectorType = EFI_USB_PWR_CTRL_PORTHW_CONNECTOR_TYPEA,

  .Ports[2].Usb2ChgDetect.HwType = {EFI_USB_PWR_CTRL_PORTHW_HW_NONE, EFI_USB_PWR_CTRL_PORTHW_HW_NONE},
  .Ports[2].Usb2ChgDetect.HwInfo = {0, 0},

  .Ports[2].Usb3.SSCtrlExtDep = NULL,
  .Ports[2].Usb3.bSSSupported = TRUE,

  .Ports[2].TypeC.HwType = {EFI_USB_PWR_CTRL_PORTHW_HW_NONE, EFI_USB_PWR_CTRL_PORTHW_HW_NONE},
  .Ports[2].TypeC.HwInfo = {0, 0},
  .Ports[2].TypeC.HwExtDep = NULL,
  .Ports[2].TypeC.bPDSupported = FALSE,

  .Ports[2].VbusCtrl.HwType = {EFI_USB_PWR_CTRL_PORTHW_HW_PMGPIO, EFI_USB_PWR_CTRL_PORTHW_HW_NONE},
  .Ports[2].VbusCtrl.HwInfo = {0, EFI_PM_GPIO_9},

  .Ports[2].VconnCtrl.HwType = {EFI_USB_PWR_CTRL_PORTHW_HW_NONE, EFI_USB_PWR_CTRL_PORTHW_HW_NONE},
  .Ports[2].VconnCtrl.HwInfo = {0, 0},

  .Ports[2].VbusDetect.HwType = {EFI_USB_PWR_CTRL_PORTHW_HW_NONE, EFI_USB_PWR_CTRL_PORTHW_HW_NONE},
  .Ports[2].VbusDetect.HwInfo = {0, 0},

  .Ports[2].VbusSrcOkDetect.HwType = {EFI_USB_PWR_CTRL_PORTHW_HW_NONE, EFI_USB_PWR_CTRL_PORTHW_HW_NONE},
  .Ports[2].VbusSrcOkDetect.HwInfo = {0, 0},

  .Ports[2].UsbIdDetect.HwType = {EFI_USB_PWR_CTRL_PORTHW_HW_NONE, EFI_USB_PWR_CTRL_PORTHW_HW_NONE},
  .Ports[2].UsbIdDetect.HwInfo = {0, 0},

  .Ports[2].PowerSink.HwType = {EFI_USB_PWR_CTRL_PORTHW_HW_NONE, EFI_USB_PWR_CTRL_PORTHW_HW_NONE},
  .Ports[2].PowerSink.HwInfo = {0, 0},
  .Ports[2].PowerSink.PwrCapability.MaxCurrentMA = 0,
  .Ports[2].PowerSink.PwrCapability.MaxVoltageMV = 0,

  .Ports[2].PowerSource.HwType = {EFI_USB_PWR_CTRL_PORTHW_HW_AUTO, EFI_USB_PWR_CTRL_PORTHW_HW_NONE},
  .Ports[2].PowerSource.HwInfo = {0, 0},
  .Ports[2].PowerSource.PwrCapability.MaxCurrentMA = 900,
  .Ports[2].PowerSource.PwrCapability.MaxVoltageMV = 5000,

  /*4th Port Configuration*/
  .Ports[3].ConnectorType = EFI_USB_PWR_CTRL_PORTHW_CONNECTOR_TYPEA,

  .Ports[3].Usb2ChgDetect.HwType = {EFI_USB_PWR_CTRL_PORTHW_HW_NONE, EFI_USB_PWR_CTRL_PORTHW_HW_NONE},
  .Ports[3].Usb2ChgDetect.HwInfo = {0, 0},

  .Ports[3].Usb3.SSCtrlExtDep = NULL,
  .Ports[3].Usb3.bSSSupported = TRUE,

  .Ports[3].TypeC.HwType = {EFI_USB_PWR_CTRL_PORTHW_HW_NONE, EFI_USB_PWR_CTRL_PORTHW_HW_NONE},
  .Ports[3].TypeC.HwInfo = {0, 0},
  .Ports[3].TypeC.HwExtDep = NULL,
  .Ports[3].TypeC.bPDSupported = FALSE,

  .Ports[3].VbusCtrl.HwType = {EFI_USB_PWR_CTRL_PORTHW_HW_PMGPIO, EFI_USB_PWR_CTRL_PORTHW_HW_NONE},
  .Ports[3].VbusCtrl.HwInfo = {4, EFI_PM_GPIO_9},

  .Ports[3].VconnCtrl.HwType = {EFI_USB_PWR_CTRL_PORTHW_HW_NONE, EFI_USB_PWR_CTRL_PORTHW_HW_NONE},
  .Ports[3].VconnCtrl.HwInfo = {0, 0},

  .Ports[3].VbusDetect.HwType = {EFI_USB_PWR_CTRL_PORTHW_HW_NONE, EFI_USB_PWR_CTRL_PORTHW_HW_NONE},
  .Ports[3].VbusDetect.HwInfo = {0, 0},

  .Ports[3].VbusSrcOkDetect.HwType = {EFI_USB_PWR_CTRL_PORTHW_HW_NONE, EFI_USB_PWR_CTRL_PORTHW_HW_NONE},
  .Ports[3].VbusSrcOkDetect.HwInfo = {0, 0},

  .Ports[3].UsbIdDetect.HwType = {EFI_USB_PWR_CTRL_PORTHW_HW_NONE, EFI_USB_PWR_CTRL_PORTHW_HW_NONE},
  .Ports[3].UsbIdDetect.HwInfo = {0, 0},

  .Ports[3].PowerSink.HwType = {EFI_USB_PWR_CTRL_PORTHW_HW_NONE, EFI_USB_PWR_CTRL_PORTHW_HW_NONE},
  .Ports[3].PowerSink.HwInfo = {0, 0},
  .Ports[3].PowerSink.PwrCapability.MaxCurrentMA = 0,
  .Ports[3].PowerSink.PwrCapability.MaxVoltageMV = 0,

  .Ports[3].PowerSource.HwType = {EFI_USB_PWR_CTRL_PORTHW_HW_AUTO, EFI_USB_PWR_CTRL_PORTHW_HW_NONE},
  .Ports[3].PowerSource.HwInfo = {0, 0},
  .Ports[3].PowerSource.PwrCapability.MaxCurrentMA = 900,
  .Ports[3].PowerSource.PwrCapability.MaxVoltageMV = 5000,
};

/** USB Power Control Config for CLS Platform */
static EFI_USB_PWR_CTRL_HW_INFO sUsbPwrCtrlHwInfo_cls =
{
  .NoOfPorts = 2,
  .PortIndexes = {0,1,0,0},

  /*1st Port Configuration*/
  .Ports[0].ConnectorType = EFI_USB_PWR_CTRL_PORTHW_CONNECTOR_TYPEC,

  .Ports[0].Usb2ChgDetect.HwType = {EFI_USB_PWR_CTRL_PORTHW_HW_PMSCHG, EFI_USB_PWR_CTRL_PORTHW_HW_NONE},
  .Ports[0].Usb2ChgDetect.HwInfo = {1, 0},

  .Ports[0].Usb3.SSCtrlExtDep = NULL,
  .Ports[0].Usb3.bSSSupported = TRUE,

  .Ports[0].TypeC.HwType = {EFI_USB_PWR_CTRL_PORTHW_HW_PMUCSI, EFI_USB_PWR_CTRL_PORTHW_HW_PMSCHG},
  .Ports[0].TypeC.HwInfo = {1, 0},
  .Ports[0].TypeC.HwExtDep = NULL,
  .Ports[0].TypeC.bPDSupported = FALSE,

  .Ports[0].VbusCtrl.HwType = {EFI_USB_PWR_CTRL_PORTHW_HW_AUTO, EFI_USB_PWR_CTRL_PORTHW_HW_NONE},
  .Ports[0].VbusCtrl.HwInfo = {1, 0},

  .Ports[0].VconnCtrl.HwType = {EFI_USB_PWR_CTRL_PORTHW_HW_AUTO, EFI_USB_PWR_CTRL_PORTHW_HW_NONE},
  .Ports[0].VconnCtrl.HwInfo = {1, 0},

  .Ports[0].VbusDetect.HwType = {EFI_USB_PWR_CTRL_PORTHW_HW_PMUCSI, EFI_USB_PWR_CTRL_PORTHW_HW_PMSCHG},
  .Ports[0].VbusDetect.HwInfo = {1, 0},

  .Ports[0].VbusSrcOkDetect.HwType = {EFI_USB_PWR_CTRL_PORTHW_HW_PMUCSI, EFI_USB_PWR_CTRL_PORTHW_HW_PMSCHG},
  .Ports[0].VbusSrcOkDetect.HwInfo = {1, 0},

  .Ports[0].UsbIdDetect.HwType = {EFI_USB_PWR_CTRL_PORTHW_HW_NONE, EFI_USB_PWR_CTRL_PORTHW_HW_NONE},
  .Ports[0].UsbIdDetect.HwInfo = {0, 0},

  .Ports[0].PowerSink.HwType = {EFI_USB_PWR_CTRL_PORTHW_HW_AUTO, EFI_USB_PWR_CTRL_PORTHW_HW_NONE},
  .Ports[0].PowerSink.HwInfo = {1, 0},
  .Ports[0].PowerSink.PwrCapability.MaxCurrentMA = 3000,
  .Ports[0].PowerSink.PwrCapability.MaxVoltageMV = 12000,

  .Ports[0].PowerSource.HwType = {EFI_USB_PWR_CTRL_PORTHW_HW_AUTO, EFI_USB_PWR_CTRL_PORTHW_HW_NONE},
  .Ports[0].PowerSource.HwInfo = {1, 0},
  .Ports[0].PowerSource.PwrCapability.MaxCurrentMA = 3000,
  .Ports[0].PowerSource.PwrCapability.MaxVoltageMV = 5000,

  /*2nd Port Configuration*/
  .Ports[1].ConnectorType = EFI_USB_PWR_CTRL_PORTHW_CONNECTOR_TYPEC,

  .Ports[1].Usb2ChgDetect.HwType = {EFI_USB_PWR_CTRL_PORTHW_HW_PMSCHG, EFI_USB_PWR_CTRL_PORTHW_HW_NONE},
  .Ports[1].Usb2ChgDetect.HwInfo = {5, 0},

  .Ports[1].Usb3.SSCtrlExtDep = NULL,
  .Ports[1].Usb3.bSSSupported = TRUE,

  .Ports[1].TypeC.HwType = {EFI_USB_PWR_CTRL_PORTHW_HW_PMUCSI, EFI_USB_PWR_CTRL_PORTHW_HW_PMSCHG},
  .Ports[1].TypeC.HwInfo = {5, 0},
  .Ports[1].TypeC.HwExtDep = NULL,
  .Ports[1].TypeC.bPDSupported = FALSE,

  .Ports[1].VbusCtrl.HwType = {EFI_USB_PWR_CTRL_PORTHW_HW_AUTO, EFI_USB_PWR_CTRL_PORTHW_HW_NONE},
  .Ports[1].VbusCtrl.HwInfo = {5, 0},

  .Ports[1].VconnCtrl.HwType = {EFI_USB_PWR_CTRL_PORTHW_HW_AUTO, EFI_USB_PWR_CTRL_PORTHW_HW_NONE},
  .Ports[1].VconnCtrl.HwInfo = {5, 0},

  .Ports[1].VbusDetect.HwType = {EFI_USB_PWR_CTRL_PORTHW_HW_PMUCSI, EFI_USB_PWR_CTRL_PORTHW_HW_PMSCHG},
  .Ports[1].VbusDetect.HwInfo = {5, 0},

  .Ports[1].VbusSrcOkDetect.HwType = {EFI_USB_PWR_CTRL_PORTHW_HW_PMUCSI, EFI_USB_PWR_CTRL_PORTHW_HW_PMSCHG},
  .Ports[1].VbusSrcOkDetect.HwInfo = {5, 0},

  .Ports[1].UsbIdDetect.HwType = {EFI_USB_PWR_CTRL_PORTHW_HW_NONE, EFI_USB_PWR_CTRL_PORTHW_HW_NONE},
  .Ports[1].UsbIdDetect.HwInfo = {0, 0},

  .Ports[1].PowerSink.HwType = {EFI_USB_PWR_CTRL_PORTHW_HW_AUTO, EFI_USB_PWR_CTRL_PORTHW_HW_NONE},
  .Ports[1].PowerSink.HwInfo = {5, 0},
  .Ports[1].PowerSink.PwrCapability.MaxCurrentMA = 3000,
  .Ports[1].PowerSink.PwrCapability.MaxVoltageMV = 12000,

  .Ports[1].PowerSource.HwType = {EFI_USB_PWR_CTRL_PORTHW_HW_AUTO, EFI_USB_PWR_CTRL_PORTHW_HW_NONE},
  .Ports[1].PowerSource.HwInfo = {5, 0},
  .Ports[1].PowerSource.PwrCapability.MaxCurrentMA = 3000,
  .Ports[1].PowerSource.PwrCapability.MaxVoltageMV = 5000,
};

EFI_USB_PWR_CTRL_PORTHW_EXT_HW_DEP sUsbPwrCtrlHw_SSCtrlDep[USB_PWR_CTRL_USB_SSCTRL_DEP_MAX] =
{
  [USB_PWR_CTRL_USB_SSCTRL_DEP_SS_DIR0]     = {EFI_USB_PWR_CTRL_PORTHW_EXT_DEP_MSM_RESOURCE, MSM_GPIO_7,   EFI_USB_PWR_CTRL_PORTHW_EXT_DEP_ENABLE_LOW},
  [USB_PWR_CTRL_USB_SSCTRL_DEP_SS_DIR1]     = {EFI_USB_PWR_CTRL_PORTHW_EXT_DEP_NC,           0,            EFI_USB_PWR_CTRL_PORTHW_EXT_DEP_ENABLE_LOW},
  [USB_PWR_CTRL_USB_SSCTRL_DEP_MODE_CTL0]   = {EFI_USB_PWR_CTRL_PORTHW_EXT_DEP_MSM_RESOURCE, MSM_GPIO_51,  EFI_USB_PWR_CTRL_PORTHW_EXT_DEP_ENABLE_LOW},
  [USB_PWR_CTRL_USB_SSCTRL_DEP_MODE_CTL1]   = {EFI_USB_PWR_CTRL_PORTHW_EXT_DEP_MSM_RESOURCE, MSM_GPIO_52,  EFI_USB_PWR_CTRL_PORTHW_EXT_DEP_ENABLE_LOW},
  [USB_PWR_CTRL_USB_SSCTRL_DEP_CC_FLIP]     = {EFI_USB_PWR_CTRL_PORTHW_EXT_DEP_MSM_RESOURCE, MSM_GPIO_6,   EFI_USB_PWR_CTRL_PORTHW_EXT_DEP_ENABLE_LOW}
};


EFI_USB_PWR_CTRL_HW_INFO *pActiveHwInfo = NULL;


/*===========================================================================*/
/*                 FUNCTION DEFINITIONS                                      */
/*===========================================================================*/

/**
  Initialize UsbPwrCtrl Hardware based input Platform Type

  @param[In] PlatformType       Platfrom Information to initialize the library.

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
  EFI_UNSUPPORTED:              The physical device does not support this function
*/
EFI_STATUS
UsbPwrCtrlLibConfig_Init(EFI_PLATFORMINFO_PLATFORM_TYPE PlatformType)
{
  EFI_STATUS Status = EFI_SUCCESS;

  switch (PlatformType)
  {
    case EFI_PLATFORMINFO_TYPE_CLS:
      USBPWRCTRL_LIB_DEBUG((EFI_D_INFO, "%a Initializing CLS configuration \r\n",  __FUNCTION__));
      pActiveHwInfo = &sUsbPwrCtrlHwInfo_cls;
      Status = UsbPwrCtrlLibConfigInit_cls();      
      break;
    default:
      USBPWRCTRL_LIB_DEBUG((EFI_D_INFO, "%a Initializing DEFAULT configuration \r\n",  __FUNCTION__));
      pActiveHwInfo = &sUsbPwrCtrlHwInfo_default;
      Status = UsbPwrCtrlLibConfigInit_default();
  }

  return Status;
}

/**
  Return UsbPwrCtrl Active Hardware Information

  @param[In] PlatformType       Hardware Information to initialize the library.
  @param[Out] pHwInfo           Pointer where HwInfo is saved

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
  EFI_UNSUPPORTED:              The physical device does not support this function
  EFI_NOT_READY:                The physical device is not ready for this request
*/

EFI_STATUS
UsbPwrCtrlLibConfig_GetHWInfo(EFI_USB_PWR_CTRL_HW_INFO **pHwInfo)
{
  EFI_STATUS Status = EFI_SUCCESS;

  if (pActiveHwInfo == NULL)
  {
    USBPWRCTRL_LIB_DEBUG((EFI_D_ERROR, "%a Hardware Info is not available \r\n"
                          , __FUNCTION__));
    return EFI_NOT_READY;
  }
  else
  {
    (*pHwInfo) = pActiveHwInfo;
  }
  return Status;
}

/**
  Initialize UsbPwrCtrl Library functions

  @param none

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
  EFI_UNSUPPORTED:              The physical device does not support this function
*/
EFI_STATUS UsbPwrCtrlLibConfigInit_default()
{
  EFI_STATUS Status = EFI_SUCCESS;
  //For default config, only need to initialize PMSCHG, PMGPIO, and PMUCSI module
  Status  = UsbPwrCtrlLibPmSchg_Init();
  Status  |= UsbPwrCtrlLibPmGpio_Init();
  Status  |= UsbPwrCtrlLibPmUcsi_Init();
  return Status;
}

/**
  Initialize UsbPwrCtrl Library functions - For CLS platform

  @param none

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
  EFI_UNSUPPORTED:              The physical device does not support this function
*/
EFI_STATUS UsbPwrCtrlLibConfigInit_cls()
{
  EFI_STATUS Status = EFI_SUCCESS;
  //For cls, only need to initialize PMSCHG and PMUCSI module
  Status  = UsbPwrCtrlLibPmSchg_Init();
  Status  |= UsbPwrCtrlLibPmUcsi_Init();
  return Status;
}
