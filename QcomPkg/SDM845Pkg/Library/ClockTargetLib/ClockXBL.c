/**
==============================================================================
  @file ClockXBL.c

  Clock functions for the XBL Loader Clock Driver.

  Copyright (c) 2016-2017 QUALCOMM Technologies, Inc.  All Rights Reserved.  
  QUALCOMM Proprietary and Confidential.

==============================================================================


==============================================================================

                            EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/boot.xf/2.2/QcomPkg/SDM845Pkg/Library/ClockTargetLib/ClockXBL.c#4 $

  when      who     what, where, why
  --------  ------  ------------------------------------------------------
  02/08/17  vphan   Fixed SOC PHY reset is always high by default
  11/11/16  vphan   Initial SDM845 revision, branched from MSM8998.

==============================================================================
*/


/*==========================================================================

                               INCLUDE FILES

===========================================================================*/
#include "ClockHWIO.h"
#include "ClockBoot.h"
#include "ClockDriver.h"
#include "ClockBSP.h"
#include "railway.h"

#include "ClockNOC_SDM845v2.h"
/* "Borrowed" from ClockNOC_SDM845v1.h header for V1, this is the only difference */
/* To prevent redefinition, I added "_V1_" to the front */
#define HWIO_V1_AGGRE_NOC_A1NOC_DCD_MAXDIV_LOW_ADDR   (AGGRE_NOC_A1NOC_SERVICE_NETWORK_REG_BASE      + 0x00012008)

/*=========================================================================
                       MACRO DEFINITIONS
==========================================================================*/


/*=========================================================================
      Data
==========================================================================*/
/* Clock performance CP for sharing with AOP */
ClockCfgCPType Clock_PerfData;

/* This list is every clock that defaults OFF that is owned by RPMh or Boot */
uint64 ClockEnableList[] =
{
  HWIO_ADDR(GCC_AGGRE_NOC_AUDIO_TBU_CBCR),
  HWIO_ADDR(GCC_AGGRE_NOC_IPA_CBCR),
  HWIO_ADDR(GCC_AGGRE_NOC_QOSGEN_EXTREF_CBCR),
  HWIO_ADDR(GCC_AGGRE_NOC_TBU1_CBCR),
  HWIO_ADDR(GCC_AGGRE_NOC_TBU2_CBCR),
  //HWIO_ADDR(GCC_AHB2PHY_SOUTH_CBCR),
  //HWIO_ADDR(GCC_CFG_NOC_AH2PHY_XO_CBCR),
  //HWIO_ADDR(GCC_CM_PHY_REFGEN1_CBCR),
  //HWIO_ADDR(GCC_CM_PHY_REFGEN2_CBCR),
  //HWIO_ADDR(GCC_CNOC_PERIPH_EAST_CBCR),
  //HWIO_ADDR(GCC_CNOC_PERIPH_NORTH_CBCR),
  //HWIO_ADDR(GCC_CNOC_PERIPH_SOUTH_CBCR),
  HWIO_ADDR(GCC_DCC_AHB_CBCR),
  HWIO_ADDR(GCC_DDR_I_HCLK_CBCR),
  //HWIO_ADDR(GCC_DDRMC_CH0_CBCR),  /* These are specially and carefully */
  //HWIO_ADDR(GCC_DDRMC_CH1_CBCR),  /* controlled in DDR clock switch (ClockBIMC.c) */
  HWIO_ADDR(GCC_DDRSS_MMNOC_HF_QX_CBCR),
  HWIO_ADDR(GCC_DDRSS_MMNOC_SF_QX_CBCR),
  HWIO_ADDR(GCC_DDRSS_SLEEP_CBCR),
  HWIO_ADDR(GCC_DDRSS_SYS_NOC_GC_AXI_CBCR),
  HWIO_ADDR(GCC_DDRSS_TCU_CBCR),
  HWIO_ADDR(GCC_DDRSS_XO_CBCR),
  //HWIO_ADDR(GCC_DNOC_CFG_CBCR),
  HWIO_ADDR(GCC_IPA_2X_CBCR),
  HWIO_ADDR(GCC_IPA_AHB_CBCR),
  HWIO_ADDR(GCC_IPA_CBCR),
  HWIO_ADDR(GCC_IPA_SLEEP_CBCR),
  HWIO_ADDR(GCC_MMNOC_AHB_CFG_CBCR),
  HWIO_ADDR(GCC_MMNOC_HF_QX_CBCR),
  HWIO_ADDR(GCC_MMNOC_QOSGEN_EXTREF_CBCR),
  HWIO_ADDR(GCC_MMNOC_SF_QX_CBCR),
  HWIO_ADDR(GCC_MMNOC_TBU_HF0_CBCR),
  HWIO_ADDR(GCC_MMNOC_TBU_HF1_CBCR),
  HWIO_ADDR(GCC_MMNOC_TBU_SF_CBCR),
  HWIO_ADDR(GCC_MMSS_QM_AHB_CBCR),
  HWIO_ADDR(GCC_MMSS_QM_CORE_CBCR),
  HWIO_ADDR(GCC_MMU_TCU_CBCR),
  HWIO_ADDR(GCC_NOC_CENTER_DCD_XO_CBCR),
  HWIO_ADDR(GCC_NOC_EAST_DCD_XO_CBCR),
  HWIO_ADDR(GCC_NOC_MMNOC_DCD_XO_CBCR),
  HWIO_ADDR(GCC_NOC_MONAQ_DCD_XO_CBCR),
  HWIO_ADDR(GCC_NOC_NORTH_DCD_XO_CBCR),
  HWIO_ADDR(GCC_NOC_SOUTH_DCD_XO_CBCR),
  HWIO_ADDR(GCC_NOC_WEST_DCD_XO_CBCR),
  //HWIO_ADDR(GCC_PIMEM_AHB_CBCR),
  //HWIO_ADDR(GCC_PIMEM_AXI_CBCR),
  HWIO_ADDR(GCC_QREFS_VBG_CAL_CBCR),
  HWIO_ADDR(GCC_RBCPR_CX_AHB_CBCR),
  HWIO_ADDR(GCC_RBCPR_CX_CBCR),
  HWIO_ADDR(GCC_RBCPR_MX_AHB_CBCR),
  HWIO_ADDR(GCC_RBCPR_MX_CBCR),
  HWIO_ADDR(GCC_SHRM_CFG_AHB_CBCR),
  HWIO_ADDR(GCC_SHRM_CBCR),
  //HWIO_ADDR(GCC_SLEEP_CBCR),
  //HWIO_ADDR(GCC_SNOC_QOSGEN_EXTREF_CBCR),
  //HWIO_ADDR(GCC_SPDM_CFG_AHB_CBCR),
  HWIO_ADDR(GCC_SPDM_DEBUG_CY_CBCR),
  HWIO_ADDR(GCC_SPDM_FF_CBCR),
  //HWIO_ADDR(GCC_SPDM_MEMNOC_CY_CBCR),
  //HWIO_ADDR(GCC_SPDM_MSTR_AHB_CBCR),
  //HWIO_ADDR(GCC_SPDM_PNOC_CY_CBCR),
  //HWIO_ADDR(GCC_SPDM_SNOC_CY_CBCR),
  HWIO_ADDR(GCC_SYS_NOC_GC_AXI_CBCR),
  //HWIO_ADDR(GCC_SYS_NOC_PIMEM_AXI_CBCR),
  //HWIO_ADDR(GCC_XO_DIV4_CBCR),
};


