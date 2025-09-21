/*! \file
*  
*  \brief  pm_app_smartchgr.c
*  \details Implementation file for pmic sbl charging algorithm of external changer
*    
*  \n &copy; Copyright (c)2018  by QUALCOMM Technologies Incorporated, All Rights Reserved
*/

/*===========================================================================
                                Edit History
when       who     what, where, why
--------   ---     ----------------------------------------------------------
04/23/18   sv      Enable VBUS on ROHM charger during every boot up (CR-2214814).
02/08/18   sv      Initial revision
===========================================================================*/

/*===========================================================================
                     INCLUDE FILES 
===========================================================================*/
#include "pm_app_smartchgr.h"
#include "pm_log_utils.h"
#include "pm_pon.h"
/*===========================================================================*/
/*                        GLOBALS and TYPE DEFINITIONS                       */
/*===========================================================================*/

#define EXTENDED_COMMAND_SETTING 1
#define VSYSREG_SET 7200  /* Voltage level to change from Precharge to Fastcharge state */
#define VBUSCLPS_TH_SET 4096 /* Vbus Collapse Detect Threshold */
#define MAX_1500_MA                         1500
#define PM_WEAK_BATTERY_CHARGING_DELAY         500 * 1000   //500ms


static SmartChargerFG_SlaveType gFGDevice = {0};
FG_PORTHW_HW_INFO FgHwInfo = {I2C_INSTANCE_BATTERYPACK, I2C_SLAVE_ID_BATTERYPACK};

static ChargerExtHw_Device gRohmDevice = {0};
EXTCHG_PORTHW_HW_INFO ChargerExtHwInfo = {I2C_INSTANCE_ROHM_CHARGER, ROHM_CHARGER_I2C_SLAVE_ID};

/*===========================================================================
                     LOCAL FUNCTION DECLARATIONS
===========================================================================*/
static pm_err_flag_type SmartChargerLibFG_Init(void);
static pm_err_flag_type SmartChargerLibFG_Deinit(void);
static pm_err_flag_type SmartChargerLibFG_GetBatteryStatus(SmartChargerFG_BattStatusInfo *pBatteryStatus);
static i2c_status fg_i2c_read(UINT8 offset, UINT8 *data_out, UINT16 size_out);

static pm_err_flag_type ChargerExtHwLibRohm_Init(void);
static pm_err_flag_type Rohm_GetChargingStatus(BOOLEAN *pChargingSts);
static pm_err_flag_type Rohm_GetHSUsbPortType(CHARGER_EXT_HW_HSUSB_CHGPORT_TYPE *ChgPortType);
static pm_err_flag_type Rohm_SetSnkMaxInputCurrent(UINT32 MaxCurrentMA);
static pm_err_flag_type Rohm_GetVbusDetect(BOOLEAN *bVbusDetect);
static pm_err_flag_type ChargerExtHwLibRohm_Deinit(void);
static pm_err_flag_type Rohm_EnableCharger(BOOLEAN bEnable);
static i2c_status ChargerExtHw_i2c_read(ChargerExtHw_Device *I2CDev, UINT8 offset, UINT8 *data_out, UINT16 size_out);
static i2c_status ChargerExtHw_i2c_write(ChargerExtHw_Device *I2CDev, UINT8 offset, UINT8 *data_in, UINT16 size_in);
static i2c_status Rohm_i2c_write_mask(UINT8 offset, UINT16 mask, UINT16 data_in, UINT16 size_in);

/*===========================================================================
                     FUNCTION DEFINITIONS 
===========================================================================*/

