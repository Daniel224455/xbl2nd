/** @file SmartChargerPlatform.c

  Implements Qualcomm's Smart Charging solution

  Copyright (c) 2017-2018,  Qualcomm Technologies Inc. All rights reserved.

**/

/*=====================================================================================
                              EDIT HISTORY


 when         who     what, where, why
 --------     ---     -----------------------------------------------------------------
 03/14/18     pbitra  Updates to let display turn on, to display BDS menu on screen
 02/28/18     pbitra  checking for Reboot from HLOS case and booting to HLOS directly
 02/01/18     mr      Added support TypeC Role select from S/w
 01/29/18     dc      Enable Type-C/PD USB Current Update in Platform Init function
 11/16/17     pbitra  Added support to read DISABLEBATTERY from UEFI variable
 11/09/17     pbitra  Added Power Key press support to boot to HLOS
 					  Added Battery Unlock sequence
 					  Added support for DISABLEBATTERY case
 07/31/17     pbitra  Initial revision

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
#include <Library/FuseControlLib.h>
#include <Library/QcomBaseLib.h>

#include <Protocol/EFIVariableServices.h>
#include <Library/MemoryAllocationLib.h>

#include <Library/BaseMemoryLib.h>
#include <Library/BaseLib.h>

/**
  EFI interfaces
 */
#include <Protocol/GraphicsOutput.h>
#include <Protocol/EFIPlatformInfo.h>
#include <Protocol/EFIUsbPwrCtrl.h>
#include <Protocol/EFIPmicPon.h>
#include <Protocol/EFIPmicPwrOn.h>


#include "SmartChargerPlatform.h"
#include "SmartChargerPlatform_File.h"

#include <api/pmic/pm/pm_chg.h>

/**
Smem Interface 
*/
#include "smem.h"


/*-------------------------------------------------------------------------
 * Type Declarations
 * ----------------------------------------------------------------------*/
extern EFI_GUID gOSAVendorGuid;

/*===========================================================================*/
/*                  FUNCTIONS PROTOTYPES                                     */
/*===========================================================================*/
#define SMART_CHARGER_IDLE_WAIT_DURATION            3000 /*3 sec in milliseconds*/
#define SMART_CHARGER_MS_TO_S                       1000

#define SMART_CHARGER_INVALID_VALUE_MARKER  -1

#define MAX_15_W 15

STATIC UINT32 gThresholdSoc   = 0;

SMART_CHARGER_PLATFORM_CFGDATA_TYPE gSmartChargerPlatformCfgData; 

STATIC BOOLEAN gChargingInitialized = FALSE;

pm_chg_info_type gSmartChargerSharedInfo;

STATIC BOOLEAN gSmartChargerCfgInitialized = FALSE;
STATIC EFI_USB_PWR_CTRL_PROTOCOL    *pUsbPwrCtrlProtocol  = NULL;
EFI_STATUS SmartChargerPlatform_GetUefiVariableValues( void );
EFI_STATUS SmartChargerPlatform_SaveChargeInfoToSmem(pm_chg_info_type *ChargerActionInfo);
EFI_STATUS SmartChargerPlatform_CheckIfOkToBoot(EFI_QCOM_CHARGER_ACTION_TYPE *pActionType, SMART_CHARGER_BATT_STATUS_INFO CurrentBatteryStatus, BOOLEAN  bPowerKeyPressed);

