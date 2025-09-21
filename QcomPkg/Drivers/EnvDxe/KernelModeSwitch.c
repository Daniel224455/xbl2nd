/** @file  
                    KernelModeSwitch.c

  Copyright (c) 2018 Qualcomm Technologies, Inc. All rights reserved.

**/

/*=============================================================================
                              EDIT HISTORY
  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.



 when       who      what, where, why
 --------   ---      ----------------------------------------------------------
 02/06/18   ps      initial version
=============================================================================*/

#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Protocol/EFIScm.h>
#include <Protocol/EFIScmModeSwitch.h>
#include <Library/TzRuntimeLib.h>
#include <Library/UefiBootServicesTableLib.h>

#define NUMPARAMS 2
/**
  This function will trigger the SCM call to switch to 32bit mode

  @param  hlos_boot_info        Structure having user params

  @retval EFI_SUCCESS           Call is successfull.

**/ 
EFI_STATUS 
SwitchTo32bitMode (
  hlos_boot_args_t hlos_boot_info
)
{
  UINT64 Parameters[SCM_MAX_NUM_PARAMETERS] = { 0 };
  EFI_STATUS  Status;
  
  Parameters[0] =  (UINT64) &hlos_boot_info;
  Parameters[1] =  sizeof(hlos_boot_args_t);
  
  Status = TzFastcall(
    TZ_DO_MODE_SWITCH,
    TZ_DO_MODE_SWITCH_PARAM_ID,
    Parameters,
    NUMPARAMS
    );

  if(Status != EFI_SUCCESS)
  {
    DEBUG((EFI_D_ERROR, " SCM call to switch to 32-bit mode failed, Status =  (0x%x)\r\n", Status));
  }
  
  /* The above call will not return */
  return Status;
}


STATIC EFI_HANDLE  mScm_handle = NULL;
QCOM_SCM_MODE_SWITCH_PROTOCOL  gQcomScmModeSwitchProtocol = 
{
  SwitchTo32bitMode
};

/**
  This function will install the runtime protocol to switch to 32bit mode

  @param  None

  @retval EFI_SUCCESS           Call is successfull.

**/ 
EFI_STATUS
TzRegisterModeSwitchProtocol (VOID)
{
  EFI_STATUS  Status = EFI_SUCCESS;
  
  Status = gBS->InstallProtocolInterface (
                &mScm_handle,
                &gQcomScmModeSwithProtocolGuid,
                EFI_NATIVE_INTERFACE,
                &gQcomScmModeSwitchProtocol
                );
                
  if(Status != EFI_SUCCESS)
  {
    DEBUG((EFI_D_ERROR, " 32-bit mode switch protocol installation failed, Status =  (0x%x)\r\n", Status));
  }                

  return Status;
}

