/*! \file pm_clk_driver.c
 *  \n
 *  \brief  
 *  \n  
 *  \n &copy; Copyright 2013-2019 QUALCOMM Technologies Incorporated, All Rights Reserved
 */

/* =======================================================================
                             Edit History
  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.

 
when        who     what, where, why
--------    ---     ----------------------------------------------------------
01/03/18    pxm     Support LN_BB_CLK3
11/12/17    aab     Moved common Register address defination to driver file  
07/16/14    akm     Comm change Updates
03/31/14    akm     Updated to the latest PMIC driver architecture  
09/25/13    kt      Updated clk module driver.
02/01/13    hw      Rearchitecting clk module driver to peripheral driver
========================================================================== */
/*===========================================================================

                     INCLUDE FILES 

===========================================================================*/
#include "pm_clk_driver.h"
#include "CoreVerify.h"
#include "device_info.h"
#include "pm_target_information.h"

/*===========================================================================

                        STATIC VARIABLES 

===========================================================================*/

/* Static global variable to store the CLK driver data */
static pm_clk_data_type *pm_clk_data_arr[PM_MAX_NUM_PMICS];


static pm_clk_register_info_type clk_reg = 
{   
  0x5000, 0x100, 0x044, 0x046, 0x048, 0x043, 0x008
};

static pm_clk_sleep_register_info_type clk_sleep_reg[1] = 
{
//  1.cal_rc4
  {0x05B}
};

static pm_clk_xo_register_info_type clk_xo_reg[1] =
{
//	1.model_ctl1 - 2.xo_adj_fine
	{0x044, 0x05C}
};

// The index of each peripheral here equal to the peripheral offset in ipcat
static pm_clk_type clk_common[PM_MAX_NUM_CLKS] =
{
   PM_CLK_XO,
   PM_CLK_BB_1,
   PM_CLK_BB_2,
   PM_CLK_BB_3,
   PM_CLK_RF_1,
   PM_CLK_RF_2,
   PM_CLK_RF_3,
   PM_CLK_RF_4,
   PM_CLK_LN_BB,
   PM_CLK_DIST,
   PM_CLK_SLEEP,
   PM_CLK_DIV_1,
   PM_CLK_DIV_2,
   PM_CLK_DIV_3,
};

/*===========================================================================

                     LOCAL FUNCTION PROTOTYPE

===========================================================================*/
static void pm_clk_periph_map_init(pm_clk_data_type *clk_ptr);
              
/*===========================================================================

                     INTERNAL DRIVER FUNCTIONS 

===========================================================================*/

void pm_clk_driver_init(pm_comm_info_type *comm_ptr, peripheral_info_type *peripheral_info,  uint8 pmic_index)
{
  pm_clk_data_type *clk_ptr = NULL;
  uint32 clk_index = 0;
  pm_register_address_type base_address = 0;
  pm_register_address_type periph_offset = 0;

  /* Initialize clock driver only for PMICs which have XO_CORE */
  if (pm_clk_data_arr[pmic_index] == NULL)
  {
     if ((peripheral_info->peripheral_subtype != PM_HW_MODULE_CLOCK_XO_CORE) && 
         (peripheral_info->peripheral_subtype != PM_HW_MODULE_CLOCK_XO_CORE_38P4MHZ))
     {
         return;
     }
  }

  if ((peripheral_info->peripheral_subtype == PM_HW_MODULE_CLOCK_CALRC2) || 
      (peripheral_info->peripheral_subtype == PM_HW_MODULE_CLOCK_CALRC3) || 
      (peripheral_info->peripheral_subtype == PM_HW_MODULE_CLOCK_LFRC))
  {
     return;
  }

  clk_ptr = pm_clk_data_arr[pmic_index];

  if (clk_ptr == NULL)
  {
    pm_malloc(sizeof(pm_clk_data_type), (void **)&clk_ptr);

    /* Assign Comm ptr */
    clk_ptr->comm_ptr = comm_ptr;

      /* Clock Register Info - Obtaining Data through dal config */
        clk_ptr->clk_reg_table = &clk_reg;

        CORE_VERIFY_PTR(clk_ptr->clk_reg_table);

        clk_ptr->clk_sleep_reg_table = clk_sleep_reg;

        CORE_VERIFY_PTR(clk_ptr->clk_sleep_reg_table);

        clk_ptr->clk_xo_reg_table = clk_xo_reg;

        CORE_VERIFY_PTR(clk_ptr->clk_xo_reg_table);
       
        /* Clock Peripheral Map Info - Obtaining Data through dal config */
        clk_ptr->clk_common = clk_common;

        CORE_VERIFY_PTR(clk_ptr->clk_common);

      /* This is kept for design consistency with other drivers. This is not really
         relied upon in clk case and will be auto-detected in the future */
        clk_ptr->num_of_peripherals = (uint32)PM_MAX_NUM_CLKS;


        /* Link a periph index to the device config data through a mapping table */
        pm_clk_periph_map_init(clk_ptr);


        DALSYS_memset(clk_ptr->periph_subtype, 0, sizeof(clk_ptr->periph_subtype));

        pm_clk_data_arr[pmic_index] = clk_ptr;
   }

    if (clk_ptr != NULL)
   {
        base_address = clk_ptr->clk_reg_table->base_address;
        periph_offset = clk_ptr->clk_reg_table->peripheral_offset;

        /* Peripheral Baseaddress should be >= first peripheral's base addr */
        CORE_VERIFY(peripheral_info->base_address >= base_address);

        /* Calculate CLK peripheral index */
        clk_index = ((peripheral_info->base_address - base_address)/periph_offset);

        /* Peripheral Index should be less than max number of peripherals */
        CORE_VERIFY(clk_index < PM_MAX_NUM_CLKS);

            /* Save Clock's Peripheral Subtype value */
        clk_ptr->periph_subtype[clk_index] = peripheral_info->peripheral_subtype;
   }
}

void pm_clk_periph_map_init(pm_clk_data_type *clk_ptr)
{
   uint8 i = 0;
   uint8 periph = 0;

   /* Initialize clk_periph mapping data */
   DALSYS_memset(clk_ptr->clk_perph_index, PM_CLK_INVALID_DATA, sizeof(clk_ptr->clk_perph_index));

   /* Go through device config table that contains the supported periph
      data and create a mapping table to be used when PMIC API is called.
      This mapping table is created to link between API level peripheral
      enum and it's periph index in the structure. 
      The goal is to allow the order of the API level peripheral enum to
      change without affecting the periph index used by the register. */
   for(i = 0; i < PM_MAX_NUM_CLKS; i++)
   {
      /* Local var periph created to satisfy KW */
      periph = clk_ptr->clk_common[i];

      if (periph < PM_ALL_CLKS)
      {
         clk_ptr->clk_perph_index[periph] = i;
      }
   }
}

pm_clk_data_type* pm_clk_get_data(uint8 pmic_index)
{
  if(pmic_index < PM_MAX_NUM_PMICS)
	{
		 return pm_clk_data_arr[pmic_index];
	}
	return NULL;

}


uint8 pm_clk_get_num_peripherals(uint8 pmic_index)
{
  if((pmic_index < PM_MAX_NUM_PMICS) && 
  	  (pm_clk_data_arr[pmic_index] !=NULL))
  {
      return pm_clk_data_arr[pmic_index]->num_of_peripherals;
  }

  return 0;
} 


