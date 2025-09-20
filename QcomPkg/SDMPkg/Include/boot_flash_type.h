#ifndef BOOT_FLASH_TYPE_H
#define BOOT_FLASH_TYPE_H

/*===========================================================================

                                Boot Flash Type
                                Header File

GENERAL DESCRIPTION
  This header file contains definitions for Boot's Flash type shared
  shared between bootrom, XBL loader and Core (UEFI) subsystems   

Copyright 2018 by Qualcomm Technologies Incorporated.  All Rights Reserved.
============================================================================*/

/*===========================================================================

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

when       who          what, where, why
--------   --------     ------------------------------------------------------
04/02/18   kpa           Initial creation
============================================================================*/

/*=============================================================================

                            INCLUDE FILES FOR MODULE

=============================================================================*/
//#include "boot_comdef.h"

/*=============================================================================

            LOCAL DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, types,
variables and other items needed by this module.

=============================================================================*/

/* Define the FLASH types.
 
   Based on the boot option that is configured, this field
   will indicate the exact boot media that has been used to
   successfully load the BL image.
 
   If control gets transferred to BL then the field will
   not have NO_FLASH*/
typedef enum
{
  NO_FLASH        = 0,
  NOR_FLASH       = 1,
  NAND_FLASH      = 2,
  ONENAND_FLASH   = 3,
  SDC_FLASH       = 4,
  MMC_FLASH       = 5,
  SPI_FLASH       = 6,
  PCIE_FLASH      = 7,
  UFS_FLASH       = 8,
  RESERVED_0_FLASH= 9,
  RESERVED_1_FLASH= 10,
  USB_FLASH       =11,           /* this is not a real flash type, it's used only for sw_type */
  SIZE_FLASH_TYPE = 0x7FFFFFFF    /* Force 4 byte alignment */
} boot_flash_type;

#endif /* BOOT_FLASH_TYPE_H */

