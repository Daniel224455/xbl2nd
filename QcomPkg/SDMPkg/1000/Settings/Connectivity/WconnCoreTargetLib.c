/** @file ConnCoreTargetLib.c
 * 
 *  Target specific configuration Items for Connectivity
 * 
 *  Copyright (c) 2019 Qualcomm Technologies, Inc.
 *  All Rights Reserved
 *  Qualcomm Technologies Proprietary and Confidential
 * 
 */

/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.


when       who     what, where, why
--------   ---     ----------------------------------------------------------
11/12/19   owen    initial edit
==============================================================================*/

#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Pi/PiStatusCode.h>
#include <Library/DebugLib.h>
#include <Protocol/EFITlmm.h>
#include <Library/PcdLib.h>
#include <Library/QcomLib.h>
#include <Lpm.h>
#include <Protocol/ReportStatusCodeHandler.h>

#include "npa.h"
#include "pmapp_npa.h"

#define MTP_2_1_HASTINGS_WL_EN  169

EFI_STATUS WconnSetWlanEnGpio(int power)
{
  EFI_TLMM_PROTOCOL *TLMMProtocol = NULL;
  UINT32 GpioValue;
  EFI_STATUS Status;

  Status = gBS->LocateProtocol(&gEfiTLMMProtocolGuid, NULL, (VOID **)&TLMMProtocol);
  if (EFI_ERROR(Status)) {
    DEBUG((EFI_D_ERROR, "%a: locate TLMM protocol failed\r\n", __func__));
    return Status;
  }

  if (power) {
    GpioValue = GPIO_HIGH_VALUE;
  } else {
    GpioValue = GPIO_LOW_VALUE;
  }

  Status = TLMMProtocol->ConfigGpio(
      (UINT32)EFI_GPIO_CFG(MTP_2_1_HASTINGS_WL_EN, 0, GPIO_OUTPUT, GPIO_NO_PULL, GPIO_16MA),
      TLMM_GPIO_ENABLE);
  if (EFI_ERROR(Status)) {
    DEBUG((EFI_D_ERROR, "%a: TLMM GPIO setup failed\r\n", __func__));
    return Status;
  }

  Status = TLMMProtocol->GpioOut(
      (UINT32)EFI_GPIO_CFG(MTP_2_1_HASTINGS_WL_EN, 0, GPIO_OUTPUT, GPIO_NO_PULL, GPIO_16MA),
      GpioValue);
  if (EFI_ERROR(Status)) {
    DEBUG((EFI_D_ERROR, "%a: TLMM GPIO output failed\r\n", __func__));
    return Status;
  }

  return Status;
}

static npa_client_handle gWlanNPAClientHandle = NULL;
npa_client_handle WconnWlanGetNPAClientHandle(void)
{
  npa_query_type query_result;
  npa_query_status query_status = NPA_QUERY_NO_VALUE;

  if (gWlanNPAClientHandle != NULL)
    return gWlanNPAClientHandle;

  /*check if client exists*/
  query_status = npa_query_by_name(PMIC_NPA_GROUP_ID_WLAN, NPA_QUERY_CURRENT_STATE, &query_result);
  if (query_status == NPA_QUERY_SUCCESS) {
    /*get client handle*/
    gWlanNPAClientHandle =  npa_create_sync_client(PMIC_NPA_GROUP_ID_WLAN, "wlan", NPA_CLIENT_REQUIRED);
    if (gWlanNPAClientHandle == NULL) {
      DEBUG((EFI_D_ERROR, "%a: Failed npa_create_sync_client 0x%x\r\n", __func__, query_status));
    }
  } else {
    DEBUG((EFI_D_ERROR, "%a: Failed npa_query_by_name 0x%x\r\n", __func__, query_status));
  }
  return gWlanNPAClientHandle;
}

EFI_STATUS WconnWlanPowerOff(void)
{
  EFI_STATUS Status = EFI_SUCCESS;
  npa_client_handle npa_handle = NULL;

  npa_handle = WconnWlanGetNPAClientHandle();
  if (npa_handle != NULL) {
    Status = WconnSetWlanEnGpio(0);
    if (EFI_ERROR (Status))
      return Status;

    npa_issue_required_request(npa_handle, PMIC_NPA_MODE_ID_GENERIC_OFF );
  }

  return Status;
}

