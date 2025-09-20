/*=============================================================================
                              DRE NULL.C
GENERAL DESCRIPTION
This is the target header file for DRE Initializtion .

Copyright 2018 by Qualcomm Technologies, Inc.  All Rights Reserved.

===========================================================================

                            EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.


when       who        what, where, why
--------   ---        --------------------------------------------------------
06/08/18   c_mkurum      Initial revision.
=============================================================================

*/

#include "ddr_drivers.h"

/* ============================================================================
**  Function : dre_init
** ============================================================================
*/
/**
*   @brief
*   This function initializes 
*   AHB Clock: 
*   DRE Core clock: Div-2 of SHUB clock
*   ATB clock
*
*   @param[in]  None
*                         
*
*   @return
*   TRUE if all clocks are inited , else return FALSE
*
*   @dependencies
*   None
*
*   @sa
*   None
*
*   @sa
*   None
*/

//boolean dre_init(BOOT_DDR_STRUCT *ddr)
boolean dre_init()
{
  return TRUE;	
}


/* ============================================================================
**  Function : dre_test
** ============================================================================
*/
/**
*   @brief
*   This function tests DRE 
*
*   @param[in]  None
*                         
*
*   @return
*   TRUE if DRE tests is success
*
*   @dependencies
*   None
*
*   @sa
*   None
*
*   @sa
*   None
*/
boolean dre_test()
{
  return TRUE;	
}