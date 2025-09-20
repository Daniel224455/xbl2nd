#ifndef __PM_QGAUGE_H__
#define __PM_QGAUGE_H__

/*! \file
*  \n
*  \brief  pm_qgauge.h PMIC-SMBC MODULE RELATED DECLARATION 
*  \details  This header file contains functions and variable declarations 
*  to support Qualcomm qgauge module.
*  \n &copy; Copyright 2018 QUALCOMM Technologies Incorporated, All Rights Reserved.
*/

/* =======================================================================
                                Edit History
This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.


when       who    what, where, why
--------   ---    ---------------------------------------------------------- 
02/15/18   sv     Initial version. 
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

/*! \enum pm_qgauge_irq_bit_field_type
   \brief different types of irq bit fields of by smbb irq module
 */
typedef enum {
   PM_QGAUGE_BATTERY_MISSING_INT_RT_STS,
   PM_QGAUGE_VBAT_LOW_INT_RT_STS,
   PM_QGAUGE_VBAT_EMPTY_INT_RT_STS,
   PM_QGAUGE_FIFO_UPDATE_DONE_INT_RT_STS,
   PM_QGAUGE_GOOD_OCV_INT_RT_STS,
   PM_QGAUGE_FSM_STAT_CHG_INT_RT_STS,
   PM_QGAUGE_QG_EVENT_INT_RT_STS,
   PM_QGAUGE_IRQ_INVALID
}pm_qgauge_irq_type;


typedef struct
{
  boolean                     qg_ok;
  boolean                     s5_ocv_validate_done;
  boolean                     pre_esr_meas_done;
  boolean                     esr_meas_done;
  boolean                     fsm_forced;
  boolean                     batfet_open;
  boolean                     battery_present;
  
  boolean                     adc_conv_fault_occured;
  boolean                     good_ocv;
  boolean                     forced_disable;
  
}pm_qgauge_status_type;


/*===========================================================================

                        SCHG DRIVER FUNCTION PROTOTYPES

===========================================================================*/


/**
* @brief This function enables irq
*
* @details
*  This function enables irq
*
* @param[in] pmic_device_index.  Primary: 0 Secondary: 1
* @param[in] irq                 pm_qgauge_irq_type
*
*
* @return  pm_err_flag_type
*          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not available on this
*          version of the PMIC.
*          PM_ERR_FLAG_SUCCESS               = SUCCESS.
*
*/
pm_err_flag_type pm_qgauge_irq_enable(uint32 device_index, pm_qgauge_irq_type irq, boolean enable);


/**
* @brief This function clears the SCHG irq
*
* @details
*  This function clears the SCHG irq
*
* @param[in] pmic_device_index.  Primary: 0 Secondary: 1
* @param[in] irq                 pm_qgauge_irq_type
*
*
* @return  pm_err_flag_type
*          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not available on this
*          version of the PMIC.
*          PM_ERR_FLAG_SUCCESS               = SUCCESS.
*
*/
pm_err_flag_type pm_qgauge_irq_clear(uint32  device_index, pm_qgauge_irq_type irq);


/**
* @brief This function configures the SCHG irq trigger
*
* @details
*  This function configures the SCHG irq trigger
*
* @param[in] pmic_device_index.  Primary: 0 Secondary: 1
* @param[in] irq                 pm_qgauge_irq_type
*
*
* @return  pm_err_flag_type
*          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not available on this
*          version of the PMIC.
*          PM_ERR_FLAG_SUCCESS               = SUCCESS.
*
*/

pm_err_flag_type pm_qgauge_irq_set_trigger(uint32 device_index, pm_qgauge_irq_type irq, pm_irq_trigger_type trigger);


/**
* @brief This function configures the SCHG for irq
*
* @details
*  This function configures the SCHG for irq
*
* @param[in] pmic_device_index.  Primary: 0 Secondary: 1
* @param[in] irq                 pm_qgauge_irq_type
*
*
* @return  pm_err_flag_type
*          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not available on this
*          version of the PMIC.
*          PM_ERR_FLAG_SUCCESS               = SUCCESS.
*
*/
pm_err_flag_type pm_qgauge_irq_status(uint32 device_index, pm_qgauge_irq_type irq, pm_irq_status_type type, boolean *status);

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
pm_err_flag_type pm_qgauge_enable_src(uint32 device_index, boolean chg_en_src);


