/*=============================================================================
 
  File: MDPEDID.c
 
  Source file for MDP panel detection and configuration
  
 
  Copyright (c) 2011-2020 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential.
=============================================================================*/
/*=========================================================================
      Include Files
==========================================================================*/

#include <Library/PcdLib.h>
#include <Library/QcomTargetLib.h>
#include "MDPTypes.h"
#include "MDPLib_i.h"
#include "MDPPlatformLib.h"
#include "MDPSystem.h"
#include "xmlparser.h"
#include "xmltags.h"
#include "string.h"
#include "MDPLib.h"


/*=========================================================================
      Local Defines
==========================================================================*/



/*=========================================================================
      Local Tables
==========================================================================*/

/* Panel configuration for each interface on the platform 
 */
static MDP_Panel_AttrType gPanelConfig;
static uint8       gDSIContorllerMapping[16];
static uint8       gDPDeviceIDMapping[16];
uint8             *gpDSIInitSequenceBuffer[MDP_DISPLAY_MAX] = {NULL};
uint8             *gpDSITermSequenceBuffer[MDP_DISPLAY_MAX] = {NULL};
uint8             *gpDSIDscPpsBuffer[MDP_DISPLAY_MAX]       = {NULL};
uint8             *gpI2CInitSequenceBuffer[MDP_DISPLAY_MAX] = {NULL};
uint8             *gpI2CTermSequenceBuffer[MDP_DISPLAY_MAX] = {NULL};

/* XML Tag parser
 */
