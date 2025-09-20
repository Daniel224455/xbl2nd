#ifndef _DP_I_H
#define _DP_I_H

/*=============================================================================
 
  File: dp_i.h
 
  Internal header file for DP Host
  
 
 Copyright (c) 2016-2020 Qualcomm Technologies, Inc.
 All Rights Reserved.
 Confidential and Proprietary - Qualcomm Technologies, Inc.
 =============================================================================*/

/*---------------------------------------------------------------------------------------------------------------------
 * Include Files
 *-------------------------------------------------------------------------------------------------------------------*/
#include "dp_types.h"
#include "dp_host.h"
#include "dp_spec.h"
#include "hal_dp.h"
#include "dp_system.h"
#include "edid.h"

/* -------------------------------------------------------------------------------------------------------------------- */
#define DISP_OSAL_Malloc(x1, x2)           MDP_OSAL_CALLOC(x1)
#define DISP_OSAL_Free(x1)                 MDP_OSAL_FREE(x1)
#define DISP_OSAL_MemZero(x1, x2)          MDP_OSAL_MEMZERO(x1, x2)
#define DISP_OSAL_MemSet(x1, x2, x3)       MDP_OSAL_MEMSET(x1, x2, x3)
#define DISP_OSAL_MemCpyEx(x1, l1, x2, l2) MDP_OSAL_MEMCPY(x1, x2, l2)

#define DISP_OSAL_LOG(fmt, ...)            MDP_Log_Message(MDP_LOGLEVEL_ERROR, fmt, ##__VA_ARGS__)
#define DISP_OSAL_LOG_WARN(fmt, ...)       MDP_Log_Message(MDP_LOGLEVEL_ERROR, fmt, ##__VA_ARGS__)
#define DISP_OSAL_LOG_INFO(fmt, ...)       MDP_Log_Message(MDP_LOGLEVEL_INFO, fmt, ##__VA_ARGS__)

#define DPHOST_GETFLAG(  _flag_, _bit_)    (((_flag_) & (_bit_)) ? 1: 0)
#define DPHOST_SETFLAG(  _flag_, _bit_)    ((_flag_) | ( _bit_))
#define DPHOST_CLEARFLAG(_flag_, _bit_)    ((_flag_) & (~(_bit_)))

/* --------------------------------------------------------------------------------------------------------------------
Enumerated types
---------------------------------------------------------------------------------------------------------------------*/

/* Dirty bits to indicate which configuration is changed and needs to be applied to hardware */
typedef enum
{
   DP_DIRTYFIELD_NONE                             = 0,
   DP_DIRTYFIELD_PANELMODE                        = DP_BIT(0),        /* Panel Mode has changed                    */
   DP_DIRTYFIELD_LINK_RATE                        = DP_BIT(1),        /* Desired Link rate changed                 */
   DP_DIRTYFIELD_SINK_ATTRIB                      = DP_BIT(2),        /* Sink attributes changed                   */
   DP_DIRTYFIELD_TU_PARAMETERS                    = DP_BIT(3),        /* TU related parameters changed             */
   DP_DIRTYFIELD_MAINLINK_PACKET                  = DP_BIT(4),        /* Main link attributes changed              */
   DP_DIRTYFIELD_STREAM_ATTRIB                    = DP_BIT(5),        /* Stream attributes changed                 */
   DP_DIRTYFIELD_LANE_NUMBER                      = DP_BIT(6),        /* Desired Lane Number changed               */
   DP_DIRTYFIELD_DPCD_CONFIG                      = DP_BIT(7),        /* DPCD config changed                       */
   DP_DIRTYFIELD_HPD_POLARITY                     = DP_BIT(8),        /* HPD Polarity changed                      */   
   DP_DIRTYFIELD_BPC                              = DP_BIT(8),        /* Bit depth changed                         */
} DP_DeviceDirtyFieldType;


