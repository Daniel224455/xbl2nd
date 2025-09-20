#ifndef __PM_SCHG_CHGR_H__
#define __PM_SCHG_CHGR_H__

/*! \file
*  \n
*  \brief  pm_SMBB.h PMIC-SMBC MODULE RELATED DECLARATION 
*  \details  This header file contains functions and variable declarations 
*  to support Qualcomm PMIC SCHG (Switch-Mode Battery Charger) module. The 
*  Switched-Mode Battery Charger (SCHG) module includes a buck regulated 
*  battery charger with integrated switches. The SCHG module, along with the 
*  Over Voltage Proection (OVP) module will majorly be used by charger 
*  appliation for charging Li-Ion batteries with high current (up to 2A).
*  \n &copy; Copyright 2016-2018 QUALCOMM Technologies Incorporated, All Rights Reserved.
*/

/* =======================================================================
                                Edit History
This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.


when       who    what, where, why
--------   ---    ---------------------------------------------------------- 
06/28/18   pxm    Add AFP enablement.
02/28/18   pxm    Remove unused APIs. Refine code
02/08/18   pxm    update pm_schg_chgr_enable_charging due to charging enablement change in PM855B
11/27/17   ra     AFP changes for SDM855
11/09/17   ra     JEITA changes for PM855B
10/24/17   ra     Removing obsolete APIs
12/19/16   sm     Added API to get SCHG Charger PmicIndex
03/01/16   sm     Added pm_schg_chgr_jeita_en_cfg()
02/16/16   va     Adding required API for enabling charger
01/24/16   al     Initial version. 
========================================================================== */

/*===========================================================================

                        INCLUDE FILES

===========================================================================*/
#include "com_dtypes.h"
#include "pm_err_flags.h"
#include "pm_resources_and_types.h"

/*===========================================================================

                        TYPE DEFINITIONS

===========================================================================*/

/*! \enum pm_schg_chgr_irq_bit_field_type
   \brief different types of irq bit fields of by smbb irq module
 */
typedef enum {
  PM_SCHG_CHGR_IRQ_CHGR_ERROR,
  PM_SCHG_CHGR_IRQ_CHARGING_STATE_CHANGE,
  PM_SCHG_CHGR_IRQ_STEP_CHARGING_STATE_CHANGE,
  PM_SCHG_CHGR_IRQ_STEP_CHARGING_SOC_UPDATE,
  PM_SCHG_CHGR_IRQ_STEP_CHARGING_SOC_UPDATE_REQUEST,
  PM_SCHG_CHGR_IRQ_FG_FVCAL_QUALIFIED,
  PM_SCHG_CHGR_IRQ_INVALID
}pm_schg_chgr_irq_type;

typedef enum
{
  PM_SCHG_CHGR_PRE_CHARGE_CURRENT,
  /*pre_charge current*/
  PM_SCHG_CHGR_FAST_CHARGE_CURRENT,
  /*fast charge current*/
  PM_SCHG_CHGR_TERMINATION_CURRENT,
  /*termination current*/
  PM_SCHG_TCCC_CHGR_TERMINATION_CURRENT,
  /*tccc termination current*/
  PM_SCHG_CHGR_CURRENT_TYPE_INVALID,
  /*invalid*/
}pm_schg_chgr_current_type;

typedef enum
{
  PM_SCHG_BATT_CHGR_STATUS_INHIBIT,
  PM_SCHG_BATT_CHGR_STATUS_TRICKLE,
  PM_SCHG_BATT_CHGR_STATUS_PRECHARGE,
  PM_SCHG_BATT_CHGR_STATUS_FULLON,
  PM_SCHG_BATT_CHGR_STATUS_TAPER,
  PM_SCHG_BATT_CHGR_STATUS_TERMINATION,
  PM_SCHG_BATT_CHGR_STATUS_PAUSE,
  PM_SCHG_BATT_CHGR_STATUS_INVALID,
}pm_schg_battery_charger_status;

typedef struct
{
  pm_schg_battery_charger_status battery_charger_status;
  uint8   step_charging_status;
  boolean zero_charging_current;
  boolean icl_incr_req_for_prechg;
}pm_schg_chgr_battery_charger_status_1;

