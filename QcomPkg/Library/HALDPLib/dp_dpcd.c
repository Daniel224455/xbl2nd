/*=====================================================================================================================
 
  File: dp_dpcd.c
 
  Source file for DP EDID read and parse related functionality 
   
  Copyright (c) 2016-2020 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
=======================================================================================================================*/


/*---------------------------------------------------------------------------------------------------------------------
 * Include Files
 *-------------------------------------------------------------------------------------------------------------------*/
#include "dp_i.h"
#include "edp_psr.h"

#ifdef __cplusplus
extern "C" {
#endif

/*---------------------------------------------------------------------------------------------------------------------
 * Defines
 *-------------------------------------------------------------------------------------------------------------------*/
#define DPCD_EDP_CONFIGURATION_CAP_LEN             1
#define DPCD_EDP_CONFIGURATION_CAP                 0x0D

#define DPCD_CAP_LEN                               16
#define DPCD_CAP_START_ADDRESS                     0

#define DPCD_DSC_CAP_LEN                           16
#define DPCD_DSC_CAP_START_ADDRESS                 0x60

#define DPCD_DSC_ENABLE_LEN                        1
#define DPCD_DSC_ENABLE_START_ADDRESS              0x160

#define DPCD_DSC_STATUS_LEN                        1
#define DPCD_DSC_STATUS_START_ADDRESS              0x20f

#define DPCD_PSR_CAP_LEN                           5
#define DPCD_PSR_CAP_START_ADDRESS                 0x70

#define DPCD_PSR_ENABLE_CONFIG_LEN                 1
#define DPCD_PSR_ENABLE_CONFIG_ADDRESS             0x170

#define DPCD_FEC_CAP_LEN                           1
#define DPCD_FEC_CAP_START_ADDRESS                 0x90

#define DPCD_FEC_CONFIG_LEN                        1
#define DPCD_FEC_CONFIG_START_ADDRESS              0x120

#define DPCD_FEC_STATUS_LEN                        3
#define DPCD_FEC_STATUS_START_ADDRESS              0x280

#define DPCD_LINK_STATUS_LEN                       6
#define DPCD_LINK_STATUS_START_ADDRESS             0x202

#define DPCD_LINKCONFIG_LEN                        2
#define DPCD_LINKCONFIG_START_ADDRESS              0x100

#define DPCD_TRAINING_PATTERN_SET_LEN              1
#define DPCD_TRAINING_PATTERN_SET_START_ADDRESS    0x102

#define DPCD_TRAINING_LANE_SET_LEN                 4
#define DPCD_TRAINING_LANE_SET_START_ADDRESS       0x103

#define DPCD_LINK_DOWNSPREAD_CTRL_LEN              1
#define DPCD_LINK_DOWNSPREAD_CTRL_ADDRESS          0x107

#define DPCD_EDP_CONFIGURATION_SET_LEN             1
#define DPCD_EDP_CONFIGURATION_SET                 0x10A

#define DPCD_SET_POWER_STATE_LEN                   1
#define DPCD_SET_POWER_STATE_START_ADDRESS         0x600

#define DPCD_TEST_CRC_R_CR_LEN                     7
#define DPCD_TEST_CRC_R_CR                         0x240

#define DPCD_TEST_SINK_START_LEN                   1
#define DPCD_TEST_SINK_START                       0x270

#define DPCD_PSR_STATUS_LEN                        5
#define DPCD_PSR_STATUS_START_ADDRESS              0x2006


#define DPCD_SINK_STATUS_LEN                       8
#define DPCD_SINK_STATUS_ADDRESS                   0x200
#define DPCD_SINK_STATUS_SINK_COUNT_MASK           0x3F
#define DPCD_SINK_STATUS_CP_READY_MASK             0x40
/* Max loop count to poll if sink link status is stable*/
#define DPCD_LINK_STATUS_CHECK_UPDATE_LOOP         100

#define DPCD_LINK_RATE_TABLE_LENGTH                8     /* Size of rate table for eDP */

/* EDP Brightness caps */
#define DPCD_BRIGHTNESS_CAP_LEN                    2
#define DPCD_BRIGHTNESS_CAP_START_ADDRESS          0x701

#define DPCD_DISPLAY_CONTROL_LEN                   1
#define DPCD_DISPLAY_CONTROL_START_ADDRESS         0x720
#define DPCD_BACKLIGHT_ENABLE_MASK                 0x1

#define DPCD_BACKLIGHT_MODE_LEN                    1
#define DPCD_BACKLIGHT_MODE_START_ADDRESS          0x721
#define DPCD_BACKLIGHT_BRIGHTNESS_CTRL_MODE_MASK   0x3
#define DPCD_BACKLIGHT_BRIGHTNESS_CTRL_BY_REG      0x2

#define DPCD_BACKLIGHT_BRIGHTNESS_MSB_ADDRESS      0x722
#define DPCD_BACKLIGHT_BRIGHTNESS_LSB_ADDRESS      0x723

#define DPCD_BACKLIGHT_BRIGHTNESS_8BIT_MAX         0xFF
#define DPCD_BACKLIGHT_BRIGHTNESS_16BIT_MAX        0x7FF


/*---------------------------------------------------------------------------------------------------------------------
 * Function pre-declarations
 *-------------------------------------------------------------------------------------------------------------------*/
static DP_Status  DP_ParseSinkDPCDCap(
  DP_DPCDSinkCapRawDataType       *psDPCDCapRawData,
  DP_DPCDSinkCapType              *psDPCDSinkCap);

static DP_Status  DP_ParseSinkDPCDStatus(
  DP_DPCDSinkStatusRawDataType    *psDPCDSinkStatusRawData,
  DP_DPCDSinkStatusType           *psDPCDSinkStatus);

static DP_Status  DP_ReadSinkStatus(
  DP_DeviceCtxType                *pDeviceCtx);

static DP_Status  DP_ModifySinkDPCDReg(
  DP_DeviceCtxType                *pDeviceCtx,
  uint32                           uRegAddress,
  uint8                            uBitMask,
  uint8                            uNewValue);

static void  DP_ParseSinkBrightnessCap(
  uint8                           *pRawData,
  DP_SinkBrightnessCapType        *pSinkBrightnessCap);


/**********************************************************************************************************************

FUNCTION: DP_ParseSinkDPCDCap()
   
   This function will parse the DPCD Capability from the Raw Data. 

Parameters:
   psDPCDCapRawData                 -[out] Raw data buffer containing DPCD data.
   psDPCDSinkCap                    -[out] Pointer to DPCD Sink Capability.

Return: 
   DP_Status 

**********************************************************************************************************************/
static DP_Status  DP_ParseSinkDPCDCap(DP_DPCDSinkCapRawDataType       *psDPCDCapRawData,
                                      DP_DPCDSinkCapType              *psDPCDSinkCap)
{
  DP_Status                       eStatus        = DP_STATUS_SUCCESS;

  DP_OSAL_MemSet((char*)psDPCDSinkCap, 0, sizeof(DP_DPCDSinkCapType));

  /* Byte 0: DPCD_REV */
  switch(psDPCDCapRawData->uDPCDRevision )
  {
    case  DPCD_SPEC_REV_MAJOR_1_0:
    {
      psDPCDSinkCap->eDPCDMajorRev = DP_DPCD_MAJORREV_1_0;
      break;
    }
    case  DPCD_SPEC_REV_MAJOR_1_1:
    {
      psDPCDSinkCap->eDPCDMajorRev = DP_DPCD_MAJORREV_1_1;
      break;
    }
    case  DPCD_SPEC_REV_MAJOR_1_2:
    {
      psDPCDSinkCap->eDPCDMajorRev = DP_DPCD_MAJORREV_1_2;
      break;
    }
    case  DPCD_SPEC_REV_MAJOR_1_3:
    {
      psDPCDSinkCap->eDPCDMajorRev = DP_DPCD_MAJORREV_1_3;
      break;
    }
    case  DPCD_SPEC_REV_MAJOR_1_4:
    {
      psDPCDSinkCap->eDPCDMajorRev = DP_DPCD_MAJORREV_1_4;
      break;
    }
    default:
    {
      psDPCDSinkCap->eDPCDMajorRev = DP_DPCD_MAJORREV_NONE;
      break;
    }
  }
      
  /* Byte 1: MAX_LINK_RATE */
  if (0 == psDPCDCapRawData->uMaxLinkRate)
  {
    uint32 uIndex = 0;
    /* 
     * Special case for eDP
     * The supported link rates are in Bytes 0x10-0x1f
     * The rates are in multiples of 200Khz
     */
    for (uIndex = 0; uIndex < DPCD_LINK_RATE_TABLE_LENGTH; uIndex ++)
    {
      psDPCDSinkCap->uSupportedLinkRatesInKhz[uIndex] = psDPCDCapRawData->uSupportedRates[uIndex] * 20;

      /* If the next entry is zero, then this is the maximum supported link rate */
      if ((uIndex < (DPCD_LINK_RATE_TABLE_LENGTH - 1)) &&
          (0      == psDPCDCapRawData->uSupportedRates[uIndex + 1]))
      {
        psDPCDSinkCap->uMaxLinkRateInKhz = psDPCDSinkCap->uSupportedLinkRatesInKhz[uIndex];
      }
    }
  }
  else
  {
    /* 
     * DP Link rate in Khz
     *
     * |-----------|---------------|-------------------|
     * |           | uMaxLinkRate  | uMaxLinkRateInKhz |
     * |           |  (From Sink)  |    x27000         |
     * |-----------|---------------|-------------------|
     * | 1.62 Mbps |     6         |    162000         |
     * | 2.7  Mbps |    10         |    270000         |
     * | 5.4  Mbps |    20         |    540000         |
     * | 8.1  Mbps |    30         |    810000         |
     * |-----------|---------------|-------------------|
     */
    
    psDPCDSinkCap->uMaxLinkRateInKhz = psDPCDCapRawData->uMaxLinkRate * 27000;
  }

  /* Byte 2: MAX_LANE_COUNT */
  psDPCDSinkCap->uMaxLaneCount = psDPCDCapRawData->uMaxLaneCount & 0x1F;
  
  if (psDPCDCapRawData->uMaxLaneCount & 0x80)
  {
    psDPCDSinkCap->eDPCDCap    |= DP_DPCD_CAP_SUPPORT_ENHANCED_FRAME;
  }
  if (psDPCDCapRawData->uMaxLaneCount & 0x40)
  {
    psDPCDSinkCap->eDPCDCap    |= DP_DPCD_CAP_SUPPORT_TRAININGPATTERN3;
  }
  if (psDPCDCapRawData->uMaxLaneCount & 0x20)
  {
    psDPCDSinkCap->eDPCDCap    |= DP_DPCD_CAP_SUPPORT_POST_LT_ADJ_REQ;
  }

  /*Byte 3: MAX_DOWNSPREAD */
  if (psDPCDCapRawData->uMaxDownSpread & 0x01)
  {
    psDPCDSinkCap->eDPCDCap    |= DP_DPCD_CAP_MAX_DOWNSPREAD_0_5;
  }
  if (psDPCDCapRawData->uMaxDownSpread & 0x40)
  {
    psDPCDSinkCap->eDPCDCap    |= DP_DPCD_CAP_NO_AUX_HANDSHAKE;
  }
  if (psDPCDCapRawData->uMaxDownSpread & 0x80)
  {
    psDPCDSinkCap->eDPCDCap    |= DP_DPCD_CAP_SUPPORT_TRAININGPATTERN4;
  }

  /* Byte 4: NORP */
  psDPCDSinkCap->uNumofRXPorts  = (psDPCDCapRawData->uNumofReceiverPorts & 0x1) + 1;

  if (psDPCDCapRawData->uNumofReceiverPorts & 0x20)
  {
    psDPCDSinkCap->eDPCDCap    |= DP_DPCD_CAP_5V_DP_PWR;
  }

  if (psDPCDCapRawData->uNumofReceiverPorts & 0x40)
  {
    psDPCDSinkCap->eDPCDCap    |= DP_DPCD_CAP_12V_DP_PWR;
  }

  if (psDPCDCapRawData->uNumofReceiverPorts & 0x80)
  {
    psDPCDSinkCap->eDPCDCap    |= DP_DPCD_CAP_18V_DP_PWR;
  } 

  /* Byte 5: DOWNSTREAMPORT_PRESENT */
  if (psDPCDCapRawData->uDownStreamPortPresent & 0x01)
  {
    psDPCDSinkCap->eDPCDCap    |= DP_DPCD_CAP_DWN_STRM_PORT_PRESENT;
  }
  
  switch((psDPCDCapRawData->uDownStreamPortPresent >> 1) & 0x03)
  {
    case  DPCD_SPEC_DWN_STRM_PORT_TYPE_DISPLAYPORT:
    {
      psDPCDSinkCap->eDwnStreamPortType = DP_DPCD_DOWNSTREAMPORTTYPE_DISPLAYPORT;
      break;
    }
    case  DPCD_SPEC_DWN_STRM_PORT_TYPE_VGA_DVI_I:
    {
      psDPCDSinkCap->eDwnStreamPortType = DP_DPCD_DOWNSTREAMPORTTYPE_VGA_DVI_I;
      break;
    }
    case  DPCD_SPEC_DWN_STRM_PORT_TYPE_DVI_HDMI:
    {
      psDPCDSinkCap->eDwnStreamPortType = DP_DPCD_DOWNSTREAMPORTTYPE_DVI_HDMI;
      break;
    }
    case  DPCD_SPEC_DWN_STRM_PORT_TYPE_OTHERS:
    {
      psDPCDSinkCap->eDwnStreamPortType = DP_DPCD_DOWNSTREAMPORTTYPE_OTHER;
      break;
    }
    default:
    {
      psDPCDSinkCap->eDwnStreamPortType = DP_DPCD_DOWNSTREAMPORTTYPE_NONE;
      break;
    }
  }
  if (psDPCDCapRawData->uDownStreamPortPresent & 0x08)
  {
    psDPCDSinkCap->eDPCDCap    |= DP_DPCD_CAP_SUPPORT_FOMRAT_CONVERSION;
  }
  if (psDPCDCapRawData->uDownStreamPortPresent & 0x10)
  {
    psDPCDSinkCap->eDPCDCap    |= DP_DPCD_CAP_DETAILED_CAP_INFO_AVAILABLE;
  }

  /* Byte 6: MAIN_LINK_CHANNEL_CODING */  

  /* Byte 7: DOWN_STREAM_PORT_COUNT */
  psDPCDSinkCap->uNumOfDownStreamPorts = psDPCDCapRawData->uDownStreamPortCount & 0x0F;
  if (psDPCDCapRawData->uDownStreamPortCount & 0x40)
  {
    psDPCDSinkCap->eDPCDCap    |= DP_DPCD_CAP_MSA_TIMING_PAR_IGNORED;
  }
  if (psDPCDCapRawData->uDownStreamPortCount&0x80)
  {
    psDPCDSinkCap->eDPCDCap    |= DP_DPCD_CAP_SUPPORT_OUI;
  }

  /* Byte 8: RECEIVE_PORT0_CAP_0 */
  if (psDPCDCapRawData->uReceivePort0Cap0 & 0x02)
  {
    psDPCDSinkCap->eDPCDCap    |= DP_DPCD_CAP_PORT0_EDID_PRESENTED;
  }
  if (psDPCDCapRawData->uReceivePort0Cap0 & 0x04)
  {
    psDPCDSinkCap->eDPCDCap    |= DP_DPCD_CAP_PORT0_USED_FOR_2ND_STREAM;
  }

  /* Byte 9: RECEIVE_PORT0_CAP_1 */
  psDPCDSinkCap->uRXPort0BufferSize  = (psDPCDCapRawData->uReceivePort0Cap1 + 1) * 32;

  /* Byte 10: RECEIVE_PORT1_CAP_0 */
  if (psDPCDCapRawData->uReceivePort1Cap0 & 0x02)
  {
    psDPCDSinkCap->eDPCDCap    |= DP_DPCD_CAP_PORT1_EDID_PRESENTED;
  }
  if (psDPCDCapRawData->uReceivePort1Cap0 & 0x04)
  {
    psDPCDSinkCap->eDPCDCap    |= DP_DPCD_CAP_PORT1_USED_FOR_2ND_STREAM;
  }

  /*Byte 11: RECEIVE_PORT1_CAP_1 */
  psDPCDSinkCap->uRXPort1BufferSize            = (psDPCDCapRawData->uReceivePort1Cap1 + 1) * 32;

  /*Byte 12: I2C speed control */
  switch(psDPCDCapRawData->uI2CSpeedControlCap)
  {
    case  DPCD_SPEC_I2C_SPEED_CTRL_1KBPS:
    {
      psDPCDSinkCap->eI2CSpeedCtrl = DP_DPCD_I2CSPEEDCTRL_1KBPS;
      break;
    }
    case  DPCD_SPEC_I2C_SPEED_CTRL_5KBPS:
    {
      psDPCDSinkCap->eI2CSpeedCtrl = DP_DPCD_I2CSPEEDCTRL_5KBPS;
      break;
    }
    case  DPCD_SPEC_I2C_SPEED_CTRL_10KBPS:
    {
      psDPCDSinkCap->eI2CSpeedCtrl = DP_DPCD_I2CSPEEDCTRL_10KBPS;
      break;
    }
    case  DPCD_SPEC_I2C_SPEED_CTRL_100KBPS:
    {
      psDPCDSinkCap->eI2CSpeedCtrl = DP_DPCD_I2CSPEEDCTRL_100KBPS;
      break;
    }
    case  DPCD_SPEC_I2C_SPEED_CTRL_400KBPS:
    {
      psDPCDSinkCap->eI2CSpeedCtrl = DP_DPCD_I2CSPEEDCTRL_400KBPS;
      break;
    }
    case  DPCD_SPEC_I2C_SPEED_CTRL_1MBPS:
    {
      psDPCDSinkCap->eI2CSpeedCtrl = DP_DPCD_I2CSPEEDCTRL_1MBPS;
      break;
    }
    default:
    {
      psDPCDSinkCap->eI2CSpeedCtrl = DP_DPCD_I2CSPEEDCTRL_NONE;
      break;
    }
  }

  /* Byte 13: eDP_CONFIGURATION_CAP is eDP specific */  
  if (psDPCDCapRawData->uEDPConfigurationCap & 0x01)
  {
   psDPCDSinkCap->eDPCDCap    |= DP_DPCD_CAP_SUPPORT_ASSR;
  }
  if (psDPCDCapRawData->uEDPConfigurationCap & 0x02)
  {
    psDPCDSinkCap->eDPCDCap    |= DP_DPCD_CAP_FORCE_ENHANCED_FRAME;
  }

  /* Byte 14: TRAINING_AUX_RD_INTERVAL */
  if ((psDPCDCapRawData->uTrainingAUXRdInterval & 0x7F) == 0)
  {
    psDPCDSinkCap->uCRTrainingIntervalInUs    = 100;    
    psDPCDSinkCap->uCETrainingIntervalInUs    = 400;
  }
  else
  {
    psDPCDSinkCap->uCETrainingIntervalInUs    = (psDPCDCapRawData->uTrainingAUXRdInterval & 0x7F) * 4000;
    psDPCDSinkCap->uCRTrainingIntervalInUs    = (psDPCDSinkCap->eDPCDMajorRev >= DP_DPCD_MAJORREV_1_4) ?
                                                                       100 : psDPCDSinkCap->uCETrainingIntervalInUs;
    
  }

  if (psDPCDCapRawData->uTrainingAUXRdInterval & 0x80)
  {
    psDPCDSinkCap->eDPCDCap    |= DP_DPCD_CAP_EXTENDED_RECEIVER_FIELD;
  }

  /* Byte 15: ADAPTER_CAP */
  if (psDPCDCapRawData->uAdapterCap & 0x01)
  {
    psDPCDSinkCap->eDPCDCap    |= DP_DPCD_CAP_SUPPORT_VGA_FORCE_LOAD_SENSE;
  }
  if (psDPCDCapRawData->uAdapterCap & 0x02)
  {
    psDPCDSinkCap->eDPCDCap    |= DP_DPCD_CAP_SUPPORT_ALTERNATE_I2C_PATTERN;
  }

  return eStatus;
}


/**********************************************************************************************************************

FUNCTION: DP_ParseSinkDPCDStatus()
   
   This function will parse the DPCD Status from the Raw Data. 

Parameters:
   psDPCDSinkStatusRawData          -[in ] Raw data buffer containing DPCD Status data.
   psDPCDSinkStatus                 -[out] Pointer to DPCD sink Status.

Return: 
   DP_Status 

**********************************************************************************************************************/
static DP_Status  DP_ParseSinkDPCDStatus(DP_DPCDSinkStatusRawDataType    *psDPCDSinkStatusRawData,
                                         DP_DPCDSinkStatusType           *psDPCDSinkStatus)
{
  DP_Status                       eStatus        = DP_STATUS_SUCCESS;

  DP_OSAL_MemSet((char*)psDPCDSinkStatus, 0, sizeof(DP_DPCDSinkStatusType));

  /* Byte 0x202: LANE0_1_STATUS */
  if (psDPCDSinkStatusRawData->uLaneStatus0_1  & 0x01)
  {
    psDPCDSinkStatus->eLaneStatus[0] |= DP_DPCD_LANE_STATUS_CR_DONE;
  }
  if (psDPCDSinkStatusRawData->uLaneStatus0_1  & 0x02)
  {
    psDPCDSinkStatus->eLaneStatus[0] |= DP_DPCD_LANE_STATUS_CHANNEL_EQ_DONE;
  }
  if (psDPCDSinkStatusRawData->uLaneStatus0_1  & 0x04)
  {
    psDPCDSinkStatus->eLaneStatus[0] |= DP_DPCD_LANE_STATUS_SYMBOL_LOCKED;
  }
  if (psDPCDSinkStatusRawData->uLaneStatus0_1  & 0x10)
  {
    psDPCDSinkStatus->eLaneStatus[1] |= DP_DPCD_LANE_STATUS_CR_DONE;
  }
  if (psDPCDSinkStatusRawData->uLaneStatus0_1  & 0x20)
  {
    psDPCDSinkStatus->eLaneStatus[1] |= DP_DPCD_LANE_STATUS_CHANNEL_EQ_DONE;
  }
  if (psDPCDSinkStatusRawData->uLaneStatus0_1  & 0x40)
  {
    psDPCDSinkStatus->eLaneStatus[1] |= DP_DPCD_LANE_STATUS_SYMBOL_LOCKED;
  }

  /* Byte 0x203: LANE2_3_STATUS */
  if (psDPCDSinkStatusRawData->uLaneStatus2_3  & 0x01)
  {
    psDPCDSinkStatus->eLaneStatus[2] |= DP_DPCD_LANE_STATUS_CR_DONE;
  }
  if (psDPCDSinkStatusRawData->uLaneStatus2_3 & 0x02)
  {
    psDPCDSinkStatus->eLaneStatus[2] |= DP_DPCD_LANE_STATUS_CHANNEL_EQ_DONE;
  }
  if (psDPCDSinkStatusRawData->uLaneStatus2_3 & 0x04)
  {
    psDPCDSinkStatus->eLaneStatus[2] |= DP_DPCD_LANE_STATUS_SYMBOL_LOCKED;
  }
  if (psDPCDSinkStatusRawData->uLaneStatus2_3 & 0x10)
  {
    psDPCDSinkStatus->eLaneStatus[3] |= DP_DPCD_LANE_STATUS_CR_DONE;
  }
  if (psDPCDSinkStatusRawData->uLaneStatus2_3 & 0x20)
  {
    psDPCDSinkStatus->eLaneStatus[3] |= DP_DPCD_LANE_STATUS_CHANNEL_EQ_DONE;
  }
  if (psDPCDSinkStatusRawData->uLaneStatus2_3 & 0x40)
  {
    psDPCDSinkStatus->eLaneStatus[3] |= DP_DPCD_LANE_STATUS_SYMBOL_LOCKED;
  }

  /* Byte 0x204: LANE_ALIGN_STATUS_UPDATED */
  if (psDPCDSinkStatusRawData->uLaneAlignStatusUpdated & 0x01)
  {
    psDPCDSinkStatus->eLinkStatusBitArray |= DP_DPCD_BIT_STATUS_INTERLANE_ALIGN_DONE;
  }
  if (psDPCDSinkStatusRawData->uLaneAlignStatusUpdated & 0x40)
  {
    psDPCDSinkStatus->eLinkStatusBitArray |= DP_DPCD_BIT_STATUS_PORT_STATUS_CHANGED;
  }
  if (psDPCDSinkStatusRawData->uLaneAlignStatusUpdated & 0x80)
  {
    psDPCDSinkStatus->eLinkStatusBitArray |= DP_DPCD_BIT_STATUS_LINK_STATUS_UPDATED;
  }

  /* Byte 0x205: SINK_STATUS */
  if (psDPCDSinkStatusRawData->uSinkStatus & 0x01)
  {
    psDPCDSinkStatus->eLinkStatusBitArray |= DP_DPCD_BIT_STATUS_PORT_0_IN_SYNC;
  }
  if (psDPCDSinkStatusRawData->uSinkStatus & 0x02)
  {
    psDPCDSinkStatus->eLinkStatusBitArray |= DP_DPCD_BIT_STATUS_PORT_1_IN_SYNC;
  }

  /* Byte 0x206: ADJUST_REQUEST_LANE0_1 */
  psDPCDSinkStatus->uVoltageSwingLevel[0]   = (psDPCDSinkStatusRawData->uAdjustRequestLane0_1)      & 0x3;
  psDPCDSinkStatus->uPreEmphasisLevel[0]    = (psDPCDSinkStatusRawData->uAdjustRequestLane0_1 >> 2) & 0x3;
  psDPCDSinkStatus->uVoltageSwingLevel[1]   = (psDPCDSinkStatusRawData->uAdjustRequestLane0_1 >> 4) & 0x3;
  psDPCDSinkStatus->uPreEmphasisLevel[1]    = (psDPCDSinkStatusRawData->uAdjustRequestLane0_1 >> 6) & 0x3;

  /* Byte 0x207: ADJUST_REQUEST_LANE2_3 */
  psDPCDSinkStatus->uVoltageSwingLevel[2]   = (psDPCDSinkStatusRawData->uAdjustRequestLane2_3)      & 0x3;
  psDPCDSinkStatus->uPreEmphasisLevel[2]    = (psDPCDSinkStatusRawData->uAdjustRequestLane2_3 >> 2) & 0x3;
  psDPCDSinkStatus->uVoltageSwingLevel[3]   = (psDPCDSinkStatusRawData->uAdjustRequestLane2_3 >> 4) & 0x3;
  psDPCDSinkStatus->uPreEmphasisLevel[3]    = (psDPCDSinkStatusRawData->uAdjustRequestLane2_3 >> 6) & 0x3;

  return eStatus;
}

/**********************************************************************************************************************

FUNCTION: DP_ReadSinkDPCDCap()
   
   This function will read Sink Capability Data from DPCD over AUX Channel

Parameters:
   pDeviceCtx                       -[in ] Pointer to device context
   psDPCDSinkCap                    -[out] Pointer to DPCD Sink Capability Data struct.

Return: 
   DP_Status 

**********************************************************************************************************************/
DP_Status  DP_ReadSinkDPCDCap(DP_DeviceCtxType    *pDeviceCtx,
                              DP_DPCDSinkCapType  *psDPCDSinkCap)
{
  DP_Status                       eStatus        = DP_STATUS_SUCCESS;  
  DP_DPCDSinkCapRawDataType       sDPCDCapRawData;
  DP_ReadPacketType               sReadPacket;
  uint32                          uReadIdx;
  uint8                           uSinkStatus[DPCD_SINK_STATUS_LEN];

  /*Read DPCD Data*/
  DP_OSAL_MemSet(&sReadPacket, 0, sizeof(DP_ReadPacketType));
  sReadPacket.eTransactionType     = DP_TRANSACTION_MODE_AUX;
  sReadPacket.puReadDataBuffer     = (uint8*)&sDPCDCapRawData;
  sReadPacket.uAddress             = DPCD_CAP_START_ADDRESS;
  sReadPacket.uReadSize            = 1;
  sReadPacket.uBytesRead           = 0;
  sReadPacket.bQueueOnly           = FALSE;

  /* Some dongles can only return DCDP byte by byte */
  for (uReadIdx = 0; uReadIdx < sizeof(DP_DPCDSinkCapRawDataType); uReadIdx++)
  {
    if (DP_STATUS_SUCCESS != (eStatus = DP_Host_ClientRead(pDeviceCtx, &sReadPacket)))
    {
      break;
    }
    sReadPacket.puReadDataBuffer++;
    sReadPacket.uAddress++;
  }

  if (DP_STATUS_SUCCESS != eStatus)
  {
    DP_LOG_MESSAGE_ERROR("DP_ReadDPCDSinkCap: Read Failed, eStatus=0x%x.",eStatus);
  }
  else if (sReadPacket.uBytesRead != sReadPacket.uReadSize)
  {
    eStatus  = DP_STATUS_FAILED_RESOURCE_FAILED;
    DP_LOG_MESSAGE_ERROR("DP_ReadDPCDSinkCap: Bytes Read(%d) is not expected.",sReadPacket.uBytesRead);
  }
  else
  {
    eStatus = DP_ParseSinkDPCDCap(&sDPCDCapRawData, psDPCDSinkCap);
  }

  /* Read Sink status */
  sReadPacket.puReadDataBuffer  = (uint8*)&uSinkStatus;
  sReadPacket.uAddress          = DPCD_SINK_STATUS_ADDRESS;
  sReadPacket.uReadSize         = DPCD_SINK_STATUS_LEN;
  if (DP_STATUS_SUCCESS != (eStatus = DP_Host_ClientRead(pDeviceCtx, &sReadPacket)))
  {
    DP_LOG_MESSAGE_ERROR("DP_ReadSinkDPCDCap: Read address(0x%x) Failed, eStatus=0x%x\n", sReadPacket.uAddress, eStatus);
  }
  else
  {
    pDeviceCtx->uSinkCount =  uSinkStatus[0] & DPCD_SINK_STATUS_SINK_COUNT_MASK;
    pDeviceCtx->bCPReady   = (uSinkStatus[0] & DPCD_SINK_STATUS_CP_READY_MASK) ? TRUE : FALSE;
  }

  return eStatus;
}

/**********************************************************************************************************************

FUNCTION: DP_ReadSinkDPCDStatus()
   
   This function will read Sink Status Raw Data from DPCD over AUX Channel

Parameters:
   pDeviceCtx                       -[in ] Pointer to device context
   psDPCDSinkStatus                 -[out] Pointer to DPCD Sink Status Data struct.

Return: 
   DP_Status 

**********************************************************************************************************************/
DP_Status  DP_ReadSinkDPCDStatus(DP_DeviceCtxType      *pDeviceCtx,
                                 DP_DPCDSinkStatusType *psDPCDSinkStatus)
{
  DP_Status                       eStatus             = DP_STATUS_SUCCESS;
  DP_DPCDSinkStatusRawDataType    sDPCDSinkStatusRawData;
  DP_ReadPacketType               sReadPacket;

  /*Read DPCD Data*/
  DP_OSAL_MemSet(&sReadPacket, 0, sizeof(DP_ReadPacketType));
  sReadPacket.eTransactionType     = DP_TRANSACTION_MODE_AUX;
  sReadPacket.puReadDataBuffer     = (uint8*)&sDPCDSinkStatusRawData;
  sReadPacket.uAddress             = DPCD_LINK_STATUS_START_ADDRESS;
  sReadPacket.uReadSize            = sizeof(DP_DPCDSinkStatusRawDataType);
  sReadPacket.uBytesRead           = 0;
  sReadPacket.bQueueOnly           = FALSE;

  if (DP_STATUS_SUCCESS != (eStatus = DP_Host_ClientRead(pDeviceCtx, &sReadPacket)))
  {
    DP_LOG_MESSAGE_ERROR("DP_ReadSinkDPCDStatus: Read Failed, eStatus=0x%x.", eStatus);
  }
  else if (sReadPacket.uBytesRead != sReadPacket.uReadSize)
  {
    eStatus = DP_STATUS_FAILED_RESOURCE_FAILED;
    DP_LOG_MESSAGE_ERROR("DP_ReadSinkDPCDStatus: Bytes Read(0x%x) is not expected.", sReadPacket.uBytesRead);
  }
  else
  {
    // Cache the raw sink status information 
    DP_OSAL_MemCpy(&pDeviceCtx->aSinkStatus, &sDPCDSinkStatusRawData, sizeof(pDeviceCtx->aSinkStatus));
    eStatus = DP_ParseSinkDPCDStatus(&sDPCDSinkStatusRawData, psDPCDSinkStatus);
  }

  return eStatus;
}

/**********************************************************************************************************************

FUNCTION: DP_WriteSinkDPCDLink()
   
   This function will Write the link setting to panel DPCD 

Parameters:
   pDeviceCtx                       -[in] Pointer to device context

Return: 
   DP_Status 

**********************************************************************************************************************/
DP_Status  DP_WriteSinkDPCDLink(DP_DeviceCtxType    *pDeviceCtx)
{
  DP_Status                 eStatus       = DP_STATUS_SUCCESS; 
  DP_WritePacketType        sWritePacket;
  uint8                     uWriteBuffer[2];
  uint32                    i;

  /*
   * For eDP 1.4+, the panel will report supported bit rates as a Table as opposed to just the max bit rate
   * on previous spec revisions. In this case, we set the selected link rate on DPCD register 0x115. For all 
   * other cases, the link rate is set on DPCD register 0x100.
   */

  /* LINK_BW_SET */
  if (DP_DEVICE_ID_EDP == pDeviceCtx->eDeviceID)
  {
    sWritePacket.eTransactionType    = DP_TRANSACTION_MODE_AUX;
    sWritePacket.puWriteData         = uWriteBuffer;
    sWritePacket.uAddress            = 0x115;
    sWritePacket.uWriteDataSize      = 1;
    sWritePacket.uBytesWritten       = 0;
    sWritePacket.bQueueOnly          = FALSE;

    /* Find the index in the list of supported rates */
    for (i = 0;i < 8;i++)
    {
      if (pDeviceCtx->sSinkDPCDCap.uSupportedLinkRatesInKhz[i] == pDeviceCtx->uLinkRateInKhz)
      {
        uWriteBuffer[0] = i;
        eStatus = DP_Host_ClientWrite(pDeviceCtx, &sWritePacket);
        if (DP_STATUS_SUCCESS != eStatus)
        {
          DP_LOG_MESSAGE_ERROR( "DP_WriteSinkDPCDLink: Write Failed, eStatus=0x%x.",eStatus);
        }
      }
    }

    /* LINK_RATE_SET */
    // Reset byte zero in the buffer for the next command
    uWriteBuffer[0] = 0;
  }    
  switch(pDeviceCtx->uLinkRateInKhz)
  {
    case 162000:
    {
      uWriteBuffer[0] = DPCD_SPEC_MAX_LINK_RATE_1_62GBPS;
      break;
    }
    case 270000:
    {
      uWriteBuffer[0] = DPCD_SPEC_MAX_LINK_RATE_2_7GBPS;
      break;
    }
    case 540000:
    {
      uWriteBuffer[0] = DPCD_SPEC_MAX_LINK_RATE_5_4GBPS;
      break;
    }
    case 810000:
    {
      uWriteBuffer[0] = DPCD_SPEC_MAX_LINK_RATE_8_1GBPS;
      break;
    }
    default:
    {
      eStatus         = DP_STATUS_FAILED_INVALID_INPUT_PARAMETER;
      DP_LOG_MESSAGE_ERROR("DP_WriteSinkDPCDLink: Invalid input parameter.");
      break;
    }
  }
  
  if (DP_STATUS_SUCCESS == eStatus )
  {
    /* LANE_COUNT_SET */
    
    uWriteBuffer[1] = (uint8) pDeviceCtx->uNumLanes;
    
    if (pDeviceCtx->sSinkDPCDCap.eDPCDCap & DP_DPCD_CAP_SUPPORT_ENHANCED_FRAME)
    {
      uWriteBuffer[1] |= DPCD_SPEC_EN_ENHANCED_FRAME_BMSK ;
    }

    /*Write DPCD Data*/
    DP_OSAL_MemSet(&sWritePacket, 0, sizeof(DP_WritePacketType));
    sWritePacket.eTransactionType    = DP_TRANSACTION_MODE_AUX;
    sWritePacket.puWriteData         = uWriteBuffer;
    sWritePacket.uAddress            = DPCD_LINKCONFIG_START_ADDRESS;
    sWritePacket.uWriteDataSize      = DPCD_LINKCONFIG_LEN;
    sWritePacket.uBytesWritten       = 0;
    sWritePacket.bQueueOnly          = FALSE;
    
    eStatus = DP_Host_ClientWrite(pDeviceCtx, &sWritePacket);

    if (DP_STATUS_SUCCESS != eStatus)
    {
      DP_LOG_MESSAGE_ERROR( "DP_WriteSinkDPCDLink: Write Failed, eStatus=0x%x.",eStatus);
    }

    if (TRUE == pDeviceCtx->bEnableSSCMode)
    {
      eStatus = DP_WriteSinkDPCDDownSpreadEnabled(pDeviceCtx);
    }
  }

  return eStatus;
}

/**********************************************************************************************************************

FUNCTION: DP_WriteSinkDPCDTrainingPattern()
   
   This function will select the training pattern for link training to panel DPCD 

Parameters:
   pDeviceCtx                       -[in ] Pointer to device context
   eTraingPattern                   -[in ] Link Training pattern 

Return: 
   DP_Status 

**********************************************************************************************************************/
DP_Status  DP_WriteSinkDPCDTrainingPattern(DP_DeviceCtxType              *pDeviceCtx,
                                           DP_DPCDTrainingPatternSetType  eTraingPattern)
{
  DP_Status                 eStatus       = DP_STATUS_SUCCESS; 
  uint8                     uWriteBuffer  = 0;
  DP_WritePacketType        sWritePacket;

  switch(eTraingPattern)
  {
    case DP_DPCD_TRAININGPATTERNSET_NONE:
    {
      uWriteBuffer = DPCD_SPEC_TRAINING_PATTERN_NONE;
      break;
    }
    case DP_DPCD_TRAININGPATTERNSET_PATTERN1:
    {
      uWriteBuffer = DPCD_SPEC_TRAINING_PATTERN_1;      
      /* Disable scrambling as this sequence is not scrambled */
      uWriteBuffer |= DPCD_SPEC_SCRAMBLING_DISABLE_BMSK;      
      break;
    }
    case DP_DPCD_TRAININGPATTERNSET_PATTERN2:
    {
      uWriteBuffer = DPCD_SPEC_TRAINING_PATTERN_2;      
      /* Disable scrambling as this sequence is not scrambled */
      uWriteBuffer |= DPCD_SPEC_SCRAMBLING_DISABLE_BMSK;
      break;
    }
    case DP_DPCD_TRAININGPATTERNSET_PATTERN3:
    {
      uWriteBuffer = DPCD_SPEC_TRAINING_PATTERN_3;      
      /* Disable scrambling as this sequence is not scrambled */
      uWriteBuffer |= DPCD_SPEC_SCRAMBLING_DISABLE_BMSK;
      break;
    }    
    case DP_DPCD_TRAININGPATTERNSET_PATTERN4:
    {
      uWriteBuffer = DPCD_SPEC_TRAINING_PATTERN_4;
      /* Disable scrambling as this sequence is not scrambled */
      uWriteBuffer |= DPCD_SPEC_SCRAMBLING_DISABLE_BMSK;
      break;
    }    
    default:
    {
      eStatus = DP_STATUS_FAILED_INVALID_INPUT_PARAMETER;
      DP_LOG_MESSAGE_ERROR("DP_WriteSinkDPCDTrainingPattern: Invalid input parameter");
      break;
    }
  }

  if (DP_STATUS_SUCCESS == eStatus)
  {
    /* Write DPCD Data */
    DP_OSAL_MemSet(&sWritePacket, 0, sizeof(DP_WritePacketType));
    sWritePacket.eTransactionType    = DP_TRANSACTION_MODE_AUX;
    sWritePacket.puWriteData         = &uWriteBuffer;
    sWritePacket.uAddress            = DPCD_TRAINING_PATTERN_SET_START_ADDRESS;
    sWritePacket.uWriteDataSize      = DPCD_TRAINING_PATTERN_SET_LEN;
    sWritePacket.uBytesWritten       = 0;
    sWritePacket.bQueueOnly          = FALSE;
    
    eStatus = DP_Host_ClientWrite(pDeviceCtx, &sWritePacket);

    if (DP_STATUS_SUCCESS != eStatus)
    {
      DP_LOG_MESSAGE_ERROR("DP_WriteSinkDPCDTrainingPattern: Write Failed, eStatus=0x%x.", eStatus);
    }
  }

  return eStatus;
}

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
DP_Status  DP_WriteSinkDPCDLevels(DP_DeviceCtxType   *pDeviceCtx,
                                  uint32              uDesiredVoltageSwingLevel,
                                  uint32              uDesiredPreEmphasisLevel)
{
  DP_Status                eStatus     = DP_STATUS_SUCCESS;  
  DP_WritePacketType       sWritePacket;
  uint8                    uWriteBuffer[4];       

  uWriteBuffer[0]    = uDesiredVoltageSwingLevel & 0x3;
  uWriteBuffer[0]   |= (uDesiredPreEmphasisLevel << 3) & 0x18;
  
  if (pDeviceCtx->uMaxVoltSwingLvl == uDesiredVoltageSwingLevel)
  {
    uWriteBuffer[0] |= DPCD_SPEC_MAX_SWING_REACHED_BMSK;
  }
  if (pDeviceCtx->uMaxPreemphLvl == uDesiredPreEmphasisLevel)
  {
    uWriteBuffer[0] |= DPCD_SPEC_MAX_PRE_EMPHASIS_REACHED_BMSK;
  }

  /* Use same config for lanes 1-4 */
  uWriteBuffer[3] = uWriteBuffer[0];
  uWriteBuffer[2] = uWriteBuffer[0];
  uWriteBuffer[1] = uWriteBuffer[0];

  /* Write DPCD Data */
  DP_OSAL_MemSet(&sWritePacket, 0, sizeof(DP_WritePacketType));
  sWritePacket.eTransactionType    = DP_TRANSACTION_MODE_AUX;
  sWritePacket.puWriteData         = uWriteBuffer;
  sWritePacket.uAddress            = DPCD_TRAINING_LANE_SET_START_ADDRESS;
  sWritePacket.uWriteDataSize      = DPCD_TRAINING_LANE_SET_LEN;
  sWritePacket.uBytesWritten       = 0;
  sWritePacket.bQueueOnly          = FALSE;
  
  eStatus = DP_Host_ClientWrite(pDeviceCtx, &sWritePacket);

  if (DP_STATUS_SUCCESS!= eStatus)
  {
    DP_LOG_MESSAGE_ERROR("DP_WriteSinkDPCDLevels: Write Failed, eStatus=0x%x.", eStatus);
  }

  return eStatus;
}

/**********************************************************************************************************************

FUNCTION: DP_WriteSinkDPCDDownSpreadEnabled()
   
   This function will indicate that downspread has been enabled on host side.

Parameters:
   pDeviceCtx                       -[in] Pointer to device context

Return: 
   DP_Status 

**********************************************************************************************************************/
DP_Status DP_WriteSinkDPCDDownSpreadEnabled(DP_DeviceCtxType    *pDeviceCtx)
{
  DP_Status                eStatus      = DP_STATUS_SUCCESS;  
  uint8                    uWriteBuffer = 0x10;    // Bit 4: SPREAD_AMP
  DP_WritePacketType       sWritePacket;

  DP_OSAL_MemSet(&sWritePacket, 0, sizeof(DP_WritePacketType));
  sWritePacket.eTransactionType    = DP_TRANSACTION_MODE_AUX;
  sWritePacket.puWriteData         = &uWriteBuffer;
  sWritePacket.uAddress            = DPCD_LINK_DOWNSPREAD_CTRL_ADDRESS;
  sWritePacket.uWriteDataSize      = DPCD_LINK_DOWNSPREAD_CTRL_LEN;
  sWritePacket.uBytesWritten       = 0;
  sWritePacket.bQueueOnly          = FALSE;
  
  eStatus = DP_Host_ClientWrite(pDeviceCtx, &sWritePacket);

  if (DP_STATUS_SUCCESS != eStatus)
  {
    DP_LOG_MESSAGE_ERROR("DP_WriteSinkDPCDDownSpreadEnabled: Write Failed, eStatus=0x%x.", eStatus);
  }

  return eStatus;
}



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
                                DP_DPCDPowerStateType  ePowerState)
{
  DP_Status                eStatus     = DP_STATUS_SUCCESS;  
  DP_WritePacketType       sWritePacket;
  uint8                    uWriteBuffer;

  switch (ePowerState)
  {
    case DP_DPCD_POWERSTATE_D0:
    {
      uWriteBuffer = DPCD_SPEC_POWER_STATE_D0;
      break;
    }
    case DP_DPCD_POWERSTATE_D3:
    {
      uWriteBuffer = DPCD_SPEC_POWER_STATE_D3;
      break;
    }
    case DP_DPCD_POWERSTATE_LINK_D3_AUX_D0:
    {
      uWriteBuffer = DPCD_SPEC_POWER_STATE_LINK_D3_AUX_D0;
      break;
    }
    default:
    {
      eStatus      = DP_STATUS_FAILED_INVALID_INPUT_PARAMETER;
      DP_LOG_MESSAGE_ERROR("DP_WriteSinkDPCDPower: Invalid input parameter ");
      break;
    }
  }

  if (DP_STATUS_SUCCESS == eStatus)
  {
    /*Write DPCD Data*/
    DP_OSAL_MemSet(&sWritePacket, 0, sizeof(DP_WritePacketType));
    sWritePacket.eTransactionType    = DP_TRANSACTION_MODE_AUX;
    sWritePacket.puWriteData         = &uWriteBuffer;
    sWritePacket.uAddress            = DPCD_SET_POWER_STATE_START_ADDRESS;
    sWritePacket.uWriteDataSize      = DPCD_SET_POWER_STATE_LEN;
    sWritePacket.uBytesWritten       = 0;
    sWritePacket.bQueueOnly          = FALSE;
    
    eStatus = DP_Host_ClientWrite(pDeviceCtx, &sWritePacket);

    if (DP_STATUS_SUCCESS != eStatus)
    {
      DP_LOG_MESSAGE_ERROR("DP_WriteSinkDPCDPower: Write Failed, eStatus=0x%x\n", eStatus);
    }
        
  }

  return eStatus;
}


