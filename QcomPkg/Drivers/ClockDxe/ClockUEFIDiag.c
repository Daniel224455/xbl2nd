/*
===========================================================================
*/
/**
  @file ClockUEFIDiag.c

  UEFI BDS Menu and other diagnostic functions.
*/
/*
  ====================================================================

  Copyright (c) 2017 - 2018 Qualcomm Technologies, Inc.  All Rights Reserved.
  QUALCOMM Proprietary and Confidential.

  ==================================================================== */
/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.


when       who     what, where, why
--------   ---     ----------------------------------------------------------
01/08/18   op      Remove dumping clocks'state to file
03/14/17   op      Enable Clocks' state File Logging

===========================================================================*/

/*=========================================================================
      Include Files
==========================================================================*/

#include "ClockDriver.h"
#include "HALclk.h"
#include "HALclkInternal.h"
#include "DALSys.h"
#include "DDIChipInfo.h"

#include "QdssSWEvent.h"
#include "tracer_event_ids.h"
#include "DDIPlatformInfo.h"
#include "ClockFileLog.h"
#include <Library/BaseLib.h>
#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Protocol/EFIPlatformInfoTypes.h>
#include <Protocol/EFIPlatformInfo.h>


/*=========================================================================
      Definitions
==========================================================================*/

/*=========================================================================
      Forward references
==========================================================================*/

/*=========================================================================
      Functions
==========================================================================*/

DALResult Clock_LogStateToFile
(
  ClockDrvCtxt        *pDrvCtxt,
  ULogHandle           hULog,
  uint32               nFlags
)
{
  DALResult status;
  status = Clock_LogState(pDrvCtxt, hULog, nFlags);
  return status;
}

DALResult Clock_aClocksRequiredOn
(
  ClockDrvCtxt *pDrvCtxt
)
{
  DALResult               eResult;
  ClockIdType             nClockId;
  ClockPropertyValueType  PropVal;
  ClockNodeType          *pClock = NULL;
  uint32                  nIndex = 0;
  ClockNameListType      *aClocksRequiredOnList = NULL;
   
  eResult = Clock_GetPropertyValue("aClocksRequiredOnList", &PropVal);

  if (eResult == DAL_SUCCESS)
  {
    aClocksRequiredOnList = (ClockNameListType *)PropVal;
  }
  else
  {
    return DAL_ERROR;
  }	

  if (aClocksRequiredOnList == NULL)
  {
    return DAL_ERROR;
  }	
       
  for (nIndex = 0; aClocksRequiredOnList[nIndex].szName!= NULL; nIndex++)
  {
    eResult = Clock_GetClockId(pDrvCtxt,aClocksRequiredOnList[nIndex].szName,(ClockIdType *)&nClockId);
    if ( eResult == DAL_SUCCESS )
      pClock = Clock_GetClockNode(pDrvCtxt, nClockId);

    if ( pClock != NULL )
      eResult = Clock_EnableClock(pDrvCtxt, pClock);
  }
  return eResult;
}

