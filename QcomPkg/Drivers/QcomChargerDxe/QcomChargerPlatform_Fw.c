/** @file ChargerPlatform.c

  Implements Qualcomm's battery voltage based threshold Charging solution

  Copyright (c) 2017-2022,  Qualcomm Technologies Inc. All rights reserved.

  **/

/*=====================================================================================
                              EDIT HISTORY


 when         who     what, where, why
 --------     ---     -----------------------------------------------------------------
 04/26/20     ravi    Added support for power up with no battery
 08/09/19     mr      Fix offmode charging implementation for SEC_MOR feature
 05/24/19     mr      Add Config key for BATT_ID_INFO for Qcom Charger
 05/02/19     cs      For CDP also to pick up MTP configurations to avoid invalid temperature range
 02/15/19     sg      Enabling End of Charge (EOC) RTC Alarm setting
 01/29/19     cs      when forcePowerTesting, even if disableBattery flag is set, need to stay in UEFI
 01/17/19     pbitra  Added support to notify user when charger PD is missing
 01/16/19     cs      added config item for power testing
 01/11/19     pbitra  Added config item to either shutdown or go to recovery mode when glink communication goes
                      down.
 12/13/18     ivy     support pmic ship mode and update for fg ship mode
 10/30/18     ivy     support battery missing error handling, either shutdown or booting to HLOS
 10/27/18     cs      fixed recovery mode stay in offmode charging and PON with power button in charging loop if usb plugged in until reach threshold
 09/27/18     ivy     support fg ship mode
 09/24/18     cs      enable soc threshold and loading FW by default
 08/24/18     cs      added config item to enable/disable loading charger FW.
 08/02/18     dc      Attempt Load Charger FW and enter Charging Loop when PON is not KYPD and RESET
 07/03/18     sb      Added DISABLEBATTERY flag
 06/29/18     dc      Move PON reading to Library Level
 06/29/18     dc      Support Input Current Status API & Log
 06/28/18     cs      need to support cls plotform recovery mode
 02/22/18     cs      set batt therm coefficient even when not load profile.
 02/09/18     ra      Removing Wipower related code
 11/27/17     ra      Battery Missing config chanegs for SDM855
 11/09/17     ra      Jeita code changes for Pm855B
 07/12/17     cs      remove redundent enum definition for platform charger Action
 06/21/17     dc      Move Platform Check to QcomChargerDxe and support config override
 06/07/17     cs      add configurable delay before enabling BMD
 05/16/17     cs      fixed in no charging wait state, battery status is printing all 0s
 04/24/17     va      Enable Charger App Changes
 03/30/17     ll      Implemented level file-logging
 03/27/17     pbitra  Updated to support section based config for different platforms.
 03/07/17     cs      remove duplicate get errors checking emergency shutdown condition.
 03/09/17     ll      Updated code based on latest smem_mem_type
 02/07/17     sm      add config to enable/disable INOV
 01/31/17     va      Adding Rconn configuration for considering compensation
 01/27/17     sm      Added support for pbs triggered AFP
 01/23/17     sm      CFG parameter Structure changes
 11/10/16     va      Adding Config VBtEmpty threshold, soc update for smem info
 11/09/16     sm      Added changes to disable watchdog before applying the config setting.
 10/19/16     sm      Removed EnableHvdcp cfg param from ChargerPlatform_ReadCfgParams()
 10/17/16     sm      Added SetChargeCmdBit() API call once CHARGERLIB_CHG_WDOG_STS_BITE_CHARGING_DISABLED is detected.
 10/14/16     va      adding aux coffs settings
 10/06/16     cs      configure batt therm/aux therm bias wait
 09/29/16     va      Emergency shutdown behavior changes and debug log update
 09/13/16     sm      removed unused parameters from ChargerPlatform_ReadCfgParams()
 09/06/16     cs      wipower changes to remove 3.5 sec, not suspend DCIn
 08/24/16     sm      Added changes for supporting different config parameters settings for
                      different platforms, MTP, QRD, etc.
 08/12/16     sm      Merged Vbatt and SOC based Platform files.
 08/05/16     va      Adding Charger led indication support
 07/26/16     va      Restarting FG Changes on warm boot
 07/15/16     cs      Adding Wipower support
 06/28/16     va      Changes to share charger info to smem
 06/24/16     sm      Added check for charging enabled in GetChargingAction()
 06/23/16     sm      Added EnableHvdcp in ChargerPlatform_ReadCfgParams()
 06/23/16     va      Adding support for Charger Fg Peripheral dumps
 06/14/16     va      Adding Thermal Support
 06/09/16     sm      Added SupportHostMode in ChargerPlatform_ReadCfgParams()
 05/31/16     va      Adding Battery profile load feature
 05/24/16     sm      Added changes to handle No_Charger_wait case in GetChargingAction()
 05/10/16     sm      Added ChargerPlatform_GetConfigData()
 04/26/16     va      Adding parameters for Aux Coffes, SkinHot and Charger Hot settings
 04/21/16     sm      Added API call for Watchdog
                      Added EnableChargerWdog in ChargerPlatform_ReadCfgParams
 04/12/16     sm      Edited variables in ChargerPlatform_ReadCfgParams()
 04/07/16     sm      Added ChargerPlatform_GetUefiVariableValues()
 03/31/16     sm      Completing API definitions
 03/28/16     va      Add logging CHARGER_DEBUG
 03/21/16     va      Remove dummy configs and add battery missing config
 01/27/16     sm      Initial revision

                              =============================================================================*/
#include <Uefi.h>

/**
  EFI interfaces
  */

#include <Library/UefiLib.h>
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

#include <Protocol/EFIPmicGlink.h>
#include <Protocol/EFIPmicRTC.h>
#include <Protocol/EFIPIL.h>
#include <Library/SerialPortShLib.h>

/**
  EFI interfaces
  */
#include <Protocol/GraphicsOutput.h>
#include <Protocol/EFIPlatformInfo.h>
#include <Protocol/EFIUsbPwrCtrl.h>

#include "QcomChargerPlatform_File.h"

#include <api/pmic/pm/pm_chg.h>

/**
Smem Interface
*/
#include "smem.h"

/*===========================================================================*/
/*                  FUNCTIONS PROTOTYPES                                     */
/*===========================================================================*/
#define QCOM_CHARGER_IDLE_WAIT_DURATION            3000 /*3 sec in milliseconds*/
#define QCOM_CHARGER_MS_TO_S                       1000

/* UI would be displayed if VBAT read is less than 3700mV is 7% Soc.
   Having a margin of extra 50mV to consider on the top of error margin */
#define QCOM_CHARGER_VBAT_ERROR_MARGIN 0 /*in Mv as of now keeping default as 0 error margin */
#define QCOM_CHARGER_LOW_THERSHOLD_VBAT 3700 /*in Mv*/
#define QCOM_CHARGER_INVALID_VALUE_MARKER  -1
#define CHARGER_FW_MODE 1
#define CHARGER_FW_MISSING 2
#define GLINK_COMM_RETRY_COUNT 10
#define THRESHOLD_CHARGING_HARDRESET_VBATT_MARGIN  20
#define THRESHOLD_CHARGING_HARDRESET_SOC_MARGIN    1

// enable off mode sourcing only when charger is present (UEFI in off-mode charging loop)
#define CHARGERLIB_OFF_MODE_SRC_EN_CHG_PRESENCE     0x0
// disable off mode sourcing 
#define CHARGERLIB_OFF_MODE_SRC_DIS                 0x40
// enable off mode sourcing regardless of charger presence (HW autonomous)
#define CHARGERLIB_OFF_MODE_SRC_EN_ALWAYS           0x80

STATIC UINT32 gThresholdVbatt = 0;
STATIC UINT32 gThresholdSoc = 0;

BOOLEAN gEnableRtcAlarm = FALSE;
UINT32 gRtcAlarmTimeInSec = 0;

STATIC EFI_QCOM_PMIC_RTC_PROTOCOL *PmicRtcProtocol = NULL;

QCOM_CHARGER_PLATFORM_CFGDATA_TYPE gChargerPlatformCfgData;

extern BOOLEAN ExitChargingLoop;
BOOLEAN gChargingInitialized = FALSE;
BOOLEAN gGoingToBoot = FALSE;
extern BOOLEAN gIsUSBChargerConnected;


pm_chg_info_type gChargerSharedInfo;

STATIC EFI_PLATFORMINFO_PLATFORM_TYPE PlatformType;
STATIC BOOLEAN gChargerCfgInitialized = FALSE;
UINT8 ShipModeExit = 0;

EFI_STATUS ChargerPlatform_GetUefiVariableValues(void);
EFI_STATUS ChargerPlatform_SaveChargeInfoToSmem(pm_chg_info_type *ChargerActionInfo);
EFI_STATUS ChargerPlatform_ProfileLoadingInit(EFI_QCOM_CHARGER_ACTION_TYPE *pActionType, QCOM_CHARGER_PLATFORM_ACTION_INFO *pChargerActionInfo, CHARGERLIB_CHARGING_ERROR_TYPES errType);
EFI_STATUS ChargerPlatform_CheckIfOkToBoot(EFI_QCOM_CHARGER_ACTION_TYPE *pActionType, QCOM_CHARGER_PLATFORM_ACTION_INFO *pChargerActionInfo, QCOM_CHARGER_BATT_STATUS_INFO             CurrentBatteryStatus);
EFI_STATUS ChargerPlatform_HandleShipMode(void);
EFI_STATUS ChargerPlatform_LinkChargerFW(void);
EFI_STATUS ChargerPlatform_LoadChargerFW(void);
EFI_STATUS ChargerPlatform_ChkChgFwLoadRequired(EFI_QCOM_CHARGER_ACTION_TYPE *pActionType, QCOM_CHARGER_PLATFORM_ACTION_INFO *pChargerActionInfo);
EFI_STATUS ChargerPlatform_HandleRecovery();
EFI_STATUS ChargerPlatform_SetEOCAlarm(EFI_QCOM_CHARGER_ACTION_TYPE *pActionType, QCOM_CHARGER_PLATFORM_ACTION_INFO *pChargerActionInfo);
EFI_STATUS ChargerPlatform_HandleOffModeSrc(void);

