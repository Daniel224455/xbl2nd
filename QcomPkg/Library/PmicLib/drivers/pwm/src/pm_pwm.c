/*! \file
*  
*  \brief  pm_pwm.c ---PWM Driver implementation.
*  \details PWM Driver implementation.
*  &copy; Copyright (c) 2012-2018 Qualcomm Technologies, Inc.  All Rights Reserved.
          Qualcomm Technologies Proprietary and Confidential.
*/

/*===========================================================================

EDIT HISTORY FOR MODULE



when       	who     what, where, why
--------   	---    	---------------------------------------------------------- 
08/20/14        al     Updating comm lib 
05/20/14        al      Architecture update 
10/16/12        al	added LUT support 
07/23/12        al      Created.

===========================================================================*/

/*===========================================================================

INCLUDE FILES 

===========================================================================*/
#include "pm_pwm.h"
#include "pm_pwm_driver.h"

/*************************************
NOTE: VIRTUAL METHOD IMPLEMENTATION
**************************************/

#define ADDRESS(pwm_ptr, pwm_rsrc, offset)  ((pwm_ptr->pwm_reg_table->base_address) + (pwm_rsrc * 0x100) + (pwm_ptr->pwm_reg_table->offset))
#define SLAVE_ID                            (pwm_ptr->comm_ptr->slave_id)

