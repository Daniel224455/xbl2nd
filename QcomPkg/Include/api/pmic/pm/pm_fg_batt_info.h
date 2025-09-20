#ifndef __PM_FG_BATT_INFO_H__
#define __PM_FG_BATT_INFO_H__


/*! \file pm_fg_batt_info.h 
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
06/13/18   cs        fix sram corruption issue for 1.0, 1.1
03/05/18   pxm     Remove unused APIs and registers.
09/19/17   cs      for SDM855 remove unsupported functions.
02/02/17   cs      add batt_id th in batt_missing_cfg
11/10/16   va      Adding JEITA Hyst Api
10/06/16   cs      configure batt therm/aux therm bias wait 
06/16/16   va      Reading IBat, Vbat from batt info
03/24/16   va      Adding batt missing cfg api
03/16/16   va      Adding pm_fg_batt_get_batt_temp_sts()
02/16/16   va      Adding required API for enanlbing charger
01/12/16   al      Initial Release
===========================================================================*/

#include "com_dtypes.h"
#include "pm_err_flags.h"
#include "pm_resources_and_types.h"

/*===========================================================================
                        TYPE DEFINITIONS 
===========================================================================*/

/* Common Interrupt Prototype */
typedef enum
{
  PM_FG_BATT_INFO_VBT_PRD_DELTA, /*Predicted battery voltage is exceeding (lower or higher) the defined theshold*/
  PM_FG_BATT_INFO_VBT_LOW,       /*Battery voltage is lower than the Battery Low threhsold*/
  PM_FG_BATT_INFO_ESR_DELTA,     /*Battery resistance has changed by more (increment od decrement) than the specificed threshold*/
  PM_FG_BATT_INFO_BT_MISS,       /*Battery missing interrupt*/
  PM_FG_BATT_INFO_BT_ATTN,       /*General purpose interrupt*/
  PM_FG_BATT_INFO_WDOG_EXP,      /*Watchdog has expired*/
  PM_FG_BATT_INFO_BT_TMPR_DELTA, /*Battery temperature has changed by more (increment od decrement) than the specificed threshold*/
  PM_FG_BATT_INFO_IRQ_INVALID,   /*Invalid*/
} pm_fg_batt_info_irq_type;

typedef enum
{
  PM_FG_BATT_INFO_ESR_PULL_DOWN_CFG,
  PM_FG_BATT_INFO_ESR_MEAS_CUR = 2,
  PM_FG_BATT_INFO_ESR_CFG_INVALID,
}pm_fg_batt_info_batt_esr_pull_down_cfg;

typedef enum
{
  PM_FG_BATT_INFO_ESR_MEAS_CUR_60MA,
  PM_FG_BATT_INFO_ESR_MEAS_CUR_120MA,
  PM_FG_BATT_INFO_ESR_MEAS_CUR_180MA,
  PM_FG_BATT_INFO_ESR_MEAS_CUR_240MA,
  PM_FG_BATT_INFO_ESR_MEAS_CUR_INVALID,
}pm_fg_batt_info_batt_esr_pull_down_cfg_ival;


/*===========================================================================
                        FUNCTION DEFINITIONS
===========================================================================*/


/**
* @brief This function Clear the output of the comparison for JEITA Threshold
* 
* @details
*  This function sets interrupt triggers
* 
* @param[in] pmic_device_index.  Primary: 0 Secondary: 1
* @param[in] clear_status        TRUE to clear and FALSE to not to clear
*
* @return  pm_err_flag_type 
*          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not available on this
*          version of the PMIC.
*          PM_ERR_FLAG_SUCCESS               = SUCCESS.
*
*/
//pm_err_flag_type pm_fg_batt_info_clear_jeita_status(uint32 device_index, boolean clear_status);


/**
* @brief This function sets interrupt triggers
* 
* @details
*  This function sets interrupt triggers
* 
* @param[in] pmic_device_index.  Primary: 0 Secondary: 1
* @param[in] coeff_type          coefficient type. Refer #pm_fg_batt_info_therm_coeff_type for more info
* @param[in] coeff_value         coefficient value
*
* @return  pm_err_flag_type 
*          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not available on this
*          version of the PMIC.
*          PM_ERR_FLAG_SUCCESS               = SUCCESS.
*
*/
//pm_err_flag_type pm_fg_batt_info_set_therm_coeff_value(uint32 device_index, pm_fg_batt_info_therm_coeff_type coeff_type, uint8 coeff_value);


/**
* @brief This function sets the JEITA threshold value 
* 
* @details
*  This function sets the JEITA threshold value 
* 
* @param[in] pmic_device_index.  Primary: 0 Secondary: 1
* @param[in] threshold_type      Threshold type. Refer #pm_fg_batt_info_jeita_threshold_type for more info
* @param[in] threshld_value      threshold value
*
* @return  pm_err_flag_type 
*          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not available on this
*          version of the PMIC.
*          PM_ERR_FLAG_SUCCESS               = SUCCESS.
*
*/
//pm_err_flag_type pm_fg_batt_info_set_jeita_threshold_value(uint32 device_index, pm_fg_batt_info_jeita_threshold_type threshold_type, uint8 threshld_value);


/**
* @brief This function gets battery temperature status
* 
* @details
*  This function gets battery temperature status
* 
* @param[in] device_index.  Primary: 0 Secondary: 1
* @param[in] temp_sts       pm_fg_batt_info_batt_temp_sts  battery temperature status
* @param[in] status         TRUE/FALSE for battery temperature 
*
* @return  pm_err_flag_type 
*          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not available on this
*          version of the PMIC.
*          PM_ERR_FLAG_SUCCESS               = SUCCESS.
*
*/
//pm_err_flag_type pm_fg_batt_info_get_batt_temp_sts(uint32 device_index, pm_fg_batt_info_batt_temp_sts temp_sts, boolean *status);

