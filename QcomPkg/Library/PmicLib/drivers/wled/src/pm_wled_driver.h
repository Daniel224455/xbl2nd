#ifndef __PM_WLED_DRIVER_H__
#define __PM_WLED_DRIVER_H__

/*! \file
 *  \n
 *  \brief  pm_wled_driver.h 
 *  \details  This file contains functions prototypes and variable/type/constant
 *  declarations for supporting WLED pin services for the Qualcomm
 *  PMIC chip set.
 *  \n &copy;  Copyright (c) 2010-2016 Qualcomm Technologies, Inc.  All Rights Reserved.
 */

/* =======================================================================
                             Edit History
  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.


when        who     what, where, why
--------    ---     ----------------------------------------------------------
03/29/16    al      Updating for newer target
07/16/14    akm     Comm change Updates
03/05/14    aab     Updated to the latest PMIC driver architecture    
04/25/13    aab     Adapted a reduced version of WLED driver for boot build   
========================================================================== */

/*===========================================================================
INCLUDE FILES FOR MODULE
===========================================================================*/

#include "pm_target_information.h"


/*===========================================================================

                     WLED TYPES AND STRUCTURES 

===========================================================================*/
typedef struct
{
    pm_register_address_type base_address;                  // 0xD800
    pm_register_address_type perph_type;                    // 0xD804
    pm_register_address_type perph_subtype;                 // 0xD805
    pm_register_address_type fault_status;                  // 0xD808
    pm_register_address_type int_rt_sts;                    // 0xD810
    pm_register_address_type int_set_type;                  // 0xD811
    pm_register_address_type int_polarity_high;             // 0xD812
    pm_register_address_type int_polarity_low;              // 0xD813
    pm_register_address_type int_latched_clr;               // 0xD814
    pm_register_address_type int_en_set;                    // 0xD815
    pm_register_address_type int_en_clr;                    // 0xD816
    pm_register_address_type int_latched_sts;               // 0xD818
    pm_register_address_type int_pending_sts;               // 0xD819
    pm_register_address_type int_mid_sel;                   // 0xD81A
    pm_register_address_type int_priority;                  // 0xD81B
    pm_register_address_type module_enable;                 // 0xD846
    pm_register_address_type feedback_control;              // 0xD848
    pm_register_address_type switching_frequency;           // 0xD84C
    pm_register_address_type wled_ovp;                      // 0xD84D
    pm_register_address_type wled_ilim;                     // 0xD84E
    pm_register_address_type sh_for_softstart;              // 0xD858                                           
    pm_register_address_type test4;                         // 0xD8E5
}pm_wled_ctrl_register_info_type;


typedef struct 
{
    pm_register_address_type   base_address;                       //0xD900
    pm_register_address_type   perph_type;                         //0x1D904
    pm_register_address_type   perph_subtype;                      //0x1D905
    pm_register_address_type   current_sink_en;                    //0x1D946
    pm_register_address_type   iled_sync_bit;                      //0x1D947
    pm_register_address_type   modulatora_en;                      //0x1D950
    pm_register_address_type   modulatora_src_sel;                 //0x1D951
    pm_register_address_type   modulatora_brightness_width_sel;    //0x1D952
    pm_register_address_type   modulatora_brightness_settings_lsb; //0x1D953
    pm_register_address_type   modulatora_brightness_settings_msb; //0x1D954
    pm_register_address_type   modulatorb_en;                      //0x1D960
    pm_register_address_type   modulatorb_src_sel;                 //0x1D961
    pm_register_address_type   modulatorb_brightness_width_sel;    //0x1D962
    pm_register_address_type   modulatorb_brightness_settings_lsb; //0x1D963
    pm_register_address_type   modulatorb_brightness_settings_msb; //0x1D964
    pm_register_address_type   modulator_sync_bit;                 //0x1D965
    pm_register_address_type   led1_full_scale_current;            //0x1D972
    pm_register_address_type   led1_src_sel;                       //0x1D973
    pm_register_address_type   led2_full_scale_current;            //0x1D982
    pm_register_address_type   led2_src_sel;                       //0x1D983
    pm_register_address_type   led3_full_scale_current;            //0x1D992
    pm_register_address_type   led3_src_sel;                       //0x1D993 
    pm_register_address_type   led4_full_scale_current;            //0x1D9A2
     pm_register_address_type  led4_src_sel;                       //0x1D9A3
}pm_wled_sink_register_info_type;


typedef struct
{
    pm_wled_ctrl_register_info_type*   wled_ctrl_reg;
    pm_wled_sink_register_info_type*   wled_sink_reg;
}pm_wled_register_info_type;


typedef struct
{
    pm_comm_info_type           *comm_ptr;
    pm_wled_register_info_type  *wled_reg_table;
    uint8                        num_of_peripherals;
}pm_wled_data_type;
/*===========================================================================

                     FUNCTION DECLARATION 

===========================================================================*/
void pm_wled_driver_init(pm_comm_info_type *comm_ptr, peripheral_info_type *peripheral_info, uint8 pmic_index);

pm_wled_data_type* pm_wled_get_data(uint8 pmic_index);

uint8 pm_wled_get_num_peripherals(uint8 pmic_index);

#endif //__PM_WLED_DRIVER_H__
