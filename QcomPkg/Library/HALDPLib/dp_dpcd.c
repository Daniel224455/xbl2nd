/*=====================================================================================================================
 
  File: dp_dpcd.c
 
  Source file for DP EDID read and parse related functionality 
   
  Copyright (c) 2016-2018 Qualcomm Technologies, Inc.  All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential.
=======================================================================================================================*/


/*---------------------------------------------------------------------------------------------------------------------
 * Include Files
 *-------------------------------------------------------------------------------------------------------------------*/
#include "dp_i.h"

#ifdef __cplusplus
extern "C" {
#endif

/*---------------------------------------------------------------------------------------------------------------------
 * Defines
 *-------------------------------------------------------------------------------------------------------------------*/
#define DPCD_CAP_LEN                               16
#define DPCD_CAP_START_ADDRESS                     0

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

#define DPCD_SET_POWER_STATE_LEN                   1
#define DPCD_SET_POWER_STATE_START_ADDRESS         0x600



/*---------------------------------------------------------------------------------------------------------------------
 * Function pre-declarations
 *-------------------------------------------------------------------------------------------------------------------*/
static DP_Status  DP_ParseSinkDPCDCap(
  DP_DPCDSinkCapRawDataType       *psDPCDCapRawData,
  DP_DPCDSinkCapType              *psDPCDSinkCap);

static DP_Status  DP_ParseSinkDPCDStatus(
  DP_DPCDSinkStatusRawDataType    *psDPCDSinkStatusRawData,
  DP_DPCDSinkStatusType           *psDPCDSinkStatus);

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
  switch(psDPCDCapRawData->uMaxLinkRate)
  {
    case  DPCD_SPEC_MAX_LINK_RATE_1_62GBPS:
    {
      psDPCDSinkCap->uMaxLinkRateInKhz = 162000;
      break;
    }
    case  DPCD_SPEC_MAX_LINK_RATE_2_7GBPS:
    {
      psDPCDSinkCap->uMaxLinkRateInKhz = 270000;
      break;
    }
    case  DPCD_SPEC_MAX_LINK_RATE_5_4GBPS:
    {
      psDPCDSinkCap->uMaxLinkRateInKhz = 540000;
      break;
    }
    case  DPCD_SPEC_MAX_LINK_RATE_8_1GBPS:
    {
      psDPCDSinkCap->uMaxLinkRateInKhz = 810000;
      break;
    }
    default:
    {
      psDPCDSinkCap->uMaxLinkRateInKhz = 0;
      break;
    }
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
    psDPCDSinkCap->uTrainingIntervalInUs      = 100;
  }
  else
  {
    psDPCDSinkCap->uTrainingIntervalInUs      = (psDPCDCapRawData->uTrainingAUXRdInterval & 0x7F) * 4000;
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

  /* Byte 16: DPRX_FEATURE_ENUMERATION_LIST */
  if (psDPCDCapRawData->uFeatureEnumerationList & 0x01)
  {
    psDPCDSinkCap->eDPCDCap    |= DP_DPCD_CAP_SUPPORT_GTC;
  }
  if (psDPCDCapRawData->uFeatureEnumerationList & 0x04)
  {
    psDPCDSinkCap->eDPCDCap    |= DP_DPCD_CAP_SUPPORT_AV_SYNC;
  }
  if (psDPCDCapRawData->uFeatureEnumerationList & 0x08)
  {
    psDPCDSinkCap->eDPCDCap    |= DP_DPCD_CAP_SUPPORT_VSC_SDP;
  }

  /* Byte 17: EXTENDED_DPRX_SLEEP_WAKE_TIMEOUT_REQUEST */
  if (0 == psDPCDCapRawData->uSleepWakeTimeout)
  {
    psDPCDSinkCap->uSleepWakeTimeoutInMs = 1;
  }
  else
  {
    psDPCDSinkCap->uSleepWakeTimeoutInMs = psDPCDCapRawData->uSleepWakeTimeout * 20;
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
   eDeviceId                        -[in ] DP device id
   psDPCDSinkCap                    -[out] Pointer to DPCD Sink Capability Data struct.

Return: 
   DP_Status 

**********************************************************************************************************************/
DP_Status  DP_ReadSinkDPCDCap(DP_DeviceIDType           eDeviceId,
                              DP_DPCDSinkCapType       *psDPCDSinkCap)
{
  DP_Status                       eStatus        = DP_STATUS_SUCCESS;  
  DP_DPCDSinkCapRawDataType       sDPCDCapRawData;
  DP_ReadPacketType               sReadPacket;

  /*Read DPCD Data*/
  sReadPacket.eTransactionType     = DP_TRANSACTION_MODE_AUX;
  sReadPacket.puReadDataBuffer     = (uint8*)&sDPCDCapRawData;
  sReadPacket.uAddress             = DPCD_CAP_START_ADDRESS;
  sReadPacket.uReadSize            = sizeof(DP_DPCDSinkCapRawDataType);
  sReadPacket.uBytesRead           = 0;
  sReadPacket.bQueueOnly           = FALSE;
  
  eStatus = DP_Host_ClientRead(eDeviceId, &sReadPacket);

  if (DP_STATUS_SUCCESS != eStatus)
  {
    DP_LOG_MESSAGE_ERROR( "DP_ReadDPCDSinkCap() : Read Failed, eStatus=0x%x.\n",eStatus);
  }
  else if (sReadPacket.uBytesRead != sReadPacket.uReadSize)
  {
    eStatus  = DP_STATUS_FAILED_RESOURCE_FAILED;
    DP_LOG_MESSAGE_ERROR( "DP_ReadDPCDSinkCap() : Bytes Read(0x%x) is not expected.\n",sReadPacket.uBytesRead);
  }
  else
  {
    DP_LOG_MESSAGE_INFO( "DP_ReadDPCDSinkCap() : uDPCDRevision=%x uMaxLinkRate=%x uMaxLaneCount=%x\n", sDPCDCapRawData.uDPCDRevision, sDPCDCapRawData.uMaxLinkRate, sDPCDCapRawData.uMaxLaneCount);

    eStatus = DP_ParseSinkDPCDCap(&sDPCDCapRawData, psDPCDSinkCap);
  }

  return eStatus;
}

/**********************************************************************************************************************

FUNCTION: DP_ReadSinkDPCDStatus()
   
   This function will read Sink Status Raw Data from DPCD over AUX Channel

Parameters:
   eDeviceId                        -[in ] DP device id
   psDPCDSinkStatus                 -[out] Pointer to DPCD Sink Status Data struct.

Return: 
   DP_Status 

**********************************************************************************************************************/
DP_Status  DP_ReadSinkDPCDStatus(DP_DeviceIDType           eDeviceId,
                                 DP_DPCDSinkStatusType    *psDPCDSinkStatus)
{
  DP_Status                       eStatus        = DP_STATUS_SUCCESS;
  DP_DPCDSinkStatusRawDataType    sDPCDSinkStatusRawData;
  DP_ReadPacketType               sReadPacket;

  /*Read DPCD Data*/
  sReadPacket.eTransactionType     = DP_TRANSACTION_MODE_AUX;
  sReadPacket.puReadDataBuffer     = (uint8*)&sDPCDSinkStatusRawData;
  sReadPacket.uAddress             = DPCD_LINK_STATUS_START_ADDRESS;
  sReadPacket.uReadSize            = sizeof(DP_DPCDSinkStatusRawDataType);
  sReadPacket.uBytesRead           = 0;
  sReadPacket.bQueueOnly           = FALSE;
  
  eStatus = DP_Host_ClientRead(eDeviceId, &sReadPacket);

  if (DP_STATUS_SUCCESS != eStatus)
  {
    DP_LOG_MESSAGE_ERROR( "DP_ReadSinkDPCDStatus() : Read Failed, eStatus=0x%x.\n",eStatus);
  }
  else if (sReadPacket.uBytesRead != sReadPacket.uReadSize)
  {
    eStatus = DP_STATUS_FAILED_RESOURCE_FAILED;
    DP_LOG_MESSAGE_ERROR( "DP_ReadSinkDPCDStatus() : Bytes Read(0x%x) is not expected.\n",sReadPacket.uBytesRead);
  }
  else
  {
    eStatus = DP_ParseSinkDPCDStatus(&sDPCDSinkStatusRawData, psDPCDSinkStatus);
  }

  return eStatus;
}

/**********************************************************************************************************************

FUNCTION: DP_WriteSinkDPCDLink()
   
   This function will Write the link setting to panel DPCD 

Parameters:
   eDeviceId                        -[in ] DP device id

Return: 
   DP_Status 

**********************************************************************************************************************/
DP_Status  DP_WriteSinkDPCDLink(DP_DeviceIDType           eDeviceId)
{
  DP_Status                 eStatus       = DP_STATUS_SUCCESS; 
  DP_DeviceCtxType         *pDeviceCtx    = DPHOST_GET_CTX(eDeviceId); 
  DP_WritePacketType        sWritePacket;
  uint8                     uWriteBuffer[2];

  /* LINK_BW_SET */
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
      DP_LOG_MESSAGE_ERROR("DP_WriteSinkDPCDLink() : Invalid input parameter.\n");
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
    sWritePacket.eTransactionType    = DP_TRANSACTION_MODE_AUX;
    sWritePacket.puWriteData         = uWriteBuffer;
    sWritePacket.uAddress            = DPCD_LINKCONFIG_START_ADDRESS;
    sWritePacket.uWriteDataSize      = DPCD_LINKCONFIG_LEN;
    sWritePacket.uBytesWritten       = 0;
    sWritePacket.bQueueOnly          = FALSE;
    
    eStatus = DP_Host_ClientWrite(eDeviceId, &sWritePacket);

    if (DP_STATUS_SUCCESS != eStatus)
    {
      DP_LOG_MESSAGE_ERROR( "DP_WriteSinkDPCDLink() : Write Failed, eStatus=0x%x.\n",eStatus);
    }

    if (TRUE == pDeviceCtx->bEnableSSCMode)
    {
      eStatus = DP_WriteSinkDPCDDownSpreadEnabled(eDeviceId);
    }
  }

  return eStatus;
}

/**********************************************************************************************************************

FUNCTION: DP_WriteSinkDPCDTrainingPattern()
   
   This function will select the training pattern for link training to panel DPCD 

Parameters:
   eDeviceId                        -[in ] DP device id
   eTraingPattern                   -[in ] Link Training pattern 

Return: 
   DP_Status 

**********************************************************************************************************************/
DP_Status  DP_WriteSinkDPCDTrainingPattern(DP_DeviceIDType                  eDeviceId,
                                           DP_DPCDTrainingPatternSetType    eTraingPattern)
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
      uWriteBuffer = DPCD_SPEC_TRAINING_PATTERN_3;
      break;
    }    
    default:
    {
      eStatus = DP_STATUS_FAILED_INVALID_INPUT_PARAMETER;
      DP_LOG_MESSAGE_ERROR("DP_WriteSinkDPCDTrainingPattern(): Invalid input parameter\n");
      break;
    }
  }

  if (DP_STATUS_SUCCESS == eStatus)
  {
    /* Write DPCD Data */
    sWritePacket.eTransactionType    = DP_TRANSACTION_MODE_AUX;
    sWritePacket.puWriteData         = &uWriteBuffer;
    sWritePacket.uAddress            = DPCD_TRAINING_PATTERN_SET_START_ADDRESS;
    sWritePacket.uWriteDataSize      = DPCD_TRAINING_PATTERN_SET_LEN;
    sWritePacket.uBytesWritten       = 0;
    sWritePacket.bQueueOnly          = FALSE;
    
    eStatus = DP_Host_ClientWrite(eDeviceId, &sWritePacket);

    if (DP_STATUS_SUCCESS != eStatus)
    {
      DP_LOG_MESSAGE_ERROR("DP_WriteSinkDPCDTrainingPattern() : Write Failed, eStatus=0x%x.\n", eStatus);
    }
  }

  return eStatus;
}

