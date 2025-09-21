/** @file SmartChargerLibCommon.c

  Provide access to SmartCharger Libraries

  Copyright (c) 2017-2018, Qualcomm Technologies Inc. All rights reserved.

**/

/*=============================================================================
                              EDIT HISTORY


 when         who     what, where, why
 --------     ---     ---------------------------------------------------------
03/29/18      pbitra  Shutting down if charger is disconnected when the PON 
                      reason is not KPDPWR or HARD_RESET
03/26/18      mr      Enable ROHM Sink for 'no charger connected UEFI Shutdonw case',
                      so USB PON can be detected upon insertion
03/15/18      pbitra  Re-enabling vbatt checking for emergency shutdown
02/06/18      dc      Skip Sink Disable in case DISABLEBATTERY flag is set
02/02/18      dc      Report Charger Presense based on Vbus and Type-C detection result
02/01/18      mr      Implemented WA for Reverse voltage on Vbus issue
12/11/17      dc       Remove unnecessary source port type assignment
11/16/17      pbitra   Added Debug cable support
11/09/17      pbitra   File Cleanup, changes to SmartChargerLib_GetErrors
08/01/17      pbitra      Initial revision
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
#include <api/pmic/pm/pm_uefi.h>
#include <api/pmic/charger/ChargerLibCommon.h>
#include <Library/QcomBaseLib.h>
#include <PrintShLib.h>

#include <Library/BaseMemoryLib.h>
#include "string.h"

#include "ChargerExtHwLib.h"
#include "SmartChargerLibFG.h"
/**
  Protocol Dependencies
*/
#include <Protocol/EFIPlatformInfo.h>
#include <Protocol/EFIPmicPwrOn.h>
#include <Protocol/EFIVariableServices.h>
#include <Protocol/EFITsens.h>
#include <Protocol/EFIPmicRTC.h>
#include <Protocol/EFIUsbfnChgSrv.h>
#include <Protocol/EFIUsbInitProtocol.h>
#include <Protocol/EFIUsbPwrCtrl.h>
#include <Protocol/EFIQcomCharger.h>


/*===========================================================================*/
/*                  TYPEDEF AND MACRO DEFINITIONS                            */
/*===========================================================================*/
#define VBATT_TOLERANCE                     5 //Battery upper voltage tolerance limit.

#define DEBUG_BOARD_LOWVBAT_SHUTDOWN        0
#define DEBUG_BOARD_LOWVBAT_NOCHARGE        1
#define DEBUG_BOARD_BOOT_TO_HLOS            2

#define MAX_3000_MA                         3000
#define MAX_1500_MA                         1500

#define THERMAL_TIMEOUT_NOTSET              0
#define MIN_TO_SEC                          60

#define MAX_THERMAL_WAIT_PERIOD_IN_MINS     60

#define USB_3_IUSB_MAX                      900 //IUSB_MAX for USB 3.0 port
#define USB_2_IUSB_MAX                      500 //IUSB_MAX for USB 2.0 port
#define USB_DEFAULT_IUSB_MAX                500
#define USB_ENUM_WAIT_DURATION              500 /*500 milliseconds*/

#define MAX_EMERGENCY_SHUTDOWN_COUNT        3

#define CHARGERLIB_SHIPMODE_VARIABLE_NAME   L"ShipMode"
/*===========================================================================*/
/*                  FUNCTIONS PROTOTYPES                                     */
/*===========================================================================*/
VOID EFIAPI SmartChargerLibEnableCallback(IN EFI_EVENT Event, IN VOID *Context);
VOID EFIAPI SmartChargerLibDisableCallback(IN EFI_EVENT Event, IN VOID *Context);
EFI_STATUS SmartChargerLib_GetTSensTemp( INT32 *TSensTemp);
EFI_STATUS SmartChargerLib_HandleThermalErrorType(ChargerLibThermalStatus ThermalStatus, CHARGERLIB_CHARGING_ERROR_TYPES *pChargingError);
EFI_STATUS SmartChargerLib_GetSEllapsedFromBoot(UINT32 *PmicTimeinS);
EFI_STATUS SmartChargerLib_GetChargerPresence(BOOLEAN *pChargerPresent);
EFI_STATUS SmartChargerLib_GetBatteryPresence(BOOLEAN *bBatteryPresence);

/*===========================================================================*/
/*                  LOCAL VARIABLE DECLARATIONS                              */
/*===========================================================================*/
STATIC EFI_USBFN_CHG_SRV_PROTOCOL           *UsbChgSrvProtocol = NULL;
STATIC EFI_QCOM_PMIC_PWRON_PROTOCOL         *PmicPwronProtocol = NULL;
STATIC EFI_USB_INIT_PROTOCOL                *UsbInitProtocol = NULL;
STATIC EFI_USB_PWR_CTRL_PROTOCOL            *UsbPwrCtrlProtocol = NULL;

chargerlib_cfgdata_type                 gSmartChargerLibCfgData = {0};

STATIC ULogHandle                     gULogHandle = NULL;

extern EFI_GUID gQcomPmicPwrOnProtocolGuid;
extern EFI_GUID gQcomUsbPwrCtrlProtocolGuid;

STATIC EFI_PLATFORMINFO_PLATFORM_TYPE  PlatformType;
STATIC BOOLEAN              gChargerHwDetected = 0;
STATIC BOOLEAN                         gChargingEnabled = FALSE;
STATIC UINT32                          ThermalTimeOutTimeStamp = 0;
STATIC BOOLEAN                         gStartControllerCalled = FALSE;


