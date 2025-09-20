/*===========================================================================

FILE:         hal_dp_phy_1_2_0.c

DESCRIPTION:  
  This file contains the programming for the EDP Phy/PLL (v1.2.0 7nm).

  Copyright (c) 2016-2020 Qualcomm Technologies, Inc.  All Rights Reserved.
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
#include "hal_dp_phy_1_2_0_hwio.h"


/* -----------------------------------------------------------------------
** Macros
** ----------------------------------------------------------------------- */

#define DP_PREEMPHASIS_LEVEL_NUM               3
#define DP_VOLTAGESWING_LEVEL_NUM              3

#define HAL_DP_PLL_POLL_MAX_READS            100   /*   100 iterations */
#define HAL_DP_PLL_POLL_TIMEOUT             1500   /* ~1500 us */
#define HAL_DP_REFGEN_TIMEOUT                 10   /* 10 us */

#define DP_PHY_REGREAD(_a_)                   in_dword((_a_))
#define DP_PHY_REGWRITE(_a_, _d_)             out_dword((_a_), (_d_))

/* -----------------------------------------------------------------------
** Structure definitions
** ----------------------------------------------------------------------- */

/*!
 * \struct DP_PHY_RateParams
 *
 * Link rate dependent parameters for DP/eDP PHY
 */
typedef struct {
  uint8 uHsclkSel;
  uint8 uLockCmpEn;
  uint8 uDecStart;
  uint8 uDivFracStart1;
  uint8 uDivFracStart2;
  uint8 uDivFracStart3;
  uint8 uCmnConfig;
  uint8 uLockCmp1;
  uint8 uLockCmp2;
  uint8 uPhyVcoDiv;
  uint8 uVcoTune1;
  uint8 uVcoTune2;
} DP_PHY_RateParams;

 /*!
  * \struct DP_PHY_SSCParams
  *
  * SSC parameters for various link rates
  */
typedef struct {
  uint8 uAdjPer1;
  uint8 uSSCPer1;
  uint8 uSSCPer2;
  uint8 uStepSize1;
  uint8 uStepSize2;
} DP_PHY_SSCParams;

/*!
 * \enum EDP_PHY_Rates
 *
 * eDP/DP PHY Link rates
 */
typedef enum {
  EDP_PHY_RATE_1P62G = 0,
  EDP_PHY_RATE_2P16G,
  EDP_PHY_RATE_2P43G,
  EDP_PHY_RATE_2P7G,
  EDP_PHY_RATE_3P24G,
  EDP_PHY_RATE_4P32G,
  EDP_PHY_RATE_5P4G,
  EDP_PHY_RATE_5P94G,
  EDP_PHY_RATE_8P1G,
  EDP_PHY_RATE_MAX
} EDP_PHY_Rates;

/*!
 * \struct EDP_PHYHwSettingsType
 *
 * Link rate dependent parameters for DP/eDP PHY mapped to link rates
 */
typedef struct
{
  uint32              uLinkRatePerLaneMbps;     // Desired link rate
  uint32              uVcoDividedClkMhz;        // vco_divided_clk
  DP_PHY_RateParams   sRateParams;              // Recommended settings
} EDP_PHYHwSettingsType;

/* -----------------------------------------------------------------------
** Static Variables
** ----------------------------------------------------------------------- */

/* Rate dependent data for eDP */
 EDP_PHYHwSettingsType gEDPPhyHwSettings[EDP_PHY_RATE_MAX] = {
  {1620,  810, {0x05, 0x00, 0x69, 0x00, 0x80, 0x07, 0x02, 0x6F, 0x08, 0x01, 0xA0, 0x03}},  // EDP_PHY_RATE_1P62G
  {2160, 1080, {0x04, 0x00, 0x70, 0x00, 0x00, 0x08, 0x02, 0x3F, 0x0B, 0x01, 0x34, 0x03}},  // EDP_PHY_RATE_2P16G
  {2430, 1215, {0x04, 0x00, 0x7E, 0x00, 0x00, 0x09, 0x02, 0xA7, 0x0C, 0x01, 0x5C, 0x02}},  // EDP_PHY_RATE_2P43G
  {2700, 1350, {0x03, 0x00, 0x69, 0x00, 0x80, 0x07, 0x02, 0x0F, 0x0E, 0x01, 0xA0, 0x03}},  // EDP_PHY_RATE_2P7G
  {3240,  810, {0x03, 0x00, 0x7E, 0x00, 0x00, 0x09, 0x02, 0xDF, 0x10, 0x02, 0x5C, 0x02}},  // EDP_PHY_RATE_3P24G
  {4320, 1080, {0x01, 0x00, 0x70, 0x00, 0x00, 0x08, 0x02, 0x7F, 0x16, 0x02, 0x34, 0x03}},  // EDP_PHY_RATE_4P32G
  {5400, 1350, {0x01, 0x00, 0x8C, 0x00, 0x00, 0x0A, 0x02, 0x1F, 0x1C, 0x02, 0x84, 0x01}},  // EDP_PHY_RATE_5P4G
  {5940,  990, {0x01, 0x00, 0x9A, 0x00, 0x00, 0x0B, 0x02, 0xEF, 0x1E, 0x00, 0xAC, 0x00}},  // EDP_PHY_RATE_5P94G
  {8100, 1350, {0x00, 0x00, 0x69, 0x00, 0x80, 0x07, 0x02, 0x2F, 0x2A, 0x00, 0xA0, 0x03}},  // EDP_PHY_RATE_8P1G
};

