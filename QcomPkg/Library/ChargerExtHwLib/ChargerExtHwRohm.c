/**
@file  ChargerExtHwRohm.c
@brief Implementation of ChargerExtHwLib API for Rohm Charger module
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
05/15/18   pbitra  Setting ROHM termination current to 100mA
02/01/18   mr      Added Charger Sink enable API
01/22/18   pbitra  Manually checking BC1.2 charger type and configuring ICL to 1.5A if CDP or DCP
12/13/17   dc      Enable Vbus Anti-Collapsing
12/11/17   dc      Fix incorrect Charge Current Cap
12/11/17   dc      Enable HW Input Current Limit update based on BC1.2 Detection
10/26/17   pbitra  File cleanup, removed APIs for checking if battery is present and
                   getting battery status from ROHM
10/16/17   pbitra  Added APIs for charging functionality
08/10/17   mr      Initial Release
=============================================================================*/

/*===========================================================================*/
/*                  INCLUDE FILES                                            */
/*===========================================================================*/
/**
  Internal Dependencies
*/
#include "ChargerExtHwLibConfig.h"
#include "RohmBD99954.h"

/**
  Protocol Dependencies
*/
#include <Protocol/EFIUsbPwrCtrl.h>


/*===========================================================================*/
/*                  TYPEDEF AND MACRO DEFINITIONS                            */
/*===========================================================================*/

#define EXTENDED_COMMAND_SETTING 1
#define VSYSREG_SET 7200  /* Voltage level to change from Precharge to Fastcharge state */
#define VBUSCLPS_TH_SET 4096 /* Vbus Collapse Detect Threshold */
#define MAX_1500_MA                         1500

/*===========================================================================*/
/*                  FUNCTIONS PROTOTYPES                                     */
/*===========================================================================*/
EFI_STATUS Rohm_PreInit(CHARGER_EXT_HW_EXT_HW_DEP *RohmExtDep);

/* Local API to 'write with mask' for Rohm Charger */
i2c_status Rohm_i2c_write_mask(UINT8 offset, UINT16 mask, UINT16 data_in, UINT16 size_in);

/* Rohm Charger Functionalities */
EFI_STATUS Rohm_GetVbusDetect(BOOLEAN *bVbusDetect);
EFI_STATUS Rohm_GetHSUsbPortType(CHARGER_EXT_HW_HSUSB_CHGPORT_TYPE *ChgPortType);
EFI_STATUS Rohm_SetSnkMaxInputCurrent(UINT32 MaxCurrentMA);
EFI_STATUS Rohm_EnableCharger(BOOLEAN Enable);
EFI_STATUS Rohm_GetChargingStatus(BOOLEAN *pChargingSts);
EFI_STATUS Rohm_SetChargingTermCurrent(UINT32 ChargingTermCurrent);
EFI_STATUS Rohm_SetFCCMax(UINT32 FCCMaxCurrent);
EFI_STATUS Rohm_SetFVMax(UINT32 FVMax);
EFI_STATUS Rohm_SetSinkEn(BOOLEAN bSinkEn);


/*===========================================================================*/
/*                  LOCAL VARIABLE DECLARATIONS                              */
/*===========================================================================*/
static ChargerExtHw_Device gRohmDevice = {0};

CHARGER_EXT_HW_FUNC gRohmFunc =
{
    Rohm_GetVbusDetect,
    Rohm_GetHSUsbPortType,
    Rohm_SetSnkMaxInputCurrent,
    Rohm_EnableCharger,
    Rohm_GetChargingStatus,
    Rohm_SetChargingTermCurrent,
    Rohm_SetFCCMax,
    Rohm_SetFVMax,
    Rohm_SetSinkEn
};