/**********************************************************************************************************************

FUNCTION: DP_WriteSinkDPCDLevels()
   
   This function will select the training Voltage Swing Level and Pre-emphasis Level.

Parameters:
   eDeviceId                        -[in ] DP device id
   uDesiredVoltageSwingLevel        -[in ] The desired Voltage Swing level for link training
   uDesiredPreEmphasisLevel         -[in ] The desired pre-emphasis level for link training

Return: 
   DP_Status 

**********************************************************************************************************************/
DP_Status  DP_WriteSinkDPCDLevels(DP_DeviceIDType     eDeviceId,
                                  uint32              uDesiredVoltageSwingLevel,
                                  uint32              uDesiredPreEmphasisLevel)
{
  DP_Status                eStatus     = DP_STATUS_SUCCESS;  
  DP_DeviceCtxType        *pDeviceCtx  = DPHOST_GET_CTX(eDeviceId); 
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
  sWritePacket.eTransactionType    = DP_TRANSACTION_MODE_AUX;
  sWritePacket.puWriteData         = uWriteBuffer;
  sWritePacket.uAddress            = DPCD_TRAINING_LANE_SET_START_ADDRESS;
  sWritePacket.uWriteDataSize      = DPCD_TRAINING_LANE_SET_LEN;
  sWritePacket.uBytesWritten       = 0;
  sWritePacket.bQueueOnly          = FALSE;
  
  eStatus = DP_Host_ClientWrite(eDeviceId, &sWritePacket);

  if (DP_STATUS_SUCCESS!= eStatus)
  {
    DP_LOG_MESSAGE_ERROR("DP_WriteSinkDPCDLevels() : Write Failed, eStatus=0x%x.\n", eStatus);
  }

  return eStatus;
}

