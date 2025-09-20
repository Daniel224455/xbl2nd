#ifndef __PM_LCDB_H__
#define __PM_LCDB_H__

/*! \file
*  \n
*  \brief  pm_lcdb.h PMIC-LCDB MODULE RELATED DECLARATION 
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
02/13/18   al      Initial version. 
========================================================================== */
#include "com_dtypes.h"
#include "pm_err_flags.h"
#include "pm_resources_and_types.h"

/*===========================================================================

                        TYPE DEFINITIONS 

===========================================================================*/

typedef enum
{
   PM_LCDB_ENABLE_CTL_MODULE_EN,
   PM_LCDB_ENABLE_CTL_HWEN_RDY,
   PM_LCDB_ENABLE_CTL_INVALID_TYPE,
}pm_lcdb_enable_ctl_type;

typedef enum
{
   PM_LCDB_PULLDOWN_CTL_ENABLE,
   PM_LCDB_PULLDOWN_CTL_STRENGHT,
   PM_LCDB_PULLDOWN_CTL_INVALID_TYPE,
}pm_lcdb_pulldown_ctl_type;


/*===========================================================================

                 SMBCHG OTG DRIVER FUNCTION PROTOTYPES

===========================================================================*/

/**
 * @brief This function allows to select default voltage or set 
 *        output voltage. If Symmetry is enabled then NCP will symmetrical to LDO in negative direction
 *  
 * @details
 *  This API allows to select default voltage or set output 
 *  voltage for LCDB
 * 
 * @param[in] pmic_device_index.   Primary: 0 Secondary: 1
 * @param[in] ldo_mv               +Ve LDO volt in mv. Range from 4000 to 6000 mV 
 * @param[in] ncp_mv               -Ve NCP volt in mv. Range from 4000 to 6000 mV  
 * @param[in] symmetry_ok          TRUE to enable, will make NCP same as LDO. FALSE will set LDO and NCP independently
 *                                
 *
 * @return  pm_err_flag_type 
 *          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not availcdble on this
 *          version of the PMIC.
 *          PM_ERR_FLAG_SUCCESS               = SUCCESS.
 *
 */
pm_err_flag_type pm_lcdb_set_output_volt(uint32 device_index, int32 ldo_mv, int32 ncp_mv, boolean symmetry_ok);


/**
 * @brief This function enables/disables HW debounce 
 *        output voltage for Vdisp
 *  
 * @details
 *  This API allows to select default voltage or set output 
 *  voltage for LCDB
 * 
 * @param[in] pmic_device_index.   Primary: 0 Secondary: 1
 * @param[in] enable               TRUE to enable HW debounce and FALSE otherwise
 *                                
 *
 * @return  pm_err_flag_type 
 *          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not availcdble on this
 *          version of the PMIC.
 *          PM_ERR_FLAG_SUCCESS               = SUCCESS.
 *
 */
pm_err_flag_type pm_lcdb_set_hwen_debounce(uint32 device_index, boolean enable);

/**
 * @brief This function configures enable control
 * 
 * @param[in] pmic_device_index.   Primary: 0 Secondary: 1
 * @param[in] ctl_type             control type to enable/disable. Refer #pm_lcdb_enable_ctl_type for more info
 * @param[in] enable               TRUE to enable configuration and FALSE otherwise
 *                                
 *
 * @return  pm_err_flag_type 
 *          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not availcdble on this
 *          version of the PMIC.
 *          PM_ERR_FLAG_SUCCESS               = SUCCESS.
 *
 */
pm_err_flag_type pm_lcdb_config_enable_ctrl(uint32 device_index, pm_lcdb_enable_ctl_type ctl_type, boolean enable);


/**
 * @brief This function configures pull down control
 * 
 * @param[in] pmic_device_index.   Primary: 0 Secondary: 1
 * @param[in] ctl_type             control type to enable/disable. Refer #pm_lcdb_enable_ctl_type for more info
 * @param[in] ctrl               TRUE to enable configuration and FALSE otherwise
 *                                
 *
 * @return  pm_err_flag_type 
 *          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not availcdble on this
 *          version of the PMIC.
 *          PM_ERR_FLAG_SUCCESS               = SUCCESS.
 *
 */
pm_err_flag_type pm_lcdb_config_pulldown_ctrl(uint32 device_index, pm_lcdb_pulldown_ctl_type ctrl_type, boolean ctrl);


/**
 * @brief This function returns LCDB VREG_OK status
 * 
 * @param[in] pmic_device_index.   Primary: 0 Secondary: 1
 * @param[out] vreg_ok             Pointer to store vreg_ok status
 *                                
 *
 * @return  pm_err_flag_type 
 *          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not availcdble on this
 *          version of the PMIC.
 *          PM_ERR_FLAG_SUCCESS               = SUCCESS.
 *
 */
pm_err_flag_type pm_lcdb_vreg_ok(uint32 device_index, boolean *vreg_ok);

#endif /* __PM_LCDB_H__*/