/*===========================================================================*/
/*                  FUNCTIONS PROTOTYPES                                     */
/*===========================================================================*/
/*
ChargerPlatform_Init(): This function locates and initializes ADC Protocol, Charger Protocol and other Protocols that are
needed for that specific platform. It also loads the cfg file and initializes charger and FG accordingly.
*/
EFI_STATUS ChargerPlatform_Init(VOID)
{
  EFI_STATUS Status = EFI_SUCCESS;

  SetMem(&gChargerPlatformCfgData, sizeof(QCOM_CHARGER_PLATFORM_CFGDATA_TYPE), 0x00);
  SetMem(&gChargerSharedInfo, sizeof(pm_chg_info_type), 0x00);

  Status |= GetPlatformType(&PlatformType);
  if (EFI_SUCCESS != Status)
  {
    DEBUG((EFI_D_WARN, "QcomChargerPlatform:: %a Error getting platform type  \r\n", __FUNCTION__));
    return EFI_DEVICE_ERROR;
  }

  //Initialize Charger and Gauge HW Type to be invalid before reading Cfg Data
  gChargerPlatformCfgData.ChargerLibCfgData.plat_hw_cfg_override = CHARGERLIB_PLATFORM_HW_CONFIG_INVALID;

  /* Load CFG file */
  Status |= ChargerPlatformFile_ReadDefaultCfgData();
  if (EFI_SUCCESS == Status)
  {
    CHARGER_DEBUG((EFI_D_WARN, "QcomChargerDxe:: %a Successfully read config file\n\r", __FUNCTION__));
    gChargerCfgInitialized = TRUE;
  }

  /* Read debug var from pmic bds menu */
  Status |= ChargerPlatform_GetUefiVariableValues();

  if ((EFI_SUCCESS == Status) && (TRUE == gChargerPlatformCfgData.ChargerLibCfgData.dbg_cfg_Data.print_charger_app_dbg_msg_to_file))
  {
    Status = ChargerPlatformFile_FileLogInit(gChargerPlatformCfgData);
  }

  CHARGER_DEBUG((EFI_D_WARN, "QcomChargerDxe:: %a chargerlib_cfgdata_type = %d QCOM_CHARGER_PLATFORM_CFGDATA_TYPE = %d \r\n", __FUNCTION__, sizeof(chargerlib_cfgdata_type), sizeof(QCOM_CHARGER_PLATFORM_CFGDATA_TYPE)));

  /* Init Charger lib */
  Status |= ChargerLibCommon_Init((chargerlib_cfgdata_type*)&gChargerPlatformCfgData.ChargerLibCfgData);
  if (EFI_UNSUPPORTED == Status)
  {
    CHARGER_DEBUG((EFI_D_ERROR, "QcomChargerDxe:: %a Error: Charging is not supported for this platform. \r\n", __FUNCTION__));
    return EFI_SUCCESS;
  }
  else if (EFI_SUCCESS != Status)
  {
    CHARGER_DEBUG((EFI_D_ERROR, "QcomChargerDxe:: %a Error when initializing ChargerLibCommon = %r \r\n", __FUNCTION__, Status));
    return Status;
  }
  else
  {
    CHARGER_DEBUG((EFI_D_WARN, "QcomChargerDxe:: %a Successfully initialized ChargerLibCommon %r \r\n", __FUNCTION__, Status));
  }

  if ((EFI_SUCCESS == Status) && (TRUE == gChargerPlatformCfgData.ChargerLibCfgData.shipmode_cfg.enable_shipmode))
  {
    Status = ChargerPlatform_HandleShipMode();
  }

  if (EFI_SUCCESS == Status)
  {
    Status = ChargerPlatform_HandleOffModeSrc();
  }

  if (((EFI_PLATFORMINFO_TYPE_MTP == PlatformType) || (EFI_PLATFORMINFO_TYPE_CDP == PlatformType))
    && (!gChargerPlatformCfgData.ChargerLibCfgData.ForceOffmodePowerTest))
  {
    /* For MTP/CDP, should not load ADSP for any PON reason, just boot to HLOS directly */
    CHARGER_DEBUG((EFI_D_WARN, "QcomChargerDxe:: %a MTP/CDP platform - boot to hlos \n\r", __FUNCTION__));
    return Status;
  }

  gThresholdVbatt = gChargerPlatformCfgData.ChargerLibCfgData.boot_to_hlos_threshold_in_mv;
  gThresholdSoc = gChargerPlatformCfgData.ChargerLibCfgData.boot_to_hlos_threshold_in_soc;

  /* Enable HW JEITA*/
  Status |= ChargerLib_EnableHWJeita(TRUE);

  /*Disable Charger wdog before applying config based wdog setting*/
  Status = ChargerLib_EnableWdog(FALSE);

  if ((QCOM_CHG_WDOG_DISABLE_ON_EXIT == gChargerPlatformCfgData.ChargerLibCfgData.enable_charger_wdog) || (QCOM_CHG_WDOG_LEAVE_ENABLED_ON_EXIT == gChargerPlatformCfgData.ChargerLibCfgData.enable_charger_wdog))
  {
    /* Enable Charger Watchdog*/
    Status = ChargerLib_EnableWdog(TRUE);
    if (EFI_SUCCESS == Status)
    {
      CHARGER_DEBUG((EFI_D_ERROR, "QcomChargerDxe:: %a Enabled WDOG.\r\n", __FUNCTION__));
    }
    else if (EFI_UNSUPPORTED == Status)
    {
      /*Do not need to return status as unsupported. Debug statement will be printed from SCG protocol API*/
      Status = EFI_SUCCESS;
    }
  }

  if (!PmicRtcProtocol) {
     Status = gBS->LocateProtocol( &gQcomPmicRtcProtocolGuid, NULL, (VOID **)&PmicRtcProtocol );
     if(EFI_SUCCESS != Status || !PmicRtcProtocol)
     {
       CHARGER_DEBUG(( EFI_D_WARN, "QcomChargerDxe:: %a Can not locate Pmic Rtc Protocol = %r \r\n", __FUNCTION__, Status));
       return EFI_DEVICE_ERROR;
     }
   }

    //Disable RTC Alarm (to take care of alarm being set at EOC)
    Status = PmicRtcProtocol->AlarmEnable(FALSE);
    if(EFI_SUCCESS != Status || !PmicRtcProtocol)
    {
       CHARGER_DEBUG(( EFI_D_WARN, "QcomChargerDxe:: %a Can not disable RTC alarm = %r \r\n", __FUNCTION__, Status));
       return EFI_DEVICE_ERROR;
    }
    else {
       CHARGER_DEBUG(( EFI_D_WARN, "QcomChargerDxe:: %a Disabled RTC alarm = %r \r\n", __FUNCTION__, Status));
    }

  return Status;
}

void ChargerPlatform_CheckChargerFWStatus(void)
{
  UINT8      ArgValue = 0;
  UINTN      ArgSize = sizeof(ArgValue);
  EFI_STATUS                    NVStatus = EFI_SUCCESS;

  /* Check for ChargerFWMissing */
  NVStatus = gRT->GetVariable(L"ChargerFWMissing", &gQcomTokenSpaceGuid, NULL, &ArgSize, &ArgValue);
  CHARGER_DEBUG((EFI_D_WARN, "*************QcomChargerDxe:: %a ChargerFW Missing Status = %r, ArgValue = %d\r\n", __FUNCTION__, NVStatus, ArgValue));

  if ((NVStatus == EFI_SUCCESS) && ArgValue && (gChargerPlatformCfgData.ChargerLibCfgData.chargerfw_missing_display_time > 0))
  {
    AsciiPrint("ADSP partition is corrupted and need to re-install firmware, otherwise off mode charging is slow \r\n");
    //WaitForTimeoutNoKey (10);
  }
}

EFI_STATUS ChargerPlatform_ChkChgFwLoadRequired(EFI_QCOM_CHARGER_ACTION_TYPE *pActionType, QCOM_CHARGER_PLATFORM_ACTION_INFO *pChargerActionInfo)
{
  EFI_STATUS                    Status = EFI_SUCCESS;
  EFI_PM_PON_REASON_TYPE        PonReasonType = { 0 };
  BOOLEAN                       IsChargingSupported = FALSE;
  chargerlib_batt_status_info   BattStatus = { 0 };
  CHARGERLIB_ATTACHED_CHGR_TYPE     AttachedCharger = CHARGERLIB_ATTACHED_CHGR__NONE;
  CHARGERLIB_PLATFORM_CHGR_MAX_PWR_REQ_CFG_STS_TYPE MaxPwrReqSts;

  if (!pActionType)
    return EFI_INVALID_PARAMETER;
  /* Load ADSP on USB PON for off mode charging and threshold charging - VPH is lower than 3.6V
  */
  Status = ChargerLib_IsChargingSupported(&IsChargingSupported);
  if (Status != EFI_SUCCESS)
  {
    CHARGER_DEBUG((EFI_D_ERROR, "QcomChargerPlatform:: %a Error getting is charging supported = %r \r\n", __FUNCTION__, Status));
    return Status;
  }

  Status = ChargerLib_GetPlatformPwrOnReason(&PonReasonType);
  if (Status != EFI_SUCCESS)
  {
    CHARGER_DEBUG((EFI_D_ERROR, "QcomChargerPlatform:: %a Error getting PON reason = %r \r\n", __FUNCTION__, Status));
    return Status;
  }

  CHARGER_DEBUG((EFI_D_WARN, "QcomChargerPlatform:: %a PonReasonType.KPDPWR = %d,  PonReasonType.HARD_RESET = %d\r\n", __FUNCTION__,
    PonReasonType.KPDPWR, PonReasonType.HARD_RESET));

  /* Charge Source Detection */
  Status = ChargerLib_GetChargingPath(&AttachedCharger);
  if (Status != EFI_SUCCESS)
  {
    CHARGER_DEBUG((EFI_D_ERROR, "QcomChargerPlatform:: %a Error getting charging path = %r \r\n", __FUNCTION__, Status));
    return Status;
  }

  //Check battery missing flag and power negotiation status
  Status = ChargerLib_GetPlatformMaxPwrReqCfgSts(&MaxPwrReqSts);
  if (Status == EFI_SUCCESS && MaxPwrReqSts.batt_missing && MaxPwrReqSts.status != CHGRPD_MAX_PWR_REQ_DONE)
  {
    *pActionType = EFI_QCOM_CHARGER_ACTION_LOAD_CHARGER_FW;
    CHARGER_DEBUG((EFI_D_WARN, "QcomChargerPlatform:: %a Set Action: LOAD_CHARGER_FW, battery missing = %d, power negotiation status = %d \r\n",
                __FUNCTION__,MaxPwrReqSts.batt_missing, MaxPwrReqSts.status));
    return Status;
  }

  //
  //  Ship mode shutdown is considered as abnormal shutdown by TZ
  //  When exiting ship mode and before entering UEFI, TZ issues a hard reset, so the original PON reason is lost
  //  To determine Charger FW load requirement:
  //    1. Normal boot: When PON reason is other than KPDPWR, OR HARD_RESET, load Charger_Fw
  //    2. Exit FG ship mode: assume PON reason is CBLPWR
  //    3. Exit PMIC ship mode
  //        a. USB charger attached:    assume PON reason is CBLPWR
  //        b. No USB charger attached: assume PON reason is KPDPWR
  //

  if (((!ShipModeExit && !PonReasonType.KPDPWR && !PonReasonType.HARD_RESET && !PonReasonType.SMPL)
      || (ShipModeExit == SHIP_MODE_FG_EXIT )
      || (ShipModeExit == SHIP_MODE_PMIC_EXIT && AttachedCharger == CHARGERLIB_ATTACHED_CHGR__USB))
      && IsChargingSupported && (!pChargerActionInfo->bPowerKeyPressed))
  {
    *pActionType = EFI_QCOM_CHARGER_ACTION_LOAD_CHARGER_FW;
    return Status;
  }
  Status = ChargerLib_GetBatteryStatus(&BattStatus);
  if (Status != EFI_SUCCESS)
  {
    CHARGER_DEBUG((EFI_D_ERROR, "QcomChargerPlatform:: %a Error getting battery status = %r \r\n", __FUNCTION__, Status));
    return Status;
  }

  switch (gChargerPlatformCfgData.ChargerLibCfgData.soc_based_boot)
  {
       case FALSE:
       {
              CHARGER_DEBUG((EFI_D_WARN, "QcomChargerDxe:: %a PonReasonType.KPDPWR = %d, VBatt = %d V, gThresholdVbatt = %d V \r\n", __FUNCTION__, PonReasonType.KPDPWR, BattStatus.BatteryVoltage, gThresholdVbatt));

              if (PonReasonType.KPDPWR)
              {
                //not enough voltage to boot, and it is key pad power on, regardless, do shutdown
                if (BattStatus.BatteryVoltage < gThresholdVbatt)
                {
                  *pActionType = EFI_QCOM_CHARGER_ACTION_SHUTDOWN;
                  CHARGER_DEBUG((EFI_D_ERROR, "QcomChargerDxe:: %a keypad power on, vbatt too low, shutting down. \r\n", __FUNCTION__));
                  return Status;
                }
                else
                {
                  *pActionType = EFI_QCOM_CHARGER_ACTION_GOOD_TO_BOOT;
                  CHARGER_DEBUG((EFI_D_ERROR, "QcomChargerDxe:: %a Chg Fw Load not required boot to HLOS \r\n", __FUNCTION__));
                }
              }
              else
              {
                *pActionType = EFI_QCOM_CHARGER_ACTION_GOOD_TO_BOOT; //default action
              }
       }
       break;

       case TRUE:
       {
             if (PonReasonType.KPDPWR || PonReasonType.HARD_RESET)
             {
               CHARGER_DEBUG((EFI_D_WARN, "QcomChargerDxe:: %a PonReasonType.KPDPWR = %d, PonReasonType.HARD_RESET = %d,, soc = %d , gThresholdSoc = %d  \r\n", __FUNCTION__, PonReasonType.KPDPWR, PonReasonType.HARD_RESET, BattStatus.StateOfCharge, gThresholdSoc));
               if ((BattStatus.StateOfCharge) < gThresholdSoc)
               {
                 if (PonReasonType.KPDPWR)
                 {
                   /* User can plug in USB later but we still do reboot to avoid charging without charging FW*/
                   *pActionType = EFI_QCOM_CHARGER_ACTION_SHUTDOWN;
                   CHARGER_DEBUG((EFI_D_ERROR, "QcomChargerDxe:: %a keypad power on and low soc, shutdown\r\n", __FUNCTION__));
                   return Status;
                 }
                 else
                 {
                   //hard reset
                   *pActionType = EFI_QCOM_CHARGER_ACTION_LOAD_CHARGER_FW;
                   CHARGER_DEBUG((EFI_D_ERROR, "QcomChargerDxe:: %a hardreset and low soc, stay in threshold charging\r\n", __FUNCTION__));
                   return Status;
                 }
               }
               else
               {
                 *pActionType = EFI_QCOM_CHARGER_ACTION_GOOD_TO_BOOT;
                 CHARGER_DEBUG((EFI_D_ERROR, "QcomChargerDxe:: %a Chg Fw Load not required boot to HLOS \r\n", __FUNCTION__));
               }
             }
             else if(pChargerActionInfo->bPowerKeyPressed)
             {
                if ((BattStatus.StateOfCharge) < gThresholdSoc)
                {
                  *pActionType = EFI_QCOM_CHARGER_ACTION_LOAD_CHARGER_FW;
                   CHARGER_DEBUG((EFI_D_ERROR, "QcomChargerDxe:: %a hardreset and low soc, stay in threshold charging\r\n", __FUNCTION__));
                   return Status;
                }
                else
                {
                  /* code */
                  *pActionType = EFI_QCOM_CHARGER_ACTION_RESET_COLD;
                  CHARGER_DEBUG((EFI_D_ERROR, "QcomChargerDxe:: %a Chg Fw Load not required boot to HLOS \r\n", __FUNCTION__));
                }
                
             }
             else
             {
               ChargerPlatform_CheckChargerFWStatus();
               *pActionType = EFI_QCOM_CHARGER_ACTION_GOOD_TO_BOOT; //default action
             }
       }
       break;
  }
  return Status;

}


#define QCOM_CHARGER_CHG_FW_HEARTBEAT_CNT_MAX 3

