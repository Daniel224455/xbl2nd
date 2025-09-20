/*! \file pm_pwr_alg.c 
*  \n
*  \brief  
*  \n  
*  \n &copy; Copyright 2012-2018 QUALCOMM Technologies Incorporated, All Rights Reserved
*/

/* =======================================================================
                             Edit History
  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.


when       who     what, where, why
--------   ---     ----------------------------------------------------------
12/04/17   aab     Optimized driver for better mem usage
10/27/17   aab     Added support for new regualtor Mode in pm_pwr_sw_mode_status_alg() 
11/08/16   aab     Added pm_pwr_ocp_latched_status_clear_alg()
11/03/15   al      Updated rails API
07/22/14   aks     Adding API to round off voltage
12/06/12   hw      Rearchitecturing module driver to peripheral driver
========================================================================== */
/*===========================================================================

                     INCLUDE FILES 

===========================================================================*/
#include "pm_pwr_alg.h"
#include "pm_comm.h"

#define SMART_BOOST_OFFSET             0xE6 //store this offset here instead of config since this bit is going to be erased in new HW
#define STEPPER_OK                     0x81
#define VREG_OK                        0x80
#define STATUS2_OFFSET                 0x009
#define OCP_CTL2_OFFSET                0x04B
#define OCP_GLOBAL_BROADCAST_EN_MASK   0x04

#define WAIT_25_US                      25
#define VREG_OK_CHECK_MAX_ITERATION     160 //MAX wait time will be 160*25usec=4milli sec

/*===========================================================================

                     FUNCTION IMPLEMENTATION 

===========================================================================*/

static pm_err_flag_type pm_pwr_sw_vreg_settled_check_alg(pm_pwr_data_type *pwr_data, pm_comm_info_type *comm_ptr, uint8 peripheral_index, boolean *vreg_settled);

pm_err_flag_type pm_pwr_pull_down_alg(pm_pwr_data_type *pwr_data, pm_comm_info_type *comm_ptr, uint8 peripheral_index, pm_on_off_type on_off)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
   
    if ((peripheral_index >= pwr_data->num_of_peripherals) || (pwr_data->pwr_specific_info[peripheral_index].periph_type == 0))
    {
        err_flag = PM_ERR_FLAG_INVALID_PERIPHERAL;
    }
    else if (on_off >= PM_INVALID)
    {
        err_flag = PM_ERR_FLAG_INVALID_PARAMETER;
    }
    else
    {
        pm_register_address_type          reg = 0xFF;
        pm_register_data_type             data = 0xFF;

        reg = (pm_register_address_type)(pwr_data->pwr_specific_info[peripheral_index].periph_base_address + pwr_data->pwr_reg_table->PD_CTL);
    
        data = (PM_OFF == on_off) ? 0 : 0x80;

        err_flag = pm_comm_write_byte_mask(comm_ptr->slave_id, reg, 0x80, data, 0);
    }

    return err_flag;
}

pm_err_flag_type pm_pwr_pull_down_status_alg(pm_pwr_data_type *pwr_data,  pm_comm_info_type *comm_ptr, uint8 peripheral_index, pm_on_off_type *on_off)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;

    if ((pwr_data == NULL) || (comm_ptr == NULL) || (on_off == NULL))
    {
        return PM_ERR_FLAG_INVALID_PARAMETER;
    }

    if (peripheral_index >= pwr_data->num_of_peripherals)
    {
        err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    else
    {
        pm_register_address_type           reg = 0x0;
        pm_register_data_type              data = 0;


        reg = (pm_register_address_type)(pwr_data->pwr_specific_info[peripheral_index].periph_base_address + pwr_data->pwr_reg_table->PD_CTL);

        err_flag = pm_comm_read_byte(comm_ptr->slave_id, reg, &data, 0);
        if (err_flag != PM_ERR_FLAG_SUCCESS)
        {
            *on_off = PM_INVALID;
            return err_flag;
        }

        *on_off = (data & 0x80) ? PM_ON : PM_OFF;
    }

    return err_flag;
}


