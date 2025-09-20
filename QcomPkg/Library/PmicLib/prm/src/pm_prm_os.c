/*! \file
 *
 *  \brief  pm_prm_os.c - Image specific initializations and functions.
 *
 * Copyright (c) 2018 QUALCOMM Technologies, Inc. (QTI). All Rights Reserved.
 * QUALCOMM Proprietary.  Export of this technology or software is regulated
 * by the U.S. Government. Diversion contrary to U.S. law prohibited.
 */

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This document is created by a code generator, therefore this section will
  not contain comments describing changes made to the module.


===========================================================================*/

/*===========================================================================

                     INCLUDE FILES

===========================================================================*/

#include "comdef.h"
#include <err.h>
#include "npa.h"
#include "npa_resource.h"
#include "npa_remote_resource.h"
#include "npa_log.h"
#include "pmapp_npa.h"
#include "pm_prm_driver.h"
#include "pm_prm_init.h"
#include "pm_target_information.h"
#include "pm_utils.h"
#include "DDIPlatformInfo.h"
//#include "pm_ulog.h"
#include "CoreVerify.h"
//#include "cmd_db.h"
#include "pm_rpmh.h"
//#include "cmd_db_core.h"
#include "cmd_db.h"
#include "pm_prm_os.h"
#include "pm_ldo_driver.h"
#include "pm_smps_driver.h"
#include "pm_vs_driver.h"
#include "pm_clk_driver.h"
#include "pm_log_utils.h"

/*****************************DEFINITIONS*************************************/

#define CLK_MAX_SETTLING_US 2000
#define WAIT_TIME_US        25
#define MAX_STR_SIZE        25
#define CMD_DB_STR_SIZE     9
#define PMIC_STR_SIZE       2

extern boolean bPMICStub;

/**************************HELPER FUNCTIONS***********************************/
static pm_sw_mode_type
pm_prm_os_vreg_mode_trans(pm_prm_vreg_mode_type prm_mode);

static void
pm_prm_os_set_ldo(pm_prm_rsrc_data_type *rsrc_data);

static void
pm_prm_os_set_smps(pm_prm_rsrc_data_type *rsrc_data);

static void
pm_prm_os_set_clk(pm_prm_rsrc_data_type *rsrc_data);

static void
pm_prm_os_set_vreg(pm_prm_rsrc_data_type *rsrc_data);

static boolean
pm_prm_os_get_ldo_status(npa_client_handle client_handle,
                      pm_prm_vreg_data_type *pam_data);

static boolean
pm_prm_os_get_smps_status(npa_client_handle client_handle,
                      pm_prm_vreg_data_type *pam_data);

static boolean
pm_prm_os_get_vs_status(npa_client_handle client_handle,
                      pm_prm_vreg_data_type *pam_data);


/******************************************************************************
 pm_prm_os_max_plugin

 Plugin for local control resources
 Uses npa_max_update_fcn for aggregation of resources
 Supports only Required client types for local control.
******************************************************************************/
const npa_resource_plugin
pm_prm_os_max_plugin =
{
  npa_max_update_fcn,
  /* Supported client types */
  NPA_CLIENT_REQUIRED,
  NULL,                         /* Create client function */
  NULL                          /* Destroy client function */
};


/******************************************************************************
 pm_prm_os_set_priority

 Sets priority of device layer dependencies of pam clients
 to the RPMH driver to differentiate between hierarchy in which
 Resource should be populated in NAS TCS's
*****************************************************************************/
void
pm_prm_os_set_priority(pm_prm_node_resource_info const  *node_rsrc)
{
  return;
}

