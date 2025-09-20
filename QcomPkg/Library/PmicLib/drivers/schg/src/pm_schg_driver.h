#ifndef __PM_SCHG_DRIVER_H__
#define __PM_SCHG_DRIVER_H__

/*! \file pm_schg_driver.h
 *  \n
 *  \brief This file contains functions prototypes and variable/type/constant
*          declarations for supporting SMBB peripheral  
 *  \n  
 *  \n &copy;
 *  Copyright (c) 2017 - 2018 Qualcomm Technologies, Inc.  All Rights Reserved. 
 *  Qualcomm Technologies Proprietary and Confidential.
 */

/*===========================================================================

                        EDIT HISTORY

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.


when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
08/01/18   ivy     Added support to set batt therm pull up to register
06/29/18   dc	   Expand ICL Status API for Max ICL
06/21/18   cs	   Added support for multi-port.
05/17/18   ivy	   Added support for Qi charging ICL register
05/14/18   ra      Support for Debug Accessory Mode
03/01/18   ra      Adding skin and die temp registers
02/28/18   pxm     Remove unused APIs and registers.
01/15/18   ra      Adding JEITA registers
10/24/17   ra      update register map for PM855B
09/19/17   cs      updated register map
02/17/17   pxm     Added sysok_reason_status in schg_misc_register_ds struct
02/08/17   ll      Added otg_eng_otg_cfg in schg_otg_register_ds and  
                    eng_ssupply_cfg2 in schg_dc_register_ds
02/03/17   sm      Added type_c_cfg_3 in schg_usb_register_ds struct
12/06/16   sm      Added eng_ssupply_cfg3 in schg_dc_register_ds struct
12/19/16   mr     Added API to get SCHG Charger PmicIndex
11/14/16   sm      Added eng_buckboost_cfg1 in schg_misc_register_ds struct
09/07/16   sm      Added pm_schg_get_dig_major API to return digital major 
01/13/16   al      Created
========================================================================== */
/*===========================================================================

                     INCLUDE FILES 

===========================================================================*/
#include "pm_target_information.h"

/*===========================================================================

                     STRUCTURE TYPE AND ENUM

===========================================================================*/


/************************************************************************/
/* register definitions                                                                     */
/************************************************************************/
typedef struct
{
  uint32                   base_address;                  //0x1200
  pm_register_address_type batt_miss_sts                ; //0x006
  pm_register_address_type int_rt_sts                   ; //0x010
  pm_register_address_type int_set_type                 ; //0x011
  pm_register_address_type int_polarity_high            ; //0x012
  pm_register_address_type int_polarity_low             ; //0x013
  pm_register_address_type int_latched_clr              ; //0x014
  pm_register_address_type int_en_set                   ; //0x015
  pm_register_address_type int_en_clr                   ; //0x016
  pm_register_address_type int_latched_sts              ; //0x018
  pm_register_address_type int_pending_sts              ; //0x019
  pm_register_address_type ship_mode                    ; //0x040
  pm_register_address_type low_batt_threshold_cfg       ; //0x061
  pm_register_address_type bat_miss_src_cfg             ; //0x070
  pm_register_address_type adc_internal_pull_up         ; //0x086
  pm_register_address_type fake_batid_ch_en             ; //0x091
}schg_batif_register_ds;

typedef struct schg_dcdc_register_ds
{
  pm_register_address_type base_address               ; //0x1100
  pm_register_address_type perph_type                 ; //0x004
  pm_register_address_type perph_subtype              ; //0x005
  pm_register_address_type icl_max_status             ; //0x006
  pm_register_address_type lockbit_d1                 ; //0x0D1
  pm_register_address_type lockbit_d2                 ; //0x0D2
  pm_register_address_type spare                      ; //0x0DF
  pm_register_address_type otg_status                 ; //0x00D
  pm_register_address_type int_rt_sts                 ; //0x010
  pm_register_address_type int_set_type               ; //0x011
  pm_register_address_type int_polarity_high          ; //0x012
  pm_register_address_type int_polarity_low           ; //0x013
  pm_register_address_type int_latched_clr            ; //0x014
  pm_register_address_type int_en_set                 ; //0x015
  pm_register_address_type int_en_clr                 ; //0x016
  pm_register_address_type int_latched_sts            ; //0x018
  pm_register_address_type int_pending_sts            ; //0x019
  pm_register_address_type int_mid_sel                ; //0x01A
  pm_register_address_type int_priority               ; //0x01B
  pm_register_address_type cmd_otg                    ; //0x040
  pm_register_address_type bat_uvlo_threshold_cfg     ; //0x051
  pm_register_address_type otg_current_limit_cfg      ; //0x052
  pm_register_address_type otg_cfg                    ; //0x053
  pm_register_address_type vsysmin_cfg                ; //0x083
  pm_register_address_type cfg_ref_max_psns           ; //0x08C
}schg_dcdc_register_ds;