/**
* @brief This function clears battery removed latched sts 
* @details
*  This function returns BMS Fule Gauge Batt system status 
*
* @param[in] pmic_pmic_device. Primary: 0 Secondary: 1
* @param[out]pm_fg_batt_sys_sts system profile status
*
* @return  pm_err_flag_type 
*          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not available on this
*          version of the PMIC.
*          PM_ERR_FLAG_SUCCESS               = SUCCESS.
*
*/
//pm_err_flag_type pm_fg_batt_info_clr_batt_removed_latched_sts(uint32 pmic_device);

/**
* @brief This function returns  Fule Gauge Batt system status * 
* @details
*  This function returns BMS Fule Gauge Batt system status 
*
* @param[in] pmic_pmic_device. Primary: 0 Secondary: 1
* @param[out]pm_fg_batt_sys_sts system profile status
*
* @return  pm_err_flag_type 
*          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not available on this
*          version of the PMIC.
*          PM_ERR_FLAG_SUCCESS               = SUCCESS.
*
*/
//pm_err_flag_type pm_fg_batt_info_get_sys_batt_sts(uint32 pmic_device, pm_fg_batt_info_sys_batt_sts batt_sys_sts, boolean *status);

/**
* @brief This function gets battery missing cfg status
* 
* @details
*  This function gets battery missing cfg status
* 
* @param[in] device_index.  Primary: 0 Secondary: 1
* @param[out] batt_missing_cfg       pm_fg_batt_info_batt_missing_cfg battery missing cfg status
*
* @return  pm_err_flag_type 
*          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not available on this
*          version of the PMIC.
*          PM_ERR_FLAG_SUCCESS               = SUCCESS.
*
*/
//pm_err_flag_type pm_fg_batt_info_get_batt_missing_cfg(uint32 pmic_device, pm_fg_batt_info_batt_missing_cfg *batt_missing_cfg);

/**
* @brief This function sets battery missing cfg status
* 
* @details
*  This function sets battery missing cfg status
* 
* @param[in] device_index.  Primary: 0 Secondary: 1
* @param[in] batt_missing_cfg       pm_fg_batt_info_batt_missing_cfg battery missing cfg status
*
* @return  pm_err_flag_type 
*          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not available on this
*          version of the PMIC.
*          PM_ERR_FLAG_SUCCESS               = SUCCESS.
*
*/
//pm_err_flag_type pm_fg_batt_info_set_batt_missing_cfg(uint32 pmic_device, pm_fg_batt_info_batt_missing_cfg batt_missing_cfg);

/**
* @brief This function reads battery voltage in Mv
* 
* @details
*  This function sets battery missing cfg status
* 
* @param[in] device_index.  Primary: 0 Secondary: 1
* @param[out] pVBat       Battery voltage
*
* @return  pm_err_flag_type 
*          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not available on this
*          version of the PMIC.
*          PM_ERR_FLAG_SUCCESS               = SUCCESS.
*
*/
pm_err_flag_type pm_fg_batt_info_get_vbatt(uint32 pmic_device, uint32 *pVBat);


/**
* @brief This function reads battery current in mA
* 
* @details
*  This function reads battery current in mA
* 
* @param[in] device_index.  Primary: 0 Secondary: 1
* @param[out] pIBat       Battery Current
*
* @return  pm_err_flag_type 
*          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not available on this
*          version of the PMIC.
*          PM_ERR_FLAG_SUCCESS               = SUCCESS.
*
*/
pm_err_flag_type pm_fg_batt_info_get_ibatt(uint32 pmic_device, int32 *pIBat);

/**
* @brief This function set esr pull down cfg
* 
* @details
*  This function reads battery current in mA
* 
* @param[in] device_index. Primary: 0 Secondary: 1
* @param[in] esr_cfg       esr pull down cfg
* @param[in] esr_value     esr value
* @return  pm_err_flag_type 
*          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not available on this
*          version of the PMIC.
*          PM_ERR_FLAG_SUCCESS               = SUCCESS.
*
*/
pm_err_flag_type pm_fg_batt_info_set_esr_pull_down_cfg(uint32 pmic_device, pm_fg_batt_info_batt_esr_pull_down_cfg esr_cfg, uint8 esr_value);

/**
* @brief This function set peek_mux4
* 
* @details
*  This function that 
* 
* @param[in] device_index.  Primary: 0 Secondary: 1
* @param[in] reg_val. value to set in peek_mux4
* @return  pm_err_flag_type 
*          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not available on this
*          version of the PMIC.
*          PM_ERR_FLAG_SUCCESS               = SUCCESS.
*
*/
pm_err_flag_type pm_fg_batt_info_set_peek_mux4(uint32 pmic_device, uint8 reg_val);

/**
* @brief This function get algact status
* 
* @details
*  This function that
* 
* @param[in]  device_index. Primary: 0 Secondary: 1
* @param[in] pAlgactv. algorithm active status
* @return  pm_err_flag_type 
*          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not available on this
*          version of the PMIC.
*          PM_ERR_FLAG_SUCCESS               = SUCCESS.
*
*/
pm_err_flag_type pm_fg_batt_info_get_alg_actv_status(uint32 pmic_device, uint8* pAlgactv);



#endif /* __PM_FG_BATT_INFO_H__ */
