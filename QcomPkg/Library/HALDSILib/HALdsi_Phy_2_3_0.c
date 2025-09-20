/*=============================================================================

  File: HALdsi_Phy_2_0_0.c
  
  

  Copyright (c) 2016-2021 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
=============================================================================*/

/*============================================================================
*                         INCLUDE FILES
============================================================================*/
#ifdef __cplusplus
extern "C" {
#endif

#include "HALdsi.h"
#include "HALdsi_Phy.h"
#include "HALdsi_Pll.h"
#include "dsiHostSystem.h"
#include "HALdsi_Phy_2_3_0.h"

/* -----------------------------------------------------------------------
** Local defines
** ----------------------------------------------------------------------- */
#define HAL_DSI_PHY_REFGEN_TIMEOUT                  10            /*  5 ms */
#define HAL_DSI_PHY_PLL_READY_TIMEOUT               150           /* ~15 ms */

#define HAL_DSI_PHY_PLL_PRE_EMPHASIS_BOTTOM_DEFAULT 0x0
#define HAL_DSI_PHY_PLL_PRE_EMPHASIS_TOP_DEFAULT    0x0

/* Generic parameters that applies to all parameter calculations */
#define HAL_DSI_PHY_TIPX_NUMERATOR                  1000000000.0f /* numerator for the TIPX formula */
#define HAL_DSI_PHY_PERCENT_DENOMENATOR             100.0f
#define HAL_DSI_PHY_TREOT                           20            /* t_reot    */

/* Range factor applied */
#define HAL_DSI_PHY_RANGEFACTOR_1                   1.0f
#define HAL_DSI_PHY_RANGEFACTOR_5                   5.0f

#define HAL_DSI_PHY_RANGEFACTOR_TCLK_PREPARE        50.0f
#define HAL_DSI_PHY_RANGEFACTOR_THS_PREPARE         50.0f
#define HAL_DSI_PHY_RANGEFACTOR_THS_ZERO            10.0f
#define HAL_DSI_PHY_RANGEFACTOR_TCLK_PRE            125.0f
#define HAL_DSI_PHY_RANGEFACTOR_TCLK_POST           5.0f
#define HAL_DSI_PHY_RANGEFACTOR_TCLK_ZERO           2.0f
#define HAL_DSI_PHY_RANGEFACTOR_NOMAX               10.0f
#define HAL_DSI_PHY_RANGEFACTOR_TRAIL               30.0f

#define HAL_DSI_PHY_TREOT                           20

/* Recommended values for TCLK_PREPARE formula */
#define HAL_DSI_PHY_TCLK_PREPARE_PHY_MIN            38.0f
#define HAL_DSI_PHY_TCLK_PREPARE_PHY_MAX            95.0f
#define HAL_DSI_PHY_TCLK_PREPARE_PHY_PARAM1         0x04
#define HAL_DSI_PHY_TCLK_PREPARE_PHY_PARAM2         0x08
#define HAL_DSI_PHY_TCLK_PREPARE_PHY_PARAM3         0x04

/* Recommended Values for TCLK_ZERO formula */
#define HAL_DSI_PHY_TCLK_ZERO_PARAM1                300.0f
#define HAL_DSI_PHY_TCLK_ZERO_PARAM3                8.0f
#define HAL_DSI_PHY_TCLK_ZERO_PARAM4                0x01
#define HAL_DSI_PHY_TCLK_ZERO_PARAM5                255
#define HAL_DSI_PHY_TCLK_ZERO_PARAM6                511

/* Recommended Values for TCLK_TRAIL formula */
#define HAL_DSI_PHY_TCLK_TRAIL_MIN                  60.0f

/* Recommended Values for T_HS_PREPARE formula */
#define HAL_DSI_PHY_T_HS_PREPARE_PARAM1             0x28
#define HAL_DSI_PHY_T_HS_PREPARE_PARAM2             0x04
#define HAL_DSI_PHY_T_HS_PREPARE_PARAM3             0x55
#define HAL_DSI_PHY_T_HS_PREPARE_PARAM4             0x06
#define HAL_DSI_PHY_T_HS_PREPARE_PARAM5             0x04
#define HAL_DSI_PHY_T_HS_PREPARE_PARAM6             0x08

/* Recommended Values for T_HS_ZERO formula */
#define HAL_DSI_PHY_T_HS_ZERO_PARAM1                0x91
#define HAL_DSI_PHY_T_HS_ZERO_PARAM2                0x0A
#define HAL_DSI_PHY_T_HS_ZERO_PARAM3                0x0B
#define HAL_DSI_PHY_T_HS_ZERO_PARAM4                0x08
#define HAL_DSI_PHY_T_HS_ZERO_PARAM5                0x01
#define HAL_DSI_PHY_T_HS_ZERO_PARAM6                0xFF

/* Recommended Values for T_XX_TRAIL formula */
#define HAL_DSI_PHY_T_T_TRAIL_PARAM1                0x3C
#define HAL_DSI_PHY_T_T_TRAIL_PARAM2                0x04
#define HAL_DSI_PHY_T_T_TRAIL_PARAM3                0x03
#define HAL_DSI_PHY_T_T_TRAIL_PARAM4                0x08
#define HAL_DSI_PHY_T_T_TRAIL_PARAM5                0x01

/* Recommended Values for TEOT formula */
#define HAL_DSI_PHY_TEOT_PARAM1                     0x69
#define HAL_DSI_PHY_TEOT_PARAM2                     0x0C

/* Recommended Values for T_HS_EXIT formula */
#define HAL_DSI_PHY_T_HS_EXIT_MIN                   100.0f
#define HAL_DSI_PHY_T_HS_EXIT_MAX                   255
#define HAL_DSI_PHY_T_HS_EXIT_PARAM1                0x08
#define HAL_DSI_PHY_T_HS_EXIT_PARAM2                0x01

/* Recommended Values for T_HS_RQST formula */
#define HAL_DSI_PHY_T_HS_RQST_PARAM1                50
#define HAL_DSI_PHY_T_HS_RQST_PARAM2                4
#define HAL_DSI_PHY_T_HS_RQST_PARAM3                8

/* Recommended Values for T_TA_GO formula */
#define HAL_DSI_PHY_T_TA_GO_PARAM1                  0x02

/* Recommended Values for T_TA_SURE formula */
#define HAL_DSI_PHY_T_TA_SURE_PARAM1                0x00

/* Recommended Values for T_TA_GET formula */
#define HAL_DSI_PHY_T_TA_GET_PARAM1                 0x04

/* Recommended Values for T_CLK_PRE formula */
#define HAL_DSI_PHY_T_CLK_PRE_PARAM1                0x08
#define HAL_DSI_PHY_T_CLK_PRE_PARAM2                0x10
#define HAL_DSI_PHY_T_CLK_PRE_PARAM3                0x02
#define HAL_DSI_PHY_T_CLK_PRE_PARAM4                0x01
#define HAL_DSI_PHY_T_CLK_PRE_MAX                   0xFF

/* Recommended Values for T_CLK_POST formula */
#define HAL_DSI_PHY_T_CLK_POST_PARAM1               0x3C
#define HAL_DSI_PHY_T_CLK_POST_PARAM2               0x34
#define HAL_DSI_PHY_T_CLK_POST_PARAM3               0x2B
#define HAL_DSI_PHY_T_CLK_POST_PARAM4               0x10
#define HAL_DSI_PHY_T_CLK_POST_PARAM5               0x01
#define HAL_DSI_PHY_T_CLK_POST_MAX                  0xFF

/* Recommended Values for PLL */
#define HAL_DSI_PLL_REF_FREQ_MHZ                    19.2f         /* 19.2 Mhz Reference Clock */
#define HAL_DSI_PLL_PRESCALE_FACTOR                 2.0f          /* Prescaler */
#define HAL_DSI_PLL_VCO_CONFIG_FREQ_MAX             2990          /* Frequency above which VCO Config will be 1 */

/* Constants Used in calculating Clock Invertor */
#define HAL_DSI_PLL_CLOCK_INVERTORS_FREQ_LEVEL_0    900
#define HAL_DSI_PLL_CLOCK_INVERTORS_FREQ_LEVEL_1    1100
#define HAL_DSI_PLL_CLOCK_INVERTORS_FREQ_LEVEL_2    1400
#define HAL_DSI_PLL_CLOCK_INVERTORS_FREQ_LEVEL_3    3020

#define HAL_DSI_PLL_CLOCK_INVERTORS_VALUE_0         0x18
#define HAL_DSI_PLL_CLOCK_INVERTORS_VALUE_1         0x10
#define HAL_DSI_PLL_CLOCK_INVERTORS_VALUE_2         0x08
#define HAL_DSI_PLL_CLOCK_INVERTORS_VALUE_3         0x00
#define HAL_DSI_PLL_CLOCK_INVERTORS_VALUE_4         0x40

#define HAL_DSI_PLL_SSC_FREQ_KHZ                    31.5f         /* 31.5 Khz */
#define HAL_DSI_PLL_SSC_OFFSET_PPM                  5000          /* 5000 ppm */
#define HAL_DSI_PLL_SSC_ADJ_PER                     2             /* Reference cycles per SSC step */
#define HAL_DSI_PLL_LOCK_TIMER                      0x40          /* 0-255 for Qlink */
#define HAL_DSI_PLL_FRAC_BITS                       18            /* Number of bits for fraction Qlink-18bits */
#define HAL_DSI_PLL_THRESHOLD_CYCLES                32            /* 0 - 255 */
#define HAL_DSI_PLL_REFCLK_CYCLES                   256           /* 1 - 1024 */
#define HAL_DSI_PLL_SYSTEM_MUXES                    0xD0
#define HAL_DSI_PLL_PLL_LOCK_DELAY                  0x06

#define HAL_DSI_PLL_PARAM_16                        16
#define HAL_DSI_PLL_PARAM_10                        10
#define HAL_DSI_PLL_PARAM_8                         8
#define HAL_DSI_PLL_PARAM_2                         2
#define HAL_DSI_PLL_PARAM_0_2F                      0.2f
#define HAL_DSI_PLL_PARAM_1                         1
#define HAL_DSI_PLL_PARAM_1900F                     1900.0f
#define HAL_DSI_PLL_PARAM_3000F                     3000.0f
#define HAL_DSI_PLL_PARAM_15                        15
#define HAL_DSI_PLL_PARAM_1M                        1000000
#define HAL_DSI_PLL_PARAM_500F                      500.0f
#define HAL_DSI_PLL_OUTDIV_LUT_COUNT                19
#define HAL_DSI_PLL_VCO_MIN_MHZ                     600.0f

#define HAL_DSI_LANE_TX_DCTRL_1                     0x00           /* TX Dctrl for Lanes 0, 1 & 2 */

/* lane3_tx_dctl: split = 4 otherwise = 0 */
#define HAL_DSI_LANE_TX_DCTRL_2                     0x04           /* TX Dctrl for Lane 3 */
#define HAL_DSI_LANE_TX_DCTRL_3                     0x01           /* TX Dctrl for CLK Lane */


/* C-Phy Parameters */
#define HAL_DSI_CPHY_HALFBYTECLK_EN_THRESHOLD       80000000
#define HAL_DSI_CPHY_T_HS_EXIT_MIN                  100.0f
#define HAL_DSI_CPHY_T_HS_EXIT_MAX                  255.0f
#define HAL_DSI_CPHY_T3_PREPARE_MIN                 38
#define HAL_DSI_CPHY_T3_PREPARE_MAX                 95
#define HAL_DSI_CPHY_T3_PREBEGIN_CNT_MIN            7
#define HAL_DSI_CPHY_T3_PREBEGIN_CNT_MAX            448
#define HAL_DSI_CPHY_T3_POST_CNT_MIN                7
#define HAL_DSI_CPHY_T3_POST_CNT_MAX                224
#define HAL_DSI_CPHY_T_HS_RQST_MIN                  50.0f
#define HAL_DSI_CPHY_BITS_SYMBOL                    3
#define HAL_DSI_CPHY_BITS_BYTE                      7
#define HAL_DSI_CPHY_RANGEFACTOR_0                  0.0f

#ifdef DSI_LOG_PRINTOUT 
#define DSI_WR(_a_,_d_) { DSI_Log_Message(DSI_LOGLEVEL_ERROR, "DSI_REG_WRITE\t0x%08X\t0x%08X\n", (_a_), (_d_)); out_dword((_a_),(_d_)); }
#else
#define DSI_WR(_a_,_d_) out_dword((_a_),(_d_))
#endif

/*!
 * \struct _HAL_DSI_PHY_LaneConfigType_
 *
 * PHY configuration parameters
 */
typedef struct _HAL_DSI_PHY_LaneConfigType_
{
  uint32           uDataStrengthLP_P;         /* Data strength LP_P */
  uint32           uDataStrengthLP_N;         /* Data strength LP_N */
  uint32           uDataStrengthHS;           /* Data strength HS */
  uint32           uClockStrengthHS;          /* Clock Strength HS */
  uint32           uPemphBottom;              /* Determines how many pre-emphasis branches for bottom termination */
  uint32           uPemphTop;                 /* Determines how many pre-emphasis branches for top termination */
  bool32           bPemphEnable;              /* Enable driver pre-emphasis */
  bool32           bStrengthOverride;         /* Strength override to use DSIPHY_LNn_TEST_STR */
} HAL_DSI_PHY_LaneConfigType;


/*!
 * \struct _HAL_DSI_PHY_TimingConfigType_
 *
 * PHY timing parameters
 */
typedef struct _HAL_DSI_PHY_TimingConfigType_
{
  bool32           bHSTX_HalfByteClk_En;      /* DSI timing parameter: HSTX_HALFBYTECLK_EN */
  uint32           uT_CLK_ZERO;               /* DSI timing parameter: t_CLK_ZERO */
  uint32           uT_CLK_PREPARE;            /* DSI timing parameter: t_CLK_PREPARE */
  uint32           uT_CLK_TRAIL;              /* DSI timing parameter: t_CLK_TRAIL */
  uint32           uT_HS_EXIT;                /* DSI timing parameter: t_HS_EXIT */
  uint32           uT_HS_ZERO;                /* DSI timing parameter: t_HS_ZERO */
  uint32           uT_HS_PREPARE;             /* DSI timing parameter: t_HS_PREPARE */
  uint32           uT_HS_TRAIL;               /* DSI timing parameter: t_HS_TRAIL */
  uint32           uT_HS_RQST;                /* DSI timing parameter: t_HS_RQST */
  uint32           uT_TA_SURE;                /* DSI timing parameter: t_TA_SURE */
  uint32           uT_TA_GO;                  /* DSI timing parameter: t_TA_GO */
  uint32           uT_TA_GET;                 /* DSI timing parameter: t_TA_GET */
  uint32           uT_CLK_PRE;                /* DSI timing parameter: t_CLK_PRE */
  uint32           uT_CLK_POST;               /* DSI timing parameter: t_CLK_POST */
  bool32           bT_CLK_PRE_INC_BY_2;       /* DSI timing parameter: t_CLK_PRE_INC_BY_2 */
} HAL_DSI_PHY_TimingConfigType;

/*!
 * \struct _HAL_DSI_PHY_PhySettingsType
 *
 * PHY configuration parameters
 */
typedef struct _HAL_DSI_PHY_PhySettingsType
{
  uint32                       uDivCtrl;
  uint32                       uPhyPostDiv;
  bool32                       bSlave;
  HAL_DSI_PHY_TimingConfigType sTimingConfig;
  HAL_DSI_PHY_LaneConfigType   sLaneConfig;
  bool32                       bCPhyMode;
  uint32                       uDesiredBitClkFreq;
} HAL_DSI_PHY_PhySettingsType;

/*!
 * \struct _HAL_DSI_PHY_PllConfigType
 *
 * PLL configuration parameters
 */
typedef struct _HAL_DSI_PHY_PllConfigType
{
  uint32                       uDecDivStart;   /* Freq divider Decimal    value */
  uint32                       uFracDivLow;    /* Freq divider Fractional part  bits 7:0   */
  uint32                       uFracDivMid;    /* Freq divider Fractional part  bits 15:8  */
  uint32                       uFracDivHigh;   /* Freq divider Fractional part  bits 17:16 */
  uint32                       uPllPostDiv;    /* PLL Post divider ratio */
  uint32                       uPropGainRate;
  uint32                       uLockDetRate;
  uint32                       uFreqDetThresh;
  uint32                       uFreqDetRefclk;
  uint32                       uFreqDetPllclk;
  uint32                       uSSCPER;
  uint32                       uSSCAdjPER;
  uint32                       uSSCStepSize;
  uint32                       uLockDelay;
  uint32                       uClockInverters;
  float                        fPllErrorPPM;
  float                        fActualFreqMHz;
} HAL_DSI_PHY_PllConfigType;

/*!
 * \struct _HAL_DSI_PHY_PLL_KVCO_RANGE_TYPE_
 *
 * DSI KVCO range settings
 */
typedef struct _HAL_DSI_PHY_PLL_KVCO_RANGE_TYPE_
{
  uint32           uLow;                      /* KVCO lower bound  */
  uint32           uHigh;                     /* KVCO higher bound */
  uint32           uKvcoSlope;                /* KVCO slope        */
  uint32           uKvcoCode;                 /* KVCO code         */
} HAL_DSI_PHY_PLL_KVCORangeType;

/*!
 * \struct HAL_DSI_PHY_DividerLUTEntryType
 *
 * Divider LUT entry
 */
typedef struct _HAL_DSI_PHY_DividerLUTEntryType_
{
  uint16        uPllPostDiv;
  uint16        uPhyPostDiv;
} HAL_DSI_PHY_DividerLUTEntryType;

/* PLL divider LUTs, see 7nm hgp section 2.4.1.1 */
static HAL_DSI_PHY_DividerLUTEntryType asPllDividerLUT_DPhy[] = {
  { 2, 5 },     /* 10 */
  { 1, 9 },     /* 9 */
  { 8, 1 },     /* 8 */
  { 1, 7 },     /* 7 */
  { 2, 3 },     /* 6 */
  { 1, 5 },     /* 5 */
  { 4, 1 },     /* 4 */
  { 1, 3 },     /* 3 */
  { 2, 1 },     /* 2 */
  { 1, 1 }      /* 1 */
};

static HAL_DSI_PHY_DividerLUTEntryType asPllDividerLUT_CPhy[] = {
  { 2, 5 },     /* 10 */
  { 8, 1 },     /* 8 */
  { 2, 3 },     /* 6 */
  { 1, 5 },     /* 5 */
  { 4, 1 },     /* 4 */
  { 1, 3 },     /* 3 */
  { 2, 1 },     /* 2 */
  { 1, 1 }      /* 1 */
};

/****************************************************************************
*
** FUNCTION: HAL_DSI_Phy_GetLaneOffset()
*/
/*!
* \brief
*     Gets register offset for the specific Phy Lane.
*
*
* \param [in]  eLainId - Lane id;
*
* \retval  Register offset value for the specified lane;
*
****************************************************************************/
static uint32 HAL_DSI_Phy_Pll_Minor(DSI_Device_IDType eDeviceId)
{
  uintPtr  uOffset    = HAL_DSI_GetRegBaseOffset(eDeviceId);
  uint32   uMinor     = in_dword(uOffset + HWIO_DSI_0_PHY_DSIPHY_CMN_REVISION_ID0_ADDR);

  uMinor = HWIO_GETVAL(DSI_0_PHY_DSIPHY_CMN_REVISION_ID0, uMinor, MINOR_3_0);

  return uMinor;
}

/****************************************************************************
*
** FUNCTION: HAL_DSI_Phy_GetLaneOffset()
*/
/*!
* \brief
*     Gets register offset for the specific Phy Lane.
*
*
* \param [in]  eLainId - Lane id;
*
* \retval  Register offset value for the specified lane;
*
****************************************************************************/
static uintPtr HAL_DSI_Phy_GetLaneOffset(DSI_LaneIDType eLaneId)
{
  uintPtr uLaneBlockOffset[DSI_LaneID_Max];
  uintPtr uOffset                           = 0;

  uLaneBlockOffset[DSI_LaneID_0]   = 0;
  uLaneBlockOffset[DSI_LaneID_1]   = HWIO_DSI_0_PHY_DSIPHY_DLN1_CFG0_ADDR - HWIO_DSI_0_PHY_DSIPHY_DLN0_CFG0_ADDR;
  uLaneBlockOffset[DSI_LaneID_2]   = HWIO_DSI_0_PHY_DSIPHY_DLN2_CFG0_ADDR - HWIO_DSI_0_PHY_DSIPHY_DLN0_CFG0_ADDR;
  uLaneBlockOffset[DSI_LaneID_3]   = HWIO_DSI_0_PHY_DSIPHY_DLN3_CFG0_ADDR - HWIO_DSI_0_PHY_DSIPHY_DLN0_CFG0_ADDR;
  uLaneBlockOffset[DSI_LaneID_CLK] = HWIO_DSI_0_PHY_DSIPHY_CKLN_CFG0_ADDR - HWIO_DSI_0_PHY_DSIPHY_DLN0_CFG0_ADDR;

  if (eLaneId < DSI_LaneID_Max)
  {
    uOffset = uLaneBlockOffset[eLaneId];
  }
  return uOffset;
}


/****************************************************************************
*
** FUNCTION: HAL_DSI_Phy_GetTxDctrl()
*/
/*!
* \brief
*     Gets Tx DCTRL value for the specific Phy Lane.
*
*
* \param [in]  eLainId - Lane id;
*
* \retval  Tx Dctrl value
*
****************************************************************************/
static uint32 HAL_DSI_Phy_GetTxDctrl(DSI_LaneIDType eLaneId)
{
  uint32 uTxDctrl = 0;

  switch (eLaneId)
  {
  case DSI_LaneID_0:
  case DSI_LaneID_1:
  case DSI_LaneID_2:
    uTxDctrl = HAL_DSI_LANE_TX_DCTRL_1;
    break;
  case DSI_LaneID_3:
    uTxDctrl = HAL_DSI_LANE_TX_DCTRL_2;
    break;
  case DSI_LaneID_CLK:
    uTxDctrl = HAL_DSI_LANE_TX_DCTRL_3;
    break;
  default:
    break;
  }
  return uTxDctrl;

}

/****************************************************************************
*
** FUNCTION: HAL_DSI_Phy_CheckNegative()
*/
/*!
* \brief
*     If the integer is negative, set it to zero
*
*
* \param [in]  iVal - integer value to be processed;
*
* \retval  0 or positive value as integer;
*
****************************************************************************/
static int32 HAL_DSI_Phy_CheckNegative(int32 iVal)
{
  if (iVal < 0)
  {
    iVal = 0;
  }
  return iVal;
}

/****************************************************************************
*
** FUNCTION: HAL_DSI_Phy_Round()
*/
/*!
* \brief
*     Rounds to the nearest integer.
*
*
* \param [in]  fFloatVal - float number to be processed;
*
* \retval  rounded integer;
*
****************************************************************************/
static int32 HAL_DSI_Phy_Round(float fFloatVal)
{
  int32  iRoundupVal = (int32)fFloatVal;
  if (fFloatVal - iRoundupVal > 0.5) iRoundupVal++;

  return iRoundupVal;
}


/****************************************************************************
*
** FUNCTION: HAL_DSI_Phy_Rounddown()
*/
/*!
* \brief
*     Rounds down to the nearest integer for positive number, 0 if it is negative number.
*
*
* \param [in]  fFloatVal - float number to be processed;
*
* \retval  rounded down integer;
*
****************************************************************************/
static uint32 HAL_DSI_Phy_Rounddown(double pFloatValue)
{
  uint32     roundupValue;

  if (pFloatValue > 0)
  {
    roundupValue = (uint32)pFloatValue;
  }
  else
  {
    roundupValue = 0;
  }

  return roundupValue;
}

/****************************************************************************
*
** FUNCTION: HAL_DSI_Phy_PllEnable()
*/
/*!
* \brief
*     Enable/Disable Phy PLL
*
* \param [in]  eDeviceId         - DSI core ID
* \param [in]  bEnable           - Enable/disable PLL
*
* \retval HAL_MDSS_ErrorType
*
****************************************************************************/
static void HAL_DSI_Phy_PllEnable(uintPtr  uOffset, bool32 bEnable)
{
  uint32               uRegVal = 0;

  if (bEnable == TRUE)
  {
     uRegVal = HWIO_OUT_FLD(uRegVal, DSI_0_PHY_DSIPHY_CMN_PLL_CNTRL, PLL_START, 1);
  }
  else
  {
     uRegVal = HWIO_OUT_FLD(uRegVal, DSI_0_PHY_DSIPHY_CMN_PLL_CNTRL, PLL_START, 0);
  }
  DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_CMN_PLL_CNTRL_ADDR, uRegVal);
}

