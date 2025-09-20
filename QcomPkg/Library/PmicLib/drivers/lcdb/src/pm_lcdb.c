/*! \file
*  
*  \brief  pm_lcdb.c driver implementation.
*  \details charger driver implementation.
*  &copy;
*  Copyright (c) 2018 Qualcomm Technologies, Inc.  All Rights Reserved.
*  Qualcomm Technologies Proprietary and Confidential.
*/

/*===========================================================================

EDIT HISTORY FOR MODULE


when       	who     	 what, where, why
--------   	---    		 ---------------------------------------------------------- 
01/16/18        al               Initial revision
========================================================================== */

/*===========================================================================

					INCLUDE FILES

===========================================================================*/

#include "pm_lcdb.h"
#include "pm_lcdb_driver.h"
#include "CoreVerify.h"

/*===========================================================================
                       MACRO DECLARATIONS
===========================================================================*/

#define ADDRESS(x) (lcdb_ptr->lcdb_reg_table->base_address + lcdb_ptr->lcdb_reg_table->x)
#define SLAVE_ID   (lcdb_ptr->comm_ptr->slave_id)

#define PM_LCDB_BOOST_MIN_OUTPUT_VOLTAGE  4700
#define PM_LCDB_BOOST_MAX_OUTPUT_VOLTAGE  6275
#define PM_LCDB_BOOST_VOLTAGE_STEP_SIZE   25
#define PM_LCDB_BOOST_HEADROOM_MV         150


#define PM_LCDB_MIN_VOLTAGE            4000
#define PM_LCDB_MAX_VOLTAGE            6000
#define PM_LCDB_STEP_SIZE1              100
#define PM_LCDB_STEP_SIZE2               50
#define PM_LCDB_STEP_SIZE_CHANGE_VOLT  4900
#define PM_LCDB_STEP_SIZE_CHANGE_REG_VAL 0x09

#define PM_EN_NCP_VOUT_SYMMETRY_MASK 0x80
#define PM_BOOST_VOLT_MASK 0x3F
#define PM_NCP_VOLT_MASK 0x1F
#define PM_LDO_VOLT_MASK 0x1F

#define PM_LCDB_VREG_OK_MASK 0x80

/*===========================================================================

FUNCTION DEFINITIONS 

===========================================================================*/

static pm_register_data_type pm_get_voltage_reg_value(pm_lcdb_volt_type volt_type, int32 millivolt)
{
   pm_register_data_type data = 0;

   switch (volt_type)
   {
   case PM_LCDB_LDO_VOLT:
   case PM_LCDB_NCP_VOLT:
      {
         if (millivolt > PM_LCDB_MAX_VOLTAGE)
         {
            millivolt = PM_LCDB_MAX_VOLTAGE;
         }
         else if (millivolt < PM_LCDB_MIN_VOLTAGE)
         {
            millivolt = PM_LCDB_MIN_VOLTAGE;
         }

         if (millivolt > PM_LCDB_STEP_SIZE_CHANGE_VOLT)
         {
            data = PM_LCDB_STEP_SIZE_CHANGE_REG_VAL + ((millivolt - PM_LCDB_STEP_SIZE_CHANGE_VOLT) / PM_LCDB_STEP_SIZE2);
         }
         else
         {
            data = PM_LCDB_STEP_SIZE_CHANGE_REG_VAL - ((PM_LCDB_STEP_SIZE_CHANGE_VOLT - millivolt) / PM_LCDB_STEP_SIZE1);
         }
      }
      break;
   case PM_LCDB_BOOST_VOLT:
      {
         if (millivolt > PM_LCDB_BOOST_MAX_OUTPUT_VOLTAGE)
         {
            millivolt = PM_LCDB_BOOST_MAX_OUTPUT_VOLTAGE;
         }
         else if (millivolt < PM_LCDB_BOOST_MIN_OUTPUT_VOLTAGE)
         {
            millivolt = PM_LCDB_BOOST_MIN_OUTPUT_VOLTAGE;
         }

         data = (millivolt - PM_LCDB_BOOST_MIN_OUTPUT_VOLTAGE) / PM_LCDB_BOOST_VOLTAGE_STEP_SIZE;
      }
      break;
   default:
      CORE_VERIFY(0);
      break;
   }

   return data;
}

