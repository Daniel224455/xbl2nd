/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*
GENERAL DESCRIPTION
  This file contains PAM Layer driver functions for PRM

EXTERNALIZED FUNCTIONS
  None.

INITIALIZATION AND SEQUENCING REQUIREMENTS
  None.

Copyright (c) 2016 QUALCOMM Technologies, Inc. (QTI). All Rights Reserved.
QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================
                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.


when       who     what, where, why
--------   ---     ----------------------------------------------------------
09/22/16   rl      Created
===========================================================================*/
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "comdef.h"
#include "npa.h"
#include "npa_resource.h"
#include "pm_prm_device_update.h"
#include "pm_prm_driver.h"
#include "pm_target_information.h"
#include "pm_log_utils.h"
#include "pm_utils.h"
#include "CoreVerify.h"
#include "pm_rpmh.h"
#include "pm_prm_device_defs.h"
#include "pm_prm_stubs.h"
#include "pmapp_npa.h"
#include "pm_prm_os.h"

/*===========================================================================

                FUNCTION PROTOTYPES

===========================================================================*/

void pm_prm_nas_cb_scalar(void *npa_rsrc);

/*===========================================================================

                FUNCTION DEFINITIONS

===========================================================================*/
/*
*  Driver function for vector and scalar requests for PAM Layer
*/
npa_resource_state
pm_prm_pam_driver_fcn (npa_resource      *resource,
                       npa_client_handle  client,
                       npa_resource_state mode_id)
{
  uint32             index          = 0;
  uint32             dep            = 0;
  uint32             current_msg_id = 0;
  npa_resource_state mode_id_req    = 0;

  if(resource == NULL || client == NULL)
  {
    PM_LOG_MSG_ERR_FATAL("Resource Pointer = %d, Client Pointer = %d",
                          resource, client);
  }

  if (client->type == NPA_CLIENT_INITIALIZE)
  {
    /*
     * Initial state of resources to be set if any, Apart from device initial
     * states
    */
  } else
  {
    /* If requested state is greater than max, error fatal*/
    if(resource->request_state > resource->active_max)
    {
      PM_LOG_MSG_ERR_FATAL("Invalid ModeId %d resource %s by client %s",
                          resource->request_state, client->resource_name,
                          client->name);
    }

    /* Use the aggregation corresponding to the current client->type.
     * The overall agg isn't important here as much as the individual
     * REQUIRED and SUPPRESSIBLE aggregations, because we have 2 client
     * handles to the dependency and it will aggregate across these anyway.
     */
    if (client->type == NPA_CLIENT_SUPPRESSIBLE)
    {
      dep = 1;
      mode_id_req = NPA_RESOURCE_SUPPRESSIBLE_REQUEST(resource);
    }
    else if(client->type == NPA_CLIENT_REQUIRED)
    {
      dep = 0;
      mode_id_req = NPA_RESOURCE_REQUIRED_REQUEST(resource);
    }
    else
    {
      PM_LOG_MSG_ERR_FATAL("Client Type Not Supported");
    }

    for (index = 0; dep < resource->node->dependency_count; index++)
    {
      dep += pm_prm_process_rsrc(client, resource, mode_id_req,
                                 index, dep, &current_msg_id);
    }

    pm_prm_barrier_request(client, current_msg_id, FALSE);

    PM_LOG_MSG_INFO("Done Rsrc=%s, Client=%s, MId=%d, Type=0x%x, attr=0x%x",
                     client->resource_name, client->name, mode_id,
                     client->type, client->request_attr);
  }
  return mode_id;
}

