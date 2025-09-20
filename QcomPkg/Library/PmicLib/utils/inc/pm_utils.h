#ifndef __PM_UTILS_H__
#define __PM_UTILS_H__

/*! \file
 *  
 *  \brief  pm_utils.h ----This file contain PMIC wrapper function of DALSYS_Malloc()
 *  \details his file contain PMIC wrapper function of DALSYS_Malloc()
 *  
 *    &copy; Copyright 2012-2018 Qualcomm Technologies Incorporated, All Rights Reserved
 */

/*===========================================================================

                EDIT HISTORY FOR MODULE

  This document is created by a code generator, therefore this section will
  not contain comments describing changes made to the module over time.


when       who     what, where, why
--------   ---     ----------------------------------------------------------
12/18/17   aab     Depricated pm_convert_time_to_timetick() and  pm_convert_timetick_to_time(), Since it assumes a fixed Clock speed 
12/05/17   aab     Added support to log total number of mem allocated
07/21/17   czq     Use Spare reg PON_PERPH_RB_SPARE instead of PON_DVDD_RB_SPARE
03/20/14   aab     Added pm_boot_adc_get_mv()
06/11/13   hs      Adding settling time for regulators.
06/20/12   hs      Created

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES 

===========================================================================*/

#include "com_dtypes.h"
#include "pm_err_flags.h"
#include "DDIAdc.h"

#define PON_PERPH_RB_SPARE_DEVICE_INIT_MASK      0x10   //PERPH SPARE 4 for device init 
#define PON_PERPH_RB_SPARE_DRIVER_INIT_MASK      0x20   //PERPH SPARE 5 for driver init 

typedef enum
{
    PM_BATT_ID_PU_30K,
    PM_BATT_ID_PU_100K,
    PM_BATT_ID_PU_400K,
    PM_BATT_ID_PU_INVALID
} pm_batt_id_pull_up_type;

/*===========================================================================

                        FUNCTION PROTOTYPES

===========================================================================*/
extern void pm_malloc(uint32 dwSize, void **ppMem);

pm_err_flag_type
pm_busy_wait ( uint32 uS );

uint8
pm_utils_get_upper_idx(uint32 data_value, uint32 *data_list, uint8 list_count);

uint32 
pm_get_mem_allocated(void);

pm_err_flag_type 
pm_sbl_get_batt_id(pm_batt_id_pull_up_type type, uint32 *batt_id_ohms);

/*
 * @description: This function calculate ADC code for battery ID a given ID resistor value in the unit of ohm.
 *
 * @param type [in]: the pull up resistor type
 *        ohms [in]: the battery ID resistor which need to calculate ADC code
 *        code [out]: the adc code calculated for battery ID
 */
pm_err_flag_type
pm_get_adc_code_of_battery_id(pm_batt_id_pull_up_type type, uint32 ohms, uint32* code);

/* This API convert expected battery temperature to adc value which can be configured in afp thresholds */
pm_err_flag_type pm_get_adc_code_of_battery_temp(uint32 celsius, uint32* code);

/*This API configures the PMIC PS_HOLD register for normal shutdown and then drops the PS_HOLD low*/
pm_err_flag_type 
pm_device_shutdown(void);

//Check if the pm_driver_init is done
boolean 
pm_driver_init_status(void);

//check if pm_device_init is done
boolean 
pm_device_init_status(void);

pm_err_flag_type  
pm_sbl_daladc_fg_set_temp_threshold(uint8 pmic_index,  AdcFGThresholdType adc_fg_threshold, int temp_value); 

uint32 pm_get_img_type(void);

pm_err_flag_type 
pm_sbl_set_batt_id_forced_mode(boolean enable);

/*disable SPMI and PMIC interrupt*/
pm_err_flag_type 
pm_utils_irq_disable_clr_all(void);

/* slave ID is supported on the device or not */
boolean 
pm_is_slave_id_supported(uint8 slave_id);


#endif // __PM_UTILS_H__
