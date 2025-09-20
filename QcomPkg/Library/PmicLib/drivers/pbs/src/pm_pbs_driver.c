/*! \file  pm_pbs_driver.c 
 *  
 *  \brief  This file contains the pmic PBS implementation
 *  \details  This file contains the pm_pbs_ram_image_load & pm_pbs_ram_image_verify API
 *  implementation for SBl1 boot and also implementation for  other helper functions
 *  used in PBS driver.
 *  
 *  &copy; Copyright 2014-2019 QUALCOMM Technologies Incorporated, All Rights Reserved
 */

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This document is created by a code generator, therefore this section will
  not contain comments describing changes made to the module.


when         who     what, where, why
----------   ---     ---------------------------------------------------------- 
01/08/2020   abh     Keeping pm_pbs_ram_image_verify under Auto boot opt flag.
07/31/2016   aab     Removed pbs ram load/verify delay
07/31/2016   aab     Updated pbs ram load/verify delay to 5us
12/04/2015   aab     Updated pm_pbs_ram_image_verify() 
07/16/2014   akm     Comm change Updates
06/02/2014   aab     Updated to use pm_clk_busy_wait() API
12/18/2013   aab     Updated pm_pbs_ram_image_load() and  pm_pbs_ram_image_verify() 
03/20/2013   kt      Moving and renaming the file to pm_pbs_driver.c 
                     from pm_pbs.c and adding comments.
10/19/2012   umr     PBS Core Driver.
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES 

===========================================================================*/
#include "pm_pbs_driver.h"
#include "pm_pbs_info.h"
#include "pm_pbs.h"
#include "pm_utils.h"
#include "pm_log_utils.h"

#define PM_PBS_CORE_VER_SUPPORT_NEW_SEQ 0x0303 //maj version 0x03 , minor version 0x03
#define PM_PCE_ENABLE_DONE_STATUS_POLLING_MAX 10


/*=========================================================================
      Function Definitions
==========================================================================*/
pm_err_flag_type
pm_pbs_get_mem_info(uint32 slave_id, uint16 *pbs_start_addr,
                         uint16 *otp_size, uint16 *pbs_size)
{
  pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
  uint8 otp_size_base = 0, otp_size_mult = 0, pbs_size_base = 0, otp_mask = 0x3;
  uint8 maj_ver = 0, min_ver = 0;

  if(pbs_start_addr == NULL || pbs_size == NULL || otp_size == NULL)
  {
    err_flag = PM_ERR_FLAG_INVALID_PARAMETER;
    return err_flag;
  }

  err_flag |= pm_comm_read_byte(slave_id, PMIO_PBS_CORE_REVISION1_ADDR,
                                &min_ver, 1);

  err_flag |= pm_comm_read_byte(slave_id, PMIO_PBS_CORE_REVISION2_ADDR,
                                &maj_ver, 1);

  err_flag |= pm_comm_read_byte(slave_id, PMIO_PBS_CORE_NVM_CFG1_ADDR,
                                &otp_size_base, 1);

  err_flag |= pm_comm_read_byte(slave_id, PMIO_PBS_CORE_NVM_CFG2_ADDR,
                                &otp_size_mult, 1);

  err_flag |= pm_comm_read_byte(slave_id, PMIO_PBS_CORE_RAM_CFG1_ADDR,
                                &pbs_size_base, 1);

  if (err_flag != PM_ERR_FLAG_SUCCESS)
  {
    return err_flag;
  }

  *otp_size = otp_size_base * (1 << (otp_size_mult & otp_mask)) * 8;

  if(maj_ver >= 3)
  {
    *pbs_start_addr = 4 * (*otp_size);
  }
  else
  {
    return PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
  }

  /*For backward compatibility with 3.0 pbs version keep both ways of pbs size
   *calculations
   */
  if ((pbs_size_base <= 8) ||
      ((maj_ver >= 3) && (min_ver >= 1)) ||
      ( maj_ver >= 4))
  {
    *pbs_size = pbs_size_base * 32;
  }
  else
  {
    *pbs_size = (2*(pbs_size_base - 8.) * 32 * 8);
  }

  return err_flag;
}

