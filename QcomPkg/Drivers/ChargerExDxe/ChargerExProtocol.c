/*! @file ChargerExProtocol.c

*  CHARGER EXTENDED MODULE RELATED DECLARATION
*  This file contains functions and variable declarations to support
*  the Charger Extended module.
*
*  Copyright (c) 2017-2020 Qualcomm Technologies, Inc.  All Rights Reserved.
*  Qualcomm Technologies Inc Proprietary and Confidential.
*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.


when       who     what, where, why
--------   ---     ----------------------------------------------------------
04/26/20   ravi    Added support for power up with no battery
08/09/19   mr      Fix offmode charging implementation for SEC_MOR feature
05/24/19   mr      Added support for CLS Platform with no Battery for Fw update feature
04/25/19   mr      Added MTP Platform support for Fw update feature
03/11/19   mr      Added interface to get SOC and support Battery Fw update
01/16/19   cs      added config item for power testing
09/18/18   dc      Change Non-Error log level from Warning to Info
07/27/18   cs      Updated IsOffModeCharging API for mtp to not return
                    offmodecharging true when mtp does not support charging
06/18/18   sb      Updated IsOffModeCharging API for CLS
06/05/18   sb      added cable pon detection for off-mode charging
03/15/18   ra      Add missing field for IsPowerOk
02/09/18   ra      Hardcoding values for bring up
03/03/16   va      Adding offmode, flash APIs to contained for ABL, making
                    open source and local header copies
12/16/16   sm      Added changes to return EFI_UNSUPPORTED Status for
                    CDP and RUMI Platforms
11/09/16   cs      modified charger present API also to detect DCIn.
09/09/16   sm      Added Status success check after locating protocol.
05/31/16   sm      New File
===========================================================================*/

/*===========================================================================
                        INCLUDE FILES FOR MODULE
===========================================================================*/

#include <Library/UefiLib.h>
/**
  PMIC Lib interfaces
 */
#include <Include/Protocol/EFIQcomCharger.h>
#include <Include/Protocol/EFIPmicUsb.h>
#include <Include/Protocol/EFIChargerEx.h>
#include <Include/Protocol/EFIPmicPwrOn.h>
#include <Include/Protocol/EFIPmicSchg.h>

#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Protocol/EFIPlatformInfo.h>
#include <Library/QcomLib.h>

#include "smem.h"
#include "../Library/ChargerLib/target/1000/ChargerLibFG.h"

#include <api/pmic/pm/pm_chg.h>
#include <api/pmic/charger/ChargerLibCommon.h>
#include "../Library/ChargerLib/ChargerLibTarget.h"


/*===========================================================================
                        MACRO AND TYPE DEFINITIONS
===========================================================================*/
#define PM_DEVICE_0     0       /* Primary PMIC index */

#define PM_FAULT_SEQ        0x40
#define PM_WARMRESET_SEQ    0x40


/*===========================================================================
                        GLOBAL DATA DEFINITION
===========================================================================*/
extern EFI_GUID gQcomPmicUsbProtocolGuid;
EFI_CHARGER_EX_PROTOCOL ChargerExProtocolImplementation;

extern chargerlib_cfgdata_type      gChargerLibTargetCfgData;
extern chargerlib_cfgdata_type      gChargerLibCfgData;

STATIC EFI_QCOM_CHARGER_PROTOCOL    *pQcomChargerProtocol = NULL;
STATIC EFI_QCOM_PMIC_SCHG_PROTOCOL  *pPmicSchgProtocol = NULL;

STATIC BOOLEAN                      InPowerTestingMode    = FALSE;
STATIC UINT8                        gShipModeExit         = FALSE;

/*===========================================================================
                        INTERNAL FUNCTION DECLARATIONS
===========================================================================*/
EFI_STATUS ChargerExSaveChargeInfoToSmem(pm_chg_info_type *pChargerSharedInfo);


/*===========================================================================
                        EXTERNAL FUNCTION DECLARATIONS
===========================================================================*/
/**
ChargerExInitialize()

@brief
Initializes charger Extended module
*/
EFI_STATUS ChargerExInitialize( IN EFI_HANDLE         ImageHandle,
                                IN EFI_SYSTEM_TABLE   *SystemTable )
{
  EFI_STATUS Status = EFI_SUCCESS;

  Status = gBS->InstallMultipleProtocolInterfaces(
    &ImageHandle,
    &gChargerExProtocolGuid,
    &ChargerExProtocolImplementation,
    NULL
    );
  if(EFI_ERROR(Status))
  {
    DEBUG((EFI_D_ERROR, "ChargerExProtocol:: %a  Install Protocol failed, Error[%r] \r\n", __FUNCTION__, Status));
  }

  return Status;
}

/**
EFI_ChargerExGetChargerPresence()

@brief
 Returns charger presence status
 */