/* Link training status types */
typedef enum
{    
   DP_LINK_TRAINING_STATUS_NONE                   = 0,
   DP_LINK_TRAINING_STATUS_CLK_RECOVERY_DONE      = DP_BIT(0),        /* clock recovery is done                    */
   DP_LINK_TRAINING_STATUS_CHANNEL_EQ_DONE        = DP_BIT(1),        /* Channel equalization is done              */
   DP_LINK_TRAINING_STATUS_SYMBOL_LOCKED          = DP_BIT(2),        /* Symbol is locked                          */
   DP_LINK_TRAINING_STATUS_INTERLANE_ALIGN_DONE   = DP_BIT(3),        /* Inter-lane align is done                  */ 
   DP_LINK_TRAINING_STATUS_MAX,
   DP_LINK_TRAINING_STATUS_32BIT                  = 0x7FFFFFFF
} DP_LinkTrainingStatusType;


typedef enum {
    DP_PANEL_COMMAND_NONE,                                /**< do nothing,  return not support */
    DP_PANEL_COMMAND_PPS,                                 /**< Send PPS command for DSC        */
    DP_PANEL_COMMAND_FORCE_32BIT           = 0x7FFFFFFF,
} dpPanelCommandType;


/* DP Host flags */
typedef enum
{
  DP_HOST_FLAG_NONE                               = 0,            /**< No host flag set                                 */
  DP_HOST_FLAG_ALT_MODE_FIXED_LANE_CONFIGURATION  = DP_BIT(0),    /**< Fixed lane configuration for alt-mode devices    */
  DP_HOST_FLAG_FORCE_32BIT                        = 0x7FFFFFFF
} DP_HostFlagType;


/* Shared resource configuration in DP bonded mode */
typedef enum
{
  DP_BONDED_MODE_SHARED_RESOURCE_NONE                   = 0x00000000,   /**< No shared resource                             */
  DP_BONDED_MODE_SHARED_RESOURCE_EXTERNAL_PIXEL_CLOCK,                  /**< Pixel clock sourced from another DP device     */
  DP_BONDED_MODE_SHARED_RESOURCE_MAX,
  DP_BONDED_MODE_SHARED_RESOURCE_FORCE_32BIT            = 0x7FFFFFFF
} DP_BondedModeSharedResourceType;


/* --------------------------------------------------------------------------------------------------------------------
** Macros and Struct
---------------------------------------------------------------------------------------------------------------------*/
#define MAX_AUX_CMD_BUFFER                         144

#ifdef DPDRIVER_EDP_MODE
/* 
 * in eDP mode we only need to parse the internal panel's EDID
 * which will be limited to the first EDID block.
 */
#define DP_MAX_NUM_CONTEXTS                        1
#define DPHOST_GET_DEVICE_CTX(_deviceId_)          (&(gDpSharedData.sDeviceCtx[0]))
#define DP_EDID_MAX_NUM_OF_BLOCKS                  (0x01)   //Maximum number of pages for EDID structure
#else
#define DP_MAX_NUM_CONTEXTS                        DP_DEVICE_MAX 
#define DPHOST_GET_DEVICE_CTX(_deviceId_)          (&(gDpSharedData.sDeviceCtx[((_deviceId_)<DP_DEVICE_MAX)?(_deviceId_):DP_DEVICE_ID_ALT_MODE_0]))
#define DP_EDID_MAX_NUM_OF_BLOCKS                  (0x80)   //Maximum number of pages for EDID structure
#endif

#define DP_EDID_START_ADDRESS                      (0x50)   //Start address of Display Port EDID base blocks.
#define DP_EDID_BLOCK_SIZE                         (0x80)   //Each page size in the EDID ROM

#define DP_EDID_MAX_BUF_SIZE                       (DP_EDID_BLOCK_SIZE * DP_EDID_MAX_NUM_OF_BLOCKS)
#define DP_EDID_EXT_BLOCK_NUM_OFFSET               (0x7E)
#define DP_EDID_SEGMENT_ADDRESS                    (0x30)

