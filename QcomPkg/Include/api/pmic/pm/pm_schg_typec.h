#ifndef __PM_SCHG_TYPEC_H__
#define __PM_SCHG_TYPEC_H__

/*! \file
*  \n
*  \brief  pm_schg_typec.h PMIC-SMBC MODULE RELATED DECLARATION 
*  \details  This header file contains functions and variable declarations 
*  to support Qualcomm PMIC SCHG (Switch-Mode Battery Charger) module. The 
*  Switched-Mode Battery Charger (SCHG) module includes a buck regulated 
*  battery charger with integrated switches. The SCHG module, along with the 
*  Over Voltage Proection (OVP) module will majorly be used by charger 
*  appliation for charging Li-Ion batteries with high current (up to 2A).
*  \n &copy; Copyright 2017-2018 QUALCOMM Technologies Inc, All Rights Reserved
*/

/* =======================================================================
                                Edit History
This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.


when       who    what, where, why
--------   ---    ---------------------------------------------------------- 
05/14/18   ra         Support for Debug Accessory Mode
02/20/18   richaa     Remove obsolete code.
10/19/17   richaa     Initial version. 
========================================================================== */
#include "com_dtypes.h"
#include "pm_err_flags.h"
#include "pm_resources_and_types.h"

/*===========================================================================

                        TYPE DEFINITIONS 

===========================================================================*/

/*
 * For detecting the current limit source can provide.
 */
typedef enum
{
  PM_SCHG_USB_TYPEC_CURR_ADV_RESERVED,
  PM_SCHG_USB_TYPEC_CURR_ADV_3A,
  /*Current advertisement 3.0A*/
  PM_SCHG_USB_TYPEC_CURR_ADV_1P5A,
  /*Current advertisement 1.5A*/
  PM_SCHG_USB_TYPEC_CURR_ADV_STD,
  /*Current advertisement STD TypeC USB Power*/
  PM_SCHG_USB_TYPEC_CURR_ADV_INVALID,
}pm_schg_usb_typec_curr_adv_type;

typedef enum
{
    PM_TYPEC_PLUG_RESERVED,
    PM_TYPEC_PLUG_UNFLIP,      // CC1 is connected
    PM_TYPEC_PLUG_FLIP,        // CC2 is connected
    PM_TYPEC_PLUG_OPEN,
    PM_TYPEC_PLUG_INVALID
} pm_schg_usb_typec_plug_orientation;

/* For the types that different source (UFP) attached (we are in DFP).
 */
typedef enum
{
    PM_SCHG_TYPEC_UFP_RESERVED,
    PM_SCHG_TYPEC_UFP_UNATTACHED,                  // OPEN OPEN
    PM_SCHG_TYPEC_UFP_ATTACHED,                    // RD OPEN
    PM_SCHG_TYPEC_UFP_NOT_ATTACHED_POWER_CABLE,    // RA OPEN
    PM_SCHG_TYPEC_UFP_ATTACHED_POWER_CABLE,        // RD RA
    PM_SCHG_TYPEC_UFP_AUDIO_ADAPTER,               // RA RA
    PM_SCHG_TYPEC_UFP_DEBUG_ACCESSORY,             // RD RD
    PM_SCHG_TYPEC_UFP_INVALID
} pm_schg_usb_typec_ufp_type;

/**  TYPEC Port status on attach of USB cable **/
typedef struct
{
    pm_schg_usb_typec_plug_orientation     cc_out_sts;
    pm_schg_usb_typec_curr_adv_type        dfp_curr_adv;
    pm_schg_usb_typec_ufp_type             ufp_conn_type;
    boolean                                vbus_sts;
    boolean                                vbus_err_sts;
    boolean                                debounce_done_sts;
    boolean                                vconn_oc_sts;
    boolean                                ccout_out_hiz_sts;
} pm_schg_usb_typec_status_type;

typedef enum
{
    PM_SCHG_USB_TYPEC_ROLE_NONE,
    PM_SCHG_USB_TYPEC_ROLE_DFP,
    PM_SCHG_USB_TYPEC_ROLE_UFP,
    PM_SCHG_USB_TYPEC_ROLE_INVALID
} pm_schg_usb_typec_port_role_type;

typedef enum
{
	PM_SCHG_USB_TYPEC_VCONN_CONTROL_BY_HW,
	PM_SCHG_USB_TYPEC_VCONN_DISABLE_BY_SW,
    PM_SCHG_USB_TYPEC_VCONN_ENABLE_BY_SW = 0x03,
    PM_SCHG_USB_TYPEC_VCONN_INVALID
} pm_schg_usb_typec_vconn_enable_type;

/**  TYPEC VConn Current Limit **/
typedef enum
{
    PM_TYPEC_VCONN_ILIMIT_300MA,
    PM_TYPEC_VCONN_ILIMIT_500MA,
    PM_TYPEC_VCONN_ILIMIT_INVALID
} pm_schg_usb_typec_vconn_ilimit;

