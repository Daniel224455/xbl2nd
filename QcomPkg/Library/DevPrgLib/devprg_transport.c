/**************************************************************************
 * FILE: devprg_transport.c
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

#include "comdef.h"
#include "devprg_firehose.h"
#include "devprg_log.h"
#include "devprg_usb.h"
#include "devprg_utils.h"
#include "devprg_transport.h"

static struct devprg_transport *transport;

struct devprg_transport_reset
{
  enum devprg_reboot_type reboot_type;
  unsigned int timeout_ms;
};

struct devprg_transport_reset transport_reset_data;

dp_res_t devprg_transport_rx_blocking(void *data, size_t size, size_t *rx_bytes)
{
  dp_res_t res;

  res = devprg_transport_rx_queue(data, size);
  if (res == DEVPRG_SUCCESS)
    res = devprg_transport_rx_queue_wait(rx_bytes);

  return res;
}

dp_res_t devprg_transport_rx_queue(void *data, size_t size)
{
  dp_res_t result;
  result = transport->ops->rx_queue(data, size);
  return result;
}

dp_res_t devprg_transport_rx_queue_wait(size_t *rx_bytes)
{
  dp_res_t result;
  uint64 start_time = 0, end_time = 0, total_time_ms = 0;
  int check_timeout = 0;

  /* Check if we need to reboot the device on timeout. */
  if (transport_reset_data.timeout_ms != 0 &&
      transport_reset_data.reboot_type != DEVPRG_REBOOT_NONE &&
      transport_reset_data.reboot_type < DEVPRG_REBOOT_MAX)
    check_timeout = 1;

  if (check_timeout)
    start_time = devprg_time_usec();

  do
  {
    result = transport->ops->rx_queue_check(rx_bytes);

    if (check_timeout)
    {
      end_time = devprg_time_usec();
      total_time_ms = (end_time - start_time) / 1000;
      if (total_time_ms > transport_reset_data.timeout_ms)
      {
        /* Call function to reboot the device. */
      }
    }

  }while (result == DEVPRG_ERROR_NOT_READY);
  return result;
}

dp_res_t devprg_transport_tx_blocking(void *data, size_t size)
{
  dp_res_t result;
  uint64 start_time = 0, end_time = 0, total_time_ms = 0;
  int check_timeout = 0;

  result = transport->ops->tx_queue(data, size);
  if (result != DEVPRG_SUCCESS)
    return result;

  /* Check if we need to reboot the device on timeout. */
  if (transport_reset_data.timeout_ms != 0 &&
      transport_reset_data.reboot_type != DEVPRG_REBOOT_NONE &&
      transport_reset_data.reboot_type < DEVPRG_REBOOT_MAX)
    check_timeout = 1;

  if (check_timeout)
    start_time = devprg_time_usec();

  do
  {
    result = transport->ops->tx_queue_check(size);

    if (check_timeout)
    {
      end_time = devprg_time_usec();
      total_time_ms = (end_time - start_time) / 1000;
      if (total_time_ms > transport_reset_data.timeout_ms)
      {
        /* Call function to reboot the device. */
      }
    }
  }while (result == DEVPRG_ERROR_NOT_READY);

  return result;
}


dp_res_t devprg_transport_set_mode(enum devprg_transport_mode_key key, unsigned int val)
{
  return transport->ops->set_mode(key, val);
}


dp_res_t devprg_transport_reset_timeout (enum devprg_reboot_type reboot_type,
                                         unsigned int timeout_ms)
{
  if (transport->ops->async_support() != 0)
    return DEVPRG_ERROR_NOT_SUPPORTED;

  if (transport_reset_data.reboot_type >= DEVPRG_REBOOT_MAX)
    return DEVPRG_ERROR_INVAL_PARAM;

  transport_reset_data.reboot_type = reboot_type;
  transport_reset_data.timeout_ms = timeout_ms;
  return DEVPRG_SUCCESS;
}


int devprg_transport_init(void)
{
  transport_reset_data.reboot_type = DEVPRG_REBOOT_NONE;
  transport_reset_data.timeout_ms = 0;

  transport = devprg_transport_usb_init();
  return 0;
}
