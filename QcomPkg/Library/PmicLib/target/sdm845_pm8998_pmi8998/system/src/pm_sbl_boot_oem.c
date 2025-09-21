/*! \file pm_sbl_boot_oem.c
*  \n
*  \brief This file contains PMIC device initialization function where initial PMIC
*  \n SBL settings are configured through the PDM auto-generated code.
*  \n
*  \n &copy; Copyright 2015-2018 QUALCOMM Technologies Incorporated, All Rights Reserved
*/
/* =======================================================================
Edit History
This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.


when       who     what, where, why
--------   ---     ----------------------------------------------------------
12/21/17   aab     Added pm_app_config_download_mode api. 
11/15/17   pxm     Add SMB support in order to apply SMB's workaround
10/31/17   czq     support for HDK platform
10/17/17   czq     Move FG sram related WAs to pm_fg_sram.c 
09/28/17   pxm     Remove the code which turning off L24A for EUD check.
09/13/17   aab     Added target specific fn: pm_spmi_tgt_vrm_init()
07/10/17   pxm     Loader to provide OTP Read API for VBAT,IBAT Gain offset
06/13/17   czq     Configure BATT_ID bias delay.
04/06/17   aab	   Updated pm_driver_post_init(): Turn off EUD Rail if VBUS is detected 
03/07/17   ds	   Removed the call to pm_aop_pre_init()
01/16/17   aab     Updated pm_driver_post_init() to support SDM845
06/01/16   aab     Updated pm_driver_post_init() to add RF_CLK Drive strength configuration if CDP is detected
04/29/15   aab     Updated pm_device_post_init() 
12/04/15   aab     Updated to support MSM8998 target 
12/04/15   aab     Creation
========================================================================== */
/*===========================================================================

                     INCLUDE FILES 

===========================================================================*/
#include "pm_sbl_boot_target.h"
#include "pm_pon.h"
#include "pm_app_pon.h"
#include "pm_sbl_boot.h"
#include "pm_device.h"
#include "pm_comm.h"
#include "pm_utils.h"
#include "pm_schg_usb.h"
#include "DDIChipInfo.h"
#include <stdio.h>
#include "boot_logger.h"
#include "CoreVerify.h"
#include "pm_ldo.h"
#include "DALDeviceId.h"
#include "DDIPlatformInfo.h"
#include "DDIChipInfo.h"
#include "pm_app_vreg.h"
#include "pm_target_information.h"
#include "pm_clk.h"
#include "pm_fg_adc_rr.h"
#include "pm_fg_sram.h"
#include "pm_version.h"
#include "pm_log_utils.h"
#include "i2c_api.h"


/*===========================================================================

                        TYPE DEFINITIONS 

===========================================================================*/
#define PMI_CLK_DIST_SID 2
#define PMI_CLK_DIST_ADDR 0x5900

// Allow 3 times retry for the scenario of slave NACKED.
#define I2C_SLAVE_NACKED (1 << 3)
#define SMB_RETRY_COUNT_MAX 3
#define SMB_TIMEOUT_MAX 2500
#define SMB_BUS_FREQUENCY_KHZ 400
#define SMB_MAX_CLOCK_STRETCH_US 500

// I2C address of SMB1355 and SMB1381
#define SMB_SLAVE_ADDRESS 0x08
// SMB1355 CS part has slave address 0x0C
#define SMB_SLAVE_ADDRESS_OPTIONAL 0x0C
// SMB1355 mounted on I2C instance 010, but since SBL sequence start from 1, so 
// here it is 011 instead 010
#define SMB_I2C_INSTANCE I2C_INSTANCE_011

typedef enum
{
    PM_SMB_NAME_INVALID,
    PM_SMB_NAME_138x,
    PM_SMB_NAME_1355
} pm_smb_name_type;

static uint32 smb_slave_address = 0;
static pm_smb_name_type pm_smb_name = PM_SMB_NAME_INVALID;
/*===========================================================================

                        FUNCTION DEFINITIONS 

===========================================================================*/