/* Link rates supported by DP Host */
#define DP_LINK_SYMBOL_RATE_KHZ_PER_LANE_162MBps   162000
#define DP_LINK_SYMBOL_RATE_KHZ_PER_LANE_270MBps   270000
#define DP_LINK_SYMBOL_RATE_KHZ_PER_LANE_540MBps   540000
#define DP_LINK_SYMBOL_RATE_KHZ_PER_LANE_810MBps   810000
#define DP_PLL_OUTPUT_RATE_810MHZ                  810
#define DP_PLL_OUTPUT_RATE_1350MHZ                 1350

#define DP_MAX_RESOLUTION_WIDTH                    4096
#define DP_MAX_RESOLUTION_HEIGHT                   4096
#define DP_MAX_REFRESH_RATE_HZ                     120


#define MAX_DP_PANEL_MODE                          24

/* Get the color info from DP_PixelFormatType */
#define DPHOST_GETPIXELFMT(_pixelFmt_)             (gsDPPixelFormatInfo[((_pixelFmt_)<DP_PIXEL_FORMAT_MAX)?(_pixelFmt_):DP_PIXEL_FORMAT_NONE])
#define DPHOST_GETCTX_FROM_HANDLE(hHandle)         ((DP_DeviceCtxType*) hHandle)

#define DPHOST_GETFLAG(  _flag_, _bit_)            (((_flag_) & (_bit_)) ? 1: 0)
#define DPHOST_SETFLAG(  _flag_, _bit_)            ((_flag_) | ( _bit_))
#define DPHOST_CLEARFLAG(_flag_, _bit_)            ((_flag_) & (~(_bit_)))

/* Convert a float _value_ in the range 0.0 - 1.0 to an integer value in the _range_ */
#define DPHOST_FLOAT_TO_INT(_value_,_range_)       (((_value_) < 1.0f) ? (uint32)(((_value_) * (_range_)) + 0.5f) : (_range_))

/* --------------------------------------------------------------------------------------------------------------------
** Preprocessor Definitions and Constants
** ----------------------------------------------------------------------------------------------------------------- */

/* color info */
typedef struct 
{
   uint32                   uBitsPerPixel;                      /* Bits per pixel                                   */
   uint32                   uBitsPerComponent;                  /* Bits per each color component (R/G/B, Y/U/V)     */
} DP_PixelFormat_Info;

/* DP bonded mode configuration info */
typedef struct
{
  bool32                           bEnabled;               /* Is the DP Device part of a bonded mode configuration                     */
  DP_BondedModeSharedResourceType  eSharedResourceConfig;  /* Specifies the resource to be shared by the DP Devices in bonded mode     */
  DP_DeviceIDType                  eSourceClockDeviceID;   /* Device ID of DP Device sourcing the shared clock                         */
} DP_BondedModeConfigType;