EFI_STATUS ChargerPlatform_GetChargingAction(EFI_QCOM_CHARGER_ACTION_TYPE *pActionType, QCOM_CHARGER_PLATFORM_ACTION_INFO *pChargerActionInfo, BOOLEAN vbattChecking)
{
  EFI_STATUS Status = EFI_SUCCESS;
  BOOLEAN    bChargerReinserted = FALSE;
  CHARGERLIB_CHARGING_ERROR_TYPES      ErrorType = CHARGERLIB_CHARGING_ERROR_NONE;
  STATIC EFI_QCOM_CHARGER_ACTION_TYPE  PrevChargerAction = EFI_QCOM_CHARGER_ACTION_INVALID;
  BOOLEAN                              ChargingEnabled = FALSE;
  //STATIC UINT32                        ChgFwHeartBeatCnt   = 0;
  STATIC UINT32                        gLinkTryCount = 0;

  if (!pChargerActionInfo || !pActionType)
    return EFI_INVALID_PARAMETER;

  if ((EFI_PLATFORMINFO_TYPE_MTP == PlatformType || EFI_PLATFORMINFO_TYPE_CDP == PlatformType
    || gChargerPlatformCfgData.ChargerLibCfgData.dbg_cfg_Data.DISABLEBATTERY) && (!gChargerPlatformCfgData.ChargerLibCfgData.ForceOffmodePowerTest))
  {
    /* For MTP/CDP or if DISABLEBATTERY flag is set, should not load ADSP for any PON reason, just boot to HLOS directly */
    PrevChargerAction = *pActionType = EFI_QCOM_CHARGER_ACTION_GOOD_TO_BOOT;
    return Status;
  }

  if (PrevChargerAction == EFI_QCOM_CHARGER_ACTION_INVALID)
  {
    /* ADSP load required ? */
    Status = ChargerPlatform_ChkChgFwLoadRequired(pActionType, pChargerActionInfo);
    if (Status != EFI_SUCCESS)
    {
      CHARGER_DEBUG((EFI_D_ERROR, "QcomChargerDxe:: %a Error = %r.\r\n", __FUNCTION__, Status));
      *pActionType = EFI_QCOM_CHARGER_ACTION_SHUTDOWN;
    }
    PrevChargerAction = *pActionType;
    return Status;
  }
  else if ((EFI_PLATFORMINFO_TYPE_CLS == PlatformType || EFI_PLATFORMINFO_TYPE_MTP == PlatformType || gChargerPlatformCfgData.ChargerLibCfgData.ForceOffmodePowerTest)
    && (PrevChargerAction == EFI_QCOM_CHARGER_ACTION_RECOVERY))
  {
    *pActionType = EFI_QCOM_CHARGER_ACTION_RECOVERY;
    PrevChargerAction = *pActionType;
    CHARGER_DEBUG((EFI_D_ERROR, "QcomChargerDxe:: %a Chg FW recover state \r\n", __FUNCTION__));
  }
  else if ((gChargerSharedInfo.uefi_charger_fw_mode == CHARGER_FW_MISSING) && (PrevChargerAction != EFI_QCOM_CHARGER_ACTION_RECOVERY))
  {
    *pActionType = EFI_QCOM_CHARGER_ACTION_RECOVERY;
    PrevChargerAction = *pActionType;
    CHARGER_DEBUG((EFI_D_ERROR, "QcomChargerDxe:: %a Chg FW recover state \r\n", __FUNCTION__));
    return Status;
  }

  /* Get Error like debug board or battery not detected first */
  Status |= ChargerLib_GetErrors(vbattChecking, &ErrorType);
  if (EFI_SUCCESS != Status)
  {
    CHARGER_DEBUG((EFI_D_ERROR, "QcomChargerDxe:: %a Error Getting Battery Error = %r.\r\n", __FUNCTION__, Status));
    *pActionType = EFI_QCOM_CHARGER_ACTION_SHUTDOWN;
    PrevChargerAction = *pActionType;
    return Status;
  }

  if ((ErrorType == CHARGERLIB_CHARGING_FW_LINK_ERROR) && (PrevChargerAction != EFI_QCOM_CHARGER_ACTION_RECOVERY))
  {
    CHARGER_DEBUG((EFI_D_ERROR, "QcomChargerDxe:: %a CHG FW LINK Error = %d \r\n", __FUNCTION__, CHARGERLIB_CHARGING_FW_LINK_ERROR));

    if (gChargerPlatformCfgData.ChargerLibCfgData.glink_down_action && (PrevChargerAction != EFI_QCOM_CHARGER_ACTION_RECOVERY))
    {
      *pActionType = EFI_QCOM_CHARGER_ACTION_RECOVERY;
      CHARGER_DEBUG((EFI_D_ERROR, "QcomChargerDxe:: %a *pActionType = %d \r\n", __FUNCTION__, *pActionType));
    }
    else if (!gChargerPlatformCfgData.ChargerLibCfgData.glink_down_action && (PrevChargerAction != EFI_QCOM_CHARGER_ACTION_SHUTDOWN))
    {
      gLinkTryCount++;

      CHARGER_DEBUG((EFI_D_ERROR, "QcomChargerDxe:: %a gLinkTryCount = %d \r\n", __FUNCTION__, gLinkTryCount));
      if (gLinkTryCount >= GLINK_COMM_RETRY_COUNT)
      {
        *pActionType = EFI_QCOM_CHARGER_ACTION_SHUTDOWN;
        CHARGER_DEBUG((EFI_D_ERROR, "QcomChargerDxe:: %a *pActionType = %d \r\n", __FUNCTION__, *pActionType));
      }
      else
      {
        *pActionType = EFI_QCOM_CHARGER_ACTION_NO_CHARGE_WAIT;
        CHARGER_DEBUG((EFI_D_ERROR, "QcomChargerDxe:: %a *pActionType = %d \r\n", __FUNCTION__, *pActionType));

      }
    }
    PrevChargerAction = *pActionType;
    return Status;
  }

    //skip GetErrorAction, if BattMissingAction is good to boot
  if ((CHARGERLIB_CHARGING_ERROR_BATTERY_NOT_DETECTED == ErrorType &&
     gChargerPlatformCfgData.ChargerLibCfgData.fg_cfg_data.FgBattIdCfg.BattMissingAction != CHGRLIB_BATT_MISSING_ACTION_BOOT_TO_HLOS) ||
    (CHARGERLIB_CHARGING_ERROR_DEBUG_BOARD == ErrorType) ||
    (CHARGERLIB_DEVICE_ERROR == ErrorType) || (CHARGERLIB_CHARGING_ERROR_UNKNOWN_BATTERY == ErrorType) ||
    (CHARGERLIB_CHARGING_ERROR_TSENSE_CRITICAL == ErrorType) || (CHARGERLIB_CHARGING_ERROR_TSENSE_TIMEOUT == ErrorType) ||
    (CHARGERLIB_CHARGING_ERROR_TSENSE_HIGH == ErrorType) ||
    (CHARGERLIB_ERROR_ACTION_CRITICAL_SHUTDOWN == ErrorType))
  {
    /* Get Battery Status for caller skip for battery not detecte errortype */
    if (CHARGERLIB_CHARGING_ERROR_BATTERY_NOT_DETECTED != ErrorType)
    {
      Status |= ChargerLib_GetBatteryStatus((chargerlib_batt_status_info*)&(pChargerActionInfo->BattStsInfo));
      if (EFI_SUCCESS != Status)
      {
        CHARGER_DEBUG((EFI_D_WARN, "QcomChargerDxe:: %a Error Getting Battery Status = %r.\r\n", __FUNCTION__, Status));
        /* Continue to get error action in more critical error as in this if condition */
      }
    }
    Status = ChargerLib_GetErrorAction(ErrorType, (((CHARGERLIB_ERROR_ACTION_TYPE*)pActionType)));
    PrevChargerAction = *pActionType;
    CHARGER_DEBUG((EFI_D_WARN, "QcomChargerDxe:: %a pActionType = %d.ErrorType = %d \r\n", __FUNCTION__, *pActionType, ErrorType));
    /*If there is a battery error, return */
    return Status;
  }

  /* Assign Led config to toggle led */
  pChargerActionInfo->LedConfigType = (QCOM_CHARGER_PLATFORM_CHARGING_LED_CONFIG_TYPE)gChargerPlatformCfgData.ChargerLibCfgData.charger_led_config;

  Status |= ChargerLib_GetBatteryStatus((chargerlib_batt_status_info*)&(pChargerActionInfo->BattStsInfo));
  if (EFI_SUCCESS != Status)
  {
    CHARGER_DEBUG((EFI_D_WARN, "QcomChargerDxe:: %a Error Getting Battery Status = %r.\r\n", __FUNCTION__, Status));
    *pActionType = EFI_QCOM_CHARGER_ACTION_STOP_CHARGING;
    PrevChargerAction = *pActionType;

    /*If there is an error, return since action is decided */
    return Status;
  }

    //skip GetErrorAction, if BattMissingAction is good to boot
  if (CHARGERLIB_CHARGING_ERROR_NONE != ErrorType &&
        (!(CHARGERLIB_CHARGING_ERROR_BATTERY_NOT_DETECTED == ErrorType && 
        gChargerPlatformCfgData.ChargerLibCfgData.fg_cfg_data.FgBattIdCfg.BattMissingAction == CHGRLIB_BATT_MISSING_ACTION_BOOT_TO_HLOS)))
  {
    Status = ChargerLib_GetErrorAction(ErrorType, (((CHARGERLIB_ERROR_ACTION_TYPE*)pActionType)));
    PrevChargerAction = *pActionType;

    /*If there is a battery error, return */
    return Status;
  }

  Status = ChargerLib_GetChargingPath(&pChargerActionInfo->ChargerAttached);
  if (EFI_SUCCESS != Status)
  {
    CHARGER_DEBUG((EFI_D_WARN, "QcomChargerDxe:: %a Error Getting Power Path = %r.\r\n", __FUNCTION__, Status));
    *pActionType = EFI_QCOM_CHARGER_ACTION_STOP_CHARGING;
    PrevChargerAction = *pActionType;
    return Status;
  }

  if ((QCOM_CHARGER_PLATFORM_CHARGER_ATTACHED_USB == pChargerActionInfo->ChargerAttached) &&
    ((EFI_QCOM_CHARGER_ACTION_INVALID == PrevChargerAction) ||
    (EFI_QCOM_CHARGER_ACTION_NO_CHARGE_WAIT == PrevChargerAction)))
  {
    *pActionType = EFI_QCOM_CHARGER_ACTION_START_CHARGING;
  }
  else
  {
    Status = ChargerLib_GetChargingStatus(&ChargingEnabled);
    if (EFI_SUCCESS != Status)
    {
      CHARGER_DEBUG((EFI_D_WARN, "ChargerLib::%a Error Getting Charging Status = %r \r\n", __FUNCTION__, Status));
      return Status;
    }

    if (FALSE == ChargingEnabled)
    {
      /*Charger register dump in case need to determine why charging is disabled*/
      Status |= ChargerLib_DumpChargerPeripheral();
    }
    else
    {
      /* Charging already started, go to continue. */
      *pActionType = EFI_QCOM_CHARGER_ACTION_CONTINUE;
      /* Assign Led config to toggle led */
      /* pChargerActionInfo->LedConfigType = (QCOM_CHARGER_PLATFORM_CHARGING_LED_CONFIG_TYPE)gChargerPlatformCfgData.ChargerLibCfgData.charger_led_config;*/
    }

    if (QCOM_CHARGER_PLATFORM_CHARGER_ATTACHED_USB == pChargerActionInfo->ChargerAttached)
    {
      /* Check if charger was swapped/re-inserted */
      Status = ChargerLib_WasChargerReinserted(&bChargerReinserted);
      if (EFI_SUCCESS != Status)
      {
        CHARGER_DEBUG((EFI_D_WARN, "ChargerLib::%a Error = %d Error Checking Charger Re-insertion. \r\n", __FUNCTION__, Status));
        return Status;
      }

      if (bChargerReinserted)
      {
        //Status = ChargerLib_CheckAPSDResults();
        Status = ChargerLib_ReRunAicl();
      }
    }
  }

  Status |= ChargerPlatform_CheckIfOkToBoot(pActionType, pChargerActionInfo, pChargerActionInfo->BattStsInfo);

  CHARGER_DEBUG((EFI_D_WARN, "QcomChargerDxe::%a gGoingToBoot = %d \r\n", __FUNCTION__, gGoingToBoot));

  //Check for End of Charge (EOC) condition if not OkToBoot, and set RTC Alarm if EOC achieved.
  if (!gGoingToBoot) {
    CHARGER_DEBUG((EFI_D_WARN, "QcomChargerDxe::%a gIsUSBChargerConnected = %d \r\n", __FUNCTION__, gIsUSBChargerConnected));

    Status |= ChargerPlatform_SetEOCAlarm(pActionType, pChargerActionInfo);
    if(EFI_SUCCESS != Status)
      {
        CHARGER_DEBUG((EFI_D_WARN, "QcomChargerDxe::%a Error = %d Could not check for EOC. \r\n", __FUNCTION__, Status));
        return Status;
      }
  }

  CHARGER_DEBUG((EFI_D_WARN, "QcomChargerDxe:: %a Action Returned = %d \r\n", __FUNCTION__, *pActionType));

  PrevChargerAction = *pActionType;

  return Status;
}