/* DCD control variables. */
boolean Boot_Clock_DCD = TRUE;

/* List of registers to initialize for DCD setup. */
struct Clock_DCD_rinit
{
  uint64 address;
  uint32 value;
} Clock_DCD_Maxdiv[] =
{
  // DCD MAXDIV registers
  // V1/V2 CONFLICT!{ HWIO_AGGRE_NOC_A1NOC_DCD_MAXDIV_LOW_ADDR, 0x4 },
  { HWIO_AGGRE_NOC_A2NOC_DCD_EAST_MAXDIV_LOW_ADDR, 0x4 },
  { HWIO_AGGRE_NOC_A2NOC_DCD_MONAQ_MAXDIV_LOW_ADDR, 0x4 },
  { HWIO_AGGRE_NOC_A2NOC_DCD_NORTH_MAXDIV_LOW_ADDR, 0x4 },
  { HWIO_AGGRE_NOC_A2NOC_DCD_SF_LPASS_MAXDIV_LOW_ADDR, 0x4 },
  { HWIO_AGGRE_NOC_A2NOC_DCD_SF_PCIE3_MAXDIV_LOW_ADDR, 0x4 },
  { HWIO_AGGRE_NOC_A2NOC_DCD_WEST_MAXDIV_LOW_ADDR, 0x4 },
  { HWIO_CNOC_DCD_CENTER_QHCLK_MAXDIV_LOW_ADDR, 0x1 },
  { HWIO_CNOC_DCD_EAST_QHCLK_MAXDIV_LOW_ADDR, 0x4 },
  //{ HWIO_CNOC_DCD_MMNOC_QHCLK_MAXDIV_LOW_ADDR, 0x4 }, // writes to this cause XBL Loader to fail ?permissions?
  { HWIO_CNOC_DCD_MONAQ_QHCLK_MAXDIV_LOW_ADDR, 0x4 },
  { HWIO_CNOC_DCD_NORTH_QHCLK_MAXDIV_LOW_ADDR, 0x4 },
  { HWIO_CNOC_DCD_SOUTH_QHCLK_MAXDIV_LOW_ADDR, 0x4 },
  { HWIO_CNOC_DCD_WEST_QHCLK_MAXDIV_LOW_ADDR, 0x4 },
  { HWIO_MNOC_DCD_HF_QXCLK_MAXDIV_LOW_ADDR, 0x4 },
  { HWIO_MNOC_DCD_SF_QXCLK_MAXDIV_LOW_ADDR, 0x4 },
  { HWIO_SNOC_DCD_EAST_QX_CLK_MAXDIV_LOW_ADDR, 0x4 },
  { HWIO_SNOC_DCD_HS_QX_CLK_MAXDIV_LOW_ADDR, 0x4 },
  { HWIO_SNOC_DCD_MONAQ_QX_CLK_MAXDIV_LOW_ADDR, 0x4 },
  { HWIO_SNOC_DCD_NORTH_QX_CLK_MAXDIV_LOW_ADDR, 0x4 },
  { HWIO_SNOC_DCD_PIMEM_CLK_MAXDIV_LOW_ADDR, 0x4 },
  { HWIO_SNOC_DCD_QX_CLK_MAXDIV_LOW_ADDR, 0x4 },
  { HWIO_SNOC_DCD_SOUTH_QX_CLK_MAXDIV_LOW_ADDR, 0x4 },

  // DCD enable registers
  { HWIO_GCC_AGGRE_NOC_EAST_DCD_CDIV_DCDR_ADDR, 0x1 },
  { HWIO_GCC_AGGRE_NOC_EAST_SF_DCD_DCDR_ADDR, 0x1 },
  { HWIO_GCC_AGGRE_NOC_MONAQ_DCD_CDIV_DCDR_ADDR, 0x1 },
  { HWIO_GCC_AGGRE_NOC_NORTH_DCD_CDIV_DCDR_ADDR, 0x1 },
  { HWIO_GCC_AGGRE_NOC_NORTH_SF_DCD_DCDR_ADDR, 0x1 },
  { HWIO_GCC_AGGRE_NOC_SOUTH_DCD_CDIV_DCDR_ADDR, 0x1 },
  { HWIO_GCC_AGGRE_NOC_WEST_DCD_CDIV_DCDR_ADDR, 0x1 },
  { HWIO_GCC_CONFIG_NOC_CENTER_DCD_CDIV_DCDR_ADDR, 0x1 },
  { HWIO_GCC_CONFIG_NOC_EAST_DCD_CDIV_DCDR_ADDR, 0x1 },
  //{ HWIO_GCC_CONFIG_NOC_MMNOC_DCD_CDIV_DCDR_ADDR, 0x1 },
  { HWIO_GCC_CONFIG_NOC_MONAQ_DCD_CDIV_DCDR_ADDR, 0x1 },
  { HWIO_GCC_CONFIG_NOC_NORTH_DCD_CDIV_DCDR_ADDR, 0x1 },
  { HWIO_GCC_CONFIG_NOC_SOUTH_DCD_CDIV_DCDR_ADDR, 0x1 },
  { HWIO_GCC_CONFIG_NOC_WEST_DCD_CDIV_DCDR_ADDR, 0x1 },
  { HWIO_GCC_CPUSS_AXI_DCD_CDIV_DCDR_ADDR, 0x1 },
  { HWIO_GCC_GPU_MEMNOC_GFX_DCD_CDIV_DCDR_ADDR, 0x1 },
  { HWIO_GCC_MEMNOC_DCD_CDIV_DCDR_ADDR, 0x1 },
  // { HWIO_GCC_MMNOC_HF_QX_DCD_CDIV_DCDR_ADDR, 0x1 }, Removed to address camera underruns
  // { HWIO_GCC_MMNOC_SF_QX_DCD_CDIV_DCDR_ADDR, 0x1 }, 
  // { HWIO_GCC_MMU_TCU_DCD_CDIV_DCDR_ADDR, 0x1 },  QCTDD04357971:Turing external access hangs after CX collapse with DCD enabled
  { HWIO_GCC_MSS_Q6_MEMNOC_AXI_DCD_CDIV_DCDR_ADDR, 0x1 },
  { HWIO_GCC_SYS_NOC_DCD_CDIV_DCDR_ADDR, 0x1 },
  { HWIO_GCC_SYS_NOC_EAST_DCD_CDIV_DCDR_ADDR, 0x1 },
  { HWIO_GCC_SYS_NOC_GC_DCD_CDIV_DCDR_ADDR, 0x1 },
  { HWIO_GCC_SYS_NOC_MONAQ_DCD_CDIV_DCDR_ADDR, 0x1 },
  { HWIO_GCC_SYS_NOC_NORTH_DCD_CDIV_DCDR_ADDR, 0x1 },
  { HWIO_GCC_SYS_NOC_SF_DCD_CDIV_DCDR_ADDR, 0x1 },
  { HWIO_GCC_SYS_NOC_SOUTH_DCD_CDIV_DCDR_ADDR, 0x1 },
  { HWIO_GCC_TURING_AXI_DCD_CDIV_DCDR_ADDR, 0x1 },
  { HWIO_VIDEO_CC_VENUS_DIV_DCDR_ADDR, 0x1 },
};
 
  
/* Enable Hardware Control of MEMNOC related clocks */
boolean Boot_Clock_MEMNOC_DCG = TRUE;

