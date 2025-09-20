/*
===========================================================================
*/
/**
  @file ClockFwk.c

  DAL framework implementation file for the clock device driver.
*/
/*
  ====================================================================

  Copyright (c) 2010,2018 Qualcomm Technologies, Inc.  All Rights Reserved.
  QUALCOMM Proprietary and Confidential.

  ====================================================================


  ====================================================================
*/

/*=========================================================================
      Include Files
==========================================================================*/

#include "DALFramework.h"
#include "DalDevice.h"
#include "DALDeviceId.h"
#include "DDIClock.h"
#include "ClockDriver.h"
#include "DDIHWIO.h"


/*=========================================================================
      Externs
==========================================================================*/

extern ClockBSPType ClockBSP;

/*=========================================================================
      Macro Definitions
==========================================================================*/

/*
 * Take a DAL driver handle and dereference into the driver context.
 */
#define HANDLE2CTXT(h) \
  &((ClockDALClientCtxt *)(h->pClientCtxt))->pClockDevCtxt->pClockDrvCtxt->DrvCtxt


/*=========================================================================
      Type Definitions
==========================================================================*/

/*
 * Driver, device and client contexts
 */
typedef struct ClockDALDrvCtxt    ClockDALDrvCtxt;
typedef struct ClockDALDevCtxt    ClockDALDevCtxt;
typedef struct ClockDALClientCtxt ClockDALClientCtxt;


/**
 * Private "Clock" Vtable
 */
typedef struct ClockDALVtbl ClockDALVtbl;
struct ClockDALVtbl
{
  DALResult (*Clock_DriverInit)(ClockDALDrvCtxt *);
  DALResult (*Clock_DriverDeInit)(ClockDALDrvCtxt *);
};


/**
 * Clock device context.
 */
struct ClockDALDevCtxt
{
  /*
   * Base Members - do not modify!
   */
  uint32           dwRefs;
  DALDEVICEID      DevId;
  uint32           dwDevCtxtRefIdx;
  ClockDALDrvCtxt *pClockDrvCtxt;
  DALSYS_PROPERTY_HANDLE_DECLARE(hProp);
  uint32           Reserved[16];
};


/**
 * Clock client context.
 */
struct ClockDALClientCtxt
{
  /*
   * Base members - do not modify!
   */
  uint32            dwRefs;
  uint32            dwAccessMode;
  void             *pPortCtxt;
  ClockDALDevCtxt  *pClockDevCtxt;
  DalClockHandle    DalClockHandle;
};



/**
 * Clock driver context.
 */
struct ClockDALDrvCtxt
{
  /*
   * Base members - do not modify!
   */
  ClockDALVtbl              ClockDALVtbl;
  uint32                    dwNumDev;
  uint32                    dwSizeDevCtxt;
  uint32                    bInit;
  uint32                    dwRefs;
  ClockDALDevCtxt           ClockDevCtxt[1];

  /*
   * Private driver context.
   */
  DalDeviceHandle          *phHWIO;
  ClockDrvCtxt              DrvCtxt;
};



/*=========================================================================
      Function Prototypes
==========================================================================*/

DALResult Clock_DeviceAttach(const char *, DALDEVICEID,DalDeviceHandle **);
static DALResult DALClock_DriverInit(ClockDALDrvCtxt *);
static DALResult DALClock_DriverDeInit(ClockDALDrvCtxt *);
ClockDrvCtxt *Clock_GetDrvCtxt(void);


/*=========================================================================
      Data Declarations
==========================================================================*/

static DALDEVICEID DALClock_DeviceId[1] =
{
  DALDEVICEID_CLOCK
};

const DALREG_DriverInfo DALClock_DriverInfo =
{
  Clock_DeviceAttach, 1, DALClock_DeviceId
};

static ClockDALDrvCtxt Clock_DriverCtxt =
{
  {
    DALClock_DriverInit,
    DALClock_DriverDeInit
    },
  1,
  sizeof(ClockDALDevCtxt)
};


/*=========================================================================
      Functions
==========================================================================*/


static uint32
Clock_AddRef(DalClockHandle *h)
{
   return 1;
}

static uint32
Clock_DeviceDetach(DalDeviceHandle* h)
{
   return 1;
}

static DALResult
Clock_DeviceInit(DalDeviceHandle *h)
{
   return DAL_SUCCESS;
}

static DALResult
Clock_DeviceDeInit(DalDeviceHandle *h)
{
   return DAL_SUCCESS;
}

