#ifndef __HALCLKAPCS_H__
#define __HALCLKAPCS_H__

/*
===========================================================================
*/
/**
  @file HALclkAPCS.h

  Header APCS-specific HAL functions.
*/
/*
  ====================================================================

  Copyright (c) 2013-2017 QUALCOMM Technologies Incorporated.  All Rights Reserved.
  QUALCOMM Proprietary and Confidential.

  ====================================================================


  ====================================================================
*/

/* ============================================================================
**    Include files
** ==========================================================================*/

#include "HALclkInternal.h"


/* ============================================================================
**    Definitions
** ==========================================================================*/

/*
 * OSM definitions
 */
#define OSM_LUT_MAX_ROWS  40


/* ============================================================================
**    Typedefs
** ==========================================================================*/

/**
 * Enumeration of CPU/Clusters.
 */
typedef enum
{
  HAL_CLK_APCS_CPU_CLUSTER0_C0,
  HAL_CLK_APCS_CPU_CLUSTER0_C1,
  HAL_CLK_APCS_CPU_CLUSTER0_C2,
  HAL_CLK_APCS_CPU_CLUSTER0_C3,
  HAL_CLK_APCS_CPU_CLUSTER1_C0,
  HAL_CLK_APCS_CPU_CLUSTER1_C1,
  HAL_CLK_APCS_CPU_CLUSTER1_C2,
  HAL_CLK_APCS_CPU_CLUSTER1_C3,
  HAL_CLK_APCS_CPU_L3,
  HAL_CLK_APCS_CPU_TOTAL,

  HAL_ENUM_32BITS(CLK_APCS_CPU)
} HAL_clk_APCSCPUType;


/*
 * OSM Trace Types.
 */
typedef enum
{
  HAL_CLK_OSM_VIRTUAL_CORNER_APM,
  HAL_CLK_OSM_VIRTUAL_CORNER_ACC,
  HAL_CLK_OSM_VIRTUAL_CORNER_TOTAL,

  HAL_ENUM_32BITS(CLK_OSM_VIRTUAL_CORNER)
} HAL_clk_OsmVirtualCornerType;


/*
 * OSM Trace Types.
 */
typedef enum
{
  HAL_CLK_OSM_GLOBAL_TRACE_ENABLE,
  HAL_CLK_OSM_CLKDOM_TRACE_ENABLE,
  HAL_CLK_OSM_CLKDOM0_PACKET_SEL,
  HAL_CLK_OSM_CLKDOM1_PACKET_SEL,
  HAL_CLK_OSM_CLKDOM2_PACKET_SEL,
  HAL_CLK_OSM_CLKDOM3_PACKET_SEL,
  HAL_CLK_OSM_CLKDOM4_PACKET_SEL,
  HAL_CLK_OSM_CLKDOM5_PACKET_SEL,
  HAL_CLK_OSM_CLKDOM6_PACKET_SEL,
  HAL_CLK_OSM_CLKDOM7_PACKET_SEL,
  HAL_CLK_OSM_CLKDOM8_PACKET_SEL,
  HAL_CLK_OSM_CLKDOM9_PACKET_SEL,
  HAL_CLK_OSM_PERIODIC_TRACE_ENABLE,

  HAL_ENUM_32BITS(CLK_OSM)
} HAL_clk_OsmTraceType;


/* ============================================================================
**    Functions
** ==========================================================================*/

/* ===========================================================================
**  Function : HAL_clk_OSM_LUT_READ_FREQUENCY
**
** ======================================================================== */
/**
  Read the programmed data of the specified row of the OSM Look-Up-Table
  Frequency values.

  @param eCore      [in]  -- CPU cluster to read.
  @param nRow       [in]  -- Row of the OSM Look-Up-Table to read.
  @param nIndex     [out] -- Index value read.
  @param nPllSource [out] -- PLL Source value read.
  @param nPllDiv    [out] -- PLL Div value read.
  @param nDroopCode [out] -- Droop code associated with this row in LUT.
  @param nCoreCount [out] -- Core Count value read.
  @param nLVal      [out] -- PLL L-Value value read.

  @return
  TRUE  -- Configuration read successfully.
  FALSE -- Failed to read configuration for any reason.

  @dependencies
  None.
*/

boolean HAL_clk_OSM_LUT_READ_FREQUENCY
(
  HAL_clk_APCSCPUType  eCore,
  uint32               nRow,
  uint32              *nIndex,
  uint32              *nPllSource,
  uint32              *nPllDiv,
  uint32              *nDroopCode,
  uint32              *nCoreCount,
  uint32              *nLVal
);