/****************************************************************************
*
** FUNCTION: HAL_DSI_Phy_PllBiasEnable()
*/
/*!
* \brief
*     Enable/Disable PLL Bias
*
* \param [in]  eDeviceId         - DSI core ID
* \param [in]  bEnable           - Enable/disable PLL Bias
*
* \retval void
*
****************************************************************************/
static void HAL_DSI_Phy_PllBiasEnable(DSI_Device_IDType eDeviceId, bool32 bEnable)
{
  uintPtr            uOffset    = HAL_DSI_GetRegBaseOffset(eDeviceId);
  uint32             uRegVal    = 0;

  if (bEnable)
  {
    /* Set BIAS_EN_MUX, BIAS_EN */
    uRegVal = HWIO_OUT_FLD(uRegVal, DSI_0_PHY_DSIPHY_PLL_SYSTEM_MUXES, CM_BIAS_EN_MUX, 1);
    uRegVal = HWIO_OUT_FLD(uRegVal, DSI_0_PHY_DSIPHY_PLL_SYSTEM_MUXES, CM_BIAS_EN, 1);
  }
  
  /* pll system muxes */
  DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_PLL_SYSTEM_MUXES_ADDR, uRegVal);
  
}


/****************************************************************************
*
** FUNCTION: HAL_DSI_Phy_PllGlobalClockEnable()
*/
/*!
* \brief
*     Enable/Disable PLL clock

*
* \param [in]  eDeviceId         - DSI core ID
* \param [in]  bEnable           - Enable/disable Global Clk
*
* \retval void
*
****************************************************************************/

static void HAL_DSI_Phy_PllGlobalClockEnable(DSI_Device_IDType eDeviceId, bool32 bEnable)
{
  uintPtr            uOffset    = HAL_DSI_GetRegBaseOffset(eDeviceId);
  uint32             uClkCfg    = in_dword(uOffset + HWIO_DSI_0_PHY_DSIPHY_CMN_CLK_CFG1_ADDR);
  uint32             uClkEnable = 0;

  /* Set CLK_EN */
  if (bEnable)
  {
    uClkEnable = 1;
  }
  
  uClkCfg = HWIO_OUT_FLD(uClkCfg, DSI_0_PHY_DSIPHY_CMN_CLK_CFG1, CLK_EN, uClkEnable);

  /* clk cfg1 */
  DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_CMN_CLK_CFG1_ADDR, uClkCfg);
}

/****************************************************************************
*
** FUNCTION: HAL_DSI_Phy_TriggerSleepB()
*/
/*!
* \brief
*    Trigger SLEEP_B signal

*
* \param [in]  eDeviceId         - DSI core ID
* \param [in]  bEnable           - Enable/disable Global Clk
*
* \retval void
*
****************************************************************************/

static void HAL_DSI_Phy_TriggerSleepB(DSI_Device_IDType eDeviceId, bool32 bTrigger)
{
  uintPtr            uOffset    = HAL_DSI_GetRegBaseOffset(eDeviceId);
  uint32             uCmnCtrl   = in_dword(uOffset + HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_3_ADDR);
  
  uCmnCtrl = HWIO_OUT_FLD(uCmnCtrl, DSI_0_PHY_DSIPHY_CMN_CTRL_3, SLEEP_B, bTrigger);

  /* SLEEP_B */
  DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_3_ADDR, uCmnCtrl);
}

/****************************************************************************
*
** FUNCTION: HAL_DSI_Phy_PllSetSource()
*/
/*!
* \brief
*     Set byte clock source for PLL
*
* \param [in]  psDsiPhyConfig         - PHY Configruation 
*
* \retval void
*
****************************************************************************/
static void HAL_DSI_Phy_PllSetSource(HAL_DSI_PhyConfigType       *psDsiPhyConfig )
{
  uintPtr            uOffset    = HAL_DSI_GetRegBaseOffset(psDsiPhyConfig->eDeviceId);
  uint32             uClkCfg    = in_dword(uOffset + HWIO_DSI_0_PHY_DSIPHY_CMN_CLK_CFG1_ADDR);

  if ((HAL_DSI_PLL_CONFIG_SPLIT_SOURCE == psDsiPhyConfig->ePLLConfigSource) &&
      (DSI_DeviceID_1 == psDsiPhyConfig->eDeviceId))
  {
    uClkCfg = HWIO_OUT_FLD(uClkCfg, DSI_0_PHY_DSIPHY_CMN_CLK_CFG1, BITCLK_SEL, 1);
  }
  else
  {
    uClkCfg = HWIO_OUT_FLD(uClkCfg, DSI_0_PHY_DSIPHY_CMN_CLK_CFG1, DSICLK_SEL, psDsiPhyConfig->uDsiClkSel);
  }

  /* clk cfg1 */
  DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_CMN_CLK_CFG1_ADDR, uClkCfg);

}

/****************************************************************************
*
** FUNCTION: HAL_DSI_Phy_WaitForRefgen()
*/
/*!
* \brief
*     Check Phy status and wait for refgen
*
* \param [in]  eDeviceId - DSI core ID
*
* \retval HAL_MDSS_ErrorType
*
****************************************************************************/
static HAL_MDSS_ErrorType HAL_DSI_Phy_WaitForRefgen(DSI_Device_IDType eDeviceId)
{
  HAL_MDSS_ErrorType eStatus     = HAL_MDSS_STATUS_SUCCESS;
#ifndef MDP_PRESIL //#TODO# - No Phy on RUMI - enable on silicon
  uintPtr            uOffset     = HAL_DSI_GetRegBaseOffset(eDeviceId);
  uint32             uTimeout    = HAL_DSI_PHY_REFGEN_TIMEOUT;
  uint32             uRefgen     = 0;

  while (0 == uRefgen)
  {
    /* Wait  for 500us before checking status register */
    DSI_OSAL_SleepUs(500);
    
    uRefgen = (in_dword(uOffset + HWIO_DSI_0_PHY_DSIPHY_CMN_PHY_STATUS_ADDR) &
                  HWIO_DSI_0_PHY_DSIPHY_CMN_PHY_STATUS_REFGEN_READY_BMSK);
    
    uTimeout--;
         
    if ((0 == uTimeout) &&
        (0 == uRefgen))
    {
      /* Timeout while polling the lock status */
      eStatus = HAL_MDSS_DSI_FAILED_UNABLE_TO_INIT_HW;
      
      DSI_Log_Message(DSI_LOGLEVEL_WARN, "Wait for Refgen timed out on DSI %d after %d microsecs\n", eDeviceId,
             HAL_DSI_PHY_REFGEN_TIMEOUT * 500);
      break;
    }
  }
#endif
  return eStatus;
}


/****************************************************************************
*
** FUNCTION: HAL_DSI_Phy_PowerUp()
*/
/*!
* \brief
*     Power up the PHY
*
* \param [in]  eDeviceId - DSI core ID
* \param [in]  pSettings - pointer to phy settings structure
*
* \retval None
*
****************************************************************************/
static void HAL_DSI_Phy_PowerUp(DSI_Device_IDType eDeviceId,
                                  HAL_DSI_PHY_PhySettingsType  *pSettings)
{
  uintPtr            uOffset         = HAL_DSI_GetRegBaseOffset(eDeviceId);
  uint32             uRescodeTopCtrl = 0;
  uint32             uRescodeBotCtrl = 0;
  uint32             uVRegCtrl0      = 0;

  /* initial_init */

  /* Deassert sleep_b */
  DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_3_ADDR, 0x00000000);
  
  /* Power up DIGTOP & PLL */
  DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_0_ADDR, 0x00000020);
  DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_0_ADDR, 0x00000060);

  /* Disable PLL */
  HAL_DSI_Phy_PllEnable(uOffset, FALSE);

  /* Resync re-time FIFO OFF*/
  DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_CMN_RBUF_CTRL_ADDR, 0x0);

  if (pSettings->uDesiredBitClkFreq <= 1500000000)
  {
    uRescodeTopCtrl = 0x03;
    uRescodeBotCtrl = 0x3C;
    uVRegCtrl0      = 0x5A;
  }
  else
  {
    uRescodeTopCtrl = 0x00;
    uRescodeBotCtrl = 0x3C;
    uVRegCtrl0      = 0x52;
  }

  /* dsi_cmn_init start */
  
  /* VREG_CTRL_0P4 */
  DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_CMN_VREG_CTRL_0_ADDR,                   uVRegCtrl0);

  /* VREG_CTRL_0P2 */
  DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_CMN_VREG_CTRL_1_ADDR,                   0x00000019);

  DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_3_ADDR,                        0x00000000);
  DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_STR_SWI_CAL_SEL_CTRL_ADDR,     0x00000000);
  DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_HSTX_STR_CTRL_0_ADDR,          0x00000088);
  DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_PEMPH_CTRL_0_ADDR,             0x00000000);
  DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_RESCODE_OFFSET_TOP_CTRL_ADDR,  uRescodeTopCtrl);
  DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_RESCODE_OFFSET_MID_CTRL_ADDR,  0x00000000);
  DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_RESCODE_OFFSET_BOT_CTRL_ADDR,  uRescodeBotCtrl);
  DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_LPTX_STR_CTRL_ADDR,            0x00000055);

  /* dsi_cmn_init end */

  /* Remove PLL, DIG and all lanes from pwrdn */
  DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_0_ADDR, 0x0000007F);

  /* Lane selection */
  if (pSettings->bCPhyMode)
  {
    DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL0_ADDR, 0x00000007);
  }
  else
  {
    DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL0_ADDR, 0x0000001F);
  }
}

/****************************************************************************
*
** FUNCTION: HAL_DSI_Phy_ResyncFIFO()
*/
/*!
* \brief
*     Resync FIFO on DSI Phy
*
* \param [in]  eDeviceId - DSI core ID
*
* \retval None
*
****************************************************************************/

