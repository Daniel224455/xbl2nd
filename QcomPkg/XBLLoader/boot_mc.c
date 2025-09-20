/*=============================================================================

                              Boot Main Control

GENERAL DESCRIPTION
  This file contains the main control functions for SBL execution.

Copyright 2010-2011, 2015 by Qualcomm Technologies Inc.  All Rights Reserved.
=============================================================================*/

/*=============================================================================

                            EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.


when       who     what, where, why
--------   ---     ------------------------------------------------------------
10/05/15   ck      Corrected copyright
03/21/11   dxiang  In boot_do_procedures(), do not force an error fatal if the 
                   procedures parameter is NULL
07/22/10   aus     Initial creation
=============================================================================*/


/*=============================================================================

                            INCLUDE FILES FOR MODULE

=============================================================================*/
#include "boot_error_handler.h"
#include "boot_mc.h"
#include "boot_sbl_shared.h"
#include "boot_util.h"
#include "err_reset_detect.h"
#include "boot_flash_dev_if.h"
#include BOOT_PBL_H

extern uint8 backup_partition_id[PARTITION_ID_MAX_SIZE];
uint32 recovery_mode[26];

static whitelst_tbl_entry_type recovery_mode_whitelist[]= {{(uint64)(&recovery_mode),26*sizeof(uint32)},{0,0}};
      

/*=============================================================================

            LOCAL DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, types,
variables and other items needed by this module.

=============================================================================*/
#ifdef FEATURE_MULTIPROCESSOR
/* Array to store the register data passed from Boot ROM */
err_reset_log_type   register_log;
#endif /* FEATURE_MULTIPROCESSOR */

/*=============================================================================
   
                              FUNCTION DEFINITIONS

=============================================================================*/

/*===========================================================================

**  Function :  boot_do_procedures

** ==========================================================================
*/
/*!
* 
* @brief
*    Processes an array of boot procedures.
* 
* @param[in] bl_shared_data Pointer to the shared data passed between functions
* @param[in] procs          Pointer to the function pointer table 
*
* @par Dependencies
*   None
* 
* @retval
*   None
* 
* @par Side Effects
*   None
* 
*/
void boot_do_procedures
(
  bl_shared_data_type *bl_shared_data,
  boot_procedure_func_type *procs
)
{
  boot_procedure_func_type *cur_proc;

  BL_VERIFY( bl_shared_data != NULL, BL_ERR_NULL_PTR_PASSED|BL_ERROR_GROUP_BOOT );

  if (procs != NULL)
  {
    for ( cur_proc = procs; *cur_proc != NULL; cur_proc++ )
    {
      (*cur_proc)( bl_shared_data );
    }
  }
} /* boot_do_procedures() */

/*===========================================================================

**  Function :  boot_save_reset_register_log

** ==========================================================================
*/
/*!
* 
* @brief
*   Save reset registers to global variable register_log. This is used to 
*   debug hardware reset by dumping registers in downloader mode.
*  
* @param[in] bl_shared_data Pointer to the shared data passed between functions
*
* @par Dependencies
*   None
*   
* @retval
*   None                                                               
* 
* @par Side Effects
*   None
* 
*/
void boot_save_reset_register_log
( 
  bl_shared_data_type *bl_shared_data 
)
{
#ifdef FEATURE_MULTIPROCESSOR
	/* Backup 26 uint32 reset registers stored by PBL at BOOT_STACK_BASE.
     SP and LR SYS, IRQ, FIQ, ABT, UDF - 10 uint32 registers
	 SVC_R0
	 SVC_R1 
	 ......
	 ......
	 SVC_SP <---- (BOOT_STACK_BAES - 0x8)
	 SVC_LR <---- (BOOT_STACK_BAES - 0x4)
	 SVC_PC <---- (BOOT_STACK_BASE)
    */
  qmemcpy((void *)&register_log, 
	      (const void *)(boot_pbl_get_boot_stack_base()
					   - sizeof(err_reset_log_type)), sizeof(err_reset_log_type));

#endif /* FEATURE_MULTIPROCESSOR */
} /* boot_save_reset_register_log() */






