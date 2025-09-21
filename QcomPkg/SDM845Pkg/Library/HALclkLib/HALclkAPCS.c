/*
==============================================================================

FILE:         HALclkAPCS.c

DESCRIPTION:
  This file contains the clock HAL code to control the APCS clocks.


==============================================================================

                             Edit History


==============================================================================
   Copyright (c) 2017 QUALCOMM Technologies Incorporated.
                    All Rights Reserved.
                  QUALCOMM Proprietary/GTDR
==============================================================================
*/

/*============================================================================

                     INCLUDE FILES FOR MODULE

============================================================================*/


#include <HALhwio.h>

#include "HALclkInternal.h"
#include "HALclkGeneric.h"
#include "HALclkHWIO.h"
#include "HALclkAPCS.h"


/*============================================================================

             DEFINITIONS AND DECLARATIONS FOR MODULE

=============================================================================*/

/* ============================================================================
**    Prototypes
** ==========================================================================*/

static void HAL_clk_REFGENSouthEnable(HAL_clk_ClockDescType *pmClockDesc);
static void HAL_clk_REFGENSouthDisable(HAL_clk_ClockDescType *pmClockDesc);


/* ============================================================================
**    Typedefs
** ==========================================================================*/


/* ============================================================================
**    Data
** ==========================================================================*/

/*
 * HAL_clk_REFGENSouth
 *
 * Special functions for controlling Napali REFGEN South.
 */
HAL_clk_ClockControlType HAL_clk_REFGENSouth =
{
  /* .Enable           = */ HAL_clk_REFGENSouthEnable,
  /* .Disable          = */ HAL_clk_REFGENSouthDisable,
  /* .IsEnabled        = */ NULL,
  /* .IsOn             = */ NULL,
  /* .Reset            = */ NULL,
  /* .IsReset          = */ NULL,
  /* .Config           = */ NULL,
  /* .DetectConfig     = */ NULL,
  /* .ConfigDivider    = */ NULL,
  /* .DetectDivider    = */ NULL,
  /* .ConfigFootswitch = */ NULL,
  /* .Invert           = */ NULL,
  /* .ResetBranch      = */ NULL,
};


/*============================================================================

               FUNCTION DEFINITIONS FOR MODULE

============================================================================*/


/* ===========================================================================
**  HAL_clk_REFGENSouthEnable
**
** ======================================================================== */

static void HAL_clk_REFGENSouthEnable
(
  HAL_clk_ClockDescType *pmClockDesc
)
{
  HWIO_OUT(PHY_REFGEN_SOUTH_BIAS_EN, 0x4);
} /* HAL_clk_REFGENSouthEnable */


/* ===========================================================================
**  HAL_clk_REFGENSouthDisable
**
** ======================================================================== */

static void HAL_clk_REFGENSouthDisable
(
  HAL_clk_ClockDescType *pmClockDesc
)
{
  HWIO_OUT(PHY_REFGEN_SOUTH_BIAS_EN, 0x0);
} /* HAL_clk_REFGENSouthDisable */


/* ===========================================================================
**  Function : HAL_clk_EnableMiscDependencies
**
** ======================================================================== */

void HAL_clk_EnableMiscDependencies()
{
  HWIO_OUTF(GPUCC_GPU_CC_GX_DOMAIN_MISC, GPU_GX_GMEM_RESET, 1);
  HAL_clk_BusyWait(1);
  HWIO_OUTF(GPUCC_GPU_CC_GX_DOMAIN_MISC, GPU_GX_GMEM_RESET, 0);
  HWIO_OUTF(GPUCC_GPU_CC_GX_DOMAIN_MISC, GPU_GX_GMEM_CLAMP_IO, 0);
} /* End HAL_clk_EnableMiscDependencies */


/* ===========================================================================
**  Function : HAL_clk_OSM_LUT_READ_FREQUENCY
**
** ======================================================================== */

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
)
{
  /* Sanity check */
  if (eCore      >= HAL_CLK_APCS_CPU_TOTAL ||
      nRow       >= OSM_LUT_MAX_ROWS       ||
      nIndex     == NULL                   ||
      nPllSource == NULL                   ||
      nPllDiv    == NULL                   ||
      nDroopCode == NULL                   ||
      nCoreCount == NULL                   ||
      nLVal      == NULL                   )
  {
    return FALSE;
  }

  /* Read program from the hardware. */
  switch(eCore)
  {
    case HAL_CLK_APCS_CPU_CLUSTER0_C0:
    case HAL_CLK_APCS_CPU_CLUSTER0_C1:
    case HAL_CLK_APCS_CPU_CLUSTER0_C2:
    case HAL_CLK_APCS_CPU_CLUSTER0_C3:
      *nIndex     = HWIO_INFI(APSS_OSM_RAIL0_CLKDOM1_DCVS_LUT_ENTRYr_FREQUENCY, nRow, INDEX);
      *nPllSource = HWIO_INFI(APSS_OSM_RAIL0_CLKDOM1_DCVS_LUT_ENTRYr_FREQUENCY, nRow, PLL_SRC);
      *nPllDiv    = HWIO_INFI(APSS_OSM_RAIL0_CLKDOM1_DCVS_LUT_ENTRYr_FREQUENCY, nRow, PLL_DIV_VAL);
      *nDroopCode = HWIO_INFI(APSS_OSM_RAIL0_CLKDOM1_DCVS_LUT_ENTRYr_FREQUENCY, nRow, DROOP_CODE);
      *nCoreCount = HWIO_INFI(APSS_OSM_RAIL0_CLKDOM1_DCVS_LUT_ENTRYr_FREQUENCY, nRow, CORE_CNT_SUPPORTED);
      *nLVal      = HWIO_INFI(APSS_OSM_RAIL0_CLKDOM1_DCVS_LUT_ENTRYr_FREQUENCY, nRow, LVAL);
      break;

    case HAL_CLK_APCS_CPU_CLUSTER1_C0:
    case HAL_CLK_APCS_CPU_CLUSTER1_C1:
    case HAL_CLK_APCS_CPU_CLUSTER1_C2:
    case HAL_CLK_APCS_CPU_CLUSTER1_C3:
      *nIndex     = HWIO_INFI(APSS_OSM_RAIL1_CLKDOM0_DCVS_LUT_ENTRYr_FREQUENCY, nRow, INDEX);
      *nPllSource = HWIO_INFI(APSS_OSM_RAIL1_CLKDOM0_DCVS_LUT_ENTRYr_FREQUENCY, nRow, PLL_SRC);
      *nPllDiv    = HWIO_INFI(APSS_OSM_RAIL1_CLKDOM0_DCVS_LUT_ENTRYr_FREQUENCY, nRow, PLL_DIV_VAL);
      *nDroopCode = HWIO_INFI(APSS_OSM_RAIL1_CLKDOM0_DCVS_LUT_ENTRYr_FREQUENCY, nRow, DROOP_CODE);
      *nCoreCount = HWIO_INFI(APSS_OSM_RAIL1_CLKDOM0_DCVS_LUT_ENTRYr_FREQUENCY, nRow, CORE_CNT_SUPPORTED);
      *nLVal      = HWIO_INFI(APSS_OSM_RAIL1_CLKDOM0_DCVS_LUT_ENTRYr_FREQUENCY, nRow, LVAL);
      break;

    case HAL_CLK_APCS_CPU_L3:
      *nIndex     = HWIO_INFI(APSS_OSM_RAIL0_CLKDOM0_DCVS_LUT_ENTRYr_FREQUENCY, nRow, INDEX);
      *nPllSource = HWIO_INFI(APSS_OSM_RAIL0_CLKDOM0_DCVS_LUT_ENTRYr_FREQUENCY, nRow, PLL_SRC);
      *nPllDiv    = HWIO_INFI(APSS_OSM_RAIL0_CLKDOM0_DCVS_LUT_ENTRYr_FREQUENCY, nRow, PLL_DIV_VAL);
      *nDroopCode = HWIO_INFI(APSS_OSM_RAIL0_CLKDOM0_DCVS_LUT_ENTRYr_FREQUENCY, nRow, DROOP_CODE);
      *nCoreCount = HWIO_INFI(APSS_OSM_RAIL0_CLKDOM0_DCVS_LUT_ENTRYr_FREQUENCY, nRow, CORE_CNT_SUPPORTED);
      *nLVal      = HWIO_INFI(APSS_OSM_RAIL0_CLKDOM0_DCVS_LUT_ENTRYr_FREQUENCY, nRow, LVAL);
      break;

    default:
      return FALSE;
  }

  return TRUE;

} /* END HAL_clk_OSM_LUT_READ_FREQUENCY */


/* ===========================================================================
**  Function : HAL_clk_OSM_LUT_READ_VOLTAGE
**
** ======================================================================== */

