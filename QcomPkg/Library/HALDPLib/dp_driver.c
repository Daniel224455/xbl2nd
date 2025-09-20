/*=============================================================================

  File: dpdriver.c

  DP Panel driver

  Copyright (c) 2016-2020 Qualcomm Technologies, Inc.  All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential.
=============================================================================*/


/* -----------------------------------------------------------------------
** Includes
** ----------------------------------------------------------------------- */
#include "MDPLib_i.h"
#include "dp_i.h"
#include "dp_driver.h"
#include "DPInterface.h"
#include "dp_dsc.h"
#include "edid.h"

/* -----------------------------------------------------------------------
** Structure Types 
** ----------------------------------------------------------------------- */
/*!
* \b DPModeType
*
*  Mode information for DP
*/
typedef struct
{
  uint32 uIdxEDIDModeList;  /**< Index for this mode in sEDIDModeList which has all the info */
} DPModeType;

/*!
* \b DPDriverBondedModeConfigType
*
*  Internal structure for holding DP bonded mode configuration info
*/
typedef struct
{
  bool32          bEnabled;              /**< If display is operating in bonded mode */
  DP_DeviceIDType eSourceClockDeviceID;  /**< Device ID of the DP device providing the shared source clock */   
  uint32          uNumDevices;           /**< Number of DP devices driven in bonded mode */
  bool32          bExternalMasterDevice; /**< Master DP Device is not one of the DP devices driving the display */
} DPDriverBondedModeConfigType;

/*!
* \b DPDriverConfigType
*
*  Internal structure for holding DP command data
*/
typedef struct {
  MDP_Display_IDType              eDisplayId;                         /**< Physical display ID  */
  DP_HandleType                   hDPHandle[MDP_MAX_NUM_DP_DEVICES];  /**< DP device handle for DP Host driver */
  uint32                          uLastHostErrorCode;                 /**< DP last host error code for packet to peripheral */
  uint32                          uLastPanelErrorCode;                /**< DP last panel error code for packet from peripheral */
  uint32                          uCmdBufSize;                        /**< DP command buffer size */
  DPModeType                      sMode[DP_DRIVER_MAX_MODES];         /**< Mode info */
  uint32                          uTotalModes;                        /**< Total number of supported modes */
  DPDriverBondedModeConfigType    sBondedModeConfig;                  /**< DP device bonded mode configuration info */
} DPDriverConfigType;

/* -----------------------------------------------------------------------
** Global variables
** ----------------------------------------------------------------------- */
DPDriverConfigType gDPDriverConfig[DP_DEVICE_NUM];

/* -----------------------------------------------------------------------
** Macro defines
** ----------------------------------------------------------------------- */
#define DPDRIVER_ARRAY_SIZE(arr)                 (sizeof((arr)) / sizeof(arr[0]))
#define DPDRIVER_GETMAX_CONTROLLERS(_dp_cfg_)    (((_dp_cfg_)->sBondedModeConfig.uNumDevices < MDP_MAX_NUM_DP_DEVICES) ? ((_dp_cfg_)->sBondedModeConfig.uNumDevices) : (MDP_MAX_NUM_DP_DEVICES))
#define DP_MODE_TIMING_SCALE_UNITY               100

/* -----------------------------------------------------------------------
** Local Prototypes
** ----------------------------------------------------------------------- */
static void GetPanelAttributesFromPanelInfo(MDP_Panel_AttrType *pPanelAttr, 
                                            DP_PanelInfoType   *pPanelInfo);

static void ScaleModeTimings(DP_PanelInfoType *pPanelInfo,
                             uint32            uScale);

static void GetPanelInfoFromPanelAttributes(DP_PanelInfoType    *pPanelInfo,
                                            MDP_Panel_AttrType  *pPanelAttr);

static bool32 IsAltModeDevice(DP_DeviceIDType eDeviceID);

/* -----------------------------------------------------------------------
** Local functions
** ----------------------------------------------------------------------- */

/* ---------------------------------------------------------------------- */
/**
** FUNCTION: GetDPDeviceIDFromDeviceIDMapping()
**
** DESCRIPTION:
**   Get the DP_DeviceIDType from the DP Device ID Mapping.
**
*//* -------------------------------------------------------------------- */
static DP_DeviceIDType GetDPDeviceIDFromDeviceIDMapping(uint32 uDeviceIDMapping)
{
  DP_DeviceIDType eDeviceID = DP_DEVICE_ID_NONE;
  
  switch (uDeviceIDMapping)
  {
  case MDP_DRIVER_DP_DEVICE_ID_ALT_MODE_0:
    eDeviceID = DP_DEVICE_ID_ALT_MODE_0;
    break;
  case MDP_DRIVER_DP_DEVICE_ID_ALT_MODE_1:
    eDeviceID = DP_DEVICE_ID_ALT_MODE_1;
    break;
  case MDP_DRIVER_DP_DEVICE_ID_DP:
    eDeviceID = DP_DEVICE_ID_DP;
    break;
  case MDP_DRIVER_DP_DEVICE_ID_EDP:
    eDeviceID = DP_DEVICE_ID_EDP;
    break;
  default:
    break;
  }

  return eDeviceID;
}

/* ---------------------------------------------------------------------- */
/**
** FUNCTION: GetDPDeviceID()
**
** DESCRIPTION:
**   Map MDP_Display_IDType to DP_DeviceIDType
**
*//* -------------------------------------------------------------------- */
static DP_DeviceIDType GetDPDeviceID(MDP_Display_IDType eDisplayId,
                                     uint32             uDeviceIndex)
{
  DP_DeviceIDType         eDeviceID  = DP_DEVICE_ID_NONE;
  MDP_Panel_AttrType     *pPanelAttr = MDP_GET_DISPLAYINFO(eDisplayId);

  if (TRUE == pPanelAttr->uAttrs.sDp.sBondedModeConfig.bEnabled)
  {
    if (uDeviceIndex < DPDRIVER_ARRAY_SIZE(pPanelAttr->uAttrs.sDp.sBondedModeConfig.uDeviceIDMapping))
    {
      eDeviceID = GetDPDeviceIDFromDeviceIDMapping(pPanelAttr->uAttrs.sDp.sBondedModeConfig.uDeviceIDMapping[uDeviceIndex]);
    }
  }
  else
  {
    switch (eDisplayId)
    {
    case MDP_DISPLAY_PRIMARY:
      eDeviceID = DP_DEVICE_ID_EDP;        /* eDP --> intf_5 --> edp_Phy */
      break;
    case MDP_DISPLAY_EXTERNAL:             /* DP0 --> intf_0 --> dpPhy2, mini DP  */
      eDeviceID = DP_DEVICE_ID_DP;
      break;
    case MDP_DISPLAY_EXTERNAL2:
      eDeviceID = DP_DEVICE_ID_ALT_MODE_0; /* DP0 --> intf_0 --> USB3_DP_PHY, DP over Type_c port 0 */
      break;
    case MDP_DISPLAY_EXTERNAL3:
      eDeviceID = DP_DEVICE_ID_ALT_MODE_1; /* DP1 --> intf_4 --> USB3_DP_PHY_SEC, DP over Type_c port 0 */
      break;
    default:
      break;
    }
  }

  return eDeviceID;
}

