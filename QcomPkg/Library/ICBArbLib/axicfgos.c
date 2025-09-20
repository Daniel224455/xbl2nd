/**
 * @file axicfgos.c
 * @note Internal source file for Bus Configuration. Implements platform independent functions
 * 
 *                REVISION  HISTORY
 *  
 * This section contains comments describing changes made to this file. Notice
 * that changes are listed in reverse chronological order.
 * 
 * 
 * when         who     what, where, why
 * ----------   ---     ---------------------------------------------------
 * 09/15/2017   sds     Add fetch for sleep ff mask
 * 06/26/2017   sds     Add Deinit
 * 05/03/2016   sds     Add support for power domains during configuration
 * 03/12/2014   sds     Change the hardware base address to a uint8_t*.
 * 12/05/2011   dj      Created file
 *  
 *  
 * Copyright (c) 2011-2017 by QUALCOMM Technologies, Incorporated.  All Rights Reserved.
 */ 

#include "DALSys.h"
#include "DALStdErr.h"
#include "DALDeviceId.h"
#include "DDIHWIO.h"
#include "axicfgos.h"
#include "string.h" 
#include "icb_soc_data.h" 
#include "DDIClock.h"



/* internal data structure */
struct AxiCfgOs_Info 
{
  char * DevStr;
  DALSYS_PROPERTY_HANDLE_DECLARE(hProp);
  DalDeviceHandle *hHWIO;
  DalDeviceHandle *hClock;

  DALSYSSyncHandle  hMutex;    /**< sync handle to mutex */
  DALSYSSyncObj  mutexSyncObj; /**< sync object for mutex */

  int32_t nActiveCnt;
};


DALResult 
AxiCfg_DeviceAttach(const char *, DALDEVICEID,DalDeviceHandle **);


/* internal private data */
static bool bInitialized=false;
struct AxiCfgOs_Info Info;

static DALDEVICEID DalAxiCfg_DeviceId[1] = { DALDEVICEID_AXICFG };

/*------------------------- public data ----------------------------------*/ 

const DALREG_DriverInfo 
DALAxiCfg_DriverInfo = { AxiCfg_DeviceAttach,
                         1,
                         DalAxiCfg_DeviceId
                       };


/*------------------------- public funs ----------------------------------*/ 
DALResult 
AxiCfg_DeviceAttach(const char * pDevSting, DALDEVICEID id ,DalDeviceHandle ** pHandle)
{
  //does nothing
  (void)pDevSting;
  (void)pHandle;
  return DAL_SUCCESS;
}

/**
  @copydoc AxiCfgOs_Init
 */   
bool AxiCfgOs_Init( void )
{
  if( !bInitialized )
  {
    
    Info.DevStr = "/icb/arb";
    Info.nActiveCnt = 0;
    
    /* Get default configuration handle from config data */
    if (DAL_SUCCESS != DALSYS_GetDALPropertyHandleStr( Info.DevStr, Info.hProp ))
    { 
      return bInitialized;
    }
     
    /*create sync object*/
    if( DAL_SUCCESS != DALSYS_SyncCreate( DALSYS_SYNC_ATTR_RESOURCE,
                                          &Info.hMutex,
                                          &Info.mutexSyncObj) )
    {
      return bInitialized;
    }

    /*attach hwio driver*/
    if( DAL_SUCCESS != DAL_DeviceAttach(DALDEVICEID_HWIO, &Info.hHWIO) )
    {
      return bInitialized;
    }

    /*attach clock driver*/
    if( DAL_SUCCESS != DAL_DeviceAttach(DALDEVICEID_CLOCK, &Info.hClock) )
    {
      return bInitialized;
    }

    bInitialized = true;

  }
  return bInitialized;
}

/**
  @copydoc AxiCfgOs_DeInit
 */   
void AxiCfgOs_Deinit( void )
{
  DALSYS_DestroyObject(&Info.mutexSyncObj);
  DAL_DeviceDetach(Info.hHWIO);
  DAL_DeviceDetach(Info.hClock);
  memset(&Info, 0, sizeof(Info));
  bInitialized = false;
}

