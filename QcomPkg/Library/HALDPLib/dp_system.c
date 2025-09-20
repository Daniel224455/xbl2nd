/*=====================================================================================================
  Copyright (c) 2016-2020 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
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

// Get Clock type for DP controller
#define DP_GETCLOCKTYPE(_id_)                        (DP_CONTROLLER_ID_MAX > (_id_) ? DP_Clocks_5xx[(_id_)] : MDP_CLOCKTYPE_MAX)


// Wait time for video ready
#define DP_READY_FOR_VIDEO_TIMEOUT_US                5000      // 5 ms
#define DP_READY_FOR_VIDEO_POLL_INTERVAL_US          1         // 1 Us
#define DP_HPD_POLL_TIME_MS                          16        // 16 ms


// DP wait times
#define EDP_HPD_ENABLE_WAIT_TIME_MS                  200       // eDP1.3/1.4 spec 200ms, eDP1.4a 80ms
#define DP_HPD_ENABLE_WAIT_TIME_MS                   110       // According to HPG, DP HPD IN detection will need up to 100ms, plus 10ms as buffer


/* -----------------------------------------------------------------------
 * Static data
 * ----------------------------------------------------------------------- */

/* DP controller to clock type map */
static const MDPClockTypes DP_Clocks_5xx[DP_CONTROLLER_ID_MAX] =
{
  MDP_CLOCKTYPE_MAX,        // DP_CONTROLLER_ID_NONE
  MDP_CLOCKTYPE_DP0,        // DP_CONTROLLER_ID_DP_0
  MDP_CLOCKTYPE_DP1,        // DP_CONTROLLER_ID_DP_1
  MDP_CLOCKTYPE_EDP,        // DP_CONTROLLER_ID_EDP
};
 
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
   hHandle     -[in]    : Handle to DP device
  bEnable      -[in]    :  Enable/Disable Clock

Return: 
  DP_Status

