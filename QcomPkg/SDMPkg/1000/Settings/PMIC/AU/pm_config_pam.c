
/*! \file  pm_config_pam.c
 *  
 *  \brief  File Contains the PMIC NPA PAM Data
 *  \details  This file contains the PAM data for the PMIC NPA layer.
 *  
 *    Platform: SDM855.XX.1.0
 *    Revision: NO REVISIONS (Working Set)
 *    Template Version: 
 *  
 *  &copy; Copyright 2019 Qualcomm Technologies Incorporated, All Rights Reserved
 */

/*===========================================================================

                        INCLUDE HEADER FILES

===========================================================================*/

#include "pmapp_npa.h"
#include "pm_prm_device_defs.h"
#include "npa_remote_resource.h"

/*===========================================================================

                        MACRO DEFINITIONS

===========================================================================*/
#define PMIC_NPA_CLIENT_NODE_DISP_DP_ALT "/node/pmic/client/disp_dp_alt"
#define PMIC_NPA_CLIENT_NODE_DISP_EDP "/node/pmic/client/disp_edp"
#define PMIC_NPA_CLIENT_NODE_DISP_EXT_DP "/node/pmic/client/disp_ext_dp"
#define PMIC_NPA_CLIENT_NODE_DISP_PRIM "/node/pmic/client/disp_prim"
#define PMIC_NPA_CLIENT_NODE_UFS "/node/pmic/client/ufs"
#define PMIC_NPA_CLIENT_NODE_UFS0 "/node/pmic/client/ufs0"
#define PMIC_NPA_CLIENT_NODE_UFS1 "/node/pmic/client/ufs1"
#define PMIC_NPA_CLIENT_NODE_USB_SS0 "/node/pmic/client/usb_ss0"
#define PMIC_NPA_CLIENT_NODE_USB_SS1 "/node/pmic/client/usb_ss1"
#define PMIC_NPA_CLIENT_NODE_USB_SS1_A "/node/pmic/client/usb_ss1_a"
#define PMIC_NPA_CLIENT_NODE_USB_SS1_B "/node/pmic/client/usb_ss1_b"
#define PMIC_NPA_CLIENT_NODE_PCIE "/node/pmic/client/pcie"
/*===========================================================================

                        VARIABLES DEFINITION

===========================================================================*/

/*************************** DISP_DP_ALT Client ***************************/

const pm_prm_vreg_data_type
pm_pam_disp_dp_alt_a_ldo9 [] =
{
     /*     mode    voltage(mV)  enable*/ 
    { PM_PRM_VREG_MODE_LPM, 1200, PM_PRM_DEV_DISABLE },  // GENERIC_OFF
    { PM_PRM_VREG_MODE_LPM, 1200, PM_PRM_DEV_ENABLE },  // GENERIC_STANDBY
    { PM_PRM_VREG_MODE_NPM, 1200, PM_PRM_DEV_ENABLE }  // GENERIC_ACTIVE
};

const pm_prm_vreg_data_type
pm_pam_disp_dp_alt_e_ldo5 [] =
{
     /*     mode    voltage(mV)  enable*/ 
    { PM_PRM_VREG_MODE_LPM, 880, PM_PRM_DEV_DISABLE },  // GENERIC_OFF
    { PM_PRM_VREG_MODE_LPM, 880, PM_PRM_DEV_ENABLE },  // GENERIC_STANDBY
    { PM_PRM_VREG_MODE_NPM, 880, PM_PRM_DEV_ENABLE }  // GENERIC_ACTIVE
};

const pm_prm_pam_client_cfg_type
pm_pam_disp_dp_alt_rails_info [] = 
{
    { (void*)pm_pam_disp_dp_alt_a_ldo9, PM_PRM_RSRC_VRM_VREG },
    { (void*)pm_pam_disp_dp_alt_e_ldo5, PM_PRM_RSRC_VRM_VREG },
}; 

npa_node_dependency
pm_pam_deps_disp_dp_alt [] =
{
    PM_PRM_CLIENT_VREG_DEP ( a,  ldo, 9 ),
    PM_PRM_CLIENT_VREG_DEP ( e,  ldo, 5 ),
};

/*************************** DISP_EDP Client ***************************/

const pm_prm_vreg_data_type
pm_pam_disp_edp_a_ldo9 [] =
{
     /*     mode    voltage(mV)  enable*/ 
    { PM_PRM_VREG_MODE_LPM, 1200, PM_PRM_DEV_DISABLE },  // GENERIC_OFF
    { PM_PRM_VREG_MODE_LPM, 1200, PM_PRM_DEV_ENABLE },  // GENERIC_STANDBY
    { PM_PRM_VREG_MODE_NPM, 1200, PM_PRM_DEV_ENABLE }  // GENERIC_ACTIVE
};