/*
*  Driver function scalar requests on Device Layer Resources
*/
npa_resource_state
pm_prm_scalar_driver_fcn (npa_resource      *resource,
                          npa_client_handle  client,
                          npa_resource_state state)
{

  pm_prm_rsrc_data_type *rsrc_key_data = NULL;
  uint32                msg_id         = 0;
  boolean               is_nas_req     = FALSE;
  rpmh_command_set_t    active_set;
  rpmh_command_set_t    sleep_set;

  if(resource == NULL || client == NULL)
  {
    PM_LOG_MSG_ERR_FATAL("Resource Pointer = %d, Client Pointer = %d",
                          resource, client);
  }

  rsrc_key_data=(pm_prm_rsrc_data_type*)resource->definition->data;

  if(rsrc_key_data == NULL)
  {
    PM_LOG_MSG_ERR_FATAL("Rsrc data is NULL");
  }

  if((rsrc_key_data->rpmh_data.vrm_ctrl_type == PM_PRM_VRM_XOB_CTRL &&
    rsrc_key_data->rsrc_key != PM_RSRC_KEY_ENABLE) ||
    (rsrc_key_data->rpmh_data.address == 0))
  {
    PM_LOG_MSG_INFO("Invalid Rsrc=%s", resource->definition->name);
    return 0;
  }

  DALSYS_memset(&active_set, 0, sizeof(rpmh_command_set_t));
  DALSYS_memset(&sleep_set,  0, sizeof(rpmh_command_set_t));

  /* For nas requests register call back and update nas count
   * NPA will call-back in LIFO manner, where the last request will
   * be called first, in call back rpmh sets will be updated to reflect
   * current aggregation per npa
   */
  if(npa_request_has_attribute(client, NPA_REQUEST_NEXT_AWAKE))
  {
    is_nas_req = TRUE;

    rsrc_key_data->rpmh_data.nas_req_cnt += 1;

    npa_notify_next_awake(pm_prm_nas_cb_scalar, (void *)resource);
  }

  /*  Immediate request is received but nas requests are still pending,
   *  it means that pmic got a request from IST context on wake-up before
   *  npa serviced the nas callback for the resource. In this case,
   *  clear out the nas request flag and honor the immediate request since
   *  the aggregation has changed.
   *  No need to lock the resource here as its locked by npa already.
   *  There is no chance of having concurrency issues since resource is
   *  also locked in the nas callback function before doing any operations
   *  Note: Immediate requests cannot come before system has gone through
   *        sleep-cycle as NAS requests are scheduled just before entering
   *        sleep per design
   */
  if(rsrc_key_data->rpmh_data.nas_req_cnt > 0 &&
     is_nas_req != TRUE)
  {
    PM_LOG_MSG_INFO("Imm Req before NAS CB for %s by %s",
                     client->resource_name,
                     resource->definition->name);

    /* Make nas request count zero for nas cb to ignore this rsrc */
    rsrc_key_data->rpmh_data.nas_req_cnt = 0;
  }

  if(client->type == NPA_CLIENT_REQUIRED ||
     client->type == NPA_CLIENT_SUPPRESSIBLE)
  {
    rsrc_key_data->rpmh_data.active_set   = resource->request_state;
    rsrc_key_data->rpmh_data.sleep_set    = NPA_RESOURCE_REQUIRED_REQUEST(resource);
    rsrc_key_data->rpmh_data.dirty_active = TRUE;
    rsrc_key_data->rpmh_data.dirty_sleep  = TRUE;
  }
  else
  {
    /** If initialization do nothing*/
    return state;
  }

  /* If direct resource request is made, send request to RPMh and barrier */
  if(rsrc_key_data->pam_req == FALSE)
  {
    if((rsrc_key_data->rpmh_data.vrm_ctrl_type == PM_PRM_VRM_LOCAL_CTRL ||
      rsrc_key_data->ctrl_type == PM_PRM_LOCAL) &&
      (rsrc_key_data->rpmh_data.dirty_active == TRUE))
    {
      pm_prm_os_set_local_device(rsrc_key_data);
    }
    else
    {
      pm_rpmh_create_cmd_set(&active_set, &sleep_set,
                            &rsrc_key_data->rpmh_data,
                            is_nas_req);

      pm_rpmh_post_set(&active_set, &sleep_set,
                      &rsrc_key_data->rpmh_data,
                      &msg_id, is_nas_req);

      pm_prm_barrier_request(client, msg_id, TRUE);
    }
    PM_LOG_MSG_INFO("Done Rsrc=%s, Client=%s, state=%d, Type=0x%x, attr=0x%x",
                     client->resource_name, client->name, state,
                     client->type, client->request_attr);
  }

return state;
}

/*
 * Scalar NAS Call-back
 */
