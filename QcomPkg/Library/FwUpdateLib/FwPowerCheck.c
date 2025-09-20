/** @file PowerCheck.c
   
  Battery power level checks for Firmware Update.

  Microsoft requirement is that system must have at least 25% battery life
  as a pre-installation check to permit firmware update.

  Non-production mode devices will allow firmware update to proceed
  when no battery is present for lab automation testing.

  Copyright (c) 2012, 2013, 2017, 2019 Qualcomm Technologies, Inc.  All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential.

**/
/*=============================================================================
                              EDIT HISTORY
  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.



 when           who         what, where, why
 --------       ---         --------------------------------------------------
 2019/03/18     vkasilya    Battery power levels check for UEFI firmware update
 2017/04/18     gparam      Changed the update status from internal type to
                            standard ones defined in EDK headers + Change to always
                            return success during battery status check till we find
                            an alternative PMIC API to use inplace of the current one
 2017/01/27     ai          Fix use of retail
 2013/04/30     mic         Added debug message
 2013/02/08     rsb         Enable battery power check on B-Family targets
 2012/10/03     rsb         Set bPowerGood correctly when function is stubbed out
 2012/08/23     al          Removing Charger and ChargerTask related
 2012/08/03     mic         Aligned with UEFI1.0 and added DeviceIndex and 
                            ResourceIndex in PmicChargerProtocol
 2012/07/25     rs          Reduced battery threshold to 25% from 50% for FwUpdate
 2012/07/10     rsb         Added non-production mode support for no battery
 2012/06/05     rs          Replaced AsciiPrint with DEBUG macro
 2012/4/30      rsb         Initial version

=============================================================================*/
#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/QcomTargetLib.h>
#include <Library/QcomLib.h>
#include <Protocol/EFIPlatformInfo.h>


#include "FwPowerCheck.h"
#include "FwEsrt.h"

/**
  Include Charger DXE Dependencies
  */
#include <Protocol/EFIChargerEx.h>

/**
  Include Charger Extension Dependencies
  */
#include <Protocol/EFIChargerExtn.h>

#define MINIMUM_BATTERY_CHARGE_PERCENT 25

/**
  Define ALLOW_NO_BATTERY_FIRMWARE_UPDATE to allow firmware update to
  proceed for non-production mode devices that have no battery and only
  DC power adapter connected.

  The logic will only permit this behavior for non-production mode
  devices for use in lab automation testing. Devices that are production
  mode will not be permitted to update when no battery is present,
  even if the macro is defined.
  */
#define ALLOW_NO_BATTERY_FIRMWARE_UPDATE

/**
 * Determine if the battery power levels have sufficient power
 * to permit firmware update to proceed based on the pre-condition
 * checks required for UEFI firmware update:
 *    - System must have at least 25% battery life
 *
 * @param bPowerGood        - TRUE : Sufficient power to proceed with firmware update.
 *                            FALSE: Insufficient power to proceed with firmware update.
 * @return EFI_STATUS 
 */


EFI_STATUS
FwUpdateCheckPowerLevel(
  OUT BOOLEAN *bPowerGood
)
{

  EFI_STATUS               Status = EFI_SUCCESS;
  EFI_CHARGER_EX_PROTOCOL *ChargerExtProtocol = NULL;
  EFI_PLATFORMINFO_PLATFORM_TYPE  PlatformType = EFI_PLATFORMINFO_TYPE_UNKNOWN;

  /* Parameters to get the current status of battery */
  UINT32                   CurrentSOC = 0;

  if (bPowerGood == NULL)
  {
    Status = EFI_INVALID_PARAMETER;
    goto power_exit;
  }

  Status = GetPlatformType(&PlatformType);
  if(EFI_SUCCESS != Status)
  {
    DEBUG((EFI_D_WARN, "FwUpdateCheckPowerLevel:: %a Error getting platform type  \r\n", __FUNCTION__));
    goto power_exit;    
  }

  if((PlatformType == EFI_PLATFORMINFO_TYPE_MTP) ||
     (PlatformType == EFI_PLATFORMINFO_TYPE_CDP))
  {
    *bPowerGood = TRUE;
    goto power_exit;    
  }

  /* Locate the Charger Extension Protocol*/
  Status = gBS->LocateProtocol( &gChargerExProtocolGuid,
                                NULL,
                                (VOID **) &ChargerExtProtocol );
  
  if ((Status != EFI_SUCCESS) || (ChargerExtProtocol == NULL))
  {
    DEBUG(( EFI_D_ERROR,"FwUpdateCheckPowerLevel: Locate EFI_CHARGER_EX_PROTOCOL returned %r\n", Status));
    goto power_exit;
  }

  /* GetChargingStatus - Get current state of charge */
  Status = ChargerExtProtocol->GetChargingStatus(&CurrentSOC,
                                                  NULL);

  if (Status != EFI_SUCCESS)
  {
    DEBUG(( EFI_D_ERROR,"FwUpdateCheckPowerLevel: GetBatteryStatus returned %r\n", Status));
    goto power_exit;
  }
  
  DEBUG(( EFI_D_WARN,"GetChargingStatus success, current battery capacity: %d%%\n", CurrentSOC));
  
  if (CurrentSOC < MINIMUM_BATTERY_CHARGE_PERCENT)
  {
    *bPowerGood = FALSE;
  }
  else
  {
    *bPowerGood = TRUE;
  }

power_exit:

  return Status;
}