/*===========================================================================*/
/*                  FUNCTIONS PROTOTYPES                                     */
/*===========================================================================*/
/*
SmartChargerPlatform_Init(): This function locates and initializes SmartCharger Protocol and other protocols that are 
needed for that specific platform. It also loads the cfg file and initializes charger and FG HW accordingly. 
*/
EFI_STATUS SmartChargerPlatform_Init( VOID )
{
  EFI_STATUS Status = EFI_SUCCESS;
  BOOLEAN	ChargerPresent = FALSE;
  BOOLEAN	BatteryPresent = FALSE;
  UINT32 batteryCheckCount = 5;
 
  SetMem(&gSmartChargerPlatformCfgData, sizeof(SMART_CHARGER_PLATFORM_CFGDATA_TYPE), 0x00);
  SetMem(&gSmartChargerSharedInfo, sizeof(pm_chg_info_type), 0xFF);

  /* Load CFG file */
  Status |= SmartChargerPlatformFile_ReadDefaultCfgData();
  if(EFI_SUCCESS == Status)
  {
    SMARTCHARGER_DEBUG(( EFI_D_WARN, "SmartChargerDxe:: %a Successfully read config file\n\r", __FUNCTION__));
    gSmartChargerCfgInitialized = TRUE;
  }

  /* Read debug var from pmic bds menu */
  Status  |= SmartChargerPlatform_GetUefiVariableValues();

  if ((EFI_SUCCESS == Status) && (TRUE == gSmartChargerPlatformCfgData.ChargerLibCfgData.dbg_cfg_Data.print_charger_app_dbg_msg_to_file))
  {
    Status = SmartChargerPlatformFile_FileLogInit(gSmartChargerPlatformCfgData);
  }

  //SMARTCHARGER_DEBUG(( EFI_D_WARN, "SmartChargerDxe:: %a chargerlib_cfgdata_type = %d SMART_CHARGER_PLATFORM_CFGDATA_TYPE = %d \r\n", __FUNCTION__, sizeof(chargerlib_cfgdata_type), sizeof(SMART_CHARGER_PLATFORM_CFGDATA_TYPE) ));

  /* Init SmartCharger lib */
  Status |= SmartChargerLibCommon_Init((chargerlib_cfgdata_type*)&gSmartChargerPlatformCfgData.ChargerLibCfgData);
  if (EFI_UNSUPPORTED == Status)
  {
      SMARTCHARGER_DEBUG(( EFI_D_ERROR, "SmartChargerDxe:: %a Error: Charging is not supported for this platform. \r\n", __FUNCTION__));
      return EFI_SUCCESS;
  }
  else if(EFI_SUCCESS != Status)
  {
    SMARTCHARGER_DEBUG(( EFI_D_ERROR, "SmartChargerDxe:: %a Error when initializing SmartChargerLibCommon = %r \r\n", __FUNCTION__, Status));
    return Status;
  }

  SMARTCHARGER_DEBUG(( EFI_D_WARN, "SmartChargerDxe:: %a Successfully initialized SmartChargerLibCommon %r \r\n", __FUNCTION__, Status));

  Status = SmartChargerLib_GetChargerPresence(&ChargerPresent);
  SMARTCHARGER_DEBUG(( EFI_D_ERROR, "SmartChargerPlatform::%a SChargerPresent : %d \r\n", __FUNCTION__, ChargerPresent));

  if ((TRUE == ChargerPresent) && (! gSmartChargerPlatformCfgData.ChargerLibCfgData.schg_cfg_data.DISABLEBATTERY))
  {
    /* Configure USB Current */
    //SMARTCHARGER_DEBUG((EFI_D_WARN, "SmartChargerPlatform::%a Configure Initial USB Current \r\n", __FUNCTION__));		
    SmartChargerLib_ConfigureUsbCurrentSetting();
    
	/* Battery Presence Detection */
	Status = SmartChargerLib_GetBatteryPresence(&BatteryPresent);
	SMARTCHARGER_DEBUG(( EFI_D_ERROR, "SmartChargerPlatform::%a SmartChargerLib_GetBatteryPresence : %d \r\n", __FUNCTION__, BatteryPresent));
	if (!BatteryPresent)
	{
	    /* Battery unlock sequence */
		SMARTCHARGER_DEBUG((EFI_D_WARN, "SmartChargerPlatform::%a Enabling charging to unlock battery \r\n", __FUNCTION__));		
		SmartChargerLib_ChargerEnable(TRUE);
		do
		{	
			/* Allow time for the battery to unlock */
			gBS->Stall((uint32)(gSmartChargerPlatformCfgData.ChargerLibCfgData.schg_cfg_data.BatteryUnlockTime));	/* 20 Msec wait */
			Status = SmartChargerLib_GetBatteryPresence(&BatteryPresent);
			if (!BatteryPresent) /* The battery is either not present or failed to unlock */
			{
				batteryCheckCount--;			
			}
			else
			{
				break;
			}
	
		}while (batteryCheckCount);

		if (!BatteryPresent) /* The battery is either not present or failed to unlock */
		{
			SMARTCHARGER_DEBUG(( EFI_D_ERROR, "SmartChargerPlatform::%a Battery not detected, disabling charging \r\n", __FUNCTION__));
			Status = SmartChargerLib_ChargerEnable(FALSE);
			return Status;
		}	
	}

  }

  /* Set ChargingTermCurrent, FCCMAx and FVMax  */
  Status = SmartChargerLib_SetChargingTermCurrent(gSmartChargerPlatformCfgData.ChargerLibCfgData.schg_cfg_data.ChargingTermCurrent);

  Status |= SmartChargerLib_SetFCCMax(gSmartChargerPlatformCfgData.ChargerLibCfgData.schg_cfg_data.ChgFccMax);

  Status |= SmartChargerLib_SetFVMax(gSmartChargerPlatformCfgData.ChargerLibCfgData.schg_cfg_data.ChgFvMax);

  gThresholdSoc   = gSmartChargerPlatformCfgData.ChargerLibCfgData.boot_to_hlos_threshold_in_soc;

  return Status;
}