/* Present Device context for DP device */
typedef struct 
{
  DP_DeviceIDType           eDeviceID;                          /* Which DP device in case there should be multiple */
  bool32                    bInitialized;                       /* Is device handle initialized                     */
  bool32                    bLinkConfigured;                    /* Is link configured                               */
  uint32                    uNumLanes;                          /* Number of lanes enabled : 1, 2 or 4              */
  uint32                    uLinkRateInKhz;                     /* Link rate                                        */
  uint32                    uPixelClkInKhz;                     /* Pixel clk                                        */
  bool32                    bSynchronousClk;                    /* Are pixel clk and link clk in synchronous mode   */
  bool32                    bEnableSSCMode;                     /* Enable spread spectrum clocking (downspread)     */
  uint32                    uModeIndex;                         /* Present mode                                     */
  bool32                    bPluggedIn;                         /* Is external display connected                    */
  bool32                    bInvertedPlug;                      /* USB type-C plug orientation (TRUE:porselect = 1) */
  bool32                    bEDIDObtained;                      /* Has EDID been read from display                  */ 
  uint32                    uNumModes;                          /* Number of modes supported by display             */
  DP_PanelInfoType          sPanelInfo[MAX_DP_PANEL_MODE];      /* Panel info for all the display modes             */
  DP_DPCDSinkCapType        sSinkDPCDCap;                       /* Sink capabilities read from DPCD                 */
  DP_LinkTrainingType       eLinkTrainingType;                  /* Type of link training requested by user          */
  uint32                    uVoltageSwingLevel;                 /* Current voltage swing level set to Sink          */
  uint32                    uPreEmphasisLevel;                  /* Current pre-emphasis level set to Sink           */
  uint32                    uMaxVoltSwingLvl;                   /* Max voltage swing level supported by sink        */
  uint32                    uMaxPreemphLvl;                     /* Max pre-emphasis level supported by sink         */
  uint32                    uDirtyBits;                         /* Which config has changed                         */
  uint8                     uAUXCmdBuffer[MAX_AUX_CMD_BUFFER];  /* Shared buffer for AUX Cmd Buffer                 */
  uint32                    uAUXCmdBufferCurrentSize;           /* Current queued Command size in Cmd buffer        */
  uint32                    uAUXNumOfTransaction;               /* Current number of AUX Transaction                */
  DP_TransactionModeType    eAUXTransactionTypeInQueue;         /* Current AUX Transaction type in CMD FIFO         */
  uint32                    uMaxAUXCmdFifoLen;                  /* The Max AUX CMD FIFO length                      */
  DP_EDIDDataType          *sSinkEDIDData;                      /* DP sink EDID data                                */
  uint8                     auEDIDCache[DP_EDID_MAX_BUF_SIZE];  /* DP Sink EDID data                                */
  uint32                    uEDIDDataLength;                    /* Length of EDID data                              */
  EDID_VendorInfoType       sVendorInfo;                        /* Vendor specific info                             */
  uint32                    uSinkCount;                         /* Sink count on connected port                     */
  bool32                    bCPReady;                           /* Indicate when all Sink devices are CP-capable    */
  DP_ControllerIDType       eControllerID;                      /* DP controller index                              */
  bool32                    bLinkLost;                          /* Link lost                                        */
  uint32                    uBitDepth;                          /* Sink device bit depth                            */
  bool32                    bDSCSupported;                      /* Dsc supported by panel                           */
  uint32                    uPixelPerClock;                     /* Dp interface transfer multiple pixel per pclk    */
  bool32                    bDTOEnable;                         /* True if DTO enabled                              */
  DP_PSRType                sPSR;                               /* PSR info                                         */
  DP_SinkDSCCapType         sDSC;                               /* DSC info                                         */
  DP_SinkFECCapType         sFEC;                               /* FEC info                                         */
  uint8                     aSinkStatus[6];                     /* Raw sink status DPCD registers 0x0202~0x0207     */
  uint8                     auPPSBuffer[248];                   /* DP Sink DSC PPS buffer                           */
  DP_SinkBrightnessCapType  sBrightnessCaps;                    /* Brightness capabilities of sink                  */
  DP_BondedModeConfigType   sBondedModeConfig;                  /* Bonded mode configuration info                   */
  uint32                    uFlags;                             /* Defines DP related flags specified in
                                                                   DP_HostFlagType enum                             */
} DP_DeviceCtxType;

/* Transfer Unit related params */
typedef struct
{
   uint32                   uVisWidth;                          /* Screen width                                     */
   uint32                   uPixelClockInKhz;                   /* Pixel clock freq in Khz                          */
   uint32                   uLinkClockInKhz;                    /* Link clock freq in Khz                           */
   uint32                   uNumberOfLanes;                     /* Number of Lanes in use                           */
   uint32                   uBPP;                               /* Bits per pixel                                   */
   bool32                   bSynchronousClk;                    /* Pixel and Link clock are synchronous             */
} DP_TUSizePararmType;

/* Device context for all the devices */
typedef struct 
{
  bool32                    bInitialized;                           /* Flag indicating if DP driver is initialized      */
  DP_DeviceCtxType          sDeviceCtx[DP_MAX_NUM_CONTEXTS];        /* Present context of this device                   */
  int32                     uSourceClockRefCount[DP_DEVICE_MAX];    /* Ref counts for clock sources; required for
                                                                       bookkeeping in bonded mode configurations        */
} DP_DeviceDataType;