/* SSC params for various eDP link rates*/
DP_PHY_SSCParams gEDPSSCParams[EDP_PHY_RATE_MAX] = {
  {0x00, 0x36, 0x01, 0x45, 0x06},  // EDP_PHY_RATE_1P62G
  {0x00, 0x36, 0x01, 0xB0, 0x06},  // EDP_PHY_RATE_2P16G
  {0x00, 0x36, 0x01, 0x86, 0x07},  // EDP_PHY_RATE_2P43G
  {0x00, 0x36, 0x01, 0x45, 0x06},  // EDP_PHY_RATE_2P7G
  {0x00, 0x40, 0x01, 0x86, 0x07},  // EDP_PHY_RATE_3P24G
  {0x00, 0x36, 0x00, 0xB0, 0x06},  // EDP_PHY_RATE_4P32G
  {0x00, 0x36, 0x01, 0x5C, 0x08},  // EDP_PHY_RATE_5P4G
  {0x00, 0x36, 0x01, 0x33, 0x09},  // EDP_PHY_RATE_5P94G
  {0x00, 0x36, 0x01, 0x45, 0x06},  // EDP_PHY_RATE_8P1G
};

/* Pre-emphasis and Voltage swing combinations supported by hardware */
static HAL_DP_PeSwPairType aPeSwPair[DP_VOLTAGESWING_LEVEL_NUM][DP_PREEMPHASIS_LEVEL_NUM] =
{
  {{0x20, 0x29 }, {0x2E, 0x37}, {0x37, 0x3F}}, /* SW0_PE0, SW0_PE1, SW0_PE2  */
  {{0x20, 0x31 }, {0x2D, 0x3D}, {0x36, 0x3F}}, /* SW1_PE0, SW1_PE1, SW1_PE2  */
  {{0x20, 0x3C }, {0x2D, 0x3F}, {0x00, 0x00}}, /* SW2_PE0, SW2_PE1, N/A     */
};

/*DP_PHY_AUX_CFG1 register settings */
static uint32 aAuxPhyCfg1Setting[] = {0x13, 0x23, 0x1d};

/* -----------------------------------------------------------------------
** Function Prototypes
** ----------------------------------------------------------------------- */

/* -----------------------------------------------------------------------
** Private Functions
** ----------------------------------------------------------------------- */

/****************************************************************************
*
** FUNCTION: HAL_DP_PHY_GetRegBaseOffset()
*/
/*!
* \brief   
*     Get DP register address offset for a give DP device ID.
*
* \param [in]  eDeviceId - DP device ID
*
* \retval - register base offset
*
****************************************************************************/
static uintPtr HAL_DP_PHY_GetRegBaseOffset(DP_DeviceIDType  eDeviceId)
{
  uintPtr   uOffset;

  switch (eDeviceId)
  {
  case DP_DEVICE_ID_EDP:
    {
      uOffset = 0;
      break;
    }

  case DP_DEVICE_ID_DP:
    {
      uOffset = (AHB2EDPPHY_CM_DPPHY2_REG_BASE - AHB2EDPPHY_CM_EDP_PHY_REG_BASE);
      break;
    }

  default:
    {
      //No hardware support for other DP device IDs
      uOffset        = 0;
      break;
    }
  }

  return uOffset;
}