uint64 Boot_Clock_MEMNOC_CBCRs[] = 
{
  // for V2 and V2.0.1
  HWIO_ADDR(GCC_DDRSS_MSS_Q6_AXI_CBCR),
  HWIO_ADDR(GCC_MSS_Q6_MEMNOC_AXI_CBCR),
  HWIO_ADDR(GCC_DDRSS_GPU_AXI_CBCR),
  HWIO_ADDR(GCC_GPU_MEMNOC_GFX_CBCR),
  HWIO_ADDR(GCC_MEMNOC_CBCR),
  HWIO_ADDR(GCC_SHRM_CBCR),
  HWIO_ADDR(GCC_DDRMC_CH0_CBCR),
  HWIO_ADDR(GCC_DDRMC_CH1_CBCR),
  HWIO_ADDR(GCC_TURING_AXI_CBCR),
  HWIO_ADDR(GCC_DDRSS_TURING_AXI_CBCR),
};

/*=========================================================================
      Function Prototypes
==========================================================================*/

boolean Clock_InitRBCPR(void);
boolean Clock_InitCrypto(ClockBootPerfLevelType eSysPerfLevel);
boolean Clock_InitIPA(ClockBootPerfLevelType eSysPerfLevel);
boolean Clock_InitPLLStatic(void);
extern boolean Clock_AGGRENOCInit(void);
extern void Clock_ReplaceUSBBootClockEnable(void);
extern void Clock_ReplaceUFSBootClockEnable(void);
extern void Clock_StoreDDRFreqKHz ( uint32 nFreqKHz );
extern boolean Clock_CopyBIMCPlanToRPM( void );
extern uint32 Clock_GetAPSSCL0SpeedKHz(void);
extern boolean Clock_ConfigAOP( void );
extern void msg_ram_init_base(uint32 *msg_ram_addr);
static void Clock_EnableDynClkDivide (void);

/*===========================================================================
                      FUNCTION DECLARATIONS
===========================================================================*/