/**********************************************************************************************************************

FUNCTION: DP_ReadSinkStatus()

This function will read sink status register for sink count and CP readiness. 

Parameters:
   pDeviceCtx                       -[in ] Pointer to device context

Return:
DP_Status

**********************************************************************************************************************/
static DP_Status  DP_ReadSinkStatus(DP_DeviceCtxType      *pDeviceCtx)
{
  DP_Status                       eStatus       = DP_STATUS_SUCCESS;  
  uint8                           uSinkStatus[DPCD_SINK_STATUS_LEN];
  DP_ReadPacketType               sReadPacket;
  DP_DPCDSinkStatusType           sSinkDPCDStatus;

  /*Read DPCD Data*/
  DP_OSAL_MemSet(&sReadPacket, 0, sizeof(DP_ReadPacketType));
  sReadPacket.eTransactionType     = DP_TRANSACTION_MODE_AUX;
  sReadPacket.puReadDataBuffer     = (uint8*)&uSinkStatus;
  sReadPacket.uAddress             = DPCD_SINK_STATUS_ADDRESS;
  sReadPacket.uReadSize            = DPCD_SINK_STATUS_LEN;
  sReadPacket.uBytesRead           = 0;
  sReadPacket.bQueueOnly           = FALSE;

  if (DP_STATUS_SUCCESS != (eStatus = DP_Host_ClientRead(pDeviceCtx, &sReadPacket)))
  {
    DP_LOG_MESSAGE_ERROR("DP_ReadSinkStatus: Read address(0x%x) Failed, eStatus=0x%x.", sReadPacket.uAddress, eStatus);
  }
  else
  {
    pDeviceCtx->uSinkCount  =  uSinkStatus[0] & DPCD_SINK_STATUS_SINK_COUNT_MASK;
    pDeviceCtx->bCPReady    = (uSinkStatus[0] & DPCD_SINK_STATUS_CP_READY_MASK)?TRUE:FALSE;
  }

  DP_OSAL_MemSet(&sSinkDPCDStatus, 0, sizeof(sSinkDPCDStatus));

  if (DP_STATUS_SUCCESS != (eStatus = DP_ReadSinkDPCDStatus(pDeviceCtx, &sSinkDPCDStatus)))
  {
    /* Failed to read link status DPCD, treat it as link lost */
    pDeviceCtx->bLinkLost = TRUE;
    DP_LOG_MESSAGE_ERROR("DP_ReadSinkStatus: DP_ParseSinkDPCDStatus Failed, eStatus=0x%x.", eStatus);
  }
  else
  {
    if ((TRUE == DP_CheckTrainingResult(pDeviceCtx, &sSinkDPCDStatus, DP_LINK_TRAINING_STATUS_CHANNEL_EQ_DONE))   &&
        (TRUE == DP_CheckTrainingResult(pDeviceCtx, &sSinkDPCDStatus, DP_LINK_TRAINING_STATUS_SYMBOL_LOCKED))     &&
        (TRUE == DP_CheckTrainingResult(pDeviceCtx, &sSinkDPCDStatus, DP_LINK_TRAINING_STATUS_CLK_RECOVERY_DONE)) &&
        (TRUE == DP_CheckTrainingResult(pDeviceCtx, &sSinkDPCDStatus, DP_LINK_TRAINING_STATUS_INTERLANE_ALIGN_DONE)))
    {
      pDeviceCtx->bLinkLost = FALSE;
    }
    else
    {
      pDeviceCtx->bLinkLost = TRUE;
    }
  }

  return eStatus;
}

