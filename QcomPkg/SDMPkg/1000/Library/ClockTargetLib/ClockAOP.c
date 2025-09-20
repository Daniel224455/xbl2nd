/**
==============================================================================
  @file ClockAOP.c

  Clock functions for enable and share data to AOP.

  Copyright (c) 2017 QUALCOMM Technologies, Inc.  All Rights Reserved.  
  QUALCOMM Proprietary and Confidential.

==============================================================================


==============================================================================

                            EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.


  when      who     what, where, why
  --------  ------  ------------------------------------------------------
  10/20/17  vphan   Integrated from SDM845 for support SDM1000
  02/23/17  vphan   Initial SDM845 revision.

==============================================================================
*/


/*==========================================================================

                               INCLUDE FILES

===========================================================================*/
#include "ClockHWIO.h"
#include "ClockBSP.h"
#include "ClockDriver.h"
#include "boot_util.h"
#include "smem.h"
#include "ddr_drivers.h"
#include "busywait.h"
#include "CoreMsgRam.h"

/*=========================================================================
                       MACRO DEFINITIONS
==========================================================================*/
#define CLOCK_SMEM_VERSION 2

/* Updated to HSRv7.0 */
#define PLUTO_CONFIG_CTL_VAL    0x60008F64
#define PLUTO_CONFIG_CTL_U_VAL  0x0001C808
#define PLUTO_CONFIG_CTL_U1_VAL 0x00000000
#define PLUTO_CONFIG_CTL_U2_VAL 0x04000780

#define PLUTO_TEST_CTL_VAL      0x0088C496
#define PLUTO_TEST_CTL_U_VAL    0x90000000
#define PLUTO_TEST_CTL_U1_VAL   0x44200030
#define PLUTO_TEST_CTL_U2_VAL   0x00000398

#define PLUTO_USER_CTL_VAL      0x00000101
#define PLUTO_USER_CTL_U_VAL    0x00250002

/*
 * Max PLUTO lock time. Max lock time with calibration is expected to be 275us.
 */
#define PLUTO_PLL_LOCK_TIMEOUT_US 600

/*=========================================================================
      Data
==========================================================================*/
uint64 ClockAOPList[] =
{
  HWIO_ADDR(AOSS_CC_AOP_RO_CBCR),
  HWIO_ADDR(AOSS_CC_PWR_MUX_CTRL_CBCR),
  HWIO_ADDR(AOSS_CC_WCSS_TS_CBCR),
  //HWIO_ADDR(AOSS_CC_TSENS_HW_CBCR), // enable tied high
  //HWIO_ADDR(AOSS_CC_SWAO_CBCR),
  //HWIO_ADDR(AOSS_CC_SWAO_AO_CBCR),
  //HWIO_ADDR(AOSS_CC_SWAO_MEM_CBCR),
  //HWIO_ADDR(AOSS_CC_SWAO_TS_CBCR),
  //HWIO_ADDR(AOSS_CC_SWAO_TS_AO_CBCR),
  //HWIO_ADDR(AOSS_CC_SWAO_BUS_CBCR),
  0,
};


typedef struct
{
  uint16  nVersion;         /* A version number to verify matching XBL and RPM data structures */
  uint16  nBootCPOffset;    /* Offset to boot CP table address */
} Clock_SMEMInfoType;

/*
 * PLLs configuration data
 */
extern Clock_ConfigType Clock_ConfigData;

/*
 * Clock configure performance levels for inform AOP during initialize
 */
extern ClockCfgCPType Clock_PerfData;

static ddr_clock_plan MCPlan;


/*=========================================================================
      Function Prototypes
==========================================================================*/
boolean Clock_ConfigAOP( void );

/*===========================================================================
                      FUNCTION DECLARATIONS
===========================================================================*/

/* =========================================================================
**  Function : Clock_EnableAOPPLL1
** =========================================================================*/
/**
  Enable AOP_PLL1.
*/

