/*=============================================================================
 
  File: hal_dp_mainlink.c
 
  Source file for DP AUX Main link functionality 
   
  Copyright (c) 2016-2018 Qualcomm Technologies, Inc.  All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential.
=============================================================================*/


/*------------------------------------------------------------------------------
 * Include Files
 *----------------------------------------------------------------------------*/
#include "hal_dp_i.h"

#ifdef __cplusplus
extern "C" { 
#endif

/* LUT to convert the HAL_DP_LaneMapType to HWIO_DP_LaneMap_Type */
HWIO_DP_LaneMap_Type eHalLaneMapToHWIOLaneMap[] =
{
  HWIO_DP_LANEMAP_TYPE_NONE,     //HAL_DP_LANEMAP_TYPE_NONE
  HWIO_DP_LANEMAP_TYPE_0123,     //HAL_DP_LANEMAP_TYPE_0123
  HWIO_DP_LANEMAP_TYPE_0132,     //HAL_DP_LANEMAP_TYPE_0132
  HWIO_DP_LANEMAP_TYPE_0213,     //HAL_DP_LANEMAP_TYPE_0213
  HWIO_DP_LANEMAP_TYPE_0231,     //HAL_DP_LANEMAP_TYPE_0231
  HWIO_DP_LANEMAP_TYPE_0312,     //HAL_DP_LANEMAP_TYPE_0312
  HWIO_DP_LANEMAP_TYPE_0321,     //HAL_DP_LANEMAP_TYPE_0321
  HWIO_DP_LANEMAP_TYPE_1023,     //HAL_DP_LANEMAP_TYPE_1023
  HWIO_DP_LANEMAP_TYPE_1032,     //HAL_DP_LANEMAP_TYPE_1032
  HWIO_DP_LANEMAP_TYPE_1203,     //HAL_DP_LANEMAP_TYPE_1203
  HWIO_DP_LANEMAP_TYPE_1230,     //HAL_DP_LANEMAP_TYPE_1230
  HWIO_DP_LANEMAP_TYPE_1302,     //HAL_DP_LANEMAP_TYPE_1302
  HWIO_DP_LANEMAP_TYPE_1320,     //HAL_DP_LANEMAP_TYPE_1320
  HWIO_DP_LANEMAP_TYPE_2013,     //HAL_DP_LANEMAP_TYPE_2013
  HWIO_DP_LANEMAP_TYPE_2031,     //HAL_DP_LANEMAP_TYPE_2031
  HWIO_DP_LANEMAP_TYPE_2103,     //HAL_DP_LANEMAP_TYPE_2103
  HWIO_DP_LANEMAP_TYPE_2130,     //HAL_DP_LANEMAP_TYPE_2130
  HWIO_DP_LANEMAP_TYPE_2301,     //HAL_DP_LANEMAP_TYPE_2301
  HWIO_DP_LANEMAP_TYPE_2310,     //HAL_DP_LANEMAP_TYPE_2310
  HWIO_DP_LANEMAP_TYPE_3012,     //HAL_DP_LANEMAP_TYPE_3012
  HWIO_DP_LANEMAP_TYPE_3021,     //HAL_DP_LANEMAP_TYPE_3021
  HWIO_DP_LANEMAP_TYPE_3102,     //HAL_DP_LANEMAP_TYPE_3102
  HWIO_DP_LANEMAP_TYPE_3120,     //HAL_DP_LANEMAP_TYPE_3120
  HWIO_DP_LANEMAP_TYPE_3201,     //HAL_DP_LANEMAP_TYPE_3201
  HWIO_DP_LANEMAP_TYPE_3210,     //HAL_DP_LANEMAP_TYPE_3210
};

/* LUT to convert the HAL_DP_RGBMapType to HWIO_DP_RGBMapType */
HWIO_DP_RGBMapType gHALRgbMapToHWIORgbMap[] =
{
  HWIO_DP_RGBMAP_TYPE_NONE,      //HAL_DP_RGBMAP_TYPE_NONE
  HWIO_DP_RGBMAP_TYPE_RGB,       //HAL_DP_RGBMAP_TYPE_RGB
  HWIO_DP_RGBMAP_TYPE_RBG,       //HAL_DP_RGBMAP_TYPE_RBG
  HWIO_DP_RGBMAP_TYPE_GBR,       //HAL_DP_RGBMAP_TYPE_GBR
  HWIO_DP_RGBMAP_TYPE_GRB,       //HAL_DP_RGBMAP_TYPE_GRB
  HWIO_DP_RGBMAP_TYPE_BGR,       //HAL_DP_RGBMAP_TYPE_BGR
  HWIO_DP_RGBMAP_TYPE_BRG,       //HAL_DP_RGBMAP_TYPE_BRG
  HWIO_DP_RGBMAP_TYPE_MAX,       //HAL_DP_RGBMAP_TYPE_RGB
};

/* -----------------------------------------------------------------------
** Public Functions
** ----------------------------------------------------------------------- */



/****************************************************************************
*
** FUNCTION: HAL_DP_Mainlink_Enable()
*/
/*!
* \brief
*   The \b Enable main link  
*
* \param [in] - bEnable TRUE - Enable, FALSE - Disable
*
* \retval void
*
****************************************************************************/
void HAL_DP_Mainlink_Enable(bool32 bEnable)
{
  uint32 uRegValue = in_dword(HWIO_DP_MAINLINK_CTRL_ADDR);

  if (TRUE == bEnable)
  {
    uRegValue = HWIO_OUT_FLD(uRegValue, DP_MAINLINK_CTRL, DP_MAINLINK_ENABLE, 1);
  }
  else
  {
    uRegValue = HWIO_OUT_FLD(uRegValue, DP_MAINLINK_CTRL, DP_MAINLINK_ENABLE, 0);
  }
  
  out_dword(HWIO_DP_MAINLINK_CTRL_ADDR, uRegValue);
}

/****************************************************************************
*
** FUNCTION: HAL_DP_SetLinkState()
*/
/*!
* \brief
*   The \b Set Link state 
*
* \param [in] - eLinkState: State to set the link to
*
* \retval void
*
****************************************************************************/
void HAL_DP_SetLinkState(HAL_DP_LinkCommandType  eLinkState)
{
  uint32 uRegVal    = 0;

  switch (eLinkState)
  {
    case HAL_DP_LINKCOMMAND_TYPE_SEND_PATTERN1:
    {
      uRegVal = HWIO_DP_STATE_CTRL_SW_LINK_TRAINING_PATTERN1_BMSK;
      break;
    }
    case HAL_DP_LINKCOMMAND_TYPE_SEND_PATTERN2:
    {
      uRegVal = HWIO_DP_STATE_CTRL_SW_LINK_TRAINING_PATTERN2_BMSK;
      break;
    }
    case HAL_DP_LINKCOMMAND_TYPE_SEND_PATTERN3:
    {
      uRegVal = HWIO_DP_STATE_CTRL_SW_LINK_TRAINING_PATTERN3_BMSK;
      break;
    }
    case HAL_DP_LINKCOMMAND_TYPE_SEND_SYMBOL_ERM:
    {
      uRegVal = HWIO_DP_STATE_CTRL_SW_LINK_SYMBOL_ERROR_RATE_MEASUREMENT_BMSK;
      break;
    }
    case HAL_DP_LINKCOMMAND_TYPE_SEND_PRBS7:
    {
      uRegVal = HWIO_DP_STATE_CTRL_SW_LINK_PRBS7_BMSK;
      break;
    }
    case HAL_DP_LINKCOMMAND_TYPE_SEND_CUSTOM_PATTERN:
    {
      uRegVal = HWIO_DP_STATE_CTRL_SW_LINK_TEST_CUSTOM_80BIT_PATTERN_BMSK;
      break;
    }
    case HAL_DP_LINKCOMMAND_TYPE_SEND_VIDEO:
    {
      uRegVal = HWIO_DP_STATE_CTRL_SW_SEND_VIDEO_BMSK;
      break;
    }
    case HAL_DP_LINKCOMMAND_TYPE_PUSH_IDLE:
    {
      uRegVal = HWIO_DP_STATE_CTRL_SW_PUSH_IDLE_BMSK;
      break;
    }

    default:
    {
      break;
    }
  }

  /* Have to clear the state first */
  out_dword(HWIO_DP_STATE_CTRL_ADDR, 0);

  /* Set the state */
  out_dword(HWIO_DP_STATE_CTRL_ADDR, uRegVal);
}

/****************************************************************************
*
** FUNCTION: HAL_DP_SetTUConfig()
*/
/*!
* \brief
*   The \b Set Transfer Unit related configuration 
*
* \param [in] -   psTUConfig:  pointer to the TU configuration 
*
* \retval void
*
****************************************************************************/
void HAL_DP_SetTUConfig(HAL_DP_TUConfigType *psTUConfig)
{
  uint32 uRegVal                = 0;

  /* Valid boundaries that determine the relation between link clock and pixel clock in link and pixel clock domains */
  uRegVal = HWIO_OUT_FLD(0,       DP_VALID_BOUNDARY, VALID_BOUNDARY_LINK,        psTUConfig->uValidBoundaryLink);
  uRegVal = HWIO_OUT_FLD(uRegVal, DP_VALID_BOUNDARY, DELAY_START_LINK,           psTUConfig->uDelayStartLink);
  out_dword(HWIO_DP_VALID_BOUNDARY_ADDR, uRegVal);

  /* Boundary moderation parameters */
  uRegVal = HWIO_OUT_FLD(0,       DP_VALID_BOUNDARY_2, BOUNDARY_MODERATION_EN,   psTUConfig->bBoundaryModEnable);
  uRegVal = HWIO_OUT_FLD(uRegVal, DP_VALID_BOUNDARY_2, VALID_LOWER_BOUNDARY_LINK,psTUConfig->uValidLowerBoundary);
  uRegVal = HWIO_OUT_FLD(uRegVal, DP_VALID_BOUNDARY_2, UPPER_BOUNDARY_COUNT,     psTUConfig->uUpperBoundaryCount);
  uRegVal = HWIO_OUT_FLD(uRegVal, DP_VALID_BOUNDARY_2, LOWER_BOUNDARY_COUNT,     psTUConfig->uLowerBoundaryCount);
  out_dword(HWIO_DP_VALID_BOUNDARY_2_ADDR, uRegVal);

  /* Transfer Unit parameters*/
  uRegVal = HWIO_OUT_FLD(uRegVal, DP_TU, TU_SIZE_MINUS1, ( psTUConfig->uTuSize -1));
  out_dword(HWIO_DP_TU_ADDR, uRegVal);
}

/****************************************************************************
*
** FUNCTION: HAL_DP_SetSinkAttrib()
*/
/*!
* \brief
*   The \b Sets lane mapping, number of lanes, color format, etc.
*
* \param [in] -   psSinkAttrib:  pointer to the sink attribute configuration 
*
* \retval void
*
****************************************************************************/
void HAL_DP_SetSinkAttrib (HAL_DP_SinkAttribType *psSinkAttrib)
{
  uint32   uRegVal      = 0;
  uint32   uBpc         = 0;
  
  if (psSinkAttrib->eLaneMapping < HAL_DP_LANEMAP_TYPE_MAX)
  {
    uRegVal = eHalLaneMapToHWIOLaneMap[psSinkAttrib->eLaneMapping];
    out_dword(HWIO_DP_LOGICAL2PHYSICAL_LANE_MAPPING_ADDR, uRegVal );
  }

  /* RGB mapping for the input data from MDP to DP */
  if (psSinkAttrib->eRGBMapping < HAL_DP_RGBMAP_TYPE_MAX)
  {
    uRegVal = gHALRgbMapToHWIORgbMap[psSinkAttrib->eRGBMapping];
    out_dword(HWIO_DP_P0CLK_RGB_MAPPING_ADDR, uRegVal );
  }

  switch (psSinkAttrib->uBitPerComponent)
  {
    case 6:
      {
        uBpc = 0;
      }
      break;
    case 10:
      {
        uBpc = 2;
      }
      break;
    case 8:
    default:
      {
        uBpc = 1;
      }
      break;
  }

  /* Main Link configuration */
  uRegVal = in_dword(HWIO_DP_CONFIGURATION_CTRL_ADDR);
  uRegVal = HWIO_OUT_FLD( uRegVal, DP_CONFIGURATION_CTRL, NUMBER_OF_LANES,(psSinkAttrib->uLaneNumber - 1));
  uRegVal = HWIO_OUT_FLD( uRegVal, DP_CONFIGURATION_CTRL, BPC,             uBpc);
  uRegVal = HWIO_OUT_FLD( uRegVal, DP_CONFIGURATION_CTRL, RGB_YUV,         psSinkAttrib->ePixelFormat);  
  out_dword(HWIO_DP_CONFIGURATION_CTRL_ADDR, uRegVal );
}

/****************************************************************************
*
** FUNCTION: HAL_DP_SetStreamAttrib()
*/
/*!
* \brief
*   The \b Sets static/dynamic, ASSR, etc.
*
* \param [in] -   psStreamAttrib:  pointer to the stream attribute configuration 
*
* \retval void
*
****************************************************************************/
void HAL_DP_SetStreamAttrib(HAL_DP_StreamAttribType *psStreamAttrib)
{
  uint32   uRegVal = in_dword(HWIO_DP_CONFIGURATION_CTRL_ADDR);

  uRegVal = HWIO_OUT_FLD( uRegVal, DP_CONFIGURATION_CTRL, STATIC_DYNAMIC_COUNTER, psStreamAttrib->bSynchronousClk);
  uRegVal = HWIO_OUT_FLD( uRegVal, DP_CONFIGURATION_CTRL, ENHANCED_FRAMING,       psStreamAttrib->bEnhancedFraming);
  uRegVal = HWIO_OUT_FLD( uRegVal, DP_CONFIGURATION_CTRL, ASSR,                   psStreamAttrib->bASSR);

  if (psStreamAttrib->bInterlaced)
  {
    uRegVal = HWIO_OUT_FLD( uRegVal, DP_CONFIGURATION_CTRL, PROGRESSIVE_INTERLACED, 0);
  }

  out_dword(HWIO_DP_CONFIGURATION_CTRL_ADDR, uRegVal );
}

/****************************************************************************
*
** FUNCTION: HAL_DP_SetMSA()
*/
/*!
* \brief
*   The \b Sets Mainstream attributes 
*
* \param [in] -   psMainStream:  pointer to MSA to be programmed 
*
* \retval void
*
****************************************************************************/
void HAL_DP_SetMSA(HAL_DP_MSAPacketType *psMainStream)
{
  uint32   uRegVal = 0;
  
  /* Set pixel clock to link clock ratio */
  out_dword(HWIO_DP_SOFTWARE_MVID_ADDR, psMainStream->uSWMVid );
  out_dword(HWIO_DP_SOFTWARE_NVID_ADDR, psMainStream->uSWNVid );

  /* Set total width and height */
  uRegVal =  HWIO_OUT_FLD( 0,       DP_TOTAL_HOR_VER, TOTAL_HOR, ( psMainStream->uHsyncPulseInPixels      + 
                                                                    psMainStream->uHsyncBackPorchInPixels  +
                                                                    psMainStream->uVisibleWidthInPixels    +  
                                                                    psMainStream->uHsyncFrontPorchInPixels));
  uRegVal =  HWIO_OUT_FLD( uRegVal, DP_TOTAL_HOR_VER, TOTAL_VER, ( psMainStream->uVsyncPulseInLines       +
                                                                    psMainStream->uVsyncBackPorchInLines   +
                                                                    psMainStream->uVisibleHeightInPixels   +
                                                                    psMainStream->uVsyncFrontPorchInLines));
  out_dword(HWIO_DP_TOTAL_HOR_VER_ADDR, uRegVal );

  /* Set horizontal and vertical active start relative to the leading edges of HSYNC and VSYNC */
  uRegVal =  HWIO_OUT_FLD( 0,       DP_START_HOR_VER_FROM_SYNC, START_FROM_HSYNC, (psMainStream->uHsyncPulseInPixels+
                                                                                    psMainStream->uHsyncBackPorchInPixels  ));
  uRegVal =  HWIO_OUT_FLD( uRegVal, DP_START_HOR_VER_FROM_SYNC, START_FROM_VSYNC, (psMainStream->uVsyncPulseInLines +
                                                                                    psMainStream->uVsyncBackPorchInLines   ));
  out_dword(HWIO_DP_START_HOR_VER_FROM_SYNC_ADDR, uRegVal );


  /* Set horizontal and vertical sync pulse width and polarity */
  uRegVal =  HWIO_OUT_FLD( 0,       DP_HSYNC_VSYNC_WIDTH, POLARITY_HSW, psMainStream->uHsyncPulseInPixels);
  uRegVal =  HWIO_OUT_FLD( uRegVal, DP_HSYNC_VSYNC_WIDTH, POLARITY_HSP, psMainStream->bHSyncActiveLow);
  uRegVal =  HWIO_OUT_FLD( uRegVal, DP_HSYNC_VSYNC_WIDTH, POLARITY_VSW, psMainStream->uVsyncPulseInLines);
  uRegVal =  HWIO_OUT_FLD( uRegVal, DP_HSYNC_VSYNC_WIDTH, POLARITY_VSP, psMainStream->bVSyncActiveLow);
  out_dword(HWIO_DP_HSYNC_VSYNC_WIDTH_POLARITY_ADDR, uRegVal );

  /* Set active video width and height */
  uRegVal =  HWIO_OUT_FLD( 0,       DP_ACTIVE_HOR_VER, ACTIVE_HOR, psMainStream->uVisibleWidthInPixels);
  uRegVal =  HWIO_OUT_FLD( uRegVal, DP_ACTIVE_HOR_VER, ACTIVE_VER, psMainStream->uVisibleHeightInPixels);
  out_dword(HWIO_DP_ACTIVE_HOR_VER_ADDR, uRegVal );
   
  /* Misc settings */
  uRegVal =  HWIO_OUT_FLD( 0,       DP_MISC1_MISC0, MISC0, psMainStream->uMisc0);
  uRegVal =  HWIO_OUT_FLD( uRegVal, DP_MISC1_MISC0, MISC1, psMainStream->uMisc1);
  out_dword(HWIO_DP_MISC1_MISC0_ADDR, uRegVal );
}

/****************************************************************************
*
** FUNCTION: HAL_DP_SetVSC()
*/
/*!
* \brief
*   The \b Sets Video Stream Config attributes 
*
* \param [in] -   psVSCPacket:  pointer to VSC to be programmed 
*
* \retval void
*
****************************************************************************/
void HAL_DP_SetVSC(HAL_DP_VSCPacketType *psVSCPacket)
{
  uint32 uRegVal;

  uRegVal = in_dword( HWIO_DP_CONFIGURATION_CTRL_ADDR );
  uRegVal = HWIO_OUT_FLD( uRegVal, DP_CONFIGURATION_CTRL, SEND_VSC, psVSCPacket->bSendVSC );
  out_dword( HWIO_DP_CONFIGURATION_CTRL_ADDR, uRegVal );

  if(TRUE == psVSCPacket->bSendVSC )
  {
    uRegVal =  HWIO_OUT_FLD( 0,       DP_VSC_DB0_PB4, DB0, psVSCPacket->uVSC_DB0);
    uRegVal =  HWIO_OUT_FLD( uRegVal, DP_VSC_DB0_PB4, PB4, psVSCPacket->uVSC_PB4);
    out_dword(HWIO_DP_VSC_DB0_PB4_ADDR, uRegVal );

    uRegVal =  HWIO_OUT_FLD( 0,       DP_VSC_HB0_PB0_HB1_PB1, HB0, psVSCPacket->uVSC_HB0);
    uRegVal =  HWIO_OUT_FLD( uRegVal, DP_VSC_HB0_PB0_HB1_PB1, PB0, psVSCPacket->uVSC_PB0);
    uRegVal =  HWIO_OUT_FLD( uRegVal, DP_VSC_HB0_PB0_HB1_PB1, HB1, psVSCPacket->uVSC_HB1);
    uRegVal =  HWIO_OUT_FLD( uRegVal, DP_VSC_HB0_PB0_HB1_PB1, PB1, psVSCPacket->uVSC_PB1);
    out_dword(HWIO_DP_VSC_HB0_PB0_HB1_PB1_ADDR, uRegVal );

    uRegVal =  HWIO_OUT_FLD( 0,       DP_VSC_HB2_PB2_HB3_PB3, HB2, psVSCPacket->uVSC_HB2);
    uRegVal =  HWIO_OUT_FLD( uRegVal, DP_VSC_HB2_PB2_HB3_PB3, PB2, psVSCPacket->uVSC_PB2);
    uRegVal =  HWIO_OUT_FLD( uRegVal, DP_VSC_HB2_PB2_HB3_PB3, HB3, psVSCPacket->uVSC_HB3);
    uRegVal =  HWIO_OUT_FLD( uRegVal, DP_VSC_HB2_PB2_HB3_PB3, PB3, psVSCPacket->uVSC_PB3);
    out_dword(HWIO_DP_VSC_HB2_PB2_HB3_PB3_ADDR, uRegVal );
  }
}

/****************************************************************************
*
** FUNCTION: HAL_DP_Mainlink_Status()
*/
/*!
* \brief
*   The \b Get Main link status 
*
* \param [in] -   
*
* \retval bit-field of HAL_DP_LinkStatusType
*
****************************************************************************/
uint32 HAL_DP_Mainlink_Status(void)
{
  uint32 uRegValue   = 0;
  uint32 uLinkStatus  = 0; 

  uRegValue   = in_dword(HWIO_DP_MAINLINK_READY_ADDR);
  uLinkStatus  = 0;
  if( uRegValue & HWIO_DP_MAINLINK_READY_READY_FOR_VIDEO_BMSK)
  {
    uLinkStatus |= HAL_DP_LINKSTATUS_TYPE_READY_FOR_VIDEO;
  }
  if( uRegValue & HWIO_DP_MAINLINK_READY_IDLE_PATTERNS_SENT_BMSK)
  {
    uLinkStatus |= HAL_DP_LINKSTATUS_TYPE_IDLE_PATTERNS_SENT;
  }
  if( uRegValue & HWIO_DP_MAINLINK_READY_READY_STATE_BMSK)
  {
    uLinkStatus |= HAL_DP_LINKSTATUS_TYPE_READY_STATE;
  }
  if( uRegValue & HWIO_DP_MAINLINK_READY_LINK_TRAINING1_BMSK)
  {
    uLinkStatus |= HAL_DP_LINKSTATUS_TYPE_SENDING_PATTERN1;
  }
  if( uRegValue & HWIO_DP_MAINLINK_READY_LINK_TRAINING2_BMSK)
  {
    uLinkStatus |= HAL_DP_LINKSTATUS_TYPE_SENDING_PATTERN2;
  }
  if( uRegValue & HWIO_DP_MAINLINK_READY_LINK_TRAINING3_BMSK)
  {
    uLinkStatus |= HAL_DP_LINKSTATUS_TYPE_SENDING_PATTERN3;
  }
  if( uRegValue & HWIO_DP_MAINLINK_READY_LINK_SYMBOL_ERM_BMSK)
  {
    uLinkStatus |= HAL_DP_LINKSTATUS_TYPE_SENDING_SYMBOL_ERM;
  }
  if( uRegValue & HWIO_DP_MAINLINK_READY_LINK_PRBS7_BMSK)
  {
    uLinkStatus |= HAL_DP_LINKSTATUS_TYPE_SENDING_PRBS7;
  }
  if( uRegValue & HWIO_DP_MAINLINK_READY_LINK_CUSTOM80_BMSK)
  {
    uLinkStatus |= HAL_DP_LINKSTATUS_TYPE_SENDING_CUSTOM_PATTERN;
  }
  if( uRegValue & HWIO_DP_MAINLINK_READY_SEND_IDLE_PATTERN_BMSK)
  {
    uLinkStatus |= HAL_DP_LINKSTATUS_TYPE_SENDING_IDLE_PATTERN;
  }
  if( uRegValue & HWIO_DP_MAINLINK_READY_SEND_BS_BMSK)
  {
    uLinkStatus |= HAL_DP_LINKSTATUS_TYPE_SENDING_BS;
  }
  if( uRegValue & HWIO_DP_MAINLINK_READY_SEND_MSA_BMSK)
  {
    uLinkStatus |= HAL_DP_LINKSTATUS_TYPE_SENDING_MSA;
  }
  if( uRegValue & HWIO_DP_MAINLINK_READY_SEND_VSC_BMSK)
  {
    uLinkStatus |= HAL_DP_LINKSTATUS_TYPE_SENDING_VSC;
  }
  if( uRegValue & HWIO_DP_MAINLINK_READY_SEND_ACTIVE18BPP_BMSK)
  {
    uLinkStatus |= HAL_DP_LINKSTATUS_TYPE_SENDING_ACTIVE18BPP;
  }
  if( uRegValue & HWIO_DP_MAINLINK_READY_SEND_ACTIVE24BPP_BMSK)
  {
    uLinkStatus |= HAL_DP_LINKSTATUS_TYPE_SENDING_ACTIVE24BPP;
  }
  if( uRegValue & HWIO_DP_MAINLINK_READY_SEND_IDLE_PATTERN2_BMSK)
  {
    uLinkStatus |= HAL_DP_LINKSTATUS_TYPE_SENDING_IDLE_PATTERN2;
  }

  uRegValue   = in_dword(HWIO_DP_MAINLINK_CTRL_ADDR);
  if( uRegValue & HWIO_DP_MAINLINK_CTRL_DP_MAINLINK_ENABLE_BMSK)
  {
    uLinkStatus |= HAL_DP_LINKSTATUS_TYPE_ENABLED;
  }
  else
  {
    uLinkStatus |= HAL_DP_LINKSTATUS_TYPE_DISABLED;
  }
  
  return uLinkStatus;
}

   
/****************************************************************************
*
** FUNCTION: HAL_DP_SetOptionalConfig()
*/
/*!
* \brief
*   The \b Sets optional attributes 
*
* \param [in] -   psCtrlOptionalCfg:  pointer to config to be programmed 
*
* \retval void
*
****************************************************************************/
void HAL_DP_SetOptionalConfig(HAL_DP_OptionalType      *psCtrlOptionalCfg)
{
  uint32 uRegVal = in_dword( HWIO_DP_MAINLINK_CTRL_ADDR );

  /* Main link options */
  if(NULL!= psCtrlOptionalCfg->pbBypassLaneSkew)
  {   
    uRegVal = HWIO_OUT_FLD( uRegVal, DP_MAINLINK_CTRL, SW_BYPASS_INTERLANE_SKEW, *(psCtrlOptionalCfg->pbBypassLaneSkew));
  }
  if(NULL!= psCtrlOptionalCfg->pbBypassScrambler)
  {   
    uRegVal = HWIO_OUT_FLD( uRegVal, DP_MAINLINK_CTRL, SW_BYPASS_SCRAMBLER, *(psCtrlOptionalCfg->pbBypassScrambler));
  }
  if(NULL!= psCtrlOptionalCfg->pbBypass8B10B)
  {   
    uRegVal = HWIO_OUT_FLD( uRegVal, DP_MAINLINK_CTRL, SW_BYPASS_8B10B, *(psCtrlOptionalCfg->pbBypass8B10B));
  }
  if(NULL!= psCtrlOptionalCfg->pbForcePixeClkOn)
  {   
    uRegVal = HWIO_OUT_FLD( uRegVal, DP_MAINLINK_CTRL, SW_FORCE_FIFO_PIXEL_CLOCK_ON, *(psCtrlOptionalCfg->pbForcePixeClkOn));
  }
  if(NULL!= psCtrlOptionalCfg->pbLCDSelfTest)
  {   
    uRegVal = HWIO_OUT_FLD( uRegVal, DP_MAINLINK_CTRL, LCD_SELF_TEST,*(psCtrlOptionalCfg->pbLCDSelfTest));
  }
  out_dword(HWIO_DP_MAINLINK_CTRL_ADDR, uRegVal );

  /* Mainlink levels */
  uRegVal = in_dword(HWIO_DP_MAINLINK_LEVELS_ADDR);
  if(NULL!= psCtrlOptionalCfg->puSafeToExitLevel)
  {
    uRegVal = HWIO_OUT_FLD( uRegVal, DP_MAINLINK_LEVELS, SAFE_TO_EXIT_LEVEL,             *(psCtrlOptionalCfg->puSafeToExitLevel));
  }
  if(NULL!= psCtrlOptionalCfg->puIdlePatternRepNum)
  {
    uRegVal = HWIO_OUT_FLD( uRegVal, DP_MAINLINK_LEVELS, IDLE_PATTERN_REPETITION_NUMBER, *(psCtrlOptionalCfg->puIdlePatternRepNum));
  }
  out_dword(HWIO_DP_MAINLINK_LEVELS_ADDR, uRegVal );

  uRegVal = in_dword(HWIO_DP_MAINLINK_LEVELS_2_ADDR);
  if(NULL!= psCtrlOptionalCfg->puMSA2VSCDelay)
  {
    uRegVal = HWIO_OUT_FLD( uRegVal, DP_MAINLINK_LEVELS_2, MSA_TO_VSC_DELAY,      *(psCtrlOptionalCfg->puMSA2VSCDelay));
  }
  if(NULL!= psCtrlOptionalCfg->puVSCRepNum)
  {
    uRegVal = HWIO_OUT_FLD( uRegVal, DP_MAINLINK_LEVELS_2, VSC_REPETITION_NUMBER, *(psCtrlOptionalCfg->puVSCRepNum));
  }
  out_dword(HWIO_DP_MAINLINK_LEVELS_2_ADDR, uRegVal );

  /* Custom test patterns */
  if(psCtrlOptionalCfg->psCustomPattern)
  {
    out_dword(HWIO_DP_TEST_80BIT_CUSTOM_PATTERN_REG0_ADDR, psCtrlOptionalCfg->psCustomPattern->uCustomPattern_R0);
    out_dword(HWIO_DP_TEST_80BIT_CUSTOM_PATTERN_REG1_ADDR, psCtrlOptionalCfg->psCustomPattern->uCustomPattern_R1);
    out_dword(HWIO_DP_TEST_80BIT_CUSTOM_PATTERN_REG2_ADDR, psCtrlOptionalCfg->psCustomPattern->uCustomPattern_R2);
  }
}

#ifdef __cplusplus
}
#endif

