/** @file boot_config_845.c

  BootConfigLib is used to get boot configuration information

  Copyright (c) 2017 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential

**/

#include <Uefi.h>
#include "HALhwio.h"
#include "BootConfig.h"

#define BOOT_CONFIG_REG             0x00786070
#define BOOT_DEVICE_MASK(val)       (((val) & 0x3E) >> 1)

#define BOOT_CONFIG_DEFAULT         0x0
#define BOOT_CONFIG_SD_UFS_EDL      0x1
#define BOOT_CONFIG_SD_EDL          0x2
#define BOOT_CONFIG_USB_EDL         0x3
#define BOOT_CONFIG_QSPI_EDL        0x4
#define BOOT_CONFIG_SPI_EDL         0x5
#define BOOT_CONFIG_UFS_SD_USB_EDL  0x8

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 *  Need SBL to share the boot device selected by PBL
 *  (pbl_sbl_shared.h --> boot_flash_type)
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 */


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
   return TRUE;
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
   return FALSE;
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
