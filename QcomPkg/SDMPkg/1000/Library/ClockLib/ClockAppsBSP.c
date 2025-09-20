/*
===========================================================================
*/
/**
  @file ClockAppsBSP.c

  BSP data for the SC8180X apps clock driver.
*/
/*
  ====================================================================

  Copyright (c) 2014-2018 QUALCOMM Technologies Incorporated.  All Rights Reserved.
  QUALCOMM Proprietary and Confidential.

  ====================================================================
  ====================================================================
*/


/*=========================================================================
      Include Files
==========================================================================*/

#include "ClockAppsBSP.h"
#include "ClockApps.h"
#include "HALhwio.h"
#include "HALclkHWIO.h"

#ifdef WIN8EA
#ifndef CONSOLE_DEBUGGING
#include "PEP_wpp.h"
#include "ClockAppsBSP.tmh"
#endif
#include "ClockWinTarget.h"
#endif

/*=========================================================================
      Externals
==========================================================================*/

#if defined(__cplusplus)
extern "C" {
#endif

#if defined(__cplusplus)
};
#endif


/*=========================================================================
      Macro Definitions
==========================================================================*/

/*=========================================================================
      Data
==========================================================================*/

/*
 * Performance level configuration data for the Silver Cluster clock.
 * This table is a placeholder and is updated at runtime.
 */
static ClockAppsBSPCPUConfigType ClockAppsBSPConfigSilverCluster =
{
  /*
   * Define performance levels.
   */
  CLOCK_APPS_PERF_LEVEL_0,
  CLOCK_APPS_PERF_LEVEL_0,

  /*
   * Define default performance level mappings.
   */
  {
    CLOCK_APPS_PERF_LEVEL_0,
    CLOCK_APPS_PERF_LEVEL_1,
    CLOCK_APPS_PERF_LEVEL_2,
    CLOCK_APPS_PERF_LEVEL_3,
    CLOCK_APPS_PERF_LEVEL_4,
    CLOCK_APPS_PERF_LEVEL_5,
    CLOCK_APPS_PERF_LEVEL_6,
    CLOCK_APPS_PERF_LEVEL_7,
    CLOCK_APPS_PERF_LEVEL_8,
    CLOCK_APPS_PERF_LEVEL_9,
    CLOCK_APPS_PERF_LEVEL_10,
    CLOCK_APPS_PERF_LEVEL_11,
    CLOCK_APPS_PERF_LEVEL_12,
    CLOCK_APPS_PERF_LEVEL_13,
    CLOCK_APPS_PERF_LEVEL_14,
    CLOCK_APPS_PERF_LEVEL_15,
    CLOCK_APPS_PERF_LEVEL_16,
    CLOCK_APPS_PERF_LEVEL_17,
    CLOCK_APPS_PERF_LEVEL_18,
    CLOCK_APPS_PERF_LEVEL_19,
    CLOCK_APPS_PERF_LEVEL_20,
    CLOCK_APPS_PERF_LEVEL_21,
    CLOCK_APPS_PERF_LEVEL_22,
    CLOCK_APPS_PERF_LEVEL_23,
    CLOCK_APPS_PERF_LEVEL_24,
    CLOCK_APPS_PERF_LEVEL_25,
    CLOCK_APPS_PERF_LEVEL_26,
    CLOCK_APPS_PERF_LEVEL_27,
    CLOCK_APPS_PERF_LEVEL_28,
    CLOCK_APPS_PERF_LEVEL_29,
    CLOCK_APPS_PERF_LEVEL_30,
    CLOCK_APPS_PERF_LEVEL_31,
    CLOCK_APPS_PERF_LEVEL_32,
    CLOCK_APPS_PERF_LEVEL_33,
    CLOCK_APPS_PERF_LEVEL_34,
    CLOCK_APPS_PERF_LEVEL_35,
    CLOCK_APPS_PERF_LEVEL_36,
    CLOCK_APPS_PERF_LEVEL_37,
    CLOCK_APPS_PERF_LEVEL_38,
    CLOCK_APPS_PERF_LEVEL_39,
  },

  /*
   * Configuration data.  Each entry should match the corresponding value
   * in the enumeration CLOCK_APPS0_CONFIG_xxx.
   */
  {
    /*  nFreqHz--------| nVRegAPCCornerIndex-| */
    { 0000000 * 1000UL,  0,                  },
    { 0000000 * 1000UL,  1,                  },
    { 0000000 * 1000UL,  2,                  },
    { 0000000 * 1000UL,  3,                  },
    { 0000000 * 1000UL,  4,                  },
    { 0000000 * 1000UL,  5,                  },
    { 0000000 * 1000UL,  6,                  },
    { 0000000 * 1000UL,  7,                  },
    { 0000000 * 1000UL,  8,                  },
    { 0000000 * 1000UL,  9,                  },
    { 0000000 * 1000UL,  10,                 },
    { 0000000 * 1000UL,  11,                 },
    { 0000000 * 1000UL,  12,                 },
    { 0000000 * 1000UL,  13,                 },
    { 0000000 * 1000UL,  14,                 },
    { 0000000 * 1000UL,  15,                 },
    { 0000000 * 1000UL,  16,                 },
    { 0000000 * 1000UL,  17,                 },
    { 0000000 * 1000UL,  18,                 },
    { 0000000 * 1000UL,  19,                 },
    { 0000000 * 1000UL,  20,                 },
    { 0000000 * 1000UL,  21,                 },
    { 0000000 * 1000UL,  22,                 },
    { 0000000 * 1000UL,  23,                 },
    { 0000000 * 1000UL,  24,                 },
    { 0000000 * 1000UL,  25,                 },
    { 0000000 * 1000UL,  26,                 },
    { 0000000 * 1000UL,  27,                 },
    { 0000000 * 1000UL,  28,                 },
    { 0000000 * 1000UL,  29,                 },
    { 0000000 * 1000UL,  30,                 },
    { 0000000 * 1000UL,  31,                 },
    { 0000000 * 1000UL,  32,                 },
    { 0000000 * 1000UL,  33,                 },
    { 0000000 * 1000UL,  34,                 },
    { 0000000 * 1000UL,  35,                 },
    { 0000000 * 1000UL,  36,                 },
    { 0000000 * 1000UL,  37,                 },
    { 0000000 * 1000UL,  38,                 },
    { 0000000 * 1000UL,  39,                 },
  }
};


