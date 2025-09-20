#ifndef SBL_CRASHDUMP_H
#define SBL_CRASHDUMP_H

/*===========================================================================

                         SBL Crashdump Header File

GENERAL DESCRIPTION
  This header file contains declarations and definitions for the SBL
  crashdump feature. At the top level this is called by boot_error_handler.
    
Copyright 2018 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
============================================================================*/

/*===========================================================================

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.


when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
09/06/18   msantos created file

============================================================================*/

/*===========================================================================
 
                           INCLUDE FILES

===========================================================================*/

/*==========================================================================
                          PUBLIC DATA DECLARATIONS
===========================================================================*/

/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/
       
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
void boot_crash_dump_handler(void);

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
void sbl_crashdump_reset_with_dload(void);


#endif /* SBL_CRASHDUMP_H */