/**
==============================================================================

  @file ClockTools.c

  This file provides clock initialization for starting SDCC clocks at boot.

  Copyright (c) 2016-2017 QUALCOMM Technologies, Inc.  All Rights Reserved.  
  QUALCOMM Proprietary and Confidential.

==============================================================================


==============================================================================

                            EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.


  when      who     what, where, why
  --------  ------  ------------------------------------------------------
  02/08/17  vphan   Support QUP V3 clock set frequency
  11/11/16  vphan   Initial SDM845 revision, branched from MSM8998.

==============================================================================
*/

/*=========================================================================
      Include Files
==========================================================================*/
#include "ClockHWIO.h"
#include "ClockBSP.h"
#include "busywait.h"

/* V1 definitions necessary for backwards compatible support */
#define HWIO_GCC_RX1_USB2_CLKREF_EN__V1_ADDR                                                           (GCC_CLK_CTL_REG_REG_BASE      + 0x0008c014)
#define HWIO_GCC_RX1_USB2_CLKREF_EN__V1_RMSK                                                           0x80000003
#define HWIO_GCC_RX1_USB2_CLKREF_EN__V1_IN          \
        in_dword_masked(HWIO_GCC_RX1_USB2_CLKREF_EN__V1_ADDR, HWIO_GCC_RX1_USB2_CLKREF_EN__V1_RMSK)
#define HWIO_GCC_RX1_USB2_CLKREF_EN__V1_INM(m)      \
        in_dword_masked(HWIO_GCC_RX1_USB2_CLKREF_EN__V1_ADDR, m)
#define HWIO_GCC_RX1_USB2_CLKREF_EN__V1_OUT(v)      \
        out_dword(HWIO_GCC_RX1_USB2_CLKREF_EN__V1_ADDR,v)
#define HWIO_GCC_RX1_USB2_CLKREF_EN__V1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GCC_RX1_USB2_CLKREF_EN__V1_ADDR,m,v,HWIO_GCC_RX1_USB2_CLKREF_EN__V1_IN)
#define HWIO_GCC_RX1_USB2_CLKREF_EN__V1_RX1_USB2_STATUS_BMSK                                           0x80000000
#define HWIO_GCC_RX1_USB2_CLKREF_EN__V1_RX1_USB2_STATUS_SHFT                                                 0x1f
#define HWIO_GCC_RX1_USB2_CLKREF_EN__V1_CREF_ENABLE_BMSK                                                      0x2
#define HWIO_GCC_RX1_USB2_CLKREF_EN__V1_CREF_ENABLE_SHFT                                                      0x1
#define HWIO_GCC_RX1_USB2_CLKREF_EN__V1_RX1_USB2_ENABLE_BMSK                                                  0x1
#define HWIO_GCC_RX1_USB2_CLKREF_EN__V1_RX1_USB2_ENABLE_SHFT                                                  0x0

/*=========================================================================
      Data
==========================================================================*/
struct Clock_RegType 
{
  uint32 nCMDReg;
  uint32 nCBCRReg;
};

const struct Clock_RegType Clock_QUPV3Regs[CLK_QUPV3_NUM_CLKS] = 
{
  { 0,0 },
  { HWIO_ADDR(GCC_QUPV3_WRAP0_S0_CMD_RCGR),  HWIO_FMSK(GCC_APCS_CLOCK_BRANCH_ENA_VOTE_1, QUPV3_WRAP0_S0_CLK_ENA) },
  { HWIO_ADDR(GCC_QUPV3_WRAP0_S1_CMD_RCGR),  HWIO_FMSK(GCC_APCS_CLOCK_BRANCH_ENA_VOTE_1, QUPV3_WRAP0_S1_CLK_ENA) },
  { HWIO_ADDR(GCC_QUPV3_WRAP0_S2_CMD_RCGR),  HWIO_FMSK(GCC_APCS_CLOCK_BRANCH_ENA_VOTE_1, QUPV3_WRAP0_S2_CLK_ENA) },
  { HWIO_ADDR(GCC_QUPV3_WRAP0_S3_CMD_RCGR),  HWIO_FMSK(GCC_APCS_CLOCK_BRANCH_ENA_VOTE_1, QUPV3_WRAP0_S3_CLK_ENA) },
  { HWIO_ADDR(GCC_QUPV3_WRAP0_S4_CMD_RCGR),  HWIO_FMSK(GCC_APCS_CLOCK_BRANCH_ENA_VOTE_1, QUPV3_WRAP0_S4_CLK_ENA) },
  { HWIO_ADDR(GCC_QUPV3_WRAP0_S5_CMD_RCGR),  HWIO_FMSK(GCC_APCS_CLOCK_BRANCH_ENA_VOTE_1, QUPV3_WRAP0_S5_CLK_ENA) },
  { HWIO_ADDR(GCC_QUPV3_WRAP0_S6_CMD_RCGR),  HWIO_FMSK(GCC_APCS_CLOCK_BRANCH_ENA_VOTE_1, QUPV3_WRAP0_S6_CLK_ENA) },
  { HWIO_ADDR(GCC_QUPV3_WRAP0_S7_CMD_RCGR),  HWIO_FMSK(GCC_APCS_CLOCK_BRANCH_ENA_VOTE_1, QUPV3_WRAP0_S7_CLK_ENA) },
  { HWIO_ADDR(GCC_QUPV3_WRAP1_S0_CMD_RCGR),  HWIO_FMSK(GCC_APCS_CLOCK_BRANCH_ENA_VOTE_1, QUPV3_WRAP1_S0_CLK_ENA) },
  { HWIO_ADDR(GCC_QUPV3_WRAP1_S1_CMD_RCGR),  HWIO_FMSK(GCC_APCS_CLOCK_BRANCH_ENA_VOTE_1, QUPV3_WRAP1_S1_CLK_ENA) },
  { HWIO_ADDR(GCC_QUPV3_WRAP1_S2_CMD_RCGR),  HWIO_FMSK(GCC_APCS_CLOCK_BRANCH_ENA_VOTE_1, QUPV3_WRAP1_S2_CLK_ENA) },
  { HWIO_ADDR(GCC_QUPV3_WRAP1_S3_CMD_RCGR),  HWIO_FMSK(GCC_APCS_CLOCK_BRANCH_ENA_VOTE_1, QUPV3_WRAP1_S3_CLK_ENA) },
  { HWIO_ADDR(GCC_QUPV3_WRAP1_S4_CMD_RCGR),  HWIO_FMSK(GCC_APCS_CLOCK_BRANCH_ENA_VOTE_1, QUPV3_WRAP1_S4_CLK_ENA) },
  { HWIO_ADDR(GCC_QUPV3_WRAP1_S5_CMD_RCGR),  HWIO_FMSK(GCC_APCS_CLOCK_BRANCH_ENA_VOTE_1, QUPV3_WRAP1_S5_CLK_ENA) },
  { 0, 0},
  { 0, 0},
  { HWIO_ADDR(GCC_QUPV3_WRAP2_S0_CMD_RCGR),  HWIO_FMSK(GCC_APCS_CLOCK_BRANCH_ENA_VOTE_2, QUPV3_WRAP2_S0_CLK_ENA) },
  { HWIO_ADDR(GCC_QUPV3_WRAP2_S1_CMD_RCGR),  HWIO_FMSK(GCC_APCS_CLOCK_BRANCH_ENA_VOTE_2, QUPV3_WRAP2_S1_CLK_ENA) },
  { HWIO_ADDR(GCC_QUPV3_WRAP2_S2_CMD_RCGR),  HWIO_FMSK(GCC_APCS_CLOCK_BRANCH_ENA_VOTE_2, QUPV3_WRAP2_S2_CLK_ENA) },
  { HWIO_ADDR(GCC_QUPV3_WRAP2_S3_CMD_RCGR),  HWIO_FMSK(GCC_APCS_CLOCK_BRANCH_ENA_VOTE_2, QUPV3_WRAP2_S3_CLK_ENA) },
  { HWIO_ADDR(GCC_QUPV3_WRAP2_S4_CMD_RCGR),  HWIO_FMSK(GCC_APCS_CLOCK_BRANCH_ENA_VOTE_2, QUPV3_WRAP2_S4_CLK_ENA) },
  { HWIO_ADDR(GCC_QUPV3_WRAP2_S5_CMD_RCGR),  HWIO_FMSK(GCC_APCS_CLOCK_BRANCH_ENA_VOTE_2, QUPV3_WRAP2_S5_CLK_ENA) },
  { 0, 0},
  { 0, 0},  
};


