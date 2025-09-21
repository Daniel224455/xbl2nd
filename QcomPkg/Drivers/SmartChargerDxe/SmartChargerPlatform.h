/*
This file is for LA vbatt based charging in UEFI, it initialize ChargerLib 
to use HW Jeita and implement the function to determine if the conditions to boot to HLOS are met.
*/
/** 
  @file  SmartChargerPlatform.h
  @brief Charger Vbatt based threshold charging UEFI Driver definitions.
         It initializes ChargerLibCommon, initializes HW Jeita 
         and implement the function to determine if the conditions to boot to HLOS are met.
*/
/*=============================================================================
  Copyright (c) 2017 Qualcomm Technologies, Incorporated.
  All rights reserved.
  Qualcomm Technologies, Confidential and Proprietary.
=============================================================================*/

/*=============================================================================
                              EDIT HISTORY


 when       who     what, where, why
 --------   ---     -----------------------------------------------------------
11/09/17    pbitra   Corrected SMART_CHARGER_PLATFORM_ACTION_INFO
 08/01/17   pbitra      Initial revision

=============================================================================*/

#ifndef __SMARTCHARGERPLATFORM_H__ 
#define __SMARTCHARGERPLATFORM_H__

#include <api/pmic/charger/ChargerLibCommon.h>
#include <Protocol/EFIPmicSchg.h>
#include <Protocol/EFIPmicFg.h>
#include <Protocol/EFIQcomCharger.h>

/*===========================================================================
MACRO DEFINITIONS
===========================================================================*/

#define SMART_CHG_WDOG_DISABLE               0
#define SMART_CHG_WDOG_DISABLE_ON_EXIT       1
#define SMART_CHG_WDOG_LEAVE_ENABLED_ON_EXIT 2

/*===========================================================================
TYPE DEFINITIONS
===========================================================================*/
typedef enum
{
  SMART_CHARGER_PLATFORM_STATUS_GOOD_TO_BOOT = 0,
  /**< Device is good to boot to HLOS */
  SMART_CHARGER_PLATFORM_STATUS_CHARGING,
  /**< Charging is on-going */
  SMART_CHARGER_PLATFORM_STATUS_ERROR,
  /**< Error occurred */
  SMART_CHARGER_PLATFORM_STATUS_PROFILE_LOAD,
  /**< Profile load is needed  */
  SMART_CHARGER_PLATFORM_STATUS_DEBUG_BOARD_GOOD_TO_BOOT,
  /**< Boot to HLOS */
  SMART_CHARGER_PLATFORM_STATUS_INVALID,
  /**< Error occurred */
}SMART_CHARGER_PLATFORM_STATUS_TYPE;


typedef struct
{
    BOOLEAN WipowerEnabled;
    /**< if wipower is enabled or not */

    UINT32  WipowerBootThreshold;
    /**< boot up threshold for wipower charging */

    BOOLEAN DCINDisableOnExit;
    /**< suspend dcin or not */

}SMART_CHARGER_PLATFORM_WIPOWER_CFGDATA_TYPE;

typedef struct {
  UINT32   CfgVersion;
  /**< Version number for CFG file*/

  BOOLEAN  ProgramBattThermCoeffs;
  /**<  Enables Batt therm coeffs to be programmed via configs instead of profiles */

  BOOLEAN  FullBattChargingEnabled;
  /**<  Enable full battery charging  */

  UINT32   DispSignOfLifeMaxThresholdMv;
  /**< battery Threshold to display */

  UINT32   SWFlashMinBattVoltageMv;
  /**< Min battery voltage for SW image flash required */

  SMART_CHARGER_PLATFORM_WIPOWER_CFGDATA_TYPE     WiPowerCfgData;

  chargerlib_cfgdata_type    ChargerLibCfgData;

} SMART_CHARGER_PLATFORM_CFGDATA_TYPE;


typedef struct 
{
  UINT8 *pFileBuffer;
  UINTN  DataSize;
}SMART_CHARGER_PLATFORM_BATT_PROFILE_DATA;

typedef enum
{
  SMART_CHARGER_PLATFORM_PROFILE_LOAD,
  SMART_CHARGER_PLATFORM_PROFILE_RESTART,
  SMART_CHARGER_PLATFORM_PROFILE_INVALID
}SMART_CHARGER_PLATFORM_BATT_PROFILE_STATE;