EFI_STATUS ChargerPlatform_TakeAction(EFI_QCOM_CHARGER_ACTION_TYPE ChargingAction, CONST QCOM_CHARGER_PLATFORM_ACTION_INFO *pChargerActionInfo)
{
  QCOM_CHARGER_BATT_STATUS_INFO CurrentBatteryStatus = { 0 };
  STATIC BOOLEAN bToggleLed = TRUE;
  EFI_STATUS Status = EFI_SUCCESS;
  CHARGERLIB_CHARGER_INPUT_STATUS ChargerInputStatus = { 0 };
  UINT8      ArgValue = 0;
  UINTN      ArgSize = sizeof(ArgValue);
  EFI_STATUS     NVStatus = EFI_SUCCESS;

  if (!pChargerActionInfo)
  {
    CHARGER_DEBUG((EFI_D_WARN, "QcomChargerDxe:: %a Invalid parameter \r\n", __FUNCTION__));
    return EFI_INVALID_PARAMETER;
  }

  CurrentBatteryStatus = (QCOM_CHARGER_BATT_STATUS_INFO)pChargerActionInfo->BattStsInfo;

  switch (ChargingAction)
  {
  case EFI_QCOM_CHARGER_ACTION_CRITICAL:
    //print out and flush critical error messages
    //Perform AFP
    CHARGER_FILE_UART_DEBUG((EFI_D_WARN, "QcomChargerDxe::%a Critical Error occurred. Shutting down \r\n", __FUNCTION__));
    ChargerLib_ForceSysShutdown(CHGAPP_RESET_AFP);
    break;

  case   EFI_QCOM_CHARGER_ACTION_SHUTDOWN:
    //print error message and trigger system shutdown
    //These errors will only be checked and handled when battery voltage is not high enough to boot and uefi charging is needed.
    CHARGER_FILE_UART_DEBUG((EFI_D_WARN, "QcomChargerDxe::%a Waiting for %d s \r\n", __FUNCTION__, QCOM_CHARGER_IDLE_WAIT_DURATION / QCOM_CHARGER_MS_TO_S));
    WaitForTimeout(QCOM_CHARGER_IDLE_WAIT_DURATION, TIMEOUT_WAIT_FOR_KEY, NULL);
    ChargerLib_ForceSysShutdown(CHGAPP_RESET_SHUTDOWN);
    break;

  case EFI_QCOM_CHARGER_ACTION_START_CHARGING:
    /* Assign entry voltage on warm boot and soc after profile is loaded or not loaded as in warm/cold boot */
    if (gChargerSharedInfo.uefi_entry_mV == QCOM_CHARGER_INVALID_VALUE_MARKER)
    {
      gChargerSharedInfo.uefi_entry_mV = CurrentBatteryStatus.BatteryVoltage;
    }
    if (gChargerPlatformCfgData.ChargerLibCfgData.fg_cfg_data.LoadBatteryProfile == TRUE)
    {
      gChargerSharedInfo.uefi_entry_soc = CurrentBatteryStatus.StateOfCharge;
    }
    else
    {
      gChargerSharedInfo.uefi_entry_soc = QCOM_CHARGER_INVALID_VALUE_MARKER;
    }
    CHARGER_DEBUG((EFI_D_WARN, "QcomChargerDxe:: %a Saving Entry VBatt  = %d SOC = %d \r\n", __FUNCTION__,
      CurrentBatteryStatus.BatteryVoltage, CurrentBatteryStatus.StateOfCharge));
    if (QCOM_CHARGER_PLATFORM_CHARGER_ATTACHED_USB == pChargerActionInfo->ChargerAttached)
    {
      ChargerLib_InitializeCharging();
    }
    break;

  case EFI_QCOM_CHARGER_ACTION_STOP_CHARGING:
    if (TRUE == gChargingInitialized)
    {
      ChargerLib_ChargerEnable(FALSE);
      gChargingInitialized = FALSE;
    }
    if (gChargerPlatformCfgData.ChargerLibCfgData.charger_led_config)
    {
      /* Turn Off Charging */
      bToggleLed = FALSE;
      ChargerLib_LedOn(bToggleLed);
    }
    break;

  case EFI_QCOM_CHARGER_ACTION_NO_CHARGE_WAIT:
  case EFI_QCOM_CHARGER_ACTION_TSENSE_HIGH_WAIT:
    ChargerLib_HandleNoChargeAndWait();
    if (gChargerPlatformCfgData.ChargerLibCfgData.enable_charger_wdog)
    {
      /* Pet the watchdog if feature is enabled */
      ChargerLib_PetChgWdog();
    }
    break;

  case EFI_QCOM_CHARGER_ACTION_CONTINUE:
    if (gChargerPlatformCfgData.ChargerLibCfgData.enable_charger_wdog)
    {
      /* Pet the watchdog if feature is enabled */
      ChargerLib_PetChgWdog();
    }
    if (gChargerPlatformCfgData.ChargerLibCfgData.charger_led_config)
    {
      /*  DEBUG ONLY */
      /* CHARGER_DEBUG((EFI_D_WARN, "QcomChargerDxe::%a SWLedToggleConfig = %d \r\n", __FUNCTION__, gChargerPlatformCfgData.ChargerLibCfgData.charger_led_config)); */
      switch (gChargerPlatformCfgData.ChargerLibCfgData.charger_led_config)
      {
      case QCOM_CHARGER_PLATFORM_CHARGING_LED_ON:
        bToggleLed = TRUE; /* Make sure to turn on flag as control can come back from wait state */
        ChargerLib_LedOn(bToggleLed);
        break;
      case QCOM_CHARGER_PLATFORM_CHARGING_LED_TOGGLE:
        ChargerLib_LedOn(bToggleLed);
        bToggleLed = (bToggleLed == TRUE) ? FALSE : TRUE;
        break;
      case QCOM_CHARGER_PLATFORM_CHARGING_LED_OFF:
        /* will not reache here */
      default:
        break;
      }
    }
    else
    {
      /*  DEBUG ONLY */
      /* CHARGER_DEBUG(( EFI_D_WARN, "QcomChargerDxe:: %a Charging Led Indication is off = %d \r\n", __FUNCTION__, gChargerPlatformCfgData.ChargerLibCfgData.charger_led_config)); */
    }
    /* Debug FG SRAM */
    ChargerLib_GetChargerInputStatus(&ChargerInputStatus);
    CHARGER_DEBUG((EFI_D_WARN, "QcomChargerDxe:: %a ICLFinal = %d mA, ICLMax = %d mA\r\n", __FUNCTION__,
      ChargerInputStatus.ICLfinalMa, ChargerInputStatus.ICLMaxMa));
    ChargerLib_DumpSram(FALSE);
    break;

  case EFI_QCOM_CHARGER_ACTION_GOOD_TO_BOOT:
    /* Assign Exit voltage and soc */
    gChargerSharedInfo.uefi_exit_mV = CurrentBatteryStatus.BatteryVoltage;
    if (gChargerPlatformCfgData.ChargerLibCfgData.fg_cfg_data.LoadBatteryProfile == TRUE)
    {
      gChargerSharedInfo.uefi_exit_soc = CurrentBatteryStatus.StateOfCharge;
    }
    else
    {
      gChargerSharedInfo.uefi_exit_soc = QCOM_CHARGER_INVALID_VALUE_MARKER;
    }
    CHARGER_DEBUG((EFI_D_WARN, "QcomChargerDxe:: %a Saving Exit VBat = %d Soc = %d \r\n", __FUNCTION__,
      CurrentBatteryStatus.BatteryVoltage, CurrentBatteryStatus.StateOfCharge));
    gChargerSharedInfo.uefi_charger_fw_mode = 0;
    /* Save Smem Info ignoring return status as XBL loader changes are not ready yet*/
    ChargerPlatform_SaveChargeInfoToSmem(&gChargerSharedInfo);
    if (gChargerPlatformCfgData.DCInCfgData.DCINDisableOnExit)
    {
      //suspend DCIn
      (void)ChargerLib_DcinSuspend(TRUE);
    }

    /* Turn Off Charging */
    bToggleLed = FALSE;
    ChargerLib_LedOn(bToggleLed);
    break;

  case EFI_QCOM_CHARGER_ACTION_SHUTDOWN_USB_DC_PON_DISABLED:
    //print error message and trigger system shutdown
    //These errors will only be checked and handled when battery voltage is not high enough to boot and uefi charging is needed.
    CHARGER_FILE_UART_DEBUG((EFI_D_WARN, "QcomChargerDxe::%a Waiting for %d s \r\n", __FUNCTION__, QCOM_CHARGER_IDLE_WAIT_DURATION / QCOM_CHARGER_MS_TO_S));
    WaitForTimeout(QCOM_CHARGER_IDLE_WAIT_DURATION, TIMEOUT_WAIT_FOR_KEY, NULL);
    ChargerLib_ForceSysShutdown(CHGAPP_RESET_SHUTDOWN_USB_DC_PON_DISABLED);
    break;

  case EFI_QCOM_CHARGER_ACTION_RESET_COLD:
    //print error message and trigger system reset
    //These errors will only be checked and handled when battery voltage is not high enough to boot and uefi charging is needed.
    CHARGER_FILE_UART_DEBUG((EFI_D_WARN, "QcomChargerDxe::%a Waiting for %d s \r\n", __FUNCTION__, QCOM_CHARGER_IDLE_WAIT_DURATION / QCOM_CHARGER_MS_TO_S));
    WaitForTimeout(QCOM_CHARGER_IDLE_WAIT_DURATION, TIMEOUT_WAIT_FOR_KEY, NULL);
    ChargerLib_ForceSysShutdown(CHGAPP_RESET_COLD);
    break;

  case EFI_QCOM_CHARGER_ACTION_LOAD_CHARGER_FW:
    gChargerSharedInfo.uefi_charger_fw_mode = CHARGER_FW_MODE;
    ChargerPlatform_SaveChargeInfoToSmem(&gChargerSharedInfo);
    if (EFI_SUCCESS == ChargerPlatform_LoadChargerFW())
    {
      /*If ChargerFWMissing exists and ChargerFWMissing == TRUE,            set it to FALSE */
      ArgSize = sizeof(ArgValue);
      NVStatus = gRT->GetVariable(L"ChargerFWMissing", &gQcomTokenSpaceGuid,
        NULL, &ArgSize, &ArgValue);
      CHARGER_DEBUG((EFI_D_WARN, "QcomChargerDxe:: %a Get ChargerFWMissing variable Status: %r, ArgValue = %d\r\n", __FUNCTION__, NVStatus, ArgValue));

      if ((NVStatus != EFI_SUCCESS) || (ArgValue == 1))
      {
        ArgValue = 0;
        ArgSize = sizeof(ArgValue);
        NVStatus = gRT->SetVariable(L"ChargerFWMissing", &gQcomTokenSpaceGuid,
          EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_NON_VOLATILE,
          ArgSize, &ArgValue);

        CHARGER_DEBUG((EFI_D_WARN, "QcomChargerDxe:: %a Set ChargerFWMissing variable Status: %r, Flag = %d\r\n", __FUNCTION__, NVStatus, ArgValue));
      }
    }
    else
    {
      CHARGER_DEBUG((EFI_D_WARN, "QcomChargerDxe:: %a ChargerPlatform_LoadChargerFW failed\r\n", __FUNCTION__));

      gChargerSharedInfo.uefi_charger_fw_mode = CHARGER_FW_MISSING;

      /* If ChargerFWMissing does not exist, create ChargerFWMissing and set it to TRUE*/
      ArgSize = sizeof(ArgValue);
      NVStatus = gRT->GetVariable(L"ChargerFWMissing", &gQcomTokenSpaceGuid,
        NULL, &ArgSize, &ArgValue);
      CHARGER_DEBUG((EFI_D_WARN, "QcomChargerDxe:: %a Get ChargerFWMissing variable Status: %r, ArgValue = %d\r\n", __FUNCTION__, NVStatus, ArgValue));

      if ((NVStatus != EFI_SUCCESS) || (ArgValue == 0))
      {
        ArgValue = 1;
        ArgSize = sizeof(ArgValue);
        NVStatus = gRT->SetVariable(L"ChargerFWMissing", &gQcomTokenSpaceGuid,
          EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_NON_VOLATILE,
          ArgSize, &ArgValue);
      }

      Status = EFI_SUCCESS;
    }
    break;

  case EFI_QCOM_CHARGER_ACTION_CONNECT_CHARGER_FW:
    Status = ChargerPlatform_LinkChargerFW();
    break;

  case EFI_QCOM_CHARGER_ACTION_RECOVERY:
    Status = ChargerPlatform_HandleRecovery();

    ChargerLib_GetChargerInputStatus(&ChargerInputStatus);
    CHARGER_DEBUG((EFI_D_WARN, "QcomChargerDxe:: %a ICLFinal = %d mA, ICLMax = %d mA\r\n", __FUNCTION__,
      ChargerInputStatus.ICLfinalMa, ChargerInputStatus.ICLMaxMa));

    break;

  default:
    CHARGER_DEBUG((EFI_D_WARN, "QcomChargerDxe:: %a Action Passed = %d \r\n", __FUNCTION__, ChargingAction));
    break;
  }

  return Status;
}