struct Clock_SDCCRegAddrType
{
  uint32 nCmd;
  uint32 nApps;
  uint32 nAHB;
};

const struct Clock_SDCCRegAddrType Clock_SDCCRegs[CLK_SDC_NUM_CLKS] = 
{ 
  { 0,0,0 },
  { 0,0,0 },
  { HWIO_ADDR(GCC_SDCC2_APPS_CMD_RCGR), HWIO_ADDR(GCC_SDCC2_APPS_CBCR), HWIO_ADDR(GCC_SDCC2_AHB_CBCR) },
  { 0,0,0 },
  { HWIO_ADDR(GCC_SDCC4_APPS_CMD_RCGR), HWIO_ADDR(GCC_SDCC4_APPS_CBCR), HWIO_ADDR(GCC_SDCC4_AHB_CBCR) },
  { 0,0,0 }
};


void Clock_SetVotableClock( uintnt nVoteRegs, uintnt nField, boolean bEnable )
{
  uintnt nRegVal = inp32( nVoteRegs );

  if ( bEnable )
  {
    nRegVal |= nField;
  }
  else
  {
    nRegVal &= ~(nField);
  }
  outp32 ( nVoteRegs, nRegVal );
}

/* ============================================================================
**  Function : Clock_SetQUPV3Frequency
** ============================================================================
*/
/*!
    Set QUP V3 clock frequency.

    @param 
      eClock   - QUPV3 index 
      nFreqKHz   Clock frequency in KHz

    @return
    TRUE -- Set frequency success
    FALSE -- Set frequency failed.

    @dependencies
    None.

    @sa None
*/
boolean Clock_SetQUPV3Frequency(ClockQUPV3Type eClock, uint32 nFreqKHz)
{
  const Clock_ConfigType *cfg = Clock_Config();
  const ClockFreqMuxCfgType *pCfg = NULL;
  ClockConfigMuxType clkCfg = {0};
  uint32 nIndex;

  /* Check for valid support QUPV3 clocks */
  if ( (eClock >= CLK_QUPV3_NUM_CLKS) || (Clock_QUPV3Regs[eClock].nCMDReg == 0) )
  {
    return FALSE;
  }

  pCfg = cfg->QUPV3_Cfg;

  /* Search for match support frequency */
  for ( nIndex = 0; nIndex < QUPV3_PERF_NUM; nIndex++ )
  {
    /* Match support frequency */
    if ( pCfg[nIndex].nFreqKHz == nFreqKHz )
    {
      break;
    }
  }

  /* Not find matching support frequency */
  if ( nIndex == QUPV3_PERF_NUM )
  {
    return FALSE;
  }

  clkCfg = pCfg[nIndex].Cfg;
  //memcpy( &clkCfg, &(pCfg[nIndex].Cfg), sizeof(ClockConfigMuxType) );
  clkCfg.nCMDRCGRAddr = Clock_QUPV3Regs[eClock].nCMDReg;

  if ( !Clock_EnableSource( clkCfg.eSource )) return FALSE;
  if ( !Clock_ConfigMux(&clkCfg)) return FALSE;

  if (eClock > CLK_QUPV3_WRAP1_S7)
  {
    Clock_SetVotableClock ( HWIO_ADDR(GCC_APCS_CLOCK_BRANCH_ENA_VOTE_2), Clock_QUPV3Regs[eClock].nCBCRReg, TRUE );  
  }
  else
  {
    Clock_SetVotableClock ( HWIO_ADDR(GCC_APCS_CLOCK_BRANCH_ENA_VOTE_1), Clock_QUPV3Regs[eClock].nCBCRReg, TRUE );
  }

  return TRUE;
}

