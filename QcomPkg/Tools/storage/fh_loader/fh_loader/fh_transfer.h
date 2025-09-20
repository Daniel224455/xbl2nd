/**************************************************************************
 * FILE: fh_transfer.h
 *
 * Transfer abstraction layer for TX and RX. Includes VIP.
 *
 * Copyright (c) 2019 Qualcomm Technologies, Inc.
 * All Rights Reserved.
 * Qualcomm Proprietary
 *
 *************************************************************************/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/boot.xf/3.0.1.c1/QcomPkg/Tools/storage/fh_loader/fh_loader/fh_transfer.h#1 $
  $DateTime: 2021/04/29 02:01:56 $
  $Author: pwbldsvc $

when         who   what, where, why
----------   ---   ---------------------------------------------------------
2019-05-16   wek   Create.

===========================================================================*/

#ifndef __FH_TRANSFER_H__
#define __FH_TRANSFER_H__

#include "fh_comdef.h"

#define MAX_XML_SIZE    (4*1024)

enum fh_transport_type
{
  FH_TRANSPORT_NONE,
  FH_TRANSPORT_COM,
  FH_TRANSPORT_HSUART,
  FH_TRANSPORT_VIP,    /* Simulates the device and generates the VIP Tables.  */
  FH_TRANSPORT_LINUX_PIPE_TEST, /* Linux pipe used for offline development purpose*/
  FH_TRANSPORT_MAX,
};

/* Intializes the transfer module. */
fh_res_t fh_transfer_init(enum fh_transport_type transfer_type);

/* Terminate the session, may not be called in certain scenarios (Ctrl-C) */
fh_res_t fh_transfer_deinit(void);

/* Get the currently configured transport type */
enum fh_transport_type fh_transfer_get_type(void);

/* Open the port at the given path */
fh_res_t fh_transfer_open(const char *path);

/* Receive data from the target, waiting for it to get a <response XML.
 *
 * param  data    Pointer to the buffer where to place the data.
 * param  size    Number of bytes in the data buffer.
 * param  bytes_read[out] Number of bytes populated into the buffer.
 *
 * return 0 on success, error otherwise.
 */
fh_res_t fh_rx_blocking_response_xml(void *data, size_t size, size_t *bytes_read);

/* Receive data from the target, waiting for it to complete.
 *
 * param  data    Pointer to the buffer where to place the data.
 * param  size    Number of bytes in the data buffer.
 * param  bytes_read[out] Number of bytes populated into the buffer.
 *
 * return 0 on success, error otherwise.
 */
fh_res_t fh_rx_blocking_raw(void *data, size_t size, size_t *bytes_read);

/* Transmit data to the target, waiting for it to complete.
 *
 * param  data   Pointer to the buffer with the data to be sent to the host.
 * param  size   Number of bytes in the data buffer.
 *
 * return 0 on success, error otherwise.
 */
fh_res_t fh_tx_blocking(void *data, size_t size);

/* Close the port that is currently open. */
fh_res_t fh_transfer_close(void);

/* Set the transfer layer to handle VIP.
 *
 * Indicate that this is a VIP type transfer, where we will need to
 * interleave data transfer with hash table transfers.
 *
 * param signed_table   Path to the signed table, this is the first blob sent.
 * param chained_tables Path to the chained digests tables. These are sent
 *                      after the hashes on the signed_table are consumed.
 * param digests_per_table  Maximum number of hashes per table, typically 256.
 *
 * return 0 on success, error otherwise.
 */
fh_res_t fh_transfer_vip(const char *signed_table, const char *chained_tables, size_t digests_per_table);

#define TRANSFER_OPTION_SHOW_DIGEST_PER_PACKET  0x01
/* Set different options for the transfer layer.
 *
 * Currently only supports Show digest per packet option.
 */
fh_res_t fh_transfer_options(uint64 options);

#endif /* __FH_TRANSFER_H__ */
