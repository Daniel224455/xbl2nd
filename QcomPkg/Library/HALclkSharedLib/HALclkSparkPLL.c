/*
==============================================================================

FILE:         HALclkSparkPLL.c

DESCRIPTION:
  This file contains the clock HAL code for the Spark PLL control.
  These PLLs are of the Spark PLL variety.

==============================================================================

                             Edit History


when       who     what, where, why
--------   ---     -----------------------------------------------------------
10/27/14   vt      Created

==============================================================================
        Copyright (c) 2018-2019 Qualcomm Technologies Incorporated.
                    All Rights Reserved.
                  QUALCOMM Proprietary/GTDR
==============================================================================
*/

/*============================================================================

                     INCLUDE FILES FOR MODULE

============================================================================*/

#include "HALhwio.h"
#include "HALclkSparkPLL.h"
#include "HALclkGenericPLL.h"


/*============================================================================

             DEFINITIONS AND DECLARATIONS FOR MODULE

=============================================================================*/

/*
 * Source control functions.  These can be used if some of the control
 * needs to be overridden by a specific PLL.
 */
static boolean HAL_clk_SparkPLLEnable           ( HAL_clk_SourceDescType *pSource, HAL_clk_SourceDisableModeType eMode, void *pData );
static void    HAL_clk_SparkPLLDisable          ( HAL_clk_SourceDescType *pSource, HAL_clk_SourceDisableModeType eMode, void *pData );
static boolean HAL_clk_SparkPLLIsEnabled        ( HAL_clk_SourceDescType *pSource );
static void    HAL_clk_SparkPLLConfig           ( HAL_clk_SourceDescType *pSource, HAL_clk_SourceConfigType eConfig );
static boolean HAL_clk_SparkPLLConfigPLL        ( HAL_clk_SourceDescType *pSource, const HAL_clk_PLLConfigType *pmConfig, HAL_clk_SourceConfigMode eMode);
static void    HAL_clk_SparkPLLDetectPLLConfig  ( HAL_clk_SourceDescType *pSource, HAL_clk_PLLConfigType *pmConfig );
static boolean HAL_clk_SparkPLLEnableVote       ( HAL_clk_SourceDescType *pSource );
static void    HAL_clk_SparkPLLDisableVote      ( HAL_clk_SourceDescType *pSource );
static boolean HAL_clk_SparkPLLIsVoteEnabled    ( HAL_clk_SourceDescType *pSource );
static boolean HAL_clk_SparkPLLIsPLLConfigEqual ( const HAL_clk_PLLConfigType *pConfigA, const HAL_clk_PLLConfigType *pConfigB);
static boolean HAL_clk_SparkPLLIsCalibrated     ( HAL_clk_SourceDescType *pSource );
static boolean HAL_clk_SparkPLLSetCalConfig     ( HAL_clk_SourceDescType *pSource, const HAL_clk_PLLConfigType *pmConfig );
static void    HAL_clk_SparkPLLSetRegSettings   ( HAL_clk_SourceDescType *pSource, const HAL_clk_SourceRegSettingsType *pmSettings );


/* ============================================================================
**    Definitions
** ==========================================================================*/


/*
 * HAL_CLK_PLL_MODE_*
 *
 * Definitions for configuring the PLLn_MODE register.
 */
#define HAL_CLK_PLL_MODE_ACTIVE  \
  (HWIO_FMSK(PLL_MODE, PLL_OUTCTRL)  | \
   HWIO_FMSK(PLL_MODE, PLL_BYPASSNL) | \
   HWIO_FMSK(PLL_MODE, PLL_RESET_N))

/*
 * Definitions for configuring the PLL in FSM Mode
 */
#define HAL_CLK_PLL_BIAS_COUNT_VAL   (0x6 << HWIO_SHFT(PLL_MODE, PLL_BIAS_COUNT))
#define HAL_CLK_PLL_LOCK_COUNT_VAL   (0x0 << HWIO_SHFT(PLL_MODE, PLL_LOCK_COUNT))

/*
 * HAL_CLK_PLL_INIT_TIME_US
 *
 * Init time in microseconds after de-asserting PLL reset (normal mode).
 */
#define HAL_CLK_PLL_INIT_TIME_US 5


/*
 * HAL_CLK_LOCK_TIMEOUT_US
 *
 * Timeout in US for PLL_LOCK_DET to get set.
 */
#define HAL_CLK_LOCK_TIMEOUT_US 200

/*
 * HAL_CLK_UPDATE_TIMEOUT_US
 *
 * Timeout in US for PLL_UPDATE to get set.
 */
#define HAL_CLK_UPDATE_TIMEOUT_US 200

/*
 * HAL_CLK_PLL_OUTCTRL_OFF_US
 *
 * Wait after PLL_OUTCTRL off before setting freeze bit.
 */
#define HAL_CLK_PLL_OUTCTRL_OFF_US 5

/*
 * GPLL1 register interface is different than the other PLL's.
 * These are the registers/fields which are affected on non-RPM masters.
 */
#define HWIO_GCC_GPLL1_STATUS_PLL_ACTIVE_FLAG_BMSK 0x20000

/*
 * Spark PLL VCO modes
 */
#define SPARK_PLL_VCO0   0
#define SPARK_PLL_VCO1   1
#define SPARK_PLL_VCO2   2
#define SPARK_PLL_VCO3   3
#define SPARK_PLL_VCO2_1 4

/* ============================================================================
**    Data Declarations
** ==========================================================================*/

/*
 * HAL_clk_SparkPLLControl
 *
 * Control function array for the Spark PLLs.
 */
HAL_clk_SourceControlType HAL_clk_SparkPLLControl =
{
  .Enable          =  HAL_clk_SparkPLLEnable,
  .Disable         =  HAL_clk_SparkPLLDisable,
  .IsEnabled       =  HAL_clk_SparkPLLIsEnabled,
  .Config          =  HAL_clk_SparkPLLConfig,
  .ConfigPLL       =  HAL_clk_SparkPLLConfigPLL,
  .DetectPLLConfig =  HAL_clk_SparkPLLDetectPLLConfig,
  .EnableVote      =  HAL_clk_SparkPLLEnableVote,
  .DisableVote     =  HAL_clk_SparkPLLDisableVote,
  .IsVoteEnabled   =  HAL_clk_SparkPLLIsVoteEnabled,
  .IsPLLConfigEqual = HAL_clk_SparkPLLIsPLLConfigEqual,
  .IsCalibrated     = HAL_clk_SparkPLLIsCalibrated,
  .SetCalConfig     = HAL_clk_SparkPLLSetCalConfig,
  .SetRegSettings   = HAL_clk_SparkPLLSetRegSettings,
};


/*============================================================================

               FUNCTION DEFINITIONS FOR MODULE

============================================================================*/