/**********************************************************************************************************************

FUNCTION: DP_WriteSinkDPCDDownSpreadEnabled()
   
   This function will indicate that downspread has been enabled on host side.

Parameters:
   eDeviceId                        -[in ] DP device id

Return: 
   DP_Status 

**********************************************************************************************************************/
DP_Status DP_WriteSinkDPCDDownSpreadEnabled(DP_DeviceIDType           eDeviceId)
{
  DP_Status                eStatus      = DP_STATUS_SUCCESS;  
  uint8                    uWriteBuffer = 0x10;    // Bit 4: SPREAD_AMP
  DP_WritePacketType       sWritePacket;

  sWritePacket.eTransactionType    = DP_TRANSACTION_MODE_AUX;
  sWritePacket.puWriteData         = &uWriteBuffer;
  sWritePacket.uAddress            = DPCD_LINK_DOWNSPREAD_CTRL_ADDRESS;
  sWritePacket.uWriteDataSize      = DPCD_LINK_DOWNSPREAD_CTRL_LEN;
  sWritePacket.uBytesWritten       = 0;
  sWritePacket.bQueueOnly          = FALSE;
  
  eStatus = DP_Host_ClientWrite(eDeviceId, &sWritePacket);

  if (DP_STATUS_SUCCESS != eStatus)
  {
    DP_LOG_MESSAGE_ERROR("DP_WriteSinkDPCDDownSpreadEnabled() : Write Failed, eStatus=0x%x.\n", eStatus);
  }

  return eStatus;
}



