/*! \file
 *  
 *  \brief  
 *   pm_config_target.c
 *  \details 
 *   This file contains customizable target specific 
 *   driver settings & PMIC registers. This file is generated from database functional
 *   configuration information that is maintained for each of the targets.
 *  
 *    PMIC code generation Version: 1.0.0.0
 *    PMIC code generation Resource Setting Information Version: VU.Please Provide Valid Label - Not Approved
 *    PMIC code generation Software Register Information Version: VU.Please Provide Valid Label - Not Approved
 *    PMIC code generation Processor Allocation Information Version: VU.Please Provide Valid Label - Not Approved
 *    This file contains code for Target specific settings and modes.
 *  
 *  &copy; Copyright 2010-2018 QUALCOMM Technologies Incorporated, All Rights Reserved
 */

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This document is created by a code generator, therefore this section will
  not contain comments describing changes made to the module.


when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/02/18   pxm     Revise LPG channel for RGB.
02/16/18   aab     Added support for Poipu target
07/29/17   sv      Added support for pm_rgb_pmic_index_type config data
05/15/17   aab     Added more config items on sbl_schg_pbs_temp_monitor_config[]
05/01/17   aab     Added support for pm_rgb_specific_data_type config data
04/28/17   aab     Added pm_device_index_type config data
03/14/17   pxm     Enable ICL and set to 1500 mA.
02/13/17    al     Add support to read PBS from REV_ID
01/11/17   pxm     add support for PBS triggered AFP
12/16/16   pxm     Added support for fake battery id range configuration
10/20/16   aab     Added support for Batt_therm Out of bound temperature check
10/13/16   pxm     Updated charger Wdog default configuration
09/17/16   aab     Updated Wipower charger default power level. 
09/17/16   aab     Added WiPower charging wait time as configurable 
08/25/16   pxm     Add skin temperature threshold configuration parameter  
08/12/16   aab     Added debugboard detection support
07/05/16   aab     Cleanup unused data
05/27/16   pxm     Add sbl_schg_wipower_specific_data[1]
04/29/16   aab     Added sbl_pon_specific_data[]
03/24/16   al      Add keypress support
03/29/16   al      Updating for newer target
03/15/16   aab     Driver update (Support for PMI8998)
02/16/16   aab     Creation
===========================================================================*/
/*===========================================================================

                     INCLUDE FILES 

===========================================================================*/
#include "pm_lpg_driver.h"
#include "pm_vib_driver.h"
#include "pm_schg_driver.h"
#include "pm_bmd_driver.h"

#include "pm_fg_sram.h"
#include "pm_fg_adc_rr.h"
#include "pm_resource_manager.h"
#include "pm_pbs_info.h"
#include "pm_pbs_driver.h"
#include "pm_config_target.h"
#include "pm_sbl_boot.h"

#include "pm_app_chgr.h"
#include "pm_app_chgr_common.h"
#include "pm_schg_batif.h"
#include "pm_schg_misc.h"
#include "pm_app_pon.h"

#include "rpmh_common.h"
#include "device_info.h"
#include "pm_app_rgb.h"

 /************************************************ DRIVER DATA *********************************/
                         //PM855#1, SMB2351#1, PM855L, PMK8002, PM855#2, SMB2351#2  SMB2351#3