/* ===========================================================================
**  HAL_clk_SparkPLLEnableVote
**
** ======================================================================== */
static boolean HAL_clk_SparkPLLEnableVote
(
  HAL_clk_SourceDescType *pSource
)
{
  uint32                  nVal;
  boolean                 bResult;

  /*
   * Set the appropriate bit.
   */
  nVal = inpdw(pSource->VoterRegister.nAddr) | pSource->VoterRegister.nMask;
  outpdw(pSource->VoterRegister.nAddr, nVal);

  /*
   * Wait for the PLL to go active during the 15 us timeout period.
   */
  bResult =
    HAL_clk_ValidateRegSetTimeout(
      HAL_CLK_PLL_HWIO_ADDR(PLL_MODE, pSource),
      HWIO_FMSK(PLL_MODE, PLL_ACTIVE_FLAG),
      HAL_CLK_LOCK_TIMEOUT_US);
  
  if (!bResult)
  {
    ERR_FATAL("HAL_clk_SparkPLLEnableVote Activate Failure %x\t%x", pSource->VoterRegister.nAddr, pSource->VoterRegister.nMask, 0);
  }

  return bResult;

} /* END HAL_clk_SparkPLLEnableVote */


/* ===========================================================================
**  HAL_clk_SparkPLLDisableVote
**
** ======================================================================== */

static void HAL_clk_SparkPLLDisableVote
(
  HAL_clk_SourceDescType *pSource
)
{
  uint32 nVal;

  /*
   * Clear the appropriate bit.
   */
  nVal = inpdw(pSource->VoterRegister.nAddr) & ~pSource->VoterRegister.nMask;
  outpdw(pSource->VoterRegister.nAddr, nVal);

} /* END HAL_clk_SparkPLLDisableVote */


/* ===========================================================================
**  HAL_clk_SparkPLLIsVoteEnabled
**
** ======================================================================== */

static boolean HAL_clk_SparkPLLIsVoteEnabled
(
  HAL_clk_SourceDescType *pSource
)
{
  /*
   * Check the appropriate bit.
   */
  if (inpdw(pSource->VoterRegister.nAddr) & pSource->VoterRegister.nMask)
  {
    return TRUE;
  }

  return FALSE;

} /* END HAL_clk_SparkPLLIsVoteEnabled */


/* ===========================================================================
**  HAL_clk_SparkPLLEnableFromStandby
**
**  PLL bringup from standby state.
**  When in the standby state, the bias is on and draws 75-100 micro amps.
**  This procedure skips calibration.
**
**  This procedure applies only to Spark PLL type.
**
**  The following fields must have remained unchanged from the values present
**  when the HAL_clk_SparkPLLDisableToStandby function was executed:
**  - PLL_L_VAL
**  - PLL_ALPHA_VAL
**  - PLL_ALPHA_VAL_U
**  - PLL_USER_CTL[ALPHA_EN]
**  - PLL_USER_CTL[VCO_SEL]
**
**  Returns:
**    boolean - TRUE if successfully enabled. FALSE otherwise.
** ======================================================================== */

static boolean HAL_clk_SparkPLLEnableFromStandby
(
  HAL_clk_SourceDescType *pSource
)
{
  uint32                  nModeVal, nUserCtlUVal;
  boolean                 bResult;

  /*
   * Read current user ctl value.
   */
  nUserCtlUVal = HAL_CLK_PLL_HWIO_IN(PLL_USER_CTL_U, pSource);

  /*
   * Assert the target ctl field.
   */
  nUserCtlUVal |= HWIO_FMSK(PLL_USER_CTL_U, TARGET_CTL);
  HAL_CLK_PLL_HWIO_OUT(PLL_USER_CTL_U, pSource, nUserCtlUVal);

  /*
   * Read current mode value.
   */
  nModeVal = HAL_CLK_PLL_HWIO_IN(PLL_MODE, pSource);

  /*
   * Assert the reset field.
   */
  nModeVal |= HWIO_FMSK(PLL_MODE, PLL_RESET_N);
  HAL_CLK_PLL_HWIO_OUT(PLL_MODE, pSource, nModeVal);

  /*
   * Wait for the PLL to lock during the 15 us timeout period.
   */
  bResult =
    HAL_clk_ValidateRegSetTimeout(
      HAL_CLK_PLL_HWIO_ADDR(PLL_MODE, pSource),
      HWIO_FMSK(PLL_MODE, PLL_LOCK_DET),
      HAL_CLK_LOCK_TIMEOUT_US);

  /*
   * Return immediately if the lock detect failed.
   */
  if (!bResult)
  {
    ERR_FATAL("HAL_clk_SparkPLLEnableFromStandby Lock Failure", 0, 0, 0);
    return FALSE;
  }

  /*
   * Assert the output control field.
   */
  nModeVal |= HWIO_FMSK(PLL_MODE, PLL_OUTCTRL);
  HAL_CLK_PLL_HWIO_OUT(PLL_MODE, pSource, nModeVal);

  /*
   * De-assert the target ctl field.
   */
  nUserCtlUVal &= ~HWIO_FMSK(PLL_USER_CTL_U, TARGET_CTL);
  HAL_CLK_PLL_HWIO_OUT(PLL_USER_CTL_U, pSource, nUserCtlUVal);

  return TRUE;

} /* END HAL_clk_SparkPLLEnableFromStandby */


/* ===========================================================================
**  HAL_clk_SparkPLLEnableFromSaved
**
**  PLL bringup with restore state.
**  When in the saved state, the bias is off and draws no power.
**  This procedure skips calibration.
**
**  This procedure applies only to Spark PLL type.
**
**  The following fields must have remained unchanged from the values present
**  when the HAL_clk_SparkPLLDisableToSaved function was executed:
**  - PLL_L_VAL
**  - PLL_ALPHA_VAL
**  - PLL_ALPHA_VAL_U
**  - PLL_USER_CTL[ALPHA_EN]
**  - PLL_USER_CTL[VCO_SEL]
**
**  Returns:
**    boolean - TRUE if successfully enabled. FALSE otherwise.
** ======================================================================== */

