/*! \file
*  
*  \brief  pm_app_schgr.c
*  \details Implementation file for pmic sbl charging algorithm
*    
*  \n &copy; Copyright (c)2016-2018  by QUALCOMM Technologies Incorporated, All Rights Reserved
*/

/*===========================================================================
                                Edit History
This document is created by a code generator, therefore this section will
not contain comments describing changes made to the module.

when       who     what, where, why
--------   ---     ----------------------------------------------------------
06/28/18   pxm     Add AFP configuration
05/07/18   pxm     Remove auto power check function as PM855B already fixed it.
12/22/17   pxm     Change SRAM access to DMA support
07/19/17   aab     Remove pm_app_chgr_set_hw_afp_thresholds(), moved functionality to common code
06/28/17   sv      Updated pm_app_auto_power_on_check API name
04/25/17   sv      Updated pm_sbl_auto_power_on_check with pon reason.
07/10/17   pxm     Move PmicFgSram_Init to pm_sbl_bool_oem.c. 
03/15/17   aab     Remove bat id conversion config from pm_sbl_enable_batt_detection()
03/14/17   pxm     Need Manually setting DCP ICL to 1500mA. 
02/17/17   pxm     Added function to check if auto power on.
02/02/17   cs      SBL needs to set the batt id missing threshold to 0 to get max threshold.
01/19/17   cs      let UEFI enable battery missing detection to add more delay before enable batt missing.
01/11/17   pxm     add support for PBS triggered AFP
12/16/16   pxm     Added fake batt_id configuration function
10/20/16   aab     Added Batt_therm Out of bound temperature check
10/13/16   pxm     Pet (reset) BARK and BITE watchdog timers in charging loop
09/30/16   aab     Updated logging:  Used  pm_log_messag() macro
08/25/16   pxm     Add skin temperature threshold configuration function 
09/17/16   aab     Updated pm_sbl_schg_get_charger_progress_status() to latest WiPower Algorithm
09/17/16   aab     Removed toggling of SHDN_N_CLEAR_CMD Reg when DC_IN path detected 
08/12/16   aab     Added debugboard detection support 
08/02/16   yps     Fixed compiling issue for Clang38
07/26/16   aab     Updated pm_sbl_schg_config_red_led()
07/07/16   aab     Updated to support saving Entry Vbatt/SOC state to smem
05/27/16   pxm     Implement wipower power customization
03/15/16   aab     Driver update (Support for PMI8998)
02/16/16   aab     Creation
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES 

===========================================================================*/
#include "com_dtypes.h"
#include "boothw_target.h"
#include "boot_api.h"
#include "CoreVerify.h"

#include "pm_chg.h"
#include "pm_app_chgr.h"
#include "pm_schg_chgr.h"
#include "pm_schg_batif.h"
#include "pm_schg_dc.h"
#include "pm_schg_dcdc.h"
#include "pm_schg_misc.h"
#include "pm_fg_batt_info.h"
#include "pm_fg_batt_soc.h"
#include "pm_fg_sram.h"
#include "pm_fg_adc_rr.h"
#include "pm_pbs_client.h"
#include "pm_bmd.h"

#include "pm_comm.h"
#include "pm_utils.h"
#include "pm_log_utils.h"
#include "pm_rgb.h"
#include "pm_target_information.h"
#include "smem.h"
#include "DDIAdc.h"


/*=========================================================================== 
 
                     GLOBAL TYPE DEFINITIONS
 
===========================================================================*/
#define  PM_WEAK_BATTERY_CHARGING_DELAY         500 * 1000   //500ms
#define  PM_WIPOWER_START_CHARGING_DELAY       3500 * 1000   //3.5sec
#define  PM_WEAK_BATTERY_CHARGING_AFP_DELAY     500 * 1000   //500ms
#define  PM_FAKE_BATT_ID_CFG_WAIT_POLL           50 * 1000   //50ms
#define  PM_FAKE_BATT_ID_CFG_WAIT_MAX          1200 * 1000   //1.2sec

#define  PM_SBL_WP_CHG_POWER_MAX_UW           20000000
#define  PM_SBL_WP_CHG_POWER_MIN_UW           5000000
#define  PM_SBL_DC_ZIN_ICL_PT_VOLT_MV         8000
#define  PM_SBL_DC_ZIN_ICL_PT_HV_VOLT_MV      9000
#define  PM_SBL_DC_ZIN_ICL_LV_VOLT_MV         5500
#define  PM_SBL_DC_ZIN_ICL_MID_LV_VOLT_MV     6500
#define  PM_SBL_DC_ZIN_ICL_MID_HV_VOLT_MV     8000
#define  PM_SBL_DC_ZIN_ICL_HV_VOLT_MV         11000

static pm_sbl_schg_vlowbatt_threshold_data_type pm_dbc_bootup_volt_threshold;
static boolean verbose_chg_log = TRUE;        //TBD: Needs update sbl charging logging option.  Probably need to use boot-profiler which keeps log messages default off 
static pm_sbl_schg_info_type schg_info;
static pm_schg_usb_apsd_result_status_type chgr_src_detected;
static uint64 wipower_chg_wait_time_ms = 0x00;