/*********************************************************************************************
*
** FUNCTION: HAL_DP_Phy_Poll_CReady()
*/
/*!
* \DESCRIPTION
*     poll if C_Ready PLL status is ready.
*
* \param [in]  uRegAddr - Address of the status register to check
*        [in]  uMask    - Mask to check on the provided status register
*
* \retval HAL_DP_StatusType
*
**********************************************************************************************/
static HAL_MDSS_ErrorType HAL_DP_Phy_Poll_Ready(uintPtr uRegAddr, uint32 uMask)
{
  HAL_MDSS_ErrorType    eStatus            = HAL_MDSS_STATUS_SUCCESS;
  uint32                uNumReadsRemaining = HAL_DP_PLL_POLL_MAX_READS;
  uint32                uRegVal            = 0;

  do
  {
    HAL_DP_SLEEP_US(HAL_DP_PLL_POLL_TIMEOUT);
    uRegVal = DP_PHY_REGREAD(uRegAddr);
    uRegVal &= uMask;
    uNumReadsRemaining--;
  } while ((!uRegVal) && (uNumReadsRemaining));

  if (0 == uRegVal)
  {
    eStatus = HAL_MDSS_DP_STATUS_FAILED;
  }

  return eStatus;
}

/****************************************************************************
*
** FUNCTION: HAL_DP_PHY_LaneCountConfig()
*/
/*!
* \brief   
*     Configure PHY lanes depending on the number of lanes requested
*
* \param [in]  uOffset   - Base register for DP PHY device
*        [in]  uNumLanes - Number of lanes to be enabled
*
* \retval - None
*
****************************************************************************/
static void HAL_DP_PHY_LaneCountConfig(uintPtr uOffset, uint32 uNumLanes)
{
  if (1 == uNumLanes)
  {
    DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_LANE_TX0_TRANSCEIVER_BIAS_EN_ADDR,       0x01);
    DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_LANE_TX1_TRANSCEIVER_BIAS_EN_ADDR,       0x00);
    DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_LANE_TX0_HIGHZ_DRVR_EN_ADDR,             0x06);
    DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_LANE_TX1_HIGHZ_DRVR_EN_ADDR,             0x07);
    DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_DP_PHY_CFG_1_ADDR,                       0x01);
  }
  else if (2 == uNumLanes)
  {
    DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_LANE_TX0_TRANSCEIVER_BIAS_EN_ADDR,       0x03);
    DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_LANE_TX1_TRANSCEIVER_BIAS_EN_ADDR,       0x00);
    DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_LANE_TX0_HIGHZ_DRVR_EN_ADDR,             0x04);
    DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_LANE_TX1_HIGHZ_DRVR_EN_ADDR,             0x07);
    DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_DP_PHY_CFG_1_ADDR,                       0x03);
  }
  else
  {
    // Default: 4 lane configuration
    DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_LANE_TX0_TRANSCEIVER_BIAS_EN_ADDR,       0x03);
    DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_LANE_TX1_TRANSCEIVER_BIAS_EN_ADDR,       0x03);
    DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_LANE_TX0_HIGHZ_DRVR_EN_ADDR,             0x04);
    DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_LANE_TX1_HIGHZ_DRVR_EN_ADDR,             0x04);
    DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_DP_PHY_CFG_1_ADDR,                       0x0f);
  }
}

/****************************************************************************
*
** FUNCTION: HAL_DP_RefgenConfig()
*/
/*!
* \brief   
*     Configure PHY lanes depending on the number of lanes requested
*
* \param [in]  eDeviceId - DP device ID
*
* \retval - HAL_MDSS_ErrorType
*
****************************************************************************/
static HAL_MDSS_ErrorType HAL_DP_RefgenConfig(DP_DeviceIDType eDeviceId)
{
  HAL_MDSS_ErrorType eStatus            = HAL_MDSS_STATUS_SUCCESS;
  uintPtr            uOffset            = HAL_DP_PHY_GetRegBaseOffset(eDeviceId);
  uint32             uRegVal            = 0;
  uint32             uMaxRetries        = 2;

  //Refgen
  DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_DP_PHY_MODE_ADDR, 0xfc);

  //Wait for Refgen ready
  do
  {
    HAL_DP_SLEEP_US(HAL_DP_REFGEN_TIMEOUT);
    uRegVal = DP_PHY_REGREAD(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_QSERDES_COM_CMN_STATUS_ADDR);
    uRegVal &= HWIO_AHB2EDPPHY_CM_DPPHY2_EDP_QSERDES_COM_CMN_STATUS_REFGEN_READY_BMSK;
  } while ((!uRegVal) && (uMaxRetries--));

  if (0 == uRegVal)
  {
    eStatus = HAL_MDSS_DP_STATUS_FAILED;
  }
  else
  {
    uint32 uLdoConfig = 0x0;

    if (DP_DEVICE_ID_EDP == eDeviceId)
    {
      uLdoConfig = 0x1;
    }

    DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_LANE_TX0_LDO_CONFIG_ADDR, uLdoConfig);
    DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_LANE_TX1_LDO_CONFIG_ADDR, uLdoConfig);
    DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_LANE_TX0_LANE_MODE_1_ADDR, 0x0);
    DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_LANE_TX1_LANE_MODE_1_ADDR, 0x0);
  }

  return eStatus;
}