DALResult Clock_LogStateToUart
(
  ClockDrvCtxt        *pDrvCtxt,
  uint32               nFlags
)
  {
  ClockBSPType                *pBSP = pDrvCtxt->pBSP;
  ClockNodeType               *pClock;
  ClockPowerDomainNodeType    *pPowerDomain;
  uint32                       i, nFreqHz, nFreqHzConfig;
  int32                        nIsOn, nIsEnabled, nReset;
  boolean                      bIsEnabled, bIsOn, bCalcSupported;
  const char                  *szSource;
  CHAR16                       sUnicodeName[50];
  HAL_clk_ClockDescType       *pmClockDesc;
  DALResult                    status;

  DEBUG(( EFI_D_ERROR, "CLOCK \t ON \t ENABLED \t RESET \t REF COUNT \t FREQ CONFIG \t FREQ CALC \t VOLTAGE,SOURCE \r\n"));

  /*------------------------------------------------------------------------*/
  /* Log clock state.                                                       */
  /*------------------------------------------------------------------------*/
  status = Clock_aClocksRequiredOn(pDrvCtxt);
  for (i = 0; i < pBSP->nNumClocks; i++)
  {
    pClock = &pBSP->aClocks[i];
    nIsEnabled = nIsOn = nReset = -1;
    nFreqHzConfig = 0;
    bCalcSupported = FALSE;

    if (CLOCK_FLAG_IS_SET(pClock, EXPLICIT_ACCESS_ONLY))
    {
      continue;
    }
    /*
     * Get enabled state.
     */
    pmClockDesc = &pClock->HALClock;
    if (pmClockDesc == NULL)
    {
      DEBUG(( EFI_D_ERROR, "Clock_LogStateToUart: pmClockDesc is NULL \n"));
      return DAL_ERROR;
    }

    if (pmClockDesc->pmControl == NULL)
    {
      continue;
    }

    if (pmClockDesc->pmControl->IsEnabled != NULL)
    {
      bIsEnabled = HAL_clk_IsClockEnabled(pmClockDesc);
      nIsEnabled = bIsEnabled ? 1 : 0;
    }

    /*
     * Get ON state.
     */
    if (pmClockDesc->pmControl->IsOn != NULL)
    {
      bIsOn = HAL_clk_IsClockOn(pmClockDesc);
      nIsOn = bIsOn ? 1 : 0;
    }

    /*
     * Get reset state.
     */
    if (pmClockDesc->pmControl->IsReset != NULL)
    {
      nReset = HAL_clk_IsClockReset(pmClockDesc);
    }

    /*
     * Get frequency.
     */
    if (pClock->pDomain->pActiveMuxConfig != NULL)
    {
      nFreqHzConfig = pClock->pDomain->pActiveMuxConfig->nFreqHz;
    }

    /*
     * Calculate frequency.
     */
    if (pmClockDesc->nDebugMuxSel != HAL_CLK_NO_DEBUG_MUX_SEL)
    {
      if (pClock->pDebugMux != NULL)
      {
        if((strncmp(pClock->pDebugMux->szName,"apcs",4)!= 0) && (strncmp(pClock->pDebugMux->szName,"apss",4)!= 0))
        {
          nFreqHz = 0;
          bCalcSupported = TRUE;
          Clock_CalcClockFrequency(pDrvCtxt, pClock, &nFreqHz);
        }
      }
    }

    /*
     * Get source.
     */
    if (pClock->pDomain->pSource)
    {
      szSource = pClock->pDomain->pSource->szName;
    }
    else
    {
      szSource = "";
    }

    /*
     * Log results.
     */
    AsciiStrToUnicodeStr(pClock->szName,sUnicodeName);
    if (bCalcSupported)
    {
      DEBUG(( EFI_D_ERROR, " %s,\t%d,\t%d,\t%d,\t%d,\t%u,\t%d,\t%d,",
         sUnicodeName, nIsOn, nIsEnabled, nReset, pClock->nReferenceCount,
        nFreqHzConfig, nFreqHz, pClock->pDomain->VRegRequest.eVRegLevel));

      AsciiStrToUnicodeStr(szSource,sUnicodeName);
      DEBUG(( EFI_D_ERROR,"\t %s \n",sUnicodeName));
    }
    else
    {
      DEBUG(( EFI_D_ERROR, " %s,\t%d,\t%d,\t%d,\t%d,\t%u,\t-1,\t%d,",
        sUnicodeName, nIsOn, nIsEnabled, nReset, pClock->nReferenceCount,
        nFreqHzConfig, pClock->pDomain->VRegRequest.eVRegLevel));

      AsciiStrToUnicodeStr(szSource,sUnicodeName);
      DEBUG(( EFI_D_ERROR,"\t %s \n",sUnicodeName));
    }
  }

  /*------------------------------------------------------------------------*/
  /* Log power domain state.                                                */
  /*------------------------------------------------------------------------*/

  DEBUG(( EFI_D_ERROR, "\n"));
  DEBUG(( EFI_D_ERROR, "\n"));
  DEBUG(( EFI_D_ERROR, " POWER DOMAIN \t ON \t ENABLED \t REF COUNT \r\n"));

  for (i = 0; i < pBSP->nNumPowerDomains; i++)
  {
    pPowerDomain = &pBSP->aPowerDomains[i];
    if (CLOCK_FLAG_IS_SET(pPowerDomain, EXPLICIT_ACCESS_ONLY))
    {
      continue;
    }

    bIsEnabled = HAL_clk_IsPowerDomainEnabled(&pBSP->aPowerDomains[i].HALPowerDomain);
    bIsOn = HAL_clk_IsPowerDomainOn(&pBSP->aPowerDomains[i].HALPowerDomain);

    AsciiStrToUnicodeStr(pPowerDomain->szName,sUnicodeName);
    DEBUG(( EFI_D_ERROR, " %s,\t %d,\t %d,\t %d \n",sUnicodeName, bIsOn,bIsEnabled, pPowerDomain->nReferenceCount));
  }

  return DAL_SUCCESS;

} /* END Clock_LogStateUart */


