#ifndef __PM_BMD_H__
#define __PM_BMD_H__

/*! \file  pm_bmd.h 
 *  \n
 *  \brief  PMIC-BMD MODULE RELATED DECLARATION
 *  \details  This file contains functions and variable declarations to support 
 *   the PMIC BMD module.
 *
 *  \n &copy; Copyright (c) 2017-2019 QUALCOMM Technologies Incorporated. All Rights Reserved.
 */

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.


when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
02/07/18   xmp     Implement BMD driver
11/20/17   richaa  Created
===========================================================================*/

/*===========================================================================
                INCLUDE FILES FOR MODULE
===========================================================================*/
#include "pm_err_flags.h"
#include "com_dtypes.h"
#include "pm_resources_and_types.h"


/*=============================================================================
                    LOCAL VARIABLE DEFINITIONS
=============================================================================*/
typedef enum 
{
  PM_BMD_SRC_BATT_ID,
  PM_BMD_SRC_BATT_THERM,
  PM_BMD_SRC_INVALID,
} pm_bmd_src_ctl_sel_type;

typedef enum
{
    PM_BMD_BAT_MISS_STS_RAW,
    PM_BMD_BAT_MISS_STS_DEB,
    PM_BMD_BAT_MISS_STS_INVALID,
} pm_bmd_bat_miss_sts_type;

typedef enum
{
    PM_BMD_MODE_IDLE,
    PM_BMD_MODE_CONTINUOUS,
    PM_BMD_MODE_SLEEP,
    PM_BMD_MODE_OFF,
    PM_BMD_MODE_INVALID
} pm_bmd_mode_type;

typedef enum
{
    PM_BMD_STATE_IDLE,
    PM_BMD_STATE_PWR_UP,
    PM_BMD_STATE_RMV_BLANK,
    PM_BMD_STATE_ACTIVE,
    PM_BMD_STATE_PWR_DN,
    PM_BMD_STATE_TIMER_EN,
    PM_BMD_STATE_ADC_ACTIVE,
    PM_BMD_STATE_INVALID
} pm_bmd_state_type;

typedef enum
{
    PM_BMD_SAMPLE_RATE_0_MS,    // 0ms means continuous
    PM_BMD_SAMPLE_RATE_1_MS,
    PM_BMD_SAMPLE_RATE_2_MS,
    PM_BMD_SAMPLE_RATE_4_MS,
    PM_BMD_SAMPLE_RATE_8_MS,
    PM_BMD_SAMPLE_RATE_16_MS,
    PM_BMD_SAMPLE_RATE_32_MS,
    PM_BMD_SAMPLE_RATE_64_MS,
    PM_BMD_SAMPLE_RATE_128_MS,
    PM_BMD_SAMPLE_RATE_256_MS,
    PM_BMD_SAMPLE_RATE_512_MS,
    PM_BMD_SAMPLE_RATE_1024_MS,
    PM_BMD_SAMPLE_RATE_2048_MS,
    PM_BMD_SAMPLE_RATE_4096_MS,
    PM_BMD_SAMPLE_RATE_8192_MS,
    PM_BMD_SAMPLE_RATE_16384_MS,
    PM_BMD_SAMPLE_RATE_INVALID,
} pm_bmd_sample_rate_type;

typedef enum
{
    PM_BMD_DEBOUNCE_0_US,
    PM_BMD_DEBOUNCE_61_US,
    PM_BMD_DEBOUNCE_122_US,
    PM_BMD_DEBOUNCE_183_US,
    PM_BMD_DEBOUNCE_244_US,
    PM_BMD_DEBOUNCE_305_US,
    PM_BMD_DEBOUNCE_366_US,
    PM_BMD_DEBOUNCE_427_US,
    PM_BMD_DEBOUNCE_488_US,
    PM_BMD_DEBOUNCE_549_US,
    PM_BMD_DEBOUNCE_610_US,
    PM_BMD_DEBOUNCE_671_US,
    PM_BMD_DEBOUNCE_733_US,
    PM_BMD_DEBOUNCE_794_US,
    PM_BMD_DEBOUNCE_855_US,
    PM_BMD_DEBOUNCE_916_US,
    PM_BMD_DEBOUNCE_INVALID,
} pm_bmd_debounce_type;

