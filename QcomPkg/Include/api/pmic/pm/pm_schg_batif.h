#ifndef __PM_SCHG_BATIF_H__
#define __PM_SCHG_BATIF_H__

/*! \file
*  \n
*  \brief  pm_schg_batif.h PMIC-SCHG MODULE RELATED DECLARATION 
*  \details  This header file contains functions and variable declarations 
*  to support Qualcomm PMIC SCHG BAT IF (Switch-Mode Battery Charger) module. The 
*  Switched-Mode Battery Charger (SCHG OTG) module includes a buck regulated 
*  battery charger with integrated switches. The SCHG OTG module, along with the 
*  Over Voltage Proection (OVP) module will majorly be used by charger 
*  appliation for charging Li-Ion batteries with high current (up to 2A).
*  \n &copy; 
*  Copyright (c) 2016-2018 Qualcomm Technologies, Inc.  All Rights Reserved. 
*  Qualcomm Technologies Proprietary and Confidential.
*/

/* =======================================================================
                                Edit History
This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.


when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
08/01/18   ivy     Added API to set batt therm pull up to register
06/21/18   cs      added fake battery detection support.
02/28/18   pxm     Remove unused APIs. Refine code
03/01/16   sm      Added pm_schg_batif_cfg_batt_missing_src()
02/16/16   va      Adding required API for enabling charger
01/14/16   al      Initial version. 
========================================================================== */
#include "com_dtypes.h"
#include "pm_err_flags.h"
#include "pm_resources_and_types.h"

/*===========================================================================

                        TYPE DEFINITIONS 

===========================================================================*/


/*! \enum pm_schg_irq_bit_field_type
   \brief different types of irq bit fields of by smbch_bat_if irq module
 */
typedef enum {
  PM_SCHG_BATIF_BAT_TEMP,
  PM_SCHG_BATIF_ALL_CH_CONV_DONE,
  PM_SCHG_BATIF_BAT_OV,
  PM_SCHG_BATIF_BAT_LOW,
  PM_SCHG_BATIF_BAT_THERM_OR_ID_MISSING,
  PM_SCHG_BATIF_BAT_TERMINAL_MISSING,
  PM_SCHG_BATIF_BUCK_OC_OV,
  PM_SCHG_BATIF_VPH_OV,
  PM_SCHG_BATIF_IRQ_INVALID
}pm_schg_batif_irq_type;



typedef enum {
  PM_SCHG_BATIF_LOW_BATTERY_THRESH_2P5,
  PM_SCHG_BATIF_LOW_BATTERY_THRESH_2P6,
  PM_SCHG_BATIF_LOW_BATTERY_THRESH_2P7,
  PM_SCHG_BATIF_LOW_BATTERY_THRESH_2P8,
  PM_SCHG_BATIF_LOW_BATTERY_THRESH_2P9,
  PM_SCHG_BATIF_LOW_BATTERY_THRESH_3P0,
  PM_SCHG_BATIF_LOW_BATTERY_THRESH_3P1,
  PM_SCHG_BATIF_LOW_BATTERY_THRESH_3P2,
  PM_SCHG_BATIF_LOW_BATTERY_THRESH_3P3,
  PM_SCHG_BATIF_LOW_BATTERY_THRESH_3P4,
  PM_SCHG_BATIF_LOW_BATTERY_THRESH_3P5,
  PM_SCHG_BATIF_LOW_BATTERY_THRESH_3P6,
  PM_SCHG_BATIF_LOW_BATTERY_THRESH_3P7,
  PM_SCHG_BATIF_LOW_BATTERY_THRESH_3P8,
  PM_SCHG_BATIF_LOW_BATTERY_THRESH_INVALID
}pm_schg_batif_low_battery_threshold;

typedef enum {
  PM_SCHG_BAT_MISS_SRC_CFG__PIN_SRC_EN = 0,
  PM_SCHG_BAT_MISS_SRC_CFG__BAT_MISS_ALG_EN = 2, // value is 2 bacause register bit for this config is 2, 1 is RESERVED.
  PM_SCHG_BAT_MISS_SRC_CFG__INVALID
}pm_schg_batif_bat_miss_src_cfg_type;

