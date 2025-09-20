/** @file
 *
 *  Interface for System sleep in UEFI
 *  
 * Copyright (c) 2018 Qualcomm Technologies, Inc. All rights reserved.
 * 
**/

/*=============================================================================
                              EDIT HISTORY


 when       who     what, where, why
 --------   ---     -----------------------------------------------------------
 02/23/18   yg      Initial version

=============================================================================*/

#ifndef __SYS_SLEEP_LIB_H__
#define __SYS_SLEEP_LIB_H__

#include <Uefi.h>
#include <Library/PowerSleepLib.h>
#include "Protocol/EFIKernelInterface.h"

#define BACKUP_SYSTEM_CONTEXT       0
#define RESTORE_SYSTEM_CONTEXT      1

/* TODO: Fixme, get the count from some upper layer */
#define SMP_MAX_CPUS    8

/*****************************************************************************
 *  SystemSleepInit
 *
 *  Initialize System Sleep module
 *
 *****************************************************************************/
VOID SystemSleepInit (UINTN MaxCoreCnt);

/*****************************************************************************
 * CoreSleepCpu
 *
 *  Put the core to sleep (Power collapse) for the given duration in milli sec
 *  on which the function is called.
 *
 * Returns POWER_SLEEP_SUCCESS on successfully sleeping or other POWER_SLEEP_*
 *   failure codes on failure.
 *
 *****************************************************************************/
INT32 CoreSleepCpu (UINTN DurationMs);

/*****************************************************************************
 *  BackupRestoreSystemCtxt 
 *
 *  Backup the system context before going into sleep. 
 *  This function returns as part of both backup/restore operation, though
 *  restore is not explicitly invoked. Restore happens when CPU comes
 *  out of sleep. So the calling function can know what execution path is
 *  this function return. This function works similar to setjmp.
 * 
 *
 *  Returns : BACKUP_SYSTEM_CONTEXT if the function is returning as a result
 *            of the backup operation.
 *          : RESTORE_SYSTEM_CONTEXT if the function is returning as a result
 *            of the restore operation
 *
 *****************************************************************************/
UINTN BackupRestoreSystemCtxt (UINTN Operation);

/*****************************************************************************
 *   SleepAborted
 *
 *   Undo the changes performed by Backup operation since the sleep module
 *   cannot go through Sleep and restore operation because of some failure.
 *
 *****************************************************************************/
VOID SleepAborted (UINT32 CurCpu, UINT32 Reason);

/*****************************************************************************
 *   GetWakeupMatchVal
 *
 *   Get the wakeup timer match value for the specified CPU
 *
 *****************************************************************************/
UINT64 GetWakeupMatchVal (UINT32 Cpu);

/*****************************************************************************
 *   SleepLogging related API's
 *
 *   To keep the sleep process independent of other threads processing,
 *   make a new logging API's that can be used only in sleep critical code
 *   during sleep.
 *
 *   Do not use these API's directly, only use the Macro provided for logging.
 *
 *****************************************************************************/

VOID SleepLoggingInit (UINTN MaxCoreCnt);

CHAR8* GetLogBuff (UINTN CpuCore, UINTN *BuffSize);

VOID PostLog (CHAR8* LogStr);

CHAR8* StartLogDrain (UINTN* BytesToDrain);

VOID LogDrainDone (CHAR8* DrainBuff);

// TODO: Map stdlib header if conflicts can be resolved
int snprintf(char *str, UINT64 len, const char *fmt, ...);

/*****************************************************************************
 *   SLEEP_DBG_LOG : Log the message to sleep buffer with least amount of
 *   locking time. Not for use outside the sleep context.
 *****************************************************************************/
#define SLEEP_DBG_LOG(Cpu,...)                                           \
                                                                         \
                            do{                                          \
                               CHAR8 *Buff;                              \
                               UINTN  FreeSize = 0;                      \
                               Buff = GetLogBuff (Cpu, &FreeSize);       \
                                                                         \
                               if ((Buff == 0) || (FreeSize == 0))       \
                                 break;                                  \
                                                                         \
                               snprintf (Buff, FreeSize, __VA_ARGS__);   \
                               PostLog (Buff);                           \
                            }while (0)


/*****************************************************************************
 *   Register and unregister Power Transition notifications for peripherals
 *   when the device needs to go into sleep and exit out of sleep.
 *****************************************************************************/
EFI_STATUS UnRegisterDevicePowerTransitionCallback (PwrTxnNotifyFn CbFn);
EFI_STATUS RegisterDevicePowerTransitionCallback (PwrTxnNotifyFn CbFn, VOID* Arg);

#endif // __SYS_SLEEP_LIB_H__

