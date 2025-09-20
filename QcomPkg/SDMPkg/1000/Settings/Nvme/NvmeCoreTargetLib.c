/** @file NvmeCoreTargetLib.c

  Target specific configuration Items for NVMe

  Copyright (c) 2018 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential

**/

/*=============================================================================
                              EDIT HISTORY

when         who     what, where, why
----------   ---     -----------------------------------------------------------
2018-06-14   wek     Turn on NVMe power rail.

=============================================================================*/

#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Pi/PiStatusCode.h>
#include <Library/DebugLib.h>
#include <Protocol/EFIPmicGpio.h>
#include <Library/PcdLib.h>
#include <Library/QcomLib.h>
#include <Lpm.h>
#include <Protocol/ReportStatusCodeHandler.h>

#include "npa.h"
#include "pmapp_npa.h"




EFI_STATUS nvme_core_target_pmic_gpio_pwr(int power)
{
  EFI_QCOM_PMIC_GPIO_PROTOCOL   *PmicGpioProtocol = NULL;
  EFI_STATUS                    Status;
  UINT32 PmicIndex = 2;
  EFI_PM_GPIO_WHICH_TYPE GpioNum = EFI_PM_GPIO_11;
  EFI_PM_GPIO_OUT_BUFFER_CONFIG_TYPE OutBuffConfig = EFI_PM_GPIO_OUT_BUFFER_CONFIG_CMOS;
  EFI_PM_GPIO_VOLTAGE_SOURCE_TYPE VSrc = EFI_PM_GPIO_VIN1;
  EFI_PM_GPIO_SOURCE_CONFIG_TYPE Source = EFI_PM_GPIO_SOURCE_GND;
  EFI_PM_GPIO_OUT_BUFFER_DRIVE_STRENGTH_TYPE BufferStrength;
  BOOLEAN inversion = 1;

  if (power == 0)
    BufferStrength = EFI_PM_GPIO_OUT_BUFFER_OFF;
  else
    BufferStrength = EFI_PM_GPIO_OUT_BUFFER_HIGH;

  Status = gBS->LocateProtocol(&gQcomPmicGpioProtocolGuid, NULL, (VOID **)&PmicGpioProtocol);
  if (EFI_ERROR (Status))
  {
    DEBUG(( EFI_D_WARN, "%a: PMIC Locate Protocol failed\r\n", __func__));
    return Status;
  }
  Status = PmicGpioProtocol->ConfigDigitalOutput(PmicIndex, GpioNum, OutBuffConfig ,
                                                 VSrc,      Source,  BufferStrength,
                                                 inversion);
  if (EFI_ERROR (Status))
  {
    DEBUG(( EFI_D_WARN, "%a: ConfigDigitalOutput failed\r\n", __func__));
    return Status;
  }

  return Status;
}

static npa_client_handle nvme_npa_client_handle = NULL;
npa_client_handle nvme_core_get_npa_client_handle(void)
{
  npa_query_type query_result;
  npa_query_status query_status = NPA_QUERY_NO_VALUE;

  if (nvme_npa_client_handle != NULL)
    return nvme_npa_client_handle;

  /*check if client exists*/
  query_status = npa_query_by_name(PMIC_NPA_GROUP_ID_NVME, NPA_QUERY_CURRENT_STATE, &query_result);
  if (query_status == NPA_QUERY_SUCCESS)
  {
    /*get client handle*/
    nvme_npa_client_handle =  npa_create_sync_client(PMIC_NPA_GROUP_ID_NVME, "nvme", NPA_CLIENT_REQUIRED);
    if (nvme_npa_client_handle == NULL)
    {
      DEBUG((EFI_D_ERROR, "NVMeCoreTargetLib: Failed npa_create_sync_client 0x%x\r\n", query_status));
    }
  }
  else
  {
    DEBUG((EFI_D_ERROR, "NVMeCoreTargetLib: Failed npa_query_by_name 0x%x\r\n", query_status));
  }
  return nvme_npa_client_handle;
}

INTN nvme_core_target_power_off(UINT32 slot)
{
  INTN result = -1;
  EFI_STATUS status;
  npa_client_handle npa_handle = NULL;

  /* We support turning on slot 0 only. */
  if (slot != 0)
  {
    return EFI_UNSUPPORTED;
  }

  npa_handle = nvme_core_get_npa_client_handle();
  if (npa_handle != NULL)
  {
    /* turn off S8C*/
    npa_issue_required_request(npa_handle, PMIC_NPA_MODE_ID_GENERIC_OFF );

    /* Turn on the PMIC GPIO #11 to turn on NVME (DBB1 and DBU4) */
    status = nvme_core_target_pmic_gpio_pwr(0);
    if (!EFI_ERROR (status))
      result = 0;
  }

  return result;
}

/* Register the callback to be notified of a Low power mode event.
  */
