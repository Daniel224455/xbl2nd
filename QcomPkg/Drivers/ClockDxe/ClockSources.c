/*
===========================================================================
*/
/**
  @file ClockSources.c

  Interface implementation file for the sources management of the
  clock device driver.
*/
/*
  ====================================================================

  Copyright (c) 2010-2018 QUALCOMM Technologies Incorporated.  All Rights Reserved.
  QUALCOMM Proprietary and Confidential.

  ====================================================================


  ====================================================================
*/


/*=========================================================================
      Include Files
==========================================================================*/

#include "DALDeviceId.h"
#include "ClockDriver.h"
#include "HALclk.h"

#ifdef WIN8EA
  #include "ClockQdss.h"
  #ifndef CONSOLE_DEBUGGING
    #include "PEP_wpp.h"
    #include "ClockSources.tmh"
  #endif
#else
  #include "QdssSWEvent.h"
  #include "tracer_event_ids.h"
#endif

/*=========================================================================
      Functions
==========================================================================*/

/* =========================================================================
**  Function : Clock_SourceNPARequest
** =========================================================================*/
/**
  Thin wrapper to make requests on an NPA-controlled source.

  @param *pSource [in] -- Pointer to a source node.
  @param  nRequest [in] -- NPA request to make.
  @param  bSuppressibleRequest [in] -- TRUE if suppressible; FALSE otherwise.

  @return
  None.
*/

static void Clock_SourceNPARequest
(
  ClockSourceNodeType *pSource,
  uint32               nRequest,
  boolean              bSuppressibleRequest
)
{
  npa_client_handle hNPA;

  if (bSuppressibleRequest)
  {
    hNPA = pSource->NPAHandleSuppressible;
  }
  else
  {
    hNPA = pSource->NPAHandle;
  }

  /*
   * If the NPA handle isn't created yet, then the resource hasn't been
   * created and our callback hasn't fired to create the client handle.
   * This requirement will be satisfied at the time of handle creation.
   */
  if (hNPA)
  {
    clock_npa_issue_scalar_request(hNPA, nRequest);
  }

} /* END Clock_SourceNPARequest */


/* =========================================================================
**  Function : Clock_EnableSourceOutputs
** =========================================================================*/
/**
  Enables all the source outputs specified in the BSP data for this source.

  @param *pSource [in] -- Pointer to a source node.

  @return
  None.
*/

static void Clock_EnableSourceOutputs
(
  ClockSourceNodeType *pSource
)
{
  uint32                  nConfigMask;
  HAL_clk_SourceDescType *pHALSource;

  /*-----------------------------------------------------------------------*/
  /* Return if source is stubbed on this platform.                         */
  /*-----------------------------------------------------------------------*/

  if (CLOCK_STUBBED(pSource))
  {
    return;
  }

  pHALSource = &pSource->HALSource;
  nConfigMask = pSource->nConfigMask;

  /*-----------------------------------------------------------------------*/
  /* Configure PLL outputs.                                                */
  /*-----------------------------------------------------------------------*/

  if (nConfigMask & CLOCK_CONFIG_PLL_AUX_OUTPUT_ENABLE)
  {
    HAL_clk_ConfigSource(pHALSource, HAL_CLK_CONFIG_PLL_AUX_OUTPUT_ENABLE);
  }

  if (nConfigMask & CLOCK_CONFIG_PLL_AUX2_OUTPUT_ENABLE)
  {
    HAL_clk_ConfigSource(pHALSource, HAL_CLK_CONFIG_PLL_AUX2_OUTPUT_ENABLE);
  }

  if (nConfigMask & CLOCK_CONFIG_PLL_EARLY_OUTPUT_ENABLE)
  {
    HAL_clk_ConfigSource(pHALSource, HAL_CLK_CONFIG_PLL_EARLY_OUTPUT_ENABLE);
  }

  if (nConfigMask & CLOCK_CONFIG_PLL_XO_DOUBLER_DCC)
  {
    HAL_clk_ConfigSource(pHALSource, HAL_CLK_CONFIG_XO_DOUBLER_DCC);
  }

  if (nConfigMask & CLOCK_CONFIG_PLL_EVEN_OUTPUT_ENABLE)
  {
    HAL_clk_ConfigSource(pHALSource, HAL_CLK_CONFIG_PLL_EVEN_OUTPUT_ENABLE);
  }

  if (nConfigMask & CLOCK_CONFIG_PLL_ODD_OUTPUT_ENABLE)
  {
    HAL_clk_ConfigSource(pHALSource, HAL_CLK_CONFIG_PLL_ODD_OUTPUT_ENABLE);
  }

  /*
   * Always enable the main output.
   */
  HAL_clk_ConfigSource(pHALSource, HAL_CLK_CONFIG_PLL_MAIN_OUTPUT_ENABLE);

} /* END Clock_EnableSourceOutputs */


