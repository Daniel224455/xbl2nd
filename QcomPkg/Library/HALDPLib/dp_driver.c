/*=============================================================================

  File: dpdriver.c

  DP Panel driver

  Copyright (c) 2016-2018 Qualcomm Technologies, Inc.  All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential.
=============================================================================*/


/* -----------------------------------------------------------------------
** Includes
** ----------------------------------------------------------------------- */
#include "MDPLib_i.h"
#include "dp_i.h"
#include "dp_driver.h"
#include "DPInterface.h"

/* -----------------------------------------------------------------------
** Global variables
** ----------------------------------------------------------------------- */
DPDriverConfigType gDPDriverConfig[DP_DEVICE_NUM];

/* -----------------------------------------------------------------------
** Macro defines
** ----------------------------------------------------------------------- */
#define DPDRIVER_GETCTX(x)          ((DPDriverConfigType*)&gDPDriverConfig[x])

/* -----------------------------------------------------------------------
** Local Prototypes
** ----------------------------------------------------------------------- */

/* -----------------------------------------------------------------------
** Local functions
** ----------------------------------------------------------------------- */

/* ---------------------------------------------------------------------- */
/**
** FUNCTION: DPDriver_Open()
**
** DESCRIPTION:
**   Initialize DP panel to use display.
**
*//* -------------------------------------------------------------------- */
MDP_Status DPDriver_Open(MDP_Display_IDType eDisplayId, uint32 uFlags)
{
  MDP_Status               eStatus          = MDP_STATUS_OK;
  DP_HostOpenConfigType    sOpenConfig      = { 0 };
  DPDriverConfigType      *pDPConfig        = DPDRIVER_GETCTX(0);

  // Save DP displayID 
  pDPConfig->eDisplayId           = eDisplayId;

  sOpenConfig.eDeviceID           = DP_DEVICE_ID0;
  sOpenConfig.bReadDPCD           = TRUE;
  sOpenConfig.bReadEDID           = TRUE;
  sOpenConfig.uLaneSwingLevel     = DPCD_SPEC_MIN_VOLTAGE_SWING_LEVEL;
  sOpenConfig.uPreemphasisLevel   = DPCD_SPEC_MIN_PRE_EMPHASIS_LEVEL;
  sOpenConfig.bEnableSSCMode      = FALSE;

  if (uFlags & DPINTERFACE_PLUG_INVERTED) /* USB type-C plug orientation */
  {
       sOpenConfig.bInverted      = TRUE;
  }
  else
  {
       sOpenConfig.bInverted      = FALSE;
  }

  sOpenConfig.bSkipHotPlugDetect  = TRUE;            // Skip GPIO based HPD

  // Initialize the host side 
  if (DP_STATUS_SUCCESS != DP_Host_Open(&pDPConfig->hDPHandle, &sOpenConfig))
  {
    eStatus = MDP_STATUS_FAILED;
    DP_LOG_MESSAGE_ERROR("DPDriver_Open: DP_Host_Open failed, eStatus =%d\n", eStatus);
  }
  else if (DP_STATUS_SUCCESS != DP_Host_QueryPanelInfo(pDPConfig->hDPHandle))
  {
    DP_Host_Close(pDPConfig->hDPHandle);
    eStatus = MDP_STATUS_FAILED;
    DP_LOG_MESSAGE_ERROR("DPDriver_Open: DP_Host_QueryPanelInfo failed, eStatus =%d\n", eStatus);
  }

  return eStatus;
}

