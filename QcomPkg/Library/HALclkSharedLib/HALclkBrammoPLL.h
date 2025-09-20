#ifndef __HALCLKBRAMMOPLL_H__
#define __HALCLKBRAMMOPLL_H__
/*
==============================================================================

FILE:         HALclkBrammoPLL.h

DESCRIPTION:
  Internal, chipset specific PLL definitions for the clock HAL module.

==============================================================================

                             Edit History


when       who     what, where, why
--------   ---     -----------------------------------------------------------
10/14/14   sr      Ported from Prius PLL.
05/30/13   frv     Created.

==============================================================================
  Copyright (c) 2013-2015 QUALCOMM Technologies Incorporated.
                    All Rights Reserved.
                  QUALCOMM Proprietary/GTDR
==============================================================================
*/

/*============================================================================

                     INCLUDE FILES FOR MODULE

============================================================================*/

#include "HALclkInternal.h"



/* ============================================================================
**    Definitions
** ==========================================================================*/


/*
 * PLL register offsets.
 */
#define HWIO_PLL_MODE_OFFS                                  0x00000000
#define HWIO_PLL_L_VAL_OFFS                                 0x00000004
#define HWIO_PLL_ALPHA_VAL_OFFS                             0x00000008
#define HWIO_PLL_ALPHA_U_VAL_OFFS                           0x0000000c
#define HWIO_PLL_USER_CTL_OFFS                              0x00000010
#define HWIO_PLL_USER_CTL_U_OFFS                            0x00000014
#define HWIO_PLL_CONFIG_CTL_OFFS                            0x00000018
#define HWIO_PLL_TEST_CTL_OFFS                              0x0000001c
#define HWIO_PLL_TEST_CTL_U_OFFS                            0x00000020
#define HWIO_PLL_STATUS_OFFS                                0x00000024

/*
 * PLL_MODE register definitions.
 */
#define HWIO_PLL_MODE_PLL_LOCK_DET_BMSK                     0x80000000
#define HWIO_PLL_MODE_PLL_LOCK_DET_SHFT                           0x1f
#define HWIO_PLL_MODE_PLL_ACTIVE_FLAG_BMSK                  0x40000000
#define HWIO_PLL_MODE_PLL_ACTIVE_FLAG_SHFT                        0x1e
#define HWIO_PLL_MODE_PLL_ACK_LATCH_BMSK                    0x20000000
#define HWIO_PLL_MODE_PLL_ACK_LATCH_SHFT                          0x1d
#define HWIO_PLL_MODE_PLL_HW_UPDATE_LOGIC_BYPASS_BMSK         0x800000
#define HWIO_PLL_MODE_PLL_HW_UPDATE_LOGIC_BYPASS_SHFT             0x17
#define HWIO_PLL_MODE_PLL_UPDATE_BMSK                         0x400000
#define HWIO_PLL_MODE_PLL_UPDATE_SHFT                             0x16
#define HWIO_PLL_MODE_PLL_VOTE_FSM_RESET_BMSK                 0x200000
#define HWIO_PLL_MODE_PLL_VOTE_FSM_RESET_SHFT                     0x15
#define HWIO_PLL_MODE_PLL_VOTE_FSM_ENA_BMSK                   0x100000
#define HWIO_PLL_MODE_PLL_VOTE_FSM_ENA_SHFT                       0x14
#define HWIO_PLL_MODE_PLL_BIAS_COUNT_BMSK                      0xfc000
#define HWIO_PLL_MODE_PLL_BIAS_COUNT_SHFT                          0xe
#define HWIO_PLL_MODE_PLL_LOCK_COUNT_BMSK                       0x3f00
#define HWIO_PLL_MODE_PLL_LOCK_COUNT_SHFT                          0x8
#define HWIO_PLL_MODE_PLL_PLLTEST_BMSK                             0x8
#define HWIO_PLL_MODE_PLL_PLLTEST_SHFT                             0x3
#define HWIO_PLL_MODE_PLL_RESET_N_BMSK                             0x4
#define HWIO_PLL_MODE_PLL_RESET_N_SHFT                             0x2
#define HWIO_PLL_MODE_PLL_BYPASSNL_BMSK                            0x2
#define HWIO_PLL_MODE_PLL_BYPASSNL_SHFT                            0x1
#define HWIO_PLL_MODE_PLL_OUTCTRL_BMSK                             0x1
#define HWIO_PLL_MODE_PLL_OUTCTRL_SHFT                             0x0

/*
 * PLL_USER_CTL register definitions.
 */
#define HWIO_PLL_USER_CTL_ALPHA_EN_BMSK                      0x1000000
#define HWIO_PLL_USER_CTL_ALPHA_EN_SHFT                           0x18
#define HWIO_PLL_USER_CTL_INCLK_SEL_BMSK                        0x3000
#define HWIO_PLL_USER_CTL_INCLK_SEL_SHFT                           0xc
#define HWIO_PLL_USER_CTL_POSTDIV_CTL_BMSK                       0x300
#define HWIO_PLL_USER_CTL_POSTDIV_CTL_SHFT                         0x8
#define HWIO_PLL_USER_CTL_OUTPUT_INV_BMSK                         0x80
#define HWIO_PLL_USER_CTL_OUTPUT_INV_SHFT                          0x7
#define HWIO_PLL_USER_CTL_PLLOUT_LV_TEST_BMSK                     0x10
#define HWIO_PLL_USER_CTL_PLLOUT_LV_TEST_SHFT                      0x4
#define HWIO_PLL_USER_CTL_PLLOUT_LV_EARLY_BMSK                     0x8
#define HWIO_PLL_USER_CTL_PLLOUT_LV_EARLY_SHFT                     0x3
#define HWIO_PLL_USER_CTL_PLLOUT_LV_AUX2_BMSK                      0x4
#define HWIO_PLL_USER_CTL_PLLOUT_LV_AUX2_SHFT                      0x2
#define HWIO_PLL_USER_CTL_PLLOUT_LV_AUX_BMSK                       0x2
#define HWIO_PLL_USER_CTL_PLLOUT_LV_AUX_SHFT                       0x1
#define HWIO_PLL_USER_CTL_PLLOUT_LV_MAIN_BMSK                      0x1
#define HWIO_PLL_USER_CTL_PLLOUT_LV_MAIN_SHFT                      0x0


/* ============================================================================
**    External Definitions
** ==========================================================================*/


/* ============================================================================
**    External Definitions
** ==========================================================================*/


/* ============================================================================
**    Functions
** ==========================================================================*/

#endif  /* __HALCLKBRAMMOPLL_H__ */

