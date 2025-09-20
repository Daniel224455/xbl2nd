#ifndef __PM_BMD_DRIVER_H__
#define __PM_BMD_DRIVER_H__

/*! \file pm_bmd_driver.h 
*  \n
*  \brief  This file contains functions prototypes and variable/type/constant
*         declarations for supporting bmd peripheral.
*  
*  \n &copy; Copyright 2017-2018 QUALCOMM Technologies Incorporated, All Rights Reserved
*/

/* =======================================================================
Edit History
This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.


when         who      what, where, why
--------   ---      ----------------------------------------------------------
02/07/18   xmp      Implement BMD driver
11/20/17   richaa   Created
========================================================================== */

/*===========================================================================
						INCLUDE FILES
===========================================================================*/


#include "pm_target_information.h"
#include "pm_comm.h"

/*===========================================================================

                     BMD TYPES AND STRUCTURES 

===========================================================================*/
typedef struct
{
    pm_register_address_type base_address;                  // 0x1A00
    pm_register_address_type bmd_bat_miss_sts;              // 0x08
    pm_register_address_type bmd_fsm_sts;                   // 0x09
    pm_register_address_type bmd_mode_ctl;                  // 0x40
    pm_register_address_type bmd_timing;                    // 0x41
    pm_register_address_type bmd_deb_ctl;                   // 0x42
    pm_register_address_type bmd_threshold_ctl;             // 0x43  the name in ipcat is bmd_ctl. Add "threshold" for readability
    pm_register_address_type bmd_src_ctl;                   // 0x44
    pm_register_address_type bmd_bmd_adc_ctl;               // 0x45
    pm_register_address_type bmd_bmd_en;                    // 0x46
    pm_register_address_type bmd_bmd_adc_dly;               // 0x47
    pm_register_address_type bmd_lockbit;                   // 0xD1
} pm_bmd_register_info_type;


typedef struct
{
    pm_comm_info_type            *comm_ptr;
    pm_bmd_register_info_type    *bmd_reg_table;
    uint8                        num_of_peripherals;
}pm_bmd_data_type;


/*===========================================================================

                     FUNCTION DECLARATION 

===========================================================================*/
void pm_bmd_driver_init(pm_comm_info_type *comm_ptr, peripheral_info_type *peripheral_info, uint8 pmic_index);

pm_bmd_data_type* pm_bmd_get_data(uint8 pmic_index);


#endif // __PM_BMD_DRIVER_H__

