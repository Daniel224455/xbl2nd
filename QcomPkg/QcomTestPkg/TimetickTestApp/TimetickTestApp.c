/** @file TimetickTestApp.c
   
  Tests for Timetick counter reads and match value writes

Copyright (c) 2012,2018 Qualcomm Technologies Inc. All rights reserved.
Portions Copyright (c) 2006 - 2016, Intel Corporation. All rights reserved.<BR>
This program and the accompanying materials
are licensed and made available under the terms and conditions of the BSD License
which accompanies this distribution.  The full text of the license may be found at
http://opensource.org/licenses/bsd-license.php

THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
  
**/

/*=============================================================================
                              EDIT HISTORY


 when       who     what, where, why
 --------   ---     -----------------------------------------------------------
 06/13/12  pbitra   Created test for Timetick functionality

=============================================================================*/

#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/TestInterface.h>
#include <Library/PcdLib.h>



#include "TimetickUEFI.h"

/**
  The user Entry Point for Application. The user code starts with this function
  as the real entry point for the application.

  @param[in] ImageHandle    The firmware allocated handle for the EFI image.  
  @param[in] SystemTable    A pointer to the EFI System Table.
  
  @retval EFI_SUCCESS       The entry point is executed successfully.
  @retval other             Some error occurs when executing this entry point.

**/
EFI_STATUS
EFIAPI
TimetickTestMain (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS  Status = EFI_SUCCESS;
  Timetick_time_64Type count;
  UINT32 freq;
  Timetick_time_64Type matchValue;
  Timetick_timer_Type Timer;

  TEST_START("TimetickTest");

  if(PcdGet32(PcdQTimerEnabled))
  {
    Timer = TIMETICK_QTIMER;
  }
  else
  {
    Timer = TIMETICK_TIMER_ADGT;
  }

  AsciiPrint("Calling Timetick_Init\n");
  Timetick_Init();
  Timetick_Enable(Timer, 1);

  Timetick_GetCount(Timer, &count);
  AsciiPrint("Timetick count read: %lx\n ", count);

  Timetick_GetFreq(Timer, &freq );
  AsciiPrint("Timetick freq read: %d\n", freq);

  Timetick_GetMatchValue(Timer, &matchValue);
  AsciiPrint("Timetick match value read: %lx \n", matchValue);

  Timetick_GetCount(Timer, &count);
  AsciiPrint("Timetick count read: %lx\n ", count);

  Timetick_SetMatchValue(Timer, count+0xF000, &matchValue);
  AsciiPrint("Timetick match value set to : %lx \n", matchValue);

  Timetick_GetMatchValue(Timer, &matchValue);
  AsciiPrint("Timetick match value read: %lx \n", matchValue);

  TestStatus("TimetickTest",Status);
  TEST_STOP("TimetickTest");

  return Status;
}