EFI_STATUS SmartChargerPlatform_GetChargingAction(EFI_QCOM_CHARGER_ACTION_TYPE *pActionType, SMART_CHARGER_PLATFORM_ACTION_INFO *pChargerActionInfo, BOOLEAN vbattChecking)
{
	EFI_STATUS Status                = EFI_SUCCESS;
	CHARGERLIB_CHARGING_ERROR_TYPES           ErrorType  = CHARGERLIB_CHARGING_ERROR_NONE;
	STATIC EFI_QCOM_CHARGER_ACTION_TYPE  PrevChargerAction = EFI_QCOM_CHARGER_ACTION_INVALID;
	BOOLEAN                                   ChargingEnabled    = FALSE;
	
	if (!pChargerActionInfo || !pActionType)
	return EFI_INVALID_PARAMETER;

	/* Get Error like debug board or battery not detected first */
	Status |= SmartChargerLib_GetErrors(vbattChecking, &ErrorType);
	if(EFI_SUCCESS != Status)
	{
		SMARTCHARGER_DEBUG(( EFI_D_ERROR, "SmartChargerDxe:: %a Error Getting Battery Error = %r.\r\n", __FUNCTION__, Status));
		*pActionType = EFI_QCOM_CHARGER_ACTION_SHUTDOWN;
		PrevChargerAction = *pActionType;
		return Status;
	}
  
	if((CHARGERLIB_CHARGING_ERROR_BATTERY_NOT_DETECTED == ErrorType ) || (CHARGERLIB_DEVICE_ERROR == ErrorType ) ||
		(CHARGERLIB_CHARGING_ERROR_TSENSE_CRITICAL == ErrorType) || (CHARGERLIB_CHARGING_ERROR_TSENSE_TIMEOUT == ErrorType) || (CHARGERLIB_CHARGING_ERROR_TSENSE_HIGH == ErrorType))
	{
		Status = SmartChargerLib_GetErrorAction(ErrorType, (((CHARGERLIB_ERROR_ACTION_TYPE*)pActionType)));
		PrevChargerAction = *pActionType;
		SMARTCHARGER_DEBUG((EFI_D_WARN, "SmartChargerDxe:: %a pActionType = %d.\r\n", __FUNCTION__, *pActionType));
		/*If there is a battery error, return */
		return Status;
	}

	if(! gSmartChargerPlatformCfgData.ChargerLibCfgData.schg_cfg_data.DISABLEBATTERY )
	{
		Status |= SmartChargerLib_GetBatteryStatus((chargerlib_batt_status_info*)&(pChargerActionInfo->BattStsInfo));
		if (EFI_SUCCESS != Status)
		{
		  SMARTCHARGER_DEBUG((EFI_D_WARN, "SmartChargerDxe:: %a Error Getting Battery Status = %r.\r\n", __FUNCTION__, Status));
		  *pActionType = EFI_QCOM_CHARGER_ACTION_STOP_CHARGING;
		  PrevChargerAction = *pActionType;

		  /*If there is an error, return since action is decided */
		  return Status;
		}
	}

	if(CHARGERLIB_CHARGING_ERROR_NONE != ErrorType)
	{
		Status = SmartChargerLib_GetErrorAction(ErrorType, (((CHARGERLIB_ERROR_ACTION_TYPE*)pActionType)));
		PrevChargerAction = *pActionType;

		/*If there is a battery error, return */
		return Status;
	}

	/* Charge Source Detection */
	Status = SmartChargerLib_GetChargingPath(&pChargerActionInfo->ChargerAttached);  
	if (EFI_SUCCESS != Status)
	{
		SMARTCHARGER_DEBUG((EFI_D_WARN, "SmartChargerDxe:: %a Error Getting Power Path = %r.\r\n", __FUNCTION__, Status));
		*pActionType = EFI_QCOM_CHARGER_ACTION_STOP_CHARGING;
		PrevChargerAction = *pActionType;
		return Status;
	}

	Status = SmartChargerLib_GetChargingStatus(&ChargingEnabled);
	if(EFI_SUCCESS != Status)
	{
		SMARTCHARGER_DEBUG((EFI_D_WARN, "SmartChargerLib::%a Error Getting Charging Status = %r \r\n", __FUNCTION__, Status));
		return Status;
	}

	if ((SMART_CHARGER_PLATFORM_CHARGER_ATTACHED_USB  == pChargerActionInfo->ChargerAttached) &&
	((EFI_QCOM_CHARGER_ACTION_INVALID == PrevChargerAction) ||
	(EFI_QCOM_CHARGER_ACTION_NO_CHARGE_WAIT == PrevChargerAction)))
	{
		*pActionType = EFI_QCOM_CHARGER_ACTION_START_CHARGING;	
	}
	else
	{
		if(FALSE == ChargingEnabled)
		{
			*pActionType = EFI_QCOM_CHARGER_ACTION_START_CHARGING;
		}
		else
		{
		  /* Charging already started, go to continue. */
		  *pActionType = EFI_QCOM_CHARGER_ACTION_CONTINUE;
		} 
	
		if (SMART_CHARGER_PLATFORM_CHARGER_ATTACHED_USB == pChargerActionInfo->ChargerAttached)
		{
			Status |= SmartChargerLib_ConfigureUsbCurrentSetting();
		}
	}

	Status |= SmartChargerPlatform_CheckIfOkToBoot(pActionType, pChargerActionInfo->BattStsInfo, pChargerActionInfo->bPowerKeyPressed);

	SMARTCHARGER_DEBUG(( EFI_D_WARN, "SmartChargerLib:: %a Action Returned = %d \r\n", __FUNCTION__,*pActionType));

	PrevChargerAction = *pActionType;

	return Status;
  
}