typedef struct
{
  uint32                   base_address;                                         //0x1000
  pm_register_address_type battery_charger_status_1                            ; //0x006
  pm_register_address_type battery_charger_status_3                            ; //0x009
  pm_register_address_type charging_enable_cmd                                 ; //0x042
  pm_register_address_type charger_cfg2                                        ; //0x051
  pm_register_address_type pre_charge_current_cfg                              ; //0x060
  pm_register_address_type fast_charge_current_cfg                             ; //0x061
  pm_register_address_type termination_charge_current_cfg                      ; //0x062
  pm_register_address_type tccc_charge_current_termination_cfg                 ; //0x063
  pm_register_address_type float_voltage_cfg                                   ; //0x070
  pm_register_address_type jeita_fvcomp_cold_cfg                               ; //0x086
  pm_register_address_type jeita_en_cfg                                        ; //0x090
  pm_register_address_type jeita_fvcomp_hot_cfg                                ; //0x091
  pm_register_address_type jeita_cccomp_hot_cfg                                ; //0x092
  pm_register_address_type jeita_cccomp_cold_cfg                               ; //0x093
  pm_register_address_type jeita_hot_threshold_msb                             ; //0x094
  pm_register_address_type jeita_hot_threshold_lsb                             ; //0x095
  pm_register_address_type jeita_cold_threshold_msb                            ; //0x096
  pm_register_address_type jeita_cold_threshold_lsb                            ; //0x097
  pm_register_address_type jeita_thot_threshold_msb                            ; //0x098
  pm_register_address_type jeita_thot_threshold_lsb                            ; //0x099
  pm_register_address_type jeita_tcold_threshold_msb                           ; //0x09A
  pm_register_address_type jeita_tcold_threshold_lsb                           ; //0x09B
  pm_register_address_type jeita_thot_afp_threshold_msb                        ; //0x09C
  pm_register_address_type jeita_thot_afp_threshold_lsb                        ; //0x09D
  pm_register_address_type jeita_tcold_afp_threshold_msb                       ; //0x09E
  pm_register_address_type jeita_tcold_afp_threshold_lsb                       ; //0x09F
}schg_chgr_register_ds;

typedef struct
{
  uint32                   base_address;                     //0x1400
  pm_register_address_type int_rt_sts                      ; //0x010
  pm_register_address_type int_latched_sts                 ; //0x018
  pm_register_address_type int_pending_sts                 ; //0x019
  pm_register_address_type cmd_il                          ; //0x040
}schg_dc_register_ds;


typedef struct
{
  uint32                   base_address;                      // 0x1600
  pm_register_address_type wdog_status                      ; //0x00C
  pm_register_address_type sysok_reason_status              ; //0x00D
  pm_register_address_type int_rt_sts                       ; //0x010
  pm_register_address_type int_set_type                     ; //0x011
  pm_register_address_type int_polarity_high                ; //0x012
  pm_register_address_type int_polarity_low                 ; //0x013
  pm_register_address_type int_latched_clr                  ; //0x014
  pm_register_address_type int_en_set                       ; //0x015
  pm_register_address_type int_en_clr                       ; //0x016
  pm_register_address_type int_latched_sts                  ; //0x018
  pm_register_address_type int_pending_sts                  ; //0x019
  pm_register_address_type afp_mode                         ; //0x041
  pm_register_address_type bark_bite_wdog_pet               ; //0x043
  pm_register_address_type wd_cfg                           ; //0x051
  pm_register_address_type snarl_bark_bite_wd_cfg           ; //0x053
  pm_register_address_type aicl_rerun_time_cfg              ; //0x061
  pm_register_address_type thermreg_src_cfg                 ; //0x070
  pm_register_address_type die_temp_h_msb                   ; //0x0A0
  pm_register_address_type die_temp_h_lsb                   ; //0x0A1
  pm_register_address_type die_temp_l_msb                   ; //0x0A2
  pm_register_address_type die_temp_l_lsb                   ; //0x0A3
}schg_misc_register_ds;


typedef struct
{
  uint32                   base_address;                         //0x1300
  pm_register_address_type apsd_status                         ; //0x007
  pm_register_address_type apsd_result_status                  ; //0x008
  pm_register_address_type int_rt_sts                          ; //0x010
  pm_register_address_type int_set_type                        ; //0x011
  pm_register_address_type int_polarity_high                   ; //0x012
  pm_register_address_type int_polarity_low                    ; //0x013
  pm_register_address_type int_latched_clr                     ; //0x014
  pm_register_address_type int_en_set                          ; //0x015
  pm_register_address_type int_en_clr                          ; //0x016
  pm_register_address_type int_latched_sts                     ; //0x018
  pm_register_address_type int_pending_sts                     ; //0x019
  pm_register_address_type cmd_il                              ; //0x040
  pm_register_address_type cmd_apsd                            ; //0x041
  pm_register_address_type usbin_adapter_allow_cfg             ; //0x060
  pm_register_address_type usbin_options_1_cfg                 ; //0x062
  pm_register_address_type usbin_load_cfg                      ; //0x065
  pm_register_address_type usbin_icl_options                   ; //0x066
  pm_register_address_type usbin_current_limit_cfg             ; //0x070
  pm_register_address_type usbin_aicl_options_cfg              ; //0x080
}schg_usb_register_ds;

