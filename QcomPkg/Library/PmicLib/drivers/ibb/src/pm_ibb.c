/*! \file
*  
*  \brief  pm_ibb.c driver implementation.
*  \details charger driver implementation.
*  &copy;
*  Copyright (c) 2013-2017 Qualcomm Technologies, Inc.  All Rights Reserved.
*  Qualcomm Technologies Proprietary and Confidential.
*/

/*===========================================================================

EDIT HISTORY FOR MODULE


when        who        what, where, why
--------    ---        ---------------------------------------------------------- 
03/29/16        al               Updating for newer target
08/20/14        al               Updating comm lib 
07/03/14        al               Adding API for delay config
05/20/14        al               Architecture update 
04/30/14        al               Initial revision
========================================================================== */

/*===========================================================================

          INCLUDE FILES

===========================================================================*/

#include "pm_ibb.h"
#include "pm_ibb_driver.h"

#define OUTPUT_VOLTAGE_STEP_SIZE  25
#define MAX_IBB_OUTPUT_VOLTAGE  5400
#define MIN_IBB_OUTPUT_VOLTAGE  1400


pm_err_flag_type pm_ibb_ibb_module_rdy(uint32 device_index, boolean ibb_rdy)
{
  pm_err_flag_type  err_flag  = PM_ERR_FLAG_SUCCESS;
  pm_register_data_type data;

  pm_ibb_data_type *ibb_ptr  = pm_ibb_get_data(device_index);

  if (NULL == ibb_ptr)
  {
    err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
  }
  else
  {
    pm_register_address_type module_rdy = ibb_ptr->ibb_reg_table->base_address + ibb_ptr->ibb_reg_table->module_rdy;
    data = (ibb_rdy)? 0x80 : 0x00;
    err_flag = pm_comm_write_byte_mask(ibb_ptr->comm_ptr->slave_id, module_rdy, 0x80, data,0);
  }

  return err_flag;
}


pm_err_flag_type pm_ibb_config_ibb_ctrl(uint32 device_index, boolean en_module, boolean swire_rdy)
{
  pm_err_flag_type  err_flag  = PM_ERR_FLAG_SUCCESS;
  pm_register_data_type data;

  pm_ibb_data_type *ibb_ptr  = pm_ibb_get_data(device_index);

  if (NULL == ibb_ptr)
  {
    err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
  }
  else
  {
    pm_register_address_type enable_ctl = ibb_ptr->ibb_reg_table->base_address + ibb_ptr->ibb_reg_table->enable_ctl;
    data = (pm_register_data_type)(en_module<<7 | swire_rdy<<6);
    err_flag |= pm_comm_write_byte_mask(ibb_ptr->comm_ptr->slave_id, enable_ctl, 0xC0, data,0);
  }

  return err_flag;
}


pm_err_flag_type
pm_ibb_config_output_volt(uint32 device_index, int32 milli_volt)
{
  pm_err_flag_type  err_flag  = PM_ERR_FLAG_SUCCESS;
  pm_register_data_type data;  

  pm_ibb_data_type *ibb_ptr  = pm_ibb_get_data(device_index);

  if (NULL == ibb_ptr)
  {
    err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
  }
  else
  {
    pm_register_address_type output_voltage = ibb_ptr->ibb_reg_table->base_address + ibb_ptr->ibb_reg_table->output_voltage;
    
    milli_volt = ABS(milli_volt);
    
    if(milli_volt > MAX_IBB_OUTPUT_VOLTAGE)
    {
      data = MAX_IBB_OUTPUT_VOLTAGE / OUTPUT_VOLTAGE_STEP_SIZE;
    }
    else if(milli_volt < MIN_IBB_OUTPUT_VOLTAGE)
    {
      data = MIN_IBB_OUTPUT_VOLTAGE / OUTPUT_VOLTAGE_STEP_SIZE;
    }
    else
    {
     data = milli_volt / OUTPUT_VOLTAGE_STEP_SIZE;
    }
    
    err_flag = pm_comm_write_byte(ibb_ptr->comm_ptr->slave_id, output_voltage, data, 0);
  }

  return err_flag;
}

pm_err_flag_type
pm_ibb_config_default_volt(uint32 device_index, int32 milli_volt)
{
  pm_err_flag_type         err_flag        = PM_ERR_FLAG_SUCCESS;
  pm_register_data_type    data            = 0;
  pm_register_address_type default_voltage = 0;
  pm_ibb_data_type         *ibb_ptr        = pm_ibb_get_data(device_index);

  if (NULL == ibb_ptr)
  {
    return PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
  }

  default_voltage = ibb_ptr->ibb_reg_table->base_address +
                    ibb_ptr->ibb_reg_table->default_voltage;

  milli_volt = ABS(milli_volt);

  milli_volt = milli_volt < MIN_IBB_OUTPUT_VOLTAGE ?
               MIN_IBB_OUTPUT_VOLTAGE : milli_volt;

  milli_volt = milli_volt > MAX_IBB_OUTPUT_VOLTAGE ?
               MAX_IBB_OUTPUT_VOLTAGE : milli_volt;

  //Round off to the high ceiling based on step-size
  milli_volt += (OUTPUT_VOLTAGE_STEP_SIZE - 1);

  data = (pm_register_data_type)(milli_volt / OUTPUT_VOLTAGE_STEP_SIZE);

  err_flag = pm_comm_write_byte(ibb_ptr->comm_ptr->slave_id, 
                                default_voltage, data, 0);

  return err_flag;
}

