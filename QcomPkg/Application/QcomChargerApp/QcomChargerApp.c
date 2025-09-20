/** @file QcomChargerApp.c

  Implements the Qualcomm's Charging application

  Copyright (c) 2016-2019,  Qualcomm Technologies Inc. All rights reserved.

**/

/*=============================================================================
                              EDIT HISTORY


 when         who     what, where, why
 --------     ---     -----------------------------------------------------------
 07/18/19     cs      put silver core to sleep when in offmode charging
 01/16/19     cs      added config item for power testing
 06/28/18     ivy     Clear screen for low battery or low battery charging (early sign of life) before booting to HLOS 
 06/28/18     cs      fixing recovery mode problem
 05/14/18     va      For CLS platform, turning display panel on only before booting 
                      to HLOS, not turning display panel before shutdown, Updated Power key press behavior
                      Added support for only CLS platform
 05/14/17     ra      Support for Debug Accessory Mode
 02/27/17     va      Added charger FW load action for Poipu
 10/13/17     va      Display splash logo if dead battery charging is perfomed
 08/22/17     va      Adding LPM event in extended structure 
 07/21/17     va      Implementing RSC model for LPM interested drivers
 07/12/17     cs      added images for debug board cases.
 06/21/17     dc      Move Platform Check to QcomChargerDxe and support config override
 04/24/17     va      Enable Charger App Changes
 03/24/17     ll      Printed out battery status when in no charge wait state
 03/22/17     ll      Fixed logging sampling time when LED blinking is selected
 03/07/17     cs      remove duplicate get errors checking emergency shutdown condition.
 02/07/17     sm      Added changes to skip ChargerApp for CLS platform
 01/17/17     sm      Code cleanup
 11/10/16     va      Adding crash dump watchdog support
 11/07/16     va      Skip once detected as Debug Board
 10/19/16     sm      Added input parameters to QcomChargerAppEvent_KeyPressHandler() API call
 10/12/16     sv      Fixed Array 'FormatString' KW issue. CR:1063098
 09/29/16     va      update to print function names %a and handle %r format specifier for file logging
 09/26/16     sm      Removed QcomChargerAppEvent_KeyPressControl() API call 
                      and Added keypress check after WaitForTimeout in MonitorCharging API
 09/06/16     cs      wipower changes to remove 3.5 sec, not suspend DCIn
 08/12/16     va      Adding Charger led indication support
 08/10/16     sm      Added changes to skip ChargerApp for RUMI
 07/26/16     va      Restarting FG Changes on warm boot
 07/15/16     cs      added wipower support
 07/07/16     va      DEBUG print to print time stamp in seconds
 07/06/16     sm      Added changes to explicitly disable charging for CDP Platform
 06/28/16     va      Changes to share charger info to smem 
 06/21/16     va      Adding Thermal and File log support
 06/17/16     vk      Delete entry log message
 05/31/16     va      Adding Sign of early life suppot 
 05/23/16     sm      Added changes to handling EFI_QCOM_CHARGER_ACTION_NO_CHARGE_WAIT action
 04/22/16     sm      Added TakeAction() API call for more ChargingAction
 04/13/16     sm      Added support for CDP platform
 03/28/16     va      Add logging CHARGER_DEBUG
 03/21/16     va      Adding Exit API for clean up
 03/15/16     sm      Added changes for bring up to only enter and exit the app
                      without performing any checks. 
 01/27/16     sm      Initial draft for new architecture of Charger app module

=============================================================================*/
#include <Uefi.h>

/**
  EFI interfaces
 */
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/QcomLib.h>
#include <Library/QcomUtilsLib.h>
#include <api/pmic/pm/pm_uefi.h>
#include <Library/QcomTargetLib.h>
#include <Library/FuseControlLib.h>
#include <Library/QcomBaseLib.h>

#include <Protocol/EFIVariableServices.h>
#include <Library/MemoryAllocationLib.h>

#include <Protocol/EFIDisplayUtils.h>

#include <Library/BaseMemoryLib.h>
#include <PrintShLib.h>

/**
  EFI interfaces
 */

#include <Protocol/EFIQcomCharger.h>
#include <Protocol/EFIPlatformInfo.h>

/**
  EFI interfaces
 */
#include <Library/BaseLib.h>

#include "QcomChargerAppFileLogging.h"
#include "QcomChargerAppEventHandler.h"
#include <Lpm.h>
#include <Library/KernelLib.h>
#include <Library/SerialPortShLib.h>
#include <Protocol/EFIKernelInterface.h>
#include <Protocol/EFIPmicPwrOn.h>

/**
File Logging Dependencies 
*/
#include "ULogFront.h"
#include "ULogUEFI.h"

