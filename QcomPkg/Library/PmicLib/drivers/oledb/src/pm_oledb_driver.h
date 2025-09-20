#ifndef PM_OLEDB_DRIVER__H
#define PM_OLEDB_DRIVER__H

/*! \file pm_oledb _driver.h
 *  \n
 *  \brief This file contains functions prototypes and variable/type/constant
*          declarations for supporting OLEDB peripheral  
 *  \n  
 *  \n &copy;
 *  Copyright (c) 2018 Qualcomm Technologies, Inc.  All Rights Reserved. 
 *  Qualcomm Technologies Proprietary and Confidential.
 */

/* =======================================================================
                             Edit History
  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.


when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
02/06/18     al        Initial revision
========================================================================== */
/*===========================================================================

                     INCLUDE FILES 

===========================================================================*/
#include "pm_target_information.h"

/*===========================================================================

                     STRUCTURE TYPE AND ENUM

===========================================================================*/


/************************************************************************/
/* register definitions                                                 */
/************************************************************************/

typedef struct
{
    uint32                      base_address;                        //0xE000
    pm_register_address_type    module_rdy;                          //0x045
    pm_register_address_type    module_enable;                       //0x046
    pm_register_address_type    ext_pin_ctl;                         //0x047
    pm_register_address_type    mode_config;                         //0x048
    pm_register_address_type    vout_pgm;                            //0x049
    pm_register_address_type    vout_default_ctl;                    //0x04A
}pm_oledb_register_info_type; 


typedef struct
{
    pm_comm_info_type            *comm_ptr;
    pm_oledb_register_info_type  *oledb_reg_table;
    uint8                        num_of_peripherals;
}pm_oledb_data_type;


/*===========================================================================

                     FUNCTION DECLARATION 

===========================================================================*/
void pm_oledb_driver_init(pm_comm_info_type *comm_ptr, peripheral_info_type *peripheral_info, uint8 pmic_index);

pm_oledb_data_type* pm_oledb_get_data(uint8 pmic_index);

uint8 pm_oledb_get_num_peripherals(uint8 pmic_index);

#endif // PM_OLEDB_DRIVER__H
