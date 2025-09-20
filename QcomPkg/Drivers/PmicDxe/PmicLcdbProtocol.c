/*! @file PmicLcdbProtocol.c 

*  PMIC- LCDB MODULE RELATED DECLARATION
*  This file contains functions and variable declarations to support 
*  the PMIC LCDB module.
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

02/05/18   al      New file.
===========================================================================*/

/*===========================================================================

INCLUDE FILES FOR MODULE

===========================================================================*/

#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>

#include "pm_uefi.h"
#include "pm_lcdb.h"

#include <Protocol/EFIPmicLcdb.h>


/*===========================================================================
EXTERNAL FUNCTION DECLARATIONS
===========================================================================*/
EFI_STATUS EFIAPI EFI_PmicLcdbSetOutput(IN UINT32 PmicDeviceIndex, IN INT32 LdoMv, IN INT32 NcpMv, IN BOOLEAN SymmetryOk)
{
    EFI_STATUS                           Status  = EFI_SUCCESS;
    pm_err_flag_type                     err_flag = PM_ERR_FLAG__SUCCESS;
    
    err_flag =  pm_lcdb_set_output_volt(PmicDeviceIndex, LdoMv, NcpMv, SymmetryOk);

    if(PM_ERR_FLAG__SUCCESS != err_flag)
    {
        Status = EFI_DEVICE_ERROR;
    }

    return Status;
}

EFI_STATUS EFIAPI EFI_PmicLcdbSetHwenDebounce(IN UINT32 PmicDeviceIndex, IN BOOLEAN Enable)
{
    EFI_STATUS                           Status  = EFI_SUCCESS;
    pm_err_flag_type                     err_flag = PM_ERR_FLAG__SUCCESS;

    err_flag =  pm_lcdb_set_hwen_debounce(PmicDeviceIndex, Enable);

    if(PM_ERR_FLAG__SUCCESS != err_flag)
    {
        Status = EFI_DEVICE_ERROR;
    }

    return Status;
}

EFI_STATUS EFIAPI EFI_PmicLcdbConfigEnableCtrl(IN UINT32 PmicDeviceIndex, IN EFI_PM_LCDB_ENABLE_CTL_TYPE CtlType, IN BOOLEAN Enable)
{
    EFI_STATUS                           Status  = EFI_SUCCESS;
    pm_err_flag_type                     err_flag = PM_ERR_FLAG__SUCCESS;

    err_flag =  pm_lcdb_config_enable_ctrl(PmicDeviceIndex, (pm_lcdb_enable_ctl_type)CtlType, (boolean)Enable);

    if(PM_ERR_FLAG__SUCCESS != err_flag)
    {
        Status = EFI_DEVICE_ERROR;
    }

    return Status;
}

EFI_STATUS EFIAPI EFI_PmicLcdbConfigPulldownCtrl(IN UINT32 PmicDeviceIndex, IN EFI_PM_LCDB_PULLDOWN_CTL_TYPE CtlType, IN BOOLEAN Ctrl)
{
    EFI_STATUS                           Status  = EFI_SUCCESS;
    pm_err_flag_type                     err_flag = PM_ERR_FLAG__SUCCESS;

    err_flag =  pm_lcdb_config_pulldown_ctrl(PmicDeviceIndex, (pm_lcdb_pulldown_ctl_type)CtlType, Ctrl);

    if(PM_ERR_FLAG__SUCCESS != err_flag)
    {
        Status = EFI_DEVICE_ERROR;
    }

    return Status;
}


EFI_STATUS EFIAPI EFI_PmicLcdbVregOk(IN UINT32  PmicDeviceIndex, OUT BOOLEAN  *VregOk)
{
    EFI_STATUS        Status   = EFI_SUCCESS;
    pm_err_flag_type  err_flag = PM_ERR_FLAG__SUCCESS;
    boolean           vreg_ok  = FALSE;

    if(VregOk == NULL)
    {
        Status =  EFI_DEVICE_ERROR;
    }
    else
    {
        err_flag =  pm_lcdb_vreg_ok(PmicDeviceIndex, &vreg_ok);
        *VregOk = vreg_ok;

        if(PM_ERR_FLAG__SUCCESS != err_flag)
        {
            Status = EFI_DEVICE_ERROR;
        }
    }

    return Status;
}

/**
PMIC LCDB UEFI Protocol implementation
*/
EFI_QCOM_PMIC_LCDB_PROTOCOL PmicLcdbProtocolImplementation = 
{
   PMIC_LCDB_REVISION,
   EFI_PmicLcdbSetOutput, 
   EFI_PmicLcdbSetHwenDebounce, 
   EFI_PmicLcdbConfigEnableCtrl,
   EFI_PmicLcdbConfigPulldownCtrl,
   EFI_PmicLcdbVregOk,  
};