/*===========================================================================

                     PROTOTYPES 

===========================================================================*/
pm_err_flag_type pm_sbl_schg_config_charger(uint32 device_index, boolean chg_prog_message_flag);
pm_err_flag_type pm_sbl_schg_get_charger_progress_status(uint32 device_index, pm_schg_misc_pwr_pth_type chgr_power_path,
                                                         pm_sbl_schg_wipower_specific_data_type *wipower_chg_param_ptr,
                                                         boolean *chg_status);
pm_err_flag_type pm_sbl_schg_config_red_led(uint32 device_index, boolean toggle_led, boolean delay);
pm_err_flag_type pm_sbl_config_wipower_chg_ilimit(uint32 device_index, uint32 wipower_chg_power_uw);


/*===========================================================================

                     FUNCTION IMPLEMENTATION 

===========================================================================*/
pm_err_flag_type pm_sbl_chg_check_weak_battery_status(uint32 device_index)
{
   pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;

   uint32  vbat_adc = 0;
   uint32  entry_vbat_adc = 0;
   int32   ibat_adc = 0;
   uint32  bootup_threshold;
   boolean apsd_done = FALSE;

   boolean vbatt_weak_status        = TRUE;
   boolean toggle_led               = FALSE;
   boolean chg_progress_status      = FALSE;
   boolean chg_prog_message_flag    = FALSE;
   boolean usbin_present            = FALSE;
   pm_schg_misc_pwr_pth_type  charger_power_path = PM_SCHG_MISC_POWER_PATH__INVALID;

   pm_sbl_schg_specific_data_type *chg_param_ptr = (pm_sbl_schg_specific_data_type*)pm_target_information_get_specific_info(PM_PROP_SBL_CHG_SPECIFIC_DATA);
   CORE_VERIFY_PTR(chg_param_ptr);
   verbose_chg_log = chg_param_ptr->verbose_uart_logging;

   pm_sbl_schg_wipower_specific_data_type *wipower_chg_param_ptr = (pm_sbl_schg_wipower_specific_data_type*)pm_target_information_get_specific_info(PM_PROP_SBL_CHG_WIPOWER_SPECIFIC_DATA);
   CORE_VERIFY_PTR(wipower_chg_param_ptr);

   pm_rgb_pmic_index_type *rgb_index_ptr = (pm_rgb_pmic_index_type*)pm_target_information_get_specific_info(PM_PROP_RGB_PMIC_INDEX);
   CORE_VERIFY_PTR(rgb_index_ptr);

   if(chg_param_ptr->dbc_bootup_volt_threshold.enable_config == PM_ENABLE_CONFIG)
   {
      //Configure Vlowbatt threshold: Used by PMI on next bootup
      pm_dbc_bootup_volt_threshold = (pm_sbl_schg_vlowbatt_threshold_data_type)chg_param_ptr->dbc_bootup_volt_threshold;
      err_flag  |= pm_sbl_chg_config_vbat_low_threshold(device_index, pm_dbc_bootup_volt_threshold); 
   }

   bootup_threshold = chg_param_ptr->bootup_battery_theshold_mv;

   err_flag |= pm_fg_batt_soc_enable_fg_algorithm(device_index, TRUE);  
   err_flag |= pm_fg_batt_info_get_vbatt(device_index, &schg_info.sbl_entry_mV);
   entry_vbat_adc = (uint32)schg_info.sbl_entry_mV;
   if(verbose_chg_log)
   {
      pm_log_message("EntryVbatt: %d",entry_vbat_adc);
   }

   //APSD rerun
   err_flag |= pm_schg_usb_get_apsd_result_status(device_index, &chgr_src_detected); 
   if ( err_flag != PM_ERR_FLAG_SUCCESS )   
   {    
      if(chgr_src_detected == PM_SCHG_USB_APSD_RESULT_STATUS_INVALID) 

      return err_flag; 
   } 

   pm_log_message("APSD result: %d", chgr_src_detected); 

   //Get APSD Done status 
   err_flag |= pm_schg_usb_get_apsd_status(device_index, PM_SCHG_USB_APSD_STATUS_APSD_DTC_STATUS_DONE, &apsd_done); 
   if (FALSE == apsd_done) 
   { 
      pm_log_message("APSD in progress", 0); 
   } 
   else if(chgr_src_detected == PM_SCHG_USB_APSD_RESULT_STATUS_INVALID)
   {
      err_flag |= pm_schg_usb_irq_status(device_index, PM_SCHG_USB_IRQ_USBIN_PLUGIN, PM_IRQ_STATUS_RT, &usbin_present);
      if(usbin_present == TRUE) 
      {
         pm_log_message("APSD re-run", 0);
         err_flag |= pm_schg_usb_rerun_apsd(device_index);
      }
      if ( err_flag != PM_ERR_FLAG_SUCCESS )  
      { 
         return err_flag;
      }
   }

   while( vbatt_weak_status == TRUE )  //While battery is in weak state
   {
      //pet charger WDOG
      err_flag |= pm_schg_misc_bark_bite_wdog_pet(device_index);

      err_flag |= pm_fg_batt_info_get_vbatt(device_index, &vbat_adc);
      err_flag |= pm_fg_batt_info_get_ibatt(device_index, &ibat_adc);
      if ( err_flag != PM_ERR_FLAG_SUCCESS )  
      { 
          return err_flag;
      }

      pm_log_message("Vbatt: %d; Ibatt: %d", vbat_adc, ibat_adc);
      //Check if ADC reading is within limit
      if ( vbat_adc >=  bootup_threshold)  //Compare it with SW bootup threshold
      {
         vbatt_weak_status = FALSE;
         // Enable charging if both charger and battery present
         if(chgr_src_detected != PM_SCHG_USB_APSD_RESULT_STATUS_INVALID)
         {
            err_flag |= pm_schg_chgr_enable_charging(device_index, TRUE);   //Enable Charging
         }
         break; //BOOTUP
      }

      err_flag |= pm_sbl_schg_config_charger(device_index, chg_prog_message_flag);   //Detect charger type and configure

      err_flag |= pm_sbl_schg_get_charger_progress_status(device_index, charger_power_path, wipower_chg_param_ptr, &chg_progress_status);
      if (chg_progress_status == FALSE)
      {
         pm_log_message("Not charging. Shutdown");
         boot_hw_powerdown();
      }
      else
      {
         pm_log_message("Charging in progress");
         chg_prog_message_flag = TRUE;
      }

      //Toggle Red LED
      toggle_led = (toggle_led == FALSE) ? TRUE : FALSE;
      err_flag |= pm_sbl_schg_config_red_led(rgb_index_ptr->charger, toggle_led, TRUE);

   }//while

   toggle_led = FALSE;
   err_flag |= pm_sbl_schg_config_red_led(rgb_index_ptr->charger, toggle_led, FALSE); //Ensure LED is turned of at the end of SBL charging

   if (err_flag != PM_ERR_FLAG_SUCCESS)
   {
      pm_log_message("ERROR, In SBL Charging ...");
   }
   else if (chg_prog_message_flag == TRUE)
   {
      pm_log_message("ExitVbatt: %d", vbat_adc);
      if(verbose_chg_log)
      {
         pm_log_message("SBL Charging completed.");
      }
   }

   return err_flag; 
}

