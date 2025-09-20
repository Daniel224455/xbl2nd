/*! \file pm_haptics_driver.c 
*  \n
*  \brief This file contains Haptics peripheral driver initialization during which the driver data is stored.
*  \n
*  \n &copy; Copyright 2012-2019 QUALCOMM Technologies Incorporated, All Rights Reserved
*/

/* =======================================================================
                             Edit History
  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.


when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
02/14/19   pxm      Add auto_res_ctrl
11/12/17   aab     Moved common Register address defination to driver file  
01/21/15   al      New file 
========================================================================== */

/*===========================================================================

                     INCLUDE FILES 

===========================================================================*/
#include "pm_haptics_driver.h"
#include "CoreVerify.h"
#include "pm_comm.h"
#include "pm_utils.h"
/*===========================================================================

                        STATIC VARIABLES 

===========================================================================*/

/* Static global variable to store the HAPTICS driver data */
static pm_haptics_data_type *pm_haptics_data_arr[PM_MAX_NUM_PMICS];

static pm_haptics_register_info_type haptics_reg =
{                  
  .base_address  = 0xC000,
  .status1       = 0x00A,
  .auto_res_lo   = 0x00B,
  .auto_res_hi   = 0x00C,
  .en_ctl1       = 0x046,
  .en_ctl2       = 0x048,
  .en_ctl3       = 0x04A,
  .auto_res_ctrl = 0x04B,
  .actuator_type = 0x04C,
  .lra_res_type  = 0x04D,
  .sel           = 0x04E,
  .auto_res_cfg  = 0x04F,
  .slew_rate     = 0x050,
  .vmax_cfg      = 0x051,
  .rate_cfg1     = 0x054,
  .rate_cfg2     = 0x055,
  .internal_pwm  = 0x056,
  .external_pwm  = 0x057,
  .pwm_cap_sel   = 0x058,
  .brake_pattern = 0x05C,
  .wf_repeat     = 0x05E,
  .wf_s0         = 0x060,
  .play          = 0x070,
} ;


/*===========================================================================

                        FUNCTION DEFINITIONS

===========================================================================*/
void pm_haptics_driver_init(pm_comm_info_type *comm_ptr, peripheral_info_type *peripheral_info, uint32 pmic_index)
{
    pm_haptics_data_type *haptics_ptr = NULL;

    haptics_ptr = pm_haptics_data_arr[pmic_index];
    
    if (haptics_ptr == NULL)
    {
        pm_malloc( sizeof(pm_haptics_data_type), (void**)&haptics_ptr);
                                                    
        /* Assign Comm ptr */
        haptics_ptr->comm_ptr = comm_ptr;

        /* HAPTICS Register Info - Obtaining Data through dal config */
        haptics_ptr->haptics_reg_table = &haptics_reg;
        CORE_VERIFY_PTR(haptics_ptr->haptics_reg_table);

        //haptics_ptr->num_of_peripherals = (uint32)PM_MAX_NUM_HAPTICS;
        haptics_ptr->num_of_peripherals = pm_target_information_get_periph_count_info(PM_PROP_HAPTICS_NUM, pmic_index);
        CORE_VERIFY(haptics_ptr->num_of_peripherals  != 0);

        pm_haptics_data_arr[pmic_index] = haptics_ptr;
    }
}

pm_haptics_data_type* pm_haptics_get_data(uint32 pmic_index)
{
  if(pmic_index < PM_MAX_NUM_PMICS) 
  {
      return pm_haptics_data_arr[pmic_index];
  }

    return NULL;
}

uint32 pm_haptics_get_num_peripherals(uint32 pmic_index)
{
  if((pmic_index < PM_MAX_NUM_PMICS)&& 
  	  (pm_haptics_data_arr[pmic_index] !=NULL))
  {
      return pm_haptics_data_arr[pmic_index]->num_of_peripherals;
  }

  return 0;
}
