#ifndef _DP_TYPES_H_
#define _DP_TYPES_H_
/*! \file */
/*=====================================================================================================
 
  File: dp_types.h
 
  Header file for data types used by DP driver 
   
  Copyright (c) 2016-2020 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
====================================================================================================*/

#include "hal_dp.h"

/* --------------------------------------------------------------------------------------------------------------------
** Macros and Struct
---------------------------------------------------------------------------------------------------------------------*/

/* --------------------------------------------------------------------------------------------------------------------
Enumerated types
---------------------------------------------------------------------------------------------------------------------*/

/*!
 * \enum DP_TransactionModeType
 *
 * The \b DP_TransactionModeType defines types of AUX Transaction. It either is AUX for DPCD read/write or I2C over 
 * AUX for EDID read.
 *
 */
typedef enum
{
  DP_TRANSACTION_MODE_NONE                       = 0x00000000,
  DP_TRANSACTION_MODE_AUX,                                             /**< This is an native AUX Transaction       */
  DP_TRANSACTION_MODE_I2C,                                             /**< This is an I2C over AUX Transaction     */
  DP_TRANSACTION_MODE_MAX,
  DP_TRANSACTION_MODE_FORCE_32BIT                = 0x7FFFFFFF
} DP_TransactionModeType;

/*!
 * \enum DP_ClockSourceType
 *
 * The \b DP_ClockSourceType defines possible clock source for DP link clock and pixel clock.
 */
typedef enum
{
  DP_CLOCKSOURCE_NONE                            = 0,
  DP_CLOCKSOURCE_DPPLL,                                                /**< Clock is from DP PLL                   */
  DP_CLOCKSOURCE_DSI1PLL,                                              /**< Clock is from DSI1 PLL                  */
  DP_CLOCKSOURCE_DSI2PLL,                                              /**< Clock is from DSI1 PLL                  */
  DP_CLOCKSOURCE_HDMIPLL,                                              /**< Clock is from HDMI PLL                  */
  DP_CLOCKSOURCE_MAX,
  DP_CLOCKSOURCE_FORCE_32BIT                     = 0x7FFFFFFF
} DP_ClockSourceType;

/*!
 * \enum DP_SetPropertyType
 *
 * The \b DP_SetPropertyType defines property type for host driver to set.
 */
typedef enum
{
  DP_SETPROPERTY_NONE                            = 0,
  DP_SETPROPERTY_SET_PANEL_MODE_INDEX,                                 /**< Set index of panel modes                */
  DP_SETPROPERTY_SET_PANEL_INFO,                                       /**< Overwrite current mode of panel info    */
  DP_SETPROPERTY_SET_LINK_RATE,                                        /**< Overwrite Link Rate(270000 or 162000)Khz*/
  DP_SETPROPERTY_SET_LANE_NUMBER,                                      /**< Overwrite desired lane number           */
  DP_SETPROPERTY_SET_ASSR_ENABLE,                                      /**< Overwrite the setting of ASSR           */
  DP_SETPROPERTY_SET_ENHANCEDFRAME_ENABLE,                             /**< Overwrite the setting of enhanced Frame */
  DP_SETPROPERTY_SET_LINK_TRAINING_TYPE,                               /**< Set the sink link training type         */
  DP_SETPROPERTY_SET_CLOCKS,                                           /**< Set clocks dynamically                  */
  DP_SETPROPERTY_RESETCORE,                                            /**< Reset all eDP blocks, core, aux, PHY    */
  DP_SETPROPERTY_CONNECTION_STATUS,                                    /**< Set connection status                   */
  DP_SETPROPERTY_EDP_PSR_ACTION,                                       /**< eDP PSR Action                          */
  DP_SETPROPERTY_DSC_PPS_INFO,                                         /**< DP PPS Info                             */
  DP_SETPROPERTY_BRIGHTNESS_CONTROL,                                   /**< Enable/Disable and set brightness level */
  DP_SETPROPERTY_RESET_PHY,                                            /**< Reset DP PHY                            */
  DP_SETPROPERTY_MAX,
  DP_SETPROPERTY_FORCE_32BIT                     = 0x7FFFFFFF
} DP_SetPropertyType;

/*!
 * \enum DP_GetPropertyType
 *
 * The \b DP_GetPropertyType defines property type for host driver to query.
 */
