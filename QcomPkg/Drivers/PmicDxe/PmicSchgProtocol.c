/*! @file PmicSchgProtocol.c 

*  PMIC- SCHG MODULE RELATED DECLARATION
*  This file contains functions and variable declarations to support 
*  the PMIC SCHG (Switch Mode Battery Charger) module.
*
*  Copyright (c) 2017 - 2020 Qualcomm Technologies, Inc.  All Rights Reserved.
*  Qualcomm Technologies Inc Proprietary and Confidential.
*/

/*===========================================================================

EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.


when       who     what, where, why
--------   ---     --------------------------------------------------------------------------
04/24/20   ravi    Added support for max power negotiation
12/13/18   ivy     get/set sdam with ship mode info
10/31/18   cs      only modify bit 0 to save disableBattery flag.
08/07/18   dc      Return SDP when BC1.2 detection is disabled in HW
08/01/18   ivy     Set HW AFP/Jeita in the same API
07/27/18   cs      for NON-SMB2351, return unsupported for getActivePort
06/18/18   sb      Set ICL to 500 mA while APSD is not finished for wall chargers
06/21/18   cs      added API to get active charging port.
05/17/18   ivy     Added support for DC IRQ status
05/14/17   ra      Support for Debug Accessory Mode
03/26/18   ra      Support for HW JEITA status
03/15/18   ra      Remove charger default setting
03/05/18   pxm     Update due to pm_schg_usb driver change
02/23/18   cs      remove checking for chip revision before enabling charger watchdog
02/20/18   ra      Adding new protocols
02/09/18   ra      Removing Wipower related protocol
01/24/18   ra      Adding support for icl status
11/27/17   ra      Support for battery missing detection and AFP
11/09/17   ra      Update JEITA code, remove OTG and Legacy cable detection WA.
09/19/17   cs      for SDM855 remvoe unsupported function calls.
06/13/17   ll      Enable/disable HW AFP based on debug board detection
03/21/17   dch     Initialize ICL for all charger as 500mA and increase it to 1500mA in case charger is not SDP/Float charger
02/15/17   va      Add re run APSD 
02/08/17   ll      Added OTG sequence for OTG device with large cap
02/06/17   cs      add config to enable/disable INOV
01/27/17   ai      Fix use of retail
01/27/17   sm      battery missing is true either latched or RT STATUS is TRUE.
                   Add support for PBS triggered AFP
                   Fixed battery missing not detected after battery missing shutdown and reboot
01/23/17   sm      CFG parameter Structure changes
12/19/16   sm      Added API to get SCHG Charger PmicIndex
12/09/16   sm      Added changes to set OTG current limit when enabling/disabling OTG
11/14/16   sm      Added required changes for preventing accidental reverse boosting in EFI_PmicSchgEnableCharger() and EFI_PmicSchgExit()
11/09/16   sm      Added changes to read latched status of PM_SCHG_MISC_IRQ_WDOG_BARK interrupt
11/04/16   sm      Added APSD done status check in EFI_PmicSchgGetChargerPortType()
10/19/16   sm      Added changes to read latched status of BAT_THERM_OR_ID_MISSING interrupt
                   Removed HVDCP configuration settings
10/17/16   sm      Added SetChargeCmdBit() API
                   Added changes to report charger wdog not supported for PMI8998 versions before v2.0
                   Added changes in EFI_PmicSchgGetChgWdogStatus() to check if disabling charging on 
                   wdog bite functionality is enabled and return status accordingly.
10/13/16   cs      add protocol API for setting dcin current limit.
09/20/16   sv      fixing klocwork issues
09/13/16   sm      Removed host mode setting from init()
09/06/16   cs      wipower changes to remove 3.5 sec, not suspend DCIn
08/24/16   sm      Added function call to set FV_MAX during Init()
08/16/16   sm      Added API to enable/disable HW JEITA and made EFI_PmicSchgEnableJeita() obsolete
07/18/16   sm      Added API to configure USB ICL options
07/15/16   cs      added wipower support
06/24/16   sm      Added API to return if charging is enabled or not. 
06/23/16   sm      Added changes to enable/disable HVDCP
06/23/16   va      Adding support for Charger Fg Peripheral dumps
06/09/16   sm      Added check for hostmode support and then resetting port role 
                   Redefined GetOtgStatus API
05/25/16   sm      Added pm_schg_chgr_set_charge_current() in Init()
                   Added  pm_schg_usb_enable_icl_override_after_apsd() in SetUSBMaxCurrent()
05/10/16   sm      Added back SchgExit for placeholder
04/21/16   sm      Added APIs to enable/disable and pet watchdog
04/07/16   sm      Completing API definitions
03/28/16   sm      Few IRQ corrections
03/28/16   va      Adding HW Jeita support, exit API
02/19/16   sm      Initial draft
===========================================================================*/

/*===========================================================================

INCLUDE FILES FOR MODULE

===========================================================================*/

#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/QcomTargetLib.h>
#include <Library/FuseControlLib.h>

#include "pm_uefi.h"
#include "pm_version.h"
#include "pm_schg_usb.h"
#include "pm_schg_typec.h"
#include "pm_schg_dc.h"
#include "pm_schg_misc.h"
#include "pm_schg_batif.h"
#include "pm_schg_chgr.h"
#include "pm_schg_dcdc.h"
#include "pm_app_chgr_common.h"
#include "pm_bmd.h"
#include "pm_schg_bob.h"
#include "pm_sdam.h"

#include <Protocol/EFIPmicSchg.h>
#include <Library/DebugLib.h>
#include <api/dal/DALSys.h>
#include "PmicFileLog.h"
/**
  PMIC Lib interfaces
 */
#include "pm_uefi.h"
#include "pm_core_utils.h"

#define MAX_CHARGING_PORT   4

EFI_PM_SCHG_CFGDATA_TYPE gSchgCfgData = {0}; 

STATIC UINT8  gSchgPmicIndex[MAX_CHARGING_PORT] = { 0 };
STATIC UINT8  gSchgSlaveIndex[MAX_CHARGING_PORT] = { 0 };
STATIC UINT8  gChargerCount = 0;
STATIC boolean gbDbgBoard = 0;
/*Charger dump */
#define PMIC_SCHG_PERIPHERAL_START_ADDR      0x1000
#define PMIC_SCHG_MISC_PERIPHERAL_START_ADDR 0x1600
#define PMIC_SCHG_PERIPHERAL_NUM        5
#define PM_DC_ZIN_ICL_PT_VOLT_MV         8000
#define PM_DC_ZIN_ICL_PT_HV_VOLT_MV      9000
#define PM_DC_ZIN_ICL_LV_VOLT_MV         5500
#define PM_DC_ZIN_ICL_MID_LV_VOLT_MV     6500
#define PM_DC_ZIN_ICL_MID_HV_VOLT_MV     8000
#define PM_DC_ZIN_ICL_HV_VOLT_MV         11000

#define PMIC_ALL_LAYER_REV_2 2
#define PMIC_METAL_REV_0 0

#define PMIC_SCHG_MAX_APSD_DONE_CHECK_ITERATION 5
#define PM_SCHG_200_MS_WAIT                     200000 //200 milli Second wait
#define PM_SCHG_2_MS_WAIT                       2000 //2 milli Second wait

#define PM_SCHG_OTG_CURRENT_LIMIT_1500_MA           1500
#define PM_SCHG_OTG_CURRENT_LIMIT_250_MA            250
#define PM_SCHG_OTG_WA_FPFET_SS                     0x3
#define PM_FORCE_USBICL_SDP_FLOAT_DEFAULT_IN_MA     500

#define SDAM_MEM_036_ADDR    36
#define SDAM_MEM_037_ADDR    37
#define SDAM_MEM_053_ADDR    53
#define SDAM_MEM_055_ADDR    55
#define SDAM_MEM_056_ADDR    56
#define SDAM_MEM_057_ADDR    57
#define SDAM_MEM_058_ADDR    58
#define SDAM_MEM_059_ADDR    59
#define SDAM_MEM_040_ADDR    40
#define SDAM_MEM_048_ADDR    48
#define SDAM_MEM_061_ADDR    61
#define TZ_RESET_DONE        0xA5

#define BTMTHR_COEFF0        1156
#define BTMTHR_COEFF1        75
#define BTMTHR_COEFF2        6

EFI_STATUS PmicSchgDumpPeripheral( VOID );
EFI_STATUS PmicSchgSetHwAfpThresholds(UINT32 PmicDeviceIndex, INT32 HotThreshold, INT32 ColdThreshold);
EFI_STATUS PmicSchgSetDieTempThresholds(UINT32 PmicDeviceIndex, INT32 HotThreshold, INT32 TooHotThreshold);
EFI_STATUS PmicSchgSetThermRegSrcCfg(UINT32 PmicDeviceIndex, BOOLEAN SkinEnable, BOOLEAN DieEnable, BOOLEAN ComparatorEnable);

/*===========================================================================
EXTERNAL FUNCTION DECLARATIONS
===========================================================================*/

/**
EFI_PmicSchgDumpPeripheral()

@brief
 Dump SCHG Peripheral
 */
EFI_STATUS
EFIAPI
EFI_PmicSchgDumpPeripheral( VOID )
{
  EFI_STATUS              Status         = EFI_SUCCESS;

  if(!RETAIL)
  {
    Status = PmicSchgDumpPeripheral();
  }
  else
  {
    return EFI_UNSUPPORTED;
  }
  return Status;
}


