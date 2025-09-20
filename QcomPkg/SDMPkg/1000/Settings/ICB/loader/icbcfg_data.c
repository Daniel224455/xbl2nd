/*==============================================================================

FILE:      icbcfg_data.c

DESCRIPTION: This file implements the ICB Configuration driver.

PUBLIC CLASSES:  Not Applicable

INITIALIZATION AND SEQUENCING REQUIREMENTS:  N/A

Edit History

$Change: 30389851 $

When        Who    What, where, why
----------  ---    -----------------------------------------------------------
2020/06/25  sds    Force allocations in system cache for GPU TCU
2019/06/19  sds    GEMNOC HSR update (disable DVM)
2018/09/20  sds    V2 address updates for overrides on GEMNOC
2018/08/09  sds    ECC port overrides (QCTDD04324786)
2018/04/19  sds    Update for Poipu
2017/09/29  sds    Updated for Hana
2017/07/19  sds    Update QM throttle config
2017/06/14  sds    OSDC configuration
2017/06/12  sds    Initial QM config
2017/05/08  sds    Initial v2 config
2016/11/18  sds    Created

                Copyright (c) 2016-2020 Qualcomm Technologies, Inc.
                             All Rights Reserved.
                          QUALCOMM Proprietary/GTDR
==============================================================================*/
#include "icbcfg_hwio.h"
#include "icbcfg_hwio_v2.h"
#include "../../../../../Library/ICBLib/icbcfgi.h"
#include "../../../Library/ICBTargetLib/icbcfg_hwio_target.h"

/*---------------------------------------------------------------------------*/
/*          Macro and constant definitions                                   */
/*---------------------------------------------------------------------------*/
//#define ARRAY_SIZE(arr) (sizeof(arr)/sizeof((arr)[0]))

/*============================================================================
                        DEVICE CONFIG PROPERTY DATA
============================================================================*/

/*---------------------------------------------------------------------------*/
/*          Properties data for device ID  = "icbcfg/boot"                   */
/*---------------------------------------------------------------------------*/

/* ICBcfg Boot Configuration Data*/

