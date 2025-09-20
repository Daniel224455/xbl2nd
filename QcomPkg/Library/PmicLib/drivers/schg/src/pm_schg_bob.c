/*! \file
*  
*  \brief  pm_schg_BOB.c driver implementation.
*  \details charger driver implementation.
*  &copy;
*  Copyright (c) 2017-2018 Qualcomm Technologies, Inc.  All Rights Reserved. 
*  Qualcomm Technologies Proprietary and Confidential.
*/

/*===========================================================================

EDIT HISTORY FOR MODULE


when       who     what, where, why
--------   ---    ------------------------------------------------------------- 
06/20/18    cs      Initial revision
=============================================================================== */

/*=============================================================================

					INCLUDE FILES

===============================================================================*/

#include "pm_schg_driver.h"
#include "pm_schg_bob.h"
#include "pm_resource_manager.h"
#include "pm_version.h"
/*===========================================================================
                       MACRO DECLARATIONS
===========================================================================*/

#define ADDRESS(x) (schg_ptr->schg_register->bob_register->base_address + schg_ptr->schg_register->bob_register->x)
#define SLAVE_ID   (schg_ptr->comm_ptr->slave_id)

/*===========================================================================

FUNCTION DEFINITIONS 

===========================================================================*/
pm_err_flag_type pm_schg_bob_is_port_active(uint32 pmic_device, boolean* is_active)
{
  pm_err_flag_type    err_flag    = PM_ERR_FLAG_SUCCESS;
  uint8 arbitor_mask = 0x02;
  uint8 reg_val = 0;
  pm_schg_data_type *schg_ptr  = pm_schg_get_data(pmic_device);
  pm_model_type model_type = PMIC_IS_INVALID;

  if (NULL == schg_ptr)
  {
    err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
  }
  else if (is_active == NULL)
  {
    err_flag = PM_ERR_FLAG_INVALID_PARAMETER;
  }
  else
  {
    model_type = pm_get_pmic_model(pmic_device);
    if (model_type == PMIC_IS_SMB2351)
    {
      pm_register_address_type address = ADDRESS(multi_port_arbiter);
      err_flag = pm_comm_read_byte_array(SLAVE_ID, address, 1, &reg_val, 0);
      if (err_flag == PM_ERR_FLAG_SUCCESS)
          *is_active = (reg_val & arbitor_mask) ? TRUE : FALSE;
    }
    else
    {
      err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }

  }

  return err_flag;
}

