/*! @file PmicAbProtocol.c 

*  PMIC- AB MODULE RELATED DECLARATION
*  This file contains functions and variable declarations to support 
*  the PMIC AB module.
*
*  Copyright (c) 2018 Qualcomm Technologies, Inc.  All Rights Reserved.
*  Qualcomm Technologies Proprietary and Confidential.
*/

/*===========================================================================

EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.


when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/13/18   al      New file.
===========================================================================*/

/*===========================================================================

INCLUDE FILES FOR MODULE

===========================================================================*/

#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>

#include "pm_uefi.h"
#include "pm_ab.h"

#include <Protocol/EFIPmicAb.h>

/*===========================================================================
EXTERNAL FUNCTION DECLARATIONS
===========================================================================*/



/**
EFI_PmicAbModuleRdy ()

@brief
Make module ready to be enabled
*/
EFI_STATUS 
EFIAPI 
EFI_PmicAbModuleRdy           
(
   IN UINT32 PmicDeviceIndex, 
   IN BOOLEAN AbRdy
)
{
    EFI_STATUS                           Status  = EFI_SUCCESS;
    pm_err_flag_type                     errFlag = PM_ERR_FLAG__SUCCESS;

    errFlag = pm_ab_module_ready_to_enable((uint32)PmicDeviceIndex, (boolean)AbRdy);

    if(PM_ERR_FLAG__SUCCESS != errFlag)
    {
        Status = EFI_DEVICE_ERROR;
    }

    return Status;
}


/**
EFI_PmicAbModuleEn ()

@brief
enables module
*/
EFI_STATUS 
EFIAPI 
EFI_PmicAbModuleEn            
(
   IN UINT32 PmicDeviceIndex, 
   IN BOOLEAN ModuleEn
)
{
    EFI_STATUS                           Status  = EFI_SUCCESS;
    pm_err_flag_type                     errFlag = PM_ERR_FLAG__SUCCESS;

    errFlag = pm_ab_en_module((uint32)PmicDeviceIndex, (boolean)ModuleEn);

    if(PM_ERR_FLAG__SUCCESS != errFlag)
    {
        Status = EFI_DEVICE_ERROR;
    }

    return Status;
}


/**
EFI_PmicAbIbbRdyEn ()

@brief
Allows module to be reday by IBB
*/
EFI_STATUS 
EFIAPI 
EFI_PmicAbIbbRdyEn            
(
   IN UINT32 PmicDeviceIndex, 
   IN BOOLEAN IbbRdyEn
)
{
    EFI_STATUS                           Status  = EFI_SUCCESS;
    pm_err_flag_type                     errFlag = PM_ERR_FLAG__SUCCESS;

    errFlag = pm_ab_ibb_rdy_en((uint32)PmicDeviceIndex, (boolean)IbbRdyEn);

    if(PM_ERR_FLAG__SUCCESS != errFlag)
    {
        Status = EFI_DEVICE_ERROR;
    }

    return Status;
}


/**
EFI_PmicAbConfigPrechargeCtrl ()

@brief
Configures precharge control
*/
EFI_STATUS 
EFIAPI 
EFI_PmicAbConfigPrechargeCtrl 
(
   IN UINT32 PmicDeviceIndex, 
   IN BOOLEAN FastPrecharge, 
   IN UINT32 MaxTimeUsec
   )
{
    EFI_STATUS                           Status  = EFI_SUCCESS;
    pm_err_flag_type                     errFlag = PM_ERR_FLAG__SUCCESS;

    errFlag = pm_ab_config_precharge_ctrl((uint32)PmicDeviceIndex, (boolean)FastPrecharge, (uint32)MaxTimeUsec);

    if(PM_ERR_FLAG__SUCCESS != errFlag)
    {
        Status = EFI_DEVICE_ERROR;
    }

    return Status;
}