/*---------------------------------------------------------------------------*/
/* SDM1000 v1                                                                */
/*---------------------------------------------------------------------------*/
icbcfg_data_type icbcfg_boot_data_sdm1000_v1[] =
{
  /* DSAT core registers */
  ICBCFG_HWIOF_DW(GCC_MMSS_QM_CORE_CBCR, CLK_ENABLE, 0x1),
  ICBCFG_WAIT_HWIOF_DW(GCC_MMSS_QM_CORE_CBCR, CLK_OFF, 0x0),
  ICBCFG_HWIOF_DW(GCC_MMSS_QM_AHB_CBCR, CLK_ENABLE, 0x1),
  ICBCFG_WAIT_HWIOF_DW(GCC_MMSS_QM_AHB_CBCR, CLK_OFF, 0x0),
  ICBCFG_HWIO_DW(QM_CORE_CLK_CGC_CNTRL, 0x1),
  ICBCFG_HWIO_DW(QM_PRE_STALL_TIMEOUT_CNT_URG_0, 0x60),
  ICBCFG_HWIO_DW(QM_PRE_STALL_TIMEOUT_CNT_URG_1, 0x27),
  ICBCFG_HWIO_DW(QM_PRE_STALL_TIMEOUT_CNT_URG_2, 0x27),
  ICBCFG_HWIO_DW(QM_PRE_STALL_TIMEOUT_CNT_URG_3, 0x27),
  ICBCFG_HWIO_DW(QM_POST_STALL_TIMEOUT_CNT_URG_0, 0x60),
  ICBCFG_HWIO_DW(QM_POST_STALL_TIMEOUT_CNT_URG_1, 0x27),
  ICBCFG_HWIO_DW(QM_POST_STALL_TIMEOUT_CNT_URG_2, 0x27),
  ICBCFG_HWIO_DW(QM_POST_STALL_TIMEOUT_CNT_URG_3, 0x27),
  ICBCFG_HWIO_DW(QM_POST_STALL_WDW_OVERLAP_CNTL, 0x1),
  ICBCFG_HWIO_DW(QM_THROTTLE_LEVEL_LEGACY_SELECT, 0x0),
  ICBCFG_HWIOI_DW(QM_THROTTLE_LEVEL_QOS_n, 7, 0x7FC),
  ICBCFG_HWIO_DW(QM_THROTTLE_LEVEL_SU_CFG, 0x7F),
  ICBCFG_HWIO_DW(QM_CLNT_2_DANGER_AGGR_CNTRL, 0x3),
  ICBCFG_HWIOI_DW(QM_CLNT_2_THROTTLE_LEVEL_QOS_n, 1, 0xEE000001),
  ICBCFG_HWIOI_DW(QM_CLNT_2_THROTTLE_LEVEL_QOS_n, 2, 0xEE000002),
  ICBCFG_HWIOI_DW(QM_CLNT_2_THROTTLE_LEVEL_QOS_n, 3, 0xEE000003),
  ICBCFG_HWIOI_DW(QM_CLNT_2_THROTTLE_LEVEL_QOS_n, 7, 0xAA000000),
  ICBCFG_HWIO_DW(QM_CLNT_2_THROTTLE_LEVEL_OUTPUT_EN, 0x3),
  ICBCFG_HWIO_DW(QM_CLNT_3_DANGER_AGGR_CNTRL, 0x3),
  ICBCFG_HWIOI_DW(QM_CLNT_3_THROTTLE_LEVEL_QOS_n, 7, 0xEE000001),
  ICBCFG_HWIO_DW(QM_CLNT_3_THROTTLE_LEVEL_OUTPUT_EN, 0x2),
  ICBCFG_HWIO_DW(QM_CLNT_8_DANGER_AGGR_CNTRL, 0x3),
  ICBCFG_HWIO_DW(QM_CLNT_8_DANGER_AGGR_MODEM_CNTRL, 0x0),
  ICBCFG_HWIOI_DW(QM_CLNT_8_THROTTLE_LEVEL_QOS_n, 3, 0xEE000003),
  ICBCFG_HWIOI_DW(QM_CLNT_8_THROTTLE_LEVEL_QOS_n, 7, 0xAA000000),
  ICBCFG_HWIO_DW(QM_CLNT_8_THROTTLE_LEVEL_OUTPUT_EN, 0x1),
  ICBCFG_HWIO_DW(QM_CLNT_9_DANGER_AGGR_CNTRL, 0x3),
  ICBCFG_HWIO_DW(QM_CLNT_9_DANGER_AGGR_MODEM_CNTRL, 0x0),
  ICBCFG_HWIOI_DW(QM_CLNT_9_THROTTLE_LEVEL_QOS_n, 1, 0xEE000001),
  ICBCFG_HWIOI_DW(QM_CLNT_9_THROTTLE_LEVEL_QOS_n, 2, 0xEE000002),
  ICBCFG_HWIOI_DW(QM_CLNT_9_THROTTLE_LEVEL_QOS_n, 3, 0xEE000003),
  ICBCFG_HWIOI_DW(QM_CLNT_9_THROTTLE_LEVEL_QOS_n, 7, 0xAA000000),
  ICBCFG_HWIO_DW(QM_CLNT_9_THROTTLE_LEVEL_OUTPUT_EN, 0x1),
  ICBCFG_HWIOI_DW(QM_CLNT_10_THROTTLE_LEVEL_QOS_n, 1, 0xEE000001),
  ICBCFG_HWIOI_DW(QM_CLNT_10_THROTTLE_LEVEL_QOS_n, 2, 0xEE000002),
  ICBCFG_HWIOI_DW(QM_CLNT_10_THROTTLE_LEVEL_QOS_n, 3, 0xEE000003),
  ICBCFG_HWIOI_DW(QM_CLNT_10_THROTTLE_LEVEL_QOS_n, 7, 0xAA000000),
  ICBCFG_HWIO_DW(QM_CLNT_10_THROTTLE_LEVEL_OUTPUT_EN, 0x1),
  ICBCFG_HWIO_DW(QM_DSP_DANGER_AGGR_CNTRL, 0x3),
  ICBCFG_HWIO_DW(QM_DSP_DANGER_AGGR_MODEM_CNTRL, 0x0),
  ICBCFG_HWIO_DW(QM_DSP_PRIORITY_MODE_SEL, 0x1),
  ICBCFG_HWIO_DW(QM_DSP_SYSTEM_DANGER_AGGR_MODEM_CNTRL, 0x0),
  ICBCFG_HWIO_DW(QM_DSP_SYSTEM_DANGER_OUTPUT_EN, 0x0),
  ICBCFG_HWIOF_READ_DW(CABO0_CABO_GLOBAL_DDR_CLK_PERIOD_CFG,DDR_CLK_PERIOD),
  ICBCFG_HWIOF_WRITE_DW(QM_PERIOD_BUS_CFG,PERIOD_BUS_SW),
  ICBCFG_HWIOF_DW(QM_PERIOD_BUS_CFG, PERIOD_BUS_LOAD_SW, 0x1),
  ICBCFG_HWIOF_DW(QM_PERIOD_BUS_CFG, PERIOD_BUS_SW_OVERRIDE, 0x0),
  ICBCFG_HWIOI_DW(QOS_FREQ_BAND_BNDRY_n, 0, 0x97B),
  ICBCFG_HWIOI_DW(QOS_FREQ_BAND_BNDRY_n, 1, 0x0),
  ICBCFG_HWIOI_DW(QOS_FREQ_BAND_BNDRY_n, 2, 0x0),
  ICBCFG_HWIO_DW(QM_AGGR_SAFE_OVERRIDE_CNTRL, 0x1),
  ICBCFG_HWIO_DW(QM_DANGER_SAFE_CNTRL,0x1),

  /* AP throttles and shapers */
  ICBCFG_HWIOF_DW(APP_BWMON_THROTTLE_0_CGC_CNTRL, THROTTLE_CGC_EN, 0x1),
  ICBCFG_HWIOF_DW(APP_BWMON_THROTTLE_0_GRANT_PERIOD, GRANT_PERIOD, 0x3e8),
  ICBCFG_HWIO_DW(APP_BWMON_THROTTLE_0_THRESHOLD_03, 0xFFFFFFFF),
  ICBCFG_HWIO_DW(APP_BWMON_THROTTLE_0_THRESHOLD_02, 0xFFFF0064),
  ICBCFG_HWIO_DW(APP_BWMON_THROTTLE_0_THRESHOLD_01, 0xFFFFFFFF),
  ICBCFG_HWIO_DW(APP_BWMON_THROTTLE_0_THRESHOLD_00, 0xFFFFFFFF),
  ICBCFG_HWIOF_DW(APP_BWMON_THROTTLE_0_PEAK_ACCUM_CREDIT, PEAK_ACCUM_CREDIT, 0x100),
  ICBCFG_HWIO_DW(APP_BWMON_THROTTLE_0_CNTRL, 0x0E01),

  ICBCFG_HWIOF_DW(APP_BWMON_THROTTLE_1_CGC_CNTRL, THROTTLE_CGC_EN, 0x1),
  ICBCFG_HWIOF_DW(APP_BWMON_THROTTLE_1_GRANT_PERIOD, GRANT_PERIOD, 0x3e8),
  ICBCFG_HWIO_DW(APP_BWMON_THROTTLE_1_THRESHOLD_03, 0xFFFFFFFF),
  ICBCFG_HWIO_DW(APP_BWMON_THROTTLE_1_THRESHOLD_02, 0xFFFF0064),
  ICBCFG_HWIO_DW(APP_BWMON_THROTTLE_1_THRESHOLD_01, 0xFFFFFFFF),
  ICBCFG_HWIO_DW(APP_BWMON_THROTTLE_1_THRESHOLD_00, 0xFFFFFFFF),
  ICBCFG_HWIOF_DW(APP_BWMON_THROTTLE_1_PEAK_ACCUM_CREDIT, PEAK_ACCUM_CREDIT, 0x100),
  ICBCFG_HWIO_DW(APP_BWMON_THROTTLE_1_CNTRL, 0x0E01),

  /* CDSP throttle and shaper */
  ICBCFG_HWIOF_DW(CDSP_BWMON_THROTTLE_0_CGC_CNTRL, THROTTLE_CGC_EN, 0x1),
  ICBCFG_HWIOF_DW(CDSP_BWMON_THROTTLE_0_GRANT_PERIOD, GRANT_PERIOD, 0x3e8),
  ICBCFG_HWIO_DW(CDSP_BWMON_THROTTLE_0_THRESHOLD_03, 0xFFFFFFFF),
  ICBCFG_HWIO_DW(CDSP_BWMON_THROTTLE_0_THRESHOLD_02, 0xFFFF0064),
  ICBCFG_HWIO_DW(CDSP_BWMON_THROTTLE_0_THRESHOLD_01, 0xFFFFFFFF),
  ICBCFG_HWIO_DW(CDSP_BWMON_THROTTLE_0_THRESHOLD_00, 0xFFFFFFFF),
  ICBCFG_HWIOF_DW(CDSP_BWMON_THROTTLE_0_PEAK_ACCUM_CREDIT, PEAK_ACCUM_CREDIT, 0x100),
  ICBCFG_HWIO_DW(CDSP_BWMON_THROTTLE_0_CNTRL, 0x0E01),

  /* GPU throttles and shapers */
  ICBCFG_HWIOF_DW(GPU_BWMON_THROTTLE_0_CGC_CNTRL, THROTTLE_CGC_EN, 0x1),
  ICBCFG_HWIOF_DW(GPU_BWMON_THROTTLE_0_GRANT_PERIOD, GRANT_PERIOD, 0x3e8),
  ICBCFG_HWIO_DW(GPU_BWMON_THROTTLE_0_THRESHOLD_03, 0xFFFFFFFF),
  ICBCFG_HWIO_DW(GPU_BWMON_THROTTLE_0_THRESHOLD_02, 0xFFFF0064),
  ICBCFG_HWIO_DW(GPU_BWMON_THROTTLE_0_THRESHOLD_01, 0xFFFFFFFF),
  ICBCFG_HWIO_DW(GPU_BWMON_THROTTLE_0_THRESHOLD_00, 0xFFFFFFFF),
  ICBCFG_HWIOF_DW(GPU_BWMON_THROTTLE_0_PEAK_ACCUM_CREDIT, PEAK_ACCUM_CREDIT, 0x100),
  ICBCFG_HWIO_DW(GPU_BWMON_THROTTLE_0_CNTRL, 0x0E01),

  ICBCFG_HWIOF_DW(GPU_BWMON_THROTTLE_1_CGC_CNTRL, THROTTLE_CGC_EN, 0x1),
  ICBCFG_HWIOF_DW(GPU_BWMON_THROTTLE_1_GRANT_PERIOD, GRANT_PERIOD, 0x3e8),
  ICBCFG_HWIO_DW(GPU_BWMON_THROTTLE_1_THRESHOLD_03, 0xFFFFFFFF),
  ICBCFG_HWIO_DW(GPU_BWMON_THROTTLE_1_THRESHOLD_02, 0xFFFF0064),
  ICBCFG_HWIO_DW(GPU_BWMON_THROTTLE_1_THRESHOLD_01, 0xFFFFFFFF),
  ICBCFG_HWIO_DW(GPU_BWMON_THROTTLE_1_THRESHOLD_00, 0xFFFFFFFF),
  ICBCFG_HWIOF_DW(GPU_BWMON_THROTTLE_1_PEAK_ACCUM_CREDIT, PEAK_ACCUM_CREDIT, 0x100),
  ICBCFG_HWIO_DW(GPU_BWMON_THROTTLE_1_CNTRL, 0x0E01),
};

