/**
@file  UsbPwrCtrlLibUsbSSCtrl.h
@brief Provide UsbPwrCtrlLib Usb SS Control related definition
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
08/29/17   mr      Initial Release
=============================================================================*/
#ifndef __USBPWRCTRLLIB_USB_SSCTRL_H__
#define __USBPWRCTRLLIB_USB_SSCTRL_H__

/*===========================================================================*/
/*                  INCLUDE FILES                                            */
/*===========================================================================*/
#include <Uefi.h>
/**
  Library Dependencies
*/
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UsbPwrCtrlLib.h>

/**
  Protocol Dependencies
*/
#include <Protocol/EFITlmm.h>

/**
  Internal Dependencies
*/
// #include "UsbPwrCtrlLibConfig.h"


/*===========================================================================*/
/*                  TYPEDEF AND MACRO DEFINITIONS                            */
/*===========================================================================*/
/* For MSM GPIO for Type-C Alternate Mode (Display Port) Funcnality */
#define MSM_GPIO_6          6       /* MSM GPIO6 */
#define MSM_GPIO_7          7       /* MSM GPIO7 */
#define MSM_GPIO_51         51      /* MSM GPIO129 */
#define MSM_GPIO_52         52      /* MSM GPIO129 */

typedef enum _USB_PWR_CTRL_USB_SSCTRL_DEP_TYPE
{
    USB_PWR_CTRL_USB_SSCTRL_DEP_SS_DIR0,
    USB_PWR_CTRL_USB_SSCTRL_DEP_SS_DIR1,
    USB_PWR_CTRL_USB_SSCTRL_DEP_MODE_CTL0,
    USB_PWR_CTRL_USB_SSCTRL_DEP_MODE_CTL1,
    USB_PWR_CTRL_USB_SSCTRL_DEP_CC_FLIP,
    USB_PWR_CTRL_USB_SSCTRL_DEP_MAX
} USB_PWR_CTRL_USB_SSCTRL_DEP_TYPE;

typedef enum _USB_PWR_CTRL_USB_SSCTRL_MODE_CONFIG_TYPE
{
    USB_PWR_CTRL_USB_SSCTRL_MODE_PWR_DOWN,            /* Power Down, Cable Mode */
    USB_PWR_CTRL_USB_SSCTRL_MODE_PWR_SRC_UNFLIPPED,   /* Power Source, No FLIP (CC1) */
    USB_PWR_CTRL_USB_SSCTRL_MODE_PWR_SRC_FLIPPED,     /* Power Source, FLIP (CC2) */
    USB_PWR_CTRL_USB_SSCTRL_MODE_PWR_SNK_UNFLIPPED,   /* Power Sink, No FLIP (CC1) */
    USB_PWR_CTRL_USB_SSCTRL_MODE_PWR_SNK_FLIPPED,     /* Power Sink, FLIP (CC2) */
    USB_PWR_CTRL_USB_SSCTRL_MODE_CONFIG_MAX
} USB_PWR_CTRL_USB_SSCTRL_MODE_CONFIG_TYPE;

typedef struct _USB_PWR_CTRL_USB_SSCTRL_MODE_CONFIG
{
    EFI_USB_PWR_CTRL_PORTHW_EXT_DEP_STATE UsbSSCtrlModeConfig[USB_PWR_CTRL_USB_SSCTRL_DEP_MAX];
} USB_PWR_CTRL_USB_SSCTRL_MODE_CONFIG;


/*===========================================================================*/
/*                  FUNCTION PROTOTYPES                                      */
/*===========================================================================*/
/**
  Initialize the interface to USB SS Control Dependencies

  @param HwInfo                 Refer EFI_USB_PWR_CTRL_PORTHW_USB3_HW_INFO for details

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
  EFI_UNSUPPORTED:              The physical device does not support this function
*/
EFI_STATUS
UsbPwrCtrlLibUSBSSCtrl_Init(EFI_USB_PWR_CTRL_PORTHW_USB3_HW_INFO USBSSCtrl);

/**
  De-initialize USB SS Control Dependencies

  @param none

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
  EFI_UNSUPPORTED:              The physical device does not support this function
*/
EFI_STATUS
UsbPwrCtrlLibUSBSSCtrl_Deinit(VOID);


/**
  Set USB SS Dependencies based on USB Port connect

  @param USBSSCtrlDep           Refer EFI_USB_PWR_CTRL_PORTHW_EXT_HW_DEP for details
  @param USBSSCtrlModeConfig    Refer USB_PWR_CTRL_USB_SSCTRL_MODE_CONFIG_TYPE for details

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
  EFI_UNSUPPORTED:              The physical device does not support this function
*/EFI_STATUS
UsbPwrCtrlLibUSBSSCtrl_SetModeConfig(
  EFI_USB_PWR_CTRL_PORTHW_EXT_HW_DEP *USBSSCtrlDep,
  USB_PWR_CTRL_USB_SSCTRL_MODE_CONFIG_TYPE USBSSCtrlModeConfig);


#endif  /* __USBPWRCTRLLIB_USB_SSCTRL_H__ */
