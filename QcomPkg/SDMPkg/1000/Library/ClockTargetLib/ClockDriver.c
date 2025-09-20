/**
==============================================================================

  @file ClockDriver.c

  Clock driver functions for PLL and clock management

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
#include "ClockBoot.h"
#include "ClockHWIO.h"
#include "busywait.h"
#include "ClockDriver.h"
#include "ClockBSP.h"

/*=========================================================================
      Macro Definitions
==========================================================================*/
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
 * Definitions for configuring the PLL in FSM Mode
 */
#define CLOCK_PLL_BIAS_COUNT_VAL  (0x0 << GPLL_SHFT(MODE, PLL_BIAS_COUNT))
#define CLOCK_PLL_LOCK_COUNT_VAL  (0x0 << GPLL_SHFT(MODE, PLL_LOCK_COUNT))

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
 * PLL lock timeout (2x max locking time).
 */
#define CLOCK_PLL_LOCK_TIMEOUT_US 200

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
 * Common root clock command fields/masks (*_CMD_RCGR)
 */
#define CLOCK_CMD_CGR_ROOT_OFF_FMSK                                  0x80000000
#define CLOCK_CMD_CGR_ROOT_EN_FMSK                                   0x00000002
#define CLOCK_CMD_CFG_UPDATE_FMSK                                    0x00000001
#define CLOCK_CMD_CFG_UPDATE_SHFT                                    0

/*
 * Common root clock configuration registers and their relative offset to
 * the first register (*_CMD_CGR)
 */
#define CLOCK_CMD_REG_OFFSET                                         0
#define CLOCK_CFG_REG_OFFSET                                         0x4
#define CLOCK_M_REG_OFFSET                                           0x8
#define CLOCK_N_REG_OFFSET                                           0xC
#define CLOCK_D_REG_OFFSET                                           0x10

/*
 * Common root clock config fields/masks (*_CFG_RCGR)
 */
#define CLOCK_CFG_RCGR_MODE_FMSK                                      0x00003000
#define CLOCK_CFG_RCGR_MODE_SHFT                                      0xc
#define CLOCK_CFG_RCGR_SRC_SEL_FMSK                                   0x00000700
#define CLOCK_CFG_RCGR_SRC_SEL_SHFT                                   0x8
#define CLOCK_CFG_RCGR_SRC_DIV_FMSK                                   0x0000001F
#define CLOCK_CFG_RCGR_SRC_DIV_SHFT                                   0

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

/*=========================================================================
      Function Prototypes
==========================================================================*/
extern Clock_RailwayType ClockRailway;
extern Clock_ConfigType Clock_ConfigData;

extern boolean Clock_SourceMapToMux(const ClockConfigMuxType *pConfig, uint32 *nMuxValue );
boolean Clock_ConfigurePLL(ClockConfigPLLType *pConfig);
boolean Clock_SourceMapToGCC(ClockSourceType eSource, uint32 *nMuxValue);
boolean Clock_SourceMapToAOSS(ClockSourceType eSource, uint32 *nMuxValue);
boolean Clock_SourceMapToDisp(ClockSourceType eSource, uint32 *nMuxValue);

/*===========================================================================
                      FUNCTION DECLARATIONS
===========================================================================*/ 

int32 Clock_ChipVersion()
{
  static int32 nVersion = -1; // -1 is invalid init, and requires HW read
  static uint32 nMajorVersion = 0xffffffff;

  if( nVersion >= 0) return nVersion;

  nMajorVersion = HWIO_INF(TCSR_SOC_HW_VERSION, MAJOR_VERSION);
  if(nMajorVersion < 2) {
    nVersion = V1;
  } else {
    nVersion = V2;
  }
  return nVersion;
}