/* Struct used for dynamic controller availability */
typedef struct
{
  DP_ControllerIDType eControllerID; /**< Controller ID.                   */
  bool32              bInUse;        /**< TRUE if controller is allocated. */
} DP_ControllerInfoType;


/*---------------------------------------------------------------------------------------------------------------------
 * Global Data Definitions
 *-------------------------------------------------------------------------------------------------------------------*/
extern DP_PixelFormat_Info                    gsDPPixelFormatInfo[DP_PIXEL_FORMAT_MAX];
extern DP_DeviceDataType                      gDpSharedData;




/**********************************************************************************************************************

FUNCTION: DP_LinkTraining()
   
   This function will read from AUX/I2c channel 

Parameters:
   pDeviceCtx                       -[in ] Pointer to device context
   eLinkTrainingType                -[in ] The type of Link training
   uFlags                           -[in ] Reserved

Return: 
   DP_Status

**********************************************************************************************************************/
DP_Status DP_LinkTraining(DP_DeviceCtxType         *pDeviceCtx,
                          DP_LinkTrainingType       eLinkTrainingType,
                          DP_FlagsType              uFlags);


/**********************************************************************************************************************

FUNCTION: DP_ReadSinkDPCDCap()
   
   This function will read Sink Capability Data from DPCD over AUX Channel

Parameters:
   pDeviceCtx                       -[in ] Pointer to device context
   psDPCDSinkCap                    -[out] Pointer to DPCD Sink Capability Data struct.

Return: 
   DP_Status

**********************************************************************************************************************/
DP_Status DP_ReadSinkDPCDCap(DP_DeviceCtxType    *pDeviceCtx,
                             DP_DPCDSinkCapType  *psDPCDSinkCap);

/**********************************************************************************************************************

FUNCTION: DP_ReadSinkDPCDStatus()
   
   This function will read Sink Status Raw Data from DPCD over AUX Channel

Parameters:
   pDeviceCtx                       -[in ] Pointer to device context
   psDPCDSinkStatus                 -[out] Pointer to DPCD Sink Status Data struct.

Return: 
   DP_Status

**********************************************************************************************************************/
DP_Status DP_ReadSinkDPCDStatus(DP_DeviceCtxType       *pDeviceCtx,
                                DP_DPCDSinkStatusType  *psDPCDSinkStatus);

/**********************************************************************************************************************

FUNCTION: DP_ReadSinkPSRCap()
   
   This function will read Sink PSR Capability over AUX Channel

Parameters:
   pDeviceCtx                       -[in ] Pointer to device context

Return: 
   DP_Status

**********************************************************************************************************************/
DP_Status  DP_ReadSinkPSRCap(DP_DeviceCtxType      *pDeviceCtx);

/**********************************************************************************************************************

FUNCTION: DP_ReadSinkPSRStatus()

This function will read sink PSR status register for sink count and CP readiness. 

Parameters:
   pDeviceCtx                       -[in ] Pointer to device context

Return:
DP_Status

**********************************************************************************************************************/
DP_Status  DP_ReadSinkPSRStatus(DP_DeviceCtxType      *pDeviceCtx);

/**********************************************************************************************************************

FUNCTION: DP_WriteSinkPSRConfiguration()

This function will write sink PSR configuration register (0x170) for sink count and CP readiness. 

Parameters:
   pDeviceCtx                       -[in ] Pointer to device context

Return:
DP_Status

**********************************************************************************************************************/
DP_Status  DP_WriteSinkPSRCOnfiguration(DP_DeviceCtxType      *pDeviceCtx);

/**********************************************************************************************************************

FUNCTION: DP_ReadSinkDSCCap()
   
   This function will read Sink DSC Capability over AUX Channel

Parameters:
   pDeviceCtx                       -[in ] Pointer to device context

Return: 
   DP_Status

**********************************************************************************************************************/
DP_Status  DP_ReadSinkDSCCap(DP_DeviceCtxType      *pDeviceCtx);

/**********************************************************************************************************************

FUNCTION: DP_ReadSinkDSCStatus()

This function will read sink DSC Status

Parameters:
   pDeviceCtx                       -[in ] Pointer to device context

Return:
DP_Status

**********************************************************************************************************************/
DP_Status  DP_ReadSinkDSCStatus(DP_DeviceCtxType      *pDeviceCtx);