typedef struct 
{
  /* Enum for profile load/restart state */
  SMART_CHARGER_PLATFORM_BATT_PROFILE_STATE eProfileState;
}SMART_CHARGER_PLATFORM_BATT_PROFILE_STATUS;

typedef struct
{
  UINT32 StateOfCharge;
  INT32  ChargeCurrent;
  INT32  BatteryVoltage;
  INT32  BatteryTemperature;
}SMART_CHARGER_BATT_STATUS_INFO;

typedef enum
{
  SMART_CHARGER_PLATFORM_CHARGING_LED_OFF,
  SMART_CHARGER_PLATFORM_CHARGING_LED_ON,
  SMART_CHARGER_PLATFORM_CHARGING_LED_TOGGLE,
  SMART_CHARGER_PLATFORM_CHARGING_LED_INVALID
}SMART_CHARGER_PLATFORM_CHARGING_LED_CONFIG_TYPE;

typedef enum 
{
   SMART_CHARGER_PLATFORM_CHARGER_ATTACHED_NONE,    /**< No charger. */
   SMART_CHARGER_PLATFORM_CHARGER_ATTACHED_BATT,    /**< Battery. */
   SMART_CHARGER_PLATFORM_CHARGER_ATTACHED_USB,     /**< USB path. */
   SMART_CHARGER_PLATFORM_CHARGER_ATTACHED_DCIN,    /**< DCIN path. */
}SMART_CHARGER_PLATFORM_CHARGER_ATTACHED_TYPE;

typedef struct 
{
  BOOLEAN bChargerSrcPresent;
  UINT32  DispSignOfLifeMaxThresholdMv;
  SMART_CHARGER_BATT_STATUS_INFO BattStsInfo;
  CHARGERLIB_ATTACHED_CHGR_TYPE ChargerAttached;
  EFI_QCOM_CHARGER_BATT_PROFILE_STATE       ProfState;
  EFI_QCOM_CHARGER_CHARGING_LED_CONFIG_TYPE LedConfigType;
  BOOLEAN bPowerKeyPressed;

}SMART_CHARGER_PLATFORM_ACTION_INFO;

typedef struct
{
  /*Charger Log File Handled */
  VOID      *gChargerLogHandle;
  BOOLEAN    bPrintChargerAppDbgMsg;
  BOOLEAN    bPrintChargerAppDbgMsgToFile;
  UINT32     fileLogLevelMask;
}SMART_CHARGER_PLATFORM_LOG_INFO;

typedef enum
{
  SMART_CHARGER_PLATFORM_CHG_WDOG_DISABLE,
  SMART_CHARGER_PLATFORM_CHG_WDOG_DISABLE_ON_EXIT,
  SMART_CHARGER_PLATFORM_CHG_WDOG_LEAVE_ENABLED_ON_EXIT,
  SMART_CHARGER_PLATFORM_CHG_WDOG_INVALID
}SMART_CHARGER_PLATFORM_CHG_WDOG_CONFIG_TYPE;

/** @} */ /* end_addtogroup efi_qcom_platform_cfg_data_types */

/*===========================================================================
  FUNCTION DEFINITIONS
===========================================================================*/
EFI_STATUS SmartChargerPlatform_Init( VOID );

EFI_STATUS SmartChargerPlatform_GetChargingAction(EFI_QCOM_CHARGER_ACTION_TYPE *pActionType, SMART_CHARGER_PLATFORM_ACTION_INFO *pChargerActionInfo, BOOLEAN vbattChecking);

EFI_STATUS SmartChargerPlatform_TakeAction(EFI_QCOM_CHARGER_ACTION_TYPE ChargingAction, CONST SMART_CHARGER_PLATFORM_ACTION_INFO *pChargerActionInfo);

VOID SmartChargerPlatform_ReadCfgParams( UINT8* Section, UINT8* Key, UINT8* Value);

EFI_STATUS SmartChargerPlatform_GetConfigData(SMART_CHARGER_PLATFORM_CFGDATA_TYPE *pChargerPlatformCfgData);

EFI_STATUS SmartChargerPlatformFile_GetLogInfo(SMART_CHARGER_PLATFORM_LOG_INFO *pFileLogInfo);

EFI_STATUS SmartChargerPlatformFile_GetChargerConfig(EFI_QCOM_CHARGER_CONFIG_KEY ChargerCfgKey, UINT32 *Value);

#endif  /* __SMARTCHARGERPLATFORM_H__ */

