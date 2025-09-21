#ifndef _HAL_DP_PHY_SETTINGS_1_0_0_H_
#define _HAL_DP_PHY_SETTINGS_1_0_0_H_

/*=============================================================================
 
  File: hal_dp_phy_settings_1_0_0.h
  
  Header file for DP PHY/PLL settings of 1.0.0 hardware 
   
  Copyright (c) 2016-2018 Qualcomm Technologies, Inc.  All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential.
=============================================================================*/


/* 
 * Note: The PHY/PLL settings provided in this file are for 10nm DP Phy specifically.
 */

typedef enum {
  DP_PHY_QSERDES_COM_HSCLK_SEL,
  DP_PHY_QSERDES_COM_DEC_START_MODE0,
  DP_PHY_QSERDES_COM_DIV_FRAC_START1_MODE0,
  DP_PHY_QSERDES_COM_DIV_FRAC_START2_MODE0,
  DP_PHY_QSERDES_COM_DIV_FRAC_START3_MODE0,
  DP_PHY_QSERDES_COM_INTEGLOOP_GAIN0_MODE0,
  DP_PHY_QSERDES_COM_INTEGLOOP_GAIN1_MODE0,
  DP_PHY_QSERDES_COM_VCO_TUNE_MAP,
  DP_PHY_QSERDES_COM_LOCK_CMP1_MODE0,
  DP_PHY_QSERDES_COM_LOCK_CMP2_MODE0,
  DP_PHY_QSERDES_COM_LOCK_CMP3_MODE0,
  DP_PHY_QSERDES_COM_SSC_EN_CENTER,
  DP_PHY_QSERDES_COM_SSC_ADJ_PER1,
  DP_PHY_QSERDES_COM_SSC_PER1,
  DP_PHY_QSERDES_COM_SSC_PER2,
  DP_PHY_QSERDES_COM_SSC_STEP_SIZE1,
  DP_PHY_QSERDES_COM_SSC_STEP_SIZE2,
  DP_PHY_QSERDES_COM_LOCK_CMP_EN,
  DP_PHY_DP_PHY_VCO_DIV,
  DP_PHY_CONFIG_MAX
} DP_PHY_ConfigRegs;

uint32 aDphySettings1620Mbps [DP_PHY_CONFIG_MAX] = 
{
  0x0000000C,    // DP_PHY_QSERDES_COM_HSCLK_SEL, hsclk_divset 8100M divided by 5
  0x00000069,    // DP_PHY_QSERDES_COM_DEC_START_MODE0,
  0x00000000,    // DP_PHY_QSERDES_COM_DIV_FRAC_START1_MODE0,
  0x00000080,    // DP_PHY_QSERDES_COM_DIV_FRAC_START2_MODE0,
  0x00000007,    // DP_PHY_QSERDES_COM_DIV_FRAC_START3_MODE0,
  0x0000003f,    // DP_PHY_QSERDES_COM_INTEGLOOP_GAIN0_MODE0,
  0x00000000,    // DP_PHY_QSERDES_COM_INTEGLOOP_GAIN1_MODE0,
  0x00000000,    // DP_PHY_QSERDES_COM_VCO_TUNE_MAP,
  0x0000006f,    // DP_PHY_QSERDES_COM_LOCK_CMP1_MODE0,
  0x00000008,    // DP_PHY_QSERDES_COM_LOCK_CMP2_MODE0,
  0x00000000,    // DP_PHY_QSERDES_COM_LOCK_CMP3_MODE0,
  0x00000001,    // DP_PHY_QSERDES_COM_SSC_EN_CENTER,
  0x00000000,    // DP_PHY_QSERDES_COM_SSC_ADJ_PER1,
  0x00000036,    // DP_PHY_QSERDES_COM_SSC_PER1,
  0x00000001,    // DP_PHY_QSERDES_COM_SSC_PER2,
  0x00000045,    // DP_PHY_QSERDES_COM_SSC_STEP_SIZE1,
  0x00000006,    // DP_PHY_QSERDES_COM_SSC_STEP_SIZE2,
  0x00000000,    // DP_PHY_QSERDES_COM_LOCK_CMP_EN,
  0x00000001,    // DP_PHY_DP_PHY_VCO_DIV, aux_clk_sel divided by 2
};

