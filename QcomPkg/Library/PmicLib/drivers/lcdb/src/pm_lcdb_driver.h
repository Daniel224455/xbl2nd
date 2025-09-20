#ifndef PM_LCDB_DRIVER__H
#define PM_LCDB_DRIVER__H

/*! \file pm_lcdb _driver.h
 *  \n
 *  \brief This file contains functions prototypes and variable/type/constant
*          declarations for supporting SMBB peripheral  
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
02/13/18   al      Initial version. 
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
    uint32                      base_address;        //0xEC00
    pm_register_address_type    status1;             //0x008
    pm_register_address_type    status2;             //0x009
    pm_register_address_type    status3;             //0x00A
    pm_register_address_type    bst_output_voltage;  //0x041
    pm_register_address_type    hwen_ctl;            //0x044
    pm_register_address_type    module_rdy;          //0x045
    pm_register_address_type    enable_ctl1;         //0x046
    pm_register_address_type    pd_ctl;              //0x047
    pm_register_address_type    ldo_output_voltage;  //0x071
    pm_register_address_type    ncp_output_voltage;  //0x081
}pm_lcdb_register_info_type; 


typedef struct
{
    pm_comm_info_type            *comm_ptr;
    pm_lcdb_register_info_type   *lcdb_reg_table;
    uint8                        num_of_peripherals;
}pm_lcdb_data_type;


typedef enum
{
   PM_LCDB_BOOST_VOLT,
   PM_LCDB_LDO_VOLT,
   PM_LCDB_NCP_VOLT,
   PM_LCDB_INVALID_VOLT,
}pm_lcdb_volt_type;

/*===========================================================================

                     FUNCTION DECLARATION 

===========================================================================*/
void pm_lcdb_driver_init(pm_comm_info_type *comm_ptr, peripheral_info_type *peripheral_info, uint8 pmic_index);

pm_lcdb_data_type* pm_lcdb_get_data(uint8 pmic_index);

uint8 pm_lcdb_get_num_peripherals(uint8 pmic_index);

#endif // PM_LCDB_DRIVER__H