/*===========================================================================*/
/*                  FUNCTIONS DEFINITIONS                                    */
/*===========================================================================*/
/**
  Initialize the interface to External Rohm Charger module

  @param none

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
  EFI_UNSUPPORTED:              The physical device does not support this function
*/
EFI_STATUS
ChargerExtHwLibRohm_Init(CHARGER_EXT_HW_INFO *pHwInfo)
{
    EFI_STATUS Status = EFI_SUCCESS;
    i2c_status i2c_sts = I2C_SUCCESS;
    UINT16 rohm_sel_cmd_set = 0;
    UINT16 data = 0;
    CHARGER_EXT_HW_HSUSB_CHGPORT_TYPE ChgPortType;

    Status = Rohm_PreInit(pHwInfo->ChgHwExtDep);
    if(EFI_ERROR(Status)) {return Status;}

    gRohmDevice.hI2CHandle = NULL;
    gRohmDevice.I2CInstance = pHwInfo->ChgrExtHwIntf.Index;

    gRohmDevice.I2CSlaveCfg.bus_frequency_khz = 100;  /* 100 KHz */
    gRohmDevice.I2CSlaveCfg.slave_address = pHwInfo->ChgrExtHwIntf.Address;
    gRohmDevice.I2CSlaveCfg.mode = I2C;
    gRohmDevice.I2CSlaveCfg.slave_max_clock_stretch_us = 0;
    gRohmDevice.I2CSlaveCfg.core_configuration1 = 0;
    gRohmDevice.I2CSlaveCfg.core_configuration2 = 0;

    gRohmDevice.I2CSlaveRegMap = ROHM_CMD_SIZE;

    gRohmDevice.TimeoutMs = 0;

    i2c_sts = i2c_open(gRohmDevice.I2CInstance, &gRohmDevice.hI2CHandle);
    if(I2C_SUCCESS != i2c_sts)
    {
        CHARGEREXTHW_LIB_DEBUG((EFI_D_ERROR, "%a i2c_open Failed \r\n", __FUNCTION__));
        return EFI_DEVICE_ERROR;
    }

    /* Detect H/w revision/version and do specific initialization */
    pHwInfo->Func = &gRohmFunc;

    /* Charger Init sequence at Boot-up */
    rohm_sel_cmd_set = EXTENDED_COMMAND_SETTING;  /* Select EXTENDED Command Set */
    i2c_sts = ChargerExtHw_i2c_write(&gRohmDevice, ROHM_CMD_SEL_MAP_SET, (UINT8 *)&rohm_sel_cmd_set, ROHM_DATA_SIZE);
    if(I2C_SUCCESS != i2c_sts)
    {
        CHARGEREXTHW_LIB_DEBUG((EFI_D_ERROR, "%a CMD_SEL_MAP set Failed \r\n", __FUNCTION__));
        return EFI_DEVICE_ERROR;
    }
    
    //Enable VBUS Anti-collapse
    data = VBUSCLPS_TH_SET;
    i2c_sts = ChargerExtHw_i2c_write(&gRohmDevice, ROHM_EXT_VBUSCLPS_TH_SET, (UINT8 *)&data, ROHM_DATA_SIZE);
    if(I2C_SUCCESS != i2c_sts)
    {
        CHARGEREXTHW_LIB_DEBUG((EFI_D_ERROR, "%a ROHM_EXT_VBUSCLPS_TH_SET set Failed \r\n", __FUNCTION__));
        return EFI_DEVICE_ERROR;
    }

    //Manually checking BC1.2 charger type and configuring ICL to 1.5A if CDP or DCP
    Status |= Rohm_GetHSUsbPortType(&ChgPortType);
    if((EFI_SUCCESS == Status) &&
	    ((ChgPortType == EFI_USB_PWR_CTRL_HSUSB_CHGPORT_CDP)||
			(ChgPortType == EFI_USB_PWR_CTRL_HSUSB_CHGPORT_DCP )))
    {
    	CHARGEREXTHW_LIB_DEBUG((EFI_D_WARN, "%a ChgPortType = %d, Setting ICL to 1.5A after manually checking BC 1.2 charger type \r\n", __FUNCTION__,ChgPortType));		
    	Status |= Rohm_SetSnkMaxInputCurrent(MAX_1500_MA);
    }

    data = (CHGWDT_FST_MASK | CHGWDT_PRE_MASK) ;
    i2c_sts = ChargerExtHw_i2c_write(&gRohmDevice, ROHM_EXT_CMD_CHGWDT_SET, (UINT8 *)&data, ROHM_DATA_SIZE);
    if(I2C_SUCCESS != i2c_sts)
    {
        CHARGEREXTHW_LIB_DEBUG((EFI_D_ERROR, "%a ROHM_EXT_CMD_CHGWDT_SET set Failed \r\n", __FUNCTION__));
        return EFI_DEVICE_ERROR;
    }

    data = IPRECH_LIM_MAX;
	i2c_sts = ChargerExtHw_i2c_write(&gRohmDevice, ROHM_EXT_CMD_IPRECH_SET, (UINT8 *)&data, ROHM_DATA_SIZE);
    if(I2C_SUCCESS != i2c_sts)
    {
        CHARGEREXTHW_LIB_DEBUG((EFI_D_ERROR, "%a ROHM_EXT_CMD_IPRECH_SET set Failed \r\n", __FUNCTION__));
        return EFI_DEVICE_ERROR;
    }

    data = VSYSREG_SET;
    i2c_sts = ChargerExtHw_i2c_write(&gRohmDevice, ROHM_EXT_CMD_VSYSREG_SET, (UINT8 *)&data, ROHM_DATA_SIZE);
    if(I2C_SUCCESS != i2c_sts)
    {
        CHARGEREXTHW_LIB_DEBUG((EFI_D_ERROR, "%a ROHM_EXT_CMD_VSYSREG_SET set Failed \r\n", __FUNCTION__));
        return EFI_DEVICE_ERROR;
    }

	i2c_sts = ChargerExtHw_i2c_read(&gRohmDevice, ROHM_EXT_CMD_ITERM_SET, (UINT8 *)&data, ROHM_DATA_SIZE);  
    if(I2C_SUCCESS != i2c_sts)
    {
        CHARGEREXTHW_LIB_DEBUG((EFI_D_ERROR, "%a ROHM_EXT_CMD_ITERM_SET read Failed \r\n", __FUNCTION__));
        return EFI_DEVICE_ERROR;
    }

    DEBUG((EFI_D_WARN, "%a ROHM_EXT_CMD_ITERM_SET : 0x%04x \r\n", __FUNCTION__, data));


    data = 100;	
	i2c_sts = ChargerExtHw_i2c_write(&gRohmDevice, ROHM_EXT_CMD_ITERM_SET, (UINT8 *)&data, ROHM_DATA_SIZE);  
    if(I2C_SUCCESS != i2c_sts)
    {
        CHARGEREXTHW_LIB_DEBUG((EFI_D_ERROR, "%a ROHM_EXT_CMD_ITERM_SET set Failed \r\n", __FUNCTION__));
        return EFI_DEVICE_ERROR;
    }

    CHARGEREXTHW_LIB_DEBUG((EFI_D_WARN, "%a SUCCESS \r\n", __FUNCTION__));

    return Status;
}

