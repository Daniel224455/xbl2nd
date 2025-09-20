/** @file TargetInitLib.c
 
  @brief Target Specific Initialization Functions
 
  Copyright (c) 2013,2017 - 2018 by Qualcomm Technologies, Inc. All Rights Reserved.

**/ 
 
/*=======================================================================
                        Edit History
  when       who     what, where, why
  --------   ----    ---------------------------------------------------
  02/27/18   yg      Add TargetConfig Params API
  02/27/17   vk      Remove unused
  11/15/13   vk      Disable DebugDisableFuse and Crash check
  03/04/13   niting  Add check for HLOS crash cookied address based on HW version
  01/28/13   vishalo Initial Version

========================================================================*/

#include <Uefi.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include "Library/TargetInitLib.h"

TargetConfigType SOCConfigParams = 
{
  /* How many bits to Consider for masking in order to get the core# from MPIDR 
   * register value Check MPIDR registers for 2 aux cores to determine this
   * Usually affinity0 or affinity 1 */
  .MpidrToCpuNumBits = 16,

  /* How many bits to shift in order to get the core# from MPIDR register value
   * Check MPIDR registers for 2 aux cores to determine this */
  .MpidrToCpuNumShift = 8,

  /* How many bits to shift to get the cluster number from MPIDR register */
  .MpidrToClustNumShift = 8,
                                         
  /* SGI interrupt base # where 2 interrupts are available for Scheduler's usage
   * Should be accessible from NS side. 
   * If x, x+1 are available, value here would be x */
  .IpiSgiIntrBaseNum = 6,

  /* CP15 timer Interrupt number */
  .ArchTimerInterruptNum = 19,
                                         
  /* GIC params */
  .GicDistributorOffset = 0x17A00000,
  .GicRedistributorOffset = 0x17A60000,
  .GicRedistributorStride  = 0x20000,

  .TimerFrameBaseAddr  = 0x17C21000,
  .TimerFrameIntrNum  = 40,
};

VOID
TargetEarlyInit (VOID)
{
}

VOID
TargetLateInit (VOID)
{
  return;
}

TargetConfigType*
GetTargetConfigParams (VOID)
{
  return &SOCConfigParams;
}