/* ============================================================================
**  Function : Clock_DebugInit
** ============================================================================
*/
/*!

    This function is called very early in boot.  It is used for work-arounds that
    need to be done before JTAG attaches at the XBL "hold focus button" spin loop.

    @return
    TRUE -- Clean up was successful.
    FALSE -- Clean up failed.

   @dependencies
    You cannot use busywait in this function.

*/
boolean Clock_DebugInit(void)
{  
  uint32 i;

  HWIO_OUTF(GCC_GPLL0_USER_CTL, PLLOUT_EVEN, 1);

  /* To improve boot time, speed up the CPU and buses */
  if( ! Clock_SetSysPerfLevel(CLOCK_BOOT_PERF_DEFAULT)) return FALSE;
  if( ! Clock_SetCPUPerfLevel(CLOCK_BOOT_PERF_DEFAULT)) return FALSE;

  /* Enable the RPMh and Boot required clocks */
  for(i=0; i < CLK_NUM_ELEM( ClockEnableList); i++)
  {
    uint64 addr = ClockEnableList[i];
    outpdw(addr, inpdw(addr) | 1 ); // Sets CLK_EN in the *_CBCR register
  }

  /* Enable SPMI clocks */
  HWIO_OUTF(AOSS_CC_SPMI_CFG_AHB_CBCR, CLK_ENABLE, 1);
  HWIO_OUTF(AOSS_CC_SPMI_SER_CBCR, CLK_ENABLE, 1);
  HWIO_OUTF(AOSS_CC_SPMI_AOD_SER_CBCR, CLK_ENABLE, 1);
  
  /* HW Errata - Reset UNI PHY always high by default */
  HWIO_OUTF( GCC_USB3_PHY_SEC_BCR, BLK_ARES, 0);

  /* Enable PIMEM clocks */
  HWIO_OUTF(GCC_PIMEM_AHB_CBCR, CLK_ENABLE, 1);
  HWIO_OUTF(GCC_SYS_NOC_PIMEM_AXI_CBCR, CLK_ENABLE, 1);
  HWIO_OUTF(GCC_PIMEM_AXI_CBCR, CLK_ENABLE, 1);

  /* Enable SPDM clocks */
  HWIO_OUTF(GCC_SPDM_CFG_AHB_CBCR, CLK_ENABLE, 1);
  HWIO_OUTF(GCC_SPDM_MSTR_AHB_CBCR, CLK_ENABLE, 1);
  HWIO_OUTF(GCC_SPDM_PNOC_CY_CBCR, CLK_ENABLE, 1);
  HWIO_OUTF(GCC_SPDM_SNOC_CY_CBCR, CLK_ENABLE, 1);
  HWIO_OUTF(GCC_SPDM_MEMNOC_CY_CBCR, CLK_ENABLE, 1);

  /* Enable AOSS_CC_PLL0_EVEN source for AOP clocks */
  HWIO_OUTF( AOSS_CC_PLL0_USER_CTL, PLLOUT_EVEN, 1);

  Clock_ToggleClock( HWIO_ADDR(GCC_CNOC_PERIPH_EAST_CBCR), CLK_TOGGLE_ENABLE );
  Clock_ToggleClock( HWIO_ADDR(GCC_CNOC_PERIPH_SOUTH_CBCR), CLK_TOGGLE_ENABLE );
  Clock_ToggleClock( HWIO_ADDR(GCC_CNOC_PERIPH_NORTH_CBCR), CLK_TOGGLE_ENABLE );
  
  Clock_ToggleClock( HWIO_ADDR(GCC_CFG_NOC_AH2PHY_XO_CBCR), CLK_TOGGLE_ENABLE );
  Clock_ToggleClock( HWIO_ADDR(GCC_USB_PHY_CFG_AHB2PHY_CBCR), CLK_TOGGLE_ENABLE );
  Clock_ToggleClock( HWIO_ADDR(GCC_AHB2PHY_SOUTH_CBCR), CLK_TOGGLE_ENABLE );
  Clock_ToggleClock( HWIO_ADDR(GCC_CM_PHY_REFGEN1_CBCR), CLK_TOGGLE_ENABLE );
  Clock_ToggleClock( HWIO_ADDR(GCC_CM_PHY_REFGEN2_CBCR), CLK_TOGGLE_ENABLE );

  /* Enable access to clock controllers */
  /* This also solves the HLOS dependency loop between GDSC driver and Clock driver */
  Clock_ToggleClock(HWIO_GCC_VIDEO_AHB_CBCR_ADDR, CLK_TOGGLE_ENABLE);
  Clock_ToggleClock(HWIO_GCC_CAMERA_AHB_CBCR_ADDR, CLK_TOGGLE_ENABLE);
  Clock_ToggleClock(HWIO_GCC_GPU_CFG_AHB_CBCR_ADDR, CLK_TOGGLE_ENABLE);
  Clock_ToggleClock(HWIO_GCC_DISP_AHB_CBCR_ADDR, CLK_TOGGLE_ENABLE);

  /* Needed for Random Stack Canary */
  HWIO_OUTF(GCC_APCS_CLOCK_BRANCH_ENA_VOTE, PRNG_AHB_CLK_ENA, 1);

  /* Config QUP V3 */
  /* V1: Wrap0 core has the same speed at Core2x */
  if(HWIO_INF(TCSR_SOC_HW_VERSION, MAJOR_VERSION) < 2) {
    HWIO_OUTF(GCC_QUPV3_WRAP0_CORE_CDIVR, CLK_DIV, 0 );
  } else {
    /* V2 needs this defaulted to 1 (QCTDD04058124) */
    HWIO_OUT(TCSR_REFGEN_QLINK_ENABLE, 1);
    /* V2 only : Disable CLKREF, HW control will enable it */
    HWIO_OUT(GCC_RX3_MODEM_CLKREF_EN, 0x0);
    
  }

  HWIO_OUTF( GCC_RPM_CLOCK_BRANCH_ENA_VOTE_1, QUPV3_WRAP_0_S_AHB_CLK_ENA, 1 );
  HWIO_OUTF( GCC_RPM_CLOCK_BRANCH_ENA_VOTE_1, QUPV3_WRAP_0_M_AHB_CLK_ENA, 1 );
  HWIO_OUTF( GCC_RPM_CLOCK_BRANCH_ENA_VOTE_1, QUPV3_WRAP0_CORE_CLK_ENA, 1 );
  HWIO_OUTF( GCC_RPM_CLOCK_BRANCH_ENA_VOTE_1, QUPV3_WRAP0_CORE_2X_CLK_ENA, 1 );
  HWIO_OUTF( GCC_RPM_CLOCK_BRANCH_ENA_VOTE_1, QUPV3_WRAP_1_S_AHB_CLK_ENA, 1 );
  HWIO_OUTF( GCC_RPM_CLOCK_BRANCH_ENA_VOTE_1, QUPV3_WRAP_1_M_AHB_CLK_ENA, 1 );
  HWIO_OUTF( GCC_RPM_CLOCK_BRANCH_ENA_VOTE_1, QUPV3_WRAP1_CORE_CLK_ENA, 1 );
  HWIO_OUTF( GCC_RPM_CLOCK_BRANCH_ENA_VOTE_1, QUPV3_WRAP1_CORE_2X_CLK_ENA, 1 );

  if( ! Clock_InitPLLStatic()) return FALSE;

  if( ! Clock_InitCrypto(CLOCK_BOOT_PERF_DEFAULT) ) return FALSE;
  if( ! Clock_InitRBCPR()) return FALSE;

  return TRUE;
}

/* ============================================================================
**  Function : Clock_Init
** ============================================================================
*/
/*!

    This function turns on the required clocks and configures
    Fabric and Krait speeds depending on the System Fabric and
    CPU boot performance level.

    @param eSysPerfLevel   -  [in] Fabric and DDR performance level to initialize.
    @param eCPUPerfLevel   -  [in] Scropion CPU performance level to initialize.

    @return
    TRUE -- Initialization was successful.
    FALSE -- Initialization failed.

   @dependencies
    None.

*/

