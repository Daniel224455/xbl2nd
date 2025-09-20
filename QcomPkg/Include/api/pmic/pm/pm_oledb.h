#ifndef __PM_OLEDB_H__
#define __PM_OLEDB_H__

/*! \file
*  \n
*  \brief  pm_uefi_lcdb.h PMIC-OLEDB MODULE RELATED DECLARATION 
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
  PM_OLEDB_MODE_CFG_BOOST,      //For Boost only Config
  PM_OLEDB_MODE_CFG_BOOST_LDO,  //For Boost+LDO Config
  PM_OLEDB_MODE_CFG_INVALID     //Invalid
}pm_oledb_mode_cfg_type;

/*===========================================================================

                 OLEDB DRIVER FUNCTION PROTOTYPES

===========================================================================*/

/**
 * @brief This function sets OLEDB module ready to be enabled
 * @details
 * 
 * @param[in] device_index.          Primary: 0 Secondary: 1
 * @param[in] oledb_rdy              TRUE: OLEDB module ready to be enabled. FALSE: Otherwise
 *                                
 *
 * @return  pm_err_flag_type 
 *          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not available on this
 *          version of the PMIC.
 *          PM_ERR_FLAG_SUCCESS               = SUCCESS.
 *
 */
pm_err_flag_type pm_oledb_module_ready_to_be_enabled(uint32 device_index, boolean oledb_rdy);


/**
 * @brief This function enables OLEDB module
 * @details
 * 
 * @param[in] device_index.          Primary: 0 Secondary: 1
 * @param[in] enable                 TRUE: Enable OLEDB module. FALSE: Otherwise
 *                                
 *
 * @return  pm_err_flag_type 
 *          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not available on this
 *          version of the PMIC.
 *          PM_ERR_FLAG_SUCCESS               = SUCCESS.
 *
 */
pm_err_flag_type pm_oledb_enable_module(uint32 device_index, boolean enable);

/**
 * @brief This function configures external pin control to enable OLEDB module
 * @details
 * 
 * @param[in] device_index.          Primary: 0 Secondary: 1
 * @param[in] enable                 TRUE: Enable OLEDB ext pin control. FALSE: Otherwise
 *                                
 *
 * @return  pm_err_flag_type 
 *          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not available on this
 *          version of the PMIC.
 *          PM_ERR_FLAG_SUCCESS               = SUCCESS.
 *
 */
pm_err_flag_type pm_oledb_enable_ext_pin_ctl(uint32 device_index, boolean enable);


/**
 * @brief This function configures external pin control to enable OLEDB module
 * @details
 * 
 * @param[in] device_index.          Primary: 0 Secondary: 1
 * @param[in] enable                 TRUE: Enable OLEDB ext pin control. FALSE: Otherwise
 *                                
 *
 * @return  pm_err_flag_type 
 *          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not available on this
 *          version of the PMIC.
 *          PM_ERR_FLAG_SUCCESS               = SUCCESS.
 *
 */
pm_err_flag_type pm_oledb_set_output_volt(uint32 device_index, uint32 vout_mv);

/**
 * @brief This configures Oledb default voltage output
 * 
 * @details
 *  This API configures Oledb default voltag output
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
pm_oledb_config_default_volt(uint32 device_index, uint32 milli_volt);

/**
 * @brief This configures Oledb to have either boost only config or boost+LDO
 * 
 * @details
 *  This configures Oledb to have either boost only config or boost +LDO
 * 
 * @param[in] device_index        : Primary: 0, Secondary: 1 etc
 * @param[in] mode                : see enum pm_oledb_mode_cfg_type
 * @return  pm_err_flag_type 
 *          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not available on this
 *          version of the PMIC.
 *          PM_ERR_FLAG_SUCCESS               = SUCCESS.
 */
pm_err_flag_type
pm_oledb_mode_config(uint32 device_index, pm_oledb_mode_cfg_type mode);

#endif /* __PM_OLEDB_H__*/