static boolean HAL_clk_SparkPLLEnableFromSaved
(
  HAL_clk_SourceDescType *pSource,
  void                   *pData
)
{
  uint32                  nModeVal, nUserCtlUVal;
  boolean                 bResult;
  uint32                 *pnCalibration = (uint32 *) pData;

  /*
   * Validate that we have a place to store the calibration.
   */
  if (pnCalibration == NULL)
  {
    ERR_FATAL("HAL_clk_SparkPLLEnableFromSaved Invalid Parameter", 0, 0, 0);
    return FALSE;
  }

  /*
   * Read current mode value.
   */
  nModeVal = HAL_CLK_PLL_HWIO_IN(PLL_MODE, pSource);

  /*
   * Assert the bypassnl field to power up the bias.
   */
  nModeVal |= HWIO_FMSK(PLL_MODE, PLL_BYPASSNL);
  HAL_CLK_PLL_HWIO_OUT(PLL_MODE, pSource, nModeVal);

  /*
   * Wait 5 us for the bias to power up - enter standby state.
   */
  HAL_clk_BusyWait(5);

  /*
   * Restore the saved state.
   */
  HAL_CLK_PLL_HWIO_OUT(PLL_FREQ_CTL, pSource, *pnCalibration);

  /*
   * Read current user ctl value.
   */
  nUserCtlUVal = HAL_CLK_PLL_HWIO_IN(PLL_USER_CTL_U, pSource);

  /*
   * Assert the target ctl and write state fields.
   */
  nUserCtlUVal |= HWIO_FMSK(PLL_USER_CTL_U, TARGET_CTL);
  nUserCtlUVal |= HWIO_FMSK(PLL_USER_CTL_U, WRITE_STATE);
  HAL_CLK_PLL_HWIO_OUT(PLL_USER_CTL_U, pSource, nUserCtlUVal);

  /*
   * Assert the update field.
   */
  nModeVal |= HWIO_FMSK(PLL_MODE, PLL_UPDATE);
  HAL_CLK_PLL_HWIO_OUT(PLL_MODE, pSource, nModeVal);

  /*
   * Wait for PLL_UPDATE to clear during the 15 us timeout period.
   */
  bResult =
    HAL_clk_ValidateRegClearTimeout(
      HAL_CLK_PLL_HWIO_ADDR(PLL_MODE, pSource),
      HWIO_FMSK(PLL_MODE, PLL_UPDATE),
      HAL_CLK_UPDATE_TIMEOUT_US);

  /*
   * Return immediately if the update failed.
   */
  if (!bResult)
  {
    ERR_FATAL("HAL_clk_SparkPLLEnableFromSaved Update Failure", 0, 0, 0);
    return FALSE;
  }

  /*
   * De-assert the write state field.
   */
  nUserCtlUVal &= ~HWIO_FMSK(PLL_USER_CTL_U, WRITE_STATE);
  HAL_CLK_PLL_HWIO_OUT(PLL_USER_CTL_U, pSource, nUserCtlUVal);

  /*
   * Assert the reset field.
   */
  nModeVal |= HWIO_FMSK(PLL_MODE, PLL_RESET_N);
  HAL_CLK_PLL_HWIO_OUT(PLL_MODE, pSource, nModeVal);

  /*
   * Wait for the PLL to lock during the 15 us timeout period.
   */
  bResult =
    HAL_clk_ValidateRegSetTimeout(
      HAL_CLK_PLL_HWIO_ADDR(PLL_MODE, pSource),
      HWIO_FMSK(PLL_MODE, PLL_LOCK_DET),
      HAL_CLK_LOCK_TIMEOUT_US);

  /*
   * Return immediately if the lock failed.
   */
  if (!bResult)
  {
    ERR_FATAL("HAL_clk_SparkPLLEnableFromSaved Lock Failure", 0, 0, 0);
    return FALSE;
  }

  /*
   * Assert the outctrl field.
   */
  nModeVal |= HWIO_FMSK(PLL_MODE, PLL_OUTCTRL);
  HAL_CLK_PLL_HWIO_OUT(PLL_MODE, pSource, nModeVal);

  /*
   * De-assert the target ctl field.
   */
  nUserCtlUVal &= ~HWIO_FMSK(PLL_USER_CTL_U, TARGET_CTL);
  HAL_CLK_PLL_HWIO_OUT(PLL_USER_CTL_U, pSource, nUserCtlUVal);

  return TRUE;

} /* END HAL_clk_SparkPLLEnableFromSaved */


/* ===========================================================================
**  HAL_clk_SparkPLLEnableNormal
**
**  PLL initial bringup.
**  When in the off state, the bias is off and draws no power.
**  This procedure results in calibration being performed.
**
**  This procedure applies only to Spark PLL type.
**
**  Returns:
**    boolean - TRUE if successfully enabled. FALSE otherwise.
** ======================================================================== */

static boolean HAL_clk_SparkPLLEnableNormal
(
  HAL_clk_SourceDescType *pSource
)
{
  uint32                  nModeVal, nUserCtlUVal;
  boolean                 bResult;

  /*
   * Read current user control value.
   */
  nUserCtlUVal = HAL_CLK_PLL_HWIO_IN(PLL_USER_CTL_U, pSource);

  /*
   * Set the lockdet field.
   */
  nUserCtlUVal |= HWIO_FMSK(PLL_USER_CTL_U, LOCK_DET);
  HAL_CLK_PLL_HWIO_OUT(PLL_USER_CTL_U, pSource, nUserCtlUVal);

  /*
   * Read current mode value.
   */
  nModeVal = HAL_CLK_PLL_HWIO_IN(PLL_MODE, pSource);

  /*
   * De-assert PLL bypass.
   * This latches L, Alpha, and AlphaU values.
   */
  nModeVal |= HWIO_FMSK(PLL_MODE, PLL_BYPASSNL);
  HAL_CLK_PLL_HWIO_OUT(PLL_MODE, pSource, nModeVal);

  /*
   * Wait for PLL init time
   */
  HAL_clk_BusyWait(HAL_CLK_PLL_INIT_TIME_US);

  /*
   * De-assert PLL reset.
   */
  nModeVal |= HWIO_FMSK(PLL_MODE, PLL_RESET_N);
  HAL_CLK_PLL_HWIO_OUT(PLL_MODE, pSource, nModeVal);

  /*
   * Wait for the PLL to lock during the 15 us timeout period.
   */
  bResult =
    HAL_clk_ValidateRegSetTimeout(
      HAL_CLK_PLL_HWIO_ADDR(PLL_MODE, pSource),
      HWIO_FMSK(PLL_MODE, PLL_LOCK_DET),
      HAL_CLK_LOCK_TIMEOUT_US);

  /*
   * Return immediately if the lock failed.
   */
  if (!bResult)
  {
    ERR_FATAL("HAL_clk_SparkPLLEnableNormal Lock Failure", 0, 0, 0);
    return FALSE;
  }

  /*
   * Put the PLL in active mode
   */
  nModeVal |= HWIO_FMSK(PLL_MODE, PLL_OUTCTRL);
  HAL_CLK_PLL_HWIO_OUT(PLL_MODE, pSource, nModeVal);

  return TRUE;

} /* END HAL_clk_SparkPLLEnableNormal */


/* ===========================================================================
**  HAL_clk_SparkPLLEnableFromFreeze
**
**  PLL bringup from 'freeze' state.
**  This procedure skips calibration.
**
**  This procedure applies only to Spark PLL type.
**
**  The following bit must be set per HPG to prevent calibration loss
**  when the HAL_clk_SparkPLLEnableFromFreeze function was executed:
**  - PLL_TEST_CTL_U[CAL_CODE_UPDATE]
**
**  Returns:
**    boolean - TRUE if successfully enabled. FALSE otherwise.
** ======================================================================== */