EFI_STATUS SmartChargerPlatform_TakeAction(EFI_QCOM_CHARGER_ACTION_TYPE ChargingAction, CONST SMART_CHARGER_PLATFORM_ACTION_INFO *pChargerActionInfo)
{
  SMART_CHARGER_BATT_STATUS_INFO CurrentBatteryStatus = {0};

  if(!pChargerActionInfo)
  {
    SMARTCHARGER_DEBUG(( EFI_D_WARN, "SmartChargerDxe:: %a Invalid parameter \r\n", __FUNCTION__));
    return EFI_INVALID_PARAMETER;
  }

  CurrentBatteryStatus = (SMART_CHARGER_BATT_STATUS_INFO)pChargerActionInfo->BattStsInfo;

  switch (ChargingAction)
  {
  case EFI_QCOM_CHARGER_ACTION_CRITICAL:
         //print out and flush critical error messages 
         //Perform AFP
         SMARTCHARGER_FILE_UART_DEBUG((EFI_D_WARN, "SmartChargerDxe::%a Critical Error occurred. Shutting down \r\n", __FUNCTION__));
         SmartChargerLib_ForceSysShutdown(CHGAPP_RESET_AFP);
         break;

  case   EFI_QCOM_CHARGER_ACTION_SHUTDOWN:
         //print error message and trigger system shutdown
         //These errors will only be checked and handled when battery voltage is not high enough to boot and uefi charging is needed.
         SMARTCHARGER_FILE_UART_DEBUG((EFI_D_WARN, "SmartChargerDxe::%a Waiting for %d s \r\n", __FUNCTION__, SMART_CHARGER_IDLE_WAIT_DURATION/SMART_CHARGER_MS_TO_S));
         WaitForTimeout (SMART_CHARGER_IDLE_WAIT_DURATION, TIMEOUT_WAIT_FOR_KEY, NULL);
         SmartChargerLib_ForceSysShutdown(CHGAPP_RESET_SHUTDOWN);
         break;

  case EFI_QCOM_CHARGER_ACTION_START_CHARGING:
          SMARTCHARGER_DEBUG(( EFI_D_WARN, "SmartChargerDxe:: %a Saving Entry VBatt  = %d SOC = %d \r\n", __FUNCTION__,
                          CurrentBatteryStatus.BatteryVoltage, CurrentBatteryStatus.StateOfCharge));
          if (SMART_CHARGER_PLATFORM_CHARGER_ATTACHED_USB == pChargerActionInfo->ChargerAttached)
          {
            if ( EFI_SUCCESS != SmartChargerLib_InitializeCharging())
            {
				SmartChargerLib_ForceSysShutdown(CHGAPP_RESET_SHUTDOWN);
            }
            gChargingInitialized = TRUE;
          }
         break;

  case EFI_QCOM_CHARGER_ACTION_STOP_CHARGING:
         if(TRUE == gChargingInitialized)
         {
           SmartChargerLib_ChargerEnable(FALSE);
           gChargingInitialized = FALSE;
         }
         break;

  case EFI_QCOM_CHARGER_ACTION_NO_CHARGE_WAIT:
  case EFI_QCOM_CHARGER_ACTION_TSENSE_HIGH_WAIT:
           //only need to disable charging or shutdown
         SmartChargerLib_HandleNoChargeAndWait();
         if(gSmartChargerPlatformCfgData.ChargerLibCfgData.enable_charger_wdog)
         {
           /* Pet the watchdog if feature is enabled */
           SmartChargerLib_PetChgWdog();
         }
         break;
    
  case EFI_QCOM_CHARGER_ACTION_CONTINUE:
         if(gSmartChargerPlatformCfgData.ChargerLibCfgData.enable_charger_wdog)
         {
           /* Pet the watchdog if feature is enabled */
           SmartChargerLib_PetChgWdog();
         }
         break;

  case EFI_QCOM_CHARGER_ACTION_GOOD_TO_BOOT:
        /* Assign Exit voltage and soc */
        gSmartChargerSharedInfo.uefi_exit_mV    = CurrentBatteryStatus.BatteryVoltage;
        gSmartChargerSharedInfo.uefi_exit_soc = CurrentBatteryStatus.StateOfCharge;

        SMARTCHARGER_DEBUG(( EFI_D_WARN, "SmartChargerDxe:: %a Good to Boot, saving Exit VBat = %d Soc = %d \r\n", __FUNCTION__,
                         CurrentBatteryStatus.BatteryVoltage, CurrentBatteryStatus.StateOfCharge));
        /* Save Smem Info ignoring return status as XBL loader changes are not ready yet*/
        SmartChargerPlatform_SaveChargeInfoToSmem(&gSmartChargerSharedInfo);
       
        break;

  case EFI_QCOM_CHARGER_ACTION_SHUTDOWN_USB_DC_PON_DISABLED:
      //print error message and trigger system shutdown
      //These errors will only be checked and handled when battery voltage is not high enough to boot and uefi charging is needed.
      SMARTCHARGER_FILE_UART_DEBUG((EFI_D_WARN, "SmartChargerDxe::%a Waiting for %d s \r\n", __FUNCTION__, SMART_CHARGER_IDLE_WAIT_DURATION/SMART_CHARGER_MS_TO_S));
      WaitForTimeout (SMART_CHARGER_IDLE_WAIT_DURATION, TIMEOUT_WAIT_FOR_KEY, NULL);
      SmartChargerLib_ForceSysShutdown(CHGAPP_RESET_SHUTDOWN_USB_DC_PON_DISABLED);
      break;

  case EFI_QCOM_CHARGER_ACTION_RESET_COLD:
      //print error message and trigger system reset
      //These errors will only be checked and handled when battery voltage is not high enough to boot and uefi charging is needed.
      SMARTCHARGER_FILE_UART_DEBUG((EFI_D_WARN, "SmartChargerDxe::%a Waiting for %d s \r\n", __FUNCTION__, SMART_CHARGER_IDLE_WAIT_DURATION/SMART_CHARGER_MS_TO_S));
      WaitForTimeout (SMART_CHARGER_IDLE_WAIT_DURATION, TIMEOUT_WAIT_FOR_KEY, NULL);
	  SmartChargerLib_ForceSysShutdown(CHGAPP_RESET_COLD);
        break;
	  
    default:
        SMARTCHARGER_DEBUG(( EFI_D_WARN, "SmartChargerDxe:: %a Action Passed = %d \r\n", __FUNCTION__,ChargingAction));
      break;
  }
  
  return EFI_SUCCESS;
}


