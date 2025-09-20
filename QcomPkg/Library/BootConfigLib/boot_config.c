/** @file boot_config.c

  Get boot configuration information from Shared IMEM

  Copyright (c) 2018 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential

**/

/*=============================================================================
                              EDIT HISTORY

when         who     what, where, why
----------   ---     ----------------------------------------------------------- 
 04/02/18    kpa     Get boot device option from xbl loader via shared imem
 02/16/18    bd      Initial version

=============================================================================*/

#include <Uefi.h>
#include <Library/DebugLib.h>
#include "BootConfig.h"

/******************************************************************************
* FUNCTION      boot_from_ufs
*
* DESCRIPTION   This function returns if device boots from UFS
*
* PARAMETERS    NONE
*
* RETURN VALUE  TRUE if device boots from UFS
*
******************************************************************************/
BOOLEAN boot_from_ufs (void)
{
  FlashDeviceType Val = EFI_MAX_FLASH_TYPE;
  
  if (GetSharedImemBootOption(&Val) != EFI_SUCCESS)
    DEBUG ((EFI_D_ERROR, "ERROR: Failed to Get Shared Imem Boot Device type \n"));

  if (Val == EFI_UFS_FLASH_TYPE)
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}  

/******************************************************************************
* FUNCTION      boot_from_sdcc
*
* DESCRIPTION   This function returns if device boots from sd card
*
* PARAMETERS    NONE
*
* RETURN VALUE  TRUE if device boots from sd card
*
******************************************************************************/
BOOLEAN boot_from_sdcc (VOID)
{
  FlashDeviceType Val = EFI_MAX_FLASH_TYPE;

  if (GetSharedImemBootOption(&Val) != EFI_SUCCESS)
    DEBUG ((EFI_D_ERROR, "ERROR: Failed to Get Shared Imem Boot Device type \n"));

  if (Val == EFI_SDC_FLASH_TYPE)
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}

/******************************************************************************
* FUNCTION      boot_from_spi_nor
*
* DESCRIPTION   This function returns if device boots from SPI NOR
*
* PARAMETERS    NONE
*
* RETURN VALUE  TRUE if device boots from SPI NOR
*
******************************************************************************/
BOOLEAN boot_from_spi_nor (VOID)
{
  FlashDeviceType Val = EFI_MAX_FLASH_TYPE;
  
  if (GetSharedImemBootOption(&Val) != EFI_SUCCESS)
    DEBUG ((EFI_D_ERROR, "ERROR: Failed to Get Shared Imem Boot Device type \n"));

  if ((Val == EFI_SPI_FLASH_TYPE)||(Val == EFI_NOR_FLASH_TYPE))
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}

/******************************************************************************
* FUNCTION      boot_from_nvme
*
* DESCRIPTION   This function returns if device boots from NVMe
*
* PARAMETERS    NONE
*
* RETURN VALUE  TRUE if device boots from NVMe
*
******************************************************************************/
BOOLEAN boot_from_nvme (VOID)
{
   return boot_from_spi_nor(); 
}
