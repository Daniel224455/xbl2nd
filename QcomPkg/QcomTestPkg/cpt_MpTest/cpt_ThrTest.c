/** @file cpt_ThrTest.c
   
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
#include <Library/QcomBaseLib.h>
#include <Library/QcomUtilsLib.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/KernelLib.h>
#include <Library/LockLib.h>

#define TEST_CONFIG_STR_LEN                  20
#define TEST_UNSIGNAL_NUM_OF_THREADS         10
#define TEST_MAX_NUM_OF_THREADS              400
#define TOTAL_CPU                            8

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
   TC_TEST_LIMIT =0xFFFF

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
  CHAR8 suite[TEST_CONFIG_STR_LEN];
  UINTN category;
  UINTN tc;
} test_config;

static test_config tc_config;

test_config *get_test_config (UINTN argc, CHAR8 **argv);

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

////////////////////////////////////////////////////////////

Thread *mpt;
Thread *mpt1;
Thread *mpt2;
Thread *mpt3;

/* counters used by a group of threads */
Thread *mpt_group[TEST_MAX_NUM_OF_THREADS];

VOID *lock_apps;
VOID *lock_private1;
VOID *lock_private2;
VOID *lock_single1;
VOID *lock_single2;
VOID *lock_recursive1;
VOID *lock_recursive2;
VOID *lock_recursive;
Semaphore* sem_apps;
Event* event1_apps1;
Event* event2_apps1;

static volatile UINTN shared_counter_g;
static volatile UINTN yield_count;
static volatile UINTN preempt_count;

/* counters used by a group of threads */
UINTN shared_counter_group_g[TEST_MAX_NUM_OF_THREADS];
UINTN shared_order_group_g[TEST_UNSIGNAL_NUM_OF_THREADS];
UINTN shared_order_ptr;

/* for cpu swapping */
UINTN shared_cpu_same_group_g[TEST_UNSIGNAL_NUM_OF_THREADS];
UINTN shared_cpu_diff_group_g[TEST_UNSIGNAL_NUM_OF_THREADS];
/* set used to check instantly */
UINTN shared_cpu_same_group_2_g[TEST_UNSIGNAL_NUM_OF_THREADS];
UINTN shared_cpu_diff_group_2_g[TEST_UNSIGNAL_NUM_OF_THREADS];

static volatile UINTN mutex_priority_compute_count1_g;
static volatile UINTN mutex_priority_compute_count2_g;
static volatile UINTN mutex_priority_compute_count3_g;
static volatile UINTN sem_priority_compute_count1_g;
static volatile UINTN sem_priority_compute_count2_g;
static volatile UINTN sem_priority_compute_count3_g;

typedef struct
{
   UINTN thread_index;
   UINTN cpu;
   CHAR8 thread_name[TEST_CONFIG_STR_LEN];
   UINTN initial_wait_ms;
   UINTN event_timeout_ms;
   UINTN compute_ms;
   UINTN release_ms;
} thread_config;

static thread_config thr_config[TEST_UNSIGNAL_NUM_OF_THREADS];

char thread_name[TEST_CONFIG_STR_LEN];

Mutex *mutex1;

Event *event1;
Event *event2;

Semaphore *sem1;

static volatile UINTN shared_handshake_stage_g;

///////////////////////////////////////////////////////////////////////////////

/* Uint to String */
EFI_STATUS uintToString(uint32 iNumber, char *pStringVal ) {
    uint32 iDigit;
    char cArray[10]; 
    uint32 iCount =0;
    uint32 iReverseCount = 0;

    if (NULL == pStringVal ) {
        AsciiPrint ("\nError: Destination Buffer Null\n");
        return EFI_D_ERROR;
    }
    while (iNumber) {
        iDigit = iNumber%10;
        iNumber = iNumber/10;
        cArray[iCount] = (char)(iDigit + 48);
        iCount++;
    }
    cArray[iCount] = '\0';
    while ( cArray[iReverseCount] != '\0') {
        *(pStringVal+iCount-(iReverseCount+1)) = cArray[iReverseCount];
        iReverseCount++;
    }
    *(pStringVal+iReverseCount)= '\0';
    return EFI_SUCCESS;
}

/* get_test_config: extract test arguments */
test_config *get_test_config (UINTN argc, CHAR8 **argv) {
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
               AsciiStrnCpy(tc_config.suite, "cpt", TEST_CONFIG_STR_LEN - 1);
            }
            else {
               //tc_config.suite = "dev";
               AsciiStrnCpy(tc_config.suite, "dev", TEST_CONFIG_STR_LEN - 1);
            }
            index++;
            break;

         case TEST_CATEGORY:

            if (AsciiStriCmp (*(argv + index), "lock") == 0) 
            {
               tc_config.category = TC_CATEGORY_LOCK;
            }
            else if (AsciiStriCmp (*(argv + index), "mutex") == 0) 
            {
               tc_config.category = TC_CATEGORY_MUTEX;
            }
            else if (AsciiStriCmp (*(argv + index), "event") == 0) 
            {
               tc_config.category = TC_CATEGORY_EVENT;
            }
            else if (AsciiStriCmp (*(argv + index), "sem") == 0) 
            {
               tc_config.category = TC_CATEGORY_SEM;
            }
            else if (AsciiStriCmp (*(argv + index), "cpu") == 0) 
            {
               tc_config.category = TC_CATEGORY_CPU;
            }
            else if (AsciiStriCmp (*(argv + index), "thread") == 0) 
            {
               tc_config.category = TC_CATEGORY_THREAD;
            }
            index++;
            break;

         case TEST_CASE:

            if (AsciiStriCmp (*(argv + index), "private") == 0) {
               tc_config.tc = TC_TEST_PRIVATE;
            }
            else if (AsciiStriCmp (*(argv + index), "single") == 0) {
               tc_config.tc = TC_TEST_SINGLE;
            }
            else if (AsciiStriCmp (*(argv + index), "long_str") == 0) {
               tc_config.tc = TC_TEST_LONG_STR;
            }

            else if (AsciiStriCmp (*(argv + index), "recursive") == 0)  {
               tc_config.tc = TC_TEST_RECURSIVE;
            }
            else if (AsciiStriCmp (*(argv + index), "app1") == 0)  {
               tc_config.tc = TC_TEST_APP1;
            }
            else if (AsciiStriCmp (*(argv + index), "app2") == 0)  {
               tc_config.tc = TC_TEST_APP2;
            }
            else if (AsciiStriCmp (*(argv + index), "clear") == 0)  {
               tc_config.tc = TC_TEST_CLEAR;
            }
            else if (AsciiStriCmp (*(argv + index), "set") == 0)  {
               tc_config.tc = TC_TEST_SET;
            }
            else if (AsciiStriCmp (*(argv + index), "get") == 0)  {
               tc_config.tc = TC_TEST_GET;
            }
            //////////////////// EVENT CATEGORY TESTS /////////////////
            else if (AsciiStriCmp (*(argv + index), "wait") == 0)  {
               tc_config.tc = TC_TEST_WAIT;
            }
            else if (AsciiStriCmp (*(argv + index), "create") == 0)  {
               tc_config.tc = TC_TEST_CREATE;
            }
            else if (AsciiStriCmp (*(argv + index), "timeout") == 0)  {
               tc_config.tc = TC_TEST_TIMEOUT;
            }
            else if (AsciiStriCmp (*(argv + index), "unsignal") == 0)  {
               tc_config.tc = TC_TEST_UNSIGNAL;
            }
            else if (AsciiStriCmp (*(argv + index), "unsig_reg") == 0)  {
               tc_config.tc = TC_TEST_UNSIG_REG;
            }
            else if (AsciiStriCmp (*(argv + index), "try") == 0)  {
               tc_config.tc = TC_TEST_TRY;
            }
            else if (AsciiStriCmp (*(argv + index), "priority_uneq") == 0)  {
               tc_config.tc = TC_TEST_PRIORITY_UNEQ;
            }
            else if (AsciiStriCmp (*(argv + index), "priority_eq") == 0)  {
               tc_config.tc = TC_TEST_PRIORITY_EQ;
            }
            else if (AsciiStriCmp (*(argv + index), "priority") == 0)  {
               tc_config.tc = TC_TEST_PRIORITY;
            }
            else if (AsciiStriCmp (*(argv + index), "name") == 0)  {
               tc_config.tc = TC_TEST_NAME;
            }
            else if (AsciiStriCmp (*(argv + index), "any") == 0)  {
               tc_config.tc = TC_TEST_ANY;
            }
            else if (AsciiStriCmp (*(argv + index), "current") == 0)  {
               tc_config.tc = TC_TEST_CURRENT;
            }
            else if (AsciiStriCmp (*(argv + index), "pin") == 0)  {
               tc_config.tc = TC_TEST_PIN;
            }
            else if (AsciiStriCmp (*(argv + index), "detach") == 0)  {
               tc_config.tc = TC_TEST_DETACH;
            }
            else if (AsciiStriCmp (*(argv + index), "yield") == 0)  {
               tc_config.tc = TC_TEST_YIELD;
            }
            else if (AsciiStriCmp (*(argv + index), "preempt") == 0)  {
               tc_config.tc = TC_TEST_PREEMPT;
            }
            else if (AsciiStriCmp (*(argv + index), "rc") == 0)  {
               tc_config.tc = TC_TEST_RC;
            }
            else if (AsciiStriCmp (*(argv + index), "max") == 0)  {
               tc_config.tc = TC_TEST_MAX;
            }
            else if (AsciiStriCmp (*(argv + index), "mp_en_all") == 0)  {
               tc_config.tc = TC_TEST_MP_EN_ALL;
            }
            else if (AsciiStriCmp (*(argv + index), "mp_en_each") == 0)  {
               tc_config.tc = TC_TEST_MP_EN_EACH;
            }
            else if (AsciiStriCmp (*(argv + index), "mp_en_last") == 0)  {
               tc_config.tc = TC_TEST_MP_EN_LAST;
            }
            else if (AsciiStriCmp (*(argv + index), "mp_en_two") == 0)  {
               tc_config.tc = TC_TEST_MP_EN_TWO;
            }
            //
            index++;
            break;

         case CMD_INVALID:

            index++;
            break;

      }  // switch CmdOp

   } // while argc 

   return &tc_config;
}

EFI_STATUS clear_counter() {

   shared_counter_g = 0;

   return EFI_SUCCESS;

}

EFI_STATUS set_counter(UINTN increment) {

   shared_counter_g += increment;

   return EFI_SUCCESS;

}

UINTN get_counter() {
   //AsciiPrint ("Thread (%d) - CPU (%d): got shared_counter_g %d\n", t, cp, shared_counter_g);
   return shared_counter_g;
}