pm_err_flag_type 
pm_sbl_fake_batt_id_range_config(uint32 device_index, pm_sbl_schg_specific_data_type *chg_param_ptr)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
    uint32 fake_batt_id_min_ohms = 0;
    pm_device_info_type pmic_device_info;
    uint32 fake_batt_id_max_ohms = 0;

    CORE_VERIFY_PTR(chg_param_ptr);

    //Check if enable fake battery id configuration
    if(chg_param_ptr->fake_batt_id_range.enable_config == PM_ENABLE_CONFIG)
    {
        fake_batt_id_min_ohms = chg_param_ptr->fake_batt_id_range.min_value;
        fake_batt_id_max_ohms = chg_param_ptr->fake_batt_id_range.max_value;

        if(fake_batt_id_max_ohms > PM_FG_ADC_RR_FG_FAKE_BATT_MAX || fake_batt_id_max_ohms < fake_batt_id_min_ohms)
        {
            pm_log_message("ERROR: FG fake_batt_id out of range");
        }
        else
        {
            uint32 fake_batt_id_min_code = 0;
            uint32 fake_batt_id_max_code = 0;
            err_flag |= pm_get_adc_code_of_battery_id(chg_param_ptr->pull_up, fake_batt_id_min_ohms, &fake_batt_id_min_code);
            err_flag |= pm_get_adc_code_of_battery_id(chg_param_ptr->pull_up, fake_batt_id_max_ohms, &fake_batt_id_max_code);

            err_flag |= pm_fg_adc_rr_set_fake_batt_id_range(device_index, fake_batt_id_min_code, fake_batt_id_max_code);
            err_flag |= pm_get_pmic_info(device_index, &pmic_device_info);
            if (err_flag != PM_ERR_FLAG_SUCCESS)
            {
                 return err_flag;
            }

            err_flag |= pm_fg_adc_rr_clear_channel_log(device_index);
            if (err_flag != PM_ERR_FLAG_SUCCESS)
            {
                pm_log_message("ERROR, pm_sbl_fake_batt_range_change()");
                return err_flag;
            }
        }
    }

    return err_flag; 
}

#define VSYSMIN_CFG_MAX 3400
static pm_err_flag_type pm_sbl_set_max_vsysmin(uint32 device_index)
{
    return pm_schg_dcdc_configure_vsysmin(device_index, VSYSMIN_CFG_MAX);
}

