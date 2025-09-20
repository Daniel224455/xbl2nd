/*! \file
*  
*  \brief  pm_prm_device_vreg.c ----This file contains driver functions for PMIC  
*                                  VREG PRM Layer 
*
* Copyright (c) 2016 QUALCOMM Technologies, Inc. (QTI). All Rights Reserved.  
* QUALCOMM Proprietary.  Export of this technology or software is regulated 
* by the U.S. Government. Diversion contrary to U.S. law prohibited.  
*  
*/
/*===========================================================================

            EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  
when       who     what, where, why
--------   ---     ----------------------------------------------------------
09/22/16   rl      Add Scalar and Vector Support for RPMh use PRM convention 
01/16/13   umr     Add Suppressible request support.
03/07/12   hs      Use DAl APIs to replace memory allocation. 
02/12/12   wra     KVP based device setting initial file 
===========================================================================*/
/*===========================================================================

             INCLUDE FILES FOR MODULE

===========================================================================*/
#include "pm_prm_device_update.h"
#include "pm_prm_driver.h"
#include "pm_log_utils.h"
#include "pm_utils.h"
#include "CoreVerify.h"
#include "pm_rpmh.h"
#include "pm_ldo.h"
#include "pm_smps.h"
#include "pm_vs.h"
#include "pm_clk.h"




npa_resource_state
pm_prm_get_vreg_rsrc_state(npa_client_handle client_handle, 
                          pm_prm_vreg_data_type *pam_data)
{
  npa_resource_state    state      = 0;
  pm_prm_rsrc_data_type *rsrc_data = NULL;
  
  if(client_handle == NULL || pam_data == NULL)
  {
    PM_LOG_MSG_ERR_FATAL("Invalid Inputs");
  }
  
  rsrc_data = (pm_prm_rsrc_data_type*)client_handle->resource->definition->data;
  
  switch(rsrc_data->rsrc_key)
  {
    case PM_RSRC_KEY_MODE:
      state = pam_data->mode; 
      break;

    case PM_RSRC_KEY_VOLTAGE:
      state = pam_data->voltage;
      break;

    case PM_RSRC_KEY_ENABLE:
      state = pam_data->enable;
      break;

    default:
      PM_LOG_MSG_ERR_FATAL("Invalid RSRC Key provided for %s", 
                                  client_handle->resource_name);
  }
  return state; 
}

npa_resource_state
pm_prm_get_clk_rsrc_state(npa_client_handle     client_handle, 
                          pm_prm_clk_data_type  *pam_data)
{
  npa_resource_state    state      = 0;
  pm_prm_rsrc_data_type *rsrc_data = NULL;
  
  if(client_handle == NULL || pam_data == NULL)
  {
    PM_LOG_MSG_ERR_FATAL("Invalid Inputs");
  }
  
  rsrc_data=(pm_prm_rsrc_data_type*)client_handle->resource->definition->data;
  
  switch(rsrc_data->rsrc_key)
  {
    case PM_RSRC_KEY_ENABLE:
      state = pam_data->enable;
      break;
      
    default:
      PM_LOG_MSG_ERR_FATAL("Invalid RSRC Key provided for %s", 
                           client_handle->resource_name);
  }
  return state; 
}

/************************* Query API's ***************************/