static boolean HAL_clk_SparkPLLEnableFromFreeze
(
  HAL_clk_SourceDescType *pSource,
  void                   *pData
)
{
  uint32                  nModeVal, nUserCtlUVal;
  boolean                 bResult;
  uint32                 *pnCalibration = (uint32 *) pData;

  /*
   * Read current mode value.
   */
  nModeVal = HAL_CLK_PLL_HWIO_IN(PLL_MODE, pSource);

  /*
   * Read current user ctl value.
   */
  nUserCtlUVal = HAL_CLK_PLL_HWIO_IN(PLL_USER_CTL_U, pSource);


  if (pnCalibration)
  {
    /*
     * Restore the saved state.
     */
    HAL_CLK_PLL_HWIO_OUT(PLL_FREQ_CTL, pSource, *pnCalibration);

    /*
     * Assert the target ctl and write state fields.
     */
    nUserCtlUVal |= HWIO_FMSK(PLL_USER_CTL_U, TARGET_CTL);
    nUserCtlUVal |= HWIO_FMSK(PLL_USER_CTL_U, WRITE_STATE);
    HAL_CLK_PLL_HWIO_OUT(PLL_USER_CTL_U, pSource, nUserCtlUVal);

    /*
     * Assert the update field.
     */
    nModeVal |= HWIO_FMSK(PLL_MODE, PLL_UPDATE);
    HAL_CLK_PLL_HWIO_OUT(PLL_MODE, pSource, nModeVal);

    /*
     * Wait for PLL_UPDATE to clear during the 15 us timeout period.
     */
    bResult =
      HAL_clk_ValidateRegClearTimeout(
        HAL_CLK_PLL_HWIO_ADDR(PLL_MODE, pSource),
        HWIO_FMSK(PLL_MODE, PLL_UPDATE),
        HAL_CLK_UPDATE_TIMEOUT_US);

    /*
     * Return immediately if the update failed.
     */
    if (!bResult)
    {
      ERR_FATAL("HAL_clk_SparkPLLEnableFromFreeze Update Failure", 0, 0, 0);
      return FALSE;
    }
  }

  HAL_CLK_PLL_HWIO_OUTF(PLL_TEST_CTL_U, pSource, CAL_CODE_UPDATE, 0x1);
  nUserCtlUVal &= ~HWIO_FMSK(PLL_USER_CTL_U, FREEZE_PLL);
  HAL_CLK_PLL_HWIO_OUT(PLL_USER_CTL_U, pSource, nUserCtlUVal);

  bResult =
    HAL_clk_ValidateRegSetTimeout(
      HAL_CLK_PLL_HWIO_ADDR(PLL_MODE, pSource),
      HWIO_FMSK(PLL_MODE, PLL_LOCK_DET),
      HAL_CLK_LOCK_TIMEOUT_US);

  /*
   * Return immediately if the lock failed.
   */
  if (!bResult)
  {
    ERR_FATAL("HAL_clk_SparkPLLEnableFromFreeze Lock Failure", 0, 0, 0);
    return FALSE;
  }

  HAL_CLK_PLL_HWIO_OUTF(PLL_TEST_CTL_U, pSource, CAL_CODE_UPDATE, 0x0);
  nModeVal |= HWIO_FVAL(PLL_MODE, PLL_OUTCTRL, 1);
  HAL_CLK_PLL_HWIO_OUT(PLL_MODE, pSource, nModeVal);

  return TRUE;

} /* END HAL_clk_SparkPLLEnableFromFreeze */


/* ===========================================================================
**  HAL_clk_SparkPLLEnable
**
**  The appropriate enable mode is executed based on the eMode parameter.
**
**  Returns:
**    boolean - TRUE if successfully enabled. FALSE otherwise.
** ======================================================================== */

static boolean HAL_clk_SparkPLLEnable
(
  HAL_clk_SourceDescType        *pSource,
  HAL_clk_SourceDisableModeType  eMode,
  void                          *pData
)
{
  boolean bResult;

  /*
   * Check if PLL is already enabled and return
   */
  if (HAL_clk_SparkPLLIsEnabled(pSource))
  {
    return TRUE;
  }

  switch (eMode)
  {
    case HAL_CLK_SOURCE_DISABLE_MODE_STANDBY:
      bResult = HAL_clk_SparkPLLEnableFromStandby(pSource);
      break;
    case HAL_CLK_SOURCE_DISABLE_MODE_SAVE:
      bResult = HAL_clk_SparkPLLEnableFromSaved(pSource, pData);
      break;
    case HAL_CLK_SOURCE_DISABLE_MODE_FREEZE:
      bResult = HAL_clk_SparkPLLEnableFromFreeze(pSource, pData);
      break;
    case HAL_CLK_SOURCE_DISABLE_MODE_NORMAL:
      bResult = HAL_clk_SparkPLLEnableNormal(pSource);
      break;
    default:
      bResult = FALSE;
      ERR_FATAL("HAL_clk_SparkPLLEnable Invalid Parameter", 0, 0, 0);
      break;
  }

  return bResult;

} /* END HAL_clk_SparkPLLEnable */


/* ===========================================================================
**  HAL_clk_SparkPLLDisableToStandby
**
**  PLL shutdown to standby state.
**  This procedure leaves the bias on:
**    skips calibration when enabling via HAL_clk_SparkPLLEnableFromStandby.
**    draws 75-100 micro amps while in standby mode.
**
**  This procedure applies only to Spark PLL type.
**
** ======================================================================== */

static void HAL_clk_SparkPLLDisableToStandby
(
  HAL_clk_SourceDescType *pSource
)
{
  uint32  nModeVal;

  /*
   * Read current mode value.
   */
  nModeVal = HAL_CLK_PLL_HWIO_IN(PLL_MODE, pSource);

  /*
   * De-assert the output control field.
   */
  nModeVal &= ~HWIO_FMSK(PLL_MODE, PLL_OUTCTRL);
  HAL_CLK_PLL_HWIO_OUT(PLL_MODE, pSource, nModeVal);

  /*
   * Wait 2 ticks of the output main clock.
   */
  HAL_clk_BusyWait(1);

  /*
   * De-assert the reset field.
   */
  nModeVal &= ~HWIO_FMSK(PLL_MODE, PLL_RESET_N);
  HAL_CLK_PLL_HWIO_OUT(PLL_MODE, pSource, nModeVal);

} /* END HAL_clk_SparkPLLDisableToStandby */


/* ===========================================================================
**  HAL_clk_SparkPLLDisableToSaved
**
**  PLL shutdown with save state.
**  When in the saved state, the bias is off and draws no power.
**  The calibration data is saved in *pData to be used later when
**  calling HAL_clk_SparkPLLEnableFromSaved.
**
**  This procedure applies only to Spark PLL type.
**
**  NOTE: the void *pData argument is expected to be pointing to a
**  uint32. Therefore it MUST be recasted to a (uint32 *) when using it.
**
** ======================================================================== */

