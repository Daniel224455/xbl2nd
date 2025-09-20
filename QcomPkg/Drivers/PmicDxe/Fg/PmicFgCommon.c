/*! @file PmicFgCommon.c
 *
 *  PMIC-FG MODULE RELATED DECLARATION
 *  This file contains functions and variable declarations to support
 *  the PMIC FG commone file.
 *
 *  Copyright (c) 2016 - 2018 Qualcomm Technologies, Inc.  All Rights Reserved.
 *  Qualcomm Technologies Proprietary and Confidential.
 */

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.


when       who    what, where, why
--------   ---    ---------------------------------------------------------- 
08/01/18   ivy    Set batt them pull up and HW Jeita/AFP after battery profile is read
07/24/18   ivy	  Set rslow age register
06/13/18   cs	  remove rr-adc temp reading.
05/17/18   ivy	  Set Rslow shape and SoC reduction parameters, and support PM8150B v2.0 register map
03/21/18   cs     integrity bit and Rconn workaround.
03/15/18   ra     Read battery temp from ADC
02/22/18   cs     modify profile parser, set batt therm coefficient even when not load profile. 
03/01/18   ra     Remove obsolete code
01/29/18   cs     modified APIs to get batt id and batt temp.
11/27/17   ra     Removing unsupported API's
11/09/17   ra     Remove unsupported protocols
09/19/17   cs     for SDM855 remvoe unsupported function calls.
06/09/17   va     Detect the bad conditions of FG SRAM (trim registers) and issue VDD hard reset on PM/PMi to recover
06/07/17   cs     add configurable delay before enabling BMD
03/30/17   ll     Implemented level file-logging
03/15/17   cs     fix SBL 160 ms extra delay due to getting fresh batt id issue
01/27/17   ai     Fix use of retail
01/23/17   sm      CFG parameter Structure changes
12/19/16   sm     Added API to get FG PmicIndex
11/10/16   va     Removing SOC EMPTY IRQ
11/07/16   cs     Added API to disable ESR Pulse
10/14/16   va     update Aux setting 
10/11/16   va     Read profile on every boot, compare profile data vs sram data
10/06/16   cs     configuring batt therm/aux therm
09/20/16   sv     fixing klocwork issues
09/20/16   va     Enable Restarting FG 
07/26/16   va     Restarting FG Changes on warm boot 
06/23/16   va     Adding support for Charger Fg Peripheral dumps
06/14/16   va     Updating calculations for jeita thresholds, Reading IBat, Vbat from batt info
05/26/16   va     Update for Profile Load
05/24/16   sm     Added API calls to Enable FG Algorithm, BCL and GOC 
04/26/16   va     Adding parameters for Aux Coffes, SkinHot and Charger Hot settings
03/28/16   va     Consolidating config data into one struct 
03/21/16   va     New File.

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
/**
  EFI interfaces
 */
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Protocol/EFIPmicFg.h>
#include <Protocol/EFIPmicPwrOn.h>
#include <Protocol/EFIPlatformInfo.h>
#include <Protocol/EFIChipInfo.h>
#include <Library/QcomTargetLib.h>
#include <Library/FuseControlLib.h>

#include "DDIPlatformInfo.h"

/**
SPMI depedency
*/
#include "SpmiBus.h"

/**
  PMIC Lib interfaces
 */
#include "pm_uefi.h"
#include "PmicFileLog.h"
#include "PmicFgCommon.h"

#include "pm_fg_batt_soc.h"
#include "pm_fg_batt_info.h"
#include "pm_fg_batt_soc.h"
#include "pm_version.h"
#include "pm_schg_batif.h"
#include "pm_schg_chgr.h"
#include "pm_schg_misc.h"
#include "pm_fg_mem_if.h"
#include <../../Library/PmicLib/utils/inc/pm_utils.h>
#include "pm_err_flags.h"
#include "pm_core_utils.h"
#include "pm_fg_adc_rr.h"

#include <Protocol/EFIPmicPwrOn.h>

/**
  PMIC Fg interface
 */
#include <PmicFgBattProfile.h>
#include <PmicFgSram.h>

/**
QCom Lib dependency
*/
//#include <Library/QcomLib.h>
//#include <Library/MemoryAllocationLib.h>
#include "string.h"
#include <Library/BaseMemoryLib.h>

/**
  ADC Dependencies
 */
#include <Protocol/EFIAdc.h>

/**
  SChg interface
 */
#include "EFIPmicSchg.h"



/*===========================================================================

                        MACRO DEFINITIONS

===========================================================================*/
/* FG Peripheral dump */
#define PMIC_FG_PERIPHERAL_START_ADDR   0x4000
#define PMIC_FG_PERIPHERAL_NUM          6

#define INCREMENT_ONE                   1
#define DECREMENT_ONE                  -1

#define PMIC_FG_TEMP_CONV_BASE          30
#define PMIC_FG_TEMP_CONV_1_LSB         2  /* (-30 + (THRESHOLD * 0.5) C;)*/

#define PMIC_FG_BATT_ID_CONV_WAIT   10000 /* 10 msec */
/*max wait for 200 msec, this only will happen for high batt id with large capacitor. normally 10 msec is enough for hig batt id*/
#define PMIC_FG_MAX_BATT_ID_CONV_COUNT   20 

#define PMIC_FG_MAX_BMD_ENABLE_DELAY     500