static boolean Clock_EnableAOPPLL1
(
  void
)
{
  uint32 nTimeout = 0;

  HWIO_OUT( AOSS_CC_PLL1_L_VAL, 0xA );
  HWIO_OUT( AOSS_CC_PLL1_USER_CTL,     PLUTO_USER_CTL_VAL );
  HWIO_OUT( AOSS_CC_PLL1_USER_CTL_U,   PLUTO_USER_CTL_U_VAL );

  HWIO_OUT( AOSS_CC_PLL1_CONFIG_CTL,    PLUTO_CONFIG_CTL_VAL );
  HWIO_OUT( AOSS_CC_PLL1_CONFIG_CTL_U,  PLUTO_CONFIG_CTL_U_VAL );
  HWIO_OUT( AOSS_CC_PLL1_CONFIG_CTL_U1, PLUTO_CONFIG_CTL_U1_VAL );
  HWIO_OUT( AOSS_CC_PLL1_CONFIG_CTL_U2, PLUTO_CONFIG_CTL_U2_VAL );
  
  HWIO_OUT( AOSS_CC_PLL1_TEST_CTL,      PLUTO_TEST_CTL_VAL );
  HWIO_OUT( AOSS_CC_PLL1_TEST_CTL_U,    PLUTO_TEST_CTL_U_VAL );
  HWIO_OUT( AOSS_CC_PLL1_TEST_CTL_U1,   PLUTO_TEST_CTL_U1_VAL );
  HWIO_OUT( AOSS_CC_PLL1_TEST_CTL_U2,   PLUTO_TEST_CTL_U2_VAL );

  /* Select internal DCO (internally generated 38.4MHz) for output. */
  HWIO_OUTF(AOSS_CC_PLL1_USER_CTL_U, INTERNAL_CLOCK_SELECTION, 2);

  /* Enable the PLL. */
  HWIO_OUTF(AOSS_CC_PLL1_OPMODE, PLL_OPMODE, 1);
  HWIO_OUTF(AOSS_CC_PLL1_MODE, PLL_RESET_N, 1);
  HWIO_OUTF(AOSS_CC_PLL1_MODE, PLL_CAL_XO_PRESENT, 1);

  /* Wait for lock, after which the PLL will be calibrated. */
  while (HWIO_INF(AOSS_CC_PLL1_MODE, PLL_LOCK_DET) == 0)
  {
    if (nTimeout++ >= PLUTO_PLL_LOCK_TIMEOUT_US)
    {
      return FALSE;
    }

    busywait(1);
  }

  /* Enable main */
  HWIO_OUTF(AOSS_CC_PLL1_USER_CTL, PLLOUT_MAIN, 1);

  /* Switch to the internally generated clock and enable the outputs. */
  HWIO_OUTF(AOSS_CC_PLL1_MODE, PLL_OUTCTRL, 1);
  HWIO_OUTF(AOSS_CC_PLL1_MODE, PLL_CLOCK_SELECT, 1);

  return TRUE;

} /* END Clock_EnableAOPPLL1 */


/* ============================================================================
**  Function : Clock_ConfigDDRModeForAOP
** ============================================================================
*/
/*!
    This function ensures DDR clocks are left in a state valid for the first
    HW switch. Transitioning from SW to HW mode only works in specific cases,
    which are documented in this table provided by Chaitanya Emmela...

    SW         |  HW      |
    --------------------------------------------
    DDRCC      |  DDRCC   |   OK
    DDRCC      |  GCC     |   OK
    GCC (RCG0) |  GCC     |   NOT OK
    GCC (RCG1) |  GCC     |   OK
    GCC        |  DDRCC   |   NOT OK

  @param
    None
    
  @return
    None
   
  @dependencies
    None.

*/
boolean Clock_ConfigDDRModeForAOP( void )
{
  uint32 nIndex;

  /* DDR frequency scaling currently not supported on RUMI. */
  if (RUMI_CHECK() == TRUE)
  {
    return TRUE;
  }

  /*
   * If we're already in DDRCC mode, then we're already safe.
   */
  if ( MCPlan.state[Clock_PerfData.nDDR].mode != DDRCC )
  {
    /*
     * If we're not in DDRCC, but there's a valid DDRCC level enabled in the
     * plan, then switch to the lowest DDRCC level. This is required because
     * the first HW switch cannot be from GCC -> DDRCC.
     */
    for ( nIndex = 0; nIndex < MCPlan.num_levels; nIndex++ )
    {
      if ( (MCPlan.state[nIndex].freq_enable==TRUE) && (MCPlan.state[nIndex].mode==DDRCC) )
      {
        return Clock_SetBIMCSpeed( MCPlan.state[nIndex].freq_khz );
      }
    }

    /*
     * If there are no DDRCC levels enabled in the plan, then make sure to
     * leave on GCC RCG1, since the first HW switch cannot be from RCG0.
     */
    if( HWIO_INF(GCC_DDRMC_CH0_CBCR, CLK_ENABLE) == 1 )
    {
      return Clock_SetBIMCSpeed( MCPlan.state[Clock_PerfData.nDDR].freq_khz );
    }
  }

  return TRUE;
}


/* ============================================================================
**  Function : Clock_ShareDataToAOP
** ============================================================================
*/
/*!
    This function is used for saving clock configure performance levels into SMEM
  for sharing with AOP.  AOP needs the know current performance level settings in
  XBL, so it can put the same initial vote during AOP initialize.
  
  @param
    None
    
  @return
    None
   
  @dependencies
    None.

*/
boolean Clock_ShareDataToAOP( void )
{
  Clock_SMEMInfoType* pSMEMInfo;
  uint32 nTip, nSize;
  uint32 smem_size = sizeof(Clock_SMEMInfoType) + sizeof(ClockCfgCPType);
  void *pSMEM = smem_alloc(SMEM_CLOCK_INFO, smem_size);

  if(pSMEM == NULL) return FALSE;

  /* Store smem address in msg_ram since there is no smem driver in AOP */
  msg_ram_set_smem_address((uint32)SMEM_CLOCK_INFO, (uint32)pSMEM);

  /* Store SMEMInfo */
  pSMEMInfo = pSMEM;
  pSMEMInfo->nVersion = CLOCK_SMEM_VERSION;
  pSMEMInfo->nBootCPOffset = sizeof(Clock_SMEMInfoType);

  /* Store BootCfgCP info */
  nTip = sizeof(Clock_SMEMInfoType);
  nSize = sizeof(ClockCfgCPType);
  qmemscpy( (pSMEM+nTip), nSize, &Clock_PerfData, nSize);
  nTip += nSize;

  if ( nTip == smem_size )
    return TRUE;
  else
    return FALSE;
}


