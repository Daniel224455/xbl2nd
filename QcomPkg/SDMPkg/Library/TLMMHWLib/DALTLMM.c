/*===========================================================================

             M A I N   D A L   T L M M   D R I V E R   F I L E

DESCRIPTION

  Contains the majority of functionality for the DAL TLMM driver.  The
  API in this module MUST always remain target independent.


===========================================================================
             Copyright (c) 2010 - 2015 QUALCOMM Technologies Incorporated.
                    All Rights Reserved.
                  QUALCOMM Proprietary/GTDR
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.


when       who     what, where, why
--------   ---     ----------------------------------------------------------
08/27/17   dcf     Revamp for internal re-architecture of the GPIO driver.
03/09/09   dcf     Fix for immediateConfigs in restoring GPIOs from sleep.
02/23/09   an      Mainlining FEATURE_MANAGE_GPIOS_FOR_SLEEP and
                   FEATURE_APPS_GPIOS_LOW_POWER by having internal flags
                   in TLMM.
01/08/09   dcf     General clean up, removal of bare enumerated types and
                   addition of uniform headers and comments.
===========================================================================*/

/*==========================================================================

                     INCLUDE FILES FOR MODULE

==========================================================================*/

#include <DALStdDef.h>
#include "com_dtypes.h"
#include "DALTLMM.h"
#include "DDITlmm.h"
#include "GPIOTypes.h"
#include "GPIO.h"
#include "GPIODiag.h"

/*==========================================================================

                     LOCAL TYPE DEFINES FOR THIS MODULE

==========================================================================*/

GPIOClientHandleType GPIOHandle;
GPIOClientHandleType GPIOLPIHandle;
GPIOClientHandleType GPIOHandleToUse;

/*==========================================================================

                     LOCAL DATA FOR THIS MODULE

==========================================================================*/

uint32 bInitialized = 0x0;

typedef struct
{
  GPIOKeyType    nKey;
  uint32         nFunc;

}GPIOKeyFuncType;

#define GPIO_NUM_ENTRIES 20
GPIOKeyFuncType gGpioFuncs[GPIO_NUM_ENTRIES];
DALSYS_PROPERTY_HANDLE_DECLARE(hNonDalProps);
static boolean gNonDalTlmmInit = FALSE;

/*==========================================================================

      GENERIC DAL FUNCTIONS NOT CURRENTLY IMPLEMENTED IN THIS MODULE

==========================================================================*/

extern void TLMM_ClearContextData(uint32 nIdx);
extern boolean GPIO_CoreInit(void);
extern boolean GPIO_ExtInit(void);

DALResult
TLMM_DriverInit(TLMMDrvCtxt *pCtxt)
{
  return DAL_SUCCESS;
}

DALResult
TLMM_DriverDeInit(TLMMDrvCtxt *pCtxt)
{
  return DAL_SUCCESS;
}

DALResult
TLMM_PowerEvent(TLMMClientCtxt *pCtxt, DalPowerCmd PowerCmd,
                    DalPowerDomain PowerDomain)
{
    return DAL_SUCCESS;
}

DALResult
TLMM_Open(TLMMClientCtxt *pCtxt, uint32 dwaccessMode )
{
    return DAL_SUCCESS;
}

DALResult
TLMM_Close(TLMMClientCtxt *pCtxt)
{
    return DAL_SUCCESS;
}

DALResult
TLMM_Info(TLMMClientCtxt *pCtxt,DalDeviceInfo *pdeviceInfo, uint32 dwSize)
{
    return DAL_SUCCESS;
}

DALResult TLMM_PostInit( TLMMClientCtxt *pCtxt );

/*==========================================================================

                     APPLICATION PROGRAMMING INTERFACE

==========================================================================*/


uint32 TLMM_DriveCvtToHW
(
  uint32 nLogicalVal
)
{
  if (nLogicalVal <=200)  { return 0;}
  if (nLogicalVal <=400)  { return 1;}
  if (nLogicalVal <=600)  { return 2;}
  if (nLogicalVal <=800)  { return 3;}
  if (nLogicalVal <=1000) { return 4;}
  if (nLogicalVal <=1200) { return 5;}
  if (nLogicalVal <=1400) { return 6;}

  return 7;

}


