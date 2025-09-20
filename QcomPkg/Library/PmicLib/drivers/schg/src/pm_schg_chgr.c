/*! \file
*
*  \brief  pm_schg_chgr.c driver implementation.
*  \details charger driver implementation.
*  &copy;
*  Copyright (c) 2016-2018 Qualcomm Technologies, Inc.  All Rights Reserved.
*  Qualcomm Technologies Proprietary and Confidential.
*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE


when        who    what, where, why
--------    ---    ----------------------------------------------------------
06/21/18    cs     added support for getting active charging port
03/15/18    ra     Change to set_jeita_threshold
02/28/18    pxm    Remove unused APIs and registers, refine driver code
01/15/18    ra     Use ADC code to program JEITA and AFP
11/09/17    ra     JEITA changes for PM855B
10/24/17    ra     Removing obsolete APIs
09/19/17    cs     removed unsupported APIs
04/21/17    czq     Change allowed max jeita_cccomp to 4500mA
12/19/16    sm     Added API to get SCHG Charger PmicIndex
05/17/16    aab    Updated pm_schg_chgr_set_chgr_cmd_type()
03/28/16    va     Updated Jeit CcComp Api
03/01/16    sm     Added pm_schg_chgr_jeita_en_cfg()
02/16/16    va     Adding required API for enanlbing charger
04/18/14    al     Initial revision
========================================================================== */

/*===========================================================================

                        INCLUDE FILES

===========================================================================*/
#include "pm_schg_chgr.h"
#include "pm_schg_driver.h"

/*===========================================================================
                        MACRO DECLARATIONS
===========================================================================*/

#define ADDRESS(x) (schg_ptr->schg_register->chgr_register->base_address + schg_ptr->schg_register->chgr_register->x)
#define SLAVE_ID   (schg_ptr->comm_ptr->slave_id)
#define PM_SCHG_FLOAT_VOLTAGE_STEP_CONSTANT           10     
#define PM_SCHG_JEITA_FLOAT_VOLTAGE_STEP_CONSTANT     10
#define PM_SCHG_JEITA_CHARGE_CURRENT_STEP_CONSTANT    50

/*===========================================================================

                        FUNCTION DEFINITIONS

===========================================================================*/


pm_err_flag_type pm_schg_chgr_set_charge_current(uint32 device_index, pm_schg_chgr_current_type charge_current_type, uint32 current)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
    pm_register_data_type data = 0;
    pm_register_data_type mask = 0;
    pm_register_address_type reg = 0;
    uint32 step_size = 0;
    uint32 max_value = 0;
    uint32 min_value = 0;

    pm_schg_data_type *schg_ptr = pm_schg_get_data(device_index);

    if (NULL == schg_ptr)
    {
        err_flag =  PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    else if (charge_current_type >= PM_SCHG_CHGR_CURRENT_TYPE_INVALID)
    {
        err_flag = PM_ERR_FLAG_INVALID_PARAMETER;
    }
    else
    {
        switch (charge_current_type)
        {
        case PM_SCHG_CHGR_PRE_CHARGE_CURRENT:
            reg = ADDRESS(pre_charge_current_cfg);
            mask = 0x07;  // BIT<2:0>
            step_size = 50;
            max_value = 450;
            min_value = 100;
            break;

        case PM_SCHG_CHGR_FAST_CHARGE_CURRENT:
            reg = ADDRESS(fast_charge_current_cfg);
            mask = 0xFF;  // BIT<7:0>
            step_size = 50;
            max_value = 8000;  // max 8A for the case SMB is connected. 6A if PM855B alone 
            min_value = 0;
            break;

        case PM_SCHG_CHGR_TERMINATION_CURRENT:
            reg = ADDRESS(termination_charge_current_cfg);
            mask = 0x07;   // BIT<2:0>
            step_size = 50;
            max_value = 450;
            min_value = 100;
            break;

        case PM_SCHG_TCCC_CHGR_TERMINATION_CURRENT:
            reg = ADDRESS(tccc_charge_current_termination_cfg);
            mask = 0x0F;   // BIT<3:0>
            step_size = 50;
            max_value = 750;
            min_value = 0;
            break;

        default:
            return PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
            break;
        }

        if (current < min_value)
        {
            current = min_value;
        }
        else if (current > max_value)
        {
            current = max_value;
        }

        /*rounding and getting corresponding register value*/
        data = ((current - min_value) + (step_size / 2)) / step_size;
        err_flag = pm_comm_write_byte_mask(SLAVE_ID, reg, mask, data, 0);
    }

    return err_flag;
}