const pm_prm_vreg_data_type
pm_pam_disp_edp_e_ldo5 [] =
{
     /*     mode    voltage(mV)  enable*/ 
    { PM_PRM_VREG_MODE_LPM, 880, PM_PRM_DEV_DISABLE },  // GENERIC_OFF
    { PM_PRM_VREG_MODE_LPM, 880, PM_PRM_DEV_ENABLE },  // GENERIC_STANDBY
    { PM_PRM_VREG_MODE_NPM, 880, PM_PRM_DEV_ENABLE }  // GENERIC_ACTIVE
};

const pm_prm_pam_client_cfg_type
pm_pam_disp_edp_rails_info [] = 
{
    { (void*)pm_pam_disp_edp_a_ldo9, PM_PRM_RSRC_VRM_VREG },
    { (void*)pm_pam_disp_edp_e_ldo5, PM_PRM_RSRC_VRM_VREG },
}; 

npa_node_dependency
pm_pam_deps_disp_edp [] =
{
    PM_PRM_CLIENT_VREG_DEP ( a,  ldo, 9 ),
    PM_PRM_CLIENT_VREG_DEP ( e,  ldo, 5 ),
};

/*************************** DISP_EXT_DP Client ***************************/

const pm_prm_vreg_data_type
pm_pam_disp_ext_dp_a_ldo9 [] =
{
     /*     mode    voltage(mV)  enable*/ 
    { PM_PRM_VREG_MODE_LPM, 1200, PM_PRM_DEV_DISABLE },  // GENERIC_OFF
    { PM_PRM_VREG_MODE_LPM, 1200, PM_PRM_DEV_ENABLE },  // GENERIC_STANDBY
    { PM_PRM_VREG_MODE_NPM, 1200, PM_PRM_DEV_ENABLE }  // GENERIC_ACTIVE
};

const pm_prm_vreg_data_type
pm_pam_disp_ext_dp_e_ldo5 [] =
{
     /*     mode    voltage(mV)  enable*/ 
    { PM_PRM_VREG_MODE_LPM, 880, PM_PRM_DEV_DISABLE },  // GENERIC_OFF
    { PM_PRM_VREG_MODE_LPM, 880, PM_PRM_DEV_ENABLE },  // GENERIC_STANDBY
    { PM_PRM_VREG_MODE_NPM, 880, PM_PRM_DEV_ENABLE }  // GENERIC_ACTIVE
};

const pm_prm_pam_client_cfg_type
pm_pam_disp_ext_dp_rails_info [] = 
{
    { (void*)pm_pam_disp_ext_dp_a_ldo9, PM_PRM_RSRC_VRM_VREG },
    { (void*)pm_pam_disp_ext_dp_e_ldo5, PM_PRM_RSRC_VRM_VREG },
}; 

npa_node_dependency
pm_pam_deps_disp_ext_dp [] =
{
    PM_PRM_CLIENT_VREG_DEP ( a,  ldo, 9 ),
    PM_PRM_CLIENT_VREG_DEP ( e,  ldo, 5 ),
};

/*************************** DISP_PRIM Client ***************************/

const pm_prm_vreg_data_type
pm_pam_disp_prim_a_ldo9 [] =
{
     /*     mode    voltage(mV)  enable*/ 
    { PM_PRM_VREG_MODE_LPM, 1200, PM_PRM_DEV_DISABLE },  // GENERIC_OFF
    { PM_PRM_VREG_MODE_LPM, 1200, PM_PRM_DEV_ENABLE },  // GENERIC_STANDBY
    { PM_PRM_VREG_MODE_NPM, 1200, PM_PRM_DEV_ENABLE }  // GENERIC_ACTIVE
};

const pm_prm_vreg_data_type
pm_pam_disp_prim_e_ldo5 [] =
{
     /*     mode    voltage(mV)  enable*/ 
    { PM_PRM_VREG_MODE_LPM, 880, PM_PRM_DEV_DISABLE },  // GENERIC_OFF
    { PM_PRM_VREG_MODE_LPM, 880, PM_PRM_DEV_ENABLE },  // GENERIC_STANDBY
    { PM_PRM_VREG_MODE_NPM, 880, PM_PRM_DEV_ENABLE }  // GENERIC_ACTIVE
};

const pm_prm_pam_client_cfg_type
pm_pam_disp_prim_rails_info [] = 
{
    { (void*)pm_pam_disp_prim_a_ldo9, PM_PRM_RSRC_VRM_VREG },
    { (void*)pm_pam_disp_prim_e_ldo5, PM_PRM_RSRC_VRM_VREG },
}; 

npa_node_dependency
pm_pam_deps_disp_prim [] =
{
    PM_PRM_CLIENT_VREG_DEP ( a,  ldo, 9 ),
    PM_PRM_CLIENT_VREG_DEP ( e,  ldo, 5 ),
};

/*************************** UFS Client ***************************/