/*
 * Performance level configuration data for the Gold Cluster clock.
 * This table is a placeholder and is updated at runtime.
 */
static ClockAppsBSPCPUConfigType ClockAppsBSPConfigGoldCluster =
{
  /*
   * Define performance levels.
   */
  CLOCK_APPS_PERF_LEVEL_0,
  CLOCK_APPS_PERF_LEVEL_0,

  /*
   * Define default performance level mappings.
   */
  {
    CLOCK_APPS_PERF_LEVEL_0,
    CLOCK_APPS_PERF_LEVEL_1,
    CLOCK_APPS_PERF_LEVEL_2,
    CLOCK_APPS_PERF_LEVEL_3,
    CLOCK_APPS_PERF_LEVEL_4,
    CLOCK_APPS_PERF_LEVEL_5,
    CLOCK_APPS_PERF_LEVEL_6,
    CLOCK_APPS_PERF_LEVEL_7,
    CLOCK_APPS_PERF_LEVEL_8,
    CLOCK_APPS_PERF_LEVEL_9,
    CLOCK_APPS_PERF_LEVEL_10,
    CLOCK_APPS_PERF_LEVEL_11,
    CLOCK_APPS_PERF_LEVEL_12,
    CLOCK_APPS_PERF_LEVEL_13,
    CLOCK_APPS_PERF_LEVEL_14,
    CLOCK_APPS_PERF_LEVEL_15,
    CLOCK_APPS_PERF_LEVEL_16,
    CLOCK_APPS_PERF_LEVEL_17,
    CLOCK_APPS_PERF_LEVEL_18,
    CLOCK_APPS_PERF_LEVEL_19,
    CLOCK_APPS_PERF_LEVEL_20,
    CLOCK_APPS_PERF_LEVEL_21,
    CLOCK_APPS_PERF_LEVEL_22,
    CLOCK_APPS_PERF_LEVEL_23,
    CLOCK_APPS_PERF_LEVEL_24,
    CLOCK_APPS_PERF_LEVEL_25,
    CLOCK_APPS_PERF_LEVEL_26,
    CLOCK_APPS_PERF_LEVEL_27,
    CLOCK_APPS_PERF_LEVEL_28,
    CLOCK_APPS_PERF_LEVEL_29,
    CLOCK_APPS_PERF_LEVEL_30,
    CLOCK_APPS_PERF_LEVEL_31,
    CLOCK_APPS_PERF_LEVEL_32,
    CLOCK_APPS_PERF_LEVEL_33,
    CLOCK_APPS_PERF_LEVEL_34,
    CLOCK_APPS_PERF_LEVEL_35,
    CLOCK_APPS_PERF_LEVEL_36,
    CLOCK_APPS_PERF_LEVEL_37,
    CLOCK_APPS_PERF_LEVEL_38,
    CLOCK_APPS_PERF_LEVEL_39,
  },

  /*
   * Configuration data.  Each entry should match the corresponding value
   * in the enumeration CLOCK_APPS1_CONFIG_xxx.
   */
  {
    /*  nFreqHz--------| nVRegAPCCornerIndex-| */
    { 0000000 * 1000UL,  0,                  },
    { 0000000 * 1000UL,  1,                  },
    { 0000000 * 1000UL,  2,                  },
    { 0000000 * 1000UL,  3,                  },
    { 0000000 * 1000UL,  4,                  },
    { 0000000 * 1000UL,  5,                  },
    { 0000000 * 1000UL,  6,                  },
    { 0000000 * 1000UL,  7,                  },
    { 0000000 * 1000UL,  8,                  },
    { 0000000 * 1000UL,  9,                  },
    { 0000000 * 1000UL,  10,                 },
    { 0000000 * 1000UL,  11,                 },
    { 0000000 * 1000UL,  12,                 },
    { 0000000 * 1000UL,  13,                 },
    { 0000000 * 1000UL,  14,                 },
    { 0000000 * 1000UL,  15,                 },
    { 0000000 * 1000UL,  16,                 },
    { 0000000 * 1000UL,  17,                 },
    { 0000000 * 1000UL,  18,                 },
    { 0000000 * 1000UL,  19,                 },
    { 0000000 * 1000UL,  20,                 },
    { 0000000 * 1000UL,  21,                 },
    { 0000000 * 1000UL,  22,                 },
    { 0000000 * 1000UL,  23,                 },
    { 0000000 * 1000UL,  24,                 },
    { 0000000 * 1000UL,  25,                 },
    { 0000000 * 1000UL,  26,                 },
    { 0000000 * 1000UL,  27,                 },
    { 0000000 * 1000UL,  28,                 },
    { 0000000 * 1000UL,  29,                 },
    { 0000000 * 1000UL,  30,                 },
    { 0000000 * 1000UL,  31,                 },
    { 0000000 * 1000UL,  32,                 },
    { 0000000 * 1000UL,  33,                 },
    { 0000000 * 1000UL,  34,                 },
    { 0000000 * 1000UL,  35,                 },
    { 0000000 * 1000UL,  36,                 },
    { 0000000 * 1000UL,  37,                 },
    { 0000000 * 1000UL,  38,                 },
    { 0000000 * 1000UL,  39,                 },
  }
};


