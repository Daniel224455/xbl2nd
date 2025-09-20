/*! \file pm_sbl_boot.c
*  \n
*  \brief This file contains PMIC device initialization function where initial PMIC
*  \n SBL settings are configured through the PDM auto-generated code.
*  \n
*  \n &copy; Copyright 2019 Qualcomm Technologies Inc, All Rights Reserved
*/
/* =======================================================================
Edit History
This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.


when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/27/19   kugaurav Branch for poipu auto
04/24/18   pxm     Branch for hana auto
========================================================================== */
/*===========================================================================

                     INCLUDE FILES 

===========================================================================*/
#include "pm_err_flags.h"
#include "pm_boot.h"
#include "pm_ldo.h"
#include "pm_sbl_boot.h"
#include "pm_device.h"
#include "pm_config_sbl.h"
#include "pm_config_sbl_test.h"
#include "pm_pbs_info.h"
#include "pm_target_information.h"
#include "pm_sbl_boot_target.h"
#include "pm_log_utils.h"
#include "pm_pbs.h"
#include "DALDeviceId.h"
#include "DDIPlatformInfo.h"
#include "DDIChipInfo.h"
#include "pmio_pon.h"
#include "railway.h"
#include "CoreVerify.h"

/*===========================================================================

                     PROTOTYPES 

===========================================================================*/
static pm_err_flag_type pm_sbl_pre_config(void);
static pm_err_flag_type pm_sbl_target_detect(void);
static pm_err_flag_type pm_config_wlan_active_and_retention_level(void);

#ifdef ENABLE_AUTO_BOOT_OPTIMIZATION
extern  boolean boot_dload_is_dload_mode_set( void );
#endif

/*===========================================================================

                        GLOBALS and TYPE DEFINITIONS 

===========================================================================*/
static boolean pm_ram_image_loaded_flag = FALSE;
static pm_sbl_specific_data_type      *sbl_param_ptr     = NULL;

/*===========================================================================

                        FUNCTION DEFINITIONS 

===========================================================================*/
pm_err_flag_type 
pm_device_init ( void )
{
   static pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
   uint32 initial_num_spmi_transn = pm_get_num_spmi_transaction(0);

   err_flag |= pm_device_setup();
   if( (pm_is_target_pre_silicon() == TRUE) && (pm_is_pmic_present(PMIC_A) == FALSE) )
   {
      pm_log_message("Bootup: No PMIC on RUMI Target");
      return err_flag = PM_ERR_FLAG_SUCCESS;
   }

   pm_target_information_init();
   pm_comm_info_init();

   err_flag |= pm_device_pre_init();

   err_flag |= pm_pon_init();

   err_flag |= pm_pbs_info_rom_init();  /* Read PBS INFO for the pmic rom devices */

   err_flag |= pm_sbl_pre_config(); /* SBL Pre Configuration */

   err_flag |= pm_sbl_config(); /* SBL Configuration */
   if (err_flag == PM_ERR_FLAG_SUCCESS)
   {
      pm_ram_image_loaded_flag = TRUE;
   }

#ifndef ENABLE_AUTO_BOOT_OPTIMIZATION
   err_flag |= pm_sbl_config_test(); /* SBL Configuration validation, only executes complete code if spare reg 0x88F bit 0 is set*/

   err_flag |= pm_pbs_info_ram_init();  /* Read PBS INFO for the pmic ram devices */

   err_flag |= pm_pbs_ram_version_validation_test(); /* PBS RAM Version validation, only executes complete code if spare reg 0x88F bit 0 is set*/
#endif
   err_flag |= pm_device_post_init(); /* Initialize PMIC with the ones PDM can not perform */

   
  //Write to Spare bit for pm_device_init_status
   if(err_flag == PM_ERR_FLAG_SUCCESS)
   {
      err_flag = pm_comm_write_byte_mask(PMIC_A_SLAVEID_PRIM, PMIO_PON_PERPH_RB_SPARE_ADDR, PON_PERPH_RB_SPARE_DEVICE_INIT_MASK,PON_PERPH_RB_SPARE_DEVICE_INIT_MASK, 0); 
   }

   pm_log_message("Device Init # SPMI Transn: %d", pm_get_num_spmi_transaction(initial_num_spmi_transn));
   return err_flag; /* NON ZERO return means an ERROR */
}



pm_err_flag_type
pm_sbl_chg_init (void)
{
   pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
   return err_flag;
}


static pm_err_flag_type
pm_sbl_pre_config(void)
{
  pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;

  if (sbl_param_ptr == NULL)
  {
     sbl_param_ptr = (pm_sbl_specific_data_type*)pm_target_information_get_specific_info(PM_PROP_SBL_SPECIFIC_DATA);
  }
  CORE_VERIFY_PTR(sbl_param_ptr);
#ifdef ENABLE_AUTO_BOOT_OPTIMIZATION
  if (boot_dload_is_dload_mode_set())
     err_flag |= pm_log_pon_reasons();
#else
  err_flag |= pm_log_pon_reasons();
#endif
  err_flag |= pm_sbl_target_detect();

  err_flag = pm_pbs_header_info_init();
#ifndef ENABLE_AUTO_BOOT_OPTIMIZATION
  pm_pbs_get_pon_reason_hist_dataset();
#endif 
  return err_flag;
}


pm_err_flag_type
pm_sbl_target_detect(void) /* SBL Target detect */
{
   static pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS; 

   return err_flag;
}

pm_err_flag_type pm_system_init(void)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;

    return err_flag;
}

