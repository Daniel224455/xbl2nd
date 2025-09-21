/*=====================================================================================================
 
  File: dp_system.c
 
  Source file for DP system level utility functions 
   
  Copyright (c) 2016-2018 Qualcomm Technologies, Inc.  All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential.
====================================================================================================*/

#include "MDPSystem.h"
#include "MDPLib_i.h"
#include "dp_i.h"

#ifdef __cplusplus
extern "C" {
#endif


/*------------------------------------------------------------------------------
 * DEFINES
 *----------------------------------------------------------------------------*/

// Wait time for video ready
#define DP_READY_FOR_VIDEO_TIMEOUT_US            5000        // 5 ms
#define DP_READY_FOR_VIDEO_POLL_INTERVAL_US         1        // 1 Us
#define DP_HPD_POLL_TIME_MS                        16        // 16 ms

/*------------------------------------------------------------------------------
 * Functions
 *----------------------------------------------------------------------------*/

/* Set memory
*/
void DP_OSAL_MemSet(void *pBuffer, uint8 uFillValue, uint32 uSizeInBytes)
{
  MDP_OSAL_MEMSET(pBuffer, uFillValue, uSizeInBytes);
}


/* Copy memory
*/
void DP_OSAL_MemCpy(void *pDst, void *pSrc, uint32 uSizeInBytes)
{
  MDP_OSAL_MEMCPY(pDst, pSrc, uSizeInBytes);
}


/* Sleep/halt processor for amount of time in milliseconds
*/
void DP_OSAL_SleepMs(uint32 ms)
{
  MDP_OSAL_DELAYMS(ms);
}

/* Sleep/halt processor for amount of time in microseconds
*/
void DP_OSAL_SleepUs(uint32 us)
{
  MDP_OSAL_DELAYUS(us);
}

/**********************************************************************************************************************

FUNCTION: DP_AUXClockEnable()
   
  This function will set up the DP aux link clocks. This must be called after MND have been calculated

Parameters:
  eDeviceId    -[in]    :  DP device ID
  bEnable      -[in]    :  Enable/Disable Clock

Return: 
  DP_Status

**********************************************************************************************************************/
DP_Status DP_AUXClockEnable(DP_DeviceIDType  eDeviceId,
                            bool32           bEnable)
{
  DP_Status       eStatus        = DP_STATUS_SUCCESS;

  MDPExternalClockEntry sDPClocks[] =
                         {
                           {"disp_cc_mdss_dp_aux_clk",       0, 0, 0, 0, 0, 0},          // cxo
                           {"\0",                            0, 0, 0, 0, 0, 0}
                         };

  // In MDPClocks we don't have a way to turn on AUX clock by itself
  // this will turn on all DP clocks, but that's ok we will set their
  // external sources later by calling this again in DP_MainClockConfig
  if (MDP_STATUS_OK != MDPSetupClocks(MDP_CLOCKTYPE_DP, (MDPExternalClockEntry *)&sDPClocks))
  {
    DP_LOG_MESSAGE_ERROR("DP Driver: DP%d AUX clock source could not be configured.\n", eDeviceId);
    eStatus = DP_STATUS_FAILED_RESOURCE_FAILED;
  }

  return eStatus;
}



/**********************************************************************************************************************

FUNCTION: DP_MainClockEnable()
   
  This function will set up the DP main link clocks. This must be called after MND have been calculated

Parameters:
  eDeviceId    -[in]    :  DP device ID
  pPixClkDivs  -[in]    :  Pointer to Pixel clock divider info.
  bEnable      -[in]    :  Enable/Disable Clock

Return: 
  DP_Status

**********************************************************************************************************************/
DP_Status DP_MainClockEnable(DP_DeviceIDType      eDeviceId,
                             DP_PixelClk_Info    *pPixClkDivs,
                             bool32               bEnable)
{
  DP_Status       eStatus        = DP_STATUS_SUCCESS;

  MDPExternalClockEntry    sDPClocks[] =
                           {
                             {"disp_cc_mdss_dp_pixel_clk",     2, 0, 0, 0, 0, 0},          // dpphy_cc_vco_div_clk
                             {"disp_cc_mdss_dp_link_clk",      1, 0, 0, 0, 0, 0},          // dpphy_cc_link_clk
                             {"disp_cc_mdss_dp_crypto_clk",    1, 0, 0, 0, 0, 0},          // dpphy_cc_link_clk
                             {"disp_cc_mdss_dp_aux_clk",       0, 0, 0, 0, 0, 0},          // cxo
                             {"disp_cc_mdss_dp_link_intf_clk", 1, 0, 0, 0, 0, 0},          // dpphy_cc_link_clk
                             {"disp_cc_mdss_hdmi_dp_ahb_clk",  0, 0, 0, 0, 0, 0},
                             {"\0",                         0, 0, 0, 0, 0, 0}
                           };

  if (bEnable)
  {
     // Pixel clock Dividers
     sDPClocks[0].nClkDiv    = pPixClkDivs->uPixelDivider;    // this is a div2x value
     sDPClocks[0].uClkPLL_M  = pPixClkDivs->uPixelClk_M;
     sDPClocks[0].uClkPLL_N  = pPixClkDivs->uPixelClk_N;
     sDPClocks[0].uClkPLL_2D = pPixClkDivs->uPixelClk_2D;

     if (MDP_STATUS_OK != MDPSetupClocks(MDP_CLOCKTYPE_DP, (MDPExternalClockEntry *)&sDPClocks))
     {
       DP_LOG_MESSAGE_ERROR("DP Driver: DP%d clock source could not be configured.\n", eDeviceId);
       eStatus = DP_STATUS_FAILED_RESOURCE_FAILED;
     }
  }
  else 
  {
     if (MDP_STATUS_OK != MDPDisableClocks(MDP_CLOCKTYPE_DP))
     {
       DP_LOG_MESSAGE_ERROR("DP Driver: DP%d clock source could not be shutdown.\n", eDeviceId);
       eStatus = DP_STATUS_FAILED_RESOURCE_FAILED;
     }

  }

  return eStatus;
}

/**********************************************************************************************************************

FUNCTION: DP_WaitForReadyForVideo()
   
  This function will Wait for DP control status to be ready for video 

Parameters:
  eDeviceId    -[in]    :  DP device ID

Return: 
  DP_Status

**********************************************************************************************************************/
DP_Status DP_WaitForReadyForVideo(DP_DeviceIDType      eDeviceId)
{
  DP_Status                       eStatus        = DP_STATUS_FAILED_TIMEOUT;
  uint32                          i              = 0;  
  uint32                          uLinkStatus    = 0;
  uint32                          uPollCount     = (DP_READY_FOR_VIDEO_TIMEOUT_US / DP_READY_FOR_VIDEO_POLL_INTERVAL_US) + 1;

  for (i = 0; i < uPollCount; ++i)
  {    
    uLinkStatus = HAL_DP_Mainlink_Status();    
    if(uLinkStatus & HAL_DP_LINKSTATUS_TYPE_READY_FOR_VIDEO)
    {
      DP_LOG_MESSAGE_INFO("DP_WaitForReadyForVideo: status=%x\n", uLinkStatus);
      eStatus = DP_STATUS_SUCCESS;
      break;
    }
    DP_OSAL_SleepUs(DP_READY_FOR_VIDEO_POLL_INTERVAL_US);
  }
  
  return eStatus;
}

/**********************************************************************************************************************

FUNCTION: DP_GetHPDPollTime()
   
  This function will get the user requested HPD polling time

Parameters:

Return: 
  uint32      polling time in ms

**********************************************************************************************************************/
uint32 DP_GetHPDPollTime()
{
  return DP_HPD_POLL_TIME_MS;
}

/**********************************************************************************************************************

FUNCTION: DP_PhyDataPathIsReverse()

This function will check if the DP PHY datapath is reverse of USB type-C specification

Parameters:

Return:
bool32      TRUE - reverse of spec      FALSE - as per spec

**********************************************************************************************************************/
bool32 DP_PhyDataPathIsReverse(DP_DeviceIDType      eDeviceId)
{
  bool32               bReverse     = FALSE;
  HAL_HW_VersionType   sDPHwVersion = { 0 };

  HAL_DP_ReadVersionInfo(&sDPHwVersion);

  if ((1 == sDPHwVersion.uMajorVersion) &&
      (0 == sDPHwVersion.uMinorVersion))
  {
    bReverse = TRUE;
  }

  return bReverse;
}

#ifdef __cplusplus
}
#endif
