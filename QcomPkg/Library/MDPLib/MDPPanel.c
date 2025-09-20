/*=============================================================================
 
  File: MDPPanel.c
 
  Source file for panel specific configuration
  
 
  Copyright (c) 2011-2020 Qualcomm Technologies, Inc.  All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential.
=============================================================================*/
/*=========================================================================
      Include Files
==========================================================================*/
#include "MDPLib.h"
#include "MDPPlatformLib.h"
#include "HALDSILib.h"
#include "ExternalDisplayDriver.h"
#include "DPInterface.h"

/* -----------------------------------------------------------------------
** Defines
** ----------------------------------------------------------------------- */

/*=========================================================================
      Local prototypes
==========================================================================*/




/*=========================================================================
      Public functions
==========================================================================*/

/****************************************************************************
*
** FUNCTION: MDPPanelDeInit()
*/
/*!
* \brief
*   This function de-intializes the interfaces.
*
* \param [in] pPanelInfo    - The panel configuration
*
* \retval MDP_Status
*
****************************************************************************/
MDP_Status  MDPPanelDeInit(MDP_Panel_AttrType    *pPanelInfo)
{
  MDP_Status eStatus    = MDP_STATUS_OK;

  switch (pPanelInfo->eDisplayId)
  {
    case MDP_DISPLAY_PRIMARY:
    case MDP_DISPLAY_SECONDARY:
      switch (pPanelInfo->ePhysConnect)
      {         
        case MDP_DISPLAY_CONNECT_PRIMARY_DSI_VIDEO:
        case MDP_DISPLAY_CONNECT_PRIMARY_DSI_CMD:
        case MDP_DISPLAY_CONNECT_SECONDARY_DSI_VIDEO:
        case MDP_DISPLAY_CONNECT_SECONDARY_DSI_CMD:
    
          // Graceful shutdown of the panel
          DsiPanelShutdown(pPanelInfo);
          
          DSIDriver_Close(pPanelInfo->eDisplayId);
          break;

        case MDP_DISPLAY_CONNECT_EDP:
        case MDP_DISPLAY_CONNECT_DP:
          DPDriver_Term(pPanelInfo->eDisplayId);
          break;

        default:
          break;
      }
      break;
   
    case MDP_DISPLAY_EXTERNAL:
    case MDP_DISPLAY_EXTERNAL2:
    case MDP_DISPLAY_EXTERNAL3:
      eStatus = ExtDisp_Close(pPanelInfo->eDisplayId);
      break;

    default:
      break;
  }

  return eStatus;
}


/****************************************************************************
*
** FUNCTION: MDPPanelInit()
*/
/*!
* \brief
*   This function setup the display for a particular mode.
*
* \param [in] eDisplayId    - The display to initialize
* \param [in] uModeIndex    - The display mode to configure
*        [in] pPanelInfo    - The panel configuration
*
* \retval MDP_Status
*
****************************************************************************/
MDP_Status  MDPPanelInit(MDP_Display_IDType eDisplayId, uint32 uModeIndex, MDP_Panel_AttrType *pPanelInfo)
{
    MDP_Status eStatus    = MDP_STATUS_OK;

    switch (pPanelInfo->eDisplayId)
    {
      case MDP_DISPLAY_PRIMARY:
      case MDP_DISPLAY_SECONDARY:
        switch (pPanelInfo->ePhysConnect)
        {
          case MDP_DISPLAY_CONNECT_PRIMARY_DSI_VIDEO:
          case MDP_DISPLAY_CONNECT_PRIMARY_DSI_CMD:
          case MDP_DISPLAY_CONNECT_SECONDARY_DSI_VIDEO:
          case MDP_DISPLAY_CONNECT_SECONDARY_DSI_CMD:
            /* Setup DSI Configuration */
            eStatus = DsiPanelInit(pPanelInfo);
            break;

          case MDP_DISPLAY_CONNECT_EDP:
          case MDP_DISPLAY_CONNECT_DP:
          {
            eStatus = DPDriver_SetMode(pPanelInfo, 0x0);
            break;
          }
          default:
            break;
        }
        break;
     
      case MDP_DISPLAY_EXTERNAL:
      case MDP_DISPLAY_EXTERNAL2:
      case MDP_DISPLAY_EXTERNAL3:
        pPanelInfo->eDisplayFlags |= MDP_PANEL_FLAG_TIMING_FROM_EDID; /* always panel timing from EDID */
        eStatus = ExtDisp_SetMode(pPanelInfo, 0);
        break;
  
      default:
        break;
    }

    return eStatus;
}

/****************************************************************************
*
** FUNCTION: MDPPanelGetEDID()
*/
/*!
* \brief
*   This function read EDID info from panel.
*
* \param [in] pPanelInfo    - The panel configuration
*
* \retval MDP_Status
*
****************************************************************************/
MDP_Status  MDPPanelGetEDID( MDP_Panel_AttrType    *pPanelInfo)
{
  MDP_Status eStatus    = MDP_STATUS_OK;

  /* Configure the display */
  switch (pPanelInfo->ePhysConnect)
  {
    case MDP_DISPLAY_CONNECT_EDP:
    {
      //#TODO#IMPLEMENT
      eStatus = MDP_STATUS_NOT_SUPPORTED;
      break;
    }
    case MDP_DISPLAY_CONNECT_DP:
    {
      eStatus = MDP_STATUS_NOT_SUPPORTED;
      break;
    }
    default:
    {
      break;
    }
  }

  return eStatus;
}

/****************************************************************************
*
** FUNCTION: MDPPanelSendCommandSequence()
*/
/*!
* \brief
*   Sends a predefined command set to the panel.
*        [in] pPanelInfo      - The panel configuration
*        [in] eCommandType    - Command type to send
*        [in] pCustomCommand  - Custom command buffer, only valid for MDP_PANEL_COMMAND_CUSTOM
*        [in] uFlags          - Reserved.
*
* \retval MDP_Status
*
****************************************************************************/
MDP_Status  MDPPanelSendCommandSequence(MDP_Panel_AttrType *pPanelInfo, MdpPanelCommandType eCommand, void *pCustomCommand, uint32 uFlags)
{
    MDP_Status eStatus    = MDP_STATUS_OK;
 
    /* Configure the display */
    switch (pPanelInfo->ePhysConnect)
    {     
      case MDP_DISPLAY_CONNECT_PRIMARY_DSI_VIDEO:
      case MDP_DISPLAY_CONNECT_PRIMARY_DSI_CMD:
      case MDP_DISPLAY_CONNECT_SECONDARY_DSI_VIDEO:
      case MDP_DISPLAY_CONNECT_SECONDARY_DSI_CMD:
        /* Setup DSI Configuration */
        eStatus = DsiPanelSendCommandSequence(pPanelInfo, eCommand, pCustomCommand, uFlags);
        break;
      case MDP_DISPLAY_CONNECT_DP_0:
      case MDP_DISPLAY_CONNECT_DP_1:
        eStatus = DPDriver_SendCommandSequence(pPanelInfo, eCommand, pCustomCommand, uFlags);
        break;
      default: 
        break;
    }

    return eStatus;
}


