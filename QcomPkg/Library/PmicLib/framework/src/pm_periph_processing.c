/*! \file pm_periph_processing.c
*  
*  \brief This file contains APIs common to loader and core
*  \n   
*  &copy; Copyright 2018 QUALCOMM Technologies Incorporated, All Rights Reserved
*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This document is created by a code generator, therefore this section will
  not contain comments describing changes made to the module.


when       	who     	what, where, why
--------   	---    		---------------------------------------------------------- 
04/04/18    al          New file
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES 

===========================================================================*/
#include "pm_target_information.h"
#include "DALSys.h"
#include "pm_comm.h"
#include "device_info.h"
#include "CoreVerify.h"


/*===========================================================================

                        STATIC VARIABLES 

===========================================================================*/
#define LOCK_BY_BIT_REG_OFFSET_D1 0x0D1


/*===========================================================================

                     FUNCTION DEFINITIONS

===========================================================================*/

pm_err_flag_type
pm_config_lock_by_bit(void)
{
  pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
  uint32 i = 0;
  uint32 slave_id = 0;
  uint16 addr = 0;

  uint32 current_img = pm_get_img_type();

  pm_lock_by_bit_info *lock_by_bit_data_ptr = (pm_lock_by_bit_info *)pm_target_information_get_specific_info(PM_PROP_LOCK_BY_BIT_DATA);

  uint32 *lock_by_bit_arr_size = (uint32 *)pm_target_information_get_specific_info(PM_PROP_LOCK_BY_BIT_ARR_SIZE);

  if((lock_by_bit_data_ptr == NULL) || (lock_by_bit_arr_size == NULL))
  {
    return PM_ERR_FLAG_INVALID_CONFIG_SETTING;
  }

  for(i = 0; i < *lock_by_bit_arr_size; i++)
  {
    if(lock_by_bit_data_ptr[i].lock_owner == current_img)
    {
      slave_id = lock_by_bit_data_ptr[i].slave_id;

      if(pm_is_slave_id_supported(slave_id) == TRUE) //write only if valid slave id 
      {
        addr = (lock_by_bit_data_ptr[i].ppid * 0x100) + LOCK_BY_BIT_REG_OFFSET_D1;

        err_flag |= pm_comm_write_byte(slave_id, addr, lock_by_bit_data_ptr[i].lock_value, 0);
      }
    }
  }

  return err_flag;
}

