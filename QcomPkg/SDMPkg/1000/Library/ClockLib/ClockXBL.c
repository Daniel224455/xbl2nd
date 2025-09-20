/** @file ClockXBL.c

  This file implements specialized image specific functions such
  as init image, enable DCVS, etc. In case of UEFI, we have few
  place holders for now.

  Copyright (c) 2010-2018, Qualcomm Technologies, Inc.
                   All rights reserved.
                 QUALCOMM Proprietary/GTDR

**/

/*=============================================================================
                              EDIT HISTORY


 when       who     what, where, why
 --------   ---     -----------------------------------------------------------
 05/30/14   sj      Fixed 64 bit compilation errors
 05/08/14   sr      Ported to 8994.
 08/09/12   sr      Ported to 8974.
 12/23/10   vs      Created.

=============================================================================*/


/*=========================================================================
      Include Files
==========================================================================*/

#include "DDIClock.h"
#include "ClockDriver.h"
#include "ClockApps.h"
#include "HALclkHWIO.h"
#include <Protocol/EFIPlatformInfoTypes.h>
#include <api/systemdrivers/icbarb.h>
#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Protocol/EFIPlatformInfo.h>

#include <Pi/PiStatusCode.h>
#include <Protocol/StatusCode.h>
#include <Protocol/ReportStatusCodeHandler.h>
#include <Library/BaseMemoryLib.h>
#include "Lpm.h"

#include "rpmh_client.h"
#include "cmd_db.h"
#include "api/systemdrivers/icbarb.h"

/*=========================================================================
      Macros
==========================================================================*/


/*=========================================================================
      Externals
==========================================================================*/

/*=========================================================================
      Prototypes
==========================================================================*/

/*=========================================================================
      Type Definitions
==========================================================================*/

/**
 * UEFI Clock Driver local context.
 */
typedef struct
{
  boolean        bLowPowerMode;
  boolean        bClusterGoldInReset;
  boolean        bRequestMDPBandwidth;
  uint32         nSilverPerfLevel;
  uint32         nMDSSMDPClockFreqHz;
  ClockNodeType *pMDSSMDPClock;
} ClockLocalCtxtType;

/*=========================================================================
      Data
==========================================================================*/

EFI_EVENT EfiExitBootServicesEvent      = (EFI_EVENT)NULL;

/*
 * Defined in DalClockFwk.c
 */
extern DALREG_DriverInfo DALClock_DriverInfo;

/*
 * gDALModDriverInfoList needs to be created for WM and UEFI - dynamic
 * loading of DAL drivers. This will not work for AMSS image (static loading
 * via DAL based XML files at compile time) so the following can't go in
 * DalClockFwk.c which is common across images
 *
 */
static DALREG_DriverInfo* DALDriverInfoArr[1] = { &DALClock_DriverInfo};
DALREG_DriverInfoList gDALModDriverInfoList= {1, DALDriverInfoArr};
static npa_client_handle hArb = NULL;
static npa_client_handle hLPM = NULL;

extern EFI_GUID gEfiRscHandlerProtocolGuid;

EFI_STATUS Clock_RegisterLPMCallBacks (BOOLEAN bIsRegisterLpm);
EFI_STATUS EFIAPI Clock_LowPowerModeHandler (
    IN EFI_STATUS_CODE_TYPE ,
    IN EFI_STATUS_CODE_VALUE,
    IN UINT32,
    IN EFI_GUID *,
    IN EFI_STATUS_CODE_DATA *
    );
EFI_STATUS Clock_RegisterBootServerices (ClockDrvCtxt *pDrvCtxt);
VOID EFIAPI Clock_ExitBootServicesEvent( IN EFI_EVENT  Event, IN VOID *Context);

/*-----------------------------------------------------------------------*/
/* Remote node definitions.                                              */
/*-----------------------------------------------------------------------*/
ICBArb_MasterSlaveType aMasterSlave[] =
{
  { ICBID_MASTER_MDP0, ICBID_SLAVE_EBI1 } ,    /* mdp + dsi */
  { ICBID_MASTER_MDP0, ICBID_SLAVE_EBI1 } ,    /* rotator */
};