/**
  De-initialize the interface to Rohm Charger module

  @param none

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
*/
EFI_STATUS
ChargerExtHwLibRohm_Deinit()
{
    EFI_STATUS Status = EFI_SUCCESS;
    i2c_status i2c_sts = I2C_SUCCESS;

    i2c_sts = i2c_close(gRohmDevice.hI2CHandle);
    if(I2C_SUCCESS != i2c_sts)
    {
        Status = EFI_DEVICE_ERROR;
    }

    return Status;
}

/**
  Get Vbus Detect result from Rohm Charger Module

  @param[OUT]   bVbusDetect     Return the result of Vbus Detect

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
*/
EFI_STATUS Rohm_GetVbusDetect(BOOLEAN *bVbusDetect)
{
    EFI_STATUS Status = EFI_SUCCESS;
    i2c_status i2c_sts = I2C_SUCCESS;
    UINT16 data = 0;

    if (bVbusDetect == NULL)
    {
        return EFI_INVALID_PARAMETER;
    }

    /* VBus detect status here */
    i2c_sts = ChargerExtHw_i2c_read(&gRohmDevice, ROHM_EXT_CMD_VBUS_VCC_STATUS, (UINT8 *)&data, ROHM_DATA_SIZE);
    if(I2C_SUCCESS != i2c_sts) {return EFI_DEVICE_ERROR;}

    *bVbusDetect = (VBUS_VCC_STS_VBUS_DET & data) ? TRUE : FALSE;
    /* For VBus valid, Check VBUS_UCD_STATUS <7>VBUS_VLD */

    return Status;
}

