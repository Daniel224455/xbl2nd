/*
===========================================================================
*/
/**
  @file ClockApps.c

  Main entry point for the SC8180X main application processor clock driver.
*/
/*
  ====================================================================

  Copyright (c) 2012-2020 QUALCOMM Technologies Incorporated.  All Rights Reserved.
  QUALCOMM Proprietary and Confidential.

  ====================================================================
  ====================================================================
*/


/*=========================================================================
      Include Files
==========================================================================*/

#include "ClockDriver.h"
#include "ClockApps.h"
#include "HALhwio.h"
#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Protocol/EFIPlatformInfoTypes.h>
#include <Protocol/EFIPlatformInfo.h>

#include <Pi/PiStatusCode.h>
#include <Protocol/StatusCode.h>
#include <Protocol/ReportStatusCodeHandler.h>
#include <Library/BaseMemoryLib.h>
#include "Lpm.h"


/*=========================================================================
      Type Definitions
==========================================================================*/

/*=========================================================================
      Constants and Macros
==========================================================================*/

/* Initial CPU cluster frequencies -- highest available at Nominal Corner */
#ifdef ENABLE_AUTO_BOOT_OPTIMIZATION
#define SILVER_CLUSTER_INITIAL_PERF_LEVEL        13          // OSM perf level (1766.4 MHz)
#define L3_CLUSTER_INITIAL_PERF_LEVEL            12          // OSM perf level (1651.2 MHz)
#define GOLD_CLUSTER_INITIAL_PERF_LEVEL          14          // OSM perf level (2496 MHz)
#else
#define SILVER_CLUSTER_INITIAL_PERF_LEVEL     	 12           // OSM perf level (1670.4 MHz)
#define L3_CLUSTER_INITIAL_PERF_LEVEL         	 11          // OSM perf level (1536 MHz)
#endif
/*=========================================================================
      Externs
==========================================================================*/

extern HAL_clk_ClockControlType           HAL_clk_GenericClockControl;
extern HAL_clk_ClockDomainControlType     HAL_clk_GenericDomainControl;
extern EFI_STATUS Clock_RegisterRPMHClient();
extern EFI_STATUS Clock_RegisterBootServerices (ClockDrvCtxt *pDrvCtxt);

/*=========================================================================
      Function Prototypes
==========================================================================*/

extern DALResult Clock_SetCPUConfig
(
  ClockDrvCtxt                  *pDrvCtxt,
  ClockAppsCPUType               eCPU,
  ClockAppsPerfLevelConfigType  *pPerfConfig
);

extern void Clock_MdssMdpRequest
(
  ClockDrvCtxt *pDrvCtxt,
  BOOLEAN       bEnable
);

extern void Clock_LogCpuFreq(void);

/*=========================================================================
      Data
==========================================================================*/

/*
 * Clock Apps context.
 */
ClockAppsCtxtType Clock_AppsCtxt;

/*=========================================================================
      CPU Clock Domains
==========================================================================*/

