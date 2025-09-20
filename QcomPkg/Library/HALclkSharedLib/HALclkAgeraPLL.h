#ifndef __HALCLKAGERAPLL_H__
#define __HALCLKAGERAPLL_H__
/*
==============================================================================

FILE:         HALclkAgeraPLL.h

DESCRIPTION:
  Internal, chipset specific PLL definitions for the clock HAL module.

==============================================================================

                             Edit History


when       who     what, where, why
--------   ---     -----------------------------------------------------------
02/22/16   sr      Created.

==============================================================================
    Copyright (c) 2016-2018 QUALCOMM Technologies Incorporated.
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
#define HWIO_PLL_USER_CTL_OFFS                              0x0000000c
#define HWIO_PLL_CONFIG_CTL_OFFS                            0x00000010
#define HWIO_PLL_CONFIG_CTL_U_OFFS                          0x00000014
#define HWIO_PLL_TEST_CTL_OFFS                              0x00000018
#define HWIO_PLL_TEST_CTL_U_OFFS                            0x0000001c
#define HWIO_PLL_SSC_DELTA_ALPHA_OFFS                       0x00000020
#define HWIO_PLL_SSC_UPDATE_RATE_OFFS                       0x00000024
#define HWIO_PLL_STATUS_OFFS                                0x0000002c
#define HWIO_PLL_SECURE_OFFS                                0x00000040


/*
 * PLL_MODE register definitions.
 */
#define HWIO_PLL_MODE_LOCK_DET_BMSK                         0x80000000
#define HWIO_PLL_MODE_LOCK_DET_SHFT                               0x1f
#define HWIO_PLL_MODE_PLL_ACTIVE_FLAG_BMSK                  0x40000000
#define HWIO_PLL_MODE_PLL_ACTIVE_FLAG_SHFT                        0x1e
#define HWIO_PLL_MODE_FSM_RES_BMSK                            0x200000
#define HWIO_PLL_MODE_FSM_RES_SHFT                                0x15
#define HWIO_PLL_MODE_FSM_EN_BMSK                             0x100000
#define HWIO_PLL_MODE_FSM_EN_SHFT                                 0x14
#define HWIO_PLL_MODE_BIAS_COUNT_BMSK                          0xfc000
#define HWIO_PLL_MODE_BIAS_COUNT_SHFT                              0xe
#define HWIO_PLL_MODE_LOCK_COUNT_BMSK                           0x3f00
#define HWIO_PLL_MODE_LOCK_COUNT_SHFT                              0x8
#define HWIO_PLL_MODE_PLLTEST_BMSK                                 0x8
#define HWIO_PLL_MODE_PLLTEST_SHFT                                 0x3
#define HWIO_PLL_MODE_RESET_N_BMSK                                 0x4
#define HWIO_PLL_MODE_RESET_N_SHFT                                 0x2
#define HWIO_PLL_MODE_BYPASSNL_BMSK                                0x2
#define HWIO_PLL_MODE_BYPASSNL_SHFT                                0x1
#define HWIO_PLL_MODE_OUTCTRL_BMSK                                 0x1
#define HWIO_PLL_MODE_OUTCTRL_SHFT                                 0x0


/*
 * PLL_USER_CTL register definitions.
 */
#define HWIO_PLL_USER_CTL_ALPHA_MODE_BMSK                    0x2000000
#define HWIO_PLL_USER_CTL_ALPHA_MODE_SHFT                         0x19
#define HWIO_PLL_USER_CTL_MN_ACC_BMSK                        0x1000000
#define HWIO_PLL_USER_CTL_MN_ACC_SHFT                             0x18
#define HWIO_PLL_USER_CTL_CLK_REF_PRE_DIV_BMSK                  0x1000
#define HWIO_PLL_USER_CTL_CLK_REF_PRE_DIV_SHFT                     0xc
#define HWIO_PLL_USER_CTL_CLK_POST_DIV_BMSK                      0x300
#define HWIO_PLL_USER_CTL_CLK_POST_DIV_SHFT                        0x8
#define HWIO_PLL_USER_CTL_OUT_INV_EN_BMSK                         0x80
#define HWIO_PLL_USER_CTL_OUT_INV_EN_SHFT                          0x7
#define HWIO_PLL_USER_CTL_PLLOUT_LV_TEST_EN_BMSK                  0x10
#define HWIO_PLL_USER_CTL_PLLOUT_LV_TEST_EN_SHFT                   0x4
#define HWIO_PLL_USER_CTL_LVEARLY_EN_BMSK                          0x8
#define HWIO_PLL_USER_CTL_LVEARLY_EN_SHFT                          0x3
#define HWIO_PLL_USER_CTL_PLLOUT_LV_AUX2_EN_BMSK                   0x4
#define HWIO_PLL_USER_CTL_PLLOUT_LV_AUX2_EN_SHFT                   0x2
#define HWIO_PLL_USER_CTL_PLLOUT_LV_AUX_EN_BMSK                    0x2
#define HWIO_PLL_USER_CTL_PLLOUT_LV_AUX_EN_SHFT                    0x1
#define HWIO_PLL_USER_CTL_LVMAIN_EN_BMSK                           0x1
#define HWIO_PLL_USER_CTL_LVMAIN_EN_SHFT                           0x0