static void HAL_DSI_Phy_ResyncFIFO(DSI_Device_IDType eDeviceId)
{
  uintPtr            uOffset     = HAL_DSI_GetRegBaseOffset(eDeviceId);

  /* Resync FIFO*/
  DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_CMN_RBUF_CTRL_ADDR, 0x00000000);

  /* Resync FIFO*/
  DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_CMN_RBUF_CTRL_ADDR, 0x00000001);
}

/****************************************************************************
*
** FUNCTION: HAL_DSI_Phy_PllLockDetect()
*/
/*!
* \brief
*     Wait and check if PLL locked.
*
* \param [in]  eDeviceId - DSI core ID
*
* \retval HAL_MDSS_ErrorType
*
****************************************************************************/
static HAL_MDSS_ErrorType HAL_DSI_Phy_PllLockDetect(DSI_Device_IDType eDeviceId)
{
  HAL_MDSS_ErrorType eStatus     = HAL_MDSS_STATUS_SUCCESS;
  uintPtr            uOffset     = HAL_DSI_GetRegBaseOffset(eDeviceId);
  uint32             uTimeout    = HAL_DSI_PHY_PLL_READY_TIMEOUT;
  uint32             uPllStatus;

  /* Enable PLL */
  HAL_DSI_Phy_PllEnable(uOffset, TRUE);


  /* Wait for Lock */
  uPllStatus = (in_dword(uOffset + HWIO_DSI_0_PHY_DSIPHY_PLL_COMMON_STATUS_ONE_ADDR) &
                   HWIO_DSI_0_PHY_DSIPHY_PLL_COMMON_STATUS_ONE_CORE_READY_STATUS_BMSK);
#ifdef MDP_PRESIL //#TODO# - No Phy on RUMI - enable on silicon
  (void) uTimeout;
#else
  while ((0 == uPllStatus) && (uTimeout))
  {
    DSI_OSAL_SleepUs(100);  /*delay ~100us*/
    uPllStatus = (in_dword(uOffset + HWIO_DSI_0_PHY_DSIPHY_PLL_COMMON_STATUS_ONE_ADDR) &
                     HWIO_DSI_0_PHY_DSIPHY_PLL_COMMON_STATUS_ONE_CORE_READY_STATUS_BMSK);
    uTimeout--;
  }

  if (0 == (uPllStatus & HWIO_DSI_0_PHY_DSIPHY_PLL_COMMON_STATUS_ONE_CORE_READY_STATUS_BMSK))
  {
    /* timeout while polling the lock status */
    eStatus = HAL_MDSS_DSI_FAILED_UNABLE_TO_INIT_HW;
    DSI_Log_Message(MDP_LOGLEVEL_ERROR, "DSI_Pll_Lock: Failed\n");
  }
#endif
  return eStatus;
}

/****************************************************************************
*
** FUNCTION: HAL_DSI_Phy_CalculateTimingParams()
*/
/*!
* \brief
*     Calculate timing parameters
*
* \param [in]  pTimingParameters - DSI core ID
*
* \retval HAL_MDSS_ErrorType
*
****************************************************************************/
static HAL_MDSS_ErrorType HAL_DSI_Phy_CalculateTimingParams(HAL_DSI_TimingSettingType *pTimingParameters,
                                                            HAL_DSI_PHY_TimingConfigType *psTimingConfig)
{
  HAL_MDSS_ErrorType     eStatus                = HAL_MDSS_STATUS_SUCCESS;
  DSI_TimingOverrideType *pPreDefinedTimings    = pTimingParameters->pPreDefinedTimings;
  float                  fMin;
  float                  fMax;
  float                  fT_clk_prepare_actual;
  float                  fT_clk_zero_actual;
  float                  fT_hs_prepare_actual;
  float                  fT_hs_rqst_clk_actual;
  float                  fT_hs_trail_actual;
  float                  fTEOT_data;
  float                  fTEOT_clock;
  float                  fUiBitclk              = HAL_DSI_PHY_TIPX_NUMERATOR / (float)pTimingParameters->uBitclock;
  float                  fVal;
  int32                  iMin;
  int32                  iMax;
  int32                  iVal;
  uint32                 uT_clk_prepare;
  uint32                 uT_clk_zero;
  uint32                 uT_clk_trail;
  uint32                 uT_hs_prepare;
  uint32                 uT_hs_zero;
  uint32                 uT_hs_trail;
  uint32                 uT_hs_rqst;
  uint32                 uHalfByteClkEn;
  uint32                 uT_hs_exit;
  uint32                 uT_ta_go;
  uint32                 uT_ta_sure;
  uint32                 uT_ta_get;
  uint32                 uT_clk_post;
  uint32                 uT_clk_pre;
  bool32                 bT_clk_pre_inc_by_2    = FALSE;

  fTEOT_data  = HAL_DSI_PHY_TEOT_PARAM1 + HAL_DSI_PHY_TEOT_PARAM2 * fUiBitclk;
  fTEOT_clock = HAL_DSI_PHY_TEOT_PARAM1 + HAL_DSI_PHY_TEOT_PARAM2 * fUiBitclk;

  /**********************************************************************************/
  /* HSTX_HALFBYTECLK_EN calculation */
  if (90000000 > pTimingParameters->uBitclock)
  {
    uHalfByteClkEn = 1;
  }
  else
  {
    uHalfByteClkEn = 0;
  }

  /**********************************************************************************/
  /* T_CLK_PREPARE calculation */
  if (pPreDefinedTimings->bTimingCLKPrepareOverride)
  {
    uT_clk_prepare = pPreDefinedTimings->uTimingCLKPrepareValue;
  }
  else
  {
    fMin = 0;
    fMax = 0;

    /* calculate the min and max in float */
    fVal = (HAL_DSI_PHY_TCLK_PREPARE_PHY_MIN / fUiBitclk) - (HAL_DSI_PHY_TCLK_PREPARE_PHY_PARAM1 * uHalfByteClkEn);
    iMin = HAL_DSI_Phy_Roundup(fVal / HAL_DSI_PHY_TCLK_PREPARE_PHY_PARAM2);
    fMin = (float)HAL_DSI_Phy_CheckNegative(iMin);

    fVal = (HAL_DSI_PHY_TCLK_PREPARE_PHY_MAX / fUiBitclk) - (HAL_DSI_PHY_TCLK_PREPARE_PHY_PARAM1 * uHalfByteClkEn);
    iMax = HAL_DSI_Phy_Rounddown(fVal / HAL_DSI_PHY_TCLK_PREPARE_PHY_PARAM2);
    fMax = (float)HAL_DSI_Phy_CheckNegative(iMax);

    /* interpolate the value at 10% between min and max */
    iVal = HAL_DSI_Phy_Roundup(((fMax - fMin) * HAL_DSI_PHY_RANGEFACTOR_TCLK_PREPARE / HAL_DSI_PHY_PERCENT_DENOMENATOR) + fMin);
    uT_clk_prepare = HAL_DSI_Phy_CheckNegative(iVal);
  }

  fT_clk_prepare_actual = ((uT_clk_prepare * HAL_DSI_PHY_TCLK_PREPARE_PHY_PARAM2) +
                           (uHalfByteClkEn * HAL_DSI_PHY_TCLK_PREPARE_PHY_PARAM3)) * fUiBitclk;

  /**********************************************************************************/
  /* T_CLK_ZERO calculation */
  if (pPreDefinedTimings->bTimingCLKZeroOverride)
  {
    uT_clk_zero = pPreDefinedTimings->uTimingCLKZeroValue;
  }
  else
  {
    fMin = HAL_DSI_PHY_TCLK_ZERO_PARAM1 - fT_clk_prepare_actual;
    iMin = HAL_DSI_Phy_Roundup((fMin / fUiBitclk) / HAL_DSI_PHY_TCLK_ZERO_PARAM3) - 1;
    iMax = (iMin > HAL_DSI_PHY_TCLK_ZERO_PARAM5) ? HAL_DSI_PHY_TCLK_ZERO_PARAM6 : HAL_DSI_PHY_TCLK_ZERO_PARAM5;
    iVal = HAL_DSI_Phy_Roundup(((iMax - iMin) * HAL_DSI_PHY_RANGEFACTOR_TCLK_ZERO / HAL_DSI_PHY_PERCENT_DENOMENATOR) + iMin);
    uT_clk_zero = HAL_DSI_Phy_CheckNegative(iVal);
  }

  fT_clk_zero_actual = ((uT_clk_zero + HAL_DSI_PHY_TCLK_ZERO_PARAM4) * HAL_DSI_PHY_TCLK_ZERO_PARAM3) * fUiBitclk;

  /**********************************************************************************/
  /* T_CLK_TRAIL calculation */
  if (pPreDefinedTimings->bTimingCLKTrailOverride)
  {
    uT_clk_trail = pPreDefinedTimings->uTimingCLKTrailValue;
  }
  else
  {
    fMin = HAL_DSI_PHY_TCLK_TRAIL_MIN;
    fMax = fTEOT_clock - HAL_DSI_PHY_TREOT;
    iMin = HAL_DSI_Phy_Roundup((fMin / fUiBitclk) / HAL_DSI_PHY_T_T_TRAIL_PARAM4) - HAL_DSI_PHY_T_T_TRAIL_PARAM5;
    iMax = HAL_DSI_Phy_Rounddown((fMax / fUiBitclk) / HAL_DSI_PHY_T_T_TRAIL_PARAM4) - HAL_DSI_PHY_T_T_TRAIL_PARAM5;
    iVal = HAL_DSI_Phy_Roundup(((iMax - iMin) * HAL_DSI_PHY_RANGEFACTOR_TRAIL / HAL_DSI_PHY_PERCENT_DENOMENATOR) + iMin);
    uT_clk_trail = HAL_DSI_Phy_CheckNegative(iVal);
  }
  /* fT_clk_trail_actual = ((uT_clk_trail + HAL_DSI_PHY_T_T_TRAIL_PARAM3) * HAL_DSI_PHY_T_T_TRAIL_PARAM4) * fUiBitclk; */

  /**********************************************************************************/
  /* T_HS_PREPARE calculation */
  if (pPreDefinedTimings->bTimingHSPrepareOverride)
  {
    uT_hs_prepare = pPreDefinedTimings->uTimingHSPrepareValue;
  }
  else
  {
    fMin = HAL_DSI_PHY_T_HS_PREPARE_PARAM1 + HAL_DSI_PHY_T_HS_PREPARE_PARAM2 * fUiBitclk;
    fMax = HAL_DSI_PHY_T_HS_PREPARE_PARAM3 + HAL_DSI_PHY_T_HS_PREPARE_PARAM4 * fUiBitclk;

    fVal = ((fMin / fUiBitclk) - HAL_DSI_PHY_T_HS_PREPARE_PARAM5 * uHalfByteClkEn);
    iVal = HAL_DSI_Phy_Roundup(fVal / HAL_DSI_PHY_T_HS_PREPARE_PARAM6);
    iMin = HAL_DSI_Phy_CheckNegative(iVal);

    fVal = ((fMax / fUiBitclk) - HAL_DSI_PHY_T_HS_PREPARE_PARAM5 * uHalfByteClkEn);
    iVal = HAL_DSI_Phy_Rounddown(fVal / HAL_DSI_PHY_T_HS_PREPARE_PARAM6);
    iMax = HAL_DSI_Phy_CheckNegative(iVal);

    iVal = HAL_DSI_Phy_Roundup(((iMax - iMin) * HAL_DSI_PHY_RANGEFACTOR_THS_PREPARE / HAL_DSI_PHY_PERCENT_DENOMENATOR) + iMin);
    uT_hs_prepare = HAL_DSI_Phy_CheckNegative(iVal);
  }

  fVal = (uT_hs_prepare * HAL_DSI_PHY_T_HS_PREPARE_PARAM6) + (uHalfByteClkEn * HAL_DSI_PHY_T_HS_PREPARE_PARAM5);
  fT_hs_prepare_actual = fVal * fUiBitclk;

  /**********************************************************************************/
  /* T_HS_ZERO calculation */
  if (pPreDefinedTimings->bTimingHSZeroOverride)
  {
    uT_hs_zero = pPreDefinedTimings->uTimingHSZeroValue;
  }
  else
  {
    fMin = HAL_DSI_PHY_T_HS_ZERO_PARAM1 + HAL_DSI_PHY_T_HS_ZERO_PARAM2 * fUiBitclk - fT_hs_prepare_actual;
    iMin = HAL_DSI_Phy_Roundup((fMin / fUiBitclk) / HAL_DSI_PHY_T_HS_ZERO_PARAM4) - HAL_DSI_PHY_T_HS_ZERO_PARAM5;
    iMax = HAL_DSI_PHY_T_HS_ZERO_PARAM6;
    iVal = HAL_DSI_Phy_Roundup(((iMax - iMin) * HAL_DSI_PHY_RANGEFACTOR_THS_ZERO / HAL_DSI_PHY_PERCENT_DENOMENATOR) + iMin);
    uT_hs_zero = HAL_DSI_Phy_CheckNegative(iVal);
  }

  /**********************************************************************************/
  /* T_HS_TRAIL calculation */
  if (pPreDefinedTimings->bTimingHSTrailOverride)
  {
    uT_hs_trail = pPreDefinedTimings->uTimingHSTrailValue;
  }
  else
  {
    fMin = HAL_DSI_PHY_T_T_TRAIL_PARAM1 + (HAL_DSI_PHY_T_T_TRAIL_PARAM2 * fUiBitclk);
    fMax = fTEOT_data - HAL_DSI_PHY_TREOT;
    iMin = HAL_DSI_Phy_Roundup((fMin / fUiBitclk) / HAL_DSI_PHY_T_T_TRAIL_PARAM4) - HAL_DSI_PHY_T_T_TRAIL_PARAM5;
    iMax = HAL_DSI_Phy_Rounddown((fMax / fUiBitclk) / HAL_DSI_PHY_T_T_TRAIL_PARAM4) - HAL_DSI_PHY_T_T_TRAIL_PARAM5;
    iVal = HAL_DSI_Phy_Roundup(((iMax - iMin) * HAL_DSI_PHY_RANGEFACTOR_TRAIL / HAL_DSI_PHY_PERCENT_DENOMENATOR) + iMin);
    uT_hs_trail = HAL_DSI_Phy_CheckNegative(iVal);
  }
  fT_hs_trail_actual = (uT_hs_trail + HAL_DSI_PHY_T_T_TRAIL_PARAM5) * HAL_DSI_PHY_T_T_TRAIL_PARAM4 * fUiBitclk;

  /**********************************************************************************/
  /* T_HS_RQST is user entry based on UI/ESCCLK */
  uT_hs_rqst = 0;
  if (pPreDefinedTimings->bTimingHSRequestOverride)
  {
    uT_hs_rqst = pPreDefinedTimings->uTimingHSRequestValue;
  }
  else
  {
    fVal = (HAL_DSI_PHY_T_HS_RQST_PARAM1 / fUiBitclk) + (HAL_DSI_PHY_T_HS_RQST_PARAM2 * uHalfByteClkEn);
    iVal = HAL_DSI_Phy_Roundup((fVal - HAL_DSI_PHY_T_HS_RQST_PARAM3) / HAL_DSI_PHY_T_HS_RQST_PARAM3);
    uT_hs_rqst = HAL_DSI_Phy_CheckNegative(iVal);
  }

  fVal = (uT_hs_rqst * HAL_DSI_PHY_T_HS_RQST_PARAM3) + HAL_DSI_PHY_T_HS_RQST_PARAM2;
  fVal = fVal + (HAL_DSI_PHY_T_HS_RQST_PARAM2 * uHalfByteClkEn);
  fT_hs_rqst_clk_actual = fVal * fUiBitclk;

  /**********************************************************************************/
  /* T_HS_EXIT calculation */
  if (pPreDefinedTimings->bTimingHSExitOverride)
  {
    uT_hs_exit = pPreDefinedTimings->uTimingHSExitValue;
  }
  else
  {
    fVal = HAL_DSI_PHY_T_HS_EXIT_MIN / fUiBitclk / HAL_DSI_PHY_T_HS_EXIT_PARAM1;
    iMin = HAL_DSI_Phy_Roundup(fVal) - HAL_DSI_PHY_T_HS_EXIT_PARAM2;
    iMax = HAL_DSI_PHY_T_HS_EXIT_MAX;
    iVal = HAL_DSI_Phy_Roundup(((iMax - iMin) * HAL_DSI_PHY_RANGEFACTOR_NOMAX / HAL_DSI_PHY_PERCENT_DENOMENATOR) + iMin);
    uT_hs_exit = HAL_DSI_Phy_CheckNegative(iVal);
  }

  /**********************************************************************************/
  /* T_TA_GO calculation (Hard coded value) */
  uT_ta_go = HAL_DSI_PHY_T_TA_GO_PARAM1;
  if (pPreDefinedTimings->bTimingTAGoOverride)
  {
    uT_ta_go = pPreDefinedTimings->uTimingTAGoValue;
  }

  /**********************************************************************************/
  /* T_TA_SURE calculation (Hard coded value) */
  uT_ta_sure = HAL_DSI_PHY_T_TA_SURE_PARAM1;
  if (pPreDefinedTimings->bTimingTASureOverride)
  {
    uT_ta_sure = pPreDefinedTimings->uTimingTASureValue;
  }

  /**********************************************************************************/
  /* T_TA_GET calculation (Hard coded value) */
  uT_ta_get = HAL_DSI_PHY_T_TA_GET_PARAM1;
  if (pPreDefinedTimings->bTimingTAGetOverride)
  {
    uT_ta_get = pPreDefinedTimings->uTimingTAGetValue;
  }

  /**********************************************************************************/
  /* T_CLK_POST calculation */
  if (pPreDefinedTimings->bTimingCLKPostOverride)
  {
    uT_clk_post = pPreDefinedTimings->uTimingCLKPostValue;
  }
  else
  {
    fMin = HAL_DSI_PHY_T_CLK_POST_PARAM1 + HAL_DSI_PHY_T_CLK_POST_PARAM2 * fUiBitclk;
    fVal = (fMin + fT_hs_trail_actual) / (HAL_DSI_PHY_T_CLK_POST_PARAM4 * fUiBitclk);
    iMin = HAL_DSI_Phy_Roundup(fVal) - HAL_DSI_PHY_T_CLK_POST_PARAM5;

    iMax = HAL_DSI_PHY_T_CLK_POST_MAX;
    iVal = HAL_DSI_Phy_Roundup((iMax - iMin) * (HAL_DSI_PHY_RANGEFACTOR_TCLK_POST / HAL_DSI_PHY_PERCENT_DENOMENATOR) + iMin);
    uT_clk_post = (iVal > 0) ? iVal : 0;
  }

  /**********************************************************************************/
  /* T_CLK_PRE calculation */
  if (pPreDefinedTimings->bTimingCLKPreOverride)
  {
    uT_clk_pre = pPreDefinedTimings->uTimingCLKPreValue;
  }
  else
  {
    fMin = HAL_DSI_PHY_T_CLK_PRE_PARAM1 * fUiBitclk;
    fVal = (fMin + fT_clk_prepare_actual + fT_clk_zero_actual + fT_hs_rqst_clk_actual);
    iMin = HAL_DSI_Phy_Roundup(fVal / (HAL_DSI_PHY_T_CLK_PRE_PARAM2 * fUiBitclk)) - HAL_DSI_PHY_T_CLK_PRE_PARAM4;
    iMax = HAL_DSI_PHY_T_CLK_PRE_MAX;
    if (iMin > iMax)
    {
      fVal = (((iMax * HAL_DSI_PHY_T_CLK_PRE_PARAM3) - iMin) * HAL_DSI_PHY_RANGEFACTOR_TCLK_PRE / HAL_DSI_PHY_PERCENT_DENOMENATOR / HAL_DSI_PHY_PERCENT_DENOMENATOR);
      iVal = HAL_DSI_Phy_Roundup(fVal + iMin);
      iVal = HAL_DSI_Phy_Rounddown(iVal / HAL_DSI_PHY_T_CLK_PRE_PARAM3);
      bT_clk_pre_inc_by_2 = TRUE;
    }
    else
    {
      iVal = HAL_DSI_Phy_Roundup(((iMax - iMin) * HAL_DSI_PHY_RANGEFACTOR_TCLK_PRE / HAL_DSI_PHY_PERCENT_DENOMENATOR / HAL_DSI_PHY_PERCENT_DENOMENATOR) + iMin);
      bT_clk_pre_inc_by_2 = FALSE;
    }
    uT_clk_pre = (iVal < 0) ? 0 : iVal;
  }

  /**********************************************************************************/
  /*   Up to here, all DSI timing parameters have been calculated                   */
  /*   now, assign the parameters and program the HW                                */
  /**********************************************************************************/

  /* Assign data lane timing parameters */
  psTimingConfig->bHSTX_HalfByteClk_En = (uHalfByteClkEn > 0) ? TRUE : FALSE;
  psTimingConfig->uT_HS_PREPARE        = uT_hs_prepare;
  psTimingConfig->uT_HS_EXIT           = uT_hs_exit;
  psTimingConfig->uT_HS_RQST           = uT_hs_rqst;
  psTimingConfig->uT_HS_TRAIL          = uT_hs_trail;
  psTimingConfig->uT_HS_ZERO           = uT_hs_zero;
  psTimingConfig->uT_TA_GET            = uT_ta_get;
  psTimingConfig->uT_TA_GO             = uT_ta_go;
  psTimingConfig->uT_TA_SURE           = uT_ta_sure;
  psTimingConfig->uT_CLK_PREPARE       = uT_clk_prepare;
  psTimingConfig->uT_CLK_ZERO          = uT_clk_zero;
  psTimingConfig->uT_CLK_TRAIL         = uT_clk_trail;
  psTimingConfig->uT_CLK_PRE           = uT_clk_pre;
  psTimingConfig->uT_CLK_POST          = uT_clk_post;
  psTimingConfig->bT_CLK_PRE_INC_BY_2  = bT_clk_pre_inc_by_2;

  return eStatus;

}