/* ===========================================================================
**  Function : HAL_clk_OSM_LUT_READ_VOLTAGE
**
** ======================================================================== */
/**
  Read the programed data of the specified row of the OSM Look-Up-Table
  Voltage values.

  @param eCore            [in]  -- CPU cluster to read.
  @param nRow             [in]  -- Row of the OSM Look-Up-Table to read.
  @param nMemAccLevel     [out] -- Mem ACC Level read.
  @param nVirtualCorner   [out] -- Virtual Corner value read.
  @param nOpenLoopVoltage [out] -- Open Loop Voltage value read.

  @return
  TRUE  -- Configuration read successfully.
  FALSE -- Failed to read configuration for any reason.

  @dependencies
  None.
*/

boolean HAL_clk_OSM_LUT_READ_VOLTAGE
(
  HAL_clk_APCSCPUType  eCore,
  uint32               nRow,
  uint32              *nMemAccLevel,
  uint32              *nVirtualCorner,
  uint32              *nOpenLoopVoltage
);


/* ===========================================================================
**  Function : HAL_clk_OSM_LUT_READ_PLL_OVERRIDES
**
** ======================================================================== */
/**
  Read the programmed data of the specified row of the OSM Look-Up-Table
  PLL Overrides values.

  @param eCore              [in]  -- CPU cluster to read.
  @param nRow               [in]  -- Row of the OSM Look-Up-Table to read.
  @param nLVal              [out] -- L-Value read as 1/L-Value.
  @param nCcSafeFrequency   [out] -- Safe frequency read (expressed in terms of
                                     LVAL sent to PLL) used by core count boost
                                     FSM when unboosting the frequency as a
                                     result of increased core count.
  @param nDcvsSafeFrequency [out] -- Safe frequency read (expressed in terms of
                                     LVAL sent to PLL) used by DCVS safe frequency
                                     FSM when going down in frequency.

  @return
  TRUE  -- Configuration read successfully.
  FALSE -- Failed to read configuration for any reason.

  @dependencies
  None.
*/

boolean HAL_clk_OSM_LUT_READ_PLL_OVERRIDES
(
  HAL_clk_APCSCPUType  eCore,
  uint32               nRow,
  uint32              *nLVal,
  uint32              *nCcSafeFrequency,
  uint32              *nDcvsSafeFrequency
);


/* ===========================================================================
**  Function : HAL_clk_OSM_LUT_READ_SPARE_0
**
** ======================================================================== */
/**
  Read the programmed data of the specified row of the OSM Look-Up-Table
  Spare 0 value.

  @param eCore  [in]  -- CPU cluster to read.
  @param nRow   [in]  -- Row of the OSM Look-Up-Table to read.
  @param nSpare [out] -- Spare value read.

  @return
  TRUE  -- Configuration read successfully.
  FALSE -- Failed to read configuration for any reason.

  @dependencies
  None.
*/

boolean HAL_clk_OSM_LUT_READ_SPARE_0
(
  HAL_clk_APCSCPUType  eCore,
  uint32               nRow,
  uint32              *nSpare
);


/* ===========================================================================
**  Function : HAL_clk_OSM_LUT_CONFIG_FREQUENCY
**
** ======================================================================== */
/**
  Program the specified row of the OSM Look-Up-Table Frequency values.

  @param eCore      [in] -- CPU cluster to configure.
  @param nRow       [in] -- Row of the OSM Look-Up-Table to program.
  @param nIndex     [in] -- Index value to program.
  @param nPllSource [in] -- PLL Source value to program.
  @param nPllDiv    [in] -- PLL Div value to program.
  @param nDroopCode [in] -- Droop code associated with this row in LUT.
  @param nCoreCount [in] -- Core Count value to program.
  @param nLVal      [in] -- PLL L-Value value to program.

  @return
  TRUE  -- Configuration applied successfully.
  FALSE -- Failed to apply configuration for any reason.

  @dependencies
  None.
*/

boolean HAL_clk_OSM_LUT_CONFIG_FREQUENCY
(
  HAL_clk_APCSCPUType eCore,
  uint32              nRow,
  uint32              nIndex,
  uint32              nPllSource,
  uint32              nPllDiv,
  uint32              nDroopCode,
  uint32              nCoreCount,
  uint32              nLVal
);


