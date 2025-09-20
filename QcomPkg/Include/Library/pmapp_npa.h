#ifndef __PMAPP_NPA_H__
#define __PMAPP_NPA_H__

/*! \file  pmapp_npa.h
 *  
 *  \brief  File Contains the PMIC NPA CMI Code
 *  \details  This file contains the needed definition and enum for PMIC NPA layer.
 *  
 *    PMIC code generation Version: 1.0.0.0
 *    PMIC code generation Locked Version: APPSBL_MSM8998_PMI8998_0p6 - LOCKED

 *    This file contains code for Target specific settings and modes.
 *  
 *  &copy; Copyright 2016-2019 Qualcomm Technologies, Inc. All Rights Reserved
 */

/*===========================================================================

                EDIT HISTORY FOR MODULE

  This document is created by a code generator, therefore this section will
  not contain comments describing changes made to the module over time.


when       who     what, where, why
--------   ---     ---------------------------------------------------------- 

===========================================================================*/

/*===========================================================================

                        DEFINITION

===========================================================================*/

#define PMIC_NPA_GROUP_ID_ANT1 "/pmic/client/ant1"
#define PMIC_NPA_GROUP_ID_BOOST "/pmic/client/boost"
#define PMIC_NPA_GROUP_ID_CAMERA_BACK  "/pmic/client/camera_back"
#define PMIC_NPA_GROUP_ID_CAMERA_FRONT "/pmic/client/camera_front"
#define PMIC_NPA_GROUP_ID_CAMERA_HELLO "/pmic/client/camera_hello"
#define PMIC_NPA_GROUP_ID_DISP_EXT_DP "/pmic/client/disp_ext_dp"
#define PMIC_NPA_GROUP_ID_DISP_DP_ALT "/pmic/client/disp_dp_alt"
#define PMIC_NPA_GROUP_ID_DISP_EXT_HDMI "/pmic/client/disp_ext_hdmi"
#define PMIC_NPA_GROUP_ID_DISP_PRIM "/pmic/client/disp_prim"
#define PMIC_NPA_GROUP_ID_DISP_PRIM_AMOLED "/pmic/client/disp_prim_amoled"
#define PMIC_NPA_GROUP_ID_DISP_EDP "/pmic/client/disp_edp"
#define PMIC_NPA_GROUP_ID_DISP_SEC "/pmic/client/disp_sec"
#define PMIC_NPA_GROUP_ID_EMMC "/pmic/client/emmc"
#define PMIC_NPA_GROUP_ID_PCIE "/pmic/client/pcie"
#define PMIC_NPA_GROUP_ID_NVME "/pmic/client/nvme"
#define PMIC_NPA_GROUP_ID_UFS "/pmic/client/ufs"
#define PMIC_NPA_GROUP_ID_WLAN "/pmic/client/wlan"
#define PMIC_NPA_GROUP_ID_HST_XO_CLS "/pmic/client/hst_xo_cls"

/**
 * PMIC_NPA_GROUP_ID -- PMIC_NPA_GROUP_ID_MEM_UVOL
 */
#define PMIC_NPA_GROUP_ID_MEM_UVOL "/pmic/client/vdd_mem_uvol"
#define PMIC_NPA_GROUP_ID_NFC "/pmic/client/nfc"
#define PMIC_NPA_GROUP_ID_RAIL_CX "/pmic/client/rail_cx"
#define PMIC_NPA_GROUP_ID_RAIL_MX "/pmic/client/rail_mx"
#define PMIC_NPA_GROUP_ID_SD "/pmic/client/sd"
#define PMIC_NPA_GROUP_ID_UFS0 "/pmic/client/ufs0"
#define PMIC_NPA_GROUP_ID_UFS1 "/pmic/client/ufs1"
#define PMIC_NPA_GROUP_ID_TSCREEN "/pmic/client/touch_screen"
#define PMIC_NPA_GROUP_ID_USB_HS1 "/pmic/client/usb_hs1"
#define PMIC_NPA_GROUP_ID_USB_HS2 "/pmic/client/usb_hs2"
#define PMIC_NPA_GROUP_ID_USB_HSIC "/pmic/client/usb_hsic"
#define PMIC_NPA_GROUP_ID_USB_SS0 "/pmic/client/usb_ss0"
#define PMIC_NPA_GROUP_ID_USB_SS1 "/pmic/client/usb_ss1"
#define PMIC_NPA_GROUP_ID_USB_SS2 "/pmic/client/usb_ss2"
#define PMIC_NPA_GROUP_ID_USB_SS3 "/pmic/client/usb_ss3"
#define PMIC_NPA_GROUP_ID_USB_SSMUX1 "/pmic/client/usb_ssmux1"
#define PMIC_NPA_GROUP_ID_USB_SSMUX2 "/pmic/client/usb_ssmux2"
#define PMIC_NPA_GROUP_ID_USB_SS1_GPIO_GND "/pmic/client/usb_ss1_gpio_gnd"
#define PMIC_NPA_GROUP_ID_USB_SS1_GPIO_HIGH "/pmic/client/usb_ss1_gpio_high"
#define PMIC_NPA_GROUP_ID_USB_SS1_A "/pmic/client/usb_ss1_a"
#define PMIC_NPA_GROUP_ID_USB_SS1_B "/pmic/client/usb_ss1_b"


//Query id to query PAM client resources
//usage: query_status = npa_query_by_name(PMIC_NPA_GROUP_ID_QFPROM, PM_PRM_QUERY_PAM_CLIENT, &query_result ); 
//return: query_result.data.value, TRUE : all resources set correctly
#define PM_PRM_QUERY_PAM_CLIENT  0x5000

/*===========================================================================

                        ENUMERATION

===========================================================================*/

enum
{
    PMIC_NPA_MODE_ID_GENERIC_OFF = 0,
    PMIC_NPA_MODE_ID_GENERIC_STANDBY = 1,
    PMIC_NPA_MODE_ID_GENERIC_ACTIVE = 2,
    PMIC_NPA_MODE_ID_GENERIC_LV = PMIC_NPA_MODE_ID_GENERIC_ACTIVE,
    PMIC_NPA_MODE_ID_GENERIC_MV = 3,
    PMIC_NPA_MODE_ID_GENERIC_HV = 4,
    PMIC_NPA_MODE_ID_GENERIC_MAX,
};

#endif // __PM_CONFIG_APPSBL_NPA_PAM_C__