boolean HAL_clk_OSM_LUT_READ_VOLTAGE
(
  HAL_clk_APCSCPUType  eCore,
  uint32               nRow,
  uint32              *nMemAccLevel,
  uint32              *nVirtualCorner,
  uint32              *nOpenLoopVoltage
)
{
  /* Sanity check */
  if (eCore            >= HAL_CLK_APCS_CPU_TOTAL ||
      nRow             >= OSM_LUT_MAX_ROWS       ||
      nMemAccLevel     == NULL                   ||
      nVirtualCorner   == NULL                   ||
      nOpenLoopVoltage == NULL                   )
  {
    return FALSE;
  }

  /* Program the hardware. */
  switch(eCore)
  {
    case HAL_CLK_APCS_CPU_CLUSTER0_C0:
    case HAL_CLK_APCS_CPU_CLUSTER0_C1:
    case HAL_CLK_APCS_CPU_CLUSTER0_C2:
    case HAL_CLK_APCS_CPU_CLUSTER0_C3:
      *nMemAccLevel     = HWIO_INFI(APSS_OSM_RAIL0_CLKDOM1_DCVS_LUT_ENTRYr_VOLTAGE, nRow, MEM_ACC_LEVEL);
      *nVirtualCorner   = HWIO_INFI(APSS_OSM_RAIL0_CLKDOM1_DCVS_LUT_ENTRYr_VOLTAGE, nRow, VIRTUAL_CORNER);
      *nOpenLoopVoltage = HWIO_INFI(APSS_OSM_RAIL0_CLKDOM1_DCVS_LUT_ENTRYr_VOLTAGE, nRow, OPEN_LOOP_VOLTAGE);
      break;

    case HAL_CLK_APCS_CPU_CLUSTER1_C0:
    case HAL_CLK_APCS_CPU_CLUSTER1_C1:
    case HAL_CLK_APCS_CPU_CLUSTER1_C2:
    case HAL_CLK_APCS_CPU_CLUSTER1_C3:
      *nMemAccLevel     = HWIO_INFI(APSS_OSM_RAIL1_CLKDOM0_DCVS_LUT_ENTRYr_VOLTAGE, nRow, MEM_ACC_LEVEL);
      *nVirtualCorner   = HWIO_INFI(APSS_OSM_RAIL1_CLKDOM0_DCVS_LUT_ENTRYr_VOLTAGE, nRow, VIRTUAL_CORNER);
      *nOpenLoopVoltage = HWIO_INFI(APSS_OSM_RAIL1_CLKDOM0_DCVS_LUT_ENTRYr_VOLTAGE, nRow, OPEN_LOOP_VOLTAGE);
      break;

    case HAL_CLK_APCS_CPU_L3:
      *nMemAccLevel     = HWIO_INFI(APSS_OSM_RAIL0_CLKDOM0_DCVS_LUT_ENTRYr_VOLTAGE, nRow, MEM_ACC_LEVEL);
      *nVirtualCorner   = HWIO_INFI(APSS_OSM_RAIL0_CLKDOM0_DCVS_LUT_ENTRYr_VOLTAGE, nRow, VIRTUAL_CORNER);
      *nOpenLoopVoltage = HWIO_INFI(APSS_OSM_RAIL0_CLKDOM0_DCVS_LUT_ENTRYr_VOLTAGE, nRow, OPEN_LOOP_VOLTAGE);
      break;

    default:
      return FALSE;
  }

  return TRUE;

} /* END HAL_clk_OSM_LUT_READ_VOLTAGE */


/* ===========================================================================
**  Function : HAL_clk_OSM_LUT_READ_PLL_OVERRIDES
**
** ======================================================================== */

boolean HAL_clk_OSM_LUT_READ_PLL_OVERRIDES
(
  HAL_clk_APCSCPUType  eCore,
  uint32               nRow,
  uint32              *nLVal,
  uint32              *nCcSafeFrequency,
  uint32              *nDcvsSafeFrequency
)
{
  /* Sanity check */
  if (eCore              >= HAL_CLK_APCS_CPU_TOTAL ||
      nRow               >= OSM_LUT_MAX_ROWS       ||
      nLVal              == NULL                   ||
      nCcSafeFrequency   == NULL                   ||
      nDcvsSafeFrequency == NULL                   )
  {
    return FALSE;
  }

  /* Program the hardware. */
  switch(eCore)
  {
    case HAL_CLK_APCS_CPU_CLUSTER0_C0:
    case HAL_CLK_APCS_CPU_CLUSTER0_C1:
    case HAL_CLK_APCS_CPU_CLUSTER0_C2:
    case HAL_CLK_APCS_CPU_CLUSTER0_C3:
      *nLVal              = HWIO_INFI(APSS_OSM_RAIL0_CLKDOM1_DCVS_LUT_ENTRYr_PLL_OVERRIDES, nRow, ONE_BY_L_VAL);
      *nCcSafeFrequency   = HWIO_INFI(APSS_OSM_RAIL0_CLKDOM1_DCVS_LUT_ENTRYr_PLL_OVERRIDES, nRow, CC_SAFE_L_VAL);
      *nDcvsSafeFrequency = HWIO_INFI(APSS_OSM_RAIL0_CLKDOM1_DCVS_LUT_ENTRYr_PLL_OVERRIDES, nRow, DCVS_SAFE_L_VAL);
      break;

    case HAL_CLK_APCS_CPU_CLUSTER1_C0:
    case HAL_CLK_APCS_CPU_CLUSTER1_C1:
    case HAL_CLK_APCS_CPU_CLUSTER1_C2:
    case HAL_CLK_APCS_CPU_CLUSTER1_C3:
      *nLVal              = HWIO_INFI(APSS_OSM_RAIL1_CLKDOM0_DCVS_LUT_ENTRYr_PLL_OVERRIDES, nRow, ONE_BY_L_VAL);
      *nCcSafeFrequency   = HWIO_INFI(APSS_OSM_RAIL1_CLKDOM0_DCVS_LUT_ENTRYr_PLL_OVERRIDES, nRow, CC_SAFE_L_VAL);
      *nDcvsSafeFrequency = HWIO_INFI(APSS_OSM_RAIL1_CLKDOM0_DCVS_LUT_ENTRYr_PLL_OVERRIDES, nRow, DCVS_SAFE_L_VAL);
      break;

    case HAL_CLK_APCS_CPU_L3:
      *nLVal              = HWIO_INFI(APSS_OSM_RAIL0_CLKDOM0_DCVS_LUT_ENTRYr_PLL_OVERRIDES, nRow, ONE_BY_L_VAL);
      *nCcSafeFrequency   = HWIO_INFI(APSS_OSM_RAIL0_CLKDOM0_DCVS_LUT_ENTRYr_PLL_OVERRIDES, nRow, CC_SAFE_L_VAL);
      *nDcvsSafeFrequency = HWIO_INFI(APSS_OSM_RAIL0_CLKDOM0_DCVS_LUT_ENTRYr_PLL_OVERRIDES, nRow, DCVS_SAFE_L_VAL);
      break;

    default:
      return FALSE;
  }

  return TRUE;

} /* END HAL_clk_OSM_LUT_READ_PLL_OVERRIDES */


/* ===========================================================================
**  Function : HAL_clk_OSM_LUT_READ_SPARE_0
**
** ======================================================================== */

boolean HAL_clk_OSM_LUT_READ_SPARE_0
(
  HAL_clk_APCSCPUType  eCore,
  uint32               nRow,
  uint32              *nSpare
)
{
  /* Sanity check */
  if (eCore  >= HAL_CLK_APCS_CPU_TOTAL ||
      nRow   >= OSM_LUT_MAX_ROWS       ||
      nSpare == NULL                   )
  {
    return FALSE;
  }

  /* Program the hardware. */
  switch(eCore)
  {
    case HAL_CLK_APCS_CPU_CLUSTER0_C0:
    case HAL_CLK_APCS_CPU_CLUSTER0_C1:
    case HAL_CLK_APCS_CPU_CLUSTER0_C2:
    case HAL_CLK_APCS_CPU_CLUSTER0_C3:
      *nSpare = HWIO_INFI(APSS_OSM_RAIL0_CLKDOM1_DCVS_LUT_ENTRYr_SPARE_0, nRow, SPARE_0);
      break;

    case HAL_CLK_APCS_CPU_CLUSTER1_C0:
    case HAL_CLK_APCS_CPU_CLUSTER1_C1:
    case HAL_CLK_APCS_CPU_CLUSTER1_C2:
    case HAL_CLK_APCS_CPU_CLUSTER1_C3:
      *nSpare = HWIO_INFI(APSS_OSM_RAIL1_CLKDOM0_DCVS_LUT_ENTRYr_SPARE_0, nRow, SPARE_0);
      break;

    case HAL_CLK_APCS_CPU_L3:
      *nSpare = HWIO_INFI(APSS_OSM_RAIL0_CLKDOM0_DCVS_LUT_ENTRYr_SPARE_0, nRow, SPARE_0);
      break;

    default:
      return FALSE;
  }

  return TRUE;

} /* END HAL_clk_OSM_LUT_READ_SPARE_0 */


/* ===========================================================================
**  Function : HAL_clk_OSM_LUT_CONFIG_FREQUENCY
**
** ======================================================================== */

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
)
{
  /* Sanity check */
  if (eCore      >= HAL_CLK_APCS_CPU_TOTAL ||
      nRow       >= OSM_LUT_MAX_ROWS       ||
      nIndex     >= OSM_LUT_MAX_ROWS       ||
      nPllSource >= 2                      ||
      nPllDiv    >= 4                      ||
      nDroopCode >  0x3f                   ||
      nCoreCount >  4                      ||
      nLVal      >  0xff                   )
  {
    return FALSE;
  }

  /* Program the hardware. */
  switch(eCore)
  {
    case HAL_CLK_APCS_CPU_CLUSTER0_C0:
    case HAL_CLK_APCS_CPU_CLUSTER0_C1:
    case HAL_CLK_APCS_CPU_CLUSTER0_C2:
    case HAL_CLK_APCS_CPU_CLUSTER0_C3:
      HWIO_OUTFI(APSS_OSM_RAIL0_CLKDOM1_DCVS_LUT_ENTRYr_FREQUENCY, nRow, INDEX,              nIndex);
      HWIO_OUTFI(APSS_OSM_RAIL0_CLKDOM1_DCVS_LUT_ENTRYr_FREQUENCY, nRow, PLL_SRC,            nPllSource);
      HWIO_OUTFI(APSS_OSM_RAIL0_CLKDOM1_DCVS_LUT_ENTRYr_FREQUENCY, nRow, PLL_DIV_VAL,        nPllDiv);
      HWIO_OUTFI(APSS_OSM_RAIL0_CLKDOM1_DCVS_LUT_ENTRYr_FREQUENCY, nRow, DROOP_CODE,         nDroopCode);
      HWIO_OUTFI(APSS_OSM_RAIL0_CLKDOM1_DCVS_LUT_ENTRYr_FREQUENCY, nRow, CORE_CNT_SUPPORTED, nCoreCount);
      HWIO_OUTFI(APSS_OSM_RAIL0_CLKDOM1_DCVS_LUT_ENTRYr_FREQUENCY, nRow, LVAL,               nLVal);
      break;

    case HAL_CLK_APCS_CPU_CLUSTER1_C0:
    case HAL_CLK_APCS_CPU_CLUSTER1_C1:
    case HAL_CLK_APCS_CPU_CLUSTER1_C2:
    case HAL_CLK_APCS_CPU_CLUSTER1_C3:
      HWIO_OUTFI(APSS_OSM_RAIL1_CLKDOM0_DCVS_LUT_ENTRYr_FREQUENCY, nRow, INDEX,              nIndex);
      HWIO_OUTFI(APSS_OSM_RAIL1_CLKDOM0_DCVS_LUT_ENTRYr_FREQUENCY, nRow, PLL_SRC,            nPllSource);
      HWIO_OUTFI(APSS_OSM_RAIL1_CLKDOM0_DCVS_LUT_ENTRYr_FREQUENCY, nRow, PLL_DIV_VAL,        nPllDiv);
      HWIO_OUTFI(APSS_OSM_RAIL1_CLKDOM0_DCVS_LUT_ENTRYr_FREQUENCY, nRow, DROOP_CODE,         nDroopCode);
      HWIO_OUTFI(APSS_OSM_RAIL1_CLKDOM0_DCVS_LUT_ENTRYr_FREQUENCY, nRow, CORE_CNT_SUPPORTED, nCoreCount);
      HWIO_OUTFI(APSS_OSM_RAIL1_CLKDOM0_DCVS_LUT_ENTRYr_FREQUENCY, nRow, LVAL,               nLVal);
      break;

    case HAL_CLK_APCS_CPU_L3:
      HWIO_OUTFI(APSS_OSM_RAIL0_CLKDOM0_DCVS_LUT_ENTRYr_FREQUENCY, nRow, INDEX,              nIndex);
      HWIO_OUTFI(APSS_OSM_RAIL0_CLKDOM0_DCVS_LUT_ENTRYr_FREQUENCY, nRow, PLL_SRC,            nPllSource);
      HWIO_OUTFI(APSS_OSM_RAIL0_CLKDOM0_DCVS_LUT_ENTRYr_FREQUENCY, nRow, PLL_DIV_VAL,        nPllDiv);
      HWIO_OUTFI(APSS_OSM_RAIL0_CLKDOM0_DCVS_LUT_ENTRYr_FREQUENCY, nRow, DROOP_CODE,         nDroopCode);
      HWIO_OUTFI(APSS_OSM_RAIL0_CLKDOM0_DCVS_LUT_ENTRYr_FREQUENCY, nRow, CORE_CNT_SUPPORTED, nCoreCount);
      HWIO_OUTFI(APSS_OSM_RAIL0_CLKDOM0_DCVS_LUT_ENTRYr_FREQUENCY, nRow, LVAL,               nLVal);
      break;

    default:
      return FALSE;
  }

  return TRUE;

} /* END HAL_clk_OSM_LUT_CONFIG_FREQUENCY */