/**********************************************************************************************************************

FUNCTION: DP_ParseSinkPSRCap()

This function will read sink status register for sink count and CP readiness. 

Parameters:
   pPsrBuf                       -[in ] buffer contains sink PSR data
   pSinkPSRCap                   -[in ] pointer of PSR struct

Return:
DP_Status

**********************************************************************************************************************/
static void  DP_ParseSinkPSRCap(uint8              *pPsrBuf,
                                     DP_SinkPSRCapType  *pSinkPSRCap)
{
  uint8  uData;

  if (pPsrBuf != NULL) 
  {
     uData = pPsrBuf[0];
     pSinkPSRCap->uPSRSupported = uData;        /* dpcd addr 0x70, PSR supported and version        */
     if (pSinkPSRCap->uPSRSupported != 0) 
     {
       uData = pPsrBuf[1];                     /* dpcd addr 0x71 */
       if (!(uData & 0x01))
       {
          pSinkPSRCap->bLinkTrainingPSRExit = TRUE;
       }
       uData >>= 1;
       uData &= 0x07;
       if (uData > 6)
       {
          uData = 6;     
       }
       /*
        * PSR Setup Time have 0 ~ 6 levels,
        * 330 us for level 0 which is default.
        * after that, each level has 55 us decreased
        */
       pSinkPSRCap->uSetupTimeUs = (330 - 55 * uData);

       if (pSinkPSRCap->uPSRSupported >= 2)   /* PSR2 */
       {
          uData >>= 3;
          if (uData & 0x01)
          {
             pSinkPSRCap->bSUYCoordinate = TRUE;
          }
          uData >>= 1;
          if (uData & 0x01)
          {
             pSinkPSRCap->bSUGranularity = TRUE;
          }
          uData = pPsrBuf[2];                     /* dpcd addr 0x72 */
          pSinkPSRCap->uSU_X_Granularity = uData;
          uData = pPsrBuf[3];                     /* dpcd addr 0x73 */
          pSinkPSRCap->uSU_X_Granularity |= (uData << 8);
          uData = pPsrBuf[4];                     /* dpcd addr 0x74 */
          pSinkPSRCap->uSU_Y_Granularity = uData;
       }
    }
  }
  DP_LOG_MESSAGE_INFO("DP_ParseSinkPSRCap: ver=%d tain=%d time=%d coord=%d gran=%d x=%x y=%d\n",
                                            pSinkPSRCap->uPSRSupported,
                                            pSinkPSRCap->bLinkTrainingPSRExit,
                                            pSinkPSRCap->uSetupTimeUs,
                                            pSinkPSRCap->bSUYCoordinate,
                                            pSinkPSRCap->bSUGranularity,
                                            pSinkPSRCap->uSU_X_Granularity,
                                            pSinkPSRCap->uSU_Y_Granularity);
}

