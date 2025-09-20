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
09/30/19   cgoel   configuring GPIO 9C and turning on L11c for COMPAL, HASTINGS and ADARSTEA platforms 
05/02/18   al      remove wa to enable Dual-Role for Charger PMICs
04/06/18   dch     Enable Dual-Role for Charger PMICs
02/01/18   aab     Updated PMI_CLK_DIST_ADDR to be VREG_SP
12/22/17   pxm     Change SRAM access to DMA support
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
#include "pm_sdam_driver.h"
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
#include "pm_pbs_driver.h"
#include "pm_version.h"
#include "pm_pbs_info.h"
#include "pm_log_utils.h"
#include "pm_fg_sram.h"
#include "boothw_target.h"
#include "DDIPlatformInfo.h"
#include "SpmiCfg.h"
#include "pm_gpio.h"
#include "DDIHWIO.h"
/*===========================================================================

                        TYPE DEFINITIONS

===========================================================================*/
#define PM_VREG_SP_SID 0
#define PM_VREG_SP_ADDR 0x3E00
#define PM_WLAN_SW_CTRL_MGPI_INDEX 3
// MULSEL 3 refer to GPIO68
#define PM_WLAN_SW_CTRL_GPIO_MUL_SEL 3

#define PM_SDCARD_DET_MGPI_INDEX 4
#define PM_VERSION_LOG_LENGTH 128

#define GPIO9_PERPH_RESET_CTL3  0x0C8DA
#define PERPH_CTL3_MASK         0x0F
#define IGNORE_WARM_RESET       0x0B
#define PM8180L_SID             0x04
#define COMPAL                  1
#define HASTINGS_AND_SDX55      1
#define ADRASTEA_AND_SDX55      2

#define HASTINGS_AND_4G		    4
#define SDAM_MEM_036            0xB264
#define HASTINGS_PRESENT		0x02
#define CHEROKEE_PRESENT		0x01

static pm_sdam_reg_info_type sdam_reg =
{
  .base_address      = 0xB000,
  .peripheral_offset = 0x100,
  .trig_busy_ret     = 0x0043,
};

/*===========================================================================

                        FUNCTION DEFINITIONS

===========================================================================*/

pm_err_flag_type pm_log_pmic_version(void)
{
  pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
  uint8 pmic_index;
  pm_device_info_type pmic_info;
  char pmic_model[PM_MAX_NUM_PMICS] = "ABCDEFG";
  char pm_version[PM_VERSION_LOG_LENGTH] = {0};
  char* ptr = pm_version;


  snprintf(ptr, PM_VERSION_LOG_LENGTH, "PMIC");
  ptr += sizeof("PMIC") - 1;  // sizeof will include the end \0

  for (pmic_index = 0; pmic_index < PM_MAX_NUM_PMICS; pmic_index++)
  {
    err_flag = pm_get_pmic_info(pmic_index, &pmic_info);
    if (err_flag != PM_ERR_FLAG_SUCCESS)
    {
      return err_flag;
    }

    if (pmic_info.ePmicModel != PMIC_IS_UNKNOWN && pmic_info.ePmicModel != PMIC_IS_INVALID)
    {
      *ptr++ = ' ';
      *ptr++ = pmic_model[pmic_index];
      *ptr++ = ':';
      *ptr++ = '0' + pmic_info.nPmicAllLayerRevision;
      *ptr++ = '.';
      *ptr++ = '0' + pmic_info.nPmicMetalRevision;
    }
  }

  boot_log_message(pm_version);

  return err_flag;
}

