
/*! \file  pm_config_pam.c
 *  
 *  \brief  File Contains the PMIC NPA PAM Data
 *  \details  This file contains the PAM data for the PMIC NPA layer.
 *  
 *    Platform: SDM845.XX.1.0
 *    Revision: NO REVISIONS (Working Set)
 *    Template Version: 
 *    $Header: //components/rel/boot.xf/2.2/QcomPkg/SDM845Pkg/Settings/PMIC/pm_config_pam.c#2 $ 
 *    $DateTime: 2018/03/13 23:30:29 $  $Author: pwbldsvc $
 *  
 *  &copy; Copyright March 21, 2018 Qualcomm Technologies Incorporated, All Rights Reserved
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

#define PMIC_NPA_CLIENT_NODE_CAMERA_BACK "/node/pmic/client/camera_back"
#define PMIC_NPA_CLIENT_NODE_CAMERA_FRONT "/node/pmic/client/camera_front"
#define PMIC_NPA_CLIENT_NODE_DISP_EXT_DP "/node/pmic/client/disp_ext_dp"
#define PMIC_NPA_CLIENT_NODE_DISP_PRIM "/node/pmic/client/disp_prim"
#define PMIC_NPA_CLIENT_NODE_PCIE "/node/pmic/client/pcie"
#define PMIC_NPA_CLIENT_NODE_USB_SS1 "/node/pmic/client/usb_ss1"
#define PMIC_NPA_CLIENT_NODE_USB_SS2 "/node/pmic/client/usb_ss2"
#define PMIC_NPA_CLIENT_NODE_USB_SS3 "/node/pmic/client/usb_ss3"
#define PMIC_NPA_CLIENT_NODE_DISP_PRIM_AMOLED "/node/pmic/client/disp_prim_amoled"
/*===========================================================================

                        VARIABLES DEFINITION

===========================================================================*/


/*************************** CAMERA_BACK Client ***************************/

const pm_prm_vreg_data_type
pm_pam_camera_back_a_ldo22 [] =
{
     /*     mode    voltage(mV)  headroom(mV)   enable*/ 
    { PM_PRM_VREG_MODE_LPM, 0, 0, PM_PRM_DEV_DISABLE },  // GENERIC_STANDBY
    { PM_PRM_VREG_MODE_NPM, 2800, 0, PM_PRM_DEV_ENABLE }  // GENERIC_ACTIVE
};

const pm_prm_vreg_data_type
pm_pam_camera_back_a_vs1 [] =
{
     /* mode, voltage, headroom, enable */ 
    { 0, 0, 0, PM_PRM_DEV_DISABLE },  // GENERIC_STANDBY
    { 0, 0, 0, PM_PRM_DEV_ENABLE }  // GENERIC_ACTIVE
};


const pm_prm_pam_client_cfg_type
pm_pam_camera_back_rails_info [] = 
{
    { (void*)pm_pam_camera_back_a_ldo22, PM_PRM_RSRC_VRM_VREG },
    { (void*)pm_pam_camera_back_a_vs1, PM_PRM_RSRC_VRM_VREG },
}; 

npa_node_dependency
pm_pam_deps_camera_back [] =
{
    PM_PRM_CLIENT_VREG_DEP ( a,  ldo, 22 ),
    PM_PRM_CLIENT_VREG_DEP ( a,  vs, 1 ),
};

/*************************** CAMERA_FRONT Client ***************************/

const pm_prm_vreg_data_type
pm_pam_camera_front_a_vs1 [] =
{
     /* mode, voltage, headroom, enable */ 
    { 0, 0, 0, PM_PRM_DEV_DISABLE },  // GENERIC_STANDBY
    { 0, 0, 0, PM_PRM_DEV_ENABLE }  // GENERIC_ACTIVE
};


const pm_prm_pam_client_cfg_type
pm_pam_camera_front_rails_info [] = 
{
    { (void*)pm_pam_camera_front_a_vs1, PM_PRM_RSRC_VRM_VREG },
}; 

npa_node_dependency
pm_pam_deps_camera_front [] =
{
    PM_PRM_CLIENT_VREG_DEP ( a,  vs, 1 ),
};

/*************************** DISP_EXT_DP Client ***************************/

const pm_prm_vreg_data_type
pm_pam_disp_ext_dp_a_ldo1 [] =
{
     /*     mode    voltage(mV)  headroom(mV)   enable*/ 
    { PM_PRM_VREG_MODE_LPM, 0, 0, PM_PRM_DEV_DISABLE },  // GENERIC_STANDBY
    { PM_PRM_VREG_MODE_NPM, 880, 0, PM_PRM_DEV_ENABLE }  // GENERIC_ACTIVE
};