pm_err_flag_type 
pm_pwm_enable_pwm (uint32 pmic_chip, pm_pwm_resource_type pwm_rsrc, boolean enable)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
    pm_register_data_type data = 0x00;

    pm_pwm_data_type *pwm_ptr = pm_pwm_get_data(pmic_chip);
    if (pwm_ptr == NULL)
    {
        err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    else if (pwm_rsrc >= pwm_ptr->num_of_peripherals)
    {
        err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    else
    {
        pm_register_address_type enable_control = ADDRESS(pwm_ptr, pwm_rsrc, enable_control); 
        data = (enable)? 0x80 : 0x00;
        err_flag = pm_comm_write_byte_mask(SLAVE_ID, enable_control, 0x80, data, 0);
    }

    return err_flag;
}

pm_err_flag_type 
pm_pwm_set_pwm_value (uint32 pmic_chip, pm_pwm_resource_type pwm_rsrc, uint16 pwm_value)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
    
    pm_pwm_data_type *pwm_ptr = pm_pwm_get_data(pmic_chip);

    if (pwm_ptr == NULL)
    {
        err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    else if (pwm_rsrc >= pwm_ptr->num_of_peripherals)
    {
        err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    else
    {
        pm_register_address_type pwm_value_lsb = ADDRESS(pwm_ptr, pwm_rsrc, pwm_value_lsb);
        pm_register_address_type pwm_value_msb = ADDRESS(pwm_ptr, pwm_rsrc, pwm_value_msb);
        pm_register_address_type pwm_sync = ADDRESS(pwm_ptr, pwm_rsrc, pwm_sync);

        if (pwm_value > 0x1FF ) //rounding up value
        {
           pwm_value = 0x1FF;
        }
        err_flag = pm_comm_write_byte(SLAVE_ID, pwm_value_lsb, (pm_register_data_type)(pwm_value & 0xFF), 0);
        err_flag |= pm_comm_write_byte_mask(SLAVE_ID, pwm_value_msb, 0x01,(pm_register_data_type)(pwm_value>>8), 0);
        //Writing 1 to pwm_sync register will update the 6/9-bit PWM value. This bit is auto-cleared
        err_flag |= pm_comm_write_byte_mask(SLAVE_ID, pwm_sync, 0x01,(pm_register_data_type)0x01, 0);
    }

    return err_flag;
} 

pm_err_flag_type 
pm_pwm_clock_config(uint32 pmic_chip, pm_pwm_resource_type pwm_rsrc, pm_pwm_bit_size_type bit_size,  pm_pwm_clock_freq_type clk_freq, uint32 pre_div, uint32 exp )
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
    
    pm_pwm_data_type *pwm_ptr = pm_pwm_get_data(pmic_chip);

    if (pwm_ptr == NULL)
    {
        err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    else if (pwm_rsrc >= pwm_ptr->num_of_peripherals)
    {
        err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    else if(bit_size >= PM_PWM_SIZE__MAX)
    {
        err_flag = PM_ERR_FLAG_INVALID_PARAMETER;
    }
    else if(clk_freq >= PM_PWM_CLK__MAX)
    {
        err_flag = PM_ERR_FLAG_INVALID_PARAMETER;
    }
    else if(pre_div > 6)
    {
        err_flag = PM_ERR_FLAG_INVALID_PARAMETER;
    }
    else if(exp >= 8)
    {
        err_flag = PM_ERR_FLAG_INVALID_PARAMETER;
    }
    else
    {
        pm_register_address_type pwm_size_clk = ADDRESS(pwm_ptr, pwm_rsrc, pwm_size_clk);
        pm_register_address_type pwm_freq_prediv_clk = ADDRESS(pwm_ptr, pwm_rsrc, pwm_freq_prediv_clk);

        pre_div = pre_div/2; //0 = PRE_DIV__1, 1 = PRE_DIV__3, 2 = PRE_DIV__5, 3 = PRE_DIV__6
        err_flag = pm_comm_write_byte_mask(SLAVE_ID, pwm_size_clk, 0x07, (pm_register_data_type)((bit_size<<2)|clk_freq), 0);
        err_flag |= pm_comm_write_byte_mask(SLAVE_ID, pwm_freq_prediv_clk, 0x67,(pm_register_data_type)((pre_div<<5)|exp), 0);

    }
    return err_flag;
}

pm_err_flag_type 
pm_pwm_glitch_removal(uint32 pmic_chip, pm_pwm_resource_type pwm_rsrc, boolean glitch_removal )
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
    pm_register_data_type data = 0x00;

    pm_pwm_data_type *pwm_ptr = pm_pwm_get_data(pmic_chip);
    
    if (pwm_ptr == NULL)
    {
        err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    else if (pwm_rsrc >= pwm_ptr->num_of_peripherals)
    {
        err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    else
    {
        pm_register_address_type pwm_type_config = ADDRESS(pwm_ptr, pwm_rsrc, pwm_type_config);

        data = (glitch_removal) ? 0xFF : 0x00;

        err_flag = pm_comm_write_byte_mask(SLAVE_ID, pwm_type_config, 0x20, data, 0);
    }

    return err_flag;
}


pm_err_flag_type 
pm_pwm_config_dtest(uint32 pmic_chip, pm_pwm_resource_type pwm_rsrc, pm_pwm_dtest dtest, uint32 set_value)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
    pm_register_address_type test_reg = 0x0;
    /*currently only o and 1 are defined*/
    pm_register_data_type data = (set_value) ? 0x1 : 0x00;

    pm_pwm_data_type *pwm_ptr = pm_pwm_get_data(pmic_chip);

    if (pwm_ptr == NULL)
    {
        err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    else if (pwm_rsrc >= pwm_ptr->num_of_peripherals)
    {
        err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    else if (dtest >= PM_PWM_DTEST_INVALID)
    {
        err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    else
    {
        switch (dtest)
        {
          case PM_PWM_DTEST1:
              test_reg = ADDRESS(pwm_ptr, pwm_rsrc, test1);
              break;
          case PM_PWM_DTEST2:
              test_reg = ADDRESS(pwm_ptr, pwm_rsrc, test2);
              break;
          default:
              return PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
        }

        err_flag = pm_comm_write_byte_mask(SLAVE_ID, test_reg, 0x3, data, 0);
    }

    return err_flag;
}


pm_err_flag_type 
pm_pwm_get_status (uint32 pmic_chip, pm_pwm_resource_type pwm_rsrc, pm_pwm_status_type *pwm_status )
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
    pm_register_data_type data;

    pm_pwm_data_type *pwm_ptr = pm_pwm_get_data(pmic_chip);
    
    if (pwm_ptr == NULL)
    {
        return PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    else if (pwm_rsrc >= pwm_ptr->num_of_peripherals)
    {
        err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }

    pm_register_address_type pwm_size_clk = ADDRESS(pwm_ptr, pwm_rsrc, pwm_size_clk);
    pm_register_address_type pwm_freq_prediv_clk = ADDRESS(pwm_ptr, pwm_rsrc, pwm_freq_prediv_clk);
    pm_register_address_type pwm_type_config = ADDRESS(pwm_ptr, pwm_rsrc, pwm_type_config);
    pm_register_address_type pwm_value_lsb = ADDRESS(pwm_ptr, pwm_rsrc, pwm_value_lsb);
    pm_register_address_type pwm_value_msb = ADDRESS(pwm_ptr, pwm_rsrc, pwm_value_msb);
    pm_register_address_type enable_control = ADDRESS(pwm_ptr, pwm_rsrc, enable_control);

    err_flag =  pm_comm_read_byte_mask(SLAVE_ID, enable_control, 0x80, &data, 0);
    pwm_status->pwm_enable = (data&0x80)?TRUE:FALSE;
    err_flag =  pm_comm_read_byte_mask(SLAVE_ID, pwm_value_lsb, 0xFF, &data, 0);
    pwm_status->pwm_value = data; //writing LSB for duty cycle
    err_flag |= pm_comm_read_byte_mask(SLAVE_ID, pwm_value_msb, 0x01, &data, 0);
    pwm_status->pwm_value |= (data<<8); //MSB for duty cycle
    err_flag =  pm_comm_read_byte_mask(SLAVE_ID, pwm_size_clk, 0x07, &data, 0);
    pwm_status->clk_freq=(pm_pwm_clock_freq_type)(data & 0x03);
    pwm_status->bit_size=(data & 0x04)? PM_PWM_SIZE__9BIT:PM_PWM_SIZE__6BIT;
    err_flag |= pm_comm_read_byte_mask(SLAVE_ID, pwm_freq_prediv_clk, 0x67,&data, 0);
    pwm_status->exp = (data&0x07);
    if(((data&0x60)>>5)==3)
    {
        pwm_status->pre_div = ((data&0x60)>>5) *2;
    }
    else
    {
        pwm_status->pre_div = (((data&0x60)>>5) *2) + 1;
    }

    err_flag =  pm_comm_read_byte_mask(SLAVE_ID, pwm_type_config, 0x20, &data, 0);
    pwm_status->glitch_removal=(data)?TRUE:FALSE;

    return err_flag;
}