/*
 * Performance level configuration data for the L3 clock.
 * This table is a placeholder and is updated at runtime.
 */
static ClockAppsBSPCPUConfigType ClockAppsBSPConfigL3 =
{
  /*
   * Define performance levels.
   */
  CLOCK_APPS_PERF_LEVEL_0,
  CLOCK_APPS_PERF_LEVEL_0,

  /*
   * Define default performance level mappings.
   */
  {
    CLOCK_APPS_PERF_LEVEL_0,
    CLOCK_APPS_PERF_LEVEL_1,
    CLOCK_APPS_PERF_LEVEL_2,
    CLOCK_APPS_PERF_LEVEL_3,
    CLOCK_APPS_PERF_LEVEL_4,
    CLOCK_APPS_PERF_LEVEL_5,
    CLOCK_APPS_PERF_LEVEL_6,
    CLOCK_APPS_PERF_LEVEL_7,
    CLOCK_APPS_PERF_LEVEL_8,
    CLOCK_APPS_PERF_LEVEL_9,
    CLOCK_APPS_PERF_LEVEL_10,
    CLOCK_APPS_PERF_LEVEL_11,
    CLOCK_APPS_PERF_LEVEL_12,
    CLOCK_APPS_PERF_LEVEL_13,
    CLOCK_APPS_PERF_LEVEL_14,
    CLOCK_APPS_PERF_LEVEL_15,
    CLOCK_APPS_PERF_LEVEL_16,
    CLOCK_APPS_PERF_LEVEL_17,
    CLOCK_APPS_PERF_LEVEL_18,
    CLOCK_APPS_PERF_LEVEL_19,
    CLOCK_APPS_PERF_LEVEL_20,
    CLOCK_APPS_PERF_LEVEL_21,
    CLOCK_APPS_PERF_LEVEL_22,
    CLOCK_APPS_PERF_LEVEL_23,
    CLOCK_APPS_PERF_LEVEL_24,
    CLOCK_APPS_PERF_LEVEL_25,
    CLOCK_APPS_PERF_LEVEL_26,
    CLOCK_APPS_PERF_LEVEL_27,
    CLOCK_APPS_PERF_LEVEL_28,
    CLOCK_APPS_PERF_LEVEL_29,
    CLOCK_APPS_PERF_LEVEL_30,
    CLOCK_APPS_PERF_LEVEL_31,
    CLOCK_APPS_PERF_LEVEL_32,
    CLOCK_APPS_PERF_LEVEL_33,
    CLOCK_APPS_PERF_LEVEL_34,
    CLOCK_APPS_PERF_LEVEL_35,
    CLOCK_APPS_PERF_LEVEL_36,
    CLOCK_APPS_PERF_LEVEL_37,
    CLOCK_APPS_PERF_LEVEL_38,
    CLOCK_APPS_PERF_LEVEL_39,
  },

  /*
   * Configuration data.  Each entry should match the corresponding value
   * in the enumeration CLOCK_APPS1_CONFIG_xxx.
   */
  {
  /*  nFreqHz---------| nVRegAPCCornerIndex-| */
    { 0000000 * 1000UL, 0,                  },
    { 0000000 * 1000UL, 1,                  },
    { 0000000 * 1000UL, 2,                  },
    { 0000000 * 1000UL, 3,                  },
    { 0000000 * 1000UL, 4,                  },
    { 0000000 * 1000UL, 5,                  },
    { 0000000 * 1000UL, 6,                  },
    { 0000000 * 1000UL, 7,                  },
    { 0000000 * 1000UL, 8,                  },
    { 0000000 * 1000UL, 9,                  },
    { 0000000 * 1000UL, 10,                 },
    { 0000000 * 1000UL, 11,                 },
    { 0000000 * 1000UL, 12,                 },
    { 0000000 * 1000UL, 13,                 },
    { 0000000 * 1000UL, 14,                 },
    { 0000000 * 1000UL, 15,                 },
    { 0000000 * 1000UL, 16,                 },
    { 0000000 * 1000UL, 17,                 },
    { 0000000 * 1000UL, 18,                 },
    { 0000000 * 1000UL, 19,                 },
    { 0000000 * 1000UL, 20,                 },
    { 0000000 * 1000UL, 21,                 },
    { 0000000 * 1000UL, 22,                 },
    { 0000000 * 1000UL, 23,                 },
    { 0000000 * 1000UL, 24,                 },
    { 0000000 * 1000UL, 25,                 },
    { 0000000 * 1000UL, 26,                 },
    { 0000000 * 1000UL, 27,                 },
    { 0000000 * 1000UL, 28,                 },
    { 0000000 * 1000UL, 29,                 },
    { 0000000 * 1000UL, 30,                 },
    { 0000000 * 1000UL, 31,                 },
    { 0000000 * 1000UL, 32,                 },
    { 0000000 * 1000UL, 33,                 },
    { 0000000 * 1000UL, 34,                 },
    { 0000000 * 1000UL, 35,                 },
    { 0000000 * 1000UL, 36,                 },
    { 0000000 * 1000UL, 37,                 },
    { 0000000 * 1000UL, 38,                 },
    { 0000000 * 1000UL, 39,                 },
  }
};


