/*! \file
*  
*  \brief  pm_schg_bat_if.c driver implementation.
*  \details charger driver implementation.
*  &copy; Copyright 2016 - 2018 QUALCOMM Technologies, Inc All Rights Reserved
*/

/*===========================================================================

EDIT HISTORY FOR MODULE


when       who     what, where, why
--------   ---     ----------------------------------------------------------
08/01/18   ivy     Added API to set batt therm pull up to register
06/21/18   cs      added API to detect batt missing and fake battery.
02/28/18   pxm     Remove unused APIs. Refine code
09/19/17    cs     remove unsupported APIs
08/25/17   czq     Correct API pm_schg_batif_set_low_batt_detect_threshold error
03/01/16   sm      Added pm_schg_batif_cfg_batt_missing_src()
02/16/16   va      Adding required API for enabling charger
01/14/16   al      Initial revision
========================================================================== */

/*===========================================================================

          INCLUDE FILES

===========================================================================*/

#include "pm_schg_batif.h"
#include "pm_schg_driver.h"
#include "pm_resource_manager.h"

/*===========================================================================
                       MACRO DECLARATIONS
===========================================================================*/

#define ADDRESS(x) (schg_ptr->schg_register->batif_register->base_address + schg_ptr->schg_register->batif_register->x)
#define SLAVE_ID   (schg_ptr->comm_ptr->slave_id)

/*===========================================================================

FUNCTION DEFINITIONS 

===========================================================================*/





pm_err_flag_type pm_schg_batif_enable_shipmode(uint32 device_index)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
    pm_schg_data_type *schg_ptr  = pm_schg_get_data(device_index);

    if (NULL == schg_ptr)
    {
        err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    else
    {
        pm_register_address_type shipmode = ADDRESS(ship_mode);
        err_flag = pm_comm_write_byte(SLAVE_ID, shipmode, 0x01, 0);
    }

    return err_flag;
}

pm_err_flag_type pm_schg_batif_irq_enable(uint32 device_index, pm_schg_batif_irq_type irq, boolean enable)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
    pm_register_address_type irq_reg;
    pm_register_data_type data = 1 << irq;

    pm_schg_data_type *schg_ptr = pm_schg_get_data(device_index);

    if (NULL == schg_ptr)
    {
        err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    else if (irq >= PM_SCHG_BATIF_IRQ_INVALID)
    {
        err_flag = PM_ERR_FLAG_INVALID_PARAMETER;
    }
    else
    {
        if (enable)
        {
            irq_reg = ADDRESS(int_en_set);
        }
        else
        {
            irq_reg = ADDRESS(int_en_clr);
        }

        err_flag = pm_comm_write_byte_mask(SLAVE_ID, irq_reg, data, data, 0);
    }

    return err_flag;
}

