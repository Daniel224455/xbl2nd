/**********************************************************************
 * spinor_api.c
 *
 * Implementation of spinor driver support functions
 *
 * Copyright (c) 2016-2018
 * Qualcomm Technologies Incorporated.
 * All Rights Reserved.
 * Qualcomm Confidential and Proprietary
 *
 **********************************************************************/
/*=======================================================================
                        Edit History


when        who   what, where, why
----------  ---   ---------------------------------------------------
2018-07-31  wek   Remove access control from read.
2017-05-25  md    Initial Release

======================================================================*/

#include "spinor_api.h"
#include "spinor_core.h"
#include "spinor_utils.h"
#include "spinor_osal.h"
#include "spinor_init_config.h"

spinor_drv_info spinor_dev;


/**********************************************************
 * Returns the device specific info.
 * \n
 *
 * @param handle [IN]
 *   SPINOR interface handle
 *
 * @param spinor_info [OUT]
 *   Pointer to the client's spinor device info data.
 *
 * @return int [OUT]
 *   Result of the operation.
 *
 ***********************************************************/
SPINOR_STATUS spinor_get_info(spinor_handle_t handle,
                              struct spinor_info *spinor_info)
{
  SPINOR_STATUS status = SPINOR_DEVICE_FAIL;

  if (spinor_dev.nor_init_done == FALSE)
  {
    spinor_log (&spinor_dev.debug_log, 0x100);
    status = SPINOR_DEVICE_FAIL;
    goto spinor_get_info_end;
  }

  memset(spinor_info, 0, sizeof( struct spinor_info));

  spinor_info->device_id = spinor_dev.config_data->device_id;
  spinor_info->block_count = spinor_dev.config_data->density_in_blocks;
  spinor_info->pages_per_block = spinor_dev.config_data->pages_per_block;
  spinor_info->page_size_bytes = spinor_dev.config_data->page_size_in_bytes;

  status = SPINOR_DEVICE_DONE;

spinor_get_info_end:
  return status;
}

/**********************************************************
 * Opens handle to a access to the spinor device
 *
 *
 * @param handle [IN/OUT]
 *   Pointer to SPINOR interface handle as an input parameter.
 *   Pointer to internal spinor_clients refernce count upon return
 *
 * @return int [OUT]
 *   Result of the operation.
 *
 **********************************************************/
SPINOR_STATUS spinor_open (spinor_handle_t *handle)
{
  SPINOR_STATUS status = SPINOR_DEVICE_FAIL;
  spinor_init_config_table_type *spinor_config;

  if (spinor_dev.nor_init_done == FALSE)
  {
    spinor_config = spinor_get_config_table_struct();
    status = spinor_init((void *)spinor_config);
    spinor_dev.ref_count = 1;
  }

  if ((spinor_dev.nor_init_done == FALSE) || (handle == NULL))
  {
    spinor_log (&spinor_dev.debug_log, 0x110);
    goto spinor_open_end;
  }

  if (spinor_dev.ref_count >= 1)
  {
    /* The partition has already been opened */
    /* Keep track of how many clients have opened the partition */
    /* The partition can only be closed when no client uses it */
    spinor_dev.ref_count++;

  }

  *handle = (spinor_handle_t)&spinor_dev.ref_count;
  status = SPINOR_DEVICE_DONE;

spinor_open_end:
  return status;
}

/**********************************************************
 * Closes handle to a spinor device.
 * Clients should close the handle when accesses to
 * the device is no longer needed.
 *
 * @param handle [IN]
 *   SPINOR interface handle.
 *
 * @return int [OUT]
 *   Result of the operation.
 *
 **********************************************************/
SPINOR_STATUS spinor_close (spinor_handle_t handle)
{
  SPINOR_STATUS status = SPINOR_DEVICE_FAIL;
  uint8 *parti_handle = NULL;
  uint8 counter;

  if (spinor_dev.nor_init_done == FALSE)
  {
    spinor_log (&spinor_dev.debug_log, 0x120);
    goto spinor_close_end;
  }

  parti_handle = (uint8 *)handle;

  status = spinor_handle_validation (parti_handle);
  if (status != SPINOR_DEVICE_DONE)
  {
    status = SPINOR_DEVICE_INVALID_PARAMETER;
    spinor_log (&spinor_dev.debug_log, 0x121);
    goto spinor_close_end;
  }

  if(*parti_handle > 0)
  {
    counter = *parti_handle;
    counter--;
    *parti_handle = counter;
  }

  status = SPINOR_DEVICE_DONE;

spinor_close_end:
  return status;
}

/**********************************************************
 * Allows read data from the spinor
 *
 * @param handle [IN]
 *   SPINOR interface handle
 *
 * @param byte_offset [IN]
 *   Byte offset from the start of region to read from.
 *
 * @param byte_count [IN]
 *   Number of bytes to read data.
 *
 * @buffer [OUT]
 *   Data buffer for a spinor read operation.
 *
 * @return int [OUT]
 *   Result of the operation.
 **********************************************************/
