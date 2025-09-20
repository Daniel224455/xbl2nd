/*=====================================================================================================================
 
  File: hal_dp_dsc.h
 
  Source file for DP DSC related functionality 
   
  Copyright (c) 2018 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
=======================================================================================================================*/


/*---------------------------------------------------------------------------------------------------------------------
 * Include Files
 *-------------------------------------------------------------------------------------------------------------------*/
#include "dp_i.h"
#include "hal_dp_i.h"
#include "hal_mdp.h"

#ifdef __cplusplus
extern "C" {
#endif

/*********************************************************************************************
*
** FUNCTION: HAL_DP_Host_DSC_CompressionMode()
*/
/*!
* \DESCRIPTION
*      Calculate and configure DP DSC compression mode register
*
*   pDeviceCtx                       -[in ] Pointer to device context
*
* \retval DP_Status
*
**********************************************************************************************/
void HAL_DP_Host_DSC_CompressionMode(DP_DeviceCtxType *pDeviceCtx);

/*********************************************************************************************
*
** FUNCTION: HAL_DP_Host_DSC_DTO()
*/
/*!
* \DESCRIPTION
*      Calculate and configure DP DSC dto mode register
*
*   pDeviceCtx                       -[in ] Pointer to device context
*
* \retval DP_Status
*
**********************************************************************************************/
void HAL_DP_Host_DSC_DTO(DP_DeviceCtxType *pDeviceCtx);

/*********************************************************************************************
*
** FUNCTION: HAL_DP_Host_FEC_Enable()
*/
/*!
* \DESCRIPTION
*      Enable host FEC function
*
*   pDeviceCtx                       -[in ] Pointer to device context
*
* \retval DP_Status
*
**********************************************************************************************/
void HAL_DP_Host_FEC_Enable(DP_DeviceCtxType *pDeviceCtx);

/*********************************************************************************************
*
** FUNCTION: HAL_DP_Host_DSC_PPS()
*/
/*!
* \DESCRIPTION
*      Commit PPS from buffer to DP controller registers
*
*   pDeviceCtx                       -[in ] Pointer to device context
*
* \retval DP_Status
*
**********************************************************************************************/
DP_Status  HAL_DP_Host_DSC_PPS(DP_DeviceCtxType  *pDeviceCtx);

/*********************************************************************************************
*
** FUNCTION: HAL_DP_Host_PPS_Flush()
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
void  HAL_DP_Host_DSC_PPS_Flush(DP_DeviceCtxType  *pDeviceCtx);

#ifdef __cplusplus
}
#endif