static void HAL_clk_SparkPLLDisableToSaved
(
  HAL_clk_SourceDescType *pSource,
  void                   *pData
)
{
  uint32 nModeVal, nUserCtlUVal;
  uint32 *pnCalibration = (uint32 *) pData;

  /*
   * Validate that we have a place to store the calibration.
   */
  if (pnCalibration == NULL)
  {
    ERR_FATAL("HAL_clk_SparkPLLDisableToSaved Invalid Parameter", 0, 0, 0);
    return;
  }

  /*
   * Read current mode value.
   */
  nModeVal = HAL_CLK_PLL_HWIO_IN(PLL_MODE, pSource);

  /*
   * De-assert the output control field.
   */
  nModeVal &= ~HWIO_FMSK(PLL_MODE, PLL_OUTCTRL);
  HAL_CLK_PLL_HWIO_OUT(PLL_MODE, pSource, nModeVal);

  /*
   * Wait 2 ticks of the output main clock.
   */
  HAL_clk_BusyWait(1);

  /*
   * Read current user ctl value.
   */
  nUserCtlUVal = HAL_CLK_PLL_HWIO_IN(PLL_USER_CTL_U, pSource);

  /*
   * Clear the status register and set the freeze pll fields.
   */
  nUserCtlUVal &= ~HWIO_FMSK(PLL_USER_CTL_U, STATUS_REGISTER);
  nUserCtlUVal |= HWIO_FMSK(PLL_USER_CTL_U, FREEZE_PLL);
  HAL_CLK_PLL_HWIO_OUT(PLL_USER_CTL_U, pSource, nUserCtlUVal);

  /*
   * Read current status value.
   */
  *pnCalibration = HAL_CLK_PLL_HWIO_IN(PLL_STATUS, pSource);

  /*
   * Clear the bypassnl and reset fields.
   */
  nModeVal &= ~(HWIO_FMSK(PLL_MODE, PLL_RESET_N) |
                HWIO_FMSK(PLL_MODE, PLL_BYPASSNL));
  HAL_CLK_PLL_HWIO_OUT(PLL_MODE, pSource, nModeVal);

  /*
   * Clear the freeze pll field.
   */
  nUserCtlUVal &= ~HWIO_FMSK(PLL_USER_CTL_U, FREEZE_PLL);
  HAL_CLK_PLL_HWIO_OUT(PLL_USER_CTL_U, pSource, nUserCtlUVal);

} /* END HAL_clk_SparkPLLDisableToSaved */


/* ===========================================================================
**  HAL_clk_SparkPLLDisableNormal
**
**  PLL shutdown.
**  When in the off state, the bias is off and draws no power.
**  This procedure results in calibration being required upon the next enable.
**
**  This procedure applies only to Spark PLL type.
**
** ======================================================================== */

static void HAL_clk_SparkPLLDisableNormal
(
  HAL_clk_SourceDescType *pSource
)
{
  uint32  nModeVal;

  /*
   * Read current mode value.
   */
  nModeVal = HAL_CLK_PLL_HWIO_IN(PLL_MODE, pSource);

  /*
   * De-assert the OUTCTL field.
   */
  nModeVal &= ~HWIO_FMSK(PLL_MODE, PLL_OUTCTRL);
  HAL_CLK_PLL_HWIO_OUT(PLL_MODE, pSource, nModeVal);

  /*
   * Wait 2 ticks of the output main clock.
   */
  HAL_clk_BusyWait(1);

  /*
   * Put the PLL in reset mode
   */
  nModeVal &= ~(HWIO_FMSK(PLL_MODE, PLL_RESET_N) |
                HWIO_FMSK(PLL_MODE, PLL_BYPASSNL));
  HAL_CLK_PLL_HWIO_OUT(PLL_MODE, pSource, nModeVal);

} /* END HAL_clk_SparkPLLDisableNormal */


/* ===========================================================================
**  HAL_clk_SparkPLLDisableToFreeze
**
**  Put PLL in the 'freeze' state.
**
**  This procedure applies only to Spark PLL type.
**
** ======================================================================== */

static void HAL_clk_SparkPLLDisableToFreeze
(
  HAL_clk_SourceDescType *pSource,
  void                   *pData
)
{
  uint32 nUserCtlUVal;
  uint32 *pnCalibration = (uint32 *) pData;

  HAL_CLK_PLL_HWIO_OUTF(PLL_MODE, pSource, PLL_OUTCTRL, 0);
  HAL_clk_BusyWait(HAL_CLK_PLL_OUTCTRL_OFF_US);

  nUserCtlUVal = HAL_CLK_PLL_HWIO_IN(PLL_USER_CTL_U, pSource);
  nUserCtlUVal &= ~HWIO_FMSK(PLL_USER_CTL_U, STATUS_REGISTER);
  nUserCtlUVal |= HWIO_FMSK(PLL_USER_CTL_U, FREEZE_PLL);
  HAL_CLK_PLL_HWIO_OUT(PLL_USER_CTL_U, pSource, nUserCtlUVal);

  if (pnCalibration)
  {
    /*
     * Read current status value.
     */
    *pnCalibration = HAL_CLK_PLL_HWIO_IN(PLL_STATUS, pSource);
  }

} /* END HAL_clk_SparkPLLDisableToFreeze */


/* ===========================================================================
**  HAL_clk_SparkPLLDisable
**
**  The appropriate disable mode is executed based on the eMode argument.
**
** ======================================================================== */

static void HAL_clk_SparkPLLDisable
(
  HAL_clk_SourceDescType        *pSource,
  HAL_clk_SourceDisableModeType  eMode,
  void                          *pData
)
{

  switch (eMode)
  {
    case HAL_CLK_SOURCE_DISABLE_MODE_STANDBY:
      HAL_clk_SparkPLLDisableToStandby(pSource);
      break;
    case HAL_CLK_SOURCE_DISABLE_MODE_SAVE:
      HAL_clk_SparkPLLDisableToSaved(pSource, pData);
      break;
    case HAL_CLK_SOURCE_DISABLE_MODE_FREEZE:
      HAL_clk_SparkPLLDisableToFreeze(pSource, pData);
      break;
    case HAL_CLK_SOURCE_DISABLE_MODE_NORMAL:
      HAL_clk_SparkPLLDisableNormal(pSource);
      break;
    default:
      ERR_FATAL("HAL_clk_SparkPLLDisable Invalid Parameter", 0, 0, 0);
      break;
  }

} /* END HAL_clk_SparkPLLDisable */


/* ===========================================================================
**  HAL_clk_SparkPLLIsEnabled
**
** ======================================================================== */

static boolean HAL_clk_SparkPLLIsEnabled
(
  HAL_clk_SourceDescType *pSource
)
{
  uint32 nModeVal;

  /*
   * Read current mode value.
   */
  nModeVal = HAL_CLK_PLL_HWIO_IN(PLL_MODE, pSource);

  /*
   * Check whether the PLL is enabled.
   */
  return ((nModeVal & HAL_CLK_PLL_MODE_ACTIVE) == HAL_CLK_PLL_MODE_ACTIVE);

} /* END HAL_clk_SparkPLLIsEnabled */


/* ===========================================================================
**  HAL_clk_SparkPLLConfig
**
** ======================================================================== */

