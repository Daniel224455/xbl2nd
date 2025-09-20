/** @file ChargerLibTarget.c

  Provide access to Charger Libraries

  Copyright (c) 2018-2022, Qualcomm Technologies Inc. All rights reserved.

**/

/*=============================================================================
                              EDIT HISTORY


 when         who     what, where, why
 --------     ---     ---------------------------------------------------------
04/24/20      ravi    Added support for device power with missing/damaged battery
08/09/19      mr      Fix offmode charging implementation for SEC_MOR feature
04/25/2019    mr      Skip error status for Vbus query API for TypeA port type
02/15/2019    sg      Enabling End of Charge (EOC) RTC Alarm setting
02/06/19      dc      Revise delay and retry implementation
01/25/19      dc      Added Delay and Retry for first connection check when ChargerFW is active
01/16/19      cs      added config item for power testing
12/13/2018    ivy     support pmic ship mode and update for fg ship mode
10/30/2018    ivy     support battery missing error handling, either shutdown or booting to HLOS
09/27/2018    ivy     support fg ship mode
09/24/2018    cs      enable soc threshold and loading FW by default, turn off i2c after FW loaded
09/18/2018    dc      Changed to Type-C Based Charger Detection
08/24/2018    cs      added config item to enable/disable loading charger FW.
08/16/2018    cs      for CLS without SMB2351, and CLS with SMB2351 but fake battery need to return charging not supported.
07/27/2018    cs      when getActivePort is not supported, then charging is not supported.
06/29/2018    dc      Support USB Charger Type Logging
06/29/2018    dc      Support Input Current Status API & Log
06/28/2018    cs      added support for smb2351.
06/21/2018    cs      added support for no FW case
03/26/2018    va      Initial revision
=============================================================================*/

/*===========================================================================*/
/*                  INCLUDE FILES                                            */
/*===========================================================================*/
#include <Uefi.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/QcomLib.h>
#include "Library/SerialPortShLib.h"
#include <Library/QcomTargetLib.h>
#include <Library/QcomBaseLib.h>
#include <PrintShLib.h>
#include <Library/UefiCfgLib.h>
#include <Library/BaseMemoryLib.h>
#include "string.h"

/**
  Library Dependencies
*/
#include <api/pmic/pm/pm_uefi.h>
#include "ChargerLibTarget.h"
#include "ChargerLibFG.h"

/**
  Protocol Dependencies
*/
#include <Protocol/EFIPlatformInfo.h>
#include <Protocol/EFIPmicVersion.h>
#include <Protocol/EFIVariableServices.h>
#include <Protocol/EFITsens.h>
#include <Protocol/EFIPmicRTC.h>
#include <Protocol/EFIUsbfnChgSrv.h>
#include <Protocol/EFIPmicRgbLed.h>
#include <Protocol/EFIUsbInitProtocol.h>
#include <Protocol/EFIUsbPwrCtrl.h>
#include <Protocol/EFIPIL.h>


/*===========================================================================*/
/*                  TYPEDEF AND MACRO DEFINITIONS                            */
/*===========================================================================*/
#define BATTERY_EMULATOR_UPPER_THRSHOLD_MV  4450
#define BATTERY_EMULATOR_LOWER_THRSHOLD_MV  3450
#define BATTERY_EMULATOR_BATT_ID            1500
#define BATTERY_EMULATOR_TEMP_C             23
#define BATTERY_EMULATOR_SOC                99

#define VBATT_TOLERANCE                     5 //Battery upper voltage tolerance limit.

#define UNKNOWN_BATT_SHUTDOWN               0
#define UNKNOWN_BATT_BOOT_TO_HLOS           1
#define UNKNOWN_BATT_CONSERVATIVE_CHARGING  2
#define UNKNOWN_BATT_REGULAR_CHARGING       3

#define DEBUG_BOARD_LOWVBAT_SHUTDOWN        0
#define DEBUG_BOARD_LOWVBAT_NOCHARGE        1
#define DEBUG_BOARD_BOOT_TO_HLOS            2

#define CHARGER_COOL_OFF_PERIOD_DEFAULT     300000 //5minutes=1000millisec*60*5=300000 milli sec
#define BOOT_THRESHOLD_VOLT                 3600

#define TWO_SECONDS                         2000000 /*2seconds = 2000000 uSec*/

#define FIVE_SEC_IN_MILLI_SEC               5000
#define TIMEOUT_WAIT_FOR_KEY                0x01

#define MAX_3000_MA                         3000
#define MAX_1500_MA                         1500

#define THERMAL_TIMEOUT_NOTSET              0
#define MIN_TO_SEC                          60

#define MAX_THERMAL_WAIT_PERIOD_IN_MINS     60

#define USB_3_IUSB_MAX                      900 //IUSB_MAX for USB 3.0 port
#define USB_2_IUSB_MAX                      500 //IUSB_MAX for USB 2.0 port
#define FCC_CONSERVE_CHG                    500
#define USB_ENUM_WAIT_DURATION              500 /*500 milliseconds*/

#define MAX_EMERGENCY_SHUTDOWN_COUNT        3

#define THERMAL_TIMEOUT_NOTSET              0
#define MIN_TO_SEC                          60

#define MAX_THERMAL_WAIT_PERIOD_IN_MINS     60

#define BATT_MISSING_BOOT_TO_HLOS           0
#define BATT_MISSING_SHUTDOWN               1
/*===========================================================================*/
/*                  FUNCTIONS PROTOTYPES                                     */
/*===========================================================================*/
EFI_STATUS ChargerLibTarget_ConfigureUsbCurrentSetting( VOID );
EFI_STATUS ChargerLibTarget_PrintDCInStatus();
EFI_STATUS ChargerLibTarget_GetBatteryType(ChgBattType *pBatteryType);
EFI_STATUS ChargerLibTarget_GetActivePortPmicIndex(UINT8* pmicIndex);
EFI_STATUS ChargerLibTarget_GetUsbChargerStatus(BOOLEAN *bChargerAttached);
EFI_STATUS ChargerLibTarget_GetVbusDetect(BOOLEAN *bVbusDetected);
EFI_STATUS ChargerLibTarget_CheckBatteryPresent(BOOLEAN FGChipPresent);

/*===========================================================================*/
/*                  LOCAL VARIABLE DECLARATIONS                              */
/*===========================================================================*/
STATIC EFI_QCOM_PMIC_SCHG_PROTOCOL          *PmicSchgProtocol  = NULL;
STATIC EFI_QCOM_PMIC_FG_BASIC_PROTOCOL      *PmicFgProtocol    = NULL;
STATIC EFI_ADC_PROTOCOL                     *AdcProtocol       = NULL;
//STATIC EFI_USBFN_CHG_SRV_PROTOCOL           *UsbChgSrvProtocol = NULL;
//STATIC EFI_QCOM_PMIC_USB_PROTOCOL           *PmicUsbProtocol   = NULL;
//STATIC EFI_USB_INIT_PROTOCOL                *UsbInitProtocol = NULL;
STATIC EFI_PMIC_GLINK_PROTOCOL              *pmicGlinkProtocol = NULL;
STATIC EFI_USB_PWR_CTRL_PROTOCOL            *UsbPwrCtrlProtocol = NULL;
extern EFI_QCOM_PMIC_PWRON_PROTOCOL         *PmicPwronProtocol;

EFI_BATTERY_CHARGER_INFO_TYPE           ChargerInfo = {0};
EFI_BATTERY_GAUGE_INFO_TYPE             BatteryGaugeInfo = {0};
chargerlib_cfgdata_type                 gChargerLibTargetCfgData = {0};

STATIC CONST CHARGERLIB_HW_CONFIG_INFO ChargerLibHwConfigTable[CHARGERLIB_PLATFORM_HW_CONFIG_MAX]= {
  {EfiBatteryChargerNone,    0x00010000, EfiBatteryGaugeNone,       0x00010000}, //NONE
  {EfiBatteryChargerQcomPmicSchg, 0x00010000, EfiBatteryGaugeQcomPmicFg, 0x00010000}, //PMIC
  {EfiBatteryChargerSMB2351, 0x00010000, EfiBatteryGaugeExternal, 0x00010000}, //SMB and External FG
};

STATIC CONST CHARGERLIB_PLATFORM_HW_CONFIG_INFO ChargerLibPlatformHwInfo[]=
{
  {EFI_PLATFORMINFO_TYPE_MTP,     NULL,   CHARGERLIB_PLATFORM_HW_CONFIG_PMIC},
  {EFI_PLATFORMINFO_TYPE_FLUID,   NULL,   CHARGERLIB_PLATFORM_HW_CONFIG_PMIC},
  {EFI_PLATFORMINFO_TYPE_QRD,     NULL,   CHARGERLIB_PLATFORM_HW_CONFIG_PMIC},
  {EFI_PLATFORMINFO_TYPE_CHI,     NULL,   CHARGERLIB_PLATFORM_HW_CONFIG_PMIC},
  {EFI_PLATFORMINFO_TYPE_SBC,     NULL,   CHARGERLIB_PLATFORM_HW_CONFIG_PMIC},
  {EFI_PLATFORMINFO_TYPE_LIQUID,  NULL,   CHARGERLIB_PLATFORM_HW_CONFIG_PMIC},
  {EFI_PLATFORMINFO_TYPE_RUMI,    NULL,   CHARGERLIB_PLATFORM_HW_CONFIG_NONE},
  {EFI_PLATFORMINFO_TYPE_CDP,     NULL,   CHARGERLIB_PLATFORM_HW_CONFIG_NONE},
  {EFI_PLATFORMINFO_TYPE_CLS,     NULL,   CHARGERLIB_PLATFORM_HW_CONFIG_SMB_EXTFG},
};

STATIC UINT32 BatteryIndex = 0;

BOOLEAN gIsUSBChargerConnected;

#define CHARGERLIB_IPC_MAX_LINK_WAIT_COUNT    20//200

#define CHARGERLIB_IPC_LINK_WAIT_TIME         1000 * 1000//1000 //100 MSEC

#define CHARGERLIB_CONNECTION_SETTLE_COUNT 10

#define CHARGERLIB_ADSP_LOAD_WAIT_TIME        1000 * 10 // 1 msec

#define CHARGERLIB_IPC_PRE_LINK_WAIT_TIME   1000 * 150 //15 mec
/*===========================================================================*/
/*                 FUNCTION DEFINITIONS                                      */
/*===========================================================================*/
/**
  Initialize ChargerLib Target functon Library functions

  @param none

  @return
  EFI_SUCCESS:           Function returned successfully.
  EFI_INVALID_PARAMETER: A Parameter was incorrect.
  EFI_DEVICE_ERROR:      The physical device reported an error.
  EFI_NOT_READY:         The physical device is busy or not ready to
                         process this request.
*/
EFI_STATUS ChargerLibTarget_Init( chargerlib_cfgdata_type *pChargerLibTargetConfig )
{
  EFI_STATUS                  Status         = EFI_SUCCESS;
  EFI_PLATFORMINFO_PLATFORM_TYPE  PlatformType;

  CopyMemS(&gChargerLibTargetCfgData, sizeof(chargerlib_cfgdata_type), pChargerLibTargetConfig , sizeof(chargerlib_cfgdata_type));

  Status = GetPlatformType(&PlatformType);
  if(EFI_SUCCESS != Status)
  {
    CHARGER_DEBUG((EFI_D_WARN, "ChargerLib:: %a  Error getting PlatformType \r\n", __FUNCTION__));
    return EFI_DEVICE_ERROR;
  }

  /* Enable USB and DCIN PON triggers to be certain and avoid bad use case*/
  //Status = ChargerLibTarget_SetPon1Trigger(EFI_PM_PON_OPTION_PON1_REDGE_PON, FALSE);
  //Status |= ChargerLibTarget_SetPon1Trigger(EFI_PM_PON_TRIGGER_CBLPWR, TRUE);

  if(Status != EFI_SUCCESS)
  {
    CHARGER_DEBUG((EFI_D_WARN, "ChargerLib:: %a Error locating Pwron Protocol = %r \r\n", __FUNCTION__, Status));
  }

  ChargerLibTarget_HWInit(pChargerLibTargetConfig, PlatformType);

  /* Locate Glink Protocol */
  if(NULL == pmicGlinkProtocol)
  {
    Status = gBS->LocateProtocol (&gpmicGlinkProtocolGuid,NULL, (VOID **) &pmicGlinkProtocol);
    if((EFI_SUCCESS != Status) || (NULL == pmicGlinkProtocol))
    {
      CHARGER_DEBUG(( EFI_D_ERROR, "ChargerLib:: %a pmicGlinkProtocol Failed %r \r\n", __FUNCTION__, Status));
    }
  }

  if (BatteryGaugeInfo.BatteryGaugeHW == EfiBatteryGaugeExternal)
  {
    if (SmartChargerLibFG_Init() == EFI_SUCCESS)
    {
      char manufactureName[MANUFACTURER_NAME_MAX_SIZE + 1] = { 0 };
      BOOLEAN FGChipPresent = TRUE;

      Status |= SmartChargerLibFG_GetManufactureName((UINT8*)manufactureName, MANUFACTURER_NAME_MAX_SIZE + 1);
      CHARGER_DEBUG((EFI_D_ERROR, "ChargerLib:: %a manufacture Name (%a) \r\n", __FUNCTION__, (char*)manufactureName));

      FGChipPresent = Status == EFI_SUCCESS ? TRUE : FALSE;
      (void)ChargerLibTarget_CheckBatteryPresent(FGChipPresent);
    }
  }

  /* Locate USB Power Ctrl Protocol */
  if (!UsbPwrCtrlProtocol)
  {
     Status = gBS->LocateProtocol( &gQcomUsbPwrCtrlProtocolGuid,
                                   NULL,
                                   (VOID **)&UsbPwrCtrlProtocol );
  }

  Status = ChargerLibTarget_EnablePmicShipMode(FALSE);
  Status |= ChargerLibTarget_SetOS(OS_HLOS);
  return Status;
}

/**
  Get Battery Status Information
  @param[out] BattStatusInfo  refer EFI_PM_FG_BATT_STATUS_INFO
  This function gets the battery status, battery SOC, Charge current, battery voltage
  and battery temperature from Fuel Gauge

  @param[out] BatteryStatus  see chargerlib_batt_status_info for more details

  @return
  EFI_SUCCESS:           Function returned successfully.
  EFI_DEVICE_ERROR:      The physical device reported an error.
  EFI_UNSUPPORTED:       No Library function linked
*/
EFI_STATUS ChargerLibTarget_GetBatteryStatus(chargerlib_batt_status_info *pBatteryStatus)
{
  EFI_STATUS Status = EFI_SUCCESS;
  EFI_PM_GLINK_LINK_STATUS      PmicGLinkStatus = { 0 };
  EFI_PM_GLINK_BATT_STATUS_INFO BattStsInfo = { 0 };

  if(NULL == pBatteryStatus)
  {
    return EFI_INVALID_PARAMETER;
  }

  switch (BatteryGaugeInfo.BatteryGaugeHW)
  {
    case EfiBatteryGaugeQcomPmicFg:
      Status = EFI_UNSUPPORTED;
      break;

    default:
      if ((EFI_SUCCESS == ChargerLibTarget_GetIPCStatus(&PmicGLinkStatus)) && (PmicGLinkStatus.IsPMICChannelOpen == TRUE))
      {
        Status = pmicGlinkProtocol->GetBatteryStatus(BatteryIndex, &BattStsInfo);
        if(EFI_SUCCESS != Status)
        {
          CHARGER_DEBUG((EFI_D_WARN, "ChargerLib:: %a Status: %r \r\n",__FUNCTION__, Status));
          return EFI_DEVICE_ERROR;
        }
        else
        {
          pBatteryStatus->StateOfCharge = BattStsInfo.StateOfCharge;
          pBatteryStatus->ChargeCurrent = BattStsInfo.ChargeCurrent;
          pBatteryStatus->BatteryVoltage= BattStsInfo.BatteryVoltage;
          pBatteryStatus->BatteryTemperature = BattStsInfo.BatteryTemperature;
        }
      }
      else
      {
        //get FG information from I2c
        SmartChargerFG_BattStatusInfo extFGBatteryStatus = { 0 };
        Status = SmartChargerLibFG_GetBatteryStatus(&extFGBatteryStatus);
        if (EFI_SUCCESS != Status)
        {
          CHARGER_DEBUG((EFI_D_WARN, "ChargerLib:: %a getting battery status from ext FG Status: %r \r\n", __FUNCTION__, Status));
          return EFI_DEVICE_ERROR;
        }
        else
        {
          pBatteryStatus->StateOfCharge = extFGBatteryStatus.StateOfCharge;
          pBatteryStatus->ChargeCurrent = extFGBatteryStatus.ChargeCurrent;
          pBatteryStatus->BatteryVoltage = extFGBatteryStatus.BatteryVoltage;
          pBatteryStatus->BatteryTemperature = extFGBatteryStatus.BatteryTemperature;
        }
      }
      break;
  }

  return Status;
}

