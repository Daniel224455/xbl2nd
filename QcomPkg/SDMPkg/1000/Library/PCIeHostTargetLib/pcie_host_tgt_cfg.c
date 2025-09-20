/**
  @file pcie_tgt_cfg.c
  @brief Internal interface include file for the PCIe target specific
  configuration layer.

  This file contains definitions of constants, data structures, and
  interfaces that provide target specific configuration data.

*/
/*
===============================================================================

                             Edit History


when       who     what, where, why
--------   ---     ------------------------------------------------------------
10/18/18   MK      Created

===============================================================================
                   Copyright (c) 2018 QUALCOMM Technologies Incorporated
                          All Rights Reserved.
                        Qualcomm Confidential and Proprietary.
===============================================================================
*/
#include "com_dtypes.h"
#include "pcie_host_tgt_cfg.h"
#include "TlmmDefs.h"
#include "api/pmic/pm/pm_lib.h"
#include "msmhwiobase.h"
#include "pcie_hwio.h"
#include <HALhwio.h>

#define PCIE_CLASS_CODE_BRIDGE_DEVICE       (0x06000000)
#define PCIE_SUBCLASS_CODE_PCI_2_PCI_BRIDGE (0x00040000)

#ifdef PRIMARY_NVME_ON_PCI2
pcie_host_ldo_config_t pcie_host_rc2_ldos[]={
    {PMIC_C, PM_LDO_3},
    {PMIC_E, PM_LDO_5}
};

const pcie_host_pmic_config_t pcie_host_rc2_pmic_config =
{
    .num_ldos = sizeof(pcie_host_rc2_ldos)/sizeof(pcie_host_ldo_config_t),
    .ldo_cfg = pcie_host_rc2_ldos
};

pcie_host_gpio_config_t pcie_host_rc2_gpio_config[] =
{
  {
   .type = MSM_GPIO,
   {
    .tlmm_gpio_config.gpio_num     = 176,// CLK_REQ
    .tlmm_gpio_config.func         = 1,
    .tlmm_gpio_config.dir          = DAL_GPIO_INPUT,
    .tlmm_gpio_config.pull         = DAL_GPIO_PULL_UP,
    .tlmm_gpio_config.drv_strength = DAL_GPIO_2MA,
    .tlmm_gpio_config.value        = DAL_GPIO_LOW_VALUE
   }
  }
  ,
  {
   .type = MSM_GPIO,
   {
    .tlmm_gpio_config.gpio_num     = 102, // PCIE2_RST
    .tlmm_gpio_config.func         = 0,
    .tlmm_gpio_config.dir          = DAL_GPIO_OUTPUT,
    .tlmm_gpio_config.pull         = DAL_GPIO_NO_PULL,
    .tlmm_gpio_config.drv_strength = DAL_GPIO_2MA,
    .tlmm_gpio_config.value        = DAL_GPIO_LOW_VALUE
   }
  }
  ,
  {
   .type = MSM_GPIO,
   {
    .tlmm_gpio_config.gpio_num     = 104, // PCIE_WAKE
    .tlmm_gpio_config.func         = 0,
    .tlmm_gpio_config.dir          = DAL_GPIO_INPUT,
    .tlmm_gpio_config.pull         = DAL_GPIO_NO_PULL,
    .tlmm_gpio_config.drv_strength = DAL_GPIO_2MA,
    .tlmm_gpio_config.value        = DAL_GPIO_LOW_VALUE
   }
  }
  ,
  {
   .type = DELAY,
   {
    .delay_us = 150 // 150 micro seconds before REFCLK gets stable
   }
  }
  ,
  {
   .type = MSM_GPIO,
   {
    .tlmm_gpio_config.gpio_num     = 102, // PCIE1_RST
    .tlmm_gpio_config.func         = 0,
    .tlmm_gpio_config.dir          = DAL_GPIO_OUTPUT,
    .tlmm_gpio_config.pull         = DAL_GPIO_NO_PULL,
    .tlmm_gpio_config.drv_strength = DAL_GPIO_2MA,
    .tlmm_gpio_config.value        = DAL_GPIO_HIGH_VALUE
   }
  }
  ,
  {
   .type = DELAY,
   {
    .delay_us = 10000 // 10 ms delay for EP device preparation for Link Training
   }
  }
  ,
  /* Default and end of sequence entry */
  {
   .type = NONE,
   {
    .delay_us = NONE
   }
  }
};

pcie_host_mem_region_t pcie_host_rc2_memreg[] =
{
  /**< axi_reg: DBI register space */
  {.pa=PCIE_GEN3X4_PCIE_WRAPPER_AXI_NO_EDMA_BASE, .sz=PCIE_GEN3X4_PCIE_WRAPPER_AXI_NO_EDMA_BASE_SIZE},
  /**< ahb_reg: PARF register space */
  {.pa=PCIE_GEN3X4_PCIE_WRAPPER_AHB_W_PHY_G3X4_BASE, .sz=PCIE_GEN3X4_PCIE_WRAPPER_AHB_W_PHY_G3X4_BASE_SIZE},
  /**< tcsr_reg: Access TCSR register space */
  {.pa=CORE_TOP_CSR_BASE, .sz=CORE_TOP_CSR_BASE_SIZE},
  /**< refgen_reg: Access TCSR register space */
  {.pa=0x88E5000, .sz=0x2000},
  /**< invalid_reg: Invalid reg base to end the PHY init sequence */
  {.pa=0xFFFFFFFFUL, .sz=0x0UL}
};

/*******************************************************************************
 * V2 Chipset specific port 2 PHY init sequence
 ******************************************************************************/