static void HAL_clk_SparkPLLConfig
(
  HAL_clk_SourceDescType   *pSource,
  HAL_clk_SourceConfigType  eConfig
)
{
  uint32 nVal;

  /*
   * Take action.
   */
  switch (eConfig)
  {
    case HAL_CLK_CONFIG_PLL_FSM_MODE_ENABLE:
      nVal = HAL_CLK_PLL_HWIO_IN(PLL_MODE, pSource);
      nVal &= ~HWIO_FMSK(PLL_MODE, PLL_BIAS_COUNT);
      nVal &= ~HWIO_FMSK(PLL_MODE, PLL_LOCK_COUNT);
      nVal |= HAL_CLK_PLL_BIAS_COUNT_VAL;
      nVal |= HAL_CLK_PLL_LOCK_COUNT_VAL;
      nVal |= HWIO_FMSK(PLL_MODE, PLL_VOTE_FSM_ENA);
      HAL_CLK_PLL_HWIO_OUT(PLL_MODE, pSource, nVal);
      break;

    case HAL_CLK_CONFIG_PLL_FSM_MODE_DISABLE:
      HAL_CLK_PLL_HWIO_OUTF(PLL_MODE, pSource, PLL_VOTE_FSM_ENA, 0);
      break;

    case HAL_CLK_CONFIG_PLL_AUX_OUTPUT_DISABLE:
      HAL_CLK_PLL_HWIO_OUTF(PLL_USER_CTL, pSource, PLLOUT_LV_AUX, 0);
      break;

    case HAL_CLK_CONFIG_PLL_AUX_OUTPUT_ENABLE:
      HAL_CLK_PLL_HWIO_OUTF(PLL_USER_CTL, pSource, PLLOUT_LV_AUX, 1);
      break;

    case HAL_CLK_CONFIG_PLL_AUX2_OUTPUT_DISABLE:
      HAL_CLK_PLL_HWIO_OUTF(PLL_USER_CTL, pSource, PLLOUT_LV_AUX2, 0);
      break;

    case HAL_CLK_CONFIG_PLL_AUX2_OUTPUT_ENABLE:
      HAL_CLK_PLL_HWIO_OUTF(PLL_USER_CTL, pSource, PLLOUT_LV_AUX2, 1);
      break;

    case HAL_CLK_CONFIG_PLL_EARLY_OUTPUT_ENABLE:
      HAL_CLK_PLL_HWIO_OUTF(PLL_USER_CTL, pSource, PLLOUT_LV_EARLY, 1);
      break;

    case HAL_CLK_CONFIG_PLL_EARLY_OUTPUT_DISABLE:
      HAL_CLK_PLL_HWIO_OUTF(PLL_USER_CTL, pSource, PLLOUT_LV_EARLY, 0);
      break;

    case HAL_CLK_CONFIG_PLL_MAIN_OUTPUT_ENABLE:
      HAL_CLK_PLL_HWIO_OUTF(PLL_USER_CTL, pSource, PLLOUT_LV_MAIN, 1);
      break;

    case HAL_CLK_CONFIG_PLL_MAIN_OUTPUT_DISABLE:
      HAL_CLK_PLL_HWIO_OUTF(PLL_USER_CTL, pSource, PLLOUT_LV_MAIN, 0);
      break;
    default:
      ERR_FATAL("HAL_clk_SparkPLLConfig Invalid Parameter", 0, 0, 0);
      break;
  }

} /* END HAL_clk_SparkPLLConfig */


/* ===========================================================================
**  HAL_clk_SparkPLLConfigPLLSlew
**
**  Returns:
**    boolean - TRUE if successfully configured.
**              FALSE if unable to detect that the PLL successfully locked.
** ======================================================================== */

static boolean HAL_clk_SparkPLLConfigPLLSlew
(
  HAL_clk_SourceDescType      *pSource,
  const HAL_clk_PLLConfigType *pConfig,
  boolean                      bTriggerUpdate
)
{
  uint32                  nModeVal;
  boolean                 bResult;
  uint32                  nMode;

  /*
   * PLL can’t slew across VCO modes
   */
  nMode  = HAL_CLK_PLL_HWIO_INF(PLL_USER_CTL, pSource, VCO_SEL);
  /* 
   * Check the VCO mode is set to 2 or 2.1 
   */
  if ((nMode == SPARK_PLL_VCO2) && HAL_CLK_PLL_HWIO_INF(PLL_USER_CTL, pSource, VCO_BOOST))
  {
	  nMode = SPARK_PLL_VCO2_1;
  }

  if (nMode != pConfig->nMode)
  {
    ERR_FATAL("HAL_clk_SparkPLLConfigPLLSlew VCO mode mismatch, can't slew across VCO modes", 0, 0, 0);
    return FALSE;  
  }

  /*
   * Program L value.
   */
  HAL_CLK_PLL_HWIO_OUT(PLL_L_VAL, pSource, pConfig->nL);

  /*
   * Program Alpha/AlphaU values.
   */
  HAL_CLK_PLL_HWIO_OUT(PLL_ALPHA_VAL, pSource, pConfig->nFracVal);
  HAL_CLK_PLL_HWIO_OUT(PLL_ALPHA_U_VAL, pSource, (pConfig->nFracVal >> 32));

  /*
   * [WARNING : UNTESTED FUNCTIONALITY]
   * Do not update the ALPHA_EN field during slewing.
   */

  /*
   * Check if the update will be triggered later.
   */
  if (bTriggerUpdate != TRUE)
  {
    return TRUE;
  }

  /*
   * Read the PLLx_Mode value
   */
  nModeVal = HAL_CLK_PLL_HWIO_IN(PLL_MODE, pSource);

  /*
   * Toggle the update bit to latch the new configuration values.
   */
  nModeVal |= HWIO_FMSK(PLL_MODE, PLL_UPDATE);
  HAL_CLK_PLL_HWIO_OUT(PLL_MODE, pSource, nModeVal);

  /*
   * Wait for PLL_UPDATE to be cleared during the 15 us timeout period.
   */
  bResult =
    HAL_clk_ValidateRegClearTimeout(
      HAL_CLK_PLL_HWIO_ADDR(PLL_MODE, pSource),
      HWIO_FMSK(PLL_MODE, PLL_UPDATE),
      HAL_CLK_UPDATE_TIMEOUT_US);

  /*
   * Return immediately if the update failed.
   */
  if (!bResult)
  {
    ERR_FATAL("HAL_clk_SparkPLLConfigPLLSlew Update Failure", 0, 0, 0);
    return FALSE;
  }

  /*
   * Wait for 11 or more PLL clk_ref ticks for PLL_LOCK_DET to transition to 0.
   * When using a reference clock of XO, the wait requirement is 570 ns.
   */
  HAL_clk_BusyWait(1);

  /*
   * Wait for the PLL to lock during the 15 us timeout period.
   */
  bResult =
    HAL_clk_ValidateRegSetTimeout(
      HAL_CLK_PLL_HWIO_ADDR(PLL_MODE, pSource),
      HWIO_FMSK(PLL_MODE, PLL_LOCK_DET),
      HAL_CLK_LOCK_TIMEOUT_US);

  if (!bResult)
  {
    ERR_FATAL("HAL_clk_SparkPLLConfigPLLSlew Lock Failure", 0, 0, 0);
  }

  return bResult;

} /* END HAL_clk_SparkPLLConfigPLLSlew */


/* ===========================================================================
**  HAL_clk_SparkPLLConfigPLLNormal
**
**  Returns:
**    boolean - TRUE if successfully configured. FALSE otherwise.
** ======================================================================== */

