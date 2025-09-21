/*============================================================================
  FILE:         PCieBsp.c

  OVERVIEW:     BSP for Pcie.

  DEPENDENCIES: None

                Copyright (c) 2016-2018 Qualcomm Technologies, Inc.
                All Rights Reserved.
============================================================================*/
/*============================================================================
  EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.  Please
  use ISO format for dates.

  when        who  what, where, why
  ----------  ---  -----------------------------------------------------------
  03/28/18    ts  Disable SHPC/Native PCIe hot plug for GEN3 root port 
  03/13/18    ts  Configure ASPM from device settings
  01/23/2018  ts  Included support for more post phy init sequence
  01/23/2018  ts  reduced ECAM region for GEN3 RP to 32MB based on IPCAT
  10/26/2017  ts  Moved the PERST GPIO function to '0' from '1'
  07/26/2017  mk  Added support for IoMMU driver to enable IO Coherency
  05/22/2017  ts  Enabled GEN3 controller
  05/05/2017  ts  Updated to support PCIE1 and included support for read
                  modify write during PHY init sequence
  04/24/2017  ts  Updated clock resets and added ICB BW vote support
  04/10/2017  ts  Included support for SDM845
  02/10/2017  ts  Enabled multi version PHY init sequence
                  Enabled IO coherency based on io_coherency_en flag
  10/07/2016  ts  Switched port and common phy init sequences, provided clocks
                  in three categories, external, gcc and reset clocks
  08/10/2016  ts  Added Hw root port index and PHY down seq
  07/08/2016  ts  Made changes to cater the new PcieBsp.h data struct
  06/08/2016  ts  updated with GPIO settings as well as clock reset changes
  06/08/2016  ts  Added pcie PHY init sequence and updated the GPIOs as well
  02/08/2016  ts  Added pcie xml config support.

============================================================================*/
/*----------------------------------------------------------------------------
 * Include Files
 * -------------------------------------------------------------------------*/
#include "PcieBsp.h"
#include "PcieHwioRegs.h"

/*----------------------------------------------------------------------------
 * Preprocessor Definitions and Constants
 * -------------------------------------------------------------------------*/
#define ARRAY_LENGTH(a)                     (sizeof(a) / sizeof((a)[0]))
#define PCIE_CLASS_CODE_BRIDGE_DEVICE       (0x06000000)
#define PCIE_SUBCLASS_CODE_PCI_2_PCI_BRIDGE (0x00040000)

#define PCIE_RP(x)                          (1UL << (x))

/***************************************************************************//**
 * @def RPMASK
 *
 * RPMASK will hold the information how many root ports to be enabled
 * and which are the root ports will be enabled
 *
 * using this def, total number of port info created inside rootPort_t
 * will be restricted
 *
 * @note
 * If Root port 0 and 1 need to be enabled, then RPMASK should be
 * #define RPMASK  (PCIE_RP(0) | PCIE_RP(1))
 ******************************************************************************/
#define RPMASK (PCIE_RP(1) | PCIE_RP(0))

/*----------------------------------------------------------------------------
 * Static Variable Definitions
 * -------------------------------------------------------------------------*/

/*******************************************************************************
 * This section will hold the port info data for root port 0
 ******************************************************************************/
#if (RPMASK & PCIE_RP(0))

/* Number of PCIe lanes present in RP0 */
#define NUM_PCIE_0_LANES 0x01

/*******************************************************************************
 * Register region information for PCIe GEN2 root port 0
 ******************************************************************************/