static const pcie_host_phy_seq_t pcie_host_rc2_v2_init_seq[] =
{
  { tcsr_reg,    PHY_REG_WRI, HWIO_OFFS(TCSR_PCIEPHY_LINK_CONFIG)                                      , 0x00, 0x01 },
  /* Register Base, Operation, Offset, Mask & Value */
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_PCIE_PARF_DEVICE_TYPE)                             , 0x00, 0x04 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_APCS_COM_POWER_DOWN_CONTROL)               , 0x00, 0x03 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_PLL_SSC_EN_CENTER)                , 0x00, 0x00 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_PLL_SSC_PER1)                     , 0x00, 0x31 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_PLL_SSC_PER2)                     , 0x00, 0x01 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_PLL_SSC_STEP_SIZE1_MODE0)         , 0x00, 0xDE },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_PLL_SSC_STEP_SIZE2_MODE0)         , 0x00, 0x07 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_PLL_SSC_STEP_SIZE1_MODE1)         , 0x00, 0x4C },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_PLL_SSC_STEP_SIZE2_MODE1)         , 0x00, 0x06 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_PLL_BIAS_EN_CLKBUFLR_EN)          , 0x00, 0x1C },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_PLL_CLK_ENABLE1)                  , 0x00, 0x90 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_PLL_PLL_IVCO)                     , 0x00, 0x0F },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_PLL_CP_CTRL_MODE0)                , 0x00, 0x06 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_PLL_CP_CTRL_MODE1)                , 0x00, 0x06 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_PLL_PLL_RCTRL_MODE0)              , 0x00, 0x16 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_PLL_PLL_RCTRL_MODE1)              , 0x00, 0x16 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_PLL_PLL_CCTRL_MODE0)              , 0x00, 0x36 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_PLL_PLL_CCTRL_MODE1)              , 0x00, 0x36 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_PLL_SYSCLK_EN_SEL)                , 0x00, 0x08 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_PLL_LOCK_CMP_EN)                  , 0x00, 0x42 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_PLL_LOCK_CMP1_MODE0)              , 0x00, 0x0A },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_PLL_LOCK_CMP2_MODE0)              , 0x00, 0x1A },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_PLL_LOCK_CMP1_MODE1)              , 0x00, 0x14 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_PLL_LOCK_CMP2_MODE1)              , 0x00, 0x34 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_PLL_DEC_START_MODE0)              , 0x00, 0x82 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_PLL_DEC_START_MODE1)              , 0x00, 0x68 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_PLL_DIV_FRAC_START1_MODE0)        , 0x00, 0x55 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_PLL_DIV_FRAC_START2_MODE0)        , 0x00, 0x55 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_PLL_DIV_FRAC_START3_MODE0)        , 0x00, 0x03 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_PLL_DIV_FRAC_START1_MODE1)        , 0x00, 0xAB },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_PLL_DIV_FRAC_START2_MODE1)        , 0x00, 0xAA },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_PLL_DIV_FRAC_START3_MODE1)        , 0x00, 0x02 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_PLL_VCO_TUNE_MAP)                 , 0x00, 0x02 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_PLL_VCO_TUNE1_MODE0)              , 0x00, 0x24 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_PLL_VCO_TUNE1_MODE1)              , 0x00, 0xB4 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_PLL_VCO_TUNE2_MODE1)              , 0x00, 0x03 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_PLL_CLK_SELECT)                   , 0x00, 0x34 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_PLL_HSCLK_SEL)                    , 0x00, 0x01 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_PLL_CORECLK_DIV_MODE1)            , 0x00, 0x08 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_PLL_BIN_VCOCAL_CMP_CODE1_MODE0)   , 0x00, 0xB9 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_PLL_BIN_VCOCAL_CMP_CODE2_MODE0)   , 0x00, 0x1E },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_PLL_BIN_VCOCAL_CMP_CODE1_MODE1)   , 0x00, 0x94 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_PLL_BIN_VCOCAL_CMP_CODE2_MODE1)   , 0x00, 0x18 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_PLL_BIN_VCOCAL_HSCLK_SEL)         , 0x00, 0x11 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_TX0_RES_CODE_LANE_OFFSET_TX)      , 0x00, 0x11 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_TX0_LANE_MODE_1)                  , 0x00, 0x35 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_TX0_RCV_DETECT_LVL_2)             , 0x00, 0x12 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX0_UCDR_FO_GAIN)                 , 0x00, 0x0C },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX0_UCDR_SO_GAIN)                 , 0x00, 0x03 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX0_UCDR_SO_SATURATION_AND_ENABLE), 0x00, 0x7F },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX0_UCDR_PI_CONTROLS)             , 0x00, 0x70 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX0_AUX_DATA_TCOARSE_TFINE)       , 0x00, 0x30 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX0_RCLK_AUXDATA_SEL)             , 0x00, 0x00 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX0_VGA_CAL_CNTRL1)               , 0x00, 0x04 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX0_VGA_CAL_CNTRL2)               , 0x00, 0x07 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX0_GM_CAL)                       , 0x00, 0x0D },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX0_RX_EQU_ADAPTOR_CNTRL1)        , 0x00, 0x00 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX0_RX_EQU_ADAPTOR_CNTRL2)        , 0x00, 0x0E },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX0_RX_EQU_ADAPTOR_CNTRL3)        , 0x00, 0x4A },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX0_RX_EQU_ADAPTOR_CNTRL4)        , 0x00, 0x0F },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX0_RX_IDAC_TSETTLE_LOW)          , 0x00, 0xC0 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX0_RX_IDAC_TSETTLE_HIGH)         , 0x00, 0x00 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX0_RX_EQ_OFFSET_ADAPTOR_CNTRL1)  , 0x00, 0x17 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX0_SIGDET_ENABLES)               , 0x00, 0x1C },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX0_SIGDET_CNTRL)                 , 0x00, 0x03 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX0_SIGDET_DEGLITCH_CNTRL)        , 0x00, 0x1E },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX0_RX_MODE_00_LOW)               , 0x00, 0xFF },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX0_RX_MODE_00_HIGH)              , 0x00, 0xFF },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX0_RX_MODE_00_HIGH2)             , 0x00, 0xFF },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX0_RX_MODE_00_HIGH3)             , 0x00, 0x7F },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX0_RX_MODE_00_HIGH4)             , 0x00, 0x66 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX0_RX_MODE_01_LOW)               , 0x00, 0x24 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX0_RX_MODE_01_HIGH)              , 0x00, 0xE4 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX0_RX_MODE_01_HIGH2)             , 0x00, 0xEC },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX0_RX_MODE_01_HIGH3)             , 0x00, 0x3B },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX0_RX_MODE_01_HIGH4)             , 0x00, 0x36 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX0_RX_MODE_10_LOW)               , 0x00, 0xD4 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX0_RX_MODE_10_HIGH)              , 0x00, 0x54 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX0_RX_MODE_10_HIGH2)             , 0x00, 0xDB },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX0_RX_MODE_10_HIGH3)             , 0x00, 0x3B },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX0_RX_MODE_10_HIGH4)             , 0x00, 0x31 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX0_DCC_CTRL1)                    , 0x00, 0x0C },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_TX1_RES_CODE_LANE_OFFSET_TX)      , 0x00, 0x11 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_TX1_LANE_MODE_1)                  , 0x00, 0x35 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_TX1_RCV_DETECT_LVL_2)             , 0x00, 0x12 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX1_UCDR_FO_GAIN)                 , 0x00, 0x0C },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX1_UCDR_SO_GAIN)                 , 0x00, 0x03 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX1_UCDR_SO_SATURATION_AND_ENABLE), 0x00, 0x7F },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX1_UCDR_PI_CONTROLS)             , 0x00, 0x70 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX1_AUX_DATA_TCOARSE_TFINE)       , 0x00, 0x30 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX1_RCLK_AUXDATA_SEL)             , 0x00, 0x00 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX1_VGA_CAL_CNTRL1)               , 0x00, 0x04 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX1_VGA_CAL_CNTRL2)               , 0x00, 0x07 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX1_GM_CAL)                       , 0x00, 0x0D },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX1_RX_EQU_ADAPTOR_CNTRL1)        , 0x00, 0x00 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX1_RX_EQU_ADAPTOR_CNTRL2)        , 0x00, 0x0E },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX1_RX_EQU_ADAPTOR_CNTRL3)        , 0x00, 0x4A },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX1_RX_EQU_ADAPTOR_CNTRL4)        , 0x00, 0x0F },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX1_RX_IDAC_TSETTLE_LOW)          , 0x00, 0xC0 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX1_RX_IDAC_TSETTLE_HIGH)         , 0x00, 0x00 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX1_RX_EQ_OFFSET_ADAPTOR_CNTRL1)  , 0x00, 0x17 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX1_SIGDET_ENABLES)               , 0x00, 0x1C },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX1_SIGDET_CNTRL)                 , 0x00, 0x03 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX1_SIGDET_DEGLITCH_CNTRL)        , 0x00, 0x1E },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX1_RX_MODE_00_LOW)               , 0x00, 0xFF },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX1_RX_MODE_00_HIGH)              , 0x00, 0xFF },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX1_RX_MODE_00_HIGH2)             , 0x00, 0xFF },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX1_RX_MODE_00_HIGH3)             , 0x00, 0x7F },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX1_RX_MODE_00_HIGH4)             , 0x00, 0x66 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX1_RX_MODE_01_LOW)               , 0x00, 0x24 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX1_RX_MODE_01_HIGH)              , 0x00, 0xE4 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX1_RX_MODE_01_HIGH2)             , 0x00, 0xEC },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX1_RX_MODE_01_HIGH3)             , 0x00, 0x3B },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX1_RX_MODE_01_HIGH4)             , 0x00, 0x36 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX1_RX_MODE_10_LOW)               , 0x00, 0xD4 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX1_RX_MODE_10_HIGH)              , 0x00, 0x54 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX1_RX_MODE_10_HIGH2)             , 0x00, 0xDB },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX1_RX_MODE_10_HIGH3)             , 0x00, 0x3B },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX1_RX_MODE_10_HIGH4)             , 0x00, 0x31 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX1_DCC_CTRL1)                    , 0x00, 0x0C },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_APCS_COM_REFGEN_REQ_CONFIG1)               , 0x00, 0x05 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_APCS_COM_RX_SIGDET_LVL)                    , 0x00, 0x88 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_APCS_COM_RATE_SLEW_CNTRL1)                 , 0x00, 0x0B },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_APCS_COM_P2U3_WAKEUP_DLY_TIME_AUXCLK_L)    , 0x00, 0x01 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_APCS_COM_EQ_CONFIG5)                       , 0x00, 0x01 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_APCS_PCIE_POWER_STATE_CONFIG2)             , 0x00, 0x0D },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_APCS_PCIE_POWER_STATE_CONFIG4)             , 0x00, 0x07 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_APCS_PCIE_ENDPOINT_REFCLK_DRIVE)           , 0x00, 0xC1 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_APCS_PCIE_L1P1_WAKEUP_DLY_TIME_AUXCLK_L)   , 0x00, 0x01 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_APCS_PCIE_L1P2_WAKEUP_DLY_TIME_AUXCLK_L)   , 0x00, 0x01 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_APCS_PCIE_OSC_DTCT_ACTIONS)                , 0x00, 0x00 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_APCS_PCIE_PRESET_P6_P7_PRE)                , 0x00, 0x33 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_APCS_PCIE_PRESET_P10_PRE)                  , 0x00, 0x00 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_APCS_PCIE_PRESET_P10_POST)                 , 0x00, 0x58 },

  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_PLL_SSC_EN_CENTER)                , 0x00, 0x00 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_PLL_SSC_PER1)                     , 0x00, 0x31 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_PLL_SSC_PER2)                     , 0x00, 0x01 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_PLL_SSC_STEP_SIZE1_MODE0)         , 0x00, 0xDE },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_PLL_SSC_STEP_SIZE2_MODE0)         , 0x00, 0x07 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_PLL_SSC_STEP_SIZE1_MODE1)         , 0x00, 0x4C },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_PLL_SSC_STEP_SIZE2_MODE1)         , 0x00, 0x06 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_PLL_BIAS_EN_CLKBUFLR_EN)          , 0x00, 0x1C },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_PLL_CLK_ENABLE1)                  , 0x00, 0x90 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_PLL_PLL_IVCO)                     , 0x00, 0x0F },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_PLL_CP_CTRL_MODE0)                , 0x00, 0x06 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_PLL_CP_CTRL_MODE1)                , 0x00, 0x06 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_PLL_PLL_RCTRL_MODE0)              , 0x00, 0x16 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_PLL_PLL_RCTRL_MODE1)              , 0x00, 0x16 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_PLL_PLL_CCTRL_MODE0)              , 0x00, 0x36 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_PLL_PLL_CCTRL_MODE1)              , 0x00, 0x36 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_PLL_SYSCLK_EN_SEL)                , 0x00, 0x08 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_PLL_LOCK_CMP_EN)                  , 0x00, 0x42 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_PLL_LOCK_CMP1_MODE0)              , 0x00, 0x0A },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_PLL_LOCK_CMP2_MODE0)              , 0x00, 0x1A },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_PLL_LOCK_CMP1_MODE1)              , 0x00, 0x14 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_PLL_LOCK_CMP2_MODE1)              , 0x00, 0x34 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_PLL_DEC_START_MODE0)              , 0x00, 0x82 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_PLL_DEC_START_MODE1)              , 0x00, 0x68 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_PLL_DIV_FRAC_START1_MODE0)        , 0x00, 0x55 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_PLL_DIV_FRAC_START2_MODE0)        , 0x00, 0x55 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_PLL_DIV_FRAC_START3_MODE0)        , 0x00, 0x03 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_PLL_DIV_FRAC_START1_MODE1)        , 0x00, 0xAB },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_PLL_DIV_FRAC_START2_MODE1)        , 0x00, 0xAA },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_PLL_DIV_FRAC_START3_MODE1)        , 0x00, 0x02 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_PLL_VCO_TUNE_MAP)                 , 0x00, 0x02 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_PLL_VCO_TUNE1_MODE0)              , 0x00, 0x24 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_PLL_VCO_TUNE1_MODE1)              , 0x00, 0xB4 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_PLL_VCO_TUNE2_MODE1)              , 0x00, 0x03 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_PLL_CLK_SELECT)                   , 0x00, 0x34 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_PLL_HSCLK_SEL)                    , 0x00, 0x01 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_PLL_CORECLK_DIV_MODE1)            , 0x00, 0x08 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_PLL_BIN_VCOCAL_CMP_CODE1_MODE0)   , 0x00, 0xB9 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_PLL_BIN_VCOCAL_CMP_CODE2_MODE0)   , 0x00, 0x1E },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_PLL_BIN_VCOCAL_CMP_CODE1_MODE1)   , 0x00, 0x94 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_PLL_BIN_VCOCAL_CMP_CODE2_MODE1)   , 0x00, 0x18 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_PLL_BIN_VCOCAL_HSCLK_SEL)         , 0x00, 0x11 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_TX0_RES_CODE_LANE_OFFSET_TX)      , 0x00, 0x11 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_TX0_LANE_MODE_1)                  , 0x00, 0x35 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_TX0_RCV_DETECT_LVL_2)             , 0x00, 0x12 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_RX0_UCDR_FO_GAIN)                 , 0x00, 0x0C },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_RX0_UCDR_SO_GAIN)                 , 0x00, 0x03 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_RX0_UCDR_SO_SATURATION_AND_ENABLE), 0x00, 0x7F },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_RX0_UCDR_PI_CONTROLS)             , 0x00, 0x70 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_RX0_AUX_DATA_TCOARSE_TFINE)       , 0x00, 0x30 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_RX0_RCLK_AUXDATA_SEL)             , 0x00, 0x00 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_RX0_VGA_CAL_CNTRL1)               , 0x00, 0x04 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_RX0_VGA_CAL_CNTRL2)               , 0x00, 0x07 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_RX0_GM_CAL)                       , 0x00, 0x0D },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_RX0_RX_EQU_ADAPTOR_CNTRL1)        , 0x00, 0x00 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_RX0_RX_EQU_ADAPTOR_CNTRL2)        , 0x00, 0x0E },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_RX0_RX_EQU_ADAPTOR_CNTRL3)        , 0x00, 0x4A },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_RX0_RX_EQU_ADAPTOR_CNTRL4)        , 0x00, 0x0F },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_RX0_RX_IDAC_TSETTLE_LOW)          , 0x00, 0xC0 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_RX0_RX_IDAC_TSETTLE_HIGH)         , 0x00, 0x00 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_RX0_RX_EQ_OFFSET_ADAPTOR_CNTRL1)  , 0x00, 0x17 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_RX0_SIGDET_ENABLES)               , 0x00, 0x1C },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_RX0_SIGDET_CNTRL)                 , 0x00, 0x03 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_RX0_SIGDET_DEGLITCH_CNTRL)        , 0x00, 0x1E },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_RX0_RX_MODE_00_LOW)               , 0x00, 0xFF },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_RX0_RX_MODE_00_HIGH)              , 0x00, 0xFF },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_RX0_RX_MODE_00_HIGH2)             , 0x00, 0xFF },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_RX0_RX_MODE_00_HIGH3)             , 0x00, 0x7F },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_RX0_RX_MODE_00_HIGH4)             , 0x00, 0x66 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_RX0_RX_MODE_01_LOW)               , 0x00, 0x24 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_RX0_RX_MODE_01_HIGH)              , 0x00, 0xE4 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_RX0_RX_MODE_01_HIGH2)             , 0x00, 0xEC },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_RX0_RX_MODE_01_HIGH3)             , 0x00, 0x3B },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_RX0_RX_MODE_01_HIGH4)             , 0x00, 0x36 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_RX0_RX_MODE_10_LOW)               , 0x00, 0xD4 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_RX0_RX_MODE_10_HIGH)              , 0x00, 0x54 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_RX0_RX_MODE_10_HIGH2)             , 0x00, 0xDB },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_RX0_RX_MODE_10_HIGH3)             , 0x00, 0x3B },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_RX0_RX_MODE_10_HIGH4)             , 0x00, 0x31 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_RX0_DCC_CTRL1)                    , 0x00, 0x0C },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_TX1_RES_CODE_LANE_OFFSET_TX)      , 0x00, 0x11 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_TX1_LANE_MODE_1)                  , 0x00, 0x35 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_TX1_RCV_DETECT_LVL_2)             , 0x00, 0x12 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_RX1_UCDR_FO_GAIN)                 , 0x00, 0x0C },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_RX1_UCDR_SO_GAIN)                 , 0x00, 0x03 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_RX1_UCDR_SO_SATURATION_AND_ENABLE), 0x00, 0x7F },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_RX1_UCDR_PI_CONTROLS)             , 0x00, 0x70 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_RX1_AUX_DATA_TCOARSE_TFINE)       , 0x00, 0x30 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_RX1_RCLK_AUXDATA_SEL)             , 0x00, 0x00 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_RX1_VGA_CAL_CNTRL1)               , 0x00, 0x04 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_RX1_VGA_CAL_CNTRL2)               , 0x00, 0x07 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_RX1_GM_CAL)                       , 0x00, 0x0D },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_RX1_RX_EQU_ADAPTOR_CNTRL1)        , 0x00, 0x00 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_RX1_RX_EQU_ADAPTOR_CNTRL2)        , 0x00, 0x0E },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_RX1_RX_EQU_ADAPTOR_CNTRL3)        , 0x00, 0x4A },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_RX1_RX_EQU_ADAPTOR_CNTRL4)        , 0x00, 0x0F },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_RX1_RX_IDAC_TSETTLE_LOW)          , 0x00, 0xC0 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_RX1_RX_IDAC_TSETTLE_HIGH)         , 0x00, 0x00 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_RX1_RX_EQ_OFFSET_ADAPTOR_CNTRL1)  , 0x00, 0x17 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_RX1_SIGDET_ENABLES)               , 0x00, 0x1C },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_RX1_SIGDET_CNTRL)                 , 0x00, 0x03 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_RX1_SIGDET_DEGLITCH_CNTRL)        , 0x00, 0x1E },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_RX1_RX_MODE_00_LOW)               , 0x00, 0xFF },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_RX1_RX_MODE_00_HIGH)              , 0x00, 0xFF },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_RX1_RX_MODE_00_HIGH2)             , 0x00, 0xFF },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_RX1_RX_MODE_00_HIGH3)             , 0x00, 0x7F },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_RX1_RX_MODE_00_HIGH4)             , 0x00, 0x66 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_RX1_RX_MODE_01_LOW)               , 0x00, 0x24 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_RX1_RX_MODE_01_HIGH)              , 0x00, 0xE4 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_RX1_RX_MODE_01_HIGH2)             , 0x00, 0xEC },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_RX1_RX_MODE_01_HIGH3)             , 0x00, 0x3B },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_RX1_RX_MODE_01_HIGH4)             , 0x00, 0x36 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_RX1_RX_MODE_10_LOW)               , 0x00, 0xD4 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_RX1_RX_MODE_10_HIGH)              , 0x00, 0x54 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_RX1_RX_MODE_10_HIGH2)             , 0x00, 0xDB },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_RX1_RX_MODE_10_HIGH3)             , 0x00, 0x3B },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_RX1_RX_MODE_10_HIGH4)             , 0x00, 0x31 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BQSERDES_RX1_DCC_CTRL1)                    , 0x00, 0x0C },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BPCS_COM_REFGEN_REQ_CONFIG1)               , 0x00, 0x05 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BPCS_COM_RX_SIGDET_LVL)                    , 0x00, 0x88 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BPCS_COM_RATE_SLEW_CNTRL1)                 , 0x00, 0x0B },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BPCS_COM_P2U3_WAKEUP_DLY_TIME_AUXCLK_L)    , 0x00, 0x01 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BPCS_COM_EQ_CONFIG5)                       , 0x00, 0x01 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BPCS_PCIE_POWER_STATE_CONFIG2)             , 0x00, 0x0D },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BPCS_PCIE_POWER_STATE_CONFIG4)             , 0x00, 0x07 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BPCS_PCIE_ENDPOINT_REFCLK_DRIVE)           , 0x00, 0xC1 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BPCS_PCIE_L1P1_WAKEUP_DLY_TIME_AUXCLK_L)   , 0x00, 0x01 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BPCS_PCIE_L1P2_WAKEUP_DLY_TIME_AUXCLK_L)   , 0x00, 0x01 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BPCS_PCIE_OSC_DTCT_ACTIONS)                , 0x00, 0x00 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BPCS_PCIE_PRESET_P6_P7_PRE)                , 0x00, 0x33 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BPCS_PCIE_PRESET_P10_PRE)                  , 0x00, 0x00 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_BPCS_PCIE_PRESET_P10_POST)                 , 0x00, 0x58 },

  { invalid_reg, PHY_REG_NOP, 0xFF, 0xFF, 0xFF },
};

