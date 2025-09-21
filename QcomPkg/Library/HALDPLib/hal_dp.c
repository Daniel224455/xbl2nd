/*=============================================================================
 
  File: hal_dp.c
 
  Source file for DP controller hardware functionality 
   
  Copyright (c) 2016-2018 Qualcomm Technologies, Inc.  All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential.
=============================================================================*/


/*------------------------------------------------------------------------------
 * Include Files
 *----------------------------------------------------------------------------*/
#include "hal_dp_i.h"

#ifdef __cplusplus
extern "C" {
#endif


/****************************************************************************

FUNCTION: HAL_DP_Reset()

   Resets DP HW Block

Parameters:
   uModule           -[in] Which HW module to reset
   uResetFlag        -[in] Reserved

Return:
   HAL_MDSS_ErrorType

****************************************************************************/
HAL_MDSS_ErrorType HAL_DP_Reset(uint32       uModule,
                                uint32       uResetFlag)
{
  uint32 uRegVal = 0;

  if (uModule & HAL_DP_MODULE_TYPE_MAINLINK)
  {
    /*Toggle Reset to Main link Controller*/
    uRegVal = in_dword(HWIO_DP_MAINLINK_CTRL_ADDR);
    uRegVal = HWIO_OUT_FLD(uRegVal, DP_MAINLINK_CTRL, SW_MAINLINK_RESET, 1);
    out_dword(HWIO_DP_MAINLINK_CTRL_ADDR, uRegVal);

    /*Clear Reset*/
    uRegVal = HWIO_OUT_FLD(uRegVal, DP_MAINLINK_CTRL, SW_MAINLINK_RESET, 0);
    out_dword(HWIO_DP_MAINLINK_CTRL_ADDR, uRegVal);
  }

  if (uModule & HAL_DP_MODULE_TYPE_AUX)
  {
    /*Toggle Reset to AUX Controller*/
    uRegVal = in_dword(HWIO_DP_AUX_CTRL_ADDR);
    uRegVal = HWIO_OUT_FLD(uRegVal, DP_AUX_CTRL, SW_AUX_RESET, 1);
    out_dword(HWIO_DP_AUX_CTRL_ADDR, uRegVal);

    /*Clear Reset*/
    uRegVal = HWIO_OUT_FLD(uRegVal, DP_AUX_CTRL, SW_AUX_RESET, 0);
    out_dword(HWIO_DP_AUX_CTRL_ADDR, uRegVal);
  }

  if (uModule & HAL_DP_MODULE_TYPE_PHY)
  {
    uRegVal = HWIO_OUT_FLD(0, DP_PHY_CTRL, SW_RESET, 1);
  }

  if (uModule & HAL_DP_MODULE_TYPE_PLL)
  {
    uRegVal = HWIO_OUT_FLD(0, DP_PHY_CTRL, SW_RESET_PLL, 1);
  }

  if ( ( uModule & HAL_DP_MODULE_TYPE_PHY )||
       ( uModule & HAL_DP_MODULE_TYPE_PLL ))
  {
    /*Toggle Reset to DP PHY/PLL Controller*/
    out_dword(HWIO_DP_PHY_CTRL_ADDR, uRegVal);

    /*Clear Reset */
    out_dword(HWIO_DP_PHY_CTRL_ADDR, 0);
  }

  return HAL_MDSS_STATUS_SUCCESS;
}

/****************************************************************************

FUNCTION: HAL_DP_Interrupt_Clear()

   Clears or Acknowledges the interrupts

Parameters:
   uClearFlags                -[in] Reserved

Return: 
   HAL_MDSS_ErrorType

****************************************************************************/
HAL_MDSS_ErrorType HAL_DP_Interrupt_Clear(HAL_DP_InterruptType          eInterruptSrc,
                                          uint32                        uClearFlags )
{
  bool32 bClear   = FALSE;
  uint32 uRegVal  = in_dword(HWIO_DP_INTERRUPT_STATUS_ADDR);

  /*******************************************************************************************/
  /*DP_INTERRUPT_STATUS */
  bClear  = ((eInterruptSrc& HAL_DP_INTR_TYPE_DPPHY_AUX_ERROR)?(TRUE):(FALSE));
  uRegVal = HWIO_OUT_FLD(uRegVal, DP_INTERRUPT_STATUS, DPPHY_AUX_ERROR_ACK, bClear);

  bClear  = ((eInterruptSrc& HAL_DP_INTR_TYPE_PLL_UNLOCK_DET)?(TRUE):(FALSE));
  uRegVal = HWIO_OUT_FLD(uRegVal, DP_INTERRUPT_STATUS, PLL_UNLOCK_DET_ACK, bClear);

  bClear  = ((eInterruptSrc& HAL_DP_INTR_TYPE_AUX_DEFER_DURING_I2C)?(TRUE):(FALSE));
  uRegVal = HWIO_OUT_FLD(uRegVal, DP_INTERRUPT_STATUS, AUX_DEFER_DURING_I2C_ACK, bClear);

  bClear  = ((eInterruptSrc& HAL_DP_INTR_TYPE_AUX_NACK_DURING_I2C)?(TRUE):(FALSE));
  uRegVal = HWIO_OUT_FLD(uRegVal, DP_INTERRUPT_STATUS, AUX_NACK_DURING_I2C_ACK, bClear);

  bClear  = ((eInterruptSrc& HAL_DP_INTR_TYPE_WRONG_READDATA_COUNT)?(TRUE):(FALSE));
  uRegVal = HWIO_OUT_FLD(uRegVal, DP_INTERRUPT_STATUS, WRONG_READDATA_COUNT_ACK, bClear);

  bClear  = ((eInterruptSrc& HAL_DP_INTR_TYPE_CONSECUTIVE_NACK_DEFER)?(TRUE):(FALSE));
  uRegVal = HWIO_OUT_FLD(uRegVal, DP_INTERRUPT_STATUS, CONSECUTIVE_NACK_DEFER_ACK, bClear);

  bClear  = ((eInterruptSrc& HAL_DP_INTR_TYPE_CONSECUTIVE_TIMEOUT)?(TRUE):(FALSE));
  uRegVal = HWIO_OUT_FLD(uRegVal, DP_INTERRUPT_STATUS, CONSECUTIVE_TIMEOUT_ACK, bClear);

  bClear  = ((eInterruptSrc& HAL_DP_INTR_TYPE_WRONG_ADDRESS)?(TRUE):(FALSE));
  uRegVal = HWIO_OUT_FLD(uRegVal, DP_INTERRUPT_STATUS, WRONG_ADDRESS_ACK, bClear);

  bClear  = ((eInterruptSrc& HAL_DP_INTR_TYPE_AUX_I2C_DONE)?(TRUE):(FALSE));
  uRegVal = HWIO_OUT_FLD(uRegVal, DP_INTERRUPT_STATUS, AUX_I2C_DONE_ACK, bClear);

  out_dword(HWIO_DP_INTERRUPT_STATUS_ADDR, uRegVal);

  /*******************************************************************************************/
  /*DP_INTERRUPT_STATUS_2 */
  uRegVal = 0;
  bClear  = ((eInterruptSrc& HAL_DP_INTR_TYPE_CRC_UPDATED)?(TRUE):(FALSE));
  uRegVal = HWIO_OUT_FLD(uRegVal , DP_INTERRUPT_STATUS_2, CRC_UPDATED_ACK, bClear);

  bClear  = ((eInterruptSrc& HAL_DP_INTR_TYPE_FRAME_END)?(TRUE):(FALSE));
  uRegVal = HWIO_OUT_FLD(uRegVal , DP_INTERRUPT_STATUS_2, FRAME_END_ACK, bClear);

  bClear  = ((eInterruptSrc& HAL_DP_INTR_TYPE_IDLE_PATTERNS_SENT)?(TRUE):(FALSE));
  uRegVal = HWIO_OUT_FLD(uRegVal , DP_INTERRUPT_STATUS_2, IDLE_PATTERNS_SENT_ACK, bClear);

  bClear  = ((eInterruptSrc& HAL_DP_INTR_TYPE_READY_FOR_VIDEO)?(TRUE):(FALSE));
  uRegVal = HWIO_OUT_FLD(uRegVal , DP_INTERRUPT_STATUS_2, READY_FOR_VIDEO_ACK, bClear);

  out_dword(HWIO_DP_INTERRUPT_STATUS_2_ADDR, uRegVal);

  // TODO - PHY interrupts

  return HAL_MDSS_STATUS_SUCCESS;
}

/****************************************************************************

FUNCTION: HAL_DP_Interrupt_GetStatus()
 
   Returns the interrupts status

Parameters:
   peIntrStatus               -[in/out]  Status for each module.
   uGetStatusFlags            -[in] Reserved

Return: 
   HAL_MDSS_ErrorType

****************************************************************************/
HAL_MDSS_ErrorType HAL_DP_Interrupt_GetStatus(HAL_DP_InterruptType         *peIntrStatus,
                                              uint32                        uGetStatusFlags)
{
  uint32 uIntReg1      = in_dword(HWIO_DP_INTERRUPT_STATUS_ADDR); 
  uint32 uIntReg2      = in_dword(HWIO_DP_INTERRUPT_STATUS_2_ADDR);
  uint32 uHalInterrupt = 0;

  if(uIntReg1 & HWIO_DP_INTERRUPT_STATUS_AUX_I2C_DONE_INT_BMSK)
  {
    uHalInterrupt |= HAL_DP_INTR_TYPE_AUX_I2C_DONE;
  }
  if(uIntReg1 & HWIO_DP_INTERRUPT_STATUS_WRONG_ADDRESS_INT_BMSK)
  {
    uHalInterrupt |= HAL_DP_INTR_TYPE_WRONG_ADDRESS;
  }
  if(uIntReg1 & HWIO_DP_INTERRUPT_STATUS_CONSECUTIVE_TIMEOUT_INT_BMSK)
  {
    uHalInterrupt |= HAL_DP_INTR_TYPE_CONSECUTIVE_TIMEOUT;
  }
  if(uIntReg1 & HWIO_DP_INTERRUPT_STATUS_CONSECUTIVE_NACK_DEFER_INT_BMSK)
  {
    uHalInterrupt |= HAL_DP_INTR_TYPE_CONSECUTIVE_NACK_DEFER;
  }
  if(uIntReg1 & HWIO_DP_INTERRUPT_STATUS_WRONG_READDATA_COUNT_INT_BMSK)
  {
    uHalInterrupt |= HAL_DP_INTR_TYPE_WRONG_READDATA_COUNT;
  }
  if(uIntReg1 & HWIO_DP_INTERRUPT_STATUS_AUX_NACK_DURING_I2C_INT_BMSK)
  {
    uHalInterrupt |= HAL_DP_INTR_TYPE_AUX_NACK_DURING_I2C;
  }
  if(uIntReg1 & HWIO_DP_INTERRUPT_STATUS_AUX_DEFER_DURING_I2C_INT_BMSK)
  {
    uHalInterrupt |= HAL_DP_INTR_TYPE_AUX_DEFER_DURING_I2C;
  }
  if(uIntReg1 & HWIO_DP_INTERRUPT_STATUS_PLL_UNLOCK_DET_INT_BMSK)
  {
    uHalInterrupt |= HAL_DP_INTR_TYPE_PLL_UNLOCK_DET;
  }
  if(uIntReg1 & HWIO_DP_INTERRUPT_STATUS_DPPHY_AUX_ERROR_INT_BMSK)
  {
    uHalInterrupt |= HAL_DP_INTR_TYPE_DPPHY_AUX_ERROR;
  }

  if(uIntReg2 & HWIO_DP_INTERRUPT_STATUS_2_READY_FOR_VIDEO_INT_BMSK)
  {
    uHalInterrupt |= HAL_DP_INTR_TYPE_READY_FOR_VIDEO;
  }
  if(uIntReg2 & HWIO_DP_INTERRUPT_STATUS_2_IDLE_PATTERNS_SENT_INT_BMSK)
  {
    uHalInterrupt |= HAL_DP_INTR_TYPE_IDLE_PATTERNS_SENT;
  }
  if(uIntReg2 & HWIO_DP_INTERRUPT_STATUS_2_FRAME_END_INT_BMSK)
  {
    uHalInterrupt |= HAL_DP_INTR_TYPE_FRAME_END;
  }
  if(uIntReg2 & HWIO_DP_INTERRUPT_STATUS_2_CRC_UPDATED_INT_BMSK)
  {
    uHalInterrupt |= HAL_DP_INTR_TYPE_CRC_UPDATED;
  }

  // TODO - PHY interrupts

  
  *peIntrStatus = (HAL_DP_InterruptType)uHalInterrupt;

  return HAL_MDSS_STATUS_SUCCESS; 
}

/**********************************************************************************************************************
*
** FUNCTION: HAL_DP_SetupBIST()
*/
/*!
* \brief
*     Set BIST test pattern in DP controller
*
* \param [in]   psMainStream    - Main stream info for the timing info
* \param [in]   dpOptions       - Test pattern ID. 0->disable test pattern
*
* \retval HAL_MDSS_ErrorType
*
**********************************************************************************************************************/
HAL_MDSS_ErrorType HAL_DP_SetupBIST(HAL_DP_MSAPacketType *psMainStream, uint32 dpOptions)
{
  enum hal_mdp_tpg_pattern
  {
    HAL_MDP_TPG_PAT_TPG_SW_RESET                  = 0,
    HAL_MDP_TPG_PAT_COUNTER_PATTERN               = 1,
    HAL_MDP_TPG_PAT_FIXED_PATTERN                 = 2,
    HAL_MDP_TPG_PAT_COLOR_RAMP_64L_64P            = 3,
    HAL_MDP_TPG_PAT_COLOR_RAMP_64L_256P           = 4,
    HAL_MDP_TPG_PAT_BLACK_WHITE_VERTICAL_LINES    = 5,
    HAL_MDP_TPG_PAT_GRAYSCALE_RAMP                = 6,
    HAL_MDP_TPG_PAT_COLOR_SQUARE                  = 7,
    HAL_MDP_TPG_PAT_CHECKERED_RECTANGLE_PATTERN   = 8,
    HAL_MDP_TPG_PAT_BASIC_COLOR_CHANGING_PATTERN  = 9,
    HAL_MDP_TPG_PAT_PRBS_PATTERN                  = 10
  };

  HAL_MDSS_ErrorType rtnVal = HAL_MDSS_STATUS_SUCCESS;

  uint32 hBlank      = psMainStream->uHsyncBackPorchInPixels + psMainStream->uHsyncPulseInPixels + psMainStream->uHsyncFrontPorchInPixels;
  uint32 vBlank      = psMainStream->uVsyncBackPorchInLines  + psMainStream->uVsyncPulseInLines  + psMainStream->uVsyncFrontPorchInLines;
  uint32 vSyncPeriod = (psMainStream->uVisibleWidthInPixels+ hBlank)   * (psMainStream->uVisibleHeightInPixels + vBlank); //in number of pixels
  uint32 uRegVal     = 0;

  //program BIST scan type
  out_dword(HWIO_DP_P0CLK_INTF_CONFIG_ADDR, 0);

  //program BIST hsync period and hsync width
  uRegVal = HWIO_OUT_FLD(uRegVal, DP_P0CLK_HSYNC_CTL, PULSE_WIDTH, psMainStream->uHsyncPulseInPixels);
  uRegVal = HWIO_OUT_FLD(uRegVal, DP_P0CLK_HSYNC_CTL, HSYNC_PERIOD, psMainStream->uVisibleWidthInPixels + hBlank);
  out_dword(HWIO_DP_P0CLK_HSYNC_CTL_ADDR, uRegVal);

  //program BIST vsync period and and vsync width
  out_dword(HWIO_DP_P0CLK_VSYNC_PERIOD_F0_ADDR, vSyncPeriod); //in number of pixels
  out_dword(HWIO_DP_P0CLK_VSYNC_PULSE_WIDTH_F0_ADDR, ((psMainStream->uVisibleWidthInPixels + hBlank) * psMainStream->uVsyncPulseInLines));//in number of pixels

  //hsync position
  uRegVal = 0;
  uRegVal = HWIO_OUT_FLD(uRegVal, DP_P0CLK_DISPLAY_HCTL, DISPLAY_START_X, psMainStream->uHsyncBackPorchInPixels + psMainStream->uHsyncPulseInPixels);
  uRegVal = HWIO_OUT_FLD(uRegVal, DP_P0CLK_DISPLAY_HCTL, DISPLAY_END_X,   psMainStream->uVisibleWidthInPixels + psMainStream->uHsyncBackPorchInPixels + psMainStream->uHsyncPulseInPixels -1);
  out_dword(HWIO_DP_P0CLK_DISPLAY_HCTL_ADDR, uRegVal);

  //vsync position
  out_dword(HWIO_DP_P0CLK_DISPLAY_V_START_F0_ADDR, ((psMainStream->uVsyncBackPorchInLines + psMainStream->uVsyncPulseInLines) * 
                                                   (psMainStream->uVisibleWidthInPixels + hBlank) +
                                                    psMainStream->uHsyncBackPorchInPixels +  psMainStream->uHsyncPulseInPixels));
  out_dword(HWIO_DP_P0CLK_DISPLAY_V_END_F0_ADDR, ((psMainStream->uVsyncBackPorchInLines + psMainStream->uVsyncPulseInLines + psMainStream->uVisibleHeightInPixels) *
                                                 (psMainStream->uVisibleWidthInPixels + hBlank)-1 - psMainStream->uHsyncFrontPorchInPixels));

  //NZ doesn't support interlace mode
  out_dword(HWIO_DP_P0CLK_VSYNC_PERIOD_F1_ADDR, 0);
  out_dword(HWIO_DP_P0CLK_VSYNC_PULSE_WIDTH_F1_ADDR, 0);
  out_dword(HWIO_DP_P0CLK_DISPLAY_V_START_F1_ADDR, 0);
  out_dword(HWIO_DP_P0CLK_DISPLAY_V_END_F1_ADDR, 0);
  out_dword(HWIO_DP_P0CLK_ACTIVE_V_START_F1_ADDR, 0);
  out_dword(HWIO_DP_P0CLK_ACTIVE_V_END_F1_ADDR, 0);

  uRegVal = 0;
  switch (dpOptions)
  {
  case   HAL_MDP_TPG_PAT_COUNTER_PATTERN:             uRegVal = HWIO_OUT_FLD(uRegVal, DP_P0CLK_TPG_MAIN_CONTROL, COUNTER_PATTERN,             1); break;
  case   HAL_MDP_TPG_PAT_FIXED_PATTERN:               uRegVal = HWIO_OUT_FLD(uRegVal, DP_P0CLK_TPG_MAIN_CONTROL, FIXED_PATTERN,               1); break;
  case   HAL_MDP_TPG_PAT_COLOR_RAMP_64L_64P:          uRegVal = HWIO_OUT_FLD(uRegVal, DP_P0CLK_TPG_MAIN_CONTROL, COLOR_RAMP_64L_64P,          1); break;
  case   HAL_MDP_TPG_PAT_COLOR_RAMP_64L_256P:         uRegVal = HWIO_OUT_FLD(uRegVal, DP_P0CLK_TPG_MAIN_CONTROL, COLOR_RAMP_64L_256P,         1); break;
  case   HAL_MDP_TPG_PAT_BLACK_WHITE_VERTICAL_LINES:  uRegVal = HWIO_OUT_FLD(uRegVal, DP_P0CLK_TPG_MAIN_CONTROL, BLACK_WHITE_VERTICAL_LINES,  1); break;
  case   HAL_MDP_TPG_PAT_GRAYSCALE_RAMP:              uRegVal = HWIO_OUT_FLD(uRegVal, DP_P0CLK_TPG_MAIN_CONTROL, GRAYSCALE_RAMP,              1); break;
  case   HAL_MDP_TPG_PAT_COLOR_SQUARE:                uRegVal = HWIO_OUT_FLD(uRegVal, DP_P0CLK_TPG_MAIN_CONTROL, COLOR_SQUARE,                1); break;
  case   HAL_MDP_TPG_PAT_CHECKERED_RECTANGLE_PATTERN: uRegVal = HWIO_OUT_FLD(uRegVal, DP_P0CLK_TPG_MAIN_CONTROL, CHECKERED_RECTANGLE_PATTERN, 1); break;
  case   HAL_MDP_TPG_PAT_BASIC_COLOR_CHANGING_PATTERN:uRegVal = HWIO_OUT_FLD(uRegVal, DP_P0CLK_TPG_MAIN_CONTROL, BASIC_COLOR_CHANGING_PATTERN,1); break;
  }
  out_dword(HWIO_DP_P0CLK_TPG_MAIN_CONTROL_ADDR, uRegVal);

  // Setup Format and Color
  uRegVal = 0;
  uRegVal = HWIO_OUT_FLD(uRegVal, DP_P0CLK_TPG_VIDEO_CONFIG, BPP, 1);
  uRegVal = HWIO_OUT_FLD(uRegVal, DP_P0CLK_TPG_VIDEO_CONFIG, RGB, 1);
  out_dword(HWIO_DP_P0CLK_TPG_VIDEO_CONFIG_ADDR, uRegVal);


  if (HAL_MDP_TPG_PAT_TPG_SW_RESET == dpOptions)
  {
    out_dword(HWIO_DP_P0CLK_BIST_ENABLE_ADDR,      0);
    out_dword(HWIO_DP_P0CLK_TIMING_ENGINE_EN_ADDR, 0);
  }
  else
  {
    out_dword(HWIO_DP_P0CLK_BIST_ENABLE_ADDR,      1);
    out_dword(HWIO_DP_P0CLK_TIMING_ENGINE_EN_ADDR, 1);
  }


  return rtnVal;
}

/****************************************************************************
*
** FUNCTION: HAL_DP_ReadVersionInfo()
*/
/*!
* \brief
*     Returns the version information
*
* \param [in/out] psDPVersionInfo            - Version information
*
* \retval None
*
****************************************************************************/
void HAL_DP_ReadVersionInfo(HAL_HW_VersionType *psDPVersionInfo)
{
  uint32      uVersionInfo = in_dword(HWIO_DP_HW_VERSION_ADDR);

  if (NULL != psDPVersionInfo)
  {
    psDPVersionInfo->uMajorVersion    = HWIO_GETVAL(DP_HW_VERSION, uVersionInfo, MAJOR);
    psDPVersionInfo->uMinorVersion    = HWIO_GETVAL(DP_HW_VERSION, uVersionInfo, MINOR);
    psDPVersionInfo->uReleaseVersion  = HWIO_GETVAL(DP_HW_VERSION, uVersionInfo, STEP);
  }
}


#ifdef __cplusplus
}
#endif