EFI_STATUS
EFIAPI
EFI_ChargerExGetChargerPresence
(
  OUT BOOLEAN  *pChargerPresent
)
{
  EFI_STATUS                           Status = EFI_SUCCESS;
  STATIC EFI_QCOM_PMIC_USB_PROTOCOL   *PmicUsbProtocol   = NULL;
  BOOLEAN                              DcInValid = FALSE;
  EFI_PLATFORMINFO_PLATFORM_TYPE       PlatformType;

  if (!pChargerPresent)
  {
    return EFI_INVALID_PARAMETER;
  }

  Status |= GetPlatformType(&PlatformType);
  if(EFI_SUCCESS != Status)
  {
    DEBUG((EFI_D_WARN, "ChargerExProtocol:: %a Error getting platform type  \r\n", __FUNCTION__));
    return EFI_DEVICE_ERROR;
  }

  if((EFI_PLATFORMINFO_TYPE_CDP == PlatformType) || (EFI_PLATFORMINFO_TYPE_RUMI == PlatformType)
    || (EFI_PLATFORMINFO_TYPE_CLS == PlatformType))
  {
    DEBUG(( EFI_D_WARN, "ChargerExProtocol:: %a CDP/RUMI/CLS (%d) Platform detected. No Battery information available. \r\n", __FUNCTION__, PlatformType));
    return EFI_UNSUPPORTED;
  }

  if (!PmicUsbProtocol)
  {
    Status = gBS->LocateProtocol( &gQcomPmicUsbProtocolGuid,
                                  NULL,
                                 (VOID **)&PmicUsbProtocol );

    if((EFI_SUCCESS != Status) || (NULL == PmicUsbProtocol))
    {
      return EFI_DEVICE_ERROR;
    }
  }

  Status = PmicUsbProtocol->GetChargerPresence(pChargerPresent);

  if (EFI_SUCCESS != Status)
  {
    return Status;
  }

  if (!pQcomChargerProtocol)
  {
    Status = gBS->LocateProtocol(&gQcomChargerProtocolGuid, NULL, (VOID **)&pQcomChargerProtocol);

    if ((EFI_SUCCESS != Status) || (NULL == pQcomChargerProtocol))
    {
      return EFI_DEVICE_ERROR;
    }
  }

  Status = pQcomChargerProtocol->IsDcInValid(&DcInValid);

  if (EFI_SUCCESS == Status)
  {
    (*pChargerPresent) |= DcInValid;
  }

  return Status;
}


/**
EFI_ChargerExGetBatteryPresence ()

@brief
Get battery presence status SCHG
*/
EFI_STATUS
EFIAPI
EFI_ChargerExGetBatteryPresence
(
  OUT BOOLEAN  *pBatteryPresent
)
{
  EFI_STATUS                      Status = EFI_SUCCESS;

  EFI_PLATFORMINFO_PLATFORM_TYPE  PlatformType;

  if (!pBatteryPresent)
  {
    return EFI_INVALID_PARAMETER;
  }

  Status |= GetPlatformType(&PlatformType);
  if(EFI_SUCCESS != Status)
  {
    DEBUG((EFI_D_WARN, "ChargerExProtocol:: %a Error getting platform type  \r\n", __FUNCTION__));
    return EFI_DEVICE_ERROR;
  }

  if((EFI_PLATFORMINFO_TYPE_CDP == PlatformType) || (EFI_PLATFORMINFO_TYPE_RUMI == PlatformType))
  {
    DEBUG(( EFI_D_WARN, "ChargerExProtocol:: %a CDP/RUMI (%d) Platform detected. No Battery information available. \r\n", __FUNCTION__, PlatformType));
    return EFI_UNSUPPORTED;
  }

  if(EFI_PLATFORMINFO_TYPE_MTP == PlatformType)
  {
    *pBatteryPresent = FALSE;
    return Status;
  }

  if(!pQcomChargerProtocol)
  {
    Status = gBS->LocateProtocol( &gQcomChargerProtocolGuid, NULL, (VOID **)&pQcomChargerProtocol );

    if((EFI_SUCCESS != Status) || (NULL == pQcomChargerProtocol))
    {
      return EFI_DEVICE_ERROR;
    }
  }

  Status = pQcomChargerProtocol->GetBatteryPresence(pBatteryPresent);

  return Status;
}