static memRegions_t port_0_reg[max_mem_reg] =
{
  /**< dbi_reg: Access DBI register space */
  {.pa=PCIE_0_PCIE20_DBI_REG_BASE, .sz=PCIE_0_PCIE20_DBI_REG_SIZE},
  /**< elbi_reg: Access ELBI register space */
  {.pa=PCIE_0_PCIE20_ELBI_REG_BASE, .sz=PCIE_0_PCIE20_ELBI_REG_SIZE},
  /**< parf_reg: Access PARF register space */
  {.pa=PCIE_0_PCIE20_PARF_REG_BASE, .sz=PCIE_0_PCIE20_PARF_REG_SIZE},
  /**< pcs_reg: Access PHY PCS register space */
  {.pa=PCIE_0_PCIE_USB3_UNI_PCS_REG_BASE, .sz=PCIE_0_PCIE_USB3_UNI_PCS_REG_SIZE},
  /**< phy_reg: Access PHY register space */
  {.pa=PCIE_0_QSERDES_COM_REG_BASE, .sz=PCIE_0_QSERDES_COM_REG_SIZE},
  /**< phy_rx_reg: Access Rx register space of PHY */
  {.pa=PCIE_0_QSERDES_RX_REG_BASE, .sz=PCIE_0_QSERDES_RX_REG_SIZE},
  /**< phy_tx_reg=6: Access Tx register space of PHY */
  {.pa=PCIE_0_QSERDES_TX_REG_BASE, .sz=PCIE_0_QSERDES_TX_REG_SIZE},
  /**< phy_misc_reg: Access Misc PCS register space of PHY */
  {.pa=PCIE_0_PCIE_USB3_UNI_PCS_MISC_REG_BASE, .sz=PCIE_0_PCIE_USB3_UNI_PCS_MISC_REG_SIZE},
  /**< qhp_l0_reg: Access QHP L0 register space */
  {.pa=PCIE_INVALID_REG_BASE, .sz=PCIE_INVALID_REG_SIZE},
  /**< qhp_l1_reg: Access QHP L1 register space */
  {.pa=PCIE_INVALID_REG_BASE, .sz=PCIE_INVALID_REG_SIZE},
  /**< axi_reg: Access AXI register space */
  {.pa=PCIE_0_PCIE20_WRAPPER_AXI_BASE, .sz=PCIE_0_PCIE20_WRAPPER_AXI_BASE_SIZE},
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
    .TlmmGpioParam.GpioNum = 39, // WIGIG_EN
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
    .TlmmGpioParam.GpioNum = 39, // WIGIG_EN
    .TlmmGpioParam.Func = 0,
    .TlmmGpioParam.Direction = GPIO_OUTPUT,
    .TlmmGpioParam.Pull = GPIO_NO_PULL,
    .TlmmGpioParam.DriveStrength = GPIO_2MA,
    .TlmmGpioParam.value = GPIO_HIGH_VALUE
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
    .DelayValue = 10000 // 10 ms delay needed for WIGIG
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
    .trained   = FALSE,
    .num_lanes = NUM_PCIE_0_LANES,
    .aspm      = aspm_l1_ena_l0s_dis
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
  /* Set Device Type */
  { parf_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_0_PCIE20_PARF_DEVICE_TYPE),                             0x0, 0x04 },
  /* Power up init sequence for RootComplex configuration  */
  { pcs_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_0_PCIE_USB3_UNI_PCS_POWER_DOWN_CONTROL),                 0x0, 0x01 },
  /* PHY common block settings */
  { phy_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_0_QSERDES_COM_BIAS_EN_CLKBUFLR_EN),                      0x0, 0x14 },
  { phy_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_0_QSERDES_COM_CLK_SELECT),                               0x0, 0x30 },
  { phy_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_0_QSERDES_COM_PLL_IVCO),                                 0x0, 0x07 },
  { phy_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_0_QSERDES_COM_CMN_CONFIG),                               0x0, 0x06 },
  { phy_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_0_QSERDES_COM_LOCK_CMP_EN),                              0x0, 0x01 },
  { phy_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_0_QSERDES_COM_RESETSM_CNTRL),                            0x0, 0x20 },
  { phy_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_0_QSERDES_COM_VCO_TUNE_MAP),                             0x0, 0x00 },
  { phy_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_0_QSERDES_COM_VCO_TUNE2_MODE0),                          0x0, 0x01 },
  { phy_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_0_QSERDES_COM_VCO_TUNE1_MODE0),                          0x0, 0xC9 },
  { phy_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_0_QSERDES_COM_VCO_TUNE_TIMER1),                          0x0, 0xFF },
  { phy_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_0_QSERDES_COM_VCO_TUNE_TIMER2),                          0x0, 0x3F },
  { phy_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_0_QSERDES_COM_SVS_MODE_CLK_SEL),                         0x0, 0x01 },
  { phy_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_0_QSERDES_COM_CORE_CLK_EN),                              0x0, 0x00 },
  { phy_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_0_QSERDES_COM_CORECLK_DIV_MODE0),                        0x0, 0x0A },
  { phy_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_0_QSERDES_COM_CLK_EP_DIV),                               0x0, 0x19 },
  { phy_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_0_QSERDES_COM_CLK_ENABLE1),                              0x0, 0x90 },
  /* PLL settings and other PLL related CMN block settings */
  { phy_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_0_QSERDES_COM_DEC_START_MODE0),                          0x0, 0x82 },
  { phy_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_0_QSERDES_COM_DIV_FRAC_START3_MODE0),                    0x0, 0x02 },
  { phy_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_0_QSERDES_COM_DIV_FRAC_START2_MODE0),                    0x0, 0xEA },
  { phy_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_0_QSERDES_COM_DIV_FRAC_START1_MODE0),                    0x0, 0xAB },
  { phy_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_0_QSERDES_COM_LOCK_CMP3_MODE0),                          0x0, 0x00 },
  { phy_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_0_QSERDES_COM_LOCK_CMP2_MODE0),                          0x0, 0x0D },
  { phy_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_0_QSERDES_COM_LOCK_CMP1_MODE0),                          0x0, 0x04 },
  { phy_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_0_QSERDES_COM_HSCLK_SEL),                                0x0, 0x00 },
  { phy_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_0_QSERDES_COM_CP_CTRL_MODE0),                            0x0, 0x06 },
  { phy_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_0_QSERDES_COM_PLL_RCTRL_MODE0),                          0x0, 0x16 },
  { phy_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_0_QSERDES_COM_PLL_CCTRL_MODE0),                          0x0, 0x36 },
  { phy_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_0_QSERDES_COM_CMN_MODE),                                 0x0, 0x01 },
  { phy_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_0_QSERDES_COM_CLK_SELECT),                               0x0, 0x33 },
  { phy_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_0_QSERDES_COM_SYS_CLK_CTRL),                             0x0, 0x02 },
  { phy_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_0_QSERDES_COM_SYSCLK_BUF_ENABLE),                        0x0, 0x06 },
  { phy_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_0_QSERDES_COM_SYSCLK_EN_SEL),                            0x0, 0x04 },
  { phy_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_0_QSERDES_COM_INTEGLOOP_GAIN1_MODE0),                    0x0, 0x00 },
  { phy_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_0_QSERDES_COM_INTEGLOOP_GAIN0_MODE0),                    0x0, 0x3F },
  { phy_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_0_QSERDES_COM_BG_TIMER),                                 0x0, 0x09 },
  /* SSC settings */
  { phy_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_0_QSERDES_COM_SSC_EN_CENTER),                            0x0, 0x01 },
  { phy_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_0_QSERDES_COM_SSC_PER1),                                 0x0, 0x40 },
  { phy_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_0_QSERDES_COM_SSC_PER2),                                 0x0, 0x01 },
  { phy_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_0_QSERDES_COM_SSC_ADJ_PER1),                             0x0, 0x02 },
  { phy_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_0_QSERDES_COM_SSC_ADJ_PER2),                             0x0, 0x00 },
  { phy_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_0_QSERDES_COM_SSC_STEP_SIZE1),                           0x0, 0x7E },
  { phy_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_0_QSERDES_COM_SSC_STEP_SIZE2),                           0x0, 0x15 },
  /* TX settings */
  { phy_tx_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_0_QSERDES_TX_RES_CODE_LANE_OFFSET_TX),                0x0, 0x02 },
  { phy_tx_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_0_QSERDES_TX_RCV_DETECT_LVL_2),                       0x0, 0x12 },
  { phy_tx_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_0_QSERDES_TX_HIGHZ_DRVR_EN),                          0x0, 0x10 },
  { phy_tx_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_0_QSERDES_TX_LANE_MODE_1),                            0x0, 0x06 },
  /* RX settings */
  { phy_rx_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_0_QSERDES_RX_SIGDET_CNTRL),                           0x0, 0x03 },
  { phy_rx_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_0_QSERDES_RX_SIGDET_ENABLES),                         0x0, 0x10 },
  { phy_rx_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_0_QSERDES_RX_SIGDET_DEGLITCH_CNTRL),                  0x0, 0x14 },
  { phy_rx_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_0_QSERDES_RX_RX_EQU_ADAPTOR_CNTRL2),                  0x0, 0x0E },
  { phy_rx_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_0_QSERDES_RX_RX_EQU_ADAPTOR_CNTRL3),                  0x0, 0x04 },
  { phy_rx_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_0_QSERDES_RX_RX_EQU_ADAPTOR_CNTRL4),                  0x0, 0x1A },
  { phy_rx_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_0_QSERDES_RX_UCDR_SO_SATURATION_AND_ENABLE),          0x0, 0x4B },
  { phy_rx_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_0_QSERDES_RX_UCDR_SO_GAIN),                           0x0, 0x04 },
  { phy_rx_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_0_QSERDES_RX_UCDR_SO_GAIN_HALF),                      0x0, 0x04 },
  { phy_rx_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_0_QSERDES_RX_RX_EQ_OFFSET_ADAPTOR_CNTRL1),            0x0, 0x71 },
  { phy_rx_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_0_QSERDES_RX_RX_MODE_00),                             0x0, 0x59 },
  { phy_rx_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_0_QSERDES_RX_RX_MODE_01),                             0x0, 0x59 },
  { phy_rx_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_0_QSERDES_RX_RX_OFFSET_ADAPTOR_CNTRL2),               0x0, 0x80 },
  { phy_rx_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_0_QSERDES_RX_RX_INTERFACE_MODE),                      0x0, 0x40 },
  { phy_rx_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_0_QSERDES_RX_UCDR_PI_CONTROLS),                       0x0, 0x71 },
  { phy_rx_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_0_QSERDES_RX_UCDR_FASTLOCK_COUNT_LOW),                0x0, 0x40 },
  /* EP REFCLK settings */
  { pcs_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_0_PCIE_USB3_UNI_PCS_ENDPOINT_REFCLK_DRIVE),              0x0, 0x04 },
  /* OSC Det settings */
  { phy_misc_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_0_PCIE_USB3_UNI_PCS_MISC_OSC_DTCT_CONFIG2),         0x0, 0x52 },
  { phy_misc_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_0_PCIE_USB3_UNI_PCS_MISC_OSC_DTCT_MODE2_CONFIG2),   0x0, 0x10 },
  { phy_misc_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_0_PCIE_USB3_UNI_PCS_MISC_OSC_DTCT_MODE2_CONFIG4),   0x0, 0x1A },
  { phy_misc_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_0_PCIE_USB3_UNI_PCS_MISC_OSC_DTCT_MODE2_CONFIG5),   0x0, 0x06 },
  /* FLL Config settings */
  { pcs_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_0_PCIE_USB3_UNI_PCS_FLL_CNTRL2),                         0x0, 0x83 },
  { pcs_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_0_PCIE_USB3_UNI_PCS_FLL_CNT_VAL_L),                      0x0, 0x09 },
  { pcs_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_0_PCIE_USB3_UNI_PCS_FLL_CNT_VAL_H_TOL),                  0x0, 0xA2 },
  { pcs_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_0_PCIE_USB3_UNI_PCS_FLL_MAN_CODE),                       0x0, 0x40 },
  { pcs_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_0_PCIE_USB3_UNI_PCS_FLL_CNTRL1),                         0x0, 0x02 },
  /* PCIe L1ss settings */
  { pcs_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_0_PCIE_USB3_UNI_PCS_OSC_DTCT_ACTIONS),                   0x0, 0x00 },
  { pcs_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_0_PCIE_USB3_UNI_PCS_PWRUP_RESET_DLY_TIME_AUXCLK),        0x0, 0x01 },
  { pcs_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_0_PCIE_USB3_UNI_PCS_L1SS_WAKEUP_DLY_TIME_AUXCLK_MSB),    0x0, 0x00 },
  { pcs_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_0_PCIE_USB3_UNI_PCS_L1SS_WAKEUP_DLY_TIME_AUXCLK_LSB),    0x0, 0x20 },
  { pcs_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_0_PCIE_USB3_UNI_PCS_LP_WAKEUP_DLY_TIME_AUXCLK_MSB),      0x0, 0x00 },
  { pcs_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_0_PCIE_USB3_UNI_PCS_LP_WAKEUP_DLY_TIME_AUXCLK),          0x0, 0x01 },
  { pcs_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_0_PCIE_USB3_UNI_PCS_PLL_LOCK_CHK_DLY_TIME),              0x0, 0x73 },
  /* PCS sigdet & RefGen settings */
  { pcs_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_0_PCIE_USB3_UNI_PCS_RX_SIGDET_LVL),                      0x0, 0xBB },
  { pcs_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_0_PCIE_USB3_UNI_PCS_SIGDET_CNTRL),                       0x0, 0x03 },
  { pcs_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_0_PCIE_USB3_UNI_PCS_REFGEN_REQ_CONFIG1),                 0x0, 0x0D },
  /* PCS Settings */
  { pcs_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_0_PCIE_USB3_UNI_PCS_POWER_STATE_CONFIG4),                0x0, 0x00 },
  { phy_misc_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_0_PCIE_USB3_UNI_PCS_MISC_PCIE_INT_AUX_CLK_CONFIG1), 0x0, 0x00 },
  { invalid_reg, PHY_REG_NOP, 0xFF, 0xFF, 0xFF },
};

