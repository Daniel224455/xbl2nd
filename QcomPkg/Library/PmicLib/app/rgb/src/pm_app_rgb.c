/*! \file
*  
*  \brief  pm_app_rgb_led.c
*  \details Implementation file for rgb led resourece type.
*
*  &copy; Copyright 2013-2018 QUALCOMM Technologies Incorporated, All Rights Reserved
*/

/*===========================================================================

EDIT HISTORY FOR MODULE

This document is created by a code generator, therefore this section will
not contain comments describing changes made to the module.


when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
05/01/18   pxm     Fixed rgb_led_state array out of bound issue
02/29/17   sv      Added pm_rgb_get_pmic_index API
05/01/17   aab     Updated pm_rgb_led_config() to use max/min dim level config data
05/17/15   aab     Updated LPG Channel assignment to support 8998 target
03/31/15   aab     Added a call to select PWM source: pm_lpg_pwm_src_select()
06/24/14   aab     Cleaned up and added support for PMI8994
03/13/13   aab     Creation
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES 

===========================================================================*/

#include "pm_err_flags.h"
#include "CoreVerify.h"
#include "pm_app_rgb.h"
#include "pm_target_information.h"

#define RED_INDEX   0
#define GREEN_INDEX 1
#define BLUE_INDEX  2
#define LED_ENABLE  1
#define LED_DISABLE 0

#define LUT_START_INDEX 0
#define LUT_END_INDEX   1
#define LUT_VALUE_DARK  0x00

static uint32 rgb_led_state[3]=
{
  [RED_INDEX]   = LED_DISABLE, 
  [GREEN_INDEX] = LED_DISABLE, 
  [BLUE_INDEX]  = LED_DISABLE
};


pm_err_flag_type pm_rgb_led_config
(
   unsigned                   device_index,
   pm_rgb_which_type          rgb_peripheral,
   uint32                     rgb_mask,
   uint32                     dim_level,
   boolean                    enable_rgb
   ) 
{
   pm_rgb_state_type rgb_state = (enable_rgb) ? PM_RGB_STATE_GLOW : PM_RGB_STATE_OFF;

   pm_app_rgb_config_type rgb_config =
                                      {
                                        .pmic_index = device_index,
                                        .rgb_state  = rgb_state,
                                        .rgb_mask   = rgb_mask,
                                        .dim_level  = dim_level
                                      }; 

   return pm_app_rgb_glow(&rgb_config);
}

pm_err_flag_type pm_rgb_get_pmic_index(pm_rgb_uasge_type Rgb_led_type, uint32 *rgb_chg_pmic_index)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
    pm_rgb_pmic_index_type *rgb_pmic_index_data_ptr = NULL;   
    rgb_pmic_index_data_ptr = (pm_rgb_pmic_index_type*)pm_target_information_get_specific_info(PM_PROP_RGB_PMIC_INDEX);
    if(rgb_pmic_index_data_ptr == NULL)
    {
      return PM_ERR_FLAG_INVALID_PARAMETER; 
    }
    if(Rgb_led_type == PM_RGB_CHARGER_LED)
    {
        *rgb_chg_pmic_index = rgb_pmic_index_data_ptr->charger;
    }
    else
    {
        return PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    
    return err_flag; 
}