/*******************************************************************************
 * Chipset specific version independent port 2 PHY power ON sequence
 ******************************************************************************/
STATIC CONST pcie_host_phy_seq_t pcie_host_rc2_pwron_seq[] =
{
  /* Register Base, Operation, Offset, Mask & Value */
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_APCS_COM_SW_RESET                         ), 0x00, 0x00 }, // Release software reset
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_APCS_COM_START_CONTROL                    ), 0x00, 0x03 }, // Start PCS and Serdes power SMs
  { invalid_reg, PHY_REG_NOP, 0xFF, 0xFF, 0xFF },
};

STATIC CONST pcie_host_phy_seq_t pcie_host_rc2_phy_delay_seq[] =
{
  /* Register Base, Operation, Offset, Mask & Value */
  { invalid_reg, PHY_REG_DELAY, 0xFF, 0xFF, 2000 },
  { invalid_reg, PHY_REG_NOP, 0xFF, 0xFF, 0xFF },
};

/*******************************************************************************
 * poll PHY power ON status of port 1 PHY
 ******************************************************************************/
STATIC CONST pcie_host_phy_seq_t pcie_host_rc2_phy_poll_seq[] =
{
  /* Register Base, Operation, Offset, Mask & Value */
  { ahb_reg,     PHY_REG_REA, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_APCS_COM_PCS_STATUS1), HWIO_PCIE_GEN3X4_CM_PCIE_APCS_COM_PCS_STATUS1_PHYSTATUS_BMSK, FALSE },
  { invalid_reg, PHY_REG_NOP, 0xFF, 0xFF, 0xFF },
};

