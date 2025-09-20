/** @file ClockLibNull.c
  
  Stub functions for ClockLib

  Copyright (c) 2014-2016,2018 Qualcomm Technologies, Inc. All rights reserved.
**/

/*=============================================================================
                              EDIT HISTORY


 when       who     what, where, why
 --------   ---     -----------------------------------------------------------
 10/03/16   kpa     Added HAL_clk_SourceType, Clock_InitUSB
 05/06/15   plc     Add new clock stubs
 05/30/14   kpa     Initial revision

=============================================================================*/


/*==========================================================================

                               INCLUDE FILES

===========================================================================*/
#include "ClockBoot.h"
#include "DALSys.h"
#include "DDIChipInfo.h"
#ifdef CLOCK_ULOG_STUB
  #include "ClockULogStub.h"
#else
  #include "ULogFront.h"
#endif
#include "ClockDefs.h"
#include "Drivers/ClockDxe/ClockBSP.h"
#include "HALclk.h"

#ifdef CLOCK_NPA_STUB
#include "ClockNPAStub.h"
#else
  #include "npa.h"
  #include "npa_resource.h"
#endif


/*==========================================================================

                                  DEFINES

===========================================================================*/
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
  HAL_CLK_SOURCE_GPLL1,
  HAL_CLK_SOURCE_GPLL2,
  HAL_CLK_SOURCE_GPLL3,
  HAL_CLK_SOURCE_GPLL4,
  HAL_CLK_SOURCE_GPLL5,

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

  /*
   * External source
   */
  HAL_CLK_SOURCE_EXTERNAL,
  HAL_CLK_NUM_OF_EXTERNAL_SOURCES,

  /*
   * Raw
   */
  HAL_CLK_SOURCE_RAW0,
  HAL_CLK_SOURCE_RAW1,
  HAL_CLK_SOURCE_RAW2,

  /*
   * Misc
   */
  HAL_CLK_SOURCE_SLEEPCLK,
  HAL_CLK_SOURCE_GROUND,
  HAL_CLK_SOURCE_GPLL0_DIV2,

  HAL_CLK_NUM_OF_SOURCES,

  HAL_ENUM_32BITS(CLK_SOURCE)
} HAL_clk_SourceType;

/*
 * Clock handle type
 * NOTE: this can also be typedef uint32 * HAL_clk_ClockHandleType
 */
typedef void *HAL_clk_ClockHandleType;

/*
 * Clock domain handle type
 * NOTE: this can also be typedef uint32 * HAL_clk_ClockDomainHandleType
 */
typedef void *HAL_clk_ClockDomainHandleType;

/*
 * Power domain handle type
 * NOTE: this can also be typedef uint32 * HAL_clk_PowerDomainHandleType
 */
typedef void *HAL_clk_PowerDomainHandleType;

/**
 * Clock driver context.
 */
typedef struct
{
  DALSYSSyncObj                  LockObj;                               /**< Sync object storage */
  DALSYSSyncHandle               hLock;                                 /**< Handle to our mutex */

  ULogHandle                     hClockLog;                             /**< Handle to our ULOG log */
  uint32                         nGlobalFlags;                          /**< Global flags for the driver */

  ULogHandle                     hClockBISTLog;                         /**< Handle to our BIST ULOG log */
  boolean                        bBISTEnabled;                          /**< Flag used to enable the BIST */
  uint32                         nBISTLogSize;                          /**< Size of the BIST ULOG log */
  ClockNameListType             *pBISTExceptions;                       /**< Pointer to the BIST Exceptions List */

  ULogHandle                     hClockDebugLog;                        /**< Handle to our Debug ULOG log */

  boolean                        bDCVSEnabled;                          /**< DCVS state */
  boolean                        bCPREnabled;                           /**< CPR state */
  void                          *pImageCtxt;                            /**< Image context */

  uint8                          anSourceIndex[HAL_CLK_NUM_OF_SOURCES]; /**< Array of source indices */
  uint32                         nNumSources;                           /**< Number of supported sources */
  ClockSourceNodeType           *aSources;                              /**< Array of supported sources */
  uint32                         nNumClockDomains;                      /**< Number of supported clock domains */
  ClockDomainNodeType           *aClockDomains;                         /**< Array of supported clock domains */
  uint32                         nNumClocks;                            /**< Number of supported clocks */
  ClockNodeType                 *aClocks;                               /**< Array of supported clocks */
  uint32                         nNumPowerDomains;                      /**< Number of supported power domains */
  ClockPowerDomainNodeType      *aPowerDomains;                         /**< Array of supported power domains */
  ClockVRegConfigType            VRegConfig;                            /**< Voltage configuration */
  ClockVRegConfigType            VRegConfigSuppressible;                /**< Suppressible voltage configuration */
  boolean                        bSuspendVoltageRequest;                /**< Voltage suspension state */
  boolean                        bQdssEnabled;                          /**< QDSS enabled status */


} ClockDrvCtxt;

