/**********************************************************************
 * spionor_utils.c
 *
 * Implementation of spinor driver support functions
 *
 * Copyright (c) 2015-2018
 * Qualcomm Technologies Incorporated.
 * All Rights Reserved.
 * Qualcomm Confidential and Proprietary
 *
 **********************************************************************/
/*=======================================================================
                        Edit History


when        who   what, where, why
----------  ---   --------------------------------------------------
2018-07-31  wek   Remove access control from read.
2018-07-16  wek   Add block access control.
2017-05-25  md    Initial Release


======================================================================*/
#include "spinor_utils.h"
#include "spinor_init_config.h"
#include "spinor_osal.h"
#include <api/systemdrivers/busywait.h>
#include "quadspi.h"
#include "spinor_core.h"
#include "spinor_spial.h"
#include "spinor_access.h"

SPINOR_STATUS spinor_io_access_validation (uint32 start_block, uint32 block_count)
{
  spinor_partition_info *info = NULL;
  uint32 count = 0;
  SPINOR_STATUS status = SPINOR_ACCESS_DENIED;
  info = spinor_devcfg_protected_regions(&count);
  if (info != NULL)
  {
    uint32 i;
    int restrict_count = 0;
    for (i = 0; i < count; i++)
    {
      uint32 restrict_start = info[i].start_sector;
      uint32 restrict_size = info[i].num_sectors;
      uint32 restrict_end = restrict_start + restrict_size - 1;
      uint32 end_block = start_block + block_count - 1;

      if (restrict_size == 0)
        continue;

      if (INT_OVERFLOW (start_block, block_count) ||
          INT_OVERFLOW (restrict_start, restrict_size))
      {
        status = SPINOR_DEVICE_INVALID_PARAMETER;
        restrict_count++;
      }
      else if ((restrict_start <= start_block && start_block <= restrict_end) ||
               (restrict_start <= end_block && end_block <= restrict_end) ||
               (start_block <= restrict_start && restrict_end <= end_block))
      {
        restrict_count++;
        status = SPINOR_ACCESS_DENIED;
      }
    }
    if (restrict_count == 0)
      status = SPINOR_DEVICE_DONE;
  }

  if (status != SPINOR_DEVICE_DONE)
    spinor_log (&spinor_dev.debug_log, 0x360);

  return status;
}

SPINOR_STATUS spinor_io_byte_access_validation (uint32 byte_offset, uint32 byte_count)
{
  uint32 start_block, block_count;
  start_block = byte_offset / spinor_dev.config_data->block_size_in_bytes;
  block_count = ((byte_count - 1) / spinor_dev.config_data->block_size_in_bytes) + 1;
  return spinor_io_access_validation (start_block, block_count);
}

/**********************************************************
 * Logs error codes. Circular log buffer. The err_index can be used
 * as the index to the most recent error code
 *
 * @param spinor_err_log [IN]
 *   log buffer
 *
 * @param code [IN]
 *   Error code
 *
 **********************************************************/
void spinor_log (spinor_logs *spinor_err_log, uint16 code)
{
  if (spinor_err_log->spinor_err_index >= SPINOR_ERR_LOG_SIZE)
  {
    spinor_err_log->spinor_err_index = 0;
  }
  spinor_err_log->spinor_err_codes[spinor_err_log->spinor_err_index++] = code;
  return;
}

/**********************************************************
 * Flash delay in uint of micro seconds
 *
 * @param usec_delay [IN]
 *   Number of micro seconds delay.
 *
 **********************************************************/
void spinor_udelay (uint32 usec_delay)
{
  busywait(usec_delay);
}

/**********************************************************
 * Sends SPI command to read register
 *
 * @reg_opcode [IN]
 *   Register Opcode
 *
 * @len [IN]
 *   How many bytes to read
 *
 * @reg_value [OUT]
 *   Register content returned
 *
 * @return int [OUT]
 *   Result of the operation.
 *
 **********************************************************/