static pm_err_flag_type ChargerExtHwLibRohm_Init()
{
    pm_err_flag_type Status = PM_ERR_FLAG_SUCCESS;
    i2c_status i2c_sts = I2C_SUCCESS;
    UINT16 rohm_sel_cmd_set = 0;
    UINT16 data = 0;
    CHARGER_EXT_HW_HSUSB_CHGPORT_TYPE ChgPortType;
          
    //Status = Rohm_PreInit(pHwInfo->ChgHwExtDep);   -----> what needs to be done here..
    //if(EFI_ERROR(Status)) {return Status;}

    gRohmDevice.hI2CHandle = NULL;
    gRohmDevice.I2CInstance = ChargerExtHwInfo.Index;

    gRohmDevice.I2CSlaveCfg.bus_frequency_khz = 100;  /* 100 KHz */
    gRohmDevice.I2CSlaveCfg.slave_address = ChargerExtHwInfo.Address;
    gRohmDevice.I2CSlaveCfg.mode = I2C;
    gRohmDevice.I2CSlaveCfg.slave_max_clock_stretch_us = 0;
    gRohmDevice.I2CSlaveCfg.core_configuration1 = 0;
    gRohmDevice.I2CSlaveCfg.core_configuration2 = 0;

    gRohmDevice.I2CSlaveRegMap = ROHM_CMD_SIZE;

    gRohmDevice.TimeoutMs = 0;

    i2c_sts = i2c_open(gRohmDevice.I2CInstance, &gRohmDevice.hI2CHandle);
    if(I2C_SUCCESS != i2c_sts)
    {
        pm_log_message("ChargerExtHwLibRohm_Init: i2c_open Failed.. ");
        return PM_ERR_FLAG_BUS_ERR;
    }

    /* Charger Init sequence at Boot-up */
    rohm_sel_cmd_set = EXTENDED_COMMAND_SETTING;  /* Select EXTENDED Command Set */
    i2c_sts = ChargerExtHw_i2c_write(&gRohmDevice, ROHM_CMD_SEL_MAP_SET, (UINT8 *)&rohm_sel_cmd_set, ROHM_DATA_SIZE);
    if(I2C_SUCCESS != i2c_sts)
    {
        pm_log_message("ChargerExtHwLibRohm_Init: CMD_SEL_MAP set Failed..");
        return PM_ERR_FLAG_BUS_ERR;
    }
    
    //Enable VBUS
    data = VBUS_EN_MASK;
    i2c_sts = Rohm_i2c_write_mask(ROHM_VIN_CTRL_SET, VBUS_EN_MASK, data, ROHM_DATA_SIZE);
    if(I2C_SUCCESS != i2c_sts)
    {
        pm_log_message("ChargerExtHwLibRohm_Init: ROHM_VIN_CTRL_SET set Failed..");
        return PM_ERR_FLAG_BUS_ERR;
    }
    
    //Enable VBUS Anti-collapse
    data = VBUSCLPS_TH_SET;
    i2c_sts = ChargerExtHw_i2c_write(&gRohmDevice, ROHM_EXT_VBUSCLPS_TH_SET, (UINT8 *)&data, ROHM_DATA_SIZE);
    if(I2C_SUCCESS != i2c_sts)
    {
        pm_log_message("ChargerExtHwLibRohm_Init: ROHM_EXT_VBUSCLPS_TH_SET set Failed..");
        return PM_ERR_FLAG_BUS_ERR;
    }

    //Manually checking BC1.2 charger type and configuring ICL to 1.5A if CDP or DCP
    Status |= Rohm_GetHSUsbPortType(&ChgPortType);
    if((PM_ERR_FLAG_SUCCESS == Status) &&
        ((ChgPortType == EFI_USB_PWR_CTRL_HSUSB_CHGPORT_CDP)||
            (ChgPortType == EFI_USB_PWR_CTRL_HSUSB_CHGPORT_DCP )))
    {
        pm_log_message("ChargerExtHwLibRohm_Init: ChgPortType = %d, Setting ICL to 1.5A after manually checking BC 1.2 charger type", ChgPortType);
        Status |= Rohm_SetSnkMaxInputCurrent(MAX_1500_MA);
    }
    
    data = (CHGWDT_FST_MASK | CHGWDT_PRE_MASK) ;
    i2c_sts = ChargerExtHw_i2c_write(&gRohmDevice, ROHM_EXT_CMD_CHGWDT_SET, (UINT8 *)&data, ROHM_DATA_SIZE);
    if(I2C_SUCCESS != i2c_sts)
    {
        pm_log_message("ChargerExtHwLibRohm_Init: ROHM_EXT_CMD_CHGWDT_SET set Failed");
        return PM_ERR_FLAG_BUS_ERR;
    }

    data = IPRECH_LIM_MAX;
    i2c_sts = ChargerExtHw_i2c_write(&gRohmDevice, ROHM_EXT_CMD_IPRECH_SET, (UINT8 *)&data, ROHM_DATA_SIZE);
    if(I2C_SUCCESS != i2c_sts)
    {
        pm_log_message("ChargerExtHwLibRohm_Init: ROHM_EXT_CMD_IPRECH_SET set Failed");
        return PM_ERR_FLAG_BUS_ERR;
    }

    data = VSYSREG_SET;
    i2c_sts = ChargerExtHw_i2c_write(&gRohmDevice, ROHM_EXT_CMD_VSYSREG_SET, (UINT8 *)&data, ROHM_DATA_SIZE);
    if(I2C_SUCCESS != i2c_sts)
    {
        pm_log_message("ChargerExtHwLibRohm_Init: ROHM_EXT_CMD_VSYSREG_SET set Failed");
        return PM_ERR_FLAG_BUS_ERR;
    }

    return Status;
}