/*===========================================================================*/
/*                  FUNCTIONS PROTOTYPES                                     */
/*===========================================================================*/
#define QCOM_CHARGER_IDLE_WAIT_DURATION_IN_MS            3000 /*3000 milliseconds*/
#define QCOM_CHARGER_DCIN_PENDING_WAIT_DURATION_IN_MS    500  /*500 msec*/
#define QCOM_CHARGER_TOGGLE_LED_WAIT_DURATION_IN_MS      500  /*500 msec*/
#define QCOM_CHARGER_WAIT_TO_LOG_COUNT                   QCOM_CHARGER_IDLE_WAIT_DURATION_IN_MS/QCOM_CHARGER_TOGGLE_LED_WAIT_DURATION_IN_MS  /* 6 times*/

/* Options passed into WaitForTimeout function to break out of the timer loop with a key stroke.*/
#define QCOM_CHARGER_TIMEOUT_WAIT_FOR_KEY    0x01
#define QCOM_CHARGER_POLLING_WAIT_IN_US      3000000 /*3 sec*/
#define QCOM_CHARGER_MS_TO_S                 1000
#define QCOM_CHARGER_CPU_SLEEP_MS            500    /*sleep 500 msec*/
#define QCOM_CHARGER_KEY_DETECTION_WAIT_TIME 5     /*wait for key press for 5 ms*/
#define QCOM_CHARGER_KEY_DETECTION_ITERATION 6    /* (6* 500) = 3000 mec, check status every 3000 msec*/

/*===========================================================================*/
/*                  FUNCTIONS PROTOTYPES                                     */
/*===========================================================================*/
EFI_STATUS QcomChargerApp_PostProcessing( VOID );
EFI_STATUS QcomChargerApp_MonitorCharging( VOID );
EFI_STATUS QcomChargerApp_Initialize( EFI_QCOM_CHARGER_ACTION_TYPE ChargingAction );
EFI_STATUS QcomChargerApp_DeInitialize( VOID );
EFI_STATUS QcomChargerApp_DisplaySignOfLifeOnVBat(EFI_QCOM_CHARGER_ACTION_INFO *ChargerActionInfo);
EFI_STATUS QcomChargerApp_ContinueCharging(EFI_QCOM_CHARGER_ACTION_TYPE ChgAction, EFI_QCOM_CHARGER_ACTION_INFO *pChgActionInfo);

STATIC EFI_QCOM_CHARGER_PROTOCOL    *pQcomChargerProtocol  = NULL;
STATIC EFI_PLATFORMINFO_PLATFORM_TYPE PlatformType = EFI_PLATFORMINFO_TYPE_UNKNOWN;
STATIC EFI_QCOM_PMIC_PWRON_PROTOCOL    *pPmicPwrOnProtocol = NULL;

BOOLEAN                    ForcePowerTest = FALSE;
STATIC BOOLEAN Enable_CPU_Sleep = FALSE;
static EFI_KERNEL_PROTOCOL  *KernIntf = NULL;

/*===========================================================================*/
/*                  FUNCTIONS DEFINITIONS                                    */
/*===========================================================================*/
/**
  QcomChargerApp_Entry ()

  @brief
  Initialize Charger App
 */