i2c_status smb_read(void *i2c_handle, uint16 offset, uint8 *buffer);
i2c_status smb_write(void *i2c_handle, uint16 offset, uint8 data);

/*
 * smb related function must be called after this function to avoid dependency.
 * In this function, we need to determine whether SMB chipset is mounted and 
 * which slave address is the right one.
 */
static pm_err_flag_type smb_init(void);

pm_err_flag_type
pm_device_pre_init(void)
{
  pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
  pm_sbl_specific_data_type *config_param_ptr = NULL;
  void *i2c_handle = NULL;
  i2c_status istatus = I2C_SUCCESS;

  config_param_ptr = (pm_sbl_specific_data_type*)pm_target_information_get_specific_info(PM_PROP_SBL_SPECIFIC_DATA);
  CORE_VERIFY_PTR(config_param_ptr);
  
  if(PM_SMB_NAME_INVALID == pm_smb_name)
  {
	  err_flag |= smb_init();
  }
  
  if(PM_SMB_NAME_138x == pm_smb_name)
  {
	  // Apply tadk leackage workaround for SMB1381/0
	  istatus = i2c_open(SMB_I2C_INSTANCE, &i2c_handle);
      if(I2C_SUCCESS != istatus || NULL == i2c_handle)
      {
        return PM_ERR_FLAG_BUS_ERR;
      }
	  istatus |= smb_write(i2c_handle, 0x36D0, 0xA5);
	  istatus |= smb_write(i2c_handle, 0x3652, 0x00);
	  istatus |= smb_write(i2c_handle, 0x36D0, 0xA5);
	  istatus |= smb_write(i2c_handle, 0x36E7, 0x80);
	  istatus |= i2c_close(i2c_handle); 
	  
	  if(I2C_SUCCESS != istatus)
	  {
		  err_flag = PM_ERR_FLAG_BUS_ERR;
	  }
  }

  if(config_param_ptr->rfclk2_config_flag)
  {
  	err_flag |= pm_comm_write_byte(0, 0x5547, 0x0, 0);	// Do NOT follow pin control
	err_flag |= pm_comm_write_byte(0, 0x5548, 0x80, 0);	// Enable pull down for RF_CLK2 peripheral
  }

  return err_flag;
}

pm_err_flag_type
pm_device_post_init(void)
{
  pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;

  //These configurations is only used for development phones and should be commented out for production phones
  err_flag |= pm_app_pon_pshold_cfg(PM_APP_PON_CFG_WARM_RESET);
  err_flag |= pm_app_pon_reset_cfg( PM_APP_PON_RESET_SOURCE_KPDPWR, PM_APP_PON_CFG_WARM_RESET, 10256, 2000); //PON KPDPWR PON Reset configuration
  err_flag |= pm_app_pon_reset_cfg( PM_APP_PON_RESET_SOURCE_RESIN_AND_KPDPWR, PM_APP_PON_CFG_DVDD_HARD_RESET, 10256, 2000); //PON RESIN_AND_KPDPWR PON Reset configuration

  err_flag |= pm_app_pon_reset_init();
  
  /*check for valid PON keypress */
  err_flag |= pm_app_pwrkey_long_press_check(PM_PON_PWRKEY_DBNC_CHK_AT_LOADER);

  if (DalPlatformInfo_IsFusion() == TRUE)
  {
     /* Configure PM GPIO10 to not follow warm_rb for download mode
        to work on fusion platforms */
     err_flag |= pm_comm_write_byte(0, 0xC9D0, 0xA5, 1);
     err_flag |= pm_comm_write_byte(0, 0xC9DA, 0x0B, 1);
  }

  return err_flag;
}

pm_err_flag_type
pm_driver_pre_init (void)
{
  pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;

  return err_flag;
}

