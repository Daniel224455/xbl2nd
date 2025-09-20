/*=============================================================================

File: hal_dp_phy_qmp_pll_7nm.c

Header file for DP PHY/PLL settings of 1.2.0 hardware

Copyright (c) 2016-2020 Qualcomm Technologies, Inc.
All Rights Reserved.
Confidential and Proprietary - Qualcomm Technologies, Inc.
=============================================================================*/

#ifdef __cplusplus
extern "C" {
#endif

/* -----------------------------------------------------------------------
** Includes
** ----------------------------------------------------------------------- */
#include "hal_dp_i.h"
#include "hal_dp_phy.h"
#include "hal_dp_phy_qmp_pll_7nm.h"
#include "hal_dp_phy_1_2_0_hwio.h"

/* -----------------------------------------------------------------------
** Types
** ----------------------------------------------------------------------- */

/* List of Frequency dependent registers */
typedef enum {
  DP_PHY_QSERDES_COM_HSCLK_SEL,
  DP_PHY_QSERDES_COM_DEC_START_MODE0,
  DP_PHY_QSERDES_COM_DIV_FRAC_START1_MODE0,
  DP_PHY_QSERDES_COM_DIV_FRAC_START2_MODE0,
  DP_PHY_QSERDES_COM_DIV_FRAC_START3_MODE0,
  DP_PHY_QSERDES_COM_INTEGLOOP_GAIN0_MODE0,
  DP_PHY_QSERDES_COM_INTEGLOOP_GAIN1_MODE0,
  DP_PHY_QSERDES_COM_VCO_TUNE_MAP,
  DP_PHY_QSERDES_COM_LOCK_CMP1_MODE0,
  DP_PHY_QSERDES_COM_LOCK_CMP2_MODE0,
  DP_PHY_QSERDES_COM_LOCK_CMP3_MODE0,
  DP_PHY_QSERDES_COM_SSC_EN_CENTER,
  DP_PHY_QSERDES_COM_SSC_ADJ_PER1,
  DP_PHY_QSERDES_COM_SSC_PER1,
  DP_PHY_QSERDES_COM_SSC_PER2,
  DP_PHY_QSERDES_COM_SSC_STEP_SIZE1,
  DP_PHY_QSERDES_COM_SSC_STEP_SIZE2,
  DP_PHY_QSERDES_COM_LOCK_CMP_EN,
  DP_PHY_DP_PHY_VCO_DIV,
  DP_PHY_CONFIG_MAX
} DP_PHY_ConfigRegs;

/* DP Rate Settings */
typedef struct
{
  uint32              uLinkRatePerLaneMbps;     // Desired link rate
  uint32              uVcoDividedClkMhz;        // vco_divided_clk
  uint32             *pSettings;                // Recommended settings
} DP_PHYHwSettingsType;

/* Frequency dependent configuration for link rate of 1.62 Mbps*/
uint32 aDphySettings1620Mbps[DP_PHY_CONFIG_MAX] =
{
  0x00000005,    // DP_PHY_QSERDES_COM_HSCLK_SEL, hsclk_divset 8100M divided by 5
  0x00000069,    // DP_PHY_QSERDES_COM_DEC_START_MODE0,
  0x00000000,    // DP_PHY_QSERDES_COM_DIV_FRAC_START1_MODE0,
  0x00000080,    // DP_PHY_QSERDES_COM_DIV_FRAC_START2_MODE0,
  0x00000007,    // DP_PHY_QSERDES_COM_DIV_FRAC_START3_MODE0,
  0x0000003f,    // DP_PHY_QSERDES_COM_INTEGLOOP_GAIN0_MODE0,
  0x00000000,    // DP_PHY_QSERDES_COM_INTEGLOOP_GAIN1_MODE0,
  0x00000000,    // DP_PHY_QSERDES_COM_VCO_TUNE_MAP,
  0x0000006f,    // DP_PHY_QSERDES_COM_LOCK_CMP1_MODE0,
  0x00000008,    // DP_PHY_QSERDES_COM_LOCK_CMP2_MODE0,
  0x00000000,    // DP_PHY_QSERDES_COM_LOCK_CMP3_MODE0,
  0x00000001,    // DP_PHY_QSERDES_COM_SSC_EN_CENTER,
  0x00000000,    // DP_PHY_QSERDES_COM_SSC_ADJ_PER1,
  0x00000036,    // DP_PHY_QSERDES_COM_SSC_PER1,
  0x00000001,    // DP_PHY_QSERDES_COM_SSC_PER2,
  0x00000045,    // DP_PHY_QSERDES_COM_SSC_STEP_SIZE1,
  0x00000006,    // DP_PHY_QSERDES_COM_SSC_STEP_SIZE2,
  0x00000004,    // DP_PHY_QSERDES_COM_LOCK_CMP_EN,
  0x00000001,    // DP_PHY_DP_PHY_VCO_DIV, aux_clk_sel divided by 2
};

/* Frequency dependent configuration for link rate of 2.7 Mbps*/
uint32 aDphySettings2700Mbps[DP_PHY_CONFIG_MAX] =
{
  0x00000003,    // DP_PHY_QSERDES_COM_HSCLK_SEL, hsclk_divset 8100M divided by 3
  0x00000069,    // DP_PHY_QSERDES_COM_DEC_START_MODE0,
  0x00000000,    // DP_PHY_QSERDES_COM_DIV_FRAC_START1_MODE0,
  0x00000080,    // DP_PHY_QSERDES_COM_DIV_FRAC_START2_MODE0,
  0x00000007,    // DP_PHY_QSERDES_COM_DIV_FRAC_START3_MODE0,
  0x0000003f,    // DP_PHY_QSERDES_COM_INTEGLOOP_GAIN0_MODE0,
  0x00000000,    // DP_PHY_QSERDES_COM_INTEGLOOP_GAIN1_MODE0,
  0x00000000,    // DP_PHY_QSERDES_COM_VCO_TUNE_MAP,
  0x0000000f,    // DP_PHY_QSERDES_COM_LOCK_CMP1_MODE0,
  0x0000000e,    // DP_PHY_QSERDES_COM_LOCK_CMP2_MODE0,
  0x00000000,    // DP_PHY_QSERDES_COM_LOCK_CMP3_MODE0,
  0x00000001,    // DP_PHY_QSERDES_COM_SSC_EN_CENTER,
  0x00000000,    // DP_PHY_QSERDES_COM_SSC_ADJ_PER1,
  0x00000036,    // DP_PHY_QSERDES_COM_SSC_PER1,
  0x00000001,    // DP_PHY_QSERDES_COM_SSC_PER2,
  0x00000045,    // DP_PHY_QSERDES_COM_SSC_STEP_SIZE1,
  0x00000006,    // DP_PHY_QSERDES_COM_SSC_STEP_SIZE2,
  0x00000008,    // DP_PHY_QSERDES_COM_LOCK_CMP_EN,
  0x00000001,    // DP_PHY_DP_PHY_VCO_DIV, aux_clk_sel divided by 2
};

/* Frequency dependent configuration for link rate of 5.4 Mbps*/
uint32 aDphySettings5400Mbps[DP_PHY_CONFIG_MAX] =
{
  0x00000001,    // DP_PHY_QSERDES_COM_HSCLK_SEL, hsclk_divset 1080M divided by 2
  0x0000008c,    // DP_PHY_QSERDES_COM_DEC_START_MODE0,
  0x00000000,    // DP_PHY_QSERDES_COM_DIV_FRAC_START1_MODE0,
  0x00000000,    // DP_PHY_QSERDES_COM_DIV_FRAC_START2_MODE0,
  0x0000000a,    // DP_PHY_QSERDES_COM_DIV_FRAC_START3_MODE0,
  0x0000003f,    // DP_PHY_QSERDES_COM_INTEGLOOP_GAIN0_MODE0,
  0x00000000,    // DP_PHY_QSERDES_COM_INTEGLOOP_GAIN1_MODE0,
  0x00000000,    // DP_PHY_QSERDES_COM_VCO_TUNE_MAP,
  0x0000001f,    // DP_PHY_QSERDES_COM_LOCK_CMP1_MODE0,
  0x0000001c,    // DP_PHY_QSERDES_COM_LOCK_CMP2_MODE0,
  0x00000000,    // DP_PHY_QSERDES_COM_LOCK_CMP3_MODE0,
  0x00000001,    // DP_PHY_QSERDES_COM_SSC_EN_CENTER,
  0x00000000,    // DP_PHY_QSERDES_COM_SSC_ADJ_PER1,
  0x00000036,    // DP_PHY_QSERDES_COM_SSC_PER1,
  0x00000001,    // DP_PHY_QSERDES_COM_SSC_PER2,
  0x0000005c,    // DP_PHY_QSERDES_COM_SSC_STEP_SIZE1,
  0x00000008,    // DP_PHY_QSERDES_COM_SSC_STEP_SIZE2,
  0x00000008,    // DP_PHY_QSERDES_COM_LOCK_CMP_EN,
  0x00000002,    // DP_PHY_DP_PHY_VCO_DIV, aux_clk_sel divided by 4
};
/* Frequency dependent configuration for link rate of 8.1 Mbps*/
uint32 aDphySettings8100Mbps[DP_PHY_CONFIG_MAX] =
{
  0x00000000,    // DP_PHY_QSERDES_COM_HSCLK_SEL, hsclk_divset 8100M divided by 1
  0x00000069,    // DP_PHY_QSERDES_COM_DEC_START_MODE0,
  0x00000000,    // DP_PHY_QSERDES_COM_DIV_FRAC_START1_MODE0,
  0x00000080,    // DP_PHY_QSERDES_COM_DIV_FRAC_START2_MODE0,
  0x00000007,    // DP_PHY_QSERDES_COM_DIV_FRAC_START3_MODE0,
  0x0000003f,    // DP_PHY_QSERDES_COM_INTEGLOOP_GAIN0_MODE0,
  0x00000000,    // DP_PHY_QSERDES_COM_INTEGLOOP_GAIN1_MODE0,
  0x00000000,    // DP_PHY_QSERDES_COM_VCO_TUNE_MAP,
  0x0000002f,    // DP_PHY_QSERDES_COM_LOCK_CMP1_MODE0,
  0x0000002a,    // DP_PHY_QSERDES_COM_LOCK_CMP2_MODE0,
  0x00000000,    // DP_PHY_QSERDES_COM_LOCK_CMP3_MODE0,
  0x00000001,    // DP_PHY_QSERDES_COM_SSC_EN_CENTER,
  0x00000000,    // DP_PHY_QSERDES_COM_SSC_ADJ_PER1,
  0x00000036,    // DP_PHY_QSERDES_COM_SSC_PER1,
  0x00000001,    // DP_PHY_QSERDES_COM_SSC_PER2,
  0x00000045,    // DP_PHY_QSERDES_COM_SSC_STEP_SIZE1,
  0x00000006,    // DP_PHY_QSERDES_COM_SSC_STEP_SIZE2,
  0x00000008,    // DP_PHY_QSERDES_COM_LOCK_CMP_EN,
  0x00000000,    // DP_PHY_DP_PHY_VCO_DIV, aux_clk_sel divided by 6
};

/* Array of DP configuraiton for various link rates */
static const DP_PHYHwSettingsType DPPHYHwSettings[] = {
  { 1620,  810, &aDphySettings1620Mbps[0] },
  { 2700, 1350, &aDphySettings2700Mbps[0] },
  { 5400, 1350, &aDphySettings5400Mbps[0] },
  { 8100, 1350, &aDphySettings8100Mbps[0] },
};

/*  Below are the currently supported settings :

 *----------------------------------------------------------------------------------------
  | linkRate |  target pll vco  hsclk_divsel   clk_divtx   aux_clk_sel   vco_divided_clk |
  |----------|---------------------------------------------------------------------------|
  |  1620    |   8100 Mhz          /5           1620 Mhz         /2            810 Mhz   |
  |----------|---------------------------------------------------------------------------|     
  |  2700    |   8100 Mhz          /3           2700 Mhz         /2           1350 Mhz   |
  |--------------------------------------------------------------------------------------|
  |  5400    |   10800 Mhz         /2           5400 Mhz         /4           1350 Mhz   |
  |--------------------------------------------------------------------------------------|
  |  8100    |   8100 Mhz          /1           8100 Mhz         /6           1350 Mhz   |
  ----------------------------------------------------------------------------------------*/


#define NUMBER_OF_DP_LINK_RATES_SUPPORTED    sizeof(DPPHYHwSettings)/(sizeof(DP_PHYHwSettingsType))


/*********************************************************************************************
*
** FUNCTION: HAL_DP_PHY_Qmp_Pll_7nm_Config()
*/
/*!
* \brief
*        Configures QMP_PLL for the specified rate.
*
* \param [in]       uOffset      - uOffset to the base address of QMP PLL
* \param [in]       pDPPllConfigInfo - DP PLL configuration
*
* \retval  HAL_MDSS_ErrorType
*
**********************************************************************************************/
HAL_MDSS_ErrorType HAL_DP_PHY_Qmp_Pll_7nm_Config(uintPtr uOffset, HAL_DP_PllConfigType  *pDPPllConfigInfo)
{
  uint32               uFreqIndex = 0;
  uint32              *pSettings  = NULL;
  HAL_MDSS_ErrorType   eStatus    = HAL_MDSS_STATUS_FAILED_NOT_SUPPORTED;

  for (uFreqIndex = 0; uFreqIndex < NUMBER_OF_DP_LINK_RATES_SUPPORTED; ++uFreqIndex)
  {
    if ((DPPHYHwSettings[uFreqIndex].uLinkRatePerLaneMbps == pDPPllConfigInfo->uLinkRateMbps) &&
        (DPPHYHwSettings[uFreqIndex].uVcoDividedClkMhz == pDPPllConfigInfo->uPixelClkMNDInputMhz))
    {
      pSettings = DPPHYHwSettings[uFreqIndex].pSettings;
      eStatus   = HAL_MDSS_STATUS_SUCCESS;
      break;
    }
  }

  if (NULL != pSettings)
  {
    out_dword(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_QSERDES_COM_SVS_MODE_CLK_SEL_ADDR,          0x05);
    out_dword(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_QSERDES_COM_SYSCLK_EN_SEL_ADDR,             0x3b);
    out_dword(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_QSERDES_COM_SYS_CLK_CTRL_ADDR,              0x02);
    out_dword(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_QSERDES_COM_CLK_ENABLE1_ADDR,               0x0c);
    out_dword(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_QSERDES_COM_SYSCLK_BUF_ENABLE_ADDR,         0x06);
    out_dword(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_QSERDES_COM_CLK_SELECT_ADDR,                0x30);

    out_dword(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_QSERDES_COM_HSCLK_SEL_ADDR,                 pSettings[DP_PHY_QSERDES_COM_HSCLK_SEL]);

    out_dword(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_QSERDES_COM_PLL_IVCO_ADDR,                  0x0f);
    out_dword(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_QSERDES_COM_LOCK_CMP_EN_ADDR,               pSettings[DP_PHY_QSERDES_COM_LOCK_CMP_EN]);
    out_dword(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_QSERDES_COM_PLL_CCTRL_MODE0_ADDR,           0x36);
    out_dword(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_QSERDES_COM_PLL_RCTRL_MODE0_ADDR,           0x16);
    out_dword(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_QSERDES_COM_CP_CTRL_MODE0_ADDR,             0x06);

    out_dword(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_QSERDES_COM_DEC_START_MODE0_ADDR,           pSettings[DP_PHY_QSERDES_COM_DEC_START_MODE0]);
    out_dword(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_QSERDES_COM_DIV_FRAC_START1_MODE0_ADDR,     pSettings[DP_PHY_QSERDES_COM_DIV_FRAC_START1_MODE0]);
    out_dword(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_QSERDES_COM_DIV_FRAC_START2_MODE0_ADDR,     pSettings[DP_PHY_QSERDES_COM_DIV_FRAC_START2_MODE0]);
    out_dword(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_QSERDES_COM_DIV_FRAC_START3_MODE0_ADDR,     pSettings[DP_PHY_QSERDES_COM_DIV_FRAC_START3_MODE0]);

    out_dword(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_QSERDES_COM_CMN_CONFIG_ADDR,                0x02);

    out_dword(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_QSERDES_COM_INTEGLOOP_GAIN0_MODE0_ADDR,     pSettings[DP_PHY_QSERDES_COM_INTEGLOOP_GAIN0_MODE0]);
    out_dword(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_QSERDES_COM_INTEGLOOP_GAIN1_MODE0_ADDR,     pSettings[DP_PHY_QSERDES_COM_INTEGLOOP_GAIN1_MODE0]);
    out_dword(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_QSERDES_COM_VCO_TUNE_MAP_ADDR,              pSettings[DP_PHY_QSERDES_COM_VCO_TUNE_MAP]);
    out_dword(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_QSERDES_COM_LOCK_CMP1_MODE0_ADDR,           pSettings[DP_PHY_QSERDES_COM_LOCK_CMP1_MODE0]);
    out_dword(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_QSERDES_COM_LOCK_CMP2_MODE0_ADDR,           pSettings[DP_PHY_QSERDES_COM_LOCK_CMP2_MODE0]);

    out_dword(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_QSERDES_COM_BG_TIMER_ADDR,                  0x0a);
    out_dword(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_QSERDES_COM_CORECLK_DIV_MODE0_ADDR,         0x0a);
    out_dword(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_QSERDES_COM_VCO_TUNE_CTRL_ADDR,             0x00);
    out_dword(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_QSERDES_COM_BIAS_EN_CLKBUFLR_EN_ADDR,       0x17);
    out_dword(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_QSERDES_COM_CORE_CLK_EN_ADDR,               0x1f);


    // SSC config
    if (HAL_DP_PLLCONFIG_FLAG_SSC & pDPPllConfigInfo->uFlags)
    {
      // Spread settings
      out_dword(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_QSERDES_COM_SSC_EN_CENTER_ADDR,           pSettings[DP_PHY_QSERDES_COM_SSC_EN_CENTER]);
      out_dword(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_QSERDES_COM_SSC_ADJ_PER1_ADDR,            pSettings[DP_PHY_QSERDES_COM_SSC_ADJ_PER1]);
      out_dword(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_QSERDES_COM_SSC_PER1_ADDR,                pSettings[DP_PHY_QSERDES_COM_SSC_PER1]);
      out_dword(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_QSERDES_COM_SSC_PER2_ADDR,                pSettings[DP_PHY_QSERDES_COM_SSC_PER2]);
    }
    else
    {
      // Spread settings
      out_dword(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_QSERDES_COM_SSC_EN_CENTER_ADDR,           0x00);
      out_dword(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_QSERDES_COM_SSC_ADJ_PER1_ADDR,            0x00);
      out_dword(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_QSERDES_COM_SSC_PER1_ADDR,                0x00);
      out_dword(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_QSERDES_COM_SSC_PER2_ADDR,                0x00);
    }

    // PLL control regs
    out_dword(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_DP_PHY_VCO_DIV_ADDR,                        pSettings[DP_PHY_DP_PHY_VCO_DIV]);
  }

  return eStatus;
}

/*********************************************************************************************
*
** FUNCTION: HAL_DP_TypeC_Phy_Poll_CReady()
*/
/*!
* \brief
*     poll if C_Ready PLL status is ready.
*
* \param [in]       uOffset      - Base address of the QMP PLL
*
* \retval HAL_DP_StatusType
*
**********************************************************************************************/
HAL_MDSS_ErrorType HAL_DP_PHY_Qmp_Pll_7nm_Poll_Cready(uintPtr uOffset)
{
  HAL_MDSS_ErrorType    eStatus            = HAL_MDSS_STATUS_SUCCESS;
  uint32                uNumReadsRemaining = HAL_DP_PLL_POLL_MAX_READS;
  uint32                uStatus            = 0;
  uintPtr                uReg              = uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_QSERDES_COM_C_READY_STATUS_ADDR;

  do
  {
    HAL_DP_SLEEP_US(HAL_DP_PLL_POLL_TIMEOUT);
    uStatus = in_dword_masked(uReg, HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_QSERDES_COM_C_READY_STATUS_C_READY_BMSK);
    uNumReadsRemaining--;
  } while ((!uStatus) && (uNumReadsRemaining));

  if (!uStatus)
  {
    eStatus = HAL_MDSS_DP_STATUS_FAILED;
  }

  return eStatus;
}

#ifdef __cplusplus
}
#endif

