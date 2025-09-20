/**
@file  ChargerLibFG.c
@brief Implementation of ChargerLib API for FG module
*/
/*=============================================================================
Copyright (c) 2018-2020 Qualcomm Technologies, Incorporated.
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
06/28/18   cs      fixed i2c open failure.
06/21/18   cs      Initial Release
=============================================================================*/

/*===========================================================================*/
/*                  INCLUDE FILES                                            */
/*===========================================================================*/
/**
  Library Dependencies
*/

/**
  Protocol Dependencies
*/

/**
  Internal Dependencies
*/
#include "ChargerLibFG.h"
#include <i2c_api.h>
#include <api/pmic/charger/ChargerLibCommon.h>
//#include <Protocol/EFIPlatformInfo.h>
//#include <Protocol/EFIPlatformInfo.h>
//#include <Protocol/EFIPmicFg.h>
//#include <Protocol/EFIPmicSchg.h>
////#include <Protocol/EFIPmicUsb.h>
//#include <Protocol/EFIPmicVersion.h>
//#include <Protocol/EFIPmicPwrOn.h>
//#include <Protocol/EFIVariableServices.h>
//#include <Protocol/EFITsens.h>
//#include <Protocol/EFIPmicRTC.h>
//#include <Protocol/EFIUsbfnChgSrv.h>
//#include <Protocol/EFIPmicRgbLed.h>
//#include <Protocol/EFIUsbInitProtocol.h>
//
//#include <Protocol/EFIPIL.h>

/*===========================================================================*/
/*                  TYPEDEF AND MACRO DEFINITIONS                            */
/*===========================================================================*/

#define I2C_SLAVE_ID_BATTERYPACK   0x0B    /* SlaveID for bq40z50*/
#define I2C_INSTANCE_BATTERYPACK   I2C_INSTANCE_020

/**
  SBS Commands
*/
#define SBS_CMD_ManufacturerAccess      0x00
#define SBS_CMD_BatteryMode             0x03
#define SBS_CMD_Temperatures            0x08
#define SBS_CMD_Voltage                 0x09
#define SBS_CMD_Current                 0x0A
#define SBS_CMD_RelativeStateOfCharge   0x0D
#define SBS_CMD_ChargingCurrent         0x14
#define SBS_CMD_ChargingVoltage         0x15
#define SBS_CMD_BatteryStatus           0x16
#define SBS_CMD_ManufactureName         0x20
#define SBS_CMD_ManufacturerData        0x23
#define SBS_CMD_ManufacturerBlockAccess 0x44
#define SBS_CMD_PFStatus                0x53
#define SBS_CMD_OperationStatus         0x54
#define SBS_CMD_ChargingStatus          0x55

/**
  SBS MAC Commands
*/
#define SBS_MAC_CMD_ShutdownMode        0x0010

#define FG_PERMANENT_FAILURE_MODE_STS_MASK  0x1000

#define SBS_DATA_SIZE 2
#define SBS_CMD_SIZE 1
#define SBS_PF_STATUS_DATA_SIZE 4
#define SBS_OP_STATUS_DATA_SIZE 4

#define FG_READ_RETRY_COUNT 3
#define FG_WRITE_RETRY_COUNT 3


#define TEMPP1K_TO_TEMPC(x) ((x-2730)/10)

/*===========================================================================*/
/*                  FUNCTIONS PROTOTYPES                                     */
/*===========================================================================*/
i2c_status fg_i2c_read(UINT8 offset, UINT8 *data_out, UINT16 size_out);
i2c_status fg_i2c_write(UINT8 offset, UINT8* data_in, UINT16 size_in);


/*===========================================================================*/
/*                  LOCAL VARIABLE DECLARATIONS                              */
/*===========================================================================*/
SmartChargerFG_SlaveType gFGDevice = {0};

FG_PORTHW_HW_INFO HwInfo = {I2C_INSTANCE_BATTERYPACK, I2C_SLAVE_ID_BATTERYPACK};
static BOOLEAN gFGI2cInitialized = FALSE;