/*******************************************************************************
 * post PHY power ON sequence for port 1 GEN3 PHY
 ******************************************************************************/
STATIC CONST pcie_host_phy_seq_t pcie_host_rc2_postpwrup_seq[] =
{
  /* Register Base, Operation, Offset, Mask & Value */
  /* Enable BME & MSE before LTSSM_EN */
  { axi_reg,     PHY_REG_RMW, HWIO_OFFS(PCIE_GEN3X4_TYPE1_STATUS_COMMAND_REG), HWIO_PCIE_GEN3X4_TYPE1_STATUS_COMMAND_REG_MSE_BMSK, (0x1 << HWIO_PCIE_GEN3X4_TYPE1_STATUS_COMMAND_REG_MSE_SHFT)},
  { axi_reg,     PHY_REG_RMW, HWIO_OFFS(PCIE_GEN3X4_TYPE1_STATUS_COMMAND_REG), HWIO_PCIE_GEN3X4_TYPE1_STATUS_COMMAND_REG_BME_BMSK, (0x1 << HWIO_PCIE_GEN3X4_TYPE1_STATUS_COMMAND_REG_BME_SHFT)},
  /* Writing Slave address space size */
  { ahb_reg,    PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_PCIE_PARF_SLV_ADDR_SPACE_SIZE), 0x0, 0x2000000},
  { ahb_reg,    PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_PCIE_PARF_SLV_ADDR_SPACE_SIZE_HI), 0x0, 0x00000000},
  { ahb_reg,    PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_PCIE_PARF_BDF_TO_SID_CFG), 0x0, 0x1},
  /* Clear REQ_NOT_ENTER_L1 field in PARF PM_CTRL to allow L1 entry */
  { ahb_reg,    PHY_REG_RMW, HWIO_OFFS(PCIE_GEN3X4_PCIE_PARF_PM_CTRL), HWIO_PCIE_GEN3X4_PCIE_PARF_PM_CTRL_REQ_NOT_ENTR_L1_BMSK, 0x00000000},
  { axi_reg,     PHY_REG_RMW, HWIO_OFFS(PCIE_GEN3X4_GEN3_EQ_FB_MODE_DIR_CHANGE_OFF), HWIO_PCIE_GEN3X4_GEN3_EQ_FB_MODE_DIR_CHANGE_OFF_GEN3_EQ_FMDC_N_EVALS_BMSK, (0x0D << HWIO_PCIE_GEN3X4_GEN3_EQ_FB_MODE_DIR_CHANGE_OFF_GEN3_EQ_FMDC_N_EVALS_SHFT)},
  { axi_reg,     PHY_REG_RMW, HWIO_OFFS(PCIE_GEN3X4_GEN3_EQ_FB_MODE_DIR_CHANGE_OFF), HWIO_PCIE_GEN3X4_GEN3_EQ_FB_MODE_DIR_CHANGE_OFF_GEN3_EQ_FMDC_MAX_PRE_CUSROR_DELTA_BMSK, (0x05 << HWIO_PCIE_GEN3X4_GEN3_EQ_FB_MODE_DIR_CHANGE_OFF_GEN3_EQ_FMDC_MAX_PRE_CUSROR_DELTA_SHFT)},
  { axi_reg,     PHY_REG_RMW, HWIO_OFFS(PCIE_GEN3X4_GEN3_EQ_FB_MODE_DIR_CHANGE_OFF), HWIO_PCIE_GEN3X4_GEN3_EQ_FB_MODE_DIR_CHANGE_OFF_GEN3_EQ_FMDC_MAX_POST_CUSROR_DELTA_BMSK, (0x05 << HWIO_PCIE_GEN3X4_GEN3_EQ_FB_MODE_DIR_CHANGE_OFF_GEN3_EQ_FMDC_MAX_POST_CUSROR_DELTA_SHFT)},
  { axi_reg,     PHY_REG_RMW, HWIO_OFFS(PCIE_GEN3X4_GEN3_EQ_FB_MODE_DIR_CHANGE_OFF), HWIO_PCIE_GEN3X4_GEN3_EQ_FB_MODE_DIR_CHANGE_OFF_GEN3_EQ_FMDC_T_MIN_PHASE23_BMSK, 0x00 },
  { axi_reg,     PHY_REG_RMW, HWIO_OFFS(PCIE_GEN3X4_GEN3_EQ_CONTROL_OFF), HWIO_PCIE_GEN3X4_GEN3_EQ_CONTROL_OFF_GEN3_EQ_PHASE23_EXIT_MODE_BMSK, 0x00 },
  { axi_reg,     PHY_REG_RMW, HWIO_OFFS(PCIE_GEN3X4_GEN3_EQ_CONTROL_OFF), HWIO_PCIE_GEN3X4_GEN3_EQ_CONTROL_OFF_GEN3_EQ_PSET_REQ_VEC_BMSK, 0x00 },
  /* Enable DBI_RO_WR_EN to access CS1 region */
  { axi_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_MISC_CONTROL_1_OFF), 0x0, 0x1},
  /* Memory Barrier */
  { invalid_reg, PHY_REG_DSB, 0xFF, 0xFF, 0xFF},
  /* Update Base class and Sub class */
  { axi_reg,     PHY_REG_RMW, HWIO_OFFS(PCIE_GEN3X4_TYPE1_CLASS_CODE_REV_ID_REG),
    (HWIO_PCIE_GEN3X4_TYPE1_CLASS_CODE_REV_ID_REG_BASE_CLASS_CODE_BMSK | HWIO_PCIE_GEN3X4_TYPE1_CLASS_CODE_REV_ID_REG_SUBCLASS_CODE_BMSK),
    (PCIE_CLASS_CODE_BRIDGE_DEVICE | PCIE_SUBCLASS_CODE_PCI_2_PCI_BRIDGE)},
  { axi_reg,     PHY_REG_RMW, HWIO_OFFS(PCIE_GEN3X4_SLOT_CAPABILITIES_REG), HWIO_PCIE_GEN3X4_SLOT_CAPABILITIES_REG_PCIE_CAP_HOT_PLUG_CAPABLE_BMSK, 0x00000000},
  /* Assert CS2 */
  { axi_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_PCIE_ELBI_CS2_ENABLE), 0x0, 0x1},
  /* Memory Barrier */
  { invalid_reg, PHY_REG_DSB, 0xFF, 0xFF, 0xFF},
  /* Disable BAR0 and BAR1 */
  { axi_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_TYPE1_BAR0_REG), 0x0, 0x0},
  { axi_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_TYPE1_BAR1_REG), 0x0, 0x0},
  { axi_reg,     PHY_REG_RMW, HWIO_OFFS(PCIE_GEN3X4_GEN2_CTRL_OFF), 0x1f00UL, (0x1 << HWIO_PCIE_GEN3X4_GEN2_CTRL_OFF_NUM_OF_LANES_SHFT) },
  { axi_reg,     PHY_REG_RMW, HWIO_OFFS(PCIE_GEN3X4_GEN3_RELATED_OFF), HWIO_PCIE_GEN3X4_GEN3_RELATED_OFF_GEN3_ZRXDC_NONCOMPL_BMSK, 0x00 },
  /* Clear the target link speed bit in link control 2 and enable only for GEN2 speeds */
  /* De-Assert CS2 */
  { axi_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_PCIE_ELBI_CS2_ENABLE), 0x0, 0x0},
  /* Disable DBI_RO_WR_EN to access CS1 region */
  { axi_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_MISC_CONTROL_1_OFF), 0x0, 0x0},
  /* Memory Barrier */
  { invalid_reg, PHY_REG_DSB, 0xFF, 0xFF, 0xFF},
  { invalid_reg, PHY_REG_NOP, 0xFF, 0xFF, 0xFF },
};