/* ============================================================================
**  Function : Clock_EnablePLL
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
boolean Clock_EnablePLL( ClockConfigPLLType *pConfig )
{
  uint64 nModeAddr, nVoteAddr;
  uint32 nModeVal,  nUserCtlVal, nTimeout = 0;

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
    Clock_ConfigurePLL(pConfig);
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
      if (nTimeout++ >= CLOCK_PLL_LOCK_TIMEOUT_US)
      {
        return FALSE;
      }
      busywait(1);
    }
  }
  /*
   * If there is no voting reg, then it is a non-voted PLL.
   * We follow the usual PLL enable sequence.
   */
  else
  {
    /*
     * Set OPMODE to 0
     */
    outpdw(nModeAddr + GPLL_OFFS(OPMODE), 0);

    /*
     * Assert the reset_n field to put the PLL in standby state.
     */
    nModeVal |= GPLL_FMSK(MODE, PLL_RESET_N);
    outpdw(nModeAddr, nModeVal);

    /*
     * Set OPMODE to Run State
     */
    outpdw(nModeAddr + GPLL_OFFS(OPMODE), PLL_OPMODE_RUN);
    busywait(CLOCK_PLL_WARMUP_TIME_US);

    /*
     * Wait for the PLL to lock within the given timeout period
     */
    while ((inpdw(nModeAddr) & GPLL_FMSK(MODE, PLL_LOCK_DET)) == 0)
    {
      if (nTimeout++ >= CLOCK_PLL_LOCK_TIMEOUT_US)
      {
        return FALSE;
      }

      busywait(1);
    }

    /*
     * Enable PLLOUT_MAIN and EVEN_OUT.
     */
    nUserCtlVal = inpdw(nModeAddr + GPLL_OFFS(USER_CTL));
    nUserCtlVal |= GPLL_FMSK(USER_CTL, PLLOUT_MAIN);
    nUserCtlVal |= GPLL_FMSK(USER_CTL, PLLOUT_EVEN);
    outpdw(nModeAddr + GPLL_OFFS(USER_CTL), nUserCtlVal);

    /*
     * Finally, Assert the PLL Output control bit.
     */
    nModeVal |= GPLL_FMSK(MODE, PLL_OUTCTRL);
    outpdw(nModeAddr, nModeVal);
  }

  return TRUE;

}


/* ============================================================================
**  Function : Clock_DisablePLL
** ============================================================================
*/
/*!
    PLL shutdown. When in the off state, the bias is off and draws no power.
    This procedure results in calibration being required upon the next enable.

    @param pConfig -  [IN] PLL configuration structure
    @return
    TRUE -- The PLL was succesfully disabled.
    FALSE -- Disable failed.

    @dependencies None.

    @sa None.
*/
boolean Clock_DisablePLL( ClockConfigPLLType *pConfig )
{
  uint64 nModeAddr;
  uint32 nModeVal;
  uintnt nVoteVal = 0x0;

  if ( (pConfig == NULL) || (pConfig->nPLLModeAddr == 0) )
  {
    return FALSE;
  }

  nModeAddr = pConfig->nPLLModeAddr;

  /* Read current mode value. */
  nModeVal  = inp32(nModeAddr);

  if (nModeVal & GPLL_FMSK(MODE, PLL_VOTE_FSM_ENA))
  {
    if(pConfig->nVoteAddr != 0) 
    {
      nVoteVal = inp32(pConfig->nVoteAddr);
      nVoteVal = (nVoteVal & ~pConfig->nVoteMask);
      outp32(pConfig->nVoteAddr, nVoteVal);
      return TRUE;
    }
    return FALSE;
  }
  else
  {
    /* De-assert the OUTCTL field. */
    nModeVal &= ~GPLL_FMSK(MODE, PLL_OUTCTRL);
    outp32(nModeAddr, nModeVal);

    /* Wait 2 ticks of the output main clock. */
    busywait(1);

    /*
     * Assert the reset_n field to put the PLL in standby state.
     */
    nModeVal |= GPLL_FMSK(MODE, PLL_RESET_N);
    outpdw(nModeAddr, nModeVal);

    /*
     * Set OPMODE to STANDBY
     */
    outpdw(nModeAddr + GPLL_OFFS(OPMODE), PLL_OPMODE_STANDBY);

  }
  return TRUE;
}


/* =========================================================================
**  Function : Clock_ConfigurePLL
** =========================================================================*/
/**
  Configures a PLL.

  @param *pConfig [in] -- PLL configuration

*/
boolean Clock_ConfigurePLL(ClockConfigPLLType *pConfig)
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
**  Function : Clock_ConfigMux
** =========================================================================*/
/*!
    Configure a clock mux which turn on the source and switch mux.

    @param pConfig -  [IN] Clock mux config structure
    @return
    TRUE -- Initialization was successful.
    FALSE -- Initialization failed.

    @dependencies
    None.

    @sa None
*/
void Clock_SetClockConfig( const ClockConfigMuxType *pClockCfg )
{
  /* Used while-loop to trap error for easy triage */
  while( !Clock_EnableSource(pClockCfg->eSource) );
  while( !Clock_ConfigMux(pClockCfg) );
}