/*===========================================================================
                      FUNCTION DECLARATIONS
===========================================================================*/ 


/* ============================================================================
**  Function : Clock_Init
** ============================================================================
*/
/*!

    This function turns on the required clocks and configures
    Fabric and Krait speeds depending on the System Fabric and
    CPU boot performance level.

    @param eSysPerfLevel   -  [in] Fabric and DDR performance level to initialize.
    @param eCPUPerfLevel   -  [in] Scropion CPU performance level to initialize.

    @return
    TRUE -- Initialization was successful.
    FALSE -- Initialization failed.

   @dependencies
    None.

*/

boolean Clock_Init(ClockBootPerfLevelType eSysPerfLevel,
                    ClockBootPerfLevelType eCPUPerfLevel)
{
  return TRUE;
}


/* ========================================================================
**  Function : Clock_PreDDRInit
** ======================================================================*/
/*
    Description: Clock_PreDDRInitEx() was added in 8974 to provide a separate API
    for emergency download (EDL) without including Mxdroop work around. Adding wrapper 
    function to avoid compilation erros in 8x26 and 8x10.

    @param None
    @return
    TRUE -- Initialization was successful.
    FALSE -- Initialization failed.

    @dependencies
    None.

    @sa None
*/
boolean Clock_PreDDRInit( uint32 ddr_type )
{
  return TRUE;
}


/* ========================================================================
**  Function : Clock_I2CInit
** ======================================================================*/
/*
    Description: Configure all clocks needed for EEPROM to be used Pre DDR.

    @param None
    @return
    TRUE -- Initialization was successful.
    FALSE -- Initialization failed.

    @dependencies
    None.

    @sa None
*/
boolean Clock_I2CInit( void )
{
  return TRUE;
}

/* ============================================================================
**  Function : Clock_InitForDownloadMode
** ============================================================================
*/
/*!
    Configure clocks for download.  Enable every RAM we want to dump.

    @param None.
    @return
    TRUE -- Initialization was successful.
    FALSE -- Initialization failed.

    @dependencies
    None.

    @sa None
*/
void Clock_InitForDownloadMode(void)
{
}


/* ============================================================================
**  Function : Clock_ExitBoot
** ============================================================================
*/
/*!

    This function turns off clocks that were used during boot, but are not
    needed after boot.  This should be called at the end of boot.

    @return
    TRUE -- Initialization was successful.
    FALSE -- Initialization failed.

   @dependencies
    None.

*/
boolean Clock_ExitBoot(void)
{
  return TRUE;
}


/* ============================================================================
**  Function : Clock_DebugInit
** ============================================================================
*/
/*!

    This function is called very early in boot.  It is used for work-arounds that
    need to be done before JTAG attaches at the SBL "hold focus button" spin loop.

    @return
    TRUE -- Clean up was successful.
    FALSE -- Clean up failed.

   @dependencies
    You cannot use busywait in this function.

*/
boolean Clock_DebugInit(void)
{
  return TRUE;
}

/* ============================================================================
**  Function : Clock_SetSysPerfLevel
** ============================================================================
*/
/**
  Configure System and Application Fabric to a perf level.

  @param eSysPerfLevel [in]  -  System NOC and DDR performance level to configure.

  @return
  TRUE -- Fabric was configured to perf level successful.
  FALSE -- Configuration failed.

  @dependencies
  None.

  @sa
  Clock_Init.
*/

boolean Clock_SetSysPerfLevel
(
  ClockBootPerfLevelType eSysPerfLevel
)
{
  return TRUE;
}

/* ============================================================================
**  Function : Clock_SetL2PerfLevel
** ============================================================================
*/
/*!
    Configure L2 cache to a specific perf level.

    @param eL2PerfLevel   -  [IN] CPU performance level

    @return
    TRUE -- Initialization was successful.
    FALSE -- Initialization failed.

    @dependencies
    None.

    @sa None
*/
boolean Clock_SetL2PerfLevel(ClockBootPerfLevelType eL2PerfLevel)
{
  return TRUE;
}


/* ============================================================================
**  Function : Clock_SetCPUPerfLevel
** ============================================================================
*/
/**
  Configure Krait CPU to a specific perf level.

  @param eCPUPerfLevel [in] - CPU performance level.

  @return
  TRUE -- CPU was configured to perf level successful.
  FALSE -- Configuration failed.

  @dependencies
  None.

  @sa
  Clock_Init.
*/