UINTN Get_Next_CPU(UINTN current_cpu) {
   UINTN tmp_cpu;
   UINTN i;
   UINTN active_mask;
   UINTN last_active_cpu;
   UINTN active_cpu_set[TOTAL_CPU];
   UINTN active_cpu;
   //UINTN tmp1, tmp2;
   BOOLEAN found_next_cpu;
   last_active_cpu = 0;

   active_mask = MpcoreGetActiveMask();
   //AsciiPrint ("Active CPU Mask(%X)\n", active_mask);

   for (i=0;i<TOTAL_CPU;i++) {
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

int t_lock_private1( IN VOID  *Arg)
{
   int cp;
   Thread *t;
   UINTN cntr, inc_value;
   UINTN lock_time_before, lock_time_after, unlock_time;

   t = GetCurrentThread();
   cp = ThreadGetCurrCpu(t);

   AsciiPrint ("Executing Thread1 (t_lock_private1) with ID: %d on CPU: %d\n", t, cp);

   /* Initialize private lock */
   AsciiPrint("Thread1: Initializes lock with ID 0.\n");
   lock_private1 = InitLock(0);

   ThreadSleep (100);

   lock_time_before = GetTimerCountms ();
   AsciiPrint ("Thread1 (%d) - CPU (%d): before got lock at time: %d ms\n", t, cp, lock_time_before);

   /* update handshake stage for thread1 get lock */
   shared_handshake_stage_g = 1;
   Lock (lock_private1);
   lock_time_after = GetTimerCountms ();
   AsciiPrint ("Thread1 (%d) - CPU (%d): after got lock at time: %d ms\n", t, cp, lock_time_after);

   cntr = get_counter();
   AsciiPrint ("Thread1 (%d) - CPU (%d): shared_counter_g (before inc): %d\n", t, cp, cntr);
   inc_value = 1000;
   AsciiPrint ("Thread1 (%d) - CPU (%d): Increment counter by value value: %d\n", t, cp, inc_value);
   set_counter(inc_value);

   if (shared_counter_g != inc_value) {
      AsciiPrint ("Thread1: shared_counter_g is invalid (expected:%d,\tactual:%d).\n", inc_value, shared_counter_g);
      AsciiPrint ("Test FAILED!\n");
      ThreadExit(EFI_D_ERROR);
      return EFI_D_ERROR; 
   }

   /* hold onto lock for much longer while sleeping to ensure that resource is not locked for next thread.
    * Next thread can lock and unlock before current thread unlocks it. */ 
   ThreadSleep (500);

   /* check thread2 has got lock, before trying to unlock */
   if (shared_handshake_stage_g != 2) {
      AsciiPrint ("\n\nThread1: Thread2 does not have lock when trying to unlock.\n");
      AsciiPrint ("Test FAILED!\n");
      ThreadExit(EFI_D_ERROR);
      return EFI_D_ERROR; 
   }
   else {
      AsciiPrint ("Thread1: shared_handshake_stage_g(%d) - Thread2 has got lock.\n\n", shared_handshake_stage_g);

      if (shared_counter_g == inc_value) {
         AsciiPrint ("Thread1: shared_counter_g is invalid (not expected:%d,\tactual:%d).\n", inc_value, shared_counter_g);
         AsciiPrint ("Test FAILED!\n");
         ThreadExit(EFI_D_ERROR);
         return EFI_D_ERROR; 
      }
      else {
         AsciiPrint ("Thread1: shared_counter_g is %d.\n", shared_counter_g);
      }

   }

   UnLock (lock_private1);
   /* update handshake stage for thread1 release lock */
   shared_handshake_stage_g = 3;

   unlock_time = GetTimerCountms ();
   AsciiPrint ("Thread1 (%d) - CPU (%d): unlocked at time: %d ms\n", t, cp, unlock_time);

   cntr = get_counter();
   AsciiPrint ("Thread1 (%d) - CPU (%d): shared_counter_g (after inc): %d\n", t, cp, cntr);

   AsciiPrint ("Exiting Thread1 (t_lock_private1) with ID: %d on CPU: %d\n", t, cp);

  return 0;
}


int t_lock_private2( IN VOID  *Arg)
{
   int cp;
   Thread *t;
   UINTN cntr, inc_value;
   UINTN lock_time_before, lock_time_after, unlock_time;

   t = GetCurrentThread();
   cp = ThreadGetCurrCpu(t);

   AsciiPrint ("Executing Thread2 (t_lock_private2) with ID: %d on CPU: %d\n", t, cp);

   /* Initialize private lock */
   AsciiPrint("Thread2: Initializes lock with ID 0.\n");
   lock_private2 = InitLock(0);

   /* wait for thread2 to get lock first */
   ThreadSleep (200);

   lock_time_before = GetTimerCountms ();
   AsciiPrint ("Thread2 (%d) - CPU (%d): before got lock at time: %d ms\n", t, cp, lock_time_before);

   /* check thread1 got lock before thread2 trying to get lock */
   if (shared_handshake_stage_g != 1) {
      AsciiPrint ("\n\nThread2: Thread1 does not have lock when requesting lock.\n");
      AsciiPrint ("Test FAILED!\n");
      ThreadExit(EFI_D_ERROR);
      return EFI_D_ERROR; 
   }
   else {
      AsciiPrint ("Thread2: shared_handshake_stage_g(%d) - Thread1 got lock.\n\n", shared_handshake_stage_g);
   }

   /* get lock */
   shared_handshake_stage_g = 2;
   Lock (lock_private2);
   lock_time_after = GetTimerCountms ();
   AsciiPrint ("Thread2 (%d) - CPU (%d): after got lock at time: %d ms\n", t, cp, lock_time_after);

   cntr = get_counter();
   AsciiPrint ("Thread2 (%d) - CPU (%d): shared_counter_g (before inc): %d\n", t, cp, cntr);
   inc_value = 1;
   AsciiPrint ("Thread2 (%d) - CPU (%d): Increment counter by value value: %d\n", t, cp, inc_value);
   set_counter(inc_value);

   /* hold onto lock while sleeping */ 
   ThreadSleep (500);

   /* check thread1 has released lock, before trying to unlock */
   if (shared_handshake_stage_g != 3) {
      AsciiPrint ("\n\nThread2: Thread1 has not released lock when attempting to unlock.\n");
      AsciiPrint ("Test FAILED!\n");
      ThreadExit(EFI_D_ERROR);
      return EFI_D_ERROR; 
   }
   else {
      AsciiPrint ("Thread2: shared_handshake_stage_g(%d) - Thread1 has released lock.\n\n", shared_handshake_stage_g);
   }

   UnLock (lock_private2);
   /* update handshake stage for thread2 release lock */
   shared_handshake_stage_g = 4;

   unlock_time = GetTimerCountms ();
   AsciiPrint ("Thread2 (%d) - CPU (%d): unlocked at time: %d ms\n", t, cp, unlock_time);

   cntr = get_counter();
   AsciiPrint ("Thread2 (%d) - CPU (%d): shared_counter_g (after inc): %d\n", t, cp, cntr);

   AsciiPrint ("Exiting Thread2 (t_lock_private2) with ID: %d on CPU: %d\n", t, cp);

   return 0;
}


EFI_STATUS Lock_private_test(VOID) {
   UINTN cp, previous_cpu, next_cpu;
   Thread *t;

   AsciiPrint("Testcase: Lock Private\n");

   t = GetCurrentThread();
   cp = ThreadGetCurrCpu(t);
   previous_cpu = cp;

   shared_handshake_stage_g = 0;
   AsciiPrint("Initialize shared_handshake_stage_g to %d\n", shared_handshake_stage_g);

   AsciiPrint("Initialize shared_counter_g to %d\n", shared_counter_g);
   shared_counter_g = 0;

   /* Generate and setup Thread1 */
   mpt1 = ThreadCreate ("LOCK_P1", &t_lock_private1, (void*)"LOCK_P1", DEFAULT_PRIORITY, DEFAULT_STACK_SIZE);
   next_cpu = Get_Next_CPU(previous_cpu);
   AsciiPrint ("Previous CPU: %d Next CPU: %d\n", previous_cpu, next_cpu);
   previous_cpu = next_cpu;
   ThreadSetPinnedCpu (mpt1, next_cpu);

   if (mpt1 == NULL)
   {
      AsciiPrint("Unable to create thread (t_lock_private1), exiting ...\n");
      return EFI_UNSUPPORTED;
   }

   ThreadDetach (mpt1);
   ThreadResume (mpt1);

   /* Generate and setup Thread2 */
   mpt2 = ThreadCreate ("LOCK_P2", &t_lock_private2, (void*)"LOCK_P2", DEFAULT_PRIORITY, DEFAULT_STACK_SIZE);
   next_cpu = Get_Next_CPU(previous_cpu);
   AsciiPrint ("Previous CPU: %d Next CPU: %d\n", previous_cpu, next_cpu);
   previous_cpu = next_cpu;
   ThreadSetPinnedCpu (mpt2, next_cpu);

   if (mpt2 == NULL)
   {
      AsciiPrint("Unable to create thread (t_lock_private2), exiting ...\n");
      return EFI_UNSUPPORTED;
   }

   ThreadDetach (mpt2);
   ThreadResume (mpt2);

   /* main thead: waits till both child threads to exit */
   ThreadSleep (2000);

   /* check thread2 has released lock */
   if (shared_handshake_stage_g != 4) {
      AsciiPrint ("\n\nMain Thread: Thread2 has not released lock.\n");
      AsciiPrint ("Test FAILED!\n");
      ThreadExit(EFI_D_ERROR);
      return EFI_D_ERROR; 
   }
   else {
      AsciiPrint ("Main Thread: shared_handshake_stage_g(%d) - Thread2 has released lock.\n\n", shared_handshake_stage_g);
      AsciiPrint ("Test PASSED!\n");
   }

   ThreadSleep (200);
   return EFI_SUCCESS;
   
}


int t_lock_single1( IN VOID  *Arg)
{
   int cp;
   Thread *t;
   UINTN cntr, inc_value;
   UINTN lock_time_before, lock_time_after, unlock_time;

   t = GetCurrentThread();
   cp = ThreadGetCurrCpu(t);

   AsciiPrint ("Executing Thread1 (t_lock_single1) with ID: %d on CPU: %d\n", t, cp);

   /* Initialize private lock */
   AsciiPrint("Initialize single lock.\n");
   lock_single1 = InitLock("L_SINGLE");

   ThreadSleep (100);

   lock_time_before = GetTimerCountms ();
   AsciiPrint ("Thread1 (%d) - CPU (%d): before got lock at time: %d ms\n", t, cp, lock_time_before);

   /* update handshake stage for thread1 get lock */
   shared_handshake_stage_g = 1;
   Lock (lock_single1);
   lock_time_after = GetTimerCountms ();
   AsciiPrint ("Thread1 (%d) - CPU (%d): after got lock at time: %d ms\n", t, cp, lock_time_after);

   cntr = get_counter();
   AsciiPrint ("Thread1 (%d) - CPU (%d): shared_counter_g (before inc): %d\n", t, cp, cntr);
   inc_value = 1000;
   AsciiPrint ("Thread1 (%d) - CPU (%d): Increment counter by value value: %d\n", t, cp, inc_value);
   set_counter(inc_value);

   /* hold onto lock for much longer while sleeping to ensure that resource is locked for next thread*/ 
   ThreadSleep (500);

   /* check thread2 waiting to get lock, before trying to unlock */
   if (shared_handshake_stage_g != 2) {
      AsciiPrint ("\n\nThread1: Thread2 not waiting to get lock when trying to unlock.\n");
      AsciiPrint ("Test FAILED!\n");
      ThreadExit(EFI_D_ERROR);
      return EFI_D_ERROR; 
   }
   else {
      AsciiPrint ("Thread1: shared_handshake_stage_g(%d) - Thread2 is waiting on lock.\n", shared_handshake_stage_g);

      if (shared_counter_g != inc_value) {
         AsciiPrint ("Thread1: shared_counter_g is invalid (expected:%d,\tactual:%d).\n", inc_value, shared_counter_g);
         AsciiPrint ("Test FAILED!\n");
         ThreadExit(EFI_D_ERROR);
         return EFI_D_ERROR; 
      }
      else {
         AsciiPrint ("Thread1: shared_counter_g is %d.\n\n", shared_counter_g);
      }
}

   UnLock (lock_single1);
   /* update handshake stage for thread2 release lock */
   shared_handshake_stage_g = 3;
   unlock_time = GetTimerCountms ();
   AsciiPrint ("Thread1 (%d) - CPU (%d): unlocked at time: %d ms\n", t, cp, unlock_time);

   cntr = get_counter();
   AsciiPrint ("Thread1 (%d) - CPU (%d): shared_counter_g (after inc): %d\n", t, cp, cntr);

   AsciiPrint ("Exiting Thread1 (t_lock_single1) with ID: %d on CPU: %d\n", t, cp);

  return 0;
}


int t_lock_single2( IN VOID  *Arg)
{
   int cp;
   Thread *t;
   UINTN cntr, inc_value;
   UINTN lock_time_before, lock_time_after, unlock_time;

   t = GetCurrentThread();
   cp = ThreadGetCurrCpu(t);

   AsciiPrint ("Executing Thread2 (t_lock_single2) with ID: %d on CPU: %d\n", t, cp);

   /* Initialize private lock */
   AsciiPrint("Initialize single lock.\n");
   lock_single2 = InitLock("L_SINGLE");
 
   ThreadSleep (200);

   lock_time_before = GetTimerCountms ();
   AsciiPrint ("Thread2 (%d) - CPU (%d): before got lock at time: %d ms\n", t, cp, lock_time_before);

   /* check thread1 got lock before thread2 trying to get lock */
   if (shared_handshake_stage_g != 1) {
      AsciiPrint ("\n\nThread2: Thread1 does not have lock when requesting lock.\n");
      AsciiPrint ("Test FAILED!\n");
      ThreadExit(EFI_D_ERROR);
      return EFI_D_ERROR; 
   }
   else {
      AsciiPrint ("Thread2: shared_handshake_stage_g(%d) - Thread1 got lock.\n\n", shared_handshake_stage_g);
   }

   /* get lock */
   shared_handshake_stage_g = 2;
   Lock (lock_single2);

   /* check thread1 has released lock and thread2 got lock */
   if (shared_handshake_stage_g != 3) {
      AsciiPrint ("\n\nThread2: Thread1 has not released lock.\n");
      AsciiPrint ("Test FAILED!\n");
      ThreadExit(EFI_D_ERROR);
      return EFI_D_ERROR; 
   }
   else {
      AsciiPrint ("Thread2: shared_handshake_stage_g(%d) - Got lock after Thread1 released it.\n\n", shared_handshake_stage_g);
   }

   lock_time_after = GetTimerCountms ();
   AsciiPrint ("Thread2 (%d) - CPU (%d): after got lock at time: %d ms\n", t, cp, lock_time_after);

   cntr = get_counter();
   AsciiPrint ("Thread2 (%d) - CPU (%d): shared_counter_g (before inc): %d\n", t, cp, cntr);
   inc_value = 1;
   AsciiPrint ("Thread2 (%d) - CPU (%d): Increment counter by value value: %d\n", t, cp, inc_value);
   set_counter(inc_value);

   if (shared_counter_g != (cntr + inc_value)) {
      AsciiPrint ("Thread2: shared_counter_g is invalid (expected:%d,\tactual:%d).\n", (cntr + inc_value), shared_counter_g);
      AsciiPrint ("Test FAILED!\n");
      ThreadExit(EFI_D_ERROR);
      return EFI_D_ERROR; 
     }

   /* hold onto lock while sleeping */ 
   ThreadSleep (500);

   UnLock (lock_single2);
   /* update handshake stage for thread2 release lock */
   shared_handshake_stage_g = 4;
   unlock_time = GetTimerCountms ();
   AsciiPrint ("Thread2 (%d) - CPU (%d): unlocked at time: %d ms\n", t, cp, unlock_time);

   cntr = get_counter();
   AsciiPrint ("Thread2 (%d) - CPU (%d): shared_counter_g (after inc): %d\n", t, cp, cntr);

   AsciiPrint ("Exiting Thread2 (t_lock_single2) with ID: %d on CPU: %d\n", t, cp);

  return 0;
}


EFI_STATUS Lock_single_test(VOID) {
   UINTN cp, previous_cpu, next_cpu;
   Thread *t;

   AsciiPrint("Testcase: Lock Single\n");

   t = GetCurrentThread();
   cp = ThreadGetCurrCpu(t);
   previous_cpu = cp;

   shared_handshake_stage_g = 0;
   AsciiPrint("Initialize shared_handshake_stage_g to %d\n", shared_handshake_stage_g);

   AsciiPrint("Initialize shared_counter_g to %d\n", shared_counter_g);
   shared_counter_g = 0;

   /* Generate and setup Thread1 */
   mpt1 = ThreadCreate ("LOCK_S1", &t_lock_single1, (void*)"LOCK_S1", DEFAULT_PRIORITY, DEFAULT_STACK_SIZE);
   next_cpu = Get_Next_CPU(previous_cpu);
   AsciiPrint ("Previous CPU: %d Next CPU: %d\n", previous_cpu, next_cpu);
   previous_cpu = next_cpu;
   ThreadSetPinnedCpu (mpt1, next_cpu);

   if (mpt1 == NULL)
   {
      AsciiPrint("Unable to create thread (t_lock_single1), exiting ...\n");
      return EFI_UNSUPPORTED;
   }

   ThreadDetach (mpt1);
   ThreadResume (mpt1);

   /* Generate and setup Thread2 */
   mpt2 = ThreadCreate ("LOCK_S2", &t_lock_single2, (void*)"LOCK_S2", DEFAULT_PRIORITY, DEFAULT_STACK_SIZE);
   next_cpu = Get_Next_CPU(previous_cpu);
   AsciiPrint ("Previous CPU: %d Next CPU: %d\n", previous_cpu, next_cpu);
   previous_cpu = next_cpu;
   ThreadSetPinnedCpu (mpt2, next_cpu);

   if (mpt2 == NULL)
   {
      AsciiPrint("Unable to create thread (t_lock_single2), exiting ...\n");
      return EFI_UNSUPPORTED;
   }

   ThreadDetach (mpt2);
   ThreadResume (mpt2);

   /* main thead: waits till both child threads to exit */
   ThreadSleep (2000);

   /* check thread2 has released lock */
   if (shared_handshake_stage_g != 4) {
      AsciiPrint ("\n\nMain Thread: Thread2 did not release lock.\n");
      AsciiPrint ("Test FAILED!\n");
      ThreadExit(EFI_D_ERROR);
      return EFI_D_ERROR; 
   }
   else {
      AsciiPrint ("Main Thread: shared_handshake_stage_g(%d) - Thread2 released lock.\n\n", shared_handshake_stage_g);
      AsciiPrint ("Test PASSED!\n");
   }

   ThreadSleep (200);
   return EFI_SUCCESS;
}

int t_lock_string1( IN VOID  *Arg)
{
   int cp;
   Thread *t;
   UINTN lock_time_before, lock_time_after;
   thread_config* thr_cfg = (thread_config*)Arg;
   UINTN thread_index = (thr_cfg->thread_index);

   t = GetCurrentThread();
   cp = ThreadGetCurrCpu(t);

   AsciiPrint ("Executing Thread1 (t_lock_string1) on CPU: %d\n", cp);

   ThreadSleep (100);

   /* update handshake state for recursive lock started */
   lock_time_before = GetTimerCountms ();
   shared_handshake_stage_g = 1;
   AsciiPrint ("Thread1 - CPU (%d): Execution started and about to get lock at time %d ms (shared_handshake_stage_g - %d)\n", 
                        cp, lock_time_before, shared_handshake_stage_g);


   /* update handshake state for recursive lock started */
   lock_time_before = GetTimerCountms ();
   AsciiPrint ("Thread1 - CPU (%d): Execution started and about to get lock at time: %d ms\n", cp, lock_time_before);

   /* get lock */
   Lock (lock_single1);

   lock_time_after = GetTimerCountms ();
   shared_handshake_stage_g = 2;
   AsciiPrint ("Thread1 - CPU (%d): after got lock at time %d ms (shared_handshake_stage_g - %d)\n", 
                     cp, lock_time_after, shared_handshake_stage_g);

   shared_counter_group_g[thread_index] = 1;

   /* hold onto lock while sleeping */ 
   ThreadSleep (200);

   AsciiPrint ("Exiting Thread1 (t_lock_string1) on CPU: %d\n", cp);

  return 0;
}



int t_lock_string2( IN VOID  *Arg)
{
   int cp;
   Thread *t;
   UINTN lock_time_before, lock_time_after;
   thread_config* thr_cfg = (thread_config*)Arg;
   UINTN thread_index = (thr_cfg->thread_index);

   t = GetCurrentThread();
   cp = ThreadGetCurrCpu(t);

   AsciiPrint ("Executing Thread2 (t_lock_string2) with ID: %d on CPU: %d\n", t, cp);

   ThreadSleep (500);

   /* check thread1 got lock before thread2 trying to get lock */
   if (shared_handshake_stage_g != 2) {
      AsciiPrint ("\n\nThread2: Thread1 did not get lock yet.\n");
      AsciiPrint ("Test FAILED!\n");
      ThreadExit(EFI_D_ERROR);
      return EFI_D_ERROR; 
   }
   else {
      AsciiPrint ("\n\nThread2: Thread1 got lock yet.\n");
   }

   lock_time_before = GetTimerCountms ();
   AsciiPrint ("Thread2 - CPU (%d): before got lock at time: %d ms\n", cp, lock_time_before);


   /* get lock */
   Lock (lock_single2);

   lock_time_after = GetTimerCountms ();
   AsciiPrint ("Thread2 - CPU (%d): after got lock at time: %d ms\n", cp, lock_time_after);

   shared_counter_group_g[thread_index] = 1;

   /* hold onto lock while sleeping */ 
   ThreadSleep (200);

   AsciiPrint ("Exiting Thread2 (t_lock_string2) on CPU: %d\n", cp);

  return 0;
}


EFI_STATUS Lock_long_string_test(VOID) {
   UINTN cp, previous_cpu, next_cpu;
   Thread *t;
   KStatus thread_status1, thread_status2;
   int thread_rc1, thread_rc2;
   UINTN thr_index;

   AsciiPrint("Testcase: Lock String Long\n");

   t = GetCurrentThread();
   cp = ThreadGetCurrCpu(t);
   previous_cpu = cp;

   AsciiPrint ("Main Thread: Test thread running on CPU(%d)\n", cp);

   /* initialize handshake flag */
   shared_handshake_stage_g  = 0;

   /* Initialize private lock */
   AsciiPrint("Initialize lock with string1 (LOCK_STRING1) and string2 (LOCK_STRING2).\n");
   lock_single1 = InitLock("LOCK_STRING1");
   lock_single2 = InitLock("LOCK_STRING2");
   //lock_single1 = InitLock("LOCK1");
   //lock_single2 = InitLock("LOCK2");

   /* Generate and setup Thread1 */
   thr_index = 0; 
   shared_counter_group_g[thr_index] = 0;
   thr_config[thr_index].thread_index = thr_index;
   mpt1 = ThreadCreate ("LOCK_S1", &t_lock_string1, (void*)&(thr_config[thr_index]), DEFAULT_PRIORITY, DEFAULT_STACK_SIZE);
   next_cpu = Get_Next_CPU(previous_cpu);
   AsciiPrint ("Previous CPU: %d Next CPU: %d\n", previous_cpu, next_cpu);
   previous_cpu = next_cpu;
   ThreadSetPinnedCpu (mpt1, next_cpu);

   if (mpt1 == NULL)
   {
      AsciiPrint("Unable to create thread (t_lock_string1), exiting ...\n");
      return EFI_UNSUPPORTED;
   }

   /* Generate and setup Thread2 */
   thr_index = 1; 
   shared_counter_group_g[thr_index] = 0;
   thr_config[thr_index].thread_index = thr_index;
   mpt2 = ThreadCreate ("LOCK_S2", &t_lock_string2, (void*)&(thr_config[thr_index]), DEFAULT_PRIORITY, DEFAULT_STACK_SIZE);
   next_cpu = Get_Next_CPU(previous_cpu);
   AsciiPrint ("Previous CPU: %d Next CPU: %d\n", previous_cpu, next_cpu);
   previous_cpu = next_cpu;
   ThreadSetPinnedCpu (mpt2, next_cpu);

   if (mpt2 == NULL)
   {
      AsciiPrint("Unable to create thread (t_lock_string2), exiting ...\n");
      return EFI_UNSUPPORTED;
   }

   ThreadResume (mpt1);
   ThreadResume (mpt2);

   ThreadSleep (200);

   thread_status1 = ThreadJoin(mpt1, &thread_rc1, 1000);
   thread_status2 = ThreadJoin(mpt2, &thread_rc2, 1000);

   /* main thead: waits till both child threads to exit */
   ThreadSleep (200);


   /* check thread1 got lock */
   if (shared_counter_group_g[0] == 0) {
      AsciiPrint ("\n\nMain Thread: Thread1 did not get lock to increment counter.\n");
      AsciiPrint ("Test FAILED!\n");
      ThreadExit(EFI_D_ERROR);
      return EFI_D_ERROR; 
   }
   else {
      AsciiPrint ("\n\nMain Thread: Thread1 got lock and incremented counter.\n");
   }

   /* check thread2 got lock */
   if (shared_counter_group_g[1] != 0) {
      AsciiPrint ("\n\nMain Thread: Thread2 got lock to increment counter (unexpected).\n");
      AsciiPrint ("Test FAILED!\n");
      ThreadExit(EFI_D_ERROR);
      return EFI_D_ERROR; 
   }
   else {
      AsciiPrint ("\n\nMain Thread: Thread2 did not get lock to increment counter.\n");
   }

   //UnLock (lock_single1);
   //AsciiPrint ("\n\nMain Thread: Unlocked lock_single1.\n");

   ThreadSleep (500);
   //UnLock (lock_single2);
   //AsciiPrint ("\n\nMain Thread: Unlocked lock_single2.\n");
   
   AsciiPrint("\nMain Thread - shared_counter_group_g:\n");
   for (thr_index=0; thr_index < 2; thr_index++) {
      AsciiPrint("\t\tshared_counter_group_g[%d] = %d\n", thr_index, shared_counter_group_g[thr_index]);
   }

   AsciiPrint ("\n\nMain Thread: Thr1 - thread_status(%d)\tthread_rc(%d)\n", thread_status1, thread_rc1);
   if ((thread_status1 != 0) || (thread_rc1 != 0)) {
      AsciiPrint ("Test FAILED!\n");
      return EFI_D_ERROR;
   }

   AsciiPrint ("\nMain Thread: Thr2 - thread_status(%d)\tthread_rc(%d)\n", thread_status2, thread_rc2);
   if ((thread_status2 == 0)  || (thread_rc2 == 0)) {
      AsciiPrint ("Test FAILED!\n");
      return EFI_D_ERROR;
   }

   AsciiPrint ("Test PASSED!\n");

   ThreadSleep (200);
   return EFI_SUCCESS;
}


int t_lock_recursive1( IN VOID  *Arg)
{
   int cp;
   Thread *t;
   UINTN cntr, inc_value;
   UINTN lock_time_before, lock_time_after, unlock_time;
   int lock_count, unlock_count, lock_total;

   lock_total = 5;

   t = GetCurrentThread();
   cp = ThreadGetCurrCpu(t);

   AsciiPrint ("Executing Thread1 (t_lock_recursive1) with ID: %d on CPU: %d\n", t, cp);

   ThreadSleep (100);

   /* update handshake state for recursive lock started */
   shared_handshake_stage_g = 1;

   for (lock_count=1; lock_count<=lock_total;lock_count++) {

      lock_time_before = GetTimerCountms ();
      AsciiPrint ("Thread1 (%d) - CPU (%d): Lock_count (%d) before got lock at time: %d ms\n", t, cp, lock_count, lock_time_before);
      Lock (lock_recursive);
      lock_time_after = GetTimerCountms ();
      AsciiPrint ("Thread1 (%d) - CPU (%d): Lock_count (%d) after got lock at time: %d ms\n", t, cp, lock_count, lock_time_after);

      cntr = get_counter();
      AsciiPrint ("Thread1 (%d) - CPU (%d): shared_counter_g (before inc): %d\n", t, cp, cntr);
      inc_value = 1000;
      AsciiPrint ("Thread1 (%d) - CPU (%d): Increment counter by value value: %d\n", t, cp, inc_value);
      set_counter(inc_value);

      if (lock_count == 1) {
         /* for first time getting lock, hold longer since want thread2 to start and tryto get lock */
         ThreadSleep (500);
      }
      else {
         ThreadSleep (100);
      }
   }

   /* check thread1 has released lock and thread2 got lock */
   if (shared_handshake_stage_g != 2) {
      AsciiPrint ("\n\nThread1: Thread2 not trying to get lock.\n");
      AsciiPrint ("Test FAILED!\n");
      ThreadExit(EFI_D_ERROR);
      return EFI_D_ERROR; 
   }
   else {
      AsciiPrint ("Thread1: shared_handshake_stage_g(%d) - Thread2 trying to get lock.\n\n", shared_handshake_stage_g);
   }   

   /* update handshake state for got all recursive lock */
   shared_handshake_stage_g = 3;

   for (unlock_count=1; unlock_count<=lock_total;unlock_count++) {
      /* hold onto lock for little while*/ 
      ThreadSleep (100);

      UnLock (lock_recursive);
      unlock_time = GetTimerCountms ();
      AsciiPrint ("Thread1 (%d) - CPU (%d): Unlock_count (%d) unlocked at time: %d ms\n", t, cp,  unlock_count, unlock_time);

      cntr = get_counter();
      AsciiPrint ("Thread1 (%d) - CPU (%d): shared_counter_g (after inc): %d\n", t, cp, cntr);
   }

   /* update handshake state for unlock done */
   shared_handshake_stage_g = 4;

   ThreadSleep (200);
   AsciiPrint ("Exiting Thread1 (t_lock_recursive1) with ID: %d on CPU: %d\n", t, cp);
   ThreadExit(0);
   return 0;
}


int t_lock_recursive2( IN VOID  *Arg)
{
   int cp;
   Thread *t;
   UINTN cntr, inc_value;
   UINTN lock_time_before, lock_time_after, unlock_time;

   t = GetCurrentThread();
   cp = ThreadGetCurrCpu(t);

   AsciiPrint ("Executing Thread2 (t_lock_recursive2) with ID: %d on CPU: %d\n", t, cp);

   ThreadSleep (200);

   lock_time_before = GetTimerCountms ();
   AsciiPrint ("Thread2 (%d) - CPU (%d): before got lock at time: %d ms\n", t, cp, lock_time_before);

  /* check thread1 recursive lock has started but not ended */
   if (shared_handshake_stage_g != 1) {
      AsciiPrint ("\n\nThread2: Not in stage - Recursive lock has not started.\n");
      AsciiPrint ("Test FAILED!\n");
      ThreadExit(EFI_D_ERROR);
      return EFI_D_ERROR; 
   }
   else {
      AsciiPrint ("Thread2: shared_handshake_stage_g(%d) - Recursive locking started.\n\n", shared_handshake_stage_g);
   }


   /* update handshake stage for thread2 trying to get lock */
   shared_handshake_stage_g = 2;

   Lock (lock_recursive);
   lock_time_after = GetTimerCountms ();
   AsciiPrint ("Thread2 (%d) - CPU (%d): after got lock at time: %d ms\n", t, cp, lock_time_after);

   cntr = get_counter();
   AsciiPrint ("Thread2 (%d) - CPU (%d): shared_counter_g (before inc): %d\n", t, cp, cntr);
   inc_value = 1;
   AsciiPrint ("Thread2 (%d) - CPU (%d): Increment counter by value value: %d\n", t, cp, inc_value);
   set_counter(inc_value);

   /* hold onto lock while sleeping */ 
   ThreadSleep (500);

   UnLock (lock_recursive);

   /* check thread1 has released all locks */
   if (shared_handshake_stage_g != 4) {
      AsciiPrint ("\n\nThread2: Thread1 has not released all locks.\n");
      AsciiPrint ("Test FAILED!\n");
      ThreadExit(EFI_D_ERROR);
      return EFI_D_ERROR; 
   }
   else {
      AsciiPrint ("Thread2: shared_handshake_stage_g(%d) - Got lock after Thread1 released all.\n\n", shared_handshake_stage_g);
   }

   unlock_time = GetTimerCountms ();
   AsciiPrint ("Thread2 (%d) - CPU (%d): unlocked at time: %d ms\n", t, cp, unlock_time);

   cntr = get_counter();
   AsciiPrint ("Thread2 (%d) - CPU (%d): shared_counter_g (after inc): %d\n", t, cp, cntr);

   AsciiPrint ("Exiting Thread2 (t_lock_recursive2) with ID: %d on CPU: %d\n", t, cp);

  return 0;
}

EFI_STATUS Lock_recursive_test(VOID) {
   UINTN cp, previous_cpu, next_cpu;
   Thread *t;
   KStatus thread_status1, thread_status2;
   int thread_rc1, thread_rc2;

   AsciiPrint("Testcase: Lock Recursive\n");

   t = GetCurrentThread();
   cp = ThreadGetCurrCpu(t);
   previous_cpu = cp;

   AsciiPrint("Initialize shared_counter_g to %d\n", shared_counter_g);
   shared_counter_g = 0;

   /* Initialize private lock */
   AsciiPrint("Initialize recursive lock.\n");
   lock_recursive = InitRecursiveLock("LOCK_RECURSIVE");

   /* Generate and setup Thread1 */
   mpt1 = ThreadCreate ("LOCK_R1", &t_lock_recursive1, (void*)"LOCK_R1", DEFAULT_PRIORITY, DEFAULT_STACK_SIZE);
   next_cpu = Get_Next_CPU(previous_cpu);
   AsciiPrint ("Previous CPU: %d Next CPU: %d\n", previous_cpu, next_cpu);
   previous_cpu = next_cpu;
   ThreadSetPinnedCpu (mpt1, next_cpu);

   if (mpt1 == NULL)
   {
      AsciiPrint("Unable to create thread (t_lock_recursive1), exiting ...\n");
      return EFI_UNSUPPORTED;
   }

   //ThreadDetach (mpt1);
   ThreadResume (mpt1);

   /* Generate and setup Thread2 */
   mpt2 = ThreadCreate ("LOCK_R2", &t_lock_recursive2, (void*)"LOCK_R2", DEFAULT_PRIORITY, DEFAULT_STACK_SIZE);
   next_cpu = Get_Next_CPU(previous_cpu);
   AsciiPrint ("Previous CPU: %d Next CPU: %d\n", previous_cpu, next_cpu);
   previous_cpu = next_cpu;
   ThreadSetPinnedCpu (mpt2, next_cpu);

   if (mpt2 == NULL)
   {
      AsciiPrint("Unable to create thread (t_lock_recursive2), exiting ...\n");
      return EFI_UNSUPPORTED;
   }

   //ThreadDetach (mpt2);
   ThreadResume (mpt2);

   thread_status1 = ThreadJoin(mpt1, &thread_rc1, 10000);
   thread_status2 = ThreadJoin(mpt2, &thread_rc2, 10000);
   AsciiPrint ("\nMain Thread: Thr1 - thread_status(%d)\tthread_rc(%d)\n", thread_status1, thread_rc1);
   AsciiPrint ("\nMain Thread: Thr2 - thread_status(%d)\tthread_rc(%d)\n", thread_status2, thread_rc2);
   if ((thread_status1 != 0)  || (thread_rc1 != 0) || (thread_status2 != 0)  || (thread_rc2 != 0)) {
      AsciiPrint ("Test FAILED!\n");
      return EFI_D_ERROR;
   }
   else
   {
      AsciiPrint ("Test PASSED!\n");
   }

   ThreadSleep (200);
   return EFI_SUCCESS;
   
}



int t_lock_app1( IN VOID  *Arg)
{
   int cp;
   Thread *t;
   UINTN lock_time_before, lock_time_after, release_time;

   t = GetCurrentThread();
   cp = ThreadGetCurrCpu(t);

   AsciiPrint ("Executing APP1 Thread1 (t_lock_app1) with ID: %d on CPU: %d\n", t, cp);

   ThreadSleep (300);
   
   lock_time_before = GetTimerCountms ();
   AsciiPrint ("APP1 Thread1 - CPU (%d): before Lock at time: %d ms\n", cp, lock_time_before);
   Lock(lock_apps);
   lock_time_after = GetTimerCountms ();

   AsciiPrint ("APP1 Thread1 - CPU (%d): got lock  at time: %d ms\n",  
                  cp, lock_time_after);

   shared_counter_g = 1000;
   AsciiPrint ("APP2 Thread1 - CPU (%d): shared_counter_g: %d\n", cp, shared_counter_g);

   ThreadSleep (200);

   UnLock(lock_apps);
   release_time = GetTimerCountms ();
   AsciiPrint ("APP1 Thread1 - CPU (%d): Lock posted at time: %d ms\n", cp, release_time);

   ThreadSleep (50);

   AsciiPrint ("APP1 Thread1 - CPU(%d): Exiting.\n", cp);
   ThreadExit(0);
   return 0;
}



EFI_STATUS Lock_app1_test(VOID) {
   UINTN cp, next_cpu, previous_cpu;
   Thread *t;
   KStatus thread_status1;
   int thread_rc1;


   AsciiPrint("Testcase: Lock App1\n");

   t = GetCurrentThread();
   cp = ThreadGetCurrCpu(t);
   previous_cpu = cp;

   AsciiPrint("Initialize shared_counter_g to %d\n", shared_counter_g);
   shared_counter_g = 0;

   /* Initialize private lock */
   AsciiPrint("Initialize lock MULTI_APPS (with ID: 1234).\n");
   lock_apps = InitLock("1234");

   /* Generate and setup Thread1 */
   mpt1 = ThreadCreate ("LOCK_A1", &t_lock_app1, (void*)"LOCK_A1", DEFAULT_PRIORITY, DEFAULT_STACK_SIZE);
   AsciiPrint ("\tPrevious CPU: %d", previous_cpu);
   next_cpu = Get_Next_CPU(previous_cpu);
   previous_cpu = next_cpu;
   AsciiPrint ("\tNext CPU: %d\n", next_cpu);
   ThreadSetPinnedCpu (mpt1, next_cpu);

   if (mpt1 == NULL)
   {
      AsciiPrint("Unable to create thread (t_lock_app1), exiting ...\n");
      return EFI_UNSUPPORTED;
   }

   ThreadResume (mpt1);

   ThreadSleep (100);

   thread_status1 = ThreadJoin(mpt1, &thread_rc1, 50000);
   AsciiPrint ("\nMain Thread: Thr1 - thread_status(%d)\tthread_rc(%d)\n", thread_status1, thread_rc1);
   if ((thread_status1 != 0)  || (thread_rc1 != 0))  {
      AsciiPrint ("Test FAILED!\n");
      return EFI_D_ERROR;
   }

   if (shared_counter_g  != 1000) {
      AsciiPrint ("\nMain Thread: Thr1 - Apps1 DID not get lock to update shared_counter_g  (shared_counter_g = %d).\n", shared_counter_g);
      AsciiPrint ("Test FAILED!\n");
      return EFI_D_ERROR;
      
   }
   else {
      AsciiPrint ("\nMain Thread: Thr1 - Apps1 got lock and updated shared_counter_g to %d.\n", shared_counter_g);
      AsciiPrint ("\nTest PASSED!\n");
   }

   return EFI_SUCCESS;
   
}





int t_event_app1( IN VOID  *Arg)
{
   int cp;
   Thread *t;
   UINTN event_time_before;
   UINTN retry, iteration_total;
   KStatus event_status;

   iteration_total = 100;

   t = GetCurrentThread();
   cp = ThreadGetCurrCpu(t);

   AsciiPrint ("Executing APP1 Thread1 (t_event_app1) with ID: %d on CPU: %d\n", t, cp);

   ThreadSleep (300);
   
   /* wait on event1 from app2 */
   event_status  = 1;
   retry = 0;
   while (event_status != 0) {
      event_time_before = GetTimerCountms ();
      AsciiPrint ("APP1 Thread1 - CPU (%d) - retry %d: wait on event1 from APPS2 at time: %d ms\n", 
                        cp, retry, event_time_before);
      event_status = EventWaitTimeout(event1_apps1, 200);

      /* update retry count only if did not get event and need to retry */
      if (event_status != 0) {
         retry++;
      }
   }

   /* to confirm that event was not raised prior to initiating wait request, need retry count to be > 0) */
   if (retry == 0) {
      /* retry >0: Confirms that APP1 was waiting for APP2 to raise event.  
       *           Also confirms that on creation of the object, event was not raised) 
       */
      AsciiPrint ("\n\nAPP1 Thread1: APP1 did not wait on event (event was valid on request).\n");
      AsciiPrint ("Test FAILED!\n");
      ThreadExit(EFI_D_ERROR);
      return EFI_D_ERROR; 
 
   }


   /* update coutenr if event received */
   if (event_status == 0) {
      shared_counter_g = 100;
      AsciiPrint ("APP1 Thread1 - CPU (%d): shared_counter_g: %d\n", cp, shared_counter_g);
   }

   ThreadSleep (100);

   /* send event to app2 */
   event_time_before = GetTimerCountms ();
   AsciiPrint ("APP1 Thread1 - CPU (%d): send event2 to APPS2 at time: %d ms\n", cp, event_time_before);
   event_status = EventSignal(event2_apps1, 1);

   ThreadSleep (100);

   AsciiPrint ("APP1 Thread1 - CPU(%d): Exiting.\n", cp);
   ThreadExit(0);
   return 0;
}



EFI_STATUS Event_app1_test(VOID) {
   UINTN cp, next_cpu, previous_cpu;
   Thread *t;
   KStatus thread_status1;
   int thread_rc1;


   AsciiPrint("Testcase: Event App1\n");

   t = GetCurrentThread();
   cp = ThreadGetCurrCpu(t);
   previous_cpu = cp;

   AsciiPrint("Initialize shared_counter_g to %d\n", shared_counter_g);
   shared_counter_g = 0;

   /* Initialize private event */
   AsciiPrint("Initialize event1 and event2 MULTI_APPS (with ID: 1234, 5678).\n");
   event1_apps1 = EventInit(1234,0,1);
   event2_apps1 = EventInit(5678,0,1);

   /* Generate and setup Thread1 */
   mpt1 = ThreadCreate ("EVENT_A1", &t_event_app1, (void*)"EVENT_A1", DEFAULT_PRIORITY, DEFAULT_STACK_SIZE);
   AsciiPrint ("\tPrevious CPU: %d", previous_cpu);
   next_cpu = Get_Next_CPU(previous_cpu);
   previous_cpu = next_cpu;
   AsciiPrint ("\tNext CPU: %d\n", next_cpu);
   ThreadSetPinnedCpu (mpt1, next_cpu);

   if (mpt1 == NULL)
   {
      AsciiPrint("Unable to create thread (t_event_app1), exiting ...\n");
      return EFI_UNSUPPORTED;
   }

   ThreadResume (mpt1);

   ThreadSleep (100);

   thread_status1 = ThreadJoin(mpt1, &thread_rc1, 20000);
   AsciiPrint ("\nMain Thread: Thr1 - thread_status(%d)\tthread_rc(%d)\n", thread_status1, thread_rc1);
   if ((thread_status1 != 0)  || (thread_rc1 != 0))  {
      AsciiPrint ("Test FAILED!\n");
      return EFI_D_ERROR;
   }

   if (shared_counter_g  != 100) {
      AsciiPrint ("\nMain Thread: Thr1 - Apps1 DID not get event to update shared_counter_g  (shared_counter_g = %d).\n", shared_counter_g);
      AsciiPrint ("Test FAILED!\n");
      return EFI_D_ERROR;
      
   }
   else {
      AsciiPrint ("\nMain Thread: Thr1 - Apps1 got event and updated shared_counter_g to %d.\n", shared_counter_g);
      AsciiPrint ("\nTest PASSED!\n");
   }

   return EFI_SUCCESS;
   
}




int t_sem_app1( IN VOID  *Arg)
{
   int cp;
   Thread *t;
   UINTN sem_time_before, sem_time_after, release_time;
   UINTN iteration, iteration_total;
   KStatus sem_status;

   iteration_total = 100;

   t = GetCurrentThread();
   cp = ThreadGetCurrCpu(t);

   AsciiPrint ("Executing APP1 Thread1 (t_sem_app1) with ID: %d on CPU: %d\n", t, cp);

   ThreadSleep (300);
   
   iteration = 0;
   //while ((iteration < iteration_total) && (shared_counter_g == 0)) {
   while (shared_counter_g == 0) {

      sem_time_before = GetTimerCountms ();
      AsciiPrint ("APP1 Thread1 - CPU (%d): before SemTryWait (iteration=%d) at time: %d ms\n", 
                     cp, iteration, sem_time_before);
      sem_status = SemTryWait(sem_apps);
      sem_time_after = GetTimerCountms ();

      if (sem_status == 0) {

         if (iteration < 10) {
            AsciiPrint ("APP1 Thread1 - CPU (%d): got semaphore but iteration (%d) is too low at time: %d ms\n",  
                           cp, iteration, sem_time_after);
            AsciiPrint ("APP1 Thread1 - CPU (%d): Was APPS2 started before APPS1???\n");  

            /* break without updating counter so test result check will fail */
            break; 
         }

         AsciiPrint ("APP1 Thread1 - CPU (%d): got semaphore (iteration=%d) at time: %d ms\n",  
                        cp, iteration, sem_time_after);

         shared_counter_g = 1000;
         AsciiPrint ("APP2 Thread1 - CPU (%d): shared_counter_g: %d\n", cp, shared_counter_g);

         ThreadSleep (200);

         SemPost(sem_apps, TRUE);
         release_time = GetTimerCountms ();
         AsciiPrint ("APP1 Thread1 - CPU (%d): Semaphore posted at time: %d ms\n", cp, release_time);

      }
      else {
         AsciiPrint ("APP1 Thread1 - CPU (%d): DID NOT get semaphore (iteration=%d) at time: %d ms\n",  
                        cp, iteration, sem_time_after);
      }

      iteration++;

      ThreadSleep (50);

   } // while 

   AsciiPrint ("APP1 Thread1 - CPU(%d): Exiting.\n", cp);
   ThreadExit(0);
   return 0;
}



EFI_STATUS Sem_app1_test(VOID) {
   UINTN cp, next_cpu, previous_cpu;
   Thread *t;
   KStatus thread_status1;
   int thread_rc1;


   AsciiPrint("Testcase: Semaphore App1\n");

   t = GetCurrentThread();
   cp = ThreadGetCurrCpu(t);
   previous_cpu = cp;

   AsciiPrint("Initialize shared_counter_g to %d\n", shared_counter_g);
   shared_counter_g = 0;

   /* Initialize private semphore */
   AsciiPrint("Initialize semaphore MULTI_APPS (with ID: 1234).\n");
   sem_apps = SemInit(1234,1);

   /* Generate and setup Thread1 */
   mpt1 = ThreadCreate ("SEM_A1", &t_sem_app1, (void*)"SEM_A1", DEFAULT_PRIORITY, DEFAULT_STACK_SIZE);
   AsciiPrint ("\tPrevious CPU: %d", previous_cpu);
   next_cpu = Get_Next_CPU(previous_cpu);
   previous_cpu = next_cpu;
   AsciiPrint ("\tNext CPU: %d\n", next_cpu);
   ThreadSetPinnedCpu (mpt1, next_cpu);

   if (mpt1 == NULL)
   {
      AsciiPrint("Unable to create thread (t_sem_app1), exiting ...\n");
      return EFI_UNSUPPORTED;
   }

   ThreadResume (mpt1);

   ThreadSleep (100);

   //thread_status1 = ThreadJoin(mpt1, &thread_rc1, 20000);  // napali
   thread_status1 = ThreadJoin(mpt1, &thread_rc1, 100000);   // hana x5
   AsciiPrint ("\nMain Thread: Thr1 - thread_status(%d)\tthread_rc(%d)\n", thread_status1, thread_rc1);
   if ((thread_status1 != 0)  || (thread_rc1 != 0))  {
      AsciiPrint ("Test FAILED!\n");
      return EFI_D_ERROR;
   }

   if (shared_counter_g  != 1000) {
      AsciiPrint ("\nMain Thread: Thr1 - Apps1 DID not get semaphore to update shared_counter_g  (shared_counter_g = %d).\n", shared_counter_g);
      AsciiPrint ("Test FAILED!\n");
      return EFI_D_ERROR;
      
   }
   else {
      AsciiPrint ("\nMain Thread: Thr1 - Apps1 got semaphore and updated shared_counter_g to %d.\n", shared_counter_g);
      AsciiPrint ("\nTest PASSED!\n");
   }

   return EFI_SUCCESS;
   
}



int t_mutex_timeout1( IN VOID  *Arg)
{
   int cp;
   Thread *t;
   UINTN mutex1_time_before, mutex1_time_after;
   KStatus mutex_status;
   UINTN cntr, inc_value;
   UINTN sleep_time;
   UINTN iteration_index;
   UINTN compute_count;

   t = GetCurrentThread();
   cp = ThreadGetCurrCpu(t);

   AsciiPrint ("Executing Thread1 (t_mutex_timeout1) on CPU: %d\n", cp);

   ThreadSleep (100);

   mutex1_time_before = GetTimerCountms ();
   AsciiPrint ("Thread1 - CPU (%d): before getting mutex at time: %d ms\n", cp, mutex1_time_before);

   /* get mutex1 */
   mutex_status = MutexAcquireTimeout(mutex1,500);
   shared_handshake_stage_g = 1;
   mutex1_time_after = GetTimerCountms ();
   if (mutex_status != 0) {
      AsciiPrint ("Thread1 - CPU (%d): DID NOT get mutex first time at time: %d ms\n", cp, mutex1_time_after);
      AsciiPrint ("Test FAILED!\n");
      ThreadExit(EFI_D_ERROR);
      return EFI_D_ERROR; 
   }
   AsciiPrint ("Thread1 - CPU (%d): got mutex at time: %d ms (shared_handshake_stage_g = %d)\n", cp, mutex1_time_after, shared_handshake_stage_g);

   /* check if mutex is held */
   mutex_status = IsMutexHeld(mutex1);
   if (mutex_status != 1) {
      AsciiPrint ("Thread1 - CPU (%d): Mutex status shows NOT held at shared_handshake_stage_g (%d).\n", cp, shared_handshake_stage_g );
      AsciiPrint ("Test FAILED!\n");
      ThreadExit(EFI_D_ERROR);
      return EFI_D_ERROR; 
   }
   else {
      AsciiPrint ("Thread1 - CPU (%d): Mutex status shows held at shared_handshake_stage_g (%d).\n", cp, shared_handshake_stage_g);
   }

   cntr = get_counter();
   AsciiPrint ("Thread1 - CPU (%d): shared_counter_g (before inc): %d\n", cp, cntr);
   inc_value = 1000;
   AsciiPrint ("Thread1 - CPU (%d): Increment counter by value value: %d\n", cp, inc_value);
   set_counter(inc_value);

   if (shared_counter_g != inc_value) {
      AsciiPrint ("Thread1: shared_counter_g is invalid (expected:%d,\tactual:%d).\n", inc_value, shared_counter_g);
      AsciiPrint ("Test FAILED!\n");
      ThreadExit(EFI_D_ERROR);
      return EFI_D_ERROR; 
   }

   compute_count = 2000;
   for (iteration_index = 0; iteration_index < compute_count; iteration_index++) {
      if (iteration_index % 50 == 0) {
         AsciiPrint ("Thread1 on CPU (%d): iteration - %d (i mod 50 = 0)\n", cp, iteration_index);
      }
   }

   sleep_time = 800;
   AsciiPrint ("Thread1 - CPU (%d): Enter sleep for %d ms\n", cp, sleep_time);
   ThreadSleep (sleep_time);

   /* check thread2 requesting for mutex1 the second time */
   if (shared_handshake_stage_g != 3) {
      AsciiPrint ("\n\nThread1: Thread2 did not requested mutex the second time yet.\n");
      AsciiPrint ("Test FAILED!\n");
      ThreadExit(EFI_D_ERROR);
      return EFI_D_ERROR; 
   }
   else {
      AsciiPrint ("\n\nThread1: Thread2 did requested mutex the second time.\n");
   }

   /* release mutex */
   shared_handshake_stage_g = 4;
   mutex1_time_after = GetTimerCountms ();
   AsciiPrint ("Thread1 - CPU (%d): About to release mutex1 at time: %d ms (shared_handshake_stage_g = %d)\n", cp, mutex1_time_after, shared_handshake_stage_g);
   mutex_status = MutexRelease(mutex1);
   if (mutex_status != 0) {
      AsciiPrint ("Thread1 - CPU (%d): Mutex not released successfully.\n", cp);
      AsciiPrint ("Test FAILED!\n");
      ThreadExit(EFI_D_ERROR);
      return EFI_D_ERROR; 
   }

   /* check if mutex is released */
   mutex_status = IsMutexHeld(mutex1);
   if (mutex_status == 1) {
      AsciiPrint ("Thread1 - CPU (%d): Mutex status does NOT show released.\n", cp);
      AsciiPrint ("Test FAILED!\n");
      ThreadExit(EFI_D_ERROR);
      return EFI_D_ERROR; 
   }

   AsciiPrint ("Handshake Stage[%d]: Thread1 released mutex.\n", shared_handshake_stage_g);

   ThreadSleep (200);
   AsciiPrint ("Exiting Thread1 (t_mutex_timeout1) on CPU: %d\n", cp);
   ThreadExit(0);
   return 0;
}


int t_mutex_timeout2( IN VOID  *Arg)
{
   int cp;
   Thread *t;
   UINTN mutex1_time_before, mutex1_time_after;
   KStatus mutex_status;

   t = GetCurrentThread();
   cp = ThreadGetCurrCpu(t);

   AsciiPrint ("Executing Thread2 (t_mutex_wait2) on CPU: %d\n", cp);


   ThreadSleep (200);


/* check thread1 got mutex before thread2 trying to get lock */
   if (shared_handshake_stage_g != 1) {
      AsciiPrint ("\n\nThread2 - CPU(%d): Thread1 does not have mutex when requesting lock.\n", cp);
      AsciiPrint ("Test FAILED!\n");
      ThreadExit(EFI_D_ERROR);
      return EFI_D_ERROR; 
   }
   else {
      AsciiPrint ("Thread2 - CPU(%d): Thread1 got mutex.\n\n", cp);
   }


   mutex1_time_before = GetTimerCountms ();
   shared_handshake_stage_g = 2;
   AsciiPrint ("Thread2 - CPU (%d): wait on mutex1 first time at: %d ms (shared_handshake_stage_g = %d)\n", 
               cp, mutex1_time_before, shared_handshake_stage_g);


   mutex_status = MutexAcquireTimeout(mutex1,600);
   mutex1_time_after = GetTimerCountms ();

   if (mutex_status == 0) {
      AsciiPrint ("Thread2 - CPU (%d): Got mutex (not expected) first time at: %d ms\n", cp, mutex1_time_after);
      AsciiPrint ("Thread2 - CPU (%d): Mutex not expected to be obtained first time at: %d ms\n", cp, mutex1_time_after);
      AsciiPrint ("Test FAILED!\n");
      ThreadExit(EFI_D_ERROR);
      return EFI_D_ERROR; 
   }
   else {
      AsciiPrint ("Thread2 - CPU (%d): Did not get mutex (as expected) first time at: %d ms\n", cp, mutex1_time_after);
   }

   /* check thread1 has not send mutex1 */
   if (shared_handshake_stage_g == 4) {
      AsciiPrint ("\n\nThread2: Thread1 released mutex first time.\n");
      AsciiPrint ("Test FAILED!\n");
      ThreadExit(EFI_D_ERROR);
      return EFI_D_ERROR; 
   }
   else {
      AsciiPrint ("\n\nThread2: Timed out since Thread1 has not released mutex first time.\n");
   }

   mutex1_time_before = GetTimerCountms ();
   shared_handshake_stage_g = 3;
   AsciiPrint ("Thread2 - CPU (%d): wait on mutex1 second time at: %d ms (shared_handshake_stage_g = %d)\n", cp, mutex1_time_before, shared_handshake_stage_g);
   mutex_status = MutexAcquireTimeout(mutex1,500);
   mutex1_time_after = GetTimerCountms ();

   /* check thread1 has not send mutex1 */
   if (shared_handshake_stage_g != 4) {
      AsciiPrint ("\n\nThread2: Thread1 has not released mutex.\n");
      AsciiPrint ("Test FAILED!\n");
      ThreadExit(EFI_D_ERROR);
      return EFI_D_ERROR; 
   }
   else {
      AsciiPrint ("\n\nThread2: Thread1 released mutex.\n");
   }

   if (mutex_status != 0) {
      AsciiPrint ("Thread2 - CPU (%d): DID NOT get mutex1 second time at: %d ms\n", cp, mutex1_time_after);
      AsciiPrint ("Test FAILED!\n");
      ThreadExit(EFI_D_ERROR);
      return EFI_D_ERROR; 
   }
   else {
      AsciiPrint ("Thread2 - CPU (%d): got mutex1 second time at: %d ms\n", cp, mutex1_time_after);
   }

   /* check if mutex is held */
   mutex_status = IsMutexHeld(mutex1);
   if (mutex_status != 1) {
      AsciiPrint ("Thread2 - CPU (%d): Mutex status shows NOT held.\n", cp);
      AsciiPrint ("Test FAILED!\n");
      ThreadExit(EFI_D_ERROR);
      return EFI_D_ERROR; 
   }

   ThreadSleep (200);
   mutex_status = MutexRelease(mutex1);
   if (mutex_status != 0) {
      AsciiPrint ("Thread2 - CPU (%d): Mutex not released successfully.\n", cp);
      AsciiPrint ("Test FAILED!\n");
      ThreadExit(EFI_D_ERROR);
      return EFI_D_ERROR; 
   }


   /* check if mutex is released */
   mutex_status = IsMutexHeld(mutex1);
   if (mutex_status == 1) {
      AsciiPrint ("Thread2 - CPU (%d): Mutex status does NOT show released.\n", cp);
      AsciiPrint ("Test FAILED!\n");
      ThreadExit(EFI_D_ERROR);
      return EFI_D_ERROR; 
   }

   ThreadSleep (200);
   AsciiPrint ("Exiting Thread2 (t_mutex_timeout2) on CPU: %d\n", cp);
   ThreadExit(0);
   return 0;
}


EFI_STATUS Mutex_timeout_test(VOID) {
   UINTN cp, previous_cpu, next_cpu;
   Thread *t;
   KStatus thread_status1, thread_status2;
   int thread_rc1, thread_rc2;

   AsciiPrint("Testcase: Mutex Timeout\n");

   t = GetCurrentThread();
   cp = ThreadGetCurrCpu(t);
   previous_cpu = cp;

   shared_handshake_stage_g = 0;
   AsciiPrint("Initialize shared_handshake_stage_g to %d\n", shared_handshake_stage_g);

   AsciiPrint("Initialize shared_counter_g to %d\n", shared_counter_g);
   shared_counter_g = 0;

   /* Initialize mutex */
   AsciiPrint("Initialize mutex of non-zero ID (9876).\n");
   mutex1 = MutexInit(9876);

   /* Generate and setup Thread1 */
   //mpt1 = ThreadCreate ("MUTEX_T1", &t_mutex_timeout1, (void*)"MUTEX_T1", DEFAULT_PRIORITY+4, DEFAULT_STACK_SIZE);
   mpt1 = ThreadCreate ("MUTEX_T1", &t_mutex_timeout1, (void*)"MUTEX_T1", DEFAULT_PRIORITY, DEFAULT_STACK_SIZE);

   next_cpu = Get_Next_CPU(previous_cpu);
   AsciiPrint ("Previous CPU: %d Next CPU: %d\n", previous_cpu, next_cpu);
   previous_cpu = next_cpu;
   ThreadSetPinnedCpu (mpt1, next_cpu);

   if (mpt1 == NULL)
   {
      AsciiPrint("Unable to create thread (t_mutex_timeout1), exiting ...\n");
      return EFI_UNSUPPORTED;
   }

   //ThreadDetach (mpt1);
   ThreadResume (mpt1);

   /* Generate and setup Thread2 */
   //mpt2 = ThreadCreate ("MUTEX_T2", &t_mutex_timeout2, (void*)"MUTEX_T2", DEFAULT_PRIORITY+4, DEFAULT_STACK_SIZE);
   mpt2 = ThreadCreate ("MUTEX_T2", &t_mutex_timeout2, (void*)"MUTEX_T2", DEFAULT_PRIORITY, DEFAULT_STACK_SIZE);
   next_cpu = Get_Next_CPU(previous_cpu);
   AsciiPrint ("Previous CPU: %d Next CPU: %d\n", previous_cpu, next_cpu);
   previous_cpu = next_cpu;
   ThreadSetPinnedCpu (mpt2, next_cpu);

   if (mpt2 == NULL)
   {
      AsciiPrint("Unable to create thread (t_mutex_timeout2), exiting ...\n");
      return EFI_UNSUPPORTED;
   }

   //ThreadDetach (mpt2);
   ThreadResume (mpt2);

   /* wait for threads to exit and join */
   thread_status1 = ThreadJoin(mpt1, &thread_rc1, 900000);
   thread_status2 = ThreadJoin(mpt2, &thread_rc2, 900000);

   /* destroy mutex that was initialized */
   MutexDestroy(mutex1);

   /* check pass/fail status */
   AsciiPrint ("\nMain Thread: Thr1 - thread_status(%d)\tthread_rc(%d)\n", thread_status1, thread_rc1);
   AsciiPrint ("\nMain Thread: Thr2 - thread_status(%d)\tthread_rc(%d)\n", thread_status2, thread_rc2);
   if ((thread_status1 != 0)  || (thread_rc1 != 0) || (thread_status2 != 0)  || (thread_rc2 != 0)) {
      AsciiPrint ("Test FAILED!\n");
      return EFI_D_ERROR;
   }
   else
   {
      AsciiPrint ("Test PASSED!\n");
   }

   ThreadSleep (200);
   return EFI_SUCCESS;
   
}




int t_event_wait1( IN VOID  *Arg)
{
   int cp;
   Thread *t;
   UINTN event1_time_before, event1_time_after;

   t = GetCurrentThread();
   cp = ThreadGetCurrCpu(t);

   AsciiPrint ("Executing Thread1 (t_event_wait1) with ID: %d on CPU: %d\n", t, cp);


   ThreadSleep (300);

   event1_time_before = GetTimerCountms ();
   AsciiPrint ("Thread1 (%d) - CPU (%d): before sending event at time: %d ms\n", t, cp, event1_time_before);

   /* send first event1 after timeout */
   shared_handshake_stage_g = 1;
   EventSignal(event1, 0);
   event1_time_after = GetTimerCountms ();
   AsciiPrint ("Thread1 (%d) - CPU (%d): after sent event at time: %d ms\n", t, cp, event1_time_after);


   ThreadSleep (200);
   AsciiPrint ("Exiting Thread1 (t_event_timeout1) with ID: %d on CPU: %d\n", t, cp);
   ThreadExit(0);
   return 0;
}


int t_event_wait2( IN VOID  *Arg)
{
   int cp;
   Thread *t;
   UINTN event1_time_before, event1_time_after;
   KStatus event_status;

   t = GetCurrentThread();
   cp = ThreadGetCurrCpu(t);

   AsciiPrint ("Executing Thread2 (t_event_wait2) with ID: %d on CPU: %d\n", t, cp);


   ThreadSleep (100);

  /* check thread1 has not send event1, before waiting on signal */
   if (shared_handshake_stage_g == 1) {
      AsciiPrint ("\n\nThread1 event send before Thread2 wait.\n");
      AsciiPrint ("Test FAILED!\n");
      ThreadExit(EFI_D_ERROR);
      return EFI_D_ERROR; 
   }

   event1_time_before = GetTimerCountms ();
   AsciiPrint ("Thread1 (%d) - CPU (%d): wait on event1 at time: %d ms\n", t, cp, event1_time_before);
   event_status = EventWait(event1);

   event1_time_after = GetTimerCountms ();
   if (event_status == 0) {
      AsciiPrint ("Thread1 (%d) - CPU (%d): did get signal for event1 first time at time: %d ms\n", 
                     t, cp, event1_time_after);
   }
   else {
      AsciiPrint ("Thread1 (%d) - CPU (%d): DID NOT get signal for event1 first time at time: %d ms\n", 
                        t, cp, event1_time_after);
      AsciiPrint ("Test FAILED!\n");
      ThreadExit(EFI_D_ERROR);
      return EFI_D_ERROR; 
   }

   ThreadSleep (200);
   AsciiPrint ("Exiting Thread2 (t_event_wait2) with ID: %d on CPU: %d\n", t, cp);
   ThreadExit(0);
   return 0;
}


EFI_STATUS Event_wait_test(VOID) {
   UINTN cp, previous_cpu, next_cpu;
   Thread *t;
   BOOLEAN event_init_status;
   KStatus thread_status1, thread_status2;
   int thread_rc1, thread_rc2;

   AsciiPrint("Testcase: Event Wait\n");

   t = GetCurrentThread();
   cp = ThreadGetCurrCpu(t);
   previous_cpu = cp;

   shared_handshake_stage_g = 0;
   AsciiPrint("Initialize shared_handshake_stage_g to %d\n", shared_handshake_stage_g);

   AsciiPrint("Initialize shared_counter_g to %d\n", shared_counter_g);
   shared_counter_g = 0;

   /* Initialize private lock of regular type (autounsignal on)*/
   AsciiPrint("Initialize event.\n");
   event1 = EventInit(0, 0, 1);
   event_init_status = EventInitialized(event1);
   if (event_init_status == 1) {
      AsciiPrint ("\tevent_init_status: %d\n", event_init_status);
   }
   else {
      AsciiPrint ("\tevent_init_status: %d\n", event_init_status);
      AsciiPrint ("Test FAILED!\n");
      return 0;
   }

   /* Generate and setup Thread1 */
   mpt1 = ThreadCreate ("EVENT_W1", &t_event_wait1, (void*)"EVENT_W1", DEFAULT_PRIORITY, DEFAULT_STACK_SIZE);

   next_cpu = Get_Next_CPU(previous_cpu);
   AsciiPrint ("Previous CPU: %d Next CPU: %d\n", previous_cpu, next_cpu);
   previous_cpu = next_cpu;
   ThreadSetPinnedCpu (mpt1, next_cpu);

   if (mpt1 == NULL)
   {
      AsciiPrint("Unable to create thread (t_event_wait1), exiting ...\n");
      return EFI_UNSUPPORTED;
   }

   //ThreadDetach (mpt1);
   ThreadResume (mpt1);

   /* Generate and setup Thread2 */
   mpt2 = ThreadCreate ("EVENT_W2", &t_event_wait2, (void*)"EVENT_W2", DEFAULT_PRIORITY, DEFAULT_STACK_SIZE);

   next_cpu = Get_Next_CPU(previous_cpu);
   AsciiPrint ("Previous CPU: %d Next CPU: %d\n", previous_cpu, next_cpu);
   previous_cpu = next_cpu;
   ThreadSetPinnedCpu (mpt2, next_cpu);


   if (mpt2 == NULL)
   {
      AsciiPrint("Unable to create thread (t_event_wait2), exiting ...\n");
      return EFI_UNSUPPORTED;
   }

   //ThreadDetach (mpt2);
   ThreadResume (mpt2);

   /* wait for threads to exit and join */
   thread_status1 = ThreadJoin(mpt1, &thread_rc1, 10000);
   thread_status2 = ThreadJoin(mpt2, &thread_rc2, 10000);

   /* destroy event that was initialized */
   EventDestroy(event1);

   /* check pass/fail status */
   AsciiPrint ("\nMain Thread: Thr1 - thread_status(%d)\tthread_rc(%d)\n", thread_status1, thread_rc1);
   AsciiPrint ("\nMain Thread: Thr2 - thread_status(%d)\tthread_rc(%d)\n", thread_status2, thread_rc2);
   if ((thread_status1 != 0)  || (thread_rc1 != 0) || (thread_status2 != 0)  || (thread_rc2 != 0)) {
      AsciiPrint ("Test FAILED!\n");
      return EFI_D_ERROR;
   }
   else
   {
      AsciiPrint ("Test PASSED!\n");
   }

   ThreadSleep (200);
   return EFI_SUCCESS;
   
}



int t_event_wait_on_creation1( IN VOID  *Arg)
{
   int cp;
   Thread *t;
   UINTN event1_time_before, event1_time_after;
   KStatus event_status;

   t = GetCurrentThread();
   cp = ThreadGetCurrCpu(t);

   AsciiPrint ("Executing Thread1 (t_event_wait_on_creation1) with ID: %d on CPU: %d\n", t, cp);

   ThreadSleep (100);

   /* first time wait */
   event1_time_before = GetTimerCountms ();
   AsciiPrint ("Thread1 (%d) - CPU (%d): wait on event1 at time: %d ms\n", t, cp, event1_time_before);
   event_status = EventWait(event1);

   if (event_status == 0) {
      AsciiPrint ("Thread1 (%d) - CPU (%d): did get signal for event1 first time at time: %d ms\n", 
                     t, cp, event1_time_before);
   }
   else {
      AsciiPrint ("Thread1 (%d) - CPU (%d): DID NOT get signal for event1 first time at time: %d ms\n", 
                        t, cp, event1_time_before);
      AsciiPrint ("Test FAILED!\n");
      ThreadExit(EFI_D_ERROR);
      return EFI_D_ERROR; 
   }

   ThreadSleep (100);

   /* second time wait */
   event1_time_before = GetTimerCountms ();
   AsciiPrint ("Thread1 - CPU (%d): wait on event1 second time at time: %d ms\n", cp, event1_time_before);
   event_status = EventWaitTimeout(event1, 500);

   event1_time_after = GetTimerCountms ();
   if (event_status != 0) {
      AsciiPrint ("Thread1 - CPU (%d): DID NOT get signal for event1 second time (timed out) with event_status(%d) at time: %d ms.\n", 
                     cp, event_status, event1_time_after);
   }
   else {
      AsciiPrint ("Thread1 - CPU (%d): DID get signal for event1 second time with event_status(%d) at time: %d ms.\n", 
                     cp, event_status, event1_time_after);
      AsciiPrint ("Thread1 - CPU (%d): Did not timeout as expected.\n", cp); 
      AsciiPrint ("Test FAILED!\n");
      ThreadExit(EFI_D_ERROR);
      return EFI_D_ERROR; 
   }

   ThreadSleep (200);
   AsciiPrint ("Exiting Thread1 (t_event_wait_on_creation1) with ID: %d on CPU: %d\n", t, cp);
   ThreadExit(0);
   return 0;
}


EFI_STATUS Event_wait_on_creation_test(VOID) {
   UINTN cp, previous_cpu, next_cpu;
   Thread *t;
   BOOLEAN event_init_status;
   KStatus thread_status1;
   int thread_rc1;

   AsciiPrint("Testcase: Event Create\n");

   t = GetCurrentThread();
   cp = ThreadGetCurrCpu(t);
   previous_cpu = cp;

   shared_handshake_stage_g = 0;
   AsciiPrint("Initialize shared_handshake_stage_g to %d\n", shared_handshake_stage_g);

   AsciiPrint("Initialize shared_counter_g to %d\n", shared_counter_g);
   shared_counter_g = 0;

   /* Initialize private lock of regular type (autounsignal on) while setting signal*/
   AsciiPrint("Initialize event.\n");
   event1 = EventInit(0, 1, 1);
   event_init_status = EventInitialized(event1);
   if (event_init_status == 1) {
      AsciiPrint ("\tevent_init_status: %d\n", event_init_status);
   }
   else {
      AsciiPrint ("\tevent_init_status: %d\n", event_init_status);
      AsciiPrint ("Test FAILED!\n");
      return 0;
   }

   /* Generate and setup Thread1 */
   mpt1 = ThreadCreate ("EVENT_W1", &t_event_wait_on_creation1, (void*)"EVENT_W1", DEFAULT_PRIORITY, DEFAULT_STACK_SIZE);

   next_cpu = Get_Next_CPU(previous_cpu);
   AsciiPrint ("Previous CPU: %d Next CPU: %d\n", previous_cpu, next_cpu);
   previous_cpu = next_cpu;
   ThreadSetPinnedCpu (mpt1, next_cpu);

   if (mpt1 == NULL)
   {
      AsciiPrint("Unable to create thread (t_event_wait_on_creation1), exiting ...\n");
      return EFI_UNSUPPORTED;
   }

   //ThreadDetach (mpt1);
   ThreadResume (mpt1);


   /* wait for threads to exit and join */
   thread_status1 = ThreadJoin(mpt1, &thread_rc1, 10000);

   /* destroy event that was initialized */
   EventDestroy(event1);

   /* check pass/fail status */
   AsciiPrint ("\nMain Thread: Thr1 - thread_status(%d)\tthread_rc(%d)\n", thread_status1, thread_rc1);
   if ((thread_status1 != 0)  || (thread_rc1 != 0) ) {
      AsciiPrint ("Test FAILED!\n");
      return EFI_D_ERROR;
   }
   else
   {
      AsciiPrint ("Test PASSED!\n");
   }

   ThreadSleep (200);
   return EFI_SUCCESS;
   
}



int t_event_timeout1( IN VOID  *Arg)
{
   int cp;
   Thread *t;
   UINTN event1_time_before, event1_time_after;

   t = GetCurrentThread();
   cp = ThreadGetCurrCpu(t);

   AsciiPrint ("Executing Thread1 (t_event_timeout1) with ID: %d on CPU: %d\n", t, cp);


   ThreadSleep (1000);

   event1_time_before = GetTimerCountms ();
   AsciiPrint ("Thread1 (%d) - CPU (%d): before sending event at time: %d ms\n", t, cp, event1_time_before);

   /* send first event1 after timeout */
   shared_handshake_stage_g = 1;
   EventSignal(event1, 0);
   event1_time_after = GetTimerCountms ();
   AsciiPrint ("Thread1 (%d) - CPU (%d): after sent event at time: %d ms\n", t, cp, event1_time_after);

   ThreadSleep (1000);

   /* no need to send event again, since already send event is not received yet */
   //event1_time_before = GetTimerCountms ();
   //AsciiPrint ("Thread1 (%d) - CPU (%d): before sending 2nd event at time: %d ms\n", t, cp, event1_time_before);
   //EventSignal(event1, 0);
   //event1_time_after = GetTimerCountms ();
   //AsciiPrint ("Thread1 (%d) - CPU (%d): after sent 2nd event at time: %d ms\n", t, cp, event1_time_after);


   ThreadSleep (200);
   AsciiPrint ("Exiting Thread1 (t_event_timeout1) with ID: %d on CPU: %d\n", t, cp);
   ThreadExit(0);
   return 0;
}


int t_event_timeout2( IN VOID  *Arg)
{
   int cp;
   Thread *t;
   UINTN event1_time_before, event1_time_after;
   KStatus event_status;

   t = GetCurrentThread();
   cp = ThreadGetCurrCpu(t);

   AsciiPrint ("Executing Thread2 (t_event_wait2) with ID: %d on CPU: %d\n", t, cp);


   ThreadSleep (100);

   /* check thread1 has not send event1, before waiting on signal */
   if (shared_handshake_stage_g == 1) {
      AsciiPrint ("\n\nThread1 event send before Thread2 wait.\n");
      AsciiPrint ("Test FAILED!\n");
      ThreadExit(EFI_D_ERROR);
      return EFI_D_ERROR; 
   }

   event1_time_before = GetTimerCountms ();
   AsciiPrint ("Thread1 (%d) - CPU (%d): wait on event1 first time at time: %d ms\n", t, cp, event1_time_before);
   event_status = EventWaitTimeout(event1,500);

   /* check thread1 has not send event1 */
   if (shared_handshake_stage_g == 1) {
      AsciiPrint ("\n\nThread1 event send before Thread2 wait timeout.\n");
      AsciiPrint ("Test FAILED!\n");
      ThreadExit(EFI_D_ERROR);
      return EFI_D_ERROR; 
   }

   event1_time_after = GetTimerCountms ();
   if (event_status == 0) {
      AsciiPrint ("Thread1 (%d) - CPU (%d): did get signal for event1 first time at time: %d ms\n", t, cp, event1_time_after);
      AsciiPrint ("Test FAILED!\n");
      ThreadExit(EFI_D_ERROR);
      return EFI_D_ERROR; 
   }
   else {
      AsciiPrint ("Thread1 (%d) - CPU (%d): Did not get signal for event1 first time at time: %d ms\n", t, cp, event1_time_after);
   }


  ThreadSleep (1000);

   /* check thread1 sent event1 */
   if (shared_handshake_stage_g == 1) {
      AsciiPrint ("\n\nHandshake Stage[%d]: Thread1 event sent.\n", shared_handshake_stage_g);
   }
   else {
      AsciiPrint ("\n\nHandshake Stage[%d]: Thread1 event is not sent.\n", shared_handshake_stage_g);
      AsciiPrint ("Test FAILED!\n");
      ThreadExit(EFI_D_ERROR);
      return EFI_D_ERROR; 
   }

   /* wait second time, after signalled */
   event1_time_before = GetTimerCountms ();
   AsciiPrint ("Thread1 (%d) - CPU (%d): wait on event1 second time at time: %d ms\n", t, cp, event1_time_before);
   event_status = EventWaitTimeout(event1,1500);

   event1_time_after = GetTimerCountms ();
   if (event_status == 0) {
      AsciiPrint ("Thread1 (%d) - CPU (%d): did get signal for event1 second time at time: %d ms\n", t, cp, event1_time_after);
   }
   else {
      AsciiPrint ("Thread1 (%d) - CPU (%d): DID NOT get signal for event1 second time at time: %d ms\n", t, cp, event1_time_after);
      AsciiPrint ("Test FAILED!\n");
      ThreadExit(EFI_D_ERROR);
      return EFI_D_ERROR; 
   }


   ThreadSleep (200);
   AsciiPrint ("Exiting Thread2 (t_event_timeout2) with ID: %d on CPU: %d\n", t, cp);
   ThreadExit(0);
   return 0;
}


EFI_STATUS Event_timeout_test(VOID) {
   UINTN cp, previous_cpu, next_cpu;
   Thread *t;
   BOOLEAN event_init_status;
   KStatus thread_status1, thread_status2;
   int thread_rc1, thread_rc2;

   AsciiPrint("Testcase: Event Timeout\n");

   t = GetCurrentThread();
   cp = ThreadGetCurrCpu(t);
   previous_cpu = cp;

   shared_handshake_stage_g = 0;
   AsciiPrint("Initialize shared_handshake_stage_g to %d\n", shared_handshake_stage_g);

   AsciiPrint("Initialize shared_counter_g to %d\n", shared_counter_g);
   shared_counter_g = 0;

   /* Initialize private lock of regular type (autounsignal on)*/
   AsciiPrint("Initialize event.\n");
   event1 = EventInit(0, 0, 1);
   event_init_status = EventInitialized(event1);
   if (event_init_status == 1) {
      AsciiPrint ("\tevent_init_status: %d\n", event_init_status);
   }
   else {
      AsciiPrint ("\tevent_init_status: %d\n", event_init_status);
      AsciiPrint ("Test FAILED!\n");
      return 0;
   }

   /* Generate and setup Thread1 */
   mpt1 = ThreadCreate ("EVENT_T1", &t_event_timeout1, (void*)"EVENT_T1", DEFAULT_PRIORITY, DEFAULT_STACK_SIZE);

   next_cpu = Get_Next_CPU(previous_cpu);
   AsciiPrint ("Previous CPU: %d Next CPU: %d\n", previous_cpu, next_cpu);
   previous_cpu = next_cpu;
   ThreadSetPinnedCpu (mpt1, next_cpu);

   if (mpt1 == NULL)
   {
      AsciiPrint("Unable to create thread (t_event_timeout1), exiting ...\n");
      return EFI_UNSUPPORTED;
   }

   //ThreadDetach (mpt1);
   ThreadResume (mpt1);

   /* Generate and setup Thread2 */
   mpt2 = ThreadCreate ("EVENT_T2", &t_event_timeout2, (void*)"EVENT_T2", DEFAULT_PRIORITY, DEFAULT_STACK_SIZE);
   next_cpu = Get_Next_CPU(previous_cpu);
   AsciiPrint ("Previous CPU: %d Next CPU: %d\n", previous_cpu, next_cpu);
   previous_cpu = next_cpu;
   ThreadSetPinnedCpu (mpt2, next_cpu);

   if (mpt2 == NULL)
   {
      AsciiPrint("Unable to create thread (t_event_timeout2), exiting ...\n");
      return EFI_UNSUPPORTED;
   }

   //ThreadDetach (mpt2);
   ThreadResume (mpt2);

   /* wait for threads to exit and join */
   thread_status1 = ThreadJoin(mpt1, &thread_rc1, 10000);
   thread_status2 = ThreadJoin(mpt2, &thread_rc2, 10000);

   /* destroy event that was initialized */
   EventDestroy(event1);

   /* check pass/fail status */
   AsciiPrint ("\nMain Thread: Thr1 - thread_status(%d)\tthread_rc(%d)\n", thread_status1, thread_rc1);
   AsciiPrint ("\nMain Thread: Thr2 - thread_status(%d)\tthread_rc(%d)\n", thread_status2, thread_rc2);
   if ((thread_status1 != 0)  || (thread_rc1 != 0) || (thread_status2 != 0)  || (thread_rc2 != 0)) {
      AsciiPrint ("Test FAILED!\n");
      return EFI_D_ERROR;
   }
   else
   {
      AsciiPrint ("Test PASSED!\n");
   }

   ThreadSleep (200);
   return EFI_SUCCESS;
   
}

int t_event_unsignal_1( IN VOID  *Arg) {
   int cp;
   Thread *t;
   UINTN event1_time_signal, event1_time_unsignal;
   BOOLEAN event_init_status;
   UINTN thread_initial_wait = 7000;
   UINTN thread_addition_wait_to_unsignal = 6000;
   UINT32 auto_unsignal_flag;
   BOOLEAN reshedule;

   t = GetCurrentThread();
   cp = ThreadGetCurrCpu(t);

   AsciiPrint ("Executing t_event_unsignal_1 with ID: %d on CPU: %d\n", t, cp);

   /* Initialize private lock */
   auto_unsignal_flag = 0; /* multiple threads can get event signal */
   AsciiPrint("Initialize event with Autounsignal flag: %d (0: all threads, 1: one thread).\n", auto_unsignal_flag);
   event1 = EventInit(0, 0, auto_unsignal_flag);
   event_init_status = EventInitialized(event1);
   if (event_init_status == 1) {
      AsciiPrint ("\tevent_init_status: %d\n", event_init_status);
   }
   else {
      AsciiPrint ("\tThread t_event_unsignal_1: Failed to initialize event (event_init_status: %d)\n", event_init_status);
      ThreadExit(EFI_D_ERROR);
      return EFI_D_ERROR; 
   }


   AsciiPrint ("Thread t_event_unsignal_1 wait for %d ms before signaling\n", thread_initial_wait);

   /* sleep awhile befere sending signal */
   ThreadSleep (thread_initial_wait );

   /* at least three threads are waiting on signal.
    *       Need at least three thread waiting to see that they are unblocked in reverse order */
   if (shared_handshake_stage_g >= 3) {
      AsciiPrint ("Thread t_event_unsignal_1: At least three thread_x are waiting on signal.\n");
   }
   else {
      AsciiPrint ("Thread t_event_unsignal_1: None of thread_x is waiting on signal.\n");
      AsciiPrint ("Thread t_event_unsignal_1: Failed for wait untils some threads already waiting.\n");
      ThreadExit(EFI_D_ERROR);
      return EFI_D_ERROR; 
   }

   event1_time_signal = GetTimerCountms ();
   reshedule = 1;
   AsciiPrint ("Thread t_event_unsignal_1 sends signal at %d ms with reshedule: %d\n", event1_time_signal, reshedule );
   EventSignal(event1, reshedule);

   AsciiPrint ("Thread t_event_unsignal_1 sleep for %d timer counter before sending unsignal.\n", thread_addition_wait_to_unsignal);
   /* additional wait to allow some tasks to get signal and block others */
   ThreadSleep (thread_addition_wait_to_unsignal);

   /* Autounsignal */
   EventUnsignal(event1);
   event1_time_unsignal = GetTimerCountms ();
   AsciiPrint ("Thread t_event_unsignal_1 sent unsignal at %d ms\n", event1_time_unsignal);

   /* hold onto lock for much longer while sleeping to ensure that resource is locked for next thread*/ 
   ThreadSleep (500);

   AsciiPrint ("Exiting Thread t_event_unsignal_1 with ID: %d on CPU: %d\n", t, cp);
   ThreadExit(0);
   return 0;
}


int t_event_unsignal_x( IN VOID  *Arg) {
   int cp;
   Thread *t;
   UINTN event1_time_wait_start, event1_time_handshake, event1_time_got_signal;
   KStatus event_status;
   thread_config* thr_cfg = (thread_config*)Arg;
   UINTN thread_index = (thr_cfg->thread_index);
   UINTN thread_initial_wait = (thr_cfg->initial_wait_ms);
   UINTN event_timeout = (thr_cfg->event_timeout_ms);

   t = GetCurrentThread();
   cp = ThreadGetCurrCpu(t);

   AsciiPrint ("Executing t_event_unsignal_x[%d] with ID: %d on CPU: %d\n", thread_index, t, cp);

   event1_time_wait_start = GetTimerCountms();
   AsciiPrint ("Thread[%d] at %d timer count, sleeps for %d ms before waiting on signal\n", 
                  thread_index, event1_time_wait_start, thread_initial_wait);

   /* sleep before waiting on event*/
   ThreadSleep (1000*thread_initial_wait);

   /* each waiting thread, increment handshake flag */
   event1_time_handshake = GetTimerCountms();
   shared_handshake_stage_g++;
   AsciiPrint ("Thread t_event_unsignal_x[%d] inc shared_handshake_stage_g to %d at %d ms.\n", 
                  thread_index, shared_handshake_stage_g, event1_time_handshake);
   event_timeout = 1000*event_timeout;
   event_status = EventWaitTimeout(event1, event_timeout);

   event1_time_got_signal = GetTimerCountms();
   if (event_status == 0) {
      AsciiPrint ("Thread t_event_unsignal_x[%d] got signal at %d ms.\n", thread_index, event1_time_got_signal);

      /* update counter of this thread */
      shared_counter_group_g[thread_index] = (1000*thread_initial_wait );
      AsciiPrint ("Thread t_event_unsignal_x[%d]) updated counter.\n", thread_index);
   }
   else {
      AsciiPrint ("Thread t_event_unsignal_x[%d]: DID NOT get signal at %d ms (timeout expiry: %d).\n",  
                  thread_index, event1_time_got_signal, event_timeout);
   }


   AsciiPrint ("Exiting Thread t_event_unsignal_x[%d]) with ID: %d on CPU: %d\n", thread_index, t, cp);
   ThreadExit(0);
   return 0;
}

EFI_STATUS Event_unsignal_test(VOID) {
   UINTN cp, previous_cpu, next_cpu;
   UINTN thr_index, num_of_child_threads;
   Thread *t;
   UINTN thr_create_time;
   KStatus thread_status;
   INT32 thread_rc;

   AsciiPrint("Testcase: Event Unsignal\n");

   t = GetCurrentThread();
   cp = ThreadGetCurrCpu(t);
   previous_cpu = cp;

   num_of_child_threads = 10;
   AsciiStrnCpy((CHAR8 *)(thr_config[0].thread_name), "EVENT_U0", TEST_CONFIG_STR_LEN - 1);
   AsciiStrnCpy((CHAR8 *)(thr_config[1].thread_name), "EVENT_U1", TEST_CONFIG_STR_LEN - 1);
   AsciiStrnCpy((CHAR8 *)(thr_config[2].thread_name), "EVENT_U2", TEST_CONFIG_STR_LEN - 1);
   AsciiStrnCpy((CHAR8 *)(thr_config[3].thread_name), "EVENT_U3", TEST_CONFIG_STR_LEN - 1);
   AsciiStrnCpy((CHAR8 *)(thr_config[4].thread_name), "EVENT_U4", TEST_CONFIG_STR_LEN - 1);
   AsciiStrnCpy((CHAR8 *)(thr_config[5].thread_name), "EVENT_U5", TEST_CONFIG_STR_LEN - 1);
   AsciiStrnCpy((CHAR8 *)(thr_config[6].thread_name), "EVENT_U6", TEST_CONFIG_STR_LEN - 1);
   AsciiStrnCpy((CHAR8 *)(thr_config[7].thread_name), "EVENT_U7", TEST_CONFIG_STR_LEN - 1);
   AsciiStrnCpy((CHAR8 *)(thr_config[8].thread_name), "EVENT_U8", TEST_CONFIG_STR_LEN - 1);
   AsciiStrnCpy((CHAR8 *)(thr_config[9].thread_name), "EVENT_U9", TEST_CONFIG_STR_LEN - 1);

   shared_handshake_stage_g = 0;
   AsciiPrint("Initialize shared_handshake_stage_g to %d\n", shared_handshake_stage_g);

   /* start first thread to send signal */
   mpt1 = ThreadCreate ("EVENT_U", &t_event_unsignal_1, (void*)"EVENT_U", DEFAULT_PRIORITY, DEFAULT_STACK_SIZE);
   if (mpt1 == NULL)
   {
      AsciiPrint("Unable to create thread mpt1, exiting ...\n");
      return EFI_UNSUPPORTED;
   }

   next_cpu = Get_Next_CPU(previous_cpu);
   AsciiPrint ("Previous CPU: %d Next CPU: %d\n", previous_cpu, next_cpu);
   previous_cpu = next_cpu;
   ThreadSetPinnedCpu (mpt1, next_cpu);

   ThreadResume (mpt1);

   /* threads to wait on signal */
   for (thr_index=0; thr_index < num_of_child_threads; thr_index++) {

      thr_config[thr_index].thread_index = thr_index;

      /* time to wait before the create thread to execute */
      thr_config[thr_index].initial_wait_ms = (UINTN)(thr_index+1);
      
      /* timeout for event wait */
      thr_config[thr_index].event_timeout_ms = (UINTN)(15-thr_index);

      /* clear counter of thread to be created */
      shared_counter_group_g[thr_index] = 0;
   
      /* core0 sleeps before creating thread */
      ThreadSleep (1000);

      thr_create_time = GetTimerCountms ();
      AsciiPrint("Creating thread[%d] at %d ms\n", thr_index, thr_create_time);


      /* Generate and setup Thread1 */
      mpt_group[thr_index] = ThreadCreate ( (CHAR8 *)(thr_config[thr_index].thread_name), &t_event_unsignal_x, 
                                            (void*)&(thr_config[thr_index]), DEFAULT_PRIORITY, DEFAULT_STACK_SIZE);

      if (mpt_group[thr_index] == NULL)
      {
         AsciiPrint("Unable to create thread mtp_group[%d], exiting ...\n", thr_index);
         return EFI_UNSUPPORTED;
      }

      next_cpu = Get_Next_CPU(previous_cpu);
      AsciiPrint ("Previous CPU: %d Next CPU: %d\n", previous_cpu, next_cpu);
      previous_cpu = next_cpu;
      ThreadSetPinnedCpu (mpt_group[thr_index], next_cpu);

      ThreadResume (mpt_group[thr_index]);
   }

   /* Wait for all threads to exit */
   thread_status = ThreadJoin(mpt1, &thread_rc, 10000);
   AsciiPrint ("\nMain Thread: Thr1 - thread_status(%d)\tthread_rc(%d)\n", thread_status, thread_rc);
   if ((thread_status != 0)  || (thread_rc != 0)) {
      AsciiPrint ("Main Thread: Thr1 did not complete successfully!\n");
      AsciiPrint ("Test FAILED!\n");
      return EFI_D_ERROR;
   }

   AsciiPrint ("Thread main: wait for all threads to exit.\n");
   for (thr_index=0; thr_index < num_of_child_threads; thr_index++) {
      /* wait for thread to join or timeout in 10000 */
      thread_status = ThreadJoin(mpt_group[thr_index], &thread_rc, 10000);
      AsciiPrint ("Main thread: t_event_unsignal_x[%d] thread_status(%d)\tthread_rc(%d)\n", 
                           thr_index, thread_status , thread_rc);
      if ((thread_status != 0)|| (thread_rc != 0)) {
         AsciiPrint ("Main Thread: t_event_unsignal_x[%d] did not complete successfully!\n", thr_index);
         AsciiPrint ("Test FAILED!\n");
         return EFI_D_ERROR;
      }
   }

   
   /* Report updated counters */ 
   for (thr_index=0; thr_index < num_of_child_threads; thr_index++) {
      AsciiPrint("Thread[%d]: shared_counter_group_g[%d] = %d\n", thr_index, thr_index, shared_counter_group_g[thr_index]);
   }

   ThreadSleep (200);

   /* destroy event that was initialized */
   EventDestroy(event1);

   /* Pass criteria check:
    *    - The first 3 threads are expected to be already waiting on signal: thus, counter > 0.
    *    - The 4th thread should have started and got signal: thus, counter > 0.
    *    - The last thread did not get signal: thus, counter = 0.
    * */ 
   if (  ((shared_counter_group_g[0] == 0) || (shared_counter_group_g[1] == 0) || (shared_counter_group_g[2] == 0)) || 
         (shared_counter_group_g[3] == 0) || 
         (shared_counter_group_g[num_of_child_threads -1] != 0) ) {
      AsciiPrint ("Test successful criteria not met!\n");
      AsciiPrint ("Test FAILED!\n");
      return EFI_D_ERROR;
   }
   else {
      AsciiPrint ("Test PASSED!\n");
      return EFI_SUCCESS;
   }
}



int t_event_unsignal_regular_1( IN VOID  *Arg)
{
   int cp;
   Thread *t;
   UINTN time_before, time_after;

   t = GetCurrentThread();
   cp = ThreadGetCurrCpu(t);

   AsciiPrint ("Executing Thread1 (t_event_unsignal_regular_1) with ID: %d on CPU: %d\n", t, cp);


   ThreadSleep (1000);

   time_before = GetTimerCountms ();
   AsciiPrint ("Thread1 (%d) - CPU (%d): before sending event at time: %d ms\n", t, cp, time_before);
   
   /* send first event1 after timeout */
   shared_handshake_stage_g = 1;
   EventSignal(event1, 0);
   time_after = GetTimerCountms ();
   AsciiPrint ("Thread1 (%d) - CPU (%d): after sent event at time: %d ms\n", t, cp, time_after);

   ThreadSleep (1000);

   /* unsignal event */
   time_before = GetTimerCountms ();
   AsciiPrint ("Thread1 (%d) - CPU (%d): before unsignalling event at time: %d ms\n", t, cp, time_before);
   
   ThreadSleep (500);

   /* send unsignal */
   shared_handshake_stage_g = 2;
   EventUnsignal(event1);
   time_after = GetTimerCountms ();
   AsciiPrint ("Thread1 (%d) - CPU (%d): after event unsignalled at time: %d ms\n", t, cp, time_after);

  /* hold onto lock for much longer while sleeping to ensure that resource is locked for next thread*/ 
   ThreadSleep (500);

   AsciiPrint ("Exiting Thread1 (t_lock_single1) with ID: %d on CPU: %d\n", t, cp);
   ThreadExit(0);
   return 0;
}


int t_event_unsignal_regular_2( IN VOID  *Arg)
{
   int cp;
   Thread *t;
   UINTN time_before, time_after;
   KStatus event_status;

   t = GetCurrentThread();
   cp = ThreadGetCurrCpu(t);

   AsciiPrint ("Executing Thread2 (t_event_unsignal_regular_2) with ID: %d on CPU: %d\n", t, cp);

   ThreadSleep (100);

   /* check thread1 has not send event1, before waiting on signal */
   if (shared_handshake_stage_g == 1) {
      AsciiPrint ("\n\nThread1 event send before Thread2 wait.\n");
      AsciiPrint ("Test FAILED!\n");
      ThreadExit(EFI_D_ERROR);
      return EFI_D_ERROR; 
   }

   time_before = GetTimerCountms ();
   AsciiPrint ("Thread1 (%d) - CPU (%d): wait on event1 first time at time: %d ms\n", t, cp, time_before);
   event_status = EventWaitTimeout(event1,500);

   /* check thread1 has not send event1 */
   if (shared_handshake_stage_g == 1) {
      AsciiPrint ("\n\nThread1 event send before Thread2 wait timeout.\n");
      AsciiPrint ("Test FAILED!\n");
      ThreadExit(EFI_D_ERROR);
      return EFI_D_ERROR; 
   }

   time_after = GetTimerCountms ();
   if (event_status == 0) {
      AsciiPrint ("Thread1 (%d) - CPU (%d): did get signal for event1 first time at time: %d ms\n", t, cp, time_after);
      AsciiPrint ("Test FAILED!\n");
      ThreadExit(EFI_D_ERROR);
      return EFI_D_ERROR; 
   }
   else {
      AsciiPrint ("Thread1 (%d) - CPU (%d): DID NOT get signal for event1 first time at time: %d ms\n", t, cp, time_after);
   }


  ThreadSleep (1000);

   /* check thread1 sent event1 */
   if (shared_handshake_stage_g == 1) {
      AsciiPrint ("\n\nHandshake Stage[%d]: Thread1 event sent.\n", shared_handshake_stage_g);
   }
   else {
      AsciiPrint ("\n\nHandshake Stage[%d]: Thread1 event is not sent.\n", shared_handshake_stage_g);
      AsciiPrint ("Test FAILED!\n");
      ThreadExit(EFI_D_ERROR);
      return EFI_D_ERROR; 
   }

  
   ThreadSleep (1000);

   /* check thread1 event1 is unsignalled */
   if (shared_handshake_stage_g == 2) {
      AsciiPrint ("\n\nHandshake Stage[%d]: Thread1 event is unsignalled.\n", shared_handshake_stage_g);
   }
   else {
      AsciiPrint ("\n\nHandshake Stage[%d]: Thread1 event is not sent.\n", shared_handshake_stage_g);
      AsciiPrint ("Test FAILED!\n");
      ThreadExit(EFI_D_ERROR);
      return EFI_D_ERROR; 
   }


   time_before = GetTimerCountms ();
   AsciiPrint ("Thread1 (%d) - CPU (%d): wait on event1 second time at time: %d ms\n", t, cp, time_before);
   event_status = EventWaitTimeout(event1,1500);

   time_after = GetTimerCountms ();
   if (event_status == 0) {
      AsciiPrint ("Thread1 (%d) - CPU (%d): did get signal for event1 second time at time: %d ms\n", t, cp, time_after);
      AsciiPrint ("Test FAILED!\n");
      ThreadExit(EFI_D_ERROR);
      return EFI_D_ERROR; 
   }
   else {
      AsciiPrint ("Thread1 (%d) - CPU (%d): DID NOT get signal for event1 second time at time: %d ms\n", t, cp, time_after);
   }

   ThreadSleep (200);
   AsciiPrint ("Exiting Thread2 (t_lock_single2) with ID: %d on CPU: %d\n", t, cp);
   ThreadExit(0);
   return 0;
}


EFI_STATUS Event_unsignal_regular_test(VOID) {
   UINTN cp, previous_cpu, next_cpu;
   Thread *t;
   BOOLEAN event_init_status;
   KStatus thread_status1, thread_status2;
   int thread_rc1, thread_rc2;


   AsciiPrint("Testcase: Event Unsignal Regular\n");

   t = GetCurrentThread();
   cp = ThreadGetCurrCpu(t);
   previous_cpu = cp;

   shared_handshake_stage_g = 0;
   AsciiPrint("Initialize shared_handshake_stage_g to %d\n", shared_handshake_stage_g);

   shared_counter_g = 0;
   AsciiPrint("Initialize shared_counter_g to %d\n", shared_counter_g);

   /* Initialize private lock of regular type (autounsignal on)*/
   AsciiPrint("Initialize event.\n");
   event1 = EventInit(0, 0, 1);
   event_init_status = EventInitialized(event1);
   if (event_init_status == 1) {
      AsciiPrint ("\tevent_init_status: %d\n", event_init_status);
   }
   else {
      AsciiPrint ("\tevent_init_status: %d\n", event_init_status);
      AsciiPrint ("Test FAILED!\n");
      return 0;
   }

   /* Generate and setup Thread1 */
   mpt1 = ThreadCreate ("EVENT_T1", &t_event_unsignal_regular_1, (void*)"EVENT_T1", DEFAULT_PRIORITY, DEFAULT_STACK_SIZE);

   next_cpu = Get_Next_CPU(previous_cpu);
   AsciiPrint ("Previous CPU: %d Next CPU: %d\n", previous_cpu, next_cpu);
   previous_cpu = next_cpu;
   ThreadSetPinnedCpu (mpt1, next_cpu);

   if (mpt1 == NULL)
   {
      AsciiPrint("Unable to create thread (t_event_unsignal_regular_1), exiting ...\n");
      return EFI_UNSUPPORTED;
   }

   //ThreadDetach (mpt1);
   ThreadResume (mpt1);

   /* Generate and setup Thread2 */
   mpt2 = ThreadCreate ("EVENT_T2", &t_event_unsignal_regular_2, (void*)"EVENT_T2", DEFAULT_PRIORITY, DEFAULT_STACK_SIZE);
   next_cpu = Get_Next_CPU(previous_cpu);
   AsciiPrint ("Previous CPU: %d Next CPU: %d\n", previous_cpu, next_cpu);
   previous_cpu = next_cpu;
   ThreadSetPinnedCpu (mpt2, next_cpu);

   if (mpt2 == NULL)
   {
      AsciiPrint("Unable to create thread (t_event_unsignal_regular_2), exiting ...\n");
      return EFI_UNSUPPORTED;
   }

   //ThreadDetach (mpt2);
   ThreadResume (mpt2);


   /* wait for threads to exit and join */
   thread_status1 = ThreadJoin(mpt1, &thread_rc1, 10000);
   thread_status2 = ThreadJoin(mpt2, &thread_rc2, 10000);

   /* destroy event that was initialized */
   EventDestroy(event1);

   /* check pass/fail status */
   AsciiPrint ("\nMain Thread: Thr1 - thread_status(%d)\tthread_rc(%d)\n", thread_status1, thread_rc1);
   AsciiPrint ("\nMain Thread: Thr2 - thread_status(%d)\tthread_rc(%d)\n", thread_status2, thread_rc2);
   if ((thread_status1 != 0)  || (thread_rc1 != 0) || (thread_status2 != 0)  || (thread_rc2 != 0)) {
      AsciiPrint ("Test FAILED!\n");
      return EFI_D_ERROR;
   }
   else
   {
      AsciiPrint ("Test PASSED!\n");
   }

   ThreadSleep (200);
   return EFI_SUCCESS;
   
}



int t_sem_wait_x( IN VOID  *Arg)
{
   int cp;
   Thread *t;
   UINTN sem1_time_before, sem1_time_after;
   KStatus sem_status;
   thread_config* thr_cfg = (thread_config*)Arg;
   UINTN thread_index = (thr_cfg->thread_index);
   UINTN thread_initial_wait = (thr_cfg->initial_wait_ms);
   UINTN sem_post_time = (thr_cfg->release_ms);


   t = GetCurrentThread();
   cp = ThreadGetCurrCpu(t);

   AsciiPrint ("Executing t_sem_wait_x[%d] with ID: %d on CPU: %d\n", thread_index, t, cp);

   ThreadSleep (thread_initial_wait);

   sem1_time_before = GetTimerCountms ();
   AsciiPrint ("Thread[%d] (with ID: %d) - CPU (%d): before getting sem at time: %d ms\n", thread_index, t, cp, sem1_time_before);

   sem_status = SemTryWait(sem1);
   if (sem_status != 0) {
      AsciiPrint ("Thread[%d] - CPU (%d): DID NOT get sem instantly with SemTryWait at time: %d ms\n", 
                     thread_index, cp, sem1_time_before);

      AsciiPrint ("Thread[%d] - CPU (%d): Waiting with SemWait.\n", thread_index, cp);
      sem_status = SemWait(sem1);
      sem1_time_after = GetTimerCountms ();
      if (sem_status != 0) {
         AsciiPrint ("Thread[%d] - CPU (%d): DID NOT get sem with SemWait at time: %d ms\n", thread_index, cp, sem1_time_after);
         AsciiPrint ("Test FAILED!\n");
         ThreadExit(EFI_D_ERROR);
         return EFI_D_ERROR; 
      }
      else {
         AsciiPrint ("Thread[%d] - CPU (%d): got sem with SemWait at time: %d ms\n", thread_index, cp, sem1_time_after);
      }
   }
   else {
      AsciiPrint ("Thread[%d] - CPU (%d): got sem instantly with SemTryWait at time: %d ms\n", thread_index, cp, sem1_time_before);
      if (thread_index >= 3) {
         AsciiPrint ("Thread[%d] - CPU (%d): Semaphore must not have been available instantly\n", thread_index, cp);
         AsciiPrint ("Test FAILED!\n");
         ThreadExit(EFI_D_ERROR);
         return EFI_D_ERROR; 
      }

   }

   /* add current thread to order of obtaining semaphore */
   shared_order_group_g[shared_order_ptr] = thread_index;
   shared_order_ptr++;

   shared_counter_group_g[thread_index] = 1000 + (1000*thread_index);
   AsciiPrint ("\nThread t_sem_wait_x[%d]) updated counter to %d.\n", thread_index, shared_counter_group_g[thread_index]);

   AsciiPrint ("Thread[%d] - CPU (%d): Enter sleep for %d ms\n", thread_index, cp, sem_post_time);
   ThreadSleep (sem_post_time);

   /* release sem */
   sem_status = SemPost(sem1, TRUE);
   sem1_time_after = GetTimerCountms();
   /*
   if (sem_status != 0) {
      AsciiPrint ("Thread1 (%d) - CPU (%d): sem not posted at: %d ms\n", t, cp, sem1_time_after);
      AsciiPrint ("Test FAILED!\n");
      ThreadExit(EFI_D_ERROR);
      return EFI_D_ERROR; 
   }
   else {
      AsciiPrint ("Thread1 (%d) - CPU (%d): sem posted at: %d ms\n", t, cp, sem1_time_after);
   }
   */
   AsciiPrint ("Thread[%d] - CPU (%d): sem posted at: %d ms (rc=%d)\n", thread_index, cp, sem1_time_after, sem_status);


   ThreadSleep (200);
   AsciiPrint ("Exiting t_sem_wait_x[%d] with ID: %d on CPU: %d\n\n", thread_index, t, cp);
   ThreadExit(0);
   return 0;
}

EFI_STATUS Sem_wait_test(VOID) {
   UINTN thr_index, num_of_threads;
   UINTN cp, previous_cpu, next_cpu;
   Thread *t;
   KStatus thread_status;
   int thread_rc;
   UINTN thr_create_time;

   AsciiPrint("Testcase: Sem Wait\n");

   t = GetCurrentThread();
   cp = ThreadGetCurrCpu(t);
   previous_cpu = cp;
   

   shared_handshake_stage_g = 0;
   AsciiPrint("Initialize shared_handshake_stage_g to %d\n", shared_handshake_stage_g);

   AsciiPrint("Initialize shared_counter_g to %d\n", shared_counter_g);
   shared_counter_g = 0;

   /* Initialize private lock of regular type (autounsignal on)*/
   AsciiPrint("Initialize semaphore of count 3.\n");
   sem1 = SemInit(0, 3);

   num_of_threads = 9;

   AsciiStrnCpy((CHAR8 *)(thr_config[0].thread_name), "SEM_W0", TEST_CONFIG_STR_LEN - 1);
   AsciiStrnCpy((CHAR8 *)(thr_config[1].thread_name), "SEM_W1", TEST_CONFIG_STR_LEN - 1);
   AsciiStrnCpy((CHAR8 *)(thr_config[2].thread_name), "SEM_W2", TEST_CONFIG_STR_LEN - 1);
   AsciiStrnCpy((CHAR8 *)(thr_config[3].thread_name), "SEM_W3", TEST_CONFIG_STR_LEN - 1);
   AsciiStrnCpy((CHAR8 *)(thr_config[4].thread_name), "SEM_W4", TEST_CONFIG_STR_LEN - 1);
   AsciiStrnCpy((CHAR8 *)(thr_config[5].thread_name), "SEM_W5", TEST_CONFIG_STR_LEN - 1);
   AsciiStrnCpy((CHAR8 *)(thr_config[6].thread_name), "SEM_W6", TEST_CONFIG_STR_LEN - 1);
   AsciiStrnCpy((CHAR8 *)(thr_config[7].thread_name), "SEM_W7", TEST_CONFIG_STR_LEN - 1);
   AsciiStrnCpy((CHAR8 *)(thr_config[8].thread_name), "SEM_W8", TEST_CONFIG_STR_LEN - 1);

   /* initialize array that stores the order in which the semaphore was obtained */
   for (thr_index=0; thr_index < num_of_threads; thr_index++) {
      shared_order_group_g[thr_index] = 0;
   }
   shared_order_ptr = 0;

   /* Generate and setup Thread1 */
   for (thr_index=0; thr_index < num_of_threads; thr_index++) {

      thr_config[thr_index].thread_index = thr_index;

      /* time to wait before the create thread to execute */
      thr_config[thr_index].initial_wait_ms = (UINTN)(200);
      
      /* timeout for event wait */
      thr_config[thr_index].release_ms = (UINTN)(500);

      /* clear counter of thread to be created */
      shared_counter_group_g[thr_index] = 0;
   
      /* core0 sleeps before creating thread */
      ThreadSleep (100);

      thr_create_time = GetTimerCountms ();
      AsciiPrint("Creating thread[%d] at %d ms\n", thr_index, thr_create_time);

      mpt_group[thr_index] = ThreadCreate ( (CHAR8 *)(thr_config[thr_index].thread_name), &t_sem_wait_x, 
                                            (void*)&(thr_config[thr_index]), DEFAULT_PRIORITY, DEFAULT_STACK_SIZE);
      if (mpt_group[thr_index] == NULL)
      {
         AsciiPrint("Unable to create thread mtp_group[%d], exiting ...\n", thr_index);
         return EFI_UNSUPPORTED;
      }

      next_cpu = Get_Next_CPU(previous_cpu);
      //AsciiPrint ("Previous CPU: %d Next CPU: %d\n", previous_cpu, next_cpu);
      previous_cpu = next_cpu;
      ThreadSetPinnedCpu (mpt_group[thr_index], next_cpu);

      ThreadResume (mpt_group[thr_index]);

   }

   AsciiPrint ("Thread main: wait for all threads to exit.\n");
   for (thr_index=0; thr_index < num_of_threads; thr_index++) {
      /* wait for thread to join or timeout in 10000 */
      thread_status = ThreadJoin(mpt_group[thr_index], &thread_rc, 10000);
      AsciiPrint ("Main thread: t_sem_wait_x[%d] thread_status(%d)\tthread_rc(%d)\n", 
                           thr_index, thread_status , thread_rc);
      if ((thread_status != 0)|| (thread_rc != 0)) {
         AsciiPrint ("Main Thread: t_sem_wait_x[%d] did not complete successfully!\n", thr_index);
         AsciiPrint ("Test FAILED!\n");
         return EFI_D_ERROR;
      }
   }

   /* destroy sem that was initialized */
   AsciiPrint("\nMain Thread: Destroy semaphore.\n");
   SemDestroy(sem1);

   /* Report updated counters */ 
   AsciiPrint ("\nThread main - shared_counter_group_g:\n");
   for (thr_index=0; thr_index < num_of_threads; thr_index++) {
      AsciiPrint("\tThread[%d]: shared_order_group_g[%d] = %d\tshared_counter_group_g[%d] = %d\n", 
                     thr_index, thr_index, shared_order_group_g[thr_index], thr_index, shared_counter_group_g[thr_index]);
   }

   /* Pass criteria check:
    *    - if counter != 0.
    *    - if order not sequential.
    * */ 
   for (thr_index=0; thr_index < num_of_threads; thr_index++) {
      if (shared_counter_group_g[thr_index] == 0) {
         AsciiPrint("\n\tTest pass criteria NOT met (shared_counter_group_g[%d] expected: %d,\t actual %d)\n", 
                        thr_index, 0, shared_counter_group_g[thr_index]);
         AsciiPrint ("Test FAILED!\n");
         return EFI_D_ERROR;
      }
      if (shared_order_group_g[thr_index] != thr_index) {
         AsciiPrint("\n\tTest pass criteria NOT met (shared_order_group_g[%d] expected: %d,\t actual %d)\n", 
                        thr_index, thr_index, shared_order_group_g[thr_index]);
         AsciiPrint ("Test FAILED!\n");
         return EFI_D_ERROR;
      }
   }

   AsciiPrint ("\nTest PASSED!\n");
   ThreadSleep (200);
   return EFI_SUCCESS;
}

int t_sem_try1( IN VOID  *Arg)
{
   int cp;
   Thread *t;
   UINTN sem1_time_before, sem1_time_after;
   KStatus sem_status;
   UINTN cntr, inc_value;
   UINTN sleep_time;

   t = GetCurrentThread();
   cp = ThreadGetCurrCpu(t);

   AsciiPrint ("Executing Thread1 (t_sem_try1) with ID: %d on CPU: %d\n", t, cp);

   ThreadSleep (100);

   sem1_time_before = GetTimerCountms ();
   AsciiPrint ("Thread1 (%d) - CPU (%d): before getting sem at time: %d ms\n", t, cp, sem1_time_before);

   /* send first sem1 (+1) */
   shared_handshake_stage_g = 1;
   sem_status = SemWait(sem1);
   sem1_time_after = GetTimerCountms ();
   if (sem_status != 0) {
      AsciiPrint ("Thread1 (%d) - CPU (%d): DID NOT get sem (+1) first time at time: %d ms\n", t, cp, sem1_time_after);
      AsciiPrint ("Test FAILED!\n");
      ThreadExit(EFI_D_ERROR);
      return EFI_D_ERROR; 
   }
   else {
      AsciiPrint ("Thread1 (%d) - CPU (%d): got sem (+1) at time: %d ms\n", t, cp, sem1_time_after);
   }

   AsciiPrint ("Handshake Stage[%d]: Thread1 got sem.\n", shared_handshake_stage_g);

   cntr = get_counter();
   AsciiPrint ("Thread1 (%d) - CPU (%d): shared_counter_g (before inc): %d\n", t, cp, cntr);
   inc_value = 1000;
   AsciiPrint ("Thread1 (%d) - CPU (%d): Increment counter by value value: %d\n", t, cp, inc_value);
   set_counter(inc_value);

   if (shared_counter_g != inc_value) {
      AsciiPrint ("Thread1: shared_counter_g is invalid (expected:%d,\tactual:%d).\n", inc_value, shared_counter_g);
      AsciiPrint ("Test FAILED!\n");
      ThreadExit(EFI_D_ERROR);
      return EFI_D_ERROR; 
   }

   sleep_time = 1000;
   AsciiPrint ("Thread1 (%d) - CPU (%d): Enter sleep for %d ms\n", t, cp, sleep_time);
   ThreadSleep (sleep_time);

   /* release sem */
   shared_handshake_stage_g = 4;
   sem_status = SemPost(sem1, TRUE);
   sem1_time_after = GetTimerCountms ();
   /*
   if (sem_status != 0) {
      AsciiPrint ("Thread1 (%d) - CPU (%d): sem not posted at: %d ms\n", t, cp, sem1_time_after);
      AsciiPrint ("Test FAILED!\n");
      ThreadExit(EFI_D_ERROR);
      return EFI_D_ERROR; 
   }
   else {
      AsciiPrint ("Thread1 (%d) - CPU (%d): sem posted at: %d ms\n", t, cp, sem1_time_after);
   }
   */
   AsciiPrint ("Thread1 (%d) - CPU (%d): sem posted at: %d ms (rc=%d)\n", t, cp, sem1_time_after, sem_status);

   AsciiPrint ("Handshake Stage[%d]: Thread1 posted sem.\n", shared_handshake_stage_g);

   ThreadSleep (200);
   AsciiPrint ("Exiting Thread1 (t_sem_try1) with ID: %d on CPU: %d\n", t, cp);
   ThreadExit(0);
   return 0;
}


int t_sem_try2( IN VOID  *Arg)
{
   int cp;
   Thread *t;
   UINTN sem2_time_before, sem2_time_after;
   KStatus sem_status;
   UINTN cntr, inc_value;
   UINTN expected_value;
   UINTN sleep_time;

   t = GetCurrentThread();
   cp = ThreadGetCurrCpu(t);

   AsciiPrint ("Executing Thread2 (t_sem_try2) with ID: %d on CPU: %d\n", t, cp);

   ThreadSleep (200);


   /* check thread1 got sem before thread2 trying to get lock */
   if (shared_handshake_stage_g != 1) {
      AsciiPrint ("\n\nThread2: Thread1 does not have sem when requesting lock.\n");
      AsciiPrint ("Test FAILED!\n");
      ThreadExit(EFI_D_ERROR);
      return EFI_D_ERROR; 
   }
   else {
      AsciiPrint ("Thread2: shared_handshake_stage_g(%d) - Thread1 got sem.\n\n", shared_handshake_stage_g);
   }


   sem2_time_before = GetTimerCountms ();
   AsciiPrint ("Thread2 (%d) - CPU (%d): before getting sem at time: %d ms\n", t, cp, sem2_time_before);

   /* send first sem1 (+2) */
   shared_handshake_stage_g = 2;
   sem_status = SemWait(sem1);
   sem2_time_after = GetTimerCountms ();
   if (sem_status != 0) {
      AsciiPrint ("Thread2 (%d) - CPU (%d): DID NOT get sem (+2) first time at time: %d ms\n", t, cp, sem2_time_after);
      AsciiPrint ("Test FAILED!\n");
      ThreadExit(EFI_D_ERROR);
      return EFI_D_ERROR; 
   }
   else {
      AsciiPrint ("Thread2 (%d) - CPU (%d): got sem (+2) at time: %d ms\n", t, cp, sem2_time_after);
   }

   AsciiPrint ("Handshake Stage[%d]: Thread2 got sem.\n", shared_handshake_stage_g);

   cntr = get_counter();
   AsciiPrint ("Thread2 (%d) - CPU (%d): shared_counter_g (before inc): %d\n", t, cp, cntr);
   inc_value = 100;
   AsciiPrint ("Thread2 (%d) - CPU (%d): Increment counter by value value: %d\n", t, cp, inc_value);
   set_counter(inc_value);

   /* check thread1 got sem1 */
   expected_value = 1100;
   if (shared_counter_g != expected_value) {
      AsciiPrint ("Thread2: shared_counter_g is invalid (expected:%d,\tactual:%d).\n", expected_value, shared_counter_g);
      AsciiPrint ("Test FAILED!\n");
      ThreadExit(EFI_D_ERROR);
      return EFI_D_ERROR; 
   }

   sleep_time = 2000;
   AsciiPrint ("Thread2 (%d) - CPU (%d): Enter sleep for %d ms\n", t, cp, sleep_time);
   ThreadSleep (sleep_time);

   ThreadSleep (200);
   AsciiPrint ("Exiting Thread2 (t_sem_try2) with ID: %d on CPU: %d\n", t, cp);
   ThreadExit(0);
   return 0;

}


int t_sem_try3( IN VOID  *Arg)
{
   int cp;
   Thread *t;
   UINTN sem3_time_before, sem3_time_after;
   KStatus sem_status;

   t = GetCurrentThread();
   cp = ThreadGetCurrCpu(t);

   AsciiPrint ("Executing Thread3 (t_sem_try3) with ID: %d on CPU: %d\n", t, cp);

   ThreadSleep (300);

   /* check thread1 and thread2 got sem before thread3 trying to get lock */
   if (shared_handshake_stage_g != 2) {
      AsciiPrint ("\n\nThread3: Thread1 and Thread2 do not have sem when requesting lock.\n");
      AsciiPrint ("Test FAILED!\n");
      ThreadExit(EFI_D_ERROR);
      return EFI_D_ERROR; 
   }
   else {
      AsciiPrint ("Thread3: shared_handshake_stage_g(%d) - Thread1 and Thread2 got sem.\n\n", shared_handshake_stage_g);
   }


   sem3_time_before = GetTimerCountms ();
   shared_handshake_stage_g = 3;
   AsciiPrint ("Thread3 (%d) - CPU (%d): wait on sem1 first time at: %d ms\n", t, cp, sem3_time_before);
   sem_status = SemTryWait(sem1);
   sem3_time_after = GetTimerCountms ();

   if (sem_status == 0) {
      AsciiPrint ("Thread3 (%d) - CPU (%d): sem not expected to be obtained first time at: %d ms\n", t, cp, sem3_time_after);
      AsciiPrint ("Test FAILED!\n");
      ThreadExit(EFI_D_ERROR);
      return EFI_D_ERROR; 
   }
   else {
      AsciiPrint ("Thread3 (%d) - CPU (%d): Did not get sem first time at: %d ms\n", t, cp, sem3_time_after);
   }

   /* wait while before trying to get sem again so Thread1 can release it. */
   AsciiPrint ("Thread3: Enter sleep for 1500 ms, allowing time for Thread1 to post before retrying.\n");
   ThreadSleep (1500);


   /* check thread1 has sent sem1 */
   if (shared_handshake_stage_g != 4) {
      AsciiPrint ("\n\nThread3: Thread1 has not released sem.\n");
      AsciiPrint ("Test FAILED!\n");
      ThreadExit(EFI_D_ERROR);
      return EFI_D_ERROR; 
   }
   else {
      AsciiPrint ("\n\nThread3: Thread1 released sem.\n");
   }

   sem3_time_before = GetTimerCountms ();
   AsciiPrint ("Thread3 (%d) - CPU (%d): wait on sem1 second time at: %d ms\n", t, cp, sem3_time_before);
   sem_status = SemTryWait(sem1);
   sem3_time_after = GetTimerCountms ();


   if (sem_status != 0) {
      AsciiPrint ("Thread3 (%d) - CPU (%d): DID NOT get sem1 second time at: %d ms\n", t, cp, sem3_time_after);
      AsciiPrint ("Test FAILED!\n");
      ThreadExit(EFI_D_ERROR);
      return EFI_D_ERROR; 
   }
   else {
      AsciiPrint ("Thread3 (%d) - CPU (%d): got sem1 second time at: %d ms\n", t, cp, sem3_time_after);
   }

   ThreadSleep (200);
   AsciiPrint ("Exiting Thread3 (t_sem_try3) with ID: %d on CPU: %d\n", t, cp);
   ThreadExit(0);
   return 0;
}


EFI_STATUS Sem_try_test(VOID) {
   UINTN cp, previous_cpu, next_cpu;
   Thread *t;
   KStatus thread_status1, thread_status2, thread_status3;
   int thread_rc1, thread_rc2, thread_rc3;

   AsciiPrint("Testcase: Sem Try\n");

   t = GetCurrentThread();
   cp = ThreadGetCurrCpu(t);
   previous_cpu = cp;

   shared_handshake_stage_g = 0;
   AsciiPrint("Initialize shared_handshake_stage_g to %d\n", shared_handshake_stage_g);

   AsciiPrint("Initialize shared_counter_g to %d\n", shared_counter_g);
   shared_counter_g = 0;

   /* Initialize private lock of regular type (autounsignal on)*/
   AsciiPrint("Initialize semaphore of count 2.\n");
   sem1 = SemInit(0, 2);

   /* Generate and setup Thread1 */
   mpt1 = ThreadCreate ("sem_T1", &t_sem_try1, (void*)"sem_T1", DEFAULT_PRIORITY, DEFAULT_STACK_SIZE);
   next_cpu = Get_Next_CPU(previous_cpu);
   AsciiPrint ("Previous CPU: %d Next CPU: %d\n", previous_cpu, next_cpu);
   previous_cpu = next_cpu;
   ThreadSetPinnedCpu (mpt1, next_cpu);

   if (mpt1 == NULL)
   {
      AsciiPrint("Unable to create thread (t_sem_try1), exiting ...\n");
      return EFI_UNSUPPORTED;
   }

   //ThreadDetach (mpt1);
   ThreadResume (mpt1);

   /* Generate and setup Thread2 */
   mpt2 = ThreadCreate ("sem_T2", &t_sem_try2, (void*)"sem_T2", DEFAULT_PRIORITY, DEFAULT_STACK_SIZE);
   next_cpu = Get_Next_CPU(previous_cpu);
   AsciiPrint ("Previous CPU: %d Next CPU: %d\n", previous_cpu, next_cpu);
   previous_cpu = next_cpu;
   ThreadSetPinnedCpu (mpt2, next_cpu);

   if (mpt2 == NULL)
   {
      AsciiPrint("Unable to create thread (t_sem_try2), exiting ...\n");
      return EFI_UNSUPPORTED;
   }

   //ThreadDetach (mpt2);
   ThreadResume (mpt2);

   /* Generate and setup Thread3 */
   mpt3 = ThreadCreate ("sem_T3", &t_sem_try3, (void*)"sem_T3", DEFAULT_PRIORITY, DEFAULT_STACK_SIZE);
   next_cpu = Get_Next_CPU(previous_cpu);
   AsciiPrint ("Previous CPU: %d Next CPU: %d\n", previous_cpu, next_cpu);
   previous_cpu = next_cpu;
   ThreadSetPinnedCpu (mpt3, next_cpu);

   if (mpt3 == NULL)
   {
      AsciiPrint("Unable to create thread (t_sem_try3), exiting ...\n");
      return EFI_UNSUPPORTED;
   }

   //ThreadDetach (mpt3);
   ThreadResume (mpt3);

   /* wait for threads to exit and join */
   thread_status1 = ThreadJoin(mpt1, &thread_rc1, 10000);
   thread_status2 = ThreadJoin(mpt2, &thread_rc2, 10000);
   thread_status3 = ThreadJoin(mpt3, &thread_rc3, 10000);

   /* destroy sem that was initialized */
   AsciiPrint("Main Thread: Destroy semaphore.\n");
   SemDestroy(sem1);

   /* check pass/fail status */
   AsciiPrint ("\nMain Thread: Thr1 - thread_status(%d)\tthread_rc(%d)\n", thread_status1, thread_rc1);
   AsciiPrint ("\nMain Thread: Thr2 - thread_status(%d)\tthread_rc(%d)\n", thread_status2, thread_rc2);
   AsciiPrint ("\nMain Thread: Thr3 - thread_status(%d)\tthread_rc(%d)\n", thread_status3, thread_rc3);
   if (  (thread_status1 != 0)  || (thread_rc1 != 0) || 
         (thread_status2 != 0)  || (thread_rc2 != 0) || 
         (thread_status3 != 0)  || (thread_rc3 != 0)) {
      AsciiPrint ("Test FAILED!\n");
      return EFI_D_ERROR;
   }
   else
   {
      AsciiPrint ("Test PASSED!\n");
   }

   ThreadSleep (200);
   return EFI_SUCCESS;
   
}




int t_sem_timeout1( IN VOID  *Arg)
{
   int cp;
   Thread *t;
   UINTN sem1_time_before, sem1_time_after;
   KStatus sem_status;
   UINTN cntr, inc_value;
   UINTN sleep_time;

   t = GetCurrentThread();
   cp = ThreadGetCurrCpu(t);

   AsciiPrint ("Executing Thread1 (t_sem_timeout1) with ID: %d on CPU: %d\n", t, cp);

   ThreadSleep (100);

   sem1_time_before = GetTimerCountms ();
   AsciiPrint ("Thread1 (%d) - CPU (%d): before getting sem at time: %d ms\n", t, cp, sem1_time_before);

   /* send first sem1 (+1) */
   shared_handshake_stage_g = 1;
   sem_status = SemWait(sem1);
   sem1_time_after = GetTimerCountms ();
   if (sem_status != 0) {
      AsciiPrint ("Thread1 (%d) - CPU (%d): DID NOT get sem (+1) first time at time: %d ms\n", t, cp, sem1_time_after);
      AsciiPrint ("Test FAILED!\n");
      ThreadExit(EFI_D_ERROR);
      return EFI_D_ERROR; 
   }
   else {
      AsciiPrint ("Thread1 (%d) - CPU (%d): got sem (+1) at time: %d ms\n", t, cp, sem1_time_after);
   }

   AsciiPrint ("Handshake Stage[%d]: Thread1 got sem.\n", shared_handshake_stage_g);

   cntr = get_counter();
   AsciiPrint ("Thread1 (%d) - CPU (%d): shared_counter_g (before inc): %d\n", t, cp, cntr);
   inc_value = 1000;
   AsciiPrint ("Thread1 (%d) - CPU (%d): Increment counter by value value: %d\n", t, cp, inc_value);
   set_counter(inc_value);

   if (shared_counter_g != inc_value) {
      AsciiPrint ("Thread1: shared_counter_g is invalid (expected:%d,\tactual:%d).\n", inc_value, shared_counter_g);
      AsciiPrint ("Test FAILED!\n");
      ThreadExit(EFI_D_ERROR);
      return EFI_D_ERROR; 
   }

   sleep_time = 1000;
   AsciiPrint ("Thread1 (%d) - CPU (%d): Enter sleep for %d ms\n", t, cp, sleep_time);
   ThreadSleep (sleep_time);

   /* release sem */
   shared_handshake_stage_g = 4;
   sem_status = SemPost(sem1, TRUE);
   sem1_time_after = GetTimerCountms ();
   /*
   if (sem_status != 0) {
      AsciiPrint ("Thread1 (%d) - CPU (%d): sem not posted at: %d ms\n", t, cp, sem1_time_after);
      AsciiPrint ("Test FAILED!\n");
      ThreadExit(EFI_D_ERROR);
      return EFI_D_ERROR; 
   }
   else {
      AsciiPrint ("Thread1 (%d) - CPU (%d): sem posted at: %d ms\n", t, cp, sem1_time_after);
   }
   */
   AsciiPrint ("Thread1 (%d) - CPU (%d): sem posted at: %d ms (rc=%d)\n", t, cp, sem1_time_after, sem_status);

   AsciiPrint ("Thread1: Handshake Stage[%d].\n", shared_handshake_stage_g);

   /* wait for Thread3 to post sem */
   ThreadSleep (300);

   /* check thread3 has released */
   if (shared_handshake_stage_g != 5) {
      AsciiPrint ("\n\nThread1: Thread3 did not release sem.\n");
      AsciiPrint ("Test FAILED!\n");
      ThreadExit(EFI_D_ERROR);
      return EFI_D_ERROR; 
   }
   else {
      AsciiPrint ("\n\nThread1: Thread3 released sem.\n");
   }

   sem1_time_before = GetTimerCountms ();
   AsciiPrint ("Thread1 (%d) - CPU (%d): wait on sem1 again at: %d ms\n", t, cp, sem1_time_before);
   sem_status = SemTimedWait(sem1,500);
   sem1_time_after = GetTimerCountms ();

 
   if (sem_status != 0) {
      AsciiPrint ("Thread1 (%d) - CPU (%d): DID NOT get sem1 again at: %d ms\n", t, cp, sem1_time_after);
      AsciiPrint ("Test FAILED!\n");
      ThreadExit(EFI_D_ERROR);
      return EFI_D_ERROR; 
   }
   else {
      AsciiPrint ("Thread1 (%d) - CPU (%d): got sem1 again at: %d ms\n", t, cp, sem1_time_after);
   }

   /* thread1 got sem again */
   shared_handshake_stage_g = 6;


   ThreadSleep (200);
   AsciiPrint ("Exiting Thread1 (t_sem_timeout1) with ID: %d on CPU: %d\n", t, cp);
   ThreadExit(0);
   return 0;
}


int t_sem_timeout2( IN VOID  *Arg)
{
   int cp;
   Thread *t;
   UINTN sem2_time_before, sem2_time_after;
   KStatus sem_status;
   UINTN cntr, inc_value;
   UINTN expected_value;
   UINTN sleep_time;

   t = GetCurrentThread();
   cp = ThreadGetCurrCpu(t);

   AsciiPrint ("Executing Thread2 (t_sem_timeout2) with ID: %d on CPU: %d\n", t, cp);

   ThreadSleep (200);


   /* check thread1 got sem before thread2 trying to get lock */
   if (shared_handshake_stage_g != 1) {
      AsciiPrint ("\n\nThread2: Thread1 does not have sem when requesting lock.\n");
      AsciiPrint ("Test FAILED!\n");
      ThreadExit(EFI_D_ERROR);
      return EFI_D_ERROR; 
   }
   else {
      AsciiPrint ("Thread2: shared_handshake_stage_g(%d) - Thread1 got sem.\n\n", shared_handshake_stage_g);
   }


   sem2_time_before = GetTimerCountms ();
   AsciiPrint ("Thread2 (%d) - CPU (%d): before getting sem at time: %d ms\n", t, cp, sem2_time_before);

   /* send first sem1 (+2) */
   shared_handshake_stage_g = 2;
   sem_status = SemWait(sem1);
   sem2_time_after = GetTimerCountms ();
   if (sem_status != 0) {
      AsciiPrint ("Thread2 (%d) - CPU (%d): DID NOT get sem (+2) first time at time: %d ms\n", t, cp, sem2_time_after);
      AsciiPrint ("Test FAILED!\n");
      ThreadExit(EFI_D_ERROR);
      return EFI_D_ERROR; 
   }
   else {
      AsciiPrint ("Thread2 (%d) - CPU (%d): got sem (+2) at time: %d ms\n", t, cp, sem2_time_after);
   }

   AsciiPrint ("Handshake Stage[%d]: Thread2 got sem.\n", shared_handshake_stage_g);

   cntr = get_counter();
   AsciiPrint ("Thread2 (%d) - CPU (%d): shared_counter_g (before inc): %d\n", t, cp, cntr);
   inc_value = 100;
   AsciiPrint ("Thread2 (%d) - CPU (%d): Increment counter by value value: %d\n", t, cp, inc_value);
   set_counter(inc_value);

   /* check thread1 got sem1 */
   expected_value = 1100;
   if (shared_counter_g != expected_value) {
      AsciiPrint ("Thread2: shared_counter_g is invalid (expected:%d,\tactual:%d).\n", expected_value, shared_counter_g);
      AsciiPrint ("Test FAILED!\n");
      ThreadExit(EFI_D_ERROR);
      return EFI_D_ERROR; 
   }


   sleep_time = 2000;
   AsciiPrint ("Thread2 (%d) - CPU (%d): Enter sleep for %d ms\n", t, cp, sleep_time);
   ThreadSleep (sleep_time);

   /* release sem */
   shared_handshake_stage_g = 8;
   sem_status = SemPost(sem1, TRUE);
   sem2_time_after = GetTimerCountms ();
   /*
   if (sem_status != 0) {
      AsciiPrint ("Thread2 (%d) - CPU (%d): sem not posted at: %d ms\n", t, cp, sem2_time_after);
      AsciiPrint ("Test FAILED!\n");
      ThreadExit(EFI_D_ERROR);
      return EFI_D_ERROR; 
   }
   else {
      AsciiPrint ("Thread2 (%d) - CPU (%d): sem posted at: %d ms\n", t, cp, sem2_time_after);
   }
   */
   AsciiPrint ("Thread2 (%d) - CPU (%d): sem posted at: %d ms (rc=%d)\n", t, cp, sem2_time_after, sem_status);

   AsciiPrint ("Handshake Stage[%d]: Thread2 posted sem.\n", shared_handshake_stage_g);

   ThreadSleep (200);
   AsciiPrint ("Exiting Thread2 (t_sem_timeout2) with ID: %d on CPU: %d\n", t, cp);
   ThreadExit(0);
   return 0;
}


int t_sem_timeout3( IN VOID  *Arg)
{
   int cp;
   Thread *t;
   UINTN sem3_time_before, sem3_time_after;
   KStatus sem_status;

   t = GetCurrentThread();
   cp = ThreadGetCurrCpu(t);

   AsciiPrint ("Executing Thread3 (t_sem_timeout3) with ID: %d on CPU: %d\n", t, cp);


   ThreadSleep (300);

   /* check thread1 and thread2 got sem before thread3 trying to get lock */
   if (shared_handshake_stage_g != 2) {
      AsciiPrint ("\n\nThread3: Thread1 and Thread2 do not have sem when requesting lock.\n");
      AsciiPrint ("Test FAILED!\n");
      ThreadExit(EFI_D_ERROR);
      return EFI_D_ERROR; 
   }
   else {
      AsciiPrint ("Thread3: shared_handshake_stage_g(%d) - Thread1 and Thread2 got sem.\n\n", shared_handshake_stage_g);
   }


   sem3_time_before = GetTimerCountms ();
   shared_handshake_stage_g = 3;
   AsciiPrint ("Thread3 (%d) - CPU (%d): wait on sem1 first time at: %d ms\n", t, cp, sem3_time_before);
   sem_status = SemTimedWait(sem1,500);
   sem3_time_after = GetTimerCountms ();

   if (sem_status == 0) {
      AsciiPrint ("Thread3 (%d) - CPU (%d): sem not expected to be obtained first time at: %d ms\n", t, cp, sem3_time_after);
      AsciiPrint ("Test FAILED!\n");
      ThreadExit(EFI_D_ERROR);
      return EFI_D_ERROR; 
   }
   else {
      AsciiPrint ("Thread3 (%d) - CPU (%d): Did not get sem first time at: %d ms\n", t, cp, sem3_time_after);
   }

   /* check thread1 has not send sem1 */
   if (shared_handshake_stage_g != 3) {
      AsciiPrint ("\n\nThread3: Thread1 or Thread2 released sem first time.\n");
      AsciiPrint ("Test FAILED!\n");
      ThreadExit(EFI_D_ERROR);
      return EFI_D_ERROR; 
   }
   else {
      AsciiPrint ("\n\nThread3: Timed out since Thread1 and Thread2 has not released sem first time.\n");
   }

   sem3_time_before = GetTimerCountms ();
   AsciiPrint ("Thread3 (%d) - CPU (%d): wait on sem1 second time at: %d ms\n", t, cp, sem3_time_before);
   sem_status = SemTimedWait(sem1,500);
   sem3_time_after = GetTimerCountms ();

   /* check thread1 has not send sem1 */
   if (shared_handshake_stage_g != 4) {
      AsciiPrint ("\n\nThread3: Thread1 has not released sem.\n");
      AsciiPrint ("Test FAILED!\n");
      ThreadExit(EFI_D_ERROR);
      return EFI_D_ERROR; 
   }
   else {
      AsciiPrint ("\n\nThread3: Thread1 released sem.\n");
   }

   if (sem_status != 0) {
      AsciiPrint ("Thread3 (%d) - CPU (%d): DID NOT get sem1 second time at: %d ms\n", t, cp, sem3_time_after);
      AsciiPrint ("Test FAILED!\n");
      ThreadExit(EFI_D_ERROR);
      return EFI_D_ERROR; 
   }
   else {
      AsciiPrint ("Thread3 (%d) - CPU (%d): got sem1 second time at: %d ms\n", t, cp, sem3_time_after);
   }

   /* Thread3 got sem1 */
   shared_handshake_stage_g = 5;
   AsciiPrint ("Thread3 (%d) - CPU (%d): shared_handshake_stage_g: %d\n", t, cp,  shared_handshake_stage_g);

   sem_status = SemPost(sem1,TRUE);
   sem3_time_after = GetTimerCountms ();
   AsciiPrint ("Thread3 (%d) - CPU (%d): posted sem1 time at: %d ms\n", t, cp, sem3_time_after);

   /* sleep for while before attempting to get it */
   ThreadSleep (400);

  /* check thread1 got semaphore back */
   if (shared_handshake_stage_g != 6) {
      AsciiPrint ("\n\nThread3: Thread1 did not get semaphore back.\n");
      AsciiPrint ("Test FAILED!\n");
      ThreadExit(EFI_D_ERROR);
      return EFI_D_ERROR; 
   }
   else {
      AsciiPrint ("Thread3: shared_handshake_stage_g(%d) - Thread1 and Thread2 got sem.\n\n", shared_handshake_stage_g);
   }

   sem3_time_before = GetTimerCountms ();
   shared_handshake_stage_g = 7;
   AsciiPrint ("Thread3 (%d) - CPU (%d): wait on sem1 third time at: %d ms\n", t, cp, sem3_time_before);
   sem_status = SemTimedWait(sem1,500);
   sem3_time_after = GetTimerCountms ();

   if (sem_status == 0) {
      AsciiPrint ("Thread3 (%d) - CPU (%d): sem not expected to be obtained third time at: %d ms\n", t, cp, sem3_time_after);
      AsciiPrint ("Test FAILED!\n");
      ThreadExit(EFI_D_ERROR);
      return EFI_D_ERROR; 
   }
   else {
      AsciiPrint ("Thread3 (%d) - CPU (%d): Did not get sem third time at: %d ms\n", t, cp, sem3_time_after);
   }

   /* check thread1 has not send sem1 */
   if (shared_handshake_stage_g != 7) {
      AsciiPrint ("\n\nThread3: Thread1 or Thread2 released sem first time.\n");
      AsciiPrint ("Test FAILED!\n");
      ThreadExit(EFI_D_ERROR);
      return EFI_D_ERROR; 
   }
   else {
      AsciiPrint ("\n\nThread3: Timed out since Thread1 and Thread2 has not released sem third time.\n");
   }

   sem3_time_before = GetTimerCountms ();
   AsciiPrint ("Thread3 (%d) - CPU (%d): wait on sem1 fourth time at: %d ms\n", t, cp, sem3_time_before);
   sem_status = SemTimedWait(sem1,500);
   sem3_time_after = GetTimerCountms ();

   /* check thread2 has send sem1 */
   if (shared_handshake_stage_g != 8) {
      AsciiPrint ("\n\nThread3: Thread2 has not released sem.\n");
      AsciiPrint ("Test FAILED!\n");
      ThreadExit(EFI_D_ERROR);
      return EFI_D_ERROR; 
   }
   else {
      AsciiPrint ("\n\nThread3: Thread2 released sem.\n");
   }

   if (sem_status != 0) {
      AsciiPrint ("Thread3 (%d) - CPU (%d): DID NOT get sem1 fourth time at: %d ms\n", t, cp, sem3_time_after);
      AsciiPrint ("Test FAILED!\n");
      ThreadExit(EFI_D_ERROR);
      return EFI_D_ERROR; 
   }
   else {
      AsciiPrint ("Thread3 (%d) - CPU (%d): got sem1 fourth time at: %d ms\n", t, cp, sem3_time_after);
   }


   ThreadSleep (200);
   AsciiPrint ("Exiting Thread3 (t_sem_timeout3) with ID: %d on CPU: %d\n", t, cp);
   ThreadExit(0);
   return 0;
}


EFI_STATUS Sem_timeout_test(VOID) {
   UINTN cp, previous_cpu, next_cpu;
   Thread *t;
   KStatus thread_status1, thread_status2, thread_status3;
   int thread_rc1, thread_rc2, thread_rc3;

   AsciiPrint("Testcase: Multiple threads Timed Wait on semaphore\n");

   t = GetCurrentThread();
   cp = ThreadGetCurrCpu(t);
   previous_cpu = cp;

   shared_handshake_stage_g = 0;
   AsciiPrint("Initialize shared_handshake_stage_g to %d\n", shared_handshake_stage_g);

   AsciiPrint("Initialize shared_counter_g to %d\n", shared_counter_g);
   shared_counter_g = 0;

   /* Initialize private lock of regular type (autounsignal on)*/
   AsciiPrint("Initialize semaphore of count 2.\n");
   sem1 = SemInit(0, 2);

   /* Generate and setup Thread1 */
   mpt1 = ThreadCreate ("sem_T1", &t_sem_timeout1, (void*)"sem_T1", DEFAULT_PRIORITY, DEFAULT_STACK_SIZE);
   next_cpu = Get_Next_CPU(previous_cpu);
   AsciiPrint ("Previous CPU: %d Next CPU: %d\n", previous_cpu, next_cpu);
   previous_cpu = next_cpu;
   ThreadSetPinnedCpu (mpt1, next_cpu);

   if (mpt1 == NULL)
   {
      AsciiPrint("Unable to create thread (t_sem_timeout1), exiting ...\n");
      return EFI_UNSUPPORTED;
   }

   //ThreadDetach (mpt1);
   ThreadResume (mpt1);

   /* Generate and setup Thread2 */
   mpt2 = ThreadCreate ("sem_T2", &t_sem_timeout2, (void*)"sem_T2", DEFAULT_PRIORITY, DEFAULT_STACK_SIZE);
   next_cpu = Get_Next_CPU(previous_cpu);
   AsciiPrint ("Previous CPU: %d Next CPU: %d\n", previous_cpu, next_cpu);
   previous_cpu = next_cpu;
   ThreadSetPinnedCpu (mpt2, next_cpu);

   if (mpt2 == NULL)
   {
      AsciiPrint("Unable to create thread (t_sem_timeout2), exiting ...\n");
      return EFI_UNSUPPORTED;
   }

   //ThreadDetach (mpt2);
   ThreadResume (mpt2);

   /* Generate and setup Thread3 */
   mpt3 = ThreadCreate ("sem_T3", &t_sem_timeout3, (void*)"sem_T3", DEFAULT_PRIORITY, DEFAULT_STACK_SIZE);
   next_cpu = Get_Next_CPU(previous_cpu);
   AsciiPrint ("Previous CPU: %d Next CPU: %d\n", previous_cpu, next_cpu);
   previous_cpu = next_cpu;
   ThreadSetPinnedCpu (mpt3, next_cpu);

   if (mpt3 == NULL)
   {
      AsciiPrint("Unable to create thread (t_sem_timeout3), exiting ...\n");
      return EFI_UNSUPPORTED;
   }

   //ThreadDetach (mpt3);
   ThreadResume (mpt3);

   /* wait for threads to exit and join */
   thread_status1 = ThreadJoin(mpt1, &thread_rc1, 10000);
   thread_status2 = ThreadJoin(mpt2, &thread_rc2, 10000);
   thread_status3 = ThreadJoin(mpt3, &thread_rc3, 10000);

   /* destroy sem that was initialized */
   AsciiPrint("Main Thread: Destroy semaphore.\n");
   SemDestroy(sem1);

   /* check pass/fail status */
   AsciiPrint ("\nMain Thread: Thr1 - thread_status(%d)\tthread_rc(%d)\n", thread_status1, thread_rc1);
   AsciiPrint ("\nMain Thread: Thr2 - thread_status(%d)\tthread_rc(%d)\n", thread_status2, thread_rc2);
   AsciiPrint ("\nMain Thread: Thr3 - thread_status(%d)\tthread_rc(%d)\n", thread_status3, thread_rc3);
   if (  (thread_status1 != 0)  || (thread_rc1 != 0) || 
         (thread_status2 != 0)  || (thread_rc2 != 0) || 
         (thread_status3 != 0)  || (thread_rc3 != 0)) {
      AsciiPrint ("Test FAILED!\n");
      return EFI_D_ERROR;
   }
   else
   {
      AsciiPrint ("Test PASSED!\n");
   }

   ThreadSleep (200);
   return EFI_SUCCESS;
   
}


int t_mutex_priority1( IN VOID  *Arg)
{
   int cp;
   Thread *t;
   UINTN mutex1_time_before, mutex1_time_after;
   KStatus mutex_status;
   UINTN compute_count_limit;
   UINTN iteration_index;

   t = GetCurrentThread();
   cp = ThreadGetCurrCpu(t);

   AsciiPrint ("Executing Thread1 (t_mutex_priority1) with ID: %d on CPU: %d\n", t, cp);

   ThreadSleep (100);

   shared_handshake_stage_g = 1;
   mutex1_time_before = GetTimerCountms ();
   AsciiPrint ("Thread1 on CPU(%d): wait on mutex at %d ms (shared_handshake_stage_g - %d)\n",  
                  cp, mutex1_time_before, shared_handshake_stage_g);

   /* send first mutex1 (+1) */
   mutex1_time_after = GetTimerCountms ();
   AsciiPrint ("Thread1 on CPU(%d): About to get mutex at %d ms (about to set shared_handshake_stage_g -2)\n",  
                  cp, mutex1_time_after);
   mutex_status = MutexAcquire(mutex1);
   shared_handshake_stage_g = 2;
   if (mutex_status != 0) {
      AsciiPrint ("Thread1 on CPU(%d): DID NOT get mutex (+1) first time at time: %d ms\n", cp, mutex1_time_after);
      AsciiPrint ("Test FAILED!\n");
      ThreadExit(EFI_D_ERROR);
      return EFI_D_ERROR; 
   }

   //compute_count_limit = 79; /* napali */
   compute_count_limit = 1000000000; /* max: 2,147,483,647 */ 
   iteration_index = 0;

   mutex_priority_compute_count1_g = 0;
   /* wait for thread2 to start executing and thread3 to wait on mutex */ 
   while ((mutex_priority_compute_count2_g == 0) || (shared_handshake_stage_g != 4)) {
      mutex_priority_compute_count1_g = 0;
   }

   //for (iteration_index = 0; iteration_index < compute_count_limit; iteration_index++) {
   while  (mutex_priority_compute_count1_g < 9999) {
      //AsciiPrint ("Thread1 on CPU (%d): iteration - %d\n", cp, iteration_index);
      mutex_priority_compute_count1_g++;
   }

   /* check thread1 finishes after thread2 and thread3 */
   if (shared_handshake_stage_g != 4)  {
      mutex1_time_after = GetTimerCountms ();
      AsciiPrint ("\n\nThread1 on CPU(%d): DID NOT finish computing after Thread3 starts and Thread2 ends at %d ms.\n", cp, mutex1_time_before);
      AsciiPrint ("Test FAILED!\n");
      ThreadExit(EFI_D_ERROR);
      return EFI_D_ERROR; 
   }

   /* update stage before post, so thread3 would know the state once gets this mutex */
   shared_handshake_stage_g = 5;
   mutex_status = MutexRelease(mutex1);
   AsciiPrint ("Thread1 on CPU(%d): post mutex1 at %d ms (shared_handshake_stage_g: %d)\n", cp, mutex1_time_after, shared_handshake_stage_g);
   if (mutex_status != 0) {
      AsciiPrint ("Thread1 on CPU(%d): Mutex not released successfully.\n", cp);
      AsciiPrint ("Test FAILED!\n");
      ThreadExit(EFI_D_ERROR);
      return EFI_D_ERROR; 
   }

   mutex1_time_after = GetTimerCountms ();
   AsciiPrint ("\n\nThread1 on CPU(%d): Finished computing after Thread3 starts and before Thread2 ends at %d ms.\n", cp, mutex1_time_before);
   AsciiPrint ("\nThread1 on CPU(%d): Final mutex_priority_compute_count1_g (%d)\n", cp, mutex_priority_compute_count1_g);  

   ThreadSleep (200);
   AsciiPrint ("Exiting Thread1 (t_mutex_priority1) with ID: %d on CPU: %d\n", t, cp);
   ThreadExit(0);
   return 0;
}


int t_mutex_priority2( IN VOID  *Arg)
{
   int cp;
   Thread *t;
   UINTN mutex2_time_before, mutex2_time_after;
   UINTN compute_count_limit;
   UINTN iteration_index;

   t = GetCurrentThread();
   cp = ThreadGetCurrCpu(t);

   AsciiPrint ("Executing Thread2 (t_mutex_priority2) with ID: %d on CPU: %d\n", t, cp);

   /* wait for thread1 to start computing */
   //ThreadSleep (200); /* napali */
   ThreadSleep (200);
   mutex2_time_before = GetTimerCountms ();

   /* check thread1 is computing */
   if (shared_handshake_stage_g != 2) {
      AsciiPrint ("\n\nThread2 on CPU(%d): Thread1 is not computing yet at %d ms.\n", cp ,mutex2_time_before);
      AsciiPrint ("Test FAILED!\n");
      ThreadExit(EFI_D_ERROR);
      return EFI_D_ERROR; 
   }
   else {
      AsciiPrint ("Thread2 on CPU(%d): Thread1 is computing (acquired mutex).\n\n", cp);
   }

   AsciiPrint ("Thread2 on CPU(%d): computing at %d ms (about to set shared_handshake_stage_g - 3)\n", cp, mutex2_time_before);
   shared_handshake_stage_g = 3;

   //compute_count_limit = 1599; /* napali */
   compute_count_limit = 2000000000; /* max: 2,147,483,647 */ 
   mutex_priority_compute_count2_g = 0;
   iteration_index = 0;
   //for (iteration_index = 0; iteration_index < compute_count_limit; iteration_index++) {
   /* until thread3 completes */
   while (shared_handshake_stage_g != 7) {
      //AsciiPrint ("\t\t\tThread2 on CPU (%d): %d\n", cp, iteration_index);
      if (mutex_priority_compute_count2_g == 2000000000) {
         mutex_priority_compute_count2_g = 1;
      }
      else {
         mutex_priority_compute_count2_g++;
      }
   }

   /* check thread2 finishes after thread1 and thread3 done computing */
   if (shared_handshake_stage_g != 7)  {
      mutex2_time_after = GetTimerCountms ();
      AsciiPrint ("\n\nThread2 on CPU(%d): DID NOT finish computing after Thread1 and Thread3 at %d ms.\n", cp ,mutex2_time_before);
      AsciiPrint ("Test FAILED!\n");
      ThreadExit(EFI_D_ERROR);
      return EFI_D_ERROR; 
   }

   shared_handshake_stage_g = 8;
   mutex2_time_after = GetTimerCountms ();
   AsciiPrint ("Thread2 on CPU(%d): end of computing at %d ms (shared_handshake_stage_g - %d)\n",  
                  cp, mutex2_time_after, shared_handshake_stage_g);

   mutex2_time_after = GetTimerCountms ();
   AsciiPrint ("\n\nThread2 on CPU(%d): Finished computing after Thread1 and Thread3 at %d ms.\n", cp ,mutex2_time_before);
   AsciiPrint ("\nThread2 on CPU(%d): Final mutex_priority_compute_count2_g (%d)\n", cp, mutex_priority_compute_count2_g);  

   ThreadSleep (200);
   AsciiPrint ("Exiting Thread2 (t_mutex_priority2) with ID: %d on CPU: %d\n", t, cp);
   ThreadExit(0);
   return 0;
}


int t_mutex_priority3( IN VOID  *Arg)
{
   int cp;
   Thread *t;
   UINTN mutex3_time_before, mutex3_time_after;
   KStatus mutex_status;
   UINTN compute_count_limit;
   UINTN iteration_index;

   t = GetCurrentThread();
   cp = ThreadGetCurrCpu(t);

   AsciiPrint ("Executing Thread3 (t_mutex_priority3) with ID: %d on CPU: %d\n", t, cp);

   /* wait for thread2 to start computing */
   //ThreadSleep (300); /* napali */
   ThreadSleep (300);

   /* check thread1 got mutex */
   if (shared_handshake_stage_g != 3) {
      AsciiPrint ("\n\nThread3 on CPU(%d): Thread1 and thread2 have not started to execute.\n", cp);
      AsciiPrint ("Test FAILED!\n");
      ThreadExit(EFI_D_ERROR);
      return EFI_D_ERROR; 
   }
   else {
      AsciiPrint ("\n\nThread3 on CPU(%d): Thread1 acquired mutex and thread2 started to execute.\n", cp);
   }

   mutex3_time_before = GetTimerCountms ();
   AsciiPrint ("Thread3 on CPU(%d): wait on mutex1  at %d ms (about to set shared_handshake_stage_g: 4)\n", cp, mutex3_time_before);
   shared_handshake_stage_g = 4;
   mutex_status = MutexAcquire(mutex1);

   /* check thread3 gets mutex1 after thread1 finish computing */
   if (shared_handshake_stage_g != 5)  {
      mutex3_time_after = GetTimerCountms ();
      AsciiPrint ("\nThread3 on CPU(%d): shared_handshake_stage_g  (%d)\n", cp, shared_handshake_stage_g);  
      AsciiPrint ("\n\nThread3 on CPU(%d): DID NOT get mutex1 after thread1 finish computing at %d ms.\n", cp, mutex3_time_before);
      AsciiPrint ("Test FAILED!\n");
      ThreadExit(EFI_D_ERROR);
      return EFI_D_ERROR; 
   }

   shared_handshake_stage_g = 6;
   //AsciiPrint ("Thread3 on CPU(%d): Got mutex1 after thread1 finish computing at %d ms (shared_handshake_stage_g: %d)\n", 
   //               cp, mutex3_time_after, shared_handshake_stage_g);

   //compute_count_limit = 299; /* napali */
   compute_count_limit = 9; 
   mutex_priority_compute_count3_g = 0;
   for (iteration_index = 0; iteration_index < compute_count_limit; iteration_index++) {
      //AsciiPrint ("\t\t\t\t\t\tThread3 on CPU (%d): %d\n", cp, iteration_index);
      mutex_priority_compute_count3_g++;

   }

   /* check thread3 finishes beforre thread2 */
   if (shared_handshake_stage_g != 6)  {
      mutex3_time_after = GetTimerCountms ();
      AsciiPrint ("\n\nThread3 on CPU(%d): DID NOT finish computing before Thread2 at %d ms.\n", cp ,mutex3_time_before);
      AsciiPrint ("Test FAILED!\n");
      ThreadExit(EFI_D_ERROR);
      return EFI_D_ERROR; 
   }
   
   shared_handshake_stage_g = 7;
   mutex_status = MutexRelease(mutex1);
   mutex3_time_after = GetTimerCountms ();
   AsciiPrint ("Thread3 on CPU(%d): post mutex1 at %d ms.\n", cp, mutex3_time_after);
   if (mutex_status != 0) {
      AsciiPrint ("Thread3 on CPU(%d): Mutex not released successfully.\n", cp);
      AsciiPrint ("Test FAILED!\n");
      ThreadExit(EFI_D_ERROR);
      return EFI_D_ERROR; 
   }


   mutex3_time_after = GetTimerCountms ();
   AsciiPrint ("Thread3 on CPU(%d): Finished computing before Thread2 at %d ms (shared_handshake_stage_g = %d).\n",
                     cp, mutex3_time_before, shared_handshake_stage_g);
   AsciiPrint ("\nThread3 on CPU(%d): Final mutex_priority_compute_count3_g (%d)\n", cp, mutex_priority_compute_count3_g);  


   ThreadSleep (200);
   AsciiPrint ("Exiting Thread3 (t_mutex_priority3) with ID: %d on CPU: %d\n", t, cp);
   ThreadExit(0);
   return 0;
}


EFI_STATUS Mutex_priority_equal_test(VOID) {
   UINTN cp, previous_cpu, test_cpu;
   Thread *t;
   KStatus thread_status1, thread_status2, thread_status3;
   int thread_rc1, thread_rc2, thread_rc3;
   UINT32 priority_level;

   AsciiPrint("Testcase: Thread Equal Priority on mutex wait\n");

   t = GetCurrentThread();
   cp = ThreadGetCurrCpu(t);
   previous_cpu = cp;

   shared_handshake_stage_g = 0;
   AsciiPrint("Initialize shared_handshake_stage_g to %d\n", shared_handshake_stage_g);

   AsciiPrint("Initialize shared_counter_g to %d\n", shared_counter_g);
   shared_counter_g = 0;

   AsciiPrint("Initialize mutex thread counter.\n");
   mutex_priority_compute_count1_g = 0;
   mutex_priority_compute_count2_g = 0;
   mutex_priority_compute_count3_g = 0;
   
   /* Initialize private lock of regular type (autounsignal on)*/
   AsciiPrint("Initialize mutex of count 1.\n");
   mutex1 = MutexInit(0);

   //test_cpu = previous_cpu; /* debug on core0 */
   test_cpu = Get_Next_CPU(previous_cpu);
   //previous_cpu = test_cpu;
   //test_cpu = Get_Next_CPU(previous_cpu);
   //previous_cpu = test_cpu;
   //test_cpu = Get_Next_CPU(previous_cpu);
   AsciiPrint ("Main Thread: Test threads running on CPU(%d)\n", test_cpu);

   /* Setup and generate Thread1 */
   //priority_level = DEFAULT_PRIORITY - 4;
   //priority_level = DEFAULT_PRIORITY - 1;
   priority_level = DEFAULT_PRIORITY;
   AsciiPrint ("Main Thread: Create Thread1 with priority_level(%d)\n", priority_level);
   mpt1 = ThreadCreate ("mutex_P1", &t_mutex_priority1, (void*)"mutex_P1", priority_level, DEFAULT_STACK_SIZE);
   ThreadSetPinnedCpu (mpt1, test_cpu);

   if (mpt1 == NULL)
   {
      AsciiPrint("Unable to create thread (t_mutex_priority1), exiting ...\n");
      return EFI_UNSUPPORTED;
   }

   //ThreadDetach (mpt1);
   ThreadResume (mpt1);

   /* Setup and generate Thread1 */
   priority_level = DEFAULT_PRIORITY;
   AsciiPrint ("Main Thread: Create Thread2 with priority_level(%d)\n", priority_level);
   mpt2 = ThreadCreate ("mutex_P2", &t_mutex_priority2, (void*)"mutex_P2", priority_level, DEFAULT_STACK_SIZE);
   ThreadSetPinnedCpu (mpt2, test_cpu);

   if (mpt2 == NULL)
   {
      AsciiPrint("Unable to create thread (t_mutex_priority2), exiting ...\n");
      return EFI_UNSUPPORTED;
   }

   //ThreadDetach (mpt2);
   ThreadResume (mpt2);

   /* Setup and generate Thread1 */
   //priority_level = DEFAULT_PRIORITY + 4;
   priority_level = DEFAULT_PRIORITY + 1;
   //priority_level = DEFAULT_PRIORITY;
   AsciiPrint ("Main Thread: Create Thread3 with priority_level(%d)\n", priority_level);
   mpt3 = ThreadCreate ("mutex_P3", &t_mutex_priority3, (void*)"mutex_P3", priority_level, DEFAULT_STACK_SIZE);
   ThreadSetPinnedCpu (mpt3, test_cpu);

   if (mpt3 == NULL)
   {
      AsciiPrint("Unable to create thread (t_mutex_priority3), exiting ...\n");
      return EFI_UNSUPPORTED;
   }

   //ThreadDetach (mpt3);
   ThreadResume (mpt3);

   /* wait for threads to exit and join */
   //thread_status1 = ThreadJoin(mpt1, &thread_rc1, 50000); /* napali */
   //thread_status2 = ThreadJoin(mpt2, &thread_rc2, 50000); /* napali */
   //thread_status3 = ThreadJoin(mpt3, &thread_rc3, 50000); /* napali */
   thread_status1 = ThreadJoin(mpt1, &thread_rc1, 2500000); /* hana x50 */
   thread_status2 = ThreadJoin(mpt2, &thread_rc2, 2500000); 
   thread_status3 = ThreadJoin(mpt3, &thread_rc3, 2500000);

   /* destroy mutex that was initialized */
   AsciiPrint("Main Thread: Destroy mutex.\n");
   MutexDestroy(mutex1);

   /* check pass/fail status */
   AsciiPrint ("\nMain Thread: Thr1 - thread_status(%d)\tthread_rc(%d)\n", thread_status1, thread_rc1);
   AsciiPrint ("\nMain Thread: Thr2 - thread_status(%d)\tthread_rc(%d)\n", thread_status2, thread_rc2);
   AsciiPrint ("\nMain Thread: Thr3 - thread_status(%d)\tthread_rc(%d)\n", thread_status3, thread_rc3);
   if (  (thread_status1 != 0)  || (thread_rc1 != 0) || 
         (thread_status2 != 0)  || (thread_rc2 != 0) || 
         (thread_status3 != 0)  || (thread_rc3 != 0)) {

      if (thread_status1 != 0) {
         AsciiPrint ("Thr1 - ThreadJoin time expired\n");
      }
      if (thread_status2 != 0) {
         AsciiPrint ("Thr2 - ThreadJoin time expired\n");
      }
      if (thread_status3 != 0) {
         AsciiPrint ("Thr3 - ThreadJoin time expired\n");
      }

      AsciiPrint ("Test FAILED!\n");
      return EFI_D_ERROR;
   }
   else
   {
      AsciiPrint ("Test PASSED!\n");
   }

   ThreadSleep (200);
   return EFI_SUCCESS;
   
}


EFI_STATUS Mutex_priority_unequal_test(VOID) {
   UINTN cp, previous_cpu, test_cpu;
   Thread *t;
   KStatus thread_status1, thread_status2, thread_status3;
   int thread_rc1, thread_rc2, thread_rc3;
   UINT32 priority_level;

   AsciiPrint("Testcase: Thread Unequal Priority on mutex wait\n");

   t = GetCurrentThread();
   cp = ThreadGetCurrCpu(t);
   previous_cpu = cp;

   shared_handshake_stage_g = 0;
   AsciiPrint("Initialize shared_handshake_stage_g to %d\n", shared_handshake_stage_g);

   AsciiPrint("Initialize shared_counter_g to %d\n", shared_counter_g);
   shared_counter_g = 0;

   AsciiPrint("Initialize mutex thread counter.\n");
   mutex_priority_compute_count1_g = 0;
   mutex_priority_compute_count2_g = 0;
   mutex_priority_compute_count3_g = 0;
   
   /* Initialize private lock of regular type (autounsignal on)*/
   AsciiPrint("Initialize mutex of count 1.\n");
   mutex1 = MutexInit(0);

   //test_cpu = previous_cpu; /* debug on core0 */
   test_cpu = Get_Next_CPU(previous_cpu);
   //previous_cpu = test_cpu;
   //test_cpu = Get_Next_CPU(previous_cpu);
   //previous_cpu = test_cpu;
   //test_cpu = Get_Next_CPU(previous_cpu);
   AsciiPrint ("Main Thread: Test threads running on CPU(%d)\n", test_cpu);

   /* Setup and generate Thread1 */
   //priority_level = DEFAULT_PRIORITY - 4;
   priority_level = DEFAULT_PRIORITY - 1;
   //priority_level = DEFAULT_PRIORITY;
   AsciiPrint ("Main Thread: Create Thread1 with priority_level(%d)\n", priority_level);
   mpt1 = ThreadCreate ("mutex_P1", &t_mutex_priority1, (void*)"mutex_P1", priority_level, DEFAULT_STACK_SIZE);
   ThreadSetPinnedCpu (mpt1, test_cpu);

   if (mpt1 == NULL)
   {
      AsciiPrint("Unable to create thread (t_mutex_priority1), exiting ...\n");
      return EFI_UNSUPPORTED;
   }

   //ThreadDetach (mpt1);
   ThreadResume (mpt1);

   /* Setup and generate Thread1 */
   priority_level = DEFAULT_PRIORITY;
   AsciiPrint ("Main Thread: Create Thread2 with priority_level(%d)\n", priority_level);
   mpt2 = ThreadCreate ("mutex_P2", &t_mutex_priority2, (void*)"mutex_P2", priority_level, DEFAULT_STACK_SIZE);
   ThreadSetPinnedCpu (mpt2, test_cpu);

   if (mpt2 == NULL)
   {
      AsciiPrint("Unable to create thread (t_mutex_priority2), exiting ...\n");
      return EFI_UNSUPPORTED;
   }

   //ThreadDetach (mpt2);
   ThreadResume (mpt2);

   /* Setup and generate Thread1 */
   //priority_level = DEFAULT_PRIORITY + 4;
   priority_level = DEFAULT_PRIORITY + 1;
   //priority_level = DEFAULT_PRIORITY;
   AsciiPrint ("Main Thread: Create Thread3 with priority_level(%d)\n", priority_level);
   mpt3 = ThreadCreate ("mutex_P3", &t_mutex_priority3, (void*)"mutex_P3", priority_level, DEFAULT_STACK_SIZE);
   ThreadSetPinnedCpu (mpt3, test_cpu);

   if (mpt3 == NULL)
   {
      AsciiPrint("Unable to create thread (t_mutex_priority3), exiting ...\n");
      return EFI_UNSUPPORTED;
   }

   //ThreadDetach (mpt3);
   ThreadResume (mpt3);

   /* wait for threads to exit and join */
   //thread_status1 = ThreadJoin(mpt1, &thread_rc1, 50000); /* napali */
   //thread_status2 = ThreadJoin(mpt2, &thread_rc2, 50000); /* napali */
   //thread_status3 = ThreadJoin(mpt3, &thread_rc3, 50000); /* napali */
   thread_status1 = ThreadJoin(mpt1, &thread_rc1, 2500000); /* hana x50 */
   thread_status2 = ThreadJoin(mpt2, &thread_rc2, 2500000); 
   thread_status3 = ThreadJoin(mpt3, &thread_rc3, 2500000);

   /* destroy mutex that was initialized */
   AsciiPrint("Main Thread: Destroy mutex.\n");
   MutexDestroy(mutex1);

   /* check pass/fail status */
   AsciiPrint ("\nMain Thread: Thr1 - thread_status(%d)\tthread_rc(%d)\n", thread_status1, thread_rc1);
   AsciiPrint ("\nMain Thread: Thr2 - thread_status(%d)\tthread_rc(%d)\n", thread_status2, thread_rc2);
   AsciiPrint ("\nMain Thread: Thr3 - thread_status(%d)\tthread_rc(%d)\n", thread_status3, thread_rc3);
   if (  (thread_status1 != 0)  || (thread_rc1 != 0) || 
         (thread_status2 != 0)  || (thread_rc2 != 0) || 
         (thread_status3 != 0)  || (thread_rc3 != 0)) {

      if (thread_status1 != 0) {
         AsciiPrint ("Thr1 - ThreadJoin time expired\n");
      }
      if (thread_status2 != 0) {
         AsciiPrint ("Thr2 - ThreadJoin time expired\n");
      }
      if (thread_status3 != 0) {
         AsciiPrint ("Thr3 - ThreadJoin time expired\n");
      }

      AsciiPrint ("Test FAILED!\n");
      return EFI_D_ERROR;
   }
   else
   {
      AsciiPrint ("Test PASSED!\n");
   }

   ThreadSleep (200);
   return EFI_SUCCESS;
   
}




int t_sem_priority1( IN VOID  *Arg)
{
   int cp;
   Thread *t;
   UINTN sem1_time_before, sem1_time_after;
   KStatus sem_status;
   //UINTN iteration_index;

   t = GetCurrentThread();
   cp = ThreadGetCurrCpu(t);

   AsciiPrint ("Executing Thread1 (t_sem_priority1) with ID: %d on CPU: %d\n", t, cp);

   ThreadSleep (100);

   shared_handshake_stage_g = 1;
   sem1_time_before = GetTimerCountms ();
   AsciiPrint ("Thread1 on CPU(%d): wait on sem at %d ms (shared_handshake_stage_g - %d)\n",  
                  cp, sem1_time_before, shared_handshake_stage_g);

   /* send first sem1 (+1) */
   AsciiPrint ("Thread1 on CPU(%d): About to get sem at %d ms (about to set shared_handshake_stage_g - 2)\n",  cp, sem1_time_before);
   sem1_time_after = GetTimerCountms ();
   sem_status = SemWait(sem1);
   shared_handshake_stage_g = 2;
   if (sem_status != 0) {
      AsciiPrint ("Thread1 on CPU(%d): DID NOT get sem (+1) first time at time: %d ms\n", cp, sem1_time_after);
      AsciiPrint ("Test FAILED!\n");
      ThreadExit(EFI_D_ERROR);
      return EFI_D_ERROR; 
   }


   /* wait for thread2 to start executing and thread3 to wait on mutex */ 
   sem_priority_compute_count1_g = 0;
   while ((sem_priority_compute_count2_g == 0) || (shared_handshake_stage_g != 4)) {
      sem_priority_compute_count1_g = 0;
   }

   //for (iteration_index = 0; iteration_index < compute_count_limit; iteration_index++) {
   while  (mutex_priority_compute_count1_g < 9999) {
      //AsciiPrint ("Thread1 on CPU (%d): iteration - %d\n", cp, iteration_index);
      mutex_priority_compute_count1_g++;
   }

   /* check thread1 finishes after thread2 and thread3 */
   if (shared_handshake_stage_g != 4)  {
      sem1_time_after = GetTimerCountms ();
      AsciiPrint ("\n\nThread1 on CPU(%d): DID NOT finish computing after Thread3 starts and Thread2 ends at %d ms.\n", cp, sem1_time_after);
      AsciiPrint ("Test FAILED!\n");
      ThreadExit(EFI_D_ERROR);
      return EFI_D_ERROR; 
   }

   shared_handshake_stage_g = 5;
   sem_status = SemPost(sem1,TRUE);
   AsciiPrint ("Thread1 on CPU(%d): post sem1 at %d ms (shared_handshake_stage_g: %d)\n", cp, sem1_time_after, shared_handshake_stage_g);
   /*
   if (sem_status != 0) {
      AsciiPrint ("Thread1 on CPU(%d): Semaphore post not successful (sem_status=%d)\n", cp, sem1_time_after, sem_status);
      AsciiPrint ("Test FAILED!\n");
      ThreadExit(EFI_D_ERROR);
      return EFI_D_ERROR; 
   }
   */

   sem1_time_after = GetTimerCountms ();
   AsciiPrint ("\n\nThread1 on CPU(%d): Finished computing after Thread3 starts and before Thread2 ends at %d ms.\n", cp, sem1_time_after);
   AsciiPrint ("\nThread1 on CPU(%d): Final sem_priority_compute_count1_g (%d)\n", cp, sem_priority_compute_count1_g);  

   ThreadSleep (200);
   AsciiPrint ("Exiting Thread1 (t_sem_priority1) with ID: %d on CPU: %d\n", t, cp);
   ThreadExit(0);
   return 0;
}


int t_sem_priority2( IN VOID  *Arg)
{
   int cp;
   Thread *t;
   UINTN sem2_time_before, sem2_time_after;
   //UINTN compute_count;
   //UINTN iteration_index;

   t = GetCurrentThread();
   cp = ThreadGetCurrCpu(t);

   AsciiPrint ("Executing Thread2 (t_sem_priority2) with ID: %d on CPU: %d\n", t, cp);

   /* wait for thread1 to start computing */
   ThreadSleep (200);
   sem2_time_before = GetTimerCountms ();

   /* check thread1 is computing */
   if (shared_handshake_stage_g != 2) {
      AsciiPrint ("\n\nThread2 on CPU(%d): Thread1 is not computing yet at %d ms.\n", cp ,sem2_time_before);
      AsciiPrint ("Test FAILED!\n");
      ThreadExit(EFI_D_ERROR);
      return EFI_D_ERROR; 
   }
   else {
      AsciiPrint ("Thread2 on CPU(%d): Thread1 is computing (got semaphore).\n\n", cp);
   }

   AsciiPrint ("Thread2 on CPU(%d): computing at %d ms (about to set shared_handshake_stage_g - %d)\n",  
                  cp, sem2_time_before, shared_handshake_stage_g);
   shared_handshake_stage_g = 3;

   sem_priority_compute_count2_g = 0;
   /* until thread3 completes */
   while (shared_handshake_stage_g != 7) {
      if (sem_priority_compute_count2_g == 2000000000) {
         sem_priority_compute_count2_g = 1;
      }
      else {
         sem_priority_compute_count2_g++;
      }
   }

   /* check thread2 finishes after thread1 and thread3 done computing */
   if (shared_handshake_stage_g != 7)  {
      sem2_time_after = GetTimerCountms ();
      //AsciiPrint ("\n\nThread3 on CPU(%d): DID NOT finish computing after Thread1 and Thread3 at %d ms.\n", cp ,sem2_time_before);
      AsciiPrint ("\n\nThread3 on CPU(%d): DID NOT finish computing after Thread3 wait on semaphore at %d ms.\n", cp ,sem2_time_after);
      AsciiPrint ("Test FAILED!\n");
      ThreadExit(EFI_D_ERROR);
      return EFI_D_ERROR; 
   }

   shared_handshake_stage_g = 8;
   sem2_time_after = GetTimerCountms ();
   AsciiPrint ("Thread2 on CPU(%d): ending at %d ms (shared_handshake_stage_g - %d)\n",  
                  cp, sem2_time_after, shared_handshake_stage_g);
   AsciiPrint ("\n\nThread2 on CPU(%d): Finished computing after Thread1 and Thread3 at %d ms.\n", cp ,sem2_time_after);
   AsciiPrint ("\nThread2 on CPU(%d): Final mutex_priority_compute_count2_g (%d)\n", cp, sem_priority_compute_count2_g);  

   ThreadSleep (200);
   AsciiPrint ("Exiting Thread2 (t_sem_priority2) with ID: %d on CPU: %d\n", t, cp);
   ThreadExit(0);
   return 0;
}


int t_sem_priority3( IN VOID  *Arg)
{
   int cp;
   Thread *t;
   UINTN sem3_time_before, sem3_time_after;
   KStatus sem_status;
   UINTN compute_count_limit;
   UINTN iteration_index;

   t = GetCurrentThread();
   cp = ThreadGetCurrCpu(t);

   AsciiPrint ("Executing Thread3 (t_sem_priority3) with ID: %d on CPU: %d\n", t, cp);

   /* wait for thread2 to start computing */
   ThreadSleep (300);

   /* check thread1 got sem */
   if (shared_handshake_stage_g != 3) {
      AsciiPrint ("\n\nThread3 on CPU(%d): Thread1 and thread2 have not started to execute.\n", cp);
      AsciiPrint ("Test FAILED!\n");
      ThreadExit(EFI_D_ERROR);
      return EFI_D_ERROR; 
   }
   else {
      AsciiPrint ("\n\nThread3 on CPU(%d): Thread1 and thread2 started to execute.\n", cp);
   }

   sem3_time_before = GetTimerCountms ();
   shared_handshake_stage_g = 4;
   AsciiPrint ("Thread3 on CPU(%d): wait on sem1  at %d ms (shared_handshake_stage_g: %d)\n", cp, sem3_time_before, shared_handshake_stage_g);
   sem_status = SemWait(sem1);

   sem3_time_after = GetTimerCountms ();
   /* check thread3 gets sem1 after thread1 finish computing */
   if (shared_handshake_stage_g != 5)  {
      AsciiPrint ("\n\nThread3 on CPU(%d): DID NOT get sem1 after thread1 finish computing at %d ms.\n", cp, sem3_time_before);
      AsciiPrint ("Test FAILED!\n");
      ThreadExit(EFI_D_ERROR);
      return EFI_D_ERROR; 
   }

   shared_handshake_stage_g = 6;
   //AsciiPrint ("Thread3 on CPU(%d): Got sem1 after thread1 finish computing at %d ms (shared_handshake_stage_g: %d)\n", 
   //               cp, sem3_time_after, shared_handshake_stage_g);

   compute_count_limit = 9;
   sem_priority_compute_count3_g = 0;
   for (iteration_index = 0; iteration_index < compute_count_limit; iteration_index++) {
      //AsciiPrint ("\t\t\t\t\t\tThread3 on CPU (%d): %d\n", cp, iteration_index);
      sem_priority_compute_count3_g++;
   }

   /* check thread3 finishes beforre thread2 */
   if (shared_handshake_stage_g != 6)  {
      sem3_time_after = GetTimerCountms ();
      AsciiPrint ("\n\nThread3 on CPU(%d): DID NOT finish computing before Thread2 at %d ms.\n", cp ,sem3_time_before);
      AsciiPrint ("Test FAILED!\n");
      ThreadExit(EFI_D_ERROR);
      return EFI_D_ERROR; 
   }

   shared_handshake_stage_g = 7;
   sem_status = SemPost(sem1,TRUE);
   sem3_time_after = GetTimerCountms ();
   AsciiPrint ("Thread3 on CPU(%d): post sem1 at %d ms (shared_handshake_stage_g: %d)\n", cp, sem3_time_after, shared_handshake_stage_g);
   /*
   if (sem_status != 0) {
      AsciiPrint ("Thread3 on CPU(%d): Semaphore post not successful (sem_status=%d)\n", cp, sem3_time_after, sem_status);
      AsciiPrint ("Test FAILED!\n");
      ThreadExit(EFI_D_ERROR);
      return EFI_D_ERROR; 
   }
   */

   sem3_time_after = GetTimerCountms ();
   AsciiPrint ("Thread3 on CPU(%d): Finished computing before Thread2 at %d ms (shared_handshake_stage_g = %d).\n",
                     cp, sem3_time_after, shared_handshake_stage_g);
   AsciiPrint ("\nThread3 on CPU(%d): Final sem_priority_compute_count3_g (%d)\n", cp, sem_priority_compute_count3_g);  

   ThreadSleep (200);
   AsciiPrint ("Exiting Thread3 (t_sem_priority3) with ID: %d on CPU: %d\n", t, cp);
   ThreadExit(0);
   return 0;
}


EFI_STATUS Sem_priority_equal_test(VOID) {
   UINTN cp, previous_cpu, test_cpu;
   Thread *t;
   KStatus thread_status1, thread_status2, thread_status3;
   int thread_rc1, thread_rc2, thread_rc3;
   UINT32 priority_level;

   AsciiPrint("Testcase: Thread Equal Priority on semaphore wait\n");

   t = GetCurrentThread();
   cp = ThreadGetCurrCpu(t);
   previous_cpu = cp;

   shared_handshake_stage_g = 0;
   AsciiPrint("Initialize shared_handshake_stage_g to %d\n", shared_handshake_stage_g);

   AsciiPrint("Initialize shared_counter_g to %d\n", shared_counter_g);
   shared_counter_g = 0;

   AsciiPrint("Initialize semaphore thread counter.\n");
   sem_priority_compute_count1_g = 0;
   sem_priority_compute_count2_g = 0;
   sem_priority_compute_count3_g = 0;

   /* Initialize private lock of regular type (autounsignal on)*/
   AsciiPrint("Initialize semaphore of count 1.\n");
   sem1 = SemInit(0, 1);

   test_cpu = previous_cpu;   /* debug on core0 */
   //test_cpu = Get_Next_CPU(previous_cpu);
   //previous_cpu = test_cpu;
   //test_cpu = Get_Next_CPU(previous_cpu);
   //previous_cpu = test_cpu;
   //test_cpu = Get_Next_CPU(previous_cpu);
   AsciiPrint ("Main Thread: Test threads running on CPU(%d)\n", test_cpu);

   /* Setup and generate Thread1 */
   //priority_level = DEFAULT_PRIORITY - 4;
   //priority_level = DEFAULT_PRIORITY - 1;
   priority_level = DEFAULT_PRIORITY;
   AsciiPrint ("Main Thread: Create Thread1 with priority_level(%d)\n", priority_level);
   mpt1 = ThreadCreate ("sem_P1", &t_sem_priority1, (void*)"sem_P1", priority_level, DEFAULT_STACK_SIZE);
   ThreadSetPinnedCpu (mpt1, test_cpu);

   if (mpt1 == NULL)
   {
      AsciiPrint("Unable to create thread (t_sem_priority1), exiting ...\n");
      return EFI_UNSUPPORTED;
   }

   //ThreadDetach (mpt1);
   ThreadResume (mpt1);

   /* Setup and generate Thread1 */
   priority_level = DEFAULT_PRIORITY;
   AsciiPrint ("Main Thread: Create Thread2 with priority_level(%d)\n", priority_level);
   mpt2 = ThreadCreate ("sem_P2", &t_sem_priority2, (void*)"sem_P2", priority_level, DEFAULT_STACK_SIZE);
   ThreadSetPinnedCpu (mpt2, test_cpu);

   if (mpt2 == NULL)
   {
      AsciiPrint("Unable to create thread (t_sem_priority2), exiting ...\n");
      return EFI_UNSUPPORTED;
   }

   //ThreadDetach (mpt2);
   ThreadResume (mpt2);

   /* Setup and generate Thread1 */
   //priority_level = DEFAULT_PRIORITY + 4;
   priority_level = DEFAULT_PRIORITY + 1;
   //priority_level = DEFAULT_PRIORITY;
   AsciiPrint ("Main Thread: Create Thread3 with priority_level(%d)\n", priority_level);
   mpt3 = ThreadCreate ("sem_P3", &t_sem_priority3, (void*)"sem_P3", priority_level, DEFAULT_STACK_SIZE);
   ThreadSetPinnedCpu (mpt3, test_cpu);

   if (mpt3 == NULL)
   {
      AsciiPrint("Unable to create thread (t_sem_priority3), exiting ...\n");
      return EFI_UNSUPPORTED;
   }

   //ThreadDetach (mpt3);
   ThreadResume (mpt3);

   /* wait for threads to exit and join */
   thread_status1 = ThreadJoin(mpt1, &thread_rc1, 50000);
   thread_status2 = ThreadJoin(mpt2, &thread_rc2, 50000);
   thread_status3 = ThreadJoin(mpt3, &thread_rc3, 50000);

   /* destroy sem that was initialized */
   AsciiPrint("Main Thread: Destroy semaphore.\n");
   SemDestroy(sem1);

   /* check pass/fail status */
   AsciiPrint ("\nMain Thread: Thr1 - thread_status(%d)\tthread_rc(%d)\n", thread_status1, thread_rc1);
   AsciiPrint ("\nMain Thread: Thr2 - thread_status(%d)\tthread_rc(%d)\n", thread_status2, thread_rc2);
   AsciiPrint ("\nMain Thread: Thr3 - thread_status(%d)\tthread_rc(%d)\n", thread_status3, thread_rc3);
   if (  (thread_status1 != 0)  || (thread_rc1 != 0) || 
         (thread_status2 != 0)  || (thread_rc2 != 0) || 
         (thread_status3 != 0)  || (thread_rc3 != 0)) {
      AsciiPrint ("Test FAILED!\n");
      return EFI_D_ERROR;
   }
   else
   {
      AsciiPrint ("Test PASSED!\n");
   }

   ThreadSleep (200);
   return EFI_SUCCESS;
   
}

EFI_STATUS Sem_priority_unequal_test(VOID) {
   UINTN cp, previous_cpu, test_cpu;
   Thread *t;
   KStatus thread_status1, thread_status2, thread_status3;
   int thread_rc1, thread_rc2, thread_rc3;
   UINT32 priority_level;

   AsciiPrint("Testcase: Thread Unequal Priority on semaphore wait\n");

   t = GetCurrentThread();
   cp = ThreadGetCurrCpu(t);
   previous_cpu = cp;

   shared_handshake_stage_g = 0;
   AsciiPrint("Initialize shared_handshake_stage_g to %d\n", shared_handshake_stage_g);

   AsciiPrint("Initialize shared_counter_g to %d\n", shared_counter_g);
   shared_counter_g = 0;

   AsciiPrint("Initialize semaphore thread counter.\n");
   sem_priority_compute_count1_g = 0;
   sem_priority_compute_count2_g = 0;
   sem_priority_compute_count3_g = 0;

   /* Initialize private lock of regular type (autounsignal on)*/
   AsciiPrint("Initialize semaphore of count 1.\n");
   sem1 = SemInit(0, 1);

   test_cpu = previous_cpu;   /* debug on core0 */
   //test_cpu = Get_Next_CPU(previous_cpu);
   //previous_cpu = test_cpu;
   //test_cpu = Get_Next_CPU(previous_cpu);
   //previous_cpu = test_cpu;
   //test_cpu = Get_Next_CPU(previous_cpu);
   AsciiPrint ("Main Thread: Test threads running on CPU(%d)\n", test_cpu);

   /* Setup and generate Thread1 */
   //priority_level = DEFAULT_PRIORITY - 4;
   priority_level = DEFAULT_PRIORITY - 1;
   //priority_level = DEFAULT_PRIORITY;
   AsciiPrint ("Main Thread: Create Thread1 with priority_level(%d)\n", priority_level);
   mpt1 = ThreadCreate ("sem_P1", &t_sem_priority1, (void*)"sem_P1", priority_level, DEFAULT_STACK_SIZE);
   ThreadSetPinnedCpu (mpt1, test_cpu);

   if (mpt1 == NULL)
   {
      AsciiPrint("Unable to create thread (t_sem_priority1), exiting ...\n");
      return EFI_UNSUPPORTED;
   }

   //ThreadDetach (mpt1);
   ThreadResume (mpt1);

   /* Setup and generate Thread1 */
   priority_level = DEFAULT_PRIORITY;
   AsciiPrint ("Main Thread: Create Thread2 with priority_level(%d)\n", priority_level);
   mpt2 = ThreadCreate ("sem_P2", &t_sem_priority2, (void*)"sem_P2", priority_level, DEFAULT_STACK_SIZE);
   ThreadSetPinnedCpu (mpt2, test_cpu);

   if (mpt2 == NULL)
   {
      AsciiPrint("Unable to create thread (t_sem_priority2), exiting ...\n");
      return EFI_UNSUPPORTED;
   }

   //ThreadDetach (mpt2);
   ThreadResume (mpt2);

   /* Setup and generate Thread1 */
   //priority_level = DEFAULT_PRIORITY + 4;
   priority_level = DEFAULT_PRIORITY + 1;
   //priority_level = DEFAULT_PRIORITY;
   AsciiPrint ("Main Thread: Create Thread3 with priority_level(%d)\n", priority_level);
   mpt3 = ThreadCreate ("sem_P3", &t_sem_priority3, (void*)"sem_P3", priority_level, DEFAULT_STACK_SIZE);
   ThreadSetPinnedCpu (mpt3, test_cpu);

   if (mpt3 == NULL)
   {
      AsciiPrint("Unable to create thread (t_sem_priority3), exiting ...\n");
      return EFI_UNSUPPORTED;
   }

   //ThreadDetach (mpt3);
   ThreadResume (mpt3);

   /* wait for threads to exit and join */
   thread_status1 = ThreadJoin(mpt1, &thread_rc1, 50000);
   thread_status2 = ThreadJoin(mpt2, &thread_rc2, 50000);
   thread_status3 = ThreadJoin(mpt3, &thread_rc3, 50000);

   /* destroy sem that was initialized */
   AsciiPrint("Main Thread: Destroy semaphore.\n");
   SemDestroy(sem1);

   /* check pass/fail status */
   AsciiPrint ("\nMain Thread: Thr1 - thread_status(%d)\tthread_rc(%d)\n", thread_status1, thread_rc1);
   AsciiPrint ("\nMain Thread: Thr2 - thread_status(%d)\tthread_rc(%d)\n", thread_status2, thread_rc2);
   AsciiPrint ("\nMain Thread: Thr3 - thread_status(%d)\tthread_rc(%d)\n", thread_status3, thread_rc3);
   if (  (thread_status1 != 0)  || (thread_rc1 != 0) || 
         (thread_status2 != 0)  || (thread_rc2 != 0) || 
         (thread_status3 != 0)  || (thread_rc3 != 0)) {
      AsciiPrint ("Test FAILED!\n");
      return EFI_D_ERROR;
   }
   else
   {
      AsciiPrint ("Test PASSED!\n");
   }

   ThreadSleep (200);
   return EFI_SUCCESS;
   
}




/*
int t_cpu_any1( IN VOID  *Arg)
{
   int cp;
   Thread *t;
   UINTN curr_time;
   UINTN compute_count;
   UINTN iteration_index;

   t = GetCurrentThread();
   cp = ThreadGetCurrCpu(t);

   AsciiPrint ("Executing Thread1 (t_cput_any1) with ID: %d on CPU: %d\n", t, cp);

   ThreadSleep (100);

   compute_count = 999;  // rc error
   //compute_count = 199;  // ok
   //compute_count = 499;  // ok 
   //compute_count = 799;  // rc err (-13) 
   for (iteration_index = 0; iteration_index < compute_count; iteration_index++) {
      cp = ThreadGetCurrCpu(t);
      curr_time = GetTimerCountms ();
      AsciiPrint ("Thread1 on CPU(%d) iteration(%d) at %d ms\n",  cp, iteration_index, curr_time);
      ThreadSleep (20);
      //ThreadSleep (50);
   }

   ThreadSleep (200);
   AsciiPrint ("Exiting Thread1 (t_cpu_any1) with ID: %d on CPU: %d\n", t, cp);
   ThreadExit(0);
   return 0;
}

int t_cpu_any2( IN VOID  *Arg)
{
   int cp;
   Thread *t;
   UINTN curr_time;
   UINTN compute_count;
   UINTN iteration_index;

   t = GetCurrentThread();
   cp = ThreadGetCurrCpu(t);

   AsciiPrint ("Executing Thread2 (t_cput_any2) with ID: %d on CPU: %d\n", t, cp);

   ThreadSleep (100);

   compute_count = 999;
   //compute_count = 199;
   //compute_count = 499;  // 
   //compute_count = 799;  // 
   for (iteration_index = 0; iteration_index < compute_count; iteration_index++) {
      cp = ThreadGetCurrCpu(t);
      curr_time = GetTimerCountms ();
      AsciiPrint ("Thread2 on CPU(%d) iteration(%d) at %d ms\n",  cp, iteration_index, curr_time);
      ThreadSleep (20);
      //ThreadSleep (50);
   }

   ThreadSleep (200);
   AsciiPrint ("Exiting Thread2 (t_cpu_any2) with ID: %d on CPU: %d\n", t, cp);
   ThreadExit(0);
   return 0;
}

EFI_STATUS Cpu_any_test(VOID) {
   UINTN cp;
   Thread *t;
   KStatus thread_status1, thread_status2;
   int thread_rc1, thread_rc2;

   AsciiPrint("Testcase: Thread on any CPU\n");

   t = GetCurrentThread();
   cp = ThreadGetCurrCpu(t);
   AsciiPrint ("Main Thread: running on CPU(%d)\n", cp);

   // Setup and generate Thread1 
   AsciiPrint ("Main Thread: Create Thread1\n");
   mpt1 = ThreadCreate ("cpu_A1", &t_cpu_any1, (void*)"cpu_A1", DEFAULT_PRIORITY, DEFAULT_STACK_SIZE);

   if (mpt1 == NULL)
   {
      AsciiPrint("Unable to create thread (t_cpu_any1), exiting ...\n");
      return EFI_UNSUPPORTED;
   }

   ThreadResume (mpt1);

   // Setup and generate Thread1 
   AsciiPrint ("Main Thread: Create Thread2\n");
   mpt2 = ThreadCreate ("cpu_A2", &t_cpu_any2, (void*)"cpu_A2", DEFAULT_PRIORITY, DEFAULT_STACK_SIZE);

   if (mpt2 == NULL)
   {
      AsciiPrint("Unable to create thread (t_cpu_any2), exiting ...\n");
      return EFI_UNSUPPORTED;
   }

   ThreadResume (mpt2);

   //ThreadSleep (100);

   // wait for threads to exit and join /
   thread_status1 = ThreadJoin(mpt1, &thread_rc1, 50000);
   thread_status2 = ThreadJoin(mpt2, &thread_rc2, 50000);

   // check pass/fail status 
   AsciiPrint ("\nMain Thread: Thr1 - thread_status(%d)\tthread_rc(%d)\n", thread_status1, thread_rc1);
   AsciiPrint ("\nMain Thread: Thr2 - thread_status(%d)\tthread_rc(%d)\n", thread_status2, thread_rc2);
   if (  (thread_status1 != 0)  || (thread_rc1 != 0) || 
         (thread_status2 != 0)  || (thread_rc2 != 0) ) {
      AsciiPrint ("Test FAILED!\n");
      return EFI_D_ERROR;
   }
   else
   {
      AsciiPrint ("Test PASSED!\n");
   }

   ThreadSleep (200);
   return EFI_SUCCESS;
   
}

*/

int t_cpu_any_x( IN VOID  *Arg)
{
   int cp_old, cp_curr;
   Thread *t;
   thread_config* thr_cfg = (thread_config*)Arg;
   UINTN thread_index = (thr_cfg->thread_index);
   UINTN compute_count;
   UINTN iteration_index;
   //UINTN curr_time;


   t = GetCurrentThread();
   cp_old = ThreadGetCurrCpu(t);
   cp_curr = cp_old;

   AsciiPrint ("Executing t_cpu_any_x[%d]  on CPU: %d\n", thread_index, cp_curr);

   //ThreadSleep (thread_initial_wait);

   /* add current thread to order of obtaining semaphore */
   //shared_order_group_g[shared_order_ptr] = thread_index;
   //shared_order_ptr++;

   compute_count = 99;
   for (iteration_index = 0; iteration_index < compute_count; iteration_index++) {

      ThreadSleep (20);

      /* get cpu to see if changed */
      cp_curr = ThreadGetCurrCpu(t);

      if (cp_curr < 0) {
         AsciiPrint ("ERROR: ThreadGetCurrCpu() returned (%d)\n", cp_curr);
         return 1;
      }
      
      if (cp_curr != cp_old) {
         shared_cpu_diff_group_g[thread_index]++;
      }

      AsciiPrint ("%aT%dC%dI%d\n",  spstr[cp_curr], thread_index, cp_curr, iteration_index);

      /* back up cp_curr to cp_old */
      cp_old = cp_curr;

    }

   //shared_counter_group_g[thread_index] = 1000 + (1000*thread_index);
   //AsciiPrint ("\nThread t_cpu_any_x[%d]) updated counter to %d.\n", thread_index, shared_counter_group_g[thread_index]);


   ThreadSleep (200);
   AsciiPrint ("Exiting t_cpu_any_x[%d] on CPU: %d\n\n", thread_index, cp_curr);
   ThreadExit(0);
   return 0;
}

EFI_STATUS Cpu_any_test(VOID) {
   UINTN thr_index, num_of_threads;
   //UINTN cp, previous_cpu, next_cpu;
   UINTN cp, previous_cpu;
   Thread *t;
   KStatus thread_status;
   int thread_rc;
   UINTN thr_create_time;

   AsciiPrint("Testcase: CPU Current\n");

   t = GetCurrentThread();
   cp = ThreadGetCurrCpu(t);
   previous_cpu = cp;

   shared_handshake_stage_g = 0;
   AsciiPrint("Initialize shared_handshake_stage_g to %d\n", shared_handshake_stage_g);

   AsciiPrint("Initialize shared_counter_g to %d\n", shared_counter_g);
   shared_counter_g = 0;

   num_of_threads = 8;
   //num_of_threads = 4;

   AsciiStrnCpy((CHAR8 *)(thr_config[0].thread_name), "CPU_C0", TEST_CONFIG_STR_LEN - 1);
   AsciiStrnCpy((CHAR8 *)(thr_config[1].thread_name), "CPU_C1", TEST_CONFIG_STR_LEN - 1);
   AsciiStrnCpy((CHAR8 *)(thr_config[2].thread_name), "CPU_C2", TEST_CONFIG_STR_LEN - 1);
   AsciiStrnCpy((CHAR8 *)(thr_config[3].thread_name), "CPU_C3", TEST_CONFIG_STR_LEN - 1);
   AsciiStrnCpy((CHAR8 *)(thr_config[4].thread_name), "CPU_C4", TEST_CONFIG_STR_LEN - 1);
   AsciiStrnCpy((CHAR8 *)(thr_config[5].thread_name), "CPU_C5", TEST_CONFIG_STR_LEN - 1);
   AsciiStrnCpy((CHAR8 *)(thr_config[6].thread_name), "CPU_C6", TEST_CONFIG_STR_LEN - 1);
   AsciiStrnCpy((CHAR8 *)(thr_config[7].thread_name), "CPU_C7", TEST_CONFIG_STR_LEN - 1);

   /* initialize flag that confirms thread had moved to different cpu */
   for (thr_index=0; thr_index < num_of_threads; thr_index++) {
      shared_cpu_diff_group_g[thr_index] = 0;
   }

   /* Generate and setup Thread1 */
   for (thr_index=0; thr_index < num_of_threads; thr_index++) {

      thr_config[thr_index].thread_index = thr_index;

      thr_create_time = GetTimerCountms ();
      AsciiPrint("Creating thread[%d] at %d ms\n", thr_index, thr_create_time);

      mpt_group[thr_index] = ThreadCreate ( (CHAR8 *)(thr_config[thr_index].thread_name), &t_cpu_any_x, 
                                            (void*)&(thr_config[thr_index]), DEFAULT_PRIORITY, DEFAULT_STACK_SIZE);
      if (mpt_group[thr_index] == NULL)
      {
         AsciiPrint("Unable to create thread mtp_group[%d], exiting ...\n", thr_index);
         return EFI_UNSUPPORTED;
      }

      //next_cpu = Get_Next_CPU(previous_cpu);
      //AsciiPrint ("Previous CPU: %d Next CPU: %d\n", previous_cpu, next_cpu);
      //previous_cpu = next_cpu;
      //ThreadSetPinnedCpu (mpt_group[thr_index], next_cpu);

      ThreadResume (mpt_group[thr_index]);

   }

   AsciiPrint ("Thread main: wait for all threads to exit.\n");
   for (thr_index=0; thr_index < num_of_threads; thr_index++) {
      /* wait for thread to join or timeout in 10000 */
      //thread_status = ThreadJoin(mpt_group[thr_index], &thread_rc, 10000); /* napali */
      thread_status = ThreadJoin(mpt_group[thr_index], &thread_rc, 300000); /* hana x30 */
      AsciiPrint ("Main thread: t_cpu_any_x[%d] thread_status(%d)\tthread_rc(%d)\n", 
                           thr_index, thread_status , thread_rc);
      if ((thread_status != 0)|| (thread_rc != 0)) {
         AsciiPrint ("Main Thread: t_cpu_any_x[%d] did not complete successfully!\n", thr_index);
         AsciiPrint ("Test FAILED!\n");
         return EFI_D_ERROR;
      }
   }

   /* Pass criteria check:
    *    - each thread had cpu changed.
    * */ 
   AsciiPrint ("\nThread main - shared_cpu_diff_group_g:\n");
   for (thr_index=0; thr_index < num_of_threads; thr_index++) {
      if (shared_cpu_diff_group_g[thr_index] == 0) {
         AsciiPrint("\tTest incomplete: Thread[%x] always on same CPU.\n", thr_index);
         AsciiPrint("\tshared_cpu_diff_group_g[%d] = %d\n", thr_index, shared_cpu_diff_group_g[thr_index]);
         AsciiPrint ("\nTest FAILED!\n");
         return EFI_D_ERROR;
      }
      else {
         AsciiPrint("\tshared_cpu_diff_group_g[%d] = %d\n", thr_index, shared_cpu_diff_group_g[thr_index]);
      }
   }

   AsciiPrint ("\n\nTest PASSED!\n");
   ThreadSleep (200);
   return EFI_SUCCESS;
}


int t_cpu_current_x( IN VOID  *Arg)
{
   int cp_curr, cp_test;
   Thread *t;
   thread_config* thr_cfg = (thread_config*)Arg;
   UINTN thread_index = (thr_cfg->thread_index);
   UINTN compute_count;
   UINTN iteration_index;
   //UINTN curr_time;


   t = GetCurrentThread();
   //cp_test = ThreadGetCurrCpu(t);
   cp_test = MpcoreGetCurrCpu();
   cp_curr = cp_test;

   AsciiPrint ("Executing t_cpu_current_x[%d]  on CPU: %d\n", thread_index, cp_curr);

   //ThreadSleep (thread_initial_wait);

   /* add current thread to order of obtaining semaphore */
   //shared_order_group_g[shared_order_ptr] = thread_index;
   //shared_order_ptr++;

   compute_count = 99;
   for (iteration_index = 0; iteration_index < compute_count; iteration_index++) {

      ThreadSleep (20);

      /* get cpu to see if changed */
      //cp_curr = ThreadGetCurrCpu(t);
      cp_curr = MpcoreGetCurrCpu();

      if (cp_curr != cp_test) {
         shared_cpu_diff_group_g[thread_index]++;
      }
      else {
         shared_cpu_same_group_g[thread_index]++;
      }


      AsciiPrint ("%aT%dC%dI%d\n",  spstr[cp_curr], thread_index, cp_curr, iteration_index);

      /* set back to test cpu (orig) */
      ThreadSetCurrCpu(t, cp_test);
      /* get cpu instantly to check (before sleep) */
      //cp_curr = ThreadGetCurrCpu(t);
      cp_curr = MpcoreGetCurrCpu();

      if (cp_curr != cp_test) {
         shared_cpu_diff_group_2_g[thread_index]++;
      }
      else {
         shared_cpu_same_group_2_g[thread_index]++;
      }
   }

   //shared_counter_group_g[thread_index] = 1000 + (1000*thread_index);
   //AsciiPrint ("\nThread t_cput_current_x[%d]) updated counter to %d.\n", thread_index, shared_counter_group_g[thread_index]);


   ThreadSleep (200);
   AsciiPrint ("Exiting t_cpu_current_x[%d] on CPU: %d\n\n", thread_index, cp_curr);
   ThreadExit(0);
   return 0;
}

EFI_STATUS Cpu_current_test(VOID) {
   UINTN thr_index, num_of_threads;
   UINTN cp, previous_cpu, next_cpu;
   Thread *t;
   KStatus thread_status;
   int thread_rc;
   UINTN thr_create_time;

   AsciiPrint("Testcase: CPU Current\n");

   t = GetCurrentThread();
   //cp = ThreadGetCurrCpu(t);
   cp = MpcoreGetCurrCpu();
   previous_cpu = cp;

   shared_handshake_stage_g = 0;
   AsciiPrint("Initialize shared_handshake_stage_g to %d\n", shared_handshake_stage_g);

   AsciiPrint("Initialize shared_counter_g to %d\n", shared_counter_g);
   shared_counter_g = 0;

   //num_of_threads = 8;
   num_of_threads = 4;

   AsciiStrnCpy((CHAR8 *)(thr_config[0].thread_name), "CPU_C0", TEST_CONFIG_STR_LEN - 1);
   AsciiStrnCpy((CHAR8 *)(thr_config[1].thread_name), "CPU_C1", TEST_CONFIG_STR_LEN - 1);
   AsciiStrnCpy((CHAR8 *)(thr_config[2].thread_name), "CPU_C2", TEST_CONFIG_STR_LEN - 1);
   AsciiStrnCpy((CHAR8 *)(thr_config[3].thread_name), "CPU_C3", TEST_CONFIG_STR_LEN - 1);
   AsciiStrnCpy((CHAR8 *)(thr_config[4].thread_name), "CPU_C4", TEST_CONFIG_STR_LEN - 1);
   AsciiStrnCpy((CHAR8 *)(thr_config[5].thread_name), "CPU_C5", TEST_CONFIG_STR_LEN - 1);
   AsciiStrnCpy((CHAR8 *)(thr_config[6].thread_name), "CPU_C6", TEST_CONFIG_STR_LEN - 1);
   AsciiStrnCpy((CHAR8 *)(thr_config[7].thread_name), "CPU_C7", TEST_CONFIG_STR_LEN - 1);

   /* initialize flag that confirms thread had moved to different cpu */
   for (thr_index=0; thr_index < num_of_threads; thr_index++) {
      shared_cpu_diff_group_g[thr_index] = 0;
      shared_cpu_same_group_g[thr_index] = 0;
      shared_cpu_diff_group_2_g[thr_index] = 0;
      shared_cpu_same_group_2_g[thr_index] = 0;
   }

   /* Generate and setup Thread1 */
   for (thr_index=0; thr_index < num_of_threads; thr_index++) {

      thr_config[thr_index].thread_index = thr_index;

      thr_create_time = GetTimerCountms ();
      AsciiPrint("Creating thread[%d] at %d ms\n", thr_index, thr_create_time);

      mpt_group[thr_index] = ThreadCreate ( (CHAR8 *)(thr_config[thr_index].thread_name), &t_cpu_current_x, 
                                            (void*)&(thr_config[thr_index]), DEFAULT_PRIORITY, DEFAULT_STACK_SIZE);
      if (mpt_group[thr_index] == NULL)
      {
         AsciiPrint("Unable to create thread mtp_group[%d], exiting ...\n", thr_index);
         return EFI_UNSUPPORTED;
      }

      /* set current thread at index to next cpu */
      next_cpu = Get_Next_CPU(previous_cpu);
      //AsciiPrint ("Previous CPU: %d Next CPU: %d\n", previous_cpu, next_cpu);
      previous_cpu = next_cpu;
      ThreadSetCurrCpu(mpt_group[thr_index], next_cpu);
      //ThreadSetPinnedCpu (mpt_group[thr_index], next_cpu);

      ThreadResume (mpt_group[thr_index]);
   }

   AsciiPrint ("Thread main: wait for all threads to exit.\n");
   for (thr_index=0; thr_index < num_of_threads; thr_index++) {
      /* wait for thread to join or timeout in 10000 */
      //thread_status = ThreadJoin(mpt_group[thr_index], &thread_rc, 10000); /* napali */
      thread_status = ThreadJoin(mpt_group[thr_index], &thread_rc, 300000); /* hana x30 */
      AsciiPrint ("Main thread: t_cpu_current_x[%d] thread_status(%d)\tthread_rc(%d)\n", 
                           thr_index, thread_status , thread_rc);
      if ((thread_status != 0)|| (thread_rc != 0)) {
         AsciiPrint ("Main Thread: t_cpu_current_x[%d] did not complete successfully!\n", thr_index);
         AsciiPrint ("Test FAILED!\n");
         return EFI_D_ERROR;
      }
   }

   /* Pass criteria check:
    *    - each thread had cpu changed.
    * */ 
   AsciiPrint ("Thread main - shared_cpu (after sleep):\n");
   for (thr_index=0; thr_index < num_of_threads; thr_index++) {
      AsciiPrint("\tshared_cpu_diff_group_g[%d] = %d\tshared_cpu_same_group_g[%d] = %d\n", 
                     thr_index, shared_cpu_diff_group_g[thr_index], thr_index, shared_cpu_same_group_g[thr_index]);
   }
   AsciiPrint ("Thread main - shared_cpu 2 (back-to-back set-get):\n");
   for (thr_index=0; thr_index < num_of_threads; thr_index++) {
      AsciiPrint("\tshared_cpu_diff_group_2_g[%d] = %d\tshared_cpu_same_group_2_g[%d] = %d\n", 
                     thr_index, shared_cpu_diff_group_2_g[thr_index], thr_index, shared_cpu_same_group_2_g[thr_index]);
   }

   for (thr_index=0; thr_index < num_of_threads; thr_index++) {
      if (shared_cpu_diff_group_2_g[thr_index] == 0)  {
         AsciiPrint("Thread main - Thread[%x] was on same CPU.\n", thr_index);
         AsciiPrint("\tshared_cpu_diff_group_2_g[%d] = %d\tshared_cpu_same_group_2_g[%d] = %d\n", 
                     thr_index, shared_cpu_diff_group_2_g[thr_index], thr_index, shared_cpu_same_group_2_g[thr_index]);
         AsciiPrint ("\nTest FAILED!\n");
         return EFI_D_ERROR;
      }
   }

   AsciiPrint ("\n\nTest PASSED!\n");
   ThreadSleep (200);
   return EFI_SUCCESS;
}



int t_cpu_pin_x( IN VOID  *Arg)
{
   int cp_curr, cp_test, cp_curr_tmp;
   Thread *t;
   thread_config* thr_cfg = (thread_config*)Arg;
   UINTN thread_index = (thr_cfg->thread_index);
   UINTN compute_count;
   UINTN iteration_index;
   //UINTN curr_time;


   t = GetCurrentThread();
   //cp_test = ThreadGetCurrCpu(t);
   cp_test = MpcoreGetCurrCpu();
   cp_curr = cp_test;

   AsciiPrint ("Executing t_cpu_pin_x[%d]  on CPU: %d\n", thread_index, cp_curr);

   //ThreadSleep (thread_initial_wait);

   /* add current thread to order of obtaining semaphore */
   //shared_order_group_g[shared_order_ptr] = thread_index;
   //shared_order_ptr++;

   compute_count = 99;
   for (iteration_index = 0; iteration_index < compute_count; iteration_index++) {

      ThreadSleep (20);

      /* get cpu to see if changed */
      //cp_curr = ThreadGetCurrCpu(t);
      cp_curr = MpcoreGetCurrCpu();

      if (cp_curr != cp_test) {
         shared_cpu_diff_group_g[thread_index]++;
      }
      else {
         shared_cpu_same_group_g[thread_index]++;
      }

      /* check ThreadGetCurrCpu returns same CPU */
      cp_curr_tmp = ThreadGetCurrCpu(t); 
      if (cp_curr != cp_curr_tmp) {
         AsciiPrint("Thread[%x]: ThreadGetCurrCpu DID NOT return pinned CPU.\n", thread_index);
         AsciiPrint("\tThreadGetCurrCpu returned (%d) and MpcoreGetCurrCpu returned (%d)\n", cp_curr_tmp, cp_curr); 
         AsciiPrint ("\nTest FAILED!\n");
         return EFI_D_ERROR;
      }

      /* check ThreadGetCurrCpu returns same CPU */
      cp_curr_tmp = ThreadGetPinnedCpu(t); 
      if (cp_curr != cp_curr_tmp) {
         AsciiPrint("Thread[%x]: ThreadGetPinnedCpu DID NOT return pinned CPU.\n", thread_index);
         AsciiPrint("\tThreadGetPinnedCpu returned (%d) and MpcoreGetCurrCpu returned (%d)\n", cp_curr_tmp, cp_curr); 
         AsciiPrint ("\nTest FAILED!\n");
         return EFI_D_ERROR;
      }

      AsciiPrint ("%aT%dC%dI%d\n",  spstr[cp_curr], thread_index, cp_curr, iteration_index);

   }

   //shared_counter_group_g[thread_index] = 1000 + (1000*thread_index);
   //AsciiPrint ("\nThread t_cput_current_x[%d]) updated counter to %d.\n", thread_index, shared_counter_group_g[thread_index]);


   ThreadSleep (200);
   AsciiPrint ("Exiting t_cpu_pin_x[%d] with ID: %d on CPU: %d\n\n", thread_index, t, cp_curr);
   ThreadExit(0);
   return 0;
}

EFI_STATUS Cpu_pin_test(VOID) {
   UINTN thr_index, num_of_threads;
   UINTN cp, previous_cpu, next_cpu;
   Thread *t;
   KStatus thread_status;
   int thread_rc;
   UINTN thr_create_time;

   AsciiPrint("Testcase: CPU Pinned\n");

   t = GetCurrentThread();
   //cp = ThreadGetCurrCpu(t);
   cp = MpcoreGetCurrCpu();
   previous_cpu = cp;

   shared_handshake_stage_g = 0;
   AsciiPrint("Initialize shared_handshake_stage_g to %d\n", shared_handshake_stage_g);

   AsciiPrint("Initialize shared_counter_g to %d\n", shared_counter_g);
   shared_counter_g = 0;

   num_of_threads = 8;
   //num_of_threads = 4;

   AsciiStrnCpy((CHAR8 *)(thr_config[0].thread_name), "CPU_C0", TEST_CONFIG_STR_LEN - 1);
   AsciiStrnCpy((CHAR8 *)(thr_config[1].thread_name), "CPU_C1", TEST_CONFIG_STR_LEN - 1);
   AsciiStrnCpy((CHAR8 *)(thr_config[2].thread_name), "CPU_C2", TEST_CONFIG_STR_LEN - 1);
   AsciiStrnCpy((CHAR8 *)(thr_config[3].thread_name), "CPU_C3", TEST_CONFIG_STR_LEN - 1);
   AsciiStrnCpy((CHAR8 *)(thr_config[4].thread_name), "CPU_C4", TEST_CONFIG_STR_LEN - 1);
   AsciiStrnCpy((CHAR8 *)(thr_config[5].thread_name), "CPU_C5", TEST_CONFIG_STR_LEN - 1);
   AsciiStrnCpy((CHAR8 *)(thr_config[6].thread_name), "CPU_C6", TEST_CONFIG_STR_LEN - 1);
   AsciiStrnCpy((CHAR8 *)(thr_config[7].thread_name), "CPU_C7", TEST_CONFIG_STR_LEN - 1);

   /* initialize flag that confirms thread had moved to different cpu */
   for (thr_index=0; thr_index < num_of_threads; thr_index++) {
      shared_cpu_diff_group_g[thr_index] = 0;
      shared_cpu_same_group_g[thr_index] = 0;
   }

   /* Generate and setup Thread1 */
   for (thr_index=0; thr_index < num_of_threads; thr_index++) {

      thr_config[thr_index].thread_index = thr_index;

      thr_create_time = GetTimerCountms ();
      AsciiPrint("Creating thread[%d] at %d ms\n", thr_index, thr_create_time);

      mpt_group[thr_index] = ThreadCreate ( (CHAR8 *)(thr_config[thr_index].thread_name), &t_cpu_current_x, 
                                            (void*)&(thr_config[thr_index]), DEFAULT_PRIORITY, DEFAULT_STACK_SIZE);
      if (mpt_group[thr_index] == NULL)
      {
         AsciiPrint("Unable to create thread mtp_group[%d], exiting ...\n", thr_index);
         return EFI_UNSUPPORTED;
      }

      /* set current thread at index to next cpu */
      next_cpu = Get_Next_CPU(previous_cpu);
      //AsciiPrint ("Previous CPU: %d Next CPU: %d\n", previous_cpu, next_cpu);
      previous_cpu = next_cpu;
      ThreadSetPinnedCpu (mpt_group[thr_index], next_cpu);

      ThreadResume (mpt_group[thr_index]);
   }

   AsciiPrint ("Thread main: wait for all threads to exit.\n");
   for (thr_index=0; thr_index < num_of_threads; thr_index++) {
      /* wait for thread to join or timeout in 10000 */
      //thread_status = ThreadJoin(mpt_group[thr_index], &thread_rc, 10000); /* napali */
      thread_status = ThreadJoin(mpt_group[thr_index], &thread_rc, 300000); /* hana x30 */
      AsciiPrint ("Main thread: t_cpu_current_x[%d] thread_status(%d)\tthread_rc(%d)\n", 
                           thr_index, thread_status , thread_rc);
      if ((thread_status != 0)|| (thread_rc != 0)) {
         AsciiPrint ("Main Thread: t_cpu_current_x[%d] did not complete successfully!\n", thr_index);
         AsciiPrint ("Test FAILED!\n");
         return EFI_D_ERROR;
      }
   }

   /* Dump cpu info for each thread.
    */ 
   AsciiPrint ("\nThread main - shared_cpu (after sleep):\n");
   for (thr_index=0; thr_index < num_of_threads; thr_index++) {
      AsciiPrint("\tshared_cpu_diff_group_g[%d] = %d\tshared_cpu_same_group_g[%d] = %d\n", 
                     thr_index, shared_cpu_diff_group_g[thr_index], thr_index, shared_cpu_same_group_g[thr_index]);
   }

   /* Pass criteria check:
    *    - each thread had cpu changed.
    * */ 
   AsciiPrint ("Thread main - shared_cpu at end of iterations:\n");
   for (thr_index=0; thr_index < num_of_threads; thr_index++) {
      if ((shared_cpu_same_group_g[thr_index] == 0) || (shared_cpu_diff_group_g[thr_index] != 0)) {
         AsciiPrint("   shared_cpu_diff_group_g[%d] = %d\tshared_cpu_same_group_g[%d] = %d\n", 
                     thr_index, shared_cpu_diff_group_g[thr_index], thr_index, shared_cpu_same_group_g[thr_index]);
         AsciiPrint("\tTest incomplete: Thread[%x] was not on same CPU.\n", thr_index);
         AsciiPrint ("\nTest FAILED!\n");
         return EFI_D_ERROR;
      }
   }

   AsciiPrint ("\n\nTest PASSED!\n");
   ThreadSleep (200);
   return EFI_SUCCESS;
}


int t_thread_priority1( IN VOID  *Arg)
{
   int cp;
   Thread *t;
   UINTN compute_count;
   UINTN iteration_index;
   UINT32 priority_level;
   priority_level = DEFAULT_PRIORITY - 4;

   t = GetCurrentThread();
   cp = ThreadGetCurrCpu(t);

   AsciiPrint ("Executing Thread1 (t_thread_priority1) with ID: %d on CPU: %d\n", t, cp);

   compute_count = 99;
   for (iteration_index = 0; iteration_index < compute_count; iteration_index++) {
      AsciiPrint ("Thr1-1 on CPU(%d): %d\n", cp, iteration_index);
   }

   ThreadSetPriority(priority_level);
   ThreadSetName("Thread1_P_low");

   for (iteration_index = 0; iteration_index < compute_count; iteration_index++) {
      AsciiPrint ("Thr1-2 on CPU(%d): %d\n", cp, iteration_index);
   }

   ThreadSleep (200);
   AsciiPrint ("Exiting Thread1 (t_thread_priority1) with ID: %d on CPU: %d\n", t, cp);
   ThreadExit(0);
   return 0;
}

int t_thread_priority2( IN VOID  *Arg)
{
   int cp;
   Thread *t;
   UINTN compute_count;
   UINTN iteration_index;
   UINT32 priority_level;
   priority_level = DEFAULT_PRIORITY;

   t = GetCurrentThread();
   cp = ThreadGetCurrCpu(t);

   AsciiPrint ("Executing Thread2 (t_thread_priority2) with ID: %d on CPU: %d\n", t, cp);

   compute_count = 99;
   for (iteration_index = 0; iteration_index < compute_count; iteration_index++) {
      AsciiPrint ("\t\t\tThr2-1 on CPU(%d): %d\n", cp, iteration_index);
   }

   ThreadSetPriority(priority_level);
   ThreadSetName("Thread2_P_med");

   for (iteration_index = 0; iteration_index < compute_count; iteration_index++) {
      AsciiPrint ("\t\t\tThr2-2 on CPU (%d): %d\n", cp, iteration_index);
   }

   ThreadSleep (200);
   AsciiPrint ("Exiting Thread2 (t_thread_priority2) with ID: %d on CPU: %d\n", t, cp);
   ThreadExit(0);
   return 0;
}

int t_thread_priority3( IN VOID  *Arg)
{
   int cp;
   Thread *t;
   UINTN compute_count;
   UINTN iteration_index;
   UINT32 priority_level;
   priority_level = DEFAULT_PRIORITY + 4;

   t = GetCurrentThread();
   cp = ThreadGetCurrCpu(t);

   AsciiPrint ("Executing Thread3 (t_thread_priority3) with ID: %d on CPU: %d\n", t, cp);

   compute_count = 99;
   for (iteration_index = 0; iteration_index < compute_count; iteration_index++) {
      AsciiPrint ("\t\t\t\t\t\tThr3-1 on CPU(%d): %d\n", cp, iteration_index);
   }

   ThreadSetPriority(priority_level );
   ThreadSetName("Thread3_P_high");

   for (iteration_index = 0; iteration_index < compute_count; iteration_index++) {
      AsciiPrint ("\t\t\t\t\t\tThr3-2 on CPU (%d): %d\n", cp, iteration_index);
   }

   ThreadSleep (200);
   AsciiPrint ("Exiting Thread3 (t_thread_priority3) with ID: %d on CPU: %d\n", t, cp);
   ThreadExit(0);
   return 0;
}


EFI_STATUS Thread_priority_test(VOID) {
   UINTN cp, previous_cpu, test_cpu;
   Thread *t;
   KStatus thread_status1, thread_status2, thread_status3;
   int thread_rc1, thread_rc2, thread_rc3;
   AsciiPrint("Testcase: Thread Priority\n");

   t = GetCurrentThread();
   cp = ThreadGetCurrCpu(t);
   previous_cpu = cp;

   test_cpu = Get_Next_CPU(previous_cpu);
   AsciiPrint ("Main Thread: Test threads running on CPU(%d)\n", test_cpu);

   /* Setup and generate Thread1 */
   AsciiPrint ("Main Thread: Create Thread1 with priority_level(%d)\n", DEFAULT_PRIORITY);
   mpt1 = ThreadCreate ("thr_P1", &t_thread_priority1, (void*)"thr_P1", DEFAULT_PRIORITY, DEFAULT_STACK_SIZE);
   ThreadSetPinnedCpu (mpt1, test_cpu);

   if (mpt1 == NULL)
   {
      AsciiPrint("Unable to create thread (t_thread_priority1), exiting ...\n");
      return EFI_UNSUPPORTED;
   }

   ThreadResume (mpt1);

   /* Setup and generate Thread1 */
   AsciiPrint ("Main Thread: Create Thread2 with priority_level(%d)\n", DEFAULT_PRIORITY);
   mpt2 = ThreadCreate ("thr_P2", &t_thread_priority2, (void*)"thr_P2", DEFAULT_PRIORITY, DEFAULT_STACK_SIZE);
   ThreadSetPinnedCpu (mpt2, test_cpu);

   if (mpt2 == NULL)
   {
      AsciiPrint("Unable to create thread (t_sem_priority2), exiting ...\n");
      return EFI_UNSUPPORTED;
   }

   ThreadResume (mpt2);

   /* Setup and generate Thread1 */
   AsciiPrint ("Main Thread: Create Thread3 with priority_level(%d)\n", DEFAULT_PRIORITY);
   mpt3 = ThreadCreate ("thr_P3", &t_thread_priority3, (void*)"thr_P3", DEFAULT_PRIORITY, DEFAULT_STACK_SIZE);
   ThreadSetPinnedCpu (mpt3, test_cpu);

   if (mpt3 == NULL)
   {
      AsciiPrint("Unable to create thread (t_sem_priority3), exiting ...\n");
      return EFI_UNSUPPORTED;
   }

   ThreadResume (mpt3);


   AsciiPrint ("\nMain Thread: Wait on threads to join.\n");

   /* threads not expected to join since detached */
   //thread_status1 = ThreadJoin(mpt1, &thread_rc1, 5000); /* napali */
   //thread_status2 = ThreadJoin(mpt2, &thread_rc2, 5000); /* napali */
   //thread_status3 = ThreadJoin(mpt3, &thread_rc3, 5000); /* napali */
   thread_status1 = ThreadJoin(mpt1, &thread_rc1, 250000);  /* hana x50 */
   thread_status2 = ThreadJoin(mpt2, &thread_rc2, 250000);  /* hana x50 */
   thread_status3 = ThreadJoin(mpt3, &thread_rc3, 250000);  /* hana x50 */


   /* check pass/fail status */
   AsciiPrint ("\nMain Thread: Thr1 - thread_status(%d)\tthread_rc(%d)\n", thread_status1, thread_rc1);
   AsciiPrint ("\nMain Thread: Thr2 - thread_status(%d)\tthread_rc(%d)\n", thread_status2, thread_rc2);
   AsciiPrint ("\nMain Thread: Thr3 - thread_status(%d)\tthread_rc(%d)\n", thread_status3, thread_rc3);
   if (  (thread_status1 != 0)  || (thread_rc1 != 0) || 
         (thread_status2 != 0)  || (thread_rc2 != 0) || 
         (thread_status3 != 0)  || (thread_rc3 != 0)) {
      AsciiPrint ("Test FAILED!\n");
      return EFI_D_ERROR;
   }
   else
   {
      AsciiPrint ("Test PASSED!\n");
   }

   ThreadSleep (200);
   return EFI_SUCCESS;
   
}

int t_thread_name( IN VOID  *Arg)
{
   int cp;
   Thread *t;
   UINTN iteration_index;
   BOOLEAN loop_done;

   t = GetCurrentThread();
   cp = ThreadGetCurrCpu(t);

   AsciiPrint ("Executing Thread1 (t_thread_name) with ID: %d on CPU: %d\n", t, cp);

   ThreadSetName("Thread1_test");

   iteration_index  = 0;
   loop_done = FALSE;

   while (loop_done == TRUE) {
      ThreadSleep (500);
      AsciiPrint ("Thr1 on CPU(%d): iteration %d\n", cp, iteration_index);
      iteration_index++;
   }

   ThreadSleep (200);
   AsciiPrint ("Exiting Thread1 (t_thread_name) with ID: %d on CPU: %d\n", t, cp);
   ThreadExit(0);
   return 0;
}


EFI_STATUS Thread_name_test(VOID) {
   UINTN cp,  test_cpu;
   Thread *t;
   AsciiPrint("Testcase: Thread Name\n");

   t = GetCurrentThread();
   cp = ThreadGetCurrCpu(t);

   test_cpu = cp;
   AsciiPrint ("Main Thread: Test threads running on CPU(%d)\n", test_cpu);

   /* Setup and generate Thread1 */
   AsciiPrint ("Main Thread: Create Thread1 to set thread name( Thread1_test)\n");
   mpt1 = ThreadCreate ("Thr1", &t_thread_name, (void*)"thr_P1", DEFAULT_PRIORITY, DEFAULT_STACK_SIZE);
   ThreadSetPinnedCpu (mpt1, test_cpu);

   if (mpt1 == NULL)
   {
      AsciiPrint("Unable to create thread (t_thread_name), exiting ...\n");
      return EFI_UNSUPPORTED;
   }

   //ThreadResume (mpt1);
   ThreadDetachAndResume (mpt1);

   AsciiPrint ("\nMain Thread: Exits.\n");

   ThreadSleep (200);
   return EFI_SUCCESS;
   
}


int t_thread_detach( IN VOID  *Arg)
{
   int cp;
   Thread *t;
   UINTN compute_count;
   UINTN iteration_index;
   UINT32 priority_level;
   priority_level = DEFAULT_PRIORITY - 4;

   t = GetCurrentThread();
   cp = ThreadGetCurrCpu(t);

   AsciiPrint ("Executing Thread (t_thread_detach) with ID: %d on CPU: %d\n", t, cp);

   compute_count = 99;
   for (iteration_index = 0; iteration_index < compute_count; iteration_index++) {
      AsciiPrint ("Thr1-1 on CPU(%d): %d\n", cp, iteration_index);
   }

   ThreadSetName("Thread1_detach");

   for (iteration_index = 0; iteration_index < compute_count; iteration_index++) {
      AsciiPrint ("Thr1-2 on CPU(%d): %d\n", cp, iteration_index);
   }

   ThreadSleep (200);
   AsciiPrint ("Exiting Thread (t_thread_detach) with ID: %d on CPU: %d\n", t, cp);
   ThreadExit(0);
   return 0;
}


EFI_STATUS Thread_detach_test(VOID) {
   UINTN cp, previous_cpu, test_cpu;
   Thread *t;
   KStatus thread_status1;
   int thread_rc1;
   AsciiPrint("Testcase: Thread Detach\n");

   t = GetCurrentThread();
   cp = ThreadGetCurrCpu(t);
   previous_cpu = cp;

   test_cpu = Get_Next_CPU(previous_cpu);
   AsciiPrint ("Main Thread: Test threads running on CPU(%d)\n", test_cpu);

   /* Setup and generate Thread1 */
   AsciiPrint ("Main Thread: Create Thread1 with priority_level(%d)\n", DEFAULT_PRIORITY);
   mpt1 = ThreadCreate ("thr_P1", &t_thread_detach, (void*)"thr_P1", DEFAULT_PRIORITY, DEFAULT_STACK_SIZE);
   ThreadSetPinnedCpu (mpt1, test_cpu);

   if (mpt1 == NULL)
   {
      AsciiPrint("Unable to create thread (t_thread_detach), exiting ...\n");
      return EFI_UNSUPPORTED;
   }

   ThreadDetachAndResume (mpt1);



   //ThreadSleep (5000); /* napali */
   ThreadSleep (50000); /* hana x10 */

   AsciiPrint ("\nMain Thread: Wait on threads to join.\n");

   /* threads not expected to join since detached */
   thread_status1 = ThreadJoin(mpt1, &thread_rc1, 50000); 

   /* check pass/fail status */
   AsciiPrint ("\nMain Thread: Thr1 - thread_status(%d)\tthread_rc(%d)\n", thread_status1, thread_rc1);
   if (  (thread_status1 != 0)  || (thread_rc1 != 0) ) {
      AsciiPrint ("Test FAILED!\n");
      return EFI_D_ERROR;
   }
   else
   {
      AsciiPrint ("Test PASSED!\n");
   }

   ThreadSleep (200);
   return EFI_SUCCESS;
   
}


int t_thread_yield1( IN VOID  *Arg)
{
   int cp;
   Thread *t;
   //UINTN compute_count;
   //UINTN iteration_index;
   UINT32 priority_level;
   //priority_level = DEFAULT_PRIORITY - 4;
   //priority_level = DEFAULT_PRIORITY - 2;
   priority_level = DEFAULT_PRIORITY - 1;
   //UINT32 tmp_counter = 0;

   t = GetCurrentThread();
   cp = ThreadGetCurrCpu(t);

   AsciiPrint ("Executing Thread1 (t_thread_yield1)  on CPU: %d\n", cp);

   ThreadSetPriority(priority_level);
   ThreadSetName("Thread1_P_low");

   ThreadSleep (100);

   /* check thread2 has not started yet */
   if (shared_handshake_stage_g != 0) {
      AsciiPrint ("\n\nThread1: Thread2 started before thread1.\n");
      ThreadExit(EFI_D_ERROR);
      return EFI_D_ERROR; 
   }

   /* thread1 starts computing */
   shared_handshake_stage_g = 1;
   AsciiPrint ("Thread1: shared_handshake_stage_g(%d) - Thread1 started computing.\n\n", shared_handshake_stage_g);

 
   /* only continue processing if Thread2 did yield CPU to increment counter */
   while (shared_handshake_stage_g == 1) { }

   /* Thread2 yield CPU to increment counter */
   while (shared_handshake_stage_g == 2) {
         shared_counter_g++; 
   }


   /* check thread1 did not finish before thread2 */
   AsciiPrint ("\n\nThread1: shared_counter_g: %d.\n", shared_counter_g);
   if (shared_counter_g == 0) {
      AsciiPrint ("\n\nThread1: Thread2 DID NOT yield in middle of computation.\n");
      ThreadExit(EFI_D_ERROR);
      return EFI_D_ERROR; 
   }
   else {
      AsciiPrint ("\n\nThread1: Thread2 yield in middle of computation.\n");
   }

   ThreadSleep (200);
   AsciiPrint ("Exiting Thread1 (t_thread_yield1) on CPU: %d\n", cp);
   ThreadExit(0);
   return 0;
}

int t_thread_yield2( IN VOID  *Arg)
{
   int cp;
   Thread *t;
   UINTN iteration_index, iteration_limit, yield_start, yield_end;
   UINT32 priority_level;
   priority_level = DEFAULT_PRIORITY - 1;

   t = GetCurrentThread();
   cp = ThreadGetCurrCpu(t);

   AsciiPrint ("Executing Thread2 (t_thread_yield2) on CPU: %d\n", cp);


   ThreadSetPriority(priority_level);
   ThreadSetName("Thread2_P_high");

   ThreadSleep (200);

   /* check thread1 did not finish before thread2 */
   if (shared_handshake_stage_g < 1) {
      AsciiPrint ("\n\nThread2: Thread1 DID NOT start computing yet.\n");
      ThreadExit(EFI_D_ERROR);
      return EFI_D_ERROR; 
   }
   else if (shared_handshake_stage_g > 1) {
      AsciiPrint ("\n\nThread2: Thread1 finished computing before thread2 computation started.\n");
      ThreadExit(EFI_D_ERROR);
      return EFI_D_ERROR; 
   }
   else {
      AsciiPrint ("\n\nThread2: Thread1 started computing.\n");
   }

   yield_count = 0;
   yield_start = 100;
   yield_end = 9990; 
   iteration_index = 0;
   iteration_limit = 10000; 
   AsciiPrint ("\n\nThread2: Computation starting at iteration: 0.\n");
   //while ((iteration_index < iteration_limit) && (shared_handshake_stage_g != 3)) {
   while  (shared_handshake_stage_g != 3) {

      //if  ( (iteration_index >= yield_start) && (iteration_index <= yield_end)) {
      if   (iteration_index >= yield_start) {

            /* wait for thread1 to get ready to iterate and increment counter */
            if (iteration_index == yield_start) {
               shared_handshake_stage_g = 2;
               //AsciiPrint ("Thread2: shared_handshake_stage_g(%d) - Thread2 about to yield in middle of computation at iteration (%d).\n\n",
               //               shared_handshake_stage_g, iteration_index);
            }

            if (shared_counter_g == 0) { 
               /* thread2 about to yield in middle of computation */
               /* ThreadYield API is not supported as it is taken care by the scheduler */
               //ThreadYield();
               yield_count++;
            }

            if ((shared_counter_g > 0) && (shared_handshake_stage_g == 2)) { 
               shared_handshake_stage_g = 3;
            }
      }

      iteration_index++;

   } // while loop 


   if (shared_handshake_stage_g == 3) { 
      AsciiPrint ("Thread2: shared_handshake_stage_g(%d) - Thread2 yield done at iteration: %d.\n\n", 
                     shared_handshake_stage_g, iteration_index);
   }
   

   /* end of thread2 */
   shared_handshake_stage_g = 4;
   AsciiPrint ("\n\nThread2: shared_counter_g(%d)\tshared_handshake_stage_g(%d).\n\n", 
                     shared_counter_g, shared_handshake_stage_g);

   /* before end of thread2 compute, thread1 compute must be done. */
   if (shared_counter_g == 0) {
      AsciiPrint ("\n\nThread2: Thread1 DID NOT execute during thread2 yield period.\n");
      ThreadExit(EFI_D_ERROR);
      return EFI_D_ERROR; 
   }
   else  {
      AsciiPrint ("\n\nThread2: Thread1 executed during thread2 yield period.\n");
   }

   ThreadSleep (200);
   AsciiPrint ("Exiting Thread2 (t_thread_priority2) on CPU: %d\n", cp);
   ThreadExit(0);
   return 0;
}


/* THIS TEST IS NO LONGER SUPPORTED: ThreadYield API is removed as it is taken care by the scheduler */
EFI_STATUS Thread_yield_test(VOID) {
   UINTN cp, previous_cpu, test_cpu;
   Thread *t;
   KStatus thread_status1, thread_status2;
   int thread_rc1, thread_rc2;
   AsciiPrint("Testcase: Thread Yield\n");

   t = GetCurrentThread();
   cp = ThreadGetCurrCpu(t);
   previous_cpu = cp;

   shared_handshake_stage_g = 0;
   AsciiPrint("Initialize shared_handshake_stage_g to %d\n", shared_handshake_stage_g);

   /* initialize thread1 execution count */  
   shared_counter_g = 0;

   test_cpu = Get_Next_CPU(previous_cpu);
   AsciiPrint ("Main Thread: Test threads running on CPU(%d)\n", test_cpu);

   /* Setup and generate Thread1 */
   AsciiPrint ("Main Thread: Create Thread1 with priority_level(%d)\n", DEFAULT_PRIORITY);
   mpt1 = ThreadCreate ("thr_P1", &t_thread_yield1, (void*)"thr_P1", DEFAULT_PRIORITY, DEFAULT_STACK_SIZE);
   ThreadSetPinnedCpu (mpt1, test_cpu);

   if (mpt1 == NULL)
   {
      AsciiPrint("Unable to create thread (t_thread_yield1), exiting ...\n");
      return EFI_UNSUPPORTED;
   }

   ThreadResume (mpt1);

   /* Setup and generate Thread1 */
   AsciiPrint ("Main Thread: Create Thread2 with priority_level(%d)\n", DEFAULT_PRIORITY);
   mpt2 = ThreadCreate ("thr_P2", &t_thread_yield2, (void*)"thr_P2", DEFAULT_PRIORITY, DEFAULT_STACK_SIZE);
   ThreadSetPinnedCpu (mpt2, test_cpu);

   if (mpt2 == NULL)
   {
      AsciiPrint("Unable to create thread (t_thread_yield2), exiting ...\n");
      return EFI_UNSUPPORTED;
   }

   ThreadResume (mpt2);

   AsciiPrint ("\nMain Thread: Wait on threads to join.\n");

   /* threads not expected to join since detached */
   thread_status1 = ThreadJoin(mpt1, &thread_rc1, 90000);
   thread_status2 = ThreadJoin(mpt2, &thread_rc2, 90000);
   
  
   /* Thread1 execution count */
   AsciiPrint ("\nMain Thread: Thr1 - execution count (%d)\n", shared_counter_g);
   AsciiPrint ("\nMain Thread: Thr2 - yield count (%d)\n", yield_count);


   /* check pass/fail status */
   AsciiPrint ("\nMain Thread: Thr1 - thread_status(%d)\tthread_rc(%d)\n", thread_status1, thread_rc1);
   AsciiPrint ("\nMain Thread: Thr2 - thread_status(%d)\tthread_rc(%d)\n", thread_status2, thread_rc2);
   if (  (thread_status1 != 0)  || (thread_rc1 != 0) || 
         (thread_status2 != 0)  || (thread_rc2 != 0) ) {
      AsciiPrint ("Test FAILED!\n");
      return EFI_D_ERROR;
   }
   else
   {
      AsciiPrint ("Test PASSED!\n");
   }

   ThreadSleep (200);
   return EFI_SUCCESS;
   
}




int t_thread_preempt1( IN VOID  *Arg)
{
   int cp;
   Thread *t;
   UINT32 priority_level;
   //priority_level = DEFAULT_PRIORITY - 4;
   //priority_level = DEFAULT_PRIORITY - 2;
   priority_level = DEFAULT_PRIORITY - 1;

   t = GetCurrentThread();
   cp = ThreadGetCurrCpu(t);

   AsciiPrint ("Executing Thread1 (t_thread_preempt1) on CPU: %d\n",  cp);

   ThreadSetPriority(priority_level);
   ThreadSetName("Thread1_P_low");

   ThreadSleep (100);

   /* check thread2 has not started yet */
   if (shared_handshake_stage_g != 0) {
      AsciiPrint ("\n\nThread1: Thread2 started before thread1.\n");
      ThreadExit(EFI_D_ERROR);
      return EFI_D_ERROR; 
   }

   /* thread1 starts computing */
   shared_handshake_stage_g = 1;
   AsciiPrint ("Thread1: shared_handshake_stage_g(%d) - Thread1 started computing.\n\n", shared_handshake_stage_g);

 
   /* only continue processing if Thread2 did yield CPU to increment counter */
   while (shared_handshake_stage_g == 1) { }

   /* Thread2 yield CPU to increment counter */
   while (shared_handshake_stage_g == 2) {
         shared_counter_g++; 
   }


   /* check thread1 did not finish before thread2 */
   AsciiPrint ("\n\nThread1: shared_counter_g: %d.\n", shared_counter_g);
   if (shared_counter_g == 0) {
      AsciiPrint ("\n\nThread1: Thread2 DID NOT yield in middle of computation.\n");
      ThreadExit(EFI_D_ERROR);
      return EFI_D_ERROR; 
   }
   else {
      AsciiPrint ("\n\nThread1: Thread2 yield in middle of computation.\n");
   }

   ThreadSleep (200);
   AsciiPrint ("Exiting Thread1 (t_thread_preempt1) with ID: %d on CPU: %d\n", t, cp);
   ThreadExit(0);
   return 0;
}

int t_thread_preempt2( IN VOID  *Arg)
{
   int cp;
   Thread *t;
   UINTN iteration_index, iteration_limit, yield_start, yield_end;
   UINT32 priority_level;
   priority_level = DEFAULT_PRIORITY - 1;

   t = GetCurrentThread();
   cp = ThreadGetCurrCpu(t);

   AsciiPrint ("Executing Thread2 (t_thread_preempt2) on CPU: %d\n", cp);


   ThreadSetPriority(priority_level);
   ThreadSetName("Thread2_P_high");

   ThreadSleep (200);

   /* check thread1 did not finish before thread2 */
   if (shared_handshake_stage_g < 1) {
      AsciiPrint ("\n\nThread2: Thread1 DID NOT start computing yet.\n");
      ThreadExit(EFI_D_ERROR);
      return EFI_D_ERROR; 
   }
   else if (shared_handshake_stage_g > 1) {
      AsciiPrint ("\n\nThread2: Thread1 finished computing before thread2 computation started.\n");
      ThreadExit(EFI_D_ERROR);
      return EFI_D_ERROR; 
   }
   else {
      AsciiPrint ("\n\nThread2: Thread1 started computing.\n");
   }

   preempt_count = 0;
   yield_start = 100;
   yield_end = 9990; 
   iteration_index = 0;
   iteration_limit = 10000; 
   AsciiPrint ("\n\nThread2: Computation starting at iteration: 0.\n");
   //while (iteration_index < iteration_limit) {
   while  (shared_handshake_stage_g != 3) {

      //if  ( (iteration_index >= yield_start) && (iteration_index <= yield_end)) {
      if   (iteration_index >= yield_start) {

            /* wait for thread1 to get ready to iterate and increment counter */
            if (iteration_index == yield_start) {
               shared_handshake_stage_g = 2;
               //AsciiPrint ("Thread2: shared_handshake_stage_g(%d) - Thread2 about to yield in middle of computation at iteration (%d).\n\n",
               //               shared_handshake_stage_g, iteration_index);
            }

            if (shared_counter_g == 0) { 
               /* thread2 about to yield in middle of computation */
               /* ThreadPreempt API is not supported as it is taken care by the scheduler */
               //ThreadPreempt();
               preempt_count++;
            }

            if ((shared_counter_g > 0) && (shared_handshake_stage_g == 2)) { 
               shared_handshake_stage_g = 3;
            }
      }

      iteration_index++;

   } // while loop 

   if (shared_handshake_stage_g == 3) { 
      AsciiPrint ("Thread2: shared_handshake_stage_g(%d) - Thread2 yield done at iteration: %d.\n\n", 
                     shared_handshake_stage_g, iteration_index);
   }
   

   /* end of thread2 */
   shared_handshake_stage_g = 4;
   AsciiPrint ("\n\nThread2: shared_counter_g(%d)\tshared_handshake_stage_g(%d).\n\n", 
                     shared_counter_g, shared_handshake_stage_g);

   /* before end of thread2 compute, thread1 compute must be done. */
   if (shared_counter_g == 0) {
      AsciiPrint ("\n\nThread2: Thread1 DID NOT execute during thread2 yield period.\n");
      ThreadExit(EFI_D_ERROR);
      return EFI_D_ERROR; 
   }
   else  {
      AsciiPrint ("\n\nThread2: Thread1 executed during thread2 yield period.\n");
   }

   ThreadSleep (200);
   AsciiPrint ("Exiting Thread2 (t_thread_priority2) with ID: %d on CPU: %d\n", t, cp);
   ThreadExit(0);
   return 0;
}


/* THIS TEST IS NO LONGER SUPPORTED: ThreadPreempt API is removed as it is taken care by the scheduler */
EFI_STATUS Thread_preempt_test(VOID) {
   UINTN cp, previous_cpu, test_cpu;
   Thread *t;
   KStatus thread_status1, thread_status2;
   int thread_rc1, thread_rc2;
   AsciiPrint("Testcase: Thread Preempt\n");

   t = GetCurrentThread();
   cp = ThreadGetCurrCpu(t);
   previous_cpu = cp;

   shared_handshake_stage_g = 0;
   AsciiPrint("Initialize shared_handshake_stage_g to %d\n", shared_handshake_stage_g);

   test_cpu = Get_Next_CPU(previous_cpu);
   AsciiPrint ("Main Thread: Test threads running on CPU(%d)\n", test_cpu);

   /* Setup and generate Thread1 */
   AsciiPrint ("Main Thread: Create Thread1 with priority_level(%d)\n", DEFAULT_PRIORITY);
   mpt1 = ThreadCreate ("thr_P1", &t_thread_preempt1, (void*)"thr_P1", DEFAULT_PRIORITY, DEFAULT_STACK_SIZE);
   ThreadSetPinnedCpu (mpt1, test_cpu);

   if (mpt1 == NULL)
   {
      AsciiPrint("Unable to create thread (t_thread_preempt1), exiting ...\n");
      return EFI_UNSUPPORTED;
   }

   ThreadResume (mpt1);

   /* Setup and generate Thread1 */
   AsciiPrint ("Main Thread: Create Thread2 with priority_level(%d)\n", DEFAULT_PRIORITY);
   mpt2 = ThreadCreate ("thr_P2", &t_thread_preempt2, (void*)"thr_P2", DEFAULT_PRIORITY, DEFAULT_STACK_SIZE);
   ThreadSetPinnedCpu (mpt2, test_cpu);

   if (mpt2 == NULL)
   {
      AsciiPrint("Unable to create thread (t_thread_preempt2), exiting ...\n");
      return EFI_UNSUPPORTED;
   }

   ThreadResume (mpt2);

   AsciiPrint ("\nMain Thread: Wait on threads to join.\n");

   /* threads not expected to join since detached */
   thread_status1 = ThreadJoin(mpt1, &thread_rc1, 90000);
   thread_status2 = ThreadJoin(mpt2, &thread_rc2, 90000);

   /* Thread1 execution count */
   AsciiPrint ("\nMain Thread: Thr1 - execution count (%d)\n", shared_counter_g);
   AsciiPrint ("\nMain Thread: Thr2 - preempt count (%d)\n", preempt_count);

   /* check pass/fail status */
   AsciiPrint ("\nMain Thread: Thr1 - thread_status(%d)\tthread_rc(%d)\n", thread_status1, thread_rc1);
   AsciiPrint ("\nMain Thread: Thr2 - thread_status(%d)\tthread_rc(%d)\n", thread_status2, thread_rc2);
   if (  (thread_status1 != 0)  || (thread_rc1 != 0) || 
         (thread_status2 != 0)  || (thread_rc2 != 0) ) {
      AsciiPrint ("Test FAILED!\n");
      return EFI_D_ERROR;
   }
   else
   {
      AsciiPrint ("Test PASSED!\n");
   }

   ThreadSleep (200);
   return EFI_SUCCESS;
   
}

int t_thread_returnCode1( IN VOID  *Arg)
{
   int cp;
   Thread *t;
   UINTN returnCode;

   t = GetCurrentThread();
   cp = ThreadGetCurrCpu(t);

   AsciiPrint ("Executing Thread1 (t_thread_returnCode1) with ID: %d on CPU: %d\n", t, cp);

   /* sleep for while before exiting thread */
   ThreadSleep (200);

   returnCode = 0xE;
   AsciiPrint ("Exiting Thread1 (t_thread_returnCode1 on CPU %d) with returnCode: %d\n", cp, returnCode);
   ThreadExit(returnCode);
   return returnCode;
}

int t_thread_returnCode2( IN VOID  *Arg)
{
   int cp;
   Thread *t;
   UINTN returnCode;

   t = GetCurrentThread();
   cp = ThreadGetCurrCpu(t);

   AsciiPrint ("Executing Thread2 (t_thread_returnCode2) with ID: %d on CPU: %d\n", t, cp);

   /* sleep for while before exiting thread */
   ThreadSleep (200);

   returnCode = 0xF;
   AsciiPrint ("Exiting Thread2 (t_thread_returnCode2 on CPU %d) with returnCode: %d\n", cp, returnCode);
   ThreadExit(returnCode);
   return returnCode;
}


EFI_STATUS Thread_returnCode_test(VOID) {
   UINTN cp, previous_cpu, next_cpu;
   Thread *t;
   KStatus thread_status1, thread_status2;
   int thread_rc1, thread_rc2;
   AsciiPrint("Testcase: Thread Return Code\n");

   t = GetCurrentThread();
   cp = ThreadGetCurrCpu(t);
   previous_cpu = cp;

   /* Setup and generate Thread1 */
   AsciiPrint ("Main Thread: Create Thread1 with priority_level(%d)\n", DEFAULT_PRIORITY);
   mpt1 = ThreadCreate ("thr_R1", &t_thread_returnCode1, (void*)"thr_R1", DEFAULT_PRIORITY, DEFAULT_STACK_SIZE);

   /* set current thread at index to next cpu */
   next_cpu = Get_Next_CPU(previous_cpu);
   previous_cpu = next_cpu;
   ThreadSetPinnedCpu (mpt1, next_cpu);

   if (mpt1 == NULL)
   {
      AsciiPrint("Unable to create thread (t_thread_returnCode1), exiting ...\n");
      return EFI_UNSUPPORTED;
   }

   ThreadResume (mpt1);

   /* Setup and generate Thread1 */
   AsciiPrint ("Main Thread: Create Thread2 with priority_level(%d)\n", DEFAULT_PRIORITY);
   mpt2 = ThreadCreate ("thr_R2", &t_thread_returnCode2, (void*)"thr_R2", DEFAULT_PRIORITY, DEFAULT_STACK_SIZE);

   /* set current thread at index to next cpu */
   next_cpu = Get_Next_CPU(previous_cpu);
   previous_cpu = next_cpu;
   ThreadSetPinnedCpu (mpt2, next_cpu);

   if (mpt2 == NULL)
   {
      AsciiPrint("Unable to create thread (t_thread_returnCode2), exiting ...\n");
      return EFI_UNSUPPORTED;
   }

   ThreadResume (mpt2);

   AsciiPrint ("\nMain Thread: Wait on threads to join.\n");

   /* threads not expected to join since detached */
   thread_status1 = ThreadJoin(mpt1, &thread_rc1, 5000);
   thread_status2 = ThreadJoin(mpt2, &thread_rc2, 5000);

   /* check pass/fail status */
   AsciiPrint ("\nMain Thread: Thr1 - thread_status(%d)\tthread_rc(%d)\n", thread_status1, thread_rc1);
   AsciiPrint ("\nMain Thread: Thr2 - thread_status(%d)\tthread_rc(%d)\n", thread_status2, thread_rc2);
   if (  (thread_status1 != 0)  || (thread_rc1 != 0xE) || 
         (thread_status2 != 0)  || (thread_rc2 != 0xF) ) {
      AsciiPrint ("Test FAILED!\n");
      return EFI_D_ERROR;
   }
   else
   {
      AsciiPrint ("Test PASSED!\n");
   }

   ThreadSleep (200);
   return EFI_SUCCESS;
   
}



int t_thread_max_x( IN VOID  *Arg) {
   int cp;
   Thread *t;
   UINTN time_in_ms;
   thread_config* thr_cfg = (thread_config*)Arg;
   UINTN thread_index = (thr_cfg->thread_index);
   UINTN thread_initial_wait = (thr_cfg->initial_wait_ms);
   KStatus event_status;

   t = GetCurrentThread();
   cp = ThreadGetCurrCpu(t);
      

   AsciiPrint ("Executing t_thread_max_x[%d] with ID: %d on CPU: %d\n", thread_index, t, cp);

   if (cp < 0) {
      AsciiPrint ("ERROR: ThreadGetCurrCpu() returned (%d)\n", cp);
      return 1;
   }

   time_in_ms = GetTimerCountms();
   AsciiPrint ("Thread[%d] at %d sleeps for %d ms.\n", 
                  thread_index, time_in_ms, thread_initial_wait*100);

   /* sleep before waiting on all threads to be created */
   ThreadSleep (100*thread_initial_wait);

   Lock (lock_single1);
   time_in_ms = GetTimerCountms ();
   AsciiPrint ("Thread_x[%d] - CPU (%d): got lock at time: %d ms\n", thread_index, cp, time_in_ms);

   /* increment total thread count */
   shared_counter_g++;

   /* increment thread count on current cpu */
   shared_counter_group_g[cp]++;
   AsciiPrint ("Thread_x[%d]: shared_counter_group_g[%d] (thread on cpu) = %d \ttotal threads = %d.\n", 
                  thread_index, cp, shared_counter_group_g[cp], shared_counter_g);

   UnLock (lock_single1);
   time_in_ms = GetTimerCountms ();
   AsciiPrint ("Thread_x[%d] - CPU (%d): unlock at time: %d ms\n", thread_index, cp, time_in_ms);

   /* wait on event */
   AsciiPrint ("Thread_x[%d] - CPU (%d): wait on event1.\n", thread_index, cp);
   event_status = EventWait(event1);

   time_in_ms = GetTimerCountms ();
   if (event_status == 0) {
      AsciiPrint ("Thread_x[%d] - CPU (%d): did get event1 to exit thread at time: %d ms\n", 
                     thread_index, cp, time_in_ms);
   }
   else {
      AsciiPrint ("Thread_x[%d] - CPU (%d): DID NOT get event1 to exit thread at time: %d ms\n", 
                     thread_index, cp, time_in_ms);
      AsciiPrint ("Test FAILED!\n");
      ThreadExit(EFI_D_ERROR);
      return EFI_D_ERROR; 
   }

   AsciiPrint ("Exiting Thread t_thead_max_x[%d]) on CPU: %d\n", thread_index, cp);
   ThreadExit(0);
   return 0;
}

EFI_STATUS Thread_max_test(VOID) {
   UINTN cp, previous_cpu, next_cpu;
   UINTN thr_index;
   UINTN num_of_child_threads = TEST_MAX_NUM_OF_THREADS; /*  */  
   //UINTN num_of_child_threads = 200; /* ok */  
   //UINTN num_of_child_threads = 71; (passed in v40)  
   //UINTN num_of_child_threads = 70;   
               // 72: failed (exe of 69 hangs)
               // 71: failed (exe of 69 hangs)
               // 70: no hang 
               //
               // 69: thr31 did not get lock  (issue always repeated at 31)
               // 60: thr31 did not get lock  (issue always repeated at 31)
               // 35: thr31 did not get lock  (issue always repeated at 31)
               
   Thread *t;
   UINTN thr_create_time;
   KStatus thread_status;
   INT32 thread_rc;
   INT32 i;
   thread_config thr_config_max[num_of_child_threads];
   EFI_STATUS test_status = EFI_SUCCESS;

   AsciiPrint("Testcase: Thread Max\n");

   AsciiPrint("Main Thread: Max number of threads supported by test is %d\n", TEST_MAX_NUM_OF_THREADS);

   t = GetCurrentThread();
   cp = ThreadGetCurrCpu(t);
   previous_cpu = cp;

   /* Initialize private lock */
   AsciiPrint("Initialize single lock.\n");
   lock_single1 = InitLock("LOCK_SINGLE");

   /*  initialize event with auto_unsignal disabled (0) so many can enter */
   AsciiPrint ("Main Thread: Initialize event (with auto unsignal disabled (0) so many can enter.\n");
   event1 = EventInit(0, 0, 0);

   /* initialize total thread count */
   shared_counter_g = 0;

   /* threads to wait on signal */
   for (thr_index=0; thr_index < num_of_child_threads; thr_index++) {

      thr_config_max[thr_index].thread_index = thr_index;

      for (i=0;i<TEST_CONFIG_STR_LEN;i++) {
         thr_config_max[thr_index].thread_name[i] = '\0';
      }
      *((CHAR8 *)&(thr_config_max[thr_index].thread_name)) = 'T';
      *((CHAR8 *)&(thr_config_max[thr_index].thread_name)+1) = 'H';
      *((CHAR8 *)&(thr_config_max[thr_index].thread_name)+2) = 'R';
      *((CHAR8 *)&(thr_config_max[thr_index].thread_name)+3) = '\0';
      uintToString(thr_index, &(thr_config_max[thr_index].thread_name[3]));
      AsciiPrint ("Thread index[%d]\tname: %a\n", thr_index, (CHAR8 *)thr_config_max[thr_index].thread_name);

      /* time to wait before the create thread to execute */
      thr_config_max[thr_index].initial_wait_ms = (UINTN)(1);
      
      /* clear counter of thread to be created */
      shared_counter_group_g[thr_index] = 0;
   
      /* core0 sleeps before creating thread */
      //ThreadSleep (1000);

      thr_create_time = GetTimerCountms ();
      AsciiPrint("Creating thread[%d] (name: %a) at %d ms\n", 
                     thr_index, (CHAR8 *)thr_config_max[thr_index].thread_name, thr_create_time);

      /* Generate and setup Thread1 */
      /* lowe priority threads than main thread */
      mpt_group[thr_index] = ThreadCreate ( (CHAR8 *)(thr_config_max[thr_index].thread_name), &t_thread_max_x, 
                                            (void*)&(thr_config_max[thr_index]), (DEFAULT_PRIORITY-4), DEFAULT_STACK_SIZE);

      if (mpt_group[thr_index] == NULL) {
         AsciiPrint("Unable to create thread mtp_group[%d], exiting ...\n", thr_index);
         return EFI_UNSUPPORTED;
      }

      next_cpu = Get_Next_CPU(previous_cpu);
      AsciiPrint ("Previous CPU: %d Next CPU: %d\n", previous_cpu, next_cpu);
      previous_cpu = next_cpu;
      ThreadSetPinnedCpu (mpt_group[thr_index], next_cpu);

      ThreadResume (mpt_group[thr_index]);
   }

   ThreadSleep (2500);

   /* send event that can be rescheduled */
   AsciiPrint ("Main Thread: Send event for all threads to exit.\n");
   EventSignal(event1, 1);

   for (thr_index=0; thr_index < num_of_child_threads; thr_index++) {
      /* wait for thread to join or timeout in 10000 */
      thread_status = ThreadJoin(mpt_group[thr_index], &thread_rc, 50000);
      AsciiPrint ("Main thread: t_thread_max_x[%d] thread_status(%d)\tthread_rc(%d)\n", 
                           thr_index, thread_status , thread_rc);
      if ((thread_status != 0)|| (thread_rc != 0)) {
         AsciiPrint ("Main Thread: t_thread_max_x[%d] did not complete successfully!\n", thr_index);
         if (test_status ==  EFI_SUCCESS) {
            test_status = EFI_D_ERROR;  
         }
      }
   } // for
   
   ThreadSleep (200);

   AsciiPrint ("\nMain Thread: Number of threads on CPUs:\n");
   for (i=0; i < TOTAL_CPU; i++) {
      AsciiPrint ("\tCPU[%d]: %d\n", i, shared_counter_group_g[i]);
   }
   AsciiPrint ("\n");

   /* Pass criteria check:
    * each counter set by thread to 1
    * */ 
   if (shared_counter_g != num_of_child_threads) {
      AsciiPrint ("Main Thread: counter for total number of thread is not correct.\n");
      AsciiPrint ("Main Thread: Actual counter value = %d \t(num_of_child_threads=%d).\n", 
                     shared_counter_g, num_of_child_threads);
      if (test_status ==  EFI_SUCCESS) {
         test_status = EFI_D_ERROR;  
      }

   }
   else {
      AsciiPrint ("Main Thread: Thread counter value = %d \t(num_of_child_threads=%d).\n", 
                     shared_counter_g, num_of_child_threads);
   }

   if (test_status !=  EFI_SUCCESS) {
      AsciiPrint ("\nTest FAILED with test_status: %d!\n", test_status);
      return test_status;
   }
   else {
      AsciiPrint ("\nTest PASSED!\n");
      return EFI_SUCCESS;
   }
}



int t_thread_mpcore_x( IN VOID  *Arg) {
   int cp;
   Thread *t;
   UINTN time_in_ms;
   thread_config* thr_cfg = (thread_config*)Arg;
   UINTN thread_index = (thr_cfg->thread_index);
   UINTN thread_initial_wait = (thr_cfg->initial_wait_ms);
   //UINTN thread_compute_ms = (thr_cfg->compute_ms);
   KStatus event_status;

   t = GetCurrentThread();
   cp = ThreadGetCurrCpu(t);

   AsciiPrint ("Executing t_thread_mpcore_x[%d] on CPU: %d\n", thread_index, cp);

   if (cp < 0) {
      AsciiPrint ("ERROR: ThreadGetCurrCpu() returned (%d)\n", cp);
      return 1;
   }

   time_in_ms = GetTimerCountms();
   AsciiPrint ("Thread[%d] at %d sleeps for %d ms.\n", 
                  thread_index, time_in_ms, thread_initial_wait*100);

   /* sleep before waiting on all threads to be created */
   ThreadSleep (thread_initial_wait);

   /* wait here before start computing */
   while (shared_handshake_stage_g == 0) {
   }

   time_in_ms = GetTimerCountms ();
   AsciiPrint ("Thread_x[%d] - CPU (%d): Start computation at time: %d ms\n", thread_index, cp, time_in_ms);

   /* compute */
   while (shared_handshake_stage_g != 2) {
      shared_counter_group_g[cp]++;
   }

   time_in_ms = GetTimerCountms ();
   AsciiPrint ("Thread_x[%d] - CPU (%d): Computation done with shared_counter_group_g[%d] of %d at time: %d ms\n", thread_index, cp,  cp, shared_counter_group_g[cp], time_in_ms);

   ThreadSleep (100);

   /* increment total thread count */
   shared_counter_g++;

   AsciiPrint ("Thread_x[%d]: shared_counter_group_g[%d] = %d \ttotal threads = %d.\n", 
                  thread_index, cp, shared_counter_group_g[cp], shared_counter_g);

   /* wait on event */
   AsciiPrint ("Thread_x[%d] - CPU (%d): wait on event1.\n", thread_index, cp);
   event_status = EventWait(event1);

   time_in_ms = GetTimerCountms ();
   if (event_status == 0) {
      AsciiPrint ("Thread_x[%d] - CPU (%d): did get event1 to exit thread at time: %d ms\n", 
                     thread_index, cp, time_in_ms);
   }
   else {
      AsciiPrint ("Thread_x[%d] - CPU (%d): DID NOT get event1 to exit thread at time: %d ms\n", 
                     thread_index, cp, time_in_ms);
      AsciiPrint ("Test FAILED!\n");
      ThreadExit(EFI_D_ERROR);
      return EFI_D_ERROR; 
   }

   AsciiPrint ("Exiting Thread t_thead_mpcore_x[%d]) on CPU: %d\n", thread_index, cp);
   ThreadExit(0);
   return 0;
}

UINT32 get_cpu_mask (UINTN cpu) {

   UINT32 cpu_mask;

   switch (cpu)
   {
      case 0:  cpu_mask = 1;   break;
      case 1:  cpu_mask = 2;   break;
      case 2:  cpu_mask = 4;   break;
      case 3:  cpu_mask = 8;   break;
      case 4:  cpu_mask = 16;  break;
      case 5:  cpu_mask = 32;  break;
      case 6:  cpu_mask = 64;  break;
      case 7:  cpu_mask = 128; break;
      default:  cpu_mask = 0;  break;
   }  // switch cpu_mask

   return cpu_mask;
}

EFI_STATUS Thread_mpcore_en_all_test(VOID) {
   UINTN cp, current_cpu, next_cpu;
   UINTN thr_index;
   UINTN num_of_child_threads = 8;   
   Thread *t;
   UINTN thr_create_time;
   KStatus thread_status;
   INT32 thread_rc;
   INT32 i;
   thread_config thr_config_max[num_of_child_threads];
   EFI_STATUS test_status = EFI_SUCCESS;
   UINT32 cpu_max;
   UINT32 cpu_avail;
   UINT32 cpu_deferred;
   UINT32 cpu_activeStatus;
   UINT32 cpu_activeMask;
   //UINT32 cpu_onlineMask;
   UINTN cpu_index;
   UINT32 cpu_init_result;

   AsciiPrint("Testcase: Thread mpcore enable all CPUs\n");

   t = GetCurrentThread();
   cp = ThreadGetCurrCpu(t);
   current_cpu = cp;
   AsciiPrint("Main: CPU(%d)\n", cp);

   /* get maximum CPU count */
   cpu_max = MpcoreGetMaxCpuCount();
   AsciiPrint("Main: Max CPU count: %d\n", cpu_max);

   /* available cpu (taken out of reset) */
   cpu_avail = MpcoreGetAvailCpuCount();
   AsciiPrint("Main: Available CPU count: %d\n", cpu_avail);

   /* calculate CPU deferred */
   cpu_deferred = cpu_max - cpu_avail;
   AsciiPrint("Main: Deferred CPU count: %d\n", cpu_deferred);

   AsciiPrint ("\n\nMain Thread: CPU Active Status BEFORE Deferred CPU out of reset: \n");
   cpu_activeMask = MpcoreGetActiveMask();
   AsciiPrint ("Main Thread: CPU Active Mask: %d\n", cpu_activeMask);
   //cpu_onlineMask = MpcoreGetOnlineMask();
   //AsciiPrint ("Main Thread: CPU Online Mask: %d\n", cpu_onlineMask);
   for (thr_index=0; thr_index < num_of_child_threads; thr_index++) {
      cpu_index = thr_index;
      cpu_activeStatus = MpcoreIsCpuActive(cpu_index);
      AsciiPrint ("\t\tThr(%d) - cpu(%d) - active(%d)\n", thr_index, cpu_index, cpu_activeStatus);
   }


   /* if any cpu, not taken out of reset, then enable them */
   if (cpu_deferred) {
      AsciiPrint("Main: Take all deferred CPU out of reset.");
      cpu_init_result = MpcoreInitDeferredCores (0);
      AsciiPrint ("Main Thread: Initialized CPU count: %d\n", cpu_init_result);
   }

   AsciiPrint ("\n\nMain Thread: CPU Active Status AFTER Deferred CPU out of reset: \n");
   cpu_activeMask = MpcoreGetActiveMask();
   AsciiPrint ("Main Thread: CPU Active Mask: %d\n", cpu_activeMask);
   //cpu_onlineMask = MpcoreGetOnlineMask();
   //AsciiPrint ("Main Thread: CPU Online Mask: %d\n", cpu_onlineMask);
   for (thr_index=0; thr_index < num_of_child_threads; thr_index++) {
      cpu_index = thr_index;
      cpu_activeStatus = MpcoreIsCpuActive(cpu_index);
      AsciiPrint ("\t\tThr(%d) - cpu(%d) - active(%d)\n", thr_index, cpu_index, cpu_activeStatus);
   }
   /* available cpu (taken out of reset) */
   cpu_avail = MpcoreGetAvailCpuCount();
   AsciiPrint("Main: AFTER CPUs out of reset, available CPU count: %d\n", cpu_avail);

   /*  initialize event with auto_unsignal disabled (0) so many can enter */
   AsciiPrint ("Main Thread: Initialize event (with auto unsignal disabled (0) so many can enter.\n");
   event1 = EventInit(0, 0, 0);

   /* initialize total thread count */
   shared_counter_g = 0;

   /* initialize handshake flag */
   shared_handshake_stage_g  = 0;

   /* create threads */
   num_of_child_threads = cpu_max;
   for (thr_index=0; thr_index < num_of_child_threads; thr_index++) {

      thr_config_max[thr_index].thread_index = thr_index;

      for (i=0;i<TEST_CONFIG_STR_LEN;i++) {
         thr_config_max[thr_index].thread_name[i] = '\0';
      }
      *((CHAR8 *)&(thr_config_max[thr_index].thread_name)) = 'T';
      *((CHAR8 *)&(thr_config_max[thr_index].thread_name)+1) = 'H';
      *((CHAR8 *)&(thr_config_max[thr_index].thread_name)+2) = 'R';
      *((CHAR8 *)&(thr_config_max[thr_index].thread_name)+3) = '\0';
      uintToString(thr_index, &(thr_config_max[thr_index].thread_name[3]));
      AsciiPrint ("Thread index[%d]\tname: %a\n", thr_index, (CHAR8 *)thr_config_max[thr_index].thread_name);

      /* time to wait before the create thread to execute */
      thr_config_max[thr_index].initial_wait_ms = (UINTN)(1);

      /* execution time for each thread */
      thr_config_max[thr_index].compute_ms = (UINTN)(5000);
   
      /* clear counter of thread to be created */
      shared_counter_group_g[thr_index] = 0;
   
      /* core0 sleeps before creating thread */
      //ThreadSleep (1000);

      thr_create_time = GetTimerCountms ();
      AsciiPrint("Creating thread[%d] (name: %a) pinned to cpu(%d) at %d ms\n", 
                     thr_index, (CHAR8 *)thr_config_max[thr_index].thread_name, current_cpu, thr_create_time);

      /* Generate and setup Thread1 */
      /* lowe priority threads than main thread */
      mpt_group[thr_index] = ThreadCreate ( (CHAR8 *)(thr_config_max[thr_index].thread_name), &t_thread_mpcore_x, 
                                            (void*)&(thr_config_max[thr_index]), (DEFAULT_PRIORITY-4), DEFAULT_STACK_SIZE);

      if (mpt_group[thr_index] == NULL) {
         AsciiPrint("Unable to create thread mtp_group[%d], exiting ...\n", thr_index);
         return EFI_UNSUPPORTED;
      }

      ThreadSetPinnedCpu (mpt_group[thr_index], current_cpu);
      next_cpu = Get_Next_CPU(current_cpu);
      AsciiPrint ("Current CPU: %d Next CPU: %d\n", current_cpu, next_cpu);

      /* update thread cpu field in structure */
      thr_config_max[thr_index].cpu = current_cpu;

      /* set up for next iteration */
      current_cpu = next_cpu;

      ThreadResume (mpt_group[thr_index]);
   }

   /* thread creation done, so they can start computing */
   shared_handshake_stage_g  = 1;

   /* for thread_x print to complete and actually compute */
   ThreadSleep (1000);

   AsciiPrint ("\n\nMain Thread: CPU Active Status during computation: \n");
   cpu_activeMask = MpcoreGetActiveMask();
   AsciiPrint ("Main Thread: CPU Active Mask: %d\n", cpu_activeMask);
   for (thr_index=0; thr_index < num_of_child_threads; thr_index++) {
      cpu_activeStatus = MpcoreIsCpuActive(thr_config_max[thr_index].cpu);
      AsciiPrint ("\t\tThr(%d) - cpu(%d) - active(%d)\n", thr_index, thr_config_max[thr_index].cpu, cpu_activeStatus);
   }

   /* thread computation done, so they can sleep */
   shared_handshake_stage_g  = 2;

   /* for thread_x print to complete and actually compute */
   ThreadSleep (100);

   ThreadSleep (2500);

   /* send event that can be rescheduled */
   AsciiPrint ("Main Thread: Send event for all threads to exit.\n");
   EventSignal(event1, 1);

   for (thr_index=0; thr_index < num_of_child_threads; thr_index++) {
      /* wait for thread to join or timeout in 10000 */
      thread_status = ThreadJoin(mpt_group[thr_index], &thread_rc, 50000);
      AsciiPrint ("Main thread: t_thread_mpcore_x[%d] thread_status(%d)\tthread_rc(%d)\n", 
                           thr_index, thread_status , thread_rc);
      if ((thread_status != 0)|| (thread_rc != 0)) {
         AsciiPrint ("Main Thread: t_thread_mpcore_x[%d] did not complete successfully!\n", thr_index);
         if (test_status ==  EFI_SUCCESS) {
            test_status = EFI_D_ERROR;  
         }
      }
   } // for
   
   ThreadSleep (200);

   AsciiPrint ("\nMain Thread: Number of threads on CPUs:\n");
   for (i=0; i < TOTAL_CPU; i++) {
      AsciiPrint ("\tCPU[%d]: %d\n", i, shared_counter_group_g[i]);
   }
   AsciiPrint ("\n");

   /* Pass criteria check:
    * each counter set by thread to 1
    * */ 
   if (shared_counter_g != num_of_child_threads) {
      AsciiPrint ("Main Thread: counter for total number of thread is not correct.\n");
      AsciiPrint ("Main Thread: Actual counter value = %d \t(num_of_child_threads=%d).\n", 
                     shared_counter_g, num_of_child_threads);
      if (test_status ==  EFI_SUCCESS) {
         test_status = EFI_D_ERROR;  
      }

   }
   else {
      AsciiPrint ("Main Thread: Thread counter value = %d \t(num_of_child_threads=%d).\n", 
                     shared_counter_g, num_of_child_threads);
   }

   if (test_status !=  EFI_SUCCESS) {
      AsciiPrint ("\nTest FAILED with test_status: %d!\n", test_status);
      return test_status;
   }
   else {
      AsciiPrint ("\nTest PASSED!\n");
      return EFI_SUCCESS;
   }
}



EFI_STATUS Thread_mpcore_en_each_test(VOID) {
   UINTN cp, current_cpu, next_cpu;
   UINTN thr_index;
   UINTN num_of_child_threads = 8;   
   Thread *t;
   UINTN thr_create_time;
   KStatus thread_status;
   INT32 thread_rc;
   INT32 i;
   thread_config thr_config_max[num_of_child_threads];
   EFI_STATUS test_status = EFI_SUCCESS;
   UINT32 cpu_max;
   UINT32 cpu_avail;
   UINT32 cpu_deferred;
   UINT32 cpu_activeStatus;
   UINT32 cpu_activeMask;
   UINTN cpu_index;
   UINT32 cpu_init_mask;
   UINT32 cpu_init_result;

   AsciiPrint("Testcase: Thread mpcore enable each CPUs\n");

   t = GetCurrentThread();
   cp = ThreadGetCurrCpu(t);
   current_cpu = cp;
   AsciiPrint("Main: CPU(%d)\n", cp);

   /* set current_cpu to 0 and pin main thread to it */
   current_cpu = 0;
   ThreadSetPinnedCpu (t, current_cpu);
   AsciiPrint("Main: Pin main thread to CPU current_cpu(%d)\n", current_cpu);

   /* get maximum CPU count */
   cpu_max = MpcoreGetMaxCpuCount();
   AsciiPrint("Main: Max CPU count: %d\n", cpu_max);

   /* available cpu (taken out of reset) */
   cpu_avail = MpcoreGetAvailCpuCount();
   AsciiPrint("Main: Available CPU count: %d\n", cpu_avail);

   /* calculate CPU deferred */
   cpu_deferred = cpu_max - cpu_avail;
   AsciiPrint("Main: Deferred CPU count: %d\n", cpu_deferred);

   AsciiPrint ("\n\nMain Thread: CPU Active Status BEFORE Deferred CPU out of reset: \n");
   cpu_activeMask = MpcoreGetActiveMask();
   AsciiPrint ("Main Thread: CPU Active Mask: %d\n", cpu_activeMask);
   for (thr_index=0; thr_index < num_of_child_threads; thr_index++) {
      cpu_index = thr_index;
      cpu_activeStatus = MpcoreIsCpuActive(cpu_index);
      AsciiPrint ("\t\tThr(%d) - cpu(%d) - active(%d)\n", thr_index, cpu_index, cpu_activeStatus);
   }

   /*  initialize event with auto_unsignal disabled (0) so many can enter */
   AsciiPrint ("\nMain Thread: Initialize event (with auto unsignal disabled (0) so many can enter.\n");
   event1 = EventInit(0, 0, 0);

   /* initialize total thread count */
   shared_counter_g = 0;

   /* initialize handshake flag */
   shared_handshake_stage_g  = 0;

   /* create threads */
   num_of_child_threads = cpu_max;
   for (thr_index=0; thr_index < num_of_child_threads; thr_index++) {

      thr_config_max[thr_index].thread_index = thr_index;

      for (i=0;i<TEST_CONFIG_STR_LEN;i++) {
         thr_config_max[thr_index].thread_name[i] = '\0';
      }
      *((CHAR8 *)&(thr_config_max[thr_index].thread_name)) = 'T';
      *((CHAR8 *)&(thr_config_max[thr_index].thread_name)+1) = 'H';
      *((CHAR8 *)&(thr_config_max[thr_index].thread_name)+2) = 'R';
      *((CHAR8 *)&(thr_config_max[thr_index].thread_name)+3) = '\0';
      uintToString(thr_index, &(thr_config_max[thr_index].thread_name[3]));
      AsciiPrint ("Thread index[%d]\tname: %a\n", thr_index, (CHAR8 *)thr_config_max[thr_index].thread_name);

      /* time to wait before the create thread to execute */
      thr_config_max[thr_index].initial_wait_ms = (UINTN)(1);

      /* execution time for each thread */
      thr_config_max[thr_index].compute_ms = (UINTN)(5000);
   
      /* clear counter of thread to be created */
      shared_counter_group_g[thr_index] = 0;
   
      /* core0 sleeps before creating thread */
      //ThreadSleep (1000);

      cpu_activeStatus = MpcoreIsCpuActive(current_cpu);
      AsciiPrint ("\t\tThr(%d) - current_cpu(%d) - active(%d)\n", thr_index, current_cpu, cpu_activeStatus);
      
      if (cpu_activeStatus == 0) {

         cpu_init_mask = get_cpu_mask(current_cpu);

         cpu_activeMask = MpcoreGetActiveMask();
         AsciiPrint ("\t\tBEFORE current_cpu(%d) taken out of reset, CPU Active Mask: %d\n", current_cpu, cpu_activeMask);
         ThreadSleep (2000);
         AsciiPrint ("\t\tBEFORE short while (for scheduler to be up) => CPU(%d) Active Mask: %d\n", current_cpu, cpu_activeMask);

         AsciiPrint("\t\tTake deferred CPU current_cpu(%d) out of reset.", current_cpu);
         cpu_init_result = MpcoreInitDeferredCores (cpu_init_mask);

         cpu_activeStatus = MpcoreIsCpuActive(current_cpu);
         cpu_activeMask = MpcoreGetActiveMask();
         AsciiPrint ("\t\tAFTER current_cpu taken out of reset => CPU(%d)\tactive(%d)\tActive Mask(%d)\tCPU Init(%d)\n", current_cpu, cpu_activeStatus, cpu_activeMask, cpu_init_result);
         ThreadSleep (2000);
         cpu_activeStatus = MpcoreIsCpuActive(current_cpu);
         cpu_activeMask = MpcoreGetActiveMask();
         AsciiPrint ("\t\tAFTER short while (for scheduler to be up) => CPU(%d)\tactive(%d)\tActive Mask(%d)\n", current_cpu, cpu_activeStatus, cpu_activeMask);

      } // cpu_activeStatus == 0
      
      thr_create_time = GetTimerCountms ();
      AsciiPrint("Creating thread[%d] (name: %a) pinned to cpu(%d) at %d ms\n", 
                     thr_index, (CHAR8 *)thr_config_max[thr_index].thread_name, current_cpu, thr_create_time);

      /* Generate and setup Thread1 */
      /* lowe priority threads than main thread */
      mpt_group[thr_index] = ThreadCreate ( (CHAR8 *)(thr_config_max[thr_index].thread_name), &t_thread_mpcore_x, 
                                            (void*)&(thr_config_max[thr_index]), (DEFAULT_PRIORITY-4), DEFAULT_STACK_SIZE);

      if (mpt_group[thr_index] == NULL) {
         AsciiPrint("Unable to create thread mtp_group[%d], exiting ...\n", thr_index);
         return EFI_UNSUPPORTED;
      }

      ThreadSetPinnedCpu (mpt_group[thr_index], current_cpu);
      next_cpu = current_cpu+1;
      AsciiPrint ("Current CPU: %d Next CPU: %d\n", current_cpu, next_cpu);

      /* update thread cpu field in structure */
      thr_config_max[thr_index].cpu = current_cpu;

      /* set up for next iteration */
      current_cpu = next_cpu;

      ThreadResume (mpt_group[thr_index]);
   }

   /* thread creation done, so they can start computing */
   shared_handshake_stage_g  = 1;

   /* for thread_x print to complete and actually compute */
   ThreadSleep (1000);

   AsciiPrint ("\n\nMain Thread: CPU Active Status AFTER each Deferred CPU out of reset: \n");
   cpu_activeMask = MpcoreGetActiveMask();
   AsciiPrint ("Main Thread: CPU Active Mask: %d\n", cpu_activeMask);
   for (thr_index=0; thr_index < num_of_child_threads; thr_index++) {
      cpu_index = thr_index;
      cpu_activeStatus = MpcoreIsCpuActive(cpu_index);
      AsciiPrint ("\t\tThr(%d) - cpu(%d) - active(%d)\n", thr_index, cpu_index, cpu_activeStatus);
   }

 
   /* thread computation done */
   shared_handshake_stage_g  = 2;


   ThreadSleep (2500);

   /* send event that can be rescheduled */
   AsciiPrint ("Main Thread: Send event for all threads to exit.\n");
   EventSignal(event1, 1);

   for (thr_index=0; thr_index < num_of_child_threads; thr_index++) {
      /* wait for thread to join or timeout in 10000 */
      thread_status = ThreadJoin(mpt_group[thr_index], &thread_rc, 50000);
      AsciiPrint ("Main thread: t_thread_mpcore_x[%d] thread_status(%d)\tthread_rc(%d)\n", 
                           thr_index, thread_status , thread_rc);
      if ((thread_status != 0)|| (thread_rc != 0)) {
         AsciiPrint ("Main Thread: t_thread_mpcore_x[%d] did not complete successfully!\n", thr_index);
         if (test_status ==  EFI_SUCCESS) {
            test_status = EFI_D_ERROR;  
         }
      }
   } // for
   
   ThreadSleep (200);

   AsciiPrint ("\nMain Thread: Counters:\n");
   for (i=0; i < TOTAL_CPU; i++) {
      AsciiPrint ("\tCPU[%d]: %d\n", i, shared_counter_group_g[i]);
   }
   AsciiPrint ("\n");

   /* Pass criteria check:
    * each counter set by thread to 1
    * */ 
   if (shared_counter_g != num_of_child_threads) {
      AsciiPrint ("Main Thread: counter for total number of thread is not correct.\n");
      AsciiPrint ("Main Thread: Actual counter value = %d \t(num_of_child_threads=%d).\n", 
                     shared_counter_g, num_of_child_threads);
      if (test_status ==  EFI_SUCCESS) {
         test_status = EFI_D_ERROR;  
      }

   }
   else {
      AsciiPrint ("Main Thread: Thread counter value = %d \t(num_of_child_threads=%d).\n", 
                     shared_counter_g, num_of_child_threads);
   }

   if (test_status !=  EFI_SUCCESS) {
      AsciiPrint ("\nTest FAILED with test_status: %d!\n", test_status);
      return test_status;
   }
   else {
      AsciiPrint ("\nTest PASSED!\n");
      return EFI_SUCCESS;
   }
}

EFI_STATUS Thread_mpcore_en_last_test(VOID) {
   UINTN cp, current_cpu;
   //UINTN next_cpu;
   UINTN thr_index;
   UINTN num_of_child_threads = 8;   
   Thread *t;
   UINTN thr_create_time;
   KStatus thread_status;
   INT32 thread_rc;
   INT32 i;
   thread_config thr_config_max[num_of_child_threads];
   EFI_STATUS test_status = EFI_SUCCESS;
   UINT32 cpu_max;
   UINT32 cpu_avail;
   UINT32 cpu_deferred;
   UINT32 cpu_activeStatus;
   UINT32 cpu_activeMask;
   UINTN cpu_index;
   UINTN cpu_last;
   UINT32 cpu_init_mask;
   UINT32 cpu_init_result;

   AsciiPrint("Testcase: Thread mpcore enable last CPU\n");

   t = GetCurrentThread();
   cp = ThreadGetCurrCpu(t);
   current_cpu = cp;
   AsciiPrint("Main: CPU(%d)\n", cp);

   /* set current_cpu to 0 and pin main thread to it */
   current_cpu = 0;
   ThreadSetPinnedCpu (t, current_cpu);
   AsciiPrint("Main: Pin main thread to CPU current_cpu(%d)\n", current_cpu);

   /* get maximum CPU count */
   cpu_max = MpcoreGetMaxCpuCount();
   AsciiPrint("Main: Max CPU count: %d\n", cpu_max);

   /* available cpu (taken out of reset) */
   cpu_avail = MpcoreGetAvailCpuCount();
   AsciiPrint("Main: Available CPU count: %d\n", cpu_avail);

   /* calculate CPU deferred */
   cpu_deferred = cpu_max - cpu_avail;
   AsciiPrint("Main: Deferred CPU count: %d\n", cpu_deferred);

   AsciiPrint ("\n\nMain Thread: CPU Active Status BEFORE Deferred CPU out of reset: \n");
   cpu_activeMask = MpcoreGetActiveMask();
   AsciiPrint ("Main Thread: CPU Active Mask: %d\n", cpu_activeMask);
   for (thr_index=0; thr_index < num_of_child_threads; thr_index++) {
      cpu_index = thr_index;
      cpu_activeStatus = MpcoreIsCpuActive(cpu_index);
      AsciiPrint ("\t\tThr(%d) - cpu(%d) - active(%d)\n", thr_index, cpu_index, cpu_activeStatus);
   }

   /*  initialize event with auto_unsignal disabled (0) so many can enter */
   AsciiPrint ("\nMain Thread: Initialize event (with auto unsignal disabled (0) so many can enter.\n");
   event1 = EventInit(0, 0, 0);

   /* initialize total thread count */
   shared_counter_g = 0;

   /* initialize handshake flag */
   shared_handshake_stage_g  = 0;

   /* create threads */
   num_of_child_threads = cpu_max;
   cpu_last = cpu_max - 1;
   for (thr_index = cpu_last; thr_index < num_of_child_threads; thr_index++) {

      thr_config_max[thr_index].thread_index = thr_index;
      current_cpu = thr_index;

      for (i=0;i<TEST_CONFIG_STR_LEN;i++) {
         thr_config_max[thr_index].thread_name[i] = '\0';
      }
      *((CHAR8 *)&(thr_config_max[thr_index].thread_name)) = 'T';
      *((CHAR8 *)&(thr_config_max[thr_index].thread_name)+1) = 'H';
      *((CHAR8 *)&(thr_config_max[thr_index].thread_name)+2) = 'R';
      *((CHAR8 *)&(thr_config_max[thr_index].thread_name)+3) = '\0';
      uintToString(thr_index, &(thr_config_max[thr_index].thread_name[3]));
      AsciiPrint ("Thread index[%d]\tname: %a\n", thr_index, (CHAR8 *)thr_config_max[thr_index].thread_name);

      /* time to wait before the create thread to execute */
      thr_config_max[thr_index].initial_wait_ms = (UINTN)(1);

      /* execution time for each thread */
      thr_config_max[thr_index].compute_ms = (UINTN)(5000);
   
      /* clear counter of thread to be created */
      shared_counter_group_g[thr_index] = 0;
   
      /* core0 sleeps before creating thread */
      //ThreadSleep (1000);

      cpu_activeStatus = MpcoreIsCpuActive(current_cpu);
      AsciiPrint ("\t\tThr(%d) - current_cpu(%d) - active(%d)\n", thr_index, current_cpu, cpu_activeStatus);
      
      if (cpu_activeStatus == 0) {
         cpu_init_mask = get_cpu_mask(current_cpu);

         cpu_activeMask = MpcoreGetActiveMask();
         AsciiPrint ("\t\tBEFORE current_cpu(%d) taken out of reset, CPU Active Mask: %d\n", current_cpu, cpu_activeMask);
         ThreadSleep (2000);
         AsciiPrint ("\t\tBEFORE short while (for scheduler to be up) => CPU(%d) Active Mask: %d\n", current_cpu, cpu_activeMask);

         AsciiPrint("\t\tTake deferred CPU current_cpu(%d) out of reset.", current_cpu);
         cpu_init_result = MpcoreInitDeferredCores (cpu_init_mask);

         cpu_activeStatus = MpcoreIsCpuActive(current_cpu);
         cpu_activeMask = MpcoreGetActiveMask();
         AsciiPrint ("\t\tAFTER current_cpu taken out of reset => CPU(%d)\tactive(%d)\tActive Mask(%d)\tCPU Init(%d)\n", current_cpu, cpu_activeStatus, cpu_activeMask, cpu_init_result);
         ThreadSleep (2000);
         cpu_activeStatus = MpcoreIsCpuActive(current_cpu);
         cpu_activeMask = MpcoreGetActiveMask();
         AsciiPrint ("\t\tAFTER short while (for scheduler to be up) => CPU(%d)\tactive(%d)\tActive Mask(%d)\n", current_cpu, cpu_activeStatus, cpu_activeMask);

      } // cpu_activeStatus == 0
      
      thr_create_time = GetTimerCountms ();
      AsciiPrint("Creating thread[%d] (name: %a) pinned to cpu(%d) at %d ms\n", 
                     thr_index, (CHAR8 *)thr_config_max[thr_index].thread_name, current_cpu, thr_create_time);

      /* Generate and setup Thread1 */
      /* lowe priority threads than main thread */
      mpt_group[thr_index] = ThreadCreate ( (CHAR8 *)(thr_config_max[thr_index].thread_name), &t_thread_mpcore_x, 
                                            (void*)&(thr_config_max[thr_index]), (DEFAULT_PRIORITY-4), DEFAULT_STACK_SIZE);

      if (mpt_group[thr_index] == NULL) {
         AsciiPrint("Unable to create thread mtp_group[%d], exiting ...\n", thr_index);
         return EFI_UNSUPPORTED;
      }

      ThreadSetPinnedCpu (mpt_group[thr_index], current_cpu);
      AsciiPrint ("Thread[%d] pinned to Current CPU: %d \n", thr_index, current_cpu);
      //next_cpu = current_cpu+1;
      //AsciiPrint ("Current CPU: %d Next CPU: %d\n", current_cpu, next_cpu);

      /* update thread cpu field in structure */
      thr_config_max[thr_index].cpu = current_cpu;

      /* set up for next iteration */
      //current_cpu = next_cpu;

      ThreadResume (mpt_group[thr_index]);
   }

   /* thread creation done, so they can start computing */
   shared_handshake_stage_g  = 1;

   /* for thread_x print to complete and actually compute */
   ThreadSleep (1000);

   AsciiPrint ("\n\nMain Thread: CPU Active Status AFTER each Deferred CPU out of reset: \n");
   cpu_activeMask = MpcoreGetActiveMask();
   AsciiPrint ("Main Thread: CPU Active Mask: %d\n", cpu_activeMask);
   for (thr_index=0; thr_index < num_of_child_threads; thr_index++) {
      cpu_index = thr_index;
      cpu_activeStatus = MpcoreIsCpuActive(cpu_index);
      AsciiPrint ("\t\tThr(%d) - cpu(%d) - active(%d)\n", thr_index, cpu_index, cpu_activeStatus);
   }

 
   /* thread computation done */
   shared_handshake_stage_g  = 2;


   ThreadSleep (2500);

   /* send event that can be rescheduled */
   AsciiPrint ("Main Thread: Send event for all threads to exit.\n");
   EventSignal(event1, 1);

   for (thr_index=cpu_last; thr_index < num_of_child_threads; thr_index++) {
      /* wait for thread to join or timeout in 10000 */
      thread_status = ThreadJoin(mpt_group[thr_index], &thread_rc, 50000);
      AsciiPrint ("Main thread: t_thread_mpcore_x[%d] thread_status(%d)\tthread_rc(%d)\n", 
                           thr_index, thread_status , thread_rc);
      if ((thread_status != 0)|| (thread_rc != 0)) {
         AsciiPrint ("Main Thread: t_thread_mpcore_x[%d] did not complete successfully!\n", thr_index);
         if (test_status ==  EFI_SUCCESS) {
            test_status = EFI_D_ERROR;  
         }
      }
   } // for
   
   ThreadSleep (200);

   AsciiPrint ("\nMain Thread: Counters:\n");
   for (i=0; i < TOTAL_CPU; i++) {
      AsciiPrint ("\tCPU[%d]: %d\n", i, shared_counter_group_g[i]);
   }
   AsciiPrint ("\n");

   if (test_status !=  EFI_SUCCESS) {
      AsciiPrint ("\nTest FAILED with test_status: %d!\n", test_status);
      return test_status;
   }
   else {
      AsciiPrint ("\nTest PASSED!\n");
      return EFI_SUCCESS;
   }
}

EFI_STATUS Thread_mpcore_en_two_test(VOID) {
   UINTN cp, current_cpu;
   //UINTN next_cpu;
   UINTN thr_index;
   UINTN num_of_child_threads = 8;   
   Thread *t;
   EFI_STATUS test_status = EFI_SUCCESS;
   UINT32 cpu_max;
   UINT32 cpu_avail;
   UINT32 cpu_deferred;
   UINT32 cpu_activeStatus;
   UINT32 cpu_activeStatus_2;
   UINT32 cpu_activeMask;
   UINTN cpu_index;
   UINTN cpu_last;
   UINTN cpu_last_2;
   UINT32 cpu_init_mask;
   UINT32 cpu_init_mask_final;
   UINT32 cpu_init_result;

   AsciiPrint("Testcase: Thread mpcore enable two CPUs\n");

   t = GetCurrentThread();
   cp = ThreadGetCurrCpu(t);
   current_cpu = cp;
   AsciiPrint("Main: CPU(%d)\n", cp);

   AsciiPrint("Main: Pin main thread to CPU current_cpu(%d)\n", current_cpu);

   /* get maximum CPU count */
   cpu_max = MpcoreGetMaxCpuCount();
   AsciiPrint("Main: Max CPU count: %d\n", cpu_max);

   /* available cpu (taken out of reset) */
   cpu_avail = MpcoreGetAvailCpuCount();
   AsciiPrint("Main: Available CPU count: %d\n", cpu_avail);

   /* calculate CPU deferred */
   cpu_deferred = cpu_max - cpu_avail;
   AsciiPrint("Main: Deferred CPU count: %d\n", cpu_deferred);

   AsciiPrint ("\n\nMain Thread: CPU Active Status BEFORE Deferred CPU out of reset: \n");
   cpu_activeMask = MpcoreGetActiveMask();
   AsciiPrint ("Main Thread: CPU Active Mask: %d\n", cpu_activeMask);
   for (cpu_index = 0; cpu_index < cpu_max; cpu_index++) {
      cpu_activeStatus = MpcoreIsCpuActive(cpu_index);
      AsciiPrint ("\t\tcpu(%d) - active(%d)\n", cpu_index, cpu_activeStatus);
   }

   /* create threads */
   num_of_child_threads = cpu_max;
   cpu_init_mask_final = 0;

   cpu_last = cpu_max - 1;
   cpu_activeStatus = MpcoreIsCpuActive(cpu_last);
   AsciiPrint ("cpu_last(%d) - active(%d)\n", cpu_last, cpu_activeStatus);

   if (cpu_activeStatus == 0) {
      cpu_init_mask = get_cpu_mask(cpu_last);
      cpu_init_mask_final = cpu_init_mask_final + cpu_init_mask; 
   } // cpu_activeStatus == 0
   else {
      test_status =  EFI_D_ERROR;
      AsciiPrint ("CPU last (%d) is not active (cpu_activeStatus=%d)\n", cpu_last, cpu_activeStatus);
      AsciiPrint ("\nTest FAILED with test_status: %d!\n", test_status);
      return test_status;
   }
   
   cpu_last_2 = cpu_last - 2;
   cpu_activeStatus_2 = MpcoreIsCpuActive(cpu_last_2);
   AsciiPrint ("cpu_last_2(%d) - active(%d)\n", cpu_last_2, cpu_activeStatus_2);
      
   if (cpu_activeStatus_2 == 0) {
      cpu_init_mask = get_cpu_mask(cpu_last_2);
      cpu_init_mask_final = cpu_init_mask_final + cpu_init_mask; 
   } // cpu_activeStatus_2 == 0
   else {
      test_status =  EFI_D_ERROR;
      AsciiPrint ("CPU (last-2) (%d) is not active (cpu_activeStatus_2=%d)\n", cpu_last_2, cpu_activeStatus_2);
      AsciiPrint ("\nTest FAILED with test_status: %d!\n", test_status);
      return test_status;
   }

   cpu_activeMask = MpcoreGetActiveMask();
   AsciiPrint ("\t\tBEFORE cpu_last(%d) and cpu_last_2(%d) taken out of reset, CPU Active Mask: %d\n", cpu_last, cpu_last_2, cpu_activeMask);

   AsciiPrint("\t\tTake deferred CPU out of reset for cpu_init_mask_final(0x%X).", cpu_init_mask_final);
   cpu_init_result = MpcoreInitDeferredCores (cpu_init_mask_final);

   cpu_activeStatus = MpcoreIsCpuActive(cpu_last);
   cpu_activeStatus_2 = MpcoreIsCpuActive(cpu_last_2);
   cpu_activeMask = MpcoreGetActiveMask();
   AsciiPrint ("\t\tAFTER taken out of reset => cpu_last(%d)\tactive(%d)\tcpu_last_2(%d)\tactive_2(%d)\tActive Mask(%d)\tCPU Init(%d)\n", 
                     cpu_last, cpu_activeStatus, cpu_last_2, cpu_activeStatus_2, cpu_activeMask, cpu_init_result);
   ThreadSleep (2000);
   cpu_activeStatus = MpcoreIsCpuActive(cpu_last);
   cpu_activeStatus_2 = MpcoreIsCpuActive(cpu_last_2);
   cpu_activeMask = MpcoreGetActiveMask();
   AsciiPrint ("\t\tAFTER short while (for scheduler to be up) => cpu_last(%d)\tactive(%d)\tcpu_last_2(%d)\tactive_2(%d)\tActive Mask(%d)\tCPU Init(%d)\n", cpu_last, cpu_activeStatus, cpu_last_2, cpu_activeStatus_2, cpu_activeMask, cpu_init_result);

   /* for thread_x print to complete and actually compute */
   ThreadSleep (1000);

   AsciiPrint ("\n\nMain Thread: CPU Active Status AFTER each Deferred CPU out of reset: \n");
   cpu_activeMask = MpcoreGetActiveMask();
   AsciiPrint ("Main Thread: CPU Active Mask: %d\n", cpu_activeMask);
   for (thr_index=0; thr_index < num_of_child_threads; thr_index++) {
      cpu_index = thr_index;
      cpu_activeStatus = MpcoreIsCpuActive(cpu_index);
      AsciiPrint ("\t\tThr(%d) - cpu(%d) - active(%d)\n", thr_index, cpu_index, cpu_activeStatus);
   }
   
   ThreadSleep (200);

   if (test_status !=  EFI_SUCCESS) {
      AsciiPrint ("\nTest FAILED with test_status: %d!\n", test_status);
      return test_status;
   }
   else {
      AsciiPrint ("\nTest PASSED!\n");
      return EFI_SUCCESS;
   }
}


EFI_STATUS Run_cpt_tests(UINTN argc, CHAR8 **argv) {
   AsciiPrint("Running CPT Test suite.\n");

   SetMem (&tc_config.suite, sizeof (tc_config.suite), 0);
   SetMem (&tc_config.category, sizeof (tc_config.category), 0);
   SetMem (&tc_config.tc, sizeof (tc_config.tc), 0);

   get_test_config (argc, argv);

   /* CPT Test suite */
   AsciiPrint("\ttc_config.suite: %a\ttc_config.category: %d\ttc_config.tc: %d\n",tc_config.suite, tc_config.category, tc_config.tc);

   if (tc_config.category == TC_CATEGORY_LOCK) {
      if (tc_config.tc == TC_TEST_PRIVATE) {
         Lock_private_test();
      }
      else if (tc_config.tc == TC_TEST_SINGLE) {
         Lock_single_test();
      }
      else if (tc_config.tc == TC_TEST_LONG_STR) {
         Lock_long_string_test();
      }
      else if (tc_config.tc == TC_TEST_RECURSIVE) {
         Lock_recursive_test();
      }
      else if (tc_config.tc == TC_TEST_CLEAR) {
         clear_counter();
      }
      else if (tc_config.tc == TC_TEST_SET) {
         set_counter(1);
      }
      else if (tc_config.tc == TC_TEST_GET) {
         get_counter();
         AsciiPrint("shared_counter_g: %d\n", shared_counter_g);
      }
      else if (tc_config.tc == TC_TEST_APP1) {
         Lock_app1_test();
      }
   }
   else if (tc_config.category == TC_CATEGORY_MUTEX) {
      if (tc_config.tc == TC_TEST_TIMEOUT) {
         Mutex_timeout_test();
      }
      else if (tc_config.tc == TC_TEST_PRIORITY_UNEQ) {
         Mutex_priority_unequal_test();
      }
      else if (tc_config.tc == TC_TEST_PRIORITY_EQ) {
         Mutex_priority_equal_test();
      }
   }
   else if (tc_config.category == TC_CATEGORY_EVENT) {
      if (tc_config.tc == TC_TEST_WAIT) {
         Event_wait_test();
      }
      else if (tc_config.tc == TC_TEST_CREATE) {
         Event_wait_on_creation_test();
      }
      else if (tc_config.tc == TC_TEST_TIMEOUT) {
         Event_timeout_test();
      }
      else if (tc_config.tc == TC_TEST_UNSIGNAL) {
         Event_unsignal_test();
      }
      else if (tc_config.tc == TC_TEST_UNSIG_REG) {
         Event_unsignal_regular_test();
      }
      else if (tc_config.tc == TC_TEST_APP1) {
         Event_app1_test();
      }
   }
   else if (tc_config.category == TC_CATEGORY_SEM) {
      if (tc_config.tc == TC_TEST_WAIT) {
         Sem_wait_test();
      }
      else if (tc_config.tc == TC_TEST_TRY) {
         Sem_try_test();
      }
      else if (tc_config.tc == TC_TEST_TIMEOUT) {
         Sem_timeout_test();
      }
      else if (tc_config.tc == TC_TEST_PRIORITY_UNEQ) {
         Sem_priority_unequal_test();
      }
      else if (tc_config.tc == TC_TEST_PRIORITY_EQ) {
         Sem_priority_equal_test();
      }
      else if (tc_config.tc == TC_TEST_APP1) {
         Sem_app1_test();
      }

   }
   else if (tc_config.category == TC_CATEGORY_CPU) {
      if (tc_config.tc == TC_TEST_ANY) {
         Cpu_any_test();
      }
      else if (tc_config.tc == TC_TEST_CURRENT) {
         Cpu_current_test();
      }
      else if (tc_config.tc == TC_TEST_PIN) {
         Cpu_pin_test();
      }
   }
   else if (tc_config.category == TC_CATEGORY_THREAD) {
      if (tc_config.tc == TC_TEST_PRIORITY) {
         Thread_priority_test();
      }
      else if (tc_config.tc == TC_TEST_NAME) {
         Thread_name_test();
      }
      else if (tc_config.tc == TC_TEST_DETACH) {
         Thread_detach_test();
      }
      else if (tc_config.tc == TC_TEST_YIELD) {
         Thread_yield_test();
      }
      else if (tc_config.tc == TC_TEST_PREEMPT) {
         Thread_preempt_test();
      }
      else if (tc_config.tc == TC_TEST_RC) {
         Thread_returnCode_test();
      }
      else if (tc_config.tc == TC_TEST_MAX) {
         Thread_max_test();
      }
      else if (tc_config.tc == TC_TEST_MP_EN_ALL) {
         Thread_mpcore_en_all_test();
      }
      else if (tc_config.tc == TC_TEST_MP_EN_EACH) {
         Thread_mpcore_en_each_test();
      }
      else if (tc_config.tc == TC_TEST_MP_EN_LAST) {
         Thread_mpcore_en_last_test();
      }
      else if (tc_config.tc == TC_TEST_MP_EN_TWO) {
         Thread_mpcore_en_two_test();
      }

   }

   else {
      AsciiPrint("No testcase specified in CPT Test Suite.\n");
   }

   return EFI_SUCCESS;
}


