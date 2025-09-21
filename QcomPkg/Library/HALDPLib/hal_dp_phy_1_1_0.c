/*===========================================================================

FILE:         hal_dp_phy_1_1_0.c

DESCRIPTION:  
  This file contains the programming for the DP Phy/PLL (v1.0.0 10nm).

  Copyright (c) 2016-2018 Qualcomm Technologies, Inc.  All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential.
===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif

/* -----------------------------------------------------------------------
** Includes
** ----------------------------------------------------------------------- */
#include "hal_dp_i.h"
#include "hal_dp_phy.h"
#include "hal_dp_phy_1_1_0_hwio.h" 
#include "hal_dp_phy_settings_1_1_0.h"


/* -----------------------------------------------------------------------
** Macros
** ----------------------------------------------------------------------- */

/* timeout counter values used in iterations of polling PLL & Phy ready status */
#define HAL_DP_PLL_POLL_MAX_READS            100   /*   100 iterations */
#define HAL_DP_PLL_POLL_TIMEOUT             1500   /* ~1500 us */

#define DP_PREEMPHASIS_LEVEL_NUM               3
#define DP_VOLTAGESWING_LEVEL_NUM              3


/* -----------------------------------------------------------------------
** Structure definitions
** ----------------------------------------------------------------------- */



/* -----------------------------------------------------------------------
** Static Variables
** ----------------------------------------------------------------------- */

/* Pre-emphasis and Voltage swing combinations supported by hardware */
static HAL_DP_PeSwPairType aPeSwPair[DP_VOLTAGESWING_LEVEL_NUM][DP_PREEMPHASIS_LEVEL_NUM] = 
{
  { { 0x20, 0x2f },{ 0x2B, 0x2F },{ 0x34, 0x36 } }, /* SW0_PE0, SW0_PE1, SW0_PE2  */
  { { 0x20, 0x31 },{ 0x2B, 0x3E },{ 0x32, 0x3F } }, /* SW1_PE0, SW1_PE1, SW1_PE2  */
  { { 0x20, 0x39 },{ 0x2B, 0x3F },{ 0x2B, 0x3F } }, /* SW2_PE0, SW2_PE1, N/A     */
};

/*DP_PHY_AUX_CFG1 register settings */
static uint32 aAuxPhyCfg1Setting[]={ 0x20, 0x13, 0x23, 0x1d };


/* -----------------------------------------------------------------------
** Function Prototypes
** ----------------------------------------------------------------------- */
static HAL_MDSS_ErrorType HAL_DP_Phy_Pll_Config(uint32 uIndx, HAL_DP_PllConfigType *pPllConfig);
static HAL_MDSS_ErrorType HAL_DP_Phy_Poll_PhyReady(uint32);
static HAL_MDSS_ErrorType HAL_DP_Phy_Poll_CReady(void);

/* -----------------------------------------------------------------------
** Private Functions
** ----------------------------------------------------------------------- */