/******************************************************************************
 pm_prm_os_get_vrm_data

 Populate vrm specific info obtained from CMD_DB in device layer resources
*****************************************************************************/
pm_err_flag_type
pm_prm_os_get_vrm_data(pm_prm_rsrc_data_type *rsrc_data)
{
  //Command Db temp variables
  uint16           cmd_db_version  = 0;
  uint32           cmd_db_data_len = 0;
  uint32           cmd_db_aux_data = 0;
  cmd_db_err_type  cmd_db_err      = CMD_DB_SUCCESS;
  uint8            pmic_chip       = 0;
  pm_model_type    pmic_model      = PMIC_IS_UNKNOWN;
  pm_err_flag_type err_flag        = PM_ERR_FLAG_SUCCESS;

  if ((bPMICStub == TRUE) /*|| (pm_target_info_is_default_cfg() == TRUE)*/)
  {
    return err_flag;
  }

  if(rsrc_data == NULL)
  {
    PM_LOG_MSG_ERR_FATAL("Rsrc Data is NULL");
  }

  /* Query command db Version */
  cmd_db_version = cmd_db_query_version(rsrc_data->cmd_db_name);

  /*Get resource address */
  rsrc_data->rpmh_data.address = cmd_db_query_addr(rsrc_data->cmd_db_name);

  if( rsrc_data->rpmh_data.address == 0 )
  {
    pmic_chip = (uint8)((char)(*rsrc_data->pmic_chip) - 'a');

    pmic_model = pm_get_pmic_model(pmic_chip);

    if(pmic_model != PMIC_IS_UNKNOWN && pmic_model != PMIC_IS_INVALID)
    {
      err_flag |= PM_ERR_FLAG_FAILURE;
      PM_LOG_MSG_INFO("Error Flag: %d, RPMh Address for %s not found in cmd_db", err_flag, rsrc_data->cmd_db_name);
    }

    /* If address is 0, return from here */
    return err_flag;
  }

  switch(cmd_db_version & 0xff00)
  {
    case 0:
      rsrc_data->rpmh_data.vrm_ctrl_type = PM_PRM_VRM_INVALID_CTRL;
      break;

    case 0x100:
      cmd_db_data_len = cmd_db_query_len(rsrc_data->cmd_db_name);

      if(cmd_db_data_len > 0)
      {
        cmd_db_err =  cmd_db_query_aux_data (rsrc_data->cmd_db_name,
                                           (uint8*)&cmd_db_data_len,
                                           (uint8*)&cmd_db_aux_data);
      }

      /* Local Control is supported for modem image */
      if(((cmd_db_aux_data & 0xf) < PM_PRM_VRM_INVALID_CTRL) && 
          (cmd_db_data_len != 0))
      {
        rsrc_data->rpmh_data.vrm_ctrl_type =
                      (pm_prm_vrm_ctrl_type)(cmd_db_aux_data & 0xf);
      }
      else
      {
        err_flag |= PM_ERR_FLAG_FAILURE;
        PM_LOG_MSG_INFO("Error_flag:%d, Rpmh Aux Data Failure for %s", err_flag, rsrc_data->cmd_db_name);
      }
      break;

    default:
      PM_LOG_MSG_ERR_FATAL("CMD_DB VRM Version not supported, ver = %d", cmd_db_version);
      break;
  }

  return err_flag;
}

/******************************************************************************
 pm_prm_os_publish_rsrcs

 Publish PAM layer resources to USER PD's for inter-pd voting of resources
*****************************************************************************/
void
pm_prm_os_publish_rsrcs(pm_prm_pam_data_type *pam_data)
{
  return;
}

/******************************************************************************
 pm_prm_os_set_pam_plugin

 Sets correct plugin for PAM layer resources based on single/multi client
 or local control support.
 Uses npa_identity_plugin for single client resources
 Uses npa_max_plugin for multi client resources
 Uses pm_prm_os_max_plugin for pam resource which has one or more local control
 dependencies.
 *****************************************************************************/
void
pm_prm_os_set_pam_plugin( npa_node_definition *node_rsrc,
                          npa_resource_definition *rsrc )
{
  npa_query_handle      query_handle  = NULL;
  npa_resource          *resource     = NULL;
  pm_prm_rsrc_data_type *rsrc_data    = NULL;
  boolean               local_ctrl    = FALSE;
  uint32                j             = 0;

  if(node_rsrc == NULL || rsrc == NULL)
  {
    PM_LOG_MSG_ERR_FATAL("Invalid Input Parameters node_rsrc = %d, rsrc = &d",
                          node_rsrc, rsrc);
  }

  if ((bPMICStub == TRUE) /* || (pm_target_info_is_default_cfg() == TRUE)*/)
  {
    return;
  }

  /*Check device layer dependency resources for local control is requirement */
  for(j = 0; j < node_rsrc->dependency_count; j++)
  {
    query_handle =
    npa_create_query_handle(node_rsrc->dependencies[j].name);

    if (query_handle != NULL)
    {
      resource = npa_query_get_resource(query_handle);
    }
    else
    {
      PM_LOG_MSG_ERR_FATAL("Unable to get query handle for rsrsc %s",
                          node_rsrc->dependencies[j].name);
    }

    if (resource != NULL)
    {
      rsrc_data = (pm_prm_rsrc_data_type*)resource->definition->data;

      if(rsrc_data != NULL &&
         rsrc_data->rpmh_data.vrm_ctrl_type == PM_PRM_VRM_LOCAL_CTRL)
      {
        local_ctrl = TRUE;
        break;
      }
    }
    query_handle = NULL;
    resource     = NULL;
  }

  /* Set plugin based on attributes and local control flag.*/
  if(local_ctrl == TRUE)
  {
    rsrc->plugin = &pm_prm_os_max_plugin;

    /* Make all dependencies as required only for Local Control Resource */
    for(j = 0; j < node_rsrc->dependency_count; j++)
    {
      node_rsrc->dependencies[j].client_type =  NPA_CLIENT_REQUIRED;
    }
  }
  else if(rsrc->attributes & NPA_RESOURCE_SINGLE_CLIENT)
  {
    rsrc->plugin = &npa_identity_plugin;
  }
  else
  {
    rsrc->plugin = &npa_max_plugin;
  }

  return;
}