/**
EFI_PmicSchgInit ()

@brief
Initializes SCHG
*/
EFI_STATUS
EFIAPI
EFI_PmicSchgInit
(
  IN UINT32 PmicDeviceIndex,
  IN EFI_PM_SCHG_CFGDATA_TYPE SchgCfgData,
  IN BOOLEAN bDbgBoard
)
{
  EFI_STATUS Status = EFI_SUCCESS;
  pm_err_flag_type errFlag = PM_ERR_FLAG__SUCCESS;
  
  DALSYS_memscpy(&gSchgCfgData, sizeof(EFI_PM_SCHG_CFGDATA_TYPE), &SchgCfgData, sizeof (EFI_PM_SCHG_CFGDATA_TYPE));
  
  /* enable interrupt for usb re-insertion  */
  errFlag |= pm_schg_usb_irq_set_trigger(PmicDeviceIndex, PM_SCHG_USB_IRQ_USBIN_PLUGIN, PM_IRQ_TRIGGER_ACTIVE_LOW);
  errFlag |= pm_schg_usb_irq_enable(PmicDeviceIndex, PM_SCHG_USB_IRQ_USBIN_PLUGIN, TRUE);

  /* enable interrupt for batt therm and batt id missing detection  */
  errFlag |= pm_schg_batif_irq_set_trigger(PmicDeviceIndex, PM_SCHG_BATIF_BAT_THERM_OR_ID_MISSING, PM_IRQ_TRIGGER_ACTIVE_LOW);
  errFlag |= pm_schg_batif_irq_enable(PmicDeviceIndex, PM_SCHG_BATIF_BAT_THERM_OR_ID_MISSING, TRUE);
  
  /* enable interrupt for charger watchdog bark/bite interrupt */
  errFlag |= pm_schg_misc_irq_set_trigger(PmicDeviceIndex, PM_SCHG_MISC_IRQ_WDOG_BARK, PM_IRQ_TRIGGER_RISING_EDGE);
  errFlag |= pm_schg_misc_irq_enable(PmicDeviceIndex, PM_SCHG_MISC_IRQ_WDOG_BARK, TRUE);

  /*Enable battery missing detection using BATT ID and BATT THERM PIN */
  errFlag |= pm_schg_batif_cfg_batt_missing_src(PmicDeviceIndex , PM_SCHG_BAT_MISS_SRC_CFG__PIN_SRC_EN, TRUE);

  /* Battery Missing Config */
  errFlag |= pm_bmd_src_ctl(PmicDeviceIndex, (pm_bmd_src_ctl_sel_type)SchgCfgData.BattMissingCfg);

  errFlag |= pm_schg_chgr_set_float_volt(PmicDeviceIndex, SchgCfgData.ChgFvMax);
  errFlag |= pm_schg_chgr_set_charge_current(PmicDeviceIndex, PM_SCHG_CHGR_FAST_CHARGE_CURRENT, SchgCfgData.ChgFccMax);

  /* Use APSD result to control Input Current Limit */
  errFlag |= pm_schg_usb_enable_icl_override_after_apsd(PmicDeviceIndex, FALSE);

  if(gSchgCfgData.SchgDbgCfg.EnableChargerFgDump)
  {
    EFI_PmicSchgDumpPeripheral();
  }

  /* Save debug board info */
  gbDbgBoard = bDbgBoard;

  Status |= PmicSchgSetThermRegSrcCfg(PmicDeviceIndex, SchgCfgData.EnableSkinTempINov, SchgCfgData.EnableDieTempINov, SchgCfgData.EnableDieTempCompINov);
  if (SchgCfgData.SchgSkinDieTemp.ProgramSkinAndChargerHotThreshold)
  {
	  /* Set the die temp theshold */
	  Status |= PmicSchgSetDieTempThresholds(PmicDeviceIndex, SchgCfgData.SchgSkinDieTemp.ChargerHotInC, SchgCfgData.SchgSkinDieTemp.ChargerTooHotInC);
  }

  return Status;
}

/*
@brief
De-Initializes SCHG
*/
EFI_STATUS
EFIAPI
EFI_PmicSchgExit
(
  IN UINT32 PmicDeviceIndex
)
{
  EFI_STATUS Status = EFI_SUCCESS;
  pm_err_flag_type errFlag = PM_ERR_FLAG__SUCCESS;

  /* TBD: placeholder for de init interrupt, cancel timers etc..*/
  errFlag |= pm_schg_usb_irq_enable(PmicDeviceIndex, PM_SCHG_USB_IRQ_USBIN_PLUGIN, FALSE);
  /*clear the interrupt*/
  errFlag |= pm_schg_usb_irq_clear(PmicDeviceIndex, PM_SCHG_USB_IRQ_USBIN_PLUGIN);
  errFlag |= pm_schg_batif_irq_enable(PmicDeviceIndex, PM_SCHG_BATIF_BAT_THERM_OR_ID_MISSING, FALSE);
  errFlag |= pm_schg_misc_irq_enable(PmicDeviceIndex, PM_SCHG_MISC_IRQ_WDOG_BARK, FALSE);

  /*clear the interrupt*/
  errFlag |= pm_schg_usb_irq_clear(PmicDeviceIndex, PM_SCHG_USB_IRQ_USBIN_PLUGIN);
  errFlag |= pm_schg_batif_irq_clear(PmicDeviceIndex, PM_SCHG_BATIF_BAT_THERM_OR_ID_MISSING);
  errFlag |= pm_schg_misc_irq_clear(PmicDeviceIndex, PM_SCHG_MISC_IRQ_WDOG_BARK);

  /* sdm855 todo need to determine if it is needed for sdm855.  PMI8998 charger prevention of accidental reverse boosting, Enable 1-in-8 mode before exiting UEFI*/
  //errFlag |= pm_schg_misc_set_buckboost_refresh_pulse(PmicDeviceIndex , PM_SCHG_MISC_BUCKBOOST_REFRESH_PULSE_1_IN_8);
  
  Status = (errFlag == PM_ERR_FLAG__SUCCESS) ? EFI_SUCCESS : EFI_DEVICE_ERROR;

  return Status;
}


/**
EFI_PmicSchgEnableCharger ()

@brief
Enable charging
*/
EFI_STATUS
EFIAPI
EFI_PmicSchgEnableCharger
(
  IN UINT32  PmicDeviceIndex,
  IN BOOLEAN ChargerEnable
)
{
  EFI_STATUS           Status  = EFI_SUCCESS;
  pm_err_flag_type     errFlag = PM_ERR_FLAG__SUCCESS;
  pm_schg_chgr_status_type    charger_status;
  BOOLEAN              HWChargingEnabled = FALSE;

  PMIC_DEBUG(( EFI_D_WARN, "PmicDxe:: %a ChargerEnable: %d (0: Disable Charging, 1: Enable Charging)\r\n", __FUNCTION__, ChargerEnable ));

  errFlag |= pm_schg_chgr_get_chgr_status(PmicDeviceIndex, &charger_status);
  HWChargingEnabled = charger_status.status_5.charging_enable;

  if(HWChargingEnabled == ChargerEnable)
  {
    PMIC_DEBUG((EFI_D_WARN, "PmicDxe:: %a No action required ChargerEnable = %d HWChargingEnable = %d charging_enable = %d \n\r", __FUNCTION__, 
                ChargerEnable, HWChargingEnabled, charger_status.status_5.charging_enable));
  }
  else
  {
    /* If Charging is disabled, and API is called to enable charging
       or if Charging is enabled and API is called to disable charging
       Perform register write */
    errFlag |= pm_schg_chgr_enable_charging(PmicDeviceIndex, ChargerEnable);
    
  }

  //sdm855 todo need to determine if it is needed for sdm855.
  //if(TRUE == ChargerEnable)
  //{
  //  /* PMI8998 charger prevention of accidental reverse boosting, Enable 1-in-4 mode when charging */
  //  errFlag |= pm_schg_misc_set_buckboost_refresh_pulse(PmicDeviceIndex , PM_SCHG_MISC_BUCKBOOST_REFRESH_PULSE_1_IN_4);
  //}

  Status = (PM_ERR_FLAG__SUCCESS == errFlag) ? EFI_SUCCESS : EFI_DEVICE_ERROR;

  return (Status);
}

EFI_STATUS
EFIAPI
EFI_PmicSchgUsbinValid
(
  IN  UINT32 PmicDeviceIndex,
  OUT BOOLEAN *Valid
)
{
  EFI_STATUS            Status  = EFI_SUCCESS;
  pm_err_flag_type      errFlag = PM_ERR_FLAG__SUCCESS;
  BOOLEAN               UsbinPlugedIn;

  if(NULL == Valid)
  {
    Status = EFI_INVALID_PARAMETER;
  }
  else
  {
    /*Read IRQ real time status*/
    errFlag = pm_schg_usb_irq_status(PmicDeviceIndex, PM_SCHG_USB_IRQ_USBIN_PLUGIN, PM_IRQ_STATUS_RT,(boolean*)&UsbinPlugedIn);

    Status = (PM_ERR_FLAG__SUCCESS == errFlag)? PM_ERR_FLAG__SUCCESS : EFI_DEVICE_ERROR;

    *Valid = (UsbinPlugedIn) ? TRUE : FALSE;
  }

  return Status;
}


EFI_STATUS
EFIAPI
EFI_PmicSchgGetPowerPath
(
  IN  UINT32                  PmicDeviceIndex,
  OUT PM_SCHG_POWER_PATH_TYPE *PowerPath
)
{
  EFI_STATUS                  Status  = EFI_SUCCESS;
  //sdm855 todo need to determine if it is needed for sdm855.
  /*pm_err_flag_type            errFlag = PM_ERR_FLAG_SUCCESS;
  pm_schg_misc_pwr_pth_type   power_path = PM_SCHG_MISC_POWER_PATH__INVALID;

  if(NULL == PowerPath)
  {
    Status = EFI_INVALID_PARAMETER;
  }
  else
  {
    errFlag = pm_schg_misc_get_power_path(PmicDeviceIndex, &power_path);

    *PowerPath =(PM_SCHG_POWER_PATH_TYPE)power_path;

    Status = (PM_ERR_FLAG__SUCCESS == errFlag)? EFI_SUCCESS : EFI_DEVICE_ERROR;
  }*/

  return Status;
}


/**
EFI_PmicSchgIsBatteryPresent ()

@brief
Gets battery presence status
*/
EFI_STATUS
EFIAPI
EFI_PmicSchgIsBatteryPresent
(
  IN  UINT32 PmicDeviceIndex,
  OUT BOOLEAN *BatteryPresent
)
{
  EFI_STATUS        Status        = EFI_SUCCESS;
  pm_err_flag_type  errFlag       = PM_ERR_FLAG__SUCCESS;
  BOOLEAN           BattMissingLatched = FALSE;
  BOOLEAN           BattMissingRT = FALSE;
  STATIC BOOLEAN    FirstDetectionDone = FALSE;

  if(NULL == BatteryPresent)
  {
    return EFI_INVALID_PARAMETER;
  }
  else
  {
    /* Check BAT_THERM_OR_ID_MISSING Interrupt Latched status. */
    errFlag = pm_schg_batif_irq_status(PmicDeviceIndex, PM_SCHG_BATIF_BAT_THERM_OR_ID_MISSING, PM_IRQ_STATUS_LATCHED, (boolean *)&BattMissingLatched);
    errFlag |= pm_schg_batif_irq_status(PmicDeviceIndex, PM_SCHG_BATIF_BAT_THERM_OR_ID_MISSING, PM_IRQ_STATUS_RT, (boolean *)&BattMissingRT);
    PMIC_DEBUG((EFI_D_WARN, "PmicDxe:: %a Battery Missing: %d, Battery Missing Latched: %d (0: present, 1: missing)\r\n", __FUNCTION__, BattMissingRT, BattMissingLatched));
    if (BattMissingLatched == TRUE)
    {
      /*clear the interrupt*/
      errFlag |= pm_schg_batif_irq_clear(PmicDeviceIndex, PM_SCHG_BATIF_BAT_THERM_OR_ID_MISSING);
    }

    if (!FirstDetectionDone)
    {
      //first time use RT battery missing due to the latched status may not be set if the battery is missing before device reboot
      //then the edge trigger is already missed, so the latched status will show not missing, but RT status will always refect the 
      //true battery missing status.
      *BatteryPresent = (BattMissingRT) ? FALSE : TRUE;
      FirstDetectionDone = TRUE;
    }
    else
    {
      //if latched is asserted or missing RT is asserted, either is true, battery is missing.
      *BatteryPresent = (BattMissingLatched | BattMissingRT) ? FALSE : TRUE;
    }

    Status = (PM_ERR_FLAG__SUCCESS == errFlag)? EFI_SUCCESS : EFI_DEVICE_ERROR;
  }

  return Status;
}


