/*============================================================================
  FILE:         PCieBsp2x2.c

  OVERVIEW:     BSP for 2x2 (default) Pcie PHY.

  DEPENDENCIES: None

                Copyright (c) 2018-2019 Qualcomm Technologies, Inc.
                All Rights Reserved.
============================================================================*/
/*============================================================================
  EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.  Please
  use ISO format for dates.

  when        who  what, where, why
  ----------  ---  -----------------------------------------------------------
  06/25/2019  ts   Updated V2 PHY sequence and added 150 delay before
                   de-asserting PERST 
  10/30/2018  ts   Initial checkin for SDM1000 2x2 PHY settings

============================================================================*/

/*----------------------------------------------------------------------------
 * Static Variable Definitions
 * -------------------------------------------------------------------------*/

/*******************************************************************************
 * This section will hold the port info data for root port 0
 ******************************************************************************/
#if (RPMASK & PCIE_RP(0))

/* Number of PCIe ports present in RP0 */
#define NUM_PCIE_0_PORTS 0x01

/*******************************************************************************
 * Register region information for PCIe GEN3X1
 ******************************************************************************/
static memRegions_t port_0_reg[max_mem_reg] =
{
  /**< axi_reg: DBI register space */
  {.pa=PCIE_GEN3X1_PCIE_WRAPPER_AXI_NO_EDMA_BASE, .sz=PCIE_GEN3X1_PCIE_WRAPPER_AXI_NO_EDMA_BASE_SIZE},
  /**< ahb_reg: PARF register space */
  {.pa=PCIE_GEN3X1_PCIE_WRAPPER_AHB_W_PHY_G3X1_BASE, .sz=PCIE_GEN3X1_PCIE_WRAPPER_AHB_W_PHY_G3X1_BASE_SIZE},
  /**< tcsr_reg: Access TCSR register space */
  {.pa=CORE_TOP_CSR_BASE, .sz=CORE_TOP_CSR_BASE_SIZE},
  /**< invalid_reg: Invalid reg base to end the PHY init sequence */
  {.pa=PCIE_INVALID_REG_BASE, .sz=PCIE_INVALID_REG_SIZE}
};

/*******************************************************************************
 * PCIe port reset clock name list for root port 0 with NULL ending
 ******************************************************************************/
STATIC CONST char * pcie_reset_clks_rp0[] =
{
  "gcc_pcie_0_aux_clk",
  NULL
};

/*******************************************************************************
 * PCIe port external clock name list for root port 0 with NULL ending
 ******************************************************************************/
STATIC CONST char * pcie_external_clks_rp0[] =
{
  "gcc_pcie_0_pipe_clk",
  NULL
};

/*******************************************************************************
 * PCIe port gcc clock name list for root port 0 with NULL ending
 ******************************************************************************/
STATIC CONST char * pcie_gcc_clks_rp0[] =
{
  "gcc_pcie_0_aux_clk",
  "gcc_pcie_0_slv_axi_clk",
  "gcc_pcie_0_slv_q2a_axi_clk",
  "gcc_pcie_0_mstr_axi_clk",
  "gcc_pcie_0_cfg_ahb_clk",
  "gcc_pcie0_phy_refgen_clk",
  NULL
};

/*******************************************************************************
 * PCIe port aggregated gcc clock name list for root port 0
 ******************************************************************************/
STATIC CONST pcieClkDesc_t pcie_clk_rp0[] =
{
  {
    .reset = pcie_reset_clks_rp0,
    .external = pcie_external_clks_rp0,
    .gcc = pcie_gcc_clks_rp0
  }
};

/*******************************************************************************
 * ICB bandwidth request for pcie root port 0
 ******************************************************************************/
STATIC CONST ICBArb_RequestType pcie_0_icb_request[] =
{
  {
    .arbType = ICBARB_REQUEST_TYPE_3,
    .arbData.type3 =
    {
      .uIb = ((100ULL) << 20), /**< Instantaneous bandwidth in Bytes/second */
      .uAb = ((100ULL) << 20), /**< Arbitrated bandwidth in Bytes/second */
      .uLatencyNs = 0          /**< Latency requirement in nanoseconds */
    }
  }
};

/*******************************************************************************
 * ICB Master and Slave list for pcie root port 0
 ******************************************************************************/
STATIC CONST ICBArb_MasterSlaveType pcie_0_icb_mstr_slv_lst[] =
{
  {
    .eMaster = ICBID_MASTER_PCIE_0,
    .eSlave = ICBID_SLAVE_EBI1
  }
};

/*******************************************************************************
 * Aggreagate bus bandwidth vote for pcie root port 0
 ******************************************************************************/
STATIC pcieIcbClkBW_t pcie_0_icb_bw[] =
{
  {
    .npa_client_name = "pcie_0",
    .npa_hdl = NULL,
    .ms_list = pcie_0_icb_mstr_slv_lst,
    .ms_size = (UINT32)sizeof(pcie_0_icb_mstr_slv_lst),
    .req_list = pcie_0_icb_request,
    .req_size = (UINT32)sizeof(pcie_0_icb_request)
  }
};

/*******************************************************************************
 * PCIe root port 0 clock settings
 ******************************************************************************/
CONST clkFreqSettings_t pcie_0_clk_settings[] =
{
  {.freq = 19200000,  .name = "gcc_pcie_0_aux_clk"},
  {.freq = 100000000, .name = "gcc_pcie0_phy_refgen_clk"},
  {.freq = 0, .name = NULL}
};

/*******************************************************************************
 * GPIO configuration for RP0
 *
 * This array will be access from index 0 upto the end of
 * array to to configure all the GPIOs before performing
 * link training
 ******************************************************************************/
/*
 * ENDP GPIO configuration for RP0
 */
STATIC CONST gpioPinConfig_t gpio_config_rp0[] =
{
  {
   .type = MSM_GPIO,
   {
    .TlmmGpioParam.GpioNum = 36, // CLK_REQ
    .TlmmGpioParam.Func = 1,
    .TlmmGpioParam.Direction = GPIO_INPUT,
    .TlmmGpioParam.Pull = GPIO_PULL_UP,
    .TlmmGpioParam.DriveStrength = GPIO_2MA,
    .TlmmGpioParam.value = GPIO_LOW_VALUE
   }
  }
  ,
  {
   .type = MSM_GPIO,
   {
    .TlmmGpioParam.GpioNum = 35, // PCIE0_RST
    .TlmmGpioParam.Func = 0,
    .TlmmGpioParam.Direction = GPIO_OUTPUT,
    .TlmmGpioParam.Pull = GPIO_NO_PULL,
    .TlmmGpioParam.DriveStrength = GPIO_2MA,
    .TlmmGpioParam.value = GPIO_LOW_VALUE
   }
  }
  ,
  {
   .type = MSM_GPIO,
   {
    .TlmmGpioParam.GpioNum = 37, // PCIE0_WAKE
    .TlmmGpioParam.Func = 0,
    .TlmmGpioParam.Direction = GPIO_INPUT,
    .TlmmGpioParam.Pull = GPIO_NO_PULL,
    .TlmmGpioParam.DriveStrength = GPIO_2MA,
    .TlmmGpioParam.value = GPIO_LOW_VALUE
   }
  }
  ,
  {
   .type = DELAY,
   {
    .DelayValue = 150 // 150 micro seconds before REFCLK get stable
   }
  }
  ,
  {
   .type = MSM_GPIO,
   {
    .TlmmGpioParam.GpioNum = 35, // PCIE0_RST
    .TlmmGpioParam.Func = 0,
    .TlmmGpioParam.Direction = GPIO_OUTPUT,
    .TlmmGpioParam.Pull = GPIO_NO_PULL,
    .TlmmGpioParam.DriveStrength = GPIO_2MA,
    .TlmmGpioParam.value = GPIO_HIGH_VALUE
   }
  }
  ,
  /* Default and end of sequence entry */
  {
   .type = NONE,
   {
    .DelayValue = NONE
   }
  }
};

/*******************************************************************************
 * Link info
 ******************************************************************************/
STATIC pcieLink_t port0_link[] =
{
  {
    .trained             = FALSE,
    .num_lanes           = 1,
    .aspm                = aspm_l1_ena_l0s_ena,
    .ltr_threshold_val   = 150,
    .ltr_threshold_scale = LTR_TH_SCALE_1024ns
  }
};

/*******************************************************************************
 * V1 Chipset specific port 0 PHY init sequence from IPCAT QMP HPG

 * Table 2.14  Napali SEC10LPE V1 QMP PCIe PHY Config Sequence (Root Complex)
 * 19.2MHz RefClk, 16.6MHz Auxclk, SSC Gen ON
 ******************************************************************************/
STATIC CONST pciePhyRegSeq_t v1_port_0_phy_init_seq[] =
{
  /* Register Base, Operation, Offset, Mask & Value */
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_PCIE_PARF_DEVICE_TYPE                               ), 0x00, 0x04 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_PCIE_USB3_UNI_PCS_POWER_DOWN_CONTROL)                , 0x00, 0x03 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_COM_SYSCLK_EN_SEL)                           , 0x00, 0x08 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_COM_CLK_SELECT)                              , 0x00, 0x34 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_COM_CORECLK_DIV_MODE1)                       , 0x00, 0x08 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_COM_PLL_IVCO)                                , 0x00, 0x0F },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_COM_LOCK_CMP_EN)                             , 0x00, 0x42 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_COM_VCO_TUNE1_MODE0)                         , 0x00, 0x24 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_COM_VCO_TUNE2_MODE1)                         , 0x00, 0x03 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_COM_VCO_TUNE1_MODE1)                         , 0x00, 0xB4 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_COM_VCO_TUNE_MAP)                            , 0x00, 0x02 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_COM_BIN_VCOCAL_HSCLK_SEL)                    , 0x00, 0x11 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_COM_DEC_START_MODE0)                         , 0x00, 0x82 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_COM_DIV_FRAC_START3_MODE0)                   , 0x00, 0x03 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_COM_DIV_FRAC_START2_MODE0)                   , 0x00, 0x55 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_COM_DIV_FRAC_START1_MODE0)                   , 0x00, 0x55 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_COM_LOCK_CMP2_MODE0)                         , 0x00, 0x1A },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_COM_LOCK_CMP1_MODE0)                         , 0x00, 0x0A },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_COM_DEC_START_MODE1)                         , 0x00, 0x68 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_COM_DIV_FRAC_START3_MODE1)                   , 0x00, 0x02 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_COM_DIV_FRAC_START2_MODE1)                   , 0x00, 0xAA },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_COM_DIV_FRAC_START1_MODE1)                   , 0x00, 0xAB },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_COM_LOCK_CMP2_MODE1)                         , 0x00, 0x34 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_COM_LOCK_CMP1_MODE1)                         , 0x00, 0x14 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_COM_HSCLK_SEL)                               , 0x00, 0x01 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_COM_CP_CTRL_MODE0)                           , 0x00, 0x06 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_COM_PLL_RCTRL_MODE0)                         , 0x00, 0x16 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_COM_PLL_CCTRL_MODE0)                         , 0x00, 0x36 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_COM_CP_CTRL_MODE1)                           , 0x00, 0x06 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_COM_PLL_RCTRL_MODE1)                         , 0x00, 0x16 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_COM_PLL_CCTRL_MODE1)                         , 0x00, 0x36 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_COM_BIN_VCOCAL_CMP_CODE2_MODE0)              , 0x00, 0x1E },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_COM_BIN_VCOCAL_CMP_CODE1_MODE0)              , 0x00, 0xB9 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_COM_BIN_VCOCAL_CMP_CODE2_MODE1)              , 0x00, 0x18 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_COM_BIN_VCOCAL_CMP_CODE1_MODE1)              , 0x00, 0x94 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_COM_SYSCLK_BUF_ENABLE)                       , 0x00, 0x07 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_COM_SSC_EN_CENTER)                           , 0x00, 0x00 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_COM_SSC_PER1)                                , 0x00, 0x31 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_COM_SSC_PER2)                                , 0x00, 0x01 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_COM_SSC_STEP_SIZE1_MODE0)                    , 0x00, 0xDE },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_COM_SSC_STEP_SIZE2_MODE0)                    , 0x00, 0x07 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_COM_SSC_STEP_SIZE1_MODE1)                    , 0x00, 0x4C },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_COM_SSC_STEP_SIZE2_MODE1)                    , 0x00, 0x06 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_TX_RCV_DETECT_LVL_2)                         , 0x00, 0x12 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_TX_LANE_MODE_1)                              , 0x00, 0x05 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_RX_SIGDET_CNTRL)                             , 0x00, 0x03 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_RX_SIGDET_ENABLES)                           , 0x00, 0x1C },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_RX_SIGDET_DEGLITCH_CNTRL)                    , 0x00, 0x14 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_RX_RX_EQU_ADAPTOR_CNTRL1)                    , 0x00, 0x07 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_RX_RX_EQU_ADAPTOR_CNTRL2)                    , 0x00, 0x6E },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_RX_RX_EQU_ADAPTOR_CNTRL3)                    , 0x00, 0x6E },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_RX_RX_EQU_ADAPTOR_CNTRL4)                    , 0x00, 0x4A },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_RX_DFE_EN_TIMER)                             , 0x00, 0x04 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_RX_UCDR_SO_SATURATION_AND_ENABLE)            , 0x00, 0x7F },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_RX_UCDR_PI_CONTROLS)                         , 0x00, 0x70 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_RX_RX_EQ_OFFSET_ADAPTOR_CNTRL1)              , 0x00, 0x17 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_RX_VGA_CAL_CNTRL1)                           , 0x00, 0x54 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_RX_VGA_CAL_CNTRL2)                           , 0x00, 0x37 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_RX_RX_MODE_10_LOW)                           , 0x00, 0xD4 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_RX_RX_MODE_10_HIGH)                          , 0x00, 0x54 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_RX_RX_MODE_10_HIGH2)                         , 0x00, 0xDB },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_RX_RX_MODE_10_HIGH3)                         , 0x00, 0x39 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_RX_RX_MODE_10_HIGH4)                         , 0x00, 0x31 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_RX_RX_MODE_01_LOW)                           , 0x00, 0x24 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_RX_RX_MODE_01_HIGH)                          , 0x00, 0xE4 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_RX_RX_MODE_01_HIGH2)                         , 0x00, 0xEC },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_RX_RX_MODE_01_HIGH3)                         , 0x00, 0x39 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_RX_RX_MODE_01_HIGH4)                         , 0x00, 0x36 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_RX_RX_MODE_00_LOW)                           , 0x00, 0x7F },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_RX_RX_MODE_00_HIGH)                          , 0x00, 0xFF },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_RX_RX_MODE_00_HIGH2)                         , 0x00, 0xFF },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_RX_RX_MODE_00_HIGH3)                         , 0x00, 0xDB },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_RX_RX_MODE_00_HIGH4)                         , 0x00, 0x75 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_RX_RX_IDAC_TSETTLE_HIGH)                     , 0x00, 0x00 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_RX_RX_IDAC_TSETTLE_LOW)                      , 0x00, 0xC0 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_RX_AUX_DATA_TCOARSE_TFINE)                   , 0x00, 0xA0 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_RX_RCLK_AUXDATA_SEL)                         , 0x00, 0xC0 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_RX_DCC_CTRL1)                                , 0x00, 0x0C },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_RX_GM_CAL)                                   , 0x00, 0x05 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_RX_UCDR_FO_GAIN)                             , 0x00, 0x0C },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_RX_UCDR_SO_GAIN)                             , 0x00, 0x03 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_PCIE_USB3_UNI_PCS_P2U3_WAKEUP_DLY_TIME_AUXCLK_L)     , 0x00, 0x01 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_PCIE_USB3_UNI_PCS_PCIE_OSC_DTCT_ACTIONS)             , 0x00, 0x00 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_PCIE_USB3_UNI_PCS_PCIE_L1P1_WAKEUP_DLY_TIME_AUXCLK_L), 0x00, 0x01 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_PCIE_USB3_UNI_PCS_PCIE_L1P2_WAKEUP_DLY_TIME_AUXCLK_L), 0x00, 0x01 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_PCIE_USB3_UNI_PCS_PCIE_INT_AUX_CLK_CONFIG1)          , 0x00, 0x00 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_PCIE_USB3_UNI_PCS_PCIE_PRESET_P10_PRE)               , 0x00, 0x00 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_PCIE_USB3_UNI_PCS_PCIE_PRESET_P10_POST)              , 0x00, 0x58 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_COM_CLK_ENABLE1)                             , 0x00, 0x90 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_PCIE_USB3_UNI_PCS_PCIE_ENDPOINT_REFCLK_DRIVE)        , 0x00, 0xC1 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_PCIE_USB3_UNI_PCS_RX_SIGDET_LVL)                     , 0x00, 0xAA },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_PCIE_USB3_UNI_PCS_RATE_SLEW_CNTRL1)                  , 0x00, 0x0B },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_PCIE_USB3_UNI_PCS_REFGEN_REQ_CONFIG1)                , 0x00, 0x0D },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_PCIE_USB3_UNI_PCS_EQ_CONFIG5)                        , 0x00, 0x01 },

  { invalid_reg, PHY_REG_NOP, 0xFF, 0xFF, 0xFF },
};

/*******************************************************************************
 * V2 Chipset specific port 0 PHY init sequence from IPCAT QMP HPG

 ******************************************************************************/