/*******************************************************************************
 * Chipset specific version independent port 0 PHY power ON sequence
 ******************************************************************************/
STATIC CONST pciePhyRegSeq_t port_0_phy_power_on_seq[] =
{
  /* Register Base, Operation, Offset, Mask & Value */
  { pcs_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_0_PCIE_USB3_UNI_PCS_POWER_DOWN_CONTROL),                 0x0, 0x03 },
  { pcs_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_0_PCIE_USB3_UNI_PCS_SW_RESET),                           0x0, 0x00 },
  { pcs_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_0_PCIE_USB3_UNI_PCS_START_CONTROL),                      0x0, 0x03 },
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
  { invalid_reg, PHY_REG_DELAY, invalid_reg, 0x0, 1000 },
  { invalid_reg, PHY_REG_NOP, 0xFF, 0xFF, 0xFF },
};

/*******************************************************************************
 * poll PHY power ON status of port 0 PHY
 ******************************************************************************/
STATIC CONST pciePhyRegSeq_t port_0_phy_poll_seq[] =
{
  /* Register Base, Operation, Offset, Mask & Value */
  { pcs_reg, PHY_REG_REA, HWIO_OFFS(PCIE_0_PCIE_USB3_UNI_PCS_PCS_STATUS), HWIO_PCIE_0_PCIE_USB3_UNI_PCS_PCS_STATUS_PHYSTATUS_BMSK, FALSE},
  { invalid_reg, PHY_REG_NOP, 0xFF, 0xFF, 0xFF },
};

/*******************************************************************************
 * post PHY power ON sequence for port 0 PHY
 ******************************************************************************/
STATIC CONST pciePhyRegSeq_t port_0_phy_post_power_up_seq[] =
{
  /* Register Base, Operation, Offset, Mask & Value */
  /* Memory Enable Compliance */
  { dbi_reg,     PHY_REG_RMW, HWIO_OFFS(PCIE_0_TYPE1_STATUS_COMMAND_REG), HWIO_PCIE_0_TYPE1_STATUS_COMMAND_REG_MSE_BMSK, HWIO_PCIE_0_TYPE1_STATUS_COMMAND_REG_MSE_BMSK },
  /* Writing Slave address space size */
  { parf_reg,    PHY_REG_WRI, HWIO_OFFS(PCIE_0_PCIE20_PARF_SLV_ADDR_SPACE_SIZE), 0x0, PCIE_0_PCIE20_WRAPPER_AXI_BASE_SIZE},
  { parf_reg,    PHY_REG_WRI, HWIO_OFFS(PCIE_0_PCIE20_PARF_SLV_ADDR_SPACE_SIZE_HI), 0x0, 0x0},
  /* Clear REQ_NOT_ENTER_L1 field in PARF PM_CTRL to allow L1 entry */
  { parf_reg,    PHY_REG_RMW, HWIO_OFFS(PCIE_0_PCIE20_PARF_PM_CTRL), HWIO_PCIE_0_PCIE20_PARF_PM_CTRL_REQ_NOT_ENTR_L1_BMSK, 0x00000000},
  /* Enable DBI_RO_WR_EN to access CS1 region */
  { dbi_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_0_MISC_CONTROL_1_REG), 0x0, 0x1},
  /* Memory Barrier */
  { invalid_reg, PHY_REG_DSB, 0xFF, 0xFF, 0xFF},
  /* Update Base class and Sub class */
  { dbi_reg,     PHY_REG_RMW, HWIO_OFFS(PCIE_0_TYPE1_CLASS_CODE_REV_ID_REG),
    (HWIO_PCIE_0_TYPE1_CLASS_CODE_REV_ID_REG_BASE_CLASS_CODE_BMSK | HWIO_PCIE_0_TYPE1_CLASS_CODE_REV_ID_REG_SUBCLASS_CODE_BMSK),
    (PCIE_CLASS_CODE_BRIDGE_DEVICE | PCIE_SUBCLASS_CODE_PCI_2_PCI_BRIDGE)},
  /* Disable DBI_RO_WR_EN to access CS1 region */
  { dbi_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_0_MISC_CONTROL_1_REG), 0x0, 0x0},
  /* Memory Barrier */
  { invalid_reg, PHY_REG_DSB, 0xFF, 0xFF, 0xFF},
  /* Assert CS2 */
  { elbi_reg,    PHY_REG_WRI, HWIO_OFFS(PCIE_0_PCIE20_ELBI_CS2_ENABLE), 0x0, 0x1},
  /* Memory Barrier */
  { invalid_reg, PHY_REG_DSB, 0xFF, 0xFF, 0xFF},
  /* Disable BAR0 and BAR1 */
  { dbi_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_0_BAR0_REG), 0x0, 0x0},
  { dbi_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_0_BAR1_REG), 0x0, 0x0},
  /* De-Assert CS2 */
  { elbi_reg,    PHY_REG_WRI, HWIO_OFFS(PCIE_0_PCIE20_ELBI_CS2_ENABLE), 0x0, 0x0},
  /* Memory Barrier */
  { invalid_reg, PHY_REG_DSB, 0xFF, 0xFF, 0xFF},
  /* -3.5 dB Selective Deemphasis */
  { dbi_reg, PHY_REG_RMW, HWIO_OFFS(PCIE_0_LINK_CONTROL2_LINK_STATUS2_REG), HWIO_PCIE_0_LINK_CONTROL2_LINK_STATUS2_REG_PCIE_CAP_SEL_DEEMPHASIS_BMSK, HWIO_PCIE_0_LINK_CONTROL2_LINK_STATUS2_REG_PCIE_CAP_SEL_DEEMPHASIS_BMSK},
  { invalid_reg, PHY_REG_NOP, 0xFF, 0xFF, 0xFF },
};