/**
SmartChargerPlatform_GetUefiVariableValues()

@brief
Reads UEFI Variables changed via BDS Menu
*/
EFI_STATUS SmartChargerPlatform_GetUefiVariableValues( VOID)
{
  UINT8          Flag          = 0;
  UINTN          VarSize       = sizeof(Flag);
  STATIC BOOLEAN VariablesRead = FALSE;
  EFI_STATUS     Status = EFI_SUCCESS;

  if (FALSE == VariablesRead)
  {
    VariablesRead = TRUE;
    /*We are supposed to read only once. if the variable doesn't exist, make assumption on the value to treat
      variable not being there as expected condition and cache the expectation instead. We are memsetting the
      structure variable and so by default it is disabled
    */
    Status = gRT->GetVariable(L"PrintChargerAppDbgMsg", &gQcomTokenSpaceGuid, NULL, &VarSize, &Flag);
    if (EFI_SUCCESS == Status )
    {
      gSmartChargerPlatformCfgData.ChargerLibCfgData.dbg_cfg_Data.print_charger_app_dbg_msg = (Flag) ? TRUE : FALSE;
    }
	
    Status = gRT->GetVariable(L"DISABLEBATTERY", &gOSAVendorGuid, NULL, &VarSize, &Flag);
    if (EFI_SUCCESS == Status )
    {
      gSmartChargerPlatformCfgData.ChargerLibCfgData.schg_cfg_data.DISABLEBATTERY = (Flag) ? TRUE : FALSE;
    }
  }

  return EFI_SUCCESS;
}

/**
SmartChargerPlatform_GetConfigData()

@brief
Returns config data
*/
EFI_STATUS SmartChargerPlatform_GetConfigData
(
  SMART_CHARGER_PLATFORM_CFGDATA_TYPE *pChargerPlatformCfgData
)
{

  if(!pChargerPlatformCfgData)
    return EFI_INVALID_PARAMETER;

  CopyMemS(pChargerPlatformCfgData, sizeof(SMART_CHARGER_PLATFORM_CFGDATA_TYPE), &gSmartChargerPlatformCfgData, sizeof(SMART_CHARGER_PLATFORM_CFGDATA_TYPE));

  return EFI_SUCCESS;
}


