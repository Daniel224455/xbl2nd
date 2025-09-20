/*! \file pm_sbl_boot.c
*  \n
*  \brief This file contains PMIC device initialization function where initial PMIC
*  \n SBL settings are configured through the PDM auto-generated code.
*  \n
*  \n &copy; Copyright 2013-2018 Qualcomm Technologies Inc, All Rights Reserved
*/
/* =======================================================================
Edit History
This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.


when       who     what, where, why
--------   ---     ----------------------------------------------------------
06/05/17   aab     Updated pm_config_wlan_active_and_retention_level() 
12/05/17   aab     Added support to log number of SPMI transaction
11/19/17   aab     Updated check to support RUMI with out PMIC.      
07/21/17   czq     Use Spare reg PON_PERPH_RB_SPARE instead of PON_DVDD_RB_SPARE
07/10/17   sv      Updated auto power on check API.
05/15/17   aab     Updated pm_sbl_chg_init() to call updated temp monitoring function
02/17/17   pxm     Called function to check if auto power on. 
01/11/17   pxm     add support for PBS triggered AFP
12/27/16   al      Porting fix to set LDO5 voltage with micro volt's.  
10/21/16   akm     Added pm_get_pon_reason_history, pm_pbs_header_info_init in pm_sbl_pre_config
10/07/16   aab     Call pm_sbl_config_skin_temp_threshold() for targets that require charging
09/13/16   aab     Updated logging text 
08/25/16   pxm     Config skin temperature threshold for all target
07/31/16   aab     Added num spmi transaction conditional logging
07/14/16   aab     Updated to support target with out PMI8998
06/02/16   aab     Updated pm_device_programmer_init()
03/23/16   aab     Updated pm_config_ssc_active_and_retention_level()
02/18/16   aab     Updated pm_sbl_chg_init()
02/18/16   aab     Added pm_system_init()
02/18/16   aab     Renamed pm_set_ssc_active_and_retention_levels_mv()  to pm_config_ssc_active_and_retention_level() 
02/05/16   aab     Added pm_set_ssc_active_and_retention_levels_mv()
01/25/16   aab     Updated pm_device_init() and pm_sbl_chg_init()to support RUMI targets with out PMIC 
12/22/15   aab     Added Support for PMI8998 SBL Charging
12/10/15   aab     Added Cx/Mx pvs_retention_data[]  
12/04/15   aab     Updated to support msm8998
10/14/15   aab     Branch from 8996 target
========================================================================== */
/*===========================================================================

                     INCLUDE FILES 

===========================================================================*/
#include "pm_err_flags.h"
#include "pm_boot.h"
#include "pm_ldo.h"
#include "pm_sbl_boot.h"
#include "pm_device.h"
#include "pm_config_sbl.h"
#include "pm_config_sbl_test.h"
#include "pm_app_chgr.h"
#include "pm_app_chgr_common.h"
#include "pm_pbs_info.h"
#include "pm_target_information.h"
#include "pm_sbl_boot_target.h"
#include "pm_log_utils.h"
#include "pm_pbs.h"
#include "pm_bmd.h"
#include "DALDeviceId.h"
#include "DDIPlatformInfo.h"
#include "DDIChipInfo.h"
#include "pmio_pon.h"
#include "CoreVerify.h"
#include "railway.h"
#include "pm_schg_bob.h"
#include "pm_schg_chgr.h"

/*===========================================================================

                     PROTOTYPES 

===========================================================================*/
static pm_err_flag_type pm_sbl_pre_config(void);
static pm_err_flag_type pm_sbl_target_detect(void);
static pm_err_flag_type pm_config_wlan_active_and_retention_level(void);


/*===========================================================================

                        GLOBALS and TYPE DEFINITIONS 

===========================================================================*/
static boolean pm_ram_image_loaded_flag = FALSE;
static pm_sbl_specific_data_type      *sbl_param_ptr     = NULL;
static pm_sbl_schg_specific_data_type *sbl_chg_param_ptr = NULL;

//TODO:  Remove this flag once SBL charging on MTP is verified.??
volatile boolean pm_skip_sbl_charging_flag = FALSE;

#define PMI_INDEX 1

