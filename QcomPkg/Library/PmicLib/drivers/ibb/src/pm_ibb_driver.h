#ifndef PM_IBB_DRIVER__H
#define PM_IBB_DRIVER__H

/*! \file pm_ibb _driver.h
 *  \n
 *  \brief This file contains functions prototypes and variable/type/constant
*          declarations for supporting SMBB peripheral  
 *  \n  
 *  \n &copy;
 *  Copyright (c) 2013-2014 Qualcomm Technologies, Inc.  All Rights Reserved. 
 *  Qualcomm Technologies Proprietary and Confidential.
 */

/* =======================================================================
                             Edit History
  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.


when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
08/20/14   al      Updating comm lib 
04/30/14   al      Initial revision  
========================================================================== */
/*===========================================================================

                     INCLUDE FILES 

===========================================================================*/
#include "pm_target_information.h"

/*===========================================================================

                     STRUCTURE TYPE AND ENUM

===========================================================================*/


/************************************************************************/
/* register definitions                                                                     */
/************************************************************************/

typedef struct
{
    uint32                      base_address;                        //0xDC00
    pm_register_address_type    status1;                             //0x008
	pm_register_address_type    default_voltage;                     //0x040
    pm_register_address_type    output_voltage;                      //0x041
    pm_register_address_type    module_rdy;                          //0x045
    pm_register_address_type    enable_ctl;                          //0x046
    pm_register_address_type    pwrup_pwrdn_ctl_1;                   //0x058
	pm_register_address_type    pwrup_pwrdn_delay;                   //0x05A
}pm_ibb_register_info_type; 


typedef struct
{
    pm_comm_info_type            *comm_ptr;
    pm_ibb_register_info_type    *ibb_reg_table;
    uint8                        num_of_peripherals;
}pm_ibb_data_type;


/*===========================================================================

                     FUNCTION DECLARATION 

===========================================================================*/
void pm_ibb_driver_init(pm_comm_info_type *comm_ptr, peripheral_info_type *peripheral_info, uint8 pmic_index);

pm_ibb_data_type* pm_ibb_get_data(uint8 pmic_index);

uint8 pm_ibb_get_num_peripherals(uint8 pmic_index);

#endif // PM_IBB_DRIVER__H