/**
  Get Charging Status

  @param[OUT]    pChargingSts     TRUE if charging is on-going
                                  FALSE if not charging

  @return
  PM_ERR_FLAG_SUCCESS:                  Function returned successfully.
  PM_ERR_FLAG_INVALID_PARAMETER:        A Parameter was incorrect.
  PM_ERR_FLAG_BUS_ERR:                  The physical device reported an error.
*/
static pm_err_flag_type Rohm_GetChargingStatus(BOOLEAN *pChargingSts)
{
  pm_err_flag_type Status = PM_ERR_FLAG_SUCCESS;  
  i2c_status i2c_sts = I2C_SUCCESS;
  UINT16 data = 0;

  if (pChargingSts == NULL)
  {
      return PM_ERR_FLAG_INVALID_PARAMETER;
  }
      
  i2c_sts = ChargerExtHw_i2c_read(&gRohmDevice, ROHM_EXT_CMD_CHGSTM_STATUS, (UINT8 *)&data, ROHM_DATA_SIZE);
  if(I2C_SUCCESS != i2c_sts) {return PM_ERR_FLAG_BUS_ERR;}

  *pChargingSts = ((CHGSTM_STATUS_TRICKLE_MASK | CHGSTM_STATUS_PRECHARGE_MASK | CHGSTM_STATUS_FAST_MASK | 
                  CHGSTM_STATUS_TOPOFF_MASK) & data) ? TRUE : FALSE;
  return Status;
}