pm_err_flag_type pm_app_rgb_glow(pm_app_rgb_config_type *rgb_config)
{
    pm_err_flag_type err_flag    = PM_ERR_FLAG_SUCCESS;
    pm_lpg_chan_type lpg_channel = PM_LPG_CHAN_1;
    uint8 trig_ret_val = 0;
    uint32 pos_i = 0;
    uint32 led = 0;
    uint32 dim_level = 0;
    uint32 rgb_mask = 0;
    uint32 device_index = 0;
    pm_rgb_state_type rgb_state;
    boolean lut_pattern_repeat = FALSE;

    pm_rgb_specific_data_type *cfg_ptr = NULL;

    cfg_ptr = (pm_rgb_specific_data_type *)pm_target_information_get_specific_info(PM_PROP_RGB_SPECIFIC_DATA);

    if (cfg_ptr == NULL || rgb_config == NULL)
    {
        return PM_ERR_FLAG_INVALID_PARAMETER;
    }

    rgb_mask = rgb_config->rgb_mask;
    rgb_state = rgb_config->rgb_state;
    device_index = rgb_config->pmic_index;
    dim_level = rgb_config->dim_level;

    //check for bounds
    dim_level = (dim_level <=  cfg_ptr->rgb_dim_level_max) ? dim_level : cfg_ptr->rgb_dim_level_max;

    while (rgb_mask & PM_RGB_SEGMENT_ALL)
    {
        led = (1 << pos_i);

        if (rgb_mask & led)
        {
            lpg_channel = cfg_ptr->led_lpg_ch[pos_i];
            switch (rgb_state)
            {
                case PM_RGB_STATE_OFF:
                {
                    rgb_led_state[pos_i] = FALSE;
                    err_flag |= pm_lpg_pwm_enable(device_index, lpg_channel, FALSE); //Disable LPG
                    err_flag |= pm_rgb_enable(device_index, PM_RGB_1, led, LED_DISABLE);
                    if (rgb_led_state[RED_INDEX] == FALSE && rgb_led_state[GREEN_INDEX] == FALSE && rgb_led_state[BLUE_INDEX] == FALSE)
                    {
                        err_flag |= pm_sdam_trig_seq(cfg_ptr->rgb_sdam_pmic, cfg_ptr->rgb_sdam, cfg_ptr->sdam_disable_arg, &trig_ret_val, PM_SDAM_TRIG_FNF);
                    }
                }
                break;

                case PM_RGB_STATE_BLINK:
                    lut_pattern_repeat = TRUE; //no break statement since rest of the config are same as steady glow
                case PM_RGB_STATE_GLOW:
                {
                    err_flag |= pm_sdam_trig_seq(cfg_ptr->rgb_sdam_pmic, cfg_ptr->rgb_sdam, cfg_ptr->sdam_enable_arg, &trig_ret_val, PM_SDAM_TRIG_WAIT);
                    err_flag |= pm_lpg_pattern_config(device_index, lpg_channel, cfg_ptr->ramp_direction, lut_pattern_repeat, cfg_ptr->ramp_toggle, cfg_ptr->en_pause_hi, cfg_ptr->en_pause_lo);
                    err_flag |= pm_lpg_pwm_clock_sel(device_index, lpg_channel, cfg_ptr->clock);
                    err_flag |= pm_lpg_pwm_set_pre_divide(device_index, lpg_channel, cfg_ptr->pre_div, cfg_ptr->exp);
                    err_flag |= pm_lpg_set_pwm_bit_size(device_index, lpg_channel, cfg_ptr->bit_size);
                    err_flag |= pm_lpg_config_pwm_type(device_index, lpg_channel, FALSE);
                    err_flag |= pm_lpg_pwm_set_pwm_value(device_index, lpg_channel, dim_level);  //Configure DIM level
                    err_flag |= pm_lpg_pwm_lut_index_set(device_index, lpg_channel, LUT_START_INDEX, LUT_END_INDEX);
                    err_flag |= pm_lpg_config_pause_time(device_index, lpg_channel, cfg_ptr->ramp_step_duration, cfg_ptr->hi_multiplier, cfg_ptr->low_multiplier);
                    err_flag |= pm_lpg_pwm_src_select(device_index, lpg_channel, PM_LPG_PWM_SRC_LUT);
                    err_flag |= pm_lpg_lut_config_set(device_index, LUT_START_INDEX, LUT_VALUE_DARK); //make LUT index 0 as 00 for dark
                    err_flag |= pm_lpg_lut_config_set(device_index, LUT_END_INDEX, dim_level);
                    err_flag |= pm_lpg_pwm_ramp_generator_enable(device_index, lpg_channel, TRUE);
                    err_flag |= pm_lpg_pwm_ramp_generator_start(device_index, lpg_channel);
                    err_flag |= pm_lpg_pwm_enable(device_index, lpg_channel, TRUE); //enable LPG
                    err_flag |= pm_rgb_enable(device_index, PM_RGB_1, led, LED_ENABLE);
                    rgb_led_state[pos_i] = TRUE;
                }
                break;

                default:
                    return PM_ERR_FLAG_INVALID_PARAMETER;
            }
        }

        rgb_mask = rgb_mask & (~led);
        pos_i++;
    }

    return err_flag;
}



