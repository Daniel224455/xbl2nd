/** 
 @file PcieLowBattCharging.c

  Implements functionalities to manage Pcie core power during low battery 
  charging use case.

  Copyright (c) 2018-2019 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.

**/


/*=============================================================================
                              EDIT HISTORY


 when       who     what, where, why
 --------   ----    ----------------------------------------------------------
 01/31/19   ts      Executing LPM only when Display receives the hint
 10/30/18   ts      Initial version
=============================================================================*/


#include <Uefi.h>
#include <Uefi/UefiSpec.h>
#include <Library/UefiLib.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Pi/PiStatusCode.h>
#include <Protocol/StatusCode.h>
#include <Protocol/ReportStatusCodeHandler.h>
#include <Library/BaseMemoryLib.h>
#include <Lpm.h>
#include "PcieLink.h"
#include "PcieBsp.h"

EFI_STATUS
EFIAPI PcieConfigLibLowBattChargingHandler (
  IN EFI_STATUS_CODE_TYPE   CodeType,
  IN EFI_STATUS_CODE_VALUE  Value,
  IN UINT32                 Instance,
  IN EFI_GUID               *CallerId,
  IN EFI_STATUS_CODE_DATA   *Data
  );

/***************************************************************************//**
 * @fn PcieConfigLibRegisterLowBattChargingCallBack
 * @brief Register a call back with ChargerDxe to get an Low Battery Charging
 * callback 
 *
 * This functions registers a callback with Rsc, which will be called
 * during low battery charging use case 
 *
 * @param[in] void
 *
 * @return EFI_SUCCESS or appropriate error value
 ******************************************************************************/
EFI_STATUS
EFIAPI
PcieConfigLibRegisterLowBattChargingCallBack (
  BOOLEAN InitType
  )
{
  EFI_STATUS                Status = EFI_SUCCESS;
  EFI_RSC_HANDLER_PROTOCOL *RscHandlerProtocol = NULL;

  Status = gBS->LocateProtocol (&gEfiRscHandlerProtocolGuid,
                                NULL,
                                (VOID **) &RscHandlerProtocol);

  if((EFI_SUCCESS == Status) && RscHandlerProtocol != NULL)
  {
    if (InitType == TRUE)
    {
      Status = RscHandlerProtocol->Register (PcieConfigLibLowBattChargingHandler,
                                             TPL_CALLBACK);
    }
    else
    {
      Status = RscHandlerProtocol->Unregister (PcieConfigLibLowBattChargingHandler);
    }

    if (EFI_ERROR(Status))
    {
      DEBUG ((EFI_D_WARN, "%a: RegisterCallback Failed \r\n", __func__));
      return Status;
    }
  }
  else
  {
    DEBUG ((EFI_D_WARN, "%a: LocateRscProtocol Failed \r\n", __func__));
  }

  return Status;
}

/***************************************************************************//**
 * @fn PcieConfigLibLPMEnter
 * @brief Enter LPM state for all the known root ports that exists 
 *
 * This functions enters into PCIe low power mode by performing the following:
 *
 * o-> For each root ports, Turnoff PHY & clocks
 * o-> Turnoff if there are any common clocks
 * o-> Turnoff the LDOs
 *
 * @param[in] void
 *
 * @return EFI_SUCCESS or appropriate error value
 ******************************************************************************/
