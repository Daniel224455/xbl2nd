
/*! \file pm_fg_adc_rr.c 
*  \n
*  \brief  PMIC-BMS MODULE RELATED DECLARATION
*  \details  This file contains functions and variable declarations to support 
*   the PMIC Fule Gauge RR ADC .
*
*  Copyright (c) 2016-2018 Qualcomm Technologies, Inc.  All Rights Reserved. 
*  Qualcomm Technologies Proprietary and Confidential.
*/

/*===========================================================================

EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.


when          who     what, where, why
--------      ---     ----------------------------------------------------------
01/30/19      pbitra  Added support to set batt therm freq
08/01/18      ivy     Added API to set batt therm pull up to register
06/18/18      cs      turn off FG_BTM for batt_id after ID is successfully read to avoid adc corruption for 1.0
03/21/18      cs      fixed reading battery temperature
03/05/18      pxm     Remove unused APIs and registers.
01/29/18      cs      added function to get batt_id_adc, set_batt_therm_cfg1
09/19/17      cs      for SDM855 remove unsupported functions.
06/13/17      czq     correct API pm_fg_adc_rr_batt_id_cfg error.
03/15/17      cs      fixed wrong function name 
10/06/16      cs      configure batt therm/aux therm bias wait 
08/25/16      pxm     Add skin temperature threshold configuration API
04/01/16      aab     Added more ADC RR APIs
01/27/16      al      Initial Release 
===========================================================================*/

#include "pm_fg_adc_rr.h"
#include "pm_fg_driver.h"


/*===========================================================================
                        TYPE DEFINITIONS 
===========================================================================*/
#define ADDRESS(x) (fg_adc_rr_ptr->fg_register->adc_rr_register->base_address + fg_adc_rr_ptr->fg_register->adc_rr_register->x)
#define ADDRESS_BTM(x) (fg_adc_rr_ptr->fg_register->adc_fg5_btm_register->base_address + fg_adc_rr_ptr->fg_register->adc_fg5_btm_register->x)
#define SLAVE_ID   (fg_adc_rr_ptr->comm_ptr->slave_id)

#define PM_FG_K_CONSTANT 273
#define  PM_FG_ADC_RR_RR_CLEAR_CH_LOG_WAIT   1000   //1ms

/*===========================================================================
                         FUNCTION DEFINITIONS
===========================================================================*/

pm_err_flag_type pm_fg_adc_rr_irq_status(uint32 pmic_device, pm_fg_adc_rr_irq_type irq, pm_irq_status_type type, boolean *status)
{
  pm_err_flag_type    err_flag    = PM_ERR_FLAG_SUCCESS;
  pm_register_data_type data;
  uint8 mask = 1 << irq;
  pm_register_address_type int_sts;
  pm_fg_data_type* fg_adc_rr_ptr = pm_fg_get_data(pmic_device);

  if (NULL == fg_adc_rr_ptr)
  {
    err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
  }
  else if ((irq >= PM_FG_ADC_RR_IRQ_INVALID) || (NULL == status))
  {
    err_flag = PM_ERR_FLAG_INVALID_PARAMETER;
  }
  else
  {
    switch (type)
    {
    case PM_IRQ_STATUS_RT:
      int_sts = ADDRESS(int_rt_sts);
      break;
    case PM_IRQ_STATUS_LATCHED:
      int_sts = ADDRESS(int_latched_sts);
      break;
    case PM_IRQ_STATUS_PENDING:
      int_sts = ADDRESS(int_pending_sts);
      break;
    default:
      return PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }

    err_flag = pm_comm_read_byte_mask(SLAVE_ID, int_sts, mask, &data, 0);
    *status = (data != 0) ? TRUE : FALSE;
  }

  return err_flag;
}