const pm_prm_vreg_data_type
pm_pam_ufs_a_ldo11 [] =
{
     /*     mode    voltage(mV)  enable*/ 
    { PM_PRM_VREG_MODE_LPM, 1200, PM_PRM_DEV_DISABLE },  // GENERIC_OFF
    { PM_PRM_VREG_MODE_LPM, 1200, PM_PRM_DEV_ENABLE },  // GENERIC_STANDBY
    { PM_PRM_VREG_MODE_NPM, 1200, PM_PRM_DEV_ENABLE }  // GENERIC_ACTIVE
};

const pm_prm_vreg_data_type
pm_pam_ufs_e_ldo7 [] =
{
     /*     mode    voltage(mV)  enable*/ 
    { PM_PRM_VREG_MODE_LPM, 1800, PM_PRM_DEV_DISABLE },  // GENERIC_OFF
    { PM_PRM_VREG_MODE_LPM, 1800, PM_PRM_DEV_ENABLE },  // GENERIC_STANDBY
    { PM_PRM_VREG_MODE_NPM, 1800, PM_PRM_DEV_ENABLE }  // GENERIC_ACTIVE
};

const pm_prm_vreg_data_type
pm_pam_ufs_e_ldo10 [] =
{
     /*     mode    voltage(mV)  enable*/ 
    { PM_PRM_VREG_MODE_LPM, 2904, PM_PRM_DEV_DISABLE },  // GENERIC_OFF
    { PM_PRM_VREG_MODE_LPM, 2904, PM_PRM_DEV_ENABLE },  // GENERIC_STANDBY
    { PM_PRM_VREG_MODE_NPM, 2904, PM_PRM_DEV_ENABLE }  // GENERIC_ACTIVE
};

const pm_prm_pam_client_cfg_type
pm_pam_ufs_rails_info [] = 
{
    { (void*)pm_pam_ufs_a_ldo11, PM_PRM_RSRC_VRM_VREG },
    { (void*)pm_pam_ufs_e_ldo7, PM_PRM_RSRC_VRM_VREG },
    { (void*)pm_pam_ufs_e_ldo10, PM_PRM_RSRC_VRM_VREG },
}; 

npa_node_dependency
pm_pam_deps_ufs [] =
{
    PM_PRM_CLIENT_VREG_DEP ( a,  ldo, 11 ),
    PM_PRM_CLIENT_VREG_DEP ( e,  ldo, 7 ),
    PM_PRM_CLIENT_VREG_DEP ( e,  ldo, 10 ),
};

/*************************** UFS0 Client ***************************/

const pm_prm_vreg_data_type
pm_pam_ufs0_a_ldo11 [] =
{
     /*     mode    voltage(mV)  enable*/ 
    { PM_PRM_VREG_MODE_LPM, 1200, PM_PRM_DEV_DISABLE },  // GENERIC_OFF
    { PM_PRM_VREG_MODE_LPM, 1200, PM_PRM_DEV_ENABLE },  // GENERIC_STANDBY
    { PM_PRM_VREG_MODE_NPM, 1200, PM_PRM_DEV_ENABLE },  // GENERIC_ACTIVE
    { PM_PRM_VREG_MODE_NPM, 1200, PM_PRM_DEV_ENABLE },  // GENERIC_MV
    { PM_PRM_VREG_MODE_NPM, 1200, PM_PRM_DEV_ENABLE }  // GENERIC_HV
};

const pm_prm_vreg_data_type
pm_pam_ufs0_e_ldo7 [] =
{
     /*     mode    voltage(mV)  enable*/ 
    { PM_PRM_VREG_MODE_LPM, 1800, PM_PRM_DEV_DISABLE },  // GENERIC_OFF
    { PM_PRM_VREG_MODE_LPM, 1800, PM_PRM_DEV_ENABLE },  // GENERIC_STANDBY
    { PM_PRM_VREG_MODE_NPM, 1800, PM_PRM_DEV_ENABLE },  // GENERIC_ACTIVE
    { PM_PRM_VREG_MODE_NPM, 1800, PM_PRM_DEV_ENABLE },  // GENERIC_MV
    { PM_PRM_VREG_MODE_NPM, 1800, PM_PRM_DEV_ENABLE }  // GENERIC_HV
};

const pm_prm_vreg_data_type
pm_pam_ufs0_e_ldo10 [] =
{
     /*     mode    voltage(mV)  enable*/ 
    { PM_PRM_VREG_MODE_LPM, 2904, PM_PRM_DEV_DISABLE },  // GENERIC_OFF
    { PM_PRM_VREG_MODE_LPM, 2904, PM_PRM_DEV_ENABLE },  // GENERIC_STANDBY
    { PM_PRM_VREG_MODE_NPM, 2504, PM_PRM_DEV_ENABLE },  // GENERIC_ACTIVE
    { PM_PRM_VREG_MODE_NPM, 2904, PM_PRM_DEV_ENABLE },  // GENERIC_MV
    { PM_PRM_VREG_MODE_NPM, 2904, PM_PRM_DEV_ENABLE }  // GENERIC_HV
};

