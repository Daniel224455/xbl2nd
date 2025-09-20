/**
@file  UsbPwrCtrlLibPmGpio.h
@brief Provide UsbPwrCtrlLibPmGpio Related definition
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
04/12/18   dc      Integrated PMIC Gpio Support
09/29/17   dc      Initial Release
=============================================================================*/
#ifndef __USBPWRCTRLLIBPMGPIO_CONFIG_H__
#define __USBPWRCTRLLIBPMGPIO_CONFIG_H__

/**
  Protocol Dependencies
*/
#include <Protocol/EFIUsbPwrCtrl.h>
#include <Protocol/EFIPmicGpio.h>

/**
  Initialize the interface to PMIC GPIO Module

  @param none

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
  EFI_UNSUPPORTED:              The physical device does not support this function
*/
EFI_STATUS
UsbPwrCtrlLibPmGpio_Init();

/**
  Get Vbus Detect result from PMIC SCHG Module

  @param[IN]    HwInfo          Hardware Information for this request
  @param[OUT]   bVbusDetect     Return the result of Vbus Detect  

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
  EFI_UNSUPPORTED:              The physical device does not support this function
*/
EFI_STATUS
UsbPwrCtrlLibPmGpio_GetVbusDetect(
  EFI_USB_PWR_CTRL_PORTHW_HW_INFO HwInfo,
  BOOLEAN *bVbusDetect);

/**
  Get Vbus Source OK result from Library

  @param[IN]    HwInfo          Hardware Information for this request
  @param[OUT]   bVbusSourceOK   Return the result of Vbus SourceOK

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
  EFI_UNSUPPORTED:              The physical device does not support this function
*/
EFI_STATUS
UsbPwrCtrlLibPmGpio_GetVbusSourceOK(
  EFI_USB_PWR_CTRL_PORTHW_HW_INFO HwInfo,
  BOOLEAN *bVbusSourceOK);

/**
  Get HSUSB Charger Port Type

  @param[IN]    HwInfo          Hardware Information for this request
  @param[OUT]   ChgPortType     Return Charger Port Tpye

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
  EFI_UNSUPPORTED:              The physical device does not support this function
*/
EFI_STATUS
UsbPwrCtrlLibPmGpio_SetVbusEn(
  EFI_USB_PWR_CTRL_PORTHW_HW_INFO HwInfo,
  BOOLEAN bVbusEn);

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
UsbPwrCtrlLibPmGpio_SetVconnEn(
  EFI_USB_PWR_CTRL_PORTHW_HW_INFO HwInfo,
  BOOLEAN bVconnEn);

#endif