typedef enum {
  PM_SCHG_BAT_MISS_THERM_CMP = 0,
  PM_SCHG_BAT_MISS_BAT_ID_BMISS_CMP,
  PM_SCHG_BAT_MISS_FAKE_BAT_DET,
  PM_SCHG_BAT_MISS_INVALID
}pm_schg_batif_bat_miss_status_type;

typedef enum{
	PM_SCHG_BATIF_PULL_UP_BAT_THM	= 0,
	PM_SCHG_BATIF_PULL_UP_MISC_THM	= 2,
	PM_SCHG_BATIF_PULL_UP_CONN_THM	= 4,
	PM_SCHG_BATIF_PULL_UP_SMB_THM	= 6,
	PM_SCHG_BATIF_PULL_UP_INVALID
}pm_schg_batif_adc_pull_up_type;

typedef enum{
	PM_SCHG_BATIF_PULL_UP_RES_NONE,
	PM_SCHG_BATIF_PULL_UP_RES_30K,
	PM_SCHG_BATIF_PULL_UP_RES_100K,
	PM_SCHG_BATIF_PULL_UP_RES_400K,
	PM_SCHG_BATIF_PULL_UP_RES_INVALID,
}pm_schg_batif_adc_pull_up_res_type;


/*===========================================================================

                 SCHG BAT_IF DRIVER FUNCTION PROTOTYPES

===========================================================================*/

/*Enable Pmic Ship Mode */
/**
 * @brief This function Enables Pmic Ship Mode.
 * 
 * @details
 *  This function Enables Pmic Ship Mode.
 * 
 * @param[in] pmic_device_index. Primary: 0 Secondary: 1
 *
 * @return  pm_err_flag_type 
 *          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not available on this
 *          version of the PMIC.
 *          PM_ERR_FLAG_SUCCESS               = SUCCESS.
 *
 */
pm_err_flag_type pm_schg_batif_enable_shipmode(uint32 device_index);

/**
* @brief This function enables IRQ
* 
* @details
*  This function enables IRQ
* 
* @param[in] pmic_device_index.  Primary: 0 Secondary: 1
* @param[in] irq                 pm_schg_batif_irq_type
*                                
*
* @return  pm_err_flag_type 
*          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not available on this
*          version of the PMIC.
*          PM_ERR_FLAG_SUCCESS               = SUCCESS.
*
*/
pm_err_flag_type pm_schg_batif_irq_enable(uint32 device_index, pm_schg_batif_irq_type irq, boolean enable);


/**
* @brief This function clears the SCHG irq 
* 
* @details
*  This function clears the SCHG irq 
* 
* @param[in] pmic_device_index.  Primary: 0 Secondary: 1
* @param[in] irq                 pm_schg_batif_irq_type
*                                
*
* @return  pm_err_flag_type 
*          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not available on this
*          version of the PMIC.
*          PM_ERR_FLAG_SUCCESS               = SUCCESS.
*
*/
pm_err_flag_type pm_schg_batif_irq_clear(uint32  device_index, pm_schg_batif_irq_type irq);

/**
* @brief This function configures the SCHG irq trigger 
* 
* @details
*  This function configures the SCHG irq trigger 
* 
* @param[in] pmic_device_index.  Primary: 0 Secondary: 1
* @param[in] irq                 pm_schg_batif_irq_type
*                                
*
* @return  pm_err_flag_type 
*          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not available on this
*          version of the PMIC.
*          PM_ERR_FLAG_SUCCESS               = SUCCESS.
*
*/
pm_err_flag_type pm_schg_batif_irq_set_trigger(uint32 device_index, pm_schg_batif_irq_type irq, pm_irq_trigger_type trigger);


/**
* @brief This function configures the SCHG for irq 
* 
* @details
*  This function configures the SCHG for irq 
* 
* @param[in] pmic_device_index.  Primary: 0 Secondary: 1
* @param[in] irq                 pm_schg_batif_irq_type
*                                
*
* @return  pm_err_flag_type 
*          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not available on this
*          version of the PMIC.
*          PM_ERR_FLAG_SUCCESS               = SUCCESS.
*
*/
pm_err_flag_type pm_schg_batif_irq_status(uint32 device_index, pm_schg_batif_irq_type irq, pm_irq_status_type type, boolean *status);

