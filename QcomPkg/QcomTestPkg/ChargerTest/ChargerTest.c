/** @file ChargerTest.c

  Tests for Charger

  Copyright (c) 2011-2019 Qualcomm Technologies, Inc. All rights reserved.
  Portions Copyright (c) 2006 - 2016, Intel Corporation. All rights reserved.<BR>
  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

/*=============================================================================
                              EDIT HISTORY



 when       who     what, where, why
 --------   ---     -----------------------------------------------------------
 03/15/19   mr      Added call to verify FW Update feature
 03/20/17   va      Updated App for charger ex
 09/29/14   va      Updated App for supporting arguments
 08/27/14   va      Updated Asciiprint to DEBUG
 04/29/13   yg      Add back Stall
 04/04/13   yg      Some cleanup
 07/03/12   aus     Replaced DEBUG with AsciiPrint
 02/02/12   sm      Replaced DEBUG messages with AsciiPrint
 06/06/11   dy      New File

=============================================================================*/

#include "ChargerTest.h"
#include <Protocol/EFIChargerEx.h>
#include <Protocol/EFIQcomCharger.h>

#include "TestInterface.h"


EFI_STATUS ChargerTest_TestFwUpdateSupport (VOID);


/* Sleep without burning CPU cycles */
EFI_STATUS
WaitForEventTimeout (IN UINT32   Timeoutms)
{
  UINTN           Index;
  EFI_STATUS      Status = EFI_NOT_READY;
  EFI_EVENT       TimerEvent;
  EFI_EVENT       WaitList[3];
  UINT64          TimeoutVal;
  EFI_INPUT_KEY   KeyVal;

  if (Timeoutms == 0)
    return EFI_INVALID_PARAMETER;

  /* Create a Timer event  */
  Status = gBS->CreateEvent (EVT_TIMER, 0, NULL, NULL, &TimerEvent);

  if (Status != EFI_SUCCESS)
    return Status;

  /* Convert to 100ns units */
  TimeoutVal = Timeoutms * 10000;

  /* Set Timer event */
  gBS->SetTimer (TimerEvent, TimerRelative, TimeoutVal);

  /* Wait for some event or the timer */
  WaitList[0] = TimerEvent;
  WaitList[1] = gST->ConIn->WaitForKey;
  Status = gBS->WaitForEvent (2, WaitList, &Index);
  if (Index == 1)
    gST->ConIn->ReadKeyStroke (gST->ConIn, &KeyVal);
  gBS->CloseEvent (TimerEvent);

  return Status;
}

/**
  The user Entry Point for Application. The user code starts with this function
  as the real entry point for the application.

  @param[in] ImageHandle    The firmware allocated handle for the EFI image.
  @param[in] SystemTable    A pointer to the EFI System Table.

  @retval EFI_SUCCESS       The entry point is executed successfully.
  @retval other             Some error occurs when executing this entry point.

**/
EFI_STATUS
EFIAPI
ChargerTestMain (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS  Status = EFI_SUCCESS;
  EFI_CHARGER_EX_PROTOCOL  *ChargerExProtocol = NULL;
  /* Parameters to get Charger Ex status */
  BOOLEAN bOffModeCharging = FALSE;
  EFI_CHARGER_EX_FLASH_INFO        FlashInfo = {0};

#if 0
  /* Act on cmd line arguments if fails continue for charge */
  Status = GetCmdLineArgs (ImageHandle, &Argc, &Argv);


  if(Argc > 0 )//Act only if arguments are present
  {
    DEBUG((EFI_D_WARN, "Setting Arguments \r\n"));

    Status = ChargerTestH_ProcessCommand(Argc, Argv, &TargetSOC, &displayStatus);

  }else
  {
    DEBUG((EFI_D_WARN,"Arguments not provided \r\n"));
  }
#endif

  /* Start Charger test now */
  TEST_START("ChargerTest: Charger Ex");

  /* Locate Charger Ex Protocol */
  Status = gBS->LocateProtocol(
                  &gChargerExProtocolGuid,
                  NULL,
                  (VOID**) &ChargerExProtocol );

  if ((Status != EFI_SUCCESS) || (NULL == ChargerExProtocol))
  {
    DEBUG((EFI_D_WARN,"ERROR: ChargerTest - Failed to locate Charger Protocol! Error Code = 0x%08X\r\n", Status));
    return Status;
  }

  DEBUG((EFI_D_WARN,"ChargerTest: Charger Ex Protocol located \n"));

  /* Get Off Mode charging status */
  Status = ChargerExProtocol->IsOffModeCharging(&bOffModeCharging);
  if (Status != EFI_SUCCESS)
  {
    DEBUG((EFI_D_WARN,"ERROR: ChargerTest - Failed to Off Mode Charging Error Code: %r \r\n", Status));
  }
  else
  {
    DEBUG((EFI_D_WARN,"ChargerTest: Off Mode charging status = %d \n", bOffModeCharging));
  }

  /* Get SW Image flash Status */
  Status = ChargerExProtocol->IsPowerOk(EFI_CHARGER_EX_POWER_FLASH_BATTERY_VOLTAGE_TYPE, &FlashInfo);
  if (Status != EFI_SUCCESS)
  {
    DEBUG((EFI_D_WARN,"ERROR: ChargerTest - Failed to Flash Battery voltage Error Code = %r \r\n", Status));
  }
  else
  {
    DEBUG((EFI_D_WARN,"ChargerTest: SW can Flash = %d Batt current volt = %dmV Required = %dmV \r\n",
          FlashInfo.bCanFlash, FlashInfo.BattCurrVoltage, FlashInfo.BattRequiredVoltage));
  }

  Status = ChargerTest_TestFwUpdateSupport();

  if(EFI_SUCCESS == Status)
  {
    TestStatus("ChargerTest: Charger Ex", Status);
  }
  else//ChargerCompletionToken error
  {
    TestStatus("ChargerTest: Charger Ex", Status);
  }

  TEST_STOP("ChargerTest: Charger Ex");

  ReadAnyKey (NULL, 0);

  return EFI_SUCCESS;
}