boolean Clock_SetCPUPerfLevel(ClockBootPerfLevelType eCPUPerfLevel)
{
  return TRUE;
}


/* ============================================================================
**  Function : Clock_DDRSpeed
** ============================================================================
*/
/*!
    Return the DDR clock rate in kilohertz.  This clock rate is the bus speed.
    It is not the controller clock (2X) clock for targets that use Legacy mode.

    @param None.
    @return
      The speed configured in Clock_PreDDRInit() of the BIMC/DDR clock.

    @dependencies
    None.

    @sa None
*/
uint32 Clock_DDRSpeed()
{
  return 0x0;
}

/* =========================================================================
**  Function : Clock_EnterLowPowerMode
** =========================================================================*/
/*
  See DDIClock.h
*/

DALResult Clock_EnterLowPowerMode
(
  ClockDrvCtxt *pDrvCtxt
)
{
  DALResult           eReturnedResult = DAL_SUCCESS;
  return(eReturnedResult);
}

/* =========================================================================
**  Function : Clock_ExitLowPowerMode
** =========================================================================*/
/*
  See DDIClock.h
*/

DALResult Clock_ExitLowPowerMode
(
  ClockDrvCtxt *pDrvCtxt
)
{
  DALResult           eReturnedResult = DAL_SUCCESS;
  return(eReturnedResult);
}

/*=========================================================================
**  Function : Clock_NormalizeChipInfo
** =========================================================================*/
/*
  See ClockDriver.h
*/

void Clock_NormalizeChipInfo
(
  HAL_clk_ContextType *HALClkCtxt
)
{
}

/* =========================================================================
**  Function : Clock_InitImage
** =========================================================================*/
/*
  See DalClock.h
*/

DALResult Clock_InitImage
(
  ClockDrvCtxt *pDrvCtxt
)
{
  return DAL_SUCCESS;
}

/* =========================================================================
**  Function : Clock_InitVoltage
** =========================================================================*/
/*
  See ClockDriver.h
*/

DALResult Clock_InitVoltage
(
  ClockDrvCtxt *pDrvCtxt
)
{
  return DAL_SUCCESS;
} /* END Clock_InitVoltage */

/* =========================================================================
**  Function : Clock_VoltageRequest
** =========================================================================*/
/*
  See ClockDriver.h
*/

DALResult Clock_VoltageRequest
(
  ClockDrvCtxt          *pDrvCtxt,
  ClockVRegRequestType  *pCurrentRequest,
  ClockVRegRequestType  *pNewRequest
)
{
  return DAL_SUCCESS;
} /* END Clock_VoltageRequest */

/* ============================================================================
**  Function : Clock_InitRPM()
** ============================================================================
*/
/**
  Perform any initialization needed just before RPM starts.  SMEM is available
  at this time.

  @param 
  @return
  TRUE -- Initialization succeeded.
  FALSE -- Initialization failed.

  @dependencies
  None.

  @sa
  Clock_Init.
  boot_smem_init
*/
boolean Clock_InitRPM( void )
{
  return TRUE;
}

/* ============================================================================
**  Function : Clock_GetClockFrequency
** ============================================================================
*/
/*!
    Get the requested clock frequency in hertz.

    @param
      eBootLogClock [in]  - targeted clock
      pnFrequencyHz [out] - frequency of clock in hertz

    @return
      True iff the requested clock frequency is placed in pnFrequencyHz.

    @dependencies
    None.

    @sa None
*/

boolean Clock_GetClockFrequency(ClockBootLogQueryType eBootLogClock,
                                uint32 *pnFrequencyHz)
{
  boolean ret = TRUE;
  return ret;
}

/* =========================================================================
**  Function : Clock_SetBIMCSpeed
** =========================================================================*/
/*!
    Switch DDR and the related BIMC roots to the requested frequency

    @param -  nFreqHz [in] Frequency to search for in the table
    @return - FALSE on fail, TRUE on success

    @dependencies
    None.

    @sa None
*/
boolean Clock_SetBIMCSpeed(uint32 nFreqKHz )
{
  return TRUE;
}

/* ============================================================================
**  Function : Clock_InitVSense
** ============================================================================
*/
/*!
    This function enables the Voltage Sensor clocks, and configure them run
    at max frequency level at NOMINAL voltage.

    @return
      TRUE -- Initialization was successful.
      FALSE -- Initialization failed.

   @dependencies
     None.

*/
boolean Clock_InitVSense( void )
{
  return TRUE;
}