typedef struct
{
    uint32                   base_address;           //0x1b00
    pm_register_address_type bsi_sts;               //0x1b08
    pm_register_address_type bsi_en;                //0x1b46
    pm_register_address_type mipi_bif_err_clear;    //0x1b4f
    pm_register_address_type mipi_bif_fsm_reset;    //0x1b50
    pm_register_address_type mipi_bif_force_bcl_low; //0x1b51
    pm_register_address_type mipi_bif_tau_cfg;      //0x1b52
    pm_register_address_type mipi_bif_mode;         //0x1b53
    pm_register_address_type mipi_bif_en;           //0x1b54
    pm_register_address_type mipi_bif_cfg;          //0x1b55
    pm_register_address_type mipi_bif_rx_cfg;       //0x1b56
    pm_register_address_type mipi_bif_tx_dly;       //0x1b59
    pm_register_address_type mipi_bif_data_tx_0;    //0x1b5a
    pm_register_address_type mipi_bif_data_tx_1;    //0x1b5b
    pm_register_address_type mipi_bif_data_tx_2;    //0x1b5c
    pm_register_address_type mipi_bif_tx_ctl;       //0x1b5d
    pm_register_address_type mipi_bif_data_rx_0;    //0x1b60
    pm_register_address_type mipi_bif_data_rx_1;    //0x1b61
    pm_register_address_type mipi_bif_data_rx_2;    //0x1b62
    pm_register_address_type rx_fifo_word1_0;       //0x1b63
    pm_register_address_type rx_fifo_word1_1;       //0x1b64
    pm_register_address_type rx_fifo_word2_0;       //0x1b65
    pm_register_address_type rx_fifo_word2_1;       //0x1b66
    pm_register_address_type rx_fifo_word3_0;       //0x1b67
    pm_register_address_type rx_fifo_word3_1;       //0x1b68
    pm_register_address_type rx_fifo_word4_0;       //0x1b69
    pm_register_address_type rx_fifo_word4_1;       //0x1b6a
    pm_register_address_type mipi_bif_error;        //0x1b70
    pm_register_address_type bat_gone_cfg;          //0x1ba7
    pm_register_address_type bat_rmv_deb_timer;     //0x1ba8
    pm_register_address_type bat_pres_deb_timer;    //0x1ba9
}schg_bsi_register_ds;

typedef struct
{
    uint32                   base_address;           //0x1500
    
    pm_register_address_type perph_type;             //0x004
    pm_register_address_type perph_subtype;          //0x005
    pm_register_address_type lockbit_d1;             //0x0D1
    pm_register_address_type lockbit_d2;             //0x0D2
    pm_register_address_type spare;                  //0x0DF
    pm_register_address_type typec_snk_status;       //0x006
    pm_register_address_type typec_mode_cfg;         //0x044
    pm_register_address_type typec_vconn_control;    //0x046
    pm_register_address_type typec_debug_access_snk_cfg;  //0x04A
    pm_register_address_type typec_debug_access_status;  //0x007
}schg_typec_register_ds;

typedef struct
{
  uint32                   base_address;           //0x1900

  pm_register_address_type perph_type;             //0x004
  pm_register_address_type perph_subtype;          //0x005
  pm_register_address_type multi_port_arbiter;     //0x006

}schg_bob_register_ds;


typedef struct
{
    schg_bsi_register_ds          *bsi_register;
    schg_chgr_register_ds         *chgr_register;
    schg_dcdc_register_ds          *dcdc_register;
    schg_batif_register_ds        *batif_register;
    schg_usb_register_ds          *usb_register;
    schg_dc_register_ds           *dc_register;
    schg_misc_register_ds         *misc_register;
    schg_typec_register_ds        *typec_register;
    schg_bob_register_ds          *bob_register;
}schg_register_ds;


typedef struct
{
    pm_comm_info_type          *comm_ptr;
    schg_register_ds           *schg_register;
    uint8                      num_of_peripherals;
}pm_schg_data_type;


/*===========================================================================

                     FUNCTION DECLARATION

===========================================================================*/
void pm_schg_driver_init(pm_comm_info_type *comm_ptr, peripheral_info_type *peripheral_info, uint8 pmic_index);

pm_schg_data_type* pm_schg_get_data(uint8 pmic_index);

uint8 pm_schg_get_dig_major(void);

void pm_schg_get_pmic_info(uint8 *pmic_index, uint8 *slave_id, uint8* charger_count, uint8 max_charger);

#endif // __PM_SCHG_DRIVER_H__
