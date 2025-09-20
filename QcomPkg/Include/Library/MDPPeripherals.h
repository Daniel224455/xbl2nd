#ifndef MDPPERIPHERALS_H
#define MDPPERIPHERALS_H
/*=============================================================================
 
  File: MDPPeripherals.h
 
  Header file for common MDP peripheral functions such as I2C
  
 
  Copyright (c) 2011-2017 Qualcomm Technologies, Inc.  
  All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential.
=============================================================================*/
/*============================================================================
*                         INCLUDE FILES
============================================================================*/
#include "../../Library/MDPLib/MDPLib_i.h"


/* -----------------------------------------------------------------------
** Macros and Defines
-------------------------------------------------------------------------- */


/* -----------------------------------------------------------------------
** I2C Driver Function Prototypes
-------------------------------------------------------------------------- */

//-------------------------------------------------------------------------------------------------
//  I2CDriver_Init
//
//  @brief
//      Initialize the I2C handles to write to I2C bus.
//
//  @params
//      [IN] pI2CConfig
//          I2C Information.
//
//  @return
//      MDP_STATUS_OK if I2C is initialized successfully.
//-------------------------------------------------------------------------------------------------
//
MDP_Status I2CDriver_Init(MDP_I2C_Configuration *pI2CConfig);

//-------------------------------------------------------------------------------------------------
//  I2CDriver_Deinit
//
//  @brief
//      Deinitialize the I2C Core.
//
//  @params
//      [IN] pI2CConfig
//          I2C information.
//
//  @return
//      VOID.
//-------------------------------------------------------------------------------------------------
//
void I2CDriver_Deinit(MDP_I2C_Configuration *pI2CConfig);

//-------------------------------------------------------------------------------------------------
//  I2CPanelSendCommandSequence
//
//  @brief
//      Send I2C command sequence.
//
//  @params
//      [IN] pI2CConfig
//          I2C information.
//      [IN] eCommand
//          Init/Term commands.
//      [IN] pCustomCommand
//          Custom command.
//      [IN] uFlags
//          Reserved.
//
//  @return
//      MDP_STATUS_OK if I2C command sequence is successfully sent.
//-------------------------------------------------------------------------------------------------
//
MDP_Status I2CPanelSendCommandSequence(MDP_I2C_Configuration *pI2CConfig, 
                                       MdpPanelCommandType    eCommand, 
                                       void                  *pCustomCommand,
                                       uint32                 uFlags);


#endif  /* #define MDPPERIPHERALS_H */