/*******************************************************************************
 * Chipset specific complete port PHY init sequence
 ******************************************************************************/
STATIC CONST pciePhyAllVerSeq_t sdm845_port_0_phy_init_seq[] =
{
  /* First write the register sequence corresponds to version 1 */
  {Pcie_Qcom_SoC_Ver_1,    v1_port_0_phy_init_seq},
  /* Then write the register sequence corresponds to version 2 if any */
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
  { pcs_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_0_PCIE_USB3_UNI_PCS_POWER_DOWN_CONTROL), 0x0, 0x00 },
  { pcs_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_0_PCIE_USB3_UNI_PCS_START_CONTROL),      0x0, 0x00 },
  { invalid_reg, PHY_REG_NOP, 0xFF, 0xFF, 0xFF },
};
#endif /* (RPMASK & PCIE_RP(0)) - root port 0 enabled */

/*******************************************************************************
 * This section will hold the port info data for root port 1
 ******************************************************************************/
#if (RPMASK & PCIE_RP(1))

/* Number of PCIe lanes present in RP0 */
#define NUM_PCIE_1_LANES 0x01

/*******************************************************************************
 * Register region information for PCIe GEN3 root port 0
 ******************************************************************************/
static memRegions_t port_1_reg[max_mem_reg] =
{
  /**< dbi_reg: Access DBI register space */
  {.pa=PCIE_GEN3_PCIE20_DBI_REG_BASE, .sz=PCIE_GEN3_PCIE20_DBI_REG_SIZE},
  /**< elbi_reg: Access ELBI register space */
  {.pa=PCIE_GEN3_PCIE20_ELBI_REG_BASE, .sz=PCIE_GEN3_PCIE20_ELBI_REG_SIZE},
  /**< parf_reg: Access PARF register space */
  {.pa=PCIE_GEN3_PCIE20_PARF_REG_BASE, .sz=PCIE_GEN3_PCIE20_PARF_REG_SIZE},
  /**< pcs_reg: Access PHY PCS register space */
  {.pa=PCIE_GEN3_HP_PCIE_PHY_HP_PCIE_PCS_REG_BASE, .sz=PCIE_GEN3_HP_PCIE_PHY_HP_PCIE_PCS_REG_SIZE},
  /**< phy_reg: Access PHY register space */
  {.pa=PCIE_GEN3_QHP_COM_HP_PCIE_COM_REG_BASE, .sz=PCIE_GEN3_QHP_COM_HP_PCIE_COM_REG_SIZE},
  /**< phy_rx_reg: Access Rx register space of PHY */
  {.pa=PCIE_INVALID_REG_BASE, .sz=PCIE_INVALID_REG_SIZE},
  /**< phy_tx_reg=6: Access Tx register space of PHY */
  {.pa=PCIE_INVALID_REG_BASE, .sz=PCIE_INVALID_REG_SIZE},
  /**< phy_misc_reg: Access Misc PCS register space of PHY */
  {.pa=PCIE_INVALID_REG_BASE, .sz=PCIE_INVALID_REG_SIZE},
  /**< qhp_l0_reg: Access QHP L0 register space */
  {.pa=PCIE_GEN3_QHP_L0_HP_PCIE_LANE_REG_BASE, .sz=PCIE_GEN3_QHP_L0_HP_PCIE_LANE_REG_SIZE},
  /**< qhp_l1_reg: Access QHP L1 register space */
  {.pa=PCIE_GEN3_QHP_L1_HP_PCIE_LANE_REG_BASE, .sz=PCIE_GEN3_QHP_L1_HP_PCIE_LANE_REG_SIZE},
  /**< axi_reg: Access AXI register space */
  {.pa=PCIE_GEN3_PCIE_WRAPPER_AXI_BASE, .sz=PCIE_GEN3_PCIE_WRAPPER_AXI_BASE_SIZE},
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
  "gcc_pcie_phy_refgen_clk",
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
      .uIb = ((100ULL) << 20), /**< Instantaneous bandwidth in Bytes/second */
      .uAb = ((100ULL) << 20), /**< Arbitrated bandwidth in Bytes/second */
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
 * PCIe root port 1 aux clock settings
 ******************************************************************************/
CONST clkFreqSettings_t pcie_1_clk_settings[] =
{
  {.freq = 19200000,  .name = "gcc_pcie_1_aux_clk"}, 
  {.freq = 100000000, .name = "gcc_pcie_phy_refgen_clk"}, 
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
    .TlmmGpioParam.GpioNum = 103, // CLK_REQ
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
    .TlmmGpioParam.GpioNum = 102, // PCIE0_RST
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
    .TlmmGpioParam.GpioNum = 104, // PCIE0_WAKE
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
    .DelayValue = 10000 // 10 ms delay for EP device preparation for Link Training
   }
  }
  ,
  {
   .type = MSM_GPIO,
   {
    .TlmmGpioParam.GpioNum = 102, // PCIE0_RST
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
STATIC pcieLink_t port1_link[] =
{
  {
    .trained   = FALSE,
    .num_lanes = NUM_PCIE_1_LANES,
    .aspm      = aspm_l1_ena_l0s_ena
  }
};

/*******************************************************************************
 * V1 Chipset specific port 1 PHY init sequence
 ******************************************************************************/
STATIC CONST pciePhyRegSeq_t v1_port_1_phy_init_seq[] =
{
  /* Register Base, Operation, Offset, Mask & Value */
  /* Set Device Type */
  { parf_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_0_PCIE20_PARF_DEVICE_TYPE),            0x0, 0x04 },
  /* Power up init sequence for RootComplex configuration  */
  { pcs_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_HP_PCIE_PHY_POWER_DOWN_CONTROL),   0x0, 0x03 },
  { phy_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_COM_SYSCLK_EN_SEL),            0x0, 0x27 },
  { phy_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_COM_SSC_EN_CENTER),            0x0, 0x01 },// Enable downspread SSC
  { phy_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_COM_SSC_PER1),                 0x0, 0x31 },// Set SSC frequency to 31.5KHz
  { phy_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_COM_SSC_PER2),                 0x0, 0x01 },//
  { phy_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_COM_SSC_STEP_SIZE1),           0x0, 0xDE },// Set SSC step size for Gen1/2 (4500ppm)
  { phy_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_COM_SSC_STEP_SIZE2),           0x0, 0x07 },//
  { phy_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_COM_SSC_STEP_SIZE1_MODE1),     0x0, 0x4C },// Set SSC step size for Gen3 (4500ppm)
  { phy_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_COM_SSC_STEP_SIZE2_MODE1),     0x0, 0x06 },//
  { phy_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_COM_BIAS_EN_CLKBUFLR_EN),      0x0, 0x18 },// Enable internal clock buffers and PCS control of endpoint clock drive
  { phy_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_COM_CLK_ENABLE1),              0x0, 0xB0 },// Enable endpoint clock output with pulldown when disabled
  { phy_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_COM_LOCK_CMP1_MODE0),          0x0, 0x8C },// Set PLL lock compare values
  { phy_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_COM_LOCK_CMP2_MODE0),          0x0, 0x20 },// Set PLL lock compare values
  { phy_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_COM_LOCK_CMP1_MODE1),          0x0, 0x14 },// Set PLL lock compare values
  { phy_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_COM_LOCK_CMP2_MODE1),          0x0, 0x34 },// Set PLL lock compare values
  { phy_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_COM_CP_CTRL_MODE0),            0x0, 0x06 },// Set mode0 charge pump control value
  { phy_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_COM_CP_CTRL_MODE1),            0x0, 0x06 },// Set mode1 charge pump control value
  { phy_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_COM_PLL_RCTRL_MODE0),          0x0, 0x16 },// Set mode0 PLL RC loop filter value
  { phy_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_COM_PLL_RCTRL_MODE1),          0x0, 0x16 },// Set mode1 PLL RC loop filter value
  { phy_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_COM_PLL_CCTRL_MODE0),          0x0, 0x36 },// Set mode0 2nd cap of loop filter value
  { phy_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_COM_PLL_CCTRL_MODE1),          0x0, 0x36 },// Set mode1 2nd cap of loop filter value
  { phy_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_COM_RESTRIM_CTRL2),            0x0, 0x05 },// Add offset to lane restrim code
  { phy_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_COM_LOCK_CMP_EN),              0x0, 0x42 },// Shorten PLL lock count from 1024 sysclk cycles to 512
  { phy_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_COM_DEC_START_MODE0),          0x0, 0x82 },// Set PLL mode0 divider value
  { phy_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_COM_DEC_START_MODE1),          0x0, 0x68 },// Set PLL mode1 divider value
  { phy_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_COM_DIV_FRAC_START1_MODE0),    0x0, 0x55 },// Set PLL mode0 fractional divider value
  { phy_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_COM_DIV_FRAC_START2_MODE0),    0x0, 0x55 },
  { phy_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_COM_DIV_FRAC_START3_MODE0),    0x0, 0x03 },
  { phy_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_COM_DIV_FRAC_START1_MODE1),    0x0, 0xAB },// Set PLL mode0 fractional divider value
  { phy_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_COM_DIV_FRAC_START2_MODE1),    0x0, 0xAA },//
  { phy_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_COM_DIV_FRAC_START3_MODE1),    0x0, 0x02 },//
  { phy_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_COM_INTEGLOOP_GAIN0_MODE0),    0x0, 0x3F },// Set mode0 integral loop gain
  { phy_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_COM_INTEGLOOP_GAIN0_MODE1),    0x0, 0x3F },// Set mode1 integral loop gain
  { phy_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_COM_VCO_TUNE_MAP),             0x0, 0x10 },// Set mode mapping: mode0 - gen1 & gen2, mode1 - gen3
  { phy_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_COM_CLK_SELECT),               0x0, 0x04 },// Select sysclk (19.2MHz) as endpoint clock source during rate change
  { phy_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_COM_HSCLK_SEL1),               0x0, 0x30 },// VCO output clock: choose div2 for gen1/2 and div1 for gen3
  { phy_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_COM_CORECLK_DIV),              0x0, 0x04 },// Set coreclk_out to div4 for mode0
  { phy_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_COM_CORE_CLK_EN),              0x0, 0x73 },// Set PLL divider for each mode and enable coreauxclk
  { phy_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_COM_CMN_CONFIG),               0x0, 0x1C },// Enable switch to coreclk and auxclk; indicate all lanes are synchronous
  { phy_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_COM_SVS_MODE_CLK_SEL),         0x0, 0x15 },// Change digclk divide for gen1 SVS support
  { phy_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_COM_CORECLK_DIV_MODE1),        0x0, 0x04 },// Set coreclk_out to div4 for mode1
  { phy_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_COM_CMN_MODE),                 0x0, 0x01 },// Disable DDR refclk buffer
  { phy_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_COM_VREGCLK_DIV1),             0x0, 0x22 },// Set vregclk divide ratio for gen1/2 & gen3
  { phy_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_COM_VREGCLK_DIV2),             0x0, 0x00 },// Set vregclk divide ratio
  { phy_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_COM_BGV_TRIM),                 0x0, 0x20 },// Set vregclk divide ratio
  { phy_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_COM_BG_CTRL),                  0x0, 0x07 },// Set vregclk divide ratio
  { qhp_l0_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L0_DRVR_CTRL0),             0x0, 0x00 },// Disable initial loading of txdriver margining
  { qhp_l0_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L0_DRVR_TAP_EN),            0x0, 0x0D },// Enable pre and post taps
  { qhp_l0_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L0_TX_BAND_MODE),           0x0, 0x01 },// Set txband for each mode
  { qhp_l0_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L0_LANE_MODE),              0x0, 0x3A },// Set lanes as synchronous; invert tclk, rclk and cdr clock
  { qhp_l0_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L0_PARALLEL_RATE),          0x0, 0x2F },// Set data width to 20 bits for gen1/2 and 16 bits for gen3
  { qhp_l0_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L0_CML_CTRL_MODE0),         0x0, 0x09 },// Set Gen1 cml clock buffer and cml2cmos current
  { qhp_l0_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L0_CML_CTRL_MODE1),         0x0, 0x09 },// Set Gen2 cml clock buffer and cml2cmos current
  { qhp_l0_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L0_CML_CTRL_MODE2),         0x0, 0x1B },// Set Gen3 cml clock buffer and cml2cmos current
  { qhp_l0_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L0_PREAMP_CTRL_MODE1),      0x0, 0x01 },// Set Gen2 receiver preamp current
  { qhp_l0_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L0_PREAMP_CTRL_MODE2),      0x0, 0x07 },// Set Gen3 receiver preamp current
  { qhp_l0_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L0_MIXER_CTRL_MODE0),       0x0, 0x31 },// Set Gen1 mixer resistor load and current
  { qhp_l0_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L0_MIXER_CTRL_MODE1),       0x0, 0x31 },// Set Gen2 mixer resistor load and current
  { qhp_l0_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L0_MIXER_CTRL_MODE2),       0x0, 0x03 },// Set Gen3 mixer resistor load and current
  { qhp_l0_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L0_CTLE_THRESH_DFE),        0x0, 0x02 },// Configure RxEq
  { qhp_l0_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L0_VGA_THRESH_DFE),         0x0, 0x00 },// Configure RxEq
  { qhp_l0_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L0_RXENGINE_EN0),           0x0, 0x12 },// Configure RxEq
  { qhp_l0_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L0_CTLE_TRAIN_TIME),        0x0, 0x25 },// Configure RxEq
  { qhp_l0_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L0_CTLE_DFE_OVRLP_TIME),    0x0, 0x00 },// Configure RxEq
  { qhp_l0_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L0_DFE_REFRESH_TIME),       0x0, 0x05 },// Configure RxEq
  { qhp_l0_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L0_DFE_ENABLE_TIME),        0x0, 0x01 },// Configure RxEq
  { qhp_l0_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L0_VGA_GAIN),               0x0, 0x26 },// Configure RxEq
  { qhp_l0_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L0_DFE_GAIN),               0x0, 0x12 },// Configure RxEq
  { qhp_l0_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L0_EQ_GAIN),                0x0, 0x04 },// Configure RxEq
  { qhp_l0_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L0_OFFSET_GAIN),            0x0, 0x04 },// Configure RxEq
  { qhp_l0_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L0_PRE_GAIN),               0x0, 0x09 },// Configure RxEq
  { qhp_l0_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L0_EQ_INITVAL),             0x0, 0x15 },// Configure RxEq (Gen1/2 Eq value & Gen3 starting value)
  { qhp_l0_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L0_EDAC_INITVAL),           0x0, 0x28 },// Use same value for long & short channel
  { qhp_l0_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L0_RXEQ_INITB0),            0x0, 0x7F },// Configure RxEq
  { qhp_l0_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L0_RXEQ_INITB1),            0x0, 0x07 },// Configure RxEq
  { qhp_l0_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L0_RCVRDONE_THRESH1),       0x0, 0x04 },// Configure RxEq
  { qhp_l0_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L0_RXEQ_CTRL),              0x0, 0x70 },// Configure RxEq
  { qhp_l0_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L0_UCDR_FO_GAIN_MODE0),     0x0, 0x8B },// Configure UCDR
  { qhp_l0_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L0_UCDR_FO_GAIN_MODE1),     0x0, 0x08 },// Configure UCDR
  { qhp_l0_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L0_UCDR_FO_GAIN_MODE2),     0x0, 0x0A },// Configure UCDR
  { qhp_l0_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L0_UCDR_SO_GAIN_MODE0),     0x0, 0x03 },// Configure UCDR
  { qhp_l0_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L0_UCDR_SO_GAIN_MODE1),     0x0, 0x04 },// Configure UCDR
  { qhp_l0_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L0_UCDR_SO_GAIN_MODE2),     0x0, 0x04 },// Configure UCDR
  { qhp_l0_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L0_UCDR_SO_CONFIG),         0x0, 0x0C },// Configure UCDR
  { qhp_l0_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L0_RX_BAND),                0x0, 0x02 },// Set rxband for each mode
  { qhp_l0_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L0_RX_RCVR_PATH1_MODE0),    0x0, 0x5C },// Config Rx path for mode0
  { qhp_l0_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L0_RX_RCVR_PATH1_MODE1),    0x0, 0x3E },// Config Rx path for mode1
  { qhp_l0_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L0_RX_RCVR_PATH1_MODE2),    0x0, 0x3F },// Config Rx path for mode2
  { qhp_l0_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L0_SIGDET_ENABLES),         0x0, 0x01 }, // Enable sigdet LP mode in PS0,1,2; bypass sigdet pulse width filter
  { qhp_l0_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L0_SIGDET_CNTRL),           0x0, 0xA0 },// Set sigdet level
  { qhp_l0_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L0_SIGDET_DEGLITCH_CNTRL),  0x0, 0x08 },// Set sigdet glitch filter bandwidth
  { qhp_l0_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L0_DCC_GAIN),               0x0, 0x01 },// Set duty cycle correction gain
  { qhp_l0_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L0_RX_EN_SIGNAL),           0x0, 0xC3 },// Disable synching of lane rclks using c_rxsync_clk
  { qhp_l0_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L0_PSM_RX_EN_CAL),          0x0, 0x00 },// Disable optional sampler cal
  { qhp_l0_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L0_RX_MISC_CNTRL0),         0x0, 0xBC },// Configure RxEq
  { qhp_l0_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L0_TS0_TIMER),              0x0, 0x7F },// Set TS0A DLL wait timer to max value
  { qhp_l0_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L0_DLL_HIGHDATARATE),       0x0, 0x15 },// Set high data-rate for gen1, gen2 & gen3
  { qhp_l0_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L0_DRVR_CTRL1),             0x0, 0x0C },// Add offset to transmitter restrim code
  { qhp_l0_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L0_DRVR_CTRL2),             0x0, 0x00 },// Napali V1: 0x00, Napali V2 & Carrera: 0x0F
  { qhp_l0_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L0_RX_RESECODE_OFFSET),     0x0, 0x04 },// Add offset to receiver restrim code
  { qhp_l0_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L0_VGA_INITVAL),            0x0, 0x20 },// Configure RxEq (Gen1/2 VGA value & Gen3 starting value)
  { qhp_l1_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L1_DRVR_CTRL0),             0x0, 0x00 },// Disable initial loading of txdriver margining
  { qhp_l1_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L1_DRVR_TAP_EN),            0x0, 0x0D },// Enable pre and post taps
  { qhp_l1_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L1_TX_BAND_MODE),           0x0, 0x01 },// Set txband for each mode
  { qhp_l1_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L1_LANE_MODE),              0x0, 0x3A },// Set lanes as synchronous; invert tclk, rclk and cdr clock
  { qhp_l1_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L1_PARALLEL_RATE),          0x0, 0x2F },// Set data width to 20 bits for gen1/2 and 16 bits for gen3
  { qhp_l1_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L1_CML_CTRL_MODE0),         0x0, 0x09 },// Set Gen1 cml clock buffer and cml2cmos current
  { qhp_l1_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L1_CML_CTRL_MODE1),         0x0, 0x09 },// Set Gen2 cml clock buffer and cml2cmos current
  { qhp_l1_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L1_CML_CTRL_MODE2),         0x0, 0x1B },// Set Gen3 cml clock buffer and cml2cmos current
  { qhp_l1_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L1_PREAMP_CTRL_MODE1),      0x0, 0x01 },// Set Gen2 receiver preamp current
  { qhp_l1_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L1_PREAMP_CTRL_MODE2),      0x0, 0x07 },// Set Gen3 receiver preamp current
  { qhp_l1_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L1_MIXER_CTRL_MODE0),       0x0, 0x31 },// Set Gen1 mixer resistor load and current
  { qhp_l1_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L1_MIXER_CTRL_MODE1),       0x0, 0x31 },// Set Gen2 mixer resistor load and current
  { qhp_l1_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L1_MIXER_CTRL_MODE2),       0x0, 0x03 },// Set Gen3 mixer resistor load and current
  { qhp_l1_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L1_CTLE_THRESH_DFE),        0x0, 0x02 },// Configure RxEq
  { qhp_l1_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L1_VGA_THRESH_DFE),         0x0, 0x00 },// Configure RxEq
  { qhp_l1_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L1_RXENGINE_EN0),           0x0, 0x12 },// Configure RxEq
  { qhp_l1_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L1_CTLE_TRAIN_TIME),        0x0, 0x25 },// Configure RxEq
  { qhp_l1_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L1_CTLE_DFE_OVRLP_TIME),    0x0, 0x00 },// Configure RxEq
  { qhp_l1_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L1_DFE_REFRESH_TIME),       0x0, 0x05 },// Configure RxEq
  { qhp_l1_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L1_DFE_ENABLE_TIME),        0x0, 0x01 },// Configure RxEq
  { qhp_l1_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L1_VGA_GAIN),               0x0, 0x26 },// Configure RxEq
  { qhp_l1_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L1_DFE_GAIN),               0x0, 0x12 },// Configure RxEq
  { qhp_l1_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L1_EQ_GAIN),                0x0, 0x04 },// Configure RxEq
  { qhp_l1_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L1_OFFSET_GAIN),            0x0, 0x04 },// Configure RxEq
  { qhp_l1_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L1_PRE_GAIN),               0x0, 0x09 },// Configure RxEq
  { qhp_l1_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L1_EQ_INITVAL),             0x0, 0x15 },// Configure RxEq (Gen1/2 Eq value & Gen3 starting value)
  { qhp_l1_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L1_EDAC_INITVAL),           0x0, 0x28 },// Use same value for short & long channel
  { qhp_l1_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L1_RXEQ_INITB0),            0x0, 0x7F },// Configure RxEq
  { qhp_l1_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L1_RXEQ_INITB1),            0x0, 0x07 },// Configure RxEq
  { qhp_l1_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L1_RCVRDONE_THRESH1),       0x0, 0x04 },// Configure RxEq
  { qhp_l1_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L1_RXEQ_CTRL),              0x0, 0x70 },// Configure RxEq
  { qhp_l1_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L1_UCDR_FO_GAIN_MODE0),     0x0, 0x8B },// Configure UCDR
  { qhp_l1_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L1_UCDR_FO_GAIN_MODE1),     0x0, 0x08 },// Configure UCDR
  { qhp_l1_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L1_UCDR_FO_GAIN_MODE2),     0x0, 0x0A },// Configure UCDR
  { qhp_l1_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L1_UCDR_SO_GAIN_MODE0),     0x0, 0x03 },// Configure UCDR
  { qhp_l1_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L1_UCDR_SO_GAIN_MODE1),     0x0, 0x04 },// Configure UCDR
  { qhp_l1_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L1_UCDR_SO_GAIN_MODE2),     0x0, 0x04 },// Configure UCDR
  { qhp_l1_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L1_UCDR_SO_CONFIG),         0x0, 0x0C },// Configure UCDR
  { qhp_l1_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L1_RX_BAND),                0x0, 0x02 },// Set rxband for each mode
  { qhp_l1_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L1_RX_RCVR_PATH1_MODE0),    0x0, 0x5C },// Config Rx path for mode0
  { qhp_l1_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L1_RX_RCVR_PATH1_MODE1),    0x0, 0x3E },// Config Rx path for mode1
  { qhp_l1_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L1_RX_RCVR_PATH1_MODE2),    0x0, 0x3F },// Config Rx path for mode2
  { qhp_l1_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L1_SIGDET_ENABLES),         0x0, 0x01 },// Enable sigdet LP mode in PS0,1,2; bypass sigdet pulse width filter
  { qhp_l1_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L1_SIGDET_CNTRL),           0x0, 0xA0 },// Set sigdet level
  { qhp_l1_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L1_SIGDET_DEGLITCH_CNTRL),  0x0, 0x08 },// Set sigdet glitch filter bandwidth
  { qhp_l1_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L1_DCC_GAIN),               0x0, 0x01 },// Set duty cycle correction gain
  { qhp_l1_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L1_RX_EN_SIGNAL),           0x0, 0xC3 },// Disable synching of lane rclks using c_rxsync_clk
  { qhp_l1_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L1_PSM_RX_EN_CAL),          0x0, 0x00 },// Disable optional sampler cal
  { qhp_l1_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L1_RX_MISC_CNTRL0),         0x0, 0xBC },// Configure RxEq
  { qhp_l1_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L1_TS0_TIMER),              0x0, 0x7F },// Set TS0A DLL wait timer to max value
  { qhp_l1_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L1_DLL_HIGHDATARATE),       0x0, 0x15 },// Set high data-rate for gen1, gen2 & gen3
  { qhp_l1_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L1_DRVR_CTRL1),             0x0, 0x0C },// Add offset to transmitter restrim code
  { qhp_l1_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L1_DRVR_CTRL2),             0x0, 0x00 },// Napali V1: 0x00, Napali V2 & Carrera: 0x0F
  { qhp_l1_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L1_RX_RESECODE_OFFSET),     0x0, 0x04 },// Add offset to receiver restrim code
  { qhp_l1_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L1_VGA_INITVAL),            0x0, 0x20 },// Configure RxEq (Gen1/2 VGA value & Gen3 starting value)
  { pcs_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_HP_PCIE_PHY_POWER_STATE_CONFIG),   0x0, 0x3F },// Disable asynchronous capture of rxelecidle_disable
  { pcs_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_HP_PCIE_PHY_PCS_TX_RX_CONFIG),     0x0, 0x58 },// Napali V1: 0x58; Napali V2: 0x50; Carrera: 0x50 Increase depth of pipe interface RTBs in G1/G2 for Napali V1 to account for large pclk latency variation
  { pcs_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_HP_PCIE_PHY_TXMGN_MAIN_V0_M3P5DB), 0x0, 0x19 },// Adjustment to improve eye height
  { pcs_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_HP_PCIE_PHY_TXMGN_POST_V0_M3P5DB), 0x0, 0x07 },// Adjustment to improve eye height
  { pcs_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_HP_PCIE_PHY_TXMGN_MAIN_V0_M6DB),   0x0, 0x17 },// Adjustment to improve eye height
  { pcs_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_HP_PCIE_PHY_TXMGN_POST_V0_M6DB),   0x0, 0x09 },// Adjustment to improve eye height
  { pcs_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_HP_PCIE_PHY_POWER_STATE_CONFIG5),  0x0, 0x9F },// Disable L1ss pre-masking
  { invalid_reg, PHY_REG_NOP, 0xFF, 0xFF, 0xFF },
};

