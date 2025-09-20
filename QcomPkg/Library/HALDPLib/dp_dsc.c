/*=====================================================================================================================
 
  File: dp_dsc.c
 
  Source file for DP DSC related functionality 
   
  Copyright (c) 2018 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
=======================================================================================================================*/


/*---------------------------------------------------------------------------------------------------------------------
 * Include Files
 *-------------------------------------------------------------------------------------------------------------------*/
#include "dp_i.h"
#include "hal_dp_i.h"
#include "hal_mdp.h"
#include "hal_dp_dsc.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct
{
  uint32 uPeakPixelRate;       /* MPixel */
  uint32 uSlicePerLine;
  uint32 uAlterSlicePerLine;   /* databus_width enabled, 6 pixel per clock */
} PeakPixelRateToSliceType;

PeakPixelRateToSliceType   gPPRSliceTable[] =
{
       { 340,   1,   1},
       { 680,   2,   1},
       {1360,   4,   2},
       {3200,   8,   4},
       {4800,  12,   8},
       {6400,  16,   8},
       {8000,  20,  12},
       {9600,  24,  12},
       {   0,   0,   0},
};

uint32 aSliceHeight[] = {40, 36, 32, 30, 25, 24, 20, 16, 15, 0};

/*********************************************************************************************
*
** FUNCTION: DP_Host_GetDSCInfo(DP_DeviceCtxType *pDeviceCtx)
*/
/*!
* \DESCRIPTION
*      get DSC Slice info (height, width and slice per line).
*
* \param [in]   pDeviceCtx        - Pointer to device context
*
* \retval DP_Status
*
**********************************************************************************************/
void DP_Host_GetDSCSliceInfo(DP_DeviceCtxType *pDeviceCtx)
{

   DP_PanelInfoType       *psPanelInfo  = &(pDeviceCtx->sPanelInfo[pDeviceCtx->uModeIndex]);
   uint32 uRate;
   uint32 uWidth;
   uint32 uHeight;
   uint32 uPictureHeight;
   uint32 uIndex = 0;
   uint32 uFreq;
   uint32 uSlice;
   PeakPixelRateToSliceType  *pPPRSlice;

   DP_LOG_MESSAGE_INFO("DP_DSC_SlicePerLine: ID=%d eId=%d ndx=%d pclk=%d w=%d hfp=%d bfp=%d h=%d vfp=%d vbp=%d frate=%x\n",
                               pDeviceCtx->eDeviceID,
                               pDeviceCtx->eControllerID,
                               psPanelInfo->uModeIndex,
                               psPanelInfo->uPclkFreq,
                               psPanelInfo->uVisibleWidthInPixels,
                               psPanelInfo->uHsyncFrontPorchInPixels,
                               psPanelInfo->uHsyncBackPorchInPixels,
                               psPanelInfo->uVisibleHeightInPixels,
                               psPanelInfo->uVsyncFrontPorchInLines,
                               psPanelInfo->uVsyncBackPorchInLines,
                               psPanelInfo->uRefreshRate);

    uWidth = psPanelInfo->uVisibleWidthInPixels + psPanelInfo->uHsyncFrontPorchInPixels + psPanelInfo->uHsyncBackPorchInPixels;
    uHeight = psPanelInfo->uVisibleHeightInPixels + psPanelInfo->uVsyncFrontPorchInLines + psPanelInfo->uVsyncBackPorchInLines;


    uFreq = psPanelInfo->uRefreshRate >> 16;
    uRate = uWidth * uHeight * uFreq;
    uRate /= 1000000;

    pPPRSlice = gPPRSliceTable;

    while(1)
    {
       if (uRate <= pPPRSlice->uPeakPixelRate)
       {
          if (pDeviceCtx->uPixelPerClock == 2)   /* 2 pixels per clock at interface */ 
          {
             uSlice = pPPRSlice->uAlterSlicePerLine;
          }
          else
          {
             uSlice =  pPPRSlice->uSlicePerLine;
          }
          break;
       }
       pPPRSlice++;
    }

    pDeviceCtx->sDSC.uSlicePerLine = uSlice;

    uPictureHeight = psPanelInfo->uVisibleHeightInPixels;

    /* calculate slice height  */
    uIndex = 0;
    do {
       uHeight = aSliceHeight[uIndex++];
       if ((uPictureHeight % uHeight) == 0)
       {
         break;
       }
    } while (uHeight != 0);


    if (uHeight == 0)
    {
       uHeight = 16;
       DP_LOG_MESSAGE_ERROR("DP_DSC_SliceHeight: Error, No proper Slice Height Found for Picture Height=%d, default slice height is 16.\n", uPictureHeight);
    }

    pDeviceCtx->sDSC.uSliceHeight = uHeight;
    pDeviceCtx->sDSC.uSliceWidth  = (psPanelInfo->uVisibleWidthInPixels / uSlice);
}