STATIC CONST pciePhyRegSeq_t v2_port_0_phy_init_seq[] =
{
  /* Register Base, Operation, Offset, Mask & Value */
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_PCIE_PARF_DEVICE_TYPE)                               , 0x00, 0x04 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_PCIE_USB3_UNI_PCS_POWER_DOWN_CONTROL)                , 0x00, 0x03 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_COM_SYSCLK_EN_SEL)                           , 0x00, 0x08 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_COM_CLK_SELECT)                              , 0x00, 0x34 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_COM_CORECLK_DIV_MODE1)                       , 0x00, 0x08 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_COM_PLL_IVCO)                                , 0x00, 0x0F },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_COM_LOCK_CMP_EN)                             , 0x00, 0x42 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_COM_VCO_TUNE1_MODE0)                         , 0x00, 0x24 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_COM_VCO_TUNE2_MODE1)                         , 0x00, 0x03 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_COM_VCO_TUNE1_MODE1)                         , 0x00, 0xB4 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_COM_VCO_TUNE_MAP)                            , 0x00, 0x02 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_COM_BIN_VCOCAL_HSCLK_SEL)                    , 0x00, 0x11 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_COM_DEC_START_MODE0)                         , 0x00, 0x82 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_COM_DIV_FRAC_START3_MODE0)                   , 0x00, 0x03 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_COM_DIV_FRAC_START2_MODE0)                   , 0x00, 0x55 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_COM_DIV_FRAC_START1_MODE0)                   , 0x00, 0x55 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_COM_LOCK_CMP2_MODE0)                         , 0x00, 0x1A },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_COM_LOCK_CMP1_MODE0)                         , 0x00, 0x0A },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_COM_DEC_START_MODE1)                         , 0x00, 0x68 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_COM_DIV_FRAC_START3_MODE1)                   , 0x00, 0x02 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_COM_DIV_FRAC_START2_MODE1)                   , 0x00, 0xAA },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_COM_DIV_FRAC_START1_MODE1)                   , 0x00, 0xAB },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_COM_LOCK_CMP2_MODE1)                         , 0x00, 0x34 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_COM_LOCK_CMP1_MODE1)                         , 0x00, 0x14 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_COM_HSCLK_SEL)                               , 0x00, 0x01 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_COM_CP_CTRL_MODE0)                           , 0x00, 0x06 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_COM_PLL_RCTRL_MODE0)                         , 0x00, 0x16 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_COM_PLL_CCTRL_MODE0)                         , 0x00, 0x36 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_COM_CP_CTRL_MODE1)                           , 0x00, 0x06 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_COM_PLL_RCTRL_MODE1)                         , 0x00, 0x16 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_COM_PLL_CCTRL_MODE1)                         , 0x00, 0x36 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_COM_BIN_VCOCAL_CMP_CODE2_MODE0)              , 0x00, 0x1E },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_COM_BIN_VCOCAL_CMP_CODE1_MODE0)              , 0x00, 0xB9 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_COM_BIN_VCOCAL_CMP_CODE2_MODE1)              , 0x00, 0x18 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_COM_BIN_VCOCAL_CMP_CODE1_MODE1)              , 0x00, 0x94 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_COM_SYSCLK_BUF_ENABLE)                       , 0x00, 0x07 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_COM_SSC_EN_CENTER)                           , 0x00, 0x00 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_COM_SSC_PER1)                                , 0x00, 0x31 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_COM_SSC_PER2)                                , 0x00, 0x01 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_COM_SSC_STEP_SIZE1_MODE0)                    , 0x00, 0xDE },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_COM_SSC_STEP_SIZE2_MODE0)                    , 0x00, 0x07 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_COM_SSC_STEP_SIZE1_MODE1)                    , 0x00, 0x4C },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_COM_SSC_STEP_SIZE2_MODE1)                    , 0x00, 0x06 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_TX_RCV_DETECT_LVL_2)                         , 0x00, 0x12 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_TX_LANE_MODE_1)                              , 0x00, 0x35 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_TX_RES_CODE_LANE_OFFSET_TX)                  , 0x00, 0x11 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_RX_SIGDET_CNTRL)                             , 0x00, 0x03 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_RX_SIGDET_ENABLES)                           , 0x00, 0x1C },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_RX_SIGDET_DEGLITCH_CNTRL)                    , 0x00, 0x1E },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_RX_RX_EQU_ADAPTOR_CNTRL1)                    , 0x00, 0x00 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_RX_RX_EQU_ADAPTOR_CNTRL2)                    , 0x00, 0x0E },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_RX_RX_EQU_ADAPTOR_CNTRL3)                    , 0x00, 0x4A },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_RX_RX_EQU_ADAPTOR_CNTRL4)                    , 0x00, 0x0F },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_RX_DFE_EN_TIMER)                             , 0x00, 0x04 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_RX_UCDR_SO_SATURATION_AND_ENABLE)            , 0x00, 0x7F },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_RX_UCDR_PI_CONTROLS)                         , 0x00, 0x70 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_RX_RX_EQ_OFFSET_ADAPTOR_CNTRL1)              , 0x00, 0x17 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_RX_VGA_CAL_CNTRL1)                           , 0x00, 0x04 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_RX_VGA_CAL_CNTRL2)                           , 0x00, 0x07 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_RX_RX_MODE_10_LOW)                           , 0x00, 0xD4 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_RX_RX_MODE_10_HIGH)                          , 0x00, 0x54 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_RX_RX_MODE_10_HIGH2)                         , 0x00, 0xDB },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_RX_RX_MODE_10_HIGH3)                         , 0x00, 0x3B },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_RX_RX_MODE_10_HIGH4)                         , 0x00, 0x31 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_RX_RX_MODE_01_LOW)                           , 0x00, 0x24 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_RX_RX_MODE_01_HIGH)                          , 0x00, 0xE4 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_RX_RX_MODE_01_HIGH2)                         , 0x00, 0xEC },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_RX_RX_MODE_01_HIGH3)                         , 0x00, 0x3B },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_RX_RX_MODE_01_HIGH4)                         , 0x00, 0x36 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_RX_RX_MODE_00_LOW)                           , 0x00, 0xFF },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_RX_RX_MODE_00_HIGH)                          , 0x00, 0xFF },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_RX_RX_MODE_00_HIGH2)                         , 0x00, 0xFF },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_RX_RX_MODE_00_HIGH3)                         , 0x00, 0x7F },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_RX_RX_MODE_00_HIGH4)                         , 0x00, 0x66 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_RX_RX_IDAC_TSETTLE_HIGH)                     , 0x00, 0x00 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_RX_RX_IDAC_TSETTLE_LOW)                      , 0x00, 0xC0 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_RX_AUX_DATA_TCOARSE_TFINE)                   , 0x00, 0x30 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_RX_RCLK_AUXDATA_SEL)                         , 0x00, 0x00 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_RX_DCC_CTRL1)                                , 0x00, 0x0C },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_RX_GM_CAL)                                   , 0x00, 0x0D },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_RX_UCDR_FO_GAIN)                             , 0x00, 0x0C },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_RX_UCDR_SO_GAIN)                             , 0x00, 0x03 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_PCIE_USB3_UNI_PCS_P2U3_WAKEUP_DLY_TIME_AUXCLK_L)     , 0x00, 0x01 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_PCIE_USB3_UNI_PCS_PCIE_OSC_DTCT_ACTIONS)             , 0x00, 0x00 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_PCIE_USB3_UNI_PCS_PCIE_L1P1_WAKEUP_DLY_TIME_AUXCLK_L), 0x00, 0x01 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_PCIE_USB3_UNI_PCS_PCIE_L1P2_WAKEUP_DLY_TIME_AUXCLK_L), 0x00, 0x01 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_PCIE_USB3_UNI_PCS_PCIE_INT_AUX_CLK_CONFIG1)          , 0x00, 0x00 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_PCIE_USB3_UNI_PCS_PCIE_PRESET_P6_P7_PRE)             , 0x00, 0x33 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_PCIE_USB3_UNI_PCS_PCIE_PRESET_P10_PRE)               , 0x00, 0x00 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_PCIE_USB3_UNI_PCS_PCIE_PRESET_P10_POST)              , 0x00, 0x58 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_PCIE_USB3_UNI_PCS_PCIE_EQ_CONFIG2)                   , 0x00, 0x0F },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_QSERDES_COM_CLK_ENABLE1)                             , 0x00, 0x90 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_PCIE_USB3_UNI_PCS_PCIE_ENDPOINT_REFCLK_DRIVE)        , 0x00, 0xC1 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_PCIE_USB3_UNI_PCS_RX_SIGDET_LVL)                     , 0x00, 0x88 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_PCIE_USB3_UNI_PCS_RATE_SLEW_CNTRL1)                  , 0x00, 0x0B },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_PCIE_USB3_UNI_PCS_REFGEN_REQ_CONFIG1)                , 0x00, 0x0D },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_PCIE_USB3_UNI_PCS_EQ_CONFIG5)                        , 0x00, 0x12 },
  { invalid_reg, PHY_REG_NOP, 0xFF, 0xFF, 0xFF },
};

/*******************************************************************************
 * Chipset specific version independent port 0 PHY power ON sequence
 ******************************************************************************/
STATIC CONST pciePhyRegSeq_t port_0_phy_power_on_seq[] =
{
  /* Register Base, Operation, Offset, Mask & Value */
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_PCIE_USB3_UNI_PCS_SW_RESET)                          , 0x00, 0x00 }, // Release software reset
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_PCIE_USB3_UNI_PCS_START_CONTROL)                     , 0x00, 0x03 }, // Start PCS and SERDES power SMs
  { invalid_reg, PHY_REG_NOP, 0xFF, 0xFF, 0xFF },
};

/*******************************************************************************
 * PLL takes ~1 ms to lock-in
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
STATIC CONST pciePhyRegSeq_t port_0_phy_delay_seq[] =
{
  /* Register Base, Operation, Offset, Mask & Value */
  { invalid_reg, PHY_REG_DELAY, 0xFF, 0xFF, 2000 },
  { invalid_reg, PHY_REG_NOP, 0xFF, 0xFF, 0xFF },
};

/*******************************************************************************
 * poll PHY power ON status of port 0 PHY
 ******************************************************************************/
STATIC CONST pciePhyRegSeq_t port_0_phy_poll_seq[] =
{
  /* Register Base, Operation, Offset, Mask & Value */
  { ahb_reg,     PHY_REG_REA, HWIO_OFFS(PCIE_GEN3X1_PCIE_USB3_UNI_PCS_PCS_STATUS1), HWIO_PCIE_GEN3X1_PCIE_USB3_UNI_PCS_PCS_STATUS1_PHYSTATUS_BMSK, FALSE },
  { invalid_reg, PHY_REG_NOP, 0xFF, 0xFF, 0xFF },
};

/*******************************************************************************
 * post PHY power ON sequence for port 0 PHY
 ******************************************************************************/
STATIC CONST pciePhyRegSeq_t port_0_phy_post_power_up_seq[] =
{
  /* Register Base, Operation, Offset, Mask & Value */
  /* Enable BME & MSE before LTSSM_EN */
  { axi_reg,     PHY_REG_RMW, HWIO_OFFS(PCIE_GEN3X1_TYPE1_STATUS_COMMAND_REG), HWIO_PCIE_GEN3X1_TYPE1_STATUS_COMMAND_REG_MSE_BMSK, (0x1 << HWIO_PCIE_GEN3X1_TYPE1_STATUS_COMMAND_REG_MSE_SHFT)},
  { axi_reg,     PHY_REG_RMW, HWIO_OFFS(PCIE_GEN3X1_TYPE1_STATUS_COMMAND_REG), HWIO_PCIE_GEN3X1_TYPE1_STATUS_COMMAND_REG_BME_BMSK, (0x1 << HWIO_PCIE_GEN3X1_TYPE1_STATUS_COMMAND_REG_BME_SHFT)},
  { axi_reg,     PHY_REG_RMW, HWIO_OFFS(PCIE_GEN3X1_GEN3_EQ_FB_MODE_DIR_CHANGE_OFF), HWIO_PCIE_GEN3X1_GEN3_EQ_FB_MODE_DIR_CHANGE_OFF_GEN3_EQ_FMDC_N_EVALS_BMSK, (0x0D << HWIO_PCIE_GEN3X1_GEN3_EQ_FB_MODE_DIR_CHANGE_OFF_GEN3_EQ_FMDC_N_EVALS_SHFT)},
  { axi_reg,     PHY_REG_RMW, HWIO_OFFS(PCIE_GEN3X1_GEN3_EQ_FB_MODE_DIR_CHANGE_OFF), HWIO_PCIE_GEN3X1_GEN3_EQ_FB_MODE_DIR_CHANGE_OFF_GEN3_EQ_FMDC_MAX_PRE_CUSROR_DELTA_BMSK, (0x05 << HWIO_PCIE_GEN3X1_GEN3_EQ_FB_MODE_DIR_CHANGE_OFF_GEN3_EQ_FMDC_MAX_PRE_CUSROR_DELTA_SHFT)},
  { axi_reg,     PHY_REG_RMW, HWIO_OFFS(PCIE_GEN3X1_GEN3_EQ_FB_MODE_DIR_CHANGE_OFF), HWIO_PCIE_GEN3X1_GEN3_EQ_FB_MODE_DIR_CHANGE_OFF_GEN3_EQ_FMDC_MAX_POST_CUSROR_DELTA_BMSK, (0x05 << HWIO_PCIE_GEN3X1_GEN3_EQ_FB_MODE_DIR_CHANGE_OFF_GEN3_EQ_FMDC_MAX_POST_CUSROR_DELTA_SHFT)},
  { axi_reg,     PHY_REG_RMW, HWIO_OFFS(PCIE_GEN3X1_GEN3_EQ_FB_MODE_DIR_CHANGE_OFF), HWIO_PCIE_GEN3X1_GEN3_EQ_FB_MODE_DIR_CHANGE_OFF_GEN3_EQ_FMDC_T_MIN_PHASE23_BMSK, 0x00 },
  { axi_reg,     PHY_REG_RMW, HWIO_OFFS(PCIE_GEN3X1_GEN3_EQ_CONTROL_OFF), HWIO_PCIE_GEN3X1_GEN3_EQ_CONTROL_OFF_GEN3_EQ_PHASE23_EXIT_MODE_BMSK, 0x00 },
  { axi_reg,     PHY_REG_RMW, HWIO_OFFS(PCIE_GEN3X1_GEN3_EQ_CONTROL_OFF), HWIO_PCIE_GEN3X1_GEN3_EQ_CONTROL_OFF_GEN3_EQ_PSET_REQ_VEC_BMSK, 0x00 },
  /* Writing Slave address space size */
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_PCIE_PARF_SLV_ADDR_SPACE_SIZE), 0x0, 0x2000000},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_PCIE_PARF_SLV_ADDR_SPACE_SIZE_HI), 0x0, 0x00000000},
  /* Clear REQ_NOT_ENTER_L1 field in PARF PM_CTRL to allow L1 entry */
  { ahb_reg,     PHY_REG_RMW, HWIO_OFFS(PCIE_GEN3X1_PCIE_PARF_PM_CTRL), HWIO_PCIE_GEN3X1_PCIE_PARF_PM_CTRL_REQ_NOT_ENTR_L1_BMSK, 0x00000000},
  /* Enable DBI_RO_WR_EN to access CS1 region */
  { axi_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_MISC_CONTROL_1_OFF), 0x0, 0x1},
  /* Memory Barrier */
  { invalid_reg, PHY_REG_DSB, 0xFF, 0xFF, 0xFF},
  /* Update Base class and Sub class */
  { axi_reg,     PHY_REG_RMW, HWIO_OFFS(PCIE_GEN3X1_TYPE1_CLASS_CODE_REV_ID_REG),
    (HWIO_PCIE_GEN3X1_TYPE1_CLASS_CODE_REV_ID_REG_BASE_CLASS_CODE_BMSK | HWIO_PCIE_GEN3X1_TYPE1_CLASS_CODE_REV_ID_REG_SUBCLASS_CODE_BMSK),
    (PCIE_CLASS_CODE_BRIDGE_DEVICE | PCIE_SUBCLASS_CODE_PCI_2_PCI_BRIDGE)},
  { axi_reg,     PHY_REG_RMW, HWIO_OFFS(PCIE_GEN3X1_SLOT_CAPABILITIES_REG), HWIO_PCIE_GEN3X1_SLOT_CAPABILITIES_REG_PCIE_CAP_HOT_PLUG_CAPABLE_BMSK, 0x00000000},
  /* Assert CS2 */
  { axi_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_PCIE_ELBI_CS2_ENABLE), 0x0, 0x1},
  /* Memory Barrier */
  { invalid_reg, PHY_REG_DSB, 0xFF, 0xFF, 0xFF},
  /* Disable BAR0 and BAR1 */
  { axi_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_TYPE1_BAR0_REG), 0x0, 0x0},
  { axi_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_TYPE1_BAR1_REG), 0x0, 0x0},
  { axi_reg,     PHY_REG_RMW, HWIO_OFFS(PCIE_GEN3X1_GEN2_CTRL_OFF), 0x1f00UL, (0x1 << HWIO_PCIE_GEN3X1_GEN2_CTRL_OFF_NUM_OF_LANES_SHFT) },
  //{ axi_reg,     PHY_REG_RMW, HWIO_OFFS(PCIE_GEN3X1_GEN2_CTRL_OFF), HWIO_PCIE_GEN3X1_GEN2_CTRL_OFF_DIRECT_SPEED_CHANGE_BMSK, (0x0) },
  { axi_reg,     PHY_REG_RMW, HWIO_OFFS(PCIE_GEN3X1_GEN3_RELATED_OFF), HWIO_PCIE_GEN3X1_GEN3_RELATED_OFF_GEN3_ZRXDC_NONCOMPL_BMSK, 0x00 },
  /* De-Assert CS2 */
  { axi_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_PCIE_ELBI_CS2_ENABLE), 0x0, 0x0},
  /* Disable DBI_RO_WR_EN to access CS1 region */
  { axi_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_MISC_CONTROL_1_OFF), 0x0, 0x0},
  /* Memory Barrier */
  { invalid_reg, PHY_REG_DSB, 0xFF, 0xFF, 0xFF},
  { invalid_reg, PHY_REG_NOP, 0xFF, 0xFF, 0xFF},
};



/*******************************************************************************
 * Chipset specific complete port PHY init sequence
 ******************************************************************************/
STATIC CONST pciePhyAllVerSeq_t sdm1000_port_0_phy_init_seq[] =
{
  /* First write the register sequence corresponds to version 1 */
  {Pcie_Qcom_SoC_Ver_1,    v1_port_0_phy_init_seq},
  /* Write the register sequence corresponds to version 2 */
  {Pcie_Qcom_SoC_Ver_2,    v2_port_0_phy_init_seq},
  /* Then write the register sequence corresponds to version 3 and up if any */
  /* Once all the version dependent sequences are written, power ON PHY */
  {Pcie_Qcom_SoC_Ver_ALL,  port_0_phy_power_on_seq},
  /* Once the PHY is brought of reset, give a finite delay */
  {Pcie_Qcom_SoC_Ver_ALL,  port_0_phy_delay_seq},
  /* Check if the PHY PLL is locked */
  {Pcie_Qcom_SoC_Ver_POLL, port_0_phy_poll_seq},
  /* Perform port PHY init sequences before link training */
  {Pcie_Qcom_SoC_Ver_ALL, port_0_phy_post_power_up_seq},
  /* End of PHY init sequence */
  {Pcie_Qcom_SoC_Ver_STOP, NULL},
};

/*******************************************************************************
 * Chipset specific port 0 PHY De-init sequence
 ******************************************************************************/
STATIC CONST pciePhyRegSeq_t port_0_phy_de_init_seq[] =
{
  /* Register Base, Operation, Offset, Mask & Value */
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X1_PCIE_USB3_UNI_PCS_POWER_DOWN_CONTROL                    ), 0x00, 0x00},
  { invalid_reg, PHY_REG_NOP, 0xFF, 0xFF, 0xFF },
};
#endif /* (RPMASK & PCIE_RP(0)) - root port 0 enabled */

/*******************************************************************************
 * This section will hold the port info data for root port 1
 ******************************************************************************/
#if (RPMASK & PCIE_RP(1))

/* Number of PCIe ports present in Gen3X2 PCIe Root Port */
#define NUM_PCIE_1_PORTS 0x01

/*******************************************************************************
 * Register region information for PCIe GEN3 X2 root port
 ******************************************************************************/
static memRegions_t port_1_reg[max_mem_reg] =
{
  /**< axi_reg: DBI register space */
  {.pa=PCIE_GEN3X2_PCIE_WRAPPER_AXI_NO_EDMA_BASE, .sz=PCIE_GEN3X2_PCIE_WRAPPER_AXI_NO_EDMA_BASE_SIZE},
  /**< ahb_reg: PARF register space */
  {.pa=PCIE_GEN3X2_PCIE_WRAPPER_AHB_W_PHY_G3X2_BASE, .sz=PCIE_GEN3X2_PCIE_WRAPPER_AHB_W_PHY_G3X2_BASE_SIZE},
  /**< tcsr_reg: Access TCSR register space */
  {.pa=CORE_TOP_CSR_BASE, .sz=CORE_TOP_CSR_BASE_SIZE},
  /**< invalid_reg: Invalid reg base to end the PHY init sequence */
  {.pa=PCIE_INVALID_REG_BASE, .sz=PCIE_INVALID_REG_SIZE}
};

/*******************************************************************************
 * PCIe port reset clock name list for root port 1 with NULL ending
 ******************************************************************************/
STATIC CONST char * pcie_reset_clks_rp1[] =
{
  "gcc_pcie_1_aux_clk",
  NULL
};

/*******************************************************************************
 * PCIe port external clock name list for root port 1 with NULL ending
 ******************************************************************************/
STATIC CONST char * pcie_external_clks_rp1[] =
{
  "gcc_pcie_1_pipe_clk",
  NULL
};

/*******************************************************************************
 * PCIe port gcc clock name list for root port 1 with NULL ending
 ******************************************************************************/
STATIC CONST char * pcie_gcc_clks_rp1[] =
{
  "gcc_pcie_1_aux_clk",
  "gcc_pcie_1_slv_axi_clk",
  "gcc_pcie_1_slv_q2a_axi_clk",
  "gcc_pcie_1_mstr_axi_clk",
  "gcc_pcie_1_cfg_ahb_clk",
  "gcc_pcie1_phy_refgen_clk",
  NULL
};

/*******************************************************************************
 * PCIe port aggregated gcc clock name list for root port 1
 ******************************************************************************/
STATIC CONST pcieClkDesc_t pcie_clk_rp1[] =
{
  {
    .reset = pcie_reset_clks_rp1,
    .external = pcie_external_clks_rp1,
    .gcc = pcie_gcc_clks_rp1
  }
};

/*******************************************************************************
 * ICB bandwidth request for pcie root port 1
 ******************************************************************************/
STATIC CONST ICBArb_RequestType pcie_1_icb_request[] =
{
  {
    .arbType = ICBARB_REQUEST_TYPE_3,
    .arbData.type3 =
    {
      .uIb = ((400ULL) << 20), /**< Instantaneous bandwidth in Bytes/second */
      .uAb = ((400ULL) << 20), /**< Arbitrated bandwidth in Bytes/second */
      .uLatencyNs = 0          /**< Latency requirement in nanoseconds */
    }
  }
};

/*******************************************************************************
 * ICB Master and Slave list for pcie root port 1
 ******************************************************************************/
STATIC CONST ICBArb_MasterSlaveType pcie_1_icb_mstr_slv_lst[] =
{
  {
    .eMaster = ICBID_MASTER_PCIE_1,
    .eSlave = ICBID_SLAVE_EBI1
  }
};

/*******************************************************************************
 * Aggreagate bus bandwidth vote for pcie root port 1
 ******************************************************************************/
STATIC pcieIcbClkBW_t pcie_1_icb_bw[] =
{
  {
    .npa_client_name = "pcie_1",
    .npa_hdl = NULL,
    .ms_list = pcie_1_icb_mstr_slv_lst,
    .ms_size = sizeof(pcie_1_icb_mstr_slv_lst),
    .req_list = pcie_1_icb_request,
    .req_size = sizeof(pcie_1_icb_request)
  }
};

