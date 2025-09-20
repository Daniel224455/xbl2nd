/*! \file
*  
*  \brief  pm_schg_otg.c driver implementation.
*  \details charger driver implementation.
*  &copy;
*  Copyright (c) 2017-2018 Qualcomm Technologies, Inc.  All Rights Reserved. 
*  Qualcomm Technologies Proprietary and Confidential.
*/

/*===========================================================================

EDIT HISTORY FOR MODULE


when        who          what, where, why
--------    ---          ---------------------------------------------------------- 
08/01/18    ivy	         Update Qi charging current limit max value and step size
06/29/18    dc	         Expand ICL Status API for Max ICL
05/17/18    ivy	         Added support for configuring Qi charging ICL
01/24/18    ra           Adding support for ICL status
11/09/17    ra           Changes per PM855B and remove obsolete APIs
02/08/17    ll           Added API for OTG workaround for OTG device with large cap
12/06/16    sm           Added API to return OTG BOOST SoftStart done status
06/09/16    sm           Redefined pm_schg_dcdc_otg_status_type struct and pm_schg_dcdc_get_otg_status() API
05/05/16    al           Changing OTG config to CMD register when enabling OTG by SW
04/18/14    al           Initial revision
========================================================================== */

/*===========================================================================

                    INCLUDE FILES

===========================================================================*/

#include "pm_schg_dcdc.h"
#include "pm_schg_driver.h"
#include "pm_resource_manager.h"

/*===========================================================================
                       MACRO DECLARATIONS
===========================================================================*/

#define ADDRESS(x) (schg_ptr->schg_register->dcdc_register->base_address + schg_ptr->schg_register->dcdc_register->x)
#define SLAVE_ID   (schg_ptr->comm_ptr->slave_id)

#define PM_SCHG_OTG_RT_STS_NOT_OK_MASK                  0x7
#define CFG_REF_MAX_PSNS_MAX							1200
#define CFG_REF_MAX_PSNS_STEP							40

/*===========================================================================

FUNCTION DEFINITIONS 

===========================================================================*/
pm_err_flag_type pm_schg_dcdc_otg_enable(uint32 pmic_device,boolean enable)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
    pm_register_data_type data = enable ? 0xFF : 0x00;
    pm_schg_data_type *schg_ptr  = pm_schg_get_data(pmic_device);

    if(NULL == schg_ptr) 
    {
        err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    else
    {
        // Configure OTG configure source as command instead of type-c/RID
        err_flag = pm_schg_dcdc_config_otg(pmic_device, PM_SCHG_DCDC_OTG_CFG_OTG_EN_SRC_CFG, FALSE); //0 = OTG Enable Source Command

        pm_register_address_type cmd_otg = ADDRESS(cmd_otg);
        err_flag |= pm_comm_write_byte_mask(SLAVE_ID, cmd_otg, 0x1, data, 0);
    }

    return err_flag;
}


pm_err_flag_type pm_schg_dcdc_config_otg(uint32 pmic_device, pm_schg_dcdc_otg_cfg_type otg_cfg_type, boolean set_value)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
    pm_schg_data_type *schg_ptr = pm_schg_get_data(pmic_device);

    if(NULL == schg_ptr) 
    {
        err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    else
    {
        pm_register_address_type otg_cfg = ADDRESS(otg_cfg);
        pm_register_data_type mask = 0x1 << otg_cfg_type;
        pm_register_data_type data = set_value ? 0xFF : 0x00;

        err_flag = pm_comm_write_byte_mask(SLAVE_ID, otg_cfg, mask, data, 0);
    }

    return err_flag;
}

pm_err_flag_type pm_schg_dcdc_set_otg_current_limit(uint32 pmic_device, uint32 otg_limit_ma)
{
	pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
	pm_schg_data_type *schg_ptr = pm_schg_get_data(pmic_device);

	if (NULL == schg_ptr)
	{
		err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
	}
	else
	{
		pm_register_address_type otg_current_limit_cfg = ADDRESS(otg_current_limit_cfg);
        pm_register_data_type mask = 0x07;
        pm_register_data_type data = 0;

		if (otg_limit_ma < 500)
		{
			data = 0x0;
		}
		else if (otg_limit_ma > 3000)
		{
			data = 0x5;
		}
		else
		{
			data = (otg_limit_ma / 500) - 1;
		}

		err_flag = pm_comm_write_byte_mask(SLAVE_ID, otg_current_limit_cfg, mask, data, 0);
	}

	return err_flag;
}

