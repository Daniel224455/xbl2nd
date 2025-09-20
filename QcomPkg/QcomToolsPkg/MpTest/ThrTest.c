/** @file MpTest.c
   
  Application to test Multi core/threading in UEFI env.
  
#  Copyright (c) 2017 Qualcomm Technologies, Inc. All rights reserved.
   
**/

/*=============================================================================
                              EDIT HISTORY


  when         who     what, where, why
  ----------   ---     -----------------------------------------------------------
  06/16/2017   yg      Initial version
=============================================================================*/

#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/QcomLib.h>
#include <Library/QcomBaseLib.h>
#include <Library/QcomUtilsLib.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/KernelLib.h>

EFI_STATUS CheckCPUTemp (UINTN* MaxTemp OPTIONAL);

char shstr[512], *ap, *ep;
char *spstr[8] = {
  " ",
  " \t  ",
  " \t  \t  ",
  " \t  \t  \t  ",
  " \t  \t  \t  \t  ",
  " \t  \t  \t  \t  \t  ",
  " \t  \t  \t  \t  \t  \t  ",
  " \t  \t  \t  \t  \t  \t  \t  ",
};

/* Kill some CPU cycles */
void st_delay (unsigned long long d);
void st_delay (unsigned long long d)
{
  volatile unsigned long long n = 0;

  d = d << 16;

  while (n < d)
  {
    ++n;
  }
}

Semaphore* AppSem;

static int PinningControlThread (void *arg)
{
  int i = 0, cp;
  char* name = "Pin";
  unsigned loop_cnt;
  Thread *t;

  SemWait (AppSem);

  t = GetCurrentThread ();
  loop_cnt = 10;

  cp = ThreadGetCurrCpu (t);

  cp = cp & 7;

  DEBUG (( EFI_D_ERROR, "%a%a:Start\n", spstr[cp], name));

  while (1)
  {
    st_delay (370);

    cp = ThreadGetCurrCpu (t);
    cp = cp & 7;

    DEBUG (( EFI_D_ERROR, "%a%a:%d\n", spstr[cp], name, i));

    i++;
    if (i > loop_cnt)
      break;

    if (i & 1)
    {
      ++cp;
      //if (cp >= MpcoreGetMaxCpuCount ())
      if (cp >= 4)
        cp = 0;

      DEBUG (( EFI_D_ERROR, "Pinning to CPU %d\n", cp));

      ThreadSetPinnedCpu (t, cp);
    }
  }

  DEBUG (( EFI_D_ERROR, "%a%a:Exit\n", spstr[cp], name));

  SemPost (AppSem, FALSE);
  
  return 0;
}

static int PriorityTestThread (void *arg)
{
  int i = 0, cp;
  char* name = (char*)arg;
  unsigned loop_cnt;
  Thread *t;

  SemWait (AppSem);

  t = GetCurrentThread ();
  loop_cnt = 60;

  cp = ThreadGetCurrCpu (t);
  cp = cp & 7;

  DEBUG (( EFI_D_ERROR, "%a%a:Start\n", spstr[cp], name));

  while (1)
  {
    st_delay (370);

    cp = ThreadGetCurrCpu (t);
    cp = cp & 7;

    DEBUG (( EFI_D_ERROR, "%a%a:%d\n", spstr[cp], name, i));

    i++;
    if (i > loop_cnt)
      break;
  }

  DEBUG (( EFI_D_ERROR, "%a%a:Exit\n", spstr[cp], name));

  SemPost (AppSem, FALSE);
  
  return 0;
}

static int app_thread (void *arg)
{
  int i = 0, cp;
  char* name = (char*)arg;
  unsigned loop_cnt, log_int;
  Thread *t;

  t = GetCurrentThread ();

  SemWait (AppSem);

  loop_cnt = 100;
  log_int = 10;

  cp = ThreadGetCurrCpu (t);
  cp = cp & 7;

  DEBUG (( EFI_D_ERROR, "%a%a:Start\n", spstr[cp], name));

  while (1)
  {
    /* Do some CPU intensive processing */
    st_delay (670);

    /* Occationally let other threads run */
    if ((i%5) == 0)
    {
      ThreadSleep (10);
    }

    cp = ThreadGetCurrCpu (t);
    cp = cp & 7;

    if ((i%log_int) == 0)
    {
      DEBUG (( EFI_D_ERROR, "%a%a:%d\n", spstr[cp], name, i));
    }

    i++;
    if (i > loop_cnt)
      break;
  }

  DEBUG (( EFI_D_ERROR, "%a%a:Exit\n", spstr[cp], name));

  SemPost (AppSem, FALSE);

  return 0;
}