/**
  Get Battery Status Information
  @param[out] BattStatusInfo  refer EFI_PM_FG_BATT_STATUS_INFO
  This function gets the battery status, battery SOC, Charge current, battery voltage
  and battery temperature from Fuel Gauge

  @param[out] BatteryStatus  see chargerlib_batt_status_info for more details

  @return
  EFI_SUCCESS:           Function returned successfully.
  EFI_DEVICE_ERROR:      The physical device reported an error.
  EFI_UNSUPPORTED:       No Library function linked
*/
EFI_STATUS ChargerLibTarget_GetBatteryProfileStatus(chargerlib_batt_profile_status *ProfileSts)
{
  if(!ProfileSts)
  {
    return EFI_INVALID_PARAMETER;
  }

  return EFI_UNSUPPORTED;
}

EFI_STATUS ChargerLibTarget_LoadProfile(OPTIONAL chargerlib_batt_profile_data *pBattProfileInfo)
{

  return EFI_UNSUPPORTED;

}

/**
  Enable charger

  @param[in] Enable  TRUE  - Enable Charging
                     FALSE - Disable Charging

  @return
  EFI_SUCCESS:           Function returned successfully.
  EFI_DEVICE_ERROR:      The physical device reported an error.
  EFI_UNSUPPORTED:       No Library function linked
*/
EFI_STATUS ChargerLibTarget_ChargerEnable(BOOLEAN Enable)
{
  EFI_STATUS Status = EFI_SUCCESS;
  //BOOLEAN    BatteryPresent = FALSE;

  //  switch (ChargerInfo.ChargerHW)
  //  {
  //    case EfiBatteryChargerQcomPmicSchg:
  //      Status = ChargerLibTarget_GetBatteryPresence(&BatteryPresent);
  //      if (BatteryPresent)
  //      {
  //        Status = PmicSchgProtocol->EnableCharger(ChargerInfo.ChargerPmicInfo.PmicIndex, Enable);
  //        if( (EFI_SUCCESS == Status ) && (FALSE == Enable ))
  //        {
  //          /* make sure to turn Off Charging Led if charging is getting disabled */
  //          if(gChargerLibTargetCfgData.charger_led_config)
  //          {
  //            /* Turn Off LED since Charging is disabled */
  //            ChargerLibTarget_LedOn(FALSE);
  //          }
  //
  //          if(Status == EFI_SUCCESS)
  //            gChargingEnabled = Enable;
  //        }
  //      }
  //      break;
  //    default:
  //      Status = EFI_UNSUPPORTED;
  //      break;
  //  }

  return Status;
}

/**
  Set charger max current

  @param[in] MaxCurrent  Max current for charger in mA

  @return
  EFI_SUCCESS:           Function returned successfully.
  EFI_DEVICE_ERROR:      The physical device reported an error.
  EFI_UNSUPPORTED:       No Library function linked
*/
EFI_STATUS ChargerLibTarget_SetMaxUsbCurrent(UINT32 MaxCurrent)
{
  EFI_STATUS Status = EFI_SUCCESS;

  /*switch (ChargerInfo.ChargerHW)
  {
  case EfiBatteryChargerQcomPmicSchg:
    Status = PmicSchgProtocol->SetUsbMaxCurrent(ChargerInfo.ChargerPmicInfo.PmicIndex, MaxCurrent);
    break;
  default:
    Status = EFI_UNSUPPORTED;
    break;
  }*/

  return Status;
}

/**
  Set Battery FCC Max current

  @param[in] FccMaxCurrent  Max current for charger in mA

  @return
  EFI_SUCCESS:           Function returned successfully.
  EFI_DEVICE_ERROR:      The physical device reported an error.
  EFI_UNSUPPORTED:       No Library function linked
*/
EFI_STATUS ChargerLibTarget_SetFccMaxCurrent(UINT32 FccMaxCurrent)
{
  EFI_STATUS Status = EFI_SUCCESS;

  /*switch (ChargerInfo.ChargerHW)
  {
  case EfiBatteryChargerQcomPmicSchg:
    Status = PmicSchgProtocol->SetFccMaxCurrent(ChargerInfo.ChargerPmicInfo.PmicIndex, FccMaxCurrent);
    break;
  default:
    Status = EFI_UNSUPPORTED;
    break;
  }*/

  return Status;
}

EFI_STATUS ChargerLibTarget_WasChargerReinserted(BOOLEAN *pChargerReinserted)
{
  EFI_STATUS Status = EFI_SUCCESS;

  if(!pChargerReinserted)
  {
    return EFI_INVALID_PARAMETER;
  }

  *pChargerReinserted = FALSE;

  return Status;
}

EFI_STATUS ChargerLibTarget_ReRunAicl( VOID )
{
  EFI_STATUS Status = EFI_SUCCESS;

  /*switch (ChargerInfo.ChargerHW)
  {
    case EfiBatteryChargerQcomPmicSchg:

      Status = PmicSchgProtocol->RerunAicl(ChargerInfo.ChargerPmicInfo.PmicIndex);

      break;
    default:
      Status = EFI_UNSUPPORTED;
      break;
  }*/

  return Status;
}

/**
  Set Battery Floating Max Voltage

  @param[in] VddMaxVoltage  Max Voltage for Charger in mV

  @return
  EFI_SUCCESS:           Function returned successfully.
  EFI_DEVICE_ERROR:      The physical device reported an error.
  EFI_UNSUPPORTED:       No Library function linked
*/
EFI_STATUS ChargerLibTarget_SetFvMaxVoltage(UINT32 VddMaxVoltage)
{
  EFI_STATUS Status = EFI_SUCCESS;

  /*switch (ChargerInfo.ChargerHW)
  {
    case EfiBatteryChargerQcomPmicSchg:
      Status = PmicSchgProtocol->SetFvMaxVoltage(ChargerInfo.ChargerPmicInfo.PmicIndex, VddMaxVoltage);
      break;
    default:
      Status = EFI_UNSUPPORTED;
      break;
  }*/

  return Status;
}


EFI_STATUS ChargerLibTarget_GetChargingPath(CHARGERLIB_ATTACHED_CHGR_TYPE *ChargingPath)
{
  EFI_PM_GLINK_LINK_STATUS      PmicGLinkStatus = { 0 };
  EFI_PM_GLINK_BATT_STATUS_INFO BattStsInfo = { 0 };
  EFI_STATUS Status = EFI_SUCCESS;
  static UINT8 nochg_retry_count = 0;
  static BOOLEAN bFWFirstCheck = TRUE;
  UINT8 active_pmic_index = 0;
  EFI_PM_SCHG_TYPEC_PORT_STATUS UsbcStatus;

  if (!ChargingPath)
  {
    return EFI_INVALID_PARAMETER;
  }
  *ChargingPath = CHARGERLIB_ATTACHED_CHGR__NONE;
  gIsUSBChargerConnected = FALSE;
  
  SetMem(&UsbcStatus, sizeof(EFI_PM_SCHG_TYPEC_PORT_STATUS), 0x0);

  if ((EFI_SUCCESS == ChargerLibTarget_GetIPCStatus(&PmicGLinkStatus)) && (PmicGLinkStatus.IsPMICChannelOpen == TRUE))
  {
    Status = pmicGlinkProtocol->GetBatteryStatus(BatteryIndex, &BattStsInfo);
    if(EFI_SUCCESS != Status)
    {
      CHARGER_DEBUG((EFI_D_WARN, "ChargerLib:: %a Status: %r \r\n",__FUNCTION__, Status));
      return EFI_DEVICE_ERROR;
    }
    else
    {
      if(BattStsInfo.ChargingSource == PMIC_GLINK_CHARGING_SOURCE_TYPE_USB)
      {
        *ChargingPath = CHARGERLIB_ATTACHED_CHGR__USB;
        nochg_retry_count = 0;
        if(bFWFirstCheck)
            bFWFirstCheck = FALSE;
        gIsUSBChargerConnected = TRUE;
      }
      else
      {
        gIsUSBChargerConnected = FALSE;

        BOOLEAN bVbusDetected = FALSE;
        Status = ChargerLibTarget_GetVbusDetect(&bVbusDetected);
        if(EFI_SUCCESS != Status)
        {
          CHARGER_DEBUG((EFI_D_WARN, "ChargerLib:: %a GetVbusDetect Status: %r \r\n",__FUNCTION__, Status));
          return EFI_DEVICE_ERROR;
        }
		Status = ChargerLibTarget_GetActivePortPmicIndex(&active_pmic_index);
		Status |= PmicSchgProtocol->GetPortState(active_pmic_index, &UsbcStatus);
        if (EFI_SUCCESS != Status)
        {
          CHARGER_DEBUG((EFI_D_WARN, "ChargerLib:: %a GetPortState Status: %r \r\n", __FUNCTION__, Status));
          return EFI_DEVICE_ERROR;
        }
        if ((bFWFirstCheck || UsbcStatus.ccout_out_hiz_sts) &&
           (nochg_retry_count++ < CHARGERLIB_CONNECTION_SETTLE_COUNT))
        {
          CHARGER_DEBUG((EFI_D_WARN, "ChargerLib:: %a BattStsInfo.ChargingSource = %d, bVbusDetected = %d, retry count = %d\r\n"
                         ,__FUNCTION__, BattStsInfo.ChargingSource, bVbusDetected, nochg_retry_count));
          *ChargingPath = CHARGERLIB_ATTACHED_CHGR__USB;
        }
        if(nochg_retry_count >= CHARGERLIB_CONNECTION_SETTLE_COUNT)
        {
            bFWFirstCheck = FALSE;
        }
      }
    }
  }
  else
  {
    BOOLEAN IsInputValid = FALSE;
    Status = ChargerLibTarget_GetUsbChargerStatus(&IsInputValid);
    if(Status != EFI_SUCCESS)
    {
      return Status;
    }
    if (IsInputValid)
    {
      *ChargingPath = CHARGERLIB_ATTACHED_CHGR__USB;
      gIsUSBChargerConnected = TRUE;
    }
    else
    {

      *ChargingPath = CHARGERLIB_ATTACHED_CHGR__NONE;
      gIsUSBChargerConnected = FALSE;
    }
  }

  return Status;
}

/**
  Get Battery Presence

  @param[out] BatteryPresence

  @return
  EFI_SUCCESS:           Function returned successfully.
  EFI_INVALID_PARAMETER: Parameter is invalid.
  EFI_DEVICE_ERROR:      The physical device reported an error.
  EFI_UNSUPPORTED:       No Library function linked
*/
EFI_STATUS ChargerLibTarget_GetBatteryPresence(BOOLEAN *BatteryPresence)
{
  EFI_STATUS Status = EFI_SUCCESS;
  EFI_PM_GLINK_LINK_STATUS      PmicGLinkStatus = { 0 };

  if ((EFI_SUCCESS == ChargerLibTarget_GetIPCStatus(&PmicGLinkStatus)) && (PmicGLinkStatus.IsPMICChannelOpen == TRUE))
  {
    Status = pmicGlinkProtocol->IsBatteryPresent(BatteryIndex, BatteryPresence);
    if(EFI_SUCCESS != Status)
    {
      CHARGER_DEBUG((EFI_D_WARN, "ChargerLib:: %a Status: %r \r\n",__FUNCTION__, Status));
    }
  }
  else
  {
     UINT8 activePortPmicIndex = 0;
     /* Check external FG battery presence here using I2C Api */
     if(!PmicSchgProtocol)
     {
       Status = gBS->LocateProtocol(&gQcomPmicSchgProtocolGuid, NULL, (VOID **)&PmicSchgProtocol);
     }

     if((NULL == PmicSchgProtocol ) || (Status != EFI_SUCCESS))
       return EFI_DEVICE_ERROR;

     Status = ChargerLibTarget_GetActivePortPmicIndex(&activePortPmicIndex);
     if (Status != EFI_SUCCESS)
     {
       return Status;
     }

     Status = PmicSchgProtocol->IsBatteryPresent(activePortPmicIndex, BatteryPresence);
  }

  return Status;
}

/**
  Gets charger port type

  @param[out] pPortType  Charger port type

  @return
  EFI_SUCCESS:           Function returned successfully.
  EFI_DEVICE_ERROR:      The physical device reported an error.
  EFI_UNSUPPORTED:       No Library function linked
*/
EFI_STATUS ChargerLibTarget_GetPortType(CHARGERLIB_CHGR_PORT_TYPE *pPortType)
{
  EFI_STATUS                  Status = EFI_SUCCESS;
  EFI_PM_SCHG_CHGR_PORT_TYPE  PortType;
  UINT8 activePortPmicIndex = 0;

  switch (ChargerInfo.ChargerHW)
  {
    case EfiBatteryChargerQcomPmicSchg:
    case EfiBatteryChargerSMB2351:
      Status = ChargerLibTarget_GetActivePortPmicIndex(&activePortPmicIndex);
      if (Status != EFI_SUCCESS)
      {
        return Status;
      }
      Status = PmicSchgProtocol->GetChargerPortType(activePortPmicIndex, &PortType);
      *pPortType = (CHARGERLIB_CHGR_PORT_TYPE)PortType;

      CHARGER_FILE_UART_DEBUG((EFI_D_WARN,"ChargerLib:: %a Charger Port type = %d \r\n", __FUNCTION__, PortType));

      break;
    default:
      Status = EFI_UNSUPPORTED;
      break;
  }

  return Status;
}


EFI_STATUS ChargerLibTarget_HandleNoChargeAndWait(void)
{
  EFI_STATUS Status = EFI_SUCCESS;

  if (TRUE == gChargerLibTargetCfgData.no_charge_and_wait)
  {
    /*Disable Charging */
    //Status = ChargerLibTarget_ChargerEnable(FALSE);
    CHARGER_DEBUG((EFI_D_WARN, "ChargerLib:: %a cannot turn off charging by SW in recovery mode \r\n", __FUNCTION__));
  }
  else
  {
    ChargerLibTarget_ForceSysShutdown(CHGAPP_RESET_AFP);
  }

  return Status;
}

EFI_STATUS ChargerLibTarget_EnableAfpMode(void)
{
  EFI_STATUS Status = EFI_SUCCESS;

  switch (ChargerInfo.ChargerHW)
  {
    case EfiBatteryChargerQcomPmicSchg:
    case EfiBatteryChargerSMB2351:
      //Status = PmicSchgProtocol->EnableAfpMode(ChargerInfo.ChargerPmicInfo.PmicIndex);
      Status = ChargerLibTarget_ForceSysShutdown(CHGAPP_RESET_AFP);
      break;
    default:
      Status = EFI_UNSUPPORTED;
      break;
  }

  return Status;
}