const pm_prm_pam_client_cfg_type
pm_pam_ufs0_rails_info [] = 
{
    { (void*)pm_pam_ufs0_a_ldo11, PM_PRM_RSRC_VRM_VREG },
    { (void*)pm_pam_ufs0_e_ldo7, PM_PRM_RSRC_VRM_VREG },
    { (void*)pm_pam_ufs0_e_ldo10, PM_PRM_RSRC_VRM_VREG },
}; 

npa_node_dependency
pm_pam_deps_ufs0 [] =
{
    PM_PRM_CLIENT_VREG_DEP ( a,  ldo, 11 ),
    PM_PRM_CLIENT_VREG_DEP ( e,  ldo, 7 ),
    PM_PRM_CLIENT_VREG_DEP ( e,  ldo, 10 ),
};

/*************************** UFS1 Client ***************************/

const pm_prm_vreg_data_type
pm_pam_ufs1_a_ldo17 [] =
{
     /*     mode    voltage(mV)  enable*/ 
    { PM_PRM_VREG_MODE_LPM, 2904, PM_PRM_DEV_DISABLE },  // GENERIC_OFF
    { PM_PRM_VREG_MODE_LPM, 2904, PM_PRM_DEV_ENABLE },  // GENERIC_STANDBY
    { PM_PRM_VREG_MODE_NPM, 2504, PM_PRM_DEV_ENABLE },  // GENERIC_ACTIVE
    { PM_PRM_VREG_MODE_NPM, 2904, PM_PRM_DEV_ENABLE },  // GENERIC_MV
    { PM_PRM_VREG_MODE_NPM, 2904, PM_PRM_DEV_ENABLE }  // GENERIC_HV
};

const pm_prm_pam_client_cfg_type
pm_pam_ufs1_rails_info [] = 
{
    { (void*)pm_pam_ufs1_a_ldo17, PM_PRM_RSRC_VRM_VREG },
}; 

npa_node_dependency
pm_pam_deps_ufs1 [] =
{
    PM_PRM_CLIENT_VREG_DEP ( a,  ldo, 17 ),
};

/*************************** USB_SS0 Client ***************************/

const pm_prm_vreg_data_type
pm_pam_usb_ss0_a_ldo12 [] =
{
     /*     mode    voltage(mV)  enable*/ 
    { PM_PRM_VREG_MODE_LPM, 1800, PM_PRM_DEV_DISABLE },  // GENERIC_OFF
    { PM_PRM_VREG_MODE_LPM, 1800, PM_PRM_DEV_ENABLE },  // GENERIC_STANDBY
    { PM_PRM_VREG_MODE_NPM, 1800, PM_PRM_DEV_ENABLE }  // GENERIC_ACTIVE
};

const pm_prm_vreg_data_type
pm_pam_usb_ss0_a_ldo9 [] =
{
     /*     mode    voltage(mV)  enable*/ 
    { PM_PRM_VREG_MODE_LPM, 1200, PM_PRM_DEV_DISABLE },  // GENERIC_OFF
    { PM_PRM_VREG_MODE_LPM, 1200, PM_PRM_DEV_ENABLE },  // GENERIC_STANDBY
    { PM_PRM_VREG_MODE_NPM, 1200, PM_PRM_DEV_ENABLE }  // GENERIC_ACTIVE
};

const pm_prm_vreg_data_type
pm_pam_usb_ss0_e_ldo9 [] =
{
     /*     mode    voltage(mV)  enable*/ 
    { PM_PRM_VREG_MODE_LPM, 880, PM_PRM_DEV_DISABLE },  // GENERIC_OFF
    { PM_PRM_VREG_MODE_LPM, 880, PM_PRM_DEV_ENABLE },  // GENERIC_STANDBY
    { PM_PRM_VREG_MODE_NPM, 880, PM_PRM_DEV_ENABLE }  // GENERIC_ACTIVE
};

const pm_prm_vreg_data_type
pm_pam_usb_ss0_e_ldo16 [] =
{
     /*     mode    voltage(mV)  enable*/ 
    { PM_PRM_VREG_MODE_LPM, 3072, PM_PRM_DEV_DISABLE },  // GENERIC_OFF
    { PM_PRM_VREG_MODE_LPM, 3072, PM_PRM_DEV_ENABLE },  // GENERIC_STANDBY
    { PM_PRM_VREG_MODE_NPM, 3072, PM_PRM_DEV_ENABLE }  // GENERIC_ACTIVE
};

const pm_prm_pam_client_cfg_type
pm_pam_usb_ss0_rails_info [] = 
{
    { (void*)pm_pam_usb_ss0_a_ldo12, PM_PRM_RSRC_VRM_VREG },
    { (void*)pm_pam_usb_ss0_a_ldo9, PM_PRM_RSRC_VRM_VREG },
    { (void*)pm_pam_usb_ss0_e_ldo9, PM_PRM_RSRC_VRM_VREG },
    { (void*)pm_pam_usb_ss0_e_ldo16, PM_PRM_RSRC_VRM_VREG },
}; 