typedef enum
{
  DP_GETPROPERTY_NONE                            = 0,
  DP_GETPROPERTY_GET_MODE_INFO,                                        /**< Get Panel modes                         */
  DP_GETPROPERTY_AUXSTATUS,                                            /**< Get DP AUX status                       */
  DP_GETPROPERTY_LANE_NUMBER,                                          /**< Get DP panel lane number                */
  DP_GETPROPERTY_LINK_RATE,                                            /**< Get DP panel link rate                  */
  DP_GETPROPERTY_CONNECTION_STATUS,                                    /**< Get a flag if DP panel support ASSR     */
  DP_GETPROPERTY_ENHANCEDFRAME_ENABLE,                                 /**< Get flag if panel support ENHANCEDFRAME */
  DP_GETPROPERTY_EDID_INFO,                                            /**< Get EDID info                           */
  DP_GETPROPERTY_DSC_SUPPORTED,                                        /**< Get DSC supported info                  */
  DP_GETPROPERTY_FRAME_CRC,                                            /**< Get frame CRC                           */
  DP_GETPROPERTY_MAX,
  DP_GETPROPERTY_FORCE_32BIT                     = 0x7FFFFFFF
} DP_GetPropertyType;


/*!
 * \enum DP_LaneMapType
 *
 * The \b DP_LaneMapType defines physical lane and logic lane mapping.   
 *
 */
typedef enum
{
   DP_LANEMAP_TYPE_NONE                           = 0x00000000,
   DP_LANEMAP_TYPE_0123,                                               /**< Physical lane map to logic lane as 0123 */
   DP_LANEMAP_TYPE_0132,                                               /**< Physical lane map to logic lane as 0132 */
   DP_LANEMAP_TYPE_0213,                                               /**< Physical lane map to logic lane as 0213 */
   DP_LANEMAP_TYPE_0231,                                               /**< Physical lane map to logic lane as 0231 */
   DP_LANEMAP_TYPE_0312,                                               /**< Physical lane map to logic lane as 0312 */
   DP_LANEMAP_TYPE_0321,                                               /**< Physical lane map to logic lane as 0321 */
   DP_LANEMAP_TYPE_1023,                                               /**< Physical lane map to logic lane as 1023 */
   DP_LANEMAP_TYPE_1032,                                               /**< Physical lane map to logic lane as 1032 */
   DP_LANEMAP_TYPE_1203,                                               /**< Physical lane map to logic lane as 1203 */
   DP_LANEMAP_TYPE_1230,                                               /**< Physical lane map to logic lane as 1230 */
   DP_LANEMAP_TYPE_1302,                                               /**< Physical lane map to logic lane as 1302 */
   DP_LANEMAP_TYPE_1320,                                               /**< Physical lane map to logic lane as 1320 */
   DP_LANEMAP_TYPE_2013,                                               /**< Physical lane map to logic lane as 2013 */
   DP_LANEMAP_TYPE_2031,                                               /**< Physical lane map to logic lane as 2031 */
   DP_LANEMAP_TYPE_2103,                                               /**< Physical lane map to logic lane as 2103 */
   DP_LANEMAP_TYPE_2130,                                               /**< Physical lane map to logic lane as 2130 */
   DP_LANEMAP_TYPE_2301,                                               /**< Physical lane map to logic lane as 2301 */
   DP_LANEMAP_TYPE_2310,                                               /**< Physical lane map to logic lane as 2310 */
   DP_LANEMAP_TYPE_3012,                                               /**< Physical lane map to logic lane as 3012 */
   DP_LANEMAP_TYPE_3021,                                               /**< Physical lane map to logic lane as 3021 */
   DP_LANEMAP_TYPE_3102,                                               /**< Physical lane map to logic lane as 3102 */
   DP_LANEMAP_TYPE_3120,                                               /**< Physical lane map to logic lane as 3120 */
   DP_LANEMAP_TYPE_3201,                                               /**< Physical lane map to logic lane as 3201 */
   DP_LANEMAP_TYPE_3210,                                               /**< Physical lane map to logic lane as 3210 */
   DP_LANEMAP_TYPE_MAX,
   DP_LANEMAP_TYPE_FORCE_32BIT                    = 0x7FFFFFFF
} DP_LaneMapType;

/*!
 * \enum DP_RGBMapType
 *
 * The \b DP_RGBMapType defines RGB component mapping.   
 *
 */