/*
 * PLL_STATUS register definitions.
 */
#define HWIO_PLL_STATUS_LOCK_DET_BMSK                          0x10000
#define HWIO_PLL_STATUS_LOCK_DET_SHFT                             0x10
#define HWIO_PLL_STATUS_OV_VLT_CNT_STS_BMSK                       0x3c
#define HWIO_PLL_STATUS_OV_VLT_CNT_STS_SHFT                        0x2
#define HWIO_PLL_STATUS_OV_VLT_DET_BMSK                            0x2
#define HWIO_PLL_STATUS_OV_VLT_DET_SHFT                            0x1
#define HWIO_PLL_STATUS_CLK_N_BMSK                                 0x1
#define HWIO_PLL_STATUS_CLK_N_SHFT                                 0x0


/*
 * PLL_SECURE register definitions.
 */
#define HWIO_PLL_SECURE_DROOP_CTL_BMSK                             0x2
#define HWIO_PLL_SECURE_DROOP_CTL_SHFT                             0x1
#define HWIO_PLL_SECURE_PLL_CTL_BMSK                               0x1
#define HWIO_PLL_SECURE_PLL_CTL_SHFT                               0x0


/* ============================================================================
**    External Definitions
** ==========================================================================*/


/* ============================================================================
**    External Definitions
** ==========================================================================*/


/* ============================================================================
**    Functions
** ==========================================================================*/

/*
 * Source control functions.  These can be used if some of the control
 * needs to be overridden by a specific PLL.
 */
static boolean HAL_clk_AgeraPLLEnable           ( HAL_clk_SourceDescType *pSource, HAL_clk_SourceDisableModeType eMode, void *pData );
static void    HAL_clk_AgeraPLLDisable          ( HAL_clk_SourceDescType *pSource, HAL_clk_SourceDisableModeType eMode, void *pData );
static boolean HAL_clk_AgeraPLLIsEnabled        ( HAL_clk_SourceDescType *pSource );
static void    HAL_clk_AgeraPLLConfig           ( HAL_clk_SourceDescType *pSource, HAL_clk_SourceConfigType eConfig );
static boolean HAL_clk_AgeraPLLConfigPLL        ( HAL_clk_SourceDescType *pSource, const HAL_clk_PLLConfigType *pmConfig, HAL_clk_SourceConfigMode eMode );
static void    HAL_clk_AgeraPLLDetectPLLConfig  ( HAL_clk_SourceDescType *pSource, HAL_clk_PLLConfigType *pmConfig );
static boolean HAL_clk_AgeraPLLEnableVote       ( HAL_clk_SourceDescType *pSource );
static void    HAL_clk_AgeraPLLDisableVote      ( HAL_clk_SourceDescType *pSource );
static boolean HAL_clk_AgeraPLLIsVoteEnabled    ( HAL_clk_SourceDescType *pSource );
static boolean HAL_clk_AgeraPLLIsPLLConfigEqual ( const HAL_clk_PLLConfigType *pConfigA, const HAL_clk_PLLConfigType *pConfigB);
static boolean HAL_clk_AgeraPLLIsCalibrated     ( HAL_clk_SourceDescType *pSource );
static boolean HAL_clk_AgeraPLLSetCalConfig     ( HAL_clk_SourceDescType *pSource, const HAL_clk_PLLConfigType *pmConfig );
static void    HAL_clk_AgeraPLLSetRegSettings   ( HAL_clk_SourceDescType *pSource, const HAL_clk_SourceRegSettingsType *pmSettings );

#endif  /* __HALCLKAGERAPLL_H__ */