/**
SmartChargerPlatform_ReadCfgParams()

@brief
Battery Parameter Default Configurations file read Call Back
*/
VOID
SmartChargerPlatform_ReadCfgParams
(
  UINT8* Section,
  UINT8* Key,
  UINT8* Value
)
{

  //SMARTCHARGER_DEBUG(( EFI_D_INFO, "Section = \"%s\", Key = \"%s\", Value = \"%s\"\n", Section, Key, Value));

  if (AsciiStriCmp ((CHAR8*)Section, "PlatformCommon") == 0) //Common
  {
    if (AsciiStriCmp ((CHAR8*)Key, "CfgVersion") == 0)
    {
    gSmartChargerPlatformCfgData.CfgVersion = SmartChargerPlatformFile_AsciiToInt((char *)Value);
    return;
    }

    if (AsciiStriCmp ((CHAR8*)Key, "OsStandardBootSocThreshold") == 0)
    {
    gSmartChargerPlatformCfgData.ChargerLibCfgData.boot_to_hlos_threshold_in_soc = SmartChargerPlatformFile_AsciiToInt((char *)Value);
    return;
    }

    if (AsciiStriCmp ((CHAR8*)Key, "EnableChargerWdog") == 0)
    {
    gSmartChargerPlatformCfgData.ChargerLibCfgData.enable_charger_wdog = SmartChargerPlatformFile_AsciiToInt((char *)Value);
    return;
    }
 
    if (AsciiStriCmp((CHAR8 *)Key, "ChargingTermCurrent") == 0)
    {
    gSmartChargerPlatformCfgData.ChargerLibCfgData.schg_cfg_data.ChargingTermCurrent = SmartChargerPlatformFile_AsciiToInt((char *)Value);
    }

    if (AsciiStriCmp ((CHAR8*)Key, "SWThermalMitigationEnable") == 0)
    {
    gSmartChargerPlatformCfgData.ChargerLibCfgData.sw_thermal_mitigation_enable = ( AsciiStriCmp ((CHAR8*)Value, "TRUE") == 0 );
    return;
    }

    if (AsciiStriCmp((CHAR8 *)Key, "BattVoltLimHighDelta") == 0)
    {
    gSmartChargerPlatformCfgData.ChargerLibCfgData.batt_volt_lim_high_delta = SmartChargerPlatformFile_AsciiToInt((char *)Value);
    return;
    }

    if (AsciiStriCmp ((CHAR8*)Key, "PrintChargerAppDbgMsg") == 0)
    {
    gSmartChargerPlatformCfgData.ChargerLibCfgData.dbg_cfg_Data.print_charger_app_dbg_msg = ( AsciiStriCmp ((CHAR8*)Value, "TRUE") == 0 );
    return;
    }

    if (AsciiStriCmp ((CHAR8*)Key, "PrintChargerAppDbgMsgToFile") == 0)
    {
    gSmartChargerPlatformCfgData.ChargerLibCfgData.dbg_cfg_Data.print_charger_app_dbg_msg_to_file= ( AsciiStriCmp ((CHAR8*)Value, "TRUE") == 0 );
    return;
    }

    if (AsciiStriCmp((CHAR8*)Key, "FileLoggingDbgLevelMask") == 0)
    {
    gSmartChargerPlatformCfgData.ChargerLibCfgData.dbg_cfg_Data.file_log_level_mask = SmartChargerPlatformFile_AsciiStrToHex((CHAR8 *)Value, AsciiStrLen((CHAR8 *)Value));
    return;
    }

    if (AsciiStriCmp ((CHAR8*)Key, "NoChargeAndWait") == 0)
    {
    gSmartChargerPlatformCfgData.ChargerLibCfgData.no_charge_and_wait = ( AsciiStriCmp ((CHAR8*)Value, "TRUE") == 0 );
    return;
    }

    if (AsciiStriCmp ((CHAR8*)Key, "EmergencyShutdownVbatt") == 0)
    {
    gSmartChargerPlatformCfgData.ChargerLibCfgData.emergency_shutdown_vbatt = SmartChargerPlatformFile_AsciiToInt((char *)Value);
    return;
    }

    if (AsciiStriCmp ((CHAR8*)Key, "DebugBoardBehavior") == 0)
    {
    gSmartChargerPlatformCfgData.ChargerLibCfgData.debug_board_behavior= SmartChargerPlatformFile_AsciiToInt((char *)Value);
    return;
    }

    if (AsciiStriCmp ((CHAR8*)Key, "TsensExtremeTemp") == 0)
    {
    gSmartChargerPlatformCfgData.ChargerLibCfgData.thermal_configs.tsens_extreme_Temp = SmartChargerPlatformFile_AsciiToInt((char *)Value);
    return;
    }

    if (AsciiStriCmp ((CHAR8*)Key, "TsensHighTemp") == 0)
    {
    gSmartChargerPlatformCfgData.ChargerLibCfgData.thermal_configs.tsens_high_temp = SmartChargerPlatformFile_AsciiToInt((char *)Value);
    return;
    }

    if (AsciiStriCmp ((CHAR8*)Key, "TsensLowTemp") == 0)
    {
    gSmartChargerPlatformCfgData.ChargerLibCfgData.thermal_configs.tsens_low_temp = SmartChargerPlatformFile_AsciiToInt((char *)Value);
    return;
    }

    if (AsciiStriCmp ((CHAR8*)Key, "TsensTimeoutMins") == 0)
    {
    gSmartChargerPlatformCfgData.ChargerLibCfgData.thermal_configs.tsens_timeout_mins = SmartChargerPlatformFile_AsciiToInt((char *)Value);
    return;
    }
 	
    if (AsciiStriCmp ((CHAR8*)Key, "ChgFccMax") == 0)
	{
	gSmartChargerPlatformCfgData.ChargerLibCfgData.schg_cfg_data.ChgFccMax = SmartChargerPlatformFile_AsciiToInt((char *)Value);
	return;
    }

	if (AsciiStriCmp ((CHAR8*)Key, "ChgFvMax") == 0)
	{
	gSmartChargerPlatformCfgData.ChargerLibCfgData.schg_cfg_data.ChgFvMax = SmartChargerPlatformFile_AsciiToInt((char *)Value);;
	return;
	}
	
	if (AsciiStriCmp ((CHAR8*)Key, "BatteryUnlockTime") == 0)
	{
	gSmartChargerPlatformCfgData.ChargerLibCfgData.schg_cfg_data.BatteryUnlockTime = SmartChargerPlatformFile_AsciiToInt((char *)Value);;
	return;
	}	

    if (AsciiStriCmp ((CHAR8*)Key, "SWRoleSelectEnable") == 0)
    {
      gSmartChargerPlatformCfgData.ChargerLibCfgData.schg_cfg_data.SWRoleSelectEnable = (AsciiStriCmp ((CHAR8*)Value, "TRUE") == 0);
      return;
    }
  }
  return;
}