/* ===========================================================================
**  Function : HAL_clk_OSM_LUT_CONFIG_VOLTAGE
**
** ======================================================================== */

boolean HAL_clk_OSM_LUT_CONFIG_VOLTAGE
(
  HAL_clk_APCSCPUType eCore,
  uint32              nRow,
  uint32              nMemAccLevel,
  uint32              nVirtualCorner,
  uint32              nOpenLoopVoltage
)
{
  /* Sanity check */
  if (eCore            >= HAL_CLK_APCS_CPU_TOTAL ||
      nRow             >= OSM_LUT_MAX_ROWS       ||
      nMemAccLevel     >  0xf                    ||
      nVirtualCorner   >  0x3f                   ||
      nOpenLoopVoltage >  0xfff                  )
  {
    return FALSE;
  }

  /* Program the hardware. */
  switch(eCore)
  {
    case HAL_CLK_APCS_CPU_CLUSTER0_C0:
    case HAL_CLK_APCS_CPU_CLUSTER0_C1:
    case HAL_CLK_APCS_CPU_CLUSTER0_C2:
    case HAL_CLK_APCS_CPU_CLUSTER0_C3:
      HWIO_OUTFI(APSS_OSM_RAIL0_CLKDOM1_DCVS_LUT_ENTRYr_VOLTAGE, nRow, MEM_ACC_LEVEL,     nMemAccLevel);
      HWIO_OUTFI(APSS_OSM_RAIL0_CLKDOM1_DCVS_LUT_ENTRYr_VOLTAGE, nRow, VIRTUAL_CORNER,    nVirtualCorner);
      HWIO_OUTFI(APSS_OSM_RAIL0_CLKDOM1_DCVS_LUT_ENTRYr_VOLTAGE, nRow, OPEN_LOOP_VOLTAGE, nOpenLoopVoltage);
      break;

    case HAL_CLK_APCS_CPU_CLUSTER1_C0:
    case HAL_CLK_APCS_CPU_CLUSTER1_C1:
    case HAL_CLK_APCS_CPU_CLUSTER1_C2:
    case HAL_CLK_APCS_CPU_CLUSTER1_C3:
      HWIO_OUTFI(APSS_OSM_RAIL1_CLKDOM0_DCVS_LUT_ENTRYr_VOLTAGE, nRow, MEM_ACC_LEVEL,     nMemAccLevel);
      HWIO_OUTFI(APSS_OSM_RAIL1_CLKDOM0_DCVS_LUT_ENTRYr_VOLTAGE, nRow, VIRTUAL_CORNER,    nVirtualCorner);
      HWIO_OUTFI(APSS_OSM_RAIL1_CLKDOM0_DCVS_LUT_ENTRYr_VOLTAGE, nRow, OPEN_LOOP_VOLTAGE, nOpenLoopVoltage);
      break;

    case HAL_CLK_APCS_CPU_L3:
      HWIO_OUTFI(APSS_OSM_RAIL0_CLKDOM0_DCVS_LUT_ENTRYr_VOLTAGE, nRow, MEM_ACC_LEVEL,     nMemAccLevel);
      HWIO_OUTFI(APSS_OSM_RAIL0_CLKDOM0_DCVS_LUT_ENTRYr_VOLTAGE, nRow, VIRTUAL_CORNER,    nVirtualCorner);
      HWIO_OUTFI(APSS_OSM_RAIL0_CLKDOM0_DCVS_LUT_ENTRYr_VOLTAGE, nRow, OPEN_LOOP_VOLTAGE, nOpenLoopVoltage);
      break;

    default:
      return FALSE;
  }

  return TRUE;

} /* END HAL_clk_OSM_LUT_CONFIG_VOLTAGE */


/* ===========================================================================
**  Function : HAL_clk_OSM_LUT_CONFIG_PLL_OVERRIDES
**
** ======================================================================== */

boolean HAL_clk_OSM_LUT_CONFIG_PLL_OVERRIDES
(
  HAL_clk_APCSCPUType eCore,
  uint32              nRow,
  uint32              nLVal,
  uint32              nCcSafeFrequency,
  uint32              nDcvsSafeFrequency
)
{
  /* Sanity check */
  if (eCore              >= HAL_CLK_APCS_CPU_TOTAL ||
      nRow               >= OSM_LUT_MAX_ROWS       ||
      nLVal              >  0xff                   ||
      nCcSafeFrequency   >  0xff                   ||
      nDcvsSafeFrequency >  0xff                   )
  {
    return FALSE;
  }

  /* Program the hardware. */
  switch(eCore)
  {
    case HAL_CLK_APCS_CPU_CLUSTER0_C0:
    case HAL_CLK_APCS_CPU_CLUSTER0_C1:
    case HAL_CLK_APCS_CPU_CLUSTER0_C2:
    case HAL_CLK_APCS_CPU_CLUSTER0_C3:
      HWIO_OUTFI(APSS_OSM_RAIL0_CLKDOM1_DCVS_LUT_ENTRYr_PLL_OVERRIDES, nRow, ONE_BY_L_VAL,    1/nLVal);
      HWIO_OUTFI(APSS_OSM_RAIL0_CLKDOM1_DCVS_LUT_ENTRYr_PLL_OVERRIDES, nRow, CC_SAFE_L_VAL,   nCcSafeFrequency);
      HWIO_OUTFI(APSS_OSM_RAIL0_CLKDOM1_DCVS_LUT_ENTRYr_PLL_OVERRIDES, nRow, DCVS_SAFE_L_VAL, nDcvsSafeFrequency);
      break;

    case HAL_CLK_APCS_CPU_CLUSTER1_C0:
    case HAL_CLK_APCS_CPU_CLUSTER1_C1:
    case HAL_CLK_APCS_CPU_CLUSTER1_C2:
    case HAL_CLK_APCS_CPU_CLUSTER1_C3:
      HWIO_OUTFI(APSS_OSM_RAIL1_CLKDOM0_DCVS_LUT_ENTRYr_PLL_OVERRIDES, nRow, ONE_BY_L_VAL,    1/nLVal);
      HWIO_OUTFI(APSS_OSM_RAIL1_CLKDOM0_DCVS_LUT_ENTRYr_PLL_OVERRIDES, nRow, CC_SAFE_L_VAL,   nCcSafeFrequency);
      HWIO_OUTFI(APSS_OSM_RAIL1_CLKDOM0_DCVS_LUT_ENTRYr_PLL_OVERRIDES, nRow, DCVS_SAFE_L_VAL, nDcvsSafeFrequency);
      break;

    case HAL_CLK_APCS_CPU_L3:
      HWIO_OUTFI(APSS_OSM_RAIL0_CLKDOM0_DCVS_LUT_ENTRYr_PLL_OVERRIDES, nRow, ONE_BY_L_VAL,    1/nLVal);
      HWIO_OUTFI(APSS_OSM_RAIL0_CLKDOM0_DCVS_LUT_ENTRYr_PLL_OVERRIDES, nRow, CC_SAFE_L_VAL,   nCcSafeFrequency);
      HWIO_OUTFI(APSS_OSM_RAIL0_CLKDOM0_DCVS_LUT_ENTRYr_PLL_OVERRIDES, nRow, DCVS_SAFE_L_VAL, nDcvsSafeFrequency);
      break;

    default:
      return FALSE;
  }

  return TRUE;

} /* END HAL_clk_OSM_LUT_CONFIG_PLL_OVERRIDES */


/* ===========================================================================
**  Function : HAL_clk_OSM_LUT_CONFIG_SPARE_0
**
** ======================================================================== */

