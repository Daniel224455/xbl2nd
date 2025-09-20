#ifndef BOOT_MC_H
#define BOOT_MC_H

/*=============================================================================

                              Boot Main Control

GENERAL DESCRIPTION
  This file contains the main control functions for SBL execution.

Copyright 2010-2011, 2015 by Qualcomm Technologies Inc.  All Rights Reserved.
=============================================================================*/

/*===========================================================================

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.


when       who     what, where, why
--------   ---     ----------------------------------------------------------
10/05/15   ck      Corrected copyright
02/01/11   dxiang  Add boot logical function pointer type 
07/22/10   aus     Initial creation
============================================================================*/

/*===========================================================================
 
                           INCLUDE FILES

===========================================================================*/
#include "boot_sbl_shared.h"



/* Cookie for primary mode - boot-primary */
#define IMAGE_RECOVERY_COOKIE_A_0 0x746f6f62
#define IMAGE_RECOVERY_COOKIE_A_1 0x6972702d
#define IMAGE_RECOVERY_COOKIE_A_2 0x7972616d
#define IMAGE_RECOVERY_COOKIE_A_3 0x00000000

/* Cookie for recovery mode - boot-recovery */
#define IMAGE_RECOVERY_COOKIE_B_0 0x746f6f62
#define IMAGE_RECOVERY_COOKIE_B_1 0x6365722d
#define IMAGE_RECOVERY_COOKIE_B_2 0x7265766f
#define IMAGE_RECOVERY_COOKIE_B_3 0x00000079

/* FDR cookies*/
#define FDR_COOKIE_0 0x6F636572
#define FDR_COOKIE_1 0x79726576
#define FDR_COOKIE_2 0x772D2D0A
#define FDR_COOKIE_3 0x5F657069
#define FDR_COOKIE_4 0x61746164
#define FDR_COOKIE_5 0x6C2D2D0A
#define FDR_COOKIE_6 0x6C61636F
#define FDR_COOKIE_7 0x6E653D65
#define FDR_COOKIE_8 0x0A53552D
#define FDR_COOKIE_9 0x0000000A

/* Partitions to be used for recovery*/
#define PARTITION_B 1
#define PARTITION_A 2
#define PARTITION_DEFAULT 0

/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/

/* The boot procedure type */
typedef void (*boot_procedure_func_type)
(
  bl_shared_data_type *bl_shared_data
);

/* The boot logical type */
typedef boot_boolean (*boot_logical_func_type)
(
  bl_shared_data_type *bl_shared_data
);
/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/

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
);

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
);




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
boolean boot_set_backup_partition_cookie ( uint8 partition_type);


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
uint8 boot_check_backup_partition_cookie ( void );


#endif  /* BOOT_MC_H */