/****************************************************************************
*
** FUNCTION: HAL_DSI_Phy_CalculateTimingParams_CPhy()
*/
/*!
* \brief
*     Calculate C-Phy timing parameters
*
* \param [in]  pTimingParameters - DSI core ID
*
* \retval HAL_MDSS_ErrorType
*
****************************************************************************/
static HAL_MDSS_ErrorType HAL_DSI_Phy_CalculateTimingParams_CPhy(HAL_DSI_TimingSettingType *pTimingParameters,
                                                                 HAL_DSI_PHY_TimingConfigType *psTimingConfig)
{
  HAL_MDSS_ErrorType     eStatus                = HAL_MDSS_STATUS_SUCCESS;
  DSI_TimingOverrideType *pPreDefinedTimings    = pTimingParameters->pPreDefinedTimings;
  float                  fMin;
  float                  fMax;
  float                  fUiBitclk;
  float                  fVal;
  int32                  iMin;
  int32                  iMax;
  int32                  iVal;
  uint32                 uHalfByteClkEn;
  uint32                 uT_hs_exit;
  uint32                 uT_prebegin;
  uint32                 uT_prepare;
  uint32                 uT_post;
  uint32                 uT_hs_rqst;
  uint32                 uT_ta_go;
  uint32                 uT_ta_sure;
  uint32                 uT_ta_get;

  if (0 == pTimingParameters->uBitclock)
  {
    eStatus = HAL_MDSS_STATUS_FAILED_NOT_SUPPORTED;
  }
  else
  {
     fUiBitclk              = HAL_DSI_PHY_TIPX_NUMERATOR / (float)pTimingParameters->uBitclock;

     /**********************************************************************************/
     /* TIMING_CTRL_0, HSTX_HALFBYTECLK_EN calculation */
     if (HAL_DSI_CPHY_HALFBYTECLK_EN_THRESHOLD > pTimingParameters->uBitclock)
     {
       uHalfByteClkEn = 1;
     }
     else
     {
       uHalfByteClkEn = 0;
     }

     /* TIMING_CTRL_4, T3_HS_EXIT */
     if (pPreDefinedTimings->bTimingHSExitOverride)
     {
       uT_hs_exit = pPreDefinedTimings->uTimingHSExitValue;
     }
     else
     {
       fVal = HAL_DSI_CPHY_T_HS_EXIT_MIN / fUiBitclk;
       iMin = HAL_DSI_Phy_Roundup(fVal / HAL_DSI_CPHY_BITS_BYTE);
       iMin -= 1;
       fMin = (float)HAL_DSI_Phy_CheckNegative(iMin);
       fMax = HAL_DSI_CPHY_T_HS_EXIT_MAX;
       iVal = HAL_DSI_Phy_Roundup((fMax - fMin) * (HAL_DSI_CPHY_RANGEFACTOR_0 / HAL_DSI_PHY_PERCENT_DENOMENATOR) + fMin);
       uT_hs_exit = HAL_DSI_Phy_CheckNegative(iVal);
     }


     /* TIMING_CTRL_5, T_HS_ZERO/T3_PREBEGIN */
     if (pPreDefinedTimings->bTimingHSZeroOverride)
     {
       uT_prebegin = pPreDefinedTimings->uTimingHSZeroValue;
     }
     else
     {
       fVal = HAL_DSI_CPHY_T3_PREBEGIN_CNT_MIN * fUiBitclk;
       fVal = 1.0f;
       iMin = HAL_DSI_Phy_Roundup((fVal / fUiBitclk) / HAL_DSI_CPHY_BITS_BYTE);
       iMin -= 1;
       fMin = (float)HAL_DSI_Phy_CheckNegative(iMin);
       fVal = HAL_DSI_CPHY_T3_PREBEGIN_CNT_MAX * fUiBitclk;
       iMax = HAL_DSI_Phy_Rounddown((fVal / fUiBitclk) / HAL_DSI_CPHY_BITS_BYTE);
       iMax -= 1;
       fMax = (float)HAL_DSI_Phy_CheckNegative(iMax);
       iVal = HAL_DSI_Phy_Roundup((fMax - fMin) * (HAL_DSI_CPHY_RANGEFACTOR_0 / HAL_DSI_PHY_PERCENT_DENOMENATOR) + fMin);
       uT_prebegin = HAL_DSI_Phy_CheckNegative(iVal);
     }


     /* TIMING_CTRL_6, T3_PREPARE/T_HS_PREPARE */
     if (pPreDefinedTimings->bTimingHSPrepareOverride)
     {
       uT_prepare = pPreDefinedTimings->uTimingHSPrepareValue;
     }
     else
     {
       /* calculate the min and max in float */
       fVal = ((float)HAL_DSI_CPHY_T3_PREPARE_MIN / fUiBitclk) - (HAL_DSI_CPHY_BITS_SYMBOL * uHalfByteClkEn);
       iMin = HAL_DSI_Phy_Roundup(fVal / HAL_DSI_CPHY_BITS_BYTE);
       fMin = (float)HAL_DSI_Phy_CheckNegative(iMin);
       fVal = ((float)HAL_DSI_CPHY_T3_PREPARE_MAX / fUiBitclk) - (HAL_DSI_CPHY_BITS_SYMBOL * uHalfByteClkEn);
       iMax = HAL_DSI_Phy_Rounddown(fVal / HAL_DSI_CPHY_BITS_BYTE);
       fMax = (float)HAL_DSI_Phy_CheckNegative(iMax);
       iVal = HAL_DSI_Phy_Roundup((fMax - fMin) * (HAL_DSI_CPHY_RANGEFACTOR_0 / HAL_DSI_PHY_PERCENT_DENOMENATOR) + fMin);
       uT_prepare = HAL_DSI_Phy_CheckNegative(iVal);
     }


     /* TIMING_CTRL_7, T3_POST/T_HS_TRAIL */
     if (pPreDefinedTimings->bTimingHSTrailOverride)
     {
       uT_post = pPreDefinedTimings->uTimingHSTrailValue;
     }
     else
     {
       fVal = HAL_DSI_CPHY_T3_POST_CNT_MIN * fUiBitclk;
       fVal = 1.0f;
       iMin = HAL_DSI_Phy_Roundup((fVal/ fUiBitclk) / HAL_DSI_CPHY_BITS_BYTE);
       iMin -= 1;
       fMin = (float)HAL_DSI_Phy_CheckNegative(iMin);
       fVal = HAL_DSI_CPHY_T3_POST_CNT_MAX * fUiBitclk;
       iMax = HAL_DSI_Phy_Roundup((fVal / fUiBitclk) / HAL_DSI_CPHY_BITS_BYTE);
       iMax -= 1;
       fMax = (float)HAL_DSI_Phy_CheckNegative(iMax);
       iVal = HAL_DSI_Phy_Roundup((fMax - fMin) * (HAL_DSI_CPHY_RANGEFACTOR_0 / HAL_DSI_PHY_PERCENT_DENOMENATOR) + fMin);
       uT_post = HAL_DSI_Phy_CheckNegative(iVal);
     }


     /* TIMING_CTRL_8, T_HS_RQST */
     uT_hs_rqst = 0;
     if (pPreDefinedTimings->bTimingHSRequestOverride)
     {
       uT_hs_rqst = pPreDefinedTimings->uTimingHSRequestValue;
     }
     else
     {
       fVal = (HAL_DSI_CPHY_T_HS_RQST_MIN / fUiBitclk) + (4 * uHalfByteClkEn) - HAL_DSI_CPHY_BITS_BYTE; 
       uT_hs_rqst = HAL_DSI_Phy_Roundup(fVal / HAL_DSI_CPHY_BITS_BYTE);
     }

     /* T_TA_GO calculation (Hard coded value) */
     uT_ta_go = HAL_DSI_PHY_T_TA_GO_PARAM1;
     if (pPreDefinedTimings->bTimingTAGoOverride)
     {
       uT_ta_go = pPreDefinedTimings->uTimingTAGoValue;
     }

     /* T_TA_SURE calculation (Hard coded value) */
     uT_ta_sure = HAL_DSI_PHY_T_TA_SURE_PARAM1;
     if (pPreDefinedTimings->bTimingTASureOverride)
     {
       uT_ta_sure = pPreDefinedTimings->uTimingTASureValue;
     }

     /* T_TA_GET calculation (Hard coded value) */
     uT_ta_get = HAL_DSI_PHY_T_TA_GET_PARAM1;
     if (pPreDefinedTimings->bTimingTAGetOverride)
     {
       uT_ta_get = pPreDefinedTimings->uTimingTAGetValue;
     }

     /**********************************************************************************/
     /*   Up to here, all DSI CPhy timing parameters have been calculated                   */
     /*   now, assign the parameters and program the HW                                */
     /**********************************************************************************/

     /* Assign data lane timing parameters */
     psTimingConfig->bHSTX_HalfByteClk_En = (uHalfByteClkEn > 0) ? TRUE : FALSE;
     psTimingConfig->uT_HS_EXIT           = uT_hs_exit;
     psTimingConfig->uT_HS_ZERO           = uT_prebegin;
     psTimingConfig->uT_HS_PREPARE        = uT_prepare;
     psTimingConfig->uT_HS_TRAIL          = uT_post;
     psTimingConfig->uT_HS_RQST           = uT_hs_rqst;
     psTimingConfig->uT_TA_SURE           = uT_ta_sure;
     psTimingConfig->uT_TA_GO             = uT_ta_go;
     psTimingConfig->uT_TA_GET            = uT_ta_get;
   }
   return eStatus;
}


/****************************************************************************
*
** FUNCTION: HAL_DSI_Phy_CommitTimingParams()
*/
/*!
* \brief
*     Config lane timing parameters
*
* \param [in]  eDeviceId     - DSI core ID
* \param [in]  pTimingConfig - pointer to lane timing parameter structure
*
* \retval None
*
****************************************************************************/
static void HAL_DSI_Phy_CommitTimingParams(DSI_Device_IDType eDeviceId, HAL_DSI_PHY_TimingConfigType *pTimingConfig)
{
  uint32               uRegVal;
  uintPtr              uOffset = HAL_DSI_GetRegBaseOffset(eDeviceId);

  /*HALFBYTECLK_EN*/
  DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_0_ADDR,                 pTimingConfig->bHSTX_HalfByteClk_En);
  /* T_CLK_ZERO */
  DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_1_ADDR,                 pTimingConfig->uT_CLK_ZERO);
  /* T_CLK_PREPARE */
  DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_2_ADDR,                 pTimingConfig->uT_CLK_PREPARE);
  /* T_CLK_TRAIL */
  DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_3_ADDR,                 pTimingConfig->uT_CLK_TRAIL);
  /* T_HS_EXIT */
  DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_4_ADDR,                 pTimingConfig->uT_HS_EXIT);
  /* T_HS_ZERO */
  DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_5_ADDR,                 pTimingConfig->uT_HS_ZERO);
  /* T_HS_PREPARE */
  DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_6_ADDR,                 pTimingConfig->uT_HS_PREPARE);
  /* T_HS_TRAIL */
  DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_7_ADDR,                 pTimingConfig->uT_HS_TRAIL);
  /* T_HS_RQST */
  DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_8_ADDR,                 pTimingConfig->uT_HS_RQST);
  /* T_TA_GO & T_TA_SURE */
  uRegVal = HWIO_OUT_FLD(0, DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_9, DSIPHY_T_TA_SURE,   pTimingConfig->uT_TA_SURE);
  uRegVal = HWIO_OUT_FLD(uRegVal, DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_9, DSIPHY_T_TA_GO,pTimingConfig->uT_TA_GO);
  DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_9_ADDR,                 uRegVal);
  /* T_TA_GET */
  DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_10_ADDR,                pTimingConfig->uT_TA_GET);
  /* TRIG3_CMD */
  DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_11_ADDR,                0x00000000);
  /* T_CLK_PRE */
  DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_12_ADDR,                pTimingConfig->uT_CLK_PRE);
  /* T_CLK_POST */
  DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_13_ADDR,                pTimingConfig->uT_CLK_POST);  

}