SPINOR_CORE_STATUS read_spi (uint8 reg_opcode, uint8 len, uint8 *reg_value)
{
  SPINOR_CORE_STATUS status = SPINOR_CORE_DONE;
  SPI_COMMAND_TYPE command_mode = qspi_register_read;

  /* Command | data addr | data buffer | length | dma enable */
  if (SPINOR_CORE_DONE != spial_transfer(command_mode,0,reg_value, len, reg_opcode))
  {
    status = SPINOR_CORE_FAIL;
    spinor_log (&spinor_dev.debug_log, 0x350);
    goto read_spi_end;
  }
read_spi_end:
  return status;
}

/**********************************************************
 * Sends SPI command to write register
 *
 * @reg_addr [IN]
 *   Register Opcode
 *
 * @len [IN]
 *   How many bytes to write
 *
 * @reg_value [OUT]
 *   Register Value
 *
 * @return int [OUT]
 *   Result of the operation.
 *
 **********************************************************/
SPINOR_CORE_STATUS write_spi (uint8 reg_opcode, uint8 len, uint8 *reg_value)
{
  SPINOR_CORE_STATUS status = SPINOR_CORE_DONE;
  uint8 qspi_status = 0;
  SPI_COMMAND_TYPE command_mode = qspi_register_write;

  status = write_enable (&qspi_status);
  if (status != SPINOR_CORE_DONE)
  {
    spinor_dev.debug_log.spinor_status = qspi_status;
    spinor_log (&spinor_dev.debug_log, 0x300);
    goto write_spi_end;
  }

  /* Command | data addr | data buffer | length | dma enable */
  if (SPINOR_CORE_DONE != spial_transfer(command_mode,0,reg_value, len, reg_opcode))
  {
    status = SPINOR_CORE_FAIL;
    spinor_log (&spinor_dev.debug_log, 0x301);
    goto write_spi_end;
  }

write_spi_end:
  return status;
}

/**********************************************************
 * Look up RAM table to find the spinor config info.
 * Save the data in spinor's RAM AON
 *
 * @param spinor_config_tbl [IN]
 *   RAM table containing config data for supported spinor devices
 *
 * @return int [OUT]
 *   Result of the operation.
 *
 **********************************************************/
SPINOR_STATUS spinor_device_lookup (void *spinor_config_tbl)
{
  SPINOR_STATUS status = SPINOR_DEVICE_FAIL;
  void *pdevice_id = NULL;
  spinor_init_config_table_type *spinor_cfg_table;
  spinor_config_data *spinor_config_entry = NULL;
  uint32 total_entries = 0;

  spinor_cfg_table = (spinor_init_config_table_type *)spinor_config_tbl;
  if ((spinor_cfg_table == NULL) || (spinor_cfg_table->magic_no != SPINOR_INIT_TABLE_MAGIC_NUMBER))
  {
    spinor_log (&spinor_dev.debug_log, 0x310);
    status = SPINOR_DEVICE_INVALID_PARAMETER;
    goto spinor_device_lookup_end;
  }

  pdevice_id = (void *)&spinor_dev.config_data->device_id;

  memset(pdevice_id, 0, sizeof(spinor_dev.config_data->device_id));

  /* Read the spinor Device ID */
  (void)spinor_core_read_device_id(pdevice_id, DEVICE_ID_LEN_IN_BYTES);

  total_entries = spinor_cfg_table->total_entries;
  spinor_config_entry = spinor_cfg_table->spinor_config_entries;

  while (total_entries--)
  {
    if (spinor_config_entry->device_id == spinor_dev.config_data->device_id)
    {
      memscpy(spinor_dev.config_data, sizeof(spinor_config_data),
              spinor_config_entry, sizeof(spinor_config_data));
      status = SPINOR_DEVICE_DONE;
      goto spinor_device_lookup_end;
    }
    spinor_config_entry++;
  }

spinor_device_lookup_end:
  return status;
}

/**********************************************************
 * Validates the passed in client's handle
 *
 * @param parti_handle [IN]
 *   SPINOR interface handle to the partition
 *
 * @return int [OUT]
 *   Result of the operation.
 *
 **********************************************************/