icbcfg_prop_type icbcfg_boot_prop_sdm1000_v1 =
{
  /* Length of the config  data array */
  ARRAY_SIZE(icbcfg_boot_data_sdm1000_v1),
  /* Pointer to config data array */
  icbcfg_boot_data_sdm1000_v1
};

icbcfg_data_type icbcfg_post_data_sdm1000_v1[] =
{
  /* Import from DDRSS SWC */
  ICBCFG_HWIO_DW(GEMNOC_HM_GLADIATOR0_CFGDUT_LOW, 0x0008D27F),
  ICBCFG_HWIO_DW(GEMNOC_HM_GLADIATOR0_ACECTL_HIGH, 0x00FF0101),
  ICBCFG_HWIO_DW(GEMNOC_HM_GLADIATOR0_ACE_SCE_STS_HIGH, 0x00000401),
  ICBCFG_HWIO_DW(GEMNOC_HM_GLADIATOR1_CFGDUT_LOW, 0x0008D27F),
  ICBCFG_HWIO_DW(GEMNOC_HM_GLADIATOR1_ACECTL_HIGH, 0x00FF0101),
  ICBCFG_HWIO_DW(GEMNOC_HM_GLADIATOR1_ACE_SCE_STS_HIGH, 0x00000401),
  ICBCFG_HWIO_DW(GEMNOC_HM_GLADIATOR2_CFGDUT_LOW, 0x0008D27F),
  ICBCFG_HWIO_DW(GEMNOC_HM_GLADIATOR2_ACECTL_HIGH, 0x00FF0101),
  ICBCFG_HWIO_DW(GEMNOC_HM_GLADIATOR2_ACE_SCE_STS_HIGH, 0x00000401),
  ICBCFG_HWIO_DW(GEMNOC_HM_GLADIATOR3_CFGDUT_LOW, 0x0008D27F),
  ICBCFG_HWIO_DW(GEMNOC_HM_GLADIATOR3_ACECTL_HIGH, 0x00FF0101),
  ICBCFG_HWIO_DW(GEMNOC_HM_GLADIATOR3_ACE_SCE_STS_HIGH, 0x00000401),
  ICBCFG_HWIO_DW(GEMNOC_HM_GPU_TCU_DYN_ATTR_CACHEINDEXVAL_LOW, 0x0000000B),
  ICBCFG_HWIO_DW(GEMNOC_HM_GPU_TCU_DYN_ATTR_MAINCTL_LOW, 0x00100000),
  ICBCFG_HWIO_DW(GEMNOC_HM_SYS_TCU_DYN_ATTR_CACHEINDEXVAL_LOW, 0x0000000D),
  ICBCFG_HWIO_DW(GEMNOC_HM_SYS_TCU_DYN_ATTR_MAINCTL_LOW, 0x00100000),

  /* ECC ports overrides (QCTDD04324786) */
  ICBCFG_HWIO_DW(GEMNOC_HM_ECC0_DYN_ATTR_ADDRMSBVAL_LOW, 0x0), /**< override bit 37 to 0. */
  ICBCFG_HWIO_DW(GEMNOC_HM_ECC0_DYN_ATTR_ADDRMSBOV_LOW,  0x2), /**< only override bit 37. */
  ICBCFG_HWIO_DW(GEMNOC_HM_ECC1_DYN_ATTR_ADDRMSBVAL_LOW, 0x0), /**< override bit 37 to 0. */
  ICBCFG_HWIO_DW(GEMNOC_HM_ECC1_DYN_ATTR_ADDRMSBOV_LOW,  0x2), /**< only override bit 37. */
};