static DALResult
Clock_DevicePowerEvent(DalDeviceHandle *h, DalPowerCmd PowerCmd,
                                DalPowerDomain PowerDomain )
{
   return DAL_SUCCESS;

}

static DALResult
Clock_DeviceOpen(DalDeviceHandle* h, uint32 mode)
{
   return DAL_SUCCESS;
}

static DALResult
Clock_DeviceClose(DalDeviceHandle* h)
{
   return DAL_SUCCESS;
}

static DALResult
Clock_DeviceInfo(DalDeviceHandle* h, DalDeviceInfo* info, uint32 infoSize)
{
   return DAL_SUCCESS;
}

static DALResult
Clock_DeviceSysRequest
(
  DalDeviceHandle *h,
  DalSysReq        ReqIdx,
  const void      *SrcBuf,
  uint32           SrcBufLen,
  void            *DestBuf,
  uint32           DestBufLen,
  uint32          *DestBufLenReq)
{
   return DAL_SUCCESS;
}

static DALResult DALClock_DriverInit
(
  ClockDALDrvCtxt *drvCtxt
)
{
  DAL_DeviceAttach(DALDEVICEID_HWIO, &Clock_DriverCtxt.phHWIO);
  return DAL_SUCCESS;
}

static DALResult DALClock_DriverDeInit(ClockDALDrvCtxt *drvCtxt)
{
  return DAL_SUCCESS;
}

static DALResult DALClock_GetClockId( DalDeviceHandle *h, const char *szClock, ClockIdType *pnId)
{
  return DAL_SUCCESS;
}

static DALResult DALClock_EnableClock( DalDeviceHandle *h, ClockIdType nClock)
{
   return DAL_SUCCESS;
}

static DALResult DALClock_DisableClock( DalDeviceHandle *h, ClockIdType nClock)
{
  return DAL_SUCCESS;
}

static DALResult DALClock_SetClockFrequency( DalDeviceHandle *h, ClockIdType nClock, uint32 nFreq, ClockFrequencyType eMatch, uint32 *pnResultFreq )
{  
return DAL_SUCCESS;
}

static DALResult DALClock_SetClockDivider( DalDeviceHandle *h, ClockIdType nClock, uint32 nDivider)
{
   return DAL_SUCCESS;
}

static DALResult DALClock_InvertClock( DalDeviceHandle *h, ClockIdType nClock, boolean bInvert)
{
   return DAL_SUCCESS;
}

static DALResult DALClock_ResetClock( DalDeviceHandle *h, ClockIdType nClock, ClockResetType eReset)
{
   return DAL_SUCCESS;
}

static DALResult DALClock_ResetClock_STM( DalDeviceHandle *h, ClockIdType nClock, ClockResetType eReset)
{
   return DAL_SUCCESS;
}

static DALResult DALClock_ResetClockBranch( DalDeviceHandle *h, ClockIdType nClock, ClockResetType eReset)
{  
  return DAL_SUCCESS;
}

static DALResult DALClock_IsClockEnabled( DalDeviceHandle *h, ClockIdType nClock, boolean *pbIsEnabled)
{
 
  return DAL_SUCCESS;
}

static DALResult DALClock_IsClockOn( DalDeviceHandle *h, ClockIdType nClock, boolean *pbIsOn)
{
  return DAL_SUCCESS;
}

static DALResult DALClock_GetClockFrequency( DalDeviceHandle *h, ClockIdType nClock, uint32 *pnFrequencyHz)
{
  return DAL_SUCCESS;
}

static DALResult DALClock_SelectExternalSource( DalDeviceHandle *h, ClockIdType nClock, uint32 nFreqHz, uint32 nSource, uint32 nDiv2x, uint32 nM, uint32 nN, uint32 n2D)
{
    return DAL_SUCCESS;
}

static DALResult DALClock_CalcClockFrequency( DalDeviceHandle *h, ClockIdType nClock, uint32 *pnFrequencyHz)
{
   return DAL_SUCCESS;
}

static DALResult DALClock_GetPowerDomainId( DalDeviceHandle *h, const char *szPowerDomain, ClockPowerDomainIdType *pnPowerDomainId)
{
    return DAL_SUCCESS;
}

static DALResult DALClock_EnablePowerDomain( DalDeviceHandle *h, ClockPowerDomainIdType nPowerDomainId)
{
    return DAL_SUCCESS;
}

static DALResult DALClock_DisablePowerDomain( DalDeviceHandle *h, ClockPowerDomainIdType nPowerDomainId)
{
    return DAL_SUCCESS;
}