/* ============================================================================
**  Function : Clock_DisableQUPV3Clock
** ============================================================================
*/
/*!
    Disable QUP V3 clock.

    @param 
      eClock   - QUPV3 index 

    @return
      TRUE -- Disable success
      FALSE -- Disable failed.

    @dependencies
      None.

    @sa None
*/
boolean Clock_DisableQUPV3Clock(ClockQUPV3Type eClock)
{

  /* Check for valid support QUPV3 clocks */
  if ( (eClock >= CLK_QUPV3_NUM_CLKS) || (Clock_QUPV3Regs[eClock].nCBCRReg == 0) )
  {
    return FALSE;
  }

  Clock_SetVotableClock ( HWIO_ADDR(GCC_APCS_CLOCK_BRANCH_ENA_VOTE_1), Clock_QUPV3Regs[eClock].nCBCRReg, FALSE );
  return TRUE;
}

/* ============================================================================
**  Function : Clock_AGGRENOCInit
** ============================================================================
*/
/*!
    Configure AGGRE NOC clocks.

    @param None.
    @return None.

    @dependencies
    None.

    @sa None
*/

void Clock_AGGRENOCInit( void )
{
  static boolean bInit = FALSE;

  /* Init Aggre NOC only one time */
  if ( !bInit )
  {
    Clock_PowerDomainEnable( HWIO_GCC_ANOC_TBU_GDSCR_ADDR );
    Clock_ToggleClock(HWIO_GCC_AGGRE_NOC_AUDIO_TBU_CBCR_ADDR, CLK_TOGGLE_ENABLE);
    Clock_ToggleClock(HWIO_GCC_AGGRE_NOC_PCIE_TBU_CBCR_ADDR, CLK_TOGGLE_ENABLE);
    Clock_ToggleClock(HWIO_GCC_AGGRE_NOC_TBU1_CBCR_ADDR, CLK_TOGGLE_ENABLE);
    Clock_ToggleClock(HWIO_GCC_AGGRE_NOC_TBU2_CBCR_ADDR, CLK_TOGGLE_ENABLE);

    Clock_ToggleClock(HWIO_GCC_AGGRE_NOC_QOSGEN_EXTREF_CBCR_ADDR, CLK_TOGGLE_ENABLE);
    bInit = TRUE;
  }
}


/* ============================================================================
**  Function : Clock_SetSDCClockFrequencyExt
** ============================================================================
*/
/*!
    Configure SDC clock to a specific perf level.

    @param eClockPerfLevel   -  [IN] Clock frequency level
           eClock            -  [IN] SDC clock to configure
    @return
    TRUE -- Initialization was successful.
    FALSE -- Initialization failed.

    @dependencies
    None.

    @sa None
*/
uint32 Clock_SetSDCClockFrequencyExt
(
  uint32 nFreqKHz,
  ClockSDCType eClock
)
{
  ClockConfigMuxType SDCCCfg = {0};
  const ClockFreqMuxCfgType *pCfg = NULL;
  uint32 n;
  const Clock_ConfigType *cfg = Clock_Config();

  /* Check for a valid SDC */
  if ( (eClock >= CLK_SDC_NUM_CLKS) || (Clock_SDCCRegs[eClock].nCmd == 0) )
  {
    return FALSE;
  }

  /* Copy the config to the local so the nCMDRCGRAddr can be modified. */
  pCfg = cfg->SDC_Ext_Cfg;

  /* Find the nearest frequency that matches the request */
  for(n = 0; pCfg[n].nFreqKHz != 0; n++)
  {
    if(pCfg[n].nFreqKHz >= nFreqKHz)
    {
      break;
    }
  }
  if ( pCfg[n].nFreqKHz == 0 )
  {
    return FALSE;
  }

  /* A configuration is found.  Set it */
  SDCCCfg = pCfg[n].Cfg; /* copy struct and fix it */
  SDCCCfg.nCMDRCGRAddr = Clock_SDCCRegs[eClock].nCmd;

  /*
   * Enable access to the config NOC.
   */
  
  /* SDM1000 has SDCC on West Tile, so the clocks that it needs default on.  
     Those clocks are:
     GCC_CFG_NOC_WEST_AHB_CLK
     GCC_CNOC_PERIPH_WEST_CLK
     GCC_AGGRE_NOC_WEST_AXI_CLK
   */
 

  if ( (!Clock_EnableSource(SDCCCfg.eSource)) ||
      (!Clock_ConfigMux(&SDCCCfg)) ||
      (!Clock_ToggleClock(Clock_SDCCRegs[eClock].nApps, CLK_TOGGLE_ENABLE)) ||
      (!Clock_ToggleClock(Clock_SDCCRegs[eClock].nAHB, CLK_TOGGLE_ENABLE)) )
  {
    return FALSE;
  }

  return (pCfg[n].nFreqKHz? TRUE : FALSE);
}


/* ============================================================================
**  Function : Clock_SetSDCClockFrequency
** ============================================================================
*/
/*!
    Configure SDC clock to a specific perf level.

    @param eClockPerfLevel   -  [IN] SDC Clock perf level
           eClock            -  [IN] SDC clock to configure
    @return
    TRUE -- Initialization was successful.
    FALSE -- Initialization failed.

    @dependencies
    None.

    @sa None
*/
boolean Clock_SetSDCClockFrequency
(
  ClockBootPerfLevelType ePerfLevel,
  ClockSDCType eClock
)
{
  const Clock_ConfigType *cfg = Clock_Config();

  if(ePerfLevel >= CLOCK_BOOT_PERF_NUM) return FALSE;
 
  return Clock_SetSDCClockFrequencyExt( cfg->SDC_Cfg[ePerfLevel], eClock);

} /* END Clock_SetSDCClockFrequency */