typedef struct
{
  boolean bat_therm_missing;
  boolean bat_ov;
  boolean bat_sft_expire;
  boolean bat_gt_full_on;
  boolean vbatt_gtet_float_voltage;
  boolean vbatt_gtet_inhibit;
  boolean vbatt_ltet_recharge;
  boolean drop_in_vbat_reference;
}pm_schg_chgr_battery_charger_status_2;

typedef struct
{
  boolean enable_fullon_mode;
  boolean enable_pre_charging;
  boolean enable_trickle;
  boolean enable_chg_sensors;
  boolean charging_enable;
  boolean force_zero_charge_current;
  boolean disable_charging;
  boolean valid_input_power_source;
}pm_schg_chgr_battery_charger_status_5;


typedef struct
{
  boolean jeita_adc_thot_afp;
  boolean jeita_adc_thot;
  boolean jeita_adc_hot;
  boolean jeita_adc_cold;
  boolean jeita_adc_tcold;
  boolean jeita_adc_tcold_afp;
  boolean reserved1;
  boolean reserved2;
}pm_schg_chgr_battery_charger_status_6;


typedef struct
{
  boolean bat_temp_status_too_cold_afp;
  boolean bat_temp_status_too_hot_afp;
  boolean bat_temp_status_too_cold;
  boolean bat_temp_status_too_hot;
  boolean bat_temp_status_cold_soft;
  boolean bat_temp_status_hot_soft;
  boolean reserved1;
  boolean reserved2;
}pm_schg_chgr_battery_charger_status_7;


typedef struct
{
  pm_schg_chgr_battery_charger_status_1 status_1;
  pm_schg_chgr_battery_charger_status_2 status_2;
  uint8                                 status_3;  //float_voltage
  uint8                                 status_4;  //charge current reference
  pm_schg_chgr_battery_charger_status_5 status_5;
  pm_schg_chgr_battery_charger_status_6 status_6;
  pm_schg_chgr_battery_charger_status_7 status_7;
  uint8                                 status_8;
}pm_schg_chgr_status_type;

typedef enum
{
  PM_SCHG_CHGR_JEITA_COLD_SL_CCC,       /** <Cold SL Charge Current Compensation              >*/
  PM_SCHG_CHGR_JEITA_HOT_SL_CCC,        /** <Hot SL Charge Current Compensation               >*/
  PM_SCHG_CHGR_JEITA_COLD_SL_FVC,       /** <Cold SL Float Voltage Compensation               >*/
  PM_SCHG_CHGR_JEITA_HOT_SL_FVC,        /** <Hot SL Float Voltage Compensation                >*/
  PM_SCHG_CHGR_JEITA_HARD_LIMIT,        /** <Jeita Temperature Hard Limit disable             >*/
  PM_SCHG_CHGR_JEITA_FVCOMP_IN_CV,      /** <Jeita hot/cold soft/hard limit reduces float voltage in CV mode >*/
  PM_SCHG_CHGR_JEITA_AFP_COLD,          /** <Jeita cold condition triggers AFP mode           >*/
  PM_SCHG_CHGR_JEITA_AFP_HOT,          /** <Jeita hot condition triggers AFP mode           >*/
  PM_SCHG_CHGR_JEITA_INVALID            /** < Invalid                                         >*/
}pm_schg_chgr_jeita_cfg_type;

typedef enum
{
	PM_SCHG_CHGR_JEITA_THRESHOLD_HOT,            /** <Soft Hot Jeita Threshold              >*/
	PM_SCHG_CHGR_JEITA_THRESHOLD_COLD,           /** <Soft Cold Jeita Threshold             >*/
	PM_SCHG_CHGR_JEITA_THRESHOLD_TOO_HOT,        /** <Hard Hot Jeita Threshold              >*/
	PM_SCHG_CHGR_JEITA_THRESHOLD_TOO_COLD,       /** <Hard COld Jeita Threshold             >*/
	PM_SCHG_CHGR_JEITA_THRESHOLD_TOO_HOT_AFP,    /** <AFP Hot Jeita Threshold               >*/
	PM_SCHG_CHGR_JEITA_THRESHOLD_TOO_COLD_AFP,   /** <AFP Cold Jeita Threshold              >*/
	PM_SCHG_CHGR_JEITA_THRESHOLD_INVALID         /** < Invalid                              >*/
}pm_schg_chgr_jeita_threshold_type;