/******************************************************************************
 pm_prm_os_set_device_plugin

 Sets plugin for device layer resources based on local control or RPMH control
 Uses npa_max_plugin for rpmh controlled resources
 Usese pm_prm_os_max_plugin for local control resources
******************************************************************************/
void
pm_prm_os_set_device_plugin(pm_prm_rsrc_data_type *rsrc_data,
                            npa_resource_definition *rsrc)
{
  if(rsrc_data == NULL || rsrc == NULL)
  {
    PM_LOG_MSG_ERR_FATAL("Resource data cannot be null");
  }

  if(rsrc_data->rpmh_data.vrm_ctrl_type == PM_PRM_VRM_LOCAL_CTRL)
  {
    rsrc->plugin = &pm_prm_os_max_plugin;
  }
  else
  {
    rsrc->plugin = &npa_max_plugin;
  }
  return;
}

/************************************LOCAL CONTROL****************************/

/******************************************************************************
 pm_prm_os_set_local_device

 Sets appropriate mode, voltage and enable for local control devices.
 Supports mode, voltage and enable settings for LDO and SMPS local control
 Supports enable setting for clock local control
*****************************************************************************/
void
pm_prm_os_set_local_device(pm_prm_rsrc_data_type *rsrc_data)
{
  if (rsrc_data == NULL)
  {
    PM_LOG_MSG_ERR_FATAL("Invalid Rsrc Data Pointer");
  }

  switch(rsrc_data->rsrc_vrm_type)
  {
    case PM_PRM_RSRC_VRM_VREG:
      pm_prm_os_set_vreg(rsrc_data);
      break;

    case PM_PRM_RSRC_VRM_CLK:
      pm_prm_os_set_clk(rsrc_data);
      break;

    default:
      PM_LOG_MSG_ERR_FATAL("Rsrc type not found for local ctrl");
      break;
  }
  return;
}


/* Helper function for local control support */
static pm_sw_mode_type
pm_prm_os_vreg_mode_trans(pm_prm_vreg_mode_type prm_mode)
{
  pm_sw_mode_type raw_mode = PM_SW_MODE_INVALID;

  if(prm_mode == PM_PRM_VREG_MODE_BYPASS ||
     prm_mode == PM4_PRM_VREG_MODE_BYPASS)
  {
    raw_mode = PM_SW_MODE_BYPASS;
  }
  else if(prm_mode == PM_PRM_VREG_MODE_RETENTION ||
          prm_mode == PM4_PRM_VREG_MODE_RETENTION)
  {
    raw_mode = PM_SW_MODE_RETENTION;
  }
  else if(prm_mode == PM_PRM_VREG_MODE_LPM ||
          prm_mode == PM4_PRM_VREG_MODE_LPM)
  {
    raw_mode = PM_SW_MODE_LPM;
  }
  else if(prm_mode == PM_PRM_VREG_MODE_AUTO ||
          prm_mode == PM4_PRM_VREG_MODE_AUTO)
  {
    raw_mode = PM_SW_MODE_AUTO;
  }
  else if(prm_mode == PM_PRM_VREG_MODE_NPM ||
          prm_mode == PM4_PRM_VREG_MODE_NPM)
  {
    raw_mode = PM_SW_MODE_NPM;
  }
  else
  {
    raw_mode = PM_SW_MODE_INVALID;
  }
  return raw_mode;

}

/* Helper function for local control support */
static void
pm_prm_os_set_ldo(pm_prm_rsrc_data_type *rsrc_data)
{
  uint8             pmic_chip = 0;
  uint8             ldo_index = 0;
  uint32            value     = 0;
  pm_err_flag_type  err_flag  = PM_ERR_FLAG_SUCCESS;
  pm_sw_mode_type   mode      = PM_SW_MODE_INVALID;

  if(rsrc_data == NULL)
  {
    PM_LOG_MSG_ERR_FATAL("Invalid Rsrc Data Pointer");
  }

  pmic_chip = (uint8)((char)(*rsrc_data->pmic_chip) - 'a');
  ldo_index = rsrc_data->periph_id;
  value     = rsrc_data->rpmh_data.active_set;

  switch(rsrc_data->rsrc_key)
  {
    case PM_RSRC_KEY_MODE:
      mode = pm_prm_os_vreg_mode_trans((pm_prm_vreg_mode_type)value);
      err_flag |= pm_ldo_sw_mode(pmic_chip, ldo_index, mode);
      break;

    case PM_RSRC_KEY_VOLTAGE:
      err_flag |= pm_ldo_volt_level(pmic_chip, ldo_index,
                                    (pm_volt_level_type)(value*1000), TRUE);
      break;

    case PM_RSRC_KEY_ENABLE:
      err_flag |= pm_ldo_sw_enable(pmic_chip, ldo_index,
                                   (pm_on_off_type)value, TRUE);
      break;

    default:
      PM_LOG_MSG_ERR_FATAL("Rsrc type not found for local ctrl");
      break;
  }

  if(err_flag != PM_ERR_FLAG_SUCCESS)
  {
    PM_LOG_MSG_ERR_FATAL("unable to set ldo pmic=%d, periph=%d, value=%d",
                          pmic_chip, ldo_index, value);
  }
  return;
}