/**
ChargerPlatform_GetUefiVariableValues()

@brief
Reads UEFI Variables changed via BDS Menu
*/
EFI_STATUS ChargerPlatform_GetUefiVariableValues(VOID)
{
  UINT8          Flag = 0;
  UINTN          VarSize = sizeof(Flag);
  STATIC BOOLEAN VariablesRead = FALSE;
  EFI_STATUS     Status = EFI_SUCCESS;

  if (FALSE == VariablesRead)
  {
    VariablesRead = TRUE;
    /*We are supposed to read only once. if the variable doesn't exist, make assumption on the value to treat
      variable not being there as expected condition and cache the expectation instead. We are memsetting the
      structure variable and so by default it is disabled
      */
    Status = gRT->GetVariable(L"DISABLEBATTERY", &gOSAVendorGuid, NULL, &VarSize, &Flag);
    if (EFI_SUCCESS == Status)
    {
      gChargerPlatformCfgData.ChargerLibCfgData.dbg_cfg_Data.DISABLEBATTERY = (Flag) ? TRUE : FALSE;
    }

    VarSize = sizeof(Flag);
    Status = gRT->GetVariable(L"PrintChargerAppDbgMsg", &gQcomTokenSpaceGuid, NULL, &VarSize, &Flag);
    if (EFI_SUCCESS == Status)
    {
      gChargerPlatformCfgData.ChargerLibCfgData.dbg_cfg_Data.print_charger_app_dbg_msg = (Flag) ? TRUE : FALSE;
    }
  }

  return EFI_SUCCESS;
}

/**
ChargerPlatform_GetConfigData()

@brief
Returns config data
*/
EFI_STATUS ChargerPlatform_GetConfigData
(
QCOM_CHARGER_PLATFORM_CFGDATA_TYPE *pChargerPlatformCfgData
)
{

  if (!pChargerPlatformCfgData)
    return EFI_INVALID_PARAMETER;

  CopyMemS(pChargerPlatformCfgData, sizeof(QCOM_CHARGER_PLATFORM_CFGDATA_TYPE), &gChargerPlatformCfgData, sizeof(QCOM_CHARGER_PLATFORM_CFGDATA_TYPE));

  return EFI_SUCCESS;
}


