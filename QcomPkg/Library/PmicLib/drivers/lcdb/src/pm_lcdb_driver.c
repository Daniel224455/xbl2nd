/*! \file
*  
*  \brief  pm_lcdb_driver.c driver implementation.
*  \details charger driver implementation.
*  &copy;
*  Copyright (c) 2018 Qualcomm Technologies, Inc.  All Rights Reserved. 
*  Qualcomm Technologies Proprietary and Confidential.
*/

/*===========================================================================

EDIT HISTORY FOR MODULE


when        who        what, where, why
--------    ---        ---------------------------------------------------------- 
02/13/18    al         Initial version. 
========================================================================== */

/*===========================================================================

          INCLUDE FILES

===========================================================================*/
#include "pm_lcdb_driver.h"
#include "CoreVerify.h"
#include "device_info.h"
#include "pm_utils.h"

/*===========================================================================

                        STATIC VARIABLES 

===========================================================================*/

/* Static global variable to store the LCDB data */
static pm_lcdb_data_type *pm_lcdb_data_arr[PM_MAX_NUM_PMICS];

static pm_lcdb_register_info_type  lcdb_reg =
{
  .base_address       = 0xEC00,
  .status1            = 0x008,
  .status2            = 0x009,
  .status3            = 0x00A,
  .bst_output_voltage = 0x041,
  .hwen_ctl           = 0x044,
  .module_rdy         = 0x045,
  .enable_ctl1        = 0x046,
  .pd_ctl             = 0x047,
  .ldo_output_voltage = 0x071,
  .ncp_output_voltage = 0x081,
}; 






/*===========================================================================

                        FUNCTION DEFINITIONS

===========================================================================*/
void pm_lcdb_driver_init(pm_comm_info_type *comm_ptr, peripheral_info_type *peripheral_info, uint8 pmic_index)
{
  pm_lcdb_data_type *lcdb_ptr = NULL;

    lcdb_ptr = pm_lcdb_data_arr[pmic_index];

    if (NULL == lcdb_ptr)
    {
        pm_malloc( sizeof(pm_lcdb_data_type), (void**)&lcdb_ptr);
                                                    
        /* Assign Comm ptr */
        lcdb_ptr->comm_ptr = comm_ptr;
    
    /* LCDB Register Info - Obtaining Data through dal config */
        lcdb_ptr->lcdb_reg_table = &lcdb_reg;
        CORE_VERIFY_PTR(lcdb_ptr->lcdb_reg_table);
    
        lcdb_ptr->num_of_peripherals = pm_target_information_get_periph_count_info(PM_PROP_LCDB_NUM, pmic_index);
        CORE_VERIFY(lcdb_ptr->num_of_peripherals  != 0);

        pm_lcdb_data_arr[pmic_index] = lcdb_ptr;
    }
}


pm_lcdb_data_type* pm_lcdb_get_data(uint8 pmic_index)
{
    if(pmic_index < PM_MAX_NUM_PMICS)
    {
        return pm_lcdb_data_arr[pmic_index];
    }
    return NULL;
}

uint8 pm_lcdb_get_num_peripherals(uint8 pmic_index)
{
  if((pmic_index < PM_MAX_NUM_PMICS) && 
     (pm_lcdb_data_arr[pmic_index] !=NULL))
  {
      return pm_lcdb_data_arr[pmic_index]->num_of_peripherals;
  }

  return 0;
}


