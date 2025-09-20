/*! \file pm_sbl_boot_oem.c
*  \n
*  \brief This file contains PMIC device initialization function where initial PMIC
*  \n SBL settings are configured through the PDM auto-generated code.
*  \n
*  \n &copy; Copyright 2019 QUALCOMM Technologies Incorporated, All Rights Reserved
*/
/* =======================================================================
Edit History
This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.


when       who     what, where, why
03/27/19   kugaurav Branch for poipu auto
04/24/18   pxm     Branch for hana auto
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
#include "pm_pbs_driver.h"
#include "pm_version.h"
#include "pm_pbs_info.h"
#include "pm_log_utils.h"
#include "boothw_target.h"


/*===========================================================================

                        TYPE DEFINITIONS 

===========================================================================*/
#define PM_VREG_SP_SID 0
#define PM_VREG_SP_ADDR 0x3E00

#define PM_VERSION_LOG_LENGTH 128


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
  int size = 0, offset = 0;

  size = snprintf(pm_version, PM_VERSION_LOG_LENGTH - 1, "PMIC");

  for (pmic_index = 0; pmic_index < PM_MAX_NUM_PMICS; pmic_index++)
  {
    err_flag = pm_get_pmic_info(pmic_index, &pmic_info);
    if (err_flag != PM_ERR_FLAG_SUCCESS)
    {
      return err_flag;
    }

    if (pmic_info.ePmicModel != PMIC_IS_UNKNOWN && pmic_info.ePmicModel != PMIC_IS_INVALID)
    {
      offset += size;
      size = snprintf(pm_version + offset, PM_VERSION_LOG_LENGTH - 1 - offset,
        " %c:%d.%d", pmic_model[pmic_index], pmic_info.nPmicAllLayerRevision, pmic_info.nPmicMetalRevision);
    }
  }

  pm_log_message(pm_version);

  return err_flag;
}

pm_err_flag_type
pm_device_pre_init ( void )
{
  pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
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

  /*These configurations is only used for development phones 
   *and should be commented out for production phones
   */
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

  err_flag |= pm_app_vreg_clear_ocp_status();

  return err_flag;
}


pm_err_flag_type
pm_sbl_chg_pre_init ( void )
{
  pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
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
  return err_flag;
}