/*
 * Voltage tables -- one per cluster -- populated by tz vpp
 */
static ClockVRegCornerDataType ClockVRegCornerDataSilverCluster[CLOCK_OSM_LUT_MAX_ROWS];
static ClockVRegCornerDataType ClockVRegCornerDataGoldCluster[CLOCK_OSM_LUT_MAX_ROWS];
static ClockVRegCornerDataType ClockVRegCornerDataL3[CLOCK_OSM_LUT_MAX_ROWS];


/*
 * Apps BSP data.
 */
static ClockAppsBSPType ClockAppsBSPConfig =
{
  .aCPUConfig =
  {
    {
      .szName                 = "apcs_silver_post_acd_clk",
      .pConfig                = &ClockAppsBSPConfigSilverCluster,
      .pVRegCornerData        = ClockVRegCornerDataSilverCluster
    },
    {
      .szName                 = "apcs_gold_post_acd_clk",
      .pConfig                = &ClockAppsBSPConfigGoldCluster,
      .pVRegCornerData        = ClockVRegCornerDataGoldCluster
    },
    {
      .szName                 = "apcs_l3_post_acd_clk",
      .pConfig                = &ClockAppsBSPConfigL3,
      .pVRegCornerData        = ClockVRegCornerDataL3
    },
  },
  .aClockOsmLut               = {},
  .aClockOsmCtxt              = {},
};

/*=========================================================================
     Functions
==========================================================================*/

/* =========================================================================
**  Function : Clock_ReadOsmLutData
** =========================================================================*/
/*
  Reads the OSM LUT from the OSM hardware.
*/