/* Helper function for local control support */
static void
pm_prm_os_set_smps(pm_prm_rsrc_data_type *rsrc_data)
{
  uint8             pmic_chip  = 0;
  uint8             smps_index = 0;
  uint32            value      = 0;
  pm_err_flag_type  err_flag   = PM_ERR_FLAG_SUCCESS;
  pm_sw_mode_type   mode       = PM_SW_MODE_INVALID;

  if(rsrc_data == NULL)
  {
    PM_LOG_MSG_ERR_FATAL("Invalid Rsrc Data Pointer");
  }

  pmic_chip = (uint8)((char)(*rsrc_data->pmic_chip) - 'a');
  smps_index = rsrc_data->periph_id;
  value     = rsrc_data->rpmh_data.active_set;

  switch(rsrc_data->rsrc_key)
  {
    case PM_RSRC_KEY_MODE:
      mode = pm_prm_os_vreg_mode_trans((pm_prm_vreg_mode_type)value);
      err_flag |= pm_smps_sw_mode(pmic_chip, smps_index, mode);
      break;

    case PM_RSRC_KEY_VOLTAGE:
      err_flag |= pm_smps_volt_level(pmic_chip, smps_index,
                                    (pm_volt_level_type)(value*1000), TRUE);
      break;

    case PM_RSRC_KEY_ENABLE:
      err_flag |= pm_smps_sw_enable(pmic_chip, smps_index,
                                   (pm_on_off_type)value, TRUE);
      break;

    default:
      PM_LOG_MSG_ERR_FATAL("Rsrc type not found for local ctrl");
      break;
  }

  if(err_flag != PM_ERR_FLAG_SUCCESS)
  {
    PM_LOG_MSG_ERR_FATAL("unable to set smps pmic=%d, periph=%d, value=%d",
                          pmic_chip, smps_index, value);
  }
  return;
}

/* Helper function for local control support */
static void
pm_prm_os_set_clk(pm_prm_rsrc_data_type *rsrc_data)
{
  uint8             pmic_chip = 0;
  uint8             clk_index = 0;
  uint32            value     = 0;
  pm_err_flag_type  err_flag  = PM_ERR_FLAG_SUCCESS;
  pm_on_off_type    on_off    = PM_OFF;
  uint32            wait_time = 0;
  pm_clk_status_type clk_status;

  if(rsrc_data == NULL)
  {
    PM_LOG_MSG_ERR_FATAL("Invalid Rsrc Data Pointer");
  }

  pmic_chip = (uint8)((char)(*rsrc_data->pmic_chip) - 'a');
  clk_index = rsrc_data->periph_id;
  value     = rsrc_data->rpmh_data.active_set;

  switch(rsrc_data->rsrc_key)
  {
    case PM_RSRC_KEY_ENABLE:
      err_flag |= pm_clk_sw_enable(pmic_chip, clk_index,
                                   (pm_on_off_type)value);

      if ((pm_on_off_type)value == PM_ON)
      {
        do
        {
          err_flag |= pm_busy_wait(WAIT_TIME_US);

          wait_time+=WAIT_TIME_US;
          
          err_flag |= pm_clk_get_status(pmic_chip, clk_index, &clk_status);

          on_off = clk_status.clk_ok;

          if(err_flag != PM_ERR_FLAG_SUCCESS)
          {
            PM_LOG_MSG_ERR_FATAL("Failed reading clk status, %d", clk_index);
          }

          }while(on_off != PM_ON && wait_time < CLK_MAX_SETTLING_US);

        if(on_off != PM_ON)
        {
          PM_LOG_MSG_ERR_FATAL("Clk failed to settle, index = %d", clk_index);
        }
      }
      break;

    default:
      PM_LOG_MSG_ERR_FATAL("Unsupported rsrc key, %d", rsrc_data->rsrc_key);
      break;
  }

  if(err_flag != PM_ERR_FLAG_SUCCESS)
  {
    PM_LOG_MSG_ERR_FATAL("unable to set clk pmic=%d, periph=%d, value=%d",
                          pmic_chip, clk_index, value);
  }
  return;
}

/* Helper function for local control support */
static void
pm_prm_os_set_vreg(pm_prm_rsrc_data_type *rsrc_data)
{
  if (rsrc_data == NULL)
  {
    PM_LOG_MSG_ERR_FATAL("Invalid Rsrc Data Pointer");
  }

  switch(rsrc_data->rsrc_type)
  {
    case PM_PRM_RSRC_LDO:
      pm_prm_os_set_ldo(rsrc_data);
      break;
    case PM_PRM_RSRC_SMPS:
      pm_prm_os_set_smps(rsrc_data);
      break;
    default:
      PM_LOG_MSG_ERR_FATAL("Unsupported rsrc type, %d", rsrc_data->rsrc_type);
  }
  return;
}



/******************************** Query API's ********************************/