const pm_prm_vreg_data_type
pm_pam_disp_ext_dp_a_ldo24 [] =
{
     /*     mode    voltage(mV)  headroom(mV)   enable*/ 
    { PM_PRM_VREG_MODE_LPM, 0, 0, PM_PRM_DEV_DISABLE },  // GENERIC_STANDBY
    { PM_PRM_VREG_MODE_NPM, 3088, 0, PM_PRM_DEV_ENABLE }  // GENERIC_ACTIVE
};

const pm_prm_vreg_data_type
pm_pam_disp_ext_dp_a_ldo26 [] =
{
     /*     mode    voltage(mV)  headroom(mV)   enable*/ 
    { PM_PRM_VREG_MODE_LPM, 0, 0, PM_PRM_DEV_DISABLE },  // GENERIC_STANDBY
    { PM_PRM_VREG_MODE_NPM, 1200, 0, PM_PRM_DEV_ENABLE }  // GENERIC_ACTIVE
};

const pm_prm_pam_client_cfg_type
pm_pam_disp_ext_dp_rails_info [] = 
{
    { (void*)pm_pam_disp_ext_dp_a_ldo1, PM_PRM_RSRC_VRM_VREG },
    { (void*)pm_pam_disp_ext_dp_a_ldo24, PM_PRM_RSRC_VRM_VREG },
    { (void*)pm_pam_disp_ext_dp_a_ldo26, PM_PRM_RSRC_VRM_VREG },
}; 

npa_node_dependency
pm_pam_deps_disp_ext_dp [] =
{
    PM_PRM_CLIENT_VREG_DEP ( a,  ldo, 1 ),
    PM_PRM_CLIENT_VREG_DEP ( a,  ldo, 24 ),
    PM_PRM_CLIENT_VREG_DEP ( a,  ldo, 26 ),
};

/*************************** DISP_PRIM Client ***************************/

const pm_prm_vreg_data_type
pm_pam_disp_prim_a_ldo1 [] =
{
     /*     mode    voltage(mV)  headroom(mV)   enable*/ 
    { PM_PRM_VREG_MODE_LPM, 0, 0, PM_PRM_DEV_DISABLE },  // GENERIC_STANDBY
    { PM_PRM_VREG_MODE_NPM, 880, 0, PM_PRM_DEV_ENABLE }  // GENERIC_ACTIVE
};

const pm_prm_vreg_data_type
pm_pam_disp_prim_a_ldo14 [] =
{
     /*     mode    voltage(mV)  headroom(mV)   enable*/ 
    { PM_PRM_VREG_MODE_LPM, 0, 0, PM_PRM_DEV_DISABLE },  // GENERIC_STANDBY
    { PM_PRM_VREG_MODE_NPM, 1880, 0, PM_PRM_DEV_ENABLE }  // GENERIC_ACTIVE
};

const pm_prm_vreg_data_type
pm_pam_disp_prim_a_ldo26 [] =
{
     /*     mode    voltage(mV)  headroom(mV)   enable*/ 
    { PM_PRM_VREG_MODE_LPM, 0, 0, PM_PRM_DEV_DISABLE },  // GENERIC_STANDBY
    { PM_PRM_VREG_MODE_NPM, 1200, 0, PM_PRM_DEV_ENABLE }  // GENERIC_ACTIVE
};

const pm_prm_pam_client_cfg_type
pm_pam_disp_prim_rails_info [] = 
{
    { (void*)pm_pam_disp_prim_a_ldo1, PM_PRM_RSRC_VRM_VREG },
    { (void*)pm_pam_disp_prim_a_ldo14, PM_PRM_RSRC_VRM_VREG },
    { (void*)pm_pam_disp_prim_a_ldo26, PM_PRM_RSRC_VRM_VREG },
}; 

npa_node_dependency
pm_pam_deps_disp_prim [] =
{
    PM_PRM_CLIENT_VREG_DEP ( a,  ldo, 1 ),
    PM_PRM_CLIENT_VREG_DEP ( a,  ldo, 14 ),
    PM_PRM_CLIENT_VREG_DEP ( a,  ldo, 26 ),
};

/*************************** DISP_PRIM_AMOLED Client ***************************/

const pm_prm_vreg_data_type
pm_pam_disp_prim_amoled_a_ldo1 [] =
{
     /*     mode    voltage(mV)  headroom(mV)   enable*/ 
    { PM_PRM_VREG_MODE_LPM, 0, 0, PM_PRM_DEV_DISABLE },  // GENERIC_STANDBY
    { PM_PRM_VREG_MODE_NPM, 880, 0, PM_PRM_DEV_ENABLE }  // GENERIC_ACTIVE
};

