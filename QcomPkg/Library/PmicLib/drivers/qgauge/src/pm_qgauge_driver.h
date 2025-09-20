#ifndef __PM_QGAUGE_DRIVER_H__
#define __PM_QGAUGE_DRIVER_H__

/*! \file pm_qgauge_driver.h
 *  \n
 *  \brief This file contains functions prototypes and variable/type/constant
*          declarations for supporting qgauge peripheral  
 *  \n  
 *  \n &copy;
 *  Copyright (c) 2018 Qualcomm Technologies, Inc.  All Rights Reserved. 
 *  Qualcomm Technologies Proprietary and Confidential.
 */

/*===========================================================================

                        EDIT HISTORY

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.


when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
02/15/18   sv      Created
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
  uint32                   base_address;                  //0x4800
  pm_register_address_type perph_type                   ; //0x004
  pm_register_address_type perph_subtype                ; //0x005
  pm_register_address_type status1                      ; //0x008
  pm_register_address_type status2                      ; //0x009
  pm_register_address_type status3                      ; //0x00A
  pm_register_address_type int_rt_sts                   ; //0x010
  pm_register_address_type int_set_type                 ; //0x011
  pm_register_address_type int_polarity_high            ; //0x012
  pm_register_address_type int_polarity_low             ; //0x013
  pm_register_address_type int_latched_clr              ; //0x014
  pm_register_address_type int_en_set                   ; //0x015
  pm_register_address_type int_en_clr                   ; //0x016
  pm_register_address_type int_latched_sts              ; //0x018
  pm_register_address_type int_pending_sts              ; //0x019
  pm_register_address_type en_ctl                       ; //0x046
  pm_register_address_type vbat_empty_threshold         ; //0x04B
  pm_register_address_type vbat_low_threshold           ; //0x04c
  pm_register_address_type last_adc_v_data0             ; //0x0c0
  pm_register_address_type last_adc_v_data1             ; //0x0c1
  pm_register_address_type last_adc_i_data0             ; //0x0c2
  pm_register_address_type last_adc_i_data1             ; //0x0c3
}qgauge_register_ds;

typedef struct
{
    pm_comm_info_type          *comm_ptr;
    qgauge_register_ds         *qgauge_register;
    uint8                      num_of_peripherals;
}pm_qgauge_data_type;


/*===========================================================================

                     FUNCTION DECLARATION

===========================================================================*/
void pm_qgauge_driver_init(pm_comm_info_type *comm_ptr, peripheral_info_type *peripheral_info, uint8 pmic_index);

pm_qgauge_data_type* pm_qgauge_get_data(uint8 pmic_index);

uint8 pm_qgauge_get_dig_major(void);

void pm_qgauge_get_pmic_info(uint8 *pmic_index, uint8 *slave_id);

#endif // __PM_QGAUGE_DRIVER_H__
