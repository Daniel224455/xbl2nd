/*! \file */

/*
===========================================================================

FILE:         hal_mdp_pingpong.c

DESCRIPTION: Manages the MDP (Display) Pingpong blocks

===========================================================================
  Copyright (c) 2012-2017 Qualcomm Technologies, Inc.  All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential.
===========================================================================
*/

#include "hal_mdp_i.h"
#include "hal_mdp_osal.h"

#ifdef __cplusplus
extern "C" {
#endif


/****************************************************************************
*
** FUNCTION: HAL_MDP_Get_PingPong_BaseOffset()
*/
/*!
* \brief
*     Returns offset of the pingpong block
*
* \param [in] ePingPongId    - pingpong block ID (none, 0, 1, 2)
*
* \retval uintPtr
*
****************************************************************************/
static uintPtr HAL_MDP_Get_PingPong_BaseOffset(HAL_MDP_PingPongId  ePingPongId)
{
   uintPtr   uRegOffset  = 0x0;

   if((HAL_MDP_PINGPONG_NONE        == ePingPongId) ||
      (HAL_MDP_PINGPONG_MAX         <= ePingPongId))
   {
      HAL_MDP_ASSERT();
   }
   else
   {
      uRegOffset = uMDPPingPongRegBaseOffset[ePingPongId];
   }
   return uRegOffset;
}

/****************************************************************************
*
** FUNCTION: HAL_MDP_PingPong_GetStateInfo()
*/
/*!
* \brief
*     Returns the current state (active/idle) of the given ping-pong id
*
* \param [in]   ePingPongId      - Pingpong block id
* \param [out]  pbActive         - TRUE : Module is active
* \param [in]   uFlags           - Reserved
*
* \retval NONE
*
****************************************************************************/
static void HAL_MDP_PingPong_GetStateInfo(HAL_MDP_PingPongId     ePingPongId,
                                          bool32                *pbActive,
                                          uint32                 uFlags)
{
   uint32                        uRegValue         = 0x00;
   HAL_MDP_ModuleStatusRegInfo  *psModuleRegInfo   = NULL;

   *(pbActive)  = FALSE;  // Initial state

   psModuleRegInfo = (HAL_MDP_ModuleStatusRegInfo  *)pgsMdpHwInfo->gpsModuleStatusRegInfo[HAL_MDP_MODULE_PINGPONG];

   if(NULL != psModuleRegInfo)
   {
      uRegValue   = in_dword(MDSS_REG_BASE + psModuleRegInfo[ePingPongId].uRegAddress);

      if(0x00 != (uRegValue & psModuleRegInfo[ePingPongId].uBitFieldMask) )
      {
         *(pbActive)  = TRUE;        // Module is in active state
      }
   }
}

/****************************************************************************
*
** FUNCTION: HAL_MDP_PingPong_DitherConfig()
*/
/*!
* \brief
*     Set up configurations for ping-pong dither module
*
* \param [in]   ePingPongId       - Ping-Pong ID
* \param [in]   psDitherConfig    - Dither configuration information
*
* \retval NONE
*
****************************************************************************/
static void HAL_MDP_PingPong_DitherConfig(HAL_MDP_PingPongId                 ePingPongId,
                                          HAL_MDP_PingPong_DitherConfigType *psDitherConfig)
{
  uintPtr  uRegisterOffset   = 0;
  uint32   uDsppOpModeInfo   = 0;
  uint32   uDitherBitDepth   = 0;

  // Extract the base address to program the HW registers
  uRegisterOffset  = HAL_MDP_Get_PingPong_BaseOffset(ePingPongId);

  uDsppOpModeInfo  = in_dword(HWIO_MDP_PPB_0_DITHER_OP_MODE_ADDR + uRegisterOffset);

  // Dither is only for RGB destination with lower color depth than RGB888
  if (TRUE == psDitherConfig->bEnable) 
  {

    uDitherBitDepth = HWIO_OUT_FLD(uDitherBitDepth,
      MDP_PPB_0_DITHER_BITDEPTH,
      C0_BITS,
      psDitherConfig->sDitherBitDepth.uColor0);

    uDitherBitDepth = HWIO_OUT_FLD(uDitherBitDepth,
      MDP_PPB_0_DITHER_BITDEPTH,
      C1_BITS,
      psDitherConfig->sDitherBitDepth.uColor1);

    uDitherBitDepth = HWIO_OUT_FLD(uDitherBitDepth,
      MDP_PPB_0_DITHER_BITDEPTH,
      C2_BITS,
      psDitherConfig->sDitherBitDepth.uColor2);

    if (psDitherConfig->eDitherType == HAL_MDP_DITHER_TYPE_TEMPORAL)
    {
      uDitherBitDepth = HWIO_OUT_FLD(uDitherBitDepth,
        MDP_PPB_0_DITHER_BITDEPTH,
        TEMPORAL_DITHER_EN,
        1);
    }
    else
    {
      uDitherBitDepth = HWIO_OUT_FLD(uDitherBitDepth,
        MDP_PPB_0_DITHER_BITDEPTH,
        TEMPORAL_DITHER_EN,
        0);
    }

    out_dword(HWIO_MDP_PPB_0_DITHER_BITDEPTH_ADDR + uRegisterOffset, uDitherBitDepth);

    // Dither matrix
    if (NULL != psDitherConfig->pDitherMatrix)
    {
      out_dword(HWIO_MDP_PPB_0_DITHER_MATRIX_ROW0_ADDR + uRegisterOffset, psDitherConfig->pDitherMatrix->uRow0Value);    // ROW#0
      out_dword(HWIO_MDP_PPB_0_DITHER_MATRIX_ROW1_ADDR + uRegisterOffset, psDitherConfig->pDitherMatrix->uRow1Value);    // ROW#1
      out_dword(HWIO_MDP_PPB_0_DITHER_MATRIX_ROW2_ADDR + uRegisterOffset, psDitherConfig->pDitherMatrix->uRow2Value);    // ROW#2
      out_dword(HWIO_MDP_PPB_0_DITHER_MATRIX_ROW3_ADDR + uRegisterOffset, psDitherConfig->pDitherMatrix->uRow3Value);    // ROW#3
    }

    uDsppOpModeInfo  = HWIO_OUT_FLD(uDsppOpModeInfo,
      MDP_PPB_0_DITHER_OP_MODE,
      DITHER_EN,
      1);
  }
  else
  {
    uDsppOpModeInfo  = HWIO_OUT_FLD(uDsppOpModeInfo,
      MDP_PPB_0_DITHER_OP_MODE,
      DITHER_EN,
      0);
  }

  out_dword(HWIO_MDP_PPB_0_DITHER_OP_MODE_ADDR + uRegisterOffset, uDsppOpModeInfo);
}



/****************************************************************************
*
** FUNCTION: HAL_MDP_PingPong_DSCConfig()
*/
/*!
* \brief
*     Enables/Disables the DSC encoder connected to the given ping-pong id
*
* \param [in]   ePingPongId      - pingpong block id
* \param [in]   psDSConfig       - Configuration parameters for DSC encoder connected to ePingPongId
* \param [in]   uFlags           - Reserved.
*
* \retval HAL_MDSS_ErrorType
*
****************************************************************************/
static void HAL_MDP_PingPong_DSCConfig(HAL_MDP_PingPongId               ePingPongId,
                                       HAL_MDP_PingPong_DSCConfigType  *psDSConfig,
                                       uint32                           uFlags)
{
   uintPtr  uRegOffset  = 0;
   uintPtr  uReg        = 0;

   uReg = HWIO_OUT_FLD(0, MDP_PP_0_DCE_DATA_OUT_SWAP, ENDIAN, (psDSConfig->bEndianess? 1 : 0));

   /* Following are byte mapping will be true always for all targets */
   uReg = HWIO_OUT_FLD(uReg, MDP_PP_0_DCE_DATA_OUT_SWAP, BYTE5, 0x05);
   uReg = HWIO_OUT_FLD(uReg, MDP_PP_0_DCE_DATA_OUT_SWAP, BYTE4, 0x04);
   uReg = HWIO_OUT_FLD(uReg, MDP_PP_0_DCE_DATA_OUT_SWAP, BYTE3, 0x03);
   uReg = HWIO_OUT_FLD(uReg, MDP_PP_0_DCE_DATA_OUT_SWAP, BYTE2, 0x02);
   uReg = HWIO_OUT_FLD(uReg, MDP_PP_0_DCE_DATA_OUT_SWAP, BYTE1, 0x01);
   uReg = HWIO_OUT_FLD(uReg, MDP_PP_0_DCE_DATA_OUT_SWAP, BYTE0, 0x00);

   // Get register base offset
   uRegOffset = HAL_MDP_Get_PingPong_BaseOffset(ePingPongId);

   out_dword(HWIO_MDP_PP_0_DCE_DATA_OUT_SWAP_ADDR + uRegOffset, uReg);
   out_dword(HWIO_MDP_PP_0_DSC_MODE_ADDR + uRegOffset, psDSConfig->bEnableDSCEnc);
}

/****************************************************************************
*
** FUNCTION: HAL_MDP_PingPong_GetInterfaceID()
*/
/*!
* \brief
*     Finds interface id attached to a given ping pong block
*
* \param [in]   ePingPongId      - pingpong block id
* \param [out]  pInterfaceId     - interface id
*
* \retval HAL_MDSS_ErrorType
*
****************************************************************************/
static HAL_MDSS_ErrorType HAL_MDP_PingPong_GetInterfaceID(HAL_MDP_PingPongId ePingPongId, HAL_MDP_InterfaceId *pInterfaceId)
{
   uint32                 uInterfaceID;
   HAL_MDSS_ErrorType     eStatus      = HAL_MDSS_MDP_STATUS_FAILED;

   // Find interface ID
   for (uInterfaceID = HAL_MDP_INTERFACE_0; uInterfaceID <= HAL_MDP_GET_PHYSICAL_INTERFACE_COUNT(); uInterfaceID++) 
   {
       uintPtr              uRegOffset   = uMDPPhyInterfaceRegBaseOffset[uInterfaceID];
       uint32               uRegVal      = in_dword(HWIO_MDP_INTF_0_MUX_ADDR + uRegOffset);
       HAL_MDP_PingPongId   ePPID        = HAL_MDP_PINGPONG_NONE;

       // Read attached ping pong block to interface 
       uint32   uPingPongSel = HWIO_GETVAL(MDP_INTF_0_MUX, uRegVal, SEL); 

       // Map from register value to HAL_MDP_PingPongId
       switch(uPingPongSel)
       {
           case 0x0:
           {   
               ePPID = HAL_MDP_PINGPONG_0;
               break;
           }
           case 0x1:
           {   
               ePPID = HAL_MDP_PINGPONG_1;
               break;
           }
           case 0x2:
           {   
               ePPID = HAL_MDP_PINGPONG_2;
               break;
           }
           case 0x3:
           {   
               ePPID = HAL_MDP_PINGPONG_3;
               break;
           }
           case 0x4:
           {   
               ePPID = HAL_MDP_PINGPONG_4;
               break;
           }
           case 0x5:
           {   
               ePPID = HAL_MDP_PINGPONG_5;
               break;
           }
           default:
              break;
          
       }

       if (ePPID == ePingPongId)
       {
           // If execution reach here, it means valid interface ID found. Terminate the loop
          *pInterfaceId  = uInterfaceID;
           eStatus       = HAL_MDSS_STATUS_SUCCESS;
           break;
       }
   }

   return eStatus;
}

/****************************************************************************
*
** FUNCTION: HAL_MDP_PingPong_Setup()
*/
/*!
* \brief
*     Set up configurations for a particular pingpong block
*
* \param [in]   ePingPongId      - pingpong block id
* \param [in]   pPingPongConfig  - Configuration for the selected pingpong block
* \param [in]   uSetupFlags      - Reserved.
*
* \retval HAL_MDSS_ErrorType
*
****************************************************************************/
HAL_MDSS_ErrorType HAL_MDP_PingPong_Setup(HAL_MDP_PingPongId             ePingPongId,
                                          HAL_MDP_PingPongConfigType     *pPingPongConfig,
                                          uint32                         uSetupFlags)
{
   HAL_MDSS_ErrorType      eStatus  = HAL_MDSS_STATUS_SUCCESS;

   if((ePingPongId >= HAL_MDP_PINGPONG_0) && ((uint32)ePingPongId <= HAL_MDP_GET_PINGPONG_COUNT()))
   {
      // Vsync counter configuration for smart panel, it will be deprecated in future revisions
      if (NULL != pPingPongConfig->pIntrVsyncConfig)
      {
        HAL_MDP_InterfaceConfigType            sInterfaceConfig;
        HAL_MDP_Interface_VsyncConfigType      sIntrVsyncConfig;
        HAL_MDP_InterfaceId                    eInterfaceId      = HAL_MDP_INTERFACE_NONE;
 
        MDSS_OSAL_MEMZERO(&sInterfaceConfig, sizeof(HAL_MDP_InterfaceConfigType));
        MDSS_OSAL_MEMZERO(&sIntrVsyncConfig, sizeof(HAL_MDP_Interface_VsyncConfigType));
 
        // Copy from Pinpong structure to Interface structure
        sIntrVsyncConfig.bEnableAutoRefresh      = pPingPongConfig->pIntrVsyncConfig->bEnableAutoRefresh;
        sIntrVsyncConfig.uAutoRefreshFrameNumDiv = pPingPongConfig->pIntrVsyncConfig->uAutoRefreshFrameNumDiv;
        sIntrVsyncConfig.uVsyncCount             = pPingPongConfig->pIntrVsyncConfig->uVsyncCount;
        sIntrVsyncConfig.uVsyncHeight            = pPingPongConfig->pIntrVsyncConfig->uVsyncHeight;
        sIntrVsyncConfig.uVsyncInitValue         = pPingPongConfig->pIntrVsyncConfig->uVsyncInitValue;
        sIntrVsyncConfig.uVsyncRdPtrIrqLine      = pPingPongConfig->pIntrVsyncConfig->uVsyncRdPtrIrqLine;
        sIntrVsyncConfig.uVsyncWrPtrIrqLine      = pPingPongConfig->pIntrVsyncConfig->uVsyncWrPtrIrqLine;
        
        sInterfaceConfig.pIntrVsyncConfig        = &sIntrVsyncConfig;

        // Find interface id of corresponding pingpong id, and setup interface
        if (HAL_MDSS_STATUS_SUCCESS == HAL_MDP_PingPong_GetInterfaceID(ePingPongId, &eInterfaceId))
        {
          HAL_MDP_Interface_Setup(eInterfaceId, &sInterfaceConfig, 0x00);
        }
      }

      // Tear check configuration for smart panel, it will be deprecated in future revisions
      if (NULL != pPingPongConfig->pIntrTEConfig)
      {
        HAL_MDP_InterfaceConfigType            sInterfaceConfig;
        HAL_MDP_Interface_TEConfigType         sIntrTEConfig;
        HAL_MDP_InterfaceId                    eInterfaceId      = HAL_MDP_INTERFACE_NONE;
        
        MDSS_OSAL_MEMZERO(&sInterfaceConfig, sizeof(HAL_MDP_InterfaceConfigType));
        MDSS_OSAL_MEMZERO(&sIntrTEConfig, sizeof(HAL_MDP_Interface_TEConfigType));
 
        // Copy from Pinpong structure to Interface structure
        sIntrTEConfig.bDedicatedTEPin    = pPingPongConfig->pIntrTEConfig->bDedicatedTEPin;
        sIntrTEConfig.eVsyncSelect       = (HAL_MDP_Interface_VsyncSelectType)(pPingPongConfig->pIntrTEConfig->eVsyncSelect);
        sIntrTEConfig.uContinueThreshold = pPingPongConfig->pIntrTEConfig->uContinueThreshold;
        sIntrTEConfig.uStartPosition     = pPingPongConfig->pIntrTEConfig->uStartPosition;
        sIntrTEConfig.uStartThreshold    = pPingPongConfig->pIntrTEConfig->uStartThreshold;
 
        sInterfaceConfig.pIntrTEConfig   = &sIntrTEConfig;

        // Find interface id of corresponding pingpong id, and setup interface
        if (HAL_MDSS_STATUS_SUCCESS == HAL_MDP_PingPong_GetInterfaceID(ePingPongId, &eInterfaceId))
        {
          HAL_MDP_Interface_Setup(eInterfaceId, &sInterfaceConfig, 0x00);
        }
      }

      // Tear check enable for smart panel, it will be deprecated in future revisions
      if (NULL != pPingPongConfig->pIntrTEEnable)
      {
        HAL_MDP_InterfaceConfigType            sInterfaceConfig;
        HAL_MDP_Interface_TEEnableType         sIntrTEEnable; 
        HAL_MDP_InterfaceId                    eInterfaceId     = HAL_MDP_INTERFACE_NONE;
        
        MDSS_OSAL_MEMZERO(&sInterfaceConfig, sizeof(HAL_MDP_InterfaceConfigType));
        MDSS_OSAL_MEMZERO(&sIntrTEEnable, sizeof(HAL_MDP_Interface_TEEnableType));

        // Copy from Pinpong structure to Interface structure
        sIntrTEEnable.bEnable          = pPingPongConfig->pIntrTEEnable->bEnable;

        sInterfaceConfig.pIntrTEEnable = &sIntrTEEnable;

        // Find interface id of corresponding pingpong id, and setup interface
        if (HAL_MDSS_STATUS_SUCCESS == HAL_MDP_PingPong_GetInterfaceID(ePingPongId, &eInterfaceId))
        {
          HAL_MDP_Interface_Setup(eInterfaceId, &sInterfaceConfig, 0x00);
        }
      }

      // Vsync counter enable for smart panel, it will be deprecated in future revisions 
      if (NULL != pPingPongConfig->pIntrVsyncEnable)
      {
        HAL_MDP_InterfaceConfigType            sInterfaceConfig;  
        HAL_MDP_Interface_VsyncEnableType      sIntrVsyncEnable;
        HAL_MDP_InterfaceId                    eInterfaceId      = HAL_MDP_INTERFACE_NONE;

        MDSS_OSAL_MEMZERO(&sInterfaceConfig, sizeof(HAL_MDP_InterfaceConfigType));
        MDSS_OSAL_MEMZERO(&sIntrVsyncEnable, sizeof(HAL_MDP_Interface_VsyncEnableType));

        // Copy from Pinpong structure to Interface structure
        sIntrVsyncEnable.bEnable          = pPingPongConfig->pIntrVsyncEnable->bEnable;

        sInterfaceConfig.pIntrVsyncEnable = &sIntrVsyncEnable;

        // Find interface id of corresponding pingpong id, and setup interface
        if (HAL_MDSS_STATUS_SUCCESS == HAL_MDP_PingPong_GetInterfaceID(ePingPongId, &eInterfaceId))
        {
          HAL_MDP_Interface_Setup(eInterfaceId, &sInterfaceConfig, 0x00);
        }
      }

      // Auto-refresh enable for smart panel, it will be deprecated in future revisions
      if (NULL != pPingPongConfig->pIntrAutoRefreshEnable)
      {
        HAL_MDP_InterfaceConfigType              sInterfaceConfig;   
        HAL_MDP_Interface_AutoRefreshEnableType  sIntrAutoRefreshEnable;
        HAL_MDP_InterfaceId                      eInterfaceId            = HAL_MDP_INTERFACE_NONE;

        MDSS_OSAL_MEMZERO(&sInterfaceConfig, sizeof(HAL_MDP_InterfaceConfigType));
        MDSS_OSAL_MEMZERO(&sIntrAutoRefreshEnable, sizeof(HAL_MDP_Interface_AutoRefreshEnableType));

        // Copy from Pinpong structure to Interface structure        
        sIntrAutoRefreshEnable.bEnable          = pPingPongConfig->pIntrAutoRefreshEnable->bEnable;

        sInterfaceConfig.pIntrAutoRefreshEnable = &sIntrAutoRefreshEnable;

        // Find interface id of corresponding pingpong id, and setup interface
        if (HAL_MDSS_STATUS_SUCCESS == HAL_MDP_PingPong_GetInterfaceID(ePingPongId, &eInterfaceId))
        {
          HAL_MDP_Interface_Setup(eInterfaceId, &sInterfaceConfig, 0x00);
        }
        
      }

      // Display stream compression (DSC)
      if(NULL != pPingPongConfig->pDSCConfig)
      {
         HAL_MDP_PingPong_DSCConfig(ePingPongId, pPingPongConfig->pDSCConfig, 0x00);
      }

      // Dithering
      if(NULL != pPingPongConfig->pDitherConfig)
      {
         HAL_MDP_PingPong_DitherConfig(ePingPongId, pPingPongConfig->pDitherConfig);
      }
      
   }
   else
   {
      eStatus = HAL_MDSS_STATUS_FAILED_INVALID_INPUT_PARAMETER;
   }

   return eStatus;
}

/****************************************************************************
*
** FUNCTION: HAL_MDP_PingPong_GetProperty()
*/
/*!
* \brief
*     Retrieve information for a particular pingpong block
*
* \param [in]   ePingPongId           - pingpong block id
* \param [out]  pPingPongInfoData     - Info data
* \param [in]   uGetPropertyFlags     - Reserved
*
* \retval HAL_MDSS_ErrorType
*
****************************************************************************/
HAL_MDSS_ErrorType HAL_MDP_PingPong_GetProperty(HAL_MDP_PingPongId                   ePingPongId,
                                                HAL_MDP_PingPongInfoParamsType       *pPingPongInfoData,
                                                uint32                               uGetPropertyFlags)
{
   HAL_MDSS_ErrorType   eStatus  = HAL_MDSS_STATUS_SUCCESS;

   // Refresh rate info, it will be deprecated in future revisions
   if (NULL != pPingPongInfoData->pFrameRefreshInfo)
   {
      HAL_MDP_InterfaceInfoParamsType           sInterfaceInfo;
      HAL_MDP_Interface_FrameRefreshInfoType    sFrameRefreshInfo; 
      HAL_MDP_InterfaceId                       eInterfaceId           = HAL_MDP_INTERFACE_NONE;
            
      MDSS_OSAL_MEMZERO(&sInterfaceInfo, sizeof(HAL_MDP_InterfaceInfoParamsType));
      MDSS_OSAL_MEMZERO(&sFrameRefreshInfo, sizeof(HAL_MDP_Interface_FrameRefreshInfoType));

      sInterfaceInfo.pFrameRefreshInfo     = &sFrameRefreshInfo;
      sInterfaceInfo.eInterfaceMode        = HAL_MDP_INTERFACE_MODE_COMMAND;

      // Find interface id of corresponding pingpong id, and query interface
      if (HAL_MDSS_STATUS_SUCCESS == HAL_MDP_PingPong_GetInterfaceID(ePingPongId, &eInterfaceId))
      {
          HAL_MDP_Interface_GetProperty(eInterfaceId, &sInterfaceInfo, 0x00);

          // Copy from Interface structure to Pinpong structure
          pPingPongInfoData->pFrameRefreshInfo->uCurrentFrameCnt = sFrameRefreshInfo.uCurrentFrameCnt;
          pPingPongInfoData->pFrameRefreshInfo->uCurrentLineCnt  = sFrameRefreshInfo.uCurrentLineCnt;
      }
   }

   // Engine status info, it will be deprecated in future revisions
   if (NULL != pPingPongInfoData->pEngineStatus)
   {
       HAL_MDP_InterfaceInfoParamsType         sInterfaceInfo;
       HAL_MDP_Interface_EngineStatusType      sEngineStatus; 
       HAL_MDP_InterfaceId                     eInterfaceId      = HAL_MDP_INTERFACE_NONE;

       MDSS_OSAL_MEMZERO(&sInterfaceInfo, sizeof(HAL_MDP_InterfaceInfoParamsType));
       MDSS_OSAL_MEMZERO(&sEngineStatus, sizeof(HAL_MDP_Interface_EngineStatusType));

       sInterfaceInfo.pEngineStatus         = &sEngineStatus;
       sInterfaceInfo.eInterfaceMode        = HAL_MDP_INTERFACE_MODE_COMMAND;

       // Find interface id of corresponding pingpong id, and query interface
       if (HAL_MDSS_STATUS_SUCCESS == HAL_MDP_PingPong_GetInterfaceID(ePingPongId, &eInterfaceId))
       {
           HAL_MDP_Interface_GetProperty(eInterfaceId, &sInterfaceInfo, 0x00);
           
           // Copy from Interface structure to Pinpong structure
           pPingPongInfoData->pEngineStatus->bIsEngineOn = sEngineStatus.bIsEngineOn;
       }
   }

   // Module active/idle state
   if(NULL != pPingPongInfoData->pbActive)
   {
      HAL_MDP_PingPong_GetStateInfo(ePingPongId, pPingPongInfoData->pbActive, 0x00);
   }

   return eStatus;
}

#ifdef __cplusplus
}
#endif