pm_err_flag_type pm_ibb_config_pwrup_pwrdn_dly(uint32 device_index, pm_ibb_dly_amnt_type pwrup_dly, pm_ibb_dly_amnt_type pwrdn_dly)
{
  pm_err_flag_type  err_flag  = PM_ERR_FLAG_SUCCESS;
  pm_register_data_type pwrup_dly_data = 0, pwrdn_dly_data = 0;

  pm_ibb_data_type *ibb_ptr  = pm_ibb_get_data(device_index);

  if (NULL == ibb_ptr)
  {
     err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
  }
  else if(pwrup_dly >= PM_IBB_DELAY_AMNT_INVALID || pwrdn_dly >= PM_IBB_DELAY_AMNT_INVALID)
  {
    err_flag = PM_ERR_FLAG_INVALID_PARAMETER;
  }
  else
  {
    pwrup_dly_data = (pm_register_data_type)(pwrup_dly & 0xFF);
    
    pwrdn_dly_data = (pm_register_data_type)(pwrdn_dly & 0xFF);
    
    pm_register_address_type pwrup_pwrdn_ctl_1 = ibb_ptr->ibb_reg_table->base_address + ibb_ptr->ibb_reg_table->pwrup_pwrdn_ctl_1;
    pm_register_address_type pwrup_pwrdn_delay = ibb_ptr->ibb_reg_table->base_address + ibb_ptr->ibb_reg_table->pwrup_pwrdn_delay;
    
    err_flag = pm_comm_write_byte(ibb_ptr->comm_ptr->slave_id, pwrup_pwrdn_delay, (pwrup_dly_data << 4) | pwrdn_dly_data, 0);
    
    err_flag = pm_comm_write_byte_mask(ibb_ptr->comm_ptr->slave_id, pwrup_pwrdn_ctl_1, 0x48, 0x48,0); //en_pwrup_dly, en_pwrdn_dly
  }

  return err_flag;
}

pm_err_flag_type
pm_ibb_get_ibb_status(uint32 device_index, pm_ibb_status_type* ibb_status)
{
  pm_err_flag_type  err_flag  = PM_ERR_FLAG_SUCCESS;
  pm_register_data_type data;

  pm_ibb_data_type *ibb_ptr  = pm_ibb_get_data(device_index);

  if (NULL == ibb_ptr)
  {
    err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
  }
  else if (NULL == ibb_status)
  {
    err_flag = PM_ERR_FLAG_INVALID_PARAMETER;
  }
  else
  {
    pm_register_address_type output_voltage = ibb_ptr->ibb_reg_table->base_address + ibb_ptr->ibb_reg_table->output_voltage;
    err_flag = pm_comm_read_byte(ibb_ptr->comm_ptr->slave_id, output_voltage, &data, 0);
    ibb_status->output_volt = data * OUTPUT_VOLTAGE_STEP_SIZE;
  
    pm_register_address_type default_voltage = ibb_ptr->ibb_reg_table->base_address + ibb_ptr->ibb_reg_table->default_voltage;
    err_flag = pm_comm_read_byte(ibb_ptr->comm_ptr->slave_id, default_voltage, &data, 0);
    ibb_status->default_volt = data * OUTPUT_VOLTAGE_STEP_SIZE;

    pm_register_address_type enable_ctl = ibb_ptr->ibb_reg_table->base_address + ibb_ptr->ibb_reg_table->enable_ctl;
    err_flag |= pm_comm_read_byte(ibb_ptr->comm_ptr->slave_id, enable_ctl, &data,0);
    ibb_status->ibb_module_en = (data & 0x80) ? TRUE : FALSE;
    ibb_status->swire_rdy = (data & 0x40)? TRUE : FALSE;

    pm_register_address_type module_rdy = ibb_ptr->ibb_reg_table->base_address + ibb_ptr->ibb_reg_table->module_rdy;
    err_flag = pm_comm_read_byte(ibb_ptr->comm_ptr->slave_id, module_rdy, &data, 0);
    ibb_status->ibb_module_rdy = (data & 0x80)? TRUE : FALSE;

    pm_register_address_type pwrup_pwrdn_delay = ibb_ptr->ibb_reg_table->base_address + ibb_ptr->ibb_reg_table->pwrup_pwrdn_delay;
    err_flag |= pm_comm_read_byte(ibb_ptr->comm_ptr->slave_id, pwrup_pwrdn_delay, &data,0);

    ibb_status->pwrdn_dly     = (data & 0xF);
    ibb_status->pwrup_dly    = ((data >>4) & 0xF);

    pm_register_address_type status1 = ibb_ptr->ibb_reg_table->base_address + ibb_ptr->ibb_reg_table->status1;
    err_flag |= pm_comm_read_byte(ibb_ptr->comm_ptr->slave_id, status1, &data,0);
    
    ibb_status->ibb_vreg_ok  =  (data & 0x80) ? TRUE: FALSE; 
    ibb_status->overload = (data & 0x40) ? TRUE: FALSE;
    ibb_status->ilimit_error =  (data & 0x20) ? TRUE: FALSE;
  }

  return err_flag ;
};

