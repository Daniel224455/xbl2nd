/**
  @file TargetResetLib.c
 
  @brief Target Specific Reset Functions
 
  Copyright (c) 2013,2018 by Qualcomm Technologies, Inc. All Rights Reserved.
**/

/*=======================================================================
                        Edit History

when       who     what, where, why
--------   ----    --------------------------------------------------- 
06/17/13   vishalo Initial version
========================================================================*/

#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/IoLib.h>

/**
  Target panic
**/
VOID
TargetPanic (VOID)
{
  CpuDeadLoop ();
}

