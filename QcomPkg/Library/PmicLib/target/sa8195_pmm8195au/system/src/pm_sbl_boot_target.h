#ifndef __PMIC_SBL_BOOT_TARGET_H__
#define __PMIC_SBL_BOOT_TARGET_H__

/*! \file pm_sbl_boot_target.h
*  \n
*  \brief This file contains PMIC device initialization function and globals declaration.
*  \n
*  \n &copy; Copyright 2016-2019 Qualcomm Technologies Inc, All Rights Reserved
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

/*===========================================================================

                        FUNCTION PROTOTYPES 

===========================================================================*/

pm_err_flag_type 
pm_spmi_tgt_vrm_init(SpmiBusCfg_Ppid *pm_vrm_rsrc_arr, uint32 *vrm_cfg_index, uint8 vrm_rsrc_arr_size);

pm_err_flag_type pm_tgt_mgpi_cfg(SpmiBusCfg_MgpiPortCfg** mgpi_port_cfg_ptr);

#endif //__PMIC_SBL_BOOT_TARGET_H__