ICBArb_RequestType aRequest[] =
{
  { ICBARB_REQUEST_TYPE_3 , { {500000000, 500000000, } } },
  { ICBARB_REQUEST_TYPE_3 , { {500000000, 500000000, } } },
};

ICBArb_MasterSlaveType aMasterSlaveLPM[] =
{
  { ICBID_MASTER_APPSS_PROC , ICBID_SLAVE_CLK_CTL } ,
  { ICBID_MASTER_UFS_CARD   , ICBID_SLAVE_EBI1    } ,
  { ICBID_MASTER_NPU        , ICBID_SLAVE_LLCC    } ,
};

ICBArb_RequestType aRequestForLPM[] =
{
  { ICBARB_REQUEST_TYPE_3 , { {20000      , 0         } } },
  { ICBARB_REQUEST_TYPE_3 , { {894029703  , 0         } } },
  { ICBARB_REQUEST_TYPE_3 , { {2925950000 , 0         } } },
};

ClockLocalCtxtType  ClockLocalCtxt;
static  rpmh_client_handle  gRpmhClient;
static  uint32              gCXAddr;
static  uint32              gReqId;

#define   CLOCK_CLUSTER_LOW_PERF_LEVEL           0 // 300Mhz


#define MDSSMDP_LOW_POWER_FREQUENCY_HZ            300000 * 1000  //  300000, disp_cc_mdss_mdp_clk, lowSV min = 19.2
#define MDSSMDP_DEFAULT_FREQUENCY_HZ              460000 * 1000 // Nominal Frequency


/*=========================================================================
      Functions
==========================================================================*/
/* ---------------------------------------------------------------------- */
/**
** FUNCTION: Clock_MdssBusBandwidthRequest(BOOLEAN)
**
** DESCRIPTION:
**      Create ICBArb client handle and request AXI bus IB/AB bus bandwidth accordingly
**
** NOTE: this function only allow to call one time
** ---------------------------------------------------------------------- */
static void Clock_MdssBusBandwidthRequest(BOOLEAN bEnable)
{
  if (ClockLocalCtxt.bRequestMDPBandwidth)  return;
  ClockLocalCtxt.bRequestMDPBandwidth = TRUE;
  if (hArb == NULL)
  {
    hArb = npa_create_sync_client_ex( "/icb/arbiter",
                                      "CLOCK_DRIVER",
                                      NPA_CLIENT_VECTOR,
                                      sizeof(aMasterSlave),
                                      &aMasterSlave);
  }

  if (hArb == NULL)
  {
    DEBUG ((EFI_D_ERROR, "Clock_MdssBusBandwidthRequest : Error, hArb == NULL\n"));
  }
  else
  {
    if (TRUE == bEnable)
    {
      npa_issue_vector_request(hArb, sizeof(aRequest)/sizeof(npa_resource_state),
                       (npa_resource_state *)aRequest );
    }
    else
    {
      npa_complete_request(hArb);
    }
  }
}

/* ---------------------------------------------------------------------- */
/**
** FUNCTION: Clock_LPMBandwidthRequest(BOOLEAN)
**
** DESCRIPTION:
**      Create ICBArb client handle and requesting lower bus bandwidth for CDSPNOC, SNOC, CNOC.
**
** NOTE: this function only allow to call one time
** ---------------------------------------------------------------------- */
static void Clock_LPMBandwidthRequest(BOOLEAN bEnable)
{
  if (hLPM == NULL)
  {
    hLPM = npa_create_sync_client_ex( "/icb/arbiter",
                                      "CLOCKLPM",
                                      NPA_CLIENT_VECTOR,
                                      sizeof(aMasterSlaveLPM),
                                      &aMasterSlaveLPM);
  }

  if (hLPM == NULL)
  {
    DEBUG ((EFI_D_ERROR, "Clock_NOCBandwidthRequest : Error, hLPM == NULL\n"));
  }
  else
  {
    if (TRUE == bEnable)
    {
      npa_issue_vector_request(hLPM, sizeof(aRequestForLPM)/sizeof(npa_resource_state),
                       (npa_resource_state *)aRequestForLPM );
    }
    else
    {
      npa_complete_request(hLPM);
    }
  }
}