EFI_STATUS QcomChargerApp_Entry
(
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
)
{
  EFI_STATUS                     Status = EFI_SUCCESS;
  EFI_QCOM_CHARGER_ACTION_INFO   ChargerActionInfo = {0};
  EFI_QCOM_CHARGER_ACTION_TYPE   ChargingAction;
  BOOLEAN                        bChargingSupported = FALSE;
    UINT32 powerTestValue = 0;
  Status = GetPlatformType(&PlatformType);
  if(EFI_SUCCESS != Status)
  {
    DEBUG((EFI_D_WARN, "QcomChargerApp:: %a Error getting platform type = %r \r\n", __FUNCTION__, Status));
    return EFI_DEVICE_ERROR;
  }

  Status = gBS->LocateProtocol( &gQcomChargerProtocolGuid, NULL, (VOID **)&pQcomChargerProtocol );
  if(EFI_SUCCESS != Status || !pQcomChargerProtocol)
  {
    CHARGERAPP_FILE_UART_DEBUG(( EFI_D_ERROR, "QcomChargerApp:: %a Can not locate Charger Protocol = %r \r\n", __FUNCTION__, Status));
    return EFI_DEVICE_ERROR;
  }

  /*Check if Charging is supported in this platform or not*/  
  Status = pQcomChargerProtocol->IsChargingSupported(&bChargingSupported);
  if(EFI_SUCCESS != Status)
  {
    CHARGERAPP_FILE_UART_DEBUG(( EFI_D_ERROR, "QcomChargerApp:: %a Cannot check if this platform support charging = %r \r\n", __FUNCTION__, Status));
    return EFI_DEVICE_ERROR;    
  }
    
  if(FALSE == bChargingSupported)
  {
    CHARGERAPP_FILE_UART_DEBUG(( EFI_D_WARN, "QcomChargerApp:Charging not supported.\r\n"));
    return EFI_SUCCESS;
  }

  if (KernIntf == NULL)
  {
    Status = gBS->LocateProtocol(&gEfiKernelProtocolGuid, NULL, (VOID **)&KernIntf);
    if ((Status == EFI_SUCCESS) && (KernIntf != NULL))
    {
      Enable_CPU_Sleep = TRUE;
    }
  }
  
  Status = pQcomChargerProtocol->GetChargerConfig(EFI_QCOM_CHARGER_CONFIG_KEY_OFFMODE_POWER_TESTING_MODE, &powerTestValue);
  if (Status != EFI_SUCCESS)
  {
    ForcePowerTest = FALSE;
  }
  else
  {
    ForcePowerTest = (BOOLEAN)powerTestValue;
  }

  SetMem(&ChargerActionInfo, sizeof(EFI_QCOM_CHARGER_ACTION_INFO), 0x00);

  if (Enable_CPU_Sleep && (EFI_PLATFORMINFO_TYPE_CLS == PlatformType || ForcePowerTest))
  {
      if (!pPmicPwrOnProtocol)
      {
        Status = gBS->LocateProtocol(&gQcomPmicPwrOnProtocolGuid, NULL, (VOID **)&pPmicPwrOnProtocol);
        if (EFI_SUCCESS == Status)
        {
           //set up power key rising edge trigger, enable power key IRQ
          BOOLEAN powerPressed = FALSE;
          //Status = pPmicPwrOnProtocol->SetIrqTriggerType(0, EFI_PM_PON_IRQ_KPDPWR_ON, EFI_PM_PON_IRQ_TRIGGER_RISING_EDGE);
          //Status |= pPmicPwrOnProtocol->EnableIrq(0, EFI_PM_PON_IRQ_KPDPWR_ON, TRUE);
          pPmicPwrOnProtocol->GetPonIrqStatus(0, EFI_PM_PON_IRQ_KPDPWR_ON, EFI_PM_PON_IRQ_STATUS_LATCHED, &powerPressed);
          if (powerPressed)
          {
            ChargerActionInfo.bPowerKeyPressed = TRUE;
            pPmicPwrOnProtocol->ClearIrq(0, EFI_PM_PON_IRQ_KPDPWR_ON);
          }
        }
      }
  } 
  
  /*Get charging Action to be taken */
  Status = pQcomChargerProtocol->GetChargingAction(&ChargingAction, &ChargerActionInfo, TRUE);

  /* In case of debug board or good to boot- to save boot time below can be ignored since debug board no Action for charger app */
  if((EFI_QCOM_CHARGER_ACTION_DEBUG_BOARD_GOOD_TO_BOOT != ChargingAction ) && (EFI_QCOM_CHARGER_ACTION_GOOD_TO_BOOT != ChargingAction ))
  {
    Status |= QcomChargerAppFileLog_Init();
    if(EFI_SUCCESS != Status)
    {
      CHARGERAPP_FILE_UART_DEBUG(( EFI_D_WARN, "QcomChargerApp:: %a recieved error from QcomChargerAppFileLog_Init() = %r \r\n", __FUNCTION__, Status));
      /* Override Status */
      Status = EFI_SUCCESS;
    }
    /* This action will be returned only on CLS platform */
    if(EFI_QCOM_CHARGER_ACTION_LOAD_CHARGER_FW  == ChargingAction)
    {
      /* Load Charger FW */
      Status |= pQcomChargerProtocol->TakeAction(ChargingAction, &ChargerActionInfo);
      if(EFI_SUCCESS == Status)
      {
        /* Connect Charger FW */
        Status |= pQcomChargerProtocol->TakeAction(EFI_QCOM_CHARGER_ACTION_CONNECT_CHARGER_FW, &ChargerActionInfo);
        /*Get Next charging Action to be taken */
        Status = pQcomChargerProtocol->GetChargingAction(&ChargingAction, &ChargerActionInfo, TRUE);
      }
    }
    /* Handle if FG battery profile needs to be loaded */
    if(EFI_QCOM_CHARGER_PLATFORM_ACTION_PROFILE_LOAD == ChargingAction)
    {
      if(ChargerActionInfo.ProfState == EFI_QCOM_CHARGER_PROFILE_LOAD)
      {
        Status |= QcomChargerApp_DisplaySignOfLifeOnVBat(&ChargerActionInfo);
      }

      /* Load Battery Profile */
      Status |= pQcomChargerProtocol->TakeAction(ChargingAction, &ChargerActionInfo);

      /*Get charging Action again to be taken to proceed further for charging if needed */
      Status = pQcomChargerProtocol->GetChargingAction(&ChargingAction, &ChargerActionInfo, FALSE);
      if(EFI_SUCCESS != Status )
      {
        CHARGERAPP_DEBUG(( EFI_D_ERROR, "QcomChargerApp:: %a GetChargingAction Status = %r \r\n", __FUNCTION__, Status));
        QcomChargerApp_DeInitialize();
        return EFI_DEVICE_ERROR;
      }
    }

    Status |= pQcomChargerProtocol->TakeAction(ChargingAction, &ChargerActionInfo);

    /* DO charger App Actions */
    switch(ChargingAction)
    {
      case EFI_QCOM_CHARGER_ACTION_START_CHARGING:
      case EFI_QCOM_CHARGER_ACTION_NO_CHARGE_WAIT:
      case EFI_QCOM_CHARGER_ACTION_CONTINUE:
      case EFI_QCOM_CHARGER_ACTION_TSENSE_HIGH_WAIT:
      case EFI_QCOM_CHARGER_ACTION_DEBUG_BOARD_WAIT:
           /*Initializes and displays the battery symbol*/
           Status |= QcomChargerApp_Initialize(ChargingAction);
           if(EFI_SUCCESS == Status)
           {
             /* if initialization success then start charging upto target SoC*/
             Status |= QcomChargerApp_MonitorCharging();
             Status |= QcomChargerApp_PostProcessing();
           }
           else
           {
             CHARGERAPP_DEBUG((EFI_D_WARN, "QcomChargerApp::%a Initialization error = %r \r\n",__FUNCTION__, Status));
             /* return Status */; /* Allowing deinit */
           }
           break;

      case EFI_QCOM_CHARGER_ACTION_GOOD_TO_BOOT:
      case EFI_QCOM_CHARGER_ACTION_DEBUG_BOARD_GOOD_TO_BOOT:
           CHARGERAPP_DEBUG((EFI_D_WARN, "QcomChargerApp:: QcomChargerApp_Start: Battery level is good to boot to HLOS. \r\n"));
           /* Clear screen for low battery or low battery charging (early sign of life) before booting to HLOS  */
           Status |= QcomChargerApp_PostProcessing();
           break;
	  case EFI_QCOM_CHARGER_ACTION_FACTORY_MODE_BOOT_TO_HLOS:
		  CHARGERAPP_DEBUG((EFI_D_WARN, "QcomChargerApp:: QcomChargerApp_Start: Factory Mode Detected, boot to HLOS. \r\n"));
		  break;
    //todo have connected charger FW but failed treat it same as recovery
      case EFI_QCOM_CHARGER_ACTION_CONNECT_CHARGER_FW:
      case EFI_QCOM_CHARGER_ACTION_RECOVERY:
        /* Take action to handle Recovery */
        /* todo Display UI - let device charge conservatively */
        /* if initialization success then start charging upto target SoC*/
        Status |= QcomChargerApp_MonitorCharging();
        Status |= QcomChargerApp_PostProcessing();
        break;
      default:
           CHARGERAPP_DEBUG(( EFI_D_ERROR, "QcomChargerApp:: %a ChargingAction = %d \r\n", __FUNCTION__, ChargingAction));
           break;
    }
  }
  else
  {
    CHARGERAPP_DEBUG(( EFI_D_ERROR, "QcomChargerApp:: %a Debug Board detected boot to hlos ChargingAction = %d \r\n", __FUNCTION__, ChargingAction));
  }
  /* After post processing call to exit module */
  Status |= QcomChargerApp_DeInitialize();

  CHARGERAPP_DEBUG((EFI_D_WARN, "QcomChargerApp:: %a Exiting \r\n", __FUNCTION__));

  return Status;
}