SPINOR_STATUS spinor_read(spinor_handle_t handle, uint32 byte_offset,
                          uint32 byte_count, void *buffer)
{
  SPINOR_STATUS status = SPINOR_DEVICE_FAIL;
  SPINOR_CORE_STATUS nor_status = SPINOR_CORE_FAIL;
  uint8 *parti_handle = NULL;

  if (spinor_dev.nor_init_done == FALSE)
  {
    spinor_log (&spinor_dev.debug_log, 0x130);
    goto spinor_read_end;
  }

  parti_handle = (uint8 *)handle;

  /* validate passed in paramerters */
  status = spinor_rw_parameters_validation (parti_handle,
                                            byte_offset, byte_count, buffer);
  if (status != SPINOR_DEVICE_DONE)
  {
    spinor_log (&spinor_dev.debug_log, 0x131);
    goto spinor_read_end;
  }

  nor_status = spinor_core_read(byte_offset, byte_count, (uint8 *)buffer);

  if (nor_status != SPINOR_CORE_DONE)
  {
    spinor_log (&spinor_dev.debug_log, 0x132);
    status = SPINOR_DEVICE_FAIL;
  }
  else
  {
    status = SPINOR_DEVICE_DONE;
  }

spinor_read_end:
  return status;
}

/**********************************************************
 * Writes data to the spinor.
 *
 * @param handle [IN]
 *   SPINOR interface handle
 *
 * @param byte_offset [IN]
 *   Byte offset from the start of the region to write to.
 *
 * @param byte_count [IN]
 *   Number of bytes to write data.
 *
 * @buffer [IN]
 *   Data buffer for spinor write operation.
 *
 * @return int [OUT]
 *   Result of the operation.
 *
 **********************************************************/
SPINOR_STATUS spinor_write (spinor_handle_t handle, uint32 byte_offset,
                            uint32 byte_count, void *buffer)
{
  SPINOR_STATUS status = SPINOR_DEVICE_FAIL;
  SPINOR_CORE_STATUS nor_status = SPINOR_CORE_FAIL;
  uint8 *parti_handle = NULL;
  uint32 transfer_start_addr = 0;
  uint32 parti_start_block = 0;

  if (spinor_dev.nor_init_done == FALSE)
  {
    spinor_log (&spinor_dev.debug_log, 0x140);
    goto spinor_write_end;
  }

  parti_handle =(uint8*) handle;

  /* validate passed in paramerters */
  status = spinor_rw_parameters_validation (parti_handle, byte_offset,
                                            byte_count, buffer);
  if (status != SPINOR_DEVICE_DONE)
  {
    spinor_log (&spinor_dev.debug_log, 0x141);
    goto spinor_write_end;
  }

  status = spinor_io_byte_access_validation (byte_offset, byte_count);
  if (status != SPINOR_DEVICE_DONE)
  {
    spinor_log (&spinor_dev.debug_log, 0x143);
    goto spinor_write_end;
  }

  transfer_start_addr = parti_start_block + byte_offset;

  nor_status = spinor_core_write (transfer_start_addr,
                                  byte_count, (uint8 *)buffer);
  if (nor_status != SPINOR_CORE_DONE)
  {
    spinor_log (&spinor_dev.debug_log, 0x142);
    status = SPINOR_DEVICE_FAIL;
  }
  else
  {
    status = SPINOR_DEVICE_DONE;
  }

spinor_write_end:
  return status;
}


/**********************************************************
 * Erases given spinor blocks
 *
 * @param handle [IN]
 *   SPINOR interface handle
 *
 * @param start_block [IN]
 *   A starting block for a number of blocks to erase.
 *
 * @param block_count [IN]
 *   Number of blocks to erase
 *
 * @return int [OUT]
 *   Result of the operation.
 *
 **********************************************************/
SPINOR_STATUS spinor_erase (spinor_handle_t handle,
                            uint32 start_block, uint32 block_count)
{
  SPINOR_STATUS status = SPINOR_DEVICE_FAIL;
  SPINOR_CORE_STATUS nor_status = SPINOR_CORE_FAIL;
  uint8 *parti_handle = NULL;

  if (spinor_dev.nor_init_done == FALSE)
  {
    spinor_log (&spinor_dev.debug_log, 0x150);
    goto spinor_erase_end;
  }

  parti_handle = (uint8 *)handle;

  status = spinor_erase_parameters_validation(parti_handle,
                                              start_block, block_count);
  if (status != SPINOR_DEVICE_DONE)
  {
    spinor_log (&spinor_dev.debug_log, 0x151);
    goto spinor_erase_end;
  }
  status = spinor_io_access_validation (start_block, block_count);
  if (status != SPINOR_DEVICE_DONE)
  {
    spinor_log (&spinor_dev.debug_log, 0x153);
    goto spinor_erase_end;
  }

  nor_status = spinor_core_erase(start_block, block_count);
  if (nor_status != SPINOR_CORE_DONE)
  {
    spinor_log (&spinor_dev.debug_log, 0x152);
    status = SPINOR_DEVICE_FAIL;
  }
  else
  {
    status = SPINOR_DEVICE_DONE;
  }

spinor_erase_end:
  return status;
}

