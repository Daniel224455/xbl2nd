/*=============================================================================
 
  File: hal_dp_hpd.c
 
  Source file for DP HPD hardware functionality 
   
  Copyright (c) 2016-2018 Qualcomm Technologies, Inc.  All Rights Reserved.
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
* \param [in] - none
*
* \retval void
*
****************************************************************************/
static void HAL_DP_HPD_Engine_Init(void)
{
  /* Setup reference timer to generate 1 strobe per Us (from 19.2 MHz source) */
  uint32 uRefTime  = (HWIO_DP_DP_HPD_REFTIMER_REFTIMER_ENABLE_BMSK | 0x13); 

  /* Setup the reference timer */
  HWIO_DP_DP_HPD_REFTIMER_OUT(uRefTime);

  /*
   * For detection thresholds, we rely on the following Hardware reset values :
   *     HPD_CONNECT_TIME = 100 ms,  HPD_GLITCH_TIME = 0.25ms
   *     IRQ_HPD_MAX_TIME = 2 ms,    HPD_DISCONNECT_TIME = 100ms
   */

  /* Clear any stale interrupt bits */
  HWIO_DP_DP_HPD_INT_ACK_OUT(0xF);
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
* \param [in] - bEnable TRUE/FALSE
*
* \retval void
*
****************************************************************************/
void HAL_DP_HPD_Engine_Enable(HAL_DP_HPDConfigType *psConfig)
{
  uint32 uValue = 0;
  
  if (TRUE == psConfig->bEnable)
  {
    /* Setup HPD timer and detection thresholds */
    HAL_DP_HPD_Engine_Init();
    
    uValue |= HWIO_DP_DP_HPD_CTRL_DP_HPD_EN_BMSK;
  }

  if (TRUE == psConfig->bActiveLow)
  {
    uValue |= HWIO_DP_DP_HPD_CTRL_HPD_PIN_POLARITY_BMSK;
  }

  /* Enable/Disable HPD hardware */
  HWIO_DP_DP_HPD_CTRL_OUT(uValue);
}

/****************************************************************************
*
** FUNCTION: HAL_DP_HPD_PanelPlugged()
*/
/*!
* \brief
*   The \b Checks whether panel is plugged in
*
* \param [in] -
*
* \retval True/False for connection
*
****************************************************************************/
bool32 HAL_DP_HPD_PanelPlugged(void)
{
  bool32 bDetected  = FALSE;
  uint32 uIntStatus = HWIO_DP_DP_HPD_INT_STATUS_IN;

  if (uIntStatus & HWIO_DP_DP_HPD_INT_STATUS_DP_HPD_PLUG_INT_STATUS_BMSK)
  {
    bDetected = TRUE;
  }

  return bDetected;
}

#ifdef __cplusplus
}
#endif