static boolean
pm_prm_get_ldo_status(npa_client_handle client_handle, 
                      pm_prm_vreg_data_type *pam_data)
{
  pm_prm_rsrc_data_type   *rsrc_data  = NULL;
  pm_on_off_type          on_off      = PM_OFF;
  boolean                 vreg_status = TRUE; 
  boolean                 ret_val     = FALSE;
  uint8                   pmic_chip   = 0; 
  pm_err_flag_type        err_flag    = PM_ERR_FLAG_SUCCESS;
  pm_sw_mode_type         mode        = 0;
  pm_volt_level_type      volt_level  = 0; 
  
  if(client_handle == NULL || pam_data == NULL)
  {
    PM_LOG_MSG_ERR_FATAL("Invalid Inputs");
  }
  
  rsrc_data = (pm_prm_rsrc_data_type*)client_handle->resource->definition->data;
  
  pmic_chip = (uint8)((char)(*rsrc_data->pmic_chip) - 'a');
  
  
  
  err_flag |= pm_ldo_sw_mode_status(pmic_chip, rsrc_data->periph_id, &mode);
  
  err_flag |= pm_ldo_volt_level_status(pmic_chip, 
                                       rsrc_data->periph_id, 
                                       &volt_level);
  
  //Convert voltage to mV as pm_ldo_volt_level_status returns voltage in uV
  //but pam uses mV units
  volt_level=volt_level/1000;    
  
  err_flag |= pm_ldo_sw_enable_status(pmic_chip, 
                                      rsrc_data->periph_id, 
                                      &on_off);
                                      
  if(on_off == PM_ON)
  {
    err_flag |= pm_ldo_vreg_ready_status (pmic_chip, 
                                      rsrc_data->periph_id,
                                      &vreg_status);
  }
      
  if((err_flag != PM_ERR_FLAG_SUCCESS)          ||
     (mode < pam_data->mode)                     ||
     (on_off < (pm_on_off_type)pam_data->enable) ||
     (volt_level < pam_data->voltage)            ||
     (vreg_status!= TRUE))
  {
    PM_LOG_MSG_INFO("Pam Data for LDO %d, enable=%d, voltage=%d, mode=%d", 
                    (rsrc_data->periph_id + 1), pam_data->enable, 
                    pam_data->voltage, pam_data->mode);
    
    PM_LOG_MSG_INFO("Fail:LDO=%d, err=%d, mode=%d, en=%d, volt=%d, vready=%d",
                    (rsrc_data->periph_id + 1), err_flag, mode, 
                    on_off, volt_level, vreg_status);
    
    ret_val = FALSE; 
  }
  else
  {
    ret_val = TRUE; 
  }

  return ret_val;
}

static boolean
pm_prm_get_smps_status(npa_client_handle client_handle, 
                      pm_prm_vreg_data_type *pam_data)
{
  pm_prm_rsrc_data_type *rsrc_data   = NULL;
  pm_sw_mode_type        mode        = 0;
  pm_volt_level_type     volt_level  = 0; 
  pm_on_off_type         on_off      = PM_OFF;
  boolean                vreg_status = TRUE; 
  boolean                ret_val     = FALSE;
  uint8                  pmic_chip   = 0; 
  pm_err_flag_type       err_flag    = PM_ERR_FLAG_SUCCESS;
  
  if(client_handle == NULL || pam_data == NULL)
  {
    PM_LOG_MSG_ERR_FATAL("Invalid Inputs");
  }
  
  rsrc_data=(pm_prm_rsrc_data_type*)client_handle->resource->definition->data;
  
  pmic_chip = (uint8)((char)(*rsrc_data->pmic_chip) - 'a');
  
  err_flag |= pm_smps_sw_mode_status(pmic_chip, rsrc_data->periph_id, &mode);
  
  err_flag |= pm_smps_volt_level_status(pmic_chip, 
                                        rsrc_data->periph_id, 
                                        &volt_level);
                                        
  /*Convert voltage to mV as pm_ldo_volt_level_status returns voltage in uV
  * but pam uses mV units
  */
  volt_level=volt_level/1000;
  
  err_flag |= pm_smps_sw_enable_status(pmic_chip, 
                                       rsrc_data->periph_id, 
                                       &on_off);
  
  if(on_off == PM_ON)
  {
    err_flag |= pm_smps_vreg_ready_status(pmic_chip, 
                                      rsrc_data->periph_id,
                                      &vreg_status);
  }
  
  if((err_flag != PM_ERR_FLAG_SUCCESS)          ||
     (mode < pam_data->mode)                     ||
     (on_off < (pm_on_off_type)pam_data->enable) ||
     (volt_level < pam_data->voltage)            || 
     (vreg_status!= TRUE))
  {
    PM_LOG_MSG_INFO("Pam Data for SMPS %d, enable=%d, voltage=%d, mode=%d",
                    (rsrc_data->periph_id + 1), pam_data->enable, 
                    pam_data->voltage, pam_data->mode); 
  
    PM_LOG_MSG_INFO("FAIL:SMP=%d, err=%d, mode=%d, en=%d, volt=%d, vready=%d",
                    (rsrc_data->periph_id + 1), err_flag, mode, 
                    on_off, volt_level, vreg_status);
                    
    ret_val = FALSE; 
  }
  else
  {
    ret_val = TRUE;
  }
  
  return ret_val;
}