/**
ChargerPlatform_ReadCfgParams()

@brief
Battery Parameter Default Configurations file read Call Back
*/
VOID
ChargerPlatform_ReadCfgParams
(
UINT8* Section,
UINT8* Key,
UINT8* Value
)
{

  //CHARGER_DEBUG(( EFI_D_INFO, "Section = \"%s\", Key = \"%s\", Value = \"%s\"\n", Section, Key, Value));

  if (AsciiStriCmp((CHAR8*)Section, "PlatformCommon") == 0) //Common
  {
    if (AsciiStriCmp((CHAR8*)Key, "CfgVersion") == 0)
    {
      gChargerPlatformCfgData.CfgVersion = ChargerPlatformFile_AsciiToInt((char *)Value);
      return;
    }

    if (AsciiStriCmp((CHAR8*)Key, "EnableChargerFW") == 0)
    {
      gChargerPlatformCfgData.ChargerLibCfgData.enable_charger_fw = ChargerPlatformFile_AsciiToInt((char *)Value);
      return;
    }
    if (AsciiStriCmp((CHAR8*)Key, "SWFlashMinBattVoltageMv") == 0)
    {
      gChargerPlatformCfgData.SWFlashMinBattVoltageMv = ChargerPlatformFile_AsciiToInt((char *)Value);
      return;
    }

    if (AsciiStriCmp((CHAR8*)Key, "BootToHLOSThresholdInMv") == 0)
    {
      gChargerPlatformCfgData.ChargerLibCfgData.boot_to_hlos_threshold_in_mv = ChargerPlatformFile_AsciiToInt((char *)Value);
      return;
    }

    if (AsciiStriCmp((CHAR8*)Key, "OsStandardBootSocThreshold") == 0)
    {
      gChargerPlatformCfgData.ChargerLibCfgData.boot_to_hlos_threshold_in_soc = ChargerPlatformFile_AsciiToInt((char *)Value);
      return;
    }

    if (AsciiStriCmp((CHAR8*)Key, "SocBasedBoot") == 0)
    {
      gChargerPlatformCfgData.ChargerLibCfgData.soc_based_boot = (AsciiStriCmp((CHAR8*)Value, "TRUE") == 0);
      return;
    }

    if (AsciiStriCmp((CHAR8*)Key, "EnableChargerWdog") == 0)
    {
      gChargerPlatformCfgData.ChargerLibCfgData.enable_charger_wdog = ChargerPlatformFile_AsciiToInt((char *)Value);
      return;
    }

    if (AsciiStriCmp((CHAR8*)Key, "DispSignOfLifeMaxThresholdMv") == 0)
    {
      gChargerPlatformCfgData.DispSignOfLifeMaxThresholdMv = ChargerPlatformFile_AsciiToInt((char *)Value);
      return;
    }

    if (AsciiStriCmp((CHAR8*)Key, "ChargerLedConfig") == 0)
    {
      gChargerPlatformCfgData.ChargerLibCfgData.charger_led_config = ChargerPlatformFile_AsciiToInt((CHAR8*)Value);
      return;
    }


    if (AsciiStriCmp((CHAR8*)Key, "DCInBootToHLOSThresholdInMv") == 0)
    {
      gChargerPlatformCfgData.DCInCfgData.DCInBootThreshold = ChargerPlatformFile_AsciiToInt((char *)Value);
      return;
    }

    if (AsciiStriCmp((CHAR8*)Key, "SuspendDCIn") == 0)
    {
      gChargerPlatformCfgData.DCInCfgData.DCINDisableOnExit = (AsciiStriCmp((CHAR8*)Value, "TRUE") == 0);
      return;
    }

    if (AsciiStriCmp((CHAR8 *)Key, "ChargingTermCurrent") == 0)
    {
      gChargerPlatformCfgData.ChargerLibCfgData.fg_cfg_data.ChargingTermCurrent = ChargerPlatformFile_AsciiToInt((char *)Value);
      gChargerPlatformCfgData.ChargerLibCfgData.schg_cfg_data.ChargingTermCurrent = gChargerPlatformCfgData.ChargerLibCfgData.fg_cfg_data.ChargingTermCurrent;
      return;
    }

    if (AsciiStriCmp((CHAR8*)Key, "BattMissingCfg") == 0)
    {
      gChargerPlatformCfgData.ChargerLibCfgData.schg_cfg_data.BattMissingCfg = ChargerPlatformFile_AsciiToInt((char *)Value);
      return;
    }

    if (AsciiStriCmp((CHAR8*)Key, "FgCondRestart") == 0)
    {
      gChargerPlatformCfgData.ChargerLibCfgData.fg_cfg_data.FgCondRestart = (AsciiStriCmp((CHAR8*)Value, "TRUE") == 0);
      return;
    }

    if (AsciiStriCmp((CHAR8*)Key, "VBattEstDiffThreshold") == 0)
    {
      gChargerPlatformCfgData.ChargerLibCfgData.fg_cfg_data.VBattEstDiffThreshold = ChargerPlatformFile_AsciiToInt((char *)Value);
      return;
    }

    if (AsciiStriCmp((CHAR8*)Key, "LoadBatteryProfile") == 0)
    {
      gChargerPlatformCfgData.ChargerLibCfgData.fg_cfg_data.LoadBatteryProfile = (AsciiStriCmp((CHAR8*)Value, "TRUE") == 0);
      return;
    }

    if (AsciiStriCmp((CHAR8*)Key, "SWThermalMitigationEnable") == 0)
    {
      gChargerPlatformCfgData.ChargerLibCfgData.sw_thermal_mitigation_enable = (AsciiStriCmp((CHAR8*)Value, "TRUE") == 0);
      return;
    }

    if (AsciiStriCmp((CHAR8 *)Key, "BattVoltLimHighDelta") == 0)
    {
      gChargerPlatformCfgData.ChargerLibCfgData.batt_volt_lim_high_delta = ChargerPlatformFile_AsciiToInt((char *)Value);
      return;
    }

    if (AsciiStriCmp((CHAR8*)Key, "PrintChargerAppDbgMsg") == 0)
    {
      gChargerPlatformCfgData.ChargerLibCfgData.dbg_cfg_Data.print_charger_app_dbg_msg = (AsciiStriCmp((CHAR8*)Value, "TRUE") == 0);
      return;
    }

    if (AsciiStriCmp((CHAR8*)Key, "PrintChargerAppDbgMsgToFile") == 0)
    {
      gChargerPlatformCfgData.ChargerLibCfgData.dbg_cfg_Data.print_charger_app_dbg_msg_to_file = (AsciiStriCmp((CHAR8*)Value, "TRUE") == 0);
      return;
    }

    if (AsciiStriCmp((CHAR8*)Key, "FileLoggingDbgLevelMask") == 0)
    {
      gChargerPlatformCfgData.ChargerLibCfgData.dbg_cfg_Data.file_log_level_mask = ChargerPlatformFile_AsciiStrToHex((CHAR8 *)Value, AsciiStrLen((CHAR8 *)Value));
      return;
    }

    if (AsciiStriCmp((CHAR8*)Key, "BatteryIdTolerance") == 0)
    {
      gChargerPlatformCfgData.ChargerLibCfgData.fg_cfg_data.FgBattIdCfg.BatteryIdTolerance = ChargerPlatformFile_AsciiToInt((char *)Value);
      return;
    }

    if (AsciiStriCmp((CHAR8*)Key, "DebugBoardBatteryIdMin") == 0)
    {
      gChargerPlatformCfgData.ChargerLibCfgData.fg_cfg_data.FgBattIdCfg.DebugBoardBatteryIdMin = ChargerPlatformFile_AsciiToInt((char *)Value);;
      return;
    }

    if (AsciiStriCmp((CHAR8*)Key, "DebugBoardBatteryIdMax") == 0)
    {
      gChargerPlatformCfgData.ChargerLibCfgData.fg_cfg_data.FgBattIdCfg.DebugBoardBatteryIdMax = ChargerPlatformFile_AsciiToInt((char *)Value);
      return;
    }

    if (AsciiStriCmp((CHAR8*)Key, "RegularBatteryIdMin") == 0)
    {
      gChargerPlatformCfgData.ChargerLibCfgData.fg_cfg_data.FgBattIdCfg.RegularBatteryIdMin = ChargerPlatformFile_AsciiToInt((char *)Value);
      return;
    }

    if (AsciiStriCmp((CHAR8*)Key, "RegularBatteryIdMax") == 0)
    {
      gChargerPlatformCfgData.ChargerLibCfgData.fg_cfg_data.FgBattIdCfg.RegularBatteryIdMax = ChargerPlatformFile_AsciiToInt((char *)Value);
      return;
    }

    if (AsciiStriCmp((CHAR8*)Key, "SmartBatteryIdMin") == 0)
    {
      gChargerPlatformCfgData.ChargerLibCfgData.fg_cfg_data.FgBattIdCfg.SmartBatteryIdMin = ChargerPlatformFile_AsciiToInt((char *)Value);
      return;
    }

    if (AsciiStriCmp((CHAR8*)Key, "SmartBatteryIdMax") == 0)
    {
      gChargerPlatformCfgData.ChargerLibCfgData.fg_cfg_data.FgBattIdCfg.SmartBatteryIdMax = ChargerPlatformFile_AsciiToInt((char *)Value);
      return;
    }

    if (AsciiStriCmp((CHAR8*)Key, "ConservChgFvDelta") == 0)
    {
      gChargerPlatformCfgData.ChargerLibCfgData.conserv_chg_fv_delta = ChargerPlatformFile_AsciiToInt((char *)Value);
      return;
    }

    if (AsciiStriCmp((CHAR8*)Key, "NoChargeAndWait") == 0)
    {
      gChargerPlatformCfgData.ChargerLibCfgData.no_charge_and_wait = (AsciiStriCmp((CHAR8*)Value, "TRUE") == 0);
      return;
    }

    if (AsciiStriCmp((CHAR8*)Key, "EmergencyShutdownVbatt") == 0)
    {
      gChargerPlatformCfgData.ChargerLibCfgData.emergency_shutdown_vbatt = ChargerPlatformFile_AsciiToInt((char *)Value);
      return;
    }

    if (AsciiStriCmp((CHAR8*)Key, "UnknownBatteryBehavior") == 0)
    {
      gChargerPlatformCfgData.ChargerLibCfgData.unknown_battery_behavior = ChargerPlatformFile_AsciiToInt((char *)Value);
      return;
    }

    if (AsciiStriCmp((CHAR8*)Key, "DebugBoardBehavior") == 0)
    {
      gChargerPlatformCfgData.ChargerLibCfgData.debug_board_behavior = ChargerPlatformFile_AsciiToInt((char *)Value);
      return;
    }

    if (AsciiStriCmp((CHAR8*)Key, "ProgramSkinAndChargerHotThreshold") == 0)
    {
      gChargerPlatformCfgData.ChargerLibCfgData.fg_cfg_data.SkinAndChargerHotThreshold.ProgramSkinAndChargerHotThreshold = (AsciiStriCmp((CHAR8*)Value, "TRUE") == 0);
      return;
    }

    if (AsciiStriCmp((CHAR8*)Key, "DeviceSkinHotInC") == 0)
    {
      gChargerPlatformCfgData.ChargerLibCfgData.fg_cfg_data.SkinAndChargerHotThreshold.DeviceSkinHotInC = ChargerPlatformFile_AsciiToInt((char *)Value);
      return;
    }

    if (AsciiStriCmp((CHAR8*)Key, "DeviceSkinTooHotInC") == 0)
    {
      gChargerPlatformCfgData.ChargerLibCfgData.fg_cfg_data.SkinAndChargerHotThreshold.DeviceSkinTooHotInC = ChargerPlatformFile_AsciiToInt((char *)Value);
      return;
    }

    if (AsciiStriCmp((CHAR8*)Key, "ChargerTooHotInC") == 0)
    {
      gChargerPlatformCfgData.ChargerLibCfgData.fg_cfg_data.SkinAndChargerHotThreshold.ChargerTooHotInC = ChargerPlatformFile_AsciiToInt((char *)Value);
      return;
    }

    if (AsciiStriCmp((CHAR8*)Key, "ChargerHotInC") == 0)
    {
      gChargerPlatformCfgData.ChargerLibCfgData.fg_cfg_data.SkinAndChargerHotThreshold.ChargerHotInC = ChargerPlatformFile_AsciiToInt((char *)Value);
      return;
    }

    if (AsciiStriCmp((CHAR8*)Key, "DumpSram") == 0)
    {
      gChargerPlatformCfgData.ChargerLibCfgData.fg_cfg_data.FgSramDbgCfg.DumpSram = (AsciiStriCmp((CHAR8*)Value, "TRUE") == 0);
      return;
    }

    if (AsciiStriCmp((CHAR8*)Key, "DumpSramStartAddr") == 0)
    {
      gChargerPlatformCfgData.ChargerLibCfgData.fg_cfg_data.FgSramDbgCfg.DumpSramStartAddr = ChargerPlatformFile_AsciiToInt((char *)Value);
      return;
    }

    if (AsciiStriCmp((CHAR8*)Key, "DumpSramEndAddr") == 0)
    {
      gChargerPlatformCfgData.ChargerLibCfgData.fg_cfg_data.FgSramDbgCfg.DumpSramEndAddr = ChargerPlatformFile_AsciiToInt((char *)Value);
      return;
    }

    if (AsciiStriCmp((CHAR8*)Key, "DumpSramDuration") == 0)
    {
      gChargerPlatformCfgData.ChargerLibCfgData.fg_cfg_data.FgSramDbgCfg.DumpSramDuration = ChargerPlatformFile_AsciiToInt((char *)Value);
      return;
    }

    if (AsciiStriCmp((CHAR8*)Key, "BattThermBiasWait") == 0)
    {
      gChargerPlatformCfgData.ChargerLibCfgData.fg_cfg_data.FgBattThermCfg.BiasWait = ChargerPlatformFile_AsciiToInt((char *)Value);
      return;
    }
    if (AsciiStriCmp((CHAR8*)Key, "BattThermGndSel") == 0)
    {
      gChargerPlatformCfgData.ChargerLibCfgData.fg_cfg_data.FgBattThermCfg.GroundSel = (AsciiStriCmp((CHAR8*)Value, "TRUE") == 0);
      return;
    }

    if (AsciiStriCmp((CHAR8*)Key, "AuxThermBiasWait") == 0)
    {
      gChargerPlatformCfgData.ChargerLibCfgData.fg_cfg_data.FgAuxThermCfg.BiasWait = ChargerPlatformFile_AsciiToInt((char *)Value);
      return;
    }
    if (AsciiStriCmp((CHAR8*)Key, "AuxThermGndSel") == 0)
    {
      gChargerPlatformCfgData.ChargerLibCfgData.fg_cfg_data.FgAuxThermCfg.GroundSel = (AsciiStriCmp((CHAR8*)Value, "TRUE") == 0);
      return;
    }

    if (AsciiStriCmp((CHAR8*)Key, "TsensExtremeTemp") == 0)
    {
      gChargerPlatformCfgData.ChargerLibCfgData.thermal_configs.tsens_extreme_Temp = ChargerPlatformFile_AsciiToInt((char *)Value);
      return;
    }

    if (AsciiStriCmp((CHAR8*)Key, "TsensHighTemp") == 0)
    {
      gChargerPlatformCfgData.ChargerLibCfgData.thermal_configs.tsens_high_temp = ChargerPlatformFile_AsciiToInt((char *)Value);
      return;
    }

    if (AsciiStriCmp((CHAR8*)Key, "TsensLowTemp") == 0)
    {
      gChargerPlatformCfgData.ChargerLibCfgData.thermal_configs.tsens_low_temp = ChargerPlatformFile_AsciiToInt((char *)Value);
      return;
    }

    if (AsciiStriCmp((CHAR8*)Key, "TsensTimeoutMins") == 0)
    {
      gChargerPlatformCfgData.ChargerLibCfgData.thermal_configs.tsens_timeout_mins = ChargerPlatformFile_AsciiToInt((char *)Value);
      return;
    }

    if (AsciiStriCmp((CHAR8*)Key, "EnableChargerFgDump") == 0)
    {
      gChargerPlatformCfgData.ChargerLibCfgData.dbg_cfg_Data.enable_charger_fg_Dump = (AsciiStriCmp((CHAR8*)Value, "TRUE") == 0);
      return;
    }

    if (AsciiStriCmp((CHAR8*)Key, "RconnComp") == 0)
    {
      gChargerPlatformCfgData.ChargerLibCfgData.fg_cfg_data.RconnComp = ChargerPlatformFile_AsciiToInt((char *)Value);
      return;
    }
    if (AsciiStriCmp((CHAR8*)Key, "EnShipMode") == 0)
    {
      gChargerPlatformCfgData.ChargerLibCfgData.shipmode_cfg.enable_shipmode = (AsciiStriCmp((CHAR8*)Value, "TRUE") == 0);
      return;
    }

    if (AsciiStriCmp((CHAR8*)Key, "ShipModeVarStr") == 0)
    {
      AsciiStrCpy(gChargerPlatformCfgData.ChargerLibCfgData.shipmode_cfg.shipMode_str, (CHAR8*)Value);
      return;
    }
    
    if (AsciiStriCmp((CHAR8*)Key, "EnOffModeSrc") == 0)
    {
      gChargerPlatformCfgData.ChargerLibCfgData.off_mode_src_cfg.enable = (AsciiStriCmp((CHAR8*)Value, "TRUE") == 0);
      return;
    }

    if(AsciiStriCmp((CHAR8*)Key, "OffModeSrcVbatThLightLoad") ==0)
    {
      gChargerPlatformCfgData.ChargerLibCfgData.off_mode_src_cfg.vbatThLightLoad = ChargerPlatformFile_AsciiToInt((char *)Value);
      return;
    }
    
    if(AsciiStriCmp((CHAR8*)Key, "OffModeSrcVbatThDeltaHeavyLoad") ==0)
    {
        gChargerPlatformCfgData.ChargerLibCfgData.off_mode_src_cfg.vbatThDeltaHeavyLoad = ChargerPlatformFile_AsciiToInt((char *)Value);
        return;
    }
    
    if (AsciiStriCmp((CHAR8*)Key, "SkinTempSrc") == 0)
    {
      gChargerPlatformCfgData.ChargerLibCfgData.schg_cfg_data.EnableSkinTempINov = (AsciiStriCmp((CHAR8*)Value, "TRUE") == 0);
      return;
    }

    if (AsciiStriCmp((CHAR8*)Key, "DieTempSrc") == 0)
    {
      gChargerPlatformCfgData.ChargerLibCfgData.schg_cfg_data.EnableDieTempINov = (AsciiStriCmp((CHAR8*)Value, "TRUE") == 0);
      return;
    }

    if (AsciiStriCmp((CHAR8*)Key, "DieTempCompSrc") == 0)
    {
      gChargerPlatformCfgData.ChargerLibCfgData.schg_cfg_data.EnableDieTempCompINov = (AsciiStriCmp((CHAR8*)Value, "TRUE") == 0);
      return;
    }

    if (AsciiStriCmp((CHAR8*)Key, "PlatHWCfgOverrdie") == 0)
    {
      gChargerPlatformCfgData.ChargerLibCfgData.plat_hw_cfg_override = ChargerPlatformFile_AsciiToInt((char *)Value);
      return;
    }

    if (AsciiStriCmp((CHAR8*)Key, "GLinkDownAction") == 0)
    {
      gChargerPlatformCfgData.ChargerLibCfgData.glink_down_action = ChargerPlatformFile_AsciiToInt((char *)Value);
      return;
    }

    if (AsciiStriCmp((CHAR8*)Key, "ChargerFWMissingDisplayTime") == 0)
    {
      gChargerPlatformCfgData.ChargerLibCfgData.chargerfw_missing_display_time = ChargerPlatformFile_AsciiToInt((char *)Value);
      return;
    }
    if (AsciiStriCmp((CHAR8*)Key, "ForcePowerTesting") == 0)
    {
      gChargerPlatformCfgData.ChargerLibCfgData.ForceOffmodePowerTest = ChargerPlatformFile_AsciiToInt((char *)Value);
      gChargerPlatformCfgData.ChargerLibCfgData.plat_hw_cfg_override = CHARGERLIB_PLATFORM_HW_CONFIG_SMB_EXTFG;
      return;
    }
  }
  else if (((EFI_PLATFORMINFO_TYPE_MTP == PlatformType || (EFI_PLATFORMINFO_TYPE_CDP == PlatformType)) && (AsciiStriCmp((CHAR8*)Section, "PlatformMTP") == 0)) ||
    (((EFI_PLATFORMINFO_TYPE_QRD == PlatformType) || (EFI_PLATFORMINFO_TYPE_HDK == PlatformType)) && (AsciiStriCmp((CHAR8*)Section, "PlatformQRD") == 0)) ||
    ((EFI_PLATFORMINFO_TYPE_CLS == PlatformType) && (AsciiStriCmp((CHAR8*)Section, "PlatformCLS") == 0)))
  {
    /*Parameters w.r.t. MTP and QRD Platforms*/
    if (AsciiStriCmp((CHAR8*)Key, "ChgFccMax") == 0)
    {
      gChargerPlatformCfgData.ChargerLibCfgData.schg_cfg_data.ChgFccMax = ChargerPlatformFile_AsciiToInt((char *)Value);
      return;
    }

    if (AsciiStriCmp((CHAR8*)Key, "ChgFvMax") == 0)
    {
      gChargerPlatformCfgData.ChargerLibCfgData.schg_cfg_data.ChgFvMax = ChargerPlatformFile_AsciiToInt((char *)Value);;
      return;
    }

    if (AsciiStriCmp((CHAR8*)Key, "JeitaFvCompHotCfg") == 0)
    {
      gChargerPlatformCfgData.ChargerLibCfgData.schg_cfg_data.SchgJeitaData.JeitaFvCompHotCfg = ChargerPlatformFile_AsciiToInt((char *)Value);
      return;
    }

    if (AsciiStriCmp((CHAR8*)Key, "JeitaFvCompColdCfg") == 0)
    {
      gChargerPlatformCfgData.ChargerLibCfgData.schg_cfg_data.SchgJeitaData.JeitaFvCompColdCfg = ChargerPlatformFile_AsciiToInt((char *)Value);
      return;
    }

    if (AsciiStriCmp((CHAR8*)Key, "JeitaCcCompHotCfg") == 0)
    {
      gChargerPlatformCfgData.ChargerLibCfgData.schg_cfg_data.SchgJeitaData.JeitaCcCompHotCfg = ChargerPlatformFile_AsciiToInt((char *)Value);
      return;
    }

    if (AsciiStriCmp((CHAR8*)Key, "JeitaCcCompColdCfg") == 0)
    {
      gChargerPlatformCfgData.ChargerLibCfgData.schg_cfg_data.SchgJeitaData.JeitaCcCompColdCfg = ChargerPlatformFile_AsciiToInt((char *)Value);
      return;
    }

    if (AsciiStriCmp((CHAR8*)Key, "HwAfpHotLimit") == 0)
    {
      gChargerPlatformCfgData.ChargerLibCfgData.schg_cfg_data.HwAfpHotThreshold = ChargerPlatformFile_AsciiToInt((char *)Value);
      return;
    }

    if (AsciiStriCmp((CHAR8*)Key, "HwAfpColdLimit") == 0)
    {
      gChargerPlatformCfgData.ChargerLibCfgData.schg_cfg_data.HwAfpColdThreshold = ChargerPlatformFile_AsciiToInt((char *)Value);
      return;
    }

    if (AsciiStriCmp((CHAR8*)Key, "JeitaCriticalTempLowLimit") == 0)
    {
      gChargerPlatformCfgData.ChargerLibCfgData.fg_cfg_data.HwJeitaThreshold.JeitaCriticalTempLowLimit = ChargerPlatformFile_AsciiToInt((char *)Value);
      gChargerPlatformCfgData.ChargerLibCfgData.schg_cfg_data.SchgJeitaData.JeitaCriticalTempLowLimit = ChargerPlatformFile_AsciiToInt((char *)Value);
      return;
    }

    if (AsciiStriCmp((CHAR8*)Key, "JeitaHardColdLimit") == 0)
    {
      gChargerPlatformCfgData.ChargerLibCfgData.fg_cfg_data.HwJeitaThreshold.JeitaHardColdLimit = ChargerPlatformFile_AsciiToInt((char *)Value);
      gChargerPlatformCfgData.ChargerLibCfgData.schg_cfg_data.SchgJeitaData.JeitaHardColdLimit = ChargerPlatformFile_AsciiToInt((char *)Value);
      return;
    }

    if (AsciiStriCmp((CHAR8*)Key, "JeitaSoftColdLimit") == 0)
    {
      gChargerPlatformCfgData.ChargerLibCfgData.fg_cfg_data.HwJeitaThreshold.JeitaSoftColdLimit = ChargerPlatformFile_AsciiToInt((char *)Value);
      gChargerPlatformCfgData.ChargerLibCfgData.schg_cfg_data.SchgJeitaData.JeitaSoftColdLimit = ChargerPlatformFile_AsciiToInt((char *)Value);
      return;
    }

    if (AsciiStriCmp((CHAR8*)Key, "JeitaSoftHotLimit") == 0)
    {
      gChargerPlatformCfgData.ChargerLibCfgData.fg_cfg_data.HwJeitaThreshold.JeitaSoftHotLimit = ChargerPlatformFile_AsciiToInt((char *)Value);
      gChargerPlatformCfgData.ChargerLibCfgData.schg_cfg_data.SchgJeitaData.JeitaSoftHotLimit = ChargerPlatformFile_AsciiToInt((char *)Value);
      return;
    }

    if (AsciiStriCmp((CHAR8*)Key, "JeitaHardHotLimit") == 0)
    {
      gChargerPlatformCfgData.ChargerLibCfgData.fg_cfg_data.HwJeitaThreshold.JeitaHardHotLimit = ChargerPlatformFile_AsciiToInt((char *)Value);
      gChargerPlatformCfgData.ChargerLibCfgData.schg_cfg_data.SchgJeitaData.JeitaHardHotLimit = ChargerPlatformFile_AsciiToInt((char *)Value);
      return;
    }

    if (AsciiStriCmp((CHAR8*)Key, "JeitaCriticalTempHighLimit") == 0)
    {
      gChargerPlatformCfgData.ChargerLibCfgData.fg_cfg_data.HwJeitaThreshold.JeitaCriticalTempHighLimit = ChargerPlatformFile_AsciiToInt((char *)Value);
      gChargerPlatformCfgData.ChargerLibCfgData.schg_cfg_data.SchgJeitaData.JeitaCriticalTempHighLimit = ChargerPlatformFile_AsciiToInt((char *)Value);
      return;
    }

    if (AsciiStriCmp((CHAR8*)Key, "JeitaHardLimitEnable") == 0)
    {
      gChargerPlatformCfgData.ChargerLibCfgData.schg_cfg_data.SchgJeitaData.JeitaHardLimitEnable = ChargerPlatformFile_AsciiToInt((char *)Value);
      return;
    }

    if (AsciiStriCmp((CHAR8*)Key, "JeitaSoftColdFvCompEnable") == 0)
    {
      gChargerPlatformCfgData.ChargerLibCfgData.fg_cfg_data.HwJeitaThreshold.JeitaSoftColdFvCompEnable = (AsciiStriCmp((CHAR8*)Value, "TRUE") == 0);
      gChargerPlatformCfgData.ChargerLibCfgData.schg_cfg_data.SchgJeitaData.JeitaSoftColdFvCompEnable = ChargerPlatformFile_AsciiToInt((char *)Value);
      return;
    }

    if (AsciiStriCmp((CHAR8*)Key, "JeitaSoftHotFvCompEnable") == 0)
    {
      gChargerPlatformCfgData.ChargerLibCfgData.fg_cfg_data.HwJeitaThreshold.JeitaSoftHotFvCompEnable = (AsciiStriCmp((CHAR8*)Value, "TRUE") == 0);
      gChargerPlatformCfgData.ChargerLibCfgData.schg_cfg_data.SchgJeitaData.JeitaSoftHotFvCompEnable = ChargerPlatformFile_AsciiToInt((char *)Value);
      return;
    }

    if (AsciiStriCmp((CHAR8*)Key, "JeitaSoftColdCcCompEnable") == 0)
    {
      gChargerPlatformCfgData.ChargerLibCfgData.fg_cfg_data.HwJeitaThreshold.JeitaSoftColdCcCompEnable = (AsciiStriCmp((CHAR8*)Value, "TRUE") == 0);
      gChargerPlatformCfgData.ChargerLibCfgData.schg_cfg_data.SchgJeitaData.JeitaSoftColdCcCompEnable = ChargerPlatformFile_AsciiToInt((char *)Value);
      return;
    }

    if (AsciiStriCmp((CHAR8*)Key, "JeitaSoftHotCcCompEnable") == 0)
    {
      gChargerPlatformCfgData.ChargerLibCfgData.fg_cfg_data.HwJeitaThreshold.JeitaSoftHotCcCompEnable = (AsciiStriCmp((CHAR8*)Value, "TRUE") == 0);
      gChargerPlatformCfgData.ChargerLibCfgData.schg_cfg_data.SchgJeitaData.JeitaSoftHotCcCompEnable = ChargerPlatformFile_AsciiToInt((char *)Value);
      return;
    }

    if (AsciiStriCmp((CHAR8*)Key, "VBatEmpty") == 0)
    {
      gChargerPlatformCfgData.ChargerLibCfgData.fg_cfg_data.VBatEmpty = ChargerPlatformFile_AsciiToInt((char *)Value);
      return;
    }

    if (AsciiStriCmp((CHAR8*)Key, "BattMissingDelay") == 0)
    {
      gChargerPlatformCfgData.ChargerLibCfgData.fg_cfg_data.FgBattIdCfg.BattMissingDelay = ChargerPlatformFile_AsciiToInt((char *)Value);
      return;
    }

    if (AsciiStriCmp((CHAR8*)Key, "BattMissingAction") == 0)
    {
      gChargerPlatformCfgData.ChargerLibCfgData.fg_cfg_data.FgBattIdCfg.BattMissingAction = ChargerPlatformFile_AsciiToInt((char *)Value);
      return;
    }

    if (AsciiStriCmp((CHAR8*)Key, "EnableRtcAlarm") == 0)
    {
        gEnableRtcAlarm = ChargerPlatformFile_AsciiToInt((char *)Value);
        return;
    }

    if (AsciiStriCmp((CHAR8*)Key, "RtcAlarmTime") == 0)
    {
        gRtcAlarmTimeInSec = ChargerPlatformFile_AsciiToInt((char *)Value);
        return;
    }

    if (AsciiStriCmp((CHAR8*)Key, "FwUpdateSupport") == 0)
    {
      gChargerPlatformCfgData.ChargerLibCfgData.FwUpdateSupport = ( AsciiStriCmp ((CHAR8*)Value, "TRUE") == 0 );
      return;
    }
  }
  return;
}