boolean Clock_Init( ClockBootPerfLevelType eSysPerfLevel,
                    ClockBootPerfLevelType eCPUPerfLevel)
{
  int i;

  if( ! Clock_SetCPUPerfLevel(eCPUPerfLevel)) return FALSE;
  if( ! Clock_SetSysPerfLevel(eSysPerfLevel)) return FALSE;
  if( ! Clock_InitIPA(eSysPerfLevel)) return FALSE;

  Clock_ReplaceUSBBootClockEnable();
  Clock_ReplaceUFSBootClockEnable();

  /* Enable DDR clock measurement be Apps is restricted from DDR clock controller access */
  HWIO_OUTF(MCCC_CH0_TEST_CLOCK_CFG, MC_CLK_ENABLE, 1); /* enable test clock */
  HWIO_OUTF(DPC_REG_DPCC_DEBUG_MUX_MUXR, MUX_SEL, 0x2c); /* Select mccc_debug */
  HWIO_OUTF(DPC_REG_DPCC_DEBUG_DIV_CDIVR, CLK_DIV, 0); /* Choose DIV1 */

  /* Keep REFGEN clock on for V1.  HW mode will be fixed in V2 */
  HWIO_OUT ( PHY_REFGEN_SOUTH_CMN_REFGEN_CTRL2, 0x3 ); 
  HWIO_OUT ( PERIPH_SS_PHY_REFGEN_NORTH_CMN_REFGEN_CTRL2, 0x3 );

  /* For V2, allow HW management of North REFGEN */
  if(HWIO_INF(TCSR_SOC_HW_VERSION, MAJOR_VERSION) > 1) {
    HWIO_OUT( PERIPH_SS_PHY_REFGEN_NORTH_BIAS_EN, 0x4);
  }

  /* Enable HW clock dividers based on activity. */
  if (Boot_Clock_DCD == TRUE)
  {
    Clock_EnableDynClkDivide();
  }

  /* Enable HW clock on/off based on activity, V2 only */
  if(HWIO_INF(TCSR_SOC_HW_VERSION, MAJOR_VERSION) > 1)
  {
    if (Boot_Clock_MEMNOC_DCG == TRUE)
    {
      for (i = 0; i < CLK_NUM_ELEM(Boot_Clock_MEMNOC_CBCRs); i++ )
      {
        /* With the software enables removed, only the hardware controlled enables
         * will remain to allow the clocks to turn off */
        outpdw(Boot_Clock_MEMNOC_CBCRs[i],
               inpdw(Boot_Clock_MEMNOC_CBCRs[i]) & ~0x1UL); // clear bit 0 (CLK_ENABLE)
      }
    }
  }
  return TRUE;
}


/* ========================================================================
**  Function : Clock_PreDDRInitEx
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
boolean Clock_PreDDRInitEx( uint32 ddr_type )
{
  ClockConfigMuxType clkCfgDDR = {HWIO_ADDR(GCC_DDRMC_CH0_ROOT_CMD_RCGR), 0, MUX_GCC, SRC_GPLL2, 4, 0, 0, 0};
  ClockConfigMuxType clkCfgSHRM = {HWIO_ADDR(GCC_SHRM_CMD_RCGR), 0, MUX_GCC, SRC_GPLL0, 3, 0, 0, 0};
  Clock_SetClockConfig( &clkCfgDDR );

  /* Enable all MemNOC clocks */
  Clock_SetClockConfig( &clkCfgSHRM );
  Clock_ToggleClock(HWIO_GCC_SHRM_CFG_AHB_CBCR_ADDR, CLK_TOGGLE_ENABLE);
  Clock_ToggleClock(HWIO_GCC_SHRM_CBCR_ADDR, CLK_TOGGLE_ENABLE);

  Clock_PowerDomainEnable( HWIO_GCC_MMNOC_GDSCR_ADDR );

  /* Enable RETAIN_FF bit for any device 2.01 or higher.  It needs to set up after power on MMNOC_GDSCR */
  if ( (HWIO_IN(TCSR_SOC_HW_VERSION) & 0xFFFF) >= 0x0201 )
  {
    HWIO_OUTF(GCC_MMNOC_GDSCR, RETAIN_FF_ENABLE, 1);
  }

  Clock_ToggleClock(HWIO_GCC_MMNOC_AHB_CFG_CBCR_ADDR, CLK_TOGGLE_ENABLE);
  Clock_ToggleClock(HWIO_GCC_MMNOC_HF_QX_CBCR_ADDR, CLK_TOGGLE_ENABLE);
  Clock_ToggleClock(HWIO_GCC_MMNOC_QOSGEN_EXTREF_CBCR_ADDR, CLK_TOGGLE_ENABLE);
  Clock_ToggleClock(HWIO_GCC_MMNOC_SF_QX_CBCR_ADDR, CLK_TOGGLE_ENABLE);
  Clock_ToggleClock(HWIO_GCC_MMNOC_TBU_HF0_CBCR_ADDR, CLK_TOGGLE_ENABLE);
  Clock_ToggleClock(HWIO_GCC_MMNOC_TBU_HF1_CBCR_ADDR, CLK_TOGGLE_ENABLE);
  Clock_ToggleClock(HWIO_GCC_NOC_MMNOC_DCD_XO_CBCR_ADDR, CLK_TOGGLE_ENABLE);

  Clock_ToggleClock( HWIO_ADDR(GCC_DNOC_CFG_CBCR), CLK_TOGGLE_ENABLE );
  Clock_ToggleClock( HWIO_ADDR(GCC_SNOC_QOSGEN_EXTREF_CBCR), CLK_TOGGLE_ENABLE );

  Clock_ToggleClock(HWIO_GCC_DDRSS_XO_CBCR_ADDR, CLK_TOGGLE_ENABLE);
  Clock_ToggleClock(HWIO_GCC_DDRSS_SLEEP_CBCR_ADDR, CLK_TOGGLE_ENABLE);
  Clock_ToggleClock(HWIO_GCC_DDRSS_MMNOC_SF_QX_CBCR_ADDR, CLK_TOGGLE_ENABLE);
  Clock_ToggleClock(HWIO_GCC_DDRSS_MMNOC_HF_QX_CBCR_ADDR, CLK_TOGGLE_ENABLE);
  Clock_ToggleClock(HWIO_GCC_DDRSS_TCU_CBCR_ADDR, CLK_TOGGLE_ENABLE);
  Clock_ToggleClock(HWIO_GCC_DDRSS_SYS_NOC_GC_AXI_CBCR_ADDR, CLK_TOGGLE_ENABLE);

  Clock_ToggleClock(HWIO_GCC_DDRSS_TURING_AXI_CBCR_ADDR, CLK_TOGGLE_ENABLE);
  Clock_ToggleClock(HWIO_GCC_DDRSS_MSS_Q6_AXI_CBCR_ADDR, CLK_TOGGLE_ENABLE);
  Clock_ToggleClock(HWIO_GCC_DDRSS_GPU_AXI_CBCR_ADDR, CLK_TOGGLE_ENABLE);

  Clock_ToggleClock(HWIO_GCC_DDR_I_HCLK_CBCR_ADDR, CLK_TOGGLE_ENABLE);
  Clock_ToggleClock(HWIO_GCC_DDRMC_CH0_CBCR_ADDR, CLK_TOGGLE_ENABLE);

  Clock_ToggleClock(HWIO_DISP_CC_MDSS_RSCC_AHB_CBCR_ADDR, CLK_TOGGLE_ENABLE);
  Clock_ToggleClock(HWIO_DISP_CC_MDSS_RSCC_VSYNC_CBCR_ADDR, CLK_TOGGLE_ENABLE);

  /* Set MSG_RAM base address for passing smem address to AOP since there is no smem driver in AOP
   * This address is based on target specific.
   * It needs to sync up with AOP team when porting this code to new target
   */
  msg_ram_init_base( (uint32*)(AOP_SS_MSG_RAM_START_ADDRESS_BASE + AOP_SS_MSG_RAM_DRV14_BASE) );

  return TRUE;
}