/*******************************************************************************
 * Chipset specific complete port 2 PHY init sequence
 ******************************************************************************/
static const pcie_host_phy_cfg_t pcie_host_rc2_phy_config[] =
{
  /* First write the register sequence corresponds to version 1 */
  /* Then write the register sequence corresponds to version 2 if any */
  {PCIE_SOC_VER_2,    pcie_host_rc2_v2_init_seq},
  /* Once all the version dependent sequences are written, power ON PHY */
  {PCIE_SOC_VER_ALL,  pcie_host_rc2_pwron_seq},
  /* Once the PHY is brought of reset, give a finite delay */
  {PCIE_SOC_VER_ALL,  pcie_host_rc2_phy_delay_seq},
  /* Check if the PHY PLL is locked */
  {PCIE_SOC_VER_ALL, pcie_host_rc2_phy_poll_seq},
  /* Perform post power up sequence */
  {PCIE_SOC_VER_ALL,  pcie_host_rc2_postpwrup_seq},
  /* End of PHY init sequence */
  {PCIE_SOC_VER_STOP, NULL},
};

pcie_host_clk_config_t pcie_host_rc2_clk_config = 2; //Just the port number is required in XBL

pcie_host_rc_config_t pcie_host_rc2_config =
{
    /* LDO Config */
    &pcie_host_rc2_pmic_config,

    /* Clock Config  */
    &pcie_host_rc2_clk_config, /* This is handled manually in hte clock driver for XBL */

    /* Reset Config */
    NULL, /* This is handled manually in the clock driver for XBL */

    /* GPIO Config */
    pcie_host_rc2_gpio_config,

    /* PHY Data */
    pcie_host_rc2_phy_config,

    /* Base Addresses */
    pcie_host_rc2_memreg
};
#else
pcie_host_ldo_config_t pcie_host_rc1_ldos[]={
    {PMIC_C, PM_LDO_3},
    {PMIC_E, PM_LDO_5}
};

/* TODO : Right now we will only have the data for the RC1 port in MTP that has NVMe */

const pcie_host_pmic_config_t pcie_host_rc1_pmic_config =
{
    .num_ldos = sizeof(pcie_host_rc1_ldos)/sizeof(pcie_host_ldo_config_t),
    .ldo_cfg = pcie_host_rc1_ldos
};


/*
pcie_host_clk_config pcie_host_rc1_clocks[] = {

}
*/