pm_err_flag_type pm_pwr_sw_mode_status_alg(pm_pwr_data_type *pwr_data, pm_comm_info_type *comm_ptr, uint8 peripheral_index, pm_sw_mode_type *sw_mode)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
    uint8 periph_type = 0;
    boolean pmic4_flag = FALSE;
                                                          
    if ((peripheral_index >= pwr_data->num_of_peripherals) || (pwr_data->pwr_specific_info[peripheral_index].periph_type == 0))
    {
        err_flag = PM_ERR_FLAG_INVALID_PERIPHERAL;
    }
    else if (sw_mode == NULL)
    {
        err_flag = PM_ERR_FLAG_INVALID_PARAMETER;
    }
    else
    {
        pm_register_address_type           reg = 0x0;
        pm_register_data_type              data = 0xFF;

        periph_type = pwr_data->pwr_specific_info[peripheral_index].periph_type;
            
        reg = (pm_register_address_type)(pwr_data->pwr_specific_info[peripheral_index].periph_base_address + pwr_data->pwr_reg_table->MODE_CTL);

        pmic4_flag = pwr_data->pwr_specific_info[peripheral_index].pmic4_flag;

        err_flag = pm_comm_read_byte(comm_ptr->slave_id, reg, &data, 0);
        
        if (err_flag == PM_ERR_FLAG_SUCCESS)
        {
            if (data == 0x07)
            {
                *sw_mode = PM_SW_MODE_NPM;
            }
            else if ((data == 0x06) && (periph_type != PM_HW_MODULE_LDO))
            {
                *sw_mode = PM_SW_MODE_AUTO;
            }
            else if (data == 0x05)
            {
                if (pmic4_flag)
                {
                    *sw_mode = PM_SW_MODE_LPM;
                }
                else
                {
                    *sw_mode = PM_SW_MODE_RETENTION;
                }
            }
            else if (data == 0x04)
            {
                if (pmic4_flag == FALSE)
                {
                    *sw_mode = PM_SW_MODE_LPM;
                }
                else
                {
                    *sw_mode = PM_SW_MODE_RETENTION;
                }
            }
            else if (data == 0x03)
            {
                if (pmic4_flag == FALSE)
                {
                    *sw_mode = PM_SW_MODE_RETENTION;
                }
                else if (periph_type == PM_HW_MODULE_LDO)
                {
                    *sw_mode = PM_SW_MODE_BYPASS;
                }
                else
                {
                    *sw_mode = PM_SW_MODE_INVALID;
                }
            }
            else if ((data <= 0x02) && (data >= 0x01) && (periph_type == PM_HW_MODULE_LDO))
            {
                *sw_mode = PM_SW_MODE_BYPASS;
            }
            else if ((data == 0x00) && (periph_type == PM_HW_MODULE_LDO) && (pmic4_flag == FALSE))
            {
                *sw_mode = PM_SW_MODE_BYPASS;
            }
            else
            {
                *sw_mode = PM_SW_MODE_INVALID;
            }
        }
    }
    return err_flag;
}

pm_err_flag_type pm_pwr_pin_ctrl_alg(pm_pwr_data_type *pwr_data, pm_comm_info_type *comm_ptr, uint8 peripheral_index, uint8 select_pin)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;

    if ((peripheral_index < pwr_data->num_of_peripherals) && (pwr_data->pwr_specific_info[peripheral_index].periph_type != 0))
    {
        pm_register_address_type reg = 0x0;

        if (pwr_data->pwr_specific_info[peripheral_index].is_periph_unified_reg ==  TRUE)
        {
            reg = (pm_register_address_type)(pwr_data->pwr_specific_info[peripheral_index].periph_base_address + pwr_data->pwr_reg_table->FOLLOW_HWEN);
        }
        else
        {
            reg = (pm_register_address_type)(pwr_data->pwr_specific_info[peripheral_index].periph_base_address + pwr_data->pwr_reg_table->EN_CTL);
        }

        err_flag = pm_comm_write_byte_mask(comm_ptr->slave_id, reg, 0x0F, (pm_register_data_type)select_pin, 1); // bit<0:3>
    }
    else
    {
        err_flag = PM_ERR_FLAG_INVALID_PERIPHERAL;
    }

    return err_flag;
}

