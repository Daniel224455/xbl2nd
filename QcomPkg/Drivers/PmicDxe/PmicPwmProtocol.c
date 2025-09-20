/*! @file PmicPwmProtocol.c 

 *  PMIC-PWM MODULE RELATED DECLARATION
 *  This file contains functions and variable declarations to support 
 *  the PMIC PWM (Pulse width modulation) module.
 *
 *  Copyright (c) 2012-2018 Qualcomm Technologies, Inc.  All Rights Reserved.
 *  Qualcomm Technologies Proprietary and Confidential.

 */

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.


when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
06/23/14   al      Added GetStatus for testing 
06/20/14   al      Updated coypright info 
06/09/14   al      Arch update
13/12/13   aa      PmicLib Dec Addition
03/26/13   al      Adding glitch removal. 
12/12/12   al      New file.
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include "pm_uefi.h"
#include "pm_pwm.h"

#include <Protocol/EFIPmicPwm.h>

/*===========================================================================
                  EXTERNAL FUNCTION DECLARATIONS
===========================================================================*/

/**
  EFI_PmicPwmConfig ()

  @brief
  PwmConfig implementation of EFI_QCOM_PMIC_PWM_PROTOCOL
 */
EFI_STATUS
EFIAPI
EFI_PmicPwmConfig
(
   IN UINT32                     PmicDeviceIndex,
   IN EFI_PM_PWM_RSRC_TYPE        PwmRsrc,
   IN UINT16                     PwmValue,
   IN UINT32                     PreDiv,
   IN UINT32                     PreDivExponent,
   IN EFI_PM_PWM_CLOCK_FREQ_TYPE ClkFreq,
   IN EFI_PM_PWM_BIT_SIZE_TYPE   BitSize
   )
{
   pm_err_flag_type errFlag = PM_ERR_FLAG_SUCCESS;

   /* Configure PWM Clock*/
   pm_pwm_clock_config(PmicDeviceIndex,
                       (pm_pwm_resource_type)PwmRsrc,
                       (pm_pwm_bit_size_type)BitSize,
                       (pm_pwm_clock_freq_type)ClkFreq,
                       (uint32)PreDiv,
                       (uint32)PreDivExponent
                       );
   if (PM_ERR_FLAG_SUCCESS != errFlag)
   {
      return errFlag;
   }

   /* enable PWM */
   pm_pwm_enable_pwm(PmicDeviceIndex,
                     (pm_pwm_resource_type)PwmRsrc,
                     (boolean)TRUE
                     );
   if (PM_ERR_FLAG_SUCCESS != errFlag)
   {
      return EFI_DEVICE_ERROR;
   }

   /* set PWM value */
   errFlag = pm_pwm_set_pwm_value(PmicDeviceIndex, (pm_pwm_resource_type)PwmRsrc, (uint16)PwmValue);
   if (PM_ERR_FLAG_SUCCESS != errFlag)
   {
      return EFI_DEVICE_ERROR;
   }

   return EFI_SUCCESS;
}

/**
  EFI_PmicPwmSetPwmValue ()

  @brief
  SetPwmValue implementation of EFI_QCOM_PMIC_PMIOC_PROTOCOL
 */
EFI_STATUS
EFIAPI
EFI_PmicPwmSetPwmValue
(
   IN UINT32                PmicDeviceIndex,
   IN EFI_PM_PWM_RSRC_TYPE   PwmRsrc,
   IN UINT16                PwmValue
   )
{
   pm_err_flag_type errFlag = PM_ERR_FLAG_SUCCESS;

   /* set PWM value */
   errFlag = pm_pwm_set_pwm_value(PmicDeviceIndex, (pm_pwm_resource_type)PwmRsrc, (uint16)PwmValue);
   if (PM_ERR_FLAG_SUCCESS != errFlag)
   {
      return EFI_DEVICE_ERROR;
   }

   return EFI_SUCCESS;
}


/**
  EFI_PmicPwmGlitchRemoval ()

  @brief
  GlitchRemoval implementation of EFI_QCOM_PMIC_PMIOC_PROTOCOL
 */
EFI_STATUS
EFIAPI
EFI_PmicPwmGlitchRemoval
(
   IN UINT32               PmicDeviceIndex,
   IN EFI_PM_PWM_RSRC_TYPE  PwmRsrc,
   IN BOOLEAN              Enable
   )
{
   pm_err_flag_type errFlag = PM_ERR_FLAG_SUCCESS;

   /* set PWM value */
   errFlag = pm_pwm_glitch_removal(PmicDeviceIndex, (pm_pwm_resource_type)PwmRsrc, (boolean)Enable);
   if (PM_ERR_FLAG_SUCCESS != errFlag)
   {
      return EFI_DEVICE_ERROR;
   }

   return EFI_SUCCESS;
}


/**
  EFI_PmicPwmGetStatus ()

  @brief
  PwmGetStatus implementation of EFI_QCOM_PMIC_PMIOC_PROTOCOL
 */
EFI_STATUS
EFIAPI
EFI_PmicPwmGetStatus
(
   IN UINT32                PmicDeviceIndex,
   IN EFI_PM_PWM_RSRC_TYPE   PwmRsrc,
   IN EFI_PM_PWM_STATUS_TYPE *PwmStatus
   )
{
   pm_err_flag_type errFlag = PM_ERR_FLAG_SUCCESS;
   pm_pwm_status_type pwm_status;

   if (NULL == PwmStatus)
   {
      return EFI_DEVICE_ERROR;
   }

   /* set PWM value */
   errFlag = pm_pwm_get_status(PmicDeviceIndex, (pm_pwm_resource_type)PwmRsrc, &pwm_status);
   if (PM_ERR_FLAG_SUCCESS != errFlag)
   {
      return EFI_DEVICE_ERROR;
   }
   else
   {
      PwmStatus->PwmEnable      = (BOOLEAN)pwm_status.pwm_enable;
      PwmStatus->PwmValue       = (UINT16)pwm_status.pwm_value;
      PwmStatus->BitSize        = (EFI_PM_PWM_BIT_SIZE_TYPE)pwm_status.bit_size;
      PwmStatus->ClkFreq        = (EFI_PM_PWM_CLOCK_FREQ_TYPE)pwm_status.clk_freq;
      PwmStatus->PreDiv         = (UINT32)pwm_status.pre_div;
      PwmStatus->Exp            = (UINT32)pwm_status.exp;
      PwmStatus->GlitchTemoval  = (BOOLEAN)pwm_status.glitch_removal;
   }

   return EFI_SUCCESS;
}

/**
  PMIC PWM UEFI Protocol implementation
 */
EFI_QCOM_PMIC_PWM_PROTOCOL PmicPwmProtocolImplementation =
{
   PMIC_PWM_REVISION,
   EFI_PmicPwmConfig,
   EFI_PmicPwmSetPwmValue,
   EFI_PmicPwmGlitchRemoval,
   EFI_PmicPwmGetStatus,
}; 
