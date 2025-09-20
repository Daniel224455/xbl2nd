/*! \file
*  
*  \brief  pm_schg_driver.c driver implementation.
*  \details charger driver implementation.
*  &copy;
*  Copyright (c) 2017 - 2018 Qualcomm Technologies, Inc.  All Rights Reserved. 
*  Qualcomm Technologies Proprietary and Confidential.
*/

/*===========================================================================

EDIT HISTORY FOR MODULE


when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
08/01/18    ivy    Added support to set batt therm pull up to register
06/29/18    dc	   Expand ICL Status API for Max ICL
06/21/18    cs	   Added support for multi-port.
05/17/18    ivy	   Added support for Qi charging ICL register
05/14/18    ra     Support for Debug Accessory Mode
03/01/18    ra     Adding skin and die temp registers
03/05/18    pxm    Remove unused registers.
01/23/18    aab    Added support for USB typec APIs
01/15/18    ra     Adding JEITA registers
11/12/17    aab    Moved common Register address defination to driver file  
12/19/16    sm     Added API to get SCHG Charger PmicIndex
09/07/16    sm     Added pm_schg_get_dig_major API to return digital major
04/18/14    al     Initial revision
========================================================================== */

/*===========================================================================

                    INCLUDE FILES

===========================================================================*/
#include "pm_schg_driver.h"
#include "CoreVerify.h"
#include "pm_utils.h"
#include "hw_module_type.h"


/*===========================================================================

                        STATIC VARIABLES

===========================================================================*/

/* Static global variable to store the SCHG driver data */
static pm_schg_data_type *pm_schg_data_arr[PM_MAX_NUM_PMICS];
static pm_register_data_type dig_major = 0;

#define SCHG_MISC_REVISION2   0x1601
#define SCHG_SLAVE_ID          0x2 


static schg_chgr_register_ds schg_chgr_reg[1] =
{
    {
        0x1000, 0x006, 0x009, 0x042, 0x51, 0x060, 0x061, 0x062, 0x063, 0x070, 0x086, 0x090, 0x091,
        0x092, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F
    }
};

static schg_dcdc_register_ds schg_dcdc_reg[1] =
{
   {0x1100, 0x004, 0x005,0x006, 0x0D1, 0x0D2, 0x0DF, 0x00D,  0x010, 0x011, 0x012, 0x013, 0x014, 0x015, 0x016, 0x018, 0x019, 0x01A, 0x01B, 0x040, 0x051, 0x052,
     0x053, 0x083, 0x8C}
};


static schg_batif_register_ds schg_batif_reg[1] =
{
    {
        0x1200 , 0x006, 0x010, 0x011, 0x012, 0x013, 0x014, 0x015, 0x016, 0x018, 0x019, 0x040, 0x061, 0x070, 0x86, 0x91
    }
};


static schg_usb_register_ds schg_usb_reg[1] =
{
    {
        0x1300, 0x007, 0x008, 0x010, 0x011, 0x012, 0x013, 0x014, 0x015, 0x016, 0x018, 0x019, 0x040, 0x041, 0x060, 0x062, 0x065, 0x066,0x070, 0x080
    }
};

static schg_dc_register_ds schg_dc_reg[1] =
{
    {
        0x1400, 0x010, 0x018, 0x019, 0x040
    }
};


static schg_misc_register_ds schg_misc_reg[1] =
{
    {
        0x1600, 0x00C, 0x00D, 0x010, 0x011, 0x012, 0x013, 0x014, 0x015, 0x016, 0x018, 0x019,
        0x041, 0x043, 0x051, 0x053, 0x061, 0x070, 0x0A0, 0x0A1, 0x0A2, 0x0A3
    }
};