/* ---------------------------------------------------------------------- */
/**
** FUNCTION: GetDPDriverContext()
**
** DESCRIPTION:
**   Get local context for DP Driver
**
*//* -------------------------------------------------------------------- */
static DPDriverConfigType* GetDPDriverContext(MDP_Display_IDType eDisplayId)
{
  DPDriverConfigType *pContext  = NULL;
  DP_DeviceIDType     eDeviceID = GetDPDeviceID(eDisplayId, 0);

  if (1 == DP_DEVICE_NUM)
  {
     /* single device context means eDP only use only once device  context */
     pContext = &gDPDriverConfig[0];
  }
  else
  {
     if (DP_DEVICE_ID_NONE < eDeviceID &&
         DP_DEVICE_MAX     > eDeviceID)
     {
       pContext = &gDPDriverConfig[eDeviceID - 1];
     }
  }

  return pContext;
}

/* ----------------------------------------------------------------------
** FUNCTION: SetupOpenConfig()
**
** DESCRIPTION:
**    Setup the config for DP_Host_Open
**
-------------------------------------------------------------------- */
static void SetupOpenConfig(DPDriverConfigType    *pDPConfig,
                            MDP_Panel_AttrType    *pPanelConfig,
                            DP_HostOpenConfigType *pOpenConfig,
                            uint32                 uFlags,
                            uint32                 uDeviceIndex)
{
  if ((NULL == pDPConfig)    ||
      (NULL == pPanelConfig) || 
      (NULL == pOpenConfig))
  {
    DP_LOG_MESSAGE_ERROR("SetupOpenConfig: Invalid input ");
  }
  else
  {
    // Initialize the DP open context data
    MDP_OSAL_MEMZERO(pOpenConfig, sizeof(DP_HostOpenConfigType));

    pOpenConfig->bReadDPCD           = TRUE;
    pOpenConfig->bReadEDID           = TRUE;
    pOpenConfig->uLaneSwingLevel     = DPCD_SPEC_MIN_VOLTAGE_SWING_LEVEL;
    pOpenConfig->uPreemphasisLevel   = DPCD_SPEC_MIN_PRE_EMPHASIS_LEVEL;
    pOpenConfig->bEnableSSCMode      = FALSE;

    if (uFlags & DPINTERFACE_PLUG_INVERTED)    // USB type-C plug orientation
    {
      pOpenConfig->bInverted      = TRUE;
    }
    else
    {
      pOpenConfig->bInverted      = FALSE;
    }

    pOpenConfig->bSkipHotPlugDetect  = TRUE;    // Skip GPIO based HPD

    // Save DP displayID 
    pOpenConfig->eDeviceID = GetDPDeviceID(pDPConfig->eDisplayId, uDeviceIndex);

    // Setup bonded mode configuration
    pOpenConfig->sBondedModeConfig.bEnabled = pDPConfig->sBondedModeConfig.bEnabled;

    if (TRUE == pOpenConfig->sBondedModeConfig.bEnabled) 
    {
      pOpenConfig->sBondedModeConfig.eSourceClockDeviceID = pDPConfig->sBondedModeConfig.eSourceClockDeviceID;

      if (TRUE == IsAltModeDevice(pOpenConfig->eDeviceID))
      {
        // Fixed lane configuration for Alt-mode devices in bonded mode
        pOpenConfig->eConfigFlags |= DP_CONFIG_FLAG_ALT_MODE_FIXED_LANE_CONFIGURATION;
        pOpenConfig->bInverted     = FALSE;
        // Perform reset of DP/USB3 combo PHY
        pOpenConfig->eConfigFlags |= DP_CONFIG_FLAG_ALT_MODE_COMMON_RESET;
      }
    }
  }
}