uint32 num_of_ldo[]        = {18,      0,      11,      0,       18,      0, 			0 };
uint32 num_of_smps[]       = {10,      0,       8,      0,       10,      0, 			0 };
uint32 num_of_gpio[]       = {10,      0,      12,      0,       10,      0, 			0 };
uint32 num_of_rtc[]        = { 1,      0,       0,      0,        1,      0, 			0 };
uint32 num_of_megaxo[]     = { 0,      0,       0,      0,        0,      0, 			0 }; 
uint32 num_of_boost[]      = { 0,      0,       0,      0,        0,      0, 			0 }; 
uint32 num_of_bob[]        = { 0,      0,       1,      0,        0,      0, 			0 };
uint32 num_of_vs[]         = { 0,      0,       0,      0,        0,      0, 			0 };
uint32 num_of_lpg_chan[]   = { 0,      0,       3,      0,        0,      0, 			0 };
uint32 num_of_pwron[]      = { 1,      1,       1,      0,        1,      1, 			1 };
uint32 num_of_misc[]       = { 1,      1,       1,      0,        1,      1, 			1 };
uint32 num_of_rgb[]        = { 0,      0,       1,      0,        0,      0, 			0 };
uint32 num_of_wled[]       = { 0,      0,       1,      0,        0,      0, 			0 };
uint32 num_of_schg[]       = { 0,      1,       0,      0,        0,      1, 			1 };
uint32 num_of_bmd[]        = { 0,      1,       0,      0,        0,      1, 			1 };
uint32 num_of_coincell[]   = { 1,      0,       0,      0,        1,      0, 			0 };
uint32 num_of_fg[]         = { 0,      1,       0,      0,        0,      0, 			0 };
uint32 num_of_ibb[]        = { 0,      0,       1,      0,        0,      0, 			0 };
uint32 num_of_lab[]        = { 0,      0,       0,      0,        0,      0, 			0 };
uint32 num_of_ab[]         = { 0,      0,       1,      0,        0,      0, 			0 };
uint32 num_of_oledb[]      = { 0,      0,       1,      0,        0,      0, 			0 };
uint32 num_of_vib[]        = { 0,      0,       0,      0,        0,      0, 			0 };
uint32 num_of_pbs_client[] = { 2,      2,       2,      0,        2,      2, 			2 };
uint32 num_of_haptics[]    = { 0,      0,       0,      0,        0,      0, 			0 };
uint32 num_of_lcdb[]       = { 0,      0,       1,      0,        0,      0, 			0 };
uint32 num_of_pwm[]        = { 0,      0,       2,      0,        0,      0, 			0 };
uint32 num_of_sdam[]       = { 8,      0,       0,      0,        8,      0, 			0 }; 

pm_lpg_specific_info_type lpg_specific[1] =  
{
    {49}
};

pm_vib_specific_data_type
vib_specific[1] = 
{
  {1200, 2000}
};


pm_sbl_pon_specific_data_type
sbl_pon_specific_data[1] = 
{
   {
      {
         16,                              // s3 timer value  (max value: 128 sec)
         PM_APP_PON_RESET_SOURCE_RESIN_AND_KPDPWR,  // pm_app_pon_reset_source_type
         PM_ENABLE_CONFIG                // PM_DISABLE_CONFIG / PM_ENABLE_CONFIG
      }, 
   }
};

pm_sbl_schg_afp_type afp_config[1] =
{
    {
        75,             // too hot threshold to trigger AFP
        TRUE,           // Configure to enable/disable AFP trigger when battery temperature higher than too hot threshold
        -272,           // too cold threshold to trigger AFP. Default to extremly low temperature to avoid be hit
                        // sinc there are cases like thermal pin floating
        TRUE            // Configure to enable/disable AFP trigger when battery temperature lower than too cold threshold
    }
};


pm_sbl_schg_specific_data_type
sbl_schg_specific_data[1] = 
{
   {
      //Configuration Value,                             Enable config
      {PM_SCHG_BATIF_LOW_BATTERY_THRESH_3P2,             PM_DISABLE_CONFIG },  //Vlowbatt Threshold
      {PM_SCHG_BATIF_LOW_BATTERY_THRESH_2P6,             PM_ENABLE_CONFIG  },  //APSD rerun Vlowbatt Threshold
      {70,                                               PM_ENABLE_CONFIG  },  //Fg skin hot threshold: valid range is -30 to 97 degree C
      {80,                                               PM_ENABLE_CONFIG  },  //Fg skin too hot threshold: valid range is -30 to 97 degree C
      {80,                                               PM_ENABLE_CONFIG  },  //Fg charge hot threshold: valid range is -30 to 97 degree C
      {90,                                               PM_ENABLE_CONFIG  },  //Fg charge too hot threshold: valid range is -30 to 97 degree C 
      {TRUE,                                             PM_DISABLE_CONFIG },  //Use BATID and/or THERM pin for battery missing detection
      {{TRUE, PM_SCHG_MISC_SNARL_WDOG_TMOUT_62P5MS, PM_SCHG_MISC_BARK_WDOG_TMOUT_128S, PM_SCHG_MISC_BITE_WDOG_TMOUT_8S}, PM_ENABLE_CONFIG },  //Enable/Disable and Timeout WDog Config
      {1000,                                             PM_DISABLE_CONFIG },  //FAST Charging Current
      {250,                                              PM_DISABLE_CONFIG },  //PRE Charge Current
      {4200,                                             PM_DISABLE_CONFIG },  //Float Voltage
      {500,                                              PM_DISABLE_CONFIG },  //USBIN Input Current Limit
      {1000,                                             PM_DISABLE_CONFIG },  //DCIN Input Current Limit
      {6750, 8250,                                       PM_ENABLE_CONFIG  },  //Fake battery detection range: Battery will be detected as fake battery if BATT ID in this range. unit: Ohms
      {2000, 14000},                                                           //Debug Board detect: BATT_ID PD Resistor ADC Min/Max Read Value range; unit: Ohms
      PM_BATT_ID_PU_30K,                                                       //battery ID pull up resistor value
      3300,                                                                    //bootup_battery_theshold_mv
      3300,                                                                    //wipowr bootup battery thesholdmv
      2800,                                                                    //apsd_reset_threshold_mv:  APSD reset only applicable if initial Vbatt level is less than this threshold
      2800,                                                                    //apsd_reset_theshold_no_uvlo_mv: Used if last reset reason is NOT UVLO
      3200,                                                                    //apsd_reset_theshold_uvlo_mv: Used if last reset reason is UVLO
      TRUE,                                                                    //Enable/Disable JEITA Hard Temp Limit Check in SBL
      TRUE,                                                                    //dbc_usb_500_mode
      FALSE,                                                                   //Verbose SBL CHG UART logging
   }
};



