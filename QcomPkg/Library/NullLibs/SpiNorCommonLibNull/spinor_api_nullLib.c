  /**********************************************************************
 * spinor_api.c
 *
 * Implementation of spinor driver support functions
 *
 * Copyright (c) 2016-2017
 * Qualcomm Technologies Incorporated.
 * All Rights Reserved.
 * Qualcomm Confidential and Proprietary
 *
 **********************************************************************/
/*=======================================================================
                        Edit History


when        who       what, where, why
----------  -----     ---------------------------------------------------

2017-05-25  md        Initial Release

======================================================================*/

#include "spinor_api.h"
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
   return 0;
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
   return 0;
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
    return 0;
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
    return 0;
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
    return 0;
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
    return 0;
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
    return 0;
}