/*===========================================================================

                        GLOBAL DATA DEFINITIONS

===========================================================================*/
EFI_PM_FG_CFGDATA_TYPE      gFgConfigData = {0};
STATIC UINT32               gFgPmicIndex  = 0, gFgSlaveIndex = 0;
extern EFI_GUID gQcomPmicPwrOnProtocolGuid;
STATIC EFI_ADC_PROTOCOL                     *AdcProtocol       = NULL;

/*===========================================================================

                        LOCAL FUNCTION DECLARATIONS

===========================================================================*/
VOID PmicFgPostExit(VOID);
EFI_STATUS PmicFgCommon_Init(UINT32 PmicDeviceIndex,  IN EFI_PM_FG_CFGDATA_TYPE FgCfgData);
EFI_STATUS PmicFgCommon_DisableBATTIDConversion(UINT32 PmicDeviceIndex);

/*===========================================================================

                        EXTERNAL FUNCTION DEFINITIONS

===========================================================================*/

/**
EFI_PmicFgExit()

@brief
Exit Fule Gauge Module
*/
EFI_STATUS
EFIAPI
EFI_PmicFgBasicExit
(
  UINT32 PmicDeviceIndex
)
{
  /* De-init interrupt, cancel timers etc..*/
  PMIC_DEBUG(( EFI_D_WARN, "PmicDxe:: %a \r\n", __FUNCTION__));

  PmicFgPostExit();

  return EFI_SUCCESS;
}

EFI_STATUS PmicFgCommon_DumpPeripheral( VOID )
{
  EFI_STATUS       Status  = EFI_SUCCESS;
  pm_err_flag_type errFlag = PM_ERR_FLAG_SUCCESS;

  errFlag = pm_fg_mem_if_get_fg_pmic_info((UINT8 *)&gFgPmicIndex, (UINT8 *)&gFgSlaveIndex);
  if(PM_ERR_FLAG_SUCCESS != errFlag)
  {
    PMIC_DEBUG((EFI_D_WARN, "PmicDxe:: %a get_fg_pmic_info  Error (%d) \n\r", __FUNCTION__, errFlag));
    return EFI_UNSUPPORTED;
  }

  errFlag =  pm_core_utils_dump_peripheral(gFgSlaveIndex, PMIC_FG_PERIPHERAL_START_ADDR, PMIC_FG_PERIPHERAL_NUM);
  if(PM_ERR_FLAG_SUCCESS != errFlag)
  {
    PMIC_DEBUG((EFI_D_WARN, "PmicDxe:: %a FG pm_core_utils_dump_peripheral Error (%d) \r\n", __FUNCTION__, errFlag));
  }

  return (Status | errFlag);
}

VOID PmicFgPostExit(VOID)
{
  EFI_CHIPINFO_PROTOCOL     *ChipInfoProtocol = NULL;
  EFIChipInfoIdType         eId = (EFIChipInfoIdType)0;
  gBS->LocateProtocol (&gEfiChipInfoProtocolGuid, NULL, (VOID **) &ChipInfoProtocol);
  if (NULL != ChipInfoProtocol)
  {
    ChipInfoProtocol->GetChipId(ChipInfoProtocol, &eId);
  }

  if ((eId == EFICHIPINFO_ID_MSM8994) && (PMIC_IS_PM8004 == pm_get_pmic_model(PM_DEVICE_2)))
  {
    PMIC_FILE_UART_DEBUG(( EFI_D_WARN, "******** UEFI IS NOT SUPPORTED ON DEVICE WITH PM8004 ******* \r\n"));
  }
}

/**
EFI_PmicFgExtendedExit()

@brief
Extended Exit for Fule Gauge Module
*/
EFI_STATUS
EFIAPI
EFI_PmicFgExtendedExit
(
  UINT32 PmicDeviceIndex
)
{
  EFI_PM_FG_BATT_STATUS_INFO BattStsInfo      = {0};
  //BOOLEAN                    bStatus          = FALSE;
  BOOLEAN                    bExceptionStatus = FALSE;

  PMIC_DEBUG(( EFI_D_WARN, "PmicDxe:: %a \r\n", __FUNCTION__));

  /* Make sure to release SRAM memory access since this is UEFI exit process */
  PmicFgSram_ReleaseFgSramImaAccess(PmicDeviceIndex, &bExceptionStatus);

  EFI_GetBatteryStatus(PmicDeviceIndex, &BattStsInfo);

  PMIC_DEBUG((EFI_D_WARN, "PmicDxe:: %a StateOfCharge:= %d \r\n", __FUNCTION__, BattStsInfo.StateOfCharge));

  PmicFgPostExit();

  return EFI_SUCCESS;
}

/**
EFI_PmicFgInit()

@brief
Initializes Fule Gauge Module
*/
EFI_STATUS
EFIAPI
EFI_PmicFgBasicInit
(
  UINT32                      PmicDeviceIndex,
  EFI_PM_FG_CFGDATA_TYPE     *pFgCfgData
)
{
  EFI_STATUS Status = EFI_SUCCESS;

  if(NULL == pFgCfgData )
  {
    return EFI_INVALID_PARAMETER;
  }

  /* Zero out global data */
  SetMem (&gFgConfigData, sizeof(EFI_PM_FG_CFGDATA_TYPE), 0x00);

  /* Assign global Config Data */
  CopyMemS(&gFgConfigData, sizeof(EFI_PM_FG_CFGDATA_TYPE), pFgCfgData, sizeof(EFI_PM_FG_CFGDATA_TYPE));

  Status |= PmicFgCommon_Init(PmicDeviceIndex, gFgConfigData);

  /* Enable after verification */
  EFI_PmicFgDumpPeripheral();

  return Status;
}


