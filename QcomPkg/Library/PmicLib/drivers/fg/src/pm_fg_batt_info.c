
/*! \file pm_fg_batt.c 
*  \n
*  \brief  PMIC-BMS MODULE RELATED DECLARATION
*  \details  This file contains functions and variable declarations to support 
*   the PMIC Fule Gauge memory Battery interface module.
*
*  Copyright (c) 2016-2018 Qualcomm Technologies, Inc.  All Rights Reserved. 
*  Qualcomm Technologies Proprietary and Confidential.
*/

/*===========================================================================

EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.


when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
06/13/18   cs      fix sram corruption for 1.0, 1.1
03/05/18   pxm     Remove unused APIs and registers.
09/19/17   cs      removed unsupported APIs
02/02/17   cs      sbl need to set batt id threshold to 0, it is too late for uefi to do it.
01/30/17   sm      set batt id missing threshold to highest
10/06/16   cs      change batt missing config
07/18/16   llioe   Handled register-address differences v1.1(and earlier) vs v2.0
06/29/16   va      updating IBat, Vbat calculation
06/16/16   va      Reading IBat, Vbat from batt info
06/01/16   va      Update Battery Temp calculation
03/24/16   va      Adding batt missing cfg api
03/16/16   va      Adding pm_fg_batt_info_get_batt_temp_sts() 
02/16/16   va      Adding required API for enanlbing charger
01/27/16   al      Initial Release 
===========================================================================*/

#include "pm_fg_batt_info.h"
#include "pm_fg_driver.h"
#include "pm_resource_manager.h"

#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>

/*===========================================================================
                        TYPE DEFINITIONS 
===========================================================================*/

#define ADDRESS(x) (fg_batt_ptr->fg_register->batt_info_register->base_address + fg_batt_ptr->fg_register->batt_info_register->x)
#define SLAVE_ID   (fg_batt_ptr->comm_ptr->slave_id)


#define PM_FG_BATT_INFO_MAX_RETRY      5

#define PM_FG_IBAT_INT_VAL         32768 /* (2^15) */
#define PM_FG_IBAT_MAX             16000

#define PM_FG_VBATT_MAXUINT16      65535 /* (2 ^ 16)*/
#define PM_FG_VBATT_MAX            8000

#define BATT_MISSING_THR_MAX          3

/*===========================================================================
                         FUNCTION DEFINITIONS
===========================================================================*/


pm_err_flag_type pm_fg_batt_info_get_ibatt_match_cp(uint32 pmic_device, int32 *pIBat, boolean *ibat_match_cp)
{
  pm_err_flag_type  err_flag    = PM_ERR_FLAG_SUCCESS;
  pm_fg_data_type *fg_batt_ptr  = pm_fg_get_data(pmic_device);
  pm_register_data_type data16[2] = {0};
  pm_register_data_type data16Cp[2] = {0};
  if (NULL == fg_batt_ptr)
  {
    err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
  }
  else if (NULL == pIBat || NULL == ibat_match_cp)
  {
    err_flag = PM_ERR_FLAG_INVALID_PARAMETER;
  }
  else
  {
    pm_register_address_type ibatt_msb_reg = ADDRESS(ibatt_msb);
    pm_register_address_type ibatt_lsb_reg = ADDRESS(ibatt_lsb);
    pm_register_address_type ibatt_msb_cp_reg = ADDRESS(ibatt_msb_cp);
    pm_register_address_type ibatt_lsb_cp_reg = ADDRESS(ibatt_lsb_cp);

    err_flag =  pm_comm_read_byte(SLAVE_ID, ibatt_msb_reg, (uint8 *)&data16[1], 0);
    err_flag |= pm_comm_read_byte(SLAVE_ID, ibatt_lsb_reg, (uint8 *)&data16, 0);
    err_flag |= pm_comm_read_byte(SLAVE_ID, ibatt_msb_cp_reg, (uint8 *)&data16Cp[1], 0);
    err_flag |= pm_comm_read_byte(SLAVE_ID, ibatt_lsb_cp_reg, (uint8 *)&data16Cp, 0);

    if (err_flag == PM_ERR_FLAG_SUCCESS)
    {
       *pIBat = data16[1] << 8 | data16[0];
       *ibat_match_cp = (data16[1] << 8 | data16[0]) ^ (data16Cp[1] << 8 | data16Cp[0]) ? FALSE : TRUE;
    }
  }

  return err_flag;
}