/****************************************************************************
*
** FUNCTION: HAL_DSI_Phy_CommitTimingParams_CPhy()
*/
/*!
* \brief
*     Config C-PHY lane timing parameters
*
* \param [in]  eDeviceId     - DSI core ID
* \param [in]  pTimingConfig - pointer to lane timing parameter structure
*
* \retval None
*
****************************************************************************/
static void HAL_DSI_Phy_CommitTimingParams_CPhy(DSI_Device_IDType eDeviceId, HAL_DSI_PHY_TimingConfigType *pTimingConfig)
{
  uint32               uRegVal = 0;
  uintPtr              uOffset = HAL_DSI_GetRegBaseOffset(eDeviceId);

  /* PHY MODE */
  uRegVal = 0;
  uRegVal = HWIO_OUT_FLD(uRegVal, DSI_0_PHY_DSIPHY_CMN_GLBL_CTRL, PHY_MODE, 1);
  DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_CTRL_ADDR, uRegVal);

  /* VREG_CTRL */
  DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_CMN_VREG_CTRL_0_ADDR, 0x00000051);
  DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_CMN_VREG_CTRL_1_ADDR, 0x00000055);

  /*HALFBYTECLK_EN*/
  DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_0_ADDR, pTimingConfig->bHSTX_HalfByteClk_En);

  /* T_HS_EXIT */
  DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_4_ADDR, pTimingConfig->uT_HS_EXIT);
  /* T_HS_ZERO */
  DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_5_ADDR, pTimingConfig->uT_HS_ZERO);
  /* T_HS_PREPARE */
  DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_6_ADDR, pTimingConfig->uT_HS_PREPARE);
  /* T_HS_TRAIL */
  DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_7_ADDR, pTimingConfig->uT_HS_TRAIL);
  /* T_HS_RQST */
  DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_8_ADDR, pTimingConfig->uT_HS_RQST);
  /* T_TA_GO & T_TA_SURE */
  uRegVal = HWIO_OUT_FLD(0, DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_9, DSIPHY_T_TA_SURE, pTimingConfig->uT_TA_SURE);
  uRegVal = HWIO_OUT_FLD(uRegVal, DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_9, DSIPHY_T_TA_GO, pTimingConfig->uT_TA_GO);
  DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_9_ADDR, uRegVal);
  /* T_TA_GET */
  DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_10_ADDR, pTimingConfig->uT_TA_GET);

  /*DSIPHY_TRIG3_CMD*/
  DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_11_ADDR, 0x00000000);

  /* DCTRL */
  DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_2_ADDR, 0x00000040);

}

/****************************************************************************
*
** FUNCTION: HAL_DSI_Phy_Setup()
*/
/*!
* \brief
*     Setup DSI PHY
*
* \param [in]  eDeviceId     - DSI core ID
* \param [in]  eLaneId       - DSI lane ID
* \param [in]  pConfig       - pointer to PHY config data structure
*
* \retval HAL_MDSS_ErrorType
*
****************************************************************************/
static HAL_MDSS_ErrorType HAL_DSI_Phy_SetupLanePhy(DSI_Device_IDType eDeviceId, DSI_LaneIDType eLaneId, HAL_DSI_PHY_LaneConfigType *pConfig)
{
  HAL_MDSS_ErrorType   eStatus       = HAL_MDSS_STATUS_SUCCESS;
  uint32               uRegVal       = 0;
  uintPtr              uOffset       = HAL_DSI_GetRegBaseOffset(eDeviceId) + HAL_DSI_Phy_GetLaneOffset(eLaneId);
  uint32               uClkLane      = 0;
  uint32               uLPRXCtrl     = 0;

  if (DSI_LaneID_CLK == eLaneId)
  {
    uClkLane      = 1;
  }
  else
  {
    uClkLane      = 0;
  }

  if (DSI_LaneID_0 == eLaneId)
  {
    uLPRXCtrl = 3;  /* enable RX contention detect */
  }

  /* DSIPHY_LPRX_EN */
  /* DSIPHY_CDRX_EN */
  DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_DLN0_LPRX_CTRL_ADDR,  0x00000000);
  DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_DLN0_LPRX_CTRL_ADDR,  uLPRXCtrl);

  /* Pin Swap */
  DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_DLN0_PIN_SWAP_ADDR,   0x00000000);

  /* PGM Delay */
  DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_DLN0_CFG0_ADDR,       0x00000000);
  DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_DLN0_CFG1_ADDR,       0x00000000);

  /* IS_CKLANE */
  uRegVal = 0;
  uRegVal = HWIO_OUT_FLD(uRegVal, DSI_0_PHY_DSIPHY_DLN0_CFG2, IS_CKLANE, uClkLane);

  /* DSIPHY_LPRX_DLY */
  uRegVal = HWIO_OUT_FLD(uRegVal, DSI_0_PHY_DSIPHY_DLN0_CFG2, DSIPHY_LPRX_DLY, 0x0000000A);
  DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_DLN0_CFG2_ADDR, uRegVal);

  /* DCTRL */
  uRegVal = HAL_DSI_Phy_GetTxDctrl(eLaneId);
  DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_DLN0_TX_DCTRL_ADDR, uRegVal);

  return eStatus;
}

/****************************************************************************
*
** FUNCTION: HAL_DSI_Phy_Toogle_Dln3_Tx_Dctrl()
*/
/*!
* \brief
*     Toogle Data lane 3's Tx_Dctrl value for the specific Phy Device.
*
*
* \param [in]  eLainId - Lane id;
*
* \retval  Tx Dctrl value
*
****************************************************************************/
static void HAL_DSI_Phy_Toogle_Dln3_Tx_Dctrl(DSI_Device_IDType eDeviceId)
{
  uintPtr  uOffset = HAL_DSI_GetRegBaseOffset(eDeviceId) + HAL_DSI_Phy_GetLaneOffset(DSI_LaneID_3);
  uint32   uRegVal = 0;

  uRegVal = in_dword(uOffset + HWIO_DSI_0_PHY_DSIPHY_DLN0_TX_DCTRL_ADDR);
  uRegVal &= ~0x01; /* clear bit 0 and keep all other bits including bit 2 */
  /* toggle bit 0 */
  DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_DLN0_TX_DCTRL_ADDR, (0x01 | uRegVal));
  DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_DLN0_TX_DCTRL_ADDR, uRegVal);
}

/****************************************************************************
*
** FUNCTION: HAL_DSI_Phy_SetupLanPhy_CPhy()
*/
/*!
* \brief
*     Setup DSI C-PHY Lane
*
* \param [in]  eDeviceId     - DSI core ID
* \param [in]  eLaneId       - DSI lane ID
* \param [in]  pConfig       - pointer to PHY config data structure
*
* \retval HAL_MDSS_ErrorType
*
****************************************************************************/
static HAL_MDSS_ErrorType HAL_DSI_Phy_SetupLanePhy_CPhy(DSI_Device_IDType eDeviceId, DSI_LaneIDType eLaneId, HAL_DSI_PHY_LaneConfigType *pConfig)
{
  HAL_MDSS_ErrorType   eStatus       = HAL_MDSS_STATUS_SUCCESS;

  uintPtr              uOffset       = HAL_DSI_GetRegBaseOffset(eDeviceId) + HAL_DSI_Phy_GetLaneOffset(eLaneId);
  uint32               uLPRXCtrl     = 0;

  if ((DSI_LaneID_CLK == eLaneId) ||
      (DSI_LaneID_3 == eLaneId))
  {
    eStatus = HAL_MDSS_DSI_FAILED_PARAMETER_OUT_OF_RANGE;
  }
  else
  {
    if (DSI_LaneID_0 == eLaneId)
    {
      uLPRXCtrl = 3;
    }

    /* DSIPHY_LPRX_EN */
    /* DSIPHY_CDRX_EN */
    /* Transition from 0 to 1 for DLN0, others stays 0 */
    DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_DLN0_LPRX_CTRL_ADDR, 0x00000000);
    DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_DLN0_LPRX_CTRL_ADDR, uLPRXCtrl);
  
    /* Pin Swap */
    DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_DLN0_PIN_SWAP_ADDR, 0x00000000);

    /* PGM Delay */
    DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_DLN0_CFG0_ADDR, 0x00000000);
    DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_DLN0_CFG1_ADDR, 0x00000000);

    /* DSIPHY_LPRX_DLY */
    DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_DLN0_CFG2_ADDR, 0x0000000A);
  }
  return eStatus;
}

/****************************************************************************
*
** FUNCTION: HAL_DSI_Phy_CommitPhy()
*/
/*!
* \brief
*     Configures the PHY hardware registers.
*
*
* \param [in]  eDeviceId     - DSI core ID
* \param [in]  pSettings     - pointer to phy settings structure
*
* \retval None
*
****************************************************************************/
static HAL_MDSS_ErrorType HAL_DSI_Phy_CommitPhy(DSI_Device_IDType             eDeviceId,
                                                HAL_DSI_PHY_PhySettingsType  *pSettings)
{
  HAL_MDSS_ErrorType eStatus       = HAL_MDSS_STATUS_SUCCESS;
  uintPtr            uOffset       = HAL_DSI_GetRegBaseOffset(eDeviceId);

  /* Wait for refgen */
  eStatus = HAL_DSI_Phy_WaitForRefgen(eDeviceId);

  if (HAL_MDSS_STATUS_SUCCESS == eStatus)
  {
    HAL_DSI_Phy_PowerUp(eDeviceId, pSettings); /* initial_init */

    /* DCTRL */
	DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_2_ADDR, 0x00000040);
	
    /* dsi_lane_init */
    if (pSettings->bCPhyMode)
    {
      HAL_DSI_Phy_SetupLanePhy_CPhy(eDeviceId, DSI_LaneID_0,   &pSettings->sLaneConfig);
      HAL_DSI_Phy_SetupLanePhy_CPhy(eDeviceId, DSI_LaneID_1,   &pSettings->sLaneConfig);
      HAL_DSI_Phy_SetupLanePhy_CPhy(eDeviceId, DSI_LaneID_2,   &pSettings->sLaneConfig);
    }
    else
    {
      HAL_DSI_Phy_SetupLanePhy(eDeviceId, DSI_LaneID_0,   &pSettings->sLaneConfig);
      HAL_DSI_Phy_SetupLanePhy(eDeviceId, DSI_LaneID_1,   &pSettings->sLaneConfig);
      HAL_DSI_Phy_SetupLanePhy(eDeviceId, DSI_LaneID_2,   &pSettings->sLaneConfig);
      HAL_DSI_Phy_SetupLanePhy(eDeviceId, DSI_LaneID_3,   &pSettings->sLaneConfig);
      HAL_DSI_Phy_SetupLanePhy(eDeviceId, DSI_LaneID_CLK, &pSettings->sLaneConfig);
    }
  }

  return eStatus;
}

/****************************************************************************
*
** FUNCTION: HAL_DSI_Phy_CommitPll()
*/
/*!
* \brief
*     Setup DSI PLL
*
* \param [in]  eDeviceId         - DSI core ID
* \param [in]  ePLLConfigSource  - PLL source
* \param [in]  pPllConfig        - pointer to PLL config data structure
*
* \retval None
*
****************************************************************************/
static void HAL_DSI_Phy_CommitPll(DSI_Device_IDType            eDeviceId,
                                  HAL_DSI_PLLConfigSourceType  ePLLConfigSource,
                                  HAL_DSI_PHY_PllConfigType   *pPllConfig)
{
  uintPtr            uOffset       = HAL_DSI_GetRegBaseOffset(eDeviceId);
  uint32             uRegVal       = 0;
  uint32             uPllPostDiv   = 0;
  uint32             i             = 0;
  uint32             uVCOConfig;
  uint32             uAnalogControlsFive;
  uint32             uClockInverters;
  uint32             uMinor        = HAL_DSI_Phy_Pll_Minor(eDeviceId);

  /* qlink_pll_init */

  /* PLL core input override */
  DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_PLL_CORE_INPUT_OVERRIDE_ADDR, 0x10);  /* pll enable + core reset */
  DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_PLL_CORE_INPUT_OVERRIDE_ADDR, 0x12);

  /* Dec div start */
  DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_PLL_DECIMAL_DIV_START_1_ADDR, pPllConfig->uDecDivStart);

  /* Frac div low part */
  DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_LOW_1_ADDR, pPllConfig->uFracDivLow);

  /* Frac div mid part */
  DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_MID_1_ADDR, pPllConfig->uFracDivMid);

  /* Frac div high part */
  DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_HIGH_1_ADDR, pPllConfig->uFracDivHigh);

  /* Enable PLL muxes i.e set BIAS_EN_MUX, BIAS_EN */
  uRegVal = 0;
  uRegVal = HWIO_OUT_FLD(uRegVal, DSI_0_PHY_DSIPHY_PLL_SYSTEM_MUXES, CM_BIAS_EN_MUX, 1);
  uRegVal = HWIO_OUT_FLD(uRegVal, DSI_0_PHY_DSIPHY_PLL_SYSTEM_MUXES, CM_BIAS_EN, 1);
  DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_PLL_SYSTEM_MUXES_ADDR, uRegVal);

  /* lockdet */
  DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_LOCKDET_RATE_1_ADDR, pPllConfig->uLockDetRate);

  /* Output divider */
  uPllPostDiv = 0;
  for (i = 0; i < 4; i++)
  {
    if (pPllConfig->uPllPostDiv & (1 << i))
    {
      uPllPostDiv = i;
      break;
    }
  }

  uRegVal = 0;
  uRegVal = HWIO_OUT_FLD(uRegVal, DSI_0_PHY_DSIPHY_PLL_PLL_OUTDIV_RATE,  PLL_OUTDIV_1, uPllPostDiv);
  DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_OUTDIV_RATE_ADDR, uRegVal);

  /* Pll lock delay */
  DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_LOCK_DELAY_ADDR, pPllConfig->uLockDelay);

  if (uMinor ==  1)
  {
     /* calculate vco divider and feed back divider */
     uAnalogControlsFive = ((pPllConfig->fActualFreqMHz < 3100) ? 0x1 : 0x3);

     /* calculate VCO COnfig base on vco rate */
     uVCOConfig     = (pPllConfig->fActualFreqMHz < 1520) ? 0x08 : 
                    (( pPllConfig->fActualFreqMHz < 2990) ? 0x01 : 0x00);

     /* calculate Clock Inverter base on vco rate */
     uClockInverters     = (  pPllConfig->fActualFreqMHz < 1000) ? 0xA0 : 
                           (( pPllConfig->fActualFreqMHz < 2550) ? 0x20 :
                           (( pPllConfig->fActualFreqMHz < 3020) ? 0x00 : 0x40));
  }
  else
  {
     uAnalogControlsFive = HAL_DSI_PLL_PRESCALE_FACTOR - 1;
     uVCOConfig          = ( pPllConfig->fActualFreqMHz >= HAL_DSI_PLL_VCO_CONFIG_FREQ_MAX) ? 1 : 0;
     uClockInverters     = ( pPllConfig->fActualFreqMHz < HAL_DSI_PLL_CLOCK_INVERTORS_FREQ_LEVEL_0 )? HAL_DSI_PLL_CLOCK_INVERTORS_VALUE_0 : 
                           (( pPllConfig->fActualFreqMHz < HAL_DSI_PLL_CLOCK_INVERTORS_FREQ_LEVEL_1) ? HAL_DSI_PLL_CLOCK_INVERTORS_VALUE_1 :
                           (( pPllConfig->fActualFreqMHz < HAL_DSI_PLL_CLOCK_INVERTORS_FREQ_LEVEL_2) ?   HAL_DSI_PLL_CLOCK_INVERTORS_VALUE_2:
                           (( pPllConfig->fActualFreqMHz < HAL_DSI_PLL_CLOCK_INVERTORS_FREQ_LEVEL_3) ? HAL_DSI_PLL_CLOCK_INVERTORS_VALUE_3 : HAL_DSI_PLL_CLOCK_INVERTORS_VALUE_4)));
  }

  /* Analog Controls Five */
  DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FIVE_1_ADDR, uAnalogControlsFive);
  DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FIVE_ADDR, uAnalogControlsFive);

  /* VCO Config */
  DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_PLL_VCO_CONFIG_1_ADDR, uVCOConfig);

  /* Clock inverters */
  DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_PLL_CLOCK_INVERTERS_1_ADDR, uClockInverters);

  /* cmode */
  DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_PLL_CMODE_1_ADDR,                        0x00000010);

  /* Frequency independent registers */
  DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_PLL_GEAR_BAND_SELECT_CONTROLS_ADDR,    0x00000021);
  DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_TWO_ADDR,          0x00000003);
  DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_THREE_ADDR,        0x00000000);
  DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_PLL_DSM_DIVIDER_ADDR,                  0x00000000);
  DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_PLL_FEEDBACK_DIVIDER_ADDR,             0x0000004E);
  DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_PLL_CALIBRATION_SETTINGS_ADDR,         0x00000040);
  DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_THREE_ADDR,  0x000000BA);
  DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_DETECT_SETTINGS_ONE_ADDR,     0x0000000C);
  DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_PLL_OUTDIV_ADDR,                       0x00000000);
  DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_PLL_CORE_OVERRIDE_ADDR,                0x00000000);
  DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_DIGITAL_TIMERS_TWO_ADDR,       0x00000008);
  DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_PROP_GAIN_RATE_1_ADDR,         0x0000000A);
  DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_BAND_SEL_RATE_1_ADDR,          0x000000C0);
  DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_INT_GAIN_IFILT_BAND_1_ADDR,    0x00000082);
  DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_FL_INT_GAIN_PFILT_BAND_1_ADDR, 0x0000004C);
  DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_LOCK_OVERRIDE_ADDR,            0x00000080);
  DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_PLL_PFILT_ADDR,                        0x0000002F);
  DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_PLL_IFILT_ADDR,                        0x0000003F);

  if (uMinor == 1)
  {
     DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_PLL_PERF_OPTIMIZE_ADDR,             0x00000022);
  }
}