/*===========================================================================

                        FUNCTION DEFINITIONS 

===========================================================================*/
pm_err_flag_type 
pm_device_init ( void )
{
   static pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
   uint32 initial_num_spmi_transn = pm_get_num_spmi_transaction(0);

   err_flag |= pm_device_setup();
   if( (pm_is_target_pre_silicon() == TRUE) && (pm_is_pmic_present(PMIC_A) == FALSE) )
   {
      pm_log_message("Bootup: No PMIC on RUMI Target");
      return err_flag = PM_ERR_FLAG_SUCCESS;
   }

   pm_target_information_init();
   pm_comm_info_init();

   err_flag |= pm_device_pre_init();

   err_flag |= pm_pon_init();

   err_flag |= pm_pbs_info_rom_init();  /* Read PBS INFO for the pmic rom devices */

   err_flag |= pm_sbl_pre_config(); /* SBL Pre Configuration */

   err_flag |= pm_sbl_config(); /* SBL Configuration */
   if (err_flag == PM_ERR_FLAG_SUCCESS)
   {
      pm_ram_image_loaded_flag = TRUE;
   }

   err_flag |= pm_sbl_config_test(); /* SBL Configuration validation, only executes complete code if spare reg 0x88F bit 0 is set*/

   err_flag |= pm_pbs_info_ram_init();  /* Read PBS INFO for the pmic ram devices */

   err_flag |= pm_pbs_ram_version_validation_test(); /* PBS RAM Version validation, only executes complete code if spare reg 0x88F bit 0 is set*/
   
   err_flag |= pm_device_post_init(); /* Initialize PMIC with the ones PDM can not perform */

   
  //Write to Spare bit for pm_device_init_status
   if(err_flag == PM_ERR_FLAG_SUCCESS)
   {
      err_flag = pm_comm_write_byte_mask(PMIC_A_SLAVEID_PRIM, PMIO_PON_PERPH_RB_SPARE_ADDR, PON_PERPH_RB_SPARE_DEVICE_INIT_MASK,PON_PERPH_RB_SPARE_DEVICE_INIT_MASK, 0); 
   }

   pm_log_message("Device Init # SPMI Transn: %d", pm_get_num_spmi_transaction(initial_num_spmi_transn));
   return err_flag; /* NON ZERO return means an ERROR */
}



pm_err_flag_type
pm_sbl_chg_init (void)
{
   pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
   uint32 device_index = PMIC_B; //PM855B
   DalPlatformInfoPlatformType platform_type = DALPLATFORMINFO_TYPE_UNKNOWN;
   pm_model_type pmic_model = PMIC_IS_UNKNOWN;
   uint8 pmic_index[PM_MAX_NUM_PMICS] = {0,0,0,0,0,0,0};
   uint8 slave_id[PM_MAX_NUM_PMICS] = {0,0,0,0,0,0,0};
   uint8 charger_count         = 0; 
   uint8 max_charger           = 3;
   uint8 active_pmic           = 0x1;
   boolean is_active           = FALSE;
   boolean fake_batt_present   = FALSE, batt_missing = FALSE;
   uint8 i = 0;

   //TODO:  Remove this check once SBL charging is verified on MTP
   if (pm_skip_sbl_charging_flag == TRUE)
   {
      return PM_ERR_FLAG_SUCCESS;
   }

   if (pm_is_target_pre_silicon() == TRUE)
   {
      pm_log_message("Bootup: No Charging support on RUMI Target");
      return err_flag = PM_ERR_FLAG_SUCCESS; 
   }
   
   if( (pm_is_pmic_present(PMIC_A) == FALSE) || (pm_is_pmic_present(PMIC_B) == FALSE) )
   {//To support RUMIs with out PMICs
      pm_log_message("Bootup: No Primary/Charging PMIC detected");
      return err_flag = PM_ERR_FLAG_SUCCESS; 
   }
  
   if(!(pm_driver_init_status()))
   {
      return PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
   }

   uint32 initial_num_spmi_transn = pm_get_num_spmi_transaction(0);
   platform_type = DalPlatformInfo_Platform();
   pmic_model = pm_get_pmic_model(device_index);

   if( pmic_model == PMIC_IS_SMB2351 )
   {
      err_flag |= pm_schg_chgr_get_schg_pmic_info(pmic_index, slave_id,
                                                  &charger_count, max_charger);

      for(i = 0; i < charger_count; i++)
      {
        err_flag |= pm_schg_bob_is_port_active((uint32)pmic_index[i], &is_active);
        active_pmic = is_active ? pmic_index[i] : active_pmic;
      }

      err_flag |= pm_bmd_enable((uint32)active_pmic, TRUE);

      for(i = 0; i < charger_count; i++)
      {
        //Disable fake battery detection on pmics which are not active.
        err_flag |= pm_schg_batif_enable_fake_bat_detect((uint32)pmic_index[i], (pmic_index[i] == active_pmic ? TRUE : FALSE));
      }

      // If Fake Battery is present, return without enabling charging
      err_flag |= pm_schg_batt_missing_status((uint32)active_pmic,
                          PM_SCHG_BAT_MISS_FAKE_BAT_DET, &fake_batt_present);

      DalPlatformInfoPlatformType platform_type = DalPlatformInfo_Platform();
      if (DALPLATFORMINFO_TYPE_CLS == platform_type && TRUE == fake_batt_present)
      {
        for (i = 0; i < charger_count; i++)
        {
          err_flag |= pm_schg_usb_set_usbin_suspend((uint32)pmic_index[i], TRUE);
        }
      }

      if (fake_batt_present)
      {
        pm_log_message("ChgFakeBatDet");
        return err_flag;
      }

      err_flag |= pm_schg_batt_missing_status((uint32)active_pmic,
                      PM_SCHG_BAT_MISS_BAT_ID_BMISS_CMP, &batt_missing);

      if(batt_missing)
      {
        pm_log_message("ChgBattMissing");
        return err_flag;
      }

      for(i = 0; i < charger_count; i++)
      {
        err_flag |= pm_schg_chgr_enable_src((uint32)pmic_index[i], FALSE);
      }
      for(i = 0; i < charger_count; i++)
      {
         err_flag |= pm_schg_chgr_enable_charging((uint32)pmic_index[i], TRUE);
      }

      pm_log_message("ChgEN");
   }

   pm_log_message("ChgSpmiTans: %d", pm_get_num_spmi_transaction(initial_num_spmi_transn));
   return err_flag;
}


