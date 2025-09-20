/**
==============================================================================
  @file ClockXBL.c

  Clock functions for the XBL Loader Clock Driver.

  Copyright (c) 2017 QUALCOMM Technologies, Inc.  All Rights Reserved.  
  QUALCOMM Proprietary and Confidential.

==============================================================================


==============================================================================

                            EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.


  when      who     what, where, why
  --------  ------  ------------------------------------------------------
  05/16/17  vphan   Initial SDM1000 revision, branched from SDM855.

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
#include "cpr.h"
#include "busywait.h"

/*=========================================================================
                       MACRO DEFINITIONS
==========================================================================*/


/*=========================================================================
      Data
==========================================================================*/
/* Clock performance CP for sharing with AOP */
ClockCfgCPType Clock_PerfData;

/* Variables used while switching the rail corners */

/* aClientRailwayCorner is a two dimensional aaray with 2 clients (BOOT, DDR) as ROWs and 3 Rails (Cx, Mx, APC) as Coloumns
 *  Used to store the current vote of the clients
 */
railway_corner aClientRailwayCorner[CLOCK_RAIL_CLIENT_NUM][CLOCK_RAIL_NUM] = {
                             /* CLOCK_RAIL_CX,  CLOCK_RAIL_MX,  CLOCK_RAIL_APC */
/* CLOCK_RAIL_CLIENT_BOOT */ { RAILWAY_NOMINAL, RAILWAY_NOMINAL, RAILWAY_NOMINAL },

                             /* CLOCK_RAIL_CX,  CLOCK_RAIL_MX,  CLOCK_RAIL_APC */
/* CLOCK_RAIL_CLIENT_DDR */  { RAILWAY_NOMINAL, RAILWAY_NOMINAL, RAILWAY_NOMINAL }
};

/* aCurrRailwayCorner is used for holding the Current Railway Corner vote.
 * This is the aggregated final vote from different clients' requests for a given Rail
 */
railway_corner aCurrRailwayCorner[CLOCK_RAIL_NUM] = {
  /* CLOCK_RAIL_CX  */ RAILWAY_NOMINAL,
  /* CLOCK_RAIL_MX  */ RAILWAY_NOMINAL,
  /* CLOCK_RAIL_APC */ RAILWAY_NOMINAL
};

boolean bClockRailwayInit = FALSE;

/* This list is every clock that defaults OFF that is owned by RPMh or Boot */
uint64 ClockRequiredPowerDomains[] = {
  HWIO_ADDR(GCC_IPA_GDSCR),
  HWIO_ADDR(GCC_MMNOC_GDSCR),
  0,
};