pm_err_flag_type pm_schg_dcdc_get_otg_status(uint32 pmic_device, pm_schg_dcdc_otg_status_type* otg_status)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
    pm_schg_data_type *schg_ptr  = pm_schg_get_data(pmic_device);

    if(NULL == schg_ptr) 
    {
        err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    else if(NULL == otg_status)
    {
        err_flag = PM_ERR_FLAG_INVALID_PARAMETER;
    }
    else
    {
        pm_register_address_type otg_status_addr = ADDRESS(otg_status);
        pm_register_data_type data;
        uint8 read_mask = 0x07;
        err_flag = pm_comm_read_byte_mask(SLAVE_ID, otg_status_addr, read_mask, &data, 0);

        *otg_status = (pm_schg_dcdc_otg_status_type)data;
    }

    return err_flag;
}

pm_err_flag_type pm_schg_dcdc_get_icl_status(uint32 pmic_device, pm_schg_dcdc_icl_status_type *icl_status)
{
	pm_err_flag_type      err_flag = PM_ERR_FLAG_SUCCESS;
	pm_register_data_type data[5] = { 0, 0, 0, 0, 0};
	uint8 aicl_done_mask = 0x01;
#define ICL_STEP_SIZE  50

	pm_schg_data_type *schg_ptr = pm_schg_get_data(pmic_device);

	if ((NULL == schg_ptr) || (NULL == icl_status))
	{
		err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
	}
	else
	{
		pm_register_address_type otg_icl_status = ADDRESS(icl_max_status);
		err_flag |= pm_comm_read_byte_array(SLAVE_ID, otg_icl_status, 5, data, 0);

		icl_status->max_icl_status = data[0] * ICL_STEP_SIZE;
		icl_status->final_icl_status = data[1] * ICL_STEP_SIZE;
		icl_status->aicl_icl_status = data[2] * ICL_STEP_SIZE;
		icl_status->thermal_icl_status = data[3] * ICL_STEP_SIZE;
		icl_status->aicl_done = data[4] & aicl_done_mask;
	}

	return err_flag;
}

#define VSYSMIN_CFG_MAX 3400
#define VSYSMIN_CFG_MIN 2700
#define VSYSMIN_CFG_STEP 100
pm_err_flag_type pm_schg_dcdc_configure_vsysmin(uint32 pmic_device, uint16 vsysmin_mv)
{
	pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
    pm_schg_data_type *schg_ptr = pm_schg_get_data(pmic_device);

    if(NULL == schg_ptr)
    {
        err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    else if(vsysmin_mv > VSYSMIN_CFG_MAX || vsysmin_mv < VSYSMIN_CFG_MIN)
    {
        err_flag = PM_ERR_FLAG_INVALID_PARAMETER;
    }
    else
    {
        pm_register_address_type address = ADDRESS(vsysmin_cfg);
        pm_register_data_type mask = 0x07; // BIT<2:0>
        pm_register_data_type data = (vsysmin_mv - VSYSMIN_CFG_MIN) / VSYSMIN_CFG_STEP;

        err_flag = pm_comm_write_byte_mask(SLAVE_ID, address, mask, data, 0);
    }

    return err_flag;
}

pm_err_flag_type pm_schg_dcdc_set_ref_max_psns(uint32 pmic_device, uint32 current_mA)
{
	pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
    pm_schg_data_type *schg_ptr = pm_schg_get_data(pmic_device);

	if(current_mA > CFG_REF_MAX_PSNS_MAX)
    {
        current_mA = CFG_REF_MAX_PSNS_MAX;
    }

	if(NULL == schg_ptr)
    {
        err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
	else
	{
		pm_register_address_type address = ADDRESS(cfg_ref_max_psns);
   		pm_register_data_type mask = 0x1F; // BIT<4:0>
    	pm_register_data_type data = (current_mA + CFG_REF_MAX_PSNS_STEP/2 ) / CFG_REF_MAX_PSNS_STEP;

    	err_flag = pm_comm_write_byte_mask(SLAVE_ID, address, mask, data, 0);
	}

    return err_flag;
}