/* ========================================================================
**  Function : Clock_PreDDRInit
** ======================================================================*/
/*
    Description: Clock_PreDDRInitEx() was added in 8974 to provide a separate API
    for emergency download (EDL) without including Mxdroop work around. Adding wrapper
    function to avoid compilation erros in 8x26 and 8x10.

    @param None
    @return
    TRUE -- Initialization was successful.
    FALSE -- Initialization failed.

    @dependencies
    None.

    @sa None
*/
boolean Clock_PreDDRInit( uint32 ddr_type )
{
  Clock_RailwayType *pClockRailway;

  /* Setup railways */
  pClockRailway = Clock_RailwayConfig();
  pClockRailway->nCxRailId = RAIL_NOT_SUPPORTED_BY_RAILWAY;
  pClockRailway->nEBIRailId = RAIL_NOT_SUPPORTED_BY_RAILWAY;

  /* Only vote for railways for normal boot (Not deviceprogrammer) */
  if ( PcdGet32 (PcdBuildType) == 0 )
  {
    /* Initialize CX & VDDA_EBI Rails */
    pClockRailway->nCxRailId = rail_id(pClockRailway->CxRail);
    pClockRailway->nEBIRailId = rail_id(pClockRailway->EBIRail);

    if( (RAIL_NOT_SUPPORTED_BY_RAILWAY == pClockRailway->nCxRailId) ||
        (RAIL_NOT_SUPPORTED_BY_RAILWAY == pClockRailway->nEBIRailId) )
    {
      return FALSE;
    }

    pClockRailway->CxVoter = railway_create_voter(pClockRailway->nCxRailId,RAILWAY_CLOCK_DRIVER_VOTER_ID);
    pClockRailway->EBIVoter = railway_create_voter(pClockRailway->nEBIRailId,RAILWAY_CLOCK_DRIVER_VOTER_ID);

    railway_corner_vote(pClockRailway->CxVoter, RAILWAY_NOMINAL);
    railway_corner_vote(pClockRailway->EBIVoter, RAILWAY_SVS_MIN);
    railway_transition_rails( );
  }

  return Clock_PreDDRInitEx( ddr_type );
}


/* ========================================================================
**  Function : Clock_I2CInit
** ======================================================================*/
/*
    Description: Configure all clocks needed for EEPROM to be used Pre DDR.

    @param None
    @return
    TRUE -- Initialization was successful.
    FALSE -- Initialization failed.

    @dependencies
    None.

    @sa None
*/
boolean Clock_I2CInit( void )
{
  /* No need to enable I2C for access EEPROM */
  return TRUE;
}


/* ============================================================================
**  Function : Clock_InitForDownloadMode
** ============================================================================
*/
/*!
    Configure clocks for download.  Enable every RAM we want to dump.

    @param None.
    @return
    TRUE -- Initialization was successful.
    FALSE -- Initialization failed.

    @dependencies
    None.

    @sa None
*/
void Clock_InitForDownloadMode(void)
{
  uint32 mask;

  /* Enable the necessary voteable APCS clocks */
  mask =
    HWIO_FMSK(GCC_APCS_CLOCK_BRANCH_ENA_VOTE, IMEM_AXI_CLK_ENA) | 
    HWIO_FMSK(GCC_APCS_CLOCK_BRANCH_ENA_VOTE, BOOT_ROM_AHB_CLK_ENA);
  HWIO_OUTM(GCC_APCS_CLOCK_BRANCH_ENA_VOTE, mask, mask);
}


/* ============================================================================
**  Function : Clock_ExitBoot
** ============================================================================
*/
/*!

    This function turns off clocks that were used during boot, but are not
    needed after boot.  This should be called at the end of boot.

    @return
    TRUE -- Initialization was successful.
    FALSE -- Initialization failed.

   @dependencies
    None.

*/
boolean Clock_ExitBoot(void)
{
  return TRUE;
}


/* ============================================================================
**  Function : Clock_SetSysPerfLevel
** ============================================================================
*/
/**
  Configure NOCs and RPM to a perf level.

  @param eSysPerfLevel [in]  -  NOCs and RPM performance level to configure.

  @return
  TRUE -- NOCs were configured to perf level successful.
  FALSE -- Configuration failed.

  @dependencies
  None.

  @sa
  Clock_Init.
*/

