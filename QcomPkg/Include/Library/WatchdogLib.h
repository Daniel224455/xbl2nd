/** @file WatchdogLib.h
   
  Library implementation to support UEFI Watchdog reset.

  Copyright (c) 2012-2016,2018 Qualcomm Technologies, Inc. All rights reserved. 

**/

/*=============================================================================
                              EDIT HISTORY


 when       who     what, where, why
 --------   ---     -----------------------------------------------------------
 06/24/12   rks      Created
=============================================================================*/
#ifndef __WATCHDOGLIB_H__
#define __WATCHDOGLIB_H__


/*==============================================================================
  FUNCTION      WatchDogEnable

  DESCRIPTION   Enable Watchdog counter to start counting
==============================================================================*/
VOID WatchDogEnable (VOID);

/*==============================================================================
  FUNCTION      WatchDogDisable

  DESCRIPTION   Disable Watchdog counter
==============================================================================*/
VOID WatchDogDisable (VOID);

/*==============================================================================
  FUNCTION      WatchDogSetBiteTimer

  DESCRIPTION   Set the WDOG_BITE_TIME with the input BiteTime in msec
  
  BITETIME      IN PARAM - time in milli seconds
  
  RETVAL        EFI_SUCCESS upon success
                EFI_INVALID_PARAMTER if BITETIME > max time 
  
  Note: If the watchdog Bite timer value is changed while wdog is enabled,
        the new Bite timer value will take effect immediately

==============================================================================*/
EFI_STATUS WatchDogSetBiteTimeout (UINT32 BiteTimeMSec);

/*==============================================================================
  FUNCTION      PetWatchDog

  DESCRIPTION   Pet watchdog so it doesn't bite.
                
==============================================================================*/
VOID WatchDogPet (VOID);

/*==============================================================================
  FUNCTION      TriggerWatchDogBite

  DESCRIPTION   Enable WatchDog and force immediate target reset
==============================================================================*/
VOID WatchDogTriggerBite (VOID);

/*==============================================================================
  FUNCTION      WatchDogSetPetTimerPeriod

  DESCRIPTION   Set the Pet timer period if available
  
  BITETIME      IN PARAM - time in milli seconds
  
  RETVAL        EFI_SUCCESS upon success
                EFI_INVALID_PARAMTER if BITETIME > max time 
  
  Note: If the watchdog Bite timer value is changed while wdog is enabled,
        the new Bite timer value will take effect immediately

==============================================================================*/
EFI_STATUS WatchDogSetPetTimerPeriod (UINT32 PeriodMSec);

#endif /* __WATCHDOGLIB_H__ */
