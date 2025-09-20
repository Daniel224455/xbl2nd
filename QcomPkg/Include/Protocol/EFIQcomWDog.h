/**
  @file  EFIQcomWDog.h
  @brief watchdog Protocol for Qcom UEFI.
*/

/*=============================================================================
  Copyright (c) 2018 Qualcomm Technologies, Incorporated.
  All rights reserved.
  Qualcomm Technologies, Confidential and Proprietary.
=============================================================================*/

/*=============================================================================
                              EDIT HISTORY


 when       who     what, where, why
 --------   ---     -----------------------------------------------------------
 04/13/18   yw      Created.

=============================================================================*/
#ifndef __EFIQCOMWDOG_H__
#define __EFIQCOMWDOG_H__

/*===========================================================================
  INCLUDE FILES
===========================================================================*/
#include <Base.h>

/*===========================================================================
  MACRO DECLARATIONS
===========================================================================*/

/**
  Protocol version.
*/
#define EFI_QCOM_WATCHDOG_PROTOCOL_VERSION 0x0000000000010002

/*  Qcom specific WATCHDOG Protocol GUID definition */
#define EFI_QCOM_WATCHDOG_PROTOCOL_GUID \
   { 0x6f8b0fa0, 0x034f, 0x47a4, { 0x8c, 0x7a, 0xbc, 0xec, 0x55, 0xb4, 0x1c, 0x64 } }

/*===========================================================================
  TYPE DEFINITIONS
===========================================================================*/


/**
  Enable WatchDog and Pet timer

  @param  none

  @retval none
  
**/
typedef
EFI_STATUS
(EFIAPI *EFI_WATCHDOG_ENABLE)(VOID);


/**
  Disable WatchDog and Pet timer

  @param  none

  @retval none
  
**/
typedef
EFI_STATUS
(EFIAPI *EFI_WATCHDOG_DISABLE)(VOID);


/**
  Enable and Set the WDOG_BITE timeout to TimeOutSec

  @param[in]  TimeOutSec   Timeout value in seconds, 0 cancels the timer

  @retval EFI_SUCCESS  success
  
**/
typedef
EFI_STATUS
(EFIAPI *EFI_WATCHDOG_SET_BITE_TIMEOUT)(IN UINT32 TimeOutSec);


/**
  Pet the WDog

  @param  none

  @retval none
  
**/
typedef
VOID
(EFIAPI *EFI_WATCHDOG_FORCE_PET)(VOID);


/**
  Enable WatchDog and force immediate reboot

  @param  none

  @retval none
  
**/

typedef
EFI_STATUS
(EFIAPI *EFI_WATCHDOG_FORCE_BITE)(VOID);


/**
  set pet timer for the wdog

  @param[in]  Peroid   pet timer peroid in seconds

  @retval EFI_SUCCESS  success
  
**/

typedef
EFI_STATUS
(EFIAPI *EFI_WATCHDOG_SET_PET_TIMER_PERIOD)(IN UINT32 PeroidSec);


/** 
  QCOM Watchdog Protocol  
  */
typedef struct _EFI_QCOM_WATCHDOG_PROTOCOL_
{
  UINT64                              Version;
  EFI_WATCHDOG_ENABLE                 Enable;
  EFI_WATCHDOG_DISABLE                Disable;
  EFI_WATCHDOG_SET_BITE_TIMEOUT       SetBiteTimeout;
  EFI_WATCHDOG_FORCE_PET              ForceWDogPet;
  EFI_WATCHDOG_FORCE_BITE             ForceWDogBite;
  EFI_WATCHDOG_SET_PET_TIMER_PERIOD   SetPetTimerPeriod;
} EFI_QCOM_WATCHDOG_PROTOCOL; 

#endif  /* __EFIQCOMWDOG_H__ */

