/** @file cpt_MpTest2.c
   
  Application to test Multi core/threading in UEFI env.
  
#  Copyright (c) 2017 - 2018 Qualcomm Technologies, Inc. All rights reserved.   
   
**/

/*=============================================================================
                              EDIT HISTORY


  when         who     what, where, why
  ----------   ---     -----------------------------------------------------------
  09/12/2018   rw      Initial version
=============================================================================*/

#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/QcomLib.h>
#include <Library/PrintLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/KernelLib.h>
#include <Protocol/EFIScm.h>
#include <Library/LockLib.h>

#include <Library/QcomBaseLib.h>

#define TEST_CONFIG_LEN2                      20
#define TOTAL_CPU2                            8

typedef enum
{
   TC_CATEGORY_INIT = 0,
   TC_CATEGORY_DEINIT,
   TC_CATEGORY_LOCK,
   TC_CATEGORY_MUTEX,
   TC_CATEGORY_EVENT,
   TC_CATEGORY_SEM,
   TC_CATEGORY_CPU,
   TC_CATEGORY_THREAD,
   TC_CATEGORY_LIMIT = 0xFFFF
} tc_category_enum_type;

typedef enum
{
   TC_TEST_PRIVATE = 0,
   TC_TEST_SINGLE,
   TC_TEST_LONG_STR,
   TC_TEST_RECURSIVE,
   TC_TEST_APP1,
   TC_TEST_APP2,
   TC_TEST_CLEAR,
   TC_TEST_SET,
   TC_TEST_GET,
   TC_TEST_WAIT,
   TC_TEST_CREATE,
   TC_TEST_TIMEOUT,
   TC_TEST_UNSIGNAL,
   TC_TEST_UNSIG_REG,
   TC_TEST_TRY,
   TC_TEST_PRIORITY_UNEQ,
   TC_TEST_PRIORITY_EQ,
   TC_TEST_PRIORITY,
   TC_TEST_NAME,
   TC_TEST_ANY,
   TC_TEST_CURRENT,
   TC_TEST_PIN,
   TC_TEST_DETACH,
   TC_TEST_YIELD,
   TC_TEST_PREEMPT,
   TC_TEST_RC,
   TC_TEST_MAX,
   TC_TEST_MP_EN_ALL,
   TC_TEST_MP_EN_EACH,
   TC_TEST_MP_EN_LAST,
   TC_TEST_MP_EN_TWO,
   TC_TEST_LIMIT=0xFFFF

} tc_test_enum_type;

/* Command Line Types */
typedef enum
{
  TEST_SUITE = 0,
  TEST_CATEGORY,
  TEST_CASE,
  CMD_INVALID,
} command_option;

/* Command Parameters */
typedef struct
{
  CHAR8 suite[TEST_CONFIG_LEN2];
  UINTN category;
  UINTN tc;
} test_config;

static test_config tc_config;

test_config *get_test_config2 (UINTN argc, CHAR8 **argv);

/**
  Entry point for Menu App

  @param[in] ImageHandle    The firmware allocated handle for the EFI image.  
  @param[in] SystemTable    A pointer to the EFI System Table.
  
  @retval EFI_SUCCESS           Erase successfully.
  @retval EFI_DEVICE_ERROR      Failed to program
  @retval EFI_OUT_OF_RESOURCES  Failed to allocate memory for 
                                the buffers
**/

