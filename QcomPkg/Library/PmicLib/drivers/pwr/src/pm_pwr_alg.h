#ifndef __PM_PWR_ALG_H__
#define __PM_PWR_ALG_H__

/*! \file pm_pwr_alg.h
 *  \n
 *  \brief   
 *  \details  
 *  \n &copy; Copyright 2012-2018 Qualcomm Technologies, Inc. All Rights Reserved
 */

/* =======================================================================
                             Edit History
  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.


when       who     what, where, why
--------   ---     ----------------------------------------------------------
12/04/17   aab     Optimized driver for better mem usage
10/27/17   aab     Updated pm_pwr_specific_info_type 
11/08/16   aab     Added pm_pwr_ocp_latched_status_clear_alg()
11/03/15   al      Updated rails API
07/16/14   akm     Comm change Updates
14/11/13   sv      Added Enum "SEC_ACCESS" to struct pm_pwr_register_info_typ (CR-567633)
12/06/12   hw      Rearchitecturing module driver to peripheral driver
========================================================================== */

/*===========================================================================

                     INCLUDE FILES 

===========================================================================*/
#include "pm_err_flags.h"
#include "pm_utils.h"
#include "bare_metal_info.h"
#include "pm_resources_and_types.h"
#include "hw_module_type.h"
#include "CoreVerify.h"
#include "pm_comm.h"


/*===========================================================================

                      TYPES AND STRUCTURES 

===========================================================================*/
#define PM_VOLT_INVALID_RANGE 255

typedef struct
{
    unsigned    start:16;
    unsigned    end:16;
}pm_pwr_range_info_type;


/* These fields are mapped to registers that are needed by power algorithms */
typedef struct
{
    pm_register_address_type    base_address;
    pm_register_address_type    peripheral_offset;
    //                                                                    COMMON/SMPS LDO     PMXX24_SMPS    PM855_BOB   PM855_VS
    pm_register_address_type    STATUS;            // STATUS                  0x08 
    pm_register_address_type    STATUS4;           // STATUS                  0x0B
    pm_register_address_type    ULS_VSET_LB;       // ULS_VSET_LB             0x39                            0x68
    pm_register_address_type    VOLTAGE_CTRL1;     // VOLTAGE_CTL1            0x40 
    pm_register_address_type    VOLTAGE_CTRL2;     // VOLTAGE_CTL2            0x41 
    pm_register_address_type    VSET_VALID_LB;     // VSET_VALID_LB           0x42
    pm_register_address_type    VSET_VALID_UB;     // VSET_VALID_UB           0x43
    pm_register_address_type    MODE_CTL;          // MODE_CTL                0x45 
    pm_register_address_type    EN_CTL;            // EN_CTL                  0x46 
    pm_register_address_type    PD_CTL;            // PD_CTL                  0xA0                             0x48 
    pm_register_address_type    FOLLOW_HWEN;       // FOLLOW_HWEN             0x47                             0x00
    pm_register_address_type    CLK_DIV;           // CLK_DIV(S_CTRL, S_FREQ) 0x50    0x00                     0x00
    pm_register_address_type    QM_MODE;           // QM_MODE(BCUCK_CMN)      0x51    0x00           0x51      0x00
    pm_register_address_type    QM_PS_ENTRY;       // QM_PS_ENTRY(S_PS)       0x63    0x00           0x63      0x00
    pm_register_address_type    QM_PS_EXIT;        // QM_PS_EXIT (S_CTRL)     0x6B    0x00           0x6B      0x00
    pm_register_address_type    ECM_EN_CTL;        // ECM_EN_CTL              0x00    0x00           0x80      0x00
    pm_register_address_type    OCP;               // OCP                     0x88                             0x00
    pm_register_address_type    VOUT_EXT_CTRL1_LB; //VOUT_EXT_CTRL1_LB(forBOB)0x00                             0x4A
    pm_register_address_type    PMIC4_ULS_VSET_LB; // PMIC4 ULS_VSET_LB       0x68                             0x00
    pm_register_address_type    RGB_HR_VOUT_MAX;   //BOB RGB_HR_VOUT_MAX                                       0x6E                             
}pm_pwr_register_info_type;



typedef struct
{
    pm_pwr_volt_info_type         pwr_vset;
    pm_register_address_type      periph_base_address;
    uint8                         periph_type;
    boolean                       is_periph_stepper;
    boolean                       is_periph_unified_reg;
    boolean                       pmic4_flag;
    boolean                       is_ret_present;
}pm_pwr_specific_info_type;


typedef struct
{
    pm_pwr_register_info_type     *pwr_reg_table;
    uint32                        num_of_peripherals;
    pm_pwr_specific_info_type     *pwr_specific_info;
}pm_pwr_data_type;



/*===========================================================================

                     FUNCTION DECLARATIONS 

===========================================================================*/

pm_err_flag_type pm_pwr_pull_down_alg(pm_pwr_data_type *pwr_data, pm_comm_info_type *comm_ptr, uint8 peripheral_index, pm_on_off_type on_off);

