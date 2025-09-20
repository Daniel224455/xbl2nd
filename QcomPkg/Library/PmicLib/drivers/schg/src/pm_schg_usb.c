/*! \file
*  
*  \brief  pm_schg_usb.c driver implementation.
*  \details charger driver implementation.
*  &copy;
*  Copyright (c) 2016-2018 Qualcomm Technologies, Inc.  All Rights Reserved. 
*  Qualcomm Technologies Proprietary and Confidential.
*/

/*===========================================================================

EDIT HISTORY FOR MODULE


when       who     what, where, why
--------   ---    ------------------------------------------------------------- 
08/07/18   dc      Add API to return option_1 settings
02/28/18   pxm     Remove unused APIs. Refine code
10/24/17    ra      Move Typec specific API's in seperate file
09/19/17    cs      remove unsupported APIs
02/03/17    sm      Added API to configure try sink mode
01/12/17    sm      Added API to returns USB connector configuration
11/08/16    sm      Changes in workaround for false factory cable detection for all PMI 8998 revisions
11/04/16    sm      Changed input variable name in pm_schg_usb_get_apsd_status()
09/07/16    sm      Added workaround for false factory cable detection on PMI 8998 v1.x
06/09/16    sm      Simplified the logic in pm_schg_usb_typec_get_port_role()
06/08/16    sm      Added correction in pm_schg_usb_typec_get_port_role() to return port role 
                    None if no cable is attached. 
05/25/16    sm      Added pm_schg_usb_enable_icl_override_after_apsd()
05/17/16    aab     Updated pm_schg_usb_set_usbin_adptr_allowance() and pm_schg_usb_get_apsd_result_status()
04/04/16    sm      Added pm_schg_usb_aicl_options_cfg()
03/28/16    sm      Added VCONN and type C related API support
03/03/16    al      Adding API to read typeC status  
03/01/16    sm      Added pm_schg_usb_set_cmd_il()
02/16/16    va      Adding required API for enabling charger
01/14/16    al      Initial revision
=============================================================================== */

/*=============================================================================

					INCLUDE FILES

===============================================================================*/

#include "pm_schg_driver.h"
#include "pm_schg_usb.h"
#include "pm_resource_manager.h"

/*===========================================================================
                       MACRO DECLARATIONS
===========================================================================*/

#define ADDRESS(x) (schg_ptr->schg_register->usb_register->base_address + schg_ptr->schg_register->usb_register->x)
#define SLAVE_ID   (schg_ptr->comm_ptr->slave_id)