/* Register the callback to be notified of a Low power mode event.
  */
EFI_STATUS EFIAPI WlanLowPowerModeCallback0 (
   IN EFI_STATUS_CODE_TYPE     CodeType,
   IN EFI_STATUS_CODE_VALUE    Value,
   IN UINT32                   Instance,
   IN EFI_GUID                 *CallerId,
   IN EFI_STATUS_CODE_DATA     *Data OPTIONAL)
{
  EFI_STATUS Status = EFI_SUCCESS;
  LPM_EVENT_EXT_DATA *LpmEventData = NULL;

  if (((CodeType & EFI_STATUS_CODE_TYPE_MASK) == EFI_PROGRESS_CODE) &&
      (Value == PcdGet32 (PcdLpm)) && (Data != NULL)) {
    /* Map LpmEventData */
    LpmEventData = (LPM_EVENT_EXT_DATA*)(Data + 1);
    if(TRUE == CompareGuid(&(LpmEventData->CalleeGuid), &gEfiWconnGuid)) {
      switch (LpmEventData->LPMState) {
      case LPM_ENTRY:
        if (WconnWlanPowerOff() != 0) {
          DEBUG((EFI_D_ERROR, "WlanLowPower Callback: LPM state = %d\r\n", LpmEventData->LPMState));
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
VOID EFIAPI WlanExitBSCallback (
   IN EFI_EVENT        Event,
   IN VOID             *Context
  )
{
   EFI_STATUS Status = EFI_SUCCESS;

   /* Unregister LPM callback in Exit Boot Services */
   if (RscHandlerProtocol != NULL) {
      Status = RscHandlerProtocol->Unregister(WlanLowPowerModeCallback0);
      if (EFI_SUCCESS != Status) {
         DEBUG ((EFI_D_ERROR, "%a: Unregister power callback failed, status 0x%x\n", __func__, Status));
      }
   }
}


extern EFI_GUID gEfiEventExitBootServicesGuid;
static EFI_EVENT ExitBootServicesEvent = NULL;
EFI_STATUS EFIAPI WlanLowPowerModeRegisterCallback0(VOID)
{
   EFI_STATUS Status = EFI_SUCCESS;
   static char is_init = 0;

   if (is_init != 0)
     return EFI_SUCCESS;

   /* Locate RscHandlerProtocol and register for LPM callback */
   Status = gBS->LocateProtocol(&gEfiRscHandlerProtocolGuid,
                                NULL,
                                (VOID **) &RscHandlerProtocol);
   if((EFI_SUCCESS == Status) && (RscHandlerProtocol != NULL)) {
      Status = RscHandlerProtocol->Register(WlanLowPowerModeCallback0, TPL_CALLBACK);
      if(Status != EFI_SUCCESS) {
         DEBUG ((EFI_D_ERROR, "Failed to register power callback , status 0x%x\n", Status));
         return Status;
      }

      /* Register for Exit Boot Service Event to Unregister LPM callback */
      Status = gBS->CreateEventEx (EVT_NOTIFY_SIGNAL,
                                   TPL_CALLBACK,
                                   WlanExitBSCallback,
                                   NULL,
                                   &gEfiEventExitBootServicesGuid,
                                   &ExitBootServicesEvent);

      ASSERT_EFI_ERROR (Status);
   }
   if (Status == EFI_SUCCESS)
     is_init = 1;

   return Status;
}

EFI_STATUS WconnWlanPowerOn(void)
{
  EFI_STATUS Status;
  npa_client_handle npa_handle = NULL;

  /* Register the callback to turn off slot 0 in case of USB Charging. */
  Status = WlanLowPowerModeRegisterCallback0();
  if (Status != EFI_SUCCESS) {
    DEBUG ((EFI_D_INFO, "Failed to register LPM callback for  Status 0x%x\r\n", Status));
  }

  npa_handle = WconnWlanGetNPAClientHandle();
  if (npa_handle != NULL) {
    Status = WconnSetWlanEnGpio(0);

    npa_issue_required_request(npa_handle, PMIC_NPA_MODE_ID_GENERIC_ACTIVE );

    gBS->Stall(1500);

    Status = WconnSetWlanEnGpio(1);
    if (EFI_ERROR (Status))
      return Status;
  }

  return Status;
}