pm_err_flag_type pm_sbl_boot_without_battery_config(uint32 device_index)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;

    err_flag |= pm_schg_chgr_enable_charging(device_index, FALSE); // Disable charging
    err_flag |= pm_schg_usb_aicl_options_cfg(device_index, PM_SCHG_USB_AICL_CFG_AICL_EN, FALSE); // Disable AICL
    err_flag |= pm_sbl_set_max_vsysmin(device_index); // Increase VSYS_MIN to maximum (3.4V)
    err_flag |= pm_schg_usb_configure_usbin_icl_options(device_index, PM_SCHG_USB_ICL_OPTIONS_USBIN_MODE_CHG, TRUE); // Set to HC mode Current level
    err_flag |= pm_schg_usb_set_usbin_current_limit_cfg(device_index, 3000); // Force 3A input current limit

    return err_flag;
}

pm_err_flag_type 
pm_sbl_get_debug_board_presence(uint32 device_index, pm_sbl_schg_specific_data_type *chg_param_ptr, boolean *debug_board_present)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
    pm_device_info_type pmic_device_info;
    uint32 batt_id_ohms = 0;

    CORE_VERIFY_PTR(chg_param_ptr);
    CORE_VERIFY_PTR(debug_board_present);

    err_flag = pm_sbl_get_batt_id(chg_param_ptr->pull_up, &batt_id_ohms);
    err_flag |= pm_get_pmic_info(device_index, &pmic_device_info);
    if(err_flag != PM_ERR_FLAG_SUCCESS)
    {
        return err_flag;
    }

    pm_log_message("battery Id: %d", batt_id_ohms);

    // The multi-time ADC read is a ADC workaround on PM855B v1.0
    // It was quite possible battery ID may get around 7k at the first time boot up after xVdd lost.
    // Issue is quite easy to reproduce on QRD855.
    // Test this work around out at PM855B v2.0 and remove it on next target which doesn't use V1.0 PM855B
    if(pmic_device_info.ePmicModel == PMIC_IS_PM855B &&
       pmic_device_info.nPmicAllLayerRevision == 1 &&
       pmic_device_info.nPmicMetalRevision == 0 &&
       batt_id_ohms <= chg_param_ptr->debug_board_batt_id_pd_res_range.max_value)//PM855B V1.0 batt_id mis-detect issue
    {
        for(uint8 i = 0; i < 16; ++i) // Deblay for at most 80ms
        {
            err_flag |= pm_sbl_get_batt_id(chg_param_ptr->pull_up, &batt_id_ohms);
            err_flag |= pm_busy_wait(5 * 1000);
            if(batt_id_ohms > chg_param_ptr->debug_board_batt_id_pd_res_range.max_value)
            {
                pm_log_message("battery Id: %d at %d times ADC reading", batt_id_ohms, i + 2);
                break;
            }
        }
    }

    if((batt_id_ohms >= chg_param_ptr->debug_board_batt_id_pd_res_range.min_vlue) &&
        (batt_id_ohms <= chg_param_ptr->debug_board_batt_id_pd_res_range.max_value))
    {
        *debug_board_present = TRUE;
        pm_log_message("Debug Board detected"); 
    }
    else
    {
        *debug_board_present = FALSE;
    }

    return err_flag; 
}

pm_err_flag_type pm_sbl_schg_config_charger(uint32 device_index, boolean chg_prog_message_flag)
{
   pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;

   //Check if USB charger is SDP
   err_flag |= pm_schg_usb_get_apsd_result_status(device_index, &chgr_src_detected);
   switch(chgr_src_detected)
   {
      case PM_SCHG_USB_APSD_RESULT_STATUS_SDP_CHARGER:
      {
         pm_log_message("Charger SRC: SDP");
      }
      break;
      case PM_SCHG_USB_APSD_RESULT_STATUS_OCP_CHARGER:
      {
         pm_log_message("Charger SRC: OCP");
      }
      break;
      case PM_SCHG_USB_APSD_RESULT_STATUS_CDP_CHARGER:
      {
         pm_log_message("Charger SRC: CDP");
      }
      break;
      case PM_SCHG_USB_APSD_RESULT_STATUS_DCP_CHARGER:
      {
         pm_log_message("Charger SRC: DCP");
      }
      break;
      case PM_SCHG_USB_APSD_RESULT_STATUS_FLOAT_CHARGER:
      {
         pm_log_message("Charger SRC: FLOAT");
      }
      break;
      case PM_SCHG_USB_APSD_RESULT_STATUS_QC_2P0:
      {
         pm_log_message("Charger SRC: QC_2P0");
      }
      break;
      case PM_SCHG_USB_APSD_RESULT_STATUS_QC_3P0:
      {
         pm_log_message("Charger SRC: QC_3P0");
      }
      break;
      default:
      {
         pm_log_message("Charger SRC: OTHER; Type: %d", chgr_src_detected);
      }
      break;
    }

    if (chg_prog_message_flag == FALSE)
    {
        //Ensure that Charging is enabled
       err_flag |= pm_schg_chgr_enable_charging(device_index, TRUE);
       err_flag |= pm_busy_wait(PM_WEAK_BATTERY_CHARGING_DELAY);
    }
   return err_flag;
}


