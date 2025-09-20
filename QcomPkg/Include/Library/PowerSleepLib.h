#ifndef POWERSLEEPLIB_H
#define POWERSLEEPLIB_H

/******************************************************************//**
 * @file PowerSleepLib.h
 *
 * @brief PowerSleepLib functions
 *
 * Copyright (c) 2017-2018 by Qualcomm Technologies, Inc.
 * All Rights Reserved.
 *
 *********************************************************************/
/*=======================================================================
                        Edit History

   when       who     what, where, why
  --------   ----    --------------------------------------------------- 
  11-10-17    mab     Created

========================================================================*/

#include <Uefi.h>
#include <stddef.h>
#include <string.h>
#include "comdef.h"
#include <Protocol/EFIPsleepInterface.h>

typedef enum
{
  POWER_SLEEP_SUCCESS                    = 0, //Successful sleep entry/exit
  POWER_SLEEP_ERROR_RESTORE_PTR          = 1, //Function pointer passed by the client is invalid
  POWER_SLEEP_ERROR_CORE_SHALLOW_SLEEP   = 2, //We tried to enter C4D4 but another core is in < C4
  POWER_SLEEP_ERROR_CORE_RUNNNING        = 3, //We tried to enter C4D4 but another core started running
  POWER_SLEEP_INSUFFICIENT_TIME          = 4, //We tried to enter C4D4 but there wasn't sufficient time to do so.
  POWER_SLEEP_INTERRUPT_PENDING          = 5, //There was a pending wakeup/interrupt so we 
                                              //short circuited and never fully entered power collapse
  POWER_SLEEP_OTHER_FAILURE              = 6, //Unknown failure, shouldn't ever hit
} power_sleep_pc_err_enum;

typedef UINTN (*power_sleep_restore_fn) (void* ctxt);

/**
 * <!-- power_sleep_init -->
 *
 * @brief Initialize power sleep library 
 */
void power_sleep_init(void);

/**
 * <!-- power_sleep_enter_pc -->
 *
 * @brief Attempts to enter the deepest sleep mode possible for the running core 
 *
 * @param restore_ptr : The function pointer to call on wakeup from sleep
 * @param ctxt        : Client context to be passed along with the function pointer
 * @param wakeup_time : Absolute time in XO ticks (19.2Mhz) that we want the subsystem to wake up.
 *                      This should be the earliest wakeup time for any cores
 *
 * @return error code if there is an issue, SUCCESS otherwise
 */
power_sleep_pc_err_enum power_sleep_enter_pc (power_sleep_restore_fn restore_ptr, void * ctxt, uint64_t wakeup_time);

/*
 * <!-- power_sleep_get_rsc_backoff -->
 *
 * @brief Retreive the RSC backoff time used by sleep 
 *
 * @return RSC backoff time in XO ticks (19.2Mhz)
 */
uint32 power_sleep_get_rsc_backoff(void);

#endif //POWERSLEEPLIB_H
