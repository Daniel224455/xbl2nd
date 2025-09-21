/*=============================================================================
 
  File: hal_dp_phy.c
 
  Source file for DP PHY hardware functionality 
   
  Copyright (c) 2016-2018 Qualcomm Technologies, Inc.  All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential.
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


/*!
* \b HAL_DP_PhyPllFunctionTable
*
* Defines the DP/PLL function table for functions to config PHY and PLL
*/
typedef struct
{
    bool32              (*HAL_DP_PHY_Config)          (HAL_DP_PllConfigType *pDPPhyConfigInfo);    
    void                (*HAL_DP_PHY_Reset)           (void);    
    void                (*HAL_DP_PHY_AUX_Init)        (void);
    HAL_MDSS_ErrorType  (*HAL_DP_PHY_SetupPeSw)       (HAL_DP_PeSwConfigType *pPeSwConfig);
    void                (*HAL_DP_PHY_GetMaxPeSwLevel) (uint32 *uMaxPreemphLvl, uint32 *uMaxVoltSwingLvl);
    HAL_MDSS_ErrorType  (*HAL_DP_PHY_AUXConfig)       (HAL_DP_AuxPhyCfgType *pAuxPhyCfg);
    void                (*HAL_DP_PHY_SetPHYMode)      (bool32 bDPMode);
} HAL_DP_PhyPllFunctionTable;




/* -----------------------------------------------------------------------
** Statics
** ----------------------------------------------------------------------- */

HAL_DP_PhyPllFunctionTable gDPPhyPllFxnTable;


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
  uint32                uRegValue =    in_dword(HWIO_DP_HW_VERSION_ADDR);
  HAL_HW_VersionType    sCoreVersion;   
  
  sCoreVersion.uMajorVersion   = HWIO_GETVAL(DP_HW_VERSION, uRegValue, MAJOR);
  sCoreVersion.uMinorVersion   = HWIO_GETVAL(DP_HW_VERSION, uRegValue, MINOR);
  sCoreVersion.uReleaseVersion = HWIO_GETVAL(DP_HW_VERSION, uRegValue, STEP);
  
  if ((1 == sCoreVersion.uMajorVersion) &&
      (1 == sCoreVersion.uMinorVersion))
  {
    /* Napali (845) */ 
    gDPPhyPllFxnTable.HAL_DP_PHY_Config          = HAL_DP_1_1_0_PHY_Config;    
    gDPPhyPllFxnTable.HAL_DP_PHY_Reset           = HAL_DP_1_1_0_PHY_Reset;
    gDPPhyPllFxnTable.HAL_DP_PHY_AUX_Init        = HAL_DP_1_1_0_PHY_AUX_Init;   
    gDPPhyPllFxnTable.HAL_DP_PHY_SetupPeSw       = HAL_DP_1_1_0_PHY_SetupPeSw;
    gDPPhyPllFxnTable.HAL_DP_PHY_GetMaxPeSwLevel = HAL_DP_1_1_0_PHY_GetMaxPeSwLevel;
    gDPPhyPllFxnTable.HAL_DP_PHY_AUXConfig       = HAL_DP_1_1_0_PHY_AUXConfig;
    gDPPhyPllFxnTable.HAL_DP_PHY_SetPHYMode      = HAL_DP_1_1_0_PHY_SetPHYMode;
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
* \param [in/out]   pPeSwConfig - Pre-emphasis/Swing level settings.
*
* \retval  HAL_MDSS_ErrorType
*
**********************************************************************************************/
HAL_MDSS_ErrorType HAL_DP_SetupPHYPeSwLevel(HAL_DP_PeSwConfigType   *pPeSwConfig)
{
  HAL_MDSS_ErrorType  eStatus = HAL_MDSS_STATUS_SUCCESS;
  
  if (NULL != gDPPhyPllFxnTable.HAL_DP_PHY_SetupPeSw)
  {
    eStatus = gDPPhyPllFxnTable.HAL_DP_PHY_SetupPeSw(pPeSwConfig);
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
* \param [in]   HAL_DP_PllConfigType  - DP PHY PLL config info
*
* \retval boolean  
*
****************************************************************************/
bool32 HAL_DP_PHY_Config(HAL_DP_PllConfigType  *pDPPllConfigInfo)
{
  bool32      bStatus = FALSE;

  if (NULL != gDPPhyPllFxnTable.HAL_DP_PHY_Config)
  {
    bStatus = gDPPhyPllFxnTable.HAL_DP_PHY_Config(pDPPllConfigInfo);
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
* \param [in]   
*
* \retval   
*
****************************************************************************/
void HAL_DP_PHY_Reset(void)
{
  if (NULL != gDPPhyPllFxnTable.HAL_DP_PHY_Reset)
  {
    gDPPhyPllFxnTable.HAL_DP_PHY_Reset();
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
* \param [in]   
*
* \retval   
*
****************************************************************************/
void HAL_DP_PHY_AUX_Init(void)
{
  if (NULL != gDPPhyPllFxnTable.HAL_DP_PHY_AUX_Init)
  {
    gDPPhyPllFxnTable.HAL_DP_PHY_AUX_Init();
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
* \param [out]   uMaxPreemphLvl   -  max Pre-emphasis level supported.
* \param [out]   uMaxVoltSwingLvl -  max voltage swing level supported.
*
* \retval  void
*
**********************************************************************************************/
HAL_MDSS_ErrorType HAL_DP_PHY_GetMaxPeSwLevel(uint32   *uMaxPreemphLvl, 
                                              uint32   *uMaxVoltSwingLvl)
{
  HAL_MDSS_ErrorType  eStatus = HAL_MDSS_STATUS_SUCCESS;
  
  if (NULL != gDPPhyPllFxnTable.HAL_DP_PHY_GetMaxPeSwLevel)
  {
    gDPPhyPllFxnTable.HAL_DP_PHY_GetMaxPeSwLevel(uMaxPreemphLvl, uMaxVoltSwingLvl);
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
* \param [in]     bPowerOn  -  TRUE: power on,  FALSE: power off
*
* \retval   
*
****************************************************************************/
void HAL_DP_PHY_Power(bool32    bPowerOn)
{
  // TODO
}

/****************************************************************************
*
** FUNCTION: HAL_DP_PHY_AUXConfig()
*/
/*!
* \DESCRIPTION
*           Power on/off DP PHY
*
* \param [in]     bPowerOn  -  TRUE: power on,  FALSE: power off
*
* \retval
*
****************************************************************************/
HAL_MDSS_ErrorType HAL_DP_PHY_AUXConfig(HAL_DP_AuxPhyCfgType *pAuxPhyCfg)
{
  HAL_MDSS_ErrorType  eStatus = HAL_MDSS_STATUS_SUCCESS;

  if (NULL != gDPPhyPllFxnTable.HAL_DP_PHY_AUXConfig)
  {
    eStatus = gDPPhyPllFxnTable.HAL_DP_PHY_AUXConfig(pAuxPhyCfg);
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
*           Set the QMP PHY Mode to DP or USB3
*
* \param [in]     bDPMode  -  TRUE: DP mode,  FALSE: USB3 Mode
*
* \retval
*
****************************************************************************/
void HAL_DP_PHY_SetPHYMode(bool32 bDPMode)
{
  if (NULL != gDPPhyPllFxnTable.HAL_DP_PHY_SetPHYMode)
  {
    gDPPhyPllFxnTable.HAL_DP_PHY_SetPHYMode(bDPMode);
  }

}


#ifdef __cplusplus
}
#endif