/**  TYPEC VConn Soft Start Pull Down Current mA - SS_PD_IMA **/
typedef enum
{
    PM_TYPEC_VCONN_SS_PD_35MA,
    PM_TYPEC_VCONN_SS_PD_88MA,
    PM_TYPEC_VCONN_SS_PD_160MA,
    PM_TYPEC_VCONN_SS_PD_248MA,
    PM_TYPEC_VCONN_SS_PD_INVALID
} pm_schg_usb_typec_vconn_ss_pd_ima;

typedef enum
{
	PM_TYPEC_DAM_ICL_3A,
	PM_TYPEC_DAM_ICL_FMB
}pm_schg_usb_typec_dam_icl_sel;

/*===========================================================================

                 SCHG DRIVER FUNCTION PROTOTYPES

===========================================================================*/



/**
* @brief This function returns the USB type-C status
* 
* @param[in]   device_index. Primary: 0. Secondary: 1
* @param[out]  typec_status:  USB type c status type. Refer #pm_schg_usb_typec_status_type for more info
*
* @return  pm_err_flag_type 
*          PM_ERR_FLAG__FEATURE_NOT_SUPPORTED = Feature not available on this
*          version of the PMIC.
*          PM_ERR_FLAG__SUCCESS               = SUCCESS.
*
*/
pm_err_flag_type pm_schg_usb_get_typec_status(uint32 pmic_device, pm_schg_usb_typec_status_type *typec_status);



/**
* @brief This function enables vconn based on the input vconn source type
* 
* @param[in]  device_index. Primary: 0. Secondary: 1
* @param[in]  status_type:  VCONN source type. Refer #pm_schg_usb_typec_vconn_enable_type for more info
*
* @return  pm_err_flag_type 
*          PM_ERR_FLAG__FEATURE_NOT_SUPPORTED = Feature not available on this
*          version of the PMIC.
*          PM_ERR_FLAG__SUCCESS               = SUCCESS.
*
*/
pm_err_flag_type pm_schg_usb_typec_vconn_enable(uint32 pmic_device, pm_schg_usb_typec_vconn_enable_type enable_type);

/**
* @brief This function gets the TypeC port role as DRP/DFP/UFP
* 
* @param[in]   device_index. Primary: 0. Secondary: 1
* @param[out]  port_role:  USB type c port role type. Refer #pm_schg_usb_typec_port_role_type for more info
*
* @return  pm_err_flag_type 
*          PM_ERR_FLAG__FEATURE_NOT_SUPPORTED = Feature not available on this
*          version of the PMIC.
*          PM_ERR_FLAG__SUCCESS               = SUCCESS.
*
*/
pm_err_flag_type pm_schg_usb_typec_get_port_role(uint32 pmic_device, pm_schg_usb_typec_port_role_type *port_role);

/**
* @brief This function sets the TypeC port role as DRP/DFP/UFP
* 
* @param[in]  device_index. Primary: 0. Secondary: 1
* @param[in]  port_role:  USB type c port role type. Refer #pm_schg_usb_typec_port_role_type for more info
*
* @return  pm_err_flag_type 
*          PM_ERR_FLAG__FEATURE_NOT_SUPPORTED = Feature not available on this
*          version of the PMIC.
*          PM_ERR_FLAG__SUCCESS               = SUCCESS.
*
*/
pm_err_flag_type pm_schg_usb_typec_set_port_role(uint32 pmic_device, pm_schg_usb_typec_port_role_type port_role);

/**
* @brief This function disables the TYPE-C block
* 
* @param[in]  device_index. Primary: 0. Secondary: 1
* @param[in]  disable: FALSE = Do not disable TYPEC block,
*                      TRUE = Put TYPEC block in disable state 
*
* @return  pm_err_flag_type 
*          PM_ERR_FLAG__FEATURE_NOT_SUPPORTED = Feature not available on this
*          version of the PMIC.
*          PM_ERR_FLAG__SUCCESS               = SUCCESS.
*
*/
pm_err_flag_type pm_schg_usb_typec_disable(uint32 pmic_device, boolean disable);

/**
* @brief This function configures try sink mode
* 
* @param[in]   device_index. Primary: 0. Secondary: 1
* @param[in]  enable: enable/disable try sink mode
*
* @return  pm_err_flag_type 
*          PM_ERR_FLAG__FEATURE_NOT_SUPPORTED = Feature not available on this
*          version of the PMIC.
*          PM_ERR_FLAG__SUCCESS               = SUCCESS.
*
*/
pm_err_flag_type pm_schg_usb_typec_enable_try_sink_mode(uint32 pmic_device, boolean enable);
pm_err_flag_type pm_schg_typec_en_dbg_access_snk(uint32 pmic_device, boolean EnDbgAccessSnk);
pm_err_flag_type pm_schg_typec_en_fmb(uint32 pmic_device, boolean EnFmb);
pm_err_flag_type pm_schg_typec_get_dam_connect_sts(uint32 pmic_device, boolean *ConnectSts);
pm_err_flag_type pm_schg_typec_set_dam_icl(uint32 pmic_device, pm_schg_usb_typec_dam_icl_sel icl_sel);

#endif /* __PM_SCHG_USB_H__ */