XML_TagDefType sXmlTagsList[] = {

    // Display Panel interface configuration
    XML_CREATE_TAG_ENTRY("InterfaceType", XML_TAG_DATATYPE_INTEGER, gPanelConfig.ePhysConnect),
    XML_CREATE_TAG_ENTRY("PanelOrientation", XML_TAG_DATATYPE_INTEGER, gPanelConfig.ePhysPanelOrientation),

    XML_CREATE_TAG_ENTRY("InterfaceColorFormat", XML_TAG_DATATYPE_INTEGER, gPanelConfig.eColorFormat),

    XML_CREATE_TAG_ENTRY("HorizontalFrontPorch", XML_TAG_DATATYPE_INTEGER, gPanelConfig.sActiveTiming.uHsyncFrontPorchDclk),
    XML_CREATE_TAG_ENTRY("HorizontalBackPorch", XML_TAG_DATATYPE_INTEGER, gPanelConfig.sActiveTiming.uHsyncBackPorchDclk),
    XML_CREATE_TAG_ENTRY("HorizontalSyncPulse", XML_TAG_DATATYPE_INTEGER, gPanelConfig.sActiveTiming.uHsyncPulseWidthDclk),
    XML_CREATE_TAG_ENTRY("HorizontalSyncSkew", XML_TAG_DATATYPE_INTEGER, gPanelConfig.sActiveTiming.uHsyncSkewDclk),

  
    XML_CREATE_TAG_ENTRY("HorizontalActive", XML_TAG_DATATYPE_INTEGER, gPanelConfig.uDisplayWidth),
    XML_CREATE_TAG_ENTRY("VerticalActive", XML_TAG_DATATYPE_INTEGER, gPanelConfig.uDisplayHeight),

    XML_CREATE_TAG_ENTRY("VerticalFrontPorch", XML_TAG_DATATYPE_INTEGER, gPanelConfig.sActiveTiming.uVsyncFrontPorchLines),
    XML_CREATE_TAG_ENTRY("VerticalBackPorch", XML_TAG_DATATYPE_INTEGER, gPanelConfig.sActiveTiming.uVsyncBackPorchLines),
    XML_CREATE_TAG_ENTRY("VerticalSyncPulse", XML_TAG_DATATYPE_INTEGER, gPanelConfig.sActiveTiming.uVsyncPulseWidthLines),

    XML_CREATE_TAG_ENTRY("HorizontalLeftBorder",  XML_TAG_DATATYPE_INTEGER, gPanelConfig.sActiveTiming.uHLeftBorderDClk),    
    XML_CREATE_TAG_ENTRY("HorizontalRightBorder", XML_TAG_DATATYPE_INTEGER, gPanelConfig.sActiveTiming.uHRightBorderDClk),
    XML_CREATE_TAG_ENTRY("VerticalTopBorder",     XML_TAG_DATATYPE_INTEGER, gPanelConfig.sActiveTiming.uVTopBorderLines),
    XML_CREATE_TAG_ENTRY("VerticalBottomBorder",  XML_TAG_DATATYPE_INTEGER, gPanelConfig.sActiveTiming.uVBottomBorderLines),   
    XML_CREATE_TAG_ENTRY("BorderColor",           XML_TAG_DATATYPE_INTEGER, gPanelConfig.sActiveTiming.uBorderColorRgb888),   
    XML_CREATE_TAG_ENTRY("UnderflowColor",         XML_TAG_DATATYPE_INTEGER, gPanelConfig.uUnderflowColor),

    XML_CREATE_TAG_ENTRY("InvertDataPolarity", XML_TAG_DATATYPE_BOOLEAN, gPanelConfig.sActiveTiming.uDataEnInvertSignal),
    XML_CREATE_TAG_ENTRY("InvertVsyncPolarity", XML_TAG_DATATYPE_BOOLEAN, gPanelConfig.sActiveTiming.uVsyncInvertSignal),  
	// Typo in InvertVsyncPolairty, use correct spelling instead. In process of deprecation. 
    XML_CREATE_TAG_ENTRY("InvertVsyncPolairty", XML_TAG_DATATYPE_BOOLEAN, gPanelConfig.sActiveTiming.uVsyncInvertSignal),    
    XML_CREATE_TAG_ENTRY("InvertHsyncPolarity", XML_TAG_DATATYPE_BOOLEAN, gPanelConfig.sActiveTiming.uHsyncInvertSignal),
    XML_CREATE_TAG_ENTRY("PanelId", XML_TAG_DATATYPE_INTEGER, gPanelConfig.uPanelId),

    // DSI Common Parameters    
    XML_CREATE_TAG_ENTRY("DSIRefreshRate", XML_TAG_DATATYPE_INTEGER, gPanelConfig.uAttrs.sDsi.uRefreshRate),
    XML_CREATE_TAG_ENTRY("DSIDynamicRefreshRates", XML_TAG_DATATYPE_HEX_LIST , gPanelConfig.uAttrs.sDsi.uDynamicRefreshRates),
    XML_CREATE_TAG_ENTRY("DSIBitClockFrequency", XML_TAG_DATATYPE_INTEGER, gPanelConfig.uAttrs.sDsi.uBitClockFrequency),    
    XML_CREATE_TAG_ENTRY("DSILanes", XML_TAG_DATATYPE_INTEGER, gPanelConfig.uAttrs.sDsi.uDataLaneNum),
    XML_CREATE_TAG_ENTRY("DSICPhyMode", XML_TAG_DATATYPE_BOOLEAN, gPanelConfig.uAttrs.sDsi.bCPhyMode),
    XML_CREATE_TAG_ENTRY("DSIChannelId", XML_TAG_DATATYPE_INTEGER, gPanelConfig.uAttrs.sDsi.eChannelId),
    XML_CREATE_TAG_ENTRY("DSIVirtualId", XML_TAG_DATATYPE_INTEGER, gPanelConfig.uAttrs.sDsi.eDisplayVC),
    XML_CREATE_TAG_ENTRY("DSIColorFormat", XML_TAG_DATATYPE_INTEGER, gPanelConfig.uAttrs.sDsi.eColorFormat),
    XML_CREATE_TAG_ENTRY("DSIPacketTransferHS", XML_TAG_DATATYPE_BOOLEAN, gPanelConfig.uAttrs.sDsi.bPacketTransferHS),
    XML_CREATE_TAG_ENTRY("DSIClockHSForceRequest", XML_TAG_DATATYPE_INTEGER, gPanelConfig.uAttrs.sDsi.uClockHSForceRequest),
    XML_CREATE_TAG_ENTRY("DSIHostLaneMapping", XML_TAG_DATATYPE_INTEGER, gPanelConfig.uAttrs.sDsi.uLaneRemap),
    XML_CREATE_TAG_ENTRY("DSILP11AtInit", XML_TAG_DATATYPE_BOOLEAN, gPanelConfig.uAttrs.sDsi.bLP11AtInit),    
    XML_CREATE_TAG_ENTRY("DSIPhyDCDCMode",        XML_TAG_DATATYPE_BOOLEAN, gPanelConfig.uAttrs.sDsi.bPhyDCDCMode),
    XML_CREATE_TAG_ENTRY("DSIDisableEoTAfterHSXfer", XML_TAG_DATATYPE_BOOLEAN, gPanelConfig.uAttrs.sDsi.bDisableEotpAfterHSXfer),
    XML_CREATE_TAG_ENTRY("DSIControllerMapping", XML_TAG_DATATYPE_BINARY, gDSIContorllerMapping),
    XML_CREATE_TAG_ENTRY("DSINullpacketInsertionBytes", XML_TAG_DATATYPE_INTEGER, gPanelConfig.uAttrs.sDsi.uNullPacketInsertionLength),
    XML_CREATE_TAG_ENTRY("DSIInterfaceMaster", XML_TAG_DATATYPE_INTEGER, gPanelConfig.uAttrs.sDsi.uDsiInterfaceMaster),
    XML_CREATE_TAG_ENTRY("DSIFlags", XML_TAG_DATATYPE_INTEGER, gPanelConfig.uAttrs.sDsi.uFlags),
    
    //DSC compression
    XML_CREATE_TAG_ENTRY("DSIDSCEnable", XML_TAG_DATATYPE_BOOLEAN, gPanelConfig.uAttrs.sDsi.bDSCEnable),
    XML_CREATE_TAG_ENTRY("DSIDSCMajor", XML_TAG_DATATYPE_INTEGER, gPanelConfig.uAttrs.sDsi.uDSCMajor),
    XML_CREATE_TAG_ENTRY("DSIDSCMinor", XML_TAG_DATATYPE_INTEGER, gPanelConfig.uAttrs.sDsi.uDSCMinor),
    XML_CREATE_TAG_ENTRY("DSIDSCScr", XML_TAG_DATATYPE_INTEGER, gPanelConfig.uAttrs.sDsi.uDSCScr),
    XML_CREATE_TAG_ENTRY("DSIDSCProfileID", XML_TAG_DATATYPE_INTEGER, gPanelConfig.uAttrs.sDsi.uDSCProfileID),
    XML_CREATE_TAG_ENTRY("DSIDSCSliceHeight", XML_TAG_DATATYPE_INTEGER, gPanelConfig.uAttrs.sDsi.uDSCSliceHeight),
    XML_CREATE_TAG_ENTRY("DSIDSCSliceWidth", XML_TAG_DATATYPE_INTEGER, gPanelConfig.uAttrs.sDsi.uDSCSliceWidth),
    XML_CREATE_TAG_ENTRY("DSIDSCSlicePerPacket", XML_TAG_DATATYPE_INTEGER, gPanelConfig.uAttrs.sDsi.uDSCSlicePerPacket),

    // DSI Timing parameters
    XML_CREATE_TAG_ENTRY("DSIAutoRefreshFrameNumDiv", XML_TAG_DATATYPE_INTEGER, gPanelConfig.uAttrs.sDsi.uAutoRefreshFrameNumDiv),
    XML_CREATE_TAG_ENTRY("DSIEnableAutoRefresh", XML_TAG_DATATYPE_BOOLEAN, gPanelConfig.uAttrs.sDsi.bEnableAutoRefresh),
    XML_CREATE_TAG_ENTRY("DSIInitMasterTime", XML_TAG_DATATYPE_INTEGER, gPanelConfig.uAttrs.sDsi.uInitMasterTime),
    XML_CREATE_TAG_ENTRY("DisplayResetGpio", XML_TAG_DATATYPE_INTEGER, gPanelConfig.uResetGpio),
    XML_CREATE_TAG_ENTRY("DisplayResetInfo", XML_TAG_DATATYPE_HEX_LIST, gPanelConfig.uAttrs.sDsi.aResetInfo),
    XML_CREATE_TAG_ENTRY("DSIEscapeClockDivisor", XML_TAG_DATATYPE_INTEGER, gPanelConfig.uAttrs.sDsi.uEscapeClockDivisor),    
    XML_CREATE_TAG_ENTRY("DSIEscapeClockFrequency", XML_TAG_DATATYPE_INTEGER, gPanelConfig.uAttrs.sDsi.uEscapeClockFreq),    

   // DSI PHY Timing parameters
    XML_CREATE_TAG_ENTRY("DSITimingHSZeroOverride",     XML_TAG_DATATYPE_BOOLEAN, gPanelConfig.uAttrs.sDsi.bTimingHSZeroOverride),
    XML_CREATE_TAG_ENTRY("DSITimingHSZeroValue",        XML_TAG_DATATYPE_INTEGER, gPanelConfig.uAttrs.sDsi.uTimingHSZeroValue),
    XML_CREATE_TAG_ENTRY("DSITimingHSExitOverride",     XML_TAG_DATATYPE_BOOLEAN, gPanelConfig.uAttrs.sDsi.bTimingHSExitOverride),
    XML_CREATE_TAG_ENTRY("DSITimingHSExitValue",        XML_TAG_DATATYPE_INTEGER, gPanelConfig.uAttrs.sDsi.uTimingHSExitValue),
    XML_CREATE_TAG_ENTRY("DSITimingHSPrepareOverride",  XML_TAG_DATATYPE_BOOLEAN, gPanelConfig.uAttrs.sDsi.bTimingHSPrepareOverride),
    XML_CREATE_TAG_ENTRY("DSITimingHSPrepareValue",     XML_TAG_DATATYPE_INTEGER, gPanelConfig.uAttrs.sDsi.uTimingHSPrepareValue),
    XML_CREATE_TAG_ENTRY("DSITimingHSTrailOverride",    XML_TAG_DATATYPE_BOOLEAN, gPanelConfig.uAttrs.sDsi.bTimingHSTrailOverride),
    XML_CREATE_TAG_ENTRY("DSITimingHSTrailValue",       XML_TAG_DATATYPE_INTEGER, gPanelConfig.uAttrs.sDsi.uTimingHSTrailValue),    
    XML_CREATE_TAG_ENTRY("DSITimingHSRequestOverride",  XML_TAG_DATATYPE_BOOLEAN, gPanelConfig.uAttrs.sDsi.bTimingHSRequestOverride),
    XML_CREATE_TAG_ENTRY("DSITimingHSRequestValue",     XML_TAG_DATATYPE_INTEGER, gPanelConfig.uAttrs.sDsi.uTimingHSRequestValue),
    XML_CREATE_TAG_ENTRY("DSITimingCLKZeroOverride",    XML_TAG_DATATYPE_BOOLEAN, gPanelConfig.uAttrs.sDsi.bTimingCLKZeroOverride),
    XML_CREATE_TAG_ENTRY("DSITimingCLKZeroValue",       XML_TAG_DATATYPE_INTEGER, gPanelConfig.uAttrs.sDsi.uTimingCLKZeroValue),
    XML_CREATE_TAG_ENTRY("DSITimingCLKTrailOverride",   XML_TAG_DATATYPE_BOOLEAN, gPanelConfig.uAttrs.sDsi.bTimingCLKTrailOverride),
    XML_CREATE_TAG_ENTRY("DSITimingCLKTrailValue",      XML_TAG_DATATYPE_INTEGER, gPanelConfig.uAttrs.sDsi.uTimingCLKTrailValue),
    XML_CREATE_TAG_ENTRY("DSITimingCLKPrepareOverride", XML_TAG_DATATYPE_BOOLEAN, gPanelConfig.uAttrs.sDsi.bTimingCLKPrepareOverride),
    XML_CREATE_TAG_ENTRY("DSITimingCLKPrepareValue",    XML_TAG_DATATYPE_INTEGER, gPanelConfig.uAttrs.sDsi.uTimingCLKPrepareValue),
    XML_CREATE_TAG_ENTRY("DSITimingCLKPreOverride",     XML_TAG_DATATYPE_BOOLEAN, gPanelConfig.uAttrs.sDsi.bTimingCLKPreOverride),
    XML_CREATE_TAG_ENTRY("DSITimingCLKPreValue",        XML_TAG_DATATYPE_INTEGER, gPanelConfig.uAttrs.sDsi.uTimingCLKPreValue),
    XML_CREATE_TAG_ENTRY("DSITimingCLKPostOverride",    XML_TAG_DATATYPE_BOOLEAN, gPanelConfig.uAttrs.sDsi.bTimingCLKPostOverride),
    XML_CREATE_TAG_ENTRY("DSITimingCLKPostValue",       XML_TAG_DATATYPE_INTEGER, gPanelConfig.uAttrs.sDsi.uTimingCLKPostValue),
    XML_CREATE_TAG_ENTRY("DSITimingTASureOverride",     XML_TAG_DATATYPE_BOOLEAN, gPanelConfig.uAttrs.sDsi.bTimingTASureOverride),
    XML_CREATE_TAG_ENTRY("DSITimingTASureValue",        XML_TAG_DATATYPE_INTEGER, gPanelConfig.uAttrs.sDsi.uTimingTASureValue),
    XML_CREATE_TAG_ENTRY("DSITimingTAGoOverride",       XML_TAG_DATATYPE_BOOLEAN, gPanelConfig.uAttrs.sDsi.bTimingTAGoOverride),
    XML_CREATE_TAG_ENTRY("DSITimingTAGoValue",          XML_TAG_DATATYPE_INTEGER, gPanelConfig.uAttrs.sDsi.uTimingTAGoValue),
    XML_CREATE_TAG_ENTRY("DSITimingTAGetOverride",      XML_TAG_DATATYPE_BOOLEAN, gPanelConfig.uAttrs.sDsi.bTimingTAGetOverride),
    XML_CREATE_TAG_ENTRY("DSITimingTAGetValue",         XML_TAG_DATATYPE_INTEGER, gPanelConfig.uAttrs.sDsi.uTimingTAGetValue),

    // DSI Video Mode Configuration
    XML_CREATE_TAG_ENTRY("DSITrafficMode",          XML_TAG_DATATYPE_INTEGER, gPanelConfig.uAttrs.sDsi.eTrafficMode),
    XML_CREATE_TAG_ENTRY("DSIHsaHseAfterVsVe",      XML_TAG_DATATYPE_BOOLEAN, gPanelConfig.uAttrs.sDsi.bSendHsaHseAfterVsVe),
    XML_CREATE_TAG_ENTRY("DSILowPowerModeInHFP",    XML_TAG_DATATYPE_BOOLEAN, gPanelConfig.uAttrs.sDsi.bLowPowerModeInHFP),
    XML_CREATE_TAG_ENTRY("DSILowPowerModeInHBP",    XML_TAG_DATATYPE_BOOLEAN, gPanelConfig.uAttrs.sDsi.bLowPowerModeInHBP),
    XML_CREATE_TAG_ENTRY("DSILowPowerModeInHSA",    XML_TAG_DATATYPE_BOOLEAN, gPanelConfig.uAttrs.sDsi.bLowPowerModeInHSA),
    XML_CREATE_TAG_ENTRY("DSILowPowerModeInBLLPEOF",XML_TAG_DATATYPE_BOOLEAN, gPanelConfig.uAttrs.sDsi.bLowPowerModeInBLLPEOF),
    XML_CREATE_TAG_ENTRY("DSILowPowerModeInBLLP",   XML_TAG_DATATYPE_BOOLEAN, gPanelConfig.uAttrs.sDsi.bLowPowerModeInBLLP),
    XML_CREATE_TAG_ENTRY("DSIForceCmdInVideoHS",    XML_TAG_DATATYPE_BOOLEAN, gPanelConfig.uAttrs.sDsi.bForceCmdInVideoHS),
    
    // DSI Command Mode Configurations 
    XML_CREATE_TAG_ENTRY("DSICmdModeIdleTime",          XML_TAG_DATATYPE_INTEGER, gPanelConfig.uAttrs.sDsi.uCommandModeIdleTime),
    XML_CREATE_TAG_ENTRY("DSICMDSwapInterface",         XML_TAG_DATATYPE_BOOLEAN, gPanelConfig.uAttrs.sDsi.bSwapInterface),
    XML_CREATE_TAG_ENTRY("DSICMDUsingTrigger",          XML_TAG_DATATYPE_BOOLEAN, gPanelConfig.uAttrs.sDsi.bUsingTrigger),
    XML_CREATE_TAG_ENTRY("DSISlaveControllerSkewLines", XML_TAG_DATATYPE_INTEGER, gPanelConfig.uAttrs.sDsi.iSlaveControllerSkewLines),
    XML_CREATE_TAG_ENTRY("DSICmdModePixelAlignment",    XML_TAG_DATATYPE_INTEGER, gPanelConfig.uAttrs.sDsi.uCmdModePixelAlignment),
    
    // DSI TE Configuration (Command mode only)
    XML_CREATE_TAG_ENTRY("DSITECheckEnable",           XML_TAG_DATATYPE_BOOLEAN, gPanelConfig.uAttrs.sDsi.sTE.bTECheckEnable),
    XML_CREATE_TAG_ENTRY("DSITEUsingDedicatedTEPin",   XML_TAG_DATATYPE_BOOLEAN, gPanelConfig.uAttrs.sDsi.sTE.bDedicatedTEPin),
    XML_CREATE_TAG_ENTRY("DSITEvSyncStartPos",         XML_TAG_DATATYPE_INTEGER, gPanelConfig.uAttrs.sDsi.sTE.vSyncStartPos),
    XML_CREATE_TAG_ENTRY("DSITEvSyncContinueLines",    XML_TAG_DATATYPE_INTEGER, gPanelConfig.uAttrs.sDsi.sTE.vSyncContinueLines),
    XML_CREATE_TAG_ENTRY("DSITEvSyncStartLineDivisor", XML_TAG_DATATYPE_INTEGER, gPanelConfig.uAttrs.sDsi.sTE.vSyncStartLineDivisor),
    XML_CREATE_TAG_ENTRY("DSITEvSyncPosSafetyMargin",  XML_TAG_DATATYPE_INTEGER, gPanelConfig.uAttrs.sDsi.sTE.vSyncPosSafetyMargin),
    XML_CREATE_TAG_ENTRY("DSITEvSyncBelowSafetyMargin",XML_TAG_DATATYPE_INTEGER, gPanelConfig.uAttrs.sDsi.sTE.vSyncBelowSafetyMargin),
    XML_CREATE_TAG_ENTRY("DSITEPercentVariance",       XML_TAG_DATATYPE_INTEGER, gPanelConfig.uAttrs.sDsi.sTE.vSyncPercentVariance),   
    XML_CREATE_TAG_ENTRY("DSITEvSyncRdPtrIrqLine",     XML_TAG_DATATYPE_INTEGER, gPanelConfig.uAttrs.sDsi.sTE.vSyncRdPtrIrqLine),
    XML_CREATE_TAG_ENTRY("DSITEvSyncSelect",           XML_TAG_DATATYPE_INTEGER, gPanelConfig.uAttrs.sDsi.sTE.vSyncSelect),
    XML_CREATE_TAG_ENTRY("DSITEvSyncInitVal",          XML_TAG_DATATYPE_INTEGER, gPanelConfig.uAttrs.sDsi.sTE.uVsyncInitValue),

    // DSI Strength configuration
    XML_CREATE_TAG_ENTRY("DSIDataStrengthLP", XML_TAG_DATATYPE_INTEGER, gPanelConfig.uAttrs.sDsi.uDataStrengthLP),
    XML_CREATE_TAG_ENTRY("DSIDataStrengthHS", XML_TAG_DATATYPE_INTEGER, gPanelConfig.uAttrs.sDsi.uDataStrengthHS),
    XML_CREATE_TAG_ENTRY("DSIClockStrengthHS", XML_TAG_DATATYPE_INTEGER, gPanelConfig.uAttrs.sDsi.uClockStrengthHS),

    // DSI Panel init sequence 
    XML_CREATE_TAG_ENTRY("DSIInitSequence", XML_TAG_DATATYPE_BINARY, gpDSIInitSequenceBuffer),
    XML_CREATE_TAG_ENTRY("DSITermSequence", XML_TAG_DATATYPE_BINARY, gpDSITermSequenceBuffer),

    // I2C configuration  
    XML_CREATE_TAG_ENTRY("DSII2CSlaveAddressList", XML_TAG_DATATYPE_INTEGER_LIST, gPanelConfig.uAttrs.sDsi.sI2CConfig.aI2CSlaveAddressList),
    XML_CREATE_TAG_ENTRY("DSII2CI2CNumOfSlaves",   XML_TAG_DATATYPE_INTEGER,      gPanelConfig.uAttrs.sDsi.sI2CConfig.uI2CNumOfSlaves),
    XML_CREATE_TAG_ENTRY("DSII2CFrequency",        XML_TAG_DATATYPE_INTEGER,      gPanelConfig.uAttrs.sDsi.sI2CConfig.uI2CBusFrequency), 
    XML_CREATE_TAG_ENTRY("DSII2CGSBIPort",         XML_TAG_DATATYPE_INTEGER,      gPanelConfig.uAttrs.sDsi.sI2CConfig.uI2CGSBICore),
    XML_CREATE_TAG_ENTRY("DSII2COffsetLength",     XML_TAG_DATATYPE_INTEGER,      gPanelConfig.uAttrs.sDsi.sI2CConfig.uI2COffsetLengthInByte),

    // I2C init/term sequence
    XML_CREATE_TAG_ENTRY("I2CInitSequence", XML_TAG_DATATYPE_BINARY, gpI2CInitSequenceBuffer),
    XML_CREATE_TAG_ENTRY("I2CTermSequence", XML_TAG_DATATYPE_BINARY, gpI2CTermSequenceBuffer),

    // I2C and EDID configuration    
    XML_CREATE_TAG_ENTRY("DynamicEDIDEnabled",         XML_TAG_DATATYPE_BOOLEAN, gPanelConfig.sDynamicEDIDConfig.bDynamicEDIDEnabled),   
    XML_CREATE_TAG_ENTRY("DynamicEDIDI2CSlaveAddress", XML_TAG_DATATYPE_INTEGER, gPanelConfig.sDynamicEDIDConfig.uI2CAddress),   
    XML_CREATE_TAG_ENTRY("DynamicEDIDI2CFrequency",    XML_TAG_DATATYPE_INTEGER, gPanelConfig.sDynamicEDIDConfig.uI2CFrequency), 
    XML_CREATE_TAG_ENTRY("DynamicEDIDI2CGSBIPort",     XML_TAG_DATATYPE_INTEGER, gPanelConfig.sDynamicEDIDConfig.uI2CGSBIPort),  
    XML_CREATE_TAG_ENTRY("DynamicEDIDPTM",             XML_TAG_DATATYPE_INTEGER, gPanelConfig.sDynamicEDIDConfig.uPreferredTimingMode),  
    XML_CREATE_TAG_ENTRY("DynamicEDIDStartAddress",    XML_TAG_DATATYPE_INTEGER, gPanelConfig.sDynamicEDIDConfig.uStartAddress),
    
    // Backlight configuration
    XML_CREATE_TAG_ENTRY("BacklightType",              XML_TAG_DATATYPE_INTEGER, gPanelConfig.sBacklightConfig.eBacklightType),
    XML_CREATE_TAG_ENTRY("BacklightPmicControlType",   XML_TAG_DATATYPE_INTEGER, gPanelConfig.sBacklightConfig.uBacklightCntrl.eBacklightCtrl),
    XML_CREATE_TAG_ENTRY("BacklightPmicPWMFrequency",  XML_TAG_DATATYPE_INTEGER, gPanelConfig.sBacklightConfig.uPwmDesiredFreqHz),
    XML_CREATE_TAG_ENTRY("BacklightDefault",           XML_TAG_DATATYPE_INTEGER, gPanelConfig.sBacklightConfig.uLevel),
    XML_CREATE_TAG_ENTRY("BacklightBitWidth",          XML_TAG_DATATYPE_INTEGER, gPanelConfig.sBacklightConfig.uNumBits),

    // MDP_DISPLAY_PRIMARY configuration flags
    XML_CREATE_TAG_ENTRY("DisplayPrimaryFlags",        XML_TAG_DATATYPE_INTEGER, gPanelConfig.eDisplayFlags),

    // Default GPIO state
    XML_CREATE_TAG_ENTRY_EX("TLMMGPIODefaultLow",         XML_TAG_DATATYPE_INTEGER_LIST, gPanelConfig.uDefaultGPIOState[0], 0xFF),
    XML_CREATE_TAG_ENTRY_EX("TLMMGPIODefaultHigh",        XML_TAG_DATATYPE_INTEGER_LIST, gPanelConfig.uDefaultGPIOState[1], 0xFF),

    // EDP Parameters
    XML_CREATE_TAG_ENTRY("EDPRefreshRate",          XML_TAG_DATATYPE_INTEGER,  gPanelConfig.uAttrs.sDp.uRefreshRate),
    XML_CREATE_TAG_ENTRY("EDPVoltageSwingLevel",    XML_TAG_DATATYPE_INTEGER,  gPanelConfig.uAttrs.sDp.uLaneSwingLevel),
    XML_CREATE_TAG_ENTRY("EDPPreemphasisLevel",     XML_TAG_DATATYPE_INTEGER,  gPanelConfig.uAttrs.sDp.uPreemphasisLevel),
    XML_CREATE_TAG_ENTRY("EDPNumberOfLanes",        XML_TAG_DATATYPE_INTEGER,  gPanelConfig.uAttrs.sDp.uNumberOfLanes),
    XML_CREATE_TAG_ENTRY("EDPLinkRate",             XML_TAG_DATATYPE_INTEGER,  gPanelConfig.uAttrs.sDp.uLinkRateInKhz),
    XML_CREATE_TAG_ENTRY("EDPMaxLinkRate",          XML_TAG_DATATYPE_INTEGER,  gPanelConfig.uAttrs.sDp.uMaxLinkRate),
    XML_CREATE_TAG_ENTRY("EDPTraining",             XML_TAG_DATATYPE_INTEGER,  gPanelConfig.uAttrs.sDp.eLinkTraining),
    XML_CREATE_TAG_ENTRY("EDPDPCDRead",             XML_TAG_DATATYPE_BOOLEAN,  gPanelConfig.uAttrs.sDp.bReadDPCD),
    XML_CREATE_TAG_ENTRY("EDPEDIDRead",             XML_TAG_DATATYPE_BOOLEAN,  gPanelConfig.uAttrs.sDp.bReadEDID),
    XML_CREATE_TAG_ENTRY("EDPHPDActiveLow",         XML_TAG_DATATYPE_BOOLEAN,  gPanelConfig.uAttrs.sDp.bHPDActiveLow),
    XML_CREATE_TAG_ENTRY("EDPPowerUpWaitInMs",      XML_TAG_DATATYPE_INTEGER,  gPanelConfig.uAttrs.sDp.uPowerupWaitinMs),
    XML_CREATE_TAG_ENTRY("EDPMaxAuxRetry ",         XML_TAG_DATATYPE_INTEGER,  gPanelConfig.uAttrs.sDp.uMaxAuxRetry),
    XML_CREATE_TAG_ENTRY("EDPDeviceIDMapping",      XML_TAG_DATATYPE_BINARY,   gDPDeviceIDMapping),
    
};