/* -----------------------------------------------------------------------
** Public Functions
** ----------------------------------------------------------------------- */

/****************************************************************************
*
** FUNCTION: HAL_DP_1_2_0_PHY_Config()
*/
/*!
* \DESCRIPTION
*   Configure DP PHY and PLL based on desired rates
*
* \param [in]   eDeviceId         - DP device ID
* \param [in]   pDPPllConfigInfo  - DP PHY PLL config info
*
* \retval boolean  TRUE = success, FALSE = fail
*
****************************************************************************/
bool32 HAL_DP_1_2_0_PHY_Config(DP_DeviceIDType        eDeviceId,
                               HAL_DP_PllConfigType  *pDPPllConfigInfo)
{
  HAL_MDSS_ErrorType     eStatus    = HAL_MDSS_STATUS_SUCCESS;
  uintPtr                uOffset    = HAL_DP_PHY_GetRegBaseOffset(eDeviceId);

  if (0 != (pDPPllConfigInfo->uFlags & HAL_DP_PLLCONFIG_FLAG_PLL))
  {
    DP_LOG_MESSAGE_WARN("HAL_DP_1_2_0_PHY_Config: Configuring PLL only not supported\n");
  }
  else
  {
    EDP_PHYHwSettingsType *pSettings  = NULL;
    uint32                 uFreqIndex;
    
    for (uFreqIndex = 0; uFreqIndex < EDP_PHY_RATE_MAX; ++uFreqIndex)
    { 
      if ((gEDPPhyHwSettings[uFreqIndex].uLinkRatePerLaneMbps == pDPPllConfigInfo->uLinkRateMbps) &&
          (gEDPPhyHwSettings[uFreqIndex].uVcoDividedClkMhz == pDPPllConfigInfo->uPixelClkMNDInputMhz))
      {
        pSettings = &gEDPPhyHwSettings[uFreqIndex];
        break;
      }
    }
    
    if (NULL != pSettings)
    { 
      DP_PHY_RateParams *pRateParams = &pSettings->sRateParams;
      DP_PHY_SSCParams  *pSSCParams  = &gEDPSSCParams[uFreqIndex];
    
      // SSC config
      if (HAL_DP_PLLCONFIG_FLAG_SSC & pDPPllConfigInfo->uFlags)
      {
        DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_QSERDES_COM_SSC_EN_CENTER_ADDR,        0x01);
        DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_QSERDES_COM_SSC_ADJ_PER1_ADDR,         (uint32) pSSCParams->uAdjPer1);
        DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_QSERDES_COM_SSC_PER1_ADDR,             (uint32) pSSCParams->uSSCPer1);
        DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_QSERDES_COM_SSC_PER2_ADDR,             (uint32) pSSCParams->uSSCPer2);
        DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_QSERDES_COM_SSC_STEP_SIZE1_MODE0_ADDR, (uint32) pSSCParams->uStepSize1);
        DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_QSERDES_COM_SSC_STEP_SIZE2_MODE0_ADDR, (uint32) pSSCParams->uStepSize2);
      }
      else
      {
        DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_QSERDES_COM_SSC_EN_CENTER_ADDR,        0x00);
        DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_QSERDES_COM_SSC_ADJ_PER1_ADDR,         0x00);
        DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_QSERDES_COM_SSC_PER1_ADDR,             0x00);
        DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_QSERDES_COM_SSC_PER2_ADDR,             0x00);
        DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_QSERDES_COM_SSC_STEP_SIZE1_MODE0_ADDR, 0x00);
        DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_QSERDES_COM_SSC_STEP_SIZE2_MODE0_ADDR, 0x00);
      }
    
      // Mainlink configuration
      DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_QSERDES_COM_SVS_MODE_CLK_SEL_ADDR,       0x01);
      DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_QSERDES_COM_SYSCLK_EN_SEL_ADDR,          0x0b);
      DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_QSERDES_COM_SYS_CLK_CTRL_ADDR,           0x02);
      DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_QSERDES_COM_CLK_ENABLE1_ADDR,            0x0c);
      DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_QSERDES_COM_SYSCLK_BUF_ENABLE_ADDR,      0x06);
      DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_QSERDES_COM_CLK_SELECT_ADDR,             0x30);
      DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_QSERDES_COM_HSCLK_SEL_ADDR,              (uint32) pRateParams->uHsclkSel);
      DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_QSERDES_COM_PLL_IVCO_ADDR,               0x0F);
      DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_QSERDES_COM_LOCK_CMP_EN_ADDR,            0x08);
      DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_QSERDES_COM_PLL_CCTRL_MODE0_ADDR,        0x36);
      DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_QSERDES_COM_PLL_RCTRL_MODE0_ADDR,        0x16);
      DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_QSERDES_COM_CP_CTRL_MODE0_ADDR,          0x06);
      DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_QSERDES_COM_DEC_START_MODE0_ADDR,        (uint32) pRateParams->uDecStart);
      DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_QSERDES_COM_DIV_FRAC_START1_MODE0_ADDR,  (uint32) pRateParams->uDivFracStart1);
      DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_QSERDES_COM_DIV_FRAC_START2_MODE0_ADDR,  (uint32) pRateParams->uDivFracStart2);
      DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_QSERDES_COM_DIV_FRAC_START3_MODE0_ADDR,  (uint32) pRateParams->uDivFracStart3);
      DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_QSERDES_COM_CMN_CONFIG_ADDR,             0x02);
      DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_QSERDES_COM_INTEGLOOP_GAIN0_MODE0_ADDR,  0x3f);
      DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_QSERDES_COM_INTEGLOOP_GAIN1_MODE0_ADDR,  0x00);
      DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_QSERDES_COM_VCO_TUNE_MAP_ADDR,           0x00);
      DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_QSERDES_COM_LOCK_CMP1_MODE0_ADDR,        (uint32) pRateParams->uLockCmp1);
      DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_QSERDES_COM_LOCK_CMP2_MODE0_ADDR,        (uint32) pRateParams->uLockCmp2);
      DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_QSERDES_COM_BG_TIMER_ADDR,               0x0a);
      DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_QSERDES_COM_CORECLK_DIV_MODE0_ADDR,      0x14);
      DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_QSERDES_COM_VCO_TUNE_CTRL_ADDR,          0x00);
      DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_QSERDES_COM_BIAS_EN_CLKBUFLR_EN_ADDR,    0x17);
      DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_QSERDES_COM_CORE_CLK_EN_ADDR,            0x0f);
      DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_QSERDES_COM_VCO_TUNE1_MODE0_ADDR,        (uint32) pRateParams->uVcoTune1);
      DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_QSERDES_COM_VCO_TUNE2_MODE0_ADDR,        (uint32) pRateParams->uVcoTune2);
    
      // Lane configuratoin
      DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_DP_PHY_TX0_TX1_LANE_CTL_ADDR,            0x05); 
      DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_DP_PHY_TX2_TX3_LANE_CTL_ADDR,            0x05); 
      DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_LANE_TX0_TRANSCEIVER_BIAS_EN_ADDR,       0x03);
      DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_LANE_TX0_CLKBUF_ENABLE_ADDR,             0x0f);
      DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_LANE_TX0_RESET_TSYNC_EN_ADDR,            0x03);
      DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_LANE_TX0_TRAN_DRVR_EMP_EN_ADDR,          0x01);
      DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_LANE_TX0_TX_BAND_ADDR,                   0x04);
      DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_LANE_TX1_TRANSCEIVER_BIAS_EN_ADDR,       0x03);
      DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_LANE_TX1_CLKBUF_ENABLE_ADDR,             0x0f);
      DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_LANE_TX1_RESET_TSYNC_EN_ADDR,            0x03);
      DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_LANE_TX1_TRAN_DRVR_EMP_EN_ADDR,          0x01);
      DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_LANE_TX1_TX_BAND_ADDR,                   0x04);
      DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_DP_PHY_VCO_DIV_ADDR,                     (uint32) pRateParams->uPhyVcoDiv);
      DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_DP_PHY_CFG_ADDR,                         0x01);
      DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_DP_PHY_CFG_ADDR,                         0x05);
      DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_DP_PHY_CFG_ADDR,                         0x01);
      DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_DP_PHY_CFG_ADDR,                         0x09);
      DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_QSERDES_COM_RESETSM_CNTRL_ADDR,          0x20);
    
    
      /* poll C_READY status */
      eStatus = HAL_DP_Phy_Poll_Ready(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_QSERDES_COM_C_READY_STATUS_ADDR,
                                      HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_QSERDES_COM_C_READY_STATUS_C_READY_BMSK);
      if (HAL_MDSS_STATUS_SUCCESS != eStatus)
      {
         DP_LOG_MESSAGE_WARN("HAL_DP_1_2_0_PHY_Config: Polling C_READY failed\n");
      } 
    
      DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_DP_PHY_CFG_ADDR,                         0x19);
      DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_LANE_TX0_HIGHZ_DRVR_EN_ADDR,             0x1f);
      DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_LANE_TX0_HIGHZ_DRVR_EN_ADDR,             0x04);
      DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_LANE_TX0_TX_POL_INV_ADDR,                0x00);
      DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_LANE_TX1_HIGHZ_DRVR_EN_ADDR,             0x1f);
      DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_LANE_TX1_HIGHZ_DRVR_EN_ADDR,             0x04);
      DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_LANE_TX1_TX_POL_INV_ADDR,                0x00);
      DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_LANE_TX1_HIGHZ_DRVR_EN_ADDR,             0x04);
      DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_LANE_TX1_TX_POL_INV_ADDR,                0x00);
    
      DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_LANE_TX0_TX_DRV_LVL_OFFSET_ADDR,         0x10);
      DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_LANE_TX1_TX_DRV_LVL_OFFSET_ADDR,         0x10);
      DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_LANE_TX0_RES_CODE_LANE_OFFSET_TX0_ADDR,  0x11);
      DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_LANE_TX0_RES_CODE_LANE_OFFSET_TX1_ADDR,  0x11);
      DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_LANE_TX1_RES_CODE_LANE_OFFSET_TX0_ADDR,  0x11);
      DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_LANE_TX1_RES_CODE_LANE_OFFSET_TX1_ADDR,  0x11);
	  
      /* premphesis and voltage swing , 0 Db */
      DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_LANE_TX0_TX_EMP_POST1_LVL_ADDR,          0x20);
      DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_LANE_TX1_TX_EMP_POST1_LVL_ADDR,          0x20);
      DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_LANE_TX0_TX_DRV_LVL_ADDR,                0x27);
      DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_LANE_TX1_TX_DRV_LVL_ADDR,                0x27);
    
      /* Num of lanes */
      HAL_DP_PHY_LaneCountConfig(uOffset, pDPPllConfigInfo->uNumLanes);
    
      /* poll for phy ready */
      eStatus = HAL_DP_Phy_Poll_Ready(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_DP_PHY_STATUS_ADDR,
                                      HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_DP_PHY_STATUS_PHY_READY_BMSK);
    
      if (HAL_MDSS_STATUS_SUCCESS != eStatus)
      {
        DP_LOG_MESSAGE_WARN("HAL_DP_1_2_0_PHY_Config: Polling PHY_READY failed\n");
      }
    }
    else
    {
      /* Requested settings not supported or config failed : Power down DP PHY*/
      DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_DP_PHY_PD_CTL_ADDR,                      0x02);
    }
  }

  return (eStatus==HAL_MDSS_STATUS_SUCCESS);
}


