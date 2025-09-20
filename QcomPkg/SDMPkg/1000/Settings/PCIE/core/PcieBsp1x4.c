/*============================================================================
  FILE:         PCieBsp1x4.c

  OVERVIEW:     BSP for Pcie 1x4 bifurcated PHY.

  DEPENDENCIES: None

                Copyright (c) 2018 Qualcomm Technologies, Inc.
                All Rights Reserved.
============================================================================*/
/*============================================================================
  EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.  Please
  use ISO format for dates.

  when        who  what, where, why
  ----------  ---  -----------------------------------------------------------
  10/30/18    ts   Included Bi-furcation support with 1x4 lane width

============================================================================*/

/*----------------------------------------------------------------------------
 * Static Variable Definitions
 * -------------------------------------------------------------------------*/

/*******************************************************************************
 * This section will hold the port info data for root port 2
 ******************************************************************************/
#if (RPMASK & PCIE_RP(2))

/*******************************************************************************
 * V2 Chipset specific 1x4 port 2 PHY init sequence
 ******************************************************************************/
STATIC CONST pciePhyRegSeq_t v2_port_2_phy_1x4_init_seq[] =
{
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
 * Chipset specific complete port 2 PHY init sequence
 ******************************************************************************/
STATIC CONST pciePhyAllVerSeq_t sdm1000_port_2_phy_1x4_init_seq[] =
{
  /* First write the register sequence corresponds to version 1 */
  {Pcie_Qcom_SoC_Ver_1,    v1_port_2_phy_init_seq},
  /* Write the register sequence corresponds to version 2 1x4 PHY */
  {Pcie_Qcom_SoC_Ver_2,    v2_port_2_phy_1x4_init_seq},
  /* Once all the version dependent sequences are written, power ON PHY */
  {Pcie_Qcom_SoC_Ver_ALL,  port_2_phy_power_on_seq},
  /* Once the PHY is brought of reset, give a finite delay */
  {Pcie_Qcom_SoC_Ver_ALL,  port_2_phy_delay_seq},
  /* Check if the PHY A PLL is locked */
  {Pcie_Qcom_SoC_Ver_POLL, port_2_phy_A_poll_seq},
  /* Perform post power up sequence */
  {Pcie_Qcom_SoC_Ver_ALL,  port_2_phy_post_power_up_seq},
  /* End of PHY init sequence */
  {Pcie_Qcom_SoC_Ver_STOP, NULL},
};

#endif /* (RPMASK & PCIE_RP(2)) - root port 2 enabled */

/*******************************************************************************
 * Chipset specific 1x4 PHY init sequence
 ******************************************************************************/
STATIC CONST pciePhyRegSeq_t common_phy_1x4_init_seq[] =
{
  /* Register Base, Operation, Offset, Mask & Value */
  /* 1x4 PHY bifurcation mode */
  { tcsr_reg, PHY_REG_WRI, HWIO_OFFS(TCSR_PCIEPHY_LINK_CONFIG), 0x00, 0x01 },

  { invalid_reg, PHY_REG_NOP, 0xFF, 0xFF, 0xFF },
};

/*******************************************************************************
 * Chipset specific complete common PHY 1x4 init sequence
 ******************************************************************************/
STATIC CONST pciePhyAllVerSeq_t sdm1000_common_phy_1x4_init_seq[] =
{
  {Pcie_Qcom_SoC_Ver_ALL , common_phy_1x4_init_seq},
  {Pcie_Qcom_SoC_Ver_STOP, NULL},
};

/*******************************************************************************
 * array of structures will hold the information
 * of available root ports
 ******************************************************************************/
static portInfo_t multi_port_1x4_info[] =
{
/* Include port_info for RP0 */
#if (RPMASK & PCIE_RP(0))
  {
    .clk                 = pcie_clk_rp0,
    .pwr                 = "pcie_0_gdsc",
    .icb_bw              = pcie_0_icb_bw,
    .clk_set             = pcie_0_clk_settings,
    .gpio_cfg            = gpio_config_rp0,
    .mem                 = port_0_reg,
    .port_phy_init_seq   = sdm1000_port_0_phy_init_seq,
    .port_phy_deinit_seq = port_0_phy_de_init_seq,
    .link                = port0_link,
    .hw_rp_index         = 0,
    .io_coherency_en     = FALSE,
    .iommu_hdl           = "\\_SB_.PCI0",
    .num_ports           = NUM_PCIE_0_PORTS
  },
#endif
/* Include port_info for RP2 if it exist */
#if (RPMASK & PCIE_RP(2))
  {
    .clk                 = pcie_clk_rp2,
    .pwr                 = "pcie_2_gdsc",
    .icb_bw              = pcie_2_icb_bw,
    .clk_set             = pcie_2_clk_settings,
    .gpio_cfg            = gpio_config_rp2,
    .mem                 = port_2_reg,
    .port_phy_init_seq   = sdm1000_port_2_phy_1x4_init_seq,
    .port_phy_deinit_seq = port_2_phy_de_init_seq,
    .link                = port2_link,
    .hw_rp_index         = 2,
    .io_coherency_en     = FALSE,
    .iommu_hdl           = "\\_SB_.PCI1",
    .num_ports           = NUM_PCIE_2_PORTS
  },
#endif
/* Include port_info for RP3 if it exist */
#if (RPMASK & PCIE_RP(3))
  {
    .clk                 = pcie_clk_rp3,
    .pwr                 = "pcie_3_gdsc",
    .icb_bw              = pcie_3_icb_bw,
    .clk_set             = pcie_3_clk_settings,
    .gpio_cfg            = gpio_config_rp3,
    .mem                 = port_3_reg,
    .port_phy_init_seq   = sdm1000_port_3_phy_init_seq,
    .port_phy_deinit_seq = port_3_phy_de_init_seq,
    .link                = port3_link,
    .hw_rp_index         = 3,
    .io_coherency_en     = FALSE,
    .iommu_hdl           = "\\_SB_.PCI3",
    .num_ports           = NUM_PCIE_3_PORTS
  },
#endif
};

/*******************************************************************************
 * This struct will hold the port info for all the available root ports and also
 * number of root ports enabled during compile time
 ******************************************************************************/
STATIC CONST rootPort_t root_port_1x4[] = {
  {
    .port_info = multi_port_1x4_info,
    .num_rp    = ARRAY_LENGTH(multi_port_1x4_info)
  }
};

/*******************************************************************************
 * The single structure will hold all the information needed to bring the
 * root ports out of reset
 ******************************************************************************/
STATIC CONST pcieBsp_t PcieBsp1x4[] =
{
  {
    /* null-terminated string common PHY clock name
     * Since, there is no common PHY in MSM8998,
     * this is assigned with NULL
     */
    .phy_clk       = pcie_clk_cmn,
    /* common PHY init sequence */
    .phy_init_seq  = sdm1000_common_phy_1x4_init_seq,
    /* root port info for all enabled root ports */
    .rp_info       = root_port_1x4
  }
};