/**
EFI_PmicFgExtendedInit()

@brief
Initializes Fule Gauge Module with battery profile if profile loading is enabled
*/
EFI_STATUS
EFIAPI
EFI_PmicFgExtendedInit
(
  IN UINT32                        PmicDeviceIndex,
  OPTIONAL EFI_PM_FG_BATT_PROFILE_DATA  *pBattProfileData
)
{
  EFI_STATUS       Status  = EFI_SUCCESS;
  pm_err_flag_type errFlag = PM_ERR_FLAG_SUCCESS;
  EFI_PM_FG_PROFILE_PARSER_STATUS parseStatus = PM_PARSER_STATUS_GOOD_MATCH;
  //EFI_PM_FG_BATT_PROFILE_STATE    eProfileLoad = PM_FG_BATT_PROFILE_INVALID;
  BOOLEAN      bProfileMatch = TRUE;
  AdcRr_BattId Battery_Id    = {0};
  PM_FG_PROFILE_STATUS profileStatus = PROFILE_NOT_LOADED;
  UINT32 OptPullUp = 0;
  
  /* Keep for FG Extended init */
  PmicFgBattProfileInit(&gFgConfigData);

  Status = PmicFgSram_Init(PmicDeviceIndex, FG_SRAM_STATUS_INIT, &gFgConfigData);

  /* Parse profile on every boot to locate each profile data */
  parseStatus |= PmicBattProfileParseDataBuffer(pBattProfileData->pFileBuffer, pBattProfileData->DataSize);

  //Status |= PmicFgCommon_ProfileLoadNeeded(PmicDeviceIndex, &eProfileLoad);

  Status |= PmicFgSram_ReadIntegrityBit(PmicDeviceIndex, &profileStatus);

  /* Set and Pass Tolerance level */
  Battery_Id.BatteryIdTolerance = gFgConfigData.FgBattIdCfg.BatteryIdTolerance;

  Status |= PmicFgSram_VerifyProfileMatch(PmicDeviceIndex, Battery_Id, &bProfileMatch);
  if((errFlag != PM_ERR_FLAG_SUCCESS) || (Status != EFI_SUCCESS))
  {
    PMIC_DEBUG(( EFI_D_WARN, "PmicDxe:: %a Error Reading Profile Match = %d, Status = %r \r\n", __FUNCTION__, bProfileMatch, Status));
    return EFI_DEVICE_ERROR;
  }
  else
  {
    /* debug success case for test */
    PMIC_DEBUG(( EFI_D_WARN, "PmicDxe:: %a Success Reading Profile Match = %d, Status = %r \r\n", __FUNCTION__, bProfileMatch, Status));
  }

  /* Read batt them pull up from battery profile. Set registers and congiure HW JEITA accordingly */
  Status |= PmicFgBattProfileGetOptBattThermPullUp(&OptPullUp);
  Status |= PmicFgCommon_SetBattThermPullUp(PmicDeviceIndex, OptPullUp); 
  Status |= PmicFgCommon_ConfigHwJeita(PmicDeviceIndex, OptPullUp);

  if (EFI_ERROR(Status)){return EFI_DEVICE_ERROR;}  	
	
  if (((bProfileMatch == FALSE) && (profileStatus == PROFILE_UEFI_LOADED)) || (PROFILE_NOT_LOADED == profileStatus))
  {
    //only when profile not loaded, or loaded by UEFI and not match, will load profile, otherwise will not load profile.
    PMIC_DEBUG((EFI_D_WARN, "PmicDxe:: %a  profileStatus = %d \r\n", __FUNCTION__, profileStatus));
    Status |= PmicFgBattHandleProfile(PmicDeviceIndex);
  }
  else
  {
    PMIC_DEBUG((EFI_D_WARN, "PmicDxe:: %a  Battery profile loading skipped. Status = %r profileStatus = %d \r\n", __FUNCTION__, Status, profileStatus));
    /* As we do not need to load profile Do Conditional Restart here if condition for restart matches */
    if((EFI_SUCCESS == Status ) && (TRUE == gFgConfigData.FgCondRestart))
    {
      /* Restart FG if needed conditions matched */
      Status  |= PmicFgSram_CondRestart(PmicDeviceIndex, gFgConfigData);
      PMIC_DEBUG(( EFI_D_INFO, "PmicDxe:: %a  FG Cond Restart Sequence if VBattEstDiffThreshold = %d mv \r\n", __FUNCTION__, gFgConfigData.VBattEstDiffThreshold));
    }
    else
    {
      PMIC_DEBUG(( EFI_D_WARN, "PmicDxe:: %a Skipping FG Cond Restart Sequence flag = %d \r\n", __FUNCTION__, Status, gFgConfigData.FgCondRestart));
    }
  }

  // Disable rslow shape for FG 1.0 to prevent rslow flooring
  Status |= PmicFgSram_SetRslowCoeff(PmicDeviceIndex, FALSE);
  
  // Set standby current and slope limit back to default values in case system did not shutdown gracefully and reconfigured those values
  Status |= PmicFgSram_SetSocReductionCfg(PmicDeviceIndex);

  // Set rslow age charge and discharge for FG 2.0  	
  Status |= PmicFgSram_SetRslowAge(PmicDeviceIndex);

  return (Status | errFlag);

}

