/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                  PMIC Startup Services

GENERAL DESCRIPTION
  This file contains initialization functions and corresponding variable
  declarations to support interaction with the Qualcomm Pmic chips.

  Copyright (c) 2013-2018 Qualcomm Technologies, Inc.  All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.


when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
09/04/18   sb      Added API to set OS flag in SDAM
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
#include "pm_sdam.h"
#include "i2c_api.h"
#include <Library/UefiBootServicesTableLib.h>
#include "EFIChipInfo.h"
#include <Protocol/EFIPlatformInfo.h>

/**
  PMIC PROTOCOL interface
*/
#include "Protocol/EFIPmicSchg.h"
#include "Protocol/EFIPmicFg.h"
#include "Protocol/EFIPmicVreg.h"
#include "Protocol/EFIPmicPwrOn.h"
#include "pm_comm.h"
#include "CoreVerify.h"
#include "pm_log_utils.h"

#include "pmapp_npa.h"
#include "npa_resource.h"
#include "pm_sdam.h"

/*===========================================================================

                 LOCAL CONSTANT AND MACRO DEFINITIONS

===========================================================================*/

#define PON_INT_MID_SEL_REG  0x81A
#define PRIMARY_PON_SLAVE_ID 0x0
#define INT_MID_0_VAL        0x0
#define SPMI_PRIORITY        0x0

#define SDAM_MEM_014_ADDR    14
#define GPIO_GROUNDED        0x0
#define SINGLE_BYTE           1


#define EXT_BUCK_BUS_FREQ       400 //400 KHz
#define EXT_BUCK_SLAVE_ADDR     0x70
#define EXT_BUCK_DEVICE_ID_REG  0x41
#define EXT_BUCK_DEVICE_ID_VAL  0x6
#define EXT_BUCK_CONTROL_REG    0x02
#define EXT_BUCK_PWM_MODE_VAL   0x04

#define DATA_SIZE_BYTE   1
#define OFFSET_SIZE_BYTE 1
#define TIMEOUT_MAX      2500

#define SDAM_OS_INFO         37
#define SDAM_OS_MASK         0x01
#define SDAM_MEM_055_ADDR    55

#define   SharedIMEMBaseAddr            0x146BF000
#define   AbnormalResetOccurredOffset   0x24
/*===========================================================================

                        TYPE DEFINITIONS

===========================================================================*/
typedef enum
{
  PM_SDAM_OS_UEFI,
  PM_SDAM_OS_HLOS,
  PM_SDAM_OS_INVALID
} pm_sdam_os;

/*===========================================================================

                VARIABLES DEFINITIONS

===========================================================================*/
extern EFI_QCOM_PMIC_SCHG_PROTOCOL        PmicSchgProtocolImplementation;
extern EFI_QCOM_PMIC_FG_BASIC_PROTOCOL    PmicFgBasicProtocolImplementation;
extern EFI_QCOM_PMIC_PON_PROTOCOL         PmicPonProtocolImplementation;


/*===========================================================================

                LOCAL FUNCTION PROTOTYPES

===========================================================================*/
static pm_err_flag_type pm_ext_buck_config(void);

/*===========================================================================

FUNCTION pm_sdam_set_active_os

DESCRIPTION
    Updates a bit in PMIC SDAM to convey the currently active OS to
    Charger firmware which is running on ADSP.

    param[in] os_type - UEFI or HLOS

===========================================================================*/
static pm_err_flag_type pm_sdam_set_active_os(pm_sdam_os os_type);

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

static pm_err_flag_type pm_sdam_set_active_os(pm_sdam_os os_type)
{
  pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
  uint8 data = 0;

  if(os_type >= PM_SDAM_OS_INVALID)
    return PM_ERR_FLAG_INVALID_PARAMETER;

  err_flag = pm_sdam_mem_read(PMIC_A, PM_SDAM_2, SDAM_OS_INFO, 1, &data);
  if(err_flag != PM_ERR_FLAG_SUCCESS)
    return err_flag;

  data = data & (~SDAM_OS_MASK) | os_type;

  err_flag = pm_sdam_mem_write(PMIC_A, PM_SDAM_2, SDAM_OS_INFO, 1, &data);

  return err_flag;
}

static pm_err_flag_type pm_sdam_clear_sec_mor_flag(void)
{
  pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
  uint8 data = 0;
  /*** Clear SEC MOR Flag set in SDAM in UEFI EXIT (was added for Uefi Off mode charging support)***/
  err_flag = pm_sdam_mem_write(PMIC_A, PM_SDAM_2, SDAM_MEM_055_ADDR, 1, &data);
  return err_flag;
}