icbcfg_prop_type icbcfg_post_prop_sdm1000_v1 =
{
  /* Length of the config  data array */
  ARRAY_SIZE(icbcfg_post_data_sdm1000_v1),
  /* Pointer to config data array */
  icbcfg_post_data_sdm1000_v1
};

icbcfg_mem_region_type map_ddr_regions_sdm1000_v1[] =
{
  { 0x080000000ULL, 0x400000000ULL },
  { 0x800000000ULL, 0x1000000000ULL },
};

/* Translation block base addresses. */
uint8 *trans_bases_sdm1000_v1[] =
{
  (uint8 *)HWIO_ADDR(LLCC0_LLCC_BEAC_ADDR_TRANSLATOR_CFG),
  (uint8 *)HWIO_ADDR(LLCC1_LLCC_BEAC_ADDR_TRANSLATOR_CFG),
  (uint8 *)HWIO_ADDR(LLCC2_LLCC_BEAC_ADDR_TRANSLATOR_CFG),
  (uint8 *)HWIO_ADDR(LLCC3_LLCC_BEAC_ADDR_TRANSLATOR_CFG),
  (uint8 *)HWIO_ADDR(LLCC4_LLCC_BEAC_ADDR_TRANSLATOR_CFG),
  (uint8 *)HWIO_ADDR(LLCC5_LLCC_BEAC_ADDR_TRANSLATOR_CFG),
  (uint8 *)HWIO_ADDR(LLCC6_LLCC_BEAC_ADDR_TRANSLATOR_CFG),
  (uint8 *)HWIO_ADDR(LLCC7_LLCC_BEAC_ADDR_TRANSLATOR_CFG),
};