/**
EFI_ChargerExGetBatteryVoltage ()

@brief
Returns battery voltage
*/
EFI_STATUS
EFIAPI
EFI_ChargerExGetBatteryVoltage
(
  OUT UINT32  *pBatteryVoltage
)
{
  EFI_STATUS                      Status = EFI_SUCCESS;

  EFI_PLATFORMINFO_PLATFORM_TYPE  PlatformType;

  if (!pBatteryVoltage)
  {
    return EFI_INVALID_PARAMETER;
  }

  Status |= GetPlatformType(&PlatformType);
  if(EFI_SUCCESS != Status)
  {
    DEBUG((EFI_D_WARN, "ChargerExProtocol:: %a Error getting platform type  \r\n", __FUNCTION__));
    return EFI_DEVICE_ERROR;
  }

  if((EFI_PLATFORMINFO_TYPE_CDP == PlatformType) || (EFI_PLATFORMINFO_TYPE_RUMI == PlatformType)
    || (EFI_PLATFORMINFO_TYPE_CLS == PlatformType))
  {
    DEBUG(( EFI_D_WARN, "ChargerExProtocol:: %a CDP/RUMI/CLS (%d) Platform detected. No Battery information available. \r\n", __FUNCTION__, PlatformType));
    return EFI_UNSUPPORTED;
  }

  if(!pQcomChargerProtocol)
  {
    Status = gBS->LocateProtocol( &gQcomChargerProtocolGuid, NULL, (VOID **)&pQcomChargerProtocol );

    if(EFI_SUCCESS != Status || NULL == pQcomChargerProtocol)
    {
      return EFI_DEVICE_ERROR;
    }
  }

  Status = pQcomChargerProtocol->GetBatteryVoltage(pBatteryVoltage);

  return Status;
}

/**
EFI_ChargerExGetChargingStatus ()

@brief
Returns Charging status
*/
EFI_STATUS
EFIAPI
EFI_ChargerExGetChargingStatus
(
    OUT UINT32  *pBatterySOC,
    OUT BOOLEAN  *IsChargerAttached
)
{
    EFI_STATUS    Status = EFI_SUCCESS;
    EFI_PLATFORMINFO_PLATFORM_TYPE  PlatformType;

    if (!pBatterySOC && !IsChargerAttached)
    {
        return EFI_INVALID_PARAMETER;
    }

    Status = GetPlatformType(&PlatformType);
    if(EFI_ERROR(Status))
    {
        DEBUG((EFI_D_ERROR, "ChargerExProtocol:: %a GetPlatformType failed, Error[%r] \r\n", __FUNCTION__, Status));
        return Status;
    }

    if(!(EFI_PLATFORMINFO_TYPE_CLS == PlatformType || EFI_PLATFORMINFO_TYPE_MTP == PlatformType))
    {
        Status = EFI_UNSUPPORTED;
        DEBUG((EFI_D_WARN, "ChargerExProtocol:: %a  Non-CLS/MTP (%d) Platform detected, Error[%r] \r\n", __FUNCTION__, PlatformType, Status));
        return Status;
    }

    if (pBatterySOC)
    {
        if(EFI_PLATFORMINFO_TYPE_MTP == PlatformType)
        {
            *pBatterySOC = 0;
        }
        else
        {
            ChgBattType BatteryType = CHG_BATT_TYPE_INVALID;

            if(gChargerLibTargetCfgData.plat_hw_cfg_override == 0)
            {
                gChargerLibTargetCfgData.plat_hw_cfg_override = CHARGERLIB_PLATFORM_HW_CONFIG_INVALID;
                ChargerLibTarget_HWInit(&gChargerLibTargetCfgData, PlatformType);
            }

            if(!pQcomChargerProtocol)
            {
                Status  = gBS->LocateProtocol(&gQcomChargerProtocolGuid, NULL, (VOID **)&pQcomChargerProtocol);
                if(EFI_ERROR(Status) || !pQcomChargerProtocol)
                {
                    DEBUG((EFI_D_ERROR, "ChargerExProtocol:: %a  QcomChargerProtocol locate failed, Error[%r] \r\n", __FUNCTION__, Status));
                    return Status;
                }
            }

            Status = pQcomChargerProtocol->GetChargerConfig(EFI_QCOM_CHARGER_CONFIG_KEY_BATT_ID_INFO, (UINT32 *)&gChargerLibCfgData.fg_cfg_data.FgBattIdCfg);
            if(EFI_ERROR(Status))
            {
                DEBUG((EFI_D_ERROR, "ChargerExProtocol:: %a  GetChargerConfig failed, Error[%r] \r\n", __FUNCTION__, Status));
                return Status;
            }

            Status = ChargerLib_GetBatteryType(&BatteryType);
            if(EFI_ERROR(Status))
            {
                DEBUG((EFI_D_ERROR, "ChargerExProtocol:: %a  _GetBatteryType failed, Error[%r] \r\n", __FUNCTION__, Status));
                return Status;
            }

            if(BatteryType == CHG_BATT_TYPE_DEBUG_BOARD)
            {
                *pBatterySOC = 30;
            }
            else
            {
                SmartChargerFG_BattStatusInfo BatteryStatus = {0};

                //Get FG information from I2C
                Status = SmartChargerLibFG_GetBatteryStatus(&BatteryStatus);
                if(EFI_ERROR(Status))
                {
                    DEBUG((EFI_D_ERROR, "ChargerExProtocol:: %a  _GetBatteryStatus failed, Error[%r] \r\n", __FUNCTION__, Status));
                    return Status;
                }

                *pBatterySOC = BatteryStatus.StateOfCharge;
            }
        }
    }

    if (IsChargerAttached)
    {
        CHARGERLIB_ATTACHED_CHGR_TYPE ChargingPath = CHARGERLIB_ATTACHED_CHGR__NONE;

        Status = ChargerLib_GetChargingPath(&ChargingPath);
        if(EFI_ERROR(Status))
        {
            DEBUG((EFI_D_ERROR, "ChargerExProtocol:: %a  _GetChargingPath failed, Error[%r] \r\n", __FUNCTION__, Status));
            return Status;
        }

        if((CHARGERLIB_ATTACHED_CHGR__BATT == ChargingPath) || (CHARGERLIB_ATTACHED_CHGR__NONE == ChargingPath))
        {
            *IsChargerAttached = FALSE;
        }
        else
            *IsChargerAttached = TRUE;
    }

    return Status;
}

