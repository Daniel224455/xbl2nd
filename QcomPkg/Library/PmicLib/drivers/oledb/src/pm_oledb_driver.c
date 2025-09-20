/*! \file
*  
*  \brief  pm_oledb_driver.c driver implementation.
*  \details charger driver implementation.
*  &copy;
*  
*  Copyright (c) 2018 Qualcomm Technologies, Inc.  All Rights Reserved.
*  Qualcomm Technologies Proprietary and Confidential.
*/

/*===========================================================================

EDIT HISTORY FOR MODULE


when        who        what, where, why
--------    ---        ---------------------------------------------------------- 
02/06/18     al        Initial revision
========================================================================== */

/*===========================================================================

          INCLUDE FILES

===========================================================================*/
#include "pm_oledb_driver.h"
#include "CoreVerify.h"
#include "device_info.h"
#include "pm_utils.h"

/*===========================================================================

                        STATIC VARIABLES 

===========================================================================*/

/* Static global variable to store the OLEDB data */
static pm_oledb_data_type *pm_oledb_data_arr[PM_MAX_NUM_PMICS];

static pm_oledb_register_info_type  oledb_reg[1] =
{
  {
    .base_address     = 0xE000,
    .module_rdy       = 0x045,
    .module_enable    = 0x046,
    .ext_pin_ctl      = 0x047,
    .mode_config      = 0x048,
    .vout_pgm         = 0x049,
    .vout_default_ctl = 0x04A,
  }
};


/*===========================================================================

                        FUNCTION DEFINITIONS

===========================================================================*/
void pm_oledb_driver_init(pm_comm_info_type *comm_ptr, peripheral_info_type *peripheral_info, uint8 pmic_index)
{
  pm_oledb_data_type *oledb_ptr = NULL;

  oledb_ptr = pm_oledb_data_arr[pmic_index];

  if(NULL == oledb_ptr)
  {
    pm_malloc(sizeof(pm_oledb_data_type), (void **)&oledb_ptr);

    /* Assign Comm ptr */
    oledb_ptr->comm_ptr = comm_ptr;

    /* OLEDB Register Info - Obtaining Data through dal config */
    oledb_ptr->oledb_reg_table = oledb_reg;
    CORE_VERIFY_PTR(oledb_ptr->oledb_reg_table);

    oledb_ptr->num_of_peripherals = pm_target_information_get_periph_count_info(PM_PROP_OLEDB_NUM, pmic_index);
    CORE_VERIFY(oledb_ptr->num_of_peripherals  != 0);

    pm_oledb_data_arr[pmic_index] = oledb_ptr;
  }
}


pm_oledb_data_type* pm_oledb_get_data(uint8 pmic_index)
{
  if(pmic_index < PM_MAX_NUM_PMICS)
  {
    return pm_oledb_data_arr[pmic_index];
  }
  return NULL;
}

uint8 pm_oledb_get_num_peripherals(uint8 pmic_index)
{
  if((pmic_index < PM_MAX_NUM_PMICS) &&
     (pm_oledb_data_arr[pmic_index] != NULL))
  {
    return pm_oledb_data_arr[pmic_index]->num_of_peripherals;
  }

  return 0;
}


