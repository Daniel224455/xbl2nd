/** @file BootConfigBds.c

  Get boot configuration information from Shared IMEM for BDS

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
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>

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
  EFI_STATUS Status;
  VOID  *Ptr =0;
   
  // Workaround to support SD boot. If booting from SD card
  // installed with emmc block io handles this api returns true  
  Status = gBS->LocateProtocol (
                  &gEfiSDBootOptionGuid,
                  NULL,
                  (VOID **) &Ptr
                  );
                  
  if(Status == EFI_SUCCESS)
  {
    // Since protocol is located, SD boot driver is installed.
    return TRUE;
  }  
 
  // If SD boot driver is not installed, check usual bootup option
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