/* get_test_config: extract test arguments */
test_config *get_test_config2 (UINTN argc, CHAR8 **argv) {
   UINT32 index = 0;

   while (index < argc)
   {
      command_option CmdOp = CMD_INVALID;

      if (AsciiStriCmp (*(argv + index), "-s") == 0) 
      {
         CmdOp = TEST_SUITE;
         index++;
      }
      else if (AsciiStriCmp (*(argv + index), "-c") == 0) 
      {
         CmdOp = TEST_CATEGORY;
         index++;
      }      
      else if (AsciiStriCmp (*(argv + index), "-t") == 0) 
      {
         CmdOp = TEST_CASE;
         index++;
      }

      switch (CmdOp)
      {

         case TEST_SUITE:

            if (AsciiStriCmp (*(argv + index), "cpt") == 0) 
            {
               //tc_config.suite = "cpt";
               AsciiStrnCpy(tc_config.suite, "cpt", TEST_CONFIG_LEN2 - 1);
            }
            else {
               //tc_config.suite = "dev";
               AsciiStrnCpy(tc_config.suite, "dev", TEST_CONFIG_LEN2 - 1);
            }
            index++;
            break;

         case TEST_CATEGORY:

            if (AsciiStriCmp (*(argv + index), "init") == 0) {
               //tc_config.category = "lock";
               tc_config.category = TC_CATEGORY_INIT;
            }
            else if (AsciiStriCmp (*(argv + index), "deinit") == 0) {
               //tc_config.category = "lock";
               tc_config.category = TC_CATEGORY_DEINIT;
            }
            else if (AsciiStriCmp (*(argv + index), "lock") == 0) {
               //tc_config.category = "lock";
               tc_config.category = TC_CATEGORY_LOCK;
            }
            else if (AsciiStriCmp (*(argv + index), "event") == 0) {
               tc_config.category = TC_CATEGORY_EVENT;
            }
            else if (AsciiStriCmp (*(argv + index), "sem") == 0) {
               tc_config.category = TC_CATEGORY_SEM;
            }
            else {
               //tc_config.category = "event";
               tc_config.category = TC_CATEGORY_EVENT;
            }
            index++;
            break;

         case TEST_CASE:

            if (AsciiStriCmp (*(argv + index), "private") == 0) 
            {
               //tc_config.tc = "private";
               tc_config.tc = TC_TEST_PRIVATE;

            }
            else if (AsciiStriCmp (*(argv + index), "single") == 0) 
            {
               //tc_config.tc = "single";
               tc_config.tc = TC_TEST_SINGLE;
            }
            else if (AsciiStriCmp (*(argv + index), "recursive") == 0)  {
               //tc_config.tc = "recursive";
               tc_config.tc = TC_TEST_RECURSIVE;
            }
            else if (AsciiStriCmp (*(argv + index), "app1") == 0)  {
               //tc_config.tc = "app1";
               tc_config.tc = TC_TEST_APP1;
            }
            else if (AsciiStriCmp (*(argv + index), "app2") == 0)  {
               //tc_config.tc = "app2";
               tc_config.tc = TC_TEST_APP2;
            }
            else if (AsciiStriCmp (*(argv + index), "clear") == 0)  {
               //tc_config.tc = "clear";
               tc_config.tc = TC_TEST_CLEAR;
            }
            else if (AsciiStriCmp (*(argv + index), "set") == 0)  {
               //tc_config.tc = "set";
               tc_config.tc = TC_TEST_SET;
            }
            else if (AsciiStriCmp (*(argv + index), "get") == 0)  {
               //tc_config.tc = "get";
               tc_config.tc = TC_TEST_GET;
            }
            index++;
            break;

         case CMD_INVALID:

            index++;
            break;

      }  // switch CmdOp

   } // while argc 

   return &tc_config;
}

Thread *mpt_apps2;

VOID *lock_apps2;

Semaphore *sem_apps2;
Event *event1_apps2;
Event *event2_apps2;

UINTN shared_counter2_g;

EFI_STATUS clear_counter2() {

   shared_counter2_g = 0;

   return EFI_SUCCESS;

}

EFI_STATUS set_counter2(UINTN increment) {

   shared_counter2_g += increment;

   return EFI_SUCCESS;

}

UINTN get_counter2() {
   //AsciiPrint ("Thread (%d) - CPU (%d): got shared_counter2_g %d\n", t, cp, shared_counter2_g);
   return shared_counter2_g;
}