uint32 TLMM_DriveCvtToLogical
(
  uint32 nHWVal
)
{
  if (nHWVal <= 7)
  {
    return ((nHWVal + 1) * 200);
  }
  return 1600;

} /* TLMM_DriveCvtToLogical */




/*==========================================================================

  FUNCTION      TLMM_DeviceInit

  DESCRIPTION   See DALTLMM.h

==========================================================================*/

DALResult TLMM_DeviceInit(TLMMClientCtxt *pCtxt)
{
  /*
   * Check for shared memory.  If already initialized, get a pointer and
   * we're done.
   */
  if( !bInitialized )
  {
    if ( FALSE == GPIO_CoreInit() )
    {
      return FALSE;
    }
    
    if ( FALSE == GPIO_ExtInit() )
    {
      return FALSE;
    }

    if ( GPIO_SUCCESS != GPIO_Attach(GPIO_DEVICE_TLMM, &GPIOHandle) )
    {
      return DAL_ERROR;
    }
    if ( GPIO_SUCCESS != GPIO_Attach(GPIO_DEVICE_SSC_LPI, &GPIOLPIHandle) )
    {
      /*
       * Since this is common code for poipu auto and poipu WP, do not treate SSC LPI attach is failure.
       * becouse SSC block descriptor is present only in Auto.
       */
      //return DAL_ERROR;
    }
    bInitialized = TRUE;
  }

  return(DAL_SUCCESS);

} /* TLMM_DeviceInit */


/*==========================================================================

  FUNCTION      TLMM_DeviceDeInit

  DESCRIPTION   See DALTLMM.h

==========================================================================*/

DALResult TLMM_DeviceDeInit
(
  TLMMClientCtxt *pCtxt
)
{
  return DAL_SUCCESS;

} /* TLMM_DeviceDeInit */


/*==========================================================================

  FUNCTION      TLMM_DetachCleanup

  DESCRIPTION   Cleans up client info from a client context after a final
                detach.

==========================================================================*/

DALResult TLMM_DetachCleanup
(
  TLMMClientCtxt *pCtxt
)
{
  #if 0
  uint32 nIdx;

  (void)DALSYS_SyncEnter(pCtxt->pDrvCtxt->hTlmmSync);

  for (nIdx=0; nIdx<gnTotalGPIO; nIdx++)
  {
    if(ganGpioIdUsers[nIdx].nClientId == pCtxt->ClientInfo.nClientId)
    {
      TLMM_ConfigGpioIdInactive(pCtxt, ganGpioIdUsers[nIdx].nGpioKey);
      TLMM_ReleaseGpioId(pCtxt, ganGpioIdUsers[nIdx].nGpioKey);
    }
  }

  TLMM_ClearContextData(pCtxt->ClientInfo.nIdx);

  (void)DALSYS_SyncLeave(pCtxt->pDrvCtxt->hTlmmSync);
  #endif
  return DAL_SUCCESS;
}


/*==========================================================================

  FUNCTION      TLMM_ConfigGpioGroup

  DESCRIPTION   See DALTLMM.h

==========================================================================*/

DALResult TLMM_ConfigGpioGroup
(
  TLMMClientCtxt*     pCtxt,
  DALGpioEnableType   eEnable,
  DALGpioSignalType*  eGpioGroup,
  uint32              nSize
)
{
  uint32 nIdx;

  if (eGpioGroup != NULL)
  {
    for (nIdx = 0; nIdx < nSize; nIdx++)
    {
      if (TLMM_ConfigGpio(pCtxt, eGpioGroup[nIdx], eEnable) != DAL_SUCCESS)
      {
        return DAL_ERROR;
      }
    }
    return DAL_SUCCESS;
  }
  return DAL_ERROR;

} /* TLMM_ConfigGpioGroup */


/*==========================================================================

  FUNCTION      TLMM_ConfigGpio

  DESCRIPTION   See DALTLMM.h

==========================================================================*/

