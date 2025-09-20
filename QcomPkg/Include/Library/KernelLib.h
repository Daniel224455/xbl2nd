/** @file
 * Shared lib interface for SMP scheduler in UEFI
 *  
 * Copyright (c) 2017, 2019 Qualcomm Technologies Inc. All rights reserved.
 * 
 * Portions Copyright (c) 2008-2012 Travis Geiselbrecht
 * Portions Copyright 2016 The Fuchsia Authors
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
 02/21/18   yg      Updated to latest interface
 06/14/17   yg      Initial version

=============================================================================*/

#include <Protocol/EFIKernelInterface.h>

#ifndef __KERNEL_LIB_H__
#define __KERNEL_LIB_H__

/*
 *  NOTE: Refer to the protocol header EFIKernelInterface.h for additional
 *   documentation of the API's and data types/macros etc
 *
 *  In All the following API's:
 *    Id is OPTIONAL, if id == 0 then will return a new object,
 *    Macro STR_OBJECT_ID can be used to define friendly ID with ASCII chars 
 */

/********************************************************************************
     Interrupt
********************************************************************************/

/********************************************************************************
 *   Interrupt handler interface
 *
 *   Note that the interrupt handlers registered should not conflict with
 *   scheduler registered interrupts for PPI, SGI and SPI interrupt vectors
 *
 *   Handlers execute in ISR context, so only events can be posted with some
 *   restrictions.
 *   
 ********************************************************************************/
VOID RegisterIntrHandler (UINT32 Vector, IntrHandler Handler, VOID *Arg);

KStatus MaskInterrupt (UINT32 Vector);
KStatus UnmaskInterrupt (UINT32 Vector);
KStatus ConfigureInterrupt (UINT32 Vector, enum IntrConfig Config, VOID* Arg);

/********************************************************************************
     Event
********************************************************************************/

/* Rules for Events:
 * - Events may be signaled from interrupt context *but* the reschedule
 *     parameter must be false in that case.
 * - Events may not be waited upon from interrupt context.
 * - Events *without* FLAG_AUTOUNSIGNAL:
 *   - Wake up any waiting threads when signaled.
 *   - Continue to do so (no threads will wait) until unsignaled.
 * - Events *with* FLAG_AUTOUNSIGNAL:
 *   - If one or more threads are waiting when signaled, one thread will
 *     be woken up and return.  The signaled state will not be set.
 *   - If no threads are waiting when signaled, the Event will remain
 *     in the signaled state until a thread attempts to wait (at which
 *     time it will unsignal atomicly and return immediately) or
 *     event_unsignal() is called.
*/
Event* EventInit (OPTIONAL UINT64 Id, BOOLEAN InitialStatus, UINT32 Flags);
VOID EventDestroy (Event *Evt);
BOOLEAN EventInitialized (Event *Evt);

KStatus EventWaitTimeout (Event *Evt, TimeDuration Tm);
KStatus EventWait (Event *Evt);

KStatus EventSignal (Event *Evt, BOOLEAN Reschedule);
KStatus EventUnsignal (Event *Evt);

BOOLEAN EventGetSignalState (Event *Evt);

/********************************************************************************
     Mutex
********************************************************************************/
/* Rules for Mutexes:
 * - Mutexes are only safe to use from thread context.
 * - Mutexes are non-recursive.
*/

Mutex* MutexInit (OPTIONAL UINT64 Id);
VOID MutexDestroy (Mutex *Mtx);

KStatus MutexAcquire (Mutex *Mtx);
KStatus MutexAcquireTimeout (Mutex *Mtx, TimeDuration Tm); /* try to acquire the mutex with a timeout value */

KStatus MutexRelease (Mutex *Mtx);

BOOLEAN IsMutexHeld (Mutex *Mtx);

/********************************************************************************
     Semaphore
********************************************************************************/

Semaphore* SemInit (OPTIONAL UINT64 Id, UINT32 Cnt);
VOID SemDestroy (Semaphore * Sem);

INT32 SemPost (Semaphore * Sem, BOOLEAN Resched);

KStatus SemWait (Semaphore *Sem);
KStatus SemTryWait (Semaphore *Sem);
KStatus SemTimedWait (Semaphore * Sem, TimeDuration Tm);

/********************************************************************************
     Spinlock
********************************************************************************/

Spinlock* SpinLockInit (UINT64 Id OPTIONAL);

BOOLEAN SpinLockHeld (Spinlock*);

INTN SpinTrylock (Spinlock*);
VOID SpinLock (Spinlock*);

VOID SpinUnlock (Spinlock*);