/**
  Initialize the interface to FG module

  @param none

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_DEVICE_ERROR:             The physical device reported an error.
*/
EFI_STATUS
SmartChargerLibFG_Init(void)
{
    EFI_STATUS Status = EFI_SUCCESS;
    i2c_status i2c_sts = I2C_SUCCESS;

    gFGDevice.hI2CFGHandle = NULL;
    gFGDevice.I2CFGInstance = HwInfo.Index;

    gFGDevice.I2CFGCfg.bus_frequency_khz = 100;  /* 100 KHz */
    gFGDevice.I2CFGCfg.slave_address = HwInfo.Address;
    gFGDevice.I2CFGCfg.mode = I2C;
    gFGDevice.I2CFGCfg.slave_max_clock_stretch_us = 100;
    gFGDevice.I2CFGCfg.core_configuration1 = 0;
    gFGDevice.I2CFGCfg.core_configuration2 = 0;

    gFGDevice.TimeoutMs = 0;

    if (!gFGI2cInitialized)
    {
        i2c_sts = i2c_open(gFGDevice.I2CFGInstance, &gFGDevice.hI2CFGHandle);
        if(I2C_SUCCESS != i2c_sts)
        {
            CHARGER_DEBUG((EFI_D_WARN, "SmartChargerLibFG:: %a  i2c_open failed, Error = [%r] \r\n", __FUNCTION__, i2c_sts));
            return EFI_DEVICE_ERROR;
        }
        gFGI2cInitialized = TRUE;
    }

    CHARGER_DEBUG((EFI_D_WARN, "SmartChargerLibFG:: %a  FG handle opened = [%r] \r\n",__FUNCTION__, i2c_sts));

    return Status;
}

/**
  De-initialize the interface to FG module

  @param none

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_DEVICE_ERROR:             The physical device reported an error.
*/
EFI_STATUS
SmartChargerLibFG_Deinit(void)
{
    EFI_STATUS Status = EFI_DEVICE_ERROR;
    i2c_status i2c_sts;

    if (gFGDevice.hI2CFGHandle != NULL && gFGI2cInitialized)
    {
        i2c_sts = i2c_close(gFGDevice.hI2CFGHandle);
        if(I2C_SUCCESS == i2c_sts)
        {
            Status = EFI_SUCCESS;
            gFGI2cInitialized = FALSE;
        }
    }

    return Status;
}

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
  SmartChargerFG_BattStatusInfo *pBatteryStatus)
{
    i2c_status i2c_sts = I2C_SUCCESS;
    UINT16 data = 0;
    INT16 current = 0;

    // Sanity Check
    if (pBatteryStatus == NULL)
    {
        return EFI_INVALID_PARAMETER;
    }

    i2c_sts = fg_i2c_read(SBS_CMD_RelativeStateOfCharge, (UINT8 *)&data, SBS_DATA_SIZE);
    if(I2C_SUCCESS != i2c_sts)
    {
        CHARGER_DEBUG((EFI_D_WARN, "SmartChargerLibFG:: %a  FG SBS_CMD_RelativeStateOfCharge read error = [%r] \r\n", __FUNCTION__, i2c_sts));
        return EFI_DEVICE_ERROR;
    }
    pBatteryStatus->StateOfCharge = data;

    data = 0;
    i2c_sts = fg_i2c_read(SBS_CMD_Current, (UINT8 *)&data, SBS_DATA_SIZE);
    if(I2C_SUCCESS != i2c_sts)
    {
        CHARGER_DEBUG((EFI_D_WARN, "SmartChargerLibFG:: %a  FG BatteryChargeCurrent read error = [%r] \r\n", __FUNCTION__, i2c_sts));
        return EFI_DEVICE_ERROR;
    }
    current = (INT16)data;
    pBatteryStatus->ChargeCurrent= current;

    data = 0;
    i2c_sts = fg_i2c_read(SBS_CMD_Voltage, (UINT8 *)&data, SBS_DATA_SIZE);
    if(I2C_SUCCESS != i2c_sts)
    {
        CHARGER_DEBUG((EFI_D_WARN, "SmartChargerLibFG:: %a  FG SBS_CMD_Voltage read error = [%r] \r\n", __FUNCTION__, i2c_sts));
        return EFI_DEVICE_ERROR;
    }
    pBatteryStatus->BatteryVoltage = data;

    data = 0;
    i2c_sts = fg_i2c_read(SBS_CMD_Temperatures, (UINT8 *)&data, SBS_DATA_SIZE);
    if(I2C_SUCCESS != i2c_sts)
    {
        CHARGER_DEBUG((EFI_D_WARN, "SmartChargerLibFG:: %a  FG SBS_CMD_Temperatures read error = [%r] \r\n", __FUNCTION__, i2c_sts));
        return EFI_DEVICE_ERROR;
    }
    pBatteryStatus->BatteryTemperature = TEMPP1K_TO_TEMPC(data);

    return i2c_sts;
}

