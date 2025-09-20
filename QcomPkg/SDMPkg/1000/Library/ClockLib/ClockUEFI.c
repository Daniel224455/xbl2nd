/*
===========================================================================
*/
/**
  @file ClockUEFI.c

  This file contains the clock device driver UEFI specific code.
*/
/*
  ====================================================================

  Copyright (c) 2017 - 2018 QUALCOMM Technologies Incorporated.  All Rights Reserved.
  QUALCOMM Proprietary and Confidential.

  ====================================================================
  ====================================================================
*/


/*=========================================================================
      Include Files
==========================================================================*/

#include <string.h>
#include "ClockDriver.h"
#include "ClockApps.h"
#include "HALclkHWIO.h"

/*=========================================================================
      Macro Definitions
==========================================================================*/

/*=========================================================================
      Type Definitions
==========================================================================*/

/*=========================================================================
      External Declarations
==========================================================================*/

/*=========================================================================
      Function Prototypes
==========================================================================*/

extern DALResult Clock_SetCPUConfig
(
  ClockDrvCtxt                  *pDrvCtxt,
  ClockAppsCPUType               eCPU,
  ClockAppsPerfLevelConfigType  *pPerfConfig
);

void Clock_ReadOSMCurrentFrequency
(
  uint32 nCPU
);

void Clock_LogCpuFreq
(
void
);

/*=========================================================================
      Data Declarations
==========================================================================*/

/*=========================================================================
      Functions
==========================================================================*/

/* =========================================================================
**  Function : Clock_GetMaxPerfLevel
** =========================================================================*/
/*
  See ClockDriver.h
*/

DALResult
Clock_GetMaxPerfLevel (
  uint32 nCPU,
  uint32 *pnPerfLevel
)
{
  ClockDrvCtxt              *pDrvCtxt = Clock_GetDrvCtxt();
  ClockAppsCtxtType         *pAppsCtxt = (ClockAppsCtxtType *)pDrvCtxt->pImageCtxt;
  ClockAppsBSPCPUConfigType *pBSPConfig;

  if ((ClockAppsCPUType)nCPU >= CLOCK_APPS_CPU_TOTAL)
  {
    return DAL_ERROR_INVALID_PARAMETER;
  }

  pBSPConfig = pAppsCtxt->aCPU[nCPU].pBSPConfig->pConfig;

  if (pBSPConfig == NULL)
  {
    return DAL_ERROR_INTERNAL;
  }

  *pnPerfLevel = pBSPConfig->nMaxPerfLevel;

  return DAL_SUCCESS;
}


/* =========================================================================
**  Function : Clock_GetMinPerfLevel
** =========================================================================*/
/*
  See ClockDriver.h
*/

DALResult Clock_GetMinPerfLevel
(
  uint32 nCPU,
  uint32 *pnPerfLevel
)
{
  ClockDrvCtxt              *pDrvCtxt = Clock_GetDrvCtxt();
  ClockAppsCtxtType         *pAppsCtxt = (ClockAppsCtxtType *)pDrvCtxt->pImageCtxt;
  ClockAppsBSPCPUConfigType *pBSPConfig;

  if ((ClockAppsCPUType)nCPU >= CLOCK_APPS_CPU_TOTAL)
  {
    return DAL_ERROR_INVALID_PARAMETER;
  }

  pBSPConfig = pAppsCtxt->aCPU[nCPU].pBSPConfig->pConfig;

  if (pBSPConfig == NULL)
  {
    return DAL_ERROR_INTERNAL;
  }

  *pnPerfLevel = pBSPConfig->nMinPerfLevel;

  return DAL_SUCCESS;
}


/* =========================================================================
**  Function : Clock_GetCpuPerfLevelFrequency
** =========================================================================*/
/*
  See ClockDriver.h
*/

