#ifndef __PMIC_SBL_BOOT_TARGET_H__
#define __PMIC_SBL_BOOT_TARGET_H__

/*! \file pm_sbl_boot_target.h
*  \n
*  \brief This file contains PMIC device initialization function and globals declaration.
*  \n
*  \n &copy; Copyright 2016-2018 Qualcomm Technologies Inc, All Rights Reserved
*/
/* =======================================================================
Edit History
This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.


when       who     what, where, why
--------   ---     ----------------------------------------------------------
09/13/17   aab     Added target specific fn: pm_spmi_tgt_vrm_init() 
02/05/16   aab     Added active/retentsion level config reg address
02/05/16   aab     Creation
========================================================================== */
/*===========================================================================

                     INCLUDE FILES 

===========================================================================*/

#include "com_dtypes.h"
#include "pm_err_flags.h"
#include "SpmiBusCfg.h"



/*===========================================================================

                        TYPE DEFINITIONS 

===========================================================================*/

#define SDAM3_SLAVE_ADDR                              0x0
#define SDAM3_BASE_ADDRESS                            0xB200
#define SDAM3_VSET1_WLAN_ACTIVE_ADDR_LB               (SDAM3_BASE_ADDRESS + 0x55)
#define SDAM3_VSET1_WLAN_ACTIVE_ADDR_UB               (SDAM3_BASE_ADDRESS + 0x56)
#define SDAM3_VSET2_WLAN_RET_ADDR_LB                  (SDAM3_BASE_ADDRESS + 0x57)
#define SDAM3_VSET2_WLAN_RET_ADDR_UB                  (SDAM3_BASE_ADDRESS + 0x58)


/*===========================================================================

                        FUNCTION PROTOTYPES 

===========================================================================*/

pm_err_flag_type 
pm_spmi_tgt_vrm_init(SpmiBusCfg_Ppid *pm_vrm_rsrc_arr, uint32 *vrm_cfg_index, uint8 vrm_rsrc_arr_size);

pm_err_flag_type pm_tgt_mgpi_cfg(SpmiBusCfg_MgpiPortCfg** mgpi_port_cfg_ptr);

#endif //__PMIC_SBL_BOOT_TARGET_H__