pm_sbl_specific_data_type
sbl_specific_data[1] = {
  {
    .verbose_uart_logging = FALSE,      // Enable/disable verbose UART logging
    .rfclk2_config_flag   = FALSE,      // FALSE: Default, TRUE: Disable RFCLK2
    .swire_cfg            = {FALSE, PM_SWIRE_MAP_INVALID}, // Swire map configuration
  }
};

pm_sbl_schg_wipower_specific_data_type
sbl_schg_wipower_specific_data[1] = 
{
   {
      {
         5000000,           // Wipower charger power valid range in uWatt: 5000000 to 20000000
         PM_ENABLE_CONFIG   // PM_DISABLE_CONFIG / PM_ENABLE_CONFIG
      }, 
      {
         3500,              //unit: ms;  default min Val: 3500ms;  NO Max Value.
         PM_ENABLE_CONFIG   //PM_DISABLE_CONFIG: use the wait time;  PM_ENABLE_CONFIG: Wait forever
      }, 
   }
};




//On kekaha:  No JEITA/AFP Triggering/Monitoring Needed ?
//Since HW JEITA/AFP takes care of it
//We just have to make sure that HW jeita/AFP is enabled in SBL ?
/*
pm_sbl_schg_batt_therm_type
sbl_schg_batt_therm[1] = 
{
   {
      PM_ENABLE_CONFIG, //Enable/Disable Batt Therm config
      {
         99000,         //min batt ID for the battery that will use this conefficient
         110000,        //max batt ID 
      },                       
      {                                                                               
        0x2306,         //Thermistor_c0_coefficient:  default = 0x2306
        0x0CC0,         //Thermistor_c1_coefficient:  default = 0x0CC0
        0xE203,         //Thermistor_c2_coefficient:  default = 0xE203
        0xC336,         //Thermistor_c3_coefficient:  default = 0xC336
        0x9466,         //Thermistor_c5_coefficient:  default = 0x9466
        0x70            //Thermistor_center_val:      default = 0x70
      }
   }
};
*/


pm_app_chg_temp_limit_config_type
pm_app_chg_temp_limit_config[1] = 
{
   {
      0xAC,     //Hot threshold:  71 Deg. c. =  (71 + 273)/2 
      0x00,     //Cold threshold: -273 Deg.C =  (-273 + 273)/2 
   }
};