typedef enum
{
   DP_RGBMAP_TYPE_NONE                            = 0x00000000,
   DP_RGBMAP_TYPE_RGB,                                                 /**< Color component order is RGB            */
   DP_RGBMAP_TYPE_RBG,                                                 /**< Color component order is RBG            */
   DP_RGBMAP_TYPE_GBR,                                                 /**< Color component order is GBR            */
   DP_RGBMAP_TYPE_GRB,                                                 /**< Color component order is GRB            */
   DP_RGBMAP_TYPE_BGR,                                                 /**< Color component order is BGR            */
   DP_RGBMAP_TYPE_BRG,                                                 /**< Color component order is BRG            */
   DP_RGBMAP_TYPE_MAX,
   DP_RGBMAP_TYPE_FORCE_32BIT                     = 0x7FFFFFFF
} DP_RGBMapType;

/*!
 * \enum DP_LinkTrainingType
 *
 * The \b DP_TrainingType defines the types of Link Training.
 *
 */
typedef enum
{
   DP_LINK_TRAINING_TYPE_NO_TRAINING              = 0x00000000,        /**< No Link Training                        */
   DP_LINK_TRAINING_TYPE_FULL_TRAINING,                                /**< Full Link Training                      */
   DP_LINK_TRAINING_TYPE_FAST_TRAINING,                                /**< Training patterns are sent - panel status not read */
   DP_LINK_TRAINING_TYPE_MAX,
   DP_LINK_TRAINING_TYPE_FORCE_32BIT              = 0x7FFFFFFF
} DP_LinkTrainingType;

/*!
 * \enum DP_PixelFormatType
 *
 * The \b DP_PixelFormatType defines the supported display/LCD color specific formats.
 *
 */
typedef enum
{
   DP_PIXEL_FORMAT_NONE                           = 0x00000000,
   DP_PIXEL_FORMAT_RGB_666_18BPP,                                      /**< Color Format RGB666                     */
   DP_PIXEL_FORMAT_RGB_888_24BPP,                                      /**< Color Format RGB888                     */
   DP_PIXEL_FORMAT_RGB_101010_30BPP,                                   /**< Color Format RGB101010                     */
   DP_PIXEL_FORMAT_MAX,
   DP_PIXEL_FORMAT_FORCE_32BIT                    = 0x7FFFFFFF
} DP_PixelFormatType;

/* 
 * \enum DP_BrightnessRequestType
 *
 * Brightness Request Types definitions
 */
typedef enum {
    DP_BRIGHTNESS_REQUEST_OFF = 0,  // Voltage disabled
    DP_BRIGHTNESS_REQUEST_ON,       // Voltage Enabled (assumed default level)
    DP_BRIGHTNESS_REQUEST_LEVEL,    // Specific voltage level requested
    DP_BRIGHTNESS_REQUEST_MAX,
    DP_BRIGHTNESS_REQUEST_FORCE_32BIT = 0x7FFFFFFF
} DP_BrightnessRequestType;

/*!
 * \enum DP_ConfigFlagType
 *
 * The \b DP_ConfigFlagType defines the set of flags for setting up DP configuration.
 *
 */
typedef enum
{
  DP_CONFIG_FLAG_NONE                               = 0x00000000,
  DP_CONFIG_FLAG_ALT_MODE_FIXED_LANE_CONFIGURATION  = 0x00000001,  /**< Fixed pin assignment and polarity in alt-mode           */
  DP_CONFIG_FLAG_ALT_MODE_COMMON_RESET              = 0x00000002,  /**< Reset the DP/USB combo PHY                              */
  DP_CONFIG_FLAG_FORCE_32BIT                        = 0x7FFFFFFF
} DP_ConfigFlagType;

/*!
 * \enum DP_PHYResetFlagType
 *
 * The \b DP_PHYResetFlagType defines the various PHY reset options.
 *
 */
typedef enum
{
  DP_PHY_RESET_NONE                               = 0x00000000,
  DP_PHY_RESET_ALT_MODE_COMMON_RESET              = 0x00000001,  /**< Reset the DP/USB combo PHY */
  DP_PHY_RESET_FORCE_32BIT                        = 0x7FFFFFFF
} DP_PHYResetFlagType;

/*!
 * \enum DP_PHYModeType
 *
 * The \b DP_PHYModeType defines the various PHY modes.
 *
 */
typedef enum
{
  DP_PHY_MODE_NONE            = 0x00000000,
  DP_PHY_MODE_USB_DATA,                        /**< USB SS only PHY mode              */
  DP_PHY_MODE_DP,                              /**< DP only PHY mode                  */
  DP_PHY_MODE_CONCURRENCY,                     /**< USB SS & DP concurrency PHY mode  */
  DP_PHY_MODE_MAX,
  DP_PHY_MODE_FORCE_32BIT     = 0x7FFFFFFF
} DP_PHYModeType;

