/**
@file  UsbPwrCtrlLibConfig.c
@brief Provide Access to UsbPWrCtrlLibrary API
*/
/*=============================================================================
Copyright (c) 2017-2018 Qualcomm Technologies, Incorporated.
All rights reserved.
Qualcomm Technologies, Confidential and Proprietary.
=============================================================================*/

/*=============================================================================

                    EDIT HISTORY

when       who     what, where, why
--------   ---     -----------------------------------------------------------
01/04/18   pbitra  Added back CLS Platform related config after License update
12/11/17   dc      Remove CLS Platform related config
11/12/17   dc      Update 2nd USB Port implementation
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

/**
  Protocol Dependencies
*/

/**
  Internal Dependencies
*/
#include "UsbPwrCtrlLibConfig.h"
#include "UsbPwrCtrlLibPmSchg.h"
#include "UsbPwrCtrlLibAnx74xx.h"
#include "UsbPwrCtrlLibUsbSSCtrl.h"
#include "UsbPwrCtrlLibPmGpio.h"

/*===========================================================================*/
/*                  TYPEDEF AND MACRO DEFINITIONS                            */
/*===========================================================================*/

/*===========================================================================*/
/*                  FUNCTIONS PROTOTYPES                                     */
/*===========================================================================*/
EFI_STATUS UsbPwrCtrlLibConfigInit_default(VOID);
EFI_STATUS UsbPwrCtrlLibConfigInit_cls(EFI_USB_PWR_CTRL_HW_INFO *pHwInfo);

