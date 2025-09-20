#ifndef XPORT_SMEM_CONFIG_H
#define XPORT_SMEM_CONFIG_H

/**
 * @file xport_smem_config.h
 *
 * Internal definitions for SMEM Glink transport configurations
 */

/*==============================================================================
     Copyright (c) 2014-2015 QUALCOMM Technologies Incorporated.
     All rights reserved.
     Qualcomm Confidential and Proprietary
==============================================================================*/

/*===========================================================================

                           EDIT HISTORY FOR FILE


when       who     what, where, why
--------   ---     ----------------------------------------------------------
05/07/15   an      QoS support
06/17/14   an      Initial version
===========================================================================*/

/*===========================================================================
                        INCLUDE FILES
===========================================================================*/
#include "glink.h"
#include "glink_os_utils.h"
#include "smem_type.h"
#include "glink_transport_if.h"

/*===========================================================================
                      MACRO DECLARATIONS
===========================================================================*/

/*===========================================================================
                      TYPE DECLARATIONS
===========================================================================*/

typedef boolean (*xport_smem_alloc_fifo_fn_type)(void *ctx_ptr);
typedef void (*xport_smem_clear_interrupt_fn_type)(void);

typedef struct _xport_smem_config_type
{
  const char* remote_ss;                /* Remote host name        */
  smem_host_type remote_host;           /* Remote host ID          */
  uint32 fifo_size;                     /* Tx FIFO size            */
  uint32 mtu_size;                      /* Tx MTU size             */
  os_ipc_intr_type irq_out;             /* Outgoing interrupt      */
  uint32 irq_in;                        /* Incoming interrupt      */
  glink_transport_qos_if_type *qos_api; /* QoS interface           */
  xport_smem_alloc_fifo_fn_type alloc_fifo; /* Function to allocate FIFO */
  xport_smem_clear_interrupt_fn_type clear_interrupt; /* Function to clear interrupt */
} xport_smem_config_type;       

#endif //XPORT_SMEM_CONFIG_H