static DALResult Clock_ReadOsmLutData
(
  ClockDrvCtxt     *pDrvCtxt,
  ClockAppsCPUType  eCPU
)
{
  uint32  nRowNumber;
  uint32  nDummy;

  for (nRowNumber = 0; nRowNumber < CLOCK_OSM_LUT_MAX_ROWS; nRowNumber++)
  {
    if (!HAL_clk_OSM_LUT_READ_FREQUENCY(
          (HAL_clk_APCSCPUType)eCPU,
          nRowNumber,
          &ClockAppsBSPConfig.aClockOsmLut[eCPU][nRowNumber].nIndex,
          &ClockAppsBSPConfig.aClockOsmLut[eCPU][nRowNumber].nClockSource,
          &ClockAppsBSPConfig.aClockOsmLut[eCPU][nRowNumber].nPostDivideRatio,
          &ClockAppsBSPConfig.aClockOsmLut[eCPU][nRowNumber].nDroopCode,
          &ClockAppsBSPConfig.aClockOsmLut[eCPU][nRowNumber].nMaxCoreCount,
          &ClockAppsBSPConfig.aClockOsmLut[eCPU][nRowNumber].nFinalFrequency))
    {
      return DAL_ERROR;
    }
    if (!HAL_clk_OSM_LUT_READ_VOLTAGE(
          (HAL_clk_APCSCPUType)eCPU,
          nRowNumber,
          &ClockAppsBSPConfig.aClockOsmLut[eCPU][nRowNumber].nMemAccLevel,
          &ClockAppsBSPConfig.aClockOsmLut[eCPU][nRowNumber].nVirtualCorner,
          &ClockAppsBSPConfig.aClockOsmLut[eCPU][nRowNumber].nOpenLoopVoltage))
    {
      return DAL_ERROR;
    }
    if (!HAL_clk_OSM_LUT_READ_PLL_OVERRIDES(
          (HAL_clk_APCSCPUType)eCPU,
          nRowNumber,
          &nDummy,  // throw away the 1/LVal value read from LUT -- unnecessary
          &ClockAppsBSPConfig.aClockOsmLut[eCPU][nRowNumber].nCoreCountSafeFrequency,
          &ClockAppsBSPConfig.aClockOsmLut[eCPU][nRowNumber].nDcvsSafeFrequency))
    {
      return DAL_ERROR;
    }
    if (!HAL_clk_OSM_LUT_READ_SPARE_0(
          (HAL_clk_APCSCPUType)eCPU,
          nRowNumber,
          &ClockAppsBSPConfig.aClockOsmLut[eCPU][nRowNumber].nSpares))
    {
      return DAL_ERROR;
    }
  }

  return DAL_SUCCESS;

} /* END Clock_ReadOsmLutData */


/* =========================================================================
**  Function : Clock_SaveOsmLutData
** =========================================================================*/
/*
  Saves the OSM LUT data from the hardware and parces it into the frequency
  and voltage tables.
*/