pm_err_flag_type pm_pwr_pin_ctrl_mode_alg(pm_pwr_data_type *pwr_data, pm_comm_info_type *comm_ptr, uint8 peripheral_index, uint8 select_pin)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
    uint8 mask = 0;

    if ((peripheral_index < pwr_data->num_of_peripherals) && (pwr_data->pwr_specific_info[peripheral_index].periph_type != 0))
    {
        pm_register_address_type reg = 0x0;

        if (pwr_data->pwr_specific_info[peripheral_index].is_periph_unified_reg ==  TRUE)
        {
            reg = (pm_register_address_type)(pwr_data->pwr_specific_info[peripheral_index].periph_base_address + pwr_data->pwr_reg_table->FOLLOW_HWEN);
            mask = 0xF0;
            select_pin = (select_pin<<4);
        }
        else
        {
            reg = (pm_register_address_type)(pwr_data->pwr_specific_info[peripheral_index].periph_base_address + pwr_data->pwr_reg_table->MODE_CTL);
            mask = 0x1F;
        }

        err_flag = pm_comm_write_byte_mask(comm_ptr->slave_id, reg, mask, (pm_register_data_type)select_pin, 0); // bit<0:4>
    }
    else
    {
        err_flag = PM_ERR_FLAG_INVALID_PERIPHERAL;
    }

    return err_flag;
}


pm_err_flag_type pm_pwr_pin_ctrl_status_alg(pm_pwr_data_type *pwr_data, pm_comm_info_type *comm_ptr, uint8 peripheral_index, pm_on_off_type *on_off)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;

    if ((pwr_data == NULL) || (comm_ptr == NULL) || (on_off == NULL))
    {
        return PM_ERR_FLAG_INVALID_PARAMETER;
    }

    if (peripheral_index >= pwr_data->num_of_peripherals)
    {
        err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    else
    {
        pm_register_address_type           reg = 0x0;
        pm_register_data_type              data = 0;

        if (pwr_data->pwr_reg_table == NULL)
        {
            return PM_ERR_FLAG_INVALID_PARAMETER;
        }

        reg = (pm_register_address_type)(pwr_data->pwr_specific_info[peripheral_index].periph_base_address + pwr_data->pwr_reg_table->EN_CTL);

        err_flag = pm_comm_read_byte(comm_ptr->slave_id, reg, &data, 0);
        if (err_flag != PM_ERR_FLAG_SUCCESS)
        {
            *on_off = PM_INVALID;
            return err_flag;
        }

        if (data & 0x0F)
        {
            *on_off = PM_ON;
        }
        else
        {
            *on_off = PM_OFF;
        }
    }

    return err_flag;
}


pm_err_flag_type pm_pwr_sw_en_pin_ctl_alg(pm_pwr_data_type *pwr_data, pm_comm_info_type *comm_ptr, uint8 peripheral_index, 
                                          pm_on_off_type on_off , uint8 select_pin)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
    uint8 periph_type = 0;
    uint8 perph_revision = 0;
    uint8 sw_en = 0;
    uint8 pc_en = 0;

    if (peripheral_index < pwr_data->num_of_peripherals && 
              pwr_data->pwr_specific_info[peripheral_index].periph_type != 0)
    {
        pm_register_address_type smart_boost_reg = 0;   
        pm_register_address_type pin_ctl_reg = 0;   
        pm_register_address_type en_ctl_reg = (pm_register_address_type)(pwr_data->pwr_specific_info[peripheral_index].periph_base_address + pwr_data->pwr_reg_table->EN_CTL);
        periph_type = pwr_data->pwr_specific_info[peripheral_index].periph_type;


        if (pwr_data->pwr_specific_info[peripheral_index].is_periph_unified_reg ==  TRUE)
        {
            pin_ctl_reg = (pm_register_address_type)(pwr_data->pwr_specific_info[peripheral_index].periph_base_address + pwr_data->pwr_reg_table->FOLLOW_HWEN);

            if(on_off != 0)
            {
                sw_en = 0x80;
            }
            
            err_flag = pm_comm_write_byte(comm_ptr->slave_id, en_ctl_reg, sw_en, 1);

            err_flag |= pm_comm_write_byte_mask(comm_ptr->slave_id, pin_ctl_reg, 0x0F, (pm_register_data_type)select_pin, 1); // bit<0:3>
        }
        else if (periph_type == PM_HW_MODULE_BOOST)
        {
            smart_boost_reg = (pm_register_address_type)(pwr_data->pwr_specific_info[peripheral_index].periph_base_address + 0x1);

            err_flag = pm_comm_read_byte(comm_ptr->slave_id, smart_boost_reg, &perph_revision, 1);

            if(perph_revision <=2)
            {
                smart_boost_reg = (pm_register_address_type)(pwr_data->pwr_specific_info[peripheral_index].periph_base_address + SMART_BOOST_OFFSET);

                if(on_off != 0)
                {
                    sw_en = 0x80;
                }
                else if(select_pin != 0)
                {
                    sw_en = 0x80;
                    pc_en = 0x80;
                }
                err_flag = pm_comm_write_byte(comm_ptr->slave_id, smart_boost_reg, pc_en, 1); // bit<7>
                err_flag = pm_comm_write_byte(comm_ptr->slave_id, en_ctl_reg,  sw_en, 1); 
            } 
            else
            {
                if(on_off != 0)
                {
                    sw_en = 0x80;
                }

                err_flag = pm_comm_write_byte(comm_ptr->slave_id, en_ctl_reg,  ( sw_en | (0xF & select_pin)), 1); 
            }
        }   
        else
        {   
            if(on_off != 0)
            {
                sw_en = 0x80;
            }
            
            err_flag = pm_comm_write_byte(comm_ptr->slave_id, en_ctl_reg,  ( sw_en | (0xF & select_pin)), 1); 
        }
        
    }
    else
    {
        err_flag = PM_ERR_FLAG_INVALID_PERIPHERAL;
    }
 
    return err_flag;
}


