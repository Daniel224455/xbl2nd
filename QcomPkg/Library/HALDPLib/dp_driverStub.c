/*=============================================================================

  File: Dp_DriverStub.c

  DP stubs for the case where DP is not supported by platform


 Copyright (c) 2016-2018 Qualcomm Technologies, Inc.  All Rights Reserved.
 Qualcomm Technologies Proprietary and Confidential.
 =============================================================================*/
#ifdef __cplusplus
extern "C" {
#endif


/*=========================================================================
      Include Files
==========================================================================*/

#include "MDPLib_i.h"
#include "dp_i.h"

/*=========================================================================
      Public Functions
==========================================================================*/


/****************************************************************************
*
** FUNCTION: DPDriver_Open()
*/
/*!
* \brief
*   Initialize DP driver (host and panel)
*
* \param [in]   eDisplayId           - DP id
*
* \retval MDP_Status
*
****************************************************************************/
DP_Status DPDriver_Open(MDP_Display_IDType eDisplayId)
{
  return DP_STATUS_FAIL;
}

/****************************************************************************
*
** FUNCTION: DPDriver_IsDisplayPlugged()
*/
/*!
* \brief
*   Check if a panel is plugged into the DP interface
i*
* \param [in]   eDisplayId           - DP id
*
* \retval bool32
*
****************************************************************************/
DP_Status DPDriver_IsDisplayPlugged(MDP_Display_IDType eDisplayId)
{
  return FALSE;
}

/************************************************************************************************************
*
** FUNCTION: DPDriver_GetModeInfo()
*/
/*!
* \brief
*   Retrieves the mode info of the display plugged into the DP interface
*
* \param [in]   eDisplayId           - DP id
* \param [in]   uModeIndex           - Index of mode to retrieve
* \param [out]  psDisplayAttr        - Pointer to mode info
*
* \retval MDP_Status          MDP_STATUS_OK = mode supported, MDP_STATUS_NOT_SUPPORTED = mode not supported
*
**************************************************************************************************************/
DP_Status DPDriver_GetModeInfo(MDP_Display_IDType eDisplayId, uint32 uModeIndex, MDP_Panel_AttrType *psDisplayAttr)
{
  return DP_STATUS_FAIL;
}

/************************************************************************************************************
*
** FUNCTION: DPDriver_SetMode()
*/
/*!
* \brief
*   Set a mode on the display plugged into the DP interface
*
* \param [in]   eDisplayId           - DP id
* \param [in]   uModeIndex           - Index of mode to set
*
* \retval MDP_Status          MDP_STATUS_OK = mode set successfully
*
**************************************************************************************************************/
DP_Status DPDriver_SetMode(MDP_Display_IDType eDisplayId, uint32 uModeIndex)
{
  return DP_STATUS_FAIL;
}


/************************************************************************************************************
*
** FUNCTION: DPDriver_Close()
*/
/*!
* \brief
*   Deinitialize DP driver (and power down components)
*
* \param [in]   eDisplayId           - DP id
*
* \retval MDP_Status
*
**************************************************************************************************************/
DP_Status DPDriver_Term(MDP_Display_IDType eDisplayId)
{
  return DP_STATUS_FAIL;
}


#ifdef __cplusplus
}
#endif