/**
EFI_PmicAbGetStatus ()

@brief
Gets AB status
*/
EFI_STATUS 
EFIAPI 
EFI_PmicAbGetStatus           
(
   IN UINT32 PmicDeviceIndex, 
   OUT EFI_PM_AB_STATUS_TYPE* AbStatus
)
{
    EFI_STATUS                           Status  = EFI_SUCCESS;
    pm_err_flag_type                     errFlag = PM_ERR_FLAG__SUCCESS;
    pm_ab_status_type                   ab_status;

    if(NULL == AbStatus)
    {
        return EFI_DEVICE_ERROR;
    }

    errFlag = pm_ab_get_ab_status((uint32)PmicDeviceIndex, &ab_status);

    if(PM_ERR_FLAG__SUCCESS != errFlag)
    {
        Status = EFI_DEVICE_ERROR;
    }
    else
    {
        AbStatus->AbModuleRdy        = ab_status.ab_module_rdy;      
        AbStatus->AbModuleEn         = ab_status.ab_module_en;       
        AbStatus->AbIbbEnRdy         = ab_status.ab_ibb_en_rdy;      
        AbStatus->EnFastPrecharge     = ab_status.en_fast_precharge;   
        AbStatus->MaxPrechgTimeUsec   = ab_status.max_prechg_time_usec;
        AbStatus->OutputVolt          = ab_status.output_volt; 
        AbStatus->AbVregOk           = ab_status.ab_vreg_ok;
        AbStatus->ShortCircuit         = ab_status.short_circuit;
        AbStatus->AbStatusEn         =  FALSE;
        AbStatus->IsenseTap           = (EFI_PM_AB_ISENSE_TYPE)ab_status.isense_tap; 
        AbStatus->IsenseGain           = (EFI_PM_AB_ISENSE_TYPE)ab_status.isense_gain; 
        AbStatus->PsThreshold          = ab_status.ps_threshold; 
        AbStatus->SelectPsTable       = ab_status.select_ps_table; 
        AbStatus->SelectDigitalPs     = ab_status.select_digital_ps; 
        AbStatus->EnablePs            = ab_status.enable_ps;
    }

    return Status;
}


/**
EFI_PmicAbConfigOutputVolt ()

@brief
Configures output volt, Vdisp
*/
EFI_STATUS
EFIAPI
EFI_PmicAbConfigOutputVolt
(
    IN UINT32   PmicDeviceIndex,
    IN BOOLEAN  OverrideOutputVolt,
    IN UINT32   OutputMillivolt
    )
{   
    EFI_STATUS                           Status  = EFI_SUCCESS;
    pm_err_flag_type                     errFlag = PM_ERR_FLAG__SUCCESS;
    
    errFlag = pm_ab_config_output_volt((uint32)PmicDeviceIndex, (boolean)OverrideOutputVolt, (uint32)OutputMillivolt);

    if(PM_ERR_FLAG__SUCCESS != errFlag)
    {
        Status = EFI_DEVICE_ERROR;
    }


    return Status;
}


EFI_STATUS
EFIAPI
EFI_PmicAbConfigCurrentSense
(
    IN UINT32   PmicDeviceIndex,
    IN EFI_PM_AB_ISENSE_TYPE  IsenseTap,
    IN EFI_PM_AB_ISENSE_TYPE  IsenseGain
    )
{   
    EFI_STATUS                           Status  = EFI_SUCCESS;
    pm_err_flag_type                     errFlag = PM_ERR_FLAG__SUCCESS;
    
    errFlag = pm_ab_config_current_sense(PmicDeviceIndex, (pm_ab_isense_type)IsenseTap, (pm_ab_isense_type)IsenseGain);

    if(PM_ERR_FLAG__SUCCESS != errFlag)
    {
        Status = EFI_DEVICE_ERROR;
    }


    return Status;
}



EFI_STATUS
EFIAPI
EFI_PmicAbConfigPulseSkipCtrl
(
    IN UINT32   PmicDeviceIndex,
    IN UINT32   PsThreshold,  
    IN BOOLEAN  EnablePs
    )
{   
    EFI_STATUS                           Status  = EFI_SUCCESS;
    pm_err_flag_type                     errFlag = PM_ERR_FLAG__SUCCESS;
    
    errFlag = pm_ab_config_pulse_skip_ctrl(PmicDeviceIndex, PsThreshold, EnablePs);

    if(PM_ERR_FLAG__SUCCESS != errFlag)
    {
        Status = EFI_DEVICE_ERROR;
    }


    return Status;
}

  
  
/**
PMIC AB UEFI Protocol implementation
*/
EFI_QCOM_PMIC_AB_PROTOCOL PmicAbProtocolImplementation = 
{
   PMIC_AB_REVISION,      
   EFI_PmicAbModuleRdy,           
   EFI_PmicAbModuleEn,            
   EFI_PmicAbIbbRdyEn,            
   EFI_PmicAbConfigPrechargeCtrl, 
   EFI_PmicAbGetStatus,
   EFI_PmicAbConfigOutputVolt,
   EFI_PmicAbConfigCurrentSense, 
   EFI_PmicAbConfigPulseSkipCtrl,   
};