/**
  @copydoc AxiCfgOs_MutexLock
 */   
void AxiCfgOs_MutexLock( void )
{
  DALSYS_SyncEnter( Info.hMutex );
}

/**
  @copydoc AxiCfgOs_MutexUnlock
 */   
void AxiCfgOs_MutexUnlock( void  )
{
  DALSYS_SyncLeave( Info.hMutex );
}

/**
  @copydoc AxiCfgOs_Mmap
 */   
uint8_t *AxiCfgOs_Mmap( const char * pBaseName, uint8_t *pBaseAddr, uint32_t uRegionSize )
    
{
  uint8_t * pBasePtr = NULL;
  DALResult eResult =  DalHWIO_MapRegion(Info.hHWIO, pBaseName , &pBasePtr ); 

  if( DAL_SUCCESS == eResult && NULL != pBasePtr) 
  {
    return pBasePtr;
  }
  return NULL; 
}


/**
  @copydoc AxiCfgOs_GetProperty 
 */   
void * AxiCfgOs_GetProperty( const char * pString )
{
  if( AxiCfgOs_Init() )
  {
    DALSYSPropertyVar PropVar;
    if ( DAL_SUCCESS == DALSYS_GetPropertyValue( Info.hProp, pString, 0, &PropVar) ) 
    {
      return (void*)PropVar.Val.pStruct;
    }
  }
  return NULL;
}

/**
  @copydoc AxiCfgOs_GetClockId
  */
bool AxiCfgOs_GetClockId( const char * pName, ClockIdType * puId )
{
  DALResult res; 

  res = DalClock_GetClockId(Info.hClock, pName, puId);

  return (res == DAL_SUCCESS);
}

/**
  @copydoc AxiCfgOs_EnableClock
  */
void AxiCfgOs_EnableClock( ClockIdType uId )
{
  DalClock_SetClockFrequency(Info.hClock, uId, 1, CLOCK_FREQUENCY_KHZ_AT_LEAST, NULL);
  DalClock_EnableClock(Info.hClock, uId);
}

/**
  @copydoc AxiCfgOs_DisableClock
  */
void AxiCfgOs_DisableClock( ClockIdType uId )
{
  DalClock_DisableClock(Info.hClock, uId);
}

/**
  @copydoc AxiCfgOs_GetPowerDomainId
  */
bool AxiCfgOs_GetPowerDomainId( const char * pName, ClockPowerDomainIdType * puId )
{
  DALResult res; 

  res = DalClock_GetPowerDomainId(Info.hClock, pName, puId);

  return (res == DAL_SUCCESS);
}

/**
  @copydoc AxiCfgOs_EnablePowerDomain
  */
void AxiCfgOs_EnablePowerDomain( ClockPowerDomainIdType uId )
{
  DalClock_EnablePowerDomain(Info.hClock, uId);
}

/**
  @copydoc AxiCfgOs_DisableClock
  */
void AxiCfgOs_DisablePowerDomain( ClockPowerDomainIdType uId )
{
  DalClock_DisablePowerDomain(Info.hClock, uId);
}

/** 
 * @copydoc AxiCfgOs_ApplyActive
 */
void AxiCfgOs_ApplyActive( void )
{
  Info.nActiveCnt += 1;
}

/** 
 * @copydoc AxiCfgOs_RemoveActive
 */
void AxiCfgOs_RemoveActive( void )
{
  Info.nActiveCnt -= 1;

  /* Prevent us from going negative. */
  if( Info.nActiveCnt <= 0 )
  {
    Info.nActiveCnt = 0;
  }
}

/**
 @brief Fetch wake set fire-and-forget mask

 @returns Mask of VCDs to issue as fire-and-forget in WAKE set
*/
uint32_t AxiCfgOs_GetWakeSetFFMask( void )
{
  return 0;
}

