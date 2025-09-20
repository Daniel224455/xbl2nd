/*! @file PmicRgbLed.c 

 *  PMIC-RGB LED MODULE RELATED DECLARATION
 *  This file contains functions and variable declarations to support 
 *  the PMIC RGB Led module.
 *
 *  Copyright (c) 2012 - 2018  by QUALCOMM Technologies, Inc.  All Rights Reserved.
 */

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.


when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
08/05/16   va      Adding Charger led toggle protocol
07/01/14   al      Adding get_status 
06/20/14   al      Updated coypright info 
06/09/14   al      Arch update
13/12/13   aa      PmicLib Dec Addition
03/15/13   al	   Adding for 8974
08/09/12   sm      New file.
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
/**
  EFI interfaces
 */
#include <Library/UefiLib.h>

/**
  PMIC Lib interfaces
 */
#include "pm_uefi.h"
#include "pm_app_rgb.h"

/**
  PMIC RGB LED interface
 */
#include "Protocol/EFIPmicRgbLed.h"

/*===========================================================================
                  EXTERNAL FUNCTION DECLARATIONS
===========================================================================*/
EFI_STATUS
EFIAPI
EFI_PmicRgbLedEnableLedDriver
(
   IN UINT32                          PmicDeviceIndex,
   IN EFI_PM_RGB_WHICH_TYPE           Rgb,
   IN UINT8                           Color,
   IN BOOLEAN                         Enable
   )
{
    pm_err_flag_type  errFlag = PM_ERR_FLAG_SUCCESS;

    errFlag = pm_rgb_enable(PmicDeviceIndex, (pm_rgb_which_type)Rgb, Color, (boolean)Enable);
    if(PM_ERR_FLAG_SUCCESS != errFlag)
    {
        return EFI_DEVICE_ERROR;
    }

    return EFI_SUCCESS;
}



EFI_STATUS
EFIAPI
EFI_PmicRgbLedGetStatus
(
   IN  UINT32                          PmicDeviceIndex,
   IN  EFI_PM_RGB_WHICH_TYPE           Rgb,
   OUT EFI_PM_RGB_STATUS_TYPE          *RgbStatus
   )
{
    EFI_STATUS                           Status  = EFI_SUCCESS;
    pm_err_flag_type                     errFlag = PM_ERR_FLAG_SUCCESS;
    pm_rgb_status_type                   rgb_status;

    if(NULL == RgbStatus)
    {
        return EFI_DEVICE_ERROR;
    }

    errFlag = pm_rgb_get_status((uint32)PmicDeviceIndex, (pm_rgb_which_type)Rgb, &rgb_status);

    if(PM_ERR_FLAG_SUCCESS != errFlag)
    {
        Status = EFI_DEVICE_ERROR;
    }
    else
    {
        RgbStatus->EnabledRgbMask     = rgb_status.enabled_mask;
    }

    return Status;
}


EFI_STATUS
EFIAPI
EFI_PmicConfigRgbLed
(
  IN  UINT32                          PmicDeviceIndex,
  IN  EFI_PM_RGB_WHICH_TYPE           Rgb,
  IN  UINT32                          rgb_mask,
  IN  UINT32                          dim_level,
  IN  BOOLEAN                         enable_rgb
  )
{
  pm_err_flag_type  errFlag = PM_ERR_FLAG_SUCCESS;

  pm_rgb_state_type rgb_state = (enable_rgb) ? PM_RGB_STATE_GLOW : PM_RGB_STATE_OFF;

  pm_app_rgb_config_type rgb_config =
                                      {
                                        .pmic_index = PmicDeviceIndex,
                                        .rgb_state  = rgb_state,
                                        .rgb_mask   = rgb_mask,
                                        .dim_level  = dim_level
                                      }; 

  errFlag |= pm_app_rgb_glow(&rgb_config);

  if(PM_ERR_FLAG_SUCCESS != errFlag)
  {
    return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;
}



/**
  PMIC RGB Led UEFI Protocol implementation
 */
EFI_QCOM_PMIC_RGB_LED_PROTOCOL PmicRgbLedProtocolImplementation =
{
    PMIC_RGB_LED_REVISION,
    EFI_PmicRgbLedEnableLedDriver,
    EFI_PmicRgbLedGetStatus,
    EFI_PmicConfigRgbLed,
};