boolean HAL_clk_OSM_LUT_CONFIG_SPARE_0
(
  HAL_clk_APCSCPUType eCore,
  uint32              nRow,
  uint32              nSpare
)
{
  /* Sanity check */
  if (eCore >= HAL_CLK_APCS_CPU_TOTAL ||
      nRow  >= OSM_LUT_MAX_ROWS       )
  {
    return FALSE;
  }

  /* Program the hardware. */
  switch(eCore)
  {
    case HAL_CLK_APCS_CPU_CLUSTER0_C0:
    case HAL_CLK_APCS_CPU_CLUSTER0_C1:
    case HAL_CLK_APCS_CPU_CLUSTER0_C2:
    case HAL_CLK_APCS_CPU_CLUSTER0_C3:
      HWIO_OUTFI(APSS_OSM_RAIL0_CLKDOM1_DCVS_LUT_ENTRYr_SPARE_0, nRow, SPARE_0, nSpare);
      break;

    case HAL_CLK_APCS_CPU_CLUSTER1_C0:
    case HAL_CLK_APCS_CPU_CLUSTER1_C1:
    case HAL_CLK_APCS_CPU_CLUSTER1_C2:
    case HAL_CLK_APCS_CPU_CLUSTER1_C3:
      HWIO_OUTFI(APSS_OSM_RAIL1_CLKDOM0_DCVS_LUT_ENTRYr_SPARE_0, nRow, SPARE_0, nSpare);
      break;

    case HAL_CLK_APCS_CPU_L3:
      HWIO_OUTFI(APSS_OSM_RAIL0_CLKDOM0_DCVS_LUT_ENTRYr_SPARE_0, nRow, SPARE_0, nSpare);
      break;

    default:
      return FALSE;
  }

  return TRUE;

} /* END HAL_clk_OSM_LUT_CONFIG_SPARE_0 */


/* ===========================================================================
**  Function : HAL_clk_OSM_ENABLE
**
** ======================================================================== */

boolean HAL_clk_OSM_ENABLE
(
  HAL_clk_APCSCPUType eCore
)
{
  /* Sanity check */
  if (eCore >= HAL_CLK_APCS_CPU_TOTAL)
  {
    return FALSE;
  }

// TODO: need access control change to all the registers below
#if 0
  /* Program the hardware. */
  HWIO_OUTF(APSS_OSM_TOP_GLOBAL_OSM_ENABLE, GLOBAL_ENABLE_OSM, 1);
#endif

  switch(eCore)
  {
    case HAL_CLK_APCS_CPU_CLUSTER0_C0:
    case HAL_CLK_APCS_CPU_CLUSTER0_C1:
    case HAL_CLK_APCS_CPU_CLUSTER0_C2:
    case HAL_CLK_APCS_CPU_CLUSTER0_C3:
      /* Program the hardware. */
      HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM1_OSM_ENABLE, ENABLE_OSM, 1);
      break;

    case HAL_CLK_APCS_CPU_CLUSTER1_C0:
    case HAL_CLK_APCS_CPU_CLUSTER1_C1:
    case HAL_CLK_APCS_CPU_CLUSTER1_C2:
    case HAL_CLK_APCS_CPU_CLUSTER1_C3:
      /* Program the hardware. */
      HWIO_OUTF(APSS_OSM_RAIL1_CLKDOM0_OSM_ENABLE, ENABLE_OSM, 1);
      break;

    case HAL_CLK_APCS_CPU_L3:
      /* Program the hardware. */
      HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM0_OSM_ENABLE, ENABLE_OSM, 1);
      break;

    default:
      return FALSE;
  }

  return TRUE;

} /* END HAL_clk_OSM_ENABLE */


/* ===========================================================================
**  Function : HAL_clk_IS_OSM_ENABLED
**
** ======================================================================== */

boolean HAL_clk_IS_OSM_ENABLED()
{
  if (HWIO_INF(APSS_OSM_RAIL0_CLKDOM1_OSM_ENABLE, ENABLE_OSM) ||
      HWIO_INF(APSS_OSM_RAIL1_CLKDOM0_OSM_ENABLE, ENABLE_OSM) ||
      HWIO_INF(APSS_OSM_RAIL0_CLKDOM0_OSM_ENABLE, ENABLE_OSM) )
  {
    return TRUE;
  }

  return FALSE;

} /* END HAL_clk_IS_OSM_ENABLED */


/* ===========================================================================
**  Function : HAL_clk_OSM_SET_DESIRED_PERF_LEVEL
**
** ======================================================================== */

boolean HAL_clk_OSM_SET_DESIRED_PERF_LEVEL
(
  HAL_clk_APCSCPUType eCore,
  uint32              nIndex
)
{
  uint32  nChipVersion = HAL_clk_GetChipVersion();
  uint32  nOffset = 0;
  uintnt  nAddress;
  uint32  nValue;

  /* Sanity check */
  if (eCore  >= HAL_CLK_APCS_CPU_TOTAL ||
      nIndex >= OSM_LUT_MAX_ROWS       )
  {
    return FALSE;
  }

  /* Some addresses between V1 HW and V2 HW differ by the amount of the nOffset value. */
  if (nChipVersion < HAL_CLK_CHIP_VERSION(2,0))
  {
    nOffset = 0x000001A0;
  }

  switch(eCore)
  {
    case HAL_CLK_APCS_CPU_CLUSTER0_C0:
      /* Prepare the address and the data. */
      nAddress = HWIO_ADDRI(APSS_OSM_RAIL0_CLKDOM1_COREd_PERF_STATE_DESIRED, 0) - nOffset;
      nValue   = HWIO_FVAL (APSS_OSM_RAIL0_CLKDOM1_COREd_PERF_STATE_DESIRED, OS_REQUEST, nIndex);
      break;

    case HAL_CLK_APCS_CPU_CLUSTER0_C1:
      /* Prepare the address and the data. */
      nAddress = HWIO_ADDRI(APSS_OSM_RAIL0_CLKDOM1_COREd_PERF_STATE_DESIRED, 1) - nOffset;
      nValue   = HWIO_FVAL (APSS_OSM_RAIL0_CLKDOM1_COREd_PERF_STATE_DESIRED, OS_REQUEST, nIndex);
      break;

    case HAL_CLK_APCS_CPU_CLUSTER0_C2:
      /* Prepare the address and the data. */
      nAddress = HWIO_ADDRI(APSS_OSM_RAIL0_CLKDOM1_COREd_PERF_STATE_DESIRED, 2) - nOffset;
      nValue   = HWIO_FVAL (APSS_OSM_RAIL0_CLKDOM1_COREd_PERF_STATE_DESIRED, OS_REQUEST, nIndex);
      break;

    case HAL_CLK_APCS_CPU_CLUSTER0_C3:
      /* Prepare the address and the data. */
      nAddress = HWIO_ADDRI(APSS_OSM_RAIL0_CLKDOM1_COREd_PERF_STATE_DESIRED, 3) - nOffset;
      nValue   = HWIO_FVAL (APSS_OSM_RAIL0_CLKDOM1_COREd_PERF_STATE_DESIRED, OS_REQUEST, nIndex);
      break;

    case HAL_CLK_APCS_CPU_CLUSTER1_C0:
      /* Prepare the address and the data. */
      nAddress = HWIO_ADDRI(APSS_OSM_RAIL1_CLKDOM0_COREd_PERF_STATE_DESIRED, 0) - nOffset;
      nValue   = HWIO_FVAL (APSS_OSM_RAIL1_CLKDOM0_COREd_PERF_STATE_DESIRED, OS_REQUEST, nIndex);
      break;

    case HAL_CLK_APCS_CPU_CLUSTER1_C1:
      /* Prepare the address and the data. */
      nAddress = HWIO_ADDRI(APSS_OSM_RAIL1_CLKDOM0_COREd_PERF_STATE_DESIRED, 1) - nOffset;
      nValue   = HWIO_FVAL (APSS_OSM_RAIL1_CLKDOM0_COREd_PERF_STATE_DESIRED, OS_REQUEST, nIndex);
      break;

    case HAL_CLK_APCS_CPU_CLUSTER1_C2:
      /* Prepare the address and the data. */
      nAddress = HWIO_ADDRI(APSS_OSM_RAIL1_CLKDOM0_COREd_PERF_STATE_DESIRED, 2) - nOffset;
      nValue   = HWIO_FVAL (APSS_OSM_RAIL1_CLKDOM0_COREd_PERF_STATE_DESIRED, OS_REQUEST, nIndex);
      break;

    case HAL_CLK_APCS_CPU_CLUSTER1_C3:
      /* Prepare the address and the data. */
      nAddress = HWIO_ADDRI(APSS_OSM_RAIL1_CLKDOM0_COREd_PERF_STATE_DESIRED, 3) - nOffset;
      nValue   = HWIO_FVAL (APSS_OSM_RAIL1_CLKDOM0_COREd_PERF_STATE_DESIRED, OS_REQUEST, nIndex);
      break;

    case HAL_CLK_APCS_CPU_L3:
      /* Prepare the address and the data. */
      nAddress = HWIO_ADDRI(APSS_OSM_RAIL0_CLKDOM0_COREd_PERF_STATE_DESIRED, 0) - nOffset;
      nValue   = HWIO_FVAL (APSS_OSM_RAIL0_CLKDOM0_COREd_PERF_STATE_DESIRED, OS_REQUEST, nIndex);
      break;

    default:
      return FALSE;
  }

  /* Program the hardware. */
  outpdw(nAddress, nValue);

  return TRUE;

} /* END HAL_clk_OSM_SET_DESIRED_PERF_LEVEL */


/* ===========================================================================
**  Function : HAL_clk_OSM_ApplyVirtualCorner
**
** ======================================================================== */

boolean HAL_clk_OSM_ApplyVirtualCorner
(
  HAL_clk_APCSCPUType          eCore,
  HAL_clk_OsmVirtualCornerType eCorner,
  uint32                       nCorner
)
{
  /* Sanity check */
  if (eCore   >= HAL_CLK_APCS_CPU_TOTAL          ||
      eCorner >= HAL_CLK_OSM_VIRTUAL_CORNER_TOTAL)
  {
    return FALSE;
  }

  switch(eCore)
  {
    case HAL_CLK_APCS_CPU_CLUSTER0_C0:
    case HAL_CLK_APCS_CPU_CLUSTER0_C1:
    case HAL_CLK_APCS_CPU_CLUSTER0_C2:
    case HAL_CLK_APCS_CPU_CLUSTER0_C3:
      /* Program the hardware. */
      if (eCorner == HAL_CLK_OSM_VIRTUAL_CORNER_APM)
      {
        HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM1_APM_CROSSOVER_VIRTUAL_CORNER, VIRTUAL_CORNER, nCorner);
      }
      else
      {
        HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM1_MEMACC_CROSSOVER_VIRTUAL_CORNER, VIRTUAL_CORNER, nCorner);
      }
      break;

    case HAL_CLK_APCS_CPU_CLUSTER1_C0:
    case HAL_CLK_APCS_CPU_CLUSTER1_C1:
    case HAL_CLK_APCS_CPU_CLUSTER1_C2:
    case HAL_CLK_APCS_CPU_CLUSTER1_C3:
      /* Program the hardware. */
      if (eCorner == HAL_CLK_OSM_VIRTUAL_CORNER_APM)
      {
        HWIO_OUTF(APSS_OSM_RAIL1_CLKDOM0_APM_CROSSOVER_VIRTUAL_CORNER, VIRTUAL_CORNER, nCorner);
      }
      else
      {
        HWIO_OUTF(APSS_OSM_RAIL1_CLKDOM0_MEMACC_CROSSOVER_VIRTUAL_CORNER, VIRTUAL_CORNER, nCorner);
      }
      break;

    case HAL_CLK_APCS_CPU_L3:
      /* Program the hardware. */
      if (eCorner == HAL_CLK_OSM_VIRTUAL_CORNER_APM)
      {
        HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM0_APM_CROSSOVER_VIRTUAL_CORNER, VIRTUAL_CORNER, nCorner);
      }
      else
      {
        HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM0_MEMACC_CROSSOVER_VIRTUAL_CORNER, VIRTUAL_CORNER, nCorner);
      }
      break;

    default:
      return FALSE;
  }

  return TRUE;

} /* END HAL_clk_OSM_ApplyVirtualCorner */


