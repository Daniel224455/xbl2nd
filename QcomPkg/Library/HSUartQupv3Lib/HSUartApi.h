#ifndef HSUART_API_H
#define HSUART_API_H
/*==================================================================================================

FILE: HSUartApi.h

DESCRIPTION: This module provides the driver software for the UART.

Copyright (c) 2018, 2021 Qualcomm Technologies, Inc.
        All Rights Reserved.
Qualcomm Technologies, Inc. Confidential and Proprietary.

==================================================================================================*/
/*==================================================================================================
                                            DESCRIPTION
====================================================================================================

GLOBAL FUNCTIONS:
   hsuart_close
   hsuart_open
   hsuart_receive
   hsuart_transmit
   hsuart_set_baud_rate
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
#include "com_dtypes.h"

/*==================================================================================================
                                             ENUMERATIONS
==================================================================================================*/

typedef enum
{
   HSUART_FIRST_PORT = 0,
   HSUART_DEBUG_PORT = HSUART_FIRST_PORT,

   HSUART_SECOND_PORT,
   HSUART_HS_PORT = HSUART_SECOND_PORT,

   HSUART_MAX_PORTS,
}hsuart_port_id;

typedef enum
{
   HSUART_SUCCESS = 0,
   HSUART_ERROR,
}hsuart_result;

typedef enum
{
  HSUART_5_BITS_PER_CHAR  = 0,
  HSUART_6_BITS_PER_CHAR  = 1,
  HSUART_7_BITS_PER_CHAR  = 2,
  HSUART_8_BITS_PER_CHAR  = 3,
} hsuart_bits_per_char;

typedef enum
{
  HSUART_0_5_STOP_BITS    = 0,
  HSUART_1_0_STOP_BITS    = 1,
  HSUART_1_5_STOP_BITS    = 2,
  HSUART_2_0_STOP_BITS    = 3,
} hsuart_num_stop_bits;

typedef enum
{
  HSUART_NO_PARITY        = 0,
  HSUART_ODD_PARITY       = 1,
  HSUART_EVEN_PARITY      = 2,
  HSUART_SPACE_PARITY     = 3,
} hsuart_parity_mode;

typedef void* hsuart_handle;

typedef void(*HSUART_CALLBACK)(uint32 num_bytes, void *cb_data);

/*==================================================================================================
                                             STRUCTURES
==================================================================================================*/

typedef struct
{
   uint32                baud_rate;
   hsuart_parity_mode      parity_mode;
   hsuart_num_stop_bits    num_stop_bits;
   hsuart_bits_per_char    bits_per_char;
   uint32                enable_loopback;

   // The callbacks will be called from ISR context.
   // Necessary precautions needs to be taken in these funtions to make sure not ISR guidelines
   // are violated.
   // DONT call uart_transmit or uart_receive API from these callbacks.
   HSUART_CALLBACK         tx_cb_isr;
   HSUART_CALLBACK         rx_cb_isr;
}hsuart_open_config;

/*==================================================================================================
                                        FUNCTION PROTOTYPES
==================================================================================================*/


/*!
 * \brief Deinitializes the UART port
 *
 * Releases  clock, interrupt, and gpio handles related to this UART
 * Cancels any pending transfers
 *
 * DON'T call from ISR context
 *
 * \param in uart_handle Handle
 * \return UART_SUCCESS|UART_ERROR
 */

hsuart_result      hsuart_close(hsuart_handle h);

/*!
 * \brief Initializes UART port
 *
 * Opens the UART port and configures the corresponding clocks, interrupts, and gpio
 *
 * DON'T call from ISR context
 *
 * \param in uart_handle* h to get handle
 * \param in uart_port_id id
 * \param in uart_open_config* config structure that holds all config data
 * \return HSUART_SUCCESS|HSUART_ERROR
 */

hsuart_result      hsuart_open(hsuart_handle* h, hsuart_port_id id, hsuart_open_config* config);

/*!
 * \brief Queues the buffer provided for receiving the data
 *
 * Asynchronous call. The rx_cb_isr will be called when the rx transfer completes.
 * The buffer is owned by the UART driver till the rx_cb_isr is called.
 *
 * There has to be always a pending RX. UART HW has a limited buffer(FIFO) and if there is
 * no SW buffer available,
 *    For HS-UART, the flow control will de-assert the RFR line.
 *    For Debug UART, the data will be lost as there is no HW flow control lines available.
 *
 * Call uart_receive immediately after uart_open to queue a buffer.
 * After every rx_cb_isr, from a different non-ISR thread, queue the next transfer.
 *
 * There can be maximum of 2 buffers queued at a time.
 *
 * DON'T call from ISR context
 *
 * \param in hsuart_handle Handle
 * \param in char* buf Buffer to be filled with data.
 * \param in uint32 buf_size Size of the buffer being passed. Must be >=4 and a multiple of 4.
 * \param in void* cb_data Call back data to be passed when rx_cb_isr is called during rx completion
 * \return HSUART_SUCCESS|HSUART_ERROR
 */

hsuart_result      hsuart_receive(hsuart_handle h, char* buf, uint32 buf_size, void* cb_data);

/*!
 * \brief Transmits the data from given buffer.
 *
 * Asynchronous call. The buffer will be queued for TX and when transmit is completed, the
 * tx_cb_isr will be called.
 *
 * The buffer is owned by the UART driver till the tx_cb_isr is called.
 *
 * DON'T call from ISR context
 *
 * There can be maximum of 2 buffers queued at a time.
 *
 * \param in hsuart_handle Handle
 * \param in char* buf Buffer with data for transmit.
 * \param in uint32 bytes_to_tx bytes of data to transmit
 * \param in void* cb_data Call back data to be passed when tx_cb_isr is called during tx completion
 * \return HSUART_SUCCESS|HSUART_ERROR
 */

hsuart_result      hsuart_transmit(hsuart_handle h, char* buf, uint32 bytes_to_tx, void* cb_data);

/*!
 * Updates UART baud rate
 *
 * Baud rate at HW gets updated post the sucess of the call.
 *
 * Baud rate is set at HW
 *
 * DON'T call from ISR context
 *
 *
 * \param in hsuart_handle Handle
 * \param in baud_rate which will be applied at HW
 * \return HSUART_SUCCESS|HSUART_ERROR */
 
hsuart_result hsuart_set_baud_rate(hsuart_handle h, uint32 baud_rate);

#endif
