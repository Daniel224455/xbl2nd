/** @file UefiSplitBoot.c
  UEFI Split boot library stub.

  Copyright (c) 2017 - 2018 Qualcomm Technologies, Inc.  All Rights Reserved.

  Qualcomm Technologies Proprietary and Confidential.

**/

/*=============================================================================
                              EDIT HISTORY


 when       who     what, where, why
 --------   ---     -----------------------------------------------------------
 10/15/18   bd      SerialLock  changes to handle MT/MC disabled case
 06/19/17   yg      Created
 
=============================================================================*/

#include <Uefi.h>
#include <Library/ArmLib.h>
#include "Library/SplitBootLib.h"

/* This function doesn't return if succeeds, instead continues the execution
 * from BootContinue function */
EFI_STATUS InitSplitBoot (BootContinueFnPtr BootContinueFn, VOID* Arg)
{
  /* For now fall back to non split booting */
  return EFI_UNSUPPORTED;
}

VOID AuxCpuCEntry (UINT64 cpu)
{
  /* Unsupported */
}

UINTN SerialPortLock (VOID)
{  
  UINTN  State = 0;
  
  State = ArmGetInterruptState();
  ArmDisableInterrupts();  
  return State;
}

VOID SerialPortUnlock (UINTN State)
{
  if (State)
    ArmEnableInterrupts(); 
}

VOID Sleep (VOID)
{
}


