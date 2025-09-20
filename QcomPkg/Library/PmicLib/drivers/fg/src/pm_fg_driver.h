#ifndef __PM_FG_DRIVER_H__
#define __PM_FG_DRIVER_H__

/*! \file
*  \n
*  \brief  pm_fg_driver.h 
*  \details  
*  Copyright (c) 2016 - 2018 Qualcomm Technologies, Inc.  All Rights Reserved. 
*  Qualcomm Technologies Proprietary and Confidential.
*/

/*===========================================================================
                             Edit History
  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.


when        who    what, where, why
--------    ---    ----------------------------------------------------------
01/30/19    pbitra Added support to set batt therm freq
08/01/18    ivy    Added support to set batt therm pull up to register
06/18/18    cs     turn off FG_BTM for batt_id after ID is successfully read to avoid adc corruption for 1.0
03/05/18    pxm    Remove unused APIs and registers.
09/19/17    cs     updated register map
01/31/17    va     Adding DMA arb reg
12/19/16    sm     Added API to get FG PmicIndex
01/27/16    al     New File
===========================================================================*/

/*===========================================================================

                        INCLUDE FILES

===========================================================================*/
#include "pm_target_information.h"

/*===========================================================================

                        TYPE DEFINES AND ENUMS

===========================================================================*/
/*  fg_batt_soc_register_ds */
typedef struct
{
  pm_register_address_type base_address;           //0x4000
  pm_register_address_type monotonic_soc;          //0x009
  pm_register_address_type int_rt_sts;             //0x010
  pm_register_address_type int_set_type;           //0x011
  pm_register_address_type int_polarity_high;      //0x012
  pm_register_address_type int_polarity_low;       //0x013
  pm_register_address_type int_latched_clr;        //0x014
  pm_register_address_type int_en_set;             //0x015
  pm_register_address_type int_en_clr;             //0x016
  pm_register_address_type int_latched_sts;        //0x018
  pm_register_address_type int_pending_sts;        //0x019
  pm_register_address_type en_ctl;                 //0x046
  pm_register_address_type restart;                //0x048
}fg_batt_soc_register_ds;


/*fg_batt_info_register_ds*/
typedef struct
{
  pm_register_address_type base_address;              //0x4100
  pm_register_address_type esr_pull_down_cfg;        // 0x069
  pm_register_address_type vbatt_msb;                // 0x0A1
  pm_register_address_type vbatt_lsb;                // 0x0A0
  pm_register_address_type ibatt_msb;                // 0x0A3
  pm_register_address_type ibatt_lsb;                // 0x0A2
  pm_register_address_type vbatt_msb_cp;             // 0x0A7
  pm_register_address_type vbatt_lsb_cp;             // 0x0A6
  pm_register_address_type ibatt_msb_cp;             // 0x0A9
  pm_register_address_type ibatt_lsb_cp;             // 0x0A8
  pm_register_address_type peek_mux4;                 //0x0EE
  pm_register_address_type peek_rd;                   //0x0EF
}fg_batt_info_register_ds;


/*fg_mem_if_register_ds*/
typedef struct
{
  pm_register_address_type base_address;            // 0x4300
  pm_register_address_type dig_major;               // 0x001
  pm_register_address_type ima_rdy_sts;             // 0x007
  pm_register_address_type int_rt_sts;              // 0x010
  pm_register_address_type int_latched_sts;         // 0x018
  pm_register_address_type int_pending_sts;         // 0x019
  pm_register_address_type mem_arb_cfg;             // 0x040
  pm_register_address_type mem_intf_cfg;            // 0x050
  pm_register_address_type mem_intf_ctl;            // 0x051
  pm_register_address_type ima_cfg;                 // 0x052
  pm_register_address_type ima_operation_sts;       // 0x054
  pm_register_address_type ima_hardware_sts;        // 0x056
  pm_register_address_type fg_beat_count;           // 0x057
  pm_register_address_type ima_byte_en;             // 0x060
  pm_register_address_type mem_intf_addr_lsb;       // 0x061
  pm_register_address_type mem_intf_addr_msb;       // 0x062
  pm_register_address_type mem_intf_wr_data0;       // 0x063
  pm_register_address_type mem_intf_wr_data1;       // 0x064
  pm_register_address_type mem_intf_rd_data0;       // 0x067
  pm_register_address_type mem_intf_rd_data1;       // 0x068
  pm_register_address_type dma_sts;                 // 0x070
  pm_register_address_type dma_intf_ctl;            // 0x071
}fg_mem_if_register_ds;

/*fg_adc_rr_register_ds*/
typedef struct
{
  pm_register_address_type base_address;           //0x4200
  pm_register_address_type int_rt_sts;             //0x010
  pm_register_address_type int_latched_sts;        //0x018
  pm_register_address_type int_pending_sts;        //0x019
  pm_register_address_type adc_ctl0;               //0x050
  pm_register_address_type adc_ctl1;               //0x051
  pm_register_address_type fake_batt_low_lsb;      //0x058
  pm_register_address_type batt_id_hi_bias_lsb;    //0x066
  pm_register_address_type batt_id_hi_bias_msb;    //0x067
  pm_register_address_type batt_id_med_bias_lsb;   //0x06E
  pm_register_address_type batt_id_med_bias_msb;   //0x06F
  pm_register_address_type batt_id_lo_bias_lsb;    //0x076
  pm_register_address_type batt_id_lo_bias_msb;    //0x077
  pm_register_address_type batt_therm_base_cfg1;   //0x081
  pm_register_address_type batt_therm_freq;        //0x082
  pm_register_address_type batt_temperature_lsb;   //0x088
  pm_register_address_type batt_temperature_msb;   //0x089
}fg_adc_rr_register_ds;

/*ADC_FG5_BTM_4_register_ds*/
typedef struct
{
  pm_register_address_type base_address;           //0x3500
  pm_register_address_type en_ctrl1;               //0x046
  pm_register_address_type conv_req;               //0x047
  pm_register_address_type btm_ctl_0;              //0x066
  pm_register_address_type btm_en_1;               //0x06F  
}adc_fg5_btm_4_register_ds;



typedef struct
{
  fg_batt_soc_register_ds   *batt_soc_register;
  fg_batt_info_register_ds  *batt_info_register;
  fg_mem_if_register_ds     *mem_if_register;
  fg_adc_rr_register_ds     *adc_rr_register;
  adc_fg5_btm_4_register_ds *adc_fg5_btm_register;
}fg_register_ds;

typedef struct
{
  pm_comm_info_type     *comm_ptr;
  fg_register_ds        *fg_register;
  uint8                 num_of_peripherals;
}pm_fg_data_type;


/*===========================================================================

                     FUNCTION DECLARATION

===========================================================================*/

/* driver init */
void pm_fg_driver_init(pm_comm_info_type *comm_ptr, peripheral_info_type *peripheral_info, uint8 pmic_index);

pm_fg_data_type* pm_fg_get_data(uint8 pmic_index);

uint8 pm_fg_get_num_peripherals(uint8 pmic_index);

void pm_fg_get_pmic_info(uint8 *pmic_index, uint8 *slave_id);

#endif // __PM_FG_DRIVER_H__