/* ============================================================================
**  Function : Clock_ConfigAOP
** ============================================================================
*/
/*!
    This function is used for config all necessary clocks, PLLs and saving 
  clock performance settings and DDR info into shared memory (SMEM) for sharing 
  with AOP.
  
  @param
    None
    
  @return
    None
   
  @dependencies
    None.

*/
boolean Clock_ConfigAOP( void )
{
  ClockConfigMuxType clkCfg =     {HWIO_ADDR(AOSS_CC_RO_CMD_RCGR),     0, MUX_AOSS, SRC_AOPLL1,       0, 0, 0, 0};  // 19.2MHz
  ClockConfigMuxType clkAOPCfg =  {HWIO_ADDR(AOSS_CC_AOP_CMD_RCGR),    0, MUX_AOSS, SRC_AOPLL0_EVEN,  0, 0, 0, 0};  // 200MHz
  ClockConfigMuxType clkSWAOCfg = {HWIO_ADDR(AOSS_CC_SWAO_CMD_RCGR),   0, MUX_AOSS, SRC_AOPLL0_EVEN,  4, 0, 0, 0};  // 100MHz
  ClockConfigMuxType clkEUDCfg =  {HWIO_ADDR(AOSS_CC_EUD_AT_CMD_RCGR), 0, MUX_AOSS, SRC_AOPLL0_EVEN, 10, 0, 0, 0};  // 40MHz
  uint32 nVer = Clock_ChipVersion();

  /* Enable AOP PLL1 */
  if ( Clock_EnableAOPPLL1() == FALSE ) return FALSE;
  /* Select AOP PLL1 as the source for AOP_PLL0 */
  HWIO_OUTF( AOSS_CC_PLL_CLK_SEL, PLL0_CLK_REF_SEL, 1);

  while ( !Clock_EnableSource(SRC_AOPLL0_EVEN) );
  Clock_ConfigMux(&clkAOPCfg);
  Clock_ConfigMux(&clkSWAOCfg);
  Clock_ConfigMux(&clkEUDCfg);


  /* Configure all PLLs before handle to AOP */
  if ( !Clock_ConfigurePLL(&Clock_ConfigData.GPLL0_Cfg) ) return FALSE;
  if ( !Clock_ConfigurePLL(&Clock_ConfigData.GPLL1_Cfg) ) return FALSE;
  if ( !Clock_ConfigurePLL(&Clock_ConfigData.GPLL2_Cfg) ) return FALSE;
  if ( !Clock_ConfigurePLL(&Clock_ConfigData.GPLL3_Cfg) ) return FALSE;
  if ( !Clock_ConfigurePLL(&Clock_ConfigData.GPLL4_Cfg) ) return FALSE;
  if ( !Clock_ConfigurePLL(&Clock_ConfigData.GPLL5_Cfg) ) return FALSE;
  if ( !Clock_ConfigurePLL(&Clock_ConfigData.GPLL6_Cfg) ) return FALSE;
  if ( !Clock_ConfigurePLL(&Clock_ConfigData.GPLL7_Cfg) ) return FALSE;
  if ( !Clock_ConfigurePLL(&Clock_ConfigData.GPLL8_Cfg) ) return FALSE;
  if ( !Clock_ConfigurePLL(&Clock_ConfigData.GPLL9_Cfg) ) return FALSE;
  if ( !Clock_ConfigurePLL(&Clock_ConfigData.GPLL10_Cfg[nVer]) ) return FALSE;

  /* Switch all AOP XO source clocks to AOP PLL1 */ 
  Clock_ConfigMux(&clkCfg);

  clkCfg.nCMDRCGRAddr = HWIO_ADDR(AOSS_CC_SPMI_AOD_SER_CMD_RCGR);
  Clock_ConfigMux(&clkCfg);

  clkCfg.nCMDRCGRAddr = HWIO_ADDR(AOSS_CC_SWAO_TS_CMD_RCGR);
  Clock_ConfigMux(&clkCfg);

  // Enabling SWAO before QDSS is unnecessary, and a problem for 
  // AOP deep sleep.
  // Clock_PowerDomainEnable( HWIO_AOSS_CC_SWAO_GDSCR_ADDR );

  Clock_ToggleEnableList( ClockAOPList );

  /* Configure DDR mode before handle to AOP */
  MCPlan = ddr_clock_query(MC_CLOCK_PLAN);
  if ( !Clock_ConfigDDRModeForAOP() ) return FALSE;

  /* Store config data to SMEM for sharing with AOP */
  return Clock_ShareDataToAOP();

}
