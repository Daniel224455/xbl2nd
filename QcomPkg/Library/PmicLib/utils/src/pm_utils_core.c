/*! \file
*  
*  \brief  pm_utils_core.c 
*  
*  &copy; Copyright 2016-2018 QUALCOMM Technologies Incorporated, All Rights Reserved
*/

/*===========================================================================

EDIT HISTORY FOR MODULE

This document is created by a code generator, therefore this section will
not contain comments describing changes made to the module.


when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
08/03/16   akm      Created

===========================================================================*/

/*===========================================================================

INCLUDE FILES 

===========================================================================*/
#include <Uefi.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/SerialPortShLib.h>
#include <Library/TimerLib.h>

#include "pm_utils.h"
#include "SpmiCfg.h"
#include "SpmiBusCfg.h"
#include "pm_target_information.h"
#include "CoreVerify.h"

#define INT_LATCHED_CLR_OFFSET 0x0014
#define INT_EN_CLR_OFFSET      0x0016
#define INT_CLEAR_VAL          0xFF
#define BROADCAST_SLAVE_ID     14

pm_err_flag_type pm_busy_wait(uint32 req_wait_usec)
{
  uint32 actual_wait_usec = 0;
	
  /*the MicroSecondDelay API returns wait_usec as return value*/
  actual_wait_usec = MicroSecondDelay(req_wait_usec);

  if(actual_wait_usec < req_wait_usec)
  {
   return PM_ERR_FLAG_INVALID_PARAMETER; 
  }
  else
  {
    return PM_ERR_FLAG_SUCCESS;
  }
	
}

pm_err_flag_type 
pm_device_shutdown(void)
{
  /*flush UART before shutdown*/
  SerialPortFlush();
  gRT->ResetSystem(EfiResetShutdown, EFI_SUCCESS, 0, NULL);
  /*control should not return from here*/
  return PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
}


pm_err_flag_type
pm_utils_irq_disable_clr_all(void)
{
  pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;

  SpmiCfg_ChannelCfg *spmi_cfg_ptr      = NULL;
  uint32 *spmi_cfg_size                  = NULL;
  uint32 periph_base_address = 0x0;
  uint32 i = 0;

  spmi_cfg_ptr = (SpmiCfg_ChannelCfg *)pm_target_information_get_specific_info(PM_PROP_SPMI_CHANNEL_CFG);
  CORE_VERIFY_PTR(spmi_cfg_ptr);

  spmi_cfg_size = (uint32 *)pm_target_information_get_specific_info(PM_PROP_SPMI_CHANNEL_CFG_SIZE);
  CORE_VERIFY_PTR(spmi_cfg_size);

  for(i = 0; i < *spmi_cfg_size; i++)
  { 
    /*if periph and irq Ownermask is APPS and slave id and not broadcast*/
    if((spmi_cfg_ptr[i].periphOwnerMask & SPMI_OWNER_APPS) 
       && (spmi_cfg_ptr[i].irqOwnerMask & SPMI_OWNER_APPS) 
       && (spmi_cfg_ptr[i].slaveId != BROADCAST_SLAVE_ID)
       )
    {
      periph_base_address = spmi_cfg_ptr[i].periphId * 0x100;
      err_flag |= pm_comm_write_byte(spmi_cfg_ptr[i].slaveId, periph_base_address + INT_EN_CLR_OFFSET, INT_CLEAR_VAL, 0); 
      err_flag |= pm_comm_write_byte(spmi_cfg_ptr[i].slaveId, periph_base_address + INT_LATCHED_CLR_OFFSET, INT_CLEAR_VAL, 0); 
    }
  }
  /*clear all SPMI irq*/
  err_flag |=  pm_comm_spmi_irq_clear_all();

  return err_flag;
}