/****************************************************************************
*
** FUNCTION: HAL_DSI_Phy_ResetPhy()
*/
/*!
* \brief
* 
*
* \param [in]  eDeviceId - DSI core ID
*
* \retval HAL_MDSS_ErrorType
*
****************************************************************************/
static void HAL_DSI_Phy_ResetPhy(DSI_Device_IDType eDeviceId)
{
  uintPtr uOffset = HAL_DSI_GetRegBaseOffset(eDeviceId);

  DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_1_ADDR, HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_1_DSIPHY_PLL_SW_RESET_BMSK);
  DSI_OSAL_SleepUs(1000);          /* delay ~1ms */
  DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_1_ADDR, 0x0);

  return;
}


/****************************************************************************
*
** FUNCTION: HAL_DSI_Phy_LaneMapConfig()
*/
/*!
* \brief
*     Configure the Lane Map
*
* \param [in]  eDeviceId     - DSI core ID
* \param [in]  paDisplayLane - pointer to lane map array
*
* \retval None
*
****************************************************************************/
static HAL_MDSS_ErrorType  HAL_DSI_Phy_LaneMapConfig(DSI_Device_IDType  eDeviceID,
                                                     DSI_LaneMapType    *paDisplayLane)
{
  HAL_MDSS_ErrorType  eStatus           = HAL_MDSS_STATUS_SUCCESS;
  uintPtr             uOffset           = HAL_DSI_GetRegBaseOffset(eDeviceID);
  uint32              uLaneSwapCtrlReg  = 0;
  uint32              uLaneMask[4];
  uint32              i;

  /* 
  * Convert lane numbers to bit masks 
  * # -> Mask(binary)
  * 1 -> 0001             
  * 2 -> 0010
  * 3 -> 0010
  * 4 -> 1000
  */
  for (i = 0; i < 4; i++)
  {
    if ((DSI_LaneMap_NONE < paDisplayLane[i]) && (DSI_LaneMap_MAX > paDisplayLane[i]))
    {
      uLaneMask[i] = 1 << (paDisplayLane[i] - 1);
    }
    else
    {
      uLaneMask[i] = 0;
    }
  }

  /* Lane cfg0 */
  uLaneSwapCtrlReg = HWIO_OUT_FLD(uLaneSwapCtrlReg, DSI_0_PHY_DSIPHY_CMN_LANE_CFG0, LOGICAL_LANE0_SEL, uLaneMask[0]);
  uLaneSwapCtrlReg = HWIO_OUT_FLD(uLaneSwapCtrlReg, DSI_0_PHY_DSIPHY_CMN_LANE_CFG0, LOGICAL_LANE1_SEL, uLaneMask[1]);
  DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_CMN_LANE_CFG0_ADDR, uLaneSwapCtrlReg);

  /* Lane cfg1 */
  uLaneSwapCtrlReg = 0;
  uLaneSwapCtrlReg = HWIO_OUT_FLD(uLaneSwapCtrlReg, DSI_0_PHY_DSIPHY_CMN_LANE_CFG1, LOGICAL_LANE2_SEL, uLaneMask[2]);
  uLaneSwapCtrlReg = HWIO_OUT_FLD(uLaneSwapCtrlReg, DSI_0_PHY_DSIPHY_CMN_LANE_CFG1, LOGICAL_LANE3_SEL, uLaneMask[3]);
  DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_CMN_LANE_CFG1_ADDR, uLaneSwapCtrlReg);

  return eStatus;
}


/****************************************************************************
*
** FUNCTION: HAL_DSI_Phy_LaneConfig()
*/
/*!
* \brief
*     Configure the Lane Map
*
* \param [in]  eDeviceId     - DSI core ID
* \param [in]  paDisplayLane - pointer to lane map array
*
* \retval HAL_MDSS_ErrorType
*
****************************************************************************/
HAL_MDSS_ErrorType HAL_DSI_Phy_LaneConfig(DSI_Device_IDType           eDeviceID,
                                          HAL_DSI_LanePropertyType   *psLaneProperty)
{
  HAL_MDSS_ErrorType  eStatus    = HAL_MDSS_STATUS_SUCCESS;
  uint32              uLaneCtrl1 = 0;
  uint32              uLaneCtrl2 = 0;
  uint32              uLaneCtrl3 = 0;
  uintPtr             uOffset    = HAL_DSI_GetRegBaseOffset(eDeviceID);

  /* Check input Parameters */
  if (NULL == psLaneProperty)
  {
    eStatus = HAL_MDSS_DSI_FAILED_EXPECTED_NON_NULL_PTR;
  }
  else
  {
    uLaneCtrl1 = in_dword(uOffset + HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL1_ADDR);
    uLaneCtrl2 = in_dword(uOffset + HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL2_ADDR);
    uLaneCtrl3 = in_dword(uOffset + HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL3_ADDR);

    /* Program clock lane */
    switch (psLaneProperty->eClockLaneOp)
    {
      case DSI_LANE_OP_ULPS_REQUEST:
      {
        uLaneCtrl1 = HWIO_OUT_FLD(uLaneCtrl1, DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL1, CLKLN_ULPS_REQUEST, 1);
      }
        break;

      case DSI_LANE_OP_ULPS_REQUEST_CLEAR:
      {
        uLaneCtrl1 = HWIO_OUT_FLD(uLaneCtrl1, DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL1, CLKLN_ULPS_REQUEST, 0);
      }
        break;

      case DSI_LANE_OP_CLOCK_HS_FORCE_REQUEST:
      {
        uLaneCtrl1 = HWIO_OUT_FLD(uLaneCtrl1, DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL1, DPHY_CONTINUOUS_CLK_MODE, 1);
        uLaneCtrl1 = HWIO_OUT_FLD(uLaneCtrl1, DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL1, DPHY_CONTNSCLKMODE_CNCK_TX_RQST_HS, 1);
      }
        break;

      case DSI_LANE_OP_CLOCK_HS_FORCE_REQUEST_CLEAR:
      {
        uLaneCtrl1 = HWIO_OUT_FLD(uLaneCtrl1, DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL1, DPHY_CONTINUOUS_CLK_MODE, 0);
        uLaneCtrl1 = HWIO_OUT_FLD(uLaneCtrl1, DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL1, DPHY_CONTNSCLKMODE_CNCK_TX_RQST_HS, 0);
      }
        break;

      case DSI_LANE_OP_ULPS_EXIT:
      {
        uLaneCtrl2 = HWIO_OUT_FLD(uLaneCtrl2, DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL2, CLKLN_ULPS_EXIT, 1);
      }
        break;

      case DSI_LANE_OP_ULPS_EXIT_CLEAR:
      {
        uLaneCtrl2 = HWIO_OUT_FLD(uLaneCtrl2, DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL2, CLKLN_ULPS_EXIT, 0);
      }
        break;

      case DSI_LANE_OP_FORCE_TX_STOP:
      {
        uLaneCtrl3 = HWIO_OUT_FLD(uLaneCtrl3, DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL3, CLKLN_FORCE_TX_STOP, 1);
      }
        break;

      case DSI_LANE_OP_FORCE_TX_STOP_CLEAR:
      {
        uLaneCtrl3 = HWIO_OUT_FLD(uLaneCtrl3, DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL3, CLKLN_FORCE_TX_STOP, 0);
      }
        break;

      default:
        break;
    }

    /* Program data lane */
    switch (psLaneProperty->eDataLaneOp)
    {
      case DSI_LANE_OP_ULPS_REQUEST:
      {
        uLaneCtrl1 = HWIO_OUT_FLD(uLaneCtrl1, DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL1, DLN0_ULPS_REQUEST,  1);
        uLaneCtrl1 = HWIO_OUT_FLD(uLaneCtrl1, DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL1, DLN1_ULPS_REQUEST,  1);
        uLaneCtrl1 = HWIO_OUT_FLD(uLaneCtrl1, DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL1, DLN2_ULPS_REQUEST,  1);
        uLaneCtrl1 = HWIO_OUT_FLD(uLaneCtrl1, DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL1, DLN3_ULPS_REQUEST,  1);
      }
        break;

      case DSI_LANE_OP_ULPS_REQUEST_CLEAR:
      {
        uLaneCtrl1 = HWIO_OUT_FLD(uLaneCtrl1, DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL1, DLN0_ULPS_REQUEST,  0);
        uLaneCtrl1 = HWIO_OUT_FLD(uLaneCtrl1, DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL1, DLN1_ULPS_REQUEST,  0);
        uLaneCtrl1 = HWIO_OUT_FLD(uLaneCtrl1, DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL1, DLN2_ULPS_REQUEST,  0);
        uLaneCtrl1 = HWIO_OUT_FLD(uLaneCtrl1, DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL1, DLN3_ULPS_REQUEST,  0);
      }
        break;

      case DSI_LANE_OP_ULPS_EXIT:
      {
        uLaneCtrl2 = HWIO_OUT_FLD(uLaneCtrl2, DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL2, DLN0_ULPS_EXIT,     1);
        uLaneCtrl2 = HWIO_OUT_FLD(uLaneCtrl2, DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL2, DLN1_ULPS_EXIT,     1);
        uLaneCtrl2 = HWIO_OUT_FLD(uLaneCtrl2, DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL2, DLN2_ULPS_EXIT,     1);
        uLaneCtrl2 = HWIO_OUT_FLD(uLaneCtrl2, DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL2, DLN3_ULPS_EXIT,     1);
      }
        break;

      case DSI_LANE_OP_ULPS_EXIT_CLEAR:
      {
        uLaneCtrl2 = HWIO_OUT_FLD(uLaneCtrl2, DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL2, DLN0_ULPS_EXIT,     0);
        uLaneCtrl2 = HWIO_OUT_FLD(uLaneCtrl2, DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL2, DLN1_ULPS_EXIT,     0);
        uLaneCtrl2 = HWIO_OUT_FLD(uLaneCtrl2, DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL2, DLN2_ULPS_EXIT,     0);
        uLaneCtrl2 = HWIO_OUT_FLD(uLaneCtrl2, DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL2, DLN3_ULPS_EXIT,     0);
      }
        break;

      case DSI_LANE_OP_FORCE_TX_STOP:
      {
        uLaneCtrl3 = HWIO_OUT_FLD(uLaneCtrl3, DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL3, DLN0_FORCE_TX_STOP, 1);
        uLaneCtrl3 = HWIO_OUT_FLD(uLaneCtrl3, DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL3, DLN1_FORCE_TX_STOP, 1);
        uLaneCtrl3 = HWIO_OUT_FLD(uLaneCtrl3, DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL3, DLN2_FORCE_TX_STOP, 1);
        uLaneCtrl3 = HWIO_OUT_FLD(uLaneCtrl3, DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL3, DLN3_FORCE_TX_STOP, 1);
      }
        break;

      case DSI_LANE_OP_FORCE_TX_STOP_CLEAR:
      {
        uLaneCtrl3 = HWIO_OUT_FLD(uLaneCtrl3, DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL3, DLN0_FORCE_TX_STOP, 0);
        uLaneCtrl3 = HWIO_OUT_FLD(uLaneCtrl3, DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL3, DLN1_FORCE_TX_STOP, 0);
        uLaneCtrl3 = HWIO_OUT_FLD(uLaneCtrl3, DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL3, DLN2_FORCE_TX_STOP, 0);
        uLaneCtrl3 = HWIO_OUT_FLD(uLaneCtrl3, DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL3, DLN3_FORCE_TX_STOP, 0);
      }
        break;

      default:
        break;
    }
    DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL1_ADDR, uLaneCtrl1);
    DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL2_ADDR, uLaneCtrl2);
    DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL3_ADDR, uLaneCtrl3);
  }
  return eStatus;
}

/****************************************************************************
*
** FUNCTION: HAL_DSI_PhyGetLaneStatus()
*/
/*!
* \brief
*    Query the lane status information for a particular DSI device.
*
* \param [in]  eDeviceID    - DSI core device ID 
* \param [out] puDsiStatus  - Device lane status information is returned
*
* \retval void
*
****************************************************************************/
HAL_MDSS_ErrorType HAL_DSI_PhyGetLaneStatus(DSI_Device_IDType   eDeviceID,
                                            uint32             *puDsiStatus)
{
  HAL_MDSS_ErrorType  eStatus      = HAL_MDSS_STATUS_SUCCESS;
  uintPtr             uOffset      = HAL_DSI_GetRegBaseOffset(eDeviceID);

  /* Valid parameters */
  if (NULL == puDsiStatus)
  {
    eStatus = HAL_MDSS_DSI_FAILED_EXPECTED_NON_NULL_PTR;
  }
  else
  {
    uint32 uLaneStatus0Reg  = 0;
    uint32 uLaneStatus1Reg  = 0;

    *puDsiStatus   =  0;

    /* Get the HW generated DSI Status. */
    uLaneStatus0Reg = in_dword(uOffset + HWIO_DSI_0_PHY_DSIPHY_CMN_LANE_STATUS0_ADDR);
    uLaneStatus1Reg = in_dword(uOffset + HWIO_DSI_0_PHY_DSIPHY_CMN_LANE_STATUS1_ADDR);

    /* Data lane 0 ULPS state */
    if (0 != HWIO_GETVAL(DSI_0_PHY_DSIPHY_CMN_LANE_STATUS0, uLaneStatus0Reg, DLN0_ULPS_ACTIVE_NOT))
    {
      *puDsiStatus |= DSI_LANE_STATUS_DLN0_ULPS_NOT_ACTIVE;
    }

    /* Data lane 1 ULPS state */
    if (0 != HWIO_GETVAL(DSI_0_PHY_DSIPHY_CMN_LANE_STATUS0, uLaneStatus0Reg, DLN1_ULPS_ACTIVE_NOT))
    {
      *puDsiStatus |= DSI_LANE_STATUS_DLN1_ULPS_NOT_ACTIVE;
    }

    /* Data lane 2 ULPS state */
    if (0 != HWIO_GETVAL(DSI_0_PHY_DSIPHY_CMN_LANE_STATUS0, uLaneStatus0Reg, DLN2_ULPS_ACTIVE_NOT))
    {
      *puDsiStatus |= DSI_LANE_STATUS_DLN2_ULPS_NOT_ACTIVE;
    }

    /* Data lane 3 ULPS state */
    if (0 != HWIO_GETVAL(DSI_0_PHY_DSIPHY_CMN_LANE_STATUS0, uLaneStatus0Reg, DLN3_ULPS_ACTIVE_NOT))
    {
      *puDsiStatus |= DSI_LANE_STATUS_DLN3_ULPS_NOT_ACTIVE;
    }

    /* Clock lane ULPS state */
    if (0 != HWIO_GETVAL(DSI_0_PHY_DSIPHY_CMN_LANE_STATUS0, uLaneStatus0Reg, CLKLN_ULPS_ACTIVE_NOT))
    {
      *puDsiStatus |= DSI_LANE_STATUS_CLKLN_ULPS_NOT_ACTIVE;
    }

    /* Data lane 0 state */
    if (0 != HWIO_GETVAL(DSI_0_PHY_DSIPHY_CMN_LANE_STATUS1, uLaneStatus1Reg, DLN0_STOPSTATE))
    {
      *puDsiStatus |= DSI_LANE_STATUS_DLN0_STOPSTATE;
    }

    /* Data lane 1 state */
    if (0 != HWIO_GETVAL(DSI_0_PHY_DSIPHY_CMN_LANE_STATUS1, uLaneStatus1Reg, DLN1_STOPSTATE))
    {
      *puDsiStatus |= DSI_LANE_STATUS_DLN1_STOPSTATE;
    }

    /* Data lane 2 state */
    if (0 != HWIO_GETVAL(DSI_0_PHY_DSIPHY_CMN_LANE_STATUS1, uLaneStatus1Reg, DLN2_STOPSTATE))
    {
      *puDsiStatus |= DSI_LANE_STATUS_DLN2_STOPSTATE;
    }

    /* Data lane 3 state */
    if (0 != HWIO_GETVAL(DSI_0_PHY_DSIPHY_CMN_LANE_STATUS1, uLaneStatus1Reg, DLN3_STOPSTATE))
    {
      *puDsiStatus |= DSI_LANE_STATUS_DLN3_STOPSTATE;
    }

    /* Clock lane state */
    if (0 != HWIO_GETVAL(DSI_0_PHY_DSIPHY_CMN_LANE_STATUS1, uLaneStatus1Reg, CLKLN_STOPSTATE))
    {
      *puDsiStatus |= DSI_LANE_STATUS_CLKLN_STOPSTATE;
    }
  }
  return eStatus;
}

/****************************************************************************
*
** FUNCTION: HAL_DSI_Phy_yValue()
*/
/*!
* \brief
*   this is for C-Phy only
*
* \retval float
*
****************************************************************************/
float  HAL_DSI_Phy_yValue(HAL_DSI_PhyConfigType *psDsiPhyConfig)
{
  float yVal = 0.0f;

  if (psDsiPhyConfig->uDsiClkSel == 3)
  {
     yVal = psDsiPhyConfig->uPhyPostDiv / 3.5f;
  }
  else if (psDsiPhyConfig->uDsiClkSel == 2)
  {
     yVal = psDsiPhyConfig->uPhyPostDiv;
  }
  else if (psDsiPhyConfig->uDsiClkSel == 1)
  {
     yVal = 0.5f;
  }
  else if (psDsiPhyConfig->uDsiClkSel == 0)
  {
     yVal = 1.0f;
  }

  return yVal; 
}

