/**
@file  ChargerLibFG.h
@brief ChargerLib API definitions for FG module
*/
/*=============================================================================
Copyright (c) 2020 Qualcomm Technologies, Incorporated.
All rights reserved.
Qualcomm Technologies, Confidential and Proprietary.
=============================================================================*/

/*=============================================================================
EDIT HISTORY


when       who     what, where, why
--------   ---     -----------------------------------------------------------
04/24/20   ravi    Added permanent failure status APIs
09/27/18   ivy     support fg ship mode
09/24/18   cs      enable soc threshold and loading FW by default
06/21/18   cs      Initial Release
=============================================================================*/
#ifndef __CHARGERLIB_FG_H__
#define __CHARGERLIB_FG_H__

/*===========================================================================*/
/*                  INCLUDE FILES                                            */
/*===========================================================================*/
#include <Uefi.h>
/**
  Library Dependencies
*/
#include "i2c_api.h"
#include <Library/UefiBootServicesTableLib.h>

/*===========================================================================*/
/*                  TYPEDEF AND MACRO DEFINITIONS                            */
/*===========================================================================*/
#define MANUFACTURER_NAME_MAX_SIZE  20

typedef VOID *SmartChargerFG_HandleType;

// I2C Slave Context for FG Device
typedef struct
{
  SmartChargerFG_HandleType hI2CFGHandle;           /**< I2C handle for slave addresses */
  i2c_instance              I2CFGInstance;          /**< I2C core Number */
  i2c_slave_config          I2CFGCfg;               /**< I2C configuration */
  UINT32                    TimeoutMs;              /**< Timeout in milisecond - in case required */
} SmartChargerFG_SlaveType;


typedef struct
{
  UINT32 StateOfCharge;
  INT32  ChargeCurrent;
  INT32  BatteryVoltage;
  INT32  BatteryTemperature;
}SmartChargerFG_BattStatusInfo;

//FG HW information
typedef struct _FG_PORTHW_HW_INFO
{
    UINT32  Index;
    UINT32  Address;
} FG_PORTHW_HW_INFO;

/*===========================================================================*/
/*                  FUNCTION PROTOTYPES                                      */
/*===========================================================================*/
/**
  Initialize the interface to FG module

  @param None

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
  EFI_UNSUPPORTED:              The physical device does not support this function
*/
EFI_STATUS
SmartChargerLibFG_Init(void);

/**
  De-initialize the interface to FG module

  @param None

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
  EFI_UNSUPPORTED:              The physical device does not support this function
*/
EFI_STATUS
SmartChargerLibFG_Deinit(void);


/**
  Get Battery Status from FG Module

  @param[IN]    HwInfo          Hardware Information for this request
  @param[OUT]   BatteryStatus   Return the battery status

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
  EFI_UNSUPPORTED:              The physical device does not support this function
*/
EFI_STATUS
SmartChargerLibFG_GetBatteryStatus(
  SmartChargerFG_BattStatusInfo *pBatteryStatus);


EFI_STATUS SmartChargerLibFG_GetManufactureName(UINT8 *manufacture_name, size_t size);

EFI_STATUS SmartChargerLibFG_GetChargingCurrentVoltage(UINT16* charging_current, UINT16* charging_voltage);
/**
  Enable FG ship mode

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
  EFI_UNSUPPORTED:              The physical device does not support this function
*/

EFI_STATUS SmartChargerLibFG_SetShipMode();

/**
  Get battery PF Active

  @param[OUT]   bPFActive        Return the battery Permanent Failure Active status

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
  EFI_UNSUPPORTED:              The physical device does not support this function
*/

EFI_STATUS SmartChargerLibFG_GetBatteryPFActive(BOOLEAN *bPFActive);

/**
  Get battery pack permanent failure status

  @param[OUT]   PfStatus        Return the battery permanent failure status

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
  EFI_UNSUPPORTED:              The physical device does not support this function
*/

EFI_STATUS SmartChargerLibFG_GetBatteryPFStatus(UINT32 *pPFStatus);

  #if 0
EFI_STATUS
SmartChargerLibFG_FgBasicExit(void);  
  
  
EFI_STATUS
SmartChargerLibFG_FgExtendedExit(void);  
 #endif 
#endif  /* __CHARGERLIB_FG_H__ */
