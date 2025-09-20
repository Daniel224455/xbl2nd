#ifndef __PM_AB_H__
#define __PM_AB_H__

/*! \file
*  \n
*  \brief  pm_ab.h AB MODULE RELATED DECLARATION 
*  \details  This header file contains functions and variable declarations 
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
02/06/18   al      Initial version. 
========================================================================== */
#include "com_dtypes.h"
#include "pm_err_flags.h"
#include "pm_resources_and_types.h"

/*===========================================================================

                        TYPE DEFINITIONS 

===========================================================================*/

typedef enum
{
  PM_AB_ISENSE_0P5,   /*0.5 ibb current sense */
  PM_AB_ISENSE_0P8,   /*0.8 ibb current sense*/
  PM_AB_ISENSE_0P9,   /*0.9 ibb current sense*/
  PM_AB_ISENSE_1P0,   /*1.0 ibb current sense*/
  PM_AB_ISENSE_1P1,   /*1.1 ibb current sense*/
  PM_AB_ISENSE_1P5,   /*1.5 ibb current sense*/
  PM_AB_ISENSE_2P0,   /*2.0 ibb current sense*/
  PM_AB_ISENSE_INVALID,
}pm_ab_isense_type; 


typedef struct
{
   boolean ab_module_rdy;        //ready to be enabled
   boolean ab_module_en;         //module enabled or disabled
   boolean ab_ibb_en_rdy;        //allow IBB module to enable ab
   boolean en_fast_precharge;     //allow fast precharge
   uint32  max_prechg_time_usec;  //maximum pre_charge time
   uint32  output_volt;           //output voltage set
   boolean ab_vreg_ok;           //AB VREG OK  
   boolean short_circuit;         //TRUE: no short circuit fault. FALSE: short circuit 
   boolean ab_status_en;         //TRUE: Current AB status enabled; FALSE: Current AB status disable
   pm_ab_isense_type isense_tap; //current sense tapping
   pm_ab_isense_type isense_gain; //current sense gain
   uint32 ps_threshold;            //threshold for when pulse-skip mode is entered
   boolean select_ps_table;        //select PS table
   boolean select_digital_ps;      //select digital PS
   boolean enable_ps;              //enable pulse
}pm_ab_status_type;




/*===========================================================================

                 AB DRIVER FUNCTION PROTOTYPES

===========================================================================*/



 /**
 * @brief This function configures module to be enabled or not enabled
 * 
 * @details
 *  This API configures module to be enabled or not enabled
 * 
 * @param[in] pmic_device_index.  Primary: 0 Secondary: 1
 * @param[in] ab_rdy             TRUE if ready to be enabled else FALSE
 *       
 *                                
 *
 * @return  pm_err_flag_type 
 *          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not avaiable on this
 *          version of the PMIC.
 *          PM_ERR_FLAG_SUCCESS               = SUCCESS.
 *
 */

pm_err_flag_type pm_ab_module_ready_to_enable(uint32 device_index, boolean ab_rdy);


 /**
 * @brief This function enables or disabled AB module
 * 
 * @details
 *  This API enables or disabled AB module
 * 
 * @param[in] pmic_device_index.  Primary: 0 Secondary: 1
 * @param[in] en_module           TRUE allows IBB module to enable AB and FALSE won't
 *       
 *                                
 *
 * @return  pm_err_flag_type 
 *          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not avaiable on this
 *          version of the PMIC.
 *          PM_ERR_FLAG_SUCCESS               = SUCCESS.
 *
 */
pm_err_flag_type pm_ab_en_module(uint32 device_index, boolean en_module);


/**
 * @brief This function sets AB to be enabled by IBB or not
 * 
 * @details
 *  This API allows AB to be enabled by IBB or not
 * 
 * @param[in] pmic_device_index.  Primary: 0 Secondary: 1
 * @param[in] ibb_en_rdy          TRUE allows IBB module to enable AB and FALSE won't 
 *       
 *                                
 *
 * @return  pm_err_flag_type 
 *          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not avaiable on this
 *          version of the PMIC.
 *          PM_ERR_FLAG_SUCCESS               = SUCCESS.
 *
 */
