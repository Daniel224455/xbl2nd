/*=============================================================================
 
  File: hal_dp_phy.c
 
  Source file for DP PHY hardware functionality 
   
  Copyright (c) 2016-2020 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
=============================================================================*/

#include "hal_dp_i.h"
#include "hal_dp_phy.h"


#ifdef __cplusplus
extern "C" {
#endif

/*---------------------------------------------------------------------------------------------------------------------
  Macros
---------------------------------------------------------------------------------------------------------------------*/



/* -----------------------------------------------------------------------
** Types
** ----------------------------------------------------------------------- */






/* -----------------------------------------------------------------------
** Statics
** ----------------------------------------------------------------------- */

HAL_DP_PhyPllFunctionTable gDPPhyPllFxnTable[DP_DEVICE_MAX];

/*---------------------------------------------------------------------------------------------------------------------
  Public Functions
---------------------------------------------------------------------------------------------------------------------*/


/****************************************************************************
*
** FUNCTION: HAL_DP_PhyFxnsInit()
*/
/*!
* \DESCRIPTION
*   Initialize DP PHY/PLL function table
*
* \retval None
*
****************************************************************************/
void HAL_DP_PhyFxnsInit(void)
{
  uint32             uRegValue     = in_dword(HWIO_DP_HW_VERSION_ADDR);
  HAL_HW_VersionType sCoreVersion;

  sCoreVersion.uMajorVersion   = HWIO_GETVAL(DP_HW_VERSION, uRegValue, MAJOR);
  sCoreVersion.uMinorVersion   = HWIO_GETVAL(DP_HW_VERSION, uRegValue, MINOR);
  sCoreVersion.uReleaseVersion = HWIO_GETVAL(DP_HW_VERSION, uRegValue, STEP);

  //clear function table
  DP_OSAL_MemSet(&gDPPhyPllFxnTable, 0, sizeof(HAL_DP_PhyPllFunctionTable) * DP_DEVICE_MAX);

  if ((1 == sCoreVersion.uMajorVersion) &&
      (2 == sCoreVersion.uMinorVersion))
  {
    DP_DeviceIDType eDeviceId;

    for (eDeviceId = DP_DEVICE_ID_ALT_MODE_0; eDeviceId < DP_DEVICE_MAX; eDeviceId++)
    {
      switch (eDeviceId)
      {
      case DP_DEVICE_ID_ALT_MODE_0:
      case DP_DEVICE_ID_ALT_MODE_1:
        HAL_DP_AltMode_1_2_0_PhyFxnsInit(&gDPPhyPllFxnTable[eDeviceId]);
        break;
      case DP_DEVICE_ID_DP:
      case DP_DEVICE_ID_EDP:
        HAL_DP_1_2_0_PhyFxnsInit(&gDPPhyPllFxnTable[eDeviceId]);
        break;
      default:
        break;
      }
    }
  }
  else
  {
    DP_LOG_MESSAGE_ERROR("HAL_DP_PhyFxnsInit: major=%d minor=%d is NOT supported\n", sCoreVersion.uMajorVersion, sCoreVersion.uMinorVersion);
  }
}

/*********************************************************************************************
*
** FUNCTION: HAL_DP_SetupPHYPeSwLevel()
*/
/*!
* \brief
*        Set up DP Lane Pre Emphasis and voltage swing level
*
* \param [in]     eDeviceId   - DP Device ID
* \param [in/out] pPeSwConfig - Pre-emphasis/Swing level settings.
*
* \retval  HAL_MDSS_ErrorType
*
**********************************************************************************************/
HAL_MDSS_ErrorType HAL_DP_SetupPHYPeSwLevel(DP_DeviceIDType        eDeviceId,
                                            HAL_DP_PeSwConfigType *pPeSwConfig)
{
  HAL_MDSS_ErrorType          eStatus   = HAL_MDSS_STATUS_SUCCESS;
  HAL_DP_PhyPllFunctionTable *pFxnTable = &gDPPhyPllFxnTable[eDeviceId];

  if (NULL != pFxnTable->HAL_DP_PHY_SetupPeSw)
  {
    eStatus = pFxnTable->HAL_DP_PHY_SetupPeSw(eDeviceId, pPeSwConfig);
  }
  else
  {
    eStatus = HAL_MDSS_STATUS_FAILED_NOT_IMPLEMENTED;
  }

  return eStatus;
}

/****************************************************************************
*
** FUNCTION: HAL_DP_PHY_Config()
*/
/*!
* \DESCRIPTION
*           Configure DP PHY and PLL 
*
* \param [in]   eDeviceId             - DP Device ID
* \param [in]   HAL_DP_PllConfigType  - DP PHY PLL config info
*
* \retval boolean  
*
****************************************************************************/
bool32 HAL_DP_PHY_Config(DP_DeviceIDType       eDeviceId,
                         HAL_DP_PllConfigType *pDPPllConfigInfo)
{
  bool32                      bStatus   = FALSE;
  HAL_DP_PhyPllFunctionTable *pFxnTable = &gDPPhyPllFxnTable[eDeviceId];

  if (NULL != pFxnTable->HAL_DP_PHY_Config)
  {
    bStatus = pFxnTable->HAL_DP_PHY_Config(eDeviceId, pDPPllConfigInfo);
  }

  return bStatus;
}

/****************************************************************************
*
** FUNCTION: HAL_DP_PHY_Reset()
*/
/*!
* \DESCRIPTION
*           Reset DP PHY
*
* \param [in]     eDeviceId   - DP Device ID
* \param [in]     eModules    - Modules to reset
*
* \retval   
*
****************************************************************************/
void HAL_DP_PHY_Reset(DP_DeviceIDType      eDeviceId,
                      DP_PHYResetFlagType  eModules)
{
  HAL_DP_PhyPllFunctionTable *pFxnTable = &gDPPhyPllFxnTable[eDeviceId];

  if (NULL != pFxnTable->HAL_DP_PHY_Reset)
  {
    pFxnTable->HAL_DP_PHY_Reset(eDeviceId, eModules);
  }
}

/****************************************************************************
*
** FUNCTION: HAL_DP_PHY_AUX_Init()
*/
/*!
* \DESCRIPTION
*           Configure DP AUX PHY
*
* \param [in]     eDeviceId   - DP Device ID
*
* \retval   
*
****************************************************************************/
void HAL_DP_PHY_AUX_Init(DP_DeviceIDType eDeviceId)
{
  HAL_DP_PhyPllFunctionTable *pFxnTable = &gDPPhyPllFxnTable[eDeviceId];

  if (NULL != pFxnTable->HAL_DP_PHY_AUX_Init)
  {
    pFxnTable->HAL_DP_PHY_AUX_Init(eDeviceId);
  }
}

/*********************************************************************************************
*
** FUNCTION: HAL_DP_PHY_GetMaxPeSwLevel()
*/
/*!
* \brief
*        Get max supported DP Lane Pre Emphasis and voltage swing levels
*
* \param [in]    eDeviceId        - DP Device ID
* \param [out]   uMaxPreemphLvl   - max Pre-emphasis level supported.
* \param [out]   uMaxVoltSwingLvl - max voltage swing level supported.
*
* \retval  void
*
**********************************************************************************************/
HAL_MDSS_ErrorType HAL_DP_PHY_GetMaxPeSwLevel(DP_DeviceIDType  eDeviceId,
                                              uint32          *uMaxPreemphLvl,
                                              uint32          *uMaxVoltSwingLvl)
{
  HAL_MDSS_ErrorType          eStatus   = HAL_MDSS_STATUS_SUCCESS;
  HAL_DP_PhyPllFunctionTable *pFxnTable = &gDPPhyPllFxnTable[eDeviceId];

  if (NULL != pFxnTable->HAL_DP_PHY_GetMaxPeSwLevel)
  {
    pFxnTable->HAL_DP_PHY_GetMaxPeSwLevel(eDeviceId, uMaxPreemphLvl, uMaxVoltSwingLvl);
  }
  else
  {
    eStatus = HAL_MDSS_STATUS_FAILED_NOT_SUPPORTED;
  }

  return eStatus;
}

/****************************************************************************
*
** FUNCTION: HAL_DP_PHY_Power()
*/
/*!
* \DESCRIPTION
*           Power on/off DP PHY
*
* \param [in]     eDeviceId - DP Device ID
* \param [in]     bPowerOn  - TRUE: power on, FALSE: power off
*
* \retval   
*
****************************************************************************/
void HAL_DP_PHY_Power(DP_DeviceIDType eDeviceId,
                      bool32          bPowerOn)
{
  HAL_DP_PhyPllFunctionTable *pFxnTable = &gDPPhyPllFxnTable[eDeviceId];

  if (NULL != pFxnTable->HAL_DP_PHY_Power)
  {
    pFxnTable->HAL_DP_PHY_Power(eDeviceId, bPowerOn);
  }
}

/****************************************************************************
*
** FUNCTION: HAL_DP_PHY_AUXConfig()
*/
/*!
* \DESCRIPTION
*           Power on/off DP PHY
*
* \param [in]     eDeviceId  - DP Device ID
* \param [in]     pAuxPhyCfg - Aux configuration
*
* \retval HAL_MDSS_ErrorType
*
****************************************************************************/
HAL_MDSS_ErrorType HAL_DP_PHY_AUXConfig(DP_DeviceIDType       eDeviceId,
                                        HAL_DP_AuxPhyCfgType *pAuxPhyCfg)
{
  HAL_MDSS_ErrorType          eStatus   = HAL_MDSS_STATUS_SUCCESS;
  HAL_DP_PhyPllFunctionTable *pFxnTable = &gDPPhyPllFxnTable[eDeviceId];

  if (NULL != pFxnTable->HAL_DP_PHY_AUXConfig)
  {
    eStatus = pFxnTable->HAL_DP_PHY_AUXConfig(eDeviceId, pAuxPhyCfg);
  }
  else
  {
    eStatus = HAL_MDSS_STATUS_FAILED_NOT_SUPPORTED;
  }

  return eStatus;
}


/****************************************************************************
*
** FUNCTION: HAL_DP_PHY_SetPHYMode()
*/
/*!
* \DESCRIPTION
*           Set the PHY Mode to DP or USB SS
*
* \param [in]     eDeviceId - DP Device ID
* \param [in]     ePHYMode  - PHY mode
*
* \retval
*
****************************************************************************/
void HAL_DP_PHY_SetPHYMode(DP_DeviceIDType eDeviceId,
                           DP_PHYModeType  ePHYMode)
{
  HAL_DP_PhyPllFunctionTable *pFxnTable = &gDPPhyPllFxnTable[eDeviceId];

  if (NULL != pFxnTable->HAL_DP_PHY_SetPHYMode)
  {
    pFxnTable->HAL_DP_PHY_SetPHYMode(eDeviceId, ePHYMode);
  }
}

/****************************************************************************
*
** FUNCTION: HAL_DP_PHY_GetPLLStatus()
*/
/*!
* \DESCRIPTION
*           Get DP PLL status
*
* \param [in]     eDeviceId   - DP Device ID
* \param [out]    pbPLLLocked - TRUE is DP PLL is locked, otherwise FALSE
*
* \retval void
*
****************************************************************************/
void HAL_DP_PHY_GetPLLStatus(DP_DeviceIDType      eDeviceId,
                             bool32              *pbPLLLocked)
{
  HAL_DP_PhyPllFunctionTable *pFxnTable = &gDPPhyPllFxnTable[eDeviceId];

  if (NULL != pFxnTable->HAL_DP_PHY_GetPLLStatus)
  {
    pFxnTable->HAL_DP_PHY_GetPLLStatus(eDeviceId, pbPLLLocked);
  }
}

/****************************************************************************
*
** FUNCTION: HAL_DP_PHY_SetPowerState()
*/
/*!
* \DESCRIPTION
*           Set the power state for PHY when switching clock source
*
* \param [in]     eDeviceId  - DP Device ID
* \param [in]     bEnabled   - TRUE if the clock source is being set to PLL
*
* \retval
*
****************************************************************************/
void HAL_DP_PHY_SetPowerState(DP_DeviceIDType      eDeviceId,
                              bool32               bEnabled)
{
  HAL_DP_PhyPllFunctionTable *pFxnTable = &gDPPhyPllFxnTable[eDeviceId];

  if (NULL != pFxnTable->HAL_DP_PHY_SetPowerState)
  {
    pFxnTable->HAL_DP_PHY_SetPowerState(eDeviceId, bEnabled);
  }
}

#ifdef __cplusplus
}
#endif

