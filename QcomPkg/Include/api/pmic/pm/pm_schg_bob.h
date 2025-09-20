#ifndef __PM_SCHG_BOB_H__
#define __PM_SCHG_BOB_H__

/*! \file
*  \n
*  \brief  pm_schg_bob.h PMIC-SMBC MODULE RELATED DECLARATION 
*  \details  This header file contains functions and variable declarations 
*  to support Qualcomm PMIC SCHG (Switch-Mode Battery Charger) module. The 
*  Switched-Mode Battery Charger (SCHG) module includes a buck regulated 
*  battery charger with integrated switches. The SCHG module, along with the 
*  Over Voltage Proection (OVP) module will majorly be used by charger 
*  appliation for charging Li-Ion batteries with high current (up to 2A).
*  \n &copy; Copyright 2017-2018 QUALCOMM Technologies Inc, All Rights Reserved
*/

/* =======================================================================
                                Edit History
This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.


when       who    what, where, why
--------   ---    ---------------------------------------------------------- 
06/20/18   cs     Initial version. 
========================================================================== */
#include "com_dtypes.h"
#include "pm_err_flags.h"
#include "pm_resources_and_types.h"

/*===========================================================================

                        TYPE DEFINITIONS 

===========================================================================*/


/*===========================================================================

                 SCHG DRIVER FUNCTION PROTOTYPES

===========================================================================*/



/**
* @brief This function returns if this charging port is active
* 
* @param[in]   device_index. Primary: 0. Secondary: 1
* @param[out]  isActive:  1 the port is active, 0, port not active.
*
* @return  pm_err_flag_type 
*          PM_ERR_FLAG__FEATURE_NOT_SUPPORTED = Feature not available on this
*          version of the PMIC.
*          PM_ERR_FLAG__SUCCESS               = SUCCESS.
*
*/
pm_err_flag_type pm_schg_bob_is_port_active(uint32 pmic_device, boolean* is_active);



#endif /* __PM_SCHG_BOB_H__ */

