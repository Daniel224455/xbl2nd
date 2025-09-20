/*! \file pm_bmd_driver.c 
*  \n
*  \brief This file contains bmd peripheral driver initialization during which the driver data is stored.
*  \n
*  \n &copy; Copyright 2017-2018 QUALCOMM Technologies Incorporated, All Rights Reserved
*/

/* =======================================================================
                             Edit History
  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.


when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
02/07/18   xmp     Implement BMD driver
11/20/17   richaa  Created
========================================================================== */

/*===========================================================================

                     INCLUDE FILES 

===========================================================================*/
#include "pm_bmd_driver.h"
#include "CoreVerify.h"

/*===========================================================================

                        STATIC VARIABLES 

===========================================================================*/

/* Static global variable to store the BMD data */
static pm_bmd_data_type *pm_bmd_data_arr[PM_MAX_NUM_PMICS];

static pm_bmd_register_info_type bmd_reg[1] = 
{
    {
        0x1A00,    // base_address
        0x08,      // bmd_bat_miss_sts
        0x09,      // bmd_fsm_sts
        0x40,      // bmd_mode_ctl
        0x41,      // bmd_timing
        0x42,      // bmd_deb_ctl
        0x43,      // bmd_threshold_ctl
        0x44,      // bmd_src_ctl
        0x45,      // bmd_bmd_adc_ctl
        0x46,      // bmd_bmd_en
        0x47,      // bmd_bmd_adc_dly
        0xD1,      // bmd_lockbit
     }
};


/*===========================================================================

                        FUNCTION DEFINITIONS

===========================================================================*/
void pm_bmd_driver_init(pm_comm_info_type *comm_ptr, peripheral_info_type *peripheral_info, uint8 pmic_index)
{
	pm_bmd_data_type *bmd_ptr = pm_bmd_data_arr[pmic_index];

	if (NULL == bmd_ptr)
    {
        pm_malloc( sizeof(pm_bmd_data_type), (void**)&bmd_ptr);
                                                    
        /* Assign Comm ptr */
        bmd_ptr->comm_ptr = comm_ptr;
		
        bmd_ptr->bmd_reg_table = bmd_reg;
        CORE_VERIFY_PTR(bmd_ptr->bmd_reg_table);
		
		bmd_ptr->num_of_peripherals = pm_target_information_get_periph_count_info(PM_PROP_BMD_NUM, pmic_index);
		CORE_VERIFY(bmd_ptr->num_of_peripherals != 0);

		pm_bmd_data_arr[pmic_index] = bmd_ptr;
    }
}


pm_bmd_data_type* pm_bmd_get_data(uint8 pmic_index)
{
    return (pmic_index < PM_MAX_NUM_PMICS) ? pm_bmd_data_arr[pmic_index] : NULL;
}