pm_err_flag_type
pm_device_pre_init ( void )
{
  uint32                slave_id        = 0;
  uint32                num_of_sdam     = 0;
  peripheral_info_type  peripheral_info;
  pm_comm_info_type*    comm_info       = NULL;
  pm_err_flag_type      err_flag        = PM_ERR_FLAG_SUCCESS;
  pm_sbl_specific_data_type *config_param_ptr = NULL;

  config_param_ptr = (pm_sbl_specific_data_type*)
            pm_target_information_get_specific_info(PM_PROP_SBL_SPECIFIC_DATA);
  CORE_VERIFY_PTR(config_param_ptr);
  
  err_flag |= pm_log_pmic_version();
  
  // This is used by PSI as a scratch register to check for which
  // configuration to load for swire map
  if ( config_param_ptr->swire_cfg.supported &&
       config_param_ptr->swire_cfg.swire_map < PM_SWIRE_MAP_INVALID )
  {
    err_flag |= pm_comm_write_byte_mask(5, 0xDC5F, 0x3,
                              (uint8)config_param_ptr->swire_cfg.swire_map, 0);
  }
  if(err_flag != PM_ERR_FLAG_SUCCESS)
  {
    return err_flag;
  }
  
  //Clear PBS_RET for each sdam
  for(uint8 i = 0; i < PM_MAX_NUM_PMICS; i++)
  {
    num_of_sdam = 0;

    err_flag = pm_get_slave_id(i, 0, &slave_id);
	
    if(err_flag != PM_ERR_FLAG_SUCCESS)
	{
	  err_flag = PM_ERR_FLAG_SUCCESS;
	  continue;
	}
      
    num_of_sdam = 
    pm_target_information_get_periph_count_info(PM_PROP_SDAM_NUM, i);

    //Since poipu has an smb configuration as well check for smb and make sdam as 0

    comm_info = pm_comm_get_comm_info(i, 0);
    if(comm_info == NULL)
    {
      CORE_VERIFY(0);
    }
 
    for(uint8 j = 0; j < num_of_sdam; j++)
    {
      memset(&peripheral_info, 0, sizeof(peripheral_info_type));
  
      peripheral_info.base_address = (sdam_reg.base_address) + 
                                     (j * sdam_reg.peripheral_offset);
	    		  
	  err_flag = pm_comm_write_byte(comm_info->slave_id, peripheral_info.base_address + sdam_reg.trig_busy_ret,0x00, 0);
	  if(err_flag != PM_ERR_FLAG_SUCCESS)
      {
        return err_flag;
      }
    }
  }
  
  if ( config_param_ptr->rfclk2_config_flag )
  {
    // Do NOT follow pin control
    err_flag |= pm_comm_write_byte(0, 0x5547, 0x0, 0);

    // Enable pull down for RF_CLK2 peripheral
    err_flag |= pm_comm_write_byte(0, 0x5548, 0x80, 0);
  }

  return err_flag;
}


pm_err_flag_type
pm_device_post_init ( void )
{
  pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
  uint8 rtc, sdam = 0;
  
  //These configurations is only used for development phones
  //and should be commented out for production phones
  err_flag |= pm_app_pon_pshold_cfg(PM_APP_PON_CFG_HARD_RESET);

  //PON KPDPWR PON Reset configuration
  err_flag |= pm_app_pon_reset_cfg(PM_APP_PON_RESET_SOURCE_KPDPWR,
                                   PM_APP_PON_CFG_WARM_RESET, 10256, 2000);

  //PON RESIN_AND_KPDPWR PON Reset configuration
  err_flag |= pm_app_pon_reset_cfg(PM_APP_PON_RESET_SOURCE_RESIN_AND_KPDPWR,
                                 PM_APP_PON_CFG_DVDD_HARD_RESET, 10256, 2000);

  err_flag |= pm_app_pon_reset_init();

  /*check for valid PON keypress */
  err_flag |= pm_app_pwrkey_long_press_check(PM_PON_PWRKEY_DBNC_CHK_AT_LOADER);
 
 //Debug prints
  err_flag |= pm_comm_read_byte(0, 0xB177, &sdam, 1);
  err_flag |= pm_comm_read_byte(0, 0x6009, &rtc, 1);
  pm_log_message("Device Init # RTC: %d, #SDAM: %d", rtc, sdam );
  
  return err_flag;
}


pm_err_flag_type
pm_driver_pre_init ( void )
{
  pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;

  return err_flag;
}


