#ifndef __PM_FG_ADC_RR_H__
#define __PM_FG_ADC_RR_H__


/*! \file pm_adc_rr.h 
 *  \n
 *  \brief  PMIC-FG MODULE RELATED DECLARATION
 *  \details  This file contains functions and variable declarations to support 
 *   the PMIC FG Fule Gauge memory interface module.
 *
 *  Copyright (c) 2016-2018 Qualcomm Technologies, Inc.  All Rights Reserved. 
 *  Qualcomm Technologies Proprietary and Confidential.
*/

/*===========================================================================

EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.


when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
01/30/19   pbitra  Added support to set batt therm freq
08/01/18   ivy     Added API to set batt therm pull up to FG register
06/18/18   cs      turn off FG_BTM for batt_id after ID is successfully read to avoid adc corruption for 1.0
03/14/18   pxm     Update pm_fg_adc_rr_set_fake_batt_id_range and pm_fg_adc_rr_clear_channel_log
03/05/18   pxm     Remove unused APIs and registers.
01/29/18   cs      added function to get batt_id_adc, set_batt_therm_cfg1
09/19/17   cs      for SDM855 removed unsupported functions move batt_temp here.
03/15/17   cs      fixed BATT_ID_CTL_TYPE bit order
04/01/16   aab     Added more ADC RR APIs 
01/12/16   al      Initial Release 
===========================================================================*/

#include "com_dtypes.h"
#include "pm_err_flags.h"
#include "pm_resources_and_types.h"

/*===========================================================================
                        MACRO DEFINITIONS 
===========================================================================*/
#define PM_FG_ADC_RR_FG_CHARGER_SKIN_TEMP_MIN    -30
#define PM_FG_ADC_RR_FG_CHARGER_SKIN_TEMP_MAX    97
#define PM_FG_ADC_RR_FG_FAKE_BATT_MAX    14000

/*===========================================================================
                        TYPE DEFINITIONS 
===========================================================================*/
/* Common Interrupt Prototype */
typedef enum
{
  pm_FG_ADC_RR_IRQ_BT_MISS,       /*battery missing*/
  PM_FG_ADC_RR_IRQ_BT_ID,         /*battery ID conversion data available*/
  PM_FG_ADC_RR_IRQ_BT_TMPR_DELTA, /*battery temperature exceeded delta (change threshold)*/
  PM_FG_ADC_RR_IRQ_BT_TMPR_HOT,   /*battery temperature hot*/
  PM_FG_ADC_RR_IRQ_BT_TMPR_COLD,  /*battery temperature cold*/
  PM_FG_ADC_RR_IRQ_RR_ATTN,       /*pkMx[3]*/
  PM_FG_ADC_RR_IRQ_INVALID        /*Invalid adc_rr irq*/
} pm_fg_adc_rr_irq_type;


typedef enum
{
  PM_FG_ADC_RR_BATT_ID_HI_BIAS, //400K PULL UP
  PM_FG_ADC_RR_BATT_ID_MED_BIAS, //100K PULL UP
  PM_FG_ADC_RR_BATT_ID_LOW_BIAS, //30K PULL UP
}pm_fg_adc_rr_batt_id_bias;

typedef enum
{
  PM_FG_BTM_LOW_THR_INT_EN,       //BIT 0
  PM_FG_BTM_HIGH_THR_INT_EN, //BIT 1
  PM_FG_BTM_MEAS_EN = 7, //BIT 7
}pm_fg_btm_en_1;

typedef enum
{
	PM_FG_THERM_RBIAS_INVALID,
	PM_FG_THERM_RBIAS_30K,
	PM_FG_THERM_RBIAS_100K,
	PM_FG_THERM_RBIAS_400K,
}pm_fg_batt_therm_pull_up_res;

/*===========================================================================
                        FUNCTION DEFINITIONS
===========================================================================*/

/**
* @brief This function returns irq status
* 
* @details
*  This function returns irq status
* 
* @param[in] pmic_device_index.  Primary: 0 Secondary: 1
* @param[in] pm_fg_adc_rr_irq_type        irq type
* @param[in] pm_irq_status_type        irq status type
* @param[in] boolean        irq TRUE/FALSE status
*
* @return  pm_err_flag_type 
*          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not available on this
*          version of the PMIC.
*          PM_ERR_FLAG_SUCCESS               = SUCCESS.
*
*/
pm_err_flag_type pm_fg_adc_rr_irq_status(uint32 device_index, pm_fg_adc_rr_irq_type irq, pm_irq_status_type type, boolean *status);



