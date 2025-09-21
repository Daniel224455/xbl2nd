#ifndef _HAL_DP_PHY_H_
#define _HAL_DP_PHY_H_

/*=============================================================================
 
  File: hal_dp_phy.h
 
  Header file for DP PHY hardware functionality 
   
  Copyright (c) 2016-2018 Qualcomm Technologies, Inc.  All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential.
=============================================================================*/


/*--------------------------------------------------------------------------------------------
 *   Data Type Defines
 *--------------------------------------------------------------------------------------------*/


/*!
 * \struct HAL_DP_PeSwConfigType
 *
 *  Input info to config PHY Voltage Swing level and pre-emphasis level
 *
 */
typedef struct
{
  uint32                            uLaneSwingLevel;                   /**< Lane voltage swing level                  */
  uint32                            uPreemphasisLevel;                 /**< Lane pre-emphasis level                   */
  bool32                            bSwingNotsupported;                /**< requested swing level not supported       */
  bool32                            bPreemphNotSupported;              /**< requested preemphasis level not supported */
} HAL_DP_PeSwConfigType;



/*!
 * \struct _HAL_DP_PllConfigType
 *
 *  Input info to config PLL 
 *
 */
typedef struct
{
   uint32                           uNumLanes;                        /* Number of lanes                        */
   uint32                           uLinkRateMbps;                    /* DP Link rate (per lane) in Mbps        */
   uint32                           uPixelClkMNDInputMhz;             /* input clk freq to pixel clk MND        */
   uint32                           uVoltSwingLevel;                  /* Lane voltage swing level               */
   uint32                           uPreemphasisLevel;                /* Lane pre-emphasis level                */
   bool32                           bInvertedPlug;                    /* Cable plugged in reverse orientation   */
   bool32                           bReversePhyDataPath;              /* PHY data path reverse of type-c spec   */
   uint32                           uFlags;                           /* PLL config options, e.g, SSC mode 
                                                                          see HAL_DP_PllConfigFlagType          */
} HAL_DP_PllConfigType;


/*!
 * \struct HAL_DP_PeSwPairType
 *
 *  pre-emphasis and voltage swing 
 *
 */
typedef struct
{
  uint32 uPreEmphasis;
  uint32 uVoltageSwing;
}HAL_DP_PeSwPairType;

/*!
* \struct HAL_DP_AuxPhyCfgType
*
*  PHY settings for AUX 
*
*/
typedef struct
{
  uint32 *pDPAuxPhyCfg1Index;                                          /* Config Index for DP_PHY_AUX_CFG1 register*/
}HAL_DP_AuxPhyCfgType;


/*--------------------------------------------------------------------------------------------
 *   Function prototypes
 *--------------------------------------------------------------------------------------------*/


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
void HAL_DP_PhyFxnsInit(void);

/****************************************************************************
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
*****************************************************************************/
HAL_MDSS_ErrorType HAL_DP_SetupPHYPeSwLevel(HAL_DP_PeSwConfigType   *pPeSwConfig);

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
bool32 HAL_DP_PHY_Config(HAL_DP_PllConfigType  *pDPPllConfigInfo);

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
void HAL_DP_PHY_Reset(void);

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
void HAL_DP_PHY_AUX_Init(void);

/****************************************************************************

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

*****************************************************************************/
HAL_MDSS_ErrorType HAL_DP_PHY_GetMaxPeSwLevel(uint32   *uMaxPreemphLvl, 
                                              uint32   *uMaxVoltSwingLvl);

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
void HAL_DP_PHY_Power(bool32    bPowerOn);


/****************************************************************************
*
** FUNCTION: HAL_DP_PHY_AUXConfig()
*/
/*!
* \DESCRIPTION
*           Config AUX PHY registers
*
* \param [in]     pAuxPhyCfg  -  AUX PHY settings.
*
* \retval HAL_MDSS_ErrorType
*
****************************************************************************/
HAL_MDSS_ErrorType HAL_DP_PHY_AUXConfig(HAL_DP_AuxPhyCfgType *pAuxPhyCfg);

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
void HAL_DP_PHY_SetPHYMode(bool32 bDPMode);

bool32 HAL_DP_1_0_0_PHY_Config (HAL_DP_PllConfigType  *pDPPllConfigInfo);
void HAL_DP_1_0_0_0_PHY_Reset (void);
void HAL_DP_1_0_0_PHY_AUX_Init (void);
HAL_MDSS_ErrorType HAL_DP_1_0_0_PHY_SetupPeSw (HAL_DP_PeSwConfigType       *pPeSwConfig);
void HAL_DP_1_0_0_PHY_GetMaxPeSwLevel(uint32   *uMaxPreemphLvl, uint32   *uMaxVoltSwingLvl);
HAL_MDSS_ErrorType HAL_DP_1_0_0_PHY_AUXConfig(HAL_DP_AuxPhyCfgType *pAuxPhyCfg);
void HAL_DP_1_0_0_PHY_SetPHYMode(bool32 bDPMode);


bool32 HAL_DP_1_1_0_PHY_Config(HAL_DP_PllConfigType  *pDPPllConfigInfo);
void HAL_DP_1_1_0_PHY_Reset(void);
void HAL_DP_1_1_0_PHY_AUX_Init(void);
HAL_MDSS_ErrorType HAL_DP_1_1_0_PHY_SetupPeSw(HAL_DP_PeSwConfigType       *pPeSwConfig);
void HAL_DP_1_1_0_PHY_GetMaxPeSwLevel(uint32   *uMaxPreemphLvl, uint32   *uMaxVoltSwingLvl);
HAL_MDSS_ErrorType HAL_DP_1_1_0_PHY_AUXConfig(HAL_DP_AuxPhyCfgType *pAuxPhyCfg);
void HAL_DP_1_1_0_PHY_SetPHYMode(bool32 bDPMode);
#endif  /* #define _HAL_DP_PHY_H_ */