pm_err_flag_type
pm_driver_post_init(void)
{
  pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
  DALResult           	  eResult  = DAL_SUCCESS;

  DalPlatformInfoPlatformType platform_type = DALPLATFORMINFO_TYPE_UNKNOWN;
  platform_type = DalPlatformInfo_Platform();

  DalDeviceHandle *chipInfoHandle = NULL;
  DalChipInfoModemType is_modem_present = 1;

  pm_sbl_specific_data_type *config_param_ptr = NULL;

  config_param_ptr = (pm_sbl_specific_data_type *)pm_target_information_get_specific_info(PM_PROP_SBL_SPECIFIC_DATA);
  CORE_VERIFY_PTR(config_param_ptr);

  if(config_param_ptr->rfclk2_config_flag)
  {
    err_flag |= pm_clk_sw_enable(0, PM_CLK_RF_2, PM_OFF); // Turn off RF_CLK2
  }

  eResult = DAL_DeviceAttach(DALDEVICEID_CHIPINFO, &chipInfoHandle);
  if(eResult != DAL_SUCCESS)
  {
    return PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
  }

  eResult = DalChipInfo_GetModemSupport(chipInfoHandle, &is_modem_present); /*API returns 0 if modem is not present*/
  if(eResult != DAL_SUCCESS)
  {
    return PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
  }

  if(is_modem_present == 0)
  {
    err_flag |= pm_ldo_sw_enable(0, PM_LDO_11, PM_OFF);
    err_flag |= pm_ldo_sw_enable(0, PM_LDO_15, PM_OFF);
  }

  if(platform_type == DALPLATFORMINFO_TYPE_CDP)
  {  //If target is CDP: Set PM RF_CLK drive strength to 3x
    err_flag |= pm_clk_drv_strength(0, PM_CLK_RF_1, PM_CLK_DRV_STRENGTH_3X);
  }

  err_flag |= pm_app_vreg_clear_ocp_status();

  return err_flag;
}

pm_err_flag_type
pm_sbl_chg_pre_init (void)
{
  pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
  DalPlatformInfoPlatformType platform_type = DALPLATFORMINFO_TYPE_UNKNOWN;
  uint32 device_index = 1; //PMI

  err_flag = PmicFgSram_Init(device_index);
  
  platform_type = DalPlatformInfo_Platform();
  if(platform_type == DALPLATFORMINFO_TYPE_QRD) 
  {
	err_flag = pm_fg_adc_rr_batt_id_cfg(device_index, PM_FG_ADC_RR_BIAS_WAIT_80_MS);
  }
  else
  {
	err_flag = pm_fg_adc_rr_batt_id_cfg(device_index, PM_FG_ADC_RR_BIAS_WAIT_20_MS);
  }

  /*  Start battery id conversion in continuous mode.
      Next image (UEFI) would need to stop this mode */
  err_flag |= pm_sbl_set_batt_id_forced_mode(TRUE);
  
  return err_flag;
}

pm_err_flag_type
pm_sbl_chg_post_init (void)
{
  pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;

  return err_flag;
}

pm_err_flag_type pm_spmi_tgt_vrm_init(SpmiBusCfg_Ppid *pm_vrm_rsrc_arr, uint32 *vrm_cfg_index, uint8 vrm_rsrc_arr_size)
{
   pm_err_flag_type err_flag   = PM_ERR_FLAG_SUCCESS;

   if (*vrm_cfg_index >= vrm_rsrc_arr_size)
   {
      return err_flag = PM_ERR_FLAG_INVALID_PARAMETER;
   }

   pm_vrm_rsrc_arr[*vrm_cfg_index].slaveId = PMI_CLK_DIST_SID;
   pm_vrm_rsrc_arr[*vrm_cfg_index].address = PMI_CLK_DIST_ADDR;
   (*vrm_cfg_index)++;

   return err_flag;
}

i2c_status smb_write(void *i2c_handle, uint16 offset, uint8 data)
{
	i2c_status istatus = I2C_SUCCESS;
	i2c_slave_config cfg;
	uint32 written_count = 0;

	cfg.bus_frequency_khz = SMB_BUS_FREQUENCY_KHZ;
	cfg.slave_address = smb_slave_address;
	cfg.mode = I2C;
	cfg.slave_max_clock_stretch_us = SMB_MAX_CLOCK_STRETCH_US;
	cfg.core_configuration1 = 0;
	cfg.core_configuration2 = 0;

	istatus = i2c_write(i2c_handle, &cfg, offset, 2, &data, 
					1, &written_count, SMB_TIMEOUT_MAX);

	return istatus;
}

