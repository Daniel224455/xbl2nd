#ifndef __PM_SCHG_DCDC_H__
#define __PM_SCHG_DCDC_H__

/*! \file
*  \n
*  \brief  pm_schg_dcdc.h PMIC-SMBC MODULE RELATED DECLARATION 
*  \details  This header file contains functions and variable declarations 
*  to support Qualcomm PMIC SCHG DCDC (Switch-Mode Battery Charger) module. The 
*  Switched-Mode Battery Charger (SCHG DCDC) module includes a buck regulated 
*  battery charger with integrated switches. The SCHG DCDC module, along with the 
*  Over Voltage Proection (OVP) module will majorly be used by charger 
*  appliation for charging Li-Ion batteries with high current (up to 2A).
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
06/29/18   dc 	   Expand ICL Status API for Max ICL
05/17/18   ivy	   Added support for configuring Qi charging ICL
01/14/16   pmx     Initial version. 
========================================================================== */
#include "com_dtypes.h"
#include "pm_err_flags.h"
#include "pm_resources_and_types.h"

/*===========================================================================

                        TYPE DEFINITIONS 

===========================================================================*/
typedef enum
{
  PM_SCHG_DCDC_OTG_HICCUP_CNTR_RST_TIMER_SEL = 0, /*0 = 450/840/1700/3500ms (dpenpends on BOOST_LARGE_CAP_SS_TIMEOUT 1 = 40ms*/
  PM_SCHG_DCDC_OTG_CFG_OTG_EN_SRC_CFG =1,             /*0 = OTG Enable Source Command       1 = OTG Enable Source Type-C/RID*/
  PM_SCHG_DCDC_OTG_CFG_ENABLE_OTG_IN_DEBUG_MODE = 2,  /*0 = Do not enable OTG in debug mode 1 = Enable OTG in debug mode*/
  PM_SCHG_DCDC_CFG_FASTROLESWAP_START_OPTION =6,  /*0 = Directly switch on boost mode, 1 = Turn off buck and turn on boost*/
  PM_SCHG_DCDC_OTG_CFG_INVALID,                       /*INVALID*/
} pm_schg_dcdc_otg_cfg_type;


/**  OTG status type **/
typedef enum
{
    PM_SCHG_DCDC_OTG_DISABLED,
    PM_SCHG_DCDC_OTG_EXECUTING_ENABLE_SEQ,
    PM_SCHG_DCDC_OTG_ENABLED,
    PM_SCHG_DCDC_OTG_EXECUTING_DISABLE_SEQ,
    PM_SCHG_DCDC_OTG_ERROR,
    PM_SCHG_DCDC_OTG_STATUS_INVALID
} pm_schg_dcdc_otg_status_type;

typedef struct _pm_schg_dcdc_icl_status_type
{
	uint32 max_icl_status;                            /* Max ICL value */
	uint32 aicl_icl_status;                           /* ICL status post AICL */
	uint32 thermal_icl_status;                        /* ICL status post HW therm adjustment */
	uint32 final_icl_status;                          /* ICL status post AICL, HW therm and SW therm adjustment */
	boolean aicl_done;
}pm_schg_dcdc_icl_status_type, *ppm_schg_dcdc_icl_status_type;

/*===========================================================================

                 SCHG DCDC DRIVER FUNCTION PROTOTYPES

===========================================================================*/
/**
 * @brief This function enable/disable OTG functionality.
 * 
 * @param[in] pmic_device. Secondary: 1, Primary: 0
 * @param[in] enable: whether to enable or disable otg
 *
 * @return  pm_err_flag_type 
 *          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not available on this
 *          version of the PMIC.
 *          PM_ERR_FLAG_SUCCESS               = SUCCESS.
 *
 */
pm_err_flag_type pm_schg_dcdc_otg_enable(uint32 pmic_device,boolean enable);

/**
 * @brief This function configures OTG configuration.
 * 
 * @details
 *  This function configures OTG configuration.
 *  The following can be configured in this API:
 *                1) OTG source
 *                2) OTG in debug mode or not
 *                3) fast role swap start option
 * 
 * @param[in] pmic_device. Secondary: 1, Primary: 0
 * @param[in] otg_cfg_type: which configuration to be configured.
 * @param[in] set_value: Destinate configure value
 *
 * @return  pm_err_flag_type 
 *          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not available on this
 *          version of the PMIC.
 *          PM_ERR_FLAG_SUCCESS               = SUCCESS.
 *
 */
pm_err_flag_type pm_schg_dcdc_config_otg(uint32 pmic_device, pm_schg_dcdc_otg_cfg_type otg_cfg_type, boolean set_value);

/**
 * @brief This function set current limit for OTG.
 * 
 * @param[in] pmic_device. Secondary: 1, Primary: 0
 * @param[in] otg_limit_ma: The max current allowed on OTG, in the unit of mA
 *
 * @return  pm_err_flag_type 
 *          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not available on this
 *          version of the PMIC.
 *          PM_ERR_FLAG_SUCCESS               = SUCCESS.
 *
 */
pm_err_flag_type pm_schg_dcdc_set_otg_current_limit(uint32 pmic_device, uint32 otg_limit_ma);

/**
 * @brief This function returns OTG status.
 * 
 * @details
 *  This function returns OTG status. enabled/disabled/executing enable sequence/executing disable sequence/error
 * 
 * @param[in] pmic_device. Secondary: 1, Primary: 0
 * @param[out] otg_status:    Refer #pm_schg_dcdc_otg_status_type for 
 *       more info
 *
 * @return  pm_err_flag_type 
 *          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not available on this
 *          version of the PMIC.
 *          PM_ERR_FLAG_SUCCESS               = SUCCESS.
 *
 */
pm_err_flag_type pm_schg_dcdc_get_otg_status(uint32 pmic_device, pm_schg_dcdc_otg_status_type* otg_status);

/**
* @brief This function returns ICL status
*
* @details
*  This function returns ICL status
*
* @param[in] pmic_device. Secondary: 1, Primary: 0
* @param[out] pm_schg_dcdc_icl_status_type:    Refer #pm_schg_dcdc_icl_status_type for
*       more info
*
* @return  pm_err_flag_type
*          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not available on this
*          version of the PMIC.
*          PM_ERR_FLAG_SUCCESS               = SUCCESS.
*
*/
pm_err_flag_type pm_schg_dcdc_get_icl_status(uint32 pmic_device, pm_schg_dcdc_icl_status_type *icl_status);


/**
* @brief This function configure vsysmin value
*
* @param[in] pmic_device: Secondary: 1, Primary: 0
* @param[in] vsysmin_mv:  Vsysmin voltage in mV
*
* @return  pm_err_flag_type
*          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not available on this
*          version of the PMIC.
*          PM_ERR_FLAG_SUCCESS               = SUCCESS.
*/
pm_err_flag_type pm_schg_dcdc_configure_vsysmin(uint32 pmic_device, uint16 vsysmin_mv);

/**
* @brief This function configure qi charging input current limit
*
* @param[in] pmic_device: Secondary: 1, Primary: 0
* @param[in] current_mA:  current in mV
*
* @return  pm_err_flag_type
*          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not available on this
*          version of the PMIC.
*          PM_ERR_FLAG_SUCCESS               = SUCCESS.
*/

pm_err_flag_type pm_schg_dcdc_set_ref_max_psns(uint32 pmic_device, uint32 current_mA);

#endif /* __PM_SCHG_DCDC_H__*/