/*===========================================================================*/
/*                  LOCAL VARIABLE DECLARATIONS                              */
/*===========================================================================*/
/** USB Power Control Config for Default (MTP) Platform */
static EFI_USB_PWR_CTRL_HW_INFO sUsbPwrCtrlHwInfo_default =
{
  .NoOfPorts = 2,
  /* Primary Port Configuration */
  .Ports[0].ConnectorType = EFI_USB_PWR_CTRL_PORTHW_CONNECTOR_TYPEC,

  .Ports[0].Usb2ChgDetect.HwType = EFI_USB_PWR_CTRL_PORTHW_HW_PMSCHG,
  .Ports[0].Usb2ChgDetect.HwInfo = {1, 0},

  .Ports[0].Usb3.SSCtrlExtDep = NULL,
  .Ports[0].Usb3.bSSSupported = TRUE,

  .Ports[0].TypeC.HwType = EFI_USB_PWR_CTRL_PORTHW_HW_PMSCHG,
  .Ports[0].TypeC.HwInfo = {1, 0},
  .Ports[0].TypeC.HwExtDep = NULL,
  .Ports[0].TypeC.bPDSupported = FALSE,

  .Ports[0].VbusCtrl.HwType = EFI_USB_PWR_CTRL_PORTHW_HW_PMSCHG,
  .Ports[0].VbusCtrl.HwInfo = {1, 0},

  .Ports[0].VconnCtrl.HwType = EFI_USB_PWR_CTRL_PORTHW_HW_PMSCHG,
  .Ports[0].VconnCtrl.HwInfo = {1, 0},

  .Ports[0].VbusDetect.HwType = EFI_USB_PWR_CTRL_PORTHW_HW_PMSCHG,
  .Ports[0].VbusDetect.HwInfo = {1, 0},

  .Ports[0].VbusSrcOkDetect.HwType = EFI_USB_PWR_CTRL_PORTHW_HW_PMSCHG,
  .Ports[0].VbusSrcOkDetect.HwInfo = {1, 0},

  .Ports[0].UsbIdDetect.HwType = EFI_USB_PWR_CTRL_PORTHW_HW_NONE,
  .Ports[0].UsbIdDetect.HwInfo = {0, 0},

  .Ports[0].PowerSink.HwType = EFI_USB_PWR_CTRL_PORTHW_HW_PMSCHG,
  .Ports[0].PowerSink.HwInfo = {1, 0},
  .Ports[0].PowerSink.PwrCapability.MaxCurrentMA = 3000,
  .Ports[0].PowerSink.PwrCapability.MaxVoltageMV = 12000,

  .Ports[0].PowerSource.HwType = EFI_USB_PWR_CTRL_PORTHW_HW_PMSCHG,
  .Ports[0].PowerSource.HwInfo = {1, 0},
  .Ports[0].PowerSource.PwrCapability.MaxCurrentMA = 3000,
  .Ports[0].PowerSource.PwrCapability.MaxVoltageMV = 5000,
  
  /* Secondary Port Configuration */
  .Ports[1].ConnectorType = EFI_USB_PWR_CTRL_PORTHW_CONNECTOR_uUSB,

  .Ports[1].Usb2ChgDetect.HwType = EFI_USB_PWR_CTRL_PORTHW_HW_NONE,
  .Ports[1].Usb2ChgDetect.HwInfo = {0, 0},

  .Ports[1].Usb3.SSCtrlExtDep = NULL,
  .Ports[1].Usb3.bSSSupported = FALSE,

  .Ports[1].TypeC.HwType = EFI_USB_PWR_CTRL_PORTHW_HW_NONE,
  .Ports[1].TypeC.HwInfo = {0, 0},
  .Ports[1].TypeC.HwExtDep = NULL,
  .Ports[1].TypeC.bPDSupported = FALSE,

  .Ports[1].VbusCtrl.HwType = EFI_USB_PWR_CTRL_PORTHW_HW_NONE,
  .Ports[1].VbusCtrl.HwInfo = {0, 0},

  .Ports[1].VconnCtrl.HwType = EFI_USB_PWR_CTRL_PORTHW_HW_NONE,
  .Ports[1].VconnCtrl.HwInfo = {0, 0},

  .Ports[1].VbusDetect.HwType = EFI_USB_PWR_CTRL_PORTHW_HW_PMGPIO,
  .Ports[1].VbusDetect.HwInfo = {1, 7}, //PMI GPIO8,

  .Ports[1].VbusSrcOkDetect.HwType = EFI_USB_PWR_CTRL_PORTHW_HW_NONE,
  .Ports[1].VbusSrcOkDetect.HwInfo = {0, 0},

  .Ports[1].UsbIdDetect.HwType = EFI_USB_PWR_CTRL_PORTHW_HW_NONE,
  .Ports[1].UsbIdDetect.HwInfo = {0, 0},

  .Ports[1].PowerSink.HwType = EFI_USB_PWR_CTRL_PORTHW_HW_NONE,
  .Ports[1].PowerSink.HwInfo = {0, 0},
  .Ports[1].PowerSink.PwrCapability.MaxCurrentMA = 0,
  .Ports[1].PowerSink.PwrCapability.MaxVoltageMV = 0,

  .Ports[1].PowerSource.HwType = EFI_USB_PWR_CTRL_PORTHW_HW_NONE,
  .Ports[1].PowerSource.HwInfo = {1, 0},
  .Ports[1].PowerSource.PwrCapability.MaxCurrentMA = 0,
  .Ports[1].PowerSource.PwrCapability.MaxVoltageMV = 0,
};

/** USB Power Control Config for CLS Platform */
EFI_USB_PWR_CTRL_PORTHW_EXT_HW_DEP sUsbPwrCtrlHw_TypeCDep[USB_PWR_CTRL_ANX74XX_EXT_DEP_MAX] =
{
  [USB_PWR_CTRL_ANX74XX_EXT_DEP_MODULE_EN]  = {EFI_USB_PWR_CTRL_PORTHW_EXT_DEP_MSM_RESOURCE, MSM_GPIO_129, EFI_USB_PWR_CTRL_PORTHW_EXT_DEP_ENABLE_HIGH}
};

