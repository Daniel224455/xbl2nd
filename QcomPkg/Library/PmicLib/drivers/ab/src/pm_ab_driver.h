#ifndef __PM_AB_DRIVER__H__
#define __PM_AB_DRIVER__H__

/*! \file pm_ab _driver.h
 *  \n
 *  \brief This file contains functions prototypes and variable/type/constant
*          declarations for supporting AB peripheral  
 *  \n  
 *  \n &copy;
 *  
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
/* register definitions                                                                     */
/************************************************************************/

typedef struct
{
    uint32                   base_address;   //0xDE00
    pm_register_address_type status1;        //0x08
    pm_register_address_type output_voltage; //0x41
    pm_register_address_type vout_default;   //0x44
    pm_register_address_type module_rdy;     //0x45
    pm_register_address_type enable_ctl;     //0x46
    pm_register_address_type pd_ctl;         //0x47
    pm_register_address_type ibb_en_rdy;     //0x49
    pm_register_address_type current_sense;  //0x4C
    pm_register_address_type ps_ctl;         //0x50
    pm_register_address_type precharge_ctl;  //0x5E
}pm_ab_register_info_type; 


typedef struct
{
    pm_comm_info_type            *comm_ptr;
    pm_ab_register_info_type     *ab_reg_table;
    uint8                        num_of_peripherals;
}pm_ab_data_type;


/*===========================================================================

                     FUNCTION DECLARATION 

===========================================================================*/
void pm_ab_driver_init(pm_comm_info_type *comm_ptr, peripheral_info_type *peripheral_info, uint8 pmic_index);

pm_ab_data_type* pm_ab_get_data(uint8 pmic_index);

uint8 pm_ab_get_num_peripherals(uint8 pmic_index);



#endif // __PM_AB_DRIVER__H__