/* =========================================================================
**  Function : Clock_MdssMdpRequest
** =========================================================================*/
void Clock_MdssMdpRequest
(
  ClockDrvCtxt *pDrvCtxt,
  BOOLEAN       bEnable
)
{
  DALResult      eResult;
  ClockIdType    nClockId;
  ClockNodeType *pClock = NULL;

  /*-----------------------------------------------------------------------*/
  /* Get the MDSS MDP clock ID.                                            */
  /*-----------------------------------------------------------------------*/
  // gcc_disp_xo_clk
  eResult = Clock_GetClockId(pDrvCtxt,"gcc_disp_xo_clk",(ClockIdType *)&nClockId);
  if ( eResult == DAL_SUCCESS )
    pClock = Clock_GetClockNode(pDrvCtxt, nClockId);

  if ( pClock != NULL )
    eResult = Clock_EnableClock(pDrvCtxt, pClock);

  // disp_cc_xo_clk
  eResult = Clock_GetClockId(pDrvCtxt,"disp_cc_xo_clk",(ClockIdType *)&nClockId);
  if ( eResult == DAL_SUCCESS )
    pClock = Clock_GetClockNode(pDrvCtxt, nClockId);

  if ( pClock != NULL )
    eResult = Clock_EnableClock(pDrvCtxt, pClock);

  // gcc_disp_ahb_clk
  eResult = Clock_GetClockId(pDrvCtxt,"gcc_disp_ahb_clk",(ClockIdType *)&nClockId);
  if ( eResult == DAL_SUCCESS )
    pClock = Clock_GetClockNode(pDrvCtxt, nClockId);

  if ( pClock != NULL )
    eResult = Clock_EnableClock(pDrvCtxt, pClock);

  //disp_cc_mdss_mdp_clk
  if ( eResult == DAL_SUCCESS )
    eResult = Clock_GetClockId(pDrvCtxt, "disp_cc_mdss_mdp_clk", (ClockIdType *)&nClockId);

  if ( eResult != DAL_SUCCESS )
  {
     DEBUG ((EFI_D_ERROR, "Clock_MdssMdpRequest :  Fail\n"));
  }
  else
  {
    ClockLocalCtxt.bRequestMDPBandwidth = bEnable;
    ClockLocalCtxt.pMDSSMDPClock = Clock_GetClockNode(pDrvCtxt, nClockId);
  }
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
  DALResult eResult;
  uint32    nResultFreq;
  boolean   bIsEnable;
  uint32    perfLevel, pnFrequencyHz;

  DALCLOCK_LOCK(pDrvCtxt);

  /*-----------------------------------------------------------------------*/
  /* Do not allow nested entries of the Low Power Mode.                    */
  /*-----------------------------------------------------------------------*/

  if (ClockLocalCtxt.bLowPowerMode == TRUE)
  {
    DALCLOCK_FREE(pDrvCtxt);
    return DAL_ERROR;
  }

  /*-----------------------------------------------------------------------*/
  /* Request MMNOC Bandwidth Request                                       */
  /*-----------------------------------------------------------------------*/
  Clock_MdssBusBandwidthRequest(TRUE);
  
  eResult = Clock_IsClockEnabled(pDrvCtxt, ClockLocalCtxt.pMDSSMDPClock, &bIsEnable);
  if ( eResult != DAL_SUCCESS ) return DAL_ERROR;

  if (bIsEnable)
  {
    /* save the MDSS MDP  clock frequency before enter LPM */
    eResult = Clock_GetClockFrequency( pDrvCtxt,
                                       ClockLocalCtxt.pMDSSMDPClock,
                                       &ClockLocalCtxt.nMDSSMDPClockFreqHz);

    if (eResult != DAL_SUCCESS)
    {
      DEBUG ((EFI_D_ERROR, "Clock_GetClockFrequencyHz MDSS MDP : Fail\n"));
    }

    /*-----------------------------------------------------------------------*/
    /* Lower the MDSS MDP clock's frequency.                                 */
    /*-----------------------------------------------------------------------*/
    if (ClockLocalCtxt.nMDSSMDPClockFreqHz != MDSSMDP_LOW_POWER_FREQUENCY_HZ)
    {
      eResult = Clock_SetClockFrequency( pDrvCtxt,
                                         ClockLocalCtxt.pMDSSMDPClock,
                                         MDSSMDP_LOW_POWER_FREQUENCY_HZ,
                                         CLOCK_FREQUENCY_HZ_CLOSEST,
                                         &nResultFreq);
      if (eResult != DAL_SUCCESS)
      {
        DEBUG ((EFI_D_ERROR, "Clock_SetClockFrequencyHz MDSS MDP : Fail\n"));
      }
    }
  }
  else
  {
    ClockLocalCtxt.nMDSSMDPClockFreqHz = 0;
  }

  Clock_LPMBandwidthRequest(TRUE);
  
  /*-----------------------------------------------------------------------*/
  /* Reduce each core's frequency to the lowest possible.                  */
  /*-----------------------------------------------------------------------*/
  // Set Silver cluster CPU0 perf level
  ClockLocalCtxt.nSilverPerfLevel = 0;
  eResult = Clock_GetCpuPerfLevel( CLOCK_APPS_CPU_C0, &perfLevel);
  if (eResult == DAL_SUCCESS)
  {
    ClockLocalCtxt.nSilverPerfLevel = perfLevel;
    // Set Silver cluster 0  to lowest perf level (300Mhz)
    eResult = Clock_SetCpuPerfLevel(CLOCK_APPS_CPU_C0, CLOCK_CLUSTER_LOW_PERF_LEVEL, &pnFrequencyHz);
  }
  else
  {
    DEBUG ((EFI_D_ERROR, "Clock_SetCpuPerfLevel CLOCK_APPS_CPU_C0 : Fail\n"));
  }

  if (gRpmhClient)
  {
    gReqId = rpmh_issue_command(gRpmhClient, RPMH_SET_ACTIVE, 0, gCXAddr, 3);
  }

  ClockLocalCtxt.bLowPowerMode = TRUE;

  DALCLOCK_FREE(pDrvCtxt);

  return eResult;

} /* END Clock_EnterLowPowerMode */


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
  DALResult eResult = DAL_SUCCESS;
  uint32    pnFrequencyHz;

  DALCLOCK_LOCK(pDrvCtxt);

  /*-----------------------------------------------------------------------*/
  /* Make sure the Low Power Mode is active before exiting it.             */
  /*-----------------------------------------------------------------------*/

  if (ClockLocalCtxt.bLowPowerMode == FALSE)
  {
    DALCLOCK_FREE(pDrvCtxt);
    return DAL_ERROR;
  }

  if (gRpmhClient)
  {
    gReqId = rpmh_issue_command(gRpmhClient, RPMH_SET_ACTIVE, 0, gCXAddr, 5); 
  }
  /*-----------------------------------------------------------------------*/
  /* Restore frequencies for CPU clusters to the nominal level.            */
  /*-----------------------------------------------------------------------*/
  /* Silver Cluster */
  eResult = Clock_SetCpuPerfLevel(CLOCK_APPS_CPU_C0, ClockLocalCtxt.nSilverPerfLevel, &pnFrequencyHz); //CLOCK_SILVER_NOMINAL_PERF_LEVEL
  if (eResult != DAL_SUCCESS)
  {
    DEBUG ((EFI_D_ERROR, "Clock_ExitLowPowerMode: Set Silver Perf level Fail\n"));
  }

  /*-----------------------------------------------------------------------*/
  /* Restore the MDSS MDP clock's frequency.                               */
  /*-----------------------------------------------------------------------*/
  if ((eResult == DAL_SUCCESS) && (ClockLocalCtxt.nMDSSMDPClockFreqHz != 0))
  {
    eResult = Clock_SetClockFrequency( pDrvCtxt,
                                       ClockLocalCtxt.pMDSSMDPClock,
                                       ClockLocalCtxt.nMDSSMDPClockFreqHz,
                                       CLOCK_FREQUENCY_HZ_CLOSEST,
                                       &pnFrequencyHz);
    if (eResult != DAL_SUCCESS)
    {
      DEBUG ((EFI_D_ERROR, "Clock_ExitLowPowerMode: Set MDSS MDP frequency Fail\n"));
    }
  }

  ClockLocalCtxt.bLowPowerMode = FALSE;

  DALCLOCK_FREE(pDrvCtxt);

  return eResult;

} /* END Clock_ExitLowPowerMode */

