/**
==============================================================================

  @file ClockSDI.c
  
  Clock functions for support System Debug Image driver.  Since SDI image is
  required to be every small memory footprint, clock drivers only brings in
  necessary code to support it.

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


/*==========================================================================

                               INCLUDE FILES

===========================================================================*/
#include "ClockHWIO.h"
#include "ClockDriver.h"

/*=========================================================================
                       MACRO DEFINITIONS
==========================================================================*/

#define CLK_ELEM_SIZE(array)      sizeof((array)[0])
#define CLK_NUM_ELEM(array)       (sizeof(array) / CLK_ELEM_SIZE(array))

#define CLK_BITS_ARE_CLR(nVal, nMask)    (((nVal) & (nMask)) == 0)
#define CLK_BITS_ARE_SET(nVal, nMask)    (((nVal) & (nMask)) == (nMask))

#define CLK_ENABLE_MSK 0x00000001
#define CLK_OFF_MSK    0x80000000
#define CLK_GDSCR_SW_COLLAPSE_MSK 0x00000001
#define CLK_GDSCR_PWR_ON_MSK      0x80000000
#define CLK_GDSCR_TIMEOUT_US      150



/*
 *  PLL register offsets.
 */
#if defined(HWIO_GCC_GPLL0_MODE_ADDR)
  #define GPLL_OFFS(x)  (HWIO_ADDR(GCC_GPLL0_##x) - HWIO_ADDR(GCC_GPLL0_MODE))
  #define GPLL_FMSK(x,m)  HWIO_FMSK(GCC_GPLL0_##x, m)
  #define GPLL_SHFT(x,m)  HWIO_SHFT(GCC_GPLL0_##x, m)
#elif defined(HWIO_SPCC_PLL0_MODE_ADDR)
  #define GPLL_OFFS(x)  (HWIO_ADDR(SPCC_PLL0_##x) - HWIO_ADDR(SPCC_PLL0_MODE))
  #define GPLL_FMSK(x,m)  HWIO_FMSK(SPCC_PLL0_##x, m)
  #define GPLL_SHFT(x,m)  HWIO_SHFT(SPCC_PLL0_##x, m)
#else
  #error "No BASE PLL definitions found in HWIO."
#endif


/* HW_CTL bit mask */
#define CLK_HW_CTL_MSK 0x2


/*
 * Common root clock command fields/masks (*_CMD_RCGR)
 */
#define CLOCK_CMD_CGR_ROOT_OFF_FMSK                                  0x80000000
#define CLOCK_CMD_CGR_ROOT_EN_FMSK                                   0x00000002
#define CLOCK_CMD_CFG_UPDATE_SHFT                                    0

/*
 * Common root clock configuration registers and their relative offset to
 * the first register (*_CMD_CGR)
 */
#define CLOCK_CMD_REG_OFFSET                                         0

/*
 * Common root clock config fields/masks (*_CFG_RCGR)
 */

#define CLOCK_CFG_CFG_DUAL_EDGE_MODE_VAL                             0x2
#define CLOCK_CFG_CFG_BYPASS_MODE_VAL                                0x0

/*
 * CLOCK_PLL_MODE_*
 *
 * Definitions for configuring the PLLn_MODE register.
 */
#define CLOCK_PLL_MODE_ACTIVE  \
  (GPLL_FMSK(MODE, PLL_OUTCTRL)  | \
   GPLL_FMSK(MODE, PLL_RESET_N))

#define PLL_OPMODE_STANDBY 0
#define PLL_OPMODE_RUN     1


/*
 * ClockSDIConfigMuxType
 *
 * Parameters used for configuring a standard clock multiplexer.
 *
 *  nCMDRCGRAddr - The address of the CMD RCGR register.
 *  eSource     - The source to use.
 *  nDiv2x      - The integer (2 * divider) value to use.
 *  nM          - The M value for any M/N counter, or 0 to bypass.
 *  nN          - The N value for any M/N counter.
 *  n2D         - Twice the D value for any M/N counter.
 */