pm_err_flag_type pm_post_pmic_initialization(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
{
  pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
  EFI_STATUS  Status = EFI_SUCCESS;
  volatile uint32* AbnormalResetOccurred = NULL;
  EFI_CHIPINFO_PROTOCOL *pEfiChipInfoProtocol = NULL;
  EFI_PLATFORMINFO_PROTOCOL *pPlatformInfoProtocol = NULL;

  EFI_PLATFORMINFO_PLATFORM_TYPE  PlatformType ;
  EFI_PLATFORMINFO_PLATFORM_INFO_TYPE  platformInfo;

  EFIChipInfoVersionType ChipVersion = 0;

  pm_register_data_type sdam_mem_data = 0;

  /* 
    PON peripheral interrupts are routed to Master PMIC such that PBS can receive PON interrupts. 
    Once UEFI boot reaches the point of pwr key hold check function, after the check passes,
    it needs to change the PON MID back to MSM which is 0 so that APPS can resume receiving the interrupts.
    INT is disabled at the UEFI exit. CR - 2157004
    */
  err_flag = pm_comm_write_byte(PRIMARY_PON_SLAVE_ID, PON_INT_MID_SEL_REG, INT_MID_0_VAL, SPMI_PRIORITY);

  Status = gBS->LocateProtocol(&gEfiChipInfoProtocolGuid, NULL, (VOID **)&pEfiChipInfoProtocol);

  Status |= gBS->LocateProtocol ( &gEfiPlatformInfoProtocolGuid, NULL, (VOID **)&pPlatformInfoProtocol);

  if ((Status == EFI_SUCCESS) && (pEfiChipInfoProtocol != NULL) && (pPlatformInfoProtocol != NULL))
  {
    Status = pEfiChipInfoProtocol->GetChipVersion(pEfiChipInfoProtocol, &ChipVersion);
    Status |=   pPlatformInfoProtocol->GetPlatformInfo( pPlatformInfoProtocol, &platformInfo );
    PlatformType = platformInfo.platform;

    if ((Status == EFI_SUCCESS) && (EFI_PLATFORMINFO_TYPE_MTP == PlatformType) && (ChipVersion < EFI_CHIPINFO_VERSION(2, 0)))
    {
      //work around for V1 MTP HW only
      err_flag |= pm_ext_buck_config();
    }
  }

      /*if ground then vote on L9E otherwise vote on L5E*/
  err_flag |= pm_sdam_mem_read ( PMIC_A, PM_SDAM_1, SDAM_MEM_014_ADDR, SINGLE_BYTE, &sdam_mem_data);

  if(sdam_mem_data == GPIO_GROUNDED)
  {
    npa_alias_resource_cb(PMIC_NPA_GROUP_ID_USB_SS1_A, PMIC_NPA_GROUP_ID_USB_SS1, NULL, NULL);
  }
  else
  {
    npa_alias_resource_cb(PMIC_NPA_GROUP_ID_USB_SS1_B, PMIC_NPA_GROUP_ID_USB_SS1, NULL, NULL);
  }

  // Set flag to indicate to Charger FW that UEFI is currently running
  err_flag |= pm_sdam_set_active_os(PM_SDAM_OS_UEFI);

  if (Status != EFI_SUCCESS)
  {
    err_flag |= PM_ERR_FLAG_FAILURE;
  }
  
  /*** Read SEC MOR Flag set in SDAM and update abnormal reset flag in shared mem for APPS to use***/
  err_flag = pm_sdam_mem_read(PMIC_A, PM_SDAM_2, SDAM_MEM_055_ADDR, 1, &sdam_mem_data);
  if(sdam_mem_data == 0xA5)
  {
      //update abnormal reset flag
       AbnormalResetOccurred = (UINT32* )((UINTN)SharedIMEMBaseAddr + AbnormalResetOccurredOffset);
      *AbnormalResetOccurred = 0x1;
  }

  return err_flag;
}

static pm_err_flag_type pm_ext_buck_config(void)
{
  pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
  i2c_status istatus       = I2C_SUCCESS;
  void *i2c_handle        = NULL;
  uint32 read_write_count  = 0;
  uint8 data             = 0;

  i2c_slave_config  cfg = {
    .bus_frequency_khz          =  EXT_BUCK_BUS_FREQ,
    .slave_address              =  EXT_BUCK_SLAVE_ADDR,
    .mode                       = I2C,
    .slave_max_clock_stretch_us = 500,
    .core_configuration1        = 0,
    .core_configuration2        = 0
  };

  istatus = i2c_open(I2C_INSTANCE_020, &i2c_handle);

  if ((istatus == I2C_SUCCESS) && (i2c_handle != NULL))
  {
    istatus |= i2c_read(i2c_handle, &cfg, EXT_BUCK_CONTROL_REG, OFFSET_SIZE_BYTE, &data, DATA_SIZE_BYTE, &read_write_count, TIMEOUT_MAX);
	
    data |= EXT_BUCK_PWM_MODE_VAL;

    istatus |= i2c_write(i2c_handle, &cfg, EXT_BUCK_CONTROL_REG, OFFSET_SIZE_BYTE, &data, DATA_SIZE_BYTE, &read_write_count, TIMEOUT_MAX);
  }

  if (istatus != I2C_SUCCESS)
  {
     pm_log_message("Failed to detect external I2C controlled buck. Buck is left at default state \n");
  }


  return err_flag;

}

pm_err_flag_type pm_uefi_exit_target_specific_cleanup(void)
{
  pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;

  // Set HLOS flag to indicate to Charger FW that UEFI has ended
  err_flag |= pm_sdam_set_active_os(PM_SDAM_OS_HLOS);

  /** Clear SEC MOR Flag set in SDAM in UEFI EXIT**/
  err_flag |= pm_sdam_clear_sec_mor_flag();
  return err_flag;
}
