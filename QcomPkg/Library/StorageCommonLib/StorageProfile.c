/** @file StorageProfile.c

  Implementation of Storage Profile Library

  Copyright (c) 2016, Qualcomm Technologies Inc.
  All rights reserved.
  Qualcomm Confidential and Proprietary.

**/

/*=============================================================================
                              EDIT HISTORY


 when       who     what, where, why
 --------   ---     -----------------------------------------------------------
 08/30/16   bb      Initial version

=============================================================================*/

#include "StorageCommon.h"
#include "StorageBootCommon.h"
#include <stdio.h>

/* Start/End time for profiling */
static UINT32 gStorageProfileTime = 0;

/* Allows to skip first 'n' profile events */
static UINT32 gStorageProfileThreshold = 0;

/* Count of total profile events */
static UINT32 gStorageProfileCount = 0;

/*
 * Wrapper for get current time
 */
UINT32 StorageProfileGetTime (void)
{
  return boot_common_get_time();
}

#define STORAGE_PROFILE_BUFF_LENGTH 256

char storage_profile_buffer[STORAGE_PROFILE_BUFF_LENGTH];

/*
 * Wrapper for logging the event
 */
void StorageProfilePrint (char *op, UINT64 address, UINT32 size, UINT32 time)
{
  snprintf(storage_profile_buffer, STORAGE_PROFILE_BUFF_LENGTH,
    "STORAGE OP:%s,ADDR:%llu,SIZE:%d,TIME:%d\n", op,address,size,time);

  boot_common_log_message(storage_profile_buffer);
}

/*
 * Get the start time
 */
void StorageProfileStart (void)
{
  if (STOR_PROFILE_BIT_MASK)
    gStorageProfileTime = StorageProfileGetTime();
}

/*
 * End the profile event
 */
void StorageProfileEnd (UINT32 opMask, char *op, UINT64 address, UINT32 size)
{
  if (opMask & STOR_PROFILE_BIT_MASK)
  {
    gStorageProfileTime = StorageProfileGetTime() - gStorageProfileTime;
    gStorageProfileCount++;

    if(gStorageProfileCount >= gStorageProfileThreshold)
    {
      /* For bench marking, the time is accumulated over
         multiple iterations. Below will make that adjustment.
         iteration count is passed in as size */
      if (opMask == STOR_PROFILE_BENCHMARK_BIT_MASK)
      {
        gStorageProfileTime = gStorageProfileTime / size;
      }

      StorageProfilePrint(op, address, size, gStorageProfileTime);

      /* Below will take care evenif the gStorageProfileCount overflows */
      gStorageProfileThreshold = 0;
    }
  }
}