/* =========================================================================
**  Function : Clock_ConfigMux
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
boolean Clock_ConfigMux (const ClockConfigMuxType *pConfig)
{
  uintnt nCmdRCGRAddr, nCfgRCGRAddr;
  uint32 nCmdRCGRVal,  nCfgRCGRVal;
  uintnt nMAddr, nNAddr, nDAddr;
  uint32 nSource  = 0;
  uint32 nTimeout = 0;

  if( ! Clock_SourceMapToMux( pConfig, &nSource) )
  {
    return FALSE;
  }

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

  /* Set MND counter mode depending on if it is in use. */
  if (pConfig->nM != 0 && (pConfig->nM < pConfig->nN))
  {
    nMAddr = pConfig->nCMDRCGRAddr + CLOCK_M_REG_OFFSET;
    nNAddr = pConfig->nCMDRCGRAddr + CLOCK_N_REG_OFFSET;
    nDAddr = pConfig->nCMDRCGRAddr + CLOCK_D_REG_OFFSET;

    outp32(nMAddr, pConfig->nM);
    outp32(nNAddr, NOT_N_MINUS_M(pConfig));
    outp32(nDAddr, NOT_2D(pConfig));

    nCfgRCGRVal |= ((CLOCK_CFG_CFG_DUAL_EDGE_MODE_VAL << CLOCK_CFG_RCGR_MODE_SHFT)
                    & CLOCK_CFG_RCGR_MODE_FMSK);
  }

  /* Write the final CFG register value. */
  outp32(nCfgRCGRAddr, nCfgRCGRVal);

  /* Trigger the update. */
  nCmdRCGRVal |= CLOCK_CMD_CFG_UPDATE_FMSK;
  outp32(nCmdRCGRAddr, nCmdRCGRVal);

  /* Wait until update finishes. */
  while ( inp32(nCmdRCGRAddr) & CLOCK_CMD_CFG_UPDATE_FMSK )
  {
    if ( nTimeout++ >= CLOCK_UPDATE_TIMEOUT_US )
    {
      return FALSE;
    }

    busywait(1);
  }

  return TRUE;
}

  
/* =========================================================================
**  Function : Clock_TriggerUpdate
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
boolean Clock_TriggerUpdate(uintnt cmdReg, uintnt nMask)
{
  uint32 nTimeout = 0;
  uintnt nVal;

  nVal = inp32(cmdReg) | nMask;

  /* Trigger CMD_REG:UPDATE */
  outp32(cmdReg, nVal);

  /* Wait for CMD_REG:UPDATE to clear, showing the clock switch is complete */
  while (inp32(cmdReg) & nMask)
  {
    if (nTimeout++ >= CLOCK_UPDATE_TIMEOUT_US)
    {
      return FALSE;
    }
    busywait(1);
  }
  return TRUE;
}  
  
/* =========================================================================
**  Function : Clock_ToggleClock
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
boolean Clock_ToggleClock(uintnt CBCR_addr, Clock_CBCRtoggleType toggle_clk)
{
  uintnt CBCR_value;
  
  if(toggle_clk >= NUM_CLK_TOGGLE_TYPES) return FALSE;

  CBCR_value = inp32(CBCR_addr);
  
  if(toggle_clk == CLK_TOGGLE_ENABLE)  
  {
    CBCR_value = CBCR_value | CLK_ENABLE_MSK;
    outp32(CBCR_addr, CBCR_value);  

    /* Poll clock status if the clock is not under HW control */
    if ( (CBCR_value & CLK_HW_CTL_MSK) != CLK_HW_CTL_MSK )
    {
      do
      {
        CBCR_value = inp32(CBCR_addr);
      }while((CBCR_value & CLK_OFF_MSK) != 0);
    }  
  }
  else
  {
    CBCR_value = CBCR_value & ~CLK_ENABLE_MSK;
    outp32(CBCR_addr, CBCR_value);  
  }
  return TRUE;
}


/* ===========================================================================
**  Clock_PowerDomainEnable
** ======================================================================== */