const pm_prm_vreg_data_type
pm_pam_disp_prim_amoled_a_ldo14 [] =
{
     /*     mode    voltage(mV)  headroom(mV)   enable*/ 
    { PM_PRM_VREG_MODE_LPM, 0, 0, PM_PRM_DEV_DISABLE },  // GENERIC_STANDBY
    { PM_PRM_VREG_MODE_NPM, 1880, 0, PM_PRM_DEV_ENABLE }  // GENERIC_ACTIVE
};

const pm_prm_vreg_data_type
pm_pam_disp_prim_amoled_a_ldo19 [] =
{
     /*     mode    voltage(mV)  headroom(mV)   enable*/ 
    { PM_PRM_VREG_MODE_LPM, 0, 0, PM_PRM_DEV_DISABLE },  // GENERIC_STANDBY
    { PM_PRM_VREG_MODE_NPM, 3000, 0, PM_PRM_DEV_ENABLE }  // GENERIC_ACTIVE
};

const pm_prm_vreg_data_type
pm_pam_disp_prim_amoled_a_ldo26 [] =
{
     /*     mode    voltage(mV)  headroom(mV)   enable*/ 
    { PM_PRM_VREG_MODE_LPM, 0, 0, PM_PRM_DEV_DISABLE },  // GENERIC_STANDBY
    { PM_PRM_VREG_MODE_NPM, 1200, 0, PM_PRM_DEV_ENABLE }  // GENERIC_ACTIVE
};

const pm_prm_pam_client_cfg_type
pm_pam_disp_prim_amoled_rails_info [] = 
{
    { (void*)pm_pam_disp_prim_amoled_a_ldo1, PM_PRM_RSRC_VRM_VREG },
    { (void*)pm_pam_disp_prim_amoled_a_ldo14, PM_PRM_RSRC_VRM_VREG },
    { (void*)pm_pam_disp_prim_amoled_a_ldo19, PM_PRM_RSRC_VRM_VREG },
    { (void*)pm_pam_disp_prim_amoled_a_ldo26, PM_PRM_RSRC_VRM_VREG },
}; 

npa_node_dependency
pm_pam_deps_disp_prim_amoled [] =
{
    PM_PRM_CLIENT_VREG_DEP ( a,  ldo, 1 ),
    PM_PRM_CLIENT_VREG_DEP ( a,  ldo, 14 ),
    PM_PRM_CLIENT_VREG_DEP ( a,  ldo, 19 ),
    PM_PRM_CLIENT_VREG_DEP ( a,  ldo, 26 ),
};

/*************************** PCIE Client ***************************/

const pm_prm_vreg_data_type
pm_pam_pcie_a_ldo1 [] =
{
     /*     mode    voltage(mV)  headroom(mV)   enable*/ 
    { PM_PRM_VREG_MODE_LPM,   0, 0, PM_PRM_DEV_DISABLE },  // PCIE_OFF
    { PM_PRM_VREG_MODE_LPM, 880, 0, PM_PRM_DEV_ENABLE },  // PCIE_SUSPEND
    { PM_PRM_VREG_MODE_NPM, 880, 0, PM_PRM_DEV_ENABLE },  // PCIE_ACTIVE_NOMINAL
    { PM_PRM_VREG_MODE_NPM, 880, 0, PM_PRM_DEV_ENABLE },  // PCIE_ACTIVE_SVS
    { PM_PRM_VREG_MODE_NPM, 880, 0, PM_PRM_DEV_ENABLE }  // PCIE_ACTIVE_TURBO
};

const pm_prm_vreg_data_type
pm_pam_pcie_a_ldo26 [] =
{
     /*     mode    voltage(mV)  headroom(mV)   enable*/ 
    { PM_PRM_VREG_MODE_LPM,        0, 0, PM_PRM_DEV_DISABLE },  // PCIE_OFF
    { PM_PRM_VREG_MODE_LPM,     1200, 0, PM_PRM_DEV_ENABLE },  // PCIE_SUSPEND
    { PM_PRM_VREG_MODE_NPM, 1200, 0, PM_PRM_DEV_ENABLE },  // PCIE_ACTIVE_NOMINAL
    { PM_PRM_VREG_MODE_NPM, 1200, 0, PM_PRM_DEV_ENABLE },  // PCIE_ACTIVE_SVS
    { PM_PRM_VREG_MODE_NPM, 1200, 0, PM_PRM_DEV_ENABLE }  // PCIE_ACTIVE_TURBO
};