DALResult TLMM_ConfigGpio
(
  TLMMClientCtxt*     pCtxt,
  DALGpioSignalType   eGpioConfig,
  DALGpioEnableType   eEnable
)
{
  GPIOKeyType GPIOKey;
  GPIOConfigType Config = {0};

  switch (DAL_GPIO_DEVICE(eGpioConfig))
  {
    case GPIO_DEVICE_SSC_LPI:
      GPIOHandleToUse = GPIOLPIHandle;
      break;
    default:
      GPIOHandleToUse = GPIOHandle;
      break;
  }
  if (GPIO_SUCCESS == GPIO_RegisterPinExplicit(GPIOHandleToUse, DAL_GPIO_NUMBER(eGpioConfig), GPIO_ACCESS_SHARED, &GPIOKey))
  {
    Config.func = DAL_GPIO_FUNCTION(eGpioConfig);
    Config.dir = DAL_GPIO_DIRECTION(eGpioConfig);
    Config.pull = DAL_GPIO_PULL(eGpioConfig);
    Config.drive = TLMM_DriveCvtToLogical(DAL_GPIO_DRIVE(eGpioConfig));
    
    if ( GPIO_SUCCESS == GPIO_ConfigPin(GPIOHandleToUse, GPIOKey, Config) )
    {
      return DAL_SUCCESS;
    }
  }
  return DAL_ERROR;

} /* TLMM_ConfigGpio */


/*==========================================================================

  FUNCTION      TLMM_SetPort

  DESCRIPTION   See DALTLMM.h

==========================================================================*/

DALResult  TLMM_SetPort
(
  TLMMClientCtxt*  pCtxt,
  DALGpioPortType  ePort,
  uint32           nValue
)
{
  //DALResult eResult = DAL_ERROR;
  #if 0
  if( (ePort < DAL_TLMM_NUM_PORTS) && (gtHALPorts[ePort].ePort != DAL_TLMM_NUM_PORTS) )
  {
    (void)DALSYS_SyncEnter(pCtxt->pDrvCtxt->hTlmmSync);
    
    if( nValue != 0 )
    {
      HAL_tlmm_SetPort(gtHALPorts[ePort].nOffset,
                       gtHALPorts[ePort].nMask,
                       gtHALPorts[ePort].nEnableValue);
    }
    else
    {
      HAL_tlmm_SetPort(gtHALPorts[ePort].nOffset,
                       gtHALPorts[ePort].nMask,
                       gtHALPorts[ePort].nDisableValue);
    }
    eResult = DAL_SUCCESS;

    (void)DALSYS_SyncLeave(pCtxt->pDrvCtxt->hTlmmSync);
  }
  #endif
  //return(eResult);
  return (DAL_SUCCESS);

} /* TLMM_SetPort */


/*==========================================================================

  FUNCTION      TLMM_GetGpioNumber

  DESCRIPTION   See DALTLMM.h

==========================================================================*/

DALResult  TLMM_GetGpioNumber
(
  TLMMClientCtxt*    pCtxt,
  DALGpioSignalType  eGpioConfig,
  uint32*            nGpioNumber
)
{
  *nGpioNumber = (uint32)DAL_GPIO_NUMBER(eGpioConfig);
  return(DAL_SUCCESS);

} /* TLMM_GetGpioNumber */


/*==========================================================================

  FUNCTION      TLMM_GpioIn

  DESCRIPTION   See DALTLMM.h

==========================================================================*/

DALResult  TLMM_GpioIn
(
  TLMMClientCtxt*    pCtxt,
  DALGpioSignalType  eGpioConfig,
  DALGpioValueType*  eValue
)
{
  GPIOKeyType GPIOKey;
  GPIOValueType PinValue = GPIO_LOW;

  switch (DAL_GPIO_DEVICE(eGpioConfig))
  {
    case GPIO_DEVICE_SSC_LPI:
      GPIOHandleToUse = GPIOLPIHandle;
      break;
    default:
      GPIOHandleToUse = GPIOHandle;
      break;
  }
  if (GPIO_SUCCESS == GPIO_RegisterPinExplicit(GPIOHandleToUse, DAL_GPIO_NUMBER(eGpioConfig), GPIO_ACCESS_SHARED, &GPIOKey))
  {
    if ( GPIO_SUCCESS == GPIO_ReadPin(GPIOHandleToUse, GPIOKey, &PinValue) )
    {
      if (PinValue == GPIO_HIGH)
      {
        *eValue = DAL_GPIO_HIGH_VALUE;        
      }
      else
      {
        *eValue = DAL_GPIO_LOW_VALUE;
      }
      
      return DAL_SUCCESS;
    }
  }
  return DAL_ERROR;

} /* TLMM_GpioIn */


