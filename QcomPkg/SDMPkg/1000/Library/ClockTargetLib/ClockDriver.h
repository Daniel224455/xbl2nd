#ifndef CLOCKDRIVER_H
#define CLOCKDRIVER_H

/**
==============================================================================

  @file ClockDriver.h

  Clock driver functions for PLL and clock management

  Copyright (c) 2016-2017 QUALCOMM Technologies, Inc.  All Rights Reserved.  
  QUALCOMM Proprietary and Confidential.

==============================================================================


==============================================================================

                            EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.


  when      who     what, where, why
  --------  ------  ------------------------------------------------------
  11/11/16  vphan   Initial SDM845 revision, branched from MSM8998.

==============================================================================
*/

#include <HALcomdef.h>

/*=========================================================================
      Definitions
==========================================================================*/
#define CLK_ELEM_SIZE(array)      sizeof((array)[0])
#define CLK_NUM_ELEM(array)       (sizeof(array) / CLK_ELEM_SIZE(array))

#define CLK_BITS_ARE_CLR(nVal, nMask)    (((nVal) & (nMask)) == 0)
#define CLK_BITS_ARE_SET(nVal, nMask)    (((nVal) & (nMask)) == (nMask))

#define CLK_ENABLE_MSK 0x00000001
#define CLK_OFF_MSK    0x80000000
#define CLK_GDSCR_SW_COLLAPSE_MSK 0x00000001
#define CLK_GDSCR_PWR_ON_MSK      0x80000000
#define CLK_GDSCR_TIMEOUT_US      150

/*
 * RCGR update timeout. In practice this should be very short (less than 1us),
 * but it depends on the clock frequency being fed into the RCG. Choosing
 * a very conservative value.
 */
#define CLOCK_UPDATE_TIMEOUT_US 1000

/* Sync to HSR Poipu v8.0 */
#define TRION_CONFIG_CTL_VAL    0x20485699
#define TRION_CONFIG_CTL_U_VAL  0x00002267
#define TRION_CONFIG_CTL_U1_VAL 0x00000024

#define TRION_TEST_CTL_VAL    0x00000000
#define TRION_TEST_CTL_U_VAL  0x00000002
#define TRION_TEST_CTL_U1_VAL 0x00000020

/* USER_CTL should be set on a per PLL basis to enable either main or main and even */
#define TRION_USER_CTL_MAIN_EVEN_VAL  0x3
#define TRION_USER_CTL_MAIN_VAL       0x1
#define TRION_USER_CTL_U_VAL  0x00000805
#define TRION_USER_CTL_U1_VAL 0x000000D0

#define DIV2X(d) ((int)(2*(d)))

/*=========================================================================
      Type Definitions
==========================================================================*/

/* These values are used to select sources in Kryo-B */
typedef enum {
  APCS_SOURCE_CXO        = 0x0, // refRoot_Clk
  APCS_SOURCE_GPLL0_MAIN = 0x1, 
  APCS_SOURCE_AG_EARLY   = 0x2, // Apps PLL early output
  APCS_SOURCE_AG_MAIN    = 0x3, // Apps PLL main output
} ClockAPCSSourceType;

typedef enum {
  ACPS_SOURCE_CXO_STATUS_MASK        = (1<<31),
  APCS_SOURCE_GPLL0_MAIN_STATUS_MASK = (1<<30),
  APCS_SOURCE_AG_EARLY_STATUS_MASK   = (1<<29),
  APCS_SOURCE_AG_MAIN_STATUS_MASK    = (1<<28)
} Clock_APCSSourceStatusMaskType;


typedef struct {
  uint8  nCPUSource;
  uint8  nLVal;
  uint8  nDiv2x;
  uint8  nReserved;
  uint32 nFreqKHz;
} ClockAPCSCfgType;

/*
 * HAL_clk_SourceType : Adopted from RPM for shared data compatibility
 *
 */