pm_err_flag_type pm_pwr_sw_enable_alg(pm_pwr_data_type *pwr_data, pm_comm_info_type *comm_ptr, uint8 peripheral_index, pm_on_off_type on_off, boolean wait_for_settle)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;

    boolean vreg_ready = FALSE;

    if ((peripheral_index >= pwr_data->num_of_peripherals) || (pwr_data->pwr_specific_info[peripheral_index].periph_type == 0))
    {
        err_flag = PM_ERR_FLAG_INVALID_PERIPHERAL;
    }
    else if (on_off >= PM_INVALID)
    {
        err_flag = PM_ERR_FLAG_INVALID_PARAMETER;
    }
    else
    {
        pm_register_address_type reg = 0x0;
        pm_register_data_type data = 0;

        //periph_type = pwr_data->pwr_specific_info[peripheral_index].periph_type;
            
        reg = (pm_register_address_type)(pwr_data->pwr_specific_info[peripheral_index].periph_base_address + pwr_data->pwr_reg_table->EN_CTL);

        if (PM_OFF == on_off)
        {
            data = 0;
			wait_for_settle = FALSE;
        }
        else
        {
            data = 0x80;
        }

        if (pwr_data->pwr_specific_info[peripheral_index].is_periph_unified_reg ==  TRUE)
        {
            err_flag = pm_comm_write_byte(comm_ptr->slave_id, reg, data, 0);
        }
        else
        {
            err_flag = pm_comm_write_byte_mask(comm_ptr->slave_id, reg, 0x80, data, 0);
        }

        if ((wait_for_settle == TRUE) && (err_flag == PM_ERR_FLAG_SUCCESS)) 
        {
          err_flag = pm_pwr_sw_vreg_settled_check_alg(pwr_data, comm_ptr, peripheral_index, &vreg_ready);

          if (vreg_ready == FALSE)
          {
              err_flag |= PM_ERR_FLAG_FAILURE;
          }
        }
    }

    return err_flag;
}

static pm_err_flag_type pm_pwr_sw_vreg_settled_check_alg(pm_pwr_data_type *pwr_data, pm_comm_info_type *comm_ptr, uint8 peripheral_index, boolean *vreg_settled)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
    pm_on_off_type on_off;
    boolean vreg_ready;

    uint32 vreg_ok_check_iteration = 0;

    if ((peripheral_index >= pwr_data->num_of_peripherals) || (pwr_data->pwr_specific_info[peripheral_index].periph_type == 0))
    {
        err_flag = PM_ERR_FLAG_INVALID_PERIPHERAL;
    }
    else if (NULL == vreg_settled)
    {
        err_flag = PM_ERR_FLAG_INVALID_PARAMETER;
    }
    else
    {
        err_flag = pm_pwr_sw_enable_status_alg(pwr_data, comm_ptr, peripheral_index, &on_off);

        if (err_flag != PM_ERR_FLAG_SUCCESS)
        {
           return err_flag;
        }
        else if (PM_OFF == on_off)
        {
           *vreg_settled = TRUE;
            return err_flag;
        }

        //PM_ON case
        do
        {
          err_flag |= pm_pwr_is_vreg_ready_alg(pwr_data, comm_ptr, peripheral_index, &vreg_ready);

          if (vreg_ready == TRUE) 
          {
            break;
          }

          pm_busy_wait(WAIT_25_US);

        }while(++vreg_ok_check_iteration < VREG_OK_CHECK_MAX_ITERATION);

        *vreg_settled = vreg_ready;
    }

    return err_flag;
}




