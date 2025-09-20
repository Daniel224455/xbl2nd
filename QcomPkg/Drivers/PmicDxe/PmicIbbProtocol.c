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
EFI_PmicIbbConfigOutputVolt ()

@brief
Configures output volt
*/
EFI_STATUS
EFIAPI
EFI_PmicIbbConfigOutputVolt
(
    IN UINT32   PmicDeviceIndex,
    IN UINT32   Millivolt
    )
{   
    EFI_STATUS                           Status  = EFI_SUCCESS;
    pm_err_flag_type                     errFlag = PM_ERR_FLAG__SUCCESS;
    
    errFlag = pm_ibb_config_output_volt((uint32)PmicDeviceIndex, (uint32)Millivolt);

    if(PM_ERR_FLAG__SUCCESS != errFlag)
    {
        Status = EFI_DEVICE_ERROR;
    }

    return Status;
}

/**
EFI_PmicIbbConfigDefaultVolt ()

@brief
Configures output volt
*/
EFI_STATUS
EFIAPI
EFI_PmicIbbConfigDefaultVolt
(
    IN UINT32   PmicDeviceIndex,
    IN UINT32   Millivolt
    )
{   
    EFI_STATUS                           Status  = EFI_SUCCESS;
    pm_err_flag_type                     errFlag = PM_ERR_FLAG__SUCCESS;
    
    errFlag = pm_ibb_config_default_volt((uint32)PmicDeviceIndex, (uint32)Millivolt);

    if(PM_ERR_FLAG__SUCCESS != errFlag)
    {
        Status = EFI_DEVICE_ERROR;
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
    IN  UINT32                    PmicDeviceIndex,
	IN  EFI_PM_IBB_DLY_AMNT_TYPE  PwrupDly,
	IN  EFI_PM_IBB_DLY_AMNT_TYPE  PwrdnDly
    )
{   
    EFI_STATUS                           Status  = EFI_SUCCESS;
    pm_err_flag_type                     errFlag = PM_ERR_FLAG__SUCCESS;
    	
	errFlag = pm_ibb_config_pwrup_pwrdn_dly(PmicDeviceIndex, (pm_ibb_dly_amnt_type)PwrupDly, (pm_ibb_dly_amnt_type)PwrdnDly);

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
       IbbStatus->IbbModuleRdy = ibb_status.ibb_module_rdy;      
       IbbStatus->IbbModuleEn  = ibb_status.ibb_module_en;       
       IbbStatus->SwireRdy     = ibb_status.swire_rdy;  
       IbbStatus->OutputVolt   = ibb_status.output_volt;
	   IbbStatus->DefaultVolt  = ibb_status.default_volt;
       
       IbbStatus->PwrupDly  = (EFI_PM_IBB_DLY_AMNT_TYPE)ibb_status.pwrup_dly;
       IbbStatus->PwrdnDly  = (EFI_PM_IBB_DLY_AMNT_TYPE)ibb_status.pwrdn_dly;
	   
       IbbStatus->IbbVregOk    = ibb_status.ibb_vreg_ok;
       IbbStatus->Overload     = ibb_status.overload;
       IbbStatus->IlimitError  = ibb_status.ilimit_error;
       IbbStatus->AmoledModeEn  = FALSE;
    }

    return Status;
}




/**
PMIC IBB UEFI Protocol implementation
*/
EFI_QCOM_PMIC_IBB_PROTOCOL PmicIbbProtocolImplementation = 
{
    PMIC_IBB_REVISION,        
    EFI_PmicIbbModuleRdy,    
    EFI_PmicIbbConfigIbbCtrl, 
    EFI_PmicIbbConfigOutputVolt,
	EFI_PmicIbbConfigDefaultVolt,    
    EFI_PmicConfigIbbPwrupPwrdnDelay,
	EFI_PmicIbbGetStatus,
};

