/*! \file pm_sdam.c
*  \n
*  \brief This file contains the implementation of the public APIs for SDAM module.
*  \n
*  \details Implementation file for SDAM module. Each of the APIs checks for
*           access and then if necessary directs the call to Driver implementation.
*  \n
*  &copy; Copyright 2018 QUALCOMM Technologies Incorporated, All Rights Reserved
*/
/*===========================================================================
EDIT HISTORY FOR MODULE

This document is created by a code generator, therefore this section will
not contain comments describing changes made to the module.


when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/23/18   rl      Created.
===========================================================================*/
/*===========================================================================

                     INCLUDE FILES

===========================================================================*/
#include "pm_sdam_driver.h"
#include "pm_sdam.h"
#include "CoreVerify.h"
#include "DALSys.h"
#include "pm_log_utils.h"

/*===========================================================================

                         DEFINITIONS

===========================================================================*/
//Max wait time in uS
#define SEQ_MAX_WAIT_TIME 500

//Busy Wait Poll time in uS
#define SEQ_BUSYWAIT_POLL_TIME 5

#define BUSY_INDICATOR 0xFF

#define PM_SDAM_MUTEX_LOCK(handle)   DALSYS_SyncEnter(handle)

#define PM_SDAM_MUTEX_UNLOCK(handle) DALSYS_SyncLeave(handle)

#define REG_SDAM(reg) ((sdam_ptr->sdam_reg_table->base_address)     \
    + ((sdam_index)*(sdam_ptr->sdam_reg_table->peripheral_offset))  \
    +  (sdam_ptr->sdam_reg_table->reg))

#define SLAVE_SDAM sdam_ptr->comm_ptr->slave_id

/*===========================================================================

                        FUNCTION PROTOTYPES

===========================================================================*/

static pm_err_flag_type pm_sdam_seq_done(pm_sdam_data_type *sdam_ptr,
                                         pm_register_address_type addr);

/*===========================================================================

                        FUNCTION DEFINITIONS

===========================================================================*/

pm_err_flag_type
pm_sdam_trig_seq ( uint8 pmic_chip, pm_sdam_type rsrc_index, 
                   uint8 trig_arg, uint8 *trig_ret_val,
                   pm_sdam_trig_type trig_fnf_wait )
{
  pm_err_flag_type    err_flag   = PM_ERR_FLAG_SUCCESS;
  uint8               sdam_index = 0;
  pm_sdam_data_type   *sdam_ptr  = NULL;

  err_flag |= pm_sdam_driver_self_init();
  if(err_flag != PM_ERR_FLAG_SUCCESS)
  {
    return err_flag;
  }

  sdam_ptr  = pm_sdam_get_data(pmic_chip);

  /* Input Error Checks */
  if ((sdam_ptr == NULL) || (trig_ret_val == NULL) ||
      (trig_fnf_wait >= PM_SDAM_TRIG_INVALID) || 
      (rsrc_index > (sdam_ptr->num_of_periph-1)))
  {
    err_flag = (sdam_ptr==NULL) ?
               PM_ERR_FLAG_FEATURE_NOT_SUPPORTED :
               PM_ERR_FLAG_INVALID_PARAMETER;

    //PM_LOG_MSG_ERROR(err_flag, "error = %d", err_flag);
    return err_flag;
  }

  /*Get correct SDAM for EE from config data */
  sdam_index = (uint8)rsrc_index;

  if(sdam_index >= (uint8)PM_SDAM_INVALID)
  {
    err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    //PM_LOG_MSG_ERROR(err_flag, "Invalid Sdam Index error = %d", err_flag);
    return err_flag;
  }

  /* Lock execution for exclusive writes to sdam */
  PM_SDAM_MUTEX_LOCK(sdam_ptr->sdam_dal_sync_handle);

  /* Check if previous sequence completed in case it was FNF
   *  Poll for 500 uS to confirm completion
   */
  err_flag |=pm_sdam_seq_done(sdam_ptr, REG_SDAM(trig_busy_ret));

  if(err_flag != PM_ERR_FLAG_SUCCESS)
  {
      /* Unlock mutex before returning */
      PM_SDAM_MUTEX_UNLOCK(sdam_ptr->sdam_dal_sync_handle);
      //PM_LOG_MSG_ERROR(err_flag, "SDAM Busy error");
      return err_flag;
  }

  /* Trigger new seq */
  err_flag |= pm_comm_write_byte(SLAVE_SDAM, REG_SDAM(trig_arg), trig_arg, 0);

  err_flag |= pm_comm_write_byte(SLAVE_SDAM, REG_SDAM(trig_busy_ret),
                                 (uint8)BUSY_INDICATOR, 0);

  err_flag |= pm_comm_write_byte(SLAVE_SDAM, REG_SDAM(trig_clr), 0x1, 0);

  err_flag |= pm_comm_write_byte(SLAVE_SDAM, REG_SDAM(trig_set), 0x1, 0);


  if(trig_fnf_wait == PM_SDAM_TRIG_WAIT)
  {
    err_flag |= pm_sdam_seq_done(sdam_ptr, REG_SDAM(trig_busy_ret));

    //PM_LOG_MSG_ERROR(err_flag, "Busy returned during wait for trig comp");

    err_flag |= pm_comm_read_byte(SLAVE_SDAM, REG_SDAM(trig_busy_ret),
                                  trig_ret_val, 0);
  }

  /* Unlock Mutex */
  PM_SDAM_MUTEX_UNLOCK(sdam_ptr->sdam_dal_sync_handle);

  //PM_LOG_MSG_ERROR(err_flag, "error = %d", err_flag);

  return err_flag;
}

