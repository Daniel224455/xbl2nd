/** @file SysSleep.c
  SysSleep library stub.

  Copyright (c) 2018 Qualcomm Technologies, Inc.  All Rights Reserved.

  Qualcomm Technologies Proprietary and Confidential.

**/

/*=============================================================================
                              EDIT HISTORY


 when       who     what, where, why
 --------   ---     -----------------------------------------------------------
 04/19/18   yg      Created
 
=============================================================================*/

#include <Uefi.h>
#include "Library/SysSleepLib.h"

VOID SystemSleepInit (UINTN MaxCoreCnt)
{
}

INT32 CoreSleepCpu (UINTN DurationMs)
{
  return 0;
}

UINTN BackupRestoreSystemCtxt (UINTN Operation)
{
  /* Unsupported */
  return RESTORE_SYSTEM_CONTEXT;
}