/* ============================================================================
**  Function : Clock_ShutdownVSense
** ============================================================================
*/
/*!
    This function disables the voltage sensor clocks.

    @return
      TRUE -- Initialization was successful.
      FALSE -- Initialization failed.

   @dependencies
      None.

*/
boolean Clock_ShutdownVSense( void )
{
  return TRUE;
}
/* ========================================================================
**  Function : Clock_UFSInit
** ======================================================================*/
/*!
    Get the requested clock frequency in hertz.

    @param
      ePerfLevel [in]  - Performance level

    @return
      True - Success init UFS clocks

    @dependencies
    None.

    @sa None
*/

boolean Clock_UFSInit( ClockBootPerfLevelType ePerfLevel )
{
  return TRUE;
}

/* ============================================================================
**  Function : Clock_InitUSB
** ============================================================================
*/
/*!
    Configure USB clocks.

    @param None.
    @return
    TRUE -- Initialization was successful.
    FALSE -- Initialization failed.

    @dependencies
    None.

    @sa None
*/
boolean Clock_InitUSB(void)
{
  return TRUE;
} /* END Clock_InitUSB */



/* ============================================================================
**  Function : Clock_SetSDCClockFrequency
** ============================================================================
*/
/*!
    Configure SDC clock to a specific perf level.

    @param eClockPerfLevel   -  [IN] SDC Clock perf level
           eClock            -  [IN] SDC clock to configure
    @return
    TRUE -- Initialization was successful.
    FALSE -- Initialization failed.

    @dependencies
    None.

    @sa None
*/
boolean Clock_SetSDCClockFrequency
(
  ClockBootPerfLevelType ePerfLevel,
  ClockSDCType eClock
)
{
  return TRUE;
}

/* ============================================================================
**  Function : Clock_Usb30EnableSWCollapse
** ============================================================================
*/
/*!
    Enable SW Collapse for USB30

    @param Enable/Disable.
    @return
    TRUE always

    @dependencies
    None.

    @sa None
   */
boolean Clock_Usb30EnableSWCollapse(boolean enable)
{
  return TRUE;
}


/* ============================================================================
**  Function : Clock_SetSDCClockFrequencyExt
** ============================================================================
*/
/*!
    Configure SDC clock to a specific perf level.

    @param eClockPerfLevel   -  [IN] Clock frequency level
           eClock            -  [IN] SDC clock to configure
    @return
    TRUE -- Initialization was successful.
    FALSE -- Initialization failed.

    @dependencies
    None.

    @sa None
*/
uint32 Clock_SetSDCClockFrequencyExt
(
  uint32 nFreqKHz,
  ClockSDCType eClock
)
{
  return TRUE;
}

/* ============================================================================
**  Function : Clock_Usb30GetSWCollapse
** ============================================================================
*/
/*!
    Returns the status of SW Collapse for USB30

    @param None
    @return
    TRUE if enabled
    FALSE if disabled

    @dependencies
    None.

    @sa None
*/
boolean Clock_Usb30GetSWCollapse(void)
{
  return FALSE;
}

/* ============================================================================
**  Function : Clock_DisableUSB
** ============================================================================
*/
/*!
    Disable USB clocks.

    @param None.
    @return
    TRUE -- Disable was successful.
    FALSE -- Disable failed.

    @dependencies
    None.

    @sa None
*/
boolean Clock_DisableUSB(void)
{
  return TRUE;
}


/* ========================================================================
**  Function : Clock_ConfigureDDR
** ======================================================================*/
/*
    Description: Configure all clocks needed for DDR configuration.  This
    extension API is used for bootup and emergency download mode.

    @param None
    @return
    TRUE -- Initialization was successful.
    FALSE -- Initialization failed.

    @dependencies
    None.

    @sa None
*/
boolean Clock_ConfigureDDR( void )
{
  return TRUE;
}

/* ============================================================================
**  Function : Clock_SetQUPV3Frequency
** ============================================================================
*/
/*!
    Set QUP V3 clock frequency.

    @param
      eClock   - QUPV3 index
      nFreqKHz   Clock frequency in KHz

    @return
    TRUE -- Set frequency success
    FALSE -- Set frequency failed.

    @dependencies
    None.

    @sa None
*/
boolean Clock_SetQUPV3Frequency(ClockQUPV3Type eClock, uint32 nFreqKHz)
{
  return TRUE;
}
/* ========================================================================
**  Function : Clock_UFS2CardInit
** ======================================================================*/
/*!
    Get the requested clock frequency in hertz.

    @param
      ePerfLevel [in]  - Performance level

    @return
      True - Success init UFS_2 Card clocks

    @dependencies
    None.

    @sa None
*/

boolean Clock_UFS2CardInit( ClockBootPerfLevelType ePerfLevel )
{
  return TRUE;
}