static pm_err_flag_type
pm_sbl_pre_config(void)
{
  pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;

  if (sbl_param_ptr == NULL)
  {
     sbl_param_ptr = (pm_sbl_specific_data_type*)pm_target_information_get_specific_info(PM_PROP_SBL_SPECIFIC_DATA);
  }
  CORE_VERIFY_PTR(sbl_param_ptr);

  if (sbl_chg_param_ptr == NULL)
  {
     sbl_chg_param_ptr = (pm_sbl_schg_specific_data_type*)pm_target_information_get_specific_info(PM_PROP_SBL_CHG_SPECIFIC_DATA);
  }
  CORE_VERIFY_PTR(sbl_chg_param_ptr);

  err_flag |= pm_log_pon_reasons();

  err_flag |= pm_sbl_target_detect();

  err_flag = pm_pbs_header_info_init();
  
  pm_pbs_get_pon_reason_hist_dataset();
  
  return err_flag;
}



pm_err_flag_type
pm_sbl_target_detect(void) /* SBL Target detect */
{
   static pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS; 

   return err_flag;
}



pm_err_flag_type pm_system_init(void)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;

    err_flag = pm_config_wlan_active_and_retention_level();

    return err_flag;
}



static pm_err_flag_type 
pm_config_wlan_active_and_retention_level(void)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
    uint32 retention_voltage_mv = 0x00;
    uint8  retention_level[2] = {0};
    uint32 active_voltage_mv = 0x00;
    uint8  active_level[2] = {0};
    uint8  rail = rail_id("vdda_wlan");

    //Set Retention Voltage
    retention_voltage_mv = railway_get_corner_voltage(rail,  RAILWAY_RETENTION) / 1000; //get Retention voltage level
    retention_level[0]  =  (uint8)( retention_voltage_mv & 0xFF);
    retention_level[1]  =  (uint8)((retention_voltage_mv >> 8) & 0xFF);
    err_flag |= pm_comm_write_byte_array(SDAM3_SLAVE_ADDR, SDAM3_VSET2_WLAN_RET_ADDR_LB, 2, retention_level, 1);

    //Set Active Voltage
    active_voltage_mv = railway_get_corner_voltage(rail,  RAILWAY_NOMINAL ) / 1000; //get Active voltage level
    active_level[0] = (uint8)( active_voltage_mv & 0xFF);      
    active_level[1] = (uint8)((active_voltage_mv >> 8) & 0xFF);
    err_flag |= pm_comm_write_byte_array(SDAM3_SLAVE_ADDR, SDAM3_VSET1_WLAN_ACTIVE_ADDR_LB, 2, active_level, 1);


    //Configure WCSS rail, L1E, with Active Level
    err_flag |= pm_ldo_volt_level(PMIC_E, PM_LDO_1, active_voltage_mv * 1000, TRUE);

    return err_flag;
}

