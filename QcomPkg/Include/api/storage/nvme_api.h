/******************************************************************************
 * nvme_api.h
 *
 * This file provides NVMe external API definitions.
 *
 * Copyright (c) 2018-2019 Qualcomm Technologies, Inc.
 * All Rights Reserved.
 * Qualcomm Proprietary
 *
 *****************************************************************************/
/*=============================================================================

                        EDIT HISTORY FOR MODULE


when        who   what, where, why
----------  ---   ---------------------------------------------------
2019-05-08  wek   Add support for Format and Write Zeroes.
2018-11-15  wek   Initial version (create).

=============================================================================*/
#ifndef __NVME_API_H__
#define __NVME_API_H__

#include "comdef.h"

/** \mainpage NVMe  Controller API Overview
 *
 * Non-Volitile Memory Express (NVMe) provides a simple set of public APIs for
 * reading and writing to the memory device.
 */

/**
 * @defgroup nvme_driver NVMe Card Driver
 *
 * The NVMe driver provides block-oriented data transfer to or from the memory
 * device.
 */

/*@{*/


/**
 * Define the error code used for the public API
 */
#define NVME_SUCCESS (0)
#define NVME_SW_ENOMEM                                0x01000001
#define NVME_SW_ECOMQ_EMPTY                           0x01000002
#define NVME_SW_ECOMQ_TIMEOUT                         0x01000003
#define NVME_SW_ECOMQ_INVAL_SUBQ_HEAD                 0x01000004
#define NVME_SW_ECOMQ_RANGE_SUBQ_HEAD                 0x01000005
#define NVME_SW_ESUBQ_FULL                            0x01000006
#define NVME_SW_EINVAL_PARAM                          0x01000007
#define NVME_SW_EINVAL_NAMESPACE                      0x01000008
#define NVME_SW_EINVAL_LBA                            0x01000009
#define NVME_SW_EMAX_HANDLE                           0x0100000A /* too many open handles*/
#define NVME_SW_EBAD_HANDLE                           0x0100000B /* using closed/invalid handle */
#define NVME_SW_ENOT_SUPPORTED                        0x0100000C

#define NVME_HW_ENVME_PWR                             0x03000001
#define NVME_HW_ENVME_PCIE                            0x03000002
#define NVME_HW_ENOT_NVME                             0x03000003
#define NVME_HW_ETIMEOUT_ENABLE                       0x03000004
#define NVME_HW_ETIMEOUT_DISABLE                      0x03000005
#define NVME_HW_EBAD_NS_INFO                          0x03000006

#define NVME_HW_ENOT_SUPPORTED                        0x0300000C

/* Error codes that with values higher than 0x08000000 are errors
 * returned by the NVMe device. For these error codes the API will
 * return (0x08000000 | NVMe_error), which makes it easy to see the
 * error code and check the NVMe spec for the error.
 * List of families of error codes
 * Starts with 0x08000000: Generic Status code.
 * Starts with 0x08100000: Command specific status code.
 * Starts with 0x08200000: Media and Data status code.
 * Starts With 0x08700000: Vendor specific status code.
 */

/**
 * NVMe device handle is a private data structure.  This opaque structure type
 * is declared so the APIs can access the memory device via a pointer to this
 * nvme_handle structure.
 */
struct nvme_handle;

/**
 * nvme_controller_basic_info data stucture is used to return basic NVMe device
 * specific information.
 */
struct nvme_device_info
{
  uint16 vendor_id;            /* PCI Vendor ID (VID) */
  uint16 subsys_vid;           /* PCI sub-system vendor ID (SSVID) */
  uint8  serial[20];           /* Product serial number (SN) */
  uint8  model_num[40];        /* Product model number  (MN) */
  uint8  fw_rev[8];            /* Firmware Revision (FR) */
  uint32 num_namespace;        /* Number of Namespaces (NN) */
};

/**
 * nvme_namespace_basic_info data stucture is used to return basic NVMe namespace
 * specific information.
 */
struct nvme_namespace_sizes
{
  uint64 size_blocks;          /* Namespace Size (NSZE) in blocks        */
  uint64 capcaity_blocks;      /* Namespace Capacity (NCAP) in blocks    */
  uint64 utilization_blocks;   /* Namespace Utilization (NUSE) in blocks */
  uint8  guid[16];             /* Namespace Globally Unique Identifier (NGUID) */
  uint32 block_size;  /* Logical Block Data Size in bytes, derived from LBADS in the spec. */
};


/*@}*/ /* group nvme_driver */

/**
 * @defgroup nvme_api NVME API Overview
 * @ingroup nvme_driver
 *
 * The NVMe API provides a list of public functions to NVMe clients.
 *
 */
/*@{*/

/** \details
 * Get the basic NVMe controller information from the device. This is a subset
 * of entries from the Identify Controller data structure in the NVMe spec.
 * This API will initialize the controller if not previously initialized.
 *
 * @param[in] id
 *    ID to address different NVMe devices connected to the system.
 *    ID start at 0.
 *
 * @param[out] info
 *    pointer to the structure to save basic controller info.
 *
 * @return 0 on success.  Error code otherwise
 */