typedef enum
{
  HAL_CLK_SOURCE_NULL             = 0,

  /*
   * XO
   */
  HAL_CLK_SOURCE_XO               = 1,

  /*
   * GCC PLL
   */
  HAL_CLK_SOURCE_GPLL0,
  HAL_CLK_SOURCE_GPLL0_EVEN,
  HAL_CLK_SOURCE_GPLL1,
  HAL_CLK_SOURCE_GPLL2,
  HAL_CLK_SOURCE_GPLL3,
  HAL_CLK_SOURCE_GPLL4,
  HAL_CLK_SOURCE_GPLL5,
  HAL_CLK_SOURCE_GPLL6,
  HAL_CLK_SOURCE_GPLL7,
  HAL_CLK_SOURCE_GPLL8,
  HAL_CLK_SOURCE_GPLL9,
  HAL_CLK_SOURCE_GPLL10,

  /*
   * AOSS PLL
   */
  HAL_CLK_SOURCE_AOPLL0_EVEN,
  HAL_CLK_SOURCE_AOPLL1,

  /*
   * MMSS PLL
   */
  HAL_CLK_SOURCE_MMPLL0,
  HAL_CLK_SOURCE_MMPLL1,
  HAL_CLK_SOURCE_MMPLL2,
  HAL_CLK_SOURCE_MMPLL3,
  HAL_CLK_SOURCE_MMPLL4,
  HAL_CLK_SOURCE_MMPLL5,
  HAL_CLK_SOURCE_MMPLL10,

  HAL_CLK_NUM_OF_SOURCES,

  HAL_ENUM_32BITS(CLK_SOURCE)
} HAL_clk_SourceType;

/*
 * ClockSourceType
 *
 * A type to be used for clock sources in the configuration data and functions.
 */

typedef enum {

  SRC_CXO         = HAL_CLK_SOURCE_XO,
  SRC_GPLL0       = HAL_CLK_SOURCE_GPLL0,
  SRC_GPLL0_EVEN  = HAL_CLK_SOURCE_GPLL0_EVEN,
  SRC_GPLL1       = HAL_CLK_SOURCE_GPLL1,
  SRC_GPLL2       = HAL_CLK_SOURCE_GPLL2,
  SRC_GPLL3       = HAL_CLK_SOURCE_GPLL3,
  SRC_GPLL4       = HAL_CLK_SOURCE_GPLL4,
  SRC_GPLL5       = HAL_CLK_SOURCE_GPLL5,
  SRC_GPLL6       = HAL_CLK_SOURCE_GPLL6,
  SRC_GPLL7       = HAL_CLK_SOURCE_GPLL7,
  SRC_GPLL8       = HAL_CLK_SOURCE_GPLL8,
  SRC_GPLL9       = HAL_CLK_SOURCE_GPLL9,
  SRC_GPLL10      = HAL_CLK_SOURCE_GPLL10,
  SRC_AOPLL0_EVEN = HAL_CLK_SOURCE_AOPLL0_EVEN,
  SRC_AOPLL1      = HAL_CLK_SOURCE_AOPLL1,
  SRC_EXT1,
  SRC_EXT2,
  SRC_DISP_PLL0,
  SRC_DISP_PLL1
} ClockSourceType;

/*
 * ClockSourceMuxType
 *
 * A type to choose the mapping for the mux.
 */
typedef enum {
  MUX_GCC,
  MUX_AOSS,
  MUX_DISP,
  NUM_MUX_TYPES
} ClockSourceMuxType;

/*
 * ClockConfigPLLType
 *
 * Parameters used for configuring a source that is a PLL.
 *  bConfig       - Flag for determine that PLL is already configured or not
 *  nPLLModeAddr  - The address of the PLL MODE register.
 *  nVoteAddr     - The address of the PLL voting register. Set to NULL for
 *                  non-FSM mode.
 *  nVoteMask     - The mask of the voting bit.
 *  nPreDiv       - The pre-divider value (generally 1 or 2).
 *  nPostDivOdd   - The post-divider odd (1,3,5,7) value
 *  nPostDivEven  - The post-divider (1,2,4,8) value
 *  nL            - The L value for the PLL.  The PLL output frequency is derived
 *                  as out_freq = ((in_freq / nPreDiv) * (L + M/N)) / nPostDiv.
 *  nCalibrationL - Calibration value
 *  nAlpha        - The Alpha(prog) value. For the 20nm PLLs, the output frequency is
 *                  derived as :
 *                  out_freq = ((in_freq / nPreDiv) * (L + ALPHAfrac)) / nPostDiv.
 *                  ALPHAprog = 2^b x ALPHAfrac where 'b' is 40 for Prius and 40 for Tesla.
 * nUserCtl,U,U1  - User control value (96-bits).  Need three registers
 * nConfigCtl,U,U1- Config control value (96-bits).  Need three registers
 * nTestCtl,U,U1  - Test control value (96-bits).  Need three registers
 */
