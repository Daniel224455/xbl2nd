/** @file
 * Lib interface for Multicore WDog implementation
 *  
 * Copyright (c) 2019 Qualcomm Technologies Inc. All rights reserved.
 *
**/

/*=============================================================================
                              EDIT HISTORY

 when       who     what, where, why
 --------   ---     -----------------------------------------------------------
 03/11/19   yg      Initial version

=============================================================================*/

#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/BaseLib.h>
#include <PiDxe.h>
#include <Library/HobLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/EFIKernelInterface.h>
#include <Library/KernelLib.h>

static WDogIntf             *gKernWdogIntf = NULL;

/********************************************************************************
      WatchDog
********************************************************************************/
EFI_STATUS WatchDogEnable (VOID)
{
  if (gKernWdogIntf)
    return gKernWdogIntf->WdogEnable ();

  return EFI_UNSUPPORTED;
}

VOID WatchDogDisable (VOID)
{
  if (gKernWdogIntf)
    gKernWdogIntf->WdogDisable ();
}

EFI_STATUS WatchDogSetBiteTimeout (UINT32 Timeoutms)
{
  if (gKernWdogIntf)
    return gKernWdogIntf->WdogSetBiteTimeout (Timeoutms);

  return EFI_UNSUPPORTED;
}

VOID WatchDogForcePet (VOID)
{
  if (gKernWdogIntf)
    gKernWdogIntf->WdogForcePet ();
}

VOID WatchDogForceBite (VOID)
{
  if (gKernWdogIntf)
    gKernWdogIntf->WdogFirceBite ();
}

EFI_STATUS WatchDogSetPetTimerPeriod (UINT32 TimerPeriodms)
{
  if (gKernWdogIntf)
    return gKernWdogIntf->WdogSetPetTimerPeriod (TimerPeriodms);

  return EFI_UNSUPPORTED;
}

EFI_STATUS
EFIAPI
KernWDogLibInit (VOID)
{
  EFI_KERNEL_PROTOCOL  *KernIntf = NULL;
  
  if (gKernWdogIntf != NULL)
    return EFI_SUCCESS;

  KernIntf = GetKernelIntfPtr ();

  /* Constructor function returning failure will result into a crash */
  if (KernIntf == NULL)
    return EFI_NOT_FOUND;

  gKernWdogIntf = KernIntf->WDog;

  return EFI_SUCCESS;
}

