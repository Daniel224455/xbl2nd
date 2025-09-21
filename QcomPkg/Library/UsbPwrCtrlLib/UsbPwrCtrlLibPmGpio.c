/**
@file  UsbPwrCtrlLibPmGpio.c
@brief Implementation of UsbPWrCtrlLibrary API for PMIC GPIO module
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
09/29/17   dc      Initial Release
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
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UsbPwrCtrlLib.h>
/**
  Protocol Dependencies
*/
#include <Protocol/EFIUsbPwrCtrl.h>
#include <Protocol/EFIPmicGpio.h>



/*===========================================================================*/
/*                  TYPEDEF AND MACRO DEFINITIONS                            */
/*===========================================================================*/

/*===========================================================================*/
/*                  FUNCTIONS PROTOTYPES                                     */
/*===========================================================================*/

/*===========================================================================*/
/*                  LOCAL VARIABLE DECLARATIONS                              */
/*===========================================================================*/
STATIC EFI_QCOM_PMIC_GPIO_PROTOCOL          *PmicGpioProtocol = NULL;
extern EFI_GUID gQcomPmicGpioProtocolGuid;


/**
  Initialize the interface to PMIC SCHG Module

  @param none

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
  EFI_UNSUPPORTED:              The physical device does not support this function
*/
EFI_STATUS
UsbPwrCtrlLibPmGpio_Init()
{
  EFI_STATUS Status = EFI_SUCCESS;

  // Get the PMIC USB charger protocol if not already available.
  if (NULL == PmicGpioProtocol) 
  {
    Status = gBS->LocateProtocol(&gQcomPmicGpioProtocolGuid, NULL, (void**)&PmicGpioProtocol);
    if (EFI_ERROR(Status))
    {
      PmicGpioProtocol = NULL;
    }
    else if(PmicGpioProtocol == NULL)
    {
      Status = EFI_DEVICE_ERROR;
    }
  }
  return Status;
}

/**
  Get Vbus Detect result from PMIC GPIO Module

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
  BOOLEAN *bVbusDetect)
{
  EFI_STATUS Status = EFI_SUCCESS;

  //Sanity Check
  if (NULL == PmicGpioProtocol)
  {
    return EFI_UNSUPPORTED;
  }
  if (bVbusDetect == NULL) 
  {
    return EFI_INVALID_PARAMETER;
  }

  Status = PmicGpioProtocol->ConfigDigitalInput(
      HwInfo.ResourceIndex,       //Which PMIC
      HwInfo.ResourceIDorAddr,    //Which GPIO
      EFI_PM_GPIO_I_SOURCE_PULL_DOWN_10uA,
      EFI_PM_GPIO_VIN1,           //Specify the VIN for VPH_PWR
      EFI_PM_GPIO_OUT_BUFFER_OFF,
      EFI_PM_GPIO_SOURCE_GND
    );
  if (Status != EFI_SUCCESS)
  {
    return Status;
  }

  Status = PmicGpioProtocol->IrqStatus(
      HwInfo.ResourceIndex,       //Which PMIC
      HwInfo.ResourceIDorAddr,    //Which GPIO
      EFI_PM_IRQ_STATUS_RT,
      bVbusDetect
    );
  return Status;
}

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
  BOOLEAN *bVbusSourceOK)
{
  EFI_STATUS Status = EFI_SUCCESS;
  //Sanity Check
  if (NULL == PmicGpioProtocol)
  {
    return EFI_UNSUPPORTED;
  }
  if (bVbusSourceOK == NULL) 
  {
    return EFI_INVALID_PARAMETER;
  }

  Status = PmicGpioProtocol->ConfigDigitalInput(
      HwInfo.ResourceIndex,       //Which PMIC
      HwInfo.ResourceIDorAddr,    //Which GPIO
      EFI_PM_GPIO_I_SOURCE_PULL_DOWN_10uA,
      EFI_PM_GPIO_VIN0,           //Specify the VIN for 1.8V
      EFI_PM_GPIO_OUT_BUFFER_OFF,
      EFI_PM_GPIO_SOURCE_GND
    );
  if (Status != EFI_SUCCESS)
  {
    return Status;
  }

  Status = PmicGpioProtocol->IrqStatus(
      HwInfo.ResourceIndex,       //Which PMIC
      HwInfo.ResourceIDorAddr,    //Which GPIO
      EFI_PM_IRQ_STATUS_RT,
      bVbusSourceOK
    );

  return Status;
}
/**
  Enable/Disable Vbus

  @param[IN]    HwInfo          Specify which port the request is for
  @param[IN]    bVbusEn         Enable/Dsiable Vbus

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
  EFI_UNSUPPORTED:              The physical device does not support this function
*/
EFI_STATUS
UsbPwrCtrlLibPmGpio_SetVbusEn(
  EFI_USB_PWR_CTRL_PORTHW_HW_INFO HwInfo,
  BOOLEAN bVbusEn)
{
  EFI_STATUS Status = EFI_SUCCESS;

  //Sanity Check
  if (NULL == PmicGpioProtocol)
  {
    return EFI_UNSUPPORTED;
  }

  Status = PmicGpioProtocol->ConfigDigitalOutput(
      HwInfo.ResourceIndex,       //Which PMIC
      HwInfo.ResourceIDorAddr,    //Which GPIO
      EFI_PM_GPIO_OUT_BUFFER_CONFIG_CMOS,
      EFI_PM_GPIO_VIN0,           //Specify the VIN for 1.8V
      EFI_PM_GPIO_SOURCE_GND,
      EFI_PM_GPIO_OUT_BUFFER_LOW,      
      bVbusEn
    );

  return Status;
}

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
  BOOLEAN bVconnEn)
{
  EFI_STATUS Status = EFI_SUCCESS;

  //Sanity Check
  if (NULL == PmicGpioProtocol)
  {
    return EFI_UNSUPPORTED;
  }

  Status = PmicGpioProtocol->ConfigDigitalOutput(
      HwInfo.ResourceIndex,       //Which PMIC
      HwInfo.ResourceIDorAddr,    //Which GPIO
      EFI_PM_GPIO_OUT_BUFFER_CONFIG_CMOS,
      EFI_PM_GPIO_VIN0,           //Specify the VIN for 1.8V
      EFI_PM_GPIO_SOURCE_GND,
      EFI_PM_GPIO_OUT_BUFFER_LOW, 
      bVconnEn
    );

  return Status;
}