/* ========================================================================
**  Function : Clock_ReplaceUSBBootClockEnable
** ======================================================================*/
/*
    Description: The register GCC_USB_BOOT_CLOCK_CTL turns on all the clocks
    necessary to access USB from the APPS.  This function must be ported
    on every new MSM.  It replaces that single register with all the clock
    enables that it replaced.  This allows for fine grained control of these
    clocks on an individual basis.

    @param None
    @return None

    @dependencies
    None.

    @sa None
*/
void Clock_ReplaceUSBBootClockEnable()
{
  uint32 nVer=Clock_ChipVersion();

  if ( HWIO_INF(GCC_USB_BOOT_CLOCK_CTL, CLK_ENABLE) != 0 )
  { 
    Clock_AGGRENOCInit();

    Clock_ToggleClock(HWIO_GCC_CFG_NOC_USB3_PRIM_AXI_CBCR_ADDR, CLK_TOGGLE_ENABLE);
    Clock_ToggleClock(HWIO_GCC_CFG_NOC_USB3_SEC_AXI_CBCR_ADDR, CLK_TOGGLE_ENABLE);
    
    Clock_ToggleClock(HWIO_GCC_MSS_MFAB_AXIS_CBCR_ADDR, CLK_TOGGLE_ENABLE);

    Clock_ToggleClock(HWIO_GCC_SYS_NOC_AHB_CFG_CBCR_ADDR, CLK_TOGGLE_ENABLE);
    Clock_ToggleClock(HWIO_GCC_SYS_NOC_AXI_CBCR_ADDR, CLK_TOGGLE_ENABLE);
    Clock_ToggleClock(HWIO_GCC_SYS_NOC_SF_AXI_CBCR_ADDR, CLK_TOGGLE_ENABLE);

    Clock_ToggleClock(HWIO_GCC_USB30_PRIM_MASTER_CBCR_ADDR, CLK_TOGGLE_ENABLE);
    Clock_ToggleClock(HWIO_GCC_USB30_PRIM_MOCK_UTMI_CBCR_ADDR, CLK_TOGGLE_ENABLE);
    Clock_ToggleClock(HWIO_GCC_USB30_PRIM_SLEEP_CBCR_ADDR, CLK_TOGGLE_ENABLE);

    /* Don't poll this clock for enable since the clock source from PHY which may not be out of reset yet */
    HWIO_OUTF(GCC_USB3_PRIM_PHY_AUX_CBCR, CLK_ENABLE, 1);
    HWIO_OUTF(GCC_USB3_PRIM_PHY_COM_AUX_CBCR, CLK_ENABLE, 1);
    HWIO_OUTF(GCC_USB3_PRIM_PHY_PIPE_CBCR, CLK_ENABLE, 1);

    /* Now disable the master boot control, allowing each of those above to replace */
    HWIO_OUTF(GCC_USB_BOOT_CLOCK_CTL, CLK_ENABLE, 0);
  }

  if(V1 == nVer)
  {
    /* Napali PHYs all use SE REF CLK as POR and DIFF REF CLK used for QUSB2 PHY can be OFF by default.
     * HW default is ON.
     */
    HWIO_OUTF(GCC_RX1_USB2_CLKREF_EN__V1, RX1_USB2_ENABLE, 0);
  }
}


/* ========================================================================
**  Function : Clock_ReplaceUFSBootClockEnable
** ======================================================================*/
/*
    Description: The register GCC_UFS_PHY_BOOT_CLOCK_CTL turns on all the clocks
    necessary to access UFS from the APPS.  This function must be ported
    on every new MSM.  It replaces that single register with all the clock
    enables that it replaced.  This allows for fine grained control of these
    clocks on an individual basis.

    @param None
    @return None

    @dependencies
    None.

    @sa None
*/
void Clock_ReplaceUFSBootClockEnable()
{
  if ( HWIO_INF(GCC_UFS_PHY_BOOT_CLOCK_CTL, CLK_ENABLE) != 0 )
  { 
    Clock_AGGRENOCInit();
    Clock_ToggleClock(HWIO_GCC_DDRSS_SYS_NOC_SLAVE_AXI_CBCR_ADDR, CLK_TOGGLE_ENABLE);
    Clock_ToggleClock(HWIO_GCC_QUPV3_WRAP_0_M_AHB_CBCR_ADDR, CLK_TOGGLE_ENABLE);
    Clock_ToggleClock(HWIO_GCC_QUPV3_WRAP_1_M_AHB_CBCR_ADDR, CLK_TOGGLE_ENABLE);
    Clock_ToggleClock(HWIO_GCC_UFS_PHY_AHB_CBCR_ADDR, CLK_TOGGLE_ENABLE);
    Clock_ToggleClock(HWIO_GCC_UFS_PHY_AXI_CBCR_ADDR, CLK_TOGGLE_ENABLE);
    Clock_ToggleClock(HWIO_GCC_UFS_PHY_ICE_CORE_CBCR_ADDR, CLK_TOGGLE_ENABLE);
    Clock_ToggleClock(HWIO_GCC_UFS_PHY_PHY_AUX_CBCR_ADDR, CLK_TOGGLE_ENABLE);
    Clock_ToggleClock(HWIO_GCC_UFS_PHY_UNIPRO_CORE_CBCR_ADDR, CLK_TOGGLE_ENABLE);
    HWIO_OUTF(GCC_UFS_PHY_RX_SYMBOL_0_CBCR, CLK_ENABLE, 1);
    HWIO_OUTF(GCC_UFS_PHY_RX_SYMBOL_1_CBCR, CLK_ENABLE, 1);
    HWIO_OUTF(GCC_UFS_PHY_TX_SYMBOL_0_CBCR, CLK_ENABLE, 1);

    /* Now disable the master boot control, allowing each of those above to replace */
    HWIO_OUTF(GCC_UFS_PHY_BOOT_CLOCK_CTL, CLK_ENABLE, 0);
    
  }
}


/* ========================================================================
**  Function : Clock_InitAHB2PHY
** ======================================================================*/
/*!
    Initialize the clocks necessary to access the PHY and REFGEN.

    @param
      None.

    @return
      None.

    @dependencies
      None.

    @sa None
*/

void Clock_InitAHB2PHY( void )
{
  Clock_ToggleClock(HWIO_ADDR(GCC_CM_PHY_REFGEN1_CBCR), 1);
  Clock_ToggleClock(HWIO_ADDR(GCC_CM_PHY_REFGEN2_CBCR), 1);
}

