/**
@file  UsbPwrCtrlLibAnx74xx.h
@brief Provide UsbPwrCtrlLib Anx74xx related definition
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
02/01/18   mr      Added low level ANX Role select API
07/31/17   mr      Initial Release
=============================================================================*/
#ifndef __USBPWRCTRLLIBANX74XX_CONFIG_H__
#define __USBPWRCTRLLIBANX74XX_CONFIG_H__

/*===========================================================================*/
/*                  INCLUDE FILES                                            */
/*===========================================================================*/
#include <Uefi.h>
/**
  Library Dependencies
*/
#include <Library/UefiBootServicesTableLib.h>
#include <Library/QcomBaseLib.h>

/**
  Protocol Dependencies
*/
#include <Protocol/EFITlmm.h>

/**
  Internal Dependencies
*/
#include "UsbPwrCtrlLibConfig.h"
#include "Anx74XXLib.h"


/*===========================================================================*/
/*                  TYPEDEF AND MACRO DEFINITIONS                            */
/*===========================================================================*/
#define ANX74XX_TCPC_ADDR   0x2A     /* TCPC I2C Slave Address */
#define ANX74XX_SPI_ADDR    0x32     /* TCPC SPI Slave Address */

/* For MSM GPIO for Type-C Module Enable/Power-On */
#define MSM_GPIO_129        129     /* MSM GPIO129 */

typedef enum _USB_PWR_CTRL_ANX74XX_EXT_DEP_HW_TYPE
{
    USB_PWR_CTRL_ANX74XX_EXT_DEP_MODULE_EN,
    USB_PWR_CTRL_ANX74XX_EXT_DEP_MAX
} USB_PWR_CTRL_ANX74XX_EXT_DEP_HW_TYPE;


/*===========================================================================*/
/*                  FUNCTION PROTOTYPES                                      */
/*===========================================================================*/
/**
  Initialize the interface to ANX TypeC Controller module

  @param TypeC                  Refer EFI_USB_PWR_CTRL_PORTHW_TYPEC_INFO for details

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
  EFI_UNSUPPORTED:              The physical device does not support this function
*/
EFI_STATUS
UsbPwrCtrlLibAnx74xx_Init(EFI_USB_PWR_CTRL_PORTHW_TYPEC_INFO TypeC);

/**
  De-initialize the interface to ANX TypeC Controller module

  @param none

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
  EFI_UNSUPPORTED:              The physical device does not support this function
*/
EFI_STATUS
UsbPwrCtrlLibAnx74xx_Deinit(VOID);

/**
  Get TypeC Port Status

  @param[IN]    HwInfo          Hardware Information for Sink
  @param[OUT]   TypeCPortStatus Return TypeC port Status;

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
  EFI_UNSUPPORTED:              The physical device does not support this function
*/
EFI_STATUS
UsbPwrCtrlLibAnx74xx_GetTypeCPortStatus(
  EFI_USB_PWR_CTRL_PORTHW_HW_INFO HwInfo,
  EFI_USB_PWR_CTRL_TYPEC_PORT_STATUS *TypeCPortStatus);

/**
  Get TypeC PD Status

  @param[IN]    HwInfo          Hardware Information for this request
  @param[OUT]   PDStatus        Return PD Negotiation Status;

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
  EFI_UNSUPPORTED:              The physical device does not support this function

  @dependency
  This API has dependency on _GetTypeCPortStatus() called before checking for PD Status.
  It may take upto 500-700 mSec for PD negotiation/contract to complete;
  so PD Status might not be updated immediately after TypeC cable insertion.
*/
EFI_STATUS
UsbPwrCtrlLibAnx74xx_GetPDStatus(
  EFI_USB_PWR_CTRL_PORTHW_HW_INFO HwInfo,
  EFI_USB_PWR_CTRL_PD_STATUS  *PDStatus);

/**
  Enable/Disable Vconn

  @param[IN]    HwInfo          Specify which port the request is for
  @param[IN]    bVconnEn        Enable/Dsiable Vconn

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
  EFI_UNSUPPORTED:              The physical device does not support this function
*/
EFI_STATUS
UsbPwrCtrlLibAnx74xx_SetVconnEn(
  EFI_USB_PWR_CTRL_PORTHW_HW_INFO HwInfo,
  BOOLEAN bVconnEn);

/**
  Set TypeC Port Role

  @param[IN]    PortIndex       Specify H/w info
  @param[IN]    TypeCPortRole   TypeC Port Role

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
  EFI_UNSUPPORTED:              The physical device does not support this function
*/
EFI_STATUS
UsbPwrCtrlLibAnx74xx_SetTypeCPortRole(
  EFI_USB_PWR_CTRL_PORTHW_HW_INFO HwInfo,
  EFI_USB_PWR_CTRL_TYPEC_POWERROLE_TYPE TypeCPortRole);


#endif  /* __USBPWRCTRLLIBANX74XX_CONFIG_H__ */