/** 
 * @name pm_pbs_ram_image_verify 
 *  
 * @brief Please refer to pm_pbs_driver.h file for info 
 *        regarding this function.
 */
pm_err_flag_type
pm_pbs_ram_image_verify(uint32 slave_id, pm_pbs_ram_data_type *data, unsigned size)
{
  int i;
  pm_err_flag_type err = PM_ERR_FLAG_SUCCESS;
  uint32 read_val, data_val;
  uint8 pmic_index;
  pm_pbs_info_data_type **pm_pbs_temp_data = NULL;
  pm_pbs_info_data_type *pm_pbs_data = NULL;

  /* Retrieve PBS info config data */
  pm_pbs_temp_data = (pm_pbs_info_data_type**)pm_target_information_get_specific_info(PM_PROP_PBS_INFO);
  if(NULL == pm_pbs_temp_data)
  {
    return PM_ERR_FLAG_DATA_VERIFY_ERR;
  }

  pmic_index = (uint8)(slave_id / 2); 
  pm_pbs_data = pm_pbs_temp_data[pmic_index];
  if(NULL == pm_pbs_data)
  {
    return PM_ERR_FLAG_DATA_VERIFY_ERR;
  }

  err |= pm_pbs_get_mem_info(slave_id, &pm_pbs_data->pbs_ram_start_addr,
                &pm_pbs_data->pbs_otp_mem_size, &pm_pbs_data->pbs_ram_mem_size);

  if(err != PM_ERR_FLAG_SUCCESS)
  {
    pm_log_message("pm_pbs_ram_image_verify: err = %d, ram size failed", err);
    return err;
  }

  /* Configuring read access in burst mode to PBS RAM */
  err = pm_pbs_config_access(slave_id, pm_pbs_data->pbs_ram_start_addr, PM_PBS_ACCESS_READ_BURST, PM_PBS_TYPE_RAM);
  if(err != PM_ERR_FLAG_SUCCESS) return err; //Stop on first error

  /* Burst Read PBS sequence and verify */
  for (i = 0; i < size; i++)
  {
    err |= pm_comm_read_byte_array(slave_id, PMIO_PBS_CORE_MEM_INTF_RD_DATA0_ADDR, 4, (uint8*) &read_val, 1);
    if(err != PM_ERR_FLAG_SUCCESS) 
    {
      pm_log_message("word number = %d, read_val = %d, slave_id = %d", i, read_val, slave_id);
      return err; //Stop on first error
    }

    data_val = *((uint32 *)&data[i]);
    if (data_val != read_val)
    {
      if( pm_pbs_data->skip_verify_supported &&
         (i >= pm_pbs_data->skip_verify_start && i <= pm_pbs_data->skip_verify_end) )
      {
        continue;
      }
      else
      {
        pm_log_message("word number = 0x%x, read_val = 0x%x, slave_id = 0x%x, dataval = 0x%x", i, read_val, slave_id, data_val);
        return PM_ERR_FLAG_DATA_VERIFY_ERR;
      }
    }
  }

  return err;
}

/** 
 * @name pm_pbs_ram_image_load 
 *  
 * @brief Please refer to pm_pbs_driver.h file for info 
 *        regarding this function.
 */