/* ============================================================================
**  Function : Clock_InitUSB
** ============================================================================
*/
/*!
    Configure USB clocks.

    @param None.
    @return
    TRUE -- Initialization was successful.
    FALSE -- Initialization failed.

    @dependencies
    None.

    @sa None
*/
boolean Clock_InitUSB(void)
{
  ClockConfigMuxType USB30Cfg = {0};
  ClockConfigMuxType USB30_MockCfg = {0};
    
  /* This is redundant with BootROM, except on the debugger tools, 
   * because the JTAG tools do not use BootROM */
  Clock_InitAHB2PHY();

  /* Configured gcc_usb30_prim_master_clk to 200 MHz */
  USB30Cfg.nCMDRCGRAddr = HWIO_ADDR(GCC_USB30_PRIM_MASTER_CMD_RCGR);
  USB30Cfg.eSource = SRC_GPLL0; 
  USB30Cfg.nDiv2x = 6;

  /* Configure gcc_usb30_prim_mock_utmi_clk to 60 MHz */
  USB30_MockCfg.nCMDRCGRAddr = HWIO_ADDR(GCC_USB30_PRIM_MOCK_UTMI_CMD_RCGR);
  USB30_MockCfg.eSource = SRC_GPLL0;
  USB30_MockCfg.nDiv2x = 20;

  Clock_AGGRENOCInit();

  if (!Clock_ConfigMux(&USB30Cfg))
  {
    return FALSE;
  }

  HWIO_OUTF(GCC_AGGRE_USB3_PRIM_AXI_CBCR, CLK_ENABLE, 1);
  HWIO_OUTF(GCC_USB30_PRIM_MASTER_CBCR, CLK_ENABLE, 1);
  HWIO_OUTF(GCC_USB30_PRIM_SLEEP_CBCR, CLK_ENABLE, 1);

  // Configure the Mock UTMI clock
  if(!Clock_ConfigMux(&USB30_MockCfg))
  {
    return FALSE; 
  }
  HWIO_OUTF(GCC_USB30_PRIM_MOCK_UTMI_CBCR, CLK_ENABLE, 1);
 
  HWIO_OUTF(GCC_USB3_PRIM_PHY_AUX_CBCR, CLK_ENABLE, 1);
  HWIO_OUTF(GCC_USB3_PRIM_PHY_COM_AUX_CBCR, CLK_ENABLE, 1);
   
  HWIO_OUTF(GCC_CFG_NOC_USB3_PRIM_AXI_CBCR, CLK_ENABLE, 1);
  HWIO_OUTF(GCC_USB3_PRIM_PHY_PIPE_CBCR, CLK_ENABLE, 1);

  return TRUE;
} /* END Clock_InitUSB */


/* ============================================================================
**  Function : Clock_USB30_PipeClkSrcSel
** ============================================================================
*/
/*!
    Select USB3 PIPE clock source.

    @param eSrc -  [IN] Clock source choice for the USB3.0 PHY
    @return
    TRUE -- Initialization was successful.
    FALSE -- Initialization failed.
    @dependencies  None.
    @sa None
*/
boolean Clock_USB30_PipeClkSrcSel(ClockUSB30PipeClkSrcType eSrc)
{
  return TRUE;
}  /* END Clock_USB30_PipeClkSrcSel */


/* ============================================================================
**  Function : Clock_Usb30EnableSWCollapse
** ============================================================================
*/
/*!
    Enable SW Collapse for USB30

    @param Enable/Disable.
    @return
    TRUE always

    @dependencies
    None.

    @sa None
   */
boolean Clock_Usb30EnableSWCollapse(boolean enable)
{
  /* Enable/Disable SW PowerCollapse for USB30 */
  HWIO_OUTF(GCC_USB30_PRIM_GDSCR, SW_COLLAPSE, enable ? 1:0 );
  return TRUE;
}


/* ============================================================================
**  Function : Clock_Usb30GetSWCollapse
** ============================================================================
*/
/*!
    Returns the status of SW Collapse for USB30

    @param None
    @return
    TRUE if enabled
    FALSE if disabled

    @dependencies
    None.

    @sa None
*/
boolean Clock_Usb30GetSWCollapse(void)
{
  uint8 sw_collapse = HWIO_INF(GCC_USB30_PRIM_GDSCR, SW_COLLAPSE);
  return (sw_collapse ? TRUE : FALSE);	
}


/* ============================================================================
**  Function : Clock_DisableUSB
** ============================================================================
*/
/*!
    Disable USB clocks.

    @param None.
    @return
    TRUE -- Disable was successful.
    FALSE -- Disable failed.

    @dependencies
    None.

    @sa None
*/
boolean Clock_DisableUSB(void)
{
  /* Don't pool this clock since it sources from PHY which may not be out of reset yet */
  HWIO_OUTF(GCC_USB3_PRIM_PHY_PIPE_CBCR, CLK_ENABLE, 0);

  if( !Clock_ToggleClock(HWIO_GCC_AGGRE_USB3_PRIM_AXI_CBCR_ADDR, CLK_TOGGLE_DISABLE)) return FALSE;
  if( !Clock_ToggleClock(HWIO_GCC_USB30_PRIM_MASTER_CBCR_ADDR, CLK_TOGGLE_DISABLE)) return FALSE;
  if( !Clock_ToggleClock(HWIO_GCC_USB30_PRIM_SLEEP_CBCR_ADDR, CLK_TOGGLE_DISABLE)) return FALSE;
  if( !Clock_ToggleClock(HWIO_GCC_USB30_PRIM_MOCK_UTMI_CBCR_ADDR, CLK_TOGGLE_DISABLE)) return FALSE;
  if( !Clock_ToggleClock(HWIO_GCC_USB3_PRIM_PHY_AUX_CBCR_ADDR, CLK_TOGGLE_DISABLE)) return FALSE;
  if( !Clock_ToggleClock(HWIO_GCC_USB3_PRIM_PHY_COM_AUX_CBCR_ADDR, CLK_TOGGLE_DISABLE)) return FALSE;
  if( !Clock_ToggleClock(HWIO_GCC_CFG_NOC_USB3_PRIM_AXI_CBCR_ADDR, CLK_TOGGLE_DISABLE)) return FALSE;

  return TRUE;
} /* END Clock_DisableUSB */


/* ========================================================================
**  Function : Clock_UFSInit
** ======================================================================*/
/*!
    Get the requested clock frequency in hertz.

    @param
      ePerfLevel [in]  - Performance level

    @return
      True - Success init UFS clocks

    @dependencies
    None.

    @sa None
*/