typedef struct ClockConfigPLL
{
  boolean bConfig;
  uint8   nRefCount;
  uintnt  nPLLModeAddr;
  uintnt  nVoteAddr;
  uintnt  nVoteMask;
  uint32  nPreDiv;
  uint32  nPostDivOdd;
  uint32  nPostDivEven;
  uint32  nL;
  uint32  nCalibrationL;
  uint32  nAlpha;
  uint32  nUserCtl;
  uint32  nUserCtl_U;
  uint32  nUserCtl_U1;
  uint32  nConfigCtl;
  uint32  nConfigCtl_U;
  uint32  nConfigCtl_U1;
  uint32  nTestCtl;
  uint32  nTestCtl_U;
  uint32  nTestCtl_U1;
} ClockConfigPLLType;

/*
 * ClockConfigMuxType
 *
 * Parameters used for configuring a standard clock multiplexer.
 *
 *  nCMDRCGRAddr - The address of the CMD RCGR register.
 *  eMux        - The mux type for mapping eSource to register source values.
 *  eSource     - The source to use.
 *  nDiv2x      - The integer (2 * divider) value to use.
 *  nM          - The M value for any M/N counter, or 0 to bypass.
 *  nN          - The N value for any M/N counter.
 *  n2D         - Twice the D value for any M/N counter.
 */
typedef struct ClockConfigMux
{
  uintnt nCMDRCGRAddr;
  uint32 nPerfLevel;
  ClockSourceMuxType eMux;
  ClockSourceType eSource;
  uint16 nDiv2x;
  uint16 nM;
  uint16 nN;
  uint16 n2D;
} ClockConfigMuxType;


/**
 * Clock config setting CP data for sharing with AOP
 */
typedef struct
{
  uint8 nSHUB;        // System HUB
  uint8 nSNOC;        // System NOC
  uint8 nCNOC;        // Config NOC
  uint8 nCDSP;        // CDSP NOC
  uint8 nCE;          // Crypto Engine
  uint8 nIPA;         // IPA
  uint8 nDDR;         // DDR
  uint8 nReserved;   // Reserved
} ClockCfgCPType;

/*
 * ClockSrcDynCfgType
 */
typedef struct
{
  uint32 nFreqKHz;
  uint16 nSource;      // Source
  uint16 nL;
  uint32 nAlpha;
  uint8 nDiv2x;
} ClockSrcDynCfgType;

/*
 * Clock_CBCRtoggleType
 *
 * A type to choose the the operation on clocks(enable/disable).
 */
typedef enum{
  CLK_TOGGLE_DISABLE    = 0,
  CLK_TOGGLE_ENABLE     = 1,
  NUM_CLK_TOGGLE_TYPES
} Clock_CBCRtoggleType;

typedef struct
{
  ClockSourceType   ePLL;
  ClockConfigPLLType PLLCfg;
} ClockSourceConfigType;

typedef struct
{
  uint32                nFreqKHz;
  ClockConfigMuxType    Cfg;
} ClockFreqMuxCfgType;

/* Macros to check to see if this is a presilicon simulation platform, that has issues */
#define SOC_RUMI 1
#define RUMI_CHECK() (HWIO_IN(TCSR_SOC_EMULATION_TYPE) == SOC_RUMI)

/* ============================================================================
**  Function : Clock_EnablePLL
** ============================================================================
*/
/*!
    Configure and enable a PLL.  If it is voteable, it will be set for FSM mode
    and voted for using the vote register.

    @param pConfig -  [IN] PLL configuration structure
    @return
    TRUE -- Initialization was successful.
    FALSE -- Initialization failed.

    @dependencies
    None.

    @sa None
*/
boolean Clock_EnablePLL( ClockConfigPLLType *pConfig );

