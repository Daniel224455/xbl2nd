/**************************************************************************
 * FILE: fh_transport.h
 *
 * Transport abstraction layer for for the different transport mediums:
 * USB, PCIe, HSUART.
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

  $Header: //components/rel/boot.xf/3.0.1.c1/QcomPkg/Tools/storage/fh_loader/fh_loader/fh_transport.h#1 $
  $DateTime: 2021/04/29 02:01:56 $
  $Author: pwbldsvc $

when         who   what, where, why
----------   ---   ---------------------------------------------------------
2019-05-16   wek   Create.

===========================================================================*/
#ifndef __FH_TRANSPORT_H__
#define __FH_TRANSPORT_H__

#include "fh_comdef.h"
#include "fh_transfer.h"

typedef struct
{
  fh_res_t (*open)(const char *path);
  fh_res_t (*rx_blocking)(void *data, size_t size, size_t *bytes_rx);
  fh_res_t (*tx_blocking)(void *data, size_t size);
  fh_res_t (*close)(void);
} fh_transport_ops;

struct fh_transport
{
  enum fh_transport_type type;
  fh_transport_ops *ops;
};

/* Initializes the specific transport layer. This function should not
 * interact or open the port. */
fh_res_t fh_transport_init(enum fh_transport_type transport_type);
fh_res_t fh_transport_deinit(void);
enum fh_transport_type fh_transport_get_type(void);

/* Opens the transport layer at the given path */
fh_res_t fh_transport_open(const char *path);
fh_res_t fh_transport_rx_blocking(void *data, size_t size, size_t *bytes_rx);
fh_res_t fh_transport_tx_blocking(void *data, size_t size);
fh_res_t fh_transport_close(void);

/* Definitions of the init functions for each transport layer. */
struct fh_transport *fh_transport_com_init(void);
struct fh_transport *fh_transport_hsuart_init(void);
struct fh_transport *fh_transport_vip_init(void);
struct fh_transport *fh_transport_linux_pipe_init(void);

/* functions shared between fh_transport_com and fh_transport_linux_pipe.
   could be moved to a separate file. */
fh_res_t fh_transport_com_rx_blocking_fd (int fd, void *data, size_t size, size_t *bytes_rx);
fh_res_t fh_transport_com_tx_blocking_fd (int fd, void *tx_data, size_t size);


#endif /* __FH_TRANSPORT_H__ */
