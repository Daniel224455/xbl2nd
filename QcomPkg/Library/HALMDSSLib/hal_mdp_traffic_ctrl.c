/*! \file */

/*
===========================================================================

FILE:        hal_mdp_traffic_ctrl.c

DESCRIPTION: Manages traffic control of memory transactions between MDP 
             and VBIF that interfaces to system memory.
  
===========================================================================
  Copyright (c) 2012-2018 Qualcomm Technologies, Inc.  All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential.
===========================================================================
*/

/* -----------------------------------------------------------------------
 * Includes
 * ----------------------------------------------------------------------- */

#include "hal_mdp_i.h"

#ifdef __cplusplus
extern "C" {
#endif

/* -------------------------------------------------------------------------------------
 * Local defines 
 * ------------------------------------------------------------------------------------ */



#define HAL_MDP_TRAFFIC_SHAPER_BYTES_PER_CLOCK_MIN                  1
#define HAL_MDP_TRAFFIC_SHAPER_BYTES_PER_CLOCK_MAX                  HWIO_MDP_VP_0_VIG_0_SSPP_TRAFFIC_SHAPER_BYTES_PER_CLOCK_BMSK
                                                                    /* width of the field is 8-bit */
/*------------------------------------------------------------------------
 * Global Data Definitions
 *------------------------------------------------------------------------ */

/* -----------------------------------------------------------------------
 * Static data declarations and functions
 * ----------------------------------------------------------------------- */

/****************************************************************************
*
** FUNCTION: HAL_MDP_TrafficCtrl_SetClientWatermarkLvl()
*/
/*!
* \brief
*     Sets a read client's watermark levels based on client's latency need.
*
* \param [in] eLatency       - latency setting for the client; 
* \param [in] uTotalSMPBytes - total number of bytes in SMP allocated for the client;
* \param [in] uFlags               - If configured surface is MacroTiled;
* \param [in] uRegAddr       - address of watermark level-0 register;
*
* \retval HAL_MDSS_ErrorType
*
****************************************************************************/
static HAL_MDSS_ErrorType HAL_MDP_TrafficCtrl_SetClientWatermarkLvl (
                              HAL_MDP_TrafficCtrl_LatencyType  eLatency,
                              uint32                           uTotalSMPBytes,
                              uint32                           uFlags,
                              uintPtr                          uRegAddr )
{
   HAL_MDSS_ErrorType    eStatus          = HAL_MDSS_STATUS_SUCCESS;

   if (NULL != pgsMdpHwInfo->sIpFxnTbl.WatermarkLvlConfig)
   {
     eStatus = pgsMdpHwInfo->sIpFxnTbl.WatermarkLvlConfig(eLatency, uTotalSMPBytes, uFlags, uRegAddr);
   }
   else
   {
      eStatus = HAL_MDSS_STATUS_FAILED_NOT_SUPPORTED;
   }

   return eStatus;
}

/****************************************************************************
*
** FUNCTION: HAL_MDP_TrafficCtrl_SetTrfShaper()
*/
/*!
* \brief
*     Configures traffice shaper.
*
* \param [in] bEnTrfShaper - enable/disable the traffic shaper;
* \param [in] uBytesPerClk - Bytes per clock as required by traffic shaper; 
* \param [in] uRegAddr     - address of the traffic shaper register;
*
* \retval HAL_MDSS_ErrorType
*
****************************************************************************/
static HAL_MDSS_ErrorType HAL_MDP_TrafficCtrl_SetTrfShaper ( 
                              bool32  bEnTrfShaper,
                              uint32  uBytesPerClk,
                              uintPtr uRegAddr )
{
   HAL_MDSS_ErrorType    eStatus      = HAL_MDSS_STATUS_SUCCESS;
   uint32                uRegValue    = 0;

   if (TRUE == bEnTrfShaper)
   {  
      uBytesPerClk = (uBytesPerClk > HAL_MDP_TRAFFIC_SHAPER_BYTES_PER_CLOCK_MAX) ?
                      HAL_MDP_TRAFFIC_SHAPER_BYTES_PER_CLOCK_MAX : uBytesPerClk;

      if (uBytesPerClk < HAL_MDP_TRAFFIC_SHAPER_BYTES_PER_CLOCK_MIN)
      {
         uBytesPerClk = HAL_MDP_TRAFFIC_SHAPER_BYTES_PER_CLOCK_MIN;
      }
   }
   else
   {
      uBytesPerClk = 0;
   }

   uRegValue = HWIO_OUT_FLD(uRegValue,
                            MDP_VP_0_VIG_0_SSPP_TRAFFIC_SHAPER,
                            EN,
                            bEnTrfShaper);
   uRegValue = HWIO_OUT_FLD(uRegValue,
                            MDP_VP_0_VIG_0_SSPP_TRAFFIC_SHAPER,
                            BYTES_PER_CLOCK,
                            uBytesPerClk);
   out_dword(uRegAddr, uRegValue);

   return eStatus;
}


/****************************************************************************
*
** FUNCTION: HAL_MDP_TrafficCtrl_InitClients()
*/
/*!
* \brief
*     Initializes settings of traffic control clients with consecutive IDs.
*
* \param [in] eClientId     - ID of the first client; IDs of the following  
*                             clients must be consecutive to this;
* \param [in] uNumOfClients - number of clients with consecutive IDs;
*
* \retval None
*
****************************************************************************/
static void HAL_MDP_TrafficCtrl_InitClients (
                HAL_MDP_TrafficCtrl_ClientIDType   eClientId,
                uint32                             uNumOfClients )
{
  HAL_MDP_PerfCtrlParamsType         sSetting[HAL_MDP_TRFCTRL_CLIENT_MAX]  = {{0}};
  HAL_MDP_PerfCtrlParamsType        *psSetting                             = &sSetting[0];
  HAL_MDP_FifoWatermark_ConfigType   sFifoWatermarkSetting                 = {0};
  HAL_MDP_TraficSharper_ConfigType   sTrfShaperSetting                     = {0};
  uint32                             uIndex                                = 0;
  
  if (0 != uNumOfClients)   /* check this condition to avoid unnecessary function calls */
  {
  
     // Set traffic control for the layer
     sTrfShaperSetting.bEnableTrfShaper   = FALSE;
     sTrfShaperSetting.uBytesPerClk       = 0;
  
     // Fifo Watermark setting, set default for Non Realtime client
     sFifoWatermarkSetting.uTotalSMPBytes = 0; // configure default watemark
     
     for (uIndex = 0; uIndex < uNumOfClients; uIndex++)
     {
        psSetting->eClientId               = eClientId;
        psSetting->eReadClientLatency      = (eClientId <= HAL_MDP_TRFCTRL_CLIENT_LAYER4_CURSOR) ?
                                                           HAL_MDP_TRFCTRL_LATENCY_REALTIME : HAL_MDP_TRFCTRL_LATENCY_BEST_EFFORT;
        psSetting->psTrfShaperSetting      = &sTrfShaperSetting;
        psSetting->psFifoWatermarkSetting  = &sFifoWatermarkSetting;
  
        
        eClientId++;
        psSetting++;
     }
  
     if (HAL_MDSS_STATUS_SUCCESS != HAL_MDP_TrafficCtrl_Setup(&sSetting[0], uNumOfClients, 0, 0))
     {
        HAL_MDP_ASSERT();
     }
  }

}


/* -------------------------------------------------------------------------------------
 * Public functions
 * ------------------------------------------------------------------------------------ */


/****************************************************************************
*
** FUNCTION: HAL_MDP_TrafficCtrl_Setup()
*/
/*!
* \brief
*     Configures the traffic control of memory transactions between MDP and VBIF
*     that interfaces to system memory.
*
* \param [in] psTrfCtrlSetting - traffic control settings;
* \param [in] uNumOfSettings   - number of settings;
* \param [in] uMdpCoreClkFreq  - MDP core clock working frequency in Hz;
* \param [in] uFlags           - reserved flags
*
* \retval HAL_MDSS_ErrorType
*
****************************************************************************/
HAL_MDSS_ErrorType HAL_MDP_TrafficCtrl_Setup (HAL_MDP_PerfCtrlParamsType *psPerfCtrlSetting,
                                              uint32                      uNumOfSettings,
                                              uint32                      uMdpCoreClkFreq,
                                              uint32                      uFlags )
  {
     HAL_MDSS_ErrorType          eStatus     = HAL_MDSS_STATUS_SUCCESS;
     HAL_MDP_PerfCtrlParamsType *psSetting   = psPerfCtrlSetting; 
     uint32                      uIndex      = 0;
     uintPtr                     uRegAddr    = 0;
     uintPtr                     uRegAddrInc = 0;
  
     for(uIndex = 0; uIndex < uNumOfSettings; uIndex++)
     {
        switch (psSetting->eClientId)
        {
        case HAL_MDP_TRFCTRL_CLIENT_VIG0:
        case HAL_MDP_TRFCTRL_CLIENT_VIG1:
        case HAL_MDP_TRFCTRL_CLIENT_VIG2:
        case HAL_MDP_TRFCTRL_CLIENT_VIG3:
          uRegAddrInc =  HWIO_MDP_VP_0_VIG_1_SSPP_TRAFFIC_SHAPER_ADDR  - HWIO_MDP_VP_0_VIG_0_SSPP_TRAFFIC_SHAPER_ADDR;
          uRegAddr    =  HWIO_MDP_VP_0_VIG_0_SSPP_TRAFFIC_SHAPER_ADDR  + (psSetting->eClientId - HAL_MDP_TRFCTRL_CLIENT_VIG0) * uRegAddrInc;
          break;
  
        case HAL_MDP_TRFCTRL_CLIENT_DMA0:
        case HAL_MDP_TRFCTRL_CLIENT_DMA1:
        case HAL_MDP_TRFCTRL_CLIENT_DMA2:
        case HAL_MDP_TRFCTRL_CLIENT_DMA3:
          uRegAddrInc =  HWIO_MDP_VP_0_DMA_1_SSPP_TRAFFIC_SHAPER_ADDR  - HWIO_MDP_VP_0_DMA_0_SSPP_TRAFFIC_SHAPER_ADDR;
          uRegAddr    =  HWIO_MDP_VP_0_DMA_0_SSPP_TRAFFIC_SHAPER_ADDR  + (psSetting->eClientId - HAL_MDP_TRFCTRL_CLIENT_DMA0) * uRegAddrInc;
          break;
  
        case HAL_MDP_TRFCTRL_CLIENT_ROT_SSPP:
          uRegAddr    = HWIO_ROT_SSPP_TRAFFIC_SHAPER_ADDR;
          break;

        case HAL_MDP_TRFCTRL_CLIENT_ROT_WB:
          uRegAddr    = HWIO_ROT_WB_TRAFFIC_SHAPER_WR_CLIENT_ADDR;
          break;
          
        case HAL_MDP_TRFCTRL_CLIENT_WB2:
           uRegAddr    = HWIO_MDP_WB_2_TRAFFIC_SHAPER_WR_CLIENT_ADDR;
           break;
  
        case HAL_MDP_TRFCTRL_CLIENT_RGB0:
        case HAL_MDP_TRFCTRL_CLIENT_RGB1:
        case HAL_MDP_TRFCTRL_CLIENT_RGB2:
        case HAL_MDP_TRFCTRL_CLIENT_RGB3:
        case HAL_MDP_TRFCTRL_CLIENT_WB0:
        case HAL_MDP_TRFCTRL_CLIENT_WB1:
        case HAL_MDP_TRFCTRL_CLIENT_NONE:
        case HAL_MDP_TRFCTRL_CLIENT_MAX:
        case HAL_MDP_TRFCTRL_CLIENT_FORCE_32BIT:
        default:
           eStatus = HAL_MDSS_STATUS_FAILED_INVALID_INPUT_PARAMETER;
           break;         
        }
  
        
        if (HAL_MDSS_STATUS_SUCCESS == eStatus)
        {
          /* configures the traffice shaper for the client */
          if(NULL!= psSetting->psTrfShaperSetting)
          {
            HAL_MDP_TrafficCtrl_SetTrfShaper(psSetting->psTrfShaperSetting->bEnableTrfShaper, 
                                             psSetting->psTrfShaperSetting->uBytesPerClk, 
                                             uRegAddr);
          }
          psSetting++;
        }
        else
        {
           break;          /* Exit on ERROR! */
        }
     }

     return eStatus;

}

/****************************************************************************
*
** FUNCTION: HAL_MDP_TrafficCtrl_Init()
*/
/*!
* \brief
*     Initializes QoS priority re-mapping and watermark level settings for  
*     read clients, the output of QoS priority level for write paths, and 
*     the traffic shaper. 
*
* \param [in] uFlags - reserved flags
*
* \retval HAL_MDSS_ErrorType
*
****************************************************************************/
HAL_MDSS_ErrorType HAL_MDP_TrafficCtrl_Init(HAL_MDP_InitConfigType *pMdpInitConfig, uint32  uFlags)
{
  HAL_MDSS_ErrorType               eStatus     = HAL_MDSS_STATUS_SUCCESS;
  
  /* init all watermark registers and traffic shapers */
  HAL_MDP_TrafficCtrl_InitClients(HAL_MDP_TRFCTRL_CLIENT_VIG0, pgsMdpHwInfo->sHwInfo.uNumOfVIGLayers);
  HAL_MDP_TrafficCtrl_InitClients(HAL_MDP_TRFCTRL_CLIENT_DMA0, pgsMdpHwInfo->sHwInfo.uNumOfDMALayers);
  HAL_MDP_TrafficCtrl_InitClients(HAL_MDP_TRFCTRL_CLIENT_WB2, pgsMdpHwInfo->sHwInfo.uNumOfBltEngines);
  return eStatus;
}

/****************************************************************************
*
** FUNCTION: HAL_MDP_TrafficCtrl_SetClientWatermarkLvl_5_0_0()
*/
/*!
* \brief
*     Sets a read client's watermark levels based on client's latency need.
*
* \param [in] eLatency             - latency setting for the client; 
* \param [in] uTotalSMPBytes       - total number of bytes in SMP allocated for the client;
* \param [in] uFlags               - If configured surface is MacroTiled;
* \param [in] uRegAddr             - address of watermark level-0 register;
*
* \retval HAL_MDSS_ErrorType
*
****************************************************************************/
HAL_MDSS_ErrorType HAL_MDP_TrafficCtrl_SetClientWatermarkLvl_5_0_0 (
                              HAL_MDP_TrafficCtrl_LatencyType  eLatency,
                              uint32                           uTotalSMPBytes,
                              uint32                           uFlags,
                              uintPtr                          uRegAddr )
{
   return HAL_MDSS_STATUS_FAILED_NOT_SUPPORTED;
}

#ifdef __cplusplus
}
#endif
