/**
@file  UsbPwrCtrlLibConfig.c
@brief Provide Access to UsbPWrCtrlLibrary API
*/
/*=============================================================================
Copyright (c) 2018 Qualcomm Technologies, Incorporated.
All rights reserved.
Qualcomm Technologies, Confidential and Proprietary.
=============================================================================*/

/*=============================================================================

                    EDIT HISTORY

when       who     what, where, why
--------   ---     -----------------------------------------------------------
06/18/18   dc      Add port indexes support in GetHwInfo API 
02/23/18   dc      Update HW Description Data Structure for Optional Backup configuration 
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

/**
  Protocol Dependencies
*/

/**
  Internal Dependencies
*/
#include "UsbPwrCtrlLibConfig.h"
#include "UsbPwrCtrlLibPmSchg.h"
#include "UsbPwrCtrlLibUsbSSCtrl.h"

/*===========================================================================*/
/*                  TYPEDEF AND MACRO DEFINITIONS                            */
/*===========================================================================*/

/*===========================================================================*/
/*                  FUNCTIONS PROTOTYPES                                     */
/*===========================================================================*/
EFI_STATUS UsbPwrCtrlLibConfigInit_default(VOID);

/*===========================================================================*/
/*                  LOCAL VARIABLE DECLARATIONS                              */
/*===========================================================================*/
/** USB Power Control Config for Default (MTP) Platform */
static EFI_USB_PWR_CTRL_HW_INFO sUsbPwrCtrlHwInfo_default =
{
  .NoOfPorts = 1,
  .PortIndexes = {0,0},
  .Ports[0].ConnectorType = EFI_USB_PWR_CTRL_PORTHW_CONNECTOR_TYPEC,

  .Ports[0].Usb2ChgDetect.HwType = {EFI_USB_PWR_CTRL_PORTHW_HW_PMSCHG, EFI_USB_PWR_CTRL_PORTHW_HW_NONE},
  .Ports[0].Usb2ChgDetect.HwInfo = {1, 0},

  .Ports[0].Usb3.SSCtrlExtDep = NULL,
  .Ports[0].Usb3.bSSSupported = TRUE,

  .Ports[0].TypeC.HwType = {EFI_USB_PWR_CTRL_PORTHW_HW_PMSCHG, EFI_USB_PWR_CTRL_PORTHW_HW_NONE},
  .Ports[0].TypeC.HwInfo = {1, 0},
  .Ports[0].TypeC.HwExtDep = NULL,
  .Ports[0].TypeC.bPDSupported = FALSE,

  .Ports[0].VbusCtrl.HwType = {EFI_USB_PWR_CTRL_PORTHW_HW_PMSCHG, EFI_USB_PWR_CTRL_PORTHW_HW_NONE},
  .Ports[0].VbusCtrl.HwInfo = {1, 0},

  .Ports[0].VconnCtrl.HwType = {EFI_USB_PWR_CTRL_PORTHW_HW_PMSCHG, EFI_USB_PWR_CTRL_PORTHW_HW_NONE},
  .Ports[0].VconnCtrl.HwInfo = {1, 0},

  .Ports[0].VbusDetect.HwType = {EFI_USB_PWR_CTRL_PORTHW_HW_PMSCHG, EFI_USB_PWR_CTRL_PORTHW_HW_NONE},
  .Ports[0].VbusDetect.HwInfo = {1, 0},

  .Ports[0].VbusSrcOkDetect.HwType = {EFI_USB_PWR_CTRL_PORTHW_HW_PMSCHG, EFI_USB_PWR_CTRL_PORTHW_HW_NONE},
  .Ports[0].VbusSrcOkDetect.HwInfo = {1, 0},

  .Ports[0].UsbIdDetect.HwType = {EFI_USB_PWR_CTRL_PORTHW_HW_NONE, EFI_USB_PWR_CTRL_PORTHW_HW_NONE},
  .Ports[0].UsbIdDetect.HwInfo = {0, 0},

  .Ports[0].PowerSink.HwType = {EFI_USB_PWR_CTRL_PORTHW_HW_PMSCHG, EFI_USB_PWR_CTRL_PORTHW_HW_NONE},
  .Ports[0].PowerSink.HwInfo = {1, 0},
  .Ports[0].PowerSink.PwrCapability.MaxCurrentMA = 3000,
  .Ports[0].PowerSink.PwrCapability.MaxVoltageMV = 12000,

  .Ports[0].PowerSource.HwType = {EFI_USB_PWR_CTRL_PORTHW_HW_PMSCHG, EFI_USB_PWR_CTRL_PORTHW_HW_NONE},
  .Ports[0].PowerSource.HwInfo = {1, 0},
  .Ports[0].PowerSource.PwrCapability.MaxCurrentMA = 3000,
  .Ports[0].PowerSource.PwrCapability.MaxVoltageMV = 5000,

  .Ports[1] = {0}
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
    default:
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
  return Status;
}

