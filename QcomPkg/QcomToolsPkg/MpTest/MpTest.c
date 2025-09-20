/** @file MpTest.c
   
  Application to test Multi core/threading in UEFI env.
  
  Copyright (c) 2017-2019 Qualcomm Technologies, Inc. All rights reserved.
   
**/
/*
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
 */

/*=============================================================================
                              EDIT HISTORY


  when         who     what, where, why
  ----------   ---     -----------------------------------------------------------
  10/30/2017   cm      Add dcvsh and thermal enable support
  10/13/2017   ih      Add runtime profile switching support
  06/16/2017   yg      Initial version
=============================================================================*/

#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/QcomBaseLib.h>
#include <Library/QcomLib.h>
#include <Library/QcomUtilsLib.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/PrintLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Protocol/LoadedImage.h>
#include <Protocol/EFIClock.h>
#include <Protocol/EFITsens.h>
#include <Library/KernelLib.h>
#include <Protocol/EFIScm.h>
#include <Include/scm_sip_interface.h>
#include <Library/UncachedMemoryAllocationLib.h>
#include <Library/LockLib.h>

#define LMH_RUNTIME_PROFILE_ID          1
#define LMH_BOOT_PROFILE_ID             0

#define SCM_MAX_NUM_PARAMETERS          10
#define SCM_MAX_NUM_RESULTS             4

#define DCVSH_NODE  0x44435653
#define DCVSH_ENABLE                0x454e424c /*ENBL*/
#define LLMCPSILVER 0x6370302d
#define LLMCPGOLD   0x6370312d
#define DCVSH_THERMAL_ALGO          0x54484d4c /*THRM*/


typedef struct {
  uint32_t            SmcId;
  uint32_t            ParamId;
  uint64_t            Parameters[SCM_MAX_NUM_PARAMETERS];
  uint64_t            Results[SCM_MAX_NUM_RESULTS];
} sip_syscall_req_t;

typedef struct {
  uint64_t            count;
} tz_lmh_get_profiles_res_t;

int set_LMH_dcvsh(uint32_t node_id, uint32_t algo, uint32_t action, uint32_t val)
{
  EFI_STATUS status = EFI_SUCCESS;
  sip_syscall_req_t   sipReq = {0};
  uint32_t *payload;

//  tz_lmh_change_profile_req_t *tzReq;
  tz_syscall_rsp_t            *tzRes;
  QCOM_SCM_PROTOCOL  *QcomSCM = NULL;

  payload = UncachedAllocatePool(sizeof(uint32_t) * 5);
  if (payload == NULL)
    return 2;

  status = gBS->LocateProtocol(&gQcomScmProtocolGuid, NULL, (VOID **)&QcomSCM);
  if ((status != EFI_SUCCESS) || (QcomSCM == NULL))
  {
    AsciiPrint ("ERROR: SCM protocol load failed\n");
    return 0;
  }
  
  sipReq.SmcId = TZ_LMH_DCVSH_CONFIG_ID;
  sipReq.ParamId = TZ_LMH_DCVSH_CONFIG_PARAM_ID;
  
  payload[0] = algo;
  payload[1] = 0;
  payload[2] = action;
  payload[3] = 1;
  payload[4] = val;

//  tzReq = (tz_lmh_change_profile_req_t *)sipReq.Parameters;
  tzRes = (tz_syscall_rsp_t *) sipReq.Results;

  sipReq.Parameters[0] = (uint64_t)payload;
  sipReq.Parameters[1] = sizeof(uint32_t) * 5;
  sipReq.Parameters[2] = DCVSH_NODE;
  sipReq.Parameters[3] = node_id;
  sipReq.Parameters[4] = 0;

  status = QcomSCM->ScmSipSysCall(QcomSCM, sipReq.SmcId, sipReq.ParamId, sipReq.Parameters, sipReq.Results);

  if (status == EFI_SUCCESS)
  {
    if(tzRes->status == 1) {
      AsciiPrint("LMH retval %d\n", val);
      return 1;
    }
  } else {
    AsciiPrint("ERROR status: %d: set_LMH_dcvsh\n", status);
  }
  return 0;
}

