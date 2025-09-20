/** @file
   
  Cause panic to test crashdump 

  Copyright (c) 2018 Qualcomm Technologies Inc. All rights reserved.

**/

/*=============================================================================
                              EDIT HISTORY


 when       who     what, where, why
 --------   ---     -----------------------------------------------------------
 9/17/18   ai      Initial version 

=============================================================================*/

#include <Uefi.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>

EFI_STATUS
EFIAPI
UefiMain (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wundefined-optimized"

  UINT32* Foo = 0x0;
  *Foo = 1;

#pragma clang diagnostic pop

  return EFI_DEVICE_ERROR;
}
