/*===========================================================================

FILE:      GPIOCore.c

DESCRIPTION:
This file provides the public class implementation of the GPIO driver.

PUBLIC CLASSES:
  GPIO_Init
  GPIO_Attach
  GPIO_RegisterPinExplicit
  GPIO_ConfigPin
  GPIO_ReadPin
  GPIO_WritePin

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
#include "GPIOTypes.h"
#include "GPIOExt.h"
#include "GPIOCore.h"
#include "HALtlmm.h"
#include "GPIOSvcSync.h"
#include "GPIOIFace.h"


/*=======================================================================================================

                               INTERNAL MACROS AND DATA STRUCTURES

  =====================================================================================================*/


/*
 * Internal array of GPIO information.  The size is a subset of all GPIO to save space as the total
 * number of GPIO pins used on a non-HLOS image is expected to be low.
 */
GPIOPinInfoType ganGpioAccess[GPIO_MAX_ARR_SIZE];


/*
 * GPIO block descriptor Array.  THis may be extracted from device config or compiled into
 * the library binary.  This extern definition usage is expected to change.
 */
extern HALgpioBlockDescType GPIOBlockDescriptors[];

extern uint32 gnNumBlockDescs;

extern boolean GPIO_RouterInit(void);
boolean GPIO_SvcInitChipVersion(uint32 *ChipVersion);

uint32 ChipVersion = 0;

boolean bGpioCoreInitialized = FALSE;


/*
 * Global counter to specify a unique ID for a GPIO registration, providing
 * a means for necessary exclusive access security where requested.  This
 * variable should not be referenced directly, except (and only) through
 * the TLMM_CREATE_UNIQUE_ID macro.
 */
static uint8 gTLMMCntr = 0;


/*
 * Unique Pin ID.  Used within the pin registration function to generate a
 * unique ID for a GPIO.
 */
#define TLMM_CREATE_UNIQUE_ID (++gTLMMCntr)



/*=======================================================================================================

                                   INTERNAL DATA STRUCTURES

  =====================================================================================================*/

/*=======================================================================================================

                            APPLICATION PROGRAMMERS INTERFACE FUNCTIONS

  =====================================================================================================*/

  
/*========================================================================================================

 FUNCTION    GPIO_CoreInit

========================================================================================================*/

boolean GPIO_CoreInit
(
  void
)
{
  uint32 nIdx;
  
  if ( bGpioCoreInitialized == FALSE )
  {
    /*
     * Initialize the internal chip version variable.
     */
    GPIO_SvcInitChipVersion(&ChipVersion);
    
    /*
     * Initialize the synchronization object, if applicable.
     */
    (void)GPIO_SvcSynInit();

    /*
     * Initialize the router protocol if available.
     */
    if ( GPIO_RouterInit() == FALSE )
    {
      return FALSE;
    }

    /*
     * Initialize each HAL block.
     */
    for ( nIdx = 0; nIdx < gnNumBlockDescs; nIdx++ )
    {
      if ( FALSE == HAL_tlmm_Init(&GPIOBlockDescriptors[nIdx]) )
      {
        return FALSE;
      }
    }
    bGpioCoreInitialized = TRUE;
  }
  return TRUE;
  
} /* GPIO_CoreInit */



/*=======================================================================================================

  FUNCTION      GPIO_Attach

  DESCRIPTION   See GPIO.h

  =====================================================================================================*/

GPIOResult GPIO_Attach
(
  GPIODeviceIdType      eDevice,
  GPIOClientHandleType *pHandle
)
{
  if ( pHandle != NULL )
  {
    pHandle->DeviceId = eDevice;
    return GPIO_SUCCESS;
  }
  return GPIO_ERROR;

} /* GPIO_Init */



/*========================================================================================================

 FUNCTION    GPIO_IsBlockDescSupported

========================================================================================================*/

boolean GPIO_IsBlockDescSupported
(
  uint32 nHWVersion
)
{
  if ( (GPIO_MIN_TO_CHIPINFO(nHWVersion) <= ChipVersion) && (GPIO_MAX_TO_CHIPINFO(nHWVersion) >= ChipVersion) )
  {
    return TRUE;
  }
  return FALSE;

}



/*=======================================================================================================

  FUNCTION      GPIO_IsPinValid

  DESCRIPTION   Checks the bounds of a GPIO pin based on the supplied HW block descriptor.

  =====================================================================================================*/

static boolean GPIO_IsPinValid
(
  HALgpioBlockDescType* pBlock,
  uint32                PinNumber
 )
{
    return (pBlock != NULL && PinNumber >= pBlock->nMinPinNumber && PinNumber <= pBlock->nMaxPinNumber);

} /* END GPIO_IsPinValid */



/*=======================================================================================================

  FUNCTION      GPIO_RegisterPinExplicit

  DESCRIPTION   See GPIO.h

  =====================================================================================================*/

