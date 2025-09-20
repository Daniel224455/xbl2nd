/** @file MpTest.c
   
  Application to test Malloc and AsciiPrint in Multi core/threading in UEFI env.
  
#  Copyright (c) 2017 - 2018 Qualcomm Technologies, Inc. All rights reserved.
   
**/

/*=============================================================================
                              EDIT HISTORY


  when         who     what, where, why
  ----------   ---     -----------------------------------------------------------
  01/12/2018   mk      Added alloc test
  06/16/2017   yg      Initial version
=============================================================================*/

#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/QcomLib.h>
#include <Library/QcomUtilsLib.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Protocol/LoadedImage.h>
#include <Library/KernelLib.h>

#define DEFAULT_PAGE_COUNT 1
#define MAX_THREAD_CNT    30

static char *TName = "AllocThr";
static char Astr[1024];
static char *ap;

static Semaphore* AppSem;

//PageTest thread 
static int PageTestThread (void *arg)
{
  
  int     i, j, cp;
  char*   name = (char*)arg;
  UINTN   Pages;
  UINTN   MemorySize;
  UINTN   ThreadID, LPattern;
  UINT8   Pattern = 0x55;
  void*   MemoryPtr = NULL ; 
  UINTN  *LPtr;
  Thread  *t ;
  
  SemWait (AppSem);
  t = GetCurrentThread ();
   
  ThreadID = name[0] - '0';
  Pages = ThreadID + DEFAULT_PAGE_COUNT;

  Pattern += ThreadID;
  
  cp = MpcoreGetCurrCpu ();
  AsciiPrint ("Start [%d] on %d\n", ThreadID, cp);
  
  for (i=0; i < 1000; i++)
  {
    UINTN AllocPgCnt = Pages + i;

    MemoryPtr = AllocatePages (AllocPgCnt);
    if (MemoryPtr == NULL)
    {
      cp = MpcoreGetCurrCpu ();
      AsciiPrint ("Fail:Thread[%3d], Mem alloc on CPU Core : %d, Iteration : %d\n", ThreadID, cp, i);
      goto Exit;      
    }

    if ((i % 100) == 0)
    {
      cp = MpcoreGetCurrCpu ();

      AsciiPrint ( "[%d] Addr %lX, Size %lX\n", cp, MemoryPtr, AllocPgCnt);
    }

    //set pattern for MemoryPtr
    MemorySize = EFI_PAGES_TO_SIZE (AllocPgCnt);
    for (j=0; j < 25; ++j)
    {     
      Pattern += (i + j);
      SetMem (MemoryPtr, MemorySize, Pattern);
      LPtr = (UINTN*) MemoryPtr;

      LPattern = Pattern;
      LPattern = LPattern | (LPattern << 8) | (LPattern << 16) | (LPattern << 24) | (LPattern << 32) | (LPattern << 40) | (LPattern << 48) | (LPattern << 56);

      ThreadSleep (10);

      while (MemorySize)
      {
        if (*LPtr != LPattern)
        {
          cp = MpcoreGetCurrCpu ();
          AsciiPrint ("Fail:Thread[%3d], %lX Pattern (%lX)!= %lX check on CPU Core : %d, Iteration : %d\n", ThreadID, LPtr, LPattern, *LPtr, cp, i);
          break;
        }
        LPtr++;
        MemorySize -= sizeof(*LPtr);
      }

      SetMem (MemoryPtr, MemorySize, (UINT8)~Pattern);
    }
    
    //cp = MpcoreGetCurrCpu ();
    //AsciiPrint ("Thread[%3d]: done on CPU Core : %d, Iteration : %d\n", ThreadID, cp, i);
    
    FreePages (MemoryPtr, AllocPgCnt);
    MemoryPtr = NULL;
    Pages = 0;

    /* Let others run too */
    ThreadSleep (10);
  }

  cp = MpcoreGetCurrCpu ();
  AsciiPrint ("Exit [%d] on %d\n", ThreadID, cp);

Exit:
    SemPost (AppSem, FALSE);
    return 0;
}

EFI_STATUS
AllocTest (IN UINTN  ThrCnt)
{
  UINTN     i = 0;
  Thread    *t;
  UINTN     PageCount;

  ap = Astr;

  AppSem = SemInit (0, ThrCnt);
  AsciiPrint ("Allocator test started, Thr Cnt : %d\n", ThrCnt);
  ThreadSleep (100);
  t = GetCurrentThread ();
  
  for (i = 0; i < ThrCnt + 30; ++i)
  {
    ap[0] = '0' + i;
    ap[1] = 0;
    
    PageCount = DEFAULT_PAGE_COUNT + i;    
    t = ThreadCreate (TName, &PageTestThread, (void*) ap, DEFAULT_PRIORITY, DEFAULT_STACK_SIZE);
    ThreadDetach (t);
    ThreadResume (t);
    ThreadSleep (50); 
    ap += 2;
  }
     
  ThreadSleep (570);
  
  /* When we don't have control over the thread contexts we can just use semaphores
   * instead of ThreadJoin to wait for all threads to exit */
  for (i = 0; i < ThrCnt; ++i)
      SemWait (AppSem);

  for (i = 0; i < ThrCnt; ++i)
      SemPost (AppSem, FALSE);
    
  ThreadSleep (270);  
  
  AsciiPrint ("All threads exited\n");
  AsciiPrint ("Exiting main thread-->mp\n");
  
  return EFI_SUCCESS;
}
  