/*********************************************************************************************
*
** FUNCTION: HAL_DP_Phy_Pll_Config()
*/
/*!
* \DESCRIPTION
*     Programs Phy/Pll registers.
*
* \param [in]   uPhySettingsTableIndex        - index to phy settings table that has to be applied
* \param [in]   pDPPllConfigInfo              - config to apply
*
* \retval HAL_DP_StatusType
*
**********************************************************************************************/
static HAL_MDSS_ErrorType HAL_DP_Phy_Pll_Config(uint32                 uPhySettingsTableIndex,
                                                HAL_DP_PllConfigType  *pDPPllConfigInfo)
{
  HAL_MDSS_ErrorType    eStatus    = HAL_MDSS_STATUS_SUCCESS;
  uint32               *pSettings  = DPPHYHwSettings[uPhySettingsTableIndex].pSettings;
  bool32                bPortSelect = pDPPllConfigInfo->bInvertedPlug;

  DP_LOG_MESSAGE_INFO("HAL_DP_Phy_pll_Config: tableIndex=%d, portSel=%d\n", uPhySettingsTableIndex, bPortSelect);

  /*
   * NOTE: SW_PORTSELECT (bit 0) of USB3_DP_COM_TYPE_CTRL is shared with usb driver 
   */
  if (FALSE == bPortSelect)
  {
    out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_USB3_DP_COM_TYPEC_CTRL_ADDR,     0x02);
  }
  else
  {
    out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_USB3_DP_COM_TYPEC_CTRL_ADDR,     0x03);
  }
  out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_USB3_DP_COM_SWI_CTRL_ADDR,         0x00);
  out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_USB3_DP_COM_SW_RESET_ADDR,         0x00);
  out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_USB3_DP_COM_POWER_DOWN_CTRL_ADDR,  0x01);

  // Power up PLL, L0/1/2/3, AUX, ungated all clocks, power up all analog blocks
  if (4 == pDPPllConfigInfo->uNumLanes)
  {
    out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_DP_PHY_PD_CTL_ADDR,           0x7d);
  }
  else
  {
    if (FALSE == bPortSelect)
    {
      out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_DP_PHY_PD_CTL_ADDR,         0x75);
    }
    else
    {
      out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_DP_PHY_PD_CTL_ADDR,         0x6d);
    }
  }

  out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_QSERDES_COM_SVS_MODE_CLK_SEL_ADDR,        0x01);
  out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_QSERDES_COM_SYSCLK_EN_SEL_ADDR,           0x37);
  out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_QSERDES_COM_SYS_CLK_CTRL_ADDR,            0x02);
  
  out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_QSERDES_COM_CLK_ENABLE1_ADDR,             0x0e);
  out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_QSERDES_COM_SYSCLK_BUF_ENABLE_ADDR,       0x06);
  out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_QSERDES_COM_CLK_SELECT_ADDR,              0x30);
  out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_QSERDES_COM_CMN_CONFIG_ADDR,              0x02);


  out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_QSERDES_COM_HSCLK_SEL_ADDR,               pSettings[DP_PHY_QSERDES_COM_HSCLK_SEL]);
  out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_QSERDES_COM_DEC_START_MODE0_ADDR,         pSettings[DP_PHY_QSERDES_COM_DEC_START_MODE0]);
  out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_QSERDES_COM_DIV_FRAC_START1_MODE0_ADDR,   pSettings[DP_PHY_QSERDES_COM_DIV_FRAC_START1_MODE0]);
  out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_QSERDES_COM_DIV_FRAC_START2_MODE0_ADDR,   pSettings[DP_PHY_QSERDES_COM_DIV_FRAC_START2_MODE0]);
  out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_QSERDES_COM_DIV_FRAC_START3_MODE0_ADDR,   pSettings[DP_PHY_QSERDES_COM_DIV_FRAC_START3_MODE0]);
  out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_QSERDES_COM_INTEGLOOP_GAIN0_MODE0_ADDR,   pSettings[DP_PHY_QSERDES_COM_INTEGLOOP_GAIN0_MODE0]);
  out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_QSERDES_COM_INTEGLOOP_GAIN1_MODE0_ADDR,   pSettings[DP_PHY_QSERDES_COM_INTEGLOOP_GAIN1_MODE0]);
  out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_QSERDES_COM_VCO_TUNE_MAP_ADDR,            pSettings[DP_PHY_QSERDES_COM_VCO_TUNE_MAP]);
  out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_QSERDES_COM_LOCK_CMP1_MODE0_ADDR,         pSettings[DP_PHY_QSERDES_COM_LOCK_CMP1_MODE0]);
  out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_QSERDES_COM_LOCK_CMP2_MODE0_ADDR,         pSettings[DP_PHY_QSERDES_COM_LOCK_CMP2_MODE0]);
  out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_QSERDES_COM_LOCK_CMP3_MODE0_ADDR,         pSettings[DP_PHY_QSERDES_COM_LOCK_CMP3_MODE0]);

  out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_QSERDES_COM_BG_TIMER_ADDR,                0x0a);
  out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_QSERDES_COM_CORECLK_DIV_MODE0_ADDR,       0x0a);
  out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_QSERDES_COM_VCO_TUNE_CTRL_ADDR,           0x00);
  out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_QSERDES_COM_BIAS_EN_CLKBUFLR_EN_ADDR,     0x3f); 
  out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_QSERDES_COM_CORE_CLK_EN_ADDR,             0x1f);


  // SSC config  
  if (HAL_DP_PLLCONFIG_FLAG_SSC & pDPPllConfigInfo->uFlags)
  {  
    // Spread settings
    out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_QSERDES_COM_SSC_EN_CENTER_ADDR,         pSettings[DP_PHY_QSERDES_COM_SSC_EN_CENTER]);
    out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_QSERDES_COM_SSC_ADJ_PER1_ADDR,          pSettings[DP_PHY_QSERDES_COM_SSC_ADJ_PER1]);
    out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_QSERDES_COM_SSC_PER1_ADDR,              pSettings[DP_PHY_QSERDES_COM_SSC_PER1]);
    out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_QSERDES_COM_SSC_PER2_ADDR,              pSettings[DP_PHY_QSERDES_COM_SSC_PER2]);
    out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_QSERDES_COM_SSC_STEP_SIZE1_ADDR,        pSettings[DP_PHY_QSERDES_COM_SSC_STEP_SIZE1]);
    out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_QSERDES_COM_SSC_STEP_SIZE2_ADDR,        pSettings[DP_PHY_QSERDES_COM_SSC_STEP_SIZE2]);
  }
  else
  {
    // Spread settings
    out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_QSERDES_COM_SSC_EN_CENTER_ADDR,         0x00);
    out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_QSERDES_COM_SSC_ADJ_PER1_ADDR,          0x00);
    out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_QSERDES_COM_SSC_PER1_ADDR,              0x00);
    out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_QSERDES_COM_SSC_PER2_ADDR,              0x00);
    out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_QSERDES_COM_SSC_STEP_SIZE1_ADDR,        0x00);
    out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_QSERDES_COM_SSC_STEP_SIZE2_ADDR,        0x00);
  }
  
  // PLL control regs
  out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_QSERDES_COM_PLL_IVCO_ADDR,                0x07);
  out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_QSERDES_COM_LOCK_CMP_EN_ADDR,             pSettings[DP_PHY_QSERDES_COM_LOCK_CMP_EN]);
  
  out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_QSERDES_COM_PLL_CCTRL_MODE0_ADDR,         0x36);
  out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_QSERDES_COM_PLL_RCTRL_MODE0_ADDR,         0x16);
  out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_QSERDES_COM_CP_CTRL_MODE0_ADDR,           0x06);

  if (FALSE == bPortSelect)
  {
    out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_DP_PHY_MODE_ADDR,         0x5c);
  }
  else
  {
    out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_DP_PHY_MODE_ADDR,         0x4c);
  }

  out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_DP_PHY_TX0_TX1_LANE_CTL_ADDR,             0x05);
  out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_DP_PHY_TX2_TX3_LANE_CTL_ADDR,             0x05);

  out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_QSERDES_TX0_TRANSCEIVER_BIAS_EN_ADDR,     0x1A);
  out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_QSERDES_TX0_VMODE_CTRL1_ADDR,             0x40);
  out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_QSERDES_TX0_PRE_STALL_LDO_BOOST_EN_ADDR,  0x30);
  out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_QSERDES_TX0_INTERFACE_SELECT_ADDR,        0x3d);
  out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_QSERDES_TX0_CLKBUF_ENABLE_ADDR,           0x0f);
  out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_QSERDES_TX0_RESET_TSYNC_EN_ADDR,          0x03);
  out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_QSERDES_TX0_TRAN_DRVR_EMP_EN_ADDR,        0x03);
  out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_QSERDES_TX0_PARRATE_REC_DETECT_IDLE_EN_ADDR, 0x00);
  out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_QSERDES_TX0_TX_INTERFACE_MODE_ADDR,       0x00);
  //out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_QSERDES_TX0_TX_EMP_POST1_LVL_ADDR, 0x1A); EMP
  out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_QSERDES_TX0_RES_CODE_LANE_TX_ADDR,        0x60);// 0x11);
  out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_QSERDES_TX0_TX_BAND_ADDR,                 0x04);

  out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_QSERDES_TX1_TRANSCEIVER_BIAS_EN_ADDR,     0x1A);
  out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_QSERDES_TX1_VMODE_CTRL1_ADDR,             0x40);
  out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_QSERDES_TX1_PRE_STALL_LDO_BOOST_EN_ADDR,  0x30);
  out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_QSERDES_TX1_INTERFACE_SELECT_ADDR,        0x3d);
  out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_QSERDES_TX1_CLKBUF_ENABLE_ADDR,           0x0f);
  out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_QSERDES_TX1_RESET_TSYNC_EN_ADDR,          0x03);
  out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_QSERDES_TX1_TRAN_DRVR_EMP_EN_ADDR,        0x03);
  out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_QSERDES_TX1_PARRATE_REC_DETECT_IDLE_EN_ADDR, 0x00);
  out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_QSERDES_TX1_TX_INTERFACE_MODE_ADDR,       0x00);
  //out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_QSERDES_TX1_TX_EMP_POST1_LVL_ADDR,      0x1A); EMP
  out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_QSERDES_TX1_RES_CODE_LANE_TX_ADDR,        0x60);//0x11);
  out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_QSERDES_TX1_TX_BAND_ADDR,                 0x04);

  /*new*/
  out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_QSERDES_TX1_RES_CODE_LANE_OFFSET_RX_ADDR, 0x07);
  out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_QSERDES_TX1_RCV_DETECT_LVL_2_ADDR,        0x00);

  //out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_QSERDES_RX1_UCDR_FASTLOCK_FO_GAIN_ADDR,   0x21);
  //out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_QSERDES_RX1_RX_EQU_ADAPTOR_CNTRL2_ADDR,   0x0);
  //out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_QSERDES_RX1_RX_EQU_ADAPTOR_CNTRL3_ADDR,   0x0);
  //out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_QSERDES_RX1_RX_EQU_ADAPTOR_CNTRL4_ADDR,   0x0);

  //out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_QSERDES_RX1_RX_EQ_OFFSET_ADAPTOR_CNTRL1_ADDR, 0x00);
  //out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_QSERDES_RX1_RX_EQ_OFFSET_ADAPTOR_CNTRL2_ADDR, 0x00);
  
  /*new*/

  //AUX clock setting
  out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_DP_PHY_VCO_DIV_ADDR,                      pSettings[DP_PHY_DP_PHY_VCO_DIV]);

  out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_DP_PHY_CFG_ADDR,                          0x01);
  out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_DP_PHY_CFG_ADDR,                          0x05);
  out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_DP_PHY_CFG_ADDR,                          0x01);
  out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_DP_PHY_CFG_ADDR,                          0x09);
  out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_QSERDES_COM_RESETSM_CNTRL_ADDR,           0x20);
    
  // Check if power up is done
  eStatus = HAL_DP_Phy_Poll_CReady();

  out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_DP_PHY_CFG_ADDR,                          0x19);
  if (HAL_MDSS_STATUS_SUCCESS == eStatus)
  {
    eStatus = HAL_DP_Phy_Poll_PhyReady(HWIO_PERIPH_SS_USB3_DP_PHY_DP_DP_PHY_STATUS_TSYNC_DONE_BMSK);
  }  

  if (4 == pDPPllConfigInfo->uNumLanes)
  {
    out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_QSERDES_TX0_HIGHZ_DRVR_EN_ADDR,         0x10);
    out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_QSERDES_TX0_TRANSCEIVER_BIAS_EN_ADDR,   0x3f);
    out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_QSERDES_TX1_HIGHZ_DRVR_EN_ADDR,         0x10);
    out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_QSERDES_TX1_TRANSCEIVER_BIAS_EN_ADDR,   0x3f);
  }
  else if (2 == pDPPllConfigInfo->uNumLanes)
  {
    if (FALSE == bPortSelect)
    {
      out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_QSERDES_TX1_HIGHZ_DRVR_EN_ADDR,       0x10);
      out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_QSERDES_TX1_TRANSCEIVER_BIAS_EN_ADDR, 0x3f);
    }
    else
    {
      out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_QSERDES_TX0_HIGHZ_DRVR_EN_ADDR,       0x10);
      out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_QSERDES_TX0_TRANSCEIVER_BIAS_EN_ADDR, 0x3f);
    }
  }
  else if (1 == pDPPllConfigInfo->uNumLanes)
  {
    if (FALSE == bPortSelect)
    {
      out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_QSERDES_TX1_HIGHZ_DRVR_EN_ADDR,       0x13);
      out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_QSERDES_TX1_TRANSCEIVER_BIAS_EN_ADDR, 0x3e);
    }
    else
    {
      out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_QSERDES_TX0_HIGHZ_DRVR_EN_ADDR,       0x13);
      out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_QSERDES_TX0_TRANSCEIVER_BIAS_EN_ADDR, 0x3e);
    }
  }

  out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_QSERDES_TX0_TX_POL_INV_ADDR,              0x0a);
  out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_QSERDES_TX1_TX_POL_INV_ADDR,              0x0a);
  out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_DP_PHY_CFG_ADDR,                          0x18);
  out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_DP_PHY_CFG_ADDR,                          0x19);

  if (HAL_MDSS_STATUS_SUCCESS == eStatus)
  {
    eStatus = HAL_DP_Phy_Poll_PhyReady(HWIO_PERIPH_SS_USB3_DP_PHY_DP_DP_PHY_STATUS_TSYNC_DONE_BMSK);
  }

  if (HAL_MDSS_STATUS_SUCCESS == eStatus)
  {
    // Configure pre-emphasis & driver strength - these may be changed during link training
    HAL_DP_PeSwConfigType  sPeSwConfig;
  
    sPeSwConfig.uLaneSwingLevel   = pDPPllConfigInfo->uVoltSwingLevel;
    sPeSwConfig.uPreemphasisLevel = pDPPllConfigInfo->uVoltSwingLevel;  
    if (HAL_MDSS_STATUS_SUCCESS != HAL_DP_1_1_0_PHY_SetupPeSw(&sPeSwConfig))
    {
      eStatus = HAL_MDSS_DSI_FAILED_INVALID_INPUT_PARAMETER;
    }
  }

  if (HAL_DP_PLLCONFIG_FLAG_ASYNC & pDPPllConfigInfo->uFlags)
  {
    // Program external PLL to output pDPPllConfigInfo->uPixelClkMNDInputMhz - no support presently
    eStatus = HAL_MDSS_STATUS_FAILED_NOT_SUPPORTED;
  }  

  return eStatus;
}