pm_err_flag_type pm_pwr_sw_enable_status_alg(pm_pwr_data_type *pwr_data, pm_comm_info_type *comm_ptr, uint8 peripheral_index, pm_on_off_type *on_off)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;

    if ((peripheral_index >= pwr_data->num_of_peripherals) || (pwr_data->pwr_specific_info[peripheral_index].periph_type == 0))
    {
        err_flag = PM_ERR_FLAG_INVALID_PERIPHERAL;
    }
    else if (on_off == NULL)
    {
        err_flag = PM_ERR_FLAG_INVALID_PARAMETER;
    }
    else
    {
        pm_register_address_type           reg = 0x0;
        pm_register_data_type              data = 0;

        reg = (pm_register_address_type)(pwr_data->pwr_specific_info[peripheral_index].periph_base_address + pwr_data->pwr_reg_table->EN_CTL);

        err_flag = pm_comm_read_byte(comm_ptr->slave_id, reg, &data, 0);
        if (err_flag != PM_ERR_FLAG_SUCCESS)
        {
            *on_off = PM_INVALID;
            return err_flag;
        }

        if (data & 0x80)
        {
            *on_off = PM_ON;
        }
        else
        {
            *on_off = PM_OFF;
        }
    }

    return err_flag;
}

pm_err_flag_type pm_pwr_is_vreg_ready_alg(pm_pwr_data_type *pwr_data, pm_comm_info_type *comm_ptr, 
                                                 uint8 peripheral_index, boolean *vreg_ready)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
    uint8 vreg_mask = 0;

    if (peripheral_index >= pwr_data->num_of_peripherals || 
             pwr_data->pwr_specific_info[peripheral_index].periph_type == 0)
    {
        err_flag = PM_ERR_FLAG_INVALID_PERIPHERAL;
    }
    else if (vreg_ready == NULL)
    {
        err_flag = PM_ERR_FLAG_INVALID_PARAMETER;
    }
    else
    {
        pm_register_address_type reg = 0x0;
        pm_register_data_type data = 0;

        reg = (pm_register_address_type)(pwr_data->pwr_specific_info[peripheral_index].periph_base_address + pwr_data->pwr_reg_table->STATUS);

        err_flag = pm_comm_read_byte(comm_ptr->slave_id, reg, &data, 0);
        if (err_flag != PM_ERR_FLAG_SUCCESS)
        {
            err_flag = pm_comm_read_byte(comm_ptr->slave_id, reg, &data, 0);
            if (err_flag != PM_ERR_FLAG_SUCCESS)
            {
                *vreg_ready = FALSE;
                return err_flag;
            }
        }

        if (pwr_data->pwr_specific_info[peripheral_index].is_periph_unified_reg == TRUE)
        {
            // In the new peripherals, VREG_READY bit is used which indicates both VREG_OK and STEPPER_OK
            vreg_mask = VREG_OK; 
        }
        else if (pwr_data->pwr_specific_info[peripheral_index].is_periph_stepper == TRUE)
        {
            vreg_mask = STEPPER_OK; 
        }
        else
        {
            vreg_mask = VREG_OK;
        }

        if ((data & vreg_mask) == vreg_mask)
        {
            *vreg_ready = TRUE;
        }
        else
        {
            *vreg_ready = FALSE;
        }
    }

    return err_flag;
}

pm_err_flag_type pm_pwr_status_reg_dump_alg(pm_pwr_data_type *pwr_data, pm_comm_info_type *comm_ptr, uint8 peripheral_index, 
                                            uint8 *status_reg_data, uint32 num_of_regs)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;

    if ((peripheral_index >= pwr_data->num_of_peripherals) || (pwr_data->pwr_specific_info[peripheral_index].periph_type == 0))
    {
        err_flag = PM_ERR_FLAG_INVALID_PERIPHERAL;
    }
    else if (status_reg_data == NULL)
    {
        err_flag = PM_ERR_FLAG_INVALID_PARAMETER;
    }
    else
    {
        pm_register_address_type reg = 0x0;

        reg = (pm_register_address_type)(pwr_data->pwr_specific_info[peripheral_index].periph_base_address + pwr_data->pwr_reg_table->STATUS);

        err_flag = pm_comm_read_byte_array(comm_ptr->slave_id, reg, num_of_regs, status_reg_data, 0);
    }

    return err_flag;
}

