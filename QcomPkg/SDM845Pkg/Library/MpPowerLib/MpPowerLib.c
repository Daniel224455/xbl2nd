/** @file MpPowerLib.c

  This file implements specialized chip specific code for Multicore
  protocol

  Copyright (c) 2015-17, Qualcomm Technologies, Inc. All rights reserved.

**/

/*=============================================================================
                              EDIT HISTORY

  $Header: //components/rel/boot.xf/2.2/QcomPkg/SDM845Pkg/Library/MpPowerLib/MpPowerLib.c#1 $
  $DateTime: 2017/10/13 04:47:49 $
  $Author: pwbldsvc $

=============================================================================*/


/*=========================================================================
      Include Files
==========================================================================*/
#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/EFIPmicVreg.h>
#include <Library/IoLib.h>
#include <Library/MpPowerLib.h>
#include <Protocol/EFIPlatformInfo.h>

#define APC1_NOM_VOLTAGE 828

/**
  Chipset-specific initialization.
  @return - EFI_SUCCESS on success, otherwise error status
*/
EFI_STATUS MpPowerInit (VOID)
{
  return EFI_SUCCESS;
}

/**
  Chipset-specific multicore enablement

  @param[in]  CoreNum - number of cores
  @return - EFI_SUCCESS on success, otherwise error status
*/
EFI_STATUS MpPower (UINTN  CoreNum)
{
  EFI_QCOM_PMIC_VREG_PROTOCOL  *PmicVRegProtocol = NULL;
  EFI_PLATFORMINFO_PROTOCOL  *PlatformInfoProtocol = NULL;
  EFI_PLATFORMINFO_PLATFORM_INFO_TYPE  PlatformInfo = {0};
  EFI_STATUS Status = EFI_SUCCESS;
  EFI_PM_VREG_STATUS_TYPE VregStatus;
  INT32 RetryCnt = 200; /* Only give PMIC 2ms to adjust otherwise HW error */

  /* Acquire PMIC Vreg EFI Protocol handle */
  Status = gBS->LocateProtocol( &gQcomPmicVregProtocolGuid,
                                NULL, (VOID **)&PmicVRegProtocol );
  if (EFI_ERROR(Status))
  {
    return EFI_NOT_FOUND;
  }

  /* Acquire Platform info Protocol handle */
  Status = gBS->LocateProtocol ( &gEfiPlatformInfoProtocolGuid, NULL,
                                (VOID **) &PlatformInfoProtocol);
  if (EFI_ERROR(Status))
  {
    return EFI_NOT_FOUND;
  }

  Status = PlatformInfoProtocol->GetPlatformInfo(PlatformInfoProtocol,
                                                 &PlatformInfo);
  if (EFI_ERROR(Status))
  {
    return Status;
  }

  /* Set Gold Cluster Rail to Nominal */
  Status = PmicVRegProtocol->VregSetLevel(0, EFI_PM_SMPS_12, APC1_NOM_VOLTAGE);
  if (EFI_ERROR(Status))
  {
    return Status;
  }

  /* Enable Gold Cluster Rail */
  Status = PmicVRegProtocol->VregControl(0, EFI_PM_SMPS_12, TRUE);
  if (EFI_ERROR(Status))
  {
    return Status;
  }

  /* Return immediatly if this is a Emulation Platform */
  if ((PlatformInfo.platform ==  EFI_PLATFORMINFO_TYPE_RUMI) ||
      (PlatformInfo.platform ==  EFI_PLATFORMINFO_TYPE_VIRTIO))
  {
    return EFI_SUCCESS;
  }

  /* Wait for rail to come up */
  do
  {
    if (RetryCnt-- <= 0)
    {
      ASSERT(0);
    }

    gBS->Stall(10);
    Status = PmicVRegProtocol->VregGetStatus(0, EFI_PM_SMPS_12,  &VregStatus);
    if (EFI_ERROR(Status))
    {
      return Status;
    }
  } while (!(VregStatus.VregOk));

  return EFI_SUCCESS;
}
