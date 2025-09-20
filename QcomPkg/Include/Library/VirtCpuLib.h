/** @file
 *  
 * Copyright (c) 2017 Qualcomm Technologies, Inc. All rights reserved.
 * 
**/

/*=============================================================================
                              EDIT HISTORY


 when       who     what, where, why
 --------   ---     -----------------------------------------------------------
 06/14/17   yg      Initial version

=============================================================================*/

#ifndef __VIRT_CPU_H__
#define __VIRT_CPU_H__

#include <Uefi.h>

EFI_STATUS InitVirtCpu (EFI_HANDLE   ImageHandle, EFI_SYSTEM_TABLE   *SystemTable);
EFI_STATUS VirtCpuRegisterInterruptHandler (IN EFI_EXCEPTION_TYPE  InterruptType,
                                            IN EFI_CPU_INTERRUPT_HANDLER  InterruptHandler);
EFI_STATUS VirtCpuEnableInterrupts (VOID);
EFI_STATUS VirtCpuDisableInterrupts (VOID);
BOOLEAN VirtCpuGetInterruptState (VOID);
VOID VirtCpuSleep (VOID* Context);

#endif // __VIRT_CPU_H__