npa_node_dependency
pm_pam_deps_usb_ss0 [] =
{
    PM_PRM_CLIENT_VREG_DEP ( a,  ldo, 12 ),
    PM_PRM_CLIENT_VREG_DEP ( a,  ldo, 9 ),
    PM_PRM_CLIENT_VREG_DEP ( e,  ldo, 9 ),
    PM_PRM_CLIENT_VREG_DEP ( e,  ldo, 16 ),
};
/*************************** USB_SS1_A Client ***************************/

const pm_prm_vreg_data_type
pm_pam_usb_ss1_a_a_ldo12 [] =
{
     /*     mode    voltage(mV)  enable*/ 
    { PM_PRM_VREG_MODE_LPM, 1800, PM_PRM_DEV_DISABLE },  // GENERIC_OFF
    { PM_PRM_VREG_MODE_LPM, 1800, PM_PRM_DEV_ENABLE },  // GENERIC_STANDBY
    { PM_PRM_VREG_MODE_NPM, 1800, PM_PRM_DEV_ENABLE }  // GENERIC_ACTIVE
};

const pm_prm_vreg_data_type
pm_pam_usb_ss1_a_a_ldo9 [] =
{
     /*     mode    voltage(mV)  enable*/ 
    { PM_PRM_VREG_MODE_LPM, 1200, PM_PRM_DEV_DISABLE },  // GENERIC_OFF
    { PM_PRM_VREG_MODE_LPM, 1200, PM_PRM_DEV_ENABLE },  // GENERIC_STANDBY
    { PM_PRM_VREG_MODE_NPM, 1200, PM_PRM_DEV_ENABLE }  // GENERIC_ACTIVE
};

const pm_prm_vreg_data_type
pm_pam_usb_ss1_a_e_ldo9 [] =
{
     /*     mode    voltage(mV)  enable*/ 
    { PM_PRM_VREG_MODE_LPM, 912, PM_PRM_DEV_DISABLE },  // GENERIC_OFF
    { PM_PRM_VREG_MODE_LPM, 912, PM_PRM_DEV_ENABLE },  // GENERIC_STANDBY
    { PM_PRM_VREG_MODE_NPM, 912, PM_PRM_DEV_ENABLE }  // GENERIC_ACTIVE
};

const pm_prm_vreg_data_type
pm_pam_usb_ss1_a_e_ldo16 [] =
{
     /*     mode    voltage(mV)  enable*/ 
    { PM_PRM_VREG_MODE_LPM, 3072, PM_PRM_DEV_DISABLE },  // GENERIC_OFF
    { PM_PRM_VREG_MODE_LPM, 3072, PM_PRM_DEV_ENABLE },  // GENERIC_STANDBY
    { PM_PRM_VREG_MODE_NPM, 3072, PM_PRM_DEV_ENABLE }  // GENERIC_ACTIVE
};

const pm_prm_pam_client_cfg_type
pm_pam_usb_ss1_a_rails_info [] = 
{
    { (void*)pm_pam_usb_ss1_a_a_ldo12, PM_PRM_RSRC_VRM_VREG },
    { (void*)pm_pam_usb_ss1_a_a_ldo9, PM_PRM_RSRC_VRM_VREG },
    { (void*)pm_pam_usb_ss1_a_e_ldo9, PM_PRM_RSRC_VRM_VREG },
    { (void*)pm_pam_usb_ss1_a_e_ldo16, PM_PRM_RSRC_VRM_VREG },
}; 

npa_node_dependency
pm_pam_deps_usb_ss1_a [] =
{
    PM_PRM_CLIENT_VREG_DEP ( a,  ldo, 12 ),
    PM_PRM_CLIENT_VREG_DEP ( a,  ldo, 9 ),
    PM_PRM_CLIENT_VREG_DEP ( e,  ldo, 9 ),
    PM_PRM_CLIENT_VREG_DEP ( e,  ldo, 16 ),
};

/*************************** USB_SS1_B Client ***************************/

const pm_prm_vreg_data_type
pm_pam_usb_ss1_b_a_ldo12 [] =
{
     /*     mode    voltage(mV)  enable*/ 
    { PM_PRM_VREG_MODE_LPM, 1800, PM_PRM_DEV_DISABLE },  // GENERIC_OFF
    { PM_PRM_VREG_MODE_LPM, 1800, PM_PRM_DEV_ENABLE },  // GENERIC_STANDBY
    { PM_PRM_VREG_MODE_NPM, 1800, PM_PRM_DEV_ENABLE }  // GENERIC_ACTIVE
};

const pm_prm_vreg_data_type
pm_pam_usb_ss1_b_a_ldo9 [] =
{
     /*     mode    voltage(mV)  enable*/ 
    { PM_PRM_VREG_MODE_LPM, 1200, PM_PRM_DEV_DISABLE },  // GENERIC_OFF
    { PM_PRM_VREG_MODE_LPM, 1200, PM_PRM_DEV_ENABLE },  // GENERIC_STANDBY
    { PM_PRM_VREG_MODE_NPM, 1200, PM_PRM_DEV_ENABLE }  // GENERIC_ACTIVE
};