/* ===========================================================================
**  Function : HAL_clk_OSM_Configure
**
** ======================================================================== */

boolean HAL_clk_OSM_Configure
(
  HAL_clk_APCSCPUType eCore
)
{
  /* Sanity check */
  if (eCore >= HAL_CLK_APCS_CPU_TOTAL)
  {
    return FALSE;
  }

  switch(eCore)
  {
    case HAL_CLK_APCS_CPU_CLUSTER0_C0:
    case HAL_CLK_APCS_CPU_CLUSTER0_C1:
    case HAL_CLK_APCS_CPU_CLUSTER0_C2:
    case HAL_CLK_APCS_CPU_CLUSTER0_C3:
      HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM1_CORE_DCVS_CTRL,                              PER_CORE_DCVS_EN,                         0x00000000);  //0x00000001);
      HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM1_PLL_MIN_FREQ,                                PLL_MIN_FREQ,                             0x00000021);
      HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM1_SPM_CORE_COUNT_DCVS_DISABLE,                 CORE_COUNT_BASED_DCVS_VOTE_DISABLE,       0x00000000);
      HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM1_SPM_CORE_COUNT_INC_HYSTERESIS,               CORE_COUNT_INCREASE_TIMER,                0x00000064);
      HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM1_SPM_CORE_COUNT_DEC_HYSTERESIS,               CORE_COUNT_DECREASE_TIMER,                0x00002710);
      HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM1_CORE_COUNT_ZERO_BEHAV_CTRL,                  USE_OVERRIDE_VALUES_FOR_CORE_COUNT_ZERO,  0x00000001);
      HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM1_CORE_COUNT_ZERO_BEHAV_CTRL,                  OVERRIDE_INDEX,                           0x00000000);
      HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM1_SPM_CORE_COUNT_CTRL,                         CORES_OUT_OF_RESET,                       0x0000000F);
      HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM1_LLM_INTF_DCVS_DISABLE,                       LLM_FREQ_VOTE_DISABLE,                    0x00000000);
      HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM1_LLM_FREQ_VOTE_INC_HYSTERESIS,                FREQ_INCREASE_TIMER,                      0x00000064);
      HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM1_LLM_FREQ_VOTE_DEC_HYSTERESIS,                FREQ_DECREASE_TIMER,                      0x00007FFF);
      HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM1_LLM_INTF_DCVS_DISABLE,                       LLM_VOL_VOTE_DISABLE,                     0x00000000);
      HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM1_LLM_VOLTAGE_VOTE_INC_HYSTERESIS,             VOLTAGE_INCREASE_TIMER,                   0x00000064);
      HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM1_LLM_VOLTAGE_VOTE_DEC_HYSTERESIS,             VOLTAGE_DECREASE_TIMER,                   0x00007FFF);
      HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM1_CPR_CORE_COUNT_BASED_VMIN_REDUCTION_ENABLE,  CORE_COUNT_BASED_VMIN_REDUCTION_ENABLE,   0x00000000);
      HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM1_CPR_VMIN_REDUCTION_TIMER_CTRL,               HYS_TIMER,                                0x000003E8);
      HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM1_CPR_CORE_COUNT_BASED_VMIN_REDUCTION_ENABLE,  MIN_CPR_VC,                               0x00000000);
      HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM1_CPR_CORE_COUNT_BASED_VMIN_REDUCTION_ENABLE,  MAX_CPR_VC,                               0x0000003F);
      HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM1_PDN_FSM_CTRL,                                IGNORE_PLL_LOCK,                          0x00000000);
      HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM1_PDN_FSM_CTRL,                                DELTA_DEXT_VAL,                           0x0000000A);
      HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM1_CC_BOOST_FSM_TIMERS_REG2,                    DELTA_DEXT,                               0x00000064);
      HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM1_PDN_FSM_CTRL,                                CC_BOOST_FSM_EN,                          0x00000001);
      HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM1_CC_BOOST_FSM_TIMERS_REG0,                    WAIT_FOR_UP_LOCK,                         0x000003E8);
      HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM1_CC_BOOST_FSM_TIMERS_REG0,                    WAIT_FOR_SAFE_FREQ,                       0x000001F4);
      HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM1_CC_BOOST_FSM_TIMERS_REG1,                    WAIT_FOR_LOCK_SAFE_FREQ,                  0x000003E8);
      HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM1_CC_BOOST_FSM_TIMERS_REG1,                    WAIT_FOR_DWN_LOCK,                        0x000003E8);
      HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM1_PDN_FSM_CTRL,                                DCVS_BOOST_FSM_EN,                        0x00000001);
      HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM1_DCVS_BOOST_FSM_TIMERS_REG0,                  WAIT_FOR_UP_LOCK,                         0x00000000);
      HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM1_DCVS_BOOST_FSM_TIMERS_REG0,                  WAIT_FOR_SAFE_FREQ,                       0x00000064);
      HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM1_DCVS_BOOST_FSM_TIMERS_REG1,                  WAIT_FOR_LOCK_SAFE_FREQ,                  0x000001F4);
      HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM1_DCVS_BOOST_FSM_TIMERS_REG1,                  WAIT_FOR_DWN_LOCK,                        0x00000000);
      HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM1_DCVS_BOOST_FSM_TIMERS_REG2,                  DELTA_DEXT,                               0x00000064);
      HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM1_PDN_FSM_CTRL,                                PS_BOOST_FSM_EN,                          0x00000001);
      HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM1_PS_BOOST_FSM_TIMERS_REG0,                    WAIT_FOR_UP_LOCK,                         0x000001F4);
      HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM1_PS_BOOST_FSM_TIMERS_REG0,                    WAIT_FOR_SAFE_FREQ,                       0x00000064);
      HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM1_PS_BOOST_FSM_TIMERS_REG1,                    WAIT_FOR_LOCK_SAFE_FREQ,                  0x000001F4);
      HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM1_PS_BOOST_FSM_TIMERS_REG1,                    WAIT_FOR_DWN_LOCK,                        0x00000064);
      HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM1_PS_BOOST_FSM_TIMERS_REG2,                    DELTA_DEXT,                               0x00000064);
      HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM1_BOOST_PROG_SYNC_DELAY,                       PROG_SYNC_DELAY,                          0x00000002);
      HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM1_DROOP_CTRL,                                  RESYNC_CTRL,                              0x00000001);
      HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM1_DROOP_PROG_SYNC_DELAY,                       PROG_SYNC_DELAY,                          0x00000001);
      HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM1_DROOP_CTRL,                                  DELTA_DROOP_CODE,                         0x00000002);
      HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM1_DROOP_RELEASE_TIMER_CTRL,                    RELEASE_TIMER_VAL,                        0x0000000A);
      HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM1_PDN_FSM_CTRL,                                DCVS_DROOP_FSM_EN,                        0x00000001);
      HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM1_DCVS_DROOP_TIMER_CTRL,                       DCVS_DROOP_TIMER_VAL,                     0x0000000F);
      HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM1_DROOP_CTRL,                                  DCVS_DROOP_CODE,                          0x00000008);
      HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM1_CYCLE_COUNTER_CTRL,                          CYCLE_COUNTERS_EN,                        0x00000001);
      HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM1_CYCLE_COUNTER_CTRL,                          OSM_CLK_TO_XO_RATIO,                      0x00000004);
      HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM1_CYCLE_COUNTER_CTRL,                          USE_XO_EDGE_FOR_COUNTER,                  0x00000001);
      break;

    case HAL_CLK_APCS_CPU_CLUSTER1_C0:
    case HAL_CLK_APCS_CPU_CLUSTER1_C1:
    case HAL_CLK_APCS_CPU_CLUSTER1_C2:
    case HAL_CLK_APCS_CPU_CLUSTER1_C3:
      HWIO_OUTF(APSS_OSM_RAIL1_CLKDOM0_CORE_DCVS_CTRL,                              PER_CORE_DCVS_EN,                         0x00000000);  //0x00000001);
      HWIO_OUTF(APSS_OSM_RAIL1_CLKDOM0_PLL_MIN_FREQ,                                PLL_MIN_FREQ,                             0x00000021);
      HWIO_OUTF(APSS_OSM_RAIL1_CLKDOM0_SPM_CORE_COUNT_DCVS_DISABLE,                 CORE_COUNT_BASED_DCVS_VOTE_DISABLE,       0x00000000);
      HWIO_OUTF(APSS_OSM_RAIL1_CLKDOM0_SPM_CORE_COUNT_INC_HYSTERESIS,               CORE_COUNT_INCREASE_TIMER,                0x00000064);
      HWIO_OUTF(APSS_OSM_RAIL1_CLKDOM0_SPM_CORE_COUNT_DEC_HYSTERESIS,               CORE_COUNT_DECREASE_TIMER,                0x00002710);
      HWIO_OUTF(APSS_OSM_RAIL1_CLKDOM0_CORE_COUNT_ZERO_BEHAV_CTRL,                  USE_OVERRIDE_VALUES_FOR_CORE_COUNT_ZERO,  0x00000001);
      HWIO_OUTF(APSS_OSM_RAIL1_CLKDOM0_CORE_COUNT_ZERO_BEHAV_CTRL,                  OVERRIDE_INDEX,                           0x00000000);
      HWIO_OUTF(APSS_OSM_RAIL1_CLKDOM0_SPM_CORE_COUNT_CTRL,                         CORES_OUT_OF_RESET,                       0x0000000F);
      HWIO_OUTF(APSS_OSM_RAIL1_CLKDOM0_LLM_INTF_DCVS_DISABLE,                       LLM_FREQ_VOTE_DISABLE,                    0x00000000);
      HWIO_OUTF(APSS_OSM_RAIL1_CLKDOM0_LLM_FREQ_VOTE_INC_HYSTERESIS,                FREQ_INCREASE_TIMER,                      0x00000064);
      HWIO_OUTF(APSS_OSM_RAIL1_CLKDOM0_LLM_FREQ_VOTE_DEC_HYSTERESIS,                FREQ_DECREASE_TIMER,                      0x00007FFF);
      HWIO_OUTF(APSS_OSM_RAIL1_CLKDOM0_LLM_INTF_DCVS_DISABLE,                       LLM_VOL_VOTE_DISABLE,                     0x00000000);
      HWIO_OUTF(APSS_OSM_RAIL1_CLKDOM0_LLM_VOLTAGE_VOTE_INC_HYSTERESIS,             VOLTAGE_INCREASE_TIMER,                   0x00000064);
      HWIO_OUTF(APSS_OSM_RAIL1_CLKDOM0_LLM_VOLTAGE_VOTE_DEC_HYSTERESIS,             VOLTAGE_DECREASE_TIMER,                   0x00007FFF);
      HWIO_OUTF(APSS_OSM_RAIL1_CLKDOM0_CPR_CORE_COUNT_BASED_VMIN_REDUCTION_ENABLE,  CORE_COUNT_BASED_VMIN_REDUCTION_ENABLE,   0x00000000);
      HWIO_OUTF(APSS_OSM_RAIL1_CLKDOM0_CPR_VMIN_REDUCTION_TIMER_CTRL,               HYS_TIMER,                                0x000003E8);
      HWIO_OUTF(APSS_OSM_RAIL1_CLKDOM0_CPR_CORE_COUNT_BASED_VMIN_REDUCTION_ENABLE,  MIN_CPR_VC,                               0x00000000);
      HWIO_OUTF(APSS_OSM_RAIL1_CLKDOM0_CPR_CORE_COUNT_BASED_VMIN_REDUCTION_ENABLE,  MAX_CPR_VC,                               0x0000003F);
      HWIO_OUTF(APSS_OSM_RAIL1_CLKDOM0_PDN_FSM_CTRL,                                IGNORE_PLL_LOCK,                          0x00000000);
      HWIO_OUTF(APSS_OSM_RAIL1_CLKDOM0_PDN_FSM_CTRL,                                DELTA_DEXT_VAL,                           0x0000000A);
      HWIO_OUTF(APSS_OSM_RAIL1_CLKDOM0_CC_BOOST_FSM_TIMERS_REG2,                    DELTA_DEXT,                               0x00000064);
      HWIO_OUTF(APSS_OSM_RAIL1_CLKDOM0_PDN_FSM_CTRL,                                CC_BOOST_FSM_EN,                          0x00000001);
      HWIO_OUTF(APSS_OSM_RAIL1_CLKDOM0_CC_BOOST_FSM_TIMERS_REG0,                    WAIT_FOR_UP_LOCK,                         0x000003E8);
      HWIO_OUTF(APSS_OSM_RAIL1_CLKDOM0_CC_BOOST_FSM_TIMERS_REG0,                    WAIT_FOR_SAFE_FREQ,                       0x000001F4);
      HWIO_OUTF(APSS_OSM_RAIL1_CLKDOM0_CC_BOOST_FSM_TIMERS_REG1,                    WAIT_FOR_LOCK_SAFE_FREQ,                  0x000003E8);
      HWIO_OUTF(APSS_OSM_RAIL1_CLKDOM0_CC_BOOST_FSM_TIMERS_REG1,                    WAIT_FOR_DWN_LOCK,                        0x000003E8);
      HWIO_OUTF(APSS_OSM_RAIL1_CLKDOM0_PDN_FSM_CTRL,                                DCVS_BOOST_FSM_EN,                        0x00000001);
      HWIO_OUTF(APSS_OSM_RAIL1_CLKDOM0_DCVS_BOOST_FSM_TIMERS_REG0,                  WAIT_FOR_UP_LOCK,                         0x00000000);
      HWIO_OUTF(APSS_OSM_RAIL1_CLKDOM0_DCVS_BOOST_FSM_TIMERS_REG0,                  WAIT_FOR_SAFE_FREQ,                       0x00000064);
      HWIO_OUTF(APSS_OSM_RAIL1_CLKDOM0_DCVS_BOOST_FSM_TIMERS_REG1,                  WAIT_FOR_LOCK_SAFE_FREQ,                  0x000001F4);
      HWIO_OUTF(APSS_OSM_RAIL1_CLKDOM0_DCVS_BOOST_FSM_TIMERS_REG1,                  WAIT_FOR_DWN_LOCK,                        0x00000000);
      HWIO_OUTF(APSS_OSM_RAIL1_CLKDOM0_DCVS_BOOST_FSM_TIMERS_REG2,                  DELTA_DEXT,                               0x00000064);
      HWIO_OUTF(APSS_OSM_RAIL1_CLKDOM0_PDN_FSM_CTRL,                                PS_BOOST_FSM_EN,                          0x00000001);
      HWIO_OUTF(APSS_OSM_RAIL1_CLKDOM0_PS_BOOST_FSM_TIMERS_REG0,                    WAIT_FOR_UP_LOCK,                         0x000001F4);
      HWIO_OUTF(APSS_OSM_RAIL1_CLKDOM0_PS_BOOST_FSM_TIMERS_REG0,                    WAIT_FOR_SAFE_FREQ,                       0x00000064);
      HWIO_OUTF(APSS_OSM_RAIL1_CLKDOM0_PS_BOOST_FSM_TIMERS_REG1,                    WAIT_FOR_LOCK_SAFE_FREQ,                  0x000001F4);
      HWIO_OUTF(APSS_OSM_RAIL1_CLKDOM0_PS_BOOST_FSM_TIMERS_REG1,                    WAIT_FOR_DWN_LOCK,                        0x00000064);
      HWIO_OUTF(APSS_OSM_RAIL1_CLKDOM0_PS_BOOST_FSM_TIMERS_REG2,                    DELTA_DEXT,                               0x00000064);
      HWIO_OUTF(APSS_OSM_RAIL1_CLKDOM0_BOOST_PROG_SYNC_DELAY,                       PROG_SYNC_DELAY,                          0x00000002);
      HWIO_OUTF(APSS_OSM_RAIL1_CLKDOM0_DROOP_CTRL,                                  RESYNC_CTRL,                              0x00000001);
      HWIO_OUTF(APSS_OSM_RAIL1_CLKDOM0_DROOP_PROG_SYNC_DELAY,                       PROG_SYNC_DELAY,                          0x00000001);
      HWIO_OUTF(APSS_OSM_RAIL1_CLKDOM0_DROOP_CTRL,                                  DELTA_DROOP_CODE,                         0x00000002);
      HWIO_OUTF(APSS_OSM_RAIL1_CLKDOM0_DROOP_RELEASE_TIMER_CTRL,                    RELEASE_TIMER_VAL,                        0x0000000A);
      HWIO_OUTF(APSS_OSM_RAIL1_CLKDOM0_PDN_FSM_CTRL,                                DCVS_DROOP_FSM_EN,                        0x00000001);
      HWIO_OUTF(APSS_OSM_RAIL1_CLKDOM0_DCVS_DROOP_TIMER_CTRL,                       DCVS_DROOP_TIMER_VAL,                     0x0000000F);
      HWIO_OUTF(APSS_OSM_RAIL1_CLKDOM0_DROOP_CTRL,                                  DCVS_DROOP_CODE,                          0x00000008);
      HWIO_OUTF(APSS_OSM_RAIL1_CLKDOM0_CYCLE_COUNTER_CTRL,                          CYCLE_COUNTERS_EN,                        0x00000001);
      HWIO_OUTF(APSS_OSM_RAIL1_CLKDOM0_CYCLE_COUNTER_CTRL,                          OSM_CLK_TO_XO_RATIO,                      0x00000004);
      HWIO_OUTF(APSS_OSM_RAIL1_CLKDOM0_CYCLE_COUNTER_CTRL,                          USE_XO_EDGE_FOR_COUNTER,                  0x00000001);
      break;

    case HAL_CLK_APCS_CPU_L3:
      HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM0_CORE_DCVS_CTRL,                              PER_CORE_DCVS_EN,                         0x00000000);  //0x00000001);
      HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM0_PLL_MIN_FREQ,                                PLL_MIN_FREQ,                             0x00000021);
      HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM0_SPM_CORE_COUNT_DCVS_DISABLE,                 CORE_COUNT_BASED_DCVS_VOTE_DISABLE,       0x00000000);
      HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM0_SPM_CORE_COUNT_INC_HYSTERESIS,               CORE_COUNT_INCREASE_TIMER,                0x00000064);
      HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM0_SPM_CORE_COUNT_DEC_HYSTERESIS,               CORE_COUNT_DECREASE_TIMER,                0x00002710);
      HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM0_CORE_COUNT_ZERO_BEHAV_CTRL,                  USE_OVERRIDE_VALUES_FOR_CORE_COUNT_ZERO,  0x00000000);
      HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM0_CORE_COUNT_ZERO_BEHAV_CTRL,                  OVERRIDE_INDEX,                           0x00000000);
      HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM0_SPM_CORE_COUNT_CTRL,                         CORES_OUT_OF_RESET,                       0x0000000F);
      HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM0_LLM_INTF_DCVS_DISABLE,                       LLM_FREQ_VOTE_DISABLE,                    0x00000000);
      HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM0_LLM_FREQ_VOTE_INC_HYSTERESIS,                FREQ_INCREASE_TIMER,                      0x00000064);
      HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM0_LLM_FREQ_VOTE_DEC_HYSTERESIS,                FREQ_DECREASE_TIMER,                      0x00007FFF);
      HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM0_LLM_INTF_DCVS_DISABLE,                       LLM_VOL_VOTE_DISABLE,                     0x00000000);
      HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM0_LLM_VOLTAGE_VOTE_INC_HYSTERESIS,             VOLTAGE_INCREASE_TIMER,                   0x00000064);
      HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM0_LLM_VOLTAGE_VOTE_DEC_HYSTERESIS,             VOLTAGE_DECREASE_TIMER,                   0x00007FFF);
      HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM0_CPR_CORE_COUNT_BASED_VMIN_REDUCTION_ENABLE,  CORE_COUNT_BASED_VMIN_REDUCTION_ENABLE,   0x00000000);
      HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM0_CPR_VMIN_REDUCTION_TIMER_CTRL,               HYS_TIMER,                                0x000003E8);
      HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM0_CPR_CORE_COUNT_BASED_VMIN_REDUCTION_ENABLE,  MIN_CPR_VC,                               0x00000000);
      HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM0_CPR_CORE_COUNT_BASED_VMIN_REDUCTION_ENABLE,  MAX_CPR_VC,                               0x0000003F);
      HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM0_PDN_FSM_CTRL,                                IGNORE_PLL_LOCK,                          0x00000000);
      HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM0_PDN_FSM_CTRL,                                DELTA_DEXT_VAL,                           0x0000000A);
      HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM0_CC_BOOST_FSM_TIMERS_REG2,                    DELTA_DEXT,                               0x00000064);
      HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM0_PDN_FSM_CTRL,                                CC_BOOST_FSM_EN,                          0x00000001);
      HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM0_CC_BOOST_FSM_TIMERS_REG0,                    WAIT_FOR_UP_LOCK,                         0x000003E8);
      HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM0_CC_BOOST_FSM_TIMERS_REG0,                    WAIT_FOR_SAFE_FREQ,                       0x000001F4);
      HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM0_CC_BOOST_FSM_TIMERS_REG1,                    WAIT_FOR_LOCK_SAFE_FREQ,                  0x000003E8);
      HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM0_CC_BOOST_FSM_TIMERS_REG1,                    WAIT_FOR_DWN_LOCK,                        0x000003E8);
      HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM0_PDN_FSM_CTRL,                                DCVS_BOOST_FSM_EN,                        0x00000001);
      HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM0_DCVS_BOOST_FSM_TIMERS_REG0,                  WAIT_FOR_UP_LOCK,                         0x00000000);
      HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM0_DCVS_BOOST_FSM_TIMERS_REG0,                  WAIT_FOR_SAFE_FREQ,                       0x00000064);
      HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM0_DCVS_BOOST_FSM_TIMERS_REG1,                  WAIT_FOR_LOCK_SAFE_FREQ,                  0x000001F4);
      HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM0_DCVS_BOOST_FSM_TIMERS_REG1,                  WAIT_FOR_DWN_LOCK,                        0x00000000);
      HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM0_DCVS_BOOST_FSM_TIMERS_REG2,                  DELTA_DEXT,                               0x00000064);
      HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM0_PDN_FSM_CTRL,                                PS_BOOST_FSM_EN,                          0x00000001);
      HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM0_PS_BOOST_FSM_TIMERS_REG0,                    WAIT_FOR_UP_LOCK,                         0x000001F4);
      HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM0_PS_BOOST_FSM_TIMERS_REG0,                    WAIT_FOR_SAFE_FREQ,                       0x00000064);
      HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM0_PS_BOOST_FSM_TIMERS_REG1,                    WAIT_FOR_LOCK_SAFE_FREQ,                  0x000001F4);
      HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM0_PS_BOOST_FSM_TIMERS_REG1,                    WAIT_FOR_DWN_LOCK,                        0x00000064);
      HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM0_PS_BOOST_FSM_TIMERS_REG2,                    DELTA_DEXT,                               0x00000064);
      HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM0_BOOST_PROG_SYNC_DELAY,                       PROG_SYNC_DELAY,                          0x00000002);
      HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM0_DROOP_CTRL,                                  RESYNC_CTRL,                              0x00000001);
      HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM0_DROOP_PROG_SYNC_DELAY,                       PROG_SYNC_DELAY,                          0x00000001);
      HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM0_DROOP_CTRL,                                  DELTA_DROOP_CODE,                         0x00000002);
      HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM0_DROOP_RELEASE_TIMER_CTRL,                    RELEASE_TIMER_VAL,                        0x0000000A);
      HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM0_PDN_FSM_CTRL,                                DCVS_DROOP_FSM_EN,                        0x00000001);
      HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM0_DCVS_DROOP_TIMER_CTRL,                       DCVS_DROOP_TIMER_VAL,                     0x0000000F);
      HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM0_DROOP_CTRL,                                  DCVS_DROOP_CODE,                          0x00000008);
      HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM0_CYCLE_COUNTER_CTRL,                          CYCLE_COUNTERS_EN,                        0x00000001);
      HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM0_CYCLE_COUNTER_CTRL,                          OSM_CLK_TO_XO_RATIO,                      0x00000004);
      HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM0_CYCLE_COUNTER_CTRL,                          USE_XO_EDGE_FOR_COUNTER,                  0x00000001);
      break;

    default:
      return FALSE;
  }

  return TRUE;

} /* END HAL_clk_OSM_Configure */


