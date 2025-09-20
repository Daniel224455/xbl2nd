
/*===========================================================================

FILE:      GPIOSvcDevice.c

DESCRIPTION:
This file contains the hardware abstraction for the micro GPIO driver.
This file provides the public class implementation of the GPIO driver.

PUBLIC CLASSES:
  TLMM_SvcDeviceInit
  TLMM_SvcDeviceGetGpioId

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
  06/02/17   dcf     Created.
  ======================================================================================================*/

#include <DALFramework.h>
#include <DALSys.h>
#include <DALDeviceId.h>
#include "com_dtypes.h"
#include "GPIOSvcDevice.h"
#include "GPIOTypes.h"
#include "GPIOIFace.h"
#include "GPIOCore.h"
#include "GPIODevCfg.h"
#include "GPIOSvcSync.h"
#include "HALtlmm.h"
#include "HALhwio.h"


struct TLMMPropHandleType
{
  DALSYS_PROPERTY_HANDLE_DECLARE(hDevice);
};

struct TLMMPropHandleType PropsHandle;
static boolean bIsInitialized = FALSE;

/*
 * GPIO block descriptor Array.  THis may be extracted from device config or compiled into
 * the library binary.  This extern definition usage is expected to change.
 */
extern HALgpioBlockDescType GPIOBlockDescriptors[];
extern uint32 gnNumBlockDescs;

extern uint32 ChipVersion;

/*
 * Array of registered GPIO pins.
 */
extern GPIOPinInfoType ganGpioAccess[];

extern boolean GPIO_IsBlockDescSupported
(
  uint32 nHWVersion
);


/*========================================================================================================

 FUNCTION    TLMM_SvcDeviceInit

========================================================================================================*/


boolean GPIO_IsPinInUse
(
  uint32 nPin
)
{
  uint32 nIdx = 0;
  
  for (nIdx = 0; nIdx < GPIO_MAX_ARR_SIZE; nIdx++ )
  {
    if ( ganGpioAccess[nIdx].Info.Pin == nPin )
    {
      return TRUE;
    }
  }
  
  return FALSE;
}

boolean GPIO_SvcDeviceInit
(
  void
)
{
  HALgpioBlockDescType* pDesc = NULL;
  DALResult eResult = DAL_ERROR;
  DALSYSPropertyVar tPropVar;
  uint32 *pSleepArr = NULL;
  uint32 nPin = 0;
  uint32 nTotalPins = 0;
  GPIOConfigType PinCfg;
  GPIOValueType eValue = GPIO_LOW;
  uint32 nDescIdx = 0, nTileIdx = 0;
  uint32 nBaseOffs = 0;
  
  if ( bIsInitialized == FALSE )
  {
    GPIO_SyncEnter();
    
    /*
     * Initialize the properties handle.
     */
    eResult = DALSYS_GetDALPropertyHandle(DALDEVICEID_TLMM, PropsHandle.hDevice);
    
    if ( eResult == DAL_SUCCESS )
    {
        /*
       * Get the list of low-power GPIO settings.
         */
      eResult = DALSYS_GetPropertyValue( PropsHandle.hDevice, "tlmm_sleep", 0, &tPropVar );

        if ( eResult == DAL_SUCCESS )
        {
        pSleepArr = (uint32*)tPropVar.Val.pStruct;
     
          /*
           * Initialize the TLMM block descriptor index.
           */
          for ( nDescIdx = 0; nDescIdx < gnNumBlockDescs; nDescIdx++ )
          {
            if ( (GPIO_DEVICE_TLMM == GPIOBlockDescriptors[nDescIdx].DeviceId) &&
               ( (TRUE == GPIO_IsBlockDescSupported(GPIOBlockDescriptors[nDescIdx].nHWVersion))) )
            {
              break;
            }
          }
          
          if ( nDescIdx == gnNumBlockDescs )
          {
            GPIO_SyncExit();
            return FALSE;
          }

        nTotalPins = (GPIOBlockDescriptors[nDescIdx].nMaxPinNumber - GPIOBlockDescriptors[nDescIdx].nMinPinNumber) + 1;

          /*
           * For each GPIO pin, decode the configuration settings and set the low-power register
           * as well as configure the pin to low-power if marked as such and not in use.
           */
          for (nPin = 0; nPin < nTotalPins; nPin++)
          {
            pDesc = &GPIOBlockDescriptors[nDescIdx];
            nTileIdx = HAL_tlmm_GetTileIndex(pDesc, nPin);
            nBaseOffs = pDesc->aTiles[nTileIdx];
            eValue = GPIO_LOW;

            PinCfg.func = 0;
            PinCfg.drive = 0;
              
            /*
             * Decode the direction value.
             */
            if (pSleepArr[nPin] & PIN_OUTPUT)
            {
              PinCfg.dir = GPIO_OUT;
                
              /*
               * Check the required output value for the pin.
               */
              if ( pSleepArr[nPin] & PIN_OUT_HIGH )
              {
                eValue = GPIO_HIGH;
              }
            }
            else
            {
              PinCfg.dir = GPIO_IN;
            }
              
            /*
             * Decode the pull value.
             */
            if ( pSleepArr[nPin] & PIN_PULL_DOWN )
            {
              PinCfg.pull = GPIO_PD;
            }
            else if ( pSleepArr[nPin] & PIN_PULL_UP )
            {
              PinCfg.pull = GPIO_PU;
            }
            else if ( pSleepArr[nPin] & PIN_KEEPER )
            {
              PinCfg.pull = GPIO_KP;
            }
            else
            {
              PinCfg.pull = GPIO_NP;
            }

            /*
             * If this GPIO pin has been marked to initialized to low-power
             * do so here.
             */
            if ( pSleepArr[nPin] & PIN_PRG_YES )
            {
              if ( GPIO_IsPinInUse(nPin) == FALSE )
              {
                if ( PinCfg.dir == GPIO_OUT )
                {
                  HAL_tlmm_WritePin(&GPIOBlockDescriptors[nDescIdx], nBaseOffs, nPin, eValue);
                }
                HAL_tlmm_ConfigPin(&GPIOBlockDescriptors[nDescIdx], nBaseOffs, nPin, PinCfg);
              }
            }

            HAL_tlmm_SetInactiveConfig(&GPIOBlockDescriptors[nDescIdx], nBaseOffs, nPin, PinCfg, eValue);
          }
          bIsInitialized = TRUE;
        }
      }
    GPIO_SyncExit();
  }

  return TRUE;

}


boolean GPIO_RouterInit(void) { return TRUE; }
