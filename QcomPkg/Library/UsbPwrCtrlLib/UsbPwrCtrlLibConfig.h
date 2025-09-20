/**
@file  UsbPwrCtrlLibConfig.h
@brief Provide UsbPwrCtrlLibConfig Related definition
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
06/18/18   dc      Add port indexes support in GetHwInfo API 
11/14/17   ra      Remove support for ChargerExtHW
08/03/17   mr      Added support for ANX and ROHM Modules
07/13/17   dc      Initial Release
=============================================================================*/
#ifndef __USBPWRCTRLLIB_CONFIG_H__
#define __USBPWRCTRLLIB_CONFIG_H__

/*===========================================================================*/
/*                  INCLUDE FILES                                            */
/*===========================================================================*/
#include <Uefi.h>

/**
  Library Dependencies
*/
#include "i2c_api.h"
#include <Library/UsbPwrCtrlLib.h>

/**
  Protocol Dependencies
*/
#include <Protocol/EFIPlatformInfo.h>


/*===========================================================================*/
/*                  TYPEDEF AND MACRO DEFINITIONS                            */
/*===========================================================================*/
#define EFI_USB_PWR_CTRL_PORTHW_MAX_PORT_NO           2   /*Support Upto 2 ports*/

typedef struct _EFI_USB_PWR_CTRL_HW_INFO
{
  UINT8                                               NoOfPorts;          /**< No of USB port in this platform */
  UINT8                                               PortIndexes[EFI_USB_PWR_CTRL_PORTHW_MAX_PORT_NO]; /**< Valid Indexes for Client to Query */
  EFI_USB_PWR_CTRL_PORTHW_INFO                        Ports[EFI_USB_PWR_CTRL_PORTHW_MAX_PORT_NO];  /**< Data Array contains port information */
}EFI_USB_PWR_CTRL_HW_INFO;

// I2C Slave Context for Anx/Charger Device
typedef VOID *UsbPwrCtrl_HandleType;

typedef struct
{
    UsbPwrCtrl_HandleType     hI2CHandle;       /**< I2C handle for slave addresses */
    i2c_instance              I2CInstance;      /**< I2C Core Number */
    i2c_slave_config          I2CSlaveCfg;      /**< I2C Slave Device configuration */
    UINT32                    I2CSlaveRegMap;   /**< I2C Slave Device Register Map size - 1Byte/2Byte */
    UINT32                    TimeoutMs;        /**< Timeout in milisecond - in case required */
} UsbPwrCtrl_Device;


/*===========================================================================*/
/*                  FUNCTION PROTOTYPES                                      */
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
UsbPwrCtrlLibConfig_Init(EFI_PLATFORMINFO_PLATFORM_TYPE PlatformType);


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
UsbPwrCtrlLibConfig_GetHWInfo(EFI_USB_PWR_CTRL_HW_INFO **pHwInfo);


/**
  APIs to read/write data on ANX/Charger Module

  @param I2CDev         - I2C Slave Device
  @param offset         - Register Offset Address
  @param data_in/out    - Data to be read/written
  @param size_in/out    - Size of data to be read/written

  @return
  I2C_SUCCESS:                  Function returned successfully.
  I2C_ERROR_INVALID_PARAMETER:  A Parameter was incorrect.
  I2C_ERROR_*:                  For other I2C bus/operation related errors, refer 'i2c_status'.
 */
i2c_status UsbPwrCtrl_i2c_read (UsbPwrCtrl_Device *I2CDev, UINT8 offset, UINT8 *data_out, UINT16 size_out);
i2c_status UsbPwrCtrl_i2c_write(UsbPwrCtrl_Device *I2CDev, UINT8 offset, UINT8 *data_in,  UINT16 size_in);


#endif