static DALResult Clock_SaveOsmLutData
(
  ClockDrvCtxt     *pDrvCtxt,
  ClockAppsCPUType  eCPU
)
{
  uint32                nRowNumber, nItemNumber, nMaxPublishedPerfLevel = 0;
  ClockAppsClusterType  eCluster;
  boolean               bLessThanFour = FALSE;
  uint32                nPrevFreq = 0, nPrevVoltage = 0;

  switch (eCPU)
  {
    case CLOCK_APPS_CPU_C0:
    case CLOCK_APPS_CPU_C1:
    case CLOCK_APPS_CPU_C2:
    case CLOCK_APPS_CPU_C3:
      eCluster = CLOCK_APPS_CPU_CLUSTER_C0;
      break;

    case CLOCK_APPS_CPU_C4:
    case CLOCK_APPS_CPU_C5:
    case CLOCK_APPS_CPU_C6:
    case CLOCK_APPS_CPU_C7:
      eCluster = CLOCK_APPS_CPU_CLUSTER_C1;
      break;

    case CLOCK_APPS_CPU_L3:
      eCluster = CLOCK_APPS_CPU_CLUSTER_L3;
      break;

    default:
      ULOG_RT_PRINTF_0 (pDrvCtxt->hClockLog,
                        "DALSYS_LOGEVENT_FATAL_ERROR: Invalid clock domain detected by Clock_SaveOsmLutData.");
      ERR_FATAL("Clock_SaveOsmLutData", 0, 0, 0);
      return DAL_ERROR_INTERNAL;
  }

  /* Count the number of valid performance levels programmed into the OSM LUT. */
  for (nRowNumber = 0; nRowNumber < CLOCK_OSM_LUT_MAX_ROWS; nRowNumber++)
  {
    /* Identify duplicate rows -- frequency and voltage pairs identical to the previous. */
    if (ClockAppsBSPConfig.aClockOsmLut[eCPU][nRowNumber].nFinalFrequency == nPrevFreq &&
        ClockAppsBSPConfig.aClockOsmLut[eCPU][nRowNumber].nOpenLoopVoltage == nPrevVoltage)
    {
      break;
    }
    nPrevFreq = ClockAppsBSPConfig.aClockOsmLut[eCPU][nRowNumber].nFinalFrequency;
    nPrevVoltage = ClockAppsBSPConfig.aClockOsmLut[eCPU][nRowNumber].nOpenLoopVoltage;
    if (ClockAppsBSPConfig.aClockOsmLut[eCPU][nRowNumber].nMaxCoreCount == 4 && !bLessThanFour)
    {
      nMaxPublishedPerfLevel++;
    }
    if (ClockAppsBSPConfig.aClockOsmLut[eCPU][nRowNumber].nMaxCoreCount < 4)
    {
      bLessThanFour = TRUE;
    }
  }

  /* Update selected frequency configuration and voltage plan values. */
  for (nRowNumber = nItemNumber = 0; nRowNumber < CLOCK_OSM_LUT_MAX_ROWS; nRowNumber++)
  {
    /* Filter out rows that belong to the Performance Boost feature. */
    if (ClockAppsBSPConfig.aClockOsmLut[eCPU][nRowNumber].nMaxCoreCount == 4)
    {
      if (nRowNumber == 0 && ClockAppsBSPConfig.aClockOsmLut[eCPU][nRowNumber].nClockSource == 0)
      {
        ClockAppsBSPConfig.aCPUConfig[eCluster].pConfig->aPerfConfig[nItemNumber].nFreqHz = 300000000;
      }
      else
      {
        ClockAppsBSPConfig.aCPUConfig[eCluster].pConfig->aPerfConfig[nItemNumber].nFreqHz =
          ClockAppsBSPConfig.aClockOsmLut[eCPU][nRowNumber].nFinalFrequency * 19200000;
      }
      ClockAppsBSPConfig.aCPUConfig[eCluster].pVRegCornerData[nItemNumber].nFreqKHz =
        ClockAppsBSPConfig.aCPUConfig[eCluster].pConfig->aPerfConfig[nItemNumber].nFreqHz / 1000;
      ClockAppsBSPConfig.aCPUConfig[eCluster].pVRegCornerData[nItemNumber].nInitialVoltageUV =
        ClockAppsBSPConfig.aClockOsmLut[eCPU][nRowNumber].nOpenLoopVoltage * 1000;
      nItemNumber++;
    }
  }

  /* Make sure that the rest of the table is also updated. */
  for (nRowNumber = nItemNumber; nRowNumber < CLOCK_OSM_LUT_MAX_ROWS; nRowNumber++)
  {
    ClockAppsBSPConfig.aCPUConfig[eCluster].pConfig->aPerfConfig[nRowNumber].nFreqHz =
      ClockAppsBSPConfig.aClockOsmLut[eCPU][nRowNumber].nFinalFrequency * 19200000;
    ClockAppsBSPConfig.aCPUConfig[eCluster].pVRegCornerData[nRowNumber].nFreqKHz =
      ClockAppsBSPConfig.aCPUConfig[eCluster].pConfig->aPerfConfig[nRowNumber].nFreqHz / 1000;
    ClockAppsBSPConfig.aCPUConfig[eCluster].pVRegCornerData[nRowNumber].nInitialVoltageUV =
      ClockAppsBSPConfig.aClockOsmLut[eCPU][nRowNumber].nOpenLoopVoltage * 1000;
  }

  /* Update the appropriate MAX Perf Level field. */
  if (nMaxPublishedPerfLevel > 0)
  {
    nMaxPublishedPerfLevel--;
  }
  ClockAppsBSPConfig.aCPUConfig[eCluster].pConfig->nMaxPerfLevel = nMaxPublishedPerfLevel;

  return DAL_SUCCESS;

} /* END Clock_SaveOsmLutData */


/* =========================================================================
**  Function : Clock_InitAppsBSP
** =========================================================================*/
/*
  See ClockAppsBSP.h
*/