EFI_STATUS ChargerLibTarget_GetBatteryID(INT32 *pBatteryID)
{
  UINT8 activePortPmicIndex = 0;
  EFI_STATUS Status = EFI_SUCCESS;
  BOOLEAN fake_batt = FALSE;

  if(NULL == pBatteryID)
  {
    return EFI_INVALID_PARAMETER;
  }
  Status = ChargerLibTarget_GetActivePortPmicIndex(&activePortPmicIndex);
  if (Status != EFI_SUCCESS)
  {
    CHARGER_DEBUG((EFI_D_ERROR, "ChargerLib:: %a  _GetActivePortPmicIndex Failed, Status = %r \r\n", __FUNCTION__, Status));
    return Status;
  }

  Status = PmicSchgProtocol->GetBattMissingStatus(activePortPmicIndex, EFI_PM_SCHG_FAKE_BAT_DET, &fake_batt);

  CHARGER_DEBUG((EFI_D_WARN, "ChargerLib:: %a  fake_batt= %d, Status = %r \r\n", __FUNCTION__, fake_batt, Status));

  *pBatteryID = fake_batt ? 7500 : 245000;

  return Status;
}

EFI_STATUS ChargerLibTarget_GetBatteryType(ChgBattType *pBatteryType)
{
  EFI_STATUS Status = EFI_SUCCESS;
  UINT32 BatteryCount = 0;
  EFI_PM_GLINK_LINK_STATUS      PmicGLinkStatus = { 0 };
  BOOLEAN fake_batt = FALSE;

  if(NULL == pBatteryType)
  {
    return EFI_INVALID_PARAMETER;
  }

  *pBatteryType = CHG_BATT_TYPE_INVALID;

  if(NULL == pmicGlinkProtocol)
  {
    Status = gBS->LocateProtocol (&gpmicGlinkProtocolGuid,NULL, (VOID **) &pmicGlinkProtocol);
    if((EFI_SUCCESS != Status) && (NULL == pmicGlinkProtocol))
    {
      CHARGER_DEBUG(( EFI_D_ERROR, "ChargerLib:: %a pmicGlinkProtocol Failed %r \r\n", __FUNCTION__, Status));
      return Status;
    }
  }
  if ((EFI_SUCCESS == ChargerLibTarget_GetIPCStatus(&PmicGLinkStatus)) && (PmicGLinkStatus.IsPMICChannelOpen == TRUE))
  {
  Status |= pmicGlinkProtocol->GetBatteryCount(&BatteryCount);

  if(EFI_SUCCESS != Status)
  {
    CHARGER_DEBUG(( EFI_D_ERROR, "ChargerLib:: %a pmicGlinkProtocol GetBatteryCount Failed %r \r\n", __FUNCTION__, Status));
    return Status;
  }

  CHARGER_DEBUG((EFI_D_ERROR, "ChargerLib:: %a Battery Count = %d \r\n", __FUNCTION__, BatteryCount));

  /*Check if battery ID is within real battery range*/
   if(BatteryCount > 0)
   {
     *pBatteryType = CHG_BATT_TYPE_NORMAL;
   }
   else
   {
     *pBatteryType = CHG_BATT_TYPE_UNKNOWN;
   }
  }
  else
  {
    UINT8 activePortPmicIndex = 0;
    switch (ChargerInfo.ChargerHW)
    {
    case EfiBatteryChargerQcomPmicSchg:
    case EfiBatteryChargerSMB2351:
      Status = ChargerLibTarget_GetActivePortPmicIndex(&activePortPmicIndex);
      if (Status != EFI_SUCCESS)
      {
        return Status;
      }
      Status = PmicSchgProtocol->GetBattMissingStatus(activePortPmicIndex, EFI_PM_SCHG_FAKE_BAT_DET, &fake_batt);
      break;
    default:
      Status = EFI_UNSUPPORTED;
      break;
    }

    *pBatteryType = fake_batt ? CHG_BATT_TYPE_DEBUG_BOARD : CHG_BATT_TYPE_NORMAL;   //for now set it to normal. SBL should have disabled charging if it is debug board.
  }


   CHARGER_DEBUG((EFI_D_WARN, "ChargerLib:: %a Battery type = %d \r\n", __FUNCTION__, *pBatteryType));

  return Status;
}

EFI_STATUS ChargerLibTarget_UsbSuspend(BOOLEAN Enable)
{
  EFI_STATUS Status = EFI_SUCCESS;

  CHARGER_DEBUG((EFI_D_WARN, "ChargerLib:: %a ChargerLib UsbSuspend = %d \r\n", __FUNCTION__, Enable));

  /*switch (ChargerInfo.ChargerHW)
  {
  case EfiBatteryChargerQcomPmicSchg:
    Status = PmicSchgProtocol->UsbSuspend(ChargerInfo.ChargerPmicInfo.PmicIndex, Enable);
    break;
  default:
    Status = EFI_UNSUPPORTED;
    break;
  }*/

  return Status;
}

EFI_STATUS ChargerLibTarget_DcinSuspend(BOOLEAN Suspend)
{
  EFI_STATUS Status = EFI_SUCCESS;

  CHARGER_DEBUG((EFI_D_WARN, "ChargerLib:: %a ChargerLib DcinSuspend = %d \r\n", __FUNCTION__, Suspend));

  /*switch (ChargerInfo.ChargerHW)
  {
  case EfiBatteryChargerQcomPmicSchg:
    Status = PmicSchgProtocol->DcinSuspend(ChargerInfo.ChargerPmicInfo.PmicIndex, Suspend);
    break;
  default:
    Status = EFI_UNSUPPORTED;
    break;
  }*/

  return Status;
}

EFI_STATUS ChargerLibTarget_DcinSetPowerLimit(UINT32 PowerInMicroW)
{
  EFI_STATUS Status = EFI_SUCCESS;

  CHARGER_DEBUG((EFI_D_WARN, "ChargerLib:: %a ChargerLib powerInMicroW = %d w\r\n", __FUNCTION__, PowerInMicroW));

  /*switch (ChargerInfo.ChargerHW)
  {
  case EfiBatteryChargerQcomPmicSchg:
    Status = PmicSchgProtocol->SetDcinPower(ChargerInfo.ChargerPmicInfo.PmicIndex, PowerInMicroW);
    break;
  default:
    Status = EFI_UNSUPPORTED;
    break;
  }*/

  return Status;
}

EFI_STATUS ChargerLibTarget_GetDebugBoardAction(CHARGERLIB_ERROR_ACTION_TYPE *pDebugBoardAction)
{
  EFI_STATUS Status = EFI_SUCCESS;
  UINT32     DebugBoardBehaviour = gChargerLibTargetCfgData.debug_board_behavior;

  chargerlib_batt_status_info  BatteryStatus;

  if (!pDebugBoardAction)
  {
    return EFI_INVALID_PARAMETER;
  }

  Status = ChargerLibTarget_GetBatteryStatus(&BatteryStatus);
  if( EFI_SUCCESS != Status )
  {
    CHARGER_DEBUG((EFI_D_WARN, "ChargerLib:: %a Error getting battery status = %r \r\n", __FUNCTION__, Status));
    BatteryStatus.BatteryVoltage = 7600;
    Status = EFI_SUCCESS;
  }

  /*ignore return type to allow continued booting*/
  ChargerLibTarget_UsbSuspend(TRUE);
  //suspend dcin
  ChargerLibTarget_DcinSuspend(TRUE);

  if(BatteryStatus.BatteryVoltage < gChargerLibTargetCfgData.boot_to_hlos_threshold_in_mv)
  {
    CHARGER_DEBUG((EFI_D_WARN, "ChargerLib:: %a Debug board voltage = %d mV is below Threshold = %d mV \r\n", __FUNCTION__, BatteryStatus.BatteryVoltage, gChargerLibTargetCfgData.boot_to_hlos_threshold_in_mv));

    switch(DebugBoardBehaviour)
    {
      case DEBUG_BOARD_LOWVBAT_SHUTDOWN:
        Status = ChargerLibTarget_SetPon1Trigger(EFI_PM_PON_TRIGGER_CBLPWR, FALSE);
           *pDebugBoardAction = CHARGERLIB_ERROR_ACTION_SHUTDOWN;
           //display red debug icon
           ChargerLibEvent_DispBattSymbol(CHARGERLIB_EVENT_DISP_IMAGE_DEBUG_LOW_SYMBOL, TRUE);
           break;
    case DEBUG_BOARD_LOWVBAT_NOCHARGE:
        *pDebugBoardAction = CHARGERLIB_ERROR_ACTION_DEBUG_BOARD_WAIT;
        ChargerLibEvent_DispBattSymbol(CHARGERLIB_EVENT_DISP_IMAGE_DEBUG_LOW_SYMBOL, TRUE);
         break;
    case DEBUG_BOARD_BOOT_TO_HLOS:
        *pDebugBoardAction = CHARGERLIB_ERROR_ACTION_DEBUG_BOARD_GOOD_TO_BOOT;
        ChargerLibEvent_DispBattSymbol(CHARGERLIB_EVENT_DISP_IMAGE_DEBUG_BOOT_SYMBOL, TRUE);
        break;
    default:

      break;
    }
  }
  else
  {
    CHARGER_DEBUG((EFI_D_WARN, "ChargerLib:: %a: Debug board voltage = %d mV is above Threshold = %d mV \r\n", __FUNCTION__, BatteryStatus.BatteryVoltage, gChargerLibTargetCfgData.boot_to_hlos_threshold_in_mv));
    *pDebugBoardAction = CHARGERLIB_ERROR_ACTION_DEBUG_BOARD_GOOD_TO_BOOT;
  }

  return EFI_SUCCESS;
}


EFI_STATUS ChargerLibTarget_ConservativeCharging( void )
{
  EFI_STATUS Status = EFI_SUCCESS;

  /*set conservative parameter */
  /*Status = ChargerLibTarget_SetFvMaxVoltage(gChargerLibTargetCfgData.schg_cfg_data.ChgFvMax - gChargerLibTargetCfgData.conserv_chg_fv_delta);
  Status |= ChargerLibTarget_SetFccMaxCurrent(FCC_CONSERVE_CHG);

  CHARGER_DEBUG((EFI_D_WARN, "ChargerLib:: %a Disabling charging \r\n", __FUNCTION__));
  Status |= ChargerLibTarget_ChargerEnable(FALSE);*/

  return Status;
}

EFI_STATUS ChargerLibTarget_GetUnknownBatteryAction(CHARGERLIB_ERROR_ACTION_TYPE *pUnknownBatteryAction)
{
  EFI_STATUS Status               = EFI_SUCCESS;
  UINT32     UnknownBattBehaviour = gChargerLibTargetCfgData.unknown_battery_behavior;
  chargerlib_batt_status_info  BatteryStatus;

  switch (UnknownBattBehaviour)
  {
    case UNKNOWN_BATT_SHUTDOWN:
      *pUnknownBatteryAction = CHARGERLIB_ERROR_ACTION_SHUTDOWN_USB_DC_PON_DISABLED;
      break;
    case UNKNOWN_BATT_BOOT_TO_HLOS:
    /*Boot to HLOS if battery more than threshold. Else shutdown */
      Status = ChargerLibTarget_GetBatteryStatus(&BatteryStatus);
      if( EFI_SUCCESS != Status )
      {
        CHARGER_DEBUG((EFI_D_WARN, "ChargerLib:: %a Error getting battery status = %r \r\n", __FUNCTION__, Status));
        BatteryStatus.BatteryVoltage = 7600;
        Status = EFI_SUCCESS;
      }

      /*ignore return type to allow continued booting*/
      ChargerLibTarget_UsbSuspend(TRUE);
      //suspend dcin
      ChargerLibTarget_DcinSuspend(TRUE);

      if(BatteryStatus.BatteryVoltage > gChargerLibTargetCfgData.boot_to_hlos_threshold_in_mv)
      {
        CHARGER_DEBUG((EFI_D_WARN, "ChargerLib:: %a: Unknown battery detected, voltage = %d mV is above Threshold = %d mV \r\n", __FUNCTION__, BatteryStatus.BatteryVoltage, gChargerLibTargetCfgData.boot_to_hlos_threshold_in_mv));
        *pUnknownBatteryAction = CHARGERLIB_ERROR_ACTION_GOOD_TO_BOOT;
      }
      else
      {
        CHARGER_DEBUG((EFI_D_WARN, "ChargerLib:: %a: Unknown battery detected, shutting down. Voltage = %d mV is below Threshold = %d mV \r\n", __FUNCTION__, BatteryStatus.BatteryVoltage, gChargerLibTargetCfgData.boot_to_hlos_threshold_in_mv));
        Status = ChargerLibTarget_SetPon1Trigger(EFI_PM_PON_TRIGGER_CBLPWR, FALSE);
        *pUnknownBatteryAction = CHARGERLIB_ERROR_ACTION_SHUTDOWN;
      }

      break;
    case UNKNOWN_BATT_CONSERVATIVE_CHARGING:
      ChargerLibTarget_ConservativeCharging();
      break;
    case UNKNOWN_BATT_REGULAR_CHARGING:
      *pUnknownBatteryAction = CHARGERLIB_ERROR_ACTION_START_CHARGING;
      break;
    default:
      ChargerLibTarget_GetDebugBoardAction( pUnknownBatteryAction );
      break;
  }

  return Status;
}


/*This API initiates different types of reset */
EFI_STATUS ChargerLibTarget_ForceSysShutdown(ChgAppSysShutdownType ShutdownType)
{
  EFI_STATUS Status = EFI_SUCCESS;

  EFI_RESET_TYPE   ResetType = EfiResetPlatformSpecific;

  switch (ShutdownType)
  {
  case CHGAPP_RESET_SHUTDOWN:
    CHARGER_DEBUG((EFI_D_WARN, "ChargerLib:: %a CHGAPP_RESET_SHUTDOWN.\r\n", __FUNCTION__));
    ResetType = EfiResetShutdown;
    break;
  case CHGAPP_RESET_AFP:
    CHARGER_DEBUG((EFI_D_WARN, "ChargerLib:: %a CHGAPP_RESET_AFP.\r\n", __FUNCTION__));
    /*AFP not supported then enable USB as edge PON trigger and issue shut down */
      //Status = ChargerLibTarget_SetPon1Trigger(EFI_PM_PON_OPTION_PON1_REDGE_PON, TRUE);
      Status = ChargerLibTarget_SetPon1Trigger(EFI_PM_PON_TRIGGER_CBLPWR, FALSE);
    ResetType = EfiResetShutdown;
    break;
  case CHGAPP_RESET_COLD:
    CHARGER_DEBUG((EFI_D_WARN, "ChargerLib:: %a CHGAPP_RESET_COLD.\r\n", __FUNCTION__));
    ResetType = EfiResetCold;
    break;
  /* Ship mode is not supported on LA
  case CHGAPP_RESET_SHIPMODE:
    CHARGER_DEBUG((EFI_D_WARN, "ChargerLib:: %a CHGAPP_RESET_TO_SHIPMODE.\r\n", __FUNCTION__));
    if (EFI_SUCCESS != ChargerLibTarget_EnableShipMode())
    {
      *if Ship mode not supported then disable USB as PON trigger and issue shut down *
      if (NULL != PmicPwronProtocol)
      {
        Status |= PmicPwronProtocol->SetPonTrigger(PM_DEVICE_0, EFI_PM_PON_TRIGGER_DC_CHG, FALSE);
        Status |= PmicPwronProtocol->SetPonTrigger(PM_DEVICE_0, EFI_PM_PON_TRIGGER_USB_CHG, FALSE);
      }
      ResetType = EfiResetShutdown;
    }
    break;
  */

  case CHGAPP_RESET_SHUTDOWN_USB_DC_PON_DISABLED:
     /*Disable USB as PON trigger and issue shut down */
     //Status = ChargerLibTarget_SetPon1Trigger(EFI_PM_PON_OPTION_PON1_REDGE_PON, TRUE);
    Status = ChargerLibTarget_SetPon1Trigger(EFI_PM_PON_TRIGGER_CBLPWR, FALSE);

     ResetType = EfiResetShutdown;
    break;
  case CHGAPP_RESET_NONE:
  case CHGAPP_RESET_INVALID:
  default:
    /*Do nothing*/
    return Status;
  }

  /* Reset  Device*/
  gRT->ResetSystem(ResetType, EFI_SUCCESS, 0, NULL);

  return Status;
}

