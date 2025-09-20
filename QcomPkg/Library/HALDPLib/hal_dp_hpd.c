/*=============================================================================
 
  File: hal_dp_hpd.c
 
  Source file for DP HPD hardware functionality 
   
  Copyright (c) 2016-2020 Qualcomm Technologies, Inc.  All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential.
=============================================================================*/

#ifdef __cplusplus
extern "C" {
#endif

/* -----------------------------------------------------------------------
** Includes
** ----------------------------------------------------------------------- */
#include "hal_dp_i.h"

/* -----------------------------------------------------------------------
** Macros
** ----------------------------------------------------------------------- */

/* -----------------------------------------------------------------------
** Static Variable
** ----------------------------------------------------------------------- */

/* -----------------------------------------------------------------------
** Private Functions
** ----------------------------------------------------------------------- */


/****************************************************************************
*
** FUNCTION: HAL_DP_HPD_Engine_Init()
*/
/*!
* \brief
*   The \b Initialize counters for HPD  
*
* \param [in]   eControllerId   - DP core ID
*
* \retval void
*
****************************************************************************/
static void HAL_DP_HPD_Engine_Init(DP_ControllerIDType eControllerId)
{
  /* Setup reference timer to generate 1 strobe per Us (from 19.2 MHz source) */
  uint32 uRefTime = (HWIO_DP_DP_HPD_REFTIMER_REFTIMER_ENABLE_BMSK | 0x13);
  uintPtr uOffset = HAL_DP_GetRegBaseOffset(eControllerId);

  /* Setup the reference timer */
  out_dword(uOffset + HWIO_DP_DP_HPD_REFTIMER_ADDR, uRefTime);

  /*
   * For detection thresholds, we rely on the following Hardware reset values :
   *     HPD_CONNECT_TIME = 100 ms,  HPD_GLITCH_TIME = 0.25ms
   *     IRQ_HPD_MAX_TIME = 2 ms,    HPD_DISCONNECT_TIME = 100ms
   */

  /* Clear any stale interrupt bits */
  out_dword(uOffset + HWIO_DP_DP_HPD_INT_ACK_ADDR, 0xF);
}



/* -----------------------------------------------------------------------
** Public Functions
** ----------------------------------------------------------------------- */


/****************************************************************************
*
** FUNCTION: HAL_DP_HPD_Engine_Enable()
*/
/*!
* \brief
*   The \b Enable Hot Plug Detect functionality 
*
* \param [in] eControllerId   - DP core ID
* \param [in] psConfig        - HPD configuration
*
* \retval void
*
****************************************************************************/
void HAL_DP_HPD_Engine_Enable(DP_ControllerIDType   eControllerId,
                              HAL_DP_HPDConfigType *psConfig)
{
  uint32  uValue  = 0;
  uintPtr uOffset = HAL_DP_GetRegBaseOffset(eControllerId);

  if (TRUE == psConfig->bEnable)
  {
    /* Setup HPD timer and detection thresholds */
    HAL_DP_HPD_Engine_Init(eControllerId);

    uValue |= HWIO_DP_DP_HPD_CTRL_DP_HPD_EN_BMSK;
  }

  if (TRUE == psConfig->bActiveLow)
  {
    uValue |= HWIO_DP_DP_HPD_CTRL_HPD_PIN_POLARITY_BMSK;
  }

  /* Enable/Disable HPD hardware */
  out_dword(uOffset + HWIO_DP_DP_HPD_CTRL_ADDR, uValue);

  uValue = in_dword(HWIO_MDP_DP_HPD_SELECT_ADDR);

  /* Route the HPD line based on DP device and controller */
  /* 0 means the controller is receiving dp_hot_plug_detect (native DP) */
  /* 1 means the controller is receiving debug_hot_plug_detect (type-c) */
  switch (psConfig->eDeviceID)
  {
    case DP_DEVICE_ID_DP:
      switch (eControllerId)
      {
        case DP_CONTROLLER_ID_DP_0:
          uValue = HWIO_OUT_FLD(uValue, MDP_DP_HPD_SELECT, DP0_SEL, 0);
          break;
        case DP_CONTROLLER_ID_DP_1:
          uValue = HWIO_OUT_FLD(uValue, MDP_DP_HPD_SELECT, DP1_SEL, 0);
          break;
        default:
          break;
      }
      break;
    case DP_DEVICE_ID_ALT_MODE_0:
    case DP_DEVICE_ID_ALT_MODE_1:
      switch (eControllerId)
      {
        case DP_CONTROLLER_ID_DP_0:
          uValue = HWIO_OUT_FLD(uValue, MDP_DP_HPD_SELECT, DP0_SEL, 1);
          break;
        case DP_CONTROLLER_ID_DP_1:
          uValue = HWIO_OUT_FLD(uValue, MDP_DP_HPD_SELECT, DP1_SEL, 1);
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }

  out_dword(HWIO_MDP_DP_HPD_SELECT_ADDR, uValue);
}

/****************************************************************************
*
** FUNCTION: HAL_DP_HPD_PanelPlugged()
*/
/*!
* \brief
*   The \b Checks whether panel is plugged in
*
* \param [in]   eControllerId   - DP core ID
*
* \retval True/False for connection
*
****************************************************************************/
bool32 HAL_DP_HPD_PanelPlugged(DP_ControllerIDType eControllerId)
{
  bool32  bDetected  = FALSE;
  uintPtr uOffset    = HAL_DP_GetRegBaseOffset(eControllerId);
  uint32  uIntStatus = in_dword(uOffset + HWIO_DP_DP_HPD_INT_STATUS_ADDR);

  if (uIntStatus & HWIO_DP_DP_HPD_INT_STATUS_DP_HPD_PLUG_INT_STATUS_BMSK)
  {
    bDetected = TRUE;
  }

  return bDetected;
}

/****************************************************************************
*
** FUNCTION: HAL_DP_HPD_PanelConnected()
*/
/*!
* \brief
*   The \b Checks whether panel is Connected in
*
* \param [in]   eControllerId   - DP core ID
*
* \retval True/False for connection
*
****************************************************************************/
HAL_DP_HPD_ConnectionType HAL_DP_HPD_Status(DP_ControllerIDType eControllerId)
{
  uintPtr uOffset          = HAL_DP_GetRegBaseOffset(eControllerId);
  HAL_DP_HPD_ConnectionType eConnect;

  eConnect = in_dword(uOffset + HWIO_DP_DP_HPD_INT_STATUS_ADDR);
  eConnect = (HAL_DP_HPD_ConnectionType)HWIO_GETVAL(DP_DP_HPD_INT_STATUS, eConnect, DP_HPD_STATE_STATUS);

  return eConnect;
}

#ifdef __cplusplus
}
#endif