pm_err_flag_type pm_fg_batt_info_get_ibatt(uint32 pmic_device, int32 *pIBat)
{
  pm_err_flag_type  err_flag    = PM_ERR_FLAG_SUCCESS;
  pm_fg_data_type *fg_batt_ptr  = pm_fg_get_data(pmic_device);
  boolean ibatt_match_cp = FALSE;
  uint8  count = 0;
  INT32  IBatt = 0;
  INT16  SignedIBatt16 = 0; INT32 SignedIBatt32 = 0;
  INT32  result = 0;

  if (NULL == fg_batt_ptr)
  {
    err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
  }
  else if (NULL == pIBat )
  {
    err_flag = PM_ERR_FLAG_INVALID_PARAMETER;
  }
  else
  {
    err_flag |= pm_fg_batt_info_get_ibatt_match_cp(pmic_device, &IBatt, &ibatt_match_cp);

    //If ibatt_match_cp is FALSE, retry
    if (ibatt_match_cp == FALSE)
    {
      for (count =0; count < PM_FG_BATT_INFO_MAX_RETRY; count++) 
      {
        err_flag |= pm_fg_batt_info_get_ibatt_match_cp(pmic_device, &IBatt, &ibatt_match_cp);
        if (ibatt_match_cp == TRUE)
        {
          break;
        }
      }
    }
    if (ibatt_match_cp == FALSE)
    {
      DEBUG(( EFI_D_WARN, "PmicDxe:: %a ibatt_match_cp = %d \n\r",__FUNCTION__, ibatt_match_cp));
      err_flag |= PM_ERR_FLAG_PERIPHERAL_ERR;
    }
    else
    {
      SignedIBatt16 = (int16)IBatt;
      SignedIBatt32 =  (int32)SignedIBatt16;
      result = (SignedIBatt32 * PM_FG_IBAT_MAX)/ PM_FG_IBAT_INT_VAL ;
      /* DEBUG(( EFI_D_WARN, "PmicDxe:: %a IBatt = 0x%x SignedIBatt = %d Result = %d mA IBatt = 0x%x mA\n\r",__FUNCTION__, IBatt, SignedIBatt32, result, result));*/
      *pIBat = result ;
    }
  }

return err_flag;

}


pm_err_flag_type pm_fg_batt_info_get_vbatt_match_cp(uint32 pmic_device, uint32 *pVBat, boolean *vbat_match_cp)
{
  pm_err_flag_type  err_flag    = PM_ERR_FLAG_SUCCESS;
  pm_fg_data_type *fg_batt_ptr  = pm_fg_get_data(pmic_device);
  pm_register_data_type data16[2] = {0};
  pm_register_data_type data16Cp[2] = {0};
  if (NULL == fg_batt_ptr)
  {
    err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
  }
  else if (NULL == pVBat || NULL == vbat_match_cp)
  {
    err_flag = PM_ERR_FLAG_INVALID_PARAMETER;
  }
  else
  {
    pm_register_address_type vbatt_msb_reg = ADDRESS(vbatt_msb);
    pm_register_address_type vbatt_lsb_reg = ADDRESS(vbatt_lsb);
    pm_register_address_type vbatt_msb_cp_reg = ADDRESS(vbatt_msb_cp);
    pm_register_address_type vbatt_lsb_cp_reg = ADDRESS(vbatt_lsb_cp);

    err_flag  = pm_comm_read_byte(SLAVE_ID, vbatt_msb_reg, (uint8 *)&data16[1], 0);
    err_flag |= pm_comm_read_byte(SLAVE_ID, vbatt_lsb_reg, (uint8 *)&data16[0], 0);
    err_flag |= pm_comm_read_byte(SLAVE_ID, vbatt_msb_cp_reg, (uint8 *)&data16Cp[1], 0);
    err_flag |= pm_comm_read_byte(SLAVE_ID, vbatt_lsb_cp_reg, (uint8 *)&data16Cp[0], 0);

    if (err_flag == PM_ERR_FLAG_SUCCESS)
    {
      *pVBat = data16[1] << 8 | data16[0];
      *vbat_match_cp = (data16[1] << 8 | data16[0]) ^ (data16Cp[1] << 8 | data16Cp[0]) ? FALSE:TRUE;
    }
  }

  return err_flag;
}