EFI_STATUS ChargerLibTarget_GetThermalStatus( ChargerLibThermalStatus *ThermalStatus)
{
  EFI_STATUS Status = EFI_SUCCESS;
  INT32 CurrTemp = 0;
  STATIC ChargerLibThermalStatus ThermalSts = CHARGERLIB_THERMAL_INIT; /* Start with Init */
  if((NULL == PmicFgProtocol ) || (NULL == ThermalStatus))
    return EFI_DEVICE_ERROR;

  switch (ChargerInfo.ChargerHW)
  {
    case EfiBatteryChargerQcomPmicSchg:
    case EfiBatteryChargerSMB2351:
      Status = ChargerLibTarget_GetTSensTemp(&CurrTemp);
      if (Status != EFI_SUCCESS)
      {
        CHARGER_FILE_UART_DEBUG((EFI_D_WARN,"ChargerLib:: %a Failed = %r \r\n", __FUNCTION__, Status));
        *ThermalStatus = ThermalSts = CHARGERLIB_THERMAL_STATUS_MAX;/*Updated output params*/
        return Status;
      }
    break;

    default:
      Status = EFI_UNSUPPORTED;
      break;
  }
  /*  Handle if any Error */
  if(EFI_ERROR(Status))
    return Status;

  switch(ThermalSts)
  {
    case CHARGERLIB_THERMAL_INIT:
    case CHARGERLIB_THERMAL_OK:
      if (CurrTemp >= gChargerLibTargetCfgData.thermal_configs.tsens_extreme_Temp)
      {
        *ThermalStatus = ThermalSts = CHARGERLIB_THERMAL_CRITICAL;/*Updated output params*/
      }
      else if (CurrTemp >= gChargerLibTargetCfgData.thermal_configs.tsens_high_temp )
      {
        *ThermalStatus = ThermalSts = CHARGERLIB_THERMAL_HIGH;
      }
      else
      {
        *ThermalStatus = ThermalSts = CHARGERLIB_THERMAL_OK;
      }
    break;

    case CHARGERLIB_THERMAL_HIGH:
      if (CurrTemp >= gChargerLibTargetCfgData.thermal_configs.tsens_extreme_Temp)
      {
        *ThermalStatus = ThermalSts = CHARGERLIB_THERMAL_CRITICAL;/*Updated output params*/
      }
      else if ((CurrTemp > gChargerLibTargetCfgData.thermal_configs.tsens_low_temp) && (CurrTemp < gChargerLibTargetCfgData.thermal_configs.tsens_extreme_Temp))
      {
        *ThermalStatus = ThermalSts = CHARGERLIB_THERMAL_HIGH;
      }
      else if (CurrTemp <= gChargerLibTargetCfgData.thermal_configs.tsens_low_temp)
      {
        *ThermalStatus = ThermalSts = CHARGERLIB_THERMAL_OK;
      }
      else
      {
        CHARGER_DEBUG((EFI_D_WARN,"ChargerLib:: %a Error = %d ThermalSts = %d \r\n", __FUNCTION__, CurrTemp, ThermalSts));
      }
    break;

    case CHARGERLIB_THERMAL_CRITICAL:
      /*should not hit this use case as it is already hanlded by then */
      *ThermalStatus = ThermalSts = CHARGERLIB_THERMAL_CRITICAL;/*Updated output params*/
    break;

    case CHARGERLIB_THERMAL_HIGH_TIMEOUT:
      /*should not hit this use case as it is already hanlded by then */
      *ThermalStatus = ThermalSts = CHARGERLIB_THERMAL_HIGH_TIMEOUT;/*Updated output params*/
    break;

    default:
    break;
  }

  CHARGER_DEBUG((EFI_D_WARN,"ChargerLib:: %a CurrTsensTemp = %d ThermalStatus = %d\r\n", __FUNCTION__, CurrTemp, *ThermalStatus));

  return Status;
}


EFI_STATUS ChargerLibTarget_GetTSensTemp( INT32 *TSensTemp)
{
  EFI_STATUS Status = EFI_SUCCESS;
  INT32 nTempDeciDegC = 0, nTempDegC = 0;

  STATIC EFI_TSENS_PROTOCOL *Tsens = NULL;

  if(NULL == Tsens)
  {
    Status = gBS->LocateProtocol(&gEfiTsensProtocolGuid, NULL, (VOID**)&Tsens);
    if (Status != EFI_SUCCESS)
    {
      CHARGER_FILE_UART_DEBUG((EFI_D_WARN,"ChargerLib:: %a TSENS Protocol Locate Failed %r \r\n", __FUNCTION__, Status));
      return Status;
    }
  }

  /* Loop if current temperature of device is greater than max */
  Status = Tsens->GetMaxTemp(&nTempDeciDegC);
  if (EFI_SUCCESS != Status)
  {
    CHARGER_DEBUG((EFI_D_WARN,"ChargerLib:: %a TSENS Protocol GetMaxTemp Failed = %r \r\n", __FUNCTION__, Status));
    /* break and exit to AFP ??*/
    return Status;
  }

  /*Temp is divide by 10 in C*/
  nTempDegC = nTempDeciDegC / 10;

  *TSensTemp  = nTempDegC;

  return Status;
}

EFI_STATUS ChargerLibTarget_DumpSram(BOOLEAN bDumpExplicit)
{
  EFI_STATUS    Status = EFI_SUCCESS;


  return Status;
}

EFI_STATUS ChargerLibTarget_DumpChargerPeripheral()
{
  EFI_STATUS Status = EFI_SUCCESS;

  switch (ChargerInfo.ChargerHW)
  {
    case EfiBatteryChargerQcomPmicSchg:
    case EfiBatteryChargerSMB2351:
      if((TRUE == gChargerLibTargetCfgData.dbg_cfg_Data.enable_charger_fg_Dump ) && ( !RETAIL))
      {
        if( EFI_SUCCESS == Status )
        {
          if(NULL != PmicSchgProtocol)
            Status |= PmicSchgProtocol->DumpPeripheral();
        }
      }
    break;

    default:
      Status = EFI_UNSUPPORTED;
    break;
  }

  return Status;
}

EFI_STATUS ChargerLibTarget_GetHwJeitaStatus(CHARGERLIB_HW_JEITA_STATUS *pHwJeitaStatus)
{
  EFI_STATUS  Status = EFI_SUCCESS;
  EFI_PM_SCHG_BATT_TEMP_STATUS HwJeitaStatus = { 0 };
  UINT8 activePortPmicIndex = 0;

  if (!pHwJeitaStatus)
    return EFI_INVALID_PARAMETER;

  switch (ChargerInfo.ChargerHW)
  {
  case EfiBatteryChargerQcomPmicSchg:
  case EfiBatteryChargerSMB2351:
    Status = ChargerLibTarget_GetActivePortPmicIndex(&activePortPmicIndex);
    if (Status != EFI_SUCCESS)
    {
      return Status;
    }
    Status |= PmicSchgProtocol->GetHwJeitaStatus(activePortPmicIndex, &HwJeitaStatus);
    break;

  default:
    break;
  }

  if (Status == EFI_SUCCESS)
  {
    pHwJeitaStatus->JeitaHardHot = HwJeitaStatus.jeitaHardHot;
    pHwJeitaStatus->JeitaSoftHot = HwJeitaStatus.jeitaSoftHot;
    pHwJeitaStatus->JeitaHardCold = HwJeitaStatus.jeitaHardCold;
    pHwJeitaStatus->JeitaSoftCold = HwJeitaStatus.jeitaSoftCold;
  }

  return Status;
}

EFI_STATUS ChargerLibTarget_GetBatteryTempStatus(CHARGERLIB_BATT_TEMP_STATUS *pBattTempStatus)
{
  //Battery temperature outside of operational range, extreme cold or hot condition (below -15 , above 70 degree, configurable) (AFP) -- BatteryTemperatureOutOfRangeError
  EFI_STATUS Status      = EFI_SUCCESS;
  CHARGERLIB_HW_JEITA_STATUS   HwJeitaStatus = {0};
  chargerlib_batt_status_info  BatteryStatus = {0};
  STATIC UINT8 jeitaCounter = 0;
  EFI_PM_GLINK_LINK_STATUS      PmicGLinkStatus = { 0 };

  if (!pBattTempStatus)
  {
    return EFI_INVALID_PARAMETER;
  }

  *pBattTempStatus = ChargerLib_Batt_Temp_Normal;

  Status = ChargerLibTarget_GetBatteryStatus(&BatteryStatus);
  if(EFI_SUCCESS != Status )
  {
    CHARGER_DEBUG((EFI_D_WARN, "ChargerLib::%a Error getting battery status = %r \r\n", __FUNCTION__, Status));
    BatteryStatus.BatteryVoltage = 7600;
    BatteryStatus.BatteryTemperature = 25;
    Status = EFI_SUCCESS;
  }
  CHARGER_DEBUG((EFI_D_WARN, "ChargerLib::%a BatteryTemp = %d C \r\n", __FUNCTION__, BatteryStatus.BatteryTemperature));

  if ((EFI_SUCCESS == ChargerLibTarget_GetIPCStatus(&PmicGLinkStatus)) && (PmicGLinkStatus.IsPMICChannelOpen == FALSE))
  {
    UINT16 chargingCurrent = 0, chargingVoltage = 0;
    //in recovery mode
    Status = SmartChargerLibFG_GetChargingCurrentVoltage(&chargingCurrent, &chargingVoltage);
    if ((Status == EFI_SUCCESS) &&
      (BatteryStatus.ChargeCurrent != 0) &&  //when battery current is 0, does not matter if battery voltage is higher than charging voltage. to cover EOC case
      ((INT16)chargingCurrent < BatteryStatus.ChargeCurrent || chargingVoltage < BatteryStatus.BatteryVoltage))
    {
      jeitaCounter++;
      if (jeitaCounter > 3)
      {
        CHARGER_DEBUG((EFI_D_WARN, "ChargerLib::%a charging current or voltage greater than required fg_fcc = %d mA, fg_fv = %d mV, counter = %d \r\n",
          __FUNCTION__, chargingCurrent, chargingVoltage, jeitaCounter));
        //in this condition, since no chargerPD can change charge current or voltage, need to shutdown
        //thus make the condition as outside operational range.
        *pBattTempStatus = ChargerLib_Batt_Temp_OutsideOperationalRange;
        return Status;
      }
    }
    else
    {
      //fcc and fv good, reset counter
      jeitaCounter = 0;
    }
  }
  Status = ChargerLibTarget_GetHwJeitaStatus( &HwJeitaStatus );
  if((TRUE == HwJeitaStatus.JeitaHardCold) || (TRUE == HwJeitaStatus.JeitaHardHot))
  {
    CHARGER_DEBUG((EFI_D_WARN, "ChargerLib::%a BatteryTemp out-of-range = %d C \r\n",__FUNCTION__, BatteryStatus.BatteryTemperature));
    *pBattTempStatus = ChargerLib_Batt_Temp_OutsideChargingRange;
  }

  if(( BatteryStatus.BatteryTemperature < gChargerLibTargetCfgData.fg_cfg_data.HwJeitaThreshold.JeitaCriticalTempLowLimit) ||
     ( BatteryStatus.BatteryTemperature > gChargerLibTargetCfgData.fg_cfg_data.HwJeitaThreshold.JeitaCriticalTempHighLimit))
  {
    CHARGER_DEBUG((EFI_D_WARN, "ChargerLib:: %a BatteryTemp = %d C, Operational Range lower limit = %d, upper limit = %d \r\n",
                __FUNCTION__, BatteryStatus.BatteryTemperature, gChargerLibTargetCfgData.fg_cfg_data.HwJeitaThreshold.JeitaCriticalTempLowLimit,
                  gChargerLibTargetCfgData.fg_cfg_data.HwJeitaThreshold.JeitaCriticalTempHighLimit));

    *pBattTempStatus = ChargerLib_Batt_Temp_OutsideOperationalRange;
  }

  if(EFI_SUCCESS != Status)
  {
    *pBattTempStatus = ChargerLib_Batt_Temp_Invalid;
    CHARGER_DEBUG((EFI_D_WARN, "ChargerLib::%a BatteryTemp invalid = %r \r\n",__FUNCTION__, Status));
  }

  return Status;
}

EFI_STATUS ChargerLibTarget_GetBatteryVoltageStatus(CHARGERLIB_VBATT_STATUS *pBatteryVoltageStatus)
{
  EFI_STATUS                   Status      = EFI_SUCCESS;
  STATIC UINT32                EmergencyShutdownCounter = 0;
  chargerlib_batt_status_info  BatteryStatus = {0};

  if (!pBatteryVoltageStatus)
  {
    return EFI_INVALID_PARAMETER;
  }

  *pBatteryVoltageStatus = ChargerLib_VBatt_Normal;

  Status = ChargerLibTarget_GetBatteryStatus(&BatteryStatus);
  if( EFI_SUCCESS != Status )
  {
    CHARGER_DEBUG((EFI_D_WARN, "ChargerLib:: %a Error getting battery status = %r \r\n", __FUNCTION__, Status));
    BatteryStatus.BatteryVoltage = 7600;
    Status = EFI_SUCCESS;
  }

  CHARGER_DEBUG((EFI_D_WARN, "ChargerLib:: %a Battery voltage = %d mV \r\n", __FUNCTION__, BatteryStatus.BatteryVoltage));

  /* Return status if vbatt is good */
  if ((UINT32)BatteryStatus.BatteryVoltage > (gChargerLibTargetCfgData.schg_cfg_data.ChgFvMax + gChargerLibTargetCfgData.batt_volt_lim_high_delta))
  {
    EmergencyShutdownCounter = 0;

    Status = ChargerLibTarget_GetBatteryStatus(&BatteryStatus);
    if( EFI_SUCCESS != Status )
    {
      CHARGER_DEBUG((EFI_D_WARN, "ChargerLib:: %a Error getting battery status = %r \r\n", __FUNCTION__, Status));
      BatteryStatus.BatteryVoltage = 7600;
      Status = EFI_SUCCESS;
    }

    CHARGER_DEBUG((EFI_D_WARN, "ChargerLib:: %a Current Battery voltage =  %d mV \r\n", __FUNCTION__, BatteryStatus.BatteryVoltage));

    /* Check for High Voltage Limit */
    if (((UINT32)(BatteryStatus.BatteryVoltage - VBATT_TOLERANCE)) > (gChargerLibTargetCfgData.schg_cfg_data.ChgFvMax + gChargerLibTargetCfgData.batt_volt_lim_high_delta))
    {
      CHARGER_DEBUG((EFI_D_WARN, "ChargerLib:: %a Current Battery voltage too high %d mV, Threshold = %d mV  \r\n", __FUNCTION__,
                                BatteryStatus.BatteryVoltage, (gChargerLibTargetCfgData.schg_cfg_data.ChgFvMax + gChargerLibTargetCfgData.batt_volt_lim_high_delta)));
      *pBatteryVoltageStatus = ChargerLib_VBatt_TooHigh;
    }
  }
  /* Check for vbatt below emergency shutdown limit */
  else if ((UINT32)BatteryStatus.BatteryVoltage < gChargerLibTargetCfgData.emergency_shutdown_vbatt)
  {
    if(BatteryStatus.ChargeCurrent > 0) /* Battery is discharging */
    {
      CHARGER_DEBUG((EFI_D_WARN, "ChargerLib:: %a Current BattVoltage = %d mV is below EmergencyShutdownVbatt = %d mV ", __FUNCTION__,
                    BatteryStatus.BatteryVoltage, gChargerLibTargetCfgData.emergency_shutdown_vbatt));
    CHARGER_DEBUG((EFI_D_WARN, " EmergencyShutdownCounter = %d \r\n", EmergencyShutdownCounter));
      if(EmergencyShutdownCounter >= MAX_EMERGENCY_SHUTDOWN_COUNT)
      {
        *pBatteryVoltageStatus = ChargerLib_VBatt_BelowThreshold;
        CHARGER_DEBUG((EFI_D_WARN, "ChargerLib:: %a EmergencyShutdownCounter Exceeded : \r\n", __FUNCTION__ ));
    CHARGER_DEBUG((EFI_D_WARN, " Curr Battery voltage = %d mV is below threshold = %d mV \r\n",
          BatteryStatus.BatteryVoltage, gChargerLibTargetCfgData.emergency_shutdown_vbatt ));
      }
      else
      {
        CHARGER_DEBUG((EFI_D_WARN, "ChargerLib:: %a EmergencyShutdownCounter = %d \r\n", __FUNCTION__, EmergencyShutdownCounter));
      }
      EmergencyShutdownCounter++;
    }
    else
    {
      EmergencyShutdownCounter = 0;
    }
  }
  else
  {
    EmergencyShutdownCounter = 0;
  }

  return Status;
}