DALResult Clock_ProcessLogClocksState
(
  ClockDrvCtxt       *pDrvCtxt,
  uint32              nLogMask,
  BDS_CLOCKLOG_STATE  eLogSetting
)
{
//  ULogHandle    ULhandle;
  DALResult     status;

  if (eLogSetting == BDSClockLogStateDisable)
  {
    DEBUG(( EFI_D_ERROR, "CLOCKS LOGGING IS DISABLE \n"));
    return DAL_SUCCESS;
  }

  if (pDrvCtxt == NULL)
  {
    return DAL_ERROR;
  }

  switch (eLogSetting)
  {
  case BDSClockLogStateToUart:
    DEBUG(( EFI_D_ERROR, "CLOCKS LOGGING to UART \n"));
    status = Clock_LogStateToUart(pDrvCtxt, nLogMask);
    break;
#if 0
  case BDSClockLogStateToFile:
    DEBUG(( EFI_D_ERROR, "CLOCKS LOGGING to FILE \n"));
    if(ClockFileLog_InitFileLog(&ULhandle)!= EFI_SUCCESS)
    {
      return DAL_ERROR;
    }
    status = Clock_LogState(pDrvCtxt, ULhandle, nLogMask);
    break;
#endif

  default:
      return DAL_ERROR;
  }
  return status;
}


/* =========================================================================
**  Function : Clock_LogClocksState
** =========================================================================*/
/*
  See DDIClock.h
*/
DALResult Clock_LogClocksState
(
  ClockDrvCtxt *pDrvCtxt,
  uint32        nLogMask
)
{
  BDS_CLOCKLOG_STATE  eLogSetting;
  DALResult           status;

  Clock_GetgClockLogSetting(&eLogSetting);
  status = Clock_ProcessLogClocksState(pDrvCtxt, nLogMask, eLogSetting);
  if (status == DAL_ERROR)
  {
    DEBUG(( EFI_D_ERROR, "Log Clocks State FAIL\n"));
  }
  return status;

}/* END Clock_LogClocksState */


/* =========================================================================
**  Function : Clock_DumpClocksState
** =========================================================================*/
/*
  See DDIClock.h
*/
DALResult Clock_DumpClocksState
(
  ClockDrvCtxt   *pDrvCtxt,
  uint32          nLogMask
)
{
  DALResult          status;
  BDS_CLOCKLOG_STATE eLogSetting;

  ClockFileLog_GetEBSClockLogSetting(&eLogSetting);
  status = Clock_ProcessLogClocksState(pDrvCtxt, nLogMask, eLogSetting);
  if (status == DAL_ERROR)
  {
    DEBUG(( EFI_D_ERROR, "ClockDxe:: Dump Clocks State FAIL\n"));
  }
  return status;
}

