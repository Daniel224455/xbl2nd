/*! \file pm_gpio_driver.c
 *  \n
 *  \brief  
 *  \details  
 *  \n &copy; Copyright 2012-2017 QUALCOMM Technologies Incorporated, All Rights Reserved
 */

/* =======================================================================
                             Edit History
  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.


when       who     what, where, why
--------   ---     ----------------------------------------------------------
11/12/17   aab     Moved common Register address defination to driver file  
07/16/14   akm     Comm change Updates
03/31/14   akm     Updated to the latest PMIC driver architecture   
03/25/13   aab     Creation
========================================================================== */

/*===========================================================================

                     INCLUDE FILES 

===========================================================================*/
#include "pm_gpio_driver.h"
#include "CoreVerify.h"
#include "pm_target_information.h"
#include "pm_version.h"
#include "device_info.h"
#include "pm_utils.h"
#include "hw_module_type.h"
/*===========================================================================

                        STATIC VARIABLES 

===========================================================================*/

/* Static global variable to store the GPIO data */
static pm_gpio_data_type *pm_gpio_data_arr[PM_MAX_NUM_PMICS];

static pm_gpio_register_info_type gpio_reg[1] =
{
  { 0xC000, 0x100, 0x008, 0x010, 0x011, 0x012, 0x013, 0x014, 0x015, 0x016, 0x018, 0x019, 0x01A, 0x01B, 0x040, 0x042, 0x043, 0x045, 0x041, 0x046, 0x044 }
};

/*===========================================================================

                        FUNCTION DEFINITIONS

===========================================================================*/
void pm_gpio_driver_init(pm_comm_info_type *comm_ptr, peripheral_info_type *peripheral_info, uint8 pmic_index)
{
    pm_gpio_data_type *gpio_ptr = NULL;
   
    gpio_ptr = pm_gpio_data_arr[pmic_index];

    
	if (gpio_ptr == NULL)
	{
	pm_malloc( sizeof(pm_gpio_data_type), (void**)&gpio_ptr); 
												
	/* Assign Comm ptr */
	gpio_ptr->comm_ptr = comm_ptr;
	
        /* LDO Register Info - Obtaining Data through dal config */
    gpio_ptr->gpio_register = gpio_reg;
        
        CORE_VERIFY_PTR(gpio_ptr->gpio_register);

        gpio_ptr->num_of_peripherals = pm_target_information_get_periph_count_info(PM_PROP_GPIO_NUM, pmic_index);
		
	if((peripheral_info->peripheral_type==PM_HW_MODULE_GPIO) && 
		((peripheral_info->peripheral_subtype>=PM_HW_MODULE_GPIO_GPIOLV) || 
		(peripheral_info->peripheral_subtype==PM_HW_MODULE_GPIO_GPIOMV)))
           gpio_ptr->is_periph_unified_reg=1;
	else
           gpio_ptr->is_periph_unified_reg=0;
        
        /* Num of peripherals cannot be 0 if this driver init gets called */
        CORE_VERIFY(gpio_ptr->num_of_peripherals  != 0);
	
	pm_gpio_data_arr[pmic_index] = gpio_ptr;
    }
}

pm_gpio_data_type* pm_gpio_get_data(uint8 pmic_index)
{
  if(pmic_index < PM_MAX_NUM_PMICS)
  {
      return pm_gpio_data_arr[pmic_index];
  }

  return NULL;
}

uint8 pm_gpio_get_num_peripherals(uint8 pmic_index)
{
  if((pmic_index < PM_MAX_NUM_PMICS) && 
  	  (pm_gpio_data_arr[pmic_index] != NULL))
  {
      return pm_gpio_data_arr[pmic_index]->num_of_peripherals;
  }

  return 0;
}