EFI_STATUS ChargerLibTarget_EnableHWJeita(BOOLEAN Enable)
{
  EFI_STATUS Status = EFI_SUCCESS;

  /*switch (ChargerInfo.ChargerHW)
  {
    case EfiBatteryChargerQcomPmicSchg:
    if (TRUE == Enable)
    {
      if (!PmicSchgProtocol)
      {
        Status = gBS->LocateProtocol(&gQcomPmicSchgProtocolGuid, NULL, (VOID **)&PmicSchgProtocol);
      }

      if ((NULL == PmicSchgProtocol) || (Status != EFI_SUCCESS))
      {
        return EFI_DEVICE_ERROR;
      }

      Status = PmicSchgProtocol->EnableHwJeita(ChargerInfo.ChargerPmicInfo.PmicIndex, Enable);
    }
    break;

    default:
      Status = EFI_UNSUPPORTED;
      break;
    }*/
  return Status;
}

EFI_STATUS ChargerLibTarget_GetSdpMaxCurrent(UINT32 *pSdpMaxCurrent)
{
  EFI_STATUS Status = EFI_SUCCESS;

  if(NULL == pSdpMaxCurrent)
  {
    return EFI_INVALID_PARAMETER;
  }

  *pSdpMaxCurrent = USB_2_IUSB_MAX;


  return Status;
}


EFI_STATUS ChargerLibTarget_GetChgWdogStatus( CHARGERLIB_CHG_WDOG_STS_TYPE *pChgWdogSts)
{
  EFI_STATUS Status = EFI_SUCCESS;

  if(NULL == pChgWdogSts)
    return EFI_INVALID_PARAMETER;

  switch (ChargerInfo.ChargerHW)
  {
    case EfiBatteryChargerQcomPmicSchg:
    case EfiBatteryChargerSMB2351:
      /* Locate SCHG Protocol */
      if (!PmicSchgProtocol)
      {
        Status |= gBS->LocateProtocol( &gQcomPmicSchgProtocolGuid,
                                      NULL,
                                     (VOID **)&PmicSchgProtocol );
      }

      if((EFI_SUCCESS == Status) && (NULL != PmicSchgProtocol))
      {
        UINT8 activePortPmicIndex = 0;
        Status = ChargerLibTarget_GetActivePortPmicIndex(&activePortPmicIndex);
        if (Status != EFI_SUCCESS)
        {
          return Status;
        }
        Status = PmicSchgProtocol->GetChgWdogStatus(activePortPmicIndex, (EFI_PM_SCHG_WDOG_STS_TYPE *)pChgWdogSts);
      }
      break;
    default:
      return EFI_UNSUPPORTED;
      break;

  }

  return Status;
}

EFI_STATUS ChargerLibTarget_InitializeCharging( VOID )
{
  EFI_STATUS Status = EFI_SUCCESS;

  CHARGER_DEBUG((EFI_D_WARN, "ChargerLib:: %a Entry \r\n", __FUNCTION__));

  return Status;
}

EFI_STATUS ChargerLibTarget_EnableWdog(BOOLEAN Enable)
{
  EFI_STATUS Status = EFI_SUCCESS;

  /*switch (ChargerInfo.ChargerHW)
  {
    case EfiBatteryChargerQcomPmicSchg:
      Status = PmicSchgProtocol->EnableChgWdog(ChargerInfo.ChargerPmicInfo.PmicIndex, Enable);
      break;
    default:
      Status = EFI_UNSUPPORTED;
      break;
  }*/

  return Status;
}

EFI_STATUS ChargerLibTarget_PetChgWdog( VOID )
{
  EFI_STATUS Status = EFI_SUCCESS;

  /*switch (ChargerInfo.ChargerHW)
  {
    case EfiBatteryChargerQcomPmicSchg:
      Status = PmicSchgProtocol->PetChgWdog(ChargerInfo.ChargerPmicInfo.PmicIndex);
      break;
    default:
      Status = EFI_UNSUPPORTED;
      break;
  }*/

  return Status;
}

/**
  Charger Lib Exit

  @return
  EFI_SUCCESS:           Function returned successfully.
  EFI_DEVICE_ERROR:      The physical device reported an error.
  EFI_UNSUPPORTED:       No Library function linked
*/
EFI_STATUS ChargerLibTarget_Exit( VOID )
{
  EFI_STATUS     Status      = EFI_SUCCESS;

  STATIC BOOLEAN ChgExitDone = FALSE;

  if(FALSE == ChgExitDone)
  {
    CHARGER_DEBUG((EFI_D_WARN, "ChargerLibTarget:: %a ChargerLibExit \r\n", __FUNCTION__));
    /* ChargerExit is called only once */
    ChgExitDone = TRUE;

  }

  switch (BatteryGaugeInfo.BatteryGaugeHW)
  {
    case EfiBatteryGaugeQcomPmicFg:
    break;

    default:
      Status = EFI_UNSUPPORTED;
    break;
  }



  return EFI_SUCCESS;
}

EFI_STATUS ChargerLibTarget_GetChargingStatus(BOOLEAN *pChargingEnabled)
{
  EFI_STATUS Status = EFI_SUCCESS;
  EFI_PM_GLINK_LINK_STATUS      PmicGLinkStatus = { 0 };
  EFI_PM_GLINK_BATT_STATUS_INFO BattStsInfo = { 0 };
  EFI_PM_SCHG_CHARGING_STATUS ChargingSts = { 0 };

  if(!pChargingEnabled)
    return EFI_INVALID_PARAMETER;

  *pChargingEnabled = FALSE;

  if ((EFI_SUCCESS == ChargerLibTarget_GetIPCStatus(&PmicGLinkStatus)) && (PmicGLinkStatus.IsPMICChannelOpen == TRUE))
  {
    Status = pmicGlinkProtocol->GetBatteryStatus(BatteryIndex, &BattStsInfo);
    if(EFI_SUCCESS != Status)
    {
      CHARGER_DEBUG((EFI_D_WARN, "ChargerLib:: %a Status: %r \r\n",__FUNCTION__, Status));
      return EFI_DEVICE_ERROR;
    }
    else if (BattStsInfo.ChargeCurrent > 0 || BattStsInfo.ChargingSource < PMIC_GLINK_CHARGING_SOURCE_TYPE_MAX)
    {
      *pChargingEnabled = TRUE;
    }
  }
  else
  {
    //get from pmic
    UINT8 activePortPmicIndex = 0;
    switch (ChargerInfo.ChargerHW)
    {
    case EfiBatteryChargerQcomPmicSchg:
    case EfiBatteryChargerSMB2351:
      Status = ChargerLibTarget_GetActivePortPmicIndex(&activePortPmicIndex);
      if (Status != EFI_SUCCESS)
      {
        return Status;
      }
      Status = PmicSchgProtocol->GetChargingStatus(activePortPmicIndex, &ChargingSts);
      if (EFI_SUCCESS == Status)
      {
        *pChargingEnabled = ChargingSts.bChargingEnabled;
      }
      break;
    default:
      Status = EFI_UNSUPPORTED;
      break;
    }
  }

  return EFI_SUCCESS;
}

EFI_STATUS ChargerLibTarget_PrintDCInStatus()
{
  EFI_STATUS Status = EFI_SUCCESS;
  EfiAdcResultType AdcResult = { 0 };

  Status = ChargerLibTarget_GetADCReading(ADC_INPUT_DC_IN, sizeof(ADC_INPUT_DC_IN), &AdcResult);

  if (EFI_ERROR(Status))
  {
    CHARGER_DEBUG((EFI_D_ERROR, "ChargerLib:: %a: Failed to get ADC reading from ADC_INPUT_DC_IN!\n", __FUNCTION__));
    return Status;
  }

  CHARGER_DEBUG((EFI_D_ERROR, "ChargerLib:: %a: DCInVoltage = %d\n", __FUNCTION__, AdcResult.nPhysical));

  Status = ChargerLibTarget_GetADCReading(ADC_INPUT_DC_IN_I, sizeof(ADC_INPUT_DC_IN_I), &AdcResult);
  if (EFI_ERROR(Status))
  {
    CHARGER_DEBUG((EFI_D_ERROR, "ChargerLib:: %a: Failed to get ADC reading from ADC_INPUT_DC_IN_I!\n", __FUNCTION__));
    return Status;
  }

  CHARGER_DEBUG((EFI_D_ERROR, "ChargerLib:: %a: DCInCurrent = %d\n", __FUNCTION__, AdcResult.nPhysical));
  return Status;
}

EFI_STATUS ChargerLibTarget_GetADCReading(CONST CHAR8 *pszInputName, UINT32 uInputNameSize, EfiAdcResultType *pEfiAdcResult)
{
  EFI_STATUS Status = EFI_SUCCESS;
  EfiAdcDeviceChannelType channel;

  if (NULL == pszInputName || NULL == pEfiAdcResult)
  {
    return EFI_INVALID_PARAMETER;
  }

  if (!AdcProtocol)
  {
    Status = gBS->LocateProtocol(&gEfiAdcProtocolGuid,
      NULL,
      (VOID**)&AdcProtocol);
  }

  if ((EFI_ERROR(Status)) || (NULL == AdcProtocol))
  {
    CHARGER_DEBUG((EFI_D_ERROR, "ChargerLib:: %a: Failed to load ADC Protocol!\n", __FUNCTION__));
    return EFI_DEVICE_ERROR;
  }

  Status = AdcProtocol->GetChannel(pszInputName, uInputNameSize, &channel);
  if (EFI_ERROR(Status))
  {
    CHARGER_DEBUG((EFI_D_ERROR, "ChargerLib:: %a: Failed to get ADC Channel!\n", __FUNCTION__ ));
    return EFI_DEVICE_ERROR;
  }

  Status = AdcProtocol->AdcRead(&channel, pEfiAdcResult);
  if ((EFI_ERROR(Status)) || (EFI_ADC_RESULT_INVALID == pEfiAdcResult->eStatus))
  {
    CHARGER_DEBUG((EFI_D_ERROR, "ChargerLib:: %a: Failed to read ADC Channel!\n", __FUNCTION__));
    return EFI_DEVICE_ERROR;
  }

  return Status;
}

EFI_STATUS ChargerLibTarget_SetUSbICLMode(CHARGERLIB_USB_ICL_MODE_TYPE IclModeType)
{
  EFI_STATUS Status = EFI_SUCCESS;

  //switch (ChargerInfo.ChargerHW)
  //{
  //  case EfiBatteryChargerQcomPmicSchg:
  //    /* Locate SCHG Protocol */
  //    if (!PmicSchgProtocol)
  //    {
  //      Status |= gBS->LocateProtocol( &gQcomPmicSchgProtocolGuid,
  //                                    NULL,
  //                                   (VOID **)&PmicSchgProtocol );
  //    }

  //    if( EFI_SUCCESS == Status )
  //    {
  //      Status = PmicSchgProtocol->SetUsbIclMode(ChargerInfo.ChargerPmicInfo.PmicIndex, (EFI_PM_SCHG_USB_ICL_MODE_TYPE) IclModeType);
  //    }
  //    break;
  //  default:
  //    break;
  //}

  return Status;
}

EFI_STATUS ChargerLibTarget_ConfigureUsbCurrentSetting( VOID )
{

  return EFI_SUCCESS;
}

EFI_STATUS ChargerLibTarget_LedOn( BOOLEAN bLedOn)
{
  EFI_STATUS Status = EFI_SUCCESS;
  STATIC EFI_QCOM_PMIC_RGB_LED_PROTOCOL *PmicRgbLedProtocol = NULL;
  static BOOLEAN ledOnStatus = FALSE;

  switch (ChargerInfo.ChargerHW)
  {
    case EfiBatteryChargerQcomPmicSchg:
    case EfiBatteryChargerSMB2351:
      if (!PmicRgbLedProtocol)
      {
        Status = gBS->LocateProtocol(&gQcomPmicRgbLedProtocolGuid, NULL, (VOID **)&PmicRgbLedProtocol);
      }
      if ((Status != EFI_SUCCESS) || (NULL == PmicRgbLedProtocol))
      {
        CHARGER_DEBUG((EFI_D_WARN, "ChargerLib:: %a Protocol Failed = %r \r\n", __FUNCTION__, Status));
        return EFI_DEVICE_ERROR;
      }
      if (bLedOn == TRUE)
      {
        /* Check if LED has already been turned on */
        if (ledOnStatus != bLedOn)
        {
          /* Change Led state irrespective of charging status called to make sure of charging status */
          Status |= PmicRgbLedProtocol->ConfigRgbLed(PM_DEVICE_2, EFI_PM_RGB_1, EFI_PM_RGB_LED_RED, EFI_PM_RGB_DIM_LEVEL_MID, bLedOn);
          /*  Handle if any Error */
          if (Status != EFI_SUCCESS)
          {
            CHARGER_DEBUG((EFI_D_WARN, "ChargerLib:: %a Failed = %r \r\n", __FUNCTION__, Status));
            return Status;
          }
          else
          {
            ledOnStatus = TRUE;
            CHARGER_DEBUG((EFI_D_WARN, "ChargerLib:: %a LED TURNED ON = %r \r\n", __FUNCTION__, Status));
          }
        }
      }
      else
      {
        /* Turn OFF LED irrespective of charging status */
        Status |= PmicRgbLedProtocol->ConfigRgbLed(PM_DEVICE_2, EFI_PM_RGB_1, EFI_PM_RGB_LED_RED, EFI_PM_RGB_DIM_LEVEL_MID, bLedOn);
        /*  Handle if any Error */
        if (Status != EFI_SUCCESS)
        {
          CHARGER_DEBUG((EFI_D_WARN, "ChargerLib:: %a Failed = %r \r\n", __FUNCTION__, Status));
          return Status;
        }
        else
        {
          ledOnStatus = FALSE;
          CHARGER_DEBUG((EFI_D_WARN, "ChargerLib:: %a LED TURNED OFF = %r \r\n", __FUNCTION__, Status));
        }
      }
      break;

    default:
      Status = EFI_UNSUPPORTED;
    break;
  }
  return Status;
}

EFI_STATUS ChargerLibTarget_SetDcinCurrent(UINT32 DcinCurrentInmA)
{
  EFI_STATUS Status = EFI_SUCCESS;

 /* CHARGER_DEBUG((EFI_D_WARN, "ChargerLib:: %a ChargerLib Current = %d mA\r\n", __FUNCTION__, DcinCurrentInmA));

  switch (ChargerInfo.ChargerHW)
  {
  case EfiBatteryChargerQcomPmicSchg:
    Status = PmicSchgProtocol->SetDcinCurrent(ChargerInfo.ChargerPmicInfo.PmicIndex, DcinCurrentInmA);
    break;
  default:
    Status = EFI_UNSUPPORTED;
    break;
  }*/

  return Status;
}

EFI_STATUS ChargeLibTarget_IsDcinValid(BOOLEAN* pIsValid)
{
  if (!pIsValid)
  {
    return EFI_INVALID_PARAMETER;
  }

  return EFI_SUCCESS;
}