pm_err_flag_type pm_sbl_schg_get_charger_progress_status(uint32 device_index, 
                                                         pm_schg_misc_pwr_pth_type chgr_power_path,
                                                         pm_sbl_schg_wipower_specific_data_type *wipower_chg_param_ptr, 
                                                         boolean *chg_progress_status)
{
   pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
   pm_schg_chgr_status_type vbatt_chging_status;

   //Check if Charging in progress
   err_flag |= pm_schg_chgr_get_chgr_status(device_index, &vbatt_chging_status);
   if( vbatt_chging_status.status_5.charging_enable == FALSE )//If charging is not in progress
   {
      if (chgr_power_path == PM_SCHG_MISC_POWER_PATH__DCIN) 
      { //WiPower Charger detected
         if(wipower_chg_param_ptr->wipower_chg_wait.enable_config == PM_DISABLE_CONFIG)//if WiPower wait is disabled.
         {
            *chg_progress_status = TRUE; //wait indefinate time 
         }
         else
         { //enable_config == PM_DISABLE_CONFIG
            //Delay a specific amount of time (while looping)for charging to begin, and check charging status again prior to shutting down.
            wipower_chg_wait_time_ms += 500; //approximate time for one iteration
            if ( wipower_chg_wait_time_ms > wipower_chg_param_ptr->wipower_chg_wait.wait_time_ms )
            {
               err_flag |= pm_schg_chgr_get_chgr_status(device_index, &vbatt_chging_status);
               if(vbatt_chging_status.status_5.charging_enable == FALSE )
               {
                  *chg_progress_status = FALSE;
               }
               else
               {
                  *chg_progress_status = TRUE;
               }
            }
         }
      }
      else
      {
         *chg_progress_status = FALSE; 
      }
    }
    else
    {
       *chg_progress_status = TRUE;
    }

   return err_flag;
}



pm_err_flag_type pm_sbl_schg_config_red_led(uint32 device_index, boolean toggle_led, boolean delay)
{
   pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;

   err_flag |= pm_rgb_led_config(device_index, PM_RGB_1, PM_RGB_SEGMENT_R,  PM_RGB_DIM_LEVEL_MID, toggle_led); 
   if(delay)
   {
      err_flag |= pm_busy_wait(PM_WEAK_BATTERY_CHARGING_DELAY); 
   }

   return err_flag;
}

pm_err_flag_type pm_sbl_chg_config_vbat_low_threshold(uint32 device_index, pm_sbl_schg_vlowbatt_threshold_data_type vlowbatt_threshold_data)
{
   pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;

   if (vlowbatt_threshold_data.enable_config == PM_ENABLE_CONFIG)
   {
      if (vlowbatt_threshold_data.vlowbatt_threshold  >= PM_SCHG_BATIF_LOW_BATTERY_THRESH_INVALID)
      {
         err_flag = PM_ERR_FLAG_INVALID_PERIPHERAL;
         return err_flag;
      }

      err_flag = pm_schg_batif_set_low_batt_detect_threshold(device_index, vlowbatt_threshold_data.vlowbatt_threshold);
   }
   return err_flag; 
}