uint64 ClockRequiredList[] = {
  HWIO_ADDR(AOSS_CC_SPMI_AOD_SER_CBCR),
  HWIO_ADDR(AOSS_CC_SPMI_CFG_AHB_CBCR),
  HWIO_ADDR(AOSS_CC_SPMI_SER_CBCR),
  HWIO_ADDR(GCC_AGGRE_NOC_AUDIO_TBU_CBCR),
  HWIO_ADDR(GCC_AGGRE_NOC_IPA_CBCR),
  HWIO_ADDR(GCC_AGGRE_NOC_QOSGEN_EXTREF_CBCR),
  HWIO_ADDR(GCC_AGGRE_NOC_TBU1_CBCR),
  HWIO_ADDR(GCC_AGGRE_NOC_TBU2_CBCR),
  HWIO_ADDR(GCC_AHB2PHY_CENTER_XO_CBCR),
  HWIO_ADDR(GCC_AHB2PHY_NORTH_CBCR),
  HWIO_ADDR(GCC_AHB2PHY_SOUTH_CBCR),
  HWIO_ADDR(GCC_AHB2PHY_WEST_XO_CBCR),
  HWIO_ADDR(GCC_CFG_NOC_AH2PHY_XO_CBCR),
  HWIO_ADDR(GCC_CPUSS_RBCPR_CBCR),
  HWIO_ADDR(GCC_DCC_AHB_CBCR),
  HWIO_ADDR(GCC_DDR_I_HCLK_CBCR),
  //HWIO_ADDR(GCC_DDRMC_CH0_CBCR), // These two need to be carefully controlled.
  //HWIO_ADDR(GCC_DDRMC_CH1_CBCR), // So wait until Clock_PreDDRInit().
  HWIO_ADDR(GCC_DDRSS_GPU_AXI_CBCR),
  HWIO_ADDR(GCC_DDRSS_MMNOC_HF_QX_CBCR),
  HWIO_ADDR(GCC_DDRSS_MMNOC_SF_QX_CBCR),
  HWIO_ADDR(GCC_DDRSS_MSS_Q6_AXI_CBCR),
  HWIO_ADDR(GCC_DDRSS_SLEEP_CBCR),
  HWIO_ADDR(GCC_DDRSS_SYS_NOC_GC_AXI_CBCR),
  HWIO_ADDR(GCC_DDRSS_TCU_CBCR),
  HWIO_ADDR(GCC_DDRSS_TURING_AXI_CBCR),
  HWIO_ADDR(GCC_DDRSS_XO_CBCR),
  HWIO_ADDR(GCC_DNOC_CFG_CBCR),
  HWIO_ADDR(GCC_GPU_CFG_AHB_CBCR),
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
  HWIO_ADDR(GCC_MSS_CFG_AHB_CBCR),
  HWIO_ADDR(GCC_NOC_CENTER_DCD_XO_CBCR),
  HWIO_ADDR(GCC_NOC_COMPUTE_DCD_XO_CBCR),
  HWIO_ADDR(GCC_NOC_EAST_DCD_XO_CBCR),
  HWIO_ADDR(GCC_NOC_LPASS_DCD_XO_CBCR),
  HWIO_ADDR(GCC_NOC_MMNOC_CNOC_DCD_XO_CBCR),
  HWIO_ADDR(GCC_NOC_MMNOC_DCD_XO_CBCR),
  HWIO_ADDR(GCC_NOC_MONAQ_DCD_XO_CBCR),
  HWIO_ADDR(GCC_NOC_SOUTH_DCD_XO_CBCR),
  HWIO_ADDR(GCC_NOC_SOUTHWEST_DCD_XO_CBCR),
  HWIO_ADDR(GCC_NOC_WEST_DCD_XO_CBCR),
  HWIO_ADDR(GCC_PIMEM_AHB_CBCR),
  HWIO_ADDR(GCC_PIMEM_AXI_CBCR),
  HWIO_ADDR(GCC_QREFS_VBG_CAL_CBCR),
  HWIO_ADDR(GCC_RBCPR_CX_AHB_CBCR),
  HWIO_ADDR(GCC_RBCPR_CX_CBCR),
  HWIO_ADDR(GCC_RBCPR_MMCX_AHB_CBCR),
  HWIO_ADDR(GCC_RBCPR_MMCX_CBCR),
  HWIO_ADDR(GCC_RBCPR_MX_AHB_CBCR),
  HWIO_ADDR(GCC_RBCPR_MX_CBCR),
  HWIO_ADDR(GCC_SHRM_CFG_AHB_CBCR),
  HWIO_ADDR(GCC_SHRM_CBCR),
  HWIO_ADDR(GCC_SLEEP_CBCR),
  HWIO_ADDR(GCC_SNOC_QOSGEN_EXTREF_CBCR),
  HWIO_ADDR(GCC_SPDM_CFG_AHB_CBCR),
  //HWIO_ADDR(GCC_SPDM_DEBUG_CY_CBCR), Must be enabled separately with no status check.
  HWIO_ADDR(GCC_SPDM_FF_CBCR),
  HWIO_ADDR(GCC_SPDM_MEMNOC_CY_CBCR),
  HWIO_ADDR(GCC_SPDM_MSTR_AHB_CBCR),
  HWIO_ADDR(GCC_SPDM_PNOC_CY_CBCR),
  HWIO_ADDR(GCC_SPDM_SNOC_CY_CBCR),
  HWIO_ADDR(GCC_SYS_NOC_GC_AXI_CBCR),
  HWIO_ADDR(GCC_TURING_TBU0_CBCR),
  HWIO_ADDR(GCC_TURING_TBU1_CBCR),
  HWIO_ADDR(GPUCC_GPU_CC_RBCPR_CBCR),
  0,
};

/* DCD control variables. */
boolean Boot_Clock_DCD = TRUE;

/* List of registers to initialize for DCD setup. */
struct Clock_DCD_rinit
{
  uint64 address;
  uint32 value;
};

