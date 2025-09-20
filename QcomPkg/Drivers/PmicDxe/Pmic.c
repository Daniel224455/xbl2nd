/* @file Pmic.c

  Implements the Pmic protocol
  
  Copyright (c) 2011-2018 Qualcomm Technologies, Inc.  All Rights Reserved. 
  Qualcomm Technologies Proprietary and Confidential.
*/


/*=============================================================================
                              EDIT HISTORY


 when         who     what, where, why
 --------     ---     -----------------------------------------------------------
 04/18/17     va      Moving target specific call to target file
 04/18/17     va      Added API call to set USB typeC port role in PmicInitialize based on HostModeSupported PCD
 08/19/16     pb      Enabled Test protocol (added QcomTargetLib.h for PRODMODE)
 03/24/16     al      Add keypress support
 03/13/16     ck      Enable PMIC NPA client support
 03/09/16     va      Moved common protocol install to pmic.c
 03/08/16     al      Checking for NpaDxe before initializing PAM   
 07/23/15     al      Refactoring code
 11/12/14     al      Adding core specific DAL initialization
 10/07/13     al      Moving platform config to target
 05/06/13     al      Storing pmic info and FabId
 04/01/13     sm      Added check for BattChgApp UEFI Variable
 03/07/13     dy      Add shutdown path for Liquid if pon trigger is charger
 11/06/12     dy      Add SMB350 Workaround
 10/25/12     al      Moved protocol installation to targets
=============================================================================*/


#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/QcomLib.h>
#include <Library/DebugLib.h>
#include <Library/SerialPortShLib.h>
#include "DALDeviceId.h"
#include "DALFramework.h"
#include "CoreVerify.h"

#include "pm_uefi.h"
#include "pm_pon.h"
#include "pm_appsbl_proc_npa.h"
#include "pm_app_pon.h"
#include "pm_core_utils.h"

/**
  PMIC PROTOCOL interface
 */

#include "Protocol/EFIPmicClkBuff.h"
#include "Protocol/EFIPmicVib.h"
#include "Protocol/EFIPmicWled.h"
#include "Protocol/EFIPmicGpio.h"
#include "Protocol/EFIPmicVreg.h"
#include "Protocol/EFIPmicLpg.h"
#include "Protocol/EFIPmicRTC.h"
#include "Protocol/EFIPmicPwrOn.h"
#include "Protocol/EFIPmicRgbLed.h"
#include "Protocol/EFIPmicVersion.h"
#include "Protocol/EFIPmicHaptics.h"
#include "Protocol/EFIPmicIbb.h"
#include "Protocol/EFIPmicAb.h"
#include "Protocol/EFIPmicOledb.h"
#include "Protocol/EFIPmicUsb.h"
#include "Protocol/EFIPmicLcdb.h"
#include "Protocol/EFIPmicPwm.h"
#include "Protocol/EFIPmicTest.h"