/**
EFI_ChargerExIsOffModeCharging ()

@brief
Returns if device needs to go for off mode charging
*/
EFI_STATUS
EFIAPI
EFI_ChargerExIsOffModeCharging
(
  OUT BOOLEAN *bOffModeCharging
)
{
  EFI_STATUS Status         = EFI_SUCCESS;

  BOOLEAN    ChargerPresent = FALSE;
  EFI_PM_PON_REASON_TYPE PonReasonType;
  BOOLEAN    IsColdBoot     = FALSE;
  BOOLEAN    BatteryPresent = FALSE;
  BOOLEAN    IsChargingSupported = FALSE;
  EFI_PLATFORMINFO_PLATFORM_TYPE  PlatformType;
  UINT32     configValue = 0;
  CHARGERLIB_ATTACHED_CHGR_TYPE   AttachedCharger = CHARGERLIB_ATTACHED_CHGR__NONE;
  CHARGERLIB_PLATFORM_CHGR_MAX_PWR_REQ_CFG_STS_TYPE MaxPwrReqCfgSts;

  UINT64 PonReasonsHist = 0;
  UINT8 *PonReasonHist = NULL;

  if(NULL == bOffModeCharging)
    return EFI_INVALID_PARAMETER;

  Status = GetPlatformType(&PlatformType);
  if(EFI_SUCCESS != Status)
  {
    DEBUG((EFI_D_WARN, "ChargerExProtocol:: %a Error getting platform type  \r\n", __FUNCTION__));
    return EFI_DEVICE_ERROR;
  }

  if (!pQcomChargerProtocol)
  {
    Status = gBS->LocateProtocol(&gQcomChargerProtocolGuid, NULL, (VOID **)&pQcomChargerProtocol);

    if ((EFI_SUCCESS != Status) || (NULL == pQcomChargerProtocol))
    {
      return EFI_DEVICE_ERROR;
    }
  }

  Status = pQcomChargerProtocol->GetChargerConfig(EFI_QCOM_CHARGER_CONFIG_KEY_OFFMODE_POWER_TESTING_MODE, &configValue);
  if (Status != EFI_SUCCESS)
  {
    InPowerTestingMode = FALSE;
  }
  else
  {
    InPowerTestingMode = (BOOLEAN)configValue;
  }

  if (((EFI_PLATFORMINFO_TYPE_CDP == PlatformType) && (!InPowerTestingMode)) || (EFI_PLATFORMINFO_TYPE_RUMI == PlatformType))
  {
    DEBUG(( EFI_D_WARN, "ChargerExProtocol:: %a CDP/RUMI (%d) Platform detected. No Battery information available. \r\n", __FUNCTION__, PlatformType));
    return EFI_UNSUPPORTED;
  }

  Status = pQcomChargerProtocol->GetChargerConfig(EFI_QCOM_CHARGER_CONFIG_SHIP_MODE_EXIT, &configValue);
  if (Status != EFI_SUCCESS)
  {
    gShipModeExit = SHIP_MODE_NONE;
  }
  else
  {
    gShipModeExit = (UINT8)configValue;
    DEBUG(( EFI_D_WARN, "ChargerExProtocol:: %a ShipModeExit = %d. \r\n", __FUNCTION__, gShipModeExit));
  }

  Status = pQcomChargerProtocol->IsChargingSupported(&IsChargingSupported);
  if (EFI_SUCCESS != Status)
  {
    return EFI_DEVICE_ERROR;
  }
  else if (!IsChargingSupported)
  {
    *bOffModeCharging = FALSE;
    DEBUG((EFI_D_WARN, "ChargerExProtocol:: %a  Offmode Charging = %a \r\n", __FUNCTION__, (*bOffModeCharging) ? "TRUE" : "FALSE"));
    return Status;
  }

  if(!pPmicSchgProtocol)
  {
    Status = gBS->LocateProtocol(&gQcomPmicSchgProtocolGuid, NULL, (VOID **)&pPmicSchgProtocol);
    if(EFI_ERROR(Status) || !pPmicSchgProtocol)
    {
      DEBUG((EFI_D_ERROR, "ChargerExProtocol:: %a  PmicSchgProtocol locate failed, Error[%r] \r\n", __FUNCTION__, Status));
      return Status;
    }
  }

  Status = pPmicSchgProtocol->GetValidPonReasons(&PonReasonsHist);
  if(Status != EFI_SUCCESS)
  {
    DEBUG((EFI_D_ERROR, "ChargerExProtocol:: %a  GetValidPonReasons failed, Error[%r] \r\n", __FUNCTION__, Status));
    return EFI_DEVICE_ERROR;
  }
  PonReasonHist = (UINT8 *)&PonReasonsHist;

  DEBUG((EFI_D_WARN, "ChargerExProtocol:: %a  (Register) PON = 0x%02X, ON = 0x%02X, OFF = 0x%02X \r\n", __FUNCTION__,
            PonReasonHist[EFI_PM_VALID_PON_REASON_PON_REASON1],
            PonReasonHist[EFI_PM_VALID_PON_REASON_ON_REASON],
            PonReasonHist[EFI_PM_VALID_PON_REASON_OFF_REASON]));

  if(PonReasonHist[EFI_PM_VALID_PON_REASON_OFF_REASON] == PM_FAULT_SEQ)
  {
    *bOffModeCharging = FALSE;
    DEBUG((EFI_D_WARN, "ChargerExProtocol:: %a  PMIC FAULT - Offmode Charging = %a \r\n", __FUNCTION__, (*bOffModeCharging) ? "TRUE" : "FALSE"));
    return Status;
  }

  /* Charge Source Detection */
  Status = ChargerLib_GetChargingPath(&AttachedCharger);
  if (Status != EFI_SUCCESS)
  {
    CHARGER_DEBUG((EFI_D_ERROR, "ChargerExProtocol:: %a Error getting charging path = %r \r\n", __FUNCTION__, Status));
    return Status;
  }

  *(UINT32 *)&PonReasonType = PonReasonHist[EFI_PM_VALID_PON_REASON_PON_REASON1];

  if (EFI_PLATFORMINFO_TYPE_CLS == PlatformType || InPowerTestingMode)
  {
    if ((!gShipModeExit && (PonReasonType.KPDPWR || PonReasonType.HARD_RESET || PonReasonType.SMPL))
      || ( gShipModeExit == SHIP_MODE_PMIC_EXIT && (AttachedCharger == CHARGERLIB_ATTACHED_CHGR__NONE || AttachedCharger == CHARGERLIB_ATTACHED_CHGR__BATT)))
    {
      *bOffModeCharging = FALSE;
    }
    else
    {
      *bOffModeCharging = TRUE;
    }

    // No battery handling : force the off mode charging based strong charger power negotiation.
	MaxPwrReqCfgSts.AsUint8 = 0;
    Status = ChargerLib_GetPlatformMaxPwrReqCfgSts(&MaxPwrReqCfgSts);
    if (Status == EFI_SUCCESS && MaxPwrReqCfgSts.batt_missing)
    {
	  if (CHGRPD_MAX_PWR_REQ_DONE == MaxPwrReqCfgSts.status)
	  {
	  	*bOffModeCharging = FALSE;
	  }
	  else
	  {
	    *bOffModeCharging = TRUE;
	  }
      DEBUG(( EFI_D_INFO, "ChargerExProtocol:: %a  Battery missing MaxPwrReqSts = 0x%x\r\n", __FUNCTION__, MaxPwrReqCfgSts.status));
    }
  }
  else
  {
    /* Get Battery Presence, Charger Presence, Voltage */
    Status  =  EFI_ChargerExGetBatteryPresence(&BatteryPresent);
    Status |= EFI_ChargerExGetChargerPresence(&ChargerPresent);

    IsColdBoot = !(PonReasonHist[EFI_PM_VALID_PON_REASON_ON_REASON] & PM_WARMRESET_SEQ);

    if(Status == EFI_SUCCESS)
    {
      if((IsColdBoot)
        && (BatteryPresent)
        && (ChargerPresent)                   /* If not warm reset and charger,battery present */
        && (!PonReasonType.HARD_RESET)
        && (!PonReasonType.KPDPWR) /* If not Hard reset and not a Powery key boot */
        && (PonReasonType.PON1 || PonReasonType.USB_CHG || PonReasonType.CBLPWR))       /* if PON1 or USB triggered PON or cable PON
                                                                                         * SYS_OK signal is now routed to CBLPWR instead of PON1 due to hardware bug */
      {
        *bOffModeCharging = TRUE;
      }
      else
      {
        DEBUG(( EFI_D_INFO, "ChargerExProtocol:: %a  IsColdBoot = %d, BatteryPresent = %d, ChargerPresent = %d \r\n", __FUNCTION__, IsColdBoot, BatteryPresent, ChargerPresent));
       *bOffModeCharging = FALSE;
      }
    }
  }

  DEBUG((EFI_D_WARN, "ChargerExProtocol:: %a  Offmode Charging = %a \r\n", __FUNCTION__, (*bOffModeCharging) ? "TRUE" : "FALSE"));

  return Status;
}

