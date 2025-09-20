/*===========================================================================

            GLink SMEM transport 8916 MPSS Configuration Structures

=============================================================================

  @file
    sport_smem_config.c

    Contains structures to be used in Glink SMEM trasnport configuration.

  Copyright (c) 2014 - 2015,2017 Qualcomm Technologies, Inc.
  All rights reserved.
  Qualcomm Confidential and Proprietary
===========================================================================*/

/*===========================================================================

                           EDIT HISTORY FOR FILE


when       who     what, where, why
--------   ---     ----------------------------------------------------------
05/11/15   an      QoS support
02/13/15   bc      Decrease the fifo size due to lack of space
01/14/15   bc      Update outgoing interrupt
01/13/15   bc      Update more processors
06/17/14   an      Initial version
===========================================================================*/

/*===========================================================================
                        INCLUDE FILES
===========================================================================*/
#include "xport_smem_config.h"
#include "xport_smem_qos_generic.h"

/*===========================================================================
                        DATA DECLARATIONS
===========================================================================*/
static glink_transport_qos_if_type xport_smem_qos_config = 
{
  /** Provides maximum data rate (tokens/sec) associated with transport. */
  20000,
  /** Provides data rate achievable by the transport for given request. */
  xport_smem_get_req_rate_generic,
  /** Allocates context structure to use by transport servicing given QoS request. */
  xport_smem_alloc_req_ctx_generic,
  /** Frees context structure used by transport servicing given QoS request. */
  xport_smem_free_req_ctx_generic,
  /** Starts QoS mode. */
  xport_smem_start_req_generic,
  /** Stops QoS mode. */
  xport_smem_stop_req_generic,
  /** Provides ramp up time in microseconds. */
  xport_smem_get_ramp_time_generic
};

const xport_smem_config_type xport_smem_config[] =
{
  /* Boot->Adsp */
  {
    "lpass",               /* Remote host name        */
    SMEM_ADSP,             /* Remote host ID          */
    16*1024,               /* Tx FIFO size            */
    4*1024,                /* Tx MTU size             */
    {(uint32 *)(uintptr_t)0x17C0000C, 0x00000100}, /* Outgoing interrupt   */
    0,                     /* Incoming interrupt      */
    &xport_smem_qos_config,
    NULL,                  /* Use default Alloc FIFO */
    NULL                   /* Use default clear interrupt */
  },
  /* Boot->Slpi */
  {
    "dsps",                /* Remote host name        */
    SMEM_SSC,              /* Remote host ID          */
    16*1024,               /* Tx FIFO size            */
    4*1024,                /* Tx MTU size             */
    {(uint32 *)(uintptr_t)0x17C0000C, 0x01000000}, /* Outgoing interrupt   */
    0,                     /* Incoming interrupt      */
    &xport_smem_qos_config,
    NULL,                  /* Use default Alloc FIFO */
    NULL                   /* Use default clear interrupt */
  },
    /* Boot->Cdsp */
  {
    "cdsp",                /* Remote host name        */
    SMEM_CDSP,            /* Remote host ID          */
    16*1024,               /* Tx FIFO size            */
    4*1024,                /* Tx MTU size             */
    {(uint32 *)(uintptr_t)0x17C0000C, 0x00000010}, /* Outgoing interrupt   */
    0,                     /* Incoming interrupt      */
    &xport_smem_qos_config,
    NULL,                  /* Use default Alloc FIFO */
    NULL                   /* Use default clear interrupt */
  },
  
    /* Boot->Mpss */
  {
    "mpss",                /* Remote host name        */
    SMEM_MODEM,            /* Remote host ID          */
    16*1024,               /* Tx FIFO size            */
    4*1024,                /* Tx MTU size             */
    {(uint32 *)(uintptr_t)0x17C0000C, 0x00001000}, /* Outgoing interrupt   */
    0,                     /* Incoming interrupt      */
    &xport_smem_qos_config,
    NULL,                  /* Use default Alloc FIFO */
    NULL                   /* Use default clear interrupt */
  }
};

const uint32 xport_smem_config_num = 4;
const smem_host_type xport_smem_this_host = SMEM_APPS;


const xport_smem_config_type* xport_smem_get_config(uint32 ind)
{
  if (ind >= xport_smem_config_num)
  {
    return NULL;
  }

  return &xport_smem_config[ind];
}