/* =========================================================================
**  Function : Clock_SourceOn
** =========================================================================*/
/**
  Enables a source.

  This function performs the final operations required to turn on a
  clock source.  It handles deciding whether to use an NPA client request,
  a HAL source vote, or a direct HAL source enable call.

  @param *pDrvCtxt [in] -- Handle to the DAL driver context.
  @param *pSource [in] -- Pointer to a source node.
  @param bSuppressibleRequest [in] -- TRUE if this is a source request is for a suppressible clock.

  @return
  None.
*/

static void Clock_SourceOn
(
  ClockDrvCtxt        *pDrvCtxt,
  ClockSourceNodeType *pSource,
  boolean              bSuppressibleRequest
)
{
  ClockVRegRequestType  VRegRequest;
  boolean               bResult, bStub;
  boolean               bNeedsCal;
  rail_voltage_level    eEnableCorner, eActiveCorner;


  if (CLOCK_SOURCE_NPA_CONTROLLED(pSource))
  {
    Clock_SourceNPARequest(pSource, CLOCK_SOURCE_REQUIRED, bSuppressibleRequest);
  }
  else
  {
    if (pSource->pActiveFreqConfig == NULL)
    {
      ULOG_RT_PRINTF_1 (pDrvCtxt->hClockLog,
        "DALSYS_LOGEVENT_FATAL_ERROR: Cannot turn on source[%s] that hasn't been configured.",
        pSource->szName);
      ERR_FATAL("Clock_SourceOn", 0, 0, 0);
      return;
    }

    /*-----------------------------------------------------------------------*/
    /* Determine calibration requirements.                                   */
    /*-----------------------------------------------------------------------*/

    eActiveCorner = (rail_voltage_level)pSource->pActiveFreqConfig->eVRegLevel;

    bNeedsCal = !HAL_clk_IsSourceCalibrated(&pSource->HALSource);
    if (!bNeedsCal)
    {
      /*
       * Source doesn't require calibration, so the enable corner is simply
       * the active corner.
       */
      eEnableCorner = eActiveCorner;
    }
    else
    {
      if (!pSource->pCalibrationFreqConfig)
      {
        ULOG_RT_PRINTF_1 (pDrvCtxt->hClockLog,
          "DALSYS_LOGEVENT_FATAL_ERROR: Source[%s] requires calibration, but no calibration config.",
          pSource->szName);
        ERR_FATAL("Clock_SourceOn", 0, 0, 0);
        return;
      }

      /*
       * Source requires calibration, so the enable corner needs to be
       * high enough to support the calibration frequency.
       */
      eEnableCorner =
        MAX(eActiveCorner, (rail_voltage_level)pSource->pCalibrationFreqConfig->eVRegLevel);
    }

    /*-----------------------------------------------------------------------*/
    /* Enable source.                                                        */
    /*-----------------------------------------------------------------------*/

    bStub = CLOCK_STUBBED(pSource);

    /*
     * Vote for voltage required to enable PLL, which may be elevated
     * due to calibration requirements.
     */
    VRegRequest.eVRegLevel = eEnableCorner;
    VRegRequest.bSuppressible = bSuppressibleRequest;
    Clock_VoltageRequest(pDrvCtxt, pSource->pRail, &pSource->VRegRequest, &VRegRequest);

    if (!bStub)
    {
      /*
       * Set calibration frequency if required. If the PLL supports
       * auto-calibration, then this will just set the values HW will use
       * when automatically calibrating during the enable below. If
       * auto-calibration is not supported, then this will actually
       * perform the calibration.
       *
       * Don't set the calibration config if the source is READ_ONLY,
       * since shared PLLs need this set during boot. Shared PLLs must either
       * not require re-calibration or must support auto-cal.
       */
      if (bNeedsCal && !CLOCK_FLAG_IS_SET(pSource, READ_ONLY))
      {
        bResult = HAL_clk_SetSourceCalConfig(&pSource->HALSource,
          &pSource->pCalibrationFreqConfig->HALConfig);
        if (bResult != TRUE)
        {
          ULOG_RT_PRINTF_1 (pDrvCtxt->hClockLog,
            "DALSYS_LOGEVENT_FATAL_ERROR: Failed to calibrate source[%s].", pSource->szName);
          ERR_FATAL("Clock_SourceOn", 0, 0, 0);
          return;
        }
      }

      /*
       * Enable the source.
       */
      if (pSource->nConfigMask & CLOCK_CONFIG_PLL_FSM_MODE_ENABLE)
      {
        bResult = HAL_clk_EnableSourceVote(&pSource->HALSource);
        if (bResult != TRUE)
        {
          ULOG_RT_PRINTF_1 (pDrvCtxt->hClockLog,
            "DALSYS_LOGEVENT_FATAL_ERROR: Unable to enable source[%s] vote", pSource->szName);
          ERR_FATAL("Clock_SourceOn", 0, 0, 0);
          return;
        }
      }
      else
      {
        bResult = HAL_clk_EnableSource(&pSource->HALSource,
          pSource->eDisableMode, NULL);
        if (bResult != TRUE)
        {
          ULOG_RT_PRINTF_1 (pDrvCtxt->hClockLog,
            "DALSYS_LOGEVENT_FATAL_ERROR: Unable to enable source[%s]", pSource->szName);
          ERR_FATAL("Clock_SourceOn", 0, 0, 0);
          return;
        }
      }
    }

    /*
     * If the PLL was enabled at an elevated voltage for calibration, then
     * restore back to the active requirement.
     */
    if (eActiveCorner != eEnableCorner)
    {
      VRegRequest.eVRegLevel = eActiveCorner;
      Clock_VoltageRequest(pDrvCtxt, pSource->pRail, &pSource->VRegRequest, &VRegRequest);
    }
  }

  /*-----------------------------------------------------------------------*/
  /* Logging.                                                              */
  /*-----------------------------------------------------------------------*/

  if (CLOCK_FLAG_IS_SET(pSource, LOG_STATE_CHANGE) ||
      CLOCK_GLOBAL_FLAG_IS_SET(LOG_SOURCE_STATE_CHANGE))
  {
    ULOG_RT_PRINTF_1 (pDrvCtxt->hClockLog,
                      "%s (Enabled: 1)", pSource->szName);
  }

  /*-----------------------------------------------------------------------*/
  /* Log source enable.                                                    */
  /*-----------------------------------------------------------------------*/

  #ifdef WIN8EA
  if (pDrvCtxt->bQdssEnabled)
  {
    PEP_CLK_TRACE_SOURCE_STATUS((uint32)&pSource->HALSource, 1);
  }
  #else
  QDSS_SWEVENT(CLOCK_EVENT_SOURCE_STATUS, &pSource->HALSource);
  #endif

} /* END Clock_SourceOn */