/*==========================================================================

  FUNCTION      TLMM_GpioOutGroup

  DESCRIPTION   See DALTLMM.h

==========================================================================*/

DALResult  TLMM_GpioOutGroup
(
  TLMMClientCtxt*    pCtxt,
  DALGpioSignalType* aeGpioConfigGroup,
  uint32             nSize,
  DALGpioValueType   eValue
)
{
  uint32 nIdx;

  if ( NULL != aeGpioConfigGroup )
  {
    for (nIdx = 0; nIdx < nSize; nIdx++)
    {
      if (DAL_SUCCESS != TLMM_GpioOut(pCtxt,
                                      aeGpioConfigGroup[nIdx],
                                      eValue))
      {
        return DAL_ERROR;
      }
    }
    return DAL_SUCCESS;
  }
  return DAL_ERROR;

} /* TLMM_GpioOutGroup */


/*==========================================================================

  FUNCTION      TLMM_GpioOut

  DESCRIPTION   See DALTLMM.h

==========================================================================*/

DALResult  TLMM_GpioOut
(
  TLMMClientCtxt*   pCtxt,
 DALGpioSignalType  eGpioConfig,
 DALGpioValueType   eValue
)
{
  GPIOKeyType GPIOKey;
  GPIOValueType ePinValue = GPIO_LOW;
  switch (DAL_GPIO_DEVICE(eGpioConfig))
  {
    case GPIO_DEVICE_SSC_LPI:
      GPIOHandleToUse = GPIOLPIHandle;
      break;
    default:
      GPIOHandleToUse = GPIOHandle;
      break;
  }
  if (GPIO_SUCCESS == GPIO_RegisterPinExplicit(GPIOHandleToUse, DAL_GPIO_NUMBER(eGpioConfig), GPIO_ACCESS_SHARED, &GPIOKey))
  {
    if ( eValue == DAL_GPIO_HIGH_VALUE )
    {
      ePinValue = GPIO_HIGH;
    }
    if ( GPIO_SUCCESS == GPIO_WritePin(GPIOHandleToUse, GPIOKey, ePinValue) )
    {
      return DAL_SUCCESS;
    }
  }
  return DAL_ERROR;

} /* TLMM_GpioOut */


/*==========================================================================

  FUNCTION      TLMM_SetInactiveConfig

  DESCRIPTION   See DALTLMM.h

==========================================================================*/

DALResult TLMM_SetInactiveConfig
(
  TLMMClientCtxt*   pCtxt,
  uint32            nGpioNumber,
  DALGpioSignalType nInactiveConfig
)
{
  return(DAL_SUCCESS);

} /* TLMM_SetInactiveConfig */


/*==========================================================================

  FUNCTION      TLMM_GetOutput

  DESCRIPTION   See DALTLMM.h

==========================================================================*/

DALResult TLMM_GetOutput
(
  TLMMClientCtxt*   pCtxt,
  uint32            nGpioNumber,
  DALGpioValueType* nValue
)
{
  GPIOKeyType GPIOKey = 0;
  switch (DAL_GPIO_DEVICE(nGpioNumber))
  {
    case GPIO_DEVICE_SSC_LPI:
      GPIOHandleToUse = GPIOLPIHandle;
      break;
    default:
      GPIOHandleToUse = GPIOHandle;
      break;
  }
  if (NULL != nValue)
  {
    if (GPIO_SUCCESS == GPIO_RegisterPinExplicit(GPIOHandleToUse, nGpioNumber, GPIO_ACCESS_SHARED, &GPIOKey))
    {
      if(TRUE == GPIO_GetOutput(GPIOHandleToUse, GPIOKey))
      {
        *nValue = DAL_GPIO_HIGH_VALUE;
      }
      else
      {
         *nValue = DAL_GPIO_LOW_VALUE;
      }
      return(DAL_SUCCESS);
    }
  }
  return(DAL_ERROR);

} /* TLMM_GetOutput */




