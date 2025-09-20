/** @file CprhDxe.c

  This file implements CPRH EFI protocol interface.

  Copyright (c) 2017-2018, Qualcomm Technologies, Inc. All rights 
  reserved. 

**/

/*=========================================================================
      Include Files
==========================================================================*/

#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/EFIDALSYSProtocol.h>
#include <Protocol/EFIDisplayPwr.h>
#include <Protocol/EFILimits.h>
#include <Library/BaseMemoryLib.h>
#include <Library/QcomLib.h>
#include <Library/QcomBaseLib.h>
#include <Library/CPR.h>
#include <Library/DebugLib.h>

/*=========================================================================
      Globals and prototypes
==========================================================================*/
/* Dependencies for CPR */
boolean DispPwrInit                         = FALSE;
boolean LimitsInit                          = FALSE;

/* Interface for Display Protocol status access */
EFI_DISPLAY_POWER_PROTOCOL *DispPwrProto = NULL;
VOID *DisplayPowerNotificationToken = NULL;
EFI_EVENT DisplayPowerNotificationEvent = (EFI_EVENT)NULL;

/* Interface for Limits Protocol status access */
EFI_LIMITS_PROTOCOL *LimitsProto            = NULL;
VOID *LimitsNotificationToken               = NULL;
EFI_EVENT LimitsNotificationEvent           = (EFI_EVENT)NULL;

VOID
LimitsInstalledNotification(
    IN EFI_EVENT   Event,
    IN VOID        *Context
    )
{
    EFI_STATUS              Status;
    Status = gBS->LocateProtocol(&gEfiLimitsProtocolGuid,
                                  NULL,
                                  (VOID**) &LimitsProto);
    ASSERT_EFI_ERROR(Status);

    LimitsInit = TRUE;

    if (LimitsInit && DispPwrInit)
        cpr_init();
}

VOID
DisplayPowerInstalledNotification(
    IN EFI_EVENT   Event,
    IN VOID        *Context
    )
{
    EFI_STATUS              Status;

    Status = gBS->LocateProtocol(&gEfiDisplayPowerStateProtocolGuid,
                                  NULL,
                                  (VOID**) &DispPwrProto);
    ASSERT_EFI_ERROR(Status);

    DispPwrInit = TRUE;

    if (LimitsInit && DispPwrInit)
        cpr_init();
}


/**
 * Entry point for the CPR DXE driver.
 */
EFI_STATUS
EFIAPI
CPRDxeEntryPoint(
    IN EFI_HANDLE ImageHandle,
    IN EFI_SYSTEM_TABLE *SystemTable
    )
{
    EFI_STATUS  Status = EFI_SUCCESS;
    DALSYS_InitMod(NULL);

    /* Locate Limits protocol */
    Status = gBS->LocateProtocol(&gEfiLimitsProtocolGuid,
                                  NULL,
                                  (VOID**) &LimitsProto);
    if (EFI_ERROR(Status))
    {
        Status = gBS->CreateEvent(EVT_NOTIFY_SIGNAL, TPL_CALLBACK,
                                LimitsInstalledNotification,
                                NULL, &LimitsNotificationEvent);
        ASSERT_EFI_ERROR (Status);

        Status = gBS->RegisterProtocolNotify(&gEfiLimitsProtocolGuid,
                                    LimitsNotificationEvent,
                                    (VOID *)&LimitsNotificationToken);
        ASSERT_EFI_ERROR (Status);
    }
    else
    {
        LimitsInit = TRUE;
    }

    /* Locate Display Power protocol */
    Status = gBS->LocateProtocol(&gEfiDisplayPowerStateProtocolGuid,
                                  NULL,
                                  (VOID**) &DispPwrProto);
    if (EFI_ERROR(Status))
    {
        Status = gBS->CreateEvent(EVT_NOTIFY_SIGNAL, TPL_CALLBACK,
                                DisplayPowerInstalledNotification,
                                NULL, &DisplayPowerNotificationEvent);
        ASSERT_EFI_ERROR (Status);

        Status = gBS->RegisterProtocolNotify(&gEfiDisplayPowerStateProtocolGuid,
                                    DisplayPowerNotificationEvent,
                                    (VOID *)&DisplayPowerNotificationToken);
        ASSERT_EFI_ERROR (Status);
    }
    else
    {
        DispPwrInit = TRUE;
    }

    if (LimitsInit && DispPwrInit)
        cpr_init();

    return Status;
}