const pm_prm_pam_client_cfg_type
pm_pam_pcie_rails_info [] = 
{
    { (void*)pm_pam_pcie_a_ldo1, PM_PRM_RSRC_VRM_VREG },
    { (void*)pm_pam_pcie_a_ldo26, PM_PRM_RSRC_VRM_VREG },
}; 

npa_node_dependency
pm_pam_deps_pcie [] =
{
    PM_PRM_CLIENT_VREG_DEP ( a,  ldo, 1 ),
    PM_PRM_CLIENT_VREG_DEP ( a,  ldo, 26 ),
};

/*************************** USB_SS1 Client ***************************/

const pm_prm_vreg_data_type
pm_pam_usb_ss1_a_ldo1 [] =
{
     /*     mode    voltage(mV)  headroom(mV)   enable*/ 
    { PM_PRM_VREG_MODE_LPM, 0, 0, PM_PRM_DEV_DISABLE },  // USB_OFF
    { PM_PRM_VREG_MODE_LPM, 880, 0, PM_PRM_DEV_DISABLE },  // USB_PERPH_SUSPEND
    { PM_PRM_VREG_MODE_NPM, 880, 0, PM_PRM_DEV_ENABLE },  // USBP_PERPH_ACTIVE
    { PM_PRM_VREG_MODE_LPM, 880, 0, PM_PRM_DEV_DISABLE },  // USB_HOST_SUSPEND
    { PM_PRM_VREG_MODE_NPM, 880, 0, PM_PRM_DEV_ENABLE }  // USB_HOST_ACTIVE
};

const pm_prm_vreg_data_type
pm_pam_usb_ss1_a_ldo12 [] =
{
     /*     mode    voltage(mV)  headroom(mV)   enable*/ 
    { PM_PRM_VREG_MODE_LPM, 0, 0, PM_PRM_DEV_DISABLE },  // USB_OFF
    { PM_PRM_VREG_MODE_LPM, 1800, 0, PM_PRM_DEV_DISABLE },  // USB_PERPH_SUSPEND
    { PM_PRM_VREG_MODE_NPM, 1800, 0, PM_PRM_DEV_ENABLE },  // USBP_PERPH_ACTIVE
    { PM_PRM_VREG_MODE_LPM, 1800, 0, PM_PRM_DEV_DISABLE },  // USB_HOST_SUSPEND
    { PM_PRM_VREG_MODE_NPM, 1800, 0, PM_PRM_DEV_ENABLE }  // USB_HOST_ACTIVE
};

const pm_prm_vreg_data_type
pm_pam_usb_ss1_a_ldo24 [] =
{
     /*     mode    voltage(mV)  headroom(mV)   enable*/ 
    { PM_PRM_VREG_MODE_LPM, 0, 0, PM_PRM_DEV_DISABLE },  // USB_OFF
    { PM_PRM_VREG_MODE_LPM, 3088, 0, PM_PRM_DEV_DISABLE },  // USB_PERPH_SUSPEND
    { PM_PRM_VREG_MODE_NPM, 3088, 0, PM_PRM_DEV_ENABLE },  // USBP_PERPH_ACTIVE
    { PM_PRM_VREG_MODE_LPM, 3088, 0, PM_PRM_DEV_DISABLE },  // USB_HOST_SUSPEND
    { PM_PRM_VREG_MODE_NPM, 3088, 0, PM_PRM_DEV_ENABLE }  // USB_HOST_ACTIVE
};

const pm_prm_vreg_data_type
pm_pam_usb_ss1_a_ldo26 [] =
{
     /*     mode    voltage(mV)  headroom(mV)   enable*/ 
    { PM_PRM_VREG_MODE_LPM, 0, 0, PM_PRM_DEV_DISABLE },  // USB_OFF
    { PM_PRM_VREG_MODE_LPM, 1200, 0, PM_PRM_DEV_DISABLE },  // USB_PERPH_SUSPEND
    { PM_PRM_VREG_MODE_NPM, 1200, 0, PM_PRM_DEV_ENABLE },  // USBP_PERPH_ACTIVE
    { PM_PRM_VREG_MODE_LPM, 1200, 0, PM_PRM_DEV_DISABLE },  // USB_HOST_SUSPEND
    { PM_PRM_VREG_MODE_NPM, 1200, 0, PM_PRM_DEV_ENABLE }  // USB_HOST_ACTIVE
};