**********************************************************************************************************************/
DP_Status DP_AUXClockEnable(DP_HandleType    hHandle,
                            bool32           bEnable)
{
  DP_Status           eStatus        = DP_STATUS_SUCCESS;
  DP_DeviceCtxType   *pDeviceCtx     = (DP_DeviceCtxType *)hHandle;
  MDPClockTypes       eDPClockType   = DP_GETCLOCKTYPE(pDeviceCtx->eControllerID);

  // In MDPClocks we don't have a way to turn on AUX clock by itself
  // this will turn on all DP clocks, but that's ok we will set their
  // external sources later by calling this again in DP_MainClockConfig
  if ((MDP_CLOCKTYPE_MAX == eDPClockType) ||
      (MDP_STATUS_OK     != MDPSetupClocks(eDPClockType, NULL)))
  {
    DP_LOG_MESSAGE_ERROR("DP Driver: DP%d AUX clock source could not be configured.\n", pDeviceCtx->eDeviceID);
    eStatus = DP_STATUS_FAILED_RESOURCE_FAILED;
  }

  return eStatus;
}



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
                             bool32               bEnable)
{
  DP_Status                  eStatus          = DP_STATUS_SUCCESS;
  DP_DeviceCtxType          *pDeviceCtx       = (DP_DeviceCtxType *)hHandle;
  MDPClockTypes              eDPClockType     = DP_GETCLOCKTYPE(pDeviceCtx->eControllerID);
  MDPExternalClockEntry     *pDPExtClock      = MDPGetExternalClockEntry(eDPClockType, FALSE);

  if ((NULL == pDPExtClock) ||
      (pDeviceCtx->eDeviceID >= DP_DEVICE_MAX))
  {
    DP_LOG_MESSAGE_ERROR("DP Driver: DP%d is not supported.\n", pDeviceCtx->eDeviceID);
    eStatus = DP_STATUS_FAILED_NOT_SUPPORTED;
  }
  else if (bEnable)
  {
    /* Clock source mappings:
     * ---------------------------------------------------------------------------------------------------------------------------------------
     * DP Device | Pixel clock source          | Link clock source       | Crypto clock source     | Aux clock source | Link intf clock source 
     * ---------------------------------------------------------------------------------------------------------------------------------------
     * USB0      | dp_phy_pll_vco_div_clk      | dp_phy_pll_link_clk     | dp_phy_pll_link_clk     | CXO              | dp_phy_pll_link_clk
     * USB1      | dptx1_phy_pll_vco_div_clk   | dptx1_phy_pll_link_clk  | dptx1_phy_pll_link_clk  | CXO              | dptx1_phy_pll_link_clk
     * DP        | dptx2_phy_pll_vco_div_clk   | dptx2_phy_pll_link_clk  | dptx2_phy_pll_link_clk  | CXO              | dptx2_phy_pll_link_clk
     * eDP       | edp_phy_pll_vco_div_clk     | edp_phy_pll_link_clk    | edp_phy_pll_link_clk    | CXO              | edp_phy_pll_link_clk
     */

    DP_DeviceIDType ePixelClockSourceDeviceID;
    uint32          uClockSource[DP_DEVICE_MAX][5] =
    {
      // Pixel clock, Link clock, Crypto clock, Aux clock, Link intf clock
      {0, 0, 0, 0, 0},  // DP_DEVICE_ID_NONE
      {2, 1, 1, 0, 1},  // DP_DEVICE_ID_ALT_MODE_0
      {4, 3, 3, 0, 3},  // DP_DEVICE_ID_ALT_MODE_1
      {6, 5, 5, 0, 5},  // DP_DEVICE_ID_DP
      {2, 1, 0, 0, 1}   // DP_DEVICE_ID_EDP
    };

    // Pixel clock Dividers - Index 0 in ext resource list for DP
    pDPExtClock[0].nClkDiv    = pPixClkDivs->uPixelDivider;    // this is a div2x value
    pDPExtClock[0].uClkPLL_M  = pPixClkDivs->uPixelClk_M;
    pDPExtClock[0].uClkPLL_N  = pPixClkDivs->uPixelClk_N;
    pDPExtClock[0].uClkPLL_2D = pPixClkDivs->uPixelClk_2D;

    if (DP_BONDED_MODE_SHARED_RESOURCE_EXTERNAL_PIXEL_CLOCK == pDeviceCtx->sBondedModeConfig.eSharedResourceConfig)
    {
      // If bonded mode configuration is active, the pixel clock is sourced from the vco_div_clk of the master PLL
      // Note: eDP vco_div_clk cannot be configured as pixel clock source for other DP blocks
      if (pDeviceCtx->sBondedModeConfig.eSourceClockDeviceID < DP_DEVICE_MAX)
      {
        ePixelClockSourceDeviceID = pDeviceCtx->sBondedModeConfig.eSourceClockDeviceID;
      }
      else
      {
        ePixelClockSourceDeviceID = 0;
        DP_LOG_MESSAGE_ERROR("DP_MainClockEnable: Device ID of device sourcing external pixel clock invalid, DP:%d\n", pDeviceCtx->sBondedModeConfig.eSourceClockDeviceID);
      }
    }
    else
    {
      ePixelClockSourceDeviceID = pDeviceCtx->eDeviceID;
    }

    pDPExtClock[0].uClkSource = uClockSource[ePixelClockSourceDeviceID][0];
    pDPExtClock[1].uClkSource = uClockSource[pDeviceCtx->eDeviceID][1];
    pDPExtClock[2].uClkSource = uClockSource[pDeviceCtx->eDeviceID][2];
    pDPExtClock[3].uClkSource = uClockSource[pDeviceCtx->eDeviceID][3];
    pDPExtClock[4].uClkSource = uClockSource[pDeviceCtx->eDeviceID][4];

    if (MDP_STATUS_OK != MDPSetupClocks(eDPClockType, pDPExtClock))
    {
      DP_LOG_MESSAGE_ERROR("DP Driver: DP%d clock source could not be configured.\n", pDeviceCtx->eDeviceID);
      eStatus = DP_STATUS_FAILED_RESOURCE_FAILED;
    }
  }
  else 
  {
     if (MDP_STATUS_OK != MDPDisableClocks(MDP_CLOCKTYPE_DP))
     {
       DP_LOG_MESSAGE_ERROR("DP Driver: DP%d clock source could not be shutdown.\n", pDeviceCtx->eDeviceID);
       eStatus = DP_STATUS_FAILED_RESOURCE_FAILED;
     }
  }

  return eStatus;
}