static pm_err_flag_type Rohm_GetHSUsbPortType(CHARGER_EXT_HW_HSUSB_CHGPORT_TYPE *ChgPortType)
{
    pm_err_flag_type Status = PM_ERR_FLAG_SUCCESS;
    i2c_status i2c_sts = I2C_SUCCESS;
    UINT16 data = 0;

    if (ChgPortType == NULL)
    {
        return PM_ERR_FLAG_INVALID_PARAMETER;
    }

    i2c_sts = ChargerExtHw_i2c_read(&gRohmDevice, ROHM_EXT_CMD_VBUS_UCD_STATUS, (UINT8 *)&data, ROHM_DATA_SIZE);
    if(I2C_SUCCESS != i2c_sts) {return PM_ERR_FLAG_BUS_ERR;}

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


static pm_err_flag_type Rohm_SetSnkMaxInputCurrent(UINT32 MaxCurrentMA)
{
    pm_err_flag_type Status = PM_ERR_FLAG_SUCCESS;
    i2c_status i2c_sts = I2C_SUCCESS;
    UINT16 data = 0;

    if(MaxCurrentMA > IBUS_LIM_MAX)
    {
        MaxCurrentMA = IBUS_LIM_MAX;
    }

    data  = (MaxCurrentMA + (IBUS_LIM_STEP/2));
    data &= IBUS_LIM_SET_MASK;

    i2c_sts = ChargerExtHw_i2c_write(&gRohmDevice, ROHM_EXT_CMD_IBUS_LIM_SET, (UINT8 *)&data, ROHM_DATA_SIZE);
    if(I2C_SUCCESS != i2c_sts) {return PM_ERR_FLAG_BUS_ERR;}

    return Status;
}


/**
  Get Vbus Detect result from Rohm Charger Module

  @param[OUT]   bVbusDetect     Return the result of Vbus Detect

  @return
  PM_ERR_FLAG_SUCCESS:                  Function returned successfully.
  PM_ERR_FLAG_INVALID_PARAMETER:        A Parameter was incorrect.
  PM_ERR_FLAG_BUS_ERR:             The physical device reported an error.
*/
static pm_err_flag_type Rohm_GetVbusDetect(BOOLEAN *bVbusDetect)
{
    pm_err_flag_type Status = PM_ERR_FLAG_SUCCESS;
    i2c_status i2c_sts = I2C_SUCCESS;
    UINT16 data = 0;

    if (bVbusDetect == NULL)
    {
        return PM_ERR_FLAG_INVALID_PARAMETER;
    }

    /* VBus detect status here */
    i2c_sts = ChargerExtHw_i2c_read(&gRohmDevice, ROHM_EXT_CMD_VBUS_VCC_STATUS, (UINT8 *)&data, ROHM_DATA_SIZE);
    if(I2C_SUCCESS != i2c_sts) {return PM_ERR_FLAG_BUS_ERR;}

    *bVbusDetect = (VBUS_VCC_STS_VBUS_DET & data) ? TRUE : FALSE;
    /* For VBus valid, Check VBUS_UCD_STATUS <7>VBUS_VLD */

    return Status;
}

static pm_err_flag_type Rohm_EnableCharger(BOOLEAN bEnable)
{
  i2c_status i2c_sts = I2C_SUCCESS;
  UINT16 data = ((UINT16)bEnable << CHGOP_SET2_CHG_EN_SHIFT);
  

  i2c_sts = Rohm_i2c_write_mask(ROHM_EXT_CMD_CHGOP_SET2, CHGOP_SET2_CHG_EN_MASK, data, ROHM_DATA_SIZE);
  if(I2C_SUCCESS != i2c_sts) 
  {
    pm_log_message("Rohm_EnableCharger: Failed I2C writing");
    return PM_ERR_FLAG_BUS_ERR;
  }
  return PM_ERR_FLAG_SUCCESS;
}

/**
  De-initialize the interface to Charging module

  @param none

  @return
  PM_ERR_FLAG_SUCCESS:              Function returned successfully.
  PM_ERR_FLAG_BUS_ERR:              The physical device reported an error.
*/
static pm_err_flag_type ChargerExtHwLibRohm_Deinit(void)
{
    pm_err_flag_type Status = PM_ERR_FLAG_SUCCESS;
    i2c_status i2c_sts;

    if (gRohmDevice.hI2CHandle != NULL)
    {
        i2c_sts = i2c_close(gRohmDevice.hI2CHandle);
        if(I2C_SUCCESS == i2c_sts)
        {
            Status = PM_ERR_FLAG_SUCCESS;
        }
    }
    return Status;
}

//-------------------------- FG -----------------------//

/**
  Initialize the interface to FG module

  @param none

  @return
  PM_ERR_FLAG_SUCCESS:           Function returned successfully.
  PM_ERR_FLAG_ERROR:             The physical device reported an error.
*/
static pm_err_flag_type SmartChargerLibFG_Init(void)
{
    i2c_status i2c_sts = I2C_SUCCESS;

    gFGDevice.hI2CFGHandle = NULL;
    gFGDevice.I2CFGInstance = FgHwInfo.Index;

    gFGDevice.I2CFGCfg.bus_frequency_khz = 100;  /* 100 KHz */
    gFGDevice.I2CFGCfg.slave_address = FgHwInfo.Address;
    gFGDevice.I2CFGCfg.mode = I2C;
    gFGDevice.I2CFGCfg.slave_max_clock_stretch_us = 100;
    gFGDevice.I2CFGCfg.core_configuration1 = 0;
    gFGDevice.I2CFGCfg.core_configuration2 = 0;

    gFGDevice.TimeoutMs = 0;

    i2c_sts = i2c_open(gFGDevice.I2CFGInstance, &gFGDevice.hI2CFGHandle);
    if(I2C_SUCCESS != i2c_sts)
    {
        pm_log_message("SmartChargerLibFG_Init: Failed to communicate to FG through I2C: %r", i2c_sts);
        return PM_ERR_FLAG_BUS_ERR;
    }
    
    return PM_ERR_FLAG_SUCCESS;
}

/**
  De-initialize the interface to FG module

  @param none

  @return
  PM_ERR_FLAG_SUCCESS:             Function returned successfully.
  PM_ERR_FLAG_BUS_ERR:             The physical device reported an error.
*/
static pm_err_flag_type SmartChargerLibFG_Deinit(void)
{
    i2c_status i2c_sts = I2C_SUCCESS;

    if (gFGDevice.hI2CFGHandle != NULL)
    {
        i2c_sts = i2c_close(gFGDevice.hI2CFGHandle);
        if(I2C_SUCCESS != i2c_sts)
        {
            return PM_ERR_FLAG_BUS_ERR;
        }
    }
    return PM_ERR_FLAG_SUCCESS;
}

//--- I2C FG-- */
/**
  Get Battery Status from FG Module

  @param[IN]    FgHwInfo          Hardware Information for this request
  @param[OUT]   BatteryStatus   Return the battery status

  @return
  PM_ERR_FLAG_SUCCESS:                  Function returned successfully.
  PM_ERR_FLAG_INVALID_PARAMETER:        A Parameter was incorrect.
  PM_ERR_FLAG_BUS_ERR:             The physical device reported an error.
  PM_ERR_FLAG_INVALID:              The physical device does not support this function
*/
static pm_err_flag_type SmartChargerLibFG_GetBatteryStatus(SmartChargerFG_BattStatusInfo *pBatteryStatus)
{
    i2c_status i2c_sts = I2C_SUCCESS;
    UINT16 data = 0;
    INT16 current = 0;

    // Sanity Check
    if (pBatteryStatus == NULL)
    {
        return PM_ERR_FLAG_INVALID_PARAMETER;
    }

    i2c_sts = fg_i2c_read(SBS_CMD_StateOfCharge, (UINT8 *)&data, SBS_DATA_SIZE);
    if(I2C_SUCCESS != i2c_sts) 
    {
        pm_log_message("FG SBS_CMD_StateOfCharge read error = [%r]", i2c_sts);
        return PM_ERR_FLAG_BUS_ERR;
    }
    pBatteryStatus->StateOfCharge = data;

    data = 0;
    i2c_sts = fg_i2c_read(SBS_CMD_Voltage, (UINT8 *)&data, SBS_DATA_SIZE);
    if(I2C_SUCCESS != i2c_sts) 
    {
        pm_log_message("FG SBS_CMD_Voltage read error = [%r]", i2c_sts);
        return PM_ERR_FLAG_BUS_ERR;
    }
    pBatteryStatus->BatteryVoltage = data;
    
    data = 0;
    i2c_sts = fg_i2c_read(SBS_CMD_Current, (UINT8 *)&data, SBS_DATA_SIZE);
    if(I2C_SUCCESS != i2c_sts) 
    {
        pm_log_message("FG BatteryChargeCurrent read error = [%r]",i2c_sts );
        return PM_ERR_FLAG_BUS_ERR;
    }
    current = (INT16)data;
    
    
    if(data & 0x8000) //if it is a negative number apply 2's compliment.
     {
         data = (~data)&(0x7FFF);
         data = data+1;
         data = (-1)* data;
     }
    current = data;
    
    pBatteryStatus->ChargeCurrent= current;
    
    return PM_ERR_FLAG_SUCCESS;
}


//--------------------------------I2C ---------------------


/**
 * I2C Read/Write Interface for External Charger HW
 */
static i2c_status ChargerExtHw_i2c_read(ChargerExtHw_Device *I2CDev, UINT8 offset, UINT8 *data_out, UINT16 size_out)
{
    i2c_status i2c_sts = I2C_TRANSFER_INVALID;
    UINT32 read_cnt = 0;

    if(I2CDev && I2CDev->hI2CHandle && data_out)
    {
        i2c_sts = i2c_read(I2CDev->hI2CHandle,
                            &I2CDev->I2CSlaveCfg,
                            (uint16)offset,
                            I2CDev->I2CSlaveRegMap,
                            data_out,
                            size_out,
                            &read_cnt,
                            I2CDev->TimeoutMs);

        if(read_cnt != size_out) {i2c_sts = I2C_TRANSFER_INVALID;}
    }
    else
    {
        i2c_sts = I2C_ERROR_INVALID_PARAMETER;
    }

    return i2c_sts;
}

static i2c_status ChargerExtHw_i2c_write(ChargerExtHw_Device *I2CDev, UINT8 offset, UINT8 *data_in, UINT16 size_in)
{
    i2c_status i2c_sts = I2C_TRANSFER_INVALID;
    UINT32 write_cnt = 0;

    if(I2CDev && I2CDev->hI2CHandle && data_in)
    {
        i2c_sts = i2c_write(I2CDev->hI2CHandle,
                            &I2CDev->I2CSlaveCfg,
                            (uint16)offset,
                            I2CDev->I2CSlaveRegMap,
                            data_in,
                            size_in,
                            &write_cnt,
                            I2CDev->TimeoutMs);

        if(write_cnt != size_in) {i2c_sts = I2C_TRANSFER_INVALID;}
    }
    else
    {
        i2c_sts = I2C_ERROR_INVALID_PARAMETER;
    }

    return i2c_sts;
}

static i2c_status Rohm_i2c_write_mask(UINT8 offset, UINT16 mask, UINT16 data_in, UINT16 size_in)
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


/**
 * I2C Read/Write Interface for FG Module
 */
static i2c_status fg_i2c_read(UINT8 offset, UINT8 *data_out, UINT16 size_out)
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
                pm_log_message("fg_i2c_read: i2c_sts : [%r], read_retry = %d", i2c_sts, read_retry);
            }
            else
            {
                break;
            }
        }
    }
    else
    {
        pm_log_message("fg_i2c_read: Invalid FG I2C handle..");
        i2c_sts = I2C_ERROR_INVALID_PARAMETER;
    }
    return i2c_sts;
}