/*
uint8 pm_pbs_afp_en_afp_val  = 0x11; //PBS SW Trigger argument value to enable AFP
uint8 pm_pbs_afp_dis_afp_val = 0x01; //PBS SW Trigger argument value to disable AFP

pm_app_chg_temp_monitor_config_type
pm_app_chg_temp_monitor_config[1] = 
{
   {
      1,        //PMIC index
      0,        //Hot/Cold Limit PMIC Reg Address SID Index
      0x088C,   //Hot Limit Reg Address  
      0x088F,   //Cold Limit reg Address 
      PM_PBS_CLIENT_3,                    //PBS Client
      PM_PBS_CLIENT_SW_TRIGGER,           //Client Trigger Num
      PM_PBS_CLIENT_TRIGGER_EDGE_INVALID, //Client HW Trigger Edge
      &pm_pbs_afp_en_afp_val,             //PBS SW Trigger argument value to enable AFP
      &pm_pbs_afp_dis_afp_val,            //PBS SW Trigger argument value to disable AFP  
      TRUE                                //PBS AFP Enable/Disable 
   }
};
*/


pm_device_index_type pm_device_index =
{
   0,  //Primary PMIC Device Index   (PM8998)
   1,  //Interface PMIC Device Index (PMI8998)
   2,  //Secondary PMIC Device Index (PM8005)
};

pm_rgb_pmic_index_type pm_rgb_pmic_index =
{
   2,   // Charger RGB LED PMIC index
   //0   // Notification RGB LED PMIC index - future purpose when multiple RGB exists on different PMIC's
};

pm_rgb_specific_data_type
  rgb_specific_data =
{
  .rgb_dim_level_max  = 0x01FF,
  .led_lpg_ch         = {PM_LPG_CHAN_3, PM_LPG_CHAN_2, PM_LPG_CHAN_1},
  .ramp_direction     = TRUE,
  .ramp_toggle        = FALSE,
  .en_pause_hi        = TRUE,
  .en_pause_lo        = TRUE,
  .clock              = PM_LPG_PWM_19_2_MHZ,
  .pre_div            = PM_LPG_PWM_PRE_DIV_6,
  .exp                = PM_LPG_PWM_FREQ_EXPO_1,
  .bit_size           = PM_LPG_PWM_9BIT,
  .ramp_step_duration = 255, //max ramp duration time in milli sec
  .hi_multiplier      = 2,   //multiplier for high time 
  .low_multiplier     = 1,   //multiplier for low time
  .rgb_sdam_pmic      = 0,
  .rgb_sdam           = PM_SDAM_2,
  .sdam_enable_arg    = 1,
  .sdam_disable_arg   = 2,
}; 


//PM855 #1
pm_pbs_info_data_type pm_pbs_info_data_a =
{
  .pbs_otp_start_addr    = 0x0,                /* PBS OTP MEMORY Start Address */
  .pbs_otp_mem_size      = 512,                /* PBS OTP MEMORY Size */
  .pbs_ram_start_addr    = 0x800,              /* PBS RAM MEMORY Start Address */
  .pbs_ram_mem_size      = 1536,               /* PBS RAM MEMORY Size */
  .skip_verify_supported = TRUE,               /* Skip Ram Image Word Range Supported */
  .skip_verify_start     = 0x4,                /* Skip Ram Image Word Range Start Word */
  .skip_verify_end       = 0x5,                /* Skip Ram Image Word Range End Word */
  .pbs_info_place_holder = PM_PBS_INFO_IN_OTP, /* Place where PBS Info stored */
  .perph_base_addr       = 0x00,               /* Periph BASE_ADDRESS */
  .trim_num              = 0x00,               /* trim_num  */
  .tp_rev                = 0x00,               /* tp_rev    */
  .fab_id                = 0x00,               /* fab_id    */
  .wafer_id              = 0x00,               /* wafer_id  */
  .x_co_ordi             = 0x00,               /* x_co_ordi */
  .y_co_ordi             = 0x00,               /* y_co_ordi */
  .lot_id                = 0x00,               /* lot_id_01_00 */
  .mfg_id                = 0x00,               /* mfg_id    */
};