SPINOR_STATUS spinor_handle_validation(uint8 *parti_handle)
{
  if ((parti_handle == NULL) || (*parti_handle == 0))
  {
    spinor_log (&spinor_dev.debug_log, 0x320);
    return SPINOR_DEVICE_FAIL;
  }
  return SPINOR_DEVICE_DONE;
}

/**********************************************************
 * Validates the input parameters for spinor Erase
 *
 * @param parti_handle [IN]
 *   SPINOR interface handle
 *
 * @param start_block [IN]
 *   Block offset from the start of the partition/image.
 *
 * @param block_count [IN]
 *   Number of blocks to perform the erase operations.
 *
 * @return int [OUT]
 *   Result of the operation.
 *
 **********************************************************/
SPINOR_STATUS spinor_erase_parameters_validation (uint8 *parti_handle,
                                                  uint32 start_block, uint32 block_count)
{
  SPINOR_STATUS status = SPINOR_DEVICE_FAIL;
  uint32 total_blocks = 0;

  status = spinor_handle_validation(parti_handle);
  if (status != SPINOR_DEVICE_DONE)
  {
    status = SPINOR_DEVICE_INVALID_PARAMETER;
    spinor_log (&spinor_dev.debug_log, 0x330);
    goto spinor_erase_parameters_validation_end;
  }

  if (block_count == 0)
  {
    status = SPINOR_DEVICE_INVALID_PARAMETER;
    spinor_log (&spinor_dev.debug_log, 0x331);
    goto spinor_erase_parameters_validation_end;
  }

  total_blocks = spinor_dev.config_data->density_in_blocks;

  /* Boundary Check */
  if (INT_OVERFLOW (start_block, block_count) ||
      (start_block + block_count) > total_blocks)
  {
    status = SPINOR_DEVICE_INVALID_PARAMETER;
    spinor_log (&spinor_dev.debug_log, 0x332);
    goto spinor_erase_parameters_validation_end;
  }

spinor_erase_parameters_validation_end:
  return status;
}

/**********************************************************
 * Validates the Read/Write input parameters
 *
 * @param parti_handle [IN]
 *   SPINOR interface handle
 *
 * @param byte_offset [IN]
 *   Byte offset from the start of the partition/image.
 *
 * @param byte_count [IN]
 *   Number of bytes to perform the read/write operations.
 *
 * @buffer [IN]
 *   Data buffer for the read/write operation.
 *
 * @return int [OUT]
 *   Result of the operation.
 *
 **********************************************************/
SPINOR_STATUS spinor_rw_parameters_validation (uint8 *parti_handle,
               uint32 byte_offset, uint32 byte_count, void *buffer)
{
  SPINOR_STATUS status = SPINOR_DEVICE_FAIL;

  status = spinor_handle_validation(parti_handle);
  if (status != SPINOR_DEVICE_DONE)
  {
    status = SPINOR_DEVICE_INVALID_PARAMETER;
    spinor_log (&spinor_dev.debug_log, 0x340);
    goto spinor_rw_parameters_validation_end;
  }

  if ((buffer == NULL) || (byte_count == 0))
  {
    status = SPINOR_DEVICE_INVALID_PARAMETER;
    spinor_log (&spinor_dev.debug_log, 0x341);
    goto spinor_rw_parameters_validation_end;
  }

  /* Overflow and Boundary Checks */
  if (INT_OVERFLOW (byte_offset, byte_count) ||
      ((byte_offset + byte_count) > (spinor_dev.config_data->density_in_blocks * spinor_dev.config_data->block_size_in_bytes)))
  {
    status = SPINOR_DEVICE_INVALID_PARAMETER;
    spinor_log (&spinor_dev.debug_log, 0x342);
    goto spinor_rw_parameters_validation_end;
  }

  if (spinor_dev.config_data->block_size_in_bytes == 0)
  {
    spinor_log (&spinor_dev.debug_log, 0x343);
    goto spinor_rw_parameters_validation_end;
  }

spinor_rw_parameters_validation_end:
  return status;
}
