/*=====================================================================================================================

File: dp_edid.c

Source file for DP EDID read and parse related functionality

Copyright (c) 2016-2018 Qualcomm Technologies, Inc.
All Rights Reserved.
Confidential and Proprietary - Qualcomm Technologies, Inc.
=======================================================================================================================*/


/*---------------------------------------------------------------------------------------------------------------------
 * Include Files
 *-------------------------------------------------------------------------------------------------------------------*/
#include "dp_i.h"
#include "edid.h"

#ifdef __cplusplus
extern "C" {
#endif


/**********************************************************************************************************************

FUNCTION: DP_ParseSinkEDID()
   
   This function will parse the DPCD Capability from the Raw Data. 

Parameters:
  pDeviceCtx                         -[in] Pointer to device context

Return:
   DP_Status 

**********************************************************************************************************************/

DP_Status  DP_ParseSinkEDID(DP_DeviceCtxType    *pDeviceCtx)
{
  DP_Status               eStatus             = DP_STATUS_SUCCESS;
  DP_HandleType           hEDIDParserHandle   = NULL;

  if(NULL == pDeviceCtx)
  {
    DP_LOG_MESSAGE_ERROR("DP_OSAL_ParseEDID(): unexpected null handle ");
    eStatus = DP_STATUS_FAILED_INVALID_INPUT_PARAMETER;
  }
  /* Open handle to EDID parser */
  else if (MDP_STATUS_OK != EDID_OpenParser(&hEDIDParserHandle))
  {
    DP_LOG_MESSAGE_ERROR("Failed to open handle to EDID parser");
    eStatus = DP_STATUS_FAILED_RESOURCE_FAILED;
  }
  else
  {
      
      EDID_ParserInfoType    sEDIDInfo;
      EDID_DispModeListType  sModeList;

      DISP_OSAL_MemZero((void *)&sEDIDInfo, sizeof(EDID_ParserInfoType));
      DISP_OSAL_MemZero((void *)&sModeList, sizeof(EDID_DispModeListType));

      /* Copy EDID and other filtering info */
      sEDIDInfo.pEDIDBuffer         = (void *)pDeviceCtx->auEDIDCache;
      sEDIDInfo.uEDIDDataLength     = pDeviceCtx->uEDIDDataLength;
      sEDIDInfo.eEDIDSpecType       = EDID_DP_SPEC_TYPE_1_3;

      /*
       * following parameters are panel resolution filter criteria at edid parser
       */
      sEDIDInfo.uMaxModeWidth       = DP_MAX_RESOLUTION_WIDTH;
      sEDIDInfo.uMaxModeHeight      = DP_MAX_RESOLUTION_HEIGHT;
      sEDIDInfo.uMaxModeRefreshRate = DP_MAX_REFRESH_RATE_HZ*1000;
      sEDIDInfo.uMaxLinkRate        = (uint64)pDeviceCtx->sSinkDPCDCap.uMaxLinkRateInKhz * 10000;
      sEDIDInfo.uNumberOfLanes      = pDeviceCtx->sSinkDPCDCap.uMaxLaneCount;
      sEDIDInfo.uMinModeWidth       = 0;
      sEDIDInfo.uMinModeHeight      = 0;
      sEDIDInfo.uMinModeRefreshRate = 0;

      if (EDID_STATUS_OK != EDID_GetModeList(hEDIDParserHandle, &sEDIDInfo, &sModeList))
      {
        DP_LOG_MESSAGE_ERROR("Failed to get filtered mode list. status %d", (uint32)eStatus);
        eStatus = DP_STATUS_FAIL;
      }
      else
      {
        uint32     uModeindex = 0;
        

        pDeviceCtx->uNumModes = sModeList.uNumOfElements < MAX_DP_PANEL_MODE ? sModeList.uNumOfElements : MAX_DP_PANEL_MODE;
        for (uModeindex = 0; uModeindex < pDeviceCtx->uNumModes; ++uModeindex)
        {
          pDeviceCtx->sPanelInfo[uModeindex].uModeIndex               = uModeindex;
          pDeviceCtx->sPanelInfo[uModeindex].uPclkFreq                = sModeList.aDispModeAttr[uModeindex].sDisplayModeInfo.uPixelRate;
          pDeviceCtx->sPanelInfo[uModeindex].uVisibleWidthInPixels    = sModeList.aDispModeAttr[uModeindex].sDisplayModeInfo.uWidthPx;
          pDeviceCtx->sPanelInfo[uModeindex].uHsyncFrontPorchInPixels = sModeList.aDispModeAttr[uModeindex].sActiveTimingInfo.iHsyncFrontPorchDclk;
          pDeviceCtx->sPanelInfo[uModeindex].uHsyncBackPorchInPixels  = sModeList.aDispModeAttr[uModeindex].sActiveTimingInfo.iHsyncBackPorchDclk;
          pDeviceCtx->sPanelInfo[uModeindex].uHsyncPulseInPixels      = sModeList.aDispModeAttr[uModeindex].sActiveTimingInfo.iHsyncPulseWidthDclk;
          pDeviceCtx->sPanelInfo[uModeindex].uHsyncSkewInPixels       = sModeList.aDispModeAttr[uModeindex].sActiveTimingInfo.iHsyncSkewDclk;
          pDeviceCtx->sPanelInfo[uModeindex].uVisibleHeightInPixels   = sModeList.aDispModeAttr[uModeindex].sDisplayModeInfo.uHeightPx;
          pDeviceCtx->sPanelInfo[uModeindex].uVsyncFrontPorchInLines  = sModeList.aDispModeAttr[uModeindex].sActiveTimingInfo.iVsyncFrontPorchLines;
          pDeviceCtx->sPanelInfo[uModeindex].uVsyncBackPorchInLines   = sModeList.aDispModeAttr[uModeindex].sActiveTimingInfo.iVsyncBackPorchLines;
          pDeviceCtx->sPanelInfo[uModeindex].uVsyncPulseInLines       = sModeList.aDispModeAttr[uModeindex].sActiveTimingInfo.iVsyncPulseWidthLines;
          pDeviceCtx->sPanelInfo[uModeindex].bHSyncActiveLow          = sModeList.aDispModeAttr[uModeindex].sActiveTimingInfo.bIsHsyncActiveLow;
          pDeviceCtx->sPanelInfo[uModeindex].bVSyncActiveLow          = sModeList.aDispModeAttr[uModeindex].sActiveTimingInfo.bIsVsyncActiveLow;
          pDeviceCtx->sPanelInfo[uModeindex].uHLeftBorderInPixels     = sModeList.aDispModeAttr[uModeindex].sActiveTimingInfo.iHLeftBorderDClk;
          pDeviceCtx->sPanelInfo[uModeindex].uHRightBorderInPixels    = sModeList.aDispModeAttr[uModeindex].sActiveTimingInfo.iHRightBorderDClk;
          pDeviceCtx->sPanelInfo[uModeindex].uVTopBorderInLines       = sModeList.aDispModeAttr[uModeindex].sActiveTimingInfo.iVTopBorderLines;
          pDeviceCtx->sPanelInfo[uModeindex].uVBottomBorderInLines    = sModeList.aDispModeAttr[uModeindex].sActiveTimingInfo.iVBottomBorderLines;
          pDeviceCtx->sPanelInfo[uModeindex].uRefreshRate             = sModeList.aDispModeAttr[uModeindex].sDisplayModeInfo.uRefreshRate;
          pDeviceCtx->sPanelInfo[uModeindex].bInterlaced              = sModeList.aDispModeAttr[uModeindex].sDisplayModeInfo.bInterlaced;
          pDeviceCtx->sPanelInfo[uModeindex].bAudioSupported          = sModeList.aDispModeAttr[uModeindex].sDisplayModeInfo.bAudioSupported;
          pDeviceCtx->sPanelInfo[uModeindex].u3DFrameFormat           = sModeList.aDispModeAttr[uModeindex].sDisplayModeInfo.e3DFrameFormat;
          pDeviceCtx->sPanelInfo[uModeindex].uAspectRatio             = sModeList.aDispModeAttr[uModeindex].sDisplayModeInfo.eAspectRatio;
          pDeviceCtx->sPanelInfo[uModeindex].uRotation                = sModeList.aDispModeAttr[uModeindex].sDisplayModeInfo.eRotation;

          if (EDID_PIXEL_COLOR_FORMAT_RGB_666_18BPP == sModeList.aDispModeAttr[uModeindex].sDisplayModeInfo.eColorFormat)
          {
              pDeviceCtx->sPanelInfo[uModeindex].eColorFormat = DP_PIXEL_FORMAT_RGB_666_18BPP;
          }
          else if (EDID_PIXEL_COLOR_FORMAT_RGB_101010_30BPP == sModeList.aDispModeAttr[uModeindex].sDisplayModeInfo.eColorFormat)
          {  
              pDeviceCtx->sPanelInfo[uModeindex].eColorFormat = DP_PIXEL_FORMAT_RGB_101010_30BPP;
          }
          else
          {
              pDeviceCtx->sPanelInfo[uModeindex].eColorFormat = DP_PIXEL_FORMAT_RGB_888_24BPP;
          }

          DP_LOG_MESSAGE_INFO("DP_ParseSinkEDID: ndx=%d pclk=%d w=%d hfp=%d bfp=%d h=%d vfp=%d vbp=%d\n",
                               pDeviceCtx->sPanelInfo[uModeindex].uModeIndex,
                               pDeviceCtx->sPanelInfo[uModeindex].uPclkFreq,
                               pDeviceCtx->sPanelInfo[uModeindex].uVisibleWidthInPixels,
                               pDeviceCtx->sPanelInfo[uModeindex].uHsyncFrontPorchInPixels,
                               pDeviceCtx->sPanelInfo[uModeindex].uHsyncBackPorchInPixels,
                               pDeviceCtx->sPanelInfo[uModeindex].uVisibleHeightInPixels,
                               pDeviceCtx->sPanelInfo[uModeindex].uVsyncFrontPorchInLines,
                               pDeviceCtx->sPanelInfo[uModeindex].uVsyncBackPorchInLines); 
        }
      }

      /* Close handle to EDID parser */
      EDID_CloseParser(hEDIDParserHandle);
  }

  return eStatus;
}

/**********************************************************************************************************************

FUNCTION: DP_ReadSinkEDIDRaw()

  This function will read EDID Raw Data from Sink over I2c.

Parameters:
  pDeviceCtx                         -[in] Pointer to device context

Return:
  DP_Status

**********************************************************************************************************************/
DP_Status  DP_ReadSinkEDIDRaw(DP_DeviceCtxType    *pDeviceCtx)
{
  DP_Status  eStatus       = DP_STATUS_SUCCESS; 

  if (NULL == pDeviceCtx)
  {
    DP_LOG_MESSAGE_ERROR("DP_ReadSinkEDIDRaw() : DP handle is NULL pointer\n");
    eStatus = DP_STATUS_FAILED_INVALID_INPUT_PARAMETER;
  }
  else
  {
    DP_DeviceIDType         eDeviceId    = pDeviceCtx->eDeviceID;
    uint8                  *puEDID       = pDeviceCtx->auEDIDCache;
    uint8                   uExtBlockNum = 0;
    uint8                   uWriteData   = 0;
    DP_ReadPacketType       sReadPacket;
    DP_WritePacketType      sWritePacket;

    /* Set default return EDID length */
    pDeviceCtx->uEDIDDataLength = 0;

    /* Write read request to EDID Data, Block 0 */
    sWritePacket.eTransactionType    = DP_TRANSACTION_MODE_I2C;
    sWritePacket.puWriteData         = &uWriteData;
    sWritePacket.uAddress            = DP_EDID_START_ADDRESS;
    sWritePacket.uWriteDataSize      = 1;

    if (DP_STATUS_SUCCESS != (eStatus = DP_Host_ClientWrite(eDeviceId, &sWritePacket)))
    {
      DP_LOG_MESSAGE_ERROR("DP_ReadSinkEDIDRaw() : DP_Host_ClientWrite Failed, eStatus=%d\n", eStatus);
    }
    else
    {
      /* Read EDID Data Block 0*/
      sReadPacket.eTransactionType     = DP_TRANSACTION_MODE_I2C;
      sReadPacket.puReadDataBuffer     = (uint8*)puEDID;
      sReadPacket.uAddress             = DP_EDID_START_ADDRESS;
      sReadPacket.uReadSize            = DP_EDID_BLOCK_SIZE;
      sReadPacket.bQueueOnly           = FALSE;
    
      if (DP_STATUS_SUCCESS != (eStatus = DP_Host_ClientRead(eDeviceId, &sReadPacket)))
      {
        DP_LOG_MESSAGE_ERROR("DP_ReadSinkEDIDRaw() : DP_Host_ClientRead Failed, eStatus=%d\n", eStatus);
      }
      else if(sReadPacket.uBytesRead != sReadPacket.uReadSize)
      {
        DP_LOG_MESSAGE_ERROR( "DP_ReadSinkEDIDRaw() : Read Failed bytes read %d (expected %d)\n", sReadPacket.uBytesRead, sReadPacket.uReadSize);
        eStatus = DP_STATUS_FAILED_RESOURCE_FAILED;
      }
      else
      {
        /* Block 0 size*/
        pDeviceCtx->uEDIDDataLength += DP_EDID_BLOCK_SIZE;
      
        /* Get Extension block number */
        uExtBlockNum = puEDID[DP_EDID_EXT_BLOCK_NUM_OFFSET];
        if (uExtBlockNum >= 1)
        {
          uint8 uBlockIdx    = 1;
          uint8 uSegmetnNum  = 0;
          uint8 uOffset      = 0;

          for (uBlockIdx = 1; uBlockIdx <= uExtBlockNum; uBlockIdx++)
          {
            /* Update Extension segment number, uSegmetnNum=1 for blocks 2 and 3; =2 for block 4 and 5.... */
            if (uBlockIdx >= 2)
            {
              uSegmetnNum           = uBlockIdx >> 1;
              sWritePacket.uAddress = DP_EDID_SEGMENT_ADDRESS;
              uWriteData            = uSegmetnNum;
              if (DP_STATUS_SUCCESS != (eStatus = DP_Host_ClientWrite(eDeviceId, &sWritePacket)))
              {
                DP_LOG_MESSAGE_ERROR("DP_ReadSinkEDIDRaw() : Failed to update extension segment number, eStatus=%d\n", eStatus);
                break;
              }
            }

            /* Write read request to next EDID block */
            uOffset               = (uBlockIdx % 2 == 0) ? 0 : DP_EDID_BLOCK_SIZE;
            sWritePacket.uAddress = DP_EDID_START_ADDRESS;
            uWriteData            = uOffset;
            if (DP_STATUS_SUCCESS != (eStatus = DP_Host_ClientWrite(eDeviceId, &sWritePacket)))
            {
              DP_LOG_MESSAGE_ERROR("DP_ReadSinkEDIDRaw() : Failed to write EDID block %d, eStatus=%d\n", uBlockIdx, eStatus);
              break;
            }
            else
            {
              /* Read next EDID Data Block */
              sReadPacket.puReadDataBuffer = (uint8*)&puEDID[uBlockIdx*DP_EDID_BLOCK_SIZE];
              if (DP_STATUS_SUCCESS != (eStatus = DP_Host_ClientRead(eDeviceId, &sReadPacket)))
              {
                DP_LOG_MESSAGE_ERROR("DP_ReadSinkEDIDRaw() : Read EDID block %d failed, eStatus=%d\n", uBlockIdx, eStatus);
              }
              else if (sReadPacket.uBytesRead != sReadPacket.uReadSize)
              {
                DP_LOG_MESSAGE_ERROR("DP_ReadSinkEDIDRaw() : Read EDID block %d failed bytes read %d (expected %d)\n", uBlockIdx, sReadPacket.uBytesRead, sReadPacket.uReadSize);
                eStatus = DP_STATUS_FAILED_RESOURCE_FAILED;
                break;
              }
              else
              {
                pDeviceCtx->uEDIDDataLength += DP_EDID_BLOCK_SIZE;
                
                /* Still have EDID blocks unread*/
                if ((pDeviceCtx->uEDIDDataLength + DP_EDID_BLOCK_SIZE > DP_EDID_MAX_BUF_SIZE) &&
                    (uBlockIdx <= uExtBlockNum))
                {
                  DP_LOG_MESSAGE_WARN("DP_ReadSinkEDIDRaw() : EDID buffer too small, %d bytes unread\n", (uExtBlockNum- uBlockIdx)*DP_EDID_BLOCK_SIZE);
                  break;
                }
              }
            }
          }
        }
      }
    }
  }

  return eStatus;
}

#ifdef __cplusplus
}
#endif