/* Define the length of the tag list
 */
#define XML_TAGSLIST_LENGTH  (sizeof(sXmlTagsList)/sizeof(XML_TagDefType))

/* ---------------------------------------------------------------------- */
/**
** FUNCTION: GetDSIControllerMapping()
** 
** DESCRIPTION:
**   Get DSI controller mapping.
**
** ---------------------------------------------------------------------- */
static void GetDSIControllerMapping(MDP_Panel_AttrType *pPanelInfo)
{
  XML_BinaryPacketType  sXMLBinaryData;
  uint8                *pSrc          = (uint8 *)gDSIContorllerMapping;
  uint8                *pDsiMapping   = NULL;

  // Use Unaligned Memory read from DSI Command buffer (Required for THUMB2 mode)
  MDP_ReadUnaligned_U32(&sXMLBinaryData.uPacketLength, pSrc);
  sXMLBinaryData.pData = (uint8* )&(((XML_BinaryPacketType*)pSrc)->pData);
  pDsiMapping = (uint8*)sXMLBinaryData.pData;

  pPanelInfo->uNumInterfaces = MDP_INTERFACE_SINGLE;

  if (0 == sXMLBinaryData.uPacketLength)
  {
    /* If Mapping is not defined, default to DSI controller 0*/
    pPanelInfo->uAttrs.sDsi.uControllerMapping[0]    = 0;
    pPanelInfo->uAttrs.sDsi.uControllerMappingMask   = 0x1;
    pPanelInfo->uAttrs.sDsi.uControllerSinglePrimary = DSI_0_SINGLE_PRIMARY;
  }
  else if (1 == sXMLBinaryData.uPacketLength)
  {
    pPanelInfo->uAttrs.sDsi.uControllerMapping[0]  = pDsiMapping[0];
    pPanelInfo->uAttrs.sDsi.uControllerMappingMask = 0x1;
    
    // Check is this is single DSI only with DSI1 controller as primary
    if (0x00 == pDsiMapping[0])
    {
        pPanelInfo->uAttrs.sDsi.uControllerSinglePrimary = DSI_0_SINGLE_PRIMARY;
    }
    else
    {
        pPanelInfo->uAttrs.sDsi.uControllerSinglePrimary = DSI_1_SINGLE_PRIMARY;
    }
  }
  else if( (0x00 == pDsiMapping[0]) && 
           (0x01 == pDsiMapping[1]))
  {
    /* Enable Dual pipe if DSI panel is dual DSI panel */
    pPanelInfo->uAttrs.sDsi.uControllerMapping[0]   = pDsiMapping[0];
    pPanelInfo->uAttrs.sDsi.uControllerMapping[1]   = pDsiMapping[1];
    pPanelInfo->uAttrs.sDsi.uControllerMappingMask  = 0x3;
    pPanelInfo->uNumInterfaces                      = MDP_INTERFACE_DUAL;
  }
  else if ((0x01 == pDsiMapping[0]) &&
           (0x00 == pDsiMapping[1]))
  {
    /* Enable Dual pipe if DSI panel is dual DSI panel */
    pPanelInfo->uAttrs.sDsi.uControllerMapping[0]  = pDsiMapping[0];
    pPanelInfo->uAttrs.sDsi.uControllerMapping[1]  = pDsiMapping[1];
    pPanelInfo->uAttrs.sDsi.uControllerMappingMask = 0x3;
    pPanelInfo->uNumInterfaces                     = MDP_INTERFACE_DUAL;
  }
}