UINTN Get_Next_CPU_2(UINTN current_cpu) {
   UINTN tmp_cpu;
   UINTN i;
   UINTN active_mask;
   UINTN last_active_cpu;
   UINTN active_cpu_set[TOTAL_CPU2];
   UINTN active_cpu;
   //UINTN tmp1, tmp2;
   BOOLEAN found_next_cpu;
   last_active_cpu = 0;

   active_mask = MpcoreGetActiveMask();
   //AsciiPrint ("Active CPU Mask(%X)\n", active_mask);

   for (i=0;i<TOTAL_CPU2;i++) {
      switch (i)
      {

         case 0:  active_cpu = active_mask & 1;
                  break;

         case 1:  active_cpu = active_mask & 2;
                  break;

         case 2:  active_cpu = active_mask & 4;
                  break;

         case 3:  active_cpu = active_mask & 8;
                  break;

         case 4:  active_cpu = active_mask & 16;
                  break;

         case 5:  active_cpu = active_mask & 32;
                  break;

         case 6:  active_cpu = active_mask & 64;
                  break;

         case 7:  active_cpu = active_mask & 128;
                  break;

         default: active_cpu = active_mask & 0;
                  break;
   
      }  // switch CmdOp

       if (active_cpu) {
         active_cpu_set[i] = 1;
         last_active_cpu = i;
      }
      else {
         active_cpu_set[i] = 0;
      }
      //AsciiPrint ("\tCPU[%d]: %d\n", i, active_cpu_set[i]);
   }

   found_next_cpu = 0;
   tmp_cpu = current_cpu;
   while (!found_next_cpu) {
      if (tmp_cpu == last_active_cpu) {
         tmp_cpu = 0;
      }
      else {
         tmp_cpu++;
      }
      if (active_cpu_set[tmp_cpu] == 1) {
         found_next_cpu = 1;
      }
   }

   return tmp_cpu;
}

int t_lock_app2( IN VOID  *Arg)
{
   int cp;
   Thread *t;
   UINTN cntr, inc_value;
   UINTN lock_time_before, lock_time_after, release_time;
   UINTN iteration, iteration_total;

   iteration_total = 100;

   t = GetCurrentThread();
   cp = ThreadGetCurrCpu(t);

   AsciiPrint ("Executing APP2 Thread1 (t_lock_app2) with ID: %d on CPU: %d\n", t, cp);

   ThreadSleep (100);

   lock_time_before = GetTimerCountms ();
   AsciiPrint ("APP2 Thread1 - CPU (%d): before got lock at time: %d ms\n", cp, lock_time_before);
   Lock(lock_apps2);
   lock_time_after = GetTimerCountms ();
   AsciiPrint ("APP2 Thread1 - CPU (%d): after got lock at time: %d ms\n",  cp, lock_time_after);

   for (iteration=0; iteration<iteration_total; iteration++) {
      shared_counter2_g++;
      AsciiPrint ("APP2 Thread1 - CPU (%d): shared_counter2_g: %d\n", cp, shared_counter2_g);

      /* sleep while between counter increment*/ 
      ThreadSleep (100);

   }

   /* hold onto lock for much longer while sleeping to ensure that resource is locked for next thread*/ 
   ThreadSleep (500);

   UnLock(lock_apps2);
   release_time = GetTimerCountms ();
   AsciiPrint ("APP2 Thread1 - CPU (%d): Unlocks at time: %d ms\n", cp, release_time);

   cntr = get_counter2();
   AsciiPrint ("APP2 Thread1 - CPU (%d): get_counter2 returned %d\n", cp, cntr);
   inc_value = 100;
   set_counter2(inc_value);
   AsciiPrint ("APP2 Thread1 - CPU (%d): set_counter2() with inc_value %d.\n", cp, inc_value);
   cntr = get_counter2();
   AsciiPrint ("APP2 Thread1 - CPU (%d): get_counter2 (after set/inc by %d) is %d\n", cp, inc_value, cntr);

   AsciiPrint ("APP2 Thread1 - CPU(%d): Exiting.\n", cp);
   ThreadExit(0);

   return 0;
}