/*===========================================================================

                VARIABLES DEFINITIONS

===========================================================================*/
extern EFI_QCOM_PMIC_CLKBUFF_PROTOCOL   PmicClkBuffProtocolImplementation;
extern EFI_QCOM_PMIC_HAPTICS_PROTOCOL   PmicHapticsProtocolImplementation;
extern EFI_QCOM_PMIC_WLED_PROTOCOL      PmicWledProtocolImplementation;
extern EFI_QCOM_PMIC_GPIO_PROTOCOL      PmicGpioProtocolImplementation;
extern EFI_QCOM_PMIC_VREG_PROTOCOL      PmicVregProtocolImplementation;
extern EFI_QCOM_PMIC_LPG_PROTOCOL       PmicLpgProtocolImplementation;
extern EFI_QCOM_PMIC_RTC_PROTOCOL       PmicRtcProtocolImplementation;
extern EFI_QCOM_PMIC_PWRON_PROTOCOL     PmicPwronProtocolImplementation;
extern EFI_QCOM_PMIC_RGB_LED_PROTOCOL   PmicRgbLedProtocolImplementation;
extern EFI_QCOM_PMIC_VERSION_PROTOCOL   PmicVersionProtocolImplementation;
extern EFI_QCOM_PMIC_IBB_PROTOCOL       PmicIbbProtocolImplementation;
extern EFI_QCOM_PMIC_AB_PROTOCOL        PmicAbProtocolImplementation;
extern EFI_QCOM_PMIC_OLEDB_PROTOCOL     PmicOledbProtocolImplementation;
extern EFI_QCOM_PMIC_USB_PROTOCOL       PmicUsbProtocolImplementation;
extern EFI_QCOM_PMIC_GENERIC_PROTOCOL   PmicGenericProtocolImplementation;
extern EFI_QCOM_PMIC_LCDB_PROTOCOL      PmicLcdbProtocolImplementation;
extern EFI_QCOM_PMIC_PWM_PROTOCOL       PmicPwmProtocolImplementation;
extern EFI_QCOM_PMIC_TEST_PROTOCOL      PmicTestProtocolImplementation;

/*===========================================================================

                 LOCAL CONSTANT AND MACRO DEFINITIONS

===========================================================================*/

EFI_EVENT  ExitBootServicesEvent;

#define ARRAY_LENGTH(x)  (sizeof(x) / sizeof(x[0]))


/*----------------------------------------------------------------------------
 * Global Data Definitions
 * -------------------------------------------------------------------------*/
extern DALREG_DriverInfo DALSystem_DriverInfo;

static StringDevice gaDALModDriverList[] =
{
  { "/pmic/target", 0, 0, &DALSystem_DriverInfo, 0, NULL },
};


static DALSYSConfig gPmicDALSYSModConfig =
{
  // string device info
  { ARRAY_LENGTH(gaDALModDriverList), gaDALModDriverList },
  //numeric device info
  { 0, NULL }
};


/*----------------------------------------------------------------------------
 * Local Functions
 * -------------------------------------------------------------------------*/

VOID PmicExitBootServicesCallback(IN EFI_EVENT Event, IN VOID *Context)
{
  pm_exit_cleanup();
}

pm_err_flag_type pm_uefi_exit_init(void)
{
  EFI_STATUS Status = EFI_SUCCESS;
  Status = gBS->CreateEventEx(EVT_NOTIFY_SIGNAL,
                              TPL_CALLBACK,
                              PmicExitBootServicesCallback,
                              NULL,
                              &gEfiEventExitBootServicesGuid,
                              &ExitBootServicesEvent
                              );
  CORE_VERIFY(Status == EFI_SUCCESS);

  return PM_ERR_FLAG_SUCCESS;
}


static pm_err_flag_type pm_install_common_protocols(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS Status         = EFI_SUCCESS;
  pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;

  Status  = gBS->InstallMultipleProtocolInterfaces(
    &ImageHandle,
    &gQcomPmicGpioProtocolGuid,    &PmicGpioProtocolImplementation,
    &gQcomPmicVregProtocolGuid,    &PmicVregProtocolImplementation,
    &gQcomPmicLpgProtocolGuid,     &PmicLpgProtocolImplementation,
    &gQcomPmicClkBuffProtocolGuid, &PmicClkBuffProtocolImplementation,
    &gQcomPmicRtcProtocolGuid,     &PmicRtcProtocolImplementation,
    &gQcomPmicPwrOnProtocolGuid,   &PmicPwronProtocolImplementation,
    &gQcomPmicVersionProtocolGuid, &PmicVersionProtocolImplementation,
    &gQcomPmicWledProtocolGuid,    &PmicWledProtocolImplementation,
    &gQcomPmicIbbProtocolGuid,     &PmicIbbProtocolImplementation,
    &gQcomPmicAbProtocolGuid,      &PmicAbProtocolImplementation,
    &gQcomPmicOledbProtocolGuid,   &PmicOledbProtocolImplementation,
    &gQcomPmicRgbLedProtocolGuid,  &PmicRgbLedProtocolImplementation,
    &gQcomPmicUsbProtocolGuid,     &PmicUsbProtocolImplementation,
    &gQcomPmicHapticsProtocolGuid, &PmicHapticsProtocolImplementation,
    &gQcomPmicGenericProtocolGuid, &PmicGenericProtocolImplementation,
    &gQcomPmicLcdbProtocolGuid,    &PmicLcdbProtocolImplementation,
    &gQcomPmicPwmProtocolGuid,     &PmicPwmProtocolImplementation,
    &gQcomPmicDriverTestProtocolGuid, &PmicTestProtocolImplementation,
    NULL
    );

  err_flag = (Status == EFI_SUCCESS) ? PM_ERR_FLAG_SUCCESS : PM_ERR_FLAG_FAILURE;

  return err_flag;
}


