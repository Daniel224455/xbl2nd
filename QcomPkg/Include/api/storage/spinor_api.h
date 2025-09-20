#ifndef SPINOR_API_H
#define SPINOR_API_H

/**********************************************************************
 * spinor_api.h
 *
 * Public interface declaration for the Spinor Drivers.
 *
 * This file describes the spinor external interface.
 *
 * Copyright (c) 2017-2018 Qualcomm Technologies Incorporated.
 * All Rights Reserved.
 * QUALCOMM Confidential and Proprietary
 *
 **********************************************************************/
/*===================================================================
 *
 *                       EDIT HISTORY FOR FILE
 *
 *   This section contains comments describing changes made to the
 *   module. Notice that changes are listed in reverse chronological
 *   order.
 *
 *
 *
 * when         who   what, where, why
 * ----------   ---   ------------------------------------------------
 * 2018-07-31   wek   Remove unused return error values.
 * 2017-05-25   md    Initial Release
 *=======================================================================*/

/**  \mainpage spinor Functional Overview
 *
 * The spinor driver provides a high-level interface for clients, such
 * as bootloaders, file systems, and tools to interact with the spinor devices. It
 * handles the spinor device specifics and abstracts with the client from the device-
 * specific algorithms, interfaces, and OS adaptations.
 *
 * \n
 *
 */

/**
* Error codes are 16 bit values, where the first 8 bit(starting from MSB) corresponds to the file name
* and the last 8 bits represent the error code within the file specified by the first byte of the error codes.
*
* Filename				     Error code
* Spinor_api.c			     0x1
* Spinor_core.c			     0x2
* Spinor_utils.c		     0x3
* Spinor_init_config.c	 0x4
****/

#include "com_dtypes.h"

/** Spinor API operation status */

#define SPINOR_DEVICE_DONE                 0   /**< Operation passed */
#define SPINOR_DEVICE_FAIL                (-1)  /**< Operation failed */
#define SPINOR_DEVICE_INVALID_PARAMETER   (-3)  /**< API parameters invalid */
#define SPINOR_ACCESS_DENIED              (-6)  /**< Access to the given region is denied by software */


typedef int SPINOR_STATUS;					   /**< Error status values used in SPINOR driver */
typedef void *spinor_handle_t;



/** Spinor client device data */
struct spinor_info
{
  char  device_name[32];              /**< Device name string */
  uint32 maker_id;                     /**< Manufacturer Identification */
  uint32 device_id;                    /**< Device Identification */
  uint32 version_id;                   /**< Device Version ID, if any */
  uint32 block_count;                  /**< Number of total blocks in device */
  uint32 pages_per_block;              /**< Number of pages in a block */
  uint32 page_size_bytes;              /**< Page size in bytes */
};

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

SPINOR_STATUS spinor_open( spinor_handle_t *handle);


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
  struct spinor_info *spinor_info);


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

SPINOR_STATUS spinor_close(spinor_handle_t handle);


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

SPINOR_STATUS spinor_erase(spinor_handle_t handle,
  uint32 start_block, uint32 block_count);

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

SPINOR_STATUS spinor_write(spinor_handle_t handle,
  uint32 byte_offset, uint32 byte_count, void *buffer);

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

SPINOR_STATUS spinor_read(spinor_handle_t handle,
  uint32 byte_offset, uint32 byte_count, void *buffer);

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

SPINOR_STATUS spinor_erase_bulk(spinor_handle_t handle,
                              uint32 start_block, uint32 block_count);


#endif /* #ifndef SPINOR_API_H */