/* =========================================================================
**  Function : Clock_SourceOff
** =========================================================================*/
/**
  Disables a source.

  This function performs the final operations required to turn off a
  clock source.  It handles deciding whether to use an NPA client request,
  a HAL source vote, or a direct HAL source disable call.

  @param *pDrvCtxt [in] -- Handle to the DAL driver context.
  @param *pSource [in] -- Pointer to a source node.
  @param bSuppressibleRequest [in] -- TRUE if this is a source request is for a suppressible clock.

  @return
  None.
*/

static void Clock_SourceOff
(
  ClockDrvCtxt        *pDrvCtxt,
  ClockSourceNodeType *pSource,
  boolean              bSuppressibleRequest
)
{
  ClockVRegRequestType VRegRequest;
  boolean              bStub;

  /*-----------------------------------------------------------------------*/
  /* Check if source is stubbed on this platform.                          */
  /*-----------------------------------------------------------------------*/

  bStub = CLOCK_STUBBED(pSource);

  /*-----------------------------------------------------------------------*/
  /* Disable the source using an NPA client if it exists, or HAL call      */
  /* otherwise.                                                            */
  /*-----------------------------------------------------------------------*/

  if (CLOCK_SOURCE_NPA_CONTROLLED(pSource))
  {
    Clock_SourceNPARequest(pSource, CLOCK_SOURCE_NOT_REQUIRED, bSuppressibleRequest);
  }
  else
  {
    if (pSource->nConfigMask & CLOCK_CONFIG_PLL_FSM_MODE_ENABLE)
    {
      if (bStub != TRUE)
      {
        HAL_clk_DisableSourceVote(&pSource->HALSource);
      }
    }
    else
    {
      if (bStub != TRUE)
      {
        HAL_clk_DisableSource(&pSource->HALSource, pSource->eDisableMode, NULL);
      }
    }

    /*-----------------------------------------------------------------------*/
    /* Deassert the voltage requirement.                                     */
    /* For PLL's in standby mode:                                            */
    /*  They require no voltage vote since they operate at retention voltage.*/
    /* For PLL's in save/restore mode:                                       */
    /*  They require no voltage vote since the bias is turned off.           */
    /*-----------------------------------------------------------------------*/

    VRegRequest.eVRegLevel = RAIL_VOLTAGE_LEVEL_OFF;
    VRegRequest.bSuppressible = bSuppressibleRequest;
    Clock_VoltageRequest(pDrvCtxt, pSource->pRail, &pSource->VRegRequest, &VRegRequest);
  }

  /* Logging */
  if (CLOCK_FLAG_IS_SET(pSource, LOG_STATE_CHANGE) ||
      CLOCK_GLOBAL_FLAG_IS_SET(LOG_SOURCE_STATE_CHANGE))
  {
    ULOG_RT_PRINTF_1 (pDrvCtxt->hClockLog,
                      "%s (Enabled: 0)", pSource->szName);
  }

  /*-----------------------------------------------------------------------*/
  /* Log source disable.                                                   */
  /*-----------------------------------------------------------------------*/

  #ifdef WIN8EA
  if (pDrvCtxt->bQdssEnabled)
  {
    PEP_CLK_TRACE_SOURCE_STATUS((uint32)&pSource->HALSource, 0);
  }
  #else
  QDSS_SWEVENT(CLOCK_EVENT_SOURCE_STATUS, &pSource->HALSource);
  #endif

} /* END Clock_SourceOff */