/*********************************************************************************************
*
** FUNCTION: HAL_DP_1_2_0_0_PHY_Reset()
*/
/*!
* \brief
*        Reset and initialize DP PHY 
*
* \param [in]  eDeviceId - DP device ID
*
* \retval 
*
**********************************************************************************************/
void HAL_DP_1_2_0_PHY_Reset(DP_DeviceIDType      eDeviceId, 
                            DP_PHYResetFlagType  eModules)
{
  // Not suppored
}

/****************************************************************************
*
** FUNCTION: HAL_DP_PHY_SetPHYMode()
*/
/*!
* \DESCRIPTION
*           Set the PHY Mode to DP or USB SS
*
* \param [in]  eDeviceId - DP device ID
* \param [in]  ePHYMode  - PHY mode
*
* \retval
*
****************************************************************************/
void HAL_DP_1_2_0_PHY_SetPHYMode(DP_DeviceIDType eDeviceId,
                                 DP_PHYModeType  ePHYMode)
{
  // Not valid for eDP/DP

}
/*********************************************************************************************
*
** FUNCTION: HAL_DP_1_2_0_PHY_AUX_Init()
*/
/*!
* \brief
*        Initialize DP AUX PHY 
*
* \param [in]  eDeviceId - DP device ID
*
* \retval 
*
**********************************************************************************************/
void HAL_DP_1_2_0_PHY_AUX_Init(DP_DeviceIDType eDeviceId)
{
  uintPtr               uOffset    = HAL_DP_PHY_GetRegBaseOffset(eDeviceId);

  HAL_DP_RefgenConfig(eDeviceId);

  // Power on
  DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_DP_PHY_PD_CTL_ADDR, 0x02); // Power down whole DP PHY
  DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_DP_PHY_PD_CTL_ADDR, 0x7d); // Power on DP PHY

  // Turn on BIAS current for PHY/PLL
  DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_QSERDES_COM_BIAS_EN_CLKBUFLR_EN_ADDR, 0x17);

  // Recommended settings
  DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_DP_PHY_AUX_CFG1_ADDR, 0x13);
  DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_DP_PHY_AUX_CFG2_ADDR, 0x24);
  DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_DP_PHY_AUX_CFG3_ADDR, 0x00);
  DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_DP_PHY_AUX_CFG4_ADDR, 0x0a);
  DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_DP_PHY_AUX_CFG5_ADDR, 0x26);
  DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_DP_PHY_AUX_CFG6_ADDR, 0x0a);
  DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_DP_PHY_AUX_CFG7_ADDR, 0x03);

  if (DP_DEVICE_ID_EDP == eDeviceId)
  {
    DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_DP_PHY_AUX_CFG8_ADDR, 0x37);
  }
  else
  {
    DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_DP_PHY_AUX_CFG8_ADDR, 0xb7);
  }
  DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_DP_PHY_AUX_CFG9_ADDR, 0x03);

  // Interrupts
  DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_DP_PHY_AUX_INTERRUPT_MASK_ADDR, 0x00); /* disable interrupts */
}

