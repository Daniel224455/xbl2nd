/*! @file PmicLcdbProtocol.c 

*  PMIC- OLEDB MODULE RELATED DECLARATION
*  This file contains functions and variable declarations to support 
*  the PMIC OLEDB module.
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
02/06/18?   al      New file.
===========================================================================*/

/*===========================================================================

INCLUDE FILES FOR MODULE

===========================================================================*/

#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>

#include "pm_uefi.h"
#include "pm_oledb.h"

#include <Protocol/EFIPmicOledb.h>


/*===========================================================================
EXTERNAL FUNCTION DECLARATIONS
===========================================================================*/

EFI_STATUS EFIAPI 
EFI_PmicOledbModuleRdy(IN UINT32 PmicDeviceIndex, IN BOOLEAN OledbRdy)
{
    EFI_STATUS Status  = EFI_SUCCESS;
                     
    pm_err_flag_type err_flag = PM_ERR_FLAG__SUCCESS;

    err_flag =  pm_oledb_module_ready_to_be_enabled(PmicDeviceIndex, OledbRdy);

    if(PM_ERR_FLAG__SUCCESS != err_flag)
    {
        Status = EFI_DEVICE_ERROR;
    }

    return Status;
}


EFI_STATUS EFIAPI 
EFI_PmicOledbEnableModule(IN UINT32 PmicDeviceIndex, IN BOOLEAN Enable)
{

    EFI_STATUS Status  = EFI_SUCCESS;
                     
    pm_err_flag_type err_flag = PM_ERR_FLAG__SUCCESS;

    err_flag = pm_oledb_enable_module(PmicDeviceIndex, Enable);

    if(PM_ERR_FLAG__SUCCESS != err_flag)
    {
        Status = EFI_DEVICE_ERROR;
    }

    return Status;
}


EFI_STATUS EFIAPI 
EFI_PmicOledbEnableExtPinCtl(IN UINT32 PmicDeviceIndex, IN BOOLEAN Enable)
{
    EFI_STATUS Status  = EFI_SUCCESS;
                     
    pm_err_flag_type err_flag = PM_ERR_FLAG__SUCCESS;

    err_flag = pm_oledb_enable_ext_pin_ctl(PmicDeviceIndex, Enable);

    if(PM_ERR_FLAG__SUCCESS != err_flag)
    {
        Status = EFI_DEVICE_ERROR;
    }

    return Status;
}


EFI_STATUS EFIAPI 
EFI_PmicOledbSetOutputVolt(IN UINT32 PmicDeviceIndex, IN UINT32 VoutMv)
{
    EFI_STATUS Status  = EFI_SUCCESS;
                     
    pm_err_flag_type err_flag = PM_ERR_FLAG__SUCCESS;

    err_flag = pm_oledb_set_output_volt(PmicDeviceIndex, VoutMv);

    if(PM_ERR_FLAG__SUCCESS != err_flag)
    {
        Status = EFI_DEVICE_ERROR;
    }

    return Status;
}


/**
PMIC OLEDB UEFI Protocol implementation
*/
EFI_QCOM_PMIC_OLEDB_PROTOCOL PmicOledbProtocolImplementation = 
{
   PMIC_OLEDB_REVISION,
   EFI_PmicOledbModuleRdy, 
   EFI_PmicOledbEnableModule, 
   EFI_PmicOledbEnableExtPinCtl,
   EFI_PmicOledbSetOutputVolt,
};