/********************************************************************************
     Thread
********************************************************************************/

Thread* ThreadCreate (CONST CHAR8 *Name, ThreadStartRoutine Entry, VOID *Arg, INT32 Priority, UINT64 StackSize);

KStatus ThreadResume (Thread * Thr);

VOID ThreadExit (INT32 Retcode);

/* the current thread */
Thread* GetCurrentThread (VOID);

/* The CPU, specified thread configured to or actually executing on
 * Gets info from the data structures in scheduler */
INT32 ThreadGetCurrCpu (Thread* Thr);
INT32 ThreadGetPinnedCpu (Thread* Thr);
VOID ThreadSetCurrCpu (Thread* Thr, INT32 Cpu);
VOID ThreadSetPinnedCpu (Thread* Thr, INT32 Cpu);
VOID ThreadSetName (CONST CHAR8 *Name);
VOID ThreadSetPriority (INT32 Priority);

VOID ThreadSleep (TimeDuration Delay);

KStatus ThreadDetach (Thread *Thr);
KStatus ThreadJoin (Thread *Thr, INT32 *Retcode, TimeDuration Timeout);
KStatus ThreadDetachAndResume (Thread *Thr);

ThreadStats* ThreadGetStats (Thread *Thr, ThreadStats* Tsp);
UINT64 ThreadGetTimeStamp (VOID);

/********************************************************************************
     Timer
********************************************************************************/

/* Rules for Timers:
 * - Timer callbacks occur from interrupt context
 * - Timers may be programmed or canceled from interrupt or thread context
 * - Timers may be canceled or reprogrammed from within their callback
 * - Timers currently are dispatched from a 10ms periodic tick
*/

Timer* TimerInit (VOID);
VOID TimerSetOneshot (Timer *Tmr, TimeDuration Delay, TimerCallback Cb, VOID *Arg);
VOID TimerSetPeriodic (Timer *Tmr, TimeDuration Period, TimerCallback Cb, VOID *Arg);
VOID CancelTimer (Timer *Tmr);

/********************************************************************************
      MPCore
********************************************************************************/
UINT32 MpcoreGetMaxCpuCount (VOID);

UINT32 MpcoreGetAvailCpuCount (VOID);

/* Initialize any cores deferred initialization. 
 *  Mask can be 0 in which case all defered cores will be initialized.
 *  If Mask is non 0, then the bits set to 1 indicate the cores to be initialized
 *  Returns how many cores got initialized */
UINT32 MpcoreInitDeferredCores (UINT32 CpuMask OPTIONAL);

/* The CPU current code is executing on
 * Gets info directly from CPU register resources */
UINT32 MpcoreGetCurrCpu (VOID);

UINT32 MpcoreIsCpuActive (UINT32 Cpu);

UINT32 MpcoreGetActiveMask (VOID);
UINT32 MpcoreGetOnlineMask (VOID);

VOID MpcoreShutdownScheduler (VOID);
VOID MpcorePowerOffCpu (UINT32 CpuMask);
VOID MpcorePowerOnCpu (UINT32 CpuMask);

INT32 MpcoreSleepCpu (UINTN DurationMs);

EFI_STATUS RegisterDevicePowerTransitionCallback (PwrTxnNotifyFn CbFn, VOID* Arg);
EFI_STATUS UnRegisterDevicePowerTransitionCallback (PwrTxnNotifyFn CbFn);

CpuSchedStats* MpCoreGetCpuSchedStats (UINT32 Cpu, CpuSchedStats* Csp);

/********************************************************************************
      WatchDog
********************************************************************************/
EFI_STATUS WatchDogEnable (VOID);
VOID WatchDogDisable (VOID);
EFI_STATUS WatchDogSetBiteTimeout (UINT32 Timeoutms);
VOID WatchDogForcePet (VOID);
VOID WatchDogForceBite (VOID);
EFI_STATUS WatchDogSetPetTimerPeriod (UINT32 TimerPeriodms);

static inline VOID WatchDogPet (VOID)
{
  WatchDogForcePet ();
}

static inline VOID WatchDogTriggerBite (VOID)
{
  WatchDogForceBite ();
}

/********************************************************************************
      Miscellaneous
********************************************************************************/
/* [15:0]  Minor version represents changes in interface and minor behaviors
 * [31:16] Major version represent underlying major implementation changes */
UINT32 GetKernelLibVersion (VOID);

EFI_KERNEL_PROTOCOL* GetKernelIntfPtr (VOID);

EFI_STATUS GetCpuLoad (UINT32(*CpuLoadArray)[16]);

#endif  /* __KERNEL_LIB_H__ */