/**
EFI_PmicChargerPortType ()

@brief
Gets charger port type
*/
EFI_STATUS
EFIAPI
EFI_PmicSchgGetChargerPortType
(
  IN  UINT32 PmicDeviceIndex,
  OUT EFI_PM_SCHG_CHGR_PORT_TYPE *PortType
)
{
  pm_err_flag_type  errFlag       = PM_ERR_FLAG__SUCCESS;
  BOOLEAN           ApsdDoneStatus = FALSE;
  BOOLEAN           bc1p2_en = FALSE;
  UINT32            ApsdDoneCheckIteration = 0;

  if(NULL == PortType)
  {
    return EFI_INVALID_PARAMETER;
  }

  *PortType = EFI_PM_SCHG_CHG_PORT_INVALID;

  
  errFlag = pm_schg_usb_get_usbin_option1_cfg(PmicDeviceIndex, PM_SCHG_USB_AUTO_SRC_DETECT, &bc1p2_en);
  if(PM_ERR_FLAG__SUCCESS != errFlag)
  {
    return EFI_DEVICE_ERROR;
  }
  if(!bc1p2_en)
  {
    *PortType = EFI_PM_SCHG_CHG_PORT_SDP_CHARGER;
    return EFI_SUCCESS;
  }
  
  do
  {
    errFlag = pm_schg_usb_get_apsd_status(PmicDeviceIndex, PM_SCHG_USB_APSD_STATUS_APSD_DTC_STATUS_DONE , &ApsdDoneStatus);
    PMIC_UART_DEBUG(( EFI_D_WARN, "PmicDxe:: %a APSD done status: %d \n\r", __FUNCTION__, ApsdDoneStatus));
    if(PM_ERR_FLAG__SUCCESS != errFlag)
    {
      break;
    }

    if(TRUE == ApsdDoneStatus)
    {
      errFlag =  pm_schg_usb_get_apsd_result_status(PmicDeviceIndex, (pm_schg_usb_apsd_result_status_type*) PortType);
      if(PM_ERR_FLAG__SUCCESS != errFlag)
      {
        break;
      }
    }
    else
    {
      /*if APSD result is not available yet, read again after 200 milli second.*/
      PMIC_DEBUG(( EFI_D_WARN, "PmicDxe:: %a APSD not done, stalling for 200mS before re-checking \n\r", __FUNCTION__));
      gBS->Stall(PM_SCHG_200_MS_WAIT);
    }

    ApsdDoneCheckIteration ++;

  }while ((FALSE == ApsdDoneStatus) && (ApsdDoneCheckIteration < PMIC_SCHG_MAX_APSD_DONE_CHECK_ITERATION));

  if(PM_ERR_FLAG__SUCCESS != errFlag)
  {
    *PortType = EFI_PM_SCHG_CHG_PORT_INVALID;
    return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
EFI_PmicSchgConfigOtg
(
IN  UINT32                    PmicDeviceIndex,
IN  EFI_PM_SCHG_DCDC_OTG_CFG_TYPE  OtgCfgType,
IN  BOOLEAN                   SetValue
)
{
	EFI_STATUS             Status = EFI_SUCCESS;
	pm_err_flag_type       errFlag = PM_ERR_FLAG__SUCCESS;

	errFlag = pm_schg_dcdc_config_otg((uint32)PmicDeviceIndex, (pm_schg_dcdc_otg_cfg_type)OtgCfgType, SetValue);

	if (PM_ERR_FLAG__SUCCESS != errFlag)
	{
		Status = EFI_DEVICE_ERROR;
	}

	return Status;
}

EFI_STATUS
EFIAPI
EFI_PmicSchgSetOtgILimit
(
IN  UINT32   PmicDeviceIndex,
IN  UINT32   ImAmp
)
{
	EFI_STATUS        Status = EFI_SUCCESS;
	pm_err_flag_type  errFlag = PM_ERR_FLAG__SUCCESS;

	errFlag = pm_schg_dcdc_set_otg_current_limit((uint32)PmicDeviceIndex, (uint32)ImAmp);
	if (PM_ERR_FLAG__SUCCESS != errFlag)
	{
		Status = EFI_DEVICE_ERROR;
	}

	return Status;
}


/**
  Sets Maximum USB current

  @param MaxCurrent  Current to be set

  @return
  EFI_SUCCESS:           Function returned successfully.
  EFI_INVALID_PARAMETER: A Parameter was incorrect.
  EFI_DEVICE_ERROR:      The physical device reported an error.
  EFI_NOT_READY:         The physical device is busy or not ready to
                         process this request.
*/

EFI_STATUS
EFIAPI
EFI_PmicSchgSetUsbMaxCurrent
(
  IN UINT32 PmicDeviceIndex,
  IN UINT32 MaxCurrent
)
{
    EFI_STATUS        Status  = EFI_SUCCESS;
    pm_err_flag_type  errFlag = PM_ERR_FLAG__SUCCESS;

    /* set desired MAX current */
    errFlag |= pm_schg_usb_set_usbin_current_limit_cfg(PmicDeviceIndex, MaxCurrent);

    if(PM_ERR_FLAG__SUCCESS != errFlag)
    {
      Status = EFI_DEVICE_ERROR;
    }

    return Status;
}



EFI_STATUS
EFIAPI
EFI_PmicSchgEnableAfpMode
(
  IN  UINT32   PmicDeviceIndex
)
{

  if(PM_ERR_FLAG__SUCCESS != pm_schg_misc_en_afp((uint32)PmicDeviceIndex))
  {
    return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
EFI_PmicSchgSetInputPriority
(
   IN  UINT32                          PmicDeviceIndex,
   IN  EFI_PM_SCHG_INPUT_PRIORITY_TYPE InputPriority
)
{
  if(InputPriority >= EFI_PM_SCHG_INPUT_PRIORITY_INVALID)
  {
    return EFI_INVALID_PARAMETER;
  }

  if(PM_ERR_FLAG__SUCCESS != pm_schg_usb_set_usbin_option1_cfg(PmicDeviceIndex, PM_SCHG_USB_INPUT_PRIORITY, (boolean)InputPriority))
  {
    return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;
}


EFI_STATUS
EFIAPI
EFI_PmicSchgEnableChgWdog
(
  IN UINT32  PmicDeviceIndex,
  IN BOOLEAN Enable
)
{
  EFI_STATUS                 Status  = EFI_SUCCESS;
  pm_err_flag_type           errFlag = PM_ERR_FLAG__SUCCESS;
  pm_schg_misc_wdog_cfg_type wd_cfg = {0};
  pm_schg_misc_sbb_wd_cfg    sbb_cfg = {0};

  //Read HW Default watchdog configuration
  errFlag = pm_schg_misc_get_config_wdog(PmicDeviceIndex, &wd_cfg);
  Status = (PM_ERR_FLAG__SUCCESS == errFlag)? PM_ERR_FLAG__SUCCESS : EFI_DEVICE_ERROR;
  if (PM_ERR_FLAG__SUCCESS != errFlag)
  {
      Status = EFI_DEVICE_ERROR;
      return Status;
  }

  //Enable watchdog and set timeout
  wd_cfg.bite_wdog_int_en = Enable;
  wd_cfg.bark_wdog_int_en = Enable;
  wd_cfg.wdog_timer_en = FALSE; /*Per PMIC system team recommendations, this bit should not be enabled. */
  wd_cfg.wdog_option = Enable;

  errFlag |= pm_schg_misc_get_snarl_bark_bite_wd_cfg(PmicDeviceIndex, &sbb_cfg);

  sbb_cfg.bite_wdog_disable_charging_cfg = Enable;
  
  if(TRUE == Enable)
  {
    sbb_cfg.bark_wdog_timeout = PM_SCHG_MISC_BARK_WDOG_TMOUT_128S;
    sbb_cfg.bite_wdog_timeout = PM_SCHG_MISC_BITE_WDOG_TMOUT_1S;
  }

  errFlag |= pm_schg_misc_set_snarl_bark_bite_wd_cfg(PmicDeviceIndex, sbb_cfg);

  errFlag |= pm_schg_misc_config_wdog(PmicDeviceIndex, wd_cfg);
  if (PM_ERR_FLAG__SUCCESS != errFlag)
  {
      Status = EFI_DEVICE_ERROR;
      PMIC_DEBUG(( EFI_D_WARN, "PmicDxe:: %a Error configuring Watchdog, Status; %d\r\n", __FUNCTION__, errFlag));
      return Status;
  }
  else
  {
    if(Enable)
    {
      PMIC_DEBUG(( EFI_D_WARN, "PmicDxe:: %a Charger Watchdog Enabled\r\n", __FUNCTION__ ));
    }
    else
    {
      PMIC_DEBUG(( EFI_D_WARN, "PmicDxe:: %a Charger Watchdog Disabled\r\n", __FUNCTION__ ));
    }
  }

  return Status;
}

EFI_STATUS
EFIAPI
EFI_PmicSchgSetFccMaxCurrent
(
  IN UINT32 PmicDeviceIndex,
  IN UINT32 MaxCurrentInmA
)
{

  if(PM_ERR_FLAG__SUCCESS != pm_schg_chgr_set_charge_current(PmicDeviceIndex, PM_SCHG_CHGR_FAST_CHARGE_CURRENT, MaxCurrentInmA))
  {
    return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;
}


EFI_STATUS
EFIAPI
EFI_PmicSchgSetFvMaxVoltage
(
  IN UINT32 PmicDeviceIndex,
  IN UINT32 MaxVoltageInMv
)
{

  if(PM_ERR_FLAG__SUCCESS != pm_schg_chgr_set_float_volt(PmicDeviceIndex, MaxVoltageInMv))
  {
    return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;
}


EFI_STATUS
EFIAPI
EFI_PmicSchgEnableOtg
(
  IN  UINT32   PmicDeviceIndex,
  IN  BOOLEAN  Enable
)
{
  EFI_STATUS                     Status = EFI_SUCCESS;
  pm_err_flag_type               errFlag = PM_ERR_FLAG__SUCCESS;

  errFlag = pm_schg_dcdc_otg_enable(PmicDeviceIndex, Enable);

  Status = (PM_ERR_FLAG__SUCCESS == errFlag) ? EFI_SUCCESS : EFI_DEVICE_ERROR;

  return Status;
}


EFI_STATUS
EFIAPI
EFI_PmicSchgGetOtgStatus
(
   IN  UINT32                       PmicDeviceIndex,
   OUT EFI_PM_SCHG_DCDC_OTG_STATUS_TYPE  *pOtgStatus
)
{
    EFI_STATUS         Status      = EFI_SUCCESS;
    pm_err_flag_type   err_flag    = PM_ERR_FLAG__SUCCESS;
    pm_schg_dcdc_otg_status_type TempOtgStatus = PM_SCHG_DCDC_OTG_STATUS_INVALID;

    err_flag = pm_schg_dcdc_get_otg_status(PmicDeviceIndex, &TempOtgStatus);

    switch (TempOtgStatus)
    {
      case PM_SCHG_DCDC_OTG_DISABLED: 
        *pOtgStatus = EFI_PM_SCHG_DCDC_OTG_DISABLED;
        break;
      case PM_SCHG_DCDC_OTG_EXECUTING_ENABLE_SEQ: 
        *pOtgStatus = EFI_PM_SCHG_DCDC_OTG_EXECUTING_ENABLE_SEQ;
        break;
      case PM_SCHG_DCDC_OTG_ENABLED: 
        *pOtgStatus = EFI_PM_SCHG_DCDC_OTG_ENABLED;
        break;
      case PM_SCHG_DCDC_OTG_EXECUTING_DISABLE_SEQ: 
        *pOtgStatus = EFI_PM_SCHG_DCDC_OTG_EXECUTING_DISABLE_SEQ;
        break;
      case PM_SCHG_DCDC_OTG_STATUS_INVALID: 
        *pOtgStatus = EFI_PM_SCHG_DCDC_OTG_DISABLED;
        break;
      case PM_SCHG_DCDC_OTG_ERROR: 
        *pOtgStatus = EFI_PM_SCHG_DCDC_OTG_ERROR;
        break;
      default: 
        *pOtgStatus = EFI_PM_SCHG_DCDC_OTG_ERROR;
    }

    Status = (PM_ERR_FLAG__SUCCESS == err_flag) ? EFI_SUCCESS : EFI_DEVICE_ERROR;

    return Status;
}

EFI_STATUS
EFIAPI
EFI_PmicSchgUsbSuspend
(
  IN UINT32  PmicDeviceIndex,
  IN BOOLEAN Enable
  )
{

  if(PM_ERR_FLAG__SUCCESS != pm_schg_usb_set_usbin_suspend(PmicDeviceIndex, Enable))
  {
    return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;
}


EFI_STATUS
EFIAPI
EFI_PmicSchgPetChgWdog
(
  IN UINT32 PmicDeviceIndex
)
{
  pm_err_flag_type          errFlag = PM_ERR_FLAG__SUCCESS;
  pm_schg_chgr_status_type  charger_status;

  errFlag |= pm_schg_chgr_get_chgr_status(PmicDeviceIndex, &charger_status);
  if((FALSE == charger_status.status_5.disable_charging) && (PM_ERR_FLAG__SUCCESS == errFlag))
  {//Pet wdog only if charging is enabled
    if (PM_ERR_FLAG__SUCCESS != pm_schg_misc_bark_bite_wdog_pet(PmicDeviceIndex))
    {
      PMIC_DEBUG(( EFI_D_WARN, "PmicDxe:: %a error while petting watchdog : (%d)\r\n", __FUNCTION__, errFlag ));
      return EFI_DEVICE_ERROR;
    }
  }
  else if(PM_ERR_FLAG__SUCCESS != errFlag)
  {
    PMIC_DEBUG(( EFI_D_WARN, "PmicDxe:: %a error while getting charger status: (%d)\r\n", __FUNCTION__, errFlag ));
    return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
EFI_PmicSchgPrintAiclSts
(
IN  UINT32   PmicDeviceIndex,
OUT BOOLEAN  *IsAICLComplete
)
{
	EFI_STATUS        Status = EFI_SUCCESS;
    /*
	pm_err_flag_type  err_flag = PM_ERR_FLAG__SUCCESS;
	input_current_limit_sts icl_sts = { 0 };

	if (IsAICLComplete == NULL)
	{
        PMIC_DEBUG((EFI_D_WARN, "PmicDxe:: %a Invalid parameter isAICLComplete is NULL \r\n",  __FUNCTION__));
		return EFI_INVALID_PARAMETER;
	}

	err_flag = pm_schg_usb_icl_sts(PmicDeviceIndex, &icl_sts);
	if (PM_ERR_FLAG__SUCCESS != err_flag)
	{
        PMIC_DEBUG((EFI_D_WARN, "PmicDxe:: %a AICL error while running pm_schg_usb_icl_sts (%d) \r\n",  __FUNCTION__, errFlag));
		return EFI_DEVICE_ERROR;
	}
	*IsAICLComplete = icl_sts.is_aicl_complete;
    PMIC_DEBUG((EFI_D_WARN, "PmicDxe:: %a AICL status info AICL completed = %d ,\n\t\t\t \
		ICL mode = %d ,\n\t\t\t \
		usbin suspended = %d ,\n\t\t\t \
		dcin suspended = %d ,\n\t\t\t \
		usbin active pwr src = %d ,\n\t\t\t \
		dcin active pwr src = %d ,\n\t\t\t \
		Input Current Limited to %d \r\n",  __FUNCTION__, 
		icl_sts.is_aicl_complete,
		icl_sts.icl_mode,
		icl_sts.is_usbin_suspended,
		icl_sts.is_dcin_suspended,
		icl_sts.is_usbin_active_pwr_src,
		icl_sts.is_dcin_active_pwr_src,
		icl_sts.input_current_limit));
	
	*/
	return Status;
}

EFI_STATUS
EFIAPI
EFI_PmicSchgChgrSourceReinserted
(
  IN  UINT32   PmicDeviceIndex,
  OUT BOOLEAN  *pReinserted
  )
{
  EFI_STATUS                           Status  = EFI_SUCCESS;
  pm_err_flag_type                     errFlag = PM_ERR_FLAG__SUCCESS;
  BOOLEAN                              LatchedStatus = FALSE;
  BOOLEAN                              RtStatus = FALSE;

  if(NULL == pReinserted)
  {
    Status = EFI_INVALID_PARAMETER;
  }
  else
  {
    *pReinserted = FALSE;
    /*if latched is low*/
    errFlag = pm_schg_usb_irq_status(PmicDeviceIndex, PM_SCHG_USB_IRQ_USBIN_PLUGIN, PM_IRQ_STATUS_LATCHED, (boolean *)&LatchedStatus);
    if(LatchedStatus == TRUE)
    {
      /*Read IRQ real time status*/
      errFlag |= pm_schg_usb_irq_status(PmicDeviceIndex, PM_SCHG_USB_IRQ_USBIN_PLUGIN, PM_IRQ_STATUS_RT, (boolean *)&RtStatus);
      *pReinserted = (RtStatus == TRUE) ? TRUE : FALSE;
      /*clear the interrupt*/
      pm_schg_usb_irq_clear(PmicDeviceIndex, PM_SCHG_USB_IRQ_USBIN_PLUGIN);
    }

    Status = (PM_ERR_FLAG__SUCCESS == errFlag) ? PM_ERR_FLAG__SUCCESS : EFI_DEVICE_ERROR;
  }

  return Status;
}

EFI_STATUS
EFIAPI
EFI_PmicSchgRerunAicl
(
  IN  UINT32                          PmicDeviceIndex,
  IN  EFI_PM_SCHG_AICL_RERUN_TIME_CFG RerunTime
  )
{
  EFI_STATUS                           Status  = EFI_SUCCESS;
  /*
  TBD: Code will be enabled once HW is available and testing can be done
  pm_err_flag_type                     errFlag = PM_ERR_FLAG__SUCCESS;

  errFlag |= pm_schg_usb_aicl_options_cfg(PmicDeviceIndex, PM_SCHG_USB_AICL_CFG_RERUN_EN, TRUE);
  errFlag |= pm_schg_misc_set_aicl_rerun_time_cfg(PmicDeviceIndex, (pm_schg_misc_aicl_rerun_time_cfg)RerunTime);
  Status = (PM_ERR_FLAG__SUCCESS == errFlag) ? PM_ERR_FLAG__SUCCESS : EFI_DEVICE_ERROR;
  */

  return Status;
}

EFI_STATUS PmicSchgDumpPeripheral( VOID )
{
  EFI_STATUS Status  = EFI_SUCCESS;
  pm_err_flag_type errFlag = PM_ERR_FLAG__SUCCESS;
  UINT8 index = 0;

  errFlag = pm_schg_chgr_get_schg_pmic_info((UINT8 *)&gSchgPmicIndex, (UINT8 *)&gSchgSlaveIndex, &gChargerCount, MAX_CHARGING_PORT);
  if(PM_ERR_FLAG__SUCCESS != errFlag)
  {
    PMIC_DEBUG((EFI_D_WARN, "PmicDxe::%a SCHG_PERIPHERAL get_schg_pmic_info Error (%d) \n\r", __FUNCTION__, errFlag));
    return EFI_UNSUPPORTED;
  }
  for (index = 0; index < gChargerCount; index++)
  {
    errFlag = pm_core_utils_dump_peripheral(gSchgSlaveIndex[index], PMIC_SCHG_PERIPHERAL_START_ADDR, PMIC_SCHG_PERIPHERAL_NUM);
  if(PM_ERR_FLAG__SUCCESS != errFlag)
  {
   PMIC_DEBUG((EFI_D_WARN, "PmicDxe::%a SCHG_PERIPHERAL pm_core_utils_dump_peripheral Error (%d) \r\n",  __FUNCTION__, errFlag));
  }

  /* Dump separately for Misc peripheral */
    errFlag |= pm_core_utils_dump_peripheral(gSchgSlaveIndex[index], PMIC_SCHG_MISC_PERIPHERAL_START_ADDR, 1);
  if(PM_ERR_FLAG__SUCCESS != errFlag)
  {
    PMIC_DEBUG((EFI_D_WARN, "PmicDxe::%a SCHG_MISC_PERIPHERAL pm_core_utils_dump_peripheral Error (%d) \n\r", __FUNCTION__, errFlag));
  }

  Status = (PM_ERR_FLAG__SUCCESS == errFlag) ? EFI_SUCCESS : EFI_DEVICE_ERROR;
  }


  return Status;
}

EFI_STATUS EFI_PmicSchgEnableJeita(UINT32 PmicDeviceIndex, EFI_PM_SCHG_JEITA_TYPE JeitaType)
{
  return EFI_UNSUPPORTED;
}

EFI_STATUS
EFIAPI
EFI_PmicSchgGetChargingStatus
(
  UINT32 PmicDeviceIndex,
  OUT EFI_PM_SCHG_CHARGING_STATUS *pChargingStatus
)
{
  pm_err_flag_type          errFlag = PM_ERR_FLAG__SUCCESS;
  pm_schg_chgr_status_type  charger_status;

  if(NULL == pChargingStatus)
  {
    return EFI_INVALID_PARAMETER;
  }

  DALSYS_memset(&charger_status, 0x00, sizeof(charger_status));

  pChargingStatus->bChargingEnabled = FALSE;

  errFlag = pm_schg_chgr_get_chgr_status(PmicDeviceIndex, &charger_status);
  if(PM_ERR_FLAG__SUCCESS != errFlag)
  {
    PMIC_DEBUG((EFI_D_WARN, "PmicDxe:: %a Error getting charging status = %d \r\n", __FUNCTION__, errFlag));
    return EFI_DEVICE_ERROR;
  }

  pChargingStatus->bChargingEnabled = charger_status.status_5.charging_enable;

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
EFI_PmicSchgDCInValid
(
IN  UINT32 PmicDeviceIndex,
OUT BOOLEAN *Valid
)
{
  EFI_STATUS            Status = EFI_SUCCESS;
  pm_err_flag_type      errFlag = PM_ERR_FLAG__SUCCESS;
  BOOLEAN               DcinPlugedIn;

  if (NULL == Valid)
  {
    Status = EFI_INVALID_PARAMETER;
  }
  else
  {
    /*get dcin irq status Read IRQ real time status*/
    errFlag = pm_schg_dc_irq_status(PmicDeviceIndex, PM_SCHG_DC_IRQ_DCIN_PLUGIN, PM_IRQ_STATUS_RT, (boolean*)&DcinPlugedIn);

    Status = (PM_ERR_FLAG__SUCCESS == errFlag) ? PM_ERR_FLAG__SUCCESS : EFI_DEVICE_ERROR;

    *Valid = (DcinPlugedIn) ? TRUE : FALSE;
  }

  return Status;
}




EFI_STATUS
EFIAPI
EFI_PmicSchgDCInSuspend
(
IN  UINT32 PmicDeviceIndex,
IN  BOOLEAN Suspend
)
{
  EFI_STATUS            Status = EFI_SUCCESS;
  pm_err_flag_type      errFlag = PM_ERR_FLAG__SUCCESS;

  errFlag = pm_schg_dc_set_cmd_il(PmicDeviceIndex, PM_SCHG_DC_CMD_IL_TYPE_DCIN_SUSPEND, Suspend);

  Status = (PM_ERR_FLAG__SUCCESS == errFlag) ? PM_ERR_FLAG__SUCCESS : EFI_DEVICE_ERROR;

  return Status;
}


EFI_STATUS
EFIAPI
EFI_PmicSchgSetDCInPower
(
IN  UINT32  PmicDeviceIndex,
IN  UINT32  PowerInMicroWalts
)
{
  pm_err_flag_type err_flag = PM_ERR_FLAG__SUCCESS;
  uint32 ilmit_level_ma = 0;

  //Configure PM_SCHG_DC_ZIN_ICL_PT;  Use 8V :  W=VI
  ilmit_level_ma = PowerInMicroWalts / PM_DC_ZIN_ICL_PT_VOLT_MV;
  err_flag |= pm_schg_dc_set_zin_icl(PmicDeviceIndex, PM_SCHG_DC_ZIN_ICL_PT, ilmit_level_ma);

  //Configure PM_SCHG_DC_ZIN_ICL_PT_HV;  9V 
  ilmit_level_ma = PowerInMicroWalts / PM_DC_ZIN_ICL_PT_HV_VOLT_MV;
  err_flag |= pm_schg_dc_set_zin_icl(PmicDeviceIndex, PM_SCHG_DC_ZIN_ICL_PT_HV, ilmit_level_ma);

  //Configure PM_SCHG_DC_ZIN_ICL_LV; Use 5.5V 
  ilmit_level_ma = PowerInMicroWalts / PM_DC_ZIN_ICL_LV_VOLT_MV;
  err_flag |= pm_schg_dc_set_zin_icl(PmicDeviceIndex, PM_SCHG_DC_ZIN_ICL_LV, ilmit_level_ma);

  //Configure PM_SCHG_DC_ZIN_ICL_MID_LV; Use 6.5V 
  ilmit_level_ma = PowerInMicroWalts / PM_DC_ZIN_ICL_MID_LV_VOLT_MV;
  err_flag |= pm_schg_dc_set_zin_icl(PmicDeviceIndex, PM_SCHG_DC_ZIN_ICL_MID_LV, ilmit_level_ma);

  //Configure PM_SCHG_DC_ZIN_ICL_MID_HV; Use 8V 
  ilmit_level_ma = PowerInMicroWalts / PM_DC_ZIN_ICL_MID_HV_VOLT_MV;
  err_flag |= pm_schg_dc_set_zin_icl(PmicDeviceIndex, PM_SCHG_DC_ZIN_ICL_MID_HV, ilmit_level_ma);

  //Configure PM_SCHG_DC_ZIN_ICL_HV; Use 11V 
  ilmit_level_ma = PowerInMicroWalts / PM_DC_ZIN_ICL_HV_VOLT_MV;
  err_flag |= pm_schg_dc_set_zin_icl(PmicDeviceIndex, PM_SCHG_DC_ZIN_ICL_HV, ilmit_level_ma);

  return err_flag;
}


EFI_STATUS
EFIAPI
EFI_PmicSchgSetUsbIclMode
(
  UINT32                            PmicDeviceIndex,
  IN EFI_PM_SCHG_USB_ICL_MODE_TYPE  IclModeType
)
{
  pm_err_flag_type              errFlag = PM_ERR_FLAG__SUCCESS;

  switch(IclModeType)
  {
    case EFI_PM_SCHG_USB_ICL_MODE_SDP_2_0: 
         /**< Select USB 2.0 CFG, Set USB51 Mode to 500 mA, set USBIN_MODE to USB 5/1 or USB 9/1.5 Current Level >*/
         errFlag |= pm_schg_usb_configure_usbin_icl_options(PmicDeviceIndex, PM_SCHG_USB_ICL_OPTIONS_USBIN_MODE_CHG, FALSE);
         errFlag |= pm_schg_usb_configure_usbin_icl_options(PmicDeviceIndex, PM_SCHG_USB_ICL_OPTIONS_USB51_MODE, TRUE);
         errFlag |= pm_schg_usb_configure_usbin_icl_options(PmicDeviceIndex, PM_SCHG_USB_ICL_OPTIONS_CFG_USB3P0_SEL, FALSE);
         break;
    case EFI_PM_SCHG_USB_ICL_MODE_SDP_3_0:
         /**< Select USB 3.0 CFG, Set USB51 Mode to 500 mA, set USBIN_MODE to USB 5/1 or USB 9/1.5 Current Level >*/
         errFlag |= pm_schg_usb_configure_usbin_icl_options(PmicDeviceIndex, PM_SCHG_USB_ICL_OPTIONS_USBIN_MODE_CHG, FALSE);
         errFlag |= pm_schg_usb_configure_usbin_icl_options(PmicDeviceIndex, PM_SCHG_USB_ICL_OPTIONS_USB51_MODE, TRUE);
         errFlag |= pm_schg_usb_configure_usbin_icl_options(PmicDeviceIndex, PM_SCHG_USB_ICL_OPTIONS_CFG_USB3P0_SEL, TRUE);
         break;
    case EFI_PM_SCHG_USB_ICL_MODE_WALL_CHARGERS: 
         /**< Select USB 2.0 CFG(Default), Set USB51 Mode to 500 mA, set USBIN_MODE to HC Mode Current Level     >*/
         errFlag |= pm_schg_usb_configure_usbin_icl_options(PmicDeviceIndex, PM_SCHG_USB_ICL_OPTIONS_USBIN_MODE_CHG, TRUE);
         errFlag |= pm_schg_usb_configure_usbin_icl_options(PmicDeviceIndex, PM_SCHG_USB_ICL_OPTIONS_USB51_MODE, TRUE);
         errFlag |= pm_schg_usb_configure_usbin_icl_options(PmicDeviceIndex, PM_SCHG_USB_ICL_OPTIONS_CFG_USB3P0_SEL, FALSE);
         break;
    default:
         return EFI_INVALID_PARAMETER;
  }

  if(PM_ERR_FLAG__SUCCESS != errFlag)
  {
    PMIC_DEBUG((EFI_D_WARN, "PmicDxe:: %a Error configuring ICL options, status = %d \r\n", __FUNCTION__, errFlag));
    return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;
}


EFI_STATUS
EFIAPI
EFI_PmicSchgGetChgWdogStatus
(
  IN UINT32                     PmicDeviceIndex,
  OUT EFI_PM_SCHG_WDOG_STS_TYPE *pWdogSts
)
{
  pm_err_flag_type              errFlag = PM_ERR_FLAG__SUCCESS;
  pm_schg_misc_wdog_status      WdogStatus;
  pm_schg_misc_sbb_wd_cfg       WdogCfg;
  BOOLEAN                       WdogBarkBiteSts = FALSE;
  
  if(NULL == pWdogSts)
  {
    return EFI_INVALID_PARAMETER;
  }

  *pWdogSts = EFI_PM_SCHG_WDOG_STS_INVALID;

  errFlag = pm_schg_misc_irq_status(PmicDeviceIndex, PM_SCHG_MISC_IRQ_WDOG_BARK, PM_IRQ_STATUS_LATCHED,  (boolean *)&WdogBarkBiteSts);

  if(WdogBarkBiteSts)
  {
    PMIC_DEBUG((EFI_D_WARN, "PmicDxe:: %a Charger watchdog bark/bite status: %d \r\n", __FUNCTION__, WdogBarkBiteSts));

    /*clear the interrupt*/
    errFlag |= pm_schg_misc_irq_clear(PmicDeviceIndex, PM_SCHG_MISC_IRQ_WDOG_BARK);

    errFlag = pm_schg_misc_get_misc_wdog_status(PmicDeviceIndex, &WdogStatus);
    if(PM_ERR_FLAG__SUCCESS != errFlag)
    {
      PMIC_DEBUG((EFI_D_WARN, "PmicDxe:: %a Error getting wdog status, status = %d \r\n", __FUNCTION__, errFlag));
      return EFI_DEVICE_ERROR;
    }
    
    errFlag = pm_schg_misc_get_snarl_bark_bite_wd_cfg( PmicDeviceIndex, &WdogCfg );
    if(PM_ERR_FLAG__SUCCESS != errFlag)
    {
      PMIC_DEBUG((EFI_D_WARN, "PmicDxe:: %a Error getting wdog snark/bark/bite status, status = %d \r\n", __FUNCTION__, errFlag));
      return EFI_DEVICE_ERROR;
    }
    
    switch(WdogStatus)
    {
      case PM_SCHG_MISC_BARK:
           *pWdogSts = EFI_PM_SCHG_WDOG_STS_BARK;
           break;
      case PM_SCHG_MISC_BITE:
           if(WdogCfg.bite_wdog_disable_charging_cfg)
           {
             *pWdogSts = EFI_PM_SCHG_WDOG_STS_BITE_CHARGING_DISABLED;
           }
           else
           {
             *pWdogSts = EFI_PM_SCHG_WDOG_STS_BITE_CHARGING_NOT_DISABLED;
           }
           break;
      default: 
           *pWdogSts = EFI_PM_SCHG_WDOG_STS_INVALID;
           break;
    }
  }
 
  return EFI_SUCCESS;
}

EFI_STATUS EFI_PmicSchgEnableHWJeita(UINT32 PmicDeviceIndex, BOOLEAN Enable)
{
  pm_err_flag_type errFlag = PM_ERR_FLAG__SUCCESS;

  if(TRUE == Enable)
  {
      /* Set Compensation for Fv and Fcc */
	  errFlag |= pm_schg_chgr_set_jeita_fvcomp_cfg(PmicDeviceIndex, gSchgCfgData.SchgJeitaData.JeitaFvCompHotCfg, gSchgCfgData.SchgJeitaData.JeitaFvCompColdCfg);
	  errFlag |= pm_schg_chgr_set_jeita_ccomp(PmicDeviceIndex, gSchgCfgData.SchgJeitaData.JeitaCcCompHotCfg, gSchgCfgData.SchgJeitaData.JeitaCcCompColdCfg);

	  /* Set enable bits for float voltage and charge current compensation */
	  errFlag |= pm_schg_chgr_jeita_en_cfg(PmicDeviceIndex, PM_SCHG_CHGR_JEITA_HARD_LIMIT, gSchgCfgData.SchgJeitaData.JeitaHardLimitEnable);
	  errFlag |= pm_schg_chgr_jeita_en_cfg(PmicDeviceIndex, PM_SCHG_CHGR_JEITA_HOT_SL_FVC, gSchgCfgData.SchgJeitaData.JeitaSoftHotFvCompEnable);
	  errFlag |= pm_schg_chgr_jeita_en_cfg(PmicDeviceIndex, PM_SCHG_CHGR_JEITA_COLD_SL_FVC, gSchgCfgData.SchgJeitaData.JeitaSoftColdFvCompEnable);
	  errFlag |= pm_schg_chgr_jeita_en_cfg(PmicDeviceIndex, PM_SCHG_CHGR_JEITA_HOT_SL_CCC, gSchgCfgData.SchgJeitaData.JeitaSoftHotCcCompEnable);
	  errFlag |= pm_schg_chgr_jeita_en_cfg(PmicDeviceIndex, PM_SCHG_CHGR_JEITA_COLD_SL_CCC, gSchgCfgData.SchgJeitaData.JeitaSoftColdCcCompEnable);

	  /* Set the temperature thresholds for JEITA */
	  errFlag |= pm_schg_chgr_set_jeita_threshold_value(PmicDeviceIndex, PM_SCHG_CHGR_JEITA_THRESHOLD_HOT, gSchgCfgData.SchgJeitaData.JeitaSoftHotLimit);
	  errFlag |= pm_schg_chgr_set_jeita_threshold_value(PmicDeviceIndex, PM_SCHG_CHGR_JEITA_THRESHOLD_COLD, gSchgCfgData.SchgJeitaData.JeitaSoftColdLimit);
	  errFlag |= pm_schg_chgr_set_jeita_threshold_value(PmicDeviceIndex, PM_SCHG_CHGR_JEITA_THRESHOLD_TOO_HOT, gSchgCfgData.SchgJeitaData.JeitaHardHotLimit);
	  errFlag |= pm_schg_chgr_set_jeita_threshold_value(PmicDeviceIndex, PM_SCHG_CHGR_JEITA_THRESHOLD_TOO_COLD, gSchgCfgData.SchgJeitaData.JeitaHardColdLimit);

	  /* set HW AFP thresholds, if using real battery (not debug board) */
	  if (!gbDbgBoard)
	  {
	    errFlag = pm_schg_chgr_set_jeita_threshold_value(PmicDeviceIndex, PM_SCHG_CHGR_JEITA_THRESHOLD_TOO_HOT_AFP, gSchgCfgData.HwAfpHotThreshold);
	    errFlag |= pm_schg_chgr_set_jeita_threshold_value(PmicDeviceIndex, PM_SCHG_CHGR_JEITA_THRESHOLD_TOO_COLD_AFP, gSchgCfgData.HwAfpColdThreshold);

	  }
  }
  else
  {
    errFlag = pm_schg_chgr_jeita_en_cfg(PmicDeviceIndex, PM_SCHG_CHGR_JEITA_HARD_LIMIT, FALSE);
	errFlag |= pm_schg_chgr_jeita_en_cfg(PmicDeviceIndex, PM_SCHG_CHGR_JEITA_HOT_SL_FVC, FALSE);
	errFlag |= pm_schg_chgr_jeita_en_cfg(PmicDeviceIndex, PM_SCHG_CHGR_JEITA_COLD_SL_FVC, FALSE);
	errFlag |= pm_schg_chgr_jeita_en_cfg(PmicDeviceIndex, PM_SCHG_CHGR_JEITA_HOT_SL_CCC, FALSE);
	errFlag |= pm_schg_chgr_jeita_en_cfg(PmicDeviceIndex, PM_SCHG_CHGR_JEITA_COLD_SL_CCC, FALSE);
  }

  if(PM_ERR_FLAG__SUCCESS != errFlag)
  {
    PMIC_DEBUG((EFI_D_WARN, "PmicDxe:: %a Error = %d \r\n", __FUNCTION__, errFlag));
    return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;
}

EFI_STATUS EFI_PmicSchgToggleWipowerSDLatch(UINT32 PmicDeviceIndex)
{ 
	return EFI_UNSUPPORTED;
}

EFI_STATUS EFI_PmicSchgSetDcInCurrent(UINT32 PmicDeviceIndex, UINT32 dcInCurrentLimit)
{
  pm_err_flag_type errFlag = PM_ERR_FLAG__SUCCESS;

  errFlag = pm_schg_dcdc_set_ref_max_psns(PmicDeviceIndex, dcInCurrentLimit);

  if (PM_ERR_FLAG__SUCCESS != errFlag)
  {
    PMIC_DEBUG((EFI_D_WARN, "PmicDxe:: %a Error = %d \r\n", __FUNCTION__, errFlag));
    return EFI_DEVICE_ERROR;
  }

  PMIC_DEBUG((EFI_D_WARN, "PmicDxe:: %a status = %d \r\n", __FUNCTION__, errFlag));
  return EFI_SUCCESS;
}

EFI_STATUS EFI_PmicSchgSetChargeCmdBit(UINT32 PmicDeviceIndex, BOOLEAN Enable)
{
  pm_err_flag_type     errFlag = PM_ERR_FLAG__SUCCESS;

  errFlag = pm_schg_chgr_enable_charging(PmicDeviceIndex, Enable);
  if (PM_ERR_FLAG__SUCCESS != errFlag)
  {
    return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;
}

/**
EFI_PmicSchgGetPmicInfo ()

@brief
Provides SCHG charger PMIC Index.
*/
EFI_STATUS
EFIAPI
EFI_PmicSchgGetPmicInfo
(
  OUT EFI_PM_SCHG_PMIC_INFO *SchgPmicInfo
)
{
  pm_err_flag_type errFlag = PM_ERR_FLAG__SUCCESS;
  if(!SchgPmicInfo)
  {
    return EFI_INVALID_PARAMETER;
  }

  errFlag = pm_schg_chgr_get_schg_pmic_info((UINT8 *)gSchgPmicIndex, (UINT8 *)gSchgSlaveIndex, &gChargerCount, MAX_CHARGING_PORT);

  if (PM_ERR_FLAG__SUCCESS != errFlag)
  {
    return EFI_DEVICE_ERROR;
  }

  //to support hana, this API will only return the first charging port.
  SchgPmicInfo->PmicIndex = gSchgPmicIndex[0];
  SchgPmicInfo->SlaveIndex = gSchgSlaveIndex[0];

  return (PM_ERR_FLAG__SUCCESS == errFlag) ? EFI_SUCCESS : EFI_DEVICE_ERROR;
}

EFI_STATUS PmicSchgSetHwAfpThresholds(UINT32 PmicDeviceIndex, INT32 HotThreshold, INT32 ColdThreshold)
{
  pm_err_flag_type     errFlag = PM_ERR_FLAG__SUCCESS;

  /* Configure the AFP thresholds */
  errFlag = pm_schg_chgr_set_jeita_threshold_value(PmicDeviceIndex, PM_SCHG_CHGR_JEITA_THRESHOLD_TOO_HOT_AFP, HotThreshold);
  errFlag |= pm_schg_chgr_set_jeita_threshold_value(PmicDeviceIndex, PM_SCHG_CHGR_JEITA_THRESHOLD_TOO_COLD_AFP, ColdThreshold);

  if (PM_ERR_FLAG__SUCCESS != errFlag)
  {
	  return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;
}

EFI_STATUS PmicSchgSetDieTempThresholds(UINT32 PmicDeviceIndex, INT32 HotThreshold, INT32 TooHotThreshold)
{
	pm_err_flag_type     errFlag = PM_ERR_FLAG__SUCCESS;

	/* Configure the Die temp thresholds */
	errFlag = pm_schg_misc_set_die_temp_thresholds(PmicDeviceIndex, HotThreshold, TooHotThreshold);

	if (PM_ERR_FLAG__SUCCESS != errFlag)
	{
		return EFI_DEVICE_ERROR;
	}

	return EFI_SUCCESS;
}


EFI_STATUS PmicSchgSetThermRegSrcCfg(UINT32 PmicDeviceIndex, BOOLEAN SkinEnable, BOOLEAN DieEnable, BOOLEAN ComparatorEnable)
{
  pm_err_flag_type     errFlag = PM_ERR_FLAG__SUCCESS;

  errFlag = pm_schg_misc_set_thermreg_src_cfg(PmicDeviceIndex, PM_SCHG_MISC_SKIN_ADC_SRC_EN, SkinEnable);
  errFlag |= pm_schg_misc_set_thermreg_src_cfg(PmicDeviceIndex, PM_SCHG_MISC_DIE_ADC_SRC_EN, DieEnable);
  errFlag |= pm_schg_misc_set_thermreg_src_cfg(PmicDeviceIndex, PM_SCHG_MISC_DIE_CMP_SRC_EN, ComparatorEnable);

  if (PM_ERR_FLAG__SUCCESS != errFlag)
  {
    return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
EFI_PmicSchgConfigApsd
(
  IN  UINT32                      PmicDeviceIndex
)
{
  pm_err_flag_type errFlag = PM_ERR_FLAG__SUCCESS;

  errFlag |= pm_schg_usb_rerun_apsd(PmicDeviceIndex);

  if (PM_ERR_FLAG__SUCCESS != errFlag)
  {
    return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
EFI_PmicSchgConfigHvdcp
(
  IN UINT32                       PmicDeviceIndex,
  IN EFI_SCHG_HVDCP_CFGDATA_TYPE *pSchgHvdcpDataType
)
{
  pm_err_flag_type errFlag = PM_ERR_FLAG__SUCCESS;

  if(!pSchgHvdcpDataType)
    return EFI_INVALID_PARAMETER;

  errFlag  = pm_schg_usb_set_usbin_option1_cfg(PM_DEVICE_1, PM_SCHG_USB_HVDCP_AUTH_ALG_EN_CFG, pSchgHvdcpDataType->bEnHvdcp);
  errFlag |= pm_schg_usb_set_usbin_option1_cfg(PM_DEVICE_1, PM_SCHG_USB_HVDCP_AUTONOMOUS_MODE_EN_CFG, pSchgHvdcpDataType->bEnHvdcp);
  if (PM_ERR_FLAG__SUCCESS != errFlag)
  {
    return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;

}

EFI_STATUS EFI_PmicSchgGetIclStatus
(
IN UINT32                       PmicDeviceIndex,
OUT EFI_PM_SCHG_ICL_STATUS *pIclStatus
)
{
	pm_err_flag_type errFlag = PM_ERR_FLAG__SUCCESS;

	if (!pIclStatus)
	{
		return EFI_INVALID_PARAMETER;
	}

	errFlag = pm_schg_dcdc_get_icl_status(PmicDeviceIndex, (pm_schg_dcdc_icl_status_type*)pIclStatus);
	if (PM_ERR_FLAG__SUCCESS != errFlag)
	{
		return EFI_DEVICE_ERROR;
	}

	return EFI_SUCCESS;

}

EFI_STATUS EFI_PmicSchgSetVconn
(
IN UINT32                       PmicDeviceIndex,
IN BOOLEAN                     VconnEn
)
{
	pm_err_flag_type errFlag = PM_ERR_FLAG__SUCCESS;
	 
	/* Set Vconn */
	errFlag = pm_schg_usb_typec_vconn_enable(PmicDeviceIndex, (VconnEn ? PM_SCHG_USB_TYPEC_VCONN_ENABLE_BY_SW:PM_SCHG_USB_TYPEC_VCONN_DISABLE_BY_SW));
	if (PM_ERR_FLAG__SUCCESS != errFlag)
	{
		return EFI_DEVICE_ERROR;
	}

	return EFI_SUCCESS;

}

EFI_STATUS EFI_PmicSchgGetPortState
(
IN UINT32                       PmicDeviceIndex,
OUT  EFI_PM_SCHG_TYPEC_PORT_STATUS  *TypeCStatus
)
{
	pm_err_flag_type errFlag = PM_ERR_FLAG__SUCCESS;
	pm_schg_usb_typec_status_type typec_status = { 0 };

	if (!TypeCStatus)
	{
		return EFI_INVALID_PARAMETER;
	}

	errFlag = pm_schg_usb_get_typec_status(PmicDeviceIndex, &typec_status);
	if (PM_ERR_FLAG__SUCCESS != errFlag)
	{
		return EFI_DEVICE_ERROR;
	}
	else
	{
		TypeCStatus->cc_out_sts = (EFI_PM_SCHG_TYPEC_CC_OUT_STATUS)typec_status.cc_out_sts;
		TypeCStatus->dfp_curr_adv = (EFI_PM_SCHG_TYPEC_DFP_CURR_ADV)typec_status.dfp_curr_adv;
		TypeCStatus->ufp_conn_type = (EFI_PM_SCHG_TYPEC_UFP_CONNECTOR_TYPE)typec_status.ufp_conn_type;
		TypeCStatus->vbus_sts = typec_status.vbus_sts;
		TypeCStatus->vbus_err_sts = typec_status.vbus_err_sts;
		TypeCStatus->debounce_done_sts = typec_status.debounce_done_sts;
		TypeCStatus->vconn_oc_sts = typec_status.vconn_oc_sts;
		TypeCStatus->ccout_out_hiz_sts = typec_status.ccout_out_hiz_sts;
	}

	return EFI_SUCCESS;
}

EFI_STATUS EFI_PmicSchgGetConnectState
(
IN UINT32                       PmicDeviceIndex,
OUT  EFI_PM_SCHG_TYPEC_CONNECT_MODE_STATUS  *TypeCConnStatus
)
{
	pm_err_flag_type errFlag = PM_ERR_FLAG__SUCCESS;
	pm_schg_usb_typec_port_role_type port_role = PM_SCHG_USB_TYPEC_ROLE_NONE;

	if (!TypeCConnStatus)
	{
		return EFI_INVALID_PARAMETER;
	}

	errFlag = pm_schg_usb_typec_get_port_role(PmicDeviceIndex, &port_role);
	if (PM_ERR_FLAG__SUCCESS != errFlag)
	{
		return EFI_DEVICE_ERROR;
	}
	else
	{
		*TypeCConnStatus = (EFI_PM_SCHG_TYPEC_CONNECT_MODE_STATUS)port_role;
	}

	return EFI_SUCCESS;

}

EFI_STATUS EFI_PmicSchgGetHwJeitaStatus
(
IN UINT32                       PmicDeviceIndex,
OUT EFI_PM_SCHG_BATT_TEMP_STATUS  *pHwJeitaStatus
)
{
	pm_err_flag_type errFlag = PM_ERR_FLAG__SUCCESS;
	pm_schg_chgr_status_type chgr_status = { { 0 } };

	if (!pHwJeitaStatus)
	{
		return EFI_INVALID_PARAMETER;
	}

	errFlag = pm_schg_chgr_get_chgr_status(PmicDeviceIndex, &chgr_status);
	if (PM_ERR_FLAG__SUCCESS != errFlag)
	{
		return EFI_DEVICE_ERROR;
	}
	else
	{
		pHwJeitaStatus->jeitaHardHot = chgr_status.status_7.bat_temp_status_too_hot;
		pHwJeitaStatus->jeitaSoftHot = chgr_status.status_7.bat_temp_status_hot_soft;
		pHwJeitaStatus->jeitaHardCold = chgr_status.status_7.bat_temp_status_too_cold;
		pHwJeitaStatus->jeitaSoftCold = chgr_status.status_7.bat_temp_status_cold_soft;
	}

	return EFI_SUCCESS;

}

EFI_STATUS EFI_PmicSchgGetDCIrqStatus
(
IN  UINT32 						PmicDeviceIndex,
IN	EFI_PM_SCHG_DC_IRQ_TYPE		irq,
OUT BOOLEAN*					irq_status
)
{
  EFI_STATUS            Status = EFI_SUCCESS;
  pm_err_flag_type      errFlag = PM_ERR_FLAG__SUCCESS;
  boolean               st;

  if (NULL == irq_status)
  {
    Status = EFI_INVALID_PARAMETER;
  }
  else
  {
    /*get dcin irq status Read IRQ real time status*/
    errFlag = pm_schg_dc_irq_status(PmicDeviceIndex, (pm_schg_dc_irq_type)irq, PM_IRQ_STATUS_RT, &st);
    Status = (PM_ERR_FLAG__SUCCESS == errFlag) ? PM_ERR_FLAG__SUCCESS : EFI_DEVICE_ERROR;

    *irq_status = (st) ? TRUE : FALSE;
  }

  return Status;
}

EFI_STATUS EFI_EnDebugAccessoryMode
(
IN UINT32                       PmicDeviceIndex,
IN  BOOLEAN                EnDbgAccessSnk,
IN  BOOLEAN                EnFmb
)
{
	pm_err_flag_type errFlag = PM_ERR_FLAG__SUCCESS;

	/* Enable/Disable debug accessory mode and FMB */
	errFlag = pm_schg_typec_en_dbg_access_snk(PmicDeviceIndex, EnDbgAccessSnk);
	errFlag |= pm_schg_typec_en_fmb(PmicDeviceIndex, EnFmb);
	if (PM_ERR_FLAG__SUCCESS != errFlag)
	{
		return EFI_DEVICE_ERROR;
	}


	return EFI_SUCCESS;

}

EFI_STATUS EFI_GetDAMConnectSts
(
IN  UINT32                       PmicDeviceIndex,
OUT  BOOLEAN                *ConnectSts
)
{
	pm_err_flag_type errFlag = PM_ERR_FLAG__SUCCESS;

	/* Get DAM connect status */
	errFlag = pm_schg_typec_get_dam_connect_sts(PmicDeviceIndex, ConnectSts);
	
	if (PM_ERR_FLAG__SUCCESS != errFlag)
	{
		return EFI_DEVICE_ERROR;
	}

	return EFI_SUCCESS;

}

EFI_STATUS EFI_SetDAMIcl
(
IN  UINT32                       PmicDeviceIndex,
IN  DAM_SEL_ICL                icl_sel
)
{
	pm_err_flag_type errFlag = PM_ERR_FLAG__SUCCESS;

	/* Set the ICL for DAM */
	errFlag = pm_schg_typec_set_dam_icl(PmicDeviceIndex, (pm_schg_usb_typec_dam_icl_sel)icl_sel);
	if (PM_ERR_FLAG__SUCCESS != errFlag)
	{
		return EFI_DEVICE_ERROR;
	}

	return EFI_SUCCESS;

}

EFI_STATUS EFI_PmicSchgGetBattMissingStatus
(
IN  UINT32 						PmicDeviceIndex,
IN	EFI_PM_SCHG_BATT_MISSING_STATUS_TYPE		status_type,
OUT BOOLEAN*					status_val
)
{
  EFI_STATUS            Status = EFI_SUCCESS;
  pm_err_flag_type      errFlag = PM_ERR_FLAG__SUCCESS;
  boolean               st;

  if (NULL == status_val)
  {
    Status = EFI_INVALID_PARAMETER;
  }
  else
  {
    /*get batt missing status*/
    errFlag = pm_schg_batt_missing_status(PmicDeviceIndex, (pm_schg_batif_bat_miss_status_type)status_type, &st);
    Status = (PM_ERR_FLAG__SUCCESS == errFlag) ? PM_ERR_FLAG__SUCCESS : EFI_DEVICE_ERROR;

    *status_val = st;
  }

  return Status;
}

EFI_STATUS EFI_PmicSchgGetActivePort
(
OUT UINT8*					  active_pmic_index
)
{
  EFI_STATUS            Status = EFI_SUCCESS;
  pm_err_flag_type      errFlag = PM_ERR_FLAG__SUCCESS;
  UINT8                 port_index = 0;
  BOOLEAN               is_active = FALSE;

  if (NULL == active_pmic_index)
  {
    Status = EFI_INVALID_PARAMETER;
  }
  else
  {
    if (gChargerCount == 0)
    {
      //no charger or have not called pm_schg_chgr_get_schg_pmic_info
      errFlag = pm_schg_chgr_get_schg_pmic_info((UINT8 *)gSchgPmicIndex, (UINT8 *)gSchgSlaveIndex, &gChargerCount, MAX_CHARGING_PORT);

      if (PM_ERR_FLAG__SUCCESS != errFlag)
      {
        return EFI_DEVICE_ERROR;
      }
    }
    //starting from pmic_index 1 for the 1st port.
    *active_pmic_index = 1;
    for (port_index = 0; port_index < gChargerCount; port_index++)
    {
      errFlag = pm_schg_bob_is_port_active(gSchgPmicIndex[port_index], &is_active);
      if (PM_ERR_FLAG_FEATURE_NOT_SUPPORTED == errFlag)
      {
        return EFI_UNSUPPORTED;
      }
      if (is_active)
      {
        //found the first active port
        *active_pmic_index = gSchgPmicIndex[port_index];
        break;
      }
    }
  }

  return Status;
}

EFI_STATUS EFI_PmicSchgSetTestModeDischarging
(
IN  BOOLEAN           testmode_discharging
)
{
  pm_err_flag_type errFlag = PM_ERR_FLAG__SUCCESS;
  uint8 data = 0;
  /* Set the ICL for DAM */
  errFlag = pm_sdam_mem_read(PMIC_A, PM_SDAM_2, SDAM_MEM_036_ADDR, 1, &data);
  if (PM_ERR_FLAG__SUCCESS != errFlag)
  {
    return EFI_DEVICE_ERROR;
  }
  data = (data & 0xFE) | testmode_discharging; //bit 7:1 should not change.
  errFlag = pm_sdam_mem_write(PMIC_A, PM_SDAM_2, SDAM_MEM_036_ADDR, 1, &data);
  if (PM_ERR_FLAG__SUCCESS != errFlag)
  {
    return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;

}

EFI_STATUS EFI_PmicSchgSetShipMode
(
IN BOOLEAN enable
)
{
  pm_err_flag_type errFlag = PM_ERR_FLAG__SUCCESS;
  uint8 data = 0;
  errFlag = pm_sdam_mem_read(PMIC_A, PM_SDAM_2, SDAM_MEM_053_ADDR, 1, &data);
  if(PM_ERR_FLAG__SUCCESS != errFlag)
  {
    return EFI_DEVICE_ERROR;
  }
  data = (data & 0xFE) | enable;
  errFlag = pm_sdam_mem_write(PMIC_A, PM_SDAM_2, SDAM_MEM_053_ADDR, 1, &data);
  if(PM_ERR_FLAG__SUCCESS != errFlag)
  {
    return EFI_DEVICE_ERROR;
  }
  
  return EFI_SUCCESS;
}
EFI_STATUS EFI_PmicSchgGetShipMode
(
IN BOOLEAN *enable
)
{
  pm_err_flag_type errFlag = PM_ERR_FLAG__SUCCESS;
  uint8 data = 0;
  errFlag = pm_sdam_mem_read(PMIC_A, PM_SDAM_2, SDAM_MEM_053_ADDR, 1, &data);
  if(PM_ERR_FLAG__SUCCESS != errFlag)
  {
    return EFI_DEVICE_ERROR;
  }
  *enable = data & 0x1;

  return EFI_SUCCESS;
}

EFI_STATUS EFI_PmicSchgGetValidPonReasons
(
IN UINT64*					  pon_reason_arr
)
{
  pm_err_flag_type errFlag = PM_ERR_FLAG__SUCCESS;
  uint8 data = 0;
  uint8 ponhist_start_addr = 0;
  if (NULL == pon_reason_arr)
  {
    return EFI_INVALID_PARAMETER;
  }
  
  /*** Read SDAM Flag ***/
  errFlag |= pm_sdam_mem_read(0, PM_SDAM_2, SDAM_MEM_055_ADDR, 1, &data);
  
  /**** Check SDAM Flag and Read PON reasons for 1st/2nd latest reset ***/
  ponhist_start_addr = (TZ_RESET_DONE == data)? SDAM_MEM_048_ADDR: SDAM_MEM_040_ADDR;
  errFlag |= pm_sdam_mem_read(0, PM_SDAM_3, ponhist_start_addr, 8, (uint8*)pon_reason_arr);
  
  return (PM_ERR_FLAG__SUCCESS != errFlag) ? EFI_DEVICE_ERROR: EFI_SUCCESS;
}

EFI_STATUS EFI_PmicSchgSetOffModeSrc
(
IN UINT8 OffModeSrcCfg
)
{
  pm_err_flag_type errFlag = PM_ERR_FLAG__SUCCESS;

  errFlag = pm_sdam_mem_write(PMIC_A, PM_SDAM_2, SDAM_MEM_056_ADDR, 1, &OffModeSrcCfg);
  if(PM_ERR_FLAG__SUCCESS != errFlag)
  {
    return EFI_DEVICE_ERROR;
  }
  
  return EFI_SUCCESS;
}

EFI_STATUS EFI_PmicSchgSetOffModeSrcVbatThresh
(
IN UINT32 voltThHi,
IN UINT32 voltThLo
)
{
  pm_err_flag_type errFlag = PM_ERR_FLAG__SUCCESS;
  uint8 adcThHi = 0;
  uint8 adcThLo = 0;
    
  //convert voltage to adc code
  //since we only have one byte of SDAM register, round it up and write only MSB
  adcThHi = ((voltThHi * BTMTHR_COEFF0) / (BTMTHR_COEFF1 * BTMTHR_COEFF2) + 0x80) >> 8;
  adcThLo = ((voltThLo * BTMTHR_COEFF0) / (BTMTHR_COEFF1 * BTMTHR_COEFF2) + 0x80) >> 8;

  errFlag = pm_sdam_mem_write(PMIC_A, PM_SDAM_2, SDAM_MEM_057_ADDR, 1, &adcThHi);
  errFlag |= pm_sdam_mem_write(PMIC_A, PM_SDAM_2, SDAM_MEM_058_ADDR, 1, &adcThLo);

  if(PM_ERR_FLAG__SUCCESS != errFlag)
  {
    return EFI_DEVICE_ERROR;
  }
  
  return EFI_SUCCESS;
}
EFI_STATUS EFI_PmicSchgGetDVDDReset
(
OUT BOOLEAN* reset
)
{
  pm_err_flag_type errFlag = PM_ERR_FLAG__SUCCESS;
  UINT8 data = 0;

  if(reset == NULL)
  {
    return EFI_INVALID_PARAMETER;
  }
  
  *reset = FALSE;
  
  errFlag |= pm_sdam_mem_read(PMIC_A, PM_SDAM_2, SDAM_MEM_059_ADDR, 1, &data);
  if(PM_ERR_FLAG__SUCCESS != errFlag)
  {
    return EFI_DEVICE_ERROR;
  }

  if (data)
  {
    if (data == 0x80)
    {
        PMIC_DEBUG((EFI_D_WARN, "PmicDxe:: %a dvdd reset happened \r\n", __FUNCTION__));  
        *reset = TRUE;
    }
    //clear the flag
    data = 0;
    errFlag = pm_sdam_mem_write(PMIC_A, PM_SDAM_2, SDAM_MEM_059_ADDR, 1, &data);    
    if(PM_ERR_FLAG__SUCCESS != errFlag)
    {
      return EFI_DEVICE_ERROR;
    }
  }
     
  return EFI_SUCCESS;  
}
EFI_STATUS EFI_PmicSchgSetOS
(
IN EFI_PM_SCHG_OS_TYPE OS
)
{
    pm_err_flag_type errFlag = PM_ERR_FLAG__SUCCESS;
    uint8 data = 0;

    if(OS >= EFI_PM_SCHG_OS_INVALID)
    {
        return EFI_INVALID_PARAMETER;
    }
    
    errFlag = pm_sdam_mem_read(PMIC_A, PM_SDAM_2, SDAM_MEM_037_ADDR, 1, &data);
    if(PM_ERR_FLAG__SUCCESS != errFlag)
    {
      return EFI_DEVICE_ERROR;
    }
    data = (data & 0xFE) | OS;
    
    errFlag = pm_sdam_mem_write(PMIC_A, PM_SDAM_2, SDAM_MEM_037_ADDR, 1, &data);
    if(PM_ERR_FLAG__SUCCESS != errFlag)
    {
      return EFI_DEVICE_ERROR;
    }
    
    return EFI_SUCCESS;
}
EFI_STATUS EFI_PmicSchgSetReqMaxPwrCfg
(
IN UINT8 config
)
{
    pm_err_flag_type errFlag = PM_ERR_FLAG__SUCCESS;

    errFlag = pm_sdam_mem_write(PMIC_A, PM_SDAM_2, SDAM_MEM_061_ADDR, 1, &config);
    if(PM_ERR_FLAG__SUCCESS != errFlag)
    {
        return EFI_DEVICE_ERROR;
    }

    return EFI_SUCCESS;
}
EFI_STATUS EFI_PmicSchgGetReqMaxPwrCfg
(
OUT UINT8 *config
)
{
    pm_err_flag_type errFlag = PM_ERR_FLAG__SUCCESS;

    if(config == NULL)
    {
        return EFI_INVALID_PARAMETER;
    }

    errFlag = pm_sdam_mem_read(PMIC_A, PM_SDAM_2, SDAM_MEM_061_ADDR, 1, config);
    if(PM_ERR_FLAG__SUCCESS != errFlag)
    {
        return EFI_DEVICE_ERROR;
    }

    return EFI_SUCCESS;
}

/**
PMIC SCHG UEFI Protocol implementation
*/
EFI_QCOM_PMIC_SCHG_PROTOCOL PmicSchgProtocolImplementation = 
{
    PMIC_SCHG_REVISION,
    EFI_PmicSchgInit,
    EFI_PmicSchgEnableCharger,
    EFI_PmicSchgGetPowerPath, 
    EFI_PmicSchgIsBatteryPresent,
    EFI_PmicSchgGetChargerPortType,
    EFI_PmicSchgConfigOtg,
	EFI_PmicSchgSetOtgILimit,
    EFI_PmicSchgEnableAfpMode,
    EFI_PmicSchgSetInputPriority,
    EFI_PmicSchgSetFccMaxCurrent,
    EFI_PmicSchgSetFvMaxVoltage,
    EFI_PmicSchgEnableOtg,
    EFI_PmicSchgUsbSuspend,
    EFI_PmicSchgEnableJeita,
    EFI_PmicSchgGetOtgStatus,
    EFI_PmicSchgUsbinValid,    
    EFI_PmicSchgSetUsbMaxCurrent,
    EFI_PmicSchgChgrSourceReinserted,
    EFI_PmicSchgRerunAicl,
    //EFI_PmicSchgPrintAiclSts,
    EFI_PmicSchgDumpPeripheral,
    EFI_PmicSchgEnableChgWdog,
    EFI_PmicSchgPetChgWdog,
    EFI_PmicSchgGetChargingStatus,
    EFI_PmicSchgDCInValid,
    EFI_PmicSchgDCInSuspend,
    EFI_PmicSchgSetDCInPower,
    EFI_PmicSchgExit,
    EFI_PmicSchgSetUsbIclMode,
    EFI_PmicSchgGetChgWdogStatus,
    EFI_PmicSchgEnableHWJeita,
    EFI_PmicSchgToggleWipowerSDLatch,
    EFI_PmicSchgSetDcInCurrent,
    EFI_PmicSchgSetChargeCmdBit,
    EFI_PmicSchgGetPmicInfo,
    EFI_PmicSchgConfigApsd,
    EFI_PmicSchgConfigHvdcp,
    EFI_PmicSchgGetIclStatus,
    EFI_PmicSchgSetVconn,
    EFI_PmicSchgGetPortState,
    EFI_PmicSchgGetConnectState,
    EFI_PmicSchgGetHwJeitaStatus,
    EFI_PmicSchgGetDCIrqStatus,
    EFI_EnDebugAccessoryMode,
    EFI_GetDAMConnectSts,
    EFI_SetDAMIcl,
    EFI_PmicSchgGetBattMissingStatus,
    EFI_PmicSchgGetActivePort,
    EFI_PmicSchgSetTestModeDischarging,
    EFI_PmicSchgSetShipMode,
    EFI_PmicSchgGetValidPonReasons,
    EFI_PmicSchgSetOffModeSrc,
    EFI_PmicSchgSetOffModeSrcVbatThresh,
    EFI_PmicSchgGetDVDDReset,
    EFI_PmicSchgSetOS,
    EFI_PmicSchgSetReqMaxPwrCfg,
    EFI_PmicSchgGetReqMaxPwrCfg,
};

