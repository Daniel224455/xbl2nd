/*===========================================================================

FILE:         hal_dp_alt_mode_phy_1_2_0.c

DESCRIPTION:  
  This file contains the programming for the DP Phy/PLL (v1.2.0 7nm).

  Copyright (c) 2016-2020 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
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
#include "hal_dp_phy_qmp_pll_7nm.h"
#include "gccreg.h"


/* -----------------------------------------------------------------------
** Macros
** ----------------------------------------------------------------------- */

#define DP_PREEMPHASIS_LEVEL_NUM               3
#define DP_VOLTAGESWING_LEVEL_NUM              3
#define DP_AUX_CFG_SETTINGS_LEN                4

#define DP_PHY_REGREAD(_a_)                   in_dword((_a_))
#define DP_PHY_REGWRITE(_a_, _d_)             out_dword((_a_), (_d_))

/* -----------------------------------------------------------------------
** Structure definitions
** ----------------------------------------------------------------------- */



/* -----------------------------------------------------------------------
** Static Variables
** ----------------------------------------------------------------------- */

/* Pre-emphasis and Voltage swing combinations supported by hardware */
static HAL_DP_PeSwPairType aPeSwPair[DP_VOLTAGESWING_LEVEL_NUM][DP_PREEMPHASIS_LEVEL_NUM] =
{
  { { 0x20, 0x27 }, { 0x2E, 0x2F }, { 0x35, 0x36 } }, /* SW0_PE0, SW0_PE1, SW0_PE2  */
  { { 0x20, 0x31 }, { 0x2E, 0x3E }, { 0x35, 0x3F } }, /* SW1_PE0, SW1_PE1, SW1_PE2  */
  { { 0x20, 0x3A }, { 0x2E, 0x3F }, { 0x00, 0x00 } }, /* SW2_PE0, SW2_PE1, N/A     */
};

/*DP_PHY_AUX_CFG1 register settings */
static uint32 aAuxPhyCfg1Setting[DP_AUX_CFG_SETTINGS_LEN] = { 0x20, 0x13, 0x23, 0x1d };


/* -----------------------------------------------------------------------
** Function Prototypes
** ----------------------------------------------------------------------- */
static HAL_MDSS_ErrorType HAL_DP_AltMode_Phy_Poll_PhyReady(DP_DeviceIDType eDeviceId, uint32);
static HAL_MDSS_ErrorType HAL_DP_AltMode_Phy_Poll_CReady(DP_DeviceIDType eDeviceId);
HAL_MDSS_ErrorType HAL_DP_AltMode_1_2_0_PHY_SetupPeSw(DP_DeviceIDType eDeviceId, HAL_DP_PeSwConfigType *pPeSwConfig);

/* -----------------------------------------------------------------------
** Private Functions
** ----------------------------------------------------------------------- */

