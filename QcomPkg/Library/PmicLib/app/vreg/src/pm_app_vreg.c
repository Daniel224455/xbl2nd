/*! \file
*  
*  \brief  pm_app_vreg.c
*  \details Implementation file for vreg app API.
*    
*  &copy; Copyright 2016-2017 QUALCOMM Technologies Incorporated, All Rights Reserved
*/

/*===========================================================================
EDIT HISTORY FOR MODULE

This document is created by a code generator, therefore this section will
not contain comments describing changes made to the module.

when       who     what, where, why
--------   ---     ----------------------------------------------------------
10/20/17   czq     Add bob OCP checking and cleraing. 
 
11/02/16   aab     Creation
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES 

===========================================================================*/
#include "pm_err_flags.h"
#include "pm_config_target.h"  
#include "pm_app_vreg.h"
#include "pm_ldo.h"
#include "pm_smps.h"
#include "pm_bob.h"
#include "pm_smps_driver.h"
#include "pm_ldo_driver.h"
#include "pm_bob_driver.h"
#include "pm_log_utils.h"


/*===========================================================================

                   API Implementation

===========================================================================*/
pm_err_flag_type
pm_app_vreg_clear_ocp_status(void)
{
  pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
  uint32 pmic_index = 0;
  uint8 smps_count = 0;
  uint8 ldo_count = 0;
  uint8 bob_count = 0;
  uint8 num_smps_periph = 0;
  uint8 num_ldo_periph  = 0;
  uint8 num_bob_periph = 0;
  boolean ocp_occured = FALSE;

  for(pmic_index = 0; pmic_index < PM_MAX_NUM_PMICS; pmic_index++)
  {
       //SMPS
       num_smps_periph = pm_smps_get_num_peripherals(pmic_index);
       for (smps_count = PM_SMPS_1; smps_count < num_smps_periph; smps_count++)
       {
          ocp_occured = FALSE;
          err_flag |= pm_smps_ocp_latched_status_clear(pmic_index, smps_count, &ocp_occured);//If OCP occured clear status
          if (ocp_occured) 
          {
             pm_log_message("OCP Occured: PMIC: %d; SMPS: %d ", pmic_index, smps_count+1); 
          }
       }

       //LDO
       num_ldo_periph = pm_ldo_get_num_peripherals(pmic_index);
       for (ldo_count = PM_LDO_1; ldo_count < num_ldo_periph; ldo_count++)
       {
          ocp_occured = FALSE;
          err_flag |= pm_ldo_ocp_latched_status_clear(pmic_index, ldo_count, &ocp_occured); //If OCP occured clear status
          if (ocp_occured) 
          {
             pm_log_message("OCP Occured: PMIC: %d; LDO: %d ", pmic_index, ldo_count+1); 
          }
       }
	   
	   //BOB
       num_bob_periph = pm_bob_get_num_peripherals(pmic_index);
       for (bob_count = PM_BOB_1; bob_count < num_bob_periph; bob_count++)
       {
          ocp_occured = FALSE;
          err_flag |= pm_bob_ocp_latched_status_clear(pmic_index, bob_count, &ocp_occured);//If OCP occured clear status
          if (ocp_occured) 
          {
             pm_log_message("OCP Occured: PMIC: %d; BOB: %d ", pmic_index, bob_count+1); 
          }
       }
   }

  return err_flag;
}