void ChargerLibTarget_HWInit(chargerlib_cfgdata_type *pChargerLibConfig, EFI_PLATFORMINFO_PLATFORM_TYPE PlatformType)
{
  EFI_STATUS Status = EFI_SUCCESS;
  UINT32  plat_index = 0;
  CHARGERLIB_PLATFORM_HW_CONFIG_TYPE config_index = CHARGERLIB_PLATFORM_HW_CONFIG_INVALID;
  UINT32  HwInfoSize = (UINT32)(sizeof(ChargerLibPlatformHwInfo)/sizeof(CHARGERLIB_PLATFORM_HW_CONFIG_INFO));

  //Initialize Charger and Battery GaugeInfo
  ChargerInfo.ChargerHW           = EfiBatteryChargerNone;
  ChargerInfo.Version             = 0x00010000;
  BatteryGaugeInfo.BatteryGaugeHW = EfiBatteryGaugeNone;
  BatteryGaugeInfo.Version        = 0x00010000;

  //Read Flag from Config Item to see if we need override the setting
  if (pChargerLibConfig->plat_hw_cfg_override < CHARGERLIB_PLATFORM_HW_CONFIG_INVALID)
  {
    CHARGER_DEBUG((EFI_D_WARN, "ChargerLib:: %a Charger PlatHWConfig = %d, (overrided by CFG item)\r\n",
                   __FUNCTION__, pChargerLibConfig->plat_hw_cfg_override));

    config_index = pChargerLibConfig->plat_hw_cfg_override;

    ChargerInfo.ChargerHW =           ChargerLibHwConfigTable[config_index].charger_hw_type;
    ChargerInfo.Version =             ChargerLibHwConfigTable[config_index].charger_hw_version;
    BatteryGaugeInfo.BatteryGaugeHW = ChargerLibHwConfigTable[config_index].gauge_hw_type;
    BatteryGaugeInfo.Version        = ChargerLibHwConfigTable[config_index].gauge_hw_version;
  }
  else
  {
    //Assign HWInfo Type based on the table
    for (plat_index = 0; plat_index < HwInfoSize; plat_index++)
    {
      if (ChargerLibPlatformHwInfo[plat_index].PlatformType == PlatformType)
      {
        //Execute HW Config Detect Function if available
        if (ChargerLibPlatformHwInfo[plat_index].pHwConfigDetectFunc != NULL)
        {
          Status = ChargerLibPlatformHwInfo[plat_index].pHwConfigDetectFunc(&ChargerInfo, &BatteryGaugeInfo);
          if (EFI_SUCCESS != Status)
          {
              CHARGER_DEBUG((EFI_D_ERROR, "ChargerLib:: %a ERROR: Platform HW detection function failed %r \r\n",
                   __FUNCTION__, Status));
          }
        }
        //Else, perform static assignment
        else
        {
          config_index = ChargerLibPlatformHwInfo[plat_index].StaticHwConfigIndex;
          if (config_index < CHARGERLIB_PLATFORM_HW_CONFIG_INVALID)
          {
            ChargerInfo.ChargerHW =           ChargerLibHwConfigTable[config_index].charger_hw_type;
            ChargerInfo.Version =             ChargerLibHwConfigTable[config_index].charger_hw_version;
            BatteryGaugeInfo.BatteryGaugeHW = ChargerLibHwConfigTable[config_index].gauge_hw_type;
            BatteryGaugeInfo.Version        = ChargerLibHwConfigTable[config_index].gauge_hw_version;

            pChargerLibConfig->plat_hw_cfg_override = config_index;

            CHARGER_DEBUG((EFI_D_WARN, "ChargerLib:: %a PlatformType = %d has Static PlatHWConfig = %d \r\n",
                           __FUNCTION__, PlatformType, config_index));
          }
          else
          {
            CHARGER_DEBUG((EFI_D_ERROR, "ChargerLib:: %a ERROR: Invalid Static Platform HW assignment \r\n",
                           __FUNCTION__, Status));
          }
        }
        break;
      }
    }
  }

  CHARGER_DEBUG((EFI_D_WARN, "ChargerLib:: %a PlatformType = %d: ChargerHW = %d, GaugeHW = %d \r\n",
                 __FUNCTION__, PlatformType, ChargerInfo.ChargerHW, BatteryGaugeInfo.BatteryGaugeHW));

  return;
}

EFI_STATUS ChargerLibTarget_IsChargingSupported(BOOLEAN *pChargingSupported)
{
  EFI_STATUS Status = EFI_SUCCESS;
  EFI_PLATFORMINFO_PLATFORM_TYPE  PlatformType;

  Status = GetPlatformType(&PlatformType);
  if(EFI_SUCCESS != Status)
  {
    CHARGER_DEBUG((EFI_D_WARN, "ChargerLib:: %a  Error getting PlatformType  \r\n", __FUNCTION__));
    return EFI_DEVICE_ERROR;
  }

  //Checking Configuration
  if ((EfiBatteryChargerNone == ChargerInfo.ChargerHW) ||
      (EfiBatteryGaugeNone == BatteryGaugeInfo.BatteryGaugeHW))
  {
    *pChargingSupported = FALSE;
  }
  else
  {
    //check if charging is supported or not
    UINT8 activePort = 0;
    if (!PmicSchgProtocol)
    {
      Status = gBS->LocateProtocol(&gQcomPmicSchgProtocolGuid, NULL, (VOID **)&PmicSchgProtocol);
    }

    if ((NULL == PmicSchgProtocol) || (Status != EFI_SUCCESS))
      return EFI_DEVICE_ERROR;
    //set testmode_discharging flag matter charging supported or not
    //when DISABLEBATTERY is set, that means in test mode we will show power on line and charging
    //when DISABLEBATTERY is not set, in test mode we will show discharging.
    (void)PmicSchgProtocol->SetTestModeDischarging(gChargerLibTargetCfgData.dbg_cfg_Data.DISABLEBATTERY);
    if (gChargerLibTargetCfgData.ForceOffmodePowerTest)
    {
      *pChargingSupported = TRUE;
    }
    else
    {
    Status = PmicSchgProtocol->GetActivePort(&activePort);
    if (EFI_SUCCESS == Status)
    {
      //check if it is fake battery
      BOOLEAN fake_batt = FALSE;
      Status = PmicSchgProtocol->GetBattMissingStatus(activePort, EFI_PM_SCHG_FAKE_BAT_DET, &fake_batt);
      if(fake_batt)
      {
        *pChargingSupported = FALSE;
      }
      else
      {
      *pChargingSupported = TRUE;
    }
    }
    else
    {
      *pChargingSupported = FALSE;
      //set back to success.
      Status = EFI_SUCCESS;
    }
    }
  }

  CHARGER_DEBUG((EFI_D_WARN, "ChargerLib:: %a PlatformType = %d: ChargerHW = %d, GaugeHW = %d \r\n",
                 __FUNCTION__, PlatformType, ChargerInfo.ChargerHW, BatteryGaugeInfo.BatteryGaugeHW));

  //Add HW Validation here in the future

  CHARGER_DEBUG((EFI_D_WARN, "ChargerLib:: %a Support Charging = %d\r\n", __FUNCTION__, *pChargingSupported));
  return Status;
}


EFI_STATUS ChargerLibTarget_SetPon1Trigger(EFI_PM_PON_TRIGGER_TYPE bit_type, BOOLEAN bFlag)
{
  EFI_STATUS Status = EFI_SUCCESS;

  /* Enable USB and DCIN PON triggers to be certain and avoid bad use case*/
  if (!PmicPwronProtocol)
  {
    Status |= gBS->LocateProtocol(&gQcomPmicPwrOnProtocolGuid, NULL, (VOID **)&PmicPwronProtocol);
  }

  if((Status == EFI_SUCCESS) && (NULL != PmicPwronProtocol))
  {
    Status |= PmicPwronProtocol->SetPonTrigger(PM_DEVICE_0, bit_type, bFlag);
  }
  else
  {
    CHARGER_DEBUG((EFI_D_WARN, "ChargerLib:: %a Error locating Pwron Protocol = %r \r\n", __FUNCTION__, Status));
  }

  return Status;
}

EFI_STATUS ChargerLibTarget_GetIPCStatus(EFI_PM_GLINK_LINK_STATUS *PmicGLinkStatus)
{
  EFI_STATUS  Status            = EFI_SUCCESS;

  if (NULL == PmicGLinkStatus)
    return EFI_INVALID_PARAMETER;

  if (!gChargerLibTargetCfgData.enable_charger_fw)
  {
    PmicGLinkStatus->IsPMICChannelOpen = FALSE;
  }

  /* Return If Link is not up */
  if(NULL == pmicGlinkProtocol)
  {
    Status = gBS->LocateProtocol (&gpmicGlinkProtocolGuid,NULL, (VOID **) &pmicGlinkProtocol);
    if((EFI_SUCCESS != Status) || (NULL == pmicGlinkProtocol))
    {
      CHARGER_DEBUG(( EFI_D_ERROR, "ChargerLib:: %a pmicGlinkProtocol Failed %r \r\n", __FUNCTION__, Status));
      return EFI_UNSUPPORTED;
    }
  }

  Status |= pmicGlinkProtocol->LinkStatus(PmicGLinkStatus);
  if(EFI_SUCCESS != Status)
  {
    CHARGER_DEBUG(( EFI_D_ERROR, "ChargerLib:: %a pmicGlinkProtocol Failed %r \r\n", __FUNCTION__, Status));
  }
  return Status;

}

EFI_STATUS ChargerLibTarget_GetIPCError(CHARGERLIB_CHARGING_ERROR_TYPES  *pChargingError)
{
  EFI_STATUS               Status          = EFI_SUCCESS;
  EFI_PM_GLINK_LINK_STATUS PmicGLinkStatus = { 0 };

  if(NULL == pChargingError)
    return EFI_INVALID_PARAMETER;

  if (!gChargerLibTargetCfgData.enable_charger_fw)
  {
    *pChargingError = CHARGERLIB_CHARGING_FW_LINK_ERROR;
    return Status;
  }

  Status = ChargerLibTarget_GetIPCStatus(&PmicGLinkStatus);

  if((EFI_SUCCESS != Status) || (PmicGLinkStatus.IsPMICChannelOpen == FALSE))
  {
    CHARGER_DEBUG(( EFI_D_ERROR, "ChargerLib:: %a Glink Channel is not Open = %r Open = %d \r\n", __FUNCTION__, Status, PmicGLinkStatus.IsPMICChannelOpen));
    *pChargingError = CHARGERLIB_CHARGING_FW_LINK_ERROR;
    return Status;
  }
  else
  {
    /* Glink channel is open and connected */
    *pChargingError = CHARGERLIB_CHARGING_ERROR_NONE;
  }

  return Status;

}

#define QCOM_CHARGER_FW_PARTITION_STRING_W L"adsp"
#define QCOM_CHARGER_FW_PARTITION_STRING   "adsp"

EFI_STATUS ChargerLibTarget_LoadChargerFW()
{
  EFI_PIL_PROTOCOL  *PILProtocol = NULL;
  EFI_STATUS         Status      = EFI_SUCCESS;

  if (!gChargerLibTargetCfgData.enable_charger_fw)
  {
    return Status;
  }
  Status = gBS->LocateProtocol (&gEfiPilProtocolGuid,NULL, (VOID **) &PILProtocol);
  if ((EFI_SUCCESS != Status) && (NULL == PILProtocol))
  {
    CHARGER_DEBUG(( EFI_D_ERROR, "ChargerLib:: %a %r \r\n", __FUNCTION__, Status));
    return Status;
  }
  
  Status = ChargerLibTarget_SetOS(OS_BOOTLOADER);
  if (EFI_SUCCESS != Status)
  {
    return Status;  
  }

  Status |= MountFatPartition(QCOM_CHARGER_FW_PARTITION_STRING_W);
  CHARGER_DEBUG(( EFI_D_ERROR, "MountFatPartition %r \r\n", Status));

   Status = PILProtocol->ProcessPilImage(L"ADSPPD");
   CHARGER_DEBUG(( EFI_D_ERROR, "ChargerLib:: %a ProcessPilImageExt = %r \r\n", __FUNCTION__, Status));
   gBS->Stall(CHARGERLIB_ADSP_LOAD_WAIT_TIME);//WAIT 1 MS
  return Status;

}

EFI_STATUS ChargerLibTarget_LinkChargerFW()
{
  EFI_STATUS Status = EFI_SUCCESS;
  EFI_PMIC_GLINK_PROTOCOL  *pmicGlinkProtocol = NULL;
  EFI_PM_GLINK_BATT_STATUS_INFO BattStsInfo = {0};
  UINT32 count =0;
  EFI_PM_GLINK_LINK_STATUS PmicGLinkStatus = { 0 };

  if (!gChargerLibTargetCfgData.enable_charger_fw)
  {
    return Status;
  }

  if(NULL == pmicGlinkProtocol)
  {
   Status = gBS->LocateProtocol (&gpmicGlinkProtocolGuid,NULL, (VOID **) &pmicGlinkProtocol);
   if((EFI_SUCCESS != Status) && (NULL == pmicGlinkProtocol))
   {
     CHARGER_DEBUG(( EFI_D_ERROR, "ChargerLib:: %a pmicGlinkProtocol Failed %r \r\n", __FUNCTION__, Status));
     return Status;
   }
  }

  gBS->Stall(CHARGERLIB_IPC_PRE_LINK_WAIT_TIME);

  Status |= pmicGlinkProtocol->Connect();
  if(EFI_SUCCESS != Status)
  {
   CHARGER_DEBUG(( EFI_D_ERROR, "ChargerLib:: %a pmicGlinkProtocol Connect Failed %r \r\n", __FUNCTION__, Status));
   return Status;
  }
  else
  {
   CHARGER_DEBUG(( EFI_D_WARN, "ChargerLib:: %a pmicGlinkProtocol Connect %r \r\n", __FUNCTION__, Status));
  }

  /* Wait Till ADSP link comes up  */
  while (count < CHARGERLIB_IPC_MAX_LINK_WAIT_COUNT)
  {
    gBS->Stall(CHARGERLIB_IPC_LINK_WAIT_TIME);
    Status |= pmicGlinkProtocol->LinkStatus(&PmicGLinkStatus);
    if(EFI_SUCCESS != Status)
    {
      break;
    }
    CHARGER_DEBUG(( EFI_D_WARN, "ChargerLib:: %a PmicGLinkStatus = %d, Count = %d \r\n", __FUNCTION__, PmicGLinkStatus.IsPMICChannelOpen, count));
    if (PmicGLinkStatus.IsPMICChannelOpen == TRUE)
    {
      break;
    }
    count++;
  }

  /* Debug Check */
  if(PmicGLinkStatus.IsPMICChannelOpen == TRUE)
  {
    Status |= pmicGlinkProtocol->GetBatteryStatus(BatteryIndex, &BattStsInfo);
    if(EFI_SUCCESS == Status)
    {
      //here the chargerpd is up and communication is good, so can deinit i2c on uefi side
      SmartChargerLibFG_Deinit();
      CHARGER_DEBUG(( EFI_D_ERROR, "ChargerLib:: %a turned off i2c_power, GetBatteryStatus = %r BatteryVoltage = %d v \r\n", __FUNCTION__, Status, BattStsInfo.BatteryVoltage));
    }
    else
    {
    CHARGER_DEBUG(( EFI_D_ERROR, "ChargerLib:: %a pmicGlinkProtocol Channel not open %r %d \r\n", __FUNCTION__, Status, PmicGLinkStatus.IsPMICChannelOpen));
    }
  }
  return Status;
}

EFI_STATUS ChargerLibTarget_DAMConnectSts(BOOLEAN *DAMConnectSts)
{
    EFI_STATUS Status = EFI_SUCCESS;

    *DAMConnectSts = FALSE;

    return Status;
}

EFI_STATUS ChargerLibTarget_SetDAMIcl(DAM_SEL_ICL DAMIcl)
{
    EFI_STATUS Status = EFI_SUCCESS;

    return Status;
}