static pm_err_flag_type
pm_sdam_seq_done( pm_sdam_data_type *sdam_ptr, pm_register_address_type addr )
{
  uint8             data         = 0;
  uint32            wait_time    = 0;
  pm_err_flag_type  err_flag     = PM_ERR_FLAG_SUCCESS;

  do
  {
    err_flag |= pm_comm_read_byte(SLAVE_SDAM, addr, &data, 0);

  }while ((data == BUSY_INDICATOR) &&
          ((wait_time += SEQ_BUSYWAIT_POLL_TIME) <= SEQ_MAX_WAIT_TIME));

  if (data == BUSY_INDICATOR)
  {
    err_flag = PM_ERR_FLAG_DATA_VERIFY_ERR;
  }

  return err_flag;
}

pm_err_flag_type
pm_sdam_mem_write ( uint8 pmic_chip, pm_sdam_type rsrc_index,
                    uint8 start_addr, uint32 num_bytes, uint8 *data_ptr)
{
  pm_err_flag_type    err_flag   = PM_ERR_FLAG_SUCCESS;
  uint8               sdam_index = 0;
  uint32              sdam_mem_size;
  pm_sdam_data_type   *sdam_ptr  = NULL;

  err_flag |= pm_sdam_driver_self_init();
  if(err_flag != PM_ERR_FLAG_SUCCESS)
  {
    return err_flag;
  }

  sdam_ptr  = pm_sdam_get_data(pmic_chip);

  /* Input Check */
  if(sdam_ptr == NULL || data_ptr == NULL ||
     num_bytes == 0 || rsrc_index > (sdam_ptr->num_of_periph-1))
  {
    err_flag |= (sdam_ptr==NULL) ?
                PM_ERR_FLAG_FEATURE_NOT_SUPPORTED :
                PM_ERR_FLAG_INVALID_PARAMETER;

    //PM_LOG_MSG_ERROR(err_flag, "error = %d", err_flag);
    return err_flag;
  }

  sdam_index = (uint8)rsrc_index;

  if(sdam_index >= (uint8)PM_SDAM_INVALID)
  {
    err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    //PM_LOG_MSG_ERROR(err_flag, "Invalid Sdam Index error = %d", err_flag);
    return err_flag;
  }

  sdam_mem_size = sdam_ptr->sdam_mem_size[sdam_index];

  if( ((uint32)start_addr > (sdam_mem_size - 1)) ||
      (num_bytes > sdam_mem_size) ||
      ((num_bytes + (uint32)start_addr) > sdam_mem_size) )
  {
    err_flag |= PM_ERR_FLAG_INVALID_PARAMETER;
    //PM_LOG_MSG_ERROR(err_flag, "addr offset = 0x%x, numbytes = %d",start_addr, num_bytes);
    return err_flag;
  }

  /* Lock before writing to memory to prevent any concurrent overwrites */
  PM_SDAM_MUTEX_LOCK(sdam_ptr->sdam_dal_sync_handle);

  err_flag |= pm_comm_write_byte_array(SLAVE_SDAM,
                                      ((REG_SDAM(mem_start)) + start_addr),
                                      num_bytes, data_ptr, 0);

  PM_SDAM_MUTEX_UNLOCK(sdam_ptr->sdam_dal_sync_handle);

  return err_flag;
}

pm_err_flag_type
pm_sdam_mem_read ( uint8 pmic_chip, pm_sdam_type rsrc_index,
                   uint8 start_addr, uint32 num_bytes, uint8 *data_ptr)
{
  pm_err_flag_type    err_flag   = PM_ERR_FLAG_SUCCESS;
  uint8               sdam_index = 0;
  uint32              sdam_mem_size;
  pm_sdam_data_type   *sdam_ptr  = NULL;

  err_flag |= pm_sdam_driver_self_init();
  if(err_flag != PM_ERR_FLAG_SUCCESS)
  {
    return err_flag;
  }

  sdam_ptr  = pm_sdam_get_data(pmic_chip);

  /* Input Check */
  if(sdam_ptr == NULL || data_ptr == NULL ||
     num_bytes == 0 || rsrc_index > (sdam_ptr->num_of_periph-1))
  {
    err_flag |= (sdam_ptr==NULL) ?
                PM_ERR_FLAG_FEATURE_NOT_SUPPORTED :
                PM_ERR_FLAG_INVALID_PARAMETER;

    //PM_LOG_MSG_ERROR(err_flag, "error = %d", err_flag);
    return err_flag;
  }

  sdam_index = (uint8)rsrc_index;

  if(sdam_index >= (uint8)PM_SDAM_INVALID)
  {
    err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    //PM_LOG_MSG_ERROR(err_flag, "Invalid Sdam Index error = %d", err_flag);
    return err_flag;
  }

  sdam_mem_size = sdam_ptr->sdam_mem_size[sdam_index];

  if( ((uint32)start_addr > (sdam_mem_size - 1)) ||
      (num_bytes > sdam_mem_size) ||
      ((num_bytes + (uint32)start_addr) > sdam_mem_size) )
  {
    err_flag |= PM_ERR_FLAG_INVALID_PARAMETER;
    //PM_LOG_MSG_ERROR(err_flag, "addr offset = 0x%x, numbytes = %d",start_addr, num_bytes);
    return err_flag;
  }

  PM_SDAM_MUTEX_LOCK(sdam_ptr->sdam_dal_sync_handle);

  err_flag |= pm_comm_read_byte_array(SLAVE_SDAM,
                                      ((REG_SDAM(mem_start)) + start_addr),
                                      num_bytes, data_ptr, 0);

  PM_SDAM_MUTEX_UNLOCK(sdam_ptr->sdam_dal_sync_handle);

  return err_flag;
}
