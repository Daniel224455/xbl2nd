/** @file MpTest.c
   
  Application to test BBM
  
#  Copyright (c) 2018 Qualcomm Technologies, Inc. All rights reserved.
   
**/

/*=============================================================================
                              EDIT HISTORY


  when         who     what, where, why
  ----------   ---     -----------------------------------------------------------
  04/16/2018   yg      Initial version
=============================================================================*/

#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/ArmLib.h>
#include <Library/QcomBaseLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/KernelLib.h>

volatile UINT64 CycleCount = 0, Test;

/* Continuously traverse through the full page */
static int BBMTestThread (void *Arg)
{
  int     i, j, cp;
  volatile UINTN  *LPtr = (volatile UINTN*)Arg;
  Thread  *t;
  UINT64 CurrTime, EndTime;
  
  CurrTime = GetTimerCountms();

  EndTime = CurrTime + 20000;

  t = GetCurrentThread ();
   
  cp = MpcoreGetCurrCpu ();
  AsciiPrint ("BBM test Start on %d for page at %X\n", cp, LPtr);
  
  for (j = 0; j < 1000; ++j)
  {
    LPtr = (volatile UINTN*)Arg;

    /* Keep walking through the page */
    for (i=0; i < 4096; i += sizeof(*LPtr))
    {
  //    CycleCount++;
      Test = *LPtr;
      ++LPtr;
    }

#if 0
    if ((j % 250) == 0)
    {
      cp = MpcoreGetCurrCpu ();
      AsciiPrint ("CPU %d  Itr %X\n", cp, j);
    }
#endif

    CurrTime = GetTimerCountms();
    if (CurrTime > EndTime)
      break;
  }

  cp = MpcoreGetCurrCpu ();
  AsciiPrint ("Exit %d after %d iters\n", cp, j);

  return 0;
}

#define MEM_1_GB    (1 * 1024 * 1024 * 1024)
#define MEM_2_MB    (2 * 1024 * 1024)

#define MEM_1_GB_SHIFT  30
#define MEM_2_MB_SHIFT  21

#define BITS_PER_PT_LEVEL   9

#define MAX_PTR_ADDR_RANGE_BITS   40

EFI_STATUS
BBMTest (UINTN Arg)
{
  int i;
  Thread    *t[8];
  UINT32    ActiveMask;
  UINTN     CpuCnt;
  INT32     RetCode;
  VOID*     MemoryPtr, *TestPtr;
  UINT64   *BlkEntryPtr, *PtPtr, PtAddr, TempNum;

  ActiveMask = MpcoreGetActiveMask ();
  CpuCnt = MpcoreGetAvailCpuCount();

  if (CpuCnt < 2)
  {
    if (MpcoreGetAvailCpuCount() < MpcoreGetMaxCpuCount())
      MpcoreInitDeferredCores (0);
    ThreadSleep (10);
    CpuCnt = MpcoreGetAvailCpuCount();
  }

  AsciiPrint ("BBM Delay %d us\n", Arg);

  /* In order to get 2MB block entry */
  MemoryPtr = AllocatePages ((2 * MEM_2_MB) / 4096);

  TestPtr = MemoryPtr;

  TestPtr = (VOID*)((UINTN)TestPtr + (MEM_2_MB - 1));

  TestPtr = (VOID*)(((UINTN)TestPtr) & ~(MEM_2_MB - 1));
  PtAddr = (UINT64)TestPtr;

  PtPtr = (UINT64*)ArmGetTTBR0BaseAddress ();

  /* WARNING:  Assumptions start ...!! 
   * - Works only for AARCH64
   * - Assuming only 3 levels of page tables
   * - First level being pointing to 1GB entry
   * - Second level being pointing to 2MB entry, this is strictly not required, this works even for PT entry
   *
   *   If the assumptions break, test does not work
   * */
  
  PtPtr += ((PtAddr >> MEM_1_GB_SHIFT) & ((1 << BITS_PER_PT_LEVEL) - 1));

  TempNum = (*PtPtr);

  /* PT descriptor to PT pointer */
  TempNum = TempNum & ~((1 << 12) - 1);
  TempNum = TempNum & ((1ULL << MAX_PTR_ADDR_RANGE_BITS) - 1);

  PtPtr = (UINT64*)TempNum;

  PtPtr += ((PtAddr >> MEM_2_MB_SHIFT) & ((1 << BITS_PER_PT_LEVEL) - 1));

  BlkEntryPtr = PtPtr;

  for (i = 1; i < CpuCnt; ++i)
  {
    t[i] = ThreadCreate ("BBM", &BBMTestThread, (void*)TestPtr, DEFAULT_PRIORITY, DEFAULT_STACK_SIZE);

    if (t[i] == NULL) return EFI_UNSUPPORTED;

    ThreadSetPinnedCpu (t[i], i);
    ThreadResume (t[i]);

    /* Leave some gap */
    TestPtr += 8192;
  }

  ThreadSleep (10);

  /* Now while test is running, do BBM */

  for (i = 0; i < 100; ++i)
  {
    UINT64 PD;
    UINTN IntState;

    /* Disable interrupts I/F bits to make sure we don't, break and go service the interrupt */
    IntState = ArmGetInterruptState();
    ArmDisableInterrupts();

    PD = *BlkEntryPtr;

    /* Break */
    *BlkEntryPtr = 0;
    ArmDataSynchronizationBarrier();
    ArmInvalidateTlb();
    ArmDataSynchronizationBarrier();
    ArmInstructionSynchronizationBarrier();

/* TODO: Still crash is seen when all the cores are enabled, need further investigation */
#ifdef WIDEN_BREAK_DURATION
    {
      UINT64 CurrTime, EndTime;
      /* Custom delay to see what kind of delay triggers
       * a real abort compared to BBM abort */
      CurrTime = GetTimerCountus();
      EndTime = CurrTime + Arg;

      while (CurrTime < EndTime)
      {
        CurrTime = GetTimerCountus();
      }
    }
#endif
    /* Make */
    *BlkEntryPtr = PD;
    ArmDataSynchronizationBarrier();
    ArmInstructionSynchronizationBarrier();
    ArmInvalidateTlb();

    if (IntState)
      ArmEnableInterrupts();
  }

  for (i = 1; i < CpuCnt; ++i)
    if (t[i])
      ThreadJoin (t[i], &RetCode, 0xFFFFFFFF);

  ThreadSleep (50);

  FreePages (MemoryPtr, (2 * MEM_2_MB) / 4096);

  return EFI_SUCCESS;
}
  
EFI_STATUS
LaunchBBMTest (UINTN Arg)
{
  UINTN i;

  for (i = 0; i < Arg; ++i)
  {
    BBMTest (i);
  }

  AsciiPrint ("Exiting BBM\n");
  
  return EFI_SUCCESS;
}