/**
* @brief This function provides Qgauge status info.
*
* @details
*  This function provides Qgauge PMIC info.
*
* @param[out] pmic_index         On what PMIC Index, Qgauge resides.
* @param[out] qgauge_status      Qgauge info in pm_qgauge_status_type.
*
* @return  pm_err_flag_type
*          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not available.
*          PM_ERR_FLAG_INVALID_PARAMETER     = Invalid parameters.
*          PM_ERR_FLAG_SUCCESS               = SUCCESS.
*/
pm_err_flag_type pm_qgauge_get_status(uint32 device_index, pm_qgauge_status_type* qgauge_status);

/**
* @brief This function gets the battery voltage.
*
* @details
*  This function provides Qgauge PMIC info.
*
* @param[out] pmic_index         On what PMIC Index, Qgauge resides.
* @param[out] vbat_inmv          Battery voltage in mV.
*
* @return  pm_err_flag_type
*          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not available.
*          PM_ERR_FLAG_INVALID_PARAMETER     = Invalid parameters.
*          PM_ERR_FLAG_SUCCESS               = SUCCESS.
*/
pm_err_flag_type pm_qgauge_get_vbat(uint32 device_index, uint32* vbat_inmv);

/**
* @brief This function gets Battery current.
*
* @details
*  This function provides Qgauge PMIC info.
*
* @param[out] pmic_index         On what PMIC Index, Qgauge resides.
* @param[out] ibat_inma          Battery current in mA.
*
* @return  pm_err_flag_type
*          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not available.
*          PM_ERR_FLAG_INVALID_PARAMETER     = Invalid parameters.
*          PM_ERR_FLAG_SUCCESS               = SUCCESS.
*/
pm_err_flag_type pm_qgauge_get_ibat(uint32 device_index, int32* ibat_inma);

/**
* @brief This function sets battery empty threshold.
*
* @details
*  This function provides Qgauge PMIC info.
*
* @param[out] pmic_index         On what PMIC Index, Qgauge resides.
* @param[out] vbatEmptyThreshold Battery empty threshold in mV.
*
* @return  pm_err_flag_type
*          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not available.
*          PM_ERR_FLAG_INVALID_PARAMETER     = Invalid parameters.
*          PM_ERR_FLAG_SUCCESS               = SUCCESS.
*/
pm_err_flag_type pm_qgauge_set_vbat_empty_threshold(uint32 device_index, uint32 vbatEmptyThreshold);

/**
* @brief This function sets battery low threshold.
*
* @details
*  This function provides Qgauge PMIC info.
*
* @param[out] pmic_index         On what PMIC Index, Qgauge resides.
* @param[out] vbatlowThreshold   Battery low voltage threshold.
*
* @return  pm_err_flag_type
*          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not available.
*          PM_ERR_FLAG_INVALID_PARAMETER     = Invalid parameters.
*          PM_ERR_FLAG_SUCCESS               = SUCCESS.
*/
pm_err_flag_type pm_qgauge_set_vbat_low_threshold(uint32 device_index, uint32 vbatlowThreshold);


/**
* @brief This function provides Qgauge PMIC info.
*
* @details
*  This function provides Qgauge PMIC info.
*
* @param[out] pmic_index         On what PMIC Index, Qgauge resides.
* @param[out] slave_id           On what SlaveID for given PMIC, Qgauge resides.
*
* @return  pm_err_flag_type
*          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not available.
*          PM_ERR_FLAG_INVALID_PARAMETER     = Invalid parameters.
*          PM_ERR_FLAG_SUCCESS               = SUCCESS.
*/
pm_err_flag_type pm_qgauge_get_qgauge_pmic_info(uint8 *pmic_index, uint8 *slave_id);


#endif /* __PM_qgauge_H__ */