/* ===========================================================================
**  Function : HAL_clk_OSM_LUT_CONFIG_VOLTAGE
**
** ======================================================================== */
/**
  Program the specified row of the OSM Look-Up-Table Voltage values.

  @param eCore            [in] -- CPU cluster to configure.
  @param nRow             [in] -- Row of the OSM Look-Up-Table to program.
  @param nMemAccLevel     [in] -- Mem ACC Level to program.
  @param nVirtualCorner   [in] -- Virtual Corner value to program.
  @param nOpenLoopVoltage [in] -- Open Loop Voltage value to program.

  @return
  TRUE  -- Configuration applied successfully.
  FALSE -- Failed to apply configuration for any reason.

  @dependencies
  None.
*/

boolean HAL_clk_OSM_LUT_CONFIG_VOLTAGE
(
  HAL_clk_APCSCPUType eCore,
  uint32              nRow,
  uint32              nMemAccLevel,
  uint32              nVirtualCorner,
  uint32              nOpenLoopVoltage
);


/* ===========================================================================
**  Function : HAL_clk_OSM_LUT_CONFIG_PLL_OVERRIDES
**
** ======================================================================== */
/**
  Program the specified row of the OSM Look-Up-Table PLL Overrides values.

  @param eCore              [in] -- CPU cluster to configure.
  @param nRow               [in] -- Row of the OSM Look-Up-Table to program.
  @param nLVal              [in] -- L-Value to be programmed as 1/L-Value.
  @param nCcSafeFrequency   [in] -- Safe frequency (expressed in terms of LVAL
                                    sent to PLL) used by core count boost FSM
                                    when unboosting the frequency as a result
                                    of increased core count.
  @param nDcvsSafeFrequency [in] -- Safe frequency (expressed in terms of LVAL
                                    sent to PLL) used by DCVS safe frequency
                                    FSM when going down in frequency.

  @return
  TRUE  -- Configuration applied successfully.
  FALSE -- Failed to apply configuration for any reason.

  @dependencies
  None.
*/

boolean HAL_clk_OSM_LUT_CONFIG_PLL_OVERRIDES
(
  HAL_clk_APCSCPUType eCore,
  uint32              nRow,
  uint32              nLVal,
  uint32              nCcSafeFrequency,
  uint32              nDcvsSafeFrequency
);


/* ===========================================================================
**  Function : HAL_clk_OSM_LUT_CONFIG_SPARE_0
**
** ======================================================================== */
/**
  Program the specified row of the OSM Look-Up-Table Spare 0 value.

  @param eCore  [in] -- CPU cluster to configure.
  @param nRow   [in] -- Row of the OSM Look-Up-Table to program.
  @param nSpare [in] -- Spare value to program.

  @return
  TRUE  -- Configuration applied successfully.
  FALSE -- Failed to apply configuration for any reason.

  @dependencies
  None.
*/

boolean HAL_clk_OSM_LUT_CONFIG_SPARE_0
(
  HAL_clk_APCSCPUType eCore,
  uint32              nRow,
  uint32              nSpare
);


/* ===========================================================================
**  Function : HAL_clk_OSM_ENABLE
**
** ======================================================================== */
/**
  Enable OSM for a specific cluster.

  @param eCore  [in] -- CPU cluster to enable.

  @return
  TRUE  -- Operation succeeded.
  FALSE -- Operation failed for any reason.

  @dependencies
  Once OSM operation is enabled, it cannot be disabled until next cold boot.
*/

boolean HAL_clk_OSM_ENABLE
(
  HAL_clk_APCSCPUType eCore
);


/* ===========================================================================
**  Function : HAL_clk_IS_OSM_ENABLED
**
** ======================================================================== */
/**
  Determines whether OSM is enabled for any cluster and returns the corresponding
  status.

  @return
  TRUE  -- OSM is enabled.
  FALSE -- OSM is disabled.

*/

boolean HAL_clk_IS_OSM_ENABLED();


/* ===========================================================================
**  Function : HAL_clk_OSM_SET_DESIRED_PERF_LEVEL
**
** ======================================================================== */
/**
  Governor's request for a specific performance state.

  @param eCore  [in] -- CPU cluster to affect.
  @param nIndex [in] -- Index into the OSM LUT.

  @return
  TRUE  -- Operation succeeded.
  FALSE -- Operation failed for any reason.

  @dependencies
  OSM must be programmed and enabled prior to this call.
*/

