/** @file SmartCharger.c

  Implements the SMART Charger protocol

  Copyright (c) 2017,  Qualcomm Technologies Inc. All rights reserved.

**/

/*=============================================================================
                              EDIT HISTORY


when         who     what, where, why
--------     ---     -----------------------------------------------------------
07/31/17     pbitra  Initial release

=============================================================================*/
#include <Uefi.h>

/**
  EFI interfaces
 */
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>

/**
  Charger PROTOCOL interface
 */
#include <Protocol/EFIQcomCharger.h>

/**
  EFI interfaces
 */
 #include <Uefi.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/QcomLib.h>

/**
  Protocol Dependencies
*/
#include <Protocol/EFIPlatformInfo.h>
#include <Library/QcomTargetLib.h>

/**
  Ext HW Dependencies
*/

#include "SmartChargerPlatform.h"

#include <api/pmic/charger/ChargerLibCommon.h>

EFI_QCOM_CHARGER_PROTOCOL QcomChargerProtocolImplementation;

/*===========================================================================*/
/*                  FUNCTIONS PROTOTYPES                                     */
/*===========================================================================*/
/**
SmartChargerInitialize()

@brief
Initializes charger module
*/
EFI_STATUS SmartChargerInitialize(IN EFI_HANDLE         ImageHandle,
                             IN EFI_SYSTEM_TABLE   *SystemTable)
{
  EFI_STATUS Status = EFI_SUCCESS;
  EFI_PLATFORMINFO_PLATFORM_TYPE PlatformType;

  Status = GetPlatformType(&PlatformType);
  if(EFI_SUCCESS != Status)
  {
    DEBUG((EFI_D_WARN, "SmartChargerDxe:: %a Error getting platform type = %r \r\n", __FUNCTION__, Status));
    return EFI_DEVICE_ERROR;
  }

  
  if (EFI_PLATFORMINFO_TYPE_CLS == PlatformType )
  {
	  /* Required Initialization */
	  Status = SmartChargerPlatform_Init();
	  if(EFI_SUCCESS == Status)
	  {
		Status = gBS->InstallMultipleProtocolInterfaces( &ImageHandle,
														  &gSmartChargerProtocolGuid,
														  &QcomChargerProtocolImplementation,
														  NULL );
	  }	  
  }
  
  return Status;
}

/**
EFI_QcomChargerEnableCharging()

@brief
Enables charging
*/
EFI_STATUS
EFIAPI 
EFI_QcomChargerEnableCharging
(
  IN BOOLEAN Enable
)
{
  EFI_STATUS Status = EFI_SUCCESS;

  Status = SmartChargerLib_ChargerEnable(Enable);

  return Status;
}

/**
EFI_QcomChargerSetMaxUsbCurrent()

@brief
Set maximum USB current
*/
EFI_STATUS
EFIAPI 
EFI_QcomChargerSetMaxUsbCurrent
(
  IN UINT32 MaxCurrent
)
{
  EFI_STATUS Status = EFI_SUCCESS;

  Status = SmartChargerLib_SetMaxUsbCurrent(MaxCurrent);

  return Status;
}

/**
EFI_QcomChargerGetMaxUsbCurrent()

@brief
Gets maximum USB current from config file
*/
EFI_STATUS
EFIAPI 
EFI_QcomChargerGetMaxUsbCurrent
(
  OUT UINT32 *pMaxCurrent
)
{
  return EFI_UNSUPPORTED;
}

/**
EFI_QcomChargerGetChargingStatus()

@brief
Initializes charger module
*/
EFI_STATUS
EFIAPI
EFI_QcomChargerGetChargingAction
(
  EFI_QCOM_CHARGER_ACTION_TYPE  *pChargingAction,
  EFI_QCOM_CHARGER_ACTION_INFO  *pChargerActionInfo,
  BOOLEAN vbattChecking
)
{
  EFI_STATUS Status = EFI_SUCCESS;
  if(NULL == pChargingAction)
  {
    return EFI_INVALID_PARAMETER;
  }

  Status = SmartChargerPlatform_GetChargingAction((EFI_QCOM_CHARGER_ACTION_TYPE *)pChargingAction, (SMART_CHARGER_PLATFORM_ACTION_INFO*)pChargerActionInfo, vbattChecking);

    
  return Status;
}