static boolean
pm_prm_get_vs_status(npa_client_handle client_handle, 
                      pm_prm_vreg_data_type *pam_data)
{   
  pm_prm_rsrc_data_type *rsrc_data   = NULL;
  pm_on_off_type         on_off      = PM_OFF;
  boolean                vreg_status = TRUE; 
  boolean                ret_val     = FALSE;
  uint8                  pmic_chip   = 0; 
  pm_err_flag_type       err_flag    = PM_ERR_FLAG_SUCCESS;
  
  if(client_handle == NULL || pam_data == NULL)
  {
    PM_LOG_MSG_ERR_FATAL("Invalid Inputs");
  }
  
  rsrc_data = (pm_prm_rsrc_data_type*)client_handle->resource->definition->data;
  
  pmic_chip = (uint8)((char)(*rsrc_data->pmic_chip) - 'a');
      
  err_flag |= pm_vs_sw_enable_status(pmic_chip, 
                                     rsrc_data->periph_id, 
                                     &on_off);
  
  if(on_off == PM_ON)
  {
    err_flag |= pm_vs_vreg_ready_status(pmic_chip, 
                                      rsrc_data->periph_id,
                                      &vreg_status);
  }
  
  if((err_flag != PM_ERR_FLAG_SUCCESS)          ||
     (on_off < (pm_on_off_type)pam_data->enable) ||
     (vreg_status!= TRUE))
  {
    PM_LOG_MSG_INFO("Pam Data for VS %d, enable=%d", 
                    (rsrc_data->periph_id + 1), pam_data->enable);
                    
    PM_LOG_MSG_INFO("Failed: VS=%d, err_flag=%d, on_off = %d", 
                    (rsrc_data->periph_id + 1) , err_flag, on_off);
  
    ret_val = FALSE; 
  }
  else 
  {
    ret_val = TRUE; 
  }
  
  return ret_val;
}

boolean
pm_prm_get_vreg_status(npa_client_handle client_handle, 
                       pm_prm_vreg_data_type *pam_data)
{
  pm_prm_rsrc_data_type *rsrc_data = NULL;
  boolean                retval    = FALSE;

  if(client_handle == NULL || pam_data == NULL)
  {
    PM_LOG_MSG_ERR_FATAL("Invalid Inputs");
  }
  
  rsrc_data =(pm_prm_rsrc_data_type*)client_handle->resource->definition->data;
  
  switch(rsrc_data->rsrc_type)
  {
    case PM_PRM_RSRC_LDO:
      retval = pm_prm_get_ldo_status(client_handle, pam_data);
      break;
  
    case PM_PRM_RSRC_SMPS:
      retval = pm_prm_get_smps_status(client_handle, pam_data);
      break;
  
    case PM_PRM_RSRC_VS:
      retval = pm_prm_get_vs_status(client_handle, pam_data);
      break;
  
    default:
      PM_LOG_MSG_ERR_FATAL("Invalid RSRC provided for");
  }
  
  return retval;
}

boolean
pm_prm_get_clk_status(npa_client_handle client_handle, 
                      pm_prm_clk_data_type *pam_data)
{
  pm_prm_rsrc_data_type *rsrc_data = NULL;
  pm_on_off_type        clk_ok     = TRUE; 
  uint8                 pmic_chip  = 0; 
  boolean               ret_val    = FALSE; 
  pm_err_flag_type      err_flag   = PM_ERR_FLAG_SUCCESS;
  pm_clk_status_type    clk_status;
  
  if(client_handle == NULL || pam_data == NULL)
  {
    PM_LOG_MSG_ERR_FATAL("Invalid Inputs");
  }
  
  rsrc_data =(pm_prm_rsrc_data_type*)client_handle->resource->definition->data;
  
  pmic_chip = (uint8)((char)(*rsrc_data->pmic_chip) - 'a');     

  err_flag |= pm_clk_get_status(pmic_chip, 
                                rsrc_data->periph_id, 
                                &clk_status);

  //If clk is off ignore clk_ok by setting it to ON
  clk_ok = clk_status.clk_sw_enable == PM_ON ? clk_status.clk_ok : PM_ON; 

  if((err_flag != PM_ERR_FLAG_SUCCESS)                            ||
     (clk_status.clk_sw_enable < (pm_on_off_type)pam_data->enable) ||
     (clk_ok != PM_ON))
  {
    PM_LOG_MSG_INFO("Pam Data for Clk %d, enable=%d", 
                    rsrc_data->periph_id, pam_data->enable);

    PM_LOG_MSG_INFO("Failed: CLK=%d, err_flag=%d, enable = %d, clk_ok=%d", 
                    (rsrc_data->periph_id + 1), err_flag, 
                    clk_status.clk_sw_enable, clk_ok);
    ret_val=FALSE; 
  }
  else
  {
    ret_val=TRUE;
  }
  
  return ret_val;
}
