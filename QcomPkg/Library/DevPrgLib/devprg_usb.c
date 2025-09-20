/**************************************************************************
 * FILE: devprg_usb.c
 *
 * USB abstraction layer for TX and RX.
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
2016-11-07   wek   Create.

===========================================================================*/

#include "comdef.h"
#include "devprg_firehose.h"
#include "devprg_log.h"
#include "devprg_usb.h"
#include "devprg_transport.h"

#ifdef DEVPRG_HSUSB_API

#include "qhsusb_al_bulk.h"
#define DEVPRG_BULK_SUCCESS  0

static void devprg_usb_al_bulk_init(void)
{
  qhsusb_al_bulk_init(0x0, QHSUSB_MODE__SER_ONLY);
}

static void devprg_usb_al_bulk_poll(void)
{
  qhsusb_al_bulk_poll();
}

static uint32 devprg_usb_al_bulk_receive(byte *rx_buf,
                                         uint32 len,
                                         sahara_rx_cb_type rx_cb,
                                         uint32 * err_code)
{
  return qhsusb_al_bulk_receive(rx_buf, len, rx_cb, err_code);
}

static uint32 devprg_usb_al_bulk_transmit(byte *tx_buf,
                                          uint32 len,
                                          sahara_tx_cb_type tx_cb,
                                          uint32 * err_code)
{
  return qhsusb_al_bulk_transmit(tx_buf, len, tx_cb, err_code);
}

static uint32 devprg_usb_al_bulk_get_max_packet_size(void)
{
  return qhsusb_al_bulk_get_max_packet_size();
}

static void devprg_usb_al_bulk_set_zlp_mode(boolean host_sends_zlp)
{
  qhsusb_al_bulk_set_zlp_mode(host_sends_zlp);
}

#else

#include "qusb_al_bulk.h"
#define DEVPRG_BULK_SUCCESS  BULK_SUCCESS
static void devprg_usb_al_bulk_init(void)
{
  qusb_al_bulk_init(QUSB_MAX_SPEED_SUPER);
}

static void devprg_usb_al_bulk_poll(void)
{
  qusb_al_bulk_poll();
}

static uint32 devprg_usb_al_bulk_receive(byte *rx_buf,
                                         uint32 len,
                                         sahara_rx_cb_type rx_cb,
                                         uint32 * err_code)
{
  return qusb_al_bulk_receive(rx_buf, len, rx_cb, err_code);
}

static uint32 devprg_usb_al_bulk_transmit(byte *tx_buf,
                                          uint32 len,
                                          sahara_tx_cb_type tx_cb,
                                          uint32 * err_code)
{
  return qusb_al_bulk_transmit(tx_buf, len, tx_cb, err_code);
}

static uint32 devprg_usb_al_bulk_get_max_packet_size(void)
{
  return qusb_al_bulk_get_max_packet_size();
}

static void devprg_usb_al_bulk_set_zlp_mode(boolean host_sends_zlp)
{
  qusb_al_bulk_set_zlp_mode(host_sends_zlp);
}

#endif


/* Three variables to communicate information from the callback, */
struct devprg_usb_cb_data
{
  uint32 error;
  uint32 bytes;
  uint8 called;
};


static struct devprg_usb_cb_data usb_cb_tx_data, usb_cb_rx_data;

static void devprg_usb_rx_callback(uint32 bytes, uint32 err_code)
{
  usb_cb_rx_data.error = err_code;
  usb_cb_rx_data.bytes = bytes;
  usb_cb_rx_data.called = 1;
  return;
}

static void devprg_usb_tx_callback(uint32 bytes, uint32 err_code)
{
  usb_cb_tx_data.error = err_code;
  usb_cb_tx_data.bytes = bytes;
  usb_cb_tx_data.called = 1;
  return;
}

dp_res_t devprg_usb_set_mode(unsigned int key, unsigned int val)
{
  if (key != 0)
    return DEVPRG_ERROR_INVAL_PARAM;

  devprg_usb_al_bulk_set_zlp_mode(val);
  return DEVPRG_SUCCESS;
}


dp_res_t devprg_usb_rx_queue(void *data, size_t size)
{
  uint32 frame_size = devprg_usb_al_bulk_get_max_packet_size ();
  uint32 usb_pad, usb_error;
  size_t rx_bytes = size;

  usb_cb_rx_data.error = DEVPRG_BULK_SUCCESS;
  usb_cb_rx_data.bytes = 0;
  usb_cb_rx_data.called = 0;

  usb_pad = rx_bytes % frame_size;
  if (usb_pad != 0)
    rx_bytes += frame_size - usb_pad; /* round up to the USB PAD boundary. */

  devprg_usb_al_bulk_receive(data, rx_bytes, devprg_usb_rx_callback, &usb_error);
  if (usb_error != DEVPRG_BULK_SUCCESS)
  {
    DP_LOGE("USB RX bulk received failed %d", usb_error);
    return DEVPRG_ERROR_RECEIVE;
  }

  return DEVPRG_SUCCESS;
}

dp_res_t devprg_usb_rx_queue_check(size_t *rx_num_bytes)
{
  *rx_num_bytes = 0;
  if(usb_cb_rx_data.called == 0)
    devprg_usb_al_bulk_poll();

  if(usb_cb_rx_data.called == 0)
    return DEVPRG_ERROR_NOT_READY;

  if(usb_cb_rx_data.error != DEVPRG_BULK_SUCCESS)
  {
    DP_LOGE("USB RX failed on callback %d", usb_cb_rx_data.error);
    return DEVPRG_ERROR_RECEIVE;
  }

  *rx_num_bytes = usb_cb_rx_data.bytes;
  return DEVPRG_SUCCESS;
}

dp_res_t devprg_usb_tx_queue(void *data, size_t size)
{
  uint32 usb_error;

  usb_cb_tx_data.error = DEVPRG_BULK_SUCCESS;
  usb_cb_tx_data.bytes = 0;
  usb_cb_tx_data.called = 0;

  devprg_usb_al_bulk_transmit(data, size, devprg_usb_tx_callback, &usb_error);

  if (usb_error != DEVPRG_BULK_SUCCESS)
    return DEVPRG_ERROR_TRANSMIT;

  return DEVPRG_SUCCESS;
}

dp_res_t devprg_usb_tx_queue_check(size_t size)
{
  if (usb_cb_tx_data.called == 0)
    devprg_usb_al_bulk_poll();

  if (usb_cb_tx_data.called == 0)
    return DEVPRG_ERROR_NOT_READY;

  if (usb_cb_tx_data.error != DEVPRG_BULK_SUCCESS || usb_cb_tx_data.bytes != size)
    return DEVPRG_ERROR_TRANSMIT;

  return DEVPRG_SUCCESS;
}

int devprg_usb_async_support(void)
{
  return 1;
}

static devprg_transport_ops devprg_transport_usb_ops =
{
  devprg_usb_set_mode,
  devprg_usb_rx_queue,
  devprg_usb_rx_queue_check,
  devprg_usb_tx_queue,
  devprg_usb_tx_queue_check,
  devprg_usb_async_support,
};

static struct devprg_transport devprg_transport_usb =
{
  DEVPRG_TRANSPORT_USB,
  &devprg_transport_usb_ops
};

struct devprg_transport *devprg_transport_usb_init(void)
{
  devprg_usb_al_bulk_init();
  return &devprg_transport_usb;
}

