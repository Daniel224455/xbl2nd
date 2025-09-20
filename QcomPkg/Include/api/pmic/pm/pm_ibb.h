#ifndef __PM_IBB_H__
#define __PM_IBB_H__

/*! \file
*  \n
*  \brief  pm_uefi_ibb.h PMIC-IBB MODULE RELATED DECLARATION 
*  \details  This header file contains functions and variable declarations 
*  \n &copy;
*  Copyright (c) 2014-2017 Qualcomm Technologies, Inc.  All Rights Reserved. 
*  Qualcomm Technologies Proprietary and Confidential.
*/

/* =======================================================================
                                Edit History
This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.


when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
07/16/14   al      Add API to configure delay between LAB and IBB during pwrup and pwrdwn
05/20/14   al      Architecture update 
04/18/14   al      Updated copyright 
09/11/12   al      Initial version. 
========================================================================== */
#include "com_dtypes.h"
#include "pm_err_flags.h"
#include "pm_resources_and_types.h"

/*===========================================================================

                        TYPE DEFINITIONS 

===========================================================================*/
typedef enum
{
   PM_IBB_DELAY_AMNT_1MS,
   PM_IBB_DELAY_AMNT_2MS,
   PM_IBB_DELAY_AMNT_4MS,
   PM_IBB_DELAY_AMNT_8MS,
   PM_IBB_DELAY_AMNT_12MS,
   PM_IBB_DELAY_AMNT_16MS,
   PM_IBB_DELAY_AMNT_20MS,
   PM_IBB_DELAY_AMNT_24MS,
   PM_IBB_DELAY_AMNT_28MS,
   PM_IBB_DELAY_AMNT_32MS,
   PM_IBB_DELAY_AMNT_36MS,
   PM_IBB_DELAY_AMNT_40MS,
   PM_IBB_DELAY_AMNT_INVALID,	
}pm_ibb_dly_amnt_type;

typedef struct
{
  boolean                 ibb_module_rdy;      /*ready to be enabled                       */
  boolean                 ibb_module_en;       /*module enabled or disabled                */
  boolean                 swire_rdy;           /*swire_rdy                                 */
  pm_ibb_dly_amnt_type    pwrup_dly;           /*delay between IBB and AB while powering up */
  pm_ibb_dly_amnt_type    pwrdn_dly;           /*delay between IBB and AB while powering down */
  uint32                  output_volt;         /*output voltage set                        */
  uint32                  default_volt;     /*default voltage set                        */
  boolean                 ibb_vreg_ok;         /*IBB VREG OK                               */ 
  boolean                 overload;            /*IBB Module short circuit detect           */
  boolean                 ilimit_error;        /*current limit error                       */
}pm_ibb_status_type;

/*===========================================================================

                 SMBCHG OTG DRIVER FUNCTION PROTOTYPES

===========================================================================*/

 /**
 * @brief This function prepares module to be enabled or not
 * 
 * @details
 *  This API prepares module to be enabled or not.
 *  When this is set to low, the IBB module cannot be enabled 
 *  (powered-up) through S-Wire or IBB_ENABLE register
 * 
 * @param[in] pmic_device_index.  Primary: 0 Secondary: 1
 * @param[in] ibb_rdy             TRUE if ready to be enabled else FALSE
 *                                
 *
 * @return  pm_err_flag_type 
 *          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not available on this
 *          version of the PMIC.
 *          PM_ERR_FLAG_SUCCESS               = SUCCESS.
 *
 */
pm_err_flag_type pm_ibb_ibb_module_rdy(uint32 device_index, boolean ibb_rdy);


 /**
 * @brief This function enables/disables module and makes S-wire ready for listening
 * 
 * @details
 *  This API enables/disables module and makes S-wire ready for listening
 * 
 * @param[in] pmic_device_index.  Primary: 0 Secondary: 1
 * @param[in] en_module      TRUE enables module and false disables module
 * @param[in] swire_rdy      TRUE ready to liste to to S-wire and FALSE not ready to listen to S-Wire
 *                                
 *
 * @return  pm_err_flag_type 
 *          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not available on this
 *          version of the PMIC.
 *          PM_ERR_FLAG_SUCCESS               = SUCCESS.
 *
 */
pm_err_flag_type pm_ibb_config_ibb_ctrl(uint32 device_index, boolean en_module, boolean swire_rdy);


 /**
 * @brief This function allows to select default voltage and set 
 *        output voltage
 *  
 * @details
 *  This API allows to select default voltage and set output 
 *  voltage
 * 
 * @param[in] pmic_device_index.     Primary: 0 Secondary: 1
 * @param[in] milli_volt             Output voltage. Range is 800 to 5400 millivolt.
 *                                
 *
 * @return  pm_err_flag_type 
 *          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not available on this
 *          version of the PMIC.
 *          PM_ERR_FLAG_SUCCESS               = SUCCESS.
 *
 */
pm_err_flag_type pm_ibb_config_output_volt(uint32 device_index, int32 milli_volt);

 /**
 * @brief This function allows to select default voltage and set 
 *        output voltage
 *  
 * @details
 *  This API allows to select default voltage and set output 
 *  voltage
 * 
 * @param[in] pmic_device_index.     Primary: 0 Secondary: 1
 * @param[in] milli_volt             Output voltage. Range is 800 to 5400 millivolt.
 *                                
 *
 * @return  pm_err_flag_type 
 *          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not available on this
 *          version of the PMIC.
 *          PM_ERR_FLAG_SUCCESS               = SUCCESS.
 *
 */
pm_err_flag_type pm_ibb_config_default_volt(uint32 device_index, int32 milli_volt);


 /**
 * @brief This configures the delay for IBB module 
 * 
 * @details
 *  This API configures delay required between IBB and LAB vregs and also delay source
 * 
 * @param[in] pmic_device_index.  Primary: 0 Secondary: 1
 * @param[in] pwrup_dly           Delay during power up. Refer #pm_ibb_dly_amnt_type for more info. 
 * @param[in] pwrdn_dly           Delay during power up. Refer #pm_ibb_dly_amnt_type for more info.
 *                                
 *
 * @return  pm_err_flag_type 
 *          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not available on this
 *          version of the PMIC.
 *          PM_ERR_FLAG_SUCCESS               = SUCCESS.
 *
 */
pm_err_flag_type pm_ibb_config_pwrup_pwrdn_dly(uint32 device_index, pm_ibb_dly_amnt_type pwrup_dly, pm_ibb_dly_amnt_type pwrdn_dly);

 /**
 * @brief This reads the IBB module setting status
 * 
 * @details
 *  This API reads the IBB module setting status
 * 
 * @param[in] pmic_device_index.  Primary: 0 Secondary: 1
 * @param[in] ibb_status          Refer struct #pm_ibb_status_type for more info
 *       
 *                                
 *
 * @return  pm_err_flag_type 
 *          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not available on this
 *          version of the PMIC.
 *          PM_ERR_FLAG_SUCCESS               = SUCCESS.
 *
 */
 pm_err_flag_type pm_ibb_get_ibb_status(uint32 device_index, pm_ibb_status_type* ibb_status);
#endif /* __PM_IBB_H__*/

