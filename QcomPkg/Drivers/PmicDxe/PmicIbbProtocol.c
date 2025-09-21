/*! @file PmicIbbProtocol.c 

*  PMIC- IBB MODULE RELATED DECLARATION
*  This file contains functions and variable declarations to support 
*  the PMIC IBB module.
*
*  Copyright (c) 2014 Qualcomm Technologies, Inc.  All Rights Reserved. 
*  Qualcomm Technologies Proprietary and Confidential.
*/

/*===========================================================================

EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.


when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
06/24/14   al      Adding API to config IBB delay 
06/09/14   al      Arch update
05/14/14   al      New file.
===========================================================================*/

/*===========================================================================

INCLUDE FILES FOR MODULE

===========================================================================*/

#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>

#include "pm_uefi.h"
#include "pm_ibb.h"

#include <Protocol/EFIPmicIbb.h>


/*===========================================================================
EXTERNAL FUNCTION DECLARATIONS
===========================================================================*/

/**
EFI_PmicIbbLcdAmoledSel ()

@brief
Selects AMOLED or LCD
*/
EFI_STATUS
EFIAPI
EFI_PmicIbbLcdAmoledSel
(
    IN UINT32  PmicDeviceIndex,
    IN BOOLEAN EnAmoledMode
    )
{   
    EFI_STATUS                           Status  = EFI_SUCCESS;
    pm_err_flag_type                     errFlag = PM_ERR_FLAG__SUCCESS;
    
    errFlag = pm_ibb_lcd_amoled_sel((uint32)PmicDeviceIndex, (boolean)EnAmoledMode);
    if(PM_ERR_FLAG__SUCCESS != errFlag)
    {
        Status = EFI_DEVICE_ERROR;
    }
    return Status;
}


/**
EFI_PmicIbbModuleRdy ()

@brief
Makes module ready to be enabled
*/
EFI_STATUS
EFIAPI 
EFI_PmicIbbModuleRdy
(
    IN UINT32  PmicDeviceIndex,
    IN BOOLEAN IbbRdy
)
{   
    EFI_STATUS                           Status  = EFI_SUCCESS;
    pm_err_flag_type                     errFlag = PM_ERR_FLAG__SUCCESS;
    
    errFlag = pm_ibb_ibb_module_rdy((uint32)PmicDeviceIndex, (boolean)IbbRdy);
    if(PM_ERR_FLAG__SUCCESS != errFlag)
    {
        Status = EFI_DEVICE_ERROR;
    }
    return Status;
}


/**
EFI_PmicIbbConfigIbbCtrl()

@brief
Configures IBB control
*/
EFI_STATUS
EFIAPI 
EFI_PmicIbbConfigIbbCtrl
(
    IN UINT32  PmicDeviceIndex,
    IN BOOLEAN EnModule,
    IN BOOLEAN SwireRdy
)
{   
    EFI_STATUS                           Status  = EFI_SUCCESS;
    pm_err_flag_type                     errFlag = PM_ERR_FLAG__SUCCESS;
    
    errFlag = pm_ibb_config_ibb_ctrl((uint32)PmicDeviceIndex, (boolean)EnModule, (boolean)SwireRdy);
    if(PM_ERR_FLAG__SUCCESS != errFlag)
    {
        Status = EFI_DEVICE_ERROR;
    }

    return Status;
}


/**
EFI_PmicIbbSetSoftStrtChgrResistor ()

@brief
Sets charger resistor for soft start
*/
EFI_STATUS
EFIAPI
EFI_PmicIbbSetSoftStrtChgrResistor
(
    IN UINT32  PmicDeviceIndex,
    IN UINT32  KOhms
    )
{   
    EFI_STATUS                           Status  = EFI_SUCCESS;
    pm_err_flag_type                     errFlag = PM_ERR_FLAG__SUCCESS;
    
    errFlag = pm_ibb_set_soft_strt_chgr_resistor((uint32)PmicDeviceIndex,(uint32)KOhms);

    if(PM_ERR_FLAG__SUCCESS != errFlag)
    {
        Status = EFI_DEVICE_ERROR;
    }
    return Status;
}



/**
EFI_pmicibbSetSwireOutputPulse ()

@brief
Sets Swire output pulse voltage
*/
EFI_STATUS
EFIAPI
EFI_PmicIbbSetSwireOutputPulse
(
    IN UINT32  PmicDeviceIndex,
    IN UINT32  VoltInMilliVolt
    )
{   
    EFI_STATUS                           Status  = EFI_SUCCESS;
    pm_err_flag_type                     errFlag = PM_ERR_FLAG__SUCCESS;
    
    errFlag = pm_ibb_set_swire_output_pulse((uint32)PmicDeviceIndex, (uint32)VoltInMilliVolt);

    if(PM_ERR_FLAG__SUCCESS != errFlag)
    {
        Status = EFI_DEVICE_ERROR;
    }

    return Status;
}


/**
EFI_PmicIbbConfigOutputVolt ()

@brief
Configures output volt
*/
EFI_STATUS
EFIAPI
EFI_PmicIbbConfigOutputVolt
(
    IN UINT32   PmicDeviceIndex,
    IN BOOLEAN  OverrideOutputVolt,
    IN UINT32   OutputMillivolt
    )
{   
    EFI_STATUS                           Status  = EFI_SUCCESS;
    pm_err_flag_type                     errFlag = PM_ERR_FLAG__SUCCESS;
    
    errFlag = pm_ibb_config_output_volt((uint32)PmicDeviceIndex, (boolean)OverrideOutputVolt, (uint32)OutputMillivolt);

    if(PM_ERR_FLAG__SUCCESS != errFlag)
    {
        Status = EFI_DEVICE_ERROR;
    }


    return Status;
}