/*******************************************************************************
 * V2 Chipset specific port 1 PHY init sequence
 ******************************************************************************/
STATIC CONST pciePhyRegSeq_t v2_port_1_phy_init_seq[] =
{
  /* Register Base, Operation, Offset, Mask & Value */
  { qhp_l0_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L0_DRVR_CTRL2),         0x0, 0x0F },// Napali V1: 0x00, Napali V2 & Carrera: 0x0F
  { qhp_l1_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L1_DRVR_CTRL2),         0x0, 0x0F },// Napali V1: 0x00, Napali V2 & Carrera: 0x0F
  { pcs_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_HP_PCIE_PHY_PCS_TX_RX_CONFIG), 0x0, 0x50 },// Napali V1: 0x58; Napali V2: 0x50; Carrera: 0x50 Increase depth of pipe interface RTBs in G1/G2 for Napali V1 to account for large pclk latency variation
};
/*******************************************************************************
 * Chipset specific version independent port 1 PHY power ON sequence
 ******************************************************************************/
STATIC CONST pciePhyRegSeq_t port_1_phy_power_on_seq[] =
{
  /* Register Base, Operation, Offset, Mask & Value */
  { pcs_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_HP_PCIE_PHY_SW_RESET),      0x0, 0x00 },// Release software reset
  { qhp_l0_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L0_RSM_START),       0x0, 0x01 },// Start reset power SM
  { qhp_l1_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L1_RSM_START),       0x0, 0x01 },// Start reset power SM
  { pcs_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_HP_PCIE_PHY_START_CONTROL), 0x0, 0x01 },// Start PCS power SM 
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
  { invalid_reg, PHY_REG_DELAY, invalid_reg, 0x0, 2000 },
  { invalid_reg, PHY_REG_NOP, 0xFF, 0xFF, 0xFF },
};