boolean Clock_UFSInit( ClockBootPerfLevelType ePerfLevel )
{
  static ClockBootPerfLevelType eCurrLevel = CLOCK_BOOT_PERF_NONE;
  Clock_CBCRtoggleType enable;
  Clock_ConfigType *cfg = Clock_Config();

  /* Don't switch performance level if it has been set previously */
  if ( eCurrLevel == ePerfLevel )
  {
    return TRUE;
  }
  eCurrLevel = ePerfLevel;

  if(ePerfLevel >= CLOCK_BOOT_PERF_NUM) return FALSE;

  /* This is redundant with BootROM, except on the debugger tools, 
   * because the JTAG tools do not use BootROM */
  Clock_InitAHB2PHY();

  if ( ePerfLevel == CLOCK_BOOT_PERF_NONE )
  {
    /* Disable UFS clocks before disabling the power domain. */
    enable = CLK_TOGGLE_DISABLE;
  }
  else
  {
    /* Enable UFS clocks after enabling the power domain. */
    enable = CLK_TOGGLE_ENABLE;

    /* Enable Aggre NOC since UFS has depedency. */
    Clock_AGGRENOCInit();

    Clock_PowerDomainEnable( HWIO_GCC_UFS_PHY_GDSCR_ADDR );
  }

  Clock_ToggleClock(HWIO_GCC_AGGRE_UFS_PHY_AXI_CBCR_ADDR, enable);

  Clock_ToggleClock(HWIO_GCC_UFS_PHY_AHB_CBCR_ADDR, enable);
  Clock_ToggleClock(HWIO_GCC_UFS_PHY_AXI_CBCR_ADDR, enable);
  Clock_ToggleClock(HWIO_GCC_UFS_PHY_ICE_CORE_CBCR_ADDR, enable);
  Clock_ToggleClock(HWIO_GCC_UFS_PHY_PHY_AUX_CBCR_ADDR, enable);
  Clock_ToggleClock(HWIO_GCC_UFS_PHY_UNIPRO_CORE_CBCR_ADDR, enable);

  HWIO_OUTF(GCC_UFS_PHY_TX_SYMBOL_0_CBCR, CLK_ENABLE, enable);
  HWIO_OUTF(GCC_UFS_PHY_RX_SYMBOL_0_CBCR, CLK_ENABLE, enable) ;
  HWIO_OUTF(GCC_UFS_PHY_RX_SYMBOL_1_CBCR, CLK_ENABLE, enable) ;

  if ( ePerfLevel == CLOCK_BOOT_PERF_NONE )
  {
    Clock_PowerDomainDisable( HWIO_GCC_UFS_PHY_GDSCR_ADDR );
  }

  /* Configure UFS clock sources */
  Clock_SetClockConfig( &cfg->gcc_ufs_phy_axi_clk_src[ePerfLevel] );
  Clock_SetClockConfig( &cfg->gcc_ufs_phy_ice_core_clk_src[ePerfLevel] );
  Clock_SetClockConfig( &cfg->gcc_ufs_phy_unipro_core_clk_src[ePerfLevel] );

  return TRUE;
}

/* ========================================================================
**  Function : Clock_UFS2CardInit
** ======================================================================*/
/*!
    Get the requested clock frequency in hertz.

    @param
      ePerfLevel [in]  - Performance level

    @return
      True - Success init UFS_2 Card clocks

    @dependencies
    None.

    @sa None
*/

boolean Clock_UFS2CardInit( ClockBootPerfLevelType ePerfLevel )
{
  static ClockBootPerfLevelType eCurrLevel = CLOCK_BOOT_PERF_NONE;
  Clock_CBCRtoggleType enable;
  Clock_ConfigType *cfg = Clock_Config();

  /* Don't switch performance level if it has been set previously */
  if ( eCurrLevel == ePerfLevel )
  {
    return TRUE;
  }
  eCurrLevel = ePerfLevel;

  if(ePerfLevel >= CLOCK_BOOT_PERF_NUM) return FALSE;

  /* This is redundant with BootROM, except on the debugger tools, 
   * because the JTAG tools do not use BootROM */
  Clock_InitAHB2PHY();

  if ( ePerfLevel == CLOCK_BOOT_PERF_NONE )
  {
    /* Disable UFS clocks before disabling the power domain. */
    enable = CLK_TOGGLE_DISABLE;
  }
  else
  {
    /* Enable UFS clocks after enabling the power domain. */
    enable = CLK_TOGGLE_ENABLE;

    /* Enable Aggre NOC since UFS has depedency. */
    Clock_AGGRENOCInit();

    Clock_PowerDomainEnable(HWIO_ADDR(GCC_UFS_CARD_2_GDSCR));
  }

  Clock_ToggleClock(HWIO_ADDR(GCC_AGGRE_UFS_CARD_2_AXI_CBCR), enable);
  Clock_ToggleClock(HWIO_ADDR(GCC_UFS_CARD_2_AHB_CBCR), enable);
  Clock_ToggleClock(HWIO_ADDR(GCC_UFS_CARD_2_AXI_CBCR), enable);
  Clock_ToggleClock(HWIO_ADDR(GCC_UFS_CARD_2_ICE_CORE_CBCR), enable);
  Clock_ToggleClock(HWIO_ADDR(GCC_UFS_CARD_2_PHY_AUX_CBCR), enable);

  Clock_ToggleClock(HWIO_ADDR(GCC_UFS_CARD_2_UNIPRO_CORE_CBCR), enable);

  HWIO_OUTF(GCC_UFS_CARD_2_TX_SYMBOL_0_CBCR, CLK_ENABLE, enable);
  HWIO_OUTF(GCC_UFS_CARD_2_RX_SYMBOL_0_CBCR, CLK_ENABLE, enable) ;
  HWIO_OUTF(GCC_UFS_CARD_2_RX_SYMBOL_1_CBCR, CLK_ENABLE, enable) ;

  if ( ePerfLevel == CLOCK_BOOT_PERF_NONE )
  {
   Clock_PowerDomainDisable( HWIO_ADDR(GCC_UFS_CARD_2_GDSCR));
  }

  /* Configure UFS clock sources */
  Clock_SetClockConfig( &cfg->gcc_ufs_card_2_axi_clk_src[ePerfLevel] );
  Clock_SetClockConfig( &cfg->gcc_ufs_card_2_ice_core_clk_src[ePerfLevel] );
  Clock_SetClockConfig( &cfg->gcc_ufs_card_2_unipro_core_clk_src[ePerfLevel] );

  return TRUE;
}