pm_err_flag_type
pm_driver_post_init ( void )
{
  pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
  pm_register_data_type sdam_flag = 0;
  err_flag |= pm_app_vreg_clear_ocp_status();

  /*Enabling L17E SD card rail for ramdump collection, since its removed from PON Sequence and needs to be enabled post MGPI configuration */
  err_flag |= pm_ldo_sw_mode(PMIC_E, PM_LDO_17, PM_SW_MODE_NPM); 
  err_flag |= pm_ldo_volt_level(PMIC_E, PM_LDO_17, 2960000, FALSE);
  err_flag |= pm_ldo_sw_enable(PMIC_E, PM_LDO_17, PM_ON, TRUE); 

  /* gp2 timer settings */
  boolean invert = TRUE;
  boolean enable = TRUE;
  boolean compal_device = FALSE;
  boolean mtp_sdx_fusion = FALSE;
  boolean mtp_hastings = FALSE;
  
  DALResult eDalResult = DAL_SUCCESS;
  DalDeviceHandle *hPlatforminfo = NULL;
  DalPlatformInfoPlatformInfoType platformInfo;
  DalPlatformInfoPlatformType platformType = DalPlatformInfo_Platform();
  ChipInfoVersionType chipVersion;
  ChipInfoIdType chip_id;
  unsigned int feature_sku;
  
  /** Read Feature ID to differentiate Poipu Pro device **/
  feature_sku = BOOT_HWIO_IN(QFPROM_CORR_PTE_ROW0_LSB,0);
  feature_sku = feature_sku >> 20;
  feature_sku = feature_sku&(0xff);
  eDalResult = DAL_PlatformInfoDeviceAttach(DALDEVICEID_PLATFORMINFO, &hPlatforminfo);

  if(eDalResult != DAL_SUCCESS || hPlatforminfo == NULL)
  {
    CORE_VERIFY(0);
  }

  eDalResult = DalChipInfo_GetChipId(hPlatforminfo, &chip_id);
  eDalResult = DalChipInfo_GetChipVersion(hPlatforminfo, &chipVersion);
  eDalResult = DalPlatformInfo_GetPlatformInfo (hPlatforminfo, &platformInfo);

  compal_device = ((platformType == DALPLATFORMINFO_TYPE_CLS) && (platformInfo.subtype == COMPAL));
  mtp_sdx_fusion = ((platformType == DALPLATFORMINFO_TYPE_MTP) && ((platformInfo.subtype == HASTINGS_AND_SDX55)||(platformInfo.subtype == ADRASTEA_AND_SDX55)));
    
  if((compal_device) || (mtp_sdx_fusion))
  {
    err_flag |= pm_comm_write_byte_mask(PM8180L_SID, GPIO9_PERPH_RESET_CTL3, PERPH_CTL3_MASK, IGNORE_WARM_RESET, 0);
    err_flag |= pm_gpio_config_digital_output(PMIC_C, PM_GPIO_9,  PM_GPIO_OUT_BUFFER_CONFIG_CMOS,  PM_GPIO_VIN1,  PM_GPIO_SOURCE_GND, PM_GPIO_OUT_BUFFER_HIGH, invert);
    err_flag |= pm_gpio_enable(PMIC_C, PM_GPIO_9, enable);
   
   if(compal_device)
   {
     err_flag |= pm_ldo_sw_mode(PMIC_C, PM_LDO_11, PM_SW_MODE_NPM); 
     err_flag |= pm_ldo_volt_level(PMIC_C, PM_LDO_11, 3104000, FALSE);
     err_flag |= pm_ldo_sw_enable(PMIC_C, PM_LDO_11, PM_ON, TRUE);
   }
  } 
  
  /** SW needs to set SDAM flag so that PMIC PBS runs correct MGPI sequence for Adreastea/Hastings. **/
  /** default value of SDAM is CHEROKEE_PRESENT**/
  mtp_hastings = ((platformType == DALPLATFORMINFO_TYPE_MTP) && ((platformInfo.subtype == HASTINGS_AND_SDX55)||(platformInfo.subtype == HASTINGS_AND_4G)));
  sdam_flag = (mtp_hastings)? HASTINGS_PRESENT : CHEROKEE_PRESENT; 
  err_flag |= pm_comm_write_byte(0, SDAM_MEM_036, sdam_flag, 1);
  
  /** Update current limit settings for Poipu Pro MSM/APQ v2.1 devices **/
  if ( ((chipVersion == CHIPINFO_VERSION(2,1)) && (chip_id == CHIPINFO_ID_SC8180X)  && (feature_sku == 0x05)) ||
       ((chipVersion == CHIPINFO_VERSION(2,1)) && (chip_id == CHIPINFO_ID_SC8180XP) && (feature_sku == 0x03)) )
   {	
	  err_flag |= pm_comm_write_byte(1, 0x3060, 0x3D, 1);
	  err_flag |= pm_comm_write_byte(1, 0x3061, 0x3C, 1);
	  err_flag |= pm_comm_write_byte(1, 0x2D60, 0x3D, 1);
	  err_flag |= pm_comm_write_byte(1, 0x2D61, 0x3C, 1);
	  err_flag |= pm_comm_write_byte(1, 0x2A60, 0x3D, 1);
	  err_flag |= pm_comm_write_byte(1, 0x2A61, 0x3C, 1);
	  err_flag |= pm_comm_write_byte(1, 0x2760, 0x3D, 1);
	  err_flag |= pm_comm_write_byte(1, 0x2761, 0x3C, 1);
	  err_flag |= pm_comm_write_byte(1, 0x2460, 0x3D, 1);
	  err_flag |= pm_comm_write_byte(1, 0x2461, 0x3C, 1);
   }
   
   if(platformType == DALPLATFORMINFO_TYPE_CLS)
   {
 	 err_flag |= pm_comm_write_byte(6, 0x5549, 0x00, 0); //do not follow pin control for rfclk2 of pmk
	 err_flag |= pm_comm_write_byte(6, 0x554A, 0x00, 0); //do not follow pin control for rfclk2 of pmk
   }
  return err_flag;
}