static ClockDomainNodeType ClockDomain_CPU_C0 =
{
  .aBSP      = NULL,
  .nBSPLen   = 0,
  .pRail     = NULL,
  .pSource   = NULL,
  .nFlags    = CLOCK_FLAG_READ_ONLY,
  .HALDomain = {
    .nCGRAddr   = 0,
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_CPU_C1 =
{
  .aBSP      = NULL,
  .nBSPLen   = 0,
  .pRail     = NULL,
  .pSource   = NULL,
  .nFlags    = CLOCK_FLAG_READ_ONLY,
  .HALDomain = {
    .nCGRAddr   = 0,
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_CPU_C2 =
{
  .aBSP      = NULL,
  .nBSPLen   = 0,
  .pRail     = NULL,
  .pSource   = NULL,
  .nFlags    = CLOCK_FLAG_READ_ONLY,
  .HALDomain = {
    .nCGRAddr   = 0,
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_CPU_C3 =
{
  .aBSP      = NULL,
  .nBSPLen   = 0,
  .pRail     = NULL,
  .pSource   = NULL,
  .nFlags    = CLOCK_FLAG_READ_ONLY,
  .HALDomain = {
    .nCGRAddr   = 0,
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_CPU_C4 =
{
  .aBSP      = NULL,
  .nBSPLen   = 0,
  .pRail     = NULL,
  .pSource   = NULL,
  .nFlags    = CLOCK_FLAG_READ_ONLY,
  .HALDomain = {
    .nCGRAddr   = 0,
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_CPU_C5 =
{
  .aBSP      = NULL,
  .nBSPLen   = 0,
  .pRail     = NULL,
  .pSource   = NULL,
  .nFlags    = CLOCK_FLAG_READ_ONLY,
  .HALDomain = {
    .nCGRAddr   = 0,
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_CPU_C6 =
{
  .aBSP      = NULL,
  .nBSPLen   = 0,
  .pRail     = NULL,
  .pSource   = NULL,
  .nFlags    = CLOCK_FLAG_READ_ONLY,
  .HALDomain = {
    .nCGRAddr   = 0,
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_CPU_C7 =
{
  .aBSP      = NULL,
  .nBSPLen   = 0,
  .pRail     = NULL,
  .pSource   = NULL,
  .nFlags    = CLOCK_FLAG_READ_ONLY,
  .HALDomain = {
    .nCGRAddr   = 0,
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_CPU_L3 =
{
  .aBSP      = NULL,
  .nBSPLen   = 0,
  .pRail     = NULL,
  .pSource   = NULL,
  .nFlags    = CLOCK_FLAG_READ_ONLY,
  .HALDomain = {
    .nCGRAddr   = 0,
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType *aCpuDomains[] =
{
  &ClockDomain_CPU_C0,
  &ClockDomain_CPU_C1,
  &ClockDomain_CPU_C2,
  &ClockDomain_CPU_C3,
  &ClockDomain_CPU_C4,
  &ClockDomain_CPU_C5,
  &ClockDomain_CPU_C6,
  &ClockDomain_CPU_C7,
  &ClockDomain_CPU_L3,
};

/*=========================================================================
      CPU Clocks
==========================================================================*/

static ClockNodeType aCpuClocks[] =
{
  {
    .szName    = "apcs_silver_0_clk",
    .pDomain   = &ClockDomain_CPU_C0,
    .pDebugMux = NULL,
    .nUniqueId = 0x0,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { 0, 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = HAL_CLK_NO_DEBUG_MUX_SEL,
    },
  },
  {
    .szName    = "apcs_silver_1_clk",
    .pDomain   = &ClockDomain_CPU_C1,
    .pDebugMux = NULL,
    .nUniqueId = 0x0,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { 0, 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = HAL_CLK_NO_DEBUG_MUX_SEL,
    },
  },
  {
    .szName    = "apcs_silver_2_clk",
    .pDomain   = &ClockDomain_CPU_C2,
    .pDebugMux = NULL,
    .nUniqueId = 0x0,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { 0, 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = HAL_CLK_NO_DEBUG_MUX_SEL,
    },
  },
  {
    .szName    = "apcs_silver_3_clk",
    .pDomain   = &ClockDomain_CPU_C3,
    .pDebugMux = NULL,
    .nUniqueId = 0x0,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { 0, 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = HAL_CLK_NO_DEBUG_MUX_SEL,
    },
  },
  {
    .szName    = "apcs_gold_0_clk",
    .pDomain   = &ClockDomain_CPU_C4,
    .pDebugMux = NULL,
    .nUniqueId = 0x0,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { 0, 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = HAL_CLK_NO_DEBUG_MUX_SEL,
    },
  },
  {
    .szName    = "apcs_gold_1_clk",
    .pDomain   = &ClockDomain_CPU_C5,
    .pDebugMux = NULL,
    .nUniqueId = 0x0,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { 0, 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = HAL_CLK_NO_DEBUG_MUX_SEL,
    },
  },
  {
    .szName    = "apcs_gold_2_clk",
    .pDomain   = &ClockDomain_CPU_C6,
    .pDebugMux = NULL,
    .nUniqueId = 0x0,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { 0, 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = HAL_CLK_NO_DEBUG_MUX_SEL,
    },
  },
  {
    .szName    = "apcs_gold_3_clk",
    .pDomain   = &ClockDomain_CPU_C7,
    .pDebugMux = NULL,
    .nUniqueId = 0x0,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { 0, 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = HAL_CLK_NO_DEBUG_MUX_SEL,
    },
  },
  {
    .szName    = "apcs_l3_clk",
    .pDomain   = &ClockDomain_CPU_L3,
    .pDebugMux = NULL,
    .nUniqueId = 0x0,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { 0, 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = HAL_CLK_NO_DEBUG_MUX_SEL,
    },
  },
};


/*=========================================================================
      Externals
==========================================================================*/

/*=========================================================================
      Functions
==========================================================================*/


/* =========================================================================
**  Function : Clock_HMSSAHBNodeReadyCallback
** =========================================================================*/
/**
  Callback when /clk/hmss_ahb is ready.

  @param *pContext [in] -- Context pointer, the driver context in this case.
  @param nEventType -- Not used.
  @param *pResourceName -- Name of the resource created.
  @param nResourceNameLen -- Length of the name of the resource created.

  @return
  NULL

  @dependencies
  None.
*/

static void Clock_HMSSAHBNodeReadyCallback
(
  void          *pContext,
  unsigned int   nEventType,
  void          *pResourceName,
  unsigned int   nResourceNameLen
)
{
  npa_client_handle  hNPAHMSSAHB;
  ClockDrvCtxt      *pDrvCtxt = (ClockDrvCtxt *)pContext;

  hNPAHMSSAHB = npa_create_sync_client(pResourceName,
    "/clock/hmss_ahb", NPA_CLIENT_SUPPRESSIBLE);

  if (hNPAHMSSAHB)
  {
    /*
     * Since nobody is managing this clock, make a suppressible
     * request for max frequency at LOW voltage. Otherwise, the clock gets
     * left on requiring NOMINAL voltage with no vote for it.
     */
    clock_npa_issue_scalar_request(hNPAHMSSAHB, 19200);
  }
  else
  {
    ULOG_RT_PRINTF_1 (pDrvCtxt->hClockLog,
                      "DALSYS_LOGEVENT_ERROR: Failed to create NPA client for %s.",
                      (char *)pResourceName);
  }

} /* END Clock_HMSSAHBNodeReadyCallback */


/* =========================================================================
**  Function : Clock_MMNOCAHBNodeReadyCallback
** =========================================================================*/
/**
  Callback when /clk/mmnoc_ahb is ready.

  @param *pContext [in] -- Context pointer, the driver context in this case.
  @param nEventType -- Not used.
  @param *pResourceName -- Name of the resource created.
  @param nResourceNameLen -- Length of the name of the resource created.

  @return
  NULL

  @dependencies
  None.
*/

static void Clock_MMNOCAHBNodeReadyCallback
(
  void          *pContext,
  unsigned int   nEventType,
  void          *pResourceName,
  unsigned int   nResourceNameLen
)
{
  npa_client_handle  hNPAMMNOCAHB;
  ClockDrvCtxt      *pDrvCtxt = (ClockDrvCtxt *)pContext;

  hNPAMMNOCAHB = npa_create_sync_client(pResourceName,
    "/clock/mmnoc_ahb", NPA_CLIENT_SUPPRESSIBLE);

  if (hNPAMMNOCAHB)
  {
    /*
     * Since RPM can turn off this clock, and we'll always need it when apps
     * is running, make a suppressible request for the fastest frequency
     * allowed at LOW voltage.
     */
    clock_npa_issue_scalar_request(hNPAMMNOCAHB, 40000);
  }
  else
  {
    ULOG_RT_PRINTF_1 (pDrvCtxt->hClockLog,
                      "DALSYS_LOGEVENT_ERROR: Failed to create NPA client for %s.",
                      (char *)pResourceName);
  }

} /* END Clock_MMNOCAHBNodeReadyCallback */


/* =========================================================================
**  Function : Clock_InitTarget
** =========================================================================*/
/*
  See ClockDriver.h
*/

DALResult Clock_InitTarget
(
  ClockDrvCtxt *pDrvCtxt
)
{
  DALResult          eResult = DAL_SUCCESS;
  ClockAppsBSPType  *pAppsBSPConfig;
  uint32             i;
  uint32             nFrequencyHz;
  ClockIdType        nClockId;
  ClockNodeType     *pClock;
  EFI_STATUS         Status;

  /*
   * Hookup the apps context to the driver context.
   */
  pDrvCtxt->pImageCtxt = &Clock_AppsCtxt;

  /*-----------------------------------------------------------------------*/
  /* Initialize the Apps BSP data.                                         */
  /*-----------------------------------------------------------------------*/

  pAppsBSPConfig = Clock_GetAppsBSP();

  for (i = 0; i < CLOCK_APPS_CPU_TOTAL; i++)
  {
    /*
     * Hookup the BSP data.
     */
    switch (i)
    {
      case CLOCK_APPS_CPU_C0:
      case CLOCK_APPS_CPU_C1:
      case CLOCK_APPS_CPU_C2:
      case CLOCK_APPS_CPU_C3:
        Clock_AppsCtxt.aCPU[i].pBSPConfig = &pAppsBSPConfig->aCPUConfig[CLOCK_APPS_CPU_CLUSTER_C0];
        break;

      case CLOCK_APPS_CPU_C4:
      case CLOCK_APPS_CPU_C5:
      case CLOCK_APPS_CPU_C6:
      case CLOCK_APPS_CPU_C7:
        Clock_AppsCtxt.aCPU[i].pBSPConfig = &pAppsBSPConfig->aCPUConfig[CLOCK_APPS_CPU_CLUSTER_C1];
        break;

      case CLOCK_APPS_CPU_L3:
        Clock_AppsCtxt.aCPU[i].pBSPConfig = &pAppsBSPConfig->aCPUConfig[CLOCK_APPS_CPU_CLUSTER_L3];
        break;

      default:
        return DAL_ERROR_INTERNAL;
    }
  }

  eResult = Clock_InitAppsBSP(pDrvCtxt);
  if (eResult != DAL_SUCCESS)
  {
    return DAL_ERROR_INTERNAL;
  }

  /*-----------------------------------------------------------------------*/
  /* Name and configure the CPU clocks.                                    */
  /*-----------------------------------------------------------------------*/

  for (i = 0; i < CLOCK_APPS_CPU_TOTAL; i++)
  {
    /*
     * Get the clock ID.
     */
    eResult = Clock_GetClockId(pDrvCtxt, Clock_AppsCtxt.aCPU[i].pBSPConfig->szName, &nClockId);
    if (eResult != DAL_SUCCESS)
    {
      return DAL_ERROR_INTERNAL;
    }
    pClock = Clock_GetClockNode(pDrvCtxt, nClockId);

    /* Populate the aCpuClocks array data with the corresponding BSP values. */
    Clock_AppsCtxt.aCPU[i].pClock = &aCpuClocks[i];
    aCpuClocks[i].pDebugMux = pClock->pDebugMux;
    aCpuClocks[i].HALClock.nDebugMuxSel = pClock->HALClock.nDebugMuxSel;

    /* Default all CPU active configuration to the first BSP entry. */
    Clock_AppsCtxt.aCPU[i].pActiveConfig = &Clock_AppsCtxt.aCPU[i].pBSPConfig->pConfig->aPerfConfig[0];

    /*
     * Store the original min and max perf levels from BSP data. The BSP values can be updated
     * by subsequent calls to PepClockSetMinPerfLevel and PepClockSetMaxPerfLevel APIs.
     */
    Clock_AppsCtxt.aCPU[i].nMinPerfLevel = Clock_AppsCtxt.aCPU[i].pBSPConfig->pConfig->nMinPerfLevel;
    Clock_AppsCtxt.aCPU[i].nMaxPerfLevel = Clock_AppsCtxt.aCPU[i].pBSPConfig->pConfig->nMaxPerfLevel;

    /* Initialize the CPU ActiveMuxConfig structure. */
    memset(&Clock_AppsCtxt.aCPU[i].ActiveMuxConfig, 0, sizeof(ClockMuxConfigType));
    Clock_AppsCtxt.aCPU[i].pClock->pDomain->pActiveMuxConfig = &Clock_AppsCtxt.aCPU[i].ActiveMuxConfig;
  }

  /*-----------------------------------------------------------------------*/
  /* Create callback events.                                               */
  /*-----------------------------------------------------------------------*/
#if 0
  npa_resource_available_cb("/clk/hmss_ahb",
    Clock_HMSSAHBNodeReadyCallback, pDrvCtxt);

  npa_resource_available_cb("/clk/mmnoc_ahb",
    Clock_MMNOCAHBNodeReadyCallback, pDrvCtxt);
#endif
  /*-----------------------------------------------------------------------*/
  /* Initialize the DCVS module.                                           */
  /*-----------------------------------------------------------------------*/

  Clock_InitDCVS(pDrvCtxt);

  /*-----------------------------------------------------------------------*/
  /* Request highest available NOMINAL_L1 frequency.                          */
  /*-----------------------------------------------------------------------*/


  CLOCK_GLOBAL_FLAG_SET(CPU_BYPASS_RANGE_CHECKING);
   for (i = CLOCK_APPS_CPU_C0; i < CLOCK_APPS_CPU_CLUSTER_0_TOTAL; i++)
  {
    if (Clock_SetCpuPerfLevel(i, SILVER_CLUSTER_INITIAL_PERF_LEVEL, &nFrequencyHz) != DAL_SUCCESS)
    {
      return DAL_ERROR_INTERNAL;
    }
  }
  
#ifdef ENABLE_AUTO_BOOT_OPTIMIZATION
   for (i = CLOCK_APPS_CPU_C4; i < CLOCK_APPS_CPU_CLUSTER_1_TOTAL; i++)
  {
    if (Clock_SetCpuPerfLevel(i, GOLD_CLUSTER_INITIAL_PERF_LEVEL, &nFrequencyHz) != DAL_SUCCESS)
    {
      return DAL_ERROR_INTERNAL;
    }
  }
#endif

  if (Clock_SetCpuPerfLevel(CLOCK_APPS_CPU_L3, L3_CLUSTER_INITIAL_PERF_LEVEL, &nFrequencyHz) != DAL_SUCCESS)
    {
      return DAL_ERROR_INTERNAL;
    }

  Clock_LogCpuFreq();

  CLOCK_GLOBAL_FLAG_CLEAR(CPU_BYPASS_RANGE_CHECKING);


  Clock_MdssMdpRequest(pDrvCtxt, TRUE);

  /*-----------------------------------------------------------------------*/
  /* Register Boot and LPM callbacks.                                               */
  /*-----------------------------------------------------------------------*/
  Status = Clock_RegisterBootServerices(pDrvCtxt);

  /* Create RPMh client handler */
  Clock_RegisterRPMHClient();
  /*-----------------------------------------------------------------------*/
  /* Update miscellaneous GPU registers.                                   */
  /*-----------------------------------------------------------------------*/

  HAL_clk_InitMiscGfxRegisters();

  /*-----------------------------------------------------------------------*/
  /* Run the BIST if enabled.                                              */
  /*-----------------------------------------------------------------------*/

  if (pDrvCtxt->nBISTLogSize == 0)
  {
    pDrvCtxt->nBISTLogSize = CLOCK_BIST_DEFAULT_LOG_SIZE;
  }

  if (pDrvCtxt->bBISTEnabled)
  {
    Clock_BIST (pDrvCtxt);
  }

  /*-----------------------------------------------------------------------*/
  /* Good to go.                                                           */
  /*-----------------------------------------------------------------------*/

  return DAL_SUCCESS;

} /* END Clock_InitTarget */


/* =========================================================================
**  Function : Clock_DeInitTarget
** =========================================================================*/
/*
  See ClockDriver.h
*/

DALResult Clock_DeInitTarget
(
  ClockDrvCtxt *pDrvCtxt
)
{
  /*-----------------------------------------------------------------------*/
  /* Clear out driver context data.                                        */
  /*-----------------------------------------------------------------------*/

  memset((void *)pDrvCtxt->pImageCtxt, 0x0, sizeof(ClockAppsCtxtType));

  return DAL_SUCCESS;

} /* END Clock_DeInitTarget */


/* =========================================================================
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
  /* Normalize the Chip ID */
  switch (HALClkCtxt->nChipId)
  {
    case CHIPINFO_ID_SDM1000:
    default:
      HALClkCtxt->nChipId = CHIPINFO_ID_SDM1000;
      break;
  }
}


/* =========================================================================
**  Function : Clock_GetCpuDomains
** =========================================================================*/

ClockDomainNodeType **Clock_GetCpuDomains(void)
{
  return &aCpuDomains[0];
}

