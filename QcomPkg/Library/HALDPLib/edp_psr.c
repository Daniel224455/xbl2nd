
/*=====================================================================================================================
 
  File: edp_psr.c
 
  Source file for eDP PSR related functionality 
   
  Copyright (c) 2018 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
=======================================================================================================================*/


/*---------------------------------------------------------------------------------------------------------------------
 * Include Files
 *-------------------------------------------------------------------------------------------------------------------*/
#include "dp_i.h"
#include "edp_psr.h"
#include "hal_edp_psr.h"

#ifdef __cplusplus
extern "C" {
#endif


/* HW PSR state */
#define HW_PSR_ST_INACTIVE      0x01
#define HW_PSR_ST_ACTIVE        0x10

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
DP_Status eDP_PSR_Enter(DP_DeviceCtxType *pDeviceCtx)
{
   DP_Status         eStatus  = DP_STATUS_SUCCESS;
   uint32 psr_state;
   DP_SinkPSRCfgType *pPsrCfg = &pDeviceCtx->sPSR.sSinkCfg;

   psr_state = HAL_eDP_Get_PSR_State(pDeviceCtx->eControllerID);
   if (psr_state == HW_PSR_ST_INACTIVE)
   {
      /* make sure sink psr function is disabled */
      DP_OSAL_MemSet(pPsrCfg, 0, sizeof(*pPsrCfg));
      DP_WriteSinkPSRCOnfiguration(pDeviceCtx); 

      /* config sink psr */
      pPsrCfg->uMainLinkState     = PSR_MAINLINK_SHUTDOWN;  /* mainlink can be turn off */

      pPsrCfg->uCRCVerified       = PSR_CRC_VERIFIED_BY_SOURCE; /* crc verification by host  */
      pPsrCfg->uFrameCapture      = PSR_FRAME_CAPTURE_AT_N1;
      pPsrCfg->uSUScanLineCapture = PSR_SU_SCANLINE_CAPTURE_AT_FIRST_LINE;
      pPsrCfg->uHPDLIinkCRCError  = PSR_NO_HPD_WITH_LINK_CRC_ERROR;
      pPsrCfg->uPSREnableProtocol = PSR_PROTOCOL_PSR;

      /* config sink psr function */
      DP_WriteSinkPSRCOnfiguration(pDeviceCtx);


      /* enable sink psr function */
      pPsrCfg->uEnablePsrFunction = PSR_ENABLE_SINK_PSR;
      DP_WriteSinkPSRCOnfiguration(pDeviceCtx);

      HAL_eDP_PSR_Cmd(pDeviceCtx->eControllerID, PSR_CMD_ENTER);   /* enter psr */

      HAL_eDP_PSR_Enter_Done(pDeviceCtx->eControllerID);   /* wait for RFB update done */

#ifdef PSR_DBEUG
      DP_ReadSinkPSRStatus(pDeviceCtx); /* read sink psr status */
#endif
    }
    else
    {
      eStatus  = DP_STATUS_FAIL;
    }
    return eStatus;
}

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
DP_Status eDP_PSR_Exit(DP_DeviceCtxType *pDeviceCtx)
{
   uint32 psr_state;
   DP_SinkPSRCfgType *pPsrCfg = &pDeviceCtx->sPSR.sSinkCfg;
   DP_Status         eStatus  = DP_STATUS_SUCCESS;

   psr_state = HAL_eDP_Get_PSR_State(pDeviceCtx->eControllerID);
   if (psr_state == HW_PSR_ST_ACTIVE)
   {
      HAL_eDP_PSR_Cmd(pDeviceCtx->eControllerID, PSR_CMD_EXIT);   /* exit psr */

      HAL_eDP_PSR_Exit_Done(pDeviceCtx->eControllerID);

      /* disable sink psr function */
      pPsrCfg->uEnablePsrFunction = PSR_DISABLE_SINK_PSR;
      DP_WriteSinkPSRCOnfiguration(pDeviceCtx);

#ifdef PSR_DBEUG
      DP_ReadSinkPSRStatus(pDeviceCtx); /* read sink psr status */
#endif
   }
   else
   {
      eStatus  = DP_STATUS_FAIL;
   }

   return eStatus;
}


#ifdef __cplusplus
}
#endif