/*********************************************************************************************
*
** FUNCTION: HAL_DP_Phy_Poll_PhyReady()
*/
/*!
* \DESCRIPTION
*     poll if Phy PLL status is ready.
*
* \param [in]   uReadyMask - Ready mask to test against
* \param [out]  None
*
* \retval HAL_DP_StatusType
*
**********************************************************************************************/
static HAL_MDSS_ErrorType HAL_DP_Phy_Poll_PhyReady(uint32 uReadyMask)
{
  HAL_MDSS_ErrorType    eStatus            = HAL_MDSS_STATUS_SUCCESS;
  uint32                uNumReadsRemaining = HAL_DP_PLL_POLL_MAX_READS;
  uint32                uStatus            = 0;

  /* Poll PHY_READY */
  do
  {
    HAL_DP_SLEEP_US(HAL_DP_PLL_POLL_TIMEOUT);  /* delay */
    uStatus = HWIO_PERIPH_SS_USB3_DP_PHY_DP_DP_PHY_STATUS_IN & uReadyMask;
    uNumReadsRemaining--;
  } while ((!uStatus) && (uNumReadsRemaining));

  if (0 == (uReadyMask & uStatus))
  {
    eStatus = HAL_MDSS_DP_STATUS_FAILED;
  }

  return eStatus;
}