typedef struct ClockSDIConfigMux
{
  uintnt nCMDRCGRAddr;
  uint32 eSource;   // Originally 'ClockSourceType'.
  uint32 nDiv2x;
} ClockSDIConfigMuxType;

/*
 * Definitions for configuring the PLL in FSM Mode.
 */
#define CLOCK_PLL_BIAS_COUNT_VAL  (0x6 << HWIO_SHFT(GCC_GPLL0_MODE, PLL_BIAS_COUNT))
#define CLOCK_PLL_LOCK_COUNT_VAL  (0x0 << HWIO_SHFT(GCC_GPLL0_MODE, PLL_LOCK_COUNT))

/*
 * CLOCK_PLL_INIT_TIME_US
 *
 * Init time in microseconds after de-asserting PLL reset (normal mode).
 */
#define CLOCK_PLL_INIT_TIME_US 5

/*
 * CLOCK_PLL_WARMUP_TIME_US
 *
 * Warm-up time in microseconds after turning on the PLL.
 */
#define CLOCK_PLL_WARMUP_TIME_US 25

/*
 * NOT_2D / NOT_N_MINUS_M
 *
 * Macros to return the inverted value of the 2D field or (N - M)
 * in a type 1 mux structure.  Used to prepare the value for writing
 * to the hardware register field.
 */
#define NOT_2D(mux)         (~(mux)->n2D)
#define NOT_N_MINUS_M(mux)  (~((mux)->nN - (mux)->nM))

/*
 * HALF_DIVIDER
 *
 * Macro to return the normalized half divider for a given mux structure.
 * NOTE: Expecting (2 * divider) value as input.
 */
#define HALF_DIVIDER(mux)  ((mux)->nDiv2x ? (((mux)->nDiv2x) - 1) : 0)

/*
 * PLL lock timeout (2x max locking time).
 */
#define CLOCK_PLL_LOCK_TIMEOUT_US 200



/* Definitions for generalizing clock configuration */
#define CLOCK_CMD_CFG_UPDATE_FMSK   HWIO_FMSK(GCC_SDCC2_APPS_CMD_RCGR, UPDATE)

#define CLOCK_CFG_REG_OFFSET        (HWIO_ADDR(GCC_SDCC2_APPS_CFG_RCGR)-HWIO_ADDR(GCC_SDCC2_APPS_CMD_RCGR))
#define CLOCK_CFG_RCGR_SRC_SEL_FMSK  HWIO_FMSK(GCC_SDCC2_APPS_CFG_RCGR, SRC_SEL)
#define CLOCK_CFG_RCGR_SRC_SEL_SHFT  HWIO_SHFT(GCC_SDCC2_APPS_CFG_RCGR, SRC_SEL)
#define CLOCK_CFG_RCGR_SRC_DIV_FMSK  HWIO_FMSK(GCC_SDCC2_APPS_CFG_RCGR, SRC_DIV)
#define CLOCK_CFG_RCGR_SRC_DIV_SHFT  HWIO_SHFT(GCC_SDCC2_APPS_CFG_RCGR, SRC_DIV)

#define CLOCK_CFG_RCGR_MODE_FMSK     HWIO_FMSK(GCC_SDCC2_APPS_CFG_RCGR, MODE)
#define CLOCK_CFG_RCGR_MODE_SHFT     HWIO_SHFT(GCC_SDCC2_APPS_CFG_RCGR, MODE)
#define CLOCK_CFG_RCGR_MODE_DUAL_EDGE_VAL  0x2
#define CLOCK_CFG_RCGR_MODE_BYPASS_VAL     0x0

#define CLOCK_M_REG_OFFSET         (HWIO_ADDR(GCC_SDCC2_APPS_M)-HWIO_ADDR(GCC_SDCC2_APPS_CMD_RCGR))
#define CLOCK_N_REG_OFFSET         (HWIO_ADDR(GCC_SDCC2_APPS_N)-HWIO_ADDR(GCC_SDCC2_APPS_CMD_RCGR))
#define CLOCK_D_REG_OFFSET         (HWIO_ADDR(GCC_SDCC2_APPS_D)-HWIO_ADDR(GCC_SDCC2_APPS_CMD_RCGR))

/*=========================================================================
      Data 
==========================================================================*/

