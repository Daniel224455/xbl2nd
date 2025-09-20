/*=============================================================================
 
  File: hal_dp.c
 
  Source file for DP controller hardware functionality 
   
  Copyright (c) 2016-2019 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
=============================================================================*/


/*------------------------------------------------------------------------------
 * Include Files
 *----------------------------------------------------------------------------*/
#include "hal_dp_i.h"

#ifdef __cplusplus
extern "C" {
#endif


/* Wait for 100 ms to make sure CRC populated after FRAME_END */ 
#define HAL_DP_WAIT_FOR_CRC    100



/****************************************************************************
*
** FUNCTION: HAL_DP_GetRegBaseOffset()
*/
/*!
* \brief   
*     Get DP register address offset for a given DP controller ID
*
* \param [in]  eControllerId - DP core ID
*
* \retval - register base offset
*
****************************************************************************/
uintPtr HAL_DP_GetRegBaseOffset(DP_ControllerIDType  eControllerId)
{
  uintPtr   uOffset;

  switch (eControllerId)
  {
  case DP_CONTROLLER_ID_DP_0:
    {
      /* DP */
      uOffset = 0;
      break;
    }
  case DP_CONTROLLER_ID_DP_1:
    {
      /* DP TX1 */
      uOffset = DP_TX1_AHBCLK_REG_BASE - DP_AHBCLK_REG_BASE;
      break;
    }
  case DP_CONTROLLER_ID_EDP:
    {
      /* eDP */
      uOffset = EDP_AHBCLK_REG_BASE - DP_AHBCLK_REG_BASE;
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


/****************************************************************************
*
** FUNCTION: HAL_DP_GetPixelClkRegBaseOffset()
*/
/*!
* \brief
*     Get DP Pclk register address offset for a given DP controller ID and 
*     stream ID
*
* \param [in]  eControllerId - DP core ID
* \param [in]  eStreamID     - Stream ID
*
* \retval - register base offset
*
****************************************************************************/
uintPtr HAL_DP_GetPixelClkRegBaseOffset(DP_ControllerIDType  eControllerId,
                                        DP_StreamIDType      eStreamId)
{
  uintPtr   uOffset     = HAL_DP_GetRegBaseOffset(eControllerId);
  uintPtr   uPclkOffset = 0;

  switch (eStreamId)
  {
  case DP_STREAM_ID_MST1:
    {
      uPclkOffset = DP_P1CLK_REG_BASE - DP_P0CLK_REG_BASE;
      break;
    }
  default:
    {
      uPclkOffset = 0;
    }
  }

  return (uOffset + uPclkOffset);
}


/**********************************************************************************************************************
*
** FUNCTION: HAL_DP_Init()
*/
/*!
* \brief
*     Initializes DP HW Block
*
* \param [in]   eControllerId       - DP core ID
*
* \retval None
*
**********************************************************************************************************************/
void HAL_DP_Init(DP_ControllerIDType eControllerId)
{
    /* DO nothing */
}

/****************************************************************************

FUNCTION: HAL_DP_Reset()

   Resets DP HW Block

Parameters: 
   eControllerId     -[in] DP core ID
   uModule           -[in] Which HW module to reset
   uResetFlag        -[in] Reserved

Return:
   HAL_MDSS_ErrorType

****************************************************************************/
HAL_MDSS_ErrorType HAL_DP_Reset(DP_ControllerIDType eControllerId,
                                uint32              uModule,
                                uint32              uResetFlag)
{
  uint32  uRegVal = 0;
  uintPtr uOffset = HAL_DP_GetRegBaseOffset(eControllerId);

  if (uModule & HAL_DP_MODULE_TYPE_MAINLINK)
  {
    /*Toggle Reset to Main link Controller*/
    uRegVal = in_dword(uOffset + HWIO_DP_MAINLINK_CTRL_ADDR);
    uRegVal = HWIO_OUT_FLD(uRegVal, DP_MAINLINK_CTRL, SW_MAINLINK_RESET, 1);
    out_dword(uOffset + HWIO_DP_MAINLINK_CTRL_ADDR, uRegVal);

    /*Clear Reset*/
    uRegVal = HWIO_OUT_FLD(uRegVal, DP_MAINLINK_CTRL, SW_MAINLINK_RESET, 0);
    out_dword(uOffset + HWIO_DP_MAINLINK_CTRL_ADDR, uRegVal);

    /* Reset controller */
    uRegVal = in_dword(uOffset + HWIO_DP_SW_RESET_ADDR);
    uRegVal = HWIO_OUT_FLD(uRegVal, DP_SW_RESET, DP_SW_RESET, 1);  
    out_dword(uOffset + HWIO_DP_SW_RESET_ADDR, uRegVal);

    uRegVal = HWIO_OUT_FLD(uRegVal, DP_SW_RESET, DP_SW_RESET, 0);  
    out_dword(uOffset + HWIO_DP_SW_RESET_ADDR, uRegVal);
  }

  if (uModule & HAL_DP_MODULE_TYPE_AUX)
  {
    /*Toggle Reset to AUX Controller*/
    uRegVal = in_dword(uOffset + HWIO_DP_AUX_CTRL_ADDR);
    uRegVal = HWIO_OUT_FLD(uRegVal, DP_AUX_CTRL, SW_AUX_RESET, 1);
    out_dword(uOffset + HWIO_DP_AUX_CTRL_ADDR, uRegVal);

    /*Clear Reset*/
    uRegVal = HWIO_OUT_FLD(uRegVal, DP_AUX_CTRL, SW_AUX_RESET, 0);
    out_dword(uOffset + HWIO_DP_AUX_CTRL_ADDR, uRegVal);
  }

  if (uModule & HAL_DP_MODULE_TYPE_PHY)
  {
    uRegVal = HWIO_OUT_FLD(0, DP_PHY_CTRL, SW_RESET, 1);
  }

  if (uModule & HAL_DP_MODULE_TYPE_PLL)
  {
    // PLL reset - SW_RESET_PLL inside DP_PHY_CTRL is NODOC now
  }

  if ( ( uModule & HAL_DP_MODULE_TYPE_PHY )||
       ( uModule & HAL_DP_MODULE_TYPE_PLL ))
  {
    /*Toggle Reset to DP PHY/PLL Controller*/
    out_dword(uOffset + HWIO_DP_PHY_CTRL_ADDR, uRegVal);

    /*Clear Reset */
    out_dword(uOffset + HWIO_DP_PHY_CTRL_ADDR, 0);
  }

  return HAL_MDSS_STATUS_SUCCESS;
}

/****************************************************************************

FUNCTION: HAL_DP_Interrupt_Clear()

   Clears or Acknowledges the interrupts

Parameters:
   eControllerId              -[in] DP core ID
   eInterruptSrc              -[in] DP interrupt mask to clear
   uClearFlags                -[in] Reserved

Return: 
   HAL_MDSS_ErrorType

****************************************************************************/
HAL_MDSS_ErrorType HAL_DP_Interrupt_Clear(DP_ControllerIDType      eControllerId,
                                          HAL_DP_InterruptType     eInterruptSrc,
                                          uint32                   uClearFlags)
{
  bool32 bClear   = FALSE;
  uintPtr uOffset = HAL_DP_GetRegBaseOffset(eControllerId);
  uint32 uRegVal  = in_dword(uOffset + HWIO_DP_INTERRUPT_STATUS_ADDR);

  /*******************************************************************************************/
  /*DP_INTERRUPT_STATUS */
  bClear  = ((eInterruptSrc & HAL_DP_INTR_TYPE_DPPHY_AUX_ERROR)?(TRUE):(FALSE));
  uRegVal = HWIO_OUT_FLD(uRegVal, DP_INTERRUPT_STATUS, DPPHY_AUX_ERROR_ACK, bClear);

  bClear  = ((eInterruptSrc & HAL_DP_INTR_TYPE_PLL_UNLOCK_DET)?(TRUE):(FALSE));
  uRegVal = HWIO_OUT_FLD(uRegVal, DP_INTERRUPT_STATUS, PLL_UNLOCK_DET_ACK, bClear);

  bClear  = ((eInterruptSrc & HAL_DP_INTR_TYPE_AUX_NACK_DURING_I2C)?(TRUE):(FALSE));
  uRegVal = HWIO_OUT_FLD(uRegVal, DP_INTERRUPT_STATUS, AUX_NACK_DURING_I2C_ACK, bClear);

  bClear  = ((eInterruptSrc & HAL_DP_INTR_TYPE_WRONG_READDATA_COUNT)?(TRUE):(FALSE));
  uRegVal = HWIO_OUT_FLD(uRegVal, DP_INTERRUPT_STATUS, WRONG_READDATA_COUNT_ACK, bClear);

  bClear  = ((eInterruptSrc & HAL_DP_INTR_TYPE_CONSECUTIVE_NACK_DEFER)?(TRUE):(FALSE));
  uRegVal = HWIO_OUT_FLD(uRegVal, DP_INTERRUPT_STATUS, CONSECUTIVE_NACK_DEFER_ACK, bClear);

  bClear  = ((eInterruptSrc & HAL_DP_INTR_TYPE_CONSECUTIVE_TIMEOUT)?(TRUE):(FALSE));
  uRegVal = HWIO_OUT_FLD(uRegVal, DP_INTERRUPT_STATUS, CONSECUTIVE_TIMEOUT_ACK, bClear);

  bClear  = ((eInterruptSrc & HAL_DP_INTR_TYPE_WRONG_ADDRESS)?(TRUE):(FALSE));
  uRegVal = HWIO_OUT_FLD(uRegVal, DP_INTERRUPT_STATUS, WRONG_ADDRESS_ACK, bClear);

  bClear  = ((eInterruptSrc & HAL_DP_INTR_TYPE_AUX_I2C_DONE)?(TRUE):(FALSE));
  uRegVal = HWIO_OUT_FLD(uRegVal, DP_INTERRUPT_STATUS, AUX_I2C_DONE_ACK, bClear);

  out_dword(uOffset + HWIO_DP_INTERRUPT_STATUS_ADDR, uRegVal);

  /*******************************************************************************************/
  /*DP_INTERRUPT_STATUS_2 */
  uRegVal = in_dword(uOffset + HWIO_DP_INTERRUPT_STATUS_2_ADDR);
  bClear  = ((eInterruptSrc & HAL_DP_INTR_TYPE_CRC_UPDATED)?(TRUE):(FALSE));
  uRegVal = HWIO_OUT_FLD(uRegVal , DP_INTERRUPT_STATUS_2, CRC_UPDATED_ACK, bClear);

  bClear  = ((eInterruptSrc & HAL_DP_INTR_TYPE_FRAME_END)?(TRUE):(FALSE));
  uRegVal = HWIO_OUT_FLD(uRegVal , DP_INTERRUPT_STATUS_2, FRAME_END_ACK, bClear);

  bClear  = ((eInterruptSrc & HAL_DP_INTR_TYPE_IDLE_PATTERNS_SENT)?(TRUE):(FALSE));
  uRegVal = HWIO_OUT_FLD(uRegVal , DP_INTERRUPT_STATUS_2, IDLE_PATTERNS_SENT_ACK, bClear);

  bClear  = ((eInterruptSrc & HAL_DP_INTR_TYPE_READY_FOR_VIDEO)?(TRUE):(FALSE));
  uRegVal = HWIO_OUT_FLD(uRegVal , DP_INTERRUPT_STATUS_2, READY_FOR_VIDEO_ACK, bClear);

  out_dword(uOffset + HWIO_DP_INTERRUPT_STATUS_2_ADDR, uRegVal);

  // TODO - PHY interrupts

  return HAL_MDSS_STATUS_SUCCESS;
}

/****************************************************************************

FUNCTION: HAL_DP_Interrupt_GetStatus()
 
   Returns the interrupts status

Parameters:
   eControllerId              -[in]     DP core ID
   peIntrStatus               -[in/out] Status for each module.
   uGetStatusFlags            -[in]     Reserved

Return: 
   HAL_MDSS_ErrorType

****************************************************************************/
HAL_MDSS_ErrorType HAL_DP_Interrupt_GetStatus(DP_ControllerIDType       eControllerId,
                                              HAL_DP_InterruptType     *peIntrStatus,
                                              uint32                    uGetStatusFlags)
{
  uintPtr uOffset      = HAL_DP_GetRegBaseOffset(eControllerId);
  uint32 uIntReg1      = in_dword(uOffset + HWIO_DP_INTERRUPT_STATUS_ADDR);
  uint32 uIntReg2      = in_dword(uOffset + HWIO_DP_INTERRUPT_STATUS_2_ADDR);
  uint32 uHalInterrupt = 0;

  if (uIntReg1 & HWIO_DP_INTERRUPT_STATUS_AUX_I2C_DONE_INT_BMSK)
  {
    uHalInterrupt |= HAL_DP_INTR_TYPE_AUX_I2C_DONE;
  }
  if (uIntReg1 & HWIO_DP_INTERRUPT_STATUS_WRONG_ADDRESS_INT_BMSK)
  {
    uHalInterrupt |= HAL_DP_INTR_TYPE_WRONG_ADDRESS;
  }
  if (uIntReg1 & HWIO_DP_INTERRUPT_STATUS_CONSECUTIVE_TIMEOUT_INT_BMSK)
  {
    uHalInterrupt |= HAL_DP_INTR_TYPE_CONSECUTIVE_TIMEOUT;
  }
  if (uIntReg1 & HWIO_DP_INTERRUPT_STATUS_CONSECUTIVE_NACK_DEFER_INT_BMSK)
  {
    uHalInterrupt |= HAL_DP_INTR_TYPE_CONSECUTIVE_NACK_DEFER;
  }
  if (uIntReg1 & HWIO_DP_INTERRUPT_STATUS_WRONG_READDATA_COUNT_INT_BMSK)
  {
    uHalInterrupt |= HAL_DP_INTR_TYPE_WRONG_READDATA_COUNT;
  }
  if (uIntReg1 & HWIO_DP_INTERRUPT_STATUS_AUX_NACK_DURING_I2C_INT_BMSK)
  {
    uHalInterrupt |= HAL_DP_INTR_TYPE_AUX_NACK_DURING_I2C;
  }
  if (uIntReg1 & HWIO_DP_INTERRUPT_STATUS_PLL_UNLOCK_DET_INT_BMSK)
  {
    uHalInterrupt |= HAL_DP_INTR_TYPE_PLL_UNLOCK_DET;
  }
  if (uIntReg1 & HWIO_DP_INTERRUPT_STATUS_DPPHY_AUX_ERROR_INT_BMSK)
  {
    uHalInterrupt |= HAL_DP_INTR_TYPE_DPPHY_AUX_ERROR;
  }

  if (uIntReg2 & HWIO_DP_INTERRUPT_STATUS_2_READY_FOR_VIDEO_INT_BMSK)
  {
    uHalInterrupt |= HAL_DP_INTR_TYPE_READY_FOR_VIDEO;
  }
  if (uIntReg2 & HWIO_DP_INTERRUPT_STATUS_2_IDLE_PATTERNS_SENT_INT_BMSK)
  {
    uHalInterrupt |= HAL_DP_INTR_TYPE_IDLE_PATTERNS_SENT;
  }
  if (uIntReg2 & HWIO_DP_INTERRUPT_STATUS_2_FRAME_END_INT_BMSK)
  {
    uHalInterrupt |= HAL_DP_INTR_TYPE_FRAME_END;
  }
  if (uIntReg2 & HWIO_DP_INTERRUPT_STATUS_2_CRC_UPDATED_INT_BMSK)
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
* \param [in]   eControllerId   - DP core ID
* \param [in]   psMainStream    - Main stream info for the timing info
* \param [in]   uVideoPattern   - Test pattern ID. 0->disable test pattern
*
* \retval HAL_MDSS_ErrorType
*
**********************************************************************************************************************/
HAL_MDSS_ErrorType HAL_DP_SetupBIST(DP_ControllerIDType   eControllerId,
                                    HAL_DP_MSAPacketType *psMainStream,
                                    uint32                uVideoPattern)
{
  HAL_MDSS_ErrorType  eStatus      = HAL_MDSS_STATUS_SUCCESS;
  uintPtr             uPclkOffset  = HAL_DP_GetPixelClkRegBaseOffset(eControllerId, psMainStream->uStreamId);

  typedef enum 
  {
    HAL_DP_TESTPATTERN_NONE                       = 0x00,     /**< Disable Test Pattern               */
    HAL_DP_TESTPATTERN_COUNTER_PATTERN,                       /**< Enable counter pattern             */
    HAL_DP_TESTPATTERN_FIXED_PATTERN,                         /**< Enable fixed pattern               */
    HAL_DP_TESTPATTERN_COLOR_RAMP_64L_64P,                    /**< Enable 64L to 64P pattern          */
    HAL_DP_TESTPATTERN_COLOR_RAMP_64L_256P,                   /**< Enable 64L to 256P pattern         */
    HAL_DP_TESTPATTERN_BLACK_WHITE_VERTICAL_LINES,            /**< Enable black white vertical pattern*/
    HAL_DP_TESTPATTERN_GRAYSCALE_RAMP,                        /**< Enable gray scale pattern          */
    HAL_DP_TESTPATTERN_COLOR_SQUARE,                          /**< Enable color square pattern        */
    HAL_DP_TESTPATTERN_CHECKERED_RECTANGLE_PATTERN,           /**< Enable checked rectangle pattern   */
    HAL_DP_TESTPATTERN_BASIC_COLOR_CHANGING_PATTERN,          /**< Enable basic color changing pattern*/
    HAL_DP_TESTPATTERN_PRBS_PATTERN,                          /**< Enable PRBS pattern                */
    HAL_DP_TESTPATTERN_MAX,
    HAL_DP_TESTPATTERN_FORCE_32BIT = 0x7FFFFFFF
  } HAL_DP_TestPatternType;

  if (HAL_DP_TESTPATTERN_NONE == uVideoPattern)
  {
    out_dword(uPclkOffset + HWIO_DP_P0CLK_BIST_ENABLE_ADDR,       0);
    out_dword(uPclkOffset + HWIO_DP_P0CLK_TIMING_ENGINE_EN_ADDR,  0);
  }
  else
  {
    uint32 hBlank      = psMainStream->uHsyncBackPorchInPixels + psMainStream->uHsyncPulseInPixels + psMainStream->uHsyncFrontPorchInPixels;
    uint32 vBlank      = psMainStream->uVsyncBackPorchInLines  + psMainStream->uVsyncPulseInLines  + psMainStream->uVsyncFrontPorchInLines;
    uint32 vSyncPeriod = (psMainStream->uVisibleWidthInPixels + hBlank) * (psMainStream->uVisibleHeightInPixels + vBlank); //in number of pixels
    uint32 uRegVal     = 0;

    //program BIST scan type
    out_dword(uPclkOffset + HWIO_DP_P0CLK_INTF_CONFIG_ADDR, 0);

    //program BIST hsync period and hsync width
    uRegVal = HWIO_OUT_FLD(uRegVal, DP_P0CLK_HSYNC_CTL, PULSE_WIDTH, psMainStream->uHsyncPulseInPixels);
    uRegVal = HWIO_OUT_FLD(uRegVal, DP_P0CLK_HSYNC_CTL, HSYNC_PERIOD, psMainStream->uVisibleWidthInPixels + hBlank);
    out_dword(uPclkOffset + HWIO_DP_P0CLK_HSYNC_CTL_ADDR, uRegVal);

    //program BIST vsync period and and vsync width
    out_dword(uPclkOffset + HWIO_DP_P0CLK_VSYNC_PERIOD_F0_ADDR, vSyncPeriod); //in number of pixels
    out_dword(uPclkOffset + HWIO_DP_P0CLK_VSYNC_PULSE_WIDTH_F0_ADDR, ((psMainStream->uVisibleWidthInPixels + hBlank) * psMainStream->uVsyncPulseInLines)); //in number of pixels

    //hsync position
    uRegVal = 0;
    uRegVal = HWIO_OUT_FLD(uRegVal, DP_P0CLK_DISPLAY_HCTL, DISPLAY_START_X, psMainStream->uHsyncBackPorchInPixels + psMainStream->uHsyncPulseInPixels);
    uRegVal = HWIO_OUT_FLD(uRegVal, DP_P0CLK_DISPLAY_HCTL, DISPLAY_END_X,   psMainStream->uVisibleWidthInPixels + psMainStream->uHsyncBackPorchInPixels + psMainStream->uHsyncPulseInPixels - 1);
    out_dword(uPclkOffset + HWIO_DP_P0CLK_DISPLAY_HCTL_ADDR, uRegVal);

    //vsync position
    out_dword(uPclkOffset + HWIO_DP_P0CLK_DISPLAY_V_START_F0_ADDR, ((psMainStream->uVsyncBackPorchInLines + psMainStream->uVsyncPulseInLines) *
                                                                (psMainStream->uVisibleWidthInPixels + hBlank) +
                                                                psMainStream->uHsyncBackPorchInPixels +  psMainStream->uHsyncPulseInPixels));
    out_dword(uPclkOffset + HWIO_DP_P0CLK_DISPLAY_V_END_F0_ADDR, ((psMainStream->uVsyncBackPorchInLines + psMainStream->uVsyncPulseInLines + psMainStream->uVisibleHeightInPixels) *
                                                              (psMainStream->uVisibleWidthInPixels + hBlank) - 1 - psMainStream->uHsyncFrontPorchInPixels));

    //NZ doesn't support interlace mode
    out_dword(uPclkOffset + HWIO_DP_P0CLK_VSYNC_PERIOD_F1_ADDR,       0);
    out_dword(uPclkOffset + HWIO_DP_P0CLK_VSYNC_PULSE_WIDTH_F1_ADDR,  0);
    out_dword(uPclkOffset + HWIO_DP_P0CLK_DISPLAY_V_START_F1_ADDR,    0);
    out_dword(uPclkOffset + HWIO_DP_P0CLK_DISPLAY_V_END_F1_ADDR,      0);
    out_dword(uPclkOffset + HWIO_DP_P0CLK_ACTIVE_V_START_F1_ADDR,     0);
    out_dword(uPclkOffset + HWIO_DP_P0CLK_ACTIVE_V_END_F1_ADDR,       0);

    uRegVal = 0;
    switch ((HAL_DP_TestPatternType)uVideoPattern)
    {
    case   HAL_DP_TESTPATTERN_COUNTER_PATTERN:             uRegVal = HWIO_OUT_FLD(uRegVal, DP_P0CLK_TPG_MAIN_CONTROL, COUNTER_PATTERN,             1); break;
    case   HAL_DP_TESTPATTERN_FIXED_PATTERN:               uRegVal = HWIO_OUT_FLD(uRegVal, DP_P0CLK_TPG_MAIN_CONTROL, FIXED_PATTERN,               1); break;
    case   HAL_DP_TESTPATTERN_COLOR_RAMP_64L_64P:          uRegVal = HWIO_OUT_FLD(uRegVal, DP_P0CLK_TPG_MAIN_CONTROL, COLOR_RAMP_64L_64P,          1); break;
    case   HAL_DP_TESTPATTERN_COLOR_RAMP_64L_256P:         uRegVal = HWIO_OUT_FLD(uRegVal, DP_P0CLK_TPG_MAIN_CONTROL, COLOR_RAMP_64L_256P,         1); break;
    case   HAL_DP_TESTPATTERN_BLACK_WHITE_VERTICAL_LINES:  uRegVal = HWIO_OUT_FLD(uRegVal, DP_P0CLK_TPG_MAIN_CONTROL, BLACK_WHITE_VERTICAL_LINES,  1); break;
    case   HAL_DP_TESTPATTERN_GRAYSCALE_RAMP:              uRegVal = HWIO_OUT_FLD(uRegVal, DP_P0CLK_TPG_MAIN_CONTROL, GRAYSCALE_RAMP,              1); break;
    case   HAL_DP_TESTPATTERN_COLOR_SQUARE:                uRegVal = HWIO_OUT_FLD(uRegVal, DP_P0CLK_TPG_MAIN_CONTROL, COLOR_SQUARE,                1); break;
    case   HAL_DP_TESTPATTERN_CHECKERED_RECTANGLE_PATTERN: uRegVal = HWIO_OUT_FLD(uRegVal, DP_P0CLK_TPG_MAIN_CONTROL, CHECKERED_RECTANGLE_PATTERN, 1); break;
    case   HAL_DP_TESTPATTERN_BASIC_COLOR_CHANGING_PATTERN:uRegVal = HWIO_OUT_FLD(uRegVal, DP_P0CLK_TPG_MAIN_CONTROL, BASIC_COLOR_CHANGING_PATTERN,1); break;
    default:
        break;
    }
    out_dword(uPclkOffset + HWIO_DP_P0CLK_TPG_MAIN_CONTROL_ADDR, uRegVal);

    // Setup Format and Color
    uRegVal = 0;
    uRegVal = HWIO_OUT_FLD(uRegVal, DP_P0CLK_TPG_VIDEO_CONFIG, BPP, 1);
    uRegVal = HWIO_OUT_FLD(uRegVal, DP_P0CLK_TPG_VIDEO_CONFIG, RGB, 1);
    out_dword(uPclkOffset + HWIO_DP_P0CLK_TPG_VIDEO_CONFIG_ADDR, uRegVal);
  
    out_dword(uPclkOffset + HWIO_DP_P0CLK_BIST_ENABLE_ADDR,      1);
    out_dword(uPclkOffset + HWIO_DP_P0CLK_TIMING_ENGINE_EN_ADDR, 1);
  }


  return eStatus;
}

/****************************************************************************
*
** FUNCTION: HAL_DP_ReadVersionInfo()
*/
/*!
* \brief
*     Returns the version information
*
* \param [in]     eControllerId     - DP core ID
* \param [in/out] psDPVersionInfo   - Version information
*
* \retval None
*
****************************************************************************/
void HAL_DP_ReadVersionInfo(DP_ControllerIDType eControllerId,
                            HAL_HW_VersionType *psDPVersionInfo)
{
  uintPtr uOffset      = HAL_DP_GetRegBaseOffset(eControllerId);
  uint32  uVersionInfo = in_dword(uOffset + HWIO_DP_HW_VERSION_ADDR);

  if (NULL != psDPVersionInfo)
  {
    psDPVersionInfo->uMajorVersion    = HWIO_GETVAL(DP_HW_VERSION, uVersionInfo, MAJOR);
    psDPVersionInfo->uMinorVersion    = HWIO_GETVAL(DP_HW_VERSION, uVersionInfo, MINOR);
    psDPVersionInfo->uReleaseVersion  = HWIO_GETVAL(DP_HW_VERSION, uVersionInfo, STEP);
  }
}

/****************************************************************************
*
** FUNCTION: HAL_DP_CrossbarConfig()
*/
/*!
* \brief
*     Returns the version information
*
* \param [in] eControllerId  - DP core ID
* \param [in] ePhyId         - DP PHY ID
*
* \retval HAL_MDSS_ErrorType
*
****************************************************************************/
HAL_MDSS_ErrorType HAL_DP_CrossbarConfig(DP_ControllerIDType eControllerId, DP_DeviceIDType ePHYId)
{
  HAL_MDSS_ErrorType eStatus = HAL_MDSS_STATUS_SUCCESS;
  uint32 uCtrlShift  = 0;
  uint32 uCtrlMask   = 0;
  uint32 uPhyShift   = 0;
  uint32 uPhyMask    = 0;
  uint32 uPrevValue  = HWIO_MDP_DP_PHY_INTF_SELECT_IN;
  uint32 uPrevPhy    = 0;
  uint32 uPrevCtrl   = 0;
  uint32 uCurCtrl    = 0;
  uint32 uCurPhy     = 0;
  uint32 uCurValue   = uPrevValue;

  /*
  * DHDR SDP
  * DHDR_mem_pool_0 <==> DPTX0, stream 0
  * DHDR_mem_pool_1 <==> DPTX1, stream 0
  */
  out_dword(HWIO_MDP_DP_DHDR_MEM_POOL_INTF_SELECT_ADDR, 0x081);

  out_dword(HWIO_MDP_HDMI_DP_CORE_SELECT_ADDR, 0x01);   /* DP Mode */


  switch (eControllerId)
  {
  case DP_CONTROLLER_ID_DP_0:
    uCtrlShift = HWIO_MDP_DP_PHY_INTF_SELECT_DP_CTRL_INTF_0_SEL_SHFT;
    uCtrlMask  = HWIO_MDP_DP_PHY_INTF_SELECT_DP_CTRL_INTF_0_SEL_BMSK;
    uCurCtrl   = 1;
    break;
  case DP_CONTROLLER_ID_DP_1:
    uCtrlShift = HWIO_MDP_DP_PHY_INTF_SELECT_DP_CTRL_INTF_1_SEL_SHFT;
    uCtrlMask  = HWIO_MDP_DP_PHY_INTF_SELECT_DP_CTRL_INTF_1_SEL_BMSK;
    uCurCtrl   = 2;
    break;
  default:
    break;
  }

  switch (ePHYId)
  {
  case DP_DEVICE_ID_ALT_MODE_0:
    uPhyShift = HWIO_MDP_DP_PHY_INTF_SELECT_DP_PHY_INTF_0_SEL_SHFT;
    uPhyMask  = HWIO_MDP_DP_PHY_INTF_SELECT_DP_PHY_INTF_0_SEL_BMSK;
    uCurPhy   = 1;
    break;
  case DP_DEVICE_ID_ALT_MODE_1:
    uPhyShift = HWIO_MDP_DP_PHY_INTF_SELECT_DP_PHY_INTF_1_SEL_SHFT;
    uPhyMask  = HWIO_MDP_DP_PHY_INTF_SELECT_DP_PHY_INTF_1_SEL_BMSK;
    uCurPhy   = 2;
    break;
  case DP_DEVICE_ID_DP:
    uPhyShift = HWIO_MDP_DP_PHY_INTF_SELECT_DP_PHY_INTF_2_SEL_SHFT;
    uPhyMask  = HWIO_MDP_DP_PHY_INTF_SELECT_DP_PHY_INTF_2_SEL_BMSK;
    uCurPhy   = 3;
    break;
  default:
    break;
  }

  /*
   * If controller & phy are both non-zero then update both fields accordingly
   * If phy is zero, then it's a call to disconnect. Read hw to find which PHY was connected to the controller and
   * clear both those fields.
   */

  if (0 < uCurCtrl)
  {
    // get current value
    uPrevPhy = (uPrevValue & uCtrlMask) >> uCtrlShift;

    // check if different
    if (uCurPhy != uPrevPhy)
    {
      // clear current value
      uCurValue &= ~uCtrlMask;

      // Set new phy
      uCurValue |= (uCurPhy << uCtrlShift);

      if (0 == uCurPhy)
      {
        // Disconnect - Clear previous PHY
        uCurPhy  = uPrevPhy;
        uCurCtrl = 0;
      }
    }

    if (0 < uCurPhy)
    {
      // Get current value
      uPrevCtrl = (uPrevValue & uPhyMask) >> uPhyShift;
      
      // Check if different
      if (uCurCtrl != uPrevCtrl)
      {
        // Clear current value
        uCurValue &= ~uPhyMask;
      
        // Set new Ctrl
        uCurValue |= (uCurCtrl << uPhyShift);
      }
    }
    
    // Program the new value, only if it is different
    if (uCurValue != uPrevValue)
    {
      HWIO_MDP_DP_PHY_INTF_SELECT_OUT(uCurValue);
    }
  }

  return eStatus;
}
/****************************************************************************
*
** FUNCTION: HAL_DP_Get_Frame_CRC()
*/
/*!
* \brief
*     Enable CRC Capture
*
* \param [in]      eControllerId  - DP core ID
*
* \retval HAL_MDSS_ErrorType
*
****************************************************************************/
HAL_MDSS_ErrorType HAL_DP_Get_Frame_CRC(DP_ControllerIDType eControllerId, uint64 *pCRC)
{
  HAL_MDSS_ErrorType eStatus = HAL_MDSS_STATUS_SUCCESS;
  uintPtr            uOffset = HAL_DP_GetRegBaseOffset(eControllerId);
  uint32             uMISR_Ctrl;
  uint32             uCRC_RG = 0;
  uint32             uCRC_B  = 0;
  uint32             uIntr2;
  uint32             uCnt;
  uint32             uTiming_Eng;

  /*
   * Enable MISR to have FRAME_END generated
   * MISR_FRAME_COUNT determines how many frames to calculate
   * CRC over MISR. This alos means how many FRAME_END will be
   * generated. Since MISR is not exactly enabled at blanking period,
   * most likely the first FRAME_END does not have CRC ready (read as 0).
   * It is sure CRC will be populated at 2nd FRAME_END.
   * Therefore MISR_FRAME_COUNT is set to 2.
   */
  uMISR_Ctrl = in_dword(uOffset + HWIO_DP_MISR_CTRL_ADDR);
  uMISR_Ctrl = HWIO_OUT_FLD(uMISR_Ctrl, DP_MISR_CTRL, MISR_ENABLE, 1);
  uMISR_Ctrl = HWIO_OUT_FLD(uMISR_Ctrl, DP_MISR_CTRL, MISR_FRAME_COUNT, 2);
  uMISR_Ctrl = HWIO_OUT_FLD(uMISR_Ctrl, DP_MISR_CTRL, MISR_ACTIVE_ONLY, 1);
  out_dword(uOffset + HWIO_DP_MISR_CTRL_ADDR, uMISR_Ctrl);

  /* start CRC timing engine */
  uTiming_Eng = in_dword(uOffset + HWIO_DP_P0CLK_TIMING_ENGINE_EN_ADDR);
  uTiming_Eng = HWIO_OUT_FLD(uTiming_Eng, DP_P0CLK_TIMING_ENGINE_EN, FRAME_CRC_EN, 1);
  out_dword(uOffset + HWIO_DP_P0CLK_TIMING_ENGINE_EN_ADDR, uTiming_Eng);

  /*
   * Wait for frame_end interrupt 
   * The waiting time is varient since frame time is varient too and 
   * depend on FPS.
   * Frame time is 40ms for 25 FPS frame which mean 80ms waiting time
   * is required since CRC is ready at 2nd FRAME_END.
   * Therefore 100 ms waiting period here is enough to cover it.
   * NOTE: this polling loop will break once CRC captured.
   */ 
  uCnt = HAL_DP_WAIT_FOR_CRC;
  while(uCnt)
  {
     DP_OSAL_SleepMs(1);

     uIntr2 = in_dword(uOffset + HWIO_DP_INTERRUPT_STATUS_2_ADDR);
     if (uIntr2 & HWIO_DP_INTERRUPT_STATUS_2_FRAME_END_INT_BMSK)
     {
        /* read CRC */
        uCRC_RG    = in_dword(uOffset + HWIO_DP_PSR_CRC_RG_ADDR);
        uCRC_B     = in_dword(uOffset + HWIO_DP_PSR_CRC_B_ADDR);

        /* ack to clear frame_end bit */
        uIntr2 = (HWIO_DP_INTERRUPT_STATUS_2_FRAME_END_ACK_BMSK | HWIO_DP_INTERRUPT_STATUS_2_CRC_UPDATED_ACK_BMSK);
        out_dword(uOffset + HWIO_DP_INTERRUPT_STATUS_2_ADDR, uIntr2); 

        if (uCRC_RG != 0)
        {
           uint64 crc = uCRC_RG;
      
           crc <<= 32;
           crc |= uCRC_B;
           *pCRC = crc;
           break;
         }
     } 
     uCnt--;
  }

  /* disable MISR */
  uMISR_Ctrl = in_dword(uOffset + HWIO_DP_MISR_CTRL_ADDR);
  uMISR_Ctrl = HWIO_OUT_FLD(uMISR_Ctrl, DP_MISR_CTRL, MISR_ENABLE, 0);
  out_dword(uOffset + HWIO_DP_MISR_CTRL_ADDR, uMISR_Ctrl);

  /* disable CRC and timing engine */
  uTiming_Eng = in_dword(uOffset + HWIO_DP_P0CLK_TIMING_ENGINE_EN_ADDR);
  uTiming_Eng = HWIO_OUT_FLD(uTiming_Eng, DP_P0CLK_TIMING_ENGINE_EN, FRAME_CRC_EN, 0);
  out_dword(uOffset + HWIO_DP_P0CLK_TIMING_ENGINE_EN_ADDR, uTiming_Eng);

  if (uCnt == 0)
  {
    *pCRC = 0;
    eStatus = HAL_MDSS_MDP_STATUS_FAILED;
  }

  return eStatus;
}

#ifdef __cplusplus
}
#endif