static DALResult DALClock_LoadNV( DalDeviceHandle *h)
{
    return DAL_SUCCESS;
}

static DALResult DALClock_ProcessorSleep( DalDeviceHandle *h, ClockSleepModeType eMode, uint32 nFlags)
{
    return DAL_SUCCESS;
}

static DALResult DALClock_ProcessorRestore( DalDeviceHandle *h, ClockSleepModeType eMode, uint32 nFlags)
{
    return DAL_SUCCESS;
}

static DALResult DALClock_EnterLowPowerMode( DalDeviceHandle *h)
{
    return DAL_SUCCESS;
}

static DALResult DALClock_ExitLowPowerMode( DalDeviceHandle *h)
{
    return DAL_SUCCESS;
}

static DALResult DALClock_ConfigClock( DalDeviceHandle *h, ClockIdType nClock, ClockConfigType eConfig)
{
    return DAL_SUCCESS;
}

static DALResult DALClock_SetUARTBaudClock( DalDeviceHandle *h, ClockIdType nClock, uint32 nDI, uint32 nFI, uint32 *pnResultFreqHz)
{
  return DAL_ERROR_NOT_SUPPORTED;
}

static DALResult DALClock_GetDividerId( DalDeviceHandle *h, const char *szDivider, ClockDividerIdType *pnId)
{
    return DAL_SUCCESS;
}

static DALResult DALClock_SetDivider( DalDeviceHandle *h, ClockDividerIdType nDividerId, uint32 nDivider)
{
   return DAL_SUCCESS;
}

static DALResult DALClock_GetMaxPerfLevel(DalDeviceHandle *h, uint32 nCPU, uint32 *pnPerfLevel)
{
   return DAL_SUCCESS;
}

static DALResult DALClock_GetMinPerfLevel(DalDeviceHandle *h, uint32 nCPU, uint32 *pnPerfLevel)
{
   return DAL_SUCCESS;
}

static DALResult DALClock_GetCpuPerfLevelFrequency(DalDeviceHandle *h, uint32 nCPU, uint32 nPerfLevel, uint32 *pnFrequencyHz, uint32 *pnRequiredVoltage)
{
   return DAL_SUCCESS;
}

static DALResult DALClock_SetCpuPerfLevel(DalDeviceHandle *h, uint32 nCPU, uint32 nPerfLevel, uint32 *pnFrequencyHz)
{
    return DAL_SUCCESS;
}

static DALResult DALClock_GetCpuPerfLevel(DalDeviceHandle *h, uint32 nCPU, uint32 *pnPerfLevel)
{
    return DAL_SUCCESS;
}

static DALResult DALClock_LogClocksState(DalDeviceHandle *h, uint32 nLogMask)
{
   return DAL_SUCCESS;
}

static DALResult DALClock_DumpClocksState(DalDeviceHandle *h, uint32 nLogMask)
{
    return DAL_SUCCESS;
}


/* =========================================================================
**  Function : Clock_InitInterface
** =========================================================================*/
/**
  Initialize the client context for communicating to the DAL.

  This function initializes the given client context.  It is invoked
  internally when a client attaches to the dal.

  @param *pclientCtxt [in] -- Pointer to the DAL client context.

  @return
  None.

  @dependencies
  None.
*/

