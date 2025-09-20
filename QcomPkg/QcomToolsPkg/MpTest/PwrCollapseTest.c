/** @file PwrCollapseTest.c
   
  Application to test Multi core/threading in UEFI env.
  
#  Copyright (c) 2017 Qualcomm Technologies, Inc. All righTc reserved.
   
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
#include <Library/PrintLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/KernelLib.h>

#include <Protocol/EFIDisplayPwrCtrl.h>

VOID
TurnOnDisplay (VOID)
{
  EFI_STATUS  Status;
  EFI_QCOM_DISPLAY_PWR_CTRL_PROTOCOL *gDisplayPwrCtrlProtocol = NULL;

  Status = gBS->LocateProtocol(&gQcomDisplayPwrCtrlProtocolGuid,
                               NULL,
                               (VOID**) &gDisplayPwrCtrlProtocol);

  if ((Status != EFI_SUCCESS) || (gDisplayPwrCtrlProtocol == NULL))
  {
    DEBUG ((EFI_D_WARN, "Display Power Control protocol not available\r\n"));
    return;
  }

  Status = gDisplayPwrCtrlProtocol->DisplayPanelPowerControl(EFI_DISPLAY_TYPE_PRIMARY, EFI_DISPLAY_POWER_STATE_ON);

  if (Status != EFI_SUCCESS)
    AsciiPrint("Display turn ON Failed\r\n");
  else
    AsciiPrint("Successfully turned ON display\r\n");
}

VOID
TurnOffDisplay (VOID)
{
  EFI_STATUS Status;
  EFI_QCOM_DISPLAY_PWR_CTRL_PROTOCOL *gDisplayPwrCtrlProtocol = NULL;

  Status = gBS->LocateProtocol(&gQcomDisplayPwrCtrlProtocolGuid,
                               NULL,
                               (VOID**) &gDisplayPwrCtrlProtocol);

  if ((Status != EFI_SUCCESS) || (gDisplayPwrCtrlProtocol == NULL))
  {
    AsciiPrint("Display Power Control protocol not available\r\n");
    return;
  }

  Status = gDisplayPwrCtrlProtocol->DisplayPanelPowerControl(EFI_DISPLAY_TYPE_PRIMARY, EFI_DISPLAY_POWER_STATE_OFF);

  if (Status != EFI_SUCCESS)
    AsciiPrint("Display turn OFF Failed\r\n");
  else
    AsciiPrint("Successfully turned OFF display\r\n");
}

int
PwrCollapseThr (IN VOID  *Arg)
{
  UINTN DurMs = (UINTN)Arg;
  UINT32 RetVal;
  UINTN cp;

  cp = MpcoreGetCurrCpu ();

  AsciiPrint ("Sleeping for %d milli sec on %d\r\n", DurMs, cp);

  RetVal = MpcoreSleepCpu (DurMs);

  AsciiPrint ("Done on %d\r\n", cp);

  return RetVal;
}

Thread* 
LaunchMcPCThread (UINT64 MilliSecs, UINT32 CpuCoreNum)
{
  CHAR8 TName[16];
  Thread *Pct;

  AsciiSPrint (TName, sizeof(TName), "PwrClp%d", CpuCoreNum);

  Pct = ThreadCreate (TName, &PwrCollapseThr, (void*)MilliSecs, HIGHEST_PRIORITY - 1, DEFAULT_STACK_SIZE);

  if (Pct == NULL)
  {
    DEBUG (( EFI_D_ERROR, "Unable to create thread on %d, exiting ...\n", CpuCoreNum));
    return NULL;
  }

  ThreadSetPinnedCpu (Pct, CpuCoreNum);
  ThreadResume (Pct);

  {
    //INT32  RetCode;
    //ThreadJoin (Pct, &RetCode, INFINITE_TIME);
  }
  return Pct;
}

EFI_STATUS RunMultiCorePowerCollapseTest (UINTN Argc, CHAR8** Argv)
{
  int i;
  Thread* Tc[8];
  UINT32 ActiveMask;
  UINTN Cpu, DurMilliSec;

  SetMem (Tc, sizeof(Tc), 0);

  ActiveMask = MpcoreGetActiveMask ();
  TurnOffDisplay ();
  
  for (i = 0; i < Argc;)
  {
    if (i < Argc)
    {
      Cpu = AsciiStrDecimalToUintn (Argv[i]);
      ++i;
    }

    DurMilliSec = 500;

    if (i < Argc)
    {
      DurMilliSec = AsciiStrDecimalToUintn (Argv[i]);
      /* Support both sec and ms, if number is >= 15 then its millisec */
      if (DurMilliSec < 15)
        DurMilliSec *= 1000;

      ++i;
    }

    if (Cpu >= 8)
      AsciiPrint ("Wrong CPU number %d dur %d\n", Cpu, DurMilliSec);

    if (ActiveMask & (1 << Cpu))
      Tc[Cpu] = LaunchMcPCThread (DurMilliSec, Cpu);
  }
  
  for (i=0; i<8; ++i)
  {
    INT32  RetCode;

    if (Tc[i])
      ThreadJoin (Tc[i], &RetCode, INFINITE_TIME);
  }

  TurnOnDisplay ();
  return EFI_SUCCESS;
}

volatile UINT64 SleepStart[8], SleepEnd[8];