ClockConfigPLLType ClockSDI_GPLL0 = 
{
  .bConfig       = FALSE,
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
};

ClockConfigPLLType ClockSDI_GPLL2 = 
{
  .bConfig       = FALSE,
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
};

/*=========================================================================
      Function Prototypes
==========================================================================*/
static boolean ClockSDI_TriggerUpdate ( uintnt cmdReg, uintnt nMask );
static boolean ClockSDI_ToggleClock   ( uintnt CBCR_addr, Clock_CBCRtoggleType toggle_clk );
static boolean ClockSDI_ConfigurePLL  (  ClockConfigPLLType *pConfig );
static boolean ClockSDI_ConfigMux     (  ClockSDIConfigMuxType *pConfig );
static boolean ClockSDI_EnablePLL     (  ClockConfigPLLType *pConfig );


/*===========================================================================
                      FUNCTION DECLARATIONS
===========================================================================*/ 

/* ============================================================================
**  Function : ClockSDI_EnablePLL
** ============================================================================
*/
/*!
    Enable a PLL.  If it is voteable, it will be set for FSM mode
    and voted for using the vote register. Doesn't configure the PLL.

    @param pConfig -  [IN] PLL configuration structure
    @return
    TRUE -- Initialization was successful.
    FALSE -- Initialization failed.

    @dependencies
    None.

    @sa None
*/
static boolean ClockSDI_EnablePLL( ClockConfigPLLType *pConfig )
{
  uint64 nModeAddr, nVoteAddr;
  uint32 nModeVal;

  if (!pConfig)
  {
    return FALSE;
  }

  nModeAddr = pConfig->nPLLModeAddr;
  nVoteAddr = pConfig->nVoteAddr;
  nModeVal = inpdw(nModeAddr);

  /*
   * If the PLL is FSM voteable, and voted for, return
   */
  if (nModeVal & GPLL_FMSK(MODE, PLL_VOTE_FSM_ENA))
  {
    if ((inpdw(nVoteAddr) & pConfig->nVoteMask) == pConfig->nVoteMask)
    {
      if((nModeVal & GPLL_FMSK(MODE, PLL_ACTIVE_FLAG)) == GPLL_FMSK(MODE, PLL_ACTIVE_FLAG))
      {
        return TRUE;
      }
    }
  }
  else if ((nModeVal & CLOCK_PLL_MODE_ACTIVE) == CLOCK_PLL_MODE_ACTIVE)
  {
    /*
     * If the PLL is manually enabled, return
     */
    return TRUE;
  }


  /*
   * Only reconfigure PLLs that are off
   */
  if ((nModeVal & GPLL_FMSK(MODE, PLL_VOTE_FSM_ENA)) == 0)
  {
    ClockSDI_ConfigurePLL(pConfig);
  }
  nModeVal = inpdw(nModeAddr);

  /*
   * Enable the PLL. If there is an address for a voting register, then the
   * PLL is in FSM voteable mode. In this case, the enable sequence is
   * handled in hardware, guided by the BIAS and LOCK counts.
   */
  if (nVoteAddr != 0)
  {
    /* Vote the PLL On */
    outpdw(nVoteAddr, inpdw(nVoteAddr) | pConfig->nVoteMask);

    /*
     * Wait for the PLL to go active.
     */
    while ((inpdw(nModeAddr) & GPLL_FMSK(MODE, PLL_ACTIVE_FLAG)) == 0)
    {
    }
  }
  /*
   * If there is no voting reg, then it is a non-voted PLL.
   * We follow the usual PLL enable sequence.
   */
  else
  {
    while(1);  // Non-voting PLLs are not supported.
  }

  return TRUE;

}