pm_err_flag_type
pm_sbl_chg_pre_init ( void )
{
  pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;

  /*  Start battery id conversion in continuous mode.
    Next image (UEFI) would need to stop this mode */
  err_flag |= pm_sbl_set_batt_id_forced_mode(TRUE);

  return err_flag;
}


pm_err_flag_type
pm_sbl_chg_post_init ( void )
{
  pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;

  return err_flag;
}


pm_err_flag_type
pm_spmi_tgt_vrm_init ( SpmiBusCfg_Ppid *pm_vrm_rsrc_arr,
                       uint32 *vrm_cfg_index,
                       uint8 vrm_rsrc_arr_size )
{
  pm_err_flag_type err_flag   = PM_ERR_FLAG_SUCCESS;

  if ( *vrm_cfg_index >= vrm_rsrc_arr_size )
  {
    return err_flag = PM_ERR_FLAG_INVALID_PARAMETER;
  }

  pm_vrm_rsrc_arr[*vrm_cfg_index].slaveId = PM_VREG_SP_SID;
  pm_vrm_rsrc_arr[*vrm_cfg_index].address = PM_VREG_SP_ADDR;
  (*vrm_cfg_index)++;

  return err_flag;
}

pm_err_flag_type
pm_tgt_mgpi_cfg ( SpmiBusCfg_MgpiPortCfg **mgpi_port_cfg_ptr )
{
  pm_err_flag_type err_flag   = PM_ERR_FLAG_SUCCESS;

  if ( NULL == mgpi_port_cfg_ptr )
  {
    return PM_ERR_FLAG_INVALID_PARAMETER;
  }

  DalPlatformInfoPlatformType platform_type = DalPlatformInfo_Platform();
  if ( DALPLATFORMINFO_TYPE_CLS == platform_type )
  {
    if ( NULL == mgpi_port_cfg_ptr[PM_WLAN_SW_CTRL_MGPI_INDEX] ||
         NULL == mgpi_port_cfg_ptr[PM_SDCARD_DET_MGPI_INDEX])
    {
      return PM_ERR_FLAG_INVALID_PARAMETER;
    }

    // For CLS, WLAN SW CTRL GPIO is 68
    SpmiBusCfg_MgpiPortCfg wlan_config = *mgpi_port_cfg_ptr[PM_WLAN_SW_CTRL_MGPI_INDEX];
    wlan_config.muxSelVal = PM_WLAN_SW_CTRL_GPIO_MUL_SEL;

    if ( SpmiCfg_ConfigureMgpiPort(&wlan_config) != SPMI_SUCCESS )
    {
      return PM_ERR_FLAG_BUS_ERR;
    }

    SpmiBusCfg_MgpiPortCfg sd_config = *mgpi_port_cfg_ptr[PM_SDCARD_DET_MGPI_INDEX];
    sd_config.negEdgeEn = TRUE;

    if ( SpmiCfg_ConfigureMgpiPort(&sd_config) != SPMI_SUCCESS )
    {
      return PM_ERR_FLAG_BUS_ERR;
    }
  }
  return err_flag;
}