EFI_STATUS PmicFgCommon_ProfileLoadNeeded(UINT32 PmicDeviceIndex, EFI_PM_FG_BATT_PROFILE_STATE *eProfileState)
{
  EFI_STATUS       Status  = EFI_SUCCESS;
  pm_err_flag_type errFlag = PM_ERR_FLAG_SUCCESS;
  PM_FG_PROFILE_STATUS       profileStatus = PROFILE_NOT_LOADED;

  if(!eProfileState)
    return EFI_INVALID_PARAMETER;

  *eProfileState = PM_FG_BATT_PROFILE_INVALID;


  Status |= PmicFgSram_ReadIntegrityBit(PmicDeviceIndex, &profileStatus);

  if (PROFILE_NOT_LOADED == profileStatus)
  {
    *eProfileState = PM_FG_BATT_PROFILE_LOAD;
  }
  else
  {
    *eProfileState = PM_FG_BATT_PROFILE_RESTART;
    PMIC_DEBUG((EFI_D_WARN, "PmicDxe:: %a Status = %r, profileStatus = %d \r\n", __FUNCTION__, Status, profileStatus));
  }
  return (Status | errFlag);

}


EFI_STATUS PmicFgCommon_SetSkinAndChargerHotThreshold(EFI_PM_SKIN_CHARGERHOT_THRESHOLD *pSkinAndChargerHotThreshold)
{
  EFI_STATUS       Status = EFI_UNSUPPORTED;
  return Status;
}


/**
PmicFgCommon_Init()

@brief
Initializes Fule Gauge Module
*/
EFI_STATUS
PmicFgCommon_Init
(
  IN UINT32                     PmicDeviceIndex,
  IN EFI_PM_FG_CFGDATA_TYPE     FgCfgData
)
{
  EFI_STATUS       Status  = EFI_SUCCESS;
  pm_err_flag_type errFlag = PM_ERR_FLAG_SUCCESS;
  //pm_fg_batt_info_batt_temp_hyst_cfg hyst_cfg = PM_FG_BATT_INFO_NO_HYSTERESIS;
  STATIC EFI_QCOM_PMIC_PWRON_PROTOCOL *PmicPwronProtocol = NULL;
  EFI_PM_PON_REASON_TYPE PonReason;
  BOOLEAN          bBatteryRemoved = FALSE;

  SetMem (&PonReason, sizeof(EFI_PM_PON_REASON_TYPE), 0x00);

  /* Enable Fg Algo */
  pm_fg_batt_soc_enable_fg_algorithm(PmicDeviceIndex, TRUE);


  /* Init File log and UART log TBD enable after File log implementation */
  PmicFileLog_InitFileLog(FgCfgData.FgDbgCfg.PrintChargerAppDbgMsg, 
                          FgCfgData.FgDbgCfg.PrintChargerAppDbgMsgToFile,
                          FgCfgData.FgDbgCfg.FileLoggingLevelMask);

  /* Enable USB and DCIN PON triggers to be certain and avoid bad use case*/
  if (!PmicPwronProtocol)
  {
    Status |= gBS->LocateProtocol(&gQcomPmicPwrOnProtocolGuid, NULL, (VOID **)&PmicPwronProtocol);
  }

  if((Status == EFI_SUCCESS) && (NULL != PmicPwronProtocol))
  {
    /* For debug purpose */
    Status |= PmicPwronProtocol->GetPonReason(PM_DEVICE_0, &PonReason);
    PMIC_DEBUG((EFI_D_WARN, "PmicDxe:: %a PON reason Hard reset = %d PON1 = %d USB_CHG =%d KPDPWR = %d \r\n", __FUNCTION__, PonReason.HARD_RESET, 
                             PonReason.PON1, PonReason.USB_CHG, PonReason.KPDPWR));

  /* Battery was removed dtermine if it dvdd reset */
    Status |= PmicPwronProtocol->GetPonPblStatus(PM_DEVICE_0, EFI_PM_PWRON_PON_PBL_STATUS_XVDD_RB_OCCURRED,
                                                  &bBatteryRemoved);
    PMIC_DEBUG((EFI_D_WARN, "PmicDxe:: %a GetPonPblStatus bBatteryRemoved = %d \r\n", __FUNCTION__, bBatteryRemoved));
  }
  else
  {
    PMIC_DEBUG((EFI_D_WARN, "PmicDxe:: %a Error locating Pwron Protocol = %r \r\n", __FUNCTION__, Status));
  }
  return (Status | errFlag);
}

/**
EFI_PmicFgBatteryStatus()

@brief
Returns Battery Status parameters such as State of Charge (SOC)
*/
EFI_STATUS
EFIAPI
EFI_GetBatteryStatus
(
   IN  UINT32         PmicDeviceIndex,
   OUT EFI_PM_FG_BATT_STATUS_INFO *BattStsInfo
)
{
  EFI_STATUS Status   = EFI_SUCCESS;
  EFI_PM_FG_BATT_STATUS_INFO stsInfo = {0};

  if(NULL == BattStsInfo )
  {
    return EFI_INVALID_PARAMETER;
  }
  Status = PmicFgCommon_GetBatteryStatus(PmicDeviceIndex, &stsInfo);

  *BattStsInfo = stsInfo;

  return Status;
}