pcie_host_gpio_config_t pcie_host_rc1_gpio_config[] =
{
  {
   .type = MSM_GPIO,
   {
    .tlmm_gpio_config.gpio_num     = 103,// CLK_REQ
    .tlmm_gpio_config.func         = 1,
    .tlmm_gpio_config.dir          = DAL_GPIO_INPUT,
    .tlmm_gpio_config.pull         = DAL_GPIO_NO_PULL,
    .tlmm_gpio_config.drv_strength = DAL_GPIO_2MA,
    .tlmm_gpio_config.value        = DAL_GPIO_LOW_VALUE
   }
  }
  ,
  {
   .type = MSM_GPIO,
   {
    .tlmm_gpio_config.gpio_num     = 175, // PCIE1_RST
    .tlmm_gpio_config.func         = 0,
    .tlmm_gpio_config.dir          = DAL_GPIO_OUTPUT,
    .tlmm_gpio_config.pull         = DAL_GPIO_NO_PULL,
    .tlmm_gpio_config.drv_strength = DAL_GPIO_2MA,
    .tlmm_gpio_config.value        = DAL_GPIO_LOW_VALUE
   }
  }
  ,
  {
   .type = MSM_GPIO,
   {
    .tlmm_gpio_config.gpio_num     = 177, // PCIE1_WAKE
    .tlmm_gpio_config.func         = 0,
    .tlmm_gpio_config.dir          = DAL_GPIO_INPUT,
    .tlmm_gpio_config.pull         = DAL_GPIO_NO_PULL,
    .tlmm_gpio_config.drv_strength = DAL_GPIO_2MA,
    .tlmm_gpio_config.value        = DAL_GPIO_LOW_VALUE
   }
  }
  ,
  {
   .type = DELAY,
   {
    .delay_us = 10000 // 10 ms delay for EP device preparation for Link Training
   }
  }
  ,
  {
   .type = MSM_GPIO,
   {
    .tlmm_gpio_config.gpio_num     = 175, // PCIE1_RST
    .tlmm_gpio_config.func         = 0,
    .tlmm_gpio_config.dir          = DAL_GPIO_OUTPUT,
    .tlmm_gpio_config.pull         = DAL_GPIO_NO_PULL,
    .tlmm_gpio_config.drv_strength = DAL_GPIO_2MA,
    .tlmm_gpio_config.value        = DAL_GPIO_HIGH_VALUE
   }
  }
  ,
  /* Default and end of sequence entry */
  {
   .type = NONE,
   {
    .delay_us = NONE
   }
  }
};

pcie_host_mem_region_t pcie_host_rc1_memreg[] =
{
  /**< axi_reg: DBI register space */
  {.pa=PCIE_GEN3X2_PCIE_WRAPPER_AXI_NO_EDMA_BASE, .sz=PCIE_GEN3X2_PCIE_WRAPPER_AXI_NO_EDMA_BASE_SIZE},
  /**< ahb_reg: PARF register space */
  {.pa=PCIE_GEN3X2_PCIE_WRAPPER_AHB_W_PHY_G3X2_BASE, .sz=PCIE_GEN3X2_PCIE_WRAPPER_AHB_W_PHY_G3X2_BASE_SIZE},
  /**< tcsr_reg: Access TCSR register space */
  {.pa=CORE_TOP_CSR_BASE, .sz=CORE_TOP_CSR_BASE_SIZE},
  /**< refgen_reg: Access TCSR register space */
  {.pa=0x88E5000, .sz=0x2000},
  /**< invalid_reg: Invalid reg base to end the PHY init sequence */
  {.pa=0xFFFFFFFFUL, .sz=0x0UL}
};

/*******************************************************************************
 * V1 Chipset specific port 1 PHY init sequence
 ******************************************************************************/
