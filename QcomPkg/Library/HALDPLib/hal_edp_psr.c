/*=============================================================================
 
  File: hal_edp_psr.c
 
  Source file for eDP PSR hardware functionality 
   
  Copyright (c) 2018 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
=============================================================================*/


/*------------------------------------------------------------------------------
 * Include Files
 *----------------------------------------------------------------------------*/
#include "hal_dp_i.h"
#include "dp_i.h"
#include "edp_psr.h"
#include "hal_edp_psr.h"
#include "hal_mdp.h"

#ifdef __cplusplus
extern "C" {
#endif

/*********************************************************************************************
*
** FUNCTION: HAL_eDP_Get_Psr_State()
*/
/*!
* \DESCRIPTION
*      get PSR stateg from hw register
*
* \param [in]  eControllerId - DP core ID
*
* \retval uint32
*
**********************************************************************************************/
uint32 HAL_eDP_Get_PSR_State(DP_ControllerIDType       eControllerId)
{
  uintPtr uOffset  = 0;  /* no offset required for edp */
  volatile uint32 uRegValue;

  uRegValue = in_dword(uOffset + HWIO_EDP_PSR_INFO_ADDR);         /* 0x0AE9A55C */

  uRegValue &= 0x0ff;   /* PSR state */
  return uRegValue;
}

/*********************************************************************************************
*
** FUNCTION: HAL_eDP_PSR_Send_VSC_Flush()
*/
/*!
* \DESCRIPTION
*      Flush SEND_VSC to be oick up at  next vsync
*
* \param [in]  eControllerId - DP core ID
*
* \retval void
*
**********************************************************************************************/
void HAL_eDP_PSR_Send_VSC_Flush(DP_ControllerIDType eControllerId)
{
  uintPtr uOffset  = 0;  /* no offset required for edp */

  /* flush */
  out_dword(uOffset + HWIO_MDP_CTL_0_PERIPH_FLUSH_ADDR, HWIO_MDP_CTL_0_PERIPH_FLUSH_INTF_5_BMSK);
  out_dword(uOffset + HWIO_MDP_CTL_0_FLUSH_ADDR, HWIO_MDP_CTL_0_FLUSH_PERIPH_BMSK);
}

/*********************************************************************************************
*
** FUNCTION: HAL_eDP_PSR_Config()
*/
/*!
* \DESCRIPTION
*      Config hw PSR 
*
* \param [in]  eControllerId - DP core ID
*
* \retval void
*
**********************************************************************************************/
void HAL_eDP_PSR_Config(DP_ControllerIDType       eControllerId)
{
  volatile uint32 uRegValue;
  uintPtr uOffset  = 0;  /* no offset required for edp */

  /* Disable PSR related interrupt generated -- polling */
  out_dword(uOffset + HWIO_EDP_INTERRUPT_MASK_4_ADDR, 0);    /* 0x0AE9A030 */

  uRegValue = in_dword(uOffset + HWIO_EDP_MAINLINK_LEVELS_2_ADDR);  /* 0x0AE9A448 */
  uRegValue |= HWIO_EDP_MAINLINK_LEVELS_2_MSA_TO_VSC_DELAY_MODE_BMSK;
  out_dword(uOffset + HWIO_EDP_MAINLINK_LEVELS_2_ADDR, uRegValue);

  uRegValue = in_dword(uOffset + HWIO_EDP_SDP_CFG2_ADDR);           /* 0x0AE9A62C */
  uRegValue &= ~HWIO_EDP_SDP_CFG2_VSC_SDPFSM_SRC_BMSK;
  uRegValue |= HWIO_EDP_SDP_CFG2_PSR_VSC_SDPSIZE_BMSK;              /* 32 bytes */
  out_dword(uOffset + HWIO_EDP_SDP_CFG2_ADDR, uRegValue);

  uRegValue = in_dword(uOffset + HWIO_EDP_PSR_CONFIG_ADDR);         /* 0x0AE9A500 */
  uRegValue |= HWIO_EDP_PSR_CONFIG_PSR_INDIRECT_UPDATE_BMSK;        /* Updata comamnd from mdp */
  uRegValue |= HWIO_EDP_PSR_CONFIG_FRAME_CAPTURE_INDICATION_BMSK;  /* frame captured at first frame */
  uRegValue |= HWIO_EDP_PSR_CONFIG_SEND_CRC_BMSK;                   /* send crc, if verification done by sink */

  uRegValue &= ~HWIO_EDP_PSR_CONFIG_POWER_STATE_BMSK;               /* mainlink can power off */

  uRegValue |= 0x01;                                                /* PSR */
  out_dword(uOffset + HWIO_EDP_PSR_CONFIG_ADDR, uRegValue);

  uRegValue = 0;
  out_dword(uOffset + HWIO_EDP_PSR_CMD_ADDR, uRegValue);         /* 0x0AE9A510 */
}

/*********************************************************************************************
*
** FUNCTION: HAL_eDP_PSR_Cmd()
*/
/*!
* \DESCRIPTION
*      issue PSR cmd to start hw state machine
*
* \param [in]  eControllerId - DP core ID
* \param [in]  psrCmd        - PSR cmd
*
* \retval void
*
**********************************************************************************************/
void  HAL_eDP_PSR_Cmd(DP_ControllerIDType       eControllerId,
                                   PSR_CmdType               psrCmd)
{
   uintPtr uOffset  = 0;  /* no offset required for edp */

   if (psrCmd < PSR_CMD_MAX)
   {
      out_dword(uOffset + HWIO_EDP_PSR_CMD_ADDR, BIT(psrCmd));
   }
}

/*********************************************************************************************
*
** FUNCTION: HAL_eDP_PSR_Enter_Done()
*/
/*!
* \DESCRIPTION
*      polling ISR ststus to make sure PSR enter cmd is done
*
* \param [in]  eControllerId - DP core ID
*
* \retval HAL_MDSS_ErrorType
*
**********************************************************************************************/
HAL_MDSS_ErrorType HAL_eDP_PSR_Enter_Done(DP_ControllerIDType eControllerId)
{
  uintPtr uOffset  = 0;  /* no offset required for edp */
  volatile uint32 uRegValue;
  uint32 uMask;
  int32  cnt = 20;
  HAL_MDSS_ErrorType eStatus = HAL_MDSS_DP_STATUS_FAILED;


  uMask = HWIO_EDP_INTERRUPT_STATUS_4_PSR_UPDATE_INT_BMSK | HWIO_EDP_INTERRUPT_STATUS_4_PSR_CAPTURE_INT_BMSK;
  while (cnt-- > 0)
  {
      DP_OSAL_SleepMs(10);    /* ms */
      uRegValue = in_dword(uOffset + HWIO_EDP_INTERRUPT_STATUS_4_ADDR);    /* 0x0AE9A02c */
      uRegValue &= uMask;
      if (uRegValue == uMask)
      {
         eStatus = HAL_MDSS_STATUS_SUCCESS;
         break;
      }
  }
  uMask <<= 1;
  out_dword(uOffset + HWIO_EDP_INTERRUPT_STATUS_4_ADDR, uMask);    /* clear status bits */

  return eStatus;
}

/*********************************************************************************************
*
** FUNCTION: HAL_eDP_PSR_Exit_Done()
*/
/*!
* \DESCRIPTION
*      polling ISR ststus to make sure PSR Exit cmd is done
*
* \param [in]  eControllerId - DP core ID
*
* \retval HAL_MDSS_ErrorType
*
**********************************************************************************************/
HAL_MDSS_ErrorType HAL_eDP_PSR_Exit_Done(DP_ControllerIDType eControllerId)
{
  uintPtr            uOffset    = 0;  /* no offset required for edp */
  volatile uint32    uRegValue;
  volatile uint32    uMask      = HWIO_EDP_INTERRUPT_STATUS_4_PSR_EXIT_INT_BMSK;
  int32              cnt        = 20;
  HAL_MDSS_ErrorType eStatus    = HAL_MDSS_DP_STATUS_FAILED;

  while (cnt-- > 0)
  {
      DP_OSAL_SleepMs(10);    /* ms */
      uRegValue = in_dword(uOffset + HWIO_EDP_INTERRUPT_STATUS_4_ADDR);    /* 0x0AE9A02c */
      uRegValue &= uMask;
      if (uRegValue == uMask)
      {
         eStatus = HAL_MDSS_STATUS_SUCCESS;
         break;
      }
  }
  uMask <<= 1;
  out_dword(uOffset + HWIO_EDP_INTERRUPT_STATUS_4_ADDR, uMask);    /* clear status bits */

  return eStatus;
}

/*********************************************************************************************
*
** FUNCTION: HAL_eDP_MDP_Ctrl_Intf_Flush()
*/
/*!
* \DESCRIPTION
*      flush pSR frame update
*
* \param [in]  eControllerId - DP core ID
*
* \retval HAL_MDSS_ErrorType
*
**********************************************************************************************/
HAL_MDSS_ErrorType HAL_eDP_MDP_Ctrl_Intf_Flush(DP_ControllerIDType       eControllerId)
{
  uintPtr         uOffset  = 0;  /* no offset required for edp */
  volatile uint32 uRegValue;

  uRegValue = HWIO_MDP_INTF_5_EDP_PSR_UPDATE_CTRL_RMSK;
  out_dword(uOffset + HWIO_MDP_INTF_5_EDP_PSR_UPDATE_CTRL_ADDR, uRegValue);

  /* CTL-0 */
  uRegValue = HWIO_MDP_CTL_0_INTF_FLUSH_INTF_5_BMSK;
  out_dword(uOffset + HWIO_MDP_CTL_0_INTF_FLUSH_ADDR, uRegValue);

  uRegValue = HWIO_MDP_CTL_0_FLUSH_INTF_BMSK;
  out_dword(uOffset + HWIO_MDP_CTL_0_FLUSH_ADDR, uRegValue);

  return HAL_MDSS_STATUS_SUCCESS;
}

/*********************************************************************************************
*
** FUNCTION: HAL_eDP_PSR_CRC()
*/
/*!
* \DESCRIPTION
*      read source PSR frame CRC
*
* \param [in]  eControllerId - DP core ID
*
* \retval HAL_MDSS_ErrorType
*
**********************************************************************************************/
HAL_MDSS_ErrorType HAL_eDP_PSR_CRC(DP_ControllerIDType       eControllerId)
{
  uintPtr uOffset  = 0;  /* no offset required for edp */
  volatile uint32 uRegValue;
  uint32 data;

  uRegValue = in_dword(uOffset + HWIO_EDP_PSR_CRC_RG_ADDR); 
  data      = in_dword(uOffset + HWIO_EDP_PSR_CRC_B_ADDR); 
  DP_LOG_MESSAGE_ERROR("HAL_edp_psr_crc: CRC_RG=%x CRC_B=%x\n", uRegValue, data);

  return HAL_MDSS_STATUS_SUCCESS;
}

#ifdef __cplusplus
}
#endif
