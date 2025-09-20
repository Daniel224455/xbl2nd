/*! \file rgb_api.c
*  \n
*  \details This file contains the top-level API wrappers for the RGB
*           peripheral.
*
*  \n &copy; Copyright 2015-2018 QUALCOMM Technologies Incorporated, All Rights Reserved
*/

/*===========================================================================

EDIT HISTORY FOR MODULE

This document is created by a code generator, therefore this section will
not contain comments describing changes made to the module.


when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
11/02/15   al      Updaing RGB with latest 
04/06/15   aab     Updating pm_rgb_enable 
08/20/14   al      Updating comm lib 
05/20/14   al      Architecture update
===========================================================================*/

/*===========================================================================
INCLUDE FILES
===========================================================================*/
#include "pm_rgb.h"
#include "pm_rgb_driver.h"

#define ADDRESS(x) (rgb_ptr->rgb_reg_table->base_address + rgb_ptr->rgb_reg_table->x)
#define SLAVE_ID   (rgb_ptr->comm_ptr->slave_id)

#define HOME_ROW_LED 2

pm_err_flag_type pm_rgb_enable
(
  uint32              pmic_chip,
  pm_rgb_which_type   rgb,
  uint32              rgb_mask,
  boolean             enable
)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
    pm_rgb_data_type *rgb_ptr = pm_rgb_get_data(pmic_chip);
    pm_register_address_type reg = 0;
    pm_register_data_type rgb_en_ctl_val;

    if (rgb_ptr == NULL)
    {
      err_flag =  PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    else if (rgb_mask > PM_RGB_SEGMENT_ALL)
    {
      err_flag =  PM_ERR_FLAG_INVALID_PARAMETER;
    }
    else
    {
      reg = ADDRESS(en_ctl);

      rgb_en_ctl_val = (enable)? (rgb_mask <<5): 0x00; /*<R: Bit 7, G: Bit6, B: Bit 5>*/

      rgb_mask = rgb_mask <<5;

      err_flag = pm_comm_write_byte_mask(SLAVE_ID, reg, rgb_mask, rgb_en_ctl_val, 0);
    }
   
    return err_flag;
}


pm_err_flag_type pm_rgb_chgr_led_config
(
  uint32              pmic_chip,
  uint32              led_mask,
  boolean             chgr_led_en
)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
    pm_register_address_type reg = 0;
    pm_register_data_type data = 0;
    pm_rgb_data_type *rgb_ptr = pm_rgb_get_data(pmic_chip);

    if (rgb_ptr == NULL)
    {
      err_flag =  PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    else if (led_mask > PM_RGB_SEGMENT_ALL)
    {
      err_flag =  PM_ERR_FLAG_INVALID_PARAMETER;
    }
    else if (rgb_ptr->sub_type != HOME_ROW_LED)
    {
      err_flag =  PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    else
    {
      reg = ADDRESS(chgr_led_config);

      data = (chgr_led_en)? 0x80: 0x00; 

      data = data|((pm_register_data_type)led_mask);

      err_flag = pm_comm_write_byte(SLAVE_ID, reg, data, 0);
    }
   
    return err_flag;
}




pm_err_flag_type pm_rgb_get_status
(
  uint32              pmic_chip,
  pm_rgb_which_type   rgb,
  pm_rgb_status_type  *status
)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
    pm_rgb_data_type *rgb_ptr = pm_rgb_get_data(pmic_chip);
    pm_register_address_type periph_index = (pm_register_address_type)rgb;

    pm_register_address_type reg_rgb_en_ctl = 0;

    pm_register_data_type en_ctl_val = 0;

    if (rgb_ptr == NULL)
    {
      return PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }

    if (periph_index >= rgb_ptr->num_of_peripherals)
    {
      return PM_ERR_FLAG_INVALID_PARAMETER;
    }

    reg_rgb_en_ctl = ADDRESS(en_ctl);

    err_flag  |= pm_comm_read_byte(SLAVE_ID, reg_rgb_en_ctl, &en_ctl_val, 0);

    status->enabled_mask = (en_ctl_val & (1 << 7)) ? PM_RGB_SEGMENT_R : 0;
    status->enabled_mask |= (en_ctl_val & (1 << 6)) ? PM_RGB_SEGMENT_G : 0;
    status->enabled_mask |= (en_ctl_val & (1 << 5)) ? PM_RGB_SEGMENT_B : 0;
    
    return err_flag;
}