/**
EFI_GetBattProfileStatus()

@brief
Returns Battery profile Status parameters such as profile load status
*/
EFI_STATUS
EFIAPI
EFI_GetBattProfileStatus
(
   IN  UINT32         PmicDeviceIndex,
   OUT EFI_PM_FG_BATT_PROFILE_STATUS *pBattProfileStatus
)
{
  EFI_STATUS                   Status           = EFI_SUCCESS;
  EFI_PM_FG_BATT_PROFILE_STATE BattProfileState = {0};

  if(NULL == pBattProfileStatus)
  {
    return EFI_INVALID_PARAMETER;
  }
  Status |= PmicFgSram_ClearDmaLog(PmicDeviceIndex);

  Status |= PmicFgCommon_ProfileLoadNeeded(PmicDeviceIndex, &BattProfileState);
  if(EFI_SUCCESS == Status)
  {
    pBattProfileStatus->eProfileState = BattProfileState;
  }
  else
  {
    PMIC_DEBUG((EFI_D_ERROR, "PmicDxe:: %a: Failed to Profile Status = %r\r\n", __FUNCTION__, Status));
  }

  return Status;
}

EFI_STATUS PmicFgCommon_ReadBattTemp(INT32 *Batt_Temp)
{
	EFI_STATUS              Status = EFI_SUCCESS;
	EfiAdcDeviceChannelType channel;
	EfiAdcResultType        AdcResult = { 0 };
	static EFI_ADC_PROTOCOL       *pAdcProtocol = NULL;

	if (NULL == Batt_Temp)
	{
		return EFI_INVALID_PARAMETER;
	}
	if (!pAdcProtocol)
	{
		Status = gBS->LocateProtocol(&gEfiAdcProtocolGuid, NULL, (VOID**)&pAdcProtocol);
	}

	if ((EFI_ERROR(Status)) || (NULL == pAdcProtocol))
	{
		PMIC_DEBUG((EFI_D_ERROR, "PmicDxe:: %a: Failed to load ADC Protocol! Status = %r \r\n", __FUNCTION__, Status));
		return EFI_DEVICE_ERROR;
	}

	Status = pAdcProtocol->GetChannel(EFI_ADC_INPUT_BATT_THERM, sizeof(EFI_ADC_INPUT_BATT_THERM), &channel);
	if (EFI_ERROR(Status))
	{
		PMIC_DEBUG((EFI_D_ERROR, "PmicDxe:: %a: Failed to get ADC Channel for BATT_ID! Status = %r \r\n", __FUNCTION__, Status));
		return EFI_DEVICE_ERROR;
	}

	Status = pAdcProtocol->AdcRead(&channel, &AdcResult);
	if ((EFI_ERROR(Status)) || (EFI_ADC_RESULT_INVALID == AdcResult.eStatus))
	{
		PMIC_DEBUG((EFI_D_ERROR, "PmicDxe:: %a: Failed to read ADC Channel - BATT_ID! Status = %r \r\n", __FUNCTION__, Status));
		return EFI_DEVICE_ERROR;
	}
	*Batt_Temp = AdcResult.nPhysical;
	PMIC_DEBUG((EFI_D_ERROR, "PmicDxe:: %a: BATT_Temp = %d \r\n", __FUNCTION__, *Batt_Temp));
	return Status;

}


/**
PmicFgCommon_GetBatteryStatus

@brief
Returns Battery Status parameters such as State of Charge (SOC)
*/
EFI_STATUS
PmicFgCommon_GetBatteryStatus
(
   IN  UINT32         PmicDeviceIndex,
   OUT EFI_PM_FG_BATT_STATUS_INFO *BattStsInfo
)
{
  EFI_STATUS       Status   = EFI_SUCCESS;
  pm_err_flag_type errFlag  = PM_ERR_FLAG_SUCCESS;
  UINT32           Soc  = 0;
  UINT32           VBat = 0;
  INT32            ibat = 0;
  INT32            BattTemp = 0;

  if(NULL == BattStsInfo )
  {
    return EFI_INVALID_PARAMETER;
  }
  /* Get Battery SOC */
  errFlag =  pm_fg_batt_soc_get_monotonic_soc(PmicDeviceIndex, &Soc);
  Status = (PM_ERR_FLAG_SUCCESS == errFlag)? EFI_SUCCESS : EFI_DEVICE_ERROR;

  BattStsInfo->StateOfCharge = Soc;

  /* Get Battery voltage */
  errFlag |= pm_fg_batt_info_get_vbatt(PmicDeviceIndex, &VBat);
  Status = (PM_ERR_FLAG_SUCCESS == errFlag)? EFI_SUCCESS : EFI_DEVICE_ERROR;

  BattStsInfo->BatteryVoltage = VBat;

  /* Get Battery charge current */
  errFlag |= pm_fg_batt_info_get_ibatt(PmicDeviceIndex, &ibat);

  BattStsInfo->ChargeCurrent = ibat;

  errFlag |= PmicFgCommon_ReadBattTemp(&BattTemp);

  BattStsInfo->BatteryTemperature = BattTemp;


  return (Status | errFlag);
}