uint32 nvme_get_device_info (uint32 id,
                             struct nvme_device_info *info);

/** \details
 * Open the specified NVMe device and namespace. This function will initialize
 * the NVMe device controller if not previously initialized. It will return NULL
 * and set the error_num in the case of error. On success it will return a valid
 * pointer to be used for IO operations.
 *
 * @param[in] id
 *    ID to address different NVMe devices connected to the system.
 *    ID start at 0.
 *
 * @param[in] namespace_num
 *    Number to identify the namespace to open. Namespace numbers start at 1.
 *
 * @param[out] error_num
 *    Set to error code when open operation fails.
 *
 * @return A pointer to the NVMe handle if successful.  NULL if failed.
 *
 */
struct nvme_handle *nvme_open (uint32 id, uint32 namespace_num, uint32 *error_num);

/** \details
 * Close the NVMe handle.
 *
 * @param[in] handle
 *    a pointer to the NVMe handle that was returned from \ref nvme_open
 *
 * @return 0 if successful.  Error code if failed.
 */
uint32 nvme_close (struct nvme_handle *handle);

/** \details
 * Get information about an opened namespace.
 *
 * @param[in] handle
 *    a pointer to the NVMe handle that was returned from \ref nvme_open
 *
 * @param[out] info
 *    pointer to the structure to save the memory card specific information
 *
 * @return 0 on success.  Error code otherwise
 */
uint32 nvme_get_namespace_sizes (struct nvme_handle *handle,
                                 struct nvme_namespace_sizes *info);

/** \details
 * Read \c num_blocks blocks of data starting at \c lba from the
 * device/namespace, place the data at the memory location given by \c buff.
 *
 * @param[in] handle
 *    a pointer to the NVMe handle that was returned from \ref nvme_open
 *
 * @param[out] buff
 *    pointer to buffer for the incoming data, data buffer must be 32bit aligned
 *
 * @param[in] lba
 *    Logic Block Address of the starting block to read from.
 *
 * @param[in] num_blocks
 *    number of blocks to be read. Size of each block depends on namespace
 *    configuration.
 *
 * @return 0 if successful.  Error code otherwise.
 */
uint32 nvme_read (struct nvme_handle *handle,
                  void               *buff,
                  uint64              lba,
                  uint64              num_blocks);

/** \details
 * Write \c num_blocks blocks of data from \c buff into the device/namespace
 * starting from \c lba.
 *
 * @param[in] handle
 *    a pointer to the NVMe handle that was returned from \ref nvme_open
 *
 * @param[out] buff
 *    pointer to buffer for the data to be written to disk.
 *
 * @param[in] lba
 *    Logic Block Address of the starting block to write to.
 *
 * @param[in] num_blocks
 *    number of blocks to be written. Size of each block depends on namespace
 *    configuration.
 *
 * @return 0 if successful.  Error code otherwise.
 */
uint32 nvme_write (struct nvme_handle *handle,
                   void               *buff,
                   uint64              lba,
                   uint64              num_blocks);

/** \details
 * Flush all data cached by the NVMe for the opened namespace into permanent
 * storage.
 *
 * @param[in] handle
 *    a pointer to the NVMe handle that was returned from \ref nvme_open
 *
 * @return 0 if successful.  Error code otherwise.
 */
uint32 nvme_flush(struct nvme_handle *handle);


/** \details
 * Make the device to zero out \c num_blocks blocks of data from the
 * device/namespace starting from \c lba. Write zeroes is an optional NVMe
 * command, when the device does not support it the API returns
 * NVME_HW_ENOT_SUPPORTED
 *
 * @param[in] handle
 *    a pointer to the NVMe handle that was returned from \ref nvme_open
 *
 * @param[in] lba
 *    Logic Block Address of the starting block to write to.
 *
 * @param[in] num_blocks
 *    number of blocks to be written. Size of each block depends on namespace
 *    configuration.
 *
 * @return 0 if successful.  Error code otherwise.
 */
uint32 nvme_write_zeroes(struct nvme_handle *handle,
                         uint64             lba,
                         uint64             num_blocks);

/** \details
 * Format the open NVMe namepsace. Format is an optional NVMe command, when
 * the device does not support it the API returns NVME_HW_ENOT_SUPPORTED.
 *
 * @param[in] handle
 *    a pointer to the NVMe handle that was returned from \ref nvme_open
 *
 * @param[in] lba_size
 *    Desired LBA Size for the formatted NVMe. Typically this would be 512.
 *
 * @return 0 if successful.  Error code otherwise.
 */
uint32 nvme_format (struct nvme_handle *handle,
                    uint32 lba_size);

/*@}*/ /* group nvme_driver */
#endif /* __NVME_API_H__ */