/*********************************************************************************************
*
** FUNCTION: HAL_DP_1_2_0_PHY_SetupPeSw()
*/
/*!
* \brief
*        Set up DP Lane Pre Emphasis and voltage swing level
*
* \param [in]     eDeviceId   - DP device ID
* \param [in/out] pPeSwConfig - Pre-emphasis/Swing level settings.
*
* \retval  HAL_MDSS_ErrorType
*
**********************************************************************************************/
HAL_MDSS_ErrorType HAL_DP_1_2_0_PHY_SetupPeSw(DP_DeviceIDType        eDeviceId,
                                              HAL_DP_PeSwConfigType *pPeSwConfig)
{
  HAL_MDSS_ErrorType  eStatus      = HAL_MDSS_STATUS_FAILED_NOT_SUPPORTED;
  uintPtr               uOffset    = HAL_DP_PHY_GetRegBaseOffset(eDeviceId);

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
      DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_LANE_TX0_TX_DRV_LVL_ADDR,       uVoltageSwingReg);
      DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_LANE_TX0_TX_EMP_POST1_LVL_ADDR, uPreEmphasisReg);

      DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_LANE_TX1_TX_DRV_LVL_ADDR,       uVoltageSwingReg);
      DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_LANE_TX1_TX_EMP_POST1_LVL_ADDR,  uPreEmphasisReg);

       eStatus = HAL_MDSS_STATUS_SUCCESS;
    }
  }

  return eStatus;
}