/*********************************************************************************************
*
** FUNCTION: HAL_DP_Phy_Poll_CReady()
*/
/*!
* \DESCRIPTION
*     poll if C_Ready PLL status is ready.
*
* \param [in]   None
* \param [out]  None
*
* \retval HAL_DP_StatusType
*
**********************************************************************************************/
static HAL_MDSS_ErrorType HAL_DP_Phy_Poll_CReady(void)
{
  HAL_MDSS_ErrorType    eStatus            = HAL_MDSS_STATUS_SUCCESS;
  uint32                uNumReadsRemaining = HAL_DP_PLL_POLL_MAX_READS;
  uint32                uStatus            = 0;

  do
  {
    HAL_DP_SLEEP_US(HAL_DP_PLL_POLL_TIMEOUT);
    uStatus = HWIO_PERIPH_SS_USB3_DP_PHY_DP_QSERDES_COM_C_READY_STATUS_IN & HWIO_PERIPH_SS_USB3_DP_PHY_DP_QSERDES_COM_C_READY_STATUS_C_READY_BMSK;
    uNumReadsRemaining--;
  } while ((!uStatus) && (uNumReadsRemaining));

  if (0 == (HWIO_PERIPH_SS_USB3_DP_PHY_DP_QSERDES_COM_C_READY_STATUS_C_READY_BMSK & uStatus))
  {
    eStatus = HAL_MDSS_DP_STATUS_FAILED;
  }
   
  return eStatus;
}

