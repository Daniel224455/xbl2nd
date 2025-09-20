#ifndef __PM_APP_RGB__H__
#define __PM_APP_RGB__H__

/*! \file pm_app_rgb.h
 *  \n
 *  \brief  PMIC RGB algorithms
 *  \details  This file contains functions and variable declarations to support
 *   the PMIC RGB
 *
 *
 *  \n &copy; Copyright (c) 2018  by QUALCOMM Technologies Incorporated, All Rights Reserved
 */

/*===========================================================================
                                Edit History
This document is created by a code generator, therefore this section will
not contain comments describing changes made to the module.

 
when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/22/18   al      Creation
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES 

===========================================================================*/

#include "pm_rgb.h"
#include "pm_sdam.h"
#include "pm_lpg.h"
#include "pm_resources_and_types.h"

/*=========================================================================== 
 
                     GLOBAL DEFINES
 
===========================================================================*/



/*=========================================================================== 
 
                     GLOBAL TYPE DEFINITIONS
 
===========================================================================*/

/*! \enum type of LED configuration 
 *  \brief .
 */
typedef enum
{
  /* Glow LED with steady brightness */
   PM_RGB_STATE_GLOW, 
   /* Blink LED */
   PM_RGB_STATE_BLINK,
  /* Turn off LED */
   PM_RGB_STATE_OFF,  
  /*Invalid*/ 
   PM_RGB_STATE_INVALID,
}pm_rgb_state_type;


typedef struct
{
  uint32                     rgb_dim_level_max;
  pm_lpg_chan_type           led_lpg_ch[3];
  boolean                    ramp_direction;
  boolean                    ramp_toggle;
  boolean                    en_pause_hi;
  boolean                    en_pause_lo;
  pm_lpg_pwm_clock_type      clock;
  pm_lpg_pwm_pre_divide_type pre_div;
  pm_lpg_pwm_freq_expo_type  exp;
  pm_lpg_pwm_size_type       bit_size;
  uint32                     max_brightness;
  uint32                     ramp_step_duration;
  uint32                     hi_multiplier;
  uint32                     low_multiplier;
  uint32                     rgb_sdam_pmic;
  pm_sdam_type               rgb_sdam;
  uint32                     sdam_enable_arg;
  uint32                     sdam_disable_arg;
}pm_rgb_specific_data_type; 

typedef struct
{
  uint32 pmic_index;            /*pmic index for RGB*/
  pm_rgb_state_type rgb_state;  /*Refer #pm_rgb_state_type for more info*/
  uint32 rgb_mask;              /*RGB color mask*/
  uint32 dim_level;             /*RGB brightness level*/
}pm_app_rgb_config_type; 


/*===========================================================================

                     PROTOTYPES 

===========================================================================*/

/*===========================================================================
* FUNCTION  pm_app_rgb_glow
* 
* DESCRIPTION
*   This function turns of LED, turns of LED or can blink LED
* 
* PARAMETERS 
* @param [in] rgb_config : RGB configuration to use. Refer struct #pm_app_rgb_config_type for more info* 
*             rgb_config->pmic_index
*                   Selects which PMIC.Valid Values: 0,1,2, etc
*  
*               rgb_config->rgb_state
*                   Refer #pm_rgb_state_type for more info
*  
*             rgb_config->rgb_mask
*                  Selects which RGB LED to config
*                  Valid Values:
*                    PM_RGB_SEGMENT_R
*                    PM_RGB_SEGMENT_G
*                    PM_RGB_SEGMENT_B
*                    PM_RGB_SEGMENT_ALL
*  
*             rgb_config->dim_level
*                  Selects the RGB LED brightness level
*                  Valid values should be between:
*                    PM_RGB_DIM_LEVEL_LOW  0x080
*                    PM_RGB_DIM_LEVEL_MID  0x100
*                    PM_RGB_DIM_LEVEL_HIGH 0x180
*                    PM_RGB_DIM_LEVEL_MAX  0x1FF (Min/Max levels are Configurable Per target)
*  
* RETURN VALUE
*    Type: pm_err_flag_type
*    - Possible values:
*      - PM_ERR_FLAG_SUCCESS               -> Operation succeeded.
*      - PM_ERR_FLAG_INVALID_PARAMETER -> Invalid input parameter
*      - PM_ERR_FLAG_FEATURE_NOT_SUPPORTED -> Feature not available on this
*                                              version of the PMIC.
*      - PM_ERR_FLAG_BUS_ERR           -> Communication with PM chip failed.
*  
*  USAGE EXAMPLE:
*    pm_rgb_led_config(2, PM_RGB_STATE_BLINK, PM_RGB_SEGMENT_R, 0x018);
*  
* DEPENDENCIES
*    The following function must have been called:
*    1) pm_driver_init()
* ===========================================================================*/

pm_err_flag_type pm_app_rgb_glow(pm_app_rgb_config_type *rgb_config);

#endif  //__PM_APP_RGB__H__