EFI_STATUS QcomChargerApp_Initialize( EFI_QCOM_CHARGER_ACTION_TYPE ChargingAction )
{
  EFI_STATUS Status = EFI_SUCCESS;

  if (EFI_PLATFORMINFO_TYPE_UNKNOWN == PlatformType )
  {
    Status = GetPlatformType(&PlatformType);
    if(EFI_SUCCESS != Status)
    {
      DEBUG((EFI_D_WARN, "QcomChargerApp:: %a Error getting platform type = %r \r\n", __FUNCTION__, Status));
      return EFI_DEVICE_ERROR;
    }
  }

  if (!pQcomChargerProtocol)
  {
    Status = gBS->LocateProtocol(&gQcomChargerProtocolGuid, NULL, (VOID **)&pQcomChargerProtocol);
    if (EFI_SUCCESS != Status)
      return Status;
  }

  if ((EFI_PLATFORMINFO_TYPE_CLS != PlatformType) && (!ForcePowerTest))
  {
    if (ChargingAction == EFI_QCOM_CHARGER_ACTION_TSENSE_HIGH_WAIT)
    {
      QcomChargerAppEvent_DispBattSymbol(EFI_QCOM_CHARGER_DISP_IMAGE_TSENS_THERMAL_SYMBOL);
    }
    else if (ChargingAction == EFI_QCOM_CHARGER_ACTION_DEBUG_BOARD_WAIT)
    {
      QcomChargerAppEvent_DispBattSymbol(EFI_QCOM_CHARGER_DISP_IMAGE_DEBUG_LOW_SYMBOL);
    }
    else
    {
      QcomChargerAppEvent_DispBattSymbol(EFI_QCOM_CHARGER_DISP_IMAGE_LOWBATTERY);
    }
    /* Start required timers */
    Status |= QcomChargerAppEvent_DisplayTimerEvent(TRUE, TRUE);
  }
  else
  {
    CHARGERAPP_DEBUG((EFI_D_WARN, "QcomChargerApp:: %a Entering LPM \r\n", __FUNCTION__));  
    QcomChargerAppEvent_EnterLPM();/* Enter LPM */
  }
  CHARGERAPP_DEBUG((EFI_D_WARN, "QcomChargerApp:: %a Status = %r \r\n", __FUNCTION__, Status));

  return Status;
}