pm_err_flag_type pm_sbl_config_charger_and_skin_temp_threshold(uint32 device_index)
{
   pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
   int skin_temp_threshold = 0;
   int charger_temp_threshold = 0;

   static pm_sbl_schg_specific_data_type *chg_param_ptr = NULL;
   
   if (chg_param_ptr == NULL)
   {
      chg_param_ptr = (pm_sbl_schg_specific_data_type*)pm_target_information_get_specific_info(PM_PROP_SBL_CHG_SPECIFIC_DATA);
      CORE_VERIFY_PTR(chg_param_ptr);
   }
   
   //Fg skin hot threshold            
   if (chg_param_ptr->fg_skin_hot_threshold.enable_config == PM_ENABLE_CONFIG)
   {
      skin_temp_threshold = chg_param_ptr->fg_skin_hot_threshold.temp_threshold;
      
      if ((skin_temp_threshold >= PM_FG_ADC_RR_FG_CHARGER_SKIN_TEMP_MIN) && (skin_temp_threshold <= PM_FG_ADC_RR_FG_CHARGER_SKIN_TEMP_MAX))
      {
         err_flag |= pm_sbl_daladc_fg_set_temp_threshold(device_index,  ADC_FG_THRESH_SKIN_HOT, skin_temp_threshold);
      }
      else
      {
         err_flag |= PM_ERR_FLAG_INVALID_PARAMETER;
         pm_log_message("ERR: FG skin hot threshold out of range");
      }
   }

   //Fg skin too hot threshold            
   if (chg_param_ptr->fg_skin_too_hot_threshold.enable_config == PM_ENABLE_CONFIG)
   {

      skin_temp_threshold = chg_param_ptr->fg_skin_too_hot_threshold.temp_threshold;
          
      if ((skin_temp_threshold >= PM_FG_ADC_RR_FG_CHARGER_SKIN_TEMP_MIN) && (skin_temp_threshold <= PM_FG_ADC_RR_FG_CHARGER_SKIN_TEMP_MAX))
      {
         err_flag |= pm_sbl_daladc_fg_set_temp_threshold(device_index,  ADC_FG_THRESH_SKIN_TOO_HOT, skin_temp_threshold);
      }
      else
      {
         err_flag |= PM_ERR_FLAG_INVALID_PARAMETER;
         pm_log_message("ERR: FG skin too hot threshold out of range");
      }  
    }
   
    //FG charger hot threshold 
    if (chg_param_ptr->fg_charger_hot_threshold.enable_config == PM_ENABLE_CONFIG) 
    {
       charger_temp_threshold = chg_param_ptr->fg_charger_hot_threshold.temp_threshold; 
       if ((charger_temp_threshold >= PM_FG_ADC_RR_FG_CHARGER_SKIN_TEMP_MIN) && (charger_temp_threshold <= PM_FG_ADC_RR_FG_CHARGER_SKIN_TEMP_MAX)) 
       {
          err_flag |= pm_sbl_daladc_fg_set_temp_threshold(device_index, ADC_FG_THRESH_CHARGER_HOT, charger_temp_threshold); 
       }
       else
       {
          err_flag |= PM_ERR_FLAG_INVALID_PARAMETER; 
          pm_log_message("ERR: FG charger hot threshold out of range"); 
       }
    }
   
   
    //FG Charger too hot thereshold 
    if (chg_param_ptr->fg_charger_too_hot_threshold.enable_config == PM_ENABLE_CONFIG) 
    {
       charger_temp_threshold = chg_param_ptr->fg_charger_too_hot_threshold.temp_threshold; 
       if ((charger_temp_threshold >= PM_FG_ADC_RR_FG_CHARGER_SKIN_TEMP_MIN) && (charger_temp_threshold <= PM_FG_ADC_RR_FG_CHARGER_SKIN_TEMP_MAX)) 
       {
          err_flag |= pm_sbl_daladc_fg_set_temp_threshold(device_index, ADC_FG_THRESH_CHARGER_TOO_HOT, charger_temp_threshold); 
       }
       else
       {
          err_flag |= PM_ERR_FLAG_INVALID_PARAMETER; 
          pm_log_message("ERR: FG charger too hot threshold out of range"); 
       } 
    } 
    return err_flag; 
}

pm_err_flag_type pm_sbl_config_wipower_chg_ilimit(uint32 device_index, uint32 wipower_chg_power_uw)
{
   pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
   uint32 ilmit_level_ma = 0;

   //Configure PM_SCHG_DC_ZIN_ICL_PT;  Use 8V :  W=VI
   ilmit_level_ma = wipower_chg_power_uw / PM_SBL_DC_ZIN_ICL_PT_VOLT_MV;
   err_flag |= pm_schg_dc_set_zin_icl(device_index, PM_SCHG_DC_ZIN_ICL_PT, ilmit_level_ma);

   //Configure PM_SCHG_DC_ZIN_ICL_PT_HV;  9V 
   ilmit_level_ma = wipower_chg_power_uw / PM_SBL_DC_ZIN_ICL_PT_HV_VOLT_MV;
   err_flag |= pm_schg_dc_set_zin_icl(device_index, PM_SCHG_DC_ZIN_ICL_PT_HV, ilmit_level_ma);

   //Configure PM_SCHG_DC_ZIN_ICL_LV; Use 5.5V 
   ilmit_level_ma = wipower_chg_power_uw / PM_SBL_DC_ZIN_ICL_LV_VOLT_MV;
   err_flag |= pm_schg_dc_set_zin_icl(device_index, PM_SCHG_DC_ZIN_ICL_LV, ilmit_level_ma); 

   //Configure PM_SCHG_DC_ZIN_ICL_MID_LV; Use 6.5V 
   ilmit_level_ma = wipower_chg_power_uw / PM_SBL_DC_ZIN_ICL_MID_LV_VOLT_MV;
   err_flag |= pm_schg_dc_set_zin_icl(device_index, PM_SCHG_DC_ZIN_ICL_MID_LV, ilmit_level_ma);

   //Configure PM_SCHG_DC_ZIN_ICL_MID_HV; Use 8V 
   ilmit_level_ma = wipower_chg_power_uw / PM_SBL_DC_ZIN_ICL_MID_HV_VOLT_MV;
   err_flag |= pm_schg_dc_set_zin_icl(device_index, PM_SCHG_DC_ZIN_ICL_MID_HV, ilmit_level_ma);

   //Configure PM_SCHG_DC_ZIN_ICL_HV; Use 11V 
   ilmit_level_ma = wipower_chg_power_uw / PM_SBL_DC_ZIN_ICL_HV_VOLT_MV;
   err_flag |= pm_schg_dc_set_zin_icl(device_index, PM_SCHG_DC_ZIN_ICL_HV, ilmit_level_ma);

   return err_flag;
}

