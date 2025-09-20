/** @file BootConfigSec.c

  Get boot configuration information from Shared IMEM for SEC

  Copyright (c) 2018 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential

**/

/*=============================================================================
                              EDIT HISTORY

when         who     what, where, why
----------   ---     ----------------------------------------------------------- 
 04/24/18    kpa     Initial version

=============================================================================*/

#include <Uefi.h>
#include "BootConfig.h"

/******************************************************************************
* FUNCTION      boot_from_emmc
*
* DESCRIPTION   This function returns if device boots from eMMC
*
* PARAMETERS    NONE
*
* RETURN VALUE  TRUE if device boots from eMMC
*
******************************************************************************/
BOOLEAN boot_from_emmc (VOID)
{
  FlashDeviceType Val = EFI_MAX_FLASH_TYPE;
  
  if (GetSharedImemBootOption(&Val) != EFI_SUCCESS)
    DEBUG ((EFI_D_ERROR, "ERROR: Failed to Get Shared Imem Boot Device type \n"));

  if (Val == EFI_MMC_FLASH_TYPE)
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}