typedef enum
{
    // the sufix P means percentage of aVdd
    // For example PM_BMD_COMPARATOR_THRESHOLD_36P means threshold is (36% * aVdd)
    PM_BMD_COMPARATOR_THRESHOLD_36P, // 0
    PM_BMD_COMPARATOR_THRESHOLD_40P, // 1
    PM_BMD_COMPARATOR_THRESHOLD_44P, // 2
    PM_BMD_COMPARATOR_THRESHOLD_48P, // 3
    PM_BMD_COMPARATOR_THRESHOLD_52P, // 4
    PM_BMD_COMPARATOR_THRESHOLD_56P, // 5
    PM_BMD_COMPARATOR_THRESHOLD_60P, // 6
    PM_BMD_COMPARATOR_THRESHOLD_64P, // 7
    PM_BMD_COMPARATOR_THRESHOLD_68P, // 8
    PM_BMD_COMPARATOR_THRESHOLD_72P, // 9
    PM_BMD_COMPARATOR_THRESHOLD_76P, // 10
    PM_BMD_COMPARATOR_THRESHOLD_80P, // 11
    PM_BMD_COMPARATOR_THRESHOLD_84P, // 12
    PM_BMD_COMPARATOR_THRESHOLD_88P, // 13
    PM_BMD_COMPARATOR_THRESHOLD_92P, // 14
    PM_BMD_COMPARATOR_THRESHOLD_96P, // 15
    PM_BMD_COMPARATOR_THRESHOLD_INVALID,
} pm_bmd_comparator_threshold_type;

/*===========================================================================
                          FUNCTION DEFINITIONS
===========================================================================*/

/**
 * @brief This function selects the input for the battery removal comparator
 * 
 * @param[in]  pmic_device_index:
 *               0 for primary pmic
 *               1 for secondary pmic
 * @param[in] bmd_sel
 *
 * @return  pm_err_flag_type 
 *          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not available on this
 *          version of the PMIC.
 *          PM_ERR_FLAG_SUCCESS               = SUCCESS.
 *
 * DEPENDENCY: pm_driver_init() must be called in advance
 */
pm_err_flag_type pm_bmd_src_ctl
(
    uint32 pmic_device,
    pm_bmd_src_ctl_sel_type bmd_sel
);

/**
 * @brief This function get battery missing run-time status
 * 
 * @param[in]  pmic_device_index:
 *               0 for primary pmic
 *               1 for secondary pmic
 * @param[in] bat_miss_type:
 *               raw or debounce
 * @param[out] missing:
 *               whether battery is gone
 * @return  pm_err_flag_type 
 *          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not available on this
 *          version of the PMIC.
 *          PM_ERR_FLAG_SUCCESS               = SUCCESS.
 *
 * DEPENDENCY: pm_driver_init() must be called in advance
 */
pm_err_flag_type pm_bmd_get_battery_missing_rt_status
(
    uint32 pmic_device,
    pm_bmd_bat_miss_sts_type bat_miss_type,
    boolean *missing
);

/**
 * @brief This function get BMD state.
 * 
 * @param[in]  pmic_device_index:
 *               0 for primary pmic
 *               1 for secondary pmic
 * @param[out] state:
 *               BMD state
 * @return  pm_err_flag_type 
 *          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not available on this
 *          version of the PMIC.
 *          PM_ERR_FLAG_SUCCESS               = SUCCESS.
 *
 * DEPENDENCY: pm_driver_init() must be called in advance
 */
pm_err_flag_type pm_bmd_get_state
(
    uint32 pmic_device,
    pm_bmd_state_type *state
);

/**
 * @brief This function get BMD mode.
 * 
 * @param[in]  pmic_device_index:
 *               0 for primary pmic
 *               1 for secondary pmic
 * @param[out] mode:
 *               BMD mode
 * @return  pm_err_flag_type 
 *          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not available on this
 *          version of the PMIC.
 *          PM_ERR_FLAG_SUCCESS               = SUCCESS.
 *
 * DEPENDENCY: pm_driver_init() must be called in advance
 */
pm_err_flag_type pm_bmd_get_mode
(
    uint32 pmic_device,
    pm_bmd_mode_type *mode
);

/**
 * @brief This function force BMD into a particular mode.
 * 
 * @param[in]  pmic_device_index:
 *               0 for primary pmic
 *               1 for secondary pmic
 * @param[in] mode:
 *               BMD mode
 * @return  pm_err_flag_type 
 *          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not available on this
 *          version of the PMIC.
 *          PM_ERR_FLAG_SUCCESS               = SUCCESS.
 *
 * DEPENDENCY: pm_driver_init() must be called in advance
 */
pm_err_flag_type pm_bmd_set_force_mode
(
    uint32 pmic_device,
    pm_bmd_mode_type mode
);

