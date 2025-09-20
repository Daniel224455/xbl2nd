#ifndef DRE_CONFIG_H
#define DRE_CONFIG_H

/**
 * @file dre_config.h
 * @brief
 * Function headers of DRE drivers internal private API
 */

/*==============================================================================
                                EDIT HISTORY

================================================================================
when       who        what, where, why
--------   ---        --------------------------------------------------------
07/23/18   c_mkurum     Initial revision.
================================================================================
                  Copyright 2018 Qualcomm Technologies Incorporated.
                            All Rights Reserved.
                    Qualcomm Confidential and Proprietary
==============================================================================*/

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
boolean ecc_syndrome_region_init();

#endif