/* =========================================================================
**  Function : ClockSDI_ConfigurePLL
** =========================================================================*/
/**
  Configures a PLL.

  @param *pConfig [in] -- PLL configuration

*/
static boolean ClockSDI_ConfigurePLL(ClockConfigPLLType *pConfig)
{
  uint64 nModeAddr;
  uint32 nUserCtlVal, nModeVal = 0;

  if ( (pConfig == NULL) || (pConfig->nPLLModeAddr == 0) )
  {
    return FALSE;
  }

  pConfig->bConfig = TRUE;
  nModeAddr = pConfig->nPLLModeAddr;
  nModeVal = inp32(nModeAddr);
  
  /*
   * If the PLL is FSM voteable, and voted for, return
   */
  if (nModeVal & GPLL_FMSK(MODE, PLL_VOTE_FSM_ENA))
  {
    if ((inpdw(pConfig->nVoteAddr) & pConfig->nVoteMask) == pConfig->nVoteMask)
    {
      if((nModeVal & GPLL_FMSK(MODE, PLL_ACTIVE_FLAG)) == GPLL_FMSK(MODE, PLL_ACTIVE_FLAG))
      {
        return TRUE;
      }
    }
  }

  /*
   * Clear all bits to put PLL in reset, bypass and output disabled.
   */
  outpdw(nModeAddr, 0x0);

  outpdw( nModeAddr + GPLL_OFFS(USER_CTL), pConfig->nUserCtl );
  outpdw( nModeAddr + GPLL_OFFS(USER_CTL_U), pConfig->nUserCtl_U );
  outpdw( nModeAddr + GPLL_OFFS(USER_CTL_U1), pConfig->nUserCtl_U1 );

  outpdw( nModeAddr + GPLL_OFFS(CONFIG_CTL), pConfig->nConfigCtl );
  outpdw( nModeAddr + GPLL_OFFS(CONFIG_CTL_U), pConfig->nConfigCtl_U );
  outpdw( nModeAddr + GPLL_OFFS(CONFIG_CTL_U1), pConfig->nConfigCtl_U1 );

  outpdw( nModeAddr + GPLL_OFFS(TEST_CTL), pConfig->nTestCtl );
  outpdw( nModeAddr + GPLL_OFFS(TEST_CTL_U), pConfig->nTestCtl_U );
  outpdw( nModeAddr + GPLL_OFFS(TEST_CTL_U1), pConfig->nTestCtl_U1 );

  /*
   * Program L, L fractional and CAL_L
   */
  outpdw(nModeAddr + GPLL_OFFS(L_VAL), pConfig->nL);
  outpdw(nModeAddr + GPLL_OFFS(ALPHA_VAL), pConfig->nAlpha);
  outpdw(nModeAddr + GPLL_OFFS(CAL_L_VAL), pConfig->nCalibrationL);

  /*
   * Get the UserCtlVal value and clear out fields we will configure.
   */
  nUserCtlVal = inpdw(nModeAddr + GPLL_OFFS(USER_CTL));
  nUserCtlVal &= ~(GPLL_FMSK(USER_CTL, PRE_DIV_RATIO)       |
                   GPLL_FMSK(USER_CTL, POST_DIV_RATIO_EVEN) |
                   GPLL_FMSK(USER_CTL, POST_DIV_RATIO_ODD));

  /*
   * Program the pre-div value (div-1 to div-2).
   */
  if (pConfig->nPreDiv <= 2 && pConfig->nPreDiv > 0)
  {
    nUserCtlVal |= ((pConfig->nPreDiv - 1) << GPLL_SHFT(USER_CTL, PRE_DIV_RATIO));
  }

  /*
   * Program the post-div ODD  value (div-1, 3, 5 and 7 are supported)
   */
  if ((pConfig->nPostDivOdd == 3) || (pConfig->nPostDivOdd == 5) ||
      (pConfig->nPostDivOdd == 7))
  {
    nUserCtlVal |= ((pConfig->nPostDivOdd) << GPLL_SHFT(USER_CTL, POST_DIV_RATIO_ODD));
  }

  /*
   * Program the post-div EVEN  value (div-1, 2, 4 and 8 are supported)
   */
  if ((pConfig->nPostDivEven == 2) || (pConfig->nPostDivEven == 4) ||
      (pConfig->nPostDivEven == 8))
  {
    nUserCtlVal |= ((pConfig->nPostDivEven - 1) << GPLL_SHFT(USER_CTL, POST_DIV_RATIO_EVEN));
  }

  /* Enable any outputs for this PLL */
  nUserCtlVal |= (pConfig->nUserCtl);

  /*
   * Finally program the PLL_USER_CTL register.
   */
  outpdw(nModeAddr + GPLL_OFFS(USER_CTL), nUserCtlVal);


  /* 
   * If there is an address for a voting register, then the PLL is in
   * FSM voteable mode. In this case, the enable sequence is handled in
   * hardware, guided by the BIAS and LOCK counts.
   */

  if (pConfig->nVoteAddr != 0)
  {
    /*
     * Put the FSM in reset.
     */
    nModeVal |= GPLL_FMSK(MODE, PLL_VOTE_FSM_RESET);
    outpdw(nModeAddr, nModeVal);

    /*
     * Program the FSM portion of the mode register.
     */
    nModeVal &= ~GPLL_FMSK(MODE, PLL_BIAS_COUNT);
    nModeVal &= ~GPLL_FMSK(MODE, PLL_LOCK_COUNT);
    nModeVal |= CLOCK_PLL_BIAS_COUNT_VAL;
    nModeVal |= CLOCK_PLL_LOCK_COUNT_VAL;
    nModeVal |= GPLL_FMSK(MODE, PLL_VOTE_FSM_ENA);
    /*
     * Finally program the Mode register.
     */
    outpdw(nModeAddr, nModeVal);

    /*
     * Take the FSM out of reset.
     */
    nModeVal &= ~GPLL_FMSK(MODE, PLL_VOTE_FSM_RESET);
    outpdw(nModeAddr, nModeVal);
  }

  /* Release the PLL from reset */
  nModeVal |= GPLL_FMSK(MODE, PLL_RESET_N);
  outpdw(nModeAddr, nModeVal);

  return TRUE;
}