int
SleepConflictThr (IN VOID  *Arg)
{
  UINT32 RetVal;
  UINTN cp;
  UINT64 CurrTime, CpuSleepStart, CpuSleepEnd;
  UINT64 SleepDuration;

  cp = MpcoreGetCurrCpu ();

  CpuSleepStart = SleepStart[cp];
  CpuSleepEnd = SleepEnd[cp];

  SleepDuration = (CpuSleepEnd - CpuSleepStart)/1000;

  DEBUG (( EFI_D_ERROR, "Sleeping for %d milli sec on %d\r\n", SleepDuration, cp));

  do {
    CurrTime = GetTimerCountus ();
  } while (CurrTime < CpuSleepStart);
  
  RetVal = MpcoreSleepCpu (SleepDuration);

  DEBUG (( EFI_D_ERROR, "Done on %d\r\n", cp));

  return RetVal;
}

Thread* 
LaunchSleepConflictThread (UINT32 CpuCoreNum)
{
  CHAR8 TName[16];
  Thread *Pct;

  AsciiSPrint (TName, sizeof(TName), "PwrClp%d", CpuCoreNum);

  Pct = ThreadCreate (TName, &SleepConflictThr, (void*)0, HIGHEST_PRIORITY - 1, DEFAULT_STACK_SIZE);

  if (Pct == NULL)
  {
    DEBUG (( EFI_D_ERROR, "Unable to create thread on %d, exiting ...\n", CpuCoreNum));
    return NULL;
  }

  ThreadSetPinnedCpu (Pct, CpuCoreNum);
  ThreadResume (Pct);

  return Pct;
}

EFI_STATUS RunSleepConflictTest (UINTN Argc, CHAR8** Argv)
{
  int i;
  Thread* Tc[8];
  UINT32 ActiveMask;
  UINT64 CurTime;
  int testloops, max_test_loops;

  max_test_loops = 200;

  if (Argc > 0)
    max_test_loops = AsciiStrDecimalToUintn (Argv[0]);

  SetMem (Tc, sizeof(Tc), 0);

  ActiveMask = MpcoreGetActiveMask ();
  TurnOffDisplay ();

  for (testloops = 0; testloops < max_test_loops; ++testloops)
  {
    CurTime = GetTimerCountus ();
    CurTime += 100000;
    
    for (i = 0; i < 8; ++i)
    {
      SleepStart[i] = CurTime;
      SleepEnd[i] = CurTime + 100000;
    }

    SleepStart[0] = SleepEnd[1] - max_test_loops + testloops * 2;
    SleepEnd[0] = SleepStart[0] + 100000;

    for (i = 1; i < 8; ++i)
    {
      if (((1 << i) & ActiveMask) == 0)
        continue;
      Tc[i] = LaunchSleepConflictThread (i);
    }

    Tc[0] = LaunchSleepConflictThread (0);
    
    for (i=0; i<8; ++i)
    {
      INT32  RetCode;

      if (Tc[i])
      {
        ThreadJoin (Tc[i], &RetCode, INFINITE_TIME);
        Tc[i] = 0;
      }
    }
    DEBUG (( EFI_D_ERROR, "================= Completed loop %d ===================\n\n", testloops));
  }

  TurnOnDisplay ();
  return EFI_SUCCESS;
}

/*
 *  Some sample tests for regression test:
 *
 *  start fv3:\mptest "aux 0xff lmh frq a temp load pc  5 22   6 15   2 25   3 20   7 8   1 20   4 8   0 18"
 *  start fv3:\mptest "frq a temp load pc  1 22   2 15   3 25   4 20   5 8   6 20   7 8   0 18"
 *  start fv3:\mptest "frq a temp load pc  1 22   3 22   5 22   7 22   6 22   4 22   2 22   0 22"
 *  start fv3:\mptest "frq a temp load pc  1 20   3 19   5 18   7 17   6 16   4 15   2 14   0 13"
 *  start fv3:\mptest "frq a pc  5 6    6 7    2 8   3 9   7 9   1 9   4 10   0 10"
 *  start fv3:\mptest "frq a pc  5 2    6 2    2 2   3 2   7 2   1 2   4 2   0 2"
 *  start fv3:\mptest "pc  5 2    6 2    2 2   3 2   7 2   1 2   4 2   0 2"
 *
 *  start fv3:\mptest "aux 0xf lmh frq a temp load pc  2 25   3 20   1 20   0 18"
 *  start fv3:\mptest "frq a temp load pc  1 22   2 15   3 25   0 18"
 *  start fv3:\mptest "frq a temp load pc  1 22   3 22   2 22   0 22"
 *  start fv3:\mptest "frq a temp load pc  1 20   3 19   2 14   0 13"
 *  start fv3:\mptest "frq a pc  2 8   3 9   1 9   0 10"
 *  start fv3:\mptest "frq a pc  2 2   3 2   1 2   0 2"
 *  start fv3:\mptest "pc  2 2   3 2   1 2   0 2"
 *
 *  start fv3:\mptest "aux 0xff lmh frq a temp load pc  5 22   6 15   2 25   3 20   7 8   1 20   4 8   0 18"
 *  start fv3:\mptest "frq a temp load pc  1 22   2 15   3 25   4 20   5 8   6 20   7 8   0 18"
 *  start fv3:\mptest "frq a temp load pc  1 22   3 22   5 22   7 22   6 22   4 22   2 22   0 22"
 *  start fv3:\mptest "frq a temp load pc  1 20   3 19   5 18   7 17   6 16   4 15   2 14   0 13"
 *  start fv3:\mptest "frq a pc  5 6    6 7    2 8   3 9   7 9   1 9   4 10   0 10"
 *  start fv3:\mptest "frq a pc  5 2    6 2    2 2   3 2   7 2   1 2   4 2   0 2"
 *  start fv3:\mptest "pc  5 2    6 2    2 2   3 2   7 2   1 2   4 2   0 2"
 *
 * */