EFI_STATUS ChargerPlatform_SaveChargeInfoToSmem(pm_chg_info_type *ChargerActionInfo)
{
  EFI_STATUS Status = EFI_SUCCESS;
  UINT32     buf_size = sizeof(pm_chg_info_type);
  UINT32     buf_size_ret = 0;
  pm_chg_info_type *pSmemChargerInfo = NULL, *pSmemChargerInfoRet = NULL;

  pSmemChargerInfo = (pm_chg_info_type *)smem_get_addr(SMEM_CHARGER_BATTERY_INFO, &buf_size_ret);
  if (!pSmemChargerInfo)
  {
    CHARGER_DEBUG((EFI_D_WARN, "QcomChargerDxe:: %a Retrieve Error.. XBL Core allocating SMEM now \r\n", __FUNCTION__));
    /* Assume Smem is already initilized in XBL Loader - No need to alloc as XBL loader will alloc for Core keeping this as debug code */
    pSmemChargerInfo = (pm_chg_info_type *)smem_alloc(SMEM_CHARGER_BATTERY_INFO, buf_size);
    if (!pSmemChargerInfo)
    {
      CHARGER_DEBUG((EFI_D_WARN, "QcomChargerDxe:: %a smem_alloc Error \r\n", __FUNCTION__));
      return EFI_DEVICE_ERROR;
    }
    else
    {
      CHARGER_DEBUG((EFI_D_WARN, "QcomChargerDxe:: %a SMEM Allocated \r\n", __FUNCTION__));
      /* Initialized to -1 since SBL did not allocate smem yet */
      pSmemChargerInfo->version = 1;
      pSmemChargerInfo->sbl_entry_mV = QCOM_CHARGER_INVALID_VALUE_MARKER;
      pSmemChargerInfo->sbl_entry_soc = QCOM_CHARGER_INVALID_VALUE_MARKER;
    }
  }

  /* Assign required values to smem memory */
  pSmemChargerInfo->uefi_entry_mV = ChargerActionInfo->uefi_entry_mV;
  pSmemChargerInfo->uefi_exit_mV = ChargerActionInfo->uefi_exit_mV;
  pSmemChargerInfo->uefi_entry_soc = ChargerActionInfo->uefi_entry_soc;
  pSmemChargerInfo->uefi_exit_soc = ChargerActionInfo->uefi_exit_soc;
  pSmemChargerInfo->uefi_charger_fw_mode = ChargerActionInfo->uefi_charger_fw_mode; /**< uefi_charger_fw_mode */

  buf_size_ret = 0;

  pSmemChargerInfoRet = (pm_chg_info_type *)smem_get_addr(SMEM_CHARGER_BATTERY_INFO, &buf_size_ret);
  if (!pSmemChargerInfoRet){
    CHARGER_DEBUG((EFI_D_WARN, "QcomChargerDxe:: %a Retrieve Error 2 \r\n", __FUNCTION__));
    return EFI_DEVICE_ERROR;
  }

  CHARGER_DEBUG((EFI_D_WARN, "QcomChargerDxe:: buf_size_ret = %d uefi_entry_mV = %d uefi_exit_mV = %d uefi_entry_soc = %d \r\n", buf_size_ret, pSmemChargerInfoRet->uefi_entry_mV, pSmemChargerInfoRet->uefi_exit_mV, pSmemChargerInfoRet->uefi_entry_soc));
  CHARGER_DEBUG((EFI_D_WARN, "QcomChargerDxe:: sbl_entry_mV = %d sbl_entry_soc = %d ueft_exit_soc = %d \r\n", pSmemChargerInfoRet->sbl_entry_mV, pSmemChargerInfoRet->sbl_entry_soc, pSmemChargerInfoRet->uefi_exit_soc));

  pSmemChargerInfoRet = NULL;
  pSmemChargerInfo = NULL;

  return Status;

}

/* TBD */

#if 0
EFI_STATUS QcomChargerPlatform_CheckIfStrongCharger
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
    CHARGER_DEBUG(( EFI_D_ERROR, "QcomChargerDxe:: %a Can not locate USB PwrCtrl Protocol = %r \r\n", __FUNCTION__, Status));
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

#endif

