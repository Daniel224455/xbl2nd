/*! \file pm_sdam_driver.c
*  \n
*  \brief SDAM driver initialization.
*  \n
*  \n &copy; Copyright 2018 QUALCOMM Technologies Incorporated, All Rights Reserved
*/
/* =======================================================================
                            Edit History
This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.


when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/24/18   rl      Created
========================================================================== */
/*===========================================================================

                     INCLUDE FILES

===========================================================================*/
#include "pm_sdam_driver.h"
#include "CoreVerify.h"
#include "pm_log_utils.h"
#include "device_info.h"
#include "pm_target_information.h"
#include "pm_utils.h"
#include "hw_module_type.h"
#include "pm_device.h"
/*===========================================================================

                         DEFINITIONS

===========================================================================*/

#define REG_SDAM(reg) ((sdam_ptr->sdam_reg_table->base_address)     \
    + ((sdam_index)*(sdam_ptr->sdam_reg_table->peripheral_offset))  \
    +  (sdam_ptr->sdam_reg_table->reg))

#define SLAVE_SDAM sdam_ptr->comm_ptr->slave_id

#define SDAM_SIZE_MULTIPLIER 32

/*===========================================================================

                        STATIC VARIABLES

===========================================================================*/

/* Static global variable to store the sdam data */
static pm_sdam_data_type *pm_sdam_data_arr[PM_MAX_NUM_PMICS];

static boolean sdam_initialized = FALSE;

static pm_sdam_reg_info_type sdam_reg =
{
  .base_address      = 0xB000,
  .peripheral_offset = 0x100,
  .mem_start         = 0x0040,
  .trig_arg          = 0x0042,
  .trig_busy_ret     = 0x0043,
  .sdam_size         = 0x0044,
  .trig_set          = 0x00E5,
  .trig_clr          = 0x00E6,
};



/*===========================================================================

                        FUNCTION DEFINITIONS

===========================================================================*/
void
pm_sdam_driver_init(pm_comm_info_type *comm_ptr,
                    peripheral_info_type *peripheral_info,
                    uint8 pmic_index)
{
  pm_sdam_data_type         *sdam_ptr       = NULL;
  //pm_sdam_supported_periph  *sdam_supported = NULL;
  pm_err_flag_type          err_flag        = PM_ERR_FLAG__SUCCESS;
  uint8                     sdam_index      = 0;
  static DALSYSSyncHandle   sync_handle_copy;

  if(sdam_initialized == TRUE)
  {
    return;
  }
  if(comm_ptr == NULL || peripheral_info == NULL)
  {
    CORE_VERIFY_PTR(0);
  }

  sdam_ptr = pm_sdam_data_arr[pmic_index];

  sdam_index = (peripheral_info->base_address & (~sdam_reg.base_address)) >> 8;

  if(sdam_ptr == NULL)
  {
    pm_malloc( sizeof(pm_sdam_data_type), (void**)&(sdam_ptr));

    /* Assign Comm ptr */
    sdam_ptr->comm_ptr = comm_ptr;
  
    /* SDAM Register Info */
    sdam_ptr->sdam_reg_table = &sdam_reg;
  
    if(sdam_ptr->sdam_reg_table == NULL)
    {
      //PM_LOG_MSG_ERR_FATAL("SDAM Reg Table not found");
      CORE_VERIFY_PTR(0);
    }
  
    sdam_ptr->num_of_periph = pm_target_information_get_periph_count_info(PM_PROP_SDAM_NUM, pmic_index);
  
    /* Num of peripherals cannot be 0 if this driver init gets called */
    if(sdam_ptr->num_of_periph == 0)
    {
      //PM_LOG_MSG_ERR_FATAL("0 peripherals returned in config, %d", pmic_index);
      CORE_VERIFY_PTR(0);
    }
  
    /* Allocate memory to store sdam size */
    pm_malloc( (sizeof(uint32) * sdam_ptr->num_of_periph),
              (void**)&(sdam_ptr->sdam_mem_size) );
  
    /* Get Supported SDAMS on this EE */
    //sdam_supported = (pm_sdam_supported_periph*)pm_target_information_get_specific_info(PM_PROP_SDAM_SUPPORTED);
  
    //if(sdam_supported == NULL)
    //{
      //PM_LOG_MSG_ERR_FATAL("Supported SDAMs not found in config");
      //CORE_VERIFY_PTR(0);
    //}
  
    //sdam_ptr->sdam_supported_periph = &sdam_supported[pmic_index];
  
    if(sync_handle_copy == NULL)
    {
      if( DAL_SUCCESS !=
          DALSYS_SyncCreate(DALSYS_SYNC_ATTR_RESOURCE,
          &(sdam_ptr->sdam_dal_sync_handle), NULL) )
      {
        //PM_LOG_MSG_ERR_FATAL("Unable to obtain mutex for sdam");
        CORE_VERIFY_PTR(0);
      }
  
      sync_handle_copy = sdam_ptr->sdam_dal_sync_handle;
    }
    else
    {
      sdam_ptr->sdam_dal_sync_handle = sync_handle_copy;
    }
  
    /* Get size of sdam memory */
    err_flag = pm_comm_read_byte(SLAVE_SDAM, REG_SDAM(sdam_size),
                            (uint8*)(&(sdam_ptr->sdam_mem_size[sdam_index])), 0);
  
    sdam_ptr->sdam_mem_size[sdam_index] *= SDAM_SIZE_MULTIPLIER;
  
    pm_sdam_data_arr[pmic_index] = sdam_ptr;
  }
  else
  {
    /* If already initialized for this pmic do not reinitialize but get the size
    * size of memory associated with peripheral and store the information
    */
    /* Get size of sdam memory */
    if(sdam_ptr->sdam_mem_size !=NULL && sdam_index <= sdam_ptr->num_of_periph)
    {
      err_flag = pm_comm_read_byte(SLAVE_SDAM, REG_SDAM(sdam_size),
                          (uint8*)(&(sdam_ptr->sdam_mem_size[sdam_index])), 0);
    }
    else
    {
      //PM_LOG_MSG_ERR_FATAL("Sdam Index > Num of SDAMs or mem size ptr NULL");
      CORE_VERIFY_PTR(0);
    }
    sdam_ptr->sdam_mem_size[sdam_index] *= SDAM_SIZE_MULTIPLIER;
  }

  if(sdam_ptr->sdam_mem_size[sdam_index] == 0)
  { //If sdam size is zero it means psi did not initialize the SDAM.
    CORE_VERIFY(0);
  }

  return;
}