#define PM_SCHG_USB_USBIN_MAX_CURRENT_LIMIT_CFG_MV      5000
#define PM_SCHG_USB_USBIN_CURRENT_LIMIT_CFG_STEP_SIZE   50
#define PM_SCHG_USB_PERPH_VERSION_1                     0x00
/*===========================================================================

FUNCTION DEFINITIONS 

===========================================================================*/
pm_err_flag_type pm_schg_usb_irq_enable(uint32 device_index, pm_schg_usb_irq_type irq, boolean enable)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
    pm_register_address_type irq_reg;
    pm_register_data_type data = 1 << irq;
    pm_schg_data_type *schg_ptr = pm_schg_get_data(device_index);

    if (NULL == schg_ptr)
    {
        err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    else if (irq >= PM_SCHG_USB_IRQ_INVALID)
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

pm_err_flag_type pm_schg_usb_irq_clear(uint32  device_index, pm_schg_usb_irq_type irq)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
    pm_register_data_type data = 1 << irq;
    pm_schg_data_type *schg_ptr = pm_schg_get_data(device_index);

    if (NULL == schg_ptr)
    {
        err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    else if (irq >= PM_SCHG_USB_IRQ_INVALID)
    {
        err_flag = PM_ERR_FLAG_INVALID_PARAMETER;
    }
    else
    {
        pm_register_address_type int_latched_clr = ADDRESS(int_latched_clr);
        err_flag = pm_comm_write_byte_mask(SLAVE_ID, int_latched_clr, data, data, 0);
    }

    return err_flag;
}


pm_err_flag_type pm_schg_usb_irq_set_trigger(uint32 device_index, pm_schg_usb_irq_type irq, pm_irq_trigger_type trigger)
{
    pm_err_flag_type    err_flag    = PM_ERR_FLAG_SUCCESS;
    uint8 mask = 1 << irq;
    pm_register_data_type set_type, polarity_high, polarity_low;
    pm_schg_data_type *schg_ptr  = pm_schg_get_data(device_index);

    if (NULL == schg_ptr)
    {
        err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    else if (irq >= PM_SCHG_USB_IRQ_INVALID || trigger >= PM_IRQ_TRIGGER_INVALID)
    {
        err_flag = PM_ERR_FLAG_INVALID_PARAMETER;
    }
    else
    {
        pm_register_address_type int_set_type = ADDRESS(int_set_type);
        pm_register_address_type int_polarity_high = ADDRESS(int_polarity_high);
        pm_register_address_type int_polarity_low = ADDRESS(int_polarity_low);

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
            return PM_ERR_FLAG_INVALID_PARAMETER;
        }

        err_flag = pm_comm_write_byte_mask(SLAVE_ID, int_set_type, mask, set_type, 0);
        err_flag |= pm_comm_write_byte_mask(SLAVE_ID, int_polarity_high, mask, polarity_high, 0);
        err_flag |= pm_comm_write_byte_mask(SLAVE_ID, int_polarity_low, mask, polarity_low, 0);
    }

    return err_flag;
}

pm_err_flag_type pm_schg_usb_irq_status(uint32 device_index, pm_schg_usb_irq_type irq, pm_irq_status_type type, boolean *status)
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
    else if (irq >= PM_SCHG_USB_IRQ_INVALID || type >= PM_IRQ_STATUS_INVALID || NULL == status)
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
            return PM_ERR_FLAG_INVALID_PARAMETER;
        }

        err_flag = pm_comm_read_byte_mask(SLAVE_ID, int_sts, mask, &data, 0);
        *status = data ? TRUE : FALSE;
    }

    return err_flag;
}

pm_err_flag_type pm_schg_usb_set_usbin_adptr_allowance(uint32 pmic_device, pm_schg_usb_usbin_adapter_allowance_type adptr_allowance)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
    pm_register_data_type data = 0;
    pm_schg_data_type *schg_ptr = pm_schg_get_data(pmic_device);

    if (NULL == schg_ptr)
    {
        err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    else if ((adptr_allowance >= PM_SCHG_USB_USBIN_ADAPTER_ALLOW_INVALID) ||
         (adptr_allowance == PM_SCHG_USB_USBIN_ADAPTER_ALLOW_RESERVED) ||
         (adptr_allowance > PM_SCHG_USB_USBIN_ADAPTER_ALLOW_5V_TO_9V && adptr_allowance <PM_SCHG_USB_USBIN_ADAPTER_ALLOW_5V_TO_12V))
    {
        err_flag = PM_ERR_FLAG_INVALID_PARAMETER;
    }
    else
    {
        pm_register_address_type usbin_adapter_allow_cfg = ADDRESS(usbin_adapter_allow_cfg);
        data = (pm_register_data_type)adptr_allowance;
        err_flag = pm_comm_write_byte_mask(SLAVE_ID, usbin_adapter_allow_cfg, 0x0F, data, 0);
    }

    return err_flag;
}


pm_err_flag_type pm_schg_usb_get_usbin_adptr_allowance(uint32 device_index, pm_schg_usb_usbin_adapter_allowance_type *adptr_allowance)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
    pm_register_data_type data = 0;
    pm_schg_data_type *schg_ptr = pm_schg_get_data(device_index);

    if (NULL == schg_ptr)
    {
        err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    else if(NULL == adptr_allowance)
    {
        err_flag = PM_ERR_FLAG_INVALID_PARAMETER;
    }
    else
    {
        pm_register_address_type usbin_adapter_allow_cfg = ADDRESS(usbin_adapter_allow_cfg);
        err_flag = pm_comm_read_byte(SLAVE_ID, usbin_adapter_allow_cfg, &data, 0);
        if (PM_ERR_FLAG_SUCCESS == err_flag)
        {
            *adptr_allowance = (pm_schg_usb_usbin_adapter_allowance_type)(data & 0xF);
        }
    }

    return err_flag;
}