/* ---------------------------------------------------------------------- */
/**
** FUNCTION: DPDriver_IsDisplayPlugged()
**
** DESCRIPTION:
**   check DP panel is plugged in.
**
*//* -------------------------------------------------------------------- */
bool32 DPDriver_IsDisplayPlugged(MDP_Display_IDType eDisplayId)
{
  bool32                     bPluggedIn  = FALSE;
  DP_GetPropertyParamsType   sPropData;
  DPDriverConfigType         *pDPConfig  = DPDRIVER_GETCTX(0);

  if (DP_STATUS_SUCCESS == DP_Host_GetProperty(pDPConfig->hDPHandle, DP_GETPROPERTY_CONNECTION_STATUS, &sPropData))
  {   
    bPluggedIn = sPropData.bPluggedIn;
  }   
      
  return bPluggedIn;
}

 /* ----------------------------------------------------------------------
 ** FUNCTION: DPDriver_Term()
 **
 ** DESCRIPTION:
 **    Close the DP Host driver
 **
 -------------------------------------------------------------------- */
MDP_Status DPDriver_Term(MDP_Display_IDType eDisplayId)
{
  MDP_Status               eStatus   = MDP_STATUS_OK;
  DPDriverConfigType       *pDPConfig = DPDRIVER_GETCTX(0);

  
  // Close DP host driver
  if (DP_STATUS_SUCCESS != DP_Host_Close(pDPConfig->hDPHandle))
  {
    eStatus = MDP_STATUS_FAILED;
    DP_LOG_MESSAGE_ERROR("DPDriver_Term: DP_Host_Close failed, eStatus =%d\n", eStatus);
  }

  DP_OSAL_MemSet(pDPConfig, 0x00, sizeof(DPDriverConfigType));

  return eStatus;
}

/* ----------------------------------------------------------------------
** FUNCTION: DPDriver_SetMode()
**
** DESCRIPTION:
**    Set a panel mode
**
-------------------------------------------------------------------- */
MDP_Status DPDriver_SetMode(MDP_Display_IDType eDisplayId, uint32 uModeIndex)
{
  DP_Status                eStatus       = DP_STATUS_SUCCESS;
  DPDriverConfigType      *pDPConfig     = DPDRIVER_GETCTX(0);
  DP_SetPropertyParamsType sPropertyData = { 0 };


  if (NULL == pDPConfig->hDPHandle)
  {
    DP_LOG_MESSAGE_ERROR("DPDriver_SetMode: Invalid input \n");
    eStatus = DP_STATUS_FAILED_RESOURCE_NOT_READY;
  }
  else
  {
    /* Change this value to change link rate  */
    //sPropertyData.uLinkRate = DP_LINK_SYMBOL_RATE_KHZ_PER_LANE_162MBps;
    //if(DP_STATUS_SUCCESS != (eStatus = DP_Host_SetProperty(pDPConfig->hDPHandle, DP_SETPROPERTY_SET_LINK_RATE, &sPropertyData)))
    //{
    //  DP_LOG_MESSAGE_ERROR("DPDriver_SetMode: failed to set link rate\n");
    //}

    sPropertyData.uLaneNumber = 4;
    if (DP_STATUS_SUCCESS != (eStatus = DP_Host_SetProperty(pDPConfig->hDPHandle, DP_SETPROPERTY_SET_LANE_NUMBER, &sPropertyData)))
    {
      DP_LOG_MESSAGE_ERROR("DPDriver_SetMode: failed to set link rate\n");
    }

    /*
     * TODO: 
     * 1) Setup the default mode 0
     * 2) chage sPropertyData.uModeIndex to select different mode
     *    support mode from 0 to 15 derived from edid parser
     *    also need to change corresponding mode at gModeInfo.uSelectedModeIndex[eDisplayId]
     */
    sPropertyData.uModeIndex = 0;
    eStatus = DP_Host_SetProperty(pDPConfig->hDPHandle, DP_SETPROPERTY_SET_PANEL_MODE_INDEX, &sPropertyData);
    if (DP_STATUS_SUCCESS == eStatus)
    {
      eStatus = DP_Host_Commit(pDPConfig->hDPHandle);
    }

    if (DP_STATUS_FAILED_LINK_TRAINING == eStatus)
    {
      /* Link training failed
      *  TODO: Check if we can Reduce bit rate (support could be added in the future) and succeed
      */
    }
  }

  if (eStatus == DP_STATUS_SUCCESS)
  {
     return MDP_STATUS_OK;
  }
  else
  {
     return MDP_STATUS_FAILED;
  }
}