DALResult Clock_InitAppsBSP
(
  ClockDrvCtxt *pDrvCtxt
)
{
  uint32             nChipVersion, nChipId;
  ClockAppsCPUType   eCPU;

  nChipVersion = HAL_clk_GetChipVersion();
  nChipId = HAL_clk_GetChipId();

  /*-----------------------------------------------------------------------*/
  /* Read data programmed into the Operation State Manager (OSM) by Trust  */
  /* Zone clock driver and recreate the corresponding frequency and        */
  /* voltage plan tables.                                                  */
  /*-----------------------------------------------------------------------*/

  /* Read the OSM hardware to get the values from the OSM LUT. */
  for (eCPU = CLOCK_APPS_CPU_C0; eCPU < CLOCK_APPS_CPU_TOTAL; eCPU++)
  {
    if (Clock_ReadOsmLutData(pDrvCtxt, eCPU) != DAL_SUCCESS)
    {
      return DAL_ERROR;
    }
    if (Clock_SaveOsmLutData(pDrvCtxt, eCPU) != DAL_SUCCESS)
    {
      return DAL_ERROR;
    }
  }

  return DAL_SUCCESS;

} /* END Clock_InitAppsBSP */


/* =========================================================================
**  Function : Clock_GetAppsBSP
** =========================================================================*/
/*
  See ClockAppsBSP.h
*/

ClockAppsBSPType *Clock_GetAppsBSP
(
  void
)
{
  return &ClockAppsBSPConfig;

} /* END Clock_GetAppsBSP */


/* =========================================================================
**  Function : Clock_ReadAppsBSPConfig
** =========================================================================*/
/*
  See ClockApps.h
*/

void Clock_ReadAppsBSPConfig
(
  eConfigParamType  eConfigParam,
  uint32           *nValue
)
{
  ClockDrvCtxt  *pDrvCtxt = Clock_GetDrvCtxt();

  switch (eConfigParam)
  {
    case CLOCK_CONFIG_PARAM_GET_VMIN_REDUCTION_STATE_CLUSTER_0:
      break;

    case CLOCK_CONFIG_PARAM_GET_PERFORMANCE_BOOST_STATE_CLUSTER_0:
      *nValue = (uint32)ClockAppsBSPConfig.aClockOsmCtxt[0].bPerformanceBoost;
      break;

    case CLOCK_CONFIG_PARAM_GET_BOOST_DELTA_HZ_CLUSTER_0:
      *nValue = (uint32)ClockAppsBSPConfig.aClockOsmCtxt[0].nBoostDeltaHz;
      break;

    case CLOCK_CONFIG_PARAM_GET_BOOST_PERF_LEVEL_CLUSTER_0:
      *nValue = CLOCK_OSM_LUT_MAX_ROWS - 1;
      break;

    case CLOCK_CONFIG_PARAM_GET_VMIN_REDUCTION_STATE_CLUSTER_1:
      break;

    case CLOCK_CONFIG_PARAM_GET_PERFORMANCE_BOOST_STATE_CLUSTER_1:
      *nValue = (uint32)ClockAppsBSPConfig.aClockOsmCtxt[4].bPerformanceBoost;
      break;

    case CLOCK_CONFIG_PARAM_GET_BOOST_DELTA_HZ_CLUSTER_1:
      *nValue = (uint32)ClockAppsBSPConfig.aClockOsmCtxt[4].nBoostDeltaHz;
      break;

    case CLOCK_CONFIG_PARAM_GET_BOOST_PERF_LEVEL_CLUSTER_1:
      *nValue = CLOCK_OSM_LUT_MAX_ROWS - 1;
      break;

    case CLOCK_CONFIG_PARAM_GET_VMIN_REDUCTION_STATE_L3:
      break;

    case CLOCK_CONFIG_PARAM_GET_PERFORMANCE_BOOST_STATE_L3:
      *nValue = (uint32)ClockAppsBSPConfig.aClockOsmCtxt[8].bPerformanceBoost;
      break;

    case CLOCK_CONFIG_PARAM_GET_BOOST_DELTA_HZ_L3:
      *nValue = (uint32)ClockAppsBSPConfig.aClockOsmCtxt[8].nBoostDeltaHz;
      break;

    case CLOCK_CONFIG_PARAM_GET_BOOST_PERF_LEVEL_L3:
      *nValue = CLOCK_OSM_LUT_MAX_ROWS - 1;
      break;

    case CLOCK_CONFIG_PARAM_INTERPOLATION_MODE:
    case CLOCK_CONFIG_PARAM_DYNAMIC_CEILING_MODE:
    case CLOCK_CONFIG_PARAM_DYNAMIC_FLOOR_MODE:
    case CLOCK_CONFIG_PARAM_PLL_SLEWING_MODE:
    case CLOCK_CONFIG_PARAM_GFX_FULL_RPM_EMULATION_MODE:
    case CLOCK_CONFIG_PARAM_PLL_DROOP_DETECTION_MODE:
    case CLOCK_CONFIG_PARAM_ACD_REDUCTION_MODE:
    case CLOCK_CONFIG_PARAM_APM_CONTROL_MODE:
    case CLOCK_CONFIG_PARAM_LDO_CONTROL_MODE:
    case CLOCK_CONFIG_PARAM_LDO_MAX_VOLTAGE_UV:
    case CLOCK_CONFIG_PARAM_LDO_MIN_VOLTAGE_UV:
    case CLOCK_CONFIG_PARAM_LDO_MIN_HEADROOM_UV:
    case CLOCK_CONFIG_PARAM_LDO_SAFETY_MARGIN_UV:
    case CLOCK_CONFIG_PARAM_LDO_APC_RETENTION_UV:
    case CLOCK_CONFIG_PARAM_GLOBAL_VOLTAGE_LIMIT_UV:
      ULOG_RT_PRINTF_0 (pDrvCtxt->hClockLog,
                        "DALSYS_LOGEVENT_WARNING: Unused Apps BSP configuration type.");
      break;

    default:
      ULOG_RT_PRINTF_0 (pDrvCtxt->hClockLog,
                        "DALSYS_LOGEVENT_FATAL_ERROR: Undefined Apps BSP configuration type.");
      ERR_FATAL("Clock_ReadAppsBSPConfig", 0, 0, 0);
      break;
  }

} /* END Clock_ReadAppsBSPConfig */