/**********************************************************************************************************************

FUNCTION: DP_ReadSinkDSCEnable()

This function will read sink DSC Enable Status

Parameters:
   pDeviceCtx                       -[in ] Pointer to device context

Return:
DP_Status

**********************************************************************************************************************/
DP_Status  DP_ReadSinkDSCEnable(DP_DeviceCtxType      *pDeviceCtx);


/**********************************************************************************************************************

FUNCTION: DP_WriteSinkDSCEnable()

This function will write sink DSC enable. 

Parameters:
   pDeviceCtx                       -[in ] Pointer to device context

Return:
DP_Status

**********************************************************************************************************************/
DP_Status  DP_WriteSinkDSCEnable(DP_DeviceCtxType      *pDeviceCtx);

/**********************************************************************************************************************

FUNCTION: DP_ReadSinkFECCap()
   
   This function will read Sink FEC Capability over AUX Channel

Parameters:
   pDeviceCtx                       -[in ] Pointer to device context

Return: 
   DP_Status

**********************************************************************************************************************/
DP_Status  DP_ReadSinkFECCap(DP_DeviceCtxType      *pDeviceCtx);

/**********************************************************************************************************************

FUNCTION: DP_ReadSinkFECStatus()

This function will read sink FEC Status

Parameters:
   pDeviceCtx                       -[in ] Pointer to device context

Return:
DP_Status

**********************************************************************************************************************/
DP_Status  DP_ReadSinkFECStatus(DP_DeviceCtxType      *pDeviceCtx);
DP_Status  DP_ReadSinkFECEnable(DP_DeviceCtxType      *pDeviceCtx);

/**********************************************************************************************************************

FUNCTION: DP_WriteSinkFECConfig()

This function will write sink FEC config. 

Parameters:
   pDeviceCtx                       -[in ] Pointer to device context

Return:
DP_Status

**********************************************************************************************************************/
DP_Status  DP_WriteSinkFECConfig(DP_DeviceCtxType      *pDeviceCtx);

/**********************************************************************************************************************

FUNCTION: DP_WriteSinkASSR()

This function will write sink PSR configuration register (0x170) for sink count and CP readiness. 

Parameters:
   pDeviceCtx                       -[in ] Pointer to device context

Return:
DP_Status

**********************************************************************************************************************/
DP_Status  DP_WriteSinkASSR(DP_DeviceCtxType      *pDeviceCtx, uint32 uEnable);

/**********************************************************************************************************************

FUNCTION: DP_ReadSinkCRC()

This function will Read sink PSR configuration register (0x170) for sink count and CP readiness. 

Parameters:
   pDeviceCtx                       -[in ] Pointer to device context

Return:
DP_Status

**********************************************************************************************************************/
DP_Status  DP_ReadSinkCRC(DP_DeviceCtxType      *pDeviceCtx);

/**********************************************************************************************************************

FUNCTION: DP_ReadSinkPSRConfiguration()

This function will read sink PSR configuration register (0x170) for sink count and CP readiness. 

Parameters:
   pDeviceCtx                       -[in ] Pointer to device context

Return:
DP_Status
**********************************************************************************************************************/
DP_Status  DP_ReadSinkPSRConfiguration(DP_DeviceCtxType      *pDeviceCtx);

/**********************************************************************************************************************
*
** FUNCTION: DP_Host_SDP_ClientWrite()
*/
/*!
* \DESCRIPTION
*    DP Host send meta data to client (panel)
*
* \param [in]   hHandle        - Logical handle of DP Device
*
* \retval DP_Status
*
**********************************************************************************************************************/
MDP_Status DP_Host_SDP_ClientWrite(DP_HandleType hDPHandle, uint32 eCommand, void *pPayload, uint32 uFlags);


