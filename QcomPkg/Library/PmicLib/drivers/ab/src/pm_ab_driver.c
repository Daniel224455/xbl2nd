/*! \file
*  
*  \brief  pm_ab_driver.c driver implementation.
*  \details charger driver implementation.
*  &copy;
*  Copyright (c) 2018 Qualcomm Technologies, Inc.  All Rights Reserved. 
*  Qualcomm Technologies Proprietary and Confidential.
*/

/*===========================================================================

EDIT HISTORY FOR MODULE


when        who          what, where, why
--------    ---          ---------------------------------------------------------- 
02/06/18     al        Initial revision
========================================================================== */

/*===========================================================================

                    INCLUDE FILES

===========================================================================*/
#include "pm_ab_driver.h"
#include "CoreVerify.h"
#include "device_info.h"



/*===========================================================================

                        STATIC VARIABLES 

===========================================================================*/

/* Static global variable to store the IBB data */
static pm_ab_data_type *pm_ab_data_arr[PM_MAX_NUM_PMICS];

static pm_ab_register_info_type  ab_reg[1]=
{
   {0xDE00, 0x08, 0x41, 0x44, 0x45, 0x46, 0x47, 0x49, 0x4C, 0x50, 0x5E}
};



/*===========================================================================

                        FUNCTION DEFINITIONS

===========================================================================*/
void pm_ab_driver_init(pm_comm_info_type *comm_ptr, peripheral_info_type *peripheral_info, uint8 pmic_index)
{
  pm_ab_data_type *ab_ptr = NULL;

  ab_ptr = pm_ab_data_arr[pmic_index];

  if (NULL == ab_ptr)
  {
    pm_malloc( sizeof(pm_ab_data_type), (void**)&ab_ptr);
                                                
    /* Assign Comm ptr */
    ab_ptr->comm_ptr = comm_ptr;
    
    /* IBB Register Info - Obtaining Data through dal config */
    ab_ptr->ab_reg_table = ab_reg;
    CORE_VERIFY_PTR(ab_ptr->ab_reg_table);
    
    ab_ptr->num_of_peripherals = pm_target_information_get_periph_count_info(PM_PROP_AB_NUM, pmic_index);
    CORE_VERIFY(ab_ptr->num_of_peripherals  != 0);

    pm_ab_data_arr[pmic_index] = ab_ptr;
  }
}


pm_ab_data_type* pm_ab_get_data(uint8 pmic_index)
{
  if(pmic_index < PM_MAX_NUM_PMICS)
  {
    return pm_ab_data_arr[pmic_index];
  }
  return NULL;
}

uint8 pm_ab_get_num_peripherals(uint8 pmic_index)
{
  if((pmic_index < PM_MAX_NUM_PMICS) &&
     (pm_ab_data_arr[pmic_index] != NULL))
  {
    return pm_ab_data_arr[pmic_index]->num_of_peripherals;
  }

  return 0;
}