/**
EFI_QcomChargerTakeAction()

@brief
Initializes charger module
*/
EFI_STATUS
EFIAPI
EFI_QcomChargerTakeAction
(
  EFI_QCOM_CHARGER_ACTION_TYPE ChargingAction,
  CONST EFI_QCOM_CHARGER_ACTION_INFO *pChargerActionInfo
)
{
  EFI_STATUS Status = EFI_SUCCESS;

  Status = SmartChargerPlatform_TakeAction((EFI_QCOM_CHARGER_ACTION_TYPE)ChargingAction, (SMART_CHARGER_PLATFORM_ACTION_INFO *)pChargerActionInfo);

  return Status;
}

EFI_STATUS
EFIAPI 
EFI_QcomChargerDisplayImage
(
  IN EFI_QCOM_CHARGER_DISP_IMAGE_TYPE ImageType,
  IN BOOLEAN ClearScreen
)
{
//	return EFI_UNSUPPORTED;
	SMARTCHARGER_FILE_UART_DEBUG(( EFI_D_WARN, "SmartChargerDxe:: %a  \r\n",__FUNCTION__));

	return EFI_SUCCESS;
}


EFI_STATUS
EFIAPI 
EFI_QcomChargerDeInitialize
(
  EFI_QCOM_CHARGER_ACTION_TYPE ChargingAction
)
{
  EFI_STATUS Status = EFI_SUCCESS;

  Status = SmartChargerLib_Exit();

  return Status;
}



/**
EFI_QcomChargerGetBatteryPresence ()

@brief
Get battery presence status
*/
EFI_STATUS
EFIAPI
EFI_QcomChargerGetBatteryPresence
(
  OUT BOOLEAN  *pBatteryPresent
)
{
  EFI_STATUS Status = EFI_SUCCESS;
  
  if(NULL == pBatteryPresent)
  {
    return EFI_INVALID_PARAMETER;
  }
  
  Status = SmartChargerLib_GetBatteryPresence(pBatteryPresent);

  return Status;
}

/**
EFI_QcomChargerGetBatteryVoltage ()

@brief
Returns battery voltage
*/
EFI_STATUS
EFIAPI
EFI_QcomChargerGetBatteryVoltage
(
  OUT UINT32  *pBatteryVoltage
)
{
  EFI_STATUS Status = EFI_SUCCESS;
  EFI_PM_FG_BATT_STATUS_INFO BatteryStatus = {0};
  
  if(NULL == pBatteryVoltage)
  {
    return EFI_INVALID_PARAMETER;
  }
  *pBatteryVoltage = 0;

  Status = SmartChargerLib_GetBatteryStatus((chargerlib_batt_status_info*)&BatteryStatus);
  
  if(EFI_SUCCESS == Status)
  {
  *pBatteryVoltage = BatteryStatus.BatteryVoltage;
  }

  return Status;
}

/**
EFI_QcomChargerGetFileLogInfo ()

@brief
Returns Charger Log File handle information 
*/
EFI_STATUS
EFIAPI
EFI_QcomChargerGetFileLogInfo
(
  OUT EFI_QCOM_CHARGER_LOG_INFO *pFileLogInfo
)
{
  EFI_STATUS                     Status = EFI_SUCCESS;
  SMART_CHARGER_PLATFORM_LOG_INFO Info   = {0};

  if(NULL == pFileLogInfo)
  {
    return EFI_INVALID_PARAMETER;
  }

  Status = SmartChargerPlatformFile_GetLogInfo(&Info);
  if(Status == EFI_SUCCESS)
  {
    pFileLogInfo->gChargerLogHandle            = Info.gChargerLogHandle;
    pFileLogInfo->bPrintChargerAppDbgMsg       = Info.bPrintChargerAppDbgMsg;
    pFileLogInfo->bPrintChargerAppDbgMsgToFile = Info.bPrintChargerAppDbgMsgToFile;
	pFileLogInfo->fileLogLevelMask             = Info.fileLogLevelMask;
  }
  else
  {
    /* DEBUG */
    SMARTCHARGER_FILE_UART_DEBUG(( EFI_D_WARN, "SmartChargerDxe:: %a Error = %r \r\n",__FUNCTION__, Status));
  }

  return Status;
}