boolean Clock_SetSysPerfLevel
(
  ClockBootPerfLevelType eSysPerfLevel
)
{
  static ClockBootPerfLevelType eCurrLevel = CLOCK_BOOT_PERF_NONE;
  Clock_ConfigType *cfg = Clock_Config();
  uint32 nVer = Clock_ChipVersion();

  /* Don't switch performance level if it has been set previously */
  if ( eCurrLevel == eSysPerfLevel )
  {
    return TRUE;
  }
  eCurrLevel = eSysPerfLevel;

  if(eSysPerfLevel >= CLOCK_BOOT_PERF_NUM) return FALSE;

  /* Configure SNOC */
  Clock_PerfData.nSNOC = cfg->SNOC_Cfg[nVer][eSysPerfLevel].nPerfLevel;
  Clock_SetClockConfig( &cfg->SNOC_Cfg[nVer][eSysPerfLevel] );
  Clock_SetClockConfig( &cfg->SNOC_GC_Cfg[eSysPerfLevel] );
  Clock_SetClockConfig( &cfg->SNOC_SF_Cfg[nVer][eSysPerfLevel] );
  
  /* Aggre NOCs */
  Clock_SetClockConfig( &cfg->AGNOC_Cfg[nVer][eSysPerfLevel] );
  Clock_SetClockConfig( &cfg->AGNOC_EAST_Cfg[nVer][eSysPerfLevel] );
  Clock_SetClockConfig( &cfg->AGNOC_NORTH_Cfg[nVer][eSysPerfLevel] );

  /* Configure CNOC */
  Clock_PerfData.nCNOC = cfg->CNOC_Cfg[eSysPerfLevel].nPerfLevel;
  Clock_SetClockConfig( &cfg->CNOC_Cfg[eSysPerfLevel] );
  Clock_SetClockConfig( &cfg->CNOC_Periph_Cfg[eSysPerfLevel] );

  /* Config SHUB */
  Clock_PerfData.nSHUB = cfg->MEMNOC_Cfg[nVer][eSysPerfLevel].nPerfLevel;
  Clock_SetClockConfig( &cfg->MEMNOC_Cfg[nVer][eSysPerfLevel] );
  Clock_SetClockConfig( &cfg->CPU_AXI_Cfg[eSysPerfLevel] );
  Clock_SetClockConfig( &cfg->TURING_AXI_Cfg[eSysPerfLevel] );
  Clock_SetClockConfig( &cfg->GPU_MEMNOC_Cfg[nVer][eSysPerfLevel] );
  Clock_SetClockConfig( &cfg->Q6_MEMNOC_Cfg[nVer][eSysPerfLevel] );

  return TRUE;
}


/* ============================================================================
**  Function : Clock_InitCrypto
** ============================================================================
*/
/*!

Configure Crypto clocks.

    @param None.
    @return
    TRUE -- Initialization was successful.
    FALSE -- Initialization failed.

    @dependencies
    None.

    @sa None
*/

boolean Clock_InitCrypto
(
  ClockBootPerfLevelType eSysPerfLevel
)
{
  uint32 mask;
  Clock_ConfigType *cfg = Clock_Config();
  static ClockBootPerfLevelType eCurrLevel = CLOCK_BOOT_PERF_NONE;

  /* Don't switch performance level if it has been set previously */
  if ( eCurrLevel == eSysPerfLevel )
  {
    return TRUE;
  }
  eCurrLevel = eSysPerfLevel;

  if(eSysPerfLevel >= CLOCK_BOOT_PERF_NUM) return FALSE;

  /* Configure crypto engine */
  Clock_SetClockConfig( &cfg->CE_Cfg[eSysPerfLevel] );
  Clock_PerfData.nCE = cfg->CE_Cfg[eSysPerfLevel].nPerfLevel;

  mask =
    HWIO_FMSK(GCC_RPM_CLOCK_BRANCH_ENA_VOTE, CE1_CLK_ENA) |
    HWIO_FMSK(GCC_RPM_CLOCK_BRANCH_ENA_VOTE, CE1_AXI_CLK_ENA) |
    HWIO_FMSK(GCC_RPM_CLOCK_BRANCH_ENA_VOTE, CE1_AHB_CLK_ENA );
  HWIO_OUTM(GCC_RPM_CLOCK_BRANCH_ENA_VOTE, mask, mask);

  return TRUE;

} /* END Clock_InitCrypto */


/* ============================================================================
**  Function : Clock_InitRBCPR
** ===========================================================================*/
/*!
    Configure RBCPR clocks.

    @param None.
    @return
    TRUE -- Initialization was successful.
    FALSE -- Initialization failed.

    @dependencies
    None.

    @sa None
*/

boolean Clock_InitRBCPR(void)
{
  /* Enable RPCPR clocks */
  Clock_ToggleClock(HWIO_GCC_RBCPR_CX_AHB_CBCR_ADDR, CLK_TOGGLE_ENABLE);
  Clock_ToggleClock(HWIO_GCC_RBCPR_CX_CBCR_ADDR, CLK_TOGGLE_ENABLE);
  Clock_ToggleClock(HWIO_GCC_RBCPR_MX_AHB_CBCR_ADDR, CLK_TOGGLE_ENABLE);
  Clock_ToggleClock(HWIO_GCC_RBCPR_MX_CBCR_ADDR, CLK_TOGGLE_ENABLE);
  Clock_ToggleClock(HWIO_GCC_CPUSS_RBCPR_CBCR_ADDR, CLK_TOGGLE_ENABLE);

  /* Enable GPU clocks */
  Clock_ToggleClock(HWIO_GPUCC_GPU_CC_RBCPR_CBCR_ADDR, CLK_TOGGLE_ENABLE);

  Clock_ToggleClock(HWIO_GCC_MSS_CFG_AHB_CBCR_ADDR, CLK_TOGGLE_ENABLE);

  return TRUE;

} /* END Clock_InitRBCPR */