EFI_STATUS SmartChargerLibFG_GetManufactureName(UINT8 *manufacture_name, size_t size)
{
  EFI_STATUS Status = EFI_SUCCESS;
  i2c_status i2c_sts = I2C_SUCCESS;


  if (manufacture_name == NULL || size < (MANUFACTURER_NAME_MAX_SIZE + 1))
  {
    return EFI_INVALID_PARAMETER;
  }

  i2c_sts = fg_i2c_read(SBS_CMD_ManufactureName, manufacture_name, MANUFACTURER_NAME_MAX_SIZE);
  if(I2C_SUCCESS != i2c_sts)
  {
    CHARGER_DEBUG((EFI_D_WARN, "SmartChargerLibFG:: %a  FG SBS_CMD_ManufactureName read error = [%r] \r\n", __FUNCTION__, i2c_sts));
    Status = EFI_DEVICE_ERROR;
  }

  return Status;
}

EFI_STATUS SmartChargerLibFG_GetChargingCurrentVoltage(UINT16* charging_current, UINT16* charging_voltage)
{
  EFI_STATUS Status = EFI_SUCCESS;
  i2c_status i2c_sts = I2C_SUCCESS;
  if (charging_current == NULL || charging_voltage == NULL)
  {
    return EFI_INVALID_PARAMETER;
  }

  i2c_sts  = fg_i2c_read(SBS_CMD_ChargingCurrent, (UINT8*)charging_current, SBS_DATA_SIZE);
  i2c_sts |= fg_i2c_read(SBS_CMD_ChargingVoltage, (UINT8*)charging_voltage, SBS_DATA_SIZE);
  if(I2C_SUCCESS != i2c_sts)
  {
    CHARGER_DEBUG((EFI_D_WARN, "SmartChargerLibFG:: %a  FG SBS_CMD_ChargingCurrent/Votage read error = [%r] \r\n", __FUNCTION__, i2c_sts));
    Status = EFI_DEVICE_ERROR;
  }

  return Status;
}

EFI_STATUS SmartChargerLibFG_SetShipMode()
{
  EFI_STATUS Status = EFI_SUCCESS;
  i2c_status i2c_sts = I2C_SUCCESS;
  UINT8 shutDown_write_count = 2;
  UINT16 data = SBS_MAC_CMD_ShutdownMode;

  /* Write shutdown command twice to take effect */
  while (shutDown_write_count)
  {
    i2c_sts |= fg_i2c_write(SBS_CMD_ManufacturerAccess, (UINT8*)&data, SBS_DATA_SIZE);
    shutDown_write_count --;
  }
  if(I2C_SUCCESS != i2c_sts)
  {
    CHARGER_DEBUG((EFI_D_WARN, "SmartChargerLibFG:: %a  FG SBS_MAC_CMD_IATAShutdown write error = [%r] \r\n",__FUNCTION__, i2c_sts));
    Status = EFI_DEVICE_ERROR;
  }

  return Status;
}

EFI_STATUS SmartChargerLibFG_GetBatteryPFActive(BOOLEAN *bPFActive)
{
    EFI_STATUS Status = EFI_SUCCESS;
    i2c_status i2c_sts = I2C_SUCCESS;
    UINT32 data = 0;

    if (bPFActive == NULL)
    {
        return EFI_INVALID_PARAMETER;
    }

    *bPFActive = 0;
    i2c_sts = fg_i2c_read(SBS_CMD_ManufacturerAccess, (UINT8*)&data, SBS_DATA_SIZE);
    if(I2C_SUCCESS != i2c_sts)
    {
        CHARGER_DEBUG((EFI_D_WARN, "SmartChargerLibFG:: %a SmartChargerLibFG_GetBatteryPFActive read error = [%r] \r\n", __FUNCTION__, i2c_sts));
        return EFI_DEVICE_ERROR;
    }

    *bPFActive = (data & FG_PERMANENT_FAILURE_MODE_STS_MASK) ? TRUE : FALSE;

    return Status;
}