static const pcie_host_phy_seq_t pcie_host_rc1_v1_init_seq[] =
{
  /* Register Base, Operation, Offset, Mask & Value */
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_PCIE_PARF_DEVICE_TYPE                             ), 0x00, 0x04 },
  { tcsr_reg,    PHY_REG_WRI, HWIO_OFFS(TCSR_PCIEPHY_LINK_CONFIG                                      ), 0x00, 0x00 },
 /* TODO: Need to find out if turning ON refgen is required for all the cores */
  { refgen_reg,  PHY_REG_WRI, (0x1018                                                                 ), 0x00, 0x08 }, // PERIPH_SS_PHY_REFGEN_NORTH_RSM_CNTRL
  { refgen_reg,  PHY_REG_WRI, (0x1020                                                                 ), 0x00, 0x4E }, // PERIPH_SS_PHY_REFGEN_NORTH_RESTRIM_BYPASS_CODE

    /* Release power-down mode and allow endpoint refclk drive */
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BPCS_COM_POWER_DOWN_CONTROL               ), 0x00, 0x03},

  /* QSERDERS PLL settings */
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_SSC_EN_CENTER                ), 0x00, 0x00},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_SSC_PER1                     ), 0x00, 0x31},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_SSC_PER2                     ), 0x00, 0x01},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_SSC_STEP_SIZE1_MODE0         ), 0x00, 0xDE},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_SSC_STEP_SIZE2_MODE0         ), 0x00, 0x07},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_SSC_STEP_SIZE1_MODE1         ), 0x00, 0x4C},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_SSC_STEP_SIZE2_MODE1         ), 0x00, 0x06},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_CLK_ENABLE1                  ), 0x00, 0x90},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_PLL_IVCO                     ), 0x00, 0x0F},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_CP_CTRL_MODE0                ), 0x00, 0x06},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_CP_CTRL_MODE1                ), 0x00, 0x06},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_PLL_RCTRL_MODE0              ), 0x00, 0x16},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_PLL_RCTRL_MODE1              ), 0x00, 0x16},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_PLL_CCTRL_MODE0              ), 0x00, 0x36},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_PLL_CCTRL_MODE1              ), 0x00, 0x36},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_SYSCLK_EN_SEL                ), 0x00, 0x08}, // 0x04: select internal diff cml clock inputs, 0x08: select SE cmos clock input
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_LOCK_CMP_EN                  ), 0x00, 0x42}, // Excel values (DNW): 0x01
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_LOCK_CMP1_MODE0              ), 0x00, 0x0A}, // Excel values (DNW): 0x7F
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_LOCK_CMP2_MODE0              ), 0x00, 0x1A}, // Excel values (DNW): 0x02
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_LOCK_CMP1_MODE1              ), 0x00, 0x14}, // Excel values (DNW): 0xFF
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_LOCK_CMP2_MODE1              ), 0x00, 0x34}, // Excel values (DNW): 0x04
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_DEC_START_MODE0              ), 0x00, 0x82},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_DEC_START_MODE1              ), 0x00, 0x68},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_DIV_FRAC_START1_MODE0        ), 0x00, 0x55},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_DIV_FRAC_START2_MODE0        ), 0x00, 0x55},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_DIV_FRAC_START3_MODE0        ), 0x00, 0x03},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_DIV_FRAC_START1_MODE1        ), 0x00, 0xAB},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_DIV_FRAC_START2_MODE1        ), 0x00, 0xAA},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_DIV_FRAC_START3_MODE1        ), 0x00, 0x02},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_VCO_TUNE_MAP                 ), 0x00, 0x02},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_VCO_TUNE1_MODE0              ), 0x00, 0x24},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_VCO_TUNE1_MODE1              ), 0x00, 0xB4},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_VCO_TUNE2_MODE1              ), 0x00, 0x03},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_CLK_SELECT                   ), 0x00, 0x34},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_HSCLK_SEL                    ), 0x00, 0x01},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_CORECLK_DIV_MODE1            ), 0x00, 0x08},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_BIN_VCOCAL_CMP_CODE1_MODE0   ), 0x00, 0xB9},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_BIN_VCOCAL_CMP_CODE2_MODE0   ), 0x00, 0x1E},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_BIN_VCOCAL_CMP_CODE1_MODE1   ), 0x00, 0x94},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_BIN_VCOCAL_CMP_CODE2_MODE1   ), 0x00, 0x18},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_BIN_VCOCAL_HSCLK_SEL         ), 0x00, 0x11}, // HSIO: 0x01, Excel: 0x11

  /* Lane 0 settings */
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_TX0_LANE_MODE_1                  ), 0x00, 0x05},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_TX0_RCV_DETECT_LVL_2             ), 0x00, 0x12},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX0_UCDR_FO_GAIN                 ), 0x00, 0x0C}, //KV add
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX0_UCDR_SO_GAIN                 ), 0x00, 0x03},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX0_UCDR_SO_SATURATION_AND_ENABLE), 0x00, 0x7F},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX0_UCDR_PI_CONTROLS             ), 0x00, 0x70},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX0_AUX_DATA_TCOARSE_TFINE       ), 0x00, 0xA0},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX0_RCLK_AUXDATA_SEL             ), 0x00, 0xC0},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX0_VGA_CAL_CNTRL1               ), 0x00, 0x54},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX0_VGA_CAL_CNTRL2               ), 0x00, 0x37},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX0_GM_CAL                       ), 0x00, 0x05},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX0_RX_EQU_ADAPTOR_CNTRL1        ), 0x00, 0x07},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX0_RX_EQU_ADAPTOR_CNTRL2        ), 0x00, 0x6E},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX0_RX_EQU_ADAPTOR_CNTRL3        ), 0x00, 0x6E},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX0_RX_EQU_ADAPTOR_CNTRL4        ), 0x00, 0x4A},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX0_RX_IDAC_TSETTLE_LOW          ), 0x00, 0xC0},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX0_RX_IDAC_TSETTLE_HIGH         ), 0x00, 0x00},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX0_RX_EQ_OFFSET_ADAPTOR_CNTRL1  ), 0x00, 0x17},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX0_SIGDET_ENABLES               ), 0x00, 0x1C},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX0_SIGDET_CNTRL                 ), 0x00, 0x03},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX0_SIGDET_DEGLITCH_CNTRL        ), 0x00, 0x14},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX0_RX_MODE_00_LOW               ), 0x00, 0x7F},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX0_RX_MODE_00_HIGH              ), 0x00, 0xFF},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX0_RX_MODE_00_HIGH2             ), 0x00, 0xFF},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX0_RX_MODE_00_HIGH3             ), 0x00, 0xDB},  // Recently updated by Zhi
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX0_RX_MODE_00_HIGH4             ), 0x00, 0x75},  // Recently updated by Zhi
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX0_RX_MODE_01_LOW               ), 0x00, 0x24},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX0_RX_MODE_01_HIGH              ), 0x00, 0xE4},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX0_RX_MODE_01_HIGH2             ), 0x00, 0xEC},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX0_RX_MODE_01_HIGH3             ), 0x00, 0x39}, // dbaum, orig: 0x39
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX0_RX_MODE_01_HIGH4             ), 0x00, 0x36},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX0_RX_MODE_10_LOW               ), 0x00, 0xD4},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX0_RX_MODE_10_HIGH              ), 0x00, 0x54},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX0_RX_MODE_10_HIGH2             ), 0x00, 0xDB},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX0_RX_MODE_10_HIGH3             ), 0x00, 0x39},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX0_RX_MODE_10_HIGH4             ), 0x00, 0x31},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX0_DCC_CTRL1                    ), 0x00, 0x0C},


  /* Lane 1 settings */
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_TX1_LANE_MODE_1                  ), 0x00, 0x05},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_TX1_RCV_DETECT_LVL_2             ), 0x00, 0x12},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX1_UCDR_FO_GAIN                 ), 0x00, 0x0C},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX1_UCDR_SO_GAIN                 ), 0x00, 0x03},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX1_UCDR_SO_SATURATION_AND_ENABLE), 0x00, 0x7F},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX1_UCDR_PI_CONTROLS             ), 0x00, 0x70},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX1_AUX_DATA_TCOARSE_TFINE       ), 0x00, 0xA0},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX1_RCLK_AUXDATA_SEL             ), 0x00, 0xC0},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX1_VGA_CAL_CNTRL1               ), 0x00, 0x54},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX1_VGA_CAL_CNTRL2               ), 0x00, 0x37},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX1_GM_CAL                       ), 0x00, 0x05},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX1_RX_EQU_ADAPTOR_CNTRL1        ), 0x00, 0x07},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX1_RX_EQU_ADAPTOR_CNTRL2        ), 0x00, 0x6E},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX1_RX_EQU_ADAPTOR_CNTRL3        ), 0x00, 0x6E},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX1_RX_EQU_ADAPTOR_CNTRL4        ), 0x00, 0x4A},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX1_RX_IDAC_TSETTLE_LOW          ), 0x00, 0xC0},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX1_RX_IDAC_TSETTLE_HIGH         ), 0x00, 0x00},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX1_RX_EQ_OFFSET_ADAPTOR_CNTRL1  ), 0x00, 0x17},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX1_SIGDET_ENABLES               ), 0x00, 0x1C},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX1_SIGDET_CNTRL                 ), 0x00, 0x03},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX1_SIGDET_DEGLITCH_CNTRL        ), 0x00, 0x14},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX1_RX_MODE_00_LOW               ), 0x00, 0x7F},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX1_RX_MODE_00_HIGH              ), 0x00, 0xFF},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX1_RX_MODE_00_HIGH2             ), 0x00, 0xFF},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX1_RX_MODE_00_HIGH3             ), 0x00, 0xDB}, // Recently updated by Zhi
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX1_RX_MODE_00_HIGH4             ), 0x00, 0x75}, // Recently updated by Zhi
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX1_RX_MODE_01_LOW               ), 0x00, 0x24},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX1_RX_MODE_01_HIGH              ), 0x00, 0xE4},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX1_RX_MODE_01_HIGH2             ), 0x00, 0xEC},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX1_RX_MODE_01_HIGH3             ), 0x00, 0x39},  // dbaum, orig: 0x39
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX1_RX_MODE_01_HIGH4             ), 0x00, 0x36},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX1_RX_MODE_10_LOW               ), 0x00, 0xD4},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX1_RX_MODE_10_HIGH              ), 0x00, 0x54},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX1_RX_MODE_10_HIGH2             ), 0x00, 0xDB},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX1_RX_MODE_10_HIGH3             ), 0x00, 0x39},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX1_RX_MODE_10_HIGH4             ), 0x00, 0x31},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX1_DCC_CTRL1                    ), 0x00, 0x0C},

  /* PCS Settings */
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BPCS_COM_REFGEN_REQ_CONFIG1               ), 0x00, 0x05},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BPCS_COM_RX_SIGDET_LVL                    ), 0x00, 0xAA}, // HSIO: 0x66, Excel: 0xAA
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BPCS_COM_P2U3_WAKEUP_DLY_TIME_AUXCLK_L    ), 0x00, 0x01},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BPCS_COM_EQ_CONFIG5                       ), 0x00, 0x01}, // HSIO code, not in HPG
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BPCS_PCIE_POWER_STATE_CONFIG2             ), 0x00, 0x0D},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BPCS_PCIE_POWER_STATE_CONFIG4             ), 0x00, 0x07},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BPCS_PCIE_ENDPOINT_REFCLK_DRIVE           ), 0x00, 0xC1},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BPCS_PCIE_L1P1_WAKEUP_DLY_TIME_AUXCLK_L   ), 0x00, 0x01},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BPCS_PCIE_L1P2_WAKEUP_DLY_TIME_AUXCLK_L   ), 0x00, 0x01},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BPCS_PCIE_OSC_DTCT_ACTIONS                ), 0x00, 0x00},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BPCS_PCIE_PRESET_P10_PRE                  ), 0x00, 0x00}, // HSIO code
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BPCS_PCIE_PRESET_P10_POST                 ), 0x00, 0x58},

  { invalid_reg, PHY_REG_NOP, 0xFF, 0xFF, 0xFF },
};

/*******************************************************************************
 * Chipset specific version independent port 1 PHY power ON sequence
 ******************************************************************************/
STATIC CONST pcie_host_phy_seq_t pcie_host_rc1_pwron_seq[] =
{
  /* Register Base, Operation, Offset, Mask & Value */
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BPCS_COM_SW_RESET                         ), 0x00, 0x00}, // Release software reset
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BPCS_COM_START_CONTROL                    ), 0x00, 0x03}, // Start PCS and Serdes power SMs
  { invalid_reg, PHY_REG_NOP, 0xFF, 0xFF, 0xFF },
};

/*******************************************************************************
 * PLL takes ~2 ms to lock-in
 *
 * This was measured while polling with 100 us interval
 * for PHYSTATUS bit PCS_STATUS register.
 * This delay here will not cause additional delay as the
 * PcieConfigLibPollBitMsk funtion will come out in the
 * first pass as we chose this constant delay waiting
 *
 * Based on programming sequence document one of the following two can be used:
 *   1) Finite delay can be added before observing PHYSTATUS
 *   2) SW can continuously poll for PHYSTATUS
 ******************************************************************************/
STATIC CONST pcie_host_phy_seq_t pcie_host_rc1_phy_delay_seq[] =
{
  /* Register Base, Operation, Offset, Mask & Value */
  { invalid_reg, PHY_REG_DELAY, 0xFF, 0xFF, 2000 },
  { invalid_reg, PHY_REG_NOP, 0xFF, 0xFF, 0xFF },
};

/*******************************************************************************
 * poll PHY power ON status of port 1 PHY
 ******************************************************************************/
STATIC CONST pcie_host_phy_seq_t pcie_host_rc1_phy_poll_seq[] =
{
  /* Register Base, Operation, Offset, Mask & Value */
  { ahb_reg, PHY_REG_POLL, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BPCS_COM_PCS_STATUS1), HWIO_PCIE_GEN3X2_CM_PCIE_BPCS_COM_PCS_STATUS1_PHYSTATUS_BMSK, FALSE },
  { invalid_reg, PHY_REG_NOP, 0xFF, 0xFF, 0xFF },
};