/* ---------------------------------------------------------------------- */
/**
** FUNCTION: GetDPBondedModeMapping()
** 
** DESCRIPTION:
**   Get DP Device ID mapping for DP bonded mode.
**
** ---------------------------------------------------------------------- */
static void GetDPBondedModeMapping(MDP_Panel_AttrType *pPanelInfo)
{
  MDP_Status            eLocalStatus  = MDP_STATUS_OK;
  XML_BinaryPacketType  sXMLBinaryData;
  uint8                *pSrc          = (uint8 *)gDPDeviceIDMapping;
  uint32                uNumDevices;
  
  /* If DP Device ID mapping is not defined or invalid, set enable to false and number of devices to 1 */
  pPanelInfo->uAttrs.sDp.sBondedModeConfig.bEnabled    = FALSE;
  pPanelInfo->uAttrs.sDp.sBondedModeConfig.uNumDevices = 1;
  
  // Use Unaligned Memory read from DSI Command buffer (Required for THUMB2 mode)
  MDP_ReadUnaligned_U32(&sXMLBinaryData.uPacketLength, pSrc);
  sXMLBinaryData.pData = (uint8* )&(((XML_BinaryPacketType*)pSrc)->pData);
  
  uNumDevices = (sXMLBinaryData.uPacketLength - 1);
  
  if ((sXMLBinaryData.uPacketLength >= 3) && 
      (uNumDevices <= MDP_MAX_NUM_DP_DEVICES))
  {
    /* Read bonded mode config */
    uint8  *pDPMapping = (uint8*)sXMLBinaryData.pData;
    uint32  uDeviceIndex;
    uint32  uMasterDeviceIndex = (sXMLBinaryData.uPacketLength - 1);

    /* Populate Device ID of DP Device providing the shared source clock */
    if (pDPMapping[uMasterDeviceIndex] < MDP_MAX_DP_DEVICE_ID)
    {
      pPanelInfo->uAttrs.sDp.sBondedModeConfig.uMasterDeviceID = pDPMapping[uMasterDeviceIndex];

      if (MDP_DRIVER_DP_DEVICE_ID_NONE == pPanelInfo->uAttrs.sDp.sBondedModeConfig.uMasterDeviceID)
      {
        pPanelInfo->uAttrs.sDp.sBondedModeConfig.bExternalMasterDevice = FALSE;
      }
      else
      {
        pPanelInfo->uAttrs.sDp.sBondedModeConfig.bExternalMasterDevice = TRUE;
      }
    }
    else
    {
      MDP_Log_Message(MDP_LOGLEVEL_WARN, "Invalid DP Device ID for DP device providing the shared source clock");
      eLocalStatus = MDP_STATUS_FAILED;
    }

    if (eLocalStatus == MDP_STATUS_OK)
    {
      /* Populate DP Device ID mapping of DP devices to be driven in bonded mode */
      for (uDeviceIndex = 0; uDeviceIndex < uNumDevices; uDeviceIndex++)
      {
        if ((pDPMapping[uDeviceIndex] > MDP_DRIVER_DP_DEVICE_ID_NONE) && 
            (pDPMapping[uDeviceIndex] < MDP_MAX_DP_DEVICE_ID))
        {
          pPanelInfo->uAttrs.sDp.sBondedModeConfig.uDeviceIDMapping[uDeviceIndex] = pDPMapping[uDeviceIndex];

          if (pPanelInfo->uAttrs.sDp.sBondedModeConfig.uMasterDeviceID == pPanelInfo->uAttrs.sDp.sBondedModeConfig.uDeviceIDMapping[uDeviceIndex])
          {
            pPanelInfo->uAttrs.sDp.sBondedModeConfig.bExternalMasterDevice = FALSE;
          }
        }
        else
        {
          MDP_Log_Message(MDP_LOGLEVEL_WARN, "Invalid DP Device ID for DP device operating in bonded mode at index = %d", uDeviceIndex);
          eLocalStatus = MDP_STATUS_FAILED;
          break;
        }
      }
    }
    
    if (eLocalStatus == MDP_STATUS_OK)
    {
      pPanelInfo->uAttrs.sDp.sBondedModeConfig.bEnabled    = TRUE;
      pPanelInfo->uAttrs.sDp.sBondedModeConfig.uNumDevices = uNumDevices;
    }
    else
    {
      MDP_Log_Message(MDP_LOGLEVEL_WARN, "Invalid DP Device ID mapping specified, disabling DP bonded mode");
    }
  
  }

  pPanelInfo->uNumInterfaces = pPanelInfo->uAttrs.sDp.sBondedModeConfig.uNumDevices;
}


