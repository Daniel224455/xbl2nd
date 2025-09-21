/** @file ChargerTest.c
   
  Tests for Charger

  Copyright (c) 2011-2015, 2017 Qualcomm Technologies, Inc. All rights reserved.
  
**/

/*=============================================================================
                              EDIT HISTORY



 when       who     what, where, why
 --------   ---     -----------------------------------------------------------
 03/20/17   va      Updated App for charger ex 
 09/29/14   va      Updated App for supporting arguments
 08/27/14   va      Updated Asciiprint to DEBUG 
 04/29/13   yg      Add back Stall
 04/04/13   yg      Some cleanup
 07/03/12   aus     Replaced DEBUG with AsciiPrint
 02/02/12   sm      Replaced DEBUG messages with AsciiPrint
 06/06/11   dy      New File

=============================================================================*/

#include <Protocol/EFIChargerEx.h>
#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/TestInterface.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>
#include <Library/BMPLib.h>
#include <Library/GuidedFvLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Protocol/EFIChargerEx.h>
#include <Protocol/EFIQcomCharger.h>
#include <Library/QcomLib.h>
#include <DDITF.h>
#include <Library/TestInterface.h>
#include <Protocol/EFIDDRGetConfig.h>
#include <Library/UefiLib.h>
#include <Protocol/EFIMemoryMap.h>
#include <Library/PrintLib.h>
#include <Library/BaseMemoryLib.h>  // for ZeroMem
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>

static EFI_CHARGER_EX_PROTOCOL  *ChargerExProtocol = NULL;

EFI_STATUS LocateChargerProtocol() {
    EFI_STATUS  Status = EFI_SUCCESS;
    /* Locate Charger Ex Protocol */
    Status = gBS->LocateProtocol(
      &gChargerExProtocolGuid,
      NULL,
      (VOID**) &ChargerExProtocol );

    if ((Status != EFI_SUCCESS) || (NULL == ChargerExProtocol)) 
    {
        AsciiPrint("ERROR: ChargerTest - Failed to locate Charger Protocol! Error Code = 0x%08X\r\n", Status);
        return TF_FAILURE;
    }
    AsciiPrint("ChargerTest: Charger Ex Protocol located \n");
    return Status;
}


EFI_STATUS ChargerPresenceTest(BOOLEAN *bChargerPresence) {

    EFI_STATUS Status = EFI_SUCCESS;

    if (ChargerExProtocol == NULL) {    
      Status = LocateChargerProtocol();
      if (Status != EFI_SUCCESS) {
        return Status;
      }  
    }

    // Get Charger Presence
    Status = ChargerExProtocol->GetChargerPresence(bChargerPresence);
    if (Status != EFI_SUCCESS)
    {
        AsciiPrint("ERROR: ChargerTest - Charger is Not Present Charging Error Code: %r \r\n", Status);
    }
    else
    {
        AsciiPrint("ChargerTest: Charger Presence Status = %d \n", *bChargerPresence);
    }
    return Status;
}


EFI_STATUS BatteryPresenceTest(BOOLEAN *bBatteryPresence) {
    EFI_STATUS Status = EFI_SUCCESS;
  
    if (ChargerExProtocol == NULL) {    
      Status = LocateChargerProtocol();
      if (Status != EFI_SUCCESS) {
        return Status;
      }  
    }


    //Get Battery Presence
    Status = ChargerExProtocol->GetBatteryPresence(bBatteryPresence);
    if (Status != EFI_SUCCESS)
    {
        AsciiPrint("ERROR: ChargerTest - Charger is Not Present Charging Error Code: %d \r\n", Status);
    }
    else
    {
        AsciiPrint("ChargerTest: Battery Presence Status = %d \n", *bBatteryPresence);
    }
    return Status;
}


