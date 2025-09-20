#ifndef __PM_PRM_DEVICE_H__
#define __PM_PRM_DEVICE_H__
/*===========================================================================
DESCRIPTION
This file contains prototype definitions prm device layer

Copyright (c) 2017 QUALCOMM Technologies, Inc. (QTI). All Rights Reserved.  
QUALCOMM Proprietary.  Export of this technology or software is regulated 
by the U.S. Government. Diversion contrary to U.S. law prohibited.
===========================================================================*/

/*===========================================================================

EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.


when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
01/10/17   akm     Created
===========================================================================*/

/*===========================================================================

INCLUDE FILES

===========================================================================*/

#include "comdef.h"
#include "npa.h"
#include "npa_resource.h"
#include "rpmh_client.h"
#include "rpmh_common.h"
#include "pm_resources_and_types.h"
/*===========================================================================

TYPE DEFINITIONS

===========================================================================*/

/*! 
 * Control Type 
 */
typedef enum
{
  PM_PRM_RPMH,
  PM_PRM_LOCAL,
  PM_PRM_INVALID_CTRL
}pm_prm_ctrl_type;

/*! 
 * VRM Control Type 
 */
typedef enum{
  PM_PRM_VRM_LOCAL_CTRL = 0,
  PM_PRM_VRM_REG_CTRL = 1,
  PM_PRM_VRM_XOB_CTRL = 2,
  PM_PRM_VRM_INVALID_CTRL,
}pm_prm_vrm_ctrl_type;

/*! 
 * RPMh Priority Enum Mapping for Next Active Set Priority Considerations
 */
typedef enum
{
  PM_PRM_PRIORITY_DEFAULT   ,//= RPMH_RC_PRIORITY_USE_CASE,
  PM_PRM_PRIORITY_SUBSYSTEM ,//= RPMH_RC_PRIORITY_SUBSYSTEM,
  PM_PRM_PRIORITY_WAN       ,//= RPMH_RC_PRIORITY_QLINK,
  PM_PRM_PRIORITY_MAX       //= RPMH_RC_PRIORITY_MAX
}pm_prm_priority_type;

/*! 
 * RPMH Set Type 
 */
typedef struct
{
  uint32                 address; 
  uint32                 offset; 
  unsigned               active_set;    /*ActiveSetValue*/
  unsigned               sleep_set;     /*SleepSetValue*/
  boolean                dirty_active;  /*Active set dirty flag*/
  boolean                dirty_sleep;   /*Sleep set dirty flag*/
  uint32                 msg_id;        /*RPMh Message Id for Vectors*/
  uint32                 nas_req_cnt;
  pm_prm_vrm_ctrl_type   vrm_ctrl_type;
}pm_rpmh_data_type;

/*! 
 * VRM Resource Type 
 */
typedef enum
{
  PM_PRM_RSRC_VRM_VREG,
  PM_PRM_RSRC_VRM_CLK,
  PM_PRM_RSRC_VRM_INVALID
}pm_prm_rsrc_vrm_type;

/*! 
 * PMIC Resource Type 
 */
typedef enum
{
  PM_PRM_RSRC_LDO,
  PM_PRM_RSRC_SMPS,
  PM_PRM_RSRC_VS,
  PM_PRM_RSRC_CLK,
  PM_PRM_RSRC_INVALID
}pm_prm_rsrc_type;

/*! 
 * Resource key types 
 */
typedef enum
{
  PM_RSRC_KEY_MODE,
  PM_RSRC_KEY_VOLTAGE,
  PM_RSRC_KEY_ENABLE,
  PM_RSRC_KEY_INVALID
}pm_prm_rsrc_key_type;

/*! 
 * Generic enable,
 */
typedef enum
{
  PM_PRM_DEV_DISABLE = 0, // default
  PM_PRM_DEV_ENABLE  = 1,
  PM_PRM_DEV_INVALID
}pm_prm_generic_enable_type;


/*! 
 * SW MODE,
 */
typedef enum
{   
  /*PMIC 5 Modes*/
  PM_PRM_VREG_MODE_BYPASS     = 2, 
  PM_PRM_VREG_MODE_RETENTION  = 3,
  PM_PRM_VREG_MODE_LPM        = 4,
  PM_PRM_VREG_MODE_AUTO       = 6,
  PM_PRM_VREG_MODE_NPM        = 7,
  /*PMIC 4 Modes*/
  PM4_PRM_VREG_MODE_BYPASS    = 3,
  PM4_PRM_VREG_MODE_RETENTION = 4,
  PM4_PRM_VREG_MODE_LPM       = 5,
  PM4_PRM_VREG_MODE_AUTO      = 6,
  PM4_PRM_VREG_MODE_NPM       = 7,
  PM_PRM_VREG_MODE_INVALID
}pm_prm_vreg_mode_type;