static void Clock_InitInterface
(
  ClockDALClientCtxt *pclientCtxt
)
{
   static const DalClock vtbl =
   {
      {
         Clock_DeviceAttach,
         Clock_DeviceDetach,
         Clock_DeviceInit,
         Clock_DeviceDeInit,
         Clock_DeviceOpen,
         Clock_DeviceClose,
         Clock_DeviceInfo,
         Clock_DevicePowerEvent,
         Clock_DeviceSysRequest
      },

      .GetClockId                             = DALClock_GetClockId,
      .EnableClock                            = DALClock_EnableClock,
      .DisableClock                           = DALClock_DisableClock,
      .SetClockFrequency                      = DALClock_SetClockFrequency,
      .SetClockDivider                        = DALClock_SetClockDivider,
      .InvertClock                            = DALClock_InvertClock,
      .ResetClock                             = DALClock_ResetClock,
      .ResetClock_STM                         = DALClock_ResetClock_STM,
      .IsClockEnabled                         = DALClock_IsClockEnabled,
      .IsClockOn                              = DALClock_IsClockOn,
      .GetClockFrequency                      = DALClock_GetClockFrequency,
      .SelectExternalSource                   = DALClock_SelectExternalSource,
      .GetPowerDomainId                       = DALClock_GetPowerDomainId,
      .EnablePowerDomain                      = DALClock_EnablePowerDomain,
      .DisablePowerDomain                     = DALClock_DisablePowerDomain,
      .LoadNV                                 = DALClock_LoadNV,
      .ProcessorSleep                         = DALClock_ProcessorSleep,
      .ProcessorRestore                       = DALClock_ProcessorRestore,
      .EnterLowPowerMode                      = DALClock_EnterLowPowerMode,
      .ExitLowPowerMode                       = DALClock_ExitLowPowerMode,
      .CalcClockFrequency                     = DALClock_CalcClockFrequency,
      .ConfigClock                            = DALClock_ConfigClock,
      .SetUARTBaudClock                       = DALClock_SetUARTBaudClock,
      .ResetClockBranch                       = DALClock_ResetClockBranch,
      .GetDividerId                           = DALClock_GetDividerId,
      .SetDivider                             = DALClock_SetDivider,
      .LogClocksState                         = DALClock_LogClocksState,
      .DumpClocksState                        = DALClock_DumpClocksState,
      .GetMaxPerfLevel                        = DALClock_GetMaxPerfLevel,
      .GetMinPerfLevel                        = DALClock_GetMinPerfLevel,
      .GetCpuPerfLevelFrequency               = DALClock_GetCpuPerfLevelFrequency,
      .SetCpuPerfLevel                        = DALClock_SetCpuPerfLevel,
      .GetCpuPerfLevel                        = DALClock_GetCpuPerfLevel,
    };

   pclientCtxt->DalClockHandle.dwDalHandleId = DALDEVICE_INTERFACE_HANDLE_ID;
   pclientCtxt->DalClockHandle.pVtbl  = &vtbl;
   pclientCtxt->DalClockHandle.pClientCtxt = pclientCtxt;

} /* END Clock_InitInterface */


/* =========================================================================
**  Function : Clock_DeviceAttach
** =========================================================================*/
/**
  Internal DAL framework function for attaching to the clock DAL.

  This function attaches to the clock DAL and initializes the given
  device handle.

  @param *pszArg[in] -- String passed by user during attach.
  @param DeviceId -- The device id, should be DALDEVICEID_CLOCK
  @param phDalDevice[out] -- DAL device handle to fill in.

  @return
  None.

  @dependencies
  None.
*/

DALResult
Clock_DeviceAttach
(
  const char       *pszArg,
  DALDEVICEID       DeviceId,
  DalDeviceHandle **phDalDevice
)
{
   DALResult                 nErr;
   static ClockDALClientCtxt ClientCtxt;

   ClockDALClientCtxt *pClientCtxt = &ClientCtxt;

   *phDalDevice = NULL;

   nErr = DALFW_AttachToDevice(DeviceId,(DALDrvCtxt *)&Clock_DriverCtxt,
                                        (DALClientCtxt *)pClientCtxt);
   if (DAL_SUCCESS == nErr)
   {
   Clock_InitInterface(pClientCtxt);
      Clock_AddRef(&(pClientCtxt->DalClockHandle));
      *phDalDevice = (DalDeviceHandle *)&(pClientCtxt->DalClockHandle);
   }
   return nErr;

} /* END Clock_DeviceAttach */


/* =========================================================================
**  Function : Clock_GetPropertyValue
** =========================================================================*/
/*
  See ClockDriver.h
*/

DALResult Clock_GetPropertyValue
(
  const char              *szName,
  ClockPropertyValueType  *pValue
)
{
  
  return DAL_SUCCESS;

} /* END Clock_GetPropertyValue */


/* =========================================================================
**  Function : Clock_MapHWIORegion
** =========================================================================*/
/*
  See ClockDriver.h
*/

DALResult Clock_MapHWIORegion
(
  uintnt  nPhysAddr,
  uint32  nSizeBytes,
  uintnt *pnVirtAddr
)
{
    return DAL_SUCCESS;
} /* END Clock_MapHWIORegion */


/*=========================================================================
**  Function : Clock_GetDrvCtxt
** =========================================================================*/
/**
  Returns driver context.

  This function is used to get the driver context when direct calls are made to
  internal debug functions.

  @return
  Pointer to clock driver context.

  @dependencies
  None.
*/

ClockDrvCtxt *Clock_GetDrvCtxt(void)
{
  return &Clock_DriverCtxt.DrvCtxt;
}

