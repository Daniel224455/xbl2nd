/*! \file
*
*  \brief  pm_oledb.c driver implementation.
*  \details charger driver implementation.
*  &copy;
*
*  Copyright (c) 2018 Qualcomm Technologies, Inc.  All Rights Reserved.
*  Qualcomm Technologies Proprietary and Confidential.
*/

/*===========================================================================

EDIT HISTORY FOR MODULE


when        who        what, where, why
--------    ---        ----------------------------------------------------------
01/16/18        al               Initial revision
========================================================================== */

/*===========================================================================

          INCLUDE FILES

===========================================================================*/

#include "pm_oledb.h"
#include "pm_oledb_driver.h"
#include "pm_log_utils.h"
/*===========================================================================
                        MACRO DECLARATIONS
===========================================================================*/
#define ADDRESS(reg)   (oledb_ptr->oledb_reg_table->base_address + oledb_ptr->oledb_reg_table->reg)
#define SLAVE_ID     (oledb_ptr->comm_ptr->slave_id)

#define PM_OLEDB_BASE_OUTPUT_VOLTAGE 4925  //Base Minimum for register
#define PM_OLEDB_MAX_OUTPUT_VOLTAGE  8000  //Recommended Maximum
#define PM_OLEDB_MIN_OUTPUT_VOLTAGE  5000  //Recommended Minimum
#define PM_OLEDB_VOLTAGE_STEP_SIZE   25

/*===========================================================================
                        FUNCTION DEFINITIONS
===========================================================================*/
pm_err_flag_type
pm_oledb_config_default_volt(uint32 device_index, uint32 milli_volt)
{
  pm_err_flag_type         err_flag     = PM_ERR_FLAG_SUCCESS;
  pm_register_data_type    code         = 0;
  pm_oledb_data_type       *oledb_ptr   = pm_oledb_get_data(device_index);
  pm_register_address_type vout_def_ctl = 0;
  if (NULL == oledb_ptr)
  {
    return PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
  }

  milli_volt = milli_volt < PM_OLEDB_MIN_OUTPUT_VOLTAGE ?
               PM_OLEDB_MIN_OUTPUT_VOLTAGE : milli_volt;

  milli_volt = milli_volt > PM_OLEDB_MAX_OUTPUT_VOLTAGE ?
               PM_OLEDB_MAX_OUTPUT_VOLTAGE : milli_volt;

  milli_volt -= PM_OLEDB_BASE_OUTPUT_VOLTAGE;

  //Round off for step size to ceiling
  code = (pm_register_data_type)((milli_volt+(PM_OLEDB_VOLTAGE_STEP_SIZE - 1))/
                                 PM_OLEDB_VOLTAGE_STEP_SIZE );

  vout_def_ctl = ADDRESS(vout_default_ctl);

  err_flag |= pm_comm_write_byte_mask(SLAVE_ID, vout_def_ctl, 0x7f, code, 0);

  return err_flag;
}

pm_err_flag_type
pm_oledb_mode_config(uint32 device_index, pm_oledb_mode_cfg_type mode)
{
  pm_err_flag_type         err_flag     = PM_ERR_FLAG_SUCCESS;
  pm_register_data_type    data         = 0;
  pm_oledb_data_type       *oledb_ptr   = pm_oledb_get_data(device_index);
  pm_register_address_type mode_config  = 0;

  if (NULL == oledb_ptr)
  {
    return PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
  }

  if(mode >= PM_OLEDB_MODE_CFG_INVALID)
  {
    return PM_ERR_FLAG_INVALID_PARAMETER;
  }

  mode_config = ADDRESS(mode_config);

  data = (pm_register_data_type)(mode << 7);

  err_flag |= pm_comm_write_byte_mask(SLAVE_ID, mode_config, 0x80, data, 0);

  return err_flag;
}



pm_err_flag_type
pm_oledb_module_ready_to_be_enabled(uint32 device_index, boolean oledb_rdy)
{
  pm_err_flag_type  err_flag  = PM_ERR_FLAG_SUCCESS;
  pm_register_data_type data;

  pm_oledb_data_type *oledb_ptr  = pm_oledb_get_data(device_index);

  if (NULL == oledb_ptr)
  {
    err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
  }
  else
  {
    pm_register_address_type module_rdy = ADDRESS(module_rdy);
    data = (oledb_rdy) ? 0x80 : 0x00;
    err_flag = pm_comm_write_byte_mask(SLAVE_ID, module_rdy, 0x80, data, 0);
  }

  return err_flag;
}

pm_err_flag_type pm_oledb_enable_module(uint32 device_index, boolean enable)
{
  pm_err_flag_type  err_flag  = PM_ERR_FLAG_SUCCESS;
  pm_register_data_type data;

  pm_oledb_data_type *oledb_ptr  = pm_oledb_get_data(device_index);

  if (NULL == oledb_ptr)
  {
    err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
  }
  else
  {
    pm_register_address_type module_rdy = ADDRESS(module_rdy);

    pm_register_address_type module_enable = ADDRESS(module_enable);

    data = (enable) ? 0x80 : 0x00;

    if (data)
    {
      //to enable module, first module ready needs to be enabled
      err_flag = pm_comm_write_byte_mask(SLAVE_ID, module_rdy, 0x80, 0x80, 0);
    }

    err_flag = pm_comm_write_byte_mask(SLAVE_ID, module_enable, 0x80, data, 0);
  }

  return err_flag;
}


pm_err_flag_type
pm_oledb_enable_ext_pin_ctl(uint32 device_index, boolean enable)
{
  pm_err_flag_type  err_flag  = PM_ERR_FLAG_SUCCESS;
  pm_register_data_type data;

  pm_oledb_data_type *oledb_ptr  = pm_oledb_get_data(device_index);

  if (NULL == oledb_ptr)
  {
    err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
  }
  else
  {
    pm_register_address_type ext_pin_ctl = ADDRESS(ext_pin_ctl);
    data = (enable) ? 0x80 : 0x00;
    err_flag = pm_comm_write_byte_mask(SLAVE_ID, ext_pin_ctl, 0x80, data, 0);
  }

  return err_flag;
}

pm_err_flag_type
pm_oledb_set_output_volt(uint32 device_index, uint32 vout_mv)
{
  pm_err_flag_type  err_flag  = PM_ERR_FLAG_SUCCESS;
  pm_register_data_type data;

  pm_oledb_data_type *oledb_ptr  = pm_oledb_get_data(device_index);

  if (NULL == oledb_ptr)
  {
    err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
  }
  else
  {
    pm_register_address_type vout_pgm = ADDRESS(vout_pgm);

    if (vout_mv > PM_OLEDB_MAX_OUTPUT_VOLTAGE)
    {
      vout_mv = PM_OLEDB_MAX_OUTPUT_VOLTAGE;
    }
    else if (vout_mv < PM_OLEDB_MIN_OUTPUT_VOLTAGE)
    {
       vout_mv = PM_OLEDB_MIN_OUTPUT_VOLTAGE;
    }

    data = (vout_mv - PM_OLEDB_BASE_OUTPUT_VOLTAGE) / PM_OLEDB_VOLTAGE_STEP_SIZE;

    err_flag |= pm_comm_write_byte_mask(SLAVE_ID, vout_pgm, 0x7F, data, 0);
  }

  return err_flag;
}

