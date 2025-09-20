/*! \file
 *  
 *  \brief  pm_prm_init.c ---- Initializes PRM PAM Client Layer 
 *  
 * Copyright (c) 2016 QUALCOMM Technologies, Inc. (QTI). All Rights Reserved.  
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
#include "pm_prm_device.h"
#include "pm_target_information.h"
#include "pm_log_utils.h"
#include "pm_utils.h"
#include "DDIPlatformInfo.h"
#include "CoreVerify.h"
#include "cmd_db.h"
#include "device_info.h"
#include "pm_prm_os.h"

/*===========================================================================

                MACRO DEFINITIONS

===========================================================================*/



/*===========================================================================

                VARIABLES DEFINITIONS

===========================================================================*/

/*PAM Layer Data*/
uint32                    *num_of_pam_nodes = 0;
npa_node_definition       *pm_prm_pam_nodes = NULL;
npa_resource_definition   *pm_prm_pam_rsrcs = NULL;

/*Device Layer Data*/
npa_resource_definition   *pm_prm_dev_rsrcs = NULL;
npa_node_definition       *pm_prm_dev_nodes = NULL;
pm_prm_resource_type      *pm_prm_rsrc_data = NULL;
boolean                   pm_prm_dev_log_disable = FALSE;

/*Stub Flag*/
extern boolean            bPMICStub;

/*===========================================================================

                FUNCTION PROTOTYPES

===========================================================================*/

static void 
pm_prm_rem_dev_log(char *log_name, struct npa_resource *resource);

static uint32 
pm_prm_num_rsrcs(pm_prm_rsrc_data_type *rsrc_data, uint32 i);

static pm_err_flag_type 
pm_prm_populate_vrm_data(pm_prm_rsrc_data_type *rsrc_data);

/*===========================================================================

                FUNCTION DEFINITIONS

===========================================================================*/
/* PAM Layer Init */
void
pm_prm_init(void)
{
  uint32 i = 0;
  pm_prm_node_resource_info const  *node_rsrc_array = NULL;
  pm_prm_node_resource_info const  *node_rsrc   = NULL;
  pm_prm_pam_data_type             *pam_data = NULL;

  /* Device Layer Init */
  pm_prm_device_init ();

  /* Get the PAM info */
  node_rsrc_array = (pm_prm_node_resource_info*)
              pm_target_information_get_specific_info(PM_PROP_PRM_PAM_RSRCS);

  num_of_pam_nodes = (uint32*)
          pm_target_information_get_specific_info(PM_PROP_PRM_PAM_RSRC_NUM);

  if ((num_of_pam_nodes == NULL) || (node_rsrc_array == NULL))
  {
    PM_LOG_MSG_ERR_FATAL("Failed to initialize PAM Client Layer !!!");
  }

  pm_malloc(sizeof(npa_node_definition)*(*num_of_pam_nodes),
            (void**)&pm_prm_pam_nodes);

  if(pm_prm_pam_nodes == NULL)
  {
    PM_LOG_MSG_ERR_FATAL("Not enough memory to initialize PAM Client nodes !!!");
  }

  pm_malloc(sizeof(npa_resource_definition)*(*num_of_pam_nodes),
           (void**)&pm_prm_pam_rsrcs);

  if(pm_prm_pam_rsrcs == NULL)
  {
    PM_LOG_MSG_ERR_FATAL("Not enough memory to initialize PAM Client Resources !!!");
  }

  /* Construct the PMIC Client Nodes and Resources */
  for(i = 0; i< (*num_of_pam_nodes); i++)
  {
    node_rsrc = &node_rsrc_array[i];

    pm_prm_pam_rsrcs[i].name             = node_rsrc->group_name;
    pm_prm_pam_rsrcs[i].units            = "ModeID";
    pm_prm_pam_rsrcs[i].max              = node_rsrc->max_mode;
    pm_prm_pam_rsrcs[i].attributes       = node_rsrc->resource_attributes;
    pm_prm_pam_rsrcs[i].data             = node_rsrc->resource_data;
    pm_prm_pam_rsrcs[i].query_fcn        = pm_prm_query_fcn;

    /* Populate Node Data */
    pm_prm_pam_nodes[i].name             = node_rsrc->node_name;
    pm_prm_pam_nodes[i].driver_fcn       = pm_prm_pam_driver_fcn;
    pm_prm_pam_nodes[i].attributes       = node_rsrc->node_attributes;
    pm_prm_pam_nodes[i].data             = node_rsrc->node_data;
    pm_prm_pam_nodes[i].dependencies     = node_rsrc->node_dependencies;
    pm_prm_pam_nodes[i].dependency_count = node_rsrc->dependency_count;
    pm_prm_pam_nodes[i].resource_count   = 1;
    pm_prm_pam_nodes[i].resources        = NULL;

    pm_prm_os_set_priority(node_rsrc);
    pm_prm_os_set_pam_plugin (&pm_prm_pam_nodes[i], &pm_prm_pam_rsrcs[i]);
  }

  //Register PMIC Client Layer Nodes and Resources with NPA
  for (i = 0; i < (*num_of_pam_nodes); i++)
  {
    pm_prm_pam_nodes[i].resources = &pm_prm_pam_rsrcs[i];
    
    if (bPMICStub == TRUE)
    {
      npa_stub_resource(pm_prm_pam_rsrcs[i].name);
    }
    else
    {
      npa_define_node (pm_prm_pam_nodes + i, NULL, NULL);
    }
  }

  /* Check for any resources that require publishing. */
  pm_prm_os_publish_rsrcs(pam_data);

} /* end of pm_prm_init() */

