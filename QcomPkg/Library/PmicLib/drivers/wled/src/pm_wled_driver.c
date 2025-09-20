/*! \file pm_wled_driver.c 
*  \n
*  \brief This file contains WLED peripheral driver initialization during which the driver data is stored.
*  \n
*  \n &copy; Copyright 2012-2017 QUALCOMM Technologies Incorporated, All Rights Reserved
*/

/* =======================================================================
                             Edit History
  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.


when        who     what, where, why
--------    ---     ----------------------------------------------------------
11/12/17    aab     Moved common Register address defination to driver file  
07/16/14    akm     Comm change Updates
03/31/14    akm     Updated to the latest PMIC driver architecture   
========================================================================== */

/*===========================================================================

                     INCLUDE FILES 

===========================================================================*/
#include "pm_wled_driver.h"
#include "CoreVerify.h"

/*===========================================================================

                        STATIC VARIABLES 

===========================================================================*/

/* Static global variable to store the WLED driver data */
static pm_wled_data_type *pm_wled_data_arr[PM_MAX_NUM_PMICS];

static pm_wled_ctrl_register_info_type wled_ctrl_reg[1] =
{
  {
    .base_address        = 0xD800,
    .perph_type          = 0x04,
    .perph_subtype       = 0x05,
    .fault_status        = 0x08,
    .int_rt_sts          = 0x10,
    .int_set_type        = 0x11,
    .int_polarity_high   = 0x12,
    .int_polarity_low    = 0x13,
    .int_latched_clr     = 0x14,
    .int_en_set          = 0x15,
    .int_en_clr          = 0x16,
    .int_latched_sts     = 0x18,
    .int_pending_sts     = 0x19,
    .int_mid_sel         = 0x1A,
    .int_priority        = 0x1B,
    .module_enable       = 0x46,
    .feedback_control    = 0x48,
    .switching_frequency = 0x4C,
    .wled_ovp            = 0x4D,
    .wled_ilim           = 0x4E,
    .sh_for_softstart    = 0x58,
    .test4               = 0xE5,
  }
};

static pm_wled_sink_register_info_type wled_sink_reg[1] =
{
  {
    .base_address                       = 0xD900,
    .perph_type                         = 0x04,
    .perph_subtype                      = 0x05,
    .current_sink_en                    = 0x46,
    .iled_sync_bit                      = 0x47,
    .modulatora_en                      = 0x50,
    .modulatora_src_sel                 = 0x51,
    .modulatora_brightness_width_sel    = 0x52,
    .modulatora_brightness_settings_lsb = 0x53,
    .modulatora_brightness_settings_msb = 0x54,
    .modulatorb_en                      = 0x60,
    .modulatorb_src_sel                 = 0x61,
    .modulatorb_brightness_width_sel    = 0x62,
    .modulatorb_brightness_settings_lsb = 0x63,
    .modulatorb_brightness_settings_msb = 0x64,
    .modulator_sync_bit                 = 0x65,
    .led1_full_scale_current            = 0x72,
    .led1_src_sel                       = 0x73,
    .led2_full_scale_current            = 0x82,
    .led2_src_sel                       = 0x83,
    .led3_full_scale_current            = 0x92,
    .led3_src_sel                       = 0x93,
    .led4_full_scale_current            = 0xA2,
    .led4_src_sel                       = 0xA3
  }
};

static pm_wled_register_info_type wled_reg[1] = 
{
    {
      wled_ctrl_reg,
      wled_sink_reg
    }
};
/*===========================================================================

                        FUNCTION DEFINITIONS

===========================================================================*/
void pm_wled_driver_init(pm_comm_info_type *comm_ptr, peripheral_info_type *peripheral_info, uint8 pmic_index)
{
    pm_wled_data_type *wled_ptr = NULL;

    wled_ptr = pm_wled_data_arr[pmic_index];

    if (NULL == wled_ptr)
    {
        pm_malloc( sizeof(pm_wled_data_type), (void**)&wled_ptr);
                                                    
        /* Assign Comm ptr */
        wled_ptr->comm_ptr = comm_ptr;

        /* WLED Register Info - Obtaining Data through dal config */
        wled_ptr->wled_reg_table = wled_reg;
        CORE_VERIFY_PTR(wled_ptr->wled_reg_table);

        //wled_ptr->num_of_peripherals = (uint32)PM_MAX_NUM_WLED;
        wled_ptr->num_of_peripherals = pm_target_information_get_periph_count_info(PM_PROP_WLED_NUM, pmic_index);
        CORE_VERIFY(wled_ptr->num_of_peripherals != 0);

        pm_wled_data_arr[pmic_index] = wled_ptr;
    }
}

pm_wled_data_type* pm_wled_get_data(uint8 pmic_index)
{
    if(pmic_index < PM_MAX_NUM_PMICS)
    {
        return pm_wled_data_arr[pmic_index];
    }

    return NULL;
}

uint8 pm_wled_get_num_peripherals(uint8 pmic_index)
{
  if((pmic_index < PM_MAX_NUM_PMICS) && 
     (pm_wled_data_arr[pmic_index] !=NULL))
    {
      return pm_wled_data_arr[pmic_index]->num_of_peripherals;
    }

    return 0;
}