/* ===========================================================================
**  Function : HAL_clk_OSM_ConfigureACD
**
** ======================================================================== */

boolean HAL_clk_OSM_ConfigureACD
(
  HAL_clk_APCSCPUType eCore
)
{
  /* Sanity check */
  if (eCore >= HAL_CLK_APCS_CPU_TOTAL)
  {
    return FALSE;
  }

  switch(eCore)
  {
    case HAL_CLK_APCS_CPU_CLUSTER0_C0:
    case HAL_CLK_APCS_CPU_CLUSTER0_C1:
    case HAL_CLK_APCS_CPU_CLUSTER0_C2:
    case HAL_CLK_APCS_CPU_CLUSTER0_C3:
      HWIO_OUTF(SILVER_ACD_AVG_CFG_0, AVG_EN,                   0x00000000);
      HWIO_OUTF(SILVER_ACD_AVG_CFG_0, HYSTERESIS_ITER_DECREASE, 0x00000004);
      HWIO_OUTF(SILVER_ACD_AVG_CFG_0, HYSTERESIS_ITER_INCREASE, 0x00000001);
      HWIO_OUTF(SILVER_ACD_AVG_CFG_0, REF_CYCLES,               0x000007A1);
      HWIO_OUTF(SILVER_ACD_AVG_CFG_1, LOST_CYCLES_DIFF,         0x00002710);
      HWIO_OUTF(SILVER_ACD_AVG_CFG_1, LOST_CYCLES_LOWER_BOUND,  0x00004E20);
      HWIO_OUTF(SILVER_ACD_AVG_CFG_2, DVMMGN_ADJ_EN,            0x00000001);
      HWIO_OUTF(SILVER_ACD_AVG_CFG_2, CMP_POINT4,               0x0000000B);
      HWIO_OUTF(SILVER_ACD_AVG_CFG_2, CMP_POINT3,               0x0000000A);
      HWIO_OUTF(SILVER_ACD_AVG_CFG_2, CMP_POINT2,               0x00000007);
      HWIO_OUTF(SILVER_ACD_AVG_CFG_2, CMP_POINT1,               0x00000002);
      HWIO_OUTF(SILVER_ACD_AVG_CFG_2, CMP_POINT0,               0x00000001);
      HWIO_OUTF(SILVER_ACD_AVG_CFG_2, MAX_VOL_REDUCTION,        0x00000002);
      break;

    case HAL_CLK_APCS_CPU_CLUSTER1_C0:
    case HAL_CLK_APCS_CPU_CLUSTER1_C1:
    case HAL_CLK_APCS_CPU_CLUSTER1_C2:
    case HAL_CLK_APCS_CPU_CLUSTER1_C3:
      HWIO_OUTF(GOLD_ACD_AVG_CFG_0,   AVG_EN,                   0x00000000);
      HWIO_OUTF(GOLD_ACD_AVG_CFG_0,   HYSTERESIS_ITER_DECREASE, 0x00000004);
      HWIO_OUTF(GOLD_ACD_AVG_CFG_0,   HYSTERESIS_ITER_INCREASE, 0x00000001);
      HWIO_OUTF(GOLD_ACD_AVG_CFG_0,   REF_CYCLES,               0x000007A1);
      HWIO_OUTF(GOLD_ACD_AVG_CFG_1,   LOST_CYCLES_DIFF,         0x00002710);
      HWIO_OUTF(GOLD_ACD_AVG_CFG_1,   LOST_CYCLES_LOWER_BOUND,  0x00004E20);
      HWIO_OUTF(GOLD_ACD_AVG_CFG_2,   DVMMGN_ADJ_EN,            0x00000001);
      HWIO_OUTF(GOLD_ACD_AVG_CFG_2,   CMP_POINT4,               0x0000000B);
      HWIO_OUTF(GOLD_ACD_AVG_CFG_2,   CMP_POINT3,               0x0000000A);
      HWIO_OUTF(GOLD_ACD_AVG_CFG_2,   CMP_POINT2,               0x00000007);
      HWIO_OUTF(GOLD_ACD_AVG_CFG_2,   CMP_POINT1,               0x00000002);
      HWIO_OUTF(GOLD_ACD_AVG_CFG_2,   CMP_POINT0,               0x00000001);
      HWIO_OUTF(GOLD_ACD_AVG_CFG_2,   MAX_VOL_REDUCTION,        0x00000002);
      break;

    case HAL_CLK_APCS_CPU_L3:
      break;

    default:
      return FALSE;
  }

  return TRUE;

} /* END HAL_clk_OSM_ConfigureACD */