#define BIT_SW_SEQUENCE_INITIATE 1
pm_err_flag_type pm_fg_adc_rr_make_sw_req(uint32 pmic_device)
{
    pm_err_flag_type    err_flag    = PM_ERR_FLAG_SUCCESS;
    pm_fg_data_type* fg_adc_rr_ptr = pm_fg_get_data(pmic_device);

    if (NULL == fg_adc_rr_ptr)
    {
        err_flag |= PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    else
    {
        pm_register_address_type reg = ADDRESS(adc_ctl0);
        pm_register_data_type data = 0;
        pm_register_data_type mask = PM_BIT(BIT_SW_SEQUENCE_INITIATE);

        err_flag = pm_comm_read_byte(SLAVE_ID, reg, &data, 0);
        if(PM_ERR_FLAG_SUCCESS == err_flag)
        {
            data = ~data;
            err_flag = pm_comm_write_byte_mask(SLAVE_ID, reg, mask, data, 0);
        }
    }

    return err_flag;
}

pm_err_flag_type pm_fg_adc_rr_clear_channel_log(uint32 pmic_device)
{
   pm_err_flag_type    err_flag    = PM_ERR_FLAG_SUCCESS;
    pm_register_data_type data;
   pm_fg_data_type* fg_adc_rr_ptr = pm_fg_get_data(pmic_device);

   if (NULL == fg_adc_rr_ptr)
   {
    err_flag |= PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
   }
   else
   {
        pm_register_address_type reg = ADDRESS(adc_ctl1);

        // Write 0x01;  wait 10us ; Write 0x00
        data = 0x01;
      err_flag = pm_comm_write_byte(SLAVE_ID, reg, data, 0);

        pm_busy_wait(PM_FG_ADC_RR_RR_CLEAR_CH_LOG_WAIT); //1ms wait

        data = 0x00;
        err_flag |= pm_comm_write_byte(SLAVE_ID, reg, data, 0);
    }

   return err_flag;
}

pm_err_flag_type pm_fg_adc_rr_set_fake_batt_id_range(uint32 pmic_device, uint32 fake_batt_min, uint32 fake_batt_max)
{
   pm_err_flag_type    err_flag    = PM_ERR_FLAG_SUCCESS;
   pm_register_address_type reg = 0;
   pm_register_data_type    fake_batt_reg_data[4] = {0};
   
   pm_fg_data_type* fg_adc_rr_ptr = pm_fg_get_data(pmic_device);

   if (NULL == fg_adc_rr_ptr)
   {
    err_flag |= PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
   }
    else if (fake_batt_min > fake_batt_max)
   {
    err_flag |= PM_ERR_FLAG_INVALID_PARAMETER;
   }
   else
   {
      reg = ADDRESS(fake_batt_low_lsb);
        fake_batt_reg_data[0] = fake_batt_min & 0xFF;  //fake_batt_low_lsb
        fake_batt_reg_data[1] = (fake_batt_min >> 8) & 0xFF;  //fake_batt_low_msb
        fake_batt_reg_data[2] = fake_batt_max & 0xFF;  //fake_batt_high_lsb
        fake_batt_reg_data[3] = (fake_batt_max >> 8) & 0xFF;  //fake_batt_high_msb
	  
      err_flag = pm_comm_write_byte_array(SLAVE_ID, reg, 4, fake_batt_reg_data, 0);
   }

   return err_flag;
}

pm_err_flag_type pm_fg_batt_info_get_batt_temp(uint32 pmic_device, int32 *batt_temp)
{ 
  pm_err_flag_type    err_flag    = PM_ERR_FLAG_SUCCESS;

  pm_register_data_type data_lsb = 0;
  pm_fg_data_type* fg_adc_rr_ptr = pm_fg_get_data(pmic_device);
  INT8 temp_rawValue = 0;
  if (NULL == fg_adc_rr_ptr)
  {
    err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
  }
  else if (NULL == batt_temp)
  {
    err_flag = PM_ERR_FLAG_INVALID_PARAMETER;
  }
  else
  {
    //battery temperature is 8 bit signed value, 1 lsb = 1 degree C.
    pm_register_address_type batt_temp_lsb_reg = ADDRESS(batt_temperature_lsb);
    err_flag = pm_comm_read_byte(SLAVE_ID, batt_temp_lsb_reg, &data_lsb, 0);
    temp_rawValue = (INT8)data_lsb;
    *batt_temp = temp_rawValue;
  }

  return err_flag;
}

pm_err_flag_type pm_fg_batt_info_get_batt_id_adc(uint32 pmic_device, pm_fg_adc_rr_batt_id_bias batt_id_bias, int16 *batt_id_adc)
{
  pm_err_flag_type    err_flag = PM_ERR_FLAG_SUCCESS;

  pm_register_data_type data_lsb = 0, data_msb = 0;
  pm_fg_data_type* fg_adc_rr_ptr = pm_fg_get_data(pmic_device);
  if (NULL == fg_adc_rr_ptr)
  {
    err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
  }
  else if (NULL == batt_id_adc)
  {
    err_flag = PM_ERR_FLAG_INVALID_PARAMETER;
  }
  else
  {
    pm_register_address_type batt_id_lsb_reg = 0;
    pm_register_address_type batt_id_msb_reg = 0;
    switch (batt_id_bias)
    {
    case PM_FG_ADC_RR_BATT_ID_MED_BIAS:
    default:
      batt_id_lsb_reg = ADDRESS(batt_id_med_bias_lsb);
      batt_id_msb_reg = ADDRESS(batt_id_med_bias_msb);
      break;
    case PM_FG_ADC_RR_BATT_ID_LOW_BIAS:
      batt_id_lsb_reg = ADDRESS(batt_id_lo_bias_lsb);
      batt_id_msb_reg = ADDRESS(batt_id_lo_bias_msb);
      break;
    case PM_FG_ADC_RR_BATT_ID_HI_BIAS:
      batt_id_lsb_reg = ADDRESS(batt_id_hi_bias_lsb);
      batt_id_msb_reg = ADDRESS(batt_id_hi_bias_msb);
      break;
    }
    err_flag = pm_comm_read_byte(SLAVE_ID, batt_id_lsb_reg, &data_lsb, 0);
    err_flag |= pm_comm_read_byte(SLAVE_ID, batt_id_msb_reg, &data_msb, 0);
    *batt_id_adc = (data_msb << 8) | data_lsb;
  }

  return err_flag;
}


pm_err_flag_type pm_fg5_btm_set_en_ctl1(uint32 pmic_device, uint8 enable)
{
  pm_err_flag_type    err_flag = PM_ERR_FLAG_SUCCESS;
  pm_register_address_type reg = 0;

  pm_fg_data_type* fg_adc_rr_ptr = pm_fg_get_data(pmic_device);

  if (NULL == fg_adc_rr_ptr)
  {
    err_flag |= PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
  }
  else
  {
    reg = ADDRESS_BTM(en_ctrl1);

    err_flag = pm_comm_write_byte(SLAVE_ID, reg, (enable << 7), 0);
  }

  return err_flag;
}

pm_err_flag_type pm_fg5_btm_set_btm_en_1(uint32 pmic_device, pm_fg_btm_en_1 enable_type, uint8 enable)
{
  pm_err_flag_type    err_flag = PM_ERR_FLAG_SUCCESS;
  pm_register_address_type reg = 0;
  pm_register_data_type data = 0;

  pm_fg_data_type* fg_adc_rr_ptr = pm_fg_get_data(pmic_device);

  if (NULL == fg_adc_rr_ptr)
  {
    err_flag |= PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
  }
  else
  {
    reg = ADDRESS_BTM(btm_en_1);
    data = enable << enable_type;

    err_flag = pm_comm_write_byte(SLAVE_ID, reg, data, 0);
  }

  return err_flag;
}

pm_err_flag_type pm_fg5_btm_set_btm_ctl_0(uint32 pmic_device, uint8 btm_ctl)
{
  pm_err_flag_type    err_flag = PM_ERR_FLAG_SUCCESS;
  pm_register_address_type reg = 0;

  pm_fg_data_type* fg_adc_rr_ptr = pm_fg_get_data(pmic_device);

  if (NULL == fg_adc_rr_ptr)
  {
    err_flag |= PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
  }
  else
  {
    reg = ADDRESS_BTM(btm_ctl_0);

    err_flag = pm_comm_write_byte(SLAVE_ID, reg, btm_ctl, 0);
  }

  return err_flag;
}

pm_err_flag_type pm_fg5_btm_set_conv_req(uint32 pmic_device, uint8 operation)
{
  pm_err_flag_type    err_flag = PM_ERR_FLAG_SUCCESS;
  pm_register_address_type reg = 0;

  pm_fg_data_type* fg_adc_rr_ptr = pm_fg_get_data(pmic_device);

  if (NULL == fg_adc_rr_ptr)
  {
    err_flag |= PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
  }
  else
  {
    reg = ADDRESS_BTM(conv_req);

    err_flag = pm_comm_write_byte(SLAVE_ID, reg, (operation << 7), 0);
  }

  return err_flag;
}

pm_err_flag_type pm_fg_set_batt_therm_pull_up(uint32 pmic_device, pm_fg_batt_therm_pull_up_res pull_up)
{
	pm_err_flag_type err_flag 		= PM_ERR_FLAG_SUCCESS;
	pm_register_address_type reg 	= 0;
	pm_fg_data_type* fg_adc_rr_ptr	= pm_fg_get_data(pmic_device);
	pm_register_data_type mask 		= 0x03;
	
	if (NULL == fg_adc_rr_ptr)
	{
		err_flag |= PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
	}
	else if(pull_up <= PM_FG_THERM_RBIAS_INVALID || pull_up > PM_FG_THERM_RBIAS_400K)
	{
		err_flag |= PM_ERR_FLAG_INVALID_PARAMETER;
	}
	else
	{
		reg = ADDRESS(batt_therm_base_cfg1);
		err_flag = pm_comm_write_byte_mask(SLAVE_ID, reg, mask, (uint8)pull_up, 0);
	}

	return err_flag;
}

pm_err_flag_type pm_fg_set_batt_therm_freq(uint32 pmic_device, uint8 value)
{
	pm_err_flag_type err_flag         = PM_ERR_FLAG_SUCCESS;
 	pm_register_address_type reg    = 0;
 	pm_fg_data_type* fg_adc_rr_ptr  = pm_fg_get_data(pmic_device);
 	
 	if (NULL == fg_adc_rr_ptr)
 	{
 	      err_flag |= PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
 	}
 	else
 	{
		reg = ADDRESS(batt_therm_freq);
 	        err_flag = pm_comm_write_byte(SLAVE_ID, reg, value, 0);
 	}
 	  return err_flag;
}
