#ifndef __PM_PRM_OS_H__
#define __PM_PRM_OS_H__

/*! \file
 *
 *  \brief  pm_prm_os.h This file contains prototype definitions prm pam layer
 *    This file contains code for Target specific settings and modes.
 *
 *    Copyright (c) 2016 QUALCOMM Technologies, Inc. (QTI). All Rights Reserved.
 *    QUALCOMM Proprietary.  Export of this technology or software is regulated
 *    by the U.S. Government. Diversion contrary to U.S. law prohibited.
 *
 */
/*===========================================================================

                EDIT HISTORY FOR MODULE

  This document is created by a code generator, therefore this section will
  not contain comments describing changes made to the module over time.


===========================================================================*/

#include "pm_prm_device.h"
#include "pm_err_flags.h"

/******************************************************************************
 pm_prm_os_publish_rsrcs

 Publish PAM layer resources to USER PD's for inter-pd voting of resources
*****************************************************************************/
void
pm_prm_os_publish_rsrcs(pm_prm_pam_data_type *pam_data);

/******************************************************************************
 pm_prm_os_set_priority

 Sets priority of device layer dependencies of pam clients
 to the RPMH driver to differentiate between hierarchy in which
 Resource should be populated in NAS TCS's
*****************************************************************************/
void
pm_prm_os_set_priority(pm_prm_node_resource_info const  *node_rsrc);

/******************************************************************************
 pm_prm_os_get_vrm_data

 Populate vrm specific info obtained from CMD_DB in device layer resources
*****************************************************************************/
pm_err_flag_type
pm_prm_os_get_vrm_data(pm_prm_rsrc_data_type *rsrc_data);



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
                          npa_resource_definition *rsrc );


/******************************************************************************
 pm_prm_os_set_device_plugin

 Sets plugin for device layer resources based on local control or RPMH control
 Uses npa_max_plugin for rpmh controlled resources
 Usese pm_prm_os_max_plugin for local control resources
******************************************************************************/
void
pm_prm_os_set_device_plugin(pm_prm_rsrc_data_type *rsrc_data,
                            npa_resource_definition *rsrc);


/******************************************************************************
 pm_prm_os_set_local_device

 Sets appropriate mode, voltage and enable for local control devices.
 Supports mode, voltage and enable settings for LDO and SMPS local control
 Supports enable setting for clock local control
*****************************************************************************/
void
pm_prm_os_set_local_device(pm_prm_rsrc_data_type *rsrc_data);


/******************************************************************************
 pm_prm_os_get_vreg_status

 Gets physical status of an LDO, SMPS or VS upon an NPA Query and compares it
 to current aggregation. If the status at device layer is greater or equal
 to the aggregation in npa then returns a true value denoting that the resource
 is set correctly in the pmic device
******************************************************************************/
boolean
pm_prm_os_get_vreg_status(npa_client_handle client_handle,
                      pm_prm_vreg_data_type *pam_data);


/******************************************************************************
 pm_prm_os_get_vreg_status

 Gets physical status of an LDO, SMPS or VS upon an NPA Query and compares it
 to current aggregation. If the status at device layer is greater or equal
 to the aggregation in npa then returns a true value denoting that the resource
 is set correctly in the pmic device
******************************************************************************/
boolean
pm_prm_os_get_clk_status(npa_client_handle client_handle,
                      pm_prm_clk_data_type *pam_data);

/******************************************************************************
 pm_prm_os_aux_device_init

 Creates vote-able resource for all pmic devices present in cmd db
******************************************************************************/
void
pm_prm_os_aux_dev_init(void);

/******************************************************************************
 pm_prm_os_rem_dev_log

 Removes NPA Ulog Logging, not applicable for images like uefi
******************************************************************************/
void
pm_prm_os_rem_dev_log( char *log_name, struct npa_resource *resource ) ;


#endif //__PM_PRM_OS_H__