EFI_STATUS
EFIAPI
EFI_PmicFgGetBatteryId
(
  OUT EFI_PM_BATTERY_ID* BatteryId
)
{
  EFI_STATUS Status = EFI_SUCCESS;

  if(NULL == BatteryId)
  {
    Status = EFI_INVALID_PARAMETER;
  }
  else
  {
    Status = PmicFgCommon_GetBatteryId(BatteryId);
  }

  return Status;
}

EFI_STATUS PmicFgCommon_GetBatteryId(EFI_PM_BATTERY_ID* BatteryId)
{
  EFI_STATUS Status = EFI_SUCCESS;
  AdcRr_BattId Batt_Id = {0};
  if(NULL == BatteryId)
  {
    Status = EFI_INVALID_PARAMETER;
  }
  else
  {
    //Reaad for ADC RR when available
    Status = PmicFgSram_ReadBattID(gFgPmicIndex, &Batt_Id);
    if(EFI_SUCCESS == Status )
      BatteryId->BattId = Batt_Id.Bid;
    else
      BatteryId->BattId = (UINT64) 7500; //debug board  ID
  }

  return Status;
}


/**
EFI_PmicFgDumpPeripheral()

@brief
 Dump FG Pheripheral
 */
EFI_STATUS
EFIAPI
EFI_PmicFgDumpPeripheral( VOID )
{
  EFI_STATUS              Status         = EFI_SUCCESS;
  //EFI_PM_FG_CFGDATA_TYPE BatteryCfgData = {0};

  if(!RETAIL)
  {
    if((EFI_SUCCESS == Status )&& (TRUE == gFgConfigData.FgDbgCfg.EnableChargerFgDump))
    {
     Status = PmicFgCommon_DumpPeripheral();
    }
    else
    {
      /* PMIC_DEBUG(( EFI_D_WARN, "PmicDxe: EFI_PmicFgDumpPeripheral Disabled \r\n")); */
    }
  }
  else
  {
    return EFI_UNSUPPORTED;
  }

  return Status;
}

/**
EFI_PmicFgDumpFgSram()

@brief
 Dump FG SRAM
*/
EFI_STATUS
EFIAPI
EFI_PmicFgDumpFgSram( IN UINT32 PmicDeviceIndex)
{
  EFI_STATUS  Status = EFI_SUCCESS;
  UINT16 NoOfBytesToRead = 0;
  if(!RETAIL)
  {
    if(gFgConfigData.FgSramDbgCfg.DumpSram == TRUE )
    {
      NoOfBytesToRead = gFgConfigData.FgSramDbgCfg.DumpSramEndAddr - gFgConfigData.FgSramDbgCfg.DumpSramStartAddr + 1;
      PMIC_DEBUG(( EFI_D_WARN, "PmicDxe:: %a NoOfBytesToRead = %d \r\n",__FUNCTION__, NoOfBytesToRead));
      Status |= PmicFgSram_Dump(PmicDeviceIndex, gFgConfigData.FgSramDbgCfg.DumpSramStartAddr, NoOfBytesToRead);
      PMIC_DEBUG(( EFI_D_WARN, "PmicDxe:: %a Dump Completed = %r \r\n", __FUNCTION__, Status));
    }
    else
    {
      PMIC_DEBUG(( EFI_D_WARN, "PmicDxe:: %a Disabled \r\n", __FUNCTION__));
    }
  }

  return Status;
}




EFI_STATUS
EFIAPI
EFI_PmicFgConfigureHwJeita( UINT32 PmicDeviceIndex )
{
  /* Functionality not supported in FG module on PM855B, moved to CHGR peripheral */
  EFI_STATUS Status = EFI_UNSUPPORTED;

  return Status;
}

EFI_STATUS
EFIAPI
EFI_PmicFgGetHWJeitaStatus( UINT32 PmicDeviceIndex, EFI_PM_FG_BATT_TEMP_STATUS *pHwJeitaStatus)
{
  /* Functionality not supported in FG module on PM855B, moved to CHGR peripheral */
	EFI_STATUS Status = EFI_UNSUPPORTED;

  return Status;
}

EFI_STATUS
EFIAPI
EFI_PmicFgEnableESRPulse(UINT32 PmicDeviceIndex, BOOLEAN enable)
{
  EFI_STATUS       Status = EFI_SUCCESS;

  Status = PmicFgSram_EnableESRPulse(PmicDeviceIndex, enable);

  return Status;
}

/**
EFI_PmicFgBasicGetPmicInfo()

@brief
Provides PMIC Index on which FG resides
*/
EFI_STATUS
EFIAPI
EFI_PmicFgBasicGetPmicInfo
(
    OUT EFI_PM_FG_PMIC_INFO *FgPmicInfo
)
{
  pm_err_flag_type errFlag = PM_ERR_FLAG_SUCCESS;

  if(!FgPmicInfo)
  {
    return EFI_INVALID_PARAMETER;
  }

  errFlag = pm_fg_mem_if_get_fg_pmic_info((UINT8 *)&gFgPmicIndex, (UINT8 *)&gFgSlaveIndex);
  FgPmicInfo->PmicIndex = gFgPmicIndex;
  FgPmicInfo->SlaveIndex = gFgSlaveIndex;

  return (PM_ERR_FLAG_SUCCESS == errFlag) ? EFI_SUCCESS : EFI_DEVICE_ERROR;

}