pm_err_flag_type pm_sbl_config_afp(uint32 device_index)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
    uint32 code;

    // Do NOT configure AFP for PM8150B v1.0.
    // ADC doesn't work well on v1.0 hence AFP may be worngly triggerred
    pm_device_info_type pmic_device_info;
    err_flag |= pm_get_pmic_info(device_index, &pmic_device_info);
    if(err_flag != PM_ERR_FLAG_SUCCESS)
    {
        return err_flag;
    }

    if(pmic_device_info.ePmicModel == PMIC_IS_PM855B &&
       pmic_device_info.nPmicAllLayerRevision == 1 &&
       pmic_device_info.nPmicMetalRevision == 0)
    {
      return PM_ERR_FLAG_SUCCESS;
    }

    // Get afp configuration from pm_config_target.c via DAL
    pm_sbl_schg_afp_type *afp_config = (pm_sbl_schg_afp_type*) pm_target_information_get_specific_info(PM_PROP_AFP_CONFIG);
    CORE_VERIFY_PTR(afp_config);

    // Calculate cold threshold code
    err_flag = pm_get_adc_code_of_battery_temp(afp_config->afp_too_cold_threshold, &code);
    if(err_flag != PM_ERR_FLAG_SUCCESS)
    {
        return err_flag;
    }

    // Configure too cold afp trigger threshold
    err_flag = pm_schg_chgr_set_jeita_threshold_value(device_index, PM_SCHG_CHGR_JEITA_THRESHOLD_TOO_COLD_AFP, code);
    if(err_flag != PM_ERR_FLAG_SUCCESS)
    {
        return err_flag;
    }

    // Calculate hot threshold code
    err_flag = pm_get_adc_code_of_battery_temp(afp_config->afp_too_hot_threshold, &code);
    if(err_flag != PM_ERR_FLAG_SUCCESS)
    {
        return err_flag;
    }

    // Configure too hot afp trigger threshold
    err_flag = pm_schg_chgr_set_jeita_threshold_value(device_index, PM_SCHG_CHGR_JEITA_THRESHOLD_TOO_HOT_AFP, code);
    if(err_flag != PM_ERR_FLAG_SUCCESS)
    {
        return err_flag;
    }

    // enable/disable AFP cold/hot configure
    err_flag = pm_schg_chgr_jeita_en_cfg(device_index, PM_SCHG_CHGR_JEITA_AFP_COLD, afp_config->enable_cold_afp_trigger);
    err_flag |= pm_schg_chgr_jeita_en_cfg(device_index, PM_SCHG_CHGR_JEITA_AFP_HOT, afp_config->enable_hot_afp_trigger);

    return err_flag;
}