/* Device Layer Init */
void
pm_prm_device_init (void)
{
  uint32                num_of_rsrcs = 0;
  npa_resource          *resource    = NULL;
  npa_query_handle      query_handle = NULL;
  pm_prm_rsrc_data_type *rsrc_data   = NULL;
  uint32                i            = 0;
  uint32                j            = 0;
  uint32                node_num     = 0;
  pm_err_flag_type      err_flag     = PM_ERR_FLAG_SUCCESS;

  pm_prm_rsrc_data = (pm_prm_resource_type*)
           pm_target_information_get_specific_info(PM_PROP_PRM_DEVICE_RSRCS);

  if ((pm_prm_rsrc_data == NULL) || (pm_prm_rsrc_data->num_rsrcs <= 0))
  {
    return;
  }

  rsrc_data = pm_prm_rsrc_data->resource_data;

  pm_malloc(pm_prm_rsrc_data->num_rsrcs * sizeof(npa_resource_definition),
            (void**)&pm_prm_dev_rsrcs);

  pm_malloc(pm_prm_rsrc_data->num_rsrc_nodes * sizeof(npa_node_definition),
            (void**)&pm_prm_dev_nodes);

  for (i = 0; i < pm_prm_rsrc_data->num_rsrcs; i+=num_of_rsrcs, node_num++)
  {
    if(node_num >= pm_prm_rsrc_data->num_rsrc_nodes)
    {
      PM_LOG_MSG_ERR_FATAL("Failed PRM Device Init");
    }

    /*Get Number of Resources for Each Node*/
    num_of_rsrcs = pm_prm_num_rsrcs(rsrc_data, i);

    /*Populate Node Data*/
    pm_prm_dev_nodes[node_num].name            = (char*)rsrc_data[i].rsrc_node;
    pm_prm_dev_nodes[node_num].driver_fcn      = pm_prm_scalar_driver_fcn;
    pm_prm_dev_nodes[node_num].attributes      = NPA_NODE_DEFAULT;
    pm_prm_dev_nodes[node_num].data            = NULL;
    pm_prm_dev_nodes[node_num].dependency_count= 0;
    pm_prm_dev_nodes[node_num].resources       = &pm_prm_dev_rsrcs[i];
    pm_prm_dev_nodes[node_num].resource_count  = num_of_rsrcs;

    /*Populate Resource Data*/
    for(j=0; j<num_of_rsrcs; j++)
    {
      err_flag = pm_prm_os_get_vrm_data(&rsrc_data[i+j]);

      if (err_flag != PM_ERR_FLAG_SUCCESS)
      {

        PM_LOG_MSG_ERR_FATAL("Invalid cmd_db data for %s",
                               rsrc_data[i+j].rsrc_name);
      }

      pm_prm_dev_rsrcs[i+j].name       = (char*)rsrc_data[i+j].rsrc_name;
      pm_prm_dev_rsrcs[i+j].units      = (char*)rsrc_data[i+j].rsrc_units;
      pm_prm_dev_rsrcs[i+j].max        = 0xFFFF;
      
      /* Get correct plugin */
      pm_prm_os_set_device_plugin(&rsrc_data[i+j], &pm_prm_dev_rsrcs[i+j]);

      pm_prm_dev_rsrcs[i+j].attributes = (NPA_RESOURCE_DEFAULT |
                                     NPA_RESOURCE_DRIVER_UNCONDITIONAL_FIRST);
      pm_prm_dev_rsrcs[i+j].data       = (void*)&rsrc_data[i+j];

      /*Create Stub if bPMICStub is true*/
      if(bPMICStub == TRUE)
      {
        npa_stub_resource(rsrc_data[i+j].rsrc_name);
      }
    }

    if (bPMICStub == FALSE)
    {
      /*Create nodes for each resource*/
      npa_define_node (&pm_prm_dev_nodes[node_num], NULL, NULL);
    }

    if(pm_prm_dev_log_disable)
    {
      for(j=0; j<num_of_rsrcs; j++)
      {
        query_handle = npa_create_query_handle(pm_prm_dev_rsrcs[i+j].name);

        if (query_handle != NULL)
        {
          resource = npa_query_get_resource(query_handle);
        }

        if(resource != NULL)
        {
          pm_prm_os_rem_dev_log(NPA_DEFAULT_LOG_NAME, resource);
        }
      }
    } //End Log Disable
  } //End node Population Loop

  return;
}


static uint32
pm_prm_num_rsrcs(pm_prm_rsrc_data_type *rsrc_data, uint32 i)
{
  uint32 num_of_rsrcs = 0;

  if(rsrc_data == NULL)
  {
    PM_LOG_MSG_ERR_FATAL("Invalid Input");
  }

  /* If Scalar */
  if (rsrc_data[i].rsrc_vrm_type == PM_PRM_RSRC_VRM_VREG)
  {
    num_of_rsrcs  = NUM_OF_VREG_KEYS;
  }
  else if (rsrc_data[i].rsrc_vrm_type == PM_PRM_RSRC_VRM_CLK)
  {
    num_of_rsrcs  = NUM_OF_CLK_KEYS;
  }
  else
  {
    CORE_VERIFY(0);
  }

  return num_of_rsrcs;
}