/*******************************************************************************
 * PCIe root port 1 clock settings
 ******************************************************************************/
CONST clkFreqSettings_t pcie_1_clk_settings[] =
{
  {.freq = 19200000,  .name = "gcc_pcie_1_aux_clk"},
  {.freq = 100000000, .name = "gcc_pcie1_phy_refgen_clk"},
  {.freq = 0, .name = NULL}
};

/*******************************************************************************
 * GPIO configuration for RP1
 *
 * This array will be access from index 0 upto the end of
 * array to to configure all the GPIOs before performing
 * link training
 ******************************************************************************/
/*
 * ENDP GPIO configuration for RP1
 */
STATIC CONST gpioPinConfig_t gpio_config_rp1[] =
{
  {
   .type = MSM_GPIO,
   {
    .TlmmGpioParam.GpioNum = 103,// CLK_REQ
    .TlmmGpioParam.Func = 1,
    .TlmmGpioParam.Direction = GPIO_INPUT,
    .TlmmGpioParam.Pull = GPIO_PULL_UP,
    .TlmmGpioParam.DriveStrength = GPIO_2MA,
    .TlmmGpioParam.value = GPIO_LOW_VALUE
   }
  }
  ,
  {
   .type = MSM_GPIO,
   {
    .TlmmGpioParam.GpioNum = 175, // PCIE1_RST
    .TlmmGpioParam.Func = 0,
    .TlmmGpioParam.Direction = GPIO_OUTPUT,
    .TlmmGpioParam.Pull = GPIO_NO_PULL,
    .TlmmGpioParam.DriveStrength = GPIO_2MA,
    .TlmmGpioParam.value = GPIO_LOW_VALUE
   }
  }
  ,
  {
   .type = MSM_GPIO,
   {
    .TlmmGpioParam.GpioNum = 177, // PCIE1_WAKE
    .TlmmGpioParam.Func = 0,
    .TlmmGpioParam.Direction = GPIO_INPUT,
    .TlmmGpioParam.Pull = GPIO_NO_PULL,
    .TlmmGpioParam.DriveStrength = GPIO_2MA,
    .TlmmGpioParam.value = GPIO_LOW_VALUE
   }
  }
  ,
  {
   .type = DELAY,
   {
    .DelayValue = 150 // 150 micro seconds before REFCLK gets stable
   }
  }
  ,
  {
   .type = MSM_GPIO,
   {
    .TlmmGpioParam.GpioNum = 175, // PCIE1_RST
    .TlmmGpioParam.Func = 0,
    .TlmmGpioParam.Direction = GPIO_OUTPUT,
    .TlmmGpioParam.Pull = GPIO_NO_PULL,
    .TlmmGpioParam.DriveStrength = GPIO_2MA,
    .TlmmGpioParam.value = GPIO_HIGH_VALUE
   }
  }
  ,
  {
   .type = DELAY,
   {
    .DelayValue = 10000 // 10 ms delay for EP device preparation for Link Training
   }
  }
  ,
  /* Default and end of sequence entry */
  {
   .type = NONE,
   {
    .DelayValue = NONE
   }
  }
};

/*******************************************************************************
 * Link info
 ******************************************************************************/
STATIC pcieLink_t port1_link[] =
{
  {
    .trained             = FALSE,
    .num_lanes           = 2,
    .aspm                = aspm_l1_ena_l0s_ena,
    .ltr_threshold_val   = 0,
    .ltr_threshold_scale = LTR_TH_SCALE_1ns
  }
};

/*******************************************************************************
 * V1 Chipset specific port 1 PHY init sequence
 ******************************************************************************/
STATIC CONST pciePhyRegSeq_t v1_port_1_phy_init_seq[] =
{
  /* Register Base, Operation, Offset, Mask & Value */
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_PCIE_PARF_DEVICE_TYPE)                             , 0x00, 0x04 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BPCS_COM_POWER_DOWN_CONTROL)               , 0x00, 0x03 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_SSC_EN_CENTER)                , 0x00, 0x00 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_SSC_PER1)                     , 0x00, 0x31 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_SSC_PER2)                     , 0x00, 0x01 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_SSC_STEP_SIZE1_MODE0)         , 0x00, 0xDE },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_SSC_STEP_SIZE2_MODE0)         , 0x00, 0x07 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_SSC_STEP_SIZE1_MODE1)         , 0x00, 0x4C },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_SSC_STEP_SIZE2_MODE1)         , 0x00, 0x06 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_CLK_ENABLE1)                  , 0x00, 0x90 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_PLL_IVCO)                     , 0x00, 0x0F },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_CP_CTRL_MODE0)                , 0x00, 0x06 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_CP_CTRL_MODE1)                , 0x00, 0x06 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_PLL_RCTRL_MODE0)              , 0x00, 0x16 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_PLL_RCTRL_MODE1)              , 0x00, 0x16 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_PLL_CCTRL_MODE0)              , 0x00, 0x36 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_PLL_CCTRL_MODE1)              , 0x00, 0x36 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_SYSCLK_EN_SEL)                , 0x00, 0x08 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_LOCK_CMP_EN)                  , 0x00, 0x42 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_LOCK_CMP1_MODE0)              , 0x00, 0x0A },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_LOCK_CMP2_MODE0)              , 0x00, 0x1A },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_LOCK_CMP1_MODE1)              , 0x00, 0x14 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_LOCK_CMP2_MODE1)              , 0x00, 0x34 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_DEC_START_MODE0)              , 0x00, 0x82 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_DEC_START_MODE1)              , 0x00, 0x68 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_DIV_FRAC_START1_MODE0)        , 0x00, 0x55 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_DIV_FRAC_START2_MODE0)        , 0x00, 0x55 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_DIV_FRAC_START3_MODE0)        , 0x00, 0x03 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_DIV_FRAC_START1_MODE1)        , 0x00, 0xAB },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_DIV_FRAC_START2_MODE1)        , 0x00, 0xAA },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_DIV_FRAC_START3_MODE1)        , 0x00, 0x02 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_VCO_TUNE_MAP)                 , 0x00, 0x02 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_VCO_TUNE1_MODE0)              , 0x00, 0x24 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_VCO_TUNE1_MODE1)              , 0x00, 0xB4 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_VCO_TUNE2_MODE1)              , 0x00, 0x03 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_CLK_SELECT)                   , 0x00, 0x34 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_HSCLK_SEL)                    , 0x00, 0x01 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_CORECLK_DIV_MODE1)            , 0x00, 0x08 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_BIN_VCOCAL_CMP_CODE1_MODE0)   , 0x00, 0xB9 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_BIN_VCOCAL_CMP_CODE2_MODE0)   , 0x00, 0x1E },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_BIN_VCOCAL_CMP_CODE1_MODE1)   , 0x00, 0x94 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_BIN_VCOCAL_CMP_CODE2_MODE1)   , 0x00, 0x18 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_BIN_VCOCAL_HSCLK_SEL)         , 0x00, 0x11 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_TX0_LANE_MODE_1)                  , 0x00, 0x05 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_TX0_RCV_DETECT_LVL_2)             , 0x00, 0x12 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX0_UCDR_FO_GAIN)                 , 0x00, 0x0C },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX0_UCDR_SO_GAIN)                 , 0x00, 0x03 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX0_UCDR_SO_SATURATION_AND_ENABLE), 0x00, 0x7F },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX0_UCDR_PI_CONTROLS)             , 0x00, 0x70 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX0_AUX_DATA_TCOARSE_TFINE)       , 0x00, 0xA0 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX0_RCLK_AUXDATA_SEL)             , 0x00, 0x00 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX0_VGA_CAL_CNTRL1)               , 0x00, 0x54 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX0_VGA_CAL_CNTRL2)               , 0x00, 0x37 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX0_GM_CAL)                       , 0x00, 0x05 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX0_RX_EQU_ADAPTOR_CNTRL1)        , 0x00, 0x07 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX0_RX_EQU_ADAPTOR_CNTRL2)        , 0x00, 0x6E },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX0_RX_EQU_ADAPTOR_CNTRL3)        , 0x00, 0x6E },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX0_RX_EQU_ADAPTOR_CNTRL4)        , 0x00, 0x4A },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX0_RX_IDAC_TSETTLE_LOW)          , 0x00, 0xC0 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX0_RX_IDAC_TSETTLE_HIGH)         , 0x00, 0x00 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX0_RX_EQ_OFFSET_ADAPTOR_CNTRL1)  , 0x00, 0x17 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX0_SIGDET_ENABLES)               , 0x00, 0x1C },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX0_SIGDET_CNTRL)                 , 0x00, 0x03 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX0_SIGDET_DEGLITCH_CNTRL)        , 0x00, 0x14 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX0_RX_MODE_00_LOW)               , 0x00, 0x7F },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX0_RX_MODE_00_HIGH)              , 0x00, 0xFF },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX0_RX_MODE_00_HIGH2)             , 0x00, 0xFF },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX0_RX_MODE_00_HIGH3)             , 0x00, 0xDB },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX0_RX_MODE_00_HIGH4)             , 0x00, 0x75 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX0_RX_MODE_01_LOW)               , 0x00, 0x24 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX0_RX_MODE_01_HIGH)              , 0x00, 0xE4 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX0_RX_MODE_01_HIGH2)             , 0x00, 0xEC },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX0_RX_MODE_01_HIGH3)             , 0x00, 0x39 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX0_RX_MODE_01_HIGH4)             , 0x00, 0x36 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX0_RX_MODE_10_LOW)               , 0x00, 0xD4 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX0_RX_MODE_10_HIGH)              , 0x00, 0x54 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX0_RX_MODE_10_HIGH2)             , 0x00, 0xDB },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX0_RX_MODE_10_HIGH3)             , 0x00, 0x39 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX0_RX_MODE_10_HIGH4)             , 0x00, 0x31 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX0_DCC_CTRL1)                    , 0x00, 0x0C },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_TX1_LANE_MODE_1)                  , 0x00, 0x05 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_TX1_RCV_DETECT_LVL_2)             , 0x00, 0x12 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX1_UCDR_FO_GAIN)                 , 0x00, 0x0C },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX1_UCDR_SO_GAIN)                 , 0x00, 0x03 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX1_UCDR_SO_SATURATION_AND_ENABLE), 0x00, 0x7F },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX1_UCDR_PI_CONTROLS)             , 0x00, 0x70 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX1_AUX_DATA_TCOARSE_TFINE)       , 0x00, 0xA0 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX1_RCLK_AUXDATA_SEL)             , 0x00, 0x00 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX1_VGA_CAL_CNTRL1)               , 0x00, 0x54 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX1_VGA_CAL_CNTRL2)               , 0x00, 0x37 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX1_GM_CAL)                       , 0x00, 0x05 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX1_RX_EQU_ADAPTOR_CNTRL1)        , 0x00, 0x07 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX1_RX_EQU_ADAPTOR_CNTRL2)        , 0x00, 0x6E },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX1_RX_EQU_ADAPTOR_CNTRL3)        , 0x00, 0x6E },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX1_RX_EQU_ADAPTOR_CNTRL4)        , 0x00, 0x4A },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX1_RX_IDAC_TSETTLE_LOW)          , 0x00, 0xC0 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX1_RX_IDAC_TSETTLE_HIGH)         , 0x00, 0x00 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX1_RX_EQ_OFFSET_ADAPTOR_CNTRL1)  , 0x00, 0x17 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX1_SIGDET_ENABLES)               , 0x00, 0x1C },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX1_SIGDET_CNTRL)                 , 0x00, 0x03 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX1_SIGDET_DEGLITCH_CNTRL)        , 0x00, 0x14 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX1_RX_MODE_00_LOW)               , 0x00, 0x7F },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX1_RX_MODE_00_HIGH)              , 0x00, 0xFF },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX1_RX_MODE_00_HIGH2)             , 0x00, 0xFF },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX1_RX_MODE_00_HIGH3)             , 0x00, 0xDB },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX1_RX_MODE_00_HIGH4)             , 0x00, 0x75 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX1_RX_MODE_01_LOW)               , 0x00, 0x24 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX1_RX_MODE_01_HIGH)              , 0x00, 0xE4 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX1_RX_MODE_01_HIGH2)             , 0x00, 0xEC },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX1_RX_MODE_01_HIGH3)             , 0x00, 0x39 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX1_RX_MODE_01_HIGH4)             , 0x00, 0x36 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX1_RX_MODE_10_LOW)               , 0x00, 0xD4 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX1_RX_MODE_10_HIGH)              , 0x00, 0x54 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX1_RX_MODE_10_HIGH2)             , 0x00, 0xDB },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX1_RX_MODE_10_HIGH3)             , 0x00, 0x39 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX1_RX_MODE_10_HIGH4)             , 0x00, 0x31 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX1_DCC_CTRL1)                    , 0x00, 0x0C },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BPCS_COM_REFGEN_REQ_CONFIG1)               , 0x00, 0x05 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BPCS_COM_RX_SIGDET_LVL)                    , 0x00, 0xAA },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BPCS_COM_RATE_SLEW_CNTRL1)                 , 0x00, 0x0B },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BPCS_COM_P2U3_WAKEUP_DLY_TIME_AUXCLK_L)    , 0x00, 0x01 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BPCS_COM_EQ_CONFIG5)                       , 0x00, 0x01 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BPCS_PCIE_POWER_STATE_CONFIG2)             , 0x00, 0x0D },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BPCS_PCIE_POWER_STATE_CONFIG4)             , 0x00, 0x07 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BPCS_PCIE_ENDPOINT_REFCLK_DRIVE)           , 0x00, 0xC1 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BPCS_PCIE_ENDPOINT_REFCLK_CNTRL)           , 0x00, 0x01 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BPCS_PCIE_L1P1_WAKEUP_DLY_TIME_AUXCLK_L)   , 0x00, 0x01 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BPCS_PCIE_L1P2_WAKEUP_DLY_TIME_AUXCLK_L)   , 0x00, 0x01 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BPCS_PCIE_OSC_DTCT_ACTIONS)                , 0x00, 0x00 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BPCS_PCIE_PRESET_P10_PRE)                  , 0x00, 0x00 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BPCS_PCIE_PRESET_P10_POST)                 , 0x00, 0x58 },

  { invalid_reg, PHY_REG_NOP, 0xFF, 0xFF, 0xFF },
};

/*******************************************************************************
 * V2 Chipset specific port 1 PHY init sequence
 ******************************************************************************/
STATIC CONST pciePhyRegSeq_t v2_port_1_phy_init_seq[] =
{
  /* Register Base, Operation, Offset, Mask & Value */
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_PCIE_PARF_DEVICE_TYPE)                             , 0x00, 0x04 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BPCS_COM_POWER_DOWN_CONTROL)               , 0x00, 0x03 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_SSC_EN_CENTER)                , 0x00, 0x00 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_SSC_PER1)                     , 0x00, 0x31 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_SSC_PER2)                     , 0x00, 0x01 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_SSC_STEP_SIZE1_MODE0)         , 0x00, 0xDE },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_SSC_STEP_SIZE2_MODE0)         , 0x00, 0x07 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_SSC_STEP_SIZE1_MODE1)         , 0x00, 0x4C },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_SSC_STEP_SIZE2_MODE1)         , 0x00, 0x06 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_BIAS_EN_CLKBUFLR_EN)          , 0x00, 0x14 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_CLK_ENABLE1)                  , 0x00, 0x90 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_PLL_IVCO)                     , 0x00, 0x0F },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_CP_CTRL_MODE0)                , 0x00, 0x06 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_CP_CTRL_MODE1)                , 0x00, 0x06 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_PLL_RCTRL_MODE0)              , 0x00, 0x16 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_PLL_RCTRL_MODE1)              , 0x00, 0x16 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_PLL_CCTRL_MODE0)              , 0x00, 0x36 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_PLL_CCTRL_MODE1)              , 0x00, 0x36 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_SYSCLK_EN_SEL)                , 0x00, 0x08 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_LOCK_CMP_EN)                  , 0x00, 0x42 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_LOCK_CMP1_MODE0)              , 0x00, 0x0A },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_LOCK_CMP2_MODE0)              , 0x00, 0x1A },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_LOCK_CMP1_MODE1)              , 0x00, 0x14 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_LOCK_CMP2_MODE1)              , 0x00, 0x34 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_DEC_START_MODE0)              , 0x00, 0x82 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_DEC_START_MODE1)              , 0x00, 0x68 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_DIV_FRAC_START1_MODE0)        , 0x00, 0x55 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_DIV_FRAC_START2_MODE0)        , 0x00, 0x55 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_DIV_FRAC_START3_MODE0)        , 0x00, 0x03 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_DIV_FRAC_START1_MODE1)        , 0x00, 0xAB },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_DIV_FRAC_START2_MODE1)        , 0x00, 0xAA },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_DIV_FRAC_START3_MODE1)        , 0x00, 0x02 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_VCO_TUNE_MAP)                 , 0x00, 0x02 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_VCO_TUNE1_MODE0)              , 0x00, 0x24 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_VCO_TUNE1_MODE1)              , 0x00, 0xB4 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_VCO_TUNE2_MODE1)              , 0x00, 0x03 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_CLK_SELECT)                   , 0x00, 0x34 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_HSCLK_SEL)                    , 0x00, 0x01 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_CORECLK_DIV_MODE1)            , 0x00, 0x08 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_BIN_VCOCAL_CMP_CODE1_MODE0)   , 0x00, 0xB9 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_BIN_VCOCAL_CMP_CODE2_MODE0)   , 0x00, 0x1E },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_BIN_VCOCAL_CMP_CODE1_MODE1)   , 0x00, 0x94 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_BIN_VCOCAL_CMP_CODE2_MODE1)   , 0x00, 0x18 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_PLL_BIN_VCOCAL_HSCLK_SEL)         , 0x00, 0x11 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_TX0_RES_CODE_LANE_OFFSET_TX)      , 0x00, 0x11 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_TX0_LANE_MODE_1)                  , 0x00, 0x35 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_TX0_RCV_DETECT_LVL_2)             , 0x00, 0x12 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX0_UCDR_FO_GAIN)                 , 0x00, 0x0C },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX0_UCDR_SO_GAIN)                 , 0x00, 0x03 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX0_UCDR_SO_SATURATION_AND_ENABLE), 0x00, 0x7F },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX0_UCDR_PI_CONTROLS)             , 0x00, 0x70 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX0_AUX_DATA_TCOARSE_TFINE)       , 0x00, 0x30 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX0_RCLK_AUXDATA_SEL)             , 0x00, 0x00 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX0_VGA_CAL_CNTRL1)               , 0x00, 0x04 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX0_VGA_CAL_CNTRL2)               , 0x00, 0x07 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX0_GM_CAL)                       , 0x00, 0x0D },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX0_RX_EQU_ADAPTOR_CNTRL1)        , 0x00, 0x00 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX0_RX_EQU_ADAPTOR_CNTRL2)        , 0x00, 0x0E },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX0_RX_EQU_ADAPTOR_CNTRL3)        , 0x00, 0x4A },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX0_RX_EQU_ADAPTOR_CNTRL4)        , 0x00, 0x0F },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX0_RX_IDAC_TSETTLE_LOW)          , 0x00, 0xC0 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX0_RX_IDAC_TSETTLE_HIGH)         , 0x00, 0x00 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX0_RX_EQ_OFFSET_ADAPTOR_CNTRL1)  , 0x00, 0x17 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX0_SIGDET_ENABLES)               , 0x00, 0x1C },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX0_SIGDET_CNTRL)                 , 0x00, 0x03 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX0_SIGDET_DEGLITCH_CNTRL)        , 0x00, 0x1E },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX0_RX_MODE_00_LOW)               , 0x00, 0xFF },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX0_RX_MODE_00_HIGH)              , 0x00, 0xFF },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX0_RX_MODE_00_HIGH2)             , 0x00, 0xFF },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX0_RX_MODE_00_HIGH3)             , 0x00, 0x7F },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX0_RX_MODE_00_HIGH4)             , 0x00, 0x66 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX0_RX_MODE_01_LOW)               , 0x00, 0x24 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX0_RX_MODE_01_HIGH)              , 0x00, 0xE4 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX0_RX_MODE_01_HIGH2)             , 0x00, 0xEC },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX0_RX_MODE_01_HIGH3)             , 0x00, 0x3B },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX0_RX_MODE_01_HIGH4)             , 0x00, 0x36 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX0_RX_MODE_10_LOW)               , 0x00, 0xD4 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX0_RX_MODE_10_HIGH)              , 0x00, 0x54 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX0_RX_MODE_10_HIGH2)             , 0x00, 0xDB },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX0_RX_MODE_10_HIGH3)             , 0x00, 0x3B },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX0_RX_MODE_10_HIGH4)             , 0x00, 0x31 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX0_DCC_CTRL1)                    , 0x00, 0x0C },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_TX1_RES_CODE_LANE_OFFSET_TX)      , 0x00, 0x11 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_TX1_LANE_MODE_1)                  , 0x00, 0x35 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_TX1_RCV_DETECT_LVL_2)             , 0x00, 0x12 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX1_UCDR_FO_GAIN)                 , 0x00, 0x0C },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX1_UCDR_SO_GAIN)                 , 0x00, 0x03 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX1_UCDR_SO_SATURATION_AND_ENABLE), 0x00, 0x7F },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX1_UCDR_PI_CONTROLS)             , 0x00, 0x70 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX1_AUX_DATA_TCOARSE_TFINE)       , 0x00, 0x30 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX1_RCLK_AUXDATA_SEL)             , 0x00, 0x00 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX1_VGA_CAL_CNTRL1)               , 0x00, 0x04 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX1_VGA_CAL_CNTRL2)               , 0x00, 0x07 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX1_GM_CAL)                       , 0x00, 0x0D },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX1_RX_EQU_ADAPTOR_CNTRL1)        , 0x00, 0x00 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX1_RX_EQU_ADAPTOR_CNTRL2)        , 0x00, 0x0E },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX1_RX_EQU_ADAPTOR_CNTRL3)        , 0x00, 0x4A },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX1_RX_EQU_ADAPTOR_CNTRL4)        , 0x00, 0x0F },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX1_RX_IDAC_TSETTLE_LOW)          , 0x00, 0xC0 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX1_RX_IDAC_TSETTLE_HIGH)         , 0x00, 0x00 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX1_RX_EQ_OFFSET_ADAPTOR_CNTRL1)  , 0x00, 0x17 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX1_SIGDET_ENABLES)               , 0x00, 0x1C },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX1_SIGDET_CNTRL)                 , 0x00, 0x03 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX1_SIGDET_DEGLITCH_CNTRL)        , 0x00, 0x1E },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX1_RX_MODE_00_LOW)               , 0x00, 0xFF },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX1_RX_MODE_00_HIGH)              , 0x00, 0xFF },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX1_RX_MODE_00_HIGH2)             , 0x00, 0xFF },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX1_RX_MODE_00_HIGH3)             , 0x00, 0x7F },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX1_RX_MODE_00_HIGH4)             , 0x00, 0x66 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX1_RX_MODE_01_LOW)               , 0x00, 0x24 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX1_RX_MODE_01_HIGH)              , 0x00, 0xE4 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX1_RX_MODE_01_HIGH2)             , 0x00, 0xEC },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX1_RX_MODE_01_HIGH3)             , 0x00, 0x3B },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX1_RX_MODE_01_HIGH4)             , 0x00, 0x36 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX1_RX_MODE_10_LOW)               , 0x00, 0xD4 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX1_RX_MODE_10_HIGH)              , 0x00, 0x54 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX1_RX_MODE_10_HIGH2)             , 0x00, 0xDB },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX1_RX_MODE_10_HIGH3)             , 0x00, 0x3B },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX1_RX_MODE_10_HIGH4)             , 0x00, 0x31 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BQSERDES_RX1_DCC_CTRL1)                    , 0x00, 0x0C },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BPCS_COM_REFGEN_REQ_CONFIG1)               , 0x00, 0x05 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BPCS_COM_RX_SIGDET_LVL)                    , 0x00, 0x88 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BPCS_COM_RATE_SLEW_CNTRL1)                 , 0x00, 0x0B },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BPCS_COM_P2U3_WAKEUP_DLY_TIME_AUXCLK_L)    , 0x00, 0x01 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BPCS_COM_EQ_CONFIG5)                       , 0x00, 0x01 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BPCS_PCIE_POWER_STATE_CONFIG2)             , 0x00, 0x0D },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BPCS_PCIE_POWER_STATE_CONFIG4)             , 0x00, 0x07 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BPCS_PCIE_ENDPOINT_REFCLK_DRIVE)           , 0x00, 0xC1 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BPCS_PCIE_L1P1_WAKEUP_DLY_TIME_AUXCLK_L)   , 0x00, 0x01 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BPCS_PCIE_L1P2_WAKEUP_DLY_TIME_AUXCLK_L)   , 0x00, 0x01 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BPCS_PCIE_OSC_DTCT_ACTIONS)                , 0x00, 0x00 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BPCS_PCIE_PRESET_P6_P7_PRE)                , 0x00, 0x33 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BPCS_PCIE_PRESET_P10_PRE)                  , 0x00, 0x00 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BPCS_PCIE_PRESET_P10_POST)                 , 0x00, 0x58 },

  { invalid_reg, PHY_REG_NOP, 0xFF, 0xFF, 0xFF },
};