const pm_prm_vreg_data_type
pm_pam_usb_ss1_b_e_ldo9 [] =
{
     /*     mode    voltage(mV)  enable*/ 
    { PM_PRM_VREG_MODE_LPM, 880, PM_PRM_DEV_DISABLE },  // GENERIC_OFF
    { PM_PRM_VREG_MODE_LPM, 880, PM_PRM_DEV_ENABLE },  // GENERIC_STANDBY
    { PM_PRM_VREG_MODE_NPM, 880, PM_PRM_DEV_ENABLE }  // GENERIC_ACTIVE
};

const pm_prm_vreg_data_type
pm_pam_usb_ss1_b_e_ldo16 [] =
{
     /*     mode    voltage(mV)  enable*/ 
    { PM_PRM_VREG_MODE_LPM, 3072, PM_PRM_DEV_DISABLE },  // GENERIC_OFF
    { PM_PRM_VREG_MODE_LPM, 3072, PM_PRM_DEV_ENABLE },  // GENERIC_STANDBY
    { PM_PRM_VREG_MODE_NPM, 3072, PM_PRM_DEV_ENABLE }  // GENERIC_ACTIVE
};

const pm_prm_pam_client_cfg_type
pm_pam_usb_ss1_b_rails_info [] = 
{
    { (void*)pm_pam_usb_ss1_b_a_ldo12, PM_PRM_RSRC_VRM_VREG },
    { (void*)pm_pam_usb_ss1_b_a_ldo9, PM_PRM_RSRC_VRM_VREG },
    { (void*)pm_pam_usb_ss1_b_e_ldo9, PM_PRM_RSRC_VRM_VREG },
    { (void*)pm_pam_usb_ss1_b_e_ldo16, PM_PRM_RSRC_VRM_VREG },
}; 

npa_node_dependency
pm_pam_deps_usb_ss1_b [] =
{
    PM_PRM_CLIENT_VREG_DEP ( a,  ldo, 12 ),
    PM_PRM_CLIENT_VREG_DEP ( a,  ldo, 9 ),
    PM_PRM_CLIENT_VREG_DEP ( e,  ldo, 9 ),
    PM_PRM_CLIENT_VREG_DEP ( e,  ldo, 16 ),
};

/*************************** PCIE Client ***************************/

const pm_prm_vreg_data_type
pm_pam_pcie_a_ldo9 [] =
{
     /*     mode    voltage(mV)  enable*/
    { PM_PRM_VREG_MODE_LPM, 1200, PM_PRM_DEV_DISABLE },  // GENERIC_OFF
    { PM_PRM_VREG_MODE_LPM, 1200, PM_PRM_DEV_ENABLE },  // GENERIC_STANDBY
    { PM_PRM_VREG_MODE_NPM, 1200, PM_PRM_DEV_ENABLE }  // GENERIC_ACTIVE
};

const pm_prm_vreg_data_type
pm_pam_pcie_e_ldo5 [] =
{
     /*     mode    voltage(mV)  enable*/
    { PM_PRM_VREG_MODE_LPM, 880, PM_PRM_DEV_DISABLE },  // GENERIC_OFF
    { PM_PRM_VREG_MODE_LPM, 880, PM_PRM_DEV_ENABLE },  // GENERIC_STANDBY
    { PM_PRM_VREG_MODE_NPM, 880, PM_PRM_DEV_ENABLE }  // GENERIC_ACTIVE
};

const pm_prm_pam_client_cfg_type
pm_pam_pcie_rails_info [] =
{
    { (void*)pm_pam_pcie_a_ldo9, PM_PRM_RSRC_VRM_VREG },
    { (void*)pm_pam_pcie_e_ldo5, PM_PRM_RSRC_VRM_VREG },
};

npa_node_dependency
pm_pam_deps_pcie [] =
{
    PM_PRM_CLIENT_VREG_DEP ( a,  ldo, 9 ),
    PM_PRM_CLIENT_VREG_DEP ( e,  ldo, 5 ),
};

/*************************** NPA NODE RESOURCE ***************************/