/**
EFI_ChargerExIsPowerOk ()

@brief
Returns if battery voltage is good to process with SW flash
*/
EFI_STATUS
EFIAPI
EFI_ChargerExIsPowerOk
(
  IN  EFI_CHARGER_EX_POWER_TYPE   PowerType,
  OUT VOID                       *pPowerTypeInfo
)
{
  EFI_STATUS Status                = EFI_SUCCESS;
  EFI_CHARGER_EX_FLASH_INFO *pFlashInfo = NULL;
  UINT32     SwFlashBattMinVoltage = 0;
  UINT32     BatteryCurrentVoltage = 0;
  BOOLEAN    BatteryPresent        = FALSE;
  BOOLEAN    ChargerPresent        = FALSE;
  EFI_PLATFORMINFO_PLATFORM_TYPE       PlatformType;

  if(!pPowerTypeInfo)
    return EFI_INVALID_PARAMETER;

  pFlashInfo = (EFI_CHARGER_EX_FLASH_INFO *)pPowerTypeInfo;

  Status |= GetPlatformType(&PlatformType);
  if(EFI_SUCCESS != Status)
  {
    DEBUG((EFI_D_WARN, "ChargerExProtocol:: %a Error getting platform type  \r\n", __FUNCTION__));
    return EFI_DEVICE_ERROR;
  }

  if((EFI_PLATFORMINFO_TYPE_CDP == PlatformType) || (EFI_PLATFORMINFO_TYPE_RUMI == PlatformType))
  {
    DEBUG(( EFI_D_WARN, "ChargerExProtocol:: %a CDP/RUMI (%d) Platform detected. No Battery information available. \r\n", __FUNCTION__, PlatformType));
    return EFI_UNSUPPORTED;
  }
  else if(EFI_PLATFORMINFO_TYPE_CLS == PlatformType)
  {
    pFlashInfo = (EFI_CHARGER_EX_FLASH_INFO *)pPowerTypeInfo;
    pFlashInfo->bCanFlash = TRUE;
    return EFI_SUCCESS;
  }

  switch(PowerType)
  {
    case EFI_CHARGER_EX_POWER_FLASH_BATTERY_VOLTAGE_TYPE:
      pFlashInfo = (EFI_CHARGER_EX_FLASH_INFO *)pPowerTypeInfo;
      /* Get Battery Presence, Charger Presence, Voltage */
      Status =  EFI_ChargerExGetBatteryPresence(&BatteryPresent);
      Status |= EFI_ChargerExGetChargerPresence(&ChargerPresent);
      Status |= EFI_ChargerExGetBatteryVoltage(&BatteryCurrentVoltage);

      if(EFI_SUCCESS == Status)
      {
        pFlashInfo->BattCurrVoltage = BatteryCurrentVoltage;
      }

      if(!pQcomChargerProtocol)
      {
        Status |= gBS->LocateProtocol( &gQcomChargerProtocolGuid, NULL, (VOID **)&pQcomChargerProtocol );
        if(EFI_SUCCESS != Status || NULL == pQcomChargerProtocol)
        {
          return EFI_DEVICE_ERROR;
        }
      }

      Status |= pQcomChargerProtocol->GetChargerConfig(EFI_QCOM_CHARGER_CONFIG_KEY_SW_FLASH_VOLTAGE, &SwFlashBattMinVoltage);

      if(Status == EFI_SUCCESS)
      {
        pFlashInfo->BattRequiredVoltage = SwFlashBattMinVoltage;
        /* If battery not present but still device boot up have debug board */
        if(!BatteryPresent || (BatteryPresent  && (BatteryCurrentVoltage > SwFlashBattMinVoltage)))
        {
          pFlashInfo->bCanFlash = TRUE;
        }
        else
        {
          DEBUG(( EFI_D_INFO, "ChargerExProtocol:: %a SwFlashBattMinVoltage = %d mV\r\n", __FUNCTION__,SwFlashBattMinVoltage));
          pFlashInfo->bCanFlash = FALSE;
        }
      }
    break;
    default:
    break;
  }

  return Status;
}