EFI_STATUS PmicFgCommon_DisableBATTIDConversion(UINT32 PmicDeviceIndex)
{
  EFI_STATUS       Status = EFI_SUCCESS;
  pm_err_flag_type errFlag = PM_ERR_FLAG_SUCCESS;
  UINT32 count = 0;
  boolean batt_id_done = FALSE;
  EFI_ADC_PROTOCOL       *pAdcProtocol = NULL;

  /* disable continous mode */
  if (!pAdcProtocol)
  {
    Status = gBS->LocateProtocol(&gEfiAdcProtocolGuid,
             NULL,
             (VOID**)&pAdcProtocol);
  }

  if ((EFI_ERROR(Status)) || (NULL == pAdcProtocol))
  {
    PMIC_DEBUG((EFI_D_ERROR, "PmicDxe:: %a: Failed to load ADC Protocol! Status = %r \r\n", __FUNCTION__, Status));
    return EFI_DEVICE_ERROR;
  }
  pAdcProtocol->AdcSetFuelGaugeBattIdForced(FALSE);

  while (count < PMIC_FG_MAX_BATT_ID_CONV_COUNT)
  {
    /* check battery ID vavailabity status until it is 1. */
    errFlag |= pm_fg_adc_rr_irq_status(PmicDeviceIndex, PM_FG_ADC_RR_IRQ_BT_ID, PM_IRQ_STATUS_RT, &batt_id_done);
    if (batt_id_done)
      break;

    /* wait for 10 ms */
    gBS->Stall(PMIC_FG_BATT_ID_CONV_WAIT);
    count++;
  }
  //for battery ID with no capacitor, delay = 0, for batt id with 0.1 uF capacity, need to use 100 ms ddelay
  if (gFgConfigData.FgBattIdCfg.BattMissingDelay > PMIC_FG_MAX_BMD_ENABLE_DELAY)
  {
    gFgConfigData.FgBattIdCfg.BattMissingDelay = PMIC_FG_MAX_BMD_ENABLE_DELAY;
  }

  if (gFgConfigData.FgBattIdCfg.BattMissingDelay != 0)
  {
    gBS->Stall(gFgConfigData.FgBattIdCfg.BattMissingDelay * 1000);
  }
  
  return (Status | errFlag);
}


EFI_STATUS PmicFgCommon_ConfigHwJeita(UINT32 PmicDeviceIndex, UINT32 OptPullUp)
{
	EFI_STATUS Status = EFI_SUCCESS;
  

	Status |= PmicFgCommon_AdcScaleJeitaThresholds(OptPullUp);
	Status |= EFI_PmicSchgEnableHWJeita(PmicDeviceIndex, TRUE);
	
	PMIC_DEBUG(( EFI_D_WARN, "PmicDxe: %a Enabled HW Jeita. Status = %r \r\n", __FUNCTION__, Status));

	return Status;	
}

