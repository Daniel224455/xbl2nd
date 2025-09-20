/*=============================================================================
 
  File: ExtDisplay_Driver.c
 
  Interface layer for External display driver
  
 
 Copyright (c) 2016 - 2019 Qualcomm Technologies, Inc.  All Rights Reserved.
 Qualcomm Technologies Proprietary and Confidential.
 =============================================================================*/
#ifdef __cplusplus
extern "C" {
#endif


/*=========================================================================
      Include Files
==========================================================================*/
#include <Library/PcdLib.h>
#include "DPInterface.h"
#include "MDPSystem.h"
#include "ExternalDisplayDriver.h"


/*=========================================================================
      Types
==========================================================================*/

/*=========================================================================
      Global variables
==========================================================================*/


/*=========================================================================
      Public Functions
==========================================================================*/

/****************************************************************************
*
** FUNCTION: ExtDisp_SupportedByPlatform()
*/
/*!
* \brief
*   Does platform support external display
*
* \param [out]   pAttr           - Attributes of external display
*
* \retval bool32                   TRUE - supported, FALSE - not supported
*
****************************************************************************/
bool32 ExtDisp_SupportedByPlatform(ExtDisp_AttrType   *pAttr)
{
  bool32 bSupported     = FALSE;

  switch (PcdGet32(PcdExtDisplayType))
  {     
    case PCD_EXTERNAL_DISPLAY_DP:
      pAttr->ePhysConnect = MDP_DISPLAY_CONNECT_DP;
      bSupported          = TRUE;
      break;
   
    case PCD_EXTERNAL_DISPLAY_NONE:
    default:
      pAttr->ePhysConnect = MDP_DISPLAY_CONNECT_NONE;
      break;
  }

  return bSupported;
}

/****************************************************************************
*
** FUNCTION: ExtDisp_Init()
*/
/*!
* \brief
*   Initialize External display driver (host and panel)
*
*
* \retval MDP_Status
*
****************************************************************************/
MDP_Status ExtDisp_Init(MDP_Display_IDType eDisplayId, uint32 Flags)
{
  MDP_Status  eStatus = MDP_STATUS_OK;

  switch (PcdGet32(PcdExtDisplayType))
  { 
    case PCD_EXTERNAL_DISPLAY_DP:
      eStatus = DPDriver_Open(eDisplayId, Flags);
      break;
      
    default:
      break;
  }

  return eStatus;
}

/****************************************************************************
*
** FUNCTION: ExtDisp_IsDisplayPlugged()
*/
/*!
* \brief
*   Check if a panel is plugged into the External display interface
*
*
* \retval bool32
*
****************************************************************************/
bool32 ExtDisp_IsDisplayPlugged(MDP_Display_IDType eDisplayId)
{  
  bool32              bPluggedIn  = FALSE;

  switch (PcdGet32(PcdExtDisplayType))
  { 
    case PCD_EXTERNAL_DISPLAY_DP:
      bPluggedIn = DPDriver_IsDisplayPlugged(eDisplayId);
      break;
      
    default:
      break;
  }

  return bPluggedIn;
}

/************************************************************************************************************
*
** FUNCTION: ExtDisp_GetModeInfo()
*/
/*!
* \brief
*   Retrieves the mode info of the display plugged into the External display interface
*
* \param [out]  pPanelInfo            - Pointer to mode info.
*
* \retval MDP_Status          MDP_STATUS_OK = mode supported, MDP_STATUS_NOT_SUPPORTED = mode not supported
*
**************************************************************************************************************/
MDP_Status ExtDisp_GetModeInfo(MDP_Panel_AttrType *pPanelInfo)
{  
  MDP_Status          eStatus       = MDP_STATUS_OK;  

  switch (PcdGet32(PcdExtDisplayType))
  { 
    case PCD_EXTERNAL_DISPLAY_DP:
      eStatus = DPDriver_GetModeInfo(pPanelInfo);      
      break;
      
    default:
      break;
  }

  return eStatus;
}

/************************************************************************************************************
*
** FUNCTION: ExtDisp_SetMode()
*/
/*!
* \brief
*   Set a mode on the display plugged into the External display interface
*
* \param [in]  pPanelInfo            - Pointer to mode info.
* \param [in]  uFlags                - Associated flags
*
* \retval MDP_Status          MDP_STATUS_OK = mode set successfully
*
**************************************************************************************************************/
MDP_Status ExtDisp_SetMode(MDP_Panel_AttrType *pPanelInfo, uint32 uFlags)
{
  MDP_Status  eStatus = MDP_STATUS_OK;  

  switch (PcdGet32(PcdExtDisplayType))
  { 
    case PCD_EXTERNAL_DISPLAY_DP:
      eStatus = DPDriver_SetMode(pPanelInfo, uFlags);
      break;
      
    default:
      break;
  } 
  
  return eStatus;
}

/************************************************************************************************************
*
** FUNCTION: ExtDisp_Close()
*/
/*!
* \brief
*   Deinitialize External display driver (and power down components)
*
*
* \retval MDP_Status          
*
**************************************************************************************************************/
MDP_Status ExtDisp_Close(MDP_Display_IDType eDisplayId)
{
  MDP_Status  eStatus = MDP_STATUS_OK;  

  switch (PcdGet32(PcdExtDisplayType))
  { 
    case PCD_EXTERNAL_DISPLAY_DP:
      eStatus = DPDriver_Term(eDisplayId);
      break;
      
    default:
      break;
  } 
  
  return eStatus;
}