int set_LMH_profile(uint64_t profile)
{
  EFI_STATUS status = EFI_SUCCESS;
  sip_syscall_req_t   sipReq = {0};

  tz_lmh_change_profile_req_t *tzReq;
  tz_syscall_rsp_t            *tzRes;
  QCOM_SCM_PROTOCOL  *QcomSCM = NULL;

  status = gBS->LocateProtocol(&gQcomScmProtocolGuid, NULL, (VOID **)&QcomSCM);
  if ((status != EFI_SUCCESS) || (QcomSCM == NULL))
  {
    AsciiPrint ("ERROR: SCM protocol load failed\n");
    return 0;
  }
  
  sipReq.SmcId = TZ_LMH_CHANGE_PROFILE_ID;
  sipReq.ParamId = TZ_LMH_CHANGE_PROFILE_PARAM_ID;
  
  tzReq = (tz_lmh_change_profile_req_t *)sipReq.Parameters;
  tzRes = (tz_syscall_rsp_t *) sipReq.Results;
  tzReq->profile = profile;

  status = QcomSCM->ScmSipSysCall(QcomSCM, sipReq.SmcId, sipReq.ParamId, sipReq.Parameters, sipReq.Results);

  if (status == EFI_SUCCESS)
  {
    if(tzRes->status == 1) {
      AsciiPrint("LMH profile chnaged to %d\n", profile);
      return 1;
    }
  } else {
    AsciiPrint("ERROR status: %d: set_LMH_profile\n", status);
  }
  return 0;
}

VOID EnableAuxCores (UINT32 CoreMask)
{
  if (MpcoreGetAvailCpuCount() < MpcoreGetMaxCpuCount())
    MpcoreInitDeferredCores (CoreMask);
  ThreadSleep (10);
}

/* TODO: This is target specific info, need to abstract it */
CHAR8* ClkInfo[8] = {
  "Silver Cluster",
  "Gold Cluster",
  "Gold+ Cluster",
  "L3 Cache",
  NULL, NULL, NULL, NULL,
};

VOID SwitchToHighPerfMode (VOID)
{
  EFI_CLOCK_PROTOCOL *EfiClockProtocol = NULL;
  UINT32 PerfLevel, CpuFreq, i, Vg;

  // now bump up the clock
  if (gBS->LocateProtocol (&gEfiClockProtocolGuid, NULL, (VOID **)&EfiClockProtocol) != EFI_SUCCESS) 
    AsciiPrint ("Unable to get Clock protocol\n");

  EfiClockProtocol->GetCpuPerfLevel (EfiClockProtocol, 0, &PerfLevel);
  EfiClockProtocol->GetCpuPerfLevelFrequency (EfiClockProtocol, 0, PerfLevel, &CpuFreq, &Vg);
  CpuFreq = CpuFreq / 1000000;
  AsciiPrint ("CPU 0 clk is %d MHz\n", CpuFreq);

  for (i = 0; i < MpcoreGetAvailCpuCount (); ++i)
  {
    if (ClkInfo[i] == NULL)
      continue;

    EfiClockProtocol->GetMaxPerfLevel (EfiClockProtocol, i, &PerfLevel);
    EfiClockProtocol->SetCpuPerfLevel (EfiClockProtocol, i, PerfLevel, &CpuFreq);
    CpuFreq = CpuFreq / 1000000;
    AsciiPrint ("%a Freq : %d MHz\n", ClkInfo[i], CpuFreq);
  }
}

