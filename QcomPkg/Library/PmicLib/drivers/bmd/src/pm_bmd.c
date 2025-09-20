/*! \file
*  
*  \brief  pm_bmd.c driver implementation.
*  &copy;
*  Copyright (c) 2016-2018 Qualcomm Technologies, Inc.  All Rights Reserved. 
*  Qualcomm Technologies Proprietary and Confidential.
*/

/*===========================================================================

EDIT HISTORY FOR MODULE


when       who      what, where, why
--------   ---      ------------------------------------------------------------- 
02/07/18   xmp      Implement BMD driver
11/20/17   richaa   Initial revision
=============================================================================== */

/*=============================================================================

					INCLUDE FILES

===============================================================================*/

#include "pm_bmd.h"
#include "pm_bmd_driver.h"
#include "pm_resource_manager.h"

/*===========================================================================
                       MACRO DECLARATIONS
===========================================================================*/

#define ADDRESS(x) (bmd_ptr->bmd_reg_table->base_address + bmd_ptr->bmd_reg_table->x)
#define SLAVE_ID   (bmd_ptr->comm_ptr->slave_id)

/*===========================================================================

FUNCTION DEFINITIONS 

===========================================================================*/
pm_err_flag_type pm_bmd_src_ctl(uint32 pmic_device, pm_bmd_src_ctl_sel_type bmd_sel)
{
    pm_err_flag_type    err_flag    = PM_ERR_FLAG_SUCCESS;
    pm_bmd_data_type *bmd_ptr  = pm_bmd_get_data(pmic_device);
    pm_register_data_type data = bmd_sel;
    pm_register_data_type mask = 0x01;

    if (NULL == bmd_ptr)
    {
        err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
	else if (bmd_sel >= PM_BMD_SRC_INVALID)
	{
		err_flag = PM_ERR_FLAG_INVALID_PARAMETER;
	}
    else
    {
		pm_register_address_type bmd_src_ctrl = ADDRESS(bmd_src_ctl);
		err_flag |= pm_comm_write_byte_mask(SLAVE_ID, bmd_src_ctrl, mask, data, 0);
    }

    return err_flag;
}

pm_err_flag_type pm_bmd_get_battery_missing_rt_status(uint32 pmic_device, pm_bmd_bat_miss_sts_type bat_miss_type, boolean *missing)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
    pm_bmd_data_type *bmd_ptr  = pm_bmd_get_data(pmic_device);

    if(NULL == bmd_ptr)
    {
        err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    else if(bat_miss_type >= PM_BMD_BAT_MISS_STS_INVALID)
    {
        err_flag = PM_ERR_FLAG_INVALID_PARAMETER;
    }
    else
    {
        pm_register_data_type data = 0;
        err_flag = pm_comm_read_byte(SLAVE_ID, ADDRESS(bmd_bat_miss_sts), &data, 0);
        if(PM_ERR_FLAG_SUCCESS == err_flag)
        {
            pm_register_data_type missing_mask = PM_BIT((uint8)bat_miss_type);
            *missing = ((missing_mask & data) != 0) ? TRUE : FALSE;
        }
    }

    return err_flag;
}

pm_err_flag_type pm_bmd_get_state(uint32 pmic_device, pm_bmd_state_type *state)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
    pm_bmd_data_type *bmd_ptr  = pm_bmd_get_data(pmic_device);

    if(NULL == bmd_ptr)
    {
        err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    else if(NULL == state)
    {
        err_flag = PM_ERR_FLAG_INVALID_PARAMETER;
    }
    else
    {
        pm_register_data_type data = 0;
        pm_register_data_type mask = 0x07; // BIT[2:0]

        err_flag = pm_comm_read_byte(SLAVE_ID, ADDRESS(bmd_fsm_sts), &data, 0);
        if(PM_ERR_FLAG_SUCCESS == err_flag)
        {
            *state = (pm_bmd_state_type)(data & mask);
        }
    }

    return err_flag;
}

pm_err_flag_type pm_bmd_get_mode(uint32 pmic_device, pm_bmd_mode_type *mode)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
    pm_bmd_data_type *bmd_ptr  = pm_bmd_get_data(pmic_device);

    if(NULL == bmd_ptr)
    {
        err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    else if(NULL == mode)
    {
        err_flag = PM_ERR_FLAG_INVALID_PARAMETER;
    }
    else
    {
        pm_register_data_type data = 0;
        pm_register_data_type mask = 0x03; // BIT<5:4>
        uint8 rshift_cnt = 4; // BIT<5:4>

        err_flag = pm_comm_read_byte(SLAVE_ID, ADDRESS(bmd_fsm_sts), &data, 0);
        if(PM_ERR_FLAG_SUCCESS == err_flag)
        {
            *mode = (pm_bmd_mode_type)((data >> rshift_cnt) & mask);
        }
    }

    return err_flag;
}

pm_err_flag_type pm_bmd_set_force_mode
(
    uint32 pmic_device,
    pm_bmd_mode_type mode
)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
    pm_bmd_data_type *bmd_ptr  = pm_bmd_get_data(pmic_device);

    if(NULL == bmd_ptr)
    {
        err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    else if(mode >= PM_BMD_MODE_INVALID)
    {
        err_flag = PM_ERR_FLAG_INVALID_PARAMETER;
    }
    else
    {
        pm_register_data_type mask = 0;

        if(PM_BMD_MODE_OFF == mode)
        {
            mask = PM_BIT(0);
        }
        else if(PM_BMD_MODE_SLEEP == mode)
        {
            mask = PM_BIT(1);
        }
        else if(PM_BMD_MODE_CONTINUOUS == mode)
        {
            mask = PM_BIT(2);
        }

        err_flag = pm_comm_write_byte_mask(SLAVE_ID, ADDRESS(bmd_mode_ctl), mask, mask, 0);
    }

    return err_flag;
}

pm_err_flag_type pm_bmd_set_sleep_rate(uint32 pmic_device, pm_bmd_sample_rate_type rate)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
    pm_bmd_data_type *bmd_ptr  = pm_bmd_get_data(pmic_device);

    if(NULL == bmd_ptr)
    {
        err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    else if(rate >= PM_BMD_SAMPLE_RATE_INVALID)
    {
        err_flag = PM_ERR_FLAG_INVALID_PARAMETER;
    }
    else
    {
        pm_register_data_type mask = 0xF0;  // <7:4>
        uint8 lshif_cnt = 4; // <7:4>
        pm_register_data_type data = ((uint8)rate) << lshif_cnt;

        err_flag |= pm_comm_write_byte_mask(SLAVE_ID, ADDRESS(bmd_timing), mask, data, 0);
    }

    return err_flag;
}

pm_err_flag_type pm_bmd_set_off_rate(uint32 pmic_device, pm_bmd_sample_rate_type rate)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
    pm_bmd_data_type *bmd_ptr  = pm_bmd_get_data(pmic_device);

    if(NULL == bmd_ptr)
    {
        err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    else if(rate >= PM_BMD_SAMPLE_RATE_INVALID)
    {
        err_flag = PM_ERR_FLAG_INVALID_PARAMETER;
    }
    else
    {
        pm_register_data_type mask = 0x0F;  // <3:0>
        pm_register_data_type data = (uint8)rate;

        err_flag |= pm_comm_write_byte_mask(SLAVE_ID, ADDRESS(bmd_timing), mask, data, 0);
    }

    return err_flag;
}

pm_err_flag_type pm_bmd_set_bat_gone_negative_edge_debounce(uint32 pmic_device, pm_bmd_debounce_type debounce)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
    pm_bmd_data_type *bmd_ptr  = pm_bmd_get_data(pmic_device);

    if(NULL == bmd_ptr)
    {
        err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    else if(debounce >= PM_BMD_DEBOUNCE_INVALID)
    {
        err_flag = PM_ERR_FLAG_INVALID_PARAMETER;
    }
    else
    {
        pm_register_data_type mask = 0xF0;  // <7:4>
        uint8 lshif_cnt = 4; // <7:4>
        pm_register_data_type data = ((uint8)debounce) << lshif_cnt;

        err_flag |= pm_comm_write_byte_mask(SLAVE_ID, ADDRESS(bmd_deb_ctl), mask, data, 0);
    }

    return err_flag;
}

pm_err_flag_type pm_bmd_set_bat_gone_posative_edge_debounce(uint32 pmic_device, pm_bmd_debounce_type debounce)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
    pm_bmd_data_type *bmd_ptr  = pm_bmd_get_data(pmic_device);

    if(NULL == bmd_ptr)
    {
        err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    else if(debounce >= PM_BMD_DEBOUNCE_INVALID)
    {
        err_flag = PM_ERR_FLAG_INVALID_PARAMETER;
    }
    else
    {
        pm_register_data_type mask = 0x0F;  // <3:0>
        pm_register_data_type data = (uint8)debounce;

        err_flag |= pm_comm_write_byte_mask(SLAVE_ID, ADDRESS(bmd_deb_ctl), mask, data, 0);
    }

    return err_flag;
}

pm_err_flag_type pm_bmd_set_battery_removal_comparator_threshold(uint32 pmic_device, pm_bmd_comparator_threshold_type threshold)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
    pm_bmd_data_type *bmd_ptr  = pm_bmd_get_data(pmic_device);

    if(NULL == bmd_ptr)
    {
        err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    else if(threshold >= PM_BMD_COMPARATOR_THRESHOLD_INVALID)
    {
        err_flag = PM_ERR_FLAG_INVALID_PARAMETER;
    }
    else
    {
        pm_register_data_type mask = 0x0F;  // <3:0>
        pm_register_data_type data = (uint8)threshold;

        err_flag |= pm_comm_write_byte_mask(SLAVE_ID, ADDRESS(bmd_threshold_ctl), mask, data, 0);
    }

    return err_flag;
}

pm_err_flag_type pm_bmd_enable_bmd_in_adc_conv(uint32 pmic_device, boolean enable)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
    pm_bmd_data_type *bmd_ptr  = pm_bmd_get_data(pmic_device);

    if(NULL == bmd_ptr)
    {
        err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    else
    {
        pm_register_data_type mask = 0x01;  // <0>
        pm_register_data_type data = enable ? 1 : 0;

        err_flag |= pm_comm_write_byte_mask(SLAVE_ID, ADDRESS(bmd_bmd_adc_ctl), mask, data, 0);
    }

    return err_flag;
}

pm_err_flag_type pm_bmd_enable(uint32 pmic_device, boolean enable)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
    pm_bmd_data_type *bmd_ptr  = pm_bmd_get_data(pmic_device);

    if(NULL == bmd_ptr)
    {
        err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    else
    {
        pm_register_data_type mask = 0x80;  // <0>
        pm_register_data_type data = enable ? 0x80 : 0;

        err_flag |= pm_comm_write_byte_mask(SLAVE_ID, ADDRESS(bmd_bmd_en), mask, data, 0);
    }

    return err_flag;
}