/******************************************************************************
 pm_prm_os_get_vreg_status

 Gets physical status of an LDO, SMPS or VS upon an NPA Query and compares it
 to current aggregation. If the status at device layer is greater or equal
 to the aggregation in npa then returns a true value denoting that the resource
 is set correctly in the pmic device
******************************************************************************/
boolean
pm_prm_os_get_vreg_status(npa_client_handle client_handle,
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
      retval = pm_prm_os_get_ldo_status(client_handle, pam_data);
      break;

    case PM_PRM_RSRC_SMPS:
      retval = pm_prm_os_get_smps_status(client_handle, pam_data);
      break;

    case PM_PRM_RSRC_VS:
      retval = pm_prm_os_get_vs_status(client_handle, pam_data);
      break;

    default:
      PM_LOG_MSG_ERR_FATAL("Invalid RSRC provided for");
  }

  return retval;
}

/******************************************************************************
 pm_prm_os_get_vreg_status

 Gets physical status of an LDO, SMPS or VS upon an NPA Query and compares it
 to current aggregation. If the status at device layer is greater or equal
 to the aggregation in npa then returns a true value denoting that the resource
 is set correctly in the pmic device
******************************************************************************/
boolean
pm_prm_os_get_clk_status(npa_client_handle client_handle,
                      pm_prm_clk_data_type *pam_data)
{
  pm_prm_rsrc_data_type *rsrc_data = NULL;
  pm_on_off_type        clk_ok     = TRUE;
  uint8                 pmic_chip  = 0;
  boolean               ret_val    = FALSE;
  pm_err_flag_type      err_flag   = PM_ERR_FLAG__SUCCESS;
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

  if((err_flag != PM_ERR_FLAG__SUCCESS)                            ||
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

/* Helper function for pm_prm_os_get_vreg_status */
static boolean
pm_prm_os_get_ldo_status(npa_client_handle client_handle,
                      pm_prm_vreg_data_type *pam_data)
{
  pm_prm_rsrc_data_type   *rsrc_data  = NULL;
  pm_on_off_type          on_off      = PM_OFF;
  boolean                 vreg_status = TRUE;
  boolean                 ret_val     = FALSE;
  uint8                   pmic_chip   = 0;
  pm_err_flag_type        err_flag    = PM_ERR_FLAG__SUCCESS;
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
     err_flag |= pm_ldo_vreg_ready_status(pmic_chip, rsrc_data->periph_id, &vreg_status);
  }

  if((err_flag != PM_ERR_FLAG__SUCCESS)          ||
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

/* Helper function for pm_prm_os_get_vreg_status */
static boolean
pm_prm_os_get_smps_status(npa_client_handle client_handle,
                      pm_prm_vreg_data_type *pam_data)
{
  pm_prm_rsrc_data_type *rsrc_data   = NULL;
  pm_sw_mode_type        mode        = 0;
  pm_volt_level_type     volt_level  = 0;
  pm_on_off_type         on_off      = PM_OFF;
  boolean                vreg_status = TRUE;
  boolean                ret_val     = FALSE;
  uint8                  pmic_chip   = 0;
  pm_err_flag_type       err_flag    = PM_ERR_FLAG__SUCCESS;

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

  if((err_flag != PM_ERR_FLAG__SUCCESS)          ||
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

/* Helper function for pm_prm_os_get_vreg_status */
static boolean
pm_prm_os_get_vs_status(npa_client_handle client_handle,
                      pm_prm_vreg_data_type *pam_data)
{
  pm_prm_rsrc_data_type *rsrc_data   = NULL;
  pm_on_off_type         on_off      = PM_OFF;
  boolean                vreg_status = TRUE;
  boolean                ret_val     = FALSE;
  uint8                  pmic_chip   = 0;
  pm_err_flag_type       err_flag    = PM_ERR_FLAG__SUCCESS;

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

  if((err_flag != PM_ERR_FLAG__SUCCESS)          ||
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


/* Helper function for pm_prm_aux_dev_init */
static void
numtostr(uint8 number, char *arr)
{
  uint8 res       = 0;
  uint8 i         = 0;
  uint8 rev_cnt   = 0;
  char *ptr_front = NULL;
  char *ptr_back  = NULL;
  char temp       = 0;

  if(arr == NULL)
  {
    PM_LOG_MSG_ERR_FATAL("Invalid Pointer");
  }

  do
  {
    res    = number % 10;
    number = number/10;
    arr[i] = res + '0'; //ascii value
    i++;
  }while(number > 0);

  arr[i] = '\0';
  ptr_front = arr;
  ptr_back  = (arr + (i-1));
  rev_cnt = i/2;

  while(rev_cnt)
  {
    temp       = *ptr_front;
    *ptr_front = *ptr_back;
    *ptr_back  = temp;
    ptr_front++;
    ptr_back--;
    rev_cnt--;
  }

  return;
}

/* Helper function for pm_prm_aux_dev_init */
static void
pm_prm_create_rsrc_str(char* str_ptr, char* rsrc_name,
                       char* key_name, uint32 rsrc_number,
                       char* pmic_name)
{
  char rsrc_num_str[5] = {0, 0, 0, 0, 0};

  if(str_ptr == NULL || rsrc_name == NULL ||
     key_name == NULL || pmic_name == NULL)
  {
    PM_LOG_MSG_ERR_FATAL("Invalid Input");
  }

  numtostr(rsrc_number, rsrc_num_str);
  strlcpy(str_ptr, "/pm/"       ,MAX_STR_SIZE);
  strlcat(str_ptr, rsrc_name    ,MAX_STR_SIZE);
  strlcat(str_ptr, pmic_name    ,MAX_STR_SIZE);
  strlcat(str_ptr, rsrc_num_str ,MAX_STR_SIZE);
  strlcat(str_ptr, "/"          ,MAX_STR_SIZE);
  strlcat(str_ptr, key_name     ,MAX_STR_SIZE);

  return;
}

/* Helper function for pm_prm_aux_dev_init */
static void
pm_prm_create_node_str(char* str_ptr, char* rsrc_name,
                       uint32 rsrc_number, char* pmic_name)
{
  char rsrc_num_str[5] = {0, 0, 0, 0, 0};

  if(str_ptr == NULL || rsrc_name == NULL || pmic_name == NULL)
  {
    PM_LOG_MSG_ERR_FATAL("Invalid Input Pointers");
  }

 //"/node/" #rtype #chipid #index
  numtostr(rsrc_number, rsrc_num_str);
  strlcpy(str_ptr, "/node/",     MAX_STR_SIZE);
  strlcat(str_ptr, rsrc_name,    MAX_STR_SIZE);
  strlcat(str_ptr, pmic_name,    MAX_STR_SIZE);
  strlcat(str_ptr, rsrc_num_str, MAX_STR_SIZE);

  return;
}

/* Helper function for pm_prm_aux_dev_init */
void
pm_prm_create_cmddb_str(char* str_ptr, char* rsrc_name,
                        uint32 rsrc_number, char *pmic_name)
{
  char rsrc_num_str[5] = {0, 0, 0, 0, 0};

  if(str_ptr == NULL || rsrc_name == NULL || pmic_name == NULL)
  {
    PM_LOG_MSG_ERR_FATAL("Invalid Input Pointers");
  }

  numtostr(rsrc_number, rsrc_num_str);
  strlcpy(str_ptr, rsrc_name,    CMD_DB_STR_SIZE);
  strlcat(str_ptr, pmic_name,    CMD_DB_STR_SIZE);
  strlcat(str_ptr, rsrc_num_str, CMD_DB_STR_SIZE);

  return;
}

/* Helper function for pm_prm_aux_dev_init */
static void
pm_prm_aux_populate_rsrc_data(char                   *rsrc_type_name,
                              char                   *pmic_name,
                              char                   *cmd_db_name,
                              uint32                 rsrc_num,
                              pm_prm_rsrc_key_type   rsrc_key,
                              pm_prm_rsrc_data_type  *rsrc_data,
                              pm_prm_rsrc_type       rsrc_type)
{
  char              *rsrc_name = NULL;
  char              *rsrc_node = NULL;
  pm_err_flag_type  err_flag   = PM_ERR_FLAG_SUCCESS;
  if(rsrc_type_name == NULL || pmic_name == NULL ||
     cmd_db_name    == NULL || rsrc_key  >= PM_RSRC_KEY_INVALID ||
     rsrc_data      == NULL || rsrc_type >= PM_PRM_RSRC_INVALID)
  {
    PM_LOG_MSG_ERR_FATAL("Invalid Inputs");
  }

  pm_malloc(sizeof(char) * (MAX_STR_SIZE), (void**)&rsrc_name);
  pm_malloc(sizeof(char) * (MAX_STR_SIZE), (void**)&rsrc_node);

  if(rsrc_name == NULL || rsrc_node == NULL)
  {
    PM_LOG_MSG_ERR_FATAL("Out of Memory");
    return;
  }

  pm_prm_create_node_str(rsrc_node, rsrc_type_name, rsrc_num, pmic_name);

  switch(rsrc_key)
  {
    case PM_RSRC_KEY_VOLTAGE:
      pm_prm_create_rsrc_str(rsrc_name, rsrc_type_name, "mV", rsrc_num, pmic_name);
      rsrc_data->rsrc_units       = "mV";
      rsrc_data->rsrc_key         = PM_RSRC_KEY_VOLTAGE;
      rsrc_data->rpmh_data.offset = PM_VREG_VOLTAGE_OFFSET;
      break;

    case PM_RSRC_KEY_ENABLE:
      pm_prm_create_rsrc_str(rsrc_name, rsrc_type_name, "en", rsrc_num, pmic_name);
      rsrc_data->rsrc_units       = "on_off";
      rsrc_data->rsrc_key         = PM_RSRC_KEY_ENABLE;
      rsrc_data->rpmh_data.offset =
                       (rsrc_type == PM_PRM_RSRC_CLK ? PM_CLK_ENABLE_OFFSET :
                                                      PM_VREG_ENABLE_OFFSET);
      break;

    case PM_RSRC_KEY_MODE:
      pm_prm_create_rsrc_str(rsrc_name, rsrc_type_name, "mode", rsrc_num, pmic_name);
      rsrc_data->rsrc_units       = "mode";
      rsrc_data->rsrc_key         = PM_RSRC_KEY_MODE;
      rsrc_data->rpmh_data.offset = PM_VREG_MODE_OFFSET;
      break;

    default:
      PM_LOG_MSG_ERR_FATAL("Invalid Rsrc Key");
      break;
  }

  rsrc_data->rsrc_name              = rsrc_name;
  rsrc_data->rsrc_node              = rsrc_node;
  rsrc_data->rsrc_type              = rsrc_type;
  rsrc_data->rsrc_vrm_type          = (rsrc_type == PM_PRM_RSRC_CLK ?
                                      PM_PRM_RSRC_VRM_CLK :
                                      PM_PRM_RSRC_VRM_VREG);
  rsrc_data->cmd_db_name            = cmd_db_name;
  rsrc_data->pmic_chip              = pmic_name;
  rsrc_data->ctrl_type              = PM_PRM_RPMH;
  rsrc_data->periph_id              = rsrc_num - 1; //since 0 based
  rsrc_data->pam_req                = 0;
  rsrc_data->rpmh_data.dirty_active = 0;
  rsrc_data->rpmh_data.dirty_sleep  = 0;

  /*Populates cmd_db address and auxi*/
  err_flag = pm_prm_os_get_vrm_data(rsrc_data);
  CORE_VERIFY((err_flag == PM_ERR_FLAG_SUCCESS));

  return;
}

/* Helper function for pm_prm_aux_dev_init */
static void
pm_prm_aux_dev_create(char             *rsrc_type_str,
                      uint32           num_rsrcs,
                      uint8            pmic_chip,
                      pm_prm_rsrc_type rsrc_type)
{
  char                     *en_key_name   = NULL;
  char                     *cmd_db_name   = NULL;
  npa_query_handle         query_handle   = NULL;
  pm_prm_rsrc_data_type    *rsrc_data     = NULL;
  char                     *pmic_name     = NULL;
  npa_node_definition      *npa_node      = NULL;
  npa_resource_definition  *npa_rsrcs     = NULL;
  uint8                    j              = 0;
  uint32                   num_of_keys    = 0;
  uint32                   vrm_addr       = 0;

  pm_malloc(sizeof(char)*(PMIC_STR_SIZE), (void**)&pmic_name);
  if(pmic_name == NULL)
  {
    PM_LOG_MSG_ERR_FATAL("Falied to fetch memory for pmic chip");
  }

  pmic_name[0] = 'a' + (char)pmic_chip;
  pmic_name[1] = '\0';

  while(num_rsrcs)
  {
    //free the memory for test en key at end of while loop
    pm_malloc(sizeof(char)*(MAX_STR_SIZE), (void**)&en_key_name);
    pm_malloc(sizeof(char)*(CMD_DB_STR_SIZE),  (void**)&cmd_db_name);

    if(en_key_name == NULL || cmd_db_name == NULL)
    {
      PM_LOG_MSG_ERR_FATAL("Out of Memory");
    }

    /*Check if en resource is created, if its there then all resources shouold be available */
    pm_prm_create_rsrc_str(en_key_name, rsrc_type_str, "en", num_rsrcs, pmic_name);

    pm_prm_create_cmddb_str(cmd_db_name,  rsrc_type_str, num_rsrcs, pmic_name);

    vrm_addr = cmd_db_query_addr(cmd_db_name);

    query_handle = npa_create_query_handle(en_key_name);

    if(query_handle == NULL && vrm_addr != 0)
    {

      num_of_keys = (rsrc_type == PM_PRM_RSRC_CLK ?
                                 NUM_OF_CLK_KEYS :
                                 NUM_OF_VREG_KEYS);

      pm_malloc((num_of_keys * sizeof(pm_prm_rsrc_data_type)),
                (void**)&rsrc_data);

      if(rsrc_data == NULL)
      {
        PM_LOG_MSG_ERR_FATAL("Failed to fetch memory for resources");
      }

      pm_prm_aux_populate_rsrc_data(rsrc_type_str, pmic_name, cmd_db_name,
                                    num_rsrcs, PM_RSRC_KEY_ENABLE,
                                    &rsrc_data[0], rsrc_type);

      if(rsrc_type != PM_PRM_RSRC_CLK)
      {
        pm_prm_aux_populate_rsrc_data(rsrc_type_str, pmic_name, cmd_db_name,
                                      num_rsrcs, PM_RSRC_KEY_VOLTAGE,
                                      &rsrc_data[1], rsrc_type);

        pm_prm_aux_populate_rsrc_data(rsrc_type_str, pmic_name, cmd_db_name,
                                    num_rsrcs, PM_RSRC_KEY_MODE,
                                    &rsrc_data[2], rsrc_type);
      }

      pm_malloc(sizeof(npa_node_definition), (void**)&npa_node);

      if(npa_node == NULL)
      {
        PM_LOG_MSG_ERR_FATAL("Not enough memory to initialize npa nodes !!!");
      }

      pm_malloc(sizeof(npa_resource_definition) * num_of_keys, (void**)&npa_rsrcs);

      if(npa_rsrcs == NULL)
      {
        PM_LOG_MSG_ERR_FATAL("Not enough memory to initialize npa rsrc !!!");
      }

      /*Populate Node Data*/
      npa_node->name             = (char*)rsrc_data[0].rsrc_node;
      npa_node->driver_fcn       = pm_prm_scalar_driver_fcn;
      npa_node->attributes       = NPA_NODE_DEFAULT;
      npa_node->data             = NULL;
      npa_node->dependency_count = 0;
      npa_node->resources        = npa_rsrcs;
      npa_node->resource_count   = num_of_keys;

       /*Populate Resource Data*/
      for(j = 0; j < num_of_keys; j++)
      {
        npa_rsrcs[j].name       = (char*)rsrc_data[j].rsrc_name;
        npa_rsrcs[j].units      = (char*)rsrc_data[j].rsrc_units;
        npa_rsrcs[j].max        = 0xFFFF;

        pm_prm_os_set_device_plugin(&rsrc_data[j], &npa_rsrcs[j]);

        npa_rsrcs[j].attributes = (NPA_RESOURCE_DEFAULT | 
                                   NPA_RESOURCE_DRIVER_UNCONDITIONAL);
        npa_rsrcs[j].data       = (void*)&rsrc_data[j];
    
        if (bPMICStub == TRUE)
        {
          npa_stub_resource(rsrc_data[j].rsrc_name);
        }
      }
      
      if (bPMICStub == FALSE)
      {
          /*Create nodes for each resource*/
        npa_define_node (npa_node, NULL, NULL);
      }
    }

    if(query_handle != NULL)
    {
      npa_destroy_query_handle(query_handle);
    }

    /*Clean up local variables for next rsrc*/
    en_key_name  = NULL;
    cmd_db_name  = NULL;
    query_handle = NULL;
    rsrc_data    = NULL;
    npa_node     = NULL;
    npa_rsrcs    = NULL;
    j            = 0;
    num_rsrcs--;
  }
  return;
}


void
pm_prm_os_aux_dev_init(void)
{
  pm_ldo_data_type  *ldo_data  = NULL;
  pm_smps_data_type *smps_data = NULL;
  pm_vs_data_type   *vs_data   = NULL;
  pm_clk_data_type  *clk_data  = NULL;
  uint8             i          = 0;
  uint8             j          = 0;
  uint32            num_rsrcs  = 0;
  uint32            num_of_clk = 0;
  uint8             num_of_rfclk =0, num_of_lnbbclk = 0, num_of_divclk = 0;

  for(i=0; i<PM_MAX_NUM_PMICS; i++)
  {
    ldo_data = pm_ldo_get_data(i);

    if (ldo_data != NULL)
    {
      num_rsrcs = ldo_data->pm_pwr_data.num_of_peripherals;
      pm_prm_aux_dev_create("ldo", num_rsrcs, i, PM_PRM_RSRC_LDO);
      num_rsrcs = 0;
    }
  }//end of ldo

  for(i=0; i<PM_MAX_NUM_PMICS; i++)
  {
    smps_data = pm_smps_get_data(i);

    if (smps_data != NULL)
    {
      num_rsrcs = smps_data->pm_pwr_data.num_of_peripherals;
      pm_prm_aux_dev_create("smp", num_rsrcs, i, PM_PRM_RSRC_SMPS);
      num_rsrcs = 0;
    }
  }//end of smps

  for(i=0; i<PM_MAX_NUM_PMICS; i++)
  {
    vs_data = pm_vs_get_data(i);

    if (vs_data != NULL)
    {
      num_rsrcs = vs_data->pm_pwr_data.num_of_peripherals;
      pm_prm_aux_dev_create("vs", num_rsrcs, i, PM_PRM_RSRC_VS);
      num_rsrcs = 0;
    }
  }//end of vs

  for(i=0; i<PM_MAX_NUM_PMICS; i++)
  {
    clk_data = pm_clk_get_data(i);
    if(clk_data != NULL)
    {
      num_rsrcs = clk_data->num_of_peripherals;
      num_of_clk = sizeof(clk_data->periph_subtype)/sizeof(clk_data->periph_subtype[0]);
      for(j = 0; j < num_of_clk; j++)
      {
        switch(clk_data->periph_subtype[j])
        {
          case PM_HW_MODULE_CLOCK_RF_CLK:
            num_of_rfclk += 1;
            break;

          case PM_HW_MODULE_CLOCK_LNBB_CLK:
            num_of_lnbbclk += 1;
            break;

          case PM_HW_MODULE_CLOCK_DIV_CLK:
            num_of_divclk +=1;
            break;
          default:
            break;
        }
      }
      pm_prm_aux_dev_create("rfclk",  num_of_rfclk,   i, PM_PRM_RSRC_CLK);
      pm_prm_aux_dev_create("lnbclk", num_of_lnbbclk, i, PM_PRM_RSRC_CLK);
      pm_prm_aux_dev_create("divclk", num_of_divclk,  i, PM_PRM_RSRC_CLK);
      num_of_rfclk = 0;
      num_of_lnbbclk = 0;
      num_of_divclk = 0;
    }
  }//end of clks
  return;
}

void
pm_prm_os_rem_dev_log( char *log_name, struct npa_resource *resource )
{
  return;
}