pm_err_flag_type
pm_pbs_ram_image_load(uint32 slave_id, pm_pbs_ram_data_type *data, unsigned size)
{
  int i;
  pm_err_flag_type err = PM_ERR_FLAG_SUCCESS;
  uint8 pmic_index;
  pm_pbs_info_data_type **pm_pbs_temp_data = NULL;
  pm_pbs_info_data_type *pm_pbs_data = NULL;

  /* Retrieve PBS info config data */
  pm_pbs_temp_data = (pm_pbs_info_data_type**)pm_target_information_get_specific_info(PM_PROP_PBS_INFO);
  if(NULL == pm_pbs_temp_data)
  {
    return PM_ERR_FLAG_DATA_VERIFY_ERR;
  }

  pmic_index = (uint8)(slave_id / 2); 
  pm_pbs_data = pm_pbs_temp_data[pmic_index];
  if(NULL == pm_pbs_data)
  {
    return PM_ERR_FLAG_DATA_VERIFY_ERR;
  }

  err |= pm_pbs_get_mem_info(slave_id, &pm_pbs_data->pbs_ram_start_addr,
                &pm_pbs_data->pbs_otp_mem_size, &pm_pbs_data->pbs_ram_mem_size);

  if(err != PM_ERR_FLAG_SUCCESS)
  {
    pm_log_message("pm_pbs_ram_image_verify: err = %d, ram size failed", err);
    return err;
  }

  /* Configuring read access in burst mode to PBS RAM */
  err = pm_pbs_config_access(slave_id, pm_pbs_data->pbs_ram_start_addr, PM_PBS_ACCESS_WRITE_BURST, PM_PBS_TYPE_RAM);
  if(err != PM_ERR_FLAG_SUCCESS)
  {
    return err;
  }

  /* Burst Write PBS sequence */
  for (i = 0; i < size; i++)
  {
    err = pm_comm_write_byte_array(slave_id, PMIO_PBS_CORE_MEM_INTF_WR_DATA0_ADDR, 4, (uint8 *)&data[i], 1);
    if(err != PM_ERR_FLAG_SUCCESS)
    {
      return err;
    }
  }

  err = pm_pbs_disable_access(slave_id, FALSE);
  if(err != PM_ERR_FLAG_SUCCESS)
  {
    return err;
  }

#ifndef ENABLE_AUTO_BOOT_OPTIMIZATION
  err = pm_pbs_ram_image_verify(slave_id, data, size);
#endif /* ENABLE_AUTO_BOOT_OPTIMIZATION */

  return err;
}

/** 
 * @name pm_pbs_disable_access 
 *  
 * @brief Please refer to pm_pbs_driver.h file for info 
 *        regarding this function.
 */
pm_err_flag_type 
pm_pbs_disable_access(uint32 slave_id, pm_pbs_type pbs_type) 
{
  pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
  uint8 read_maj_ver = 0;
  uint8 read_min_ver = 0;
  uint32 support_version = 0;

  if(pbs_type >= PM_PBS_TYPE_INVALID)
  {
    return PM_ERR_FLAG_INVALID_PARAMETER;
  }

  if(PM_PBS_TYPE_ROM == pbs_type)
  {
    //check if DIG MAJ is greater than 3 and DIG MIN is greater 3 pbs_core >= 3.3
    err_flag = pm_comm_read_byte(slave_id, PMIO_PBS_CORE_REVISION2_ADDR, &read_maj_ver, 1);
    if (err_flag != PM_ERR_FLAG_SUCCESS)
    {
      return err_flag;
    }
    err_flag = pm_comm_read_byte(slave_id, PMIO_PBS_CORE_REVISION1_ADDR, &read_min_ver, 1);
    if (err_flag != PM_ERR_FLAG_SUCCESS)
    {
      return err_flag;
    }

    support_version = (uint32)read_maj_ver << 8 | (uint32)read_min_ver ; 

    if(support_version >= PM_PBS_CORE_VER_SUPPORT_NEW_SEQ)
    {
      err_flag = pm_comm_write_byte(slave_id, PMIO_PBS_CORE_PMIC5_OTP_CFG1_ADDR, 0x00, 1);
      err_flag |= pm_comm_write_byte(slave_id, PMIO_PBS_CORE_PMIC5_OTP_PCE_FORCE_ON_ADDR, 0x00, 1);
    }
  }

  /* Configuring to disable PBS ROM/RAM access */
  err_flag |= pm_comm_write_byte(slave_id, PMIO_PBS_CORE_MEM_INTF_CFG_ADDR, 0x00, 1);
  err_flag |= pm_comm_write_byte(slave_id, PMIO_PBS_CORE_MEM_INTF_CTL_ADDR, 0x00, 1);

  return err_flag;
}

/** 
 * @name pm_pbs_config_access 
 *  
 * @brief Please refer to pm_pbs_driver.h file for info 
 *        regarding this function.
 */
