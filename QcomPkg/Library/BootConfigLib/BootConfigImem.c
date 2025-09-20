/** @file BootConfigImem.c

  Get boot configuration information from Shared IMEM

  Copyright (c) 2018 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential

**/

/*=============================================================================
                              EDIT HISTORY

when         who     what, where, why
----------   ---     ----------------------------------------------------------- 
 04/02/18    kpa     Initial version

=============================================================================*/

#include <Uefi.h>
#include <Library/UefiCfgLib.h>
#include "boot_shared_imem_cookie.h"
#include "boot_flash_type.h"
#include <Protocol/EFIBootConfig.h>
#include "BootConfigHwio.h"

/******************************************************************************
* FUNCTION      GetFlashDeviceType
*
* DESCRIPTION   This function maps boot device option passed by loader to  
*               corresponding UEFI boot device type.
*
* PARAMETERS    ImemBootFlashType Input loader boot option to be mapped
*               UefiFlashDevice   Output UEFI boot device option.
*
* RETURN VALUE  EFI_SUCCESS if success
*
******************************************************************************/
EFI_STATUS GetFlashDeviceType (boot_flash_type *ImemBootFlashType, FlashDeviceType *UefiFlashDevice)
{
  if (UefiFlashDevice == NULL || ImemBootFlashType == NULL )
  {
    return EFI_INVALID_PARAMETER;
  }
  *UefiFlashDevice = EFI_MAX_FLASH_TYPE;
  
  switch(*ImemBootFlashType)
  {
    case NO_FLASH:          *UefiFlashDevice = EFI_NO_FLASH_TYPE;         break;
    case NOR_FLASH:         *UefiFlashDevice = EFI_NOR_FLASH_TYPE;        break;
    case NAND_FLASH:        *UefiFlashDevice = EFI_NAND_FLASH_TYPE;       break;
    case ONENAND_FLASH:     *UefiFlashDevice = EFI_ONENAND_FLASH_TYPE;    break;
    case SDC_FLASH:         *UefiFlashDevice = EFI_SDC_FLASH_TYPE;        break;        
    case MMC_FLASH:         *UefiFlashDevice = EFI_MMC_FLASH_TYPE;        break;        
    case SPI_FLASH:         *UefiFlashDevice = EFI_SPI_FLASH_TYPE;        break;        
    case PCIE_FLASH:        *UefiFlashDevice = EFI_PCIE_FLASH_TYPE;       break;        
    case UFS_FLASH:         *UefiFlashDevice = EFI_UFS_FLASH_TYPE;        break;        
    case RESERVED_0_FLASH:  *UefiFlashDevice = EFI_RESERVED_0_FLASH_TYPE; break;
    case RESERVED_1_FLASH:  *UefiFlashDevice = EFI_RESERVED_1_FLASH_TYPE; break;
    case USB_FLASH:         *UefiFlashDevice = EFI_USB_FLASH_TYPE;        break;        
    case SIZE_FLASH_TYPE:   *UefiFlashDevice = EFI_MAX_FLASH_TYPE;        break;        
 
    default:       break;
  }
  if(*UefiFlashDevice == EFI_MAX_FLASH_TYPE)
  {
    return EFI_UNSUPPORTED;
  }

  return EFI_SUCCESS;
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
  EFI_STATUS Status = EFI_SUCCESS;
  UINT32 SharedIMEMBaseAddr = 0, OverRideSharedImemBootCfg = 0;
  struct boot_shared_imem_cookie_type *BootSharedImemPtr = NULL;
  boot_flash_type Val = SIZE_FLASH_TYPE;
  
  if (BootDeviceOption == NULL)
  {
    return EFI_INVALID_PARAMETER;
  }
  
  /* Get SharedIMEMBaseAddr value */
  Status = GetConfigValue ("SharedIMEMBaseAddr", (UINT32 *)&SharedIMEMBaseAddr); 
  if ((Status != EFI_SUCCESS) || (SharedIMEMBaseAddr == 0))
  {
    return EFI_NOT_FOUND;
  }
  
  BootSharedImemPtr = (struct boot_shared_imem_cookie_type *)(UINTN)SharedIMEMBaseAddr;
  
  /*Ensure SharedImem is initialized */
  if ((BootSharedImemPtr->shared_imem_magic != BOOT_SHARED_IMEM_MAGIC_NUM) ||
      (BootSharedImemPtr->shared_imem_version < BOOT_SHARED_IMEM_VERSION_NUM)
     )
  {
    return EFI_PROTOCOL_ERROR;
  }  
  

  Val = BootSharedImemPtr->boot_device_type;
  Status = GetFlashDeviceType(&Val, BootDeviceOption);
  
  if ((Status != EFI_SUCCESS))
  {
    return Status;
  }  
  
  // Get OverRideSharedImemBootCfg value
  Status = GetConfigValue ("OverRideSharedImemBootCfg", (UINT32 *)&OverRideSharedImemBootCfg); 
  if (Status == EFI_SUCCESS)
  {
    //Check Range
    if(OverRideSharedImemBootCfg >= EFI_NO_FLASH_TYPE && 
       OverRideSharedImemBootCfg <= EFI_USB_FLASH_TYPE) 
    {
      *BootDeviceOption = OverRideSharedImemBootCfg;
    }
    else
    {
      Status = EFI_INVALID_PARAMETER;
    }
  }
  return EFI_SUCCESS;
}  
