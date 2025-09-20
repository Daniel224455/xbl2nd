
#ifndef _DP_DSC_H_
#define _DP_DSC_H_
/*! \file */
/*=====================================================================================================
 
  File: dp_dsc.h
 
  Header file for eDP PSR definitions.
   
  Copyright (c) 2018 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
====================================================================================================*/

/*********************************************************************************************
*
** FUNCTION: DP_Host_GetDSCInfo(DP_DeviceCtxType *pDeviceCtx)
*/
/*!
* \DESCRIPTION
*      get DSC Slice info (height, width and slice per line).
*
* \param [in]   pDeviceCtx        - DP Device
*
* \retval DP_Status
*
**********************************************************************************************/
void DP_Host_GetDSCSliceInfo(DP_DeviceCtxType *pDeviceCtx);

/*********************************************************************************************
*
** FUNCTION: DP_Host_DSC_Commit()
*/
/*!
* \DESCRIPTION
*      comit host side DSC and send PPS at next vsync
*
* \param [in]   pDeviceCtx        - Pointer to device context
*
* \retval DP_Status
*
**********************************************************************************************/
DP_Status  DP_Host_DSC_Commit(DP_DeviceCtxType  *pDeviceCtx);

/*********************************************************************************************
*
** FUNCTION: DP_Host_PPS_Flush()
*/
/*!
* \DESCRIPTION
*      Flush PPS Peripheral and Interface  so that PPS will be Sent at  next vsync
*
* \param [in]  eControllerId - DP core ID
*
* \retval void
*
**********************************************************************************************/
void  DP_Host_DSC_PPS_Flush(DP_DeviceCtxType  *pDeviceCtx);

/*********************************************************************************************
*
** FUNCTION: DP_Host_Sink_FEC_Enable()
*/
/*!
* \DESCRIPTION
*      enable sinker's FEC also setup host compression mode and DTO
*
* \param [in]   pDeviceCtx        - Pointer to device context
*
* \retval DP_Status
*
**********************************************************************************************/
void  DP_Host_Sink_FEC_Enable(DP_DeviceCtxType  *pDeviceCtx);

/*********************************************************************************************
*
** FUNCTION: DP_Host_DSC_Calc_ECC()
*/
/*!
* \DESCRIPTION
*      Calculate SDP parity byte out of data word
*
* \param [in]   pDeviceCtx        - Pointer to device context
* \param [in]   numByte           - number of byte
*
* \retval DP_Status
*
**********************************************************************************************/
uint8 DP_Host_DSC_Calc_ECC(uint32 uData, uint8 numByte);

#endif //_DP_DSC_H_