/**********************************************************************************************************************

FUNCTION: DP_ReadSinkPSRStatus()

This function will read sink PSR status register for sink count and CP readiness. 

Parameters:
   pDeviceCtx                       -[in ] Pointer to device context

Return:
DP_Status

**********************************************************************************************************************/
DP_Status  DP_ReadSinkPSRStatus(DP_DeviceCtxType      *pDeviceCtx)
{
  DP_Status                       eStatus       = DP_STATUS_SUCCESS;  
  uint8                           uPsrBuf[DPCD_PSR_STATUS_LEN];
  DP_ReadPacketType               sReadPacket;

  /*Read DPCD Data*/
  DP_OSAL_MemSet(&sReadPacket, 0, sizeof(DP_ReadPacketType));
  sReadPacket.eTransactionType     = DP_TRANSACTION_MODE_AUX;
  sReadPacket.puReadDataBuffer     = (uint8*)&uPsrBuf;
  sReadPacket.uAddress             = DPCD_PSR_STATUS_START_ADDRESS;
  sReadPacket.uReadSize            = DPCD_PSR_STATUS_LEN;
  sReadPacket.uBytesRead           = 0;
  sReadPacket.bQueueOnly           = FALSE;


  if (DP_STATUS_SUCCESS != (eStatus = DP_Host_ClientRead(pDeviceCtx, &sReadPacket)))
  {
    eStatus = DP_STATUS_FAILED_RESOURCE_FAILED;
    DP_LOG_MESSAGE_ERROR("DP_ReadSinkPSRStatus: Read Failed, eStatus=0x%x.", eStatus);
  }
  else if (sReadPacket.uBytesRead != sReadPacket.uReadSize)
  {
    eStatus = DP_STATUS_FAILED_RESOURCE_FAILED;
    DP_LOG_MESSAGE_ERROR("DP_ReadSinkPSRStatus: Bytes Read(0x%x) is not expected.", sReadPacket.uBytesRead);
  }

  DP_LOG_MESSAGE_ERROR("DP_ReadSinkPSRStatus: %x %x %x %x %x\n", uPsrBuf[0], uPsrBuf[1], uPsrBuf[2], uPsrBuf[3], uPsrBuf[4]);

  return eStatus;
}