VOID SwitchToLowPerfMode (VOID)
{
  EFI_CLOCK_PROTOCOL *EfiClockProtocol = NULL;
  UINT32 PerfLevel, CpuFreq, i;

  // now bump up the clock
  if (gBS->LocateProtocol (&gEfiClockProtocolGuid, NULL, (VOID **)&EfiClockProtocol) != EFI_SUCCESS) 
    AsciiPrint ("Unable to get Clock protocol\n");

  for (i = 1; i < MpcoreGetAvailCpuCount (); ++i)
  {
    if (ClkInfo[i] == NULL)
      continue;

    EfiClockProtocol->GetMinPerfLevel (EfiClockProtocol, i, &PerfLevel);
    EfiClockProtocol->SetCpuPerfLevel (EfiClockProtocol, i, PerfLevel, &CpuFreq);
    CpuFreq = CpuFreq / 1000000;
    AsciiPrint ("%a Freq : %d MHz\n", ClkInfo[i], CpuFreq);
  }
}

VOID* TSensLock;

EFI_STATUS CheckCPUTemp (UINTN* MaxTemp OPTIONAL)
{
  EFI_TSENS_PROTOCOL *Tsens = NULL;
  int temp, sensor_id, max = 0;
  EFI_STATUS Status = EFI_SUCCESS;
  CHAR8 TempStr[128], cpbuff[32];

  Status = gBS->LocateProtocol(&gEfiTsensProtocolGuid, NULL, (VOID **)&Tsens);

  if (Status != EFI_SUCCESS)
    return Status;

  TempStr[0] = 0;
  AsciiStrCatS (TempStr, sizeof(TempStr), "CPU Temp F : ");

  Lock (TSensLock);

  /* Sensor ID's change based on floorplan of the chip */
  for (sensor_id = 1; sensor_id < 5; sensor_id ++) {
     Status = Tsens->GetTemp(sensor_id, &temp);
     if (max < temp)
       max = temp;
     //AsciiSPrint (cpbuff, sizeof(cpbuff), "%d: %d, ", sensor_id-1, temp);
     AsciiSPrint (cpbuff, sizeof(cpbuff), " %3d,",temp/10);
     AsciiStrCatS (TempStr, sizeof(TempStr), cpbuff);
  }

  for (sensor_id = 7; sensor_id < 11; sensor_id ++) {
     Status = Tsens->GetTemp(sensor_id, &temp);
     if (max < temp)
       max = temp;
     //AsciiSPrint (cpbuff, sizeof(cpbuff), "%d: %d, ", sensor_id-3, temp);
     AsciiSPrint (cpbuff, sizeof(cpbuff), " %3d,", temp/10);
     AsciiStrCatS (TempStr, sizeof(TempStr), cpbuff);
  }

  AsciiStrCatS (TempStr, sizeof(TempStr), "\n");
  AsciiPrint (TempStr);
  if (MaxTemp)
    *MaxTemp = max;

  UnLock (TSensLock);

  return Status;
}

volatile UINTN ExitMgmtThr = 0;

static int TempMonitorThread (VOID* Arg)
{
  while (1)
  {
    ThreadSleep (5000);
    CheckCPUTemp (0);
    if (ExitMgmtThr)
      break;
  }

  return 0;
}

#define SMP_MAX_CPUS      8

UINT32 CpuLoadData[16];

VOID PrintStats (VOID)
{
  UINT32 i;
  CHAR8 TempStr[128], LoadStr[16];

  AsciiSPrint (TempStr, sizeof(TempStr), "\nCpu Load :");

  if (GetCpuLoad (&CpuLoadData) != EFI_SUCCESS)
  {
    DEBUG ((EFI_D_WARN, "Unable to CPU Load values\r\n"));
    return;
  }

  for (i = 0; i < SMP_MAX_CPUS; ++i)
  {
    AsciiSPrint (LoadStr, sizeof(LoadStr), " %3d,", CpuLoadData[i]);
    AsciiStrCatS (TempStr, sizeof(TempStr), LoadStr);
  }

  AsciiStrCatS (TempStr, sizeof(TempStr), "\n");
  AsciiPrint (TempStr);
}

static int CpuLoadStatsThr (VOID* Arg)
{
  while (1)
  {
    ThreadSleep (5000);
    PrintStats ();
    if (ExitMgmtThr)
      break;
  }

  return 0;
}

