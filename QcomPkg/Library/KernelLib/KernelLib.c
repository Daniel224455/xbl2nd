/** @file
 * Lib interface for SMP Kernel protocol interface in UEFI
 *  
 * Copyright (c) 2017 - 2019 Qualcomm Technologies Inc. All rights reserved.
 * 
 * Portions Copyright (c) 2008-2014 Travis Geiselbrecht
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files
 * (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 *
**/

/*=============================================================================
                              EDIT HISTORY

 when       who     what, where, why
 --------   ---     -----------------------------------------------------------
 02/23/18   yg      Renamed file from Kernel to scheduler
 02/21/18   yg      Initial version

=============================================================================*/

#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/BaseLib.h>
#include <PiDxe.h>
#include <Library/HobLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/EFIKernelInterface.h>
#include <Library/KernelLib.h>

static EFI_KERNEL_PROTOCOL  *KernIntf = NULL;

BOOLEAN KernelInterfaceInit (VOID);

/********************************************************************************
     Interrupt
********************************************************************************/
VOID RegisterIntrHandler (UINT32 Vector, IntrHandler Handler, VOID *Arg)
{
  if (KernIntf)
    KernIntf->Interrupt->RegisterIntrHandler (Vector, Handler, Arg);
}

KStatus MaskInterrupt (UINT32 Vector)
{
  if (KernIntf)
    return KernIntf->Interrupt->MaskInterrupt (Vector);

  return ERR_NOT_READY;
}

KStatus UnmaskInterrupt (UINT32 Vector)
{
  if (KernIntf)
    return KernIntf->Interrupt->UnmaskInterrupt (Vector);

  return ERR_NOT_READY;
}

KStatus ConfigureInterrupt (UINT32 Vector, enum IntrConfig Config, VOID* Arg)
{
  if (KernIntf)
  {
    if (KernIntf->Version < EFI_KERNEL_PROTOCOL_VER_INTR_CONFIG)
      return ERR_NOT_VALID;
    return KernIntf->Interrupt->ConfigureInterrupt (Vector, Config, Arg);
  }

  return ERR_NOT_READY;
}

/********************************************************************************
     Event
********************************************************************************/
Event* EventInit (OPTIONAL UINT64 Id, BOOLEAN InitialStatus, UINT32 Flags)
{
  if (KernIntf)
    return KernIntf->Event->EventInit (Id, InitialStatus, Flags);

  return 0;
}

VOID EventDestroy (Event *Evt)
{
  if (KernIntf)
    KernIntf->Event->EventDestroy (Evt);
}

BOOLEAN EventInitialized (Event *Evt)
{
  if (KernIntf)
    return KernIntf->Event->EventInitialized (Evt);

  return FALSE;
}

KStatus EventWaitTimeout (Event *Evt, TimeDuration Tm)
{
  if (KernIntf)
    return KernIntf->Event->EventWaitTimeout (Evt, Tm);

  return ERR_NOT_READY;
}

KStatus EventWait (Event *Evt)
{
  if (KernIntf)
    return KernIntf->Event->EventWait (Evt);

  return ERR_NOT_READY;
}

KStatus EventSignal (Event *Evt, BOOLEAN Reschedule)
{
  if (KernIntf)
    return KernIntf->Event->EventSignal (Evt, Reschedule);

  return ERR_NOT_READY;
}

KStatus EventUnsignal (Event *Evt)
{
  if (KernIntf)
    return KernIntf->Event->EventUnsignal (Evt);

  return ERR_NOT_READY;
}

BOOLEAN EventGetSignalState (Event *Evt)
{
  if (KernIntf)
    return KernIntf->Event->EventGetSignalState (Evt);

  return 0;
}

/********************************************************************************
     Mutex
********************************************************************************/
Mutex* MutexInit (OPTIONAL UINT64 Id)
{
  if (KernIntf)
    return KernIntf->Mutex->MutexInit (Id);

  return 0;
}