DALResult
Clock_GetCpuPerfLevelFrequency (
  uint32 nCPU,
  uint32 nPerfLevel,
  uint32 *pnFrequencyHz,
  uint32 *pnRequiredVoltage
)
{
  ClockDrvCtxt              *pDrvCtxt = Clock_GetDrvCtxt();
  ClockAppsCtxtType         *pAppsCtxt = (ClockAppsCtxtType *)pDrvCtxt->pImageCtxt;
  ClockAppsBSPCPUConfigType *pBSPConfig;
  uint32                     nCornerTableIndex;

  if ((ClockAppsCPUType)nCPU >= CLOCK_APPS_CPU_TOTAL)
  {
    return DAL_ERROR_INVALID_PARAMETER;
  }

  pBSPConfig = pAppsCtxt->aCPU[nCPU].pBSPConfig->pConfig;

  if (pBSPConfig == NULL)
  {
    return DAL_ERROR_INTERNAL;
  }

  if (nPerfLevel < pBSPConfig->nMinPerfLevel || nPerfLevel > pBSPConfig->nMaxPerfLevel)
  {
    return DAL_ERROR_OUT_OF_RANGE_PARAMETER;
  }

  if (nPerfLevel >= SIZEOF_ARRAY(pBSPConfig->anPerfLevel))
  {
    return DAL_ERROR_OUT_OF_RANGE_PARAMETER;
  }

  *pnFrequencyHz = Clock_MapPerfLevelToFrequency(pDrvCtxt, (ClockAppsCPUType)nCPU, nPerfLevel);

  nCornerTableIndex = pBSPConfig->aPerfConfig[pBSPConfig->anPerfLevel[nPerfLevel]].nVRegAPCCornerIndex;

  *pnRequiredVoltage = (Clock_MapVDDAPCCornerToVoltage(pDrvCtxt, (ClockAppsCPUType)nCPU, nCornerTableIndex) + 500) / 1000;

  return DAL_SUCCESS;
}


/* =========================================================================
**  Function : Clock_SetCpuPerfLevel
** =========================================================================*/
/*
  See ClockDriver.h
*/

DALResult
Clock_SetCpuPerfLevel (
  uint32 nCPU,
  uint32 nPerfLevel,
  uint32 *pnFrequencyHz
)
{
  DALResult                  eResult;
  ClockDrvCtxt              *pDrvCtxt = Clock_GetDrvCtxt();
  ClockAppsCtxtType         *pAppsCtxt = (ClockAppsCtxtType *)pDrvCtxt->pImageCtxt;
  ClockAppsBSPCPUConfigType *pBSPConfig;
  uint32                     nBoostPerfLevel = (uint32)-1;

  if (pnFrequencyHz == NULL)
  {
    return DAL_ERROR_INVALID_PARAMETER;
  }

  *pnFrequencyHz = 0;

  if ((ClockAppsCPUType)nCPU >= CLOCK_APPS_CPU_TOTAL)
  {
    return DAL_ERROR_INVALID_PARAMETER;
  }

  pBSPConfig = pAppsCtxt->aCPU[nCPU].pBSPConfig->pConfig;

  if (pBSPConfig == NULL)
  {
    return DAL_ERROR_INTERNAL;
  }

  /* Do not check DCVS perf level range, if set to bypass it. */
  if (!CLOCK_GLOBAL_FLAG_IS_SET(CPU_BYPASS_RANGE_CHECKING))
  {
    if (nPerfLevel < pBSPConfig->nMinPerfLevel || nPerfLevel > pBSPConfig->nMaxPerfLevel)
    {
      Clock_ReadAppsBSPConfig(CLOCK_CONFIG_PARAM_GET_BOOST_PERF_LEVEL_CLUSTER_0, &nBoostPerfLevel);
      if (nPerfLevel != nBoostPerfLevel)
      {
        return DAL_ERROR_OUT_OF_RANGE_PARAMETER;
      }
    }
  }
  /* Clamp the given perf level within the valid range. */
  else
  {
    if (nPerfLevel < pBSPConfig->nMinPerfLevel)
    {
      nPerfLevel = pBSPConfig->nMinPerfLevel;
    }
    if (nPerfLevel > pBSPConfig->nMaxPerfLevel)
    {
      nPerfLevel = pBSPConfig->nMaxPerfLevel;
    }
  }

  /* Do not check DCVS perf level range, if set to bypass it. */
  if (!CLOCK_GLOBAL_FLAG_IS_SET(CPU_BYPASS_RANGE_CHECKING))
  {
    if (nPerfLevel > pBSPConfig->nMaxPerfLevel && nPerfLevel != nBoostPerfLevel)
    {
      return DAL_ERROR_OUT_OF_RANGE_PARAMETER;
    }
  }

  if (nPerfLevel >= SIZEOF_ARRAY(pBSPConfig->anPerfLevel))
  {
    return DAL_ERROR_OUT_OF_RANGE_PARAMETER;
  }

  /* Check the state of DCVS and return immediately if DCVS is disabled. */
  if (pDrvCtxt->bDCVSEnabled == TRUE || CLOCK_GLOBAL_FLAG_IS_SET(CPU_TEST_IN_PROGRESS))
  {
    eResult = Clock_SetCPUConfig(
              pDrvCtxt,
              (ClockAppsCPUType)nCPU,
              &pBSPConfig->aPerfConfig[pBSPConfig->anPerfLevel[nPerfLevel]]);

    if (eResult != DAL_SUCCESS)
    {
      return eResult;
    }
  }

  *pnFrequencyHz = pBSPConfig->aPerfConfig[pBSPConfig->anPerfLevel[nPerfLevel]].nFreqHz;

  // for Debug only. Will coment out when BU done
  //Clock_ReadOSMCurrentFrequency(nCPU);

  return DAL_SUCCESS;
}