/**
* @brief 
* This function do sw sequence initiate
* 
* @details
* Initiate one sequence of the round robin ADC on demand
* 
* @param[in] pmic_device   : Primary: 0 Secondary: 1
*
* @return  pm_err_flag_type 
*          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not
*          available on this ersion of the PMIC.
*          PM_ERR_FLAG__PAR_OUT_OF_RANGE = parameter out of
*          range
*          PM_ERR_FLAG_SUCCESS = SUCCESS.
*
*/
pm_err_flag_type pm_fg_adc_rr_make_sw_req(uint32 pmic_device);




/**
* @brief 
* This function clears Channel log
* 
* @details
* Clears Channel Log of the round robin 
* ADC on demand 
* 
* @param[in] pmic_device   : Primary: 0 Secondary: 1
*
* @return  pm_err_flag_type 
*          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not
*          available on this ersion of the PMIC.
*          PM_ERR_FLAG__PAR_OUT_OF_RANGE = parameter out of
*          range
*          PM_ERR_FLAG_SUCCESS = SUCCESS.
*
*/
pm_err_flag_type pm_fg_adc_rr_clear_channel_log(uint32 pmic_device);


/**
* @brief 
* This function configure FG ADC RR fake battery id range
* 
* @param[in] pmic_device   : Primary: 0 Secondary: 1
* @param[in] fake_id_min_ohms : uint32  
*       fake battery id lower limit in the unit of ohms
* @param[in] fake_id_max_ohms : uint32 
*                 fake battery id uper limit in the unit of ohms
*
* @return  pm_err_flag_type 
*          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not
*          available on this ersion of the PMIC.
*          PM_ERR_FLAG__PAR_OUT_OF_RANGE = parameter out of
*          range
*          PM_ERR_FLAG_SUCCESS = SUCCESS.
*/
pm_err_flag_type pm_fg_adc_rr_set_fake_batt_id_range(uint32 pmic_device, uint32 fake_id_min_ohms, uint32 fake_id_max_ohms);


/**
* @brief This function gets the battery temperature
* @details
*  This function gets the battery temperature
* 
* @param[in] pmic_device_index.  Primary: 0 Secondary: 1
* @param[out] batt_temp          battery tempearature
*
* @return  pm_err_flag_type 
*          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not
*          available on this ersion of the PMIC.
*          PM_ERR_FLAG__PAR_OUT_OF_RANGE = parameter out of
*          range
*          PM_ERR_FLAG_SUCCESS = SUCCESS.
*
*/
pm_err_flag_type pm_fg_batt_info_get_batt_temp(uint32 pmic_device, int32 *batt_temp);

/**
* @brief This function gets the battery id raw adc code
* @details
*  This function gets the battery id adc code
*
* @param[in] pmic_device_index.  Primary: 0 Secondary: 1
* @param[in] pm_fg_adc_rr_batt_id_bias.  hi, low, mediam bias
* @param[out] batt_id_adc          battery id adc code
*
* @return  pm_err_flag_type
*          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not
*          available on this ersion of the PMIC.
*          PM_ERR_FLAG__PAR_OUT_OF_RANGE = parameter out of
*          range
*          PM_ERR_FLAG_SUCCESS = SUCCESS.
*
*/
pm_err_flag_type pm_fg_batt_info_get_batt_id_adc(uint32 pmic_device, pm_fg_adc_rr_batt_id_bias batt_id_bias, int16 *batt_id_adc);

/**
* @brief This function sets to enable or disable ADC for fg_btm
*  This function enable/disable FG_BTM
*
* @param[in] pmic_device_index.  Primary: 0 Secondary: 1
* @param[in] enable              0: disable, 1: enable
*
* @return  pm_err_flag_type
*          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not
*          available on this ersion of the PMIC.
*          PM_ERR_FLAG__PAR_OUT_OF_RANGE = parameter out of
*          range
*          PM_ERR_FLAG_SUCCESS = SUCCESS.
*
*/
pm_err_flag_type pm_fg5_btm_set_en_ctl1(uint32 pmic_device, uint8 enable);


