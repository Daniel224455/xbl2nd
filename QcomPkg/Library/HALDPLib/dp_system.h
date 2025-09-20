#ifndef _DP_SYSTEM_H_
#define _DP_SYSTEM_H_
/*! \file */
/*=====================================================================================================
 
  File: dp_system.c
 
  Header file for DP system level utility functions 
   
  Copyright (c) 2016-2018 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
====================================================================================================*/
#include "MDPSystem.h"
#include "dp_host.h"
#include "dp_types.h"

/* --------------------------------------------------------------------------------------------------------------------
  Defines
---------------------------------------------------------------------------------------------------------------------*/


/* Convenience macro to print file name and line number in debug messages
*/

#define DP_LOG_MESSAGE_ERROR(fmt, ...)     MDP_Log_Message(MDP_LOGLEVEL_ERROR, fmt, ##__VA_ARGS__)
#define DP_LOG_MESSAGE_WARN(fmt, ...)      MDP_Log_Message(MDP_LOGLEVEL_WARN, fmt, ##__VA_ARGS__)
#define DP_LOG_MESSAGE_INFO(fmt, ...)      MDP_Log_Message(MDP_LOGLEVEL_INFO, fmt, ##__VA_ARGS__)
#define HAL_DP_SLEEP_US(_DelayUs_)         DP_OSAL_SleepUs(_DelayUs_)
#define HAL_DP_SLEEP_MS(_Delayms_)         DP_OSAL_SleepMs(_Delayms_)

/*---------------------------------------------------------------------------------------------------------------------
  Flags
---------------------------------------------------------------------------------------------------------------------*/
/* 
 * DP_MainClockConfig Flags
 */

/* Configure the source clock in conjunction with the dividers */
#define DP_MAINCLOCKCONFIG_SETSOURCE                   0x00000001

/*---------------------------------------------------------------------------------------------------------------------
  Functions
---------------------------------------------------------------------------------------------------------------------*/
 
/**********************************************************************************************************************

FUNCTION: DP_AUXClockEnable()
   
  This function will set up the DP aux link clocks. This must be called after MND have been calculated

Parameters:
  hHandle      -[in]    : Handle to DP device
  bEnable      -[in]    :  Enable/Disable Clock

Return: 
  DP_Status

**********************************************************************************************************************/
DP_Status DP_AUXClockEnable(DP_HandleType    hHandle,
                            bool32           bEnable);

/**********************************************************************************************************************

FUNCTION: DP_MainClockEnable()
   
  This function will set up the DP main link clocks. This must be called after MND have been calculated

Parameters:
  hHandle      -[in]    : Handle to DP device
  pPixClkDivs  -[in]    : Pointer to Pixel clock divider info.
  bEnable      -[in]    : Enable/Disable Clock

Return: 
  DP_Status

**********************************************************************************************************************/
DP_Status DP_MainClockEnable(DP_HandleType        hHandle, 
                             DP_PixelClk_Info    *pPixClkDivs,
                             bool32               bEnable);

/**********************************************************************************************************************

FUNCTION: DP_WaitForReadyForVideo()
   
  This function will Wait for DP control status to be ready for video 

Parameters:
  hHandle      -[in]    : Handle to DP device

Return: 
  DP_Status

**********************************************************************************************************************/
DP_Status DP_WaitForReadyForVideo(DP_HandleType        hHandle);

/**********************************************************************************************************************

FUNCTION: DP_GetHPDPollTime()
   
  This function will get the user requested HPD polling time in ms

Parameters:

Return: 
  DP_Status

**********************************************************************************************************************/
uint32 DP_GetHPDPollTime();



/**********************************************************************************************************************

FUNCTION: DP_PhyDataPathIsReverse()
   
  This function will check if the DP PHY data path is reverse of USB type-C specification

Parameters:
  hHandle      -[in]    : Handle to DP device

Return: 
  bool32      TRUE - reverse of spec      FALSE - as per spec 
  
**********************************************************************************************************************/
bool32 DP_PhyDataPathIsReverse(DP_HandleType        hHandle);

/**********************************************************************************************************************

FUNCTION: DP_SetAuxPolarityGPIO()

  This function will set GPIO to select the AUX channel polarity

Parameters:
  hHandle      -[in]    : Handle to DP device
  bReverted    -[in]    : Set GPIO level to select AUX polarity

Return:
  DP_Status

**********************************************************************************************************************/
DP_Status DP_SetAuxPolarityGPIO(DP_HandleType      hHandle,
                                bool32             bReverted);

/**********************************************************************************************************************

FUNCTION: DP_CheckConnectionStatus()

  This function will check cable connection status

Parameters:
  hHandle    -[in]    : Handle to DP device

Return:
  DP_Status

**********************************************************************************************************************/
DP_Status DP_CheckConnectionStatus(DP_HandleType        hHandle);


/**********************************************************************************************************************
*
** FUNCTION: DP_OSAL_MemSet()
*/
/*!
* \brief
*   The \b MDSS_OSAL_MemSet function fills the buffer with the uFillValue
*
* \param [in]   pBuffer           - Input buffer pointer
* \param [in]   uFillValue        - Value to be filled into the buffer
* \param [in]   uSizeInBytes      - Buffer size to be filled
*
* \return void
*
**********************************************************************************************************************/
void DP_OSAL_MemSet(void *pBuffer, uint8 uFillValue, uint32 uSizeInBytes);

/**********************************************************************************************************************
*
** FUNCTION: DP_OSAL_MemCpy()
*/
/*!
* \brief
*   The \b DP_OSAL_MemCpy function copies bytes from one buffer to another
*
* \param [in]   pDst             - a pointer to where you want the function to copy the data
* \param [in]   pSrc             - a pointer to the buffer that you want to copy data from
* \param [in]   uSizeInBytes     - the number of bytes to copy
*
* \return void
*
**********************************************************************************************************************/
void DP_OSAL_MemCpy(void *pDst, void *pSrc, uint32 uSizeInBytes);

/**********************************************************************************************************************
*
** FUNCTION: DP_OSAL_SleepMs()
*/
/*!
* \brief
*   The \b DP_OSAL_SleepMs function sleeps for a specific period
*
* \param [in]  uMilliSeconds            sleep time in millisecond
*
* \retval void
*
**********************************************************************************************************************/
void DP_OSAL_SleepMs(uint32 uMilliSeconds);

/**********************************************************************************************************************
*
** FUNCTION: DP_OSAL_SleepUs()
*/
/*!
* \brief
*   The \b DP_OSAL_SleepUs function sleeps for a specific period
*
* \param [in]  uMicroSeconds            sleep time in microsecond
*
* \retval void
*
**********************************************************************************************************************/
void DP_OSAL_SleepUs(uint32 uMicroSeconds);

/* Stall processor for amount of time in microseconds
*/
/**********************************************************************************************************************
*
** FUNCTION: DP_OSAL_StallUs()
*/
/*!
* \brief
*   The \b DP_OSAL_StallUs function stall for a specific period
*
* \param [in]  us            stall time in microsecond
*
* \retval void
*
**********************************************************************************************************************/
void DP_OSAL_StallUs(uint32 us);

/**********************************************************************************************************************

FUNCTION: DP_CheckCOnnectionStatus()

   This function detect HPD connection status

Parameters:
   hHandle    -[in]    : Handle to DP device

Return:
   DP_Status

**********************************************************************************************************************/
DP_Status DP_CheckConnectionStatus(DP_HandleType      hHandle);

#endif //_DP_SYSTEM_H_