struct Clock_DCD_rinit Clock_DCD[] =
{
  // DCD MAXDIV registers
  { HWIO_AGGRE_NOC_A1NOC_DCD_SF_PCIE3_MAXDIV_LOW_ADDR, 4},
  { HWIO_AGGRE_NOC_A1NOC_DCD_SOUTHWEST_MAXDIV_LOW_ADDR, 4},
  { HWIO_AGGRE_NOC_A2NOC_DCD_EAST_MAXDIV_LOW_ADDR, 4},
  { HWIO_AGGRE_NOC_A2NOC_DCD_MONAQ_MAXDIV_LOW_ADDR, 4},
  { HWIO_AGGRE_NOC_A2NOC_DCD_SF_LPASS_MAXDIV_LOW_ADDR, 4},
  { HWIO_AGGRE_NOC_A2NOC_DCD_WEST_MAXDIV_LOW_ADDR, 4},
  { HWIO_AGGRE_NOC_CDSPNOC_DCD_EAST_CDSP_MAXDIV_LOW_ADDR, 4},
  { HWIO_AGGRE_NOC_CDSPNOC_DCD_EAST_NPU_MAXDIV_LOW_ADDR, 4},
  { HWIO_CNOC_DCD_CENTER_QHCLK_MAXDIV_LOW_ADDR, 1},
  { HWIO_CNOC_DCD_COMPUTE_QHCLK_MAXDIV_LOW_ADDR, 4},
  { HWIO_CNOC_DCD_EAST_QHCLK_MAXDIV_LOW_ADDR, 4},
  { HWIO_CNOC_DCD_MMNOC_QHCLK_MAXDIV_LOW_ADDR, 4},
  { HWIO_CNOC_DCD_MONAQ_QHCLK_MAXDIV_LOW_ADDR, 4},
  { HWIO_CNOC_DCD_SOUTH_QHCLK_MAXDIV_LOW_ADDR, 4},
  { HWIO_CNOC_DCD_SOUTHWEST_QHCLK_MAXDIV_LOW_ADDR, 4},
  { HWIO_CNOC_DCD_WEST_QHCLK_MAXDIV_LOW_ADDR, 4},
  { HWIO_DC_NOC_DC_NOC_DCD_MAXDIV_LOW_ADDR, 4},
  { HWIO_MNOC_DCD_HF_QXCLK_MAXDIV_LOW_ADDR, 4},
  { HWIO_MNOC_DCD_SF_QXCLK_MAXDIV_LOW_ADDR, 4},
  { HWIO_SNOC_DCD_CENTER_QX_CLK_MAXDIV_LOW_ADDR, 4},
  { HWIO_SNOC_DCD_MONAQ_HS_QX_CLK_MAXDIV_LOW_ADDR, 4},
  { HWIO_SNOC_DCD_MONAQ_QX_CLK_MAXDIV_LOW_ADDR, 4},
  { HWIO_SNOC_DCD_PIMEM_CLK_MAXDIV_LOW_ADDR, 4},
  { HWIO_SNOC_DCD_SF_QX_CLK_MAXDIV_LOW_ADDR, 4},
  { HWIO_SNOC_DCD_SOUTH_QX_CLK_MAXDIV_LOW_ADDR, 4},
  { HWIO_SNOC_DCD_WEST_QX_CLK_MAXDIV_LOW_ADDR, 4},
  { HWIO_TITAN_A_CAMNOC_CAMNOC_DCD_MAIN_DYNAMICCLOCKDIVISION_MAXDIV_LOW_ADDR, 4},