/**********************************************************************************************************************

FUNCTION: DP_ReadSinkPSRCap()

This function will read sink Capability register for sink count and CP readiness. 

Parameters:
   pDeviceCtx                       -[in ] Pointer to device context

Return:
DP_Status

**********************************************************************************************************************/
DP_Status  DP_ReadSinkPSRCap(DP_DeviceCtxType      *pDeviceCtx)
{
  DP_Status                       eStatus       = DP_STATUS_SUCCESS;  
  uint8                           uPsrBuf[DPCD_PSR_CAP_LEN];
  DP_ReadPacketType               sReadPacket;

  /*Read DPCD Data*/
  DP_OSAL_MemSet(&sReadPacket, 0, sizeof(DP_ReadPacketType));
  sReadPacket.eTransactionType     = DP_TRANSACTION_MODE_AUX;
  sReadPacket.puReadDataBuffer     = (uint8*)&uPsrBuf;
  sReadPacket.uAddress             = DPCD_PSR_CAP_START_ADDRESS;
  sReadPacket.uReadSize            = DPCD_PSR_CAP_LEN;
  sReadPacket.uBytesRead           = 0;
  sReadPacket.bQueueOnly           = FALSE;


  if (DP_STATUS_SUCCESS != (eStatus = DP_Host_ClientRead(pDeviceCtx, &sReadPacket)))
  {
    DP_LOG_MESSAGE_ERROR("DP_ReadSinkPSRCap: Read Failed, eStatus=0x%x.", eStatus);
  }
  else if (sReadPacket.uBytesRead != sReadPacket.uReadSize)
  {
    eStatus = DP_STATUS_FAILED_RESOURCE_FAILED;
    DP_LOG_MESSAGE_ERROR("DP_ReadSinkPSRCap: Bytes Read(0x%x) is not expected.", sReadPacket.uBytesRead);
  }
  else
  {
    DP_ParseSinkPSRCap(uPsrBuf, &pDeviceCtx->sPSR.sSinkCap);
  }

  return eStatus;
}

/**********************************************************************************************************************

FUNCTION: DP_WriteSinkPSRConfiguration()

This function will write sink PSR configuration register (0x170) for sink count and CP readiness. 

Parameters:
   pDeviceCtx                       -[in ] Pointer to device context

Return:
DP_Status

**********************************************************************************************************************/
DP_Status  DP_WriteSinkPSRCOnfiguration(DP_DeviceCtxType      *pDeviceCtx)
{
     DP_Status           eStatus       = DP_STATUS_SUCCESS;  
     uint8               data          = 0;
     DP_WritePacketType  sWritePacket;
     DP_SinkPSRCfgType   *pPsrCfg = &pDeviceCtx->sPSR.sSinkCfg;


     if (pPsrCfg->uEnablePsrFunction == PSR_ENABLE_SINK_PSR)
     {
       data |= BIT(0);
     }
     if (pPsrCfg->uMainLinkState == PSR_MAINLINK_REMAIN_ACTIVE)
     {
       data |= BIT(1);
     }
     if (pPsrCfg->uCRCVerified == PSR_CRC_VERIFIED_BY_SINK)
     {
       data |= BIT(2);
     }
     if (pPsrCfg->uFrameCapture == PSR_FRAME_CAPTURE_AT_N1)
     {
       data |= BIT(3);
     }
     if (pPsrCfg->uSUScanLineCapture == PSR_SU_SCANLINE_CAPTURE_AT_SECOND_LINE)
     {
       data |= BIT(4);
     }
     if (pPsrCfg->uHPDLIinkCRCError == PSR_HPD_WITH_LINK_CRC_ERROR)
     {
       data |= BIT(5);
     }
     if (pPsrCfg->uPSREnableProtocol == PSR_PROTOCOL_PSR2)
     {
       data |= BIT(6);
     }

     /* write DPCD Data*/
     DP_OSAL_MemSet(&sWritePacket, 0, sizeof(DP_WritePacketType));
     sWritePacket.eTransactionType    = DP_TRANSACTION_MODE_AUX;
     sWritePacket.puWriteData         = &data;
     sWritePacket.uAddress            = DPCD_PSR_ENABLE_CONFIG_ADDRESS;
     sWritePacket.uWriteDataSize      = DPCD_PSR_ENABLE_CONFIG_LEN;
     sWritePacket.uBytesWritten       = 0;
     sWritePacket.bQueueOnly          = FALSE;

     eStatus = DP_Host_ClientWrite(pDeviceCtx, &sWritePacket);
     if (DP_STATUS_SUCCESS != eStatus)
     {
        DP_LOG_MESSAGE_ERROR( "DP_WriteSinkPSRConfiguration: Write Failed, eStatus=0x%x.",eStatus);
     }

     return eStatus;
}

/**********************************************************************************************************************

FUNCTION: DP_WriteSinkASSR()

This function will write sink PSR configuration register (0x170) for sink count and CP readiness. 

Parameters:
   pDeviceCtx                       -[in ] Pointer to device context

Return:
DP_Status

**********************************************************************************************************************/
DP_Status  DP_WriteSinkASSR(DP_DeviceCtxType      *pDeviceCtx, uint32 uEnable)
{
     DP_Status           eStatus       = DP_STATUS_SUCCESS;  
     uint8               data          = 0;
     DP_WritePacketType  sWritePacket;

     data = uEnable;  /* Alternate Scrambler Reset Enable */
     /* write DPCD Data*/
     DP_OSAL_MemSet(&sWritePacket, 0, sizeof(DP_WritePacketType));
     sWritePacket.eTransactionType    = DP_TRANSACTION_MODE_AUX;
     sWritePacket.puWriteData         = &data;
     sWritePacket.uAddress            = DPCD_EDP_CONFIGURATION_SET;
     sWritePacket.uWriteDataSize      = DPCD_EDP_CONFIGURATION_SET_LEN;
     sWritePacket.uBytesWritten       = 0;
     sWritePacket.bQueueOnly          = FALSE;

     eStatus = DP_Host_ClientWrite(pDeviceCtx, &sWritePacket);
     if (DP_STATUS_SUCCESS != eStatus)
     {
        DP_LOG_MESSAGE_ERROR( "DP_WriteSinkASSR: Write Failed, eStatus=0x%x.",eStatus);
     }

     return eStatus;
}