/****************************************************************************
*
** FUNCTION: HAL_DSI_Phy_DsiClk_Divider()
*/
/*!
* \brief
*     Calculate Pixel Clock's Source Clock Divider (Div_ctrl_7_4).
*
* \param [in]  psDsiPhyConfig       - PHY config info
* \param [out] psDsiPhyConfigInfo   - Phy & PLL config pass back info
*
* \retval uint32
*
****************************************************************************/
uint32  HAL_DSI_Phy_DsiClk_Divider(HAL_DSI_PhyConfigType       *psDsiPhyConfig, 
                                   HAL_DSI_PhyConfigInfoType   *psDsiPhyConfigInfo)
{
  uint32 uM_Val = psDsiPhyConfigInfo->uPClkDivNumerator;
  uint32 uN_Val = psDsiPhyConfigInfo->uPClkDivDenominator;
  uint32 uDivCtrl;
  uint32 bpp = psDsiPhyConfig->uBitsPerPixel / 8;
  float  yVal;
  float  fVal;


  if (psDsiPhyConfig->bCPhyMode) /* C-phy */
  {
    yVal = HAL_DSI_Phy_yValue(psDsiPhyConfig);
    fVal = (((((float)uM_Val / uN_Val) * 7 * bpp) / 
              (psDsiPhyConfig->uNumOfDataLanes * 2)) * yVal);

    uDivCtrl = HAL_DSI_Phy_Rounddown(fVal);
    if (uDivCtrl > 15)
    {
      uDivCtrl = 15;
    }
    else if (uDivCtrl < 1)
    {
      uDivCtrl = 1;
    }
  }
  else
  {
    fVal = (float)(uM_Val * psDsiPhyConfig->uBitsPerPixel) / (float)(uN_Val * psDsiPhyConfig->uNumOfDataLanes * 2);
    if (psDsiPhyConfig->uBitsPerPixel >= 16)
    {
      uDivCtrl = (uint32)fVal;
    }
    else
    {
      uDivCtrl = HAL_DSI_Phy_Rounddown(fVal);
    }

  }

  return uDivCtrl;

}

/****************************************************************************
*
** FUNCTION: HAL_DSI_Phy_DsiCLkSel()
*/
/*!
* \brief
*     Select Pixel Clock Source Clock.
*
* \param [in]  psDsiPhyConfig       - PHY config info
* \param [out] psDsiPhyConfigInfo   - Phy & PLL config pass back info
*
*
****************************************************************************/
void  HAL_DSI_Phy_DsiClk_Sel(HAL_DSI_PhyConfigType       *psDsiPhyConfig, 
                              HAL_DSI_PhyConfigInfoType   *psDsiPhyConfigInfo)
{     
  uint32 dsiClkSel = 0;

  if (psDsiPhyConfig->bCPhyMode) /* C-phy */
  {
    if (psDsiPhyConfig->uBitsPerPixel == 24)
    {
      dsiClkSel = 3;
    }
    else if ((psDsiPhyConfig->uBitsPerPixel == 16) &&
             (psDsiPhyConfig->uNumOfDataLanes == 2))
    {
      dsiClkSel = 3;
    }
  }
  else   /* D-Phy */
  {
    dsiClkSel = 1;
  }

  psDsiPhyConfig->uDsiClkSel = dsiClkSel;
}

/****************************************************************************
*
** FUNCTION: HAL_DSI_Phy_MND_Divider()
*/
/*!
* \brief
*     Calculate Pixel Clock MND Divider.
*
* \param [in]  psDsiPhyConfig       - PHY config info
* \param [out] psDsiPhyConfigInfo   - Phy & PLL config pass back info
*
* \retval uint32
*
****************************************************************************/
void  HAL_DSI_Phy_MND_Divider(HAL_DSI_PhyConfigType       *psDsiPhyConfig, 
                              HAL_DSI_PhyConfigInfoType   *psDsiPhyConfigInfo)
{     
  uint32    uM_Val              = 1;
  uint32    uN_Val              = 1;

  if (psDsiPhyConfig->bCPhyMode) /* C-phy */
  {
    if (psDsiPhyConfig->uBitsPerPixel == 18)
    {
      switch (psDsiPhyConfig->uNumOfDataLanes)
      {
      case 1:
        uM_Val = 8;
        uN_Val = 63;
        break;
      case 2:
        uM_Val = 16;
        uN_Val = 63;
        break;
      case 3:
        uM_Val = 8;
        uN_Val = 21;
        break;
      default:
        break;
      }
    }
    else if ((psDsiPhyConfig->uBitsPerPixel == 16) &&
             (psDsiPhyConfig->uNumOfDataLanes == 3))
    {
      uM_Val = 3;
      uN_Val = 7;
    }
    else if ((psDsiPhyConfig->uBitsPerPixel == 24) &&
             (psDsiPhyConfig->uNumOfDataLanes == 2))
    {
      uM_Val = 2;
      uN_Val = 3;
    }
    else if (psDsiPhyConfig->uBitsPerPixel == 30)
    {
      switch (psDsiPhyConfig->uNumOfDataLanes)
      {
      case 1:
        uM_Val = 4;
        uN_Val = 15;
        break;
      case 2:
      case 3:
        uM_Val = 16;
        uN_Val = 35;
        break;
      default:
        break;
      }
    }
  }
  else      /* D-Phy */
  {
    if (psDsiPhyConfig->uBitsPerPixel == 18)
    {
      switch (psDsiPhyConfig->uNumOfDataLanes)
      {
      case 1:
      case 2:
        uM_Val = 2;
        uN_Val = 9;
        break;
      case 4:
        uM_Val = 4;
        uN_Val = 9;
        break;

      default:
        break;
      }
    }
    else if ((psDsiPhyConfig->uBitsPerPixel == 16) &&
             (psDsiPhyConfig->uNumOfDataLanes == 3))
    {
      uM_Val = 3;
      uN_Val = 8;
    }
    else if ((psDsiPhyConfig->uBitsPerPixel == 30) &&
             (psDsiPhyConfig->uNumOfDataLanes == 4))
    {
       uM_Val = 2;
       uN_Val = 3;
    }
  }

  /*Save M/N info */
  psDsiPhyConfigInfo->uPClkDivNumerator   = uM_Val;
  psDsiPhyConfigInfo->uPClkDivDenominator = uN_Val;

}

/****************************************************************************
*
** FUNCTION: HAL_DSI_Phy_CalcPixelClockDivider()
*/
/*!
* \brief
*     Calculate Pixel Clock divider.
*
* \param [in]  psDsiPhyConfig       - PHY config info
* \param [in]  psPllConfig          - PLL config info
* \param [out] psDsiPhyConfigInfo   - Phy & PLL config pass back info
*
* \retval uint32
*
****************************************************************************/
float  HAL_DSI_Phy_CalcClockDivider(HAL_DSI_PhyConfigType       *psDsiPhyConfig, 
                                    HAL_DSI_PHY_PllConfigType   *psPllConfig,
                                    HAL_DSI_PhyConfigInfoType   *psDsiPhyConfigInfo)
{     
  /***************************************************************************************
  * Notes:
  *    MMSS DSI PCLK MND
  *         = 2/9, if RGB666 packed and 2 lanes
  *         = 3/8, if RGB565 and 3 lanes
  *         = 4/9, if RGB666 packed and 4 lanes
  *         = 1/1, for other cases
  *   Bitclk_divider
  *         - Clock division ratio required for the divider controlled by DIV_CTRL_3_0
  *   VCO clock = full-rate-bitclk * Bitclk_divider
  *   Dsiclk_sel
  *         = 0, if RGB111 and 3/4 lanes
  *         = 1, for other cases
  *   Dsiclk_divider
  *         - Clock division ratio required for the divider controlled by DIV_CTRL_7_4
  *         - (MND * 8 * byte_per_pixel/lane/(dsiclk_sel+1)), if byte_per_pixel >= 2
  *         - rounddown(MND * 8 * byte_per_pixel/lane/(dsiclk_sel+1)), for other cases
  ***************************************************************************************/
  /* init numerator and denominator of divider ratio in CC to 1 */
  int32     i;
  uintPtr   uOffset             = HAL_DSI_GetRegBaseOffset(psDsiPhyConfig->eDeviceId);
  uint32    uM_Val              = 1;
  uint32    uN_Val              = 1;
  uint32    uDivCtrl            = 0;
  uint32    uRegVal             = 0;
  uint32    uPllPostDiv         = 0;
  uint32    uPhyPostDiv         = 0;
  bool32    bDivFound           = FALSE;
  float     fVCOFreqMHz         = 0.0f;
  float     fVal                = 0.0f;
  float     fPllOutputFreqMHz;
  float     fDesiredBitclockMHz;
  uint32    lut_max;
  HAL_DSI_PHY_DividerLUTEntryType *lut;
  
  HAL_DSI_Phy_MND_Divider(psDsiPhyConfig, psDsiPhyConfigInfo);

  uM_Val = psDsiPhyConfigInfo->uPClkDivNumerator;
  uN_Val = psDsiPhyConfigInfo->uPClkDivDenominator;

  /* Desired clock in MHz */
  fDesiredBitclockMHz = (float)psDsiPhyConfig->uDesiredBitClkFreq / 1000000.0f;

  if (psDsiPhyConfig->bCPhyMode)
  {
    /* C Phy */
    /* 7 symbols per 16 bits*/
    fDesiredBitclockMHz *= 7;
    fDesiredBitclockMHz /= 16;
    lut = asPllDividerLUT_CPhy;
    lut_max = sizeof(asPllDividerLUT_CPhy) / sizeof(*lut);
    lut += (lut_max - 1);
  }
  else 
  {
    /* D Phy */
    lut = asPllDividerLUT_DPhy;
    lut_max = sizeof(asPllDividerLUT_DPhy) / sizeof(*lut);
    lut += (lut_max - 1);
  }

  /*
  * PLL Post Div - from LUT
  * Check the LUT in reverse order
  */
  for (i = lut_max - 1; i >= 0; i--, lut--)
  {
    fVal = lut->uPhyPostDiv * lut->uPllPostDiv;

    if (fVal)
    {
      fVal = HAL_DSI_PLL_VCO_MIN_MHZ / fVal;

      if (fDesiredBitclockMHz > fVal)
      {
        /* Range found */
        uPllPostDiv = lut->uPllPostDiv;
        uPhyPostDiv = lut->uPhyPostDiv;
        bDivFound = TRUE;
        break;
      }
    }
  }

  if (bDivFound)
  {
    psPllConfig->uPllPostDiv = uPllPostDiv;
    psDsiPhyConfig->uPllPostDiv = uPllPostDiv;
    psDsiPhyConfig->uPhyPostDiv = uPhyPostDiv;

    /* Div Ctrl*/
    HAL_DSI_Phy_DsiClk_Sel(psDsiPhyConfig, psDsiPhyConfigInfo);
    uDivCtrl = HAL_DSI_Phy_DsiClk_Divider(psDsiPhyConfig, psDsiPhyConfigInfo); /*div_ctrl_7_4 */

    /* DIV_CTRL_7_4 DIV_CTRL_3_0  (DIV_CTRL_3_0 = PHY post divider ratio) */
    uRegVal = 0;
    uRegVal = HWIO_OUT_FLD(uRegVal, DSI_0_PHY_DSIPHY_CMN_CLK_CFG0, DIV_CTRL_7_4, uDivCtrl);

    if ((HAL_DSI_PLL_CONFIG_SPLIT_SOURCE == psDsiPhyConfig->ePLLConfigSource) &&
      (DSI_DeviceID_1 == psDsiPhyConfig->eDeviceId))
    {
      /* For slave node PHY post div is forced to 1 */
      uRegVal = HWIO_OUT_FLD(uRegVal, DSI_0_PHY_DSIPHY_CMN_CLK_CFG0, DIV_CTRL_3_0, 1);
    }
    else
    {
      uRegVal = HWIO_OUT_FLD(uRegVal, DSI_0_PHY_DSIPHY_CMN_CLK_CFG0, DIV_CTRL_3_0, uPhyPostDiv);
    }
    DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_CMN_CLK_CFG0_ADDR, uRegVal);


    /* PLL output frequency = fDesiredBitclockMHz * uPhyPostDiv */
    fPllOutputFreqMHz = fDesiredBitclockMHz * uPhyPostDiv;

    /* VCO output freq*/
    fVCOFreqMHz       = fPllOutputFreqMHz * uPllPostDiv;

  }

  return fVCOFreqMHz;
}

/* -----------------------------------------------------------------------
** Public functions
** ----------------------------------------------------------------------- */

/****************************************************************************
*
** FUNCTION: HAL_DSI_2_3_0_PhyDisable()
*/
/*!
* \brief
*     Disables DSI Phy.
*
* \param [in]   eDeviceId   - DSI core ID
*
* \retval None
*
****************************************************************************/
void HAL_DSI_2_3_0_PhyDisable(DSI_Device_IDType   eDeviceId)
{
  uintPtr uOffset = HAL_DSI_GetRegBaseOffset(eDeviceId);

  /* Disable PLL */
  HAL_DSI_Phy_PllEnable(uOffset, FALSE);

  /* PLL block power down */
  DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_0_ADDR, 0);
}

/****************************************************************************
*
** FUNCTION: HAL_DSI_2_3_0_PhyPllEnable()
*/
/*!
* \brief
*     Enable or Disable Pll
*
* \param [in]   eDeviceId   - DSI core ID
* \param [in]   bEnable     - true to enable pll and false to disable pll

*
* \retval None
*
****************************************************************************/
void HAL_DSI_2_3_0_PhyPllEnable(DSI_Device_IDType eDeviceId, bool32 bEnable)
{
  HAL_DSI_Phy_PllEnable(eDeviceId, bEnable);
}

/****************************************************************************
*
** FUNCTION: HAL_DSI_2_3_0_PhyPllPowerCtrl()
*/
/*!
* \brief
*     Power up/down PLL, LDO and powergen.
*
*
* \param [in]  eDeviceId    - DSI core ID
* \param [in]  bPllPowerUp  - TRUE: power up, FALSE: power down;
*
* \retval HAL_MDSS_ErrorType
*
****************************************************************************/
HAL_MDSS_ErrorType HAL_DSI_2_3_0_PhyPllPowerCtrl(DSI_Device_IDType   eDeviceId,
                                                 bool32              bPllPowerUp)
{
  HAL_MDSS_ErrorType   eStatus     = HAL_MDSS_STATUS_SUCCESS;
  /*
  * Moved the implementation to HAL_DSI_2_0_0_PhyPllSetup to enable programming PHY and PLL
  * together
  */

  return eStatus;
}


/****************************************************************************
*
** FUNCTION: HAL_DSI_2_3_0_PhySetup()
*/
/*!
* \brief
*     Set up DSI Phy alone.
*
* \param [in]  eDeviceId          - DSI core ID
* \param [IN]  psDsiPhyConfig     - Phy configuration
*
* \retval HAL_MDSS_ErrorType
*
****************************************************************************/
HAL_MDSS_ErrorType HAL_DSI_2_3_0_PhySetup(DSI_Device_IDType          eDeviceId,
                                          HAL_DSI_PhyConfigType     *psDsiPhyConfig)
{
  HAL_MDSS_ErrorType          eStatus         = HAL_MDSS_STATUS_SUCCESS;
  HAL_DSI_PHY_PhySettingsType sPhySettings;


  DSI_OSAL_MemZero(&sPhySettings, sizeof(HAL_DSI_PHY_PhySettingsType));

  /* Lane Settings */
  sPhySettings.bCPhyMode          = psDsiPhyConfig->bCPhyMode;
  sPhySettings.uDesiredBitClkFreq = psDsiPhyConfig->uDesiredBitClkFreq;

  /* Program HW */
  eStatus = HAL_DSI_Phy_CommitPhy(eDeviceId, &sPhySettings);

  return eStatus;
}