/* =========================================================================
**  Function : ClockSDI_ConfigMux
** =========================================================================*/
/*!
    Configure a clock mux. 

    @param pConfig -  [IN] Clock mux config structure
    @return
    TRUE -- Initialization was successful.
    FALSE -- Initialization failed.

    @dependencies
    None.

    @sa None
*/
static boolean ClockSDI_ConfigMux (ClockSDIConfigMuxType *pConfig)
{
  uintnt nCmdRCGRAddr, nCfgRCGRAddr;
  uint32 nCmdRCGRVal,  nCfgRCGRVal;
  uint32 nSource  = 0;

  nSource = pConfig->eSource;

  nCmdRCGRAddr = pConfig->nCMDRCGRAddr;
  nCmdRCGRVal  = inp32(nCmdRCGRAddr);
  nCfgRCGRAddr = pConfig->nCMDRCGRAddr + CLOCK_CFG_REG_OFFSET; 
  nCfgRCGRVal  = inp32(nCfgRCGRAddr);

  /* Clear the fields. */
  nCfgRCGRVal &= ~(CLOCK_CFG_RCGR_SRC_SEL_FMSK |
                  CLOCK_CFG_RCGR_SRC_DIV_FMSK |
                  CLOCK_CFG_RCGR_MODE_FMSK);

  /* Program the source and divider values. */
  nCfgRCGRVal |= (nSource << CLOCK_CFG_RCGR_SRC_SEL_SHFT)
                  & CLOCK_CFG_RCGR_SRC_SEL_FMSK;
  nCfgRCGRVal |= ((HALF_DIVIDER(pConfig) << CLOCK_CFG_RCGR_SRC_DIV_SHFT)
                  & CLOCK_CFG_RCGR_SRC_DIV_FMSK);

  /* Write the final CFG register value. */
  outp32(nCfgRCGRAddr, nCfgRCGRVal);

  /* Trigger the update. */
  nCmdRCGRVal |= CLOCK_CMD_CFG_UPDATE_FMSK;
  outp32(nCmdRCGRAddr, nCmdRCGRVal);

  /* Wait until update finishes. */
  while ( inp32(nCmdRCGRAddr) & CLOCK_CMD_CFG_UPDATE_FMSK );

  return TRUE;
}