EFI_STATUS SmartChargerPlatform_SaveChargeInfoToSmem(pm_chg_info_type *ChargerActionInfo)
{
  EFI_STATUS Status = EFI_SUCCESS;
  UINT32     buf_size = sizeof(pm_chg_info_type);
  UINT32     buf_size_ret = 0;
  pm_chg_info_type *pSmemChargerInfo = NULL, *pSmemChargerInfoRet = NULL;

  pSmemChargerInfo = (pm_chg_info_type *) smem_get_addr(SMEM_RESERVED_BATT_INFO, &buf_size_ret);
  if(!pSmemChargerInfo)
  {
    SMARTCHARGER_DEBUG((EFI_D_WARN, "SmartChargerDxe:: %a Retrieve Error.. XBL Core allocating SMEM now \r\n",__FUNCTION__));
      /* Assume Smem is already initilized in XBL Loader - No need to alloc as XBL loader will alloc for Core keeping this as debug code */
    pSmemChargerInfo = (pm_chg_info_type *)smem_alloc(SMEM_RESERVED_BATT_INFO, buf_size);
    if(!pSmemChargerInfo)
    {
      SMARTCHARGER_DEBUG((EFI_D_WARN, "SmartChargerDxe:: %a smem_alloc Error \r\n",__FUNCTION__));
      return EFI_DEVICE_ERROR;
    }
    else
    {
      SMARTCHARGER_DEBUG((EFI_D_WARN, "SmartChargerDxe:: %a SMEM Allocated \r\n",__FUNCTION__));
      /* Initialized to -1 since SBL did not allocate smem yet */
      pSmemChargerInfo->version       = 1;
      pSmemChargerInfo->sbl_entry_mV  = SMART_CHARGER_INVALID_VALUE_MARKER;
      pSmemChargerInfo->sbl_entry_soc = SMART_CHARGER_INVALID_VALUE_MARKER;
    }
  }

  /* Assign required values to smem memory */
  pSmemChargerInfo->uefi_entry_mV  = ChargerActionInfo->uefi_entry_mV;
  pSmemChargerInfo->uefi_exit_mV   = ChargerActionInfo->uefi_exit_mV;
  pSmemChargerInfo->uefi_entry_soc = ChargerActionInfo->uefi_entry_soc;
  pSmemChargerInfo->uefi_exit_soc  = ChargerActionInfo->uefi_exit_soc;

  buf_size_ret = 0;

  pSmemChargerInfoRet = (pm_chg_info_type *) smem_get_addr(SMEM_RESERVED_BATT_INFO, &buf_size_ret);
  if(!pSmemChargerInfoRet){
    SMARTCHARGER_DEBUG((EFI_D_WARN, "SmartChargerDxe:: %a Retrieve Error 2 \r\n",__FUNCTION__));
    return EFI_DEVICE_ERROR;
  }

  SMARTCHARGER_DEBUG((EFI_D_WARN, "SmartChargerDxe:: buf_size_ret = %d uefi_entry_mV = %d uefi_exit_mV = %d uefi_entry_soc = %d \r\n", buf_size_ret, pSmemChargerInfoRet->uefi_entry_mV, pSmemChargerInfoRet->uefi_exit_mV, pSmemChargerInfoRet->uefi_entry_soc));
  SMARTCHARGER_DEBUG((EFI_D_WARN, "SmartChargerDxe:: sbl_entry_mV = %d sbl_entry_soc = %d ueft_exit_soc = %d \r\n", pSmemChargerInfoRet->sbl_entry_mV, pSmemChargerInfoRet->sbl_entry_soc, pSmemChargerInfoRet->uefi_exit_soc));

  pSmemChargerInfoRet = NULL;
  pSmemChargerInfo    = NULL;

  return Status;

}

EFI_STATUS SmartChargerPlatform_CheckIfStrongCharger
(
  BOOLEAN *bStrongCharger
)
{
  EFI_STATUS                     Status = EFI_SUCCESS;
  EFI_USB_PWR_CTRL_PD_STATUS  PDStatus;
  EFI_USB_PWR_CTRL_TYPEC_PORT_STATUS TypeCPortStatus;
  UINT32 Wattage=0;

	*bStrongCharger = FALSE;
	Status = gBS->LocateProtocol( &gQcomUsbPwrCtrlProtocolGuid, NULL, (VOID **)&pUsbPwrCtrlProtocol );
	if(EFI_SUCCESS != Status || !pUsbPwrCtrlProtocol)
	{
	  SMARTCHARGER_DEBUG(( EFI_D_ERROR, "SmartChargerDxe:: %a Can not locate USB PwrCtrl Protocol = %r \r\n", __FUNCTION__, Status));
	  return EFI_DEVICE_ERROR;
	}
	Status = pUsbPwrCtrlProtocol->GetTypeCPortPDStatus (EFI_USB_PWR_CTRL_USBPORT_CHARGER, &PDStatus);
	if (EFI_SUCCESS == Status && PDStatus.bPDConsumer && PDStatus.PDConsumerState.MaxCurrentMA)
	{
		Wattage = PDStatus.PDConsumerState.MaxCurrentMA * PDStatus.PDConsumerState.MaxVoltageMV;
	}
	else
	{
	  Status = pUsbPwrCtrlProtocol->GetTypeCPortStatus (EFI_USB_PWR_CTRL_USBPORT_CHARGER, &TypeCPortStatus);
	  if (TypeCPortStatus.DetectedSinkType == EFI_USB_PWR_CTRL_TYPEC_PORT_SNK_3P0)
	  {
		  Wattage = MAX_15_W; //5V * 3A
	  }
	}
	if (Wattage >= MAX_15_W)
	{
	  *bStrongCharger = TRUE;	
	}
	return EFI_SUCCESS;
}