//SMB2351 #1
pm_pbs_info_data_type pm_pbs_info_data_b =
{
  .pbs_otp_start_addr    = 0x0,                   /* PBS OTP MEMORY Start Address */
  .pbs_otp_mem_size      = 512,                   /* PBS OTP MEMORY Size */
  .pbs_ram_start_addr    = 0x800,                 /* PBS RAM MEMORY Start Address */
  .pbs_ram_mem_size      = 512,                   /* PBS RAM MEMORY Size */
  .skip_verify_supported = FALSE,                 /* Skip Ram Image Word Range Supported */
  .skip_verify_start     = 0x0,                   /* Skip Ram Image Word Range Start Word */
  .skip_verify_end       = 0x0,                   /* Skip Ram Image Word Range End Word */
  .pbs_info_place_holder = PM_PBS_INFO_IN_OTP,    /* Place where PBS Info stored */
  .perph_base_addr       = 0x0100,                /* REV_ID Periph BASE_ADDRESS */
  .trim_num              = 0xF0,                  /* trim_num  */
  .tp_rev                = 0xF1,                  /* tp_rev    */
  .fab_id                = 0xF2,                  /* fab_id    */
  .wafer_id              = 0xF3,                  /* wafer_id  */
  .x_co_ordi             = 0xF4,                  /* x_co_ordi */
  .y_co_ordi             = 0xF5,                  /* y_co_ordi */
  .lot_id                = 0xF6,                  /* lot_id_11_10 */
  .mfg_id                = 0xFF,                  /* mfg_id    */
};

//PM855L
pm_pbs_info_data_type pm_pbs_info_data_c =
{
  .pbs_otp_start_addr    = 0x0,                /* PBS OTP MEMORY Start Address */
  .pbs_otp_mem_size      = 512,                /* PBS OTP MEMORY Size */
  .pbs_ram_start_addr    = 0x800,              /* PBS RAM MEMORY Start Address */
  .pbs_ram_mem_size      = 256,                /* PBS RAM MEMORY Size */
  .skip_verify_supported = FALSE,              /* Skip Ram Image Word Range Supported */
  .skip_verify_start     = 0x0,                /* Skip Ram Image Word Range Start Word */
  .skip_verify_end       = 0x0,                /* Skip Ram Image Word Range End Word */
  .pbs_info_place_holder = PM_PBS_INFO_IN_OTP, /* Place where PBS Info stored */
  .perph_base_addr       = 0x00,               /* Periph BASE_ADDRESS */
  .trim_num              = 0x00,               /* trim_num  */
  .tp_rev                = 0x00,               /* tp_rev    */
  .fab_id                = 0x00,               /* fab_id    */
  .wafer_id              = 0x00,               /* wafer_id  */
  .x_co_ordi             = 0x00,               /* x_co_ordi */
  .y_co_ordi             = 0x00,               /* y_co_ordi */
  .lot_id                = 0x00,               /* lot_id_01_00 */
  .mfg_id                = 0x00,               /* mfg_id    */
};

//PMK8002:  place holder
pm_pbs_info_data_type pm_pbs_info_data_d =
{
  .pbs_otp_start_addr    = 0x0,                /* PBS OTP MEMORY Start Address */
  .pbs_otp_mem_size      = 512,                /* PBS OTP MEMORY Size */
  .pbs_ram_start_addr    = 0x800,              /* PBS RAM MEMORY Start Address */
  .pbs_ram_mem_size      = 1536,               /* PBS RAM MEMORY Size */
  .skip_verify_supported = FALSE,              /* Skip Ram Image Word Range Supported */
  .skip_verify_start     = 0x0,                /* Skip Ram Image Word Range Start Word */
  .skip_verify_end       = 0x0,                /* Skip Ram Image Word Range End Word */
  .pbs_info_place_holder = PM_PBS_INFO_IN_OTP, /* Place where PBS Info stored */
  .perph_base_addr       = 0x00,               /* Periph BASE_ADDRESS */
  .trim_num              = 0x00,               /* trim_num  */
  .tp_rev                = 0x00,               /* tp_rev    */
  .fab_id                = 0x00,               /* fab_id    */
  .wafer_id              = 0x00,               /* wafer_id  */
  .x_co_ordi             = 0x00,               /* x_co_ordi */
  .y_co_ordi             = 0x00,               /* y_co_ordi */
  .lot_id                = 0x00,               /* lot_id_01_00 */
  .mfg_id                = 0x00,               /* mfg_id    */
};