/* ========================================================================
**  Function : Clock_PCIeInit
** ======================================================================*/
/*!
    Get the requested clock frequency in hertz.

    @param
      ePerfLevel [in]  - Performance level

    @return
      True - Success init UFS clocks

    @dependencies
    None.

    @sa None
*/

struct clk_pcie_regs {
  uint64 bcr_addr; 
  uint64 gdscr_addr;
  uint64 phy_refgen_cbcr_addr;
  uint64 clkref_addr;
  uint64 vote_addr;
  uint64 vote_mask;
};

const struct clk_pcie_regs pcie_registers[CLK_PCIE_NUM_CLKS] =
{
  { .bcr_addr = HWIO_ADDR(GCC_PCIE_0_BCR), 
    .gdscr_addr = HWIO_ADDR(GCC_PCIE_0_GDSCR), 
    .phy_refgen_cbcr_addr = HWIO_ADDR(GCC_PCIE0_PHY_REFGEN_CBCR),
    .clkref_addr = HWIO_ADDR(GCC_PCIE_0_CLKREF_EN),
    .vote_addr = HWIO_ADDR(GCC_APCS_CLOCK_BRANCH_ENA_VOTE_1),
    .vote_mask = ( HWIO_GCC_APCS_CLOCK_BRANCH_ENA_VOTE_1_PCIE_0_PIPE_CLK_ENA_BMSK |
                   HWIO_GCC_APCS_CLOCK_BRANCH_ENA_VOTE_1_PCIE_0_AUX_CLK_ENA_BMSK |
                   HWIO_GCC_APCS_CLOCK_BRANCH_ENA_VOTE_1_PCIE_0_CFG_AHB_CLK_ENA_BMSK |
                   HWIO_GCC_APCS_CLOCK_BRANCH_ENA_VOTE_1_PCIE_0_MSTR_AXI_CLK_ENA_BMSK |
                   HWIO_GCC_APCS_CLOCK_BRANCH_ENA_VOTE_1_PCIE_0_SLV_AXI_CLK_ENA_BMSK |
                   HWIO_GCC_APCS_CLOCK_BRANCH_ENA_VOTE_1_PCIE_0_SLV_Q2A_AXI_CLK_ENA_BMSK )},

  { .bcr_addr = HWIO_ADDR(GCC_PCIE_1_BCR),
    .gdscr_addr = HWIO_ADDR(GCC_PCIE_1_GDSCR), 
    .phy_refgen_cbcr_addr = HWIO_ADDR(GCC_PCIE1_PHY_REFGEN_CBCR),
    .clkref_addr = HWIO_ADDR(GCC_PCIE_1_CLKREF_EN),
    .vote_addr = HWIO_ADDR(GCC_APCS_CLOCK_BRANCH_ENA_VOTE),
    .vote_mask = ( HWIO_GCC_APCS_CLOCK_BRANCH_ENA_VOTE_PCIE_1_PIPE_CLK_ENA_BMSK |
                   HWIO_GCC_APCS_CLOCK_BRANCH_ENA_VOTE_PCIE_1_AUX_CLK_ENA_BMSK |
                   HWIO_GCC_APCS_CLOCK_BRANCH_ENA_VOTE_PCIE_1_CFG_AHB_CLK_ENA_BMSK |
                   HWIO_GCC_APCS_CLOCK_BRANCH_ENA_VOTE_PCIE_1_MSTR_AXI_CLK_ENA_BMSK |
                   HWIO_GCC_APCS_CLOCK_BRANCH_ENA_VOTE_PCIE_1_SLV_AXI_CLK_ENA_BMSK |
                   HWIO_GCC_APCS_CLOCK_BRANCH_ENA_VOTE_PCIE_1_SLV_Q2A_AXI_CLK_ENA_BMSK )},

  { .bcr_addr = HWIO_ADDR(GCC_PCIE_2_BCR),
    .gdscr_addr = HWIO_ADDR(GCC_PCIE_2_GDSCR), 
    .phy_refgen_cbcr_addr = HWIO_ADDR(GCC_PCIE2_PHY_REFGEN_CBCR),
    .clkref_addr = HWIO_ADDR(GCC_PCIE_2_CLKREF_EN),
    .vote_addr = HWIO_ADDR(GCC_APCS_CLOCK_BRANCH_ENA_VOTE_2),
    .vote_mask = ( HWIO_GCC_APCS_CLOCK_BRANCH_ENA_VOTE_2_PCIE_2_PIPE_CLK_ENA_BMSK |
                   HWIO_GCC_APCS_CLOCK_BRANCH_ENA_VOTE_2_PCIE_2_AUX_CLK_ENA_BMSK |
                   HWIO_GCC_APCS_CLOCK_BRANCH_ENA_VOTE_2_PCIE_2_CFG_AHB_CLK_ENA_BMSK |
                   HWIO_GCC_APCS_CLOCK_BRANCH_ENA_VOTE_2_PCIE_2_MSTR_AXI_CLK_ENA_BMSK |
                   HWIO_GCC_APCS_CLOCK_BRANCH_ENA_VOTE_2_PCIE_2_SLV_AXI_CLK_ENA_BMSK |
                   HWIO_GCC_APCS_CLOCK_BRANCH_ENA_VOTE_2_PCIE_2_SLV_Q2A_AXI_CLK_ENA_BMSK )},

  { .bcr_addr = HWIO_ADDR(GCC_PCIE_3_BCR),
    .gdscr_addr = HWIO_ADDR(GCC_PCIE_3_GDSCR), 
    .phy_refgen_cbcr_addr = HWIO_ADDR(GCC_PCIE3_PHY_REFGEN_CBCR),
    .clkref_addr = HWIO_ADDR(GCC_PCIE_3_CLKREF_EN),
    .vote_addr = HWIO_ADDR(GCC_APCS_CLOCK_BRANCH_ENA_VOTE_2),
    .vote_mask = ( HWIO_GCC_APCS_CLOCK_BRANCH_ENA_VOTE_2_PCIE_3_PIPE_CLK_ENA_BMSK |
                   HWIO_GCC_APCS_CLOCK_BRANCH_ENA_VOTE_2_PCIE_3_AUX_CLK_ENA_BMSK |
                   HWIO_GCC_APCS_CLOCK_BRANCH_ENA_VOTE_2_PCIE_3_CFG_AHB_CLK_ENA_BMSK |
                   HWIO_GCC_APCS_CLOCK_BRANCH_ENA_VOTE_2_PCIE_3_MSTR_AXI_CLK_ENA_BMSK |
                   HWIO_GCC_APCS_CLOCK_BRANCH_ENA_VOTE_2_PCIE_3_SLV_AXI_CLK_ENA_BMSK |
                   HWIO_GCC_APCS_CLOCK_BRANCH_ENA_VOTE_2_PCIE_3_SLV_Q2A_AXI_CLK_ENA_BMSK )},
};