EFI_STATUS SmartChargerLibFG_GetBatteryPFStatus(UINT32 *pPFStatus)
{
    EFI_STATUS Status = EFI_SUCCESS;
    i2c_status i2c_sts = I2C_SUCCESS;
    UINT16 addr = SBS_CMD_PFStatus;

    if (pPFStatus == NULL)
    {
        return EFI_INVALID_PARAMETER;
    }

    i2c_sts = fg_i2c_write(SBS_CMD_ManufacturerAccess, (UINT8*)&addr, SBS_DATA_SIZE);
    i2c_sts |= fg_i2c_read(SBS_CMD_ManufacturerData, (UINT8*)pPFStatus, SBS_PF_STATUS_DATA_SIZE);
    if(I2C_SUCCESS != i2c_sts)
    {
        CHARGER_DEBUG((EFI_D_WARN, "SmartChargerLibFG:: %a  FG SBS_CMD_PFStatus read error = [%r] \r\n", __FUNCTION__, i2c_sts));
        Status = EFI_DEVICE_ERROR;
    }

    return Status;
}

/**
 * I2C Read/Write Interface for FG Module
 */
i2c_status fg_i2c_read(UINT8 offset, UINT8 *data_out, UINT16 size_out)
{
    i2c_status i2c_sts = I2C_TRANSFER_INVALID;
    UINT32 read_cnt = 0;
    UINT32 read_retry = FG_READ_RETRY_COUNT;

    if (!gFGI2cInitialized)
    {
      SmartChargerLibFG_Init();
    }
    if(gFGDevice.hI2CFGHandle)
    {
        while (read_retry)
        {
            i2c_sts = i2c_read(gFGDevice.hI2CFGHandle,
                                &gFGDevice.I2CFGCfg,
                                (uint16)offset,
                                SBS_CMD_SIZE,
                                data_out,
                                size_out,
                                &read_cnt,
                                gFGDevice.TimeoutMs);
            if (i2c_sts != I2C_SUCCESS)
            {
                read_retry--;
                CHARGER_DEBUG((EFI_D_WARN, "SmartChargerLibFG:: %a, i2c_sts : [%r], read_retry = %d \r\n",__FUNCTION__, i2c_sts, read_retry));
            }
            else
            {
                break;
            }
        }
    }
    else
    {
        CHARGER_DEBUG((EFI_D_WARN, "SmartChargerLibFG:: %a, Invalid FG handle \r\n",__FUNCTION__));
        i2c_sts = I2C_ERROR_INVALID_PARAMETER;
    }
    return i2c_sts;
}

i2c_status fg_i2c_write(UINT8 offset, UINT8* data_in, UINT16 size_in)
{
    i2c_status i2c_sts = I2C_TRANSFER_INVALID;
    UINT32 write_cnt = 0;
    UINT32 write_retry = FG_WRITE_RETRY_COUNT;

    if(gFGDevice.hI2CFGHandle)
    {
        while (write_retry)
        {
            i2c_sts = i2c_write(gFGDevice.hI2CFGHandle,
                                &gFGDevice.I2CFGCfg,
                                (uint16)offset,
                                SBS_CMD_SIZE,
                                data_in,
                                size_in,
                                &write_cnt,
                                gFGDevice.TimeoutMs);
            if (i2c_sts != I2C_SUCCESS)
            {
                write_retry--;
                CHARGER_DEBUG((EFI_D_WARN, "SmartChargerLibFG:: %a, i2c_sts : [%r], write_retry = %d \r\n",__FUNCTION__, i2c_sts, write_retry));
            }
            else
            {
                break;
            }
        }
    }
    else
    {
        CHARGER_DEBUG((EFI_D_WARN, "SmartChargerLibFG:: %a, Invalid FG handle \r\n",__FUNCTION__));
        i2c_sts = I2C_ERROR_INVALID_PARAMETER;
    }
    return i2c_sts;
}