EFI_STATUS Lock_app2_test(VOID) {
   UINTN cp, next_cpu, previous_cpu;
   Thread *t;

   AsciiPrint("Testcase: Lock App2\n");

   t = GetCurrentThread();
   cp = ThreadGetCurrCpu(t);
   previous_cpu = cp;

   AsciiPrint("Main2: Initialize shared_counter2_g to %d\n", shared_counter2_g);
   shared_counter2_g = 0;

   /* Initialize private lock */
   AsciiPrint("Initialize  Lock MULTI_APPS (with ID: 1234).\n");
   lock_apps2 = InitLock("1234");

   /* Generate and setup Thread1 */
   mpt_apps2 = ThreadCreate ("LOCK_A2", &t_lock_app2, (void*)"LOCK_A2", DEFAULT_PRIORITY, DEFAULT_STACK_SIZE);
   AsciiPrint ("\tPrevious CPU: %d", previous_cpu);
   next_cpu = Get_Next_CPU_2(previous_cpu);
   previous_cpu = next_cpu;
   next_cpu = Get_Next_CPU_2(previous_cpu);
   previous_cpu = next_cpu;
   AsciiPrint ("\tNext CPU: %d\n", next_cpu);
   ThreadSetPinnedCpu (mpt_apps2, next_cpu);

  if (mpt_apps2 == NULL)
  {
    AsciiPrint("Unable to create thread (t_lock_app2), exiting ...\n");
    return EFI_UNSUPPORTED;
  }

  ThreadDetach (mpt_apps2);
  ThreadResume (mpt_apps2);

  /* core0 waits till both child threads exits */
  //ThreadSleep (15000);
  ThreadSleep (100);

  return EFI_SUCCESS;
   
}


int t_event_app2( IN VOID  *Arg)
{
   int cp;
   Thread *t;
   UINTN event_time_before;
   UINTN iteration, iteration_total;
   KStatus event_status;

   iteration_total = 100;

   t = GetCurrentThread();
   cp = ThreadGetCurrCpu(t);

   AsciiPrint ("Executing APP2 Thread1 (t_event_app2) with ID: %d on CPU: %d\n", t, cp);

   ThreadSleep (5000);

   /* send event to app1 */
   event_time_before = GetTimerCountms ();
   AsciiPrint ("APP2 Thread1 - CPU (%d): send event1 to APPS1 at time: %d ms\n", cp, event_time_before);
   event_status = EventSignal(event1_apps2, 1);

   ThreadSleep (1000);

   
   /* wait on event2 from app1 */
   event_status  = 1;
   iteration = 0;
   while ((event_status != 0) && (iteration < 10)){
      event_time_before = GetTimerCountms ();
      AsciiPrint ("APP2 Thread1 - CPU (%d) - iteration %d: wait on event2 from APPS1 at time: %d ms\n", 
                        cp, iteration, event_time_before);
      event_status = EventWaitTimeout(event2_apps2, 1000);
      iteration++;
   }


   /* update counter if event received */ 
   if (event_status == 0) {
      shared_counter2_g = 100;
      AsciiPrint ("\n\nAPP2 Thread1 - CPU (%d): APP2 got event at iteration count (%d).\n", cp, iteration);
      AsciiPrint ("APP2 Thread1 - CPU (%d): shared_counter2_g: %d\n", cp, shared_counter2_g);
   }
   else {
      AsciiPrint ("\n\nAPP2 Thread1 - CPU (%d): APP2 did not get event for iteration count (%d).\n", cp, iteration);
      AsciiPrint ("Test FAILED!\n");
      ThreadExit(EFI_D_ERROR);
      return EFI_D_ERROR; 
   }

   /* hold onto eventaphore for much longer while sleeping to ensure that resource is locked for next thread*/ 
   ThreadSleep (500);

   AsciiPrint ("APP2 Thread1 - CPU(%d): Exiting.\n", cp);
   ThreadExit(0);

   return 0;
}

