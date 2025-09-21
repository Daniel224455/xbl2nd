/*=============================================================================
 
  File: Dp_host.c
 
  DP Host layer
  
 
 Copyright (c) 2016-2018 Qualcomm Technologies, Inc.  All Rights Reserved.
 Qualcomm Technologies Proprietary and Confidential.
 =============================================================================*/
#ifdef __cplusplus
extern "C" {
#endif


/*---------------------------------------------------------------------------------------------------------------------
 * Include Files
 *-------------------------------------------------------------------------------------------------------------------*/
#include "dp_i.h"
#include "hal_dp_phy.h"


/*---------------------------------------------------------------------------------------------------------------------
 * Defines
 *-------------------------------------------------------------------------------------------------------------------*/



/*---------------------------------------------------------------------------------------------------------------------
 * Global Data Definitions
 *-------------------------------------------------------------------------------------------------------------------*/
DP_DeviceDataType  gDpSharedData;

/*---------------------------------------------------------------------------------------------------------------------
 * Private Functions
 *---------------------------------------------------------------------------------------------------------------------*/


/*********************************************************************************************
*
** FUNCTION: DP_Host_ReadModeInfoFromSink()
*/
/*!
* \DESCRIPTION
*     Retrieve modes from EDID of DP sink
*
* \param [in]   pDeviceCtx        - Pointer to device context
*
* \retval DP_Status
*
**********************************************************************************************/
static DP_Status DP_Host_ReadModeInfoFromSink(DP_DeviceCtxType    *pDeviceCtx)
{
  DP_Status                 eStatus      = DP_STATUS_SUCCESS;

  eStatus = DP_ReadSinkEDIDRaw(pDeviceCtx);

  if (DP_STATUS_SUCCESS == eStatus)
  {
    pDeviceCtx->sSinkEDIDData = (DP_EDIDDataType *)pDeviceCtx->auEDIDCache;

    DP_LOG_MESSAGE_INFO("DP_ReadModeInfoFromSink: edid: head=%x %x manu=%x ver=%x rev=%x\n",
pDeviceCtx->sSinkEDIDData->uHeader[0], pDeviceCtx->sSinkEDIDData->uHeader[1], pDeviceCtx->sSinkEDIDData->uManufacureName, pDeviceCtx->sSinkEDIDData->uEdidVersion, pDeviceCtx->sSinkEDIDData->uEdidRevision);

  }
  else 
  {
    DP_LOG_MESSAGE_ERROR("DP_Host_ReadModeInfoFromSink: DP_ReadSinkEDIDRaw failed, eStatus = 0x%x", eStatus);
  }

  if (DP_STATUS_SUCCESS != (eStatus = DP_ParseSinkEDID(pDeviceCtx)))
  {
    DP_LOG_MESSAGE_ERROR("DP_Host_ReadModeInfoFromSink: DP_ParseSinkEDID failed, eStatus = 0x%x", eStatus);
  }
  else
  {
     pDeviceCtx->bEDIDObtained = TRUE;
  }
    
  return eStatus;
}

/*********************************************************************************************
*
** FUNCTION: DP_Host_SetMode()
*/
/*!
* \DESCRIPTION
*     Set selected display mode
*
* \param [in]   pDeviceCtx        - Pointer to device context
*
* \retval DP_Status
*
**********************************************************************************************/
static DP_Status  DP_Host_SetMode(DP_DeviceCtxType      *pDeviceCtx)
{
  DP_Status               eStatus      = DP_STATUS_SUCCESS;  
  DP_PanelInfoType       *psPanelInfo  = &(pDeviceCtx->sPanelInfo[pDeviceCtx->uModeIndex]);

  HAL_DP_PllConfigType            sPhyConfig;    
  DP_TUSizePararmType             sTUSizeParam;    
  HAL_DP_TUConfigType             sHALTuConfig;
  HAL_DP_StreamAttribType         sStreamAttrib;
  HAL_DP_MSAPacketType            sHALMSAPacket;
  DP_PixelClk_Info                sPixClkInfo;
  HAL_DP_SinkAttribType           sSinkAttrib;

  DP_OSAL_MemSet((void *)&sPhyConfig,    0, sizeof(HAL_DP_PllConfigType));
  DP_OSAL_MemSet((void *)&sTUSizeParam,  0, sizeof(DP_TUSizePararmType));
  DP_OSAL_MemSet((void *)&sHALTuConfig,  0, sizeof(HAL_DP_TUConfigType));  
  DP_OSAL_MemSet((void *)&sStreamAttrib, 0, sizeof(HAL_DP_StreamAttribType));  
  DP_OSAL_MemSet((void *)&sSinkAttrib,   0, sizeof(HAL_DP_SinkAttribType));


  /* Calculate all the link settings */
  eStatus = DP_CalculateLink(pDeviceCtx->eDeviceID, &pDeviceCtx->sSinkDPCDCap, psPanelInfo, &sPixClkInfo);
  if(DP_STATUS_SUCCESS != eStatus)
  {
    DP_LOG_MESSAGE_ERROR("DP_Host_SetMode: Failed to calculate link settings\n");      
    eStatus = DP_STATUS_FAIL;
    goto exit;
  }

  /* Calculate TU size and related settings */
  sTUSizeParam.uBPP                = 3 * DPHOST_GETPIXELFMT(psPanelInfo->eColorFormat).uBitsPerComponent;  
  sTUSizeParam.uLinkClockInKhz     = pDeviceCtx->uLinkRateInKhz;
  sTUSizeParam.uPixelClockInKhz    = pDeviceCtx->uPixelClkInKhz;
  sTUSizeParam.uNumberOfLanes      = pDeviceCtx->uNumLanes;
  sTUSizeParam.uVisWidth           = psPanelInfo->uVisibleWidthInPixels;
  eStatus = DP_CalculateTU(&sTUSizeParam, &sHALTuConfig);    

  /* Set up PHY PLL */
  sPhyConfig.uNumLanes                 = pDeviceCtx->uNumLanes;      
  sPhyConfig.uLinkRateMbps             = (pDeviceCtx->uLinkRateInKhz / 1000) * 10;   // symbol size = 10 bits
  sPhyConfig.uPixelClkMNDInputMhz      = sPixClkInfo.uPixelClkMNDInputMhz;
  sPhyConfig.uPreemphasisLevel         = pDeviceCtx->uPreEmphasisLevel;
  sPhyConfig.uVoltSwingLevel           = pDeviceCtx->uVoltageSwingLevel;
  sPhyConfig.bInvertedPlug             = pDeviceCtx->bInvertedPlug;
  sPhyConfig.bReversePhyDataPath       = DP_PhyDataPathIsReverse(pDeviceCtx->eDeviceID);
  if(TRUE == pDeviceCtx->bEnableSSCMode)
  {
    sPhyConfig.uFlags              |= HAL_DP_PLLCONFIG_FLAG_SSC;
  }
  if(FALSE == pDeviceCtx->bSynchronousClk)
  {
    sPhyConfig.uFlags              |= HAL_DP_PLLCONFIG_FLAG_ASYNC;
  }
  if(FALSE == HAL_DP_PHY_Config(&sPhyConfig))
  {
    DP_LOG_MESSAGE_ERROR("DP_Host_SetMode: Setup of PHY PLL failed\n");      
    eStatus = DP_STATUS_FAIL;
    goto exit;
  }

  /* Set up external clock source and dividers for all the main link clocks */
  eStatus = DP_MainClockEnable(pDeviceCtx->eDeviceID, &sPixClkInfo, TRUE);
  if(DP_STATUS_SUCCESS != eStatus )
  {
    DP_LOG_MESSAGE_ERROR("DP_Host_SetMode: Failed to configure main link clocks \n");      
    eStatus = DP_STATUS_FAIL;
    goto exit;
  }
  
  /* Write link configuration to the sink DPCD */ 
  eStatus = DP_WriteSinkDPCDLink(pDeviceCtx->eDeviceID);
  if(DP_STATUS_SUCCESS != eStatus)
  {
    DP_LOG_MESSAGE_ERROR("DP_Host_SetMode: Failed to write link settings to panel DPCD \n");       
    eStatus = DP_STATUS_FAIL;
    goto exit;
  }

  /* Configure link configuration on host side */   
  
  /* Set Lane mapping based on USB Type-C plug polarity */
  sSinkAttrib.eLaneMapping     = HAL_DP_LANEMAP_TYPE_0123;
  sSinkAttrib.ePixelFormat     = HAL_DP_PIXELFORMAT_TYPE_RGB;
  sSinkAttrib.eRGBMapping      = HAL_DP_RGBMAP_TYPE_RGB;
  sSinkAttrib.uBitPerComponent = DPHOST_GETPIXELFMT(psPanelInfo->eColorFormat).uBitsPerComponent;
  sSinkAttrib.uLaneNumber      = pDeviceCtx->uNumLanes;
  HAL_DP_SetSinkAttrib(&sSinkAttrib);
  
  HAL_DP_SetTUConfig(&sHALTuConfig);   

  sStreamAttrib.bSynchronousClk  = pDeviceCtx->bSynchronousClk;
  sStreamAttrib.bEnhancedFraming = pDeviceCtx->sSinkDPCDCap.eDPCDCap & DP_DPCD_CAP_SUPPORT_ENHANCED_FRAME;
  sStreamAttrib.bASSR            = FALSE;  // ASSR is eDP specific
  sStreamAttrib.bInterlaced      = psPanelInfo->bInterlaced;
  HAL_DP_SetStreamAttrib(&sStreamAttrib);

  /* Set Main stream attributes on host side */
  sHALMSAPacket.uSWMVid                  = sPixClkInfo.uSWMVid;
  sHALMSAPacket.uSWNVid                  = sPixClkInfo.uSWNVid;  
  sHALMSAPacket.uVisibleWidthInPixels    = psPanelInfo->uVisibleWidthInPixels; 
  sHALMSAPacket.uVisibleHeightInPixels   = psPanelInfo->uVisibleHeightInPixels; 
  sHALMSAPacket.uHsyncBackPorchInPixels  = psPanelInfo->uHsyncBackPorchInPixels; 
  sHALMSAPacket.uHsyncFrontPorchInPixels = psPanelInfo->uHsyncFrontPorchInPixels; 
  sHALMSAPacket.uHsyncPulseInPixels      = psPanelInfo->uHsyncPulseInPixels;
  sHALMSAPacket.uHsyncSkewInPixels       = psPanelInfo->uHsyncSkewInPixels;
  sHALMSAPacket.uVsyncBackPorchInLines   = psPanelInfo->uVsyncBackPorchInLines; 
  sHALMSAPacket.uVsyncFrontPorchInLines  = psPanelInfo->uVsyncFrontPorchInLines; 
  sHALMSAPacket.uVsyncPulseInLines       = psPanelInfo->uVsyncPulseInLines;
  sHALMSAPacket.uHLeftBorderInPixels     = psPanelInfo->uHLeftBorderInPixels; 
  sHALMSAPacket.uHRightBorderInPixels    = psPanelInfo->uHRightBorderInPixels;
  sHALMSAPacket.uVTopBorderInLines       = psPanelInfo->uVTopBorderInLines; 
  sHALMSAPacket.uVBottomBorderInLines    = psPanelInfo->uVBottomBorderInLines;
  sHALMSAPacket.bHSyncActiveLow          = psPanelInfo->bHSyncActiveLow;
  sHALMSAPacket.bVSyncActiveLow          = psPanelInfo->bVSyncActiveLow;
  sHALMSAPacket.uMisc0                   = DP_CalculateMisc0( psPanelInfo->eColorFormat );
  if(TRUE == pDeviceCtx->bSynchronousClk)
  {
    sHALMSAPacket.uMisc0                |= DP_SPEC_MSA_MISCO_SYNC_CLK_BMSK;
  }
  sHALMSAPacket.uMisc1                   = 0;  
  HAL_DP_SetMSA(&sHALMSAPacket);

  /* Enable Main link */ 
  HAL_DP_Mainlink_Enable(TRUE); 

  /* SW Reset DP mainlik HW */
  HAL_DP_Reset(HAL_DP_MODULE_TYPE_MAINLINK , 0);

  /* Link Training */
  eStatus = DP_LinkTraining(pDeviceCtx->eDeviceID, pDeviceCtx->eLinkTrainingType, 0);  
  if(DP_STATUS_SUCCESS != eStatus)
  {    
    DP_LOG_MESSAGE_ERROR("DP_Host_SetMode : Link Training failed (link rate: %d Mbps, lanes: %d\n)", 
                                                          sPhyConfig.uLinkRateMbps, 
                                                          sPhyConfig.uNumLanes);    
    eStatus = DP_STATUS_FAILED_LINK_TRAINING;  
  }  
  
  /* Start Video */
  HAL_DP_SetLinkState(HAL_DP_LINKCOMMAND_TYPE_SEND_VIDEO);
    
  eStatus =  DP_WaitForReadyForVideo(pDeviceCtx->eDeviceID);
  if(DP_STATUS_SUCCESS != eStatus)
  {
    DP_LOG_MESSAGE_ERROR("DP_Host_SetMode: Failed to start video\n");
    eStatus = DP_STATUS_FAIL;
    goto exit;
  } 
  
  DP_LOG_MESSAGE_INFO("DP_Host_SetMode: done (link rate: %d Mbps, lanes: %d\n)", sPhyConfig.uLinkRateMbps, sPhyConfig.uNumLanes);

exit:
  return eStatus;  
}

/*---------------------------------------------------------------------------------------------------------------------
 * Public Functions
 *-------------------------------------------------------------------------------------------------------------------*/

/*********************************************************************************************
*
** FUNCTION: DP_Host_QueryPanelInfo()
*/
/*!
* \DESCRIPTION
*     Query EDID/DPCD info from panel
*
* \param [in]   hHandle           - Handle to DP device
*
* \retval DP_Status
*
**********************************************************************************************/
DP_Status DP_Host_QueryPanelInfo(DP_HandleType    hHandle)
{
  DP_Status                eStatus        = DP_STATUS_SUCCESS; 
  DP_DeviceCtxType        *pDeviceCtx     = (DP_DeviceCtxType *)hHandle;

  if (NULL == hHandle)
  {
    DP_LOG_MESSAGE_ERROR("DP_Host_GetInfo: unexpected null handle \n");
    eStatus = DP_STATUS_FAILED_DRIVER_NOT_INITIALIZED;
  }
  else
  {
          /* Set DP Sink to D0(power on) state */
          if (DP_STATUS_SUCCESS != (eStatus = DP_WriteSinkDPCDPower(pDeviceCtx->eDeviceID, DP_DPCD_POWERSTATE_D0)))
          {
            pDeviceCtx->bPluggedIn = FALSE;
            DP_LOG_MESSAGE_ERROR("DP_Host_GetInfo: Failed to Set Sink Device to D0 state\n");
          }
          /* Read DPCD  */
          else if (DP_STATUS_SUCCESS != (eStatus = DP_ReadSinkDPCDCap(pDeviceCtx->eDeviceID, &(pDeviceCtx->sSinkDPCDCap))))
          {
            DP_LOG_MESSAGE_ERROR("DP_Host_GetInfo: Failed to Read DPCD info from Sink Device\n");
          }
          /* Read EDID */
          else if (DP_STATUS_SUCCESS != (eStatus = DP_Host_ReadModeInfoFromSink(pDeviceCtx)))
          {
            DP_LOG_MESSAGE_ERROR("DP_Host_GetInfo: Failed to get any display modes from EDID of Sink Device\n");
          }
  }

  return eStatus;
}


/*********************************************************************************************
*
** FUNCTION: DP_Host_Open()
*/
/*!
* \DESCRIPTION
*        This init function must be called before any other DP function is called
*
* \param [out]  phHandle        - Pointer to handle of DP Device
* \param [in]   psOpenConfig    - Pointer to host open configuration
*
* \retval DP_Status
*
**********************************************************************************************/
DP_Status  DP_Host_Open(DP_HandleType               *phHandle,
                        DP_HostOpenConfigType       *psOpenConfig)
{
  DP_Status                       eStatus      = DP_STATUS_SUCCESS;
  HAL_DP_AuxHwCapsType            sHALHwCap;

  if ((NULL == phHandle) ||
      (NULL == psOpenConfig))
  {
    eStatus = DP_STATUS_FAILED_INVALID_INPUT_PARAMETER;
    DP_LOG_MESSAGE_ERROR("DP_Host_Open() : Got Null Pointer\n");
  }
  else
  {
    DP_DeviceCtxType   *pDeviceCtx   = DPHOST_GET_CTX(psOpenConfig->eDeviceID);

    if(TRUE == pDeviceCtx->bInitialized)
    {
      /* Not an error, just ignore the call and return the handler */
      DP_LOG_MESSAGE_WARN("DP_Host_Open() : Device already Initialized\n");
      *phHandle = (void*)pDeviceCtx;
    }
    else
    {
      /* Initialize PHY function vector table */
      HAL_DP_PhyFxnsInit();
      HAL_DP_PHY_SetPHYMode(TRUE);	/* DP enabled and USB2 disabled */

      /* Set Default values */      
      pDeviceCtx->eLinkTrainingType         = DP_LINK_TRAINING_TYPE_FULL_TRAINING;      
      pDeviceCtx->bSynchronousClk           = TRUE;   // Pixel clock and Main-Link Symbol clock are synchronous with each other 

      /* Update Open Parameters */
      pDeviceCtx->eDeviceID                 = psOpenConfig->eDeviceID;
      pDeviceCtx->bEnableSSCMode            = psOpenConfig->bEnableSSCMode;

      /*
       * USB type-C plug orientation
       * FALSE: portselect 0
       * TRUE : portselect 1
       */
      pDeviceCtx->bInvertedPlug             = psOpenConfig->bInverted; /* plug orientation */

      if (DPCD_SPEC_MAX_VOLTAGE_SWING_LEVEL >= psOpenConfig->uLaneSwingLevel)
      {
        pDeviceCtx->uVoltageSwingLevel = psOpenConfig->uLaneSwingLevel;
      }
      else
      {
        pDeviceCtx->uVoltageSwingLevel = DPCD_SPEC_MIN_VOLTAGE_SWING_LEVEL;
        DP_LOG_MESSAGE_WARN("DP_Host_Open() : Invalid voltage swing level, set to MIN_VOLTAGE_SWING_LEVEL\n");
      }

      if (DPCD_SPEC_MAX_PRE_EMPHASIS_LEVEL >= psOpenConfig->uPreemphasisLevel)
      {
        pDeviceCtx->uPreEmphasisLevel = psOpenConfig->uPreemphasisLevel;
      }
      else
      {
        pDeviceCtx->uPreEmphasisLevel = DPCD_SPEC_MIN_PRE_EMPHASIS_LEVEL;
        DP_LOG_MESSAGE_WARN("DP_Host_Open() : Invalid pre-emphasis level, set to MIN_PRE_EMPHASIS_LEVEL\n");
      }

      /* Get the max supported voltage swing & pre-emphasis levels in hardware */
      if (HAL_MDSS_STATUS_SUCCESS != HAL_DP_PHY_GetMaxPeSwLevel(&pDeviceCtx->uMaxPreemphLvl, &pDeviceCtx->uMaxVoltSwingLvl))
      {
        pDeviceCtx->uMaxPreemphLvl   = DPCD_SPEC_MAX_PRE_EMPHASIS_LEVEL;
        pDeviceCtx->uMaxVoltSwingLvl = DPCD_SPEC_MAX_VOLTAGE_SWING_LEVEL;
      }

      /* Can we assume panel is plugged in and skip HPD */
      pDeviceCtx->bPluggedIn         = psOpenConfig->bSkipHotPlugDetect;


      /* Get the Max CMD FIFO length from HAL */
      HAL_DP_AUX_GetHwCaps(&sHALHwCap);
      pDeviceCtx->uMaxAUXCmdFifoLen       = sHALHwCap.uAUXCmdFifoLen;
      pDeviceCtx->bInitialized            = TRUE;

      /* Enable clocks prior to any controller/phy programming */
      if (DP_STATUS_SUCCESS != (eStatus = DP_AUXClockEnable(TRUE, 0)))
      {
        DP_LOG_MESSAGE_ERROR("DP: Failed to enable AUX clock\n");
      }
      else
      {
        HAL_DP_AUXConfigType     sAUXCtrlConfig;
        HAL_DP_AUXAttribType     sAUXAttrib;
        uint32                   uAuxTimeout      = 0xf;      //HWIO_MMSS_DP_AUX_LIMITS_TIMEOUTS_LIMIT_BMSK;
        uint32                   uAuxTimeoutCount = 0xFFFFF;  //HWIO_MMSS_DP_TIMEOUT_COUNT_TIMEOUT_COUNT_BMSK;
        HAL_DP_HPDConfigType     sHPDConfig;

        /* SW Reset DP HW */
        HAL_DP_Reset(( HAL_DP_MODULE_TYPE_AUX | HAL_DP_MODULE_TYPE_PHY), 0);

        /* HPD init */
        DP_OSAL_MemSet(&sHPDConfig, 0, sizeof(HAL_DP_HPDConfigType));
        sHPDConfig.bEnable    = TRUE;
        sHPDConfig.bActiveLow = FALSE;
        HAL_DP_HPD_Engine_Enable(&sHPDConfig);

        /* Set max AUX timeout config */
        DP_OSAL_MemSet(&sAUXCtrlConfig, 0, sizeof(HAL_DP_AUXConfigType));
        DP_OSAL_MemSet(&sAUXAttrib,     0, sizeof(HAL_DP_AUXAttribType));
        sAUXAttrib.puTimeoutLimit   = &uAuxTimeout;
        sAUXAttrib.puReadZeroLimit  = &uAuxTimeout;
        sAUXAttrib.puNackDeferLimit = &uAuxTimeout;
        sAUXAttrib.puI2CDeferLimit  = &uAuxTimeout;
        sAUXAttrib.puTimeoutCount   = &uAuxTimeoutCount;
        sAUXCtrlConfig.psAUXAttrib  = &sAUXAttrib;
        HAL_DP_AUXConfig(&sAUXCtrlConfig, 0);

        /* Initialize Auxiliary  */
        HAL_DP_PHY_AUX_Init();

        /* Enable AUX */
        HAL_DP_AUXChannel_Enable(TRUE);

        /* Clear any stale interrupts */
        HAL_DP_Interrupt_Clear(HAL_DP_AUX_INTR_ALL, 0);
      }

      *(phHandle)                         = (void *) pDeviceCtx;
    }
  }

  return eStatus;
}


/**********************************************************************************************************************

FUNCTION: DP_Host_Close()
   
   This API is called to de-initialize the corresponding resources that is initialized when DP_Open() is called.
   It will return an error if the device handle is invalid. All resources will be de-initialized . User should ensure 
   that no other DP function will be called before another DP_Open() were called. otherwise corresponding error will 
   be returned to the user. Upon an invalid device handle, this call will also return an error.

Parameters:
    hHandle              - [in] handle to DP device 

Return: 
   DP_Status 

**********************************************************************************************************************/
DP_Status  DP_Host_Close(DP_HandleType    hHandle)
{
  DP_Status                       eStatus        = DP_STATUS_SUCCESS;
  DP_DeviceCtxType               *pDeviceCtx     = (DP_DeviceCtxType *)(hHandle);

  if (FALSE == pDeviceCtx->bInitialized)
  {
    eStatus = DP_STATUS_FAILED_DRIVER_NOT_INITIALIZED;
    DP_LOG_MESSAGE_ERROR("DP_Host_Close() : Device has not been Initialized yet\n");
  }
  else
  {
    /* Push idle pattern */
    HAL_DP_SetLinkState(HAL_DP_LINKCOMMAND_TYPE_PUSH_IDLE);

    DP_MainClockEnable(pDeviceCtx->eDeviceID, NULL, FALSE);

    /* Disable links */
    HAL_DP_AUXChannel_Enable(FALSE);
    HAL_DP_Mainlink_Enable(FALSE);
    HAL_DP_PHY_Power(FALSE);
    HAL_DP_PHY_SetPHYMode(FALSE);


    pDeviceCtx->bPluggedIn    = FALSE;
    pDeviceCtx->bInitialized  = FALSE;
    pDeviceCtx->bEDIDObtained = FALSE;
  }

  return eStatus;
}



/**********************************************************************************************************************

FUNCTION:  DP_Host_Commit

  This API allows users to commit all the settings and apply to H/W. 

Parameters:
  hHandle                           -[in] Logic handle of DP Device.

Return: 
  DP_Status 

**********************************************************************************************************************/
DP_Status  DP_Host_Commit(DP_HandleType    hHandle)
{
  DP_Status                       eStatus        = DP_STATUS_SUCCESS;
  DP_DeviceCtxType               *pDeviceCtx     = (DP_DeviceCtxType *)hHandle;

  /* The settings are usually applied on a SetMode */  
  if (pDeviceCtx->uDirtyBits & DP_DIRTYFIELD_PANELMODE)
  {
    eStatus = DP_Host_SetMode(pDeviceCtx);
  }
  else
  {
    /* Individual actions not tied to a mode set */    
  }

  /* Clear all dirty bits */
  pDeviceCtx->uDirtyBits         = DP_DIRTYFIELD_NONE;

  return eStatus;
}


/*********************************************************************************************
*
** FUNCTION: DP_Host_SetProperty()
*/
/*!
* \DESCRIPTION
*      Allows the user to set the attributes and properties of the DP device
*
* \param [in]   hHandle        - Handle to DP device
* \param [in]   ePropertyType  - Property ID that requires update
* \param [in]   pPropertyData  - parameters of the property to be applied
*
* \retval DP_Status
*
**********************************************************************************************/
DP_Status  DP_Host_SetProperty(DP_HandleType                   hHandle,
                               DP_SetPropertyType              ePropertyType,
                               DP_SetPropertyParamsType       *pPropertyData)
{
  DP_Status                       eStatus        = DP_STATUS_SUCCESS;
  DP_DeviceCtxType               *pDeviceCtx     = (DP_DeviceCtxType *) hHandle;  

  if(NULL == pDeviceCtx) 
  {
    DP_LOG_MESSAGE_ERROR("DP_Host_SetProperty() : Null handle\n");
    eStatus = DP_STATUS_FAILED_INVALID_INPUT_PARAMETER;
  }
  else
  {
    switch (ePropertyType)
    {
      case DP_SETPROPERTY_SET_LINK_RATE:
      {
        pDeviceCtx->uLinkRateInKhz                = pPropertyData->uLinkRate;
        pDeviceCtx->uDirtyBits                   |= DP_DIRTYFIELD_SINK_ATTRIB  |
                                                    DP_DIRTYFIELD_TU_PARAMETERS;
        break;
      }
      case DP_SETPROPERTY_SET_LANE_NUMBER:
      {
        pDeviceCtx->uNumLanes                     = pPropertyData->uLaneNumber;
        pDeviceCtx->uDirtyBits                   |= DP_DIRTYFIELD_SINK_ATTRIB |
                                                    DP_DIRTYFIELD_TU_PARAMETERS;
        break;
      }
      case DP_SETPROPERTY_SET_PANEL_INFO:
      {
        DP_OSAL_MemCpy((void*)&pDeviceCtx->sPanelInfo[pDeviceCtx->uModeIndex], 
                       (void*)&(pPropertyData->sPanelInfo), 
                       sizeof(DP_PanelInfoType));

        pDeviceCtx->uDirtyBits                    |= DP_DIRTYFIELD_SINK_ATTRIB    |
                                                     DP_DIRTYFIELD_TU_PARAMETERS  |
                                                     DP_DIRTYFIELD_MAINLINK_PACKET;
        break;
      }
      case DP_SETPROPERTY_SET_PANEL_MODE_INDEX:
      {        
        if (pPropertyData->uModeIndex <= pDeviceCtx->uNumModes)
        {
          pDeviceCtx->uModeIndex                     = pPropertyData->uModeIndex;
          pDeviceCtx->uDirtyBits                    |= DP_DIRTYFIELD_SINK_ATTRIB    |
                                                       DP_DIRTYFIELD_TU_PARAMETERS  |
                                                       DP_DIRTYFIELD_MAINLINK_PACKET|
                                                       DP_DIRTYFIELD_PANELMODE;
        }
        else
        {
          eStatus = DP_STATUS_FAILED_INVALID_INPUT_PARAMETER;
        }
        break;
      }
      case DP_SETPROPERTY_SET_LINK_TRAINING_TYPE:
      {
        pDeviceCtx->eLinkTrainingType             = pPropertyData->eLinkTrainingType;
        break;
      }
      case DP_SETPROPERTY_CONNECTIONSTATUS:
      {
        pDeviceCtx->bPluggedIn = pPropertyData->bConnected;
        break;
      }
      default:
      {
        eStatus = DP_STATUS_FAILED_NOT_SUPPORTED;
        DP_LOG_MESSAGE_ERROR("DP_Host_SetProperty() : Invalid property\n");
        break;
      }
    }
  }

  return eStatus;
}


/*********************************************************************************************
*
** FUNCTION: DP_Host_GetProperty()
*/
/*!
* \DESCRIPTION
*      Allows the user to query the attributes and properties of the DP device
*
* \param [in]   hHandle        - Logical handle of DP Device
* \param [in]   ePropertyType  - Property ID query
* \param [in]   pPropertyData  - parameters of the property that will be returned
*
* \retval DP_Status
*
**********************************************************************************************/
DP_Status DP_Host_GetProperty(DP_HandleType                   hHandle,
                              DP_GetPropertyType              ePropertyType,
                              DP_GetPropertyParamsType       *pPropertyData)
{
  DP_Status                  eStatus        = DP_STATUS_SUCCESS;
  DP_DeviceCtxType          *pDeviceCtx     = (DP_DeviceCtxType *) hHandle;

  if ((NULL == pPropertyData)|| 
      (NULL == hHandle))
  {
    DP_LOG_MESSAGE_ERROR("DP_Host_GetProperty() : Null input\n");
    eStatus = DP_STATUS_FAILED_INVALID_INPUT_PARAMETER;
  }
  else
  {
    switch (ePropertyType)
    {
      case DP_GETPROPERTY_GET_MODE_INFO:
      {
        eStatus = DP_STATUS_FAILED_INVALID_INPUT_PARAMETER;
        /* Check if we need to read the EDID from DP Sink */
        if (FALSE == pDeviceCtx->bEDIDObtained)
        {
          if (pDeviceCtx->bPluggedIn)
          {
            /* If Mode info is read from sink successfully, bEDIDObtained will be set to TRUE */
            eStatus = DP_Host_ReadModeInfoFromSink(pDeviceCtx);
          }
        }

        /* If EDID has been read from sink successfully */
        if (TRUE == pDeviceCtx->bEDIDObtained)
        {
          if (pPropertyData->sPanelInfo.uModeIndex < pDeviceCtx->uNumModes)
          {
            /* Return selected mode info */
            DP_OSAL_MemCpy(&pPropertyData->sPanelInfo, 
                            &pDeviceCtx->sPanelInfo[pPropertyData->sPanelInfo.uModeIndex], 
                            sizeof(DP_PanelInfoType));

            eStatus = DP_STATUS_SUCCESS;
          }
          
        }

        break;
      }
      case DP_GETPROPERTY_LANE_NUMBER:
      {        
        pPropertyData->uLaneNumber = pDeviceCtx->uNumLanes;
        DP_LOG_MESSAGE_INFO("DP_Host_GetProperty: LANE_NUMBER, lane=%d\n", pDeviceCtx->uNumLanes);
        break;
      }
      case DP_GETPROPERTY_LINK_RATE:
      {
        pPropertyData->uLinkRateInKhz = pDeviceCtx->uLinkRateInKhz;
        DP_LOG_MESSAGE_INFO("DP_Host_GetProperty: LINK_RATE, lane=%d\n", pDeviceCtx->uLinkRateInKhz);
        break;
      }      
      case DP_GETPROPERTY_CONNECTION_STATUS:
      {
        DP_LOG_MESSAGE_INFO("DP_Host_GetProperity : CONNECTION_STATUS: bPlugged=%d\n", pDeviceCtx->bPluggedIn);
        pPropertyData->bPluggedIn = pDeviceCtx->bPluggedIn;
        break;
      }
      case DP_GETPROPERTY_EDID_INFO:
      {
        if (pDeviceCtx->bPluggedIn)
        {
          uint32   uCopyLength = sizeof(*(pDeviceCtx->sSinkEDIDData));

          if (pPropertyData->sEdidInfo.uDataLength)
          {
            if (pPropertyData->sEdidInfo.uDataLength < uCopyLength)
            {
              uCopyLength = pPropertyData->sEdidInfo.uDataLength;
            }
          }

          if (pPropertyData->sEdidInfo.pDataBuf)
          {
            DP_OSAL_MemCpy(pPropertyData->sEdidInfo.pDataBuf, pDeviceCtx->sSinkEDIDData, uCopyLength);
            eStatus = DP_STATUS_SUCCESS;
          }
          pPropertyData->sEdidInfo.uDataLength = uCopyLength;
        }
        else
        {
          eStatus = DP_STATUS_FAILED_RESOURCE_NOT_READY;
          DP_LOG_MESSAGE_WARN("DP_Host_GetProperty() : Not supported\n");
        }
        break;
      }
      default:
      {
        DP_LOG_MESSAGE_ERROR("DP_Host_GetProperty() : Not supported\n");
        eStatus = DP_STATUS_FAILED_NOT_SUPPORTED;
        break;
      }      
    }
  }
  
  return eStatus;
}



#ifdef __cplusplus
}
#endif