  // DCD enables
  { HWIO_GCC_AGGRE_NOC_CDSP_NOC_DCD_CDIV_DCDR_ADDR, 0x1 },
  { HWIO_GCC_AGGRE_NOC_DCD_CDIV_DCDR_ADDR, 0x1 },
  { HWIO_GCC_AGGRE_NOC_EAST_DCD_CDIV_DCDR_ADDR, 0x1 },
  { HWIO_GCC_AGGRE_NOC_LPASS_SF_DCD_CDIV_DCDR_ADDR, 0x1 },
  { HWIO_GCC_AGGRE_NOC_MONAQ_DCD_CDIV_DCDR_ADDR, 0x1 },
  { HWIO_GCC_AGGRE_NOC_SOUTH_SF_DCD_CDIV_DCDR_ADDR, 0x1 },
  { HWIO_GCC_AGGRE_NOC_SOUTHWEST_DCD_CDIV_DCDR_ADDR, 0x1 },
  { HWIO_GCC_AGGRE_NOC_WEST_DCD_CDIV_DCDR_ADDR, 0x1 },
  { HWIO_GCC_CONFIG_NOC_CENTER_DCD_CDIV_DCDR_ADDR, 0x1 },
  { HWIO_GCC_CONFIG_NOC_COMPUTE_DCD_CDIV_DCDR_ADDR, 0x1 },
  { HWIO_GCC_CONFIG_NOC_EAST_DCD_CDIV_DCDR_ADDR, 0x1 },
  { HWIO_GCC_CONFIG_NOC_MMNOC_DCD_CDIV_DCDR_ADDR, 0x1 },
  { HWIO_GCC_CONFIG_NOC_MONAQ_DCD_CDIV_DCDR_ADDR, 0x1 },
  { HWIO_GCC_CONFIG_NOC_SOUTH_DCD_CDIV_DCDR_ADDR, 0x1 },
  { HWIO_GCC_CONFIG_NOC_SOUTHWEST_DCD_CDIV_DCDR_ADDR, 0x1 },
  { HWIO_GCC_CONFIG_NOC_WEST_DCD_CDIV_DCDR_ADDR, 0x1 },
  { HWIO_GCC_CPUSS_AXI_DCD_CDIV_DCDR_ADDR, 0x1 },
  //{ HWIO_GCC_DDRMC_CH0_ROOT_DCD_CDIV_DCDR_ADDR, 0x1 }, // Should be DCG, Clocks HPG Ch 4
  //{ HWIO_GCC_DDRMC_CH1_ROOT_DCD_CDIV_DCDR_ADDR, 0x1 }, // Should be DCG, Clocks HPG Ch 4
  //{ HWIO_GCC_GPU_MEMNOC_GFX_DCD_CDIV_DCDR_ADDR, 0x1 }, // Should be DCG, Clocks HPG Ch 4
  { HWIO_GCC_IPA_2X_DCD_CDIV_DCDR_ADDR, 0x1 },
  //{ HWIO_GCC_MEMNOC_DCD_CDIV_DCDR_ADDR, 0x1 }, // Should be DCG, Clocks HPG Ch 4
  { HWIO_GCC_MMNOC_HF_QX_DCD_CDIV_DCDR_ADDR, 0x1 },
  { HWIO_GCC_MMNOC_SF_QX_DCD_CDIV_DCDR_ADDR, 0x1 },
  // { HWIO_GCC_MMU_TCU_DCD_CDIV_DCDR_ADDR, 0x1 }, // Should be DCG, Clocks HPG Ch 4
  // { HWIO_GCC_MSS_Q6_MEMNOC_AXI_DCD_CDIV_DCDR_ADDR, 0x1 }, // Should be DCG, Clocks HPG Ch 4
  { HWIO_GCC_NPU_AXI_DCD_CDIV_DCDR_ADDR, 0x1 },
  // { HWIO_GCC_SHRM_DCD_CDIV_DCDR_ADDR, 0x1 }, // Should be DCG, Clocks HPG Ch 4
  { HWIO_GCC_SYS_NOC_DCD_CDIV_DCDR_ADDR, 0x1 },
  { HWIO_GCC_SYS_NOC_GC_DCD_CDIV_DCDR_ADDR, 0x1 },
  { HWIO_GCC_SYS_NOC_LPASS_DCD_CDIV_DCDR_ADDR, 0x1 },
  { HWIO_GCC_SYS_NOC_MONAQ_AXI_DCD_CDIV_DCDR_ADDR, 0x1 },
  { HWIO_GCC_SYS_NOC_MONAQ_DCD_CDIV_DCDR_ADDR, 0x1 },
  { HWIO_GCC_SYS_NOC_MONAQ_SF_DCD_CDIV_DCDR_ADDR, 0x1 },
  { HWIO_GCC_SYS_NOC_SF_DCD_CDIV_DCDR_ADDR, 0x1 },
  { HWIO_GCC_SYS_NOC_SOUTH_DCD_CDIV_DCDR_ADDR, 0x1 },
  //{ HWIO_GCC_TURING_AXI_DCD_CDIV_DCDR_ADDR, 0x1 },// Should be DCG, Clocks HPG Ch 4
  { HWIO_VIDEO_CC_IRIS_DIV_DCDR_ADDR, 0x1},
};

/* Enable Hardware Control of MEMNOC related clocks */
boolean Boot_Clock_MEMNOC_DCG = TRUE;

uint64 Boot_Clock_MEMNOC_CBCRs[] = 
{
  HWIO_GCC_DDRSS_MSS_Q6_AXI_CBCR_ADDR,
  HWIO_GCC_MSS_Q6_MEMNOC_AXI_CBCR_ADDR,
  HWIO_GCC_DDRSS_GPU_AXI_CBCR_ADDR,
  //HWIO_ADDR(GCC_GPU_MEMNOC_GFX_CBCR),
  //HWIO_ADDR(GCC_MEMNOC_CBCR), // HW Errata (QCTDD04864232): Don't enable clock gating at GCC for SHUB clock
  HWIO_GCC_SHRM_CBCR_ADDR,
  HWIO_GCC_DDRMC_CH0_CBCR_ADDR,
  HWIO_GCC_DDRMC_CH1_CBCR_ADDR,
  HWIO_GCC_TURING_AXI_CBCR_ADDR,
  HWIO_GCC_DDRSS_TURING_AXI_CBCR_ADDR,
};

/*=========================================================================
      Function Prototypes
==========================================================================*/

boolean Clock_InitCrypto(ClockBootPerfLevelType eSysPerfLevel);
boolean Clock_InitPLLStatic(void);
static void Clock_EnableDynClkDivide (void);
extern void Clock_AGGRENOCInit(void);
extern void Clock_ReplaceUSBBootClockEnable(void);
extern void Clock_ReplaceUFSBootClockEnable(void);
extern void Clock_StoreDDRFreqKHz ( uint32 nFreqKHz );
extern boolean Clock_CopyBIMCPlanToRPM( void );
extern uint32 Clock_GetAPSSCL0SpeedKHz(void);
extern boolean Clock_ConfigAOP( void );
extern void msg_ram_init_base(uint32 *msg_ram_addr);