const pm_prm_pam_client_cfg_type
pm_pam_usb_ss1_rails_info [] = 
{
    { (void*)pm_pam_usb_ss1_a_ldo1, PM_PRM_RSRC_VRM_VREG },
    { (void*)pm_pam_usb_ss1_a_ldo12, PM_PRM_RSRC_VRM_VREG },
    { (void*)pm_pam_usb_ss1_a_ldo24, PM_PRM_RSRC_VRM_VREG },
    { (void*)pm_pam_usb_ss1_a_ldo26, PM_PRM_RSRC_VRM_VREG },
}; 

npa_node_dependency
pm_pam_deps_usb_ss1 [] =
{
    PM_PRM_CLIENT_VREG_DEP ( a,  ldo, 1 ),
    PM_PRM_CLIENT_VREG_DEP ( a,  ldo, 12 ),
    PM_PRM_CLIENT_VREG_DEP ( a,  ldo, 24 ),
    PM_PRM_CLIENT_VREG_DEP ( a,  ldo, 26 ),
};

/*************************** USB_SS2 Client ***************************/

const pm_prm_vreg_data_type
pm_pam_usb_ss2_a_ldo1 [] =
{
     /*     mode    voltage(mV)  headroom(mV)   enable*/ 
    { PM_PRM_VREG_MODE_LPM, 0, 0, PM_PRM_DEV_DISABLE },  // USB_OFF
    { PM_PRM_VREG_MODE_LPM, 880, 0, PM_PRM_DEV_DISABLE },  // USB_PERPH_SUSPEND
    { PM_PRM_VREG_MODE_NPM, 880, 0, PM_PRM_DEV_ENABLE },  // USBP_PERPH_ACTIVE
    { PM_PRM_VREG_MODE_LPM, 880, 0, PM_PRM_DEV_DISABLE },  // USB_HOST_SUSPEND
    { PM_PRM_VREG_MODE_NPM, 880, 0, PM_PRM_DEV_ENABLE }  // USB_HOST_ACTIVE
};

const pm_prm_vreg_data_type
pm_pam_usb_ss2_a_ldo12 [] =
{
     /*     mode    voltage(mV)  headroom(mV)   enable*/ 
    { PM_PRM_VREG_MODE_LPM, 0, 0, PM_PRM_DEV_DISABLE },  // USB_OFF
    { PM_PRM_VREG_MODE_LPM, 1800, 0, PM_PRM_DEV_DISABLE },  // USB_PERPH_SUSPEND
    { PM_PRM_VREG_MODE_NPM, 1800, 0, PM_PRM_DEV_ENABLE },  // USBP_PERPH_ACTIVE
    { PM_PRM_VREG_MODE_LPM, 1800, 0, PM_PRM_DEV_DISABLE },  // USB_HOST_SUSPEND
    { PM_PRM_VREG_MODE_NPM, 1800, 0, PM_PRM_DEV_ENABLE }  // USB_HOST_ACTIVE
};

const pm_prm_vreg_data_type
pm_pam_usb_ss2_a_ldo24 [] =
{
     /*     mode    voltage(mV)  headroom(mV)   enable*/ 
    { PM_PRM_VREG_MODE_LPM, 0, 0, PM_PRM_DEV_DISABLE },  // USB_OFF
    { PM_PRM_VREG_MODE_LPM, 3088, 0, PM_PRM_DEV_DISABLE },  // USB_PERPH_SUSPEND
    { PM_PRM_VREG_MODE_NPM, 3088, 0, PM_PRM_DEV_ENABLE },  // USBP_PERPH_ACTIVE
    { PM_PRM_VREG_MODE_LPM, 3088, 0, PM_PRM_DEV_DISABLE },  // USB_HOST_SUSPEND
    { PM_PRM_VREG_MODE_NPM, 3088, 0, PM_PRM_DEV_ENABLE }  // USB_HOST_ACTIVE
};

const pm_prm_vreg_data_type
pm_pam_usb_ss2_a_ldo26 [] =
{
     /*     mode    voltage(mV)  headroom(mV)   enable*/ 
    { PM_PRM_VREG_MODE_LPM, 0, 0, PM_PRM_DEV_DISABLE },  // USB_OFF
    { PM_PRM_VREG_MODE_LPM, 1200, 0, PM_PRM_DEV_DISABLE },  // USB_PERPH_SUSPEND
    { PM_PRM_VREG_MODE_NPM, 1200, 0, PM_PRM_DEV_ENABLE },  // USBP_PERPH_ACTIVE
    { PM_PRM_VREG_MODE_LPM, 1200, 0, PM_PRM_DEV_DISABLE },  // USB_HOST_SUSPEND
    { PM_PRM_VREG_MODE_NPM, 1200, 0, PM_PRM_DEV_ENABLE }  // USB_HOST_ACTIVE
};