EFI_STATUS SmartChargerPlatform_CheckIfOkToBoot
(
  EFI_QCOM_CHARGER_ACTION_TYPE *pActionType, 
  SMART_CHARGER_BATT_STATUS_INFO      CurrentBatteryStatus,
  BOOLEAN  bPowerKeyPressed
)
{
	EFI_STATUS                     Status = EFI_SUCCESS;
	BOOLEAN bStrongCharger;
	EFI_QCOM_PMIC_PON_PROTOCOL           *PmicPonProtocol = NULL;
	EFI_PM_PON_REASON_TYPE PM_PonReasonType;
  
	/* Check if it is a reboot scenario */
	Status = gBS->LocateProtocol( &gQcomPmicPonProtocolGuid,
								 NULL,
								(VOID **)&PmicPonProtocol
	);

	if (Status != EFI_SUCCESS)
	{
		SMARTCHARGER_DEBUG(( EFI_D_ERROR, "SmartChargerDxe:: %a Error locating PmicPonProtocol \r\n", __FUNCTION__));
   		return Status;
	}
	Status = PmicPonProtocol->GetPonReason(PM_DEVICE_0 ,&PM_PonReasonType);
	if (Status != EFI_SUCCESS)
	{
		SMARTCHARGER_DEBUG(( EFI_D_ERROR, "SmartChargerDxe:: %a Error getting PON reason \r\n", __FUNCTION__));
   		return Status;
	}
	SMARTCHARGER_DEBUG(( EFI_D_WARN, "SmartChargerDxe:: %a PM_PonReasonType : 0x%x \r\n", __FUNCTION__, PM_PonReasonType));	 	 

	Status = SmartChargerPlatform_CheckIfStrongCharger(&bStrongCharger);

	/* Good to boot if:
	 * DISABLE_BATTERY is set
	 */
	if(gSmartChargerPlatformCfgData.ChargerLibCfgData.schg_cfg_data.DISABLEBATTERY) 
	{
		SMARTCHARGER_DEBUG(( EFI_D_WARN, "SmartChargerDxe:: %a GOOD_TO_BOOT: DISABLE_BATTERY is set \r\n", __FUNCTION__));	 	 
		*pActionType = EFI_QCOM_CHARGER_ACTION_GOOD_TO_BOOT;	
	}
	else if(bStrongCharger || (CurrentBatteryStatus.StateOfCharge >= gThresholdSoc))
	{
		/*
		 * If Power key is pressed, reset. We will check the PON reason in
		 * the next cycle, and determine if we could boot to HLOS.
		 * This is a workaround for the issue of "BDS menu not displayed on screen"
		 */ 
		if(bPowerKeyPressed)
		{
			SMARTCHARGER_FILE_UART_DEBUG((EFI_D_WARN, "SmartChargerDxe::%a Power Key pressed, performing Reset \r\n", __FUNCTION__));
			*pActionType = EFI_QCOM_CHARGER_ACTION_RESET_COLD;
			return Status;
		}
		else if(PM_PonReasonType.KPDPWR || PM_PonReasonType.HARD_RESET)
		{
			/* Good to boot if:
			 * PON Reason is Power Key or HARD_RESET, and we have a strong charger or a good battery 
			 */
	  		SMARTCHARGER_FILE_UART_DEBUG(( EFI_D_WARN, "SmartChargerDxe:: %a GOOD_TO_BOOT: PM_PonReasonType.KPDPWR = %d, PM_PonReasonType.HARD_RESET = %d, \r\n", __FUNCTION__, PM_PonReasonType.KPDPWR, PM_PonReasonType.HARD_RESET));	 	 
		
	  *pActionType = EFI_QCOM_CHARGER_ACTION_GOOD_TO_BOOT;	
	}
	}
	return Status;
 
}

EFI_STATUS SmartChargerPlatformFile_GetChargerConfig(EFI_QCOM_CHARGER_CONFIG_KEY ChargerCfgKey, UINT32 *KeyValue)
{
  EFI_STATUS Status = EFI_SUCCESS;

  if(FALSE == gSmartChargerCfgInitialized)
  {
    return EFI_DEVICE_ERROR;
  }

  if(NULL == KeyValue)
  {
    return EFI_INVALID_PARAMETER;
  }

  switch(ChargerCfgKey)
  {
    case EFI_QCOM_CHARGER_CONFIG_KEY_SW_FLASH_VOLTAGE:
      *KeyValue = gSmartChargerPlatformCfgData.SWFlashMinBattVoltageMv;
    break;
    default:
      Status = EFI_INVALID_PARAMETER;
    break;
  }

  return Status;
}


