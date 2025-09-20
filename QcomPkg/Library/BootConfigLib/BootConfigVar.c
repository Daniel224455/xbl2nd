/** @file BootConfigVar.c

  Get boot configuration information from Shared IMEM 
  via runtime services

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
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Protocol/EFIBootConfig.h>

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
  UINT32 Val = 0;
  UINTN  DataSize = 0;
  EFI_STATUS Status = EFI_SUCCESS;

  if (BootDeviceOption == NULL)
  {
    return EFI_INVALID_PARAMETER;
  }
  
  DataSize = sizeof(Val);    
  Status = gRT->GetVariable (L"SharedImemBootCfgVal",
                           &gQcomTokenSpaceGuid,
                           NULL,
                           &DataSize,
                           &Val);
                           
  if (Status != EFI_SUCCESS)
    DEBUG ((EFI_D_ERROR, "ERROR: Failed to Get Variable SharedImemBootCfgVal: 0x%08x\n", Status));
  
  *BootDeviceOption = Val;
  
  return Status;
}  