EFI_STATUS ChargerLibTarget_GetErrorAction( CHARGERLIB_CHARGING_ERROR_TYPES  ChargingErrorType, CHARGERLIB_ERROR_ACTION_TYPE *pErrorAction)
{
  EFI_STATUS  Status = EFI_SUCCESS;
  chargerlib_batt_status_info BatteryStatus = {0};
  EFI_PM_PON_REASON_TYPE PM_PonReasonType = { 0 };


  if(!pErrorAction)
  {
    return EFI_INVALID_PARAMETER;
  }

  *pErrorAction = CHARGERLIB_ERROR_ACTION_NONE;

  Status = ChargerLib_GetBatteryStatus(&BatteryStatus);
  if( EFI_SUCCESS != Status )
  {
    CHARGER_DEBUG((EFI_D_WARN, "ChargerLib:: %a Error getting battery status = %r \r\n", __FUNCTION__, Status));
    return Status;
  }

  switch (ChargingErrorType)
  {
    case CHARGERLIB_CHARGING_ERROR_NONE:
    case CHARGERLIB_CHARGING_ERROR_TSENSE_OK:
        /* No Action just continue */
        break;
    case CHARGERLIB_CHARGING_ERROR_TSENSE_CRITICAL:
         // case ChargerLib_ChargingError_TsensCritical:
       /* Display LED in case of CLS platform ?*/
         ChargerLibEvent_DispBattSymbol(CHARGERLIB_EVENT_DISP_IMAGE_TSENS_CRITICAL_SYMBOL, TRUE);
    case CHARGERLIB_CHARGING_ERROR_BATT_TEMP_OUTSIDE_OPERATIONAL_RANGE:
         *pErrorAction = CHARGERLIB_ERROR_ACTION_CRITICAL_SHUTDOWN;
        break;
    case CHARGERLIB_CHARGING_ERROR_BATTERY_NOT_DETECTED:
       /* No display needed for CLS platform */
         ChargerLibEvent_DispBattSymbol(CHARGERLIB_EVENT_DISP_IMAGE_NOBATTERY, TRUE);
         switch(gChargerLibTargetCfgData.fg_cfg_data.FgBattIdCfg.BattMissingAction)
         {
            case BATT_MISSING_BOOT_TO_HLOS:
              *pErrorAction = CHARGERLIB_ERROR_ACTION_NONE;
              break;
            case BATT_MISSING_SHUTDOWN:
              *pErrorAction = CHARGERLIB_ERROR_ACTION_SHUTDOWN;
              break;
            default:
              break;
         }
         break;
    case CHARGERLIB_CHARGING_ERROR_LOW_VBATT:
         /* Emergency Low VBatt shutdown */
         //ChargerLibEvent_DispBattSymbol(CHARGERLIB_EVENT_DISP_IMAGE_LOWBATTERY, TRUE);
         *pErrorAction = CHARGERLIB_ERROR_ACTION_SHUTDOWN_USB_DC_PON_DISABLED;
      break;
    case CHARGERLIB_CHARGING_ERROR_TSENSE_TIMEOUT:
         ChargerLibEvent_DispBattSymbol(CHARGERLIB_EVENT_DISP_IMAGE_TSENS_THERMAL_SYMBOL, TRUE);
    case CHARGERLIB_CHARGING_ERROR_ADC_ERROR:
    case CHARGERLIB_CHARGING_ERROR_FG_ERROR:
    case CHARGERLIB_CHARGING_ERROR_VBATT_OUTSIDE_RANGE:
    case CHARGERLIB_CHARGING_ERROR_CHARGING_TIMEOUT:
    case CHARGERLIB_DEVICE_ERROR:
        *pErrorAction = CHARGERLIB_ERROR_ACTION_SHUTDOWN;
        break;
    case CHARGERLIB_CHARGING_ERROR_CHARGING_SOURCE_NOT_DETECTED:
      if (NULL == PmicPwronProtocol)
      {
        Status = gBS->LocateProtocol( &gQcomPmicPonProtocolGuid,
                                      NULL,
                                      (VOID **)&PmicPwronProtocol);

        if (Status != EFI_SUCCESS)
        {
          CHARGER_DEBUG(( EFI_D_ERROR, "ChargerLib:: %a Error locating PmicPonProtocol \r\n", __FUNCTION__));
          return Status;
        }
      }
      Status = ChargerLibTarget_GetPlatformPwrOnReason(&PM_PonReasonType);
      if (Status != EFI_SUCCESS)
      {
        CHARGER_DEBUG(( EFI_D_ERROR, "ChargerLib:: %a Error getting PON reason \r\n", __FUNCTION__));
        return Status;
      }

      if (PM_PonReasonType.KPDPWR || PM_PonReasonType.SMPL || (PM_PonReasonType.HARD_RESET && !ShipModeExit))
      {
        //if already reached threshold,it's not an error and goot to boot
        CHARGER_FILE_UART_DEBUG(( EFI_D_WARN, "ChargerLib:: %a Charger not detected , PM_PonReasonType : 0x%x, GOOD TO BOOT \r\n", __FUNCTION__, PM_PonReasonType));

        if(gChargerLibTargetCfgData.soc_based_boot == TRUE)
        {
          if(BatteryStatus.StateOfCharge < gChargerLibTargetCfgData.boot_to_hlos_threshold_in_soc)
          {
            //ChargerLibEvent_DispBattSymbol(CHARGERLIB_EVENT_DISP_IMAGE_NOCHARGER, TRUE);
            *pErrorAction = CHARGERLIB_ERROR_ACTION_SHUTDOWN;
          }
          else
          {//if already reached threshold,it's not an error and goot to boot
            *pErrorAction = CHARGERLIB_ERROR_ACTION_GOOD_TO_BOOT;
          }
        }
        else
        {
          if(BatteryStatus.BatteryVoltage < gChargerLibTargetCfgData.boot_to_hlos_threshold_in_mv)
          {
            //ChargerLibEvent_DispBattSymbol(CHARGERLIB_EVENT_DISP_IMAGE_NOCHARGER, TRUE);
            *pErrorAction = CHARGERLIB_ERROR_ACTION_SHUTDOWN;
          }
          else
          {//if already reached threshold,it's not an error and goot to boot
            *pErrorAction = CHARGERLIB_ERROR_ACTION_GOOD_TO_BOOT;
          }
        }
      }
      else
      {
        *pErrorAction = CHARGERLIB_ERROR_ACTION_SHUTDOWN;
      }
       break;

    case CHARGERLIB_CHARGING_ERROR_BATT_TEMP_OUTSIDE_CHARGING_RANGE:
        if(gChargerLibTargetCfgData.soc_based_boot == TRUE)
        {
          if(BatteryStatus.StateOfCharge < gChargerLibTargetCfgData.boot_to_hlos_threshold_in_soc)
          {
            *pErrorAction = CHARGERLIB_ERROR_ACTION_NO_CHARGE_WAIT;
          }
          else
          {//if already reached threshold,it's not an error and goot to boot
            *pErrorAction = CHARGERLIB_ERROR_ACTION_GOOD_TO_BOOT;
            CHARGER_DEBUG((EFI_D_WARN, "ChargerLib:: %a Temp outside charging range good soc boot to hlos \r\n", __FUNCTION__));
          }
        }
        else
        {
          if(BatteryStatus.BatteryVoltage < gChargerLibTargetCfgData.boot_to_hlos_threshold_in_mv)
          {
            *pErrorAction = CHARGERLIB_ERROR_ACTION_NO_CHARGE_WAIT;
          }
          else
          {  //if already reached threshold,it's not an error and goot to boot
            CHARGER_DEBUG((EFI_D_WARN, "ChargerLib:: %a Temp outside charging range good vbatt boot to hlos \r\n", __FUNCTION__));
            *pErrorAction = CHARGERLIB_ERROR_ACTION_GOOD_TO_BOOT;
          }
        }
       break;
    case CHARGERLIB_CHARGING_ERROR_TSENSE_HIGH:
         *pErrorAction = CHARGERLIB_ERROR_ACTION_TSENSE_HIGH_WAIT;
         break;
    case CHARGERLIB_CHARGING_ERROR_DEBUG_BOARD:
         Status = ChargerLibTarget_GetDebugBoardAction(pErrorAction);
         break;
    case CHARGERLIB_CHARGING_ERROR_UNKNOWN_BATTERY:
         Status = ChargerLibTarget_GetUnknownBatteryAction(pErrorAction);
         break;
     default:
       *pErrorAction = CHARGERLIB_ERROR_ACTION_STOP_CHARGING;
       CHARGER_DEBUG((EFI_D_WARN, "ChargerLib:: %a default action stop charging %d \r\n", __FUNCTION__, *pErrorAction));
      break;
  }

  return Status;
}


EFI_STATUS ChargerLibTarget_HandleRecovery()
{
  EFI_STATUS       Status    = EFI_SUCCESS;
  CHARGER_DEBUG((EFI_D_ERROR, "ChargerLib:: %a \r\n", __FUNCTION__));

  /* TBD to handle recovery - use USBPWRCtrl to read and print status */
  return Status;
}


EFI_STATUS ChargerLibTarget_GetActivePortPmicIndex(UINT8* pmicIndex)
{
  EFI_STATUS Status = EFI_SUCCESS;
  if (NULL == pmicIndex)
  {
    return EFI_INVALID_PARAMETER;
  }
  switch (ChargerInfo.ChargerHW)
  {
    case EfiBatteryChargerQcomPmicSchg:
    case EfiBatteryChargerSMB2351:
      {
        /* Locate Schg Protocol */
        if(NULL == PmicSchgProtocol)
        {
          Status = gBS->LocateProtocol (&gQcomPmicSchgProtocolGuid,NULL, (VOID **) &PmicSchgProtocol);
          if((EFI_SUCCESS != Status) || (NULL == PmicSchgProtocol))
          {
            CHARGER_DEBUG((EFI_D_ERROR, "ChargerLib:: %a  Locate PmicSchgProtocol failed %r \r\n", __FUNCTION__, Status));
            return Status;
          }
        }

        Status = PmicSchgProtocol->GetActivePort(pmicIndex);
        if (EFI_SUCCESS != Status)
        {
          //if not success get the master charger pmic index
          *pmicIndex = ChargerInfo.ChargerPmicInfo.PmicIndex;
        }
      }
      break;

    default:
      Status = EFI_UNSUPPORTED;
      break;
  }
  return Status;
}

EFI_STATUS ChargerLibTarget_GetChargerInputStatus(CHARGERLIB_CHARGER_INPUT_STATUS *pChargerInputStatus)
{
  EFI_STATUS Status = EFI_SUCCESS;
  EFI_PM_SCHG_ICL_STATUS pmschg_icl_status = {0};
  UINT8 activePortPmicIndex = 1;
  BOOLEAN IsUSBChargerAttached = FALSE;

  //NULL Check
  if(pChargerInputStatus == NULL)
    return EFI_INVALID_PARAMETER;

  //Reset Parameter
  pChargerInputStatus->bAiclDone = FALSE;
  pChargerInputStatus->ICLfinalMa = 0;
  pChargerInputStatus->ICLMaxMa = 0;


  //Check If USB Charger is Attached
  ChargerLibTarget_GetUsbChargerStatus(&IsUSBChargerAttached);

  switch (ChargerInfo.ChargerHW)
  {
   case EfiBatteryChargerQcomPmicSchg:
   case EfiBatteryChargerSMB2351:
    if (!PmicSchgProtocol)
    {
      Status = gBS->LocateProtocol(&gQcomPmicSchgProtocolGuid,
                                   NULL,
                                   (VOID **)&PmicSchgProtocol);

      if((Status != EFI_SUCCESS) || (NULL == PmicSchgProtocol ))
      {
        CHARGER_DEBUG((EFI_D_WARN,"ChargerLib:: %a pmicSchgProtocol Failed = %r \r\n", __FUNCTION__, Status));
        return EFI_DEVICE_ERROR;
      }
    }
    //Get Active Port
    Status = ChargerLibTarget_GetActivePortPmicIndex(&activePortPmicIndex);
    if (Status != EFI_SUCCESS)
    {
      return EFI_DEVICE_ERROR;
    }
    CHARGER_DEBUG((EFI_D_WARN,"ChargerLib:: %a Reading Input Status From PmicIndex =%d \r\n", __FUNCTION__, activePortPmicIndex));
    Status = PmicSchgProtocol->GetIclStatus(activePortPmicIndex, &pmschg_icl_status);
    break;
  default:
    Status = EFI_UNSUPPORTED;
    break;
  }

  if(Status == EFI_SUCCESS)
  {
    pChargerInputStatus->bAiclDone = (pmschg_icl_status.aicl_done) > 0? TRUE:FALSE;
    pChargerInputStatus->ICLfinalMa = pmschg_icl_status.final_icl_status;
    pChargerInputStatus->ICLMaxMa = pmschg_icl_status.max_icl_status;
  }
  return Status;
}

EFI_STATUS ChargerLibTarget_GetUsbChargerStatus(BOOLEAN *bChargerAttached)
{
  EFI_USB_PWR_CTRL_TYPEC_PORT_STATUS  typec_status;
  EFI_USB_PWR_CTRL_HSUSB_CHGPORT_TYPE hsusb_type;
  EFI_USB_PWR_CTRL_PD_STATUS          pd_status;
  BOOLEAN                             VbusDetect = FALSE;
  EFI_USB_PWR_CTRL_USB_PORTHW_INFO    usbport_info;
  EFI_STATUS                          Status = EFI_SUCCESS;
  uint8                               index;
  CHARGERLIB_CHG_USB_CHARGER_DATA     UsbChargerData;

  //NULL Check
  if(bChargerAttached == NULL)
    return EFI_INVALID_PARAMETER;

  //Reset Parameter
  *bChargerAttached = FALSE;

  if (!UsbPwrCtrlProtocol)
  {
    Status = gBS->LocateProtocol(&gQcomUsbPwrCtrlProtocolGuid,
                                 NULL,
                                 (VOID **)&UsbPwrCtrlProtocol);
    if((Status != EFI_SUCCESS) || (NULL == UsbPwrCtrlProtocol ))
    {
      CHARGER_DEBUG((EFI_D_WARN,"ChargerLib:: %a UsbPwrCtrlProtocol Failed = %r \r\n", __FUNCTION__, Status));
      return EFI_DEVICE_ERROR;
    }
  }

  Status = UsbPwrCtrlProtocol->GetHwInfo(&usbport_info);
  if (Status != EFI_SUCCESS)
  {
    CHARGER_DEBUG((EFI_D_WARN,"ChargerLib:: %a Get Hw Info Failed = %r \r\n", __FUNCTION__, Status));
    return Status;
  }

  CHARGER_DEBUG((EFI_D_WARN,"ChargerLib:: %a  GetHwInfo - NumOfPorts = %d \r\n", __FUNCTION__, usbport_info.NumOfPorts));

  for (index = 0; index < usbport_info.NumOfPorts; index++)
  {
    Status = UsbPwrCtrlProtocol->GetVbusDetectStatus(usbport_info.PortIndexes[index], &VbusDetect);
    if (EFI_ERROR(Status) || !VbusDetect)
    {
      CHARGER_DEBUG((EFI_D_WARN,"ChargerLib:: %a  VbusDetect = %d, Error (ignore) = %r \r\n", __FUNCTION__, VbusDetect, Status));
      // Update Status = EFI_SUCCESS for GetVbusDetectStatus() failure for Type-A ports
      Status = EFI_SUCCESS;
      continue;
    }

    //Reset Data
    SetMem(&UsbChargerData, sizeof(CHARGERLIB_CHG_USB_CHARGER_DATA), 0x0);
    UsbChargerData.BC1p2 = CHARGERLIB_CHG_PORT_INVALID;

    Status = UsbPwrCtrlProtocol->GetTypeCPortPDStatus(usbport_info.PortIndexes[index], &pd_status);
    if(Status == EFI_SUCCESS)
    {
      UsbChargerData.PD.bPDActive = pd_status.bPDConsumer;
    }

    Status = UsbPwrCtrlProtocol->GetTypeCPortStatus(usbport_info.PortIndexes[index], &typec_status);
    if(Status == EFI_SUCCESS)
    {
      switch (typec_status.DetectedSinkType)
      {
      case EFI_USB_PWR_CTRL_TYPEC_PORT_SNK_3P0:
        UsbChargerData.TypeC = CHARGERLIB_CHG_USBC_SINK_3P0A;
        break;
      case EFI_USB_PWR_CTRL_TYPEC_PORT_SNK_1P5:
        UsbChargerData.TypeC = CHARGERLIB_CHG_USBC_SINK_1P5A;
        break;
      case EFI_USB_PWR_CTRL_TYPEC_PORT_SNK_DEFAULT:
        UsbChargerData.TypeC = CHARGERLIB_CHG_USBC_SINK_DFAULT;
        break;
      default:
        UsbChargerData.TypeC = CHARGERLIB_CHG_USBC_SINK_NONE;
        break;
      }
    }

    Status = UsbPwrCtrlProtocol->GetHsUsbChgPortType(usbport_info.PortIndexes[index], &hsusb_type);
    if(Status == EFI_SUCCESS)
    {
      switch (hsusb_type)
      {
      case EFI_USB_PWR_CTRL_HSUSB_CHGPORT_SDP:
        UsbChargerData.BC1p2 = CHARGERLIB_CHG_PORT_SDP_CHARGER;
        break;
      case EFI_USB_PWR_CTRL_HSUSB_CHGPORT_OCP:
        UsbChargerData.BC1p2 = CHARGERLIB_CHG_PORT_OCP_CHARGER;
        break;
      case EFI_USB_PWR_CTRL_HSUSB_CHGPORT_CDP:
        UsbChargerData.BC1p2 = CHARGERLIB_CHG_PORT_OCP_CHARGER;
        break;
      case EFI_USB_PWR_CTRL_HSUSB_CHGPORT_DCP:
        UsbChargerData.BC1p2 = CHARGERLIB_CHG_PORT_DCP_CHARGER;
        break;
      case EFI_USB_PWR_CTRL_HSUSB_CHGPORT_FLOAT_CHARGER:
        UsbChargerData.BC1p2 = CHARGERLIB_CHG_PORT_FLOAT_CHARGER;
        break;
      case EFI_USB_PWR_CTRL_HSUSB_CHGPORT_QC_2P0:
        UsbChargerData.BC1p2 = CHARGERLIB_CHG_PORT_QC_2P0;
        break;
      case EFI_USB_PWR_CTRL_HSUSB_CHGPORT_QC_3P0:
        UsbChargerData.BC1p2 = CHARGERLIB_CHG_PORT_QC_3P0;
        break;
      case EFI_USB_PWR_CTRL_HSUSB_CHGPORT_UNKNOWN:
      default:
        UsbChargerData.BC1p2 = CHARGERLIB_CHG_PORT_INVALID;
      }
    }

    if ( UsbChargerData.PD.bPDActive ||
        (UsbChargerData.TypeC != CHARGERLIB_CHG_USBC_SINK_NONE) ||
        (UsbChargerData.BC1p2 != CHARGERLIB_CHG_PORT_INVALID))
    {
      if(UsbChargerData.PD.bPDActive)
      {
        CHARGER_DEBUG((EFI_D_WARN,"ChargerLib:: %a Port[%d] Charger bPDActive= %d \r\n",
                                 __FUNCTION__, usbport_info.PortIndexes[index], UsbChargerData.PD.bPDActive));
      }
      else
      {
        CHARGER_DEBUG((EFI_D_WARN, "ChargerLib:: %a Port[%d] Charger Usbc Sink Port type = %d \r\n",
                                 __FUNCTION__, usbport_info.PortIndexes[index], UsbChargerData.TypeC));
        CHARGER_DEBUG((EFI_D_WARN, "ChargerLib:: %a Port[%d] Charger Port type = %d \r\n",
                               __FUNCTION__, usbport_info.PortIndexes[index], UsbChargerData.BC1p2));
      }

      *bChargerAttached = TRUE;
    }
  }
  return Status;
}