/* =========================================================================
**  Function : ClockSDI_TriggerUpdate
** =========================================================================*/
/*!
    Clock switch and wait for UPDATE bit to complete

    @param cmdReg - Address of the CBCR register

    TRUE -- CBCR programming successful.
    FALSE -- CBCR programming failed.

    @dependencies
    None.

    @sa None
*/
static boolean ClockSDI_TriggerUpdate(uintnt cmdReg, uintnt nMask)
{
  //uint32 nTimeout = 0;
  uintnt nVal;

  nVal = inp32(cmdReg) | nMask;

  /* Trigger CMD_REG:UPDATE */
  outp32(cmdReg, nVal);

  /* Wait for CMD_REG:UPDATE to clear, showing the clock switch is complete */
  while (inp32(cmdReg) & nMask)
  {
    // SDI lacks busywait()
    // if (nTimeout++ >= CLOCK_UPDATE_TIMEOUT_US)
    // {
    //   return FALSE;
    // }
    // busywait(1);
  }
  return TRUE;
}


/* =========================================================================
**  Function : ClockSDI_ToggleClock
** =========================================================================*/
/*!
    Enable/Disable a Clock and poll for CLK_OFF BIT. 

    @param CBCR_addr - Address of the CBCR register
           enable :-  enable/disable the CBCR 
    TRUE -- CBCR programming successful.
    FALSE -- CBCR programming failed.

    @dependencies
    None.

    @sa None
*/
static boolean ClockSDI_ToggleClock(uintnt CBCR_addr, Clock_CBCRtoggleType toggle_clk)
{
  uintnt CBCR_value;
  
  if(toggle_clk >= NUM_CLK_TOGGLE_TYPES) return FALSE;

  CBCR_value = inp32(CBCR_addr);
  
  if(toggle_clk == CLK_TOGGLE_ENABLE)  
  {
    CBCR_value = CBCR_value | CLK_ENABLE_MSK;
    outp32(CBCR_addr, CBCR_value);  
    do
    {
      CBCR_value = inp32(CBCR_addr);
    }while((CBCR_value & CLK_OFF_MSK) != 0);
    
  }
  else
  {
    CBCR_value = CBCR_value & ~CLK_ENABLE_MSK;
    outp32(CBCR_addr, CBCR_value);  
  }
  return TRUE;
}




