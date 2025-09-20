#ifndef HSUART_DEFS_H
#define HSUART_DEFS_H
/*==================================================================================================

FILE: HSUartDefs.h

DESCRIPTION: This module provides the driver software for the HS-UART.

Copyright (c) 2018 Qualcomm Technologies, Inc.
        All Rights Reserved.
Qualcomm Technologies, Inc. Confidential and Proprietary.

==================================================================================================*/
/*==================================================================================================
Edit History


when       who     what, where, why
--------   ---     --------------------------------------------------------
11/16/18   NM       Initial revision

==================================================================================================*/
/*==================================================================================================
                                           INCLUDE FILES
==================================================================================================*/
#include "HSUartApi.h"

typedef struct
{
   volatile uint32 uart_base;
   volatile uint32 qup_common_base;
   uint32 gpio_tx_config;
   uint32 gpio_rx_config;
   uint32 gpio_cts_config;
   uint32 gpio_rfr_config;
   uint32 clock_id_index;
   uint8**  common_clock_id;
   uint8**  core_clock_id;
   uint32 base_freq;
   uint32 irq_num;
   uint32 tcsr_base;
   uint32 tcsr_offset;
   uint32 tcsr_value;
}HSUART_PROPERTIES;

typedef struct hsuart_context
{
   uint32               port_id;
   boolean              is_port_open;
   HSUART_PROPERTIES*     properties;
   hsuart_open_config     open_config;

   char*                tx_buf;
   uint32               bytes_to_tx;
   uint32               bytes_txed;
   void*                tx_cb_data;
   volatile void*       tx_signal;


   char*                rx_buf;
   uint32               rx_write_offset;
   uint32               rx_buf_size;
   void*                rx_cb_data;

   void*                clock_handle;
   void*                tlmm_handle;
   void*                interrupt_context;
}hsuart_context;

#endif