const pm_prm_pam_client_cfg_type
pm_pam_usb_ss2_rails_info [] = 
{
    { (void*)pm_pam_usb_ss2_a_ldo1, PM_PRM_RSRC_VRM_VREG },
    { (void*)pm_pam_usb_ss2_a_ldo12, PM_PRM_RSRC_VRM_VREG },
    { (void*)pm_pam_usb_ss2_a_ldo24, PM_PRM_RSRC_VRM_VREG },
    { (void*)pm_pam_usb_ss2_a_ldo26, PM_PRM_RSRC_VRM_VREG },
}; 

npa_node_dependency
pm_pam_deps_usb_ss2 [] =
{
    PM_PRM_CLIENT_VREG_DEP ( a,  ldo, 1 ),
    PM_PRM_CLIENT_VREG_DEP ( a,  ldo, 12 ),
    PM_PRM_CLIENT_VREG_DEP ( a,  ldo, 24 ),
    PM_PRM_CLIENT_VREG_DEP ( a,  ldo, 26 ),
};

/*************************** USB_SS3 Client ***************************/

const pm_prm_vreg_data_type
pm_pam_usb_ss3_a_ldo5 [] =
{
     /*     mode    voltage(mV)  headroom(mV)   enable*/ 
    { PM_PRM_VREG_MODE_LPM, 0, 0, PM_PRM_DEV_DISABLE },  // USB_OFF
    { PM_PRM_VREG_MODE_LPM, 880, 0, PM_PRM_DEV_DISABLE },  // USB_PERPH_SUSPEND
    { PM_PRM_VREG_MODE_NPM, 880, 0, PM_PRM_DEV_ENABLE },  // USBP_PERPH_ACTIVE
    { PM_PRM_VREG_MODE_LPM, 880, 0, PM_PRM_DEV_DISABLE },  // USB_HOST_SUSPEND
    { PM_PRM_VREG_MODE_NPM, 880, 0, PM_PRM_DEV_ENABLE }  // USB_HOST_ACTIVE
};

const pm_prm_vreg_data_type
pm_pam_usb_ss3_a_ldo12 [] =
{
     /*     mode    voltage(mV)  headroom(mV)   enable*/ 
    { PM_PRM_VREG_MODE_LPM, 0, 0, PM_PRM_DEV_DISABLE },  // USB_OFF
    { PM_PRM_VREG_MODE_LPM, 1800, 0, PM_PRM_DEV_DISABLE },  // USB_PERPH_SUSPEND
    { PM_PRM_VREG_MODE_NPM, 1800, 0, PM_PRM_DEV_ENABLE },  // USBP_PERPH_ACTIVE
    { PM_PRM_VREG_MODE_LPM, 1800, 0, PM_PRM_DEV_DISABLE },  // USB_HOST_SUSPEND
    { PM_PRM_VREG_MODE_NPM, 1800, 0, PM_PRM_DEV_ENABLE }  // USB_HOST_ACTIVE
};

const pm_prm_vreg_data_type
pm_pam_usb_ss3_a_ldo24 [] =
{
     /*     mode    voltage(mV)  headroom(mV)   enable*/ 
    { PM_PRM_VREG_MODE_LPM, 0, 0, PM_PRM_DEV_DISABLE },  // USB_OFF
    { PM_PRM_VREG_MODE_LPM, 3088, 0, PM_PRM_DEV_DISABLE },  // USB_PERPH_SUSPEND
    { PM_PRM_VREG_MODE_NPM, 3088, 0, PM_PRM_DEV_ENABLE },  // USBP_PERPH_ACTIVE
    { PM_PRM_VREG_MODE_LPM, 3088, 0, PM_PRM_DEV_DISABLE },  // USB_HOST_SUSPEND
    { PM_PRM_VREG_MODE_NPM, 3088, 0, PM_PRM_DEV_ENABLE }  // USB_HOST_ACTIVE
};

const pm_prm_vreg_data_type
pm_pam_usb_ss3_a_ldo26 [] =
{
     /*     mode    voltage(mV)  headroom(mV)   enable*/ 
    { PM_PRM_VREG_MODE_LPM, 0, 0, PM_PRM_DEV_DISABLE },  // USB_OFF
    { PM_PRM_VREG_MODE_LPM, 1200, 0, PM_PRM_DEV_DISABLE },  // USB_PERPH_SUSPEND
    { PM_PRM_VREG_MODE_NPM, 1200, 0, PM_PRM_DEV_ENABLE },  // USBP_PERPH_ACTIVE
    { PM_PRM_VREG_MODE_LPM, 1200, 0, PM_PRM_DEV_DISABLE },  // USB_HOST_SUSPEND
    { PM_PRM_VREG_MODE_NPM, 1200, 0, PM_PRM_DEV_ENABLE }  // USB_HOST_ACTIVE
};

