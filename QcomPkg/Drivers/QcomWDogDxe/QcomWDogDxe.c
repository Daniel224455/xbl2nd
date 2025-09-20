/*===============================================================================
  Copyright (c) 2018 Qualcomm Technologies, Inc.  All Rights Reserved.

  FILE:         QcomWDogDxe.c
  DESCRIPTION:

  REVISION HISTORY
  when       who         what, where, why
  --------   ---         --------------------------------------------------------
  04/13/18   yw         initial version

================================================================================*/

#include <Protocol/EFIQcomWDog.h>
#include <UefiInfoBlk.h>
#include <Library/PcdLib.h>
#include <Library/QcomLib.h>
#include <Library/UefiLib.h>
#include <Library/KernelLib.h>
#include <Library/QcomBaseLib.h>
#include <Library/TargetResetLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Protocol/StatusCode.h>
#include <Protocol/ReportStatusCodeHandler.h>

#define WDOG_TIMEOUT_IN_SECS                25 // in sec
#define WDOG_PET_TIME_IN_SECS                2 // in sec

#define SEC_TO_MSEC(x)   ((x) * 1000)

STATIC     EFI_EVENT  ExitBootServicesEvent;

STATIC     EFI_RSC_HANDLER_PROTOCOL *RscHandlerProtocol = NULL;
BOOLEAN    gRscHandlerRegisterd = FALSE;

/* wdog status */
BOOLEAN    gWdogEnabled = FALSE;

/* wdog bite timeout in sec */
UINT32     gWdogBiteTimeout = WDOG_TIMEOUT_IN_SECS;

STATIC EFI_STATUS QcomWdogDisable (VOID);

/********************************************************************************
 *    Internal helper functions
********************************************************************************/