/**********************************************************************************************************************

FUNCTION: DP_WaitForReadyForVideo()
   
   This function will Wait for DP control status to be ready for video 

Parameters:
   hHandle    -[in]    : Handle to DP device

Return: 
   DP_Status

**********************************************************************************************************************/
DP_Status DP_WaitForReadyForVideo(DP_HandleType        hHandle)
{
  DP_Status                       eStatus        = DP_STATUS_FAILED_TIMEOUT;
  DP_DeviceCtxType               *pDeviceCtx     = (DP_DeviceCtxType *)hHandle;
  uint32                          i              = 0;  
  uint32                          uLinkStatus    = 0;
  uint32                          uPollCount     = (DP_READY_FOR_VIDEO_TIMEOUT_US / DP_READY_FOR_VIDEO_POLL_INTERVAL_US) + 1;

  for (i = 0; i < uPollCount; ++i)
  {    
    uLinkStatus = HAL_DP_Mainlink_Status(pDeviceCtx->eControllerID);
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
   hHandle    -[in]    : Handle to DP device

Return:
   bool32      TRUE - reverse of spec      FALSE - as per spec

**********************************************************************************************************************/
bool32 DP_PhyDataPathIsReverse(DP_HandleType        hHandle)
{
  bool32               bReverse     = FALSE;
  HAL_HW_VersionType   sDPHwVersion = { 0 };
  DP_DeviceCtxType    *pDeviceCtx   = (DP_DeviceCtxType *)hHandle;

  HAL_DP_ReadVersionInfo(pDeviceCtx->eControllerID, &sDPHwVersion);

  if ((1 == sDPHwVersion.uMajorVersion) &&
      (0 == sDPHwVersion.uMinorVersion))
  {
    bReverse = TRUE;
  }

  return bReverse;
}

/**********************************************************************************************************************

FUNCTION: DP_SetAuxPolarityGPIO()

   This function will set GPIO to select the AUX channel polarity

Parameters:
   hHandle    -[in]    : Handle to DP device
   bReverted  -[in]    : Set GPIO level to select AUX polarity

Return:
   DP_Status

**********************************************************************************************************************/
DP_Status DP_SetAuxPolarityGPIO(DP_HandleType      hHandle,
                                bool32             bReverted)
{
  // #TODO#IMPLEMENT
  return DP_STATUS_SUCCESS;
}

/**********************************************************************************************************************

FUNCTION: DP_CheckCOnnectionStatus()

   This function detect HPD connection status

Parameters:
   hHandle    -[in]    : Handle to DP device

Return:
   DP_Status

**********************************************************************************************************************/
DP_Status DP_CheckConnectionStatus(DP_HandleType      hHandle)
{
  DP_DeviceCtxType *pDeviceCtx = (DP_DeviceCtxType *)hHandle;
  DP_Status  eStatus           = DP_STATUS_FAILED_TIMEOUT;
  uint32     uLoopDelay        = 5;
  uint32     uLoop;
  uint32     eConnect;
  uint32     uMaxDelay;
  
  switch (pDeviceCtx->eDeviceID)
  {
    case DP_DEVICE_ID_ALT_MODE_0:
    case DP_DEVICE_ID_ALT_MODE_1:
    case DP_DEVICE_ID_DP:
      uMaxDelay = DP_HPD_ENABLE_WAIT_TIME_MS;
      break;
    case DP_DEVICE_ID_EDP:
      uMaxDelay = EDP_HPD_ENABLE_WAIT_TIME_MS;
    default:
      uMaxDelay = DP_HPD_ENABLE_WAIT_TIME_MS;
      break;
  }
  
  for (uLoop = 0; uLoop < uMaxDelay/uLoopDelay; uLoop++)
  {
    eConnect = HAL_DP_HPD_Status(pDeviceCtx->eControllerID);
    if ((HAL_HPD_STATUS_CONNECT_PENDING == eConnect) ||
        (HAL_HPD_STATUS_CONNECTED == eConnect))
    {
      eStatus = DP_STATUS_SUCCESS;
      break;
    }
    MDP_OSAL_DELAYMS(uLoopDelay);
  }
  
  return eStatus;
}

#ifdef __cplusplus
}
#endif