/**********************************************************
 * Erases given spinor blocks of specified block size.
 *
 * If the start_block and block_count align to the bulk erase size,
 * the entire transfer is done in bulk erase size (32KB, 64KB, 512KB, etc).
 * If the start_block and block_count do not align, this function
 * breaks the transfer into up to 3 transfers where the small
 * unaligned blocks at the beginning and the end will use 4KB erase, and
 * use bulk erase for the aligned blocks in the middle.
 *
 * @param handle [IN]
 *   SPINOR interface handle
 *
 * @param start_block [IN]
 *   A starting block for a number of blocks to erase
 *
 * @param block_count [IN]
 *   Number of blocks to erase
 *
 * @return int [OUT]
 *   Result of the operation.
 *
 **********************************************************/
SPINOR_STATUS spinor_erase_bulk (spinor_handle_t handle,
                                 uint32 start_block, uint32 block_count)
{
  SPINOR_STATUS status = SPINOR_DEVICE_FAIL;
  SPINOR_CORE_STATUS nor_status = SPINOR_CORE_FAIL;
  uint32 transfer_start_block = 0;
  uint32 alignment_padding_blocks = 0;
  uint32 transfer_size = 0;
  uint32 max_transfer_blocks = 0;
  uint8 *parti_handle = NULL;
  uint32 parti_start_block = 0;


  if (spinor_dev.nor_init_done == FALSE)
  {
    spinor_log (&spinor_dev.debug_log, 0x160);
    goto spinor_erase_bulk_end;
  }

  parti_handle = (uint8 *)handle;
  status = spinor_erase_parameters_validation(parti_handle,
                                              start_block, block_count);
  if (status != SPINOR_DEVICE_DONE)
  {
    nor_status = SPINOR_CORE_INVALID_PARAMETER;
    spinor_log (&spinor_dev.debug_log, 0x161);
    goto spinor_erase_bulk_end;
  }
  status = spinor_io_access_validation (start_block, block_count);
  if (status != SPINOR_DEVICE_DONE)
  {
    nor_status = status;
    spinor_log (&spinor_dev.debug_log, 0x166);
    goto spinor_erase_bulk_end;
  }

  transfer_start_block = parti_start_block + start_block;

  max_transfer_blocks = spinor_dev.config_data->bulk_erase_size_in_4KB;

  /* use 4KB erase if bulk erase is not supported */
  if ((spinor_dev.config_data->bulk_erase_opcode == 0) ||
      (spinor_dev.config_data->bulk_erase_size_in_4KB == 0))
  {
    nor_status = spinor_core_erase(start_block, block_count);
    if (nor_status != SPINOR_CORE_DONE)
    {
      spinor_log (&spinor_dev.debug_log, 0x162);
    }

    goto spinor_erase_bulk_end;
  }

  /* bulk erase is supported */
  /* physical starting block alignment check */
  if (transfer_start_block % max_transfer_blocks)
  {
    alignment_padding_blocks = (max_transfer_blocks - (transfer_start_block % max_transfer_blocks));

    transfer_size = (block_count > alignment_padding_blocks) ? (alignment_padding_blocks) : (block_count);

    /* use 4KB erase until the physical starting block aligns to bulk erase size */
    nor_status = spinor_core_erase(transfer_start_block, transfer_size);
    if (nor_status != SPINOR_CORE_DONE)
    {
      spinor_log (&spinor_dev.debug_log, 0x163);
      goto spinor_erase_bulk_end;
    }

    block_count -= transfer_size;
    transfer_start_block += transfer_size;
  }

  /* use bulk erase for aligned start block and aligned block count */
  if (block_count >= max_transfer_blocks)
  {
    transfer_size = block_count - (block_count % max_transfer_blocks);

    nor_status = spinor_core_erase_bulk(transfer_start_block, transfer_size);
    if (nor_status != SPINOR_CORE_DONE)
    {
      spinor_log (&spinor_dev.debug_log, 0x164);
      goto spinor_erase_bulk_end;
    }

    block_count -= transfer_size;
    transfer_start_block += transfer_size;
  }

  if (block_count == 0)
  {
    goto spinor_erase_bulk_end;
  }

  /* non-aligned remaining blocks will use 4KB erase */
  nor_status = spinor_core_erase(transfer_start_block, block_count);
  if (nor_status != SPINOR_CORE_DONE)
  {
    spinor_log (&spinor_dev.debug_log, 0x165);
  }

spinor_erase_bulk_end:
  if (nor_status == SPINOR_CORE_DONE)
    status = SPINOR_DEVICE_DONE;

  return status;
}