EFI_STATUS ChargerLibTarget_EnableFgShipMode()
{
    EFI_STATUS Status = EFI_SUCCESS;
    Status = SmartChargerLibFG_SetShipMode();

    return Status;
}
EFI_STATUS ChargerLibTarget_EnablePmicShipMode(BOOLEAN enable)
{
  EFI_STATUS Status = EFI_SUCCESS;

  if(!PmicSchgProtocol)
  {
    Status = gBS->LocateProtocol(&gQcomPmicSchgProtocolGuid, NULL, (VOID **)&PmicSchgProtocol);
  }

  if((NULL == PmicSchgProtocol ) || (Status != EFI_SUCCESS))
    return EFI_DEVICE_ERROR;

  Status = PmicSchgProtocol->SetShipMode(enable);

  return Status;
}

EFI_STATUS ChargerLibTarget_GetVbusDetect(BOOLEAN *bVbusDetected)
{
  BOOLEAN                             VbusDetect = FALSE;
  EFI_USB_PWR_CTRL_USB_PORTHW_INFO    usbport_info;
  EFI_STATUS                          Status = EFI_SUCCESS;
  uint8                               index;

  //NULL Check
  if(bVbusDetected == NULL)
    return EFI_INVALID_PARAMETER;

  //Reset Parameter
  *bVbusDetected = FALSE;

  if (!UsbPwrCtrlProtocol)
  {
    Status = gBS->LocateProtocol(&gQcomUsbPwrCtrlProtocolGuid,
                                 NULL,
                                 (VOID **)&UsbPwrCtrlProtocol);
    if((Status != EFI_SUCCESS) || (NULL == UsbPwrCtrlProtocol ))
    {
      CHARGER_DEBUG((EFI_D_WARN,"ChargerLib:: %a UsbPwrCtrlProtocol Failed = %r \r\n", __FUNCTION__, Status));
      return EFI_DEVICE_ERROR;
    }
  }

  Status = UsbPwrCtrlProtocol->GetHwInfo(&usbport_info);
  if (Status != EFI_SUCCESS)
  {
    CHARGER_DEBUG((EFI_D_WARN,"ChargerLib:: %a Get Hw Info Failed = %r \r\n", __FUNCTION__, Status));
    return Status;
  }

  for (index = 0; index < usbport_info.NumOfPorts; index++)
  {
    Status = UsbPwrCtrlProtocol->GetVbusDetectStatus(usbport_info.PortIndexes[index], &VbusDetect);
    if (!EFI_ERROR(Status) && VbusDetect)
    {
      *bVbusDetected = TRUE;
      break;
    }
  }
  return Status;
}

EFI_STATUS ChargerLibTarget_GetPlatformPwrOnReason(EFI_PM_PON_REASON_TYPE *pPonReason)
{
    EFI_STATUS Status = EFI_SUCCESS;
    STATIC UINT64 PonReasonsHist = 0;
    UINT8 *PonReasonHist = NULL;

    if (NULL == pPonReason)
    {
        return EFI_INVALID_PARAMETER;
    }

    PonReasonHist = (UINT8 *)&PonReasonsHist;

    if(PonReasonsHist == 0)  /* To read only once and keep static copy */
    {
        if (PmicSchgProtocol == NULL)
        {
          Status = gBS->LocateProtocol(&gQcomPmicSchgProtocolGuid, NULL, (VOID **)&PmicSchgProtocol);
          if (Status != EFI_SUCCESS)
          {
            CHARGER_DEBUG((EFI_D_ERROR, "ChargerLib:: %a  PmicSchgProtocol locate failed, Error[%r] \r\n", __FUNCTION__, Status));
            return Status;
          }
        }

        Status = PmicSchgProtocol->GetValidPonReasons(&PonReasonsHist);
        if (Status != EFI_SUCCESS)
        {
          CHARGER_DEBUG((EFI_D_ERROR, "ChargerLib:: %a  GetValidPonReasons failed, Error[%r] \r\n", __FUNCTION__, Status));
          return Status;
        }

        CHARGER_DEBUG((EFI_D_WARN, "ChargerLib:: %a  PonReasonsHist = 0x%16X \r\n", __FUNCTION__, PonReasonsHist));
    }

    *(UINT8 *)pPonReason = PonReasonHist[EFI_PM_VALID_PON_REASON_PON_REASON1];

    return Status;
}

EFI_STATUS ChargerLibTarget_SetOffModeSrc(UINT8 offModeSrcCfg)
{
    EFI_STATUS Status = EFI_SUCCESS;
    BOOLEAN reset = TRUE;
    if(!PmicSchgProtocol)
    {
        Status = gBS->LocateProtocol(&gQcomPmicSchgProtocolGuid, NULL, (VOID **)&PmicSchgProtocol);
    }
    
    if((NULL == PmicSchgProtocol ) || (Status != EFI_SUCCESS))
        return EFI_DEVICE_ERROR;
    
    Status = PmicSchgProtocol->GetDvddReset(&reset);
    if(EFI_SUCCESS != Status)
    {
        CHARGER_DEBUG((EFI_D_ERROR, "ChargerLib:: %a  GetDvddReset failed, Error[%r] \r\n", __FUNCTION__, Status));
        return Status;
    }
    if(reset)
    {
        Status = PmicSchgProtocol->SetOffModeSrcVbatThresh(gChargerLibTargetCfgData.off_mode_src_cfg.vbatThLightLoad, 
                                                            gChargerLibTargetCfgData.off_mode_src_cfg.vbatThLightLoad - gChargerLibTargetCfgData.off_mode_src_cfg.vbatThDeltaHeavyLoad);
        if(EFI_SUCCESS != Status)
        {
            CHARGER_DEBUG((EFI_D_ERROR, "ChargerLib:: %a  SettOffModeSrcVbatThresh failed, Error[%r] \r\n", __FUNCTION__, Status));
            return Status;
        }
    }
    Status = PmicSchgProtocol->SetOffModeSrc(offModeSrcCfg);
    if (EFI_SUCCESS != Status)
    {
        CHARGER_DEBUG((EFI_D_ERROR, "ChargerLib:: %a  SetOffModeSrc failed, Error[%r] \r\n", __FUNCTION__, Status));
        return Status;
    }
    CHARGER_DEBUG((EFI_D_WARN, "ChargerLib:: %a  SetOffModeSrc = 0x%X \r\n", __FUNCTION__, offModeSrcCfg));
    return Status;
}

EFI_STATUS ChargerLibTarget_SetOS(CHARGERLIB_OS_TYPE OS)
{
    EFI_STATUS Status = EFI_SUCCESS;
    
    if(!PmicSchgProtocol)
    {
        Status = gBS->LocateProtocol(&gQcomPmicSchgProtocolGuid, NULL, (VOID **)&PmicSchgProtocol);
    }
    
    if((NULL == PmicSchgProtocol ) || (Status != EFI_SUCCESS))
        return EFI_DEVICE_ERROR;

    if (OS >= OS_INVALID)
        return EFI_INVALID_PARAMETER;
    
    Status = PmicSchgProtocol->SetOS((EFI_PM_SCHG_OS_TYPE)OS);
    if (EFI_SUCCESS != Status)
    {
        CHARGER_DEBUG((EFI_D_ERROR, "ChargerLib:: %a  SetOS failed, Error[%r] \r\n", __FUNCTION__, Status));
        return Status;
    }
    CHARGER_DEBUG((EFI_D_WARN, "ChargerLib:: %a  SetOS = %d \r\n", __FUNCTION__, OS));
    return Status;

}

EFI_STATUS ChargerLibTarget_SetReqMaxPwrCfg(CHARGERLIB_PLATFORM_CHGR_MAX_PWR_REQ_CFG_STS_TYPE config)
{
    EFI_STATUS Status = EFI_SUCCESS;

    if(!PmicSchgProtocol)
    {
        Status = gBS->LocateProtocol(&gQcomPmicSchgProtocolGuid, NULL, (VOID **)&PmicSchgProtocol);
    }

    if((NULL == PmicSchgProtocol ) || (Status != EFI_SUCCESS))
        return EFI_DEVICE_ERROR;

    Status = PmicSchgProtocol->SetReqMaxPwrCfg(config.AsUint8);
    if (EFI_SUCCESS != Status)
    {
        CHARGER_DEBUG((EFI_D_ERROR, "ChargerLib:: %a  SetReqMaxPwrCfg failed, Error[%r] \r\n", __FUNCTION__, Status));
        return Status;
    }
    CHARGER_DEBUG((EFI_D_WARN, "ChargerLib:: %a  SetReqMaxPwrCfg 0x%x done\r\n", __FUNCTION__, config));
    return Status;

}

EFI_STATUS ChargerLibTarget_GetReqMaxPwrConfig(CHARGERLIB_PLATFORM_CHGR_MAX_PWR_REQ_CFG_STS_TYPE *pConfig)
{
    EFI_STATUS Status = EFI_SUCCESS;

    if (NULL == pConfig)
    {
        return EFI_INVALID_PARAMETER;
    }

    if(!PmicSchgProtocol)
    {
        Status = gBS->LocateProtocol(&gQcomPmicSchgProtocolGuid, NULL, (VOID **)&PmicSchgProtocol);
    }

    if((NULL == PmicSchgProtocol ) || (Status != EFI_SUCCESS))
        return EFI_DEVICE_ERROR;

    Status = PmicSchgProtocol->GetReqMaxPwrCfg((UINT8*)pConfig);
    if (EFI_SUCCESS != Status)
    {
        CHARGER_DEBUG((EFI_D_ERROR, "ChargerLib:: %a  GetReqMaxPwrCfg failed, Error[%r] \r\n", __FUNCTION__, Status));
        return Status;
    }
    CHARGER_DEBUG((EFI_D_WARN, "ChargerLib:: %a  GetReqMaxPwrCfg 0x%x done\r\n", __FUNCTION__, *pConfig));
    return Status;

}

EFI_STATUS ChargerLibTarget_CheckBatteryPresent(BOOLEAN FGChipPresent)
{
    EFI_STATUS Status = EFI_SUCCESS;
    BOOLEAN DamagedBattery = FALSE;
    BOOLEAN bPFActive = 0;
    BOOLEAN BatteryPresence = TRUE;
    UINT32 PFStatus = 0;
    CHARGERLIB_PLATFORM_CHGR_MAX_PWR_REQ_CFG_STS_TYPE MaxPwrReqSts;

    if (FGChipPresent == TRUE)
    {
        Status = SmartChargerLibFG_GetBatteryPFActive(&bPFActive);
        if (Status == EFI_SUCCESS && bPFActive == TRUE)
        {
            Status = SmartChargerLibFG_GetBatteryPFStatus(&PFStatus);
            if (Status == EFI_SUCCESS && PFStatus)
            {
                DamagedBattery = TRUE;
                CHARGER_DEBUG((EFI_D_WARN, "ChargerLib:: %a  Damaged Battery PF Status 0x%x\r\n", __FUNCTION__, PFStatus));
            }
        }
    }
    //set battery missing flag, if FGCHip is missing or damaged battery..
    if (FGChipPresent == FALSE || DamagedBattery == TRUE)
    {
        BatteryPresence = FALSE;
    }
    if (BatteryPresence == TRUE)
    {
        //Clear SDAM
        MaxPwrReqSts.AsUint8 = 0;
        Status = ChargerLibTarget_SetReqMaxPwrCfg(MaxPwrReqSts);
        if (Status != EFI_SUCCESS)
        {
            CHARGER_DEBUG((EFI_D_WARN, "ChargerLib:: %a  ChargerLibTarget_SetReqMaxPwrCfg failed 0x%x\r\n", __FUNCTION__, Status));
        }
    }
    else
    {
        //read SDAM
        MaxPwrReqSts.AsUint8 = 0;
        Status = ChargerLibTarget_GetReqMaxPwrConfig(&MaxPwrReqSts);
        if (Status == EFI_SUCCESS &&
            MaxPwrReqSts.batt_missing==1 &&
            MaxPwrReqSts.status == CHGRPD_MAX_PWR_REQ_DONE)
        {
            //Already we negotiated power..
            CHARGER_DEBUG((EFI_D_ERROR, "ChargerLib:: %a already Power requested.\r\n", __FUNCTION__));
        }
        else
        {
            //Clear
            MaxPwrReqSts.AsUint8 = 0;
            //set battery missing flag
            MaxPwrReqSts.batt_missing = 1;
            Status = ChargerLibTarget_SetReqMaxPwrCfg(MaxPwrReqSts);
            if (Status != EFI_SUCCESS)
            {
                CHARGER_DEBUG((EFI_D_WARN, "ChargerLib:: %a  ChargerLibTarget_SetReqMaxPwrCfg failed 0x%x\r\n", __FUNCTION__, Status));
            }
        }
    }

    return Status;
}