/* =========================================================================
**  Function : Clock_EnableSourceInternal
** =========================================================================*/
/*
  See ClockDriver.h
*/

DALResult Clock_EnableSourceInternal
(
  ClockDrvCtxt        *pDrvCtxt,
  ClockSourceNodeType *pSource,
  boolean              bSuppressibleRequest
)
{
  ClockVRegRequestType VRegRequest;
  DALResult            eResult;

  /*-----------------------------------------------------------------------*/
  /* Make sure the source handle is valid.                                 */
  /*-----------------------------------------------------------------------*/

  if (pSource == NULL)
  {
    return DAL_ERROR;
  }

  /*-----------------------------------------------------------------------*/
  /* Check if source requires initial configuration before enabling.       */
  /*-----------------------------------------------------------------------*/

  if (!CLOCK_FLAG_IS_SET(pSource, INITIALIZED))
  {
    eResult = Clock_InitSource(pDrvCtxt, pSource, NULL);
    if (eResult != DAL_SUCCESS)
    {
      return DAL_ERROR_INTERNAL;
    }
  }

  /*-----------------------------------------------------------------------*/
  /* Process insuppressible requests.                                      */
  /*-----------------------------------------------------------------------*/

  if (!bSuppressibleRequest)
  {
    if (pSource->nReferenceCount == 0)
    {
      /*
       * If the source has a source, then recursively enable that.
       * If the current state is enabled for a insuppressible clock, then we
       * need to release the previously registered vote.
       */
      if (pSource->pSource != NULL)
      {
        eResult = Clock_EnableSourceInternal(pDrvCtxt, pSource->pSource, FALSE);
        if (eResult != DAL_SUCCESS)
        {
          return eResult;
        }
      }

      /*
       * Turn on the source if it is not already on.  If there is already
       * a suppressible reference count we just need to update the NPA state
       * and voltage request with the insuppressible requirements.
       */
      if (pSource->nReferenceCountSuppressible == 0)
      {
        Clock_SourceOn(pDrvCtxt, pSource, bSuppressibleRequest);
      }
      else
      {
        if (CLOCK_SOURCE_NPA_CONTROLLED(pSource))
        {
          Clock_SourceNPARequest(pSource, CLOCK_SOURCE_REQUIRED, bSuppressibleRequest);
        }
        else
        {
          VRegRequest.eVRegLevel = pSource->pActiveFreqConfig->eVRegLevel;
          VRegRequest.bSuppressible = FALSE;
          Clock_VoltageRequest(pDrvCtxt, pSource->pRail, &pSource->VRegRequest, &VRegRequest);
        }
      }
    }

    /*
     * Increment the reference count.
     */
    pSource->nReferenceCount++;
  }

  /*-----------------------------------------------------------------------*/
  /* Process suppressible request.                                         */
  /*-----------------------------------------------------------------------*/

  else
  {
    if (pSource->nReferenceCountSuppressible == 0)
    {
      /*
       * If the source has a source, then recursively enable that.
       */
      if (pSource->pSource != NULL)
      {
        eResult = Clock_EnableSourceInternal(pDrvCtxt, pSource->pSource, TRUE);
        if (eResult != DAL_SUCCESS)
        {
          return eResult;
        }
      }

      /*
       * If the source is not already enabled by an insuppressible request
       * then enable it.  Use "0" for any NPA request to indicate
       * suppressible-based and that the source is not required. If there is
       * already an insuppressible reference count, then just update the voltage
       * request with the suppressible requirement.
       */
      if (pSource->nReferenceCount == 0)
      {
        Clock_SourceOn(pDrvCtxt, pSource, TRUE);
      }
    }

    /*
     * Increment the reference count.
     */
    pSource->nReferenceCountSuppressible++;
  }

  return DAL_SUCCESS;

} /* END Clock_EnableSourceInternal */


/* =========================================================================
**  Function : Clock_DisableSourceInternal
** =========================================================================*/
/*
  See ClockDriver.h
*/