const pm_prm_node_resource_info
pm_prm_pam_rsrcs [] =
{
   {
     PMIC_NPA_GROUP_ID_DISP_DP_ALT,  // Resource Name
    PMIC_NPA_MODE_ID_GENERIC_ACTIVE,   // Maximum Value
    NPA_RESOURCE_DEFAULT | NPA_RESOURCE_DRIVER_UNCONDITIONAL_FIRST, // Resource Attribute
    (void*) pm_pam_disp_dp_alt_rails_info, // Resource User Data
    PMIC_NPA_CLIENT_NODE_DISP_DP_ALT, // Node Name
    NPA_NODE_DEFAULT,  // Node Attributes
    NULL,
    pm_pam_deps_disp_dp_alt,  // Node Dependency
    NPA_ARRAY_SIZE(pm_pam_deps_disp_dp_alt),  // Deps Count
   },
   {
     PMIC_NPA_GROUP_ID_DISP_EDP,  // Resource Name
    PMIC_NPA_MODE_ID_GENERIC_ACTIVE,   // Maximum Value
    NPA_RESOURCE_SINGLE_CLIENT | NPA_RESOURCE_DRIVER_UNCONDITIONAL, // Resource Attribute
    (void*) pm_pam_disp_edp_rails_info, // Resource User Data
    PMIC_NPA_CLIENT_NODE_DISP_EDP, // Node Name
    NPA_NODE_DEFAULT,  // Node Attributes
    NULL,
    pm_pam_deps_disp_edp,  // Node Dependency
    NPA_ARRAY_SIZE(pm_pam_deps_disp_edp),  // Deps Count
   },
   {
     PMIC_NPA_GROUP_ID_DISP_EXT_DP,  // Resource Name
    PMIC_NPA_MODE_ID_GENERIC_ACTIVE,   // Maximum Value
    NPA_RESOURCE_SINGLE_CLIENT | NPA_RESOURCE_DRIVER_UNCONDITIONAL, // Resource Attribute
    (void*) pm_pam_disp_ext_dp_rails_info, // Resource User Data
    PMIC_NPA_CLIENT_NODE_DISP_EXT_DP, // Node Name
    NPA_NODE_DEFAULT,  // Node Attributes
    NULL,
    pm_pam_deps_disp_ext_dp,  // Node Dependency
    NPA_ARRAY_SIZE(pm_pam_deps_disp_ext_dp),  // Deps Count
   },
   {
     PMIC_NPA_GROUP_ID_DISP_PRIM,  // Resource Name
    PMIC_NPA_MODE_ID_GENERIC_ACTIVE,   // Maximum Value
    NPA_RESOURCE_SINGLE_CLIENT | NPA_RESOURCE_DRIVER_UNCONDITIONAL, // Resource Attribute
    (void*) pm_pam_disp_prim_rails_info, // Resource User Data
    PMIC_NPA_CLIENT_NODE_DISP_PRIM, // Node Name
    NPA_NODE_DEFAULT,  // Node Attributes
    NULL,
    pm_pam_deps_disp_prim,  // Node Dependency
    NPA_ARRAY_SIZE(pm_pam_deps_disp_prim),  // Deps Count
   },
   {
    PMIC_NPA_GROUP_ID_UFS,  // Resource Name
    PMIC_NPA_MODE_ID_GENERIC_ACTIVE,   // Maximum Value
    NPA_RESOURCE_SINGLE_CLIENT | NPA_RESOURCE_DRIVER_UNCONDITIONAL, // Resource Attribute
    (void*) pm_pam_ufs_rails_info, // Resource User Data
    PMIC_NPA_CLIENT_NODE_UFS, // Node Name
    NPA_NODE_DEFAULT,  // Node Attributes
    NULL,
    pm_pam_deps_ufs,  // Node Dependency
    NPA_ARRAY_SIZE(pm_pam_deps_ufs),  // Deps Count
   },
   {
    PMIC_NPA_GROUP_ID_UFS0,  // Resource Name
    PMIC_NPA_MODE_ID_GENERIC_HV,   // Maximum Value
    NPA_RESOURCE_SINGLE_CLIENT | NPA_RESOURCE_DRIVER_UNCONDITIONAL, // Resource Attribute
    (void*) pm_pam_ufs0_rails_info, // Resource User Data
    PMIC_NPA_CLIENT_NODE_UFS0, // Node Name
    NPA_NODE_DEFAULT,  // Node Attributes
    NULL,
    pm_pam_deps_ufs0,  // Node Dependency
    NPA_ARRAY_SIZE(pm_pam_deps_ufs0),  // Deps Count
   },
   {
     PMIC_NPA_GROUP_ID_UFS1,  // Resource Name
    PMIC_NPA_MODE_ID_GENERIC_HV,   // Maximum Value
    NPA_RESOURCE_SINGLE_CLIENT | NPA_RESOURCE_DRIVER_UNCONDITIONAL, // Resource Attribute
    (void*) pm_pam_ufs1_rails_info, // Resource User Data
    PMIC_NPA_CLIENT_NODE_UFS1, // Node Name
    NPA_NODE_DEFAULT,  // Node Attributes
    NULL,
    pm_pam_deps_ufs1,  // Node Dependency
    NPA_ARRAY_SIZE(pm_pam_deps_ufs1),  // Deps Count
   },
   {
     PMIC_NPA_GROUP_ID_USB_SS0,  // Resource Name
    PMIC_NPA_MODE_ID_GENERIC_ACTIVE,   // Maximum Value
    NPA_RESOURCE_DEFAULT | NPA_RESOURCE_DRIVER_UNCONDITIONAL_FIRST, // Resource Attribute
    (void*) pm_pam_usb_ss0_rails_info, // Resource User Data
    PMIC_NPA_CLIENT_NODE_USB_SS0, // Node Name
    NPA_NODE_DEFAULT,  // Node Attributes
    NULL,
    pm_pam_deps_usb_ss0,  // Node Dependency
    NPA_ARRAY_SIZE(pm_pam_deps_usb_ss0),  // Deps Count
   },  
   {
     PMIC_NPA_GROUP_ID_USB_SS1_A,  // Resource Name
    PMIC_NPA_MODE_ID_GENERIC_ACTIVE,   // Maximum Value
    NPA_RESOURCE_DEFAULT | NPA_RESOURCE_DRIVER_UNCONDITIONAL_FIRST, // Resource Attribute
    (void*) pm_pam_usb_ss1_a_rails_info, // Resource User Data
    PMIC_NPA_CLIENT_NODE_USB_SS1_A, // Node Name
    NPA_NODE_DEFAULT,  // Node Attributes
    NULL,
    pm_pam_deps_usb_ss1_a,  // Node Dependency
    NPA_ARRAY_SIZE(pm_pam_deps_usb_ss1_a),  // Deps Count    
   },
   {
     PMIC_NPA_GROUP_ID_USB_SS1_B,  // Resource Name
    PMIC_NPA_MODE_ID_GENERIC_ACTIVE,   // Maximum Value
    NPA_RESOURCE_DEFAULT | NPA_RESOURCE_DRIVER_UNCONDITIONAL_FIRST, // Resource Attribute
    (void*) pm_pam_usb_ss1_b_rails_info, // Resource User Data
    PMIC_NPA_CLIENT_NODE_USB_SS1_B, // Node Name
    NPA_NODE_DEFAULT,  // Node Attributes
    NULL,
    pm_pam_deps_usb_ss1_b,  // Node Dependency
    NPA_ARRAY_SIZE(pm_pam_deps_usb_ss1_b),  // Deps Count    
   },
   {
    PMIC_NPA_GROUP_ID_PCIE,  // Resource Name
    PMIC_NPA_MODE_ID_GENERIC_ACTIVE,   // Maximum Value
    NPA_RESOURCE_SINGLE_CLIENT | NPA_RESOURCE_DRIVER_UNCONDITIONAL, // Resource Attribute
    (void*) pm_pam_pcie_rails_info, // Resource User Data
    PMIC_NPA_CLIENT_NODE_PCIE, // Node Name
    NPA_NODE_DEFAULT,  // Node Attributes
    NULL,
    pm_pam_deps_pcie,  // Node Dependency
    NPA_ARRAY_SIZE(pm_pam_deps_pcie),  // Deps Count
   },
};