pm_err_flag_type pm_pwr_volt_level_alg(pm_pwr_data_type *pwr_data, pm_comm_info_type *comm_ptr, uint8 peripheral_index, pm_volt_level_type  volt_level, boolean wait_for_settle)
{
    pm_err_flag_type                  err_flag = PM_ERR_FLAG_SUCCESS;
    uint32                            vmin = 0;  // microvolt
    uint32                            vStep = 0; // microvolt
    uint32                            calculated_vset = 0;
    pm_register_data_type             vset_ht[2] = { 0 };
    pm_register_address_type          reg = 0;

    boolean vreg_settled = FALSE;

    if (peripheral_index >= (pwr_data->num_of_peripherals) ||
        pwr_data->pwr_specific_info[peripheral_index].periph_type == 0)
    {
        err_flag = PM_ERR_FLAG_INVALID_PERIPHERAL;
    } 
    else
    {
        if ((volt_level <= pwr_data->pwr_specific_info[peripheral_index].pwr_vset.RangeMax) &&
            (volt_level >= pwr_data->pwr_specific_info[peripheral_index].pwr_vset.RangeMin))
        {
            vmin = pwr_data->pwr_specific_info[peripheral_index].pwr_vset.RangeMin;
            vStep = pwr_data->pwr_specific_info[peripheral_index].pwr_vset.VStep;
        }

        if (vStep > 0)
        {
            calculated_vset = (volt_level - vmin) / vStep;

            if (pwr_data->pwr_specific_info[peripheral_index].is_periph_unified_reg ==  TRUE)
            {
                vset_ht[0] = ((calculated_vset) & 0xFF);
                vset_ht[1] = ((calculated_vset >> 8) & 0xFF);

                reg =  (pm_register_address_type)(pwr_data->pwr_specific_info[peripheral_index].periph_base_address + pwr_data->pwr_reg_table->VOLTAGE_CTRL1);

                // Set vset
                err_flag = pm_comm_write_byte_array(comm_ptr->slave_id, reg, 2, vset_ht, 0);

                if ((wait_for_settle == TRUE) && (err_flag == PM_ERR_FLAG_SUCCESS))
                {
                    err_flag = pm_pwr_sw_vreg_settled_check_alg(pwr_data, comm_ptr, peripheral_index, &vreg_settled);

                    if (vreg_settled == FALSE)
                    {
                        err_flag |= PM_ERR_FLAG_FAILURE;
                    }
                }
            } 
            else
            {
                err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
            }
        } 
        else
        {
            err_flag = PM_ERR_FLAG_INVALID_PARAMETER;
        }
    }

    return err_flag;
}




pm_err_flag_type pm_pwr_volt_level_status_alg(pm_pwr_data_type *pwr_data, pm_comm_info_type *comm_ptr, uint8  peripheral_index, pm_volt_level_type *volt_level)
{
    pm_err_flag_type                 err_flag = PM_ERR_FLAG_SUCCESS;
    uint32                           vmin = 0; // microvolt
    uint32                           vStep = 0; //microvolt
    uint16                           vset_ht = 0;
    pm_register_address_type         reg = 0;
    pm_register_data_type            reg_data[2] = {0};
    boolean                          vreg_ready = FALSE;

    if (peripheral_index >= (pwr_data->num_of_peripherals) || 
            pwr_data->pwr_specific_info[peripheral_index].periph_type == 0)
    {
        err_flag = PM_ERR_FLAG_INVALID_PERIPHERAL;
    }
    else if (volt_level == NULL)
    {
        err_flag = PM_ERR_FLAG_INVALID_PARAMETER;
    }
    else
    {
        err_flag |= pm_pwr_is_vreg_ready_alg(pwr_data, comm_ptr, peripheral_index, &vreg_ready);

        if ((pwr_data->pwr_specific_info[peripheral_index].is_periph_unified_reg ==  TRUE) && 
            (vreg_ready ==  TRUE))
        {
            reg =  (pm_register_address_type)(pwr_data->pwr_specific_info[peripheral_index].periph_base_address + pwr_data->pwr_reg_table->VSET_VALID_LB);
        }
        else
        {
            // Get the voltage level LUT
            reg =  (pm_register_address_type)(pwr_data->pwr_specific_info[peripheral_index].periph_base_address + pwr_data->pwr_reg_table->VOLTAGE_CTRL1);
        }

        err_flag |= pm_comm_read_byte_array(comm_ptr->slave_id, reg, 2, reg_data, 0);

        if ( err_flag == PM_ERR_FLAG_SUCCESS )
        {
            vmin  = pwr_data->pwr_specific_info[peripheral_index].pwr_vset.RangeMin; // Micro volt
            vStep = pwr_data->pwr_specific_info[peripheral_index].pwr_vset.VStep;    // Micro volt

            if (pwr_data->pwr_specific_info[peripheral_index].is_periph_unified_reg == TRUE)
            {
                vset_ht = (reg_data[1]<<8) | (reg_data[0]);

                *volt_level = vmin + vset_ht * vStep;
            }
            else
            {
               err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
            }
        }
    }

    return err_flag;
}

