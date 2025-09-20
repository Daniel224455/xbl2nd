/** @file boot_config.c

  BootConfigLibNull is used to get boot configuration information

  Copyright (c) 2015-2018 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential

**/

/*=============================================================================
                              EDIT HISTORY

when         who     what, where, why
----------   ---     ----------------------------------------------------------- 
2018-02-16   bd      Add API to read from boot config register
2017-12-01   jt      Update for NVMe  
2015-09-01   vk      Stub out for core
2015-06-03   plc     Added boot_from_spi_nor stub
2015-05-08   plc     Initial version

=============================================================================*/

#include <Uefi.h>
#include <Protocol/EFIBootConfig.h>

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
  return FALSE;
}  

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
  return TRUE;
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
   return FALSE;
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
   return FALSE;
}

/******************************************************************************
* FUNCTION      GetSharedImemBootOption
*
* DESCRIPTION   This function returns boot option passed on by xbl Loader 
*               via shared imem
*
* PARAMETERS    BootDeviceOption  Boot Device option shared by XBL loader
*                                 mapped to UEFI equivalent type
*
* RETURN VALUE  EFI_SUCCESS if success
*
******************************************************************************/
EFI_STATUS GetSharedImemBootOption (FlashDeviceType *BootDeviceOption)
{
  if (BootDeviceOption == NULL)
  {
    return EFI_INVALID_PARAMETER;
  }
  
  *BootDeviceOption = UFS_FLASH_TYPE;
  return EFI_SUCCESS;
}
