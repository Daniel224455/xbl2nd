/** @file
  Virtual CPU abstraction for Timer/Exception/Interrupts of ARM CPU

  Copyright (c) 2014, 2016 Qualcomm Technologies, Inc. All rights reserved.
  Portions Copyright (c) 2011-2013 ARM Ltd. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/


#include <PiDxe.h>

#include <Library/ArmLib.h>

#include <Protocol/Timer.h>
#include <Protocol/HardwareInterrupt.h>
#include <Protocol/Cpu.h>
#include <Library/VirtCpuLib.h>

EFI_STATUS InitVirtCpu (EFI_HANDLE  ImageHandle, EFI_SYSTEM_TABLE   *SystemTable)
{
  return EFI_UNSUPPORTED;  
}

EFI_STATUS
VirtCpuRegisterInterruptHandler (IN EFI_EXCEPTION_TYPE             InterruptType,
                                 IN EFI_CPU_INTERRUPT_HANDLER      InterruptHandler)
{
  return EFI_UNSUPPORTED;
}

EFI_STATUS
VirtCpuEnableInterrupts (VOID)
{
  return EFI_UNSUPPORTED;  
}

EFI_STATUS
VirtCpuDisableInterrupts (VOID)
{
  return EFI_UNSUPPORTED;  
}

BOOLEAN VirtCpuGetInterruptState(VOID)
{
  return 0;
}

VOID
VirtCpuSleep (VOID* Context)
{

}