EFI_STATUS ChargerPlatform_CheckIfOkToBoot
(
EFI_QCOM_CHARGER_ACTION_TYPE *pActionType,
QCOM_CHARGER_PLATFORM_ACTION_INFO  *pChargerActionInfo,
QCOM_CHARGER_BATT_STATUS_INFO      CurrentBatteryStatus
)
{
  EFI_STATUS                     Status = EFI_SUCCESS;
  //BOOLEAN bStrongCharger;
  EFI_PM_PON_REASON_TYPE PonReasonType = { 0 };
  BOOLEAN AllowBoot = FALSE;
  CHARGERLIB_ATTACHED_CHGR_TYPE AttachedCharger = CHARGERLIB_ATTACHED_CHGR__NONE;
  CHARGERLIB_CHARGING_ERROR_TYPES      ErrorType = CHARGERLIB_CHARGING_ERROR_NONE;
  CHARGERLIB_PLATFORM_CHGR_MAX_PWR_REQ_CFG_STS_TYPE MaxPwrReqSts;

  /* Check if it is a reboot scenario */
  Status = ChargerLib_GetPlatformPwrOnReason(&PonReasonType);
  if (Status != EFI_SUCCESS)
  {
    CHARGER_DEBUG((EFI_D_ERROR, "QcomChargerDxe:: %a Error getting PON reason \r\n", __FUNCTION__));
    return Status;
  }
  CHARGER_DEBUG((EFI_D_WARN, "QcomChargerDxe:: %a PonReasonType : 0x%x \r\n", __FUNCTION__, PonReasonType));

  /* Charge Source Detection */
  Status = ChargerLib_GetChargingPath(&AttachedCharger);
  if (Status != EFI_SUCCESS)
  {
    CHARGER_DEBUG((EFI_D_ERROR, "QcomChargerPlatform:: %a Error getting charging path = %r \r\n", __FUNCTION__, Status));
    return Status;
  }

  /* TBD */
  //Status = QcomChargerPlatform_CheckIfStrongCharger(&bStrongCharger);

  // Call GetErrors for Battery Missing, vbattChecking not required.
  if (EFI_SUCCESS == ChargerLib_GetErrors(FALSE, &ErrorType) && 
      ErrorType == CHARGERLIB_CHARGING_ERROR_BATTERY_NOT_DETECTED && 
      gChargerPlatformCfgData.ChargerLibCfgData.fg_cfg_data.FgBattIdCfg.BattMissingAction == CHGRLIB_BATT_MISSING_ACTION_BOOT_TO_HLOS)
  {
    if (EFI_SUCCESS == ChargerLib_GetPlatformMaxPwrReqCfgSts(&MaxPwrReqSts) && 
		MaxPwrReqSts.status == CHGRPD_MAX_PWR_REQ_DONE)
    {
      //Check Pwrkey Press
      if (pChargerActionInfo->bPowerKeyPressed)
      {
        CHARGER_FILE_UART_DEBUG((EFI_D_WARN, "QcomChargerDxe::%a Power Key pressed & Battery missing, Set GOOD_TO_BOOT \r\n", __FUNCTION__));
        *pActionType = EFI_QCOM_CHARGER_ACTION_RESET_COLD;
        gGoingToBoot = TRUE;
      }
    }
    return Status;
  }

  switch (gChargerPlatformCfgData.ChargerLibCfgData.soc_based_boot)
  {
  case FALSE:
    if (((gChargerSharedInfo.uefi_charger_fw_mode != CHARGER_FW_MODE) && (CurrentBatteryStatus.BatteryVoltage >= gThresholdVbatt))
      || ((gChargerSharedInfo.uefi_charger_fw_mode == CHARGER_FW_MODE) && (CurrentBatteryStatus.BatteryVoltage >= gThresholdVbatt + THRESHOLD_CHARGING_HARDRESET_VBATT_MARGIN)))
    {
      AllowBoot = TRUE;
      //*pActionType = EFI_QCOM_CHARGER_ACTION_GOOD_TO_BOOT;
      CHARGER_DEBUG((EFI_D_WARN, "QcomChargerDxe:: %a Good To Boot To HLOS BatteryVoltage= %d gThresholdVbatt = %d \r\n",
        __FUNCTION__, CurrentBatteryStatus.BatteryVoltage, gThresholdVbatt));
    }
    else
    { /* TBD comment out */
      CHARGER_DEBUG((EFI_D_WARN, "QcomChargerDxe:: %a NOT Enough BatteryVoltage To Boot = %d gThresholdVbatt = %d \r\n",
        __FUNCTION__, CurrentBatteryStatus.BatteryVoltage, gThresholdVbatt));
        gGoingToBoot = FALSE;
    }
    break;

  case TRUE:
    if (((gChargerSharedInfo.uefi_charger_fw_mode != CHARGER_FW_MODE) && (CurrentBatteryStatus.StateOfCharge >= gThresholdSoc))
      || ((gChargerSharedInfo.uefi_charger_fw_mode == CHARGER_FW_MODE) && (CurrentBatteryStatus.StateOfCharge >= gThresholdSoc + THRESHOLD_CHARGING_HARDRESET_SOC_MARGIN)))
    {
      AllowBoot = TRUE;
      //*pActionType = EFI_QCOM_CHARGER_ACTION_GOOD_TO_BOOT;
      CHARGER_DEBUG((EFI_D_WARN, "QcomChargerDxe:: %a Good To Boot To HLOS StateOfCharge= %d gThresholdSoc = %d \r\n",
        __FUNCTION__, CurrentBatteryStatus.StateOfCharge, gThresholdSoc));
    }
    else
    { /* TBD comment out */
      CHARGER_DEBUG((EFI_D_WARN, "QcomChargerDxe:: %a Not Enough StateOfCharge= %d To Boot gThresholdSoc = %d \r\n",
        __FUNCTION__, CurrentBatteryStatus.StateOfCharge, gThresholdSoc));
        gGoingToBoot = FALSE;
    }
    break;
  default:
    break;
  }
  if (AllowBoot == TRUE)
  {
    /*
     * If Power key is pressed, reset. We will check the PON reason in
     * the next cycle, and determine if we could boot to HLOS.
     * This is a workaround for the issue of "BDS menu not displayed on screen"
     */
    if (pChargerActionInfo->bPowerKeyPressed)
    {
      CHARGER_FILE_UART_DEBUG((EFI_D_WARN, "QcomChargerDxe::%a Power Key pressed, performing Reset \r\n", __FUNCTION__));
      *pActionType = EFI_QCOM_CHARGER_ACTION_RESET_COLD;
      gGoingToBoot = TRUE;
      return Status;
    }
    else if ((!ShipModeExit && (PonReasonType.KPDPWR || PonReasonType.HARD_RESET || PonReasonType.SMPL))
            || (ShipModeExit == SHIP_MODE_PMIC_EXIT && (AttachedCharger == CHARGERLIB_ATTACHED_CHGR__NONE || AttachedCharger == CHARGERLIB_ATTACHED_CHGR__BATT)))
    {
      /* Good to boot if:
       * PON Reason is Power Key or HARD_RESET, and we have a strong charger or a good battery
       */
      CHARGER_FILE_UART_DEBUG((EFI_D_WARN, "QcomChargerDxe:: %a GOOD_TO_BOOT: PonReasonType.KPDPWR = %d, PonReasonType.HARD_RESET = %d, \r\n", __FUNCTION__, PonReasonType.KPDPWR, PonReasonType.HARD_RESET));

      ChargerPlatform_CheckChargerFWStatus();
      if (gChargerSharedInfo.uefi_charger_fw_mode == CHARGER_FW_MODE)
      {
        *pActionType = EFI_QCOM_CHARGER_ACTION_RESET_COLD;
      }
      else
      {
        *pActionType = EFI_QCOM_CHARGER_ACTION_GOOD_TO_BOOT;
        gGoingToBoot = TRUE;
      }
    }
    else
      gGoingToBoot = FALSE;
  }

  return EFI_SUCCESS;
}




/*This function (if feature enabled in config file) checks for EOC (End Of Charge).
* If EOC is reached, it configures RTC Alarm for XX time later and shuts system down, disabling USB PON.
*/
EFI_STATUS ChargerPlatform_SetEOCAlarm(EFI_QCOM_CHARGER_ACTION_TYPE *pActionType, QCOM_CHARGER_PLATFORM_ACTION_INFO *pChargerActionInfo)
{
    EFI_STATUS Status = EFI_SUCCESS;

    BOOLEAN isFullCharge = FALSE;

    CHARGER_DEBUG(( EFI_D_WARN, "QcomChargerDxe:: %a Entry. EnableRtcAlarm = %d \r\n", __FUNCTION__, gEnableRtcAlarm));

    // If this feature is enabled in config file then proceed further, else skip.
    if (gEnableRtcAlarm)
    {
        // For EOC, SoC == 100%
        isFullCharge = (pChargerActionInfo->BattStsInfo.StateOfCharge == 100) ? TRUE : FALSE;

        if (gIsUSBChargerConnected && isFullCharge)
        {
            // EOC is reached.

            CHARGER_DEBUG(( EFI_D_WARN, "QcomChargerDxe:: %a EOC reached \r\n", __FUNCTION__));

            if (!PmicRtcProtocol) {
                Status = gBS->LocateProtocol( &gQcomPmicRtcProtocolGuid, NULL, (VOID **)&PmicRtcProtocol );
                if(EFI_SUCCESS != Status || !PmicRtcProtocol)
                {
                    CHARGER_DEBUG(( EFI_D_WARN, "QcomChargerDxe:: %a Can not locate Pmic Rtc Protocol = %r \r\n", __FUNCTION__, Status));
                    return EFI_DEVICE_ERROR;
                }
            }

            // Set RTC Alarm for XX time later and shut down.
            // Device will wake up at specified time with pon reason as RTC alarm.

            EFI_PM_RTC_TIME_TYPE AlarmTime;
            AlarmTime.Sec = gRtcAlarmTimeInSec;
            AlarmTime.Msec = 0;
            Status = PmicRtcProtocol->AlarmSetTime(&AlarmTime);
            if(EFI_SUCCESS != Status || !PmicRtcProtocol)
            {
                CHARGER_DEBUG(( EFI_D_WARN, "QcomChargerDxe:: %a Can not set RTC alarm = %r \r\n", __FUNCTION__, Status));
                return EFI_DEVICE_ERROR;
            }
            else {
                CHARGER_DEBUG(( EFI_D_WARN, "QcomChargerDxe:: %a Set RTC Alarm for %u seconds later = %r \r\n", __FUNCTION__, AlarmTime.Sec, Status));
            }

            *pActionType = EFI_QCOM_CHARGER_ACTION_SHUTDOWN_USB_DC_PON_DISABLED;
        }
        else {
            CHARGER_DEBUG(( EFI_D_WARN, "QcomChargerDxe:: %a EOC NOT reached \r\n", __FUNCTION__));
        }
    }

    return EFI_SUCCESS;
}



EFI_STATUS ChargerPlatformFile_GetChargerConfig(EFI_QCOM_CHARGER_CONFIG_KEY ChargerCfgKey, UINT32 *KeyValue)
{
  EFI_STATUS Status = EFI_SUCCESS;

  if(FALSE == gChargerCfgInitialized)
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
      *KeyValue = gChargerPlatformCfgData.SWFlashMinBattVoltageMv;
    break;
    case EFI_QCOM_CHARGER_CONFIG_KEY_OFFMODE_POWER_TESTING_MODE:
      *KeyValue = gChargerPlatformCfgData.ChargerLibCfgData.ForceOffmodePowerTest;
      break;
    case EFI_QCOM_CHARGER_CONFIG_KEY_CHARGER_FW_INFO:
      *KeyValue = gChargerPlatformCfgData.ChargerLibCfgData.enable_charger_fw;
      break;
    case EFI_QCOM_CHARGER_CONFIG_KEY_FW_UPDATE_SUPPORT:
      *KeyValue = gChargerPlatformCfgData.ChargerLibCfgData.FwUpdateSupport;
    case EFI_QCOM_CHARGER_CONFIG_KEY_BATT_ID_INFO:
      {
        EFI_PM_BATTID_CFGDATA_TYPE *FgBattIdCfg = (EFI_PM_BATTID_CFGDATA_TYPE *)KeyValue;
        FgBattIdCfg->RegularBatteryIdMin = gChargerPlatformCfgData.ChargerLibCfgData.fg_cfg_data.FgBattIdCfg.RegularBatteryIdMin;
        FgBattIdCfg->RegularBatteryIdMax = gChargerPlatformCfgData.ChargerLibCfgData.fg_cfg_data.FgBattIdCfg.RegularBatteryIdMax;
        FgBattIdCfg->SmartBatteryIdMin = gChargerPlatformCfgData.ChargerLibCfgData.fg_cfg_data.FgBattIdCfg.SmartBatteryIdMin;
        FgBattIdCfg->SmartBatteryIdMax = gChargerPlatformCfgData.ChargerLibCfgData.fg_cfg_data.FgBattIdCfg.SmartBatteryIdMax;
        FgBattIdCfg->DebugBoardBatteryIdMin = gChargerPlatformCfgData.ChargerLibCfgData.fg_cfg_data.FgBattIdCfg.DebugBoardBatteryIdMin;
        FgBattIdCfg->DebugBoardBatteryIdMax = gChargerPlatformCfgData.ChargerLibCfgData.fg_cfg_data.FgBattIdCfg.DebugBoardBatteryIdMax;
      }
      break;
    case EFI_QCOM_CHARGER_CONFIG_SHIP_MODE_EXIT:
      *KeyValue = ShipModeExit;
      break;
    default:
      Status = EFI_INVALID_PARAMETER;
      break;
  }

  return Status;
}


EFI_STATUS ChargerPlatform_HandleShipMode(void)
{
  EFI_STATUS Status = EFI_SUCCESS;
  CHARGERLIB_PLATFORM_SHIP_MODE_TYPE    ShipModeType = SHIP_MODE_NONE;

  /* Read Ship mode var value */
  Status = ChargerLib_GetShipMode(&ShipModeType);

  if (EFI_SUCCESS == Status)
  {
    switch(ShipModeType)
    {
      case SHIP_MODE_FG_ENTER:
      case SHIP_MODE_PMIC_ENTER:
        Status = ChargerLib_EnableShipMode(ShipModeType);
        CHARGER_DEBUG((EFI_D_WARN, "QcomChargerDxe:: %a Enable ship mode Status = %r \r\n", __FUNCTION__, Status));
        break;

      case SHIP_MODE_FG_EXIT:
      case SHIP_MODE_PMIC_EXIT:
        ShipModeExit = ShipModeType;
        CHARGER_DEBUG((EFI_D_WARN, "QcomChargerDxe:: %a Exiting ship mode type %d\r\n", __FUNCTION__, ShipModeExit));
        break;

      default:
        break;
    }
  }

  return EFI_SUCCESS;
}


EFI_STATUS ChargerPlatform_HandleOffModeSrc(void)
{
  EFI_STATUS Status = EFI_SUCCESS;
  UINT8  OffModeSrcVar = 0;  
  UINTN  DataSize = 0;

  DataSize = sizeof(OffModeSrcVar);

  UINT8  OffModeSrcCfg = CHARGERLIB_OFF_MODE_SRC_EN_CHG_PRESENCE;

  if (gChargerPlatformCfgData.ChargerLibCfgData.off_mode_src_cfg.enable)
  {
    Status = gRT->GetVariable (L"OffModeSrc", &gQcomTokenSpaceGuid, NULL, &DataSize, &OffModeSrcVar);
    if (EFI_SUCCESS == Status)
    {
      if (OffModeSrcVar == 0 || OffModeSrcVar == 1)
        OffModeSrcCfg = CHARGERLIB_OFF_MODE_SRC_DIS;
      else if (OffModeSrcVar == 3)
        OffModeSrcCfg = CHARGERLIB_OFF_MODE_SRC_EN_ALWAYS;
    }
    else
    {
      OffModeSrcCfg = CHARGERLIB_OFF_MODE_SRC_DIS;
    }
  }
  
  Status |= ChargerLib_SetOffModeSrc(OffModeSrcCfg);
  
  return EFI_SUCCESS;
}


EFI_STATUS ChargerPlatform_LoadChargerFW()
{
  EFI_STATUS         Status = EFI_SUCCESS;

  if (!RETAIL)
  {
    SerialPortFlush();
  }
  /* TBD - Return BACK up NV error state so Action can display with minimum charging specification or return error during actual loading */
  Status = ChargerLib_LoadChargerFW();

  if (!RETAIL)
  {
    SerialPortFlush();
  }

  return Status;
}


EFI_STATUS ChargerPlatform_LinkChargerFW()
{
  EFI_STATUS                Status = EFI_SUCCESS;

  Status = ChargerLib_LinkChargerFW();
  return Status;
}

EFI_STATUS ChargerPlatform_HandleRecovery()
{
  EFI_STATUS                Status = EFI_SUCCESS;

  Status = ChargerLib_HandleRecovery();
  return Status;

}