boolean Tlmm_Init
(
   void
)
{
  if ( gNonDalTlmmInit == FALSE )
  {
     DALSYS_GetDALPropertyHandle(DALDEVICEID_TLMM, hNonDalProps);

    if ( TRUE == GPIO_CoreInit() )
    {
      GPIO_ExtInit();
    }
    if ( GPIO_SUCCESS != GPIO_Attach(GPIO_DEVICE_TLMM, &GPIOHandle) )
    {
      return DAL_ERROR;
    }
    if ( GPIO_SUCCESS != GPIO_Attach(GPIO_DEVICE_SSC_LPI, &GPIOLPIHandle) )
    {
      /*
       * Since this is common code for poipu auto and poipu WP, do not treate SSC LPI attach is failure.
       * becouse SSC block descriptor is present only in Auto.
       */
      //return DAL_ERROR;
    }
    gNonDalTlmmInit = TRUE;   
  }
  return( TRUE );

}


DALResult Tlmm_ConfigGpioGroup
(
  DALGpioEnableType  eEnable,
  DALGpioSignalType* eGpioGroup, 
  uint32             nSize
)
{
  uint32 nIdx = 0;
  DALResult eResult = DAL_ERROR;
  GPIOKeyType GPIOKey;
  GPIOConfigType Config;
  GPIOValueType eValue = GPIO_LOW;

  if( eGpioGroup == NULL )
  {
    return(DAL_ERROR);
  }

  if( gNonDalTlmmInit == FALSE )
  {
    if(FALSE == Tlmm_Init())
    {
      return(DAL_ERROR);
    }
  }
  switch (DAL_GPIO_DEVICE(eGpioGroup[nIdx]))
  {
    case GPIO_DEVICE_SSC_LPI:
      GPIOHandleToUse = GPIOLPIHandle;
      break;
    default:
      GPIOHandleToUse = GPIOHandle;
      break;
  }
  if (GPIO_SUCCESS == GPIO_RegisterPinExplicit(GPIOHandleToUse, DAL_GPIO_NUMBER(eGpioGroup[nIdx]), GPIO_ACCESS_SHARED, &GPIOKey))
  {
  switch(eEnable)
  {
    case DAL_TLMM_GPIO_ENABLE:
      for( nIdx = 0; nIdx < nSize; nIdx++ )
      {
        if (GPIO_SUCCESS == GPIO_RegisterPinExplicit(GPIOHandleToUse, DAL_GPIO_NUMBER(eGpioGroup[nIdx]), GPIO_ACCESS_SHARED, &GPIOKey))
        {
          Config.func = DAL_GPIO_FUNCTION(eGpioGroup[nIdx]);
          Config.dir = DAL_GPIO_DIRECTION(eGpioGroup[nIdx]);
          Config.pull = DAL_GPIO_PULL(eGpioGroup[nIdx]);
          Config.drive = TLMM_DriveCvtToLogical(DAL_GPIO_DRIVE(eGpioGroup[nIdx]));

          if ( Config.dir == DAL_GPIO_OUTPUT )
          {
            if ( DAL_GPIO_HIGH_VALUE ==  (DALGpioValueType)DAL_GPIO_GET_OUTVAL(eGpioGroup[nIdx]) )
            {
              eValue = GPIO_HIGH;
            }
            if ( GPIO_SUCCESS != GPIO_WritePin(GPIOHandleToUse, GPIOKey, eValue) )
            {
              return DAL_ERROR;
            }            
          }
          if ( GPIO_SUCCESS != GPIO_ConfigPin(GPIOHandleToUse, GPIOKey, Config) )
          {
            return DAL_ERROR;
          }
        }
      }
      return DAL_SUCCESS;
      break;

    case DAL_TLMM_GPIO_DISABLE:
      for( nIdx = 0; nIdx < nSize; nIdx++ )
      {
        if (GPIO_SUCCESS == GPIO_RegisterPinExplicit(GPIOHandleToUse, DAL_GPIO_NUMBER(eGpioGroup[nIdx]), GPIO_ACCESS_SHARED, &GPIOKey))
        {
          if ( GPIO_SUCCESS != GPIO_ConfigPinInactive(GPIOHandleToUse, GPIOKey) )
          {
            return DAL_ERROR;
          }
        }
      }
      return DAL_SUCCESS;
      break;

    default:
      eResult = DAL_ERROR;
      break;
  }
  }
  return(eResult);

}