pm_err_flag_type pm_schg_chgr_enable_charging(uint32 device_index, boolean enable)
{
    pm_err_flag_type    err_flag    = PM_ERR_FLAG_SUCCESS;
    pm_schg_data_type *schg_ptr  = pm_schg_get_data(device_index);

    if (NULL == schg_ptr)
    {
        err_flag =  PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    else
    {
        pm_register_address_type reg = ADDRESS(charging_enable_cmd);
        pm_register_data_type data = enable ? 1 : 0;
        pm_register_data_type mask = 0x01;
        err_flag = pm_comm_write_byte_mask(SLAVE_ID, reg, mask, data, 0);
    }

    return err_flag;
}

pm_err_flag_type pm_schg_chgr_enable_src(uint32 device_index, boolean enable)
{
    pm_err_flag_type    err_flag    = PM_ERR_FLAG_SUCCESS;
    pm_schg_data_type *schg_ptr  = pm_schg_get_data(device_index);

    if (NULL == schg_ptr)
    {
        err_flag =  PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    else
    {
        pm_register_address_type reg = ADDRESS(charger_cfg2);
        pm_register_data_type data = enable ? 0xFF : 0x00;
        pm_register_data_type mask = 0x80;
        err_flag = pm_comm_write_byte_mask(SLAVE_ID, reg, mask, data, 0);
    }

    return err_flag;
}


pm_err_flag_type pm_schg_chgr_get_chgr_status(uint32 device_index, pm_schg_chgr_status_type *charger_status)
{
    pm_err_flag_type    err_flag    = PM_ERR_FLAG_SUCCESS;

    pm_schg_data_type *schg_ptr  = pm_schg_get_data(device_index);
    /*between status 2 and status 3 there is gap and hence breaking*/
    pm_register_data_type data[6]={0,0,0,0,0,0};

    if (NULL == schg_ptr)
    {
        err_flag =  PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    else if(NULL == charger_status)
    {
        err_flag = PM_ERR_FLAG_INVALID_PARAMETER;
    }
    else
    {
        pm_register_address_type status1 = ADDRESS(battery_charger_status_1);
        /*read 2 status register first and then rest 6 in other attempt*/
        err_flag = pm_comm_read_byte_array(SLAVE_ID, status1, 2, data, 0);
        /*status1 related*/
        charger_status->status_1.battery_charger_status = (pm_schg_battery_charger_status)(data[0] & 0x7);
        charger_status->status_1.step_charging_status = (data[0] & 0x38)>> 3;
        charger_status->status_1.zero_charging_current   = (data[0] & 0x40) ? TRUE : FALSE;
        charger_status->status_1.icl_incr_req_for_prechg = (data[0] & 0x80) ? TRUE : FALSE;
        /*status2 related*/
        charger_status->status_2.bat_therm_missing         = (data[1] & 0x01) ? TRUE : FALSE;
        charger_status->status_2.bat_ov                    = (data[1] & 0x02) ? TRUE : FALSE;
        charger_status->status_2.bat_sft_expire            = (data[1] & 0x04) ? TRUE : FALSE;
        charger_status->status_2.bat_gt_full_on            = (data[1] & 0x08) ? TRUE : FALSE;
        charger_status->status_2.vbatt_gtet_float_voltage  = (data[1] & 0x10) ? TRUE : FALSE;
        charger_status->status_2.vbatt_gtet_inhibit        = (data[1] & 0x20) ? TRUE : FALSE;
        charger_status->status_2.vbatt_ltet_recharge       = (data[1] & 0x40) ? TRUE : FALSE;
        charger_status->status_2.drop_in_vbat_reference    = (data[1] & 0x80) ? TRUE : FALSE;

        /*read reset of 6 status registers*/
        DALSYS_memset(data, 0, sizeof(data));

        pm_register_address_type status3 = ADDRESS(battery_charger_status_3);
        err_flag |= pm_comm_read_byte_array(SLAVE_ID, status3, 6, data, 0);
        /*status3 related*/
        charger_status->status_3 = data[0];
        /*status4 related*/
        charger_status->status_4 = data[1];
        /*status 5 related*/
        charger_status->status_5.enable_fullon_mode        = (data[2] & 0x01) ? TRUE : FALSE;
        charger_status->status_5.enable_pre_charging       = (data[2] & 0x02) ? TRUE : FALSE;
        charger_status->status_5.enable_trickle            = (data[2] & 0x04) ? TRUE : FALSE;
        charger_status->status_5.enable_chg_sensors        = (data[2] & 0x08) ? TRUE : FALSE;
        charger_status->status_5.charging_enable              = (data[2] & 0x10) ? TRUE : FALSE;
        charger_status->status_5.force_zero_charge_current    = (data[2] & 0x20) ? TRUE : FALSE;
        charger_status->status_5.disable_charging             = (data[2] & 0x40) ? TRUE : FALSE;
        charger_status->status_5.valid_input_power_source     = (data[2] & 0x80) ? TRUE : FALSE;

        /*status 6 related*/
        charger_status->status_6.jeita_adc_thot_afp        = (data[3] & 0x01) ? TRUE : FALSE;
        charger_status->status_6.jeita_adc_thot            = (data[3] & 0x02) ? TRUE : FALSE;
        charger_status->status_6.jeita_adc_hot             = (data[3] & 0x04) ? TRUE : FALSE;
        charger_status->status_6.jeita_adc_cold            = (data[3] & 0x08) ? TRUE : FALSE;
        charger_status->status_6.jeita_adc_tcold           = (data[3] & 0x10) ? TRUE : FALSE;
        charger_status->status_6.jeita_adc_tcold_afp       = (data[3] & 0x20) ? TRUE : FALSE;

        /*status 7 related*/
        charger_status->status_7.bat_temp_status_too_cold_afp  = (data[4] & 0x01) ? TRUE : FALSE;
        charger_status->status_7.bat_temp_status_too_hot_afp   = (data[4] & 0x02) ? TRUE : FALSE;
        charger_status->status_7.bat_temp_status_too_cold      = (data[4] & 0x04) ? TRUE : FALSE;
        charger_status->status_7.bat_temp_status_too_hot       = (data[4] & 0x08) ? TRUE : FALSE;
        charger_status->status_7.bat_temp_status_cold_soft     = (data[4] & 0x10) ? TRUE : FALSE;
        charger_status->status_7.bat_temp_status_hot_soft      = (data[4] & 0x20) ? TRUE : FALSE;

        /*status 8 related*/
        charger_status->status_8 = data[5];
    }

    return err_flag;
}

/*This API configures the floating voltage. Valid range is 3600mV to 4500 mv*/
pm_err_flag_type pm_schg_chgr_set_float_volt(uint32 device_index, uint32 float_volt_mv)
{
    pm_err_flag_type  err_flag  = PM_ERR_FLAG_SUCCESS;
    pm_register_data_type data = 0x00;
    pm_schg_data_type *schg_ptr  = pm_schg_get_data(device_index);

    if (NULL == schg_ptr)
    {
        err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    else
    {
        pm_register_address_type fv_cfg = ADDRESS(float_voltage_cfg);
        if (float_volt_mv < 3600)
        {
            data = 0x0;
        }
        else if (float_volt_mv >= 4920)
        {
            data = 0xBF;
        }
        else
        {
            /* Float voltage setting = 3.6V + (DATA x 10mV) */
            data = (float_volt_mv - 3600) / PM_SCHG_FLOAT_VOLTAGE_STEP_CONSTANT;
        }

        err_flag = pm_comm_write_byte(SLAVE_ID, fv_cfg, data, 0);
    }

    return err_flag;
}


/* This api sets floating voltage conpmensation code */
pm_err_flag_type pm_schg_chgr_set_jeita_fvcomp_cfg(uint32 device_index, uint32 fv_comp_hot, uint32 fv_comp_cold)
{
    pm_err_flag_type  err_flag  = PM_ERR_FLAG_SUCCESS;
    pm_register_data_type data = 0;
    pm_schg_data_type *schg_ptr  = pm_schg_get_data(device_index);

    if (NULL == schg_ptr)
    {
        err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    else
    {
        pm_register_address_type fv_cmp_cold_cfg = ADDRESS(jeita_fvcomp_cold_cfg);
        pm_register_address_type fv_cmp_hot_cfg = ADDRESS(jeita_fvcomp_hot_cfg);

        /* JEITA Float Voltage Compensation in cold soft limit */
        data = fv_comp_cold / PM_SCHG_JEITA_FLOAT_VOLTAGE_STEP_CONSTANT;
        err_flag |= pm_comm_write_byte(SLAVE_ID, fv_cmp_cold_cfg, data, 0);

        /* JEITA Float Voltage Compensation in hot soft limit  */
        data = fv_comp_hot / PM_SCHG_JEITA_FLOAT_VOLTAGE_STEP_CONSTANT;
        err_flag |= pm_comm_write_byte(SLAVE_ID, fv_cmp_hot_cfg, data, 0);
    }

    return err_flag;
}

/*This API configures the fast-charge current compensation value*/
pm_err_flag_type pm_schg_chgr_set_jeita_ccomp(uint32 device_index, uint32 ccomp_hot, uint32 ccomp_cold)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
    pm_register_data_type data = 0;
    pm_schg_data_type *schg_ptr = pm_schg_get_data(device_index);

    if (NULL == schg_ptr)
    {
        err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    else
    {
        pm_register_address_type fcc_cmp_cold_cfg = ADDRESS(jeita_cccomp_cold_cfg);
        pm_register_address_type fcc_cmp_hot_cfg = ADDRESS(jeita_cccomp_hot_cfg);

        /* JEITA Charge Current Compensation in cold soft limit */
        data = ccomp_cold / PM_SCHG_JEITA_CHARGE_CURRENT_STEP_CONSTANT;
        err_flag |= pm_comm_write_byte(SLAVE_ID, fcc_cmp_cold_cfg, data, 0);

        /* JEITA Charge Current Compensation in hot soft limit */
        data = ccomp_hot / PM_SCHG_JEITA_CHARGE_CURRENT_STEP_CONSTANT;
        err_flag |= pm_comm_write_byte(SLAVE_ID, fcc_cmp_hot_cfg, data, 0);
    }

    return err_flag;
}
 
pm_err_flag_type pm_schg_chgr_set_jeita_threshold_value(uint32 device_index, pm_schg_chgr_jeita_threshold_type threshold_type, uint32 jeita_threshold)
{
	pm_err_flag_type      err_flag = PM_ERR_FLAG_SUCCESS;
	pm_schg_data_type *schg_ptr = pm_schg_get_data(device_index);
	uint32 jeita_threshold_lsb = (jeita_threshold & 0xFF);
	uint32 jeita_threshold_msb = jeita_threshold >> 8;

	if (NULL == schg_ptr)
	{
		err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
	}
	else
	{
		switch (threshold_type)
		{
		case PM_SCHG_CHGR_JEITA_THRESHOLD_HOT:
		{
			pm_register_address_type hot_threshold_msb = ADDRESS(jeita_hot_threshold_msb);
			pm_register_address_type hot_threshold_lsb = ADDRESS(jeita_hot_threshold_lsb);
			err_flag = pm_comm_write_byte(SLAVE_ID, hot_threshold_msb, jeita_threshold_msb, 0);
			err_flag |= pm_comm_write_byte(SLAVE_ID, hot_threshold_lsb, jeita_threshold_lsb, 0);
			break;
		}
		case PM_SCHG_CHGR_JEITA_THRESHOLD_COLD:
		{
			pm_register_address_type cold_threshold_msb = ADDRESS(jeita_cold_threshold_msb);
			pm_register_address_type cold_threshold_lsb = ADDRESS(jeita_cold_threshold_lsb);
			err_flag = pm_comm_write_byte(SLAVE_ID, cold_threshold_msb, jeita_threshold_msb, 0);
			err_flag |= pm_comm_write_byte(SLAVE_ID, cold_threshold_lsb, jeita_threshold_lsb, 0);
			break;
		}
		case PM_SCHG_CHGR_JEITA_THRESHOLD_TOO_HOT:
		{
			pm_register_address_type thot_threshold_msb = ADDRESS(jeita_thot_threshold_msb);
			pm_register_address_type thot_threshold_lsb = ADDRESS(jeita_thot_threshold_lsb);
			err_flag = pm_comm_write_byte(SLAVE_ID, thot_threshold_msb, jeita_threshold_msb, 0);
			err_flag |= pm_comm_write_byte(SLAVE_ID, thot_threshold_lsb, jeita_threshold_lsb, 0);
			break;
		}
		case PM_SCHG_CHGR_JEITA_THRESHOLD_TOO_COLD:
		{
			pm_register_address_type tcold_threshold_msb = ADDRESS(jeita_tcold_threshold_msb);
			pm_register_address_type tcold_threshold_lsb = ADDRESS(jeita_tcold_threshold_lsb);
			err_flag = pm_comm_write_byte(SLAVE_ID, tcold_threshold_msb, jeita_threshold_msb, 0);
			err_flag |= pm_comm_write_byte(SLAVE_ID, tcold_threshold_lsb, jeita_threshold_lsb, 0);
			break;
		}
		case PM_SCHG_CHGR_JEITA_THRESHOLD_TOO_HOT_AFP:
		{
			pm_register_address_type thot_afp_threshold_msb = ADDRESS(jeita_thot_afp_threshold_msb);
			pm_register_address_type thot_afp_threshold_lsb = ADDRESS(jeita_thot_afp_threshold_lsb);
			err_flag = pm_comm_write_byte(SLAVE_ID, thot_afp_threshold_msb, jeita_threshold_msb, 0);
			err_flag |= pm_comm_write_byte(SLAVE_ID, thot_afp_threshold_lsb, jeita_threshold_lsb, 0);
			break;
		}
		case PM_SCHG_CHGR_JEITA_THRESHOLD_TOO_COLD_AFP:
		{
			pm_register_address_type tcold_afp_threshold_msb = ADDRESS(jeita_tcold_afp_threshold_msb);
			pm_register_address_type tcold_afp_threshold_lsb = ADDRESS(jeita_tcold_afp_threshold_lsb);
			err_flag = pm_comm_write_byte(SLAVE_ID, tcold_afp_threshold_msb, jeita_threshold_msb, 0);
			err_flag |= pm_comm_write_byte(SLAVE_ID, tcold_afp_threshold_lsb, jeita_threshold_lsb, 0);
			break;
		}
		default:
			break;
		}
	}
	return err_flag;
}

pm_err_flag_type pm_schg_chgr_jeita_en_cfg(uint32 device_index, pm_schg_chgr_jeita_cfg_type jeita_cfg_type, boolean enable)
{
    pm_err_flag_type  err_flag  = PM_ERR_FLAG_SUCCESS;
    pm_schg_data_type *schg_ptr  = pm_schg_get_data(device_index);

    if (NULL == schg_ptr)
    {
        err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    else if (jeita_cfg_type >= PM_SCHG_CHGR_JEITA_INVALID)
    {
        err_flag = PM_ERR_FLAG_INVALID_PARAMETER;
    }
    else
    {
        pm_register_address_type jeita_en_cfg = ADDRESS(jeita_en_cfg);
        err_flag = pm_comm_write_byte_mask(SLAVE_ID, jeita_en_cfg, (uint8)(1 << jeita_cfg_type), (enable ? 0xFF : 0x00), 0);
    }

    return err_flag;
}

pm_err_flag_type pm_schg_chgr_get_schg_pmic_info(uint8 *pmic_index, uint8 *slave_id, uint8* charger_count, uint8 max_charger)
{
  uint8 i = 0;

  if (NULL == pmic_index || NULL == slave_id || NULL == charger_count)
  {
    return PM_ERR_FLAG_INVALID_PARAMETER;
  }

  *charger_count = 0;
  pm_schg_get_pmic_info(pmic_index, slave_id, charger_count, max_charger);

  for (i = 0; i < *charger_count; i++)
  {
    if (pmic_index[i] >= PM_MAX_NUM_PMICS || slave_id[i] >= PM_MAX_NUM_SLAVE_IDS)
    {
        return PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
  }

  return PM_ERR_FLAG_SUCCESS;
}