/**
  LPM feature - Clock_LowPowerModeHandler

  @param[IN] CodeType        :EFI_PROGRESS_CODE
  @param[IN] Value           :PcdLpm refer QcomPkg.dec for the value details
  @param[IN] Instance        : 0
  @param[IN] *CallerId       : Caller Id who initiated this CB
  @param[IN] *Data OPTIONAL  : Extended data refer LPM_EVENT_EXT_DATA in lpm.h

  @return
  EFI_SUCCESS                : CB handled
  EFI_UNSUPPORTED            : CB not intended for this module
**/

EFI_STATUS EFIAPI Clock_LowPowerModeHandler
(
    IN EFI_STATUS_CODE_TYPE     CodeType,
    IN EFI_STATUS_CODE_VALUE    Value,
    IN UINT32                   Instance,
    IN EFI_GUID                 *CallerId,
    IN EFI_STATUS_CODE_DATA     *Data OPTIONAL
)
{
  DALResult           eResult = DAL_SUCCESS;
  ClockDrvCtxt       *pDrvCtxt;
  EFI_GUID            QcomProtocolId = gEfiClockProtocolGuid ;
  BOOLEAN             bMatch;
  EFI_STATUS          status = EFI_UNSUPPORTED;
  LPM_EVENT_EXT_DATA *LpmEventData = NULL;


  if (((CodeType & EFI_STATUS_CODE_TYPE_MASK) == EFI_PROGRESS_CODE) &&
      (Value == PcdGet32 (PcdLpm)) &&
      (Data != NULL))
  {
    /* Map LpmEventData */
    LpmEventData = (LPM_EVENT_EXT_DATA*)( Data + 1);

    bMatch = CompareGuid(&(LpmEventData->CalleeGuid), &QcomProtocolId);
    if(TRUE == bMatch)
    {
      pDrvCtxt = Clock_GetDrvCtxt();
      if(pDrvCtxt == NULL)
      {
        DEBUG(( EFI_D_ERROR, "Clock_LowPowerModeHandler: Clock_GetDrvCtxt=NULL \r\n"));
        return EFI_ABORTED;
      }

      //DEBUG(( EFI_D_WARN, "Clock_LowPowerModeHandler::  QcomProtocolId Called ID GUID  Match %d \r\n",CallerId));
      switch (ClockLocalCtxt.bLowPowerMode)
      {
        case FALSE:
          if(LPM_ENTRY == LpmEventData->LPMState )
          {
            eResult = Clock_EnterLowPowerMode(pDrvCtxt);
            status = (eResult==DAL_SUCCESS)? EFI_SUCCESS : EFI_ABORTED;
          }
          else
          {
            DEBUG(( EFI_D_WARN, "Clock_LowPowerModeHandler: state %d != LPM_ENTRY \r\n", LpmEventData->LPMState));
            status = EFI_SUCCESS;
          }
          break;
        case TRUE:
          if(LPM_EXIT == LpmEventData->LPMState )
          {
            eResult = Clock_ExitLowPowerMode(pDrvCtxt);
            status = (eResult==DAL_SUCCESS)? EFI_SUCCESS : EFI_ABORTED;
          }
          else
          {
            DEBUG(( EFI_D_WARN, "Clock_LowPowerModeHandler: state %d != LPM_EXIT \r\n", LpmEventData->LPMState));
            status = EFI_SUCCESS;
          }
          break;
        default:
          DEBUG(( EFI_D_WARN, "Clock_LowPowerModeHandler: wrong  LPM state %d \r\n", LpmEventData->LPMState));
          status =  EFI_UNSUPPORTED;
      }
    }
  }

  return status;
}