/**
  Get HSUSB Charger Port Type

  @param[OUT]   ChgPortType     Return Charger Port Type

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
*/
EFI_STATUS Rohm_GetHSUsbPortType(CHARGER_EXT_HW_HSUSB_CHGPORT_TYPE *ChgPortType)
{
    EFI_STATUS Status = EFI_SUCCESS;
    i2c_status i2c_sts = I2C_SUCCESS;
    UINT16 data = 0;

    if (ChgPortType == NULL)
    {
        return EFI_INVALID_PARAMETER;
    }

    i2c_sts = ChargerExtHw_i2c_read(&gRohmDevice, ROHM_EXT_CMD_VBUS_UCD_STATUS, (UINT8 *)&data, ROHM_DATA_SIZE);
    if(I2C_SUCCESS != i2c_sts) {return EFI_DEVICE_ERROR;}

    *ChgPortType = CHARGER_EXT_HW_HSUSB_CHGPORT_UNKNOWN;

    if(VBUS_UCD_STS_PORT_SDP == (data & VBUS_UCD_STS_PORT_DET_MASK))
    {
        *ChgPortType = CHARGER_EXT_HW_HSUSB_CHGPORT_SDP;
    }
    else if(VBUS_UCD_STS_PORT_CDP == (data & VBUS_UCD_STS_PORT_DET_MASK))
    {
        *ChgPortType = CHARGER_EXT_HW_HSUSB_CHGPORT_CDP;
    }
    else if(VBUS_UCD_STS_PORT_DCP == (data & VBUS_UCD_STS_PORT_DET_MASK))
    {
        *ChgPortType = CHARGER_EXT_HW_HSUSB_CHGPORT_DCP;
    }
    else if(VBUS_UCD_STS_PORT_PULLUP == (data & VBUS_UCD_STS_PORT_DET_MASK))
    {
        *ChgPortType = VBUS_UCD_STS_PORT_PULLUP;
    }
    else if(VBUS_UCD_STS_PORT_OPEN == (data & VBUS_UCD_STS_PORT_DET_MASK))
    {
        *ChgPortType = VBUS_UCD_STS_PORT_OPEN;
    }

    return Status;
}

/**
  Set Sink Max Input Current

  @param[IN]    MaxCurrentMA    Value to set Max Input Current

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
*/
EFI_STATUS Rohm_SetSnkMaxInputCurrent(UINT32 MaxCurrentMA)
{
    EFI_STATUS Status = EFI_SUCCESS;
    i2c_status i2c_sts = I2C_SUCCESS;
    UINT16 data = 0;

    if(MaxCurrentMA > IBUS_LIM_MAX)
        MaxCurrentMA = IBUS_LIM_MAX;

    data  = (MaxCurrentMA + (IBUS_LIM_STEP/2));
    data &= IBUS_LIM_SET_MASK;

    i2c_sts = ChargerExtHw_i2c_write(&gRohmDevice, ROHM_EXT_CMD_IBUS_LIM_SET, (UINT8 *)&data, ROHM_DATA_SIZE);
    if(I2C_SUCCESS != i2c_sts) {return EFI_DEVICE_ERROR;}

    return Status;
}

/**
  Set Source Max Output Current

  @param[IN]    MaxCurrentMA    Value to set Max Output Current

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
*/
EFI_STATUS Rohm_SetSrcMaxOutputCurrent(UINT32 MaxCurrentMA)
{
  EFI_STATUS Status = EFI_SUCCESS;

  /* Set Source max current here */

  return Status;
}