/**
EFI_PmicIbbGetStatus ()

@brief
Gets IBB status
*/
EFI_STATUS
EFIAPI
EFI_PmicIbbGetStatus
(
    IN  UINT32                   PmicDeviceIndex,
    OUT EFI_PM_IBB_STATUS_TYPE   *IbbStatus
    )
{   
    EFI_STATUS                           Status  = EFI_SUCCESS;
    pm_err_flag_type                     errFlag = PM_ERR_FLAG__SUCCESS;
    pm_ibb_status_type                   ibb_status;

    if(NULL == IbbStatus)
    {
        return EFI_DEVICE_ERROR;
    }

    errFlag = pm_ibb_get_ibb_status((uint32)PmicDeviceIndex, &ibb_status);

    if(PM_ERR_FLAG__SUCCESS != errFlag)
    {
        Status = EFI_DEVICE_ERROR;
    }
    else
    {
       IbbStatus->AmoledModeEn        = ibb_status.en_amoled_mode;      
       IbbStatus->IbbModuleRdy        = ibb_status.ibb_module_rdy;      
       IbbStatus->IbbModuleEn         = ibb_status.ibb_module_en;       
       IbbStatus->SwireRdy            = ibb_status.swire_rdy;           
       IbbStatus->ChgrResistorKohms   = ibb_status.chgr_resistor_kohms; 
       IbbStatus->Swire_pulse_mv      = ibb_status.swire_pulse_mv;      
       IbbStatus->OverrideOutputVolt  = ibb_status.override_output_volt;
       IbbStatus->OutputVolt          = ibb_status.output_volt;
       
       IbbStatus->IbbDelayConfig.EnPwrupDelay  = (BOOLEAN)(ibb_status.ibb_dly_config.en_pwrup_dly);
       IbbStatus->IbbDelayConfig.PwrupDelayMs  = ibb_status.ibb_dly_config.pwrup_dly_ms;
       IbbStatus->IbbDelayConfig.PwrupDelaySrc = (EFI_PM_IBB_PWRUP_DLY_SRC_TYPE)(ibb_status.ibb_dly_config.pwrup_dly_src);
       IbbStatus->IbbDelayConfig.PwrdnDelayMs  = ibb_status.ibb_dly_config.pwrdn_dly_ms;
       IbbStatus->IbbDelayConfig.PwrdnDelaySrc = (EFI_PM_IBB_PWRDN_DLY_SRC_TYPE)(ibb_status.ibb_dly_config.pwrdn_dly_src);

       IbbStatus->IbbVregOk            = ibb_status.ibb_vreg_ok;
       IbbStatus->ShortCircuit         = ibb_status.short_circuit;
       IbbStatus->IlimitError          = ibb_status.ilimit_error;
    }

    return Status;
}


/**
EFI_PmicConfigPwrupPwrdnDly ()

@brief
Config IBB delay
*/
EFI_STATUS
EFIAPI
EFI_PmicConfigIbbPwrupPwrdnDelay
(
    IN  UINT32                              PmicDeviceIndex,
    OUT EFI_PM_IBB_PWRUP_PWRDN_DELAY_TYPE   *DelayConfig
    )
{   
    EFI_STATUS                           Status  = EFI_SUCCESS;
    pm_err_flag_type                     errFlag = PM_ERR_FLAG__SUCCESS;
    pm_ibb_pwrup_pwrdn_dly_type          dly_config;

    if(NULL == DelayConfig)
    {
        return EFI_DEVICE_ERROR;
    }

    dly_config.pwrdn_dly_ms  = DelayConfig->PwrdnDelayMs;   
    dly_config.pwrdn_dly_src = (pm_ibb_pwrdn_dly_src_type) (DelayConfig->PwrdnDelaySrc);
    dly_config.pwrup_dly_ms  = DelayConfig->PwrupDelayMs;   
    dly_config.en_pwrup_dly  = (boolean) (DelayConfig->EnPwrupDelay); 
    dly_config.pwrup_dly_src = (pm_ibb_pwrup_dly_src_type) (DelayConfig->PwrupDelaySrc);

    errFlag = pm_ibb_config_pwrup_pwrdn_dly((uint32)PmicDeviceIndex, &dly_config);

    if(PM_ERR_FLAG__SUCCESS != errFlag)
    {
        Status = EFI_DEVICE_ERROR;
    }
    
    return Status;
}


/**
PMIC IBB UEFI Protocol implementation
*/
EFI_QCOM_PMIC_IBB_PROTOCOL PmicIbbProtocolImplementation = 
{
    PMIC_IBB_REVISION,     
    EFI_PmicIbbLcdAmoledSel,       
    EFI_PmicIbbModuleRdy,    
    EFI_PmicIbbConfigIbbCtrl, 
    EFI_PmicIbbSetSoftStrtChgrResistor,    
    EFI_PmicIbbSetSwireOutputPulse, 
    EFI_PmicIbbConfigOutputVolt,
    EFI_PmicIbbGetStatus,
    EFI_PmicConfigIbbPwrupPwrdnDelay,
};