/* =========================================================================
**  Function : Clock_GetCpuPerfLevel
** =========================================================================*/
/*
  See ClockDriver.h
*/

DALResult
Clock_GetCpuPerfLevel (
  uint32 nCPU,
  uint32 *pnPerfLevel
)
{
  ClockDrvCtxt                 *pDrvCtxt = Clock_GetDrvCtxt();
  ClockAppsCtxtType            *pAppsCtxt = (ClockAppsCtxtType *)pDrvCtxt->pImageCtxt;
  ClockAppsBSPCPUConfigType    *pBSPConfig;
  ClockAppsPerfLevelConfigType *pPerfConfig;
  uint32                        nPerfLevel;

  if ((ClockAppsCPUType)nCPU >= CLOCK_APPS_CPU_TOTAL)
  {
    return DAL_ERROR_INVALID_PARAMETER;
  }

  pBSPConfig = pAppsCtxt->aCPU[nCPU].pBSPConfig->pConfig;

  if (pBSPConfig == NULL)
  {
    return DAL_ERROR_INTERNAL;
  }

  pPerfConfig = pAppsCtxt->aCPU[nCPU].pActiveConfig;

  if (pPerfConfig == NULL)
  {
    return DAL_ERROR_INTERNAL;
  }

  for (nPerfLevel = pBSPConfig->nMinPerfLevel;
       nPerfLevel < pBSPConfig->nMaxPerfLevel + 1;
       ++nPerfLevel)
  {
    if (&pBSPConfig->aPerfConfig[pBSPConfig->anPerfLevel[nPerfLevel]] == pPerfConfig)
    {
      *pnPerfLevel = nPerfLevel;
      // for Debug only
      //Clock_ReadOSMCurrentFrequency(nCPU);

      return DAL_SUCCESS;
    }
  }

  return DAL_ERROR_OUT_OF_RANGE_PARAMETER;
}

