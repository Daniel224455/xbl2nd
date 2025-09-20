/** @file
  Scheduler instance wrapper lib for SMP scheduler in UEFI
   
  Copyright (c) 2017 - 2019 Qualcomm Technologies, Inc. All rights reserved.
  
**/

/*=============================================================================
                              EDIT HISTORY


 when       who     what, where, why
 --------   ---     -----------------------------------------------------------
 02/22/18    yg     Renamed file from Kernel to scheduler
 06/14/17    yg     Initial version

=============================================================================*/
#include <PiPei.h>
#include <Pi/PiHob.h>
#include <Library/HobLib.h>

#include "list.h"
#include "interrupts.h"
#include "kernel/spinlock.h"
#include "kernel/thread.h"
#include "kernel/mutex.h"
#include "kernel/timer.h"
#include "kernel/semaphore.h"
#include "kernel/event.h"
#include "kernel/wait.h"

#include "Protocol/EFIKernelInterface.h"

#include "StWrappers.h"
#include "Library/SysSleepLib.h"
#include <Library/McoreWdogMgmt.h>


InterruptIntf InterruptInst = 
{
  .RegisterIntrHandler = (REGISTER_INTR_HANDLER)&register_int_handler,
  .MaskInterrupt = mask_interrupt,
  .UnmaskInterrupt = unmask_interrupt,
  .ConfigureInterrupt = (CONFIGURE_INTERRUPT)&configure_interrupt,
};

EventIntf EventInst = 
{
  .EventInit = init_event,
  .EventDestroy = event_destroy,
  .EventInitialized = event_initialized,
  .EventWait = event_wait,
  .EventWaitTimeout = event_wait_timeout,
  .EventSignal = event_signal,
  .EventUnsignal = event_unsignal,
  .EventGetSignalState = event_get_signal_state,
};

MutexIntf MutexInst = 
{
  .MutexInit = init_mutex,
  .MutexDestroy = mutex_destroy,
  .MutexAcquire = mutex_acquire,
  .MutexAcquireTimeout = mutex_acquire_timeout,
  .MutexRelease = mutex_release,
  .IsMutexHeld = mutex_held,
};

SemIntf SemInst = 
{
  .SemInit = init_sem,
  .SemDestroy = sem_destroy,
  .SemPost = sem_post,
  .SemWait = sem_wait,
  .SemTryWait = sem_trywait,
  .SemTimedWait = sem_timedwait,
};

SpinlockIntf SpinlockInst =
{
  .SpinLockInit = 0,
  .SpinLockHeld = 0,
  .SpinTryLock = 0,
  .SpinLock = 0,
  .SpinUnlock = 0,
};

ThreadIntf ThreadInst =
{
  .ThreadCreate = thread_create,
  .ThreadResume = thread_resume,
  .ThreadExit = thread_exit,
  .GetCurrentThread = thread_get_current,
  .ThreadGetCurrCpu = thread_get_curr_cpu,
  .ThreadGetPinnedCpu = thread_get_pinned_cpu,
  .ThreadSetCurrCpu = gthread_set_curr_cpu,
  .ThreadSetPinnedCpu = gthread_set_pinned_cpu,
  .ThreadSetName = thread_set_name,
  .ThreadSetPriority = thread_set_priority,
  .ThreadSleep = thread_sleep,
  .ThreadDetach = thread_detach,
  .ThreadJoin = thread_join,
  .ThreadDetachAndResume = thread_detach_and_resume,
  .GetThreadSchedStatsDeprecated = (THREAD_GET_CPU_SCHED_STATS)&get_cpu_sched_stats,
  .ThreadGetTimeStamp = (THREAD_GET_TIMESTAMP)&current_time_hires,
  .ThreadGetStats = (THREAD_GET_STATS)&get_thread_exec_stats,
};

TimerIntf TimerInst = 
{
   .TimerInit = initialize_timer,
   .TimerSetOneshot = (TIMER_SET_ONESHOT)&timer_set_oneshot,
   .TimerSetPeriodic = (TIMER_SET_PERIODIC)&timer_set_periodic,
   .CancelTimer = timer_cancel,
};

MpCpuIntf MpCpu = 
{
    .MpcoreGetMaxCpuCount = mpcore_get_max_cpu_count,
    .MpcoreGetAvailCpuCount = mpcore_get_avail_cpu_count,
    .MpcoreInitDeferredCores = mpcore_init_deferred_cores,
    .MpcoreGetCurrCpu = &mpcore_get_curr_cpu,
    .MpcoreIsCpuActive = mpcore_is_cpu_active,
    .MpcoreGetActiveMask = mpcore_get_active_mask,
    .MpcoreShutdownScheduler = mpcore_shutdown_sched,
    .MpcorePowerOffCpu = mpcore_shutdown_cpu,
    .MpcorePowerOnCpu = mpcore_powerup_cpu,
    .MpcoreSleepCpu = &CoreSleepCpu,
    .RegisterPwrTransitionNotify = &RegisterDevicePowerTransitionCallback,
    .UnRegisterPwrTransitionNotify = &UnRegisterDevicePowerTransitionCallback,
    .MpcoreGetCpuSchedStats = (MPCORE_GET_CPU_SCHED_STATS)&get_cpu_sched_stats,
};

WDogIntf Wdog = 
{
    .WdogEnable = WDogMonitorStart,
    .WdogDisable = WDogMonitorStop,
    .WdogSetBiteTimeout = WDogMonitorSetBiteTimeout,
    .WdogForcePet = WDogMonitorForcePet,
    .WdogFirceBite = WDogMonitorForceBite,
    .WdogSetPetTimerPeriod = WDogMonitorSetPetTimerPeriod,
};

EFI_KERNEL_PROTOCOL SchedIntf = 
{
   .Version = EFI_KERNEL_PROTOCOL_VERSION,
   .Interrupt = &InterruptInst,
   .Timer = &TimerInst,
   .Thread= &ThreadInst,
   .Event = &EventInst,
   .Mutex = &MutexInst,
   .Sem = &SemInst,
   .Spinlock = &SpinlockInst,
   .MpCpu = &MpCpu,
   .WDog = &Wdog,
   .GetLibVersion = &get_sched_lib_version,
};

extern EFI_GUID gEfiSchedIntfGuid;

void SchedIntfInit (void)
{
  //UINTN Data = (UINTN)&SchedIntf;
  EFI_KERNEL_PROTOCOL *SchedIntfProtocol = (VOID *)0x9fc36620;
  /* Asserts on failure */
  //BuildGuidDataHob (&gEfiSchedIntfGuid, &Data, sizeof(Data));
  BuildGuidDataHob (&gEfiSchedIntfGuid, &SchedIntfProtocol, sizeof (SchedIntfProtocol));
}

bool event_initialized(event_t *e)
{
#define EVENT_MAGIC (0x65766E74)  // "evnt"
  return (*((uint32_t*)e) == EVENT_MAGIC);
}

status_t event_wait(event_t *e)
{
  return event_wait_timeout(e, INFINITE_TIME);
}

status_t mutex_acquire(mutex_t *m)
{
  return mutex_acquire_timeout (m, INFINITE_TIME);
}