static schg_bsi_register_ds schg_bsi_reg[1] =
{
    {
        0x1b00, 0x008, 0x046, 0x04f, 0x050, 0x051, 0x052, 0x053, 0x054, 0x055, 0x056, 0x059, 0x05a, 0x05b, 0x05c, 0x05d, 0x060, 0x061, 0x062,
        0x063, 0x064, 0x065, 0x066, 0x067, 0x068, 0x069, 0x06a, 0x070, 0x0a7, 0x0a8, 0x0a9
    }
};


static schg_typec_register_ds schg_typec_reg[1] = 
{
    {
        0x1500, 0x004 ,0x005, 0x0D1, 0x0D2, 0x0DF, 0x006, 0x044,0x046, 0x04A, 0x007
    }
};

static schg_bob_register_ds schg_bob_reg[1] =
{
  {
    0x1900, 0x004, 0x005, 0x006
  }
};

static schg_register_ds  schg_reg[1]=
{
   {
    schg_bsi_reg,
    schg_chgr_reg,
    schg_dcdc_reg,
    schg_batif_reg,
    schg_usb_reg,
    schg_dc_reg,
    schg_misc_reg,
    schg_typec_reg,
    schg_bob_reg
   }
};


/*===========================================================================

                        FUNCTION DEFINITIONS

===========================================================================*/
void pm_schg_driver_init(pm_comm_info_type *comm_ptr, peripheral_info_type *peripheral_info, uint8 pmic_index)
{
    pm_schg_data_type *schg_ptr = NULL;
    pm_err_flag_type err_flag = PM_ERR_FLAG__SUCCESS;

    /*return if coincell since it is not supported at the moment*/
    if(PM_HW_MODULE_CHARGER_COINCELL == peripheral_info->peripheral_subtype)
    {
      return;
    }

    schg_ptr = pm_schg_data_arr[pmic_index];

    if (NULL == schg_ptr)
    {
        pm_malloc( sizeof(pm_schg_data_type), (void**)&schg_ptr);

        /* Assign Comm ptr */
        schg_ptr->comm_ptr = comm_ptr;

        /* schg Register Info - Obtaining Data through dal config */
        schg_ptr->schg_register = schg_reg;
        CORE_VERIFY_PTR(schg_ptr->schg_register);

        schg_ptr->num_of_peripherals = pm_target_information_get_periph_count_info(PM_PROP_CHARGER_NUM, pmic_index);
        CORE_VERIFY(schg_ptr->num_of_peripherals != 0);

        pm_schg_data_arr[pmic_index] = schg_ptr;

        err_flag = pm_comm_read_byte(SCHG_SLAVE_ID, SCHG_MISC_REVISION2, &dig_major, 0);
        CORE_VERIFY(err_flag == PM_ERR_FLAG__SUCCESS);
    }
}

pm_schg_data_type* pm_schg_get_data(uint8 pmic_index)
{
  if(pmic_index < PM_MAX_NUM_PMICS)
  {
      return pm_schg_data_arr[pmic_index];
  }

  return NULL;
}

uint8 pm_schg_get_num_peripherals(uint8 pmic_index)
{
  if((pmic_index < PM_MAX_NUM_PMICS)&&
  	  (pm_schg_data_arr[pmic_index] != NULL))
  {
      return pm_schg_data_arr[pmic_index]->num_of_peripherals;
  }

  return 0;
}

uint8 pm_schg_get_dig_major(void)
{
  return dig_major;
}


void pm_schg_get_pmic_info(uint8 *pmic_index, uint8 *slave_id, uint8* charger_count, uint8 max_charger)
{
    uint8 i = 0, count = 0;

    if(NULL == pmic_index || NULL == slave_id || NULL == charger_count)
    {
        return;
    }

    for(i = 0; (i < PM_MAX_NUM_PMICS) && (count < max_charger); i++)
    {
        if(pm_schg_data_arr[i] != NULL)
        {
            pmic_index[count] = pm_schg_data_arr[i]->comm_ptr->pmic_index;
            slave_id[count]   = pm_schg_data_arr[i]->comm_ptr->slave_id;
            count++;
        }
    }
    *charger_count = count;
    return;
}

