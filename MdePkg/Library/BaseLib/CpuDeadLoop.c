/** @file
  Base Library CPU Functions for all architectures.

  Copyright (c) 2016 Qualcomm Technologies, Inc.  All Rights Reserved.
  Portions Copyright (c) 2006 - 2008, Intel Corporation. All rights reserved.<BR>
  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/



#include <Base.h>
#include <Library/BaseLib.h>

#if defined (QCOM_EDK2_PATCH)
#include <Library/QcomBaseLib.h>
#include <Library/TargetResetLib.h>
#endif
/**
  Executes an infinite loop.

  Forces the CPU to execute an infinite loop. A debugger may be used to skip
  past the loop and the code that follows the loop must execute properly. This
  implies that the infinite loop must not cause the code that follow it to be
  optimized away.

**/
VOID
EFIAPI
CpuDeadLoop (
  VOID
  )
{
  volatile UINTN  Index;

#if defined (QCOM_EDK2_PATCH)
  if (RETAIL)
  {
    TargetPanic();
  }
#endif

  for (Index = 0; Index == 0;);
}

#if defined (QCOM_EDK2_PATCH)
VOID
EFIAPI
CpuBreakpoint (
  VOID
  )
{
  CpuDeadLoop();
}
#endif
