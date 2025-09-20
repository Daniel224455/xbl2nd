#ifndef _HAL_DP_PHY_QMP_PLL_7NM_H_
#define _HAL_DP_PHY_QMP_PLL_7NM_H_


/* timeout counter values used in iterations of polling PLL & Phy ready status */
#define HAL_DP_PLL_POLL_MAX_READS            100   /*   100 iterations */
#define HAL_DP_PLL_POLL_TIMEOUT             1500   /* ~1500 us */


/*********************************************************************************************
*
** FUNCTION: HAL_DP_PHY_Qmp_Pll_7nm_Config()
*/
/*!
* \brief
*        Configures QMP_PLL for the specified rate.
*
* \param [in]       uQmpPllBase      - Base address of the QMP PLL
* \param [in]       pDPPllConfigInfo - DP PLL configuration
*
* \retval  HAL_MDSS_ErrorType
*
**********************************************************************************************/
HAL_MDSS_ErrorType HAL_DP_PHY_Qmp_Pll_7nm_Config(uintPtr uQmpPllBase, HAL_DP_PllConfigType  *pDPPllConfigInfo);


/*********************************************************************************************
*
** FUNCTION: HAL_DP_TypeC_Phy_Poll_CReady()
*/
/*!
* \brief
*     poll if C_Ready PLL status is ready.
*
* \param [in]       uQmpPllBase      - Base address of the QMP PLL
*
* \retval HAL_DP_StatusType
*
**********************************************************************************************/
HAL_MDSS_ErrorType HAL_DP_PHY_Qmp_Pll_7nm_Poll_Cready(uintPtr uQmpPllBase);


#endif /* _HAL_DP_PHY_QMP_PLL_7NM_H_ */ 