STATIC
EFI_STATUS
PcieConfigLibLPMEnter (
  void
  )
{
  rootPort_t   * RootPort;
  UINT32         NumRp;
  UINT32         Rp;
  STATIC BOOLEAN InLpm = FALSE;
  EFI_STATUS     Status = EFI_SUCCESS;

  /* 
   * This function cannot be executed more than once
   * as there is no way to exit LPM in UEFI for PCIe
   */
  if (InLpm == TRUE)
    return EFI_SUCCESS;

  RootPort = PcieConfigLibGetPciRootPortInfo();
  if (RootPort == NULL)
  {
    DEBUG(( EFI_D_WARN, "%a: No root ports to process\r\n", __func__));
    return EFI_DEVICE_ERROR;
  }

  /* Get number of root ports available in the system */
  NumRp = RootPort->num_rp;

  for (Rp = 0; Rp < NumRp; Rp++)
  {
    /*
     * Turn OFF the port PHY
     */
    Status = PcieConfigLibDeInitializePHY(Rp);
    if (EFI_ERROR(Status))
    {
      DEBUG(( EFI_D_WARN, "%a: Disable Port Clock failed RP:%u\r\n",
              __func__, Rp));
      return Status;
    }

    /*
     * Turn OFF the port clocks
     */
    Status = PcieConfigLibTurnOffPortClocks(Rp);
    if (EFI_ERROR(Status))
    {
      DEBUG(( EFI_D_WARN, "%a: Disable Port Clock failed RP:%u\r\n",
              __func__, Rp));
      return Status;
    }
  }

  /*
   * Turn OFF the commonn clocks
   */
  Status = PcieConfigLibTurnOffCommonClocks();
  if (EFI_ERROR(Status))
  {
    DEBUG(( EFI_D_WARN, "%a: Disable Common Clock failed\r\n", __func__));
    return Status;
  }

  /*
   * Turn OFF LDO
   */
  Status = PcieConfigLibDeInitializeLDO();
  if (EFI_ERROR(Status))
  {
    DEBUG(( EFI_D_WARN, "%a: De-Init LDO failed\r\n", __func__));
    return Status;
  }

  /* Indicate that the device is in Low power mode */
  InLpm = TRUE;

  return Status;
}

/***************************************************************************//**
 * @fn PcieConfigLibLowBattChargingHandler
 * @brief LPM callback function 
 *
 * Callback function to handle low power mode entry and exit for PCIe

 * @param  CodeType             Indicates the type of status code being
 *                              reported.
 * @param  Value                Describes the current status of a hardware or
 *                              software entity. This included information about
 *                              the class and subclass that is used to classify
 *                              the entity as well as an operation.
 * @param  Instance             The enumeration of a hardware or software entity
 *                              within the system. Valid instance numbers start
 *                              with 1.
 * @param  CallerId             This optional parameter may be used to identify
 *                              the caller. This parameter allows the status code
 *                              driver to apply different rules to different
 *                              callers.
 * @param  Data                 This optional parameter may be used to pass
 *                              additional data.
 *
 * @retval EFI_SUCCESS          The function completed successfully.
 * @retval EFI_DEVICE_ERROR     Charger Protocol GUID not matched
 ******************************************************************************/
EFI_STATUS
EFIAPI PcieConfigLibLowBattChargingHandler (
  IN EFI_STATUS_CODE_TYPE   CodeType,
  IN EFI_STATUS_CODE_VALUE  Value,
  IN UINT32                 Instance,
  IN EFI_GUID               *CallerId,
  IN EFI_STATUS_CODE_DATA   *Data
  )
{
  LPM_EVENT_EXT_DATA * LpmEventData = NULL;
  BOOLEAN              bMatch       = FALSE;
  EFI_STATUS           Status       = EFI_UNSUPPORTED;

  DEBUG(( EFI_D_INFO, "%a Called\r\n", __func__));

  if (((CodeType & EFI_STATUS_CODE_TYPE_MASK) == EFI_PROGRESS_CODE ) &&
      (Data != NULL))
  {
    LpmEventData = (LPM_EVENT_EXT_DATA*)( Data + 1);

    bMatch = CompareGuid(&(LpmEventData->CalleeGuid), &gQcomDisplayPwrCtrlProtocolGuid);
    if (bMatch == TRUE)
    {
      switch (LpmEventData->LPMState)
      {
        case LPM_ENTRY:
          DEBUG(( EFI_D_WARN, "%a Charging Entry\r\n", __func__));
          Status = PcieConfigLibLPMEnter();
          if (EFI_ERROR(Status))
          {
            DEBUG(( EFI_D_WARN, "%a: Enter LPM failed\r\n", __func__));
            return Status;
          }
        break;
        case LPM_EXIT:
          /* Should never enter this case, as it is unsupported */
        break;
        default:
          DEBUG(( EFI_D_WARN, "%a Invalid LPM State\r\n", __func__));
        break;
      }
    }
  }

  return Status;
}