boolean Clock_PowerDomainEnable ( uintnt nGDSCRAddr )
{
  uint32 nVal;
  uintnt nTimeout = 0;

  if( nGDSCRAddr == 0 )
  {
    return FALSE;
  }

  nVal  = inp32(nGDSCRAddr);

  /*
   * Clear the SW PD collapse bit
   */
  nVal &= ~CLK_GDSCR_SW_COLLAPSE_MSK;
  outp32(nGDSCRAddr, nVal);

  /*
   * Wait for PD ON
   */
  while( !(inp32(nGDSCRAddr) & CLK_GDSCR_PWR_ON_MSK) )
  {
    if (nTimeout++ >= CLK_GDSCR_TIMEOUT_US)
    {
      return FALSE;
    }
    busywait(1);
  }
  return TRUE;

} /* END Clock_PowerDomainEnable */


/* ===========================================================================
**  Clock_PowerDomainDisable
** ======================================================================== */

boolean Clock_PowerDomainDisable ( uintnt nGDSCRAddr )
{
  uintnt nVal;

  /* Sanity check */
  if(nGDSCRAddr == 0)
  {
    return FALSE;
  }

  nVal  = inp32(nGDSCRAddr);

  /* Set the SW PD collapse bit */
  nVal |= CLK_GDSCR_SW_COLLAPSE_MSK;
  outp32(nGDSCRAddr, nVal);

  return TRUE;
} /* END Clock_PowerDomainDisable */


boolean (*Clock_MuxMap[NUM_MUX_TYPES])(ClockSourceType, uint32 *) =
{
  Clock_SourceMapToGCC, 
  Clock_SourceMapToAOSS,
  Clock_SourceMapToDisp
};

/*=========================================================================
      Functions
==========================================================================*/

/* ============================================================================
**  Function : Clock_RailwayCfg
** ============================================================================
*/
/*!
    Return a pointer to the Railway configuration data.

   @param  None

   @retval a pointer to the Railway configuration data

*/
Clock_RailwayType *Clock_RailwayConfig( void )
{
  return &ClockRailway;
}

/* ============================================================================
**  Function : Clock_Config
** ============================================================================
*/
/*!
    Return a pointer to the configuration data.

   @param  None

   @retval a pointer to the configuration data

*/
Clock_ConfigType *Clock_Config( void )
{
  return &Clock_ConfigData;
}

/* ============================================================================
**  Function : Clock_SourceMapToMux
** ============================================================================
*/
boolean Clock_SourceMapToMux
(
  const ClockConfigMuxType *pConfig,
  uint32 *nMuxValue
)
{
  if( (pConfig == NULL) ||
      (nMuxValue == NULL) ||
      pConfig->eMux >= NUM_MUX_TYPES )
  {
    return FALSE;
  }

  return Clock_MuxMap[pConfig->eMux](pConfig->eSource, nMuxValue);
}


/* ============================================================================
**  Function : Clock_SourceMapToGCC
** ============================================================================
*/
/*!
   Map the source enumeration to a physical mux setting for GCC.

   @param  eSource : The source enumeration to map.
   @param  nMuxValue : output parameter.

   @retval a pointer to the configuration data

*/
boolean Clock_SourceMapToGCC(ClockSourceType eSource, uint32 *nMuxValue)
{
  switch( eSource )
  {
    case SRC_CXO:
      *nMuxValue = 0;
      break;
    case SRC_GPLL0:
      *nMuxValue = 1;
      break;
    case SRC_GPLL2:
    case SRC_GPLL6:
    case SRC_GPLL8:
    case SRC_GPLL9:
      *nMuxValue = 2;
      break;
    case SRC_GPLL5:
    case SRC_GPLL7:
    case SRC_GPLL3:
      *nMuxValue = 3;
      break;
    case SRC_GPLL1:
      *nMuxValue = 4;
      break;
    case SRC_GPLL4:
    case SRC_GPLL10:
      *nMuxValue = 5;
      break;      
    case SRC_GPLL0_EVEN:
      *nMuxValue = 6;
      break;
    default:
      return FALSE;
  }
  return TRUE;
}