/*******************************************************************************
 * Chipset specific version independent port 1 PHY power ON sequence
 ******************************************************************************/
STATIC CONST pciePhyRegSeq_t port_1_phy_power_on_seq[] =
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
STATIC CONST pciePhyRegSeq_t port_1_phy_delay_seq[] =
{
  /* Register Base, Operation, Offset, Mask & Value */
  { invalid_reg, PHY_REG_DELAY, 0xFF, 0xFF, 2000 },
  { invalid_reg, PHY_REG_NOP, 0xFF, 0xFF, 0xFF },
};

/*******************************************************************************
 * poll PHY power ON status of port 1 PHY
 ******************************************************************************/
STATIC CONST pciePhyRegSeq_t port_1_phy_poll_seq[] =
{
  /* Register Base, Operation, Offset, Mask & Value */
  { ahb_reg, PHY_REG_REA, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BPCS_COM_PCS_STATUS1), HWIO_PCIE_GEN3X2_CM_PCIE_BPCS_COM_PCS_STATUS1_PHYSTATUS_BMSK, FALSE },
};

/*******************************************************************************
 * post PHY power ON sequence for port 1 GEN3 PHY
 ******************************************************************************/
STATIC CONST pciePhyRegSeq_t port_1_phy_post_power_up_seq[] =
{
  /* Register Base, Operation, Offset, Mask & Value */
  /* Writing Slave address space size */
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_PCIE_PARF_SLV_ADDR_SPACE_SIZE), 0x0, 0x2000000},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_PCIE_PARF_SLV_ADDR_SPACE_SIZE_HI), 0x0, 0x00000000},
  /* Clear REQ_NOT_ENTER_L1 field in PARF PM_CTRL to allow L1 entry */
  { ahb_reg,     PHY_REG_RMW, HWIO_OFFS(PCIE_GEN3X2_PCIE_PARF_PM_CTRL), HWIO_PCIE_GEN3X2_PCIE_PARF_PM_CTRL_REQ_NOT_ENTR_L1_BMSK, 0x00000000},
  /* Disable PCI HALT */
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_PCIE_PARF_AXI_MSTR_WR_ADDR_HALT), 0x0, 0x0},
  { axi_reg,     PHY_REG_RMW, HWIO_OFFS(PCIE_GEN3X2_GEN3_EQ_FB_MODE_DIR_CHANGE_OFF), HWIO_PCIE_GEN3X2_GEN3_EQ_FB_MODE_DIR_CHANGE_OFF_GEN3_EQ_FMDC_N_EVALS_BMSK, (0x0D << HWIO_PCIE_GEN3X2_GEN3_EQ_FB_MODE_DIR_CHANGE_OFF_GEN3_EQ_FMDC_N_EVALS_SHFT)},
  { axi_reg,     PHY_REG_RMW, HWIO_OFFS(PCIE_GEN3X2_GEN3_EQ_FB_MODE_DIR_CHANGE_OFF), HWIO_PCIE_GEN3X2_GEN3_EQ_FB_MODE_DIR_CHANGE_OFF_GEN3_EQ_FMDC_MAX_PRE_CUSROR_DELTA_BMSK, (0x05 << HWIO_PCIE_GEN3X2_GEN3_EQ_FB_MODE_DIR_CHANGE_OFF_GEN3_EQ_FMDC_MAX_PRE_CUSROR_DELTA_SHFT)},
  { axi_reg,     PHY_REG_RMW, HWIO_OFFS(PCIE_GEN3X2_GEN3_EQ_FB_MODE_DIR_CHANGE_OFF), HWIO_PCIE_GEN3X2_GEN3_EQ_FB_MODE_DIR_CHANGE_OFF_GEN3_EQ_FMDC_MAX_POST_CUSROR_DELTA_BMSK, (0x05 << HWIO_PCIE_GEN3X2_GEN3_EQ_FB_MODE_DIR_CHANGE_OFF_GEN3_EQ_FMDC_MAX_POST_CUSROR_DELTA_SHFT)},
  { axi_reg,     PHY_REG_RMW, HWIO_OFFS(PCIE_GEN3X2_GEN3_EQ_FB_MODE_DIR_CHANGE_OFF), HWIO_PCIE_GEN3X2_GEN3_EQ_FB_MODE_DIR_CHANGE_OFF_GEN3_EQ_FMDC_T_MIN_PHASE23_BMSK, 0x00 },
  { axi_reg,     PHY_REG_RMW, HWIO_OFFS(PCIE_GEN3X2_GEN3_EQ_CONTROL_OFF), HWIO_PCIE_GEN3X2_GEN3_EQ_CONTROL_OFF_GEN3_EQ_PHASE23_EXIT_MODE_BMSK, 0x00 },
  { axi_reg,     PHY_REG_RMW, HWIO_OFFS(PCIE_GEN3X2_GEN3_EQ_CONTROL_OFF), HWIO_PCIE_GEN3X2_GEN3_EQ_CONTROL_OFF_GEN3_EQ_PSET_REQ_VEC_BMSK, 0x00 },
  /* Enable DBI_RO_WR_EN to access CS1 region */
  { axi_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_MISC_CONTROL_1_OFF), 0x0, 0x1},
  /* Memory Barrier */
  { invalid_reg, PHY_REG_DSB, 0xFF, 0xFF, 0xFF},
  { axi_reg,     PHY_REG_RMW, HWIO_OFFS(PCIE_GEN3X2_DEVICE_CONTROL_DEVICE_STATUS), HWIO_PCIE_GEN3X2_DEVICE_CONTROL_DEVICE_STATUS_PCIE_CAP_EN_NO_SNOOP_BMSK, 0x0},
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
  { axi_reg,     PHY_REG_RMW, HWIO_OFFS(PCIE_GEN3X2_GEN3_RELATED_OFF), HWIO_PCIE_GEN3X2_GEN3_RELATED_OFF_GEN3_ZRXDC_NONCOMPL_BMSK, 0x00 },
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
STATIC CONST pciePhyAllVerSeq_t sdm1000_port_1_phy_init_seq[] =
{
  /* First write the register sequence corresponds to version 1 */
  {Pcie_Qcom_SoC_Ver_1,    v1_port_1_phy_init_seq},
  /* Write the register sequence corresponds to version 2 */
  {Pcie_Qcom_SoC_Ver_2,    v2_port_1_phy_init_seq},
  /* Once all the version dependent sequences are written, power ON PHY */
  {Pcie_Qcom_SoC_Ver_ALL,  port_1_phy_power_on_seq},
  /* Once the PHY is brought of reset, give a finite delay */
  {Pcie_Qcom_SoC_Ver_ALL,  port_1_phy_delay_seq},
  /* Check if the PHY PLL is locked */
  {Pcie_Qcom_SoC_Ver_POLL, port_1_phy_poll_seq},
  /* Perform post power up sequence */
  {Pcie_Qcom_SoC_Ver_ALL,  port_1_phy_post_power_up_seq},
  /* End of PHY init sequence */
  {Pcie_Qcom_SoC_Ver_STOP, NULL},
};

/*******************************************************************************
 * Chipset specific port 1 PHY De-init sequence
 ******************************************************************************/
STATIC CONST pciePhyRegSeq_t port_1_phy_de_init_seq[] =
{
  /* Register Base, Operation, Offset, Mask & Value */
  { ahb_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X2_CM_PCIE_BPCS_COM_POWER_DOWN_CONTROL               ), 0x0, 0x00},
  { invalid_reg, PHY_REG_NOP, 0xFF, 0xFF, 0xFF },
};
#endif /* (RPMASK & PCIE_RP(1)) - root port 1 enabled */

/*******************************************************************************
 * This section will hold the port info data for root port 2
 ******************************************************************************/
#if (RPMASK & PCIE_RP(2))

/* Number of PCIe lanes present in RP2 */
#define NUM_PCIE_2_PORTS 0x01

/*******************************************************************************
 * Register region information for PCIe GEN3 X4 root port
 ******************************************************************************/
static memRegions_t port_2_reg[max_mem_reg] =
{
  /**< axi_reg: DBI register space */
  {.pa=PCIE_GEN3X4_PCIE_WRAPPER_AXI_NO_EDMA_BASE, .sz=PCIE_GEN3X4_PCIE_WRAPPER_AXI_NO_EDMA_BASE_SIZE},
  /**< ahb_reg: PARF register space */
  {.pa=PCIE_GEN3X4_PCIE_WRAPPER_AHB_W_PHY_G3X4_BASE, .sz=PCIE_GEN3X4_PCIE_WRAPPER_AHB_W_PHY_G3X4_BASE_SIZE},
  /**< tcsr_reg: Access TCSR register space */
  {.pa=CORE_TOP_CSR_BASE, .sz=CORE_TOP_CSR_BASE_SIZE},
  /**< invalid_reg: Invalid reg base to end the PHY init sequence */
  {.pa=PCIE_INVALID_REG_BASE, .sz=PCIE_INVALID_REG_SIZE}
};

/*******************************************************************************
 * PCIe port reset clock name list for root port 2 with NULL ending
 ******************************************************************************/
STATIC CONST char * pcie_reset_clks_rp2[] =
{
  "gcc_pcie_2_aux_clk",
  NULL
};

/*******************************************************************************
 * PCIe port external clock name list for root port 2 with NULL ending
 ******************************************************************************/
STATIC CONST char * pcie_external_clks_rp2[] =
{
  "gcc_pcie_2_pipe_clk",
  NULL
};

/*******************************************************************************
 * PCIe port gcc clock name list for root port 2 with NULL ending
 ******************************************************************************/
STATIC CONST char * pcie_gcc_clks_rp2[] =
{
  "gcc_pcie_2_aux_clk",
  "gcc_pcie_2_slv_axi_clk",
  "gcc_pcie_2_slv_q2a_axi_clk",
  "gcc_pcie_2_mstr_axi_clk",
  "gcc_pcie_2_cfg_ahb_clk",
  "gcc_pcie2_phy_refgen_clk",
  NULL
};

/*******************************************************************************
 * PCIe port aggregated gcc clock name list for root port 2
 ******************************************************************************/
STATIC CONST pcieClkDesc_t pcie_clk_rp2[] =
{
  {
    .reset = pcie_reset_clks_rp2,
    .external = pcie_external_clks_rp2,
    .gcc = pcie_gcc_clks_rp2
  }
};

/*******************************************************************************
 * ICB bandwidth request for pcie root port 2
 ******************************************************************************/
STATIC CONST ICBArb_RequestType pcie_2_icb_request[] =
{
  {
    .arbType = ICBARB_REQUEST_TYPE_3,
    .arbData.type3 =
    {
      .uIb = ((400ULL) << 20), /**< Instantaneous bandwidth in Bytes/second */
      .uAb = ((400ULL) << 20), /**< Arbitrated bandwidth in Bytes/second */
      .uLatencyNs = 0          /**< Latency requirement in nanoseconds */
    }
  }
};

/*******************************************************************************
 * ICB Master and Slave list for pcie root port 2
 ******************************************************************************/
STATIC CONST ICBArb_MasterSlaveType pcie_2_icb_mstr_slv_lst[] =
{
  {
    .eMaster = ICBID_MASTER_PCIE_2,
    .eSlave = ICBID_SLAVE_EBI1
  }
};

/*******************************************************************************
 * Aggreagate bus bandwidth vote for pcie root port 2
 ******************************************************************************/
STATIC pcieIcbClkBW_t pcie_2_icb_bw[] =
{
  {
    .npa_client_name = "pcie_2",
    .npa_hdl = NULL,
    .ms_list = pcie_2_icb_mstr_slv_lst,
    .ms_size = sizeof(pcie_2_icb_mstr_slv_lst),
    .req_list = pcie_2_icb_request,
    .req_size = sizeof(pcie_2_icb_request)
  }
};

/*******************************************************************************
 * PCIe root port 2 clock settings
 ******************************************************************************/
CONST clkFreqSettings_t pcie_2_clk_settings[] =
{
  {.freq = 19200000,  .name = "gcc_pcie_2_aux_clk"},
  {.freq = 100000000, .name = "gcc_pcie2_phy_refgen_clk"},
  {.freq = 0, .name = NULL}
};

/*******************************************************************************
 * GPIO configuration for RP2
 *
 * This array will be access from index 0 upto the end of
 * array to to configure all the GPIOs before performing
 * link training
 ******************************************************************************/
/*
 * ENDP GPIO configuration for RP2
 */
STATIC CONST gpioPinConfig_t gpio_config_rp2[] =
{
  {
   .type = MSM_GPIO,
   {
    .TlmmGpioParam.GpioNum = 176, // CLK_REQ
    .TlmmGpioParam.Func = 1,
    .TlmmGpioParam.Direction = GPIO_INPUT,
    .TlmmGpioParam.Pull = GPIO_PULL_UP,
    .TlmmGpioParam.DriveStrength = GPIO_2MA,
    .TlmmGpioParam.value = GPIO_LOW_VALUE
   }
  }
  ,
  {
   .type = MSM_GPIO,
   {
    .TlmmGpioParam.GpioNum = 102, // PCIE_RST
    .TlmmGpioParam.Func = 0,
    .TlmmGpioParam.Direction = GPIO_OUTPUT,
    .TlmmGpioParam.Pull = GPIO_NO_PULL,
    .TlmmGpioParam.DriveStrength = GPIO_2MA,
    .TlmmGpioParam.value = GPIO_LOW_VALUE
   }
  }
  ,
  {
   .type = MSM_GPIO,
   {
    .TlmmGpioParam.GpioNum = 131, // WIGG_EN
    .TlmmGpioParam.Func = 0,
    .TlmmGpioParam.Direction = GPIO_OUTPUT,
    .TlmmGpioParam.Pull = GPIO_NO_PULL,
    .TlmmGpioParam.DriveStrength = GPIO_2MA,
    .TlmmGpioParam.value = GPIO_LOW_VALUE
   }
  }
#if 0
  ,
  {
   .type = MSM_GPIO,
   {
    .TlmmGpioParam.GpioNum = 131, // WIGG_EN
    .TlmmGpioParam.Func = 0,
    .TlmmGpioParam.Direction = GPIO_OUTPUT,
    .TlmmGpioParam.Pull = GPIO_NO_PULL,
    .TlmmGpioParam.DriveStrength = GPIO_2MA,
    .TlmmGpioParam.value = GPIO_HIGH_VALUE
   }
  }
#endif
  ,
  {
   .type = MSM_GPIO,
   {
    .TlmmGpioParam.GpioNum = 104, // PCIE_WAKE
    .TlmmGpioParam.Func = 0,
    .TlmmGpioParam.Direction = GPIO_INPUT,
    .TlmmGpioParam.Pull = GPIO_NO_PULL,
    .TlmmGpioParam.DriveStrength = GPIO_2MA,
    .TlmmGpioParam.value = GPIO_LOW_VALUE
   }
  }
  ,
  {/* TODO: */
   .type = DELAY,
   {
    .DelayValue = 150 // 150 micro seconds before REFCLK gets stable
   }
  }
  ,
  {
   .type = MSM_GPIO,
   {
    .TlmmGpioParam.GpioNum = 102, // PCIE_RST
    .TlmmGpioParam.Func = 0,
    .TlmmGpioParam.Direction = GPIO_OUTPUT,
    .TlmmGpioParam.Pull = GPIO_NO_PULL,
    .TlmmGpioParam.DriveStrength = GPIO_2MA,
    .TlmmGpioParam.value = GPIO_HIGH_VALUE
   }
  }
  ,
  {
   .type = DELAY,
   {
    .DelayValue = 10000 // 10 ms delay for EP device preparation for Link Training
   }
  }
  ,
  /* Default and end of sequence entry */
  {
   .type = NONE,
   {
    .DelayValue = NONE
   }
  }
};

/*******************************************************************************
 * Link info
 ******************************************************************************/
STATIC pcieLink_t port2_link[] =
{
  {
    .trained             = FALSE,
    .num_lanes           = 2,
    .aspm                = aspm_l1_ena_l0s_ena,
    .ltr_threshold_val   = 0,
    .ltr_threshold_scale = LTR_TH_SCALE_1ns
  }
};

/*******************************************************************************
 * V1 Chipset specific port 2 PHY init sequence
 ******************************************************************************/
