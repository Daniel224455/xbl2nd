#ifndef __PM_SDAM_DRIVER_H__
#define __PM_SDAM_DRIVER_H__

/*! \file pm_sdam_driver.h
*  \n
*  \brief PBS Client driver type definitions and init function prototype.
*  \n
*  \n &copy; Copyright 2018 QUALCOMM Technologies Incorporated, All Rights Reserved
*/
/* =======================================================================
                                Edit History
This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.


when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/24/18   rl      Created.
========================================================================== */
/*===========================================================================

                     INCLUDE FILES

===========================================================================*/
#include "pm_err_flags.h"
#include "pm_target_information.h"
#include "DALSys.h"
#include "pm_sdam.h"
#include "pm_config_target.h"

/*===========================================================================

                        TYPE DEFINITIONS

===========================================================================*/
typedef struct
{
    pm_register_address_type  base_address;
    pm_register_address_type  peripheral_offset;
    pm_register_address_type  mem_start;
    pm_register_address_type  trig_arg;
    pm_register_address_type  trig_busy_ret;
    pm_register_address_type  sdam_size;
    pm_register_address_type  trig_set;
    pm_register_address_type  trig_clr;
}pm_sdam_reg_info_type;

typedef struct
{
  /*Comm info*/
  pm_comm_info_type  *comm_ptr;

  /*Register Table*/
  pm_sdam_reg_info_type*  sdam_reg_table;

  /*Number of Peripherals*/
  uint8  num_of_periph;

  /*Array for size of memory on each peripheral*/
  uint32  *sdam_mem_size;

  /*Dal Handle for Mutex*/
  DALSYSSyncHandle  sdam_dal_sync_handle;

}pm_sdam_data_type;

/*===========================================================================

                        FUNCTION PROTOTYPES

===========================================================================*/
void 
pm_sdam_driver_init(pm_comm_info_type *comm_ptr, 
                    peripheral_info_type *peripheral_info,
                    uint8 pmic_index);

pm_sdam_data_type* pm_sdam_get_data(uint8 pmic_index);

pm_err_flag_type pm_sdam_driver_self_init(void);


#endif /* __PM_SDAM_DRIVER_H__ */