/*********************************************************************************************
*
** FUNCTION: HAL_DP_1_2_0_PHY_GetMaxPeSwLevel()
*/
/*!
* \brief
*        Get max supported DP Lane Pre Emphasis and voltage swing levels
*
* \param [in]  eDeviceId        - DP device ID
* \param [out] uMaxPreemphLvl   - max Pre-emphasis level supported.
* \param [out] uMaxVoltSwingLvl - max voltage swing level supported.
*
* \retval  void
*
**********************************************************************************************/
void HAL_DP_1_2_0_PHY_GetMaxPeSwLevel(DP_DeviceIDType  eDeviceId,
                                      uint32          *uMaxPreemphLvl,
                                      uint32          *uMaxVoltSwingLvl)
{
  *uMaxPreemphLvl   = DP_PREEMPHASIS_LEVEL_NUM - 1;
  *uMaxVoltSwingLvl = DP_VOLTAGESWING_LEVEL_NUM - 1;
}

/*********************************************************************************************
*
** FUNCTION: HAL_DP_1_2_0_PHY_AUXConfig()
*/
/*!
* \brief
*        Config AUX PHY registers
*
* \param [in] eDeviceId  - DP device ID
* \param [in] pAuxPhyCfg - AUX PHY settings.
*
* \retval  HAL_MDSS_ErrorType
*
**********************************************************************************************/
HAL_MDSS_ErrorType HAL_DP_1_2_0_PHY_AUXConfig(DP_DeviceIDType       eDeviceId,
                                              HAL_DP_AuxPhyCfgType *pAuxPhyCfg)
{
   HAL_MDSS_ErrorType eStatus = HAL_MDSS_STATUS_FAILED_NOT_SUPPORTED;
   uintPtr            uOffset = HAL_DP_PHY_GetRegBaseOffset(eDeviceId);

  if ((NULL != pAuxPhyCfg) &&
      (NULL != pAuxPhyCfg->pDPAuxPhyCfg1Index))
  {     
    if (*pAuxPhyCfg->pDPAuxPhyCfg1Index < HAL_DP_LIST_LENGTH(aAuxPhyCfg1Setting))
    {
      DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_DP_PHY_AUX_CFG1_ADDR, aAuxPhyCfg1Setting[*pAuxPhyCfg->pDPAuxPhyCfg1Index]);

      eStatus = HAL_MDSS_STATUS_SUCCESS;
    }
  }
  
  return eStatus;
}