/* --------------------------------------------------------------------------------------------------------------------
Data structure Defines
---------------------------------------------------------------------------------------------------------------------*/

/*!
 * \struct DP_ClockConfigType
 *
 * The \b DP_ClockConfigType defines the clock setting for DP link clock and pixel clock.
 *
 */
typedef struct  
{
   DP_ClockSourceType               eLinkClockSource;                   /**< Desired clock Source for link clock     */
   uint32                           uLinkClockFreq;                     /**< Desired link clock frequency            */
   DP_ClockSourceType               ePixelClockSource;                  /**< Desired clock Source for pixel clock    */
   uint32                           uPixelClockFreq;                    /**< Desired pixel clock frequency           */
   uint32                           uRefreshRateFreq;                   /**< Desired refresh rate in Hz              */
} DP_ClockConfigType;


/*!
 * \struct DP_PanelInfoType
 *
 * The \b DP_PanelInfoType defines the panel parameters, color format, timing parameters.
 *
 */
typedef struct
{
   uint32                           uModeIndex;
   DP_PixelFormatType               eColorFormat;                       /**< Color format of panel                                        */
   uint32                           uRefreshRate;                       /**< Refresh rate.                                                */
   uint32                           uPclkFreq;                          /**< Panel Pixel clk frequency in Hz                              */

   uint32                           uVisibleWidthInPixels;              /**< Visible active area width in pixels                          */
   uint32                           uVisibleHeightInPixels;             /**< Visible active area height in pixels                         */

   uint32                           uHsyncFrontPorchInPixels;           /**< HSYNC Front Porch width in pixels                            */
   uint32                           uHsyncBackPorchInPixels;            /**< HSYNC Back Porch width in pixels                             */
   uint32                           uHsyncPulseInPixels;                /**< HSYNC pulse width in pixels                                  */
   uint32                           uHsyncSkewInPixels;                 /**< HSYNC active edge is delayed from VSYNC                      */

   uint32                           uVsyncFrontPorchInLines;            /**< VSYNC Front Porch width in lines                             */
   uint32                           uVsyncBackPorchInLines;             /**< VSYNC Back Porch width in lines                              */
   uint32                           uVsyncPulseInLines;                 /**< VSYNC pulse width in lines                                   */
   
   uint32                           uHLeftBorderInPixels;               /**< Left border in pixels                                        */
   uint32                           uHRightBorderInPixels;              /**< Right border in pixels                                       */
   uint32                           uVTopBorderInLines;                 /**< Top border in lines                                          */
   uint32                           uVBottomBorderInLines;              /**< Bottom border in lines                                       */
   
   bool32                           bHSyncActiveLow;                    /**< Hsync polarity                                               */
   bool32                           bVSyncActiveLow;                    /**< Vsync polarity                                               */
   bool32                           bInterlaced;                        /**< Interlaced support                                           */
   bool32                           bAudioSupported;                    /**< Indicates whether current display mode supports Audio        */
   uint32                           u3DFrameFormat;                     /**< Indicates the type of 3D format this mode is associated with */
   uint32                           uAspectRatio;                       /**< Indicates the aspect ratio of the current mode               */
   uint32                           uRotation;                          /**< Indicates the orientation of the screen                      */
} DP_PanelInfoType;

/*!
 * \struct DP_WritePacketType
 *
 * The \b DP_WritePacketType defines DP AUX write packet. 
 *
 */
typedef struct
{
   DP_TransactionModeType          eTransactionType;                   /**< AUX transaction type                    */
   uint32                          uAddress;                           /**< AUX Write start address                 */
   uint8                          *puWriteData;                        /**< Buffer of data to be written            */
   uint32                          uWriteDataSize;                     /**< Size of data to be written in byte      */
   uint32                          uBytesWritten;                      /**< How many bytes written                  */
   bool32                          bQueueOnly;                         /**< Only queue the command - do not send    */
} DP_WritePacketType;

/*!
 * \struct DP_ReadPacketType
 *
 * The \b DP_ReadPacketType defines DP AUX read packet. 
 *
 */
typedef struct
{
   DP_TransactionModeType          eTransactionType;                   /**< AUX transaction type                    */
   uint32                          uAddress;                           /**< AUX Write start address                 */
   uint8                          *puReadDataBuffer;                   /**< Data buffer for return values           */
   uint32                          uReadSize;                          /**< Data size to be read in byte            */
   uint32                          uBytesRead;                         /**< How many bytes Read from FIFO           */   
   bool32                          bQueueOnly;                         /**< Only queue the command - do not send    */
} DP_ReadPacketType;