EFI_STATUS Event_app2_test(VOID) {
   UINTN cp, next_cpu, previous_cpu;
   Thread *t;

   AsciiPrint("Testcase: Event App2\n");

   t = GetCurrentThread();
   cp = ThreadGetCurrCpu(t);
   previous_cpu = cp;

   AsciiPrint("Main2: Initialize shared_counter2_g to %d\n", shared_counter2_g);
   shared_counter2_g = 0;

   /* Initialize private lock */
   AsciiPrint("Initialize event1 and event2 MULTI_APPS (with ID: 1234, 5678).\n");
   // ID, initialStatus, autounsignal [1: regular (unsignal), 0: disable unsignal]
   event1_apps2 = EventInit(1234, 0, 1);
   event2_apps2 = EventInit(5678, 0, 1);

   /* Generate and setup Thread1 */
   mpt_apps2 = ThreadCreate ("EVENT_A2", &t_event_app2, (void*)"EVENT_A2", DEFAULT_PRIORITY, DEFAULT_STACK_SIZE);
   AsciiPrint ("\tPrevious CPU: %d", previous_cpu);
   next_cpu = Get_Next_CPU_2(previous_cpu);
   previous_cpu = next_cpu;
   next_cpu = Get_Next_CPU_2(previous_cpu);
   previous_cpu = next_cpu;
   AsciiPrint ("\tNext CPU: %d\n", next_cpu);
   ThreadSetPinnedCpu (mpt_apps2, next_cpu);

  if (mpt_apps2 == NULL)
  {
    AsciiPrint("Unable to create thread (t_event_app2), exiting ...\n");
    return EFI_UNSUPPORTED;
  }

  ThreadDetach (mpt_apps2);
  ThreadResume (mpt_apps2);

  /* core0 waits till both child threads exits */
  //ThreadSleep (15000);
  ThreadSleep (100);

  return EFI_SUCCESS;
   
}

int t_sem_app2( IN VOID  *Arg)
{
   int cp;
   Thread *t;
   UINTN cntr, inc_value;
   UINTN sem_time_before, sem_time_after, release_time;
   UINTN iteration, iteration_total;
   KStatus sem_status;

   iteration_total = 100;

   t = GetCurrentThread();
   cp = ThreadGetCurrCpu(t);

   AsciiPrint ("Executing APP2 Thread1 (t_sem_app2) with ID: %d on CPU: %d\n", t, cp);

   ThreadSleep (100);

   sem_time_before = GetTimerCountms ();
   AsciiPrint ("APP2 Thread1 - CPU (%d): before got semaphore at time: %d ms\n", cp, sem_time_before);
   sem_status = SemTryWait(sem_apps2);
   sem_time_after = GetTimerCountms ();
   AsciiPrint ("APP2 Thread1 - CPU (%d): after got semaphore at time: %d ms\n",  cp, sem_time_after);

   for (iteration=0; iteration<iteration_total; iteration++) {
      shared_counter2_g++;
      AsciiPrint ("APP2 Thread1 - CPU (%d): shared_counter2_g: %d\n", cp, shared_counter2_g);

      /* sleep while between counter increment*/ 
      ThreadSleep (100);

   }

   /* hold onto semaphore for much longer while sleeping to ensure that resource is locked for next thread*/ 
   ThreadSleep (500);

   SemPost(sem_apps2, TRUE);
   release_time = GetTimerCountms ();
   AsciiPrint ("APP2 Thread1 - CPU (%d): Semaphore posted at time: %d ms\n", cp, release_time);

   cntr = get_counter2();
   AsciiPrint ("APP2 Thread1 - CPU (%d): get_counter2 returned %d\n", cp, cntr);
   inc_value = 100;
   set_counter2(inc_value);
   AsciiPrint ("APP2 Thread1 - CPU (%d): set_counter2() with inc_value %d.\n", cp, inc_value);
   cntr = get_counter2();
   AsciiPrint ("APP2 Thread1 - CPU (%d): get_counter2 (after set/inc by %d) is %d\n", cp, inc_value, cntr);

   AsciiPrint ("APP2 Thread1 - CPU(%d): Exiting.\n", cp);
   ThreadExit(0);

   return 0;
}

EFI_STATUS Sem_app2_test(VOID) {
   UINTN cp, next_cpu, previous_cpu;
   Thread *t;

   AsciiPrint("Testcase: Semaphore App2\n");

   t = GetCurrentThread();
   cp = ThreadGetCurrCpu(t);
   previous_cpu = cp;

   AsciiPrint("Main2: Initialize shared_counter2_g to %d\n", shared_counter2_g);
   shared_counter2_g = 0;

   /* Initialize private lock */
   AsciiPrint("Initialize  Semaphore MULTI_APPS (with ID: 1234).\n");
   sem_apps2 = SemInit(1234, 1);

   /* Generate and setup Thread1 */
   mpt_apps2 = ThreadCreate ("SEM_A2", &t_sem_app2, (void*)"SEM_A2", DEFAULT_PRIORITY, DEFAULT_STACK_SIZE);
   AsciiPrint ("\tPrevious CPU: %d", previous_cpu);
   next_cpu = Get_Next_CPU_2(previous_cpu);
   previous_cpu = next_cpu;
   next_cpu = Get_Next_CPU_2(previous_cpu);
   previous_cpu = next_cpu;
   AsciiPrint ("\tNext CPU: %d\n", next_cpu);
   ThreadSetPinnedCpu (mpt_apps2, next_cpu);

  if (mpt_apps2 == NULL)
  {
    AsciiPrint("Unable to create thread (t_sem_app2), exiting ...\n");
    return EFI_UNSUPPORTED;
  }

  ThreadDetach (mpt_apps2);
  ThreadResume (mpt_apps2);

  /* core0 waits till both child threads exits */
  //ThreadSleep (15000);
  ThreadSleep (100);

  return EFI_SUCCESS;
   
}