/**
* @brief This function set low battery detect cfg
* 
* @details
*  This function sets low battery detect cfg
* 
* @param[in] pmic_device_index.  Primary: 0 Secondary: 1
* @param[in] lbt_cfg             low batt detect cfg
*                                
*
* @return  pm_err_flag_type 
*          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not available on this
*          version of the PMIC.
*          PM_ERR_FLAG_SUCCESS               = SUCCESS.
*
*/
pm_err_flag_type pm_schg_batif_set_low_batt_detect_threshold(uint32 device_index, pm_schg_batif_low_battery_threshold lbt_cfg);


/**
* @brief This function configures battery missing source
* 
* @details
*  This function configures battery missing source
* 
* @param[in] pmic_device_index.  Primary: 0 Secondary: 1
* @param[in] bat_miss_cfg        battery missing config type
*                                See pm_schg_batif_bat_miss_src_cfg_type for details
* @param[in] enable.             FALSE: 0 TRUE: 1
*
* @return  pm_err_flag_type 
*          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not available on this
*          version of the PMIC.
*          PM_ERR_FLAG_SUCCESS               = SUCCESS.
*
*/
pm_err_flag_type pm_schg_batif_cfg_batt_missing_src(uint32 device_index, pm_schg_batif_bat_miss_src_cfg_type bat_miss_cfg, boolean enable);


/**
* @brief This function gets battery missing and fake battery status
*
* @details
*  This function gets battery missing and fake battery status
*
* @param[in] pmic_device_index.  Primary: 0 Secondary: 1
* @param[in] status_type         battery missing status type
*                                See pm_schg_batif_bat_miss_status_type for details
* @param[out] enable.             FALSE: 0 TRUE: 1
*
* @return  pm_err_flag_type
*          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not available on this
*          version of the PMIC.
*          PM_ERR_FLAG_SUCCESS               = SUCCESS.
*
*/
pm_err_flag_type pm_schg_batt_missing_status(uint32 device_index, pm_schg_batif_bat_miss_status_type status_type, boolean* st);


/**
* @brief This function disables fake battery detection for a given pmic
*
* @details
*  This function disables fake battery detection
*
* @param[in] pmic_device_index.  Primary: 0 Secondary: 1
* @param[in] enable: TRUE: enable, FALSE: disable
*
* @return  pm_err_flag_type
*          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not available on this
*          version of the PMIC.
*          PM_ERR_FLAG_SUCCESS               = SUCCESS.
*
*/
pm_err_flag_type 
pm_schg_batif_enable_fake_bat_detect( uint32 device_index, boolean enable);

/**
* @brief This function sets adc internal pull up resistance value for a given pmic
*
* @details
*  This function sets adc internal pull up resistance value
*
* @param[in] pmic_device_index.  Primary: 0 Secondary: 1
*
* @return  pm_err_flag_type
*          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not available on this
*          version of the PMIC.
*          PM_ERR_FLAG_SUCCESS               = SUCCESS.
*
*/

pm_err_flag_type
pm_schg_batif_set_adc_internal_pull_up(uint32 device_index, pm_schg_batif_adc_pull_up_type pull_up_type, pm_schg_batif_adc_pull_up_res_type resistance);

/**
* @brief This function gets adc internal pull up resistance value for a given pmic
*
* @details
*  This function gets adc internal pull up resistance value
*
* @param[in] pmic_device_index.  Primary: 0 Secondary: 1
*
* @return  pm_err_flag_type
*          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not available on this
*          version of the PMIC.
*          PM_ERR_FLAG_SUCCESS               = SUCCESS.
*
*/
/*
pm_err_flag_type
pm_schg_batif_get_adc_internal_pull_up(uint32 device_index, pm_schg_batif_adc_pull_up_type pull_up_type, pm_schg_batif_adc_pull_up_res_type *resistance);
*/
#endif /* __PM_SCHG_BATIF_H__*/