/* -----------------------------------------------------------------------
** Public Functions
** ----------------------------------------------------------------------- */

/****************************************************************************
*
** FUNCTION: HAL_DP_1_1_0_PHY_Config()
*/
/*!
* \DESCRIPTION
*   Configure DP PHY and PLL based on desired rates
*
* \param [in]   pDPPllConfigInfo  - DP PHY PLL config info
*
* \retval boolean  TRUE = success, FALSE = fail
*
****************************************************************************/
bool32 HAL_DP_1_1_0_PHY_Config(HAL_DP_PllConfigType  *pDPPllConfigInfo)
{
  bool32      bStatus    = FALSE;
  uint32      uFreqIndex = 0;

  for (uFreqIndex = 0; uFreqIndex < NUMBER_OF_DP_LINK_RATES_SUPPORTED; ++uFreqIndex)
  {
    if ((DPPHYHwSettings[uFreqIndex].uLinkRatePerLaneMbps == pDPPllConfigInfo->uLinkRateMbps) &&
        ((HAL_DP_PLLCONFIG_FLAG_ASYNC & pDPPllConfigInfo->uFlags) || 
         (DPPHYHwSettings[uFreqIndex].uVcoDividedClkMhz == pDPPllConfigInfo->uPixelClkMNDInputMhz)))
    {
      break;
    }
  }

  if (NUMBER_OF_DP_LINK_RATES_SUPPORTED > uFreqIndex)
  {
    /* program DP PLL and PHY */
    bStatus = (HAL_MDSS_STATUS_SUCCESS == HAL_DP_Phy_Pll_Config(uFreqIndex, 
                                                                pDPPllConfigInfo));
  }

  if (!bStatus)
  {    
    /* Requested settings not supported or config failed : Power down */
    out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_DP_PHY_PD_CTL_ADDR, 0x02);  // Power down whole DP PHY 
  }

  return bStatus;
}