EFI_STATUS Run_cpt_tests2(UINTN argc, CHAR8 **argv)
{
   AsciiPrint("Running CPT Test suite 2.\n");

   SetMem (&tc_config.suite, sizeof (tc_config.suite), 0);
   SetMem (&tc_config.category, sizeof (tc_config.category), 0);
   SetMem (&tc_config.tc, sizeof (tc_config.tc), 0);

   get_test_config2 (argc, argv);

   /* CPT Test suite */
   AsciiPrint("\ttc_config.suite: %a\ttc_config.category: %d\ttc_config.tc: %d\n",tc_config.suite, tc_config.category, tc_config.tc);

   if (tc_config.category == TC_CATEGORY_LOCK) {
      if (tc_config.tc == TC_TEST_APP2) {
         Lock_app2_test();
      }
   }
   else if (tc_config.category == TC_CATEGORY_EVENT) {
      if (tc_config.tc == TC_TEST_APP2) {
         Event_app2_test();
      }
      else if (tc_config.tc == TC_TEST_CLEAR) {
         clear_counter2();
      }
      else if (tc_config.tc == TC_TEST_SET) {
         set_counter2(1);
      }
      else if (tc_config.tc == TC_TEST_GET) {
         get_counter2();
         AsciiPrint("shared_counter2_g: %d\n", shared_counter2_g);
      }
   }
   else if (tc_config.category == TC_CATEGORY_SEM) {
      if (tc_config.tc == TC_TEST_APP2) {
         Sem_app2_test();
      }
   }
   else {
      AsciiPrint("No testcase specified in CPT Test Suite.\n");
   }


   return EFI_SUCCESS;
}


EFI_STATUS
EFIAPI
CPT_MpTest2Main ( IN EFI_HANDLE         ImageHandle,
             IN EFI_SYSTEM_TABLE   *SystemTable)
{
  EFI_STATUS                  Status = EFI_SUCCESS;
  UINTN                       Argc;
  CHAR8                     **Argv = NULL;
  CHAR8                     **TempArgv;

  Status = GetCmdLineArgs (ImageHandle, &Argc, &Argv);

  if (EFI_ERROR (Status))
    return Status;

  TempArgv = Argv;

  /* Check if the app was launched by shell and the Cmd str is actually the
   * Cmd app name itself, ie it would be "cmd" or "cmd.efi", then again process
   * to split, this is because Ebl doesn't pass the name of app, but shell does
   * other methods could yeild mixed results (like BDS launching apps via Menu, etc) */
  if (Argc &&
      ((AsciiStriCmp (Argv[0], "cpt_mptest2") == 0) ||
       (AsciiStriCmp (Argv[0], "cpt_mptest2.efi") == 0)))
  {
    --Argc;
    ++Argv;
  }

  /***************************************************************
   *    Run external test here in high perf mode
   **************************************************************/

   /* CPT Test suite */
   if (Argc && (AsciiStriCmp (Argv[0], "-s") == 0) && (AsciiStriCmp (Argv[1], "cpt") == 0) )
   {
      Run_cpt_tests2(Argc, Argv);
      
      AsciiPrint ("\r\ncpt_MpTest2 Driver returned...\r\n");
   }
   else {
        AsciiPrint ("\r\nPress any key to exit...\r\n");
        ReadAnyKey (NULL, 0);

        AsciiPrint ("\r\nExiting...\r\n");
   }

  return Status;
}