icbcfg_device_config_type sdm1000_v1 =
{
  /* Chip version information for this device data. */
  .family  = CHIPINFO_FAMILY_SDM1000,     /**< Chip family */
  .match   = false,                       /**< Exact match for version? */
  .version = 0,                           /**< Chip version */

  /* Device information. */
  .map_ddr_region_count = ARRAY_SIZE(map_ddr_regions_sdm1000_v1), /**< Number of regions in the DDR map */
  .map_ddr_regions      = map_ddr_regions_sdm1000_v1,             /**< Array of mappable DDR regions */
  .num_segments         = 6,                                      /**< Number of mapping segments. */
  .num_channels         = ARRAY_SIZE(trans_bases_sdm1000_v1),     /**< Number of channels. */
  .addr_width           = 36,                                     /**< Address width in bits. */
  .trans_type           = ICBCFG_ADDR_TRANS_LLCC,                 /**< Address translator hardware type. */
  .trans_bases          = trans_bases_sdm1000_v1,                 /**< Translation block base addresses. */

  /* Prop lists. */
  .prop_data      = &icbcfg_boot_prop_sdm1000_v1, /**< Init time prop list. */
  .post_prop_data = &icbcfg_post_prop_sdm1000_v1, /**< Post-init time prop list. */
};

/*---------------------------------------------------------------------------*/
/* SDM1000 v2                                                                */
/*---------------------------------------------------------------------------*/
icbcfg_data_type icbcfg_post_data_sdm1000_v2[] =
{
  /* Import from DDRSS SWC */
  ICBCFG_HWIO_DW(V2_GEMNOC_HM_GLADIATOR0_CFGDUT_LOW, 0x0008D27F),
  ICBCFG_HWIO_DW(V2_GEMNOC_HM_GLADIATOR0_ACECTL_HIGH, 0x00FF0101),
  ICBCFG_HWIO_DW(V2_GEMNOC_HM_GLADIATOR0_ACE_SCE_STS_HIGH, 0x00000401),
  ICBCFG_HWIO_DW(V2_GEMNOC_HM_GLADIATOR1_CFGDUT_LOW, 0x0008D27F),
  ICBCFG_HWIO_DW(V2_GEMNOC_HM_GLADIATOR1_ACECTL_HIGH, 0x00FF0101),
  ICBCFG_HWIO_DW(V2_GEMNOC_HM_GLADIATOR1_ACE_SCE_STS_HIGH, 0x00000401),
  ICBCFG_HWIO_DW(V2_GEMNOC_HM_GLADIATOR2_CFGDUT_LOW, 0x0008D27F),
  ICBCFG_HWIO_DW(V2_GEMNOC_HM_GLADIATOR2_ACECTL_HIGH, 0x00FF0101),
  ICBCFG_HWIO_DW(V2_GEMNOC_HM_GLADIATOR2_ACE_SCE_STS_HIGH, 0x00000401),
  ICBCFG_HWIO_DW(V2_GEMNOC_HM_GLADIATOR3_CFGDUT_LOW, 0x0008D27F),
  ICBCFG_HWIO_DW(V2_GEMNOC_HM_GLADIATOR3_ACECTL_HIGH, 0x00FF0101),
  ICBCFG_HWIO_DW(V2_GEMNOC_HM_GLADIATOR3_ACE_SCE_STS_HIGH, 0x00000401),
  ICBCFG_HWIO_DW(V2_GEMNOC_HM_GPU_TCU_DYN_ATTR_CACHEINDEXVAL_LOW, 0x0000000B),
  ICBCFG_HWIO_DW(V2_GEMNOC_HM_GPU_TCU_DYN_ATTR_MAINCTL_LOW, 0x00100000),
  ICBCFG_HWIO_DW(V2_GEMNOC_HM_SYS_TCU_DYN_ATTR_CACHEINDEXVAL_LOW, 0x0000000D),
  ICBCFG_HWIO_DW(V2_GEMNOC_HM_SYS_TCU_DYN_ATTR_MAINCTL_LOW, 0x00100000),

  /* ECC ports overrides (QCTDD04324786) */
  ICBCFG_HWIO_DW(V2_GEMNOC_HM_ECC0_DYN_ATTR_ADDRMSBVAL_LOW, 0x0), /**< override bit 37 to 0. */
  ICBCFG_HWIO_DW(V2_GEMNOC_HM_ECC0_DYN_ATTR_ADDRMSBOV_LOW,  0x2), /**< only override bit 37. */
  ICBCFG_HWIO_DW(V2_GEMNOC_HM_ECC1_DYN_ATTR_ADDRMSBVAL_LOW, 0x0), /**< override bit 37 to 0. */
  ICBCFG_HWIO_DW(V2_GEMNOC_HM_ECC1_DYN_ATTR_ADDRMSBOV_LOW,  0x2), /**< only override bit 37. */

  /* LLCC overrides */
  ICBCFG_HWIO_DW(V2_LLCC_BROADCAST_LLCC_FEAC_NOALLOC_ON_MISS_OVERRIDE_CFG0, 0x2800),
};