/**
EFI_ChargerExSetFirmwareUpdateMode ()

@brief
Process FW Update action
*/
EFI_STATUS
EFIAPI
EFI_ChargerExSetFirmwareUpdateMode
(
  IN EFI_CHARGER_EX_FW_UPDATE_MODE_TYPE  FwUpdateMode
)
{
    EFI_STATUS    Status = EFI_SUCCESS;
    UINT32 ChargerFWEn = 0, FwUpdateEnable = 0;
    EFI_PLATFORMINFO_PLATFORM_TYPE  PlatformType;

    Status = GetPlatformType(&PlatformType);
    if(EFI_ERROR(Status))
    {
        DEBUG((EFI_D_ERROR, "ChargerExProtocol:: %a GetPlatformType failed, Error[%r] \r\n", __FUNCTION__, Status));
        return Status;
    }

    if(!(EFI_PLATFORMINFO_TYPE_CLS == PlatformType || EFI_PLATFORMINFO_TYPE_MTP == PlatformType))
    {
        Status = EFI_UNSUPPORTED;
        DEBUG((EFI_D_WARN, "ChargerExProtocol:: %a  Non-CLS/MTP (%d) Platform detected, Error[%r] \r\n", __FUNCTION__, PlatformType, Status));
        return Status;
    }

    if(!pQcomChargerProtocol)
    {
        Status  = gBS->LocateProtocol(&gQcomChargerProtocolGuid, NULL, (VOID **)&pQcomChargerProtocol);
        if(EFI_ERROR(Status) || !pQcomChargerProtocol)
        {
            DEBUG((EFI_D_ERROR, "ChargerExProtocol:: %a  QcomChargerProtocol locate failed, Error[%r] \r\n", __FUNCTION__, Status));
            return Status;
        }
    }

    Status = pQcomChargerProtocol->GetChargerConfig(EFI_QCOM_CHARGER_CONFIG_KEY_CHARGER_FW_INFO, &ChargerFWEn);
    if(EFI_ERROR(Status))
    {
        DEBUG((EFI_D_ERROR, "ChargerExProtocol:: %a  GetChargerConfig failed, Error[%r] \r\n", __FUNCTION__, Status));
        return Status;
    }

    if(FALSE == ChargerFWEn)
    {
        Status = EFI_UNSUPPORTED;
        DEBUG((EFI_D_ERROR, "ChargerExProtocol:: %a  ChargerFW Enable = %d, Error[%r] \r\n", __FUNCTION__, ChargerFWEn, Status));
        return Status;
    }
    // Set enable_charger_fw for ChargerLib to support loading ChargerFW (ADSP ChargerPD)
    gChargerLibTargetCfgData.enable_charger_fw = ChargerFWEn;

    Status = pQcomChargerProtocol->GetChargerConfig(EFI_QCOM_CHARGER_CONFIG_KEY_FW_UPDATE_SUPPORT, &FwUpdateEnable);
    if(EFI_ERROR(Status))
    {
        DEBUG((EFI_D_ERROR, "ChargerExProtocol:: %a  GetChargerConfig failed, Error[%r] \r\n", __FUNCTION__, Status));
        return Status;
    }

    DEBUG((EFI_D_WARN, "ChargerExProtocol:: %a  FwUpdateEnable = %d, FwUpdateMode = %d\r\n", __FUNCTION__, FwUpdateEnable, FwUpdateMode));

    if(FALSE == FwUpdateEnable)
    {
        Status = EFI_UNSUPPORTED;
        DEBUG((EFI_D_ERROR, "ChargerExProtocol:: %a  FwUpdateEnable = %d, Error[%r] \r\n", __FUNCTION__, FwUpdateEnable, Status));
        return Status;
    }

    if(!pPmicSchgProtocol)
    {
      Status = gBS->LocateProtocol(&gQcomPmicSchgProtocolGuid, NULL, (VOID **)&pPmicSchgProtocol);
      if(EFI_ERROR(Status) || !pPmicSchgProtocol)
      {
        DEBUG((EFI_D_ERROR, "ChargerExProtocol:: %a  PmicSchgProtocol locate failed, Error[%r] \r\n", __FUNCTION__, Status));
        return Status;
      }
    }

    switch(FwUpdateMode)
    {
        case EFI_CHARGER_EX_FW_UPDATE_MODE_ENTER:
        {
            pm_chg_info_type ChargerSharedInfo = {0};

            // Required for ChargerLib debug-messages
            gChargerLibCfgData.dbg_cfg_Data.print_charger_app_dbg_msg = TRUE;

            ChargerSharedInfo.uefi_charger_fw_mode = 1 /* CHARGER_FW_MODE */;
            Status = ChargerExSaveChargeInfoToSmem(&ChargerSharedInfo);
            if(EFI_ERROR(Status))
            {
                DEBUG((EFI_D_ERROR, "ChargerExProtocol:: %a  _SaveChargeInfoToSmem failed, Error[%r] \r\n", __FUNCTION__, Status));
                return Status;
            }

            Status = pPmicSchgProtocol->SetTestModeDischarging(TRUE);
            if(EFI_ERROR(Status))
            {
                DEBUG((EFI_D_ERROR, "ChargerExProtocol:: %a  SetTestModeDischarging failed, Error[%r] \r\n", __FUNCTION__, Status));
                return Status;
            }

            Status = ChargerLib_LoadChargerFW();
            if(EFI_ERROR(Status))
            {
                DEBUG((EFI_D_ERROR, "ChargerExProtocol:: %a  _LoadChargerFW failed, Error[%r] \r\n", __FUNCTION__, Status));
                return Status;
            }
        }
        break;

        case EFI_CHARGER_EX_FW_UPDATE_MODE_EXIT:
        {
            Status = pPmicSchgProtocol->SetTestModeDischarging(FALSE);
            if(EFI_ERROR(Status))
            {
                DEBUG((EFI_D_ERROR, "ChargerExProtocol:: %a  SetTestModeDischarging failed, Error[%r] \r\n", __FUNCTION__, Status));
                return Status;
            }

            gChargerLibCfgData.dbg_cfg_Data.print_charger_app_dbg_msg = FALSE;
        }
        break;

        default:
        {
            Status = EFI_UNSUPPORTED;
            DEBUG((EFI_D_ERROR, "ChargerExProtocol:: %a  Invalid FwUpdateMode [%d], Error[%r] \r\n", __FUNCTION__, FwUpdateMode, Status));
        }
    }

    return Status;
}


