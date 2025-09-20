/*===========================================================================

FILE:      GPIOExt.c

DESCRIPTION:
  This file provides the extension API functions to the GPIO core driver.
  It is meant to be included in environments that do not have excessive
  memory constraints.

PUBLIC CLASSES:
  GPIO_Init
  GPIO_Attach
  GPIO_RegisterPin
  GPIO_GetPinConfig
  GPIO_IsPinActive
  GPIO_ConfigPinInactive
  GPIO_GetOutput
  GPIO_GetInactiveConfig
  GPIO_SetInactiveConfig

===========================================================================
Copyright (c) 2017 QUALCOMM Technologies Incorporated.
All Rights Reserved.
QUALCOMM Proprietary/GTDR
========================================================================================================*/

/*========================================================================================================

  EDIT HISTORY FOR FILE

  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.


  when       who     what, where, why
  --------   ---     -------------------------------------------------------------------------------------
  03/02/17   dcf     Created.
  ======================================================================================================*/

/*=======================================================================================================

  INCLUDE FILES FOR MODULE

  =====================================================================================================*/

#include "GPIO.h"
#include "GPIODiag.h"
#include "GPIOTypes.h"
#include "GPIOExt.h"
#include "GPIOCore.h"
#include "HALtlmm.h"
#include "GPIOSvcDevice.h"
#include "GPIOSvcSync.h"
#include "GPIOIFace.h"


/*=======================================================================================================

                               INTERNAL MACROS AND DATA STRUCTURES

  =====================================================================================================*/

/*
 * Generic sanity checker on array sizes.
 */
#define TLMM_MAX_STRING_LENGTH 50


/*
 * GPIO block descriptor Array.  THis may be extracted from device config or compiled into
 * the library binary.  This extern definition usage is expected to change.
 */
extern HALgpioBlockDescType GPIOBlockDescriptors[];
extern uint32 gnNumBlockDescs;

/*
 * Static initialization flag.
 */
static boolean bGpioExtInitialized = FALSE;

extern GPIOPinInfoType ganGpioAccess[];


/*=======================================================================================================

                                   INTERNAL DATA STRUCTURES

  =====================================================================================================*/

/*=======================================================================================================

                            APPLICATION PROGRAMMERS INTERFACE FUNCTIONS

  =====================================================================================================*/


/*=======================================================================================================

  FUNCTION      GPIO_Init

  DESCRIPTION   See GPIO.h

  =====================================================================================================*/

boolean GPIO_ExtInit
(
  void
)
{
  if ( FALSE == bGpioExtInitialized )
  {
    /*
     * Initialize dependencies for this driver.
     */
    if ( FALSE == GPIO_SvcDeviceInit() )
    {
      /*
       * This is a fatal error condition.
       */
      return FALSE;
    }

    bGpioExtInitialized = TRUE;
  }
  return TRUE;

} /* GPIO_Init */



/*=======================================================================================================

  FUNCTION      GPIO_RegisterPin

  DESCRIPTION   See GPIO.h

  =====================================================================================================*/

GPIOResult GPIO_RegisterPin
(
  GPIOClientHandleType  Handle,
  const char           *pszSignal,
  GPIOAccessType        AccessType,
  GPIOConfigType       *pConfig,
  GPIOKeyType          *pGPIOKey
)
{
  GPIOIDType *pPinId = GPIO_SvcDeviceGetGpioId ( pszSignal );

  if ( pPinId == NULL )
  {
    return GPIO_ERROR;
  }
  else
  {
    pConfig->func = pPinId->func;
  }

  return GPIO_RegisterPinExplicit(Handle, pPinId->pin, AccessType, pGPIOKey);

} /* GPIO_RegisterPin */



/*=======================================================================================================

  FUNCTION      GPIO_ConfigPinInactive

  DESCRIPTION   See GPIO.h

  =====================================================================================================*/

GPIOResult GPIO_ConfigPinInactive
(
  GPIOClientHandleType Handle,
  GPIOKeyType          GPIOKey
)
{
  HALgpioBlockDescType *pDesc = NULL;
  uint32                nBaseOffs = 0;
  GPIOPinInfoType       PinInfo;
  GPIOValueType         eValue;
  GPIOConfigType        Config;
  GPIOResult            nResult = GPIO_ERROR;

  PinInfo.All = GPIOKey;

  /*
   * Synchronize access.
   */
  GPIO_SyncEnter();

  if ( ganGpioAccess[PinInfo.Info.PinIdx].All == GPIOKey )
  {
    pDesc = &GPIOBlockDescriptors[PinInfo.Info.DescIdx];

    /*
     * Get the offset from the base address and program the GPIO.
     */
    nBaseOffs = pDesc->aTiles[PinInfo.Info.TileIdx];

    HAL_tlmm_GetInactiveConfig(pDesc, nBaseOffs, PinInfo.Info.Pin, &Config, &eValue);
    if ( Config.dir == GPIO_OUT )
    {
      HAL_tlmm_WritePin(pDesc, nBaseOffs, PinInfo.Info.Pin, eValue);
    }
    HAL_tlmm_ConfigPin(pDesc, nBaseOffs, PinInfo.Info.Pin, Config);
    nResult = GPIO_SUCCESS;
  }

  GPIO_SyncExit();

  return nResult;

} /* END GPIO_ConfigPin */



/*=======================================================================================================

  FUNCTION      GPIO_IsPinActive

  DESCRIPTION   See GPIO.h

  =====================================================================================================*/

