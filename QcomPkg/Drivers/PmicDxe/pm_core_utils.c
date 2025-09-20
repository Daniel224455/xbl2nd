/*! \file
*  
*  \brief  pm_core_utils.c ----This file contains the implementation of pmic peripheral dump ()
*  \details This file contains the implementation of pm_malloc()
*  
*  &copy; Copyright 2012-2018 QUALCOMM Technologies Incorporated, All Rights Reserved
*/

/*===========================================================================

EDIT HISTORY FOR MODULE

This document is created by a code generator, therefore this section will
not contain comments describing changes made to the module.


when       who     what, where, why
--------   ---     ----------------------------------------------------------'
09/04/18   sb      Added API for target specific cleanup
07/07/16   va      Flush serial port 
08/29/15   va      Created

===========================================================================*/

/*===========================================================================

INCLUDE FILES 

===========================================================================*/
//#include "pm_utils.h"
#include "SpmiBus.h"
#include "pm_uefi.h"
#include <../../Library/PmicLib/utils/inc/pm_utils.h>
#include <../../Library/PmicLib/framework/inc/pm_device.h>
#include <Library/BaseMemoryLib.h>
#include "PmicFileLog.h"
#include <Library/DebugLib.h>
#include "CoreVerify.h"

#include <Library/QcomBaseLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/QcomLib.h>
#include <Protocol/EFIVariableServices.h>
#include <pm_core_utils.h>
#include "Library/SerialPortShLib.h"


#define PMIC_MAX_NO_OF_PERIPHERAL  24
#define PMIC_PERIPHERAL_NO_OF_REGS 0x100

#define PMIC_GENERIC_REVISION 0x0000000000010001
extern EFI_GUID gQcomPmicGenericProtocolGuid;

pm_err_flag_type pm_core_utils_dump_single_peripheral(UINT8 slave_id, UINT32 StartAddr)
{
  SpmiBus_ResultType spmi_result = SPMI_BUS_SUCCESS;
  pm_err_flag_type   errFlg = PM_ERR_FLAG_SUCCESS;
  UINT8              ReadArr[PMIC_PERIPHERAL_NO_OF_REGS];
  UINT32             dataReadLength = 0, printCount = 0; 
  UINT32             dataLength = PMIC_PERIPHERAL_NO_OF_REGS;
  SetMem(ReadArr, sizeof(ReadArr)/sizeof(ReadArr[0]), 0x00);

  spmi_result = SpmiBus_ReadLong(slave_id, (SpmiBus_AccessPriorityType)0, StartAddr, ReadArr, dataLength, (UINT32*)&dataReadLength);
  if ( spmi_result != SPMI_BUS_SUCCESS )
  {
    PMIC_DEBUG((EFI_D_WARN, "PmicDxe:: %a SPMI API read Failure (%d) \n\r",__FUNCTION__, spmi_result));
    return PM_ERR_FLAG_BUS_ERR;
  }

  PMIC_DEBUG((EFI_D_WARN, "PmicDxe:: Pheripheral Dump Start = (0x%x) dataReadLength = 0x%x \n\r", StartAddr, dataReadLength));
  /* print current read Bytes */
  for(printCount = 0; printCount < dataReadLength ; printCount++)
    PMIC_DEBUG((EFI_D_WARN, "PmicDxe:: Addres:(0x%x) = Value (0x%x) \n\r", StartAddr + printCount, ReadArr[printCount]));

  PMIC_DEBUG((EFI_D_WARN, "PmicDxe:: Peripheral Dump End (0x%x) \n\r", StartAddr));

  /* Flush serial buffer in non production mode */
  SerialPortFlush();
  PMIC_DEBUG((EFI_D_WARN, "PmicDxe:: %a SerialPortFlush for PMIC Charger Fg dumps \r\n", __FUNCTION__));
  SerialPortFlush();

  return errFlg;
}


pm_err_flag_type pm_core_utils_dump_peripheral(UINT8 slave_id, UINT32 PherStartAddr, UINT8 NumOfPeripheral)
{
  UINT16  Count = 0;
  UINT16  StartRegAddr = (UINT16) PherStartAddr;
  pm_err_flag_type   errFlg = PM_ERR_FLAG_SUCCESS;

  if( NumOfPeripheral >= PMIC_MAX_NO_OF_PERIPHERAL )
    return PM_ERR_FLAG_INVALID_PARAMETER;

  SerialPortFlush();

  for(Count = 0; Count < NumOfPeripheral; Count++)
  {
    errFlg |= pm_core_utils_dump_single_peripheral(slave_id, StartRegAddr);
    //Incrment for next peripheral 
    StartRegAddr  += PMIC_PERIPHERAL_NO_OF_REGS;
  }

  return errFlg;
}


EFI_STATUS EFIAPI EFI_PmExitCleanup(VOID)
{
  EFI_STATUS Status = EFI_SUCCESS;
  pm_err_flag_type   err_flag = PM_ERR_FLAG_SUCCESS;

  err_flag |= pm_uefi_exit_target_specific_cleanup();

  err_flag |= pm_utils_irq_disable_clr_all();

  err_flag |= pm_config_lock_by_bit();

  Status = (err_flag == PM_ERR_FLAG_SUCCESS) ? EFI_SUCCESS : EFI_DEVICE_ERROR;

  return Status;
}


EFI_QCOM_PMIC_GENERIC_PROTOCOL PmicGenericProtocolImplementation =
{
  PMIC_GENERIC_REVISION,
  EFI_PmExitCleanup,
};


void pm_exit_cleanup(void)
{
  EFI_STATUS  Status = EFI_SUCCESS;

  EFI_QCOM_PMIC_GENERIC_PROTOCOL     *PmicGenericProtocol = NULL;

  Status = gBS->LocateProtocol(
    &gQcomPmicGenericProtocolGuid,
    NULL,
    (VOID **)&PmicGenericProtocol
    );

  if (Status == EFI_SUCCESS && PmicGenericProtocol != NULL)
  {
    Status = PmicGenericProtocol->PmExitCleanup();
  }

  if(Status != EFI_SUCCESS)
  {
    PMIC_DEBUG((EFI_D_ERROR, "PmicDxe: pm_exit_cleanup\n\r"));
  }

}