/*
Return vbat in Mv
*/pm_err_flag_type pm_fg_batt_info_get_vbatt(uint32 pmic_device, uint32 *pVBat)
{
  pm_err_flag_type  err_flag    = PM_ERR_FLAG_SUCCESS;
  pm_fg_data_type *fg_batt_ptr  = pm_fg_get_data(pmic_device);
  boolean vbatt_match_cp = FALSE;
  uint8  count =0;
  uint32 VBatt = 0;

  if (NULL == fg_batt_ptr)
  {
    err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
  }
  else if (NULL == pVBat )
  {
    err_flag = PM_ERR_FLAG_INVALID_PARAMETER;
  }
  else
  {
    err_flag |= pm_fg_batt_info_get_vbatt_match_cp(pmic_device, &VBatt, &vbatt_match_cp);

    //If vbatt_match_cp is FALSE, retry
    if (vbatt_match_cp == FALSE)
    {
      for (count =0; count < PM_FG_BATT_INFO_MAX_RETRY; count++) 
      {
        err_flag |= pm_fg_batt_info_get_vbatt_match_cp(pmic_device, &VBatt, &vbatt_match_cp);
        if (vbatt_match_cp == TRUE)
        {
          break;
        }
       }
    }
    if (vbatt_match_cp == FALSE)
    {
      DEBUG(( EFI_D_WARN, "PmicDxe:: %a vbatt_match_cp = %d \n\r",__FUNCTION__, vbatt_match_cp));
      err_flag |= PM_ERR_FLAG_PERIPHERAL_ERR;
    }
    else
    {
      VBatt =  (VBatt * PM_FG_VBATT_MAX)/PM_FG_VBATT_MAXUINT16;
      /* DEBUG(( EFI_D_WARN, "PmicDxe:: %a VBatt = %d mV VBatt = 0x%x \n\r",__FUNCTION__, VBatt, VBatt));*/
      *pVBat = VBatt;
    }
  }

  return err_flag;
}



pm_err_flag_type pm_fg_batt_info_set_esr_pull_down_cfg(uint32 pmic_device, pm_fg_batt_info_batt_esr_pull_down_cfg esr_cfg, uint8 esr_value)
{
  pm_err_flag_type    err_flag    = PM_ERR_FLAG_SUCCESS;
  pm_fg_data_type *fg_batt_ptr  = pm_fg_get_data(pmic_device);
  if (NULL == fg_batt_ptr)
  {
    err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
  }
  else if (esr_cfg >= PM_FG_BATT_INFO_ESR_CFG_INVALID)
  {
    err_flag = PM_ERR_FLAG_INVALID_PARAMETER;
  }
  else
  {
    pm_register_address_type esr_reg = ADDRESS(esr_pull_down_cfg);
    err_flag  = pm_comm_write_byte_mask(SLAVE_ID, esr_reg, (esr_value << esr_cfg ), (pm_register_data_type)(esr_value << esr_cfg ), 0);
  }

  return err_flag;
}

pm_err_flag_type pm_fg_batt_info_set_peek_mux4(uint32 pmic_device, uint8 reg_val)
{
  pm_err_flag_type    err_flag = PM_ERR_FLAG_SUCCESS;
  pm_fg_data_type *fg_batt_ptr = pm_fg_get_data(pmic_device);
  if (NULL == fg_batt_ptr)
  {
    err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
  }
  else
  {
    pm_register_address_type peek_mux4_reg = ADDRESS(peek_mux4);
    err_flag = pm_comm_write_byte(SLAVE_ID, peek_mux4_reg, (pm_register_data_type)(reg_val), 0);
  }

  return err_flag;
}


pm_err_flag_type pm_fg_batt_info_get_alg_actv_status(uint32 pmic_device, uint8* pAlgactv)
{
  pm_err_flag_type  err_flag = PM_ERR_FLAG_SUCCESS;
  pm_fg_data_type *fg_batt_ptr = pm_fg_get_data(pmic_device);
  uint8 mask = 0x08; //bit 3 for algorithm active

  if (NULL == fg_batt_ptr)
  {
    err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
  }
  else if (NULL == pAlgactv)
  {
    err_flag = PM_ERR_FLAG_INVALID_PARAMETER;
  }
  else
  {
    pm_register_address_type alg_actv_reg = ADDRESS(peek_rd);
    err_flag = pm_comm_read_byte_mask(SLAVE_ID, alg_actv_reg, mask, pAlgactv, 0);

  }

  return err_flag;
}