uint32 aDphySettings2700Mbps [DP_PHY_CONFIG_MAX] = 
{
  0x00000004,    // DP_PHY_QSERDES_COM_HSCLK_SEL, hsclk_divset 8100M divided by 3
  0x00000069,    // DP_PHY_QSERDES_COM_DEC_START_MODE0,
  0x00000000,    // DP_PHY_QSERDES_COM_DIV_FRAC_START1_MODE0,
  0x00000080,    // DP_PHY_QSERDES_COM_DIV_FRAC_START2_MODE0,
  0x00000007,    // DP_PHY_QSERDES_COM_DIV_FRAC_START3_MODE0,
  0x0000003f,    // DP_PHY_QSERDES_COM_INTEGLOOP_GAIN0_MODE0,
  0x00000000,    // DP_PHY_QSERDES_COM_INTEGLOOP_GAIN1_MODE0,
  0x00000000,    // DP_PHY_QSERDES_COM_VCO_TUNE_MAP,
  0x0000000f,    // DP_PHY_QSERDES_COM_LOCK_CMP1_MODE0,
  0x0000000e,    // DP_PHY_QSERDES_COM_LOCK_CMP2_MODE0,
  0x00000000,    // DP_PHY_QSERDES_COM_LOCK_CMP3_MODE0,
  0x00000001,    // DP_PHY_QSERDES_COM_SSC_EN_CENTER,
  0x00000000,    // DP_PHY_QSERDES_COM_SSC_ADJ_PER1,
  0x00000036,    // DP_PHY_QSERDES_COM_SSC_PER1,
  0x00000001,    // DP_PHY_QSERDES_COM_SSC_PER2,
  0x00000045,    // DP_PHY_QSERDES_COM_SSC_STEP_SIZE1,
  0x00000006,    // DP_PHY_QSERDES_COM_SSC_STEP_SIZE2,
  0x00000000,    // DP_PHY_QSERDES_COM_LOCK_CMP_EN,
  0x00000001,    // DP_PHY_DP_PHY_VCO_DIV, aux_clk_sel divided by 2
};

uint32 aDphySettings5400Mbps[DP_PHY_CONFIG_MAX] =
{
  0x00000000,    // DP_PHY_QSERDES_COM_HSCLK_SEL, hsclk_divset 1080M divided by 2
  0x0000008c,    // DP_PHY_QSERDES_COM_DEC_START_MODE0,
  0x00000000,    // DP_PHY_QSERDES_COM_DIV_FRAC_START1_MODE0,
  0x00000000,    // DP_PHY_QSERDES_COM_DIV_FRAC_START2_MODE0,
  0x0000000a,    // DP_PHY_QSERDES_COM_DIV_FRAC_START3_MODE0,
  0x0000003f,    // DP_PHY_QSERDES_COM_INTEGLOOP_GAIN0_MODE0,
  0x00000000,    // DP_PHY_QSERDES_COM_INTEGLOOP_GAIN1_MODE0,
  0x00000000,    // DP_PHY_QSERDES_COM_VCO_TUNE_MAP,
  0x0000001f,    // DP_PHY_QSERDES_COM_LOCK_CMP1_MODE0,
  0x0000001c,    // DP_PHY_QSERDES_COM_LOCK_CMP2_MODE0,
  0x00000000,    // DP_PHY_QSERDES_COM_LOCK_CMP3_MODE0,
  0x00000001,    // DP_PHY_QSERDES_COM_SSC_EN_CENTER,
  0x00000000,    // DP_PHY_QSERDES_COM_SSC_ADJ_PER1,
  0x00000036,    // DP_PHY_QSERDES_COM_SSC_PER1,
  0x00000001,    // DP_PHY_QSERDES_COM_SSC_PER2,
  0x0000005c,    // DP_PHY_QSERDES_COM_SSC_STEP_SIZE1,
  0x00000008,    // DP_PHY_QSERDES_COM_SSC_STEP_SIZE2,
  0x00000000,    // DP_PHY_QSERDES_COM_LOCK_CMP_EN,
  0x00000002,    // DP_PHY_DP_PHY_VCO_DIV, aux_clk_sel divided by 4 
};