STATIC CONST pciePhyRegSeq_t v1_port_2_phy_init_seq[] =
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
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_TX0_LANE_MODE_1)                  , 0x00, 0x05 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_TX0_RCV_DETECT_LVL_2)             , 0x00, 0x12 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX0_UCDR_FO_GAIN)                 , 0x00, 0x0C },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX0_UCDR_SO_GAIN)                 , 0x00, 0x03 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX0_UCDR_SO_SATURATION_AND_ENABLE), 0x00, 0x7F },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX0_UCDR_PI_CONTROLS)             , 0x00, 0x70 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX0_AUX_DATA_TCOARSE_TFINE)       , 0x00, 0xA0 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX0_RCLK_AUXDATA_SEL)             , 0x00, 0x00 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX0_VGA_CAL_CNTRL1)               , 0x00, 0x54 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX0_VGA_CAL_CNTRL2)               , 0x00, 0x37 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX0_GM_CAL)                       , 0x00, 0x05 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX0_RX_EQU_ADAPTOR_CNTRL1)        , 0x00, 0x07 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX0_RX_EQU_ADAPTOR_CNTRL2)        , 0x00, 0x6E },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX0_RX_EQU_ADAPTOR_CNTRL3)        , 0x00, 0x6E },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX0_RX_EQU_ADAPTOR_CNTRL4)        , 0x00, 0x4A },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX0_RX_IDAC_TSETTLE_LOW)          , 0x00, 0xC0 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX0_RX_IDAC_TSETTLE_HIGH)         , 0x00, 0x00 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX0_RX_EQ_OFFSET_ADAPTOR_CNTRL1)  , 0x00, 0x17 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX0_SIGDET_ENABLES)               , 0x00, 0x1C },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX0_SIGDET_CNTRL)                 , 0x00, 0x03 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX0_SIGDET_DEGLITCH_CNTRL)        , 0x00, 0x14 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX0_RX_MODE_00_LOW)               , 0x00, 0x7F },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX0_RX_MODE_00_HIGH)              , 0x00, 0xFF },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX0_RX_MODE_00_HIGH2)             , 0x00, 0xFF },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX0_RX_MODE_00_HIGH3)             , 0x00, 0xDB },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX0_RX_MODE_00_HIGH4)             , 0x00, 0x75 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX0_RX_MODE_01_LOW)               , 0x00, 0x24 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX0_RX_MODE_01_HIGH)              , 0x00, 0xE4 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX0_RX_MODE_01_HIGH2)             , 0x00, 0xEC },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX0_RX_MODE_01_HIGH3)             , 0x00, 0x39 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX0_RX_MODE_01_HIGH4)             , 0x00, 0x36 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX0_RX_MODE_10_LOW)               , 0x00, 0xD4 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX0_RX_MODE_10_HIGH)              , 0x00, 0x54 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX0_RX_MODE_10_HIGH2)             , 0x00, 0xDB },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX0_RX_MODE_10_HIGH3)             , 0x00, 0x39 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX0_RX_MODE_10_HIGH4)             , 0x00, 0x31 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX0_DCC_CTRL1)                    , 0x00, 0x0C },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_TX1_LANE_MODE_1)                  , 0x00, 0x05 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_TX1_RCV_DETECT_LVL_2)             , 0x00, 0x12 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX1_UCDR_FO_GAIN)                 , 0x00, 0x0C },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX1_UCDR_SO_GAIN)                 , 0x00, 0x03 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX1_UCDR_SO_SATURATION_AND_ENABLE), 0x00, 0x7F },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX1_UCDR_PI_CONTROLS)             , 0x00, 0x70 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX1_AUX_DATA_TCOARSE_TFINE)       , 0x00, 0xA0 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX1_RCLK_AUXDATA_SEL)             , 0x00, 0x00 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX1_VGA_CAL_CNTRL1)               , 0x00, 0x54 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX1_VGA_CAL_CNTRL2)               , 0x00, 0x37 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX1_GM_CAL)                       , 0x00, 0x05 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX1_RX_EQU_ADAPTOR_CNTRL1)        , 0x00, 0x07 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX1_RX_EQU_ADAPTOR_CNTRL2)        , 0x00, 0x6E },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX1_RX_EQU_ADAPTOR_CNTRL3)        , 0x00, 0x6E },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX1_RX_EQU_ADAPTOR_CNTRL4)        , 0x00, 0x4A },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX1_RX_IDAC_TSETTLE_LOW)          , 0x00, 0xC0 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX1_RX_IDAC_TSETTLE_HIGH)         , 0x00, 0x00 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX1_RX_EQ_OFFSET_ADAPTOR_CNTRL1)  , 0x00, 0x17 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX1_SIGDET_ENABLES)               , 0x00, 0x1C },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX1_SIGDET_CNTRL)                 , 0x00, 0x03 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX1_SIGDET_DEGLITCH_CNTRL)        , 0x00, 0x14 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX1_RX_MODE_00_LOW)               , 0x00, 0x7F },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX1_RX_MODE_00_HIGH)              , 0x00, 0xFF },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX1_RX_MODE_00_HIGH2)             , 0x00, 0xFF },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX1_RX_MODE_00_HIGH3)             , 0x00, 0xDB },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX1_RX_MODE_00_HIGH4)             , 0x00, 0x75 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX1_RX_MODE_01_LOW)               , 0x00, 0x24 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX1_RX_MODE_01_HIGH)              , 0x00, 0xE4 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX1_RX_MODE_01_HIGH2)             , 0x00, 0xEC },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX1_RX_MODE_01_HIGH3)             , 0x00, 0x39 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX1_RX_MODE_01_HIGH4)             , 0x00, 0x36 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX1_RX_MODE_10_LOW)               , 0x00, 0xD4 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX1_RX_MODE_10_HIGH)              , 0x00, 0x54 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX1_RX_MODE_10_HIGH2)             , 0x00, 0xDB },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX1_RX_MODE_10_HIGH3)             , 0x00, 0x39 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX1_RX_MODE_10_HIGH4)             , 0x00, 0x31 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_RX1_DCC_CTRL1)                    , 0x00, 0x0C },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_APCS_COM_REFGEN_REQ_CONFIG1)               , 0x00, 0x05 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_APCS_COM_RX_SIGDET_LVL)                    , 0x00, 0xAA },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_APCS_COM_RATE_SLEW_CNTRL1)                 , 0x00, 0x0B },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_APCS_COM_P2U3_WAKEUP_DLY_TIME_AUXCLK_L)    , 0x00, 0x01 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_APCS_COM_EQ_CONFIG5)                       , 0x00, 0x01 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_APCS_PCIE_POWER_STATE_CONFIG2)             , 0x00, 0x0D },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_APCS_PCIE_POWER_STATE_CONFIG4)             , 0x00, 0x07 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_APCS_PCIE_ENDPOINT_REFCLK_DRIVE)           , 0x00, 0xC1 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_APCS_PCIE_ENDPOINT_REFCLK_CNTRL)           , 0x00, 0x01 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_APCS_PCIE_L1P1_WAKEUP_DLY_TIME_AUXCLK_L)   , 0x00, 0x01 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_APCS_PCIE_L1P2_WAKEUP_DLY_TIME_AUXCLK_L)   , 0x00, 0x01 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_APCS_PCIE_OSC_DTCT_ACTIONS)                , 0x00, 0x00 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_APCS_PCIE_PRESET_P10_PRE)                  , 0x00, 0x00 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_APCS_PCIE_PRESET_P10_POST)                 , 0x00, 0x58 },

  { invalid_reg, PHY_REG_NOP, 0xFF, 0xFF, 0xFF },
};

/*******************************************************************************
 * V2 Chipset specific port 2 PHY init sequence
 ******************************************************************************/
STATIC CONST pciePhyRegSeq_t v2_port_2_phy_init_seq[] =
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
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_AQSERDES_PLL_BIAS_EN_CLKBUFLR_EN)          , 0x00, 0x14 },
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

  { invalid_reg, PHY_REG_NOP, 0xFF, 0xFF, 0xFF },
};

/*******************************************************************************
 * Chipset specific version independent port 2 PHY power ON sequence
 ******************************************************************************/
STATIC CONST pciePhyRegSeq_t port_2_phy_power_on_seq[] =
{
  /* Register Base, Operation, Offset, Mask & Value */
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_APCS_COM_SW_RESET                         ), 0x00, 0x00 }, // Release software reset
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_APCS_COM_START_CONTROL                    ), 0x00, 0x03 }, // Start PCS and Serdes power SMs
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
STATIC CONST pciePhyRegSeq_t port_2_phy_delay_seq[] =
{
  /* Register Base, Operation, Offset, Mask & Value */
  { invalid_reg, PHY_REG_DELAY, 0xFF, 0xFF, 2000 },
  { invalid_reg, PHY_REG_NOP, 0xFF, 0xFF, 0xFF },
};

/*******************************************************************************
 * poll PHY power ON status of port 2 PHYA
 ******************************************************************************/
STATIC CONST pciePhyRegSeq_t port_2_phy_A_poll_seq[] =
{
  /* Register Base, Operation, Offset, Mask & Value */
  { ahb_reg,     PHY_REG_REA, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_APCS_COM_PCS_STATUS1), HWIO_PCIE_GEN3X4_CM_PCIE_APCS_COM_PCS_STATUS1_PHYSTATUS_BMSK, FALSE },
  { invalid_reg, PHY_REG_NOP, 0xFF, 0xFF, 0xFF },
};

/*******************************************************************************
 * post PHY power ON sequence for port 2 GEN3 PHY
 ******************************************************************************/
STATIC CONST pciePhyRegSeq_t port_2_phy_post_power_up_seq[] =
{
  /* Register Base, Operation, Offset, Mask & Value */
  /* Enable BME & MSE before LTSSM_EN */
  { axi_reg,     PHY_REG_RMW, HWIO_OFFS(PCIE_GEN3X4_TYPE1_STATUS_COMMAND_REG), HWIO_PCIE_GEN3X4_TYPE1_STATUS_COMMAND_REG_MSE_BMSK, (0x1 << HWIO_PCIE_GEN3X4_TYPE1_STATUS_COMMAND_REG_MSE_SHFT)},
  { axi_reg,     PHY_REG_RMW, HWIO_OFFS(PCIE_GEN3X4_TYPE1_STATUS_COMMAND_REG), HWIO_PCIE_GEN3X4_TYPE1_STATUS_COMMAND_REG_BME_BMSK, (0x1 << HWIO_PCIE_GEN3X4_TYPE1_STATUS_COMMAND_REG_BME_SHFT)},
  /* Writing Slave address space size */
  { ahb_reg,    PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_PCIE_PARF_SLV_ADDR_SPACE_SIZE), 0x0, 0x2000000},
  { ahb_reg,    PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_PCIE_PARF_SLV_ADDR_SPACE_SIZE_HI), 0x0, 0x00000000},
  /* Disable PCI halt */
  { ahb_reg,    PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_PCIE_PARF_AXI_MSTR_WR_ADDR_HALT), 0x0, 0x0},
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
  { axi_reg,     PHY_REG_RMW, HWIO_OFFS(PCIE_GEN3X4_DEVICE_CONTROL_DEVICE_STATUS), HWIO_PCIE_GEN3X4_DEVICE_CONTROL_DEVICE_STATUS_PCIE_CAP_EN_NO_SNOOP_BMSK, 0x0},
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
STATIC CONST pciePhyAllVerSeq_t sdm1000_port_2_phy_init_seq[] =
{
  /* First write the register sequence corresponds to version 1 */
  {Pcie_Qcom_SoC_Ver_1,    v1_port_2_phy_init_seq},
  /* Write the register sequence corresponds to version 2 */
  {Pcie_Qcom_SoC_Ver_2,    v2_port_2_phy_init_seq},
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

/*******************************************************************************
 * Chipset specific port 2 PHY De-init sequence
 ******************************************************************************/
STATIC CONST pciePhyRegSeq_t port_2_phy_de_init_seq[] =
{
  /* Register Base, Operation, Offset, Mask & Value */
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_CM_PCIE_APCS_COM_POWER_DOWN_CONTROL                    ), 0x00, 0x00},
  { invalid_reg, PHY_REG_NOP, 0xFF, 0xFF, 0xFF },
};
#endif /* (RPMASK & PCIE_RP(2)) - root port 2 enabled */

/*******************************************************************************
 * This section will hold the port info data for root port 1
 ******************************************************************************/
#if (RPMASK & PCIE_RP(3))

/* Number of PCIe lanes present in RP3 */
#define NUM_PCIE_3_PORTS 0x01

/*******************************************************************************
 * Register region information for PCIe GEN3 EDMA X4 root port
 ******************************************************************************/
static memRegions_t port_3_reg[max_mem_reg] =
{
  /**< axi_reg: DBI register space */
  {.pa=PCIE_GEN3X4_EDMA_PCIE_WRAPPER_AXI_WITH_EDMA_BASE, .sz=PCIE_GEN3X4_EDMA_PCIE_WRAPPER_AXI_WITH_EDMA_BASE_SIZE},
  /**< ahb_reg: PARF register space */
  {.pa=PCIE_GEN3X4_EDMA_PCIE_WRAPPER_AHB_W_PHY_G3X4_BASE, .sz=PCIE_GEN3X4_EDMA_PCIE_WRAPPER_AHB_W_PHY_G3X4_BASE_SIZE},
  /**< tcsr_reg: Access TCSR register space */
  {.pa=CORE_TOP_CSR_BASE, .sz=CORE_TOP_CSR_BASE_SIZE},
  /**< invalid_reg: Invalid reg base to end the PHY init sequence */
  {.pa=PCIE_INVALID_REG_BASE, .sz=PCIE_INVALID_REG_SIZE}
};

/*******************************************************************************
 * PCIe port reset clock name list for root port 3 with NULL ending
 ******************************************************************************/
STATIC CONST char * pcie_reset_clks_rp3[] =
{
  "gcc_pcie_3_aux_clk",
  NULL
};

/*******************************************************************************
 * PCIe port external clock name list for root port 3 with NULL ending
 ******************************************************************************/
STATIC CONST char * pcie_external_clks_rp3[] =
{
  "gcc_pcie_3_pipe_clk",
  NULL
};

/*******************************************************************************
 * PCIe port gcc clock name list for root port 3 with NULL ending
 ******************************************************************************/
STATIC CONST char * pcie_gcc_clks_rp3[] =
{
  "gcc_pcie_3_aux_clk",
  "gcc_pcie_3_slv_axi_clk",
  "gcc_pcie_3_slv_q2a_axi_clk",
  "gcc_pcie_3_mstr_axi_clk",
  "gcc_pcie_3_cfg_ahb_clk",
  "gcc_pcie3_phy_refgen_clk",
  NULL
};

/*******************************************************************************
 * PCIe port aggregated gcc clock name list for root port 3
 ******************************************************************************/
STATIC CONST pcieClkDesc_t pcie_clk_rp3[] =
{
  {
    .reset = pcie_reset_clks_rp3,
    .external = pcie_external_clks_rp3,
    .gcc = pcie_gcc_clks_rp3
  }
};

/*******************************************************************************
 * ICB bandwidth request for pcie root port 3
 ******************************************************************************/
STATIC CONST ICBArb_RequestType pcie_3_icb_request[] =
{
  {
    .arbType = ICBARB_REQUEST_TYPE_3,
    .arbData.type3 =
    {
      .uIb = ((100ULL) << 20), /**< Instantaneous bandwidth in Bytes/second */
      .uAb = ((100ULL) << 20), /**< Arbitrated bandwidth in Bytes/second */
      .uLatencyNs = 0          /**< Latency requirement in nanoseconds */
    }
  }
};

/*******************************************************************************
 * ICB Master and Slave list for pcie root port 3
 ******************************************************************************/
STATIC CONST ICBArb_MasterSlaveType pcie_3_icb_mstr_slv_lst[] =
{
  {
    .eMaster = ICBID_MASTER_PCIE_3,
    .eSlave = ICBID_SLAVE_EBI1
  }
};

/*******************************************************************************
 * Aggreagate bus bandwidth vote for pcie root port 3
 ******************************************************************************/
STATIC pcieIcbClkBW_t pcie_3_icb_bw[] =
{
  {
    .npa_client_name = "pcie_3",
    .npa_hdl = NULL,
    .ms_list = pcie_3_icb_mstr_slv_lst,
    .ms_size = sizeof(pcie_3_icb_mstr_slv_lst),
    .req_list = pcie_3_icb_request,
    .req_size = sizeof(pcie_3_icb_request)
  }
};

/*******************************************************************************
 * PCIe root port 3 clock settings
 ******************************************************************************/
CONST clkFreqSettings_t pcie_3_clk_settings[] =
{
  {.freq = 19200000,  .name = "gcc_pcie_3_aux_clk"},
  {.freq = 100000000, .name = "gcc_pcie3_phy_refgen_clk"},
  {.freq = 0, .name = NULL}
};

/*******************************************************************************
 * GPIO configuration for RP3
 *
 * This array will be access from index 0 upto the end of
 * array to to configure all the GPIOs before performing
 * link training
 ******************************************************************************/
/*
 * ENDP GPIO configuration for RP1
 */
STATIC CONST gpioPinConfig_t gpio_config_rp3[] =
{
  {
   .type = MSM_GPIO,
   {
    .TlmmGpioParam.GpioNum = 179, // CLK_REQ
    .TlmmGpioParam.Func = 1,
    .TlmmGpioParam.Direction = GPIO_INPUT,
    .TlmmGpioParam.Pull = GPIO_PULL_UP,
    .TlmmGpioParam.DriveStrength = GPIO_2MA,
    .TlmmGpioParam.value = GPIO_LOW_VALUE
   }
  }
  ,
  {
   .type = MSM_GPIO,
   {
    .TlmmGpioParam.GpioNum = 178, // PCIE_RST
    .TlmmGpioParam.Func = 0,
    .TlmmGpioParam.Direction = GPIO_OUTPUT,
    .TlmmGpioParam.Pull = GPIO_NO_PULL,
    .TlmmGpioParam.DriveStrength = GPIO_2MA,
    .TlmmGpioParam.value = GPIO_LOW_VALUE
   }
  }
  ,
  {
   .type = MSM_GPIO,
   {
    .TlmmGpioParam.GpioNum = 180, // PCIE_WAKE
    .TlmmGpioParam.Func = 0,
    .TlmmGpioParam.Direction = GPIO_INPUT,
    .TlmmGpioParam.Pull = GPIO_NO_PULL,
    .TlmmGpioParam.DriveStrength = GPIO_2MA,
    .TlmmGpioParam.value = GPIO_LOW_VALUE
   }
  }
  ,
  {
   .type = DELAY,
   {
    .DelayValue = 150 // 150 micro seconds before REFCLK get stabl
   }
  }
  ,
  {
   .type = MSM_GPIO,
   {
    .TlmmGpioParam.GpioNum = 178, // PCIE_RST
    .TlmmGpioParam.Func = 0,
    .TlmmGpioParam.Direction = GPIO_OUTPUT,
    .TlmmGpioParam.Pull = GPIO_NO_PULL,
    .TlmmGpioParam.DriveStrength = GPIO_2MA,
    .TlmmGpioParam.value = GPIO_HIGH_VALUE
   }
  }
  ,
  {
   .type = DELAY,
   {
    .DelayValue = 20000 // 20 ms delay for EP device preparation for Link Training
   }
  }
  ,
  /* Default and end of sequence entry */
  {
   .type = NONE,
   {
    .DelayValue = NONE
   }
  }
};

/*******************************************************************************
 * Link info
 ******************************************************************************/
STATIC pcieLink_t port3_link[] =
{
  {
    .trained             = FALSE,
    .num_lanes           = 4,
    .aspm                = aspm_l1_ena_l0s_ena,
    .ltr_threshold_val   = 0,
    .ltr_threshold_scale = LTR_TH_SCALE_1ns
  }
};

/*******************************************************************************
 * V1 Chipset specific port 3 PHY init sequence
 ******************************************************************************/
