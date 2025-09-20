/**
  @file TargetResetLib.c
 
  @brief Target Specific Reset Functions
 
  Copyright (c) 2013,2018 by Qualcomm Technologies, Inc. All Rights Reserved.
 
**/

/*=======================================================================
                        Edit History

when       who     what, where, why
--------   ----    ---------------------------------------------------
11/09/18   kpa     TargetPanic to loop directly instead of calling CpuDeadLoop. 
10/01/13   vk      Shutdown on ASSERT
06/17/13   vishalo Initial version
========================================================================*/

#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/IoLib.h>
#include <Library/PcdLib.h>

/**
  Precondition: PmicShutdownBootLib has been called to set target reset behavior. Currently, this is done SEC.
**/
VOID
TargetPanic (VOID)
{
  volatile UINTN  Index;
  
  /* This register controls the PSHOLD value. 
     1: Asserted
     0: Deasserted */
  MmioWrite32 (PcdGet32 (PcdPsHoldAddress), 0);

  /* Loop here until reset is complete */
  for (Index = 0; Index == 0;);
}