boolean HAL_clk_OSM_SET_DESIRED_PERF_LEVEL
(
  HAL_clk_APCSCPUType eCore,
  uint32              nIndex
);


/* ===========================================================================
**  Function : HAL_clk_OSM_ApplyVirtualCorner
**
** ======================================================================== */
/**
  Apply OSM Virtual Corner based on the passed in corner type.

  @param eCore   [in] -- CPU cluster to affect.
  @param eCorner [in] -- Virtual Corner type.
  @param nCorner [in] -- Virtual Corner value.

  @return
  TRUE  -- Operation succeeded.
  FALSE -- Operation failed for any reason.

  @dependencies
  None.
*/

boolean HAL_clk_OSM_ApplyVirtualCorner
(
  HAL_clk_APCSCPUType          eCore,
  HAL_clk_OsmVirtualCornerType eCorner,
  uint32                       nCorner
);


/* ===========================================================================
**  Function : HAL_clk_OSM_Configure
**
** ======================================================================== */
/**
  Configure various OSM registers to prepare for normal operation.

  @param eCore  [in] -- CPU cluster to configure.

  @return
  TRUE  -- Operation succeeded.
  FALSE -- Operation failed for any reason.

  @dependencies
  None.
*/

boolean HAL_clk_OSM_Configure
(
  HAL_clk_APCSCPUType eCore
);


/* ===========================================================================
**  Function : HAL_clk_OSM_ConfigureACD
**
** ======================================================================== */
/**
  Configure various ACD registers to prepare for normal operation.

  @param eCore  [in] -- CPU cluster to configure.

  @return
  TRUE  -- Operation succeeded.
  FALSE -- Operation failed for any reason.

  @dependencies
  None.
*/

boolean HAL_clk_OSM_ConfigureACD
(
  HAL_clk_APCSCPUType eCore
);


/* ===========================================================================
**  Function : HAL_clk_OSM_ConfigureTraceControl
**
** ======================================================================== */
/**
  Configure OSM trace control register with the desired value.

  @param eOsmTraceType  [in] -- Trace type to configure.
  @param nOsmTraceValue [in] -- Value to write.

  @return
  TRUE  -- Operation succeeded.
  FALSE -- Operation failed for any reason.

  @dependencies
  None.
*/

boolean HAL_clk_OSM_ConfigureTraceControl
(
  HAL_clk_OsmTraceType  eOsmTraceType,
  uint32                nOsmTraceValue
);


/* ===========================================================================
**  Function : HAL_clk_OSM_ConfigureTraceTimer
**
** ======================================================================== */
/**
  Configure OSM trace timer register with the desired timer value.

  @param nTraceTimerValue [in] -- Timer value to write.

  @return
  TRUE  -- Operation succeeded.
  FALSE -- Operation failed for any reason.

  @dependencies
  None.
*/

boolean HAL_clk_OSM_ConfigureTraceTimer
(
  uint32 nTraceTimerValue
);


/* ===========================================================================
**  Function : HAL_clk_OSM_ConfigurePerformanceBoost
**
** ======================================================================== */
/**
  Configure OSM performance boost feature -- enable/disable.

  @param eCore             [in] -- CPU cluster to configure.
  @param bPerformanceBoost [in] -- OSM performance boost feature -- enable/disable.

  @return
  TRUE  -- Operation succeeded.
  FALSE -- Operation failed for any reason.

  @dependencies
  None.
*/

boolean HAL_clk_OSM_ConfigurePerformanceBoost
(
  HAL_clk_APCSCPUType eCore,
  boolean             bPerformanceBoost
);


/* ===========================================================================
**  Function : HAL_clk_OSM_ConfigureVminReduction
**
** ======================================================================== */
/**
  Configure OSM Vmin Reduction feature -- enable/disable and parameters.

  @param eCore          [in] -- CPU cluster to configure.
  @param bVminReduction [in] -- OSM Vmin Reduction feature -- enable/disable.
  @param nMaxCprVc      [in] -- Maximum corresponding Virtual Corner.
  @param nMinCprVc      [in] -- Minimum corresponding Virtual Corner.

  @return
  TRUE  -- Operation succeeded.
  FALSE -- Operation failed for any reason.

  @dependencies
  None.
*/

boolean HAL_clk_OSM_ConfigureVminReduction
(
  HAL_clk_APCSCPUType eCore,
  boolean             bVminReduction,
  uint32              nMaxCprVc,
  uint32              nMinCprVc
);


#endif

