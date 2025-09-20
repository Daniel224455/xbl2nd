/** @file cpt_MpTest.c
   
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
#include <Library/QcomUtilsLib.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/PrintLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Protocol/EFIClock.h>
#include <Library/KernelLib.h>
#include <Protocol/EFIScm.h>
#include <Library/LockLib.h>

VOID EnableAllCores (VOID)
{
  if (MpcoreGetAvailCpuCount() < MpcoreGetMaxCpuCount())
    MpcoreInitDeferredCores (0);
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
    //AsciiPrint ("CPU %d : %d MHz\n", i, CpuFreq);
    AsciiPrint ("%a Freq : %d MHz\n", ClkInfo[i], CpuFreq);
  }

   AsciiPrint ("\n\nCPU Clock Frequyencies:\n");
   for (i = 0; i < MpcoreGetAvailCpuCount (); ++i)
   {

      EfiClockProtocol->GetCpuPerfLevel (EfiClockProtocol, i, &PerfLevel);
      EfiClockProtocol->GetCpuPerfLevelFrequency (EfiClockProtocol, i, PerfLevel, &CpuFreq, &Vg);
      CpuFreq = CpuFreq / 1000000;
      AsciiPrint ("\t\tCPU %d - %d MHz\n", i, CpuFreq);

   }
   AsciiPrint ("\nSwitchToHighPerfMode done!\n");
}

VOID SwitchToLowPerfMode (VOID)
{
  EFI_CLOCK_PROTOCOL *EfiClockProtocol = NULL;
  UINT32 PerfLevel, CpuFreq, i, Vg;

  // now bump up the clock
  if (gBS->LocateProtocol (&gEfiClockProtocolGuid, NULL, (VOID **)&EfiClockProtocol) != EFI_SUCCESS) 
    AsciiPrint ("Unable to get Clock protocol\n");

  for (i = 1; i < MpcoreGetAvailCpuCount (); ++i)
  {
    EfiClockProtocol->GetMinPerfLevel (EfiClockProtocol, i, &PerfLevel);
    EfiClockProtocol->SetCpuPerfLevel (EfiClockProtocol, i, PerfLevel, &CpuFreq);
    CpuFreq = CpuFreq / 1000000;
    AsciiPrint ("CPU %d : %d MHz\n", i, CpuFreq);
  }

   AsciiPrint ("\n\nCPU Clock Frequyencies:\n");
   for (i = 0; i < MpcoreGetAvailCpuCount (); ++i)
   {

      EfiClockProtocol->GetCpuPerfLevel (EfiClockProtocol, i, &PerfLevel);
      EfiClockProtocol->GetCpuPerfLevelFrequency (EfiClockProtocol, i, PerfLevel, &CpuFreq, &Vg);
      CpuFreq = CpuFreq / 1000000;
      AsciiPrint ("\t\tCPU %d - %d MHz\n", i, CpuFreq);

   }
   AsciiPrint ("\nSwitchToHighPerfMode done!\n");
}

void Run_cpt_test_setup_only(void) {
  SwitchToHighPerfMode ();
}

void Run_cpt_test_setup_and_enable_cores(void) {
  EnableAllCores ();
  SwitchToHighPerfMode ();

}

void Run_cpt_test_teardown(void) {
  SwitchToLowPerfMode ();
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
EFI_STATUS Run_cpt_tests(UINTN Argc, CHAR8 **Argv);

EFI_STATUS
EFIAPI
CPT_MpTestMain ( IN EFI_HANDLE         ImageHandle,
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
      ((AsciiStriCmp (Argv[0], "cpt_mptest") == 0) ||
       (AsciiStriCmp (Argv[0], "cpt_mptest.efi") == 0)))
  {
    --Argc;
    ++Argv;
  }

   /* CPT Test suite */
   if (Argc && (AsciiStriCmp (Argv[0], "-s") == 0) && (AsciiStriCmp (Argv[1], "cpt") == 0) )
   {
      if (Argc && (AsciiStriCmp (Argv[2], "-c") == 0) && (AsciiStriCmp (Argv[3], "setup") == 0) )
      {
         Run_cpt_test_setup_only();
      }
      else if (Argc && (AsciiStriCmp (Argv[2], "-c") == 0) && (AsciiStriCmp (Argv[3], "en_cores") == 0) )
      {
         Run_cpt_test_setup_and_enable_cores();
      }
      else if (Argc && (AsciiStriCmp (Argv[2], "-c") == 0) && (AsciiStriCmp (Argv[3], "teardown") == 0) ) {
         Run_cpt_test_teardown();
      }
      else {
         Run_cpt_tests(Argc, Argv);
      }
     
      AsciiPrint ("\r\nPress any key to exit...\r\n");
      ReadAnyKey (NULL, 0);

      AsciiPrint ("\r\nExiting...\r\n");
      return Status;

   }

  return Status;
}