volatile UINTN RscNotificationCnt = 0;
EFI_STATUS EFIAPI 
WdogAssertHandler ( IN EFI_STATUS_CODE_TYPE     CodeType,
                    IN EFI_STATUS_CODE_VALUE    Value,
                    IN UINT32                   Instance,
                    IN EFI_GUID                 *CallerId,
                    IN EFI_STATUS_CODE_DATA     *Data OPTIONAL)
{
  ++RscNotificationCnt;
  if (CodeType == (EFI_ERROR_CODE | EFI_ERROR_UNRECOVERED) &&
      (Value & EFI_STATUS_CODE_CLASS_MASK) ==  EFI_SOFTWARE &&
      (Value & EFI_STATUS_CODE_OPERATION_MASK) == EFI_SW_EC_ILLEGAL_SOFTWARE_STATE)
  {
    if (!RETAIL)
    {
    /* TODO: Confirm if its correct operation to do. Trigger a Bite to cause crash dump */
      WatchDogTriggerBite ();
    }
    else
    {
      // Shutdown system
      TargetPanic ();
    }
  }

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
RegisterWdogRscHandler (VOID)
{
  EFI_STATUS Status;

  if (gRscHandlerRegisterd)
    return EFI_SUCCESS;

  /* register status code handler, to trigger reset on assertion */
  if (RscHandlerProtocol == NULL)
  {
    Status = gBS->LocateProtocol (&gEfiRscHandlerProtocolGuid,
                                  NULL,
                                  (VOID **) &RscHandlerProtocol);

    if (EFI_ERROR (Status) || (RscHandlerProtocol == NULL))
    {
      DEBUG ((EFI_D_ERROR, "Failed to locate RSC handler protocol %r\n", Status));
      return Status;
    }
  }

  /* with TPL_HIGH_LEVEL ASSERT () will directly call the RSC handler callback, otherwise
     go thourgh event signal */
  Status = RscHandlerProtocol->Register (WdogAssertHandler, TPL_HIGH_LEVEL);
  if (EFI_ERROR (Status))
  {
    DEBUG ((EFI_D_ERROR, "Failed to register assertion handler %r\n", Status));
    return Status;
  }

  gRscHandlerRegisterd = TRUE;
  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
UnregisterWdogRscHandler (VOID)
{
  EFI_STATUS Status;

  if (!gRscHandlerRegisterd)
    return EFI_SUCCESS;

  if (RscHandlerProtocol == NULL)
  {
    Status = gBS->LocateProtocol (&gEfiRscHandlerProtocolGuid,
                                  NULL,
                                  (VOID **) &RscHandlerProtocol);

    if (EFI_ERROR (Status) || (RscHandlerProtocol == NULL))
    {
      DEBUG ((EFI_D_ERROR, "Failed to locate RSC handler protocol %r\n", Status));
      return Status;
    }
  }

  Status = RscHandlerProtocol->Unregister (WdogAssertHandler);
  if (EFI_ERROR (Status))
  {
    DEBUG ((EFI_D_ERROR, "Failed to register assertion handler %r\n", Status));
    return Status;
  }

  gRscHandlerRegisterd = FALSE;
  return EFI_SUCCESS;
}

STATIC
VOID
DisableWdogAndPetEBSCb (EFI_EVENT  Event, VOID *Context)
{
  QcomWdogDisable ();
}


/********************************************************************************
 *    Public protocol functions
 ********************************************************************************/
STATIC
EFI_STATUS
QcomWdogEnable (VOID)
{
  EFI_STATUS Status;

  Status = RegisterWdogRscHandler ();
  if (EFI_ERROR (Status))
  {
    DEBUG ((EFI_D_ERROR, "Failed to register RSC handler\n"));
    return Status;
  }

  Status = WatchDogSetBiteTimeout (SEC_TO_MSEC (gWdogBiteTimeout));

  Status = WatchDogEnable ();
  
  if (Status == EFI_SUCCESS)
    gWdogEnabled = TRUE;
  else
    Status = UnregisterWdogRscHandler ();
  
  return Status;
}

STATIC
EFI_STATUS
QcomWdogDisable (VOID)
{
  EFI_STATUS Status;

  if (!gWdogEnabled)
     return EFI_SUCCESS;

  WatchDogDisable ();

  gWdogEnabled = FALSE;
  Status = UnregisterWdogRscHandler ();
  if (EFI_ERROR (Status))
  {
    DEBUG ((EFI_D_ERROR, "Failed to unregister RSC handler\n"));
    return Status;
  }

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
QcomWdogSetBiteTimeout (IN UINT32 TimeOutSec)
{
  EFI_STATUS Status = WatchDogSetBiteTimeout (SEC_TO_MSEC (TimeOutSec));

  if (Status == EFI_SUCCESS)
    gWdogBiteTimeout = TimeOutSec;

  return Status;
}

STATIC
VOID
QcomWdogPetWDog (VOID)
{
  return WatchDogPet ();
}

STATIC
EFI_STATUS
QcomWdogTriggerBite (VOID)
{
  WatchDogTriggerBite ();

  return EFI_NOT_READY;
}

STATIC
EFI_STATUS
QcomWdogSetPetTimerPeriod (IN UINT32 PeriodInSec)
{
  return WatchDogSetPetTimerPeriod (SEC_TO_MSEC(PeriodInSec));
}

/**
  Qcom WatchDog protocol definition

**/

STATIC EFI_QCOM_WATCHDOG_PROTOCOL QcomWatchDogProtocol =
{
  .Version            = EFI_QCOM_WATCHDOG_PROTOCOL_VERSION,
  .Enable             = QcomWdogEnable,
  .Disable            = QcomWdogDisable,
  .SetBiteTimeout     = QcomWdogSetBiteTimeout,
  .ForceWDogPet       = QcomWdogPetWDog,
  .ForceWDogBite      = QcomWdogTriggerBite,
  .SetPetTimerPeriod  = QcomWdogSetPetTimerPeriod,
};


/**
  The Entry Point for watchdog module.

  @param[in] ImageHandle    The firmware allocated handle for the EFI image.
  @param[in] SystemTable    A pointer to the EFI System Table.

  @retval EFI_SUCCESS       The entry point is executed successfully.
  @retval other             Some error occurred when executing this entry point.

**/

EFI_STATUS EFIAPI
WDogDxeEntry
(
  IN EFI_HANDLE ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
)
{
  EFI_HANDLE         Handle = NULL;
  EFI_STATUS         Status;
  EFI_STATUS         ErrStatus;
  UefiInfoBlkType   *InfoBlkPtr;
  BOOLEAN            EnableHWWdogVar = FALSE;
  UINTN              VarSize = sizeof (EnableHWWdogVar);
  BOOLEAN            EnableHwWdog;
  CHAR8*             WdogCfgStr = "PCD";
  CHAR8*             WdogEnDisStr = "Disabled";

  InfoBlkPtr = (UefiInfoBlkType*)GetInfoBlkPtr ();
  if (InfoBlkPtr == NULL)
  {
    DEBUG ((EFI_D_ERROR, "InfoBlkPtr is invalid\n"));
    return EFI_UNSUPPORTED;
  }

  EnableHwWdog = FALSE;

  do{

    if (InfoBlkPtr->ForceDisableHWWdog)
    {
      WdogCfgStr = "T32 Debug";
      break;
    }

    /* if the variable ForceEnableHWWdog exists, do what the variable tells.
       assume variabledxe is loaded already */
    Status = gRT->GetVariable (L"ForceEnableHWWdog", &gQcomTokenSpaceGuid,
                               NULL, &VarSize, (UINT8*)&EnableHWWdogVar);

    if (Status == EFI_SUCCESS)
    {
      WdogCfgStr = "Variable";

      if (EnableHWWdogVar)
        EnableHwWdog = TRUE;

      break;
    }

    /* no wdog variable is set, look for ForceEnableHWWdog in PCD */
    if (PcdGetBool (ForceEnableHWWdog) == TRUE)
      EnableHwWdog = TRUE;

  } while (0);


  if (EnableHwWdog)
    WdogEnDisStr = "Enabled";

  DEBUG ((EFI_D_ERROR, "HW Wdog Setting from %a : %a\n", WdogCfgStr, WdogEnDisStr));

  if (EnableHwWdog)
  {
    Status = QcomWdogEnable ();

    if (EFI_ERROR (Status))
    {
      DEBUG ((EFI_D_ERROR, "Failed to Enable Wdog\n"));
      goto Wdog_Error;
    }
  }

  /* set callback to disable wdog before jump to hlos.
  need this CB registered whatever wdog and pet started or not
  as long as driver is not unloaded*/
  Status = gBS->CreateEventEx (EVT_NOTIFY_SIGNAL,
                               TPL_CALLBACK,
                               DisableWdogAndPetEBSCb,
                               NULL,
                               &gEfiEventExitBootServicesGuid,
                               &ExitBootServicesEvent);
  if (EFI_ERROR (Status))
  {
    DEBUG ((EFI_D_ERROR, "Failed to register ExitBS callback\n"));
    goto Wdog_Error;
  }

  Status = gBS->InstallMultipleProtocolInterfaces (&Handle,
                                                   &gEfiQcomWDogProtocolGuid,
                                                   (VOID **)&QcomWatchDogProtocol,
                                                   NULL);
  if (EFI_ERROR (Status))
  {
    DEBUG ((EFI_D_WARN, "WDog protocol install Failed\n"));
    goto Wdog_Error;
  }

  if (gWdogEnabled)
    DEBUG ((EFI_D_WARN, "HW Wdog Enabled\n"));

  return EFI_SUCCESS;

Wdog_Error:

  if (ExitBootServicesEvent != NULL)
  {
    ErrStatus = gBS->CloseEvent (ExitBootServicesEvent);
    if (ErrStatus == EFI_SUCCESS)
      ExitBootServicesEvent = NULL;
  }

  QcomWdogDisable ();
  return EFI_DEVICE_ERROR;
}