/**
* @brief This function sets to enable one type of BTM measurement
*  This function enable/disable one type of BTM measurement
*
* @param[in] pmic_device_index.  Primary: 0 Secondary: 1
* @param[in] enable_type         defined in pm_fg_btm_en_1
* @param[in] enable              0: disable, 1: enable
*
* @return  pm_err_flag_type
*          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not
*          available on this ersion of the PMIC.
*          PM_ERR_FLAG__PAR_OUT_OF_RANGE = parameter out of
*          range
*          PM_ERR_FLAG_SUCCESS = SUCCESS.*/
pm_err_flag_type pm_fg5_btm_set_btm_en_1(uint32 pmic_device, pm_fg_btm_en_1 enable_type, uint8 enable);


/**
* @brief This function sets the delay for adc conversion
*  This function sets the delay for adc conversion
*
* @param[in] pmic_device_index.  Primary: 0 Secondary: 1
* @param[in] enable_type         defined in pm_fg_btm_en_1
* @param[in] enable              0: disable, 1: enable
*
* @return  pm_err_flag_type
*          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not
*          available on this ersion of the PMIC.
*          PM_ERR_FLAG__PAR_OUT_OF_RANGE = parameter out of
*          range
*          PM_ERR_FLAG_SUCCESS = SUCCESS.*/
pm_err_flag_type pm_fg5_btm_set_btm_ctl_0(uint32 pmic_device, uint8 btm_ctl);


/**
* @brief This function set start or no conversion operation
*  This function set start or no conversion operation
*
* @param[in] pmic_device_index.     Primary: 0 Secondary: 1
* @param[in] operation              0: no conversion req, 1: start conversion req
*
* @return  pm_err_flag_type
*          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not
*          available on this ersion of the PMIC.
*          PM_ERR_FLAG__PAR_OUT_OF_RANGE = parameter out of
*          range
*          PM_ERR_FLAG_SUCCESS = SUCCESS.*/
pm_err_flag_type pm_fg5_btm_set_conv_req(uint32 pmic_device, uint8 operation);

/**
* @brief This function sets battery therm pull up resistance
*  This function sets battery therm pull up resistance
*
* @param[in] pmic_device_index.  Primary: 0 Secondary: 1
* @param[in] pull_up              0: invalid, 1: 30K, 2: 100K, 3: 40K
*
* @return  pm_err_flag_type
*          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not
*          available on this ersion of the PMIC.
*          PM_ERR_FLAG__PAR_OUT_OF_RANGE = parameter out of
*          range
*          PM_ERR_FLAG_SUCCESS = SUCCESS.
*
*/

pm_err_flag_type pm_fg_set_batt_therm_pull_up(uint32 pmic_device, pm_fg_batt_therm_pull_up_res pull_up);


/**
* @brief This function set start or no conversion operation
*  This function set start or no conversion operation
*
* @param[in] pmic_device_index.     Primary: 0 Secondary: 1
* @param[in] operation              0: no conversion req, 1: start conversion req
*
* @return  pm_err_flag_type
*          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not
*          available on this ersion of the PMIC.
*          PM_ERR_FLAG__PAR_OUT_OF_RANGE = parameter out of
*          range
*          PM_ERR_FLAG_SUCCESS = SUCCESS.*/
pm_err_flag_type pm_fg5_btm_set_conv_req(uint32 pmic_device, uint8 operation);
 
/**
* @brief This function sets battery therm measurement frequency
*  This function sets batt temp measuring frequency
*
* @param[in] pmic_device_index.  Primary: 0 Secondary: 1
* @param[in] value              1: 1 seconds, 2: 2 seconds etc.
*
* @return  pm_err_flag_type
*          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not
*          available on this ersion of the PMIC.
*          PM_ERR_FLAG__PAR_OUT_OF_RANGE = parameter out of
*          range
*          PM_ERR_FLAG_SUCCESS = SUCCESS.
*
*/
pm_err_flag_type pm_fg_set_batt_therm_freq(uint32 pmic_device, uint8 value);


#endif /* __PM_FG_ADC_RR_H__ */