/**
  Enable/Disable Vbus

  @param[IN]    bVbusEn         Enable/Disable Vbus

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
*/
EFI_STATUS Rohm_SetVbusEn(UINT32 bVbusEn)
{
  EFI_STATUS Status = EFI_SUCCESS;

  /* Set VBus enable here */

  return Status;
}

/**
  Enable/Disable Charger

  @param[IN]    bEnable         Enable/Disable charger

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
*/
EFI_STATUS Rohm_EnableCharger(BOOLEAN bEnable)
{
  i2c_status i2c_sts = I2C_SUCCESS;
  UINT16 data = ((UINT16)bEnable << CHGOP_SET2_CHG_EN_SHIFT);
  
  i2c_sts = Rohm_i2c_write_mask(ROHM_EXT_CMD_CHGOP_SET2, CHGOP_SET2_CHG_EN_MASK, data, ROHM_DATA_SIZE);
  if(I2C_SUCCESS != i2c_sts) 
  	{
	  CHARGEREXTHW_LIB_DEBUG((EFI_D_WARN, "%a Failed writing \r\n", __FUNCTION__));
		return EFI_DEVICE_ERROR;
		}
  return EFI_SUCCESS;
}


/**
  Get Charging Status

  @param[OUT]    pChargingSts     TRUE if charging is on-going
  								  FALSE if not charging

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
*/
EFI_STATUS Rohm_GetChargingStatus(BOOLEAN *pChargingSts)
{
  EFI_STATUS Status = EFI_SUCCESS;  
  i2c_status i2c_sts = I2C_SUCCESS;
  UINT16 data = 0;

  i2c_sts = ChargerExtHw_i2c_read(&gRohmDevice, ROHM_EXT_CMD_CHGSTM_STATUS, (UINT8 *)&data, ROHM_DATA_SIZE);
  if(I2C_SUCCESS != i2c_sts) {return EFI_DEVICE_ERROR;}

  *pChargingSts = ((CHGSTM_STATUS_TRICKLE_MASK | CHGSTM_STATUS_PRECHARGE_MASK | CHGSTM_STATUS_FAST_MASK | 
  					CHGSTM_STATUS_TOPOFF_MASK) & data) ? TRUE : FALSE;
  return Status;
}
/**
  Set Charging terminating current

  @param[IN]    ChargingTermCurrent    

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
  EFI_UNSUPPORTED:              The physical device does not support this function
*/

EFI_STATUS Rohm_SetChargingTermCurrent(UINT32 ChargingTermCurrent)
{
  EFI_STATUS Status = EFI_SUCCESS;  
  i2c_status i2c_sts = I2C_SUCCESS;
  UINT16 data;

  
  if(ChargingTermCurrent > ITERM_LIM_MAX)
  {
	  ChargingTermCurrent = ITERM_LIM_MAX;
  }
  data = (UINT16)ChargingTermCurrent;

  i2c_sts = ChargerExtHw_i2c_write(&gRohmDevice, ROHM_EXT_CMD_ITERM_SET, (UINT8 *)&data, ROHM_DATA_SIZE);  
  if(I2C_SUCCESS != i2c_sts) {return EFI_DEVICE_ERROR;}

  return Status;
}


/**
  Set Max Fast Charge Current

  @param[IN]    FCCMaxCurrent    

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
  EFI_UNSUPPORTED:              The physical device does not support this function
*/

EFI_STATUS Rohm_SetFCCMax(UINT32 FCCMaxCurrent)
{
  EFI_STATUS Status = EFI_SUCCESS;  
  i2c_status i2c_sts = I2C_SUCCESS;
  UINT16 data;

  
  if(FCCMaxCurrent > ICHG_LIM_MAX)
  {
	  FCCMaxCurrent = ICHG_LIM_MAX;
  }
  data = (UINT16)FCCMaxCurrent;

  i2c_sts = ChargerExtHw_i2c_write(&gRohmDevice, ROHM_EXT_CMD_ICHG_SET, (UINT8 *)&data, ROHM_DATA_SIZE);  
  if(I2C_SUCCESS != i2c_sts) {return EFI_DEVICE_ERROR;}

  return Status;
}