/*!
 * \struct DP_SetBondedModeConfigType
 *
 * The \b DP_SetBondedModeConfigType struct defines the DP bonded mode configuration info of the DP display.
 *
 */
typedef struct
{
  bool32             bEnabled;                    /**< True if DP is in bonded mode                          */
  DP_DeviceIDType    eSourceClockDeviceID;        /**< Device ID of DP Device sourcing the shared clock      */
} DP_SetBondedModeConfigType;

/*!
 * \struct DP_HostOpenConfigType
 *
 * The \b DP_HostOpenConfigType defines open parameters for DP_Host_Open().
 *
 */
typedef struct  
{
   DP_DeviceIDType                  eDeviceID;
   bool32                           bHPDActiveLow;                      /**< Indicate if HPD from panel in active low     */
   uint32                           uLaneSwingLevel;                    /**< Lane voltage swing level                     */
   uint32                           uPreemphasisLevel;                  /**< Lane pre-emphasis level                      */
   bool32                           bEnableSSCMode;                     /**< Enable SSC mode                              */
   bool32                           bAsynchronousMode;                  /**< Enable async mode for pixel rate & link rate */
   bool32                           bSkipHotPlugDetect;                 /**< Skip HPD: assume display is plugged in       */
   bool32                           bReadDPCD;                          /**< Flag to read DPCD from panel                 */
   bool32                           bReadEDID;                          /**< Flag to read EDID from panel                 */
   uint32                           uPowerupWaitinMs;                   /**< Time to wait after panel powering up         */
   uint32                           uMaxAuxRetry;                       /**< Number of retry before AUX I/O success.      */
   bool32                           bInverted;                          /**< Flag to specify usb type-c plug orientation  */
   DP_ConfigFlagType                eConfigFlags;                       /**< Configuration flags from DP_ConfigFlagType   */
   DP_SetBondedModeConfigType       sBondedModeConfig;                  /**< DP bonded mode configuration info            */
} DP_HostOpenConfigType;

/* Sink DPCD DSC Capability struct */
typedef struct
{                                                           
   /* dp dpcd spec for sink dsc capabilities */
   bool32                           bSupported;                          /* DSC supported          */
   uint32                           uMajor;                              /* DSC Version Major      */
   uint32                           uMinor;                              /* DSC Version Minor      */
   uint32                           uRCBufferBlockSize;                  /* DSC RC buffer block size      */
   uint32                           uSliceSupported;                     /* DSC Slice supported      */
   uint32                           uLineBufBitDepth;                    /* DSC Buffer Bit Depth       */
   bool32                           bBlockPrediction;                    /* DSC Block Prediction Support          */
   uint32                           uMaxBitsPerPixel;                    /* DSCBits per Pixel, u6.4 format      */
   uint32                           uColorFormatCap;                     /* DSC Color format supported      */
   uint32                           uColorDepthCap;                      /* DSC Color Depth supported, compression ratio  */
   uint32                           uPeakThroughputMode0;                /* DSC throughtput mode0, 4:4:4 and 4:2:2  */
   uint32                           uPeakThroughputMode1;                /* DSC throughtput mode1, 4:2:2 and 4:2:0  */
   uint32                           uMaxSliceWidth;                      /* DSC Max Slice Width      */
   uint32                           uBitsPerPixelIncrement;              /* DSC Bits per Pixel Increment      */

   /* Below are for local host */
   uint32                           ubpc;
   uint32                           ubpp;
   uint32                           uPixelPerClock;
   bool32                           bDTOEnable;
   uint32                           uCompressionRatio;
   uint32                           uSlicePerLine;
   uint32                           uSliceWidth;
   uint32                           uSliceHeight;
   uint32                           uChunkSize;
   uint32                           uPclk_dsc;                            /* Pixel clock freq in Khz   */
   uint32                           uWidth_dsc;                           /* Dcs total width           */
   uint32                           uHBP_dsc;                             /* Dcs Horizonatl back porch */ 
   void                            *pPPSBuf;                              /* Add it for PPS buffer     */
} DP_SinkDSCCapType;

