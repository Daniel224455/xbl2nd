/*! \file
*
*  \brief  pm_ab.c driver implementation.
*  \details charger driver implementation.
*  &copy;
*  Copyright (c) 2018 Qualcomm Technologies, Inc.  All Rights Reserved.
*
*  Qualcomm Technologies Proprietary and Confidential.
*/

/*===========================================================================

EDIT HISTORY FOR MODULE


when        who        what, where, why
--------    ---        ----------------------------------------------------------
02/06/18     al        Initial revision
========================================================================== */

/*===========================================================================

          INCLUDE FILES

===========================================================================*/

#include "pm_ab.h"
#include "pm_ab_driver.h"
#include "pm_resource_manager.h"

/*===========================================================================
                        MACRO DECLARATIONS
===========================================================================*/

#define ADDRESS(reg)  (ab_ptr->ab_reg_table->base_address + ab_ptr->ab_reg_table->reg)
#define SLAVE_ID    (ab_ptr->comm_ptr->slave_id)

#define PM_AB_OUTPUT_MVOLT_BASE 4600    //Base Minimum for register
#define PM_AB_OUTPUT_MVOLT_MIN  4600    //Recomended Minimum
#define PM_AB_OUTPUT_MVOLT_MAX  5200    //Recomended Maximum
#define PM_AB_OUTPUT_MVOLT_STEP 100

/*===========================================================================
                        FUNCTION DEFINITIONS
===========================================================================*/
pm_err_flag_type
pm_ab_config_default_volt(uint32 device_index, uint32 milli_volt)
{
  pm_err_flag_type         err_flag     = PM_ERR_FLAG_SUCCESS;
  uint8                    code         = 0;
  pm_register_address_type vout_default = 0;
  pm_ab_data_type         *ab_ptr     = pm_ab_get_data(device_index);

  if (NULL == ab_ptr)
  {
    return PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
  }

  milli_volt = milli_volt < PM_AB_OUTPUT_MVOLT_MIN ?
               PM_AB_OUTPUT_MVOLT_MIN : milli_volt;

  milli_volt = milli_volt > PM_AB_OUTPUT_MVOLT_MAX ?
               PM_AB_OUTPUT_MVOLT_MAX : milli_volt;

  milli_volt -= PM_AB_OUTPUT_MVOLT_BASE;

  //Round off for step size to ceiling
  code = (uint8)( (milli_volt + (PM_AB_OUTPUT_MVOLT_STEP - 1)) /
                  PM_AB_OUTPUT_MVOLT_STEP );

  vout_default = ADDRESS(vout_default);

  err_flag |= pm_comm_write_byte_mask(SLAVE_ID, vout_default, 0xf, code, 0);

  return err_flag;
}



pm_err_flag_type pm_ab_module_ready_to_enable(uint32 device_index, boolean ab_rdy)
{
  pm_err_flag_type  err_flag  = PM_ERR_FLAG_SUCCESS;
  pm_register_data_type data;

  pm_ab_data_type *ab_ptr  = pm_ab_get_data(device_index);

  if (NULL == ab_ptr)
  {
    err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
  }
  else
  {
    pm_register_address_type module_rdy = ADDRESS(module_rdy);

    data = (ab_rdy)? 0x80 : 0x00;

    err_flag = pm_comm_write_byte_mask(SLAVE_ID, module_rdy, 0x80, data,0);
  }

  return err_flag;
}


pm_err_flag_type pm_ab_en_module(uint32 device_index, boolean en_module)
{
  pm_err_flag_type  err_flag  = PM_ERR_FLAG_SUCCESS;
  pm_register_data_type data;

  pm_ab_data_type *ab_ptr  = pm_ab_get_data(device_index);

  if (NULL == ab_ptr)
  {
    err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
  }
  else
  {
    pm_register_address_type enable_ctl = ADDRESS(enable_ctl);

    data = (en_module)? 0x80 : 0x00;

    err_flag |= pm_comm_write_byte_mask(SLAVE_ID, enable_ctl, 0x80, data,0);
  }

  return err_flag;
}