/**
  Set Float Voltage Max

  @param[IN]    FVMax    

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
  EFI_UNSUPPORTED:              The physical device does not support this function
*/

EFI_STATUS Rohm_SetFVMax(UINT32 FVMax)
{
  EFI_STATUS Status = EFI_SUCCESS;  
  i2c_status i2c_sts = I2C_SUCCESS;
  UINT16 data;

  
  if(FVMax > FV_LIM_MAX)
  {
	  FVMax = FV_LIM_MAX;
  }
  else if(FVMax < FV_LIM_MIN)
  {
	  FVMax = FV_LIM_MIN;
  }
  data = (UINT16)FVMax;

  i2c_sts = ChargerExtHw_i2c_write(&gRohmDevice, ROHM_EXT_CMD_VFASTCHG_REG_SET1, (UINT8 *)&data, ROHM_DATA_SIZE);  
  if(I2C_SUCCESS != i2c_sts) {return EFI_DEVICE_ERROR;}

  return Status;
}

/**
  Set Charger Sink Enable

  @param[IN]    bSinkEn

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
  EFI_UNSUPPORTED:              The physical device does not support this function
*/
EFI_STATUS Rohm_SetSinkEn(BOOLEAN bSinkEn)
{
  EFI_STATUS Status = EFI_SUCCESS;
  i2c_status i2c_sts = I2C_SUCCESS;
  UINT16 data = 0;

  i2c_sts = ChargerExtHw_i2c_read(&gRohmDevice, ROHM_VIN_CTRL_SET, (UINT8 *)&data, ROHM_DATA_SIZE);

  data = (bSinkEn) ? VBUS_EN_MASK : 0;

  i2c_sts = Rohm_i2c_write_mask(ROHM_VIN_CTRL_SET, VBUS_EN_MASK, data, ROHM_DATA_SIZE);
  if(I2C_SUCCESS != i2c_sts) {return EFI_DEVICE_ERROR;}

  return Status;
}


/*===========================================================================*/
/*                  INTERNAL FUNCTIONS DEFINITIONS                           */
/*===========================================================================*/
EFI_STATUS Rohm_PreInit(CHARGER_EXT_HW_EXT_HW_DEP *RohmExtDep)
{
    EFI_STATUS Status = EFI_SUCCESS;
    UINT8 NumDep = 0;

    if(NULL == RohmExtDep) {return EFI_INVALID_PARAMETER;}

    for(NumDep = 0; NumDep < CHARGER_EXT_HW_EXT_HW_DEP_MAX; NumDep++)
    {
        if(RohmExtDep[NumDep].InitState >= CHARGER_EXT_HW_EXT_HW_DEP_STATE_INVALID)
        {
            continue;
        }
        if(RohmExtDep[NumDep].InitState == CHARGER_EXT_HW_EXT_HW_DEP_STATE_NONE)
            continue;

        Status = ChargerExtHw_Gpio_Config(RohmExtDep[NumDep].ExtDepResource, RohmExtDep[NumDep].ResourceID, RohmExtDep[NumDep].InitState);
        if(EFI_ERROR(Status)) break;
    }

    return Status;
}

i2c_status Rohm_i2c_write_mask(UINT8 offset, UINT16 mask, UINT16 data_in, UINT16 size_in)
{
    i2c_status i2c_sts = I2C_TRANSFER_INVALID;
    UINT16 data = 0;

    i2c_sts = ChargerExtHw_i2c_read(&gRohmDevice, offset, (UINT8 *)&data, sizeof(data));
    if(I2C_SUCCESS != i2c_sts) {return i2c_sts;}

    data &= (~mask);
    data |= (data_in & mask);

    i2c_sts = ChargerExtHw_i2c_write(&gRohmDevice, offset, (UINT8 *)&data, sizeof(data));

    return i2c_sts;
}