STATIC CONST pciePhyRegSeq_t v1_port_3_phy_init_seq[] =
{
  /* Register Base, Operation, Offset, Mask & Value */
  /* Register Base, Operation, Offset, Mask & Value */
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_PCIE_PARF_DEVICE_TYPE)                             , 0x00, 0x04 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_APCS_COM_POWER_DOWN_CONTROL)               , 0x00, 0x03 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_PLL_SSC_EN_CENTER)                , 0x00, 0x00 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_PLL_SSC_PER1)                     , 0x00, 0x31 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_PLL_SSC_PER2)                     , 0x00, 0x01 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_PLL_SSC_STEP_SIZE1_MODE0)         , 0x00, 0xDE },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_PLL_SSC_STEP_SIZE2_MODE0)         , 0x00, 0x07 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_PLL_SSC_STEP_SIZE1_MODE1)         , 0x00, 0x4C },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_PLL_SSC_STEP_SIZE2_MODE1)         , 0x00, 0x06 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_PLL_BIAS_EN_CLKBUFLR_EN)          , 0x00, 0x1C },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_PLL_CLK_ENABLE1)                  , 0x00, 0x90 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_PLL_PLL_IVCO)                     , 0x00, 0x0F },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_PLL_CP_CTRL_MODE0)                , 0x00, 0x06 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_PLL_CP_CTRL_MODE1)                , 0x00, 0x06 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_PLL_PLL_RCTRL_MODE0)              , 0x00, 0x16 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_PLL_PLL_RCTRL_MODE1)              , 0x00, 0x16 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_PLL_PLL_CCTRL_MODE0)              , 0x00, 0x36 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_PLL_PLL_CCTRL_MODE1)              , 0x00, 0x36 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_PLL_SYSCLK_EN_SEL)                , 0x00, 0x08 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_PLL_LOCK_CMP_EN)                  , 0x00, 0x42 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_PLL_LOCK_CMP1_MODE0)              , 0x00, 0x0A },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_PLL_LOCK_CMP2_MODE0)              , 0x00, 0x1A },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_PLL_LOCK_CMP1_MODE1)              , 0x00, 0x14 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_PLL_LOCK_CMP2_MODE1)              , 0x00, 0x34 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_PLL_DEC_START_MODE0)              , 0x00, 0x82 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_PLL_DEC_START_MODE1)              , 0x00, 0x68 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_PLL_DIV_FRAC_START1_MODE0)        , 0x00, 0x55 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_PLL_DIV_FRAC_START2_MODE0)        , 0x00, 0x55 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_PLL_DIV_FRAC_START3_MODE0)        , 0x00, 0x03 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_PLL_DIV_FRAC_START1_MODE1)        , 0x00, 0xAB },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_PLL_DIV_FRAC_START2_MODE1)        , 0x00, 0xAA },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_PLL_DIV_FRAC_START3_MODE1)        , 0x00, 0x02 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_PLL_VCO_TUNE_MAP)                 , 0x00, 0x02 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_PLL_VCO_TUNE1_MODE0)              , 0x00, 0x24 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_PLL_VCO_TUNE1_MODE1)              , 0x00, 0xB4 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_PLL_VCO_TUNE2_MODE1)              , 0x00, 0x03 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_PLL_CLK_SELECT)                   , 0x00, 0x34 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_PLL_HSCLK_SEL)                    , 0x00, 0x01 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_PLL_CORECLK_DIV_MODE1)            , 0x00, 0x08 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_PLL_BIN_VCOCAL_CMP_CODE1_MODE0)   , 0x00, 0xB9 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_PLL_BIN_VCOCAL_CMP_CODE2_MODE0)   , 0x00, 0x1E },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_PLL_BIN_VCOCAL_CMP_CODE1_MODE1)   , 0x00, 0x94 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_PLL_BIN_VCOCAL_CMP_CODE2_MODE1)   , 0x00, 0x18 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_PLL_BIN_VCOCAL_HSCLK_SEL)         , 0x00, 0x11 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_TX0_LANE_MODE_1)                  , 0x00, 0x05 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_TX0_RCV_DETECT_LVL_2)             , 0x00, 0x12 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX0_UCDR_FO_GAIN)                 , 0x00, 0x0C },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX0_UCDR_SO_GAIN)                 , 0x00, 0x03 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX0_UCDR_SO_SATURATION_AND_ENABLE), 0x00, 0x7F },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX0_UCDR_PI_CONTROLS)             , 0x00, 0x70 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX0_AUX_DATA_TCOARSE_TFINE)       , 0x00, 0xA0 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX0_RCLK_AUXDATA_SEL)             , 0x00, 0x00 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX0_VGA_CAL_CNTRL1)               , 0x00, 0x04 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX0_VGA_CAL_CNTRL2)               , 0x00, 0x37 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX0_GM_CAL)                       , 0x00, 0x05 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX0_RX_EQU_ADAPTOR_CNTRL1)        , 0x00, 0x07 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX0_RX_EQU_ADAPTOR_CNTRL2)        , 0x00, 0x6E },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX0_RX_EQU_ADAPTOR_CNTRL3)        , 0x00, 0x6E },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX0_RX_EQU_ADAPTOR_CNTRL4)        , 0x00, 0x4A },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX0_RX_IDAC_TSETTLE_LOW)          , 0x00, 0xC0 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX0_RX_IDAC_TSETTLE_HIGH)         , 0x00, 0x00 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX0_RX_EQ_OFFSET_ADAPTOR_CNTRL1)  , 0x00, 0x17 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX0_SIGDET_ENABLES)               , 0x00, 0x1C },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX0_SIGDET_CNTRL)                 , 0x00, 0x03 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX0_SIGDET_DEGLITCH_CNTRL)        , 0x00, 0x14 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX0_RX_MODE_00_LOW)               , 0x00, 0x7F },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX0_RX_MODE_00_HIGH)              , 0x00, 0xFF },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX0_RX_MODE_00_HIGH2)             , 0x00, 0xFF },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX0_RX_MODE_00_HIGH3)             , 0x00, 0xDB },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX0_RX_MODE_00_HIGH4)             , 0x00, 0x75 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX0_RX_MODE_01_LOW)               , 0x00, 0x24 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX0_RX_MODE_01_HIGH)              , 0x00, 0xE4 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX0_RX_MODE_01_HIGH2)             , 0x00, 0xEC },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX0_RX_MODE_01_HIGH3)             , 0x00, 0x39 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX0_RX_MODE_01_HIGH4)             , 0x00, 0x36 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX0_RX_MODE_10_LOW)               , 0x00, 0xD4 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX0_RX_MODE_10_HIGH)              , 0x00, 0x54 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX0_RX_MODE_10_HIGH2)             , 0x00, 0xDB },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX0_RX_MODE_10_HIGH3)             , 0x00, 0x39 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX0_RX_MODE_10_HIGH4)             , 0x00, 0x31 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX0_DCC_CTRL1)                    , 0x00, 0x0C },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_TX1_LANE_MODE_1)                  , 0x00, 0x05 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_TX1_RCV_DETECT_LVL_2)             , 0x00, 0x12 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX1_UCDR_FO_GAIN)                 , 0x00, 0x0C },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX1_UCDR_SO_GAIN)                 , 0x00, 0x03 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX1_UCDR_SO_SATURATION_AND_ENABLE), 0x00, 0x7F },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX1_UCDR_PI_CONTROLS)             , 0x00, 0x70 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX1_AUX_DATA_TCOARSE_TFINE)       , 0x00, 0xA0 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX1_RCLK_AUXDATA_SEL)             , 0x00, 0x00 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX1_VGA_CAL_CNTRL1)               , 0x00, 0x04 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX1_VGA_CAL_CNTRL2)               , 0x00, 0x37 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX1_GM_CAL)                       , 0x00, 0x05 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX1_RX_EQU_ADAPTOR_CNTRL1)        , 0x00, 0x07 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX1_RX_EQU_ADAPTOR_CNTRL2)        , 0x00, 0x6E },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX1_RX_EQU_ADAPTOR_CNTRL3)        , 0x00, 0x6E },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX1_RX_EQU_ADAPTOR_CNTRL4)        , 0x00, 0x4A },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX1_RX_IDAC_TSETTLE_LOW)          , 0x00, 0xC0 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX1_RX_IDAC_TSETTLE_HIGH)         , 0x00, 0x00 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX1_RX_EQ_OFFSET_ADAPTOR_CNTRL1)  , 0x00, 0x17 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX1_SIGDET_ENABLES)               , 0x00, 0x1C },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX1_SIGDET_CNTRL)                 , 0x00, 0x03 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX1_SIGDET_DEGLITCH_CNTRL)        , 0x00, 0x14 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX1_RX_MODE_00_LOW)               , 0x00, 0x7F },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX1_RX_MODE_00_HIGH)              , 0x00, 0xFF },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX1_RX_MODE_00_HIGH2)             , 0x00, 0xFF },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX1_RX_MODE_00_HIGH3)             , 0x00, 0xDB },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX1_RX_MODE_00_HIGH4)             , 0x00, 0x75 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX1_RX_MODE_01_LOW)               , 0x00, 0x24 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX1_RX_MODE_01_HIGH)              , 0x00, 0xE4 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX1_RX_MODE_01_HIGH2)             , 0x00, 0xEC },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX1_RX_MODE_01_HIGH3)             , 0x00, 0x39 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX1_RX_MODE_01_HIGH4)             , 0x00, 0x36 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX1_RX_MODE_10_LOW)               , 0x00, 0xD4 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX1_RX_MODE_10_HIGH)              , 0x00, 0x54 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX1_RX_MODE_10_HIGH2)             , 0x00, 0xDB },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX1_RX_MODE_10_HIGH3)             , 0x00, 0x39 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX1_RX_MODE_10_HIGH4)             , 0x00, 0x31 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX1_DCC_CTRL1)                    , 0x00, 0x0C },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_APCS_COM_REFGEN_REQ_CONFIG1)               , 0x00, 0x05 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_APCS_COM_RX_SIGDET_LVL)                    , 0x00, 0xAA },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_APCS_COM_RATE_SLEW_CNTRL1)                 , 0x00, 0x0B },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_APCS_COM_P2U3_WAKEUP_DLY_TIME_AUXCLK_L)    , 0x00, 0x01 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_APCS_COM_EQ_CONFIG5)                       , 0x00, 0x01 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_APCS_PCIE_POWER_STATE_CONFIG2)             , 0x00, 0x0D },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_APCS_PCIE_POWER_STATE_CONFIG4)             , 0x00, 0x07 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_APCS_PCIE_ENDPOINT_REFCLK_DRIVE)           , 0x00, 0xC1 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_APCS_PCIE_L1P1_WAKEUP_DLY_TIME_AUXCLK_L)   , 0x00, 0x01 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_APCS_PCIE_L1P2_WAKEUP_DLY_TIME_AUXCLK_L)   , 0x00, 0x01 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_APCS_PCIE_OSC_DTCT_ACTIONS)                , 0x00, 0x00 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_APCS_PCIE_PRESET_P10_PRE)                  , 0x00, 0x00 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_APCS_PCIE_PRESET_P10_POST)                 , 0x00, 0x58 },

  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_PLL_SSC_EN_CENTER)                , 0x00, 0x00 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_PLL_SSC_PER1)                     , 0x00, 0x31 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_PLL_SSC_PER2)                     , 0x00, 0x01 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_PLL_SSC_STEP_SIZE1_MODE0)         , 0x00, 0xDE },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_PLL_SSC_STEP_SIZE2_MODE0)         , 0x00, 0x07 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_PLL_SSC_STEP_SIZE1_MODE1)         , 0x00, 0x4C },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_PLL_SSC_STEP_SIZE2_MODE1)         , 0x00, 0x06 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_PLL_BIAS_EN_CLKBUFLR_EN)          , 0x00, 0x1C },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_PLL_CLK_ENABLE1)                  , 0x00, 0x90 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_PLL_PLL_IVCO)                     , 0x00, 0x0F },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_PLL_CP_CTRL_MODE0)                , 0x00, 0x06 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_PLL_CP_CTRL_MODE1)                , 0x00, 0x06 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_PLL_PLL_RCTRL_MODE0)              , 0x00, 0x16 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_PLL_PLL_RCTRL_MODE1)              , 0x00, 0x16 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_PLL_PLL_CCTRL_MODE0)              , 0x00, 0x36 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_PLL_PLL_CCTRL_MODE1)              , 0x00, 0x36 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_PLL_SYSCLK_EN_SEL)                , 0x00, 0x08 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_PLL_LOCK_CMP_EN)                  , 0x00, 0x42 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_PLL_LOCK_CMP1_MODE0)              , 0x00, 0x0A },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_PLL_LOCK_CMP2_MODE0)              , 0x00, 0x1A },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_PLL_LOCK_CMP1_MODE1)              , 0x00, 0x14 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_PLL_LOCK_CMP2_MODE1)              , 0x00, 0x34 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_PLL_DEC_START_MODE0)              , 0x00, 0x82 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_PLL_DEC_START_MODE1)              , 0x00, 0x68 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_PLL_DIV_FRAC_START1_MODE0)        , 0x00, 0x55 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_PLL_DIV_FRAC_START2_MODE0)        , 0x00, 0x55 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_PLL_DIV_FRAC_START3_MODE0)        , 0x00, 0x03 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_PLL_DIV_FRAC_START1_MODE1)        , 0x00, 0xAB },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_PLL_DIV_FRAC_START2_MODE1)        , 0x00, 0xAA },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_PLL_DIV_FRAC_START3_MODE1)        , 0x00, 0x02 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_PLL_VCO_TUNE_MAP)                 , 0x00, 0x02 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_PLL_VCO_TUNE1_MODE0)              , 0x00, 0x24 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_PLL_VCO_TUNE1_MODE1)              , 0x00, 0xB4 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_PLL_VCO_TUNE2_MODE1)              , 0x00, 0x03 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_PLL_CLK_SELECT)                   , 0x00, 0x34 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_PLL_HSCLK_SEL)                    , 0x00, 0x01 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_PLL_CORECLK_DIV_MODE1)            , 0x00, 0x08 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_PLL_BIN_VCOCAL_CMP_CODE1_MODE0)   , 0x00, 0xB9 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_PLL_BIN_VCOCAL_CMP_CODE2_MODE0)   , 0x00, 0x1E },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_PLL_BIN_VCOCAL_CMP_CODE1_MODE1)   , 0x00, 0x94 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_PLL_BIN_VCOCAL_CMP_CODE2_MODE1)   , 0x00, 0x18 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_PLL_BIN_VCOCAL_HSCLK_SEL)         , 0x00, 0x11 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_TX0_LANE_MODE_1)                  , 0x00, 0x05 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_TX0_RCV_DETECT_LVL_2)             , 0x00, 0x12 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX0_UCDR_FO_GAIN)                 , 0x00, 0x0C },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX0_UCDR_SO_GAIN)                 , 0x00, 0x03 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX0_UCDR_SO_SATURATION_AND_ENABLE), 0x00, 0x7F },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX0_UCDR_PI_CONTROLS)             , 0x00, 0x70 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX0_AUX_DATA_TCOARSE_TFINE)       , 0x00, 0xA0 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX0_RCLK_AUXDATA_SEL)             , 0x00, 0x00 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX0_VGA_CAL_CNTRL1)               , 0x00, 0x04 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX0_VGA_CAL_CNTRL2)               , 0x00, 0x37 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX0_GM_CAL)                       , 0x00, 0x05 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX0_RX_EQU_ADAPTOR_CNTRL1)        , 0x00, 0x07 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX0_RX_EQU_ADAPTOR_CNTRL2)        , 0x00, 0x6E },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX0_RX_EQU_ADAPTOR_CNTRL3)        , 0x00, 0x6E },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX0_RX_EQU_ADAPTOR_CNTRL4)        , 0x00, 0x4A },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX0_RX_IDAC_TSETTLE_LOW)          , 0x00, 0xC0 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX0_RX_IDAC_TSETTLE_HIGH)         , 0x00, 0x00 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX0_RX_EQ_OFFSET_ADAPTOR_CNTRL1)  , 0x00, 0x17 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX0_SIGDET_ENABLES)               , 0x00, 0x1C },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX0_SIGDET_CNTRL)                 , 0x00, 0x03 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX0_SIGDET_DEGLITCH_CNTRL)        , 0x00, 0x14 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX0_RX_MODE_00_LOW)               , 0x00, 0x7F },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX0_RX_MODE_00_HIGH)              , 0x00, 0xFF },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX0_RX_MODE_00_HIGH2)             , 0x00, 0xFF },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX0_RX_MODE_00_HIGH3)             , 0x00, 0xDB },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX0_RX_MODE_00_HIGH4)             , 0x00, 0x75 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX0_RX_MODE_01_LOW)               , 0x00, 0x24 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX0_RX_MODE_01_HIGH)              , 0x00, 0xE4 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX0_RX_MODE_01_HIGH2)             , 0x00, 0xEC },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX0_RX_MODE_01_HIGH3)             , 0x00, 0x39 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX0_RX_MODE_01_HIGH4)             , 0x00, 0x36 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX0_RX_MODE_10_LOW)               , 0x00, 0xD4 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX0_RX_MODE_10_HIGH)              , 0x00, 0x54 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX0_RX_MODE_10_HIGH2)             , 0x00, 0xDB },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX0_RX_MODE_10_HIGH3)             , 0x00, 0x39 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX0_RX_MODE_10_HIGH4)             , 0x00, 0x31 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX0_DCC_CTRL1)                    , 0x00, 0x0C },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_TX1_LANE_MODE_1)                  , 0x00, 0x05 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_TX1_RCV_DETECT_LVL_2)             , 0x00, 0x12 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX1_UCDR_FO_GAIN)                 , 0x00, 0x0C },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX1_UCDR_SO_GAIN)                 , 0x00, 0x03 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX1_UCDR_SO_SATURATION_AND_ENABLE), 0x00, 0x7F },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX1_UCDR_PI_CONTROLS)             , 0x00, 0x70 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX1_AUX_DATA_TCOARSE_TFINE)       , 0x00, 0xA0 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX1_RCLK_AUXDATA_SEL)             , 0x00, 0x00 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX1_VGA_CAL_CNTRL1)               , 0x00, 0x04 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX1_VGA_CAL_CNTRL2)               , 0x00, 0x37 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX1_GM_CAL)                       , 0x00, 0x05 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX1_RX_EQU_ADAPTOR_CNTRL1)        , 0x00, 0x07 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX1_RX_EQU_ADAPTOR_CNTRL2)        , 0x00, 0x6E },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX1_RX_EQU_ADAPTOR_CNTRL3)        , 0x00, 0x6E },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX1_RX_EQU_ADAPTOR_CNTRL4)        , 0x00, 0x4A },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX1_RX_IDAC_TSETTLE_LOW)          , 0x00, 0xC0 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX1_RX_IDAC_TSETTLE_HIGH)         , 0x00, 0x00 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX1_RX_EQ_OFFSET_ADAPTOR_CNTRL1)  , 0x00, 0x17 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX1_SIGDET_ENABLES)               , 0x00, 0x1C },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX1_SIGDET_CNTRL)                 , 0x00, 0x03 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX1_SIGDET_DEGLITCH_CNTRL)        , 0x00, 0x14 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX1_RX_MODE_00_LOW)               , 0x00, 0x7F },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX1_RX_MODE_00_HIGH)              , 0x00, 0xFF },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX1_RX_MODE_00_HIGH2)             , 0x00, 0xFF },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX1_RX_MODE_00_HIGH3)             , 0x00, 0xDB },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX1_RX_MODE_00_HIGH4)             , 0x00, 0x75 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX1_RX_MODE_01_LOW)               , 0x00, 0x24 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX1_RX_MODE_01_HIGH)              , 0x00, 0xE4 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX1_RX_MODE_01_HIGH2)             , 0x00, 0xEC },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX1_RX_MODE_01_HIGH3)             , 0x00, 0x39 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX1_RX_MODE_01_HIGH4)             , 0x00, 0x36 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX1_RX_MODE_10_LOW)               , 0x00, 0xD4 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX1_RX_MODE_10_HIGH)              , 0x00, 0x54 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX1_RX_MODE_10_HIGH2)             , 0x00, 0xDB },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX1_RX_MODE_10_HIGH3)             , 0x00, 0x39 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX1_RX_MODE_10_HIGH4)             , 0x00, 0x31 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX1_DCC_CTRL1)                    , 0x00, 0x0C },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BPCS_COM_REFGEN_REQ_CONFIG1)               , 0x00, 0x05 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BPCS_COM_RX_SIGDET_LVL)                    , 0x00, 0xAA },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BPCS_COM_RATE_SLEW_CNTRL1)                 , 0x00, 0x0B },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BPCS_COM_P2U3_WAKEUP_DLY_TIME_AUXCLK_L)    , 0x00, 0x01 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BPCS_COM_EQ_CONFIG5)                       , 0x00, 0x01 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BPCS_PCIE_POWER_STATE_CONFIG2)             , 0x00, 0x0D },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BPCS_PCIE_POWER_STATE_CONFIG4)             , 0x00, 0x07 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BPCS_PCIE_ENDPOINT_REFCLK_DRIVE)           , 0x00, 0xC1 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BPCS_PCIE_L1P1_WAKEUP_DLY_TIME_AUXCLK_L)   , 0x00, 0x01 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BPCS_PCIE_L1P2_WAKEUP_DLY_TIME_AUXCLK_L)   , 0x00, 0x01 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BPCS_PCIE_OSC_DTCT_ACTIONS)                , 0x00, 0x00 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BPCS_PCIE_PRESET_P10_PRE)                  , 0x00, 0x00 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BPCS_PCIE_PRESET_P10_POST)                 , 0x00, 0x58 },

  { invalid_reg, PHY_REG_NOP, 0xFF, 0xFF, 0xFF },
};

