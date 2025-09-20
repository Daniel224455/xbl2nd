#ifndef __PM_DEVICE_H__
#define __PM_DEVICE_H__

/*! \file pm_device.h
*  \n
*  \brief This header file contains enums and helper function declarations needed
*  \n during PMIC device initialization .
*  \n
*  \n &copy; Copyright 2016-2018 QUALCOMM Technologies Incorporated, All Rights Reserved
*/
/* =======================================================================
Edit History
This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.


when       who     what, where, why
--------   ---     ----------------------------------------------------------
08/03/16   akm     Created
 
========================================================================== */
/*===========================================================================

                     INCLUDE FILES 

===========================================================================*/

#include "pm_err_flags.h"
#include "pm_boot.h"
#include "pm_version.h"

/*===========================================================================

                        TYPE DEFINITIONS 

===========================================================================*/

/*
 * pm_periph_owner
 *
 * A type to choose the interrupt and peripehral owner.
 */
typedef enum 
{
  PM_APPS_HLOS_OWNER = 0,
  PM_APPS_TZ_OWNER,
  PM_MSS_OWNER,
  PM_LPASS_OWNER,
  PM_AOP_OWNER,
  PM_SSC_OWNER,
  PM_SECPROC_OWNER
} pm_periph_owner;

/*===========================================================================

                        FUNCTION PROTOTYPES 

===========================================================================*/

/*! \brief This function executes the pmic device setup needed to communicate with PMICs
 *   \brief  It initilizes SPMI bus and detects PMIC on target
 *  \return Error flag.
 *
 */
pm_err_flag_type pm_device_setup(void);


/*! \brief This function executes the pmic device setup needed to communicate with PMICs
 *   \brief  It initilizes for dynamic SPMI channel configuration
 *  \return Error flag.
 *
 */
pm_err_flag_type pm_spmi_dynamic_chnl_init(void);


/*! \brief This function executes the pmic device setup needed to communicate with PMICs
 *   \brief  It initilizes for static SPMI channel, PVC Port and VRM Port configuration.
 *  \return Error flag.
 *
 */
pm_err_flag_type pm_spmi_static_chnl_init(void);

/** 
 * @name pm_comm_disable_spmi 
 *  
 * @brief This function is called to disable spmi communication 
 *        with PMIC
 * @param[in] slave_id :  SPMI slave id of specific PMIC 
 *  
 *  
 *
 * @return  pm_err_flag_type 
 *          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Error in
 *          returning slave id.
 *          PM_ERR_FLAG_SUCCESS = SUCCESS.
 */
pm_err_flag_type pm_spmi_disable_comm(uint8 slave_id);

/** 
 * @name pm_is_pmic_present 
 *  
 * @brief This function check PMIC presence
 * 
 * @param[in] uint8 
 *
 * @return 
 *     TRUE : PMIC is Present
 *     FALSE: PMIC is NOT Present
 *
 */
boolean 
pm_is_pmic_present(uint8 pmic_index);

/** 
 * @name pm_is_target_pre_silicon 
 *  
 * @brief This function checks if target is pre-silicon 
 * 
 * @param[in] uint8 
 *
 * @return 
 *    TRUE  : Tartet is Pre-Silicon
 *    FALSE : Target is NOT Pre-Silicon
 *
 */
boolean 
pm_is_target_pre_silicon(void);


/** 
 * @name pm_config_lock_by_bit 
 *  
 * @brief This function writes to lock by bit register to lock 
 *        peripheral
 *
 * @return  pm_err_flag_type 
 *
 */
pm_err_flag_type
pm_config_lock_by_bit(void);

#endif // __PM_DEVICE_H__