/* Battery FW Update feature validation */
EFI_STATUS ChargerTest_TestFwUpdateSupport (VOID)
{
    EFI_STATUS  Status = EFI_SUCCESS;
    EFI_CHARGER_EX_PROTOCOL  *ChargerExProtocol = NULL;
    EFI_QCOM_CHARGER_PROTOCOL *pQcomChargerProtocol = NULL;

    UINT32 BatterySOC = 0;
    BOOLEAN IsChargerAttached = FALSE;

    UINT8  TestFwUpdateSupport = 0;
    UINTN  VarSize = sizeof(UINT8);

    Status = gRT->GetVariable(L"TestFwUpdateSupport", &gOSAVendorGuid, NULL, &VarSize, &TestFwUpdateSupport);
    if (EFI_SUCCESS != Status)
    {
        DEBUG((EFI_D_ERROR, "ChargerTest:: %a  GetVariable failed, Error[%r] \r\n", __FUNCTION__, Status));
        goto TestExit;
    }

    DEBUG((EFI_D_WARN, "ChargerTest:: %a  TestFwUpdateSupport = %d \r\n", __FUNCTION__, TestFwUpdateSupport));

    if(!TestFwUpdateSupport)
    {
        goto TestExit;
        DEBUG((EFI_D_WARN, "ChargerTest:: %a  Feature verification not supported \r\n", __FUNCTION__));
    }

    /* Locate Charger Ex Protocol */
    Status = gBS->LocateProtocol(&gChargerExProtocolGuid, NULL, (VOID**) &ChargerExProtocol);
    if(EFI_SUCCESS != Status || !ChargerExProtocol)
    {
        DEBUG((EFI_D_ERROR, "ChargerTest:: %a  ChargerExProtocol locate failed, Error = [%r] \r\n", __FUNCTION__, Status));
        goto TestExit;
    }

    /* Locate Qcom Charger Protocol */
    Status = gBS->LocateProtocol( &gQcomChargerProtocolGuid, NULL, (VOID **)&pQcomChargerProtocol );
    if(EFI_SUCCESS != Status || !pQcomChargerProtocol)
    {
        DEBUG((EFI_D_ERROR, "ChargerTest:: %a  pQcomChargerProtocol locate failed, Error = [%r] \r\n", __FUNCTION__, Status));
        goto TestExit;
    }

    Status = ChargerExProtocol->GetChargingStatus(&BatterySOC, &IsChargerAttached);
    if (EFI_SUCCESS != Status)
    {
        DEBUG((EFI_D_ERROR, "ChargerTest:: %a  GetChargingStatus failed, Error[%r] \r\n", __FUNCTION__, Status));
        goto TestExit;
    }

    DEBUG((EFI_D_WARN, "ChargerTest:: %a  (Before) BatterySOC = %d, IsChargerAttached = %d \r\n", __FUNCTION__, BatterySOC, IsChargerAttached));

    if (BatterySOC > 25 || IsChargerAttached)
    {
        UINT32 wait_time = 0;

        Status = ChargerExProtocol->SetFirmwareUpdateMode(EFI_CHARGER_EX_FW_UPDATE_MODE_ENTER);
        if (EFI_SUCCESS != Status)
        {
            DEBUG((EFI_D_ERROR, "ChargerTest:: %a  SetFirmwareUpdateMode failed, Error[%r] \r\n", __FUNCTION__, Status));
            goto TestExit;
        }

        wait_time = 5000;
        DEBUG((EFI_D_WARN, "ChargerTest:: %a  Waiting for [%d]msec\r\n", __FUNCTION__, wait_time));
        WaitForTimeout (wait_time, TIMEOUT_WAIT_FOR_KEY, NULL);

        BatterySOC = 0; IsChargerAttached = FALSE;
        Status = ChargerExProtocol->GetChargingStatus(&BatterySOC, &IsChargerAttached);
        if (EFI_SUCCESS != Status)
        {
            DEBUG((EFI_D_ERROR, "ChargerTest:: %a  GetChargingStatus failed, Error[%r] \r\n", __FUNCTION__, Status));
            // goto TestExit;
        }
        else  DEBUG((EFI_D_WARN, "ChargerTest:: %a  (After) BatterySOC = %d, IsChargerAttached = %d \r\n", __FUNCTION__, BatterySOC, IsChargerAttached));

        Status = ChargerExProtocol->SetFirmwareUpdateMode(EFI_CHARGER_EX_FW_UPDATE_MODE_EXIT);
        if (EFI_SUCCESS != Status)
        {
            DEBUG((EFI_D_ERROR, "ChargerTest:: %a  SetFirmwareUpdateMode failed, Error[%r] \r\n", __FUNCTION__, Status));
            goto TestExit;
        }

        // Shutdown/Reboot required after FW update complete
        {
            EFI_QCOM_CHARGER_ACTION_INFO ChargerActionInfo = {0};

            Status = pQcomChargerProtocol->TakeAction(EFI_QCOM_CHARGER_ACTION_SHUTDOWN, &ChargerActionInfo);
            if (EFI_SUCCESS != Status)
            {
              DEBUG((EFI_D_ERROR, "ChargerTest:: %a  TakeAction failed, Error[%r] \r\n", __FUNCTION__, Status));
              goto TestExit;
            }
        }
    }

TestExit:
    return Status;
}