//PM855 #2
pm_pbs_info_data_type pm_pbs_info_data_e =
{
  .pbs_otp_start_addr    = 0x0,                /* PBS OTP MEMORY Start Address */
  .pbs_otp_mem_size      = 512,                /* PBS OTP MEMORY Size */
  .pbs_ram_start_addr    = 0x800,              /* PBS RAM MEMORY Start Address */
  .pbs_ram_mem_size      = 1536,               /* PBS RAM MEMORY Size */
  .skip_verify_supported = FALSE,              /* Skip Ram Image Word Range Supported */
  .skip_verify_start     = 0x0,                /* Skip Ram Image Word Range Start Word */
  .skip_verify_end       = 0x0,                /* Skip Ram Image Word Range End Word */
  .pbs_info_place_holder = PM_PBS_INFO_IN_OTP, /* Place where PBS Info stored */
  .perph_base_addr       = 0x00,               /* Periph BASE_ADDRESS */
  .trim_num              = 0x00,               /* trim_num  */
  .tp_rev                = 0x00,               /* tp_rev    */
  .fab_id                = 0x00,               /* fab_id    */
  .wafer_id              = 0x00,               /* wafer_id  */
  .x_co_ordi             = 0x00,               /* x_co_ordi */
  .y_co_ordi             = 0x00,               /* y_co_ordi */
  .lot_id                = 0x00,               /* lot_id_01_00 */
  .mfg_id                = 0x00,               /* mfg_id    */
};

//SMB2351 #2
pm_pbs_info_data_type pm_pbs_info_data_f =
{
  .pbs_otp_start_addr    = 0x0,                   /* PBS OTP MEMORY Start Address */
  .pbs_otp_mem_size      = 512,                   /* PBS OTP MEMORY Size */
  .pbs_ram_start_addr    = 0x800,                 /* PBS RAM MEMORY Start Address */
  .pbs_ram_mem_size      = 512,                   /* PBS RAM MEMORY Size */
  .skip_verify_supported = FALSE,                 /* Skip Ram Image Word Range Supported */
  .skip_verify_start     = 0x0,                   /* Skip Ram Image Word Range Start Word */
  .skip_verify_end       = 0x0,                   /* Skip Ram Image Word Range End Word */
  .pbs_info_place_holder = PM_PBS_INFO_IN_OTP,    /* Place where PBS Info stored */
  .perph_base_addr       = 0x0100,                /* REV_ID Periph BASE_ADDRESS */
  .trim_num              = 0xF0,                  /* trim_num  */
  .tp_rev                = 0xF1,                  /* tp_rev    */
  .fab_id                = 0xF2,                  /* fab_id    */
  .wafer_id              = 0xF3,                  /* wafer_id  */
  .x_co_ordi             = 0xF4,                  /* x_co_ordi */
  .y_co_ordi             = 0xF5,                  /* y_co_ordi */
  .lot_id                = 0xF6,                  /* lot_id_11_10 */
  .mfg_id                = 0xFF,                  /* mfg_id    */
};
//SMB2351 #3
pm_pbs_info_data_type pm_pbs_info_data_g =
{
  .pbs_otp_start_addr    = 0x0,                   /* PBS OTP MEMORY Start Address */
  .pbs_otp_mem_size      = 512,                   /* PBS OTP MEMORY Size */
  .pbs_ram_start_addr    = 0x800,                 /* PBS RAM MEMORY Start Address */
  .pbs_ram_mem_size      = 512,                   /* PBS RAM MEMORY Size */
  .skip_verify_supported = FALSE,                 /* Skip Ram Image Word Range Supported */
  .skip_verify_start     = 0x0,                   /* Skip Ram Image Word Range Start Word */
  .skip_verify_end       = 0x0,                   /* Skip Ram Image Word Range End Word */
  .pbs_info_place_holder = PM_PBS_INFO_IN_OTP,    /* Place where PBS Info stored */
  .perph_base_addr       = 0x0100,                /* REV_ID Periph BASE_ADDRESS */
  .trim_num              = 0xF0,                  /* trim_num  */
  .tp_rev                = 0xF1,                  /* tp_rev    */
  .fab_id                = 0xF2,                  /* fab_id    */
  .wafer_id              = 0xF3,                  /* wafer_id  */
  .x_co_ordi             = 0xF4,                  /* x_co_ordi */
  .y_co_ordi             = 0xF5,                  /* y_co_ordi */
  .lot_id                = 0xF6,                  /* lot_id_11_10 */
  .mfg_id                = 0xFF,                  /* mfg_id    */
};

pm_pbs_info_data_type *pm_pbs_info_data[] = {&pm_pbs_info_data_a, &pm_pbs_info_data_b, &pm_pbs_info_data_c, &pm_pbs_info_data_d, &pm_pbs_info_data_e, &pm_pbs_info_data_f, &pm_pbs_info_data_g};