pm_err_flag_type 
pm_pbs_config_access(uint32 slave_id, uint16 addr, pm_pbs_access_type access_type, pm_pbs_type pbs_type) 
{
  pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
  uint8 access_val = 0;
  uint8 otp_cfg_value = 0x00;
  uint8 otp_force_en_value = 0x01;
  uint8 pce_en_value = 0;
  uint8 read_maj_ver = 0;
  uint8 read_min_ver = 0;
  uint32 support_version = 0;
  uint8 pce_en_read_count = 0;

  if(pbs_type >= PM_PBS_TYPE_INVALID)
  {
    return PM_ERR_FLAG_INVALID_PARAMETER;
  }

  switch (access_type)
  {
    case PM_PBS_ACCESS_READ_SINGLE:
      access_val = 0x00;
      break;

    case PM_PBS_ACCESS_WRITE_SINGLE:
      access_val = 0x40;
      break;

    case PM_PBS_ACCESS_READ_BURST:
      access_val = 0x80;
      break;

    case PM_PBS_ACCESS_WRITE_BURST:
      access_val = 0xC0;
      break;

    default:
      return PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
  }

  /* Configuring to enable PBS ROM/RAM access */
  err_flag = pm_comm_write_byte(slave_id, PMIO_PBS_CORE_MEM_INTF_CFG_ADDR, 0x80, 1);
  if (err_flag != PM_ERR_FLAG_SUCCESS)
  {
    return err_flag;
  }

  /* Configuring type of access to PBS ROM/RAM */
  err_flag |= pm_comm_write_byte(slave_id, PMIO_PBS_CORE_MEM_INTF_CTL_ADDR, access_val, 1);
  if (err_flag != PM_ERR_FLAG_SUCCESS)
  {
    return err_flag;
  }

  if(PM_PBS_TYPE_ROM == pbs_type)
  {
    //check if DIG MAJ is greater than 3 and DIG MIN is greater 3 pbs_core >= 3.3
    err_flag = pm_comm_read_byte(slave_id, PMIO_PBS_CORE_REVISION2_ADDR, &read_maj_ver, 1);
    if (err_flag != PM_ERR_FLAG_SUCCESS)
    {
      return err_flag;
    }
    err_flag = pm_comm_read_byte(slave_id, PMIO_PBS_CORE_REVISION1_ADDR, &read_min_ver, 1);
    if (err_flag != PM_ERR_FLAG_SUCCESS)
    {
      return err_flag;
    }

    support_version = (uint32)read_maj_ver << 8 | (uint32)read_min_ver ; 
    if(support_version >= PM_PBS_CORE_VER_SUPPORT_NEW_SEQ)
    {
      err_flag = pm_comm_write_byte(slave_id, PMIO_PBS_CORE_PMIC5_OTP_CFG1_ADDR, otp_cfg_value, 1);
      if (err_flag != PM_ERR_FLAG_SUCCESS)
      {
        return err_flag;
      }

      err_flag = pm_comm_write_byte(slave_id, PMIO_PBS_CORE_PMIC5_OTP_PCE_FORCE_ON_ADDR, otp_force_en_value, 1);
      if (err_flag != PM_ERR_FLAG_SUCCESS)
      {
        return err_flag;
      }

      // keep reading PCE_ENABLE_DONE to check whether it already been set.
      while(pce_en_read_count++ < PM_PCE_ENABLE_DONE_STATUS_POLLING_MAX)
      {
        err_flag = pm_comm_read_byte_mask(slave_id, PMIO_PBS_CORE_PCE_ENABLE_DONE_STATUS_ADDR, 0x01, &pce_en_value, 1);
        if (err_flag != PM_ERR_FLAG_SUCCESS)
        {
          return err_flag;
        }

        if(0x01 == pce_en_value)
        {
          break;
        }

        err_flag = pm_busy_wait(1);
        if (err_flag != PM_ERR_FLAG_SUCCESS)
        {
          return err_flag;
        }
      }
      if(pce_en_value != 0x01)
      {
        return PM_ERR_FLAG_PERIPHERAL_ERR;
      }
    }
  }

  /* Configuring the base address */
  err_flag |= pm_comm_write_byte_array(slave_id, PMIO_PBS_CORE_MEM_INTF_ADDR_LSB_ADDR, 2, (uint8*)&addr, 1);

  return err_flag;
}