VOID TimeoutNotify (EFI_EVENT Evt, VOID* Arg)
{
//  AsciiPrint ("Timeout happened\r\n");
}

#define EFI_TIMER_MILLI_SEC(x)   ((x) * 10000)

#define EVT_TEST_STALL       1
#define EVT_TEST_READ_KEY    2
#define EVT_TEST_TPL_CHG     4


volatile UINTN Counter;
VOID
EvtLockTest (UINTN Flags, UINTN TimeoutSec)
{
  EFI_STATUS              Status;
  EFI_EVENT               TimeoutEvt;
  UINTN                   KeyCode, OldTPL, Strt;
  EFI_INPUT_KEY           Key;
  volatile static UINTN   done = 0;

  Status = gBS->CreateEvent (EVT_TIMER, TPL_CALLBACK,
                             TimeoutNotify, NULL, &TimeoutEvt);
  if (EFI_ERROR (Status))
    return ;

  Status = gBS->SetTimer (TimeoutEvt, TimerRelative, EFI_TIMER_MILLI_SEC(TimeoutSec * 1000));
  if (EFI_ERROR (Status))
    return;

  if (Flags & EVT_TEST_TPL_CHG)
    OldTPL = gBS->RaiseTPL (TPL_CALLBACK);

  Strt = GetTimerCountms ();
  AsciiPrint ("Event locking test starting %d\r\n", Strt);

  while (EFI_ERROR (gBS->CheckEvent (TimeoutEvt)))
  {
    if (Flags & EVT_TEST_STALL)
      gBS->Stall (10);

    if (Flags & EVT_TEST_READ_KEY)
    {
      if (ReadAnyKey (&Key, READ_KEY_ATTRIB_NO_BLOCKING) == EFI_SUCCESS)
      {
        KeyCode = Key.ScanCode;
        if (KeyCode == 0)
          KeyCode = Key.UnicodeChar;
        AsciiPrint ("Got key %X\r\n", KeyCode);
        if (KeyCode == 'q')
          break;
      }
    }

    ++Counter;

    if (done)
      break;
  }
  AsciiPrint ("Test Done, timed out in %d ms\r\n", GetTimerCountms() - Strt);

  if (Flags & EVT_TEST_TPL_CHG)
    gBS->RestoreTPL (OldTPL);
}

/**
  Entry point for Menu App

  @param[in] ImageHandle    The firmware allocated handle for the EFI image.  
  @param[in] SystemTable    A pointer to the EFI System Table.
  
  @retval EFI_SUCCESS           Erase successfully.
  @retval EFI_DEVICE_ERROR      Failed to program
  @retval EFI_OUT_OF_RESOURCES  Failed to allocate memory for 
                                the buffers
**/

EFI_STATUS RunMultiThreadTest (VOID);
EFI_STATUS RunPwrCollapseTest (UINTN DurationSec);
EFI_STATUS RunAuxPwrCollapseTest (UINTN DurationSec, UINTN AuxCoreNum);
EFI_STATUS AllocTest (IN UINTN  ThrCnt);
EFI_STATUS RunMultiCorePowerCollapseTest (UINTN Argc, CHAR8** Argv);
EFI_STATUS LaunchBBMTest (UINTN Arg);
EFI_STATUS RunSleepConflictTest (UINTN Argc, CHAR8** Argv);