pm_err_flag_type pm_pwr_uls_alg(pm_pwr_data_type *pwr_data, pm_comm_info_type *comm_ptr, uint8 peripheral_index, pm_volt_level_type volt_level)
{
  pm_err_flag_type                  err_flag = PM_ERR_FLAG_SUCCESS;
  uint32                            calculated_vset = 0;
  pm_register_data_type             vset_ht[2] = { 0 };
  pm_register_address_type          reg = 0;

  if (peripheral_index >= (pwr_data->num_of_peripherals) ||
      pwr_data->pwr_specific_info[peripheral_index].periph_type == 0)
  {
    err_flag = PM_ERR_FLAG_INVALID_PERIPHERAL;
  } 
  else
  {
    calculated_vset = volt_level / 1000; // translate from uV to mV
    calculated_vset = (calculated_vset + 7) & (~0x07); // round up to 8mV LSB as it was required in SWI

    if (pwr_data->pwr_specific_info[peripheral_index].is_periph_unified_reg == TRUE)
    {
      vset_ht[0] = ((calculated_vset) & 0xFF);
      vset_ht[1] = ((calculated_vset >> 8) & 0xFF);

      reg =  (pm_register_address_type)(pwr_data->pwr_specific_info[peripheral_index].periph_base_address + pwr_data->pwr_reg_table->ULS_VSET_LB);

      // Set vset
      err_flag = pm_comm_write_byte_array(comm_ptr->slave_id, reg, 2, vset_ht, 0);
    } 
    else
    {
      err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
  }

  return err_flag;
}


pm_err_flag_type pm_pwr_volt_calculate_vset_celing_uv(pm_pwr_data_type *pwr_data, pm_comm_info_type *comm_ptr, 
                                                      uint8 peripheral_index, pm_volt_level_type req_volt_level, 
                                                      uint32 *calculated_vset, pm_volt_level_type *ceiling_uv )
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
    uint32           vmin  = 0;  // microvolt
    uint32           vStep = 0;  // microvolt
    uint32           vset  =  0;

    if (peripheral_index >= (pwr_data->num_of_peripherals) || 
            pwr_data->pwr_specific_info[peripheral_index].periph_type == 0)
    {
        err_flag = PM_ERR_FLAG_INVALID_PERIPHERAL;
    }
    else if (calculated_vset == NULL || ceiling_uv == NULL)
    {
        err_flag = PM_ERR_FLAG_INVALID_PARAMETER;
    }
    else
    {
        if ((req_volt_level <= pwr_data->pwr_specific_info[peripheral_index].pwr_vset.RangeMax) && 
            (req_volt_level >= pwr_data->pwr_specific_info[peripheral_index].pwr_vset.RangeMin))
        {
            vmin = pwr_data->pwr_specific_info[peripheral_index].pwr_vset.RangeMin;
            vStep = pwr_data->pwr_specific_info[peripheral_index].pwr_vset.VStep;
        }
        else
        {
            err_flag = PM_ERR_FLAG_INVALID_PARAMETER;
        }
           
        if (vStep > 0)
        {
            // Calculate VSET
            vset = ((req_volt_level - vmin)/vStep);

            /* Check if rounding off vset is required - if the voltage request
               cannot be met with the vstep , then it ends up being set to one
               vstep lower. So move it to next higher vstep boundary */
             if( ((vset*vStep) + vmin)  <  req_volt_level )
             {
                // Increment the voltage by a step count if the voltage is less than the expected 
                vset++; 
                *ceiling_uv =  (vset*vStep) + vmin; 
             }
             *calculated_vset = vset;
        }
        else
        {
            err_flag = PM_ERR_FLAG_INVALID_PARAMETER;
        }
    }

    return err_flag;
}