/*
QcomChargerApp_MonitorCharging(): This is the charging loop that will keep the chargerApp running, 
it will also call GetChargingAction function to get the error status, get charging/gauging status, 
control charging and determine when to exit charging loop.
*/
EFI_STATUS QcomChargerApp_MonitorCharging( VOID )
{
  EFI_STATUS                    Status = EFI_SUCCESS;
  EFI_QCOM_CHARGER_ACTION_TYPE  ChargingAction;
  EFI_QCOM_CHARGER_ACTION_INFO  ChargerActionInfo = {0};
  BOOLEAN                       ExitChargingLoop = FALSE;
  EFI_INPUT_KEY                 UserKey;

  CHARGERAPP_DEBUG((EFI_D_WARN, "QcomChargerApp:: %a \r\n", __FUNCTION__));

  if(!pQcomChargerProtocol)
  {
    Status = gBS->LocateProtocol( &gQcomChargerProtocolGuid, NULL, (VOID **)&pQcomChargerProtocol );
    if(EFI_SUCCESS != Status)
      return Status;
  }
  
  /* Disable crash dump watchdog only when charger app starts charging */
  gBS->SetWatchdogTimer(0, 0, 0, NULL);

  CHARGERAPP_FILE_DEBUG((EFI_D_WARN, "RebootCount,TimeStamp,StateOfCharge,Voltage,ChargeCurrent,Temp \r\n"));
  DEBUG((EFI_D_WARN, "QcomChargerApp::%a TimeStamp,StateOfCharge,Voltage,ChargeCurrent,Temp \r\n",__FUNCTION__));

  do
  {
    /* Get charging action to be taken */
    Status = pQcomChargerProtocol->GetChargingAction(&ChargingAction, &ChargerActionInfo, TRUE);
    CHARGERAPP_DEBUG((EFI_D_WARN, "QcomChargerApp:: %a ChargingAction = %d Status = %r LedConfigType = %d \r\n", __FUNCTION__, ChargingAction, Status, ChargerActionInfo.LedConfigType));
    if(EFI_SUCCESS != Status)
    {
      return Status;
    }

    switch(ChargingAction)
    {
      case EFI_QCOM_CHARGER_ACTION_GOOD_TO_BOOT:
      case EFI_QCOM_CHARGER_ACTION_DEBUG_BOARD_GOOD_TO_BOOT:
           Status = pQcomChargerProtocol->TakeAction(ChargingAction, &ChargerActionInfo);

           QcomChargerAppEvent_ExitLPM();/* Exit LPM and */
           QcomChargerAppEvent_DisplayPanelOn(); /* Turn display panel ON */

           QcomChargerAppEvent_DisplayTimerEvent(FALSE, FALSE);  /*Display image */

           ExitChargingLoop = TRUE;
           break;

      case EFI_QCOM_CHARGER_ACTION_SHUTDOWN:

           QcomChargerAppEvent_ExitLPM(); /* Exit LPM */
           if ((EFI_PLATFORMINFO_TYPE_CLS != PlatformType) && (!ForcePowerTest))
           {
              QcomChargerAppEvent_DisplayTimerEvent(FALSE, FALSE); /*Display image before shutting down*/
              QcomChargerAppEvent_DisplayPanelOn(); /* Turn display panel ON */
           }

           Status = pQcomChargerProtocol->TakeAction(ChargingAction, &ChargerActionInfo);
           QcomChargerAppFileLog_LogParam(&ChargerActionInfo);
           break;

      case EFI_QCOM_CHARGER_ACTION_NO_CHARGE_WAIT:
      case EFI_QCOM_CHARGER_ACTION_DEBUG_BOARD_WAIT:
           /* Take Action to make sure charging is disabled */
           Status = pQcomChargerProtocol->TakeAction(EFI_QCOM_CHARGER_ACTION_NO_CHARGE_WAIT, &ChargerActionInfo);
           
           QcomChargerAppFileLog_LogParam(&ChargerActionInfo);
           CHARGERAPP_FILE_UART_DEBUG((EFI_D_WARN, "QcomChargerApp:: %a No Charge Wait ChrgAct = %d wait %d ms \r\n", __FUNCTION__, ChargingAction, QCOM_CHARGER_IDLE_WAIT_DURATION_IN_MS));
           WaitForTimeout (QCOM_CHARGER_IDLE_WAIT_DURATION_IN_MS, QCOM_CHARGER_TIMEOUT_WAIT_FOR_KEY, &UserKey);

           /*Check if WaitForTimeout returned due to key press */
           if((UserKey.UnicodeChar != 0x00) || (UserKey.ScanCode != 0x00))
           {
             DEBUG((EFI_D_WARN, "QcomChargerApp:: %a Keypress detected \r\n", __FUNCTION__));
             if (ChargingAction == EFI_QCOM_CHARGER_ACTION_DEBUG_BOARD_WAIT)
             {
               QcomChargerAppEvent_KeyPressHandler(QCOMCHARGERAPP_EVENT_ANIMATION__DEBUG_BOARD_NO_ANIM);
             }
             else
             {
               QcomChargerAppEvent_KeyPressHandler(QCOMCHARGERAPP_EVENT_ANIMATION__LOW_BATTERY_CHARGER_NO_ANIM);
             }            
           }

           break;

      case EFI_QCOM_CHARGER_ACTION_TSENSE_HIGH_WAIT:
        {
          /* Take Action to make sure charging is disabled */
          Status = pQcomChargerProtocol->TakeAction(ChargingAction, &ChargerActionInfo);

      QcomChargerAppFileLog_LogParam(&ChargerActionInfo);
          CHARGERAPP_FILE_UART_DEBUG((EFI_D_WARN, "QcomChargerApp:: %a No Charge Wait ChrgAct = %d wait %d ms \r\n", __FUNCTION__, ChargingAction, QCOM_CHARGER_IDLE_WAIT_DURATION_IN_MS));
          WaitForTimeout(QCOM_CHARGER_IDLE_WAIT_DURATION_IN_MS, QCOM_CHARGER_TIMEOUT_WAIT_FOR_KEY, &UserKey);
          /*Check if WaitForTimeout returned due to key press */
          if ((UserKey.UnicodeChar != 0x00) || (UserKey.ScanCode != 0x00))
          {
            DEBUG((EFI_D_WARN, "QcomChargerApp:: %a Keypress detected \r\n", __FUNCTION__));
            QcomChargerAppEvent_KeyPressHandler(QCOMCHARGERAPP_EVENT_ANIMATION__THERMAL_NO_ANIM);
          }
        }
      break;
      
      case EFI_QCOM_CHARGER_ACTION_CRITICAL:
           /*Exit LPM and Display image before shutting down*/
           QcomChargerAppEvent_ExitLPM();
           if (EFI_PLATFORMINFO_TYPE_CLS != PlatformType && (!ForcePowerTest))
           {
                 QcomChargerAppEvent_DisplayPanelOn(); /* Turn display panel ON */
           }
           Status = pQcomChargerProtocol->TakeAction(ChargingAction, &ChargerActionInfo);
           break;

      case EFI_QCOM_CHARGER_ACTION_SHUTDOWN_USB_DC_PON_DISABLED:
           /*Exit LPM and Display image before shutting down*/
           QcomChargerAppEvent_ExitLPM();
           if (EFI_PLATFORMINFO_TYPE_CLS != PlatformType && (!ForcePowerTest))
           {
                QcomChargerAppEvent_DisplayPanelOn(); /* Turn display panel ON */
           }
           CHARGERAPP_FILE_UART_DEBUG((EFI_D_WARN, "QcomChargerApp:: %a Shutdown with PON disabled \r\n", __FUNCTION__));

           Status = pQcomChargerProtocol->TakeAction(ChargingAction, &ChargerActionInfo);
           QcomChargerAppFileLog_LogParam(&ChargerActionInfo);
           break;
      case EFI_QCOM_CHARGER_ACTION_CONTINUE:
      default:
           Status = QcomChargerApp_ContinueCharging(ChargingAction, &ChargerActionInfo);
           break;
    }

  }while (FALSE == ExitChargingLoop);

  /* Start 10min watchdog timer */
  gBS->SetWatchdogTimer(10 * 60, 0, 0, NULL);

  if (ChargingAction == EFI_QCOM_CHARGER_ACTION_DEBUG_BOARD_GOOD_TO_BOOT)
  {
    Status |= pQcomChargerProtocol->DisplayImage(EFI_QCOM_CHARGER_DISP_IMAGE_DEBUG_BOOT_SYMBOL, TRUE);
  }
  else
  {
    Status |= pQcomChargerProtocol->DisplayImage(EFI_QCOM_CHARGER_DISP_IMAGE_ABOVE_THRESHOLD, TRUE);
  }

  CHARGERAPP_DEBUG((EFI_D_WARN, "QcomChargerApp:: %a Exiting ChargingAction = %d \r\n", __FUNCTION__, ChargingAction));

  return Status;
}