/* ============================================================================
**  Function : Clock_DisablePLL
** ============================================================================
*/
/*!
    Disable a PLL.

    @param pConfig -  [IN] PLL configuration structure
    @return
    TRUE -- The PLL was succesfully disabled.
    FALSE -- Disable failed.

    @dependencies
    None.

    @sa None
*/
boolean Clock_DisablePLL( ClockConfigPLLType *pConfig );

/* =========================================================================
**  Function : Clock_ConfigurePLL
** =========================================================================*/
/**
  Configures a PLL but do not enable it.  If the PLL is already enabled or
  set to FSM mode, this will fail.

  @param *pConfig [in] -- PLL configuration
  @return
  TRUE -- Configuration was successful
  FALSE -- The PLL was already enabled or in FSM mode.

*/
boolean Clock_ConfigurePLL( ClockConfigPLLType *pConfig);


/* =========================================================================
**  Function : Clock_ConfigMux
** =========================================================================*/
/*!
    Configure a clock mux which turn on the source and switch mux.

    @param pConfig -  [IN] Clock mux config structure
    @return
    TRUE -- Initialization was successful.
    FALSE -- Initialization failed.

    @dependencies
    None.

    @sa None
*/
void Clock_SetClockConfig( const ClockConfigMuxType *pClockCfg );


/* =========================================================================
**  Function : Clock_ConfigMux
** =========================================================================*/
/*!
    Configure a clock mux.

    @param pConfig -  [IN] Clock mux config structure
    @return
    TRUE -- Initialization was successful.
    FALSE -- Initialization failed.

    @dependencies
    None.

    @sa None
*/
boolean Clock_ConfigMux (const ClockConfigMuxType *pConfig);

/* =========================================================================
**  Function : Clock_DetectMuxCfg
** =========================================================================*/
/*!
    Detect a clock mux configuration.

    @param pMuxCfg -  [IN] Clock mux config structure.
    @param CurrentCfg -  [IN] Current hardware Clock mux config structure.
    @return
    TRUE -- Initialization was successful.
    FALSE -- Initialization failed.

    @dependencies
    None.

    @sa None
*/

boolean Clock_DetectMuxCfg(const ClockConfigMuxType *pMuxCfg,ClockConfigMuxType *CurrentCfg);

/* =========================================================================
**  Function : Clock_TriggerUpdate
** =========================================================================*/
/*!
    Trigger a clock switch, and wait for a timeout.

    @param cmdReg : HWIO_ADDR(*_CMD_CBCR)
    @return
    TRUE -- Clock switch completed.
    FALSE -- Clock switch timed out.

    @dependencies
    None.

    @sa None
*/
boolean Clock_TriggerUpdate(uintnt cmdReg, uintnt nMask);

/* =========================================================================
**  Function : Clock_ToggleClock
** =========================================================================*/
/*!
    Enable/Disable a Clock.

    @param CBCR_addr - Address of the CBCR register
           enable :-  enable/disable the CBCR
    TRUE -- CBCR programming successful.
    FALSE -- CBCR programming failed.

    @dependencies
    None.

    @sa None
*/

boolean Clock_ToggleClock(uintnt CBCR_addr, Clock_CBCRtoggleType clk_enable);


/* ===========================================================================
**  Clock_PowerDomainEnable
** ======================================================================== */
boolean Clock_PowerDomainEnable ( uintnt nGDSCRAddr );

/* ===========================================================================
**  Clock_PowerDomainDisable
** ======================================================================== */
boolean Clock_PowerDomainDisable ( uintnt nGDSCRAddr );


/* ============================================================================
**  Function : Clock_ToggleEnableList
** ============================================================================
*/
void Clock_ToggleEnableList( uint64* pList );

/* ============================================================================
**  Function : Clock_ChipVersion
** ============================================================================
  A simple function to read and cache the chip version
*/
int32 Clock_ChipVersion();

#endif /* CLOCKDRIVER_H */