/**
  Register/Unregister LPM support to get LPM entry/exit status in CB

  @param[IN] bIsRegisterLpm  : TRUE - Register LPM handler
                               FALSE - UnRegister LPM handler
  @return
  EFI_SUCCESS                : CB registered .
  EFI_DEVICE_ERROR           : device reported an error.
  EFI_UNSUPPORTED            : not supported by this device.
  EFI_INVALID_PARAMETER      : Parameter passed is incorrect.
**/

EFI_STATUS
Clock_RegisterLPMCallBacks (BOOLEAN bIsRegisterLpm)
{
  EFI_STATUS                Status              = EFI_SUCCESS;
  EFI_RSC_HANDLER_PROTOCOL *mRscHandlerProtocol = NULL;

  Status = gBS->LocateProtocol (&gEfiRscHandlerProtocolGuid,
                                NULL,
                               (VOID **) &mRscHandlerProtocol);

  if((EFI_SUCCESS != Status) || (mRscHandlerProtocol == NULL))
  {
    DEBUG ((EFI_D_ERROR, "Clock_RegisterLPMCallBacks Fail %r\n",Status));
    return Status;
  }

  if(bIsRegisterLpm)
  {
    Status |= mRscHandlerProtocol->Register(Clock_LowPowerModeHandler, TPL_CALLBACK);
  }
  else
  {
    Status |= mRscHandlerProtocol->Unregister(Clock_LowPowerModeHandler);
  }
  return Status;
}

