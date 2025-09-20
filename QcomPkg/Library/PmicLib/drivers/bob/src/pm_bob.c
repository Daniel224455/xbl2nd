/*! \file pm_bob.c
*
*  \brief Implementation file for BOOST BYPASS public APIs.
*  \n
*  &copy; Copyright 2014-2018 QUALCOMM Technologies Incorporated, All Rights Reserved
*/

/* =======================================================================
                             Edit History
  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.


when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
12/08/17   aab     Updated for pm_sw_mode_type change
10/20/17   czq     Add pm_bob_ocp_latched_status_clear()
06/02/17   al      Add bob volt get status 
06/09/14   kt      Created
===========================================================================*/
/*===========================================================================

                     INCLUDE FILES

===========================================================================*/
#include "pm_bob.h"
#include "pm_bob_driver.h"

/*===========================================================================

                     API IMPLEMENTATION

===========================================================================*/

#define PM_MAX_BOB_PIN_UV_VOLT    (0x1FFF * 1000)

pm_err_flag_type pm_bob_sw_mode(uint8 pmic_chip, uint8 peripheral_index, pm_bob_mode_type mode)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
    pm_bob_data_type *bob_ptr = pm_bob_get_data(pmic_chip);
    //uint8 periph_type = 0;

    if (bob_ptr == NULL)
    {
        err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    else if (peripheral_index >= bob_ptr->pm_pwr_data.num_of_peripherals)
    {
        err_flag = PM_ERR_FLAG_INVALID_PERIPHERAL;
    }
    else if (mode >= PM_BOB_MODE_INVALID)
    {
        err_flag = PM_ERR_FLAG_INVALID_PARAMETER;
    }
    else
    {
        pm_register_address_type          reg = 0;
        pm_register_data_type             data = (pm_register_data_type)mode;
        pm_pwr_data_type                  *pwr_data = &(bob_ptr->pm_pwr_data);

        //periph_type = pwr_data->pwr_specific_info[peripheral_index].periph_type;

        reg = (pm_register_address_type)(pwr_data->pwr_specific_info[peripheral_index].periph_base_address + pwr_data->pwr_reg_table->MODE_CTL);
        err_flag = pm_comm_write_byte(bob_ptr->comm_ptr->slave_id, reg, data, 0);
    }

    if (PM_ERR_FLAG_SUCCESS == err_flag)
    {
        //PM_LOG_MSG_DEBUG(PMIC_DRV_PWR_MODE, pmic_chip, periph_type, peripheral_index, mode);
    }
    else
    {
        //PM_LOG_MSG_ERROR(PMIC_DRV_PWR_MODE_ERROR, pmic_chip, periph_type, peripheral_index, err_flag);
    }

    return err_flag;
}