pm_err_flag_type pm_ab_ibb_rdy_en(uint32 device_index, boolean ibb_en_rdy)
{
  pm_err_flag_type  err_flag  = PM_ERR_FLAG_SUCCESS;
  pm_register_data_type data;

  pm_ab_data_type *ab_ptr  = pm_ab_get_data(device_index);

  if (NULL == ab_ptr)
  {
    err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
  }
  else
  {
    pm_register_address_type ibb_en_rdy_reg = ADDRESS(ibb_en_rdy);

    data = (ibb_en_rdy)? 0x80 : 0x00;

    err_flag = pm_comm_write_byte_mask(SLAVE_ID, ibb_en_rdy_reg, 0x80, data,0);
  }

  return err_flag;
}



pm_err_flag_type pm_ab_get_ab_status(uint32 device_index, pm_ab_status_type *ab_status)
{
  pm_err_flag_type  err_flag  = PM_ERR_FLAG_SUCCESS;
  pm_register_data_type data;

  pm_ab_data_type *ab_ptr  = pm_ab_get_data(device_index);

  if (NULL == ab_ptr)
  {
     err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
  }
  else if (NULL == ab_status)
  {
     err_flag = PM_ERR_FLAG_INVALID_PARAMETER;
  }
  else
  {
    pm_register_address_type module_rdy = ADDRESS(module_rdy);
    err_flag = pm_comm_read_byte(SLAVE_ID, module_rdy, &data, 0);
    ab_status->ab_module_rdy = (data & 0x80) ? TRUE : FALSE;
    pm_register_address_type enable_ctl = ADDRESS(enable_ctl);
    err_flag |= pm_comm_read_byte(SLAVE_ID, enable_ctl, &data, 0);
    ab_status->ab_module_en = (data & 0x80) ? TRUE : FALSE;

    pm_register_address_type ibb_en_rdy_reg = ADDRESS(ibb_en_rdy);
    err_flag |= pm_comm_read_byte(SLAVE_ID, ibb_en_rdy_reg, &data, 0);
    ab_status->ab_ibb_en_rdy = (data & 0x80) ? TRUE : FALSE;

    pm_register_address_type precharge_ctl = ADDRESS(precharge_ctl);
    err_flag |= pm_comm_read_byte(SLAVE_ID, precharge_ctl, &data, 0);
    ab_status->en_fast_precharge = (data & 0x04) ? TRUE : FALSE;
    ab_status->max_prechg_time_usec = ((data & 0x03) * 100) + 200;

    pm_register_address_type output_voltage = ADDRESS(output_voltage);
    err_flag |= pm_comm_read_byte(SLAVE_ID, output_voltage, &data, 0);
    ab_status->output_volt = ((data & 0xF) * 100) + 4600;

    pm_register_address_type status1 = ADDRESS(status1);
    err_flag |= pm_comm_read_byte(SLAVE_ID, status1, &data, 0);
    ab_status->ab_vreg_ok     = (data & 0x40) ? TRUE : FALSE;
    ab_status->short_circuit   = (data & 0x20) ? FALSE : TRUE; //if false then no short circuit else short circuit;

    pm_register_address_type current_sense = ADDRESS(current_sense);
    err_flag |= pm_comm_read_byte(SLAVE_ID, current_sense, &data, 0);
    ab_status->isense_tap  = (pm_ab_isense_type)(data & 0x0C);
    ab_status->isense_gain = (pm_ab_isense_type)(data & 0x03);;

    pm_register_address_type ps_ctl = ADDRESS(ps_ctl);
    err_flag |= pm_comm_read_byte(SLAVE_ID, ps_ctl, &data, 0);
    ab_status->ps_threshold = ((data & 0x3) * 10) + 50;
    ab_status->select_ps_table = (data & 0x04) ? TRUE : FALSE;
    ab_status->select_digital_ps = (data & 0x08) ? TRUE : FALSE;
    ab_status->enable_ps = (data & 0x80) ? TRUE : FALSE;
  }

  return err_flag;
}


pm_err_flag_type pm_ab_config_precharge_ctrl(uint32 device_index, boolean en_fast_precharge, uint32 max_prechg_time_usec)
{
  pm_err_flag_type  err_flag  = PM_ERR_FLAG_SUCCESS;
  pm_register_data_type data;

  pm_ab_data_type *ab_ptr  = pm_ab_get_data(device_index);

  if (NULL == ab_ptr)
  {
     err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
  }
  else
  {
    pm_register_address_type precharge_ctl = ADDRESS(precharge_ctl);
    /*range for max_prechg_time_usec 200 to 500*/
    max_prechg_time_usec = (max_prechg_time_usec < 200)? 200 : max_prechg_time_usec;
    max_prechg_time_usec = (max_prechg_time_usec>400)? 0x3 : ((max_prechg_time_usec-200)/100);
    data = (pm_register_data_type)((en_fast_precharge<<2) | max_prechg_time_usec);
    err_flag = pm_comm_write_byte_mask(SLAVE_ID, precharge_ctl, 0x07, data,0);
  }

  return err_flag;
}