/*********************************************************************************************
*
** FUNCTION: HAL_DP_1_2_0_PHY_Power()
*/
/*!
* \brief
*        Config AUX PHY registers
*
* \param [in]   eDeviceId    - DP Device ID
* \param [in]   bPowerOn     - TRUE: power on, FALSE: power off
*
* \retval  void
*
**********************************************************************************************/
void HAL_DP_1_2_0_PHY_Power(DP_DeviceIDType eDeviceId, bool32 bPowerOn)
{
  uintPtr            uOffset = HAL_DP_PHY_GetRegBaseOffset(eDeviceId);

  if (FALSE == bPowerOn)
  {
    // Power down whole DP PHY
    DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_DP_PHY_PD_CTL_ADDR, 0x02);
  }
  else
  {
    // Power up is embedded in DP PHY config function for correct sequence. 
  }
}

/****************************************************************************
*
** FUNCTION: HAL_DP_1_2_0_PHY_GetPLLStatus()
*/
/*!
* \DESCRIPTION
*           Get DP PLL status
*
* \param [in]     eDeviceId   - DP Device ID
* \param [out]    pbPLLLocked - TRUE is DP PLL is locked, otherwise FALSE
*
* \retval void
*
****************************************************************************/
void HAL_DP_1_2_0_PHY_GetPLLStatus(DP_DeviceIDType eDeviceId, bool32 *pbPLLLocked)
{
  uint32             uStatus = 0;
  uintPtr            uOffset = HAL_DP_PHY_GetRegBaseOffset(eDeviceId);

  uStatus = DP_PHY_REGREAD(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_DP_PHY_STATUS_ADDR) & 
                           HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_DP_PHY_STATUS_TSYNC_DONE_BMSK;    

  if (NULL != pbPLLLocked)
  {
    *pbPLLLocked = (0 == uStatus) ? FALSE : TRUE;
  }
}

/****************************************************************************
*
** FUNCTION: HAL_DP_PHY_SetPowerState()
*/
/*!
* \DESCRIPTION
*           Set the power state for PHY when switching clock source
*
* \param [in]     eDeviceId - DP Device ID
* \param [in]     bEnabled  - TRUE if the clock source is being set to PLL
*
* \retval
*
****************************************************************************/
void HAL_DP_1_2_0_PHY_SetPowerState(DP_DeviceIDType eDeviceId, bool32 bEnabled)
{
  uintPtr   uOffset   = HAL_DP_PHY_GetRegBaseOffset(eDeviceId);
  uint32    uRegVal   = 0; 

  // Set Retimer state
  uRegVal = DP_PHY_REGREAD(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_DP_PHY_CFG_ADDR);
  uRegVal = HWIO_OUT_FLD(uRegVal, AHB2EDPPHY_CM_EDP_PHY_EDP_DP_PHY_CFG, RETIMING_ENABLE, (TRUE == bEnabled) ? 1 : 0);  
  DP_PHY_REGWRITE(uOffset + HWIO_AHB2EDPPHY_CM_EDP_PHY_EDP_DP_PHY_CFG_ADDR, uRegVal);
}

/****************************************************************************
*
** FUNCTION: HAL_DP_PhyFxnsInit()
*/
/*!
* \DESCRIPTION
*   Initialize DP PHY/PLL function table
*
* \param [in]  pFxnTable - Pointer to DP PHY function table
*
* \retval None
*
****************************************************************************/
void HAL_DP_1_2_0_PhyFxnsInit(HAL_DP_PhyPllFunctionTable *pFxnTable)
{
  pFxnTable->HAL_DP_PHY_Config          = HAL_DP_1_2_0_PHY_Config;
  pFxnTable->HAL_DP_PHY_Reset           = HAL_DP_1_2_0_PHY_Reset;
  pFxnTable->HAL_DP_PHY_AUX_Init        = HAL_DP_1_2_0_PHY_AUX_Init;
  pFxnTable->HAL_DP_PHY_SetupPeSw       = HAL_DP_1_2_0_PHY_SetupPeSw;
  pFxnTable->HAL_DP_PHY_GetMaxPeSwLevel = HAL_DP_1_2_0_PHY_GetMaxPeSwLevel;
  pFxnTable->HAL_DP_PHY_AUXConfig       = HAL_DP_1_2_0_PHY_AUXConfig;
  pFxnTable->HAL_DP_PHY_SetPHYMode      = HAL_DP_1_2_0_PHY_SetPHYMode;
  pFxnTable->HAL_DP_PHY_Power           = HAL_DP_1_2_0_PHY_Power;
  pFxnTable->HAL_DP_PHY_GetPLLStatus    = HAL_DP_1_2_0_PHY_GetPLLStatus;
  pFxnTable->HAL_DP_PHY_SetPowerState   = HAL_DP_1_2_0_PHY_SetPowerState;
}


#ifdef __cplusplus
}
#endif