pm_err_flag_type pm_bob_sw_mode_status(uint8 pmic_chip, uint8 peripheral_index, pm_bob_mode_type* mode)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
    pm_bob_data_type *bob_ptr = pm_bob_get_data(pmic_chip);
    //uint8 periph_type = 0;

    if (bob_ptr == NULL)
    {
        err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    else if (peripheral_index >= bob_ptr->pm_pwr_data.num_of_peripherals)
    {
        err_flag = PM_ERR_FLAG_INVALID_PERIPHERAL;
    }
    else if (mode == NULL)
    {
        err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    else
    {
        pm_register_address_type          reg = 0;
        pm_register_data_type             data = (pm_register_data_type)mode;
        pm_pwr_data_type                  *pwr_data = &(bob_ptr->pm_pwr_data);

        reg = (pm_register_address_type)(pwr_data->pwr_specific_info[peripheral_index].periph_base_address + pwr_data->pwr_reg_table->MODE_CTL);
        err_flag = pm_comm_read_byte(bob_ptr->comm_ptr->slave_id, reg, &data, 0);
        *mode = (pm_bob_mode_type)data;
    }

    return err_flag;
}


pm_err_flag_type pm_bob_pin_volt_level(uint8 pmic_chip, uint8 peripheral_index, uint8 pin_number, pm_volt_level_type volt_level)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
    pm_register_data_type vout_set[2] = { 0 };
    pm_bob_data_type *bob_ptr = pm_bob_get_data(pmic_chip);
    uint32 vmin = 0;  // microvolt
    uint32 vStep = 0; // microvolt
    uint32 calculated_vset = 0;

    if (bob_ptr == NULL)
    {
        err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    else if (peripheral_index >= bob_ptr->pm_pwr_data.num_of_peripherals)
    {
        err_flag = PM_ERR_FLAG_INVALID_PERIPHERAL;
    }
    else if (pin_number >= PM_BOB_EXT_PIN_MAX || pin_number == PM_BOB_EXT_PIN_RESERVED)
    {
        err_flag = PM_ERR_FLAG_INVALID_PERIPHERAL;
    }
    else
    {
        pm_register_address_type          reg_lb = 0;

        pm_pwr_data_type                  *pwr_data = &(bob_ptr->pm_pwr_data);

        if ((volt_level <= pwr_data->pwr_specific_info[peripheral_index].pwr_vset.RangeMax) && 
            (volt_level >= pwr_data->pwr_specific_info[peripheral_index].pwr_vset.RangeMin) )
        {
            vmin = pwr_data->pwr_specific_info[peripheral_index].pwr_vset.RangeMin;
            vStep = pwr_data->pwr_specific_info[peripheral_index].pwr_vset.VStep;
        }

        if (vStep > 0)
        {
            calculated_vset = (volt_level - vmin)/vStep;

            vout_set[0] = ((calculated_vset) & 0xFF);
            vout_set[1] = ((calculated_vset >> 8) & 0xFF);

            reg_lb = (pm_register_address_type)(pwr_data->pwr_specific_info[peripheral_index].periph_base_address + pwr_data->pwr_reg_table->VOUT_EXT_CTRL1_LB); //0x04A
            reg_lb = reg_lb + (2 * (pin_number - 1));

            err_flag = pm_comm_write_byte_array(bob_ptr->comm_ptr->slave_id, reg_lb, 2, vout_set, 0);
        }
    }

    if (PM_ERR_FLAG_SUCCESS == err_flag)
    {
        //PM_LOG_MSG_DEBUG(PMIC_DRV_PWR_PIN_VOLT_LVL, pmic_chip, periph_type, peripheral_index, volt_level);
    }
    else
    {
        //PM_LOG_MSG_ERROR(PMIC_DRV_PWR_PIN_VOLT_LVL_ERROR, pmic_chip, periph_type, peripheral_index, err_flag);
    }

    return err_flag;
}


pm_err_flag_type pm_bob_pin_volt_level_status(uint8 pmic_chip, uint8 peripheral_index, uint8 pin_number, pm_volt_level_type* volt_level)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
    pm_register_data_type vout_set[2] = { 0 };
    pm_bob_data_type *bob_ptr = pm_bob_get_data(pmic_chip);
    //uint8 periph_type = 0;
    uint32 vmin = 0;  // microvolt
    uint32 vStep = 0; // microvolt

    if (bob_ptr == NULL)
    {
        err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    else if (volt_level == NULL)
    {
        err_flag = PM_ERR_FLAG_INVALID_PARAMETER;
    }
    else if (peripheral_index >= bob_ptr->pm_pwr_data.num_of_peripherals)
    {
        err_flag = PM_ERR_FLAG_INVALID_PERIPHERAL;
    }
    else if (pin_number >= PM_BOB_EXT_PIN_MAX || pin_number == PM_BOB_EXT_PIN_RESERVED)
    {
        err_flag = PM_ERR_FLAG_INVALID_PERIPHERAL;
    }
    else
    {
        pm_register_address_type          reg_lb = 0;

        pm_pwr_data_type                  *pwr_data = &(bob_ptr->pm_pwr_data);

        reg_lb = (pm_register_address_type)(pwr_data->pwr_specific_info[peripheral_index].periph_base_address + pwr_data->pwr_reg_table->VOUT_EXT_CTRL1_LB); //0x04A

        reg_lb = reg_lb + (2 * (pin_number - 1));

        err_flag = pm_comm_read_byte_array(bob_ptr->comm_ptr->slave_id, reg_lb, 2, vout_set, 0);

        *volt_level = (vout_set[1] << 8)|vout_set[0];

        vmin = pwr_data->pwr_specific_info[peripheral_index].pwr_vset.RangeMin; // microvolt
        vStep = pwr_data->pwr_specific_info[peripheral_index].pwr_vset.VStep; //microvolt

        *volt_level = vmin + (*volt_level) * vStep;
    }

    return err_flag;
}


pm_err_flag_type pm_bob_volt_level_status(uint8 pmic_chip, uint8 peripheral_index, pm_volt_level_type *volt_level)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
    pm_bob_data_type *bob_ptr = pm_bob_get_data(pmic_chip);

    if (bob_ptr == NULL)
    {
        err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    else
    {
        err_flag = pm_pwr_volt_level_status_alg(&(bob_ptr->pm_pwr_data), bob_ptr->comm_ptr, peripheral_index, volt_level);
    }

    return err_flag;
} 


pm_err_flag_type 
pm_bob_ocp_latched_status_clear(uint8 pmic_chip, uint8 peripheral_index,  boolean *ocp_occured)
{
   pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
   pm_bob_data_type *bob_ptr = pm_bob_get_data(pmic_chip);

   if (bob_ptr == NULL)
   {
       err_flag |= PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
   }
   else if(peripheral_index >= bob_ptr->pm_pwr_data.num_of_peripherals)
   {
      err_flag |= PM_ERR_FLAG_INVALID_PERIPHERAL;
   }
   else
   {
       err_flag |= pm_pwr_ocp_latched_status_clear_alg(&(bob_ptr->pm_pwr_data), bob_ptr->comm_ptr, peripheral_index, ocp_occured);
   }

   return err_flag;
}


pm_err_flag_type 
pm_bob_set_rgb_headroom(uint8 pmic_chip, uint8 peripheral_index, uint32 volt_level)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
    pm_bob_data_type *bob_ptr = pm_bob_get_data(pmic_chip);
    uint32 calculated_vset = 0;
    pm_register_data_type data = 0;
    uint32 vmin = 0;  // microvolt
    uint32 vStep = 0; // microvolt
    //uint8 periph_type = 0;

    if (bob_ptr == NULL)
    {
        err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    else if (peripheral_index >= bob_ptr->pm_pwr_data.num_of_peripherals)
    {
        err_flag = PM_ERR_FLAG_INVALID_PERIPHERAL;
    }
    else
    {
       pm_register_address_type          rgb_hr_reg = 0;

        pm_pwr_data_type                  *pwr_data = &(bob_ptr->pm_pwr_data);

        if ((volt_level <= pwr_data->pwr_specific_info[peripheral_index].pwr_vset.RangeMax) && 
            (volt_level >= pwr_data->pwr_specific_info[peripheral_index].pwr_vset.RangeMin) )
        {
            vmin = pwr_data->pwr_specific_info[peripheral_index].pwr_vset.RangeMin;
            vStep = pwr_data->pwr_specific_info[peripheral_index].pwr_vset.VStep;
        }

        if (vStep > 0)
        {
            calculated_vset = (volt_level - vmin)/vStep;

            data = (calculated_vset) & 0xFF;

            rgb_hr_reg = (pm_register_address_type)(pwr_data->pwr_specific_info[peripheral_index].periph_base_address + pwr_data->pwr_reg_table->RGB_HR_VOUT_MAX); 

            err_flag = pm_comm_write_byte(bob_ptr->comm_ptr->slave_id, rgb_hr_reg, data, 0);
        }
    }

    return err_flag;
}
