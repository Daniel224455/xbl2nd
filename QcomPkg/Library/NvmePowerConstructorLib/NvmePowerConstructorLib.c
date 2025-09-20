/**************************************************************************
 * FILE: NvmePowerConstructorLib.c
 *
 * Library to power on the NVMe rails. It is linked anonymously to NvmExpressDxe
 * by defining it in the DSC file.
 *
 * Copyright (c) 2020 Qualcomm Technologies, Inc.
 * All Rights Reserved.
 * Qualcomm Proprietary
 *
 *************************************************************************/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.


when        who   what, where, why
----------  ---   ---------------------------------------------------------
2020-01-17  wek   Create.

===========================================================================*/

#include <Uefi.h>
#include <Library/DebugLib.h>
#include <Library/BootConfig.h>
#include <NvmeBsp.h>

/**
  Constructor:

  Power on the NVME.

  @param[in] ImageHandle  Driver's image handle.
  @param[in] *SystemTable Pointer to system table.

  @return EFI_SUCCESS

**/
EFI_STATUS
EFIAPI
NvmePowerConstructor (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  DEBUG((DEBUG_VERBOSE, "%a\n", __FUNCTION__));
  if (boot_from_nvme()) 
  {
    INTN result;
    result = nvme_core_target_power_on(0);
    if (result != 0)
    {
      DEBUG ((EFI_D_ERROR, "NvmExpressDriverEntry: Failed to turn on NVMe power rail!\n"));
    }
  }

  return EFI_SUCCESS;
}