static boolean HAL_clk_SparkPLLConfigPLLNormal
(
  HAL_clk_SourceDescType      *pSource,
  const HAL_clk_PLLConfigType *pConfig
)
{
  uint32  nModeVal, nUserVal;
  uint32  nPLLInFSMMode = 0;
  boolean bPLLEnabled = FALSE;

  /*
   * Check PLL mode.
   */
  nPLLInFSMMode = HAL_CLK_PLL_HWIO_IN(PLL_MODE, pSource) &
    HWIO_FMSK(PLL_MODE, PLL_VOTE_FSM_ENA);
  bPLLEnabled = HAL_clk_SparkPLLIsEnabled(pSource);

  /*
   * Disable the PLL.
   */
  if (bPLLEnabled)
  {
    HAL_clk_SparkPLLDisableNormal(pSource);
  }

  /*
   * Read the PLLx_Mode value
   */
  nModeVal  = HAL_CLK_PLL_HWIO_IN(PLL_MODE, pSource);

  /*
   * Disable FSM Mode
   */
  if (nPLLInFSMMode != 0)
  {
    nModeVal &= ~HWIO_FMSK(PLL_MODE, PLL_VOTE_FSM_ENA);
    HAL_CLK_PLL_HWIO_OUT(PLL_MODE, pSource, nModeVal);
  }

  /*
   * Program L/Alpha/AlphaU values.
   */
  HAL_CLK_PLL_HWIO_OUT(PLL_L_VAL, pSource, pConfig->nL);
  HAL_CLK_PLL_HWIO_OUT(PLL_ALPHA_VAL, pSource, pConfig->nFracVal);
  HAL_CLK_PLL_HWIO_OUT(PLL_ALPHA_U_VAL, pSource, (pConfig->nFracVal >> 32));

  /*
   * Get the CONFIG value and clear out fields we will configure.
   */
  nUserVal  =  HAL_CLK_PLL_HWIO_IN(PLL_USER_CTL, pSource);
  nUserVal &= ~(HWIO_FMSK(PLL_USER_CTL, ALPHA_EN) |
				HWIO_FMSK(PLL_USER_CTL, VCO_SEL) |
                HWIO_FMSK(PLL_USER_CTL, PRE_DIV_RATIO) |
                HWIO_FMSK(PLL_USER_CTL, POST_DIV_RATIO));
  /*
   * Find the VCO
   */
  switch (pConfig->nMode)
  {
	case SPARK_PLL_VCO2_1: 
	   /* 
	    * VCO 2.1, set VCO_SEL to 2 and enable the VCO boost mode
	    */
      nUserVal |= HWIO_FVAL(PLL_USER_CTL, VCO_SEL, SPARK_PLL_VCO2);
	  nUserVal |= HWIO_FVAL(PLL_USER_CTL, VCO_BOOST, 1);
	  break;

    case SPARK_PLL_VCO3:
	case SPARK_PLL_VCO2:
	case SPARK_PLL_VCO1:
	case SPARK_PLL_VCO0:
      nUserVal |= HWIO_FVAL(PLL_USER_CTL, VCO_SEL, (uint32) pConfig->nMode);
      break;

    default:
      ERR_FATAL("HAL_clk_SparkPLLConfigPLLNormal Invalid VCO Specification", 0, 0, 0);
      break;
  }

  /*
   * Program the pre-div value (div-1 to div-8).
   */
  if (pConfig->nPreDiv <= 8 && pConfig->nPreDiv > 0)
  {
    nUserVal |= HWIO_FVAL(PLL_USER_CTL, PRE_DIV_RATIO, (pConfig->nPreDiv - 1));
  }

  /*
   * Program the main post-div value (div-1,2,4,8 and 16 are supported)
   */
  if ((pConfig->PostDivs.Spark.nMain == 2) || (pConfig->PostDivs.Spark.nMain == 4) ||
      (pConfig->PostDivs.Spark.nMain == 8) || (pConfig->PostDivs.Spark.nMain == 16))
  {
    nUserVal |= HWIO_FVAL(PLL_USER_CTL, POST_DIV_RATIO, (pConfig->PostDivs.Spark.nMain - 1));
  }
  /*
   * Program the Aux post-div value (dive - 1, 3, 5, and 7 are supported)
   */
  if ((pConfig->PostDivs.Spark.nAux == 3) || (pConfig->PostDivs.Spark.nAux == 5) ||
      (pConfig->PostDivs.Spark.nAux == 7))
  {
	  nUserVal |= HWIO_FVAL(PLL_USER_CTL, AUX_POST_DIV_RATIO, (pConfig->PostDivs.Spark.nAux));
  }	  

  /*
   *  Check fractional output or integer output.
   */
  if (pConfig->nFracVal != 0)
  {
    nUserVal |= HWIO_FMSK(PLL_USER_CTL, ALPHA_EN);
  }

  /*
   * Finally program the USER_CTL register.
   */
  HAL_CLK_PLL_HWIO_OUT(PLL_USER_CTL, pSource, nUserVal);

  /*
   * If there is an address for a voting register, then the PLL is in
   * FSM voteable mode. In this case, the enable sequence is handled in
   * hardware, guided by the BIAS and LOCK counts.
   */
  if (nPLLInFSMMode)
  {
    /*
     * Put the FSM in reset.
     */
    nModeVal |= HWIO_FMSK(PLL_MODE, PLL_VOTE_FSM_RESET);
    HAL_CLK_PLL_HWIO_OUT(PLL_MODE, pSource, nModeVal);

    /*
     * Program the FSM portion of the mode register.
     */
    nModeVal &= ~HWIO_FMSK(PLL_MODE, PLL_BIAS_COUNT);
    nModeVal &= ~HWIO_FMSK(PLL_MODE, PLL_LOCK_COUNT);

    nModeVal |= HAL_CLK_PLL_BIAS_COUNT_VAL;
    nModeVal |= HAL_CLK_PLL_LOCK_COUNT_VAL;

    nModeVal |= HWIO_FMSK(PLL_MODE, PLL_VOTE_FSM_ENA);
    /*
     * Finally program the Mode register.
     */
    HAL_CLK_PLL_HWIO_OUT(PLL_MODE, pSource, nModeVal);

    /*
     * Take the FSM out of reset.
     */
    nModeVal &= ~HWIO_FMSK(PLL_MODE, PLL_VOTE_FSM_RESET);
    HAL_CLK_PLL_HWIO_OUT(PLL_MODE, pSource, nModeVal);
  }

  /*
   * Enable the PLL if it was on before and not in FSM mode (in FSM
   * mode this function is not used).
   */
  if (bPLLEnabled && nPLLInFSMMode == 0)
  {
    HAL_clk_SparkPLLEnableNormal(pSource);
  }

  return TRUE;

} /* END HAL_clk_SparkPLLConfigPLLNormal */


/* ===========================================================================
**  HAL_clk_SparkPLLConfigPLL
**
**  Returns:
**    boolean - TRUE if successfully configured. FALSE otherwise.
** ======================================================================== */