/*===========================================================================*/
/*                 FUNCTION DEFINITIONS                                      */
/*===========================================================================*/
/**
  Initialize ChargerDXE Library functions

  @param none

  @return
  EFI_SUCCESS:           Function returned successfully.
  EFI_INVALID_PARAMETER: A Parameter was incorrect.
  EFI_DEVICE_ERROR:      The physical device reported an error.
  EFI_NOT_READY:         The physical device is busy or not ready to
                         process this request.
*/
EFI_STATUS SmartChargerLibCommon_Init( chargerlib_cfgdata_type *pSmartChargerLibConfig )
{
  EFI_STATUS                  Status         = EFI_SUCCESS;
  EFI_USB_PWR_CTRL_TYPEC_PORT_STATUS TypeCPortStatus;

  CopyMemS(&gSmartChargerLibCfgData, sizeof(chargerlib_cfgdata_type), pSmartChargerLibConfig, sizeof(chargerlib_cfgdata_type));

  Status |= GetPlatformType(&PlatformType);
  if(EFI_SUCCESS != Status)
  {
    SMARTCHARGER_DEBUG((EFI_D_WARN, "SmartChargerLib:: %a Error getting platform type  \r\n", __FUNCTION__));
    return EFI_DEVICE_ERROR;
  }

  /* Enable USB and DCIN PON triggers to be certain and avoid bad use case*/
  if (!PmicPwronProtocol)
  {
    Status |= gBS->LocateProtocol(&gQcomPmicPwrOnProtocolGuid, NULL, (VOID **)&PmicPwronProtocol);
  }

  if((Status == EFI_SUCCESS) && (NULL != PmicPwronProtocol))
  {
    Status |= PmicPwronProtocol->EnableEdgePonTrigger(PM_DEVICE_0, EFI_PM_PON_OPTION_PON1_REDGE_PON, FALSE);
  }
  else
  {
    SMARTCHARGER_DEBUG((EFI_D_WARN, "ChargerLib:: %a Error locating Pwron Protocol = %r \r\n", __FUNCTION__, Status));
  }
  /* Initialize Charger and Gauge HW */
  Status = ChargerExtHwLib_Init();
  if ( EFI_SUCCESS == Status)
  {
    gChargerHwDetected = 1;
  }
  else
  {
    SMARTCHARGER_DEBUG((EFI_D_WARN, "SmartChargerLib:: %a Error initializing charger HW  \r\n", __FUNCTION__));
  }

  if (!UsbPwrCtrlProtocol)
  {
    Status = gBS->LocateProtocol(&gQcomUsbPwrCtrlProtocolGuid, NULL, (VOID **)&UsbPwrCtrlProtocol);
    if(EFI_SUCCESS != Status || !UsbPwrCtrlProtocol)
    {
      SMARTCHARGER_DEBUG(( EFI_D_ERROR, "SmartChargerLib:: %a Can not locate USB PwrCtrl Protocol = %r \r\n", __FUNCTION__, Status));
      return EFI_DEVICE_ERROR;
    }

  }

  Status = UsbPwrCtrlProtocol->GetTypeCPortStatus (EFI_USB_PWR_CTRL_USBPORT_CHARGER, &TypeCPortStatus);
  if(EFI_ERROR(Status)) {return Status;}

  //UEFI Variable DISABLEBATTERY is NOT SET
  if(!gSmartChargerLibCfgData.schg_cfg_data.DISABLEBATTERY)
  {
    Status = SmartChargerLibFG_Init();
    if(EFI_ERROR(Status)) {return Status;}

    if(gSmartChargerLibCfgData.schg_cfg_data.SWRoleSelectEnable == TRUE)
    {
        BOOLEAN VbusPresent = FALSE;
        Status = UsbPwrCtrlProtocol->GetVbusDetectStatus(0, &VbusPresent);

        //SMARTCHARGER_DEBUG((EFI_D_ERROR, "%a VbusPresent: %d, PowerRole: %d\r\n", __FUNCTION__, VbusPresent, TypeCPortStatus.PowerRole));

        if(EFI_ERROR(Status)) {return Status;}

        // VBUS_DET == FALSE && CC_STATUS != UFP
        if( (FALSE == VbusPresent) &&
            (EFI_USB_PWR_CTRL_TYPEC_POWERROLE_SINK != TypeCPortStatus.PowerRole))
        {
            // Call SinkEn API to Set ROHM VIN_CTRL_SET(0Ah), bit 6=0 (VBUS Input OFF)
            Status = ChargerExtHwLib_SetSinkEn(FALSE);
            // Call Role Select API to Set ANX7411 ROLE=DRP (Dual Role)
            Status |= UsbPwrCtrlProtocol->SetTypeCPortRole(EFI_USB_PWR_CTRL_USBPORT_CHARGER, EFI_USB_PWR_CTRL_TYPEC_POWERROLE_DUAL);
            if(EFI_ERROR(Status)) {return Status;}
        }
        else
        {
            // Call Role Select API to set  ANX7411 ROLE=UFP
            Status = UsbPwrCtrlProtocol->SetTypeCPortRole(EFI_USB_PWR_CTRL_USBPORT_CHARGER, EFI_USB_PWR_CTRL_TYPEC_POWERROLE_SINK);
            if(EFI_ERROR(Status)) {return Status;}
        }
    }
    else
  {
        // Call Role Select API to set  ANX7411 ROLE=DRP in case WA is not enabled
        Status = UsbPwrCtrlProtocol->SetTypeCPortRole(EFI_USB_PWR_CTRL_USBPORT_CHARGER, EFI_USB_PWR_CTRL_TYPEC_POWERROLE_DUAL);
        if(EFI_ERROR(Status)) {return Status;}
    }
  }
  else
  {
    //UEFI Variable DISABLEBATTERY is NOT SET
    Status = SmartChargerLib_SetMaxUsbCurrent(MAX_3000_MA);
    if (EFI_SUCCESS == Status)
    {
      SmartChargerLib_ChargerEnable(TRUE);
    }
  }

  Status = ChargerExtHwLib_ConfigExtDep(CHARGER_EXT_HW_EXT_HW_DEP_QUP_IO_EN, CHARGER_EXT_HW_EXT_HW_DEP_ENABLE_HIGH);

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
EFI_STATUS SmartChargerLib_GetBatteryStatus(chargerlib_batt_status_info *pBatteryStatus)
{
  EFI_STATUS Status;

  Status = SmartChargerLibFG_GetBatteryStatus((SmartChargerFG_BattStatusInfo*)pBatteryStatus);
  return Status;
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
EFI_STATUS SmartChargerLib_ChargerEnable(BOOLEAN Enable)
{
  EFI_STATUS Status = EFI_SUCCESS;
    SMARTCHARGER_UART_DEBUG((EFI_D_WARN, "SmartChargerLib:: %a ChargerEnable : %d \r\n",__FUNCTION__, Enable));

    Status = ChargerExtHwLib_EnableCharger(Enable);
    if ( EFI_SUCCESS == Status)
    {
        gChargingEnabled = Enable;
  }

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
EFI_STATUS SmartChargerLib_SetMaxUsbCurrent(UINT32 MaxCurrent)
{
  EFI_STATUS Status = EFI_SUCCESS;
  EFI_USB_PWR_CTRL_INOUT_POWER_CONFIG  PowerCfg;

  if (!UsbPwrCtrlProtocol)
  {
    Status = gBS->LocateProtocol(&gQcomUsbPwrCtrlProtocolGuid, NULL, (VOID **)&UsbPwrCtrlProtocol);
    if ( EFI_SUCCESS != Status)
        return Status;
  }
  PowerCfg.MaxCurrentMA = MaxCurrent;
  Status = UsbPwrCtrlProtocol->SetSinkPower(0, PowerCfg);

  return Status;
}

EFI_STATUS SmartChargerLib_GetChargingPath(CHARGERLIB_ATTACHED_CHGR_TYPE *pChargingPath)
{
  EFI_STATUS Status = EFI_SUCCESS;
  BOOLEAN IsInputValid = FALSE;

    if (!pChargingPath)
    {
        return EFI_INVALID_PARAMETER;
    }
    *pChargingPath = CHARGERLIB_ATTACHED_CHGR__NONE;

    if (!UsbPwrCtrlProtocol)
    {
        Status = gBS->LocateProtocol(&gQcomUsbPwrCtrlProtocolGuid, NULL, (VOID **)&UsbPwrCtrlProtocol);
    }

    if(Status != EFI_SUCCESS)
    {
    SMARTCHARGER_UART_DEBUG((EFI_D_WARN, "SmartChargerLib:: %a Locating UsbPwrCtrlProtocol failed  \r\n",__FUNCTION__));

    return Status;
    }

    Status = UsbPwrCtrlProtocol->GetVbusDetectStatus(0, &IsInputValid);
    if (( EFI_SUCCESS == Status ) && (IsInputValid))
    {
      *pChargingPath = CHARGERLIB_ATTACHED_CHGR__USB;
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
EFI_STATUS SmartChargerLib_GetBatteryPresence(BOOLEAN *bBatteryPresence)
{
  EFI_STATUS Status = EFI_SUCCESS;
  SmartChargerFG_BattStatusInfo BatteryStatus;

  Status = SmartChargerLibFG_GetBatteryStatus(&BatteryStatus);

  if(EFI_SUCCESS == Status)
  {
  *bBatteryPresence = TRUE;
  }
  else
  {
    *bBatteryPresence = FALSE;
  }

  return Status;
}

/**
SmartChargerLib_PrintDebugMsg()

@brief
Returns SmartChargerLib_PrintDebugMsg Flag Status
*/
inline BOOLEAN
SmartChargerLib_PrintDebugMsg(void)
{
  return ((BOOLEAN)gSmartChargerLibCfgData.dbg_cfg_Data.print_charger_app_dbg_msg);
}

/**
SmartChargerLib_PrintDebugMsgToFile()

@brief
Returns SmartChargerLib_PrintDebugMsgToFile Flag Status
*/
inline BOOLEAN
SmartChargerLib_PrintDebugMsgToFile(void)
{
  if (NULL != gULogHandle) return (TRUE);
  return FALSE;
}

/**
ULogPrint ()

@brief Helper API to print multiple arguments
Returns
*/
inline void EFIAPI
ULogPrint(
  IN  UINTN        LogLevel,
  IN  CONST CHAR8  *Format,
  ...
  )
{
  va_list vlist;
  UINT32  dataCount = 0;
  UINTN   i         = 0;
  BOOLEAN  boEsc = FALSE;
  STATIC EFI_STATUS lastLogResult = EFI_SUCCESS;
  EFI_STATUS CurrentLogResult     = EFI_SUCCESS;
  CHAR8 CurrentFormat[512];
  UINTN CurrentFormatIndex = 0;
  UINT32 size = sizeof(CurrentFormat);
  BOOLEAN appendFlag = FALSE;
  CHAR8 *FnName = NULL;

  if ((LogLevel & gSmartChargerLibCfgData.dbg_cfg_Data.file_log_level_mask) != 0)
  {
      SetMem(CurrentFormat, sizeof(CurrentFormat), 0x00);

      /* If volume corrupted do not attempt to log */
      if (EFI_VOLUME_CORRUPTED == lastLogResult)
          return;

      for (i = 0, dataCount = 0; (NULL != (Format + i)) && (Format[i] != '\0'); i++, CurrentFormatIndex++)
      {
          CurrentFormat[CurrentFormatIndex] = Format[i];
          if (Format[i] == '%' && !boEsc)
          {
              if (Format[i + 1] == 'a')
              {
                  appendFlag = TRUE;
                  va_start(vlist, Format);
                  FnName = (CHAR8*)va_arg(vlist, CHAR8*);
                  AsciiVSPrint((CurrentFormat + CurrentFormatIndex), size, FnName, vlist);
                  CurrentFormatIndex = CurrentFormatIndex + AsciiStrLen(FnName);
                  CurrentFormat[CurrentFormatIndex] = ' ';
                  i += 2;
              }
              else if (Format[i + 1] == 'r')
              {
                  CurrentFormat[++CurrentFormatIndex] = 'd';
                  i++;
                  dataCount++;
              }
              else
              {
                  dataCount++;
              }
          }
          else if (Format[i] == '\\' && !boEsc)
          {
              boEsc = TRUE;
          }
          else if (boEsc)
          {
              boEsc = FALSE;
          }
      }
      /*
      if(appendFlag)
      {
      va_end(vlist);
      }*/
      /* Append end of the string */
      CurrentFormat[CurrentFormatIndex] = '\0';

      //SMARTCHARGER_UART_DEBUG((EFI_D_WARN, "SmartChargerLib:: %a dataCount = %d \r\n",__FUNCTION__, dataCount));

      if (appendFlag == FALSE)
      {
          va_start(vlist, Format);
      }
      //SMARTCHARGER_UART_DEBUG((EFI_D_WARN, "SmartChargerLib:: %a formatstring = %a \r\n",__FUNCTION__, Format));
      //SMARTCHARGER_UART_DEBUG((EFI_D_WARN, "SmartChargerLib:: %a Current Fn Name = %a size = %d \r\n",__FUNCTION__, CurrentFormat, size));
      if (appendFlag)
      {
          CurrentLogResult = (EFI_STATUS)ULogFront_RealTimeVprintf(gULogHandle, dataCount, CurrentFormat, vlist);
      }
      else
      {
          CurrentLogResult = (EFI_STATUS)ULogFront_RealTimeVprintf(gULogHandle, dataCount, Format, vlist);
      }
      if (CurrentLogResult == EFI_VOLUME_CORRUPTED)
      {
          SMARTCHARGER_UART_DEBUG((EFI_D_WARN, "SmartChargerLib:: %a File Log Print Volume Error = %r \r\n", __FUNCTION__, CurrentLogResult));
          lastLogResult = CurrentLogResult;
      }
      else if (CurrentLogResult != EFI_SUCCESS)
      {
          SMARTCHARGER_UART_DEBUG((EFI_D_WARN, "SmartChargerLib:: %a File Log Print Error = %r \r\n", __FUNCTION__, CurrentLogResult));
      }
      else { /* SUCCESS use case */ }

      va_end(vlist);
  }
}


/**
SmartChargerLib_InitFileLog(BOOLEAN )

@brief  Init File Logging in ULog
Returns Status
*/
EFI_STATUS SmartChargerLib_InitFileLog(BOOLEAN PrintChgAppDbgMsgToFile)
{
  ULogResult result = 0;
  EFI_STATUS       Status     =  EFI_SUCCESS;
  UINT32           VarSize    =  1;
  BOOLEAN          FileLoggingIsEnabled = TRUE;
  ULOG_CONFIG_TYPE  configType = {0};
  //DEBUG(( EFI_D_WARN, "SmartChargerLib:: %a \r\n", __FUNCTION__));

  if (!RETAIL)
  {
    /* Enable default file logging if production mode is disabled */
    if( TRUE == PrintChgAppDbgMsgToFile )
    {
      Status = gRT->SetVariable (L"EnableFileLogging", &gQcomTokenSpaceGuid,
                                 EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_NON_VOLATILE,
                                 VarSize, &FileLoggingIsEnabled);
      if (EFI_SUCCESS != Status)
      {
        DEBUG(( EFI_D_WARN, "SmartChargerLib:: %a Global File Logging BDS Menu Variable read error = ?%d \r\n", __FUNCTION__, Status));
        return EFI_SUCCESS;
      }

      result = ULogFile_Open(&gULogHandle, LOGFILE_IN_EFS, DEFAULT_LOGFILE_SIZE);
      if (0 == result)
      {
        /* Set ULog configuration */
        ULogFile_GetConfig(&gULogHandle, &configType);
        configType.separator = ',';
        configType.PrintTimestamp = TRUE;
        configType.TrimNewline = TRUE;
        ULogFile_SetConfig(&gULogHandle, &configType);
        if (Status != EFI_SUCCESS){
          SMARTCHARGER_UART_DEBUG(( EFI_D_ERROR, "SmartChargerLib:: ULogFile_SetConfig FAILED \r\n"));
        }
        ULOG_RT_PRINTF_1(gULogHandle, "SmartChargerLib::InitFileLog SUCCESS gULogHandle = %lld", gULogHandle);
        DEBUG((EFI_D_WARN, "SmartChargerLib:: %a InitFileLog SUCCESS \r\n",  __FUNCTION__));

        /* Print Battery status information Log headers during charging loop */
        /* DEBUG((EFI_D_WARN, "RebootCount,TimeStamp,StateOfCharge,RatedCapacity,Voltage,ChargeCurrent,Temp \r\n"));
        DEBUG((EFI_D_WARN, "TimeStamp,StateOfCharge,RatedCapacity,Voltage,ChargeCurrent,Temp \r\n")); */
      }
      else
      {
        SMARTCHARGER_DEBUG((EFI_D_WARN, "SmartChargerLib:: %a InitFileLog FAILED \r\n",  __FUNCTION__));
      }
    }
  }

  return EFI_SUCCESS;
}


/**
SmartChargerLib_GetFileLogHandle(BOOLEAN )

@brief  Returns File long handle if alreayd initilized
Returns Status
*/
EFI_STATUS SmartChargerLib_GetLogInfo(chargerlib_loginfo *gFileHandle )
{
  ULogResult result = 0;
  ULOG_CONFIG_TYPE  configType = {0};

  if(!gFileHandle)
    return EFI_INVALID_PARAMETER;

  if(NULL != gULogHandle)
  {
    result = ULogFile_Open(&(gFileHandle->gChargerLogHandle), LOGFILE_IN_EFS, DEFAULT_LOGFILE_SIZE);
    if (0 == result)
    {
      /* Set ULog configuration */
      ULogFile_GetConfig(&(gFileHandle->gChargerLogHandle), &configType);
      configType.separator = ',';
      configType.PrintTimestamp = TRUE;
      configType.TrimNewline = TRUE;
      ULogFile_SetConfig(&gFileHandle->gChargerLogHandle, &configType);
      ULOG_RT_PRINTF_1(gFileHandle->gChargerLogHandle, "SmartChargerLib::InitFileLog SUCCESS gULogHandle = %lld", gFileHandle->gChargerLogHandle);
      DEBUG((EFI_D_WARN, "SmartChargerLib:: %a InitFileLog SUCCESS \r\n",  __FUNCTION__));
    }
    else
    {
      SMARTCHARGER_DEBUG((EFI_D_WARN, "SmartChargerLib:: %a InitFileLog FAILED \r\n",  __FUNCTION__));
    }
  }
  gFileHandle->print_charger_app_dbg_msg = gSmartChargerLibCfgData.dbg_cfg_Data.print_charger_app_dbg_msg;
  gFileHandle->print_charger_app_dbg_msg_to_file = gSmartChargerLibCfgData.dbg_cfg_Data.print_charger_app_dbg_msg_to_file;

   return EFI_SUCCESS;
}

/**
  Gets charger port type

  @param[out] pPortType  Charger port type

  @return
  EFI_SUCCESS:           Function returned successfully.
  EFI_DEVICE_ERROR:      The physical device reported an error.
  EFI_UNSUPPORTED:       No Library function linked
*/
EFI_STATUS SmartChargerLib_GetPortType(EFI_USB_PWR_CTRL_HSUSB_CHGPORT_TYPE *pPortType)
{
  EFI_STATUS                  Status = EFI_SUCCESS;

  if (!UsbPwrCtrlProtocol)
  {
    Status = gBS->LocateProtocol(&gQcomUsbPwrCtrlProtocolGuid, NULL, (VOID **)&UsbPwrCtrlProtocol);
  }
  Status |= UsbPwrCtrlProtocol->GetHsUsbChgPortType(0, pPortType);
  SMARTCHARGER_FILE_UART_DEBUG((EFI_D_WARN,"SmartChargerLib:: %a Charger Port type = %d \r\n", __FUNCTION__, *pPortType));

  return Status;
}


EFI_STATUS SmartChargerLib_HandleNoChargeAndWait(void)
{
  EFI_STATUS Status = EFI_SUCCESS;

  if (TRUE == gSmartChargerLibCfgData.no_charge_and_wait)
  {
    /*Disable Charging */
    Status = SmartChargerLib_ChargerEnable(FALSE);
    SMARTCHARGER_DEBUG((EFI_D_WARN, "SmartChargerLib:: %a Charging Disabled \r\n", __FUNCTION__));
  }
  else
  {
    SmartChargerLib_ForceSysShutdown(CHGAPP_RESET_AFP);
  }

  return Status;
}


/*This API initiates different types of reset */
EFI_STATUS SmartChargerLib_ForceSysShutdown(ChgAppSysShutdownType ShutdownType)
{
  EFI_STATUS Status = EFI_SUCCESS;

  EFI_RESET_TYPE   ResetType = EfiResetPlatformSpecific;

  /*Disable charging*/
  SMARTCHARGER_DEBUG((EFI_D_WARN, "SmartChargerLib:: %a Disable charging ShutDown Type = %d \r\n", __FUNCTION__, ShutdownType));
  SmartChargerLib_ChargerEnable(FALSE);

  if (!RETAIL)
  {
    /* Flush serial buffer in production mode */
    SerialPortFlush();
    SMARTCHARGER_DEBUG((EFI_D_WARN, "SmartChargerLib:: %a SerialPortFlush Shutdown Type = %d \r\n", __FUNCTION__, ShutdownType));
    SerialPortFlush();
  }

  /* Clean up */
  Status = SmartChargerLib_Exit();

  switch (ShutdownType)
  {
  case CHGAPP_RESET_SHUTDOWN:
    SMARTCHARGER_DEBUG((EFI_D_WARN, "SmartChargerLib:: %a CHGAPP_RESET_SHUTDOWN.\r\n", __FUNCTION__));
    ResetType = EfiResetShutdown;
    break;
  case CHGAPP_RESET_COLD:
    SMARTCHARGER_DEBUG((EFI_D_WARN, "SmartChargerLib:: %a CHGAPP_RESET_COLD.\r\n", __FUNCTION__));
    ResetType = EfiResetCold;
    break;
  case CHGAPP_RESET_SHUTDOWN_USB_DC_PON_DISABLED:
     /*Disable USB as PON trigger and issue shut down */
     if (NULL != PmicPwronProtocol)
     {
       // TBD to make sure on PON1 disabling
       Status |= PmicPwronProtocol->EnableEdgePonTrigger(PM_DEVICE_0, EFI_PM_PON_OPTION_PON1_REDGE_PON, TRUE);
     }
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

UINT64 SmartChargerLib_TimeGet64(void)
{
  UINT64 Result = 0 ;
  Result = GetPerformanceCounter();
  return(Result);
}

UINT64 SmartChargerLib_CalculateTime()
{
  UINTN  CpuFrequency = 0;
  UINT64 TotalTicks = 0;
  UINT64 Seconds =0;

  /* Debug Print CPU Frequncey */
  TotalTicks   = SmartChargerLib_TimeGet64();
  /* Debug Print CPU Frequncey */
  CpuFrequency = GetPerformanceCounterProperties(NULL, NULL);
  Seconds = MultU64x32(TotalTicks, 1000000);
  Seconds = DivU64x32(Seconds, CpuFrequency);
  Seconds /= 1000000;

  return Seconds;
}

UINT64  SmartChargerLib_CalculateTimeMSec()
{
  UINTN  CpuFrequency = 0;
  UINT64 TotalTicks = 0;
  UINT64 Seconds =0;

  /* Debug Print CPU Frequncey */
  TotalTicks   = SmartChargerLib_TimeGet64();
  /* Debug Print CPU Frequncey */
  CpuFrequency = GetPerformanceCounterProperties(NULL, NULL);
  Seconds = MultU64x32(TotalTicks, 1000000);
  Seconds = DivU64x32(Seconds, CpuFrequency);
  Seconds /= 1000;

  return Seconds;
}

EFI_STATUS SmartChargerLib_GetThermalStatus( ChargerLibThermalStatus *ThermalStatus)
{
  EFI_STATUS Status = EFI_SUCCESS;
  INT32 CurrTemp = 0;
  STATIC ChargerLibThermalStatus ThermalSts = CHARGERLIB_THERMAL_INIT; /* Start with Init */

  if(NULL == ThermalStatus)
    return EFI_DEVICE_ERROR;
  Status = SmartChargerLib_GetTSensTemp(&CurrTemp);
  if (Status != EFI_SUCCESS)
  {
    SMARTCHARGER_FILE_UART_DEBUG((EFI_D_WARN,"SmartChargerLib:: %a Failed = %r \r\n", __FUNCTION__, Status));
    *ThermalStatus = ThermalSts = CHARGERLIB_THERMAL_STATUS_MAX;/*Updated output params*/
    return Status;
  }

  /*  Handle if any Error */
  if(EFI_ERROR(Status))
    return Status;

  switch(ThermalSts)
  {
    case CHARGERLIB_THERMAL_INIT:
    case CHARGERLIB_THERMAL_OK:
      if (CurrTemp >= gSmartChargerLibCfgData.thermal_configs.tsens_extreme_Temp)
      {
        *ThermalStatus = ThermalSts = CHARGERLIB_THERMAL_CRITICAL;/*Updated output params*/
      }
      else if (CurrTemp >= gSmartChargerLibCfgData.thermal_configs.tsens_high_temp )
      {
        *ThermalStatus = ThermalSts = CHARGERLIB_THERMAL_HIGH;
      }
      else
      {
        *ThermalStatus = ThermalSts = CHARGERLIB_THERMAL_OK;
      }
    break;

    case CHARGERLIB_THERMAL_HIGH:
      if (CurrTemp >= gSmartChargerLibCfgData.thermal_configs.tsens_extreme_Temp)
      {
        *ThermalStatus = ThermalSts = CHARGERLIB_THERMAL_CRITICAL;/*Updated output params*/
      }
      else if ((CurrTemp > gSmartChargerLibCfgData.thermal_configs.tsens_low_temp) && (CurrTemp < gSmartChargerLibCfgData.thermal_configs.tsens_extreme_Temp))
      {
        *ThermalStatus = ThermalSts = CHARGERLIB_THERMAL_HIGH;
      }
      else if (CurrTemp <= gSmartChargerLibCfgData.thermal_configs.tsens_low_temp)
      {
        *ThermalStatus = ThermalSts = CHARGERLIB_THERMAL_OK;
      }
      else
      {
        SMARTCHARGER_DEBUG((EFI_D_WARN,"SmartChargerLib:: %a Error = %d ThermalSts = %d \r\n", __FUNCTION__, CurrTemp, ThermalSts));
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

  SMARTCHARGER_DEBUG((EFI_D_WARN,"SmartChargerLib:: %a CurrTsensTemp = %d ThermalStatus = %d\r\n", __FUNCTION__, CurrTemp, *ThermalStatus));

  return Status;
}


EFI_STATUS SmartChargerLib_GetTSensTemp( INT32 *TSensTemp)
{
  EFI_STATUS Status = EFI_SUCCESS;
  INT32 nTempDeciDegC = 0, nTempDegC = 0;

  STATIC EFI_TSENS_PROTOCOL *Tsens = NULL;

  if(NULL == Tsens)
  {
    Status = gBS->LocateProtocol(&gEfiTsensProtocolGuid, NULL, (VOID**)&Tsens);
    if (Status != EFI_SUCCESS)
    {
      SMARTCHARGER_FILE_UART_DEBUG((EFI_D_WARN,"SmartChargerLib:: %a TSENS Protocol Locate Failed %r \r\n", __FUNCTION__, Status));
      return Status;
    }
  }

  /* Loop if current temperature of device is greater than max */
  Status = Tsens->GetMaxTemp(&nTempDeciDegC);
  if (EFI_SUCCESS != Status)
  {
    SMARTCHARGER_DEBUG((EFI_D_WARN,"SmartChargerLib:: %a TSENS Protocol GetMaxTemp Failed = %r \r\n", __FUNCTION__, Status));
    /* break and exit to AFP ??*/
    return Status;
  }

  /*Temp is divide by 10 in C*/
  nTempDegC = nTempDeciDegC / 10;

  *TSensTemp  = nTempDegC;

  return Status;
}




EFI_STATUS SmartChargerLib_GetBatteryVoltageStatus(CHARGERLIB_VBATT_STATUS *pBatteryVoltageStatus)
{
  EFI_STATUS                   Status      = EFI_SUCCESS;
  STATIC UINT32                EmergencyShutdownCounter = 0;
  chargerlib_batt_status_info  BatteryStatus = {0};

  if (!pBatteryVoltageStatus)
  {
    return EFI_INVALID_PARAMETER;
  }

  *pBatteryVoltageStatus = ChargerLib_VBatt_Normal;

  Status = SmartChargerLib_GetBatteryStatus(&BatteryStatus);
  if( EFI_SUCCESS != Status )
  {
    SMARTCHARGER_DEBUG((EFI_D_WARN, "SmartChargerLib:: %a Error getting battery status = %r \r\n", __FUNCTION__, Status));
    return Status;
  }

  //DEBUG((EFI_D_WARN, "SmartChargerLib:: %a Battery voltage = %d mV \r\n", __FUNCTION__, BatteryStatus.BatteryVoltage));

  /* Return status if vbatt is good */
  if ((UINT32)BatteryStatus.BatteryVoltage > (gSmartChargerLibCfgData.schg_cfg_data.ChgFvMax + gSmartChargerLibCfgData.batt_volt_lim_high_delta))
  {
    EmergencyShutdownCounter = 0;
    SMARTCHARGER_DEBUG((EFI_D_WARN, "SmartChargerLib:: %a BatteryVoltage more than ChgFvMax \r\n", __FUNCTION__));


    Status = SmartChargerLib_GetBatteryStatus(&BatteryStatus);
    if( EFI_SUCCESS != Status )
    {
      SMARTCHARGER_DEBUG((EFI_D_WARN, "SmartChargerLib:: %a Error getting battery status = %r \r\n", __FUNCTION__, Status));
      return Status;
    }

    SMARTCHARGER_DEBUG((EFI_D_WARN, "SmartChargerLib:: %a Current Battery voltage =  0x%04X mV \r\n", __FUNCTION__, BatteryStatus.BatteryVoltage));

    /* Check for High Voltage Limit */
    if (((UINT32)(BatteryStatus.BatteryVoltage - VBATT_TOLERANCE)) > (gSmartChargerLibCfgData.schg_cfg_data.ChgFvMax + gSmartChargerLibCfgData.batt_volt_lim_high_delta))
    {
       SMARTCHARGER_DEBUG((EFI_D_WARN, "SmartChargerLib:: %a Current Battery voltage too high: %d mV, Threshold = %d mV \r\n",__FUNCTION__,
                                    BatteryStatus.BatteryVoltage, (gSmartChargerLibCfgData.schg_cfg_data.ChgFvMax + gSmartChargerLibCfgData.batt_volt_lim_high_delta)));
      *pBatteryVoltageStatus = ChargerLib_VBatt_TooHigh;
    }
  }

  /* Check for vbatt below emergency shutdown limit */
  else if ((UINT32)BatteryStatus.BatteryVoltage < gSmartChargerLibCfgData.emergency_shutdown_vbatt)
  {
    if((INT16)BatteryStatus.ChargeCurrent < 0) /* Battery is discharging */
    {
      SMARTCHARGER_DEBUG((EFI_D_WARN, "SmartChargerLib:: %a Current BattVoltage = %d mV is below EmergencyShutdownVbatt = %d mV ", __FUNCTION__,
                    BatteryStatus.BatteryVoltage, gSmartChargerLibCfgData.emergency_shutdown_vbatt));
      if(EmergencyShutdownCounter >= MAX_EMERGENCY_SHUTDOWN_COUNT)
      {
        *pBatteryVoltageStatus = ChargerLib_VBatt_BelowThreshold;
        SMARTCHARGER_DEBUG((EFI_D_WARN, "SmartChargerLib:: %a EmergencyShutdownCounter Exceeded : \r\n", __FUNCTION__ ));
        SMARTCHARGER_DEBUG((EFI_D_WARN, " Curr Battery voltage = 0x%04X mV is below threshold = %d mV \r\n",
          BatteryStatus.BatteryVoltage, gSmartChargerLibCfgData.emergency_shutdown_vbatt ));
      }
      else
      {
        SMARTCHARGER_DEBUG((EFI_D_WARN, "SmartChargerLib:: %a EmergencyShutdownCounter = %d \r\n", __FUNCTION__, EmergencyShutdownCounter));
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

EFI_STATUS SmartChargerLib_GetErrors(BOOLEAN vbattChecking, CHARGERLIB_CHARGING_ERROR_TYPES  *pChargingError)
{
  EFI_STATUS                        Status            = EFI_SUCCESS;
  ChargerLibThermalStatus           ThermalStatus     = CHARGERLIB_THERMAL_STATUS_MAX;
  CHARGERLIB_VBATT_STATUS           BattVoltageStatus = ChargerLib_VBatt_Normal;
  BOOLEAN                           ChargerPresent    = FALSE;
  BOOLEAN     BatteryPresent = FALSE;

  if (!pChargingError)
  {
    return EFI_INVALID_PARAMETER;
  }

  *pChargingError = CHARGERLIB_CHARGING_ERROR_NONE;

  /* Check Critical errors first */

    if (!gSmartChargerLibCfgData.schg_cfg_data.DISABLEBATTERY)
    {
  /* Battery Presence Detection */
  Status = SmartChargerLib_GetBatteryPresence(&BatteryPresent);
  if (Status != EFI_SUCCESS)
      SMARTCHARGER_DEBUG((EFI_D_WARN, "SmartChargerLib:: %a  GetBatteryPresence Error: %r \r\n", __FUNCTION__, Status ));

  if (!BatteryPresent)
  {
    *pChargingError = CHARGERLIB_CHARGING_ERROR_BATTERY_NOT_DETECTED;
    SMARTCHARGER_DEBUG(( EFI_D_ERROR, "SmartChargerLib:: %a ERROR Battery not detected \r\n", __FUNCTION__));
    return Status;//Got Critical error, return
  }

  if (vbattChecking)
  {
    /* Check Voltage errors*/
    if(EFI_SUCCESS == SmartChargerLib_GetBatteryVoltageStatus(&BattVoltageStatus))
    {
      if(ChargerLib_VBatt_TooHigh == BattVoltageStatus)
      {
        *pChargingError = CHARGERLIB_CHARGING_ERROR_VBATT_OUTSIDE_RANGE;
        SMARTCHARGER_DEBUG(( EFI_D_ERROR, "SmartChargerLib:: %a ERROR Battery voltage out of range \r\n", __FUNCTION__));
        return Status;
      }
      else if(ChargerLib_VBatt_BelowThreshold == BattVoltageStatus)
      {
        *pChargingError = CHARGERLIB_CHARGING_ERROR_LOW_VBATT;
        SMARTCHARGER_DEBUG(( EFI_D_ERROR, "SmartChargerLib:: %a ERROR Battery voltage below threshold \r\n", __FUNCTION__));
        return Status;
      }
    }
    else
    {
      *pChargingError = CHARGERLIB_CHARGING_ERROR_FG_ERROR;
       SMARTCHARGER_DEBUG(( EFI_D_ERROR, "SmartChargerLib:: %a ERROR FG Error from  SmartChargerLib_GetBatteryVoltageStatus \r\n", __FUNCTION__));
    }
  }
  }

  /* Check Charge Source Presence */
  Status = SmartChargerLib_GetChargerPresence(&ChargerPresent);
  if (FALSE == ChargerPresent)
  {
    *pChargingError = CHARGERLIB_CHARGING_ERROR_CHARGING_SOURCE_NOT_DETECTED;
    SMARTCHARGER_DEBUG(( EFI_D_ERROR, "SmartChargerLib::%a ERROR Charging source not detected \r\n", __FUNCTION__));
  }

  /* Check MSM tsens temp if enabled */
  if(TRUE == gSmartChargerLibCfgData.sw_thermal_mitigation_enable)
  {
    Status |= SmartChargerLib_GetThermalStatus(&ThermalStatus);
    if (EFI_SUCCESS == Status)
    {
      CHARGERLIB_CHARGING_ERROR_TYPES thermalError = CHARGERLIB_CHARGING_ERROR_TSENSE_OK;
      /* Charging Error will be updated if tesens critical or time out error take place */
      Status |= SmartChargerLib_HandleThermalErrorType(ThermalStatus, &thermalError);
      if (EFI_SUCCESS != Status)
      {
        *pChargingError = CHARGERLIB_CHARGING_ERROR_TSENSE_CRITICAL;
        SMARTCHARGER_DEBUG(( EFI_D_ERROR, "SmartChargerLib:: %a SmartChargerLib_HandleThermalErrorType Failed = %r \r\n",__FUNCTION__, Status));
      }
      if((thermalError == CHARGERLIB_CHARGING_ERROR_TSENSE_HIGH )
         || (thermalError == CHARGERLIB_CHARGING_ERROR_TSENSE_TIMEOUT)
         || (thermalError == CHARGERLIB_CHARGING_ERROR_TSENSE_CRITICAL))
      {
        *pChargingError = thermalError;
        SMARTCHARGER_DEBUG(( EFI_D_ERROR, "SmartChargerLib:: %a Tsense pChargingError = %d \r\n",__FUNCTION__, pChargingError));
      }

      if ( (thermalError == CHARGERLIB_CHARGING_ERROR_TSENSE_HIGH) &&  (FALSE == ChargerPresent))
      {
        if(gSmartChargerLibCfgData.schg_cfg_data.DISABLEBATTERY)
        {
           *pChargingError = CHARGERLIB_CHARGING_ERROR_CHARGING_SOURCE_NOT_DETECTED;
        }
        else
        {
        chargerlib_batt_status_info BatteryStatus = { 0 };
        //need to check soc or vbatt, if vbatt low, no charger presense then report no charger presense to shutdown
        Status |= SmartChargerLib_GetBatteryStatus(&BatteryStatus);
        if (BatteryStatus.StateOfCharge < gSmartChargerLibCfgData.boot_to_hlos_threshold_in_soc)
        {
           *pChargingError = CHARGERLIB_CHARGING_ERROR_CHARGING_SOURCE_NOT_DETECTED;
        }
        }
      }
      //no need to handle else condition
      //CHARGERLIB_CHARGING_ERROR_TSENSE_HIGH chargerPresent will be considered as thermal Error
      //if there is no thermalError, then do not overwrite pChargingError, so no action here either.
    }
    else
    {
      SMARTCHARGER_FILE_UART_DEBUG((EFI_D_WARN,"SmartChargerLib:: %a SmartChargerLib_GetThermalStatus Failed %r \r\n", __FUNCTION__,Status));
      /* break and exit to AFP ??*/
      *pChargingError = CHARGERLIB_DEVICE_ERROR;
      return Status;
    }
  }

  SMARTCHARGER_DEBUG((EFI_D_ERROR, "SmartChargerLib:: %a pChargingError = %d \r\n", __FUNCTION__, *pChargingError));
  return Status;
}

EFI_STATUS GetSdpMaxCurrent(UINT32 *pSdpMaxCurrent)
{
  EFI_STATUS Status = EFI_SUCCESS;
  EFI_USBFN_CHG_SRV_SPEED_TYPE                SdpSpeed = USB_CHG_SPD_UNKNOWN;

  if(NULL == pSdpMaxCurrent)
  {
    return EFI_INVALID_PARAMETER;
  }

  *pSdpMaxCurrent = USB_2_IUSB_MAX;

  if (!UsbInitProtocol)
  {
    Status = gBS->LocateProtocol(&gEfiUsbInitProtocolGuid, NULL, (VOID **)&UsbInitProtocol);
  }

  if (!UsbChgSrvProtocol)
  {
    Status |= gBS->LocateProtocol(&gEfiUsbfnChgSrvProtocolGuid, NULL, (VOID **)&UsbChgSrvProtocol);
  }

  if((EFI_SUCCESS == Status) && (NULL != UsbInitProtocol) && (NULL != UsbChgSrvProtocol))
  {
    if((FALSE == gStartControllerCalled))
    {
      Status |= UsbInitProtocol->StartController(UsbInitProtocol, 0, EFI_USB_PERIPHERAL_MODE);

      Status |= UsbChgSrvProtocol->StartController(UsbChgSrvProtocol);

      if(EFI_SUCCESS == Status)
      {
        /* Store Enumeration done state, since we will need to call StopController in case of any error and charger swap*/
        gStartControllerCalled = TRUE;
        SMARTCHARGER_DEBUG((EFI_D_WARN, "SmartChargerLib:: %a success from StartController API, Status: %r \r\n", __FUNCTION__, Status));
      }
      else
      {
        SMARTCHARGER_DEBUG((EFI_D_WARN, "SmartChargerLib:: %a error from StartController API, Status: %r \r\n", __FUNCTION__, Status));
      }
    }
    else
    {
      WaitForTimeoutNoKey (USB_ENUM_WAIT_DURATION);
    }

    Status |= UsbChgSrvProtocol->GetSpeed(UsbChgSrvProtocol, &SdpSpeed);
    if(EFI_SUCCESS == Status)
    {/*If Status is not success, SDP */
      if(USB_CHG_SPD_SUPER == SdpSpeed)
      {
        *pSdpMaxCurrent = USB_3_IUSB_MAX;
        SMARTCHARGER_DEBUG((EFI_D_WARN, "SmartChargerLib:: %a SDP 3.0 Detected, ChargerCurrent = %d mA \r\n", __FUNCTION__, *pSdpMaxCurrent));
        return Status;
      }
    }
    else
    {
       SMARTCHARGER_DEBUG((EFI_D_WARN, "SmartChargerLib:: %a error getting SDP Speed, Status: : %r \r\n", __FUNCTION__, Status));
    }
  }
  else
  {
    SMARTCHARGER_DEBUG((EFI_D_WARN, "SmartChargerLib:: %a error locating UsbChgSrv Protocol, Status: : %r \r\n", __FUNCTION__, Status));
    /*if the protocol is not supported, we will fall back to USB2.0 and return SUCCESS*/
    Status = EFI_SUCCESS;
  }

  *pSdpMaxCurrent = USB_2_IUSB_MAX;
  SMARTCHARGER_DEBUG((EFI_D_WARN, "SmartChargerLib:: %a SDP 2.0 Detected, ChargerCurrent = %d mA \r\n", __FUNCTION__, *pSdpMaxCurrent));

  return Status;
}

EFI_STATUS SmartChargerLib_GetChgWdogStatus( CHARGERLIB_CHG_WDOG_STS_TYPE *pChgWdogSts)
{
    return EFI_SUCCESS;
}

EFI_STATUS SmartChargerLib_InitializeCharging( VOID )
{
  EFI_STATUS Status = EFI_SUCCESS;
  EFI_USB_PWR_CTRL_TYPEC_PORT_STATUS TypeCPortStatus;

  //SMARTCHARGER_DEBUG((EFI_D_WARN, "SmartChargerLib:: %a Entry \r\n", __FUNCTION__));

  if(gSmartChargerLibCfgData.schg_cfg_data.SWRoleSelectEnable == TRUE)
  {
    // Call Role Select API to set  ANX7411 ROLE=UFP
    Status = UsbPwrCtrlProtocol->GetTypeCPortStatus(EFI_USB_PWR_CTRL_USBPORT_CHARGER, &TypeCPortStatus);
    if(EFI_ERROR(Status)) {return Status;}

    //SMARTCHARGER_DEBUG((EFI_D_ERROR, "%a PowerRole: %d\r\n", __FUNCTION__, TypeCPortStatus.PowerRole));

    if( (TRUE == TypeCPortStatus.ConnectSts) && (EFI_USB_PWR_CTRL_TYPEC_POWERROLE_SINK == TypeCPortStatus.PowerRole) )
    {
        // Call Initial Role Select API to Set ANX7411 ROLE=UFP
        Status = UsbPwrCtrlProtocol->SetTypeCPortRole(EFI_USB_PWR_CTRL_USBPORT_CHARGER, EFI_USB_PWR_CTRL_TYPEC_POWERROLE_SINK);
        // Call SinkEn API to Set ROHM VIN_CTRL_SET(0Ah), bit 6=1 (VBUS Input ON)
        Status |= ChargerExtHwLib_SetSinkEn(TRUE);
        if(EFI_ERROR(Status)) {return Status;}
    }
  }

  Status |= SmartChargerLib_ConfigureUsbCurrentSetting();

  if( FALSE == gChargingEnabled )
  {
    //SMARTCHARGER_DEBUG((EFI_D_WARN, "SmartChargerLib:: %a Enabling charging \r\n", __FUNCTION__));
    Status |= SmartChargerLib_ChargerEnable(TRUE);

    gChargingEnabled = TRUE;
  }
  //else
  //    {
  //    SMARTCHARGER_DEBUG((EFI_D_WARN, "SmartChargerLib:: %a Charging already enabled \r\n", __FUNCTION__));
  //    }

  return Status;
}


EFI_STATUS SmartChargerLib_GetDebugBoardAction(CHARGERLIB_ERROR_ACTION_TYPE *pDebugBoardAction)
{
   EFI_STATUS Status = EFI_SUCCESS;
// ToDo: Update this with Debug mode support
  return Status;
}

EFI_STATUS SmartChargerLib_GetErrorAction( CHARGERLIB_CHARGING_ERROR_TYPES  ChargingErrorType, CHARGERLIB_ERROR_ACTION_TYPE *pErrorAction)
{
  EFI_STATUS  Status = EFI_SUCCESS;
  chargerlib_batt_status_info BatteryStatus = {0};
  EFI_QCOM_PMIC_PON_PROTOCOL			 *PmicPonProtocol = NULL;
  EFI_PM_PON_REASON_TYPE PM_PonReasonType;

  if(!pErrorAction)
  {
    return EFI_INVALID_PARAMETER;
  }

  *pErrorAction = CHARGERLIB_ERROR_ACTION_NONE;


  switch (ChargingErrorType)
  {
    case CHARGERLIB_CHARGING_ERROR_NONE:
    case CHARGERLIB_CHARGING_ERROR_TSENSE_OK:
        /* No Action just continue */
        break;
    case CHARGERLIB_CHARGING_ERROR_TSENSE_CRITICAL:
    case CHARGERLIB_CHARGING_ERROR_BATT_TEMP_OUTSIDE_OPERATIONAL_RANGE:
        *pErrorAction = CHARGERLIB_ERROR_ACTION_CRITICAL_SHUTDOWN;
        break;
    case CHARGERLIB_CHARGING_ERROR_BATTERY_NOT_DETECTED:
         *pErrorAction = CHARGERLIB_ERROR_ACTION_SHUTDOWN;
         break;
    case CHARGERLIB_CHARGING_ERROR_LOW_VBATT:
         /* Emergency Low VBatt shutdown */
         *pErrorAction = CHARGERLIB_ERROR_ACTION_SHUTDOWN_USB_DC_PON_DISABLED;
      break;
    case CHARGERLIB_CHARGING_ERROR_TSENSE_TIMEOUT:
    case CHARGERLIB_CHARGING_ERROR_ADC_ERROR:
    case CHARGERLIB_CHARGING_ERROR_FG_ERROR:
    case CHARGERLIB_CHARGING_ERROR_VBATT_OUTSIDE_RANGE:
    case CHARGERLIB_CHARGING_ERROR_CHARGING_TIMEOUT:
    case CHARGERLIB_DEVICE_ERROR:
        *pErrorAction = CHARGERLIB_ERROR_ACTION_SHUTDOWN;
        break;
    case CHARGERLIB_CHARGING_ERROR_CHARGING_SOURCE_NOT_DETECTED:
          if (!gSmartChargerLibCfgData.schg_cfg_data.DISABLEBATTERY)
          {
            Status = SmartChargerLib_GetBatteryStatus(&BatteryStatus);
            if( EFI_SUCCESS != Status )
            {
                SMARTCHARGER_DEBUG((EFI_D_WARN, "SmartChargerLib:: %a Error getting battery status = %r \r\n", __FUNCTION__, Status));
                return Status;
            }
		
			Status = gBS->LocateProtocol( &gQcomPmicPonProtocolGuid,
										 NULL,
										(VOID **)&PmicPonProtocol
			);
			
			if (Status != EFI_SUCCESS)
       {
				SMARTCHARGER_DEBUG(( EFI_D_ERROR, "SmartChargerLib:: %a Error locating PmicPonProtocol \r\n", __FUNCTION__));
				return Status;
       }
			Status = PmicPonProtocol->GetPonReason(PM_DEVICE_0 ,&PM_PonReasonType);
			if (Status != EFI_SUCCESS)
			{
				SMARTCHARGER_DEBUG(( EFI_D_ERROR, "SmartChargerLib:: %a Error getting PON reason \r\n", __FUNCTION__));
				return Status;
			}
	
			if((PM_PonReasonType.KPDPWR || PM_PonReasonType.HARD_RESET) && (BatteryStatus.StateOfCharge > gSmartChargerLibCfgData.boot_to_hlos_threshold_in_soc) )
       {
				//if already reached threshold,it's not an error and goot to boot 
            *pErrorAction = CHARGERLIB_ERROR_ACTION_GOOD_TO_BOOT;
				SMARTCHARGER_FILE_UART_DEBUG(( EFI_D_WARN, "SmartChargerLib:: %a Charger not detected , PM_PonReasonType : 0x%x, GOOD TO BOOT \r\n", __FUNCTION__, PM_PonReasonType)); 
       }
       else
			{
			    *pErrorAction = CHARGERLIB_ERROR_ACTION_SHUTDOWN;	
				SMARTCHARGER_FILE_UART_DEBUG(( EFI_D_WARN, "SmartChargerLib:: %a Charger not detected, PM_PonReasonType : 0x%x, SHUTDOWN \r\n", __FUNCTION__, PM_PonReasonType)); 
       }
          }
          else
          {//if debug board is connected, goot to boot
            *pErrorAction = CHARGERLIB_ERROR_ACTION_GOOD_TO_BOOT;
          }
       break;
    case CHARGERLIB_CHARGING_ERROR_TSENSE_HIGH:
         *pErrorAction = CHARGERLIB_ERROR_ACTION_TSENSE_HIGH_WAIT;
         break;
    case CHARGERLIB_CHARGING_ERROR_DEBUG_BOARD:
         Status = SmartChargerLib_GetDebugBoardAction(pErrorAction);
         break;
    default:
         break;
  }

  return Status;
}

EFI_STATUS SmartChargerLib_EnableWdog(BOOLEAN Enable)
{
  return EFI_SUCCESS;
}

EFI_STATUS SmartChargerLib_PetChgWdog( VOID )
{
  return EFI_SUCCESS;
}

/**
  Charger Lib Exit

  @return
  EFI_SUCCESS:           Function returned successfully.
  EFI_DEVICE_ERROR:      The physical device reported an error.
  EFI_UNSUPPORTED:       No Library function linked
*/
EFI_STATUS SmartChargerLib_Exit( VOID )
{
  EFI_STATUS     Status      = EFI_SUCCESS;
  STATIC BOOLEAN ChgExitDone = FALSE;

  if(FALSE == ChgExitDone)
  {
    SMARTCHARGER_DEBUG((EFI_D_WARN, "SmartChargerLib:: %a Exiting \r\n", __FUNCTION__));
    /* ChargerExit is called only once */
    ChgExitDone = TRUE;

    if (!gSmartChargerLibCfgData.schg_cfg_data.DISABLEBATTERY)
    {
      Status = SmartChargerLibFG_Deinit();

      if(gSmartChargerLibCfgData.schg_cfg_data.SWRoleSelectEnable == TRUE)
      {
        /* Enable SINK for no Charger connected shutdonw case in UEFI,
           so Device can detect USB PON upon insertion */
        Status = ChargerExtHwLib_SetSinkEn(TRUE);
        if(EFI_ERROR(Status))
        {
          SMARTCHARGER_DEBUG((EFI_D_WARN, "SmartChargerLib:: %a _SetSinkEn Failed, Status: 0x%08X\n", __FUNCTION__, Status));
        }
      }
    }

    Status |= ChargerExtHwLib_Exit();
    if (Status != EFI_SUCCESS)
    {
        SMARTCHARGER_DEBUG((EFI_D_WARN, "SmartChargerLib:: %a Error exiting Charger and FG Libs, Status: 0x%08X\n",__FUNCTION__, Status));
    }
  }

  if(TRUE == gStartControllerCalled)
  {
    if (!UsbChgSrvProtocol)
    {
      Status |= gBS->LocateProtocol(&gEfiUsbfnChgSrvProtocolGuid, NULL, (VOID **)&UsbChgSrvProtocol);
    }

    Status |= UsbChgSrvProtocol->StopController(UsbChgSrvProtocol);

    if (!UsbInitProtocol)
    {
      Status |= gBS->LocateProtocol(&gEfiUsbInitProtocolGuid, NULL, (VOID **)&UsbInitProtocol);
    }

    Status |= UsbInitProtocol->StopController(UsbInitProtocol, 0, EFI_USB_PERIPHERAL_MODE);

    if(EFI_SUCCESS != Status)
    {
      SMARTCHARGER_DEBUG((EFI_D_WARN, "SmartChargerLib:: %a Error with StopController, Status: 0x%08X\n",__FUNCTION__, Status));
    }

    gStartControllerCalled = FALSE;
  }

  return Status;
}

EFI_STATUS SmartChargerLib_RecordThermalStartTimeOut()
{
  EFI_STATUS Status   = EFI_SUCCESS;

  /*Update time stamp to global */
  Status = SmartChargerLib_GetSEllapsedFromBoot(&ThermalTimeOutTimeStamp);
  if (Status == EFI_SUCCESS)
  {
    SMARTCHARGER_FILE_UART_DEBUG((EFI_D_WARN, "SmartChargerLib:: %a ThermalTimeOutTimeStamp = %d Seconds \r\n",__FUNCTION__, ThermalTimeOutTimeStamp));
  }
  else
  {
    SMARTCHARGER_FILE_UART_DEBUG((EFI_D_WARN, "ChargerApp:: %a Error = %d \r\n",__FUNCTION__, Status));
  }

  return Status;
}

EFI_STATUS SmartChargerLib_GetSEllapsedFromBoot(UINT32 *PmicTimeinS)
{
  EFI_STATUS Status = EFI_SUCCESS;
  UINT32 PmicTime = 0;
  STATIC EFI_QCOM_PMIC_RTC_PROTOCOL  *pPmicRtc = NULL;

  if(!PmicTimeinS)
    return EFI_INVALID_PARAMETER;

  if(!pPmicRtc)
  {
    Status = gBS->LocateProtocol( &gQcomPmicRtcProtocolGuid,
                                  NULL,
                                  (VOID**) &pPmicRtc);
    if (Status != EFI_SUCCESS) {
        return Status;
    }
  }

  Status |= pPmicRtc->GetTimeValueRaw(0, &PmicTime);
  if (Status != EFI_SUCCESS)
  {
    SMARTCHARGER_FILE_UART_DEBUG((EFI_D_WARN, "SmartChargerLib:: %a Error = %d \r\n",__FUNCTION__, Status));
  }

  *PmicTimeinS = PmicTime;

  return Status;
}

EFI_STATUS SmartChargerLib_HandleThermalHigh(CHARGERLIB_CHARGING_ERROR_TYPES *pChargingError)
{
  EFI_STATUS Status = EFI_SUCCESS;
  UINT32 PmicCurrTimeinS = 0;
  UINT32 ConfigThermalTimeOutinS = 0;

  if(NULL == pChargingError)
    return EFI_INVALID_PARAMETER;

  /* Assign default - but do not need to override charge none error type unless it is time out*/
  /* *pChargingError = CHARGERLIB_CHARGING_ERROR_TSENSE_OK; */

  /* Note Thermal Start Time */
  if(THERMAL_TIMEOUT_NOTSET == ThermalTimeOutTimeStamp)
  {
    /*Record start Time in Seconds */
    Status |= SmartChargerLib_RecordThermalStartTimeOut();
    *pChargingError = CHARGERLIB_CHARGING_ERROR_TSENSE_HIGH;
  }
  else
  {
    /*Timeout in seconds */
    ConfigThermalTimeOutinS = gSmartChargerLibCfgData.thermal_configs.tsens_timeout_mins * MIN_TO_SEC;

    Status |= SmartChargerLib_GetSEllapsedFromBoot(&PmicCurrTimeinS);
    if((PmicCurrTimeinS - ThermalTimeOutTimeStamp ) >= ConfigThermalTimeOutinS)
    {
      SMARTCHARGER_FILE_UART_DEBUG((EFI_D_WARN, "SmartChargerLib:: %aChgAppHandleThermalHigh TimeOut: Shutdown device \r\n", __FUNCTION__));
      /*DEBUG */
      SMARTCHARGER_FILE_UART_DEBUG((EFI_D_WARN, "SmartChargerLib:: %a PmicCurrTimeinS = %d s, ThermalTimeOutTimeStamp =  %d s, ",
                               __FUNCTION__, PmicCurrTimeinS, ThermalTimeOutTimeStamp ));
      SMARTCHARGER_FILE_UART_DEBUG((EFI_D_WARN, "ConfigThermalTimeOutinS = %d s \r\n", ConfigThermalTimeOutinS));
      *pChargingError = CHARGERLIB_CHARGING_ERROR_TSENSE_TIMEOUT;
    }
    else
    { /*DEBUG */
      SMARTCHARGER_FILE_UART_DEBUG((EFI_D_WARN, "SmartChargerLib:: %a PmicCurrTimeinS = %d s, ThermalTimeOutTimeStamp = %d s, ",
                               __FUNCTION__, PmicCurrTimeinS, ThermalTimeOutTimeStamp));
      SMARTCHARGER_FILE_UART_DEBUG((EFI_D_WARN, " ConfigThermalTimeOutinS = %d s  Status = %d \r\n",
                              ConfigThermalTimeOutinS, Status));
      *pChargingError = CHARGERLIB_CHARGING_ERROR_TSENSE_HIGH;
    }
  }
  return Status;
}


EFI_STATUS SmartChargerLib_HandleThermalErrorType(ChargerLibThermalStatus ThermalStatus, CHARGERLIB_CHARGING_ERROR_TYPES *pChargingError)
{
  EFI_STATUS Status = EFI_SUCCESS;

  if(NULL == pChargingError)
    return EFI_INVALID_PARAMETER;

  /* Assign default - but do not need to override charge none error type unless it is time out*/
  /* *pChargingError = CHARGERLIB_CHARGING_ERROR_TSENSE_OK;*/

  switch(ThermalStatus)
  {
    /*Thermal CRITICAL shutodwn with AFP */
    case CHARGERLIB_THERMAL_CRITICAL:
      *pChargingError = CHARGERLIB_CHARGING_ERROR_TSENSE_CRITICAL;
      break;

    /*Themal timeout during HIGH thermal zone, shutdown */
    case CHARGERLIB_THERMAL_HIGH_TIMEOUT:
      *pChargingError = CHARGERLIB_CHARGING_ERROR_TSENSE_TIMEOUT;
      break;

    case CHARGERLIB_THERMAL_OK:
      /* Do not need to override charger error none here keeping below for easy reading */
      /* *pChargingError = CHARGERLIB_CHARGING_ERROR_TSENSE_OK; */

      /*Reset TimeOut Stamp*/
      if(THERMAL_TIMEOUT_NOTSET != ThermalTimeOutTimeStamp)
      {
        /* Enable Charging */
        Status |= SmartChargerLib_ChargerEnable(TRUE);
        SMARTCHARGER_DEBUG((EFI_D_WARN, "SmartChargerLib:: %a Thermal Time Record Reset \r\n", __FUNCTION__));
        ThermalTimeOutTimeStamp = 0;
      }
      break;

    /* Themal HIGH to wait for thermal to come down TsensLowTemp */
    case CHARGERLIB_THERMAL_HIGH:
      /* Disable Charging - take action will diable charging */
      /* Status |= SmartChargerLib_ChargerEnable(FALSE); */
      Status |= SmartChargerLib_HandleThermalHigh(pChargingError);
      if(EFI_SUCCESS != Status)
      {
        *pChargingError = CHARGERLIB_CHARGING_ERROR_TSENSE_CRITICAL;
        SMARTCHARGER_DEBUG((EFI_D_WARN, "SmartChargerLib:: %a ChargerLib_HandleThermalHigh Error = %r Setting Thermal Time out Timer \r\n", __FUNCTION__, Status));
      }
    break;

    default:
    case CHARGERLIB_THERMAL_STATUS_MAX:
      SMARTCHARGER_FILE_UART_DEBUG((EFI_D_WARN,"SmartChargerLib:: %a ThermalStatus = %d \r\n", __FUNCTION__, ThermalStatus));
      *pChargingError = CHARGERLIB_CHARGING_ERROR_TSENSE_CRITICAL;
    break;
  }

  SMARTCHARGER_FILE_UART_DEBUG((EFI_D_WARN,"SmartChargerLib:: %a ThermalStatus = %d \r\n", __FUNCTION__, ThermalStatus));
  return Status;
}

EFI_STATUS SmartChargerLib_GetChargingStatus(BOOLEAN *pChargingEnabled)
{
  EFI_STATUS Status = EFI_SUCCESS;
  BOOLEAN ChargingSts = 0;

  if(!pChargingEnabled)
    return EFI_INVALID_PARAMETER;

  *pChargingEnabled = FALSE;
  Status |= ChargerExtHwLib_GetChargingStatus(&ChargingSts);
  if(EFI_SUCCESS == Status )
  {
    *pChargingEnabled = ChargingSts;
  }
  return Status;
}

EFI_STATUS SmartChargerLib_ConfigureUsbCurrentSetting( VOID )
{
  EFI_STATUS Status = EFI_SUCCESS;
  UINT32     MaxCurrent = 0;
  EFI_USB_PWR_CTRL_HSUSB_CHGPORT_TYPE PortType = EFI_USB_PWR_CTRL_HSUSB_CHGPORT_UNKNOWN;
  UINT32     UsbInputCurrentLimitInMa = 0;
  EFI_USB_PWR_CTRL_PD_STATUS  PDStatus;
  EFI_USB_PWR_CTRL_TYPEC_PORT_STATUS TypeCPortStatus;

  if (!UsbPwrCtrlProtocol)
  {
    Status = gBS->LocateProtocol(&gQcomUsbPwrCtrlProtocolGuid, NULL, (VOID **)&UsbPwrCtrlProtocol);
    if(EFI_SUCCESS != Status || !UsbPwrCtrlProtocol)
    {
      SMARTCHARGER_DEBUG(( EFI_D_ERROR, "SmartChargerLib:: %a Can not locate USB PwrCtrl Protocol = %r \r\n", __FUNCTION__, Status));
      return EFI_DEVICE_ERROR;
    }

  }

    Status = UsbPwrCtrlProtocol->GetTypeCPortStatus (EFI_USB_PWR_CTRL_USBPORT_CHARGER, &TypeCPortStatus);
  Status |= UsbPwrCtrlProtocol->GetTypeCPortPDStatus (EFI_USB_PWR_CTRL_USBPORT_CHARGER, &PDStatus);

    if (EFI_SUCCESS != Status)
    {
        return Status;
    }


  if ((TypeCPortStatus.DetectedSinkType == EFI_USB_PWR_CTRL_TYPEC_PORT_SNK_DEBUG) ||
         (TypeCPortStatus.DetectedSourceType == EFI_USB_PWR_CTRL_TYPEC_PORT_SRC_UNORIENTED_RD_RD) )
  {
      UsbInputCurrentLimitInMa = MAX_3000_MA;

  }
  else if ((TypeCPortStatus.DetectedSinkType == EFI_USB_PWR_CTRL_TYPEC_PORT_SNK_3P0) ||
         (TypeCPortStatus.DetectedSinkType == EFI_USB_PWR_CTRL_TYPEC_PORT_SNK_1P5))
  {
    if (PDStatus.bPDConsumer && PDStatus.PDConsumerState.MaxCurrentMA)
    {
        UsbInputCurrentLimitInMa = PDStatus.PDConsumerState.MaxCurrentMA;
    }
    else if (TypeCPortStatus.DetectedSinkType == EFI_USB_PWR_CTRL_TYPEC_PORT_SNK_3P0)
    {
        UsbInputCurrentLimitInMa = MAX_3000_MA;
    }
    else if (TypeCPortStatus.DetectedSinkType == EFI_USB_PWR_CTRL_TYPEC_PORT_SNK_1P5)
    {
        UsbInputCurrentLimitInMa = MAX_1500_MA;
    }
  }
    else
    {
      Status |= SmartChargerLib_GetPortType(&PortType);
      if(EFI_SUCCESS == Status)
      {
        switch (PortType)
        {
          case EFI_USB_PWR_CTRL_HSUSB_CHGPORT_SDP:
            Status = GetSdpMaxCurrent(&MaxCurrent);
            if(USB_3_IUSB_MAX == MaxCurrent)
            {
                UsbInputCurrentLimitInMa = USB_3_IUSB_MAX;
            }
            else
            {
                UsbInputCurrentLimitInMa = USB_2_IUSB_MAX;
            }
            break;
          case EFI_USB_PWR_CTRL_HSUSB_CHGPORT_CDP:
          case EFI_USB_PWR_CTRL_HSUSB_CHGPORT_DCP:
            UsbInputCurrentLimitInMa = MAX_1500_MA;
            break;
          case EFI_USB_PWR_CTRL_HSUSB_CHGPORT_UNKNOWN:
            UsbInputCurrentLimitInMa = USB_DEFAULT_IUSB_MAX;
            break;
          default:
            /*Do nothing*/
            break;
        }

      }

  }
  SMARTCHARGER_DEBUG((EFI_D_ERROR, "SmartChargerLib:: %a Setting ICL to %d mA \r\n", __FUNCTION__, UsbInputCurrentLimitInMa));
  Status = SmartChargerLib_SetMaxUsbCurrent(UsbInputCurrentLimitInMa);

  if(Status != EFI_SUCCESS)
  {
    SMARTCHARGER_DEBUG((EFI_D_ERROR, "SmartChargerLib:: %a Failed to configure ICL\r\n", __FUNCTION__));
  }

  return Status;
}


EFI_STATUS SmartChargerLib_IsChargingSupported(BOOLEAN *pChargingSupported)
{
  EFI_STATUS Status = EFI_SUCCESS;
  BOOLEAN bBatteryPresence;

  *pChargingSupported = FALSE;

  //Checking Configuration
  if (!gSmartChargerLibCfgData.schg_cfg_data.DISABLEBATTERY)
  {
    Status = SmartChargerLib_GetBatteryPresence(&bBatteryPresence);
    if(gChargerHwDetected && bBatteryPresence )
    {
        *pChargingSupported = TRUE;
  }
  }
  else if(gChargerHwDetected )
  {
    *pChargingSupported = TRUE;
  }
  SMARTCHARGER_DEBUG((EFI_D_WARN, "SmartChargerLib:: %a Support Charging = %d\r\n", __FUNCTION__, *pChargingSupported));
  return EFI_SUCCESS;

}


EFI_STATUS SmartChargerLib_GetChargerPresence(BOOLEAN *pChargerPresent)
{
  EFI_STATUS Status = EFI_SUCCESS;
  BOOLEAN bVbusDetect = FALSE;
  EFI_USB_PWR_CTRL_TYPEC_PORT_STATUS TypeCPortStatus;

  if (!UsbPwrCtrlProtocol)
  {
    Status = gBS->LocateProtocol(&gQcomUsbPwrCtrlProtocolGuid, NULL, (VOID **)&UsbPwrCtrlProtocol);
    if ( EFI_SUCCESS != Status)
        return Status;
  }

  Status = UsbPwrCtrlProtocol->GetVbusDetectStatus(0, &bVbusDetect);
  Status |= UsbPwrCtrlProtocol->GetTypeCPortStatus (0, &TypeCPortStatus);
  if (EFI_SUCCESS != Status)
  {
      return Status;
  }
  if (bVbusDetect &&
      TypeCPortStatus.PowerRole == EFI_USB_PWR_CTRL_TYPEC_POWERROLE_SINK)
  {
    *pChargerPresent = TRUE;
  }
  else
  {
    *pChargerPresent = FALSE;
  }
  return Status;
}


EFI_STATUS SmartChargerLib_SetChargingTermCurrent(UINT32 ChargingTermCurrent)
{
  return ChargerExtHwLib_SetChargingTermCurrent(ChargingTermCurrent);
}

EFI_STATUS SmartChargerLib_SetFCCMax(UINT32 FCCMax)
{
  return ChargerExtHwLib_SetFCCMax(FCCMax);
}

EFI_STATUS SmartChargerLib_SetFVMax(UINT32 FVMax)
{
  return ChargerExtHwLib_SetFVMax(FVMax);
}