/* max_num_seqs, ram_start_addr, poff_trig_id, poff_otp_addr, warmreset_trig_id, warmreset_otp_addr */
pm_pbs_custom_seq_data_type pbs_custom_seq_data_a = {2, 0x07DC, 3, 0x000C, 2, 0x0008};

/* max_num_seqs, ram_start_addr, poff_trig_id, poff_otp_addr, warmreset_trig_id, warmreset_otp_addr */
pm_pbs_custom_seq_data_type pbs_custom_seq_data_b = {33, 0x0554, 2, 0x0008, 7, 0x001C};
 
pm_pbs_custom_seq_data_type* pbs_custom_seq_data[] = 
{
   NULL,
   NULL,
   NULL,
   NULL,
};






const uint8 pm_periph_bitmap[][PM_MAX_NUM_SLAVE_IDS_PER_PMIC][PM_MAX_BITMAP_ENTRIES] =
{
  /* pm855 #1*/
  {
    {
      0xf2, 0x0b, 0x00, 0x00, 
      0x10, 0x11, 0xe6, 0x73, 
      0x00, 0x00, 0x7f, 0x1e, 
      0xc7, 0x00, 0x3f, 0x00, 
      0x00, 0x00, 0x00, 0x00, 
      0x00, 0x00, 0xff, 0x00, 
      0xff, 0x03, 0x00, 0x00, 
      0x00, 0x00, 0x00, 0x00, 
    },
    {
      0x02, 0x00, 0xf1, 0xff, 
      0xff, 0xff, 0x03, 0x00, 
      0xff, 0xff, 0x03, 0x00, 
      0x00, 0x00, 0x00, 0x00, 
      0x00, 0x00, 0x00, 0x00, 
      0x00, 0x00, 0x00, 0x00, 
      0x00, 0x00, 0x00, 0x00, 
      0x00, 0x00, 0x00, 0x00, 
    },
  },
  /* smb2351 #1*/
  {
    {
      0x72, 0x06, 0xef, 0x66, 
      0x00, 0x00, 0xe6, 0x02, 
      0x00, 0x00, 0x00, 0x0a, 
      0x00, 0x00, 0x1f, 0x00, 
      0x00, 0x00, 0x00, 0x00, 
      0x00, 0x00, 0x00, 0x00, 
      0x00, 0x00, 0x00, 0x00, 
      0x00, 0x00, 0x00, 0x00, 
    },
    {
      0x00, 0x00, 0x00, 0x00, 
      0x00, 0x00, 0x00, 0x00, 
      0x00, 0x00, 0x00, 0x00, 
      0x00, 0x00, 0x00, 0x00, 
      0x00, 0x00, 0x00, 0x00, 
      0x00, 0x00, 0x00, 0x00, 
      0x00, 0x00, 0x00, 0x00, 
      0x00, 0x00, 0x00, 0x00, 
    },
  },
  /* pm855l */
  {
    {
      0x72, 0x03, 0x00, 0x00, 
      0x10, 0x10, 0xe6, 0xf2, 
      0x00, 0x00, 0x00, 0x0a, 
      0xc0, 0x00, 0x3f, 0x00, 
      0x00, 0x00, 0x00, 0x00, 
      0x00, 0x00, 0x00, 0x00, 
      0xff, 0x0f, 0x00, 0x00, 
      0x00, 0x00, 0x00, 0x00, 
    },
    {
      0x02, 0x00, 0xf1, 0xff, 
      0xff, 0x0f, 0x00, 0x00, 
      0xff, 0x07, 0x00, 0x00, 
      0x00, 0x00, 0x00, 0x00, 
      0x00, 0x00, 0x00, 0x00, 
      0x03, 0x00, 0x0f, 0x30, 
      0x00, 0x00, 0x09, 0x03, 
      0x00, 0x10, 0x00, 0x00, 
    },
  },
  /* pmk8002 */
  {
    {
      0x62, 0x02, 0x00, 0x00, 
      0x00, 0x10, 0x00, 0x00, 
      0x00, 0x00, 0xf1, 0x0f, 
      0x00, 0x00, 0x00, 0x00, 
      0x00, 0x00, 0x00, 0x00, 
      0x00, 0x00, 0x00, 0x00, 
      0x00, 0x00, 0x00, 0x00, 
      0x00, 0x00, 0x00, 0x00, 
    },
    {
      0x02, 0x00, 0x00, 0x00, 
      0x00, 0x00, 0x00, 0x00, 
      0x00, 0x00, 0x00, 0x00, 
      0x00, 0x00, 0x00, 0x00, 
      0x00, 0x00, 0x00, 0x00, 
      0x00, 0x00, 0x00, 0x00, 
      0x00, 0x00, 0x00, 0x00, 
      0x00, 0x00, 0x00, 0x00, 
    },
  },
  /* pm855 #2*/
  {
    {
      0xf2, 0x0b, 0x00, 0x00, 
      0x10, 0x11, 0xe6, 0x73, 
      0x00, 0x00, 0x7f, 0x1e, 
      0xc7, 0x00, 0x3f, 0x00, 
      0x00, 0x00, 0x00, 0x00, 
      0x00, 0x00, 0xff, 0x00, 
      0xff, 0x03, 0x00, 0x00, 
      0x00, 0x00, 0x00, 0x00, 
    },
    {
      0x02, 0x00, 0xf1, 0xff, 
      0xff, 0xff, 0x03, 0x00, 
      0xff, 0xff, 0x03, 0x00, 
      0x00, 0x00, 0x00, 0x00, 
      0x00, 0x00, 0x00, 0x00, 
      0x00, 0x00, 0x00, 0x00, 
      0x00, 0x00, 0x00, 0x00, 
      0x00, 0x00, 0x00, 0x00, 
    },
  },
  /* smb2351 #2*/
  {
    {
      0x72, 0x06, 0xef, 0x66, 
      0x00, 0x00, 0xe6, 0x02, 
      0x00, 0x00, 0x00, 0x0a, 
      0x00, 0x00, 0x1f, 0x00, 
      0x00, 0x00, 0x00, 0x00, 
      0x00, 0x00, 0x00, 0x00, 
      0x00, 0x00, 0x00, 0x00, 
      0x00, 0x00, 0x00, 0x00, 
    },
    {
      0x00, 0x00, 0x00, 0x00, 
      0x00, 0x00, 0x00, 0x00, 
      0x00, 0x00, 0x00, 0x00, 
      0x00, 0x00, 0x00, 0x00, 
      0x00, 0x00, 0x00, 0x00, 
      0x00, 0x00, 0x00, 0x00, 
      0x00, 0x00, 0x00, 0x00, 
      0x00, 0x00, 0x00, 0x00, 
    },
  },
  /* smb2351 #3*/
  {
    {
      0x72, 0x06, 0xef, 0x66, 
      0x00, 0x00, 0xe6, 0x02, 
      0x00, 0x00, 0x00, 0x0a, 
      0x00, 0x00, 0x1f, 0x00, 
      0x00, 0x00, 0x00, 0x00, 
      0x00, 0x00, 0x00, 0x00, 
      0x00, 0x00, 0x00, 0x00, 
      0x00, 0x00, 0x00, 0x00, 
    },
    {
      0x00, 0x00, 0x00, 0x00, 
      0x00, 0x00, 0x00, 0x00, 
      0x00, 0x00, 0x00, 0x00, 
      0x00, 0x00, 0x00, 0x00, 
      0x00, 0x00, 0x00, 0x00, 
      0x00, 0x00, 0x00, 0x00, 
      0x00, 0x00, 0x00, 0x00, 
      0x00, 0x00, 0x00, 0x00, 
    },
  },
};

/*key press in milli seconds for valid boot*/
pm_pwrkey_dbnc_chk_type pm_pon_pwrkey_dbnc_chk[] =
{
   {
      .chk_at = PM_PON_PWRKEY_DBNC_CHK_AT_CORE,  // check key press at
      .dbnc_time_msec = 20,                      // valid debounce time msec
      .sdam_dbnc_supported = TRUE,               // SDAM support for reading dbnc time
      .pmic_index = 0,                           // primary pmic index
      .min_pmic_rev = 3,                         // minimum revision of pmic supporting SDAM RTC VAL
      .sdam_periph_index = PM_SDAM_2,            // SDAM peripheral
      .sdam_key_press_offset = 26,               // MEM_026
      .sdam_key_release_offset = 30,             // MEM_030
   }
};

uint32 rpmh_drv_id [] = {(uint32)RSC_DRV_HLOS};

