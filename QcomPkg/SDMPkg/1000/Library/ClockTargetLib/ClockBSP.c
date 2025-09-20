/**
==============================================================================

  @file ClockBSP.c

  Definitions of the support clock perf level.

  Copyright (c) 2017 QUALCOMM Technologies, Inc.  All Rights Reserved.  
  QUALCOMM Proprietary and Confidential.

==============================================================================


==============================================================================

                            EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.


  when      who     what, where, why
  --------  ------  ------------------------------------------------------
  05/16/17  vphan   Initial SDM855 revision, branched from SDM845.

==============================================================================
*/


/*=========================================================================
      Include Files
==========================================================================*/

#include "ClockBSP.h"
#include "ClockDriver.h"
#include "ClockHWIO.h"

/*=========================================================================
      Prototypes
==========================================================================*/


/*=========================================================================
      Data
==========================================================================*/
Clock_RailwayType ClockRailway =
{
  "vddcx",
  0,
  0,
  "vdda_ebi",
  0,
  0
};

Clock_ConfigType Clock_ConfigData =
{
  /* GPLL0 @ 600 MHz (general purpose PLL). */
  .GPLL0_Cfg =
  {
    .nRefCount     = 0,
    .nPLLModeAddr  = HWIO_ADDR(GCC_GPLL0_MODE),
    .nVoteAddr     = HWIO_ADDR(GCC_SPARE_GPLL_ENA_VOTE),
    .nVoteMask     = HWIO_FMSK(GCC_SPARE_GPLL_ENA_VOTE, GPLL0),
    .nPreDiv       = 1,
    .nPostDivOdd   = 0,
    .nPostDivEven  = 2,
    .nL            = 0x1F,
    .nAlpha        = 0x4000,
    .nCalibrationL = 68,
    .nUserCtl      = TRION_USER_CTL_MAIN_EVEN_VAL,
    .nUserCtl_U    = TRION_USER_CTL_U_VAL,
    .nUserCtl_U1   = TRION_USER_CTL_U1_VAL,
    .nConfigCtl    = TRION_CONFIG_CTL_VAL,
    .nConfigCtl_U  = TRION_CONFIG_CTL_U_VAL,
    .nConfigCtl_U1 = TRION_CONFIG_CTL_U1_VAL,
    .nTestCtl      = TRION_TEST_CTL_VAL,
    .nTestCtl_U    = TRION_TEST_CTL_U_VAL,
    .nTestCtl_U1   = TRION_TEST_CTL_U1_VAL,
  },
 
  /* GPLL1 @ 1066 MHz. */
  .GPLL1_Cfg =
  {
    .nRefCount     = 0,
    .nPLLModeAddr  = HWIO_ADDR(GCC_GPLL1_MODE),
    .nVoteAddr     = HWIO_ADDR(GCC_SPARE_GPLL_ENA_VOTE),
    .nVoteMask     = HWIO_FMSK(GCC_SPARE_GPLL_ENA_VOTE, GPLL1),
    .nPreDiv       = 1,
    .nPostDivOdd   = 0,
    .nPostDivEven  = 0,
    .nL            = 55,
    .nAlpha        = 0x8555,
    .nCalibrationL = 68,
    .nUserCtl      = TRION_USER_CTL_MAIN_VAL,
    .nUserCtl_U    = TRION_USER_CTL_U_VAL,
    .nUserCtl_U1   = TRION_USER_CTL_U1_VAL,
    .nConfigCtl    = TRION_CONFIG_CTL_VAL,
    .nConfigCtl_U  = TRION_CONFIG_CTL_U_VAL,
    .nConfigCtl_U1 = TRION_CONFIG_CTL_U1_VAL,
    .nTestCtl      = TRION_TEST_CTL_VAL,
    .nTestCtl_U    = TRION_TEST_CTL_U_VAL,
    .nTestCtl_U1   = TRION_TEST_CTL_U1_VAL,
  },

  /* GPLL2 @ 400 MHz (dynamic config--dedicated to BIMC). */
  .GPLL2_Cfg =
  {
    .nRefCount     = 0,
    .nPLLModeAddr  = HWIO_ADDR(GCC_GPLL2_MODE),
    .nVoteAddr     = HWIO_ADDR(GCC_SPARE_GPLL_ENA_VOTE),
    .nVoteMask     = HWIO_FMSK(GCC_SPARE_GPLL_ENA_VOTE, GPLL2),
    .nPreDiv       = 1,
    .nPostDivOdd   = 0,
    .nPostDivEven  = 0,
    .nL            = 20,
    .nAlpha        = 0xD555,
    .nCalibrationL = 68,
    .nUserCtl      = TRION_USER_CTL_MAIN_VAL,
    .nUserCtl_U    = TRION_USER_CTL_U_VAL,
    .nUserCtl_U1   = TRION_USER_CTL_U1_VAL,
    .nConfigCtl    = TRION_CONFIG_CTL_VAL,
    .nConfigCtl_U  = TRION_CONFIG_CTL_U_VAL,
    .nConfigCtl_U1 = TRION_CONFIG_CTL_U1_VAL,
    .nTestCtl      = TRION_TEST_CTL_VAL,
    .nTestCtl_U    = TRION_TEST_CTL_U_VAL,
    .nTestCtl_U1   = TRION_TEST_CTL_U1_VAL,
  },

  /* GPLL3 @ 547.2 MHz (dynamic config--dedicated to BIMC). */
  .GPLL3_Cfg =
  {
    .nRefCount     = 0,
    .nPLLModeAddr  = HWIO_ADDR(GCC_GPLL3_MODE), 
    .nVoteAddr     = HWIO_ADDR(GCC_SPARE_GPLL_ENA_VOTE), 
    .nVoteMask     = HWIO_FMSK(GCC_SPARE_GPLL_ENA_VOTE, GPLL3),
    .nPreDiv       = 1,
    .nPostDivOdd   = 0,
    .nPostDivEven  = 0,
    .nL            = 28,
    .nAlpha        = 0x8000,
    .nCalibrationL = 68,
    .nUserCtl      = TRION_USER_CTL_MAIN_VAL,
    .nUserCtl_U    = TRION_USER_CTL_U_VAL,
    .nUserCtl_U1   = TRION_USER_CTL_U1_VAL,
    .nConfigCtl    = TRION_CONFIG_CTL_VAL,
    .nConfigCtl_U  = TRION_CONFIG_CTL_U_VAL,
    .nConfigCtl_U1 = TRION_CONFIG_CTL_U1_VAL,
    .nTestCtl      = TRION_TEST_CTL_VAL,
    .nTestCtl_U    = TRION_TEST_CTL_U_VAL,
    .nTestCtl_U1   = TRION_TEST_CTL_U1_VAL,
  },

  /* GPLL4 @ 806 MHz */
  .GPLL4_Cfg =
  {
    .nRefCount     = 0,
    .nPLLModeAddr  = HWIO_ADDR(GCC_GPLL4_MODE), 
    .nVoteAddr     = HWIO_ADDR(GCC_SPARE_GPLL_ENA_VOTE),
    .nVoteMask     = HWIO_FMSK(GCC_SPARE_GPLL_ENA_VOTE, GPLL4),
    .nPreDiv       = 1,
    .nPostDivOdd   = 0,
    .nPostDivEven  = 0,
    .nL            = 41,
    .nAlpha        = 0xFAAA,
    .nCalibrationL = 68,
    .nUserCtl      = TRION_USER_CTL_MAIN_VAL,
    .nUserCtl_U    = TRION_USER_CTL_U_VAL,
    .nUserCtl_U1   = TRION_USER_CTL_U1_VAL,
    .nConfigCtl    = TRION_CONFIG_CTL_VAL,
    .nConfigCtl_U  = TRION_CONFIG_CTL_U_VAL,
    .nConfigCtl_U1 = TRION_CONFIG_CTL_U1_VAL,
    .nTestCtl      = TRION_TEST_CTL_VAL,
    .nTestCtl_U    = TRION_TEST_CTL_U_VAL,
    .nTestCtl_U1   = TRION_TEST_CTL_U1_VAL,
  },

  /* GPLL5 @ 933 MHz */
  .GPLL5_Cfg =
  {
    .nRefCount     = 0,
    .nPLLModeAddr  = HWIO_ADDR(GCC_GPLL5_MODE), 
    .nVoteAddr     = HWIO_ADDR(GCC_SPARE_GPLL_ENA_VOTE),
    .nVoteMask     = HWIO_FMSK(GCC_SPARE_GPLL_ENA_VOTE, GPLL5),
    .nPreDiv       = 1,
    .nPostDivOdd   = 0,
    .nPostDivEven  = 0,
    .nL            = 48,
    .nAlpha        = 0x9800,
    .nCalibrationL = 68,
    .nUserCtl      = TRION_USER_CTL_MAIN_VAL,
    .nUserCtl_U    = TRION_USER_CTL_U_VAL,
    .nUserCtl_U1   = TRION_USER_CTL_U1_VAL,
    .nConfigCtl    = TRION_CONFIG_CTL_VAL,
    .nConfigCtl_U  = TRION_CONFIG_CTL_U_VAL,
    .nConfigCtl_U1 = TRION_CONFIG_CTL_U1_VAL,
    .nTestCtl      = TRION_TEST_CTL_VAL,
    .nTestCtl_U    = TRION_TEST_CTL_U_VAL,
    .nTestCtl_U1   = TRION_TEST_CTL_U1_VAL,
  },

  /* GPLL6 @ 880 MHz */
  .GPLL6_Cfg =
  {
    .nRefCount     = 0,
    .nPLLModeAddr  = HWIO_ADDR(GCC_GPLL6_MODE), 
    .nVoteAddr     = HWIO_ADDR(GCC_SPARE_GPLL_ENA_VOTE),
    .nVoteMask     = HWIO_FMSK(GCC_SPARE_GPLL_ENA_VOTE, GPLL6),
    .nPreDiv       = 1,
    .nPostDivOdd   = 0,
    .nPostDivEven  = 0,
    .nL            = 45,
    .nAlpha        = 0xD555,
    .nCalibrationL = 68,
    .nUserCtl      = TRION_USER_CTL_MAIN_VAL,
    .nUserCtl_U    = TRION_USER_CTL_U_VAL,
    .nUserCtl_U1   = TRION_USER_CTL_U1_VAL,
    .nConfigCtl    = TRION_CONFIG_CTL_VAL,
    .nConfigCtl_U  = TRION_CONFIG_CTL_U_VAL,
    .nConfigCtl_U1 = TRION_CONFIG_CTL_U1_VAL,
    .nTestCtl      = TRION_TEST_CTL_VAL,
    .nTestCtl_U    = TRION_TEST_CTL_U_VAL,
    .nTestCtl_U1   = TRION_TEST_CTL_U1_VAL,
  },

  /* GPLL7 @ 500 MHz */
  .GPLL7_Cfg =
  {
    .nRefCount     = 0,
    .nPLLModeAddr  = HWIO_ADDR(GCC_GPLL7_MODE), 
    .nVoteAddr     = HWIO_ADDR(GCC_SPARE_GPLL_ENA_VOTE),
    .nVoteMask     = HWIO_FMSK(GCC_SPARE_GPLL_ENA_VOTE, GPLL7),
    .nPreDiv       = 1,
    .nPostDivOdd   = 0,
    .nPostDivEven  = 0,
    .nL            = 26,
    .nAlpha        = 0x0AAA,
    .nCalibrationL = 68,
    .nUserCtl      = TRION_USER_CTL_MAIN_VAL,
    .nUserCtl_U    = TRION_USER_CTL_U_VAL,
    .nUserCtl_U1   = TRION_USER_CTL_U1_VAL,
    .nConfigCtl    = TRION_CONFIG_CTL_VAL,
    .nConfigCtl_U  = TRION_CONFIG_CTL_U_VAL,
    .nConfigCtl_U1 = TRION_CONFIG_CTL_U1_VAL,
    .nTestCtl      = TRION_TEST_CTL_VAL,
    .nTestCtl_U    = TRION_TEST_CTL_U_VAL,
    .nTestCtl_U1   = TRION_TEST_CTL_U1_VAL,
  },

  /* GPLL8 @ 710 MHz */
  .GPLL8_Cfg =
  {
    .nRefCount     = 0,
    .nPLLModeAddr  = HWIO_ADDR(GCC_GPLL8_MODE), 
    .nVoteAddr     = HWIO_ADDR(GCC_SPARE_GPLL_ENA_VOTE),
    .nVoteMask     = HWIO_FMSK(GCC_SPARE_GPLL_ENA_VOTE, GPLL8),
    .nPreDiv       = 1,
    .nPostDivOdd   = 0,
    .nPostDivEven  = 0,
    .nL            = 36,
    .nAlpha        = 0xFAAA,
    .nCalibrationL = 68,
    .nUserCtl      = TRION_USER_CTL_MAIN_VAL,
    .nUserCtl_U    = TRION_USER_CTL_U_VAL,
    .nUserCtl_U1   = TRION_USER_CTL_U1_VAL,
    .nConfigCtl    = TRION_CONFIG_CTL_VAL,
    .nConfigCtl_U  = TRION_CONFIG_CTL_U_VAL,
    .nConfigCtl_U1 = TRION_CONFIG_CTL_U1_VAL,
    .nTestCtl      = TRION_TEST_CTL_VAL,
    .nTestCtl_U    = TRION_TEST_CTL_U_VAL,
    .nTestCtl_U1   = TRION_TEST_CTL_U1_VAL,
  },

  /* GPLL9 @ 806.4 MHz */
  .GPLL9_Cfg =
  {
    .nRefCount     = 0,
    .nPLLModeAddr  = HWIO_ADDR(GCC_GPLL9_MODE), 
    .nVoteAddr     = HWIO_ADDR(GCC_SPARE_GPLL_ENA_VOTE),
    .nVoteMask     = HWIO_FMSK(GCC_SPARE_GPLL_ENA_VOTE, GPLL9),
    .nPreDiv       = 1,
    .nPostDivOdd   = 0,
    .nPostDivEven  = 0,
    .nL            = 42,
    .nAlpha        = 0x1555,
    .nCalibrationL = 68,
    .nUserCtl      = TRION_USER_CTL_MAIN_VAL,
    .nUserCtl_U    = TRION_USER_CTL_U_VAL,
    .nUserCtl_U1   = TRION_USER_CTL_U1_VAL,
    .nConfigCtl    = TRION_CONFIG_CTL_VAL,
    .nConfigCtl_U  = TRION_CONFIG_CTL_U_VAL,
    .nConfigCtl_U1 = TRION_CONFIG_CTL_U1_VAL,
    .nTestCtl      = TRION_TEST_CTL_VAL,
    .nTestCtl_U    = 0x00000004, // A special requirement for SSC.
    .nTestCtl_U1   = TRION_TEST_CTL_U1_VAL,
  },

  /* GPLL10 @ 750 MHz */
  .GPLL10_Cfg[V1] =
  {
    .nRefCount     = 0,
    .nPLLModeAddr  = HWIO_ADDR(GCC_GPLL10_MODE), 
    .nVoteAddr     = HWIO_ADDR(GCC_SPARE_GPLL_ENA_VOTE),
    .nVoteMask     = HWIO_FMSK(GCC_SPARE_GPLL_ENA_VOTE, GPLL10),
    .nPreDiv       = 1,
    .nPostDivOdd   = 0,
    .nPostDivEven  = 0,
    .nL            = 39,
    .nAlpha        = 0x1000,
    .nCalibrationL = 68,
    .nUserCtl      = TRION_USER_CTL_MAIN_VAL,
    .nUserCtl_U    = TRION_USER_CTL_U_VAL,
    .nUserCtl_U1   = TRION_USER_CTL_U1_VAL,
    .nConfigCtl    = TRION_CONFIG_CTL_VAL,
    .nConfigCtl_U  = TRION_CONFIG_CTL_U_VAL,
    .nConfigCtl_U1 = TRION_CONFIG_CTL_U1_VAL,
    .nTestCtl      = TRION_TEST_CTL_VAL,
    .nTestCtl_U    = TRION_TEST_CTL_U_VAL,
    .nTestCtl_U1   = TRION_TEST_CTL_U1_VAL,
  },
  /* GPLL10 @ 1036 MHz */
  .GPLL10_Cfg[V2] =
  {
    .nRefCount     = 0,
    .nPLLModeAddr  = HWIO_ADDR(GCC_GPLL10_MODE), 
    .nVoteAddr     = HWIO_ADDR(GCC_SPARE_GPLL_ENA_VOTE),
    .nVoteMask     = HWIO_FMSK(GCC_SPARE_GPLL_ENA_VOTE, GPLL10),
    .nPreDiv       = 1,
    .nPostDivOdd   = 0,
    .nPostDivEven  = 2,
    .nL            = 0x6B,
    .nAlpha        = 0xEAAA,
    .nCalibrationL = 68,
    .nUserCtl      = TRION_USER_CTL_MAIN_EVEN_VAL,
    .nUserCtl_U    = TRION_USER_CTL_U_VAL,
    .nUserCtl_U1   = TRION_USER_CTL_U1_VAL,
    .nConfigCtl    = TRION_CONFIG_CTL_VAL,
    .nConfigCtl_U  = TRION_CONFIG_CTL_U_VAL,
    .nConfigCtl_U1 = TRION_CONFIG_CTL_U1_VAL,
    .nTestCtl      = TRION_TEST_CTL_VAL,
    .nTestCtl_U    = TRION_TEST_CTL_U_VAL,
    .nTestCtl_U1   = TRION_TEST_CTL_U1_VAL,
  },

  /* AOSS_PLL0 @ 400 MHz */
  .AOSS_PLL0_Cfg =
  {
    .nRefCount     = 0,
    .nPLLModeAddr  = HWIO_ADDR(AOSS_CC_PLL0_MODE), 
    .nVoteAddr     = 0,
    .nVoteMask     = 0,
    .nPreDiv       = 1,
    .nPostDivOdd   = 0,
    .nPostDivEven  = 2,
    .nL            = 20,
    .nAlpha        = 0xD555,
    .nCalibrationL = 68,
    .nUserCtl      = TRION_USER_CTL_MAIN_EVEN_VAL,
    .nUserCtl_U    = TRION_USER_CTL_U_VAL,
    .nUserCtl_U1   = TRION_USER_CTL_U1_VAL,
    .nConfigCtl    = TRION_CONFIG_CTL_VAL,
    .nConfigCtl_U  = TRION_CONFIG_CTL_U_VAL,
    .nConfigCtl_U1 = TRION_CONFIG_CTL_U1_VAL,
    .nTestCtl      = TRION_TEST_CTL_VAL,
    .nTestCtl_U    = TRION_TEST_CTL_U_VAL,
    .nTestCtl_U1   = TRION_TEST_CTL_U1_VAL,
  },

  /* Copied from the xbl_data.py autogenerator */
  .gcc_aggre_noc_clk_src = 
  {
    {HWIO_ADDR(GCC_AGGRE_NOC_CMD_RCGR), 2, MUX_GCC, SRC_CXO, 2, 0, 0, 0}, /*  PERF NONE  - 19.2 MHz */
    {HWIO_ADDR(GCC_AGGRE_NOC_CMD_RCGR), 5, MUX_GCC, SRC_GPLL0, 6, 0, 0, 0}, /*  NOM - 200.0 MHz */
    {HWIO_ADDR(GCC_AGGRE_NOC_CMD_RCGR), 6, MUX_GCC, SRC_GPLL0, 5, 0, 0, 0}, /*  MAX - 240.0 MHz */
    {HWIO_ADDR(GCC_AGGRE_NOC_CMD_RCGR), 5, MUX_GCC, SRC_GPLL0, 6, 0, 0, 0}, /*  DEFAULT - 200.0 MHz */
  },
  .gcc_aggre_noc_lpass_sf_clk_src = 
  {
    {HWIO_ADDR(GCC_AGGRE_NOC_LPASS_SF_CMD_RCGR), 1, MUX_GCC, SRC_CXO, 2, 0, 0, 0}, /*  PERF NONE  - 19.2 MHz */
    {HWIO_ADDR(GCC_AGGRE_NOC_LPASS_SF_CMD_RCGR), 5, MUX_GCC, SRC_GPLL0, 3, 0, 0, 0}, /*  NOM - 400.0 MHz */
    {HWIO_ADDR(GCC_AGGRE_NOC_LPASS_SF_CMD_RCGR), 6, MUX_GCC, SRC_GPLL1, 4, 0, 0, 0}, /*  MAX - 533.0 MHz */
    {HWIO_ADDR(GCC_AGGRE_NOC_LPASS_SF_CMD_RCGR), 5, MUX_GCC, SRC_GPLL0, 3, 0, 0, 0}, /*  DEFAULT - 400.0 MHz */
  },
  .gcc_aggre_noc_south_sf_clk_src = 
  {
    {HWIO_ADDR(GCC_AGGRE_NOC_SOUTH_SF_CMD_RCGR), 1, MUX_GCC, SRC_CXO, 2, 0, 0, 0}, /*  PERF NONE  - 19.2 MHz */
    {HWIO_ADDR(GCC_AGGRE_NOC_SOUTH_SF_CMD_RCGR), 5, MUX_GCC, SRC_GPLL0, 3, 0, 0, 0}, /*  NOM - 400.0 MHz */
    {HWIO_ADDR(GCC_AGGRE_NOC_SOUTH_SF_CMD_RCGR), 6, MUX_GCC, SRC_GPLL1, 4, 0, 0, 0}, /*  MAX - 533.0 MHz */
    {HWIO_ADDR(GCC_AGGRE_NOC_SOUTH_SF_CMD_RCGR), 5, MUX_GCC, SRC_GPLL0, 3, 0, 0, 0}, /*  DEFAULT - 400.0 MHz */
  },
  .gcc_ce1_clk_src = 
  {
    {HWIO_ADDR(GCC_CE1_CMD_RCGR), 1, MUX_GCC, SRC_CXO, 2, 0, 0, 0}, /*  PERF NONE  - 19.2 MHz */
    {HWIO_ADDR(GCC_CE1_CMD_RCGR), 3, MUX_GCC, SRC_GPLL0, 7, 0, 0, 0}, /*  NOM - 171.428571 MHz */
    {HWIO_ADDR(GCC_CE1_CMD_RCGR), 3, MUX_GCC, SRC_GPLL0, 7, 0, 0, 0}, /*  MAX - 171.428571 MHz */
    {HWIO_ADDR(GCC_CE1_CMD_RCGR), 3, MUX_GCC, SRC_GPLL0, 7, 0, 0, 0}, /*  DEFAULT - 171.428571 MHz */
  },
  .gcc_cnoc_periph_clk_src = 
  {
    {HWIO_ADDR(GCC_CNOC_PERIPH_CMD_RCGR), 1, MUX_GCC, SRC_CXO, 2, 0, 0, 0}, /*  PERF NONE  - 19.2 MHz */
    {HWIO_ADDR(GCC_CNOC_PERIPH_CMD_RCGR), 3, MUX_GCC, SRC_GPLL0, 12, 0, 0, 0}, /*  NOM - 100.0 MHz */
    {HWIO_ADDR(GCC_CNOC_PERIPH_CMD_RCGR), 3, MUX_GCC, SRC_GPLL0, 12, 0, 0, 0}, /*  MAX - 100.0 MHz */
    {HWIO_ADDR(GCC_CNOC_PERIPH_CMD_RCGR), 3, MUX_GCC, SRC_GPLL0, 12, 0, 0, 0}, /*  DEFAULT - 100.0 MHz */
  },
  .gcc_config_noc_clk_src = 
  {
    {HWIO_ADDR(GCC_CONFIG_NOC_CMD_RCGR), 1, MUX_GCC, SRC_CXO, 2, 0, 0, 0}, /*  PERF NONE  - 19.2 MHz */
    {HWIO_ADDR(GCC_CONFIG_NOC_CMD_RCGR), 3, MUX_GCC, SRC_GPLL0, 16, 0, 0, 0}, /*  NOM - 75.0 MHz */
    {HWIO_ADDR(GCC_CONFIG_NOC_CMD_RCGR), 3, MUX_GCC, SRC_GPLL0, 16, 0, 0, 0}, /*  MAX - 75.0 MHz */
    {HWIO_ADDR(GCC_CONFIG_NOC_CMD_RCGR), 3, MUX_GCC, SRC_GPLL0, 16, 0, 0, 0}, /*  DEFAULT - 75.0 MHz */
  },
  .gcc_cpuss_axi_clk_src = 
  {
    {HWIO_ADDR(GCC_CPUSS_AXI_CMD_RCGR), 0, MUX_GCC, SRC_CXO, 2, 0, 0, 0}, /*  PERF NONE  - 19.2 MHz */
    {HWIO_ADDR(GCC_CPUSS_AXI_CMD_RCGR), 5, MUX_GCC, SRC_GPLL4, 2, 0, 0, 0}, /*  NOM - 806.0 MHz */
    {HWIO_ADDR(GCC_CPUSS_AXI_CMD_RCGR), 6, MUX_GCC, SRC_GPLL5, 2, 0, 0, 0}, /*  MAX - 933.0 MHz */
    {HWIO_ADDR(GCC_CPUSS_AXI_CMD_RCGR), 5, MUX_GCC, SRC_GPLL4, 2, 0, 0, 0}, /*  DEFAULT - 806.0 MHz */
  },
  .gcc_gpu_memnoc_gfx_clk_src[V1] = 
  {
    {HWIO_ADDR(GCC_GPU_MEMNOC_GFX_CMD_RCGR), 0, MUX_GCC, SRC_CXO, 2, 0, 0, 0}, /*  PERF NONE  - 19.2 MHz */
    {HWIO_ADDR(GCC_GPU_MEMNOC_GFX_CMD_RCGR), 5, MUX_GCC, SRC_GPLL1, 4, 0, 0, 0}, /*  NOM - 533.0 MHz */
    {HWIO_ADDR(GCC_GPU_MEMNOC_GFX_CMD_RCGR), 6, MUX_GCC, SRC_GPLL0, 2, 0, 0, 0}, /*  MAX - 600.0 MHz */
    {HWIO_ADDR(GCC_GPU_MEMNOC_GFX_CMD_RCGR), 5, MUX_GCC, SRC_GPLL1, 4, 0, 0, 0}, /*  DEFAULT - 533.0 MHz */
  },
  .gcc_gpu_memnoc_gfx_clk_src[V2] = 
  {
    {HWIO_ADDR(GCC_GPU_MEMNOC_GFX_CMD_RCGR), 0, MUX_GCC, SRC_CXO, 2, 0, 0, 0}, /*  PERF NONE  - 19.2 MHz */
    {HWIO_ADDR(GCC_GPU_MEMNOC_GFX_CMD_RCGR), 5, MUX_GCC, SRC_GPLL1, 4, 0, 0, 0}, /*  NOM - 533.0 MHz */
    {HWIO_ADDR(GCC_GPU_MEMNOC_GFX_CMD_RCGR), 6, MUX_GCC, SRC_GPLL1, 3, 0, 0, 0}, /*  MAX - 710.666667 MHz */
    {HWIO_ADDR(GCC_GPU_MEMNOC_GFX_CMD_RCGR), 5, MUX_GCC, SRC_GPLL1, 4, 0, 0, 0}, /*  DEFAULT - 533.0 MHz */
  },
  .gcc_memnoc_clk_src[V1] = 
  {
    {HWIO_ADDR(GCC_MEMNOC_CMD_RCGR), 0, MUX_GCC, SRC_CXO, 2, 0, 0, 0}, /*  PERF NONE  - 19.2 MHz */
    {HWIO_ADDR(GCC_MEMNOC_CMD_RCGR), 5, MUX_GCC, SRC_GPLL10, 2, 0, 0, 0}, /*  NOM - 750.0 MHz */
    {HWIO_ADDR(GCC_MEMNOC_CMD_RCGR), 6, MUX_GCC, SRC_GPLL6, 2, 0, 0, 0}, /*  MAX - 880.0 MHz */
    {HWIO_ADDR(GCC_MEMNOC_CMD_RCGR), 5, MUX_GCC, SRC_GPLL10, 2, 0, 0, 0}, /*  DEFAULT - 750.0 MHz */
  },
  .gcc_memnoc_clk_src[V2] = 
  {
    {HWIO_ADDR(GCC_MEMNOC_CMD_RCGR), 0, MUX_GCC, SRC_CXO, 2, 0, 0, 0}, /*  PERF NONE  - 19.2 MHz */
    {HWIO_ADDR(GCC_MEMNOC_CMD_RCGR), 5, MUX_GCC, SRC_GPLL4, 2, 0, 0, 0}, /*  NOM - 806.0 MHz */
    {HWIO_ADDR(GCC_MEMNOC_CMD_RCGR), 6, MUX_GCC, SRC_GPLL5, 2, 0, 0, 0}, /*  MAX - 933.0 MHz */
    {HWIO_ADDR(GCC_MEMNOC_CMD_RCGR), 5, MUX_GCC, SRC_GPLL4, 2, 0, 0, 0}, /*  DEFAULT - 806.0 MHz */
  },
  .gcc_mmu_tcu_clk_src = 
  {
    {HWIO_ADDR(GCC_MMU_TCU_CMD_RCGR), 0, MUX_GCC, SRC_CXO, 2, 0, 0, 0}, /*  PERF NONE  - 19.2 MHz */
    {HWIO_ADDR(GCC_MMU_TCU_CMD_RCGR), 5, MUX_GCC, SRC_GPLL4, 4, 0, 0, 0}, /*  NOM - 403.0 MHz */
    {HWIO_ADDR(GCC_MMU_TCU_CMD_RCGR), 6, MUX_GCC, SRC_GPLL1, 4, 0, 0, 0}, /*  MAX - 533.0 MHz */
    {HWIO_ADDR(GCC_MMU_TCU_CMD_RCGR), 5, MUX_GCC, SRC_GPLL4, 4, 0, 0, 0}, /*  DEFAULT - 403.0 MHz */
  },
  .gcc_mss_q6_memnoc_axi_clk_src = 
  {
    {HWIO_ADDR(GCC_MSS_Q6_MEMNOC_AXI_CMD_RCGR), 0, MUX_GCC, SRC_CXO, 2, 0, 0, 0}, /*  PERF NONE  - 19.2 MHz */
    {HWIO_ADDR(GCC_MSS_Q6_MEMNOC_AXI_CMD_RCGR), 5, MUX_GCC, SRC_GPLL0, 2, 0, 0, 0}, /*  NOM - 600.0 MHz */
    {HWIO_ADDR(GCC_MSS_Q6_MEMNOC_AXI_CMD_RCGR), 6, MUX_GCC, SRC_GPLL1, 3, 0, 0, 0}, /*  MAX - 710.666667 MHz */
    {HWIO_ADDR(GCC_MSS_Q6_MEMNOC_AXI_CMD_RCGR), 5, MUX_GCC, SRC_GPLL0, 2, 0, 0, 0}, /*  DEFAULT - 600.0 MHz */
  },
  .gcc_pcie_phy_refgen_clk_src = 
  {
    {HWIO_ADDR(GCC_PCIE_PHY_REFGEN_CMD_RCGR), 0, MUX_GCC, SRC_CXO, 2, 0, 0, 0}, /*  PERF NONE  - 19.2 MHz */
    {HWIO_ADDR(GCC_PCIE_PHY_REFGEN_CMD_RCGR), 0, MUX_GCC, SRC_GPLL0, 12, 0, 0, 0}, /*  NOM - 100.0 MHz */
    {HWIO_ADDR(GCC_PCIE_PHY_REFGEN_CMD_RCGR), 0, MUX_GCC, SRC_GPLL0, 12, 0, 0, 0}, /*  MAX - 100.0 MHz */
    {HWIO_ADDR(GCC_PCIE_PHY_REFGEN_CMD_RCGR), 0, MUX_GCC, SRC_GPLL0, 12, 0, 0, 0}, /*  DEFAULT - 100.0 MHz */
  },
  .gcc_qupv3_wrap0_core_2x_clk_src = 
  {
    {HWIO_ADDR(GCC_QUPV3_WRAP0_CORE_2X_CMD_RCGR), 1, MUX_GCC, SRC_CXO, 2, 0, 0, 0}, /*  PERF NONE  - 19.2 MHz */
    {HWIO_ADDR(GCC_QUPV3_WRAP0_CORE_2X_CMD_RCGR), 5, MUX_GCC, SRC_GPLL0, 6, 0, 0, 0}, /*  NOM - 200.0 MHz */
    {HWIO_ADDR(GCC_QUPV3_WRAP0_CORE_2X_CMD_RCGR), 5, MUX_GCC, SRC_GPLL0, 6, 0, 0, 0}, /*  MAX - 200.0 MHz */
    {HWIO_ADDR(GCC_QUPV3_WRAP0_CORE_2X_CMD_RCGR), 5, MUX_GCC, SRC_GPLL0, 6, 0, 0, 0}, /*  DEFAULT - 200.0 MHz */
  },
  .gcc_sys_noc_clk_src = 
  {
    {HWIO_ADDR(GCC_SYS_NOC_CMD_RCGR), 1, MUX_GCC, SRC_CXO, 2, 0, 0, 0}, /*  PERF NONE  - 19.2 MHz */
    {HWIO_ADDR(GCC_SYS_NOC_CMD_RCGR), 5, MUX_GCC, SRC_GPLL0, 6, 0, 0, 0}, /*  NOM - 200.0 MHz */
    {HWIO_ADDR(GCC_SYS_NOC_CMD_RCGR), 6, MUX_GCC, SRC_GPLL0, 5, 0, 0, 0}, /*  MAX - 240.0 MHz */
    {HWIO_ADDR(GCC_SYS_NOC_CMD_RCGR), 5, MUX_GCC, SRC_GPLL0, 6, 0, 0, 0}, /*  DEFAULT - 200.0 MHz */
  },
  .gcc_sys_noc_gc_axi_clk_src = 
  {
    {HWIO_ADDR(GCC_SYS_NOC_GC_AXI_CMD_RCGR), 1, MUX_GCC, SRC_CXO, 2, 0, 0, 0}, /*  PERF NONE  - 19.2 MHz */
    {HWIO_ADDR(GCC_SYS_NOC_GC_AXI_CMD_RCGR), 5, MUX_GCC, SRC_GPLL0, 4, 0, 0, 0}, /*  NOM - 300.0 MHz */
    {HWIO_ADDR(GCC_SYS_NOC_GC_AXI_CMD_RCGR), 6, MUX_GCC, SRC_GPLL1, 6, 0, 0, 0}, /*  MAX - 355.333333 MHz */
    {HWIO_ADDR(GCC_SYS_NOC_GC_AXI_CMD_RCGR), 5, MUX_GCC, SRC_GPLL0, 4, 0, 0, 0}, /*  DEFAULT - 300.0 MHz */
  },
  .gcc_sys_noc_monaq_axi_clk_src = 
  {
    {HWIO_ADDR(GCC_SYS_NOC_MONAQ_AXI_CMD_RCGR), 1, MUX_GCC, SRC_CXO, 2, 0, 0, 0}, /*  PERF NONE  - 19.2 MHz */
    {HWIO_ADDR(GCC_SYS_NOC_MONAQ_AXI_CMD_RCGR), 5, MUX_GCC, SRC_GPLL5, 4, 0, 0, 0}, /*  NOM - 466.5 MHz */
    {HWIO_ADDR(GCC_SYS_NOC_MONAQ_AXI_CMD_RCGR), 6, MUX_GCC, SRC_GPLL1, 4, 0, 0, 0}, /*  MAX - 533.0 MHz */
    {HWIO_ADDR(GCC_SYS_NOC_MONAQ_AXI_CMD_RCGR), 5, MUX_GCC, SRC_GPLL5, 4, 0, 0, 0}, /*  DEFAULT - 466.5 MHz */
  },
  .gcc_sys_noc_sf_axi_clk_src = 
  {
    {HWIO_ADDR(GCC_SYS_NOC_SF_AXI_CMD_RCGR), 1, MUX_GCC, SRC_CXO, 2, 0, 0, 0}, /*  PERF NONE  - 19.2 MHz */
    {HWIO_ADDR(GCC_SYS_NOC_SF_AXI_CMD_RCGR), 5, MUX_GCC, SRC_GPLL4, 4, 0, 0, 0}, /*  NOM - 403.0 MHz */
    {HWIO_ADDR(GCC_SYS_NOC_SF_AXI_CMD_RCGR), 6, MUX_GCC, SRC_GPLL1, 4, 0, 0, 0}, /*  MAX - 533.0 MHz */
    {HWIO_ADDR(GCC_SYS_NOC_SF_AXI_CMD_RCGR), 5, MUX_GCC, SRC_GPLL4, 4, 0, 0, 0}, /*  DEFAULT - 403.0 MHz */
  },
  .gcc_ufs_phy_axi_clk_src = 
  {
    {HWIO_ADDR(GCC_UFS_PHY_AXI_CMD_RCGR), 0, MUX_GCC, SRC_CXO, 2, 0, 0, 0}, /*  PERF NONE  - 19.2 MHz */
    {HWIO_ADDR(GCC_UFS_PHY_AXI_CMD_RCGR), 0, MUX_GCC, SRC_GPLL0, 4, 0, 0, 0}, /*  NOM - 300.0 MHz */
    {HWIO_ADDR(GCC_UFS_PHY_AXI_CMD_RCGR), 0, MUX_GCC, SRC_GPLL0, 4, 0, 0, 0}, /*  MAX - 300.0 MHz */
    {HWIO_ADDR(GCC_UFS_PHY_AXI_CMD_RCGR), 0, MUX_GCC, SRC_GPLL0, 4, 0, 0, 0}, /*  DEFAULT - 300.0 MHz */
  },
  .gcc_ufs_phy_ice_core_clk_src = 
  {
    {HWIO_ADDR(GCC_UFS_PHY_ICE_CORE_CMD_RCGR), 0, MUX_GCC, SRC_CXO, 2, 0, 0, 0}, /*  PERF NONE  - 19.2 MHz */
    {HWIO_ADDR(GCC_UFS_PHY_ICE_CORE_CMD_RCGR), 0, MUX_GCC, SRC_GPLL0, 4, 0, 0, 0}, /*  NOM - 300.0 MHz */
    {HWIO_ADDR(GCC_UFS_PHY_ICE_CORE_CMD_RCGR), 0, MUX_GCC, SRC_GPLL0, 4, 0, 0, 0}, /*  MAX - 300.0 MHz */
    {HWIO_ADDR(GCC_UFS_PHY_ICE_CORE_CMD_RCGR), 0, MUX_GCC, SRC_GPLL0, 4, 0, 0, 0}, /*  DEFAULT - 300.0 MHz */
  },
  .gcc_ufs_phy_unipro_core_clk_src = 
  {
    {HWIO_ADDR(GCC_UFS_PHY_UNIPRO_CORE_CMD_RCGR), 0, MUX_GCC, SRC_CXO, 2, 0, 0, 0}, /*  PERF NONE  - 19.2 MHz */
    {HWIO_ADDR(GCC_UFS_PHY_UNIPRO_CORE_CMD_RCGR), 0, MUX_GCC, SRC_GPLL0, 4, 0, 0, 0}, /*  NOM - 300.0 MHz */
    {HWIO_ADDR(GCC_UFS_PHY_UNIPRO_CORE_CMD_RCGR), 0, MUX_GCC, SRC_GPLL0, 4, 0, 0, 0}, /*  MAX - 300.0 MHz */
    {HWIO_ADDR(GCC_UFS_PHY_UNIPRO_CORE_CMD_RCGR), 0, MUX_GCC, SRC_GPLL0, 4, 0, 0, 0}, /*  DEFAULT - 300.0 MHz */
  },
  .gcc_ufs_card_2_axi_clk_src = 
  {
    {HWIO_ADDR(GCC_UFS_CARD_2_AXI_CMD_RCGR), 0, MUX_GCC, SRC_CXO, 2, 0, 0, 0}, /*  PERF NONE  - 19.2 MHz */
    {HWIO_ADDR(GCC_UFS_CARD_2_AXI_CMD_RCGR), 0, MUX_GCC, SRC_GPLL0, 4, 0, 0, 0}, /*  NOM - 300.0 MHz */
    {HWIO_ADDR(GCC_UFS_CARD_2_AXI_CMD_RCGR), 0, MUX_GCC, SRC_GPLL0, 4, 0, 0, 0}, /*  MAX - 300.0 MHz */
    {HWIO_ADDR(GCC_UFS_CARD_2_AXI_CMD_RCGR), 0, MUX_GCC, SRC_GPLL0, 4, 0, 0, 0}, /*  DEFAULT - 300.0 MHz */
  },
  .gcc_ufs_card_2_ice_core_clk_src = 
  {
    {HWIO_ADDR(GCC_UFS_CARD_2_ICE_CORE_CMD_RCGR), 0, MUX_GCC, SRC_CXO, 2, 0, 0, 0}, /*  PERF NONE  - 19.2 MHz */
    {HWIO_ADDR(GCC_UFS_CARD_2_ICE_CORE_CMD_RCGR), 0, MUX_GCC, SRC_GPLL0, 4, 0, 0, 0}, /*  NOM - 300.0 MHz */
    {HWIO_ADDR(GCC_UFS_CARD_2_ICE_CORE_CMD_RCGR), 0, MUX_GCC, SRC_GPLL0, 4, 0, 0, 0}, /*  MAX - 300.0 MHz */
    {HWIO_ADDR(GCC_UFS_CARD_2_ICE_CORE_CMD_RCGR), 0, MUX_GCC, SRC_GPLL0, 4, 0, 0, 0}, /*  DEFAULT - 300.0 MHz */
  },
  .gcc_ufs_card_2_unipro_core_clk_src = 
  {
    {HWIO_ADDR(GCC_UFS_CARD_2_UNIPRO_CORE_CMD_RCGR), 0, MUX_GCC, SRC_CXO, 2, 0, 0, 0}, /*  PERF NONE  - 19.2 MHz */
    {HWIO_ADDR(GCC_UFS_CARD_2_UNIPRO_CORE_CMD_RCGR), 0, MUX_GCC, SRC_GPLL0, 4, 0, 0, 0}, /*  NOM - 300.0 MHz */
    {HWIO_ADDR(GCC_UFS_CARD_2_UNIPRO_CORE_CMD_RCGR), 0, MUX_GCC, SRC_GPLL0, 4, 0, 0, 0}, /*  MAX - 300.0 MHz */
    {HWIO_ADDR(GCC_UFS_CARD_2_UNIPRO_CORE_CMD_RCGR), 0, MUX_GCC, SRC_GPLL0, 4, 0, 0, 0}, /*  DEFAULT - 300.0 MHz */
  },
  /* End of copy from xbl_data.py autogenerated configs */

  .CPU_Cfg[V2] =
  {
    /* ClockAPCSSourceType, nLVal, nDiv2x, nReserved, nFreqKHz */
    {APCS_SOURCE_CXO,         0,     0,        0,       19200  },  /* PERF NONE - 19.2MHz  */
    {APCS_SOURCE_AG_EARLY,   77,     0,        0,      1478400 },  /* NOM       - 1478.4 MHz  */
    {APCS_SOURCE_AG_EARLY,   92,     0,        0,      1766400 },  /* MAX       - 1766.4MHz   */
    {APCS_SOURCE_AG_EARLY,   77,     0,        0,      1478400 },  /* DEFAULT   - 1478.4 MHz  */
  },
  .L3_Cfg[V2] =
  {
    /* ClockAPCSSourceType, nLVal, nDiv2x, nReserved, nFreqKHz */
    {APCS_SOURCE_CXO,         0,     0,        0,        19200 },  /* PERF NONE - 19.2MHz  */
    {APCS_SOURCE_AG_EARLY,   74,     0,        0,      1420800 },  /* NOM       - 1420.8MHz   */
    {APCS_SOURCE_AG_EARLY,   86,     0,        0,      1651200 },  /* MAX       - 1651.2MHz   */
    {APCS_SOURCE_AG_EARLY,   74,     0,        0,      1420800 },  /* DEFAULT   - 1420.8MHz   */
  },
  .CPU_Cfg[V1] =
  {
    /* ClockAPCSSourceType, nLVal, nDiv2x, nReserved, nFreqKHz */
    {APCS_SOURCE_CXO,         0,     0,        0,       19200  },  /* PERF NONE - 19.2MHz  */
    {APCS_SOURCE_AG_EARLY,   62,     0,        0,      1190400 },  /* NOM       - 1190.4 MHz  */
    {APCS_SOURCE_AG_EARLY,   62,     0,        0,      1190400 },  /* MAX       - 1190.4 MHz  */
    {APCS_SOURCE_AG_EARLY,   62,     0,        0,      1190400 },  /* DEFAULT   - 1190.4 MHz  */
  },
  .L3_Cfg[V1] =
  {
    /* ClockAPCSSourceType, nLVal, nDiv2x, nReserved, nFreqKHz */
    {APCS_SOURCE_CXO,         0,     0,        0,        19200 },  /* PERF NONE - 19.2MHz  */
    {APCS_SOURCE_AG_EARLY,   60,     0,        0,      1152000 },  /* NOM       - 1152.0   */
    {APCS_SOURCE_AG_EARLY,   60,     0,        0,      1152000 },  /* MAX       - 1152.0   */
    {APCS_SOURCE_AG_EARLY,   60,     0,        0,      1152000 },  /* DEFAULT   - 1152.0   */
  },

  .gcc_qspi_core_clk_src =
  {
    { 19200, {HWIO_ADDR(GCC_QSPI_CORE_CMD_RCGR), 0, MUX_GCC, SRC_CXO,    2, 0, 0, 0}},
    { 75000, {HWIO_ADDR(GCC_QSPI_CORE_CMD_RCGR), 0, MUX_GCC, SRC_GPLL0, 16, 0, 0, 0}},
    {150000, {HWIO_ADDR(GCC_QSPI_CORE_CMD_RCGR), 0, MUX_GCC, SRC_GPLL0,  8, 0, 0, 0}},
    {300000, {HWIO_ADDR(GCC_QSPI_CORE_CMD_RCGR), 0, MUX_GCC, SRC_GPLL0,  4, 0, 0, 0}},
    {0}
  },


  .QUPV3_Cfg =
  {
    {  7372800, {0, 0, MUX_GCC, SRC_GPLL0, 1, 192, 15625, 15625} }, 
    { 14745600, {0, 0, MUX_GCC, SRC_GPLL0, 1, 384, 15625, 15625} },
    { 19200000, {0, 0, MUX_GCC, SRC_CXO,   1,   0,     0,     0} },
    { 96000000, {0, 0, MUX_GCC, SRC_GPLL0, 1,   8,    25,    25} },
  },

  /* SDC configuration : for backwards compatiblity to the old API */
  .SDC_Cfg =
  {
    0,       /* PERF NONE           */
    25000,   /* NOMINAL - 25MHz     */
    50000,   /* MAX - 50MHz         */
    400      /* DEFAULT - At 400KHz */
  },

  /*  SDC extended configurations */
  .SDC_Ext_Cfg =
  {
    {   400,  {0, 0, MUX_GCC, SRC_CXO,   24, 1, 4, 4}},
    { 25000,  {0, 0, MUX_GCC, SRC_GPLL0, 24, 1, 2, 2}},
    { 50000,  {0, 0, MUX_GCC, SRC_GPLL0, 24, 0, 0, 0}},
    {100000,  {0, 0, MUX_GCC, SRC_GPLL0, 12, 0, 0, 0}},
    {201600,  {0, 0, MUX_GCC, SRC_GPLL9,  8, 0, 0, 0}},
  },
  /* Mapping the clock Driver Voltage corner ENUMs to Railway Driver Voltage Corner ENUMs */
  .aClockMapCorner[CLOCK_BOOT_PERF_NONE]    = RAILWAY_NO_REQUEST,
  .aClockMapCorner[CLOCK_BOOT_PERF_NOMINAL] = RAILWAY_NOMINAL,
  .aClockMapCorner[CLOCK_BOOT_PERF_TURBO]   = RAILWAY_TURBO,
  .aClockMapCorner[CLOCK_BOOT_PERF_DEFAULT] = RAILWAY_NOMINAL,
 };