/*********************************************************************************************
*
** FUNCTION: HAL_DP_1_1_0_0_PHY_Reset()
*/
/*!
* \brief
*        Reset and initialize DP PHY 
*
* \param [in]   
*
* \retval 
*
**********************************************************************************************/
void HAL_DP_1_1_0_PHY_Reset (void)
{
  
}
/****************************************************************************
*
** FUNCTION: HAL_DP_PHY_SetPHYMode()
*/
/*!
* \DESCRIPTION
*           Set the QMP PHY Mode to DP or USB3
*
* \param [in]     bDPMode  -  TRUE: DP mode,  FALSE: USB3 Mode
*
* \retval
*
****************************************************************************/
void HAL_DP_1_1_0_PHY_SetPHYMode(bool32 bDPMode)
{

  if (bDPMode)
  {
    // USB3 disabled and DP enabled 
    out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_USB3_DP_COM_PHY_MODE_CTRL_ADDR, 0x02);
  }
  else
  {
    // USB3 enabled and DP disabled 
    out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_USB3_DP_COM_PHY_MODE_CTRL_ADDR, 0x1);
  }
}
/*********************************************************************************************
*
** FUNCTION: HAL_DP_1_1_0_PHY_AUX_Init()
*/
/*!
* \brief
*        Initialize DP AUX PHY 
*
* \param [in]   
*
* \retval 
*
**********************************************************************************************/
void HAL_DP_1_1_0_PHY_AUX_Init (void)
{  
  // Power on
  out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_DP_PHY_PD_CTL_ADDR, 0x02);  // Power down whole DP PHY 
  out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_DP_PHY_PD_CTL_ADDR, 0x7d);  // Power on DP PHY 

  // Turn on BIAS current for PHY/PLL 
  out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_QSERDES_COM_BIAS_EN_CLKBUFLR_EN_ADDR, 0x3f);

  // Recommended settings  
  out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_DP_PHY_AUX_CFG1_ADDR, 0x13);
  out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_DP_PHY_AUX_CFG2_ADDR, 0x04);
  out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_DP_PHY_AUX_CFG3_ADDR, 0x00);
  out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_DP_PHY_AUX_CFG4_ADDR, 0x0a);
  out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_DP_PHY_AUX_CFG5_ADDR, 0x26);
  out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_DP_PHY_AUX_CFG6_ADDR, 0x0a);
  out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_DP_PHY_AUX_CFG7_ADDR, 0x03);
  out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_DP_PHY_AUX_CFG8_ADDR, 0xBB);
  out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_DP_PHY_AUX_CFG9_ADDR, 0x03);

}