pm_err_flag_type pm_sbl_config_chg_parameters(uint32 device_index)
{
   pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;

   static pm_sbl_schg_specific_data_type *chg_param_ptr;
   static pm_sbl_schg_wipower_specific_data_type *wipower_chg_param_ptr = NULL;

   if(chg_param_ptr == NULL)
   {
      chg_param_ptr = (pm_sbl_schg_specific_data_type*)pm_target_information_get_specific_info(PM_PROP_SBL_CHG_SPECIFIC_DATA);
      CORE_VERIFY_PTR(chg_param_ptr);
   }

   if(wipower_chg_param_ptr == NULL)
   {
      wipower_chg_param_ptr = (pm_sbl_schg_wipower_specific_data_type*)pm_target_information_get_specific_info(PM_PROP_SBL_CHG_WIPOWER_SPECIFIC_DATA);
      CORE_VERIFY_PTR(wipower_chg_param_ptr);
   }

   //Vlowbatt Threshold  
   //  - Done on:  pm_sbl_chg_config_vbat_low_threshold()

   //Battery Missing Detection Source 
   if (chg_param_ptr->bat_miss_detect_src.enable_config == PM_ENABLE_CONFIG)
   {
      boolean en_bat_missing_via_batid_therm_pin = chg_param_ptr->bat_miss_detect_src.en_bat_missing_via_batid_therm_pin;
      err_flag |= pm_schg_batif_cfg_batt_missing_src(device_index, PM_SCHG_BAT_MISS_SRC_CFG__PIN_SRC_EN, en_bat_missing_via_batid_therm_pin);
   }

   //WDOG Timeout      
   if (chg_param_ptr->wdog_timeout.enable_config == PM_ENABLE_CONFIG)
   {
      pm_schg_misc_sbb_wd_cfg wdog_timeout = chg_param_ptr->wdog_timeout.wdog_timeout;

      if( (wdog_timeout.snarl_wdog_timeout < PM_SCHG_MISC_SNARL_WDOG_TMOUT_INVALID) ||
          (wdog_timeout.bark_wdog_timeout  < PM_SCHG_MISC_BARK_WDOG_TMOUT_INVALID)  ||
          (wdog_timeout.bite_wdog_timeout  < PM_SCHG_MISC_BITE_WDOG_TMOUT_INVALID) )
      {
         err_flag |= pm_schg_misc_set_snarl_bark_bite_wd_cfg(device_index, wdog_timeout);
      }
      else
      {
         err_flag |= PM_ERR_FLAG_INVALID_PARAMETER;
      }
   }


   //FAST Charging Current            
   if (chg_param_ptr->fast_chg_i.enable_config == PM_ENABLE_CONFIG)
   {

      uint32 fast_chg_i_ma = chg_param_ptr->fast_chg_i.fast_chg_i_ma;
      if ((fast_chg_i_ma >= 300) && (fast_chg_i_ma <= 3000) )
      {
          err_flag |= pm_schg_chgr_set_charge_current(device_index, PM_SCHG_CHGR_FAST_CHARGE_CURRENT, fast_chg_i_ma);
      }
      else
      {
         err_flag |= PM_ERR_FLAG_INVALID_PARAMETER;
      }
   }

   //Pre Charge Current               
   if (chg_param_ptr->pre_chg_i.enable_config == PM_ENABLE_CONFIG)
   {
      uint32 pre_chg_i_ma = chg_param_ptr->pre_chg_i.pre_chg_i_ma;
      if ((pre_chg_i_ma >= 100) && (pre_chg_i_ma <= 550) )
      {
          err_flag |= pm_schg_chgr_set_charge_current(device_index, PM_SCHG_CHGR_PRE_CHARGE_CURRENT, pre_chg_i_ma);
      }
      else
      {
         err_flag |= PM_ERR_FLAG_INVALID_PARAMETER;
      }
   }


   //Float Voltage : 3600mV to 4500 mv                   
   if (chg_param_ptr->float_volt_theshold_mv.enable_config == PM_ENABLE_CONFIG)
   {
      uint32 float_volt_mv = chg_param_ptr->float_volt_theshold_mv.float_volt_theshold_mv;
      if ((float_volt_mv >= 3600) && (float_volt_mv <= 4500))
      {
          err_flag |= pm_schg_chgr_set_float_volt(device_index, float_volt_mv);
      }
      else
      {
         err_flag |= PM_ERR_FLAG_INVALID_PARAMETER;
      }
   }


   //USBIN Input Current Limit  :Valid value is 300 to 3000mAmp      
   if (chg_param_ptr->usbin_input_current_limit.enable_config == PM_ENABLE_CONFIG)
   {
      uint32 usbin_i_limit_ma = chg_param_ptr->usbin_input_current_limit.usbin_input_current_limit;
      if ((usbin_i_limit_ma >= 300) && (usbin_i_limit_ma <= 3000))
      {
          err_flag |= pm_schg_usb_set_usbin_current_limit_cfg(device_index, usbin_i_limit_ma);
      }
      else
      {
         err_flag |= PM_ERR_FLAG_INVALID_PARAMETER;
      }
   }


   //DCIN Input Current Limit : valid range is 300 to 2000 mAmp         
   if (chg_param_ptr->dcin_input_current_limit.enable_config == PM_ENABLE_CONFIG)
   {
      uint32 dcin_i_limit_ma = chg_param_ptr->dcin_input_current_limit.dcin_input_current_limit;
      if ((dcin_i_limit_ma >= 300) && (dcin_i_limit_ma <= 3200))
      {
          err_flag |= pm_schg_dc_set_usbin_current_limit_cfg(device_index, dcin_i_limit_ma);
      }
      else
      {
         err_flag |= PM_ERR_FLAG_INVALID_PARAMETER;
      }
   }

   //Configure Wipower Current Limit level to match power value
   if (wipower_chg_param_ptr->wipower_chg_power_level.enable_config == PM_ENABLE_CONFIG)
   {
      uint32 wipower_chg_power_uw = wipower_chg_param_ptr->wipower_chg_power_level.wipower_chg_power_level;
      if ((wipower_chg_power_uw >= PM_SBL_WP_CHG_POWER_MIN_UW) && (wipower_chg_power_uw <= PM_SBL_WP_CHG_POWER_MAX_UW))
      {
         err_flag |= pm_sbl_config_wipower_chg_ilimit(device_index, wipower_chg_power_uw);
      }
      else
      {
         err_flag |= PM_ERR_FLAG_INVALID_PARAMETER;
      }
   }

   return err_flag;
}

pm_err_flag_type pm_sbl_schg_info_to_smem_update()
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
    uint32 buf_size = sizeof(pm_chg_info_type);
    uint32 buf_size_ret = 0;
    pm_chg_info_type *pChargerInfo = NULL;

    pChargerInfo = (pm_chg_info_type *) smem_get_addr(SMEM_CHARGER_BATTERY_INFO, &buf_size_ret);
    if(!pChargerInfo)
    { //Assume Smem is already initilized in XBL Loader 
        pChargerInfo = (pm_chg_info_type *)smem_alloc(SMEM_CHARGER_BATTERY_INFO, buf_size);
    }

    if(!pChargerInfo)
    {
        pm_log_message("SMEM Chgr Info, ERROR Allocating");
        err_flag = PM_ERR_FLAG_INVALID;
    }
    else
    {
        schg_info.sbl_entry_soc = -1;   // Now we just assign sbl_entry_soc -1 as this workaround is current not required by HLOS
        pChargerInfo->sbl_entry_mV   = schg_info.sbl_entry_mV;
        pChargerInfo->sbl_entry_soc  = schg_info.sbl_entry_soc;
        pm_log_message("SMEM Chgr Info Write Success");
    }

    return err_flag; 
}

