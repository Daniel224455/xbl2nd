/** @file NvmeCoreTargetLib.c

  Target specific configuration Items for NVMe

  Copyright (c) 2018 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential

**/

/*=============================================================================
                              EDIT HISTORY

when         who     what, where, why
----------   ---     -----------------------------------------------------------
2018-12-04   wek     Turn on NVMe power rail for Loader.

=============================================================================*/

#include <Library/BaseLib.h>
#include "NvmeBsp.h"
#include "pm_smps.h"
#include "pm_gpio.h"
#include "pm_version.h"

INTN nvme_core_target_power_on(UINT32 slot)
{
  pm_err_flag_type err_flag = PM_ERR_FLAG__SUCCESS;

  UINT32 PmicIndex = 2;
  pm_gpio_perph_index GpioNum = PM_GPIO_11;
  pm_gpio_out_buffer_config_type OutBuffConfig = PM_GPIO_OUT_BUFFER_CONFIG_CMOS;
  pm_gpio_volt_src_type VSrc = PM_GPIO_VIN1;
  pm_gpio_src_config_type Source = PM_GPIO_SOURCE_GND;
  pm_gpio_out_buffer_drv_strength_type BufferStrength = PM_GPIO_OUT_BUFFER_HIGH;
  BOOLEAN inversion = 1;
  int result = 0;

  err_flag = pm_smps_sw_enable(PMIC_C, PM_SMPS_8, PM_ON, TRUE);
  if (PM_ERR_FLAG__SUCCESS != err_flag)
    result = -1;

  err_flag = pm_gpio_config_digital_output(PmicIndex, GpioNum, OutBuffConfig ,
                                           VSrc,      Source,  BufferStrength,
                                           inversion);
  if (PM_ERR_FLAG__SUCCESS != err_flag)
    result = -1;

  return result;
}