uint32 aDphySettings8100Mbps[DP_PHY_CONFIG_MAX] =
{
  0x00000003,    // DP_PHY_QSERDES_COM_HSCLK_SEL, hsclk_divset 8100M divided by 1
  0x00000069,    // DP_PHY_QSERDES_COM_DEC_START_MODE0,
  0x00000000,    // DP_PHY_QSERDES_COM_DIV_FRAC_START1_MODE0,
  0x00000080,    // DP_PHY_QSERDES_COM_DIV_FRAC_START2_MODE0,
  0x00000007,    // DP_PHY_QSERDES_COM_DIV_FRAC_START3_MODE0,
  0x0000003f,    // DP_PHY_QSERDES_COM_INTEGLOOP_GAIN0_MODE0,
  0x00000000,    // DP_PHY_QSERDES_COM_INTEGLOOP_GAIN1_MODE0,
  0x00000000,    // DP_PHY_QSERDES_COM_VCO_TUNE_MAP,
  0x0000002f,    // DP_PHY_QSERDES_COM_LOCK_CMP1_MODE0,
  0x0000002a,    // DP_PHY_QSERDES_COM_LOCK_CMP2_MODE0,
  0x00000000,    // DP_PHY_QSERDES_COM_LOCK_CMP3_MODE0,
  0x00000001,    // DP_PHY_QSERDES_COM_SSC_EN_CENTER,
  0x00000000,    // DP_PHY_QSERDES_COM_SSC_ADJ_PER1,
  0x00000036,    // DP_PHY_QSERDES_COM_SSC_PER1,
  0x00000001,    // DP_PHY_QSERDES_COM_SSC_PER2,
  0x00000045,    // DP_PHY_QSERDES_COM_SSC_STEP_SIZE1,
  0x00000036,    // DP_PHY_QSERDES_COM_SSC_STEP_SIZE2,
  0x00000008,    // DP_PHY_QSERDES_COM_LOCK_CMP_EN,
  0x00000000,    // DP_PHY_DP_PHY_VCO_DIV, aux_clk_sel divided by 6 
};

typedef struct
{
  uint32              uLinkRatePerLaneMbps;     // Desired link rate  
  uint32              uVcoDividedClkMhz;        // vco_divided_clk
  uint32             *pSettings;                // Recommended settings
}DPPHYHwSettingsType;


static const DPPHYHwSettingsType DPPHYHwSettings[] = {
  { 1620,  810, &aDphySettings1620Mbps[0] },
  { 2700, 1350, &aDphySettings2700Mbps[0] },
  { 5400, 1350, &aDphySettings5400Mbps[0] },
  { 8100, 1350, &aDphySettings8100Mbps[0] },
};

/*  Below are the currently supported settings :

 *----------------------------------------------------------------------------------------
  | linkRate |  target pll vco  hsclk_divsel   clk_divtx   aux_clk_sel   vco_divided_clk |
  |----------|---------------------------------------------------------------------------|
  |  1620    |   8100 Mhz          /5           1620 Mhz         /2            810 Mhz   |
  |----------|---------------------------------------------------------------------------|     
  |  2700    |   8100 Mhz          /3           2700 Mhz         /2           1350 Mhz   |
  |--------------------------------------------------------------------------------------|
  |  5400    |   10800 Mhz         /2           5400 Mhz         /4           1350 Mhz   |
  |--------------------------------------------------------------------------------------|
  |  8100    |   8100 Mhz          /1           8100 Mhz         /6           1350 Mhz   |
  ----------------------------------------------------------------------------------------*/


#define NUMBER_OF_DP_LINK_RATES_SUPPORTED    sizeof(DPPHYHwSettings)/(sizeof(DPPHYHwSettingsType))


#endif /* _HAL_DP_PHY_SETTINGS_1_0_0_H_ */ 