/**********************************************************************************************************************

FUNCTION: DP_ReadSinkPSRConfiguration()

This function will Read sink PSR configuration register (0x170) for sink count and CP readiness. 

Parameters:
   pDeviceCtx                       -[in ] Pointer to device context

Return:
DP_Status

**********************************************************************************************************************/
DP_Status  DP_ReadSinkPSRConfiguration(DP_DeviceCtxType      *pDeviceCtx)
{
  DP_Status                       eStatus       = DP_STATUS_SUCCESS;  
  uint8                           uPsrBuf[DPCD_PSR_ENABLE_CONFIG_ADDRESS];
  DP_ReadPacketType               sReadPacket;

  /*Read DPCD Data*/
  DP_OSAL_MemSet(&sReadPacket, 0, sizeof(DP_ReadPacketType));
  sReadPacket.eTransactionType     = DP_TRANSACTION_MODE_AUX;
  sReadPacket.puReadDataBuffer     = (uint8*)&uPsrBuf;
  sReadPacket.uAddress             = DPCD_PSR_ENABLE_CONFIG_ADDRESS;
  sReadPacket.uReadSize            = DPCD_PSR_ENABLE_CONFIG_LEN;
  sReadPacket.uBytesRead           = 0;
  sReadPacket.bQueueOnly           = FALSE;


  if (DP_STATUS_SUCCESS != (eStatus = DP_Host_ClientRead(pDeviceCtx, &sReadPacket)))
  {
    eStatus = DP_STATUS_FAILED_RESOURCE_FAILED;
    DP_LOG_MESSAGE_ERROR("DP_ReadSinkPSRCap: Read Failed, eStatus=0x%x.", eStatus);
  }
  else if (sReadPacket.uBytesRead != sReadPacket.uReadSize)
  {
    eStatus = DP_STATUS_FAILED_RESOURCE_FAILED;
    DP_LOG_MESSAGE_ERROR("DP_ReadSinkPSRCap: Bytes Read(0x%x) is not expected.", sReadPacket.uBytesRead);
  }

  DP_LOG_MESSAGE_ERROR("DP_ReadSinkPSRConfiguration: %x\n", uPsrBuf[0]);

  return eStatus;
}

/**********************************************************************************************************************

FUNCTION: DP_ParseSinkDSCCap()

This function will parser sink DSC Capabilities register 

Parameters:
   pPsrBuf                       -[in ] buffer contains sink PSR data
   pSinkDSCCap                   -[in ] pointer of DSC struct

Return:
DP_Status

**********************************************************************************************************************/
static void  DP_ParseSinkDSCCap(uint8              *pPsrBuf,
                                DP_SinkDSCCapType  *pSinkDSCCap)
{
  uint8  uData;

  if (pPsrBuf != NULL) 
  {
     uData = pPsrBuf[0]; /* dpcd 0x60 */
     if (uData & 0x01)
     {
        pSinkDSCCap->bSupported = TRUE;
        uData = pPsrBuf[1];                      /* dpcd 0x61 */
        pSinkDSCCap->uMajor = uData  & 0x0f;
        pSinkDSCCap->uMinor = (uData >> 4);
        uData = pPsrBuf[2];	                  /* dpcd 0x62 */
        switch (uData)
        {
           case 0:
              pSinkDSCCap->uRCBufferBlockSize = 1024;  /* 1k */
              break;
           case 1:
              pSinkDSCCap->uRCBufferBlockSize = 4096;  /* 4k */
              break;
           case 2:
              pSinkDSCCap->uRCBufferBlockSize = 16384;  /* 16k */
              break;
           case 3:
              pSinkDSCCap->uRCBufferBlockSize = 65536;  /* 64 */
              break;
           default:
              pSinkDSCCap->uRCBufferBlockSize = 0;
              break;
        } 
        uData = pPsrBuf[3];	                  /* dpcd 0x63 */
        pSinkDSCCap->uRCBufferBlockSize *= (uData + 1);

        uData = pPsrBuf[4];	                  /* dpcd 0x64 */
        pSinkDSCCap->uSliceSupported = uData;
        uData = pPsrBuf[13];	                  /* dpcd 0x6d */
        pSinkDSCCap->uSliceSupported |= (uData << 8);

        uData = pPsrBuf[5];	                  /* dpcd 0x65 */
        uData &= 0x0f;
        if (uData == 0x1000)
        {
           pSinkDSCCap->uLineBufBitDepth = 8;
        }
        else
        {
           pSinkDSCCap->uLineBufBitDepth = uData + 9;
        }

        uData = pPsrBuf[6];	                  /* dpcd 0x66 */
        if (uData & 0x01)
        {
           pSinkDSCCap->bBlockPrediction = TRUE;
        }
       
        /* MaxBitsPerPixel is used at edp 1.4a or higher, U6.4 format */
        uData = pPsrBuf[7];	                  /* dpcd 0x67 */
        pSinkDSCCap->uMaxBitsPerPixel = uData;
        uData = pPsrBuf[8];	                  /* dpcd 0x68 */
        uData &= 0x03;
        pSinkDSCCap->uMaxBitsPerPixel |= (uData << 8);

        uData = pPsrBuf[9];	                  /* dpcd 0x69 */
        uData &= 0x1F;
        pSinkDSCCap->uColorFormatCap = uData;

        uData = pPsrBuf[0x0a];	                  /* dpcd 0x6a */
        uData &= 0x0F;
        pSinkDSCCap->uColorDepthCap = uData;      /* compression ratio: 8, 10 and 12 bits bpc */

        uData = pPsrBuf[0x0b];	                  /* dpcd 0x6b */
        pSinkDSCCap->uPeakThroughputMode0 = uData & 0x0f;  /* index to throughput table */
        pSinkDSCCap->uPeakThroughputMode1 = (uData >> 4);  /* index to throughput table */

        uData = pPsrBuf[0x0c];	                  /* dpcd 0x6c */
        pSinkDSCCap->uMaxSliceWidth = uData * 320;

        uData = pPsrBuf[0x0f];	                  /* dpcd 0x6f */
        uData &= 0x07;
        pSinkDSCCap->uBitsPerPixelIncrement = uData; /* 1/16, 1/8, 1/4, 1/2, 1/1 */
     }
  }

  DP_LOG_MESSAGE_INFO("DP_ParseSinkDSCCap: sup=%d maj=%d minor=%d blk_size=%d slice=%d buf_depth=%d pred=%d maxbits=%d\n",
        pSinkDSCCap-> bSupported,
        pSinkDSCCap-> uMajor,
        pSinkDSCCap-> uMinor,
        pSinkDSCCap-> uRCBufferBlockSize,
        pSinkDSCCap-> uSliceSupported,
        pSinkDSCCap-> uLineBufBitDepth,
        pSinkDSCCap-> bBlockPrediction,
        pSinkDSCCap-> uMaxBitsPerPixel);

  DP_LOG_MESSAGE_INFO("DP_ParseSinkDSCCap: col_format=%d col_depth=%d peadput=%d peakput=%d slice-wid=%d pixel_inc=%d\n",
        pSinkDSCCap-> uColorFormatCap,
        pSinkDSCCap-> uColorDepthCap,
        pSinkDSCCap-> uPeakThroughputMode0,
        pSinkDSCCap-> uPeakThroughputMode1,
        pSinkDSCCap-> uMaxSliceWidth,
        pSinkDSCCap-> uBitsPerPixelIncrement);


}

/**********************************************************************************************************************

FUNCTION: DP_ReadSinkDSCCap()

This function will read sink DSC Capabilities register and parser them. 

Parameters:
   pDeviceCtx                       -[in ] Pointer to device context

Return:
DP_Status

**********************************************************************************************************************/
DP_Status  DP_ReadSinkDSCCap(DP_DeviceCtxType      *pDeviceCtx)
{
  DP_Status                       eStatus       = DP_STATUS_SUCCESS;  
  uint8                           uPsrBuf[DPCD_DSC_CAP_LEN];
  DP_ReadPacketType               sReadPacket;

  /*Read DPCD Data*/
  DP_OSAL_MemSet(&sReadPacket, 0, sizeof(DP_ReadPacketType));
  sReadPacket.eTransactionType     = DP_TRANSACTION_MODE_AUX;
  sReadPacket.puReadDataBuffer     = (uint8*)&uPsrBuf;
  sReadPacket.uAddress             = DPCD_DSC_CAP_START_ADDRESS;
  sReadPacket.uReadSize            = DPCD_DSC_CAP_LEN;
  sReadPacket.uBytesRead           = 0;
  sReadPacket.bQueueOnly           = FALSE;

  if (DP_STATUS_SUCCESS != (eStatus = DP_Host_ClientRead(pDeviceCtx, &sReadPacket)))
  {
    DP_LOG_MESSAGE_ERROR("DP_ReadSinkDSCCap: Read Failed, eStatus=0x%x\n", eStatus);
  }
  else if (sReadPacket.uBytesRead != sReadPacket.uReadSize)
  {
    eStatus = DP_STATUS_FAILED_RESOURCE_FAILED;
    DP_LOG_MESSAGE_ERROR("DP_ReadSinkDSCCap: Bytes Read(0x%x) is not expected\n", sReadPacket.uBytesRead);
  }
  else
  {
     DP_ParseSinkDSCCap(uPsrBuf, &pDeviceCtx->sDSC);

     if ((pDeviceCtx->sDSC.bSupported == TRUE) &&
         (pDeviceCtx->sFEC.bCapable == TRUE))
     {
         pDeviceCtx->bDSCSupported = TRUE; 
         pDeviceCtx->sDSC.pPPSBuf  = pDeviceCtx->auPPSBuffer;  /* populated dsc PPS buffer */
     }
  }

  return eStatus;
}
/**********************************************************************************************************************

FUNCTION: DP_ReadSinkDSCStatus()

This function will read sink DSC Status

Parameters:
   pDeviceCtx                       -[in ] Pointer to device context

Return:
DP_Status

**********************************************************************************************************************/
DP_Status  DP_ReadSinkDSCStatus(DP_DeviceCtxType      *pDeviceCtx)
{
  DP_Status                       eStatus       = DP_STATUS_SUCCESS;  
  uint8                           uPsrBuf[DPCD_DSC_STATUS_LEN];
  DP_ReadPacketType               sReadPacket;

  /*Read DPCD Data*/
  DP_OSAL_MemSet(&sReadPacket, 0, sizeof(DP_ReadPacketType));
  sReadPacket.eTransactionType     = DP_TRANSACTION_MODE_AUX;
  sReadPacket.puReadDataBuffer     = (uint8*)&uPsrBuf;
  sReadPacket.uAddress             = DPCD_DSC_STATUS_START_ADDRESS;
  sReadPacket.uReadSize            = DPCD_DSC_STATUS_LEN;
  sReadPacket.uBytesRead           = 0;
  sReadPacket.bQueueOnly           = FALSE;

  if (DP_STATUS_SUCCESS != (eStatus = DP_Host_ClientRead(pDeviceCtx, &sReadPacket)))
  {
    DP_LOG_MESSAGE_ERROR("DP_ReadSinkDSCStatus: Read Failed, eStatus=0x%x.\n", eStatus);
  }
  else if (sReadPacket.uBytesRead != sReadPacket.uReadSize)
  {
    eStatus = DP_STATUS_FAILED_RESOURCE_FAILED;
    DP_LOG_MESSAGE_ERROR("DP_ReadSinkDSCStatus: Bytes Read(0x%x) is not expected.\n", sReadPacket.uBytesRead);
  }
  else
  {
    DP_LOG_MESSAGE_ERROR("DP_ReadSinkDSCStatus: status=%x\n", uPsrBuf[0]);
  }

  return eStatus;
}

/**********************************************************************************************************************

FUNCTION: DP_WriteSinkDSCEnable()

This function will write sink DSC enable. 

Parameters:
   pDeviceCtx                       -[in ] Pointer to device context

Return:
DP_Status

**********************************************************************************************************************/
DP_Status  DP_WriteSinkDSCEnable(DP_DeviceCtxType      *pDeviceCtx)
{
     DP_Status           eStatus       = DP_STATUS_SUCCESS;  
     uint8               data          = 0;
     DP_WritePacketType  sWritePacket;

     data = 0x01; /* enabled */
     /* write DPCD Data*/
     DP_OSAL_MemSet(&sWritePacket, 0, sizeof(DP_WritePacketType));
     sWritePacket.eTransactionType    = DP_TRANSACTION_MODE_AUX;
     sWritePacket.puWriteData         = &data;
     sWritePacket.uAddress            = DPCD_DSC_ENABLE_START_ADDRESS;
     sWritePacket.uWriteDataSize      = DPCD_DSC_ENABLE_LEN;
     sWritePacket.uBytesWritten       = 0;
     sWritePacket.bQueueOnly          = FALSE;

     eStatus = DP_Host_ClientWrite(pDeviceCtx, &sWritePacket);
     if (DP_STATUS_SUCCESS != eStatus)
     {
        DP_LOG_MESSAGE_ERROR( "DP_WriteSinkDSCEnable: Write Failed, eStatus=0x%x\n",eStatus);
     }

     return eStatus;
}