/****************************************************************************
*
** FUNCTION: HAL_DP_AltMode_GetRegBaseOffset()
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
uintPtr HAL_DP_AltMode_GetRegBaseOffset(DP_DeviceIDType  eDeviceId)
{
  uintPtr   uOffset;

  switch (eDeviceId)
  {
  case DP_DEVICE_ID_ALT_MODE_0:
    {
      uOffset = 0;
      break;
    }

  case DP_DEVICE_ID_ALT_MODE_1:
    {
      uOffset = (AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_DP_PHY_SECCM_USB3_DP_CON_SW_REG_BASE - 
                 AHB2PHY_SOUTH_USB3_DP_PHY_USB3_DP_COM_USB3_DP_COM_USB3_DP_COM_REG_BASE);
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
** FUNCTION: HAL_DP_AltMode_Phy_Pll_Config()
*/
/*!
* \DESCRIPTION
*     Programs Phy/Pll registers.
*
* \param [in]  eDeviceId         - DP device ID
* \param [in]  pDPPllConfigInfo  - Config to apply
*
* \retval HAL_DP_StatusType
*
**********************************************************************************************/
static HAL_MDSS_ErrorType HAL_DP_AltMode_Phy_Pll_Config(DP_DeviceIDType       eDeviceId,
                                                        HAL_DP_PllConfigType *pDPPllConfigInfo)
{
  HAL_MDSS_ErrorType    eStatus     = HAL_MDSS_STATUS_SUCCESS;
  uintPtr               uOffset     = HAL_DP_AltMode_GetRegBaseOffset(eDeviceId);

  if (0 != (pDPPllConfigInfo->uFlags & HAL_DP_PLLCONFIG_FLAG_PLL))
  {
    // Power up PLL, L0/1/2/3 off, AUX off, ungated all clocks, power up all analog blocks
    DP_PHY_REGWRITE(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_DP_PHY_PD_CTL_ADDR,               0x61); 
    
    /* Configure QMP PLL */
    eStatus = HAL_DP_PHY_Qmp_Pll_7nm_Config(uOffset, pDPPllConfigInfo);
    
    DP_PHY_REGWRITE(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_DP_PHY_TX0_TX1_LANE_CTL_ADDR,     0x01);
    DP_PHY_REGWRITE(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_DP_PHY_TX2_TX3_LANE_CTL_ADDR,     0x01);
	
    /* PLL reset sequence */
    DP_PHY_REGWRITE(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_DP_PHY_CFG_ADDR,                  0x01);
    DP_PHY_REGWRITE(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_DP_PHY_CFG_ADDR,                  0x05);
    DP_PHY_REGWRITE(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_DP_PHY_CFG_ADDR,                  0x01);
    DP_PHY_REGWRITE(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_DP_PHY_CFG_ADDR,                  0x09);
    DP_PHY_REGWRITE(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_QSERDES_COM_RESETSM_CNTRL_ADDR,   0x20);
    
    // Check if power up is done
    eStatus = HAL_DP_AltMode_Phy_Poll_CReady(eDeviceId);
  }
  else
  {
    bool32 bPortSelect = pDPPllConfigInfo->bInvertedPlug;

    /*
     * NOTE: SW_PORTSELECT (bit 0) of USB3_DP_COM_TYPE_CTRL is shared with usb driver 
     */
    if (FALSE == bPortSelect)
    {
      DP_PHY_REGWRITE(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_DP_COM_TYPEC_CTRL_ADDR,     0x02);
    }
    else
    {
      DP_PHY_REGWRITE(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_DP_COM_TYPEC_CTRL_ADDR,     0x03);
    }
    DP_PHY_REGWRITE(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_DP_COM_SWI_CTRL_ADDR,         0x00);
    DP_PHY_REGWRITE(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_DP_COM_SW_RESET_ADDR,         0x00);
    DP_PHY_REGWRITE(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_DP_COM_POWER_DOWN_CTRL_ADDR,  0x01);
    
    // Power up PLL, L0/1/2/3, AUX, ungated all clocks, power up all analog blocks
    if (4 == pDPPllConfigInfo->uNumLanes)
    {
      DP_PHY_REGWRITE(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_DP_PHY_PD_CTL_ADDR,           0x7d);
    }
    else
    {
      if (FALSE == bPortSelect)
      {
        DP_PHY_REGWRITE(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_DP_PHY_PD_CTL_ADDR,         0x75);
      }
      else
      {
        DP_PHY_REGWRITE(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_DP_PHY_PD_CTL_ADDR,         0x6d);
      }
    }
    
    if (FALSE == bPortSelect)
    {
      DP_PHY_REGWRITE(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_DP_PHY_MODE_ADDR,         0x5c);
    }
    else
    {
      DP_PHY_REGWRITE(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_DP_PHY_MODE_ADDR,         0x4c);
    }
    
    /* Configure QMP PLL */
    eStatus = HAL_DP_PHY_Qmp_Pll_7nm_Config(uOffset, pDPPllConfigInfo);
    
    DP_PHY_REGWRITE(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_DP_PHY_TX0_TX1_LANE_CTL_ADDR,             0x05);
    DP_PHY_REGWRITE(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_DP_PHY_TX2_TX3_LANE_CTL_ADDR,             0x05);
    
    DP_PHY_REGWRITE(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_QSERDES_TX0_TRANSCEIVER_BIAS_EN_ADDR,     0x1A);
    DP_PHY_REGWRITE(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_QSERDES_TX0_VMODE_CTRL1_ADDR,             0x40);
    DP_PHY_REGWRITE(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_QSERDES_TX0_PRE_STALL_LDO_BOOST_EN_ADDR,  0x30);
    DP_PHY_REGWRITE(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_QSERDES_TX0_INTERFACE_SELECT_ADDR,        0x3b);
    DP_PHY_REGWRITE(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_QSERDES_TX0_CLKBUF_ENABLE_ADDR,           0x0f);
    DP_PHY_REGWRITE(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_QSERDES_TX0_RESET_TSYNC_EN_ADDR,          0x03);
    DP_PHY_REGWRITE(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_QSERDES_TX0_TRAN_DRVR_EMP_EN_ADDR,        0x0f);
    DP_PHY_REGWRITE(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_QSERDES_TX0_PARRATE_REC_DETECT_IDLE_EN_ADDR, 0x00);
    DP_PHY_REGWRITE(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_QSERDES_TX0_TX_INTERFACE_MODE_ADDR,       0x00);
    DP_PHY_REGWRITE(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_QSERDES_TX0_RES_CODE_LANE_OFFSET_TX_ADDR, 0x11);
    DP_PHY_REGWRITE(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_QSERDES_TX0_RES_CODE_LANE_OFFSET_RX_ADDR, 0x11);
    DP_PHY_REGWRITE(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_QSERDES_TX0_TX_BAND_ADDR,                 0x04);
    
    DP_PHY_REGWRITE(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_QSERDES_TX1_TRANSCEIVER_BIAS_EN_ADDR,     0x1A);
    DP_PHY_REGWRITE(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_QSERDES_TX1_VMODE_CTRL1_ADDR,             0x40);
    DP_PHY_REGWRITE(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_QSERDES_TX1_PRE_STALL_LDO_BOOST_EN_ADDR,  0x30);
    DP_PHY_REGWRITE(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_QSERDES_TX1_INTERFACE_SELECT_ADDR,        0x3b);
    DP_PHY_REGWRITE(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_QSERDES_TX1_CLKBUF_ENABLE_ADDR,           0x0f);
    DP_PHY_REGWRITE(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_QSERDES_TX1_RESET_TSYNC_EN_ADDR,          0x03);
    DP_PHY_REGWRITE(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_QSERDES_TX1_TRAN_DRVR_EMP_EN_ADDR,        0x0f);
    DP_PHY_REGWRITE(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_QSERDES_TX1_PARRATE_REC_DETECT_IDLE_EN_ADDR, 0x00);
    DP_PHY_REGWRITE(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_QSERDES_TX1_TX_INTERFACE_MODE_ADDR,       0x00);
    DP_PHY_REGWRITE(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_QSERDES_TX1_RES_CODE_LANE_OFFSET_TX_ADDR, 0x11);
    DP_PHY_REGWRITE(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_QSERDES_TX1_RES_CODE_LANE_OFFSET_RX_ADDR, 0x11);
    DP_PHY_REGWRITE(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_QSERDES_TX1_TX_BAND_ADDR,                 0x04);
    DP_PHY_REGWRITE(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_QSERDES_TX1_TRANSCEIVER_BIAS_EN_ADDR,     0x3f);
    
    DP_PHY_REGWRITE(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_DP_PHY_CFG_ADDR,                          0x01);
    DP_PHY_REGWRITE(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_DP_PHY_CFG_ADDR,                          0x05);
    DP_PHY_REGWRITE(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_DP_PHY_CFG_ADDR,                          0x01);
    DP_PHY_REGWRITE(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_DP_PHY_CFG_ADDR,                          0x09);
    
    out_dword(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_QSERDES_COM_RESETSM_CNTRL_ADDR,                 0x20);
    
    // Check if power up is done
    eStatus = HAL_DP_AltMode_Phy_Poll_CReady(eDeviceId);
    
    DP_PHY_REGWRITE(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_DP_PHY_CFG_ADDR,                          0x19);
    if (HAL_MDSS_STATUS_SUCCESS == eStatus)
    {
      eStatus = HAL_DP_AltMode_Phy_Poll_PhyReady(eDeviceId, HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_DP_PHY_STATUS_TSYNC_DONE_BMSK);
    }
    
    if (4 == pDPPllConfigInfo->uNumLanes)
    {
      DP_PHY_REGWRITE(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_QSERDES_TX0_HIGHZ_DRVR_EN_ADDR,         0x10);
      DP_PHY_REGWRITE(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_QSERDES_TX0_TRANSCEIVER_BIAS_EN_ADDR,   0x3f);
      DP_PHY_REGWRITE(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_QSERDES_TX1_HIGHZ_DRVR_EN_ADDR,         0x10);
      DP_PHY_REGWRITE(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_QSERDES_TX1_TRANSCEIVER_BIAS_EN_ADDR,   0x3f);
    }
    else if (2 == pDPPllConfigInfo->uNumLanes)
    {
      if (FALSE == bPortSelect)
      {
        DP_PHY_REGWRITE(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_QSERDES_TX1_HIGHZ_DRVR_EN_ADDR,       0x10);
        DP_PHY_REGWRITE(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_QSERDES_TX1_TRANSCEIVER_BIAS_EN_ADDR, 0x3f);
      }
      else
      {
        DP_PHY_REGWRITE(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_QSERDES_TX0_HIGHZ_DRVR_EN_ADDR,       0x10);
        DP_PHY_REGWRITE(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_QSERDES_TX0_TRANSCEIVER_BIAS_EN_ADDR, 0x3f);
      }
    }
    else if (1 == pDPPllConfigInfo->uNumLanes)
    {
      if (FALSE == bPortSelect)
      {
        DP_PHY_REGWRITE(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_QSERDES_TX1_HIGHZ_DRVR_EN_ADDR,       0x13);
        DP_PHY_REGWRITE(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_QSERDES_TX1_TRANSCEIVER_BIAS_EN_ADDR, 0x3e);
      }
      else
      {
        DP_PHY_REGWRITE(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_QSERDES_TX0_HIGHZ_DRVR_EN_ADDR,       0x13);
        DP_PHY_REGWRITE(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_QSERDES_TX0_TRANSCEIVER_BIAS_EN_ADDR, 0x3e);
      }
    }
    
    DP_PHY_REGWRITE(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_QSERDES_TX0_TX_POL_INV_ADDR,              0x0a);
    DP_PHY_REGWRITE(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_QSERDES_TX1_TX_POL_INV_ADDR,              0x0a);
    DP_PHY_REGWRITE(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_DP_PHY_CFG_ADDR,                          0x18);
    DP_PHY_REGWRITE(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_DP_PHY_CFG_ADDR,                          0x19);
    
    if (HAL_MDSS_STATUS_SUCCESS == eStatus)
    {
      eStatus = HAL_DP_AltMode_Phy_Poll_PhyReady(eDeviceId, HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_DP_PHY_STATUS_TSYNC_DONE_BMSK);
    }
    
    if (HAL_MDSS_STATUS_SUCCESS == eStatus)
    {
      // Configure pre-emphasis & driver strength - these may be changed during link training
      HAL_DP_PeSwConfigType  sPeSwConfig;
    
      sPeSwConfig.uLaneSwingLevel   = pDPPllConfigInfo->uVoltSwingLevel;
      sPeSwConfig.uPreemphasisLevel = pDPPllConfigInfo->uPreemphasisLevel;

      eStatus = HAL_DP_AltMode_1_2_0_PHY_SetupPeSw(eDeviceId, &sPeSwConfig);
    }
  }

  if (HAL_MDSS_STATUS_SUCCESS != eStatus)
  {
    eStatus = HAL_MDSS_DP_STATUS_FAILED;
  }

  return eStatus;
}

