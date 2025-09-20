/**************************************************************************
 * FILE: devprg_transport.h
 *
 * Abstraction layer for Transmit (TX) and receive(RX) data to/from host.
 *
 * Copyright (c) 2016 Qualcomm Technologies, Inc.
 * All Rights Reserved.
 * Qualcomm Proprietary
 *
 *************************************************************************/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.


when         who   what, where, why
----------   ---   ---------------------------------------------------------
2018-10-04   wek   Create.

===========================================================================*/
#ifndef __DEVPRG_TRANSPORT_H__
#define __DEVPRG_TRANSPORT_H__

#include "comdef.h"
#include "devprg_firehose.h"
#include "devprg_log.h"
#include "devprg_usb.h"

struct devprg_transport;

enum devprg_transport_mode_key
{
  DEVPRG_USB_ZLP_KEY,
  DEVPRG_TRANSPORT_SET_MODE_MAX,
};

/* Operations that can be executed by the transport layer. */
typedef struct
{
  dp_res_t (*set_mode)(enum devprg_transport_mode_key key, unsigned int val); /* Set ZLP for USB */
  dp_res_t (*rx_queue)(void *data, size_t size);
  dp_res_t (*rx_queue_check)(size_t *rx_num_bytes);
  dp_res_t (*tx_queue)(void *data, size_t size);
  dp_res_t (*tx_queue_check)(size_t rx_num_bytes);
  int (*async_support)(void); /* Check if ASYNC interface is supported (allowing timeout) */
} devprg_transport_ops;

/* Supported transport layers. */
enum devprg_transport_type
{
  DEVPRG_TRANSPORT_USB = 0,
  DEVPRG_TRANSPORT_UART = 1,
  DEVPRG_TRANSPORT_PCIE = 2,
  DEVPRG_TRANSPORT_MAX,
};

/* Transport device definitions. This fields should be set in init() call. */
struct devprg_transport
{
  enum devprg_transport_type type;
  devprg_transport_ops *ops;
};

/* Initalize the structures in the layer, will populate the operations for the
   configured transport layer (USB/UART/PCIe). */
int devprg_transport_init(void);

/* Receive data from host, waiting for it to complete.
 *
 * param  data    Pointer to the buffer where to place the data.
 * param  size    Number of bytes in the data buffer.
 * param  rsize   Number of bytes received and placed in the data buffer.
 *
 * return -1 on error, number of bytes read on success.
 */
dp_res_t devprg_transport_rx_blocking(void *data, size_t size, size_t *rsize);

/* Queue receive from host of a given size.
 *
 * param  data    Pointer to the buffer where to place the data.
 * param  size       Number of bytes in the data buffer.
 */
dp_res_t devprg_transport_rx_queue(void *data, size_t size);

/* Wait for the queued receive to complete.
 *
 * param  rx_bytes   number of bytes received.
 */
dp_res_t devprg_transport_rx_queue_wait(size_t *rx_bytes);

/* Transmit data to the host. Return when transmission is complete.
 *
 * param  data   Pointer to data to transmit.
 * param  size   Number of bytes in the data buffer.
 *
 * return  0 (DEVPRG_SUCCESS) on success, error code otherwise.
 */
dp_res_t devprg_transport_tx_blocking(void *data, size_t size);



/* Set mode, used in USB for setting ZLP Mode.
 *
 * To indicate the hosts zends USB ZLP Packet (like windows host) do:
 * devprg_transport_set_mode(DEVPRG_USB_ZLP_KEY, 1);
 *
 **/
dp_res_t devprg_transport_set_mode(enum devprg_transport_mode_key key, unsigned int val);

/* Tells the transport layer to reboot/power off the device if no tx/rx happens
 * after the given timeout.
 *
 * param  reboot_type   Reset, EDL, Power off.
 * param  timeout_ms    Timeout in milliseconds after which reboot should happen.
 *
 * return  0 (DEVPRG_SUCCESS) on success, error code if the transport layer does
 *         not support asynchronous transfer.
 */
dp_res_t devprg_transport_reset_timeout (enum devprg_reboot_type reboot_type,
                                         unsigned int timeout_ms);

#endif /* __DEVPRG_TRANSPORT_H__ */

