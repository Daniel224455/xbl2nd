/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                  PMIC Startup Services

GENERAL DESCRIPTION
  This file contains initialization functions and corresponding variable
  declarations to support interaction with the Qualcomm Pmic chips.

  Copyright (c) 2013-2019 Qualcomm Technologies, Inc.  All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.


when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
09/04/18   sb      Added API for target specific cleanup
04/18/17   va      Moving target specific call to target file
04/18/17   va      Added API call to set USB typeC port role in PmicInitialize based on HostModeSupported PCD
04/12/17   va      Adding PON protocol 
02/07/17   sm      Added changes to skip installing SCHG and FG protocols for CLS platform
03/30/16   va      enable protocol 
03/09/16   va      moved common protocol install to pmic.c
03/08/16   al      Moving NPA to common
07/23/15   al      Adding NPA
07/21/14   va      Adding Fuel Gauge Protocol
07/03/14   al      Adding MIPI-BIF, RGB. Removing SW workaround since it doesn't solve issue
06/06/14   al      Adding workaround for HW issue 
04/28/14   va      Expose Npa Test protocol
05/09/14   al      Adding IBB and LAB 
04/18/14   al      Added SMBCHG 
11/22/13   va      PmicLib Dec Addition
10/02/13   al      New File
===========================================================================*/
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "com_dtypes.h"
#include "pm_uefi.h"
#include <Library/UefiBootServicesTableLib.h>

/**
  PMIC PROTOCOL interface
*/
#include "Protocol/EFIPmicSchg.h"
#include "Protocol/EFIPmicFg.h"
#include "Protocol/EFIPmicVreg.h"
#include "Protocol/EFIPmicPwrOn.h"
#include "pm_comm.h"
#include "CoreVerify.h"
/*===========================================================================

                 LOCAL CONSTANT AND MACRO DEFINITIONS

===========================================================================*/

#define PON_INT_MID_SEL_REG  0x81A
#define PRIMARY_PON_SLAVE_ID 0x0
#define INT_MID_0_VAL        0x0
#define SPMI_PRIORITY        0x0

/*===========================================================================

                VARIABLES DEFINITIONS

===========================================================================*/

extern EFI_QCOM_PMIC_SCHG_PROTOCOL        PmicSchgProtocolImplementation;
extern EFI_QCOM_PMIC_FG_BASIC_PROTOCOL    PmicFgBasicProtocolImplementation;
extern EFI_QCOM_PMIC_PON_PROTOCOL         PmicPonProtocolImplementation;


/*===========================================================================

                LOCAL FUNCTION PROTOTYPES

===========================================================================*/


/*===========================================================================

                EXTERNAL FUNCTION PROTOTYPES

===========================================================================*/

pm_err_flag_type pm_install_target_protocols(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
{

  pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
  EFI_STATUS  Status = EFI_SUCCESS;

  Status = gBS->InstallMultipleProtocolInterfaces(
    &ImageHandle,
    &gQcomPmicPonProtocolGuid,  &PmicPonProtocolImplementation,       /*installing gQcomPmicPonProtocolGuid for open source*/
    &gQcomPmicSchgProtocolGuid, &PmicSchgProtocolImplementation,
    &gQcomPmicFgProtocolGuid,   &PmicFgBasicProtocolImplementation,
    NULL
    );

  err_flag = (Status == EFI_SUCCESS) ? PM_ERR_FLAG_SUCCESS : PM_ERR_FLAG_FAILURE;

  return err_flag;
}

pm_err_flag_type pm_post_pmic_initialization(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
{
   pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;

  /* 
    PON peripheral interrupts are routed to Master PMIC such that PBS can receive PON interrupts. 
    Once UEFI boot reaches the point of pwr key hold check function, after the check passes,
    it needs to change the PON MID back to MSM which is 0 so that APPS can resume receiving the interrupts.
    INT is disabled at the UEFI exit. CR - 2157004
    */
  err_flag = pm_comm_write_byte(PRIMARY_PON_SLAVE_ID, PON_INT_MID_SEL_REG, INT_MID_0_VAL, SPMI_PRIORITY);

  return err_flag;
}

pm_err_flag_type pm_uefi_exit_target_specific_cleanup(void)
{
  pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;

  return err_flag;
}