DALResult Clock_DisableSourceInternal
(
  ClockDrvCtxt        *pDrvCtxt,
  ClockSourceNodeType *pSource,
  boolean              bSuppressibleRequest,
  boolean              bDoNotDisable
)
{
  ClockVRegRequestType VRegRequest;
  DALResult eResult;

  /*-----------------------------------------------------------------------*/
  /* Make sure the source handle is valid.                                 */
  /*-----------------------------------------------------------------------*/

  if (pSource == NULL)
  {
    return DAL_ERROR;
  }

  bDoNotDisable =
    bDoNotDisable ||
    CLOCK_FLAG_IS_SET(pSource, DO_NOT_DISABLE) ||
    CLOCK_GLOBAL_FLAG_IS_SET(DO_NOT_DISABLE_SOURCES);

  /*-----------------------------------------------------------------------*/
  /* Process insuppressible requests.                                      */
  /*-----------------------------------------------------------------------*/

  if (!bSuppressibleRequest)
  {
    /*
     * Decrement insuppressible reference count.
     */
    if (pSource->nReferenceCount > 0)
    {
      pSource->nReferenceCount--;
    }

    /*
     * If the count is zero and the DO_NOT_DISABLE bits aren't set, we can
     * disable the source.
     */
    if (pSource->nReferenceCount == 0)
    {
      if (!bDoNotDisable)
      {
        /*
         * If there is no suppressible reference count then disable the source.
         * If there are suppressible requests on this source then we just remove
         * the insuppressible requirement from the NPA state and voltage request.
         */
        if (pSource->nReferenceCountSuppressible == 0)
        {
          Clock_SourceOff(pDrvCtxt, pSource, FALSE);
        }
        else
        {
          if (CLOCK_SOURCE_NPA_CONTROLLED(pSource))
          {
            Clock_SourceNPARequest(pSource, CLOCK_SOURCE_NOT_REQUIRED, bSuppressibleRequest);
          }
          else
          {
            VRegRequest.eVRegLevel = pSource->pActiveFreqConfig->eVRegLevel;
            VRegRequest.bSuppressible = TRUE;
            Clock_VoltageRequest(pDrvCtxt, pSource->pRail, &pSource->VRegRequest, &VRegRequest);
          }
        }
      }

      /*
       * If the source has a source, then recursively disable that.
       */
      if (pSource->pSource != NULL)
      {
        eResult = Clock_DisableSourceInternal(pDrvCtxt, pSource->pSource, FALSE, bDoNotDisable);
        if (eResult != DAL_SUCCESS)
        {
          return eResult;
        }
      }
    }
  }

  /*-----------------------------------------------------------------------*/
  /* Process suppressible request.                                         */
  /*-----------------------------------------------------------------------*/

  else
  {
    if (pSource->nReferenceCountSuppressible > 0)
    {
      pSource->nReferenceCountSuppressible--;
    }

    if (pSource->nReferenceCountSuppressible == 0)
    {
      if (!bDoNotDisable)
      {
        /*
         * Disable the source if there aren't any insuppressible references. If
         * there is an insuppressible reference count, then just remove the
         * suppressible voltage request.
         */
        if (pSource->nReferenceCount == 0)
        {
          Clock_SourceOff(pDrvCtxt, pSource, TRUE);
        }
      }

      /*
       * If the source has a source, then recursively disable that.
       */
      if (pSource->pSource != NULL)
      {
        eResult = Clock_DisableSourceInternal(pDrvCtxt, pSource->pSource, TRUE, bDoNotDisable);
        if (eResult != DAL_SUCCESS)
        {
          return eResult;
        }
      }
    }
  }

  return DAL_SUCCESS;

} /* END Clock_DisableSourceInternal */


/* =========================================================================
**  Function : Clock_ConfigSource
** =========================================================================*/
/*
  See ClockDriver.h
*/