/*===========================================================================
**  Function :  boot_check_backup_partition_cookie
** ==========================================================================
*/
/*!
*
* @brief
*   This function opens BACKUP partition and checks if we need to boot
*   backup partitions
*
* @par Dependencies
*   None
*
* @retval
*   boolean - TRUE - recovery mode
*             FALSE - normal mode
*
* @par Side Effects
*   None
*
*/
uint8 boot_check_backup_partition_cookie ( void )
{
   boot_flash_trans_if_type *trans_if;      /* translation interface */
   uint8 success = 0;


   recovery_mode[0] = 0; recovery_mode[1] = 0; recovery_mode[2] = 0; recovery_mode[3] = 0;

   do
   {
      /* Configure the target image */
      boot_flash_configure_target_image(backup_partition_id);

      /* Initialize flash translation interface to read backup partition */
      trans_if = boot_flash_dev_open_image(GEN_IMG);

      /* If partition not found, boot normally */
      if(trans_if == NULL)
      {
        success = 0;
        break;
      }
    
      
      boot_set_active_hdr_meta_whitelist_table(recovery_mode_whitelist) ;

      /* Read the recovery cookie from the base of the partition */
      success = boot_flash_trans_read( trans_if,
                                       recovery_mode,
                                       0 /* offset */,
                                       sizeof(recovery_mode),
                                        HDR_META_INFO_TYPE  );

      BL_VERIFY( success, BL_ERR_OTHERS );

      /* Release the translation layer resource */
      boot_flash_dev_close_image( &trans_if );

      /* Check if the cookie is present */
    if ((recovery_mode[0] == IMAGE_RECOVERY_COOKIE_B_0) && (recovery_mode[1] == IMAGE_RECOVERY_COOKIE_B_1) &&
        (recovery_mode[2] == IMAGE_RECOVERY_COOKIE_B_2) && ((recovery_mode[3] & 0x0000FFFF) == IMAGE_RECOVERY_COOKIE_B_3) &&
        (recovery_mode[16] != FDR_COOKIE_0) && (recovery_mode[17] != FDR_COOKIE_1) &&
        (recovery_mode[18] != FDR_COOKIE_2) && (recovery_mode[19] != FDR_COOKIE_3) &&
        (recovery_mode[20] != FDR_COOKIE_4) && (recovery_mode[21] != FDR_COOKIE_5) &&
        (recovery_mode[22] != FDR_COOKIE_6) && (recovery_mode[23] != FDR_COOKIE_7) &&
        (recovery_mode[24] != FDR_COOKIE_8))
    {
      success = PARTITION_B;
    }
    else if((recovery_mode[0] == IMAGE_RECOVERY_COOKIE_A_0) && (recovery_mode[1] == IMAGE_RECOVERY_COOKIE_A_1) &&
              (recovery_mode[2] == IMAGE_RECOVERY_COOKIE_A_2) && ((recovery_mode[3] & 0x000000FF) == IMAGE_RECOVERY_COOKIE_A_3) &&
        (recovery_mode[16] != FDR_COOKIE_0) && (recovery_mode[17] != FDR_COOKIE_1) &&
        (recovery_mode[18] != FDR_COOKIE_2) && (recovery_mode[19] != FDR_COOKIE_3) &&
        (recovery_mode[20] != FDR_COOKIE_4) && (recovery_mode[21] != FDR_COOKIE_5) &&
        (recovery_mode[22] != FDR_COOKIE_6) && (recovery_mode[23] != FDR_COOKIE_7) &&
        (recovery_mode[24] != FDR_COOKIE_8))
      {
          success = PARTITION_A;
      }
      else
      {
          success = PARTITION_DEFAULT;
      }

    } while(0);

   return success;
}


/*===========================================================================
**  Function :  boot_set_backup_partition_cookie
** ==========================================================================
*/
/*!
*
* @brief
*   This function opens BACKUP partition and writes backup partition cookie
*
* @par Dependencies
*   None
*
* @retval
*   boolean - TRUE - success
*             FALSE - failure
*
* @par Side Effects
*   None
*
*/
boolean boot_set_backup_partition_cookie ( uint8 partition_type)
{
   boot_flash_trans_if_type *trans_if;      /* translation interface */
   boolean success = FALSE;

   if(partition_type == PARTITION_B)
   {
     recovery_mode[0] = IMAGE_RECOVERY_COOKIE_B_0;
     recovery_mode[1] = IMAGE_RECOVERY_COOKIE_B_1;
     recovery_mode[2] = IMAGE_RECOVERY_COOKIE_B_2;
     recovery_mode[3] = IMAGE_RECOVERY_COOKIE_B_3;
   }
   else
   {
     recovery_mode[0] = IMAGE_RECOVERY_COOKIE_A_0;
     recovery_mode[1] = IMAGE_RECOVERY_COOKIE_A_1;
     recovery_mode[2] = IMAGE_RECOVERY_COOKIE_A_2;
     recovery_mode[3] = IMAGE_RECOVERY_COOKIE_A_3;
   }
   do
   {
      /* Configure the target image */
      boot_flash_configure_target_image(backup_partition_id);

      /* Initialize flash translation interface to write backup partition */
      trans_if = boot_flash_dev_open_image(GEN_IMG);

      /* If partition not found, boot normally */
      if(trans_if == NULL)
      {
        success = FALSE;
        break;
      }

      /* Write the recovery cookie to the base of the partition */
      
      success = boot_flash_trans_write( trans_if,
                                       recovery_mode,
                                       0 ,
                                       sizeof(recovery_mode));

      /* Release the translation layer resource */
      boot_flash_dev_close_image( &trans_if );

    } while(0);

   return success;
}