icbcfg_prop_type icbcfg_post_prop_sdm1000_v2 =
{
  /* Length of the config  data array */
  ARRAY_SIZE(icbcfg_post_data_sdm1000_v2),
  /* Pointer to config data array */
  icbcfg_post_data_sdm1000_v2
};

icbcfg_device_config_type sdm1000_v2_8ch =
{
  /* Chip version information for this device data. */
  .family   = CHIPINFO_FAMILY_SDM1000, /**< Chip family */
  .match    =  false,                  /**< Exact match for version? */
  .version  = CHIPINFO_VERSION(2,0),   /**< Chip version */
  .reg_addr = (uint8 *)HWIO_ADDR(TCSR_TIMEOUT_INTR_STATUS), /**< Register or fuse. */
  .reg_mask = 0x2, /**< DDR_CH4_OR_CH8_SEL */
  .reg_val  = 0x0, /**< DDR_CH4_OR_CH8_SEL = 0 */
  
  /* Device information. */
  .map_ddr_region_count = ARRAY_SIZE(map_ddr_regions_sdm1000_v1), /**< Number of regions in the DDR map */
  .map_ddr_regions      = map_ddr_regions_sdm1000_v1,             /**< Array of mappable DDR regions */
  .num_segments         = 6,                                      /**< Number of mapping segments. */
  .num_channels         = ARRAY_SIZE(trans_bases_sdm1000_v1),     /**< Number of channels. */
  .addr_width           = 36,                                     /**< Address width in bits. */
  .trans_type           = ICBCFG_ADDR_TRANS_LLCC,                 /**< Address translator hardware type. */
  .trans_bases          = trans_bases_sdm1000_v1,                 /**< Translation block base addresses. */
  
  /* Prop lists. */
  .prop_data      = &icbcfg_boot_prop_sdm1000_v1,           /**< Init time prop list. */
  .post_prop_data = &icbcfg_post_prop_sdm1000_v2,           /**< Post-init time prop list. */
};