/*******************************************************************************
 * poll PHY power ON status of port 1 PHY
 ******************************************************************************/
STATIC CONST pciePhyRegSeq_t port_1_phy_poll_seq[] =
{
  /* Register Base, Operation, Offset, Mask & Value */
  { pcs_reg, PHY_REG_REA, HWIO_OFFS(PCIE_GEN3_HP_PCIE_PHY_PCS_STATUS), HWIO_PCIE_GEN3_HP_PCIE_PHY_PCS_STATUS_PHYSTATUS_BMSK, FALSE },
  { invalid_reg, PHY_REG_NOP, 0xFF, 0xFF, 0xFF },
};

/*******************************************************************************
 * post PHY power ON sequence for port 1 GEN3 PHY
 ******************************************************************************/
STATIC CONST pciePhyRegSeq_t port_1_phy_post_power_up_seq[] =
{
  /* Register Base, Operation, Offset, Mask & Value */
  /* Memory Enable Compliance */
  { dbi_reg,     PHY_REG_RMW, HWIO_OFFS(PCIE_GEN3_TYPE1_STATUS_COMMAND_REG), HWIO_PCIE_GEN3_TYPE1_STATUS_COMMAND_REG_MSE_BMSK, HWIO_PCIE_GEN3_TYPE1_STATUS_COMMAND_REG_MSE_BMSK },
  /* Writing Slave address space size */
  { parf_reg,    PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_PCIE20_PARF_SLV_ADDR_SPACE_SIZE), 0x0, 0x20000000},
  { parf_reg,    PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_PCIE20_PARF_SLV_ADDR_SPACE_SIZE_HI), 0x0, 0x0},
  /* Clear REQ_NOT_ENTER_L1 field in PARF PM_CTRL to allow L1 entry */
  { parf_reg,    PHY_REG_RMW, HWIO_OFFS(PCIE_GEN3_PCIE20_PARF_PM_CTRL), HWIO_PCIE_GEN3_PCIE20_PARF_PM_CTRL_REQ_NOT_ENTR_L1_BMSK, 0x00000000},
  /* Enable DBI_RO_WR_EN to access CS1 region */
  { dbi_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_MISC_CONTROL_1_REG), 0x0, 0x1},
  /* Memory Barrier */
  { invalid_reg, PHY_REG_DSB, 0xFF, 0xFF, 0xFF},
  /* Update Base class and Sub class */
  { dbi_reg,     PHY_REG_RMW, HWIO_OFFS(PCIE_GEN3_TYPE1_CLASS_CODE_REV_ID_REG),
    (HWIO_PCIE_GEN3_TYPE1_CLASS_CODE_REV_ID_REG_BASE_CLASS_CODE_BMSK | HWIO_PCIE_GEN3_TYPE1_CLASS_CODE_REV_ID_REG_SUBCLASS_CODE_BMSK),
    (PCIE_CLASS_CODE_BRIDGE_DEVICE | PCIE_SUBCLASS_CODE_PCI_2_PCI_BRIDGE)},
  /* Disable hot plug capability bit in slot capabilities register */
  { dbi_reg,     PHY_REG_RMW, HWIO_OFFS(PCIE_GEN3_SLOT_CAPABILITIES_REG),
    HWIO_PCIE_GEN3_SLOT_CAPABILITIES_REG_PCIE_CAP_HOT_PLUG_CAPABLE_BMSK,
    0x00000000},
  /* Disable DBI_RO_WR_EN to access CS1 region */
  { dbi_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_MISC_CONTROL_1_REG), 0x0, 0x0},
  /* Memory Barrier */
  { invalid_reg, PHY_REG_DSB, 0xFF, 0xFF, 0xFF},
  /* Assert CS2 */
  { elbi_reg,    PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_PCIE20_ELBI_CS2_ENABLE), 0x0, 0x1},
  /* Memory Barrier */
  { invalid_reg, PHY_REG_DSB, 0xFF, 0xFF, 0xFF},
  /* Disable BAR0 and BAR1 */
  { dbi_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_BAR0_REG), 0x0, 0x0},
  { dbi_reg,     PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_BAR1_REG), 0x0, 0x0},
  /* De-Assert CS2 */
  { elbi_reg,    PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_PCIE20_ELBI_CS2_ENABLE), 0x0, 0x0},
  /* Memory Barrier */
  { invalid_reg, PHY_REG_DSB, 0xFF, 0xFF, 0xFF},
  { dbi_reg, PHY_REG_RMW, HWIO_OFFS(PCIE_GEN3_TYPE0_GEN2_CTRL_REG), 0x1f00UL, (0x1 << HWIO_PCIE_GEN3_TYPE0_GEN2_CTRL_REG_NUM_OF_LANES_SHFT) },
  { dbi_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_GEN3_EQ_FB_MODE_DIR_CHANGE_REG), 0x0, 0x155A0UL },
  { dbi_reg, PHY_REG_RMW, HWIO_OFFS(PCIE_GEN3_TYPE0_GEN3_EQ_CONTROL_REG), (0x1UL << 4), 0x0 },
  { dbi_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_MISC_CONTROL_1_REG), 0x0, 0x01 },
  { dbi_reg, PHY_REG_WRI, 0x154, 0x0, 0x77777777 /* pcie preset */ },
  { dbi_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_MISC_CONTROL_1_REG), 0x0, 0x00 },
  { dbi_reg, PHY_REG_RMW, HWIO_OFFS(PCIE_GEN3_TYPE0_PORT_LINK_CTRL_REG),
    HWIO_PCIE_GEN3_TYPE0_PORT_LINK_CTRL_REG_LINK_CAPABLE_BMSK,
    (0x3 << HWIO_PCIE_GEN3_TYPE0_PORT_LINK_CTRL_REG_LINK_CAPABLE_SHFT) },
  { dbi_reg, PHY_REG_RMW, HWIO_OFFS(PCIE_GEN3_TYPE0_PORT_LINK_CTRL_REG),
    HWIO_PCIE_GEN3_TYPE0_PORT_LINK_CTRL_REG_LINK_RATE_BMSK,
    (0x3 << HWIO_PCIE_GEN3_TYPE0_PORT_LINK_CTRL_REG_LINK_RATE_SHFT) },
  
  { invalid_reg, PHY_REG_NOP, 0xFF, 0xFF, 0xFF },
};

