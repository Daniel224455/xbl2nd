/******************************************************************//**
 * @file PowerSleepLibNull.c
 *
 * @brief PowerSleepLib Null APIs
 *
 * Copyright (c) 2017-2018 by Qualcomm Technologies, Inc.
 * All Rights Reserved.
 *
 *********************************************************************/

#include <Library/PowerSleepLib.h>

uint32 power_sleep_get_rsc_backoff(void)
{
  return 0;
}


sleep_stats_t *power_sleep_get_lpm_counts(power_sleep_stats_enum_t sleep_mode)
{
  return NULL;
}


power_sleep_pc_err_enum power_sleep_enter_pc(power_sleep_restore_fn restore_ptr, void * ctxt, uint64 wakeup_time)
{
  return POWER_SLEEP_SUCCESS;
}