VOID MutexDestroy (Mutex *Mtx)
{
  if (KernIntf)
    return KernIntf->Mutex->MutexDestroy (Mtx);
}

KStatus MutexAcquire (Mutex *Mtx)
{
  if (KernIntf)
    return KernIntf->Mutex->MutexAcquire (Mtx);

  return ERR_NOT_READY;
}

KStatus MutexAcquireTimeout (Mutex *Mtx, TimeDuration Tm)
{
  if (KernIntf)
    return KernIntf->Mutex->MutexAcquireTimeout (Mtx, Tm);

  return ERR_NOT_READY;
}

KStatus MutexRelease (Mutex *Mtx)
{
  if (KernIntf)
    return KernIntf->Mutex->MutexRelease (Mtx);

  return ERR_NOT_READY;
}

BOOLEAN IsMutexHeld (Mutex *Mtx)
{
  if (KernIntf)
    return KernIntf->Mutex->IsMutexHeld (Mtx);

  return FALSE;
}

/********************************************************************************
     Semaphore
********************************************************************************/

Semaphore* SemInit (OPTIONAL UINT64 Id, UINT32 Cnt)
{
  if (KernIntf)
    return KernIntf->Sem->SemInit (Id, Cnt);

  return 0;
}

VOID SemDestroy (Semaphore * Sem)
{
  if (KernIntf)
    KernIntf->Sem->SemDestroy (Sem);
}

INT32 SemPost (Semaphore * Sem, BOOLEAN Resched)
{
  if (KernIntf)
    return KernIntf->Sem->SemPost (Sem, Resched);

  return ERR_NOT_READY;
}

KStatus SemWait (Semaphore *Sem)
{
  if (KernIntf)
    return KernIntf->Sem->SemWait (Sem);

  return ERR_NOT_READY;
}

KStatus SemTryWait (Semaphore *Sem)
{
  if (KernIntf)
    return KernIntf->Sem->SemTryWait (Sem);

  return ERR_NOT_READY;
}

KStatus SemTimedWait (Semaphore * Sem, TimeDuration Tm)
{
  if (KernIntf)
    return KernIntf->Sem->SemTimedWait (Sem, Tm);

  return ERR_NOT_READY;
}

/********************************************************************************
     Spinlock
********************************************************************************/

Spinlock* SpinLockInit (UINT64 Id OPTIONAL)
{
  if (KernIntf)
    return KernIntf->Spinlock->SpinLockInit (Id);

  return 0;
}

BOOLEAN SpinLockHeld (Spinlock* Spl)
{
  if (KernIntf)
    return KernIntf->Spinlock->SpinLockHeld (Spl);

  return FALSE;
}

INTN SpinTryLock (Spinlock* Spl)
{
  if (KernIntf)
    return KernIntf->Spinlock->SpinTryLock (Spl);

  return ERR_NOT_READY;
}
VOID SpinLock (Spinlock* Spl)
{
  if (KernIntf)
    KernIntf->Spinlock->SpinLock (Spl);
}

VOID SpinUnlock (Spinlock* Spl)
{
  if (KernIntf)
    KernIntf->Spinlock->SpinUnlock (Spl);
}

/********************************************************************************
     Thread
********************************************************************************/

Thread* ThreadCreate (CONST CHAR8 *Name, ThreadStartRoutine Entry, VOID *Arg, INT32 Priority, UINT64 StackSize)
{
  if (KernIntf)
    return KernIntf->Thread->ThreadCreate (Name, Entry, Arg, Priority, StackSize);

  return 0;
}

KStatus ThreadResume (Thread * Thr)
{
  if (KernIntf)
    return KernIntf->Thread->ThreadResume (Thr);

  return ERR_NOT_READY;
}


VOID ThreadExit (INT32 Retcode)
{
  if (KernIntf)
    KernIntf->Thread->ThreadExit (Retcode);
}