pm_err_flag_type pm_pwr_pull_down_status_alg(pm_pwr_data_type *pwr_data,  pm_comm_info_type *comm_ptr, uint8 peripheral_index, pm_on_off_type *on_off);

pm_err_flag_type pm_pwr_sw_mode_status_alg(pm_pwr_data_type *pwr_data, pm_comm_info_type *comm_ptr, uint8 peripheral_index, pm_sw_mode_type *mode);

pm_err_flag_type pm_pwr_sw_mode_status_raw_alg(pm_pwr_data_type *pwr_data, pm_comm_info_type *comm_ptr, uint8 peripheral_index, uint8 *mode_ctl);

pm_err_flag_type pm_pwr_pin_ctrl_alg(pm_pwr_data_type *pwr_data, pm_comm_info_type *comm_ptr, uint8 peripheral_index, uint8 select_pin);
  
pm_err_flag_type pm_pwr_pin_ctrl_status_alg(pm_pwr_data_type *pwr_data, pm_comm_info_type *comm_ptr, uint8 peripheral_index, pm_on_off_type *on_off);

pm_err_flag_type pm_pwr_pin_ctrl_mode_alg(pm_pwr_data_type *pwr_data, pm_comm_info_type *comm_ptr, uint8 peripheral_index, uint8 select_pin);

pm_err_flag_type pm_pwr_sw_enable_alg(pm_pwr_data_type *pwr_data, pm_comm_info_type *comm_ptr, uint8 peripheral_index, pm_on_off_type on_off, boolean wait_for_settle);

pm_err_flag_type pm_pwr_sw_en_pin_ctl_alg(pm_pwr_data_type *pwr_data, pm_comm_info_type *comm_ptr, uint8 peripheral_index, 
	                                           pm_on_off_type on_off , uint8 select_pin);


pm_err_flag_type pm_pwr_sw_enable_status_alg(pm_pwr_data_type *pwr_data, pm_comm_info_type *comm_ptr, uint8 peripheral_index, pm_on_off_type *on_off);

pm_err_flag_type pm_pwr_is_vreg_ready_alg(pm_pwr_data_type *pwr_data, pm_comm_info_type *comm_ptr, 
	                                                 uint8 peripheral_index, boolean *vreg_ready);

pm_err_flag_type pm_pwr_status_reg_dump_alg(pm_pwr_data_type *pwr_data, pm_comm_info_type *comm_ptr, uint8 peripheral_index, uint8 *status_reg_data, uint32 num_of_regs);

pm_err_flag_type pm_pwr_volt_level_alg(pm_pwr_data_type *pwr_data, pm_comm_info_type *comm_ptr, uint8 peripheral_index, pm_volt_level_type volt_level, boolean wait_for_settle);

pm_err_flag_type pm_pwr_volt_level_status_alg(pm_pwr_data_type *pwr_data, pm_comm_info_type *comm_ptr, uint8 peripheral_index, pm_volt_level_type *volt_level);

pm_err_flag_type pm_pwr_volt_calculate_vset_celing_uv(pm_pwr_data_type *pwr_data, pm_comm_info_type *comm_ptr, uint8  peripheral_index, pm_volt_level_type req_volt_level, uint32 *calculated_vset, pm_volt_level_type *ceiling_uv );

pm_err_flag_type pm_pwr_volt_get_vstep(pm_pwr_data_type *pwr_data, pm_comm_info_type *comm_ptr, uint8 peripheral_index, uint8 *vstep_ptr);

pm_err_flag_type pm_pwr_irq_enable_alg(pm_pwr_data_type *pwr_data, pm_comm_info_type *comm_ptr, uint8 peripheral_index, pm_pwr_irq_type irq, boolean enable);

pm_err_flag_type pm_pwr_irq_clear_alg(pm_pwr_data_type *pwr_data, pm_comm_info_type *comm_ptr, uint8 peripheral_index, pm_pwr_irq_type irq);

pm_err_flag_type pm_pwr_irq_set_trigger_alg(pm_pwr_data_type *pwr_data, pm_comm_info_type *comm_ptr, uint8 peripheral_index, pm_pwr_irq_type irq, pm_irq_trigger_type trigger);

pm_err_flag_type pm_pwr_irq_register_isr_alg(pm_pwr_data_type *pwr_data, pm_comm_info_type *comm_ptr, uint8 peripheral_index, pm_pwr_irq_type irq, void *isr_ptr, uint32 isr_ctx);

pm_err_flag_type pm_pwr_ocp_latched_status_clear_alg(pm_pwr_data_type *pwr_data, pm_comm_info_type *comm_ptr, uint8 peripheral_index, boolean *ocp_occured);

pm_err_flag_type pm_pwr_ocp_broadcast_alg(pm_pwr_data_type *pwr_data, pm_comm_info_type *comm_ptr, uint8 peripheral_index, pm_on_off_type on_off);
pm_err_flag_type pm_pwr_uls_alg(pm_pwr_data_type *pwr_data, pm_comm_info_type *comm_ptr, uint8 peripheral_index, pm_volt_level_type volt_level);

#endif /* __PM_PWR_ALG_H__ */
