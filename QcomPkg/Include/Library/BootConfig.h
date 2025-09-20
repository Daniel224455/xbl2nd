#ifndef BOOTCONFIG_H
#define BOOTCONFIG_H

/** @file BootConfig.h
   
  This file is header file for BootConfigLib

  Copyright (c) 2015-2018, Qualcomm Technologies, Inc.
  All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential
    
**/

/*=============================================================================
                              EDIT HISTORY


when         who    what, where, why
----------   ---    --------------------------------------------------------------
2018-04-03   kpa    Added GetSharedImemBootOption
2018-02-16   bd     Add API to read from boot config register
2017-12-01   jt     Add NVMe support 
2015-04-30   as     Add SPI NOR functionality.
2015-03-31   rm     Initial version

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
BOOLEAN boot_from_ufs (void);

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
BOOLEAN boot_from_emmc (void);

/******************************************************************************
* FUNCTION      boot_from_sdcc
*
* DESCRIPTION   This function returns if device boots from sdcc
*
* PARAMETERS    NONE
*
* RETURN VALUE  TRUE if device boots from sdcc
*
******************************************************************************/
BOOLEAN boot_from_sdcc (void);

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
BOOLEAN boot_from_spi_nor (void);

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
BOOLEAN boot_from_nvme (void);


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
EFI_STATUS GetSharedImemBootOption (FlashDeviceType *BootDeviceOption);


#endif /* #ifndef BOOTCONFIG_H */