boolean Clock_PCIeInit( ClockPCIEType eClock, ClockBootPerfLevelType ePerfLevel )
{
  static ClockBootPerfLevelType eCurrLevel = CLOCK_BOOT_PERF_NONE;
  Clock_CBCRtoggleType enable;
  Clock_ConfigType *cfg = Clock_Config();

  /* Don't switch performance level if it has been set previously */
  if ( eCurrLevel == ePerfLevel )
  {
    return TRUE;
  }
  eCurrLevel = ePerfLevel;

  if(ePerfLevel >= CLOCK_BOOT_PERF_NUM) {
    return FALSE;
  }
  
  /* This is redundant with BootROM, except on the debugger tools, 
   * because the JTAG tools do not use BootROM */
  Clock_InitAHB2PHY();

  if ( ePerfLevel == CLOCK_BOOT_PERF_NONE )
  {
    /* Disable UFS clocks before disabling the power domain. */
    enable = CLK_TOGGLE_DISABLE;
  }
  else
  {
    /* Enable UFS clocks after enabling the power domain. */
    enable = CLK_TOGGLE_ENABLE;

    /* Enable Aggre NOC since UFS has depedency. */
    Clock_AGGRENOCInit();
    Clock_PowerDomainEnable( pcie_registers[eClock].gdscr_addr );
  }


  // reset PCIE_1 block
  outpdw(pcie_registers[eClock].bcr_addr, 1);
  busywait(500);
  outpdw(pcie_registers[eClock].bcr_addr, 0);
  
  /* Vote on all the PCIE clocks */
  outpdw(pcie_registers[eClock].vote_addr, inpdw(pcie_registers[eClock].vote_addr) | pcie_registers[eClock].vote_mask );

  /* Turn on the clocks */
  // Clock_ToggleClock(HWIO_GCC_PCIE_1_CLKREF_EN_ADDR, 1);
  outpdw(pcie_registers[eClock].clkref_addr, enable);
  // Clock_ToggleClock(HWIO_GCC_PCIE1_PHY_REFGEN_CBCR_ADDR, 1);
  Clock_ToggleClock(pcie_registers[eClock].phy_refgen_cbcr_addr, enable);

  /* Switch PCIE refgens to 100 MHz */
  Clock_ToggleClock(HWIO_GCC_PCIE_PHY_AUX_CBCR_ADDR, enable); 
  Clock_SetClockConfig( &cfg->gcc_pcie_phy_refgen_clk_src[ePerfLevel] );

  if ( ePerfLevel == CLOCK_BOOT_PERF_NONE )
  {
    Clock_PowerDomainDisable( HWIO_GCC_PCIE_1_GDSCR_ADDR );
  }

  return TRUE;
}


/* ============================================================================
**  Function : Clock_InitVSense
** ============================================================================
*/
/*!
    This function enables the Voltage Sensor clocks, and configure them run
    at max frequency level at NOMINAL voltage.

    @return
      TRUE -- Initialization was successful.
      FALSE -- Initialization failed.

   @dependencies
     None.

*/
boolean Clock_InitVSense( void )
{
  return TRUE;
}


/* ============================================================================
**  Function : Clock_ShutdownVSense
** ============================================================================
*/
/*!
    This function disables the voltage sensor clocks.

    @return
      TRUE -- Initialization was successful.
      FALSE -- Initialization failed.

   @dependencies
      None.

*/
boolean Clock_ShutdownVSense( void )
{
  return TRUE;
}

/* ============================================================================
**  Function : Clock_SetQSPIFrequency
** ============================================================================
*/
/*!
    Set QSPI clock frequency.  Clocks will turn off if request frequency is zero

    @param nFreqKHz - Clock frequency in KHz

    @return - Frequency that clock has been match and set to.

    @dependencies
    None.

    @sa None
*/
uint32 Clock_SetQSPIFrequency( uint32 nFreqKHz )
{
  const Clock_ConfigType *cfg = Clock_Config();
  const ClockFreqMuxCfgType *pCfg = NULL;
  uint32 n;

  /* Turn clock off */
  if ( nFreqKHz == 0 )
  {
    Clock_ToggleClock(HWIO_GCC_QSPI_CNOC_PERIPH_AHB_CBCR_ADDR, CLK_TOGGLE_DISABLE);
    Clock_ToggleClock(HWIO_GCC_QSPI_CORE_CBCR_ADDR, CLK_TOGGLE_DISABLE);
    return 0;
  }

  Clock_ToggleClock(HWIO_GCC_AGGRE_NOC_TBU1_CBCR_ADDR, CLK_TOGGLE_ENABLE);
  Clock_ToggleClock(HWIO_GCC_AGGRE_NOC_TBU2_CBCR_ADDR, CLK_TOGGLE_ENABLE);

  pCfg = cfg->gcc_qspi_core_clk_src;

  /* Find the nearest frequency that matches the request */
  for(n = 0; pCfg[n].nFreqKHz != 0; n++)
  {
    if(pCfg[n].nFreqKHz >= nFreqKHz) break;
  }
  if(pCfg[n].nFreqKHz == 0) return 0;

  Clock_SetClockConfig( &pCfg[n].Cfg ); // Will spin if it fails 

  Clock_ToggleClock(HWIO_GCC_QSPI_CNOC_PERIPH_AHB_CBCR_ADDR, CLK_TOGGLE_ENABLE);
  Clock_ToggleClock(HWIO_GCC_QSPI_CORE_CBCR_ADDR, CLK_TOGGLE_ENABLE);

  return pCfg[n].nFreqKHz;
}