/**
Charger External UEFI Protocol implementation
*/
EFI_CHARGER_EX_PROTOCOL ChargerExProtocolImplementation =
{
    CHARGER_EX_REVISION,
    EFI_ChargerExGetChargerPresence,
    EFI_ChargerExGetBatteryPresence,
    EFI_ChargerExGetBatteryVoltage,
    EFI_ChargerExIsOffModeCharging,
    EFI_ChargerExIsPowerOk,
    EFI_ChargerExGetChargingStatus,
    EFI_ChargerExSetFirmwareUpdateMode
};


/*===========================================================================
                        INTERNAL FUNCTION DEFINITIONS
===========================================================================*/
EFI_STATUS ChargerExSaveChargeInfoToSmem(pm_chg_info_type *pChargerSharedInfo)
{
    EFI_STATUS    Status = EFI_SUCCESS;
    UINT32     buf_size = sizeof(pm_chg_info_type);
    UINT32     buf_size_ret = 0;
    pm_chg_info_type *pSmemChargerInfo = NULL;

    pSmemChargerInfo = (pm_chg_info_type *) smem_get_addr(SMEM_CHARGER_BATTERY_INFO, &buf_size_ret);
    if(!pSmemChargerInfo)
    {
        DEBUG((EFI_D_WARN, "ChargerExProtocol:: %a  SMEM Retrieve Error.. XBL Core allocating SMEM now \r\n",__FUNCTION__));

        pSmemChargerInfo = (pm_chg_info_type *)smem_alloc(SMEM_CHARGER_BATTERY_INFO, buf_size);
        if(!pSmemChargerInfo)
        {
            Status = EFI_DEVICE_ERROR;
            DEBUG((EFI_D_ERROR, "ChargerExProtocol:: %a  PmicSchgProtocol locate failed, Error[%r] \r\n", __FUNCTION__, Status));
            return Status;
        }

        DEBUG((EFI_D_WARN, "ChargerExProtocol:: %a  SMEM CHARGER_BATTERY_INFO allocated\r\n", __FUNCTION__));

        /* Initialized to -1 since SBL did not allocate smem yet */
        pSmemChargerInfo->version       = 1;
        pSmemChargerInfo->sbl_entry_mV  = -1;
        pSmemChargerInfo->sbl_entry_soc = -1;
    }

    /* Assign required values to smem memory */
    pSmemChargerInfo->uefi_entry_mV  = pChargerSharedInfo->uefi_entry_mV;
    pSmemChargerInfo->uefi_exit_mV   = pChargerSharedInfo->uefi_exit_mV;
    pSmemChargerInfo->uefi_entry_soc = pChargerSharedInfo->uefi_entry_soc;
    pSmemChargerInfo->uefi_exit_soc  = pChargerSharedInfo->uefi_exit_soc;
    pSmemChargerInfo->uefi_charger_fw_mode = pChargerSharedInfo->uefi_charger_fw_mode;

    return Status;
}