// c * a ^ 1
uint8 getG0(uint8 data)
{
    uint8 c[4];
    uint8 g[4];
    uint8 rData = 0;
    uint8 num;

    // Convert to single bit array for multiply
    for(num = 0; num < 4; num++)
    {
        c[num] = (data >> num) & 0x01;
    }

    g[0] = c[3];
    g[1] = c[0] ^ c[3];
    g[2] = c[1];
    g[3] = c[2];

    for(num = 0; num < 4; num++)
    {
        rData = ((g[num] & 0x01) << num) | rData;
    }

    return(rData);
}

// c * a ^ 4
uint8 getG1(uint8 data)
{
    uint8 c[4];
    uint8 g[4];
    uint8 rData = 0;
    uint8 num;

    // Convert to single bit array for multiply
    for(num = 0; num < 4; num++)
    {
        c[num] = (data >> num) & 0x01;
    }

    g[0] = c[0] ^ c[3];
    g[1] = c[0] ^ c[1] ^ c[3];
    g[2] = c[1] ^ c[2];
    g[3] = c[2] ^ c[3];

    for(num = 0; num < 4; num++)
    {
        rData = ((g[num] & 0x01) << num) | rData;
    }

    return rData;
}

/*********************************************************************************************
*
** FUNCTION: DP_Host_DSC_Calc_ECC()
*/
/*!
* \DESCRIPTION
*      Calculate SDP parity BYte out of data Byte
*
*   pDeviceCtx                       -[in ] Pointer to device context
*
* \retval DP_Status
*
**********************************************************************************************/
uint8 DP_Host_DSC_Calc_ECC(uint32 uData, uint8 numByte)
{
    uint8 x0 = 0;
    uint8 x1 = 0;
    uint8 ci = 0;
    uint8 uNibble = 0;
    uint8 ParityByte;
    uint8 num = 0;

    for(num = 0; num < numByte*2; num++)
    {
       uNibble = (uData >> num*4) & 0xF;

       ci = uNibble ^ x1;
       x1 = x0 ^ getG1(ci);
       x0 = getG0(ci);
    }

    ParityByte = x1 | (x0 << 4);

    return ParityByte;
}

/*********************************************************************************************
*
** FUNCTION: DP_Host_Sink_FEC_Enable()
*/
/*!
* \DESCRIPTION
*      enable sinker's FEC also setup host compression mode and DTO
*
* \param [in]   pDeviceCtx        - Pointer to device context
*
* \retval DP_Status
*
**********************************************************************************************/
DP_Status  DP_Host_Sink_FEC_Enable(DP_DeviceCtxType  *pDeviceCtx)
{

    DP_WriteSinkFECConfig(pDeviceCtx);

    /* enable host dsc */
    HAL_DP_Host_DSC_CompressionMode(pDeviceCtx);
    HAL_DP_Host_DSC_DTO(pDeviceCtx);

    return DP_STATUS_SUCCESS; 
}

/*********************************************************************************************
*
** FUNCTION: DP_Host_Sink_DSC_Enable()
*/
/*!
* \DESCRIPTION
*      enable sinker's  DSC
*
* \param [in]   pDeviceCtx        - Pointer to device context
*
* \retval DP_Status
*
**********************************************************************************************/
DP_Status  DP_Host_Sink_DSC_Enable(DP_DeviceCtxType  *pDeviceCtx)
{
      return DP_WriteSinkDSCEnable(pDeviceCtx);
}

/*********************************************************************************************
*
** FUNCTION: DP_Host_DSC_Commit()
*/
/*!
* \DESCRIPTION
*      comit host side DSC and send PPS at next vsync
*
* \param [in]   pDeviceCtx        - Pointer to device context
*
* \retval DP_Status
*
**********************************************************************************************/
DP_Status  DP_Host_DSC_Commit(DP_DeviceCtxType  *pDeviceCtx)
{
  DP_Status               eStatus      = DP_STATUS_FAIL; 

  if (pDeviceCtx->bDSCSupported == TRUE)
  {

      /* setup host */
      HAL_DP_Host_FEC_Enable(pDeviceCtx);

      DP_Host_Sink_DSC_Enable(pDeviceCtx);

      HAL_DP_Host_DSC_PPS(pDeviceCtx);
     
      HAL_DP_Host_DSC_PPS_Flush(pDeviceCtx);

      eStatus     = DP_STATUS_SUCCESS; 
  }

  return eStatus;
}

#ifdef __cplusplus
}
#endif