pm_err_flag_type pm_lcdb_set_output_volt(uint32 device_index, int32 ldo_mv, int32 ncp_mv, boolean symmetry_ok)
{
   pm_err_flag_type  err_flag  = PM_ERR_FLAG_SUCCESS;

   pm_register_data_type ldo_data = 0, ncp_data = 0, boost_data = 0, current_boost_data = 0;
   int32 boost_volt = 0;

   pm_lcdb_data_type *lcdb_ptr  = pm_lcdb_get_data(device_index);

   if (NULL == lcdb_ptr)
   {
      err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
   }
   else
   {
      pm_register_address_type ldo_output_voltage = ADDRESS(ldo_output_voltage);
      pm_register_address_type ncp_output_voltage = ADDRESS(ncp_output_voltage);
      pm_register_address_type bst_output_voltage = ADDRESS(bst_output_voltage);
      
      ldo_mv = ABS(ldo_mv);
      ldo_data = pm_get_voltage_reg_value(PM_LCDB_LDO_VOLT, ldo_mv);

      if (symmetry_ok)
      {
         ncp_mv = ldo_mv;
         ncp_data = ldo_data | PM_EN_NCP_VOUT_SYMMETRY_MASK;
      }
      else
      {
         ncp_mv = ABS(ncp_mv);
         ncp_data = pm_get_voltage_reg_value(PM_LCDB_NCP_VOLT, ncp_mv);
      }

      boost_volt = MAX(ldo_mv, ncp_mv) + PM_LCDB_BOOST_HEADROOM_MV;
      boost_data = pm_get_voltage_reg_value(PM_LCDB_BOOST_VOLT, boost_volt);

      err_flag |= pm_comm_read_byte(SLAVE_ID, bst_output_voltage, &current_boost_data, 0);
      current_boost_data = current_boost_data & PM_BOOST_VOLT_MASK;
      /*if current boost setting is more than MAX(LDO,NCP) then first change the LDO and NCP voltage 
        otherwise first change boost voltage*/
      if (current_boost_data >= boost_data)
      {
         //set ldo and ncp
         err_flag |= pm_comm_write_byte(SLAVE_ID, ldo_output_voltage, ldo_data, 0);
         err_flag |= pm_comm_write_byte(SLAVE_ID, ncp_output_voltage, ncp_data, 0);
         //set boost
         err_flag |= pm_comm_write_byte(SLAVE_ID, bst_output_voltage, boost_data, 0);
      }
      else
      {
         //set boost
         err_flag |= pm_comm_write_byte(SLAVE_ID, bst_output_voltage, boost_data, 0);
         //set ldo and ncp
         err_flag |= pm_comm_write_byte(SLAVE_ID, ldo_output_voltage, ldo_data, 0);
         err_flag |= pm_comm_write_byte(SLAVE_ID, ncp_output_voltage, ncp_data, 0);
      }
   }

   return err_flag;
}


pm_err_flag_type pm_lcdb_set_hwen_debounce(uint32 device_index, boolean enable)
{
   pm_err_flag_type  err_flag  = PM_ERR_FLAG_SUCCESS;
   pm_register_data_type data;

   pm_lcdb_data_type *lcdb_ptr  = pm_lcdb_get_data(device_index);

   if (NULL == lcdb_ptr)
   {
      err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
   }
   else
   {
      pm_register_address_type hwen_ctl_reg = ADDRESS(hwen_ctl);

      data = (enable) ? 0x1 : 0x0 ;

      err_flag = pm_comm_write_byte_mask(SLAVE_ID, hwen_ctl_reg, 0x1, data, 0);
   }

   return err_flag;
}

pm_err_flag_type pm_lcdb_config_enable_ctrl(uint32 device_index, pm_lcdb_enable_ctl_type ctl_type, boolean enable)
{
   pm_err_flag_type  err_flag  = PM_ERR_FLAG_SUCCESS;
   pm_register_data_type data = (enable) ? 0xFF : 0;
   pm_register_data_type mask = 0;

   pm_lcdb_data_type *lcdb_ptr  = pm_lcdb_get_data(device_index);

   if (NULL == lcdb_ptr)
   {
      err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
   }
   else if (ctl_type >= PM_LCDB_ENABLE_CTL_INVALID_TYPE)
   {
      err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
   }
   else
   {
      pm_register_address_type module_rdy = ADDRESS(module_rdy);
      
      pm_register_address_type enable_ctl = ADDRESS(enable_ctl1);

      mask = (ctl_type == PM_LCDB_ENABLE_CTL_MODULE_EN) ? (0x1 << 7) : (0x1 << 6);

      if (data)
      {
         //to enable module, module_rdy need to be enabled
         err_flag = pm_comm_write_byte_mask(SLAVE_ID, module_rdy, 0x80, 0x80,0);
      }

      err_flag |= pm_comm_write_byte_mask(SLAVE_ID, enable_ctl, mask, data,0);
   }

   return err_flag;
}


pm_err_flag_type pm_lcdb_config_pulldown_ctrl(uint32 device_index, pm_lcdb_pulldown_ctl_type ctrl_type, boolean ctrl)
{
   pm_err_flag_type  err_flag  = PM_ERR_FLAG_SUCCESS;
   pm_register_data_type data = (ctrl) ? 0xFF : 0;
   pm_register_data_type mask = 0;

   pm_lcdb_data_type *lcdb_ptr  = pm_lcdb_get_data(device_index);

   if (NULL == lcdb_ptr)
   {
      err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
   }
   else if (ctrl_type >= PM_LCDB_PULLDOWN_CTL_INVALID_TYPE)
   {
      err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
   }
   else
   {
      pm_register_address_type enable_ctl = ADDRESS(pd_ctl);
      mask = (ctrl_type == PM_LCDB_PULLDOWN_CTL_ENABLE) ? (0x1 << 1) : 0x1;
      err_flag |= pm_comm_write_byte_mask(SLAVE_ID, enable_ctl, mask, data,0);
   }

   return err_flag;
}

pm_err_flag_type pm_lcdb_vreg_ok(uint32 device_index, boolean *vreg_ok)
{
   pm_err_flag_type  err_flag  = PM_ERR_FLAG_SUCCESS;
   pm_register_data_type data = 0;

   pm_lcdb_data_type *lcdb_ptr  = pm_lcdb_get_data(device_index);

   if (NULL == lcdb_ptr)
   {
      err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
   }
   else if (NULL == vreg_ok)
   {
      err_flag = PM_ERR_FLAG_INVALID_PARAMETER;
   }
   else
   {
      pm_register_address_type status1 = ADDRESS(status1);
      err_flag |= pm_comm_read_byte(SLAVE_ID, status1, &data,0);
      *vreg_ok = (data & PM_LCDB_VREG_OK_MASK)? TRUE : FALSE;
   }

   return err_flag;
}