/*******************************************************************************
 * V2 Chipset specific port 3 PHY init sequence
 ******************************************************************************/
STATIC CONST pciePhyRegSeq_t v2_port_3_phy_init_seq[] =
{
  /* Register Base, Operation, Offset, Mask & Value */
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_PCIE_PARF_DEVICE_TYPE)                             , 0x00, 0x04 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_APCS_COM_POWER_DOWN_CONTROL)               , 0x00, 0x03 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_PLL_SSC_EN_CENTER)                , 0x00, 0x00 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_PLL_SSC_PER1)                     , 0x00, 0x31 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_PLL_SSC_PER2)                     , 0x00, 0x01 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_PLL_SSC_STEP_SIZE1_MODE0)         , 0x00, 0xDE },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_PLL_SSC_STEP_SIZE2_MODE0)         , 0x00, 0x07 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_PLL_SSC_STEP_SIZE1_MODE1)         , 0x00, 0x4C },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_PLL_SSC_STEP_SIZE2_MODE1)         , 0x00, 0x06 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_PLL_BIAS_EN_CLKBUFLR_EN)          , 0x00, 0x1C },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_PLL_CLK_ENABLE1)                  , 0x00, 0x90 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_PLL_PLL_IVCO)                     , 0x00, 0x0F },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_PLL_CP_CTRL_MODE0)                , 0x00, 0x06 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_PLL_CP_CTRL_MODE1)                , 0x00, 0x06 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_PLL_PLL_RCTRL_MODE0)              , 0x00, 0x16 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_PLL_PLL_RCTRL_MODE1)              , 0x00, 0x16 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_PLL_PLL_CCTRL_MODE0)              , 0x00, 0x36 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_PLL_PLL_CCTRL_MODE1)              , 0x00, 0x36 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_PLL_SYSCLK_EN_SEL)                , 0x00, 0x08 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_PLL_LOCK_CMP_EN)                  , 0x00, 0x42 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_PLL_LOCK_CMP1_MODE0)              , 0x00, 0x0A },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_PLL_LOCK_CMP2_MODE0)              , 0x00, 0x1A },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_PLL_LOCK_CMP1_MODE1)              , 0x00, 0x14 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_PLL_LOCK_CMP2_MODE1)              , 0x00, 0x34 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_PLL_DEC_START_MODE0)              , 0x00, 0x82 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_PLL_DEC_START_MODE1)              , 0x00, 0x68 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_PLL_DIV_FRAC_START1_MODE0)        , 0x00, 0x55 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_PLL_DIV_FRAC_START2_MODE0)        , 0x00, 0x55 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_PLL_DIV_FRAC_START3_MODE0)        , 0x00, 0x03 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_PLL_DIV_FRAC_START1_MODE1)        , 0x00, 0xAB },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_PLL_DIV_FRAC_START2_MODE1)        , 0x00, 0xAA },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_PLL_DIV_FRAC_START3_MODE1)        , 0x00, 0x02 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_PLL_VCO_TUNE_MAP)                 , 0x00, 0x02 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_PLL_VCO_TUNE1_MODE0)              , 0x00, 0x24 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_PLL_VCO_TUNE1_MODE1)              , 0x00, 0xB4 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_PLL_VCO_TUNE2_MODE1)              , 0x00, 0x03 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_PLL_CLK_SELECT)                   , 0x00, 0x34 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_PLL_HSCLK_SEL)                    , 0x00, 0x01 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_PLL_CORECLK_DIV_MODE1)            , 0x00, 0x08 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_PLL_BIN_VCOCAL_CMP_CODE1_MODE0)   , 0x00, 0xB9 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_PLL_BIN_VCOCAL_CMP_CODE2_MODE0)   , 0x00, 0x1E },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_PLL_BIN_VCOCAL_CMP_CODE1_MODE1)   , 0x00, 0x94 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_PLL_BIN_VCOCAL_CMP_CODE2_MODE1)   , 0x00, 0x18 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_PLL_BIN_VCOCAL_HSCLK_SEL)         , 0x00, 0x11 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_TX0_RES_CODE_LANE_OFFSET_TX)      , 0x00, 0x11 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_TX0_LANE_MODE_1)                  , 0x00, 0x35 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_TX0_RCV_DETECT_LVL_2)             , 0x00, 0x12 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX0_UCDR_FO_GAIN)                 , 0x00, 0x0C },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX0_UCDR_SO_GAIN)                 , 0x00, 0x03 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX0_UCDR_SO_SATURATION_AND_ENABLE), 0x00, 0x7F },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX0_UCDR_PI_CONTROLS)             , 0x00, 0x70 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX0_AUX_DATA_TCOARSE_TFINE)       , 0x00, 0x30 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX0_RCLK_AUXDATA_SEL)             , 0x00, 0x00 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX0_VGA_CAL_CNTRL1)               , 0x00, 0x04 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX0_VGA_CAL_CNTRL2)               , 0x00, 0x07 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX0_GM_CAL)                       , 0x00, 0x0D },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX0_RX_EQU_ADAPTOR_CNTRL1)        , 0x00, 0x00 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX0_RX_EQU_ADAPTOR_CNTRL2)        , 0x00, 0x0E },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX0_RX_EQU_ADAPTOR_CNTRL3)        , 0x00, 0x4A },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX0_RX_EQU_ADAPTOR_CNTRL4)        , 0x00, 0x0F },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX0_RX_IDAC_TSETTLE_LOW)          , 0x00, 0xC0 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX0_RX_IDAC_TSETTLE_HIGH)         , 0x00, 0x00 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX0_RX_EQ_OFFSET_ADAPTOR_CNTRL1)  , 0x00, 0x17 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX0_SIGDET_ENABLES)               , 0x00, 0x1C },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX0_SIGDET_CNTRL)                 , 0x00, 0x03 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX0_SIGDET_DEGLITCH_CNTRL)        , 0x00, 0x1E },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX0_RX_MODE_00_LOW)               , 0x00, 0xFF },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX0_RX_MODE_00_HIGH)              , 0x00, 0xFF },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX0_RX_MODE_00_HIGH2)             , 0x00, 0xFF },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX0_RX_MODE_00_HIGH3)             , 0x00, 0x7F },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX0_RX_MODE_00_HIGH4)             , 0x00, 0x66 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX0_RX_MODE_01_LOW)               , 0x00, 0x24 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX0_RX_MODE_01_HIGH)              , 0x00, 0xE4 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX0_RX_MODE_01_HIGH2)             , 0x00, 0xEC },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX0_RX_MODE_01_HIGH3)             , 0x00, 0x3B },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX0_RX_MODE_01_HIGH4)             , 0x00, 0x36 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX0_RX_MODE_10_LOW)               , 0x00, 0xD4 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX0_RX_MODE_10_HIGH)              , 0x00, 0x54 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX0_RX_MODE_10_HIGH2)             , 0x00, 0xDB },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX0_RX_MODE_10_HIGH3)             , 0x00, 0x3B },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX0_RX_MODE_10_HIGH4)             , 0x00, 0x31 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX0_DCC_CTRL1)                    , 0x00, 0x0C },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_TX1_RES_CODE_LANE_OFFSET_TX)      , 0x00, 0x11 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_TX1_LANE_MODE_1)                  , 0x00, 0x35 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_TX1_RCV_DETECT_LVL_2)             , 0x00, 0x12 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX1_UCDR_FO_GAIN)                 , 0x00, 0x0C },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX1_UCDR_SO_GAIN)                 , 0x00, 0x03 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX1_UCDR_SO_SATURATION_AND_ENABLE), 0x00, 0x7F },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX1_UCDR_PI_CONTROLS)             , 0x00, 0x70 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX1_AUX_DATA_TCOARSE_TFINE)       , 0x00, 0x30 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX1_RCLK_AUXDATA_SEL)             , 0x00, 0x00 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX1_VGA_CAL_CNTRL1)               , 0x00, 0x04 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX1_VGA_CAL_CNTRL2)               , 0x00, 0x07 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX1_GM_CAL)                       , 0x00, 0x0D },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX1_RX_EQU_ADAPTOR_CNTRL1)        , 0x00, 0x00 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX1_RX_EQU_ADAPTOR_CNTRL2)        , 0x00, 0x0E },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX1_RX_EQU_ADAPTOR_CNTRL3)        , 0x00, 0x4A },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX1_RX_EQU_ADAPTOR_CNTRL4)        , 0x00, 0x0F },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX1_RX_IDAC_TSETTLE_LOW)          , 0x00, 0xC0 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX1_RX_IDAC_TSETTLE_HIGH)         , 0x00, 0x00 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX1_RX_EQ_OFFSET_ADAPTOR_CNTRL1)  , 0x00, 0x17 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX1_SIGDET_ENABLES)               , 0x00, 0x1C },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX1_SIGDET_CNTRL)                 , 0x00, 0x03 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX1_SIGDET_DEGLITCH_CNTRL)        , 0x00, 0x1E },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX1_RX_MODE_00_LOW)               , 0x00, 0xFF },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX1_RX_MODE_00_HIGH)              , 0x00, 0xFF },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX1_RX_MODE_00_HIGH2)             , 0x00, 0xFF },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX1_RX_MODE_00_HIGH3)             , 0x00, 0x7F },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX1_RX_MODE_00_HIGH4)             , 0x00, 0x66 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX1_RX_MODE_01_LOW)               , 0x00, 0x24 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX1_RX_MODE_01_HIGH)              , 0x00, 0xE4 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX1_RX_MODE_01_HIGH2)             , 0x00, 0xEC },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX1_RX_MODE_01_HIGH3)             , 0x00, 0x3B },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX1_RX_MODE_01_HIGH4)             , 0x00, 0x36 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX1_RX_MODE_10_LOW)               , 0x00, 0xD4 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX1_RX_MODE_10_HIGH)              , 0x00, 0x54 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX1_RX_MODE_10_HIGH2)             , 0x00, 0xDB },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX1_RX_MODE_10_HIGH3)             , 0x00, 0x3B },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX1_RX_MODE_10_HIGH4)             , 0x00, 0x31 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_AQSERDES_RX1_DCC_CTRL1)                    , 0x00, 0x0C },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_APCS_COM_REFGEN_REQ_CONFIG1)               , 0x00, 0x05 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_APCS_COM_RX_SIGDET_LVL)                    , 0x00, 0x88 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_APCS_COM_RATE_SLEW_CNTRL1)                 , 0x00, 0x0B },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_APCS_COM_P2U3_WAKEUP_DLY_TIME_AUXCLK_L)    , 0x00, 0x01 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_APCS_COM_EQ_CONFIG5)                       , 0x00, 0x01 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_APCS_PCIE_POWER_STATE_CONFIG2)             , 0x00, 0x0D },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_APCS_PCIE_POWER_STATE_CONFIG4)             , 0x00, 0x07 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_APCS_PCIE_ENDPOINT_REFCLK_DRIVE)           , 0x00, 0xC1 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_APCS_PCIE_L1P1_WAKEUP_DLY_TIME_AUXCLK_L)   , 0x00, 0x01 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_APCS_PCIE_L1P2_WAKEUP_DLY_TIME_AUXCLK_L)   , 0x00, 0x01 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_APCS_PCIE_OSC_DTCT_ACTIONS)                , 0x00, 0x00 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_APCS_PCIE_PRESET_P6_P7_PRE)                , 0x00, 0x33 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_APCS_PCIE_PRESET_P10_PRE)                  , 0x00, 0x00 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_APCS_PCIE_PRESET_P10_POST)                 , 0x00, 0x58 },

  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_PLL_SSC_EN_CENTER)                , 0x00, 0x00 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_PLL_SSC_PER1)                     , 0x00, 0x31 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_PLL_SSC_PER2)                     , 0x00, 0x01 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_PLL_SSC_STEP_SIZE1_MODE0)         , 0x00, 0xDE },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_PLL_SSC_STEP_SIZE2_MODE0)         , 0x00, 0x07 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_PLL_SSC_STEP_SIZE1_MODE1)         , 0x00, 0x4C },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_PLL_SSC_STEP_SIZE2_MODE1)         , 0x00, 0x06 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_PLL_BIAS_EN_CLKBUFLR_EN)          , 0x00, 0x1C },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_PLL_CLK_ENABLE1)                  , 0x00, 0x90 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_PLL_PLL_IVCO)                     , 0x00, 0x0F },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_PLL_CP_CTRL_MODE0)                , 0x00, 0x06 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_PLL_CP_CTRL_MODE1)                , 0x00, 0x06 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_PLL_PLL_RCTRL_MODE0)              , 0x00, 0x16 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_PLL_PLL_RCTRL_MODE1)              , 0x00, 0x16 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_PLL_PLL_CCTRL_MODE0)              , 0x00, 0x36 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_PLL_PLL_CCTRL_MODE1)              , 0x00, 0x36 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_PLL_SYSCLK_EN_SEL)                , 0x00, 0x08 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_PLL_LOCK_CMP_EN)                  , 0x00, 0x42 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_PLL_LOCK_CMP1_MODE0)              , 0x00, 0x0A },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_PLL_LOCK_CMP2_MODE0)              , 0x00, 0x1A },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_PLL_LOCK_CMP1_MODE1)              , 0x00, 0x14 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_PLL_LOCK_CMP2_MODE1)              , 0x00, 0x34 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_PLL_DEC_START_MODE0)              , 0x00, 0x82 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_PLL_DEC_START_MODE1)              , 0x00, 0x68 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_PLL_DIV_FRAC_START1_MODE0)        , 0x00, 0x55 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_PLL_DIV_FRAC_START2_MODE0)        , 0x00, 0x55 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_PLL_DIV_FRAC_START3_MODE0)        , 0x00, 0x03 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_PLL_DIV_FRAC_START1_MODE1)        , 0x00, 0xAB },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_PLL_DIV_FRAC_START2_MODE1)        , 0x00, 0xAA },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_PLL_DIV_FRAC_START3_MODE1)        , 0x00, 0x02 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_PLL_VCO_TUNE_MAP)                 , 0x00, 0x02 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_PLL_VCO_TUNE1_MODE0)              , 0x00, 0x24 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_PLL_VCO_TUNE1_MODE1)              , 0x00, 0xB4 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_PLL_VCO_TUNE2_MODE1)              , 0x00, 0x03 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_PLL_CLK_SELECT)                   , 0x00, 0x34 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_PLL_HSCLK_SEL)                    , 0x00, 0x01 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_PLL_CORECLK_DIV_MODE1)            , 0x00, 0x08 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_PLL_BIN_VCOCAL_CMP_CODE1_MODE0)   , 0x00, 0xB9 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_PLL_BIN_VCOCAL_CMP_CODE2_MODE0)   , 0x00, 0x1E },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_PLL_BIN_VCOCAL_CMP_CODE1_MODE1)   , 0x00, 0x94 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_PLL_BIN_VCOCAL_CMP_CODE2_MODE1)   , 0x00, 0x18 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_PLL_BIN_VCOCAL_HSCLK_SEL)         , 0x00, 0x11 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_TX0_RES_CODE_LANE_OFFSET_TX)      , 0x00, 0x11 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_TX0_LANE_MODE_1)                  , 0x00, 0x35 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_TX0_RCV_DETECT_LVL_2)             , 0x00, 0x12 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX0_UCDR_FO_GAIN)                 , 0x00, 0x0C },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX0_UCDR_SO_GAIN)                 , 0x00, 0x03 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX0_UCDR_SO_SATURATION_AND_ENABLE), 0x00, 0x7F },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX0_UCDR_PI_CONTROLS)             , 0x00, 0x70 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX0_AUX_DATA_TCOARSE_TFINE)       , 0x00, 0x30 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX0_RCLK_AUXDATA_SEL)             , 0x00, 0x00 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX0_VGA_CAL_CNTRL1)               , 0x00, 0x04 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX0_VGA_CAL_CNTRL2)               , 0x00, 0x07 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX0_GM_CAL)                       , 0x00, 0x0D },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX0_RX_EQU_ADAPTOR_CNTRL1)        , 0x00, 0x00 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX0_RX_EQU_ADAPTOR_CNTRL2)        , 0x00, 0x0E },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX0_RX_EQU_ADAPTOR_CNTRL3)        , 0x00, 0x4A },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX0_RX_EQU_ADAPTOR_CNTRL4)        , 0x00, 0x0F },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX0_RX_IDAC_TSETTLE_LOW)          , 0x00, 0xC0 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX0_RX_IDAC_TSETTLE_HIGH)         , 0x00, 0x00 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX0_RX_EQ_OFFSET_ADAPTOR_CNTRL1)  , 0x00, 0x17 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX0_SIGDET_ENABLES)               , 0x00, 0x1C },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX0_SIGDET_CNTRL)                 , 0x00, 0x03 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX0_SIGDET_DEGLITCH_CNTRL)        , 0x00, 0x1E },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX0_RX_MODE_00_LOW)               , 0x00, 0xFF },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX0_RX_MODE_00_HIGH)              , 0x00, 0xFF },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX0_RX_MODE_00_HIGH2)             , 0x00, 0xFF },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX0_RX_MODE_00_HIGH3)             , 0x00, 0x7F },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX0_RX_MODE_00_HIGH4)             , 0x00, 0x66 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX0_RX_MODE_01_LOW)               , 0x00, 0x24 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX0_RX_MODE_01_HIGH)              , 0x00, 0xE4 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX0_RX_MODE_01_HIGH2)             , 0x00, 0xEC },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX0_RX_MODE_01_HIGH3)             , 0x00, 0x3B },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX0_RX_MODE_01_HIGH4)             , 0x00, 0x36 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX0_RX_MODE_10_LOW)               , 0x00, 0xD4 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX0_RX_MODE_10_HIGH)              , 0x00, 0x54 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX0_RX_MODE_10_HIGH2)             , 0x00, 0xDB },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX0_RX_MODE_10_HIGH3)             , 0x00, 0x3B },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX0_RX_MODE_10_HIGH4)             , 0x00, 0x31 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX0_DCC_CTRL1)                    , 0x00, 0x0C },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_TX1_RES_CODE_LANE_OFFSET_TX)      , 0x00, 0x11 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_TX1_LANE_MODE_1)                  , 0x00, 0x35 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_TX1_RCV_DETECT_LVL_2)             , 0x00, 0x12 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX1_UCDR_FO_GAIN)                 , 0x00, 0x0C },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX1_UCDR_SO_GAIN)                 , 0x00, 0x03 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX1_UCDR_SO_SATURATION_AND_ENABLE), 0x00, 0x7F },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX1_UCDR_PI_CONTROLS)             , 0x00, 0x70 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX1_AUX_DATA_TCOARSE_TFINE)       , 0x00, 0x30 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX1_RCLK_AUXDATA_SEL)             , 0x00, 0x00 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX1_VGA_CAL_CNTRL1)               , 0x00, 0x04 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX1_VGA_CAL_CNTRL2)               , 0x00, 0x07 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX1_GM_CAL)                       , 0x00, 0x0D },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX1_RX_EQU_ADAPTOR_CNTRL1)        , 0x00, 0x00 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX1_RX_EQU_ADAPTOR_CNTRL2)        , 0x00, 0x0E },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX1_RX_EQU_ADAPTOR_CNTRL3)        , 0x00, 0x4A },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX1_RX_EQU_ADAPTOR_CNTRL4)        , 0x00, 0x0F },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX1_RX_IDAC_TSETTLE_LOW)          , 0x00, 0xC0 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX1_RX_IDAC_TSETTLE_HIGH)         , 0x00, 0x00 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX1_RX_EQ_OFFSET_ADAPTOR_CNTRL1)  , 0x00, 0x17 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX1_SIGDET_ENABLES)               , 0x00, 0x1C },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX1_SIGDET_CNTRL)                 , 0x00, 0x03 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX1_SIGDET_DEGLITCH_CNTRL)        , 0x00, 0x1E },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX1_RX_MODE_00_LOW)               , 0x00, 0xFF },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX1_RX_MODE_00_HIGH)              , 0x00, 0xFF },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX1_RX_MODE_00_HIGH2)             , 0x00, 0xFF },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX1_RX_MODE_00_HIGH3)             , 0x00, 0x7F },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX1_RX_MODE_00_HIGH4)             , 0x00, 0x66 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX1_RX_MODE_01_LOW)               , 0x00, 0x24 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX1_RX_MODE_01_HIGH)              , 0x00, 0xE4 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX1_RX_MODE_01_HIGH2)             , 0x00, 0xEC },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX1_RX_MODE_01_HIGH3)             , 0x00, 0x3B },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX1_RX_MODE_01_HIGH4)             , 0x00, 0x36 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX1_RX_MODE_10_LOW)               , 0x00, 0xD4 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX1_RX_MODE_10_HIGH)              , 0x00, 0x54 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX1_RX_MODE_10_HIGH2)             , 0x00, 0xDB },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX1_RX_MODE_10_HIGH3)             , 0x00, 0x3B },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX1_RX_MODE_10_HIGH4)             , 0x00, 0x31 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BQSERDES_RX1_DCC_CTRL1)                    , 0x00, 0x0C },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BPCS_COM_REFGEN_REQ_CONFIG1)               , 0x00, 0x05 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BPCS_COM_RX_SIGDET_LVL)                    , 0x00, 0x88 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BPCS_COM_RATE_SLEW_CNTRL1)                 , 0x00, 0x0B },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BPCS_COM_P2U3_WAKEUP_DLY_TIME_AUXCLK_L)    , 0x00, 0x01 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BPCS_COM_EQ_CONFIG5)                       , 0x00, 0x01 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BPCS_PCIE_POWER_STATE_CONFIG2)             , 0x00, 0x0D },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BPCS_PCIE_POWER_STATE_CONFIG4)             , 0x00, 0x07 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BPCS_PCIE_ENDPOINT_REFCLK_DRIVE)           , 0x00, 0xC1 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BPCS_PCIE_L1P1_WAKEUP_DLY_TIME_AUXCLK_L)   , 0x00, 0x01 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BPCS_PCIE_L1P2_WAKEUP_DLY_TIME_AUXCLK_L)   , 0x00, 0x01 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BPCS_PCIE_OSC_DTCT_ACTIONS)                , 0x00, 0x00 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BPCS_PCIE_PRESET_P6_P7_PRE)                , 0x00, 0x33 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BPCS_PCIE_PRESET_P10_PRE)                  , 0x00, 0x00 },
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BPCS_PCIE_PRESET_P10_POST)                 , 0x00, 0x58 },

  { invalid_reg, PHY_REG_NOP, 0xFF, 0xFF, 0xFF },
};