/**********************************************************************************************************************
FUNCTION: DP_ReadSinkDSCEnable()

This function will read sink DSC enable Status

Parameters:
   pDeviceCtx                       -[in ] Pointer to device context

Return:
DP_Status

**********************************************************************************************************************/
DP_Status  DP_ReadSinkDSCEnable(DP_DeviceCtxType      *pDeviceCtx)
{
  DP_Status                       eStatus       = DP_STATUS_SUCCESS;  
  uint8                           uPsrBuf[DPCD_DSC_ENABLE_LEN];
  DP_ReadPacketType               sReadPacket;

  /*Read DPCD Data*/
  DP_OSAL_MemSet(&sReadPacket, 0, sizeof(DP_ReadPacketType));
  sReadPacket.eTransactionType     = DP_TRANSACTION_MODE_AUX;
  sReadPacket.puReadDataBuffer     = (uint8*)&uPsrBuf;
  sReadPacket.uAddress             = DPCD_DSC_ENABLE_START_ADDRESS;
  sReadPacket.uReadSize            = DPCD_DSC_ENABLE_LEN;
  sReadPacket.uBytesRead           = 0;
  sReadPacket.bQueueOnly           = FALSE;

  if (DP_STATUS_SUCCESS != (eStatus = DP_Host_ClientRead(pDeviceCtx, &sReadPacket)))
  {
    DP_LOG_MESSAGE_ERROR("DP_ReadSinkDSCEnable: Read Failed, eStatus=0x%x.\n", eStatus);
  }
  else if (sReadPacket.uBytesRead != sReadPacket.uReadSize)
  {
    eStatus = DP_STATUS_FAILED_RESOURCE_FAILED;
    DP_LOG_MESSAGE_ERROR("DP_ReadSinkDSCEnable: Bytes Read(0x%x) is not expected.\n", sReadPacket.uBytesRead);
  }
  else
  {
    DP_LOG_MESSAGE_ERROR("DP_ReadSinkDSCEnable: DSCenable=%x\n", uPsrBuf[0]);
  }

  return eStatus;
}

/**********************************************************************************************************************

FUNCTION: DP_ParseSinkFECCap()

This function will parser sink FEC Capabilities register 

Parameters:
   pPsrBuf                       -[in ] buffer contains sink PSR data
   pSinkDSCCap                   -[in ] pointer of DSC struct

Return:
DP_Status

**********************************************************************************************************************/
static void  DP_ParseSinkFECCap(uint8              *pPsrBuf,
                                DP_SinkFECCapType  *pSinkFECCap)
{
  uint8  uData = 0;

  if (pPsrBuf != NULL) 
  {
     uData = pPsrBuf[0]; /* dpcd 0x90 */
     if (uData & 0x01)
     {
        pSinkFECCap->bCapable = TRUE;
     }
     if (uData & 0x02)
     {
        pSinkFECCap->bUncorrectedBlockErrorCountCapable = TRUE;
     }
     if (uData & 0x04)
     {
        pSinkFECCap->bCorrectedBlockErrorCountCapable = TRUE;
     }
     if (uData & 0x08)
     {
        pSinkFECCap->bBitErrorCountCapable = TRUE;
     }

  }

  DP_LOG_MESSAGE_INFO("DP_ParseSinkFECCap: capable=%d %d %d %d\n", 
                                            pSinkFECCap->bCapable,
                                            pSinkFECCap->bUncorrectedBlockErrorCountCapable,
                                            pSinkFECCap->bCorrectedBlockErrorCountCapable,
                                            pSinkFECCap->bBitErrorCountCapable);
}

/**********************************************************************************************************************

FUNCTION: DP_ReadSinkFECCap()

This function will read sink FEC Capabilities register and parser them. 

Parameters:
   pDeviceCtx                       -[in ] Pointer to device context

Return:
DP_Status

**********************************************************************************************************************/
DP_Status  DP_ReadSinkFECCap(DP_DeviceCtxType      *pDeviceCtx)
{
  DP_Status                       eStatus       = DP_STATUS_SUCCESS;  
  uint8                           uPsrBuf[DPCD_FEC_CAP_LEN];
  DP_ReadPacketType               sReadPacket;

  /*Read DPCD Data*/
  DP_OSAL_MemSet(&sReadPacket, 0, sizeof(DP_ReadPacketType));
  sReadPacket.eTransactionType     = DP_TRANSACTION_MODE_AUX;
  sReadPacket.puReadDataBuffer     = (uint8*)&uPsrBuf;
  sReadPacket.uAddress             = DPCD_FEC_CAP_START_ADDRESS;
  sReadPacket.uReadSize            = DPCD_FEC_CAP_LEN;
  sReadPacket.uBytesRead           = 0;
  sReadPacket.bQueueOnly           = FALSE;

  if (DP_STATUS_SUCCESS != (eStatus = DP_Host_ClientRead(pDeviceCtx, &sReadPacket)))
  {
    DP_LOG_MESSAGE_ERROR("DP_ReadSinkFECCap: Read Failed, eStatus=0x%x\n", eStatus);
  }
  else if (sReadPacket.uBytesRead != sReadPacket.uReadSize)
  {
    eStatus = DP_STATUS_FAILED_RESOURCE_FAILED;
    DP_LOG_MESSAGE_ERROR("DP_ReadSinkFECCap: Bytes Read(0x%x) is not expected\n", sReadPacket.uBytesRead);
  }
  else
  {
    DP_ParseSinkFECCap(uPsrBuf, &pDeviceCtx->sFEC);
  }

  return eStatus;
}

/**********************************************************************************************************************

FUNCTION: DP_ReadSinkFECStatus()

This function will read sink FEC Status

Parameters:
   pDeviceCtx                       -[in ] Pointer to device context

Return:
DP_Status

**********************************************************************************************************************/
DP_Status  DP_ReadSinkFECStatus(DP_DeviceCtxType      *pDeviceCtx)
{
  DP_Status                       eStatus       = DP_STATUS_SUCCESS;  
  uint8                           uPsrBuf[DPCD_FEC_STATUS_LEN];
  DP_ReadPacketType               sReadPacket;

  /* Read DPCD Data */
  DP_OSAL_MemSet(&sReadPacket, 0, sizeof(DP_ReadPacketType));
  sReadPacket.eTransactionType     = DP_TRANSACTION_MODE_AUX;
  sReadPacket.puReadDataBuffer     = (uint8*)&uPsrBuf;
  sReadPacket.uAddress             = DPCD_FEC_STATUS_START_ADDRESS;
  sReadPacket.uReadSize            = DPCD_FEC_STATUS_LEN;
  sReadPacket.uBytesRead           = 0;
  sReadPacket.bQueueOnly           = FALSE;

  if (DP_STATUS_SUCCESS != (eStatus = DP_Host_ClientRead(pDeviceCtx, &sReadPacket)))
  {
    DP_LOG_MESSAGE_ERROR("DP_ReadSinkFECStatus: Read Failed, eStatus=0x%x.\n", eStatus);
  }
  else if (sReadPacket.uBytesRead != sReadPacket.uReadSize)
  {
    eStatus = DP_STATUS_FAILED_RESOURCE_FAILED;
    DP_LOG_MESSAGE_ERROR("DP_ReadSinkFECStatus: Bytes Read(0x%x) is not expected.\n", sReadPacket.uBytesRead);
  }
  else
  {
    DP_LOG_MESSAGE_ERROR("DP_ReadSinkFECStatus: detected=%x, error=%x %x\n", uPsrBuf[0],uPsrBuf[1],uPsrBuf[2]);
  }

  return eStatus;
}

/**********************************************************************************************************************

FUNCTION: DP_WriteSinkFECConfig()

This function will write sink FEC config. 

Parameters:
   pDeviceCtx                       -[in ] Pointer to device context

Return:
DP_Status

**********************************************************************************************************************/
DP_Status  DP_WriteSinkFECConfig(DP_DeviceCtxType      *pDeviceCtx)
{
     DP_Status           eStatus       = DP_STATUS_SUCCESS;  
     uint8               data          = 0;
     DP_WritePacketType  sWritePacket;

     /* Fec ready, bit error count, link 0 for error */
     data = 0x07;
     /* Write DPCD Data */
     DP_OSAL_MemSet(&sWritePacket, 0, sizeof(DP_WritePacketType));
     sWritePacket.eTransactionType    = DP_TRANSACTION_MODE_AUX;
     sWritePacket.puWriteData         = &data;
     sWritePacket.uAddress            = DPCD_FEC_CONFIG_START_ADDRESS;
     sWritePacket.uWriteDataSize      = DPCD_FEC_CONFIG_LEN;
     sWritePacket.uBytesWritten       = 0;
     sWritePacket.bQueueOnly          = FALSE;

     eStatus = DP_Host_ClientWrite(pDeviceCtx, &sWritePacket);
     if (DP_STATUS_SUCCESS != eStatus)
     {
        DP_LOG_MESSAGE_ERROR( "DP_WriteSinkFECConfig: Write Failed, eStatus=0x%x\n",eStatus);
     }

     return eStatus;
}

/**********************************************************************************************************************

FUNCTION: DP_ReadSinkFECEnable()

This function will read sink FEC Status

Parameters:
   pDeviceCtx                       -[in ] Pointer to device context

Return:
DP_Status

**********************************************************************************************************************/
DP_Status  DP_ReadSinkFECEnable(DP_DeviceCtxType      *pDeviceCtx)
{
  DP_Status                       eStatus       = DP_STATUS_SUCCESS;  
  uint8                           uPsrBuf[DPCD_FEC_CONFIG_LEN];
  DP_ReadPacketType               sReadPacket;

  /* Read DPCD Data */
  DP_OSAL_MemSet(&sReadPacket, 0, sizeof(DP_ReadPacketType));
  sReadPacket.eTransactionType     = DP_TRANSACTION_MODE_AUX;
  sReadPacket.puReadDataBuffer     = (uint8*)&uPsrBuf;
  sReadPacket.uAddress             = DPCD_FEC_CONFIG_START_ADDRESS;
  sReadPacket.uReadSize            = DPCD_FEC_CONFIG_LEN;
  sReadPacket.uBytesRead           = 0;
  sReadPacket.bQueueOnly           = FALSE;

  if (DP_STATUS_SUCCESS != (eStatus = DP_Host_ClientRead(pDeviceCtx, &sReadPacket)))
  {
    DP_LOG_MESSAGE_ERROR("DP_ReadSinkFECEnable: Read Failed, eStatus=0x%x.\n", eStatus);
  }
  else if (sReadPacket.uBytesRead != sReadPacket.uReadSize)
  {
    eStatus = DP_STATUS_FAILED_RESOURCE_FAILED;
    DP_LOG_MESSAGE_ERROR("DP_ReadSinkFECEnable: Bytes Read(0x%x) is not expected.\n", sReadPacket.uBytesRead);
  }
  else
  {
    DP_LOG_MESSAGE_ERROR("DP_ReadSinkFECEnable: Enable=%x\n", uPsrBuf[0]);
  }

  return eStatus;
}