/* ============================================================================
**  Function : Clock_InitIPA
** ===========================================================================*/
/*!
    Configure and enable IPA clocks

    @param None.
    @return
    TRUE -- Initialization was successful.
    FALSE -- Initialization failed.

    @dependencies
    None.

    @sa None
*/
boolean Clock_InitIPA( ClockBootPerfLevelType eSysPerfLevel )
{
  Clock_ConfigType *cfg = Clock_Config();
  static ClockBootPerfLevelType eCurrLevel = CLOCK_BOOT_PERF_NONE;

  /* Don't switch performance level if it has been set previously */
  if ( eCurrLevel == eSysPerfLevel )
  {
    return TRUE;
  }
  eCurrLevel = eSysPerfLevel;

  if(eSysPerfLevel >= CLOCK_BOOT_PERF_NUM) return FALSE;

  /* Configure IPA */
  
  Clock_SetClockConfig( &cfg->IPA_Cfg[eSysPerfLevel] );
  Clock_PerfData.nIPA = cfg->IPA_Cfg[eSysPerfLevel].nPerfLevel;

  /* Enable IPA power */
  HWIO_OUTF(GCC_IPA_GDSCR, SW_COLLAPSE, 0);
  if( ! Clock_ToggleClock(HWIO_GCC_IPA_AHB_CBCR_ADDR, CLK_TOGGLE_ENABLE)) return FALSE;
  if( ! Clock_ToggleClock(HWIO_GCC_AGGRE_NOC_IPA_CBCR_ADDR, CLK_TOGGLE_ENABLE)) return FALSE;
  if( ! Clock_ToggleClock(HWIO_GCC_IPA_2X_CBCR_ADDR, CLK_TOGGLE_ENABLE)) return FALSE;
  if( ! Clock_ToggleClock(HWIO_GCC_IPA_CBCR_ADDR, CLK_TOGGLE_ENABLE)) return FALSE;
  if( ! Clock_ToggleClock(HWIO_GCC_IPA_SLEEP_CBCR_ADDR, CLK_TOGGLE_ENABLE)) return FALSE;

  return TRUE;
} /* END Clock_InitIPA */


/* ============================================================================
**  Function : Clock_GetClockFrequency
** ============================================================================
*/
/*!
    Get the requested clock frequency in hertz.

    @param
      eBootLogClock [in]  - targeted clock
      pnFrequencyHz [out] - frequency of clock in hertz

    @return
      True iff the requested clock frequency is placed in pnFrequencyHz.

    @dependencies
    None.

    @sa None
*/

boolean Clock_GetClockFrequency(ClockBootLogQueryType eBootLogClock,
                                uint32 *pnFrequencyHz)
{
  boolean ret = TRUE;

  switch(eBootLogClock)
  {
  case CLK_BOOT_LOG_APPS_CLUSTER_0:
    *pnFrequencyHz = (Clock_GetAPSSCL0SpeedKHz() * 1000);
    break;

  case CLK_BOOT_LOG_DDR:
    *pnFrequencyHz = (Clock_DDRSpeed() * 1000);
    break;

  default:
    ret = FALSE;
    break;
  }

  return ret;
}


/* ============================================================================
**  Function : Clock_InitRPM()
** ============================================================================
*/
/**
  Perform any initialization needed just before RPM starts.  SMEM is available
  at this time.

  @param
  @return
  TRUE -- Initialization succeeded.
  FALSE -- Initialization failed.

  @dependencies
  None.

  @sa
  Clock_Init.
  boot_smem_init
*/
boolean Clock_InitRPM( void )
{
  return Clock_ConfigAOP();
}


/* ============================================================================
**  Function : Clock_InitPLLStatic()
** ============================================================================
*/
/**
  Perform initial static configurations and workarounds
  defined in frequency plan documentation,

  @param
  @return
  TRUE -- Initialization succeeded.
  FALSE -- Initialization failed.

  @dependencies
  None.

*/
boolean Clock_InitPLLStatic( void )
{
  /* Updating the Apps PLLs here provides a simple single bootloader release method when the 
   * recommended PLL values change.  */
  const uint64 nMMSSPLLsConfigCtlFab[] = 
  {
    HWIO_ADDR(CAM_CC_PLL0_USER_CTL_U),
    HWIO_ADDR(CAM_CC_PLL1_USER_CTL_U),
    HWIO_ADDR(CAM_CC_PLL2_USER_CTL_U),
    HWIO_ADDR(CAM_CC_PLL3_USER_CTL_U),
    HWIO_ADDR(VIDEO_CC_VIDEO_PLL0_USER_CTL_U),
    HWIO_ADDR(GPUCC_GPU_CC_PLL0_USER_CTL_U),
    HWIO_ADDR(GPUCC_GPU_CC_PLL1_USER_CTL_U),
    HWIO_ADDR(DISP_CC_PLL0_USER_CTL_U),
  };

  int i;

  /* Adds SCALE_ON_RESTART, which is safe for an active PLL like GPLL0 */
  HWIO_OUT(GCC_GPLL0_USER_CTL_U, FABIA_USER_CTL_U_VAL);

  for(i = 0; i < CLK_NUM_ELEM(nMMSSPLLsConfigCtlFab); i++)
  {
    outpdw(nMMSSPLLsConfigCtlFab[i]+0x0, FABIA_USER_CTL_U_VAL );
    outpdw(nMMSSPLLsConfigCtlFab[i]+0x4, FABIA_CONFIG_CTL_VAL & 0xffffffff);
    outpdw(nMMSSPLLsConfigCtlFab[i]+0x8, (FABIA_CONFIG_CTL_VAL>>32) & 0xffffffff);
    outpdw(nMMSSPLLsConfigCtlFab[i]+0xC, FABIA_PLL_TEST_CTL_VAL & 0xffffffff);
    outpdw(nMMSSPLLsConfigCtlFab[i]+0x10, (FABIA_PLL_TEST_CTL_VAL>>32) & 0xffffffff);
  }

  return TRUE;

}

static void Clock_EnableDynClkDivide (void)
{
  int i;

  // V1/V2 CONFLICT! { HWIO_AGGRE_NOC_A1NOC_DCD_MAXDIV_LOW_ADDR, 0x4 },
  // This is a little bit hard to maintain, but about 800 bytes smaller than
  // two separately compiled lists of registers based on version.
  if(HWIO_INF(TCSR_SOC_HW_VERSION, MAJOR_VERSION) < 2) {
    outpdw(HWIO_V1_AGGRE_NOC_A1NOC_DCD_MAXDIV_LOW_ADDR, 4);
  } else {
    outpdw(HWIO_AGGRE_NOC_A1NOC_DCD_MAXDIV_LOW_ADDR, 4);
  }

  for (i = 0; i < CLK_NUM_ELEM(Clock_DCD_Maxdiv); i++ )
  {
    outpdw(Clock_DCD_Maxdiv[i].address, Clock_DCD_Maxdiv[i].value);
  }
}