/****************************************************************************
*
** FUNCTION: HAL_DSI_2_3_0_PhyPllSetup()
*/
/*!
* \brief
*     Set up DSI PLL, pass back some config parameters, such as VCO output frequency, 
*     PCLK divider ratio for CC in the form of numerator and denominator, etc.
*
* \param [in]   psDsiPhyConfig     - Phy config info
* \param [out]  psDsiPhyConfigInfo - Phy & PLL config pass back info
*
* \retval HAL_MDSS_ErrorType
*
****************************************************************************/
HAL_MDSS_ErrorType HAL_DSI_2_3_0_PhyPllSetup(HAL_DSI_PhyConfigType       *psDsiPhyConfig,
                                             HAL_DSI_PhyConfigInfoType   *psDsiPhyConfigInfo)
{
  HAL_MDSS_ErrorType        eStatus           = HAL_MDSS_STATUS_SUCCESS;
  DSI_Device_IDType         eDeviceId         = psDsiPhyConfig->eDeviceId;

  /* validate input parameters */
  if ((eDeviceId != DSI_DeviceID_0) &&
      (eDeviceId != DSI_DeviceID_1))
  {
    eStatus = HAL_MDSS_DSI_FAILED_INVALID_INPUT_PARAMETER;
  }
  else if ((psDsiPhyConfig->uBitsPerPixel != 16) &&
           (psDsiPhyConfig->uBitsPerPixel != 18) &&
           (psDsiPhyConfig->uBitsPerPixel != 24))
  {
    /* Unsupported pixel bit depth */
    eStatus = HAL_MDSS_DSI_FAILED_INVALID_INPUT_PARAMETER;
  }
  else if ((psDsiPhyConfig->uNumOfDataLanes == 0) ||
           (psDsiPhyConfig->uNumOfDataLanes > 4))
  {
    /* Illegal number of DSI data lanes */
    eStatus = HAL_MDSS_DSI_FAILED_INVALID_INPUT_PARAMETER;
  }
  else
  {
    float                       fActualFreqMHz;
    float                       fVCOFreqMHz;
    float                       fPllClkTarget;
    float                       fRefFreqMHz          = HAL_DSI_PLL_REF_FREQ_MHZ;
    float                       fDecFrac             = 0.0f;
    float                       fFrac                = 0.0f;
    float                       fVal                 = 0.0f;
    uint32                      uFrac                = 0;
    uint32                      uDec                 = 0;
    uint32                      uPropGain            = 0;
    uint32                      uSSCPERStart;
    uint32                      uSSCMod;
    uint32                      uSSCPER;
    HAL_DSI_PHY_PllConfigType   sPllConfig;

    DSI_OSAL_MemZero(&sPllConfig,  sizeof(HAL_DSI_PHY_PllConfigType));

    //Save Clock info

    psDsiPhyConfigInfo->uBitClkFreq = psDsiPhyConfig->uDesiredBitClkFreq;

    /* Ref clk after prescaler */
    fRefFreqMHz = fRefFreqMHz * HAL_DSI_PLL_PRESCALE_FACTOR;

    fVCOFreqMHz = HAL_DSI_Phy_CalcClockDivider(psDsiPhyConfig, &sPllConfig, psDsiPhyConfigInfo);

    if (0.0f == fVCOFreqMHz)
    {
      /* bitclock too low  - unsupported */
      eStatus = HAL_MDSS_DSI_FAILED_INVALID_INPUT_PARAMETER;
    }
    else
    {
      /* Enable PLL bias */
      HAL_DSI_Phy_PllBiasEnable(psDsiPhyConfig->eDeviceId, TRUE);

      /* Set byte clk source */
      HAL_DSI_Phy_PllSetSource(psDsiPhyConfig);

      if ((HAL_DSI_PLL_CONFIG_SPLIT_SOURCE == psDsiPhyConfig->ePLLConfigSource) &&
          (DSI_DeviceID_1 == psDsiPhyConfig->eDeviceId))
      {
        uintPtr uOffset = HAL_DSI_GetRegBaseOffset(psDsiPhyConfig->eDeviceId);

        /* Split dsi mode needs set perf optimize register to 0x22 for both DSI0 and DSI1*/
        DSI_WR(uOffset + HWIO_DSI_0_PHY_DSIPHY_PLL_PERF_OPTIMIZE_ADDR, 0x00000022);

        /* Assert SLEEP_B signal */
        HAL_DSI_Phy_TriggerSleepB(psDsiPhyConfig->eDeviceId, TRUE);
          
        /* Global clock enable */
        HAL_DSI_Phy_PllGlobalClockEnable(psDsiPhyConfig->eDeviceId, TRUE);

        /* Resync FIFOs */
        HAL_DSI_Phy_ResyncFIFO(eDeviceId);

      }
      else
      {

        /* Prop Gain*/
        if (HAL_DSI_PLL_PARAM_1900F >= fVCOFreqMHz)
        {
          uPropGain = 8;
        }
        else if (HAL_DSI_PLL_PARAM_3000F >= fVCOFreqMHz)
        {
          uPropGain = 10;
        }
        else
        {
          uPropGain = 12;
        }

        sPllConfig.uPropGainRate = uPropGain;

        /* Clock multiplier ratio */
        fDecFrac = fVCOFreqMHz / fRefFreqMHz;

        /* Decimal part */
        uDec = HAL_DSI_Phy_Rounddown(fDecFrac);
        sPllConfig.uDecDivStart = uDec;

        /* Fractional part - 18 bits */
        fFrac = (fDecFrac - uDec) * (1 << HAL_DSI_PLL_FRAC_BITS);
        uFrac = HAL_DSI_Phy_Round(fFrac);

        /* Frac Low : bits 0-3 */
        sPllConfig.uFracDivLow = (uFrac & 0xF);

        /* Frac Mid : bits 8-15 */
        sPllConfig.uFracDivMid = (uFrac >> 8) & 0xFF;

        /* Frac High - bits 16-17 */
        sPllConfig.uFracDivHigh = (uFrac >> 16) & 0x3;

        /* Actual Frequency */
        fVal = uDec + ((float)uFrac) / (1 << HAL_DSI_PLL_FRAC_BITS);
        fActualFreqMHz = fVal * fRefFreqMHz;
        sPllConfig.fActualFreqMHz = fActualFreqMHz;

        /* SSC Starting PER */
        uSSCPERStart = HAL_DSI_PLL_REF_FREQ_MHZ * 1000 / (2 * HAL_DSI_PLL_SSC_FREQ_KHZ);

        /* SSC MOD */
        uSSCMod = (uSSCPERStart + 1) % (HAL_DSI_PLL_SSC_ADJ_PER + 1);

        /* SSC PER */
        uSSCPER = uSSCPERStart - uSSCMod;
        sPllConfig.uSSCPER = uSSCPER;

        /* SSC Adj PER */
        sPllConfig.uSSCAdjPER = HAL_DSI_PLL_SSC_ADJ_PER;

        /* SSC Step Size */
        fVal = (uDec * (1 << HAL_DSI_PLL_FRAC_BITS)) + uFrac;
        fVal = fVal * HAL_DSI_PLL_SSC_OFFSET_PPM / HAL_DSI_PLL_PARAM_1M;
        fVal = fVal * (HAL_DSI_PLL_SSC_ADJ_PER + 1) / (uSSCPER + 1);
        sPllConfig.uSSCStepSize = HAL_DSI_Phy_Round(fVal);

        /* Freq detect threshold */
        sPllConfig.uFreqDetThresh = HAL_DSI_PLL_THRESHOLD_CYCLES;

        /* Refclk cycles */
        sPllConfig.uFreqDetRefclk = HAL_DSI_PLL_REFCLK_CYCLES - 1;

        /* PLL Clk Target */
        fPllClkTarget = (fActualFreqMHz * HAL_DSI_PLL_REFCLK_CYCLES) / (8 * fRefFreqMHz);
        sPllConfig.uFreqDetRefclk = (HAL_DSI_Phy_Round(fPllClkTarget) - 1);

        /* lockdet */
        sPllConfig.uLockDetRate = HAL_DSI_PLL_LOCK_TIMER;

        /* Lock delay */
        sPllConfig.uLockDelay = HAL_DSI_PLL_PLL_LOCK_DELAY;

        /* Error */
        sPllConfig.fPllErrorPPM = (1 + HAL_DSI_PLL_THRESHOLD_CYCLES) * HAL_DSI_PLL_PARAM_1M / fPllClkTarget;

        /* Save clock info */
        if (psDsiPhyConfig->bCPhyMode)
        {
          fVal = psDsiPhyConfig->uDesiredBitClkFreq;
          fVal *= 7;
          fVal /= 16;
          psDsiPhyConfigInfo->uBitClkFreq = (uint32)fVal;
          psDsiPhyConfigInfo->uByteClkFreq =  (uint32)fVal / 7;
          fVal = (float)psDsiPhyConfigInfo->uByteClkFreq * (float)psDsiPhyConfig->uNumOfDataLanes * 2 / (float)(psDsiPhyConfig->uBitsPerPixel / 8);

        }
        else
        {
          psDsiPhyConfigInfo->uBitClkFreq = psDsiPhyConfig->uDesiredBitClkFreq;
          psDsiPhyConfigInfo->uByteClkFreq = psDsiPhyConfig->uDesiredBitClkFreq / 8;

          fVal = (float)psDsiPhyConfig->uDesiredBitClkFreq * (float)psDsiPhyConfig->uNumOfDataLanes / (float)psDsiPhyConfig->uBitsPerPixel;
        }
        psDsiPhyConfigInfo->uPclkFreq = (uint32)fVal;
        psDsiPhyConfigInfo->uPllVcoOutputFreq = (uint32)((uint64)psDsiPhyConfig->uDesiredBitClkFreq * psDsiPhyConfig->uPllPostDiv * psDsiPhyConfig->uPhyPostDiv);

        /* Step 3 only for Master */
        if (HAL_MDSS_STATUS_SUCCESS == eStatus)
        {
          HAL_DSI_Phy_CommitPll(eDeviceId, psDsiPhyConfig->ePLLConfigSource, &sPllConfig);
        }

        /* Steps 6,7 Start PLL & Lock */
        if (HAL_MDSS_STATUS_SUCCESS == eStatus)
        {
          eStatus = HAL_DSI_Phy_PllLockDetect(eDeviceId);
        }

        /* Step 8 - Resync Data Paths */
        if (HAL_MDSS_STATUS_SUCCESS == eStatus)
        {
          /* Assert SLEEP_B signal */
          HAL_DSI_Phy_TriggerSleepB(psDsiPhyConfig->eDeviceId, TRUE);
          
          /* Global clock enable */
          HAL_DSI_Phy_PllGlobalClockEnable(psDsiPhyConfig->eDeviceId, TRUE);

          /* Resync FIFOs */
          HAL_DSI_Phy_ResyncFIFO(eDeviceId);
        }
      }
      /* Phy Programming complete */
    }
  }

  return eStatus;
}


/****************************************************************************
*
** FUNCTION: HAL_DSI_2_3_0_PhySetupTimingParams()
*/
/*!
* \brief
*     Calculate PHY timing parameters.
*
* \param [in]  pTimingParameters - DSI core ID
*
* \retval HAL_MDSS_ErrorType
*
****************************************************************************/
HAL_MDSS_ErrorType HAL_DSI_2_3_0_PhySetupTimingParams(HAL_DSI_TimingSettingType *pTimingParameters)
{
  HAL_MDSS_ErrorType           eStatus             = HAL_MDSS_STATUS_SUCCESS;
  HAL_DSI_PHY_TimingConfigType sTimingConfig;

  /* validate input parameters */
  if ((pTimingParameters->eDeviceId != DSI_DeviceID_0) &&
      (pTimingParameters->eDeviceId != DSI_DeviceID_1))
  {
    eStatus = HAL_MDSS_DSI_FAILED_INVALID_INPUT_PARAMETER;
  }
  else
  {
    DSI_OSAL_MemZero(&sTimingConfig, sizeof(HAL_DSI_PHY_TimingConfigType));

    if (pTimingParameters->bCPhyMode)
    {
      /* Calculate timing parameters */
      HAL_DSI_Phy_CalculateTimingParams_CPhy(pTimingParameters, &sTimingConfig);

      /* config timing parameters */
      HAL_DSI_Phy_CommitTimingParams_CPhy(pTimingParameters->eDeviceId, &sTimingConfig);

    }
    else
    {
      /* Calculate timing parameters */
      HAL_DSI_Phy_CalculateTimingParams(pTimingParameters, &sTimingConfig);

      /* config timing parameters */
      HAL_DSI_Phy_CommitTimingParams(pTimingParameters->eDeviceId, &sTimingConfig);
    }

  }
 
  return eStatus;
}



/****************************************************************************
*
** FUNCTION: HAL_DSI_2_3_0_PhyPllReConfigure()
*/
/*!
* \brief
*     Sets up the DSI PLL for dynamic refresh rate reconfiguration and passes
*     back the calculated DSI Phy and PLL information to the caller for the
*     requested refresh rate. Also allows the caller to poll the DSI PHY PLL
*     to ensure that the PLL is locked to the new refresh rate.
*
* \param [in]      psDsiPhyConfig        - Phy configuration information
* \param [in/out]  psDsiPhyPLLConfigInfo - Phy PLL configuration information
*
* \retval HAL_MDSS_ErrorType
*
****************************************************************************/
HAL_MDSS_ErrorType HAL_DSI_2_3_0_PhyPllReConfigure(HAL_DSI_PhyConfigType       *psDsiPhyConfig,
                                                   HAL_DSI_PhyConfigInfoType   *psDsiPhyPLLConfigInfo) 
{
  // Reconfiguring PLL for new refresh rate - To be Implemented
  return HAL_MDSS_STATUS_FAILED_NOT_SUPPORTED;
}


/****************************************************************************
*
** FUNCTION: HAL_DSI_2_3_0_PhyPllInitialize()
*/
/*!
* \brief
*     Initialize the phy in to a power-on reset state.
*
* \param 
*
* \retval 
*
****************************************************************************/
void HAL_DSI_2_3_0_PhyPllInitialize(void)
{
  uint32 uControllerIndex;

  /* Inialize all phys */
  for (uControllerIndex = 0; uControllerIndex < DSI_DeviceID_MAX; uControllerIndex++)
  {
    HAL_DSI_2_3_0_PhyPllPowerCtrl((DSI_Device_IDType)uControllerIndex, TRUE);
  }
}

/****************************************************************************
*
** FUNCTION: HAL_DSI_2_3_0_PhySetProperty()
*/
/*!
* \brief
*   Configure DSI Phy properties
*
* \param [in] eDeviceId     - DSI core ID
* \param [in] eProperty     - Property ID that requires update
* \param [in] pPropertyData - Contains the new parameters of the property 
*                             that will be applied.
*
* \retval HAL_MDSS_ErrorType
*
****************************************************************************/
HAL_MDSS_ErrorType HAL_DSI_2_3_0_PhySetProperty(DSI_Device_IDType              eDeviceId,
                                                HAL_DSI_PhyPropertyType        eProperty,
                                                HAL_DSI_PhyPropertyParamsType *pPropertyData)
{
  HAL_MDSS_ErrorType eStatus = HAL_MDSS_STATUS_SUCCESS;

  switch (eProperty)
  {
    case HAL_DSI_PHY_PROPERTYID_LANEMAP:
      eStatus = HAL_DSI_Phy_LaneMapConfig(eDeviceId, &pPropertyData->aLaneMap[0]);
      break;

    case HAL_DSI_PHY_PROPERTYID_LANECONFIG:
      eStatus = HAL_DSI_Phy_LaneConfig(eDeviceId, &pPropertyData->sLaneConfig);
      break;

    case HAL_DSI_PHY_PROPERTYID_POST_CLK_CONFIG:
      HAL_DSI_Phy_ResyncFIFO(eDeviceId);
      break;

    default:
      eStatus = HAL_MDSS_STATUS_FAILED_NOT_SUPPORTED;
      break;
  }
  return eStatus;
}

/****************************************************************************
*
** FUNCTION: HAL_DSI_2_3_0_PhyGetProperty
*/
/*!
* \brief
*   Get DSI Phy properties
*
* \param [in] eDeviceId     - DSI core ID
* \param [in] eProperty     - Property ID that is being queried
* \param [in] pPropertyData - Contains the parameters of the property 
*                             that are being queried
****************************************************************************/
HAL_MDSS_ErrorType HAL_DSI_2_3_0_PhyGetProperty(DSI_Device_IDType              eDeviceId,
                                                HAL_DSI_PhyPropertyType        eProperty,
                                                HAL_DSI_PhyPropertyParamsType *pPropertyData)
{
  HAL_MDSS_ErrorType eStatus = HAL_MDSS_STATUS_SUCCESS;

  switch (eProperty)
  {
    case HAL_DSI_PHY_PROPERTYID_LANESTATUS:
      eStatus = HAL_DSI_PhyGetLaneStatus(eDeviceId, &pPropertyData->uLaneStatus);
      break;

    default:
      eStatus = HAL_MDSS_STATUS_FAILED_NOT_SUPPORTED;
      break;
  }
  return eStatus;
}

/****************************************************************************
*
** FUNCTION: HAL_DSI_2_3_0_PhyGetPllCodes
*/
/*!
* \brief
*   Get PLL codes
*
* \param [in] eDeviceId     - DSI core ID
* \param [in] pBuffer       - Pointer to buffer that stores pll codes
* \param [in] uSize         - Size of the buffer that stores pll codes 
*                             
****************************************************************************/
HAL_MDSS_ErrorType HAL_DSI_2_3_0_PhyGetPllCodes(DSI_Device_IDType               eDeviceId,
                                                HAL_DSI_PHY_Dynamic_Cal_Result *psPllCodes)
{
  HAL_MDSS_ErrorType eStatus = HAL_MDSS_STATUS_SUCCESS;
  uintPtr            uOffset = HAL_DSI_GetRegBaseOffset(eDeviceId);  

  if (NULL == psPllCodes)
  {
    eStatus = HAL_MDSS_DSI_FAILED_INVALID_INPUT_PARAMETER;
  }
  else
  {
    psPllCodes->uPllCodes[0] = in_dword(uOffset + HWIO_DSI_0_PHY_DSIPHY_PLL_ICODE_ACCUM_STATUS_HIGH_ADDR);
    psPllCodes->uPllCodes[1] = in_dword(uOffset + HWIO_DSI_0_PHY_DSIPHY_PLL_ICODE_ACCUM_STATUS_LOW_ADDR);
    psPllCodes->uPllCodes[2] = in_dword(uOffset + HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_ADDR);
  }
  return eStatus;
}

#ifdef __cplusplus
}
#endif