/**********************************************************************************************************************

FUNCTION: DP_WriteSinkDPCDLink()
   
   This function will Write the link setting to panel DPCD 

Parameters:
   pDeviceCtx                       -[in ] Pointer to device context

Return: 
   DP_Status

**********************************************************************************************************************/
DP_Status DP_WriteSinkDPCDLink(DP_DeviceCtxType    *pDeviceCtx);

/**********************************************************************************************************************

FUNCTION: DP_WriteSinkDPCDTrainingPattern()
   
   This function will select the training pattern for link training to panel DPCD 

Parameters:
   pDeviceCtx                       -[in ] Pointer to device context
   eTraingPattern                   -[in ] Link Training pattern 

Return: 
   DP_Status

**********************************************************************************************************************/
DP_Status DP_WriteSinkDPCDTrainingPattern(DP_DeviceCtxType               *pDeviceCtx,
                                          DP_DPCDTrainingPatternSetType   eTraingPattern);

/**********************************************************************************************************************

FUNCTION: DP_WriteSinkDPCDLevels()
   
   This function will select the training Voltage Swing Level and Pre-emphasis Level.

Parameters:
   pDeviceCtx                       -[in ] Pointer to device context
   uDesiredVoltageSwingLevel        -[in ] The desired Voltage Swing level for link training
   uDesiredPreEmphasisLevel         -[in ] The desired pre-emphasis level for link training

Return: 
   DP_Status

**********************************************************************************************************************/
DP_Status DP_WriteSinkDPCDLevels(DP_DeviceCtxType    *pDeviceCtx,
                                 uint32               uDesiredVoltageSwingLevel,
                                 uint32               uDesiredPreEmphasisLevel);

/**********************************************************************************************************************

FUNCTION: DP_WriteSinkDPCDPower()

   This function will set the sink power state. i.e Power on/off for main link or AUX

Parameters:
   pDeviceCtx                       -[in ] Pointer to device context
   ePowerState                      -[in ] Sink power state to be set

Return:
   DP_Status

**********************************************************************************************************************/
DP_Status DP_WriteSinkDPCDPower(DP_DeviceCtxType      *pDeviceCtx,
                                DP_DPCDPowerStateType  ePowerState);

/**********************************************************************************************************************

FUNCTION: DP_WriteSinkDPCDDownSpreadEnabled()
   
   This function will indicate that downspread has been enabled on host side.

Parameters:
   pDeviceCtx                       -[in ] Pointer to device context

Return: 
   DP_Status 

**********************************************************************************************************************/
DP_Status DP_WriteSinkDPCDDownSpreadEnabled(DP_DeviceCtxType    *pDeviceCtx);

/**********************************************************************************************************************

FUNCTION: DP_ParseSinkEDID()
   
   This function will parse the EDID info from the Raw EDID Data. 

Parameters:
   pDeviceCtx                       -[in ] Pointer to device context

Return:
   DP_Status

**********************************************************************************************************************/
DP_Status  DP_ParseSinkEDID(DP_DeviceCtxType    *pDeviceCtx);

/**********************************************************************************************************************

FUNCTION: DP_ReadSinkEDIDRaw()

   This function will read EDID Raw Data from Sink over I2c.

Parameters:
   pDeviceCtx                         -[in] Pointer to device context

Return:
   DP_Status

**********************************************************************************************************************/
DP_Status  DP_ReadSinkEDIDRaw(DP_DeviceCtxType    *pDeviceCtx);

/**********************************************************************************************************************

FUNCTION: DP_CalculateTU()
   
   This function will calculate the TU based the the lane #, link clock, pixel clock, color format. 

Parameters:
   psDisplayInfo                    -[in ] Panel info
   psTUConfig                       -[out] The TU parameters 

Return: 
   DP_Status

**********************************************************************************************************************/
DP_Status DP_CalculateTU(DP_TUSizePararmType             *psDisplayInfo,
                         HAL_DP_TUConfigType             *psTUConfig);