/* ---------------------------------------------------------------------- */
/**
** FUNCTION: DPDriver_Open()
**
** DESCRIPTION:
**   Initialize DP panel to use display.
**
*//* -------------------------------------------------------------------- */
MDP_Status DPDriver_Open(MDP_Display_IDType eDisplayId,
                         uint32             uFlags)
{
  MDP_Status               eStatus          = MDP_STATUS_OK;
  DP_HostOpenConfigType    sOpenConfig      = { 0 };
  DPDriverConfigType      *pDPConfig        = NULL;
  MDP_Panel_AttrType      *pPanelAttr       = MDP_GET_DISPLAYINFO(eDisplayId);
  uint32                   uDeviceIndex;

  if (NULL == (pDPConfig = GetDPDriverContext(eDisplayId)))
  {
    eStatus = MDP_STATUS_NOT_SUPPORTED;
    DP_LOG_MESSAGE_ERROR("DPDrv%i: DPDriver_Open: Invalid display ID\n", eDisplayId);
  }
  else
  {
    pDPConfig->eDisplayId = eDisplayId;

    // Populate the DP driver config with the bonded mode config from the panel attributes
    if (TRUE == pPanelAttr->uAttrs.sDp.sBondedModeConfig.bEnabled)
    {
      pDPConfig->sBondedModeConfig.bEnabled              = TRUE;
      pDPConfig->sBondedModeConfig.uNumDevices           = pPanelAttr->uAttrs.sDp.sBondedModeConfig.uNumDevices;
      pDPConfig->sBondedModeConfig.eSourceClockDeviceID  = GetDPDeviceIDFromDeviceIDMapping(pPanelAttr->uAttrs.sDp.sBondedModeConfig.uMasterDeviceID);
      pDPConfig->sBondedModeConfig.bExternalMasterDevice = pPanelAttr->uAttrs.sDp.sBondedModeConfig.bExternalMasterDevice;
    }
    else
    {
      pDPConfig->sBondedModeConfig.bEnabled              = FALSE;
      pDPConfig->sBondedModeConfig.uNumDevices           = 1;
      pDPConfig->sBondedModeConfig.eSourceClockDeviceID  = DP_DEVICE_ID_NONE;
      pDPConfig->sBondedModeConfig.bExternalMasterDevice = FALSE;
    }

    for (uDeviceIndex = 0; uDeviceIndex < DPDRIVER_GETMAX_CONTROLLERS(pDPConfig); uDeviceIndex++)
    {
      SetupOpenConfig(pDPConfig, pPanelAttr, &sOpenConfig, uFlags, uDeviceIndex);

      // Initialize the host side 
      if (DP_STATUS_SUCCESS != DP_Host_Open(&pDPConfig->hDPHandle[uDeviceIndex], &sOpenConfig))
      {
        eStatus = MDP_STATUS_FAILED;
        DP_LOG_MESSAGE_ERROR("DPDrv%i: DPDriver_Open: DP_Host_Open failed, uDeviceIndex =%d, eStatus =%d\n", eDisplayId, uDeviceIndex, eStatus);
      }
      else if (DP_STATUS_SUCCESS != DP_Host_QueryPanelInfo(pDPConfig->hDPHandle[uDeviceIndex]))
      {
        DP_Host_Close(pDPConfig->hDPHandle[uDeviceIndex]);
        eStatus = MDP_STATUS_FAILED;
        DP_LOG_MESSAGE_ERROR("DPDrv%i: DPDriver_Open: DP_Host_QueryPanelInfo failed, uDeviceIndex =%d, eStatus =%d\n", eDisplayId, uDeviceIndex, eStatus);
      }
    }

    // If master DP device is not one of the DP devices opened above and is an alt-mode device, perform a common (DP & USB3) PHY reset
    if ((TRUE == pDPConfig->sBondedModeConfig.bEnabled) && 
        (TRUE == pDPConfig->sBondedModeConfig.bExternalMasterDevice) &&
        (TRUE == IsAltModeDevice(pDPConfig->sBondedModeConfig.eSourceClockDeviceID)))
    {
      DP_SetPropertyParamsType sPropertyData;
      
      DP_OSAL_MemSet(&sPropertyData, 0x00, sizeof(DP_SetPropertyParamsType));
      sPropertyData.sPHYResetInfo.eDeviceID      = pDPConfig->sBondedModeConfig.eSourceClockDeviceID;
      sPropertyData.sPHYResetInfo.ePHYResetFlag |= DP_PHY_RESET_ALT_MODE_COMMON_RESET;
      
      if (DP_STATUS_SUCCESS != (DP_Host_SetProperty(pDPConfig->hDPHandle[0], DP_SETPROPERTY_RESET_PHY , &sPropertyData)))
      {
        eStatus = MDP_STATUS_FAILED;
        DP_LOG_MESSAGE_ERROR("DPDrv%i: DPDriver_Open: failed to set bonded mode config, uDeviceIndex =%d, Status =%d", eDisplayId, uDeviceIndex, eStatus);
      }
    }
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
  DPDriverConfigType         *pDPConfig  = GetDPDriverContext(eDisplayId);

  if ((NULL              != pDPConfig) &&
      (DP_STATUS_SUCCESS == DP_Host_GetProperty(pDPConfig->hDPHandle[0], DP_GETPROPERTY_CONNECTION_STATUS, &sPropData)))
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
  DPDriverConfigType      *pDPConfig = GetDPDriverContext(eDisplayId);
  uint32                   uDeviceIndex;

  // Close DP host driver
  if (NULL != pDPConfig)
  {
    for (uDeviceIndex = 0; uDeviceIndex < DPDRIVER_GETMAX_CONTROLLERS(pDPConfig); uDeviceIndex++)
    {
      if (DP_STATUS_SUCCESS != DP_Host_Close(pDPConfig->hDPHandle[uDeviceIndex]))
      {
        eStatus = MDP_STATUS_FAILED;
        DP_LOG_MESSAGE_ERROR("DPDrv%i: DPDriver_Term: DP_Host_Close failed, uDeviceIndex =%d, eStatus =%d\n", eDisplayId, uDeviceIndex, eStatus);
      }
    }

    DP_OSAL_MemSet(pDPConfig, 0x00, sizeof(DPDriverConfigType));
  }

  return eStatus;
}

/* ---------------------------------------------------------------------- */
/**
** FUNCTION: MDP_Status DPDriver_SetupDSCProperty()
**
** DESCRIPTION:
**   Fill up DSCDescType struct and pass it to DSC calculator module
**
*//* -------------------------------------------------------------------- */
static MDP_Status DPDriver_SetupDSCProperty(MDP_Panel_AttrType *pPanelInfo,
                                            DP_SinkDSCCapType  *pSinkDSCCap)
{
  if ((NULL == pPanelInfo) || 
      (NULL == pSinkDSCCap))
  {
    DP_LOG_MESSAGE_WARN("DPDriver_SetupDSCProperty: Invalid input parameter ID\n");
  }
  else
  {
    DSCDescType   *pDSCDesc;
  
    pDSCDesc = &pPanelInfo->sDSCDesc;
    MDP_OSAL_MEMZERO(pDSCDesc, sizeof(DSCDescType));
  
    MDP_OSAL_MEMZERO(pSinkDSCCap->pPPSBuf, MDP_DSI_DSC_PPS_TOTAL_PACKET_SIZE);
  
    pDSCDesc->bDSCEnable           = TRUE;
    pDSCDesc->uPixelPerClock       = pSinkDSCCap->uPixelPerClock;
    pDSCDesc->bDTOEnable           = pSinkDSCCap->bDTOEnable;
    pDSCDesc->uDSCMajor            = 1;   
    pDSCDesc->uDSCMinor            = 1;    /* host only support minor = 1 */
    pDSCDesc->uDSCScr              = 0;


    if (pPanelInfo->uDisplayWidth >= 2560)    /* wqxga */
    {
       pDSCDesc->uDSCProfileID       = 7;    /* 2 pipe --> 2 mixer --> 2 dsc --> 1 intf */
    }
    else
    {
       pDSCDesc->uDSCProfileID       = 4;    /* 1 pipe --> 1 mixer --> 1 dsc --> 1 intf */
    }
    pDSCDesc->uDSCBpc                = gDscProfileModes[pDSCDesc->uDSCProfileID].uBitsPerComponent;
    pDSCDesc->uDSCBpp                = gDscProfileModes[pDSCDesc->uDSCProfileID].uBitsPerPixel;
    pDSCDesc->uDSCCompressionRatio   = gDscProfileModes[pDSCDesc->uDSCProfileID].uCompressionRatio;
    pDSCDesc->bDSCLayerMixSplit      = gDscProfileModes[pDSCDesc->uDSCProfileID].bLMSplitEnable;
    pDSCDesc->uDSCEncodersNum        = gDscProfileModes[pDSCDesc->uDSCProfileID].uEncodersNum;

    pDSCDesc->bDSCBlockPred          = pSinkDSCCap->bBlockPrediction;
    pDSCDesc->uDSCSliceHeight        = pSinkDSCCap->uSliceHeight;
    pDSCDesc->uDSCSliceWidth         = pSinkDSCCap->uSliceWidth;
    pDSCDesc->uDSCSlicePerPacket     = pSinkDSCCap->uSlicePerLine;

    pDSCDesc->pDSCEncCfg             = &pPanelInfo->uAttrs.sDp.sDSCEncCfg;
    pDSCDesc->pDSCEncParams          = &pPanelInfo->uAttrs.sDp.sDSCEncParams;
    pDSCDesc->pDSCEncPktType         = &pPanelInfo->uAttrs.sDp.sDSCEncPktType;

    /* reserved PPS buf header */
    pDSCDesc->pDSCPpsBuffer          = pSinkDSCCap->pPPSBuf;

    DP_LOG_MESSAGE_INFO("DPDrv%i: DPDriver_SetupDSCProperty: sw=%d sh=%d sp=%d pred=%d bpp=%d bpc=%d\n",
                         pPanelInfo->eDisplayId,
                         pDSCDesc->uDSCSliceWidth,
                         pDSCDesc->uDSCSliceHeight,
                         pDSCDesc->uDSCSlicePerPacket,
                         pDSCDesc->bDSCBlockPred,
                         pDSCDesc->uDSCBpc,
                         pDSCDesc->uDSCBpp);

    MDPCalculateDSCParameters(pPanelInfo);

    /* keep useful results */
    pSinkDSCCap->ubpc              = pDSCDesc->uDSCBpc;
    pSinkDSCCap->ubpp              = pDSCDesc->uDSCBpp;
    pSinkDSCCap->uSliceWidth       = pDSCDesc->uDSCSliceWidth; 
    pSinkDSCCap->uSliceHeight      = pDSCDesc->uDSCSliceHeight; 
    pSinkDSCCap->uCompressionRatio = pDSCDesc->uDSCCompressionRatio;
    pSinkDSCCap->uChunkSize        = pDSCDesc->pDSCEncParams->uChunkSize;
  }

   return MDP_STATUS_OK;
}

/* ----------------------------------------------------------------------
** FUNCTION: DPDriver_SetMode()
**
** DESCRIPTION:
**    Set a panel mode
**
-------------------------------------------------------------------- */
MDP_Status DPDriver_SetMode(MDP_Panel_AttrType *pPanelInfo,
                            uint32              uFlags)
{
  DP_Status                eStatus       = DP_STATUS_SUCCESS;
  MDP_Status               eRetStatus    = MDP_STATUS_OK;
  DPDriverConfigType      *pDPConfig     = NULL;
  DP_SetPropertyParamsType sPropertyData;
  DP_GetPropertyParamsType sGetPropertyData;
  DP_SinkDSCCapType        sSinkDSCCap;

  if ((NULL == pPanelInfo) ||
      (NULL == (pDPConfig = GetDPDriverContext(pPanelInfo->eDisplayId))))
  {
    DP_LOG_MESSAGE_ERROR("DPDriver_SetMode: Invalid input \n");
    eRetStatus = MDP_STATUS_BAD_PARAM;
  }
  else
  {
    uint32 uLinkRate   = 0;      // Default: Get from DPCD
    uint32 uNumLanes   = 0;      // Default: Calculate as needed
    uint32 uModeIndex  = 0;      // Default: First mode 
    uint32 uDeviceIndex;
   
    /* If we are not trying to force the mode from the driver supported list or reading from xml */
    if ((pPanelInfo->eDisplayFlags & MDP_PANEL_FLAG_TIMING_FROM_EDID) && 
        (FALSE == pPanelInfo->bForceMode))
    {
      /* timing from panel edid */
       uModeIndex = pPanelInfo->uModeId;
    }
    else
    {
      /* timing from panel xml file */
      if (0 != pPanelInfo->uAttrs.sDp.uLinkRateInKhz)
      {
        uLinkRate = pPanelInfo->uAttrs.sDp.uLinkRateInKhz;
      }

      /* Override number of lanes  */
      if (0 != pPanelInfo->uAttrs.sDp.uNumberOfLanes)
      {
        uNumLanes = pPanelInfo->uAttrs.sDp.uNumberOfLanes;
      }

      if (NULL != pPanelInfo)
      {
        /* Get timing information from XML */
        DP_OSAL_MemSet(&sPropertyData, 0x00, sizeof(DP_SetPropertyParamsType));
        GetPanelInfoFromPanelAttributes(&sPropertyData.sPanelInfo, pPanelInfo);
        
        if (TRUE == pDPConfig->sBondedModeConfig.bEnabled)
        {
          /* Timing info in panel attributes represents display with all bonded devices combined.
           * Timings need to be scaled down per DP device at the DP Host level. */
          ScaleModeTimings(&sPropertyData.sPanelInfo, DP_MODE_TIMING_SCALE_UNITY / pDPConfig->sBondedModeConfig.uNumDevices);
        }

        DP_LOG_MESSAGE_INFO("DPDrv%i: DPDriver_SetMode: ndx=%d pclk=%d w=%d hfp=%d bfp=%d h=%d vfp=%d vbp=%d, frate=%x\n",
                            pPanelInfo->eDisplayId,
                            sPropertyData.sPanelInfo.uModeIndex,
                            sPropertyData.sPanelInfo.uPclkFreq,
                            sPropertyData.sPanelInfo.uVisibleWidthInPixels,
                            sPropertyData.sPanelInfo.uHsyncFrontPorchInPixels,
                            sPropertyData.sPanelInfo.uHsyncBackPorchInPixels,
                            sPropertyData.sPanelInfo.uVisibleHeightInPixels,
                            sPropertyData.sPanelInfo.uVsyncFrontPorchInLines,
                            sPropertyData.sPanelInfo.uVsyncBackPorchInLines,
                            sPropertyData.sPanelInfo.uRefreshRate);

        for (uDeviceIndex = 0; uDeviceIndex < DPDRIVER_GETMAX_CONTROLLERS(pDPConfig); uDeviceIndex++)
        {
          // Set the mode
          eStatus = DP_Host_SetProperty(pDPConfig->hDPHandle[uDeviceIndex], DP_SETPROPERTY_SET_PANEL_INFO, &sPropertyData);
          if (DP_STATUS_SUCCESS != eStatus)
          {
            DP_LOG_MESSAGE_WARN("DPDrv%i: DPDriver_SetModeInfo: failed to set panel info, uDeviceIndex =%d, eStatus=%d\n", pPanelInfo->eDisplayId, uDeviceIndex, eStatus);
          }
        }
      }
    }

    for (uDeviceIndex = 0; uDeviceIndex < DPDRIVER_GETMAX_CONTROLLERS(pDPConfig); uDeviceIndex++)
    {
      if (0 != uLinkRate)
      {
        DP_OSAL_MemSet(&sPropertyData, 0x00, sizeof(DP_SetPropertyParamsType));
        sPropertyData.uLinkRate = uLinkRate;
        if(DP_STATUS_SUCCESS != (eStatus = DP_Host_SetProperty(pDPConfig->hDPHandle[uDeviceIndex], DP_SETPROPERTY_SET_LINK_RATE, &sPropertyData)))
        {
          DP_LOG_MESSAGE_WARN("DPDrv%i: DPDriver_SetMode: failed to set link rate, uDeviceIndex =%d, eStatus=%d\n", pPanelInfo->eDisplayId, uDeviceIndex, eStatus);
        }
      }
	  
      if (0 != uNumLanes)
      {
         DP_OSAL_MemSet(&sPropertyData, 0x00, sizeof(DP_SetPropertyParamsType));
         sPropertyData.uLaneNumber = uNumLanes;
         if (DP_STATUS_SUCCESS != (eStatus = DP_Host_SetProperty(pDPConfig->hDPHandle[uDeviceIndex], DP_SETPROPERTY_SET_LANE_NUMBER, &sPropertyData)))
         {
           DP_LOG_MESSAGE_WARN("DPDrv%i: DPDriver_SetMode: failed to set link rate, uDeviceIndex =%d, eStatus=%d\n", pPanelInfo->eDisplayId, uDeviceIndex, eStatus);
         }
      }
	  
      DP_OSAL_MemSet(&sPropertyData, 0x00, sizeof(DP_SetPropertyParamsType));
      sPropertyData.uModeIndex = uModeIndex;
      if (DP_STATUS_SUCCESS != (eStatus = DP_Host_SetProperty(pDPConfig->hDPHandle[uDeviceIndex], DP_SETPROPERTY_SET_PANEL_MODE_INDEX, &sPropertyData)))
      {
        DP_LOG_MESSAGE_ERROR("DPDrv%i: DPDriver_SetMode: failed to set Mode Index, uDeviceIndex =%d, eStatus=%d\n", pPanelInfo->eDisplayId, uDeviceIndex, eStatus);
        eRetStatus = MDP_STATUS_FAILED;
        break;
      }
    }

    if (MDP_STATUS_OK == eRetStatus)
    {
      DP_OSAL_MemSet(&sGetPropertyData, 0x00, sizeof(DP_GetPropertyParamsType));
      DP_OSAL_MemSet(&sSinkDSCCap, 0x00, sizeof(sSinkDSCCap));
      DP_OSAL_MemSet(&sPropertyData, 0x00, sizeof(DP_SetPropertyParamsType));
	  
      /* get panel DSC info */
      sGetPropertyData.pSinkDSCCap = &sSinkDSCCap;
      if (DP_STATUS_SUCCESS != (eStatus = DP_Host_GetProperty(pDPConfig->hDPHandle[0], DP_GETPROPERTY_DSC_SUPPORTED, &sGetPropertyData)))
      {
        eRetStatus = MDP_STATUS_FAILED;
        DP_LOG_MESSAGE_ERROR("DPDrv%i: DPDriver_SetMode: failed to Get DSC Supported Info, eStatus=%d\n", pPanelInfo->eDisplayId, eStatus);
      }
      else if (sSinkDSCCap.bSupported)
      {
        DP_LOG_MESSAGE_INFO("DPDrv%i: DPDriver_SetMode: bDSCSupported=%d m=%d %d bsize=%di slice=%d maxslice=%d ppsBuf=%x\n", 
                            pPanelInfo->eDisplayId,
                            sSinkDSCCap.bSupported,
                            sSinkDSCCap.uMajor,
                            sSinkDSCCap.uMinor,
                            sSinkDSCCap.uRCBufferBlockSize,
                            sSinkDSCCap.uSliceSupported,
                            sSinkDSCCap.uMaxSliceWidth,
                            sSinkDSCCap.pPPSBuf);
	    
        DPDriver_SetupDSCProperty(pPanelInfo, &sSinkDSCCap);
	    
        /* save calculated DSC cap including new pps info  to host dsc database */
        sPropertyData.pSinkDSCCap = &sSinkDSCCap;
        if (DP_STATUS_SUCCESS != (eStatus = DP_Host_SetProperty(pDPConfig->hDPHandle[0], DP_SETPROPERTY_DSC_PPS_INFO, &sPropertyData)))
        {
          eRetStatus = MDP_STATUS_FAILED;
          DP_LOG_MESSAGE_ERROR("DPDrv%i: DPDriver_SetMode: failed to Set DSC Supported Info, eStatus=%d\n", pPanelInfo->eDisplayId, eStatus);
        }
      }
    }

    if (MDP_STATUS_OK == eRetStatus)
    {
      for (uDeviceIndex = 0; uDeviceIndex < DPDRIVER_GETMAX_CONTROLLERS(pDPConfig); uDeviceIndex++)
      {
        if (DP_STATUS_SUCCESS != (eStatus = DP_Host_Commit(pDPConfig->hDPHandle[uDeviceIndex])))
        {
          eRetStatus = MDP_STATUS_FAILED;
          DP_LOG_MESSAGE_ERROR("DPDrv%i: DPDriver_SetMode: failed to commit, uDeviceIndex =%d, eStatus=%d\n", pPanelInfo->eDisplayId, uDeviceIndex, eStatus);
          break;
        }      
      }
    }
  }

  return eRetStatus;
}

/* ----------------------------------------------------------------------
** FUNCTION: DPDriver_GetModeInfo()
**
** DESCRIPTION:
**    Get DP panel mode info.
**
-------------------------------------------------------------------- */
MDP_Status DPDriver_GetModeInfo(MDP_Panel_AttrType *pPanelInfo)
{
  MDP_Status                      eStatus   = MDP_STATUS_OK;
  DPDriverConfigType             *pDPConfig = NULL;

  if ((NULL == pPanelInfo)                                               ||
      (NULL == (pDPConfig = GetDPDriverContext(pPanelInfo->eDisplayId))))
  {
    eStatus = MDP_STATUS_FAILED;
  }
  else
  {
    DP_GetPropertyParamsType       sDPProp;

    DP_OSAL_MemSet(&sDPProp, 0x00, sizeof(DP_GetPropertyParamsType));
    sDPProp.sPanelInfo.uModeIndex = (pPanelInfo->bForceMode) ? 0: pPanelInfo->uModeId;

    // Query the mode
    if (DP_STATUS_SUCCESS == DP_Host_GetProperty(pDPConfig->hDPHandle[0], DP_GETPROPERTY_GET_MODE_INFO, &sDPProp))
    {
      if (pPanelInfo->bForceMode)
      {
        EDID_DispModeAttrType sModeInfo;

        MDP_OSAL_MEMZERO(&sModeInfo, sizeof(EDID_DispModeAttrType));

        if (MDP_STATUS_OK != (eStatus = (MDP_Status)EDID_GetMode_ByIndex(&sModeInfo, pPanelInfo->uModeId)))
        {
          DP_LOG_MESSAGE_ERROR("DPDrv%i: DPDriver_GetModeInfo: failed to get mode information by index\n", pPanelInfo->eDisplayId);
        }
        else
        {
          pPanelInfo->sActiveTiming.uHsyncPulseWidthDclk     = sModeInfo.sActiveTimingInfo.iHsyncPulseWidthDclk;
          pPanelInfo->sActiveTiming.uHsyncFrontPorchDclk     = sModeInfo.sActiveTimingInfo.iHsyncFrontPorchDclk;
          pPanelInfo->sActiveTiming.uHsyncBackPorchDclk      = sModeInfo.sActiveTimingInfo.iHsyncBackPorchDclk;       
          pPanelInfo->sActiveTiming.uHsyncInvertSignal       = sModeInfo.sActiveTimingInfo.bIsHsyncActiveLow;
          pPanelInfo->sActiveTiming.uVsyncPulseWidthLines    = sModeInfo.sActiveTimingInfo.iVsyncPulseWidthLines;
          pPanelInfo->sActiveTiming.uVsyncFrontPorchLines    = sModeInfo.sActiveTimingInfo.iVsyncFrontPorchLines;
          pPanelInfo->sActiveTiming.uVsyncBackPorchLines     = sModeInfo.sActiveTimingInfo.iVsyncBackPorchLines;
          pPanelInfo->sActiveTiming.uVsyncInvertSignal       = sModeInfo.sActiveTimingInfo.bIsVsyncActiveLow;
          pPanelInfo->uRefreshRate                           = sModeInfo.sDisplayModeInfo.uRefreshRate;
          pPanelInfo->uDisplayWidth                          = sModeInfo.sDisplayModeInfo.uWidthPx;
          pPanelInfo->uDisplayHeight                         = sModeInfo.sDisplayModeInfo.uHeightPx;
          pPanelInfo->uPixelRate                             = sModeInfo.sDisplayModeInfo.uPixelRate;
          pPanelInfo->uAttrs.sDp.uRefreshRate                = sModeInfo.sDisplayModeInfo.uRefreshRate;
          pPanelInfo->sActiveTiming.uDataEnInvertSignal      = FALSE;
          pPanelInfo->sActiveTiming.uHsyncSkewDclk           = 0;

          if (DP_PIXEL_FORMAT_RGB_101010_30BPP == sDPProp.sPanelInfo.eColorFormat)
          {
            pPanelInfo->eColorFormat = MDP_PIXEL_FORMAT_RGB_101010_30BPP;
          }
          else if (DP_PIXEL_FORMAT_RGB_666_18BPP == sDPProp.sPanelInfo.eColorFormat)
          {
            pPanelInfo->eColorFormat = MDP_PIXEL_FORMAT_RGB_666_18BPP;
          }
          else
          {
            pPanelInfo->eColorFormat = MDP_PIXEL_FORMAT_RGB_888_24BPP;
          }
        }
      } 
      else
      {
        if (TRUE == pDPConfig->sBondedModeConfig.bEnabled)
        {
          // Timing info in panel attributes to be represented as single display with all bonded devices combined
          ScaleModeTimings(&sDPProp.sPanelInfo, DP_MODE_TIMING_SCALE_UNITY * pDPConfig->sBondedModeConfig.uNumDevices);
        }

        // Populate panel attributes with info recieved from the query
        GetPanelAttributesFromPanelInfo(pPanelInfo, &sDPProp.sPanelInfo);
      }

      DP_LOG_MESSAGE_INFO("DPDrv%i: DPDriver_GetModeInfo:#%d Resolution=%dx%dp%d Pclock=%dHz\n",
                          pPanelInfo->eDisplayId,
                          pPanelInfo->uModeId,
                          pPanelInfo->uDisplayWidth,
                          pPanelInfo->uDisplayHeight,
                          pPanelInfo->uRefreshRate>>16,
                          sDPProp.sPanelInfo.uPclkFreq);
    }
    else
    {
      eStatus = MDP_STATUS_FAILED;
    }
  }

  return eStatus;
}

/* ----------------------------------------------------------------------
** FUNCTION: DPDriver_PSR()
**
** DESCRIPTION:
**    Set a panel mode
**
-------------------------------------------------------------------- */
MDP_Status DPDriver_PSR(MDP_Display_IDType eDisplayId,
                        uint32             uEnable,
                        uint32             uFlags)
{
  MDP_Status                eStatus     = MDP_STATUS_OK;
  DPDriverConfigType        *pDPConfig  = NULL;
  DP_SetPropertyParamsType  sDPProp;

  if (MDP_DISPLAY_PRIMARY == eDisplayId)
  {
     if (NULL == (pDPConfig = GetDPDriverContext(eDisplayId)))
     {
        eStatus = MDP_STATUS_NOT_SUPPORTED;
        DP_LOG_MESSAGE_ERROR("DPDrv%i: DPDriver_PSR: Invalid display ID\n", eDisplayId);
     }
     else
     {
        if (uEnable)
        {
           sDPProp.bEnabled = TRUE;
        }
        else
        {
           sDPProp.bEnabled = FALSE;
        }

        if (DP_STATUS_SUCCESS != DP_Host_SetProperty(pDPConfig->hDPHandle[0], DP_SETPROPERTY_EDP_PSR_ACTION, &sDPProp))
        {
           eStatus = MDP_STATUS_FAILED;
        }
     }
  }
  else
  {
     eStatus = MDP_STATUS_NOT_SUPPORTED;
     DP_LOG_MESSAGE_ERROR("DPDrv%i: DPDriver_PSR: PSR for display is not supported\n", eDisplayId);
  }
  return eStatus;
}

/* ----------------------------------------------------------------------
** FUNCTION: DPDriver_SendComamndSequence()
**
** DESCRIPTION:
**    DP Driver sends meta data to client (panel)
**
-------------------------------------------------------------------- */
MDP_Status DPDriver_SendCommandSequence(MDP_Panel_AttrType  *pPanelInfo,
                                        MdpPanelCommandType  eCommand,
                                        void                *pPayload,
                                        uint32               uFlags)
{
    MDP_Status               eStatus          = MDP_STATUS_NOT_SUPPORTED;
    DPDriverConfigType      *pDPConfig        = GetDPDriverContext(pPanelInfo->eDisplayId);

    if (pDPConfig)
    {
       switch (eCommand)
       {
          case MDP_PANEL_COMMAND_PPS:
             eStatus = DP_Host_SDP_ClientWrite(pDPConfig->hDPHandle[0], DP_PANEL_COMMAND_PPS, pPayload, uFlags);
             break;
          default:
             break;
       }
    }

    return eStatus;
}

/* ----------------------------------------------------------------------
** FUNCTION: DPDriver_GetFrameCRC()
**
** DESCRIPTION:
**    Enable CRC capture or read from CRC registers
**
-------------------------------------------------------------------- */
MDP_Status DPDriver_GetFrameCRC(MDP_Panel_AttrType *pPanelInfo,
                                uint64             *pCRC)
{
    MDP_Status                 eStatus          = MDP_STATUS_BAD_PARAM;
    DPDriverConfigType        *pDPConfig        = GetDPDriverContext(pPanelInfo->eDisplayId);
    DP_GetPropertyParamsType   sParams;

    if (pDPConfig)
    {
          eStatus = (MDP_Status)DP_Host_GetProperty(pDPConfig->hDPHandle[0], DP_GETPROPERTY_FRAME_CRC, &sParams);
          if (eStatus == MDP_STATUS_OK)
          {
             *pCRC = sParams.uCRC;
          }
    }

    return eStatus;
}

/* ----------------------------------------------------------------------
** FUNCTION: DPDriver_BrightnessEnable()
**
** DESCRIPTION:
**    Enable panel brightness through AUX
**
-------------------------------------------------------------------- */
MDP_Status DPDriver_BrightnessEnable(MDP_Display_IDType eDisplayId, bool32 bEnable)
{
  MDP_Status                eStatus     = MDP_STATUS_OK;
  DPDriverConfigType       *pDPConfig   = NULL;

  if (NULL == (pDPConfig = GetDPDriverContext(eDisplayId)))
  {
    eStatus = MDP_STATUS_NOT_SUPPORTED;
    DP_LOG_MESSAGE_ERROR("DPDrv%i: DPDriver_BrightnessEnable: Invalid display\n", eDisplayId);
  }
  else
  {
    DP_SetPropertyParamsType  sBacklightProperty;
    DISP_OSAL_MemZero(&sBacklightProperty, sizeof(DP_SetPropertyParamsType));

    sBacklightProperty.sBrightnessInfo.eBrightnessRequest = bEnable ? DP_BRIGHTNESS_REQUEST_ON : DP_BRIGHTNESS_REQUEST_OFF;

    if (MDP_STATUS_OK != (eStatus = (MDP_Status)DP_Host_SetProperty(pDPConfig->hDPHandle, 
                                                                    DP_SETPROPERTY_BRIGHTNESS_CONTROL, 
                                                                    &sBacklightProperty)))
    {
      DP_LOG_MESSAGE_ERROR("DPDrv%i: DPDriver_BrightnessEnable: Failed for display with status 0x%x \n",
                           eDisplayId, eStatus);
    }
  }
  
  return eStatus;
}

/* ----------------------------------------------------------------------
** FUNCTION: DPDriver_BrightnessLevel()
**
** DESCRIPTION:
**    Set panel brightness to requested level
**
-------------------------------------------------------------------- */
MDP_Status DPDriver_BrightnessLevel(MDP_Display_IDType eDisplayId, float fNewLevel)
{
  MDP_Status               eStatus          = MDP_STATUS_OK;
  DPDriverConfigType      *pDPConfig        = GetDPDriverContext(eDisplayId);

  if (NULL == pDPConfig)
  {
    eStatus = MDP_STATUS_BAD_HANDLE;
  }
  else
  {
    DP_SetPropertyParamsType  sPropertyData;
    DISP_OSAL_MemZero(&sPropertyData, sizeof(DP_SetPropertyParamsType));
  
    // Initialize brightness request to new level
    sPropertyData.sBrightnessInfo.fLevel              = fNewLevel;
    sPropertyData.sBrightnessInfo.eBrightnessRequest  = DP_BRIGHTNESS_REQUEST_LEVEL;

    // Send the request to DP Host
    if (DP_STATUS_SUCCESS != DP_Host_SetProperty(pDPConfig->hDPHandle, DP_SETPROPERTY_BRIGHTNESS_CONTROL, &sPropertyData))
    {
      DP_LOG_MESSAGE_ERROR("DPDrv%i: DPDriver_SetBacklight: Failed with status %d for display\n", eDisplayId, eStatus);
    }
  }
  return eStatus;
}

/* ---------------------------------------------------------------------- */
/**
** FUNCTION: GetPanelInfoFromPanelAttributes()
**
** DESCRIPTION:
**   Populate the DP_PanelInfoType struct with timing info from the 
**   MDP_Panel_AttrType struct.
**
*//* -------------------------------------------------------------------- */
static void GetPanelInfoFromPanelAttributes(DP_PanelInfoType   *pPanelInfo,
                                            MDP_Panel_AttrType *pPanelAttr)
{
  if ((NULL == pPanelInfo) ||
      (NULL == pPanelAttr))
  {
    DP_LOG_MESSAGE_ERROR("GetPanelInfoFromPanelAttributes: Invalid input");
  }
  else
  {
    pPanelInfo->uVisibleWidthInPixels    = pPanelAttr->uDisplayWidth;
    pPanelInfo->uHsyncFrontPorchInPixels = pPanelAttr->sActiveTiming.uHsyncFrontPorchDclk;
    pPanelInfo->uHsyncBackPorchInPixels  = pPanelAttr->sActiveTiming.uHsyncBackPorchDclk;
    pPanelInfo->uHsyncPulseInPixels      = pPanelAttr->sActiveTiming.uHsyncPulseWidthDclk;
    pPanelInfo->uHsyncSkewInPixels       = pPanelAttr->sActiveTiming.uHsyncSkewDclk;
    pPanelInfo->uVisibleHeightInPixels   = pPanelAttr->uDisplayHeight;
    pPanelInfo->uVsyncFrontPorchInLines  = pPanelAttr->sActiveTiming.uVsyncFrontPorchLines;
    pPanelInfo->uVsyncBackPorchInLines   = pPanelAttr->sActiveTiming.uVsyncBackPorchLines;
    pPanelInfo->uVsyncPulseInLines       = pPanelAttr->sActiveTiming.uVsyncPulseWidthLines;
    pPanelInfo->uHLeftBorderInPixels     = pPanelAttr->sActiveTiming.uHLeftBorderDClk;
    pPanelInfo->uHRightBorderInPixels    = pPanelAttr->sActiveTiming.uHRightBorderDClk;
    pPanelInfo->uVTopBorderInLines       = pPanelAttr->sActiveTiming.uVTopBorderLines;
    pPanelInfo->uVBottomBorderInLines    = pPanelAttr->sActiveTiming.uVBottomBorderLines;
    pPanelInfo->bHSyncActiveLow          = pPanelAttr->sActiveTiming.uHsyncInvertSignal;
    pPanelInfo->bVSyncActiveLow          = pPanelAttr->sActiveTiming.uVsyncInvertSignal;
    pPanelInfo->uRefreshRate             = pPanelAttr->uAttrs.sDp.uRefreshRate;
    pPanelInfo->uPclkFreq                = (uint32)(((uint64)pPanelAttr->uDisplayWidth +
                                           pPanelAttr->sActiveTiming.uHsyncFrontPorchDclk +
                                           pPanelAttr->sActiveTiming.uHsyncBackPorchDclk +
                                           pPanelAttr->sActiveTiming.uHsyncPulseWidthDclk) *
                                           (uint64) ((uint64)pPanelAttr->uDisplayHeight +
                                           pPanelAttr->sActiveTiming.uVsyncFrontPorchLines +
                                           pPanelAttr->sActiveTiming.uVsyncBackPorchLines +
                                           pPanelAttr->sActiveTiming.uVsyncPulseWidthLines) *
                                           pPanelAttr->uAttrs.sDp.uRefreshRate / 0x10000);
    pPanelInfo->bInterlaced              = pPanelAttr->sActiveTiming.bInterlaced;

    if (MDP_PIXEL_FORMAT_RGB_101010_30BPP == pPanelAttr->eColorFormat)
    {
      pPanelInfo->eColorFormat = DP_PIXEL_FORMAT_RGB_101010_30BPP;
    }
    else if (MDP_PIXEL_FORMAT_RGB_666_18BPP == pPanelAttr->eColorFormat)
    {
      pPanelInfo->eColorFormat = DP_PIXEL_FORMAT_RGB_666_18BPP;
    }
    else
    {
      pPanelInfo->eColorFormat = DP_PIXEL_FORMAT_RGB_888_24BPP;
    }
  }
}

/* ---------------------------------------------------------------------- */
/**
** FUNCTION: ScaleModeTimings()
**
** DESCRIPTION:
**   Upscales/downscales timings for tiled/bonded mode DP configurations.
**
*//* -------------------------------------------------------------------- */
static void ScaleModeTimings(DP_PanelInfoType *pPanelInfo,
                             uint32            uScale)
{
  if (NULL != pPanelInfo)
  {
    pPanelInfo->uPclkFreq                = ((uint64)pPanelInfo->uPclkFreq * uScale) / DP_MODE_TIMING_SCALE_UNITY;
    pPanelInfo->uVisibleWidthInPixels    = (pPanelInfo->uVisibleWidthInPixels * uScale) / DP_MODE_TIMING_SCALE_UNITY;
    pPanelInfo->uHsyncFrontPorchInPixels = (pPanelInfo->uHsyncFrontPorchInPixels * uScale) / DP_MODE_TIMING_SCALE_UNITY;
    pPanelInfo->uHsyncBackPorchInPixels  = (pPanelInfo->uHsyncBackPorchInPixels * uScale) / DP_MODE_TIMING_SCALE_UNITY;
    pPanelInfo->uHsyncPulseInPixels      = (pPanelInfo->uHsyncPulseInPixels * uScale) / DP_MODE_TIMING_SCALE_UNITY;
    pPanelInfo->uHLeftBorderInPixels     = (pPanelInfo->uHLeftBorderInPixels * uScale) / DP_MODE_TIMING_SCALE_UNITY;
    pPanelInfo->uHRightBorderInPixels    = (pPanelInfo->uHRightBorderInPixels * uScale) / DP_MODE_TIMING_SCALE_UNITY;
  }
}

/* ---------------------------------------------------------------------- */
/**
** FUNCTION: GetPanelAttributesFromPanelInfo()
**
** DESCRIPTION:
**   Populate the MDP_Panel_AttrType struct with timing info from the
**   DP_PanelInfoType struct.
**
*//* -------------------------------------------------------------------- */
static void GetPanelAttributesFromPanelInfo(MDP_Panel_AttrType *pPanelAttr,
                                            DP_PanelInfoType   *pPanelInfo)
{
  if ((NULL == pPanelAttr) ||
      (NULL == pPanelInfo))
  {
    DP_LOG_MESSAGE_ERROR("GetPanelAttributesFromPanelInfo: Invalid input");
  }
  else
  {
    pPanelAttr->uDisplayWidth                       = pPanelInfo->uVisibleWidthInPixels;
    pPanelAttr->uDisplayHeight                      = pPanelInfo->uVisibleHeightInPixels;
    pPanelAttr->uAttrs.sDp.uRefreshRate             = pPanelInfo->uRefreshRate;
    pPanelAttr->sActiveTiming.uHsyncBackPorchDclk   = pPanelInfo->uHsyncBackPorchInPixels;
    pPanelAttr->sActiveTiming.uHsyncFrontPorchDclk  = pPanelInfo->uHsyncFrontPorchInPixels;
    pPanelAttr->sActiveTiming.uHsyncPulseWidthDclk  = pPanelInfo->uHsyncPulseInPixels;
    pPanelAttr->sActiveTiming.uHsyncSkewDclk        = pPanelInfo->uHsyncSkewInPixels;
    pPanelAttr->sActiveTiming.uVsyncBackPorchLines  = pPanelInfo->uVsyncBackPorchInLines;
    pPanelAttr->sActiveTiming.uVsyncFrontPorchLines = pPanelInfo->uVsyncFrontPorchInLines;
    pPanelAttr->sActiveTiming.uVsyncPulseWidthLines = pPanelInfo->uVsyncPulseInLines;
    pPanelAttr->sActiveTiming.uHLeftBorderDClk      = pPanelInfo->uHLeftBorderInPixels;
    pPanelAttr->sActiveTiming.uHRightBorderDClk     = pPanelInfo->uHRightBorderInPixels;
    pPanelAttr->sActiveTiming.uVTopBorderLines      = pPanelInfo->uVTopBorderInLines;
    pPanelAttr->sActiveTiming.uVBottomBorderLines   = pPanelInfo->uVBottomBorderInLines;
    pPanelAttr->sActiveTiming.bInterlaced           = pPanelInfo->bInterlaced;
    pPanelAttr->sActiveTiming.uHsyncInvertSignal    = pPanelInfo->bHSyncActiveLow;     
    pPanelAttr->sActiveTiming.uVsyncInvertSignal    = pPanelInfo->bVSyncActiveLow;
    pPanelAttr->sActiveTiming.uDataEnInvertSignal   = FALSE;

    if (DP_PIXEL_FORMAT_RGB_101010_30BPP == pPanelInfo->eColorFormat)
    {
      pPanelAttr->eColorFormat = MDP_PIXEL_FORMAT_RGB_101010_30BPP;
    }
    else if (DP_PIXEL_FORMAT_RGB_666_18BPP == pPanelInfo->eColorFormat)
    {
      pPanelAttr->eColorFormat = MDP_PIXEL_FORMAT_RGB_666_18BPP;
    }
    else
    {
      pPanelAttr->eColorFormat = MDP_PIXEL_FORMAT_RGB_888_24BPP;
    }        
  }
}

/* ---------------------------------------------------------------------- */
/**
** FUNCTION: DPDriver_UpdateBondedModeTimings()
**
** DESCRIPTION:
**   Scale the panel attribute timings to represent the display with all 
**   bonded devices combined.
**
*//* -------------------------------------------------------------------- */
void DPDriver_UpdateBondedModeTimings(MDP_Panel_AttrType *pPanelAttr)
{
  if (NULL == pPanelAttr)
  {
    DP_LOG_MESSAGE_ERROR("DPDriver_UpdateBondedModeTimings: Invalid input");
  }
  else
  {
    DP_PanelInfoType sPanelInfo;

    MDP_OSAL_MEMZERO(&sPanelInfo, sizeof(DP_PanelInfoType));

    /* Populate timing info from panel attributes struct into the DP panel info struct  */
    GetPanelInfoFromPanelAttributes(&sPanelInfo, pPanelAttr);

    /* Upscale the horizontal timing parmeters in the DP panel info struct based on
     * the total number of devices operating in bonded mode */
    ScaleModeTimings(&sPanelInfo, DP_MODE_TIMING_SCALE_UNITY * pPanelAttr->uAttrs.sDp.sBondedModeConfig.uNumDevices);

    /* Update the panel attribute timing info with the scaled timing info */
    GetPanelAttributesFromPanelInfo(pPanelAttr, &sPanelInfo);
  }
}

/* ---------------------------------------------------------------------- */
/**
** FUNCTION: IsAltModeDevice()
**
** DESCRIPTION:
**   Returns true if DP Device ID belongs to an Alt-Mode device.
**
*//* -------------------------------------------------------------------- */
static bool32 IsAltModeDevice(DP_DeviceIDType eDeviceID)
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