/*! 
 * Device Resource Data 
 */
typedef struct
{ //Resource data
  const char              *rsrc_name;     /* NPA resource name */
  const char              *rsrc_node;     /* Name of resource node */
  pm_prm_rsrc_type        rsrc_type;      /* Type of resource on pmic: LDO, SMPS, CLK, VS*/
  pm_prm_rsrc_vrm_type    rsrc_vrm_type;  /* Rsrc Type on VRM Side Vreg or Buff */
  const char              *cmd_db_name;   /* Name to refer command db*/
  const char              *rsrc_units;    /* Resource Units */ 
  const char              *pmic_chip;     /* PMIC Chip for Resource */
  pm_prm_ctrl_type        ctrl_type;      /* Type of control, local or rpmh */
  uint8                   periph_id;      /* Driver layer peripheral ID for local Control */
  pm_prm_rsrc_key_type    rsrc_key;       /* Wheather resource is enable, voltage, etc */
  //Pam Request                           
  boolean                 pam_req;        /*Flag for notifying PAM Request*/
  //RPMh Data                             
  pm_rpmh_data_type       rpmh_data;      /*Data for RPMh Set information*/
}pm_prm_rsrc_data_type;

/*! 
 * Device Layer Data 
 */
typedef struct
{
  pm_prm_rsrc_data_type   *resource_data;
  unsigned int            num_rsrcs;
  unsigned int            num_rsrc_nodes;
}pm_prm_resource_type;

/*! 
 * PAM Layer Rail Data and type 
 */
typedef struct
{
  void                  *rail_data;
  pm_prm_rsrc_vrm_type  rail_type;
}pm_prm_pam_client_cfg_type;

/*! 
 * PAM Layer Node Data 
 */
typedef struct pm_prm_node_resource_info_type
{
  const char                  *group_name;          /* Name of resource */
  unsigned                    max_mode;            /* Maximum state/length of vector resource */
  unsigned int                resource_attributes; /* Resource Attributes */
  pm_prm_pam_client_cfg_type  *resource_data;       /* User data field  */

  const char                  *node_name;           /* Node name */
  unsigned int                node_attributes;     /* Node Attributes */
  npa_user_data               *node_data;           /* User data */
  npa_node_dependency         *node_dependencies;   /* Dep array */
  unsigned int                dependency_count;     /* Number of dependencies */
}pm_prm_node_resource_info;

/*! 
 * Publish PAM Clients 
 */
typedef struct
{
  uint32 domain;
  char** client_list;
  uint32 num_clients;
}pm_prm_pam_rsrc_pub_pd_type;

/*! 
 * Clock Resource Data 
 */
typedef struct
{
  npa_resource_state enable;
}pm_prm_clk_data_type; 

/*! 
 * VREG Resource Data 
 */
typedef struct
{
  npa_resource_state mode;
  npa_resource_state voltage;
  npa_resource_state enable;
}pm_prm_vreg_data_type; 

typedef struct
{
  pm_prm_node_resource_info const   *pm_prm_pam_rsrcs;
  uint32                            *num_of_pam_rsrcs;
  pm_prm_resource_type              *pm_prm_device_rsrcs;
  pm_prm_pam_rsrc_pub_pd_type       *pm_prm_pub_pam_rsrcs;
}pm_prm_pam_data_type;

/*===========================================================================

DEFINITIONS

===========================================================================*/
/*! 
 * CLK Resource keys and offsets 
 */
#define NUM_OF_CLK_KEYS             1
#define PM_CLK_ENABLE_OFFSET        4

/*! 
 * VREG Resource keys and offsets 
 */
#define NUM_OF_VREG_KEYS            3
#define PM_VREG_VOLTAGE_OFFSET      0
#define PM_VREG_ENABLE_OFFSET       4
#define PM_VREG_MODE_OFFSET         8


#define PM_PRM_RSRC_NAME(chipid, rtype, index, rkey)  \
        "/pm/" #rtype #chipid #index "/" #rkey


#define PM_PRM_NODE_NAME( chipid, rtype, index) \
        "/node/" #rtype #chipid #index
        
    
#endif /* __PM_PRM_DEVICE_H__ */