/* ===========================================================================
**  Function : HAL_clk_OSM_ConfigureTraceControl
**
** ======================================================================== */

boolean HAL_clk_OSM_ConfigureTraceControl
(
  HAL_clk_OsmTraceType  eOsmTraceType,
  uint32                nOsmTraceValue
)
{
// TODO: need access control change to all the registers below
#if 0
  /*
   * Write the corresponding register/value.
   */
  switch (eOsmTraceType)
  {
    case HAL_CLK_OSM_GLOBAL_TRACE_ENABLE:
      if (nOsmTraceValue <= 1)
      {
        HWIO_OUTF(APSS_OSM_TOP_GLOBAL_TRACE_CTRL, GLOBAL_TRACE_ENABLE, nOsmTraceValue);
      }
      else
      {
        return FALSE;
      }
      break;

    case HAL_CLK_OSM_CLKDOM_TRACE_ENABLE:
      if (nOsmTraceValue <= 0x3ff)
      {
        HWIO_OUTF(APSS_OSM_TOP_GLOBAL_TRACE_CTRL, CLKDOM_TRACE_ENABLE, nOsmTraceValue);
      }
      else
      {
        return FALSE;
      }
      break;

    case HAL_CLK_OSM_CLKDOM0_PACKET_SEL:
      if (nOsmTraceValue <= 7)
      {
        HWIO_OUTF(APSS_OSM_TOP_CLKDOM_TRACE_CTRL, CLKDOM0_TRACE_PACKET_SEL, nOsmTraceValue);
      }
      else
      {
        return FALSE;
      }
      break;

    case HAL_CLK_OSM_CLKDOM1_PACKET_SEL:
      if (nOsmTraceValue <= 7)
      {
        HWIO_OUTF(APSS_OSM_TOP_CLKDOM_TRACE_CTRL, CLKDOM1_TRACE_PACKET_SEL, nOsmTraceValue);
      }
      else
      {
        return FALSE;
      }
      break;

    case HAL_CLK_OSM_CLKDOM2_PACKET_SEL:
      if (nOsmTraceValue <= 7)
      {
        HWIO_OUTF(APSS_OSM_TOP_CLKDOM_TRACE_CTRL, CLKDOM2_TRACE_PACKET_SEL, nOsmTraceValue);
      }
      else
      {
        return FALSE;
      }
      break;

    case HAL_CLK_OSM_CLKDOM3_PACKET_SEL:
      if (nOsmTraceValue <= 7)
      {
        HWIO_OUTF(APSS_OSM_TOP_CLKDOM_TRACE_CTRL, CLKDOM3_TRACE_PACKET_SEL, nOsmTraceValue);
      }
      else
      {
        return FALSE;
      }
      break;

    case HAL_CLK_OSM_CLKDOM4_PACKET_SEL:
      if (nOsmTraceValue <= 7)
      {
        HWIO_OUTF(APSS_OSM_TOP_CLKDOM_TRACE_CTRL, CLKDOM4_TRACE_PACKET_SEL, nOsmTraceValue);
      }
      else
      {
        return FALSE;
      }
      break;

    case HAL_CLK_OSM_CLKDOM5_PACKET_SEL:
      if (nOsmTraceValue <= 7)
      {
        HWIO_OUTF(APSS_OSM_TOP_CLKDOM_TRACE_CTRL, CLKDOM5_TRACE_PACKET_SEL, nOsmTraceValue);
      }
      else
      {
        return FALSE;
      }
      break;

    case HAL_CLK_OSM_CLKDOM6_PACKET_SEL:
      if (nOsmTraceValue <= 7)
      {
        HWIO_OUTF(APSS_OSM_TOP_CLKDOM_TRACE_CTRL, CLKDOM6_TRACE_PACKET_SEL, nOsmTraceValue);
      }
      else
      {
        return FALSE;
      }
      break;

    case HAL_CLK_OSM_CLKDOM7_PACKET_SEL:
      if (nOsmTraceValue <= 7)
      {
        HWIO_OUTF(APSS_OSM_TOP_CLKDOM_TRACE_CTRL, CLKDOM7_TRACE_PACKET_SEL, nOsmTraceValue);
      }
      else
      {
        return FALSE;
      }
      break;

    case HAL_CLK_OSM_CLKDOM8_PACKET_SEL:
      if (nOsmTraceValue <= 7)
      {
        HWIO_OUTF(APSS_OSM_TOP_CLKDOM_TRACE_CTRL, CLKDOM8_TRACE_PACKET_SEL, nOsmTraceValue);
      }
      else
      {
        return FALSE;
      }
      break;

    case HAL_CLK_OSM_CLKDOM9_PACKET_SEL:
      if (nOsmTraceValue <= 7)
      {
        HWIO_OUTF(APSS_OSM_TOP_CLKDOM_TRACE_CTRL, CLKDOM9_TRACE_PACKET_SEL, nOsmTraceValue);
      }
      else
      {
        return FALSE;
      }
      break;

    case HAL_CLK_OSM_PERIODIC_TRACE_ENABLE:
      if (nOsmTraceValue <= 1)
      {
        HWIO_OUTF(APSS_OSM_TOP_GLOBAL_TRACE_CTRL, PERIODIC_TRACE_ENABLE, nOsmTraceValue);
      }
      else
      {
        return FALSE;
      }
      break;

    default:
      return FALSE;
  }
#endif
  return TRUE;

} /* END HAL_clk_OSM_ConfigureTraceControl */