i2c_status smb_read(void *i2c_handle, uint16 offset, uint8 *buffer)
{
	i2c_status istatus = I2C_SUCCESS;
	i2c_slave_config cfg;
	uint32 read_count = 0;

	cfg.bus_frequency_khz = SMB_BUS_FREQUENCY_KHZ;
	cfg.slave_address = smb_slave_address;
	cfg.mode = I2C;
	cfg.slave_max_clock_stretch_us = SMB_MAX_CLOCK_STRETCH_US;
	cfg.core_configuration1 = 0;
	cfg.core_configuration2 = 0;

	istatus = i2c_read(i2c_handle, &cfg, offset, 2, buffer,
						1, &read_count, SMB_TIMEOUT_MAX);

	return istatus;
}

#define SMB_ADDRESS_PERPH_SUBTYPE 0x0105
#define SMB_SUBTYPE_1355 0x1C
#define SMB_SUBTYPE_138x 0x17
static pm_err_flag_type smb_init(void)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
    i2c_status istatus = I2C_SUCCESS;
    uint8 data = 0;
    void *i2c_handle = NULL;
    uint8 retry_idx = 0;

    istatus = i2c_open(SMB_I2C_INSTANCE, &i2c_handle);
    if(I2C_SUCCESS != istatus)
    {
		// Just no I2C device, not error.
        pm_log_message("i2c open error code: %d", (int)istatus);
        return PM_ERR_FLAG_SUCCESS;
    }

    smb_slave_address = SMB_SLAVE_ADDRESS;

    // this function would be the first location to access SMB register.
    // There is a know issue that some SMB chipset would NACK at the first time.
    // The workaround is to retry at most 3 times.
    for(retry_idx = 0; retry_idx < SMB_RETRY_COUNT_MAX; ++retry_idx)
    {
        istatus = smb_read(i2c_handle, SMB_ADDRESS_PERPH_SUBTYPE, &data);
        if(I2C_SUCCESS == istatus
            || (I2C_SLAVE_NACKED != (istatus & I2C_SLAVE_NACKED)))
        {
            break;
        }
    }

    // SMB1355 has 2 address, try for the other address if failed on first 
    // address
    if(I2C_SUCCESS != istatus)
    {
        smb_slave_address = SMB_SLAVE_ADDRESS_OPTIONAL;
        for(retry_idx = 0; retry_idx < SMB_RETRY_COUNT_MAX; ++retry_idx)
        {
            istatus = smb_read(i2c_handle, SMB_ADDRESS_PERPH_SUBTYPE, &data);
            if((I2C_SUCCESS == istatus)
                || (I2C_SLAVE_NACKED != (istatus & I2C_SLAVE_NACKED)))
            {
                break;
            }
        }
    }

	if(I2C_SUCCESS == istatus)
	{
		switch(data)
		{
			case SMB_SUBTYPE_1355:
				pm_smb_name = PM_SMB_NAME_1355;
				break;
			case SMB_SUBTYPE_138x:
				pm_smb_name = PM_SMB_NAME_138x;
				break;
			default:
				pm_smb_name = PM_SMB_NAME_INVALID;
				break;
		}
	}

    istatus |= i2c_close(i2c_handle); 
    if((istatus != I2C_SUCCESS) && (PM_SMB_NAME_INVALID != pm_smb_name))
    {
        err_flag = PM_ERR_FLAG_BUS_ERR;
    }

    return err_flag;
}



pm_err_flag_type
pm_app_config_download_mode(void)
{
  pm_err_flag_type err_flag = PM_ERR_FLAG__SUCCESS;

  if (DalPlatformInfo_IsFusion() == TRUE)
  {
     /* Configure PM GPIO10 to not follow warm_rb, shutdownx_rb for download mode
        to work on fusion platforms */
     err_flag |= pm_comm_write_byte(0, 0xC9D0, 0xA5, 1);
     err_flag |= pm_comm_write_byte(0, 0xC9DA, 0x08, 1);
  }

  return err_flag;
}