pm_err_flag_type pm_ab_ibb_rdy_en(uint32 device_index, boolean ibb_rdy_en);


 /**
 * @brief This function configures precharge mode and sets max precharge time
 * 
 * @details
 *  This API configures precharge mode and sets max precharge time
 * 
 * @param[in] pmic_device_index.  Primary: 0 Secondary: 1
 * @param[in] fast_precharge     TRUE configures the fast_precharge and FLASE slow_precharge 
 * @param[in] max_time_usec      Max pre-charge time. Valid range is 200 to 500usec
 *       
 *                                
 *
 * @return  pm_err_flag_type 
 *          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not avaiable on this
 *          version of the PMIC.
 *          PM_ERR_FLAG_SUCCESS               = SUCCESS.
 *
 */
pm_err_flag_type pm_ab_config_precharge_ctrl(uint32 device_index, boolean fast_precharge, uint32 max_time_usec);


 /**
 * @brief This reads the AB module setting status
 * 
 * @details
 *  This API reads the AB module setting status
 * 
 * @param[in] pmic_device_index.  Primary: 0 Secondary: 1
 * @param[in] ab_status          Refer struct #pm_ab_status_type for more info
 *       
 *                                
 *
 * @return  pm_err_flag_type 
 *          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not avaiable on this
 *          version of the PMIC.
 *          PM_ERR_FLAG_SUCCESS               = SUCCESS.
 *
 */
pm_err_flag_type pm_ab_get_ab_status(uint32 device_index, pm_ab_status_type* ab_status);

 /**
 * @brief This function allows to select default voltage or set 
 *        output voltage for Vdisp
 *  
 * @details
 *  This API allows to select default voltage or set output 
 *  voltage for +Vdisp
 * 
 * @param[in] pmic_device_index.     Primary: 0 Secondary: 1
 * @param[in] override_output_volt   TRUE sets Output voltage given by SET_OUTPUT_VOLTAGE field otherwise default
 * @param[in] volt_millivolt         Output voltage. Range is 4600 to 6100 milli volt.
 *                                
 *
 * @return  pm_err_flag_type 
 *          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not avaiable on this
 *          version of the PMIC.
 *          PM_ERR_FLAG_SUCCESS               = SUCCESS.
 *
 */
pm_err_flag_type pm_ab_config_output_volt(uint32 device_index, boolean override_output_volt, uint32 volt_millivolt);

/**
 * @brief This configures AB current sense 
 * 
 * @details
 *  This API configures current sense tapping point and current
 *  sense gain
 * 
 * @param[in] pmic_device_index.  Primary: 0 Secondary: 1
 * @param[in] isense_tap          : Current sense tapping point. Refer #pm_ab_isense_type for more info
 * @param[in] isense_gain         : Current sense gain. Refer #pm_ab_isense_type for more info
 *       
 *                                
 *
 * @return  pm_err_flag_type 
 *          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not avaiable on this
 *          version of the PMIC.
 *          PM_ERR_FLAG_SUCCESS               = SUCCESS.
 *
 */
pm_err_flag_type pm_ab_config_current_sense(uint32 device_index, pm_ab_isense_type isense_tap, pm_ab_isense_type isense_gain);


/**
 * @brief This configures AB pulse skipping parameters  
 * 
 * @details
 *  This API configures pulse skipping parameters
 * 
 * @param[in] pmic_device_index.  Primary: 0 Secondary: 1
 * @param[in] ps_threshold      : Threshold when pulse skipping mode is entered. Range is 50 to 80 mAmps
 * @param[in] enable_ps         : TRUE to enable PS, otherwise FALSE to disable it
 *       
 *                                
 *
 * @return  pm_err_flag_type 
 *          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not avaiable on this
 *          version of the PMIC.
 *          PM_ERR_FLAG_SUCCESS               = SUCCESS.
 *
 */
pm_err_flag_type 
pm_ab_config_pulse_skip_ctrl(uint32 device_index, uint32 ps_threshold, boolean enable_ps);


/**
 * @brief This configures AB default voltage output
 * 
 * @details
 *  This API configures AB default voltag output
 * 
 * @param[in] device_index        : Primary: 0, Secondary: 1 etc
 * @param[in] milli_volt          : Default voltage value to be programmed in
 *                                  milli-volts.
 * @return  pm_err_flag_type 
 *          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not available on this
 *          version of the PMIC.
 *          PM_ERR_FLAG_SUCCESS               = SUCCESS.
 */
pm_err_flag_type
pm_ab_config_default_volt(uint32 device_index, uint32 milli_volt);


#endif /* __PM_AB_H__*/

