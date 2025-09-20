
#ifndef _hal_edp_psr_h_
#define _hal_edp_psr_h_
/*! \file */
/*=====================================================================================================
 
  File: hal_edp_psr.h
 
  Header file for eDP PSR definitions.
   
  Copyright (c) 2018 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
====================================================================================================*/


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
void HAL_eDP_PSR_Config(DP_ControllerIDType       eControllerId);

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
uint32 HAL_eDP_Get_PSR_State(DP_ControllerIDType       eControllerId);

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
void HAL_eDP_PSR_Cmd(DP_ControllerIDType eControllerId, PSR_CmdType psrCmd);

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
HAL_MDSS_ErrorType HAL_eDP_PSR_Enter_Done(DP_ControllerIDType       eControllerId);

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
HAL_MDSS_ErrorType HAL_eDP_PSR_Exit_Done(DP_ControllerIDType eControllerId);

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
HAL_MDSS_ErrorType HAL_eDP_MDP_Ctrl_Intf_Flush(DP_ControllerIDType       eControllerId);


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
void HAL_eDP_PSR_Send_VSC_Flush(DP_ControllerIDType eControllerId);

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
HAL_MDSS_ErrorType HAL_eDP_PSR_CRC(DP_ControllerIDType       eControllerId);
#endif  /* _hal_edp_psr_h_ */