/*!
* \b DP_PHYPowerType
*
*  Defines properties to power up DP PHY modules.
*/
typedef struct
{
  DP_BrightnessRequestType         eBrightnessRequest;                 /**< Backlight request enabled/disabled/level      */
  float                            fLevel;                             /**< Backlight level to be configured [0.0-1.0]    */
} DP_BrightnessControlType;

/*!
 * \struct DP_PHYResetType
 *
 * The \b DP_PHYResetType struct defines the DP PHY reset info.
 *
 */
typedef struct
{
  DP_DeviceIDType       eDeviceID;        /**< ID of DP Device PHY to reset */
  DP_PHYResetFlagType   ePHYResetFlag;    /**< Type of PHY reset            */
} DP_PHYResetType;

/*!
* \b DP_SetPropertyParamsType
*
*  Defines the union of all the types used for SetProperty
*/
typedef union
{
  bool32                           bEnabled;                           /**< DP_SETPROPERTY_SET_ASSR_ENABLE,        */
  uint32                           uModeIndex;                         /**< DP_SETPROPERTY_SET_PANEL_MODE_INDEX    */
  DP_PanelInfoType                 sPanelInfo;                         /**< DP_SETPROPERTY_SET_PANEL_INFO          */
  uint32                           uLinkRate;                          /**< DP_SETPROPERTY_SET_LINK_RATE           */
  uint32                           uLaneNumber;                        /**< DP_SETPROPERTY_SET_LANE_NUMBER         */
  DP_LinkTrainingType              eLinkTrainingType;                  /**< DP_SETPROPERTY_SET_LINK_TRAINING_TYPE  */
  DP_ClockConfigType               sClockConfig;                       /**< DP_SETPROPERTY_SET_CLOCKS              */
  bool32                           bConnected;                         /**< DP_SETPROPERTY_CONNECTIONSTATUS        */
  DP_SinkDSCCapType               *pSinkDSCCap;                        /**< DP_SETPROPERTY_DSC_PPS_INFO            */
  DP_BrightnessControlType         sBrightnessInfo;                    /**< DP_SETPROPERTY_BRIGHTNESS_CONTROL      */
  DP_PHYResetType                  sPHYResetInfo;                      /**< DP_SETPROPERTY_RESET_PHY               */
} DP_SetPropertyParamsType;

/*!
* \b EdidInfoType
*
* Structure captures RAW EDID information
*/
typedef struct
{
  uint32                                  uDataLength;                  /* Length of the EDID data block */
  uint8*                                  pDataBuf;                     /* Data buffer storing the raw EDID data */
} EdidInfoType;

/*!
* \b DP_GetPropertyParamsType
*
*  Defines the union of all the types used for GetProperty
*/
typedef union
{
  bool32                            bEnabled;                           /**< DP_GETPROPERTY_ASSR_ENABLE,            */
  DP_PanelInfoType                  sPanelInfo;                         /**< DP_GETPROPERTY_GET_MODE_INFO           */
  uint32                            uLinkRateInKhz;                     /**< DP_GETPROPERTY_LINK_RATE               */
  uint32                            uLaneNumber;                        /**< DP_GETPROPERTY_LANE_NUMBER             */
  bool32                            bAUXStatusOK;                       /**< DP_GETPROPERTY_AUXSTATUS               */
  bool32                            bPluggedIn;                         /**< DP_GETPROPERTY_CONNECTION_STATUS       */
  DP_SinkDSCCapType                *pSinkDSCCap;                        /**< DP_GETPROPERTY_DSC_SUPPORT             */
  EdidInfoType                      sEdidInfo;                          /**< DP_GETPROPERTY_EDID_INFO               */
  uint64                            uCRC;                               /**< DP_GETPROPERTY_CRC_INFO                */
} DP_GetPropertyParamsType;


/*!
* \b DP_PixelClk_Info
*
*  Defines the structure for pixel clock divider info
*/
typedef struct
{
  uint32                           uPixelClkMNDInputMhz;               /* Input frequency to MND RCG block          */
  uint32                           uPixelClk_M;                        /* M value for Pixel clock to MMSS_CC        */
  uint32                           uPixelClk_N;                        /* N value for Pixel clock to MMSS_CC        */
  uint32                           uPixelClk_2D;                       /* 2D Value for MMSS_CC                      */
  uint32                           uPixelDivider;                      /* divider for pixel clock to MMSS_CC        */
  uint32                           uSWMvid;                            /* SWMvid value to DP Sink                   */
  uint32                           uSWNvid;                            /* SWMvid value to DP Sink                   */  
} DP_PixelClk_Info;


#endif //_DP_TYPES_H_