/*===========================================================================

                        SCHG DRIVER FUNCTION PROTOTYPES

===========================================================================*/

/**
 * @brief This function enables source as command or pin
 *        controlled.
 *
 * @details
 *  This function enables source as command or pin controlled.
 *
 * @param[in] pmic_device_index.  Primary: 0 Secondary: 1
 * @param[in] chg_en_src          TRUE: Enables Pin
 *                                FALSE: Command Register
 *
 *
 * @return  pm_err_flag_type
 *          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not available on this
 *          version of the PMIC.
 *          PM_ERR_FLAG_SUCCESS               = SUCCESS.
 *
 */
pm_err_flag_type pm_schg_chgr_enable_src(uint32 device_index, boolean chg_en_src);

/**
 * @brief This function sets the command/pin polarity
 *
 * @details
 *  This function sets the command/pin polarity .
 *
 * @param[in] pmic_device_index.  Primary: 0 Secondary: 1
 * @param[in] chg_pol_low         TRUE: sets polarity low
 *                                FASLE: sets polarity high
 *
 * @return  pm_err_flag_type
 *          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not available on this
 *          version of the PMIC.
 *          PM_ERR_FLAG_SUCCESS               = SUCCESS.
 *
 */
pm_err_flag_type pm_schg_chgr_set_chg_polarity_low(uint32 device_index, boolean chg_pol_low);

 /**
 * @brief This function sets the various type of current
 *
 * @details
 *  This function sets the various type of current
 *
 * @param[in] pmic_device_index.   Primary: 0 Secondary: 1
 * @param[in] charge_current_type  Refer #pm_schg_chgr_current_type
 * @param[in] current              current value
 *
 *
 * @return  pm_err_flag_type
 *          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not available on this
 *          version of the PMIC.
 *          PM_ERR_FLAG_SUCCESS               = SUCCESS.
 *
 */
pm_err_flag_type pm_schg_chgr_set_charge_current(uint32 device_index, pm_schg_chgr_current_type charge_current_type, uint32 current);

/**
 * @brief This function enable/disable charging function
 *
 * @details
 *  This function sets the various type of charge cmd registers
 *
 * @param[in] pmic_device_index.  Primary: 0 Secondary: 1
 * @param[in] set_value           value to be set
 *                                
 *
 * @return  pm_err_flag_type 
 *          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not available on this
 *          version of the PMIC.
 *          PM_ERR_FLAG_SUCCESS               = SUCCESS.
 *
 */
pm_err_flag_type pm_schg_chgr_enable_charging(uint32 device_index, boolean enable);

/**
 * @brief This function returns the various charger status type
 * 
 * @details
 *  This function returns the various charger status type
 * 
 * @param[in] pmic_device_index.  Primary: 0 Secondary: 1
 * @param[out] charger_status      refer #pm_schg_chgr_status_type
 *                                
 *
 * @return  pm_err_flag_type 
 *          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not available on this
 *          version of the PMIC.
 *          PM_ERR_FLAG_SUCCESS               = SUCCESS.
 *
 */
pm_err_flag_type pm_schg_chgr_get_chgr_status(uint32 device_index, pm_schg_chgr_status_type *charger_status);

/**
* @brief This function This API configures the floating voltage
* 
* @details
* This API configures the floating voltage. Valid range is 3600mV to 4500 mv
* 
* @param[in] pmic_device_index.  Primary: 0 Secondary: 1
* @param[in] float_volt_mv       Valid range is 3600mV to 4500 mv
*                                
*
* @return  pm_err_flag_type 
*          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not available on this
*          version of the PMIC.
*          PM_ERR_FLAG_SUCCESS               = SUCCESS.
*
*/
pm_err_flag_type pm_schg_chgr_set_float_volt(uint32 device_index, uint32 float_volt_mv);