EFI_STATUS EFIAPI NvmeLowPowerModeCallback0 (
   IN EFI_STATUS_CODE_TYPE     CodeType,
   IN EFI_STATUS_CODE_VALUE    Value,
   IN UINT32                   Instance,
   IN EFI_GUID                 *CallerId,
   IN EFI_STATUS_CODE_DATA     *Data OPTIONAL)
{
  EFI_STATUS Status = EFI_SUCCESS;
  LPM_EVENT_EXT_DATA *LpmEventData = NULL;

  if (((CodeType & EFI_STATUS_CODE_TYPE_MASK) == EFI_PROGRESS_CODE) &&
      (Value == PcdGet32 (PcdLpm)) && (Data != NULL))
  {
    /* Map LpmEventData */
    LpmEventData = (LPM_EVENT_EXT_DATA*)(Data + 1);

    if(TRUE == CompareGuid(&(LpmEventData->CalleeGuid), &gEfiNvme0Guid))
    {
      switch (LpmEventData->LPMState)
      {
      case LPM_ENTRY:
        if (nvme_core_target_power_off(0) != 0)
        {
          DEBUG((EFI_D_ERROR, "NvmeLowPowerModeCallback: LPM state = %d\r\n", LpmEventData->LPMState));
          Status = EFI_DEVICE_ERROR;
        }
        break;
      default:
        break;
      }
    }
  }

  return Status;
}


static EFI_RSC_HANDLER_PROTOCOL *RscHandlerProtocol = NULL;
VOID EFIAPI NvmeExitBSCallback (
   IN EFI_EVENT        Event,
   IN VOID             *Context
  )
{
   EFI_STATUS Status = EFI_SUCCESS;

   /* Unregister LPM callback in Exit Boot Services */
   if (RscHandlerProtocol != NULL)
   {
      Status = RscHandlerProtocol->Unregister(NvmeLowPowerModeCallback0);
      if (EFI_SUCCESS != Status)
      {
         DEBUG ((EFI_D_ERROR, "Unregister NvmeLowPowerModeCallback failed, status 0x%x\n", Status));
      }
   }
}


/* Exit Boot services event */
extern EFI_GUID gEfiEventExitBootServicesGuid;
static EFI_EVENT ExitBootServicesEvent = NULL;
/* Register the callback to power off Slot 0.
   UEFI does not have a good way to handle low power modes or peripheral shutdown.
   For NVMe we require to tear down the protocol first, then turn of the PCIe host
   controller, but UEFI does not have a way to secify this order by using the
   callback. Current target has only onle NVMe device, this code allows to turn
   off that single NVMe device. */
EFI_STATUS EFIAPI NvmeLowPowerModeRegisterCallback0(VOID)
{
   EFI_STATUS Status = EFI_SUCCESS;
   static char is_init = 0;

   if (is_init != 0)
     return EFI_SUCCESS;

   /* Locate RscHandlerProtocol and register for LPM callback */
   Status = gBS->LocateProtocol(&gEfiRscHandlerProtocolGuid,
                                NULL,
                                (VOID **) &RscHandlerProtocol);
   if((EFI_SUCCESS == Status) && (RscHandlerProtocol != NULL))
   {
      Status = RscHandlerProtocol->Register(NvmeLowPowerModeCallback0, TPL_CALLBACK);
      if(Status != EFI_SUCCESS)
      {
         DEBUG ((EFI_D_ERROR, "Failed to register NvmeLowPowerModeCallback0, status 0x%x\n", Status));
         return Status;
      }

      /* Register for Exit Boot Service Event to Unregister LPM callback */
      Status = gBS->CreateEventEx (EVT_NOTIFY_SIGNAL,
                                   TPL_CALLBACK,
                                   NvmeExitBSCallback,
                                   NULL,
                                   &gEfiEventExitBootServicesGuid,
                                   &ExitBootServicesEvent);

      ASSERT_EFI_ERROR (Status);
   }
   if (Status == EFI_SUCCESS)
     is_init = 1;

   return Status;
}

INTN nvme_core_target_power_on(UINT32 slot)
{
  INTN result = -1;
  EFI_STATUS status;
  npa_client_handle npa_handle = NULL;

  /* We support turning on slot 0 only. */
  if (slot != 0)
  {
    return EFI_UNSUPPORTED;
  }

  /* Register the callbakc to turn off slot 0 in case of USB Charging. */
  status = NvmeLowPowerModeRegisterCallback0();
  if (status != EFI_SUCCESS)
  {
    DEBUG ((EFI_D_INFO, "Failed to register LPM callback for NVMe Status 0x%x\r\n", status));
  }

  npa_handle = nvme_core_get_npa_client_handle();
  if (npa_handle != NULL)
  {
    /* enable request to turn on S8C */
    npa_issue_required_request(npa_handle, PMIC_NPA_MODE_ID_GENERIC_ACTIVE );

    /* To disable request run disable request*/
    /* npa_issue_required_request(npa_handle, PMIC_NPA_MODE_ID_GENERIC_OFF ); */

    /* Turn on the PMIC GPIO #11 to turn on NVME (DBB1 and DBU4) */
    status = nvme_core_target_pmic_gpio_pwr(1);
    if (!EFI_ERROR (status))
      result = 0;

  }

  return result;
}