DALResult Clock_ConfigSource
(
  ClockDrvCtxt                    *pDrvCtxt,
  ClockSourceNodeType             *pSource,
  const ClockSourceFreqConfigType *pFreqConfig
)
{
  const ClockSourceFreqConfigType *pActiveFreqConfig;
  ClockVRegRequestType             VRegRequest;
  DALResult                        eResult;
  boolean                          bResult, bStub;

  /*-----------------------------------------------------------------------*/
  /* Make sure the source handle is valid.                                 */
  /*-----------------------------------------------------------------------*/

  if (pSource == NULL || pFreqConfig == NULL)
  {
    return DAL_ERROR;
  }

  /*-----------------------------------------------------------------------*/
  /* Make sure source is initialized.                                      */
  /*-----------------------------------------------------------------------*/

  if (!CLOCK_FLAG_IS_SET(pSource, INITIALIZED))
  {
    eResult = Clock_InitSource(pDrvCtxt, pSource, pFreqConfig);
    if (eResult != DAL_SUCCESS)
    {
      return DAL_ERROR_INTERNAL;
    }
  }

  /*-----------------------------------------------------------------------*/
  /* Short-circuit if this config is already active.                       */
  /*-----------------------------------------------------------------------*/

  pActiveFreqConfig = pSource->pActiveFreqConfig;
  if (pFreqConfig == pActiveFreqConfig)
  {
    return DAL_SUCCESS;
  }

  /*-----------------------------------------------------------------------*/
  /* Set the new frequency configuration.                                  */
  /*-----------------------------------------------------------------------*/

  bStub = CLOCK_STUBBED(pSource);

  /*
   * If source is off, then perform a normal, offline configuration.
   */
  if ((pSource->nReferenceCount == 0) &&
      (pSource->nReferenceCountSuppressible == 0))
  {
    if (bStub != TRUE)
    {
      bResult = HAL_clk_ConfigPLL(&pSource->HALSource, &pFreqConfig->HALConfig,
        HAL_CLK_SOURCE_CONFIG_MODE_NORMAL);
      if (bResult != TRUE)
      {
        return DAL_ERROR_INTERNAL;
      }
    }
  }

  /*
   * If source is on, then dynamically slew from old frequency to new frequency.
   */
  else
  {
    /*
     * Verify active frequency configured to slew from.
     */
    if (pActiveFreqConfig == NULL)
    {
      return DAL_ERROR;
    }

    /*
     * Increase the voltage vote if the new config has a higher requirement.
     */
    if (pFreqConfig->eVRegLevel > pActiveFreqConfig->eVRegLevel)
    {
      VRegRequest.eVRegLevel = pFreqConfig->eVRegLevel;
      VRegRequest.bSuppressible = pSource->VRegRequest.bSuppressible;
      Clock_VoltageRequest(pDrvCtxt, pSource->pRail, &pSource->VRegRequest, &VRegRequest);
    }

    /*
     * Slew to new configuration.
     */
    if (bStub != TRUE)
    {
      bResult = HAL_clk_ConfigPLL(&pSource->HALSource, &pFreqConfig->HALConfig,
        HAL_CLK_SOURCE_CONFIG_MODE_SLEW);
      if (bResult != TRUE)
      {
        return DAL_ERROR_INTERNAL;
      }
    }

    /*
     * Decrease the voltage vote if the new config has a lower requirement.
     */
    if (pFreqConfig->eVRegLevel < pActiveFreqConfig->eVRegLevel)
    {
      VRegRequest.eVRegLevel = pFreqConfig->eVRegLevel;
      VRegRequest.bSuppressible = pSource->VRegRequest.bSuppressible;
      Clock_VoltageRequest(pDrvCtxt, pSource->pRail, &pSource->VRegRequest, &VRegRequest);
    }
  }

  /*-----------------------------------------------------------------------*/
  /* Update state.                                                         */
  /*-----------------------------------------------------------------------*/

  pSource->pActiveFreqConfig = pFreqConfig;

  /* TODO: Properly handle dynamically switching between reference sources. */
  pSource->pSource = pFreqConfig->pSource;

  /*-----------------------------------------------------------------------*/
  /* Log source config change.                                             */
  /*-----------------------------------------------------------------------*/

  if (CLOCK_FLAG_IS_SET(pSource, LOG_FREQ_CHANGE) ||
      CLOCK_GLOBAL_FLAG_IS_SET(LOG_SOURCE_FREQ_CHANGE))
  {
    ULOG_RT_PRINTF_2 (pDrvCtxt->hClockLog,
                      "%s (Freq: %lu)",
                      pSource->szName, pFreqConfig->nFreqHz);
  }

  return DAL_SUCCESS;

} /* END Clock_ConfigSource */


/* =========================================================================
**  Function : Clock_SourceResourceAvailableCallback
** =========================================================================*/
/**
  Callback when source NPA resource is ready.

  @param *pContext [in] -- Context pointer, the source pointer in this case.
  @param  nEventType [in] -- 0 for this callback type.
  @param  pResourceName [in] -- Pointer to resource name string.
  @param  nResourceNameLen [in] -- Length of pResourceName string.

  @return
  NULL

  @dependencies
  None.
*/

static void Clock_SourceResourceAvailableCallback
(
  void         *pContext,
  unsigned int  nEventType,
  void         *pResourceName,
  unsigned int  nResourceNameLen
)
{
  ClockSourceNodeType *pSource = pContext;
  const char          *szResource = pResourceName;
  ClockDrvCtxt        *pDrvCtxt = Clock_GetDrvCtxt();

  /*
   * Multiple callbacks may be registered. Nothing to do after first
   * callback has fired.
   */
  if (pSource->NPAHandle)
  {
    return;
  }

  /*
   * Create client handles.
   */
  pSource->NPAHandle = npa_create_sync_client(szResource,
    CLOCK_NPA_CLIENT_NAME, NPA_CLIENT_REQUIRED);
  pSource->NPAHandleSuppressible = npa_create_sync_client(szResource,
    CLOCK_NPA_CLIENT_NAME, NPA_CLIENT_SUPPRESSIBLE);

  if (!pSource->NPAHandle || !pSource->NPAHandleSuppressible)
  {
    ULOG_RT_PRINTF_1 (pDrvCtxt->hClockLog,
                      "DALSYS_LOGEVENT_FATAL_ERROR: Unable to create NPA handles for %s.",
                      pSource->szName);
    ERR_FATAL("Clock_SourceResourceAvailableCallback", 0, 0, 0);
    return;
  }

  /*
   * Vote for current requirements.
   */
  if (pSource->nReferenceCount > 0)
  {
    clock_npa_issue_scalar_request(pSource->NPAHandle, CLOCK_SOURCE_REQUIRED);
  }
  if (pSource->nReferenceCountSuppressible > 0)
  {
    clock_npa_issue_scalar_request(pSource->NPAHandleSuppressible, CLOCK_SOURCE_REQUIRED);
  }

} /* END Clock_SourceResourceAvailableCallback */


/* =========================================================================
**  Function : Clock_InitSource
** =========================================================================*/
/*
  See ClockDriver.h
*/

