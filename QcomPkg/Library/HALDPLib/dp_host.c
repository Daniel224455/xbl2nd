/*=============================================================================
 
  File: Dp_host.c
 
  DP Host layer
  
 
 Copyright (c) 2016-2020 Qualcomm Technologies, Inc.
 All Rights Reserved.
 Confidential and Proprietary - Qualcomm Technologies, Inc.
 =============================================================================*/
#ifdef __cplusplus
extern "C" {
#endif


/*---------------------------------------------------------------------------------------------------------------------
 * Include Files
 *-------------------------------------------------------------------------------------------------------------------*/
#include "dp_i.h"
#include "hal_dp_phy.h"
#include "edp_psr.h"
#include "hal_edp_psr.h"
#include "dp_spec.h"
#include "dp_dsc.h"

/*---------------------------------------------------------------------------------------------------------------------
 * Defines
 *-------------------------------------------------------------------------------------------------------------------*/

#define DPHOST_GET_SHARED_CTX()           (&gDpSharedData)  // Get global context fo DP Host

#define FEC_OVERHEAD_RATIO                       0.976

/* AUX timeout limits to be set in hardware */
#define DP_AUX_CLKS_FOR_ONE_MSEC                 0x4B00             // 19.2Mhz clock assumed
#define DP_AUX_HW_TIMEOUT_MS                     4

#define DP_MAX_CONTROLLER_NUM                    2
#define DPHOST_GET_CONTROLLER_INFO(_id_)         (((_id_) < DP_MAX_CONTROLLER_NUM) ? (&sDPControllerPool[(_id_)]) : (&sDPControllerPool[0]))

/*---------------------------------------------------------------------------------------------------------------------
 * Global Data Definitions
 *-------------------------------------------------------------------------------------------------------------------*/
DP_DeviceDataType  gDpSharedData;

/* Controller pool table for dynamic controller allocation. */
DP_ControllerInfoType sDPControllerPool[DP_MAX_CONTROLLER_NUM] =
{
  // eControllerId,       bInUse
  {DP_CONTROLLER_ID_DP_0, FALSE}, // DP_CONTROLLER_ID_DP_0
  {DP_CONTROLLER_ID_DP_1, FALSE}, // DP_CONTROLLER_ID_DP_1
};

/*---------------------------------------------------------------------------------------------------------------------
 * Private Functions
 *---------------------------------------------------------------------------------------------------------------------*/

static DP_Status  DP_Host_PSR(DP_DeviceCtxType  *pDeviceCtx, bool32 bEnterPSR);

static DP_ControllerIDType AllocateDPController(DP_DeviceIDType   eDeviceID,
                                                DP_DeviceCtxType *pDeviceCtx);
                                                
static void FreeDPController(DP_DeviceCtxType *pDeviceCtx);

static DP_Status ConfigureExternalSourceClock(DP_DeviceCtxType     *pDeviceCtx,
                                              HAL_DP_PllConfigType *pPhyConfig,
                                              bool32                bEnable);

static int32 UpdateSourceClockRefCount(DP_DeviceCtxType *pDeviceCtx,
                                       int32             iValue);

static bool32 IsAltModeDPDevice(DP_DeviceIDType eDeviceID);

static DP_Status ConfigureBondedModeInfo(DP_DeviceCtxType      *pDeviceCtx,
                                         DP_HostOpenConfigType *psOpenConfig);


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

  if (DP_STATUS_SUCCESS == (eStatus = DP_ReadSinkEDIDRaw(pDeviceCtx)))
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
** FUNCTION: LinkRateReduce()
*/
/*!
* \DESCRIPTION
*     Reduce link rate if link training for higher link rate failed
*
* \param [in]   pDeviceCtx        - Pointer to context
*
* \retval DP_Status
*
**********************************************************************************************/
static DP_Status LinkRateReduce(DP_DeviceCtxType         *pDeviceCtx)
{
  DP_Status                 eStatus     = DP_STATUS_SUCCESS;

  if (NULL == pDeviceCtx)
  {
    DP_LOG_MESSAGE_ERROR("LinkRateReduce: unexpected null handle ");
    eStatus = DP_STATUS_FAILED_DRIVER_NOT_INITIALIZED;
  }
  else
  {
    uint32                    uLinkRateInKhz = 0;
   
    switch (pDeviceCtx->uLinkRateInKhz)
    {
      case DP_LINK_SYMBOL_RATE_KHZ_PER_LANE_810MBps:
      {
        uLinkRateInKhz = DP_LINK_SYMBOL_RATE_KHZ_PER_LANE_540MBps;
        break;
      }
      case DP_LINK_SYMBOL_RATE_KHZ_PER_LANE_540MBps:
      {
        uLinkRateInKhz = DP_LINK_SYMBOL_RATE_KHZ_PER_LANE_270MBps;
        break;
      }
      case DP_LINK_SYMBOL_RATE_KHZ_PER_LANE_270MBps:
      {
        uLinkRateInKhz = DP_LINK_SYMBOL_RATE_KHZ_PER_LANE_162MBps;
        break;
      }
      default:
      {
        eStatus = DP_STATUS_FAILED_NOT_SUPPORTED;
      }
    }

    if (DP_STATUS_SUCCESS == eStatus)
    { 
      DP_PanelInfoType *psPanelInfo    = &(pDeviceCtx->sPanelInfo[pDeviceCtx->uModeIndex]);

      // Check if lower link rate can support the requested mode
      // comparison is in bits per second
      uint64 uModeBitRate = (uint64)psPanelInfo->uPclkFreq * pDeviceCtx->uBitDepth * 3;      //  1 pix = 3 * bpc
      uint64 uLinkBitRate = (uint64)uLinkRateInKhz * 1000 * 8 * pDeviceCtx->uNumLanes;       //  effective link symbol = 8 bits

      if (uModeBitRate > uLinkBitRate)
      {
        // Indicate that EDID needs to be reparsed
        pDeviceCtx->bEDIDObtained = FALSE;
        eStatus = DP_STATUS_FAILED_NOT_SUPPORTED;
      }

      pDeviceCtx->uLinkRateInKhz  = uLinkRateInKhz;
      pDeviceCtx->uDirtyBits     |= DP_DIRTYFIELD_LINK_RATE;
    }
  }

  return eStatus;
}


/*********************************************************************************************
*
** FUNCTION: DP_Host_DSC_Timing()
*/
/*!
* \DESCRIPTION
*     Calculate new DSC timing
*
* \param [in]   pDeviceCtx        - Pointer to device context
*
* \retval DP_Status
*
**********************************************************************************************/
static DP_Status  DP_Host_DSC_Timing(DP_DeviceCtxType    *pDeviceCtx)
{

    DP_PanelInfoType   *psPanelInfo  = &(pDeviceCtx->sPanelInfo[pDeviceCtx->uModeIndex]);
    DP_SinkDSCCapType  *pDSCCap      = &pDeviceCtx->sDSC;
    uint32              uOrigHBP     = psPanelInfo->uHsyncFrontPorchInPixels + 
                                       psPanelInfo->uHsyncBackPorchInPixels +
                                       psPanelInfo->uHsyncPulseInPixels;
    uint32 uTotalEOCNumber           = 0;
    uint32 uTotalHorizontalBytes     = 0;
    uint32 uTotalDummyBytes          = 0;
    uint32 uEOCBytes                 = 0;
    uint32 uWidth_dsc                = 0;
    uint32 uWidthBytesSlice_dsc      = 0;
    uint32 uBPP                      = 3 * DPHOST_GETPIXELFMT(psPanelInfo->eColorFormat).uBitsPerComponent;  
    float  fPclk_dsc                 = 0;
    float  fHBP_dsc                  = 0;


    /* Width bytes per slice after compression */
    if (pDSCCap->uCompressionRatio != 0)
    {
       uWidthBytesSlice_dsc    = ((pDSCCap->uSliceWidth * uBPP) / 8) / pDSCCap->uCompressionRatio;
    }

    if (uWidthBytesSlice_dsc != 0)
    {
       uEOCBytes             = pDSCCap->uSliceWidth % pDeviceCtx->uNumLanes;
       uTotalEOCNumber       = pDeviceCtx->uNumLanes * pDSCCap->uSlicePerLine;
       uTotalHorizontalBytes = uWidthBytesSlice_dsc * pDSCCap->uSlicePerLine;
       uTotalDummyBytes      = (pDeviceCtx->uNumLanes - uEOCBytes) * pDSCCap->uSlicePerLine;

       uWidth_dsc           = (uTotalHorizontalBytes + uTotalEOCNumber + ((0 == uEOCBytes) ? 0 : uTotalDummyBytes)) / 3;

       /* Pclk_dsc = original pclk * dsc width / old width */
       if (psPanelInfo->uVisibleWidthInPixels != 0)
       {
          fPclk_dsc  = (float)(pDeviceCtx->uPixelClkInKhz * uWidth_dsc) / psPanelInfo->uVisibleWidthInPixels;
       }

       /* this is hsync back porch relative the pclk_dsc */
       if (pDeviceCtx->uPixelClkInKhz != 0)
       {
          fHBP_dsc             = (float)(uOrigHBP * fPclk_dsc) / pDeviceCtx->uPixelClkInKhz;
       }

       /* Output */
       pDSCCap->uPclk_dsc   = fPclk_dsc;
       pDSCCap->uWidth_dsc  = uWidth_dsc;
       pDSCCap->uHBP_dsc    = fHBP_dsc;
    }
    else
    {
       /* Use original value */
       pDSCCap->uPclk_dsc   = pDeviceCtx->uPixelClkInKhz;
       pDSCCap->uWidth_dsc  = psPanelInfo->uVisibleWidthInPixels;
       pDSCCap->uHBP_dsc    = 0;
    }

    return DP_STATUS_SUCCESS;
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
static DP_Status  DP_Host_SetMode(DP_DeviceCtxType    *pDeviceCtx)
{
  DP_Status               eStatus      = DP_STATUS_SUCCESS;

  if (NULL == pDeviceCtx)
  {
    DP_LOG_MESSAGE_ERROR("DP_Host_GetInfo: unexpected null handle ");
    eStatus = DP_STATUS_FAILED_DRIVER_NOT_INITIALIZED;
  }
  else
  {
    DP_PanelInfoType       *psPanelInfo  = &(pDeviceCtx->sPanelInfo[pDeviceCtx->uModeIndex]);
    HAL_DP_PllConfigType    sPhyConfig;
    DP_TUSizePararmType     sTUSizeParam;
    HAL_DP_TUConfigType     sHALTuConfig;
    HAL_DP_StreamAttribType sStreamAttrib;
    HAL_DP_MSAPacketType    sHALMSAPacket;
    DP_PixelClk_Info        sPixClkInfo;
    HAL_DP_SinkAttribType   sSinkAttrib;

    for (;;)
    {
       DP_OSAL_MemSet((void *)&sPhyConfig,    0, sizeof(HAL_DP_PllConfigType));
       DP_OSAL_MemSet((void *)&sTUSizeParam,  0, sizeof(DP_TUSizePararmType));
       DP_OSAL_MemSet((void *)&sHALTuConfig,  0, sizeof(HAL_DP_TUConfigType));  
       DP_OSAL_MemSet((void *)&sStreamAttrib, 0, sizeof(HAL_DP_StreamAttribType));  
       DP_OSAL_MemSet((void *)&sSinkAttrib,   0, sizeof(HAL_DP_SinkAttribType));

       DP_LOG_MESSAGE_INFO("DP_Host_SetMode: ID=%d ndx=%d pclk=%d w=%d hfp=%d bfp=%d h=%d vfp=%d vbp=%d frate=%x\n",
                               pDeviceCtx->eDeviceID,
                               psPanelInfo->uModeIndex,
                               psPanelInfo->uPclkFreq,
                               psPanelInfo->uVisibleWidthInPixels,
                               psPanelInfo->uHsyncFrontPorchInPixels,
                               psPanelInfo->uHsyncBackPorchInPixels,
                               psPanelInfo->uVisibleHeightInPixels,
                               psPanelInfo->uVsyncFrontPorchInLines,
                               psPanelInfo->uVsyncBackPorchInLines,
                               psPanelInfo->uRefreshRate);

       /*
        * Default to max supported lane link rate and lane number from DPCD if they are not
        * set by DP_Host_SetProperty for mode changing. The values could have been changed
        * by previous SetMode when link training failure and reduce link rate
        */
       if (0 == (pDeviceCtx->uDirtyBits & DP_DIRTYFIELD_LINK_RATE))
       {
         pDeviceCtx->uLinkRateInKhz = pDeviceCtx->sSinkDPCDCap.uMaxLinkRateInKhz;
       }
    
       if (0 == (pDeviceCtx->uDirtyBits & DP_DIRTYFIELD_LANE_NUMBER))
       {
         pDeviceCtx->uNumLanes = pDeviceCtx->sSinkDPCDCap.uMaxLaneCount;
       }

       if (TRUE == pDeviceCtx->bDSCSupported)
       {
           /* enable sink fec before link training */
           DP_Host_Sink_FEC_Enable(pDeviceCtx);  
       }

       /* Calculate all the link settings */
       eStatus = DP_CalculateLink(pDeviceCtx, &pDeviceCtx->sSinkDPCDCap, psPanelInfo, &sPixClkInfo);
       if(DP_STATUS_SUCCESS != eStatus)
       {
         DP_LOG_MESSAGE_ERROR("DP_Host_SetMode: Failed to calculate link settings\n");      
         eStatus = DP_STATUS_FAIL;
         goto exit;
       }

       /* Calculate TU size and related settings */
       if (pDeviceCtx->bDSCSupported == TRUE)
       {
           DP_Host_DSC_Timing(pDeviceCtx);

           sTUSizeParam.uLinkClockInKhz     = pDeviceCtx->uLinkRateInKhz * FEC_OVERHEAD_RATIO; /* deducted FEC overhead */
           sTUSizeParam.uPixelClockInKhz    = pDeviceCtx->sDSC.uPclk_dsc;
           sTUSizeParam.uVisWidth           = pDeviceCtx->sDSC.uWidth_dsc;
       }
       else
       {
           sTUSizeParam.uLinkClockInKhz     = pDeviceCtx->uLinkRateInKhz;
           sTUSizeParam.uPixelClockInKhz    = pDeviceCtx->uPixelClkInKhz;
           sTUSizeParam.uVisWidth           = psPanelInfo->uVisibleWidthInPixels;
       }

       sTUSizeParam.uBPP                = 3 * DPHOST_GETPIXELFMT(psPanelInfo->eColorFormat).uBitsPerComponent;  
       sTUSizeParam.uNumberOfLanes      = pDeviceCtx->uNumLanes;
       sTUSizeParam.bSynchronousClk     = pDeviceCtx->bSynchronousClk;
       eStatus = DP_CalculateTU(&sTUSizeParam, &sHALTuConfig);    

       /* Set up PHY PLL */
       sPhyConfig.uNumLanes                 = pDeviceCtx->uNumLanes;
       sPhyConfig.uLinkRateMbps             = (pDeviceCtx->uLinkRateInKhz / 1000) * 10;   // symbol size = 10 bits
       sPhyConfig.uPixelClkMNDInputMhz      = sPixClkInfo.uPixelClkMNDInputMhz;
       sPhyConfig.uPreemphasisLevel         = pDeviceCtx->uPreEmphasisLevel;
       sPhyConfig.uVoltSwingLevel           = pDeviceCtx->uVoltageSwingLevel;
       sPhyConfig.bInvertedPlug             = pDeviceCtx->bInvertedPlug;
       sPhyConfig.bReversePhyDataPath       = DP_PhyDataPathIsReverse(pDeviceCtx);
    
       if(TRUE == pDeviceCtx->bEnableSSCMode)
       {
         sPhyConfig.uFlags              |= HAL_DP_PLLCONFIG_FLAG_SSC;
       }

       if(FALSE == HAL_DP_PHY_Config(pDeviceCtx->eDeviceID, &sPhyConfig))
       {
         DP_LOG_MESSAGE_ERROR("DP_Host_SetMode: Setup of PHY PLL failed");      
         eStatus = DP_STATUS_FAIL;
         goto exit;
       }

       // Setup required for external PLL in bonded mode, if the clock has not been configured yet
       if (0 == UpdateSourceClockRefCount(pDeviceCtx, 0))
       {
         // Disable SSC on externally sourced pixel clock
         sPhyConfig.uFlags &= ~HAL_DP_PLLCONFIG_FLAG_SSC;

         sPhyConfig.uFlags |= HAL_DP_PLLCONFIG_FLAG_PLL;

         if (DP_STATUS_SUCCESS != (eStatus = ConfigureExternalSourceClock(pDeviceCtx, &sPhyConfig, TRUE)))
         {
           DP_LOG_MESSAGE_ERROR("DP_Host_SetMode: Setup of external PLL failed");
           goto exit;
         }
       } 

       /* Set up external clock source and dividers for all the main link clocks */
       eStatus = DP_MainClockEnable(pDeviceCtx, &sPixClkInfo, TRUE);
       if(DP_STATUS_SUCCESS != eStatus )
       {
         DP_LOG_MESSAGE_ERROR("DP_Host_SetMode: Failed to configure main link clocks ");      
         eStatus = DP_STATUS_FAIL;
         goto exit;
       }

       // Reset PHY re-timing buffer
       HAL_DP_PHY_SetPowerState(pDeviceCtx->eDeviceID, FALSE);
       HAL_DP_PHY_SetPowerState(pDeviceCtx->eDeviceID, TRUE);

       /* Write link configuration to the sink DPCD */ 
       eStatus = DP_WriteSinkDPCDLink(pDeviceCtx);
       if(DP_STATUS_SUCCESS != eStatus)
       {
         DP_LOG_MESSAGE_ERROR("DP_Host_SetMode: Failed to write link settings to panel DPCD ");       
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
       HAL_DP_SetSinkAttrib(pDeviceCtx->eControllerID, &sSinkAttrib);
       HAL_DP_SetTUConfig(pDeviceCtx->eControllerID, &sHALTuConfig);   

       sStreamAttrib.bSynchronousClk  = pDeviceCtx->bSynchronousClk;
       if (pDeviceCtx->sSinkDPCDCap.eDPCDCap & DP_DPCD_CAP_SUPPORT_ENHANCED_FRAME)
       {
          sStreamAttrib.bEnhancedFraming = TRUE;
       }

       if (DP_DEVICE_ID_EDP == pDeviceCtx->eDeviceID)
       {
          // ASSR is eDP specific
          if (pDeviceCtx->sSinkDPCDCap.eDPCDCap & DP_DPCD_CAP_SUPPORT_ASSR)
          {
             sStreamAttrib.bASSR    = TRUE;
             DP_WriteSinkASSR(pDeviceCtx, 1); /* configure sink with ASSR */
          }

          HAL_eDP_PSR_Config(pDeviceCtx->eControllerID);
          HAL_eDP_PSR_Send_VSC_Flush(pDeviceCtx->eControllerID);
       }
       sStreamAttrib.bInterlaced      = psPanelInfo->bInterlaced;
       HAL_DP_SetStreamAttrib(pDeviceCtx->eControllerID, &sStreamAttrib);

       /* Set Main stream attributes on host side */
       sHALMSAPacket.uSWMVid                  = sPixClkInfo.uSWMvid;
       sHALMSAPacket.uSWNVid                  = sPixClkInfo.uSWNvid;  
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
       HAL_DP_SetMSA(pDeviceCtx->eControllerID, &sHALMSAPacket);

       /* Enable Main link */ 
       HAL_DP_Mainlink_Enable(pDeviceCtx->eControllerID, TRUE); 

       /* SW Reset DP mainlik HW */
       HAL_DP_Reset(pDeviceCtx->eControllerID, HAL_DP_MODULE_TYPE_MAINLINK , 0);

       /* Link Training */
       eStatus = DP_LinkTraining(pDeviceCtx, pDeviceCtx->eLinkTrainingType, 0);  
       if(DP_STATUS_SUCCESS != eStatus)
       {    
         /* Log the error and try to send to video data */
         DP_LOG_MESSAGE_WARN("DP_Host_SetMode : Link Training failed (link rate: %d Mbps, lanes: %d)\n",
                                                            sPhyConfig.uLinkRateMbps, 
                                                            sPhyConfig.uNumLanes);    
       }
       else
       {
           /* Link Training succeed, Push idle pattern */
           HAL_DP_SetLinkState(pDeviceCtx->eControllerID, HAL_DP_LINKCOMMAND_TYPE_PUSH_IDLE);
           break;
       }

       /*
        * If link training failed, reduce link rate and try to set up the link again.
        * If we are already at the minimum possible link rate for this mode, then LinkRateReduce will fail
        */
       if (eStatus != DP_STATUS_SUCCESS)
       {
          if (DP_STATUS_SUCCESS != (eStatus = LinkRateReduce(pDeviceCtx)))
          {
              DP_LOG_MESSAGE_ERROR("DP_Host_SetMode : mininum link rate reached. Link training will fail\n");
              goto exit;
          }
          else
          {
              DP_LOG_MESSAGE_ERROR("DP_Host_SetMode : Reduce link rate to %d\n", pDeviceCtx->uLinkRateInKhz);
          }
       }
    }  /* for loop */

    /* Setup MDP Backpressure to enable data flow */
    HAL_DP_SetBackPressure(pDeviceCtx->eControllerID, DP_STREAM_ID_MST0, TRUE);

    /* Start Video */
    HAL_DP_SetLinkState(pDeviceCtx->eControllerID, HAL_DP_LINKCOMMAND_TYPE_SEND_VIDEO);

    eStatus =  DP_WaitForReadyForVideo(pDeviceCtx);
    if(DP_STATUS_SUCCESS != eStatus)
    {
      DP_LOG_MESSAGE_ERROR("DP_Host_SetMode: Failed to start video");
      eStatus = DP_STATUS_FAIL;
      goto exit;
    }
    else
    {
      pDeviceCtx->bLinkConfigured = TRUE;
    }

    /* Increment pixel clock source ref count, for shared resource bookkeeping in bonded mode. */
    UpdateSourceClockRefCount(pDeviceCtx, 1);
    
    DP_LOG_MESSAGE_INFO("DP_Host_SetMode: done (link rate: %d Mbps, lanes: %d\n)", sPhyConfig.uLinkRateMbps, sPhyConfig.uNumLanes);
  }

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
    DP_LOG_MESSAGE_ERROR("DP_Host_QueryPanelInfo: Unexpected null handle\n");
    eStatus = DP_STATUS_FAILED_DRIVER_NOT_INITIALIZED;
  }
  else
  {
      /* Only USB-TypeC AltMode needs to toggle AUX polarity GPIO */
      if (0 != DPHOST_GETFLAG(pDeviceCtx->uFlags, DP_HOST_FLAG_ALT_MODE_FIXED_LANE_CONFIGURATION))
      {
        /* Skip configuring AUX polarity GPIO if fixed lane configuration flag set */
      }
      else if (TRUE == IsAltModeDPDevice(pDeviceCtx->eDeviceID))
      {
        /* Set GPIO for AUX polarity if it is DP over USB TypeC */
        DP_SetAuxPolarityGPIO(pDeviceCtx, pDeviceCtx->bInvertedPlug);
      }

      /* Set DP Sink to D0(power on) state */
      if (DP_STATUS_SUCCESS != (eStatus = DP_WriteSinkDPCDPower(pDeviceCtx, DP_DPCD_POWERSTATE_D0)))
      {
        pDeviceCtx->bPluggedIn = FALSE;
        DP_LOG_MESSAGE_ERROR("DP_Host_QueryPanelInfo: Failed to Set Sink Device to D0 state");
      }
      /* Read DPCD  */
      else if (DP_STATUS_SUCCESS != (eStatus = DP_ReadSinkDPCDCap(pDeviceCtx, &(pDeviceCtx->sSinkDPCDCap))))
      {
        DP_LOG_MESSAGE_ERROR("DP_Host_QueryPanelInfo: Failed to Read DPCD info from Sink Device");
      }
      else if (DP_STATUS_SUCCESS != (eStatus = DP_ReadSinkPSRCap(pDeviceCtx)))
      {
        DP_LOG_MESSAGE_ERROR("DP_Host_QueryPanelInfo: Failed to Read DPCD PSR Cap from Sink Device");
      }
      else if (DP_STATUS_SUCCESS != (eStatus = DP_ReadSinkFECCap(pDeviceCtx)))
      {
        DP_LOG_MESSAGE_ERROR("DP_Host_QueryPanelInfo: Failed to Read DPCD FEC Cap from Sink Device");
      }
      else if (DP_STATUS_SUCCESS != (eStatus = DP_ReadSinkDSCCap(pDeviceCtx)))
      {
        DP_LOG_MESSAGE_ERROR("DP_Host_QueryPanelInfo: Failed to Read DPCD DSC Cap from Sink Device");
      }
      else if (DP_STATUS_SUCCESS != (eStatus = DP_ReadSinkBrightnessCap(pDeviceCtx)))
      {
        DP_LOG_MESSAGE_ERROR("DP_Host_QueryPanelInfo: Failed to Read DPCD Brightness Cap from Sink Device");
      }
      /* Read EDID */
      else if (DP_STATUS_SUCCESS != (eStatus = DP_Host_ReadModeInfoFromSink(pDeviceCtx)))
      {
        DP_LOG_MESSAGE_ERROR("DP_Host_QueryPanelInfo: Failed to get any display modes from EDID of Sink Device");
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
    DP_LOG_MESSAGE_ERROR("DP_Host_Open: Got Null Pointer ");
  }
  else
  {
    DP_DeviceDataType    *pSharedData     = DPHOST_GET_SHARED_CTX();
    DP_DeviceCtxType     *pDeviceCtx      = DPHOST_GET_DEVICE_CTX(psOpenConfig->eDeviceID);
    
    /* Caution! bInitialized is not protected from multi-threaded access */
    if (FALSE == pSharedData->bInitialized)
    {
      DISP_OSAL_MemZero(pSharedData, sizeof(DP_DeviceDataType));

      /* Initialize PHY function vector table */
      HAL_DP_PhyFxnsInit();

      pSharedData->bInitialized = TRUE;
    }
    
    if(TRUE == pDeviceCtx->bInitialized)
    {
      /* Not an error, just ignore the call and return the handler */
      DP_LOG_MESSAGE_WARN("DP_Host_Open() : Device already Initialized\n");
      *phHandle = (void*)pDeviceCtx;
    }
    else
    {
      /* Update Open Parameters */
      pDeviceCtx->eDeviceID                 = psOpenConfig->eDeviceID;
      pDeviceCtx->bEnableSSCMode            = psOpenConfig->bEnableSSCMode;
      pDeviceCtx->uPixelPerClock            = 2;          /* transfer 2 pixel per pclk at disp_intf */
      pDeviceCtx->bDTOEnable                = TRUE;

      /*
       * USB type-C plug orientation
       * FALSE: portselect 0
       * TRUE : portselect 1
       */
      pDeviceCtx->bInvertedPlug             = psOpenConfig->bInverted; /* plug orientation */

      /* Allocate DP controller */
      if (DP_CONTROLLER_ID_NONE == (pDeviceCtx->eControllerID = AllocateDPController(psOpenConfig->eDeviceID, pDeviceCtx)))
      {
        DP_LOG_MESSAGE_ERROR("DP_Host_Open: No controller is available for this device %d\n", pDeviceCtx->eDeviceID);
        eStatus = DP_STATUS_FAILED_RESOURCE_NOT_READY;
      }
      else
      {
        if (0 != (psOpenConfig->eConfigFlags & DP_CONFIG_FLAG_ALT_MODE_COMMON_RESET))
        {
          /* Reset DP & USB combo PHY */
          HAL_DP_PHY_Reset(pDeviceCtx->eDeviceID, DP_PHY_RESET_ALT_MODE_COMMON_RESET);
        }
        
        HAL_DP_PHY_SetPHYMode(pDeviceCtx->eDeviceID, DP_PHY_MODE_DP); /* DP enabled and USB SS disabled */

        /* Set Default values */      
        pDeviceCtx->eLinkTrainingType         = DP_LINK_TRAINING_TYPE_FULL_TRAINING;      
        pDeviceCtx->bSynchronousClk           = TRUE;   // Pixel clock and Main-Link Symbol clock are synchronous with each other 

        if (DPCD_SPEC_MAX_VOLTAGE_SWING_LEVEL >= psOpenConfig->uLaneSwingLevel)
        {
          pDeviceCtx->uVoltageSwingLevel = psOpenConfig->uLaneSwingLevel;
        }
        else
        {
          pDeviceCtx->uVoltageSwingLevel = DPCD_SPEC_MIN_VOLTAGE_SWING_LEVEL;
          DP_LOG_MESSAGE_WARN("DP_Host_Open: Invalid voltage swing level, set to MIN_VOLTAGE_SWING_LEVEL\n");
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
        if (HAL_MDSS_STATUS_SUCCESS != HAL_DP_PHY_GetMaxPeSwLevel(pDeviceCtx->eDeviceID,
                                                                 &pDeviceCtx->uMaxPreemphLvl,
                                                                 &pDeviceCtx->uMaxVoltSwingLvl))
        {
          pDeviceCtx->uMaxPreemphLvl   = DPCD_SPEC_MAX_PRE_EMPHASIS_LEVEL;
          pDeviceCtx->uMaxVoltSwingLvl = DPCD_SPEC_MAX_VOLTAGE_SWING_LEVEL;
        }

        /* assume panel is plugged in and skip HPD */
        pDeviceCtx->bPluggedIn         = psOpenConfig->bSkipHotPlugDetect;

        if (0 != (psOpenConfig->eConfigFlags & DP_CONFIG_FLAG_ALT_MODE_FIXED_LANE_CONFIGURATION))
        {
          if (TRUE == IsAltModeDPDevice(pDeviceCtx->eDeviceID))
          {
            pDeviceCtx->uFlags = DPHOST_SETFLAG(pDeviceCtx->uFlags, DP_HOST_FLAG_ALT_MODE_FIXED_LANE_CONFIGURATION);
          }
          else
          {
            DP_LOG_MESSAGE_WARN("DP_Host_Open: Lane mapping only supported on Type-C ports, eDeviceID = %d", pDeviceCtx->eDeviceID);
          }
        }

        /* Get the Max CMD FIFO length from HAL */
        HAL_DP_AUX_GetHwCaps(pDeviceCtx->eControllerID, &sHALHwCap);
        pDeviceCtx->uMaxAUXCmdFifoLen       = sHALHwCap.uAUXCmdFifoLen;

        /* Set bonded mode configuration info */
        if (DP_STATUS_SUCCESS != (eStatus = ConfigureBondedModeInfo(pDeviceCtx, psOpenConfig)))
        {
          DP_LOG_MESSAGE_ERROR("DP: Invalid bonded mode configuration, eStatus=%d\n", eStatus);
        }
        else if (DP_STATUS_SUCCESS != (eStatus = DP_AUXClockEnable(pDeviceCtx, TRUE))) /* Enable clocks prior to any controller/phy programming */
        {
          DP_LOG_MESSAGE_ERROR("DP: Failed to enable AUX clock\n");
          eStatus = DP_STATUS_FAILED_RESOURCE_FAILED;
        }
        else if (HAL_MDSS_STATUS_SUCCESS != HAL_DP_CrossbarConfig(pDeviceCtx->eControllerID, pDeviceCtx->eDeviceID))
        {
          DP_LOG_MESSAGE_ERROR("DP: Failed to configure DP PHY crossbar\n");
          eStatus = DP_STATUS_FAILED_RESOURCE_FAILED;
        }
        else
        {
          HAL_DP_AUXConfigType     sAUXCtrlConfig;
          HAL_DP_AUXAttribType     sAUXAttrib;
          uint32                   uAuxTimeoutCount = DP_AUX_CLKS_FOR_ONE_MSEC;    // corresponds to 1 ms
          uint32                   uAuxTimeout      = DP_AUX_HW_TIMEOUT_MS;
          HAL_DP_HPDConfigType     sHPDConfig;

          /* Initialize DP HW */
          HAL_DP_Init(pDeviceCtx->eControllerID);

          /* SW Reset DP HW */
          HAL_DP_Reset(pDeviceCtx->eControllerID, (HAL_DP_MODULE_TYPE_AUX | HAL_DP_MODULE_TYPE_PHY), 0);

          /* HPD init */
          DP_OSAL_MemSet(&sHPDConfig, 0, sizeof(HAL_DP_HPDConfigType));
          sHPDConfig.eDeviceID  = pDeviceCtx->eDeviceID;
          sHPDConfig.bEnable    = TRUE;
          sHPDConfig.bActiveLow = FALSE;
          HAL_DP_HPD_Engine_Enable(pDeviceCtx->eControllerID, &sHPDConfig);

          /* Set max AUX timeout config */
          DP_OSAL_MemSet(&sAUXCtrlConfig, 0, sizeof(HAL_DP_AUXConfigType));
          DP_OSAL_MemSet(&sAUXAttrib,     0, sizeof(HAL_DP_AUXAttribType));
          sAUXAttrib.puTimeoutLimit   = &uAuxTimeout;
          sAUXAttrib.puReadZeroLimit  = &uAuxTimeout;
          sAUXAttrib.puNackDeferLimit = &uAuxTimeout;
          sAUXAttrib.puI2CDeferLimit  = &uAuxTimeout;
          sAUXAttrib.puTimeoutCount   = &uAuxTimeoutCount;
          sAUXCtrlConfig.psAUXAttrib  = &sAUXAttrib;
          HAL_DP_AUXConfig(pDeviceCtx->eControllerID, &sAUXCtrlConfig, 0);

          /* Initialize Auxiliary  */
          HAL_DP_PHY_AUX_Init(pDeviceCtx->eDeviceID);

          /* Clear any stale interrupts */
          HAL_DP_Interrupt_Clear(pDeviceCtx->eControllerID, HAL_DP_AUX_INTR_ALL, 0);

          /* Enable AUX */
          HAL_DP_AUXChannel_Enable(pDeviceCtx->eControllerID, TRUE);

        }

        if (DP_STATUS_SUCCESS != eStatus)
        {
          FreeDPController(pDeviceCtx);
        }

        *(phHandle)               = (void *) pDeviceCtx;
        pDeviceCtx->bInitialized  = TRUE;

        /*
         * according to panel spec, aux channel will ready once HPD become high
         * wait for HPD become high
         */
        if (DP_STATUS_SUCCESS != DP_CheckConnectionStatus((DP_HandleType)pDeviceCtx))
        {
          DP_LOG_MESSAGE_WARN("DP_Host_Open: Failed to detect panel connection\n");
        }
      }

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
  DP_DeviceCtxType               *pDeviceCtx     = DPHOST_GETCTX_FROM_HANDLE(hHandle);

  if (NULL == pDeviceCtx)
  {
    eStatus = DP_STATUS_FAILED_INVALID_INPUT_PARAMETER;
    DP_LOG_MESSAGE_ERROR("DP_Host_Close: Null pointer handle passed in");
  }
  else if (FALSE == pDeviceCtx->bInitialized)
  {
    DP_LOG_MESSAGE_INFO("DP_Host_Close: Device has not been Initialized yet");
  }
  else
  {
    /* if DP is still plugged in, send power state command to sink*/
    if (pDeviceCtx->bPluggedIn)
    {
       DP_WriteSinkDPCDPower(pDeviceCtx, DP_DPCD_POWERSTATE_D3);
    }

    /* if modeset, then set link to idle and disable clocks */
    if (pDeviceCtx->bLinkConfigured)
    {
       /* Push idle pattern */
       HAL_DP_SetLinkState(pDeviceCtx->eControllerID, HAL_DP_LINKCOMMAND_TYPE_PUSH_IDLE);

       /* Turn off MDP Backpressure */
       HAL_DP_SetBackPressure(pDeviceCtx->eControllerID, DP_STREAM_ID_MST0, FALSE);

       /* Disable links before clocks turned off */
       HAL_DP_Mainlink_Enable(pDeviceCtx->eControllerID, FALSE);
       HAL_DP_PHY_Power(pDeviceCtx->eDeviceID, FALSE);

       /* Decrement pixel clock source ref count, for shared resource bookkeeping in bonded mode. */
       if (0 == UpdateSourceClockRefCount(pDeviceCtx, -1))
       {
         /* Tear down required for external PLL in bonded mode */
         if (DP_STATUS_SUCCESS != (eStatus = ConfigureExternalSourceClock(pDeviceCtx, NULL, FALSE)))
         {
           DP_LOG_MESSAGE_ERROR("DP%i: ConfigureExternalSourceClock failed with error=%d in DP_Host_Close.", pDeviceCtx->eDeviceID, eStatus);
         }
       }

       /* Set the DP clocks source to CXO before turning them off*/
       DP_MainClockEnable(pDeviceCtx, NULL, FALSE);
    }

    HAL_DP_AUXChannel_Enable(pDeviceCtx->eControllerID, FALSE);

    HAL_DP_PHY_SetPHYMode(pDeviceCtx->eDeviceID, DP_PHY_MODE_USB_DATA);

    FreeDPController(pDeviceCtx);

    pDeviceCtx->bPluggedIn      = FALSE;
    pDeviceCtx->bInitialized    = FALSE;
    pDeviceCtx->bEDIDObtained   = FALSE;
    pDeviceCtx->bLinkConfigured = FALSE;
    
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
  DP_DeviceCtxType               *pDeviceCtx     = DPHOST_GETCTX_FROM_HANDLE(hHandle);

  if (NULL == pDeviceCtx)
  {
    eStatus = DP_STATUS_FAILED_INVALID_INPUT_PARAMETER;
    DP_LOG_MESSAGE_ERROR("DP_Host_Commit: Null pointer handle passed in");
  }
  /* The settings are usually applied on a SetMode */
  else
  {
    if (pDeviceCtx->uDirtyBits & DP_DIRTYFIELD_PANELMODE)
    {
      eStatus = DP_Host_SetMode(pDeviceCtx);
    }
    else
    {
      /* Individual actions not tied to a mode set */
    }

    /* Clear all dirty bits */
    pDeviceCtx->uDirtyBits = DP_DIRTYFIELD_NONE;
  }

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
  DP_DeviceCtxType               *pDeviceCtx     = DPHOST_GETCTX_FROM_HANDLE(hHandle);

  if(NULL == pDeviceCtx) 
  {
    DP_LOG_MESSAGE_ERROR("DP_Host_SetProperty: Null handle");
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
                                                    DP_DIRTYFIELD_LINK_RATE    |
                                                    DP_DIRTYFIELD_TU_PARAMETERS;
        break;
      }
      case DP_SETPROPERTY_SET_LANE_NUMBER:
      {
        pDeviceCtx->uNumLanes                     = pPropertyData->uLaneNumber;
        pDeviceCtx->uDirtyBits                   |= DP_DIRTYFIELD_SINK_ATTRIB |
                                                    DP_DIRTYFIELD_LANE_NUMBER |
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
      case DP_SETPROPERTY_CONNECTION_STATUS:
      {
        pDeviceCtx->bPluggedIn = pPropertyData->bConnected;
        break;
      }
      case DP_SETPROPERTY_EDP_PSR_ACTION:
      {
        eStatus = DP_Host_PSR(pDeviceCtx, pPropertyData->bEnabled);
        break;
      }
      case DP_SETPROPERTY_DSC_PPS_INFO:
      {
           DP_SinkDSCCapType   *pSinkDSCCap = pPropertyData->pSinkDSCCap;

           pDeviceCtx->sDSC.ubpc              = pSinkDSCCap->ubpc;
           pDeviceCtx->sDSC.ubpp              = pSinkDSCCap->ubpp;
           pDeviceCtx->sDSC.uSliceWidth       = pSinkDSCCap->uSliceWidth;
           pDeviceCtx->sDSC.uSliceHeight      = pSinkDSCCap->uSliceHeight;
           pDeviceCtx->sDSC.uSlicePerLine     = pSinkDSCCap->uSlicePerLine;
           pDeviceCtx->sDSC.uChunkSize        = pSinkDSCCap->uChunkSize;
           pDeviceCtx->sDSC.uCompressionRatio = pSinkDSCCap->uCompressionRatio;
           break;
       }
      case DP_SETPROPERTY_BRIGHTNESS_CONTROL:
      {
        switch(pPropertyData->sBrightnessInfo.eBrightnessRequest)
        {
          case DP_BRIGHTNESS_REQUEST_OFF:
          {
            eStatus = DP_WriteSinkBrightnessEnable(pDeviceCtx, FALSE);
            break;
          }
          case DP_BRIGHTNESS_REQUEST_ON:
          {
            eStatus = DP_WriteSinkBrightnessEnable(pDeviceCtx, TRUE);
            break;
          }
          case DP_BRIGHTNESS_REQUEST_LEVEL:
          {
            eStatus = DP_WriteSinkBrightnessLevel(pDeviceCtx, pPropertyData->sBrightnessInfo.fLevel);
            break;
          }
          default:
          {
            eStatus = DP_STATUS_FAILED_NOT_SUPPORTED;
            DP_LOG_MESSAGE_ERROR("DP_SETPROPERTY_BRIGHTNESS_CONTROL - unknown request type %d",
                                  pPropertyData->sBrightnessInfo.eBrightnessRequest);
            break;
          }
        }
        break;
      }
      case DP_SETPROPERTY_RESET_PHY:
      {
        if ((pPropertyData->sPHYResetInfo.eDeviceID > DP_DEVICE_ID_NONE) &&
            (pPropertyData->sPHYResetInfo.eDeviceID < DP_DEVICE_MAX))
        {
          HAL_DP_PHY_Reset(pPropertyData->sPHYResetInfo.eDeviceID,
                           pPropertyData->sPHYResetInfo.ePHYResetFlag);
        }
        else
        {
          eStatus = DP_STATUS_FAILED_INVALID_INPUT_PARAMETER;
        }     
        break;
      }
      default:
      {
        eStatus = DP_STATUS_FAILED_NOT_SUPPORTED;
        DP_LOG_MESSAGE_ERROR("DP_Host_SetProperty: Invalid property\n");
        break;
      }
    }
  }

  return eStatus;
}

/********************************************************************************************* *
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
  DP_DeviceCtxType          *pDeviceCtx     = DPHOST_GETCTX_FROM_HANDLE(hHandle);

  if ((NULL == pPropertyData)|| 
      (NULL == hHandle))
  {
    DP_LOG_MESSAGE_ERROR("DP_Host_GetProperty: Null input");
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
          DP_LOG_MESSAGE_WARN("DP_Host_GetProperty: Not supported");
        }
        break;
      }
      case DP_GETPROPERTY_DSC_SUPPORTED:
      {
          if (pDeviceCtx->bDSCSupported == TRUE)
          {
             DP_SinkDSCCapType         *pSinkDSCCap = pPropertyData->pSinkDSCCap;

             DP_Host_GetDSCSliceInfo(pDeviceCtx);

             pDeviceCtx->sDSC.uPixelPerClock = pDeviceCtx->uPixelPerClock;
             pDeviceCtx->sDSC.bDTOEnable     = pDeviceCtx->bDTOEnable;
             DP_OSAL_MemCpy(pSinkDSCCap, &pDeviceCtx->sDSC, sizeof(DP_SinkDSCCapType));
          }
          eStatus = DP_STATUS_SUCCESS;
          break;
      }
      case DP_GETPROPERTY_FRAME_CRC:
      {
         if (HAL_MDSS_STATUS_SUCCESS != HAL_DP_Get_Frame_CRC(pDeviceCtx->eControllerID, &pPropertyData->uCRC))
         {
            eStatus = DP_STATUS_FAILED_TIMEOUT;
         }
         break;
      }
      default:
      {
        DP_LOG_MESSAGE_ERROR("DP_Host_GetProperty: Not supported");
        eStatus = DP_STATUS_FAILED_NOT_SUPPORTED;
        break;
      }      
    }
  }
  
  return eStatus;
}

/*********************************************************************************************
*
** FUNCTION: DP_Host_PSR()
*/
/*!
* \DESCRIPTION
*      Allows the user to query the attributes and properties of the DP device
*
* \param [in]   hHandle        - Logical handle of DP Device
* \param [in]   uAtctive       - Enable/Disable
*
* \retval DP_Status
*
**********************************************************************************************/
static DP_Status  DP_Host_PSR(DP_DeviceCtxType  *pDeviceCtx, bool32 bEnterPSR)
{
  DP_PSRType        *pPSR       = &pDeviceCtx->sPSR;
  DP_Status         eStatus     = DP_STATUS_FAIL; 
  
  if (pPSR->sSinkCap.uPSRSupported)
  {
     if (bEnterPSR)
     {
         eStatus = eDP_PSR_Enter(pDeviceCtx);
     }
     else
     {
        eStatus = eDP_PSR_Exit(pDeviceCtx);
     }
  }
  else
  {
     eStatus = DP_STATUS_FAILED_NOT_SUPPORTED;
     DP_LOG_MESSAGE_ERROR("DP_Host_PSR: PSR is not supportted by Panel\n");
  }

  return eStatus;
}

/*********************************************************************************************
*
** FUNCTION: DP_Host_DSC_Start()
*/
/*!
* \DESCRIPTION
*     Commit host DSC registers and transmit PPS to sinker
*
* \param [in]   hHandle        - Logical handle of DP Device
*
* \retval DP_Status
*
**********************************************************************************************/
static void DP_Host_DSC_Start(DP_HandleType                   hHandle)
{
    DP_DeviceCtxType          *pDeviceCtx     = DPHOST_GETCTX_FROM_HANDLE(hHandle);

    if (TRUE == pDeviceCtx->bDSCSupported)
    {
       DP_Host_DSC_Commit(pDeviceCtx);
    }
}

/* ----------------------------------------------------------------------
** FUNCTION: DP_Host_SDP_ClientWrite()
**
** DESCRIPTION:
**   
**   DP Host sends meta data to client (panel)
**
-------------------------------------------------------------------- */
MDP_Status DP_Host_SDP_ClientWrite(DP_HandleType hDPHandle, uint32 eCommand, void *pPayload, uint32 uFlags)
{
    MDP_Status               eStatus          = MDP_STATUS_OK;

    switch (eCommand)
    {
      case DP_PANEL_COMMAND_PPS:
         DP_Host_DSC_Start(hDPHandle);
         break;
      default:
         eStatus = MDP_STATUS_NOT_SUPPORTED;
         break;
    }

    return eStatus;
}

/* ---------------------------------------------------------------------- 
*
** FUNCTION: AllocateDPController()
*/
/*!
* \DESCRIPTION
*     Map DP_DeviceIDType to DP_ControllerIDType. For external displays,
*     dynamically allocate controller. 
*
* \param [in]   eDeviceID         - Device ID
* \param [in]   pDeviceCtx        - Device context 
*
* \retval DP Controller ID
*
**********************************************************************************************/
static DP_ControllerIDType AllocateDPController(DP_DeviceIDType   eDeviceID,
                                                DP_DeviceCtxType *pDeviceCtx)
{
  DP_ControllerIDType eControllerID = DP_CONTROLLER_ID_NONE;

  if (DP_DEVICE_ID_EDP == eDeviceID)
  {
    eControllerID = DP_CONTROLLER_ID_EDP;
  }
  else if (DP_CONTROLLER_ID_NONE != pDeviceCtx->eControllerID)
  {
    // Controller already allocated, no need to re-allocate
    eControllerID = pDeviceCtx->eControllerID;
  }
  else
  {
    uint32                 uCount;

    // Find an unused controller from the pool of DP controllers
    for (uCount = 0; uCount < DP_MAX_CONTROLLER_NUM; uCount++)
    {
      DP_ControllerInfoType *pControllerInfo;

      pControllerInfo = DPHOST_GET_CONTROLLER_INFO(uCount);

      if (FALSE == pControllerInfo->bInUse)
      {
        eControllerID           = pControllerInfo->eControllerID;
        pControllerInfo->bInUse = TRUE;
        break;
      }
    }
  }

  return eControllerID;
}

/* ---------------------------------------------------------------------- 
*
** FUNCTION: FreeDPController()
*/
/*!
* \DESCRIPTION
*     Release the controller back to controller pool. 
*
* \param [in]   pDeviceCtx        - Device context 
*
* \retval 
*
************************************************************************/
static void FreeDPController(DP_DeviceCtxType *pDeviceCtx)
{
  uint32                 uCount;

  for (uCount = 0; uCount < DP_MAX_CONTROLLER_NUM; uCount++)
  {
    DP_ControllerInfoType *pControllerInfo;
    
    pControllerInfo = DPHOST_GET_CONTROLLER_INFO(uCount);

    if (pDeviceCtx->eControllerID == pControllerInfo->eControllerID)
    {
      pControllerInfo->bInUse   = FALSE;
      pDeviceCtx->eControllerID = DP_CONTROLLER_ID_NONE;
      
      break;
    }
  }
}

/*********************************************************************************************
*
** FUNCTION: ConfigureExternalSourceClock()
*/
/*!
* \DESCRIPTION
*      Configures the DP PHY PLL of a separate DP device
*
* \param [in]   pDeviceCtx       - Pointer to device context
* \param [in]   pPhyConfig       - Pointer to DP PHY PLL config info
* \param [in]   bEnable          - Flag to enable/disable PLL
*
* \retval DP_Status
*
**********************************************************************************************/
static DP_Status ConfigureExternalSourceClock(DP_DeviceCtxType     *pDeviceCtx,
                                              HAL_DP_PllConfigType *pPhyConfig,
                                              bool32                bEnable)
{
  DP_Status eStatus = DP_STATUS_SUCCESS;

  if (NULL == pDeviceCtx)
  {
    eStatus = DP_STATUS_FAILED_INVALID_INPUT_PARAMETER;
    DP_LOG_MESSAGE_ERROR("ConfigureExternalSourceClock: unexpected null pointer");
  }
  else if (DP_BONDED_MODE_SHARED_RESOURCE_EXTERNAL_PIXEL_CLOCK == pDeviceCtx->sBondedModeConfig.eSharedResourceConfig)
  {
    DP_DeviceIDType eDeviceID = pDeviceCtx->sBondedModeConfig.eSourceClockDeviceID;
  
    if (TRUE == bEnable)
    {
      HAL_DP_PHY_SetPHYMode(eDeviceID, DP_PHY_MODE_CONCURRENCY);
      
      /* Configure and enable DP PLL */
      if (FALSE == HAL_DP_PHY_Config(eDeviceID, pPhyConfig))
      {
        DP_LOG_MESSAGE_ERROR("DP%i: HAL_DP_PHY_Config failed in ConfigureExternalSourceClock.", eDeviceID);   
        eStatus = DP_STATUS_FAIL;
      }
    }
    else
    {	  
      /* Power down DP PLL */
      HAL_DP_PHY_Power(eDeviceID, FALSE);
    }
  }

  return eStatus;
}

/*********************************************************************************************
*
** FUNCTION: UpdateSourceClockRefCount()
*/
/*!
* \DESCRIPTION
*      Increments/decrements source clock ref count and returns the current value
*
* \param [in]   pDeviceCtx       - Pointer to device context
* \param [in]   iValue           - Value used to increment/decrement source clock ref count
*
* \retval int32
*
**********************************************************************************************/
static int32 UpdateSourceClockRefCount(DP_DeviceCtxType *pDeviceCtx,
                                       int32             iValue)
{
  int32 iRetVal = -1;

  if (NULL == pDeviceCtx)
  {
    DP_LOG_MESSAGE_ERROR("UpdateSourceClockRefCount: Unexpected null context.");
  }
  else if (TRUE == pDeviceCtx->sBondedModeConfig.bEnabled)
  {
    /* Increment/decrement ref count for PLL internal/external to DP device based on input value provided */
    DP_DeviceIDType  eDeviceID  = (DP_BONDED_MODE_SHARED_RESOURCE_EXTERNAL_PIXEL_CLOCK == pDeviceCtx->sBondedModeConfig.eSharedResourceConfig) ? 
                                  pDeviceCtx->sBondedModeConfig.eSourceClockDeviceID : pDeviceCtx->eDeviceID;

    if (eDeviceID < DP_DEVICE_MAX)
    {
      int32 *pRefCount = &gDpSharedData.uSourceClockRefCount[eDeviceID];

      (*pRefCount) += iValue;

      if (*pRefCount < 0)
      {
        *pRefCount = 0;
        DP_LOG_MESSAGE_WARN("UpdateSourceClockRefCount: Clock source ref count reached minimum count, eDeviceID = %d", eDeviceID);
      }
      else if (*pRefCount > DP_DEVICE_MAX)
      {
        *pRefCount = DP_DEVICE_MAX;
        DP_LOG_MESSAGE_WARN("UpdateSourceClockRefCount: Clock source ref count exceeded maximum count, eDeviceID = %d", eDeviceID);
      }

      iRetVal = *pRefCount;
    }
  }

  return iRetVal;
}

/*********************************************************************************************
*
** FUNCTION: IsAltModeDPDevice()
*/
/*!
* \DESCRIPTION
*      Return TRUE if DP Device ID belongs to an Alt-mode DP Device
*
* \param [in]   eDeviceID - DP Device ID
*
* \retval bool32
*
**********************************************************************************************/
static bool32 IsAltModeDPDevice(DP_DeviceIDType eDeviceID)
{
  bool32 bRetVal = FALSE;

  switch (eDeviceID)
  {
    case DP_DEVICE_ID_ALT_MODE_0:
    case DP_DEVICE_ID_ALT_MODE_1:
      bRetVal = TRUE;
      break;
    default:
      break;
  }

  return bRetVal;
}

/*********************************************************************************************
*
** FUNCTION: ConfigureBondedModeInfo()
*/
/*!
* \DESCRIPTION
*      Populate the device context with the bonded mode configuration info
*
* \param [in]   pDeviceCtx     - DP Device ID
* \param [in]   psOpenConfig   - Pointer to host open configuration
*
* \retval DP_Status
*
**********************************************************************************************/
static DP_Status ConfigureBondedModeInfo(DP_DeviceCtxType      *pDeviceCtx,
                                         DP_HostOpenConfigType *psOpenConfig)
{
  DP_Status eStatus = DP_STATUS_SUCCESS;

  /* Set bonded mode config */
  pDeviceCtx->sBondedModeConfig.bEnabled = psOpenConfig->sBondedModeConfig.bEnabled;

  if (TRUE == pDeviceCtx->sBondedModeConfig.bEnabled)
  {
    /* Check Device ID of device providing the source clock */
    if (psOpenConfig->sBondedModeConfig.eSourceClockDeviceID < DP_DEVICE_MAX)
    {
      if ((psOpenConfig->sBondedModeConfig.eSourceClockDeviceID > DP_DEVICE_ID_NONE) &&
          (psOpenConfig->sBondedModeConfig.eSourceClockDeviceID != pDeviceCtx->eDeviceID))
      {
        /* If DP device pixel clock needs to be sourced from a separate DP device source clock */
        pDeviceCtx->sBondedModeConfig.eSharedResourceConfig = DP_BONDED_MODE_SHARED_RESOURCE_EXTERNAL_PIXEL_CLOCK;
        pDeviceCtx->sBondedModeConfig.eSourceClockDeviceID  = psOpenConfig->sBondedModeConfig.eSourceClockDeviceID; 
        /* Sourcing an external pixel clock is asynchronous with respect to our link clock */
        pDeviceCtx->bSynchronousClk = FALSE;
      }
      else
      {
        /* If DP device pixel clock is sourced from its corresponding source clock */
        pDeviceCtx->sBondedModeConfig.eSharedResourceConfig = DP_BONDED_MODE_SHARED_RESOURCE_NONE;
        pDeviceCtx->sBondedModeConfig.eSourceClockDeviceID  = pDeviceCtx->eDeviceID;
      }
    }
    else
    {
      /* Invalid source clock Device ID */
      eStatus = DP_STATUS_FAILED_INVALID_INPUT_PARAMETER;
      DP_LOG_MESSAGE_ERROR("ConfigureBondedModeInfo() invalid source clock Device ID in bonded mode configuration %d, eDeviceID=%d",
                           psOpenConfig->sBondedModeConfig.eSourceClockDeviceID, pDeviceCtx->eDeviceID);
    }
  }
  else
  {
    pDeviceCtx->sBondedModeConfig.eSharedResourceConfig = DP_BONDED_MODE_SHARED_RESOURCE_NONE;
    pDeviceCtx->sBondedModeConfig.eSourceClockDeviceID  = DP_DEVICE_ID_NONE;
  }

  return eStatus;
}

#ifdef __cplusplus
}
#endif