Thread* GetCurrentThread (VOID)
{
  if (KernIntf)
    return KernIntf->Thread->GetCurrentThread ();

  return 0;
}

INT32 ThreadGetCurrCpu (Thread* Thr)
{
  if (KernIntf)
    return KernIntf->Thread->ThreadGetCurrCpu (Thr);

  return ERR_NOT_READY;
}

INT32 ThreadGetPinnedCpu (Thread* Thr)
{
  if (KernIntf)
    return KernIntf->Thread->ThreadGetPinnedCpu (Thr);

  return ERR_NOT_READY;
}

VOID ThreadSetCurrCpu (Thread* Thr, INT32 Cpu)
{
  if (KernIntf)
    KernIntf->Thread->ThreadSetCurrCpu (Thr, Cpu);
}

VOID ThreadSetPinnedCpu (Thread* Thr, INT32 Cpu)
{
  if (KernIntf)
    KernIntf->Thread->ThreadSetPinnedCpu (Thr, Cpu);
}

VOID ThreadSetName (CONST CHAR8 *Name)
{
  if (KernIntf)
    KernIntf->Thread->ThreadSetName (Name);
}

VOID ThreadSetPriority (INT32 Priority)
{
  if (KernIntf)
    KernIntf->Thread->ThreadSetPriority (Priority);
}

VOID ThreadSleep (TimeDuration Delay)
{
  if (KernIntf)
    KernIntf->Thread->ThreadSleep (Delay);
  else
    gBS->Stall (Delay * 1000);
}

KStatus ThreadDetach (Thread *Thr)
{
  if (KernIntf)
    return KernIntf->Thread->ThreadDetach (Thr);

  return ERR_NOT_READY;
}

KStatus ThreadJoin (Thread *Thr, INT32* Retcode, TimeDuration Timeout)
{
  if (KernIntf)
    return KernIntf->Thread->ThreadJoin (Thr, Retcode, Timeout);

  return ERR_NOT_READY;
}

KStatus ThreadDetachAndResume (Thread *Thr)
{
  if (KernIntf)
    return KernIntf->Thread->ThreadDetachAndResume (Thr);

  return ERR_NOT_READY;
}

ThreadStats* ThreadGetStats (Thread *Thr, ThreadStats* Tsp)
{
  if (KernIntf)
  {
    if (KernIntf->Version >= EFI_KERNEL_PROTOCOL_VER_THR_CPU_STATS)
      return KernIntf->Thread->ThreadGetStats (Thr, Tsp);
    else
      return NULL; // Old Core UEFI image doesn't support real thread stats
  }

  return 0;
}

UINT64 ThreadGetTimeStamp (VOID)
{
  if (KernIntf)
    return KernIntf->Thread->ThreadGetTimeStamp ();

  return 0;
}


/********************************************************************************
     Timer
********************************************************************************/

Timer* TimerInit (VOID)
{
  if (KernIntf)
    return KernIntf->Timer->TimerInit ();

  return 0;
}

VOID TimerSetOneshot (Timer *Tmr, TimeDuration Delay, TimerCallback Cb, VOID *Arg)
{
  if (KernIntf)
    KernIntf->Timer->TimerSetOneshot (Tmr, Delay, Cb, Arg);
}

VOID TimerSetPeriodic (Timer *Tmr, TimeDuration Period, TimerCallback Cb, VOID *Arg)
{
  if (KernIntf)
    KernIntf->Timer->TimerSetPeriodic (Tmr, Period, Cb, Arg);
}

VOID CancelTimer (Timer *Tmr)
{
  if (KernIntf)
    KernIntf->Timer->CancelTimer (Tmr);
}


/********************************************************************************
      MPCore
********************************************************************************/

UINT32 MpcoreGetMaxCpuCount (VOID)
{
  if (KernIntf)
    return KernIntf->MpCpu->MpcoreGetMaxCpuCount ();

  return 1;
}