void pm_prm_nas_cb_scalar(void *npa_rsrc)
{
  npa_resource          *resource      = NULL;
  pm_prm_rsrc_data_type *rsrc_key_data = NULL;
  uint32                 msg_id        = 0;
  rpmh_command_set_t     active_set;
  rpmh_command_set_t     sleep_set;

  DALSYS_memset(&active_set, 0, sizeof(rpmh_command_set_t));
  DALSYS_memset(&sleep_set,  0, sizeof(rpmh_command_set_t));

  if (npa_rsrc == NULL)
  {
    PM_LOG_MSG_ERR_FATAL("Invalid Resource in NAS Callback");
  }

  resource = (npa_resource*)npa_rsrc;

  /* Lock resource to protect against parallel on-going immediate reqeust */
  npa_resource_lock(resource);

  rsrc_key_data=(pm_prm_rsrc_data_type*)resource->definition->data;

  if (rsrc_key_data->rpmh_data.nas_req_cnt == 0)
  {
    /* Un-lock Resource */
    npa_resource_unlock(resource);
    return;
  }

  rsrc_key_data->rpmh_data.nas_req_cnt = 0;

  /* Use active state here instead of requst state since active
   *state is current aggregated value
   */
  rsrc_key_data->rpmh_data.active_set   = resource->active_state;
  rsrc_key_data->rpmh_data.sleep_set    = NPA_RESOURCE_REQUIRED_REQUEST(resource);
  rsrc_key_data->rpmh_data.dirty_active = TRUE;
  rsrc_key_data->rpmh_data.dirty_sleep  = TRUE;


  /*
   * Create command set based on the previous aggregated active
   * and sleep set values in rpmh data for the resource, this should only have
   * sleep set on RPMh drivr since active set is already posted as Next Awake
   * and RPMh driver would have made it Active set after wake-up from sleep.
   */
  pm_rpmh_create_cmd_set(&active_set, &sleep_set, &rsrc_key_data->rpmh_data, 0);

  /* MSG ID returned here will always be 0 as we will only be updating
   * sleep set and active set would be short circuited at rpmh layer
   */
  pm_rpmh_post_set(&active_set, &sleep_set,
                   &rsrc_key_data->rpmh_data,
                   &msg_id, 0);

  PM_LOG_MSG_INFO("Done NAS_CB: rsrc=%s, AS=%d, SS=%d, msg_id=%d",
                  resource->definition->name,
                  rsrc_key_data->rpmh_data.active_set,
                  rsrc_key_data->rpmh_data.sleep_set, msg_id);

  /* Un-lock resource */
  npa_resource_unlock(resource);
  return;
}

/*
 *  Helper function for posting barrier requests to RPMh
 */
void
pm_prm_barrier_request(npa_client_handle client,
                       uint32 msg_id,
                       boolean is_single)
{
  if(client == NULL)
  {
    PM_LOG_MSG_ERR_FATAL("Client Pointer = %d", client);
  }

  if (msg_id == 0 ||
      npa_request_has_attribute(client, NPA_REQUEST_FIRE_AND_FORGET))
  {
    return;
  }

  if(is_single)
  {
    pm_rpmh_barrier_single(msg_id);
  }
  else
  {
    pm_rpmh_barrier_all(msg_id);
  }

  return;
}


npa_query_status
pm_prm_query_fcn ( npa_resource *resource,
                   unsigned int query_id,
                   npa_query_type *query_result )
{
  uint32                     index             = 0;
  uint32                     dep               = 0;
  npa_resource_state         mode_id_req       = 0;
  pm_prm_pam_client_cfg_type *pam_data         = NULL;
  boolean                    rsrc_status       = FALSE;
  void                       *data_ptr         = NULL;
  npa_client_handle          client_handle_dep = NULL;

  if(resource==NULL || query_result==NULL)
  {
    PM_LOG_MSG_ERR_FATAL("Invalid Inputs");
  }

  if (query_id != PM_PRM_QUERY_PAM_CLIENT)
  {
    return NPA_QUERY_UNSUPPORTED_QUERY_ID;
  }

  pam_data = (pm_prm_pam_client_cfg_type *) resource->definition->data;

  for (index = 0; dep < resource->node->dependency_count; index++)
  {
    client_handle_dep = NPA_DEPENDENCY( resource, dep );

    mode_id_req = resource->active_state;

    switch(pam_data[index].rail_type)
    {
      case PM_PRM_RSRC_VRM_VREG:
        data_ptr    = (pm_prm_vreg_data_type *)pam_data[index].rail_data;
        data_ptr    = ((pm_prm_vreg_data_type *)data_ptr + mode_id_req);
        rsrc_status = pm_prm_os_get_vreg_status(client_handle_dep, data_ptr);
        dep        += (DEP_OFFSET * NUM_OF_VREG_KEYS);
        break;

      case PM_PRM_RSRC_VRM_CLK:
        data_ptr    = (pm_prm_clk_data_type *)pam_data[index].rail_data;
        data_ptr    = ((pm_prm_clk_data_type *)data_ptr + mode_id_req);
        rsrc_status = pm_prm_os_get_clk_status(client_handle_dep, data_ptr);
        dep        += (DEP_OFFSET * NUM_OF_CLK_KEYS);
        break;

      default:
        CORE_VERIFY(0);
    }

    query_result->data.value = (unsigned int)rsrc_status;

    if (rsrc_status==FALSE)
    {
      break;
    }
  }

  return  NPA_QUERY_SUCCESS ;
}


