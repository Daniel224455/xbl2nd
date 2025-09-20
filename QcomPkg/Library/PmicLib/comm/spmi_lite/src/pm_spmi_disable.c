/*! \file 
*  \n
*  \brief pm_comm_spmi_lite.c ---- 
*  \n
*  \n &copy; Copyright 2017 QUALCOMM Technologies Incorporated, All Rights Reserved
*/
/* =======================================================================
Edit History
This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.


when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
04/19/17   al   created  
========================================================================== */

/*===========================================================================

INCLUDE FILES FOR MODULE

===========================================================================*/
#include "pm_comm.h"  
#include "SpmiBus.h"
#include "device_info.h"

/*===========================================================================

                        GLOBALS and TYPE DEFINITIONS 

===========================================================================*/

#define PBS_RAM_SLAVE_ID                0x2
#define PBS_RAM_CONFIG_ADDR             0x7040
#define PBS_CORE_MEM_INTF_CTL_ADDR      0x7041
#define PBS_RAM_WRITE_ADDR              0x7048
#define PMI_RAM_BASE_ADDR               (0x800+504)
#define PBS_CLIENT2_EN_CTL              0x7346
#define PBS_CLIENT2_TRIG_CTL            0x7342
#define PBS_CORE_TRIG_START_ADDR_LSB_0  0x7050
#define PBS_CORE_TRIG_START_ADDR_MSB_0  0x7051
#define PBS_BYTE_NUMBER                 0x4
#define BOB_VSET_BYTE                   0x2
#define BOB_CONFIG_VOUT_LB              0xA040
#define BOB_SLAVE_ID                    0x3
/*===========================================================================

                        FUNCTION DEFINITIONS 

===========================================================================*/
extern boolean pm_version_initialized;

pm_err_flag_type pm_spmi_disable_comm(uint8 slave_id)
{
  pm_register_data_type enable = 0x80;
  pm_register_address_type ram_base_lsb = (PMI_RAM_BASE_ADDR & 0xFF);
  pm_register_address_type ram_base_msb = (PMI_RAM_BASE_ADDR & 0xFF00)>>8;
  pm_register_data_type pbs_config_data[PBS_BYTE_NUMBER] = {0x80,0xC0, ram_base_lsb, ram_base_msb};
  pm_register_data_type pbs_spmi_slave_rb_data[PBS_BYTE_NUMBER] = {0x00, 0x43, 0x08, 0x02};
  pm_register_data_type pbs_ram_return_data[PBS_BYTE_NUMBER] = {0xff, 0xff, 0xff, 0xfc};
  pm_register_data_type bob_volt_config_data[BOB_VSET_BYTE] = {0x74, 0x0E}; //0xE74 = 3700

  /* initialize SPMI */
  SpmiBus_Init();

  /*increaes BOB to 3.7v before disabling*/
  SpmiBus_WriteLong(BOB_SLAVE_ID, 0, BOB_CONFIG_VOUT_LB, bob_volt_config_data, BOB_VSET_BYTE);

  /*configure PBS for RAM sequence write*/
  SpmiBus_WriteLong(PBS_RAM_SLAVE_ID, 0, PBS_RAM_CONFIG_ADDR, pbs_config_data, PBS_BYTE_NUMBER);
  /*write to set PMI into reset*/
  SpmiBus_WriteLong(PBS_RAM_SLAVE_ID, 0, PBS_RAM_WRITE_ADDR, pbs_spmi_slave_rb_data, PBS_BYTE_NUMBER);
  /*write return sequence */
  SpmiBus_WriteLong(PBS_RAM_SLAVE_ID, 0, PBS_RAM_WRITE_ADDR, pbs_ram_return_data, PBS_BYTE_NUMBER);
  /*enable the trigger*/
  SpmiBus_WriteLong(PBS_RAM_SLAVE_ID, 0, PBS_CLIENT2_EN_CTL, &enable, 1);
  SpmiBus_WriteLong(PBS_RAM_SLAVE_ID, 0, PBS_CORE_TRIG_START_ADDR_LSB_0, (uint8*)&ram_base_lsb, 1);
  SpmiBus_WriteLong(PBS_RAM_SLAVE_ID, 0, PBS_CORE_TRIG_START_ADDR_MSB_0, (uint8*)&ram_base_msb, 1);
  SpmiBus_WriteLong(PBS_RAM_SLAVE_ID, 0, PBS_CLIENT2_TRIG_CTL, &enable, 1);

  /*to re-rum pmic version detection*/
  pm_version_deinit();

  return PM_ERR_FLAG_SUCCESS;
}