EFI_USB_PWR_CTRL_PORTHW_EXT_HW_DEP sUsbPwrCtrlHw_SSCtrlDep[USB_PWR_CTRL_USB_SSCTRL_DEP_MAX] =
{
  [USB_PWR_CTRL_USB_SSCTRL_DEP_SS_DIR0]     = {EFI_USB_PWR_CTRL_PORTHW_EXT_DEP_MSM_RESOURCE, MSM_GPIO_7,   EFI_USB_PWR_CTRL_PORTHW_EXT_DEP_ENABLE_LOW},
  [USB_PWR_CTRL_USB_SSCTRL_DEP_SS_DIR1]     = {EFI_USB_PWR_CTRL_PORTHW_EXT_DEP_NC,           0,            EFI_USB_PWR_CTRL_PORTHW_EXT_DEP_ENABLE_LOW},
  [USB_PWR_CTRL_USB_SSCTRL_DEP_MODE_CTL0]   = {EFI_USB_PWR_CTRL_PORTHW_EXT_DEP_MSM_RESOURCE, MSM_GPIO_51,  EFI_USB_PWR_CTRL_PORTHW_EXT_DEP_ENABLE_LOW},
  [USB_PWR_CTRL_USB_SSCTRL_DEP_MODE_CTL1]   = {EFI_USB_PWR_CTRL_PORTHW_EXT_DEP_MSM_RESOURCE, MSM_GPIO_52,  EFI_USB_PWR_CTRL_PORTHW_EXT_DEP_ENABLE_LOW},
  [USB_PWR_CTRL_USB_SSCTRL_DEP_CC_FLIP]     = {EFI_USB_PWR_CTRL_PORTHW_EXT_DEP_MSM_RESOURCE, MSM_GPIO_6,   EFI_USB_PWR_CTRL_PORTHW_EXT_DEP_ENABLE_LOW}
};

static EFI_USB_PWR_CTRL_HW_INFO sUsbPwrCtrlHwInfo_cls =
{
  .NoOfPorts = 1,
  .Ports[0].ConnectorType = EFI_USB_PWR_CTRL_PORTHW_CONNECTOR_TYPEC,

  .Ports[0].Usb2ChgDetect.HwType = EFI_USB_PWR_CTRL_PORTHW_HW_ROHM,
  .Ports[0].Usb2ChgDetect.HwInfo = {I2C_INSTANCE_013, 0x09},

  .Ports[0].Usb3.SSCtrlExtDep = sUsbPwrCtrlHw_SSCtrlDep,
  .Ports[0].Usb3.bSSSupported = TRUE,

  .Ports[0].TypeC.HwType = EFI_USB_PWR_CTRL_PORTHW_HW_ANX74XX,
  .Ports[0].TypeC.HwInfo = {I2C_INSTANCE_011, ANX74XX_SPI_ADDR},
  .Ports[0].TypeC.HwExtDep = sUsbPwrCtrlHw_TypeCDep,
  .Ports[0].TypeC.bPDSupported = TRUE,

  .Ports[0].VbusCtrl.HwType = EFI_USB_PWR_CTRL_PORTHW_HW_AUTO,
  .Ports[0].VbusCtrl.HwInfo = {I2C_INSTANCE_011, ANX74XX_SPI_ADDR},

  .Ports[0].VconnCtrl.HwType = EFI_USB_PWR_CTRL_PORTHW_HW_AUTO,         /* Auto from ANX */
  .Ports[0].VconnCtrl.HwInfo = {I2C_INSTANCE_011, ANX74XX_SPI_ADDR},

  .Ports[0].VbusDetect.HwType = EFI_USB_PWR_CTRL_PORTHW_HW_ROHM,
  .Ports[0].VbusDetect.HwInfo = {I2C_INSTANCE_013, 0x09},

  .Ports[0].VbusSrcOkDetect.HwType = EFI_USB_PWR_CTRL_PORTHW_HW_NONE,   /* TODO: Need to relook for TLMM GPIO Use */
  .Ports[0].VbusSrcOkDetect.HwInfo = {0, 0},

  .Ports[0].UsbIdDetect.HwType = EFI_USB_PWR_CTRL_PORTHW_HW_NONE,
  .Ports[0].UsbIdDetect.HwInfo = {0, 0},

  .Ports[0].PowerSink.HwType = EFI_USB_PWR_CTRL_PORTHW_HW_ROHM,
  .Ports[0].PowerSink.HwInfo = {I2C_INSTANCE_013, 0x09},
  .Ports[0].PowerSink.PwrCapability.MaxCurrentMA = 3000,
  .Ports[0].PowerSink.PwrCapability.MaxVoltageMV = 20000,

  .Ports[0].PowerSource.HwType = EFI_USB_PWR_CTRL_PORTHW_HW_AUTO,       /* Auto from ANX */
  .Ports[0].PowerSource.HwInfo = {0, 0},
  .Ports[0].PowerSource.PwrCapability.MaxCurrentMA = 3000,
  .Ports[0].PowerSource.PwrCapability.MaxVoltageMV = 5000,

  .Ports[1] = {0}
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
      USBPWRCTRL_LIB_DEBUG((EFI_D_WARN, "%a Initializing CLS configuration \r\n",  __FUNCTION__));
      pActiveHwInfo = &sUsbPwrCtrlHwInfo_cls;
      Status = UsbPwrCtrlLibConfigInit_cls(pActiveHwInfo);
      break;

    default:
      USBPWRCTRL_LIB_DEBUG((EFI_D_WARN, "%a Initializing DEFAULT configuration \r\n",  __FUNCTION__));
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
  //For default config, only need to initialize PMSCHG module
  Status  = UsbPwrCtrlLibPmSchg_Init();
  Status  |= UsbPwrCtrlLibPmGpio_Init();
  return Status;
}