DALResult Clock_InitSource
(
  ClockDrvCtxt                    *pDrvCtxt,
  ClockSourceNodeType             *pSource,
  const ClockSourceFreqConfigType *pSourceFreqConfig
)
{
  DALResult eResult;
  boolean   bStub;
  uint32    i;

  /*
   * Verify input parameters.
   */
  if (pSource == NULL)
  {
    return DAL_ERROR;
  }

  /*
   * Nothing to do if source already initialized.
   */
  if (CLOCK_FLAG_IS_SET(pSource, INITIALIZED))
  {
    return DAL_SUCCESS;
  }

  /*
   * If a source is NPA-controlled, then just setup a callback for the
   * resource to become available and return.
   */
  if (CLOCK_SOURCE_NPA_CONTROLLED(pSource))
  {
    npa_resource_available_cb(
      pSource->szName,
      Clock_SourceResourceAvailableCallback,
      pSource);
    goto done;
  }

  /*-----------------------------------------------------------------------*/
  /* Detect initial settings.                                              */
  /*-----------------------------------------------------------------------*/

  /*
   * If a source doesn't have any BSP data (like for XO), then no
   * initialization to be performed.
   */
  if (pSource->aBSP == NULL)
  {
    goto done;
  }

  /*
   * Hookup the calibration frequency config if applicable.
   */
  if (pSource->pCalibrationFreqConfig != NULL)
  {
    eResult = Clock_FindSourceConfig(pSource->pCalibrationFreqConfig,
      pSource->nCalibrationFreqConfigLen, 0, CLOCK_FREQUENCY_HZ_CLOSEST,
      &pSource->pCalibrationFreqConfig);
    if (eResult != DAL_SUCCESS)
    {
      return eResult;
    }

    pSource->nCalibrationFreqConfigLen = 1;
  }

  /*
   * Hookup version-specific PLL settings.
   */
  if (pSource->pRegSettings)
  {
    for (i = 0; i < pSource->nRegSettingsLen; i++, pSource->pRegSettings++)
    {
      if (Clock_IsBSPSupported(&pSource->pRegSettings->HWVersion))
      {
        break;
      }
    }

    if (i == pSource->nRegSettingsLen)
    {
      return DAL_ERROR;
    }

    pSource->nRegSettingsLen = 1;
  }

  /*
   * If we successfully detect the configuration then don't re-initialize.
   */
  eResult = Clock_DetectSourceConfig(pDrvCtxt, pSource);
  if (eResult == DAL_SUCCESS)
  {
    goto done;
  }

  /*
   * If no explicit source configuration supplied, then use the first
   * supported configuration.
   */
  if (!pSourceFreqConfig)
  {
    eResult = Clock_FindSourceConfig(pSource->aBSP, pSource->nBSPLen,
      0, CLOCK_FREQUENCY_HZ_AT_LEAST, &pSourceFreqConfig);
    if ((eResult != DAL_SUCCESS) || (pSourceFreqConfig == NULL))
    {
      return eResult;
    }
  }

  /*
   * If source is read-only, then it should already be configured by an
   * earlier SW image. Just hookup the active configuration and don't
   * touch HW.
   */
  if (CLOCK_FLAG_IS_SET(pSource, READ_ONLY))
  {
    pSource->pActiveFreqConfig = pSourceFreqConfig;
    pSource->pSource = pSourceFreqConfig->pSource;
    goto done;
  }

  /*-----------------------------------------------------------------------*/
  /* Apply the initial active config.                                      */
  /*-----------------------------------------------------------------------*/

  bStub = CLOCK_STUBBED(pSource);

  /*
   * Enable the FSM before Clock_ConfigSource, since HAL_clk_ConfigPLL
   * often relies on this bit being set in the register to setup diffently
   * for FSM mode.
   */
  if (pSource->nConfigMask & CLOCK_CONFIG_PLL_FSM_MODE_ENABLE)
  {
    if (bStub != TRUE)
    {
      HAL_clk_ConfigSource(&pSource->HALSource, HAL_CLK_CONFIG_PLL_FSM_MODE_ENABLE);
    }
  }

  /*
   * Set static PLL register settings.
   */
  if (pSource->pRegSettings && !bStub)
  {
    HAL_clk_SetSourceRegSettings(&pSource->HALSource,
      &pSource->pRegSettings->HALSettings);
  }

  /*
   * Set initial frequency configuration.
   *
   * Set initialized flag before calling Clock_ConfigSource, so it doesn't
   * call back to Clock_InitSource.
   */
  CLOCK_FLAG_SET(pSource, INITIALIZED);
  eResult = Clock_ConfigSource(pDrvCtxt, pSource, pSourceFreqConfig);
  if (eResult != DAL_SUCCESS)
  {
    return eResult;
  }

  /*
   * Enable the required outputs.
   */
  Clock_EnableSourceOutputs(pSource);

  /*-----------------------------------------------------------------------*/
  /* Done.                                                                 */
  /*-----------------------------------------------------------------------*/

done:
  CLOCK_FLAG_SET(pSource, INITIALIZED);
  return DAL_SUCCESS;

} /* END Clock_InitSource */