/**
EFI_QcomChargerDumpPeripheral ()

@brief
Debug Prints Charger Fg Peripheral  
*/
EFI_STATUS
EFIAPI
EFI_QcomChargerDumpPeripheral( )
{
	//return EFI_UNSUPPORTED;
	return EFI_SUCCESS;

}

/**
EFI_QcomChargerIsDcInValid ()

@brief
get if dcin is valid or not
*/
EFI_STATUS
EFIAPI
EFI_QcomChargerIsDcInValid(BOOLEAN* pIsDcInValid)
{
	return EFI_UNSUPPORTED;

}



/**
EFI_QcomChargerGetChargerConfig ()

@brief
Returns Charger Log File handle information 
*/
EFI_STATUS
EFIAPI
EFI_QcomChargerGetChargerConfig
(
  IN EFI_QCOM_CHARGER_CONFIG_KEY  ChargerCfgKey,
  OUT  UINT32 *KeyValue
)
{
  EFI_STATUS                     Status = EFI_SUCCESS;
  if(NULL == KeyValue)
  {
    return EFI_INVALID_PARAMETER;
  }

  Status = SmartChargerPlatformFile_GetChargerConfig(ChargerCfgKey, KeyValue);
  if(Status == EFI_SUCCESS)
  {
    SMARTCHARGER_DEBUG(( EFI_D_WARN, "SmartChargerDxe:: %a CfgKey = %d Value = %d \r\n",__FUNCTION__, ChargerCfgKey, *KeyValue));
  }
  else
  {
    /* DEBUG */
    SMARTCHARGER_DEBUG(( EFI_D_WARN, "SmartChargerDxe:: %a Error = %r \r\n",__FUNCTION__, Status));
  }

  return Status;
}

/**
EFI_QcomChargerIsChargingSupported ()

@brief
Returns If Charger is supported based on platform, configuration and run-time detection
*/
EFI_STATUS
EFIAPI
EFI_QcomChargerIsChargingSupported
(
  OUT  BOOLEAN *pIsChargingSupported
)
{
    //Calling Library API to check if this platform support charging or not
    EFI_STATUS                     Status = EFI_SUCCESS;
    *pIsChargingSupported = FALSE;
    Status = SmartChargerLib_IsChargingSupported(pIsChargingSupported);
    return Status;
}

/**
  PMIC FG UEFI Protocol implementation
 */
EFI_QCOM_CHARGER_PROTOCOL QcomChargerProtocolImplementation =
{
  QCOM_CHARGER_REVISION,
  EFI_QcomChargerEnableCharging,
  EFI_QcomChargerGetMaxUsbCurrent,
  EFI_QcomChargerSetMaxUsbCurrent,
  EFI_QcomChargerGetChargingAction,
  EFI_QcomChargerTakeAction,
  EFI_QcomChargerDisplayImage,
  EFI_QcomChargerGetBatteryPresence,
  EFI_QcomChargerGetBatteryVoltage,
  EFI_QcomChargerDeInitialize,
  EFI_QcomChargerGetFileLogInfo,
  EFI_QcomChargerDumpPeripheral,
  EFI_QcomChargerIsDcInValid,
  EFI_QcomChargerGetChargerConfig,
  EFI_QcomChargerIsChargingSupported
};