static boolean HAL_clk_SparkPLLConfigPLL
(
  HAL_clk_SourceDescType      *pSource,
  const HAL_clk_PLLConfigType *pConfig,
  HAL_clk_SourceConfigMode     eMode
)
{
  boolean bResult = FALSE;

  /*
   * Program the appropriate config sequence.
   */
  if (eMode == HAL_CLK_SOURCE_CONFIG_MODE_NORMAL)
  {
    bResult = HAL_clk_SparkPLLConfigPLLNormal(pSource, pConfig);
  }
  else if(eMode == HAL_CLK_SOURCE_CONFIG_MODE_SLEW)
  {
    bResult = HAL_clk_SparkPLLConfigPLLSlew(pSource, pConfig, TRUE);
  }
  else if (eMode == HAL_CLK_SOURCE_CONFIG_MODE_PENDING)
  {
    bResult = HAL_clk_SparkPLLConfigPLLSlew(pSource, pConfig, FALSE);
  }

  return bResult;

} /* END HAL_clk_SparkPLLConfigPLL */


/* ===========================================================================
**  HAL_clk_SparkPLLDetectPLLConfig
**
** ======================================================================== */

static void HAL_clk_SparkPLLDetectPLLConfig
(
  HAL_clk_SourceDescType *pSource,
  HAL_clk_PLLConfigType  *pConfig
)
{
  uint32 nConfigVal;

  /*
   * Read current L, Alpha, and AlphaU values.
   */
  pConfig->nL = HAL_CLK_PLL_HWIO_IN(PLL_L_VAL, pSource);
  /* 
   * Spark PLL support only ALPHA mode 
   */
  pConfig->eFracMode = HAL_CLK_PLL_FRAC_MODE_ALPHA;
  /* 
   * nFracVal is 64 bit value. 0-31 bits for ALPHA_VAL and 
   * 32-39 bits for ALPHA_U_VAL
   */
  pConfig->nFracVal = HAL_CLK_PLL_HWIO_IN(PLL_ALPHA_U_VAL, pSource);
  pConfig->nFracVal = pConfig->nFracVal << 32;
  pConfig->nFracVal |= HAL_CLK_PLL_HWIO_IN(PLL_ALPHA_VAL, pSource);
  
  /*
  * No reference clock mux, so default to 0.
  */
  pConfig->nSource = 0;
  
  /*
   * Get the VCO, nMode can be used to get the VCO value in the Spark PLL
   */
  nConfigVal = HAL_CLK_PLL_HWIO_IN(PLL_USER_CTL, pSource);
  pConfig->nMode = HAL_CLK_PLL_HWIO_INF(PLL_USER_CTL, pSource, VCO_SEL);

  /*
   * Spark PLL 11lpp is supporting VCO mode 2.1 which is identified by USER_CTL[22]
   */
  if ((pConfig->nMode == SPARK_PLL_VCO2) && HAL_CLK_PLL_HWIO_INF(PLL_USER_CTL, pSource, VCO_BOOST))
  {
	  pConfig->nMode = SPARK_PLL_VCO2_1;
  }
  /*
   * Get the pre and post dividers.
   */
  pConfig->nPreDiv = ((nConfigVal & HWIO_FMSK(PLL_USER_CTL, PRE_DIV_RATIO))
                         >> (HWIO_SHFT(PLL_USER_CTL, PRE_DIV_RATIO))) + 1;
  pConfig->PostDivs.Spark.nMain = ((nConfigVal & HWIO_FMSK(PLL_USER_CTL, POST_DIV_RATIO))
                >> HWIO_SHFT(PLL_USER_CTL, POST_DIV_RATIO)) + 1;
  pConfig->PostDivs.Spark.nAux = ((nConfigVal & HWIO_FMSK(PLL_USER_CTL, AUX_POST_DIV_RATIO))
                >> HWIO_SHFT(PLL_USER_CTL, AUX_POST_DIV_RATIO));
} /* END HAL_clk_SparkPLLDetectPLLConfig */


/* ===========================================================================
**  HAL_clk_SparkPLLIsPLLConfigEqual
**
** ======================================================================== */

static boolean HAL_clk_SparkPLLIsPLLConfigEqual
(
  const HAL_clk_PLLConfigType  *pConfigA,
  const HAL_clk_PLLConfigType  *pConfigB
)
{
  uint32 nMainDivA, nMainDivB;
  uint32 nAuxDivA, nAuxDivB;

  if (HAL_clk_GenericIsPLLConfigEqual(pConfigA, pConfigB))
  {
    /* Treat div-0 and div-1 as the same thing. */
    nMainDivA = MAX(1, pConfigA->PostDivs.Spark.nMain);
    nMainDivB = MAX(1, pConfigB->PostDivs.Spark.nMain);
    nAuxDivA = MAX(1, pConfigA->PostDivs.Spark.nAux);
    nAuxDivB = MAX(1, pConfigB->PostDivs.Spark.nAux);
	
    if ((nMainDivA == nMainDivB) && (nAuxDivA == nAuxDivB))
    {
      return TRUE;
    }
  }

  return FALSE;

} /* END HAL_clk_SparkPLLIsPLLConfigEqual */


/* ===========================================================================
**  HAL_clk_SparkPLLIsCalibrated
**
** ======================================================================== */

static boolean HAL_clk_SparkPLLIsCalibrated
(
  HAL_clk_SourceDescType *pSource
)
{
  /*
   * No way to dynamically query the Spark calibration status,
   * so just return that it's not calibrated to be safe.
   */
  return FALSE;

} /* HAL_clk_SparkPLLIsCalibrated */


/* ===========================================================================
**  HAL_clk_SparkPLLSetCalConfig
**
** ======================================================================== */

static boolean HAL_clk_SparkPLLSetCalConfig
(
  HAL_clk_SourceDescType      *pSource,
  const HAL_clk_PLLConfigType *pConfig
)
{
  HAL_CLK_PLL_HWIO_OUTF(PLL_USER_CTL_U, pSource, CAL_L_VAL, pConfig->nL);

  return TRUE;

} /* HAL_clk_SparkPLLSetCalConfig */


/* ===========================================================================
**  HAL_clk_SparkPLLSetRegSettings
**
** ======================================================================== */

static void HAL_clk_SparkPLLSetRegSettings
(
  HAL_clk_SourceDescType              *pSource,
  const HAL_clk_SourceRegSettingsType *pmSettings
)
{

     HAL_CLK_PLL_HWIO_OUT(PLL_CONFIG_CTL, pSource, pmSettings->nConfigCtl);
     HAL_CLK_PLL_HWIO_OUT(PLL_TEST_CTL, pSource, pmSettings->nTestCtl);
     HAL_CLK_PLL_HWIO_OUT(PLL_TEST_CTL_U, pSource, pmSettings->nTestCtl1);
     HAL_CLK_PLL_HWIO_OUT(PLL_USER_CTL, pSource, pmSettings->nUserCtl);
     HAL_CLK_PLL_HWIO_OUT(PLL_USER_CTL_U, pSource, pmSettings->nUserCtl1);

} /* END HAL_clk_SparkPLLSetRegSettings */