UINT32 MpcoreGetAvailCpuCount (VOID)
{
  if (KernIntf)
    return KernIntf->MpCpu->MpcoreGetAvailCpuCount ();

  return 1;
}

/* Initialize any cores deferred initialization. Mask can be 0 in which case
 * all defered cores will be initialized, otherwise the cores with mask 0 will
 * not be initialized
 * Returns how many cores got initialized */
UINT32 MpcoreInitDeferredCores (UINT32 CpuMask OPTIONAL)
{
  if (KernIntf)
    return KernIntf->MpCpu->MpcoreInitDeferredCores (CpuMask);

  return 0;
}

UINT32 MpcoreGetCurrCpu (VOID)
{
  if (KernIntf)
    return KernIntf->MpCpu->MpcoreGetCurrCpu ();

  return 0;
}

UINT32 MpcoreIsCpuActive (UINT32 Cpu)
{
  if (KernIntf)
    return KernIntf->MpCpu->MpcoreIsCpuActive (Cpu);

  return 0;
}

UINT32 MpcoreGetActiveMask (VOID)
{
  if (KernIntf)
    return KernIntf->MpCpu->MpcoreGetActiveMask ();

  return 1;
}

UINT32 MpcoreGetOnlineMask (VOID)
{
  if (KernIntf)
    return KernIntf->MpCpu->MpcoreGetOnlineMask ();

  return 1;
}

VOID MpcoreShutdownScheduler (VOID)
{
  if (KernIntf)
    KernIntf->MpCpu->MpcoreShutdownScheduler ();
}

VOID MpcorePowerOffCpu (UINT32 CpuMask)
{
  if (KernIntf)
    KernIntf->MpCpu->MpcorePowerOffCpu (CpuMask);
}

VOID MpcorePowerOnCpu (UINT32 CpuMask)
{
  if (KernIntf)
    KernIntf->MpCpu->MpcorePowerOnCpu (CpuMask);
}

INT32 MpcoreSleepCpu  (UINTN DurationMs)
{
  if (KernIntf)
    return KernIntf->MpCpu->MpcoreSleepCpu (DurationMs);

  return 0;
}

EFI_STATUS RegisterDevicePowerTransitionCallback (PwrTxnNotifyFn CbFn, VOID* Arg)
{
  if (KernIntf)
    return KernIntf->MpCpu->RegisterPwrTransitionNotify (CbFn, Arg);

  return EFI_UNSUPPORTED;
}

EFI_STATUS UnRegisterDevicePowerTransitionCallback (PwrTxnNotifyFn CbFn)
{
  if (KernIntf)
    return KernIntf->MpCpu->UnRegisterPwrTransitionNotify (CbFn);

  return EFI_UNSUPPORTED;
}

CpuSchedStats* MpCoreGetCpuSchedStats (UINT32 Cpu, CpuSchedStats* Csp)
{
  if (KernIntf)
  {
    if (KernIntf->Version >= EFI_KERNEL_PROTOCOL_VER_THR_CPU_STATS)
      return KernIntf->MpCpu->MpcoreGetCpuSchedStats (Cpu, Csp);
    else
      return KernIntf->Thread->GetThreadSchedStatsDeprecated (Cpu, Csp);
  }

  return 0;
}

/********************************************************************************
      Miscellaneous
********************************************************************************/
UINT32 GetKernelLibVersion (VOID)
{
  if (KernIntf)
  {
    if (KernIntf->Version >= EFI_KERNEL_PROTOCOL_VER_LIB_VER_API)
      return KernIntf->GetLibVersion();
    else
      return (1 << 16);  // return version as 1.0
  }

  return 0;
}

/********************************************************************************
      Instances and setup functions
********************************************************************************/
static  InterruptIntf   *gInterrupt;
static  TimerIntf       *gTimer;
static  ThreadIntf      *gThread;
static  EventIntf       *gEvent;
static  MutexIntf       *gMutex;
static  SemIntf         *gSem;
static  SpinlockIntf    *gSpinlock;
static  MpCpuIntf       *gMpCpu;