/**********************************************************************************************************************

FUNCTION: DP_CalculateLink()
   
   This function will Calculate the Link settings, such as clock freq, number of lane needed.

Parameters:
   pDeviceCtx                       -[in ] Pointer to device context
   psDPCDSinkCap                    -[in ] Pointer to sink DPCD capabilites
   psPanelInfo                      -[in ] Pointer to panel info corresponding to current mode
   pPixClkInfo                      -[out] Pointer to pixel clock info (dividers, etc.)

Return: 
   DP_Status 

**********************************************************************************************************************/
DP_Status  DP_CalculateLink(DP_DeviceCtxType         *pDeviceCtx,
                            DP_DPCDSinkCapType       *psDPCDSinkCap,
                            DP_PanelInfoType         *psPanelInfo,
                            DP_PixelClk_Info         *pPixClkInfo);

/**********************************************************************************************************************

FUNCTION: DP_CalculateMisc0()
   
   This function will calculate the MISCO value according to DP Spec 

Parameters:
   eColorFormat                     -[in ] Sink color format

Return: 
   uint8 Misc0 value 

**********************************************************************************************************************/
uint8  DP_CalculateMisc0(DP_PixelFormatType eColorFormat );

/**********************************************************************************************************************

FUNCTION: DP_ClientWrite()
   
   This API allows users to send the DP command out to DP Panels. 

Parameters:
   pDeviceCtx                       -[in ] Pointer to device context
   pWritePacket                     -[out] Command package to be sent out to DP panels

Return: 
   DP_Status

**********************************************************************************************************************/
DP_Status DP_Host_ClientWrite(DP_DeviceCtxType    *pDeviceCtx,
                              DP_WritePacketType  *psWritePacket);

/**********************************************************************************************************************

FUNCTION: DP_Host_ClientRead()
   
   This function will read from AUX/I2c channel 

Parameters:
   pDeviceCtx                       -[in ] Pointer to device context
   psReadPacket                     -[out] the read packet

Return: 
   DP_Status

**********************************************************************************************************************/
DP_Status DP_Host_ClientRead(DP_DeviceCtxType    *pDeviceCtx,
                             DP_ReadPacketType   *psReadPacket);


/**********************************************************************************************************

FUNCTION: DP_CheckTrainingResult()
   
   This function will get the max (among all lanes) drive setting requested by the sink 

Parameters:
   pDeviceCtx                         -[in ] Pointer to device context
   psDPCDSinkStatus                   -[in ] Pointer to DPCP sink status
   eTrainingType                      -[in ] type of link training done

Return: 
   bool32                             - TRUE = training successful 

************************************************************************************************************/
bool32  DP_CheckTrainingResult(DP_DeviceCtxType            *pDeviceCtx,
                               DP_DPCDSinkStatusType       *psDPCDSinkStatus,
                               DP_LinkTrainingStatusType    eTrainingType);

/**********************************************************************************************************************

FUNCTION: DP_ReadSinkBrightnessCap()

This function will read Sink Brightness Capability over AUX Channel

Parameters:
  pDeviceCtx                        -[in ] Pointer to device context

Return:
  DP_Status

**********************************************************************************************************************/
DP_Status  DP_ReadSinkBrightnessCap(DP_DeviceCtxType *pDeviceCtx);

/**********************************************************************************************************************

FUNCTION: DP_WriteSinkBrightnessEnable()

Function to enable/disable panel brightness

Parameters:
  pDeviceCtx                      -[in ] Pointer to device context
  bEnable                         -[in ] TRUE to enable backlight

Return:
  DP_Status

**********************************************************************************************************************/
DP_Status  DP_WriteSinkBrightnessEnable(DP_DeviceCtxType *pDeviceCtx,
                                        bool32            bEnable);

/**********************************************************************************************************************

FUNCTION: DP_WriteSinkBrightnessLevel()

This function will write Sink Brightness level over AUX Channel to only MSB (722h) 
or both MSB & LSB (722h&723h) based on caps

Parameters:
  pDeviceCtx                      -[in ] Pointer to device context
  fLevel                          -[in ] Backlight level to set

Return:
  DP_Status

**********************************************************************************************************************/
DP_Status  DP_WriteSinkBrightnessLevel(DP_DeviceCtxType *pDeviceCtx,
                                       float             fLevel);

#endif // _DP_I_H
