/** @file WatchdogPetTest.c
   
  Test application for Watchdog timer pet.

  Copyright (c) 2017-2018, Qualcomm Technologies, Inc. All rights reserved.
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
 07/09/18   ps      gEfiQcomWDogProtocolGuid added to avoid target dependency
 07/25/17   c_mku   Initial revision to demonstrate watchdog pet.

=============================================================================*/

#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/TestInterface.h>
#include <Library/WatchdogLib.h>
#include <Protocol/EFIQcomWDog.h>

#define SEC_TO_MICROSEC(x)        MultU64x32((UINT64)(x), 1000000)
#define PET_TIMER_SEC             2
#define BITE_TIMER_SEC            3
#define FORCE_BITE_PET_TIMER_SEC  (BITE_TIMER_SEC * 3)

STATIC EFI_QCOM_WATCHDOG_PROTOCOL      *WatchDogProtocol = NULL;
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
WatchdogPetTest (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS Status = EFI_SUCCESS;

  TEST_START("WatchDogPetTest");

  /* Disable UEFI's crash dump watchdog  */
  Status = gBS->SetWatchdogTimer(0, 0, 0, NULL);
  if(Status != EFI_SUCCESS)
  {
    goto Exit;
  }

  Status = gBS->LocateProtocol(&gEfiQcomWDogProtocolGuid,
                             NULL,
                             (VOID**)&WatchDogProtocol);
  if(EFI_ERROR(Status))
  {
    DEBUG((EFI_D_ERROR,"Locate WatchDogProtocol failed: %r \n", Status));
    goto Exit;
  }

  AsciiPrint("Start : Disable Watchdog\n");
  WatchDogProtocol->Disable();
  AsciiPrint("Finish : Disable Watchdog\n");

  Status = WatchDogProtocol->SetBiteTimeout(BITE_TIMER_SEC);
  if(EFI_ERROR(Status))
  {
    DEBUG((EFI_D_ERROR,"WatchDogProtocol->SetBiteTimer failed: %r \n", Status));
    goto Exit;
  }
  AsciiPrint("BiteTime Register value set to %d Seconds\n", BITE_TIMER_SEC);

  Status = WatchDogProtocol->SetPetTimerPeriod(PET_TIMER_SEC);
  if(Status != EFI_SUCCESS)
  {
    DEBUG((EFI_D_ERROR,"WatchDogProtocol->SetPetTimerPeriod failed: %r \n", Status));
    goto Exit;
  }
  AsciiPrint("PetTime Register value set to %d Seconds\n", PET_TIMER_SEC);

  AsciiPrint("Start : Enable Watchdog\n");
  WatchDogProtocol->Enable();
  AsciiPrint("Finish : Enable Watchdog\n");

  AsciiPrint("Stall for %d seconds, no crash expected\n", FORCE_BITE_PET_TIMER_SEC);
  gBS->Stall(SEC_TO_MICROSEC(FORCE_BITE_PET_TIMER_SEC));

  Status = WatchDogProtocol->SetPetTimerPeriod(FORCE_BITE_PET_TIMER_SEC);
  if(Status != EFI_SUCCESS)
  {
    goto Exit;
  }
  AsciiPrint("PetTime Register value set to %d Seconds\n", FORCE_BITE_PET_TIMER_SEC);
  AsciiPrint("Stall for %d seconds, expect crash after this!\n", FORCE_BITE_PET_TIMER_SEC);
  gBS->Stall(SEC_TO_MICROSEC(FORCE_BITE_PET_TIMER_SEC));

Exit:
  /* Should not reach here */
  TestStatus("WatchDogPetTest", EFI_DEVICE_ERROR);
  TEST_STOP("WatchDogPetTest");
  return EFI_DEVICE_ERROR;
}