/*After Charging is completed, handle cases for threshold charging and full battery charging*/
EFI_STATUS QcomChargerApp_PostProcessing( VOID )
{
  EFI_STATUS Status = EFI_SUCCESS;
  EFI_QCOM_DISPLAY_UTILS_PROTOCOL *pDisplayUtilsProtocol = NULL;
  EFI_DISPLAY_UTILS_RENDERLOGO_CONFIG RenderLogoCfg = { 0 };

  if(!pQcomChargerProtocol)
  {
    Status = gBS->LocateProtocol( &gQcomChargerProtocolGuid, NULL, (VOID **)&pQcomChargerProtocol );
  }

  QcomChargerAppEvent_ExitLPM();

  /* Close timer events */
  QcomChargerAppEvent_DisplayTimerEvent(FALSE, FALSE); 

  /* Ensure charging is enabled when booting from UEFI to HLOS */
  Status |= pQcomChargerProtocol->EnableCharging(TRUE);

  if(NULL == pDisplayUtilsProtocol)
  {
    Status |= gBS->LocateProtocol( &gQcomDisplayUtilsProtocolGuid, NULL, (VOID **)&pDisplayUtilsProtocol);
  }

  if((EFI_SUCCESS == Status) && (pDisplayUtilsProtocol != NULL))
  {
    RenderLogoCfg.uDisplayId = 0;
    RenderLogoCfg.uFlags     = DISPLAY_UTILS_RENDERLOGO_CLEARSCREEN;
    RenderLogoCfg.xPosition  = 0;
    RenderLogoCfg.yPosition  = 0;
    Status |= pDisplayUtilsProtocol->DisplayUtilsRenderLogo(&RenderLogoCfg);
  }

  return Status;
}