const pm_prm_pam_client_cfg_type
pm_pam_usb_ss3_rails_info [] = 
{
    { (void*)pm_pam_usb_ss3_a_ldo5, PM_PRM_RSRC_VRM_VREG },
    { (void*)pm_pam_usb_ss3_a_ldo12, PM_PRM_RSRC_VRM_VREG },
    { (void*)pm_pam_usb_ss3_a_ldo24, PM_PRM_RSRC_VRM_VREG },
    { (void*)pm_pam_usb_ss3_a_ldo26, PM_PRM_RSRC_VRM_VREG },
}; 

npa_node_dependency
pm_pam_deps_usb_ss3 [] =
{
    PM_PRM_CLIENT_VREG_DEP ( a,  ldo, 5 ),
    PM_PRM_CLIENT_VREG_DEP ( a,  ldo, 12 ),
    PM_PRM_CLIENT_VREG_DEP ( a,  ldo, 24 ),
    PM_PRM_CLIENT_VREG_DEP ( a,  ldo, 26 ),
};
/*************************** NPA NODE RESOURCE ***************************/

const pm_prm_node_resource_info
pm_prm_pam_rsrcs [] =
{
   {
     PMIC_NPA_GROUP_ID_CAMERA_BACK,  // Resource Name
    PMIC_NPA_MODE_ID_GENERIC_ACTIVE,   // Maximum Value
    NPA_RESOURCE_SINGLE_CLIENT | NPA_RESOURCE_DRIVER_UNCONDITIONAL, // Resource Attribute
    (void*) pm_pam_camera_back_rails_info, // Resource User Data
    PMIC_NPA_CLIENT_NODE_CAMERA_BACK, // Node Name
    NPA_NODE_DEFAULT,  // Node Attributes
    NULL,
    pm_pam_deps_camera_back,  // Node Dependency
    NPA_ARRAY_SIZE(pm_pam_deps_camera_back),  // Deps Count 
   },
   {
     PMIC_NPA_GROUP_ID_CAMERA_FRONT,  // Resource Name
    PMIC_NPA_MODE_ID_GENERIC_ACTIVE,   // Maximum Value
    NPA_RESOURCE_SINGLE_CLIENT | NPA_RESOURCE_DRIVER_UNCONDITIONAL, // Resource Attribute
    (void*) pm_pam_camera_front_rails_info, // Resource User Data
    PMIC_NPA_CLIENT_NODE_CAMERA_FRONT, // Node Name
    NPA_NODE_DEFAULT,  // Node Attributes
    NULL,
    pm_pam_deps_camera_front,  // Node Dependency
    NPA_ARRAY_SIZE(pm_pam_deps_camera_front),  // Deps Count 
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
     PMIC_NPA_GROUP_ID_DISP_PRIM_AMOLED,  // Resource Name
    PMIC_NPA_MODE_ID_GENERIC_ACTIVE,   // Maximum Value
    NPA_RESOURCE_SINGLE_CLIENT | NPA_RESOURCE_DRIVER_UNCONDITIONAL, // Resource Attribute
    (void*) pm_pam_disp_prim_amoled_rails_info, // Resource User Data
    PMIC_NPA_CLIENT_NODE_DISP_PRIM_AMOLED, // Node Name
    NPA_NODE_DEFAULT,  // Node Attributes
    NULL,
    pm_pam_deps_disp_prim_amoled,  // Node Dependency
    NPA_ARRAY_SIZE(pm_pam_deps_disp_prim_amoled),  // Deps Count 
   },
   {
     PMIC_NPA_GROUP_ID_PCIE,  // Resource Name
    PMIC_NPA_MODE_ID_PCIE_ACTIVE_TURBO,   // Maximum Value
    NPA_RESOURCE_SINGLE_CLIENT | NPA_RESOURCE_DRIVER_UNCONDITIONAL, // Resource Attribute
    (void*) pm_pam_pcie_rails_info, // Resource User Data
    PMIC_NPA_CLIENT_NODE_PCIE, // Node Name
    NPA_NODE_DEFAULT,  // Node Attributes
    NULL,
    pm_pam_deps_pcie,  // Node Dependency
    NPA_ARRAY_SIZE(pm_pam_deps_pcie),  // Deps Count 
   },
   {
    PMIC_NPA_GROUP_ID_USB_SS1,  // Resource Name
    PMIC_NPA_MODE_ID_USB_HOST_ACTIVE,   // Maximum Value
    NPA_RESOURCE_SINGLE_CLIENT | NPA_RESOURCE_DRIVER_UNCONDITIONAL, // Resource Attribute
    (void*) pm_pam_usb_ss1_rails_info, // Resource User Data
    PMIC_NPA_CLIENT_NODE_USB_SS1, // Node Name
    NPA_NODE_DEFAULT,  // Node Attributes
    NULL,
    pm_pam_deps_usb_ss1,  // Node Dependency
    NPA_ARRAY_SIZE(pm_pam_deps_usb_ss1),  // Deps Count 
   },
   {
    PMIC_NPA_GROUP_ID_USB_SS2,  // Resource Name
    PMIC_NPA_MODE_ID_USB_HOST_ACTIVE,   // Maximum Value
    NPA_RESOURCE_SINGLE_CLIENT | NPA_RESOURCE_DRIVER_UNCONDITIONAL, // Resource Attribute
    (void*) pm_pam_usb_ss2_rails_info, // Resource User Data
    PMIC_NPA_CLIENT_NODE_USB_SS2, // Node Name
    NPA_NODE_DEFAULT,  // Node Attributes
    NULL,
    pm_pam_deps_usb_ss2,  // Node Dependency
    NPA_ARRAY_SIZE(pm_pam_deps_usb_ss2),  // Deps Count 
   },
   {
    PMIC_NPA_GROUP_ID_USB_SS3,  // Resource Name
    PMIC_NPA_MODE_ID_USB_HOST_ACTIVE,   // Maximum Value
    NPA_RESOURCE_SINGLE_CLIENT | NPA_RESOURCE_DRIVER_UNCONDITIONAL, // Resource Attribute
    (void*) pm_pam_usb_ss3_rails_info, // Resource User Data
    PMIC_NPA_CLIENT_NODE_USB_SS3, // Node Name
    NPA_NODE_DEFAULT,  // Node Attributes
    NULL,
    pm_pam_deps_usb_ss3,  // Node Dependency
    NPA_ARRAY_SIZE(pm_pam_deps_usb_ss3),  // Deps Count 
   },
};