/*******************************************************************************
 * post PHY power ON sequence for port 1 GEN3 PHY
 ******************************************************************************/
STATIC CONST pcie_host_phy_seq_t pcie_host_rc1_postpwrup_seq[] =
{
  /* Register Base, Operation, Offset, Mask & Value */
  /* Writing Slave address space size */
  { ahb_reg,    PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_PCIE_PARF_SLV_ADDR_SPACE_SIZE), 0x0, 0x2000000},
  { ahb_reg,    PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_PCIE_PARF_SLV_ADDR_SPACE_SIZE_HI), 0x0, 0x00000000},
  /* Disable the BDF to SID table */
  { ahb_reg,    PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_PCIE_PARF_BDF_TO_SID_CFG), 0x0, 0x1},
  /* Clear REQ_NOT_ENTER_L1 field in PARF PM_CTRL to allow L1 entry */
  { ahb_reg,    PHY_REG_RMW, HWIO_OFFS(PCIE_GEN3X2_PCIE_PARF_PM_CTRL), HWIO_PCIE_GEN3X2_PCIE_PARF_PM_CTRL_REQ_NOT_ENTR_L1_BMSK, 0x00000000},
  /* Enable DBI_RO_WR_EN to access CS1 region */
  { axi_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_MISC_CONTROL_1_OFF), 0x0, 0x1},
  /* Memory Barrier */
  { invalid_reg, PHY_REG_DSB, 0xFF, 0xFF, 0xFF},
  /* Update Base class and Sub class */
  { axi_reg,     PHY_REG_RMW, HWIO_OFFS(PCIE_GEN3X2_TYPE1_CLASS_CODE_REV_ID_REG),
    (HWIO_PCIE_GEN3X2_TYPE1_CLASS_CODE_REV_ID_REG_BASE_CLASS_CODE_BMSK | HWIO_PCIE_GEN3X2_TYPE1_CLASS_CODE_REV_ID_REG_SUBCLASS_CODE_BMSK),
    (PCIE_CLASS_CODE_BRIDGE_DEVICE | PCIE_SUBCLASS_CODE_PCI_2_PCI_BRIDGE)},
  { axi_reg,     PHY_REG_RMW, HWIO_OFFS(PCIE_GEN3X2_SLOT_CAPABILITIES_REG), HWIO_PCIE_GEN3X2_SLOT_CAPABILITIES_REG_PCIE_CAP_HOT_PLUG_CAPABLE_BMSK, 0x00000000},
  /* Assert CS2 */
  { axi_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_PCIE_ELBI_CS2_ENABLE), 0x0, 0x1},
  /* Memory Barrier */
  { invalid_reg, PHY_REG_DSB, 0xFF, 0xFF, 0xFF},
  /* Disable BAR0 and BAR1 */
  { axi_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_TYPE1_BAR0_REG), 0x0, 0x0},
  { axi_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_TYPE1_BAR1_REG), 0x0, 0x0},
  { axi_reg,     PHY_REG_RMW, HWIO_OFFS(PCIE_GEN3X2_GEN2_CTRL_OFF), 0x1f00UL, (0x1 << HWIO_PCIE_GEN3X2_GEN2_CTRL_OFF_NUM_OF_LANES_SHFT) },
  //{ axi_reg,     PHY_REG_RMW, HWIO_OFFS(PCIE_GEN3X2_GEN2_CTRL_OFF), HWIO_PCIE_GEN3X2_GEN2_CTRL_OFF_DIRECT_SPEED_CHANGE_BMSK, (0x0) },
  /* Enable BME & MSE before LTSSM_EN */
  { axi_reg,     PHY_REG_RMW, HWIO_OFFS(PCIE_GEN3X2_TYPE1_STATUS_COMMAND_REG), HWIO_PCIE_GEN3X2_TYPE1_STATUS_COMMAND_REG_MSE_BMSK, (0x1 << HWIO_PCIE_GEN3X2_TYPE1_STATUS_COMMAND_REG_MSE_SHFT)},
  { axi_reg,     PHY_REG_RMW, HWIO_OFFS(PCIE_GEN3X2_TYPE1_STATUS_COMMAND_REG), HWIO_PCIE_GEN3X2_TYPE1_STATUS_COMMAND_REG_BME_BMSK, (0x1 << HWIO_PCIE_GEN3X2_TYPE1_STATUS_COMMAND_REG_BME_SHFT)},
  /* De-Assert CS2 */
  { axi_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_PCIE_ELBI_CS2_ENABLE), 0x0, 0x0},
  /* Disable DBI_RO_WR_EN to access CS1 region */
  { axi_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_MISC_CONTROL_1_OFF), 0x0, 0x0},
  /* Memory Barrier */
  { invalid_reg, PHY_REG_DSB, 0xFF, 0xFF, 0xFF},
  { invalid_reg, PHY_REG_NOP, 0xFF, 0xFF, 0xFF },
};

/*******************************************************************************
 * Chipset specific complete port 1 PHY init sequence
 ******************************************************************************/
static const pcie_host_phy_cfg_t pcie_host_rc1_phy_config[] =
{
  /* First write the register sequence corresponds to version 1 */
  {PCIE_SOC_VER_1,    pcie_host_rc1_v1_init_seq},
  /* Then write the register sequence corresponds to version 2 if any */
  /* Once all the version dependent sequences are written, power ON PHY */
  {PCIE_SOC_VER_ALL,  pcie_host_rc1_pwron_seq},
  /* Once the PHY is brought of reset, give a finite delay */
  {PCIE_SOC_VER_ALL,  pcie_host_rc1_phy_delay_seq},
  /* Check if the PHY PLL is locked */
  {PCIE_SOC_VER_ALL, pcie_host_rc1_phy_poll_seq},
  /* Perform post power up sequence */
  {PCIE_SOC_VER_ALL,  pcie_host_rc1_postpwrup_seq},
  /* End of PHY init sequence */
  {PCIE_SOC_VER_STOP, NULL},
};

#ifdef UNUSED
/*******************************************************************************
 * Chipset specific port 1 PHY De-init sequence
 ******************************************************************************/
STATIC CONST pcie_host_phy_seq_t pcie_host_rc1_phy_de_init_seq[] =
{
  /* Register Base, Operation, Offset, Mask & Value */
  { ahb_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BPCS_COM_POWER_DOWN_CONTROL               ), 0x0, 0x03},
  { invalid_reg, PHY_REG_NOP, 0xFF, 0xFF, 0xFF },
};
#endif

pcie_host_clk_config_t pcie_host_rc1_clk_config = 1; //Just the port number is required in XBL

pcie_host_rc_config_t pcie_host_rc1_config =
{
    /* LDO Config */
    &pcie_host_rc1_pmic_config,

    /* Clock Config  */
    &pcie_host_rc1_clk_config, /* This is handled manually in hte clock driver for XBL */

    /* Reset Config */
    NULL, /* This is handled manually in the clock driver for XBL */

    /* GPIO Config */
    pcie_host_rc1_gpio_config,

    /* PHY Data */
    pcie_host_rc1_phy_config,

    /* Base Addresses */
    pcie_host_rc1_memreg
};
#endif

/* The structure moves to the XBL Config */

pcie_host_rc_config_t *pcie_host_rc_cfg[] =
{
#ifdef PRIMARY_NVME_ON_PCI2
    &pcie_host_rc2_config
#else
    &pcie_host_rc1_config
#endif
};

pcie_host_tgt_config_t *pcie_host_tgt_config;

pcie_host_tgt_config_t pcie_host_tgt_config_data =
{
    (sizeof(pcie_host_rc_cfg)/sizeof(pcie_host_rc_config_t *)),
    pcie_host_rc_cfg,
    100000
};

/* ============================================================================
**  Function : pcie_host_tgt_cfg_init
** ============================================================================
*/
/**
    @brief
    Initializes target specific config data

    @details
    Initializes target specific configuration data to be used by the PCIe host
    driver

    @return
    None

*/

void pcie_host_tgt_cfg_init()
{
    /* Does nothing at the moment until we move to XBL Config */
    pcie_host_tgt_config = &pcie_host_tgt_config_data;
}