EFI_STATUS QcomChargerApp_DeInitialize( VOID)
{
  EFI_STATUS Status = EFI_SUCCESS;

  if(!pQcomChargerProtocol)
  {
    Status = gBS->LocateProtocol( &gQcomChargerProtocolGuid, NULL, (VOID **)&pQcomChargerProtocol );
  }

  if((EFI_SUCCESS == Status) && (pQcomChargerProtocol))
  {
    Status = pQcomChargerProtocol->ChargerDeInit(EFI_QCOM_CHARGER_ACTION_EXIT);
  }
  if (pPmicPwrOnProtocol != NULL)
  {
    Status |= pPmicPwrOnProtocol->EnableIrq(0, EFI_PM_PON_IRQ_KPDPWR_ON, FALSE);
    pPmicPwrOnProtocol->ClearIrq(0, EFI_PM_PON_IRQ_KPDPWR_ON);
  }
  return Status;
}


EFI_STATUS QcomChargerApp_DisplaySignOfLifeOnVBat(EFI_QCOM_CHARGER_ACTION_INFO *ChargerActionInfo)
{
  EFI_STATUS Status   = EFI_SUCCESS;

  if(!ChargerActionInfo)
    return EFI_INVALID_PARAMETER;

  CHARGERAPP_DEBUG((EFI_D_WARN, "QcomChargerApp::%a CurrSoc = %d, BatteryVolt = %d mV, VbatThresholdForDisp = %d mV, Status = %r \r\n",__FUNCTION__,
                ChargerActionInfo->BatteryStatusInfo.StateOfCharge , ChargerActionInfo->BatteryStatusInfo.BatteryVoltage, ChargerActionInfo->DispSignOfLifeMaxThresholdMv, Status));

  if(ChargerActionInfo->BatteryStatusInfo.BatteryVoltage <= (ChargerActionInfo->DispSignOfLifeMaxThresholdMv ))
  {
    if(TRUE == ChargerActionInfo->bChargerSrcPresent)
    {
      Status |= QcomChargerAppEvent_DispBattSymbol(EFI_QCOM_CHARGER_DISP_IMAGE_LOWBATTERYCHARGING);
    }
    else
    {
      Status |= QcomChargerAppEvent_DispBattSymbol(EFI_QCOM_CHARGER_DISP_IMAGE_LOWBATTERY);
    }
  }
  else
  {
    CHARGERAPP_DEBUG((EFI_D_WARN, "QcomChargerApp:: %a Skipping Init Display On CurrSoc = %d, BatteryVolt = %d mV, ", __FUNCTION__, 
                      ChargerActionInfo->BatteryStatusInfo.StateOfCharge , ChargerActionInfo->BatteryStatusInfo.BatteryVoltage));
  CHARGERAPP_DEBUG((EFI_D_WARN, "DispSignOfLifeMaxThresholdMv = %d mV Status = %r \r\n",
                      ChargerActionInfo->DispSignOfLifeMaxThresholdMv, Status));
  }

  return Status;
}

/**
  QcomChargerApp_ContinueCharging ()

  @brief
  Take action to continue charging
 */