/**
* @brief This function sets Jeita floating voltage conpmensation cfg value
* 
* @details
*  This api sets floating voltage conpmensation code. Valid
*  value is from 0 to 63
* 
* @param[in] pmic_device_index.  Primary: 0 Secondary: 1
* @param[in] fv_comp_hot       Floating voltage compensation for JEITA soft hot zone
* @param[in] fv_comp_cold      Floating voltage compensation for JEITA soft cold zone
*                         
*
* @return  pm_err_flag_type 
*          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not available on this
*          version of the PMIC.
*          PM_ERR_FLAG_SUCCESS               = SUCCESS.
*
*/
pm_err_flag_type pm_schg_chgr_set_jeita_fvcomp_cfg(uint32 device_index, uint32 fv_comp_hot, uint32 fv_comp_cold);

/**
* @brief This function configures the fast-charger current compensation value in milliamp
* 
* @details
*  This API configures the fast-charger current compensation value in milliamp.
*  Valid values are 300 to 1200 mAmp
* 
* @param[in] pmic_device_index.  Primary: 0 Secondary: 1
* @param[in] ccomp_hot       Charge current compensation for JEITA soft hot zone
* @param[in] ccomp_cold      Charge current compensation for JEITA soft cold zone
*                                
*
* @return  pm_err_flag_type 
*          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not available on this
*          version of the PMIC.
*          PM_ERR_FLAG_SUCCESS               = SUCCESS.
*
*/
pm_err_flag_type pm_schg_chgr_set_jeita_ccomp(uint32 device_index, uint32 ccomp_hot, uint32 ccomp_cold);

/**
* @brief This function sets the JEITA temperature thresholds
*
* @details
*  This API configures the JEITA temperature thresholds for soft and hard limits.
*
* @param[in] pmic_device_index.  Primary: 0 Secondary: 1
* @param[in] pm_schg_chgr_jeita_threshold_type       Jeita zone to be configured
* @param[in] jeita_threshold      Temperature value for the JEITA zone
*
*
* @return  pm_err_flag_type
*          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not available on this
*          version of the PMIC.
*          PM_ERR_FLAG_SUCCESS               = SUCCESS.
*
*/
pm_err_flag_type pm_schg_chgr_set_jeita_threshold_value(uint32 device_index, pm_schg_chgr_jeita_threshold_type threshold_type, uint32 jeita_threshold);

/**
* @brief This function helps enable HW JEITA related parameters. 
* 
* @details
*  This function helps enable HW JEITA related parameters. 
* 
* @param[in] pmic_device_index.  Primary: 0 Secondary: 1
* @param[in] jeita_cfg_type      JEITA config type see pm_schg_chgr_jeita_cfg_type for details
* @param[in] enable              Enables the config; 0: FALSE, 1: TRUE 
*
*
* @return  pm_err_flag_type 
*          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not available on this
*          version of the PMIC.
*          PM_ERR_FLAG_SUCCESS               = SUCCESS.
*
*/
pm_err_flag_type pm_schg_chgr_jeita_en_cfg(uint32 device_index, pm_schg_chgr_jeita_cfg_type jeita_cfg_type, boolean enable);

/**
* @brief This function provides SCHG charger PMIC info.
*
* @details
*  This function provides SCHG charger PMIC info.
*
* @param[out] pmic_index         On what PMIC Index, SCHG charger resides.
* @param[out] slave_id           On what SlaveID for given PMIC, SCHG charger resides.
*
* @return  pm_err_flag_type
*          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not available.
*          PM_ERR_FLAG_INVALID_PARAMETER     = Invalid parameters.
*          PM_ERR_FLAG_SUCCESS               = SUCCESS.
*/
pm_err_flag_type pm_schg_chgr_get_schg_pmic_info(uint8 *pmic_index, uint8 *slave_id, uint8* charger_count, uint8 max_charger);

#endif /* __PM_SCHG_CHGR_H__ */