/* ============================================================================
**  Function : Clock_SourceMapToAOSS
** ============================================================================
*/
/*!
   Map a ClockSourceType into a physical mux setting for the AOSS muxes.

   @param  None

   @retval a pointer to the configuration data

*/
boolean Clock_SourceMapToAOSS(ClockSourceType eSource, uint32 *nMuxValue)
{
  switch( eSource )
  {
    case SRC_CXO:
      *nMuxValue = 0;
      break;
    case SRC_AOPLL0_EVEN:
      *nMuxValue = 1;
      break;
    case SRC_AOPLL1:
      *nMuxValue = 3;
      break;
    default:
      return FALSE;
  }
  return TRUE;
}

/* ============================================================================
**  Function : Clock_SourceMapToDisp
** ============================================================================
*/
/*!
   Map a ClockSourceType into a physical mux setting for the AOSS muxes.

   @param  None

   @retval a pointer to the configuration data

*/
boolean Clock_SourceMapToDisp(ClockSourceType eSource, uint32 *nMuxValue)
{
  switch( eSource )
  {
    case SRC_CXO:
      *nMuxValue = 0;
      break;
    case SRC_EXT1:
    case SRC_DISP_PLL0:
      *nMuxValue = 1;
      break;
    case SRC_EXT2:
      *nMuxValue = 2;
      break;
    case SRC_GPLL0:
    case SRC_DISP_PLL1:
      *nMuxValue = 4;
      break;
    case SRC_GPLL0_EVEN:
      *nMuxValue = 5;
      break;
    default:
      return FALSE;
  }
  return TRUE;
}
/* ============================================================================
**  Function : Clock_EnableSource
** ============================================================================
*/

boolean Clock_EnableSource( ClockSourceType eSource )
{
  boolean rtrn = TRUE;
  ClockConfigPLLType *pPLLCfg = NULL;
  uint32 nVer = Clock_ChipVersion();

  switch( eSource )
  {
    case SRC_CXO:
      break;

    case SRC_GPLL0:
    case SRC_GPLL0_EVEN:
      pPLLCfg = &Clock_ConfigData.GPLL0_Cfg;
      break;

    case SRC_GPLL1:
      pPLLCfg = &Clock_ConfigData.GPLL1_Cfg;
      break;

    case SRC_GPLL2:
      pPLLCfg = &Clock_ConfigData.GPLL2_Cfg;
      break;

    case SRC_GPLL3:
      pPLLCfg = &Clock_ConfigData.GPLL3_Cfg;
      break;

    case SRC_GPLL4:
      pPLLCfg = &Clock_ConfigData.GPLL4_Cfg;
      break;

    case SRC_GPLL5:
      pPLLCfg = &Clock_ConfigData.GPLL5_Cfg;
      break;

    case SRC_GPLL6:
      pPLLCfg = &Clock_ConfigData.GPLL6_Cfg;
      break;

    case SRC_GPLL7:
      pPLLCfg = &Clock_ConfigData.GPLL7_Cfg;
      break;

    case SRC_GPLL8:
      pPLLCfg = &Clock_ConfigData.GPLL8_Cfg;
      break;

    case SRC_GPLL9:
      pPLLCfg = &Clock_ConfigData.GPLL9_Cfg;
      break;

    case SRC_GPLL10:
      pPLLCfg = &Clock_ConfigData.GPLL10_Cfg[nVer];
      break;

    case SRC_AOPLL0_EVEN:
      pPLLCfg = &Clock_ConfigData.AOSS_PLL0_Cfg;
      break;

    default:
      return FALSE; // Not support PLL
      break;
  }

  if ( pPLLCfg != NULL )
  {
    if ( pPLLCfg->nRefCount == 0 )
    {
      rtrn = Clock_EnablePLL(pPLLCfg);
    }
    pPLLCfg->nRefCount++;
  }
  
  return rtrn;
}