CHAR8* PredefinedTests [] = {
     /* First just 4 core power collapse */
    "aux 0xFF",
    "pc  1 4002   2 8090   3 4426   7 6708    5 42  4 5420  6 404  0 8199",
#if 0
    "pc  1 4   3 4   2 4   0 4",
    "pc  1 1   3 2   2 3   0 4",
    "pc  1 4   3 3   2 2   0 2",
    "pc  2 3   3 4   1 4   0 5",
    "pc  2 2   3 2   1 2   0 2",
    "pc  2 2   1 2   3 2   0 2",
    "pc  2 2   3 2   0 2",


    /* All 8 cores power collapse */
    "aux 0xff pc  5 12   6 8   2 12   3 14   7 3   1 4   4 5   0 6",
    "pc  5 2   6 8   2 12   3 14   7 3   1 4   4 5   0 6",
    "pc  1 3   3 3   5 3   7 3   6 3   4 3   2 3   0 3",
    "pc  1 9   3 8   5 7   7 6   6 5   4 4   2 3   0 2",
    "pc  5 7    6 8    2 6   3 5   7 5   1 5   4 6   0 7",
    "pc  5 4    6 5    2 6   3 7   7 8   1 9   4 9   0 9",

    "aux 0xff frq a temp load alloc 100",
    "aux 0xff frq a temp load mt",
    "bbm",
#endif
};


