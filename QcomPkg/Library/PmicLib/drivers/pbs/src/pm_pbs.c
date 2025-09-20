/*! \file  pm_pbs.c 
 *  
 *  \brief  This file contains the pmic PBS driver implementation
 *  \details  This file contains exposed APIs that can be used to configure PBS.
 *  
 *  &copy; Copyright 2014 - 2018 QUALCOMM Technologies Incorporated, All Rights Reserved
 */

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This document is created by a code generator, therefore this section will
  not contain comments describing changes made to the module.


when         who     what, where, why
----------   ---     ---------------------------------------------------------- 
12/18/17     aab     Depricated pm_pbs_add_ram_sequences() 
06/25/15     al      Switching to use DALSYS_BusyWait
07/16/14     akm     Comm change Updates
03/19/13     kt      Added pm_pbs_add_ram_sequence API.
10/19/12     umr     PBS Core Driver.
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES 

===========================================================================*/
#include <DALSys.h>
#include "pm_pbs.h"
#include "pm_utils.h"
#include "pm_pbs_driver.h"
#include "pm_target_information.h"

/*===========================================================================

                        TYPE DEFINITIONS 

===========================================================================*/
/* Overall maximum number of sequences per trigger
   independent of trigger type */
#define PM_PBS_MAX_NUM_SEQS                  35

/* Un-initialized PBS sequence value */
#define PM_PBS_SEQ_UNINITIALIZED             0xFFFFFFFF

/* Trigger start address LSB where x is the trigger id */
#define PMIO_PBS_CORE_TRIG_START_ADDR_LSB(x)   (0x7050 + (0x4 * (x)))

/* Trigger start address MSB where x is the trigger id */
#define PMIO_PBS_CORE_TRIG_START_ADDR_MSB(x)   (0x7051 + (0x4 * (x)))

/*===========================================================================

                        FUNCTION DEFINITIONS 

===========================================================================*/
/** 
 * @name pm_pbs_ram_sequences_verify 
 *  
 * @brief This is an internal helper function. This function 
 *        reads the RAM sequences from PBS RAM in READ BURST
 *        mode starting from the PBS RAM address specified by
 *        the caller and compares it with the expected sequences
 *        sent in by the caller. This function internally calls
 *        pm_pbs_config_access to configure READ BURST mode
 *        access to PBS RAM.
 *  
 * @param[in]  slave_id. Primary PMIC chip: 1. Secondary PMIC Chip: 4
 * @param[in]  ram_addr: PBS RAM address starting from which the
 *             sequences are read.
 * @param[in]  seq_ptr: Pointer to expected PBS RAM sequences to
 *             which the read sequences are compared against.
 * @param[in]  num_seqs: Number of sequences to be verified in 
 *             PBS RAM.
 *
 * @return  pm_err_flag_type
 *          PM_ERR_FLAG_DATA_VERIFY_ERR = if the read
 *          sequences from PBS RAM don't match the sequences
 *          sent in by the caller.
 *          PM_ERR_FLAG_SUCCESS = SUCCESS.
 *          else SPMI ERROR.
 *  
 * @dependencies none
 */
static pm_err_flag_type 
pm_pbs_ram_sequences_verify(uint32 slave_id, uint16 ram_addr, uint32* seq_ptr, uint8 num_seqs) 
{
  pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
  uint32 seq_read = 0;
  uint8 seq_cnt = 0;

  /* Configuring read access in burst mode to PBS RAM */
  err_flag = pm_pbs_config_access(slave_id, ram_addr, PM_PBS_ACCESS_READ_BURST, PM_PBS_TYPE_RAM);

  if (err_flag != PM_ERR_FLAG_SUCCESS)
  {
    return err_flag;
  }

  /* Burst Read loaded PBS RAM sequence and verify */
  for (seq_cnt = 0;seq_cnt < num_seqs;seq_cnt++)
  {

    /* Writing the sequence sent by user to PBS RAM */
    err_flag = pm_comm_read_byte_array(slave_id, PMIO_PBS_CORE_MEM_INTF_RD_DATA0_ADDR, 4, (uint8*)&seq_read, 1);

    DALSYS_BusyWait(10);

    if (seq_ptr[seq_cnt] != seq_read)
    {
      return PM_ERR_FLAG_DATA_VERIFY_ERR;
    }

    if (err_flag != PM_ERR_FLAG_SUCCESS)
    {
      return err_flag;
    }

  }

  return err_flag;
}

/** 
 * @name pm_pbs_ram_sequences_load 
 *  
 * @brief This is an internal helper function. This function 
 *        loads RAM sequences to PBS RAM in WRITE BURST mode
 *        starting from the PBS RAM address specified by the
 *        caller. This function internally calls
 *        pm_pbs_ram_sequences_verify functions to check if the
 *        sequences are loaded correctly to PBS RAM. This
 *        function also internally calls pm_pbs_config_access to
 *        configure WRITE BURST mode access to PBS RAM.
 *  
 * @param[in]  slave_id. Primary PMIC chip: 1. Secondary PMIC Chip: 4
 * @param[in]  ram_addr: PBS RAM address at which the sequences 
 *             are started loading.
 * @param[in]  seq_ptr: Pointer to PBS RAM sequences to be 
 *             loaded.
 * @param[in]  num_seqs: Number of sequences to be added to PBS 
 *             RAM.
 *
 * @return  pm_err_flag_type
 *          PM_ERR_FLAG_DATA_VERIFY_ERR = if the RAM
 *          sequences are not loaded correctly.
 *          PM_ERR_FLAG_SUCCESS = SUCCESS.
 *          else SPMI ERROR.
 *  
 * @dependencies none
 */
static pm_err_flag_type 
pm_pbs_ram_sequences_load(uint32 slave_id, uint16 ram_addr, uint32* seq_ptr, uint8 num_seqs) 
{
  pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
  uint8 seq_cnt = 0;

  /* Configuring write access in burst mode to PBS RAM */
  err_flag = pm_pbs_config_access(slave_id, ram_addr, PM_PBS_ACCESS_WRITE_BURST, PM_PBS_TYPE_RAM);

  if (err_flag != PM_ERR_FLAG_SUCCESS)
  {
    return err_flag;
  }

  for (seq_cnt = 0;seq_cnt < num_seqs;seq_cnt++)
  {

    /* Burst write the sequence sent by user to PBS RAM */
    err_flag = pm_comm_write_byte_array(slave_id, PMIO_PBS_CORE_MEM_INTF_WR_DATA0_ADDR, 4, (uint8*)&seq_ptr[seq_cnt], 1);

    pm_busy_wait(10);

    if (err_flag != PM_ERR_FLAG_SUCCESS)
    {
      return err_flag;
    }

  }

  /* Verify if the sequences are loaded correctly to PBS RAM */
  err_flag = pm_pbs_ram_sequences_verify(slave_id, ram_addr, seq_ptr, num_seqs);

  if (err_flag != PM_ERR_FLAG_SUCCESS)
  {
    return err_flag;
  }

  return err_flag;
}