pm_err_flag_type
pm_sdam_driver_self_init(void)
{
  uint32                slave_id        = 0;
  uint32                num_of_sdam     = 0;
  peripheral_info_type  peripheral_info;
  pm_comm_info_type*    comm_info       = NULL;
  pm_err_flag_type      err_flag        = PM_ERR_FLAG_SUCCESS;
  pm_err_flag_type      err_sid         = PM_ERR_FLAG_SUCCESS;
  boolean               target_init     = FALSE;
  boolean               comm_init       = FALSE;

  if(sdam_initialized == TRUE)
  {
    return err_flag;
  }
  err_flag |= pm_device_setup();

  if(err_flag != PM_ERR_FLAG_SUCCESS)
  {
    CORE_VERIFY(0);
  }

  if (!( (target_init = pm_target_information_initialized()) &&
         (comm_init = pm_comm_initialized()) ))
    return PM_ERR_FLAG_FAILURE;

  for(uint8 i = 0; i < PM_MAX_NUM_PMICS; i++)
  {
    num_of_sdam = 0;

    err_sid = pm_get_slave_id(i, 0, &slave_id);
    if(err_sid != PM_ERR_FLAG_SUCCESS)
      continue;

    num_of_sdam = 
    pm_target_information_get_periph_count_info(PM_PROP_SDAM_NUM, i);

    //Since poipu has an smb configuration as well check for smb and make sdam as 0
    if(pm_get_pmic_model(i) == PMIC_IS_SMB2351)
    {
      num_of_sdam = 0;
    }

    if(num_of_sdam == 0)
      continue;

    for(uint8 j = 0; j < num_of_sdam; j++)
    {
      memset(&peripheral_info, 0, sizeof(peripheral_info_type));

      comm_info = pm_comm_get_comm_info(i, 0);
      if(comm_info == NULL)
      {
        CORE_VERIFY(0);
      }

      peripheral_info.base_address = (sdam_reg.base_address) + 
                                     (j * sdam_reg.peripheral_offset);

      err_flag = pm_target_information_read_peripheral_rev(comm_info, 
                                                           &peripheral_info);
      if(err_flag != PM_ERR_FLAG_SUCCESS)
      { //Some data is incorrect resulting in failure of above call
        CORE_VERIFY(0);
      }
      if((pm_hw_module_type)peripheral_info.peripheral_type!=PM_HW_MODULE_SDAM)
      { //This has to be an SDAM peripheral otherwise there is a data flaw.
        CORE_VERIFY(0);
      }
      else
      {
        pm_sdam_driver_init(comm_info, &peripheral_info, i);
      }
    }
  }

  sdam_initialized = TRUE;

  return err_flag;
}

pm_sdam_data_type*
pm_sdam_get_data(uint8 pmic_index)
{
  if(pmic_index < PM_MAX_NUM_PMICS)
  {
    return pm_sdam_data_arr[pmic_index];
  }
  return NULL;
}


uint8
pm_sdam_get_num_periph(uint8 pmic_index)
{
  if( (pmic_index < PM_MAX_NUM_PMICS) &&
      (pm_sdam_data_arr[pmic_index] != NULL))
  {
      return pm_sdam_data_arr[pmic_index]->num_of_periph;
  }

  return 0;
}

