/**
@file  SmartChargerLibFG.c
@brief Implementation of SmartChargerLib API for FG module
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
05/15/18   pbitra    Change from reading Absolute SOC to Relative SOC
03/27/18   pbitra    Added Temperature reading support
11/09/17   pbitra    File Cleanup
08/04/17   pbitra      Initial Release
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
#include "SmartChargerLibFG.h"
#include <api/pmic/charger/ChargerLibCommon.h>

/*===========================================================================*/
/*                  TYPEDEF AND MACRO DEFINITIONS                            */
/*===========================================================================*/

#define I2C_SLAVE_ID_BATTERYPACK   0x0B    /* SlaveID for bq30z55*/
#define I2C_INSTANCE_BATTERYPACK   I2C_INSTANCE_013

/**
  SBS Commands
*/
#define SBS_CMD_BatteryMode		0x03
#define SBS_CMD_RelativeStateOfCharge	0x0D
#define SBS_CMD_Current			0x0A
#define SBS_CMD_Temperatures	0x08
#define SBS_CMD_Voltage			0x09
#define SBS_CMD_ChargingCurrent	0x14
#define SBS_CMD_ChargingVoltage	0x15
#define SBS_CMD_BatteryStatus	0x16
#define SBS_CMD_OperationStatus	0x54
#define SBS_CMD_ChargingStatus	0x55


#define SBS_DATA_SIZE 2
#define SBS_CMD_SIZE 1

#define FG_READ_RETRY_COUNT 3


#define TEMPP1K_TO_TEMPC(x) ((x-2730)/10)

/*===========================================================================*/
/*                  FUNCTIONS PROTOTYPES                                     */
/*===========================================================================*/
i2c_status fg_i2c_read(UINT8 offset, UINT8 *data_out, UINT16 size_out);

/*===========================================================================*/
/*                  LOCAL VARIABLE DECLARATIONS                              */
/*===========================================================================*/
SmartChargerFG_SlaveType gFGDevice = {0};

FG_PORTHW_HW_INFO HwInfo = {I2C_INSTANCE_BATTERYPACK, I2C_SLAVE_ID_BATTERYPACK};

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

    i2c_sts = i2c_open(gFGDevice.I2CFGInstance, &gFGDevice.hI2CFGHandle);
    if(I2C_SUCCESS != i2c_sts)
    {
        return EFI_DEVICE_ERROR;
    }
	SMARTCHARGER_DEBUG((EFI_D_WARN, "SmartChargerLibFG:: %a, FG handle opened = %r \r\n",__FUNCTION__, i2c_sts));

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

	if (gFGDevice.hI2CFGHandle != NULL)
	{
		i2c_sts = i2c_close(gFGDevice.hI2CFGHandle);
		if(I2C_SUCCESS == i2c_sts)
		{
			Status = EFI_SUCCESS;
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
    EFI_STATUS Status = EFI_SUCCESS;
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
    	SMARTCHARGER_DEBUG((EFI_D_WARN, "SmartChargerLibFG:: %a	FG SBS_CMD_RelativeStateOfCharge read error = [%r] \r\n",__FUNCTION__, i2c_sts)); 
		return EFI_DEVICE_ERROR;
	}
    pBatteryStatus->StateOfCharge = data;

	data = 0;
	i2c_sts = fg_i2c_read(SBS_CMD_Current, (UINT8 *)&data, SBS_DATA_SIZE);
    if(I2C_SUCCESS != i2c_sts) 
	{
		SMARTCHARGER_DEBUG((EFI_D_WARN, "SmartChargerLibFG:: %a	FG BatteryChargeCurrent read error = [%r] \r\n",__FUNCTION__, i2c_sts)); 
		return EFI_DEVICE_ERROR;
	}	
	current = (INT16)data;
	pBatteryStatus->ChargeCurrent= data;

	data = 0;
	i2c_sts = fg_i2c_read(SBS_CMD_Voltage, (UINT8 *)&data, SBS_DATA_SIZE);
    if(I2C_SUCCESS != i2c_sts) 
	{
		SMARTCHARGER_DEBUG((EFI_D_WARN, "SmartChargerLibFG:: %a	FG SBS_CMD_Voltage read error = [%r] \r\n",__FUNCTION__, i2c_sts)); 
		return EFI_DEVICE_ERROR;
	}
     pBatteryStatus->BatteryVoltage = data;


	data = 0;
	i2c_sts = fg_i2c_read(SBS_CMD_Temperatures, (UINT8 *)&data, SBS_DATA_SIZE);
    if(I2C_SUCCESS != i2c_sts) 
	{
		SMARTCHARGER_DEBUG((EFI_D_WARN, "SmartChargerLibFG:: %a	FG SBS_CMD_Temperatures read error = [%r] \r\n",__FUNCTION__, i2c_sts)); 
		return EFI_DEVICE_ERROR;
	}
     pBatteryStatus->BatteryTemperature = TEMPP1K_TO_TEMPC(data); 

	

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
				SMARTCHARGER_DEBUG((EFI_D_WARN, "SmartChargerLibFG:: %a, i2c_sts : [%r], read_retry = %d \r\n",__FUNCTION__, i2c_sts, read_retry));
			}
			else
			{
				break;
			}
		}
    }
    else
	{
		SMARTCHARGER_DEBUG((EFI_D_WARN, "SmartChargerLibFG:: %a, Invalid FG handle \r\n",__FUNCTION__));
	    i2c_sts = I2C_ERROR_INVALID_PARAMETER;
	}
    return i2c_sts;
}
