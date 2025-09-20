#ifndef CLOCKBSP_H
#define CLOCKBSP_H
/**
==============================================================================

  @file ClockBSP.h

  Internal header file for the BSP data structures.

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

#include "ClockBoot.h"
#include "ClockDriver.h"

/*=========================================================================
      References
==========================================================================*/

/*=========================================================================
      Definitions
==========================================================================*/

/*=========================================================================
      Type Definitions
==========================================================================*/
#define QUPV3_PERF_NUM 4

/* Offset to index for chip revisions */
#define V1 0
#define V2 1
#define CHIP_REVS 2

/* Data structure for configuration data */
typedef struct
{
  /* PLL configurations */
  ClockConfigPLLType GPLL0_Cfg;
  ClockConfigPLLType GPLL1_Cfg;
  ClockConfigPLLType GPLL2_Cfg;
  ClockConfigPLLType GPLL3_Cfg;
  ClockConfigPLLType GPLL4_Cfg;
  ClockConfigPLLType GPLL5_Cfg;
  ClockConfigPLLType GPLL6_Cfg;
  ClockConfigPLLType GPLL7_Cfg;
  ClockConfigPLLType GPLL8_Cfg;
  ClockConfigPLLType GPLL9_Cfg;
  ClockConfigPLLType GPLL10_Cfg[CHIP_REVS];;
  ClockConfigPLLType AOSS_PLL0_Cfg;

  /* CPU & L3 config data */
  ClockAPCSCfgType CPU_Cfg[CHIP_REVS][CLOCK_BOOT_PERF_NUM];
  ClockAPCSCfgType L3_Cfg[CHIP_REVS][CLOCK_BOOT_PERF_NUM];

  /* System NOC config data */
  ClockConfigMuxType gcc_sys_noc_clk_src[CLOCK_BOOT_PERF_NUM];
  ClockConfigMuxType gcc_sys_noc_gc_axi_clk_src[CLOCK_BOOT_PERF_NUM];
  ClockConfigMuxType gcc_sys_noc_sf_axi_clk_src[CLOCK_BOOT_PERF_NUM];
  ClockConfigMuxType gcc_sys_noc_monaq_axi_clk_src[CLOCK_BOOT_PERF_NUM];
  
  /* Aggre NOCs */
  ClockConfigMuxType gcc_aggre_noc_clk_src[CLOCK_BOOT_PERF_NUM];
  ClockConfigMuxType gcc_aggre_noc_lpass_sf_clk_src[CLOCK_BOOT_PERF_NUM];
  ClockConfigMuxType gcc_aggre_noc_south_sf_clk_src[CLOCK_BOOT_PERF_NUM];

  /* Config NOCs */
  ClockConfigMuxType gcc_config_noc_clk_src[CLOCK_BOOT_PERF_NUM];
  ClockConfigMuxType gcc_cnoc_periph_clk_src[CLOCK_BOOT_PERF_NUM];

  /* System HUB */
  ClockConfigMuxType gcc_cpuss_axi_clk_src[CLOCK_BOOT_PERF_NUM];
  ClockConfigMuxType gcc_gpu_memnoc_gfx_clk_src[CHIP_REVS][CLOCK_BOOT_PERF_NUM];
  ClockConfigMuxType gcc_memnoc_clk_src[CHIP_REVS][CLOCK_BOOT_PERF_NUM];
  ClockConfigMuxType gcc_mmu_tcu_clk_src[CLOCK_BOOT_PERF_NUM];
  ClockConfigMuxType gcc_mss_q6_memnoc_axi_clk_src[CLOCK_BOOT_PERF_NUM];

  /* Crypto Engine (CE) config data */
  ClockConfigMuxType gcc_ce1_clk_src[CLOCK_BOOT_PERF_NUM];

  /* UFS */
  ClockConfigMuxType gcc_ufs_phy_axi_clk_src[CLOCK_BOOT_PERF_NUM];
  ClockConfigMuxType gcc_ufs_phy_ice_core_clk_src[CLOCK_BOOT_PERF_NUM];
  ClockConfigMuxType gcc_ufs_phy_unipro_core_clk_src[CLOCK_BOOT_PERF_NUM];

  /* UFS_2 card */
  ClockConfigMuxType gcc_ufs_card_2_axi_clk_src[CLOCK_BOOT_PERF_NUM];
  ClockConfigMuxType gcc_ufs_card_2_ice_core_clk_src[CLOCK_BOOT_PERF_NUM];
  ClockConfigMuxType gcc_ufs_card_2_unipro_core_clk_src[CLOCK_BOOT_PERF_NUM];

  ClockConfigMuxType gcc_pcie_phy_refgen_clk_src[CLOCK_BOOT_PERF_NUM];

  /* Core2x QUP V3 config data */
  ClockConfigMuxType gcc_qupv3_wrap0_core_2x_clk_src[CLOCK_BOOT_PERF_NUM];

  /* QUP V3 configurations */
  ClockFreqMuxCfgType QUPV3_Cfg[QUPV3_PERF_NUM];

  /* SDC table (for backwards compatibility) */
  uint32 SDC_Cfg[CLOCK_BOOT_PERF_NUM];

  /* SDC extended configurations */
  ClockFreqMuxCfgType SDC_Ext_Cfg[5];

  /* SPI Serial configurations */
  ClockFreqMuxCfgType gcc_qspi_core_clk_src[5];

  /* SPI configurations */
  ClockFreqMuxCfgType SPI_Cfg[3];
  
  /* aClockMapCorner Used for Mapping the clock Driver Voltage corner ENUMs to Railway Driver Voltage Corner ENUMs */
  railway_corner aClockMapCorner[CLOCK_BOOT_PERF_NUM];

} Clock_ConfigType;

/* Data structure for Railway data */
typedef struct
{
  char*             CxRail;
  int               nCxRailId;
  railway_voter_t   CxVoter;
  char*             EBIRail;
  int               nEBIRailId;
  railway_voter_t   EBIVoter;
}Clock_RailwayType;

typedef enum
{
	CLOCK_RAIL_CX,
	CLOCK_RAIL_MX,
	CLOCK_RAIL_APC,
	CLOCK_RAIL_NUM,
}ClockRailType;

typedef enum
{
  CLOCK_RAIL_CLIENT_BOOT,
  CLOCK_RAIL_CLIENT_DDR,
  CLOCK_RAIL_CLIENT_NUM,	
}ClockRailClinet;

extern Clock_ConfigType *Clock_Config( void );
extern Clock_RailwayType *Clock_RailwayConfig( void );
extern boolean Clock_EnableSource( ClockSourceType eSource );
extern boolean Clock_DisableSource( ClockSourceType eSource );
extern boolean Clock_ConfigureSource( ClockSourceType eSource );
extern boolean Clock_SwitchRail(ClockRailType eRail, railway_corner corner_vote, ClockRailClinet eClient);


boolean Clock_SourceMapToMux
(
  const ClockConfigMuxType *pConfig,
  uint32 *nMuxValue
);

#endif /* !CLOCKBSP_H */