/**
 * @brief This function set BMD rate for sleep mode.
 * 
 * @param[in]  pmic_device_index:
 *               0 for primary pmic
 *               1 for secondary pmic
 * @param[in] rate:
 *               BMD sleep rate
 * @return  pm_err_flag_type 
 *          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not available on this
 *          version of the PMIC.
 *          PM_ERR_FLAG_SUCCESS               = SUCCESS.
 *
 * DEPENDENCY: pm_driver_init() must be called in advance
 */
pm_err_flag_type pm_bmd_set_sleep_rate
(
    uint32 pmic_device,
    pm_bmd_sample_rate_type rate
);

/**
 * @brief This function set BMD rate for off mode.
 * 
 * @param[in]  pmic_device_index:
 *               0 for primary pmic
 *               1 for secondary pmic
 * @param[in] rate:
 *               BMD off rate
 * @return  pm_err_flag_type 
 *          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not available on this
 *          version of the PMIC.
 *          PM_ERR_FLAG_SUCCESS               = SUCCESS.
 *
 * DEPENDENCY: pm_driver_init() must be called in advance
 */
pm_err_flag_type pm_bmd_set_off_rate
(
    uint32 pmic_device,
    pm_bmd_sample_rate_type rate
);

/**
 * @brief This function set BMD negative edge debounce.
 * 
 * @param[in]  pmic_device_index:
 *               0 for primary pmic
 *               1 for secondary pmic
 * @param[in] debounce:
 *               BMD negative edge debounce
 * @return  pm_err_flag_type 
 *          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not available on this
 *          version of the PMIC.
 *          PM_ERR_FLAG_SUCCESS               = SUCCESS.
 *
 * DEPENDENCY: pm_driver_init() must be called in advance
 */
pm_err_flag_type pm_bmd_set_bat_gone_negative_edge_debounce
(
    uint32 pmic_device,
    pm_bmd_debounce_type debounce
);

/**
 * @brief This function set BMD posative edge debounce.
 * 
 * @param[in]  pmic_device_index:
 *               0 for primary pmic
 *               1 for secondary pmic
 * @param[in] debounce:
 *               BMD posative edge debounce
 * @return  pm_err_flag_type 
 *          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not available on this
 *          version of the PMIC.
 *          PM_ERR_FLAG_SUCCESS               = SUCCESS.
 *
 * DEPENDENCY: pm_driver_init() must be called in advance
 */
pm_err_flag_type pm_bmd_set_bat_gone_posative_edge_debounce
(
    uint32 pmic_device,
    pm_bmd_debounce_type debounce
);

/**
 * @brief This function set battery removal comparator threshold. The threshold is set to a ratio
 * of the aVdd rail at 1.8V
 * 
 * @param[in]  pmic_device_index:
 *               0 for primary pmic
 *               1 for secondary pmic
 * @param[in] threshold:
 *               battery removal comparator threshold
 * @return  pm_err_flag_type 
 *          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not available on this
 *          version of the PMIC.
 *          PM_ERR_FLAG_SUCCESS               = SUCCESS.
 *
 * DEPENDENCY: pm_driver_init() must be called in advance
 */
pm_err_flag_type pm_bmd_set_battery_removal_comparator_threshold
(
    uint32 pmic_device,
    pm_bmd_comparator_threshold_type threshold
);

/**
 * @brief This function enable/disable BMD during VADC conversion of BAT_ID or BAT_THERM
 * 
 * @param[in]  pmic_device_index:
 *               0 for primary pmic
 *               1 for secondary pmic
 * @param[in] enable:
 *               enable or disable
 * @return  pm_err_flag_type 
 *          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not available on this
 *          version of the PMIC.
 *          PM_ERR_FLAG_SUCCESS               = SUCCESS.
 *
 * DEPENDENCY: pm_driver_init() must be called in advance
 */
pm_err_flag_type pm_bmd_enable_bmd_in_adc_conv
(
    uint32 pmic_device,
    boolean enable
);

/**
 * @brief This function enable/disable BMD functionality
 * 
 * @param[in]  pmic_device_index:
 *               0 for primary pmic
 *               1 for secondary pmic
 * @param[in] enable:
 *               enable or disable
 * @return  pm_err_flag_type 
 *          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not available on this
 *          version of the PMIC.
 *          PM_ERR_FLAG_SUCCESS               = SUCCESS.
 *
 * DEPENDENCY: pm_driver_init() must be called in advance
 */
pm_err_flag_type pm_bmd_enable
(
    uint32 pmic_device,
    boolean enable
);

#endif