/*=========================================================================
      Public Functions
==========================================================================*/

/* ---------------------------------------------------------------------- */
/**
** FUNCTION: MDPDetectPanel()
** 
** DESCRIPTION:
**   Panel detection code, via XML, I2C or EDID
**
** ---------------------------------------------------------------------- */
MDP_Status MDPDetectPanel(MDP_Display_IDType eDisplayId, MDP_Panel_AttrType *pDisplayInfo)
{
  MDP_Status           eStatus                  = MDP_STATUS_OK;

  MDP_LOG_FUNC_ENTRY("MDPDetectPanel()", eDisplayId);

  if (eDisplayId >= MDP_DISPLAY_MAX)
  {
    MDP_Log_Message(MDP_LOGLEVEL_ERROR, "MDPDetectPanel: Invalid display ID %d\n", eDisplayId);  
    eStatus = MDP_STATUS_BAD_PARAM;
  }
  else if (NULL == pDisplayInfo)
  {
    eStatus = MDP_STATUS_BAD_PARAM;
    MDP_Log_Message(MDP_LOGLEVEL_ERROR, "MDPDetectPanel: Invalid Panel attribute for display %d\n", eDisplayId);  
  }
  else if ((pDisplayInfo->bIsSupported == TRUE) && 
      ((NULL == gpDSIInitSequenceBuffer[eDisplayId] ) ||
      (NULL == gpDSITermSequenceBuffer[eDisplayId] ) || 
      (NULL == gpI2CInitSequenceBuffer[eDisplayId])  || 
      (NULL == gpI2CTermSequenceBuffer[eDisplayId])  ||
      (NULL == gpDSIDscPpsBuffer[eDisplayId])))
  {
    eStatus = MDP_STATUS_NO_RESOURCES;
    MDP_Log_Message(MDP_LOGLEVEL_ERROR, "MDPDetectPanel: No resources for display %d\n", eDisplayId);
  }
  else
  {
    MDPPlatformParams    sPlatformParams;

    MDP_OSAL_MEMZERO(&sPlatformParams, sizeof(MDPPlatformParams));
    MDP_OSAL_MEMZERO(&gPanelConfig, sizeof(MDP_Panel_AttrType));
    MDP_OSAL_MEMZERO(&gDSIContorllerMapping, sizeof(gDSIContorllerMapping));
    MDP_OSAL_MEMZERO(&gDPDeviceIDMapping, sizeof(gDPDeviceIDMapping));
    MDP_OSAL_MEMZERO(gpDSIInitSequenceBuffer[eDisplayId], MDP_DSI_COMMAND_BUFFER_SIZE);
    MDP_OSAL_MEMZERO(gpDSITermSequenceBuffer[eDisplayId], MDP_DSI_COMMAND_BUFFER_SIZE);
    MDP_OSAL_MEMZERO(gpI2CInitSequenceBuffer[eDisplayId], MDP_I2C_COMMAND_BUFFER_SIZE);
    MDP_OSAL_MEMZERO(gpI2CTermSequenceBuffer[eDisplayId], MDP_I2C_COMMAND_BUFFER_SIZE);
    MDP_OSAL_MEMZERO(gpDSIDscPpsBuffer[eDisplayId], MDP_DSI_DSC_PPS_TOTAL_PACKET_SIZE);

    //Update XML tag list for dynamic allocated pointer.
    if(XML_PARSER_STATUS_BADPARAM == XML_UpdateTag(sXmlTagsList, XML_TAGSLIST_LENGTH,(uint8*)"DSIInitSequence",sizeof("DSIInitSequence"), gpDSIInitSequenceBuffer[eDisplayId], MDP_DSI_COMMAND_BUFFER_SIZE ))
    {
      eStatus = MDP_STATUS_BAD_PARAM;
    }
    else if (XML_PARSER_STATUS_BADPARAM == XML_UpdateTag(sXmlTagsList, XML_TAGSLIST_LENGTH, (uint8*)"DSITermSequence",sizeof("DSITermSequence"), gpDSITermSequenceBuffer[eDisplayId], MDP_DSI_COMMAND_BUFFER_SIZE ))
    {
      eStatus = MDP_STATUS_BAD_PARAM;
    }
    else if(XML_PARSER_STATUS_BADPARAM == XML_UpdateTag(sXmlTagsList, XML_TAGSLIST_LENGTH,(uint8*)"I2CInitSequence",sizeof("I2CInitSequence"), gpI2CInitSequenceBuffer[eDisplayId], MDP_I2C_COMMAND_BUFFER_SIZE ))
    {
      eStatus = MDP_STATUS_BAD_PARAM;
    }
    else if(XML_PARSER_STATUS_BADPARAM == XML_UpdateTag(sXmlTagsList, XML_TAGSLIST_LENGTH,(uint8*)"I2CTermSequence",sizeof("I2CTermSequence"), gpI2CTermSequenceBuffer[eDisplayId], MDP_I2C_COMMAND_BUFFER_SIZE ))
    {
      eStatus = MDP_STATUS_BAD_PARAM;
    }
    else if ((eDisplayId == pDisplayInfo->eDisplayId) &&
             (TRUE       == pDisplayInfo->bDetected))
    {
      // Platform already detected just continue
      eStatus = MDP_STATUS_OK;
    }
    else if (MDP_STATUS_OK != (eStatus = MDPPlatformConfigure(eDisplayId, MDPPLATFORM_CONFIG_GETPANELCONFIG, &sPlatformParams)))
    {
        MDP_Log_Message(MDP_LOGLEVEL_WARN, "MDPLib: MDPPlatformConfigure(MDPPLATFORM_CONFIG_GETPANELCONFIG) failed!\n");  
    }
    else if ((NULL == sPlatformParams.sPlatformPanel.pPanelXMLConfig) && 
             (0 == sPlatformParams.sPlatformPanel.uConfigSize))
    {
      // Platform configuration is NULL or invalid
      eStatus = MDP_STATUS_BAD_PARAM;   
      MDP_Log_Message(MDP_LOGLEVEL_ERROR, "MDPLib: Platform configuration does not exist!\n");  
    }
    else if (XML_PARSER_STATUS_OK == XML_Parser(sPlatformParams.sPlatformPanel.pPanelXMLConfig, sPlatformParams.sPlatformPanel.uConfigSize, sXmlTagsList, XML_TAGSLIST_LENGTH))
    {
        MDP_Panel_AttrType *pPanelInfo = &gPanelConfig;

        //set panel index
        pPanelInfo->eSelectedPanel = sPlatformParams.sPlatformPanel.eSelectedPanel;
        pPanelInfo->uNumInterfaces = MDP_INTERFACE_SINGLE;

        // Setup any display interface specific settings
        switch (pPanelInfo->ePhysConnect)
        {
        case MDP_DISPLAY_CONNECT_PRIMARY_DSI_VIDEO:
        case MDP_DISPLAY_CONNECT_PRIMARY_DSI_CMD:
        case MDP_DISPLAY_CONNECT_SECONDARY_DSI_VIDEO:
        case MDP_DISPLAY_CONNECT_SECONDARY_DSI_CMD:
        {
            pPanelInfo->uAttrs.sDsi.pInitBuffer   = (void*)gpDSIInitSequenceBuffer[eDisplayId];
            pPanelInfo->uAttrs.sDsi.pTermBuffer   = (void*)gpDSITermSequenceBuffer[eDisplayId];
            pPanelInfo->uAttrs.sDsi.pDSCPpsBuffer = (void*)gpDSIDscPpsBuffer[eDisplayId];

            // Check if there are any I2C slave addresses, if yes, then assume I2C initialization is required
            if (0 != pPanelInfo->uAttrs.sDsi.sI2CConfig.uI2CNumOfSlaves)
            {
              pPanelInfo->uAttrs.sDsi.sI2CConfig.pI2CInitSequence = gpI2CInitSequenceBuffer[eDisplayId];
              pPanelInfo->uAttrs.sDsi.sI2CConfig.pI2CTermSequence = gpI2CTermSequenceBuffer[eDisplayId];
            }
      
            if (TRUE == pPanelInfo->uAttrs.sDsi.bDSCEnable)
            {
              if ((1 > pPanelInfo->uAttrs.sDsi.uDSCMajor) ||
                  (1 > pPanelInfo->uAttrs.sDsi.uDSCMinor))
              {
                /* Reset to default DSC major and minor version */
                MDP_Log_Message(MDP_LOGLEVEL_WARN, "MDPDetectPanel: DSC major:%d minor:%d invalid, using default\n", pPanelInfo->uAttrs.sDsi.uDSCMajor, pPanelInfo->uAttrs.sDsi.uDSCMinor);
                pPanelInfo->uAttrs.sDsi.uDSCMajor = 1;
                pPanelInfo->uAttrs.sDsi.uDSCMinor = 1;
                pPanelInfo->uAttrs.sDsi.uDSCScr   = 0;
              }
            }

            GetDSIControllerMapping(pPanelInfo);

            break;
        }
        case MDP_DISPLAY_CONNECT_DP:
        {
            GetDPBondedModeMapping(pPanelInfo);
            MDPPanelGetEDID(pPanelInfo);
            break;
        }
        default:
            break;
        }

        // Only non-production mode should have this printed.
        if (!RETAIL)
        { 
            MDP_Log_Message(MDP_LOGLEVEL_WARN, "DisplayDxe: Panel ID:0x%08x [%a] \n", sPlatformParams.sPlatformPanel.uPanelId,
                                                                                            (MDP_PMIC_MODULE_CONTROLTYPE_IBB_LAB_OLED==sPlatformParams.sPlatformPanel.uPowerGridId)?"OLED":"LCD");
        }

        MDP_Log_Message(MDP_LOGLEVEL_ERROR, "DisplayDxe: Resolution %dx%d (%d intf)\n", pPanelInfo->uDisplayWidth,
                                                                                      pPanelInfo->uDisplayHeight,
                                                                                      pPanelInfo->uNumInterfaces);

        /*
         * Perform a quick sanity check
         * for eDP & DP, if both width and height are 0, then panel timing is derived from edid
         * so no sanity check
         */
        if ((pPanelInfo->ePhysConnect !=  MDP_DISPLAY_CONNECT_EDP) && 
            (pPanelInfo->ePhysConnect !=  MDP_DISPLAY_CONNECT_DP) && 
            ((0 == pPanelInfo->uDisplayHeight) || (0 == pPanelInfo->uDisplayWidth)))
        {
            MDP_Log_Message(MDP_LOGLEVEL_WARN, "DisplayDxe: Invalid mode detected for panel id %i!\n", pDisplayInfo->eDisplayId);
            eStatus = MDP_STATUS_BAD_PARAM;
        }
        else
        {
            // Check panel orientation override
            MDP_OSAL_MEMZERO(&sPlatformParams, sizeof(MDPPlatformParams));
            if (MDP_STATUS_OK == MDPPlatformConfigure(eDisplayId, MDPPLATFORM_CONFIG_GETPLATFORMINFO, &sPlatformParams))
            {
               if (sPlatformParams.sPlatformInfo.uPanelOrientationOverride != MDP_ROTATE_NONE)
               {
                  gPanelConfig.ePhysPanelOrientation = sPlatformParams.sPlatformInfo.uPanelOrientationOverride;
               }
            } 


            // Copy local to the caller's data structure      
            MDP_OSAL_MEMCPY(pDisplayInfo, pPanelInfo, sizeof(MDP_Panel_AttrType));

            switch (pPanelInfo->ePhysConnect)
            {
             case MDP_DISPLAY_CONNECT_PRIMARY_DSI_VIDEO:
             case MDP_DISPLAY_CONNECT_PRIMARY_DSI_CMD:
             case MDP_DISPLAY_CONNECT_SECONDARY_DSI_VIDEO:
             case MDP_DISPLAY_CONNECT_SECONDARY_DSI_CMD:
             {
               if (NULL != pPanelInfo->uAttrs.sDsi.pInitBuffer)
               {
                  if (NULL == (pDisplayInfo->uAttrs.sDsi.pInitBuffer = (uint8*)MDP_OSAL_CALLOC(MDP_DSI_COMMAND_BUFFER_SIZE)))
                  {
                    MDP_Log_Message(MDP_LOGLEVEL_WARN, "DisplayDxe: Allocate display init buffer failed!\n");
                  }
                  else
                  {
                    MDP_OSAL_MEMCPY(pDisplayInfo->uAttrs.sDsi.pInitBuffer, pPanelInfo->uAttrs.sDsi.pInitBuffer, MDP_DSI_COMMAND_BUFFER_SIZE);
                  }
               }

               if (NULL != pPanelInfo->uAttrs.sDsi.pTermBuffer)
               {
                  if (NULL == (pDisplayInfo->uAttrs.sDsi.pTermBuffer = (uint8*)MDP_OSAL_CALLOC(MDP_DSI_COMMAND_BUFFER_SIZE)))
                  {
                    MDP_Log_Message(MDP_LOGLEVEL_WARN, "DisplayDxe: Allocate display term buffer failed!\n");
                  }
                  else
                  {
                    MDP_OSAL_MEMCPY(pDisplayInfo->uAttrs.sDsi.pTermBuffer, pPanelInfo->uAttrs.sDsi.pTermBuffer,   MDP_DSI_COMMAND_BUFFER_SIZE);
                  }
               }

               if (NULL != pPanelInfo->uAttrs.sDsi.pDSCPpsBuffer)
               {
                  if (NULL == (pDisplayInfo->uAttrs.sDsi.pDSCPpsBuffer = (uint8*)MDP_OSAL_CALLOC(MDP_DSI_DSC_PPS_TOTAL_PACKET_SIZE)))
                  {
                    MDP_Log_Message(MDP_LOGLEVEL_WARN, "DisplayDxe: Allocate display pps buffer failed!\n");
                  }
                  else
                  {
                    MDP_OSAL_MEMCPY(pDisplayInfo->uAttrs.sDsi.pDSCPpsBuffer, pPanelInfo->uAttrs.sDsi.pDSCPpsBuffer, MDP_DSI_DSC_PPS_TOTAL_PACKET_SIZE);
                  }
               }
               break;
             }
             default:
               break;
            }

            // Report display is detected & set the display id
            pDisplayInfo->eDisplayId = eDisplayId;      
            pDisplayInfo->bDetected  = TRUE;

            // Check for sw rendering mode (used for invalid platforms)
            MDP_OSAL_MEMZERO(&sPlatformParams, sizeof(MDPPlatformParams));
            if (MDP_STATUS_OK == MDPPlatformConfigure(eDisplayId, MDPPLATFORM_CONFIG_SW_RENDERER, &sPlatformParams))
            { 
                pDisplayInfo->bSWRenderer = sPlatformParams.sPlatformInfo.bSWRender;
            }

            // Append PCB to primary flags to append any new flags
            pDisplayInfo->eDisplayFlags |= PcdGet32(PcdPrimaryFlags);
        }
    } 
    else
    {
        // Parser failed
        MDP_Log_Message(MDP_LOGLEVEL_ERROR, "DisplayDxe: Invalid or missing XML configuration!\n");
        eStatus = MDP_STATUS_FAILED;
    }

  }
  MDP_LOG_FUNC_EXIT("MDPDetectPanel()");

  return eStatus;
}