EFI_STATUS
EFIAPI
KernelLibInit (VOID)
{
  EFI_STATUS Status = EFI_NOT_FOUND;
  
  if (KernIntf != NULL)
    return EFI_SUCCESS;
  else
  {
    Status = gBS->LocateProtocol (&gEfiKernelProtocolGuid, NULL, (VOID **)&KernIntf);

    if ((Status == EFI_SUCCESS) && (KernIntf != NULL))
    {
      gInterrupt  = KernIntf->Interrupt;
      gTimer      = KernIntf->Timer;
      gThread     = KernIntf->Thread;
      gEvent      = KernIntf->Event;
      gMutex      = KernIntf->Mutex;
      gSem        = KernIntf->Sem;
      gSpinlock   = KernIntf->Spinlock;
      gMpCpu      = KernIntf->MpCpu;
    }
    else
      KernelInterfaceInit ();
  }

  /* Return success regardless. We can't have constructor function fail */
  return EFI_SUCCESS;
}

extern EFI_GUID gEfiSchedIntfGuid;

EFI_KERNEL_PROTOCOL* 
GetSchedIntf (VOID)
{
  EFI_HOB_GUID_TYPE *GuidHob;
  UINT32** DataPtr;

  GuidHob = GetFirstGuidHob (&gEfiSchedIntfGuid);

  if (GuidHob == NULL)
    return NULL;

  DataPtr = GET_GUID_HOB_DATA (GuidHob);
  return (EFI_KERNEL_PROTOCOL*) *DataPtr;
}

BOOLEAN
KernelInterfaceInit (VOID)
{
  if (KernIntf == NULL)
    KernIntf = GetSchedIntf ();

  if (KernIntf == NULL)
    return FALSE;
  else
    return TRUE;
}

EFI_KERNEL_PROTOCOL*
GetKernelIntfPtr (VOID)
{
  return KernIntf;
}

/********************************************************************************
      Miscellaneous helper functions
********************************************************************************/
#define SMP_MAX_CPUS      8
static CpuSchedStats OldStats[SMP_MAX_CPUS], CurrStat;
static UINT64 LastTs;

EFI_STATUS GetCpuLoad (UINT32(*CpuLoadArray)[16])
{
  UINT32 CpuCnt, i;
  CpuSchedStats *Csp;
  UINT64 SamplingPeriod, BusyPercent, IdleTime, BusyTime, CurrTs, ActiveCpuMask;

  if (CpuLoadArray == NULL)
    return EFI_INVALID_PARAMETER;

  CpuCnt = MpcoreGetAvailCpuCount();

  ActiveCpuMask = MpcoreGetActiveMask ();

  CurrTs = ThreadGetTimeStamp ();

  for (i = 0; i < CpuCnt; ++i)
  {
    if ((ActiveCpuMask & (1 << i)) == 0)
    {
      (*CpuLoadArray)[i] = 0;
      continue;
    }

    Csp = MpCoreGetCpuSchedStats (i, &CurrStat);

    if (Csp == NULL)
      continue;

    /* Convert accumulated duration to idle since last sample */
    IdleTime = Csp->IdleTime - OldStats[i].IdleTime;
    
    /* Since our last TS is not in sync with when Sched updates the IdleTime
     * we might incur wrong offset based on sampling sequence */
    SamplingPeriod = CurrTs - LastTs;

    BusyTime = (SamplingPeriod - IdleTime) * 100;
    if (SamplingPeriod < IdleTime)
      BusyTime = 0;

    BusyPercent = BusyTime / SamplingPeriod;

    (*CpuLoadArray)[i] = BusyPercent;

    OldStats[i] = *Csp;
  }

  LastTs = CurrTs;

  return EFI_SUCCESS;
}