//-----Main function ----//

pm_err_flag_type pm_ext_chgr_weakBatt_init()
{
    pm_err_flag_type Status = PM_ERR_FLAG_SUCCESS;
    
    CHARGER_EXT_HW_HSUSB_CHGPORT_TYPE ChgPortType = CHARGER_EXT_HW_HSUSB_CHGPORT_INVALID;
    SmartChargerFG_BattStatusInfo pBatteryStatus;
    BOOLEAN bBatteryPresence = FALSE;
    BOOLEAN bVbusDetect = FALSE;
    BOOLEAN chgStatus = FALSE;
    
    //Enable USB as PON trigger:
    //pm_pon_set_option_bit(0, PM_PON_OPTION_PON1_REDGE_PON, FALSE);
    
    //Initialize the Charger & FG 
    Status = ChargerExtHwLibRohm_Init();
    Status |= SmartChargerLibFG_Init();
    if(Status != PM_ERR_FLAG_SUCCESS)
    {
        pm_log_message("pm_ext_chgr_weakBatt_init: Failed to initialize Charger/FG..");
        return PM_ERR_FLAG_FAILURE;
    }
    
    if(PM_ERR_FLAG_SUCCESS != Rohm_GetHSUsbPortType(&ChgPortType))
    {
        pm_log_message("pm_ext_chgr_weakBatt_init: Failed to get ChargerType");
        return PM_ERR_FLAG_FAILURE;
    }
    
    pm_log_message("pm_ext_chgr_weakBatt_init: ChargerType %d", ChgPortType);
    if((ChgPortType == CHARGER_EXT_HW_HSUSB_CHGPORT_DCP) || (ChgPortType == CHARGER_EXT_HW_HSUSB_CHGPORT_CDP) )
    {
        if(PM_ERR_FLAG_SUCCESS != Rohm_SetSnkMaxInputCurrent(MAX_1500_MA)) //set to 1.5Amps
        {
            return PM_ERR_FLAG_FAILURE;
        }
        pm_log_message("pm_ext_chgr_weakBatt_init: Charger detected as DCP/CDP, so booting to UEFI..");
    }
    else //If ChgPortType type is weak charger (SDP)
    {
       if(PM_ERR_FLAG_SUCCESS == SmartChargerLibFG_GetBatteryStatus(&pBatteryStatus))
       {
           bBatteryPresence= TRUE;  // if the I2C communication to battery status is success treat it as Battery present.
       }
       else
       {
           pm_busy_wait(20*1000);  // Wait for 20ms and try again, As smart battery will not be accessible to I2C until some current flow through it.
           if(PM_ERR_FLAG_SUCCESS == SmartChargerLibFG_GetBatteryStatus(&pBatteryStatus))
           {
               bBatteryPresence= TRUE;
           }
           else
           {
               bBatteryPresence= FALSE;
           }
           
       }
       
       pm_log_message("pm_ext_chgr_weakBatt_init: Battery Present status: %d", bBatteryPresence);
       
       if(bBatteryPresence)
       {
         if(pBatteryStatus.BatteryVoltage > 6600 )
         {
            //Boot to UEFi
            pm_log_message("pm_ext_chgr_weakBatt_init: Battery Voltage %d, So booting to UEFI..", pBatteryStatus.BatteryVoltage);
         }
         else
         {
             //Is charger present or VBUS Ok.
             if(PM_ERR_FLAG_SUCCESS != Rohm_GetVbusDetect(&bVbusDetect))
             {
                 pm_log_message("pm_ext_chgr_weakBatt_init: Failed to get VBUS status");
                 return PM_ERR_FLAG_FAILURE;
             }
             
             pm_log_message("pm_ext_chgr_weakBatt_init: VBUS status %d ", bVbusDetect);
             if(!bVbusDetect)
             {
                //Shutdown the device
                pm_log_message("pm_ext_chgr_weakBatt_init: No VBUS and Low battery, Device going for shutdown");
                boot_hw_powerdown();
             }
             
             //Low battery with weak charger: Enable charging and perform dead battery charging
             Status = Rohm_GetChargingStatus(&chgStatus);
             if(!chgStatus)
             {
                 Status |= Rohm_EnableCharger(TRUE);
             }
             
             if(Status!= PM_ERR_FLAG_SUCCESS)
             {
                 pm_log_message("pm_ext_chgr_weakBatt_init: Failed to enable charging");
                 return PM_ERR_FLAG_FAILURE;
             }
             
             //Loop till battery SOC reached to 5%
             while (pBatteryStatus.BatteryVoltage <= 6600)
             {
                Status |= SmartChargerLibFG_GetBatteryStatus(&pBatteryStatus);
                
                //if vbus removed then Shutdown the device. //Todo: if battery removed then also go for shutdown
                Status |= Rohm_GetVbusDetect(&bVbusDetect);
                if(!bVbusDetect)
                {
                   //Shutdown the device
                   pm_log_message("pm_ext_chgr_weakBatt_init: Vbus status %d & Low battery, So Device going for shutdown", bVbusDetect);
                   boot_hw_powerdown();
                }
                
                //Print the charging status
                pm_log_message("Battery Status: SOC :%d, IBAT:%d, VBAT:%d ",  pBatteryStatus.StateOfCharge, pBatteryStatus.ChargeCurrent, pBatteryStatus.BatteryVoltage );
                
                //delay 500 ms;
                Status |= pm_busy_wait(PM_WEAK_BATTERY_CHARGING_DELAY); 
                
                if(Status != PM_ERR_FLAG_SUCCESS){return PM_ERR_FLAG_FAILURE;}
             }        
         }  
       }
       else //if no battery present and weak Charger connected (SDP)
       {
          pm_log_message("No battery and Weak charger connected (SDP), Continue to Boot");  //To support Automation Farm setup
          
          //Disable USB as PON reason and shutdown
          //pm_log_message("No battery and Weak charger, So device going for shutdown");
          //pm_pon_set_option_bit(0, PM_PON_OPTION_PON1_REDGE_PON, TRUE);
          //boot_hw_powerdown();
       }
    }
    Status = ChargerExtHwLibRohm_Deinit();
    Status |= SmartChargerLibFG_Deinit();
    if(Status != PM_ERR_FLAG_SUCCESS){return PM_ERR_FLAG_FAILURE;}
    
    return Status;
}
  