boolean GPIO_IsPinActive
(
  GPIOClientHandleType Handle,
  GPIOKeyType          GPIOKey
)
{
  HALgpioBlockDescType* pDesc = NULL;
  GPIOPinInfoType PinInfo;
  GPIOConfigType Current, Inactive;
  uint32 nBaseOffs = 0;
  GPIOValueType eValue;

  PinInfo.All = GPIOKey;

  if ( ganGpioAccess[PinInfo.Info.PinIdx].All == PinInfo.All )
  {
    pDesc = &GPIOBlockDescriptors[PinInfo.Info.DescIdx];
    nBaseOffs = pDesc->aTiles[PinInfo.Info.TileIdx];

    HAL_tlmm_GetInactiveConfig(pDesc, nBaseOffs, PinInfo.Info.Pin, &Inactive, &eValue);
    HAL_tlmm_GetPinConfig(pDesc, nBaseOffs, PinInfo.Info.Pin, &Current);

    if ( (Current.func   == Inactive.func)  &&
         (Current.dir    == Inactive.dir)   &&
         (Current.pull   == Inactive.pull)  &&
         (Current.drive  == Inactive.drive) )
    {
      return FALSE;
    }
  }
  return TRUE;

} /* END GPIO_IsPinActive */



/*=======================================================================================================

  FUNCTION      GPIO_GetPinConfig

  DESCRIPTION   See GPIO.h

  =====================================================================================================*/

GPIOResult GPIO_GetPinConfig
(
  GPIOClientHandleType  Handle,
  GPIOKeyType           GPIOKey,
  GPIOConfigType       *pCfg
)
{
  HALgpioBlockDescType* pDesc = NULL;
  GPIOPinInfoType PinInfo;
  uint32 nBaseOffs = 0;

  PinInfo.All = GPIOKey;

  if ( ganGpioAccess[PinInfo.Info.PinIdx].All == PinInfo.All )
  {
    pDesc = &GPIOBlockDescriptors[PinInfo.Info.DescIdx];
    nBaseOffs = pDesc->aTiles[PinInfo.Info.TileIdx];
    HAL_tlmm_GetPinConfig(pDesc, nBaseOffs, PinInfo.Info.Pin, pCfg);
    return GPIO_SUCCESS;
  }
  return GPIO_ERROR;

} /* END GPIO_GetPinConfig */



/*=======================================================================================================

  FUNCTION      GPIO_GetOutput

  DESCRIPTION   See GPIOExt.h

  =====================================================================================================*/

GPIOValueType GPIO_GetOutput
(
  GPIOClientHandleType  Handle,
  GPIOKeyType           GPIOKey
)
{
  HALgpioBlockDescType* pDesc =  NULL;
  GPIOPinInfoType PinInfo;
  uint32 nBaseOffs = 0;

  PinInfo.All = GPIOKey;

  if ( ganGpioAccess[PinInfo.Info.PinIdx].All == PinInfo.All )
  {
    pDesc = &GPIOBlockDescriptors[PinInfo.Info.DescIdx];
    nBaseOffs = pDesc->aTiles[PinInfo.Info.TileIdx];
    if ( TRUE == HAL_tlmm_ReadPinOutput( pDesc, nBaseOffs, PinInfo.Info.Pin ) )
    {
      return GPIO_HIGH;
    }
  }
  return GPIO_LOW;

} /* END GPIO_GetOutput */



/*=======================================================================================================

  FUNCTION      GPIO_GetInactiveConfig

  DESCRIPTION   See GPIOExt.h

  =====================================================================================================*/

GPIOResult GPIO_GetInactiveConfig
(
  GPIOClientHandleType Handle,
  GPIOKeyType          GPIOKey,
  GPIOConfigType      *pConfig,
  GPIOValueType       *pValue
)
{
  HALgpioBlockDescType* pDesc =  NULL;
  GPIOPinInfoType PinInfo;
  uint32 nBaseOffs = 0;

  PinInfo.All = GPIOKey;

  if ( ganGpioAccess[PinInfo.Info.PinIdx].All == PinInfo.All )
  {
    pDesc = &GPIOBlockDescriptors[PinInfo.Info.DescIdx];
    nBaseOffs = pDesc->aTiles[PinInfo.Info.TileIdx];
    HAL_tlmm_GetInactiveConfig(pDesc, nBaseOffs, PinInfo.Info.Pin, pConfig, pValue);
    return GPIO_SUCCESS;
  }
  return GPIO_ERROR;

} /* GPIO_GetInactiveConfig */



/*=======================================================================================================

  FUNCTION      GPIO_SetInactiveConfig

  DESCRIPTION   See GPIOExt.h

  =====================================================================================================*/

GPIOResult GPIO_SetInactiveConfig
(
  GPIOClientHandleType Handle,
  GPIOKeyType          GPIOKey,
  GPIOConfigType       Config,
  GPIOValueType        Value
)
{
  HALgpioBlockDescType* pDesc =  NULL;
  GPIOPinInfoType PinInfo;
  uint32 nBaseOffs = 0;

  PinInfo.All = GPIOKey;

  if ( ganGpioAccess[PinInfo.Info.PinIdx].All == PinInfo.All )
  {
    pDesc = &GPIOBlockDescriptors[PinInfo.Info.DescIdx];
    nBaseOffs = pDesc->aTiles[PinInfo.Info.TileIdx];
    HAL_tlmm_SetInactiveConfig(pDesc, nBaseOffs, PinInfo.Info.Pin, Config, Value);
    return GPIO_SUCCESS;
  }
  return GPIO_ERROR;

} /* GPIO_GetInactiveConfig */


/*=======================================================================================================

  FUNCTION      GPIO_GetPinNumber

  DESCRIPTION   See GPIODiag.h

  =====================================================================================================*/

uint32 GPIO_GetPinNumber( GPIOKeyType nGpioId )
{
  GPIOPinInfoType PinInfo;
  PinInfo.All = nGpioId;
  return ((uint32)PinInfo.Info.Pin);
  
} /* GPIO_GetPinNumber */