/* =========================================================================
**  Function : Clock_FindSourceConfig
** =========================================================================*/
/*
  See ClockDriver.h
*/

DALResult Clock_FindSourceConfig
(
  const ClockSourceFreqConfigType  *aBSP,
  uint32                            nBSPLen,
  uint32                            nFreqHz,
  ClockFrequencyType                eMatch,
  const ClockSourceFreqConfigType **pMatchingConfig
)
{
  uint32                           nAtLeastFrequency = 0xFFFFFFFF;
  const ClockSourceFreqConfigType *pAtLeastConfig = NULL;
  uint32                           nAtMostFrequency = 0;
  const ClockSourceFreqConfigType *pAtMostConfig = NULL;
  const ClockSourceFreqConfigType *pBSPConfig;
  uint32                           i;

  /* Make sure the list of configurations isn't empty */
  if (aBSP == NULL ||
      pMatchingConfig == NULL)
  {
    return DAL_ERROR;
  }

  /*
   * Otherwise find config based on the nFreqHz field.
   * Loop over all configurations.
   */
  for (i = 0; i < nBSPLen; i++)
  {
    pBSPConfig = &aBSP[i];

    /* Stop if we find an exact match */
    if (pBSPConfig->nFreqHz == nFreqHz)
    {
      *pMatchingConfig = pBSPConfig;
      return DAL_SUCCESS;
    }

    /* Look for the lowest at-least frequency */
    if ((pBSPConfig->nFreqHz > nFreqHz) &&
        (pBSPConfig->nFreqHz < nAtLeastFrequency))
    {
      nAtLeastFrequency = pBSPConfig->nFreqHz;
      pAtLeastConfig = pBSPConfig;
    }
    /* Look for the highest at-most frequency */
    else if ((pBSPConfig->nFreqHz < nFreqHz) &&
             (pBSPConfig->nFreqHz > nAtMostFrequency))
    {
      nAtMostFrequency = pBSPConfig->nFreqHz;
      pAtMostConfig = pBSPConfig;
    }
  }

  /* Select a config based on the match criteria */
  switch (eMatch)
  {
    case CLOCK_FREQUENCY_HZ_AT_LEAST:
      if (pAtLeastConfig != NULL)
      {
        *pMatchingConfig = pAtLeastConfig;
        return DAL_SUCCESS;
      }
      break;

    case CLOCK_FREQUENCY_HZ_AT_MOST:
      if (pAtMostConfig != NULL)
      {
        *pMatchingConfig = pAtMostConfig;
        return DAL_SUCCESS;
      }
      break;

    case CLOCK_FREQUENCY_HZ_CLOSEST:
      if ((pAtLeastConfig != NULL) && (pAtMostConfig == NULL))
      {
        *pMatchingConfig = pAtLeastConfig;
        return DAL_SUCCESS;
      }
      else if ((pAtMostConfig != NULL) && (pAtLeastConfig == NULL))
      {
        *pMatchingConfig = pAtMostConfig;
        return DAL_SUCCESS;
      }
      else if ((pAtLeastConfig != NULL) && (pAtMostConfig != NULL))
      {
        /* Select the closest match, select the lowest in case of a tie */
        if ((nAtLeastFrequency - nFreqHz) < (nFreqHz - nAtMostFrequency))
        {
          *pMatchingConfig = pAtLeastConfig;
          return DAL_SUCCESS;
        }
        else
        {
          *pMatchingConfig = pAtMostConfig;
          return DAL_SUCCESS;
        }
      }
      break;

    default:
      break;
  }

  /* No match was found */
  *pMatchingConfig = NULL;

  return DAL_ERROR;

} /* END Clock_FindSourceConfig */


/* =========================================================================
**  Function : Clock_DetectSourceConfig
** =========================================================================*/
/*
  See ClockDriver.h
*/

DALResult Clock_DetectSourceConfig
(
  ClockDrvCtxt        *pDrvCtxt,
  ClockSourceNodeType *pSource
)
{
  HAL_clk_PLLConfigType            DetectedConfig = {0};
  const ClockSourceFreqConfigType *pFreqConfig;
  uint32                           i;

  if (!pSource->aBSP || pSource->pActiveFreqConfig)
  {
    return DAL_SUCCESS;
  }

  HAL_clk_DetectPLLConfig(&pSource->HALSource, &DetectedConfig);

  for (i = 0; i < pSource->nBSPLen; i++)
  {
    pFreqConfig = &pSource->aBSP[i];
    if (!Clock_IsBSPSupported(&pFreqConfig->HWVersion))
    {
      continue;
    }

    if (HAL_clk_IsPLLConfigEqual(&pSource->HALSource,
                                 &pFreqConfig->HALConfig, &DetectedConfig))
    {
      pSource->pActiveFreqConfig = pFreqConfig;
      pSource->pSource = pFreqConfig->pSource;
      return DAL_SUCCESS;
    }
  }

  return DAL_ERROR;

} /* END Clock_DetectDetectedConfig */