uint32 num_of_pam_rsrcs [] = { sizeof(pm_prm_pam_rsrcs) / sizeof(pm_prm_node_resource_info) };


/*************************** Device Layer Resources ***************************/

pm_prm_rsrc_data_type
pm_prm_device_rsrc_data [] =
{
    PM_PRM_DEV_RSRC_VREG ( a, ldo, 1, PM_PRM_RSRC_LDO,  PM_PRM_RPMH, PM_LDO_1 ), 
    PM_PRM_DEV_RSRC_VREG ( a, ldo, 5, PM_PRM_RSRC_LDO,  PM_PRM_RPMH, PM_LDO_5 ), 
    PM_PRM_DEV_RSRC_VREG ( a, ldo, 12, PM_PRM_RSRC_LDO,  PM_PRM_RPMH, PM_LDO_12 ), 
    PM_PRM_DEV_RSRC_VREG ( a, ldo, 14, PM_PRM_RSRC_LDO,  PM_PRM_RPMH, PM_LDO_14 ),
    PM_PRM_DEV_RSRC_VREG ( a, ldo, 19, PM_PRM_RSRC_LDO,  PM_PRM_RPMH, PM_LDO_19 ),  
    PM_PRM_DEV_RSRC_VREG ( a, ldo, 22, PM_PRM_RSRC_LDO,  PM_PRM_RPMH, PM_LDO_22 ), 
    PM_PRM_DEV_RSRC_VREG ( a, ldo, 24, PM_PRM_RSRC_LDO,  PM_PRM_RPMH, PM_LDO_24 ), 
    PM_PRM_DEV_RSRC_VREG ( a, ldo, 26, PM_PRM_RSRC_LDO,  PM_PRM_RPMH, PM_LDO_26 ), 
    PM_PRM_DEV_RSRC_VREG ( a, vs, 1, PM_PRM_RSRC_VS, PM_PRM_RPMH, PM_VS_LVS_1)
};

pm_prm_resource_type
pm_prm_device_rsrcs [ ] =
{
    {
        pm_prm_device_rsrc_data, 
        sizeof(pm_prm_device_rsrc_data)/sizeof(pm_prm_rsrc_data_type), //Number of Resources 
        9
    }
};