/*********************************************************************************************
*
** FUNCTION: HAL_DP_AltMode_Phy_Poll_PhyReady()
*/
/*!
* \DESCRIPTION
*     poll if Phy PLL status is ready.
*
* \param [in]   eDeviceId  - DP device ID
* \param [in]   uReadyMask - Ready mask to test against
* \param [out]  None
*
* \retval HAL_DP_StatusType
*
**********************************************************************************************/
static HAL_MDSS_ErrorType HAL_DP_AltMode_Phy_Poll_PhyReady(DP_DeviceIDType eDeviceId,
                                                           uint32          uReadyMask)
{
  HAL_MDSS_ErrorType    eStatus            = HAL_MDSS_STATUS_SUCCESS;
  uintPtr               uOffset            = HAL_DP_AltMode_GetRegBaseOffset(eDeviceId);
  uint32                uNumReadsRemaining = HAL_DP_PLL_POLL_MAX_READS;
  uint32                uStatus            = 0;

  /* Poll PHY_READY */
  do
  {
    HAL_DP_SLEEP_US(HAL_DP_PLL_POLL_TIMEOUT);  /* delay */
    uStatus = DP_PHY_REGREAD(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_DP_PHY_STATUS_ADDR) & uReadyMask;
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
** FUNCTION: HAL_DP_AltMode_Phy_Poll_CReady()
*/
/*!
* \DESCRIPTION
*     poll if C_Ready PLL status is ready.
*
* \param [in]  eDeviceId - DP device ID
* \param [out] None
*
* \retval HAL_DP_StatusType
*
**********************************************************************************************/
static HAL_MDSS_ErrorType HAL_DP_AltMode_Phy_Poll_CReady(DP_DeviceIDType eDeviceId)
{
  uintPtr uOffset  = HAL_DP_AltMode_GetRegBaseOffset(eDeviceId);

  return HAL_DP_PHY_Qmp_Pll_7nm_Poll_Cready(uOffset);
}

/* -----------------------------------------------------------------------
** Public Functions
** ----------------------------------------------------------------------- */

/****************************************************************************
*
** FUNCTION: HAL_DP_AltMode_1_2_0_PHY_Config()
*/
/*!
* \DESCRIPTION
*   Configure DP PHY and PLL based on desired rates
*
* \param [in]  eDeviceId        - DP device ID
* \param [in]  pDPPllConfigInfo - DP PHY PLL config info
*
* \retval boolean  TRUE = success, FALSE = fail
*
****************************************************************************/
bool32 HAL_DP_AltMode_1_2_0_PHY_Config(DP_DeviceIDType       eDeviceId,
                                       HAL_DP_PllConfigType *pDPPllConfigInfo)
{
  bool32      bStatus    = FALSE;
  uintPtr     uOffset    = HAL_DP_AltMode_GetRegBaseOffset(eDeviceId);

  /* program DP PLL and PHY */
  bStatus = (HAL_MDSS_STATUS_SUCCESS == HAL_DP_AltMode_Phy_Pll_Config(eDeviceId,
                                                                    pDPPllConfigInfo));

  if (!bStatus)
  {
    /* Requested settings not supported or config failed : Power down */
    DP_PHY_REGWRITE(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_DP_PHY_PD_CTL_ADDR, 0x02);  // Power down whole DP PHY
  }

  return bStatus;
}


/*********************************************************************************************
*
** FUNCTION: HAL_DP_AltMode_1_2_0_0_PHY_Reset()
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
void HAL_DP_AltMode_1_2_0_PHY_Reset(DP_DeviceIDType      eDeviceId,
                                    DP_PHYResetFlagType  eModules)
{
  uint32 uRegVal;

  if (0 != (eModules & DP_PHY_RESET_ALT_MODE_COMMON_RESET))
  {
    switch (eDeviceId)
    {
      case DP_DEVICE_ID_ALT_MODE_0:
      {
        // Disable QMP PIPE3 clock branch
        uRegVal = DP_PHY_REGREAD(HWIO_GCC_USB3_PRIM_PHY_PIPE_CBCR_ADDR);
        uRegVal = HWIO_OUT_FLD(uRegVal, GCC_USB3_PRIM_PHY_PIPE_CBCR, CLK_ENABLE, 0);  
        DP_PHY_REGWRITE(HWIO_GCC_USB3_PRIM_PHY_PIPE_CBCR_ADDR, uRegVal);

        // Reset the PHY block
        uRegVal = DP_PHY_REGREAD(HWIO_GCC_USB3_PHY_PRIM_SP0_BCR_ADDR);
        uRegVal = HWIO_OUT_FLD(uRegVal, GCC_USB3_PHY_PRIM_SP0_BCR, BLK_ARES, 1);  
        DP_PHY_REGWRITE(HWIO_GCC_USB3_PHY_PRIM_SP0_BCR_ADDR, uRegVal);
        HAL_DP_SLEEP_US(10);
        uRegVal = HWIO_OUT_FLD(uRegVal, GCC_USB3_PHY_PRIM_SP0_BCR, BLK_ARES, 0);  
        DP_PHY_REGWRITE(HWIO_GCC_USB3_PHY_PRIM_SP0_BCR_ADDR, uRegVal);
        HAL_DP_SLEEP_US(10);

        // Enable QMP PIPE3 clock branch
        uRegVal = DP_PHY_REGREAD(HWIO_GCC_USB3_PRIM_PHY_PIPE_CBCR_ADDR);
        uRegVal = HWIO_OUT_FLD(uRegVal, GCC_USB3_PRIM_PHY_PIPE_CBCR, CLK_ENABLE, 1);  
        DP_PHY_REGWRITE(HWIO_GCC_USB3_PRIM_PHY_PIPE_CBCR_ADDR, uRegVal);
        break;
      }

      case DP_DEVICE_ID_ALT_MODE_1:
      {
        // Disable QMP PIPE3 clock branch
        uRegVal = DP_PHY_REGREAD(HWIO_GCC_USB3_SEC_PHY_PIPE_CBCR_ADDR);
        uRegVal = HWIO_OUT_FLD(uRegVal, GCC_USB3_SEC_PHY_PIPE_CBCR, CLK_ENABLE, 0);  
        DP_PHY_REGWRITE(HWIO_GCC_USB3_SEC_PHY_PIPE_CBCR_ADDR, uRegVal);

        // Reset the PHY block
        uRegVal = DP_PHY_REGREAD(HWIO_GCC_USB3_PHY_PRIM_SP1_BCR_ADDR);
        uRegVal = HWIO_OUT_FLD(uRegVal, GCC_USB3_PHY_PRIM_SP1_BCR, BLK_ARES, 1);  
        DP_PHY_REGWRITE(HWIO_GCC_USB3_PHY_PRIM_SP1_BCR_ADDR, uRegVal);
        HAL_DP_SLEEP_US(10);
        uRegVal = HWIO_OUT_FLD(uRegVal, GCC_USB3_PHY_PRIM_SP1_BCR, BLK_ARES, 0);  
        DP_PHY_REGWRITE(HWIO_GCC_USB3_PHY_PRIM_SP1_BCR_ADDR, uRegVal);
        HAL_DP_SLEEP_US(10);

        // Enable QMP PIPE3 clock branch
        uRegVal = DP_PHY_REGREAD(HWIO_GCC_USB3_SEC_PHY_PIPE_CBCR_ADDR);
        uRegVal = HWIO_OUT_FLD(uRegVal, GCC_USB3_SEC_PHY_PIPE_CBCR, CLK_ENABLE, 1);  
        DP_PHY_REGWRITE(HWIO_GCC_USB3_SEC_PHY_PIPE_CBCR_ADDR, uRegVal);
        break;
      }
  
      default:
      {
        break;
      }
    }
  }
}

/****************************************************************************
*
** FUNCTION: HAL_DP_AltMode_1_2_0_PHY_SetPHYMode()
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
void HAL_DP_AltMode_1_2_0_PHY_SetPHYMode(DP_DeviceIDType eDeviceId,
                                         DP_PHYModeType  ePHYMode)
{
  uintPtr               uOffset     = HAL_DP_AltMode_GetRegBaseOffset(eDeviceId);

  switch (ePHYMode)
  {
    case DP_PHY_MODE_USB_DATA:
    {
      // USB SS enabled and DP disabled
      DP_PHY_REGWRITE(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_DP_COM_PHY_MODE_CTRL_ADDR, 0x01);
      break;
    }

    case DP_PHY_MODE_DP:
    {
      // USB SS disabled and DP enabled
      DP_PHY_REGWRITE(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_DP_COM_PHY_MODE_CTRL_ADDR, 0x02);
      break;
    }

    case DP_PHY_MODE_CONCURRENCY:
    {
      // USB SS enabled and DP enabled
      DP_PHY_REGWRITE(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_DP_COM_PHY_MODE_CTRL_ADDR, 0x03);
      break;
    }

    default:
    {
      break;
    }
  }
}
/*********************************************************************************************
*
** FUNCTION: HAL_DP_AltMode_1_2_0_PHY_AUX_Init()
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
void HAL_DP_AltMode_1_2_0_PHY_AUX_Init(DP_DeviceIDType eDeviceId)
{
  uintPtr               uOffset     = HAL_DP_AltMode_GetRegBaseOffset(eDeviceId);

  // Power on
  DP_PHY_REGWRITE(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_DP_PHY_PD_CTL_ADDR, 0x02);  // Power down whole DP PHY
  DP_PHY_REGWRITE(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_DP_PHY_PD_CTL_ADDR, 0x67);  // Power on DP PHY

  // #TODO#VERIFY - already done during qmp pll init
  // Turn on BIAS current for PHY/PLL
  DP_PHY_REGWRITE(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_QSERDES_COM_BIAS_EN_CLKBUFLR_EN_ADDR, 0x17);

  // Recommended settings
  DP_PHY_REGWRITE(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_DP_PHY_AUX_CFG1_ADDR, 0x13);
  DP_PHY_REGWRITE(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_DP_PHY_AUX_CFG2_ADDR, 0x24);
  DP_PHY_REGWRITE(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_DP_PHY_AUX_CFG3_ADDR, 0x00);
  DP_PHY_REGWRITE(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_DP_PHY_AUX_CFG4_ADDR, 0x0a);
  DP_PHY_REGWRITE(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_DP_PHY_AUX_CFG5_ADDR, 0x26);
  DP_PHY_REGWRITE(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_DP_PHY_AUX_CFG6_ADDR, 0x0a);
  DP_PHY_REGWRITE(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_DP_PHY_AUX_CFG7_ADDR, 0x03);
  DP_PHY_REGWRITE(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_DP_PHY_AUX_CFG8_ADDR, 0xB7);
  DP_PHY_REGWRITE(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_DP_PHY_AUX_CFG9_ADDR, 0x03);

  /* disable interrupts */
  DP_PHY_REGWRITE(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_DP_PHY_AUX_INTERRUPT_MASK_ADDR, 0x00);

}

/*********************************************************************************************
*
** FUNCTION: HAL_DP_AltMode_1_2_0_PHY_SetupPeSw()
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
HAL_MDSS_ErrorType HAL_DP_AltMode_1_2_0_PHY_SetupPeSw(DP_DeviceIDType        eDeviceId, 
                                                      HAL_DP_PeSwConfigType *pPeSwConfig)
{
  HAL_MDSS_ErrorType  eStatus       = HAL_MDSS_STATUS_FAILED_NOT_SUPPORTED;
  uintPtr             uOffset       = HAL_DP_AltMode_GetRegBaseOffset(eDeviceId);

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
      DP_PHY_REGWRITE(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_QSERDES_TX0_TX_DRV_LVL_ADDR,       uVoltageSwingReg);
      DP_PHY_REGWRITE(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_QSERDES_TX0_TX_EMP_POST1_LVL_ADDR, uPreEmphasisReg);

      DP_PHY_REGWRITE(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_QSERDES_TX1_TX_DRV_LVL_ADDR,       uVoltageSwingReg);
      DP_PHY_REGWRITE(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_QSERDES_TX1_TX_EMP_POST1_LVL_ADDR,  uPreEmphasisReg);

      eStatus = HAL_MDSS_STATUS_SUCCESS;
    }
  }

  return eStatus;
}

/*********************************************************************************************
*
** FUNCTION: HAL_DP_AltMode_1_2_0_PHY_GetMaxPeSwLevel()
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
void HAL_DP_AltMode_1_2_0_PHY_GetMaxPeSwLevel(DP_DeviceIDType  eDeviceId,
                                              uint32          *uMaxPreemphLvl,
                                              uint32          *uMaxVoltSwingLvl)
{
  *uMaxPreemphLvl   = DP_PREEMPHASIS_LEVEL_NUM - 1;
  *uMaxVoltSwingLvl = DP_VOLTAGESWING_LEVEL_NUM - 1;
}

/*********************************************************************************************
*
** FUNCTION: HAL_DP_AltMode_1_2_0_PHY_AUXConfig()
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
HAL_MDSS_ErrorType HAL_DP_AltMode_1_2_0_PHY_AUXConfig(DP_DeviceIDType       eDeviceId,
                                                      HAL_DP_AuxPhyCfgType *pAuxPhyCfg)
{
  HAL_MDSS_ErrorType eStatus = HAL_MDSS_STATUS_FAILED_NOT_SUPPORTED;
  uintPtr            uOffset = HAL_DP_AltMode_GetRegBaseOffset(eDeviceId);

  if ((NULL != pAuxPhyCfg) &&
      (NULL != pAuxPhyCfg->pDPAuxPhyCfg1Index))
  {
    if (*pAuxPhyCfg->pDPAuxPhyCfg1Index < DP_AUX_CFG_SETTINGS_LEN)
    {
      DP_PHY_REGWRITE(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_DP_PHY_AUX_CFG1_ADDR, aAuxPhyCfg1Setting[*pAuxPhyCfg->pDPAuxPhyCfg1Index]);

      eStatus = HAL_MDSS_STATUS_SUCCESS;
    }
  }

  return eStatus;
}

/*********************************************************************************************
*
** FUNCTION: HAL_DP_AltMode_1_2_0_PHY_Power()
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
void HAL_DP_AltMode_1_2_0_PHY_Power(DP_DeviceIDType eDeviceId, bool32 bPowerOn)
{
  uintPtr            uOffset = HAL_DP_AltMode_GetRegBaseOffset(eDeviceId);

  if (FALSE == bPowerOn)
  {
    // Power down whole DP PHY
    DP_PHY_REGWRITE(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_DP_PHY_PD_CTL_ADDR, 0x02);
  }
  else
  {
    // Power up is embedded in DP PHY config function for correct sequence. 
  }
}

/****************************************************************************
*
** FUNCTION: HAL_DP_AltMode_1_2_0_PHY_GetPLLStatus()
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
void HAL_DP_AltMode_1_2_0_PHY_GetPLLStatus(DP_DeviceIDType eDeviceId, bool32 *pbPLLLocked)
{
  uint32             uStatus = 0;
  uintPtr            uOffset = HAL_DP_AltMode_GetRegBaseOffset(eDeviceId);

  uStatus = DP_PHY_REGREAD(uOffset + HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_DP_PHY_STATUS_ADDR) & 
                           HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_DP_DP_PHY_STATUS_TSYNC_DONE_BMSK;

  if (NULL != pbPLLLocked)
  {
    *pbPLLLocked = (0 == uStatus) ? FALSE : TRUE;
  }
}

/****************************************************************************
*
** FUNCTION: HAL_DP_AltMode_PhyFxnsInit()
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
void HAL_DP_AltMode_1_2_0_PhyFxnsInit(HAL_DP_PhyPllFunctionTable *pFxnTable)
{
  pFxnTable->HAL_DP_PHY_Config          = HAL_DP_AltMode_1_2_0_PHY_Config;
  pFxnTable->HAL_DP_PHY_Reset           = HAL_DP_AltMode_1_2_0_PHY_Reset;
  pFxnTable->HAL_DP_PHY_AUX_Init        = HAL_DP_AltMode_1_2_0_PHY_AUX_Init;
  pFxnTable->HAL_DP_PHY_SetupPeSw       = HAL_DP_AltMode_1_2_0_PHY_SetupPeSw;
  pFxnTable->HAL_DP_PHY_GetMaxPeSwLevel = HAL_DP_AltMode_1_2_0_PHY_GetMaxPeSwLevel;
  pFxnTable->HAL_DP_PHY_AUXConfig       = HAL_DP_AltMode_1_2_0_PHY_AUXConfig;
  pFxnTable->HAL_DP_PHY_SetPHYMode      = HAL_DP_AltMode_1_2_0_PHY_SetPHYMode;
  pFxnTable->HAL_DP_PHY_Power           = HAL_DP_AltMode_1_2_0_PHY_Power;
  pFxnTable->HAL_DP_PHY_GetPLLStatus    = HAL_DP_AltMode_1_2_0_PHY_GetPLLStatus;
}


#ifdef __cplusplus
}
#endif