/**
  Initialize UsbPwrCtrl Library functions for CLS Platform

  @param none

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
  EFI_UNSUPPORTED:              The physical device does not support this function
*/
EFI_STATUS UsbPwrCtrlLibConfigInit_cls(EFI_USB_PWR_CTRL_HW_INFO *pHwInfo)
{
  EFI_STATUS Status = EFI_SUCCESS;
  UINT8 PortIndex = 0;

  if (pHwInfo == NULL)
  {
    USBPWRCTRL_LIB_DEBUG((EFI_D_ERROR, "%a Hardware Info is not available \r\n"
                          , __FUNCTION__));
    return EFI_INVALID_PARAMETER;
  }

  // Initialize ROHM Charger Module
  Status = ChargerExtHwLib_Init();
  if(EFI_ERROR(Status))
  {
    USBPWRCTRL_LIB_DEBUG((EFI_D_ERROR, "%a LocateProtocol Failed, Status [%r] \r\n", __FUNCTION__, Status));
    return Status;
  }

  if(pHwInfo->NoOfPorts >= EFI_USB_PWR_CTRL_USBPORT_MAX)
    pHwInfo->NoOfPorts = EFI_USB_PWR_CTRL_USBPORT_MAX;

  for(PortIndex = 0; PortIndex < pHwInfo->NoOfPorts; PortIndex++)
  {
    // For CLS Platform, initialization for ANX Module
    Status  = UsbPwrCtrlLibAnx74xx_Init(pHwInfo->Ports[PortIndex].TypeC);
    if(EFI_ERROR(Status))
    {
      USBPWRCTRL_LIB_DEBUG((EFI_D_ERROR, "%a LocateProtocol Failed, Status [%r] \r\n", __FUNCTION__, Status));
      return Status;
    }

    // For CLS Platform, USB SS config at Init time
    Status  = UsbPwrCtrlLibUSBSSCtrl_Init(pHwInfo->Ports[PortIndex].Usb3);
    if(EFI_ERROR(Status))
    {
      USBPWRCTRL_LIB_DEBUG((EFI_D_ERROR, "%a LocateProtocol Failed, Status [%r] \r\n", __FUNCTION__, Status));
      return Status;
    }
  }

  return Status;
}
