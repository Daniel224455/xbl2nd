/**************************************************************************
 * FILE: fh_transport_linux_pipe.h
 *
 * Implementation of the Linux pipe transport layer. Used only for
 * development and testing purposes.
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

  $Header: //components/rel/boot.xf/3.0.1.c1/QcomPkg/Tools/storage/fh_loader/fh_loader/fh_transport_linux_pipe.c#1 $
  $DateTime: 2021/04/29 02:01:56 $
  $Author: pwbldsvc $

when         who   what, where, why
----------   ---   ---------------------------------------------------------
2019-05-21   wek   Create.

===========================================================================*/

#include "fh_comdef.h"
#include "fh_log.h"
#include "fh_transport.h"

#ifdef FIREHOSE_SIMULATION
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/select.h>
#include <limits.h>
#include "stringl.h"

struct fh_linux_pipe_data
{
  int init;
  int tx_fd;
  int rx_fd;
};

static struct fh_linux_pipe_data linux_pipe_data = {0};

static fh_res_t fh_linux_pipe_open(const char *path)
{
  struct fh_linux_pipe_data *pdata = &linux_pipe_data;
  char path_t2h[100];

  if (pdata->init != 0)
  {
    FH_LOGE("Opening the port without closing %d.\n", pdata->init);
    return INVALID_PARAMETER;
  }

  if (strlen(path) + 4 > sizeof(path_t2h))
  {
    FH_LOGE("Port path too long %d>%d", strlen(path), sizeof(path_t2h) - 4);
    return INVALID_PARAMETER;
  }

  strlcpy(path_t2h, path, sizeof(path_t2h));
  strlcat(path_t2h, "t2h", sizeof(path_t2h));

  pdata->tx_fd = open(path, O_RDWR | O_SYNC);
  if (pdata->tx_fd == -1)
  {
    FH_LOGE("Failed to open tx port: %s", strerror(errno));
    return OPEN_PORT_FAILED;
  }
  pdata->rx_fd = open(path_t2h, O_RDONLY | O_SYNC);
  if (pdata->rx_fd == -1)
  {
    close(pdata->tx_fd);
    FH_LOGE("Failed to open rx port: %s", strerror(errno));
    return OPEN_PORT_FAILED;
  }

  pdata->init = 1;
  return SUCCESS;
}

static fh_res_t fh_linux_pipe_rx_blocking (void *data, size_t size, size_t *bytes_rx)
{
  struct fh_linux_pipe_data *pdata = &linux_pipe_data;
  if (pdata->init != 1)
    return INVALID_PARAMETER;

  return fh_transport_com_rx_blocking_fd(pdata->rx_fd, data, size, bytes_rx);
}

static fh_res_t fh_linux_pipe_tx_blocking(void *tx_data, size_t size)
{
  struct fh_linux_pipe_data *pdata = &linux_pipe_data;
  if (pdata->init != 1)
    return INVALID_PARAMETER;

  return fh_transport_com_tx_blocking_fd(pdata->tx_fd, tx_data, size);
}

static fh_res_t fh_linux_pipe_close(void)
{
  struct fh_linux_pipe_data *pdata = &linux_pipe_data;
  if (pdata->init != 1)
    return INVALID_PARAMETER;

  close(pdata->tx_fd);
  close(pdata->rx_fd);
  pdata->init = 0;

  return SUCCESS;
}

#else

static fh_res_t fh_linux_pipe_open(const char *path)
{
  (void) path;
  return NOT_SUPPORTED;
}

static fh_res_t fh_linux_pipe_rx_blocking (void *rx_data, size_t size, size_t *bytes_rx)
{
  (void) rx_data;
  (void) size;
  (void) bytes_rx;
  return NOT_SUPPORTED;
}

static fh_res_t fh_linux_pipe_tx_blocking(void *tx_data, size_t size)
{
  (void) tx_data;
  (void) size;
  return NOT_SUPPORTED;
}

static fh_res_t fh_linux_pipe_close(void)
{
  return NOT_SUPPORTED;
}

#endif

static fh_transport_ops fh_transport_linux_pipe_ops =
{
  fh_linux_pipe_open,
  fh_linux_pipe_rx_blocking,
  fh_linux_pipe_tx_blocking,
  fh_linux_pipe_close,
};

static struct fh_transport fh_transport_linux_pipe =
{
  FH_TRANSPORT_LINUX_PIPE_TEST,
  &fh_transport_linux_pipe_ops,
};

struct fh_transport *fh_transport_linux_pipe_init(void)
{
  return &fh_transport_linux_pipe;
}