/* ========================================================================
**  Function : Clock_ConfigureDDR
** ======================================================================*/
/*
    Description: Configure all clocks needed for DDR configuration.  This
    extension API is used for bootup and emergency download mode.

    @param None
    @return
    TRUE -- Initialization was successful.
    FALSE -- Initialization failed.

    @dependencies
    None.

    @sa None
*/
boolean Clock_ConfigureDDR( void )
{
  ClockSDIConfigMuxType clkCfgDDR = {HWIO_ADDR(GCC_DDRMC_CH0_ROOT_CMD_RCGR), 2, 4}; // 200 MHz
  ClockSDIConfigMuxType clkCfgSHRM = {HWIO_ADDR(GCC_SHRM_CMD_RCGR), 1, 4};          // 300 MHz
  ClockSDIConfigMuxType clkCfgMemNOC = {HWIO_ADDR(GCC_MEMNOC_CMD_RCGR), 1, 0};      // 600 MHz
  
  (void)ClockSDI_EnablePLL( &ClockSDI_GPLL2 );
  (void)ClockSDI_ConfigMux( &clkCfgDDR );

  /* Enable all MemNOC clocks */
  ClockSDI_ToggleClock(HWIO_GCC_MSS_Q6_MEMNOC_AXI_CBCR_ADDR, CLK_TOGGLE_ENABLE);

  (void)ClockSDI_EnablePLL( &ClockSDI_GPLL0 );
  (void)ClockSDI_ConfigMux( &clkCfgSHRM );
  (void)ClockSDI_ConfigMux( &clkCfgMemNOC );

  ClockSDI_ToggleClock(HWIO_GCC_SHRM_CBCR_ADDR, CLK_TOGGLE_ENABLE);
  ClockSDI_ToggleClock(HWIO_GCC_PIMEM_AHB_CBCR_ADDR, CLK_TOGGLE_ENABLE);
  ClockSDI_ToggleClock(HWIO_GCC_PIMEM_AXI_CBCR_ADDR, CLK_TOGGLE_ENABLE);
  ClockSDI_ToggleClock(HWIO_GCC_SYS_NOC_GC_AXI_CBCR_ADDR, CLK_TOGGLE_ENABLE);

  HWIO_OUTF(GCC_MMNOC_GDSCR, SW_COLLAPSE, 0);
  while( HWIO_INF(GCC_MMNOC_GDSCR, PWR_ON) == 0);

  ClockSDI_ToggleClock(HWIO_GCC_MMNOC_AHB_CFG_CBCR_ADDR, CLK_TOGGLE_ENABLE);
  // ClockSDI_ToggleClock(HWIO_GCC_MMNOC_HF_QX_CBCR_ADDR, CLK_TOGGLE_ENABLE);
  // ClockSDI_ToggleClock(HWIO_GCC_MMNOC_QOSGEN_EXTREF_CBCR_ADDR, CLK_TOGGLE_ENABLE);
  // ClockSDI_ToggleClock(HWIO_GCC_MMNOC_SF_QX_CBCR_ADDR, CLK_TOGGLE_ENABLE);
  // ClockSDI_ToggleClock(HWIO_GCC_MMNOC_TBU_HF0_CBCR_ADDR, CLK_TOGGLE_ENABLE);
  // ClockSDI_ToggleClock(HWIO_GCC_MMNOC_TBU_HF1_CBCR_ADDR, CLK_TOGGLE_ENABLE);
  ClockSDI_ToggleClock(HWIO_GCC_NOC_MMNOC_DCD_XO_CBCR_ADDR, CLK_TOGGLE_ENABLE);

  ClockSDI_ToggleClock( HWIO_ADDR(GCC_DNOC_CFG_CBCR), CLK_TOGGLE_ENABLE );
  ClockSDI_ToggleClock( HWIO_ADDR(GCC_SNOC_QOSGEN_EXTREF_CBCR), CLK_TOGGLE_ENABLE );

  ClockSDI_ToggleClock(HWIO_GCC_DDRSS_XO_CBCR_ADDR, CLK_TOGGLE_ENABLE);
  ClockSDI_ToggleClock(HWIO_GCC_DDRSS_SLEEP_CBCR_ADDR, CLK_TOGGLE_ENABLE);
  // ClockSDI_ToggleClock(HWIO_GCC_DDRSS_MMNOC_SF_QX_CBCR_ADDR, CLK_TOGGLE_ENABLE);
  // ClockSDI_ToggleClock(HWIO_GCC_DDRSS_MMNOC_HF_QX_CBCR_ADDR, CLK_TOGGLE_ENABLE);
  ClockSDI_ToggleClock(HWIO_GCC_DDRSS_TCU_CBCR_ADDR, CLK_TOGGLE_ENABLE);
  ClockSDI_ToggleClock(HWIO_GCC_DDRSS_SYS_NOC_GC_AXI_CBCR_ADDR, CLK_TOGGLE_ENABLE);
  ClockSDI_ToggleClock(HWIO_GCC_DDRSS_SYS_NOC_SF_AXI_CBCR_ADDR, CLK_TOGGLE_ENABLE);

  // CR 2291005 : Hang here during reset crashdump, this next clock was unneeded.
  // ClockSDI_ToggleClock(HWIO_GCC_DDRSS_TURING_AXI_CBCR_ADDR, CLK_TOGGLE_ENABLE);
  ClockSDI_ToggleClock(HWIO_GCC_DDRSS_MSS_Q6_AXI_CBCR_ADDR, CLK_TOGGLE_ENABLE);
  ClockSDI_ToggleClock(HWIO_GCC_DDRSS_GPU_AXI_CBCR_ADDR, CLK_TOGGLE_ENABLE);

  ClockSDI_ToggleClock(HWIO_GCC_DDR_I_HCLK_CBCR_ADDR, CLK_TOGGLE_ENABLE);
  ClockSDI_ToggleClock(HWIO_GCC_DDRMC_CH0_CBCR_ADDR, CLK_TOGGLE_ENABLE);


  return TRUE;
}