#define MAX_THREAD_CNT    25

int
mptest ( IN VOID  *Arg)
{
  int i = 0, cnt, cp;
  Thread *t;
  UINT32 Prio;

  AsciiPrint ("ALK\r\n");

  AppSem = SemInit (0, MAX_THREAD_CNT + 2);

  ap = shstr;
  ep = shstr + sizeof (shstr);  

  DEBUG (( EFI_D_ERROR, "Mptest app started\n"));
  ThreadSleep (600);

  Prio = DEFAULT_PRIORITY + 4;

  for (cnt = 0; cnt < 5; ++cnt)
  {
    t = GetCurrentThread ();
    cp = ThreadGetCurrCpu (t);
    cp = cp & 7;

    DEBUG (( EFI_D_ERROR, "%a%a:%d\n", spstr[cp], "Mp", cnt));

    ThreadSleep (100);

    for (i=0; i<3; ++i)
    {
      st_delay (1000);
      DEBUG (( EFI_D_ERROR, "."));
    }

    ThreadSleep (270);
    
    DEBUG (( EFI_D_ERROR, "Priority test thread with priority %d\n", Prio));

    t = ThreadCreate ("Prio", &PriorityTestThread, (void*)"Prio", Prio, DEFAULT_STACK_SIZE);
//    ThreadSetPinnedCpu (t, 0);
    ThreadResume (t);
    ThreadDetach (t);

    
    t = ThreadCreate ("Pin", &PinningControlThread, (void*)"Pin", Prio, DEFAULT_STACK_SIZE);
//    ThreadSetPinnedCpu (t, 0);
    ThreadResume (t);
    ThreadDetach (t);

    Prio -= 4;
    if (Prio == 0)
      Prio = DEFAULT_PRIORITY + 8;


    DEBUG (( EFI_D_ERROR, "Starting %d threads\n", MAX_THREAD_CNT));

    ap = shstr;
    for (i=0; i<MAX_THREAD_CNT; ++i)
    {
      ap[0] = 'A' + i;
      ap[1] = ap[0];
      ap[2] = 0;

      t = ThreadCreate (ap, &app_thread, (void*)ap, DEFAULT_PRIORITY, DEFAULT_STACK_SIZE);
//      ThreadSetPinnedCpu (t, 0);
      ThreadDetach (t);
      ThreadResume (t);
      ThreadSleep ( 50 + i * 25);

      ap += 4;
      if (ap >= ep)
      {
        DEBUG (( EFI_D_ERROR, "Out of string mem\n"));
        break;
      }
    }

    /* Wait for all threads to exit +2 accounts for priority test thread and pinning control thread
     * Since we cannot exit the app before those threads exit, since UEFI would free all the memory held
     * by the app would cause crash of the threads still running */
    for (i=0; i<MAX_THREAD_CNT + 2; ++i)
      SemWait (AppSem);

    for (i=0; i<MAX_THREAD_CNT + 2; ++i)
      SemPost (AppSem, TRUE);

    DEBUG (( EFI_D_ERROR, "All threads exited\n\n"));

    ThreadSleep (570);
  }

  DEBUG (( EFI_D_ERROR, "Exiting main thread\n"));
  
  AsciiPrint ("AULK\r\n");

  return 0;
}

Thread *mpt;

EFI_STATUS
RunMultiThreadTest (VOID)
{
  INT32 RetCode;

  mpt = ThreadCreate ("MP", &mptest, (void*)"MP", DEFAULT_PRIORITY, DEFAULT_STACK_SIZE);
//    ThreadSetPinnedCpu (mpt, 0);

  if (mpt == NULL)
  {
    DEBUG (( EFI_D_ERROR, "Unable to create thread, exiting ...\n"));
    return EFI_UNSUPPORTED;
  }

  ThreadResume (mpt);

  ThreadJoin (mpt, &RetCode, 0xFFFFFFF);
  return EFI_SUCCESS;
}

