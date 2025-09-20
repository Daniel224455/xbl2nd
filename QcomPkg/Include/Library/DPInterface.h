#ifndef DPINTERFACE_H
#define DPINTERFACE_H
/*=============================================================================
 
  File: DPInterface.h
 
  Header file for DP driver
  
 
  Copyright (c) 2018-2020 Qualcomm Technologies, Inc.  All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential.
=============================================================================*/

#include "MDPTypes.h"
#include "../MDPLib/MDPLib_i.h"

/* -----------------------------------------------------------------------
**    Defines
-------------------------------------------------------------------------- */

/* Flags for DP_Driver_Open function */
#define DPINTERFACE_PLUG_INVERTED         0x00000001      /* configure the thpe-c for an inverted polarity */

/* -----------------------------------------------------------------------
** Function Declarations
-------------------------------------------------------------------------- */


/****************************************************************************
*
** FUNCTION: DPDriver_Open()
*/
/*!
* \brief
*   Initialize DP driver (host and panel)
*
* \param [in]   eDisplayId           - display id
* \param [in]   Flags                - DP driver flags
*
* \retval MDP_Status
*
****************************************************************************/
MDP_Status DPDriver_Open(MDP_Display_IDType eDisplayId, uint32 Flags);

/****************************************************************************
*
** FUNCTION: DP_Driver_IsDisplayPlugged()
*/
/*!
* \brief
*   Check if a panel is plugged into the DP interface
*
* \param [in]   eDisplayId           - display id
*
* \retval bool32
*
****************************************************************************/
bool32 DPDriver_IsDisplayPlugged( MDP_Display_IDType eDisplayId);

/************************************************************************************************************
*
** FUNCTION: DP_Driver_GetModeInfo()
*/
/*!
* \brief
*   Retrieves the mode info of the display plugged into the DP interface
*
* \param [out]  pPanelInfo            - Pointer to mode info.
*
* \retval MDP_Status          MDP_STATUS_OK = mode supported, MDP_STATUS_NOT_SUPPORTED = mode not supported
*
**************************************************************************************************************/
MDP_Status DPDriver_GetModeInfo(MDP_Panel_AttrType *pPanelInfo);

/************************************************************************************************************
*
** FUNCTION: DP_Driver_SetMode()
*/
/*!
* \brief
*   Set a mode on the display plugged into the DP interface using either a mode index or panel info
*   Set pPanelInfo to NULL to set the mode index based on the EDID data
*
* \param [in]  pPanelInfo            - Pointer to mode info.
* \param [in]  uFlags                - Associated flags
*
* \retval MDP_Status          MDP_STATUS_OK = mode set successfully
*
**************************************************************************************************************/
MDP_Status DPDriver_SetMode(MDP_Panel_AttrType *pPanelInfo, uint32 uFlags);

/* ----------------------------------------------------------------------
** FUNCTION: DPDriver_SendCommandSequence()
**
** DESCRIPTION:
**   Send DP command sequence
**
* \param [in]  pPanelInfo     - panel info attributes
* \param [in]  eCommand       - Panel commands
* \param [in]  pPayload       - meta data payload
* \param [in]  uFlags         - Reserved
*
* \retval MDP_Status
**
**
-------------------------------------------------------------------- */
MDP_Status DPDriver_SendCommandSequence(MDP_Panel_AttrType *pPanelInfo, MdpPanelCommandType eCommand, void *pPayload, uint32 uFlags);


/************************************************************************************************************
*
** FUNCTION: DP_Driver_Close()
*/
/*!
* \brief
*   Deinitialize DP driver (and power down components)
*
* \param [in]   eDisplayId           - display id
*
* \retval MDP_Status          
*
**************************************************************************************************************/
MDP_Status DPDriver_Term(MDP_Display_IDType eDisplayId);

/************************************************************************************************************
*
** FUNCTION: DPDriver_GetFrameCRC()
*/
/*!
* \brief
*    Enable CRC capture or read from CRC registers
*
* \param [in]      pPanelInfo       - panel info
* \param [in/out]  pCRC             - CRC register information
*
* \retval MDP_Status
*
**************************************************************************************************************/
MDP_Status DPDriver_GetFrameCRC(MDP_Panel_AttrType *pPanelInfo, uint64 *pCRC);

/************************************************************************************************************
*
** FUNCTION: DPDriver_BrightnessEnable()
*/
/*!
* \brief
*    Enable panel brightness through AUX
*
* \param [in]      eDisplayId       - display id
* \param [in/out]  bEnable          - TRUE to enable brightness
*
* \retval MDP_Status
*
**************************************************************************************************************/

MDP_Status DPDriver_BrightnessEnable(MDP_Display_IDType eDisplayId, bool32 bEnable);

/************************************************************************************************************
*
** FUNCTION: DPDriver_BrightnessLevel()
*/
/*!
* \brief
*    Set panel brightness to requested level
*
* \param [in]      eDisplayId       - display id
* \param [in/out]  fNewLevel        - brightness level to set [0.0 - 1.0]
*
* \retval MDP_Status
*
**************************************************************************************************************/

MDP_Status DPDriver_BrightnessLevel(MDP_Display_IDType eDisplayId, float fNewLevel);

/************************************************************************************************************
*
** FUNCTION: DPDriver_UpdateBondedModeTimings()
*/
/*!
* \brief
*    Scale the panel attribute timings to represent the display with all bonded devices combined.
*
* \param [in]      pPanelAttr       - Pointer to panel attributes
*
* \retval NONE
*
**************************************************************************************************************/
void DPDriver_UpdateBondedModeTimings(MDP_Panel_AttrType *pPanelAttr);

#endif  /* #define DPINTERFACE_H */