/*===========================================================================
                      FUNCTION DECLARATIONS
===========================================================================*/
/* ============================================================================
**  Function : Clock_MaxRailVote
** ============================================================================
*/
/**
  Return the Max corner vote for a given rail.

  @param eRail [in]         Rail to search for corner vote from all the clients.
  @return - Max Corner Vote

  @dependencies
  None.
*/

railway_corner Clock_MaxRailVote(ClockRailType eRail)
{
  ClockRailClinet eClient = 0;
  railway_corner rail_max_corner = RAILWAY_NO_REQUEST;

  while( eClient < CLOCK_RAIL_CLIENT_NUM)
  {
    if ( rail_max_corner < aClientRailwayCorner[eClient][eRail] )
    {
      rail_max_corner = aClientRailwayCorner[eClient][eRail];
    }
      eClient++;
  }
  return rail_max_corner;
}

/* ============================================================================
**  Function : Clock_SwitchRail
** ============================================================================
*/
/**
  Switch the given Rail to given corner.

  @param eRail [in]         Rail to be switched.
  @param corner_vote [in]   Railway corner to switch to.
  @param eClient [in]       Requesting Client.
  @return - FALSE on fail, TRUE on success

  @dependencies
  None.
*/

boolean Clock_SwitchRail(ClockRailType eRail, railway_corner corner_vote, ClockRailClinet eClient)
{
  Clock_RailwayType *pClockRailway = Clock_RailwayConfig();
  railway_corner MaxCornerVote;

  if(eRail >= CLOCK_RAIL_NUM || eClient >= CLOCK_RAIL_CLIENT_NUM)
  {
    return FALSE;
  }

  if(bClockRailwayInit == FALSE) return TRUE;

  aClientRailwayCorner[eClient][eRail] = corner_vote;
  MaxCornerVote = Clock_MaxRailVote(eRail);
  if( aCurrRailwayCorner[eRail] != MaxCornerVote)
  {
    /* Make sure Rails are running at minimum NOM level */
    if( MaxCornerVote < RAILWAY_NOMINAL) MaxCornerVote = RAILWAY_NOMINAL;

    if(eRail == CLOCK_RAIL_CX)
    {
      railway_corner_vote(pClockRailway->CxVoter, MaxCornerVote);
      railway_transition_rails( );
    }

    if(eRail == CLOCK_RAIL_MX)
    {
      railway_corner_vote(pClockRailway->EBIVoter, MaxCornerVote);
      railway_transition_rails( );
    }

    /* For APC rail custom API implemented and it is supported for TURBO level only */
    if(eRail == CLOCK_RAIL_APC)
    {
       if(MaxCornerVote == RAILWAY_TURBO)
       {
        cpr_set_apc0_olv(APC_TUR);
        aCurrRailwayCorner[eRail] = MaxCornerVote;
       }
       return TRUE;
    }
    aCurrRailwayCorner[eRail] = MaxCornerVote;
  }

  return TRUE;
}
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
  int i;

  HWIO_OUTF(GCC_GPLL0_USER_CTL, PLLOUT_EVEN, 1);

  /* To improve boot time, speed up the CPU and buses */
  if( ! Clock_SetSysPerfLevel(CLOCK_BOOT_PERF_DEFAULT)) return FALSE;
  if( ! Clock_SetCPUPerfLevel(CLOCK_BOOT_PERF_DEFAULT)) return FALSE;

  /* Enable required boot power domains */
  for( i=0; ClockRequiredPowerDomains[i] != 0; i++)
  {
    Clock_PowerDomainEnable (ClockRequiredPowerDomains[i]);
  }
  /* Enable the RPMh and Boot required clocks */
  Clock_ToggleEnableList( ClockRequiredList );
  // For power savings, it enables only when gcc_debug_clk is enabled. Skip status check.
  HWIO_OUTF(GCC_SPDM_DEBUG_CY_CBCR, CLK_ENABLE, 1);

  /* Factored out of V1 section to apply to V1 and V2 */
  Clock_ToggleClock( HWIO_ADDR(GCC_AHB2PHY_CENTER_XO_CBCR), CLK_TOGGLE_ENABLE );

  /* On SC8180 V2, the REFGEN was fixed */
  if( Clock_ChipVersion() == V1 )
  {
    /* The REFGEN instances require an override on V1 */
    /* Needs GCC_CFG_NOC_SOUTH_AHB_CLK : Defaults ON */
    HWIO_OUT(AHB2PHY_SOUTH_PHY_REFGEN_SOUTH_0_RESTRIM_BYPASS_CODE, 0x4E);
    HWIO_OUT(AHB2PHY_SOUTH_PHY_REFGEN_SOUTH_0_RSM_CNTRL, 0x8);
    HWIO_OUT(AHB2PHY_SOUTH_PHY_REFGEN_SOUTH_1_RESTRIM_BYPASS_CODE, 0x4E);
    HWIO_OUT(AHB2PHY_SOUTH_PHY_REFGEN_SOUTH_1_RSM_CNTRL, 0x8);
    /* Needs GCC_NOC_CENTER_DCD_XO_CLK : from ClockRequiredList */
    HWIO_OUT(REFGEN_CENTER_PHY_REFGEN_CENTER_RESTRIM_BYPASS_CODE, 0x4E);
    HWIO_OUT(REFGEN_CENTER_PHY_REFGEN_CENTER_RSM_CNTRL, 0x8);
    /* Needs GCC_NOC_EAST_DCD_XO_CLK : from ClockRequiredList */
    HWIO_OUT(REFGEN_EAST_PHY_REFGEN_EAST_RESTRIM_BYPASS_CODE, 0x4E);
    HWIO_OUT(REFGEN_EAST_PHY_REFGEN_EAST_RSM_CNTRL, 0x8);
  }

  Clock_AGGRENOCInit();

  /*
   * This clock required for MMNOC power collapse to work. If not enabled, the
   * MMNOC GDS HW controller will get stuck waiting on an ACK from MMNOC.
   * Don't include as part of ClockRequiredList, since Clock_ToggleClock will
   * poll for CLK_OFF = 0, but this clock will be forced off by the MMNOC GDSC
   * at this point.
   */
  HWIO_OUTF(GCC_MMNOC_AT_CBCR, CLK_ENABLE, 1);

  /* Enable access to clock controllers */
  Clock_ToggleClock(HWIO_GCC_GPU_CFG_AHB_CBCR_ADDR, CLK_TOGGLE_ENABLE);

  /* Needed for Random Stack Canary */
  HWIO_OUTF(GCC_APCS_CLOCK_BRANCH_ENA_VOTE, PRNG_AHB_CLK_ENA, 1);
  /* Hold it on with an RPM vote, CR 2115680 */
  HWIO_OUTF(GCC_RPM_CLOCK_BRANCH_ENA_VOTE, PRNG_AHB_CLK_ENA, 1);

  HWIO_OUTF( GCC_RPM_CLOCK_BRANCH_ENA_VOTE_1, QUPV3_WRAP_0_S_AHB_CLK_ENA, 1 );
  HWIO_OUTF( GCC_RPM_CLOCK_BRANCH_ENA_VOTE_1, QUPV3_WRAP_0_M_AHB_CLK_ENA, 1 );
  HWIO_OUTF( GCC_RPM_CLOCK_BRANCH_ENA_VOTE_1, QUPV3_WRAP0_CORE_CLK_ENA, 1 );
  HWIO_OUTF( GCC_RPM_CLOCK_BRANCH_ENA_VOTE_1, QUPV3_WRAP0_CORE_2X_CLK_ENA, 1 );
  HWIO_OUTF( GCC_RPM_CLOCK_BRANCH_ENA_VOTE_1, QUPV3_WRAP_1_S_AHB_CLK_ENA, 1 );
  HWIO_OUTF( GCC_RPM_CLOCK_BRANCH_ENA_VOTE_1, QUPV3_WRAP_1_M_AHB_CLK_ENA, 1 );
  HWIO_OUTF( GCC_RPM_CLOCK_BRANCH_ENA_VOTE_1, QUPV3_WRAP1_CORE_CLK_ENA, 1 );
  HWIO_OUTF( GCC_RPM_CLOCK_BRANCH_ENA_VOTE_1, QUPV3_WRAP1_CORE_2X_CLK_ENA, 1 );
  HWIO_OUTF( GCC_RPM_CLOCK_BRANCH_ENA_VOTE_2, QUPV3_WRAP_2_S_AHB_CLK_ENA, 1 );
  HWIO_OUTF( GCC_RPM_CLOCK_BRANCH_ENA_VOTE_2, QUPV3_WRAP_2_M_AHB_CLK_ENA, 1 );
  HWIO_OUTF( GCC_RPM_CLOCK_BRANCH_ENA_VOTE_2, QUPV3_WRAP2_CORE_CLK_ENA, 1 );
  HWIO_OUTF( GCC_RPM_CLOCK_BRANCH_ENA_VOTE_2, QUPV3_WRAP2_CORE_2X_CLK_ENA, 1 );

  if( ! Clock_InitPLLStatic()) return FALSE;

  if( ! Clock_InitCrypto(CLOCK_BOOT_PERF_DEFAULT) ) return FALSE;

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
  if( ! Clock_SetCPUPerfLevel(eCPUPerfLevel)) return FALSE;
  if( ! Clock_SetSysPerfLevel(eSysPerfLevel)) return FALSE;

  Clock_ReplaceUSBBootClockEnable();
  Clock_ReplaceUFSBootClockEnable();


  if(HWIO_INF(GCC_UFS_PHY_ICE_CORE_CBCR, CLK_OFF) == 1)
  {
    Clock_UFSInit( CLOCK_BOOT_PERF_DEFAULT );
  }

  if (Boot_Clock_MEMNOC_DCG == TRUE)
  {
    int i;
    for (i = 0; i < CLK_NUM_ELEM(Boot_Clock_MEMNOC_CBCRs); i++ )
    {
      /* With the software enables removed, only the hardware controlled enables
       * will remain to allow the clocks to turn off */
      outpdw(Boot_Clock_MEMNOC_CBCRs[i],
             inpdw(Boot_Clock_MEMNOC_CBCRs[i]) & ~0x1UL); // clear bit 0 (CLK_ENABLE)
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
  ClockConfigMuxType clkCfgSHRM = {HWIO_ADDR(GCC_SHRM_CMD_RCGR), 0, MUX_GCC, SRC_GPLL0, 4, 0, 0, 0}; // 300 MHz on V2

  Clock_SetClockConfig( &clkCfgDDR );
  Clock_ToggleClock(HWIO_GCC_DDRMC_CH0_CBCR_ADDR, CLK_TOGGLE_ENABLE);

  Clock_SetClockConfig( &clkCfgSHRM );

  Clock_PerfData.nDDR = 2; /* Initial DDR @ 200 MHz is performance level 2 */
  Clock_PerfData.nIPA = 3;

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
    railway_corner_vote(pClockRailway->EBIVoter, RAILWAY_NOMINAL);
    railway_transition_rails( );
	bClockRailwayInit = TRUE;

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
  if( HWIO_INI2(RPMH_ARC_DRVd_RMm_VOTE, 2, 8) < 0x5) {
    // Vote on behalf of DRV2 (HLOS) on ARC8 (MMCX) to level 2 (On)
    HWIO_OUTI2(RPMH_ARC_DRVd_RMm_VOTE, 2, 8, 0x5); 
  }

  /* If this Clock_ToggleClock is stuck, look at the AOP because the */
  /* MMCX ARC did not enable, so the AOP probably crashed on start up. */
  Clock_ToggleClock(HWIO_GCC_DISP_AHB_CBCR_ADDR, CLK_TOGGLE_ENABLE);
  /* Wait for SLEEP to enable.  It defaults on when the MMCX was enabled above. */
  while (HWIO_INF(DISP_CC_SLEEP_CBCR, CLK_OFF) == 1);

  Clock_ToggleClock(HWIO_DISP_CC_MDSS_RSCC_VSYNC_CBCR_ADDR, CLK_TOGGLE_ENABLE);
  Clock_ToggleClock(HWIO_DISP_CC_MDSS_RSCC_AHB_CBCR_ADDR, CLK_TOGGLE_ENABLE);


  /* CR 2147232 "Reset Disp MDSS RSCC" : This is temporary until the display driver
   can solve their ownership issues. */
  HWIO_OUT(DISP_CC_MDSS_RSCC_BCR, 0x1);
  while (HWIO_IN(DISP_CC_MDSS_RSCC_BCR) != 0x1); // DISP team recommends

  busywait(126); // DISP needs ~4 sleep clocks to reset 

  HWIO_OUT(DISP_CC_MDSS_RSCC_BCR, 0x0);
  while ( HWIO_IN(DISP_CC_MDSS_RSCC_BCR) != 0x0); // DISP team recommends
  busywait(1);

  /* Enable HW clock dividers based on activity. Put here to init MMSS related DCD */
  if (Boot_Clock_DCD == TRUE)
  {
    Clock_ToggleClock(HWIO_GCC_CAMERA_AHB_CBCR_ADDR, CLK_TOGGLE_ENABLE);
    Clock_ToggleClock(HWIO_GCC_VIDEO_AHB_CBCR_ADDR, CLK_TOGGLE_ENABLE);
    Clock_EnableDynClkDivide();
  }

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
  uint32 nVer = Clock_ChipVersion();
  Clock_ConfigType *cfg = Clock_Config();

  /* Don't switch performance level if it has been set previously */
  if ( eCurrLevel == eSysPerfLevel )
  {
    return TRUE;
  }
  
  if ( eSysPerfLevel >= CLOCK_BOOT_PERF_NUM ) return FALSE;
   /* This function is called from Clock_DebugInit, but by that time railway driver will not be ready..
   * bClockRailwayInit will be set to TRUE in the Clock_PreDDRInit function till that time No  rail-switching supported
   */
  if(eSysPerfLevel == CLOCK_BOOT_PERF_TURBO && bClockRailwayInit == FALSE) eSysPerfLevel = CLOCK_BOOT_PERF_NOMINAL;
  
  eCurrLevel = eSysPerfLevel;
  
  /* Switchh the Cx to the requested PerfLevel*/
  if( Clock_SwitchRail(CLOCK_RAIL_CX, cfg->aClockMapCorner[eSysPerfLevel], CLOCK_RAIL_CLIENT_BOOT) ==FALSE) return FALSE;


  /* Configure SNOC */
  Clock_PerfData.nSNOC = cfg->gcc_sys_noc_clk_src[eSysPerfLevel].nPerfLevel;
  Clock_SetClockConfig( &cfg->gcc_sys_noc_clk_src[eSysPerfLevel] );
  Clock_SetClockConfig( &cfg->gcc_sys_noc_gc_axi_clk_src[eSysPerfLevel] );
  Clock_SetClockConfig( &cfg->gcc_sys_noc_sf_axi_clk_src[eSysPerfLevel] );
  Clock_SetClockConfig( &cfg->gcc_sys_noc_monaq_axi_clk_src[eSysPerfLevel] );
  
  /* Aggre NOCs */
  Clock_SetClockConfig( &cfg->gcc_aggre_noc_clk_src[eSysPerfLevel] );
  Clock_SetClockConfig( &cfg->gcc_aggre_noc_south_sf_clk_src[eSysPerfLevel] );

  /* Configure CNOCs */
  Clock_PerfData.nCNOC = cfg->gcc_config_noc_clk_src[eSysPerfLevel].nPerfLevel;
  Clock_SetClockConfig( &cfg->gcc_config_noc_clk_src[eSysPerfLevel] );
  Clock_SetClockConfig( &cfg->gcc_cnoc_periph_clk_src[eSysPerfLevel] );

  /* Config SHUB */
  Clock_PerfData.nSHUB = cfg->gcc_memnoc_clk_src[nVer][eSysPerfLevel].nPerfLevel;
  Clock_SetClockConfig( &cfg->gcc_cpuss_axi_clk_src[eSysPerfLevel] );
  Clock_SetClockConfig( &cfg->gcc_gpu_memnoc_gfx_clk_src[nVer][eSysPerfLevel] );
  Clock_SetClockConfig( &cfg->gcc_memnoc_clk_src[nVer][eSysPerfLevel] );
  Clock_SetClockConfig( &cfg->gcc_mmu_tcu_clk_src[eSysPerfLevel] );
  Clock_SetClockConfig( &cfg->gcc_mss_q6_memnoc_axi_clk_src[eSysPerfLevel] );

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
  Clock_SetClockConfig( &cfg->gcc_ce1_clk_src[eSysPerfLevel] );
  Clock_PerfData.nCE = cfg->gcc_ce1_clk_src[eSysPerfLevel].nPerfLevel;

  mask =
    HWIO_FMSK(GCC_RPM_CLOCK_BRANCH_ENA_VOTE, CE1_CLK_ENA) |
    HWIO_FMSK(GCC_RPM_CLOCK_BRANCH_ENA_VOTE, CE1_AXI_CLK_ENA) |
    HWIO_FMSK(GCC_RPM_CLOCK_BRANCH_ENA_VOTE, CE1_AHB_CLK_ENA );
  HWIO_OUTM(GCC_RPM_CLOCK_BRANCH_ENA_VOTE, mask, mask);

  return TRUE;

} /* END Clock_InitCrypto */


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
  boolean aop_ret = Clock_ConfigAOP();

  return aop_ret;
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
  /* Be very careful changing settings on GPLL0.  It is enabled, and these changes should only be "safe" changes, as confirmed with the PLL design team. */
  HWIO_OUT(GCC_GPLL0_TEST_CTL_U1, TRION_TEST_CTL_U1_VAL );
  // Setup GPLL9 spread spectrum setting.
  HWIO_OUT(GCC_GPLL9_SSC_NUM_STEPS, 0xF);
  HWIO_OUT(GCC_GPLL9_SSC_UPDATE_RATE, 0xF);
  HWIO_OUT(GCC_GPLL9_SSC_DELTA_ALPHA, 8);

  return TRUE;

}

static void Clock_EnableDynClkDivide (void)
{
  int i;

  /* Initialize the registesr for DCD */
  for (i = 0; i < CLK_NUM_ELEM(Clock_DCD); i++ )
  {
    outpdw(Clock_DCD[i].address, Clock_DCD[i].value);
  }
  
  //if (V2 == Clock_ChipVersion())
  //{
  //  for (i = 0; i < CLK_NUM_ELEM(Clock_DCD_V2); i++ )
  //  {
  //    outpdw(Clock_DCD_V2[i].address, Clock_DCD_V2[i].value);
  //  }
  //}

}