/* ----------------------------------------------------------------------
** FUNCTION: DPDriver_GetModeInfo()
**
** DESCRIPTION:
**    Get DP panel mode info.
**
-------------------------------------------------------------------- */
MDP_Status DPDriver_GetModeInfo(MDP_Display_IDType eDisplayId, uint32 uModeIndex, MDP_Panel_AttrType *psDisplayAttr)
{
  MDP_Status                      eStatus   = MDP_STATUS_OK;
  DPDriverConfigType             *pDPConfig = DPDRIVER_GETCTX(0);

  if (NULL == pDPConfig->hDPHandle)
  {
    eStatus = MDP_STATUS_FAILED;
  }
  else
  {
    DP_GetPropertyParamsType       sDPProp;


    DP_OSAL_MemSet(&sDPProp, 0x00, sizeof(DP_GetPropertyParamsType));
    sDPProp.sPanelInfo.uModeIndex = uModeIndex;
    //Query the mode
     if (DP_STATUS_SUCCESS == DP_Host_GetProperty(pDPConfig->hDPHandle, DP_GETPROPERTY_GET_MODE_INFO, &sDPProp))
     {
       psDisplayAttr->uDisplayWidth                          = sDPProp.sPanelInfo.uVisibleWidthInPixels;
       psDisplayAttr->uDisplayHeight                         = sDPProp.sPanelInfo.uVisibleHeightInPixels;
       psDisplayAttr->uRefreshRate                           = sDPProp.sPanelInfo.uRefreshRate;

      if (DP_PIXEL_FORMAT_RGB_101010_30BPP == sDPProp.sPanelInfo.eColorFormat)
      {
       psDisplayAttr->eColorFormat = MDP_PIXEL_FORMAT_RGB_101010_30BPP;
      }
      else if (DP_PIXEL_FORMAT_RGB_666_18BPP == sDPProp.sPanelInfo.eColorFormat)
      {
        psDisplayAttr->eColorFormat = MDP_PIXEL_FORMAT_RGB_666_18BPP;
      }
      else
      {
       psDisplayAttr->eColorFormat = MDP_PIXEL_FORMAT_RGB_888_24BPP;
      }

       psDisplayAttr->sActiveTiming.uDataEnInvertSignal      = FALSE;
       psDisplayAttr->sActiveTiming.uHsyncPulseWidthDclk     = sDPProp.sPanelInfo.uHsyncPulseInPixels;
       psDisplayAttr->sActiveTiming.uHsyncFrontPorchDclk     = sDPProp.sPanelInfo.uHsyncFrontPorchInPixels;
       psDisplayAttr->sActiveTiming.uHsyncBackPorchDclk      = sDPProp.sPanelInfo.uHsyncBackPorchInPixels;
       psDisplayAttr->sActiveTiming.uHsyncSkewDclk           = 0;         
       psDisplayAttr->sActiveTiming.uHsyncInvertSignal       = sDPProp.sPanelInfo.bHSyncActiveLow;
       psDisplayAttr->sActiveTiming.uVsyncPulseWidthLns      = sDPProp.sPanelInfo.uVsyncPulseInLines;
       psDisplayAttr->sActiveTiming.uVsyncFrontPorchLns      = sDPProp.sPanelInfo.uVsyncFrontPorchInLines;
       psDisplayAttr->sActiveTiming.uVsyncBackPorchLns       = sDPProp.sPanelInfo.uVsyncBackPorchInLines;
       psDisplayAttr->sActiveTiming.uVsyncInvertSignal       = sDPProp.sPanelInfo.bVSyncActiveLow;
     }
     else
     {
       eStatus = MDP_STATUS_FAILED;
     }

  }

  return eStatus;
}