/*********************************************************************************************
*
** FUNCTION: HAL_DP_1_1_0_PHY_SetupPeSw()
*/
/*!
* \brief
*        Set up DP Lane Pre Emphasis and voltage swing level
*
* \param [in/out]   pPeSwConfig - Pre-emphasis/Swing level settings.
*
* \retval  HAL_MDSS_ErrorType
*
**********************************************************************************************/
HAL_MDSS_ErrorType HAL_DP_1_1_0_PHY_SetupPeSw(HAL_DP_PeSwConfigType       *pPeSwConfig)
{
  HAL_MDSS_ErrorType  eStatus          = HAL_MDSS_STATUS_FAILED_NOT_SUPPORTED;
  
  pPeSwConfig->bPreemphNotSupported = FALSE;
  pPeSwConfig->bSwingNotsupported   = FALSE;
 
  if (pPeSwConfig->uPreemphasisLevel >= DP_PREEMPHASIS_LEVEL_NUM)
  {
    pPeSwConfig->bPreemphNotSupported = TRUE;
  }
  else if (pPeSwConfig->uLaneSwingLevel >= DP_VOLTAGESWING_LEVEL_NUM)
  {    
    pPeSwConfig->bSwingNotsupported   = TRUE;
  }
  else
  {
     uint32 uPreEmphasisReg  = aPeSwPair[pPeSwConfig->uLaneSwingLevel][pPeSwConfig->uPreemphasisLevel].uPreEmphasis;
     uint32 uVoltageSwingReg = aPeSwPair[pPeSwConfig->uLaneSwingLevel][pPeSwConfig->uPreemphasisLevel].uVoltageSwing;

     if (0 == uPreEmphasisReg)
     {      
       pPeSwConfig->bSwingNotsupported   = TRUE;
     }
     else
     {  

  DP_LOG_MESSAGE_INFO("HAL_DP_1_1_0_Phy_SetupPeSw: pe=%x sw=%x, pl=%d sl=%d\n", uPreEmphasisReg, uVoltageSwingReg, pPeSwConfig->uPreemphasisLevel, pPeSwConfig->uLaneSwingLevel);

       out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_QSERDES_TX0_TX_DRV_LVL_ADDR,       uVoltageSwingReg);
       out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_QSERDES_TX0_TX_EMP_POST1_LVL_ADDR, uPreEmphasisReg);
       
       out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_QSERDES_TX1_TX_DRV_LVL_ADDR,       uVoltageSwingReg);
       out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_QSERDES_TX1_TX_EMP_POST1_LVL_ADDR,  uPreEmphasisReg);

       eStatus = HAL_MDSS_STATUS_SUCCESS;
     }
  }

  return eStatus;
}