/*******************************************************************************
 * Chipset specific complete port 1 PHY init sequence
 ******************************************************************************/
STATIC CONST pciePhyAllVerSeq_t sdm845_port_1_phy_init_seq[] =
{
  /* First write the register sequence corresponds to version 1 */
  {Pcie_Qcom_SoC_Ver_1,    v1_port_1_phy_init_seq},
  /* Then write the register sequence corresponds to version 2 if any */
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
  { pcs_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_HP_PCIE_PHY_POWER_DOWN_CONTROL), 0x0, 0x00 },
  { qhp_l0_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L0_RSM_START),            0x0, 0x00 },// Start reset power SM
  { qhp_l1_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_QHP_L1_RSM_START),            0x0, 0x00 },// Start reset power SM
  { pcs_reg, PHY_REG_WRI, HWIO_OFFS(PCIE_GEN3_HP_PCIE_PHY_START_CONTROL),      0x0, 0x00 },// Start PCS power SM 
  { invalid_reg, PHY_REG_NOP, 0xFF, 0xFF, 0xFF },
};
#endif /* (RPMASK & PCIE_RP(1)) - root port 1 enabled */

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
STATIC CONST pciePhyRegSeq_t common_phy_init_seq[]=
{
  /* Register Base,   Offset,                                         Value */
  { invalid_reg, PHY_REG_NOP,                                         0xFF }
};