void Clock_ReadOSMCurrentFrequency
(
  uint32 nCPU
)
{
  boolean              bDebugCpuClocks;
  ClockAppsCPUType     eCPU;

  bDebugCpuClocks = 1;
  eCPU = (ClockAppsCPUType)nCPU;

  if (bDebugCpuClocks)
  {
    uint32              nFreqMhz,nLval ;
    volatile boolean    bValid;
    uint32              nCount = 200;
    uint32              nCluster;

    do{
      switch(eCPU)
      {
        case CLOCK_APPS_CPU_C0: //silver cluster
        case CLOCK_APPS_CPU_C1:
        case CLOCK_APPS_CPU_C2:
        case CLOCK_APPS_CPU_C3:
          bValid = ((inp32(HWIO_APSS_OSM_RAIL0_CLKDOM1_PSTATE_STATUS_ADDR) &
                    HWIO_APSS_OSM_RAIL0_CLKDOM1_PSTATE_STATUS_VALID_BMSK) >>
                    HWIO_APSS_OSM_RAIL0_CLKDOM1_PSTATE_STATUS_VALID_SHFT);
          break;

        case CLOCK_APPS_CPU_C4: //Gold cluster
        case CLOCK_APPS_CPU_C5:
        case CLOCK_APPS_CPU_C6:
        case CLOCK_APPS_CPU_C7:
          bValid = ((inp32(HWIO_APSS_OSM_RAIL1_CLKDOM0_PSTATE_STATUS_ADDR) &
                    HWIO_APSS_OSM_RAIL1_CLKDOM0_PSTATE_STATUS_VALID_BMSK) >>
                    HWIO_APSS_OSM_RAIL1_CLKDOM0_PSTATE_STATUS_VALID_SHFT);
          break;

        case CLOCK_APPS_CPU_L3: //L3 cluster
          bValid = ((inp32(HWIO_APSS_OSM_RAIL0_CLKDOM0_PSTATE_STATUS_ADDR) &
                    HWIO_APSS_OSM_RAIL0_CLKDOM0_PSTATE_STATUS_VALID_BMSK) >>
                    HWIO_APSS_OSM_RAIL0_CLKDOM0_PSTATE_STATUS_VALID_SHFT);
          break;

         default:
          DEBUG((EFI_D_WARN,">>> Error: Invalid CPU ID %d ",nCPU));
          return;
      }
    }while ((--nCount) && !bValid);


    switch(eCPU)
    {
      case CLOCK_APPS_CPU_C0: //silver cluster
      case CLOCK_APPS_CPU_C1:
      case CLOCK_APPS_CPU_C2:
      case CLOCK_APPS_CPU_C3:
        nCluster = CLOCK_APPS_CPU_CLUSTER_C0;
        nLval = ((inp32(HWIO_APSS_OSM_RAIL0_CLKDOM1_PSTATE_STATUS_ADDR) &
                  HWIO_APSS_OSM_RAIL0_CLKDOM1_PSTATE_STATUS_CURRENT_FREQUENCY_BMSK) >>
                  HWIO_APSS_OSM_RAIL0_CLKDOM1_PSTATE_STATUS_CURRENT_FREQUENCY_SHFT);
        break;

     case CLOCK_APPS_CPU_L3: //L3
        nCluster = CLOCK_APPS_CPU_CLUSTER_L3;
        nLval = ((inp32(HWIO_APSS_OSM_RAIL0_CLKDOM0_PSTATE_STATUS_ADDR)&
                  HWIO_APSS_OSM_RAIL0_CLKDOM0_PSTATE_STATUS_CURRENT_FREQUENCY_BMSK) >>
                  HWIO_APSS_OSM_RAIL0_CLKDOM0_PSTATE_STATUS_CURRENT_FREQUENCY_SHFT);
        break;

     case CLOCK_APPS_CPU_C4: //Gold cluster
     case CLOCK_APPS_CPU_C5:
     case CLOCK_APPS_CPU_C6:
     case CLOCK_APPS_CPU_C7:
        nCluster = CLOCK_APPS_CPU_CLUSTER_C1;
        nLval = ((inp32(HWIO_APSS_OSM_RAIL1_CLKDOM0_PSTATE_STATUS_ADDR)&
                  HWIO_APSS_OSM_RAIL1_CLKDOM0_PSTATE_STATUS_CURRENT_FREQUENCY_BMSK) >>
                  HWIO_APSS_OSM_RAIL1_CLKDOM0_PSTATE_STATUS_CURRENT_FREQUENCY_SHFT);
        break;

      default:
        DEBUG((EFI_D_WARN,">>> Error: Invalid CPU ID %d ",nCPU));
        return;
    }
    nFreqMhz = (nLval * 192000)/10000;
    DEBUG((EFI_D_WARN, ">>> Cluster %d: %8d MHz\n", nCluster, nFreqMhz));
  }
} /* END Clock_ReadOSMCurrentFrequency */

void Clock_LogCpuFreq
(
void
)
{
  Clock_ReadOSMCurrentFrequency(CLOCK_APPS_CPU_C0);
  Clock_ReadOSMCurrentFrequency(CLOCK_APPS_CPU_C4);
  Clock_ReadOSMCurrentFrequency(CLOCK_APPS_CPU_L3);
} /* END Clock_LogCpuFreq */