pm_err_flag_type pm_schg_batif_irq_clear(uint32  device_index, pm_schg_batif_irq_type irq)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
    pm_register_data_type data = 1 << irq;
    pm_register_address_type int_latched_clr;
    pm_schg_data_type *schg_ptr = pm_schg_get_data(device_index);

    if (NULL == schg_ptr)
    {
        err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    else if (irq >= PM_SCHG_BATIF_IRQ_INVALID)
    {
        err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    else
    {
        int_latched_clr = ADDRESS(int_latched_clr);
        err_flag = pm_comm_write_byte_mask(SLAVE_ID, int_latched_clr, data, data, 0);
    }

    return err_flag;
}


pm_err_flag_type pm_schg_batif_irq_set_trigger(uint32 device_index, pm_schg_batif_irq_type irq, pm_irq_trigger_type trigger)
{
    pm_err_flag_type    err_flag    = PM_ERR_FLAG_SUCCESS;
    uint8 mask = 1 << irq;
    pm_register_data_type set_type, polarity_high, polarity_low;
    pm_register_address_type int_set_type, int_polarity_high, int_polarity_low;

    pm_schg_data_type *schg_ptr  = pm_schg_get_data(device_index);

    if (NULL == schg_ptr)
    {
        err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    else if (irq >= PM_SCHG_BATIF_IRQ_INVALID)
    {
        err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    else
    {
        int_set_type = ADDRESS(int_set_type);
        int_polarity_high = ADDRESS(int_polarity_high);
        int_polarity_low = ADDRESS(int_polarity_low);

        switch (trigger)
        {
        case PM_IRQ_TRIGGER_ACTIVE_LOW:
            set_type = 0x00;
            polarity_high = 0x00;
            polarity_low = 0xFF;
            break;
        case PM_IRQ_TRIGGER_ACTIVE_HIGH:
            set_type = 0x00;
            polarity_high = 0xFF;
            polarity_low = 0x00;
            break;
        case PM_IRQ_TRIGGER_RISING_EDGE:
            set_type = 0xFF;
            polarity_high = 0xFF;
            polarity_low = 0x00;
            break;
        case PM_IRQ_TRIGGER_FALLING_EDGE:
            set_type = 0xFF;
            polarity_high = 0x00;
            polarity_low = 0xFF;
            break;
        case PM_IRQ_TRIGGER_DUAL_EDGE:
            set_type = 0xFF;
            polarity_high = 0xFF;
            polarity_low = 0xFF;
            break;
        default:
            return PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
        }
        err_flag = pm_comm_write_byte_mask(SLAVE_ID, int_set_type, mask, set_type, 0);
        err_flag |= pm_comm_write_byte_mask(SLAVE_ID, int_polarity_high, mask, polarity_high, 0);
        err_flag |= pm_comm_write_byte_mask(SLAVE_ID, int_polarity_low, mask, polarity_low, 0);
    }

    return err_flag;
}


pm_err_flag_type pm_schg_batif_irq_status(uint32 device_index, pm_schg_batif_irq_type irq, pm_irq_status_type type, boolean *status)
{
    pm_err_flag_type    err_flag    = PM_ERR_FLAG_SUCCESS;
    pm_register_data_type data;
    uint8 mask = 1 << irq;
    pm_register_address_type int_sts;
    pm_schg_data_type *schg_ptr  = pm_schg_get_data(device_index);

    if (NULL == schg_ptr)
    {
        err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    else if (irq >= PM_SCHG_BATIF_IRQ_INVALID)
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
        *status = data ? TRUE : FALSE;
    }

    return err_flag;
}

/* This API set low battery detect threshold*/
pm_err_flag_type pm_schg_batif_set_low_batt_detect_threshold(uint32 device_index, pm_schg_batif_low_battery_threshold lbt_cfg)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
    pm_schg_data_type *schg_ptr = pm_schg_get_data(device_index);

    if (NULL == schg_ptr)
    {
        err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    else if (lbt_cfg >= PM_SCHG_BATIF_LOW_BATTERY_THRESH_INVALID)
    {
        err_flag = PM_ERR_FLAG_INVALID_PARAMETER;
    }
    else
    {
        pm_register_address_type lbt_cfg_reg = ADDRESS(low_batt_threshold_cfg);
        err_flag = pm_comm_write_byte_mask(SLAVE_ID, lbt_cfg_reg , 0x0F, (pm_register_data_type)lbt_cfg, 0);
    }

    return err_flag;
}

pm_err_flag_type pm_schg_batif_cfg_batt_missing_src(uint32 device_index, pm_schg_batif_bat_miss_src_cfg_type bat_miss_cfg, boolean enable)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
    pm_schg_data_type *schg_ptr  = pm_schg_get_data(device_index);
    uint8 mask = (1 << bat_miss_cfg);

    if (NULL == schg_ptr)
    {
        err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    else if(bat_miss_cfg >= PM_SCHG_BAT_MISS_SRC_CFG__INVALID)
    {
        err_flag = PM_ERR_FLAG_INVALID_PARAMETER;
    }
    else
    {
        pm_register_address_type batt_miss_cfg_reg = ADDRESS(bat_miss_src_cfg);
        err_flag = pm_comm_write_byte_mask(SLAVE_ID, batt_miss_cfg_reg , mask, enable ? 0xFF : 0x00, 0);
    }

    return err_flag;
}

pm_err_flag_type pm_schg_batt_missing_status(uint32 device_index, pm_schg_batif_bat_miss_status_type status_type, boolean* st)
{
  pm_err_flag_type    err_flag = PM_ERR_FLAG_SUCCESS;
  pm_register_data_type data;
  uint8 mask = 1 << status_type;
  pm_register_address_type batt_missing_sts;
  pm_schg_data_type *schg_ptr = pm_schg_get_data(device_index);

  if (NULL == schg_ptr)
  {
    err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
  }
  else if (status_type >= PM_SCHG_BAT_MISS_INVALID)
  {
    err_flag = PM_ERR_FLAG_INVALID_PARAMETER;
  }
  else
  {
      batt_missing_sts = ADDRESS(batt_miss_sts);
      err_flag = pm_comm_read_byte_mask(SLAVE_ID, batt_missing_sts, mask, &data, 0);
      *st = data ? TRUE : FALSE;
  }

  return err_flag;

}

pm_err_flag_type 
pm_schg_batif_enable_fake_bat_detect(uint32 device_index, boolean enable)
{
  pm_err_flag_type    err_flag = PM_ERR_FLAG_SUCCESS;
  uint8 mask = 1;
  uint8 data = 0;
  pm_register_address_type fake_batid_ch_en;
  pm_schg_data_type *schg_ptr = pm_schg_get_data(device_index);

  if (NULL == schg_ptr)
  {
    err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
  }
  else
  {
      fake_batid_ch_en = ADDRESS(fake_batid_ch_en);
      data = enable ? 1 : 0;
      err_flag = pm_comm_write_byte_mask(SLAVE_ID, fake_batid_ch_en,
                                         mask, data, 0);
  }
  return err_flag;
}

pm_err_flag_type
pm_schg_batif_set_adc_internal_pull_up(uint32 device_index, pm_schg_batif_adc_pull_up_type pull_up_type, pm_schg_batif_adc_pull_up_res_type resistance)
{
	pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
	uint8 mask = 0x03 << pull_up_type;
	pm_register_data_type data = resistance << pull_up_type;
	pm_register_address_type adc_pull_up_reg;
	pm_schg_data_type *schg_ptr = pm_schg_get_data(device_index);

	if (NULL == schg_ptr)
	{
		err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
	}
	else if (pull_up_type >= PM_SCHG_BATIF_PULL_UP_INVALID || resistance >= PM_SCHG_BATIF_PULL_UP_RES_INVALID)
	{
		err_flag = PM_ERR_FLAG_INVALID_PARAMETER;
	}	
	else
	{
		adc_pull_up_reg = ADDRESS(adc_internal_pull_up);
		err_flag = pm_comm_write_byte_mask(SLAVE_ID, adc_pull_up_reg, mask, data, 0);
	}
	return err_flag;
}