/*---------------------------------------------------------------------------*/
/* SDM1000 v2-4ch                                                            */
/*---------------------------------------------------------------------------*/
/* Translation block base addresses. */
uint8 *trans_bases_sdm1000_v2_4ch[] =
{
  (uint8 *)HWIO_ADDR(LLCC4_LLCC_BEAC_ADDR_TRANSLATOR_CFG),
  (uint8 *)HWIO_ADDR(LLCC5_LLCC_BEAC_ADDR_TRANSLATOR_CFG),
  (uint8 *)HWIO_ADDR(LLCC6_LLCC_BEAC_ADDR_TRANSLATOR_CFG),
  (uint8 *)HWIO_ADDR(LLCC7_LLCC_BEAC_ADDR_TRANSLATOR_CFG),
};

icbcfg_device_config_type sdm1000_v2_4ch =
{
  /* Chip version information for this device data. */
  .family   = CHIPINFO_FAMILY_SDM1000, /**< Chip family */
  .match    =  false,                  /**< Exact match for version? */
  .version  = CHIPINFO_VERSION(2,0),   /**< Chip version */
  .reg_addr = (uint8 *)HWIO_ADDR(TCSR_TIMEOUT_INTR_STATUS), /**< Register or fuse. */
  .reg_mask = 0x2, /**< DDR_CH4_OR_CH8_SEL */
  .reg_val  = 0x2, /**< DDR_CH4_OR_CH8_SEL = 1 */
  
  /* Device information. */
  .map_ddr_region_count = ARRAY_SIZE(map_ddr_regions_sdm1000_v1), /**< Number of regions in the DDR map */
  .map_ddr_regions      = map_ddr_regions_sdm1000_v1,             /**< Array of mappable DDR regions */
  .num_segments         = 6,                                      /**< Number of mapping segments. */
  .num_channels         = ARRAY_SIZE(trans_bases_sdm1000_v2_4ch), /**< Number of channels. */
  .addr_width           = 36,                                     /**< Address width in bits. */
  .trans_type           = ICBCFG_ADDR_TRANS_LLCC,                 /**< Address translator hardware type. */
  .trans_bases          = trans_bases_sdm1000_v2_4ch,             /**< Translation block base addresses. */
  
  /* Prop lists. */
  .prop_data      = &icbcfg_boot_prop_sdm1000_v1,           /**< Init time prop list. */
  .post_prop_data = &icbcfg_post_prop_sdm1000_v2,           /**< Post-init time prop list. */
};

/*---------------------------------------------------------------------------*/
/* TARGET LIST                                                               */
/*---------------------------------------------------------------------------*/
icbcfg_device_config_type *configs_sdm1000[] =
{
  [0] = &sdm1000_v2_8ch,
  [1] = &sdm1000_v2_4ch,
  [2] = &sdm1000_v1,
};

/*---------------------------------------------------------------------------*/
/* EXPORTED TARGET DEFINITION                                                */
/*---------------------------------------------------------------------------*/
icbcfg_info_type icbcfg_info =
{
  .num_configs = ARRAY_SIZE(configs_sdm1000),
  .configs     = configs_sdm1000,
};