EFI_STATUS
EFIAPI
MpTestMain ( IN EFI_HANDLE         ImageHandle,
             IN EFI_SYSTEM_TABLE   *SystemTable)
{
  EFI_STATUS                  Status = EFI_SUCCESS;
  UINTN                       Argc, i;
  CHAR8                     **Argv, **TempArgv;
  Thread                      *TempThread, *CpuUsageThr;
  CHAR8                       FreqSwitch = 'N';
  UINTN                       Predefined = 0;
  INT32                       RetCode;

  TempThread = NULL;
  CpuUsageThr = NULL;
  Status = GetCmdLineArgs (ImageHandle, &Argc, &Argv);

  if (EFI_ERROR (Status))
    return Status;

  TempArgv = Argv;

  /* Check if the app was launched by shell and the Cmd str is actually the
   * Cmd app name itself, ie it would be "cmd" or "cmd.efi", then again process
   * to split, this is because Ebl doesn't pass the name of app, but shell does
   * other methods could yeild mixed results (like BDS launching apps via Menu, etc) */
  if (Argc &&
      ((AsciiStriCmp (Argv[0], "mptest") == 0) ||
       (AsciiStriCmp (Argv[0], "mptest.efi") == 0)))
  {
    --Argc;
    ++Argv;
  }

  /*  Usage : start fvx:mptest "[aux msk ][lmh ][frq h/l/a ][temp ][load ][pc x xx ... ][alloc][mt][fq l]"
   *  commands :
   *       aux : power up aux cores, mask is 1 char hex of bitmask of CPU's to enable
   *       lmh : set runtime profile of LMH
   *       frq : set frequency to high or low or automatic
   *       temp : temperature monitor thread
   *       load : Cpu load monitor thread
   *       pc : power collapse test
   *             params : cpu num, secs to sleep, upto 8 or max cores
   *                      eg: pc 2 8 3 10 4 6 0 15 etc
   *                         in above eg: put core 2 to sleep for 8 sec
   *                                      core 3 to sleep for 10 sec
   *                                      core 4 to sleep for 6 sec
   *                                      core 0 to sleep for 15 sec
   *       alloc : alloc test, param thread count (default 128 if not given)
   *       mt : multi thread stress test
   *       
   *  Commands are executed in the given order                              
   *
   *  eg:
   *    Ebl>start fv3:\mptest "aux 0xff frq a temp load pc 5 22 6 15 2 25 3 20 7 8 1 20 4 8 0 18"
   *    Ebl>start fv3:\mptest "aux 0xff frq a temp load alloc 100"
   *    Ebl>start fv3:\mptest "aux 0xff frq a temp load mt"
   */

  for (i = 0 ; i < Argc; )
  {
    if (AsciiStriCmp (Argv[i], "aux") == 0)
    {
      UINTN Mask = 0xFF;
      ++i;

      if (i < Argc)
      {
        Mask = AsciiStrHexToUintn (Argv[i]);
        ++i;
      }

      EnableAuxCores (Mask);
      continue;
    }

    if (AsciiStriCmp (Argv[i], "bbm") == 0)
    {
      UINTN MaxDelay = 20;

      ++i;

      if (i < Argc)
      {
        MaxDelay = AsciiStrDecimalToUintn (Argv[i]);
        ++i;
      }
      LaunchBBMTest (MaxDelay);
      continue;
    }

    if (AsciiStriCmp (Argv[i], "lmh") == 0)
    {
      ++i;
      
      AsciiPrint ("lmh profile setting is not required anymore\r\n");
      continue;
    }

    if (AsciiStriCmp (Argv[i], "evt") == 0)
    {
      ++i;
      UINTN Opt = 0, ToSec = 2;

      if (i < Argc)
      {
        Opt = AsciiStrHexToUintn (Argv[i]);
        ++i;
      }

      if (i < Argc)
      {
        ToSec = AsciiStrHexToUintn (Argv[i]);
        ++i;
      }

      EvtLockTest (Opt, ToSec);

      continue;
    }

    if (AsciiStriCmp (Argv[i], "asrt") == 0)
    {
      ++i;
      AsciiPrint ("Will crash with assert\r\n");
      ASSERT (FALSE);
      AsciiPrint ("Test failed...\r\n");
      continue;
    }

    /*
     *  st fv3:\mptest  "dabt 0x8000000000"
     *  st fv3:\mptest  "dabt 0x40000000"
     *  st fv3:\mptest  "dabt 0x85800000"
     *  st fv3:\mptest  "dabt 0x85700000"
     */
    if (AsciiStriCmp (Argv[i], "dabt") == 0)
    {
#define OPRN_READ      1
#define OPRN_WRITE     2
      UINT64* DPtr = (UINT64*)Argv[i];
      UINTN   Oprn = OPRN_WRITE;
      ++i;

      if (i < Argc)
      {
        char ch = Argv[i][0];
        ++i;
        if ((ch == 'r') || (ch == 'R'))
          Oprn = OPRN_WRITE;
        else if ((ch == 'w') || (ch == 'W'))
          Oprn = OPRN_WRITE;
        else
          --i;
      }

      if (i < Argc)
      {
        DPtr = (UINT64*) AsciiStrHexToUintn (Argv[i]);
        ++i;
      }

      AsciiPrint ("Data abort crash test deref %X\r\n", DPtr);

      AsciiPrint ("Data at %X is %X\r\n", DPtr, *DPtr);

      AsciiPrint ("Test failed, no crash for the address in question...\r\n");

      continue;
    }

    if (AsciiStriCmp (Argv[i], "iabt") == 0)
    {
      VOID (*AbtFn)(VOID) = (VOID (*)(VOID)) Argv[i];
      ++i;

      if (i < Argc)
      {
        AbtFn = (VOID (*)(VOID))AsciiStrHexToUintn (Argv[i]);
        ++i;
      }

      AsciiPrint ("Will execute instr at %X\r\n", AbtFn);

      AbtFn ();
      
      AsciiPrint ("Test failed...\r\n");
      continue;
    }

    if (AsciiStriCmp (Argv[i], "other") == 0)
    {
      UINTN OldTpl; 

      ++i;

      OldTpl = gBS->RaiseTPL (TPL_CALLBACK);

      AsciiPrint ("Ascii print test with raised TPL, ");
      AsciiPrint ("Passed\r\n");

      DEBUG ((EFI_D_WARN, "Debug log print to UART test with raised TPL, "));
      DEBUG ((EFI_D_WARN, "Passed\r\n"));

      gBS->RestoreTPL (OldTpl);
      continue;
    }

    if (AsciiStriCmp (Argv[i], "frq") == 0)
    {
      ++i;
      if (i < Argc)
      {
        if ((Argv[i][0] == 'L') || (Argv[i][0] == 'l'))
          SwitchToHighPerfMode ();

        if ((Argv[i][0] == 'H') || (Argv[i][0] == 'h'))
          SwitchToLowPerfMode ();

        if ((Argv[i][0] == 'A') || (Argv[i][0] == 'a'))
        {
          SwitchToHighPerfMode ();
          FreqSwitch = 'A';
        }
        ++i;
      }
      continue;
    }

    if (AsciiStriCmp (Argv[i], "temp") == 0)
    {
      ++i;
      TSensLock = InitLock ("TSENS");

      TempThread = ThreadCreate ("TempMonitor", &TempMonitorThread, (void*)0, DEFAULT_PRIORITY + 5, DEFAULT_STACK_SIZE);
      ThreadSetPinnedCpu (TempThread, 0);
      ThreadResume (TempThread);
      continue;
    }

    if (AsciiStriCmp (Argv[i], "load") == 0)
    {
      ++i;
      CpuUsageThr = ThreadCreate ("CpuUsage", &CpuLoadStatsThr, (void*)0, DEFAULT_PRIORITY + 5, DEFAULT_STACK_SIZE);
      ThreadSetPinnedCpu (CpuUsageThr, 0);
      ThreadResume (CpuUsageThr);
      continue;
    }

    if (AsciiStriCmp (Argv[i], "pc") == 0)
    {
      ++i;

      RunMultiCorePowerCollapseTest (Argc - i, &Argv[i]);
      break;
    }

    if (AsciiStriCmp (Argv[i], "slp") == 0)
    {
      ++i;

      RunSleepConflictTest (Argc - i, &Argv[i]);
      break;
    }

    if (AsciiStriCmp (Argv[i], "alloc") == 0)
    {
      UINTN ThrCnt = 128;

      ++i;

      if (i < Argc)
      {
        ThrCnt = AsciiStrDecimalToUintn (Argv[i]);
        ++i;
      }

      Status = AllocTest (ThrCnt);
      continue;
    }

    if (AsciiStriCmp (Argv[i], "mt") == 0)
    {
      ++i;
      RunMultiThreadTest ();
      continue;
    }

    /* Keep this as last item so that we can just re-use above check and launch the tests */
    if (Predefined || (AsciiStriCmp (Argv[i], "predef") == 0))
    {
      static UINTN PreDefTest = 0;
      static UINTN MaxPredefTestCnt = 0;
      CHAR8*  TestCmd = NULL;

      ++i;

      if (TempArgv)
      {
        if (i < Argc)
        {
          MaxPredefTestCnt = AsciiStrDecimalToUintn (Argv[i]);
          ++i;
        }
        else
          MaxPredefTestCnt = sizeof(PredefinedTests)/sizeof(PredefinedTests[0]);

        FreePool (TempArgv);
        TempArgv = NULL;

        Argv = AllocatePool (sizeof(CHAR8*) * 32);
        TestCmd = AllocatePool (512);
      }

      if ((TestCmd == NULL) || (Argv == NULL))
      {
        AsciiPrint ("Failed to allocate memory\r\n");
        break;
      }

      if ((PreDefTest >= MaxPredefTestCnt) || (PreDefTest >= sizeof(PredefinedTests)/sizeof(PredefinedTests[0])))
        break;

      AsciiStrnCpy (TestCmd, PredefinedTests[PreDefTest], 512);

      AsciiPrint ("\r\nTest %d : %a\r\n", PreDefTest, TestCmd);

      Status = ProcessCmdLineStr (TestCmd, AsciiStrLen(TestCmd), &Argc, Argv);

      if (Predefined == 0)
        Predefined++;
      else
        ThreadSleep (1);

      PreDefTest++;
      i = 0;
    }
    continue;
  }
  
  ++ExitMgmtThr;

  if (TempThread) ThreadJoin (TempThread, &RetCode, 8000);
  if (CpuUsageThr) ThreadJoin (CpuUsageThr, &RetCode, 8000);

  if (FreqSwitch == 'A')
    SwitchToLowPerfMode ();

  AsciiPrint ("\r\nPress any key to exit...\r\n");
  ReadAnyKey (NULL, 0);

  AsciiPrint ("\r\nExiting...\r\n");
  
  return Status;
}