/*******************************************************************************
 * Chipset specific complete common PHY init sequence
 ******************************************************************************/
STATIC CONST pciePhyAllVerSeq_t msm8998_common_phy_init_seq[] =
{
  {Pcie_Qcom_SoC_Ver_STOP, common_phy_init_seq},
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
    .clk                          = pcie_clk_rp0,
    .pwr                          = "pcie_0_gdsc",
    .icb_bw                       = pcie_0_icb_bw,
    .clk_set                      = pcie_0_clk_settings,
    .gpio_cfg                     = gpio_config_rp0,
    .mem                          = port_0_reg,
    .port_phy_init_seq            = sdm845_port_0_phy_init_seq,
    .port_phy_deinit_seq          = port_0_phy_de_init_seq,
    .link                         = port0_link,
    .hw_rp_index                  = 0,
    .io_coherency_en              = FALSE,
    .iommu_hdl                    = "\\_SB_.PCI0",
  },
#endif
/* Include port_info for RP1 if it exist */
#if (RPMASK & PCIE_RP(1))
  {
    .clk                          = pcie_clk_rp1,
    .pwr                          = "pcie_1_gdsc",
    .icb_bw                       = pcie_1_icb_bw,
    .clk_set                      = pcie_1_clk_settings,
    .gpio_cfg                     = gpio_config_rp1,
    .mem                          = port_1_reg,
    .port_phy_init_seq            = sdm845_port_1_phy_init_seq,
    .port_phy_deinit_seq          = port_1_phy_de_init_seq,
    .link                         = port1_link,
    .hw_rp_index                  = 1,
    .io_coherency_en              = FALSE,
    .iommu_hdl                    = "\\_SB_.PCI1",
  }
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
    .phy_init_seq  = msm8998_common_phy_init_seq,
    /* root port info for all enabled root ports */
    .rp_info       = root_port
  }
};