pm_err_flag_type pm_ab_config_output_volt(uint32 device_index, boolean override_output_volt, uint32 volt_millivolt)
{
  pm_err_flag_type  err_flag  = PM_ERR_FLAG_SUCCESS;
  pm_register_data_type data;

  pm_ab_data_type *ab_ptr  = pm_ab_get_data(device_index);

  if (NULL == ab_ptr)
  {
    err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
  }
  else
  {
    pm_register_address_type output_voltage = ADDRESS(output_voltage);

    /*limit is 4600 to 6100*/
    volt_millivolt = (volt_millivolt < 4600)? 4600 : volt_millivolt;

    volt_millivolt = (volt_millivolt > 6100) ? 0xF : ((volt_millivolt - 4600)/100);

    data = (pm_register_data_type)(0xF & volt_millivolt);

    err_flag = pm_comm_write_byte_mask(SLAVE_ID, output_voltage, 0xF, data, 0);
  }

  return err_flag;
}



pm_err_flag_type pm_ab_config_current_sense(uint32 device_index, pm_ab_isense_type isense_tap, pm_ab_isense_type isense_gain)
{
  pm_err_flag_type  err_flag  = PM_ERR_FLAG_SUCCESS;
  pm_register_data_type data;
  uint32 tap_isense = 0;
  uint32 gain_isense = 0;

  pm_ab_data_type *ab_ptr  = pm_ab_get_data(device_index);

  if (isense_tap >= PM_AB_ISENSE_INVALID || isense_gain >= PM_AB_ISENSE_INVALID)
  {
    err_flag = PM_ERR_FLAG_INVALID_PARAMETER;
  }
  else if (NULL == ab_ptr)
  {
    err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
  }
  else
  {
    switch (isense_tap)
    {
      case PM_AB_ISENSE_0P8: tap_isense = 0x0 ; break;
      case PM_AB_ISENSE_0P9: tap_isense = 0x1 ; break;
      case PM_AB_ISENSE_1P0: tap_isense = 0x2 ; break;
      case PM_AB_ISENSE_1P1: tap_isense = 0x3 ; break;
      default: tap_isense =  0x1; //default value
    }

    switch (isense_gain)
    {
      case PM_AB_ISENSE_0P5: gain_isense = 0x0 ; break;
      case PM_AB_ISENSE_1P0: gain_isense = 0x1 ; break;
      case PM_AB_ISENSE_1P5: gain_isense = 0x2 ; break;
      case PM_AB_ISENSE_2P0: gain_isense = 0x3 ; break;
      default: gain_isense =  0x1; //default value
    }

    data = (tap_isense <<2)|gain_isense;

    pm_register_address_type current_sense = ADDRESS(current_sense);

    err_flag = pm_comm_write_byte_mask(SLAVE_ID, current_sense, 0x0F,data,0);
  }

  return err_flag;
}

pm_err_flag_type
pm_ab_config_pulse_skip_ctrl(uint32 device_index,
                             uint32 ps_threshold,
                             boolean enable_ps)
{
  pm_err_flag_type  err_flag  = PM_ERR_FLAG_SUCCESS;
  pm_register_data_type data= 0x00;

  pm_ab_data_type *ab_ptr  = pm_ab_get_data(device_index);

  if (NULL == ab_ptr)
  {
    err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
  }
  else
  {
    if (ps_threshold >80)
    {
       ps_threshold = 80;
    }
    else if (ps_threshold < 50)
    {
      ps_threshold = 50;
    }

    /*0=50, 1=60, 2 = 70, 3 = 80*/
     ps_threshold = (ps_threshold - 50)/10;

    data = (enable_ps << 7) | ps_threshold ;

    pm_register_address_type ps_ctl = ADDRESS(ps_ctl);

    err_flag = pm_comm_write_byte_mask(SLAVE_ID, ps_ctl, 0x83, data, 0);
  }

  return err_flag;
}


