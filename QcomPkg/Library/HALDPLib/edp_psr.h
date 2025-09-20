
#ifndef _eDP_PSR_H_
#define _eDP_PSR_H_
/*! \file */
/*=====================================================================================================
 
  File: edp_psr.h
 
  Header file for eDP PSR definitions.
   
  Copyright (c) 2018 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
====================================================================================================*/

#define BIT(x)  (1 << x)

/*!
 * \e PSR_StateMachineType
 *
 * eDP PSR State
 *
 */
typedef enum
{
   PSR_ST_DISABLED = 0,
   PSR_ST_INACTIVE,
   PSR_ST_ACTIVE,
   PSR_ST_ACTIVE_RFB_CAPTURED,
   PSR_ST_ACTIVE_RFB_UPDATE,
   PSR_ST_EXIT,
   PSR_ST_MAX
} PSR_StateMachineType;

/*!
 * \e PSR_EventType
 *
 * eDP PSR Event
 *
 */
typedef enum
{
   PSR_EV_ENABLE = 0,
   PSR_EV_CHOOSE_ENTER_ACTIVE,
   PSR_EV_FB_TXED_TURN_OFF_HOLD_PERIOD,
   PSR_EV_CHOOSE_RFB_UPDATE,
   PSR_EV_RFB_UPDATE,
   PSR_EV_CHOOSE_EXIT_ACTIVE,
   PSR_EV_INACTIVE_SINK_TXED,
   PSR_EV_ABORT_PSR_ENTRY,
   PSR_EV_ENTER_DEEP_SLEEP,
   PSR_EV_MAX,
} PSR_EventType;


/*!
 * \e PSR_EnableSinkPSR
 *
 * eDP PSR Enable Sink PSR
 *
 */
typedef enum
{
   PSR_DISABLE_SINK_PSR = 0,
   PSR_ENABLE_SINK_PSR
} PSR_EnableSinkPSR;


/*!
 * \e PSR_MainlInkStateType
 *
 * eDP PSR Main Link State
 *
 */
typedef enum
{
   PSR_MAINLINK_SHUTDOWN = 0,
   PSR_MAINLINK_REMAIN_ACTIVE
} PSR_MainLinkStateType;

/*!
 * \e PSR_CRCVerifiedType
 *
 * eDP PSR CRC verified
 *
 */
typedef enum
{
   PSR_CRC_VERIFIED_BY_SOURCE = 0,
   PSR_CRC_VERIFIED_BY_SINK
} PSR_CRCVerifiedType;

/*!
 * \e PSR_FrameCaptureType
 *
 * eDP PSR Frame Capture
 *
 */
typedef enum
{
   PSR_FRAME_CAPTURE_AT_N = 0,
   PSR_FRAME_CAPTURE_AT_N1
} PSR_FrameCaptureType;

/*!
 * \e PSR_SUScanLinkCaptureType
 *
 * eDP PSR SU Scan Link Capture
 *
 */
typedef enum
{
   PSR_SU_SCANLINE_CAPTURE_AT_FIRST_LINE = 0,
   PSR_SU_SCANLINE_CAPTURE_AT_SECOND_LINE
} PSR_SUScanLinkCaptureType;

/*!
 * \e PSR_HPDLinkErrorType
 *
 * eDP PSR HDP Link error indication
 *
 */
typedef enum
{
   PSR_NO_HPD_WITH_LINK_CRC_ERROR = 0,
   PSR_HPD_WITH_LINK_CRC_ERROR
} PSR_HPDLinkErrorType;

/*!
 * \e PSR_ProtocolType
 *
 * eDP PSR Protocol
 *
 */
typedef enum
{
   PSR_PROTOCOL_PSR = 0,
   PSR_PROTOCOL_PSR2
} PSR_ProtocolType;

/*!
 * \e PSR_CMdType
 *
 * eDP PSR HW cmd
 *
 */
typedef enum
{
   PSR_CMD_ENTER = 0,
   PSR_CMD_EXIT,
   PSR_CMD_UPDATE,
   PSR_CMD_BURST_UPDATE,
   PSR_CMD_MAX
} PSR_CmdType;


/*********************************************************************************************
*
** FUNCTION: eDP_PSR_Enter()
*/
/*!
* \DESCRIPTION
*      Start eDP PSR
*
*   pDeviceCtx                       -[in ] Pointer to device context
*
* \retval DP_Status
*
**********************************************************************************************/
DP_Status eDP_PSR_Enter(DP_DeviceCtxType *pDeviceCtx);


/*********************************************************************************************
*
** FUNCTION: eDP_PSR_Exit()
*/
/*!
* \DESCRIPTION
*      Exit eDP PSR
*
*   pDeviceCtx                       -[in ] Pointer to device context
*
* \retval DP_Status
*
**********************************************************************************************/
DP_Status eDP_PSR_Exit(DP_DeviceCtxType *pDeviceCtx);

#endif //_eDP_PSR_H_