/* ============================================================================
**  Function : Clock_DisableSource
** ============================================================================
*/
boolean Clock_DisableSource( ClockSourceType eSource )
{
  boolean rtrn = FALSE;
  ClockConfigPLLType *pPLLCfg = NULL;
  uint32 nVer = Clock_ChipVersion();

  switch( eSource )
  {
    case SRC_CXO:
      rtrn = TRUE;
      break;

    case SRC_GPLL0:
    case SRC_GPLL0_EVEN:
      /* There is currently no use case to disable GPLL0 in XBL Loader. */
      break;

    case SRC_GPLL1:
      pPLLCfg = &Clock_ConfigData.GPLL1_Cfg;
      break;

    case SRC_GPLL2:
      pPLLCfg = &Clock_ConfigData.GPLL2_Cfg;
      break;

    case SRC_GPLL3:
      pPLLCfg = &Clock_ConfigData.GPLL3_Cfg;
      break;

    case SRC_GPLL4:
      pPLLCfg = &Clock_ConfigData.GPLL4_Cfg;
      break;

    case SRC_GPLL5:
      pPLLCfg = &Clock_ConfigData.GPLL5_Cfg;
      break;

    case SRC_GPLL6:
      pPLLCfg = &Clock_ConfigData.GPLL6_Cfg;
      break;

    case SRC_GPLL7:
      pPLLCfg = &Clock_ConfigData.GPLL7_Cfg;
      break;

    case SRC_GPLL8:
      pPLLCfg = &Clock_ConfigData.GPLL8_Cfg;
      break;

    case SRC_GPLL9:
      pPLLCfg = &Clock_ConfigData.GPLL9_Cfg;
      break;

    case SRC_GPLL10:
      pPLLCfg = &Clock_ConfigData.GPLL10_Cfg[nVer];
      break;

    case SRC_AOPLL0_EVEN:
      pPLLCfg = &Clock_ConfigData.AOSS_PLL0_Cfg;
      break;

    default:
      break;
  }
  
  if ( pPLLCfg != NULL )
  {
    if ( pPLLCfg->nRefCount == 1 )
    {
      rtrn = Clock_DisablePLL(pPLLCfg);
    }
    if ( pPLLCfg->nRefCount > 0 )
      pPLLCfg->nRefCount--;
  }
  
  return rtrn;
}

/* ============================================================================
**  Function : Clock_ConfigureSource
** ============================================================================
*/
boolean Clock_ConfigureSource( ClockSourceType eSource )
{
  boolean rtrn = FALSE;
  uint32 nVer = Clock_ChipVersion();

  switch( eSource )
  {
    case SRC_CXO:
      rtrn = TRUE;
      break;

    case SRC_GPLL0:
    case SRC_GPLL0_EVEN:
      rtrn = Clock_ConfigurePLL(&Clock_ConfigData.GPLL0_Cfg);
      break;

    case SRC_GPLL1:
      rtrn = Clock_ConfigurePLL(&Clock_ConfigData.GPLL1_Cfg);
      break;

    case SRC_GPLL2:
      rtrn = Clock_ConfigurePLL(&Clock_ConfigData.GPLL2_Cfg);
      break;

    case SRC_GPLL3:
      rtrn = Clock_ConfigurePLL(&Clock_ConfigData.GPLL3_Cfg);
      break;

    case SRC_GPLL4:
      rtrn = Clock_ConfigurePLL(&Clock_ConfigData.GPLL4_Cfg);
      break;

    case SRC_GPLL5:
      rtrn = Clock_ConfigurePLL(&Clock_ConfigData.GPLL5_Cfg);
      break;

    case SRC_GPLL6:
      rtrn = Clock_ConfigurePLL(&Clock_ConfigData.GPLL6_Cfg);
      break;

    case SRC_GPLL7:
      rtrn = Clock_ConfigurePLL(&Clock_ConfigData.GPLL7_Cfg);
      break;

    case SRC_GPLL8:
      rtrn = Clock_ConfigurePLL(&Clock_ConfigData.GPLL8_Cfg);
      break;

    case SRC_GPLL9:
      rtrn = Clock_ConfigurePLL(&Clock_ConfigData.GPLL9_Cfg);
      break;

    case SRC_GPLL10:
      rtrn = Clock_ConfigurePLL(&Clock_ConfigData.GPLL10_Cfg[nVer]);
      break;

    case SRC_AOPLL0_EVEN:
      rtrn = Clock_ConfigurePLL(&Clock_ConfigData.AOSS_PLL0_Cfg);
      break;

    default:
      break;
  }
  return rtrn;
}


/* ============================================================================
**  Function : Clock_ToggleEnableList
** ============================================================================
*/
void Clock_ToggleEnableList( uint64* pList )
{
  uint32 i = 0;

  /* Enable list clocks */
  while ( pList[i] != 0 )
  {
    Clock_ToggleClock( pList[i], CLK_TOGGLE_ENABLE );
    i++;
  }
}