EFI_STATUS QcomChargerApp_ContinueCharging(EFI_QCOM_CHARGER_ACTION_TYPE ChgAction, EFI_QCOM_CHARGER_ACTION_INFO *pChgActionInfo)
{
  EFI_STATUS Status = EFI_SUCCESS;
  UINT8 ledToggleCnt;
  EFI_INPUT_KEY UserKey;
  UINT8 index = 0;
  /* Take Action */
  if (pChgActionInfo->LedConfigType == EFI_QCOM_CHARGER_CHARGING_LED_TOGGLE)
  {
    ledToggleCnt = 0;
    do
    {
      Status |= pQcomChargerProtocol->TakeAction(ChgAction, pChgActionInfo);
      WaitForTimeout(QCOM_CHARGER_TOGGLE_LED_WAIT_DURATION_IN_MS, QCOM_CHARGER_TIMEOUT_WAIT_FOR_KEY, &UserKey);
      /*Check if WaitForTimeout returned due to key press */
      if ((UserKey.UnicodeChar != 0x00) || (UserKey.ScanCode != 0x00))
      {
        DEBUG((EFI_D_WARN, "QcomChargerApp:: %a Keypress detected \r\n", __FUNCTION__));
        QcomChargerAppEvent_KeyPressHandler(QCOMCHARGERAPP_EVENT_ANIMATION__CHARGING_ANIM);
      }
      ledToggleCnt++;
      // Wait 3sec (QCOM_CHARGER_IDLE_WAIT_DURATION_IN_MS) to go to next charger status check loop
    } while (ledToggleCnt < QCOM_CHARGER_WAIT_TO_LOG_COUNT); 

  }
  else
  {
    Status |= pQcomChargerProtocol->TakeAction(ChgAction, pChgActionInfo);
    if (Enable_CPU_Sleep) 
    {
      for (index = 0; index < QCOM_CHARGER_KEY_DETECTION_ITERATION; index++)
      {
        BOOLEAN powerPressed = FALSE;               
        if ((pPmicPwrOnProtocol != NULL) && (EFI_PLATFORMINFO_TYPE_CLS == PlatformType || ForcePowerTest))
        {  /*Check if WaitForTimeout returned due to power key press */
          pPmicPwrOnProtocol->GetPonIrqStatus(0, EFI_PM_PON_IRQ_KPDPWR_ON, EFI_PM_PON_IRQ_STATUS_LATCHED, &powerPressed);
          if (powerPressed)
          {
            pChgActionInfo->bPowerKeyPressed = TRUE;
            pPmicPwrOnProtocol->ClearIrq(0, EFI_PM_PON_IRQ_KPDPWR_ON);
            break;
          }
        }
        else
        {
          WaitForTimeout(QCOM_CHARGER_KEY_DETECTION_WAIT_TIME, QCOM_CHARGER_TIMEOUT_WAIT_FOR_KEY, &UserKey); 
          /*Check if WaitForTimeout returned due to key press */
          if ((UserKey.UnicodeChar != 0x00) || (UserKey.ScanCode != 0x00))
          {
            DEBUG((EFI_D_WARN, "QcomChargerApp:: %a Keypress detected \r\n", __FUNCTION__));
            QcomChargerAppEvent_ExitLPM();/* Exit LPM and */
            QcomChargerAppEvent_DisplayPanelOn(); /* Turn display panel ON */
            QcomChargerAppEvent_KeyPressHandler(QCOMCHARGERAPP_EVENT_ANIMATION__CHARGING_ANIM);
            break;
          }
        }
        MpcoreSleepCpu(QCOM_CHARGER_CPU_SLEEP_MS);
      }
      QcomChargerAppFileLog_LogParam(pChgActionInfo);
      CHARGERAPP_FILE_UART_DEBUG((EFI_D_INFO, "QcomChargerApp:: %a Waiting for %d ms \r\n", __FUNCTION__, QCOM_CHARGER_IDLE_WAIT_DURATION_IN_MS));    
      return Status;
    }
    else
    {
      WaitForTimeout(QCOM_CHARGER_IDLE_WAIT_DURATION_IN_MS, QCOM_CHARGER_TIMEOUT_WAIT_FOR_KEY, &UserKey);
      if (EFI_PLATFORMINFO_TYPE_CLS == PlatformType || ForcePowerTest)
      {  /*Check if WaitForTimeout returned due to power key press */
        if ((UserKey.UnicodeChar == 0x00) && (UserKey.ScanCode == 0x102))
        {
          pChgActionInfo->bPowerKeyPressed = TRUE;
        }
      }
      else
      {
        /*Check if WaitForTimeout returned due to key press */
        if ((UserKey.UnicodeChar != 0x00) || (UserKey.ScanCode != 0x00))
        {
          DEBUG((EFI_D_WARN, "QcomChargerApp:: %a Keypress detected \r\n", __FUNCTION__));
          QcomChargerAppEvent_ExitLPM();/* Exit LPM and */
          QcomChargerAppEvent_DisplayPanelOn(); /* Turn display panel ON */
          QcomChargerAppEvent_KeyPressHandler(QCOMCHARGERAPP_EVENT_ANIMATION__CHARGING_ANIM);
        }
      }

    }
    
  }
  QcomChargerAppFileLog_LogParam(pChgActionInfo);
  CHARGERAPP_FILE_UART_DEBUG((EFI_D_INFO, "QcomChargerApp:: %a Waiting for %d ms \r\n", __FUNCTION__, QCOM_CHARGER_IDLE_WAIT_DURATION_IN_MS));

  return Status;
}