pm_err_flag_type pm_pwr_ocp_latched_status_clear_alg(pm_pwr_data_type *pwr_data, pm_comm_info_type *comm_ptr, uint8 peripheral_index, boolean *ocp_occured)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
    uint8 periph_type = 0;
    pm_register_address_type ocp_status_reg = 0x0;
    pm_register_address_type ocp_reg  = 0x0;
    pm_register_data_type status_data = 0;
    pm_register_data_type ocp_clear_data = 0;
    pm_register_data_type status_mask    = 0;
    pm_register_data_type ocp_clear_mask = 0;
  
      
    if ((peripheral_index >= pwr_data->num_of_peripherals) || (pwr_data->pwr_specific_info[peripheral_index].periph_type == 0) )
    {
        err_flag |= PM_ERR_FLAG_INVALID_PERIPHERAL;
    }
    else if (NULL == ocp_occured)
    {
       err_flag |= PM_ERR_FLAG_INVALID_PARAMETER;
    }
    else
    {
        periph_type = pwr_data->pwr_specific_info[peripheral_index].periph_type;

        if (pwr_data->pwr_specific_info[peripheral_index].is_periph_unified_reg == TRUE)
        {
            ocp_status_reg = (pm_register_address_type)(pwr_data->pwr_specific_info[peripheral_index].periph_base_address + pwr_data->pwr_reg_table->STATUS);
            status_mask = 0x1 << 5;
            ocp_reg = (pm_register_address_type)(pwr_data->pwr_specific_info[peripheral_index].periph_base_address + pwr_data->pwr_reg_table->OCP);
            ocp_clear_mask = 0x1 << 1;
            ocp_clear_data = 0x1 << 1;
        }
        else if (periph_type == PM_HW_MODULE_ULT_LDO)
        {
            /*Only PMOS LDO have OCP but for other LDO either these registers are missing or the bit field is missing 
              and so status will never reported to be high  */
            ocp_status_reg = (pm_register_address_type)(pwr_data->pwr_specific_info[peripheral_index].periph_base_address + STATUS2_OFFSET);
            status_mask = 0x1 << 6;
            ocp_reg = (pm_register_address_type)(pwr_data->pwr_specific_info[peripheral_index].periph_base_address + OCP_CTL2_OFFSET);
            ocp_clear_mask = 0x1 << 6;
            ocp_clear_data = 0x1 << 6;
        }
        else
        {
            return PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
        }

        err_flag |= pm_comm_read_byte(comm_ptr->slave_id, ocp_status_reg, &status_data, 0);
        if (err_flag != PM_ERR_FLAG_SUCCESS)
        {
            err_flag |= pm_comm_read_byte(comm_ptr->slave_id, ocp_status_reg, &status_data, 0);
        }
        CORE_VERIFY(err_flag == PM_ERR_FLAG_SUCCESS);

        *ocp_occured = (status_data & status_mask)? TRUE : FALSE;

        if (*ocp_occured)
        {
           //OCP_STATUS_CLEAR : Write 1d then 0 (HW needs to see a 1 to 0 transition)
           err_flag |= pm_comm_write_byte_mask(comm_ptr->slave_id, ocp_reg, ocp_clear_mask, ocp_clear_data, 0);
           err_flag |= pm_comm_write_byte_mask(comm_ptr->slave_id, ocp_reg, ocp_clear_mask, 0, 0);
        }

    }
    return err_flag;
}


pm_err_flag_type pm_pwr_ocp_broadcast_alg(pm_pwr_data_type *pwr_data, pm_comm_info_type *comm_ptr, uint8 peripheral_index, pm_on_off_type on_off)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
    pm_register_address_type ocp_reg = 0x0;

    pm_register_data_type ocp_broadcast_data = (on_off) ? (0xFF & OCP_GLOBAL_BROADCAST_EN_MASK) : 0x0;

    if ((peripheral_index >= pwr_data->num_of_peripherals) || (pwr_data->pwr_specific_info[peripheral_index].periph_type == 0))
    {
        err_flag = PM_ERR_FLAG_INVALID_PERIPHERAL;
    }
    else
    {
        ocp_reg = (pm_register_address_type)(pwr_data->pwr_specific_info[peripheral_index].periph_base_address + pwr_data->pwr_reg_table->OCP);
        err_flag |= pm_comm_write_byte_mask(comm_ptr->slave_id, ocp_reg, OCP_GLOBAL_BROADCAST_EN_MASK, ocp_broadcast_data, 0);
    }

    return err_flag;
}

