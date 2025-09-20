/*
==============================================================================

FILE:         ClockBSP.c

DESCRIPTION:
  This file contains the clock driver BSP data.

  Auto-generated from the following IPCat controller releases:

    SC8180Xv1 (poipu_1.0):
      PLL_HSR   : 9.0
      APSS_CC   : None
      CAM_CC    : cam_cc_2.1_p3q2r12.0 [poipu_cam_cc.r3]
      DISP_CC   : z_disp_cc_v2.0_p3q3r9.7 [hana_disp_cc.r5]
      GCC       : z_poipu_clk_ctl_p3q2r43 (2) [poipu_gcc.r15]
      GPU_CC    : z_gpu_cc_2.1_p3q0r8 [poipu_gpu_cc.r3]
      NPU_CC    : npu_1.0.0_p3q3r10.0.4644 [poipu_npu_cc.r2]
      SCC       : z_ssc_cc_hana_1.0_p3q2r11_s3 [poipu_scc.r5]
      VIDEO_CC  : video_cc_2.0_p3q3r4.9 [hana_video_cc.r5]

    SC8180Xv2 (poipu_2.0):
      PLL_HSR   : 9.0
      APSS_CC   : None
      CAM_CC    : cam_cc_2.3_p3q2r8.1 [poipu_cam_cc.r3]
      DISP_CC   : z_disp_cc_2.3_p3q3r1.4.1 [hana_disp_cc.r5]
      GCC       : Z_poipu_clk_ctl_V2.0_P3Q2R117.6 [poipu_v2_gcc.r6]
      GPU_CC    : z_gpu_cc_2.3_p3q0r5 [poipu_v2_gpu_cc.r2]
      NPU_CC    : npu_1.1.0_p3q3r9.0 [poipu_v2_npu_cc.r2]
      SCC       : z_ssc_cc_hana_2.0_p2q1r08_s7 [poipu_v2_scc.r3]
      VIDEO_CC  : video_cc_2.2_p3q3r1.31 [hana_v2_video_cc.r2]


  Generated using the following clockdrivergen settings:

CLOCKDRIVERGEN_CONFIG = \
{'db_override': {'APSS_CC': {'cbc': {'apcs_gold_post_acd_clk': {'d_sw_owner': {'APCS': 'RW'}},
                                     'apcs_l3_post_acd_clk': {'d_sw_owner': {'APCS': 'RW'}},
                                     'apcs_silver_post_acd_clk': {'d_sw_owner': {'APCS': 'RW'}}}},
                 'GCC': {'cbc': {'gcc_cxo2_tx1_clkref_en1': {'d_registers': {'cbcr_name': 'GCC_CXO2_TX1_CLKREF_EN1'},
                                                             'd_sw_owner': {'APCS': 'RW'},
                                                             'l_master': ['bi_tcxo']},
                                 'gcc_hlos1_vote_aggre_noc_mmu_audio_tbu_clk': {'d_registers': {'cbcr_name': 'GCC_HLOS1_VOTE_AGGRE_NOC_MMU_AUDIO_TBU_CLK'},
                                                                                'd_sw_owner': {'APCS': 'RW'},
                                                                                'l_master': ['bi_tcxo']},
                                 'gcc_hlos1_vote_aggre_noc_mmu_pcie_tbu_clk': {'d_registers': {'cbcr_name': 'GCC_HLOS1_VOTE_AGGRE_NOC_MMU_PCIE_TBU_CLK'},
                                                                               'd_sw_owner': {'APCS': 'RW'},
                                                                               'l_master': ['bi_tcxo']},
                                 'gcc_hlos1_vote_aggre_noc_mmu_tbu1_clk': {'d_registers': {'cbcr_name': 'GCC_HLOS1_VOTE_AGGRE_NOC_MMU_TBU1_CLK'},
                                                                           'd_sw_owner': {'APCS': 'RW'},
                                                                           'l_master': ['bi_tcxo']},
                                 'gcc_hlos1_vote_aggre_noc_mmu_tbu2_clk': {'d_registers': {'cbcr_name': 'GCC_HLOS1_VOTE_AGGRE_NOC_MMU_TBU2_CLK'},
                                                                           'd_sw_owner': {'APCS': 'RW'},
                                                                           'l_master': ['bi_tcxo']},
                                 'gcc_hlos1_vote_all_smmu_mmu_clk': {'d_registers': {'cbcr_name': 'GCC_HLOS1_VOTE_ALL_SMMU_MMU_CLK'},
                                                                     'd_sw_owner': {'APCS': 'RW'},
                                                                     'l_master': ['bi_tcxo']},
                                 'gcc_hlos1_vote_gpu_smmu_clk': {'d_registers': {'cbcr_name': 'GCC_HLOS1_VOTE_GPU_SMMU_CLK'},
                                                                 'd_sw_owner': {'APCS': 'RW'},
                                                                 'l_master': ['bi_tcxo']},
                                 'gcc_hlos1_vote_mmnoc_mmu_tbu_hf0_clk': {'d_registers': {'cbcr_name': 'GCC_HLOS1_VOTE_MMNOC_MMU_TBU_HF0_CLK'},
                                                                          'd_sw_owner': {'APCS': 'RW'},
                                                                          'l_master': ['bi_tcxo']},
                                 'gcc_hlos1_vote_mmnoc_mmu_tbu_hf1_clk': {'d_registers': {'cbcr_name': 'GCC_HLOS1_VOTE_MMNOC_MMU_TBU_HF1_CLK'},
                                                                          'd_sw_owner': {'APCS': 'RW'},
                                                                          'l_master': ['bi_tcxo']},
                                 'gcc_hlos1_vote_mmnoc_mmu_tbu_sf_clk': {'d_registers': {'cbcr_name': 'GCC_HLOS1_VOTE_MMNOC_MMU_TBU_SF_CLK'},
                                                                         'd_sw_owner': {'APCS': 'RW'},
                                                                         'l_master': ['bi_tcxo']},
                                 'gcc_hlos1_vote_mmu_tcu_clk': {'d_registers': {'cbcr_name': 'GCC_HLOS1_VOTE_MMU_TCU_CLK'},
                                                                'd_sw_owner': {'APCS': 'RW'},
                                                                'l_master': ['bi_tcxo']},
                                 'gcc_hlos1_vote_turing_mmu_tbu0_clk': {'d_registers': {'cbcr_name': 'GCC_HLOS1_VOTE_TURING_MMU_TBU0_CLK'},
                                                                        'd_sw_owner': {'APCS': 'RW'},
                                                                        'l_master': ['bi_tcxo']},
                                 'gcc_hlos1_vote_turing_mmu_tbu1_clk': {'d_registers': {'cbcr_name': 'GCC_HLOS1_VOTE_TURING_MMU_TBU1_CLK'},
                                                                        'd_sw_owner': {'APCS': 'RW'},
                                                                        'l_master': ['bi_tcxo']},
                                 'gcc_pcie_0_clkref_en': {'d_registers': {'cbcr_name': 'GCC_PCIE_0_CLKREF_EN'},
                                                          'd_sw_owner': {'APCS': 'RW'},
                                                          'l_master': ['bi_tcxo']},
                                 'gcc_pcie_1_clkref_en': {'d_registers': {'cbcr_name': 'GCC_PCIE_1_CLKREF_EN'},
                                                          'd_sw_owner': {'APCS': 'RW'},
                                                          'l_master': ['bi_tcxo']},
                                 'gcc_rx1_usb2_clkref_en': {'d_registers': {'cbcr_name': 'GCC_RX1_USB2_CLKREF_EN'},
                                                            'd_sw_owner': {'APCS': 'RW'},
                                                            'l_master': ['bi_tcxo']},
                                 'gcc_rx2_qlink_clkref_en': {'d_registers': {'cbcr_name': 'GCC_RX2_QLINK_CLKREF_EN'},
                                                             'd_sw_owner': {'APCS': 'RW'},
                                                             'l_master': ['bi_tcxo']},
                                 'gcc_rx3_clkref_en': {'d_registers': {'cbcr_name': 'GCC_RX3_CLKREF_EN'},
                                                       'd_sw_owner': {'APCS': 'RW'},
                                                       'l_master': ['bi_tcxo']},
                                 'gcc_ufs_card_clkref_en': {'d_registers': {'cbcr_name': 'GCC_UFS_CARD_CLKREF_EN'},
                                                            'd_sw_owner': {'APCS': 'RW'},
                                                            'l_master': ['bi_tcxo']},
                                 'gcc_ufs_mem_clkref_en': {'d_registers': {'cbcr_name': 'GCC_UFS_MEM_CLKREF_EN'},
                                                           'd_sw_owner': {'APCS': 'RW'},
                                                           'l_master': ['bi_tcxo']},
                                 'gcc_usb3_prim_clkref_en': {'d_registers': {'cbcr_name': 'GCC_USB3_PRIM_CLKREF_EN'},
                                                             'd_sw_owner': {'APCS': 'RW'},
                                                             'l_master': ['bi_tcxo']},
                                 'gcc_usb3_sec_clkref_en': {'d_registers': {'cbcr_name': 'GCC_USB3_SEC_CLKREF_EN'},
                                                            'd_sw_owner': {'APCS': 'RW'},
                                                            'l_master': ['bi_tcxo']}},
                         'sm_gdsc': {'gcc_hlos1_vote_aggre_noc_mmu_audio_tbu_gds': {'d_registers': {'gdscr_name': 'GCC_HLOS1_VOTE_AGGRE_NOC_MMU_AUDIO_TBU_GDS'},
                                                                                    'd_sw_owner': {'APCS': 'RW'}},
                                     'gcc_hlos1_vote_aggre_noc_mmu_pcie_tbu_gds': {'d_registers': {'gdscr_name': 'GCC_HLOS1_VOTE_AGGRE_NOC_MMU_PCIE_TBU_GDS'},
                                                                                   'd_sw_owner': {'APCS': 'RW'}},
                                     'gcc_hlos1_vote_aggre_noc_mmu_tbu1_gds': {'d_registers': {'gdscr_name': 'GCC_HLOS1_VOTE_AGGRE_NOC_MMU_TBU1_GDS'},
                                                                               'd_sw_owner': {'APCS': 'RW'}},
                                     'gcc_hlos1_vote_aggre_noc_mmu_tbu2_gds': {'d_registers': {'gdscr_name': 'GCC_HLOS1_VOTE_AGGRE_NOC_MMU_TBU2_GDS'},
                                                                               'd_sw_owner': {'APCS': 'RW'}},
                                     'gcc_hlos1_vote_all_smmu_mmu_gds': {'d_registers': {'gdscr_name': 'GCC_HLOS1_VOTE_ALL_SMMU_MMU_GDS'},
                                                                         'd_sw_owner': {'APCS': 'RW'}},
                                     'gcc_hlos1_vote_gpu_smmu_gds': {'d_registers': {'gdscr_name': 'GCC_HLOS1_VOTE_GPU_SMMU_GDS'},
                                                                     'd_sw_owner': {'APCS': 'RW'}},
                                     'gcc_hlos1_vote_mmnoc_mmu_tbu_hf0_gds': {'d_registers': {'gdscr_name': 'GCC_HLOS1_VOTE_MMNOC_MMU_TBU_HF0_GDS'},
                                                                              'd_sw_owner': {'APCS': 'RW'}},
                                     'gcc_hlos1_vote_mmnoc_mmu_tbu_hf1_gds': {'d_registers': {'gdscr_name': 'GCC_HLOS1_VOTE_MMNOC_MMU_TBU_HF1_GDS'},
                                                                              'd_sw_owner': {'APCS': 'RW'}},
                                     'gcc_hlos1_vote_mmnoc_mmu_tbu_sf_gds': {'d_registers': {'gdscr_name': 'GCC_HLOS1_VOTE_MMNOC_MMU_TBU_SF_GDS'},
                                                                             'd_sw_owner': {'APCS': 'RW'}},
                                     'gcc_hlos1_vote_mmu_tcu_gds': {'d_registers': {'gdscr_name': 'GCC_HLOS1_VOTE_MMU_TCU_GDS'},
                                                                    'd_sw_owner': {'APCS': 'RW'}},
                                     'gcc_hlos1_vote_turing_mmu_tbu0_gds': {'d_registers': {'gdscr_name': 'GCC_HLOS1_VOTE_TURING_MMU_TBU0_GDS'},
                                                                            'd_sw_owner': {'APCS': 'RW'}},
                                     'gcc_hlos1_vote_turing_mmu_tbu1_gds': {'d_registers': {'gdscr_name': 'GCC_HLOS1_VOTE_TURING_MMU_TBU1_GDS'},
                                                                            'd_sw_owner': {'APCS': 'RW'}}}}},
 'filter_warning': 'sw',
 'generators': [{'options': {'clock_flags': {'gcc_boot_rom_ahb_clk': ['SUPPRESSIBLE',
                                                                      'LEAVE_ALWAYS_ON'],
                                             'gcc_hlos1_vote_aggre_noc_mmu_audio_tbu_gds': ['IGNORE_OFF_STATUS'],
                                             'gcc_hlos1_vote_aggre_noc_mmu_pcie_tbu_gds': ['IGNORE_OFF_STATUS'],
                                             'gcc_hlos1_vote_aggre_noc_mmu_tbu1_gds': ['IGNORE_OFF_STATUS'],
                                             'gcc_hlos1_vote_aggre_noc_mmu_tbu2_gds': ['IGNORE_OFF_STATUS'],
                                             'gcc_hlos1_vote_all_smmu_mmu_gds': ['IGNORE_OFF_STATUS'],
                                             'gcc_hlos1_vote_gpu_smmu_gds': ['IGNORE_OFF_STATUS'],
                                             'gcc_hlos1_vote_mmnoc_mmu_tbu_hf0_gds': ['IGNORE_OFF_STATUS'],
                                             'gcc_hlos1_vote_mmnoc_mmu_tbu_hf1_gds': ['IGNORE_OFF_STATUS'],
                                             'gcc_hlos1_vote_mmnoc_mmu_tbu_sf_gds': ['IGNORE_OFF_STATUS'],
                                             'gcc_hlos1_vote_mmu_tcu_gds': ['IGNORE_OFF_STATUS'],
                                             'gcc_hlos1_vote_turing_mmu_tbu0_gds': ['IGNORE_OFF_STATUS'],
                                             'gcc_hlos1_vote_turing_mmu_tbu1_gds': ['IGNORE_OFF_STATUS'],
                                             'gcc_pcie_0_pipe_clk': ['IGNORE_ON_STATUS'],
                                             'gcc_pcie_1_pipe_clk': ['IGNORE_ON_STATUS'],
                                             'gcc_pcie_2_pipe_clk': ['IGNORE_ON_STATUS'],
                                             'gcc_pcie_3_pipe_clk': ['IGNORE_ON_STATUS'],
                                             'gcc_qupv3_wrap0_s0_clk_src': ['DOMAIN_WAIT_FOR_ROOT_OFF'],
                                             'gcc_qupv3_wrap0_s1_clk_src': ['DOMAIN_WAIT_FOR_ROOT_OFF'],
                                             'gcc_qupv3_wrap0_s2_clk_src': ['DOMAIN_WAIT_FOR_ROOT_OFF'],
                                             'gcc_qupv3_wrap0_s3_clk_src': ['DOMAIN_WAIT_FOR_ROOT_OFF'],
                                             'gcc_qupv3_wrap0_s4_clk_src': ['DOMAIN_WAIT_FOR_ROOT_OFF'],
                                             'gcc_qupv3_wrap0_s5_clk_src': ['DOMAIN_WAIT_FOR_ROOT_OFF'],
                                             'gcc_qupv3_wrap0_s6_clk_src': ['DOMAIN_WAIT_FOR_ROOT_OFF'],
                                             'gcc_qupv3_wrap0_s7_clk_src': ['DOMAIN_WAIT_FOR_ROOT_OFF'],
                                             'gcc_qupv3_wrap1_s0_clk_src': ['DOMAIN_WAIT_FOR_ROOT_OFF'],
                                             'gcc_qupv3_wrap1_s1_clk_src': ['DOMAIN_WAIT_FOR_ROOT_OFF'],
                                             'gcc_qupv3_wrap1_s2_clk_src': ['DOMAIN_WAIT_FOR_ROOT_OFF'],
                                             'gcc_qupv3_wrap1_s3_clk_src': ['DOMAIN_WAIT_FOR_ROOT_OFF'],
                                             'gcc_qupv3_wrap1_s4_clk_src': ['DOMAIN_WAIT_FOR_ROOT_OFF'],
                                             'gcc_qupv3_wrap1_s5_clk_src': ['DOMAIN_WAIT_FOR_ROOT_OFF'],
                                             'gcc_qupv3_wrap1_s6_clk_src': ['DOMAIN_WAIT_FOR_ROOT_OFF'],
                                             'gcc_qupv3_wrap1_s7_clk_src': ['DOMAIN_WAIT_FOR_ROOT_OFF'],
                                             'gcc_qupv3_wrap2_s0_clk_src': ['DOMAIN_WAIT_FOR_ROOT_OFF'],
                                             'gcc_qupv3_wrap2_s1_clk_src': ['DOMAIN_WAIT_FOR_ROOT_OFF'],
                                             'gcc_qupv3_wrap2_s2_clk_src': ['DOMAIN_WAIT_FOR_ROOT_OFF'],
                                             'gcc_qupv3_wrap2_s3_clk_src': ['DOMAIN_WAIT_FOR_ROOT_OFF'],
                                             'gcc_qupv3_wrap2_s4_clk_src': ['DOMAIN_WAIT_FOR_ROOT_OFF'],
                                             'gcc_qupv3_wrap2_s5_clk_src': ['DOMAIN_WAIT_FOR_ROOT_OFF'],
                                             'gcc_qupv3_wrap2_s6_clk_src': ['DOMAIN_WAIT_FOR_ROOT_OFF'],
                                             'gcc_qupv3_wrap2_s7_clk_src': ['DOMAIN_WAIT_FOR_ROOT_OFF'],
                                             'gcc_sys_noc_cpuss_ahb_clk': ['SUPPRESSIBLE',
                                                                           'LEAVE_ALWAYS_ON'],
                                             'gcc_ufs_phy_rx_symbol_0_clk': ['IGNORE_ON_STATUS'],
                                             'gcc_ufs_phy_rx_symbol_1_clk': ['IGNORE_ON_STATUS'],
                                             'gcc_ufs_phy_tx_symbol_0_clk': ['IGNORE_ON_STATUS'],
                                             'gcc_usb3_mp_phy_pipe_0_clk': ['IGNORE_ON_STATUS'],
                                             'gcc_usb3_prim_phy_pipe_clk': ['IGNORE_ON_STATUS'],
                                             'gcc_usb3_sec_phy_pipe_clk': ['IGNORE_ON_STATUS'],
                                             'usb30_prim_gdsc': ['SUPPRESSIBLE'],
                                             'usb30_sec_gdsc': ['SUPPRESSIBLE']},
                             'clock_flags_hal': {'disp_cc_mdss_mdp_clk_src': ['DOMAIN_HW_CLK_CONTROL']},
                             'driver_flags': ['LOG_CLOCK_FREQ_CHANGE',
                                              'LOG_SOURCE_STATE_CHANGE'],
                             'env': ['apcs', 'gpuss', 'APCS_AUTO'],
                             'hal_ctl_structs': {},
                             'include_file': {'HALclkPLLSettings.h': True},
                             'include_fmax': ['.*'],
                             'include_source_map': ['.*'],
                             'map_bases': True,
                             'npa_rename': {'/pmic/client/xo': '/arc/client/rail_xo',
                                            '/vcs/vdd_cx': '/arc/client/rail_cx',
                                            '/vcs/vdd_mm': '/arc/client/rail_mmcx',
                                            '/vcs/vdd_mx': '/arc/client/rail_mx'},
                             'standalone_bcrs': ['.*pcie.*', '.*usb.*']},
                 'output_dir': 'scx8180_output_dir',
                 'type': 'dalclock'}],
 'target': ['poipu']}

==============================================================================

$Header: //components/rel/boot.xf/3.0.1.c1/QcomPkg/SDMPkg/1000/Settings/Clock/core/ClockBSP.c#1 $

==============================================================================
            Copyright (c) 2020 QUALCOMM Technologies Incorporated.
                    All Rights Reserved.
                  QUALCOMM Proprietary/GTDR
==============================================================================

*/


/*=========================================================================
      Include Files
==========================================================================*/

#include "Drivers/ClockDxe/ClockBSP.h"
#include "../../Library/HALclkLib/HALclkHWIO.h"
#include "HALhwio.h"


/*=========================================================================
      Externs
==========================================================================*/

extern HAL_clk_ClockControlType           HAL_clk_GenericClockControl;
extern HAL_clk_ClockDomainControlType     HAL_clk_GenericDomainControl;
extern HAL_clk_ClockDomainControlType     HAL_clk_GenericDomainMuxControl;
extern HAL_clk_DividerControlType         HAL_clk_GenericDividerControl;
extern HAL_clk_PowerDomainControlType     HAL_clk_GenericPowerDomainControl;
extern HAL_clk_SourceControlType          HAL_clk_RegeraPLLControl;
extern HAL_clk_SourceControlType          HAL_clk_TrionPLLControl;

/*
 * Debug mux control structures.
 */
extern HAL_clk_DebugMuxControlType        HAL_clk_DebugMuxControlAPSSCC;
extern HAL_clk_DebugMuxControlType        HAL_clk_DebugMuxControlCAMCC;
extern HAL_clk_DebugMuxControlType        HAL_clk_DebugMuxControlDISPCC;
extern HAL_clk_DebugMuxControlType        HAL_clk_DebugMuxControlGCC;
extern HAL_clk_DebugMuxControlType        HAL_clk_DebugMuxControlGPUCC;
extern HAL_clk_DebugMuxControlType        HAL_clk_DebugMuxControlNPUCC;
extern HAL_clk_DebugMuxControlType        HAL_clk_DebugMuxControlSCC;
extern HAL_clk_DebugMuxControlType        HAL_clk_DebugMuxControlVIDEOCC;


/*=========================================================================
      Data Declarations
==========================================================================*/


/*=========================================================================
      Bases
==========================================================================*/

uintnt HAL_clk_nHWIOBaseAPSSHMPOIPU;
uintnt HAL_clk_nHWIOBaseCLKCTL;
uintnt HAL_clk_nHWIOBaseGFX;
uintnt HAL_clk_nHWIOBaseMDSS;
uintnt HAL_clk_nHWIOBaseNPU;
uintnt HAL_clk_nHWIOBaseSSC;
uintnt HAL_clk_nHWIOBaseTITANSS;
uintnt HAL_clk_nHWIOBaseVIDEOSSWRAPPER;

static ClockBaseType aBases[] =
{
  { APSS_HM_POIPU_BASE_PHYS,  APSS_HM_POIPU_BASE_SIZE,  &HAL_clk_nHWIOBaseAPSSHMPOIPU  },
  { CLK_CTL_BASE_PHYS,        CLK_CTL_BASE_SIZE,        &HAL_clk_nHWIOBaseCLKCTL       },
  { GFX_BASE_PHYS,            GFX_BASE_SIZE,            &HAL_clk_nHWIOBaseGFX          },
  { MDSS_BASE_PHYS,           MDSS_BASE_SIZE,           &HAL_clk_nHWIOBaseMDSS         },
  { NPU_BASE_PHYS,            NPU_BASE_SIZE,            &HAL_clk_nHWIOBaseNPU          },
  { SSC_BASE_PHYS,            SSC_BASE_SIZE,            &HAL_clk_nHWIOBaseSSC          },
  { TITAN_SS_BASE_PHYS,       TITAN_SS_BASE_SIZE,       &HAL_clk_nHWIOBaseTITANSS      },
  { VIDEO_SS_WRAPPER_BASE_PHYS, VIDEO_SS_WRAPPER_BASE_SIZE, &HAL_clk_nHWIOBaseVIDEOSSWRAPPER },
};

/*=========================================================================
      Voltage Rails
==========================================================================*/

static ClockRailType ClockRail_VDDCX =
{
  .szName = "/arc/client/rail_cx",
};

static ClockRailType ClockRail_VDDGX =
{
  .szName = "/vcs/vdd_gx",
};

static ClockRailType ClockRail_VDDMM =
{
  .szName = "/arc/client/rail_mmcx",
};

static ClockRailType ClockRail_VDDMX =
{
  .szName = "/arc/client/rail_mx",
};

#ifdef ENABLE_AUTO_PLAT
	static ClockRailType ClockRail_VDDSSCINT =
	{
	.szName = "/vcs/vdd_ssc_int",
	};
#endif
static ClockRailType *aRails[] =
{
  &ClockRail_VDDCX,
  &ClockRail_VDDGX,
  &ClockRail_VDDMM,
  &ClockRail_VDDMX,
#ifdef ENABLE_AUTO_PLAT
  &ClockRail_VDDSSCINT,
#endif
};

/*=========================================================================
      Debug Muxes
==========================================================================*/

static ClockDebugMuxType ClockDebugMux_GCC =
{
  .szName       = "gcc",
  .nDivider     = 2,
  .HALDebugMux  = {
    .pmControl  = &HAL_clk_DebugMuxControlGCC,
  },
};

static ClockDebugMuxInputDivType aMuxInputDividers_APSSCC[] =
{
  { 33, 4 },
  { 37, 8 },
  { 65, 4 },
  { 68, 16 },
  { 69, 16 },
  { 70, 16 },
  { CLOCK_DEBUG_MUX_INPUT_DIV_END },
};

static ClockDebugMuxType ClockDebugMux_APSSCC =
{
  .szName       = "apss_cc",
  .pParent      = &ClockDebugMux_GCC,
  .nParentSel   = 214,
  .anInputDivs  = aMuxInputDividers_APSSCC,
  .HALDebugMux  = {
    .pmControl  = &HAL_clk_DebugMuxControlAPSSCC,
  },
};

static ClockDebugMuxType ClockDebugMux_CAMCC =
{
  .szName       = "cam_cc",
  .pParent      = &ClockDebugMux_GCC,
  .nParentSel   = 70,
  .nDivider     = 4,
  .HALDebugMux  = {
    .pmControl  = &HAL_clk_DebugMuxControlCAMCC,
  },
};

static ClockDebugMuxType ClockDebugMux_DISPCC =
{
  .szName       = "disp_cc",
  .pParent      = &ClockDebugMux_GCC,
  .nParentSel   = 71,
  .nDivider     = 4,
  .HALDebugMux  = {
    .pmControl  = &HAL_clk_DebugMuxControlDISPCC,
  },
};

static ClockDebugMuxType ClockDebugMux_GPUCC =
{
  .szName       = "gpu_cc",
  .pParent      = &ClockDebugMux_GCC,
  .nParentSel   = 324,
  .nDivider     = 2,
  .HALDebugMux  = {
    .pmControl  = &HAL_clk_DebugMuxControlGPUCC,
  },
};

static ClockDebugMuxType ClockDebugMux_NPUCC =
{
  .szName       = "npu_cc",
  .pParent      = &ClockDebugMux_GCC,
  .nParentSel   = 454,
  .nDivider     = 2,
  .HALDebugMux  = {
    .pmControl  = &HAL_clk_DebugMuxControlNPUCC,
  },
};

static ClockDebugMuxType ClockDebugMux_SCC =
{
  .szName       = "scc",
  .pParent      = &ClockDebugMux_GCC,
  .nParentSel   = 267,
  .HALDebugMux  = {
    .pmControl  = &HAL_clk_DebugMuxControlSCC,
  },
};

static ClockDebugMuxType ClockDebugMux_VIDEOCC =
{
  .szName       = "video_cc",
  .pParent      = &ClockDebugMux_GCC,
  .nParentSel   = 72,
  .nDivider     = 5,
  .HALDebugMux  = {
    .pmControl  = &HAL_clk_DebugMuxControlVIDEOCC,
  },
};

static ClockDebugMuxType *aDebugMuxes[] =
{
  &ClockDebugMux_APSSCC,
  &ClockDebugMux_CAMCC,
  &ClockDebugMux_DISPCC,
  &ClockDebugMux_GCC,
  &ClockDebugMux_GPUCC,
  &ClockDebugMux_NPUCC,
  &ClockDebugMux_SCC,
  &ClockDebugMux_VIDEOCC,
};


/*=========================================================================
      Primary Sources
==========================================================================*/

static ClockSourceNodeType ClockSource_XO =
{
  .szName    = "/arc/client/rail_xo",
  .nFlags    = 0x0,
};


/*=========================================================================
      Source Register Settings
==========================================================================*/

static const ClockSourceRegSettingsType ClockSourceRegSettings_CAMCC_CAMCCPLL0[] =
{
  {
    .HALSettings = {
      .nConfigCtl   = 0x20485699,
      .nConfigCtl1  = 0x00002267,
      .nConfigCtl2  = 0x00000024,
      .nTestCtl     = 0x00000000,
      .nTestCtl1    = 0x00000002,
      .nTestCtl2    = 0x00000020,
      .nUserCtl     = 0x00000000,
      .nUserCtl1    = 0x00000805,
      .nUserCtl2    = 0x000000D0,
    },
    .HWVersion = {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X},
  },
  {
    .HALSettings = {
      .nConfigCtl   = 0x20485699,
      .nConfigCtl1  = 0x00002267,
      .nConfigCtl2  = 0x00000024,
      .nTestCtl     = 0x00000000,
      .nTestCtl1    = 0x00000000,
      .nTestCtl2    = 0x00000020,
      .nUserCtl     = 0x00000000,
      .nUserCtl1    = 0x00000805,
      .nUserCtl2    = 0x000000D0,
    },
    .HWVersion = {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X},
  },
};

static const ClockSourceRegSettingsType ClockSourceRegSettings_CAMCC_CAMCCPLL2[] =
{
  {
    .HALSettings = {
      .nConfigCtl   = 0x10000927,
      .nConfigCtl1  = 0x00000011,
      .nConfigCtl2  = 0x04300142,
      .nTestCtl     = 0x04000400,
      .nTestCtl1    = 0x00004000,
      .nTestCtl2    = 0x00000000,
      .nUserCtl     = 0x00000000,
      .nUserCtl1    = 0x00000000,
      .nUserCtl2    = 0x00000000,
    },
    .HWVersion = {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X},
  },
  {
    .HALSettings = {
      .nConfigCtl   = 0x10000807,
      .nConfigCtl1  = 0x00000011,
      .nConfigCtl2  = 0x04300142,
      .nTestCtl     = 0x04000400,
      .nTestCtl1    = 0x00004000,
      .nTestCtl2    = 0x00000000,
      .nUserCtl     = 0x00000000,
      .nUserCtl1    = 0x00000000,
      .nUserCtl2    = 0x00000000,
    },
    .HWVersion = {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X},
  },
};


/*=========================================================================
      Source BSP
==========================================================================*/

/*
 * ClockSourceBSP_CAMCC_CAMCCPLL0
 *
 * Used by:
 *   ClockSource_CAMCC_CAMCCPLL0
 */
static const ClockSourceFreqConfigType ClockSourceBSP_CAMCC_CAMCCPLL0[] =
{
  { 1200000000, &ClockSource_XO, { 0, 0, 1, 62,  0x00008000, HAL_CLK_PLL_FRAC_MODE_ALPHA, {.Trion.nEven = 2, .Trion.nOdd = 3} }, RAIL_VOLTAGE_LEVEL_SVS_L1,    },
};
static const ClockSourceFreqConfigType ClockSourceBSPCal_CAMCC_CAMCCPLL0[] =
{
  { 1305600000, &ClockSource_XO, { 0, 0, 1, 68,  0x00000000, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                      }, RAIL_VOLTAGE_LEVEL_SVS_L1,    },
};

/*
 * ClockSourceBSP_CAMCC_CAMCCPLL1
 *
 * Used by:
 *   ClockSource_CAMCC_CAMCCPLL1
 */
static const ClockSourceFreqConfigType ClockSourceBSP_CAMCC_CAMCCPLL1[] =
{
  {  375000000, &ClockSource_XO, { 0, 0, 1, 19,  0x00008800, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                      }, RAIL_VOLTAGE_LEVEL_MIN_SVS,   },
  {  475000000, &ClockSource_XO, { 0, 0, 1, 24,  0x0000BD55, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                      }, RAIL_VOLTAGE_LEVEL_MIN_SVS,   },
  {  520000000, &ClockSource_XO, { 0, 0, 1, 27,  0x00001555, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                      }, RAIL_VOLTAGE_LEVEL_MIN_SVS,   },
  {  600000000, &ClockSource_XO, { 0, 0, 1, 31,  0x00004000, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                      }, RAIL_VOLTAGE_LEVEL_MIN_SVS,   },
};

/*
 * ClockSourceBSP_CAMCC_CAMCCPLL2
 *
 * Used by:
 *   ClockSource_CAMCC_CAMCCPLL2
 */
static const ClockSourceFreqConfigType ClockSourceBSP_CAMCC_CAMCCPLL2[] =
{
  {  960000000, &ClockSource_XO, { 0, 0, 1, 50,  0x00000000, HAL_CLK_PLL_FRAC_MODE_ALPHA, {.Regera.nMain = 2}          }, RAIL_VOLTAGE_LEVEL_MIN_SVS,   },
};

/*
 * ClockSourceBSP_CAMCC_CAMCCPLL3
 *
 * Used by:
 *   ClockSource_CAMCC_CAMCCPLL3
 *   ClockSource_CAMCC_CAMCCPLL4
 *   ClockSource_CAMCC_CAMCCPLL5
 *   ClockSource_CAMCC_CAMCCPLL6
 */
static const ClockSourceFreqConfigType ClockSourceBSP_CAMCC_CAMCCPLL3[] =
{
  {  400000000, &ClockSource_XO, { 0, 0, 1, 20,  0x0000D555, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                      }, RAIL_VOLTAGE_LEVEL_MIN_SVS,   },
  {  558000000, &ClockSource_XO, { 0, 0, 1, 29,  0x00001000, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                      }, RAIL_VOLTAGE_LEVEL_MIN_SVS,   },
  {  637000000, &ClockSource_XO, { 0, 0, 1, 33,  0x00002D55, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                      }, RAIL_VOLTAGE_LEVEL_SVS,       },
  {  760000000, &ClockSource_XO, { 0, 0, 1, 39,  0x00009555, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                      }, RAIL_VOLTAGE_LEVEL_SVS,       },
};

/*
 * ClockSourceBSP_DISPCC_DISPCCPLL0
 *
 * Used by:
 *   ClockSource_DISPCC_DISPCCPLL0
 */
static const ClockSourceFreqConfigType ClockSourceBSP_DISPCC_DISPCCPLL0[] =
{
  { 1380000000, &ClockSource_XO, { 0, 0, 1, 71,  0x0000E000, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                      }, RAIL_VOLTAGE_LEVEL_SVS_L1,    },
};

/*
 * ClockSourceBSP_DISPCC_DISPCCPLL1
 *
 * Used by:
 *   ClockSource_DISPCC_DISPCCPLL1
 */
static const ClockSourceFreqConfigType ClockSourceBSP_DISPCC_DISPCCPLL1[] =
{
  {  600000000, &ClockSource_XO, { 0, 0, 1, 31,  0x00004000, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                      }, RAIL_VOLTAGE_LEVEL_MIN_SVS,   },
};

/*
 * ClockSourceBSP_GCC_GPLL0
 *
 * Used by:
 */
static const ClockSourceFreqConfigType ClockSourceBSP_GCC_GPLL0[] =
{
  {  600000000, &ClockSource_XO, { 0, 0, 1, 31,  0x00004000, HAL_CLK_PLL_FRAC_MODE_ALPHA, {.Trion.nEven = 2}           }, RAIL_VOLTAGE_LEVEL_MIN_SVS,   },
};

/*
 * ClockSourceBSP_GCC_GPLL7
 *
 * Used by:
 */
static const ClockSourceFreqConfigType ClockSourceBSP_GCC_GPLL7[] =
{
  {  500000000, &ClockSource_XO, { 0, 0, 1, 26,  0x00000AAA, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                      }, RAIL_VOLTAGE_LEVEL_MIN_SVS,   },
};

/*
 * ClockSourceBSP_GCC_GPLL9
 *
 * Used by:
 */
static const ClockSourceFreqConfigType ClockSourceBSP_GCC_GPLL9[] =
{
  {  808000000, &ClockSource_XO, { 0, 0, 1, 42,  0x00001555, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                      }, RAIL_VOLTAGE_LEVEL_SVS,       {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}},
};
static const ClockSourceFreqConfigType ClockSourceBSPCal_GCC_GPLL9[] =
{
  { 1305600000, &ClockSource_XO, { 0, 0, 1, 68,  0x00000000, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                      }, RAIL_VOLTAGE_LEVEL_SVS_L1,    {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}},
};

/*
 * ClockSourceBSP_GPUCC_GPUCCPLL0
 *
 * Used by:
 *   ClockSource_GPUCC_GPUCCPLL0
 */
static const ClockSourceFreqConfigType ClockSourceBSP_GPUCC_GPUCCPLL0[] =
{
  {  312000000, &ClockSource_XO, { 0, 0, 1, 16,  0x00004000, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                      }, RAIL_VOLTAGE_LEVEL_MIN_SVS,   {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}},
  {  354000000, &ClockSource_XO, { 0, 0, 1, 18,  0x00007000, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                      }, RAIL_VOLTAGE_LEVEL_MIN_SVS,   {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}},
  {  464000000, &ClockSource_XO, { 0, 0, 1, 24,  0x00002AAA, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                      }, RAIL_VOLTAGE_LEVEL_MIN_SVS,   {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}},
  {  470000000, &ClockSource_XO, { 0, 0, 1, 24,  0x00007AAA, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                      }, RAIL_VOLTAGE_LEVEL_MIN_SVS,   {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}},
  {  512000000, &ClockSource_XO, { 0, 0, 1, 26,  0x0000AAAA, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                      }, RAIL_VOLTAGE_LEVEL_MIN_SVS,   {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}},
  {  630000000, &ClockSource_XO, { 0, 0, 1, 32,  0x0000D000, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                      }, RAIL_VOLTAGE_LEVEL_SVS,       },
  {  784000000, &ClockSource_XO, { 0, 0, 1, 40,  0x0000D555, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                      }, RAIL_VOLTAGE_LEVEL_SVS,       {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}},
  {  810000000, &ClockSource_XO, { 0, 0, 1, 42,  0x00003000, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                      }, RAIL_VOLTAGE_LEVEL_SVS,       {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}},
  {  922000000, &ClockSource_XO, { 0, 0, 1, 48,  0x00000555, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                      }, RAIL_VOLTAGE_LEVEL_SVS,       {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}},
  { 1000000000, &ClockSource_XO, { 0, 0, 1, 52,  0x00001555, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                      }, RAIL_VOLTAGE_LEVEL_SVS,       {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}},
  { 1028000000, &ClockSource_XO, { 0, 0, 1, 53,  0x00008AAA, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                      }, RAIL_VOLTAGE_LEVEL_SVS,       {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}},
  { 1060000000, &ClockSource_XO, { 0, 0, 1, 55,  0x00003555, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                      }, RAIL_VOLTAGE_LEVEL_SVS,       {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}},
  { 1190000000, &ClockSource_XO, { 0, 0, 1, 61,  0x0000FAAA, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                      }, RAIL_VOLTAGE_LEVEL_SVS_L1,    {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}},
  { 1250000000, &ClockSource_XO, { 0, 0, 1, 65,  0x00001AAA, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                      }, RAIL_VOLTAGE_LEVEL_SVS_L1,    {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}},
  { 1340000000, &ClockSource_XO, { 0, 0, 1, 69,  0x0000CAAA, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                      }, RAIL_VOLTAGE_LEVEL_SVS_L1,    {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}},
};

/*
 * ClockSourceBSP_GPUCC_GPUCCPLL1
 *
 * Used by:
 *   ClockSource_GPUCC_GPUCCPLL1
 */
static const ClockSourceFreqConfigType ClockSourceBSP_GPUCC_GPUCCPLL1[] =
{
  {  500000000, &ClockSource_XO, { 0, 0, 1, 26,  0x00000AAA, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                      }, RAIL_VOLTAGE_LEVEL_MIN_SVS,   },
};

/*
 * ClockSourceBSP_NPUCC_NPUCCPLL0
 *
 * Used by:
 *   ClockSource_NPUCC_NPUCCPLL0
 */
static const ClockSourceFreqConfigType ClockSourceBSP_NPUCC_NPUCCPLL0[] =
{
  {  250000000, &ClockSource_XO, { 0, 0, 1, 13,  0x00000555, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                      }, RAIL_VOLTAGE_LEVEL_MIN_SVS,   {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}},
  {  600000000, &ClockSource_XO, { 0, 0, 1, 31,  0x00004000, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                      }, RAIL_VOLTAGE_LEVEL_MIN_SVS,   },
  {  700000000, &ClockSource_XO, { 0, 0, 1, 36,  0x00007555, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                      }, RAIL_VOLTAGE_LEVEL_SVS,       {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}},
  {  800000000, &ClockSource_XO, { 0, 0, 1, 41,  0x0000AAAA, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                      }, RAIL_VOLTAGE_LEVEL_SVS,       },
  {  974000000, &ClockSource_XO, { 0, 0, 1, 50,  0x0000BAAA, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                      }, RAIL_VOLTAGE_LEVEL_SVS,       {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}},
  { 1200000000, &ClockSource_XO, { 0, 0, 1, 62,  0x00008000, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                      }, RAIL_VOLTAGE_LEVEL_SVS_L1,    {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}},
  { 1304000000, &ClockSource_XO, { 0, 0, 1, 67,  0x0000EAAA, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                      }, RAIL_VOLTAGE_LEVEL_SVS_L1,    {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}},
  { 1430000000, &ClockSource_XO, { 0, 0, 1, 74,  0x00007AAA, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                      }, RAIL_VOLTAGE_LEVEL_SVS_L1,    {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}},
  { 1546000000, &ClockSource_XO, { 0, 0, 1, 80,  0x00008555, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                      }, RAIL_VOLTAGE_LEVEL_SVS_L1,    {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}},
  { 1622000000, &ClockSource_XO, { 0, 0, 1, 84,  0x00007AAA, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                      }, RAIL_VOLTAGE_LEVEL_NOM,       {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}},
  { 1816000000, &ClockSource_XO, { 0, 0, 1, 94,  0x00009555, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                      }, RAIL_VOLTAGE_LEVEL_NOM,       {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}},
};

/*
 * ClockSourceBSP_NPUCC_NPUCCPLL1
 *
 * Used by:
 *   ClockSource_NPUCC_NPUCCPLL1
 */
static const ClockSourceFreqConfigType ClockSourceBSP_NPUCC_NPUCCPLL1[] =
{
  {  700000000, &ClockSource_XO, { 0, 0, 1, 36,  0x00007555, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                      }, RAIL_VOLTAGE_LEVEL_SVS,       {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}},
  {  800000000, &ClockSource_XO, { 0, 0, 1, 41,  0x0000AAAA, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                      }, RAIL_VOLTAGE_LEVEL_SVS,       {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}},
};
#ifdef ENABLE_AUTO_PLAT
/*
 * ClockSourceBSP_SCC_SCCPLL
 *
 * Used by:
 *   ClockSource_SCC_SCCPLL
 */
static const ClockSourceFreqConfigType ClockSourceBSP_SCC_SCCPLL[] =
{
  {  576000000, &ClockSource_XO, { 0, 0, 1, 30,  0x00000000, HAL_CLK_PLL_FRAC_MODE_ALPHA, {.Trion.nEven = 2}           }, RAIL_VOLTAGE_LEVEL_MIN_SVS,   {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}},
  {  600000000, &ClockSource_XO, { 0, 0, 1, 31,  0x00004000, HAL_CLK_PLL_FRAC_MODE_ALPHA, {.Trion.nEven = 2}           }, RAIL_VOLTAGE_LEVEL_MIN_SVS,   {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}},
};
#endif
/*
 * ClockSourceBSP_VIDEOCC_VIDEOPLL0
 *
 * Used by:
 *   ClockSource_VIDEOCC_VIDEOPLL0
 */
static const ClockSourceFreqConfigType ClockSourceBSP_VIDEOCC_VIDEOPLL0[] =
{
  {  400000000, &ClockSource_XO, { 0, 0, 1, 20,  0x0000D555, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                      }, RAIL_VOLTAGE_LEVEL_MIN_SVS,   },
  {  450000000, &ClockSource_XO, { 0, 0, 1, 23,  0x00007000, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                      }, RAIL_VOLTAGE_LEVEL_MIN_SVS,   {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}},
  {  480000000, &ClockSource_XO, { 0, 0, 1, 25,  0x00000000, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                      }, RAIL_VOLTAGE_LEVEL_MIN_SVS,   {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}},
  {  600000000, &ClockSource_XO, { 0, 0, 1, 31,  0x00004000, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                      }, RAIL_VOLTAGE_LEVEL_MIN_SVS,   {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}},
  {  676000000, &ClockSource_XO, { 0, 0, 1, 35,  0x00003555, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                      }, RAIL_VOLTAGE_LEVEL_SVS,       {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}},
  {  730000000, &ClockSource_XO, { 0, 0, 1, 38,  0x00000555, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                      }, RAIL_VOLTAGE_LEVEL_SVS,       },
  {  864000000, &ClockSource_XO, { 0, 0, 1, 45,  0x00000000, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                      }, RAIL_VOLTAGE_LEVEL_SVS,       {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}},
  {  888000000, &ClockSource_XO, { 0, 0, 1, 46,  0x00004000, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                      }, RAIL_VOLTAGE_LEVEL_SVS,       {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}},
  {  960000000, &ClockSource_XO, { 0, 0, 1, 50,  0x00000000, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                      }, RAIL_VOLTAGE_LEVEL_SVS,       {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}},
  { 1066000000, &ClockSource_XO, { 0, 0, 1, 55,  0x00008555, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                      }, RAIL_VOLTAGE_LEVEL_SVS,       {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}},
};


/*=========================================================================
      Sources
==========================================================================*/

static ClockSourceNodeType ClockSource_CAMCC_CAMCCPLL0 =
{
  .szName       = "cam_cc_pll0",
  .nFlags       = CLOCK_FLAG_INTERNAL_CONTROL,
  .aBSP         = ClockSourceBSP_CAMCC_CAMCCPLL0,
  .nBSPLen      = SIZEOF_ARRAY(ClockSourceBSP_CAMCC_CAMCCPLL0),
  .pRail        = &ClockRail_VDDMX,
  .nConfigMask  = CLOCK_CONFIG_PLL_EVEN_OUTPUT_ENABLE |
                  CLOCK_CONFIG_PLL_ODD_OUTPUT_ENABLE,
  .eDisableMode = HAL_CLK_SOURCE_DISABLE_MODE_NORMAL,
  .HALSource = {
    .nAddr          = HWIO_PHYS(CAM_CC_PLL0_MODE),
    .pmControl      = &HAL_clk_TrionPLLControl,
  },
  .pCalibrationFreqConfig     = ClockSourceBSPCal_CAMCC_CAMCCPLL0,
  .nCalibrationFreqConfigLen  = SIZEOF_ARRAY(ClockSourceBSPCal_CAMCC_CAMCCPLL0),
  .pRegSettings               = ClockSourceRegSettings_CAMCC_CAMCCPLL0,
  .nRegSettingsLen            = SIZEOF_ARRAY(ClockSourceRegSettings_CAMCC_CAMCCPLL0),
};

static ClockSourceNodeType ClockSource_CAMCC_CAMCCPLL1 =
{
  .szName       = "cam_cc_pll1",
  .nFlags       = CLOCK_FLAG_INTERNAL_CONTROL,
  .aBSP         = ClockSourceBSP_CAMCC_CAMCCPLL1,
  .nBSPLen      = SIZEOF_ARRAY(ClockSourceBSP_CAMCC_CAMCCPLL1),
  .pRail        = &ClockRail_VDDMX,
  .nConfigMask  = CLOCK_CONFIG_PLL_EVEN_OUTPUT_ENABLE,
  .eDisableMode = HAL_CLK_SOURCE_DISABLE_MODE_NORMAL,
  .HALSource = {
    .nAddr          = HWIO_PHYS(CAM_CC_PLL1_MODE),
    .pmControl      = &HAL_clk_TrionPLLControl,
  },
  .pCalibrationFreqConfig     = ClockSourceBSPCal_CAMCC_CAMCCPLL0,
  .nCalibrationFreqConfigLen  = SIZEOF_ARRAY(ClockSourceBSPCal_CAMCC_CAMCCPLL0),
  .pRegSettings               = ClockSourceRegSettings_CAMCC_CAMCCPLL0,
  .nRegSettingsLen            = SIZEOF_ARRAY(ClockSourceRegSettings_CAMCC_CAMCCPLL0),
};

static ClockSourceNodeType ClockSource_CAMCC_CAMCCPLL2 =
{
  .szName       = "cam_cc_pll2",
  .nFlags       = CLOCK_FLAG_INTERNAL_CONTROL,
  .aBSP         = ClockSourceBSP_CAMCC_CAMCCPLL2,
  .nBSPLen      = SIZEOF_ARRAY(ClockSourceBSP_CAMCC_CAMCCPLL2),
  .pRail        = &ClockRail_VDDMX,
  .nConfigMask  = CLOCK_CONFIG_PLL_EARLY_OUTPUT_ENABLE,
  .eDisableMode = HAL_CLK_SOURCE_DISABLE_MODE_NORMAL,
  .HALSource = {
    .nAddr          = HWIO_PHYS(CAM_CC_PLL2_MODE),
    .pmControl      = &HAL_clk_RegeraPLLControl,
  },
  .pCalibrationFreqConfig     = ClockSourceBSP_CAMCC_CAMCCPLL2,
  .nCalibrationFreqConfigLen  = SIZEOF_ARRAY(ClockSourceBSP_CAMCC_CAMCCPLL2),
  .pRegSettings               = ClockSourceRegSettings_CAMCC_CAMCCPLL2,
  .nRegSettingsLen            = SIZEOF_ARRAY(ClockSourceRegSettings_CAMCC_CAMCCPLL2),
};

static ClockSourceNodeType ClockSource_CAMCC_CAMCCPLL3 =
{
  .szName       = "cam_cc_pll3",
  .nFlags       = CLOCK_FLAG_INTERNAL_CONTROL,
  .aBSP         = ClockSourceBSP_CAMCC_CAMCCPLL3,
  .nBSPLen      = SIZEOF_ARRAY(ClockSourceBSP_CAMCC_CAMCCPLL3),
  .pRail        = &ClockRail_VDDMX,
  .nConfigMask  = CLOCK_CONFIG_PLL_EVEN_OUTPUT_ENABLE,
  .eDisableMode = HAL_CLK_SOURCE_DISABLE_MODE_NORMAL,
  .HALSource = {
    .nAddr          = HWIO_PHYS(CAM_CC_PLL3_MODE),
    .pmControl      = &HAL_clk_TrionPLLControl,
  },
  .pCalibrationFreqConfig     = ClockSourceBSPCal_CAMCC_CAMCCPLL0,
  .nCalibrationFreqConfigLen  = SIZEOF_ARRAY(ClockSourceBSPCal_CAMCC_CAMCCPLL0),
  .pRegSettings               = ClockSourceRegSettings_CAMCC_CAMCCPLL0,
  .nRegSettingsLen            = SIZEOF_ARRAY(ClockSourceRegSettings_CAMCC_CAMCCPLL0),
};

static ClockSourceNodeType ClockSource_CAMCC_CAMCCPLL4 =
{
  .szName       = "cam_cc_pll4",
  .nFlags       = CLOCK_FLAG_INTERNAL_CONTROL,
  .aBSP         = ClockSourceBSP_CAMCC_CAMCCPLL3,
  .nBSPLen      = SIZEOF_ARRAY(ClockSourceBSP_CAMCC_CAMCCPLL3),
  .pRail        = &ClockRail_VDDMX,
  .nConfigMask  = CLOCK_CONFIG_PLL_EVEN_OUTPUT_ENABLE,
  .eDisableMode = HAL_CLK_SOURCE_DISABLE_MODE_NORMAL,
  .HALSource = {
    .nAddr          = HWIO_PHYS(CAM_CC_PLL4_MODE),
    .pmControl      = &HAL_clk_TrionPLLControl,
  },
  .pCalibrationFreqConfig     = ClockSourceBSPCal_CAMCC_CAMCCPLL0,
  .nCalibrationFreqConfigLen  = SIZEOF_ARRAY(ClockSourceBSPCal_CAMCC_CAMCCPLL0),
  .pRegSettings               = ClockSourceRegSettings_CAMCC_CAMCCPLL0,
  .nRegSettingsLen            = SIZEOF_ARRAY(ClockSourceRegSettings_CAMCC_CAMCCPLL0),
};

static ClockSourceNodeType ClockSource_CAMCC_CAMCCPLL5 =
{
  .szName       = "cam_cc_pll5",
  .nFlags       = CLOCK_FLAG_INTERNAL_CONTROL,
  .aBSP         = ClockSourceBSP_CAMCC_CAMCCPLL3,
  .nBSPLen      = SIZEOF_ARRAY(ClockSourceBSP_CAMCC_CAMCCPLL3),
  .pRail        = &ClockRail_VDDMX,
  .nConfigMask  = CLOCK_CONFIG_PLL_EVEN_OUTPUT_ENABLE,
  .eDisableMode = HAL_CLK_SOURCE_DISABLE_MODE_NORMAL,
  .HALSource = {
    .nAddr          = HWIO_PHYS(CAM_CC_PLL5_MODE),
    .pmControl      = &HAL_clk_TrionPLLControl,
  },
  .pCalibrationFreqConfig     = ClockSourceBSPCal_CAMCC_CAMCCPLL0,
  .nCalibrationFreqConfigLen  = SIZEOF_ARRAY(ClockSourceBSPCal_CAMCC_CAMCCPLL0),
  .pRegSettings               = ClockSourceRegSettings_CAMCC_CAMCCPLL0,
  .nRegSettingsLen            = SIZEOF_ARRAY(ClockSourceRegSettings_CAMCC_CAMCCPLL0),
};

static ClockSourceNodeType ClockSource_CAMCC_CAMCCPLL6 =
{
  .szName       = "cam_cc_pll6",
  .nFlags       = CLOCK_FLAG_INTERNAL_CONTROL,
  .aBSP         = ClockSourceBSP_CAMCC_CAMCCPLL3,
  .nBSPLen      = SIZEOF_ARRAY(ClockSourceBSP_CAMCC_CAMCCPLL3),
  .pRail        = &ClockRail_VDDMX,
  .nConfigMask  = CLOCK_CONFIG_PLL_EVEN_OUTPUT_ENABLE,
  .eDisableMode = HAL_CLK_SOURCE_DISABLE_MODE_NORMAL,
  .HALSource = {
    .nAddr          = HWIO_PHYS(CAM_CC_PLL6_MODE),
    .pmControl      = &HAL_clk_TrionPLLControl,
  },
  .pCalibrationFreqConfig     = ClockSourceBSPCal_CAMCC_CAMCCPLL0,
  .nCalibrationFreqConfigLen  = SIZEOF_ARRAY(ClockSourceBSPCal_CAMCC_CAMCCPLL0),
  .pRegSettings               = ClockSourceRegSettings_CAMCC_CAMCCPLL0,
  .nRegSettingsLen            = SIZEOF_ARRAY(ClockSourceRegSettings_CAMCC_CAMCCPLL0),
};

static ClockSourceNodeType ClockSource_DISPCC_DISPCCPLL0 =
{
  .szName       = "disp_cc_pll0",
  .nFlags       = CLOCK_FLAG_INTERNAL_CONTROL,
  .aBSP         = ClockSourceBSP_DISPCC_DISPCCPLL0,
  .nBSPLen      = SIZEOF_ARRAY(ClockSourceBSP_DISPCC_DISPCCPLL0),
  .pRail        = &ClockRail_VDDMM,
  .nConfigMask  = 0x0,
  .eDisableMode = HAL_CLK_SOURCE_DISABLE_MODE_NORMAL,
  .HALSource = {
    .nAddr          = HWIO_PHYS(DISP_CC_PLL0_MODE),
    .pmControl      = &HAL_clk_TrionPLLControl,
  },
  .pCalibrationFreqConfig     = ClockSourceBSPCal_CAMCC_CAMCCPLL0,
  .nCalibrationFreqConfigLen  = SIZEOF_ARRAY(ClockSourceBSPCal_CAMCC_CAMCCPLL0),
  .pRegSettings               = ClockSourceRegSettings_CAMCC_CAMCCPLL0,
  .nRegSettingsLen            = SIZEOF_ARRAY(ClockSourceRegSettings_CAMCC_CAMCCPLL0),
};

static ClockSourceNodeType ClockSource_DISPCC_DISPCCPLL1 =
{
  .szName       = "disp_cc_pll1",
  .nFlags       = CLOCK_FLAG_INTERNAL_CONTROL,
  .aBSP         = ClockSourceBSP_DISPCC_DISPCCPLL1,
  .nBSPLen      = SIZEOF_ARRAY(ClockSourceBSP_DISPCC_DISPCCPLL1),
  .pRail        = &ClockRail_VDDMM,
  .nConfigMask  = 0x0,
  .eDisableMode = HAL_CLK_SOURCE_DISABLE_MODE_NORMAL,
  .HALSource = {
    .nAddr          = HWIO_PHYS(DISP_CC_PLL1_MODE),
    .pmControl      = &HAL_clk_TrionPLLControl,
  },
  .pCalibrationFreqConfig     = ClockSourceBSPCal_CAMCC_CAMCCPLL0,
  .nCalibrationFreqConfigLen  = SIZEOF_ARRAY(ClockSourceBSPCal_CAMCC_CAMCCPLL0),
  .pRegSettings               = ClockSourceRegSettings_CAMCC_CAMCCPLL0,
  .nRegSettingsLen            = SIZEOF_ARRAY(ClockSourceRegSettings_CAMCC_CAMCCPLL0),
};

static ClockSourceNodeType ClockSource_GCC_GPLL0 =
{
  .szName       = "gpll0",
  .nFlags       = CLOCK_FLAG_INTERNAL_CONTROL |
                  CLOCK_FLAG_READ_ONLY,
  .aBSP         = ClockSourceBSP_GCC_GPLL0,
  .nBSPLen      = SIZEOF_ARRAY(ClockSourceBSP_GCC_GPLL0),
  .pRail        = &ClockRail_VDDCX,
  .nConfigMask  = CLOCK_CONFIG_PLL_EVEN_OUTPUT_ENABLE |
                  CLOCK_CONFIG_PLL_FSM_MODE_ENABLE,
  .eDisableMode = HAL_CLK_SOURCE_DISABLE_MODE_NORMAL,
  .HALSource = {
    .nAddr          = HWIO_PHYS(GCC_GPLL0_MODE),
    .VoterRegister  = {HWIO_PHYS(GCC_PROC_CLK_PLL_ENA_VOTE), 0x1},
    .pmControl      = &HAL_clk_TrionPLLControl,
  },
  .pCalibrationFreqConfig     = ClockSourceBSPCal_CAMCC_CAMCCPLL0,
  .nCalibrationFreqConfigLen  = SIZEOF_ARRAY(ClockSourceBSPCal_CAMCC_CAMCCPLL0),
};

static ClockSourceNodeType ClockSource_GCC_GPLL7 =
{
  .szName       = "gpll7",
  .nFlags       = CLOCK_FLAG_INTERNAL_CONTROL |
                  CLOCK_FLAG_READ_ONLY,
  .aBSP         = ClockSourceBSP_GCC_GPLL7,
  .nBSPLen      = SIZEOF_ARRAY(ClockSourceBSP_GCC_GPLL7),
  .pRail        = &ClockRail_VDDCX,
  .nConfigMask  = CLOCK_CONFIG_PLL_FSM_MODE_ENABLE,
  .eDisableMode = HAL_CLK_SOURCE_DISABLE_MODE_NORMAL,
  .HALSource = {
    .nAddr          = HWIO_PHYS(GCC_GPLL7_MODE),
    .VoterRegister  = {HWIO_PHYS(GCC_PROC_CLK_PLL_ENA_VOTE), 0x80},
    .pmControl      = &HAL_clk_TrionPLLControl,
  },
  .pCalibrationFreqConfig     = ClockSourceBSPCal_CAMCC_CAMCCPLL0,
  .nCalibrationFreqConfigLen  = SIZEOF_ARRAY(ClockSourceBSPCal_CAMCC_CAMCCPLL0),
};

static ClockSourceNodeType ClockSource_GCC_GPLL9 =
{
  .szName       = "gpll9",
  .nFlags       = CLOCK_FLAG_INTERNAL_CONTROL |
                  CLOCK_FLAG_READ_ONLY,
  .aBSP         = ClockSourceBSP_GCC_GPLL9,
  .nBSPLen      = SIZEOF_ARRAY(ClockSourceBSP_GCC_GPLL9),
  .pRail        = &ClockRail_VDDCX,
  .nConfigMask  = CLOCK_CONFIG_PLL_FSM_MODE_ENABLE,
  .eDisableMode = HAL_CLK_SOURCE_DISABLE_MODE_NORMAL,
  .HALSource = {
    .nAddr          = HWIO_PHYS(GCC_GPLL9_MODE),
    .VoterRegister  = {HWIO_PHYS(GCC_PROC_CLK_PLL_ENA_VOTE), 0x200},
    .pmControl      = &HAL_clk_TrionPLLControl,
  },
  .pCalibrationFreqConfig     = ClockSourceBSPCal_GCC_GPLL9,
  .nCalibrationFreqConfigLen  = SIZEOF_ARRAY(ClockSourceBSPCal_GCC_GPLL9),
};

static ClockSourceNodeType ClockSource_GPUCC_GPUCCPLL0 =
{
  .szName       = "gpu_cc_pll0",
  .nFlags       = CLOCK_FLAG_INTERNAL_CONTROL,
  .aBSP         = ClockSourceBSP_GPUCC_GPUCCPLL0,
  .nBSPLen      = SIZEOF_ARRAY(ClockSourceBSP_GPUCC_GPUCCPLL0),
  .pRail        = &ClockRail_VDDMX,
  .nConfigMask  = CLOCK_CONFIG_PLL_EVEN_OUTPUT_ENABLE,
  .eDisableMode = HAL_CLK_SOURCE_DISABLE_MODE_NORMAL,
  .HALSource = {
    .nAddr          = HWIO_PHYS(GPUCC_GPU_CC_PLL0_MODE),
    .pmControl      = &HAL_clk_TrionPLLControl,
  },
  .pCalibrationFreqConfig     = ClockSourceBSPCal_CAMCC_CAMCCPLL0,
  .nCalibrationFreqConfigLen  = SIZEOF_ARRAY(ClockSourceBSPCal_CAMCC_CAMCCPLL0),
  .pRegSettings               = ClockSourceRegSettings_CAMCC_CAMCCPLL0,
  .nRegSettingsLen            = SIZEOF_ARRAY(ClockSourceRegSettings_CAMCC_CAMCCPLL0),
};

static ClockSourceNodeType ClockSource_GPUCC_GPUCCPLL1 =
{
  .szName       = "gpu_cc_pll1",
  .nFlags       = CLOCK_FLAG_INTERNAL_CONTROL,
  .aBSP         = ClockSourceBSP_GPUCC_GPUCCPLL1,
  .nBSPLen      = SIZEOF_ARRAY(ClockSourceBSP_GPUCC_GPUCCPLL1),
  .pRail        = &ClockRail_VDDMX,
  .nConfigMask  = 0x0,
  .eDisableMode = HAL_CLK_SOURCE_DISABLE_MODE_NORMAL,
  .HALSource = {
    .nAddr          = HWIO_PHYS(GPUCC_GPU_CC_PLL1_MODE),
    .pmControl      = &HAL_clk_TrionPLLControl,
  },
  .pCalibrationFreqConfig     = ClockSourceBSPCal_CAMCC_CAMCCPLL0,
  .nCalibrationFreqConfigLen  = SIZEOF_ARRAY(ClockSourceBSPCal_CAMCC_CAMCCPLL0),
  .pRegSettings               = ClockSourceRegSettings_CAMCC_CAMCCPLL0,
  .nRegSettingsLen            = SIZEOF_ARRAY(ClockSourceRegSettings_CAMCC_CAMCCPLL0),
};

static ClockSourceNodeType ClockSource_NPUCC_NPUCCPLL0 =
{
  .szName       = "npu_cc_pll0",
  .nFlags       = CLOCK_FLAG_INTERNAL_CONTROL,
  .aBSP         = ClockSourceBSP_NPUCC_NPUCCPLL0,
  .nBSPLen      = SIZEOF_ARRAY(ClockSourceBSP_NPUCC_NPUCCPLL0),
  .pRail        = &ClockRail_VDDCX,
  .nConfigMask  = CLOCK_CONFIG_PLL_EVEN_OUTPUT_ENABLE,
  .eDisableMode = HAL_CLK_SOURCE_DISABLE_MODE_NORMAL,
  .HALSource = {
    .nAddr          = HWIO_PHYS(NPU_CC_PLL0_MODE),
    .pmControl      = &HAL_clk_TrionPLLControl,
  },
  .pCalibrationFreqConfig     = ClockSourceBSPCal_CAMCC_CAMCCPLL0,
  .nCalibrationFreqConfigLen  = SIZEOF_ARRAY(ClockSourceBSPCal_CAMCC_CAMCCPLL0),
  .pRegSettings               = ClockSourceRegSettings_CAMCC_CAMCCPLL0,
  .nRegSettingsLen            = SIZEOF_ARRAY(ClockSourceRegSettings_CAMCC_CAMCCPLL0),
};

static ClockSourceNodeType ClockSource_NPUCC_NPUCCPLL1 =
{
  .szName       = "npu_cc_pll1",
  .nFlags       = CLOCK_FLAG_INTERNAL_CONTROL,
  .aBSP         = ClockSourceBSP_NPUCC_NPUCCPLL1,
  .nBSPLen      = SIZEOF_ARRAY(ClockSourceBSP_NPUCC_NPUCCPLL1),
  .pRail        = &ClockRail_VDDCX,
  .nConfigMask  = CLOCK_CONFIG_PLL_EVEN_OUTPUT_ENABLE,
  .eDisableMode = HAL_CLK_SOURCE_DISABLE_MODE_NORMAL,
  .HALSource = {
    .nAddr          = HWIO_PHYS(NPU_CC_PLL1_MODE),
    .pmControl      = &HAL_clk_TrionPLLControl,
  },
  .pCalibrationFreqConfig     = ClockSourceBSPCal_CAMCC_CAMCCPLL0,
  .nCalibrationFreqConfigLen  = SIZEOF_ARRAY(ClockSourceBSPCal_CAMCC_CAMCCPLL0),
  .pRegSettings               = ClockSourceRegSettings_CAMCC_CAMCCPLL0,
  .nRegSettingsLen            = SIZEOF_ARRAY(ClockSourceRegSettings_CAMCC_CAMCCPLL0),
};
#ifdef ENABLE_AUTO_PLAT
static ClockSourceNodeType ClockSource_SCC_SCCPLL =
{
  .szName       = "scc_pll",
  .nFlags       = CLOCK_FLAG_INTERNAL_CONTROL,
  .aBSP         = ClockSourceBSP_SCC_SCCPLL,
  .nBSPLen      = SIZEOF_ARRAY(ClockSourceBSP_SCC_SCCPLL),
  .pRail        = &ClockRail_VDDSSCINT,
  .nConfigMask  = CLOCK_CONFIG_PLL_EVEN_OUTPUT_ENABLE,
  .eDisableMode = HAL_CLK_SOURCE_DISABLE_MODE_NORMAL,
  .HALSource = {
    .nAddr          = HWIO_PHYS(SSC_SCC_PLL_MODE),
    .pmControl      = &HAL_clk_TrionPLLControl,
  },
  .pCalibrationFreqConfig     = ClockSourceBSPCal_CAMCC_CAMCCPLL0,
  .nCalibrationFreqConfigLen  = SIZEOF_ARRAY(ClockSourceBSPCal_CAMCC_CAMCCPLL0),
  .pRegSettings               = ClockSourceRegSettings_CAMCC_CAMCCPLL0,
  .nRegSettingsLen            = SIZEOF_ARRAY(ClockSourceRegSettings_CAMCC_CAMCCPLL0),
};
#endif


static ClockSourceNodeType ClockSource_VIDEOCC_VIDEOPLL0 =
{
  .szName       = "video_pll0",
  .nFlags       = CLOCK_FLAG_INTERNAL_CONTROL,
  .aBSP         = ClockSourceBSP_VIDEOCC_VIDEOPLL0,
  .nBSPLen      = SIZEOF_ARRAY(ClockSourceBSP_VIDEOCC_VIDEOPLL0),
  .pRail        = &ClockRail_VDDMM,
  .nConfigMask  = 0x0,
  .eDisableMode = HAL_CLK_SOURCE_DISABLE_MODE_NORMAL,
  .HALSource = {
    .nAddr          = HWIO_PHYS(VIDEO_CC_VIDEO_PLL0_MODE),
    .pmControl      = &HAL_clk_TrionPLLControl,
  },
  .pCalibrationFreqConfig     = ClockSourceBSPCal_CAMCC_CAMCCPLL0,
  .nCalibrationFreqConfigLen  = SIZEOF_ARRAY(ClockSourceBSPCal_CAMCC_CAMCCPLL0),
  .pRegSettings               = ClockSourceRegSettings_CAMCC_CAMCCPLL0,
  .nRegSettingsLen            = SIZEOF_ARRAY(ClockSourceRegSettings_CAMCC_CAMCCPLL0),
};


static ClockSourceNodeType *aSources[] =
{
  &ClockSource_CAMCC_CAMCCPLL0,
  &ClockSource_CAMCC_CAMCCPLL1,
  &ClockSource_CAMCC_CAMCCPLL2,
  &ClockSource_CAMCC_CAMCCPLL3,
  &ClockSource_CAMCC_CAMCCPLL4,
  &ClockSource_CAMCC_CAMCCPLL5,
  &ClockSource_CAMCC_CAMCCPLL6,
  &ClockSource_DISPCC_DISPCCPLL0,
  &ClockSource_DISPCC_DISPCCPLL1,
  &ClockSource_GCC_GPLL0,
  &ClockSource_GCC_GPLL7,
  &ClockSource_GCC_GPLL9,
  &ClockSource_GPUCC_GPUCCPLL0,
  &ClockSource_GPUCC_GPUCCPLL1,
  &ClockSource_NPUCC_NPUCCPLL0,
  &ClockSource_NPUCC_NPUCCPLL1,
#ifdef ENABLE_AUTO_PLAT
  &ClockSource_SCC_SCCPLL,
#endif
  &ClockSource_VIDEOCC_VIDEOPLL0,
  &ClockSource_XO,
};


/*=========================================================================
      Domain BSP
==========================================================================*/

/*
 * ClockDomainBSP_CAMCC_CAMCCBPS
 *
 * Used by:
 *   ClockDomain_CAMCC_CAMCCBPS
 *   ClockDomain_CAMCC_CAMCCJPEG
 */
static ClockMuxConfigType ClockDomainBSP_CAMCC_CAMCCBPS[] =
{
  {   19200000, &ClockSource_XO,                          { 0,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = BI_TCXO */
  {  100000000, &ClockSource_CAMCC_CAMCCPLL0,             { 2,     12,    0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[2] = CAM_CC_PLL0_OUT_EVEN */
  {  200000000, &ClockSource_CAMCC_CAMCCPLL0,             { 3,     4,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[3] = CAM_CC_PLL0_OUT_ODD */
  {  400000000, &ClockSource_CAMCC_CAMCCPLL0,             { 3,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS,        },                                                            /* Mux[3] = CAM_CC_PLL0_OUT_ODD */
  {  480000000, &ClockSource_CAMCC_CAMCCPLL2,             { 5,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS_L1,     },                                                            /* Mux[5] = CAM_CC_PLL2_OUT_MAIN */
  {  600000000, &ClockSource_CAMCC_CAMCCPLL0,             { 1,     4,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_NOM,        },                                                            /* Mux[1] = CAM_CC_PLL0_OUT_MAIN */
};

/*
 * ClockDomainBSP_CAMCC_CAMCCCAMNOCAXI
 *
 * Used by:
 *   ClockDomain_CAMCC_CAMCCCAMNOCAXI
 */
static ClockMuxConfigType ClockDomainBSP_CAMCC_CAMCCCAMNOCAXI[] =
{
  {   19200000, &ClockSource_XO,                          { 0,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = BI_TCXO */
  {  150000000, &ClockSource_CAMCC_CAMCCPLL0,             { 2,     8,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[2] = CAM_CC_PLL0_OUT_EVEN */
  {  266666667, &ClockSource_CAMCC_CAMCCPLL0,             { 3,     3,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS,        },                                                            /* Mux[3] = CAM_CC_PLL0_OUT_ODD */
  {  320000000, &ClockSource_CAMCC_CAMCCPLL2,             { 5,     3,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS_L1,     },                                                            /* Mux[5] = CAM_CC_PLL2_OUT_MAIN */
  {  400000000, &ClockSource_CAMCC_CAMCCPLL0,             { 1,     6,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_NOM,        },                                                            /* Mux[1] = CAM_CC_PLL0_OUT_MAIN */
  {  480000000, &ClockSource_CAMCC_CAMCCPLL2,             { 5,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_TUR,        },                                                            /* Mux[5] = CAM_CC_PLL2_OUT_MAIN */
};

/*
 * ClockDomainBSP_CAMCC_CAMCCCCI0
 *
 * Used by:
 *   ClockDomain_CAMCC_CAMCCCCI0
 *   ClockDomain_CAMCC_CAMCCCCI1
 *   ClockDomain_CAMCC_CAMCCCCI2
 *   ClockDomain_CAMCC_CAMCCCCI3
 */
static ClockMuxConfigType ClockDomainBSP_CAMCC_CAMCCCCI0[] =
{
  {   19200000, &ClockSource_XO,                          { 0,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = BI_TCXO */
  {   37500000, &ClockSource_CAMCC_CAMCCPLL0,             { 2,     32,    0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[2] = CAM_CC_PLL0_OUT_EVEN */
};

/*
 * ClockDomainBSP_CAMCC_CAMCCCPHYRX
 *
 * Used by:
 *   ClockDomain_CAMCC_CAMCCCPHYRX
 */
static ClockMuxConfigType ClockDomainBSP_CAMCC_CAMCCCPHYRX[] =
{
  {   19200000, &ClockSource_XO,                          { 0,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = BI_TCXO */
  {  400000000, &ClockSource_CAMCC_CAMCCPLL0,             { 3,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[3] = CAM_CC_PLL0_OUT_ODD */
};

/*
 * ClockDomainBSP_CAMCC_CAMCCCSI0PHYTIMER
 *
 * Used by:
 *   ClockDomain_CAMCC_CAMCCCSI0PHYTIMER
 *   ClockDomain_CAMCC_CAMCCCSI1PHYTIMER
 *   ClockDomain_CAMCC_CAMCCCSI2PHYTIMER
 *   ClockDomain_CAMCC_CAMCCCSI3PHYTIMER
 */
static ClockMuxConfigType ClockDomainBSP_CAMCC_CAMCCCSI0PHYTIMER[] =
{
  {   19200000, &ClockSource_XO,                          { 0,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = BI_TCXO */
  {  300000000, &ClockSource_CAMCC_CAMCCPLL0,             { 2,     4,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[2] = CAM_CC_PLL0_OUT_EVEN */
};

/*
 * ClockDomainBSP_CAMCC_CAMCCFASTAHB
 *
 * Used by:
 *   ClockDomain_CAMCC_CAMCCFASTAHB
 */
static ClockMuxConfigType ClockDomainBSP_CAMCC_CAMCCFASTAHB[] =
{
  {   19200000, &ClockSource_XO,                          { 0,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = BI_TCXO */
  {   50000000, &ClockSource_CAMCC_CAMCCPLL0,             { 2,     24,    0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[2] = CAM_CC_PLL0_OUT_EVEN */
  {  100000000, &ClockSource_CAMCC_CAMCCPLL0,             { 2,     12,    0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[2] = CAM_CC_PLL0_OUT_EVEN */
  {  200000000, &ClockSource_CAMCC_CAMCCPLL0,             { 2,     6,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS,        },                                                            /* Mux[2] = CAM_CC_PLL0_OUT_EVEN */
  {  300000000, &ClockSource_CAMCC_CAMCCPLL0,             { 1,     8,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS_L1,     },                                                            /* Mux[1] = CAM_CC_PLL0_OUT_MAIN */
  {  400000000, &ClockSource_CAMCC_CAMCCPLL0,             { 1,     6,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_NOM,        },                                                            /* Mux[1] = CAM_CC_PLL0_OUT_MAIN */
};

/*
 * ClockDomainBSP_CAMCC_CAMCCFDCORE
 *
 * Used by:
 *   ClockDomain_CAMCC_CAMCCFDCORE
 *   ClockDomain_CAMCC_CAMCCIFE2CSID
 *   ClockDomain_CAMCC_CAMCCIFE3CSID
 *   ClockDomain_CAMCC_CAMCCIFELITE0CSID
 *   ClockDomain_CAMCC_CAMCCIFELITE1CSID
 *   ClockDomain_CAMCC_CAMCCIFELITE2CSID
 *   ClockDomain_CAMCC_CAMCCIFELITE3CSID
 */
static ClockMuxConfigType ClockDomainBSP_CAMCC_CAMCCFDCORE[] =
{
  {   19200000, &ClockSource_XO,                          { 0,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = BI_TCXO */
  {  400000000, &ClockSource_CAMCC_CAMCCPLL0,             { 3,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[3] = CAM_CC_PLL0_OUT_ODD */
  {  480000000, &ClockSource_CAMCC_CAMCCPLL2,             { 5,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS_L1,     },                                                            /* Mux[5] = CAM_CC_PLL2_OUT_MAIN */
  {  600000000, &ClockSource_CAMCC_CAMCCPLL0,             { 1,     4,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_NOM,        },                                                            /* Mux[1] = CAM_CC_PLL0_OUT_MAIN */
};

/*
 * ClockDomainBSP_CAMCC_CAMCCICP
 *
 * Used by:
 *   ClockDomain_CAMCC_CAMCCICP
 */
static ClockMuxConfigType ClockDomainBSP_CAMCC_CAMCCICP[] =
{
  {   19200000, &ClockSource_XO,                          { 0,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = BI_TCXO */
  {  400000000, &ClockSource_CAMCC_CAMCCPLL0,             { 3,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[3] = CAM_CC_PLL0_OUT_ODD */
  {  600000000, &ClockSource_CAMCC_CAMCCPLL0,             { 1,     4,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS_L1,     },                                                            /* Mux[1] = CAM_CC_PLL0_OUT_MAIN */
};

/*
 * ClockDomainBSP_CAMCC_CAMCCIFE0
 *
 * Used by:
 *   ClockDomain_CAMCC_CAMCCIFE0
 */
static ClockMuxConfigType ClockDomainBSP_CAMCC_CAMCCIFE0[] =
{
  {   19200000, &ClockSource_XO,                          { 0,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = BI_TCXO */
  {  400000000, &ClockSource_CAMCC_CAMCCPLL3,             { 6,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    {{0}}, &ClockSourceBSP_CAMCC_CAMCCPLL3[ 0] /* 400.0  */ },    /* Mux[6] = CAM_CC_PLL3_OUT_EVEN */
  {  558000000, &ClockSource_CAMCC_CAMCCPLL3,             { 6,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS,        {{0}}, &ClockSourceBSP_CAMCC_CAMCCPLL3[ 1] /* 558.0  */ },    /* Mux[6] = CAM_CC_PLL3_OUT_EVEN */
  {  637000000, &ClockSource_CAMCC_CAMCCPLL3,             { 6,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS_L1,     {{0}}, &ClockSourceBSP_CAMCC_CAMCCPLL3[ 2] /* 637.0  */ },    /* Mux[6] = CAM_CC_PLL3_OUT_EVEN */
  {  760000000, &ClockSource_CAMCC_CAMCCPLL3,             { 6,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_NOM,        {{0}}, &ClockSourceBSP_CAMCC_CAMCCPLL3[ 3] /* 760.0  */ },    /* Mux[6] = CAM_CC_PLL3_OUT_EVEN */
};

/*
 * ClockDomainBSP_CAMCC_CAMCCIFE0CSID
 *
 * Used by:
 *   ClockDomain_CAMCC_CAMCCIFE0CSID
 *   ClockDomain_CAMCC_CAMCCIFE1CSID
 */
static ClockMuxConfigType ClockDomainBSP_CAMCC_CAMCCIFE0CSID[] =
{
  {   19200000, &ClockSource_XO,                          { 0,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = BI_TCXO */
  {   75000000, &ClockSource_CAMCC_CAMCCPLL0,             { 2,     16,    0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[2] = CAM_CC_PLL0_OUT_EVEN */
  {  400000000, &ClockSource_CAMCC_CAMCCPLL0,             { 3,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[3] = CAM_CC_PLL0_OUT_ODD */
  {  480000000, &ClockSource_CAMCC_CAMCCPLL2,             { 5,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS_L1,     },                                                            /* Mux[5] = CAM_CC_PLL2_OUT_MAIN */
  {  600000000, &ClockSource_CAMCC_CAMCCPLL0,             { 1,     4,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_NOM,        },                                                            /* Mux[1] = CAM_CC_PLL0_OUT_MAIN */
};

/*
 * ClockDomainBSP_CAMCC_CAMCCIFE1
 *
 * Used by:
 *   ClockDomain_CAMCC_CAMCCIFE1
 */
static ClockMuxConfigType ClockDomainBSP_CAMCC_CAMCCIFE1[] =
{
  {   19200000, &ClockSource_XO,                          { 0,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = BI_TCXO */
  {  400000000, &ClockSource_CAMCC_CAMCCPLL4,             { 6,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    {{0}}, &ClockSourceBSP_CAMCC_CAMCCPLL3[ 0] /* 400.0  */ },    /* Mux[6] = CAM_CC_PLL4_OUT_EVEN */
  {  558000000, &ClockSource_CAMCC_CAMCCPLL4,             { 6,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS,        {{0}}, &ClockSourceBSP_CAMCC_CAMCCPLL3[ 1] /* 558.0  */ },    /* Mux[6] = CAM_CC_PLL4_OUT_EVEN */
  {  637000000, &ClockSource_CAMCC_CAMCCPLL4,             { 6,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS_L1,     {{0}}, &ClockSourceBSP_CAMCC_CAMCCPLL3[ 2] /* 637.0  */ },    /* Mux[6] = CAM_CC_PLL4_OUT_EVEN */
  {  760000000, &ClockSource_CAMCC_CAMCCPLL4,             { 6,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_NOM,        {{0}}, &ClockSourceBSP_CAMCC_CAMCCPLL3[ 3] /* 760.0  */ },    /* Mux[6] = CAM_CC_PLL4_OUT_EVEN */
};

/*
 * ClockDomainBSP_CAMCC_CAMCCIFE2
 *
 * Used by:
 *   ClockDomain_CAMCC_CAMCCIFE2
 */
static ClockMuxConfigType ClockDomainBSP_CAMCC_CAMCCIFE2[] =
{
  {   19200000, &ClockSource_XO,                          { 0,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = BI_TCXO */
  {  400000000, &ClockSource_CAMCC_CAMCCPLL5,             { 6,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    {{0}}, &ClockSourceBSP_CAMCC_CAMCCPLL3[ 0] /* 400.0  */ },    /* Mux[6] = CAM_CC_PLL5_OUT_EVEN */
  {  558000000, &ClockSource_CAMCC_CAMCCPLL5,             { 6,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS,        {{0}}, &ClockSourceBSP_CAMCC_CAMCCPLL3[ 1] /* 558.0  */ },    /* Mux[6] = CAM_CC_PLL5_OUT_EVEN */
  {  637000000, &ClockSource_CAMCC_CAMCCPLL5,             { 6,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS_L1,     {{0}}, &ClockSourceBSP_CAMCC_CAMCCPLL3[ 2] /* 637.0  */ },    /* Mux[6] = CAM_CC_PLL5_OUT_EVEN */
  {  760000000, &ClockSource_CAMCC_CAMCCPLL5,             { 6,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_NOM,        {{0}}, &ClockSourceBSP_CAMCC_CAMCCPLL3[ 3] /* 760.0  */ },    /* Mux[6] = CAM_CC_PLL5_OUT_EVEN */
};

/*
 * ClockDomainBSP_CAMCC_CAMCCIFE3
 *
 * Used by:
 *   ClockDomain_CAMCC_CAMCCIFE3
 */
static ClockMuxConfigType ClockDomainBSP_CAMCC_CAMCCIFE3[] =
{
  {   19200000, &ClockSource_XO,                          { 0,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = BI_TCXO */
  {  400000000, &ClockSource_CAMCC_CAMCCPLL6,             { 6,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    {{0}}, &ClockSourceBSP_CAMCC_CAMCCPLL3[ 0] /* 400.0  */ },    /* Mux[6] = CAM_CC_PLL6_OUT_EVEN */
  {  558000000, &ClockSource_CAMCC_CAMCCPLL6,             { 6,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS,        {{0}}, &ClockSourceBSP_CAMCC_CAMCCPLL3[ 1] /* 558.0  */ },    /* Mux[6] = CAM_CC_PLL6_OUT_EVEN */
  {  637000000, &ClockSource_CAMCC_CAMCCPLL6,             { 6,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS_L1,     {{0}}, &ClockSourceBSP_CAMCC_CAMCCPLL3[ 2] /* 637.0  */ },    /* Mux[6] = CAM_CC_PLL6_OUT_EVEN */
  {  760000000, &ClockSource_CAMCC_CAMCCPLL6,             { 6,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_NOM,        {{0}}, &ClockSourceBSP_CAMCC_CAMCCPLL3[ 3] /* 760.0  */ },    /* Mux[6] = CAM_CC_PLL6_OUT_EVEN */
};

/*
 * ClockDomainBSP_CAMCC_CAMCCIFELITE0
 *
 * Used by:
 *   ClockDomain_CAMCC_CAMCCIFELITE0
 *   ClockDomain_CAMCC_CAMCCIFELITE1
 *   ClockDomain_CAMCC_CAMCCIFELITE2
 *   ClockDomain_CAMCC_CAMCCIFELITE3
 */
static ClockMuxConfigType ClockDomainBSP_CAMCC_CAMCCIFELITE0[] =
{
  {   19200000, &ClockSource_XO,                          { 0,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = BI_TCXO */
  {  320000000, &ClockSource_CAMCC_CAMCCPLL2,             { 5,     3,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[5] = CAM_CC_PLL2_OUT_MAIN */
  {  400000000, &ClockSource_CAMCC_CAMCCPLL0,             { 3,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS,        },                                                            /* Mux[3] = CAM_CC_PLL0_OUT_ODD */
  {  480000000, &ClockSource_CAMCC_CAMCCPLL2,             { 5,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS_L1,     },                                                            /* Mux[5] = CAM_CC_PLL2_OUT_MAIN */
  {  600000000, &ClockSource_CAMCC_CAMCCPLL0,             { 1,     4,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_NOM,        },                                                            /* Mux[1] = CAM_CC_PLL0_OUT_MAIN */
};

/*
 * ClockDomainBSP_CAMCC_CAMCCIPE0
 *
 * Used by:
 *   ClockDomain_CAMCC_CAMCCIPE0
 */
static ClockMuxConfigType ClockDomainBSP_CAMCC_CAMCCIPE0[] =
{
  {   19200000, &ClockSource_XO,                          { 0,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = BI_TCXO */
  {  375000000, &ClockSource_CAMCC_CAMCCPLL1,             { 4,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    {{0}}, &ClockSourceBSP_CAMCC_CAMCCPLL1[ 0] /* 375.0  */ },    /* Mux[4] = CAM_CC_PLL1_OUT_EVEN */
  {  475000000, &ClockSource_CAMCC_CAMCCPLL1,             { 4,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS,        {{0}}, &ClockSourceBSP_CAMCC_CAMCCPLL1[ 1] /* 475.0  */ },    /* Mux[4] = CAM_CC_PLL1_OUT_EVEN */
  {  520000000, &ClockSource_CAMCC_CAMCCPLL1,             { 4,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS_L1,     {{0}}, &ClockSourceBSP_CAMCC_CAMCCPLL1[ 2] /* 520.0  */ },    /* Mux[4] = CAM_CC_PLL1_OUT_EVEN */
  {  600000000, &ClockSource_CAMCC_CAMCCPLL1,             { 4,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_NOM,        {{0}}, &ClockSourceBSP_CAMCC_CAMCCPLL1[ 3] /* 600.0  */ },    /* Mux[4] = CAM_CC_PLL1_OUT_EVEN */
};

/*
 * ClockDomainBSP_CAMCC_CAMCCLRME
 *
 * Used by:
 *   ClockDomain_CAMCC_CAMCCLRME
 */
static ClockMuxConfigType ClockDomainBSP_CAMCC_CAMCCLRME[] =
{
  {   19200000, &ClockSource_XO,                          { 0,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = BI_TCXO */
  {  100000000, &ClockSource_CAMCC_CAMCCPLL0,             { 2,     12,    0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[2] = CAM_CC_PLL0_OUT_EVEN */
  {  240000000, &ClockSource_CAMCC_CAMCCPLL2,             { 5,     4,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[5] = CAM_CC_PLL2_OUT_MAIN */
  {  300000000, &ClockSource_CAMCC_CAMCCPLL0,             { 2,     4,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS,        },                                                            /* Mux[2] = CAM_CC_PLL0_OUT_EVEN */
  {  320000000, &ClockSource_CAMCC_CAMCCPLL2,             { 5,     3,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS_L1,     },                                                            /* Mux[5] = CAM_CC_PLL2_OUT_MAIN */
  {  400000000, &ClockSource_CAMCC_CAMCCPLL0,             { 1,     6,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_NOM,        },                                                            /* Mux[1] = CAM_CC_PLL0_OUT_MAIN */
};

/*
 * ClockDomainBSP_CAMCC_CAMCCMCLK0
 *
 * Used by:
 *   ClockDomain_CAMCC_CAMCCMCLK0
 *   ClockDomain_CAMCC_CAMCCMCLK1
 *   ClockDomain_CAMCC_CAMCCMCLK2
 *   ClockDomain_CAMCC_CAMCCMCLK3
 *   ClockDomain_CAMCC_CAMCCMCLK4
 *   ClockDomain_CAMCC_CAMCCMCLK5
 *   ClockDomain_CAMCC_CAMCCMCLK6
 *   ClockDomain_CAMCC_CAMCCMCLK7
 */
static ClockMuxConfigType ClockDomainBSP_CAMCC_CAMCCMCLK0[] =
{
  {   19200000, &ClockSource_XO,                          { 0,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = BI_TCXO_MX */
  {   24000000, &ClockSource_CAMCC_CAMCCPLL2,             { 5,     20,    1,     4,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS,        },                                                            /* Mux[5] = CAM_CC_PLL2_OUT_EARLY */
  {   68571429, &ClockSource_CAMCC_CAMCCPLL2,             { 5,     28,    0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS,        },                                                            /* Mux[5] = CAM_CC_PLL2_OUT_EARLY */
};

/*
 * ClockDomainBSP_CAMCC_CAMCCQDSSDEBUG
 *
 * Used by:
 *   ClockDomain_CAMCC_CAMCCQDSSDEBUG
 */
static ClockMuxConfigType ClockDomainBSP_CAMCC_CAMCCQDSSDEBUG[] =
{
  {   19200000, &ClockSource_XO,                          { 0,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = BI_TCXO */
  {   75000000, &ClockSource_CAMCC_CAMCCPLL0,             { 2,     16,    0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[2] = CAM_CC_PLL0_OUT_EVEN */
  {  150000000, &ClockSource_CAMCC_CAMCCPLL0,             { 2,     8,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS,        },                                                            /* Mux[2] = CAM_CC_PLL0_OUT_EVEN */
  {  300000000, &ClockSource_CAMCC_CAMCCPLL0,             { 1,     8,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS_L1,     },                                                            /* Mux[1] = CAM_CC_PLL0_OUT_MAIN */
};

/*
 * ClockDomainBSP_CAMCC_CAMCCSLEEP
 *
 * Used by:
 *   ClockDomain_CAMCC_CAMCCSLEEP
 */
static ClockMuxConfigType ClockDomainBSP_CAMCC_CAMCCSLEEP[] =
{
  {      32000, NULL,                                     { 0,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = CHIP_SLEEP_CLK */
};

/*
 * ClockDomainBSP_CAMCC_CAMCCSLOWAHB
 *
 * Used by:
 *   ClockDomain_CAMCC_CAMCCSLOWAHB
 */
static ClockMuxConfigType ClockDomainBSP_CAMCC_CAMCCSLOWAHB[] =
{
  {   19200000, &ClockSource_XO,                          { 0,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = BI_TCXO */
  {   80000000, &ClockSource_CAMCC_CAMCCPLL0,             { 2,     15,    0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[2] = CAM_CC_PLL0_OUT_EVEN */
};

/*
 * ClockDomainBSP_CAMCC_CAMCCXO
 *
 * Used by:
 *   ClockDomain_CAMCC_CAMCCXO
 */
static ClockMuxConfigType ClockDomainBSP_CAMCC_CAMCCXO[] =
{
  {   19200000, &ClockSource_XO,                          { 0,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = BI_TCXO */
};

/*
 * ClockDomainBSP_DISPCC_DISPCCMDSSAHB
 *
 * Used by:
 *   ClockDomain_DISPCC_DISPCCMDSSAHB
 */
static ClockMuxConfigType ClockDomainBSP_DISPCC_DISPCCMDSSAHB[] =
{
  {   19200000, &ClockSource_XO,                          { 0,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = BI_TCXO */
  {   37500000, &ClockSource_DISPCC_DISPCCPLL1,           { 4,     32,    0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS,        },                                                            /* Mux[4] = DISP_CC_PLL1_OUT_MAIN */
  {   75000000, &ClockSource_DISPCC_DISPCCPLL1,           { 4,     16,    0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_NOM,        },                                                            /* Mux[4] = DISP_CC_PLL1_OUT_MAIN */
};

/*
 * ClockDomainBSP_DISPCC_DISPCCMDSSBYTE0
 *
 * Used by:
 *   ClockDomain_DISPCC_DISPCCMDSSBYTE0
 *   ClockDomain_DISPCC_DISPCCMDSSBYTE1
 *   ClockDomain_DISPCC_DISPCCMDSSDPAUX1
 *   ClockDomain_DISPCC_DISPCCMDSSDPAUX
 *   ClockDomain_DISPCC_DISPCCMDSSDPLINK1
 *   ClockDomain_DISPCC_DISPCCMDSSDPLINK
 *   ClockDomain_DISPCC_DISPCCMDSSDPPIXEL1
 *   ClockDomain_DISPCC_DISPCCMDSSDPPIXEL2
 *   ClockDomain_DISPCC_DISPCCMDSSDPPIXEL
 *   ClockDomain_DISPCC_DISPCCMDSSEDPAUX
 *   ClockDomain_DISPCC_DISPCCMDSSEDPLINK
 *   ClockDomain_DISPCC_DISPCCMDSSEDPPIXEL
 *   ClockDomain_DISPCC_DISPCCMDSSPCLK0
 *   ClockDomain_DISPCC_DISPCCMDSSPCLK1
 *   ClockDomain_DISPCC_DISPCCMDSSVSYNC
 *   ClockDomain_DISPCC_DISPCCXO
 */
static ClockMuxConfigType ClockDomainBSP_DISPCC_DISPCCMDSSBYTE0[] =
{
  {   12800000, &ClockSource_XO,                          { 0,     3,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = BI_TCXO */
  {   19200000, &ClockSource_XO,                          { 0,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = BI_TCXO */
};

/*
 * ClockDomainBSP_DISPCC_DISPCCMDSSDPCRYPTO1
 *
 * Used by:
 *   ClockDomain_DISPCC_DISPCCMDSSDPCRYPTO1
 *   ClockDomain_DISPCC_DISPCCMDSSDPCRYPTO
 */
static ClockMuxConfigType ClockDomainBSP_DISPCC_DISPCCMDSSDPCRYPTO1[] =
{
  {   12800000, &ClockSource_XO,                          { 0,     3,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = BI_TCXO */
  {   19200000, &ClockSource_XO,                          { 0,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[0] = BI_TCXO */
};

/*
 * ClockDomainBSP_DISPCC_DISPCCMDSSEDPGTC
 *
 * Used by:
 *   ClockDomain_DISPCC_DISPCCMDSSEDPGTC
 */
static ClockMuxConfigType ClockDomainBSP_DISPCC_DISPCCMDSSEDPGTC[] =
{
  {   19200000, &ClockSource_XO,                          { 0,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = BI_TCXO */
};

/*
 * ClockDomainBSP_DISPCC_DISPCCMDSSESC0
 *
 * Used by:
 *   ClockDomain_DISPCC_DISPCCMDSSESC0
 *   ClockDomain_DISPCC_DISPCCMDSSESC1
 */
static ClockMuxConfigType ClockDomainBSP_DISPCC_DISPCCMDSSESC0[] =
{
  {    6403613, &ClockSource_XO,                          { 0,     6,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = BI_TCXO */
  {    7683164, &ClockSource_XO,                          { 0,     5,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = BI_TCXO */
  {    9602490, &ClockSource_XO,                          { 0,     4,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = BI_TCXO */
  {   12803711, &ClockSource_XO,                          { 0,     3,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = BI_TCXO */
  {   19200000, &ClockSource_XO,                          { 0,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = BI_TCXO */
};

/*
 * ClockDomainBSP_DISPCC_DISPCCMDSSMDP
 *
 * Used by:
 *   ClockDomain_DISPCC_DISPCCMDSSMDP
 */
static ClockMuxConfigType ClockDomainBSP_DISPCC_DISPCCMDSSMDP[] =
{
  {   19200000, &ClockSource_XO,                          { 0,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = BI_TCXO */
  {   85714286, &ClockSource_DISPCC_DISPCCPLL1,           { 4,     14,    0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[4] = DISP_CC_PLL1_OUT_MAIN */
  {  100000000, &ClockSource_DISPCC_DISPCCPLL1,           { 4,     12,    0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[4] = DISP_CC_PLL1_OUT_MAIN */
  {  150000000, &ClockSource_DISPCC_DISPCCPLL1,           { 4,     8,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[4] = DISP_CC_PLL1_OUT_MAIN */
  {  171428571, &ClockSource_DISPCC_DISPCCPLL1,           { 4,     7,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[4] = DISP_CC_PLL1_OUT_MAIN */
  {  200000000, &ClockSource_DISPCC_DISPCCPLL1,           { 4,     6,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS,        },                                                            /* Mux[4] = DISP_CC_PLL1_OUT_MAIN */
  {  300000000, &ClockSource_DISPCC_DISPCCPLL1,           { 4,     4,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS,        },                                                            /* Mux[4] = DISP_CC_PLL1_OUT_MAIN */
  {  345000000, &ClockSource_DISPCC_DISPCCPLL0,           { 1,     8,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS_L1,     },                                                            /* Mux[1] = DISP_CC_PLL0_OUT_MAIN */
  {  460000000, &ClockSource_DISPCC_DISPCCPLL0,           { 1,     6,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_NOM,        },                                                            /* Mux[1] = DISP_CC_PLL0_OUT_MAIN */
};

/*
 * ClockDomainBSP_DISPCC_DISPCCMDSSROT
 *
 * Used by:
 *   ClockDomain_DISPCC_DISPCCMDSSROT
 */
static ClockMuxConfigType ClockDomainBSP_DISPCC_DISPCCMDSSROT[] =
{
  {   19200000, &ClockSource_XO,                          { 0,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = BI_TCXO */
  {  171428571, &ClockSource_DISPCC_DISPCCPLL1,           { 4,     7,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[4] = DISP_CC_PLL1_OUT_MAIN */
  {  300000000, &ClockSource_DISPCC_DISPCCPLL1,           { 4,     4,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS,        },                                                            /* Mux[4] = DISP_CC_PLL1_OUT_MAIN */
  {  345000000, &ClockSource_DISPCC_DISPCCPLL0,           { 1,     8,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS_L1,     },                                                            /* Mux[1] = DISP_CC_PLL0_OUT_MAIN */
  {  460000000, &ClockSource_DISPCC_DISPCCPLL0,           { 1,     6,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_NOM,        },                                                            /* Mux[1] = DISP_CC_PLL0_OUT_MAIN */
};

/*
 * ClockDomainBSP_DISPCC_DISPCCSLEEP
 *
 * Used by:
 *   ClockDomain_DISPCC_DISPCCSLEEP
 */
static ClockMuxConfigType ClockDomainBSP_DISPCC_DISPCCSLEEP[] =
{
  {      32000, NULL,                                     { 0,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = CHIP_SLEEP_CLK */
};

/*
 * ClockDomainBSP_GCC_GCCCPUSSAHB
 *
 * Used by:
 *   ClockDomain_GCC_GCCCPUSSAHB
 */
static ClockMuxConfigType ClockDomainBSP_GCC_GCCCPUSSAHB[] =
{
  {   19200000, &ClockSource_XO,                          { 0,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = BI_TCXO */
  {   50000000, &ClockSource_GCC_GPLL0,                   { 1,     24,    0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS,        },                                                            /* Mux[1] = GPLL0_OUT_MAIN */
  {  100000000, &ClockSource_GCC_GPLL0,                   { 1,     12,    0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_NOM,        },                                                            /* Mux[1] = GPLL0_OUT_MAIN */
};

/*
 * ClockDomainBSP_GCC_GCCEMACPTP
 *
 * Used by:
 *   ClockDomain_GCC_GCCEMACPTP
 */
static ClockMuxConfigType ClockDomainBSP_GCC_GCCEMACPTP[] =
{
  {   19200000, &ClockSource_XO,                          { 0,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = BI_TCXO */
  {   50000000, &ClockSource_GCC_GPLL0,                   { 6,     12,    0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[6] = GPLL0_OUT_EVEN */
  {  125000000, &ClockSource_GCC_GPLL7,                   { 3,     8,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS,        },                                                            /* Mux[3] = GPLL7_OUT_MAIN */
  {  250000000, &ClockSource_GCC_GPLL7,                   { 3,     4,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_NOM,        },                                                            /* Mux[3] = GPLL7_OUT_MAIN */
};

/*
 * ClockDomainBSP_GCC_GCCEMACRGMII
 *
 * Used by:
 *   ClockDomain_GCC_GCCEMACRGMII
 */
static ClockMuxConfigType ClockDomainBSP_GCC_GCCEMACRGMII[] =
{
  {    2500000, &ClockSource_XO,                          { 0,     2,     25,    192,   0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = BI_TCXO */
  {    5000000, &ClockSource_XO,                          { 0,     2,     25,    96,    0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = BI_TCXO */
  {   19200000, &ClockSource_XO,                          { 0,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = BI_TCXO */
  {   25000000, &ClockSource_GCC_GPLL0,                   { 6,     24,    0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[6] = GPLL0_OUT_EVEN */
  {   50000000, &ClockSource_GCC_GPLL0,                   { 6,     12,    0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[6] = GPLL0_OUT_EVEN */
  {  125000000, &ClockSource_GCC_GPLL7,                   { 3,     8,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS,        },                                                            /* Mux[3] = GPLL7_OUT_MAIN */
  {  250000000, &ClockSource_GCC_GPLL7,                   { 3,     4,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_NOM,        },                                                            /* Mux[3] = GPLL7_OUT_MAIN */
};

/*
 * ClockDomainBSP_GCC_GCCGP1
 *
 * Used by:
 *   ClockDomain_GCC_GCCGP1
 *   ClockDomain_GCC_GCCGP2
 *   ClockDomain_GCC_GCCGP3
 *   ClockDomain_GCC_GCCGP4
 *   ClockDomain_GCC_GCCGP5
 */
static ClockMuxConfigType ClockDomainBSP_GCC_GCCGP1[] =
{
  {   19200000, &ClockSource_XO,                          { 0,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = BI_TCXO */
  {   25000000, &ClockSource_GCC_GPLL0,                   { 6,     24,    0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[6] = GPLL0_OUT_EVEN */
  {   50000000, &ClockSource_GCC_GPLL0,                   { 6,     12,    0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[6] = GPLL0_OUT_EVEN */
  {  100000000, &ClockSource_GCC_GPLL0,                   { 1,     12,    0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS,        },                                                            /* Mux[1] = GPLL0_OUT_MAIN */
  {  200000000, &ClockSource_GCC_GPLL0,                   { 1,     6,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_NOM,        },                                                            /* Mux[1] = GPLL0_OUT_MAIN */
};

/*
 * ClockDomainBSP_GCC_GCCPCIE0AUX
 *
 * Used by:
 *   ClockDomain_GCC_GCCPCIE0AUX
 *   ClockDomain_GCC_GCCPCIE1AUX
 *   ClockDomain_GCC_GCCPCIE2AUX
 *   ClockDomain_GCC_GCCPCIE3AUX
 */
static ClockMuxConfigType ClockDomainBSP_GCC_GCCPCIE0AUX[] =
{
  {    9600000, &ClockSource_XO,                          { 0,     4,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = BI_TCXO */
  {   19200000, &ClockSource_XO,                          { 0,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS,        },                                                            /* Mux[0] = BI_TCXO */
};

/*
 * ClockDomainBSP_GCC_GCCPCIEPHYREFGEN
 *
 * Used by:
 *   ClockDomain_GCC_GCCPCIEPHYREFGEN
 */
static ClockMuxConfigType ClockDomainBSP_GCC_GCCPCIEPHYREFGEN[] =
{
  {   19200000, &ClockSource_XO,                          { 0,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = BI_TCXO */
  {  100000000, &ClockSource_GCC_GPLL0,                   { 1,     12,    0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS,        },                                                            /* Mux[1] = GPLL0_OUT_MAIN */
};

/*
 * ClockDomainBSP_GCC_GCCPDM2
 *
 * Used by:
 *   ClockDomain_GCC_GCCPDM2
 */
static ClockMuxConfigType ClockDomainBSP_GCC_GCCPDM2[] =
{
  {    9600000, &ClockSource_XO,                          { 0,     4,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = BI_TCXO */
  {   19200000, &ClockSource_XO,                          { 0,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[0] = BI_TCXO */
  {   60000000, &ClockSource_GCC_GPLL0,                   { 1,     20,    0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS,        },                                                            /* Mux[1] = GPLL0_OUT_MAIN */
};

/*
 * ClockDomainBSP_GCC_GCCQSPI1CORE
 *
 * Used by:
 *   ClockDomain_GCC_GCCQSPI1CORE
 *   ClockDomain_GCC_GCCQSPICORE
 */
static ClockMuxConfigType ClockDomainBSP_GCC_GCCQSPI1CORE[] =
{
  {   19200000, &ClockSource_XO,                          { 0,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = BI_TCXO */
  {   75000000, &ClockSource_GCC_GPLL0,                   { 6,     8,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[6] = GPLL0_OUT_EVEN */
  {  150000000, &ClockSource_GCC_GPLL0,                   { 1,     8,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS,        },                                                            /* Mux[1] = GPLL0_OUT_MAIN */
  {  300000000, &ClockSource_GCC_GPLL0,                   { 1,     4,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_NOM,        },                                                            /* Mux[1] = GPLL0_OUT_MAIN */
};

/*
 * ClockDomainBSP_GCC_GCCQUPV3WRAP0S0
 *
 * Note: These frequencies are used by domains possibly under DFS control. The
 *       DFS-enable state will be detected during init. If under DFS control,
 *       the domain will dynamically allocate its own BSP array and populate
 *       it with configurations detected from the DFS registers.
 *
 * Used by:
 *   ClockDomain_GCC_GCCQUPV3WRAP0S0
 *   ClockDomain_GCC_GCCQUPV3WRAP0S1
 *   ClockDomain_GCC_GCCQUPV3WRAP0S2
 *   ClockDomain_GCC_GCCQUPV3WRAP0S3
 *   ClockDomain_GCC_GCCQUPV3WRAP0S4
 *   ClockDomain_GCC_GCCQUPV3WRAP0S5
 *   ClockDomain_GCC_GCCQUPV3WRAP0S6
 *   ClockDomain_GCC_GCCQUPV3WRAP0S7
 *   ClockDomain_GCC_GCCQUPV3WRAP1S0
 *   ClockDomain_GCC_GCCQUPV3WRAP1S1
 *   ClockDomain_GCC_GCCQUPV3WRAP1S2
 *   ClockDomain_GCC_GCCQUPV3WRAP1S3
 *   ClockDomain_GCC_GCCQUPV3WRAP1S4
 *   ClockDomain_GCC_GCCQUPV3WRAP1S5
 *   ClockDomain_GCC_GCCQUPV3WRAP2S0
 *   ClockDomain_GCC_GCCQUPV3WRAP2S1
 *   ClockDomain_GCC_GCCQUPV3WRAP2S2
 *   ClockDomain_GCC_GCCQUPV3WRAP2S3
 *   ClockDomain_GCC_GCCQUPV3WRAP2S4
 *   ClockDomain_GCC_GCCQUPV3WRAP2S5
 */
static ClockMuxConfigType ClockDomainBSP_GCC_GCCQUPV3WRAP0S0[] =
{
  {    7372800, &ClockSource_GCC_GPLL0,                   { 6,     2,     384,   15625, 0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[6] = GPLL0_OUT_EVEN */
  {   14745600, &ClockSource_GCC_GPLL0,                   { 6,     2,     768,   15625, 0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[6] = GPLL0_OUT_EVEN */
  {   19200000, &ClockSource_XO,                          { 0,     2,     0,     0,     0x00 }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = BI_TCXO */
  {   29491200, &ClockSource_GCC_GPLL0,                   { 6,     2,     1536,  15625, 0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[6] = GPLL0_OUT_EVEN */
  {   32000000, &ClockSource_GCC_GPLL0,                   { 6,     2,     8,     75,    0x01 }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[6] = GPLL0_OUT_EVEN */
  {   48000000, &ClockSource_GCC_GPLL0,                   { 6,     2,     4,     25,    0x02 }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[6] = GPLL0_OUT_EVEN */
  {   50000000, &ClockSource_GCC_GPLL0,                   { 6,     12,    0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[6] = GPLL0_OUT_EVEN */
  {   64000000, &ClockSource_GCC_GPLL0,                   { 6,     2,     16,    75,    0x03 }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[6] = GPLL0_OUT_EVEN */
  {   75000000, &ClockSource_GCC_GPLL0,                   { 6,     8,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[6] = GPLL0_OUT_EVEN */
  {   80000000, &ClockSource_GCC_GPLL0,                   { 6,     2,     4,     15,    0xFF }, RAIL_VOLTAGE_LEVEL_SVS,        },                                                            /* Mux[6] = GPLL0_OUT_EVEN */
  {   96000000, &ClockSource_GCC_GPLL0,                   { 6,     2,     8,     25,    0x04 }, RAIL_VOLTAGE_LEVEL_SVS,        },                                                            /* Mux[6] = GPLL0_OUT_EVEN */
  {  100000000, &ClockSource_GCC_GPLL0,                   { 1,     12,    0,     0,     0x05 }, RAIL_VOLTAGE_LEVEL_SVS,        },                                                            /* Mux[1] = GPLL0_OUT_MAIN */
  {  102400000, &ClockSource_GCC_GPLL0,                   { 6,     2,     128,   375,   0xFF }, RAIL_VOLTAGE_LEVEL_NOM,        },                                                            /* Mux[6] = GPLL0_OUT_EVEN */
  {  112000000, &ClockSource_GCC_GPLL0,                   { 6,     2,     28,    75,    0xFF }, RAIL_VOLTAGE_LEVEL_NOM,        },                                                            /* Mux[6] = GPLL0_OUT_EVEN */
  {  117964800, &ClockSource_GCC_GPLL0,                   { 6,     2,     6144,  15625, 0xFF }, RAIL_VOLTAGE_LEVEL_NOM,        },                                                            /* Mux[6] = GPLL0_OUT_EVEN */
  {  120000000, &ClockSource_GCC_GPLL0,                   { 6,     5,     0,     0,     0x06 }, RAIL_VOLTAGE_LEVEL_NOM,        },                                                            /* Mux[6] = GPLL0_OUT_EVEN */
  {  128000000, &ClockSource_GCC_GPLL0,                   { 1,     2,     16,    75,    0xFF }, RAIL_VOLTAGE_LEVEL_NOM,        },                                                            /* Mux[1] = GPLL0_OUT_MAIN */
};

/*
 * ClockDomainBSP_GCC_GCCSDCC2APPS
 *
 * Used by:
 *   ClockDomain_GCC_GCCSDCC2APPS
 */
static ClockMuxConfigType ClockDomainBSP_GCC_GCCSDCC2APPS[] =
{
  {     400000, &ClockSource_XO,                          { 0,     24,    1,     4,     0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = BI_TCXO */
  {    9600000, &ClockSource_XO,                          { 0,     4,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = BI_TCXO */
  {   19200000, &ClockSource_XO,                          { 0,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[0] = BI_TCXO */
  {   25000000, &ClockSource_GCC_GPLL0,                   { 1,     24,    1,     2,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS,        },                                                            /* Mux[1] = GPLL0_OUT_MAIN */
  {   50000000, &ClockSource_GCC_GPLL0,                   { 1,     24,    0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS,        },                                                            /* Mux[1] = GPLL0_OUT_MAIN */
  {  100000000, &ClockSource_GCC_GPLL0,                   { 1,     12,    0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS,        },                                                            /* Mux[1] = GPLL0_OUT_MAIN */
  {  200000000, &ClockSource_GCC_GPLL0,                   { 1,     6,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS_L1,     {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}, },     /* Mux[1] = GPLL0_OUT_MAIN */
  {  202000000, &ClockSource_GCC_GPLL9,                   { 2,     8,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS_L1,     {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}, },     /* Mux[2] = GPLL9_OUT_MAIN */
};

/*
 * ClockDomainBSP_GCC_GCCSDCC4APPS
 *
 * Used by:
 *   ClockDomain_GCC_GCCSDCC4APPS
 */
static ClockMuxConfigType ClockDomainBSP_GCC_GCCSDCC4APPS[] =
{
  {     400000, &ClockSource_XO,                          { 0,     24,    1,     4,     0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = BI_TCXO */
  {    9600000, &ClockSource_XO,                          { 0,     4,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = BI_TCXO */
  {   19200000, &ClockSource_XO,                          { 0,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[0] = BI_TCXO */
  {   37500000, &ClockSource_GCC_GPLL0,                   { 1,     32,    0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS,        {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}, },     /* Mux[1] = GPLL0_OUT_MAIN */
  {   50000000, &ClockSource_GCC_GPLL0,                   { 1,     24,    0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS,        {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}, },     /* Mux[1] = GPLL0_OUT_MAIN */
  {   50000000, &ClockSource_GCC_GPLL0,                   { 1,     24,    0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS_L1,     {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}, },     /* Mux[1] = GPLL0_OUT_MAIN */
  {   75000000, &ClockSource_GCC_GPLL0,                   { 1,     16,    0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_NOM,        {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}, },     /* Mux[1] = GPLL0_OUT_MAIN */
  {  100000000, &ClockSource_GCC_GPLL0,                   { 1,     12,    0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_NOM,        {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}, },     /* Mux[1] = GPLL0_OUT_MAIN */
};

/*
 * ClockDomainBSP_GCC_GCCTSIFREF
 *
 * Used by:
 *   ClockDomain_GCC_GCCTSIFREF
 */
static ClockMuxConfigType ClockDomainBSP_GCC_GCCTSIFREF[] =
{
  {     105495, &ClockSource_XO,                          { 0,     4,     1,     91,    0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = BI_TCXO */
};

/*
 * ClockDomainBSP_GCC_GCCUFSCARD2AXI
 *
 * Used by:
 *   ClockDomain_GCC_GCCUFSCARD2AXI
 *   ClockDomain_GCC_GCCUFSCARD2ICECORE
 *   ClockDomain_GCC_GCCUFSCARD2UNIPROCORE
 *   ClockDomain_GCC_GCCUFSPHYICECORE
 *   ClockDomain_GCC_GCCUFSPHYUNIPROCORE
 */
static ClockMuxConfigType ClockDomainBSP_GCC_GCCUFSCARD2AXI[] =
{
  {   37500000, &ClockSource_GCC_GPLL0,                   { 6,     16,    0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[6] = GPLL0_OUT_EVEN */
  {   75000000, &ClockSource_GCC_GPLL0,                   { 6,     8,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[6] = GPLL0_OUT_EVEN */
  {  150000000, &ClockSource_GCC_GPLL0,                   { 1,     8,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS,        },                                                            /* Mux[1] = GPLL0_OUT_MAIN */
  {  300000000, &ClockSource_GCC_GPLL0,                   { 1,     4,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_NOM,        },                                                            /* Mux[1] = GPLL0_OUT_MAIN */
};

/*
 * ClockDomainBSP_GCC_GCCUFSCARD2PHYAUX
 *
 * Used by:
 *   ClockDomain_GCC_GCCUFSCARD2PHYAUX
 *   ClockDomain_GCC_GCCUFSCARDPHYAUX
 *   ClockDomain_GCC_GCCUSB3MPPHYAUX
 *   ClockDomain_GCC_GCCUSB3PRIMPHYAUX
 *   ClockDomain_GCC_GCCUSB3SECPHYAUX
 */
static ClockMuxConfigType ClockDomainBSP_GCC_GCCUFSCARD2PHYAUX[] =
{
  {   19200000, &ClockSource_XO,                          { 0,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = BI_TCXO */
};

/*
 * ClockDomainBSP_GCC_GCCUFSCARDAXI
 *
 * Used by:
 *   ClockDomain_GCC_GCCUFSCARDAXI
 */
static ClockMuxConfigType ClockDomainBSP_GCC_GCCUFSCARDAXI[] =
{
  {   25000000, &ClockSource_GCC_GPLL0,                   { 6,     24,    0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}, },     /* Mux[6] = GPLL0_OUT_EVEN */
  {   50000000, &ClockSource_GCC_GPLL0,                   { 6,     12,    0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}, },     /* Mux[6] = GPLL0_OUT_EVEN */
  {  100000000, &ClockSource_GCC_GPLL0,                   { 1,     12,    0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS,        {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}, },     /* Mux[1] = GPLL0_OUT_MAIN */
  {  200000000, &ClockSource_GCC_GPLL0,                   { 1,     6,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_NOM,        {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}, },     /* Mux[1] = GPLL0_OUT_MAIN */
  {  240000000, &ClockSource_GCC_GPLL0,                   { 1,     5,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_TUR,        {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}, },     /* Mux[1] = GPLL0_OUT_MAIN */
};

/*
 * ClockDomainBSP_GCC_GCCUFSCARDICECORE
 *
 * Used by:
 *   ClockDomain_GCC_GCCUFSCARDICECORE
 */
static ClockMuxConfigType ClockDomainBSP_GCC_GCCUFSCARDICECORE[] =
{
  {   75000000, &ClockSource_GCC_GPLL0,                   { 6,     8,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}, },     /* Mux[6] = GPLL0_OUT_EVEN */
  {  150000000, &ClockSource_GCC_GPLL0,                   { 1,     8,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS,        {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}, },     /* Mux[1] = GPLL0_OUT_MAIN */
  {  300000000, &ClockSource_GCC_GPLL0,                   { 1,     4,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_NOM,        {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}, },     /* Mux[1] = GPLL0_OUT_MAIN */
};

/*
 * ClockDomainBSP_GCC_GCCUFSCARDUNIPROCORE
 *
 * Used by:
 *   ClockDomain_GCC_GCCUFSCARDUNIPROCORE
 */
static ClockMuxConfigType ClockDomainBSP_GCC_GCCUFSCARDUNIPROCORE[] =
{
  {   37500000, &ClockSource_GCC_GPLL0,                   { 6,     16,    0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}, },     /* Mux[6] = GPLL0_OUT_EVEN */
  {   75000000, &ClockSource_GCC_GPLL0,                   { 1,     16,    0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS,        {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}, },     /* Mux[1] = GPLL0_OUT_MAIN */
  {  150000000, &ClockSource_GCC_GPLL0,                   { 1,     8,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_NOM,        {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}, },     /* Mux[1] = GPLL0_OUT_MAIN */
};

/*
 * ClockDomainBSP_GCC_GCCUFSPHYAXI
 *
 * Used by:
 *   ClockDomain_GCC_GCCUFSPHYAXI
 */
static ClockMuxConfigType ClockDomainBSP_GCC_GCCUFSPHYAXI[] =
{
  {   25000000, &ClockSource_GCC_GPLL0,                   { 6,     24,    0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[6] = GPLL0_OUT_EVEN */
  {   37500000, &ClockSource_GCC_GPLL0,                   { 6,     16,    0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[6] = GPLL0_OUT_EVEN */
  {   75000000, &ClockSource_GCC_GPLL0,                   { 6,     8,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[6] = GPLL0_OUT_EVEN */
  {  150000000, &ClockSource_GCC_GPLL0,                   { 1,     8,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS,        },                                                            /* Mux[1] = GPLL0_OUT_MAIN */
  {  300000000, &ClockSource_GCC_GPLL0,                   { 1,     4,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_NOM,        },                                                            /* Mux[1] = GPLL0_OUT_MAIN */
};

/*
 * ClockDomainBSP_GCC_GCCUFSPHYPHYAUX
 *
 * Used by:
 *   ClockDomain_GCC_GCCUFSPHYPHYAUX
 */
static ClockMuxConfigType ClockDomainBSP_GCC_GCCUFSPHYPHYAUX[] =
{
  {    9600000, &ClockSource_XO,                          { 0,     4,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = BI_TCXO */
  {   19200000, &ClockSource_XO,                          { 0,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = BI_TCXO */
};

/*
 * ClockDomainBSP_GCC_GCCUSB30MPMASTER
 *
 * Used by:
 *   ClockDomain_GCC_GCCUSB30MPMASTER
 *   ClockDomain_GCC_GCCUSB30PRIMMASTER
 *   ClockDomain_GCC_GCCUSB30SECMASTER
 */
static ClockMuxConfigType ClockDomainBSP_GCC_GCCUSB30MPMASTER[] =
{
  {   33333333, &ClockSource_GCC_GPLL0,                   { 6,     18,    0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[6] = GPLL0_OUT_EVEN */
  {   66666667, &ClockSource_GCC_GPLL0,                   { 6,     9,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[6] = GPLL0_OUT_EVEN */
  {  133333333, &ClockSource_GCC_GPLL0,                   { 1,     9,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS,        },                                                            /* Mux[1] = GPLL0_OUT_MAIN */
  {  200000000, &ClockSource_GCC_GPLL0,                   { 1,     6,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_NOM,        },                                                            /* Mux[1] = GPLL0_OUT_MAIN */
  {  240000000, &ClockSource_GCC_GPLL0,                   { 1,     5,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_TUR,        },                                                            /* Mux[1] = GPLL0_OUT_MAIN */
};

/*
 * ClockDomainBSP_GCC_GCCUSB30MPMOCKUTMI
 *
 * Used by:
 *   ClockDomain_GCC_GCCUSB30MPMOCKUTMI
 *   ClockDomain_GCC_GCCUSB30PRIMMOCKUTMI
 *   ClockDomain_GCC_GCCUSB30SECMOCKUTMI
 */
static ClockMuxConfigType ClockDomainBSP_GCC_GCCUSB30MPMOCKUTMI[] =
{
  {   19200000, &ClockSource_XO,                          { 0,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = BI_TCXO */
  {   20000000, &ClockSource_GCC_GPLL0,                   { 6,     30,    0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[6] = GPLL0_OUT_EVEN */
  {   40000000, &ClockSource_GCC_GPLL0,                   { 6,     15,    0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[6] = GPLL0_OUT_EVEN */
  {   60000000, &ClockSource_GCC_GPLL0,                   { 1,     20,    0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS,        },                                                            /* Mux[1] = GPLL0_OUT_MAIN */
};

/*
 * ClockDomainBSP_GPUCC_GPUCCGMU
 *
 * Used by:
 *   ClockDomain_GPUCC_GPUCCGMU
 */
static ClockMuxConfigType ClockDomainBSP_GPUCC_GPUCCGMU[] =
{
  {   19200000, &ClockSource_XO,                          { 0,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = BI_TCXO */
  {  200000000, NULL,                                     { 6,     3,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[6] = GPLL0_OUT_MAIN_DIV */
  {  400000000, &ClockSource_GCC_GPLL0,                   { 5,     3,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS,        {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}, },     /* Mux[5] = GPLL0_OUT_MAIN */
  {  500000000, &ClockSource_GPUCC_GPUCCPLL1,             { 3,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS,        {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}, },     /* Mux[3] = GPU_CC_PLL1_OUT_MAIN */
  {  500000000, &ClockSource_GPUCC_GPUCCPLL1,             { 3,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS_L1,     {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}, },     /* Mux[3] = GPU_CC_PLL1_OUT_MAIN */
};

/*
 * ClockDomainBSP_GPUCC_GPUCCGXGFX3D
 *
 * Used by:
 *   ClockDomain_GPUCC_GPUCCGXGFX3D
 */
static ClockMuxConfigType ClockDomainBSP_GPUCC_GPUCCGXGFX3D[] =
{
  {  116000000, &ClockSource_GPUCC_GPUCCPLL0,             { 1,     4,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}, &ClockSourceBSP_GPUCC_GPUCCPLL0[ 2] /* 464.0  */ },  /* Mux[1] = GPU_CC_PLL0_OUT_EVEN */
  {  156000000, &ClockSource_GPUCC_GPUCCPLL0,             { 1,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}, &ClockSourceBSP_GPUCC_GPUCCPLL0[ 0] /* 312.0  */ },  /* Mux[1] = GPU_CC_PLL0_OUT_EVEN */
  {  177000000, &ClockSource_GPUCC_GPUCCPLL0,             { 1,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}, &ClockSourceBSP_GPUCC_GPUCCPLL0[ 1] /* 354.0  */ },  /* Mux[1] = GPU_CC_PLL0_OUT_EVEN */
  {  235000000, &ClockSource_GPUCC_GPUCCPLL0,             { 1,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}, &ClockSourceBSP_GPUCC_GPUCCPLL0[ 3] /* 470.0  */ },  /* Mux[1] = GPU_CC_PLL0_OUT_EVEN */
  {  256000000, &ClockSource_GPUCC_GPUCCPLL0,             { 1,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS,        {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}, &ClockSourceBSP_GPUCC_GPUCCPLL0[ 4] /* 512.0  */ },  /* Mux[1] = GPU_CC_PLL0_OUT_EVEN */
  {  315000000, &ClockSource_GPUCC_GPUCCPLL0,             { 1,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS,        {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}, &ClockSourceBSP_GPUCC_GPUCCPLL0[ 5] /* 630.0  */ },  /* Mux[1] = GPU_CC_PLL0_OUT_EVEN */
  {  315000000, &ClockSource_GPUCC_GPUCCPLL0,             { 1,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS_L1,     {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}, &ClockSourceBSP_GPUCC_GPUCCPLL0[ 5] /* 630.0  */ },  /* Mux[1] = GPU_CC_PLL0_OUT_EVEN */
  {  392000000, &ClockSource_GPUCC_GPUCCPLL0,             { 1,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS_L1,     {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}, &ClockSourceBSP_GPUCC_GPUCCPLL0[ 6] /* 784.0  */ },  /* Mux[1] = GPU_CC_PLL0_OUT_EVEN */
  {  405000000, &ClockSource_GPUCC_GPUCCPLL0,             { 1,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_NOM,        {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}, &ClockSourceBSP_GPUCC_GPUCCPLL0[ 7] /* 810.0  */ },  /* Mux[1] = GPU_CC_PLL0_OUT_EVEN */
  {  461000000, &ClockSource_GPUCC_GPUCCPLL0,             { 1,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_NOM_L1,     {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}, &ClockSourceBSP_GPUCC_GPUCCPLL0[ 8] /* 922.0  */ },  /* Mux[1] = GPU_CC_PLL0_OUT_EVEN */
  {  500000000, &ClockSource_GPUCC_GPUCCPLL0,             { 1,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_TUR,        {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}, &ClockSourceBSP_GPUCC_GPUCCPLL0[ 9] /* 1000.0 */ },  /* Mux[1] = GPU_CC_PLL0_OUT_EVEN */
  {  514000000, &ClockSource_GPUCC_GPUCCPLL0,             { 1,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_TUR_L1,     {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}, &ClockSourceBSP_GPUCC_GPUCCPLL0[10] /* 1028.0 */ },  /* Mux[1] = GPU_CC_PLL0_OUT_EVEN */
  {  530000000, &ClockSource_GPUCC_GPUCCPLL0,             { 1,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_NOM,        {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}, &ClockSourceBSP_GPUCC_GPUCCPLL0[11] /* 1060.0 */ },  /* Mux[1] = GPU_CC_PLL0_OUT_EVEN */
  {  595000000, &ClockSource_GPUCC_GPUCCPLL0,             { 1,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_NOM_L1,     {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}, &ClockSourceBSP_GPUCC_GPUCCPLL0[12] /* 1190.0 */ },  /* Mux[1] = GPU_CC_PLL0_OUT_EVEN */
  {  625000000, &ClockSource_GPUCC_GPUCCPLL0,             { 1,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_TUR,        {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}, &ClockSourceBSP_GPUCC_GPUCCPLL0[13] /* 1250.0 */ },  /* Mux[1] = GPU_CC_PLL0_OUT_EVEN */
  {  670000000, &ClockSource_GPUCC_GPUCCPLL0,             { 1,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_TUR_L1,     {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}, &ClockSourceBSP_GPUCC_GPUCCPLL0[14] /* 1340.0 */ },  /* Mux[1] = GPU_CC_PLL0_OUT_EVEN */
};

/*
 * ClockDomainBSP_NPUCC_NPUCCCALDP
 *
 * Used by:
 *   ClockDomain_NPUCC_NPUCCCALDP
 */
static ClockMuxConfigType ClockDomainBSP_NPUCC_NPUCCCALDP[] =
{
  {    9600000, &ClockSource_XO,                          { 0,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}, },     /* Mux[0] = BI_TCXO */
  {  125000000, &ClockSource_NPUCC_NPUCCPLL0,             { 2,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}, &ClockSourceBSP_NPUCC_NPUCCPLL0[ 0] /* 250.0  */ },  /* Mux[2] = NPU_CC_PLL0_OUT_EVEN */
  {  300000000, &ClockSource_NPUCC_NPUCCPLL0,             { 2,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    {{0}}, &ClockSourceBSP_NPUCC_NPUCCPLL0[ 1] /* 600.0  */ },    /* Mux[2] = NPU_CC_PLL0_OUT_EVEN */
  {  350000000, &ClockSource_NPUCC_NPUCCPLL0,             { 2,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS,        {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}, &ClockSourceBSP_NPUCC_NPUCCPLL0[ 2] /* 700.0  */ },  /* Mux[2] = NPU_CC_PLL0_OUT_EVEN */
  {  400000000, &ClockSource_NPUCC_NPUCCPLL0,             { 2,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS,        {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}, &ClockSourceBSP_NPUCC_NPUCCPLL0[ 3] /* 800.0  */ },  /* Mux[2] = NPU_CC_PLL0_OUT_EVEN */
  {  400000000, &ClockSource_NPUCC_NPUCCPLL0,             { 2,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS_L1,     {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}, &ClockSourceBSP_NPUCC_NPUCCPLL0[ 3] /* 800.0  */ },  /* Mux[2] = NPU_CC_PLL0_OUT_EVEN */
  {  487000000, &ClockSource_NPUCC_NPUCCPLL0,             { 2,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS_L1,     {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}, &ClockSourceBSP_NPUCC_NPUCCPLL0[ 4] /* 974.0  */ },  /* Mux[2] = NPU_CC_PLL0_OUT_EVEN */
  {  600000000, &ClockSource_NPUCC_NPUCCPLL0,             { 2,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_NOM,        {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}, &ClockSourceBSP_NPUCC_NPUCCPLL0[ 5] /* 1200.0 */ },  /* Mux[2] = NPU_CC_PLL0_OUT_EVEN */
  {  652000000, &ClockSource_NPUCC_NPUCCPLL0,             { 2,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_NOM,        {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}, &ClockSourceBSP_NPUCC_NPUCCPLL0[ 6] /* 1304.0 */ },  /* Mux[2] = NPU_CC_PLL0_OUT_EVEN */
  {  715000000, &ClockSource_NPUCC_NPUCCPLL0,             { 2,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_TUR,        {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}, &ClockSourceBSP_NPUCC_NPUCCPLL0[ 7] /* 1430.0 */ },  /* Mux[2] = NPU_CC_PLL0_OUT_EVEN */
  {  773000000, &ClockSource_NPUCC_NPUCCPLL0,             { 2,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_NOM_L1,     {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}, &ClockSourceBSP_NPUCC_NPUCCPLL0[ 8] /* 1546.0 */ },  /* Mux[2] = NPU_CC_PLL0_OUT_EVEN */
  {  811000000, &ClockSource_NPUCC_NPUCCPLL0,             { 2,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_TUR,        {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}, &ClockSourceBSP_NPUCC_NPUCCPLL0[ 9] /* 1622.0 */ },  /* Mux[2] = NPU_CC_PLL0_OUT_EVEN */
  {  908000000, &ClockSource_NPUCC_NPUCCPLL0,             { 2,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_TUR_L1,     {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}, &ClockSourceBSP_NPUCC_NPUCCPLL0[10] /* 1816.0 */ },  /* Mux[2] = NPU_CC_PLL0_OUT_EVEN */
};

/*
 * ClockDomainBSP_NPUCC_NPUCCNPUCORE
 *
 * Used by:
 *   ClockDomain_NPUCC_NPUCCNPUCORE
 */
static ClockMuxConfigType ClockDomainBSP_NPUCC_NPUCCNPUCORE[] =
{
  {   19200000, &ClockSource_XO,                          { 0,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}, },     /* Mux[0] = BI_TCXO */
  {   60000000, NULL,                                     { 5,     10,    0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}, },     /* Mux[5] = GPLL0_OUT_MAIN_DIV */
  {  100000000, &ClockSource_GCC_GPLL0,                   { 4,     12,    0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[4] = GPLL0_OUT_MAIN */
  {  150000000, &ClockSource_GCC_GPLL0,                   { 4,     8,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS,        },                                                            /* Mux[4] = GPLL0_OUT_MAIN */
  {  200000000, &ClockSource_GCC_GPLL0,                   { 4,     6,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS_L1,     {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}, },     /* Mux[4] = GPLL0_OUT_MAIN */
  {  200000000, &ClockSource_NPUCC_NPUCCPLL1,             { 1,     8,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS_L1,     {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}, },     /* Mux[1] = NPU_CC_PLL1_OUT_EVEN */
  {  300000000, &ClockSource_GCC_GPLL0,                   { 4,     4,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_NOM,        },                                                            /* Mux[4] = GPLL0_OUT_MAIN */
  {  350000000, &ClockSource_NPUCC_NPUCCPLL1,             { 1,     4,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_TUR,        {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}, },     /* Mux[1] = NPU_CC_PLL1_OUT_EVEN */
  {  400000000, &ClockSource_NPUCC_NPUCCPLL1,             { 1,     4,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_TUR,        {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}, },     /* Mux[1] = NPU_CC_PLL1_OUT_EVEN */
};
#ifdef ENABLE_AUTO_PLAT
/*
 * ClockDomainBSP_SCC_SCCMAINRCG
 *
 * Used by:
 *   ClockDomain_SCC_SCCMAINRCG
 */
static ClockMuxConfigType ClockDomainBSP_SCC_SCCMAINRCG[] =
{
  {   96000000, &ClockSource_SCC_SCCPLL,                  { 2,     6,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}, },       /* Mux[2] = SCC_PLL_OUT_EVEN */
  {  100000000, &ClockSource_SCC_SCCPLL,                  { 2,     6,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}, },       /* Mux[2] = SCC_PLL_OUT_EVEN */
};

/*
 * ClockDomainBSP_SCC_SCCQUPV3SE0
 *
 * Note: These frequencies are used by domains possibly under DFS control. The
 *       DFS-enable state will be detected during init. If under DFS control,
 *       the domain will dynamically allocate its own BSP array and populate
 *       it with configurations detected from the DFS registers.
 *
 * Used by:
 *   ClockDomain_SCC_SCCQUPV3SE0
 *   ClockDomain_SCC_SCCQUPV3SE1
 *   ClockDomain_SCC_SCCQUPV3SE2
 *   ClockDomain_SCC_SCCQUPV3SE3
 *   ClockDomain_SCC_SCCQUPV3SE4
 *   ClockDomain_SCC_SCCQUPV3SE5
 */
static ClockMuxConfigType ClockDomainBSP_SCC_SCCQUPV3SE0[] =
{
  {    7372800, &ClockSource_SCC_SCCPLL,                  { 2,     2,     16,    625,   0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}, },       /* Mux[2] = SCC_PLL_OUT_EVEN */
  {    7372800, &ClockSource_SCC_SCCPLL,                  { 2,     2,     384,   15625, 0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}, },       /* Mux[2] = SCC_PLL_OUT_EVEN */
  {   14745600, &ClockSource_SCC_SCCPLL,                  { 2,     2,     32,    625,   0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}, },       /* Mux[2] = SCC_PLL_OUT_EVEN */
  {   14745600, &ClockSource_SCC_SCCPLL,                  { 2,     2,     768,   15625, 0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}, },       /* Mux[2] = SCC_PLL_OUT_EVEN */
  {   19200000, &ClockSource_XO,                          { 0,     2,     0,     0,     0x00 }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = AOSS_CC_RO_CLK */
  {   29491200, &ClockSource_SCC_SCCPLL,                  { 2,     2,     64,    625,   0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}, },       /* Mux[2] = SCC_PLL_OUT_EVEN */
  {   29491200, &ClockSource_SCC_SCCPLL,                  { 2,     2,     1536,  15625, 0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}, },       /* Mux[2] = SCC_PLL_OUT_EVEN */
  {   32000000, &ClockSource_SCC_SCCPLL,                  { 2,     18,    0,     0,     0x01 }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}, },       /* Mux[2] = SCC_PLL_OUT_EVEN */
  {   32000000, &ClockSource_SCC_SCCPLL,                  { 2,     2,     8,     75,    0x01 }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}, },       /* Mux[2] = SCC_PLL_OUT_EVEN */
  {   48000000, &ClockSource_SCC_SCCPLL,                  { 2,     12,    0,     0,     0x02 }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}, },       /* Mux[2] = SCC_PLL_OUT_EVEN */
  {   48000000, &ClockSource_SCC_SCCPLL,                  { 2,     2,     4,     25,    0x02 }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}, },       /* Mux[2] = SCC_PLL_OUT_EVEN */
  {   64000000, &ClockSource_SCC_SCCPLL,                  { 2,     9,     0,     0,     0x03 }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}, },       /* Mux[2] = SCC_PLL_OUT_EVEN */
  {   64000000, &ClockSource_SCC_SCCPLL,                  { 2,     2,     16,    75,    0x03 }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}, },       /* Mux[2] = SCC_PLL_OUT_EVEN */
  {   80000000, &ClockSource_SCC_SCCPLL,                  { 6,     15,    0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}, },       /* Mux[6] = SCC_PLL_OUT_MAIN */
  {   96000000, &ClockSource_SCC_SCCPLL,                  { 2,     2,     8,     25,    0x04 }, RAIL_VOLTAGE_LEVEL_SVS,        {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}, },       /* Mux[2] = SCC_PLL_OUT_EVEN */
  {   96000000, &ClockSource_SCC_SCCPLL,                  { 6,     12,    0,     0,     0x04 }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}, },       /* Mux[6] = SCC_PLL_OUT_MAIN */
  {  100000000, &ClockSource_SCC_SCCPLL,                  { 2,     6,     0,     0,     0x05 }, RAIL_VOLTAGE_LEVEL_SVS,        {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}, },       /* Mux[2] = SCC_PLL_OUT_EVEN */
  {  100000000, &ClockSource_SCC_SCCPLL,                  { 6,     2,     25,    144,   0x05 }, RAIL_VOLTAGE_LEVEL_SVS,        {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}, },       /* Mux[6] = SCC_PLL_OUT_MAIN */
  {  102400000, &ClockSource_SCC_SCCPLL,                  { 2,     2,     128,   375,   0xFF }, RAIL_VOLTAGE_LEVEL_SVS,        {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}, },       /* Mux[2] = SCC_PLL_OUT_EVEN */
  {  102400000, &ClockSource_SCC_SCCPLL,                  { 6,     2,     8,     45,    0xFF }, RAIL_VOLTAGE_LEVEL_SVS,        {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}, },       /* Mux[6] = SCC_PLL_OUT_MAIN */
  {  112000000, &ClockSource_SCC_SCCPLL,                  { 2,     2,     28,    75,    0xFF }, RAIL_VOLTAGE_LEVEL_SVS,        {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}, },       /* Mux[2] = SCC_PLL_OUT_EVEN */
  {  112000000, &ClockSource_SCC_SCCPLL,                  { 6,     2,     7,     36,    0xFF }, RAIL_VOLTAGE_LEVEL_SVS,        {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}, },       /* Mux[6] = SCC_PLL_OUT_MAIN */
  {  117964800, &ClockSource_SCC_SCCPLL,                  { 2,     2,     6144,  15625, 0xFF }, RAIL_VOLTAGE_LEVEL_SVS,        {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}, },       /* Mux[2] = SCC_PLL_OUT_EVEN */
  {  117964800, &ClockSource_SCC_SCCPLL,                  { 6,     2,     128,   625,   0xFF }, RAIL_VOLTAGE_LEVEL_SVS,        {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}, },       /* Mux[6] = SCC_PLL_OUT_MAIN */
  {  120000000, &ClockSource_SCC_SCCPLL,                  { 2,     5,     0,     0,     0x06 }, RAIL_VOLTAGE_LEVEL_SVS,        {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}, },       /* Mux[2] = SCC_PLL_OUT_EVEN */
  {  120000000, &ClockSource_SCC_SCCPLL,                  { 6,     2,     5,     24,    0x06 }, RAIL_VOLTAGE_LEVEL_SVS,        {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}, },       /* Mux[6] = SCC_PLL_OUT_MAIN */
  {  128000000, &ClockSource_SCC_SCCPLL,                  { 2,     2,     32,    75,    0xFF }, RAIL_VOLTAGE_LEVEL_NOM,        {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}, },       /* Mux[2] = SCC_PLL_OUT_EVEN */
  {  128000000, &ClockSource_SCC_SCCPLL,                  { 6,     9,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS,        {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}, },       /* Mux[6] = SCC_PLL_OUT_MAIN */
  {  144000000, &ClockSource_SCC_SCCPLL,                  { 6,     8,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS_L1,     {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}, },       /* Mux[6] = SCC_PLL_OUT_MAIN */
  {  192000000, &ClockSource_SCC_SCCPLL,                  { 6,     6,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_NOM,        {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}, },       /* Mux[6] = SCC_PLL_OUT_MAIN */
};
#endif
/*
 * ClockDomainBSP_VIDEOCC_VIDEOCCIRIS
 *
 * Used by:
 *   ClockDomain_VIDEOCC_VIDEOCCIRIS
 */
static ClockMuxConfigType ClockDomainBSP_VIDEOCC_VIDEOCCIRIS[] =
{
  {  200000000, &ClockSource_VIDEOCC_VIDEOPLL0,           { 1,     4,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    {{0}}, &ClockSourceBSP_VIDEOCC_VIDEOPLL0[ 0] /* 400.0  */ },  /* Mux[1] = VIDEO_PLL0_OUT_MAIN */
  {  225000000, &ClockSource_VIDEOCC_VIDEOPLL0,           { 1,     4,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}, &ClockSourceBSP_VIDEOCC_VIDEOPLL0[ 1] /* 450.0  */ },  /* Mux[1] = VIDEO_PLL0_OUT_MAIN */
  {  240000000, &ClockSource_VIDEOCC_VIDEOPLL0,           { 1,     4,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}, &ClockSourceBSP_VIDEOCC_VIDEOPLL0[ 2] /* 480.0  */ },  /* Mux[1] = VIDEO_PLL0_OUT_MAIN */
  {  300000000, &ClockSource_VIDEOCC_VIDEOPLL0,           { 1,     4,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS,        {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}, &ClockSourceBSP_VIDEOCC_VIDEOPLL0[ 3] /* 600.0  */ },  /* Mux[1] = VIDEO_PLL0_OUT_MAIN */
  {  338000000, &ClockSource_VIDEOCC_VIDEOPLL0,           { 1,     4,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS,        {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}, &ClockSourceBSP_VIDEOCC_VIDEOPLL0[ 4] /* 676.0  */ },  /* Mux[1] = VIDEO_PLL0_OUT_MAIN */
  {  365000000, &ClockSource_VIDEOCC_VIDEOPLL0,           { 1,     4,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS_L1,     {{0}}, &ClockSourceBSP_VIDEOCC_VIDEOPLL0[ 5] /* 730.0  */ },  /* Mux[1] = VIDEO_PLL0_OUT_MAIN */
  {  432000000, &ClockSource_VIDEOCC_VIDEOPLL0,           { 1,     4,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_NOM,        {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}, &ClockSourceBSP_VIDEOCC_VIDEOPLL0[ 6] /* 864.0  */ },  /* Mux[1] = VIDEO_PLL0_OUT_MAIN */
  {  444000000, &ClockSource_VIDEOCC_VIDEOPLL0,           { 1,     4,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_NOM,        {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}, &ClockSourceBSP_VIDEOCC_VIDEOPLL0[ 7] /* 888.0  */ },  /* Mux[1] = VIDEO_PLL0_OUT_MAIN */
  {  480000000, &ClockSource_VIDEOCC_VIDEOPLL0,           { 1,     4,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_TUR,        {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}, &ClockSourceBSP_VIDEOCC_VIDEOPLL0[ 8] /* 960.0  */ },  /* Mux[1] = VIDEO_PLL0_OUT_MAIN */
  {  533000000, &ClockSource_VIDEOCC_VIDEOPLL0,           { 1,     4,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_TUR,        {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}, &ClockSourceBSP_VIDEOCC_VIDEOPLL0[ 9] /* 1066.0 */ },  /* Mux[1] = VIDEO_PLL0_OUT_MAIN */
};

/*
 * ClockDomainBSP_VIDEOCC_VIDEOCCSLEEP
 *
 * Used by:
 *   ClockDomain_VIDEOCC_VIDEOCCSLEEP
 */
static ClockMuxConfigType ClockDomainBSP_VIDEOCC_VIDEOCCSLEEP[] =
{
  {      32000, NULL,                                     { 0,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}, },     /* Mux[0] = CHIP_SLEEP_CLK */
};

/*
 * ClockDomainBSP_VIDEOCC_VIDEOCCXO
 *
 * Used by:
 *   ClockDomain_VIDEOCC_VIDEOCCXO
 */
static ClockMuxConfigType ClockDomainBSP_VIDEOCC_VIDEOCCXO[] =
{
  {   19200000, &ClockSource_XO,                          { 0,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}, },     /* Mux[0] = BI_TCXO */
};

/*=========================================================================
      Domain Source Maps
==========================================================================*/

/*
 * ClockSourceMap_CAMCC_CAMCCBPS
 *
 * Used by:
 *   ClockDomain_CAMCC_CAMCCBPS
 *   ClockDomain_CAMCC_CAMCCCAMNOCAXI
 *   ClockDomain_CAMCC_CAMCCCCI0
 *   ClockDomain_CAMCC_CAMCCCCI1
 *   ClockDomain_CAMCC_CAMCCCCI2
 *   ClockDomain_CAMCC_CAMCCCCI3
 *   ClockDomain_CAMCC_CAMCCCPHYRX
 *   ClockDomain_CAMCC_CAMCCCSI0PHYTIMER
 *   ClockDomain_CAMCC_CAMCCCSI1PHYTIMER
 *   ClockDomain_CAMCC_CAMCCCSI2PHYTIMER
 *   ClockDomain_CAMCC_CAMCCCSI3PHYTIMER
 *   ClockDomain_CAMCC_CAMCCFASTAHB
 *   ClockDomain_CAMCC_CAMCCFDCORE
 *   ClockDomain_CAMCC_CAMCCICP
 *   ClockDomain_CAMCC_CAMCCIFE0CSID
 *   ClockDomain_CAMCC_CAMCCIFE1CSID
 *   ClockDomain_CAMCC_CAMCCIFE2CSID
 *   ClockDomain_CAMCC_CAMCCIFE3CSID
 *   ClockDomain_CAMCC_CAMCCIFELITE0
 *   ClockDomain_CAMCC_CAMCCIFELITE0CSID
 *   ClockDomain_CAMCC_CAMCCIFELITE1
 *   ClockDomain_CAMCC_CAMCCIFELITE1CSID
 *   ClockDomain_CAMCC_CAMCCIFELITE2
 *   ClockDomain_CAMCC_CAMCCIFELITE2CSID
 *   ClockDomain_CAMCC_CAMCCIFELITE3
 *   ClockDomain_CAMCC_CAMCCIFELITE3CSID
 *   ClockDomain_CAMCC_CAMCCJPEG
 *   ClockDomain_CAMCC_CAMCCLRME
 *   ClockDomain_CAMCC_CAMCCQDSSDEBUG
 *   ClockDomain_CAMCC_CAMCCSLOWAHB
 */
static const ClockSourceMapType ClockSourceMap_CAMCC_CAMCCBPS[] =
{
  { 0,   19200000, &ClockSource_XO,                         },
  { 1, 1200000000, &ClockSource_CAMCC_CAMCCPLL0,            },
  { 2,  600000000, &ClockSource_CAMCC_CAMCCPLL0,            },
  { 3,  400000000, &ClockSource_CAMCC_CAMCCPLL0,            },
  { 5,  480000000, &ClockSource_CAMCC_CAMCCPLL2,            },
  { 7,  400000000, NULL,                                    },
  { CLOCK_SOURCE_MAP_END }
};

/*
 * ClockSourceMap_CAMCC_CAMCCDEBUGMUX
 *
 * Used by:
 *   ClockDomain_CAMCC_CAMCCDEBUGMUX
 */
static const ClockSourceMapType ClockSourceMap_CAMCC_CAMCCDEBUGMUX[] =
{
  { CLOCK_SOURCE_MAP_END }
};

/*
 * ClockSourceMap_CAMCC_CAMCCIFE0
 *
 * Used by:
 *   ClockDomain_CAMCC_CAMCCIFE0
 */
static const ClockSourceMapType ClockSourceMap_CAMCC_CAMCCIFE0[] =
{
  { 0,   19200000, &ClockSource_XO,                         },
  { 6,          0, &ClockSource_CAMCC_CAMCCPLL3,            },
  { 7,  400000000, NULL,                                    },
  { CLOCK_SOURCE_MAP_END }
};

/*
 * ClockSourceMap_CAMCC_CAMCCIFE1
 *
 * Used by:
 *   ClockDomain_CAMCC_CAMCCIFE1
 */
static const ClockSourceMapType ClockSourceMap_CAMCC_CAMCCIFE1[] =
{
  { 0,   19200000, &ClockSource_XO,                         },
  { 6,          0, &ClockSource_CAMCC_CAMCCPLL4,            },
  { 7,  400000000, NULL,                                    },
  { CLOCK_SOURCE_MAP_END }
};

/*
 * ClockSourceMap_CAMCC_CAMCCIFE2
 *
 * Used by:
 *   ClockDomain_CAMCC_CAMCCIFE2
 */
static const ClockSourceMapType ClockSourceMap_CAMCC_CAMCCIFE2[] =
{
  { 0,   19200000, &ClockSource_XO,                         },
  { 6,          0, &ClockSource_CAMCC_CAMCCPLL5,            },
  { 7,  400000000, NULL,                                    },
  { CLOCK_SOURCE_MAP_END }
};

/*
 * ClockSourceMap_CAMCC_CAMCCIFE3
 *
 * Used by:
 *   ClockDomain_CAMCC_CAMCCIFE3
 */
static const ClockSourceMapType ClockSourceMap_CAMCC_CAMCCIFE3[] =
{
  { 0,   19200000, &ClockSource_XO,                         },
  { 6,          0, &ClockSource_CAMCC_CAMCCPLL6,            },
  { 7,  400000000, NULL,                                    },
  { CLOCK_SOURCE_MAP_END }
};

/*
 * ClockSourceMap_CAMCC_CAMCCIPE0
 *
 * Used by:
 *   ClockDomain_CAMCC_CAMCCIPE0
 */
static const ClockSourceMapType ClockSourceMap_CAMCC_CAMCCIPE0[] =
{
  { 0,   19200000, &ClockSource_XO,                         },
  { 4,          0, &ClockSource_CAMCC_CAMCCPLL1,            },
  { 7,  400000000, NULL,                                    },
  { CLOCK_SOURCE_MAP_END }
};

/*
 * ClockSourceMap_CAMCC_CAMCCMCLK0
 *
 * Used by:
 *   ClockDomain_CAMCC_CAMCCMCLK0
 *   ClockDomain_CAMCC_CAMCCMCLK1
 *   ClockDomain_CAMCC_CAMCCMCLK2
 *   ClockDomain_CAMCC_CAMCCMCLK3
 *   ClockDomain_CAMCC_CAMCCMCLK4
 *   ClockDomain_CAMCC_CAMCCMCLK5
 *   ClockDomain_CAMCC_CAMCCMCLK6
 *   ClockDomain_CAMCC_CAMCCMCLK7
 */
static const ClockSourceMapType ClockSourceMap_CAMCC_CAMCCMCLK0[] =
{
  { 0,   19200000, &ClockSource_XO,                         },
  { 5,  960000000, &ClockSource_CAMCC_CAMCCPLL2,            },
  { 7,  400000000, NULL,                                    },
  { CLOCK_SOURCE_MAP_END }
};

/*
 * ClockSourceMap_CAMCC_CAMCCPLLTESTMUX
 *
 * Used by:
 *   ClockDomain_CAMCC_CAMCCPLLTESTMUX
 */
static const ClockSourceMapType ClockSourceMap_CAMCC_CAMCCPLLTESTMUX[] =
{
  { 0, 1200000000, &ClockSource_CAMCC_CAMCCPLL0,            },
  { 1,          0, &ClockSource_CAMCC_CAMCCPLL1,            },
  { 2,  960000000, &ClockSource_CAMCC_CAMCCPLL2,            },
  { 3,          0, &ClockSource_CAMCC_CAMCCPLL3,            },
  { CLOCK_SOURCE_MAP_END }
};

/*
 * ClockSourceMap_CAMCC_CAMCCSLEEP
 *
 * Used by:
 *   ClockDomain_CAMCC_CAMCCSLEEP
 */
static const ClockSourceMapType ClockSourceMap_CAMCC_CAMCCSLEEP[] =
{
  { 0,      32000, NULL,                                    },
  { 7,  400000000, NULL,                                    },
  { CLOCK_SOURCE_MAP_END }
};

/*
 * ClockSourceMap_CAMCC_CAMCCXO
 *
 * Used by:
 *   ClockDomain_CAMCC_CAMCCXO
 */
static const ClockSourceMapType ClockSourceMap_CAMCC_CAMCCXO[] =
{
  { 0,   19200000, &ClockSource_XO,                         },
  { 7,  400000000, NULL,                                    },
  { CLOCK_SOURCE_MAP_END }
};

/*
 * ClockSourceMap_DISPCC_DISPCCMDSSAHB
 *
 * Used by:
 *   ClockDomain_DISPCC_DISPCCMDSSAHB
 *   ClockDomain_DISPCC_DISPCCMDSSEDPGTC
 */
static const ClockSourceMapType ClockSourceMap_DISPCC_DISPCCMDSSAHB[] =
{
  { 0,   19200000, &ClockSource_XO,                         },
  { 4,  600000000, &ClockSource_DISPCC_DISPCCPLL1,          },
  { 5,  600000000, &ClockSource_DISPCC_DISPCCPLL1,          },
  { 7,  400000000, NULL,                                    },
  { CLOCK_SOURCE_MAP_END }
};

/*
 * ClockSourceMap_DISPCC_DISPCCMDSSBYTE0
 *
 * Used by:
 *   ClockDomain_DISPCC_DISPCCMDSSBYTE0
 *   ClockDomain_DISPCC_DISPCCMDSSBYTE1
 *   ClockDomain_DISPCC_DISPCCMDSSESC0
 *   ClockDomain_DISPCC_DISPCCMDSSESC1
 */
static const ClockSourceMapType ClockSourceMap_DISPCC_DISPCCMDSSBYTE0[] =
{
  { 0,   19200000, &ClockSource_XO,                         },
  { 1,          0, NULL,                                    },
  { 2,          0, NULL,                                    },
  { 7,  400000000, NULL,                                    },
  { CLOCK_SOURCE_MAP_END }
};

/*
 * ClockSourceMap_DISPCC_DISPCCMDSSDPAUX1
 *
 * Used by:
 *   ClockDomain_DISPCC_DISPCCMDSSDPAUX1
 *   ClockDomain_DISPCC_DISPCCMDSSDPAUX
 *   ClockDomain_DISPCC_DISPCCMDSSEDPAUX
 *   ClockDomain_DISPCC_DISPCCMDSSVSYNC
 *   ClockDomain_DISPCC_DISPCCXO
 */
static const ClockSourceMapType ClockSourceMap_DISPCC_DISPCCMDSSDPAUX1[] =
{
  { 0,   19200000, &ClockSource_XO,                         },
  { 7,  400000000, NULL,                                    },
  { CLOCK_SOURCE_MAP_END }
};

/*
 * ClockSourceMap_DISPCC_DISPCCMDSSDPCRYPTO1
 *
 * Used by:
 *   ClockDomain_DISPCC_DISPCCMDSSDPCRYPTO1
 *   ClockDomain_DISPCC_DISPCCMDSSDPCRYPTO
 *   ClockDomain_DISPCC_DISPCCMDSSDPLINK1
 *   ClockDomain_DISPCC_DISPCCMDSSDPLINK
 *   ClockDomain_DISPCC_DISPCCMDSSDPPIXEL1
 *   ClockDomain_DISPCC_DISPCCMDSSDPPIXEL2
 *   ClockDomain_DISPCC_DISPCCMDSSDPPIXEL
 */
static const ClockSourceMapType ClockSourceMap_DISPCC_DISPCCMDSSDPCRYPTO1[] =
{
  { 0,   19200000, &ClockSource_XO,                         },
  { 1,          0, NULL,                                    },
  { 2,          0, NULL,                                    },
  { 3,          0, NULL,                                    },
  { 4,          0, NULL,                                    },
  { 5,          0, NULL,                                    },
  { 6,          0, NULL,                                    },
  { 7,  400000000, NULL,                                    },
  { CLOCK_SOURCE_MAP_END }
};

/*
 * ClockSourceMap_DISPCC_DISPCCMDSSEDPLINK
 *
 * Used by:
 *   ClockDomain_DISPCC_DISPCCMDSSEDPLINK
 *   ClockDomain_DISPCC_DISPCCMDSSEDPPIXEL
 */
static const ClockSourceMapType ClockSourceMap_DISPCC_DISPCCMDSSEDPLINK[] =
{
  { 0,   19200000, &ClockSource_XO,                         },
  { 1,          0, NULL,                                    },
  { 2,          0, NULL,                                    },
  { 7,  400000000, NULL,                                    },
  { CLOCK_SOURCE_MAP_END }
};

/*
 * ClockSourceMap_DISPCC_DISPCCMDSSMDP
 *
 * Used by:
 *   ClockDomain_DISPCC_DISPCCMDSSMDP
 *   ClockDomain_DISPCC_DISPCCMDSSROT
 */
static const ClockSourceMapType ClockSourceMap_DISPCC_DISPCCMDSSMDP[] =
{
  { 0,   19200000, &ClockSource_XO,                         },
  { 1, 1380000000, &ClockSource_DISPCC_DISPCCPLL0,          },
  { 4,  600000000, &ClockSource_DISPCC_DISPCCPLL1,          },
  { 5,  600000000, &ClockSource_DISPCC_DISPCCPLL1,          },
  { 7,  400000000, NULL,                                    },
  { CLOCK_SOURCE_MAP_END }
};

/*
 * ClockSourceMap_DISPCC_DISPCCMDSSPCLK0
 *
 * Used by:
 *   ClockDomain_DISPCC_DISPCCMDSSPCLK0
 *   ClockDomain_DISPCC_DISPCCMDSSPCLK1
 */
static const ClockSourceMapType ClockSourceMap_DISPCC_DISPCCMDSSPCLK0[] =
{
  { 0,   19200000, &ClockSource_XO,                         },
  { 1,          0, NULL,                                    },
  { 2,          0, NULL,                                    },
  { 7,  400000000, NULL,                                    },
  { CLOCK_SOURCE_MAP_END }
};

/*
 * ClockSourceMap_DISPCC_DISPCCPLLTESTMUX
 *
 * Used by:
 *   ClockDomain_DISPCC_DISPCCPLLTESTMUX
 */
static const ClockSourceMapType ClockSourceMap_DISPCC_DISPCCPLLTESTMUX[] =
{
  { 0, 1380000000, &ClockSource_DISPCC_DISPCCPLL0,          },
  { 1,  600000000, &ClockSource_DISPCC_DISPCCPLL1,          },
  { CLOCK_SOURCE_MAP_END }
};

/*
 * ClockSourceMap_DISPCC_DISPCCSLEEP
 *
 * Used by:
 *   ClockDomain_DISPCC_DISPCCSLEEP
 */
static const ClockSourceMapType ClockSourceMap_DISPCC_DISPCCSLEEP[] =
{
  { 0,      32000, NULL,                                    },
  { 7,  400000000, NULL,                                    },
  { CLOCK_SOURCE_MAP_END }
};

/*
 * ClockSourceMap_GCC_GCCCPUSSAHB
 *
 * Used by:
 *   ClockDomain_GCC_GCCCPUSSAHB
 *   ClockDomain_GCC_GCCPCIEPHYREFGEN
 *   ClockDomain_GCC_GCCPDM2
 *   ClockDomain_GCC_GCCQSPI1CORE
 *   ClockDomain_GCC_GCCQSPICORE
 *   ClockDomain_GCC_GCCQUPV3WRAP0S0
 *   ClockDomain_GCC_GCCQUPV3WRAP0S1
 *   ClockDomain_GCC_GCCQUPV3WRAP0S2
 *   ClockDomain_GCC_GCCQUPV3WRAP0S3
 *   ClockDomain_GCC_GCCQUPV3WRAP0S4
 *   ClockDomain_GCC_GCCQUPV3WRAP0S5
 *   ClockDomain_GCC_GCCQUPV3WRAP0S6
 *   ClockDomain_GCC_GCCQUPV3WRAP0S7
 *   ClockDomain_GCC_GCCQUPV3WRAP1S0
 *   ClockDomain_GCC_GCCQUPV3WRAP1S1
 *   ClockDomain_GCC_GCCQUPV3WRAP1S2
 *   ClockDomain_GCC_GCCQUPV3WRAP1S3
 *   ClockDomain_GCC_GCCQUPV3WRAP1S4
 *   ClockDomain_GCC_GCCQUPV3WRAP1S5
 *   ClockDomain_GCC_GCCQUPV3WRAP2S0
 *   ClockDomain_GCC_GCCQUPV3WRAP2S1
 *   ClockDomain_GCC_GCCQUPV3WRAP2S2
 *   ClockDomain_GCC_GCCQUPV3WRAP2S3
 *   ClockDomain_GCC_GCCQUPV3WRAP2S4
 *   ClockDomain_GCC_GCCQUPV3WRAP2S5
 *   ClockDomain_GCC_GCCUFSCARD2AXI
 *   ClockDomain_GCC_GCCUFSCARD2ICECORE
 *   ClockDomain_GCC_GCCUFSCARD2UNIPROCORE
 *   ClockDomain_GCC_GCCUFSCARDAXI
 *   ClockDomain_GCC_GCCUFSCARDICECORE
 *   ClockDomain_GCC_GCCUFSCARDUNIPROCORE
 *   ClockDomain_GCC_GCCUFSPHYAXI
 *   ClockDomain_GCC_GCCUFSPHYICECORE
 *   ClockDomain_GCC_GCCUFSPHYUNIPROCORE
 *   ClockDomain_GCC_GCCUSB30MPMASTER
 *   ClockDomain_GCC_GCCUSB30MPMOCKUTMI
 *   ClockDomain_GCC_GCCUSB30PRIMMASTER
 *   ClockDomain_GCC_GCCUSB30PRIMMOCKUTMI
 *   ClockDomain_GCC_GCCUSB30SECMASTER
 *   ClockDomain_GCC_GCCUSB30SECMOCKUTMI
 */
static const ClockSourceMapType ClockSourceMap_GCC_GCCCPUSSAHB[] =
{
  { 0,   19200000, &ClockSource_XO,                         },
  { 1,  600000000, &ClockSource_GCC_GPLL0,                  },
  { 6,  300000000, &ClockSource_GCC_GPLL0,                  },
  { 7,  400000000, NULL,                                    },
  { CLOCK_SOURCE_MAP_END }
};

/*
 * ClockSourceMap_GCC_GCCEMACPTP
 *
 * Used by:
 *   ClockDomain_GCC_GCCEMACPTP
 *   ClockDomain_GCC_GCCEMACRGMII
 */
static const ClockSourceMapType ClockSourceMap_GCC_GCCEMACPTP[] =
{
  { 0,   19200000, &ClockSource_XO,                         },
  { 1,  600000000, &ClockSource_GCC_GPLL0,                  },
  { 3,  500000000, &ClockSource_GCC_GPLL7,                  },
  { 6,  300000000, &ClockSource_GCC_GPLL0,                  },
  { 7,  400000000, NULL,                                    },
  { CLOCK_SOURCE_MAP_END }
};

/*
 * ClockSourceMap_GCC_GCCGP1
 *
 * Used by:
 *   ClockDomain_GCC_GCCGP1
 *   ClockDomain_GCC_GCCGP2
 *   ClockDomain_GCC_GCCGP3
 *   ClockDomain_GCC_GCCGP4
 *   ClockDomain_GCC_GCCGP5
 */
static const ClockSourceMapType ClockSourceMap_GCC_GCCGP1[] =
{
  { 0,   19200000, &ClockSource_XO,                         },
  { 1,  600000000, &ClockSource_GCC_GPLL0,                  },
  { 5,      32000, NULL,                                    },
  { 6,  300000000, &ClockSource_GCC_GPLL0,                  },
  { 7,  400000000, NULL,                                    },
  { CLOCK_SOURCE_MAP_END }
};

/*
 * ClockSourceMap_GCC_GCCPCIE0AUX
 *
 * Used by:
 *   ClockDomain_GCC_GCCPCIE0AUX
 *   ClockDomain_GCC_GCCPCIE1AUX
 *   ClockDomain_GCC_GCCPCIE2AUX
 *   ClockDomain_GCC_GCCPCIE3AUX
 *   ClockDomain_GCC_GCCUSB3MPPHYAUX
 *   ClockDomain_GCC_GCCUSB3PRIMPHYAUX
 *   ClockDomain_GCC_GCCUSB3SECPHYAUX
 */
static const ClockSourceMapType ClockSourceMap_GCC_GCCPCIE0AUX[] =
{
  { 0,   19200000, &ClockSource_XO,                         },
  { 5,      32000, NULL,                                    },
  { 7,  400000000, NULL,                                    },
  { CLOCK_SOURCE_MAP_END }
};

/*
 * ClockSourceMap_GCC_GCCPCIE0PIPE
 *
 * Used by:
 *   ClockDomain_GCC_GCCPCIE0PIPE
 */
static const ClockSourceMapType ClockSourceMap_GCC_GCCPCIE0PIPE[] =
{
  { 0,          0, NULL,                                    },
  { 1,  400000000, NULL,                                    },
  { 2,   19200000, &ClockSource_XO,                         },
  { CLOCK_SOURCE_MAP_END }
};

/*
 * ClockSourceMap_GCC_GCCPCIE1PIPE
 *
 * Used by:
 *   ClockDomain_GCC_GCCPCIE1PIPE
 */
static const ClockSourceMapType ClockSourceMap_GCC_GCCPCIE1PIPE[] =
{
  { 0,          0, NULL,                                    },
  { 1,  400000000, NULL,                                    },
  { 2,   19200000, &ClockSource_XO,                         },
  { CLOCK_SOURCE_MAP_END }
};

/*
 * ClockSourceMap_GCC_GCCPCIE2PIPE
 *
 * Used by:
 *   ClockDomain_GCC_GCCPCIE2PIPE
 */
static const ClockSourceMapType ClockSourceMap_GCC_GCCPCIE2PIPE[] =
{
  { 0,          0, NULL,                                    },
  { 1,  400000000, NULL,                                    },
  { 2,   19200000, &ClockSource_XO,                         },
  { CLOCK_SOURCE_MAP_END }
};

/*
 * ClockSourceMap_GCC_GCCPCIE3PIPE
 *
 * Used by:
 *   ClockDomain_GCC_GCCPCIE3PIPE
 */
static const ClockSourceMapType ClockSourceMap_GCC_GCCPCIE3PIPE[] =
{
  { 0,          0, NULL,                                    },
  { 1,  400000000, NULL,                                    },
  { 2,   19200000, &ClockSource_XO,                         },
  { CLOCK_SOURCE_MAP_END }
};

/*
 * ClockSourceMap_GCC_GCCSDCC2APPS
 *
 * Used by:
 *   ClockDomain_GCC_GCCSDCC2APPS
 */
static const ClockSourceMapType ClockSourceMap_GCC_GCCSDCC2APPS[] =
{
  { 0,   19200000, &ClockSource_XO,                         },
  { 1,  600000000, &ClockSource_GCC_GPLL0,                  },
  { 2,   19200000, &ClockSource_GCC_GPLL9,                  {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}},
  { 2,  808000000, &ClockSource_GCC_GPLL9,                  {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}},
  { 5,  806000000, NULL,                                    },
  { 6,  300000000, &ClockSource_GCC_GPLL0,                  },
  { 7,  400000000, NULL,                                    },
  { CLOCK_SOURCE_MAP_END }
};

/*
 * ClockSourceMap_GCC_GCCSDCC4APPS
 *
 * Used by:
 *   ClockDomain_GCC_GCCSDCC4APPS
 */
static const ClockSourceMapType ClockSourceMap_GCC_GCCSDCC4APPS[] =
{
  { 0,   19200000, &ClockSource_XO,                         },
  { 1,  600000000, &ClockSource_GCC_GPLL0,                  },
  { 7,  400000000, NULL,                                    },
  { CLOCK_SOURCE_MAP_END }
};

/*
 * ClockSourceMap_GCC_GCCTSIFREF
 *
 * Used by:
 *   ClockDomain_GCC_GCCTSIFREF
 */
static const ClockSourceMapType ClockSourceMap_GCC_GCCTSIFREF[] =
{
  { 0,   19200000, &ClockSource_XO,                         },
  { 1,  600000000, &ClockSource_GCC_GPLL0,                  },
  { 2,   24000000, NULL,                                    },
  { 6,  300000000, &ClockSource_GCC_GPLL0,                  },
  { 7,  400000000, NULL,                                    },
  { CLOCK_SOURCE_MAP_END }
};

/*
 * ClockSourceMap_GCC_GCCUFSCARD2PHYAUX
 *
 * Used by:
 *   ClockDomain_GCC_GCCUFSCARD2PHYAUX
 *   ClockDomain_GCC_GCCUFSCARDPHYAUX
 *   ClockDomain_GCC_GCCUFSPHYPHYAUX
 */
static const ClockSourceMapType ClockSourceMap_GCC_GCCUFSCARD2PHYAUX[] =
{
  { 0,   19200000, &ClockSource_XO,                         },
  { 7,  400000000, NULL,                                    },
  { CLOCK_SOURCE_MAP_END }
};

/*
 * ClockSourceMap_GCC_GCCUFSCARD2RXSYMBOL0
 *
 * Used by:
 *   ClockDomain_GCC_GCCUFSCARD2RXSYMBOL0
 */
static const ClockSourceMapType ClockSourceMap_GCC_GCCUFSCARD2RXSYMBOL0[] =
{
  { 0,          0, NULL,                                    },
  { 1,          0, NULL,                                    },
  { 2,   19200000, &ClockSource_XO,                         },
  { CLOCK_SOURCE_MAP_END }
};

/*
 * ClockSourceMap_GCC_GCCUFSCARD2RXSYMBOL1
 *
 * Used by:
 *   ClockDomain_GCC_GCCUFSCARD2RXSYMBOL1
 */
static const ClockSourceMapType ClockSourceMap_GCC_GCCUFSCARD2RXSYMBOL1[] =
{
  { 0,          0, NULL,                                    },
  { 1,          0, NULL,                                    },
  { 2,   19200000, &ClockSource_XO,                         },
  { CLOCK_SOURCE_MAP_END }
};

/*
 * ClockSourceMap_GCC_GCCUFSCARD2TXSYMBOL0
 *
 * Used by:
 *   ClockDomain_GCC_GCCUFSCARD2TXSYMBOL0
 */
static const ClockSourceMapType ClockSourceMap_GCC_GCCUFSCARD2TXSYMBOL0[] =
{
  { 0,          0, NULL,                                    },
  { 1,          0, NULL,                                    },
  { 2,   19200000, &ClockSource_XO,                         },
  { CLOCK_SOURCE_MAP_END }
};

/*
 * ClockSourceMap_GCC_GCCUFSCARDRXSYMBOL0
 *
 * Used by:
 *   ClockDomain_GCC_GCCUFSCARDRXSYMBOL0
 */
static const ClockSourceMapType ClockSourceMap_GCC_GCCUFSCARDRXSYMBOL0[] =
{
  { 0,          0, NULL,                                    {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}},
  { 1,          0, NULL,                                    {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}},
  { 2,   19200000, &ClockSource_XO,                         },
  { CLOCK_SOURCE_MAP_END }
};

/*
 * ClockSourceMap_GCC_GCCUFSCARDRXSYMBOL1
 *
 * Used by:
 *   ClockDomain_GCC_GCCUFSCARDRXSYMBOL1
 */
static const ClockSourceMapType ClockSourceMap_GCC_GCCUFSCARDRXSYMBOL1[] =
{
  { 0,          0, NULL,                                    {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}},
  { 1,          0, NULL,                                    {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}},
  { 2,   19200000, &ClockSource_XO,                         },
  { CLOCK_SOURCE_MAP_END }
};

/*
 * ClockSourceMap_GCC_GCCUFSCARDTXSYMBOL0
 *
 * Used by:
 *   ClockDomain_GCC_GCCUFSCARDTXSYMBOL0
 */
static const ClockSourceMapType ClockSourceMap_GCC_GCCUFSCARDTXSYMBOL0[] =
{
  { 0,          0, NULL,                                    {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}},
  { 1,          0, NULL,                                    {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}},
  { 2,   19200000, &ClockSource_XO,                         },
  { CLOCK_SOURCE_MAP_END }
};

/*
 * ClockSourceMap_GCC_GCCUFSPHYRXSYMBOL0
 *
 * Used by:
 *   ClockDomain_GCC_GCCUFSPHYRXSYMBOL0
 */
static const ClockSourceMapType ClockSourceMap_GCC_GCCUFSPHYRXSYMBOL0[] =
{
  { 0,          0, NULL,                                    },
  { 1,          0, NULL,                                    },
  { 2,   19200000, &ClockSource_XO,                         },
  { CLOCK_SOURCE_MAP_END }
};

/*
 * ClockSourceMap_GCC_GCCUFSPHYRXSYMBOL1
 *
 * Used by:
 *   ClockDomain_GCC_GCCUFSPHYRXSYMBOL1
 */
static const ClockSourceMapType ClockSourceMap_GCC_GCCUFSPHYRXSYMBOL1[] =
{
  { 0,          0, NULL,                                    },
  { 1,          0, NULL,                                    },
  { 2,   19200000, &ClockSource_XO,                         },
  { CLOCK_SOURCE_MAP_END }
};

/*
 * ClockSourceMap_GCC_GCCUFSPHYTXSYMBOL0
 *
 * Used by:
 *   ClockDomain_GCC_GCCUFSPHYTXSYMBOL0
 */
static const ClockSourceMapType ClockSourceMap_GCC_GCCUFSPHYTXSYMBOL0[] =
{
  { 0,          0, NULL,                                    },
  { 1,          0, NULL,                                    },
  { 2,   19200000, &ClockSource_XO,                         },
  { CLOCK_SOURCE_MAP_END }
};

/*
 * ClockSourceMap_GCC_GCCUSB3MPPHYPIPE0
 *
 * Used by:
 *   ClockDomain_GCC_GCCUSB3MPPHYPIPE0
 */
static const ClockSourceMapType ClockSourceMap_GCC_GCCUSB3MPPHYPIPE0[] =
{
  { 0,          0, NULL,                                    },
  { 1,  400000000, NULL,                                    },
  { 2,   19200000, &ClockSource_XO,                         },
  { CLOCK_SOURCE_MAP_END }
};

/*
 * ClockSourceMap_GCC_GCCUSB3MPPHYPIPE1
 *
 * Used by:
 *   ClockDomain_GCC_GCCUSB3MPPHYPIPE1
 */
static const ClockSourceMapType ClockSourceMap_GCC_GCCUSB3MPPHYPIPE1[] =
{
  { 0,          0, NULL,                                    },
  { 1,  400000000, NULL,                                    },
  { 2,   19200000, &ClockSource_XO,                         },
  { CLOCK_SOURCE_MAP_END }
};

/*
 * ClockSourceMap_GCC_GCCUSB3PRIMPHYPIPE
 *
 * Used by:
 *   ClockDomain_GCC_GCCUSB3PRIMPHYPIPE
 */
static const ClockSourceMapType ClockSourceMap_GCC_GCCUSB3PRIMPHYPIPE[] =
{
  { 0,          0, NULL,                                    },
  { 1,  400000000, NULL,                                    },
  { 2,   19200000, &ClockSource_XO,                         },
  { CLOCK_SOURCE_MAP_END }
};

/*
 * ClockSourceMap_GCC_GCCUSB3SECPHYPIPE
 *
 * Used by:
 *   ClockDomain_GCC_GCCUSB3SECPHYPIPE
 */
static const ClockSourceMapType ClockSourceMap_GCC_GCCUSB3SECPHYPIPE[] =
{
  { 0,          0, NULL,                                    },
  { 1,  400000000, NULL,                                    },
  { 2,   19200000, &ClockSource_XO,                         },
  { CLOCK_SOURCE_MAP_END }
};

/*
 * ClockSourceMap_GPUCC_GPUCCDEBUGMUX
 *
 * Used by:
 *   ClockDomain_GPUCC_GPUCCDEBUGMUX
 */
static const ClockSourceMapType ClockSourceMap_GPUCC_GPUCCDEBUGMUX[] =
{
  { CLOCK_SOURCE_MAP_END }
};

/*
 * ClockSourceMap_GPUCC_GPUCCGMU
 *
 * Used by:
 *   ClockDomain_GPUCC_GPUCCGMU
 */
static const ClockSourceMapType ClockSourceMap_GPUCC_GPUCCGMU[] =
{
  { 0,   19200000, &ClockSource_XO,                         },
  { 1,          0, &ClockSource_GPUCC_GPUCCPLL0,            },
  { 3,  500000000, &ClockSource_GPUCC_GPUCCPLL1,            },
  { 5,  600000000, &ClockSource_GCC_GPLL0,                  },
  { 6,  300000000, NULL,                                    },
  { 7,  400000000, NULL,                                    },
  { CLOCK_SOURCE_MAP_END }
};

/*
 * ClockSourceMap_GPUCC_GPUCCGXGFX3D
 *
 * Used by:
 *   ClockDomain_GPUCC_GPUCCGXGFX3D
 */
static const ClockSourceMapType ClockSourceMap_GPUCC_GPUCCGXGFX3D[] =
{
  { 0,   19200000, &ClockSource_XO,                         },
  { 1,          0, &ClockSource_GPUCC_GPUCCPLL0,            },
  { 2,          0, &ClockSource_GPUCC_GPUCCPLL0,            },
  { 3,  500000000, &ClockSource_GPUCC_GPUCCPLL1,            },
  { 4,  500000000, &ClockSource_GPUCC_GPUCCPLL1,            },
  { 5,  600000000, &ClockSource_GCC_GPLL0,                  },
  { 7,  400000000, NULL,                                    },
  { CLOCK_SOURCE_MAP_END }
};

/*
 * ClockSourceMap_GPUCC_GPUCCPLLTESTMUX
 *
 * Used by:
 *   ClockDomain_GPUCC_GPUCCPLLTESTMUX
 */
static const ClockSourceMapType ClockSourceMap_GPUCC_GPUCCPLLTESTMUX[] =
{
  { 0,          0, &ClockSource_GPUCC_GPUCCPLL0,            },
  { 1,  500000000, &ClockSource_GPUCC_GPUCCPLL1,            },
  { CLOCK_SOURCE_MAP_END }
};

/*
 * ClockSourceMap_NPUCC_NPUCCCALDP
 *
 * Used by:
 *   ClockDomain_NPUCC_NPUCCCALDP
 *   ClockDomain_NPUCC_NPUCCNPUCORE
 */
static const ClockSourceMapType ClockSourceMap_NPUCC_NPUCCCALDP[] =
{
  { 0,   19200000, &ClockSource_XO,                         },
  { 1,  700000000, &ClockSource_NPUCC_NPUCCPLL1,            {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}},
  { 1,  800000000, &ClockSource_NPUCC_NPUCCPLL1,            {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}},
  { 2,          0, &ClockSource_NPUCC_NPUCCPLL0,            },
  { 4,  600000000, &ClockSource_GCC_GPLL0,                  },
  { 5,  300000000, NULL,                                    },
  { 7,  400000000, NULL,                                    },
  { CLOCK_SOURCE_MAP_END }
};

/*
 * ClockSourceMap_NPUCC_NPUCCDEBUGMUX
 *
 * Used by:
 *   ClockDomain_NPUCC_NPUCCDEBUGMUX
 */
static const ClockSourceMapType ClockSourceMap_NPUCC_NPUCCDEBUGMUX[] =
{
  { CLOCK_SOURCE_MAP_END }
};

/*
 * ClockSourceMap_NPUCC_NPUCCPLLTESTMUX
 *
 * Used by:
 *   ClockDomain_NPUCC_NPUCCPLLTESTMUX
 */
static const ClockSourceMapType ClockSourceMap_NPUCC_NPUCCPLLTESTMUX[] =
{
  { 0,          0, &ClockSource_NPUCC_NPUCCPLL0,            },
  { 1,  700000000, &ClockSource_NPUCC_NPUCCPLL1,            {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}},
  { 1,  800000000, &ClockSource_NPUCC_NPUCCPLL1,            {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}},
  { CLOCK_SOURCE_MAP_END }
};

#ifdef ENABLE_AUTO_PLAT
/*
 * ClockSourceMap_SCC_SCCMAINRCG
 *
 * Used by:
 *   ClockDomain_SCC_SCCMAINRCG
 *   ClockDomain_SCC_SCCQUPV3SE0
 *   ClockDomain_SCC_SCCQUPV3SE1
 *   ClockDomain_SCC_SCCQUPV3SE2
 *   ClockDomain_SCC_SCCQUPV3SE3
 *   ClockDomain_SCC_SCCQUPV3SE4
 *   ClockDomain_SCC_SCCQUPV3SE5
 */
static const ClockSourceMapType ClockSourceMap_SCC_SCCMAINRCG[] =
{
  { 0,   19200000, &ClockSource_XO,                         },
  { 1,      32768, NULL,                                    },
  { 2,  288000000, &ClockSource_SCC_SCCPLL,                 {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}},
  { 2,  300000000, &ClockSource_SCC_SCCPLL,                 {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}},
  { 3,   19200000, &ClockSource_XO,                         },
  { 4,  576000000, &ClockSource_SCC_SCCPLL,                 {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}},
  { 4,  600000000, &ClockSource_SCC_SCCPLL,                 {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}},
  { 5,          0, NULL,                                    },
  { 6,  576000000, &ClockSource_SCC_SCCPLL,                 {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}},
  { 6,  600000000, &ClockSource_SCC_SCCPLL,                 {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}},
  { 7,  400000000, NULL,                                    },
  { CLOCK_SOURCE_MAP_END }
};
#endif
/*
 * ClockSourceMap_VIDEOCC_VIDEOCCIRIS
 *
 * Used by:
 *   ClockDomain_VIDEOCC_VIDEOCCIRIS
 */
static const ClockSourceMapType ClockSourceMap_VIDEOCC_VIDEOCCIRIS[] =
{
  { 0,   19200000, &ClockSource_XO,                         },
  { 1,          0, &ClockSource_VIDEOCC_VIDEOPLL0,          },
  { 2,          0, &ClockSource_VIDEOCC_VIDEOPLL0,          },
  { 3,          0, &ClockSource_VIDEOCC_VIDEOPLL0,          },
  { 7,  400000000, NULL,                                    },
  { CLOCK_SOURCE_MAP_END }
};

/*
 * ClockSourceMap_VIDEOCC_VIDEOCCSLEEP
 *
 * Used by:
 *   ClockDomain_VIDEOCC_VIDEOCCSLEEP
 */
static const ClockSourceMapType ClockSourceMap_VIDEOCC_VIDEOCCSLEEP[] =
{
  { 0,      32000, NULL,                                    },
  { 7,  400000000, NULL,                                    },
  { CLOCK_SOURCE_MAP_END }
};

/*
 * ClockSourceMap_VIDEOCC_VIDEOCCXO
 *
 * Used by:
 *   ClockDomain_VIDEOCC_VIDEOCCXO
 */
static const ClockSourceMapType ClockSourceMap_VIDEOCC_VIDEOCCXO[] =
{
  { 0,   19200000, &ClockSource_XO,                         },
  { 7,  400000000, NULL,                                    },
  { CLOCK_SOURCE_MAP_END }
};


/*=========================================================================
      Domain FMAX
==========================================================================*/

/*
 * ClockDomainFMax_CAMCC_CAMCCBPS
 *
 * Used by:
 *   ClockDomain_CAMCC_CAMCCBPS
 */
static const ClockFMaxType ClockDomainFMax_CAMCC_CAMCCBPS[] =
{
  {   19200000, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },
  {  200000000, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },
  {  400000000, RAIL_VOLTAGE_LEVEL_SVS,        },
  {  480000000, RAIL_VOLTAGE_LEVEL_SVS_L1,     },
  {  600000000, RAIL_VOLTAGE_LEVEL_NOM,        },
  {  760000000, RAIL_VOLTAGE_LEVEL_TUR,        {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}},
  {  0  }
};

/*
 * ClockDomainFMax_CAMCC_CAMCCCAMNOCAXI
 *
 * Used by:
 *   ClockDomain_CAMCC_CAMCCCAMNOCAXI
 */
static const ClockFMaxType ClockDomainFMax_CAMCC_CAMCCCAMNOCAXI[] =
{
  {   19200000, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },
  {  150000000, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },
  {  266666667, RAIL_VOLTAGE_LEVEL_SVS,        },
  {  320000000, RAIL_VOLTAGE_LEVEL_SVS_L1,     },
  {  400000000, RAIL_VOLTAGE_LEVEL_NOM,        },
  {  480000000, RAIL_VOLTAGE_LEVEL_TUR,        },
  {  0  }
};

/*
 * ClockDomainFMax_CAMCC_CAMCCCCI0
 *
 * Used by:
 *   ClockDomain_CAMCC_CAMCCCCI0
 *   ClockDomain_CAMCC_CAMCCCCI1
 *   ClockDomain_CAMCC_CAMCCCCI2
 *   ClockDomain_CAMCC_CAMCCCCI3
 */
static const ClockFMaxType ClockDomainFMax_CAMCC_CAMCCCCI0[] =
{
  {   19200000, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },
  {   37500000, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },
  {  0  }
};

/*
 * ClockDomainFMax_CAMCC_CAMCCCPHYRX
 *
 * Used by:
 *   ClockDomain_CAMCC_CAMCCCPHYRX
 */
static const ClockFMaxType ClockDomainFMax_CAMCC_CAMCCCPHYRX[] =
{
  {   19200000, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },
  {  400000000, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },
  {  0  }
};

/*
 * ClockDomainFMax_CAMCC_CAMCCCSI0PHYTIMER
 *
 * Used by:
 *   ClockDomain_CAMCC_CAMCCCSI0PHYTIMER
 *   ClockDomain_CAMCC_CAMCCCSI1PHYTIMER
 *   ClockDomain_CAMCC_CAMCCCSI2PHYTIMER
 *   ClockDomain_CAMCC_CAMCCCSI3PHYTIMER
 */
static const ClockFMaxType ClockDomainFMax_CAMCC_CAMCCCSI0PHYTIMER[] =
{
  {   19200000, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },
  {  300000000, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },
  {  0  }
};

/*
 * ClockDomainFMax_CAMCC_CAMCCDEBUGMUX
 *
 * Used by:
 *   ClockDomain_CAMCC_CAMCCDEBUGMUX
 */
static const ClockFMaxType ClockDomainFMax_CAMCC_CAMCCDEBUGMUX[] =
{
  {   19200000, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },
  {  600000000, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },
  {  0  }
};

/*
 * ClockDomainFMax_CAMCC_CAMCCFASTAHB
 *
 * Used by:
 *   ClockDomain_CAMCC_CAMCCFASTAHB
 */
static const ClockFMaxType ClockDomainFMax_CAMCC_CAMCCFASTAHB[] =
{
  {   19200000, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },
  {  100000000, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },
  {  200000000, RAIL_VOLTAGE_LEVEL_SVS,        },
  {  300000000, RAIL_VOLTAGE_LEVEL_SVS_L1,     },
  {  400000000, RAIL_VOLTAGE_LEVEL_NOM,        },
  {  0  }
};

/*
 * ClockDomainFMax_CAMCC_CAMCCFDCORE
 *
 * Used by:
 *   ClockDomain_CAMCC_CAMCCFDCORE
 *   ClockDomain_CAMCC_CAMCCIFE0CSID
 *   ClockDomain_CAMCC_CAMCCIFE1CSID
 *   ClockDomain_CAMCC_CAMCCIFE2CSID
 *   ClockDomain_CAMCC_CAMCCIFE3CSID
 *   ClockDomain_CAMCC_CAMCCIFELITE0CSID
 *   ClockDomain_CAMCC_CAMCCIFELITE1CSID
 *   ClockDomain_CAMCC_CAMCCIFELITE2CSID
 *   ClockDomain_CAMCC_CAMCCIFELITE3CSID
 */
static const ClockFMaxType ClockDomainFMax_CAMCC_CAMCCFDCORE[] =
{
  {   19200000, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },
  {  400000000, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },
  {  480000000, RAIL_VOLTAGE_LEVEL_SVS_L1,     },
  {  600000000, RAIL_VOLTAGE_LEVEL_NOM,        },
  {  0  }
};

/*
 * ClockDomainFMax_CAMCC_CAMCCICP
 *
 * Used by:
 *   ClockDomain_CAMCC_CAMCCICP
 */
static const ClockFMaxType ClockDomainFMax_CAMCC_CAMCCICP[] =
{
  {   19200000, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },
  {  400000000, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },
  {  600000000, RAIL_VOLTAGE_LEVEL_SVS_L1,     },
  {  0  }
};

/*
 * ClockDomainFMax_CAMCC_CAMCCIFE0
 *
 * Used by:
 *   ClockDomain_CAMCC_CAMCCIFE0
 *   ClockDomain_CAMCC_CAMCCIFE1
 *   ClockDomain_CAMCC_CAMCCIFE2
 *   ClockDomain_CAMCC_CAMCCIFE3
 */
static const ClockFMaxType ClockDomainFMax_CAMCC_CAMCCIFE0[] =
{
  {   19200000, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },
  {  400000000, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },
  {  558000000, RAIL_VOLTAGE_LEVEL_SVS,        },
  {  637000000, RAIL_VOLTAGE_LEVEL_SVS_L1,     },
  {  760000000, RAIL_VOLTAGE_LEVEL_NOM,        },
  {  0  }
};

/*
 * ClockDomainFMax_CAMCC_CAMCCIFELITE0
 *
 * Used by:
 *   ClockDomain_CAMCC_CAMCCIFELITE0
 *   ClockDomain_CAMCC_CAMCCIFELITE1
 *   ClockDomain_CAMCC_CAMCCIFELITE2
 *   ClockDomain_CAMCC_CAMCCIFELITE3
 */
static const ClockFMaxType ClockDomainFMax_CAMCC_CAMCCIFELITE0[] =
{
  {   19200000, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },
  {  320000000, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },
  {  400000000, RAIL_VOLTAGE_LEVEL_SVS,        },
  {  480000000, RAIL_VOLTAGE_LEVEL_SVS_L1,     },
  {  600000000, RAIL_VOLTAGE_LEVEL_NOM,        },
  {  0  }
};

/*
 * ClockDomainFMax_CAMCC_CAMCCIPE0
 *
 * Used by:
 *   ClockDomain_CAMCC_CAMCCIPE0
 */
static const ClockFMaxType ClockDomainFMax_CAMCC_CAMCCIPE0[] =
{
  {   19200000, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },
  {  375000000, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },
  {  475000000, RAIL_VOLTAGE_LEVEL_SVS,        },
  {  520000000, RAIL_VOLTAGE_LEVEL_SVS_L1,     },
  {  600000000, RAIL_VOLTAGE_LEVEL_NOM,        },
  {  0  }
};

/*
 * ClockDomainFMax_CAMCC_CAMCCJPEG
 *
 * Used by:
 *   ClockDomain_CAMCC_CAMCCJPEG
 */
static const ClockFMaxType ClockDomainFMax_CAMCC_CAMCCJPEG[] =
{
  {   19200000, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },
  {  200000000, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },
  {  400000000, RAIL_VOLTAGE_LEVEL_SVS,        },
  {  480000000, RAIL_VOLTAGE_LEVEL_SVS_L1,     },
  {  600000000, RAIL_VOLTAGE_LEVEL_NOM,        },
  {  0  }
};

/*
 * ClockDomainFMax_CAMCC_CAMCCLRME
 *
 * Used by:
 *   ClockDomain_CAMCC_CAMCCLRME
 */
static const ClockFMaxType ClockDomainFMax_CAMCC_CAMCCLRME[] =
{
  {   19200000, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },
  {  240000000, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },
  {  300000000, RAIL_VOLTAGE_LEVEL_SVS,        },
  {  320000000, RAIL_VOLTAGE_LEVEL_SVS_L1,     },
  {  400000000, RAIL_VOLTAGE_LEVEL_NOM,        },
  {  0  }
};

/*
 * ClockDomainFMax_CAMCC_CAMCCMCLK0
 *
 * Used by:
 *   ClockDomain_CAMCC_CAMCCMCLK0
 *   ClockDomain_CAMCC_CAMCCMCLK1
 *   ClockDomain_CAMCC_CAMCCMCLK2
 *   ClockDomain_CAMCC_CAMCCMCLK3
 *   ClockDomain_CAMCC_CAMCCMCLK4
 *   ClockDomain_CAMCC_CAMCCMCLK5
 *   ClockDomain_CAMCC_CAMCCMCLK6
 *   ClockDomain_CAMCC_CAMCCMCLK7
 */
static const ClockFMaxType ClockDomainFMax_CAMCC_CAMCCMCLK0[] =
{
  {   19200000, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },
  {   68571429, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },
  {  0  }
};

/*
 * ClockDomainFMax_CAMCC_CAMCCPLLTESTMUX
 *
 * Used by:
 *   ClockDomain_CAMCC_CAMCCPLLTESTMUX
 */
static const ClockFMaxType ClockDomainFMax_CAMCC_CAMCCPLLTESTMUX[] =
{
  {   19200000, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },
  {  600000000, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },
  { 1200000000, RAIL_VOLTAGE_LEVEL_SVS_L1,     },
  {  0  }
};

/*
 * ClockDomainFMax_CAMCC_CAMCCQDSSDEBUG
 *
 * Used by:
 *   ClockDomain_CAMCC_CAMCCQDSSDEBUG
 */
static const ClockFMaxType ClockDomainFMax_CAMCC_CAMCCQDSSDEBUG[] =
{
  {   19200000, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },
  {   75000000, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },
  {  150000000, RAIL_VOLTAGE_LEVEL_SVS,        },
  {  300000000, RAIL_VOLTAGE_LEVEL_SVS_L1,     },
  {  0  }
};

/*
 * ClockDomainFMax_CAMCC_CAMCCSLEEP
 *
 * Used by:
 *   ClockDomain_CAMCC_CAMCCSLEEP
 */
static const ClockFMaxType ClockDomainFMax_CAMCC_CAMCCSLEEP[] =
{
  {      32000, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },
  {  0  }
};

/*
 * ClockDomainFMax_CAMCC_CAMCCSLOWAHB
 *
 * Used by:
 *   ClockDomain_CAMCC_CAMCCSLOWAHB
 */
static const ClockFMaxType ClockDomainFMax_CAMCC_CAMCCSLOWAHB[] =
{
  {   19200000, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },
  {   80000000, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },
  {  0  }
};

/*
 * ClockDomainFMax_CAMCC_CAMCCXO
 *
 * Used by:
 *   ClockDomain_CAMCC_CAMCCXO
 */
static const ClockFMaxType ClockDomainFMax_CAMCC_CAMCCXO[] =
{
  {   19200000, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },
  {  0  }
};

/*
 * ClockDomainFMax_DISPCC_DISPCCMDSSAHB
 *
 * Used by:
 *   ClockDomain_DISPCC_DISPCCMDSSAHB
 */
static const ClockFMaxType ClockDomainFMax_DISPCC_DISPCCMDSSAHB[] =
{
  {   19200000, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },
  {   37500000, RAIL_VOLTAGE_LEVEL_SVS,        },
  {   75000000, RAIL_VOLTAGE_LEVEL_NOM,        },
  {  0  }
};

/*
 * ClockDomainFMax_DISPCC_DISPCCMDSSBYTE0
 *
 * Used by:
 *   ClockDomain_DISPCC_DISPCCMDSSBYTE0
 *   ClockDomain_DISPCC_DISPCCMDSSBYTE1
 */
static const ClockFMaxType ClockDomainFMax_DISPCC_DISPCCMDSSBYTE0[] =
{
  {   19200000, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },
  {  187500000, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },
  {  300000000, RAIL_VOLTAGE_LEVEL_SVS,        },
  {  358000000, RAIL_VOLTAGE_LEVEL_SVS_L1,     },
  {  0  }
};

/*
 * ClockDomainFMax_DISPCC_DISPCCMDSSDPAUX1
 *
 * Used by:
 *   ClockDomain_DISPCC_DISPCCMDSSDPAUX1
 *   ClockDomain_DISPCC_DISPCCMDSSDPAUX
 *   ClockDomain_DISPCC_DISPCCMDSSEDPAUX
 *   ClockDomain_DISPCC_DISPCCMDSSEDPGTC
 *   ClockDomain_DISPCC_DISPCCMDSSESC0
 *   ClockDomain_DISPCC_DISPCCMDSSESC1
 *   ClockDomain_DISPCC_DISPCCMDSSVSYNC
 *   ClockDomain_DISPCC_DISPCCXO
 */
static const ClockFMaxType ClockDomainFMax_DISPCC_DISPCCMDSSDPAUX1[] =
{
  {   19200000, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },
  {  0  }
};

/*
 * ClockDomainFMax_DISPCC_DISPCCMDSSDPCRYPTO1
 *
 * Used by:
 *   ClockDomain_DISPCC_DISPCCMDSSDPCRYPTO1
 *   ClockDomain_DISPCC_DISPCCMDSSDPCRYPTO
 */
static const ClockFMaxType ClockDomainFMax_DISPCC_DISPCCMDSSDPCRYPTO1[] =
{
  {   12800000, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },
  {  180000000, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },
  {  360000000, RAIL_VOLTAGE_LEVEL_SVS_L1,     },
  {  540000000, RAIL_VOLTAGE_LEVEL_NOM,        },
  {  0  }
};

/*
 * ClockDomainFMax_DISPCC_DISPCCMDSSDPLINK1
 *
 * Used by:
 *   ClockDomain_DISPCC_DISPCCMDSSDPLINK1
 *   ClockDomain_DISPCC_DISPCCMDSSDPLINK
 */
static const ClockFMaxType ClockDomainFMax_DISPCC_DISPCCMDSSDPLINK1[] =
{
  {   19200000, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },
  {  270000000, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },
  {  540000000, RAIL_VOLTAGE_LEVEL_SVS_L1,     },
  {  810000000, RAIL_VOLTAGE_LEVEL_NOM,        },
  {  0  }
};

/*
 * ClockDomainFMax_DISPCC_DISPCCMDSSDPPIXEL1
 *
 * Used by:
 *   ClockDomain_DISPCC_DISPCCMDSSDPPIXEL1
 *   ClockDomain_DISPCC_DISPCCMDSSDPPIXEL2
 *   ClockDomain_DISPCC_DISPCCMDSSDPPIXEL
 */
static const ClockFMaxType ClockDomainFMax_DISPCC_DISPCCMDSSDPPIXEL1[] =
{
  {   19200000, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },
  {  337500000, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },
  {  675000000, RAIL_VOLTAGE_LEVEL_SVS_L1,     {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}},
  {  675000000, RAIL_VOLTAGE_LEVEL_NOM,        {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}},
  {  0  }
};

/*
 * ClockDomainFMax_DISPCC_DISPCCMDSSEDPLINK
 *
 * Used by:
 *   ClockDomain_DISPCC_DISPCCMDSSEDPLINK
 */
static const ClockFMaxType ClockDomainFMax_DISPCC_DISPCCMDSSEDPLINK[] =
{
  {   19200000, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },
  {  270000000, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },
  {  594000000, RAIL_VOLTAGE_LEVEL_SVS,        {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}},
  {  594000000, RAIL_VOLTAGE_LEVEL_SVS_L1,     {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}},
  {  810000000, RAIL_VOLTAGE_LEVEL_NOM,        },
  {  0  }
};

/*
 * ClockDomainFMax_DISPCC_DISPCCMDSSEDPPIXEL
 *
 * Used by:
 *   ClockDomain_DISPCC_DISPCCMDSSEDPPIXEL
 */
static const ClockFMaxType ClockDomainFMax_DISPCC_DISPCCMDSSEDPPIXEL[] =
{
  {   19200000, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },
  {  337500000, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },
  {  371500000, RAIL_VOLTAGE_LEVEL_SVS,        {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}},
  {  675000000, RAIL_VOLTAGE_LEVEL_SVS_L1,     {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}},
  {  675000000, RAIL_VOLTAGE_LEVEL_NOM,        {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}},
  {  0  }
};

/*
 * ClockDomainFMax_DISPCC_DISPCCMDSSMDP
 *
 * Used by:
 *   ClockDomain_DISPCC_DISPCCMDSSMDP
 *   ClockDomain_DISPCC_DISPCCMDSSROT
 */
static const ClockFMaxType ClockDomainFMax_DISPCC_DISPCCMDSSMDP[] =
{
  {   19200000, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },
  {  171428571, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },
  {  300000000, RAIL_VOLTAGE_LEVEL_SVS,        },
  {  345000000, RAIL_VOLTAGE_LEVEL_SVS_L1,     },
  {  460000000, RAIL_VOLTAGE_LEVEL_NOM,        },
  {  0  }
};

/*
 * ClockDomainFMax_DISPCC_DISPCCMDSSPCLK0
 *
 * Used by:
 *   ClockDomain_DISPCC_DISPCCMDSSPCLK0
 *   ClockDomain_DISPCC_DISPCCMDSSPCLK1
 */
static const ClockFMaxType ClockDomainFMax_DISPCC_DISPCCMDSSPCLK0[] =
{
  {   19200000, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },
  {  300000000, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },
  {  480000000, RAIL_VOLTAGE_LEVEL_SVS,        },
  {  625000000, RAIL_VOLTAGE_LEVEL_SVS_L1,     },
  {  0  }
};

/*
 * ClockDomainFMax_DISPCC_DISPCCPLLTESTMUX
 *
 * Used by:
 *   ClockDomain_DISPCC_DISPCCPLLTESTMUX
 */
static const ClockFMaxType ClockDomainFMax_DISPCC_DISPCCPLLTESTMUX[] =
{
  {  500000000, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },
  { 1000000000, RAIL_VOLTAGE_LEVEL_SVS_L1,     },
  {  0  }
};

/*
 * ClockDomainFMax_DISPCC_DISPCCSLEEP
 *
 * Used by:
 *   ClockDomain_DISPCC_DISPCCSLEEP
 */
static const ClockFMaxType ClockDomainFMax_DISPCC_DISPCCSLEEP[] =
{
  {      32000, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },
  {  0  }
};

/*
 * ClockDomainFMax_GCC_GCCCPUSSAHB
 *
 * Used by:
 *   ClockDomain_GCC_GCCCPUSSAHB
 */
static const ClockFMaxType ClockDomainFMax_GCC_GCCCPUSSAHB[] =
{
  {   19200000, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },
  {   50000000, RAIL_VOLTAGE_LEVEL_SVS,        },
  {  100000000, RAIL_VOLTAGE_LEVEL_NOM,        },
  {  0  }
};

/*
 * ClockDomainFMax_GCC_GCCEMACPTP
 *
 * Used by:
 *   ClockDomain_GCC_GCCEMACPTP
 *   ClockDomain_GCC_GCCEMACRGMII
 */
static const ClockFMaxType ClockDomainFMax_GCC_GCCEMACPTP[] =
{
  {   19200000, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },
  {   50000000, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },
  {  125000000, RAIL_VOLTAGE_LEVEL_SVS,        },
  {  250000000, RAIL_VOLTAGE_LEVEL_NOM,        },
  {  0  }
};

/*
 * ClockDomainFMax_GCC_GCCGP1
 *
 * Used by:
 *   ClockDomain_GCC_GCCGP1
 *   ClockDomain_GCC_GCCGP2
 *   ClockDomain_GCC_GCCGP3
 *   ClockDomain_GCC_GCCGP4
 *   ClockDomain_GCC_GCCGP5
 */
static const ClockFMaxType ClockDomainFMax_GCC_GCCGP1[] =
{
  {   19200000, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },
  {   50000000, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },
  {  100000000, RAIL_VOLTAGE_LEVEL_SVS,        },
  {  200000000, RAIL_VOLTAGE_LEVEL_NOM,        },
  {  0  }
};

/*
 * ClockDomainFMax_GCC_GCCPCIE0AUX
 *
 * Used by:
 *   ClockDomain_GCC_GCCPCIE0AUX
 *   ClockDomain_GCC_GCCPCIE1AUX
 *   ClockDomain_GCC_GCCPCIE2AUX
 *   ClockDomain_GCC_GCCPCIE3AUX
 */
static const ClockFMaxType ClockDomainFMax_GCC_GCCPCIE0AUX[] =
{
  {    9600000, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },
  {   19200000, RAIL_VOLTAGE_LEVEL_SVS,        },
  {  0  }
};

/*
 * ClockDomainFMax_GCC_GCCPCIE0PIPE
 *
 * Used by:
 *   ClockDomain_GCC_GCCPCIE0PIPE
 *   ClockDomain_GCC_GCCPCIE1PIPE
 *   ClockDomain_GCC_GCCPCIE2PIPE
 *   ClockDomain_GCC_GCCPCIE3PIPE
 */
static const ClockFMaxType ClockDomainFMax_GCC_GCCPCIE0PIPE[] =
{
  {   62500000, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },
  {  125000000, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },
  {  250000000, RAIL_VOLTAGE_LEVEL_NOM,        },
  {  0  }
};

/*
 * ClockDomainFMax_GCC_GCCPCIEPHYREFGEN
 *
 * Used by:
 *   ClockDomain_GCC_GCCPCIEPHYREFGEN
 */
static const ClockFMaxType ClockDomainFMax_GCC_GCCPCIEPHYREFGEN[] =
{
  {   19200000, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },
  {  100000000, RAIL_VOLTAGE_LEVEL_SVS,        },
  {  0  }
};

/*
 * ClockDomainFMax_GCC_GCCPDM2
 *
 * Used by:
 *   ClockDomain_GCC_GCCPDM2
 */
static const ClockFMaxType ClockDomainFMax_GCC_GCCPDM2[] =
{
  {    9600000, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },
  {   19200000, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },
  {   60000000, RAIL_VOLTAGE_LEVEL_SVS,        },
  {  0  }
};

/*
 * ClockDomainFMax_GCC_GCCQSPI1CORE
 *
 * Used by:
 *   ClockDomain_GCC_GCCQSPI1CORE
 *   ClockDomain_GCC_GCCQSPICORE
 */
static const ClockFMaxType ClockDomainFMax_GCC_GCCQSPI1CORE[] =
{
  {   19200000, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },
  {   75000000, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },
  {  150000000, RAIL_VOLTAGE_LEVEL_SVS,        },
  {  300000000, RAIL_VOLTAGE_LEVEL_NOM,        },
  {  0  }
};

/*
 * ClockDomainFMax_GCC_GCCQUPV3WRAP0S0
 *
 * Used by:
 *   ClockDomain_GCC_GCCQUPV3WRAP0S0
 *   ClockDomain_GCC_GCCQUPV3WRAP0S1
 *   ClockDomain_GCC_GCCQUPV3WRAP0S2
 *   ClockDomain_GCC_GCCQUPV3WRAP0S3
 *   ClockDomain_GCC_GCCQUPV3WRAP0S4
 *   ClockDomain_GCC_GCCQUPV3WRAP0S5
 *   ClockDomain_GCC_GCCQUPV3WRAP0S6
 *   ClockDomain_GCC_GCCQUPV3WRAP0S7
 *   ClockDomain_GCC_GCCQUPV3WRAP1S0
 *   ClockDomain_GCC_GCCQUPV3WRAP1S1
 *   ClockDomain_GCC_GCCQUPV3WRAP1S2
 *   ClockDomain_GCC_GCCQUPV3WRAP1S3
 *   ClockDomain_GCC_GCCQUPV3WRAP1S4
 *   ClockDomain_GCC_GCCQUPV3WRAP1S5
 *   ClockDomain_GCC_GCCQUPV3WRAP2S0
 *   ClockDomain_GCC_GCCQUPV3WRAP2S1
 *   ClockDomain_GCC_GCCQUPV3WRAP2S2
 *   ClockDomain_GCC_GCCQUPV3WRAP2S3
 *   ClockDomain_GCC_GCCQUPV3WRAP2S4
 *   ClockDomain_GCC_GCCQUPV3WRAP2S5
 */
static const ClockFMaxType ClockDomainFMax_GCC_GCCQUPV3WRAP0S0[] =
{
  {   50000000, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },
  {   75000000, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },
  {  100000000, RAIL_VOLTAGE_LEVEL_SVS,        },
  {  128000000, RAIL_VOLTAGE_LEVEL_NOM,        },
  {  0  }
};

/*
 * ClockDomainFMax_GCC_GCCSDCC2APPS
 *
 * Used by:
 *   ClockDomain_GCC_GCCSDCC2APPS
 */
static const ClockFMaxType ClockDomainFMax_GCC_GCCSDCC2APPS[] =
{
  {    9600000, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },
  {   19200000, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },
  {  100000000, RAIL_VOLTAGE_LEVEL_SVS,        },
  {  200000000, RAIL_VOLTAGE_LEVEL_SVS_L1,     {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}},
  {  202000000, RAIL_VOLTAGE_LEVEL_SVS_L1,     {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}},
  {  0  }
};

/*
 * ClockDomainFMax_GCC_GCCSDCC4APPS
 *
 * Used by:
 *   ClockDomain_GCC_GCCSDCC4APPS
 */
static const ClockFMaxType ClockDomainFMax_GCC_GCCSDCC4APPS[] =
{
  {    9600000, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },
  {   19200000, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },
  {   37500000, RAIL_VOLTAGE_LEVEL_SVS,        {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}},
  {   50000000, RAIL_VOLTAGE_LEVEL_SVS,        {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}},
  {   50000000, RAIL_VOLTAGE_LEVEL_SVS_L1,     {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}},
  {   75000000, RAIL_VOLTAGE_LEVEL_NOM,        {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}},
  {  100000000, RAIL_VOLTAGE_LEVEL_NOM,        {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}},
  {  0  }
};

/*
 * ClockDomainFMax_GCC_GCCTSIFREF
 *
 * Used by:
 *   ClockDomain_GCC_GCCTSIFREF
 */
static const ClockFMaxType ClockDomainFMax_GCC_GCCTSIFREF[] =
{
  {     105495, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },
  {  0  }
};

/*
 * ClockDomainFMax_GCC_GCCUFSCARD2AXI
 *
 * Used by:
 *   ClockDomain_GCC_GCCUFSCARD2AXI
 *   ClockDomain_GCC_GCCUFSCARD2ICECORE
 *   ClockDomain_GCC_GCCUFSCARD2UNIPROCORE
 *   ClockDomain_GCC_GCCUFSPHYAXI
 *   ClockDomain_GCC_GCCUFSPHYICECORE
 *   ClockDomain_GCC_GCCUFSPHYUNIPROCORE
 */
static const ClockFMaxType ClockDomainFMax_GCC_GCCUFSCARD2AXI[] =
{
  {   37500000, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },
  {   75000000, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },
  {  150000000, RAIL_VOLTAGE_LEVEL_SVS,        },
  {  300000000, RAIL_VOLTAGE_LEVEL_NOM,        },
  {  0  }
};

/*
 * ClockDomainFMax_GCC_GCCUFSCARD2PHYAUX
 *
 * Used by:
 *   ClockDomain_GCC_GCCUFSCARD2PHYAUX
 *   ClockDomain_GCC_GCCUFSPHYPHYAUX
 *   ClockDomain_GCC_GCCUSB3MPPHYAUX
 *   ClockDomain_GCC_GCCUSB3PRIMPHYAUX
 *   ClockDomain_GCC_GCCUSB3SECPHYAUX
 */
static const ClockFMaxType ClockDomainFMax_GCC_GCCUFSCARD2PHYAUX[] =
{
  {   19200000, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },
  {  0  }
};

/*
 * ClockDomainFMax_GCC_GCCUFSCARD2RXSYMBOL0
 *
 * Used by:
 *   ClockDomain_GCC_GCCUFSCARD2RXSYMBOL0
 *   ClockDomain_GCC_GCCUFSCARD2RXSYMBOL1
 *   ClockDomain_GCC_GCCUFSCARD2TXSYMBOL0
 *   ClockDomain_GCC_GCCUFSPHYRXSYMBOL0
 *   ClockDomain_GCC_GCCUFSPHYRXSYMBOL1
 *   ClockDomain_GCC_GCCUFSPHYTXSYMBOL0
 */
static const ClockFMaxType ClockDomainFMax_GCC_GCCUFSCARD2RXSYMBOL0[] =
{
  {   37500000, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },
  {   75000000, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },
  {  150000000, RAIL_VOLTAGE_LEVEL_SVS,        },
  {  300000000, RAIL_VOLTAGE_LEVEL_NOM,        },
  {  0  }
};

/*
 * ClockDomainFMax_GCC_GCCUFSCARDAXI
 *
 * Used by:
 *   ClockDomain_GCC_GCCUFSCARDAXI
 */
static const ClockFMaxType ClockDomainFMax_GCC_GCCUFSCARDAXI[] =
{
  {   50000000, RAIL_VOLTAGE_LEVEL_MIN_SVS,    {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}},
  {  100000000, RAIL_VOLTAGE_LEVEL_SVS,        {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}},
  {  200000000, RAIL_VOLTAGE_LEVEL_NOM,        {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}},
  {  240000000, RAIL_VOLTAGE_LEVEL_TUR,        {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}},
  {  0  }
};

/*
 * ClockDomainFMax_GCC_GCCUFSCARDICECORE
 *
 * Used by:
 *   ClockDomain_GCC_GCCUFSCARDICECORE
 */
static const ClockFMaxType ClockDomainFMax_GCC_GCCUFSCARDICECORE[] =
{
  {   75000000, RAIL_VOLTAGE_LEVEL_MIN_SVS,    {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}},
  {  150000000, RAIL_VOLTAGE_LEVEL_SVS,        {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}},
  {  300000000, RAIL_VOLTAGE_LEVEL_NOM,        {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}},
  {  0  }
};

/*
 * ClockDomainFMax_GCC_GCCUFSCARDPHYAUX
 *
 * Used by:
 *   ClockDomain_GCC_GCCUFSCARDPHYAUX
 */
static const ClockFMaxType ClockDomainFMax_GCC_GCCUFSCARDPHYAUX[] =
{
  {   19200000, RAIL_VOLTAGE_LEVEL_MIN_SVS,    {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}},
  {  0  }
};

/*
 * ClockDomainFMax_GCC_GCCUFSCARDRXSYMBOL0
 *
 * Used by:
 *   ClockDomain_GCC_GCCUFSCARDRXSYMBOL0
 *   ClockDomain_GCC_GCCUFSCARDRXSYMBOL1
 *   ClockDomain_GCC_GCCUFSCARDTXSYMBOL0
 */
static const ClockFMaxType ClockDomainFMax_GCC_GCCUFSCARDRXSYMBOL0[] =
{
  {   37500000, RAIL_VOLTAGE_LEVEL_MIN_SVS,    {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}},
  {   75000000, RAIL_VOLTAGE_LEVEL_SVS,        {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}},
  {  150000000, RAIL_VOLTAGE_LEVEL_NOM,        {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}},
  {  0  }
};

/*
 * ClockDomainFMax_GCC_GCCUFSCARDUNIPROCORE
 *
 * Used by:
 *   ClockDomain_GCC_GCCUFSCARDUNIPROCORE
 */
static const ClockFMaxType ClockDomainFMax_GCC_GCCUFSCARDUNIPROCORE[] =
{
  {   37500000, RAIL_VOLTAGE_LEVEL_MIN_SVS,    {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}},
  {   75000000, RAIL_VOLTAGE_LEVEL_SVS,        {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}},
  {  150000000, RAIL_VOLTAGE_LEVEL_NOM,        {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}},
  {  0  }
};

/*
 * ClockDomainFMax_GCC_GCCUSB30MPMASTER
 *
 * Used by:
 *   ClockDomain_GCC_GCCUSB30MPMASTER
 *   ClockDomain_GCC_GCCUSB30PRIMMASTER
 *   ClockDomain_GCC_GCCUSB30SECMASTER
 */
static const ClockFMaxType ClockDomainFMax_GCC_GCCUSB30MPMASTER[] =
{
  {   33333333, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },
  {   66666667, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },
  {  133333333, RAIL_VOLTAGE_LEVEL_SVS,        },
  {  200000000, RAIL_VOLTAGE_LEVEL_NOM,        },
  {  240000000, RAIL_VOLTAGE_LEVEL_TUR,        },
  {  0  }
};

/*
 * ClockDomainFMax_GCC_GCCUSB30MPMOCKUTMI
 *
 * Used by:
 *   ClockDomain_GCC_GCCUSB30MPMOCKUTMI
 *   ClockDomain_GCC_GCCUSB30PRIMMOCKUTMI
 *   ClockDomain_GCC_GCCUSB30SECMOCKUTMI
 */
static const ClockFMaxType ClockDomainFMax_GCC_GCCUSB30MPMOCKUTMI[] =
{
  {   40000000, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },
  {   60000000, RAIL_VOLTAGE_LEVEL_SVS,        },
  {  0  }
};

/*
 * ClockDomainFMax_GCC_GCCUSB3MPPHYPIPE0
 *
 * Used by:
 *   ClockDomain_GCC_GCCUSB3MPPHYPIPE0
 *   ClockDomain_GCC_GCCUSB3MPPHYPIPE1
 *   ClockDomain_GCC_GCCUSB3PRIMPHYPIPE
 *   ClockDomain_GCC_GCCUSB3SECPHYPIPE
 */
static const ClockFMaxType ClockDomainFMax_GCC_GCCUSB3MPPHYPIPE0[] =
{
  {   62500000, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },
  {  125000000, RAIL_VOLTAGE_LEVEL_SVS,        },
  {  312500000, RAIL_VOLTAGE_LEVEL_NOM,        },
  {  0  }
};

/*
 * ClockDomainFMax_GPUCC_GPUCCDEBUGMUX
 *
 * Used by:
 *   ClockDomain_GPUCC_GPUCCDEBUGMUX
 */
static const ClockFMaxType ClockDomainFMax_GPUCC_GPUCCDEBUGMUX[] =
{
  {  116000000, RAIL_VOLTAGE_LEVEL_MIN_SVS,    {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}},
  {  156000000, RAIL_VOLTAGE_LEVEL_MIN_SVS,    {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}},
  {  177000000, RAIL_VOLTAGE_LEVEL_LOW_SVS,    {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}},
  {  235000000, RAIL_VOLTAGE_LEVEL_LOW_SVS,    {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}},
  {  256000000, RAIL_VOLTAGE_LEVEL_SVS,        {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}},
  {  315000000, RAIL_VOLTAGE_LEVEL_SVS,        {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}},
  {  315000000, RAIL_VOLTAGE_LEVEL_SVS_L1,     {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}},
  {  392000000, RAIL_VOLTAGE_LEVEL_SVS_L1,     {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}},
  {  405000000, RAIL_VOLTAGE_LEVEL_NOM,        {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}},
  {  461000000, RAIL_VOLTAGE_LEVEL_NOM_L1,     {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}},
  {  500000000, RAIL_VOLTAGE_LEVEL_TUR,        {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}},
  {  514000000, RAIL_VOLTAGE_LEVEL_TUR_L1,     {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}},
  {  530000000, RAIL_VOLTAGE_LEVEL_NOM,        {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}},
  {  595000000, RAIL_VOLTAGE_LEVEL_NOM_L1,     {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}},
  {  625000000, RAIL_VOLTAGE_LEVEL_TUR,        {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}},
  {  670000000, RAIL_VOLTAGE_LEVEL_TUR_L1,     {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}},
  {  0  }
};

/*
 * ClockDomainFMax_GPUCC_GPUCCGMU
 *
 * Used by:
 *   ClockDomain_GPUCC_GPUCCGMU
 */
static const ClockFMaxType ClockDomainFMax_GPUCC_GPUCCGMU[] =
{
  {  200000000, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },
  {  400000000, RAIL_VOLTAGE_LEVEL_SVS,        {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}},
  {  500000000, RAIL_VOLTAGE_LEVEL_SVS,        {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}},
  {  500000000, RAIL_VOLTAGE_LEVEL_SVS_L1,     {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}},
  {  0  }
};

/*
 * ClockDomainFMax_GPUCC_GPUCCGXGFX3D
 *
 * Used by:
 *   ClockDomain_GPUCC_GPUCCGXGFX3D
 */
static const ClockFMaxType ClockDomainFMax_GPUCC_GPUCCGXGFX3D[] =
{
  {  116000000, RAIL_VOLTAGE_LEVEL_MIN_SVS,    {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}},
  {  156000000, RAIL_VOLTAGE_LEVEL_MIN_SVS,    {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}},
  {  177000000, RAIL_VOLTAGE_LEVEL_LOW_SVS,    {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}},
  {  235000000, RAIL_VOLTAGE_LEVEL_LOW_SVS,    {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}},
  {  256000000, RAIL_VOLTAGE_LEVEL_SVS,        {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}},
  {  315000000, RAIL_VOLTAGE_LEVEL_SVS,        {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}},
  {  315000000, RAIL_VOLTAGE_LEVEL_SVS_L1,     {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}},
  {  392000000, RAIL_VOLTAGE_LEVEL_SVS_L1,     {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}},
  {  405000000, RAIL_VOLTAGE_LEVEL_NOM,        {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}},
  {  461000000, RAIL_VOLTAGE_LEVEL_NOM_L1,     {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}},
  {  500000000, RAIL_VOLTAGE_LEVEL_TUR,        {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}},
  {  514000000, RAIL_VOLTAGE_LEVEL_TUR_L1,     {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}},
  {  530000000, RAIL_VOLTAGE_LEVEL_NOM,        {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}},
  {  595000000, RAIL_VOLTAGE_LEVEL_NOM_L1,     {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}},
  {  625000000, RAIL_VOLTAGE_LEVEL_TUR,        {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}},
  {  670000000, RAIL_VOLTAGE_LEVEL_TUR_L1,     {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}},
  {  0  }
};

/*
 * ClockDomainFMax_GPUCC_GPUCCPLLTESTMUX
 *
 * Used by:
 *   ClockDomain_GPUCC_GPUCCPLLTESTMUX
 */
static const ClockFMaxType ClockDomainFMax_GPUCC_GPUCCPLLTESTMUX[] =
{
  {   92500000, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },
  {  142500000, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },
  {  179000000, RAIL_VOLTAGE_LEVEL_SVS,        },
  {  250000000, RAIL_VOLTAGE_LEVEL_SVS_L1,     },
  {  0  }
};

/*
 * ClockDomainFMax_NPUCC_NPUCCCALDP
 *
 * Used by:
 *   ClockDomain_NPUCC_NPUCCCALDP
 */
static const ClockFMaxType ClockDomainFMax_NPUCC_NPUCCCALDP[] =
{
  {  300000000, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },
  {  350000000, RAIL_VOLTAGE_LEVEL_SVS,        {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}},
  {  400000000, RAIL_VOLTAGE_LEVEL_SVS,        {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}},
  {  400000000, RAIL_VOLTAGE_LEVEL_SVS_L1,     {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}},
  {  487000000, RAIL_VOLTAGE_LEVEL_SVS_L1,     {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}},
  {  600000000, RAIL_VOLTAGE_LEVEL_NOM,        {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}},
  {  652000000, RAIL_VOLTAGE_LEVEL_NOM,        {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}},
  {  715000000, RAIL_VOLTAGE_LEVEL_TUR,        {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}},
  {  773000000, RAIL_VOLTAGE_LEVEL_NOM_L1,     {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}},
  {  811000000, RAIL_VOLTAGE_LEVEL_TUR,        {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}},
  {  908000000, RAIL_VOLTAGE_LEVEL_TUR_L1,     {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}},
  {  0  }
};

/*
 * ClockDomainFMax_NPUCC_NPUCCDEBUGMUX
 *
 * Used by:
 *   ClockDomain_NPUCC_NPUCCDEBUGMUX
 */
static const ClockFMaxType ClockDomainFMax_NPUCC_NPUCCDEBUGMUX[] =
{
  {  300000000, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },
  {  350000000, RAIL_VOLTAGE_LEVEL_SVS,        {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}},
  {  400000000, RAIL_VOLTAGE_LEVEL_SVS,        {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}},
  {  400000000, RAIL_VOLTAGE_LEVEL_SVS_L1,     {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}},
  {  487000000, RAIL_VOLTAGE_LEVEL_SVS_L1,     {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}},
  {  600000000, RAIL_VOLTAGE_LEVEL_NOM,        {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}},
  {  652000000, RAIL_VOLTAGE_LEVEL_NOM,        {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}},
  {  715000000, RAIL_VOLTAGE_LEVEL_TUR,        {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}},
  {  773000000, RAIL_VOLTAGE_LEVEL_NOM_L1,     {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}},
  {  811000000, RAIL_VOLTAGE_LEVEL_TUR,        {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}},
  {  908000000, RAIL_VOLTAGE_LEVEL_TUR_L1,     {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}},
  {  0  }
};

/*
 * ClockDomainFMax_NPUCC_NPUCCNPUCORE
 *
 * Used by:
 *   ClockDomain_NPUCC_NPUCCNPUCORE
 */
static const ClockFMaxType ClockDomainFMax_NPUCC_NPUCCNPUCORE[] =
{
  {  100000000, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },
  {  150000000, RAIL_VOLTAGE_LEVEL_SVS,        },
  {  200000000, RAIL_VOLTAGE_LEVEL_SVS_L1,     },
  {  300000000, RAIL_VOLTAGE_LEVEL_NOM,        },
  {  350000000, RAIL_VOLTAGE_LEVEL_TUR,        {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}},
  {  400000000, RAIL_VOLTAGE_LEVEL_TUR,        {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}},
  {  0  }
};

/*
 * ClockDomainFMax_NPUCC_NPUCCPLLTESTMUX
 *
 * Used by:
 *   ClockDomain_NPUCC_NPUCCPLLTESTMUX
 */
static const ClockFMaxType ClockDomainFMax_NPUCC_NPUCCPLLTESTMUX[] =
{
  {  600000000, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },
  {  700000000, RAIL_VOLTAGE_LEVEL_SVS,        {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}},
  {  800000000, RAIL_VOLTAGE_LEVEL_SVS,        {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}},
  {  800000000, RAIL_VOLTAGE_LEVEL_SVS_L1,     {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}},
  {  974000000, RAIL_VOLTAGE_LEVEL_SVS_L1,     {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}},
  { 1200000000, RAIL_VOLTAGE_LEVEL_NOM,        {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}},
  { 1304000000, RAIL_VOLTAGE_LEVEL_NOM,        {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}},
  { 1430000000, RAIL_VOLTAGE_LEVEL_TUR,        {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}},
  { 1546000000, RAIL_VOLTAGE_LEVEL_NOM_L1,     {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}},
  { 1622000000, RAIL_VOLTAGE_LEVEL_TUR,        {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}},
  { 1816000000, RAIL_VOLTAGE_LEVEL_TUR_L1,     {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}},
  {  0  }
};
#ifdef ENABLE_AUTO_PLAT
/*
 * ClockDomainFMax_SCC_SCCMAINRCG
 *
 * Used by:
 *   ClockDomain_SCC_SCCMAINRCG
 */
static const ClockFMaxType ClockDomainFMax_SCC_SCCMAINRCG[] =
{
  {   96000000, RAIL_VOLTAGE_LEVEL_MIN_SVS,    {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}},
  {  100000000, RAIL_VOLTAGE_LEVEL_MIN_SVS,    {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}},
  {  200000000, RAIL_VOLTAGE_LEVEL_NOM,        {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}},
  {  576000000, RAIL_VOLTAGE_LEVEL_SVS,        {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}},
  {  0  }
};

/*
 * ClockDomainFMax_SCC_SCCQUPV3SE0
 *
 * Used by:
 *   ClockDomain_SCC_SCCQUPV3SE0
 *   ClockDomain_SCC_SCCQUPV3SE1
 *   ClockDomain_SCC_SCCQUPV3SE2
 *   ClockDomain_SCC_SCCQUPV3SE3
 *   ClockDomain_SCC_SCCQUPV3SE4
 *   ClockDomain_SCC_SCCQUPV3SE5
 */
static const ClockFMaxType ClockDomainFMax_SCC_SCCQUPV3SE0[] =
{
  {   48000000, RAIL_VOLTAGE_LEVEL_MIN_SVS,    {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}},
  {   50000000, RAIL_VOLTAGE_LEVEL_MIN_SVS,    {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}},
  {   80000000, RAIL_VOLTAGE_LEVEL_LOW_SVS,    {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}},
  {   96000000, RAIL_VOLTAGE_LEVEL_LOW_SVS,    {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}},
  {  120000000, RAIL_VOLTAGE_LEVEL_SVS,        {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}},
  {  128000000, RAIL_VOLTAGE_LEVEL_SVS,        {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}},
  {  144000000, RAIL_VOLTAGE_LEVEL_SVS_L1,     {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}},
  {  150000000, RAIL_VOLTAGE_LEVEL_NOM,        {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}},
  {  192000000, RAIL_VOLTAGE_LEVEL_NOM,        {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}},
  {  0  }
};
#endif
/*
 * ClockDomainFMax_VIDEOCC_VIDEOCCIRIS
 *
 * Used by:
 *   ClockDomain_VIDEOCC_VIDEOCCIRIS
 */
static const ClockFMaxType ClockDomainFMax_VIDEOCC_VIDEOCCIRIS[] =
{
  {  200000000, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },
  {  225000000, RAIL_VOLTAGE_LEVEL_LOW_SVS,    {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}},
  {  240000000, RAIL_VOLTAGE_LEVEL_LOW_SVS,    {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}},
  {  300000000, RAIL_VOLTAGE_LEVEL_SVS,        {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}},
  {  338000000, RAIL_VOLTAGE_LEVEL_SVS,        {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}},
  {  365000000, RAIL_VOLTAGE_LEVEL_SVS_L1,     },
  {  432000000, RAIL_VOLTAGE_LEVEL_NOM,        {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}},
  {  444000000, RAIL_VOLTAGE_LEVEL_NOM,        {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}},
  {  480000000, RAIL_VOLTAGE_LEVEL_TUR,        {{0x01, 0x00}, {0x02, 0x00}, CHIPINFO_FAMILY_SC8180X}},
  {  533000000, RAIL_VOLTAGE_LEVEL_TUR,        {{0x02, 0x00}, {0xFF, 0xFF}, CHIPINFO_FAMILY_SC8180X}},
  {  0  }
};

/*
 * ClockDomainFMax_VIDEOCC_VIDEOCCSLEEP
 *
 * Used by:
 *   ClockDomain_VIDEOCC_VIDEOCCSLEEP
 */
static const ClockFMaxType ClockDomainFMax_VIDEOCC_VIDEOCCSLEEP[] =
{
  {      32000, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },
  {  0  }
};

/*
 * ClockDomainFMax_VIDEOCC_VIDEOCCXO
 *
 * Used by:
 *   ClockDomain_VIDEOCC_VIDEOCCXO
 */
static const ClockFMaxType ClockDomainFMax_VIDEOCC_VIDEOCCXO[] =
{
  {   19200000, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },
  {  0  }
};


/*=========================================================================
      Domains
==========================================================================*/

static ClockDomainNodeType ClockDomain_CAMCC_CAMCCBPS =
{
  .aBSP      = ClockDomainBSP_CAMCC_CAMCCBPS,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_CAMCC_CAMCCBPS),
  .pRail     = &ClockRail_VDDMM,
  .aFMax     = ClockDomainFMax_CAMCC_CAMCCBPS,
  .aSourceMap = ClockSourceMap_CAMCC_CAMCCBPS,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(CAM_CC_BPS_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_CAMCC_CAMCCCAMNOCAXI =
{
  .aBSP      = ClockDomainBSP_CAMCC_CAMCCCAMNOCAXI,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_CAMCC_CAMCCCAMNOCAXI),
  .pRail     = &ClockRail_VDDMM,
  .aFMax     = ClockDomainFMax_CAMCC_CAMCCCAMNOCAXI,
  .aSourceMap = ClockSourceMap_CAMCC_CAMCCBPS,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(CAM_CC_CAMNOC_AXI_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_CAMCC_CAMCCCCI0 =
{
  .aBSP      = ClockDomainBSP_CAMCC_CAMCCCCI0,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_CAMCC_CAMCCCCI0),
  .pRail     = &ClockRail_VDDMM,
  .aFMax     = ClockDomainFMax_CAMCC_CAMCCCCI0,
  .aSourceMap = ClockSourceMap_CAMCC_CAMCCBPS,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(CAM_CC_CCI_0_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 8,
  },
};

static ClockDomainNodeType ClockDomain_CAMCC_CAMCCCCI1 =
{
  .aBSP      = ClockDomainBSP_CAMCC_CAMCCCCI0,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_CAMCC_CAMCCCCI0),
  .pRail     = &ClockRail_VDDMM,
  .aFMax     = ClockDomainFMax_CAMCC_CAMCCCCI0,
  .aSourceMap = ClockSourceMap_CAMCC_CAMCCBPS,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(CAM_CC_CCI_1_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 8,
  },
};

static ClockDomainNodeType ClockDomain_CAMCC_CAMCCCCI2 =
{
  .aBSP      = ClockDomainBSP_CAMCC_CAMCCCCI0,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_CAMCC_CAMCCCCI0),
  .pRail     = &ClockRail_VDDMM,
  .aFMax     = ClockDomainFMax_CAMCC_CAMCCCCI0,
  .aSourceMap = ClockSourceMap_CAMCC_CAMCCBPS,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(CAM_CC_CCI_2_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 8,
  },
};

static ClockDomainNodeType ClockDomain_CAMCC_CAMCCCCI3 =
{
  .aBSP      = ClockDomainBSP_CAMCC_CAMCCCCI0,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_CAMCC_CAMCCCCI0),
  .pRail     = &ClockRail_VDDMM,
  .aFMax     = ClockDomainFMax_CAMCC_CAMCCCCI0,
  .aSourceMap = ClockSourceMap_CAMCC_CAMCCBPS,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(CAM_CC_CCI_3_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 8,
  },
};

static ClockDomainNodeType ClockDomain_CAMCC_CAMCCCPHYRX =
{
  .aBSP      = ClockDomainBSP_CAMCC_CAMCCCPHYRX,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_CAMCC_CAMCCCPHYRX),
  .pRail     = &ClockRail_VDDMM,
  .aFMax     = ClockDomainFMax_CAMCC_CAMCCCPHYRX,
  .aSourceMap = ClockSourceMap_CAMCC_CAMCCBPS,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(CAM_CC_CPHY_RX_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_CAMCC_CAMCCCSI0PHYTIMER =
{
  .aBSP      = ClockDomainBSP_CAMCC_CAMCCCSI0PHYTIMER,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_CAMCC_CAMCCCSI0PHYTIMER),
  .pRail     = &ClockRail_VDDMM,
  .aFMax     = ClockDomainFMax_CAMCC_CAMCCCSI0PHYTIMER,
  .aSourceMap = ClockSourceMap_CAMCC_CAMCCBPS,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(CAM_CC_CSI0PHYTIMER_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_CAMCC_CAMCCCSI1PHYTIMER =
{
  .aBSP      = ClockDomainBSP_CAMCC_CAMCCCSI0PHYTIMER,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_CAMCC_CAMCCCSI0PHYTIMER),
  .pRail     = &ClockRail_VDDMM,
  .aFMax     = ClockDomainFMax_CAMCC_CAMCCCSI0PHYTIMER,
  .aSourceMap = ClockSourceMap_CAMCC_CAMCCBPS,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(CAM_CC_CSI1PHYTIMER_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_CAMCC_CAMCCCSI2PHYTIMER =
{
  .aBSP      = ClockDomainBSP_CAMCC_CAMCCCSI0PHYTIMER,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_CAMCC_CAMCCCSI0PHYTIMER),
  .pRail     = &ClockRail_VDDMM,
  .aFMax     = ClockDomainFMax_CAMCC_CAMCCCSI0PHYTIMER,
  .aSourceMap = ClockSourceMap_CAMCC_CAMCCBPS,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(CAM_CC_CSI2PHYTIMER_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_CAMCC_CAMCCCSI3PHYTIMER =
{
  .aBSP      = ClockDomainBSP_CAMCC_CAMCCCSI0PHYTIMER,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_CAMCC_CAMCCCSI0PHYTIMER),
  .pRail     = &ClockRail_VDDMM,
  .aFMax     = ClockDomainFMax_CAMCC_CAMCCCSI0PHYTIMER,
  .aSourceMap = ClockSourceMap_CAMCC_CAMCCBPS,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(CAM_CC_CSI3PHYTIMER_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_CAMCC_CAMCCDEBUGMUX =
{
  .aBSP      = NULL,
  .nBSPLen   = 0,
  .pRail     = &ClockRail_VDDMM,
  .aFMax     = ClockDomainFMax_CAMCC_CAMCCDEBUGMUX,
  .aSourceMap = ClockSourceMap_CAMCC_CAMCCDEBUGMUX,
  .pSource   = NULL,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(CAM_CC_DEBUG_MUX_MUXR),
    .pmControl  = &HAL_clk_GenericDomainMuxControl,
  },
};

static ClockDomainNodeType ClockDomain_CAMCC_CAMCCFASTAHB =
{
  .aBSP      = ClockDomainBSP_CAMCC_CAMCCFASTAHB,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_CAMCC_CAMCCFASTAHB),
  .pRail     = &ClockRail_VDDMM,
  .aFMax     = ClockDomainFMax_CAMCC_CAMCCFASTAHB,
  .aSourceMap = ClockSourceMap_CAMCC_CAMCCBPS,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(CAM_CC_FAST_AHB_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_CAMCC_CAMCCFDCORE =
{
  .aBSP      = ClockDomainBSP_CAMCC_CAMCCFDCORE,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_CAMCC_CAMCCFDCORE),
  .pRail     = &ClockRail_VDDMM,
  .aFMax     = ClockDomainFMax_CAMCC_CAMCCFDCORE,
  .aSourceMap = ClockSourceMap_CAMCC_CAMCCBPS,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(CAM_CC_FD_CORE_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_CAMCC_CAMCCICP =
{
  .aBSP      = ClockDomainBSP_CAMCC_CAMCCICP,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_CAMCC_CAMCCICP),
  .pRail     = &ClockRail_VDDMM,
  .aFMax     = ClockDomainFMax_CAMCC_CAMCCICP,
  .aSourceMap = ClockSourceMap_CAMCC_CAMCCBPS,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(CAM_CC_ICP_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_CAMCC_CAMCCIFE0 =
{
  .aBSP      = ClockDomainBSP_CAMCC_CAMCCIFE0,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_CAMCC_CAMCCIFE0),
  .pRail     = &ClockRail_VDDMM,
  .aFMax     = ClockDomainFMax_CAMCC_CAMCCIFE0,
  .aSourceMap = ClockSourceMap_CAMCC_CAMCCIFE0,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(CAM_CC_IFE_0_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_CAMCC_CAMCCIFE0CSID =
{
  .aBSP      = ClockDomainBSP_CAMCC_CAMCCIFE0CSID,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_CAMCC_CAMCCIFE0CSID),
  .pRail     = &ClockRail_VDDMM,
  .aFMax     = ClockDomainFMax_CAMCC_CAMCCFDCORE,
  .aSourceMap = ClockSourceMap_CAMCC_CAMCCBPS,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(CAM_CC_IFE_0_CSID_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_CAMCC_CAMCCIFE1 =
{
  .aBSP      = ClockDomainBSP_CAMCC_CAMCCIFE1,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_CAMCC_CAMCCIFE1),
  .pRail     = &ClockRail_VDDMM,
  .aFMax     = ClockDomainFMax_CAMCC_CAMCCIFE0,
  .aSourceMap = ClockSourceMap_CAMCC_CAMCCIFE1,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(CAM_CC_IFE_1_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_CAMCC_CAMCCIFE1CSID =
{
  .aBSP      = ClockDomainBSP_CAMCC_CAMCCIFE0CSID,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_CAMCC_CAMCCIFE0CSID),
  .pRail     = &ClockRail_VDDMM,
  .aFMax     = ClockDomainFMax_CAMCC_CAMCCFDCORE,
  .aSourceMap = ClockSourceMap_CAMCC_CAMCCBPS,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(CAM_CC_IFE_1_CSID_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_CAMCC_CAMCCIFE2 =
{
  .aBSP      = ClockDomainBSP_CAMCC_CAMCCIFE2,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_CAMCC_CAMCCIFE2),
  .pRail     = &ClockRail_VDDMM,
  .aFMax     = ClockDomainFMax_CAMCC_CAMCCIFE0,
  .aSourceMap = ClockSourceMap_CAMCC_CAMCCIFE2,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(CAM_CC_IFE_2_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_CAMCC_CAMCCIFE2CSID =
{
  .aBSP      = ClockDomainBSP_CAMCC_CAMCCFDCORE,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_CAMCC_CAMCCFDCORE),
  .pRail     = &ClockRail_VDDMM,
  .aFMax     = ClockDomainFMax_CAMCC_CAMCCFDCORE,
  .aSourceMap = ClockSourceMap_CAMCC_CAMCCBPS,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(CAM_CC_IFE_2_CSID_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_CAMCC_CAMCCIFE3 =
{
  .aBSP      = ClockDomainBSP_CAMCC_CAMCCIFE3,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_CAMCC_CAMCCIFE3),
  .pRail     = &ClockRail_VDDMM,
  .aFMax     = ClockDomainFMax_CAMCC_CAMCCIFE0,
  .aSourceMap = ClockSourceMap_CAMCC_CAMCCIFE3,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(CAM_CC_IFE_3_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_CAMCC_CAMCCIFE3CSID =
{
  .aBSP      = ClockDomainBSP_CAMCC_CAMCCFDCORE,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_CAMCC_CAMCCFDCORE),
  .pRail     = &ClockRail_VDDMM,
  .aFMax     = ClockDomainFMax_CAMCC_CAMCCFDCORE,
  .aSourceMap = ClockSourceMap_CAMCC_CAMCCBPS,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(CAM_CC_IFE_3_CSID_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_CAMCC_CAMCCIFELITE0 =
{
  .aBSP      = ClockDomainBSP_CAMCC_CAMCCIFELITE0,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_CAMCC_CAMCCIFELITE0),
  .pRail     = &ClockRail_VDDMM,
  .aFMax     = ClockDomainFMax_CAMCC_CAMCCIFELITE0,
  .aSourceMap = ClockSourceMap_CAMCC_CAMCCBPS,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(CAM_CC_IFE_LITE_0_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_CAMCC_CAMCCIFELITE0CSID =
{
  .aBSP      = ClockDomainBSP_CAMCC_CAMCCFDCORE,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_CAMCC_CAMCCFDCORE),
  .pRail     = &ClockRail_VDDMM,
  .aFMax     = ClockDomainFMax_CAMCC_CAMCCFDCORE,
  .aSourceMap = ClockSourceMap_CAMCC_CAMCCBPS,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(CAM_CC_IFE_LITE_0_CSID_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_CAMCC_CAMCCIFELITE1 =
{
  .aBSP      = ClockDomainBSP_CAMCC_CAMCCIFELITE0,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_CAMCC_CAMCCIFELITE0),
  .pRail     = &ClockRail_VDDMM,
  .aFMax     = ClockDomainFMax_CAMCC_CAMCCIFELITE0,
  .aSourceMap = ClockSourceMap_CAMCC_CAMCCBPS,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(CAM_CC_IFE_LITE_1_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_CAMCC_CAMCCIFELITE1CSID =
{
  .aBSP      = ClockDomainBSP_CAMCC_CAMCCFDCORE,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_CAMCC_CAMCCFDCORE),
  .pRail     = &ClockRail_VDDMM,
  .aFMax     = ClockDomainFMax_CAMCC_CAMCCFDCORE,
  .aSourceMap = ClockSourceMap_CAMCC_CAMCCBPS,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(CAM_CC_IFE_LITE_1_CSID_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_CAMCC_CAMCCIFELITE2 =
{
  .aBSP      = ClockDomainBSP_CAMCC_CAMCCIFELITE0,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_CAMCC_CAMCCIFELITE0),
  .pRail     = &ClockRail_VDDMM,
  .aFMax     = ClockDomainFMax_CAMCC_CAMCCIFELITE0,
  .aSourceMap = ClockSourceMap_CAMCC_CAMCCBPS,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(CAM_CC_IFE_LITE_2_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_CAMCC_CAMCCIFELITE2CSID =
{
  .aBSP      = ClockDomainBSP_CAMCC_CAMCCFDCORE,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_CAMCC_CAMCCFDCORE),
  .pRail     = &ClockRail_VDDMM,
  .aFMax     = ClockDomainFMax_CAMCC_CAMCCFDCORE,
  .aSourceMap = ClockSourceMap_CAMCC_CAMCCBPS,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(CAM_CC_IFE_LITE_2_CSID_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_CAMCC_CAMCCIFELITE3 =
{
  .aBSP      = ClockDomainBSP_CAMCC_CAMCCIFELITE0,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_CAMCC_CAMCCIFELITE0),
  .pRail     = &ClockRail_VDDMM,
  .aFMax     = ClockDomainFMax_CAMCC_CAMCCIFELITE0,
  .aSourceMap = ClockSourceMap_CAMCC_CAMCCBPS,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(CAM_CC_IFE_LITE_3_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_CAMCC_CAMCCIFELITE3CSID =
{
  .aBSP      = ClockDomainBSP_CAMCC_CAMCCFDCORE,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_CAMCC_CAMCCFDCORE),
  .pRail     = &ClockRail_VDDMM,
  .aFMax     = ClockDomainFMax_CAMCC_CAMCCFDCORE,
  .aSourceMap = ClockSourceMap_CAMCC_CAMCCBPS,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(CAM_CC_IFE_LITE_3_CSID_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_CAMCC_CAMCCIPE0 =
{
  .aBSP      = ClockDomainBSP_CAMCC_CAMCCIPE0,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_CAMCC_CAMCCIPE0),
  .pRail     = &ClockRail_VDDMM,
  .aFMax     = ClockDomainFMax_CAMCC_CAMCCIPE0,
  .aSourceMap = ClockSourceMap_CAMCC_CAMCCIPE0,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(CAM_CC_IPE_0_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_CAMCC_CAMCCJPEG =
{
  .aBSP      = ClockDomainBSP_CAMCC_CAMCCBPS,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_CAMCC_CAMCCBPS),
  .pRail     = &ClockRail_VDDMM,
  .aFMax     = ClockDomainFMax_CAMCC_CAMCCJPEG,
  .aSourceMap = ClockSourceMap_CAMCC_CAMCCBPS,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(CAM_CC_JPEG_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_CAMCC_CAMCCLRME =
{
  .aBSP      = ClockDomainBSP_CAMCC_CAMCCLRME,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_CAMCC_CAMCCLRME),
  .pRail     = &ClockRail_VDDMM,
  .aFMax     = ClockDomainFMax_CAMCC_CAMCCLRME,
  .aSourceMap = ClockSourceMap_CAMCC_CAMCCBPS,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(CAM_CC_LRME_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_CAMCC_CAMCCMCLK0 =
{
  .aBSP      = ClockDomainBSP_CAMCC_CAMCCMCLK0,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_CAMCC_CAMCCMCLK0),
  .pRail     = &ClockRail_VDDMX,
  .aFMax     = ClockDomainFMax_CAMCC_CAMCCMCLK0,
  .aSourceMap = ClockSourceMap_CAMCC_CAMCCMCLK0,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(CAM_CC_MCLK0_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 8,
  },
};

static ClockDomainNodeType ClockDomain_CAMCC_CAMCCMCLK1 =
{
  .aBSP      = ClockDomainBSP_CAMCC_CAMCCMCLK0,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_CAMCC_CAMCCMCLK0),
  .pRail     = &ClockRail_VDDMX,
  .aFMax     = ClockDomainFMax_CAMCC_CAMCCMCLK0,
  .aSourceMap = ClockSourceMap_CAMCC_CAMCCMCLK0,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(CAM_CC_MCLK1_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 8,
  },
};

static ClockDomainNodeType ClockDomain_CAMCC_CAMCCMCLK2 =
{
  .aBSP      = ClockDomainBSP_CAMCC_CAMCCMCLK0,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_CAMCC_CAMCCMCLK0),
  .pRail     = &ClockRail_VDDMX,
  .aFMax     = ClockDomainFMax_CAMCC_CAMCCMCLK0,
  .aSourceMap = ClockSourceMap_CAMCC_CAMCCMCLK0,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(CAM_CC_MCLK2_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 8,
  },
};

static ClockDomainNodeType ClockDomain_CAMCC_CAMCCMCLK3 =
{
  .aBSP      = ClockDomainBSP_CAMCC_CAMCCMCLK0,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_CAMCC_CAMCCMCLK0),
  .pRail     = &ClockRail_VDDMX,
  .aFMax     = ClockDomainFMax_CAMCC_CAMCCMCLK0,
  .aSourceMap = ClockSourceMap_CAMCC_CAMCCMCLK0,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(CAM_CC_MCLK3_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 8,
  },
};

static ClockDomainNodeType ClockDomain_CAMCC_CAMCCMCLK4 =
{
  .aBSP      = ClockDomainBSP_CAMCC_CAMCCMCLK0,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_CAMCC_CAMCCMCLK0),
  .pRail     = &ClockRail_VDDMX,
  .aFMax     = ClockDomainFMax_CAMCC_CAMCCMCLK0,
  .aSourceMap = ClockSourceMap_CAMCC_CAMCCMCLK0,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(CAM_CC_MCLK4_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 8,
  },
};

static ClockDomainNodeType ClockDomain_CAMCC_CAMCCMCLK5 =
{
  .aBSP      = ClockDomainBSP_CAMCC_CAMCCMCLK0,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_CAMCC_CAMCCMCLK0),
  .pRail     = &ClockRail_VDDMX,
  .aFMax     = ClockDomainFMax_CAMCC_CAMCCMCLK0,
  .aSourceMap = ClockSourceMap_CAMCC_CAMCCMCLK0,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(CAM_CC_MCLK5_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 8,
  },
};

static ClockDomainNodeType ClockDomain_CAMCC_CAMCCMCLK6 =
{
  .aBSP      = ClockDomainBSP_CAMCC_CAMCCMCLK0,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_CAMCC_CAMCCMCLK0),
  .pRail     = &ClockRail_VDDMX,
  .aFMax     = ClockDomainFMax_CAMCC_CAMCCMCLK0,
  .aSourceMap = ClockSourceMap_CAMCC_CAMCCMCLK0,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(CAM_CC_MCLK6_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 8,
  },
};

static ClockDomainNodeType ClockDomain_CAMCC_CAMCCMCLK7 =
{
  .aBSP      = ClockDomainBSP_CAMCC_CAMCCMCLK0,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_CAMCC_CAMCCMCLK0),
  .pRail     = &ClockRail_VDDMX,
  .aFMax     = ClockDomainFMax_CAMCC_CAMCCMCLK0,
  .aSourceMap = ClockSourceMap_CAMCC_CAMCCMCLK0,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(CAM_CC_MCLK7_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 8,
  },
};

static ClockDomainNodeType ClockDomain_CAMCC_CAMCCPLLTESTMUX =
{
  .aBSP      = NULL,
  .nBSPLen   = 0,
  .pRail     = &ClockRail_VDDMX,
  .aFMax     = ClockDomainFMax_CAMCC_CAMCCPLLTESTMUX,
  .aSourceMap = ClockSourceMap_CAMCC_CAMCCPLLTESTMUX,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(CAM_CC_PLL_TEST_MUX_MUXR),
    .pmControl  = &HAL_clk_GenericDomainMuxControl,
  },
};

static ClockDomainNodeType ClockDomain_CAMCC_CAMCCQDSSDEBUG =
{
  .aBSP      = ClockDomainBSP_CAMCC_CAMCCQDSSDEBUG,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_CAMCC_CAMCCQDSSDEBUG),
  .pRail     = &ClockRail_VDDMM,
  .aFMax     = ClockDomainFMax_CAMCC_CAMCCQDSSDEBUG,
  .aSourceMap = ClockSourceMap_CAMCC_CAMCCBPS,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(CAM_CC_QDSS_DEBUG_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_CAMCC_CAMCCSLEEP =
{
  .aBSP      = ClockDomainBSP_CAMCC_CAMCCSLEEP,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_CAMCC_CAMCCSLEEP),
  .pRail     = &ClockRail_VDDMM,
  .aFMax     = ClockDomainFMax_CAMCC_CAMCCSLEEP,
  .aSourceMap = ClockSourceMap_CAMCC_CAMCCSLEEP,
  .pSource   = NULL,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(CAM_CC_SLEEP_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_CAMCC_CAMCCSLOWAHB =
{
  .aBSP      = ClockDomainBSP_CAMCC_CAMCCSLOWAHB,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_CAMCC_CAMCCSLOWAHB),
  .pRail     = &ClockRail_VDDMM,
  .aFMax     = ClockDomainFMax_CAMCC_CAMCCSLOWAHB,
  .aSourceMap = ClockSourceMap_CAMCC_CAMCCBPS,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(CAM_CC_SLOW_AHB_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 8,
  },
};

static ClockDomainNodeType ClockDomain_CAMCC_CAMCCXO =
{
  .aBSP      = ClockDomainBSP_CAMCC_CAMCCXO,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_CAMCC_CAMCCXO),
  .pRail     = &ClockRail_VDDMM,
  .aFMax     = ClockDomainFMax_CAMCC_CAMCCXO,
  .aSourceMap = ClockSourceMap_CAMCC_CAMCCXO,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(CAM_CC_XO_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_DISPCC_DISPCCMDSSAHB =
{
  .aBSP      = ClockDomainBSP_DISPCC_DISPCCMDSSAHB,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_DISPCC_DISPCCMDSSAHB),
  .pRail     = &ClockRail_VDDMM,
  .aFMax     = ClockDomainFMax_DISPCC_DISPCCMDSSAHB,
  .aSourceMap = ClockSourceMap_DISPCC_DISPCCMDSSAHB,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(DISP_CC_MDSS_AHB_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_DISPCC_DISPCCMDSSBYTE0 =
{
  .aBSP      = ClockDomainBSP_DISPCC_DISPCCMDSSBYTE0,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_DISPCC_DISPCCMDSSBYTE0),
  .pRail     = &ClockRail_VDDMM,
  .aFMax     = ClockDomainFMax_DISPCC_DISPCCMDSSBYTE0,
  .aSourceMap = ClockSourceMap_DISPCC_DISPCCMDSSBYTE0,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(DISP_CC_MDSS_BYTE0_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_DISPCC_DISPCCMDSSBYTE1 =
{
  .aBSP      = ClockDomainBSP_DISPCC_DISPCCMDSSBYTE0,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_DISPCC_DISPCCMDSSBYTE0),
  .pRail     = &ClockRail_VDDMM,
  .aFMax     = ClockDomainFMax_DISPCC_DISPCCMDSSBYTE0,
  .aSourceMap = ClockSourceMap_DISPCC_DISPCCMDSSBYTE0,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(DISP_CC_MDSS_BYTE1_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_DISPCC_DISPCCMDSSDPAUX1 =
{
  .aBSP      = ClockDomainBSP_DISPCC_DISPCCMDSSBYTE0,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_DISPCC_DISPCCMDSSBYTE0),
  .pRail     = &ClockRail_VDDMM,
  .aFMax     = ClockDomainFMax_DISPCC_DISPCCMDSSDPAUX1,
  .aSourceMap = ClockSourceMap_DISPCC_DISPCCMDSSDPAUX1,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(DISP_CC_MDSS_DP_AUX1_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_DISPCC_DISPCCMDSSDPAUX =
{
  .aBSP      = ClockDomainBSP_DISPCC_DISPCCMDSSBYTE0,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_DISPCC_DISPCCMDSSBYTE0),
  .pRail     = &ClockRail_VDDMM,
  .aFMax     = ClockDomainFMax_DISPCC_DISPCCMDSSDPAUX1,
  .aSourceMap = ClockSourceMap_DISPCC_DISPCCMDSSDPAUX1,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(DISP_CC_MDSS_DP_AUX_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_DISPCC_DISPCCMDSSDPCRYPTO1 =
{
  .aBSP      = ClockDomainBSP_DISPCC_DISPCCMDSSDPCRYPTO1,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_DISPCC_DISPCCMDSSDPCRYPTO1),
  .pRail     = &ClockRail_VDDMM,
  .aFMax     = ClockDomainFMax_DISPCC_DISPCCMDSSDPCRYPTO1,
  .aSourceMap = ClockSourceMap_DISPCC_DISPCCMDSSDPCRYPTO1,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(DISP_CC_MDSS_DP_CRYPTO1_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_DISPCC_DISPCCMDSSDPCRYPTO =
{
  .aBSP      = ClockDomainBSP_DISPCC_DISPCCMDSSDPCRYPTO1,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_DISPCC_DISPCCMDSSDPCRYPTO1),
  .pRail     = &ClockRail_VDDMM,
  .aFMax     = ClockDomainFMax_DISPCC_DISPCCMDSSDPCRYPTO1,
  .aSourceMap = ClockSourceMap_DISPCC_DISPCCMDSSDPCRYPTO1,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(DISP_CC_MDSS_DP_CRYPTO_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_DISPCC_DISPCCMDSSDPLINK1 =
{
  .aBSP      = ClockDomainBSP_DISPCC_DISPCCMDSSBYTE0,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_DISPCC_DISPCCMDSSBYTE0),
  .pRail     = &ClockRail_VDDMM,
  .aFMax     = ClockDomainFMax_DISPCC_DISPCCMDSSDPLINK1,
  .aSourceMap = ClockSourceMap_DISPCC_DISPCCMDSSDPCRYPTO1,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(DISP_CC_MDSS_DP_LINK1_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_DISPCC_DISPCCMDSSDPLINK =
{
  .aBSP      = ClockDomainBSP_DISPCC_DISPCCMDSSBYTE0,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_DISPCC_DISPCCMDSSBYTE0),
  .pRail     = &ClockRail_VDDMM,
  .aFMax     = ClockDomainFMax_DISPCC_DISPCCMDSSDPLINK1,
  .aSourceMap = ClockSourceMap_DISPCC_DISPCCMDSSDPCRYPTO1,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(DISP_CC_MDSS_DP_LINK_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_DISPCC_DISPCCMDSSDPPIXEL1 =
{
  .aBSP      = ClockDomainBSP_DISPCC_DISPCCMDSSBYTE0,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_DISPCC_DISPCCMDSSBYTE0),
  .pRail     = &ClockRail_VDDMM,
  .aFMax     = ClockDomainFMax_DISPCC_DISPCCMDSSDPPIXEL1,
  .aSourceMap = ClockSourceMap_DISPCC_DISPCCMDSSDPCRYPTO1,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(DISP_CC_MDSS_DP_PIXEL1_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 16,
  },
};

static ClockDomainNodeType ClockDomain_DISPCC_DISPCCMDSSDPPIXEL2 =
{
  .aBSP      = ClockDomainBSP_DISPCC_DISPCCMDSSBYTE0,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_DISPCC_DISPCCMDSSBYTE0),
  .pRail     = &ClockRail_VDDMM,
  .aFMax     = ClockDomainFMax_DISPCC_DISPCCMDSSDPPIXEL1,
  .aSourceMap = ClockSourceMap_DISPCC_DISPCCMDSSDPCRYPTO1,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(DISP_CC_MDSS_DP_PIXEL2_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 16,
  },
};

static ClockDomainNodeType ClockDomain_DISPCC_DISPCCMDSSDPPIXEL =
{
  .aBSP      = ClockDomainBSP_DISPCC_DISPCCMDSSBYTE0,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_DISPCC_DISPCCMDSSBYTE0),
  .pRail     = &ClockRail_VDDMM,
  .aFMax     = ClockDomainFMax_DISPCC_DISPCCMDSSDPPIXEL1,
  .aSourceMap = ClockSourceMap_DISPCC_DISPCCMDSSDPCRYPTO1,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(DISP_CC_MDSS_DP_PIXEL_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 16,
  },
};

static ClockDomainNodeType ClockDomain_DISPCC_DISPCCMDSSEDPAUX =
{
  .aBSP      = ClockDomainBSP_DISPCC_DISPCCMDSSBYTE0,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_DISPCC_DISPCCMDSSBYTE0),
  .pRail     = &ClockRail_VDDMM,
  .aFMax     = ClockDomainFMax_DISPCC_DISPCCMDSSDPAUX1,
  .aSourceMap = ClockSourceMap_DISPCC_DISPCCMDSSDPAUX1,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(DISP_CC_MDSS_EDP_AUX_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_DISPCC_DISPCCMDSSEDPGTC =
{
  .aBSP      = ClockDomainBSP_DISPCC_DISPCCMDSSEDPGTC,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_DISPCC_DISPCCMDSSEDPGTC),
  .pRail     = &ClockRail_VDDMM,
  .aFMax     = ClockDomainFMax_DISPCC_DISPCCMDSSDPAUX1,
  .aSourceMap = ClockSourceMap_DISPCC_DISPCCMDSSAHB,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(DISP_CC_MDSS_EDP_GTC_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_DISPCC_DISPCCMDSSEDPLINK =
{
  .aBSP      = ClockDomainBSP_DISPCC_DISPCCMDSSBYTE0,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_DISPCC_DISPCCMDSSBYTE0),
  .pRail     = &ClockRail_VDDMM,
  .aFMax     = ClockDomainFMax_DISPCC_DISPCCMDSSEDPLINK,
  .aSourceMap = ClockSourceMap_DISPCC_DISPCCMDSSEDPLINK,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(DISP_CC_MDSS_EDP_LINK_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_DISPCC_DISPCCMDSSEDPPIXEL =
{
  .aBSP      = ClockDomainBSP_DISPCC_DISPCCMDSSBYTE0,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_DISPCC_DISPCCMDSSBYTE0),
  .pRail     = &ClockRail_VDDMM,
  .aFMax     = ClockDomainFMax_DISPCC_DISPCCMDSSEDPPIXEL,
  .aSourceMap = ClockSourceMap_DISPCC_DISPCCMDSSEDPLINK,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(DISP_CC_MDSS_EDP_PIXEL_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 16,
  },
};

static ClockDomainNodeType ClockDomain_DISPCC_DISPCCMDSSESC0 =
{
  .aBSP      = ClockDomainBSP_DISPCC_DISPCCMDSSESC0,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_DISPCC_DISPCCMDSSESC0),
  .pRail     = &ClockRail_VDDMM,
  .aFMax     = ClockDomainFMax_DISPCC_DISPCCMDSSDPAUX1,
  .aSourceMap = ClockSourceMap_DISPCC_DISPCCMDSSBYTE0,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(DISP_CC_MDSS_ESC0_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_DISPCC_DISPCCMDSSESC1 =
{
  .aBSP      = ClockDomainBSP_DISPCC_DISPCCMDSSESC0,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_DISPCC_DISPCCMDSSESC0),
  .pRail     = &ClockRail_VDDMM,
  .aFMax     = ClockDomainFMax_DISPCC_DISPCCMDSSDPAUX1,
  .aSourceMap = ClockSourceMap_DISPCC_DISPCCMDSSBYTE0,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(DISP_CC_MDSS_ESC1_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_DISPCC_DISPCCMDSSMDP =
{
  .aBSP      = ClockDomainBSP_DISPCC_DISPCCMDSSMDP,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_DISPCC_DISPCCMDSSMDP),
  .pRail     = &ClockRail_VDDMM,
  .aFMax     = ClockDomainFMax_DISPCC_DISPCCMDSSMDP,
  .aSourceMap = ClockSourceMap_DISPCC_DISPCCMDSSMDP,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(DISP_CC_MDSS_MDP_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nFlags     = HAL_CLK_FLAG_DOMAIN_HW_CLK_CONTROL,
  },
};

static ClockDomainNodeType ClockDomain_DISPCC_DISPCCMDSSPCLK0 =
{
  .aBSP      = ClockDomainBSP_DISPCC_DISPCCMDSSBYTE0,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_DISPCC_DISPCCMDSSBYTE0),
  .pRail     = &ClockRail_VDDMM,
  .aFMax     = ClockDomainFMax_DISPCC_DISPCCMDSSPCLK0,
  .aSourceMap = ClockSourceMap_DISPCC_DISPCCMDSSPCLK0,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(DISP_CC_MDSS_PCLK0_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 8,
  },
};

static ClockDomainNodeType ClockDomain_DISPCC_DISPCCMDSSPCLK1 =
{
  .aBSP      = ClockDomainBSP_DISPCC_DISPCCMDSSBYTE0,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_DISPCC_DISPCCMDSSBYTE0),
  .pRail     = &ClockRail_VDDMM,
  .aFMax     = ClockDomainFMax_DISPCC_DISPCCMDSSPCLK0,
  .aSourceMap = ClockSourceMap_DISPCC_DISPCCMDSSPCLK0,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(DISP_CC_MDSS_PCLK1_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 8,
  },
};

static ClockDomainNodeType ClockDomain_DISPCC_DISPCCMDSSROT =
{
  .aBSP      = ClockDomainBSP_DISPCC_DISPCCMDSSROT,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_DISPCC_DISPCCMDSSROT),
  .pRail     = &ClockRail_VDDMM,
  .aFMax     = ClockDomainFMax_DISPCC_DISPCCMDSSMDP,
  .aSourceMap = ClockSourceMap_DISPCC_DISPCCMDSSMDP,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(DISP_CC_MDSS_ROT_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_DISPCC_DISPCCMDSSVSYNC =
{
  .aBSP      = ClockDomainBSP_DISPCC_DISPCCMDSSBYTE0,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_DISPCC_DISPCCMDSSBYTE0),
  .pRail     = &ClockRail_VDDMM,
  .aFMax     = ClockDomainFMax_DISPCC_DISPCCMDSSDPAUX1,
  .aSourceMap = ClockSourceMap_DISPCC_DISPCCMDSSDPAUX1,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(DISP_CC_MDSS_VSYNC_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_DISPCC_DISPCCPLLTESTMUX =
{
  .aBSP      = NULL,
  .nBSPLen   = 0,
  .pRail     = &ClockRail_VDDMM,
  .aFMax     = ClockDomainFMax_DISPCC_DISPCCPLLTESTMUX,
  .aSourceMap = ClockSourceMap_DISPCC_DISPCCPLLTESTMUX,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(DISP_CC_PLL_TEST_MUX_MUXR),
    .pmControl  = &HAL_clk_GenericDomainMuxControl,
  },
};

static ClockDomainNodeType ClockDomain_DISPCC_DISPCCSLEEP =
{
  .aBSP      = ClockDomainBSP_DISPCC_DISPCCSLEEP,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_DISPCC_DISPCCSLEEP),
  .pRail     = &ClockRail_VDDMM,
  .aFMax     = ClockDomainFMax_DISPCC_DISPCCSLEEP,
  .aSourceMap = ClockSourceMap_DISPCC_DISPCCSLEEP,
  .pSource   = NULL,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(DISP_CC_SLEEP_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_DISPCC_DISPCCXO =
{
  .aBSP      = ClockDomainBSP_DISPCC_DISPCCMDSSBYTE0,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_DISPCC_DISPCCMDSSBYTE0),
  .pRail     = &ClockRail_VDDMM,
  .aFMax     = ClockDomainFMax_DISPCC_DISPCCMDSSDPAUX1,
  .aSourceMap = ClockSourceMap_DISPCC_DISPCCMDSSDPAUX1,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(DISP_CC_XO_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_GCC_CLOCKDRIVERGENXO =
{
  .aBSP      = NULL,
  .nBSPLen   = 0,
  .pRail     = NULL,
  .pSource   = &ClockSource_XO,
  .nFlags    = CLOCK_FLAG_READ_ONLY,
  .HALDomain = {
    .nCGRAddr   = 0,
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_GCC_CLOCKDRIVERGENNOSOURCE =
{
  .aBSP      = NULL,
  .nBSPLen   = 0,
  .pRail     = NULL,
  .pSource   = NULL,
  .nFlags    = CLOCK_FLAG_READ_ONLY,
  .HALDomain = {
    .nCGRAddr   = 0,
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCAGGRENOC =
{
  .aBSP      = NULL,
  .nBSPLen   = 0,
  .pRail     = &ClockRail_VDDCX,
  .pSource   = &ClockSource_XO,
  .nFlags    = CLOCK_FLAG_READ_ONLY,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(GCC_AGGRE_NOC_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCAGGRENOCSOUTHSF =
{
  .aBSP      = NULL,
  .nBSPLen   = 0,
  .pRail     = &ClockRail_VDDCX,
  .pSource   = &ClockSource_XO,
  .nFlags    = CLOCK_FLAG_READ_ONLY,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(GCC_AGGRE_NOC_SOUTH_SF_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCCNOCPERIPH =
{
  .aBSP      = NULL,
  .nBSPLen   = 0,
  .pRail     = &ClockRail_VDDCX,
  .aFMax     = ClockDomainFMax_GCC_GCCCPUSSAHB,
  .aSourceMap = ClockSourceMap_GCC_GCCCPUSSAHB,
  .pSource   = &ClockSource_XO,
  .nFlags    = CLOCK_FLAG_READ_ONLY,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(GCC_CNOC_PERIPH_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCCONFIGNOC =
{
  .aBSP      = NULL,
  .nBSPLen   = 0,
  .pRail     = &ClockRail_VDDCX,
  .aSourceMap = ClockSourceMap_GCC_GCCCPUSSAHB,
  .pSource   = &ClockSource_XO,
  .nFlags    = CLOCK_FLAG_READ_ONLY,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(GCC_CONFIG_NOC_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCCPUSSAHB =
{
  .aBSP      = ClockDomainBSP_GCC_GCCCPUSSAHB,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_GCC_GCCCPUSSAHB),
  .pRail     = &ClockRail_VDDCX,
  .aFMax     = ClockDomainFMax_GCC_GCCCPUSSAHB,
  .aSourceMap = ClockSourceMap_GCC_GCCCPUSSAHB,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(GCC_CPUSS_AHB_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCCPUSSRBCPR =
{
  .aBSP      = NULL,
  .nBSPLen   = 0,
  .pRail     = &ClockRail_VDDCX,
  .aFMax     = ClockDomainFMax_GCC_GCCUFSCARD2PHYAUX,
  .aSourceMap = ClockSourceMap_GCC_GCCSDCC4APPS,
  .pSource   = &ClockSource_XO,
  .nFlags    = CLOCK_FLAG_READ_ONLY,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(GCC_CPUSS_RBCPR_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCEMACPTP =
{
  .aBSP      = ClockDomainBSP_GCC_GCCEMACPTP,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_GCC_GCCEMACPTP),
  .pRail     = &ClockRail_VDDCX,
  .aFMax     = ClockDomainFMax_GCC_GCCEMACPTP,
  .aSourceMap = ClockSourceMap_GCC_GCCEMACPTP,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(GCC_EMAC_PTP_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCEMACRGMII =
{
  .aBSP      = ClockDomainBSP_GCC_GCCEMACRGMII,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_GCC_GCCEMACRGMII),
  .pRail     = &ClockRail_VDDCX,
  .aFMax     = ClockDomainFMax_GCC_GCCEMACPTP,
  .aSourceMap = ClockSourceMap_GCC_GCCEMACPTP,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(GCC_EMAC_RGMII_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 8,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCGP1 =
{
  .aBSP      = ClockDomainBSP_GCC_GCCGP1,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_GCC_GCCGP1),
  .pRail     = &ClockRail_VDDCX,
  .aFMax     = ClockDomainFMax_GCC_GCCGP1,
  .aSourceMap = ClockSourceMap_GCC_GCCGP1,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(GCC_GP1_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 8,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCGP2 =
{
  .aBSP      = ClockDomainBSP_GCC_GCCGP1,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_GCC_GCCGP1),
  .pRail     = &ClockRail_VDDCX,
  .aFMax     = ClockDomainFMax_GCC_GCCGP1,
  .aSourceMap = ClockSourceMap_GCC_GCCGP1,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(GCC_GP2_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 8,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCGP3 =
{
  .aBSP      = ClockDomainBSP_GCC_GCCGP1,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_GCC_GCCGP1),
  .pRail     = &ClockRail_VDDCX,
  .aFMax     = ClockDomainFMax_GCC_GCCGP1,
  .aSourceMap = ClockSourceMap_GCC_GCCGP1,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(GCC_GP3_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 8,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCGP4 =
{
  .aBSP      = ClockDomainBSP_GCC_GCCGP1,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_GCC_GCCGP1),
  .pRail     = &ClockRail_VDDCX,
  .aFMax     = ClockDomainFMax_GCC_GCCGP1,
  .aSourceMap = ClockSourceMap_GCC_GCCGP1,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(GCC_GP4_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 8,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCGP5 =
{
  .aBSP      = ClockDomainBSP_GCC_GCCGP1,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_GCC_GCCGP1),
  .pRail     = &ClockRail_VDDCX,
  .aFMax     = ClockDomainFMax_GCC_GCCGP1,
  .aSourceMap = ClockSourceMap_GCC_GCCGP1,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(GCC_GP5_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 8,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCGPUMEMNOCGFX =
{
  .aBSP      = NULL,
  .nBSPLen   = 0,
  .pRail     = &ClockRail_VDDCX,
  .pSource   = &ClockSource_XO,
  .nFlags    = CLOCK_FLAG_READ_ONLY,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(GCC_GPU_MEMNOC_GFX_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCMEMNOC =
{
  .aBSP      = NULL,
  .nBSPLen   = 0,
  .pRail     = &ClockRail_VDDCX,
  .pSource   = &ClockSource_XO,
  .nFlags    = CLOCK_FLAG_READ_ONLY,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(GCC_MEMNOC_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCMMNOCHFQX =
{
  .aBSP      = NULL,
  .nBSPLen   = 0,
  .pRail     = &ClockRail_VDDCX,
  .pSource   = &ClockSource_XO,
  .nFlags    = CLOCK_FLAG_READ_ONLY,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(GCC_MMNOC_HF_QX_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCMMNOCSFQX =
{
  .aBSP      = NULL,
  .nBSPLen   = 0,
  .pRail     = &ClockRail_VDDCX,
  .pSource   = &ClockSource_XO,
  .nFlags    = CLOCK_FLAG_READ_ONLY,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(GCC_MMNOC_SF_QX_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCNPUAXI =
{
  .aBSP      = NULL,
  .nBSPLen   = 0,
  .pRail     = &ClockRail_VDDCX,
  .pSource   = &ClockSource_XO,
  .nFlags    = CLOCK_FLAG_READ_ONLY,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(GCC_NPU_AXI_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCPCIE0AUX =
{
  .aBSP      = ClockDomainBSP_GCC_GCCPCIE0AUX,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_GCC_GCCPCIE0AUX),
  .pRail     = &ClockRail_VDDCX,
  .aFMax     = ClockDomainFMax_GCC_GCCPCIE0AUX,
  .aSourceMap = ClockSourceMap_GCC_GCCPCIE0AUX,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(GCC_PCIE_0_AUX_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 16,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCPCIE0PIPE =
{
  .aBSP      = NULL,
  .nBSPLen   = 0,
  .pRail     = &ClockRail_VDDCX,
  .aFMax     = ClockDomainFMax_GCC_GCCPCIE0PIPE,
  .aSourceMap = ClockSourceMap_GCC_GCCPCIE0PIPE,
  .pSource   = NULL,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(GCC_PCIE_0_PIPE_MUXR),
    .pmControl  = &HAL_clk_GenericDomainMuxControl,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCPCIE1AUX =
{
  .aBSP      = ClockDomainBSP_GCC_GCCPCIE0AUX,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_GCC_GCCPCIE0AUX),
  .pRail     = &ClockRail_VDDCX,
  .aFMax     = ClockDomainFMax_GCC_GCCPCIE0AUX,
  .aSourceMap = ClockSourceMap_GCC_GCCPCIE0AUX,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(GCC_PCIE_1_AUX_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 16,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCPCIE1PIPE =
{
  .aBSP      = NULL,
  .nBSPLen   = 0,
  .pRail     = &ClockRail_VDDCX,
  .aFMax     = ClockDomainFMax_GCC_GCCPCIE0PIPE,
  .aSourceMap = ClockSourceMap_GCC_GCCPCIE1PIPE,
  .pSource   = NULL,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(GCC_PCIE_1_PIPE_MUXR),
    .pmControl  = &HAL_clk_GenericDomainMuxControl,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCPCIE2AUX =
{
  .aBSP      = ClockDomainBSP_GCC_GCCPCIE0AUX,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_GCC_GCCPCIE0AUX),
  .pRail     = &ClockRail_VDDCX,
  .aFMax     = ClockDomainFMax_GCC_GCCPCIE0AUX,
  .aSourceMap = ClockSourceMap_GCC_GCCPCIE0AUX,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(GCC_PCIE_2_AUX_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 16,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCPCIE2PIPE =
{
  .aBSP      = NULL,
  .nBSPLen   = 0,
  .pRail     = &ClockRail_VDDCX,
  .aFMax     = ClockDomainFMax_GCC_GCCPCIE0PIPE,
  .aSourceMap = ClockSourceMap_GCC_GCCPCIE2PIPE,
  .pSource   = NULL,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(GCC_PCIE_2_PIPE_MUXR),
    .pmControl  = &HAL_clk_GenericDomainMuxControl,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCPCIE3AUX =
{
  .aBSP      = ClockDomainBSP_GCC_GCCPCIE0AUX,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_GCC_GCCPCIE0AUX),
  .pRail     = &ClockRail_VDDCX,
  .aFMax     = ClockDomainFMax_GCC_GCCPCIE0AUX,
  .aSourceMap = ClockSourceMap_GCC_GCCPCIE0AUX,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(GCC_PCIE_3_AUX_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 16,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCPCIE3PIPE =
{
  .aBSP      = NULL,
  .nBSPLen   = 0,
  .pRail     = &ClockRail_VDDCX,
  .aFMax     = ClockDomainFMax_GCC_GCCPCIE0PIPE,
  .aSourceMap = ClockSourceMap_GCC_GCCPCIE3PIPE,
  .pSource   = NULL,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(GCC_PCIE_3_PIPE_MUXR),
    .pmControl  = &HAL_clk_GenericDomainMuxControl,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCPCIEPHYREFGEN =
{
  .aBSP      = ClockDomainBSP_GCC_GCCPCIEPHYREFGEN,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_GCC_GCCPCIEPHYREFGEN),
  .pRail     = &ClockRail_VDDCX,
  .aFMax     = ClockDomainFMax_GCC_GCCPCIEPHYREFGEN,
  .aSourceMap = ClockSourceMap_GCC_GCCCPUSSAHB,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(GCC_PCIE_PHY_REFGEN_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCPDM2 =
{
  .aBSP      = ClockDomainBSP_GCC_GCCPDM2,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_GCC_GCCPDM2),
  .pRail     = &ClockRail_VDDCX,
  .aFMax     = ClockDomainFMax_GCC_GCCPDM2,
  .aSourceMap = ClockSourceMap_GCC_GCCCPUSSAHB,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(GCC_PDM2_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCQDSSATBA =
{
  .aBSP      = NULL,
  .nBSPLen   = 0,
  .pRail     = &ClockRail_VDDCX,
  .aSourceMap = ClockSourceMap_GCC_GCCGP1,
  .pSource   = &ClockSource_XO,
  .nFlags    = CLOCK_FLAG_READ_ONLY,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(GCC_QDSS_ATB_A_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCQDSSATBB =
{
  .aBSP      = NULL,
  .nBSPLen   = 0,
  .pRail     = &ClockRail_VDDCX,
  .aSourceMap = ClockSourceMap_GCC_GCCGP1,
  .pSource   = &ClockSource_XO,
  .nFlags    = CLOCK_FLAG_READ_ONLY,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(GCC_QDSS_ATB_B_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCQDSSTRIG =
{
  .aBSP      = NULL,
  .nBSPLen   = 0,
  .pRail     = &ClockRail_VDDCX,
  .aSourceMap = ClockSourceMap_GCC_GCCCPUSSAHB,
  .pSource   = &ClockSource_XO,
  .nFlags    = CLOCK_FLAG_READ_ONLY,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(GCC_QDSS_TRIG_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCQSPI1CORE =
{
  .aBSP      = ClockDomainBSP_GCC_GCCQSPI1CORE,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_GCC_GCCQSPI1CORE),
  .pRail     = &ClockRail_VDDCX,
  .aFMax     = ClockDomainFMax_GCC_GCCQSPI1CORE,
  .aSourceMap = ClockSourceMap_GCC_GCCCPUSSAHB,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(GCC_QSPI_1_CORE_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCQSPICORE =
{
  .aBSP      = ClockDomainBSP_GCC_GCCQSPI1CORE,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_GCC_GCCQSPI1CORE),
  .pRail     = &ClockRail_VDDCX,
  .aFMax     = ClockDomainFMax_GCC_GCCQSPI1CORE,
  .aSourceMap = ClockSourceMap_GCC_GCCCPUSSAHB,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(GCC_QSPI_CORE_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCQUPV3WRAP0CORE2X =
{
  .aBSP      = NULL,
  .nBSPLen   = 0,
  .pRail     = &ClockRail_VDDCX,
  .pSource   = &ClockSource_XO,
  .nFlags    = CLOCK_FLAG_READ_ONLY,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(GCC_QUPV3_WRAP0_CORE_2X_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCQUPV3WRAP0S0 =
{
  .aBSP      = ClockDomainBSP_GCC_GCCQUPV3WRAP0S0,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_GCC_GCCQUPV3WRAP0S0),
  .pRail     = &ClockRail_VDDCX,
  .aFMax     = ClockDomainFMax_GCC_GCCQUPV3WRAP0S0,
  .aSourceMap = ClockSourceMap_GCC_GCCCPUSSAHB,
  .pSource   = &ClockSource_XO,
  .nFlags    = CLOCK_FLAG_DOMAIN_WAIT_FOR_ROOT_OFF,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(GCC_QUPV3_WRAP0_S0_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 16,
    .nDFSStates = 8,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCQUPV3WRAP0S1 =
{
  .aBSP      = ClockDomainBSP_GCC_GCCQUPV3WRAP0S0,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_GCC_GCCQUPV3WRAP0S0),
  .pRail     = &ClockRail_VDDCX,
  .aFMax     = ClockDomainFMax_GCC_GCCQUPV3WRAP0S0,
  .aSourceMap = ClockSourceMap_GCC_GCCCPUSSAHB,
  .pSource   = &ClockSource_XO,
  .nFlags    = CLOCK_FLAG_DOMAIN_WAIT_FOR_ROOT_OFF,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(GCC_QUPV3_WRAP0_S1_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 16,
    .nDFSStates = 8,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCQUPV3WRAP0S2 =
{
  .aBSP      = ClockDomainBSP_GCC_GCCQUPV3WRAP0S0,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_GCC_GCCQUPV3WRAP0S0),
  .pRail     = &ClockRail_VDDCX,
  .aFMax     = ClockDomainFMax_GCC_GCCQUPV3WRAP0S0,
  .aSourceMap = ClockSourceMap_GCC_GCCCPUSSAHB,
  .pSource   = &ClockSource_XO,
  .nFlags    = CLOCK_FLAG_DOMAIN_WAIT_FOR_ROOT_OFF,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(GCC_QUPV3_WRAP0_S2_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 16,
    .nDFSStates = 8,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCQUPV3WRAP0S3 =
{
  .aBSP      = ClockDomainBSP_GCC_GCCQUPV3WRAP0S0,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_GCC_GCCQUPV3WRAP0S0),
  .pRail     = &ClockRail_VDDCX,
  .aFMax     = ClockDomainFMax_GCC_GCCQUPV3WRAP0S0,
  .aSourceMap = ClockSourceMap_GCC_GCCCPUSSAHB,
  .pSource   = &ClockSource_XO,
  .nFlags    = CLOCK_FLAG_DOMAIN_WAIT_FOR_ROOT_OFF,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(GCC_QUPV3_WRAP0_S3_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 16,
    .nDFSStates = 8,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCQUPV3WRAP0S4 =
{
  .aBSP      = ClockDomainBSP_GCC_GCCQUPV3WRAP0S0,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_GCC_GCCQUPV3WRAP0S0),
  .pRail     = &ClockRail_VDDCX,
  .aFMax     = ClockDomainFMax_GCC_GCCQUPV3WRAP0S0,
  .aSourceMap = ClockSourceMap_GCC_GCCCPUSSAHB,
  .pSource   = &ClockSource_XO,
  .nFlags    = CLOCK_FLAG_DOMAIN_WAIT_FOR_ROOT_OFF,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(GCC_QUPV3_WRAP0_S4_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 16,
    .nDFSStates = 8,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCQUPV3WRAP0S5 =
{
  .aBSP      = ClockDomainBSP_GCC_GCCQUPV3WRAP0S0,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_GCC_GCCQUPV3WRAP0S0),
  .pRail     = &ClockRail_VDDCX,
  .aFMax     = ClockDomainFMax_GCC_GCCQUPV3WRAP0S0,
  .aSourceMap = ClockSourceMap_GCC_GCCCPUSSAHB,
  .pSource   = &ClockSource_XO,
  .nFlags    = CLOCK_FLAG_DOMAIN_WAIT_FOR_ROOT_OFF,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(GCC_QUPV3_WRAP0_S5_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 16,
    .nDFSStates = 8,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCQUPV3WRAP0S6 =
{
  .aBSP      = ClockDomainBSP_GCC_GCCQUPV3WRAP0S0,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_GCC_GCCQUPV3WRAP0S0),
  .pRail     = &ClockRail_VDDCX,
  .aFMax     = ClockDomainFMax_GCC_GCCQUPV3WRAP0S0,
  .aSourceMap = ClockSourceMap_GCC_GCCCPUSSAHB,
  .pSource   = &ClockSource_XO,
  .nFlags    = CLOCK_FLAG_DOMAIN_WAIT_FOR_ROOT_OFF,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(GCC_QUPV3_WRAP0_S6_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 16,
    .nDFSStates = 8,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCQUPV3WRAP0S7 =
{
  .aBSP      = ClockDomainBSP_GCC_GCCQUPV3WRAP0S0,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_GCC_GCCQUPV3WRAP0S0),
  .pRail     = &ClockRail_VDDCX,
  .aFMax     = ClockDomainFMax_GCC_GCCQUPV3WRAP0S0,
  .aSourceMap = ClockSourceMap_GCC_GCCCPUSSAHB,
  .pSource   = &ClockSource_XO,
  .nFlags    = CLOCK_FLAG_DOMAIN_WAIT_FOR_ROOT_OFF,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(GCC_QUPV3_WRAP0_S7_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 16,
    .nDFSStates = 8,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCQUPV3WRAP1CORE2X =
{
  .aBSP      = NULL,
  .nBSPLen   = 0,
  .pRail     = &ClockRail_VDDCX,
  .pSource   = &ClockSource_XO,
  .nFlags    = CLOCK_FLAG_READ_ONLY,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(GCC_QUPV3_WRAP1_CORE_2X_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCQUPV3WRAP1S0 =
{
  .aBSP      = ClockDomainBSP_GCC_GCCQUPV3WRAP0S0,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_GCC_GCCQUPV3WRAP0S0),
  .pRail     = &ClockRail_VDDCX,
  .aFMax     = ClockDomainFMax_GCC_GCCQUPV3WRAP0S0,
  .aSourceMap = ClockSourceMap_GCC_GCCCPUSSAHB,
  .pSource   = &ClockSource_XO,
  .nFlags    = CLOCK_FLAG_DOMAIN_WAIT_FOR_ROOT_OFF,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(GCC_QUPV3_WRAP1_S0_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 16,
    .nDFSStates = 8,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCQUPV3WRAP1S1 =
{
  .aBSP      = ClockDomainBSP_GCC_GCCQUPV3WRAP0S0,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_GCC_GCCQUPV3WRAP0S0),
  .pRail     = &ClockRail_VDDCX,
  .aFMax     = ClockDomainFMax_GCC_GCCQUPV3WRAP0S0,
  .aSourceMap = ClockSourceMap_GCC_GCCCPUSSAHB,
  .pSource   = &ClockSource_XO,
  .nFlags    = CLOCK_FLAG_DOMAIN_WAIT_FOR_ROOT_OFF,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(GCC_QUPV3_WRAP1_S1_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 16,
    .nDFSStates = 8,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCQUPV3WRAP1S2 =
{
  .aBSP      = ClockDomainBSP_GCC_GCCQUPV3WRAP0S0,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_GCC_GCCQUPV3WRAP0S0),
  .pRail     = &ClockRail_VDDCX,
  .aFMax     = ClockDomainFMax_GCC_GCCQUPV3WRAP0S0,
  .aSourceMap = ClockSourceMap_GCC_GCCCPUSSAHB,
  .pSource   = &ClockSource_XO,
  .nFlags    = CLOCK_FLAG_DOMAIN_WAIT_FOR_ROOT_OFF,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(GCC_QUPV3_WRAP1_S2_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 16,
    .nDFSStates = 8,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCQUPV3WRAP1S3 =
{
  .aBSP      = ClockDomainBSP_GCC_GCCQUPV3WRAP0S0,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_GCC_GCCQUPV3WRAP0S0),
  .pRail     = &ClockRail_VDDCX,
  .aFMax     = ClockDomainFMax_GCC_GCCQUPV3WRAP0S0,
  .aSourceMap = ClockSourceMap_GCC_GCCCPUSSAHB,
  .pSource   = &ClockSource_XO,
  .nFlags    = CLOCK_FLAG_DOMAIN_WAIT_FOR_ROOT_OFF,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(GCC_QUPV3_WRAP1_S3_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 16,
    .nDFSStates = 8,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCQUPV3WRAP1S4 =
{
  .aBSP      = ClockDomainBSP_GCC_GCCQUPV3WRAP0S0,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_GCC_GCCQUPV3WRAP0S0),
  .pRail     = &ClockRail_VDDCX,
  .aFMax     = ClockDomainFMax_GCC_GCCQUPV3WRAP0S0,
  .aSourceMap = ClockSourceMap_GCC_GCCCPUSSAHB,
  .pSource   = &ClockSource_XO,
  .nFlags    = CLOCK_FLAG_DOMAIN_WAIT_FOR_ROOT_OFF,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(GCC_QUPV3_WRAP1_S4_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 16,
    .nDFSStates = 8,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCQUPV3WRAP1S5 =
{
  .aBSP      = ClockDomainBSP_GCC_GCCQUPV3WRAP0S0,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_GCC_GCCQUPV3WRAP0S0),
  .pRail     = &ClockRail_VDDCX,
  .aFMax     = ClockDomainFMax_GCC_GCCQUPV3WRAP0S0,
  .aSourceMap = ClockSourceMap_GCC_GCCCPUSSAHB,
  .pSource   = &ClockSource_XO,
  .nFlags    = CLOCK_FLAG_DOMAIN_WAIT_FOR_ROOT_OFF,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(GCC_QUPV3_WRAP1_S5_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 16,
    .nDFSStates = 8,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCQUPV3WRAP2CORE2X =
{
  .aBSP      = NULL,
  .nBSPLen   = 0,
  .pRail     = &ClockRail_VDDCX,
  .pSource   = &ClockSource_XO,
  .nFlags    = CLOCK_FLAG_READ_ONLY,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(GCC_QUPV3_WRAP2_CORE_2X_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCQUPV3WRAP2S0 =
{
  .aBSP      = ClockDomainBSP_GCC_GCCQUPV3WRAP0S0,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_GCC_GCCQUPV3WRAP0S0),
  .pRail     = &ClockRail_VDDCX,
  .aFMax     = ClockDomainFMax_GCC_GCCQUPV3WRAP0S0,
  .aSourceMap = ClockSourceMap_GCC_GCCCPUSSAHB,
  .pSource   = &ClockSource_XO,
  .nFlags    = CLOCK_FLAG_DOMAIN_WAIT_FOR_ROOT_OFF,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(GCC_QUPV3_WRAP2_S0_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 16,
    .nDFSStates = 8,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCQUPV3WRAP2S1 =
{
  .aBSP      = ClockDomainBSP_GCC_GCCQUPV3WRAP0S0,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_GCC_GCCQUPV3WRAP0S0),
  .pRail     = &ClockRail_VDDCX,
  .aFMax     = ClockDomainFMax_GCC_GCCQUPV3WRAP0S0,
  .aSourceMap = ClockSourceMap_GCC_GCCCPUSSAHB,
  .pSource   = &ClockSource_XO,
  .nFlags    = CLOCK_FLAG_DOMAIN_WAIT_FOR_ROOT_OFF,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(GCC_QUPV3_WRAP2_S1_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 16,
    .nDFSStates = 8,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCQUPV3WRAP2S2 =
{
  .aBSP      = ClockDomainBSP_GCC_GCCQUPV3WRAP0S0,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_GCC_GCCQUPV3WRAP0S0),
  .pRail     = &ClockRail_VDDCX,
  .aFMax     = ClockDomainFMax_GCC_GCCQUPV3WRAP0S0,
  .aSourceMap = ClockSourceMap_GCC_GCCCPUSSAHB,
  .pSource   = &ClockSource_XO,
  .nFlags    = CLOCK_FLAG_DOMAIN_WAIT_FOR_ROOT_OFF,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(GCC_QUPV3_WRAP2_S2_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 16,
    .nDFSStates = 8,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCQUPV3WRAP2S3 =
{
  .aBSP      = ClockDomainBSP_GCC_GCCQUPV3WRAP0S0,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_GCC_GCCQUPV3WRAP0S0),
  .pRail     = &ClockRail_VDDCX,
  .aFMax     = ClockDomainFMax_GCC_GCCQUPV3WRAP0S0,
  .aSourceMap = ClockSourceMap_GCC_GCCCPUSSAHB,
  .pSource   = &ClockSource_XO,
  .nFlags    = CLOCK_FLAG_DOMAIN_WAIT_FOR_ROOT_OFF,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(GCC_QUPV3_WRAP2_S3_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 16,
    .nDFSStates = 8,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCQUPV3WRAP2S4 =
{
  .aBSP      = ClockDomainBSP_GCC_GCCQUPV3WRAP0S0,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_GCC_GCCQUPV3WRAP0S0),
  .pRail     = &ClockRail_VDDCX,
  .aFMax     = ClockDomainFMax_GCC_GCCQUPV3WRAP0S0,
  .aSourceMap = ClockSourceMap_GCC_GCCCPUSSAHB,
  .pSource   = &ClockSource_XO,
  .nFlags    = CLOCK_FLAG_DOMAIN_WAIT_FOR_ROOT_OFF,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(GCC_QUPV3_WRAP2_S4_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 16,
    .nDFSStates = 8,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCQUPV3WRAP2S5 =
{
  .aBSP      = ClockDomainBSP_GCC_GCCQUPV3WRAP0S0,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_GCC_GCCQUPV3WRAP0S0),
  .pRail     = &ClockRail_VDDCX,
  .aFMax     = ClockDomainFMax_GCC_GCCQUPV3WRAP0S0,
  .aSourceMap = ClockSourceMap_GCC_GCCCPUSSAHB,
  .pSource   = &ClockSource_XO,
  .nFlags    = CLOCK_FLAG_DOMAIN_WAIT_FOR_ROOT_OFF,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(GCC_QUPV3_WRAP2_S5_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 16,
    .nDFSStates = 8,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCSDCC2APPS =
{
  .aBSP      = ClockDomainBSP_GCC_GCCSDCC2APPS,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_GCC_GCCSDCC2APPS),
  .pRail     = &ClockRail_VDDCX,
  .aFMax     = ClockDomainFMax_GCC_GCCSDCC2APPS,
  .aSourceMap = ClockSourceMap_GCC_GCCSDCC2APPS,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(GCC_SDCC2_APPS_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 8,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCSDCC4APPS =
{
  .aBSP      = ClockDomainBSP_GCC_GCCSDCC4APPS,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_GCC_GCCSDCC4APPS),
  .pRail     = &ClockRail_VDDCX,
  .aFMax     = ClockDomainFMax_GCC_GCCSDCC4APPS,
  .aSourceMap = ClockSourceMap_GCC_GCCSDCC4APPS,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(GCC_SDCC4_APPS_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 8,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCSLEEP =
{
  .aBSP      = NULL,
  .nBSPLen   = 0,
  .pRail     = &ClockRail_VDDCX,
  .pSource   = NULL,
  .nFlags    = CLOCK_FLAG_READ_ONLY,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(GCC_SLEEP_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCSYSNOC =
{
  .aBSP      = NULL,
  .nBSPLen   = 0,
  .pRail     = &ClockRail_VDDCX,
  .pSource   = &ClockSource_XO,
  .nFlags    = CLOCK_FLAG_READ_ONLY,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(GCC_SYS_NOC_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCSYSNOCSFAXI =
{
  .aBSP      = NULL,
  .nBSPLen   = 0,
  .pRail     = &ClockRail_VDDCX,
  .pSource   = &ClockSource_XO,
  .nFlags    = CLOCK_FLAG_READ_ONLY,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(GCC_SYS_NOC_SF_AXI_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCTSIFREF =
{
  .aBSP      = ClockDomainBSP_GCC_GCCTSIFREF,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_GCC_GCCTSIFREF),
  .pRail     = &ClockRail_VDDCX,
  .aFMax     = ClockDomainFMax_GCC_GCCTSIFREF,
  .aSourceMap = ClockSourceMap_GCC_GCCTSIFREF,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(GCC_TSIF_REF_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 8,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCUFSCARD2AXI =
{
  .aBSP      = ClockDomainBSP_GCC_GCCUFSCARD2AXI,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_GCC_GCCUFSCARD2AXI),
  .pRail     = &ClockRail_VDDCX,
  .aFMax     = ClockDomainFMax_GCC_GCCUFSCARD2AXI,
  .aSourceMap = ClockSourceMap_GCC_GCCCPUSSAHB,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(GCC_UFS_CARD_2_AXI_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 8,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCUFSCARD2ICECORE =
{
  .aBSP      = ClockDomainBSP_GCC_GCCUFSCARD2AXI,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_GCC_GCCUFSCARD2AXI),
  .pRail     = &ClockRail_VDDCX,
  .aFMax     = ClockDomainFMax_GCC_GCCUFSCARD2AXI,
  .aSourceMap = ClockSourceMap_GCC_GCCCPUSSAHB,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(GCC_UFS_CARD_2_ICE_CORE_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCUFSCARD2PHYAUX =
{
  .aBSP      = ClockDomainBSP_GCC_GCCUFSCARD2PHYAUX,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_GCC_GCCUFSCARD2PHYAUX),
  .pRail     = &ClockRail_VDDCX,
  .aFMax     = ClockDomainFMax_GCC_GCCUFSCARD2PHYAUX,
  .aSourceMap = ClockSourceMap_GCC_GCCUFSCARD2PHYAUX,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(GCC_UFS_CARD_2_PHY_AUX_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCUFSCARD2RXSYMBOL0 =
{
  .aBSP      = NULL,
  .nBSPLen   = 0,
  .pRail     = &ClockRail_VDDCX,
  .aFMax     = ClockDomainFMax_GCC_GCCUFSCARD2RXSYMBOL0,
  .aSourceMap = ClockSourceMap_GCC_GCCUFSCARD2RXSYMBOL0,
  .pSource   = NULL,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(GCC_UFS_CARD_2_RX_SYMBOL_0_MUXR),
    .pmControl  = &HAL_clk_GenericDomainMuxControl,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCUFSCARD2RXSYMBOL1 =
{
  .aBSP      = NULL,
  .nBSPLen   = 0,
  .pRail     = &ClockRail_VDDCX,
  .aFMax     = ClockDomainFMax_GCC_GCCUFSCARD2RXSYMBOL0,
  .aSourceMap = ClockSourceMap_GCC_GCCUFSCARD2RXSYMBOL1,
  .pSource   = NULL,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(GCC_UFS_CARD_2_RX_SYMBOL_1_MUXR),
    .pmControl  = &HAL_clk_GenericDomainMuxControl,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCUFSCARD2TXSYMBOL0 =
{
  .aBSP      = NULL,
  .nBSPLen   = 0,
  .pRail     = &ClockRail_VDDCX,
  .aFMax     = ClockDomainFMax_GCC_GCCUFSCARD2RXSYMBOL0,
  .aSourceMap = ClockSourceMap_GCC_GCCUFSCARD2TXSYMBOL0,
  .pSource   = NULL,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(GCC_UFS_CARD_2_TX_SYMBOL_0_MUXR),
    .pmControl  = &HAL_clk_GenericDomainMuxControl,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCUFSCARD2UNIPROCORE =
{
  .aBSP      = ClockDomainBSP_GCC_GCCUFSCARD2AXI,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_GCC_GCCUFSCARD2AXI),
  .pRail     = &ClockRail_VDDCX,
  .aFMax     = ClockDomainFMax_GCC_GCCUFSCARD2AXI,
  .aSourceMap = ClockSourceMap_GCC_GCCCPUSSAHB,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(GCC_UFS_CARD_2_UNIPRO_CORE_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCUFSCARDAXI =
{
  .aBSP      = ClockDomainBSP_GCC_GCCUFSCARDAXI,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_GCC_GCCUFSCARDAXI),
  .pRail     = &ClockRail_VDDCX,
  .aFMax     = ClockDomainFMax_GCC_GCCUFSCARDAXI,
  .aSourceMap = ClockSourceMap_GCC_GCCCPUSSAHB,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(GCC_UFS_CARD_AXI_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 8,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCUFSCARDICECORE =
{
  .aBSP      = ClockDomainBSP_GCC_GCCUFSCARDICECORE,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_GCC_GCCUFSCARDICECORE),
  .pRail     = &ClockRail_VDDCX,
  .aFMax     = ClockDomainFMax_GCC_GCCUFSCARDICECORE,
  .aSourceMap = ClockSourceMap_GCC_GCCCPUSSAHB,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(GCC_UFS_CARD_ICE_CORE_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCUFSCARDPHYAUX =
{
  .aBSP      = ClockDomainBSP_GCC_GCCUFSCARD2PHYAUX,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_GCC_GCCUFSCARD2PHYAUX),
  .pRail     = &ClockRail_VDDCX,
  .aFMax     = ClockDomainFMax_GCC_GCCUFSCARDPHYAUX,
  .aSourceMap = ClockSourceMap_GCC_GCCUFSCARD2PHYAUX,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(GCC_UFS_CARD_PHY_AUX_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCUFSCARDRXSYMBOL0 =
{
  .aBSP      = NULL,
  .nBSPLen   = 0,
  .pRail     = &ClockRail_VDDCX,
  .aFMax     = ClockDomainFMax_GCC_GCCUFSCARDRXSYMBOL0,
  .aSourceMap = ClockSourceMap_GCC_GCCUFSCARDRXSYMBOL0,
  .pSource   = NULL,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(GCC_UFS_CARD_RX_SYMBOL_0_MUXR),
    .pmControl  = &HAL_clk_GenericDomainMuxControl,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCUFSCARDRXSYMBOL1 =
{
  .aBSP      = NULL,
  .nBSPLen   = 0,
  .pRail     = &ClockRail_VDDCX,
  .aFMax     = ClockDomainFMax_GCC_GCCUFSCARDRXSYMBOL0,
  .aSourceMap = ClockSourceMap_GCC_GCCUFSCARDRXSYMBOL1,
  .pSource   = NULL,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(GCC_UFS_CARD_RX_SYMBOL_1_MUXR),
    .pmControl  = &HAL_clk_GenericDomainMuxControl,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCUFSCARDTXSYMBOL0 =
{
  .aBSP      = NULL,
  .nBSPLen   = 0,
  .pRail     = &ClockRail_VDDCX,
  .aFMax     = ClockDomainFMax_GCC_GCCUFSCARDRXSYMBOL0,
  .aSourceMap = ClockSourceMap_GCC_GCCUFSCARDTXSYMBOL0,
  .pSource   = NULL,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(GCC_UFS_CARD_TX_SYMBOL_0_MUXR),
    .pmControl  = &HAL_clk_GenericDomainMuxControl,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCUFSCARDUNIPROCORE =
{
  .aBSP      = ClockDomainBSP_GCC_GCCUFSCARDUNIPROCORE,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_GCC_GCCUFSCARDUNIPROCORE),
  .pRail     = &ClockRail_VDDCX,
  .aFMax     = ClockDomainFMax_GCC_GCCUFSCARDUNIPROCORE,
  .aSourceMap = ClockSourceMap_GCC_GCCCPUSSAHB,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(GCC_UFS_CARD_UNIPRO_CORE_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCUFSPHYAXI =
{
  .aBSP      = ClockDomainBSP_GCC_GCCUFSPHYAXI,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_GCC_GCCUFSPHYAXI),
  .pRail     = &ClockRail_VDDCX,
  .aFMax     = ClockDomainFMax_GCC_GCCUFSCARD2AXI,
  .aSourceMap = ClockSourceMap_GCC_GCCCPUSSAHB,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(GCC_UFS_PHY_AXI_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 8,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCUFSPHYICECORE =
{
  .aBSP      = ClockDomainBSP_GCC_GCCUFSCARD2AXI,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_GCC_GCCUFSCARD2AXI),
  .pRail     = &ClockRail_VDDCX,
  .aFMax     = ClockDomainFMax_GCC_GCCUFSCARD2AXI,
  .aSourceMap = ClockSourceMap_GCC_GCCCPUSSAHB,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(GCC_UFS_PHY_ICE_CORE_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCUFSPHYPHYAUX =
{
  .aBSP      = ClockDomainBSP_GCC_GCCUFSPHYPHYAUX,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_GCC_GCCUFSPHYPHYAUX),
  .pRail     = &ClockRail_VDDCX,
  .aFMax     = ClockDomainFMax_GCC_GCCUFSCARD2PHYAUX,
  .aSourceMap = ClockSourceMap_GCC_GCCUFSCARD2PHYAUX,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(GCC_UFS_PHY_PHY_AUX_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCUFSPHYRXSYMBOL0 =
{
  .aBSP      = NULL,
  .nBSPLen   = 0,
  .pRail     = &ClockRail_VDDCX,
  .aFMax     = ClockDomainFMax_GCC_GCCUFSCARD2RXSYMBOL0,
  .aSourceMap = ClockSourceMap_GCC_GCCUFSPHYRXSYMBOL0,
  .pSource   = NULL,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(GCC_UFS_PHY_RX_SYMBOL_0_MUXR),
    .pmControl  = &HAL_clk_GenericDomainMuxControl,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCUFSPHYRXSYMBOL1 =
{
  .aBSP      = NULL,
  .nBSPLen   = 0,
  .pRail     = &ClockRail_VDDCX,
  .aFMax     = ClockDomainFMax_GCC_GCCUFSCARD2RXSYMBOL0,
  .aSourceMap = ClockSourceMap_GCC_GCCUFSPHYRXSYMBOL1,
  .pSource   = NULL,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(GCC_UFS_PHY_RX_SYMBOL_1_MUXR),
    .pmControl  = &HAL_clk_GenericDomainMuxControl,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCUFSPHYTXSYMBOL0 =
{
  .aBSP      = NULL,
  .nBSPLen   = 0,
  .pRail     = &ClockRail_VDDCX,
  .aFMax     = ClockDomainFMax_GCC_GCCUFSCARD2RXSYMBOL0,
  .aSourceMap = ClockSourceMap_GCC_GCCUFSPHYTXSYMBOL0,
  .pSource   = NULL,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(GCC_UFS_PHY_TX_SYMBOL_0_MUXR),
    .pmControl  = &HAL_clk_GenericDomainMuxControl,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCUFSPHYUNIPROCORE =
{
  .aBSP      = ClockDomainBSP_GCC_GCCUFSCARD2AXI,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_GCC_GCCUFSCARD2AXI),
  .pRail     = &ClockRail_VDDCX,
  .aFMax     = ClockDomainFMax_GCC_GCCUFSCARD2AXI,
  .aSourceMap = ClockSourceMap_GCC_GCCCPUSSAHB,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(GCC_UFS_PHY_UNIPRO_CORE_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCUSB30MPMASTER =
{
  .aBSP      = ClockDomainBSP_GCC_GCCUSB30MPMASTER,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_GCC_GCCUSB30MPMASTER),
  .pRail     = &ClockRail_VDDCX,
  .aFMax     = ClockDomainFMax_GCC_GCCUSB30MPMASTER,
  .aSourceMap = ClockSourceMap_GCC_GCCCPUSSAHB,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(GCC_USB30_MP_MASTER_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 8,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCUSB30MPMOCKUTMI =
{
  .aBSP      = ClockDomainBSP_GCC_GCCUSB30MPMOCKUTMI,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_GCC_GCCUSB30MPMOCKUTMI),
  .pRail     = &ClockRail_VDDCX,
  .aFMax     = ClockDomainFMax_GCC_GCCUSB30MPMOCKUTMI,
  .aSourceMap = ClockSourceMap_GCC_GCCCPUSSAHB,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(GCC_USB30_MP_MOCK_UTMI_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCUSB30PRIMMASTER =
{
  .aBSP      = ClockDomainBSP_GCC_GCCUSB30MPMASTER,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_GCC_GCCUSB30MPMASTER),
  .pRail     = &ClockRail_VDDCX,
  .aFMax     = ClockDomainFMax_GCC_GCCUSB30MPMASTER,
  .aSourceMap = ClockSourceMap_GCC_GCCCPUSSAHB,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(GCC_USB30_PRIM_MASTER_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 8,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCUSB30PRIMMOCKUTMI =
{
  .aBSP      = ClockDomainBSP_GCC_GCCUSB30MPMOCKUTMI,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_GCC_GCCUSB30MPMOCKUTMI),
  .pRail     = &ClockRail_VDDCX,
  .aFMax     = ClockDomainFMax_GCC_GCCUSB30MPMOCKUTMI,
  .aSourceMap = ClockSourceMap_GCC_GCCCPUSSAHB,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(GCC_USB30_PRIM_MOCK_UTMI_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCUSB30SECMASTER =
{
  .aBSP      = ClockDomainBSP_GCC_GCCUSB30MPMASTER,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_GCC_GCCUSB30MPMASTER),
  .pRail     = &ClockRail_VDDCX,
  .aFMax     = ClockDomainFMax_GCC_GCCUSB30MPMASTER,
  .aSourceMap = ClockSourceMap_GCC_GCCCPUSSAHB,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(GCC_USB30_SEC_MASTER_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 8,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCUSB30SECMOCKUTMI =
{
  .aBSP      = ClockDomainBSP_GCC_GCCUSB30MPMOCKUTMI,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_GCC_GCCUSB30MPMOCKUTMI),
  .pRail     = &ClockRail_VDDCX,
  .aFMax     = ClockDomainFMax_GCC_GCCUSB30MPMOCKUTMI,
  .aSourceMap = ClockSourceMap_GCC_GCCCPUSSAHB,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(GCC_USB30_SEC_MOCK_UTMI_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCUSB3MPPHYAUX =
{
  .aBSP      = ClockDomainBSP_GCC_GCCUFSCARD2PHYAUX,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_GCC_GCCUFSCARD2PHYAUX),
  .pRail     = &ClockRail_VDDCX,
  .aFMax     = ClockDomainFMax_GCC_GCCUFSCARD2PHYAUX,
  .aSourceMap = ClockSourceMap_GCC_GCCPCIE0AUX,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(GCC_USB3_MP_PHY_AUX_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCUSB3MPPHYPIPE0 =
{
  .aBSP      = NULL,
  .nBSPLen   = 0,
  .pRail     = &ClockRail_VDDCX,
  .aFMax     = ClockDomainFMax_GCC_GCCUSB3MPPHYPIPE0,
  .aSourceMap = ClockSourceMap_GCC_GCCUSB3MPPHYPIPE0,
  .pSource   = NULL,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(GCC_USB3_MP_PHY_PIPE_0_MUXR),
    .pmControl  = &HAL_clk_GenericDomainMuxControl,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCUSB3MPPHYPIPE1 =
{
  .aBSP      = NULL,
  .nBSPLen   = 0,
  .pRail     = &ClockRail_VDDCX,
  .aFMax     = ClockDomainFMax_GCC_GCCUSB3MPPHYPIPE0,
  .aSourceMap = ClockSourceMap_GCC_GCCUSB3MPPHYPIPE1,
  .pSource   = NULL,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(GCC_USB3_MP_PHY_PIPE_1_MUXR),
    .pmControl  = &HAL_clk_GenericDomainMuxControl,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCUSB3PRIMPHYAUX =
{
  .aBSP      = ClockDomainBSP_GCC_GCCUFSCARD2PHYAUX,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_GCC_GCCUFSCARD2PHYAUX),
  .pRail     = &ClockRail_VDDCX,
  .aFMax     = ClockDomainFMax_GCC_GCCUFSCARD2PHYAUX,
  .aSourceMap = ClockSourceMap_GCC_GCCPCIE0AUX,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(GCC_USB3_PRIM_PHY_AUX_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCUSB3PRIMPHYPIPE =
{
  .aBSP      = NULL,
  .nBSPLen   = 0,
  .pRail     = &ClockRail_VDDCX,
  .aFMax     = ClockDomainFMax_GCC_GCCUSB3MPPHYPIPE0,
  .aSourceMap = ClockSourceMap_GCC_GCCUSB3PRIMPHYPIPE,
  .pSource   = NULL,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(GCC_USB3_PRIM_PHY_PIPE_MUXR),
    .pmControl  = &HAL_clk_GenericDomainMuxControl,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCUSB3SECPHYAUX =
{
  .aBSP      = ClockDomainBSP_GCC_GCCUFSCARD2PHYAUX,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_GCC_GCCUFSCARD2PHYAUX),
  .pRail     = &ClockRail_VDDCX,
  .aFMax     = ClockDomainFMax_GCC_GCCUFSCARD2PHYAUX,
  .aSourceMap = ClockSourceMap_GCC_GCCPCIE0AUX,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(GCC_USB3_SEC_PHY_AUX_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCUSB3SECPHYPIPE =
{
  .aBSP      = NULL,
  .nBSPLen   = 0,
  .pRail     = &ClockRail_VDDCX,
  .aFMax     = ClockDomainFMax_GCC_GCCUSB3MPPHYPIPE0,
  .aSourceMap = ClockSourceMap_GCC_GCCUSB3SECPHYPIPE,
  .pSource   = NULL,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(GCC_USB3_SEC_PHY_PIPE_MUXR),
    .pmControl  = &HAL_clk_GenericDomainMuxControl,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCVSENSOR =
{
  .aBSP      = NULL,
  .nBSPLen   = 0,
  .pRail     = &ClockRail_VDDCX,
  .pSource   = &ClockSource_XO,
  .nFlags    = CLOCK_FLAG_READ_ONLY,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(GCC_VSENSOR_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCXO =
{
  .aBSP      = NULL,
  .nBSPLen   = 0,
  .pRail     = &ClockRail_VDDCX,
  .aFMax     = ClockDomainFMax_GCC_GCCUFSCARD2PHYAUX,
  .aSourceMap = ClockSourceMap_GCC_GCCUFSCARD2PHYAUX,
  .pSource   = &ClockSource_XO,
  .nFlags    = CLOCK_FLAG_READ_ONLY,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(GCC_XO_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_GPUCC_GPUCCDEBUGMUX =
{
  .aBSP      = NULL,
  .nBSPLen   = 0,
  .pRail     = &ClockRail_VDDCX,
  .aFMax     = ClockDomainFMax_GPUCC_GPUCCDEBUGMUX,
  .aSourceMap = ClockSourceMap_GPUCC_GPUCCDEBUGMUX,
  .pSource   = NULL,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(GPUCC_GPU_CC_DEBUG_MUX_MUXR),
    .pmControl  = &HAL_clk_GenericDomainMuxControl,
  },
};

static ClockDomainNodeType ClockDomain_GPUCC_GPUCCGMU =
{
  .aBSP      = ClockDomainBSP_GPUCC_GPUCCGMU,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_GPUCC_GPUCCGMU),
  .pRail     = &ClockRail_VDDCX,
  .aFMax     = ClockDomainFMax_GPUCC_GPUCCGMU,
  .aSourceMap = ClockSourceMap_GPUCC_GPUCCGMU,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(GPUCC_GPU_CC_GMU_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_GPUCC_GPUCCGXGFX3D =
{
  .aBSP      = ClockDomainBSP_GPUCC_GPUCCGXGFX3D,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_GPUCC_GPUCCGXGFX3D),
  .pRail     = &ClockRail_VDDGX,
  .aFMax     = ClockDomainFMax_GPUCC_GPUCCGXGFX3D,
  .aSourceMap = ClockSourceMap_GPUCC_GPUCCGXGFX3D,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(GPUCC_GPU_CC_GX_GFX3D_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_GPUCC_GPUCCPLLTESTMUX =
{
  .aBSP      = NULL,
  .nBSPLen   = 0,
  .pRail     = &ClockRail_VDDCX,
  .aFMax     = ClockDomainFMax_GPUCC_GPUCCPLLTESTMUX,
  .aSourceMap = ClockSourceMap_GPUCC_GPUCCPLLTESTMUX,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(GPUCC_GPU_CC_PLL_TEST_MUX_MUXR),
    .pmControl  = &HAL_clk_GenericDomainMuxControl,
  },
};

static ClockDomainNodeType ClockDomain_NPUCC_NPUCCCALDP =
{
  .aBSP      = ClockDomainBSP_NPUCC_NPUCCCALDP,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_NPUCC_NPUCCCALDP),
  .pRail     = &ClockRail_VDDCX,
  .aFMax     = ClockDomainFMax_NPUCC_NPUCCCALDP,
  .aSourceMap = ClockSourceMap_NPUCC_NPUCCCALDP,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(NPU_CC_CAL_DP_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_NPUCC_NPUCCDEBUGMUX =
{
  .aBSP      = NULL,
  .nBSPLen   = 0,
  .pRail     = &ClockRail_VDDCX,
  .aFMax     = ClockDomainFMax_NPUCC_NPUCCDEBUGMUX,
  .aSourceMap = ClockSourceMap_NPUCC_NPUCCDEBUGMUX,
  .pSource   = NULL,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(NPU_CC_DEBUG_MUX_MUXR),
    .pmControl  = &HAL_clk_GenericDomainMuxControl,
  },
};

static ClockDomainNodeType ClockDomain_NPUCC_NPUCCNPUCORE =
{
  .aBSP      = ClockDomainBSP_NPUCC_NPUCCNPUCORE,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_NPUCC_NPUCCNPUCORE),
  .pRail     = &ClockRail_VDDCX,
  .aFMax     = ClockDomainFMax_NPUCC_NPUCCNPUCORE,
  .aSourceMap = ClockSourceMap_NPUCC_NPUCCCALDP,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(NPU_CC_NPU_CORE_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_NPUCC_NPUCCPLLTESTMUX =
{
  .aBSP      = NULL,
  .nBSPLen   = 0,
  .pRail     = &ClockRail_VDDCX,
  .aFMax     = ClockDomainFMax_NPUCC_NPUCCPLLTESTMUX,
  .aSourceMap = ClockSourceMap_NPUCC_NPUCCPLLTESTMUX,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(NPU_CC_PLL_TEST_MUX_MUXR),
    .pmControl  = &HAL_clk_GenericDomainMuxControl,
  },
};
#ifdef ENABLE_AUTO_PLAT
static ClockDomainNodeType ClockDomain_SCC_SCCMAINRCG =
{
  .aBSP      = ClockDomainBSP_SCC_SCCMAINRCG,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_SCC_SCCMAINRCG),
  .pRail     = &ClockRail_VDDSSCINT,
  .aFMax     = ClockDomainFMax_SCC_SCCMAINRCG,
  .aSourceMap = ClockSourceMap_SCC_SCCMAINRCG,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(SSC_SCC_MAIN_RCG_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_SCC_SCCQUPV3SE0 =
{
  .aBSP      = ClockDomainBSP_SCC_SCCQUPV3SE0,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_SCC_SCCQUPV3SE0),
  .pRail     = &ClockRail_VDDSSCINT,
  .aFMax     = ClockDomainFMax_SCC_SCCQUPV3SE0,
  .aSourceMap = ClockSourceMap_SCC_SCCMAINRCG,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(SSC_SCC_QUPV3_SE0_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 16,
    .nDFSStates = 8,
  },
};

static ClockDomainNodeType ClockDomain_SCC_SCCQUPV3SE1 =
{
  .aBSP      = ClockDomainBSP_SCC_SCCQUPV3SE0,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_SCC_SCCQUPV3SE0),
  .pRail     = &ClockRail_VDDSSCINT,
  .aFMax     = ClockDomainFMax_SCC_SCCQUPV3SE0,
  .aSourceMap = ClockSourceMap_SCC_SCCMAINRCG,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(SSC_SCC_QUPV3_SE1_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 16,
    .nDFSStates = 8,
  },
};

static ClockDomainNodeType ClockDomain_SCC_SCCQUPV3SE2 =
{
  .aBSP      = ClockDomainBSP_SCC_SCCQUPV3SE0,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_SCC_SCCQUPV3SE0),
  .pRail     = &ClockRail_VDDSSCINT,
  .aFMax     = ClockDomainFMax_SCC_SCCQUPV3SE0,
  .aSourceMap = ClockSourceMap_SCC_SCCMAINRCG,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(SSC_SCC_QUPV3_SE2_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 16,
    .nDFSStates = 8,
  },
};

static ClockDomainNodeType ClockDomain_SCC_SCCQUPV3SE3 =
{
  .aBSP      = ClockDomainBSP_SCC_SCCQUPV3SE0,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_SCC_SCCQUPV3SE0),
  .pRail     = &ClockRail_VDDSSCINT,
  .aFMax     = ClockDomainFMax_SCC_SCCQUPV3SE0,
  .aSourceMap = ClockSourceMap_SCC_SCCMAINRCG,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(SSC_SCC_QUPV3_SE3_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 16,
    .nDFSStates = 8,
  },
};

static ClockDomainNodeType ClockDomain_SCC_SCCQUPV3SE4 =
{
  .aBSP      = ClockDomainBSP_SCC_SCCQUPV3SE0,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_SCC_SCCQUPV3SE0),
  .pRail     = &ClockRail_VDDSSCINT,
  .aFMax     = ClockDomainFMax_SCC_SCCQUPV3SE0,
  .aSourceMap = ClockSourceMap_SCC_SCCMAINRCG,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(SSC_SCC_QUPV3_SE4_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 16,
    .nDFSStates = 8,
  },
};

static ClockDomainNodeType ClockDomain_SCC_SCCQUPV3SE5 =
{
  .aBSP      = ClockDomainBSP_SCC_SCCQUPV3SE0,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_SCC_SCCQUPV3SE0),
  .pRail     = &ClockRail_VDDSSCINT,
  .aFMax     = ClockDomainFMax_SCC_SCCQUPV3SE0,
  .aSourceMap = ClockSourceMap_SCC_SCCMAINRCG,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(SSC_SCC_QUPV3_SE5_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 16,
    .nDFSStates = 8,
  },
};
#endif
static ClockDomainNodeType ClockDomain_VIDEOCC_VIDEOCCIRIS =
{
  .aBSP      = ClockDomainBSP_VIDEOCC_VIDEOCCIRIS,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_VIDEOCC_VIDEOCCIRIS),
  .pRail     = &ClockRail_VDDMM,
  .aFMax     = ClockDomainFMax_VIDEOCC_VIDEOCCIRIS,
  .aSourceMap = ClockSourceMap_VIDEOCC_VIDEOCCIRIS,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(VIDEO_CC_IRIS_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_VIDEOCC_VIDEOCCSLEEP =
{
  .aBSP      = ClockDomainBSP_VIDEOCC_VIDEOCCSLEEP,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_VIDEOCC_VIDEOCCSLEEP),
  .pRail     = &ClockRail_VDDMM,
  .aFMax     = ClockDomainFMax_VIDEOCC_VIDEOCCSLEEP,
  .aSourceMap = ClockSourceMap_VIDEOCC_VIDEOCCSLEEP,
  .pSource   = NULL,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(VIDEO_CC_SLEEP_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_VIDEOCC_VIDEOCCXO =
{
  .aBSP      = ClockDomainBSP_VIDEOCC_VIDEOCCXO,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_VIDEOCC_VIDEOCCXO),
  .pRail     = &ClockRail_VDDMM,
  .aFMax     = ClockDomainFMax_VIDEOCC_VIDEOCCXO,
  .aSourceMap = ClockSourceMap_VIDEOCC_VIDEOCCXO,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_PHYS(VIDEO_CC_XO_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};


static ClockDomainNodeType *aDomains[] =
{
  &ClockDomain_CAMCC_CAMCCBPS,
  &ClockDomain_CAMCC_CAMCCCAMNOCAXI,
  &ClockDomain_CAMCC_CAMCCCCI0,
  &ClockDomain_CAMCC_CAMCCCCI1,
  &ClockDomain_CAMCC_CAMCCCCI2,
  &ClockDomain_CAMCC_CAMCCCCI3,
  &ClockDomain_CAMCC_CAMCCCPHYRX,
  &ClockDomain_CAMCC_CAMCCCSI0PHYTIMER,
  &ClockDomain_CAMCC_CAMCCCSI1PHYTIMER,
  &ClockDomain_CAMCC_CAMCCCSI2PHYTIMER,
  &ClockDomain_CAMCC_CAMCCCSI3PHYTIMER,
  &ClockDomain_CAMCC_CAMCCDEBUGMUX,
  &ClockDomain_CAMCC_CAMCCFASTAHB,
  &ClockDomain_CAMCC_CAMCCFDCORE,
  &ClockDomain_CAMCC_CAMCCICP,
  &ClockDomain_CAMCC_CAMCCIFE0,
  &ClockDomain_CAMCC_CAMCCIFE0CSID,
  &ClockDomain_CAMCC_CAMCCIFE1,
  &ClockDomain_CAMCC_CAMCCIFE1CSID,
  &ClockDomain_CAMCC_CAMCCIFE2,
  &ClockDomain_CAMCC_CAMCCIFE2CSID,
  &ClockDomain_CAMCC_CAMCCIFE3,
  &ClockDomain_CAMCC_CAMCCIFE3CSID,
  &ClockDomain_CAMCC_CAMCCIFELITE0,
  &ClockDomain_CAMCC_CAMCCIFELITE0CSID,
  &ClockDomain_CAMCC_CAMCCIFELITE1,
  &ClockDomain_CAMCC_CAMCCIFELITE1CSID,
  &ClockDomain_CAMCC_CAMCCIFELITE2,
  &ClockDomain_CAMCC_CAMCCIFELITE2CSID,
  &ClockDomain_CAMCC_CAMCCIFELITE3,
  &ClockDomain_CAMCC_CAMCCIFELITE3CSID,
  &ClockDomain_CAMCC_CAMCCIPE0,
  &ClockDomain_CAMCC_CAMCCJPEG,
  &ClockDomain_CAMCC_CAMCCLRME,
  &ClockDomain_CAMCC_CAMCCMCLK0,
  &ClockDomain_CAMCC_CAMCCMCLK1,
  &ClockDomain_CAMCC_CAMCCMCLK2,
  &ClockDomain_CAMCC_CAMCCMCLK3,
  &ClockDomain_CAMCC_CAMCCMCLK4,
  &ClockDomain_CAMCC_CAMCCMCLK5,
  &ClockDomain_CAMCC_CAMCCMCLK6,
  &ClockDomain_CAMCC_CAMCCMCLK7,
  &ClockDomain_CAMCC_CAMCCPLLTESTMUX,
  &ClockDomain_CAMCC_CAMCCQDSSDEBUG,
  &ClockDomain_CAMCC_CAMCCSLEEP,
  &ClockDomain_CAMCC_CAMCCSLOWAHB,
  &ClockDomain_CAMCC_CAMCCXO,
  &ClockDomain_DISPCC_DISPCCMDSSAHB,
  &ClockDomain_DISPCC_DISPCCMDSSBYTE0,
  &ClockDomain_DISPCC_DISPCCMDSSBYTE1,
  &ClockDomain_DISPCC_DISPCCMDSSDPAUX,
  &ClockDomain_DISPCC_DISPCCMDSSDPAUX1,
  &ClockDomain_DISPCC_DISPCCMDSSDPCRYPTO,
  &ClockDomain_DISPCC_DISPCCMDSSDPCRYPTO1,
  &ClockDomain_DISPCC_DISPCCMDSSDPLINK,
  &ClockDomain_DISPCC_DISPCCMDSSDPLINK1,
  &ClockDomain_DISPCC_DISPCCMDSSDPPIXEL,
  &ClockDomain_DISPCC_DISPCCMDSSDPPIXEL1,
  &ClockDomain_DISPCC_DISPCCMDSSDPPIXEL2,
  &ClockDomain_DISPCC_DISPCCMDSSEDPAUX,
  &ClockDomain_DISPCC_DISPCCMDSSEDPGTC,
  &ClockDomain_DISPCC_DISPCCMDSSEDPLINK,
  &ClockDomain_DISPCC_DISPCCMDSSEDPPIXEL,
  &ClockDomain_DISPCC_DISPCCMDSSESC0,
  &ClockDomain_DISPCC_DISPCCMDSSESC1,
  &ClockDomain_DISPCC_DISPCCMDSSMDP,
  &ClockDomain_DISPCC_DISPCCMDSSPCLK0,
  &ClockDomain_DISPCC_DISPCCMDSSPCLK1,
  &ClockDomain_DISPCC_DISPCCMDSSROT,
  &ClockDomain_DISPCC_DISPCCMDSSVSYNC,
  &ClockDomain_DISPCC_DISPCCPLLTESTMUX,
  &ClockDomain_DISPCC_DISPCCSLEEP,
  &ClockDomain_DISPCC_DISPCCXO,
  &ClockDomain_GCC_CLOCKDRIVERGENNOSOURCE,
  &ClockDomain_GCC_CLOCKDRIVERGENXO,
  &ClockDomain_GCC_GCCAGGRENOC,
  &ClockDomain_GCC_GCCAGGRENOCSOUTHSF,
  &ClockDomain_GCC_GCCCNOCPERIPH,
  &ClockDomain_GCC_GCCCONFIGNOC,
  &ClockDomain_GCC_GCCCPUSSAHB,
  &ClockDomain_GCC_GCCCPUSSRBCPR,
  &ClockDomain_GCC_GCCEMACPTP,
  &ClockDomain_GCC_GCCEMACRGMII,
  &ClockDomain_GCC_GCCGP1,
  &ClockDomain_GCC_GCCGP2,
  &ClockDomain_GCC_GCCGP3,
  &ClockDomain_GCC_GCCGP4,
  &ClockDomain_GCC_GCCGP5,
  &ClockDomain_GCC_GCCGPUMEMNOCGFX,
  &ClockDomain_GCC_GCCMEMNOC,
  &ClockDomain_GCC_GCCMMNOCHFQX,
  &ClockDomain_GCC_GCCMMNOCSFQX,
  &ClockDomain_GCC_GCCNPUAXI,
  &ClockDomain_GCC_GCCPCIE0AUX,
  &ClockDomain_GCC_GCCPCIE0PIPE,
  &ClockDomain_GCC_GCCPCIE1AUX,
  &ClockDomain_GCC_GCCPCIE1PIPE,
  &ClockDomain_GCC_GCCPCIE2AUX,
  &ClockDomain_GCC_GCCPCIE2PIPE,
  &ClockDomain_GCC_GCCPCIE3AUX,
  &ClockDomain_GCC_GCCPCIE3PIPE,
  &ClockDomain_GCC_GCCPCIEPHYREFGEN,
  &ClockDomain_GCC_GCCPDM2,
  &ClockDomain_GCC_GCCQDSSATBA,
  &ClockDomain_GCC_GCCQDSSATBB,
  &ClockDomain_GCC_GCCQDSSTRIG,
  &ClockDomain_GCC_GCCQSPI1CORE,
  &ClockDomain_GCC_GCCQSPICORE,
  &ClockDomain_GCC_GCCQUPV3WRAP0CORE2X,
  &ClockDomain_GCC_GCCQUPV3WRAP0S0,
  &ClockDomain_GCC_GCCQUPV3WRAP0S1,
  &ClockDomain_GCC_GCCQUPV3WRAP0S2,
  &ClockDomain_GCC_GCCQUPV3WRAP0S3,
  &ClockDomain_GCC_GCCQUPV3WRAP0S4,
  &ClockDomain_GCC_GCCQUPV3WRAP0S5,
  &ClockDomain_GCC_GCCQUPV3WRAP0S6,
  &ClockDomain_GCC_GCCQUPV3WRAP0S7,
  &ClockDomain_GCC_GCCQUPV3WRAP1CORE2X,
  &ClockDomain_GCC_GCCQUPV3WRAP1S0,
  &ClockDomain_GCC_GCCQUPV3WRAP1S1,
  &ClockDomain_GCC_GCCQUPV3WRAP1S2,
  &ClockDomain_GCC_GCCQUPV3WRAP1S3,
  &ClockDomain_GCC_GCCQUPV3WRAP1S4,
  &ClockDomain_GCC_GCCQUPV3WRAP1S5,
  &ClockDomain_GCC_GCCQUPV3WRAP2CORE2X,
  &ClockDomain_GCC_GCCQUPV3WRAP2S0,
  &ClockDomain_GCC_GCCQUPV3WRAP2S1,
  &ClockDomain_GCC_GCCQUPV3WRAP2S2,
  &ClockDomain_GCC_GCCQUPV3WRAP2S3,
  &ClockDomain_GCC_GCCQUPV3WRAP2S4,
  &ClockDomain_GCC_GCCQUPV3WRAP2S5,
  &ClockDomain_GCC_GCCSDCC2APPS,
  &ClockDomain_GCC_GCCSDCC4APPS,
  &ClockDomain_GCC_GCCSLEEP,
  &ClockDomain_GCC_GCCSYSNOC,
  &ClockDomain_GCC_GCCSYSNOCSFAXI,
  &ClockDomain_GCC_GCCTSIFREF,
  &ClockDomain_GCC_GCCUFSCARD2AXI,
  &ClockDomain_GCC_GCCUFSCARD2ICECORE,
  &ClockDomain_GCC_GCCUFSCARD2PHYAUX,
  &ClockDomain_GCC_GCCUFSCARD2RXSYMBOL0,
  &ClockDomain_GCC_GCCUFSCARD2RXSYMBOL1,
  &ClockDomain_GCC_GCCUFSCARD2TXSYMBOL0,
  &ClockDomain_GCC_GCCUFSCARD2UNIPROCORE,
  &ClockDomain_GCC_GCCUFSCARDAXI,
  &ClockDomain_GCC_GCCUFSCARDICECORE,
  &ClockDomain_GCC_GCCUFSCARDPHYAUX,
  &ClockDomain_GCC_GCCUFSCARDRXSYMBOL0,
  &ClockDomain_GCC_GCCUFSCARDRXSYMBOL1,
  &ClockDomain_GCC_GCCUFSCARDTXSYMBOL0,
  &ClockDomain_GCC_GCCUFSCARDUNIPROCORE,
  &ClockDomain_GCC_GCCUFSPHYAXI,
  &ClockDomain_GCC_GCCUFSPHYICECORE,
  &ClockDomain_GCC_GCCUFSPHYPHYAUX,
  &ClockDomain_GCC_GCCUFSPHYRXSYMBOL0,
  &ClockDomain_GCC_GCCUFSPHYRXSYMBOL1,
  &ClockDomain_GCC_GCCUFSPHYTXSYMBOL0,
  &ClockDomain_GCC_GCCUFSPHYUNIPROCORE,
  &ClockDomain_GCC_GCCUSB30MPMASTER,
  &ClockDomain_GCC_GCCUSB30MPMOCKUTMI,
  &ClockDomain_GCC_GCCUSB30PRIMMASTER,
  &ClockDomain_GCC_GCCUSB30PRIMMOCKUTMI,
  &ClockDomain_GCC_GCCUSB30SECMASTER,
  &ClockDomain_GCC_GCCUSB30SECMOCKUTMI,
  &ClockDomain_GCC_GCCUSB3MPPHYAUX,
  &ClockDomain_GCC_GCCUSB3MPPHYPIPE0,
  &ClockDomain_GCC_GCCUSB3MPPHYPIPE1,
  &ClockDomain_GCC_GCCUSB3PRIMPHYAUX,
  &ClockDomain_GCC_GCCUSB3PRIMPHYPIPE,
  &ClockDomain_GCC_GCCUSB3SECPHYAUX,
  &ClockDomain_GCC_GCCUSB3SECPHYPIPE,
  &ClockDomain_GCC_GCCVSENSOR,
  &ClockDomain_GCC_GCCXO,
  &ClockDomain_GPUCC_GPUCCDEBUGMUX,
  &ClockDomain_GPUCC_GPUCCGMU,
  &ClockDomain_GPUCC_GPUCCGXGFX3D,
  &ClockDomain_GPUCC_GPUCCPLLTESTMUX,
  &ClockDomain_NPUCC_NPUCCCALDP,
  &ClockDomain_NPUCC_NPUCCDEBUGMUX,
  &ClockDomain_NPUCC_NPUCCNPUCORE,
  &ClockDomain_NPUCC_NPUCCPLLTESTMUX,
#ifdef ENABLE_AUTO_PLAT
  &ClockDomain_SCC_SCCMAINRCG,
  &ClockDomain_SCC_SCCQUPV3SE0,
  &ClockDomain_SCC_SCCQUPV3SE1,
  &ClockDomain_SCC_SCCQUPV3SE2,
  &ClockDomain_SCC_SCCQUPV3SE3,
  &ClockDomain_SCC_SCCQUPV3SE4,
  &ClockDomain_SCC_SCCQUPV3SE5,
#endif
  &ClockDomain_VIDEOCC_VIDEOCCIRIS,
  &ClockDomain_VIDEOCC_VIDEOCCSLEEP,
  &ClockDomain_VIDEOCC_VIDEOCCXO,
};


/*=========================================================================
      Clocks
==========================================================================*/

static ClockNodeType aClocks[] =
{
  /*-----------------------------------------------------------------------*/
  /* ClockDomain_CAMCC_CAMCCBPS                                            */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "cam_cc_bps_clk",
    .pDomain   = &ClockDomain_CAMCC_CAMCCBPS,
    .pDebugMux = &ClockDebugMux_CAMCC,
    .nUniqueId = 0x2FE00ED4,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(CAM_CC_BPS_CBCR), HWIO_PHYS(CAM_CC_BPS_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 11,
    },
  },
  {
    .szName    = "cam_cc_spdm_bps_clk",
    .pDomain   = &ClockDomain_CAMCC_CAMCCBPS,
    .pDebugMux = &ClockDebugMux_CAMCC,
    .nUniqueId = 0xF0BFA945,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(CAM_CC_SPDM_BPS_CBCR), HWIO_PHYS(CAM_CC_BPS_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 45,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_CAMCC_CAMCCCAMNOCAXI                                      */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "cam_cc_bps_axi_clk",
    .pDomain   = &ClockDomain_CAMCC_CAMCCCAMNOCAXI,
    .pDebugMux = &ClockDebugMux_CAMCC,
    .nUniqueId = 0x27EC2F88,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(CAM_CC_BPS_AXI_CBCR), HWIO_PHYS(CAM_CC_BPS_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 12,
    },
  },
  {
    .szName    = "cam_cc_camnoc_axi_clk",
    .pDomain   = &ClockDomain_CAMCC_CAMCCCAMNOCAXI,
    .pDebugMux = &ClockDebugMux_CAMCC,
    .nUniqueId = 0x8AA30E80,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(CAM_CC_CAMNOC_AXI_CBCR), HWIO_PHYS(CAM_CC_CAMNOC_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 39,
    },
  },
  {
    .szName    = "cam_cc_ife_0_axi_clk",
    .pDomain   = &ClockDomain_CAMCC_CAMCCCAMNOCAXI,
    .pDebugMux = &ClockDebugMux_CAMCC,
    .nUniqueId = 0x62A5CCDC,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(CAM_CC_IFE_0_AXI_CBCR), HWIO_PHYS(CAM_CC_IFE_0_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 27,
    },
  },
  {
    .szName    = "cam_cc_ife_1_axi_clk",
    .pDomain   = &ClockDomain_CAMCC_CAMCCCAMNOCAXI,
    .pDebugMux = &ClockDebugMux_CAMCC,
    .nUniqueId = 0xD12D93AA,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(CAM_CC_IFE_1_AXI_CBCR), HWIO_PHYS(CAM_CC_IFE_1_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 33,
    },
  },
  {
    .szName    = "cam_cc_ife_2_axi_clk",
    .pDomain   = &ClockDomain_CAMCC_CAMCCCAMNOCAXI,
    .pDebugMux = &ClockDebugMux_CAMCC,
    .nUniqueId = 0x31E1D953,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(CAM_CC_IFE_2_AXI_CBCR), HWIO_PHYS(CAM_CC_IFE_2_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 74,
    },
  },
  {
    .szName    = "cam_cc_ife_3_axi_clk",
    .pDomain   = &ClockDomain_CAMCC_CAMCCCAMNOCAXI,
    .pDebugMux = &ClockDebugMux_CAMCC,
    .nUniqueId = 0x3773461F,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(CAM_CC_IFE_3_AXI_CBCR), HWIO_PHYS(CAM_CC_IFE_3_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 81,
    },
  },
  {
    .szName    = "cam_cc_ipe_0_axi_clk",
    .pDomain   = &ClockDomain_CAMCC_CAMCCCAMNOCAXI,
    .pDebugMux = &ClockDebugMux_CAMCC,
    .nUniqueId = 0x831626E2,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(CAM_CC_IPE_0_AXI_CBCR), HWIO_PHYS(CAM_CC_IPE_0_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 16,
    },
  },
  {
    .szName    = "cam_cc_ipe_1_axi_clk",
    .pDomain   = &ClockDomain_CAMCC_CAMCCCAMNOCAXI,
    .pDebugMux = &ClockDebugMux_CAMCC,
    .nUniqueId = 0x06A42FA6,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(CAM_CC_IPE_1_AXI_CBCR), HWIO_PHYS(CAM_CC_IPE_1_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 20,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_CAMCC_CAMCCCCI0                                           */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "cam_cc_cci_0_clk",
    .pDomain   = &ClockDomain_CAMCC_CAMCCCCI0,
    .pDebugMux = &ClockDebugMux_CAMCC,
    .nUniqueId = 0x2F0FA585,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(CAM_CC_CCI_0_CBCR), HWIO_PHYS(CAM_CC_CCI_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 42,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_CAMCC_CAMCCCCI1                                           */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "cam_cc_cci_1_clk",
    .pDomain   = &ClockDomain_CAMCC_CAMCCCCI1,
    .pDebugMux = &ClockDebugMux_CAMCC,
    .nUniqueId = 0x0072D990,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(CAM_CC_CCI_1_CBCR), HWIO_PHYS(CAM_CC_CCI_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 59,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_CAMCC_CAMCCCCI2                                           */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "cam_cc_cci_2_clk",
    .pDomain   = &ClockDomain_CAMCC_CAMCCCCI2,
    .pDebugMux = &ClockDebugMux_CAMCC,
    .nUniqueId = 0x68C6ABCF,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(CAM_CC_CCI_2_CBCR), HWIO_PHYS(CAM_CC_CCI_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 91,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_CAMCC_CAMCCCCI3                                           */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "cam_cc_cci_3_clk",
    .pDomain   = &ClockDomain_CAMCC_CAMCCCCI3,
    .pDebugMux = &ClockDebugMux_CAMCC,
    .nUniqueId = 0x07E4E704,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(CAM_CC_CCI_3_CBCR), HWIO_PHYS(CAM_CC_CCI_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 92,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_CAMCC_CAMCCCPHYRX                                         */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "cam_cc_csiphy0_clk",
    .pDomain   = &ClockDomain_CAMCC_CAMCCCPHYRX,
    .pDebugMux = &ClockDebugMux_CAMCC,
    .nUniqueId = 0x5BD9C966,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(CAM_CC_CSIPHY0_CBCR), HWIO_PHYS(CAM_CC_CSI0PHY_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 6,
    },
  },
  {
    .szName    = "cam_cc_csiphy1_clk",
    .pDomain   = &ClockDomain_CAMCC_CAMCCCPHYRX,
    .pDebugMux = &ClockDebugMux_CAMCC,
    .nUniqueId = 0xD1C62B7E,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(CAM_CC_CSIPHY1_CBCR), HWIO_PHYS(CAM_CC_CSI1PHY_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 8,
    },
  },
  {
    .szName    = "cam_cc_csiphy2_clk",
    .pDomain   = &ClockDomain_CAMCC_CAMCCCPHYRX,
    .pDebugMux = &ClockDebugMux_CAMCC,
    .nUniqueId = 0xFB341D3F,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(CAM_CC_CSIPHY2_CBCR), HWIO_PHYS(CAM_CC_CSI2PHY_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 10,
    },
  },
  {
    .szName    = "cam_cc_csiphy3_clk",
    .pDomain   = &ClockDomain_CAMCC_CAMCCCPHYRX,
    .pDebugMux = &ClockDebugMux_CAMCC,
    .nUniqueId = 0x29439ED4,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(CAM_CC_CSIPHY3_CBCR), HWIO_PHYS(CAM_CC_CSI3PHY_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 54,
    },
  },
  {
    .szName    = "cam_cc_ife_0_cphy_rx_clk",
    .pDomain   = &ClockDomain_CAMCC_CAMCCCPHYRX,
    .pDebugMux = &ClockDebugMux_CAMCC,
    .nUniqueId = 0x94279150,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(CAM_CC_IFE_0_CPHY_RX_CBCR), HWIO_PHYS(CAM_CC_IFE_0_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 26,
    },
  },
  {
    .szName    = "cam_cc_ife_1_cphy_rx_clk",
    .pDomain   = &ClockDomain_CAMCC_CAMCCCPHYRX,
    .pDebugMux = &ClockDebugMux_CAMCC,
    .nUniqueId = 0x0BA4A1D1,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(CAM_CC_IFE_1_CPHY_RX_CBCR), HWIO_PHYS(CAM_CC_IFE_1_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 32,
    },
  },
  {
    .szName    = "cam_cc_ife_2_cphy_rx_clk",
    .pDomain   = &ClockDomain_CAMCC_CAMCCCPHYRX,
    .pDebugMux = &ClockDebugMux_CAMCC,
    .nUniqueId = 0x2EB3D283,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(CAM_CC_IFE_2_CPHY_RX_CBCR), HWIO_PHYS(CAM_CC_IFE_2_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 73,
    },
  },
  {
    .szName    = "cam_cc_ife_3_cphy_rx_clk",
    .pDomain   = &ClockDomain_CAMCC_CAMCCCPHYRX,
    .pDebugMux = &ClockDebugMux_CAMCC,
    .nUniqueId = 0xFAD305C8,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(CAM_CC_IFE_3_CPHY_RX_CBCR), HWIO_PHYS(CAM_CC_IFE_3_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 80,
    },
  },
  {
    .szName    = "cam_cc_ife_lite_0_cphy_rx_clk",
    .pDomain   = &ClockDomain_CAMCC_CAMCCCPHYRX,
    .pDebugMux = &ClockDebugMux_CAMCC,
    .nUniqueId = 0x6E2D801F,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(CAM_CC_IFE_LITE_0_CPHY_RX_CBCR), HWIO_PHYS(CAM_CC_IFE_LITE_0_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 36,
    },
  },
  {
    .szName    = "cam_cc_ife_lite_1_cphy_rx_clk",
    .pDomain   = &ClockDomain_CAMCC_CAMCCCPHYRX,
    .pDebugMux = &ClockDebugMux_CAMCC,
    .nUniqueId = 0x39B1D748,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(CAM_CC_IFE_LITE_1_CPHY_RX_CBCR), HWIO_PHYS(CAM_CC_IFE_LITE_1_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 58,
    },
  },
  {
    .szName    = "cam_cc_ife_lite_2_cphy_rx_clk",
    .pDomain   = &ClockDomain_CAMCC_CAMCCCPHYRX,
    .pDebugMux = &ClockDebugMux_CAMCC,
    .nUniqueId = 0x7F2C1E50,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(CAM_CC_IFE_LITE_2_CPHY_RX_CBCR), HWIO_PHYS(CAM_CC_IFE_LITE_2_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 87,
    },
  },
  {
    .szName    = "cam_cc_ife_lite_3_cphy_rx_clk",
    .pDomain   = &ClockDomain_CAMCC_CAMCCCPHYRX,
    .pDebugMux = &ClockDebugMux_CAMCC,
    .nUniqueId = 0xA89D107C,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(CAM_CC_IFE_LITE_3_CPHY_RX_CBCR), HWIO_PHYS(CAM_CC_IFE_LITE_3_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 90,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_CAMCC_CAMCCCSI0PHYTIMER                                   */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "cam_cc_csi0phytimer_clk",
    .pDomain   = &ClockDomain_CAMCC_CAMCCCSI0PHYTIMER,
    .pDebugMux = &ClockDebugMux_CAMCC,
    .nUniqueId = 0xDF3E55F8,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(CAM_CC_CSI0PHYTIMER_CBCR), HWIO_PHYS(CAM_CC_CSI0PHY_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 5,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_CAMCC_CAMCCCSI1PHYTIMER                                   */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "cam_cc_csi1phytimer_clk",
    .pDomain   = &ClockDomain_CAMCC_CAMCCCSI1PHYTIMER,
    .pDebugMux = &ClockDebugMux_CAMCC,
    .nUniqueId = 0xBD91807B,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(CAM_CC_CSI1PHYTIMER_CBCR), HWIO_PHYS(CAM_CC_CSI1PHY_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 7,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_CAMCC_CAMCCCSI2PHYTIMER                                   */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "cam_cc_csi2phytimer_clk",
    .pDomain   = &ClockDomain_CAMCC_CAMCCCSI2PHYTIMER,
    .pDebugMux = &ClockDebugMux_CAMCC,
    .nUniqueId = 0x411BC4E9,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(CAM_CC_CSI2PHYTIMER_CBCR), HWIO_PHYS(CAM_CC_CSI2PHY_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 9,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_CAMCC_CAMCCCSI3PHYTIMER                                   */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "cam_cc_csi3phytimer_clk",
    .pDomain   = &ClockDomain_CAMCC_CAMCCCSI3PHYTIMER,
    .pDebugMux = &ClockDebugMux_CAMCC,
    .nUniqueId = 0xD8F7C361,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(CAM_CC_CSI3PHYTIMER_CBCR), HWIO_PHYS(CAM_CC_CSI3PHY_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 53,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_CAMCC_CAMCCDEBUGMUX                                       */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "cam_cc_debug_clk",
    .pDomain   = &ClockDomain_CAMCC_CAMCCDEBUGMUX,
    .pDebugMux = NULL,
    .nUniqueId = 0x8E6D4CE1,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(CAM_CC_DEBUG_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = HAL_CLK_NO_DEBUG_MUX_SEL,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_CAMCC_CAMCCFASTAHB                                        */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "cam_cc_bps_areg_clk",
    .pDomain   = &ClockDomain_CAMCC_CAMCCFASTAHB,
    .pDebugMux = &ClockDebugMux_CAMCC,
    .nUniqueId = 0x94A0ECE6,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(CAM_CC_BPS_AREG_CBCR), HWIO_PHYS(CAM_CC_BPS_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 13,
    },
  },
  {
    .szName    = "cam_cc_ipe_0_areg_clk",
    .pDomain   = &ClockDomain_CAMCC_CAMCCFASTAHB,
    .pDebugMux = &ClockDebugMux_CAMCC,
    .nUniqueId = 0x647660BB,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(CAM_CC_IPE_0_AREG_CBCR), HWIO_PHYS(CAM_CC_IPE_0_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 17,
    },
  },
  {
    .szName    = "cam_cc_ipe_1_areg_clk",
    .pDomain   = &ClockDomain_CAMCC_CAMCCFASTAHB,
    .pDebugMux = &ClockDebugMux_CAMCC,
    .nUniqueId = 0x186180C8,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(CAM_CC_IPE_1_AREG_CBCR), HWIO_PHYS(CAM_CC_IPE_1_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 21,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_CAMCC_CAMCCFDCORE                                         */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "cam_cc_fd_core_clk",
    .pDomain   = &ClockDomain_CAMCC_CAMCCFDCORE,
    .pDebugMux = &ClockDebugMux_CAMCC,
    .nUniqueId = 0x783197F9,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(CAM_CC_FD_CORE_CBCR), HWIO_PHYS(CAM_CC_FD_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 40,
    },
  },
  {
    .szName    = "cam_cc_fd_core_uar_clk",
    .pDomain   = &ClockDomain_CAMCC_CAMCCFDCORE,
    .pDebugMux = &ClockDebugMux_CAMCC,
    .nUniqueId = 0x229F9FC9,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(CAM_CC_FD_CORE_UAR_CBCR), HWIO_PHYS(CAM_CC_FD_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 41,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_CAMCC_CAMCCICP                                            */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "cam_cc_icp_clk",
    .pDomain   = &ClockDomain_CAMCC_CAMCCICP,
    .pDebugMux = &ClockDebugMux_CAMCC,
    .nUniqueId = 0xFDC6BEB1,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(CAM_CC_ICP_CBCR), HWIO_PHYS(CAM_CC_ICP_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 38,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_CAMCC_CAMCCIFE0                                           */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "cam_cc_ife_0_atspeedreg_clk",
    .pDomain   = &ClockDomain_CAMCC_CAMCCIFE0,
    .pDebugMux = NULL,
    .nUniqueId = 0xD9EBAAB1,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { 0, HWIO_PHYS(CAM_CC_IFE_0_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = HAL_CLK_NO_DEBUG_MUX_SEL,
    },
  },
  {
    .szName    = "cam_cc_ife_0_clk",
    .pDomain   = &ClockDomain_CAMCC_CAMCCIFE0,
    .pDebugMux = &ClockDebugMux_CAMCC,
    .nUniqueId = 0x93CC2ED3,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(CAM_CC_IFE_0_CBCR), HWIO_PHYS(CAM_CC_IFE_0_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 23,
    },
  },
  {
    .szName    = "cam_cc_ife_0_dsp_clk",
    .pDomain   = &ClockDomain_CAMCC_CAMCCIFE0,
    .pDebugMux = &ClockDebugMux_CAMCC,
    .nUniqueId = 0xCA294DE3,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(CAM_CC_IFE_0_DSP_CBCR), HWIO_PHYS(CAM_CC_IFE_0_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 24,
    },
  },
  {
    .szName    = "cam_cc_spdm_ife_0_clk",
    .pDomain   = &ClockDomain_CAMCC_CAMCCIFE0,
    .pDebugMux = &ClockDebugMux_CAMCC,
    .nUniqueId = 0x8B62CE74,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(CAM_CC_SPDM_IFE_0_CBCR), HWIO_PHYS(CAM_CC_IFE_0_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 49,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_CAMCC_CAMCCIFE0CSID                                       */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "cam_cc_ife_0_csid_clk",
    .pDomain   = &ClockDomain_CAMCC_CAMCCIFE0CSID,
    .pDebugMux = &ClockDebugMux_CAMCC,
    .nUniqueId = 0x336D0D27,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(CAM_CC_IFE_0_CSID_CBCR), HWIO_PHYS(CAM_CC_IFE_0_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 25,
    },
  },
  {
    .szName    = "cam_cc_spdm_ife_0_csid_clk",
    .pDomain   = &ClockDomain_CAMCC_CAMCCIFE0CSID,
    .pDebugMux = &ClockDebugMux_CAMCC,
    .nUniqueId = 0xD05FD3DB,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(CAM_CC_SPDM_IFE_0_CSID_CBCR), HWIO_PHYS(CAM_CC_IFE_0_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 50,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_CAMCC_CAMCCIFE1                                           */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "cam_cc_ife_1_atspeedreg_clk",
    .pDomain   = &ClockDomain_CAMCC_CAMCCIFE1,
    .pDebugMux = NULL,
    .nUniqueId = 0xF7A8E377,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { 0, HWIO_PHYS(CAM_CC_IFE_1_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = HAL_CLK_NO_DEBUG_MUX_SEL,
    },
  },
  {
    .szName    = "cam_cc_ife_1_clk",
    .pDomain   = &ClockDomain_CAMCC_CAMCCIFE1,
    .pDebugMux = &ClockDebugMux_CAMCC,
    .nUniqueId = 0x20EDB204,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(CAM_CC_IFE_1_CBCR), HWIO_PHYS(CAM_CC_IFE_1_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 29,
    },
  },
  {
    .szName    = "cam_cc_ife_1_dsp_clk",
    .pDomain   = &ClockDomain_CAMCC_CAMCCIFE1,
    .pDebugMux = &ClockDebugMux_CAMCC,
    .nUniqueId = 0x44BE7A4B,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(CAM_CC_IFE_1_DSP_CBCR), HWIO_PHYS(CAM_CC_IFE_1_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 30,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_CAMCC_CAMCCIFE1CSID                                       */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "cam_cc_ife_1_csid_clk",
    .pDomain   = &ClockDomain_CAMCC_CAMCCIFE1CSID,
    .pDebugMux = &ClockDebugMux_CAMCC,
    .nUniqueId = 0x3BA9C280,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(CAM_CC_IFE_1_CSID_CBCR), HWIO_PHYS(CAM_CC_IFE_1_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 31,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_CAMCC_CAMCCIFE2                                           */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "cam_cc_ife_2_atspeedreg_clk",
    .pDomain   = &ClockDomain_CAMCC_CAMCCIFE2,
    .pDebugMux = NULL,
    .nUniqueId = 0x3A602ADF,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { 0, HWIO_PHYS(CAM_CC_IFE_2_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = HAL_CLK_NO_DEBUG_MUX_SEL,
    },
  },
  {
    .szName    = "cam_cc_ife_2_clk",
    .pDomain   = &ClockDomain_CAMCC_CAMCCIFE2,
    .pDebugMux = &ClockDebugMux_CAMCC,
    .nUniqueId = 0x1A2BDA1C,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(CAM_CC_IFE_2_CBCR), HWIO_PHYS(CAM_CC_IFE_2_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 68,
    },
  },
  {
    .szName    = "cam_cc_ife_2_dsp_clk",
    .pDomain   = &ClockDomain_CAMCC_CAMCCIFE2,
    .pDebugMux = &ClockDebugMux_CAMCC,
    .nUniqueId = 0x64428D0F,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(CAM_CC_IFE_2_DSP_CBCR), HWIO_PHYS(CAM_CC_IFE_2_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 70,
    },
  },
  {
    .szName    = "cam_cc_spdm_ife_2_clk",
    .pDomain   = &ClockDomain_CAMCC_CAMCCIFE2,
    .pDebugMux = &ClockDebugMux_CAMCC,
    .nUniqueId = 0xDB780231,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(CAM_CC_SPDM_IFE_2_CBCR), HWIO_PHYS(CAM_CC_IFE_2_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 69,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_CAMCC_CAMCCIFE2CSID                                       */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "cam_cc_ife_2_csid_clk",
    .pDomain   = &ClockDomain_CAMCC_CAMCCIFE2CSID,
    .pDebugMux = &ClockDebugMux_CAMCC,
    .nUniqueId = 0x9068AD14,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(CAM_CC_IFE_2_CSID_CBCR), HWIO_PHYS(CAM_CC_IFE_2_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 71,
    },
  },
  {
    .szName    = "cam_cc_spdm_ife_2_csid_clk",
    .pDomain   = &ClockDomain_CAMCC_CAMCCIFE2CSID,
    .pDebugMux = &ClockDebugMux_CAMCC,
    .nUniqueId = 0xB7CE433F,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(CAM_CC_SPDM_IFE_2_CSID_CBCR), HWIO_PHYS(CAM_CC_IFE_2_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 72,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_CAMCC_CAMCCIFE3                                           */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "cam_cc_ife_3_atspeedreg_clk",
    .pDomain   = &ClockDomain_CAMCC_CAMCCIFE3,
    .pDebugMux = NULL,
    .nUniqueId = 0x9AD43F6E,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { 0, HWIO_PHYS(CAM_CC_IFE_3_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = HAL_CLK_NO_DEBUG_MUX_SEL,
    },
  },
  {
    .szName    = "cam_cc_ife_3_clk",
    .pDomain   = &ClockDomain_CAMCC_CAMCCIFE3,
    .pDebugMux = &ClockDebugMux_CAMCC,
    .nUniqueId = 0xBB3A13E8,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(CAM_CC_IFE_3_CBCR), HWIO_PHYS(CAM_CC_IFE_3_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 75,
    },
  },
  {
    .szName    = "cam_cc_ife_3_dsp_clk",
    .pDomain   = &ClockDomain_CAMCC_CAMCCIFE3,
    .pDebugMux = &ClockDebugMux_CAMCC,
    .nUniqueId = 0x5C3AF373,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(CAM_CC_IFE_3_DSP_CBCR), HWIO_PHYS(CAM_CC_IFE_3_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 77,
    },
  },
  {
    .szName    = "cam_cc_spdm_ife_3_clk",
    .pDomain   = &ClockDomain_CAMCC_CAMCCIFE3,
    .pDebugMux = &ClockDebugMux_CAMCC,
    .nUniqueId = 0x28526F96,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(CAM_CC_SPDM_IFE_3_CBCR), HWIO_PHYS(CAM_CC_IFE_3_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 76,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_CAMCC_CAMCCIFE3CSID                                       */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "cam_cc_ife_3_csid_clk",
    .pDomain   = &ClockDomain_CAMCC_CAMCCIFE3CSID,
    .pDebugMux = &ClockDebugMux_CAMCC,
    .nUniqueId = 0x7882FA5F,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(CAM_CC_IFE_3_CSID_CBCR), HWIO_PHYS(CAM_CC_IFE_3_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 78,
    },
  },
  {
    .szName    = "cam_cc_spdm_ife_3_csid_clk",
    .pDomain   = &ClockDomain_CAMCC_CAMCCIFE3CSID,
    .pDebugMux = &ClockDebugMux_CAMCC,
    .nUniqueId = 0x8B6F8940,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(CAM_CC_SPDM_IFE_3_CSID_CBCR), HWIO_PHYS(CAM_CC_IFE_3_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 79,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_CAMCC_CAMCCIFELITE0                                       */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "cam_cc_ife_lite_0_clk",
    .pDomain   = &ClockDomain_CAMCC_CAMCCIFELITE0,
    .pDebugMux = &ClockDebugMux_CAMCC,
    .nUniqueId = 0xC035C6EE,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(CAM_CC_IFE_LITE_0_CBCR), HWIO_PHYS(CAM_CC_IFE_LITE_0_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 34,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_CAMCC_CAMCCIFELITE0CSID                                   */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "cam_cc_ife_lite_0_csid_clk",
    .pDomain   = &ClockDomain_CAMCC_CAMCCIFELITE0CSID,
    .pDebugMux = &ClockDebugMux_CAMCC,
    .nUniqueId = 0x31D0C2DB,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(CAM_CC_IFE_LITE_0_CSID_CBCR), HWIO_PHYS(CAM_CC_IFE_LITE_0_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 35,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_CAMCC_CAMCCIFELITE1                                       */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "cam_cc_ife_lite_1_clk",
    .pDomain   = &ClockDomain_CAMCC_CAMCCIFELITE1,
    .pDebugMux = &ClockDebugMux_CAMCC,
    .nUniqueId = 0x298E18F2,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(CAM_CC_IFE_LITE_1_CBCR), HWIO_PHYS(CAM_CC_IFE_LITE_1_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 56,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_CAMCC_CAMCCIFELITE1CSID                                   */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "cam_cc_ife_lite_1_csid_clk",
    .pDomain   = &ClockDomain_CAMCC_CAMCCIFELITE1CSID,
    .pDebugMux = &ClockDebugMux_CAMCC,
    .nUniqueId = 0x36D0A82E,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(CAM_CC_IFE_LITE_1_CSID_CBCR), HWIO_PHYS(CAM_CC_IFE_LITE_1_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 57,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_CAMCC_CAMCCIFELITE2                                       */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "cam_cc_ife_lite_2_clk",
    .pDomain   = &ClockDomain_CAMCC_CAMCCIFELITE2,
    .pDebugMux = &ClockDebugMux_CAMCC,
    .nUniqueId = 0xD85913C3,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(CAM_CC_IFE_LITE_2_CBCR), HWIO_PHYS(CAM_CC_IFE_LITE_2_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 85,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_CAMCC_CAMCCIFELITE2CSID                                   */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "cam_cc_ife_lite_2_csid_clk",
    .pDomain   = &ClockDomain_CAMCC_CAMCCIFELITE2CSID,
    .pDebugMux = &ClockDebugMux_CAMCC,
    .nUniqueId = 0x935F0F94,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(CAM_CC_IFE_LITE_2_CSID_CBCR), HWIO_PHYS(CAM_CC_IFE_LITE_2_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 86,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_CAMCC_CAMCCIFELITE3                                       */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "cam_cc_ife_lite_3_clk",
    .pDomain   = &ClockDomain_CAMCC_CAMCCIFELITE3,
    .pDebugMux = &ClockDebugMux_CAMCC,
    .nUniqueId = 0x1A473378,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(CAM_CC_IFE_LITE_3_CBCR), HWIO_PHYS(CAM_CC_IFE_LITE_3_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 88,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_CAMCC_CAMCCIFELITE3CSID                                   */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "cam_cc_ife_lite_3_csid_clk",
    .pDomain   = &ClockDomain_CAMCC_CAMCCIFELITE3CSID,
    .pDebugMux = &ClockDebugMux_CAMCC,
    .nUniqueId = 0xB3BC8CF4,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(CAM_CC_IFE_LITE_3_CSID_CBCR), HWIO_PHYS(CAM_CC_IFE_LITE_3_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 89,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_CAMCC_CAMCCIPE0                                           */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "cam_cc_ipe_0_clk",
    .pDomain   = &ClockDomain_CAMCC_CAMCCIPE0,
    .pDebugMux = &ClockDebugMux_CAMCC,
    .nUniqueId = 0xA15076BA,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(CAM_CC_IPE_0_CBCR), HWIO_PHYS(CAM_CC_IPE_0_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 15,
    },
  },
  {
    .szName    = "cam_cc_ipe_1_clk",
    .pDomain   = &ClockDomain_CAMCC_CAMCCIPE0,
    .pDebugMux = &ClockDebugMux_CAMCC,
    .nUniqueId = 0xA6BA0F5D,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(CAM_CC_IPE_1_CBCR), HWIO_PHYS(CAM_CC_IPE_1_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 19,
    },
  },
  {
    .szName    = "cam_cc_spdm_ipe_0_clk",
    .pDomain   = &ClockDomain_CAMCC_CAMCCIPE0,
    .pDebugMux = &ClockDebugMux_CAMCC,
    .nUniqueId = 0xF8B85420,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(CAM_CC_SPDM_IPE_0_CBCR), HWIO_PHYS(CAM_CC_IPE_0_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 47,
    },
  },
  {
    .szName    = "cam_cc_spdm_ipe_1_clk",
    .pDomain   = &ClockDomain_CAMCC_CAMCCIPE0,
    .pDebugMux = &ClockDebugMux_CAMCC,
    .nUniqueId = 0x1DF96669,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(CAM_CC_SPDM_IPE_1_CBCR), HWIO_PHYS(CAM_CC_IPE_1_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 48,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_CAMCC_CAMCCJPEG                                           */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "cam_cc_jpeg_clk",
    .pDomain   = &ClockDomain_CAMCC_CAMCCJPEG,
    .pDebugMux = &ClockDebugMux_CAMCC,
    .nUniqueId = 0x9F6924DC,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(CAM_CC_JPEG_CBCR), HWIO_PHYS(CAM_CC_JPEG_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 37,
    },
  },
  {
    .szName    = "cam_cc_spdm_jpeg_clk",
    .pDomain   = &ClockDomain_CAMCC_CAMCCJPEG,
    .pDebugMux = &ClockDebugMux_CAMCC,
    .nUniqueId = 0x244595C2,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(CAM_CC_SPDM_JPEG_CBCR), HWIO_PHYS(CAM_CC_JPEG_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 52,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_CAMCC_CAMCCLRME                                           */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "cam_cc_lrme_clk",
    .pDomain   = &ClockDomain_CAMCC_CAMCCLRME,
    .pDebugMux = &ClockDebugMux_CAMCC,
    .nUniqueId = 0x2F3C280A,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(CAM_CC_LRME_CBCR), HWIO_PHYS(CAM_CC_LRME_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 43,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_CAMCC_CAMCCMCLK0                                          */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "cam_cc_mclk0_clk",
    .pDomain   = &ClockDomain_CAMCC_CAMCCMCLK0,
    .pDebugMux = &ClockDebugMux_CAMCC,
    .nUniqueId = 0xAFDBB67A,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(CAM_CC_MCLK0_CBCR), HWIO_PHYS(CAM_CC_MCLK0_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 1,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_CAMCC_CAMCCMCLK1                                          */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "cam_cc_mclk1_clk",
    .pDomain   = &ClockDomain_CAMCC_CAMCCMCLK1,
    .pDebugMux = &ClockDebugMux_CAMCC,
    .nUniqueId = 0x57396D81,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(CAM_CC_MCLK1_CBCR), HWIO_PHYS(CAM_CC_MCLK1_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 2,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_CAMCC_CAMCCMCLK2                                          */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "cam_cc_mclk2_clk",
    .pDomain   = &ClockDomain_CAMCC_CAMCCMCLK2,
    .pDebugMux = &ClockDebugMux_CAMCC,
    .nUniqueId = 0x70806571,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(CAM_CC_MCLK2_CBCR), HWIO_PHYS(CAM_CC_MCLK2_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 3,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_CAMCC_CAMCCMCLK3                                          */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "cam_cc_mclk3_clk",
    .pDomain   = &ClockDomain_CAMCC_CAMCCMCLK3,
    .pDebugMux = &ClockDebugMux_CAMCC,
    .nUniqueId = 0x35F364E9,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(CAM_CC_MCLK3_CBCR), HWIO_PHYS(CAM_CC_MCLK3_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 4,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_CAMCC_CAMCCMCLK4                                          */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "cam_cc_mclk4_clk",
    .pDomain   = &ClockDomain_CAMCC_CAMCCMCLK4,
    .pDebugMux = &ClockDebugMux_CAMCC,
    .nUniqueId = 0x315FF885,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(CAM_CC_MCLK4_CBCR), HWIO_PHYS(CAM_CC_MCLK4_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 93,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_CAMCC_CAMCCMCLK5                                          */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "cam_cc_mclk5_clk",
    .pDomain   = &ClockDomain_CAMCC_CAMCCMCLK5,
    .pDebugMux = &ClockDebugMux_CAMCC,
    .nUniqueId = 0xD8521D1F,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(CAM_CC_MCLK5_CBCR), HWIO_PHYS(CAM_CC_MCLK5_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 94,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_CAMCC_CAMCCMCLK6                                          */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "cam_cc_mclk6_clk",
    .pDomain   = &ClockDomain_CAMCC_CAMCCMCLK6,
    .pDebugMux = &ClockDebugMux_CAMCC,
    .nUniqueId = 0x405311E7,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(CAM_CC_MCLK6_CBCR), HWIO_PHYS(CAM_CC_MCLK6_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 95,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_CAMCC_CAMCCMCLK7                                          */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "cam_cc_mclk7_clk",
    .pDomain   = &ClockDomain_CAMCC_CAMCCMCLK7,
    .pDebugMux = &ClockDebugMux_CAMCC,
    .nUniqueId = 0x19DCEE64,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(CAM_CC_MCLK7_CBCR), HWIO_PHYS(CAM_CC_MCLK7_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 96,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_CAMCC_CAMCCPLLTESTMUX                                     */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "cam_cc_pll_test_clk",
    .pDomain   = &ClockDomain_CAMCC_CAMCCPLLTESTMUX,
    .pDebugMux = NULL,
    .nUniqueId = 0x519E3D27,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(CAM_CC_PLL_TEST_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = HAL_CLK_NO_DEBUG_MUX_SEL,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_CAMCC_CAMCCQDSSDEBUG                                      */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "cam_cc_qdss_debug_clk",
    .pDomain   = &ClockDomain_CAMCC_CAMCCQDSSDEBUG,
    .pDebugMux = &ClockDebugMux_CAMCC,
    .nUniqueId = 0x1843A6D1,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(CAM_CC_QDSS_DEBUG_CBCR), HWIO_PHYS(CAM_CC_QDSS_DEBUG_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 61,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_CAMCC_CAMCCSLEEP                                          */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "cam_cc_sleep_clk",
    .pDomain   = &ClockDomain_CAMCC_CAMCCSLEEP,
    .pDebugMux = &ClockDebugMux_CAMCC,
    .nUniqueId = 0xCFDF030C,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(CAM_CC_SLEEP_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 63,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_CAMCC_CAMCCSLOWAHB                                        */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "cam_cc_bps_ahb_clk",
    .pDomain   = &ClockDomain_CAMCC_CAMCCSLOWAHB,
    .pDebugMux = &ClockDebugMux_CAMCC,
    .nUniqueId = 0xCE8465C2,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(CAM_CC_BPS_AHB_CBCR), HWIO_PHYS(CAM_CC_BPS_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 14,
    },
  },
  {
    .szName    = "cam_cc_core_ahb_clk",
    .pDomain   = &ClockDomain_CAMCC_CAMCCSLOWAHB,
    .pDebugMux = &ClockDebugMux_CAMCC,
    .nUniqueId = 0xA974C223,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(CAM_CC_CORE_AHB_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 46,
    },
  },
  {
    .szName    = "cam_cc_cpas_ahb_clk",
    .pDomain   = &ClockDomain_CAMCC_CAMCCSLOWAHB,
    .pDebugMux = &ClockDebugMux_CAMCC,
    .nUniqueId = 0x7E6C7824,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(CAM_CC_CPAS_AHB_CBCR), HWIO_PHYS(CAM_CC_CPAS_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 44,
    },
  },
  {
    .szName    = "cam_cc_icp_ahb_clk",
    .pDomain   = &ClockDomain_CAMCC_CAMCCSLOWAHB,
    .pDebugMux = &ClockDebugMux_CAMCC,
    .nUniqueId = 0x32CA8C6A,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(CAM_CC_ICP_AHB_CBCR), HWIO_PHYS(CAM_CC_ICP_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 55,
    },
  },
  {
    .szName    = "cam_cc_ipe_0_ahb_clk",
    .pDomain   = &ClockDomain_CAMCC_CAMCCSLOWAHB,
    .pDebugMux = &ClockDebugMux_CAMCC,
    .nUniqueId = 0x93BDABBB,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(CAM_CC_IPE_0_AHB_CBCR), HWIO_PHYS(CAM_CC_IPE_0_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 18,
    },
  },
  {
    .szName    = "cam_cc_ipe_1_ahb_clk",
    .pDomain   = &ClockDomain_CAMCC_CAMCCSLOWAHB,
    .pDebugMux = &ClockDebugMux_CAMCC,
    .nUniqueId = 0x1AC7D611,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(CAM_CC_IPE_1_AHB_CBCR), HWIO_PHYS(CAM_CC_IPE_1_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 22,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_CAMCC_CAMCCXO                                             */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "cam_cc_camnoc_dcd_xo_clk",
    .pDomain   = &ClockDomain_CAMCC_CAMCCXO,
    .pDebugMux = &ClockDebugMux_CAMCC,
    .nUniqueId = 0xD2B89C42,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(CAM_CC_CAMNOC_DCD_XO_CBCR), HWIO_PHYS(CAM_CC_CAMNOC_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 51,
    },
  },
  {
    .szName    = "cam_cc_gdsc_clk",
    .pDomain   = &ClockDomain_CAMCC_CAMCCXO,
    .pDebugMux = &ClockDebugMux_CAMCC,
    .nUniqueId = 0xC9FEBE6E,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(CAM_CC_GDSC_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 60,
    },
  },
  {
    .szName    = "cam_cc_qdss_debug_xo_clk",
    .pDomain   = &ClockDomain_CAMCC_CAMCCXO,
    .pDebugMux = &ClockDebugMux_CAMCC,
    .nUniqueId = 0xFC7EF9CE,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(CAM_CC_QDSS_DEBUG_XO_CBCR), HWIO_PHYS(CAM_CC_QDSS_DEBUG_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 62,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_DISPCC_DISPCCMDSSAHB                                      */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "disp_cc_mdss_ahb_clk",
    .pDomain   = &ClockDomain_DISPCC_DISPCCMDSSAHB,
    .pDebugMux = &ClockDebugMux_DISPCC,
    .nUniqueId = 0xD86CF434,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(DISP_CC_MDSS_AHB_CBCR), HWIO_PHYS(DISP_CC_MDSS_CORE_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 43,
    },
  },
  {
    .szName    = "disp_cc_mdss_non_gdsc_ahb_clk",
    .pDomain   = &ClockDomain_DISPCC_DISPCCMDSSAHB,
    .pDebugMux = &ClockDebugMux_DISPCC,
    .nUniqueId = 0xD594BB06,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(DISP_CC_MDSS_NON_GDSC_AHB_CBCR), HWIO_PHYS(DISP_CC_MDSS_RSCC_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 44,
    },
  },
  {
    .szName    = "disp_cc_mdss_rscc_ahb_clk",
    .pDomain   = &ClockDomain_DISPCC_DISPCCMDSSAHB,
    .pDebugMux = &ClockDebugMux_DISPCC,
    .nUniqueId = 0x23666C25,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(DISP_CC_MDSS_RSCC_AHB_CBCR), HWIO_PHYS(DISP_CC_MDSS_RSCC_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 46,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_DISPCC_DISPCCMDSSBYTE0                                    */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "disp_cc_mdss_byte0_clk",
    .pDomain   = &ClockDomain_DISPCC_DISPCCMDSSBYTE0,
    .pDebugMux = &ClockDebugMux_DISPCC,
    .nUniqueId = 0x62A800C1,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(DISP_CC_MDSS_BYTE0_CBCR), HWIO_PHYS(DISP_CC_MDSS_CORE_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 21,
    },
  },
  {
    .szName    = "disp_cc_mdss_byte0_intf_clk",
    .pDomain   = &ClockDomain_DISPCC_DISPCCMDSSBYTE0,
    .pDebugMux = &ClockDebugMux_DISPCC,
    .nUniqueId = 0xEC7D494E,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(DISP_CC_MDSS_BYTE0_INTF_CBCR), HWIO_PHYS(DISP_CC_MDSS_CORE_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 22,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_DISPCC_DISPCCMDSSBYTE1                                    */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "disp_cc_mdss_byte1_clk",
    .pDomain   = &ClockDomain_DISPCC_DISPCCMDSSBYTE1,
    .pDebugMux = &ClockDebugMux_DISPCC,
    .nUniqueId = 0x0DE1DE46,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(DISP_CC_MDSS_BYTE1_CBCR), HWIO_PHYS(DISP_CC_MDSS_CORE_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 23,
    },
  },
  {
    .szName    = "disp_cc_mdss_byte1_intf_clk",
    .pDomain   = &ClockDomain_DISPCC_DISPCCMDSSBYTE1,
    .pDebugMux = &ClockDebugMux_DISPCC,
    .nUniqueId = 0xFEEDA5FA,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(DISP_CC_MDSS_BYTE1_INTF_CBCR), HWIO_PHYS(DISP_CC_MDSS_CORE_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 24,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_DISPCC_DISPCCMDSSDPAUX1                                   */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "disp_cc_mdss_dp_aux1_clk",
    .pDomain   = &ClockDomain_DISPCC_DISPCCMDSSDPAUX1,
    .pDebugMux = &ClockDebugMux_DISPCC,
    .nUniqueId = 0x55601B02,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(DISP_CC_MDSS_DP_AUX1_CBCR), HWIO_PHYS(DISP_CC_MDSS_CORE_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 37,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_DISPCC_DISPCCMDSSDPAUX                                    */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "disp_cc_mdss_dp_aux_clk",
    .pDomain   = &ClockDomain_DISPCC_DISPCCMDSSDPAUX,
    .pDebugMux = &ClockDebugMux_DISPCC,
    .nUniqueId = 0x1F21F4AB,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(DISP_CC_MDSS_DP_AUX_CBCR), HWIO_PHYS(DISP_CC_MDSS_CORE_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 32,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_DISPCC_DISPCCMDSSDPCRYPTO1                                */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "disp_cc_mdss_dp_crypto1_clk",
    .pDomain   = &ClockDomain_DISPCC_DISPCCMDSSDPCRYPTO1,
    .pDebugMux = &ClockDebugMux_DISPCC,
    .nUniqueId = 0x17934F54,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(DISP_CC_MDSS_DP_CRYPTO1_CBCR), HWIO_PHYS(DISP_CC_MDSS_CORE_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 36,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_DISPCC_DISPCCMDSSDPCRYPTO                                 */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "disp_cc_mdss_dp_crypto_clk",
    .pDomain   = &ClockDomain_DISPCC_DISPCCMDSSDPCRYPTO,
    .pDebugMux = &ClockDebugMux_DISPCC,
    .nUniqueId = 0x117E7413,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(DISP_CC_MDSS_DP_CRYPTO_CBCR), HWIO_PHYS(DISP_CC_MDSS_CORE_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 29,
    },
  },
  {
    .szName    = "disp_cc_mdss_spdm_dp_crypto_clk",
    .pDomain   = &ClockDomain_DISPCC_DISPCCMDSSDPCRYPTO,
    .pDebugMux = &ClockDebugMux_DISPCC,
    .nUniqueId = 0x75EFBB45,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(DISP_CC_MDSS_SPDM_DP_CRYPTO_CBCR), HWIO_PHYS(DISP_CC_MDSS_SPDM_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 51,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_DISPCC_DISPCCMDSSDPLINK1                                  */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "disp_cc_mdss_dp_link1_clk",
    .pDomain   = &ClockDomain_DISPCC_DISPCCMDSSDPLINK1,
    .pDebugMux = &ClockDebugMux_DISPCC,
    .nUniqueId = 0x9D38A7D2,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(DISP_CC_MDSS_DP_LINK1_CBCR), HWIO_PHYS(DISP_CC_MDSS_CORE_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 34,
    },
  },
  {
    .szName    = "disp_cc_mdss_dp_link1_intf_clk",
    .pDomain   = &ClockDomain_DISPCC_DISPCCMDSSDPLINK1,
    .pDebugMux = &ClockDebugMux_DISPCC,
    .nUniqueId = 0x09084F6C,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(DISP_CC_MDSS_DP_LINK1_INTF_CBCR), HWIO_PHYS(DISP_CC_MDSS_CORE_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 35,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_DISPCC_DISPCCMDSSDPLINK                                   */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "disp_cc_mdss_dp_link_clk",
    .pDomain   = &ClockDomain_DISPCC_DISPCCMDSSDPLINK,
    .pDebugMux = &ClockDebugMux_DISPCC,
    .nUniqueId = 0x106D59FD,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(DISP_CC_MDSS_DP_LINK_CBCR), HWIO_PHYS(DISP_CC_MDSS_CORE_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 27,
    },
  },
  {
    .szName    = "disp_cc_mdss_dp_link_intf_clk",
    .pDomain   = &ClockDomain_DISPCC_DISPCCMDSSDPLINK,
    .pDebugMux = &ClockDebugMux_DISPCC,
    .nUniqueId = 0x472D1CE2,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(DISP_CC_MDSS_DP_LINK_INTF_CBCR), HWIO_PHYS(DISP_CC_MDSS_CORE_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 28,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_DISPCC_DISPCCMDSSDPPIXEL1                                 */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "disp_cc_mdss_dp_pixel1_clk",
    .pDomain   = &ClockDomain_DISPCC_DISPCCMDSSDPPIXEL1,
    .pDebugMux = &ClockDebugMux_DISPCC,
    .nUniqueId = 0xD7D74325,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(DISP_CC_MDSS_DP_PIXEL1_CBCR), HWIO_PHYS(DISP_CC_MDSS_CORE_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 31,
    },
  },
  {
    .szName    = "disp_cc_mdss_spdm_dp_pixel1_clk",
    .pDomain   = &ClockDomain_DISPCC_DISPCCMDSSDPPIXEL1,
    .pDebugMux = &ClockDebugMux_DISPCC,
    .nUniqueId = 0x24157A60,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(DISP_CC_MDSS_SPDM_DP_PIXEL1_CBCR), HWIO_PHYS(DISP_CC_MDSS_SPDM_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 53,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_DISPCC_DISPCCMDSSDPPIXEL2                                 */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "disp_cc_mdss_dp_pixel2_clk",
    .pDomain   = &ClockDomain_DISPCC_DISPCCMDSSDPPIXEL2,
    .pDebugMux = &ClockDebugMux_DISPCC,
    .nUniqueId = 0x6FCE9EED,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(DISP_CC_MDSS_DP_PIXEL2_CBCR), HWIO_PHYS(DISP_CC_MDSS_CORE_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 33,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_DISPCC_DISPCCMDSSDPPIXEL                                  */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "disp_cc_mdss_dp_pixel_clk",
    .pDomain   = &ClockDomain_DISPCC_DISPCCMDSSDPPIXEL,
    .pDebugMux = &ClockDebugMux_DISPCC,
    .nUniqueId = 0xD5B7D006,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(DISP_CC_MDSS_DP_PIXEL_CBCR), HWIO_PHYS(DISP_CC_MDSS_CORE_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 30,
    },
  },
  {
    .szName    = "disp_cc_mdss_spdm_dp_pixel_clk",
    .pDomain   = &ClockDomain_DISPCC_DISPCCMDSSDPPIXEL,
    .pDebugMux = &ClockDebugMux_DISPCC,
    .nUniqueId = 0x72B57E57,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(DISP_CC_MDSS_SPDM_DP_PIXEL_CBCR), HWIO_PHYS(DISP_CC_MDSS_SPDM_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 52,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_DISPCC_DISPCCMDSSEDPAUX                                   */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "disp_cc_mdss_edp_aux_clk",
    .pDomain   = &ClockDomain_DISPCC_DISPCCMDSSEDPAUX,
    .pDebugMux = &ClockDebugMux_DISPCC,
    .nUniqueId = 0x8E6E1E09,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(DISP_CC_MDSS_EDP_AUX_CBCR), HWIO_PHYS(DISP_CC_MDSS_CORE_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 41,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_DISPCC_DISPCCMDSSEDPGTC                                   */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "disp_cc_mdss_edp_gtc_clk",
    .pDomain   = &ClockDomain_DISPCC_DISPCCMDSSEDPGTC,
    .pDebugMux = &ClockDebugMux_DISPCC,
    .nUniqueId = 0xCA650E99,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(DISP_CC_MDSS_EDP_GTC_CBCR), HWIO_PHYS(DISP_CC_MDSS_CORE_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 42,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_DISPCC_DISPCCMDSSEDPLINK                                  */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "disp_cc_mdss_edp_link_clk",
    .pDomain   = &ClockDomain_DISPCC_DISPCCMDSSEDPLINK,
    .pDebugMux = &ClockDebugMux_DISPCC,
    .nUniqueId = 0x19A44D5A,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(DISP_CC_MDSS_EDP_LINK_CBCR), HWIO_PHYS(DISP_CC_MDSS_CORE_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 39,
    },
  },
  {
    .szName    = "disp_cc_mdss_edp_link_intf_clk",
    .pDomain   = &ClockDomain_DISPCC_DISPCCMDSSEDPLINK,
    .pDebugMux = &ClockDebugMux_DISPCC,
    .nUniqueId = 0xF5E33D4F,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(DISP_CC_MDSS_EDP_LINK_INTF_CBCR), HWIO_PHYS(DISP_CC_MDSS_CORE_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 40,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_DISPCC_DISPCCMDSSEDPPIXEL                                 */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "disp_cc_mdss_edp_pixel_clk",
    .pDomain   = &ClockDomain_DISPCC_DISPCCMDSSEDPPIXEL,
    .pDebugMux = &ClockDebugMux_DISPCC,
    .nUniqueId = 0xA86C1D8D,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(DISP_CC_MDSS_EDP_PIXEL_CBCR), HWIO_PHYS(DISP_CC_MDSS_CORE_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 38,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_DISPCC_DISPCCMDSSESC0                                     */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "disp_cc_mdss_esc0_clk",
    .pDomain   = &ClockDomain_DISPCC_DISPCCMDSSESC0,
    .pDebugMux = &ClockDebugMux_DISPCC,
    .nUniqueId = 0x4485FD1B,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(DISP_CC_MDSS_ESC0_CBCR), HWIO_PHYS(DISP_CC_MDSS_CORE_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 25,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_DISPCC_DISPCCMDSSESC1                                     */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "disp_cc_mdss_esc1_clk",
    .pDomain   = &ClockDomain_DISPCC_DISPCCMDSSESC1,
    .pDebugMux = &ClockDebugMux_DISPCC,
    .nUniqueId = 0x27702EBC,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(DISP_CC_MDSS_ESC1_CBCR), HWIO_PHYS(DISP_CC_MDSS_CORE_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 26,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_DISPCC_DISPCCMDSSMDP                                      */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "disp_cc_mdss_mdp_clk",
    .pDomain   = &ClockDomain_DISPCC_DISPCCMDSSMDP,
    .pDebugMux = &ClockDebugMux_DISPCC,
    .nUniqueId = 0x6A10EA96,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(DISP_CC_MDSS_MDP_CBCR), HWIO_PHYS(DISP_CC_MDSS_CORE_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 17,
    },
  },
  {
    .szName    = "disp_cc_mdss_mdp_lut_clk",
    .pDomain   = &ClockDomain_DISPCC_DISPCCMDSSMDP,
    .pDebugMux = &ClockDebugMux_DISPCC,
    .nUniqueId = 0xF02E7C9A,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(DISP_CC_MDSS_MDP_LUT_CBCR), HWIO_PHYS(DISP_CC_MDSS_CORE_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 19,
    },
  },
  {
    .szName    = "disp_cc_mdss_spdm_mdp_clk",
    .pDomain   = &ClockDomain_DISPCC_DISPCCMDSSMDP,
    .pDebugMux = &ClockDebugMux_DISPCC,
    .nUniqueId = 0x8D7A3901,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(DISP_CC_MDSS_SPDM_MDP_CBCR), HWIO_PHYS(DISP_CC_MDSS_SPDM_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 49,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_DISPCC_DISPCCMDSSPCLK0                                    */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "disp_cc_mdss_pclk0_clk",
    .pDomain   = &ClockDomain_DISPCC_DISPCCMDSSPCLK0,
    .pDebugMux = &ClockDebugMux_DISPCC,
    .nUniqueId = 0xDF2F28DF,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(DISP_CC_MDSS_PCLK0_CBCR), HWIO_PHYS(DISP_CC_MDSS_CORE_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 15,
    },
  },
  {
    .szName    = "disp_cc_mdss_spdm_pclk0_clk",
    .pDomain   = &ClockDomain_DISPCC_DISPCCMDSSPCLK0,
    .pDebugMux = &ClockDebugMux_DISPCC,
    .nUniqueId = 0x757CD20E,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(DISP_CC_MDSS_SPDM_PCLK0_CBCR), HWIO_PHYS(DISP_CC_MDSS_SPDM_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 47,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_DISPCC_DISPCCMDSSPCLK1                                    */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "disp_cc_mdss_pclk1_clk",
    .pDomain   = &ClockDomain_DISPCC_DISPCCMDSSPCLK1,
    .pDebugMux = &ClockDebugMux_DISPCC,
    .nUniqueId = 0xEC01E1B5,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(DISP_CC_MDSS_PCLK1_CBCR), HWIO_PHYS(DISP_CC_MDSS_CORE_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 16,
    },
  },
  {
    .szName    = "disp_cc_mdss_spdm_pclk1_clk",
    .pDomain   = &ClockDomain_DISPCC_DISPCCMDSSPCLK1,
    .pDebugMux = &ClockDebugMux_DISPCC,
    .nUniqueId = 0x3C6FD75A,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(DISP_CC_MDSS_SPDM_PCLK1_CBCR), HWIO_PHYS(DISP_CC_MDSS_SPDM_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 48,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_DISPCC_DISPCCMDSSROT                                      */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "disp_cc_mdss_rot_clk",
    .pDomain   = &ClockDomain_DISPCC_DISPCCMDSSROT,
    .pDebugMux = &ClockDebugMux_DISPCC,
    .nUniqueId = 0xE5657DD4,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(DISP_CC_MDSS_ROT_CBCR), HWIO_PHYS(DISP_CC_MDSS_CORE_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 18,
    },
  },
  {
    .szName    = "disp_cc_mdss_spdm_rot_clk",
    .pDomain   = &ClockDomain_DISPCC_DISPCCMDSSROT,
    .pDebugMux = &ClockDebugMux_DISPCC,
    .nUniqueId = 0x9F34B547,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(DISP_CC_MDSS_SPDM_ROT_CBCR), HWIO_PHYS(DISP_CC_MDSS_SPDM_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 50,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_DISPCC_DISPCCMDSSVSYNC                                    */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "disp_cc_mdss_rscc_vsync_clk",
    .pDomain   = &ClockDomain_DISPCC_DISPCCMDSSVSYNC,
    .pDebugMux = &ClockDebugMux_DISPCC,
    .nUniqueId = 0x47CD7D13,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(DISP_CC_MDSS_RSCC_VSYNC_CBCR), HWIO_PHYS(DISP_CC_MDSS_RSCC_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 45,
    },
  },
  {
    .szName    = "disp_cc_mdss_vsync_clk",
    .pDomain   = &ClockDomain_DISPCC_DISPCCMDSSVSYNC,
    .pDebugMux = &ClockDebugMux_DISPCC,
    .nUniqueId = 0x80004355,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(DISP_CC_MDSS_VSYNC_CBCR), HWIO_PHYS(DISP_CC_MDSS_CORE_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 20,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_DISPCC_DISPCCPLLTESTMUX                                   */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "disp_cc_pll_test_clk",
    .pDomain   = &ClockDomain_DISPCC_DISPCCPLLTESTMUX,
    .pDebugMux = NULL,
    .nUniqueId = 0x7D7A63ED,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(DISP_CC_PLL_TEST_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = HAL_CLK_NO_DEBUG_MUX_SEL,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_DISPCC_DISPCCSLEEP                                        */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "disp_cc_sleep_clk",
    .pDomain   = &ClockDomain_DISPCC_DISPCCSLEEP,
    .pDebugMux = &ClockDebugMux_DISPCC,
    .nUniqueId = 0xE16C7ED9,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(DISP_CC_SLEEP_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 55,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_DISPCC_DISPCCXO                                           */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "disp_cc_xo_clk",
    .pDomain   = &ClockDomain_DISPCC_DISPCCXO,
    .pDebugMux = &ClockDebugMux_DISPCC,
    .nUniqueId = 0x9C4A81EA,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(DISP_CC_XO_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 54,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_CLOCKDRIVERGENXO                                      */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_cxo2_tx1_clkref_en1",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENXO,
    .pDebugMux = NULL,
    .nUniqueId = 0xD8E8FAEC,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_CXO2_TX1_CLKREF_EN1), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = HAL_CLK_NO_DEBUG_MUX_SEL,
    },
  },
  {
    .szName    = "gcc_gpll0_ufs_card_2_rx_symbol_0_clk",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENXO,
    .pDebugMux = NULL,
    .nUniqueId = 0xCE80B787,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_GPLL0_UFS_CARD_2_RX_SYMBOL_0_ACGCR), HWIO_PHYS(GCC_UFS_CARD_2_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = HAL_CLK_NO_DEBUG_MUX_SEL,
    },
  },
  {
    .szName    = "gcc_gpll0_ufs_card_2_rx_symbol_1_clk",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENXO,
    .pDebugMux = NULL,
    .nUniqueId = 0xC91A1D2A,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_GPLL0_UFS_CARD_2_RX_SYMBOL_1_ACGCR), HWIO_PHYS(GCC_UFS_CARD_2_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = HAL_CLK_NO_DEBUG_MUX_SEL,
    },
  },
  {
    .szName    = "gcc_gpll0_ufs_card_2_tx_symbol_0_clk",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENXO,
    .pDebugMux = NULL,
    .nUniqueId = 0xA6B8715A,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_GPLL0_UFS_CARD_2_TX_SYMBOL_0_ACGCR), HWIO_PHYS(GCC_UFS_CARD_2_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = HAL_CLK_NO_DEBUG_MUX_SEL,
    },
  },
  {
    .szName    = "gcc_gpll0_ufs_card_rx_symbol_0_clk",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENXO,
    .pDebugMux = NULL,
    .nUniqueId = 0x1D4B5B56,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_GPLL0_UFS_CARD_RX_SYMBOL_0_ACGCR), HWIO_PHYS(GCC_UFS_CARD_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = HAL_CLK_NO_DEBUG_MUX_SEL,
    },
  },
  {
    .szName    = "gcc_gpll0_ufs_card_rx_symbol_1_clk",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENXO,
    .pDebugMux = NULL,
    .nUniqueId = 0x62251476,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_GPLL0_UFS_CARD_RX_SYMBOL_1_ACGCR), HWIO_PHYS(GCC_UFS_CARD_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = HAL_CLK_NO_DEBUG_MUX_SEL,
    },
  },
  {
    .szName    = "gcc_gpll0_ufs_card_tx_symbol_0_clk",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENXO,
    .pDebugMux = NULL,
    .nUniqueId = 0x3FF50AC2,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_GPLL0_UFS_CARD_TX_SYMBOL_0_ACGCR), HWIO_PHYS(GCC_UFS_CARD_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = HAL_CLK_NO_DEBUG_MUX_SEL,
    },
  },
  {
    .szName    = "gcc_gpll0_ufs_phy_rx_symbol_0_clk",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENXO,
    .pDebugMux = NULL,
    .nUniqueId = 0x106963AD,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_GPLL0_UFS_PHY_RX_SYMBOL_0_ACGCR), HWIO_PHYS(GCC_UFS_PHY_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = HAL_CLK_NO_DEBUG_MUX_SEL,
    },
  },
  {
    .szName    = "gcc_gpll0_ufs_phy_rx_symbol_1_clk",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENXO,
    .pDebugMux = NULL,
    .nUniqueId = 0x9A09B4DA,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_GPLL0_UFS_PHY_RX_SYMBOL_1_ACGCR), HWIO_PHYS(GCC_UFS_PHY_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = HAL_CLK_NO_DEBUG_MUX_SEL,
    },
  },
  {
    .szName    = "gcc_gpll0_ufs_phy_tx_symbol_0_clk",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENXO,
    .pDebugMux = NULL,
    .nUniqueId = 0x098D4A5E,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_GPLL0_UFS_PHY_TX_SYMBOL_0_ACGCR), HWIO_PHYS(GCC_UFS_PHY_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = HAL_CLK_NO_DEBUG_MUX_SEL,
    },
  },
  {
    .szName    = "gcc_gpu_gpll0_clk_src",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENXO,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x4B1A4AC7,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { 0, HWIO_PHYS(GCC_GPU_BCR), {HWIO_PHYS(GCC_PROC_CLK_BRANCH_ENA_VOTE), 0x8000} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 328,
    },
  },
  {
    .szName    = "gcc_gpu_gpll0_div_clk_src",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENXO,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x6EEEF079,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { 0, HWIO_PHYS(GCC_GPU_BCR), {HWIO_PHYS(GCC_PROC_CLK_BRANCH_ENA_VOTE), 0x10000} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 329,
    },
  },
  {
    .szName    = "gcc_hlos1_vote_aggre_noc_mmu_audio_tbu_clk",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENXO,
    .pDebugMux = NULL,
    .nUniqueId = 0x163AA536,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_HLOS1_VOTE_AGGRE_NOC_MMU_AUDIO_TBU_CLK), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = HAL_CLK_NO_DEBUG_MUX_SEL,
    },
  },
  {
    .szName    = "gcc_hlos1_vote_aggre_noc_mmu_pcie_tbu_clk",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENXO,
    .pDebugMux = NULL,
    .nUniqueId = 0xB541E243,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_HLOS1_VOTE_AGGRE_NOC_MMU_PCIE_TBU_CLK), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = HAL_CLK_NO_DEBUG_MUX_SEL,
    },
  },
  {
    .szName    = "gcc_hlos1_vote_aggre_noc_mmu_tbu1_clk",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENXO,
    .pDebugMux = NULL,
    .nUniqueId = 0x73963888,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_HLOS1_VOTE_AGGRE_NOC_MMU_TBU1_CLK), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = HAL_CLK_NO_DEBUG_MUX_SEL,
    },
  },
  {
    .szName    = "gcc_hlos1_vote_aggre_noc_mmu_tbu2_clk",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENXO,
    .pDebugMux = NULL,
    .nUniqueId = 0x78CD70C0,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_HLOS1_VOTE_AGGRE_NOC_MMU_TBU2_CLK), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = HAL_CLK_NO_DEBUG_MUX_SEL,
    },
  },
  {
    .szName    = "gcc_hlos1_vote_all_smmu_mmu_clk",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENXO,
    .pDebugMux = NULL,
    .nUniqueId = 0xF26FB3CE,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_HLOS1_VOTE_ALL_SMMU_MMU_CLK), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = HAL_CLK_NO_DEBUG_MUX_SEL,
    },
  },
  {
    .szName    = "gcc_hlos1_vote_gpu_smmu_clk",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENXO,
    .pDebugMux = NULL,
    .nUniqueId = 0x4A750D50,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_HLOS1_VOTE_GPU_SMMU_CLK), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = HAL_CLK_NO_DEBUG_MUX_SEL,
    },
  },
  {
    .szName    = "gcc_hlos1_vote_mmnoc_mmu_tbu_hf0_clk",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENXO,
    .pDebugMux = NULL,
    .nUniqueId = 0x5A23B219,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_HLOS1_VOTE_MMNOC_MMU_TBU_HF0_CLK), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = HAL_CLK_NO_DEBUG_MUX_SEL,
    },
  },
  {
    .szName    = "gcc_hlos1_vote_mmnoc_mmu_tbu_hf1_clk",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENXO,
    .pDebugMux = NULL,
    .nUniqueId = 0xC690F22F,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_HLOS1_VOTE_MMNOC_MMU_TBU_HF1_CLK), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = HAL_CLK_NO_DEBUG_MUX_SEL,
    },
  },
  {
    .szName    = "gcc_hlos1_vote_mmnoc_mmu_tbu_sf_clk",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENXO,
    .pDebugMux = NULL,
    .nUniqueId = 0xF9235537,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_HLOS1_VOTE_MMNOC_MMU_TBU_SF_CLK), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = HAL_CLK_NO_DEBUG_MUX_SEL,
    },
  },
  {
    .szName    = "gcc_hlos1_vote_mmu_tcu_clk",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENXO,
    .pDebugMux = NULL,
    .nUniqueId = 0x45B7596F,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_HLOS1_VOTE_MMU_TCU_CLK), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = HAL_CLK_NO_DEBUG_MUX_SEL,
    },
  },
  {
    .szName    = "gcc_hlos1_vote_turing_mmu_tbu0_clk",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENXO,
    .pDebugMux = NULL,
    .nUniqueId = 0xEAAAE7E3,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_HLOS1_VOTE_TURING_MMU_TBU0_CLK), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = HAL_CLK_NO_DEBUG_MUX_SEL,
    },
  },
  {
    .szName    = "gcc_hlos1_vote_turing_mmu_tbu1_clk",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENXO,
    .pDebugMux = NULL,
    .nUniqueId = 0x8A6A6C38,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_HLOS1_VOTE_TURING_MMU_TBU1_CLK), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = HAL_CLK_NO_DEBUG_MUX_SEL,
    },
  },
  {
    .szName    = "gcc_npu_gpll0_clk_src",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENXO,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0xC5B8532C,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { 0, HWIO_PHYS(GCC_NPU_BCR), {HWIO_PHYS(GCC_PROC_CLK_BRANCH_ENA_VOTE), 0x40000} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 417,
    },
  },
  {
    .szName    = "gcc_npu_gpll0_div_clk_src",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENXO,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x4EC2E3EF,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { 0, HWIO_PHYS(GCC_NPU_BCR), {HWIO_PHYS(GCC_PROC_CLK_BRANCH_ENA_VOTE), 0x80000} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 418,
    },
  },
  {
    .szName    = "gcc_pcie_0_clkref_en",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENXO,
    .pDebugMux = NULL,
    .nUniqueId = 0xA10632F8,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_PCIE_0_CLKREF_EN), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = HAL_CLK_NO_DEBUG_MUX_SEL,
    },
  },
  {
    .szName    = "gcc_pcie_1_clkref_en",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENXO,
    .pDebugMux = NULL,
    .nUniqueId = 0x0AD4DDE2,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_PCIE_1_CLKREF_EN), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = HAL_CLK_NO_DEBUG_MUX_SEL,
    },
  },
  {
    .szName    = "gcc_rx1_usb2_clkref_en",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENXO,
    .pDebugMux = NULL,
    .nUniqueId = 0xD0F0D96C,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_RX1_USB2_CLKREF_EN), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = HAL_CLK_NO_DEBUG_MUX_SEL,
    },
  },
  {
    .szName    = "gcc_rx2_qlink_clkref_en",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENXO,
    .pDebugMux = NULL,
    .nUniqueId = 0x5648B23F,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_RX2_QLINK_CLKREF_EN), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = HAL_CLK_NO_DEBUG_MUX_SEL,
    },
  },
  {
    .szName    = "gcc_rx3_clkref_en",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENXO,
    .pDebugMux = NULL,
    .nUniqueId = 0x22221302,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_RX3_CLKREF_EN), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = HAL_CLK_NO_DEBUG_MUX_SEL,
    },
  },
  {
    .szName    = "gcc_ufs_card_clkref_en",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENXO,
    .pDebugMux = NULL,
    .nUniqueId = 0xA9F0C670,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_UFS_CARD_CLKREF_EN), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = HAL_CLK_NO_DEBUG_MUX_SEL,
    },
  },
  {
    .szName    = "gcc_ufs_mem_clkref_en",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENXO,
    .pDebugMux = NULL,
    .nUniqueId = 0xEEC060E2,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_UFS_MEM_CLKREF_EN), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = HAL_CLK_NO_DEBUG_MUX_SEL,
    },
  },
  {
    .szName    = "gcc_usb3_prim_clkref_en",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENXO,
    .pDebugMux = NULL,
    .nUniqueId = 0x4FEDA921,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_USB3_PRIM_CLKREF_EN), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = HAL_CLK_NO_DEBUG_MUX_SEL,
    },
  },
  {
    .szName    = "gcc_usb3_sec_clkref_en",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENXO,
    .pDebugMux = NULL,
    .nUniqueId = 0x38F2AA56,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_USB3_SEC_CLKREF_EN), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = HAL_CLK_NO_DEBUG_MUX_SEL,
    },
  },
  {
    .szName    = "gpu_cc_acd_cxo_clk",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENXO,
    .pDebugMux = &ClockDebugMux_GPUCC,
    .nUniqueId = 0xFB4F06FB,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GPUCC_GPU_CC_ACD_CXO_CBCR), HWIO_PHYS(GPUCC_GPU_CC_ACD_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 31,
    },
  },
  {
    .szName    = "gpu_cc_cx_apb_clk",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENXO,
    .pDebugMux = &ClockDebugMux_GPUCC,
    .nUniqueId = 0x6E5557EC,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GPUCC_GPU_CC_CX_APB_CBCR), HWIO_PHYS(GPUCC_GPU_CC_CX_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 20,
    },
  },
  {
    .szName    = "gpu_cc_cx_qdss_tsctr_clk",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENXO,
    .pDebugMux = &ClockDebugMux_GPUCC,
    .nUniqueId = 0x403CC85F,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GPUCC_GPU_CC_CX_QDSS_TSCTR_CBCR), HWIO_PHYS(GPUCC_GPU_CC_CX_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 19,
    },
  },
  {
    .szName    = "gpu_cc_cxo_aon_clk",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENXO,
    .pDebugMux = &ClockDebugMux_GPUCC,
    .nUniqueId = 0xA006B60F,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GPUCC_GPU_CC_CXO_AON_CBCR), HWIO_PHYS(GPUCC_GPU_CC_XO_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 10,
    },
  },
  {
    .szName    = "gpu_cc_cxo_clk",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENXO,
    .pDebugMux = &ClockDebugMux_GPUCC,
    .nUniqueId = 0x8DFA73CF,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GPUCC_GPU_CC_CXO_CBCR), HWIO_PHYS(GPUCC_GPU_CC_CX_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 25,
    },
  },
  {
    .szName    = "gpu_cc_gx_cxo_clk",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENXO,
    .pDebugMux = &ClockDebugMux_GPUCC,
    .nUniqueId = 0x28DEB84B,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GPUCC_GPU_CC_GX_CXO_CBCR), HWIO_PHYS(GPUCC_GPU_CC_GX_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 14,
    },
  },
  {
    .szName    = "gpu_cc_gx_qdss_tsctr_clk",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENXO,
    .pDebugMux = &ClockDebugMux_GPUCC,
    .nUniqueId = 0x39A96FAC,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GPUCC_GPU_CC_GX_QDSS_TSCTR_CBCR), HWIO_PHYS(GPUCC_GPU_CC_GX_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 13,
    },
  },
  {
    .szName    = "npu_cc_bto_core_clk",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENXO,
    .pDebugMux = &ClockDebugMux_NPUCC,
    .nUniqueId = 0xF9573CD0,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(NPU_CC_BTO_CORE_CBCR), HWIO_PHYS(NPU_CC_NPU_CORE_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 18,
    },
  },
  {
    .szName    = "npu_cc_bwmon_clk",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENXO,
    .pDebugMux = &ClockDebugMux_NPUCC,
    .nUniqueId = 0x32788CDD,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(NPU_CC_BWMON_CBCR), HWIO_PHYS(NPU_CC_NPU_CORE_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 15,
    },
  },
  {
    .szName    = "npu_cc_npu_core_apb_clk",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENXO,
    .pDebugMux = &ClockDebugMux_NPUCC,
    .nUniqueId = 0xE11E5C68,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(NPU_CC_NPU_CORE_APB_CBCR), HWIO_PHYS(NPU_CC_NPU_CORE_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 14,
    },
  },
  {
    .szName    = "npu_cc_npu_cpc_timer_clk",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENXO,
    .pDebugMux = &ClockDebugMux_NPUCC,
    .nUniqueId = 0x8FE0FF96,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(NPU_CC_NPU_CPC_TIMER_CBCR), HWIO_PHYS(NPU_CC_NPU_CORE_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 5,
    },
  },
  {
    .szName    = "npu_cc_qtimer_core_clk",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENXO,
    .pDebugMux = &ClockDebugMux_NPUCC,
    .nUniqueId = 0xEB124B15,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(NPU_CC_QTIMER_CORE_CBCR), HWIO_PHYS(NPU_CC_NPU_CORE_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 6,
    },
  },
  {
    .szName    = "npu_cc_xo_clk",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENXO,
    .pDebugMux = &ClockDebugMux_NPUCC,
    .nUniqueId = 0xFF623F47,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(NPU_CC_XO_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 17,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_CLOCKDRIVERGENNOSOURCE                                */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "apcs_gold_post_acd_clk",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENNOSOURCE,
    .pDebugMux = &ClockDebugMux_APSSCC,
    .nUniqueId = 0xE8494F7E,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { 0, 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 37,
    },
  },
  {
    .szName    = "apcs_l3_post_acd_clk",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENNOSOURCE,
    .pDebugMux = &ClockDebugMux_APSSCC,
    .nUniqueId = 0x4D6B4B8D,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { 0, 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 65,
    },
  },
  {
    .szName    = "apcs_silver_post_acd_clk",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENNOSOURCE,
    .pDebugMux = &ClockDebugMux_APSSCC,
    .nUniqueId = 0xBC40F245,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { 0, 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 33,
    },
  },
  {
    .szName    = "disp_cc_debug_clk",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENNOSOURCE,
    .pDebugMux = NULL,
    .nUniqueId = 0x17181EB2,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(DISP_CC_DEBUG_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = HAL_CLK_NO_DEBUG_MUX_SEL,
    },
  },
  {
    .szName    = "disp_cc_mdss_spdm_debug_clk",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENNOSOURCE,
    .pDebugMux = NULL,
    .nUniqueId = 0x1B862752,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(DISP_CC_MDSS_SPDM_DEBUG_CBCR), HWIO_PHYS(DISP_CC_MDSS_SPDM_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = HAL_CLK_NO_DEBUG_MUX_SEL,
    },
  },
  {
    .szName    = "gcc_pcie_0_link_down_bcr",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENNOSOURCE,
    .pDebugMux = NULL,
    .nUniqueId = 0xB6C4C692,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { 0, HWIO_PHYS(GCC_PCIE_0_LINK_DOWN_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = HAL_CLK_NO_DEBUG_MUX_SEL,
    },
  },
  {
    .szName    = "gcc_pcie_0_nocsr_com_phy_bcr",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENNOSOURCE,
    .pDebugMux = NULL,
    .nUniqueId = 0xD13C8283,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { 0, HWIO_PHYS(GCC_PCIE_0_NOCSR_COM_PHY_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = HAL_CLK_NO_DEBUG_MUX_SEL,
    },
  },
  {
    .szName    = "gcc_pcie_0_phy_bcr",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENNOSOURCE,
    .pDebugMux = NULL,
    .nUniqueId = 0x822A9820,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { 0, HWIO_PHYS(GCC_PCIE_0_PHY_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = HAL_CLK_NO_DEBUG_MUX_SEL,
    },
  },
  {
    .szName    = "gcc_pcie_0_phy_nocsr_com_phy_bcr",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENNOSOURCE,
    .pDebugMux = NULL,
    .nUniqueId = 0xA7917945,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { 0, HWIO_PHYS(GCC_PCIE_0_PHY_NOCSR_COM_PHY_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = HAL_CLK_NO_DEBUG_MUX_SEL,
    },
  },
  {
    .szName    = "gcc_pcie_1_link_down_bcr",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENNOSOURCE,
    .pDebugMux = NULL,
    .nUniqueId = 0xAB0C0523,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { 0, HWIO_PHYS(GCC_PCIE_1_LINK_DOWN_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = HAL_CLK_NO_DEBUG_MUX_SEL,
    },
  },
  {
    .szName    = "gcc_pcie_1_nocsr_com_phy_bcr",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENNOSOURCE,
    .pDebugMux = NULL,
    .nUniqueId = 0x7D99D6D6,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { 0, HWIO_PHYS(GCC_PCIE_1_NOCSR_COM_PHY_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = HAL_CLK_NO_DEBUG_MUX_SEL,
    },
  },
  {
    .szName    = "gcc_pcie_1_phy_bcr",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENNOSOURCE,
    .pDebugMux = NULL,
    .nUniqueId = 0x778DB555,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { 0, HWIO_PHYS(GCC_PCIE_1_PHY_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = HAL_CLK_NO_DEBUG_MUX_SEL,
    },
  },
  {
    .szName    = "gcc_pcie_1_phy_nocsr_com_phy_bcr",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENNOSOURCE,
    .pDebugMux = NULL,
    .nUniqueId = 0x7E512A4B,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { 0, HWIO_PHYS(GCC_PCIE_1_PHY_NOCSR_COM_PHY_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = HAL_CLK_NO_DEBUG_MUX_SEL,
    },
  },
  {
    .szName    = "gcc_pcie_2_link_down_bcr",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENNOSOURCE,
    .pDebugMux = NULL,
    .nUniqueId = 0xB9EED8AD,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { 0, HWIO_PHYS(GCC_PCIE_2_LINK_DOWN_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = HAL_CLK_NO_DEBUG_MUX_SEL,
    },
  },
  {
    .szName    = "gcc_pcie_2_nocsr_com_phy_bcr",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENNOSOURCE,
    .pDebugMux = NULL,
    .nUniqueId = 0xF0D7C339,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { 0, HWIO_PHYS(GCC_PCIE_2_NOCSR_COM_PHY_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = HAL_CLK_NO_DEBUG_MUX_SEL,
    },
  },
  {
    .szName    = "gcc_pcie_2_phy_bcr",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENNOSOURCE,
    .pDebugMux = NULL,
    .nUniqueId = 0xCFD1A5AE,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { 0, HWIO_PHYS(GCC_PCIE_2_PHY_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = HAL_CLK_NO_DEBUG_MUX_SEL,
    },
  },
  {
    .szName    = "gcc_pcie_2_phy_nocsr_com_phy_bcr",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENNOSOURCE,
    .pDebugMux = NULL,
    .nUniqueId = 0x6A8AD9DC,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { 0, HWIO_PHYS(GCC_PCIE_2_PHY_NOCSR_COM_PHY_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = HAL_CLK_NO_DEBUG_MUX_SEL,
    },
  },
  {
    .szName    = "gcc_pcie_3_link_down_bcr",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENNOSOURCE,
    .pDebugMux = NULL,
    .nUniqueId = 0xCD5BCA7F,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { 0, HWIO_PHYS(GCC_PCIE_3_LINK_DOWN_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = HAL_CLK_NO_DEBUG_MUX_SEL,
    },
  },
  {
    .szName    = "gcc_pcie_3_nocsr_com_phy_bcr",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENNOSOURCE,
    .pDebugMux = NULL,
    .nUniqueId = 0xA1A5DAF2,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { 0, HWIO_PHYS(GCC_PCIE_3_NOCSR_COM_PHY_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = HAL_CLK_NO_DEBUG_MUX_SEL,
    },
  },
  {
    .szName    = "gcc_pcie_3_phy_bcr",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENNOSOURCE,
    .pDebugMux = NULL,
    .nUniqueId = 0xD9A7DFCA,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { 0, HWIO_PHYS(GCC_PCIE_3_PHY_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = HAL_CLK_NO_DEBUG_MUX_SEL,
    },
  },
  {
    .szName    = "gcc_pcie_3_phy_nocsr_com_phy_bcr",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENNOSOURCE,
    .pDebugMux = NULL,
    .nUniqueId = 0x60649B3D,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { 0, HWIO_PHYS(GCC_PCIE_3_PHY_NOCSR_COM_PHY_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = HAL_CLK_NO_DEBUG_MUX_SEL,
    },
  },
  {
    .szName    = "gcc_pcie_phy_cfg_ahb_bcr",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENNOSOURCE,
    .pDebugMux = NULL,
    .nUniqueId = 0x5FB6DA8B,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { 0, HWIO_PHYS(GCC_PCIE_PHY_CFG_AHB_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = HAL_CLK_NO_DEBUG_MUX_SEL,
    },
  },
  {
    .szName    = "gcc_pcie_phy_com_bcr",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENNOSOURCE,
    .pDebugMux = NULL,
    .nUniqueId = 0x20D7AA6B,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { 0, HWIO_PHYS(GCC_PCIE_PHY_COM_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = HAL_CLK_NO_DEBUG_MUX_SEL,
    },
  },
  {
    .szName    = "gcc_qusb2phy_5_bcr",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENNOSOURCE,
    .pDebugMux = NULL,
    .nUniqueId = 0x1D426099,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { 0, HWIO_PHYS(GCC_QUSB2PHY_5_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = HAL_CLK_NO_DEBUG_MUX_SEL,
    },
  },
  {
    .szName    = "gcc_qusb2phy_mp0_bcr",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENNOSOURCE,
    .pDebugMux = NULL,
    .nUniqueId = 0x6B1E6589,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { 0, HWIO_PHYS(GCC_QUSB2PHY_MP0_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = HAL_CLK_NO_DEBUG_MUX_SEL,
    },
  },
  {
    .szName    = "gcc_qusb2phy_mp1_bcr",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENNOSOURCE,
    .pDebugMux = NULL,
    .nUniqueId = 0xDEB15F57,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { 0, HWIO_PHYS(GCC_QUSB2PHY_MP1_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = HAL_CLK_NO_DEBUG_MUX_SEL,
    },
  },
  {
    .szName    = "gcc_qusb2phy_prim_bcr",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENNOSOURCE,
    .pDebugMux = NULL,
    .nUniqueId = 0x9E2329D1,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { 0, HWIO_PHYS(GCC_QUSB2PHY_PRIM_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = HAL_CLK_NO_DEBUG_MUX_SEL,
    },
  },
  {
    .szName    = "gcc_qusb2phy_sec_bcr",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENNOSOURCE,
    .pDebugMux = NULL,
    .nUniqueId = 0x7D3CC967,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { 0, HWIO_PHYS(GCC_QUSB2PHY_SEC_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = HAL_CLK_NO_DEBUG_MUX_SEL,
    },
  },
  {
    .szName    = "gcc_usb3_dp_phy_prim_sp0_bcr",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENNOSOURCE,
    .pDebugMux = NULL,
    .nUniqueId = 0x7D23EDA5,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { 0, HWIO_PHYS(GCC_USB3_DP_PHY_PRIM_SP0_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = HAL_CLK_NO_DEBUG_MUX_SEL,
    },
  },
  {
    .szName    = "gcc_usb3_dp_phy_prim_sp1_bcr",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENNOSOURCE,
    .pDebugMux = NULL,
    .nUniqueId = 0xDB484D3B,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { 0, HWIO_PHYS(GCC_USB3_DP_PHY_PRIM_SP1_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = HAL_CLK_NO_DEBUG_MUX_SEL,
    },
  },
  {
    .szName    = "gcc_usb3_dp_phy_sec_bcr",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENNOSOURCE,
    .pDebugMux = NULL,
    .nUniqueId = 0x6969477E,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { 0, HWIO_PHYS(GCC_USB3_DP_PHY_SEC_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = HAL_CLK_NO_DEBUG_MUX_SEL,
    },
  },
  {
    .szName    = "gcc_usb3_phy_prim_sp0_bcr",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENNOSOURCE,
    .pDebugMux = NULL,
    .nUniqueId = 0xE5CC27F1,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { 0, HWIO_PHYS(GCC_USB3_PHY_PRIM_SP0_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = HAL_CLK_NO_DEBUG_MUX_SEL,
    },
  },
  {
    .szName    = "gcc_usb3_phy_prim_sp1_bcr",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENNOSOURCE,
    .pDebugMux = NULL,
    .nUniqueId = 0xC5668383,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { 0, HWIO_PHYS(GCC_USB3_PHY_PRIM_SP1_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = HAL_CLK_NO_DEBUG_MUX_SEL,
    },
  },
  {
    .szName    = "gcc_usb3_phy_sec_bcr",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENNOSOURCE,
    .pDebugMux = NULL,
    .nUniqueId = 0xA8BC18B6,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { 0, HWIO_PHYS(GCC_USB3_PHY_SEC_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = HAL_CLK_NO_DEBUG_MUX_SEL,
    },
  },
  {
    .szName    = "gcc_usb3_uniphy_mp0_bcr",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENNOSOURCE,
    .pDebugMux = NULL,
    .nUniqueId = 0xD3D1C566,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { 0, HWIO_PHYS(GCC_USB3_UNIPHY_MP0_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = HAL_CLK_NO_DEBUG_MUX_SEL,
    },
  },
  {
    .szName    = "gcc_usb3_uniphy_mp1_bcr",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENNOSOURCE,
    .pDebugMux = NULL,
    .nUniqueId = 0x7C21157F,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { 0, HWIO_PHYS(GCC_USB3_UNIPHY_MP1_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = HAL_CLK_NO_DEBUG_MUX_SEL,
    },
  },
  {
    .szName    = "gcc_usb3phy_phy_prim_sp0_bcr",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENNOSOURCE,
    .pDebugMux = NULL,
    .nUniqueId = 0x0C0CC325,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { 0, HWIO_PHYS(GCC_USB3PHY_PHY_PRIM_SP0_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = HAL_CLK_NO_DEBUG_MUX_SEL,
    },
  },
  {
    .szName    = "gcc_usb3phy_phy_prim_sp1_bcr",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENNOSOURCE,
    .pDebugMux = NULL,
    .nUniqueId = 0x71CB3CE2,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { 0, HWIO_PHYS(GCC_USB3PHY_PHY_PRIM_SP1_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = HAL_CLK_NO_DEBUG_MUX_SEL,
    },
  },
  {
    .szName    = "gcc_usb3phy_phy_sec_bcr",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENNOSOURCE,
    .pDebugMux = NULL,
    .nUniqueId = 0x5ECE2A2C,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { 0, HWIO_PHYS(GCC_USB3PHY_PHY_SEC_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = HAL_CLK_NO_DEBUG_MUX_SEL,
    },
  },
  {
    .szName    = "gcc_usb3uniphy_phy_mp0_bcr",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENNOSOURCE,
    .pDebugMux = NULL,
    .nUniqueId = 0x7ED2B843,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { 0, HWIO_PHYS(GCC_USB3UNIPHY_PHY_MP0_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = HAL_CLK_NO_DEBUG_MUX_SEL,
    },
  },
  {
    .szName    = "gcc_usb3uniphy_phy_mp1_bcr",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENNOSOURCE,
    .pDebugMux = NULL,
    .nUniqueId = 0x3E6BF177,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { 0, HWIO_PHYS(GCC_USB3UNIPHY_PHY_MP1_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = HAL_CLK_NO_DEBUG_MUX_SEL,
    },
  },
  {
    .szName    = "gpu_cc_sleep_clk",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENNOSOURCE,
    .pDebugMux = &ClockDebugMux_GPUCC,
    .nUniqueId = 0xC2E1C465,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GPUCC_GPU_CC_SLEEP_CBCR), HWIO_PHYS(GPUCC_GPU_CC_CX_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 22,
    },
  },
  {
    .szName    = "npu_cc_comp_noc_axi_clk",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENNOSOURCE,
    .pDebugMux = &ClockDebugMux_NPUCC,
    .nUniqueId = 0x5D103D72,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(NPU_CC_COMP_NOC_AXI_CBCR), HWIO_PHYS(NPU_CC_NPU_CORE_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 9,
    },
  },
  {
    .szName    = "npu_cc_conf_noc_ahb_clk",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENNOSOURCE,
    .pDebugMux = &ClockDebugMux_NPUCC,
    .nUniqueId = 0x2A98F68F,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(NPU_CC_CONF_NOC_AHB_CBCR), HWIO_PHYS(NPU_CC_NPU_CORE_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 10,
    },
  },
  {
    .szName    = "npu_cc_npu_core_atb_clk",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENNOSOURCE,
    .pDebugMux = &ClockDebugMux_NPUCC,
    .nUniqueId = 0x8381AA2D,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(NPU_CC_NPU_CORE_ATB_CBCR), HWIO_PHYS(NPU_CC_NPU_CORE_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 11,
    },
  },
  {
    .szName    = "npu_cc_npu_core_cti_clk",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENNOSOURCE,
    .pDebugMux = &ClockDebugMux_NPUCC,
    .nUniqueId = 0xEB25ECA4,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(NPU_CC_NPU_CORE_CTI_CBCR), HWIO_PHYS(NPU_CC_NPU_CORE_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 12,
    },
  },
  {
    .szName    = "npu_cc_sleep_clk",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENNOSOURCE,
    .pDebugMux = &ClockDebugMux_NPUCC,
    .nUniqueId = 0x99639FCD,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(NPU_CC_SLEEP_CBCR), HWIO_PHYS(NPU_CC_NPU_CORE_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 7,
    },
  },
  {
    .szName    = "video_cc_debug_clk",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENNOSOURCE,
    .pDebugMux = NULL,
    .nUniqueId = 0x9AD7014D,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(VIDEO_CC_DEBUG_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = HAL_CLK_NO_DEBUG_MUX_SEL,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCAGGRENOC                                           */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_emac_axi_clk",
    .pDomain   = &ClockDomain_GCC_GCCAGGRENOC,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x5BAD8613,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_EMAC_AXI_CBCR), HWIO_PHYS(GCC_EMAC_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 365,
    },
  },
  {
    .szName    = "gcc_qupv3_wrap_0_m_ahb_clk",
    .pDomain   = &ClockDomain_GCC_GCCAGGRENOC,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x79AD882E,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_QUPV3_WRAP_0_M_AHB_CBCR), HWIO_PHYS(GCC_QUPV3_WRAPPER_0_BCR), {HWIO_PHYS(GCC_PROC_CLK_BRANCH_ENA_VOTE_1), 0x40} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 116,
    },
  },
  {
    .szName    = "gcc_qupv3_wrap_1_m_ahb_clk",
    .pDomain   = &ClockDomain_GCC_GCCAGGRENOC,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x728861DF,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_QUPV3_WRAP_1_M_AHB_CBCR), HWIO_PHYS(GCC_QUPV3_WRAPPER_1_BCR), {HWIO_PHYS(GCC_PROC_CLK_BRANCH_ENA_VOTE_1), 0x100000} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 128,
    },
  },
  {
    .szName    = "gcc_qupv3_wrap_2_m_ahb_clk",
    .pDomain   = &ClockDomain_GCC_GCCAGGRENOC,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0xFBEA7E74,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_QUPV3_WRAP_2_M_AHB_CBCR), HWIO_PHYS(GCC_QUPV3_WRAPPER_2_BCR), {HWIO_PHYS(GCC_PROC_CLK_BRANCH_ENA_VOTE_2), 0x4} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 405,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCAGGRENOCSOUTHSF                                    */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_aggre_noc_pcie_tbu_clk",
    .pDomain   = &ClockDomain_GCC_GCCAGGRENOCSOUTHSF,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x523BCBA7,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_AGGRE_NOC_PCIE_TBU_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 45,
    },
  },
  {
    .szName    = "gcc_pcie_1_mstr_axi_clk",
    .pDomain   = &ClockDomain_GCC_GCCAGGRENOCSOUTHSF,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0xD8A72CC2,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_PCIE_1_MSTR_AXI_CBCR), HWIO_PHYS(GCC_PCIE_1_BCR), {HWIO_PHYS(GCC_PROC_CLK_BRANCH_ENA_VOTE), 0x8000000} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 234,
    },
  },
  {
    .szName    = "gcc_pcie_2_mstr_axi_clk",
    .pDomain   = &ClockDomain_GCC_GCCAGGRENOCSOUTHSF,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0xA909F6AB,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_PCIE_2_MSTR_AXI_CBCR), HWIO_PHYS(GCC_PCIE_2_BCR), {HWIO_PHYS(GCC_PROC_CLK_BRANCH_ENA_VOTE_2), 0x1000} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 371,
    },
  },
  {
    .szName    = "gcc_pcie_3_mstr_axi_clk",
    .pDomain   = &ClockDomain_GCC_GCCAGGRENOCSOUTHSF,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x0DE22986,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_PCIE_3_MSTR_AXI_CBCR), HWIO_PHYS(GCC_PCIE_3_BCR), {HWIO_PHYS(GCC_PROC_CLK_BRANCH_ENA_VOTE_2), 0x40000} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 378,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCCNOCPERIPH                                         */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_emac_slv_ahb_clk",
    .pDomain   = &ClockDomain_GCC_GCCCNOCPERIPH,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x94B7DA8E,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_EMAC_SLV_AHB_CBCR), HWIO_PHYS(GCC_EMAC_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 364,
    },
  },
  {
    .szName    = "gcc_pdm_ahb_clk",
    .pDomain   = &ClockDomain_GCC_GCCCNOCPERIPH,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0xA6F0A072,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_PDM_AHB_CBCR), HWIO_PHYS(GCC_PDM_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 140,
    },
  },
  {
    .szName    = "gcc_qspi_1_cnoc_periph_ahb_clk",
    .pDomain   = &ClockDomain_GCC_GCCCNOCPERIPH,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x2AE5F620,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_QSPI_1_CNOC_PERIPH_AHB_CBCR), HWIO_PHYS(GCC_QSPI_1_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 367,
    },
  },
  {
    .szName    = "gcc_qspi_cnoc_periph_ahb_clk",
    .pDomain   = &ClockDomain_GCC_GCCCNOCPERIPH,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x3510FA5B,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_QSPI_CNOC_PERIPH_AHB_CBCR), HWIO_PHYS(GCC_QSPI_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 360,
    },
  },
  {
    .szName    = "gcc_sdcc2_ahb_clk",
    .pDomain   = &ClockDomain_GCC_GCCCNOCPERIPH,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0xF7AE64AF,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_SDCC2_AHB_CBCR), HWIO_PHYS(GCC_SDCC2_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 113,
    },
  },
  {
    .szName    = "gcc_sdcc4_ahb_clk",
    .pDomain   = &ClockDomain_GCC_GCCCNOCPERIPH,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0xB1EE1963,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_SDCC4_AHB_CBCR), HWIO_PHYS(GCC_SDCC4_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 115,
    },
  },
  {
    .szName    = "gcc_tsif_ahb_clk",
    .pDomain   = &ClockDomain_GCC_GCCCNOCPERIPH,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0xDF9DD523,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_TSIF_AHB_CBCR), HWIO_PHYS(GCC_TSIF_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 144,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCCONFIGNOC                                          */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_boot_rom_ahb_clk",
    .pDomain   = &ClockDomain_GCC_GCCCONFIGNOC,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x20C2EB7C,
    .nFlags    = CLOCK_FLAG_LEAVE_ALWAYS_ON |
                 CLOCK_FLAG_SUPPRESSIBLE,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_BOOT_ROM_AHB_CBCR), 0, {HWIO_PHYS(GCC_PROC_CLK_BRANCH_ENA_VOTE), 0x400} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 148,
    },
  },
  {
    .szName    = "gcc_camera_ahb_clk",
    .pDomain   = &ClockDomain_GCC_GCCCONFIGNOC,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0xCB81AAEC,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_CAMERA_AHB_CBCR), HWIO_PHYS(GCC_MMSS_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 58,
    },
  },
  {
    .szName    = "gcc_disp_ahb_clk",
    .pDomain   = &ClockDomain_GCC_GCCCONFIGNOC,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0xCC324D48,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_DISP_AHB_CBCR), HWIO_PHYS(GCC_MMSS_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 59,
    },
  },
  {
    .szName    = "gcc_gpu_cfg_ahb_clk",
    .pDomain   = &ClockDomain_GCC_GCCCONFIGNOC,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0xBDC5D904,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_GPU_CFG_AHB_CBCR), HWIO_PHYS(GCC_GPU_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 322,
    },
  },
  {
    .szName    = "gcc_npu_cfg_ahb_clk",
    .pDomain   = &ClockDomain_GCC_GCCCONFIGNOC,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x38A33585,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_NPU_CFG_AHB_CBCR), HWIO_PHYS(GCC_NPU_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 488,
    },
  },
  {
    .szName    = "gcc_pcie_0_cfg_ahb_clk",
    .pDomain   = &ClockDomain_GCC_GCCCONFIGNOC,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0xAB33D8F8,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_PCIE_0_CFG_AHB_CBCR), HWIO_PHYS(GCC_PCIE_0_BCR), {HWIO_PHYS(GCC_PROC_CLK_BRANCH_ENA_VOTE_1), 0x4} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 228,
    },
  },
  {
    .szName    = "gcc_pcie_1_cfg_ahb_clk",
    .pDomain   = &ClockDomain_GCC_GCCCONFIGNOC,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0xB6E6F09E,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_PCIE_1_CFG_AHB_CBCR), HWIO_PHYS(GCC_PCIE_1_BCR), {HWIO_PHYS(GCC_PROC_CLK_BRANCH_ENA_VOTE), 0x10000000} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 235,
    },
  },
  {
    .szName    = "gcc_pcie_2_cfg_ahb_clk",
    .pDomain   = &ClockDomain_GCC_GCCCONFIGNOC,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x56718B0A,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_PCIE_2_CFG_AHB_CBCR), HWIO_PHYS(GCC_PCIE_2_BCR), {HWIO_PHYS(GCC_PROC_CLK_BRANCH_ENA_VOTE_2), 0x2000} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 372,
    },
  },
  {
    .szName    = "gcc_pcie_3_cfg_ahb_clk",
    .pDomain   = &ClockDomain_GCC_GCCCONFIGNOC,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0xA9E3A1CC,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_PCIE_3_CFG_AHB_CBCR), HWIO_PHYS(GCC_PCIE_3_BCR), {HWIO_PHYS(GCC_PROC_CLK_BRANCH_ENA_VOTE_2), 0x80000} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 379,
    },
  },
  {
    .szName    = "gcc_prng_ahb_clk",
    .pDomain   = &ClockDomain_GCC_GCCCONFIGNOC,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0xBFB51CCC,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_PRNG_AHB_CBCR), HWIO_PHYS(GCC_PRNG_BCR), {HWIO_PHYS(GCC_PROC_CLK_BRANCH_ENA_VOTE), 0x2000} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 143,
    },
  },
  {
    .szName    = "gcc_qmip_camera_nrt_ahb_clk",
    .pDomain   = &ClockDomain_GCC_GCCCONFIGNOC,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x5508D425,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_QMIP_CAMERA_NRT_AHB_CBCR), HWIO_PHYS(GCC_MMSS_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 61,
    },
  },
  {
    .szName    = "gcc_qmip_camera_rt_ahb_clk",
    .pDomain   = &ClockDomain_GCC_GCCCONFIGNOC,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x15F7C6F4,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_QMIP_CAMERA_RT_AHB_CBCR), HWIO_PHYS(GCC_MMSS_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 352,
    },
  },
  {
    .szName    = "gcc_qmip_disp_ahb_clk",
    .pDomain   = &ClockDomain_GCC_GCCCONFIGNOC,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x2FE1361E,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_QMIP_DISP_AHB_CBCR), HWIO_PHYS(GCC_MMSS_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 62,
    },
  },
  {
    .szName    = "gcc_qmip_video_cvp_ahb_clk",
    .pDomain   = &ClockDomain_GCC_GCCCONFIGNOC,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x5577C832,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_QMIP_VIDEO_CVP_AHB_CBCR), HWIO_PHYS(GCC_MMSS_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 60,
    },
  },
  {
    .szName    = "gcc_qmip_video_vcodec_ahb_clk",
    .pDomain   = &ClockDomain_GCC_GCCCONFIGNOC,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0xED09B49D,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_QMIP_VIDEO_VCODEC_AHB_CBCR), HWIO_PHYS(GCC_MMSS_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 348,
    },
  },
  {
    .szName    = "gcc_qupv3_wrap_0_s_ahb_clk",
    .pDomain   = &ClockDomain_GCC_GCCCONFIGNOC,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0xB8C59FEE,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_QUPV3_WRAP_0_S_AHB_CBCR), HWIO_PHYS(GCC_QUPV3_WRAPPER_0_BCR), {HWIO_PHYS(GCC_PROC_CLK_BRANCH_ENA_VOTE_1), 0x80} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 117,
    },
  },
  {
    .szName    = "gcc_qupv3_wrap_1_s_ahb_clk",
    .pDomain   = &ClockDomain_GCC_GCCCONFIGNOC,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x3428967E,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_QUPV3_WRAP_1_S_AHB_CBCR), HWIO_PHYS(GCC_QUPV3_WRAPPER_1_BCR), {HWIO_PHYS(GCC_PROC_CLK_BRANCH_ENA_VOTE_1), 0x200000} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 129,
    },
  },
  {
    .szName    = "gcc_qupv3_wrap_2_s_ahb_clk",
    .pDomain   = &ClockDomain_GCC_GCCCONFIGNOC,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x306EA255,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_QUPV3_WRAP_2_S_AHB_CBCR), HWIO_PHYS(GCC_QUPV3_WRAPPER_2_BCR), {HWIO_PHYS(GCC_PROC_CLK_BRANCH_ENA_VOTE_2), 0x2} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 406,
    },
  },
  {
    .szName    = "gcc_ufs_card_2_ahb_clk",
    .pDomain   = &ClockDomain_GCC_GCCCONFIGNOC,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0xB547FBEE,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_UFS_CARD_2_AHB_CBCR), HWIO_PHYS(GCC_UFS_CARD_2_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 395,
    },
  },
  {
    .szName    = "gcc_ufs_card_ahb_clk",
    .pDomain   = &ClockDomain_GCC_GCCCONFIGNOC,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x00959E86,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_UFS_CARD_AHB_CBCR), HWIO_PHYS(GCC_UFS_CARD_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 241,
    },
  },
  {
    .szName    = "gcc_ufs_phy_ahb_clk",
    .pDomain   = &ClockDomain_GCC_GCCCONFIGNOC,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x79B5465B,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_UFS_PHY_AHB_CBCR), HWIO_PHYS(GCC_UFS_PHY_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 252,
    },
  },
  {
    .szName    = "gcc_video_ahb_clk",
    .pDomain   = &ClockDomain_GCC_GCCCONFIGNOC,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x3DC15104,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_VIDEO_AHB_CBCR), HWIO_PHYS(GCC_MMSS_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 57,
    },
  },
  {
    .szName    = "gpu_cc_acd_ahb_clk",
    .pDomain   = &ClockDomain_GCC_GCCCONFIGNOC,
    .pDebugMux = &ClockDebugMux_GPUCC,
    .nUniqueId = 0xD8CEEBD9,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GPUCC_GPU_CC_ACD_AHB_CBCR), HWIO_PHYS(GPUCC_GPU_CC_ACD_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 32,
    },
  },
  {
    .szName    = "gpu_cc_ahb_clk",
    .pDomain   = &ClockDomain_GCC_GCCCONFIGNOC,
    .pDebugMux = &ClockDebugMux_GPUCC,
    .nUniqueId = 0x763BA1C4,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GPUCC_GPU_CC_AHB_CBCR), HWIO_PHYS(GPUCC_GPU_CC_CX_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 16,
    },
  },
  {
    .szName    = "gpu_cc_crc_ahb_clk",
    .pDomain   = &ClockDomain_GCC_GCCCONFIGNOC,
    .pDebugMux = &ClockDebugMux_GPUCC,
    .nUniqueId = 0xACF3D523,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GPUCC_GPU_CC_CRC_AHB_CBCR), HWIO_PHYS(GPUCC_GPU_CC_CX_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 17,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCCPUSSAHB                                           */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_cpuss_ahb_clk",
    .pDomain   = &ClockDomain_GCC_GCCCPUSSAHB,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x4D73717F,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_CPUSS_AHB_CBCR), 0, {HWIO_PHYS(GCC_PROC_CLK_BRANCH_ENA_VOTE), 0x200000} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 206,
    },
  },
  {
    .szName    = "gcc_sys_noc_cpuss_ahb_clk",
    .pDomain   = &ClockDomain_GCC_GCCCPUSSAHB,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0xF00CD065,
    .nFlags    = CLOCK_FLAG_LEAVE_ALWAYS_ON |
                 CLOCK_FLAG_SUPPRESSIBLE,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_SYS_NOC_CPUSS_AHB_CBCR), 0, {HWIO_PHYS(GCC_PROC_CLK_BRANCH_ENA_VOTE), 0x1} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 12,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCCPUSSRBCPR                                         */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_cpuss_rbcpr_clk",
    .pDomain   = &ClockDomain_GCC_GCCCPUSSRBCPR,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x37F2E506,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_CPUSS_RBCPR_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 208,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCEMACPTP                                            */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_emac_ptp_clk",
    .pDomain   = &ClockDomain_GCC_GCCEMACPTP,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x181C7F6A,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_EMAC_PTP_CBCR), HWIO_PHYS(GCC_EMAC_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 467,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCEMACRGMII                                          */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_emac_rgmii_clk",
    .pDomain   = &ClockDomain_GCC_GCCEMACRGMII,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0xBE0D2323,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_EMAC_RGMII_CBCR), HWIO_PHYS(GCC_EMAC_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 362,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCGP1                                                */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_gp1_clk",
    .pDomain   = &ClockDomain_GCC_GCCGP1,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x5BCD42D6,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_GP1_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 222,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCGP2                                                */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_gp2_clk",
    .pDomain   = &ClockDomain_GCC_GCCGP2,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0xC0B86CC1,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_GP2_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 223,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCGP3                                                */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_gp3_clk",
    .pDomain   = &ClockDomain_GCC_GCCGP3,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x81385E59,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_GP3_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 224,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCGP4                                                */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_gp4_clk",
    .pDomain   = &ClockDomain_GCC_GCCGP4,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x38E2B0C2,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_GP4_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 383,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCGP5                                                */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_gp5_clk",
    .pDomain   = &ClockDomain_GCC_GCCGP5,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x1FDD030F,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_GP5_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 384,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCGPUMEMNOCGFX                                       */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_ddrss_gpu_axi_clk",
    .pDomain   = &ClockDomain_GCC_GCCGPUMEMNOCGFX,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0xDF8E077C,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_DDRSS_GPU_AXI_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 187,
    },
  },
  {
    .szName    = "gcc_gpu_memnoc_gfx_clk",
    .pDomain   = &ClockDomain_GCC_GCCGPUMEMNOCGFX,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x114959A4,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_GPU_MEMNOC_GFX_CBCR), HWIO_PHYS(GCC_GPU_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 325,
    },
  },
  {
    .szName    = "gcc_gpu_snoc_dvm_gfx_clk",
    .pDomain   = &ClockDomain_GCC_GCCGPUMEMNOCGFX,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x22ACC42A,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_GPU_SNOC_DVM_GFX_CBCR), HWIO_PHYS(GCC_GPU_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 327,
    },
  },
  {
    .szName    = "gpu_cc_cx_snoc_dvm_clk",
    .pDomain   = &ClockDomain_GCC_GCCGPUMEMNOCGFX,
    .pDebugMux = &ClockDebugMux_GPUCC,
    .nUniqueId = 0xB73879BC,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GPUCC_GPU_CC_CX_SNOC_DVM_CBCR), HWIO_PHYS(GPUCC_GPU_CC_CX_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 21,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCMEMNOC                                             */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_cpuss_gnoc_clk",
    .pDomain   = &ClockDomain_GCC_GCCMEMNOC,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x28DF76FC,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_CPUSS_GNOC_CBCR), 0, {HWIO_PHYS(GCC_PROC_CLK_BRANCH_ENA_VOTE), 0x400000} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 207,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCMMNOCHFQX                                          */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_camera_hf_axi_clk",
    .pDomain   = &ClockDomain_GCC_GCCMMNOCHFQX,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x62191536,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_CAMERA_HF_AXI_CBCR), HWIO_PHYS(GCC_MMSS_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 64,
    },
  },
  {
    .szName    = "gcc_disp_hf_axi_clk",
    .pDomain   = &ClockDomain_GCC_GCCMMNOCHFQX,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0xBF311160,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_DISP_HF_AXI_CBCR), HWIO_PHYS(GCC_MMSS_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 65,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCMMNOCSFQX                                          */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_camera_sf_axi_clk",
    .pDomain   = &ClockDomain_GCC_GCCMMNOCSFQX,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x033685A3,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_CAMERA_SF_AXI_CBCR), HWIO_PHYS(GCC_MMSS_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 425,
    },
  },
  {
    .szName    = "gcc_disp_sf_axi_clk",
    .pDomain   = &ClockDomain_GCC_GCCMMNOCSFQX,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0xB96F8542,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_DISP_SF_AXI_CBCR), HWIO_PHYS(GCC_MMSS_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 353,
    },
  },
  {
    .szName    = "gcc_video_axi0_clk",
    .pDomain   = &ClockDomain_GCC_GCCMMNOCSFQX,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x527C9897,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_VIDEO_AXI0_CBCR), HWIO_PHYS(GCC_MMSS_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 63,
    },
  },
  {
    .szName    = "gcc_video_axi1_clk",
    .pDomain   = &ClockDomain_GCC_GCCMMNOCSFQX,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x4F8C5CD6,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_VIDEO_AXI1_CBCR), HWIO_PHYS(GCC_MMSS_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 423,
    },
  },
  {
    .szName    = "gcc_video_axic_clk",
    .pDomain   = &ClockDomain_GCC_GCCMMNOCSFQX,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x3A4C7374,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_VIDEO_AXIC_CBCR), HWIO_PHYS(GCC_MMSS_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 424,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCNPUAXI                                             */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_npu_axi_clk",
    .pDomain   = &ClockDomain_GCC_GCCNPUAXI,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x58605E2F,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_NPU_AXI_CBCR), HWIO_PHYS(GCC_NPU_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 489,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCPCIE0AUX                                           */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_pcie_0_aux_clk",
    .pDomain   = &ClockDomain_GCC_GCCPCIE0AUX,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0xDE4195A6,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_PCIE_0_AUX_CBCR), HWIO_PHYS(GCC_PCIE_0_BCR), {HWIO_PHYS(GCC_PROC_CLK_BRANCH_ENA_VOTE_1), 0x8} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 229,
    },
  },
  {
    .szName    = "gcc_pcie_phy_aux_clk",
    .pDomain   = &ClockDomain_GCC_GCCPCIE0AUX,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x33C29ED1,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_PCIE_PHY_AUX_CBCR), HWIO_PHYS(GCC_PCIE_PHY_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 239,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCPCIE0PIPE                                          */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_pcie_0_pipe_clk",
    .pDomain   = &ClockDomain_GCC_GCCPCIE0PIPE,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0xDCB72BF8,
    .nFlags    = CLOCK_FLAG_IGNORE_ON_STATUS,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_PCIE_0_PIPE_CBCR), HWIO_PHYS(GCC_PCIE_0_BCR), {HWIO_PHYS(GCC_PROC_CLK_BRANCH_ENA_VOTE_1), 0x10} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 230,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCPCIE1AUX                                           */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_pcie_1_aux_clk",
    .pDomain   = &ClockDomain_GCC_GCCPCIE1AUX,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0xBC6B4728,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_PCIE_1_AUX_CBCR), HWIO_PHYS(GCC_PCIE_1_BCR), {HWIO_PHYS(GCC_PROC_CLK_BRANCH_ENA_VOTE), 0x20000000} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 236,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCPCIE1PIPE                                          */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_pcie_1_pipe_clk",
    .pDomain   = &ClockDomain_GCC_GCCPCIE1PIPE,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0xB60CB671,
    .nFlags    = CLOCK_FLAG_IGNORE_ON_STATUS,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_PCIE_1_PIPE_CBCR), HWIO_PHYS(GCC_PCIE_1_BCR), {HWIO_PHYS(GCC_PROC_CLK_BRANCH_ENA_VOTE), 0x40000000} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 237,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCPCIE2AUX                                           */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_pcie_2_aux_clk",
    .pDomain   = &ClockDomain_GCC_GCCPCIE2AUX,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x02AA47A6,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_PCIE_2_AUX_CBCR), HWIO_PHYS(GCC_PCIE_2_BCR), {HWIO_PHYS(GCC_PROC_CLK_BRANCH_ENA_VOTE_2), 0x4000} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 373,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCPCIE2PIPE                                          */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_pcie_2_pipe_clk",
    .pDomain   = &ClockDomain_GCC_GCCPCIE2PIPE,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x30C0B966,
    .nFlags    = CLOCK_FLAG_IGNORE_ON_STATUS,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_PCIE_2_PIPE_CBCR), HWIO_PHYS(GCC_PCIE_2_BCR), {HWIO_PHYS(GCC_PROC_CLK_BRANCH_ENA_VOTE_2), 0x8000} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 374,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCPCIE3AUX                                           */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_pcie_3_aux_clk",
    .pDomain   = &ClockDomain_GCC_GCCPCIE3AUX,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x74EC0495,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_PCIE_3_AUX_CBCR), HWIO_PHYS(GCC_PCIE_3_BCR), {HWIO_PHYS(GCC_PROC_CLK_BRANCH_ENA_VOTE_2), 0x100000} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 380,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCPCIE3PIPE                                          */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_pcie_3_pipe_clk",
    .pDomain   = &ClockDomain_GCC_GCCPCIE3PIPE,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x7D18CF6E,
    .nFlags    = CLOCK_FLAG_IGNORE_ON_STATUS,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_PCIE_3_PIPE_CBCR), HWIO_PHYS(GCC_PCIE_3_BCR), {HWIO_PHYS(GCC_PROC_CLK_BRANCH_ENA_VOTE_2), 0x200000} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 381,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCPCIEPHYREFGEN                                      */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_pcie0_phy_refgen_clk",
    .pDomain   = &ClockDomain_GCC_GCCPCIEPHYREFGEN,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0xFCC0AA5E,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_PCIE0_PHY_REFGEN_CBCR), HWIO_PHYS(GCC_PCIE_PHY_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 499,
    },
  },
  {
    .szName    = "gcc_pcie1_phy_refgen_clk",
    .pDomain   = &ClockDomain_GCC_GCCPCIEPHYREFGEN,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0xBE73926B,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_PCIE1_PHY_REFGEN_CBCR), HWIO_PHYS(GCC_PCIE_PHY_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 356,
    },
  },
  {
    .szName    = "gcc_pcie2_phy_refgen_clk",
    .pDomain   = &ClockDomain_GCC_GCCPCIEPHYREFGEN,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x404A5B28,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_PCIE2_PHY_REFGEN_CBCR), HWIO_PHYS(GCC_PCIE_PHY_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 355,
    },
  },
  {
    .szName    = "gcc_pcie3_phy_refgen_clk",
    .pDomain   = &ClockDomain_GCC_GCCPCIEPHYREFGEN,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0xAA9446B7,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_PCIE3_PHY_REFGEN_CBCR), HWIO_PHYS(GCC_PCIE_PHY_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 354,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCPDM2                                               */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_pdm2_clk",
    .pDomain   = &ClockDomain_GCC_GCCPDM2,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0xF8EA86EA,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_PDM2_CBCR), HWIO_PHYS(GCC_PDM_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 142,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCQDSSATBA                                           */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_npu_at_clk",
    .pDomain   = &ClockDomain_GCC_GCCQDSSATBA,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x1745D177,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_NPU_AT_CBCR), HWIO_PHYS(GCC_NPU_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 491,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCQDSSATBB                                           */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gpu_cc_cx_qdss_at_clk",
    .pDomain   = &ClockDomain_GCC_GCCQDSSATBB,
    .pDebugMux = &ClockDebugMux_GPUCC,
    .nUniqueId = 0x3A64CC97,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GPUCC_GPU_CC_CX_QDSS_AT_CBCR), HWIO_PHYS(GPUCC_GPU_CC_CX_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 18,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCQDSSTRIG                                           */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_npu_trig_clk",
    .pDomain   = &ClockDomain_GCC_GCCQDSSTRIG,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x0CD993F2,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_NPU_TRIG_CBCR), HWIO_PHYS(GCC_NPU_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 490,
    },
  },
  {
    .szName    = "gpu_cc_cx_qdss_trig_clk",
    .pDomain   = &ClockDomain_GCC_GCCQDSSTRIG,
    .pDebugMux = &ClockDebugMux_GPUCC,
    .nUniqueId = 0x046DFEBA,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GPUCC_GPU_CC_CX_QDSS_TRIG_CBCR), HWIO_PHYS(GPUCC_GPU_CC_CX_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 23,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCQSPI1CORE                                          */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_qspi_1_core_clk",
    .pDomain   = &ClockDomain_GCC_GCCQSPI1CORE,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x1B8B8008,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_QSPI_1_CORE_CBCR), HWIO_PHYS(GCC_QSPI_1_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 368,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCQSPICORE                                           */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_qspi_core_clk",
    .pDomain   = &ClockDomain_GCC_GCCQSPICORE,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x106165C5,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_QSPI_CORE_CBCR), HWIO_PHYS(GCC_QSPI_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 361,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCQUPV3WRAP0CORE2X                                   */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_qupv3_wrap0_core_2x_clk",
    .pDomain   = &ClockDomain_GCC_GCCQUPV3WRAP0CORE2X,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x77020808,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_QUPV3_WRAP0_CORE_2X_CBCR), HWIO_PHYS(GCC_QUPV3_WRAPPER_0_BCR), {HWIO_PHYS(GCC_PROC_CLK_BRANCH_ENA_VOTE_1), 0x200} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 119,
    },
  },
  {
    .szName    = "gcc_qupv3_wrap0_core_clk",
    .pDomain   = &ClockDomain_GCC_GCCQUPV3WRAP0CORE2X,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x1B88DC58,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_QUPV3_WRAP0_CORE_CBCR), HWIO_PHYS(GCC_QUPV3_WRAPPER_0_BCR), {HWIO_PHYS(GCC_PROC_CLK_BRANCH_ENA_VOTE_1), 0x100} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 118,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCQUPV3WRAP0S0                                       */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_qupv3_wrap0_s0_clk",
    .pDomain   = &ClockDomain_GCC_GCCQUPV3WRAP0S0,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x56C239B6,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_QUPV3_WRAP0_S0_CBCR), HWIO_PHYS(GCC_QUPV3_WRAPPER_0_BCR), {HWIO_PHYS(GCC_PROC_CLK_BRANCH_ENA_VOTE_1), 0x400} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 120,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCQUPV3WRAP0S1                                       */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_qupv3_wrap0_s1_clk",
    .pDomain   = &ClockDomain_GCC_GCCQUPV3WRAP0S1,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x299AA1D4,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_QUPV3_WRAP0_S1_CBCR), HWIO_PHYS(GCC_QUPV3_WRAPPER_0_BCR), {HWIO_PHYS(GCC_PROC_CLK_BRANCH_ENA_VOTE_1), 0x800} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 121,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCQUPV3WRAP0S2                                       */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_qupv3_wrap0_s2_clk",
    .pDomain   = &ClockDomain_GCC_GCCQUPV3WRAP0S2,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x06533C2C,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_QUPV3_WRAP0_S2_CBCR), HWIO_PHYS(GCC_QUPV3_WRAPPER_0_BCR), {HWIO_PHYS(GCC_PROC_CLK_BRANCH_ENA_VOTE_1), 0x1000} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 122,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCQUPV3WRAP0S3                                       */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_qupv3_wrap0_s3_clk",
    .pDomain   = &ClockDomain_GCC_GCCQUPV3WRAP0S3,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x939588DB,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_QUPV3_WRAP0_S3_CBCR), HWIO_PHYS(GCC_QUPV3_WRAPPER_0_BCR), {HWIO_PHYS(GCC_PROC_CLK_BRANCH_ENA_VOTE_1), 0x2000} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 123,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCQUPV3WRAP0S4                                       */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_qupv3_wrap0_s4_clk",
    .pDomain   = &ClockDomain_GCC_GCCQUPV3WRAP0S4,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x62F41847,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_QUPV3_WRAP0_S4_CBCR), HWIO_PHYS(GCC_QUPV3_WRAPPER_0_BCR), {HWIO_PHYS(GCC_PROC_CLK_BRANCH_ENA_VOTE_1), 0x4000} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 124,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCQUPV3WRAP0S5                                       */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_qupv3_wrap0_s5_clk",
    .pDomain   = &ClockDomain_GCC_GCCQUPV3WRAP0S5,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x5751B5F3,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_QUPV3_WRAP0_S5_CBCR), HWIO_PHYS(GCC_QUPV3_WRAPPER_0_BCR), {HWIO_PHYS(GCC_PROC_CLK_BRANCH_ENA_VOTE_1), 0x8000} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 125,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCQUPV3WRAP0S6                                       */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_qupv3_wrap0_s6_clk",
    .pDomain   = &ClockDomain_GCC_GCCQUPV3WRAP0S6,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x917A11AD,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_QUPV3_WRAP0_S6_CBCR), HWIO_PHYS(GCC_QUPV3_WRAPPER_0_BCR), {HWIO_PHYS(GCC_PROC_CLK_BRANCH_ENA_VOTE_1), 0x10000} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 126,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCQUPV3WRAP0S7                                       */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_qupv3_wrap0_s7_clk",
    .pDomain   = &ClockDomain_GCC_GCCQUPV3WRAP0S7,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x69D7DBAF,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_QUPV3_WRAP0_S7_CBCR), HWIO_PHYS(GCC_QUPV3_WRAPPER_0_BCR), {HWIO_PHYS(GCC_PROC_CLK_BRANCH_ENA_VOTE_1), 0x20000} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 474,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCQUPV3WRAP1CORE2X                                   */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_qupv3_wrap1_core_2x_clk",
    .pDomain   = &ClockDomain_GCC_GCCQUPV3WRAP1CORE2X,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0xD7DB82C0,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_QUPV3_WRAP1_CORE_2X_CBCR), HWIO_PHYS(GCC_QUPV3_WRAPPER_1_BCR), {HWIO_PHYS(GCC_PROC_CLK_BRANCH_ENA_VOTE_1), 0x40000} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 131,
    },
  },
  {
    .szName    = "gcc_qupv3_wrap1_core_clk",
    .pDomain   = &ClockDomain_GCC_GCCQUPV3WRAP1CORE2X,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x40B685B0,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_QUPV3_WRAP1_CORE_CBCR), HWIO_PHYS(GCC_QUPV3_WRAPPER_1_BCR), {HWIO_PHYS(GCC_PROC_CLK_BRANCH_ENA_VOTE_1), 0x80000} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 130,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCQUPV3WRAP1S0                                       */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_qupv3_wrap1_s0_clk",
    .pDomain   = &ClockDomain_GCC_GCCQUPV3WRAP1S0,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0xA40B3E1C,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_QUPV3_WRAP1_S0_CBCR), HWIO_PHYS(GCC_QUPV3_WRAPPER_1_BCR), {HWIO_PHYS(GCC_PROC_CLK_BRANCH_ENA_VOTE_1), 0x400000} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 132,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCQUPV3WRAP1S1                                       */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_qupv3_wrap1_s1_clk",
    .pDomain   = &ClockDomain_GCC_GCCQUPV3WRAP1S1,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0xDE8BDAAC,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_QUPV3_WRAP1_S1_CBCR), HWIO_PHYS(GCC_QUPV3_WRAPPER_1_BCR), {HWIO_PHYS(GCC_PROC_CLK_BRANCH_ENA_VOTE_1), 0x800000} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 133,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCQUPV3WRAP1S2                                       */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_qupv3_wrap1_s2_clk",
    .pDomain   = &ClockDomain_GCC_GCCQUPV3WRAP1S2,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x45AB34F6,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_QUPV3_WRAP1_S2_CBCR), HWIO_PHYS(GCC_QUPV3_WRAPPER_1_BCR), {HWIO_PHYS(GCC_PROC_CLK_BRANCH_ENA_VOTE_1), 0x1000000} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 134,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCQUPV3WRAP1S3                                       */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_qupv3_wrap1_s3_clk",
    .pDomain   = &ClockDomain_GCC_GCCQUPV3WRAP1S3,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x6CBC20FD,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_QUPV3_WRAP1_S3_CBCR), HWIO_PHYS(GCC_QUPV3_WRAPPER_1_BCR), {HWIO_PHYS(GCC_PROC_CLK_BRANCH_ENA_VOTE_1), 0x2000000} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 135,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCQUPV3WRAP1S4                                       */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_qupv3_wrap1_s4_clk",
    .pDomain   = &ClockDomain_GCC_GCCQUPV3WRAP1S4,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x2A814B46,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_QUPV3_WRAP1_S4_CBCR), HWIO_PHYS(GCC_QUPV3_WRAPPER_1_BCR), {HWIO_PHYS(GCC_PROC_CLK_BRANCH_ENA_VOTE_1), 0x4000000} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 136,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCQUPV3WRAP1S5                                       */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_qupv3_wrap1_s5_clk",
    .pDomain   = &ClockDomain_GCC_GCCQUPV3WRAP1S5,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x8279058C,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_QUPV3_WRAP1_S5_CBCR), HWIO_PHYS(GCC_QUPV3_WRAPPER_1_BCR), {HWIO_PHYS(GCC_PROC_CLK_BRANCH_ENA_VOTE_1), 0x8000000} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 137,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCQUPV3WRAP2CORE2X                                   */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_qupv3_wrap2_core_2x_clk",
    .pDomain   = &ClockDomain_GCC_GCCQUPV3WRAP2CORE2X,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x70CC1B69,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_QUPV3_WRAP2_CORE_2X_CBCR), HWIO_PHYS(GCC_QUPV3_WRAPPER_2_BCR), {HWIO_PHYS(GCC_PROC_CLK_BRANCH_ENA_VOTE_2), 0x8} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 408,
    },
  },
  {
    .szName    = "gcc_qupv3_wrap2_core_clk",
    .pDomain   = &ClockDomain_GCC_GCCQUPV3WRAP2CORE2X,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0xAC5F6885,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_QUPV3_WRAP2_CORE_CBCR), HWIO_PHYS(GCC_QUPV3_WRAPPER_2_BCR), {HWIO_PHYS(GCC_PROC_CLK_BRANCH_ENA_VOTE_2), 0x1} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 407,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCQUPV3WRAP2S0                                       */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_qupv3_wrap2_s0_clk",
    .pDomain   = &ClockDomain_GCC_GCCQUPV3WRAP2S0,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x62380FFF,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_QUPV3_WRAP2_S0_CBCR), HWIO_PHYS(GCC_QUPV3_WRAPPER_2_BCR), {HWIO_PHYS(GCC_PROC_CLK_BRANCH_ENA_VOTE_2), 0x10} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 409,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCQUPV3WRAP2S1                                       */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_qupv3_wrap2_s1_clk",
    .pDomain   = &ClockDomain_GCC_GCCQUPV3WRAP2S1,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0xF02791D2,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_QUPV3_WRAP2_S1_CBCR), HWIO_PHYS(GCC_QUPV3_WRAPPER_2_BCR), {HWIO_PHYS(GCC_PROC_CLK_BRANCH_ENA_VOTE_2), 0x20} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 410,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCQUPV3WRAP2S2                                       */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_qupv3_wrap2_s2_clk",
    .pDomain   = &ClockDomain_GCC_GCCQUPV3WRAP2S2,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0xA5E32B19,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_QUPV3_WRAP2_S2_CBCR), HWIO_PHYS(GCC_QUPV3_WRAPPER_2_BCR), {HWIO_PHYS(GCC_PROC_CLK_BRANCH_ENA_VOTE_2), 0x40} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 411,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCQUPV3WRAP2S3                                       */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_qupv3_wrap2_s3_clk",
    .pDomain   = &ClockDomain_GCC_GCCQUPV3WRAP2S3,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0xEEF92C79,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_QUPV3_WRAP2_S3_CBCR), HWIO_PHYS(GCC_QUPV3_WRAPPER_2_BCR), {HWIO_PHYS(GCC_PROC_CLK_BRANCH_ENA_VOTE_2), 0x80} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 412,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCQUPV3WRAP2S4                                       */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_qupv3_wrap2_s4_clk",
    .pDomain   = &ClockDomain_GCC_GCCQUPV3WRAP2S4,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x7980B17E,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_QUPV3_WRAP2_S4_CBCR), HWIO_PHYS(GCC_QUPV3_WRAPPER_2_BCR), {HWIO_PHYS(GCC_PROC_CLK_BRANCH_ENA_VOTE_2), 0x100} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 413,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCQUPV3WRAP2S5                                       */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_qupv3_wrap2_s5_clk",
    .pDomain   = &ClockDomain_GCC_GCCQUPV3WRAP2S5,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x9777FCA3,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_QUPV3_WRAP2_S5_CBCR), HWIO_PHYS(GCC_QUPV3_WRAPPER_2_BCR), {HWIO_PHYS(GCC_PROC_CLK_BRANCH_ENA_VOTE_2), 0x200} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 414,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCSDCC2APPS                                          */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_sdcc2_apps_clk",
    .pDomain   = &ClockDomain_GCC_GCCSDCC2APPS,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x99917219,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_SDCC2_APPS_CBCR), HWIO_PHYS(GCC_SDCC2_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 112,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCSDCC4APPS                                          */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_sdcc4_apps_clk",
    .pDomain   = &ClockDomain_GCC_GCCSDCC4APPS,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x582886BC,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_SDCC4_APPS_CBCR), HWIO_PHYS(GCC_SDCC4_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 114,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCSLEEP                                              */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_tsif_inactivity_timers_clk",
    .pDomain   = &ClockDomain_GCC_GCCSLEEP,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0xC1B012A8,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_TSIF_INACTIVITY_TIMERS_CBCR), HWIO_PHYS(GCC_TSIF_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 146,
    },
  },
  {
    .szName    = "gcc_usb30_mp_sleep_clk",
    .pDomain   = &ClockDomain_GCC_GCCSLEEP,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x16CEDFD7,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_USB30_MP_SLEEP_CBCR), HWIO_PHYS(GCC_USB30_MP_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 432,
    },
  },
  {
    .szName    = "gcc_usb30_prim_sleep_clk",
    .pDomain   = &ClockDomain_GCC_GCCSLEEP,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0xA3AD9ACB,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_USB30_PRIM_SLEEP_CBCR), HWIO_PHYS(GCC_USB30_PRIM_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 96,
    },
  },
  {
    .szName    = "gcc_usb30_sec_sleep_clk",
    .pDomain   = &ClockDomain_GCC_GCCSLEEP,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x3E143E0B,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_USB30_SEC_SLEEP_CBCR), HWIO_PHYS(GCC_USB30_SEC_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 102,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCSYSNOC                                             */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_cpuss_dvm_bus_clk",
    .pDomain   = &ClockDomain_GCC_GCCSYSNOC,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0xF3C88EAC,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_CPUSS_DVM_BUS_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 211,
    },
  },
  {
    .szName    = "gcc_pcie_0_mstr_axi_clk",
    .pDomain   = &ClockDomain_GCC_GCCSYSNOC,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x6ECCBB13,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_PCIE_0_MSTR_AXI_CBCR), HWIO_PHYS(GCC_PCIE_0_BCR), {HWIO_PHYS(GCC_PROC_CLK_BRANCH_ENA_VOTE_1), 0x2} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 227,
    },
  },
  {
    .szName    = "gcc_pcie_0_slv_axi_clk",
    .pDomain   = &ClockDomain_GCC_GCCSYSNOC,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x9331B3EF,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_PCIE_0_SLV_AXI_CBCR), HWIO_PHYS(GCC_PCIE_0_BCR), {HWIO_PHYS(GCC_PROC_CLK_BRANCH_ENA_VOTE_1), 0x1} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 226,
    },
  },
  {
    .szName    = "gcc_pcie_0_slv_q2a_axi_clk",
    .pDomain   = &ClockDomain_GCC_GCCSYSNOC,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x5B161349,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_PCIE_0_SLV_Q2A_AXI_CBCR), HWIO_PHYS(GCC_PCIE_0_BCR), {HWIO_PHYS(GCC_PROC_CLK_BRANCH_ENA_VOTE_1), 0x20} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 225,
    },
  },
  {
    .szName    = "gcc_pcie_1_slv_axi_clk",
    .pDomain   = &ClockDomain_GCC_GCCSYSNOC,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0xAA65C59A,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_PCIE_1_SLV_AXI_CBCR), HWIO_PHYS(GCC_PCIE_1_BCR), {HWIO_PHYS(GCC_PROC_CLK_BRANCH_ENA_VOTE), 0x4000000} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 233,
    },
  },
  {
    .szName    = "gcc_pcie_1_slv_q2a_axi_clk",
    .pDomain   = &ClockDomain_GCC_GCCSYSNOC,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x5F52BA5E,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_PCIE_1_SLV_Q2A_AXI_CBCR), HWIO_PHYS(GCC_PCIE_1_BCR), {HWIO_PHYS(GCC_PROC_CLK_BRANCH_ENA_VOTE), 0x2000000} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 232,
    },
  },
  {
    .szName    = "gcc_pcie_2_slv_axi_clk",
    .pDomain   = &ClockDomain_GCC_GCCSYSNOC,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x6BC63136,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_PCIE_2_SLV_AXI_CBCR), HWIO_PHYS(GCC_PCIE_2_BCR), {HWIO_PHYS(GCC_PROC_CLK_BRANCH_ENA_VOTE_2), 0x800} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 370,
    },
  },
  {
    .szName    = "gcc_pcie_2_slv_q2a_axi_clk",
    .pDomain   = &ClockDomain_GCC_GCCSYSNOC,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x36C81DB4,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_PCIE_2_SLV_Q2A_AXI_CBCR), HWIO_PHYS(GCC_PCIE_2_BCR), {HWIO_PHYS(GCC_PROC_CLK_BRANCH_ENA_VOTE_2), 0x400} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 369,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCSYSNOCSFAXI                                        */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_pcie_3_slv_axi_clk",
    .pDomain   = &ClockDomain_GCC_GCCSYSNOCSFAXI,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x98464356,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_PCIE_3_SLV_AXI_CBCR), HWIO_PHYS(GCC_PCIE_3_BCR), {HWIO_PHYS(GCC_PROC_CLK_BRANCH_ENA_VOTE_2), 0x20000} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 377,
    },
  },
  {
    .szName    = "gcc_pcie_3_slv_q2a_axi_clk",
    .pDomain   = &ClockDomain_GCC_GCCSYSNOCSFAXI,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x2CEC088B,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_PCIE_3_SLV_Q2A_AXI_CBCR), HWIO_PHYS(GCC_PCIE_3_BCR), {HWIO_PHYS(GCC_PROC_CLK_BRANCH_ENA_VOTE_2), 0x10000} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 376,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCTSIFREF                                            */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_tsif_ref_clk",
    .pDomain   = &ClockDomain_GCC_GCCTSIFREF,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0xCE96D335,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_TSIF_REF_CBCR), HWIO_PHYS(GCC_TSIF_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 145,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCUFSCARD2AXI                                        */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_aggre_ufs_card_2_axi_clk",
    .pDomain   = &ClockDomain_GCC_GCCUFSCARD2AXI,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x24453BE7,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_AGGRE_UFS_CARD_2_AXI_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 461,
    },
  },
  {
    .szName    = "gcc_ufs_card_2_axi_clk",
    .pDomain   = &ClockDomain_GCC_GCCUFSCARD2AXI,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x329DEB51,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_UFS_CARD_2_AXI_CBCR), HWIO_PHYS(GCC_UFS_CARD_2_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 394,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCUFSCARD2ICECORE                                    */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_ufs_card_2_ice_core_clk",
    .pDomain   = &ClockDomain_GCC_GCCUFSCARD2ICECORE,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x304BD203,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_UFS_CARD_2_ICE_CORE_CBCR), HWIO_PHYS(GCC_UFS_CARD_2_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 401,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCUFSCARD2PHYAUX                                     */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_ufs_card_2_phy_aux_clk",
    .pDomain   = &ClockDomain_GCC_GCCUFSCARD2PHYAUX,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x739A83E6,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_UFS_CARD_2_PHY_AUX_CBCR), HWIO_PHYS(GCC_UFS_CARD_2_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 402,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCUFSCARD2RXSYMBOL0                                  */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_ufs_card_2_rx_symbol_0_clk",
    .pDomain   = &ClockDomain_GCC_GCCUFSCARD2RXSYMBOL0,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0xE67DE593,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_UFS_CARD_2_RX_SYMBOL_0_CBCR), HWIO_PHYS(GCC_UFS_CARD_2_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 397,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCUFSCARD2RXSYMBOL1                                  */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_ufs_card_2_rx_symbol_1_clk",
    .pDomain   = &ClockDomain_GCC_GCCUFSCARD2RXSYMBOL1,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x7ED38B94,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_UFS_CARD_2_RX_SYMBOL_1_CBCR), HWIO_PHYS(GCC_UFS_CARD_2_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 403,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCUFSCARD2TXSYMBOL0                                  */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_ufs_card_2_tx_symbol_0_clk",
    .pDomain   = &ClockDomain_GCC_GCCUFSCARD2TXSYMBOL0,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x118D1C65,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_UFS_CARD_2_TX_SYMBOL_0_CBCR), HWIO_PHYS(GCC_UFS_CARD_2_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 396,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCUFSCARD2UNIPROCORE                                 */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_ufs_card_2_unipro_core_clk",
    .pDomain   = &ClockDomain_GCC_GCCUFSCARD2UNIPROCORE,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0xA9CC8456,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_UFS_CARD_2_UNIPRO_CORE_CBCR), HWIO_PHYS(GCC_UFS_CARD_2_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 400,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCUFSCARDAXI                                         */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_aggre_ufs_card_axi_clk",
    .pDomain   = &ClockDomain_GCC_GCCUFSCARDAXI,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x5DAE1758,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_AGGRE_UFS_CARD_AXI_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 286,
    },
  },
  {
    .szName    = "gcc_ufs_card_axi_clk",
    .pDomain   = &ClockDomain_GCC_GCCUFSCARDAXI,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x505319F0,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_UFS_CARD_AXI_CBCR), HWIO_PHYS(GCC_UFS_CARD_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 240,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCUFSCARDICECORE                                     */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_ufs_card_ice_core_clk",
    .pDomain   = &ClockDomain_GCC_GCCUFSCARDICECORE,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x50CC9E16,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_UFS_CARD_ICE_CORE_CBCR), HWIO_PHYS(GCC_UFS_CARD_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 247,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCUFSCARDPHYAUX                                      */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_ufs_card_phy_aux_clk",
    .pDomain   = &ClockDomain_GCC_GCCUFSCARDPHYAUX,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x67584DF0,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_UFS_CARD_PHY_AUX_CBCR), HWIO_PHYS(GCC_UFS_CARD_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 248,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCUFSCARDRXSYMBOL0                                   */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_ufs_card_rx_symbol_0_clk",
    .pDomain   = &ClockDomain_GCC_GCCUFSCARDRXSYMBOL0,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x8D9198D7,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_UFS_CARD_RX_SYMBOL_0_CBCR), HWIO_PHYS(GCC_UFS_CARD_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 243,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCUFSCARDRXSYMBOL1                                   */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_ufs_card_rx_symbol_1_clk",
    .pDomain   = &ClockDomain_GCC_GCCUFSCARDRXSYMBOL1,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x44C2406D,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_UFS_CARD_RX_SYMBOL_1_CBCR), HWIO_PHYS(GCC_UFS_CARD_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 249,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCUFSCARDTXSYMBOL0                                   */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_ufs_card_tx_symbol_0_clk",
    .pDomain   = &ClockDomain_GCC_GCCUFSCARDTXSYMBOL0,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x2C56DF62,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_UFS_CARD_TX_SYMBOL_0_CBCR), HWIO_PHYS(GCC_UFS_CARD_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 242,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCUFSCARDUNIPROCORE                                  */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_ufs_card_unipro_core_clk",
    .pDomain   = &ClockDomain_GCC_GCCUFSCARDUNIPROCORE,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x3C3E010E,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_UFS_CARD_UNIPRO_CORE_CBCR), HWIO_PHYS(GCC_UFS_CARD_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 246,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCUFSPHYAXI                                          */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_aggre_ufs_phy_axi_clk",
    .pDomain   = &ClockDomain_GCC_GCCUFSPHYAXI,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x9D270053,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_AGGRE_UFS_PHY_AXI_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 285,
    },
  },
  {
    .szName    = "gcc_ufs_phy_axi_clk",
    .pDomain   = &ClockDomain_GCC_GCCUFSPHYAXI,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x9310F430,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_UFS_PHY_AXI_CBCR), HWIO_PHYS(GCC_UFS_PHY_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 251,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCUFSPHYICECORE                                      */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_ufs_phy_ice_core_clk",
    .pDomain   = &ClockDomain_GCC_GCCUFSPHYICECORE,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0xEB42CB41,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_UFS_PHY_ICE_CORE_CBCR), HWIO_PHYS(GCC_UFS_PHY_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 258,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCUFSPHYPHYAUX                                       */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_ufs_phy_phy_aux_clk",
    .pDomain   = &ClockDomain_GCC_GCCUFSPHYPHYAUX,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x224FBCE6,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_UFS_PHY_PHY_AUX_CBCR), HWIO_PHYS(GCC_UFS_PHY_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 259,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCUFSPHYRXSYMBOL0                                    */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_ufs_phy_rx_symbol_0_clk",
    .pDomain   = &ClockDomain_GCC_GCCUFSPHYRXSYMBOL0,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0xC250BA4B,
    .nFlags    = CLOCK_FLAG_IGNORE_ON_STATUS,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_UFS_PHY_RX_SYMBOL_0_CBCR), HWIO_PHYS(GCC_UFS_PHY_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 254,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCUFSPHYRXSYMBOL1                                    */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_ufs_phy_rx_symbol_1_clk",
    .pDomain   = &ClockDomain_GCC_GCCUFSPHYRXSYMBOL1,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0xB7C57145,
    .nFlags    = CLOCK_FLAG_IGNORE_ON_STATUS,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_UFS_PHY_RX_SYMBOL_1_CBCR), HWIO_PHYS(GCC_UFS_PHY_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 260,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCUFSPHYTXSYMBOL0                                    */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_ufs_phy_tx_symbol_0_clk",
    .pDomain   = &ClockDomain_GCC_GCCUFSPHYTXSYMBOL0,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0xDE965215,
    .nFlags    = CLOCK_FLAG_IGNORE_ON_STATUS,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_UFS_PHY_TX_SYMBOL_0_CBCR), HWIO_PHYS(GCC_UFS_PHY_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 253,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCUFSPHYUNIPROCORE                                   */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_ufs_phy_unipro_core_clk",
    .pDomain   = &ClockDomain_GCC_GCCUFSPHYUNIPROCORE,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x815C816D,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_UFS_PHY_UNIPRO_CORE_CBCR), HWIO_PHYS(GCC_UFS_PHY_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 257,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCUSB30MPMASTER                                      */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_aggre_usb3_mp_axi_clk",
    .pDomain   = &ClockDomain_GCC_GCCUSB30MPMASTER,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x4440A630,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_AGGRE_USB3_MP_AXI_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 456,
    },
  },
  {
    .szName    = "gcc_cfg_noc_usb3_mp_axi_clk",
    .pDomain   = &ClockDomain_GCC_GCCUSB30MPMASTER,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0xBFF38380,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_CFG_NOC_USB3_MP_AXI_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 455,
    },
  },
  {
    .szName    = "gcc_usb30_mp_master_clk",
    .pDomain   = &ClockDomain_GCC_GCCUSB30MPMASTER,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0xE9ADA344,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_USB30_MP_MASTER_CBCR), HWIO_PHYS(GCC_USB30_MP_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 431,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCUSB30MPMOCKUTMI                                    */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_usb30_mp_mock_utmi_clk",
    .pDomain   = &ClockDomain_GCC_GCCUSB30MPMOCKUTMI,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0xC8E5E96A,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_USB30_MP_MOCK_UTMI_CBCR), HWIO_PHYS(GCC_USB30_MP_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 433,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCUSB30PRIMMASTER                                    */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_aggre_usb3_prim_axi_clk",
    .pDomain   = &ClockDomain_GCC_GCCUSB30PRIMMASTER,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x9C0C2B41,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_AGGRE_USB3_PRIM_AXI_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 283,
    },
  },
  {
    .szName    = "gcc_cfg_noc_usb3_prim_axi_clk",
    .pDomain   = &ClockDomain_GCC_GCCUSB30PRIMMASTER,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0xE7023B78,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_CFG_NOC_USB3_PRIM_AXI_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 29,
    },
  },
  {
    .szName    = "gcc_usb30_prim_master_clk",
    .pDomain   = &ClockDomain_GCC_GCCUSB30PRIMMASTER,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x91482B7C,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_USB30_PRIM_MASTER_CBCR), HWIO_PHYS(GCC_USB30_PRIM_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 95,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCUSB30PRIMMOCKUTMI                                  */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_usb30_prim_mock_utmi_clk",
    .pDomain   = &ClockDomain_GCC_GCCUSB30PRIMMOCKUTMI,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0xFEDF50FF,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_USB30_PRIM_MOCK_UTMI_CBCR), HWIO_PHYS(GCC_USB30_PRIM_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 97,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCUSB30SECMASTER                                     */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_aggre_usb3_sec_axi_clk",
    .pDomain   = &ClockDomain_GCC_GCCUSB30SECMASTER,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0xE32B43A1,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_AGGRE_USB3_SEC_AXI_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 284,
    },
  },
  {
    .szName    = "gcc_cfg_noc_usb3_sec_axi_clk",
    .pDomain   = &ClockDomain_GCC_GCCUSB30SECMASTER,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0xA36F9AC7,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_CFG_NOC_USB3_SEC_AXI_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 30,
    },
  },
  {
    .szName    = "gcc_usb30_sec_master_clk",
    .pDomain   = &ClockDomain_GCC_GCCUSB30SECMASTER,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x1A382B77,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_USB30_SEC_MASTER_CBCR), HWIO_PHYS(GCC_USB30_SEC_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 101,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCUSB30SECMOCKUTMI                                   */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_usb30_sec_mock_utmi_clk",
    .pDomain   = &ClockDomain_GCC_GCCUSB30SECMOCKUTMI,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x7ADD1A36,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_USB30_SEC_MOCK_UTMI_CBCR), HWIO_PHYS(GCC_USB30_SEC_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 103,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCUSB3MPPHYAUX                                       */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_usb3_mp_phy_aux_clk",
    .pDomain   = &ClockDomain_GCC_GCCUSB3MPPHYAUX,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x48161673,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_USB3_MP_PHY_AUX_CBCR), HWIO_PHYS(GCC_USB30_MP_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 445,
    },
  },
  {
    .szName    = "gcc_usb3_mp_phy_com_aux_clk",
    .pDomain   = &ClockDomain_GCC_GCCUSB3MPPHYAUX,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0xE75DA741,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_USB3_MP_PHY_COM_AUX_CBCR), HWIO_PHYS(GCC_USB30_MP_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 446,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCUSB3MPPHYPIPE0                                     */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_usb3_mp_phy_pipe_0_clk",
    .pDomain   = &ClockDomain_GCC_GCCUSB3MPPHYPIPE0,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x84EB8704,
    .nFlags    = CLOCK_FLAG_IGNORE_ON_STATUS,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_USB3_MP_PHY_PIPE_0_CBCR), HWIO_PHYS(GCC_USB30_MP_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 447,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCUSB3MPPHYPIPE1                                     */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_usb3_mp_phy_pipe_1_clk",
    .pDomain   = &ClockDomain_GCC_GCCUSB3MPPHYPIPE1,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0xBCB19CF3,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_USB3_MP_PHY_PIPE_1_CBCR), HWIO_PHYS(GCC_USB30_MP_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 448,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCUSB3PRIMPHYAUX                                     */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_usb3_prim_phy_aux_clk",
    .pDomain   = &ClockDomain_GCC_GCCUSB3PRIMPHYAUX,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x5BBF945A,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_USB3_PRIM_PHY_AUX_CBCR), HWIO_PHYS(GCC_USB30_PRIM_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 98,
    },
  },
  {
    .szName    = "gcc_usb3_prim_phy_com_aux_clk",
    .pDomain   = &ClockDomain_GCC_GCCUSB3PRIMPHYAUX,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x84C54E8E,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_USB3_PRIM_PHY_COM_AUX_CBCR), HWIO_PHYS(GCC_USB30_PRIM_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 99,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCUSB3PRIMPHYPIPE                                    */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_usb3_prim_phy_pipe_clk",
    .pDomain   = &ClockDomain_GCC_GCCUSB3PRIMPHYPIPE,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x8F9EA437,
    .nFlags    = CLOCK_FLAG_IGNORE_ON_STATUS,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_USB3_PRIM_PHY_PIPE_CBCR), HWIO_PHYS(GCC_USB30_PRIM_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 100,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCUSB3SECPHYAUX                                      */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_usb3_sec_phy_aux_clk",
    .pDomain   = &ClockDomain_GCC_GCCUSB3SECPHYAUX,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0xC038D786,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_USB3_SEC_PHY_AUX_CBCR), HWIO_PHYS(GCC_USB30_SEC_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 104,
    },
  },
  {
    .szName    = "gcc_usb3_sec_phy_com_aux_clk",
    .pDomain   = &ClockDomain_GCC_GCCUSB3SECPHYAUX,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0xAC880F10,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_USB3_SEC_PHY_COM_AUX_CBCR), HWIO_PHYS(GCC_USB30_SEC_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 105,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCUSB3SECPHYPIPE                                     */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_usb3_sec_phy_pipe_clk",
    .pDomain   = &ClockDomain_GCC_GCCUSB3SECPHYPIPE,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x4D56A2D2,
    .nFlags    = CLOCK_FLAG_IGNORE_ON_STATUS,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_USB3_SEC_PHY_PIPE_CBCR), HWIO_PHYS(GCC_USB30_SEC_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 106,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCVSENSOR                                            */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gpu_cc_gx_vsense_clk",
    .pDomain   = &ClockDomain_GCC_GCCVSENSOR,
    .pDebugMux = &ClockDebugMux_GPUCC,
    .nUniqueId = 0x2DA8421F,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GPUCC_GPU_CC_GX_VSENSE_CBCR), HWIO_PHYS(GPUCC_GPU_CC_GX_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 12,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCXO                                                 */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_camera_xo_clk",
    .pDomain   = &ClockDomain_GCC_GCCXO,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0xEF993118,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_CAMERA_XO_CBCR), HWIO_PHYS(GCC_MMSS_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 67,
    },
  },
  {
    .szName    = "gcc_disp_xo_clk",
    .pDomain   = &ClockDomain_GCC_GCCXO,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x9A471C5C,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_DISP_XO_CBCR), HWIO_PHYS(GCC_MMSS_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 68,
    },
  },
  {
    .szName    = "gcc_pdm_xo4_clk",
    .pDomain   = &ClockDomain_GCC_GCCXO,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x1C323047,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_PDM_XO4_CBCR), HWIO_PHYS(GCC_PDM_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 141,
    },
  },
  {
    .szName    = "gcc_video_xo_clk",
    .pDomain   = &ClockDomain_GCC_GCCXO,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x79581D18,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GCC_VIDEO_XO_CBCR), HWIO_PHYS(GCC_MMSS_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 66,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GPUCC_GPUCCDEBUGMUX                                       */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gpu_cc_debug_clk",
    .pDomain   = &ClockDomain_GPUCC_GPUCCDEBUGMUX,
    .pDebugMux = NULL,
    .nUniqueId = 0x2D49728D,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GPUCC_GPU_CC_DEBUG_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = HAL_CLK_NO_DEBUG_MUX_SEL,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GPUCC_GPUCCGMU                                            */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gpu_cc_cx_gmu_clk",
    .pDomain   = &ClockDomain_GPUCC_GPUCCGMU,
    .pDebugMux = &ClockDebugMux_GPUCC,
    .nUniqueId = 0x85040864,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GPUCC_GPU_CC_CX_GMU_CBCR), HWIO_PHYS(GPUCC_GPU_CC_CX_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 24,
    },
  },
  {
    .szName    = "gpu_cc_gx_gmu_clk",
    .pDomain   = &ClockDomain_GPUCC_GPUCCGMU,
    .pDebugMux = &ClockDebugMux_GPUCC,
    .nUniqueId = 0xFD46B6AB,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GPUCC_GPU_CC_GX_GMU_CBCR), HWIO_PHYS(GPUCC_GPU_CC_GX_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 15,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GPUCC_GPUCCGXGFX3D                                        */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gpu_cc_cx_gfx3d_clk",
    .pDomain   = &ClockDomain_GPUCC_GPUCCGXGFX3D,
    .pDebugMux = &ClockDebugMux_GPUCC,
    .nUniqueId = 0x2B627FA6,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GPUCC_GPU_CC_CX_GFX3D_CBCR), HWIO_PHYS(GPUCC_GPU_CC_GFX3D_AON_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 26,
    },
  },
  {
    .szName    = "gpu_cc_cx_gfx3d_slv_clk",
    .pDomain   = &ClockDomain_GPUCC_GPUCCGXGFX3D,
    .pDebugMux = &ClockDebugMux_GPUCC,
    .nUniqueId = 0xD96DA83B,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GPUCC_GPU_CC_CX_GFX3D_SLV_CBCR), HWIO_PHYS(GPUCC_GPU_CC_GFX3D_AON_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 27,
    },
  },
  {
    .szName    = "gpu_cc_gx_gfx3d_clk",
    .pDomain   = &ClockDomain_GPUCC_GPUCCGXGFX3D,
    .pDebugMux = &ClockDebugMux_GPUCC,
    .nUniqueId = 0x40E4E782,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GPUCC_GPU_CC_GX_GFX3D_CBCR), HWIO_PHYS(GPUCC_GPU_CC_GX_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 11,
    },
  },
  {
    .szName    = "gpu_cc_spdm_gx_gfx3d_div_clk",
    .pDomain   = &ClockDomain_GPUCC_GPUCCGXGFX3D,
    .pDebugMux = &ClockDebugMux_GPUCC,
    .nUniqueId = 0xFB0C5507,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GPUCC_GPU_CC_SPDM_GX_GFX3D_DIV_CBCR), HWIO_PHYS(GPUCC_GPU_CC_SPDM_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 30,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GPUCC_GPUCCPLLTESTMUX                                     */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gpu_cc_pll_test_clk",
    .pDomain   = &ClockDomain_GPUCC_GPUCCPLLTESTMUX,
    .pDebugMux = NULL,
    .nUniqueId = 0x8F8278F9,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(GPUCC_GPU_CC_PLL_TEST_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = HAL_CLK_NO_DEBUG_MUX_SEL,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_NPUCC_NPUCCCALDP                                          */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "npu_cc_cal_dp_cdc_clk",
    .pDomain   = &ClockDomain_NPUCC_NPUCCCALDP,
    .pDebugMux = &ClockDebugMux_NPUCC,
    .nUniqueId = 0xA25073B8,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(NPU_CC_CAL_DP_CDC_CBCR), HWIO_PHYS(NPU_CC_NPU_CORE_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 8,
    },
  },
  {
    .szName    = "npu_cc_cal_dp_clk",
    .pDomain   = &ClockDomain_NPUCC_NPUCCCALDP,
    .pDebugMux = &ClockDebugMux_NPUCC,
    .nUniqueId = 0x65886E89,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(NPU_CC_CAL_DP_CBCR), HWIO_PHYS(NPU_CC_CAL_DP_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 1,
    },
  },
  {
    .szName    = "npu_cc_perf_cnt_clk",
    .pDomain   = &ClockDomain_NPUCC_NPUCCCALDP,
    .pDebugMux = &ClockDebugMux_NPUCC,
    .nUniqueId = 0x2E58BB25,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(NPU_CC_PERF_CNT_CBCR), HWIO_PHYS(NPU_CC_NPU_CORE_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 16,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_NPUCC_NPUCCDEBUGMUX                                       */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "npu_cc_debug_clk",
    .pDomain   = &ClockDomain_NPUCC_NPUCCDEBUGMUX,
    .pDebugMux = NULL,
    .nUniqueId = 0x96D5E65E,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(NPU_CC_DEBUG_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = HAL_CLK_NO_DEBUG_MUX_SEL,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_NPUCC_NPUCCNPUCORE                                        */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "npu_cc_armwic_core_clk",
    .pDomain   = &ClockDomain_NPUCC_NPUCCNPUCORE,
    .pDebugMux = &ClockDebugMux_NPUCC,
    .nUniqueId = 0x40F44F4A,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(NPU_CC_ARMWIC_CORE_CBCR), HWIO_PHYS(NPU_CC_NPU_CORE_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 4,
    },
  },
  {
    .szName    = "npu_cc_npu_core_clk",
    .pDomain   = &ClockDomain_NPUCC_NPUCCNPUCORE,
    .pDebugMux = &ClockDebugMux_NPUCC,
    .nUniqueId = 0xD69EC296,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(NPU_CC_NPU_CORE_CBCR), HWIO_PHYS(NPU_CC_NPU_CORE_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 2,
    },
  },
  {
    .szName    = "npu_cc_npu_cpc_clk",
    .pDomain   = &ClockDomain_NPUCC_NPUCCNPUCORE,
    .pDebugMux = &ClockDebugMux_NPUCC,
    .nUniqueId = 0xB1A48562,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(NPU_CC_NPU_CPC_CBCR), HWIO_PHYS(NPU_CC_NPU_CORE_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 3,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_NPUCC_NPUCCPLLTESTMUX                                     */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "npu_cc_pll_test_clk",
    .pDomain   = &ClockDomain_NPUCC_NPUCCPLLTESTMUX,
    .pDebugMux = NULL,
    .nUniqueId = 0xEC4C8715,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(NPU_CC_PLL_TEST_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = HAL_CLK_NO_DEBUG_MUX_SEL,
    },
  },
#ifdef ENABLE_AUTO_PLAT
  /*-----------------------------------------------------------------------*/
  /* ClockDomain_SCC_SCCMAINRCG                                            */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "scc_qupv3_2xcore_clk",
    .pDomain   = &ClockDomain_SCC_SCCMAINRCG,
    .pDebugMux = &ClockDebugMux_SCC,
    .nUniqueId = 0x5364E8BD,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(SSC_SCC_QUPV3_2XCORE_CBCR), 0, {HWIO_PHYS(SSC_SCC_HLOS_CLOCK_BRANCH_EN_VOTE), 0x400} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 42,
    },
  },
  {
    .szName    = "scc_qupv3_core_clk",
    .pDomain   = &ClockDomain_SCC_SCCMAINRCG,
    .pDebugMux = &ClockDebugMux_SCC,
    .nUniqueId = 0x0464AF34,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(SSC_SCC_QUPV3_CORE_CBCR), 0, {HWIO_PHYS(SSC_SCC_HLOS_CLOCK_BRANCH_EN_VOTE), 0x800} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 43,
    },
  },
  {
    .szName    = "scc_qupv3_m_hclk_clk",
    .pDomain   = &ClockDomain_SCC_SCCMAINRCG,
    .pDebugMux = &ClockDebugMux_SCC,
    .nUniqueId = 0xA25E882C,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(SSC_SCC_QUPV3_M_HCLK_CBCR), 0, {HWIO_PHYS(SSC_SCC_HLOS_CLOCK_BRANCH_EN_VOTE), 0x2} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 32,
    },
  },
  {
    .szName    = "scc_qupv3_s_hclk_clk",
    .pDomain   = &ClockDomain_SCC_SCCMAINRCG,
    .pDebugMux = &ClockDebugMux_SCC,
    .nUniqueId = 0xC4C51B76,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(SSC_SCC_QUPV3_S_HCLK_CBCR), 0, {HWIO_PHYS(SSC_SCC_HLOS_CLOCK_BRANCH_EN_VOTE), 0x1} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 18,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_SCC_SCCQUPV3SE0                                           */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "scc_qupv3_se0_clk",
    .pDomain   = &ClockDomain_SCC_SCCQUPV3SE0,
    .pDebugMux = &ClockDebugMux_SCC,
    .nUniqueId = 0x48EB0E05,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(SSC_SCC_QUPV3_SE0_CBCR), 0, {HWIO_PHYS(SSC_SCC_HLOS_CLOCK_BRANCH_EN_VOTE), 0x8} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 2,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_SCC_SCCQUPV3SE1                                           */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "scc_qupv3_se1_clk",
    .pDomain   = &ClockDomain_SCC_SCCQUPV3SE1,
    .pDebugMux = &ClockDebugMux_SCC,
    .nUniqueId = 0x00FEC79E,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(SSC_SCC_QUPV3_SE1_CBCR), 0, {HWIO_PHYS(SSC_SCC_HLOS_CLOCK_BRANCH_EN_VOTE), 0x10} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 3,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_SCC_SCCQUPV3SE2                                           */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "scc_qupv3_se2_clk",
    .pDomain   = &ClockDomain_SCC_SCCQUPV3SE2,
    .pDebugMux = &ClockDebugMux_SCC,
    .nUniqueId = 0x027E453A,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(SSC_SCC_QUPV3_SE2_CBCR), 0, {HWIO_PHYS(SSC_SCC_HLOS_CLOCK_BRANCH_EN_VOTE), 0x20} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 4,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_SCC_SCCQUPV3SE3                                           */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "scc_qupv3_se3_clk",
    .pDomain   = &ClockDomain_SCC_SCCQUPV3SE3,
    .pDebugMux = &ClockDebugMux_SCC,
    .nUniqueId = 0xF91EF977,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(SSC_SCC_QUPV3_SE3_CBCR), 0, {HWIO_PHYS(SSC_SCC_HLOS_CLOCK_BRANCH_EN_VOTE), 0x40} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 6,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_SCC_SCCQUPV3SE4                                           */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "scc_qupv3_se4_clk",
    .pDomain   = &ClockDomain_SCC_SCCQUPV3SE4,
    .pDebugMux = &ClockDebugMux_SCC,
    .nUniqueId = 0x2A17CC89,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(SSC_SCC_QUPV3_SE4_CBCR), 0, {HWIO_PHYS(SSC_SCC_HLOS_CLOCK_BRANCH_EN_VOTE), 0x80} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 7,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_SCC_SCCQUPV3SE5                                           */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "scc_qupv3_se5_clk",
    .pDomain   = &ClockDomain_SCC_SCCQUPV3SE5,
    .pDebugMux = &ClockDebugMux_SCC,
    .nUniqueId = 0x82F3BBEB,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(SSC_SCC_QUPV3_SE5_CBCR), 0, {HWIO_PHYS(SSC_SCC_HLOS_CLOCK_BRANCH_EN_VOTE), 0x100} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 8,
    },
  },
#endif
  /*-----------------------------------------------------------------------*/
  /* ClockDomain_VIDEOCC_VIDEOCCIRIS                                       */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "video_cc_iris_ahb_clk",
    .pDomain   = &ClockDomain_VIDEOCC_VIDEOCCIRIS,
    .pDebugMux = &ClockDebugMux_VIDEOCC,
    .nUniqueId = 0xF5C4F74A,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(VIDEO_CC_IRIS_AHB_CBCR), HWIO_PHYS(VIDEO_CC_INTERFACE_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 7,
    },
  },
  {
    .szName    = "video_cc_mvs0_core_clk",
    .pDomain   = &ClockDomain_VIDEOCC_VIDEOCCIRIS,
    .pDebugMux = &ClockDebugMux_VIDEOCC,
    .nUniqueId = 0x36A902E6,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(VIDEO_CC_MVS0_CORE_CBCR), HWIO_PHYS(VIDEO_CC_MVS0_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 3,
    },
  },
  {
    .szName    = "video_cc_mvs1_core_clk",
    .pDomain   = &ClockDomain_VIDEOCC_VIDEOCCIRIS,
    .pDebugMux = &ClockDebugMux_VIDEOCC,
    .nUniqueId = 0x00727E47,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(VIDEO_CC_MVS1_CORE_CBCR), HWIO_PHYS(VIDEO_CC_MVS1_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 5,
    },
  },
  {
    .szName    = "video_cc_mvsc_core_clk",
    .pDomain   = &ClockDomain_VIDEOCC_VIDEOCCIRIS,
    .pDebugMux = &ClockDebugMux_VIDEOCC,
    .nUniqueId = 0x3D608C6B,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(VIDEO_CC_MVSC_CORE_CBCR), HWIO_PHYS(VIDEO_CC_MVSC_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 1,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_VIDEOCC_VIDEOCCSLEEP                                      */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "video_cc_sleep_clk",
    .pDomain   = &ClockDomain_VIDEOCC_VIDEOCCSLEEP,
    .pDebugMux = &ClockDebugMux_VIDEOCC,
    .nUniqueId = 0x2CE12181,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(VIDEO_CC_SLEEP_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 9,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_VIDEOCC_VIDEOCCXO                                         */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "video_cc_xo_clk",
    .pDomain   = &ClockDomain_VIDEOCC_VIDEOCCXO,
    .pDebugMux = &ClockDebugMux_VIDEOCC,
    .nUniqueId = 0x3777936C,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_PHYS(VIDEO_CC_XO_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 8,
    },
  },
};


/*=========================================================================
      Power Domains
==========================================================================*/

static ClockPowerDomainNodeType aPowerDomains[] =
{
  {
    .szName         = "bps_gdsc",
    .pRail          = &ClockRail_VDDMM,
    .nFlags         = 0x0,
    .HALPowerDomain = {
      .nGDSCRAddr   = HWIO_PHYS(CAM_CC_BPS_GDSCR),
      .pmControl    = &HAL_clk_GenericPowerDomainControl,
    },
  },
  {
    .szName         = "ife_0_gdsc",
    .pRail          = &ClockRail_VDDMM,
    .nFlags         = 0x0,
    .HALPowerDomain = {
      .nGDSCRAddr   = HWIO_PHYS(CAM_CC_IFE_0_GDSCR),
      .pmControl    = &HAL_clk_GenericPowerDomainControl,
    },
  },
  {
    .szName         = "ife_1_gdsc",
    .pRail          = &ClockRail_VDDMM,
    .nFlags         = 0x0,
    .HALPowerDomain = {
      .nGDSCRAddr   = HWIO_PHYS(CAM_CC_IFE_1_GDSCR),
      .pmControl    = &HAL_clk_GenericPowerDomainControl,
    },
  },
  {
    .szName         = "ife_2_gdsc",
    .pRail          = &ClockRail_VDDMM,
    .nFlags         = 0x0,
    .HALPowerDomain = {
      .nGDSCRAddr   = HWIO_PHYS(CAM_CC_IFE_2_GDSCR),
      .pmControl    = &HAL_clk_GenericPowerDomainControl,
    },
  },
  {
    .szName         = "ife_3_gdsc",
    .pRail          = &ClockRail_VDDMM,
    .nFlags         = 0x0,
    .HALPowerDomain = {
      .nGDSCRAddr   = HWIO_PHYS(CAM_CC_IFE_3_GDSCR),
      .pmControl    = &HAL_clk_GenericPowerDomainControl,
    },
  },
  {
    .szName         = "ipe_0_gdsc",
    .pRail          = &ClockRail_VDDMM,
    .nFlags         = 0x0,
    .HALPowerDomain = {
      .nGDSCRAddr   = HWIO_PHYS(CAM_CC_IPE_0_GDSCR),
      .pmControl    = &HAL_clk_GenericPowerDomainControl,
    },
  },
  {
    .szName         = "ipe_1_gdsc",
    .pRail          = &ClockRail_VDDMM,
    .nFlags         = 0x0,
    .HALPowerDomain = {
      .nGDSCRAddr   = HWIO_PHYS(CAM_CC_IPE_1_GDSCR),
      .pmControl    = &HAL_clk_GenericPowerDomainControl,
    },
  },
  {
    .szName         = "titan_top_gdsc",
    .pRail          = &ClockRail_VDDMM,
    .nFlags         = 0x0,
    .HALPowerDomain = {
      .nGDSCRAddr   = HWIO_PHYS(CAM_CC_TITAN_TOP_GDSCR),
      .pmControl    = &HAL_clk_GenericPowerDomainControl,
    },
  },
  {
    .szName         = "mdss_core_gdsc",
    .pRail          = &ClockRail_VDDMM,
    .nFlags         = 0x0,
    .HALPowerDomain = {
      .nGDSCRAddr   = HWIO_PHYS(DISP_CC_MDSS_CORE_GDSCR),
      .pmControl    = &HAL_clk_GenericPowerDomainControl,
    },
  },
  {
    .szName         = "emac_gdsc",
    .pRail          = &ClockRail_VDDCX,
    .nFlags         = 0x0,
    .HALPowerDomain = {
      .nGDSCRAddr   = HWIO_PHYS(GCC_EMAC_GDSCR),
      .pmControl    = &HAL_clk_GenericPowerDomainControl,
    },
  },
  {
    .szName         = "gcc_hlos1_vote_aggre_noc_mmu_audio_tbu_gds",
    .pRail          = &ClockRail_VDDCX,
    .nFlags         = CLOCK_FLAG_IGNORE_OFF_STATUS,
    .HALPowerDomain = {
      .nGDSCRAddr   = HWIO_PHYS(GCC_HLOS1_VOTE_AGGRE_NOC_MMU_AUDIO_TBU_GDS),
      .pmControl    = &HAL_clk_GenericPowerDomainControl,
    },
  },
  {
    .szName         = "gcc_hlos1_vote_aggre_noc_mmu_pcie_tbu_gds",
    .pRail          = &ClockRail_VDDCX,
    .nFlags         = CLOCK_FLAG_IGNORE_OFF_STATUS,
    .HALPowerDomain = {
      .nGDSCRAddr   = HWIO_PHYS(GCC_HLOS1_VOTE_AGGRE_NOC_MMU_PCIE_TBU_GDS),
      .pmControl    = &HAL_clk_GenericPowerDomainControl,
    },
  },
  {
    .szName         = "gcc_hlos1_vote_aggre_noc_mmu_tbu1_gds",
    .pRail          = &ClockRail_VDDCX,
    .nFlags         = CLOCK_FLAG_IGNORE_OFF_STATUS,
    .HALPowerDomain = {
      .nGDSCRAddr   = HWIO_PHYS(GCC_HLOS1_VOTE_AGGRE_NOC_MMU_TBU1_GDS),
      .pmControl    = &HAL_clk_GenericPowerDomainControl,
    },
  },
  {
    .szName         = "gcc_hlos1_vote_aggre_noc_mmu_tbu2_gds",
    .pRail          = &ClockRail_VDDCX,
    .nFlags         = CLOCK_FLAG_IGNORE_OFF_STATUS,
    .HALPowerDomain = {
      .nGDSCRAddr   = HWIO_PHYS(GCC_HLOS1_VOTE_AGGRE_NOC_MMU_TBU2_GDS),
      .pmControl    = &HAL_clk_GenericPowerDomainControl,
    },
  },
  {
    .szName         = "gcc_hlos1_vote_all_smmu_mmu_gds",
    .pRail          = &ClockRail_VDDCX,
    .nFlags         = CLOCK_FLAG_IGNORE_OFF_STATUS,
    .HALPowerDomain = {
      .nGDSCRAddr   = HWIO_PHYS(GCC_HLOS1_VOTE_ALL_SMMU_MMU_GDS),
      .pmControl    = &HAL_clk_GenericPowerDomainControl,
    },
  },
  {
    .szName         = "gcc_hlos1_vote_gpu_smmu_gds",
    .pRail          = &ClockRail_VDDCX,
    .nFlags         = CLOCK_FLAG_IGNORE_OFF_STATUS,
    .HALPowerDomain = {
      .nGDSCRAddr   = HWIO_PHYS(GCC_HLOS1_VOTE_GPU_SMMU_GDS),
      .pmControl    = &HAL_clk_GenericPowerDomainControl,
    },
  },
  {
    .szName         = "gcc_hlos1_vote_mmnoc_mmu_tbu_hf0_gds",
    .pRail          = &ClockRail_VDDCX,
    .nFlags         = CLOCK_FLAG_IGNORE_OFF_STATUS,
    .HALPowerDomain = {
      .nGDSCRAddr   = HWIO_PHYS(GCC_HLOS1_VOTE_MMNOC_MMU_TBU_HF0_GDS),
      .pmControl    = &HAL_clk_GenericPowerDomainControl,
    },
  },
  {
    .szName         = "gcc_hlos1_vote_mmnoc_mmu_tbu_hf1_gds",
    .pRail          = &ClockRail_VDDCX,
    .nFlags         = CLOCK_FLAG_IGNORE_OFF_STATUS,
    .HALPowerDomain = {
      .nGDSCRAddr   = HWIO_PHYS(GCC_HLOS1_VOTE_MMNOC_MMU_TBU_HF1_GDS),
      .pmControl    = &HAL_clk_GenericPowerDomainControl,
    },
  },
  {
    .szName         = "gcc_hlos1_vote_mmnoc_mmu_tbu_sf_gds",
    .pRail          = &ClockRail_VDDCX,
    .nFlags         = CLOCK_FLAG_IGNORE_OFF_STATUS,
    .HALPowerDomain = {
      .nGDSCRAddr   = HWIO_PHYS(GCC_HLOS1_VOTE_MMNOC_MMU_TBU_SF_GDS),
      .pmControl    = &HAL_clk_GenericPowerDomainControl,
    },
  },
  {
    .szName         = "gcc_hlos1_vote_mmu_tcu_gds",
    .pRail          = &ClockRail_VDDCX,
    .nFlags         = CLOCK_FLAG_IGNORE_OFF_STATUS,
    .HALPowerDomain = {
      .nGDSCRAddr   = HWIO_PHYS(GCC_HLOS1_VOTE_MMU_TCU_GDS),
      .pmControl    = &HAL_clk_GenericPowerDomainControl,
    },
  },
  {
    .szName         = "gcc_hlos1_vote_turing_mmu_tbu0_gds",
    .pRail          = &ClockRail_VDDCX,
    .nFlags         = CLOCK_FLAG_IGNORE_OFF_STATUS,
    .HALPowerDomain = {
      .nGDSCRAddr   = HWIO_PHYS(GCC_HLOS1_VOTE_TURING_MMU_TBU0_GDS),
      .pmControl    = &HAL_clk_GenericPowerDomainControl,
    },
  },
  {
    .szName         = "gcc_hlos1_vote_turing_mmu_tbu1_gds",
    .pRail          = &ClockRail_VDDCX,
    .nFlags         = CLOCK_FLAG_IGNORE_OFF_STATUS,
    .HALPowerDomain = {
      .nGDSCRAddr   = HWIO_PHYS(GCC_HLOS1_VOTE_TURING_MMU_TBU1_GDS),
      .pmControl    = &HAL_clk_GenericPowerDomainControl,
    },
  },
  {
    .szName         = "pcie_0_gdsc",
    .pRail          = &ClockRail_VDDCX,
    .nFlags         = 0x0,
    .HALPowerDomain = {
      .nGDSCRAddr   = HWIO_PHYS(GCC_PCIE_0_GDSCR),
      .pmControl    = &HAL_clk_GenericPowerDomainControl,
    },
  },
  {
    .szName         = "pcie_1_gdsc",
    .pRail          = &ClockRail_VDDCX,
    .nFlags         = 0x0,
    .HALPowerDomain = {
      .nGDSCRAddr   = HWIO_PHYS(GCC_PCIE_1_GDSCR),
      .pmControl    = &HAL_clk_GenericPowerDomainControl,
    },
  },
  {
    .szName         = "pcie_2_gdsc",
    .pRail          = &ClockRail_VDDCX,
    .nFlags         = 0x0,
    .HALPowerDomain = {
      .nGDSCRAddr   = HWIO_PHYS(GCC_PCIE_2_GDSCR),
      .pmControl    = &HAL_clk_GenericPowerDomainControl,
    },
  },
  {
    .szName         = "pcie_3_gdsc",
    .pRail          = &ClockRail_VDDCX,
    .nFlags         = 0x0,
    .HALPowerDomain = {
      .nGDSCRAddr   = HWIO_PHYS(GCC_PCIE_3_GDSCR),
      .pmControl    = &HAL_clk_GenericPowerDomainControl,
    },
  },
  {
    .szName         = "ufs_card_2_gdsc",
    .pRail          = &ClockRail_VDDCX,
    .nFlags         = 0x0,
    .HALPowerDomain = {
      .nGDSCRAddr   = HWIO_PHYS(GCC_UFS_CARD_2_GDSCR),
      .pmControl    = &HAL_clk_GenericPowerDomainControl,
    },
  },
  {
    .szName         = "ufs_card_gdsc",
    .pRail          = &ClockRail_VDDCX,
    .nFlags         = 0x0,
    .HALPowerDomain = {
      .nGDSCRAddr   = HWIO_PHYS(GCC_UFS_CARD_GDSCR),
      .pmControl    = &HAL_clk_GenericPowerDomainControl,
    },
  },
  {
    .szName         = "ufs_phy_gdsc",
    .pRail          = &ClockRail_VDDCX,
    .nFlags         = 0x0,
    .HALPowerDomain = {
      .nGDSCRAddr   = HWIO_PHYS(GCC_UFS_PHY_GDSCR),
      .pmControl    = &HAL_clk_GenericPowerDomainControl,
    },
  },
  {
    .szName         = "usb30_mp_gdsc",
    .pRail          = &ClockRail_VDDCX,
    .nFlags         = 0x0,
    .HALPowerDomain = {
      .nGDSCRAddr   = HWIO_PHYS(GCC_USB30_MP_GDSCR),
      .pmControl    = &HAL_clk_GenericPowerDomainControl,
    },
  },
  {
    .szName         = "usb30_prim_gdsc",
    .pRail          = &ClockRail_VDDCX,
    .nFlags         = CLOCK_FLAG_SUPPRESSIBLE,
    .HALPowerDomain = {
      .nGDSCRAddr   = HWIO_PHYS(GCC_USB30_PRIM_GDSCR),
      .pmControl    = &HAL_clk_GenericPowerDomainControl,
    },
  },
  {
    .szName         = "usb30_sec_gdsc",
    .pRail          = &ClockRail_VDDCX,
    .nFlags         = CLOCK_FLAG_SUPPRESSIBLE,
    .HALPowerDomain = {
      .nGDSCRAddr   = HWIO_PHYS(GCC_USB30_SEC_GDSCR),
      .pmControl    = &HAL_clk_GenericPowerDomainControl,
    },
  },
  {
    .szName         = "cx_gdsc",
    .pRail          = &ClockRail_VDDCX,
    .nFlags         = 0x0,
    .HALPowerDomain = {
      .nGDSCRAddr   = HWIO_PHYS(GPUCC_GPU_CC_CX_GDSCR),
      .pmControl    = &HAL_clk_GenericPowerDomainControl,
    },
  },
  {
    .szName         = "gx_gdsc",
    .pRail          = &ClockRail_VDDCX,
    .nFlags         = 0x0,
    .HALPowerDomain = {
      .nGDSCRAddr   = HWIO_PHYS(GPUCC_GPU_CC_GX_GDSCR),
      .pmControl    = &HAL_clk_GenericPowerDomainControl,
    },
  },
  {
    .szName         = "npu_core_gdsc",
    .pRail          = &ClockRail_VDDCX,
    .nFlags         = 0x0,
    .HALPowerDomain = {
      .nGDSCRAddr   = HWIO_PHYS(NPU_CC_NPU_CORE_GDSCR),
      .pmControl    = &HAL_clk_GenericPowerDomainControl,
    },
  },
  {
    .szName         = "mvs0_gdsc",
    .pRail          = &ClockRail_VDDMM,
    .nFlags         = 0x0,
    .HALPowerDomain = {
      .nGDSCRAddr   = HWIO_PHYS(VIDEO_CC_MVS0_GDSCR),
      .pmControl    = &HAL_clk_GenericPowerDomainControl,
    },
  },
  {
    .szName         = "mvs1_gdsc",
    .pRail          = &ClockRail_VDDMM,
    .nFlags         = 0x0,
    .HALPowerDomain = {
      .nGDSCRAddr   = HWIO_PHYS(VIDEO_CC_MVS1_GDSCR),
      .pmControl    = &HAL_clk_GenericPowerDomainControl,
    },
  },
  {
    .szName         = "mvsc_gdsc",
    .pRail          = &ClockRail_VDDMM,
    .nFlags         = 0x0,
    .HALPowerDomain = {
      .nGDSCRAddr   = HWIO_PHYS(VIDEO_CC_MVSC_GDSCR),
      .pmControl    = &HAL_clk_GenericPowerDomainControl,
    },
  },
};

/*=========================================================================
      Main BSP
==========================================================================*/

ClockBSPType ClockBSP =
{
  .aBases            = aBases,
  .nNumBases         = SIZEOF_ARRAY(aBases),
  .aRails            = aRails,
  .nNumRails         = SIZEOF_ARRAY(aRails),
  .aClocks           = aClocks,
  .nNumClocks        = SIZEOF_ARRAY(aClocks),
  .aDomains          = aDomains,
  .nNumDomains       = SIZEOF_ARRAY(aDomains),
  .aSources          = aSources,
  .nNumSources       = SIZEOF_ARRAY(aSources),
  .aPowerDomains     = aPowerDomains,
  .nNumPowerDomains  = SIZEOF_ARRAY(aPowerDomains),
  .aDebugMuxes       = aDebugMuxes,
  .nNumDebugMuxes    = SIZEOF_ARRAY(aDebugMuxes),
  .nFlags            = CLOCK_GLOBAL_FLAG_LOG_CLOCK_FREQ_CHANGE |
                       CLOCK_GLOBAL_FLAG_LOG_SOURCE_STATE_CHANGE,
  .SafeMuxConfig     = { 0, 2, 0, 0 },
};