UINT32 ChargerTestMain (uint32 dwParam, char *apszParam[])
{
  EFI_STATUS  Status = EFI_SUCCESS;

  EFI_CHARGER_EX_FLASH_INFO        FlashInfo = {0};
  UINT32 BatteryVoltageOld = 0;
  BOOLEAN IsCharging = FALSE;
  BOOLEAN IsChargerPresent = FALSE;
  BOOLEAN IsBatteryPresent = FALSE;

  Status = LocateChargerProtocol();
  if (Status != EFI_SUCCESS) {
    return Status;
  }

  // Check if charger is not present or can't check then fail
  Status = ChargerPresenceTest(&IsChargerPresent);
  if (Status != EFI_SUCCESS) {
    return Status;
  } else if(!IsChargerPresent) {
    return TF_FAILURE;
  }

  Status = BatteryPresenceTest(&IsBatteryPresent);
  if (Status != EFI_SUCCESS) {
    return Status;
  } else if(!IsBatteryPresent) {
    return TF_FAILURE;
  }


  uint32 chargingTime = 30000000; //wait for 30 secs
  uint32 chargingThreshold = 5; //5mV voltage difference should be seen in 30 secs of charging for this test to pass
  uint32 maxChargeVoltage = 3.6 * 1000; //3.6 V 

  if (dwParam == 3) {
    chargingTime = AsciiStrDecimalToUintn(apszParam[0]);
    chargingThreshold = AsciiStrDecimalToUintn(apszParam[1]); 
    maxChargeVoltage = AsciiStrDecimalToUintn(apszParam[2]); 
  }

  /* Get SW Image flash Status, 0 */
  Status = ChargerExProtocol->IsPowerOk(EFI_CHARGER_EX_POWER_FLASH_BATTERY_VOLTAGE_TYPE, &FlashInfo);
  if (Status != EFI_SUCCESS) {
     AsciiPrint("ERROR: ChargerTest - Failed to read battery voltage with status = %d \r\n", Status);
     return Status;
  } else {
     BatteryVoltageOld = FlashInfo.BattCurrVoltage;
     AsciiPrint("ChargerTest: BattCurrVoltage before: %dmV \r\n", FlashInfo.BattCurrVoltage);
  }

  if (BatteryVoltageOld >= maxChargeVoltage) {
    AsciiPrint("ChargerTest: Charging cannot be validated \n");
    AsciiPrint("ChargerTest: Battery is voltage is: %dmV which is greater than max voltage: %dmV\n",
                FlashInfo.BattCurrVoltage, maxChargeVoltage);
    Print(L"Battery voltage is: %dmV which is greater than max voltage: %dmV\n",
                FlashInfo.BattCurrVoltage, maxChargeVoltage);
    Status = TF_FAILURE;
    return Status;
  }

  // Force charger on as we don't know for sure the state UEFI charging code will leave it in
  EFI_QCOM_CHARGER_PROTOCOL    *pQcomChargerProtocol  = NULL;
  Status = gBS->LocateProtocol( &gQcomChargerProtocolGuid, NULL, (VOID **)&pQcomChargerProtocol );
  Status |= pQcomChargerProtocol->EnableCharging(TRUE);
  if( Status != EFI_SUCCESS) {
     AsciiPrint("ERROR: Failed EnableCharging with status = %d \r\n", Status);
     return Status;
  }

  // Force 1A charging for test because screen and CPU takes ~ 400mA and will not charge battery
  // if charging is only set to 500mA
  Status = pQcomChargerProtocol->SetMaxUsbCurrent(1000);
  if( Status != EFI_SUCCESS) {
     AsciiPrint("ERROR: Failed SetMaxUsbCurrent with status = %d \r\n", Status);
     return Status;
  }

  gBS->Stall(chargingTime);

  // Set charging current back to safe value of 500mA after test
  Status = pQcomChargerProtocol->SetMaxUsbCurrent(500);
  if( Status != EFI_SUCCESS) {
     AsciiPrint("ERROR: Failed SetMaxUsbCurrent with status = %d \r\n", Status);
     return Status;
  }

  /* Get SW Image flash Status 1*/
  Status = ChargerExProtocol->IsPowerOk(EFI_CHARGER_EX_POWER_FLASH_BATTERY_VOLTAGE_TYPE, &FlashInfo);
  if (Status != EFI_SUCCESS)
  {
     AsciiPrint("ERROR: ChargerTest - Failed to read battery voltage with status = %d \r\n", Status);
     return Status;
  }
  else
  {
     AsciiPrint("ChargerTest: BattCurrVoltage after: %dmV \r\n", FlashInfo.BattCurrVoltage);
  }

  /*Check voltage changes to verfiy the battery is charging*/
  if ( (FlashInfo.BattCurrVoltage > BatteryVoltageOld) &&
    (FlashInfo.BattCurrVoltage - BatteryVoltageOld) >= chargingThreshold)
  {
      IsCharging = TRUE;
      AsciiPrint("ChargerTest: Battery Charging Status = %d \n", IsCharging);
  }
  else {
    Status = TF_FAILURE;
    IsCharging = FALSE;
    AsciiPrint("ChargerTest: Battery Charging Status = %d \n", IsCharging);
  }

  return Status;
}