EFI_STATUS PmicFgCommon_AdcScaleJeitaThresholds(UINT32 OptPullUp)
{
  EFI_STATUS Status 		  =  EFI_SUCCESS;
  EfiAdcDeviceChannelType	  AdcChannel  = {0};
  EfiAdcResultType			  AdcResult   = {0};
  
    
  /* Based on the pull up value, use ADC to scale the JEITA and AFP values */
  /* Locate ADC Protocol */
  if (!AdcProtocol)
  {
	Status |= gBS->LocateProtocol(&gEfiAdcProtocolGuid,
	  NULL,
	  (VOID **)&AdcProtocol);
  }
  if ((EFI_SUCCESS == Status) && (NULL != AdcProtocol))
  {
	  if (OptPullUp == BATT_THERM_PULL_UP_LOW)
	  {
		  Status = AdcProtocol->GetChannel(ADC_INPUT_BATT_THERM_PU_30K, sizeof(ADC_INPUT_BATT_THERM_PU_30K), &AdcChannel);		  
		  if (EFI_SUCCESS != Status)
		  {
			  PMIC_DEBUG((EFI_D_WARN, "PmicDxe:: %a Failed to get the channel for ADC_INPUT_BATT_THERM_PU_30K : 0x%08X \r\n", __FUNCTION__, Status));
			  return Status;
		  }
  
	  }
	  else if (OptPullUp == BATT_THERM_PULL_UP_HIGH)
	  {
		  Status = AdcProtocol->GetChannel(ADC_INPUT_BATT_THERM_PU_400K, sizeof(ADC_INPUT_BATT_THERM_PU_400K), &AdcChannel);
		  if (EFI_SUCCESS != Status)
		  {
			  PMIC_DEBUG((EFI_D_WARN, "PmicDxe:: %a Failed to get the channel for ADC_INPUT_BATT_THERM_PU_400K : 0x%08X \r\n", __FUNCTION__, Status));
			  return Status;
		  }
	  }
	  else
	  {
		  Status = AdcProtocol->GetChannel(ADC_INPUT_BATT_THERM, sizeof(ADC_INPUT_BATT_THERM), &AdcChannel);
		  if (EFI_SUCCESS != Status)
		  {
			  PMIC_DEBUG((EFI_D_WARN, "PmicDxe:: %a Failed to get the channel for ADC_INPUT_BATT_THERM : 0x%08X \r\n", __FUNCTION__, Status));
			  return Status;
		  }
	  }
  
	/* ADC scaled code for Jeita Hard Hot Limit */
	Status = AdcProtocol->AdcProcessConversionResultInverse(&AdcChannel, gSchgCfgData.SchgJeitaData.JeitaHardHotLimit, &AdcResult);
	
	if (EFI_SUCCESS != Status)
	{
	  PMIC_DEBUG((EFI_D_WARN, "PmicDxe:: %a Failed to get ADC scaling code for JEITA hard hot limit : 0x%08X \r\n", __FUNCTION__, Status));
	  return Status;
	}
  	gSchgCfgData.SchgJeitaData.JeitaHardHotLimit = AdcResult.nCode;
  
	/* ADC scaled code for Jeita Hard Cold Limit */
	Status = AdcProtocol->AdcProcessConversionResultInverse(&AdcChannel, gSchgCfgData.SchgJeitaData.JeitaHardColdLimit, &AdcResult);
	if (EFI_SUCCESS != Status)
	{
	  PMIC_DEBUG((EFI_D_WARN, "PmicDxe:: %a Failed to get ADC scaling code for JEITA hard cold limit : 0x%08X \r\n", __FUNCTION__, Status));
	  return Status;
	}
	gSchgCfgData.SchgJeitaData.JeitaHardColdLimit = AdcResult.nCode;
  
	/* ADC scaled code for Jeita Soft Hot Limit */
	Status = AdcProtocol->AdcProcessConversionResultInverse(&AdcChannel, gSchgCfgData.SchgJeitaData.JeitaSoftHotLimit, &AdcResult);
	if (EFI_SUCCESS != Status)
	{
	  PMIC_DEBUG((EFI_D_WARN, "PmicDxe:: %a Failed to get ADC scaling code for JEITA soft hot limit : 0x%08X \r\n", __FUNCTION__, Status));
	  return Status;
	}
	gSchgCfgData.SchgJeitaData.JeitaSoftHotLimit = AdcResult.nCode;
  
	/* ADC scaled code for Jeita Soft Cold Limit */
	Status = AdcProtocol->AdcProcessConversionResultInverse(&AdcChannel, gSchgCfgData.SchgJeitaData.JeitaSoftColdLimit, &AdcResult);
	if (EFI_SUCCESS != Status)
	{
	  PMIC_DEBUG((EFI_D_WARN, "PmicDxe:: %a Failed to get ADC scaling code for JEITA soft cold limit : 0x%08X \r\n", __FUNCTION__, Status));
	  return Status;
	}
	gSchgCfgData.SchgJeitaData.JeitaSoftColdLimit = AdcResult.nCode;
  
	/* ADC scaled code for AFP Hot Limit */
	Status = AdcProtocol->AdcProcessConversionResultInverse(&AdcChannel, gSchgCfgData.HwAfpHotThreshold, &AdcResult);
	if (EFI_SUCCESS != Status)
	{
	  PMIC_DEBUG((EFI_D_WARN, "PmicDxe:: %a Failed to get ADC scaling code for HW AFP Hot limit : 0x%08X \r\n", __FUNCTION__, Status));
	  return Status;
	}
	gSchgCfgData.HwAfpHotThreshold = AdcResult.nCode;
  
	/* ADC scaled code for AFP Cold Limit */
	Status = AdcProtocol->AdcProcessConversionResultInverse(&AdcChannel, gSchgCfgData.HwAfpColdThreshold, &AdcResult);
	if (EFI_SUCCESS != Status)
	{
	  PMIC_DEBUG((EFI_D_WARN, "PmicDxe:: %a Failed to get ADC scaling code for HW AFP Cold limit : 0x%08X \r\n", __FUNCTION__, Status));
	  return Status;
	}
	gSchgCfgData.HwAfpColdThreshold = AdcResult.nCode;
  
  }
    
  return Status;

}
EFI_STATUS PmicFgCommon_SetBattThermPullUp(UINT32 PmicDeviceIndex, UINT32 OptThermPullUp )
{
  EFI_STATUS  							Status  = EFI_SUCCESS;
  pm_err_flag_type						err_flag = PM_ERR_FLAG_SUCCESS;
  pm_schg_batif_adc_pull_up_res_type	OptPmSchgPullup;
  pm_fg_batt_therm_pull_up_res			OptPmFgPullup;

  switch(OptThermPullUp)
  {
  	case BATT_THERM_PULL_UP_LOW:
		OptPmSchgPullup = PM_SCHG_BATIF_PULL_UP_RES_30K;
		OptPmFgPullup = PM_FG_THERM_RBIAS_30K;
		break;
		
	case BATT_THERM_PULL_UP_HIGH:
		OptPmSchgPullup = PM_SCHG_BATIF_PULL_UP_RES_400K;
		OptPmFgPullup = PM_FG_THERM_RBIAS_400K;
		break;
		
	default:
	  	OptPmSchgPullup = PM_SCHG_BATIF_PULL_UP_RES_100K;
		OptPmFgPullup = PM_FG_THERM_RBIAS_100K;
		break;
  }
  

  /* Write pull up to PMIC SCHG*/
  err_flag = pm_schg_batif_set_adc_internal_pull_up(PmicDeviceIndex, PM_SCHG_BATIF_PULL_UP_BAT_THM, OptPmSchgPullup);

  /* Write pull up to PMIC FG */
  err_flag |= pm_fg_set_batt_therm_pull_up(PmicDeviceIndex, OptPmFgPullup);

  
  PMIC_DEBUG(( EFI_D_WARN, "PmicDxe: %a Set Batt Therm Pull up %dk. Status = %d \r\n", 
		__FUNCTION__, OptThermPullUp, err_flag));

  return (Status | err_flag);
}