uint32 num_of_pam_rsrcs [] = { sizeof(pm_prm_pam_rsrcs) / sizeof(pm_prm_node_resource_info) };


/*************************** Device Layer Resources ***************************/

pm_prm_rsrc_data_type
pm_prm_device_rsrc_data [] =
{
    PM_PRM_DEV_RSRC_VREG ( a, ldo, 9, PM_PRM_RSRC_LDO,  PM_PRM_RPMH, PM_LDO_9),
    PM_PRM_DEV_RSRC_VREG ( a, ldo, 11, PM_PRM_RSRC_LDO,  PM_PRM_RPMH, PM_LDO_11),
    PM_PRM_DEV_RSRC_VREG ( a, ldo, 12, PM_PRM_RSRC_LDO,  PM_PRM_RPMH, PM_LDO_12),
    PM_PRM_DEV_RSRC_VREG ( a, ldo, 17, PM_PRM_RSRC_LDO,  PM_PRM_RPMH, PM_LDO_17),
    PM_PRM_DEV_RSRC_VREG ( e, ldo, 5, PM_PRM_RSRC_LDO,  PM_PRM_RPMH, PM_LDO_5),
    PM_PRM_DEV_RSRC_VREG ( e, ldo, 7, PM_PRM_RSRC_LDO,  PM_PRM_RPMH, PM_LDO_7),
    PM_PRM_DEV_RSRC_VREG ( e, ldo, 9, PM_PRM_RSRC_LDO,  PM_PRM_RPMH, PM_LDO_9),
    PM_PRM_DEV_RSRC_VREG ( e, ldo, 10, PM_PRM_RSRC_LDO,  PM_PRM_RPMH, PM_LDO_10),
    PM_PRM_DEV_RSRC_VREG ( e, ldo, 16, PM_PRM_RSRC_LDO,  PM_PRM_RPMH, PM_LDO_16),
};

pm_prm_resource_type
pm_prm_device_rsrcs [] =
{
    {
        pm_prm_device_rsrc_data, 
        sizeof(pm_prm_device_rsrc_data)/sizeof(pm_prm_rsrc_data_type), //Number of Resources
        9
    }
};
