/*=============================================================================

                               SBL Crashdump

GENERAL DESCRIPTION
  This file contains the QUALCOMM SBL crashdump feature. 
  At the top level this is called by boot_error_handler.

Copyright 2018 by Qualcomm Technologies, Inc.  All Rights Reserved.
=============================================================================*/


/*=============================================================================

                            EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.


when       who     what, where, why
--------   ---     ------------------------------------------------------------
09/06/18   msantos created file


=============================================================================*/


/*=============================================================================

                            INCLUDE FILES FOR MODULE

=============================================================================*/
#include "Target_cust.h"
#include "boot_error_handler.h"
#include "boot_dload.h"
#include "boot_logger.h"
#include "boothw_target.h"
#include "boot_cache_mmu.h"
#include "boot_util.h"

/*=============================================================================

            LOCAL DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, types,
variables and other items needed by this module.

=============================================================================*/
boot_crash_dump_type boot_crash_dump_data;

/*=============================================================================
   
                              FUNCTION DEFINITIONS

=============================================================================*/

/*===========================================================================
**  Function :  boot_crash_dump_handler
** ==========================================================================
*/
/*!
* 
* @brief
*   This fills the boot_crash_dump_data struct for use by
*   simulator tool, and copies it and other boot memory
*   regions to an unused region for dumping
*
* @param[in] None
*        
* @par Dependencies
*  none
*   
*/
void boot_crash_dump_handler(void)
{
  /* Initialize the boot crash dump data type */
  boot_crash_dump_data.crash_dump_cookie = BOOT_CRASH_DUMP_COOKIE;
  boot_crash_dump_data.version = BOOT_CRASH_DUMP_VERSION;

  /* Update SBL DDR region meta data */
  boot_crash_dump_data.sbl_ddr_addr = SCL_SBL1_DDR_BASE;
  boot_crash_dump_data.sbl_ddr_size = SCL_SBL1_DDR_SIZE;
  boot_crash_dump_data.sbl_ddr_dump_addr = SCL_SBL1_DDR_DUMP_ADDR;
    
  /* Update BOOT IMEM region meta data */
  boot_crash_dump_data.sbl_boot_imem_addr = SCL_BOOT_IMEM_BASE_ADDR;
  boot_crash_dump_data.sbl_boot_imem_size = SCL_BOOT_IMEM_BASE_SIZE;
  boot_crash_dump_data.sbl_boot_imem_dump_addr = SCL_BOOT_IMEM_DUMP_ADDR;

  /* Update SHARED IMEM region meta data */
  boot_crash_dump_data.sbl_shared_imem_addr = SHARED_IMEM_BASE;
  boot_crash_dump_data.sbl_shared_imem_size = SHARED_IMEM_SIZE;
  boot_crash_dump_data.sbl_shared_imem_dump_addr = SHARED_IMEM_DUMP_ADDR;

  /* Ensure no more api get called beyond this point. So RW/ZI variables 
   * are consistent in dumps */
   
  /* Flush boot_crash_dump_data struct */
  qmemcpy((void *)BOOT_CRASH_DUMP_DATA_DUMP_ADDR,
          &boot_crash_dump_data,
          BOOT_CRASH_DUMP_DATA_SIZE);
          
  /* Flush SBL DDR region */
  qmemcpy((void *)boot_crash_dump_data.sbl_ddr_dump_addr, 
          (const void *)boot_crash_dump_data.sbl_ddr_addr,
          boot_crash_dump_data.sbl_ddr_size);

  /* Flush BOOT IMEM region */
  qmemcpy((void *)boot_crash_dump_data.sbl_boot_imem_dump_addr, 
          (const void *)boot_crash_dump_data.sbl_boot_imem_addr,
          boot_crash_dump_data.sbl_boot_imem_size);
          
  /* Flush SHARED IMEM region */
  qmemcpy((void *)boot_crash_dump_data.sbl_shared_imem_dump_addr,
          (const void *)boot_crash_dump_data.sbl_shared_imem_addr,
          boot_crash_dump_data.sbl_shared_imem_size);

  /* Flush the cache */
  mmu_flush_cache();
}

/*===========================================================================
**  Function :  sbl_crashdump_reset_with_dload
** ==========================================================================
*/
/*!
* 
* @brief
*   prepares to reenter XBL Loader with DLOAD support,
*   but if it was already prepared then this function
*   was entered twice and will go to PBL EDL to avoid loop
*
* @param[in] None
*        
* @par Dependencies
*  none
*   
*/
void sbl_crashdump_reset_with_dload(void)
{
  if(boot_dload_is_dload_mode_set())
  {
    boot_log_message("sbl_crashdump_reset_with_dload: Ramdump not allowed but DLOAD cookie is set. Entering PBL EDL");
    boot_dload_transition_pbl_forced_dload();
  }
  else
  {
    /* Need to reenter XBL Loader with DLOAD support */
    boot_log_message("sbl_crashdump_reset_with_dload: Ramdump not allowed. Going for warm reset with DLOAD support");
    boot_dload_set_cookie();
    boot_dload_set_rawdump_cookie();
    boot_hw_reset(BOOT_WARM_RESET_TYPE);  
  }
}