/**********************************************************************************************************************

FUNCTION: DP_ReadSinkBrightnessCap()

This function will read Sink Brightness Capability over AUX Channel

Parameters:
  pDeviceCtx                        -[in ] Pointer to device context

Return:
  DP_Status

**********************************************************************************************************************/
DP_Status  DP_ReadSinkBrightnessCap(DP_DeviceCtxType *pDeviceCtx)
{
  DP_Status                       eStatus       = DP_STATUS_SUCCESS;  
  uint8                           uRawData[DPCD_BRIGHTNESS_CAP_LEN];
  DP_ReadPacketType               sReadPacket;

  /*Read DPCD Data*/
  DP_OSAL_MemSet(&sReadPacket, 0, sizeof(DP_ReadPacketType));
  DP_OSAL_MemSet(&uRawData,    0, sizeof(uRawData));
  sReadPacket.eTransactionType     = DP_TRANSACTION_MODE_AUX;
  sReadPacket.puReadDataBuffer     = (uint8*)&uRawData;
  sReadPacket.uAddress             = DPCD_BRIGHTNESS_CAP_START_ADDRESS;
  sReadPacket.uReadSize            = DPCD_BRIGHTNESS_CAP_LEN;
  sReadPacket.uBytesRead           = 0;
  sReadPacket.bQueueOnly           = FALSE;

  if (DP_STATUS_SUCCESS != (eStatus = DP_Host_ClientRead(pDeviceCtx, &sReadPacket)))
  {
    DP_LOG_MESSAGE_ERROR("DP_ReadSinkBrightnessCap: Read Failed, eStatus=0x%x.\n", eStatus);
  }
  else if (sReadPacket.uBytesRead != sReadPacket.uReadSize)
  {
    eStatus = DP_STATUS_FAILED_RESOURCE_FAILED;
    DP_LOG_MESSAGE_ERROR("DP_ReadSinkBrightnessCap: Bytes Read(0x%x) is not expected.\n", sReadPacket.uBytesRead);
  }
  else
  {
    DP_ParseSinkBrightnessCap(uRawData, &pDeviceCtx->sBrightnessCaps);
  }

  return eStatus;
}

  
/**********************************************************************************************************************

FUNCTION: DP_ParseSinkBrightnessCap()

This function will parse sink Brightness Capabilities data 

Parameters:
  pRawData                        -[in ] Buffer containing sink Brightness cap data
  pSinkBrightnessCap              -[in ] Pointer of Brightness cap struct

Return:
  None

**********************************************************************************************************************/
static void  DP_ParseSinkBrightnessCap(uint8                    *pRawData,
                                       DP_SinkBrightnessCapType *pSinkBrightnessCap)
{  
  if (pRawData != NULL) 
  {
    DP_LOG_MESSAGE_INFO("DP: 701h (EDP General Cap) - Raw: 0x%x, \
                        Brightness Adjust: 0x%x, Pin enable: 0x%x, Aux enable: 0x%x \n", 
                        pRawData[0],
                        DPHOST_GETFLAG(pRawData[0], DP_GENERAL_CAP_TCON_BACKLIGHT_ADJUST), 
                        DPHOST_GETFLAG(pRawData[0], DP_GENERAL_CAP_BACKLIGHT_PIN_ENABLE), 
                        DPHOST_GETFLAG(pRawData[0], DP_GENERAL_CAP_BACKLIGHT_AUX_ENABLE));
    
    DP_LOG_MESSAGE_INFO("DP: 702h (EDP Backlight Adjustment Cap) - Raw: 0x%x, \
                        Pin level set: 0x%x, Aux level set: 0x%x, Level bit depth: 0x%x \n", 
                        pRawData[1],
                        DPHOST_GETFLAG(pRawData[1], DP_BACKLIGHT_CAP_BRIGHTNESS_PWM_PIN), 
                        DPHOST_GETFLAG(pRawData[1], DP_BACKLIGHT_CAP_BRIGHTNESS_AUX_SET), 
                        DPHOST_GETFLAG(pRawData[1], DP_BACKLIGHT_CAP_BRIGHTNESS_BYTE_COUNT));
  
    /* if sink supports brightness control save required caps */
    if (DPHOST_GETFLAG(pRawData[0], DP_GENERAL_CAP_TCON_BACKLIGHT_ADJUST))
    {
      /* set aux cap if both enable and level control are supported */
      pSinkBrightnessCap->bAuxControlCapable = (DPHOST_GETFLAG(pRawData[0], DP_GENERAL_CAP_BACKLIGHT_AUX_ENABLE)  &
                                                DPHOST_GETFLAG(pRawData[1], DP_BACKLIGHT_CAP_BRIGHTNESS_AUX_SET)) ?
                                               TRUE : FALSE;

      /* fetch the bit depth for brightness level */
      if (DPHOST_GETFLAG(pRawData[1], DP_BACKLIGHT_CAP_BRIGHTNESS_BYTE_COUNT))
      {
        pSinkBrightnessCap->eLevelBitDepth    = DPCD_BRIGHTNESS_CONTROL_16BIT;
      }
      else
      {
        pSinkBrightnessCap->eLevelBitDepth    = DPCD_BRIGHTNESS_CONTROL_8BIT;
      }
    }
  }
}

  
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
                                        bool32            bEnable)
{
  DP_Status                eStatus       = DP_STATUS_SUCCESS;

  if (0 == pDeviceCtx->sBrightnessCaps.bAuxControlCapable)
  {
    eStatus = DP_STATUS_FAILED_NOT_SUPPORTED;
    DP_LOG_MESSAGE_ERROR("DP_WriteSinkBrightnessEnable: Backlight control through AUX is not suported!\n");  
  }
  else
  {
    if (bEnable)
    {
      /* Set backlight mode first during enable */
      if (DP_STATUS_SUCCESS != DP_ModifySinkDPCDReg(pDeviceCtx, 
                                                    DPCD_BACKLIGHT_MODE_START_ADDRESS, 
                                                    DPCD_BACKLIGHT_BRIGHTNESS_CTRL_MODE_MASK,
                                                    DPCD_BACKLIGHT_BRIGHTNESS_CTRL_BY_REG))
      {
        DP_LOG_MESSAGE_WARN("DP_WriteSinkBrightnessEnable: Failed to set brightness mode!\n");
      }

      /* Set duty cycle to 0 to prevent flash to random level when enabled */
      if (DP_STATUS_SUCCESS != DP_WriteSinkBrightnessLevel(pDeviceCtx, 0))
      {
        DP_LOG_MESSAGE_WARN("DP_WriteSinkBrightnessEnable: Failed to clear brightness level!\n");
      }
    }

    /* Set backlight state */
    if (DP_STATUS_SUCCESS != (eStatus = DP_ModifySinkDPCDReg(pDeviceCtx, 
                                        DPCD_DISPLAY_CONTROL_START_ADDRESS, 
                                        DPCD_BACKLIGHT_ENABLE_MASK,
                                        bEnable ? DPCD_BACKLIGHT_ENABLE_MASK : 0x0)))
    {
      DP_LOG_MESSAGE_ERROR("DP_WriteSinkBrightnessEnable: Failed to enable brightness!\n");
    }
  }

  return eStatus;
}

  
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
                                       float             fLevel)
{
  DP_Status                eStatus       = DP_STATUS_SUCCESS;

  if (0 == pDeviceCtx->sBrightnessCaps.bAuxControlCapable)
  {
    eStatus = DP_STATUS_FAILED_NOT_SUPPORTED;
    DP_LOG_MESSAGE_ERROR("DP_WriteSinkBrightnessLevel: Backlight control through AUX is not suported!\n");  
  }
  else
  {
    DP_WritePacketType    sWritePacket;
    uint8                 uData[2];
    uint32                uDataSize           = 0;
    uint32                uLevelConverted     = 0;
    
    if (DPCD_BRIGHTNESS_CONTROL_16BIT == pDeviceCtx->sBrightnessCaps.eLevelBitDepth)
    {
      /* Sink is using both MSB (722h) and LSB (723h) */
      uDataSize       = 2;
      uLevelConverted = DPHOST_FLOAT_TO_INT(fLevel, DPCD_BACKLIGHT_BRIGHTNESS_16BIT_MAX);
      uData[1]        = (uint8)uLevelConverted;         // LSB
      uData[0]        = (uint8)(uLevelConverted >> 8);  // MSB
    }
    else if (DPCD_BRIGHTNESS_CONTROL_8BIT == pDeviceCtx->sBrightnessCaps.eLevelBitDepth)
    {
      /* Sink is using only MSB (722h) */
      uDataSize       = 1;
      uLevelConverted = DPHOST_FLOAT_TO_INT(fLevel, DPCD_BACKLIGHT_BRIGHTNESS_8BIT_MAX);
      uData[0]        = (uint8)uLevelConverted;
    }
    
    /* Write DPCD Data*/
    DP_OSAL_MemSet(&sWritePacket, 0, sizeof(DP_WritePacketType));
    sWritePacket.eTransactionType    = DP_TRANSACTION_MODE_AUX;
    sWritePacket.uAddress            = DPCD_BACKLIGHT_BRIGHTNESS_MSB_ADDRESS;
    sWritePacket.puWriteData         = uData;
    sWritePacket.uWriteDataSize      = uDataSize;
    sWritePacket.uBytesWritten       = 0;
    sWritePacket.bQueueOnly          = FALSE;

    if (DP_STATUS_SUCCESS != (eStatus = DP_Host_ClientWrite(pDeviceCtx, &sWritePacket)))
    {
      DP_LOG_MESSAGE_ERROR("DP_WriteSinkBrightnessLevel: Write Failed, eStatus=0x%x.\n", eStatus);
    }
  }

  return eStatus;
}


/**********************************************************************************************************************

FUNCTION: DP_ModifySinkDPCDReg()

Function to read and update selective bits within a register

Parameters:
  pDeviceCtx                      -[in ] Pointer to device context
  uRegAddress                     -[in ] Address of the DPCD register to modify
  uBitMask                        -[in ] Mask for the specific bits in the register
  uNewValue                       -[in ] Updated bit values in exact positions

Return:
  DP_Status

**********************************************************************************************************************/
static DP_Status  DP_ModifySinkDPCDReg(DP_DeviceCtxType *pDeviceCtx,
                                       uint32            uRegAddress,
                                       uint8             uBitMask,
                                       uint8             uNewValue)
{
  DP_Status                eStatus       = DP_STATUS_SUCCESS;
  uint8                    uRawData;
  DP_ReadPacketType        sReadPacket;
  DP_WritePacketType       sWritePacket;

  /* Read DPCD Data */
  DP_OSAL_MemSet(&sReadPacket, 0, sizeof(DP_ReadPacketType));
  DP_OSAL_MemSet(&uRawData,    0, sizeof(uRawData));

  sReadPacket.eTransactionType     = DP_TRANSACTION_MODE_AUX;
  sReadPacket.puReadDataBuffer     = (uint8*)&uRawData;
  sReadPacket.uAddress             = uRegAddress;
  sReadPacket.uReadSize            = 1;
  sReadPacket.uBytesRead           = 0;
  sReadPacket.bQueueOnly           = FALSE;

  if (DP_STATUS_SUCCESS != (eStatus = DP_Host_ClientRead(pDeviceCtx, &sReadPacket)))
  {
    DP_LOG_MESSAGE_ERROR("DP_ModifySinkDPCDReg: Read Failed at Address=0x%x, eStatus=0x%x.\n", uRegAddress, eStatus);
  }
  else if (sReadPacket.uBytesRead != sReadPacket.uReadSize)
  {
    eStatus = DP_STATUS_FAILED_RESOURCE_FAILED;
    DP_LOG_MESSAGE_ERROR("DP_ModifySinkDPCDReg: Bytes Read(0x%x) is not expected(0x1).\n", sReadPacket.uBytesRead);
  }
  else
  {  
    /* Get the current value */
    uint8 uCurrentValue = uRawData & uBitMask;
  
    /* Modify only the required bits */
    uint8 uNewData      = (uRawData & ~uBitMask) | uNewValue;

    /* Send new value only if it is different */
    if (uCurrentValue != uNewValue)
    {
      /* Write DPCD Data*/
      DP_OSAL_MemSet(&sWritePacket, 0, sizeof(DP_WritePacketType));
      sWritePacket.eTransactionType    = DP_TRANSACTION_MODE_AUX;
      sWritePacket.puWriteData         = &uNewData;
      sWritePacket.uAddress            = uRegAddress;
      sWritePacket.uWriteDataSize      = 1;
      sWritePacket.uBytesWritten       = 0;
      sWritePacket.bQueueOnly          = FALSE;

      eStatus = DP_Host_ClientWrite(pDeviceCtx, &sWritePacket);
      if (DP_STATUS_SUCCESS != eStatus)
      {
        DP_LOG_MESSAGE_ERROR("DP_ModifySinkDPCDReg: Write Failed at Address=0x%x, eStatus=0x%x.\n",
                              uRegAddress, eStatus);
      }
    }
  }

  return eStatus;
}


#ifdef __cplusplus
}
#endif

