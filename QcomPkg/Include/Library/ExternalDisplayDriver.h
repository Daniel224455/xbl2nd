#ifndef EXTERNALDISPLAYDRIVER_H
#define EXTERNALDISPLAYDRIVER_H
/*=============================================================================
 
  File: ExternalDisplayDriver.h
 
  Header file for DP driver
  
 
  Copyright (c) 2016-2018 Qualcomm Technologies, Inc.  All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential.
=============================================================================*/

#include "MDPTypes.h"
#include "../MDPLib/MDPLib_i.h"


/* --------------------------------------------------------------------------------------------------------------------
**  Enumerated types
---------------------------------------------------------------------------------------------------------------------*/


/* --------------------------------------------------------------------------------------------------------------------
**  Data types
---------------------------------------------------------------------------------------------------------------------*/

typedef struct
{
  MDP_DisplayConnectType          ePhysConnect;                                   /**< Physical connection of display. */
} ExtDisp_AttrType;



/* -----------------------------------------------------------------------
** Function Declarations
-------------------------------------------------------------------------- */

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
bool32 ExtDisp_SupportedByPlatform(ExtDisp_AttrType   *pAttr);


/****************************************************************************
*
** FUNCTION: ExtDisp_Init()
*/
/*!
* \brief
*   Initialize External display driver (host and panel)
*
* \param [in]   eDisplayId           - display id
* \param [in]   Flags                - DP Driver flags
*
* \retval MDP_Status
*
****************************************************************************/
MDP_Status ExtDisp_Init(MDP_Display_IDType eDisplayId, uint32 Flags);

/****************************************************************************
*
** FUNCTION: DP_Driver_IsDisplayPlugged()
*/
/*!
* \brief
*   Check if a panel is plugged into the External display interface
*
* \param [in]   eDisplayId           - display id
*
* \retval bool32
*
****************************************************************************/
bool32 ExtDisp_IsDisplayPlugged(MDP_Display_IDType eDisplayId);

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
MDP_Status ExtDisp_GetModeInfo(MDP_Panel_AttrType *pPanelInfo);

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
MDP_Status ExtDisp_SetMode(MDP_Panel_AttrType *pPanelInfo, uint32 uFlags);

/************************************************************************************************************
*
** FUNCTION: ExtDisp_Close()
*/
/*!
* \brief
*   Deinitialize External display driver (and power down components)
*
* \param [in]   eDisplayId           - display id
*
* \retval MDP_Status          
*
**************************************************************************************************************/
MDP_Status ExtDisp_Close(MDP_Display_IDType eDisplayId);


#endif  /* #define EXTERNALDISPLAYDRIVER_H */