/**********************************************************************************************************************

FUNCTION: DP_WriteSinkDPCDPower()
   
   This function will set the sink power state. i.e Power on/off for main link or AUX

Parameters:
   eDeviceId                        -[in ] DP device id
   ePowerState                      -[in ] Sink power state to be set

Return: 
   DP_Status 

**********************************************************************************************************************/
DP_Status DP_WriteSinkDPCDPower(DP_DeviceIDType           eDeviceId,
                                DP_DPCDPowerStateType     ePowerState)
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
      DP_LOG_MESSAGE_ERROR("DP_WriteSinkDPCDPower() : Invalid input parameter \n");
      break;
    }
  }

  if (DP_STATUS_SUCCESS == eStatus)
  {
    /*Write DPCD Data*/
    sWritePacket.eTransactionType    = DP_TRANSACTION_MODE_AUX;
    sWritePacket.puWriteData         = &uWriteBuffer;
    sWritePacket.uAddress            = DPCD_SET_POWER_STATE_START_ADDRESS;
    sWritePacket.uWriteDataSize      = DPCD_SET_POWER_STATE_LEN;
    sWritePacket.uBytesWritten       = 0;
    sWritePacket.bQueueOnly          = FALSE;
    
    eStatus = DP_Host_ClientWrite(eDeviceId, &sWritePacket);

    if (DP_STATUS_SUCCESS != eStatus)
    {
      DP_LOG_MESSAGE_ERROR("DP_WriteSinkDPCDPower() : Write Failed, eStatus=0x%x.\n", eStatus);
    }
        
  }

  return eStatus;
}


#ifdef __cplusplus
}
#endif

