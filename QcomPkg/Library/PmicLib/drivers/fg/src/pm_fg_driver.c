/*! \file
*  \n
*  \brief  pm_fg_driver.c
*  \details
*  Copyright (c) 2014 - 2018 Qualcomm Technologies, Inc.  All Rights Reserved.
*  Qualcomm Technologies Proprietary and Confidential.
*/

/*===========================================================================

                        EDIT HISTORY

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

when        who     what, where, why
--------    ---     ---------------------------------------------------------------- 
01/30/19    pbitra  Added support to set batt therm freq
08/01/18    ivy     Added support to set batt therm pull up to register
06/18/18    cs      turn off FG_BTM for batt_id after ID is successfully read to avoid adc corruption for 1.0
03/05/18    pxm     Remove unused registers.
01/29/18    cs      updated batt_info_reg size
11/12/17    aab     Moved common Register address defination to driver file  
12/19/16    sm      Added API to get FG PmicIndex
07/18/16    llioe   Handled register-address differences v1.1(and earlier) vs v2.0
09/22/14    aab     Porting FG driver to SBL        
08/20/14    al      Updating comm lib 
08/29/14    al      KW fixes
06/09/14    al      Arch update 
04/01/14    va      New file
================================================================================== */

#include "pm_fg_driver.h"
#include "CoreVerify.h"
#include "pm_utils.h"
#include "device_info.h"
#include "hw_module_type.h"

/*===========================================================================

                        STATIC VARIABLES

===========================================================================*/
/* Static global variable to store the FG driver data */
static pm_fg_data_type *pm_fg_data_arr[PM_MAX_NUM_PMICS];

static fg_batt_soc_register_ds batt_soc_reg[1] =
{
  {
    0x4000, 0x009, 0x010, 0x011, 0x012, 0x013, 0x014, 0x015, 0x016, 0x018, 0x019, 0x046, 0x048
  }
};

static fg_batt_info_register_ds batt_info_reg[1] =
{
  //[0] is latest revision
  {
    0x4100, 0x069, 0x0A1, 0x0A0, 0x0A3, 0x0A2, 0x0A7, 0x0A6, 0x0A9, 0x0A8, 0xEE, 0xEF
  }
};

static fg_mem_if_register_ds mem_if_reg[1] =
{
  {
    0x4300, 0x001, 0x007, 0x010, 0x018, 0x019, 0x040, 0x050, 0x051, 0x052, 0x054, 0x056, 0x057,
    0x060, 0x061, 0x062, 0x063, 0x064, 0x067, 0x068, 0x070, 0x071
  }
};

static fg_adc_rr_register_ds adc_rr_reg[1] =
{
  {
    0x4200, 0x010, 0x018, 0x019, 0x050, 0x051, 0x058, 0x066, 0x067, 0x06E, 0x06F, 0x076, 0x077, 0x81, 0x82, 0x088, 0x089
  }
};

static adc_fg5_btm_4_register_ds fg_btm_reg[1] =
{
  {
    0x3500, 0x046, 0x047, 0x066, 0x06F
  }
};

static fg_register_ds  fg_reg[1]=
{
   {
     batt_soc_reg,
     batt_info_reg,
     mem_if_reg,
     adc_rr_reg,
     fg_btm_reg,
   }
};


/*===========================================================================

                        FUNCTION DEFINITIONS

===========================================================================*/
void pm_fg_driver_init(pm_comm_info_type *comm_ptr, peripheral_info_type *peripheral_info, uint8 pmic_index)
{
    pm_fg_data_type *fg_ptr = NULL;

    fg_ptr = pm_fg_data_arr[pmic_index];

    if (NULL == fg_ptr)
    {
        pm_malloc( sizeof(pm_fg_data_type), (void**)&fg_ptr);

        /* Assign Comm ptr */
        fg_ptr->comm_ptr = comm_ptr;

        /* fg Register Info - Obtaining Data through dal config */
        fg_ptr->fg_register = fg_reg;
        CORE_VERIFY_PTR(fg_ptr->fg_register);

        fg_ptr->num_of_peripherals = pm_target_information_get_periph_count_info(PM_PROP_FG_NUM, pmic_index);
        CORE_VERIFY(fg_ptr->num_of_peripherals != 0);

        pm_fg_data_arr[pmic_index] = fg_ptr;
    }
}

pm_fg_data_type* pm_fg_get_data(uint8 pmic_index)
{
  if(pmic_index <PM_MAX_NUM_PMICS)
  {
      return pm_fg_data_arr[pmic_index];
  }

  return NULL;
}

uint8 pm_fg_get_num_peripherals(uint8 pmic_index)
{
  if((pmic_index < PM_MAX_NUM_PMICS) &&
     (pm_fg_data_arr[pmic_index] !=NULL))
  {
      return pm_fg_data_arr[pmic_index]->num_of_peripherals;
  }

  return 0;
}

void pm_fg_get_pmic_info(uint8 *pmic_index, uint8 *slave_id)
{
    uint8 i = 0;

    if(!pmic_index || !slave_id)
        return;

    for(i = 0; i < PM_MAX_NUM_PMICS; i++)
    {
        if(pm_fg_data_arr[i])
        {
            *pmic_index = pm_fg_data_arr[i]->comm_ptr->pmic_index;
            *slave_id   = pm_fg_data_arr[i]->comm_ptr->slave_id;
            break;
        }
    }

    return;
}