GPIOResult GPIO_RegisterPinExplicit
(
  GPIOClientHandleType  Handle,
  uint32                nGpioNumber,
  GPIOAccessType        AccessType,
  GPIOKeyType          *GPIOKey
)
{
  uint32     nSlotIdx, nDescIdx;
  GPIOResult eStatus = GPIO_SUCCESS;

  /*
   * Check if there is a current match and if it is specified as shared
   * access.  Return FALSE if it is exclusive.
   */

  GPIO_SyncEnter();

  for ( nSlotIdx = 0; nSlotIdx < GPIO_MAX_ARR_SIZE; nSlotIdx++ )
  {
    /*
     * If we encounter an empty array slot this pin has not been registered yet.
     */
    if ( ganGpioAccess[nSlotIdx].All == 0 )
    {
      break;
    }
    if ( (ganGpioAccess[nSlotIdx].Info.Pin == nGpioNumber) &&
         (ganGpioAccess[nSlotIdx].Info.DeviceId == Handle.DeviceId) )
    {
      if ( ganGpioAccess[nSlotIdx].Info.Access == GPIO_ACCESS_SHARED )
      {
        /*
         * Found a match.  Short circuit and return success.
         */
        *GPIOKey = ganGpioAccess[nSlotIdx].All;
        GPIO_SyncExit();
        return GPIO_SUCCESS;
      }
      else
      {
        eStatus = GPIO_ERROR;
      }
    }
  }

  /*
   * GPIO has not been registered yet.  Hook up the next available spot.
   */
  if ( (nSlotIdx < GPIO_MAX_ARR_SIZE) && (eStatus != GPIO_ERROR) )
  {
    for ( nDescIdx = 0; nDescIdx < gnNumBlockDescs; nDescIdx++ )
    {
      if ( (Handle.DeviceId == GPIOBlockDescriptors[nDescIdx].DeviceId) &&
         ( (TRUE == GPIO_IsBlockDescSupported(GPIOBlockDescriptors[nDescIdx].nHWVersion))) )
      {
        break;
      }
    }

    /*
     * If the block descriptor was found and the GPIO is valid, we can hook it up.
     */
    if ( nDescIdx < gnNumBlockDescs &&
           GPIO_IsPinValid(&GPIOBlockDescriptors[nDescIdx], nGpioNumber) )
    {
      ganGpioAccess[nSlotIdx].Info.PinIdx   = nSlotIdx;
      ganGpioAccess[nSlotIdx].Info.Pin      = nGpioNumber;
      ganGpioAccess[nSlotIdx].Info.Id       = TLMM_CREATE_UNIQUE_ID;
      ganGpioAccess[nSlotIdx].Info.Access   = AccessType;
      ganGpioAccess[nSlotIdx].Info.DeviceId = Handle.DeviceId;
      ganGpioAccess[nSlotIdx].Info.TileIdx  = HAL_tlmm_GetTileIndex(&GPIOBlockDescriptors[nDescIdx], nGpioNumber);
      ganGpioAccess[nSlotIdx].Info.DescIdx  = nDescIdx;
      *GPIOKey                              = ganGpioAccess[nSlotIdx].All;
      eStatus = GPIO_SUCCESS;
    }
    else
    {
      eStatus = GPIO_ERROR;
    }
  }

  GPIO_SyncExit();

  return eStatus;

} /* GPIO_RegisterPinExplicit */



/*=======================================================================================================

  FUNCTION      GPIO_ConfigPin

  DESCRIPTION   See GPIO.h

  =====================================================================================================*/

GPIOResult GPIO_ConfigPin
(
  GPIOClientHandleType Handle,
  GPIOKeyType          GPIOKey,
  GPIOConfigType       Config
)
{
  HALgpioBlockDescType *pDesc = NULL;
  uint32                nBaseOffs = 0;
  GPIOPinInfoType       PinInfo;
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
    HAL_tlmm_ConfigPin(pDesc, nBaseOffs, PinInfo.Info.Pin, Config);
    nResult = GPIO_SUCCESS;
  }

  GPIO_SyncExit();

  return nResult;

} /* END GPIO_ConfigPin */



/*=======================================================================================================

  FUNCTION      GPIO_ReadPin

  DESCRIPTION   See GPIO.h

  =====================================================================================================*/

GPIOResult GPIO_ReadPin
(
  GPIOClientHandleType Handle,
  GPIOKeyType          GPIOKey,
  GPIOValueType       *pValue
)
{
  HALgpioBlockDescType* pDesc = NULL;
  GPIOPinInfoType PinInfo;
  uint32 nBaseOffs = 0;

  PinInfo.All = GPIOKey;

  if (pValue != NULL)
  {
    if ( ganGpioAccess[PinInfo.Info.PinIdx].All == PinInfo.All )
    {
      /*
       * Get the block descriptor for this GPIO.
       */
      pDesc = &GPIOBlockDescriptors[PinInfo.Info.DescIdx];
      nBaseOffs = pDesc->aTiles[PinInfo.Info.TileIdx];
      
      *pValue = HAL_tlmm_ReadPin(pDesc, nBaseOffs, PinInfo.Info.Pin);
    }
    return GPIO_SUCCESS;    
  }
  return GPIO_ERROR;  

} /* GPIO_ReadPin */



/*=======================================================================================================

  FUNCTION      GPIO_WritePin

  DESCRIPTION   See GPIO.h

  =====================================================================================================*/

GPIOResult GPIO_WritePin
(
  GPIOClientHandleType Handle,
  GPIOKeyType          GPIOKey,
  GPIOValueType        eValue
)
{
  HALgpioBlockDescType* pDesc = NULL;
  GPIOPinInfoType PinInfo;
  uint32 nBaseOffs = 0;

  PinInfo.All = GPIOKey;

  if ( ganGpioAccess[PinInfo.Info.PinIdx].All == PinInfo.All )
  {
    /*
     * Synchronize access.
     */
    GPIO_SyncEnter();

    pDesc = &GPIOBlockDescriptors[PinInfo.Info.DescIdx];
    nBaseOffs = pDesc->aTiles[PinInfo.Info.TileIdx];
    HAL_tlmm_WritePin(pDesc, nBaseOffs, PinInfo.Info.Pin, eValue);

    GPIO_SyncExit();

    return GPIO_SUCCESS;
  }
  return GPIO_ERROR;

} /* GPIO_WritePin */