pm_err_flag_type pm_schg_usb_get_apsd_status(uint32 pmic_device, pm_schg_usb_apsd_status_type apsd_status_type, boolean *result)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
    pm_register_data_type data = 0;
    pm_schg_data_type *schg_ptr = pm_schg_get_data(pmic_device);

    if (NULL == schg_ptr)
    {
        err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    else if (NULL == result || apsd_status_type >= PM_SCHG_USB_APSD_STATUS_INVALID)
    {
        err_flag = PM_ERR_FLAG_INVALID_PARAMETER;
    }
    else
    {
        pm_register_address_type apsd_status = ADDRESS(apsd_status);
        err_flag = pm_comm_read_byte(SLAVE_ID, apsd_status, &data, 0);
        if (PM_ERR_FLAG_SUCCESS == err_flag)
        {
            *result = (data & (0x1 << apsd_status_type)) ? TRUE : FALSE;
        }
    }

    return err_flag;
}

pm_err_flag_type pm_schg_usb_get_apsd_result_status(uint32 pmic_device, pm_schg_usb_apsd_result_status_type *apsd_result)
{
    pm_err_flag_type    err_flag    = PM_ERR_FLAG_SUCCESS;
    pm_register_data_type data = 0;
    pm_schg_data_type *schg_ptr  = pm_schg_get_data(pmic_device);
    pm_register_address_type apsd_result_status;
    int8 usb_type = 0x06;

    if (NULL == schg_ptr)
    {
        err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    else if(NULL == apsd_result)
    {
        err_flag = PM_ERR_FLAG_INVALID_PARAMETER;
    }
    else
    {
        *apsd_result = PM_SCHG_USB_APSD_RESULT_STATUS_INVALID;
        apsd_result_status = ADDRESS(apsd_result_status);

        err_flag = pm_comm_read_byte(SLAVE_ID, apsd_result_status, &data, 0);
        if(PM_ERR_FLAG_SUCCESS == err_flag)
        {
            /*Check for charger port type starting from bit 6 to 0*/
            while (usb_type >= 0)
            {
                if (data & (1 << usb_type))
                {
                    *apsd_result = (pm_schg_usb_apsd_result_status_type)usb_type;
                    break;
                }
                usb_type--;
            }
        }
    }

    return err_flag;
}

pm_err_flag_type pm_schg_usb_configure_usbin_icl_options(uint32 pmic_device, pm_schg_usb_icl_options_type icl_options, boolean set_value)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
    pm_schg_data_type *schg_ptr = pm_schg_get_data(pmic_device);
    pm_register_data_type data = (set_value) ? 0xFF : 0x00;
    pm_register_data_type mask = 0x1 << icl_options ;

    if (NULL == schg_ptr)
    {
        err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    else if (icl_options >= PM_SCHG_USB_ICL_OPTIONS_INVALID)
    {
        err_flag = PM_ERR_FLAG_INVALID_PARAMETER;
    }
    else
    {
        pm_register_address_type usbin_icl_options = ADDRESS(usbin_icl_options);
        err_flag = pm_comm_write_byte_mask(SLAVE_ID, usbin_icl_options, mask, data, 0);
    }

    return err_flag;
}

pm_err_flag_type pm_schg_usb_set_usbin_current_limit_cfg(uint32 device_index, uint32 usbin_current_limit)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
    pm_register_data_type data = 0x0;
    pm_schg_data_type *schg_ptr = pm_schg_get_data(device_index);

    if (NULL == schg_ptr)
    {
        err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    else
    {
        if (usbin_current_limit > PM_SCHG_USB_USBIN_MAX_CURRENT_LIMIT_CFG_MV)
        {
            data = 100;
        }
        else
        {
            data = (usbin_current_limit + (PM_SCHG_USB_USBIN_CURRENT_LIMIT_CFG_STEP_SIZE / 2)) / PM_SCHG_USB_USBIN_CURRENT_LIMIT_CFG_STEP_SIZE;
        }
        pm_register_address_type usbin_current_limit_cfg = ADDRESS(usbin_current_limit_cfg);

        err_flag = pm_comm_write_byte(SLAVE_ID, usbin_current_limit_cfg, data, 0);
    }

    return err_flag;
}

pm_err_flag_type pm_schg_usb_rerun_apsd(uint32 device_index)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
    pm_schg_data_type *schg_ptr = pm_schg_get_data(device_index);

    if (NULL == schg_ptr)
    {
        err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    else
    {
        pm_register_address_type apsd_cfg_reg = ADDRESS(cmd_apsd);
        err_flag |= pm_comm_write_byte_mask(SLAVE_ID, apsd_cfg_reg, 1, 1, 0);
    }

    return err_flag;
}

pm_err_flag_type pm_schg_usb_set_usbin_option1_cfg(uint32 pmic_device, pm_schg_usb_usbin_options_1_cfg usbin_cfg, boolean enable)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
    pm_register_data_type data = enable ? 0xFF : 0x00;
    pm_schg_data_type *schg_ptr  = pm_schg_get_data(pmic_device);

    if (NULL == schg_ptr)
    {
        err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    else if (usbin_cfg >= PM_SCHG_USB_USBIN_OPTION1_CFG_INVALID)
    {
        err_flag = PM_ERR_FLAG_INVALID_PARAMETER;
    }
    else
    {
        pm_register_address_type usbin_option1_cfg = ADDRESS(usbin_options_1_cfg);
        err_flag = pm_comm_write_byte_mask(SLAVE_ID, usbin_option1_cfg, (1 << usbin_cfg), data, 0);
    }

    return err_flag;
}

pm_err_flag_type pm_schg_usb_set_usbin_suspend(uint32 device_index, boolean set_value)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
    pm_schg_data_type *schg_ptr = pm_schg_get_data(device_index);

    if (NULL == schg_ptr)
    {
        err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    else
    {
        pm_register_address_type cmd_il = ADDRESS(cmd_il);
        err_flag = pm_comm_write_byte_mask(SLAVE_ID, cmd_il, 0x01, set_value ? 0x01 : 0x00, 0);
    }

    return err_flag;
}

pm_err_flag_type pm_schg_usb_aicl_options_cfg(uint32 device_index, pm_schg_usb_aicl_cfg_type aicl_cfg_type, boolean enable)
{
    pm_err_flag_type    err_flag    = PM_ERR_FLAG_SUCCESS;
    pm_register_data_type data = enable ? 0xFF : 0x00;
    pm_register_data_type mask = 0x1 << aicl_cfg_type;
    pm_schg_data_type *schg_ptr = pm_schg_get_data(device_index);

    if (NULL == schg_ptr)
    {
        err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    else if(aicl_cfg_type >= PM_SCHG_USB_AICL_CFG_INVALID)
    {
        err_flag = PM_ERR_FLAG_INVALID_PARAMETER;
    }
    else
    {
        pm_register_address_type usbin_aicl_options_cfg = ADDRESS(usbin_aicl_options_cfg);
        err_flag = pm_comm_write_byte_mask(SLAVE_ID, usbin_aicl_options_cfg, mask, data, 0);
    }

    return err_flag;
}

pm_err_flag_type pm_schg_usb_enable_icl_override_after_apsd(uint32 device_index, boolean enable)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
    uint8 mask = 0x10;
    pm_register_data_type data = (enable << 4);
    pm_schg_data_type *schg_ptr = pm_schg_get_data(device_index);

    if (NULL == schg_ptr)
    {
        err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    else
    {
        pm_register_address_type usbin_load_cfg = ADDRESS(usbin_load_cfg);
        err_flag = pm_comm_write_byte_mask(SLAVE_ID, usbin_load_cfg, mask, data, 0);
    }

    return err_flag;
}

pm_err_flag_type pm_schg_usb_get_usbin_option1_cfg(uint32 pmic_device, pm_schg_usb_usbin_options_1_cfg usbin_cfg, boolean *enable)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
    pm_register_data_type data = 0;
    pm_schg_data_type *schg_ptr  = pm_schg_get_data(pmic_device);
    
    if (NULL == schg_ptr)
    {
        err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    else if (usbin_cfg >= PM_SCHG_USB_USBIN_OPTION1_CFG_INVALID || enable == NULL)
    {
        err_flag = PM_ERR_FLAG_INVALID_PARAMETER;
    }
    else
    {
        pm_register_address_type usbin_option1_cfg = ADDRESS(usbin_options_1_cfg);
        err_flag = pm_comm_read_byte_mask(SLAVE_ID, usbin_option1_cfg, (1 << usbin_cfg), &data, 0);
        *enable = data? TRUE : FALSE;
    }
    return err_flag;
}