/*********************************************************************************************
*
** FUNCTION: HAL_DP_1_1_0_PHY_GetMaxPeSwLevel()
*/
/*!
* \brief
*        Get max supported DP Lane Pre Emphasis and voltage swing levels
*
* \param [out]   uMaxPreemphLvl   -  max Pre-emphasis level supported.
* \param [out]   uMaxVoltSwingLvl -  max voltage swing level supported.
*
* \retval  void
*
**********************************************************************************************/
void HAL_DP_1_1_0_PHY_GetMaxPeSwLevel(uint32   *uMaxPreemphLvl, 
                                      uint32   *uMaxVoltSwingLvl)
{
  *uMaxPreemphLvl   = DP_PREEMPHASIS_LEVEL_NUM - 1;
  *uMaxVoltSwingLvl = DP_VOLTAGESWING_LEVEL_NUM - 1;
}

/*********************************************************************************************
*
** FUNCTION: HAL_DP_1_1_0_PHY_AUXConfig()
*/
/*!
* \brief
*        Config AUX PHY registers
*
* \param [in]   pAuxPhyCfg   -  AUX PHY settings.
*
* \retval  HAL_MDSS_ErrorType
*
**********************************************************************************************/
HAL_MDSS_ErrorType HAL_DP_1_1_0_PHY_AUXConfig(HAL_DP_AuxPhyCfgType *pAuxPhyCfg)
{
  HAL_MDSS_ErrorType eStatus = HAL_MDSS_STATUS_FAILED_NOT_SUPPORTED;

  if ((NULL != pAuxPhyCfg) &&
      (NULL != pAuxPhyCfg->pDPAuxPhyCfg1Index))
  {
    if (*pAuxPhyCfg->pDPAuxPhyCfg1Index < HAL_DP_LIST_LENGTH(aAuxPhyCfg1Setting))
    {
      out_dword(HWIO_PERIPH_SS_USB3_DP_PHY_DP_DP_PHY_AUX_CFG1_ADDR, aAuxPhyCfg1Setting[*pAuxPhyCfg->pDPAuxPhyCfg1Index]);

      eStatus = HAL_MDSS_STATUS_SUCCESS;
    }
  }
  
  return eStatus;
}
#ifdef __cplusplus
}
#endif