/*******************************************************************************
 * Chipset specific version independent port 1 PHY power ON sequence
 ******************************************************************************/
STATIC CONST pciePhyRegSeq_t port_3_phy_power_on_seq[] =
{
  /* Register Base, Operation, Offset, Mask & Value */
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_APCS_COM_SW_RESET                         ), 0x00, 0x00 }, // Release software reset
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_APCS_COM_START_CONTROL                    ), 0x00, 0x03 }, // Start PCS and Serdes power SMs
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
STATIC CONST pciePhyRegSeq_t port_3_phy_delay_seq[] =
{
  /* Register Base, Operation, Offset, Mask & Value */
  { invalid_reg, PHY_REG_DELAY, 0xFF, 0xFF, 2000 },
  { invalid_reg, PHY_REG_NOP, 0xFF, 0xFF, 0xFF },
};

/*******************************************************************************
 * poll PHY power ON status of port 3 PHYA
 ******************************************************************************/
STATIC CONST pciePhyRegSeq_t port_3_phy_A_poll_seq[] =
{
  /* Register Base, Operation, Offset, Mask & Value */
  { ahb_reg,     PHY_REG_REA, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_APCS_COM_PCS_STATUS1), HWIO_PCIE_GEN3X4_EDMA_CM_PCIE_APCS_COM_PCS_STATUS1_PHYSTATUS_BMSK, FALSE },
  { invalid_reg, PHY_REG_NOP, 0xFF, 0xFF, 0xFF },
};

/*******************************************************************************
 * poll PHY power ON status of port 3 PHYB
 ******************************************************************************/
STATIC CONST pciePhyRegSeq_t port_3_phy_B_poll_seq[] =
{
  /* Register Base, Operation, Offset, Mask & Value */
  { ahb_reg,     PHY_REG_REA, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BPCS_COM_PCS_STATUS1), HWIO_PCIE_GEN3X4_EDMA_CM_PCIE_BPCS_COM_PCS_STATUS1_PHYSTATUS_BMSK, FALSE },
  { invalid_reg, PHY_REG_NOP, 0xFF, 0xFF, 0xFF },
};


/*******************************************************************************
 * post PHY power ON sequence for port 3 GEN3 PHY
 ******************************************************************************/
STATIC CONST pciePhyRegSeq_t port_3_phy_post_power_up_seq[] =
{
  /* Register Base, Operation, Offset, Mask & Value */
  /* Enable BME & MSE before LTSSM_EN */
  { axi_reg,     PHY_REG_RMW, HWIO_OFFS(PCIE_GEN3X4_EDMA_TYPE1_STATUS_COMMAND_REG), HWIO_PCIE_GEN3X4_EDMA_TYPE1_STATUS_COMMAND_REG_MSE_BMSK, (0x1 << HWIO_PCIE_GEN3X4_EDMA_TYPE1_STATUS_COMMAND_REG_MSE_SHFT)},
  { axi_reg,     PHY_REG_RMW, HWIO_OFFS(PCIE_GEN3X4_EDMA_TYPE1_STATUS_COMMAND_REG), HWIO_PCIE_GEN3X4_EDMA_TYPE1_STATUS_COMMAND_REG_BME_BMSK, (0x1 << HWIO_PCIE_GEN3X4_EDMA_TYPE1_STATUS_COMMAND_REG_BME_SHFT)},
  /* Writing Slave address space size */
  { ahb_reg,    PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_PCIE_PARF_SLV_ADDR_SPACE_SIZE), 0x0, 0x2000000},
  { ahb_reg,    PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_PCIE_PARF_SLV_ADDR_SPACE_SIZE_HI), 0x0, 0x00000000},
  /* Clear REQ_NOT_ENTER_L1 field in PARF PM_CTRL to allow L1 entry */
  { ahb_reg,    PHY_REG_RMW, HWIO_OFFS(PCIE_GEN3X4_EDMA_PCIE_PARF_PM_CTRL), HWIO_PCIE_GEN3X4_EDMA_PCIE_PARF_PM_CTRL_REQ_NOT_ENTR_L1_BMSK, 0x00000000},
  /* Aux power enable for L2 */
  { ahb_reg,    PHY_REG_RMW, HWIO_OFFS(PCIE_GEN3X4_EDMA_PCIE_PARF_SYS_CTRL), HWIO_PCIE_GEN3X1_PCIE_PARF_SYS_CTRL_AUX_PWR_DET_BMSK, (0x1 <<  HWIO_PCIE_GEN3X1_PCIE_PARF_SYS_CTRL_AUX_PWR_DET_SHFT)},
  { axi_reg,     PHY_REG_RMW, HWIO_OFFS(PCIE_GEN3X4_EDMA_GEN3_EQ_FB_MODE_DIR_CHANGE_OFF), HWIO_PCIE_GEN3X4_EDMA_GEN3_EQ_FB_MODE_DIR_CHANGE_OFF_GEN3_EQ_FMDC_N_EVALS_BMSK, (0x0D << HWIO_PCIE_GEN3X4_EDMA_GEN3_EQ_FB_MODE_DIR_CHANGE_OFF_GEN3_EQ_FMDC_N_EVALS_SHFT)},
  { axi_reg,     PHY_REG_RMW, HWIO_OFFS(PCIE_GEN3X4_EDMA_GEN3_EQ_FB_MODE_DIR_CHANGE_OFF), HWIO_PCIE_GEN3X4_EDMA_GEN3_EQ_FB_MODE_DIR_CHANGE_OFF_GEN3_EQ_FMDC_MAX_PRE_CUSROR_DELTA_BMSK, (0x05 << HWIO_PCIE_GEN3X4_EDMA_GEN3_EQ_FB_MODE_DIR_CHANGE_OFF_GEN3_EQ_FMDC_MAX_PRE_CUSROR_DELTA_SHFT)},
  { axi_reg,     PHY_REG_RMW, HWIO_OFFS(PCIE_GEN3X4_EDMA_GEN3_EQ_FB_MODE_DIR_CHANGE_OFF), HWIO_PCIE_GEN3X4_EDMA_GEN3_EQ_FB_MODE_DIR_CHANGE_OFF_GEN3_EQ_FMDC_MAX_POST_CUSROR_DELTA_BMSK, (0x05 << HWIO_PCIE_GEN3X4_EDMA_GEN3_EQ_FB_MODE_DIR_CHANGE_OFF_GEN3_EQ_FMDC_MAX_POST_CUSROR_DELTA_SHFT)},
  { axi_reg,     PHY_REG_RMW, HWIO_OFFS(PCIE_GEN3X4_EDMA_GEN3_EQ_FB_MODE_DIR_CHANGE_OFF), HWIO_PCIE_GEN3X4_EDMA_GEN3_EQ_FB_MODE_DIR_CHANGE_OFF_GEN3_EQ_FMDC_T_MIN_PHASE23_BMSK, 0x00 },
  { axi_reg,     PHY_REG_RMW, HWIO_OFFS(PCIE_GEN3X4_EDMA_GEN3_EQ_CONTROL_OFF), HWIO_PCIE_GEN3X4_EDMA_GEN3_EQ_CONTROL_OFF_GEN3_EQ_PHASE23_EXIT_MODE_BMSK, 0x00 },
  { axi_reg,     PHY_REG_RMW, HWIO_OFFS(PCIE_GEN3X4_EDMA_GEN3_EQ_CONTROL_OFF), HWIO_PCIE_GEN3X4_EDMA_GEN3_EQ_CONTROL_OFF_GEN3_EQ_PSET_REQ_VEC_BMSK, 0x00 },
  /* Enable DBI_RO_WR_EN to access CS1 region */
  { axi_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_MISC_CONTROL_1_OFF), 0x0, 0x1},
  /* Memory Barrier */
  { invalid_reg, PHY_REG_DSB, 0xFF, 0xFF, 0xFF},
  /* Update Base class and Sub class */
  { axi_reg,     PHY_REG_RMW, HWIO_OFFS(PCIE_GEN3X4_EDMA_TYPE1_CLASS_CODE_REV_ID_REG),
    (HWIO_PCIE_GEN3X4_EDMA_TYPE1_CLASS_CODE_REV_ID_REG_BASE_CLASS_CODE_BMSK | HWIO_PCIE_GEN3X4_EDMA_TYPE1_CLASS_CODE_REV_ID_REG_SUBCLASS_CODE_BMSK),
    (PCIE_CLASS_CODE_BRIDGE_DEVICE | PCIE_SUBCLASS_CODE_PCI_2_PCI_BRIDGE)},
  { axi_reg,     PHY_REG_RMW, HWIO_OFFS(PCIE_GEN3X4_EDMA_SLOT_CAPABILITIES_REG), HWIO_PCIE_GEN3X4_EDMA_SLOT_CAPABILITIES_REG_PCIE_CAP_HOT_PLUG_CAPABLE_BMSK, 0x00000000},
  /* Assert CS2 */
  { axi_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_PCIE_ELBI_CS2_ENABLE), 0x0, 0x1},
  /* Memory Barrier */
  { invalid_reg, PHY_REG_DSB, 0xFF, 0xFF, 0xFF},
  /* Disable BAR0 and BAR1 */
  { axi_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_TYPE1_BAR0_REG), 0x0, 0x0},
  { axi_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_TYPE1_BAR1_REG), 0x0, 0x0},
  /* Fix NFTS */
  { axi_reg,     PHY_REG_RMW, HWIO_OFFS(PCIE_GEN3X4_EDMA_ACK_F_ASPM_CTRL_OFF), HWIO_PCIE_GEN3X4_EDMA_ACK_F_ASPM_CTRL_OFF_COMMON_CLK_N_FTS_BMSK, (0x80 << HWIO_PCIE_GEN3X4_EDMA_ACK_F_ASPM_CTRL_OFF_COMMON_CLK_N_FTS_SHFT) },
  { axi_reg,     PHY_REG_RMW, HWIO_OFFS(PCIE_GEN3X4_EDMA_ACK_F_ASPM_CTRL_OFF), HWIO_PCIE_GEN3X4_EDMA_ACK_F_ASPM_CTRL_OFF_ACK_N_FTS_BMSK, (0x80 << HWIO_PCIE_GEN3X4_EDMA_ACK_F_ASPM_CTRL_OFF_ACK_N_FTS_SHFT) },
  /* Treats out of BAR TLPs as supported request - MUST for RC mode */
  { axi_reg,     PHY_REG_RMW, HWIO_OFFS(PCIE_GEN3X4_EDMA_SYMBOL_TIMER_FILTER_1_OFF), HWIO_PCIE_GEN3X4_EDMA_SYMBOL_TIMER_FILTER_1_OFF_MASK_RADM_1_BMSK, (0x4 << HWIO_PCIE_GEN3X4_EDMA_SYMBOL_TIMER_FILTER_1_OFF_MASK_RADM_1_SHFT) },
  { axi_reg,     PHY_REG_RMW, HWIO_OFFS(PCIE_GEN3X4_EDMA_GEN3_RELATED_OFF), HWIO_PCIE_GEN3X4_EDMA_GEN3_RELATED_OFF_GEN3_ZRXDC_NONCOMPL_BMSK, 0x00 },
  { axi_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_PL_LTR_LATENCY_OFF), 0x0, 0x88468846 },
  { axi_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_AUX_CLK_FREQ_OFF), 0x0, 0x13 },
  { axi_reg,     PHY_REG_RMW, HWIO_OFFS(PCIE_GEN3X4_EDMA_GEN2_CTRL_OFF), HWIO_PCIE_GEN3X4_EDMA_GEN2_CTRL_OFF_NUM_OF_LANES_BMSK, (0x1 << HWIO_PCIE_GEN3X4_EDMA_GEN2_CTRL_OFF_NUM_OF_LANES_SHFT) },
  { axi_reg,     PHY_REG_RMW, HWIO_OFFS(PCIE_GEN3X4_EDMA_PORT_LINK_CTRL_OFF), HWIO_PCIE_GEN3X4_EDMA_PORT_LINK_CTRL_OFF_LINK_CAPABLE_BMSK, (0x4 << HWIO_PCIE_GEN3X4_EDMA_PORT_LINK_CTRL_OFF_LINK_CAPABLE_SHFT) },
  /* De-Assert CS2 */
  { axi_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_PCIE_ELBI_CS2_ENABLE), 0x0, 0x0},
  /* Disable DBI_RO_WR_EN to access CS1 region */
  { axi_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_MISC_CONTROL_1_OFF), 0x0, 0x0},
  /* Memory Barrier */
  { invalid_reg, PHY_REG_DSB, 0xFF, 0xFF, 0xFF},
  { invalid_reg, PHY_REG_NOP, 0xFF, 0xFF, 0xFF },
};


/*******************************************************************************
 * Chipset specific complete port 3 PHY init sequence
 ******************************************************************************/
STATIC CONST pciePhyAllVerSeq_t sdm1000_port_3_phy_init_seq[] =
{
  /* First write the register sequence corresponds to version 1 */
  {Pcie_Qcom_SoC_Ver_1,    v1_port_3_phy_init_seq},
  /* Write the register sequence corresponds to version 2 */
  {Pcie_Qcom_SoC_Ver_2,    v2_port_3_phy_init_seq},
  /* Once all the version dependent sequences are written, power ON PHY */
  {Pcie_Qcom_SoC_Ver_ALL,  port_3_phy_power_on_seq},
  /* Once the PHY is brought of reset, give a finite delay */
  {Pcie_Qcom_SoC_Ver_ALL,  port_3_phy_delay_seq},
  /* Check if the PHY A PLL is locked */
  {Pcie_Qcom_SoC_Ver_POLL, port_3_phy_A_poll_seq},
  /* Check if the PHY B PLL is locked */
  {Pcie_Qcom_SoC_Ver_POLL, port_3_phy_B_poll_seq},
  /* Perform post power up sequence */
  {Pcie_Qcom_SoC_Ver_ALL,  port_3_phy_post_power_up_seq},
  /* End of PHY init sequence */
  {Pcie_Qcom_SoC_Ver_STOP, NULL},
};

/*******************************************************************************
 * Chipset specific port 3 PHY De-init sequence
 ******************************************************************************/
STATIC CONST pciePhyRegSeq_t port_3_phy_de_init_seq[] =
{
  /* Register Base, Operation, Offset, Mask & Value */
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_APCS_COM_POWER_DOWN_CONTROL               ), 0x00, 0x00},
  { ahb_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3X4_EDMA_CM_PCIE_BPCS_COM_POWER_DOWN_CONTROL               ), 0x00, 0x00},
  { invalid_reg, PHY_REG_NOP, 0xFF, 0xFF, 0xFF },
};
#endif /* (RPMASK & PCIE_RP(3)) - root port 3 enabled */

/*******************************************************************************
 * Common Register region information
 ******************************************************************************/
static memRegions_t pcie_common_reg[max_mem_reg] =
{
  /**< axi_reg: DBI register space */
  {.pa=PCIE_EMPTY_REG_BASE, .sz=PCIE_EMPTY_REG_SIZE},
  /**< ahb_reg: PARF register space */
  {.pa=PCIE_EMPTY_REG_BASE, .sz=PCIE_EMPTY_REG_SIZE},
  /**< tcsr_reg: Access TCSR register space */
  {.pa=CORE_TOP_CSR_BASE, .sz=CORE_TOP_CSR_BASE_SIZE},
  /**< invalid_reg: Invalid reg base to end the PHY init sequence */
  {.pa=PCIE_INVALID_REG_BASE, .sz=PCIE_INVALID_REG_SIZE}
};

/*******************************************************************************
 * PCIe common gcc reset clock name list with NULL ending
 ******************************************************************************/
STATIC CONST char * pcie_reset_clks_cmn[] =
{
  "gcc_pcie_phy_aux_clk",
  NULL
};

/*******************************************************************************
 * PCIe common gcc clock name list with NULL ending
 ******************************************************************************/
STATIC CONST char * pcie_gcc_clks_cmn[] =
{
  "gcc_aggre_noc_pcie_tbu_clk",
  "gcc_pcie_phy_aux_clk",
  NULL
};

/*******************************************************************************
 * PCIe aggregated gcc clock name list common for both root ports
 ******************************************************************************/
STATIC CONST pcieClkDesc_t pcie_clk_cmn[] =
{
  {
    .reset = pcie_reset_clks_cmn,
    .external = NULL,
    .gcc = pcie_gcc_clks_cmn
  }
};

/*******************************************************************************
 * Chipset specific PHY init sequence
 ******************************************************************************/
STATIC CONST pciePhyRegSeq_t common_phy_init_seq[] =
{
  /* Register Base, Operation, Offset, Mask & Value */
  /* 2x2 PHY mode */
  { tcsr_reg, PHY_REG_WRI, HWIO_OFFS(TCSR_PCIEPHY_LINK_CONFIG), 0x00, 0x00 },
  { invalid_reg, PHY_REG_NOP, 0xFF, 0xFF, 0xFF },
};

/*******************************************************************************
 * Chipset specific complete common PHY init sequence
 ******************************************************************************/
STATIC CONST pciePhyAllVerSeq_t sdm1000_common_phy_init_seq[] =
{
  {Pcie_Qcom_SoC_Ver_ALL , common_phy_init_seq},
  {Pcie_Qcom_SoC_Ver_STOP, NULL},
};

/*******************************************************************************
 * array of structures will hold the information
 * of available root ports
 ******************************************************************************/
static portInfo_t multi_port_info[] =
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
/* Include port_info for RP1 if it exist */
#if (RPMASK & PCIE_RP(1))
  {
    .clk                 = pcie_clk_rp1,
    .pwr                 = "pcie_1_gdsc",
    .icb_bw              = pcie_1_icb_bw,
    .clk_set             = pcie_1_clk_settings,
    .gpio_cfg            = gpio_config_rp1,
    .mem                 = port_1_reg,
    .port_phy_init_seq   = sdm1000_port_1_phy_init_seq,
    .port_phy_deinit_seq = port_1_phy_de_init_seq,
    .link                = port1_link,
    .hw_rp_index         = 1,
    .io_coherency_en     = FALSE,
    .iommu_hdl           = "\\_SB_.PCI2",
    .num_ports           = NUM_PCIE_1_PORTS
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
    .port_phy_init_seq   = sdm1000_port_2_phy_init_seq,
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
STATIC CONST rootPort_t root_port[] = {
  {
    .port_info = multi_port_info,
    .num_rp    = ARRAY_LENGTH(multi_port_info)
  }
};

/*******************************************************************************
 * The single structure will hold all the information needed to bring the
 * root ports out of reset
 ******************************************************************************/
STATIC CONST pcieBsp_t PcieBsp[] =
{
  {
    /* null-terminated string common PHY clock name
     * Since, there is no common PHY in MSM8998,
     * this is assigned with NULL
     */
    .phy_clk       = pcie_clk_cmn,
    /* common PHY init sequence */
    .phy_init_seq  = sdm1000_common_phy_init_seq,
    /* root port info for all enabled root ports */
    .rp_info       = root_port
  }
};