/* ===========================================================================
**  Function : HAL_clk_OSM_ConfigureTraceTimer
**
** ======================================================================== */

boolean HAL_clk_OSM_ConfigureTraceTimer
(
  uint32 nTraceTimerValue
)
{
// TODO: need access control change to all the registers below
#if 0
  /*
   * Write the register.
   */
  HWIO_OUTF(APSS_OSM_TOP_PERIODIC_TRACE_TIMER_CTRL, PERIODIC_TRACE_TIMER_VAL, nTraceTimerValue);
#endif
  return TRUE;

} /* END HAL_clk_OSM_ConfigureTraceTimer */


/* ===========================================================================
**  Function : HAL_clk_OSM_ConfigurePerformanceBoost
**
** ======================================================================== */

boolean HAL_clk_OSM_ConfigurePerformanceBoost
(
  HAL_clk_APCSCPUType eCore,
  boolean             bPerformanceBoost
)
{
  /* Sanity check */
  if (eCore >= HAL_CLK_APCS_CPU_TOTAL ||
      bPerformanceBoost > 1           )
  {
    return FALSE;
  }

  /* Program the hardware. */
  switch(eCore)
  {
    case HAL_CLK_APCS_CPU_CLUSTER0_C0:
    case HAL_CLK_APCS_CPU_CLUSTER0_C1:
    case HAL_CLK_APCS_CPU_CLUSTER0_C2:
    case HAL_CLK_APCS_CPU_CLUSTER0_C3:
      /* Program the hardware. */
      HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM1_PDN_FSM_CTRL, CC_BOOST_FSM_EN, bPerformanceBoost);
      break;

    case HAL_CLK_APCS_CPU_CLUSTER1_C0:
    case HAL_CLK_APCS_CPU_CLUSTER1_C1:
    case HAL_CLK_APCS_CPU_CLUSTER1_C2:
    case HAL_CLK_APCS_CPU_CLUSTER1_C3:
      /* Program the hardware. */
      HWIO_OUTF(APSS_OSM_RAIL1_CLKDOM0_PDN_FSM_CTRL, CC_BOOST_FSM_EN, bPerformanceBoost);
      break;

    case HAL_CLK_APCS_CPU_L3:
      /* Program the hardware. */
      HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM0_PDN_FSM_CTRL, CC_BOOST_FSM_EN, bPerformanceBoost);
      break;

    default:
      return FALSE;
  }

  return TRUE;

} /* END HAL_clk_OSM_ConfigurePerformanceBoost */


/* ===========================================================================
**  Function : HAL_clk_OSM_ConfigureVminReduction
**
** ======================================================================== */

boolean HAL_clk_OSM_ConfigureVminReduction
(
  HAL_clk_APCSCPUType eCore,
  boolean             bVminReduction,
  uint32              nMaxCprVc,
  uint32              nMinCprVc
)
{
  /* Sanity check */
  if (eCore >= HAL_CLK_APCS_CPU_TOTAL ||
      bVminReduction > 1              ||
      nMaxCprVc > 63                  ||
      nMinCprVc > 63                  )
  {
    return FALSE;
  }

  /* Program the hardware. */
  switch(eCore)
  {
    case HAL_CLK_APCS_CPU_CLUSTER0_C0:
    case HAL_CLK_APCS_CPU_CLUSTER0_C1:
    case HAL_CLK_APCS_CPU_CLUSTER0_C2:
    case HAL_CLK_APCS_CPU_CLUSTER0_C3:
      /* Program the hardware. */
      HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM1_CPR_CORE_COUNT_BASED_VMIN_REDUCTION_ENABLE, CORE_COUNT_BASED_VMIN_REDUCTION_ENABLE, bVminReduction);
      HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM1_CPR_CORE_COUNT_BASED_VMIN_REDUCTION_ENABLE, MAX_CPR_VC,                             nMaxCprVc);
      HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM1_CPR_CORE_COUNT_BASED_VMIN_REDUCTION_ENABLE, MIN_CPR_VC,                             nMinCprVc);
      break;

    case HAL_CLK_APCS_CPU_CLUSTER1_C0:
    case HAL_CLK_APCS_CPU_CLUSTER1_C1:
    case HAL_CLK_APCS_CPU_CLUSTER1_C2:
    case HAL_CLK_APCS_CPU_CLUSTER1_C3:
      /* Program the hardware. */
      HWIO_OUTF(APSS_OSM_RAIL1_CLKDOM0_CPR_CORE_COUNT_BASED_VMIN_REDUCTION_ENABLE, CORE_COUNT_BASED_VMIN_REDUCTION_ENABLE, bVminReduction);
      HWIO_OUTF(APSS_OSM_RAIL1_CLKDOM0_CPR_CORE_COUNT_BASED_VMIN_REDUCTION_ENABLE, MAX_CPR_VC,                             nMaxCprVc);
      HWIO_OUTF(APSS_OSM_RAIL1_CLKDOM0_CPR_CORE_COUNT_BASED_VMIN_REDUCTION_ENABLE, MIN_CPR_VC,                             nMinCprVc);
      break;

    case HAL_CLK_APCS_CPU_L3:
      /* Program the hardware. */
      HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM0_CPR_CORE_COUNT_BASED_VMIN_REDUCTION_ENABLE, CORE_COUNT_BASED_VMIN_REDUCTION_ENABLE, bVminReduction);
      HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM0_CPR_CORE_COUNT_BASED_VMIN_REDUCTION_ENABLE, MAX_CPR_VC,                             nMaxCprVc);
      HWIO_OUTF(APSS_OSM_RAIL0_CLKDOM0_CPR_CORE_COUNT_BASED_VMIN_REDUCTION_ENABLE, MIN_CPR_VC,                             nMinCprVc);
      break;

    default:
      return FALSE;
  }

  return TRUE;

} /* END HAL_clk_OSM_ConfigureVminReduction */