EFI_STATUS Clock_RegisterRPMHClient()
{
  gRpmhClient = rpmh_create_handle(RSC_DRV_HLOS, "UEFI_LPM_CX");

  if (gRpmhClient)
  {
    rpmh_client_register_completion_callback(gRpmhClient, NULL);
    gCXAddr = cmd_db_query_addr("cx.lvl");
    return EFI_SUCCESS;
  }
  else 
  {
    DEBUG ((EFI_D_ERROR, "Failed to create RPMH handle %r\n"));
    return EFI_INVALID_PARAMETER;
  }
}
/**
  Callback function when UEFI Exit, free resources.

  @params [IN] Event     : Event handle.
          [IN] Context   : Callback parameter context.

  @return None
**/

VOID EFIAPI Clock_ExitBootServicesEvent
(
IN EFI_EVENT  Event,
IN VOID *Context
)
{
  /* Un register LPM CB */
  if (EFI_SUCCESS != Clock_RegisterLPMCallBacks(FALSE))
  {
    DEBUG((EFI_D_WARN, "ClockDxe: Failed to Unregister LPM CB!\n"));
  }
}


/**
  Register/Unregister LPM support to get LPM entry/exit status in CB

  @param[IN] bIsRegisterLpm  : TRUE - Register LPM handler
                               FALSE - UnRegister LPM handler
  @return
  EFI_SUCCESS                : CB registered .
  EFI_DEVICE_ERROR           : device reported an error.
  EFI_UNSUPPORTED            : not supported by this device.
  EFI_INVALID_PARAMETER      : Parameter passed is incorrect.
**/
EFI_STATUS
Clock_RegisterBootServerices
(
ClockDrvCtxt *pDrvCtxt
)
{
  EFI_STATUS status;

  status = Clock_RegisterLPMCallBacks (TRUE);

  if (status == EFI_SUCCESS)
  {
    if (EFI_SUCCESS != gBS->CreateEvent(EVT_SIGNAL_EXIT_BOOT_SERVICES, TPL_CALLBACK, Clock_ExitBootServicesEvent, NULL, &EfiExitBootServicesEvent))
    {
      DEBUG((EFI_D_INFO, "ClockDxe: Failed to create EXIT_BOOT_SERVICES event!\n"));
      status = EFI_UNSUPPORTED;
    }
  }
  return status;
}
