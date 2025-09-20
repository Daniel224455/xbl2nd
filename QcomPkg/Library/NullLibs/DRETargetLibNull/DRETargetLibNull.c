/** @file DRETargetLibNull.c
  
  Stub functions for DRETargetLib

  Copyright (c) 2018, Qualcomm Technologies, Inc. All rights reserved.
**/

/*=============================================================================
                              EDIT HISTORY


 when       who       what, where, why
 --------   ---       -------------------------------------------------------
 07/23/18   c_mkurum  Initial revision

=============================================================================*/


/*==========================================================================

                               INCLUDE FILES

===========================================================================*/
#include "ddr_drivers.h"
#include "ddr_common.h"

/*===========================================================================
                      FUNCTION DECLARATIONS
===========================================================================*/ 


/* ============================================================================
**  Function : ecc_syndrome_region_init
** ============================================================================
*/
/**
*   @brief
*   This function sets the DDR driver's device table to an external device table.
*   The Syndrome DRAM region need to be initialized to “0”. The traffic generator
*   modules in DDRSS infrastructure are to be used.
*
*   @param[in]  None
*                         
*
*   @return
*   TRUE if DDR_TG0_TG_STREAM_WR_STATUS0 is SET, else return FALSE
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
boolean ecc_syndrome_region_init()
{
  return TRUE;
}