/* =========================================================================
**  Function : Clock_UpdateAppsBSPConfig
** =========================================================================*/
/*
  See ClockApps.h
*/

void Clock_UpdateAppsBSPConfig
(
  eConfigParamType eConfigParam,
  uint32           nValue
)
{
  ClockDrvCtxt  *pDrvCtxt = Clock_GetDrvCtxt();

  switch (eConfigParam)
  {
    case CLOCK_CONFIG_PARAM_APCS_SHARED_CX_RAIL:
    case CLOCK_CONFIG_PARAM_INTERPOLATION_MODE:
    case CLOCK_CONFIG_PARAM_DYNAMIC_CEILING_MODE:
    case CLOCK_CONFIG_PARAM_DYNAMIC_FLOOR_MODE:
    case CLOCK_CONFIG_PARAM_PLL_SLEWING_MODE:
    case CLOCK_CONFIG_PARAM_GFX_FULL_RPM_EMULATION_MODE:
    case CLOCK_CONFIG_PARAM_PLL_DROOP_DETECTION_MODE:
    case CLOCK_CONFIG_PARAM_ACD_REDUCTION_MODE:
    case CLOCK_CONFIG_PARAM_APM_CONTROL_MODE:
    case CLOCK_CONFIG_PARAM_LDO_CONTROL_MODE:
    case CLOCK_CONFIG_PARAM_LDO_MAX_VOLTAGE_UV:
    case CLOCK_CONFIG_PARAM_LDO_MIN_VOLTAGE_UV:
    case CLOCK_CONFIG_PARAM_LDO_MIN_HEADROOM_UV:
    case CLOCK_CONFIG_PARAM_LDO_SAFETY_MARGIN_UV:
    case CLOCK_CONFIG_PARAM_LDO_APC_RETENTION_UV:
    case CLOCK_CONFIG_PARAM_GLOBAL_VOLTAGE_LIMIT_UV:
      ULOG_RT_PRINTF_0 (pDrvCtxt->hClockLog,
                        "DALSYS_LOGEVENT_WARNING: Unused Apps BSP configuration type.");
      break;

    default:
      ULOG_RT_PRINTF_0 (pDrvCtxt->hClockLog,
                        "DALSYS_LOGEVENT_FATAL_ERROR: Undefined Apps BSP configuration type.");
      ERR_FATAL("Clock_UpdateAppsBSPConfig", 0, 0, 0);
      break;
  }

} /* END Clock_UpdateAppsBSPConfig */


/* =========================================================================
**  Function : Clock_MapPerfLevelToFrequency
** =========================================================================*/
/**
  See ClockApps.h
*/

uint32 Clock_MapPerfLevelToFrequency
(
  ClockDrvCtxt     *pDrvCtxt,
  ClockAppsCPUType  eCPU,
  uint32            nPerfLevel
)
{
  ClockAppsCtxtType         *pAppsCtxt = (ClockAppsCtxtType *)pDrvCtxt->pImageCtxt;
  ClockAppsBSPCPUConfigType *pBSPConfig;

  /*-----------------------------------------------------------------------*/
  /* Get the frequency data.                                               */
  /*-----------------------------------------------------------------------*/

  pBSPConfig = pAppsCtxt->aCPU[eCPU].pBSPConfig->pConfig;
  return pBSPConfig->aPerfConfig[pBSPConfig->anPerfLevel[nPerfLevel]].nFreqHz;

} /* END Clock_MapPerfLevelToFrequency */