static pm_err_flag_type pm_pmicdxe_init(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS Status         = EFI_SUCCESS;
  pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;

  Status |= pm_install_common_protocols(ImageHandle, SystemTable);
  Status |= pm_install_target_protocols(ImageHandle, SystemTable);

  if (EFI_SUCCESS == Status)
  {
    /*check for valid PON keypress if PMIC is present*/
    err_flag |= pm_app_pwrkey_long_press_check(PM_PON_PWRKEY_DBNC_CHK_AT_CORE);

    err_flag |= pm_uefi_exit_init();

    err_flag |= pm_post_pmic_initialization(ImageHandle, SystemTable);
  }
  else
  {
    err_flag = PM_ERR_FLAG_FAILURE;
  }

  return err_flag;
}


static pm_err_flag_type pm_pmicdxe_init_stubs(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS Status         = EFI_SUCCESS;
  pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
  EFI_PLATFORMINFO_PLATFORM_TYPE PlatformType;

  DEBUG((EFI_D_ERROR, "PmicDxe: PMIC was not detected \n\r"));

  GetPlatformType(&PlatformType);

  //if platform is not rumi then return failure
  if (PlatformType == EFI_PLATFORMINFO_TYPE_RUMI)
  {
    /*install protocols if RUMI with no PMIC*/
    Status |= pm_install_common_protocols(ImageHandle, SystemTable);
    Status |= pm_install_target_protocols(ImageHandle, SystemTable);
  }
  else
  {
    Status = EFI_DEVICE_ERROR;
  }

  err_flag = (Status == EFI_SUCCESS) ? PM_ERR_FLAG_SUCCESS : PM_ERR_FLAG_FAILURE;

  return err_flag;
}


/*----------------------------------------------------------------------------
 * Global Functions
 * -------------------------------------------------------------------------*/


/**
  PmicInitialize ()

  @brief
  Init implementation of EFI_QCOM_PMIC_CLIENT_DEVICE_PROTOCOL
 */
EFI_STATUS
PmicInitialize(
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable)
{
  EFI_STATUS Status = EFI_SUCCESS;

  pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;

  /* initialize DAL */
  DALSYS_InitMod(&gPmicDALSYSModConfig);


  /* Initialize the PMIC Library */
  err_flag = pm_init();

  if (err_flag == PM_ERR_FLAG_SUCCESS)
  {
    err_flag = pm_pmicdxe_init(ImageHandle, SystemTable);
  }
  else if (err_flag == PM_ERR_FLAG_FAILURE)
  {
    /*PMIC not detected*/
    err_flag = pm_pmicdxe_init_stubs(ImageHandle, SystemTable);
  }
  else
  {
    err_flag = PM_ERR_FLAG_FAILURE;
  }

  Status = (err_flag == PM_ERR_FLAG_SUCCESS) ? EFI_SUCCESS : EFI_DEVICE_ERROR;

  return Status;
}






