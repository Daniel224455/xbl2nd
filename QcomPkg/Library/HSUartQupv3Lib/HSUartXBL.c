/*==================================================================================================

FILE: HSUart.c

DESCRIPTION: This module provides the driver Software for the UART.

Copyright (c) 2018-2019, 2021 Qualcomm Technologies, Inc.
        All Rights Reserved.
Qualcomm Technologies, Inc. Confidential and Proprietary.

==================================================================================================*/
/*==================================================================================================
                                            DESCRIPTION
====================================================================================================

GLOBAL FUNCTIONS:
   hsuart_close
   hsuart_deinitialize
   hsuart_initialize
   hsuart_open
   hsuart_poll
   hsuart_receive
   hsuart_read
   hsuart_transmit
   hsuart_write

==================================================================================================*/
/*==================================================================================================
Edit History


when       who     what, where, why
--------   ---     --------------------------------------------------------
03/05/21   PCR     Corrected division for uart
01/21/21   PCR     Added changes to support set_baudrate API
04/22/19   NM      Support de-initialize API
11/16/18   NM      Initial revision

==================================================================================================*/

/*==================================================================================================
                                           INCLUDE FILES
==================================================================================================*/
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <Library/IoLib.h>    // MmioWrite32(), etc.
#include "HSUart.h"
#include "HSUartApi.h"
#include "HSUartDefs.h"
#include "HSUartOs.h"
#include "HSUartHal.h"
#include "HSUartHwio.h"
#include "HSUartLog.h"
#include "stringl/stringl.h"
#include "HSUartFWLoad.h"
#include "LoaderUtils.h"
/*==================================================================================================
                                              TYPEDEFS
==================================================================================================*/
typedef struct
{
   uint32 bit_rate;
   uint32 input_freq;
   uint32 divider;
} CLOCK_SETTINGS;

/*==================================================================================================
                                          LOCAL VARIABLES
==================================================================================================*/
#define UART_LOGGING_FILE_ID 10

#define RING_SIZE      256  // size of the ring buffer (must be a power of 2)


hsuart_context static_hsuart_context[HSUART_MAX_PORTS];

static UINT8 ring_buffer[RING_SIZE];  // size must be a power of 2
static UINT32 read_index = 0;   // must be masked before subscripting
static UINT32 write_index = 0;  // must be masked before subscripting
static UINT32 tx_fifo_size = 0; // will be populated in register_init
static hsuart_handle hsuart_port_handle;
static const CLOCK_SETTINGS *entry;

static const CLOCK_SETTINGS baud_table[] =
{
   //  bit_rate,  input_freq, divider
   {        300,    7372800,   24576 },
   {       1200,    7372800,    6144 },
   {       2400,    7372800,    3072 },
   {       4800,    7372800,    1536 },
   {       9600,    7372800,     768 },
   {      19200,    7372800,     384 },
   {      38400,    7372800,     192 },
   {      57600,    7372800,     128 },
   {     115200,    7372800,      64 },
   {     230400,    7372800,      32 },
   {     460800,    7372800,      16 },
   {     921600,   14745600,      16 },
   {    2000000,   32000000,      16 },
   {    3000000,   48000000,      16 },
   {    4000000,   128000000,     32 },
   {0}
};

/*==================================================================================================
                                               MACROS
==================================================================================================*/

// The read/write indices do not wrap around to zero until integer overflow occurs.  This keeps
// the logic simple when determining the number of bytes in the ring buffer.  But they must be
// masked before subscripting and the size of the ring buffer must be a power of 2.

#define BYTES_IN_RING()     ( write_index - read_index )
#define ROOM_IN_RING()      ( RING_SIZE - BYTES_IN_RING() - 1 )  // always leave one empty slot
#define RING_GET_BYTE()     ( ring_buffer[read_index++  & (RING_SIZE - 1)] )
#define RING_PUT_BYTE(data) { ring_buffer[write_index++ & (RING_SIZE - 1)] = data; }


/*==================================================================================================
                                     LOCAL FUNCTION PROTOTYPES
==================================================================================================*/

static void         check_rx_error(hsuart_context* hsuart_ctxt);
static hsuart_result  clock_enable(hsuart_context* hsuart_ctxt);
static void         flush_tx(hsuart_context* hsuart_ctxt);
static void         process_tx_data(hsuart_context* hsuart_ctxt);
static void         receive_data(hsuart_context* hsuart_ctxt);
static void         register_init(hsuart_context* hsuart_ctxt);
static void         rx_transfer_start(hsuart_context* hsuart_ctxt);
static void*        hsuart_isr(void *h);

/*==================================================================================================
                                          LOCAL FUNCTIONS
==================================================================================================*/
/*==================================================================================================

FUNCTION: check_rx_error

DESCRIPTION:

==================================================================================================*/
static void check_rx_error(hsuart_context* hsuart_ctxt)
{
   return;
}

/*==================================================================================================

FUNCTION: clock_enable

DESCRIPTION:

==================================================================================================*/
static hsuart_result clock_enable(hsuart_context* hsuart_ctxt)
{
   uint32 input_freq;
   uint32 divider;
   uint32 bit_rate;

   bit_rate = hsuart_ctxt->open_config.baud_rate;
   for (entry = baud_table; entry->bit_rate; entry++)
   {
      if ( entry->bit_rate == bit_rate ) { break; }
   }

   if (entry->bit_rate == 0)
   {
      return HSUART_ERROR;
   }
   input_freq = entry->input_freq;
   divider    = entry->divider;

   if( hsuart_clock_open(hsuart_ctxt, input_freq)== HSUART_ERROR)
   {
      return HSUART_ERROR;
   }

   return HSUART_SUCCESS;
}

/*==================================================================================================

FUNCTION: flush_tx

DESCRIPTION:

==================================================================================================*/
static void flush_tx(hsuart_context* hsuart_ctxt)
{
   return;
}

/*==================================================================================================

FUNCTION: write_to_fifo

DESCRIPTION:

==================================================================================================*/
static uint32 write_to_fifo(volatile uint32 base, char* ptr, uint32 num_bytes)
{
   uint32 full_words = num_bytes >> 2;
   uint32 partial_bytes = num_bytes & 0x03;
   uint32 empty_slots;
   uint32 words_in_fifo;
   uint32 tx_fifo_size;
   uint32 word_value;
   uint32 words_to_send;
   uint32 bytes_to_send;
   uint32 i;

   words_in_fifo = REG_IN(base + GENI4_DATA, GENI_TX_FIFO_STATUS) & TX_FIFO_WC;

   tx_fifo_size = (REG_IN(base + QUPV3_SE_DMA, SE_HW_PARAM_0) & TX_FIFO_DEPTH_MASK) >> TX_FIFO_DEPTH_SHIFT;

   empty_slots = tx_fifo_size - words_in_fifo;

   words_to_send = (full_words >  empty_slots) ? (empty_slots) : (full_words);
   bytes_to_send = (full_words >= empty_slots) ? (0) : (partial_bytes);

   if ( (ptr - (char *)NULL) & 0x03 )  // buffer is not 32-bit word aligned
   {
      for (i = 0; i < words_to_send; i++)
      {
         word_value = ptr[0] << 0 | ptr[1] << 8 | ptr[2] << 16 | ptr[3] << 24;
         REG_OUTI(base + GENI4_DATA, GENI_TX_FIFOn, 0, word_value);
         ptr += 4;
      }
   }
   else  // buffer is 32-bit word aligned
   {
      for (i = 0; i < words_to_send; i++)
      {
         word_value = *(uint32 *)ptr;
         REG_OUTI(base + GENI4_DATA, GENI_TX_FIFOn, 0, word_value);
         ptr += 4;
      }
   }

   if (bytes_to_send)
   {
      word_value = 0;
      for (i = 0; i < bytes_to_send; i++) { word_value |= ptr[i] << i * 8; }
      REG_OUTI(base + GENI4_DATA, GENI_TX_FIFOn, 0, word_value);
   }

   return(words_to_send * 4 + bytes_to_send);

}
/*==================================================================================================

FUNCTION: process_tx_data

DESCRIPTION: This function calls the HAL layer to transmit data and also does polling for TXLEV
             and calculates the wait time.

==================================================================================================*/

static void process_tx_data(hsuart_context* hsuart_ctxt)
{
   uint32 num_bytes = hsuart_ctxt->bytes_to_tx - hsuart_ctxt->bytes_txed;
   char* buf = (hsuart_ctxt->tx_buf)+(hsuart_ctxt->bytes_txed);

   volatile uint32 base = hsuart_ctxt->properties->uart_base;

   if (num_bytes)
   {
       hsuart_ctxt->bytes_txed += write_to_fifo(base, buf, num_bytes);
       if (hsuart_ctxt->bytes_txed == hsuart_ctxt->bytes_to_tx)
       {
           REG_OUT(base + GENI4_DATA, GENI_M_IRQ_EN_CLEAR, TF_FIFO_WATERMARK_IRQ);
       }
       else
       {
           REG_OUT(base + GENI4_DATA, GENI_M_IRQ_EN_SET, TF_FIFO_WATERMARK_IRQ);
       }
   }
}

/*==================================================================================================

FUNCTION: receive_data

DESCRIPTION:

==================================================================================================*/
static void receive_data(hsuart_context* hsuart_ctxt)
{
   volatile uint32  base = hsuart_ctxt->properties->uart_base;
   char*   buf = hsuart_ctxt->rx_buf;
   uint32  buf_size = hsuart_ctxt->rx_buf_size;
   uint32  rx_fifo_status;
   uint32  partial_bytes_to_read = 0;
   uint32  words_to_read = 0;
   uint32  rx_word;
   uint32  s_irq_status;
   uint32  is_stale = FALSE;
   uint32  avail_bytes;
   uint32  i;

   if (buf == NULL)
   {
      return;
   }

   s_irq_status = REG_IN(base + GENI4_DATA, GENI_S_IRQ_STATUS);
   rx_fifo_status = REG_IN(base + GENI4_DATA, GENI_RX_FIFO_STATUS);


   if (s_irq_status & (RX_LAST_IRQ | RX_FIFO_WATERMARK_IRQ) )
   {
      partial_bytes_to_read = (rx_fifo_status & RX_LAST_VALID_BYTES_MASK) >> RX_LAST_VALID_BYTES_SHIFT;
      words_to_read = rx_fifo_status & RX_FIFO_WC;
      if (partial_bytes_to_read != 0 && partial_bytes_to_read != 4)
      {
         words_to_read -= 1;
      }
      else
      {
         partial_bytes_to_read = 0;
      }
      if (s_irq_status & RX_LAST_IRQ)
      {
         is_stale = TRUE;
      }
   }
   avail_bytes = (words_to_read * 4) + partial_bytes_to_read;

   // If the buffer is full, callback the client and disable the interrupts till a new buffer is availble
   // through uart_receive function.

   if ((hsuart_ctxt->rx_write_offset + avail_bytes) > buf_size)
   {
      if (buf_size - hsuart_ctxt->rx_write_offset < 4)
      {
         if (hsuart_ctxt->open_config.rx_cb_isr)
         {
            hsuart_ctxt->open_config.rx_cb_isr(hsuart_ctxt->rx_write_offset, hsuart_ctxt->rx_cb_data);
         }

          // Clear the interrupts
          REG_OUT(base + GENI4_DATA, GENI_S_IRQ_CLEAR, s_irq_status);
          // Disable the fifo wm and stale interrupts
          REG_OUT(base + GENI4_DATA, GENI_S_IRQ_EN_CLEAR, RX_LAST_IRQ | RX_FIFO_WATERMARK_IRQ);
          return;
      }
      else
      {
          words_to_read = ((buf_size - hsuart_ctxt->rx_write_offset) & 0x3 ) >> 2;
          partial_bytes_to_read = 0;
          avail_bytes = (words_to_read * 4) + partial_bytes_to_read;
      }
   }
   else
   {

      // If you are definitely going to read till the LAST byte, clear the interrupt.
      // The RX LAST STATUS is not a level source to the secondary engine interrupt register
      // This will result in RX getting stuck as clearing the interrupt without any reading
      // the last byte will cause the interrupt to be permanently lost.

      REG_OUT(base + GENI4_DATA, GENI_S_IRQ_CLEAR, (s_irq_status & RX_LAST_IRQ));
   }

   buf += hsuart_ctxt->rx_write_offset;

   if ( (buf - (char *)NULL) & 0x03 )  // buffer is not 32-bit word aligned
   {
      for (i = 0; i < words_to_read; i++)
      {
         rx_word = REG_INI(base + GENI4_DATA, GENI_RX_FIFOn, 0);
         buf[0] = (uint8)(rx_word >>  0);
         buf[1] = (uint8)(rx_word >>  8);
         buf[2] = (uint8)(rx_word >> 16);
         buf[3] = (uint8)(rx_word >> 24);
         buf += 4;
      }
   }
   else  // buffer is 32-bit word aligned
   {
      for (i = 0; i < words_to_read; i++)
      {
         rx_word = REG_INI(base + GENI4_DATA, GENI_RX_FIFOn, 0);
         *(uint32 *)buf = rx_word;
         buf += 4;
      }
   }

   if (partial_bytes_to_read)
   {
      rx_word = REG_INI(base + GENI4_DATA, GENI_RX_FIFOn, 0);
      for (i = 0; i < partial_bytes_to_read; i++) { buf[i] = (uint8)(rx_word >> i * 8); }
   }

   hsuart_ctxt->rx_write_offset += avail_bytes;

   if (hsuart_ctxt->open_config.rx_cb_isr)
   {
      // If it is stale interrupt( end of rx transfer ) call the client callback
      if (is_stale)
      {
         // Disable the fifo wm and stale interrupts since the current buffer is complete
         // When the client queues the next buffer, the uart_receive function will automatically
         // enable the interrupts

         REG_OUT(base + GENI4_DATA, GENI_S_IRQ_EN_CLEAR, RX_LAST_IRQ | RX_FIFO_WATERMARK_IRQ);
         hsuart_ctxt->open_config.rx_cb_isr(hsuart_ctxt->rx_write_offset, hsuart_ctxt->rx_cb_data);
      }
   }
   REG_OUT(base + GENI4_DATA, GENI_S_IRQ_CLEAR, (s_irq_status & RX_FIFO_WATERMARK_IRQ));

}

/*==================================================================================================

FUNCTION: reginit

DESCRIPTION:

==================================================================================================*/
void register_init(hsuart_context* hsuart_ctxt)
{
   volatile uint32 base = hsuart_ctxt->properties->uart_base;
   uint32 temp;

   temp = REG_IN(base + QUPV3_SE_DMA, SE_HW_PARAM_0); // to get TXFIFO size

   tx_fifo_size = ((temp & TX_FIFO_DEPTH_MASK) >> TX_FIFO_DEPTH_SHIFT << 2); // It will be in words.
                                                                             // so << 2.

   REG_OUT(base + GENI4_DATA, GENI_TX_WATERMARK_REG, 4);

   temp = REG_IN(base + QUPV3_SE_DMA, SE_HW_PARAM_1);// to get RXFIFO width

   temp = (temp & RX_FIFO_DEPTH_MASK) >> RX_FIFO_DEPTH_SHIFT;

   REG_OUT(base + GENI4_DATA, GENI_RX_WATERMARK_REG, temp - 8);

   REG_OUT(base + GENI4_DATA, GENI_RX_RFR_WATERMARK_REG, temp - 4);


   REG_OUT(base + GENI4_CFG, GENI_SER_M_CLK_CFG, 0x1|(entry->divider/2));

   REG_OUT(base + GENI4_CFG, GENI_SER_S_CLK_CFG, 0x1|(entry->divider/2));

   if (hsuart_ctxt->open_config.enable_loopback)
   {
      REG_OUT(base + GENI4_IMAGE_REGS, UART_LOOPBACK_CFG, 0x3);
   }

   REG_OUT(base + GENI4_IMAGE_REGS, UART_TX_WORD_LEN, 0x8);

   REG_OUT(base + GENI4_IMAGE_REGS, UART_RX_WORD_LEN, 0x8);

   REG_OUT(base + GENI4_IMAGE_REGS, UART_TX_PARITY_CFG, 0x0);
   
   if (!hsuart_ctxt->properties->gpio_cts_config)
   {
      REG_OUT(base + GENI4_IMAGE_REGS, UART_TX_TRANS_CFG, 0x2);
   }

   REG_OUT(base + GENI4_IMAGE_REGS, UART_RX_PARITY_CFG, 0x0);
   REG_OUT(base + GENI4_IMAGE_REGS, UART_RX_TRANS_CFG, 0x0);
   
   if (hsuart_ctxt->properties->gpio_rfr_config)
   {
      REG_OUT(base + GENI4_IMAGE_REGS, UART_MANUAL_RFR, 0x1);
   }

   REG_OUT(base + GENI4_IMAGE_REGS, UART_TX_STOP_BIT_LEN, 0x0);

   REG_OUT(base + GENI4_IMAGE_REGS, UART_RX_STALE_CNT, 0x16*10);

   REG_OUT(base + GENI4_IMAGE_REGS, GENI_TX_PACKING_CFG0, 0x4380E);
   REG_OUT(base + GENI4_IMAGE_REGS, GENI_TX_PACKING_CFG1, 0xC3E0E);

   REG_OUT(base + GENI4_IMAGE_REGS, GENI_RX_PACKING_CFG0, 0x4380E);
   REG_OUT(base + GENI4_IMAGE_REGS, GENI_RX_PACKING_CFG1, 0xC3E0E);

   rx_transfer_start(hsuart_ctxt);

}


/*==================================================================================================

FUNCTION: rx_transfer_start

DESCRIPTION:
   This function starts a new RX transfer.

==================================================================================================*/
static void rx_transfer_start(hsuart_context* hsuart_ctxt)
{
   volatile uint32 base = hsuart_ctxt->properties->uart_base;

   REG_OUT(base + GENI4_DATA, GENI_S_CMD0, 0x8000000);
}

/*==================================================================================================

FUNCTION: hsuart_close

DESCRIPTION:

==================================================================================================*/
hsuart_result hsuart_close(hsuart_handle h)
{
   hsuart_context* hsuart_ctxt;
   hsuart_result    result = HSUART_SUCCESS;

   HSUART_LOG_0(INFO,"+uart_close");

   if (NULL == h)
   {
      HSUART_LOG_0(ERROR,"Calling uart_close with a NULL handle.");
      return HSUART_ERROR;
   }

   hsuart_ctxt = (hsuart_context*)h;
   if (FALSE == hsuart_ctxt->is_port_open)
   {
      HSUART_LOG_0(ERROR,"Calling uart_close on a closed port.");
      return HSUART_ERROR;
   }

   flush_tx(hsuart_ctxt);

   hsuart_ctxt->is_port_open = FALSE;


   if(HSUART_SUCCESS != hsuart_clock_close(hsuart_ctxt))
   {
      HSUART_LOG_0(ERROR, "Uart_clock_close failed.");
      result = HSUART_ERROR;
   }

   if(HSUART_SUCCESS != hsuart_tlmm_close(hsuart_ctxt))
   {
      HSUART_LOG_0(ERROR, "Uart_tlmm_close failed.");
      result = HSUART_ERROR;
   }
   if(HSUART_SUCCESS != hsuart_interrupt_close(hsuart_ctxt))
   {
      HSUART_LOG_0(ERROR, "Uart_interrupt_close failed.");
      result = HSUART_ERROR;
   }

   HSUART_LOG_0(INFO,"-uart_close");
   return result;
}


/*==================================================================================================

FUNCTION: hsuart_open

DESCRIPTION:

==================================================================================================*/
hsuart_result hsuart_open(hsuart_handle* h, hsuart_port_id id, hsuart_open_config* config)
{
   hsuart_context* hsuart_ctxt;

   HSUART_LOG_0(INFO,"+uart_open");

   if (NULL == h)
   {
      HSUART_LOG_0(ERROR, "Calling uart_open with NULL handle pointer.");
      return HSUART_ERROR;
   }

   *h = NULL;

   if (id < HSUART_MAX_PORTS)
   {
      hsuart_ctxt = &static_hsuart_context[id];

      if (hsuart_ctxt->is_port_open == TRUE)
      {
          return HSUART_ERROR;
      }

      hsuart_ctxt->port_id = id;
   }
   else
   {
      HSUART_LOG_1(ERROR, "Invalid Port ID. Port: %d", id);
      return HSUART_ERROR;
   }

   memscpy((void*)&hsuart_ctxt->open_config, sizeof(hsuart_open_config), (void*) config, sizeof(hsuart_open_config));

   if(HSUART_SUCCESS != hsuart_get_properties(hsuart_ctxt))
   {
      HSUART_LOG_0(ERROR, "uart_get_properties failed.");
      return HSUART_ERROR;
   }

   if(HSUART_SUCCESS != clock_enable(hsuart_ctxt))
   {
      HSUART_LOG_0(ERROR, "clock_enable failed.");
      return HSUART_ERROR;
   }

   if(HSUART_SUCCESS != hsuart_fw_load(hsuart_ctxt))
   {
      HSUART_LOG_0(ERROR, "hsuart_fw_load failed.");
      return HSUART_ERROR;
   }

   if(HSUART_SUCCESS != hsuart_tlmm_open(hsuart_ctxt))
   {
      HSUART_LOG_0(ERROR, "hsuart_tlmm_open failed.");
      hsuart_close((hsuart_handle)hsuart_ctxt);
      return HSUART_ERROR;
   }

   if(HSUART_SUCCESS != hsuart_interrupt_open(hsuart_ctxt,(void*)hsuart_isr))
   {
      HSUART_LOG_0(ERROR, "hsuart_interrupt_open failed.");
      hsuart_close((hsuart_handle)hsuart_ctxt);
      return HSUART_ERROR;
   }

   register_init(hsuart_ctxt);

   *h = (hsuart_handle)hsuart_ctxt;
   hsuart_ctxt->is_port_open = TRUE;

   HSUART_LOG_0(INFO,"-uart_open");
   return HSUART_SUCCESS;
}


/*==================================================================================================

FUNCTION: hsuart_isr

DESCRIPTION:

==================================================================================================*/
static void *hsuart_isr(void *h)
{
   hsuart_context *hsuart_ctxt = (hsuart_context *)h;
   volatile uint32        base = hsuart_ctxt->properties->uart_base;
   uint32        m_irq_status = 0;

   HSUART_LOG_0(INFO,"+uart_isr");

   m_irq_status = REG_IN(base + GENI4_DATA, GENI_M_IRQ_STATUS);

   REG_OUT(base + GENI4_DATA, GENI_M_IRQ_CLEAR, m_irq_status);
   if (m_irq_status & TF_FIFO_WATERMARK_IRQ)
   {
      process_tx_data(hsuart_ctxt);
   }

   if (m_irq_status & M_CMD_DONE_IRQ)
   {
      if (hsuart_ctxt->open_config.tx_cb_isr == NULL)
      {
         //uart_os_signal_set(&hsuart_ctxt->tx_signal);
      }
      else
      {
         hsuart_ctxt->open_config.tx_cb_isr(hsuart_ctxt->bytes_txed, hsuart_ctxt->tx_cb_data);
      }
      hsuart_ctxt->tx_buf = NULL;
   }

   if ( hsuart_ctxt->open_config.rx_cb_isr && (m_irq_status & SEC_IRQ))
   {
      receive_data(hsuart_ctxt);
   }

   REG_OUT(base + GENI4_DATA, GENI_M_IRQ_CLEAR, m_irq_status);
   return NULL;
}

/*==================================================================================================

FUNCTION: hsuart_receive

DESCRIPTION:

==================================================================================================*/
hsuart_result hsuart_receive(hsuart_handle h, char* buf, uint32 buf_size, void* cb_data)
{
   hsuart_context* hsuart_ctxt;
   volatile uint32 base;

   HSUART_LOG_0(INFO,"+uart_receive");

   if (NULL == h)
   {
      HSUART_LOG_0(ERROR,"Calling hsuart_receive with a NULL handle.");
      return 0;
   }

   if (NULL == buf)
   {
      HSUART_LOG_0(ERROR,"Calling hsuart_receive with a NULL buffer.");
      return 0;
   }

   if (0 == buf_size)          { return HSUART_ERROR; }

   hsuart_ctxt = (hsuart_context*)h;

   base = hsuart_ctxt->properties->uart_base;
   if (FALSE == hsuart_ctxt->is_port_open)
   {
      HSUART_LOG_0(ERROR,"Calling uart_receive on a closed port.");
      return 0;
   }

   hsuart_ctxt->rx_buf = buf;
   hsuart_ctxt->rx_buf_size = buf_size;
   hsuart_ctxt->rx_cb_data = cb_data;
   hsuart_ctxt->rx_write_offset = 0;

   // Enable the receive interrupts only after receiving a buffer from client.
   // if not there is nothing to process when interrupt fires and we will end in interrupt storm

   REG_OUT(base + GENI4_DATA, GENI_S_IRQ_EN_SET, RX_LAST_IRQ | RX_FIFO_WATERMARK_IRQ);

   if (hsuart_ctxt->open_config.rx_cb_isr == NULL)
   {
      receive_data(hsuart_ctxt);

      /* Override cb_data as bytes received in case of polling option */
      if (cb_data != NULL)
      {
         *(uint32*)cb_data = hsuart_ctxt->rx_write_offset;
      }

      hsuart_ctxt->rx_buf = NULL;
      hsuart_ctxt->rx_buf_size = 0;
      hsuart_ctxt->rx_cb_data = NULL;
      hsuart_ctxt->rx_write_offset = 0;
   }

   HSUART_LOG_0(INFO,"-uart_receive");

   return HSUART_SUCCESS;
}


/*==================================================================================================

FUNCTION: hsuart_transmit

DESCRIPTION:

==================================================================================================*/
hsuart_result hsuart_transmit(hsuart_handle h, char* buf, uint32 bytes_to_tx, void* cb_data)
{
   hsuart_context* hsuart_ctxt;
   volatile uint32 base;

   HSUART_LOG_0(INFO,"+hsuart_transmit");

   if (NULL == h)
   {
      HSUART_LOG_0(ERROR,"Calling uart_transmit with a NULL handle.");
      return 0;
   }

   if (NULL == buf)
   {
      HSUART_LOG_0(ERROR,"Calling uart_transmit with a NULL buffer.");
      return 0;
   }

   if (0 == bytes_to_tx)            { return HSUART_SUCCESS; }

   hsuart_ctxt = (hsuart_context*)h;
   base = hsuart_ctxt->properties->uart_base;

   if (FALSE == hsuart_ctxt->is_port_open)
   {
      HSUART_LOG_0(ERROR,"Calling uart_transmit on a closed port.");
      return 0;
   }

   if (REG_IN(base + GENI4_CFG, GENI_STATUS) & M_GENI_CMD_ACTIVE)
   {
      if ((hsuart_ctxt->open_config.tx_cb_isr == NULL) && (cb_data != NULL))
      {
            *(uint32*)cb_data = 0;
           return HSUART_ERROR;
      }
   }

   hsuart_ctxt->tx_buf = buf;
   hsuart_ctxt->bytes_to_tx = bytes_to_tx;
   hsuart_ctxt->tx_cb_data = cb_data;
   hsuart_ctxt->bytes_txed = 0;

   // Start the TX state machine
   REG_OUT(base + GENI4_IMAGE_REGS, UART_TX_TRANS_LEN, hsuart_ctxt->bytes_to_tx);
   REG_OUT(base + GENI4_DATA, GENI_M_CMD0, 0x08000000);

   process_tx_data(hsuart_ctxt);

   if (cb_data != NULL)
   {
      *(uint32*)cb_data = hsuart_ctxt->bytes_to_tx;
   }

   HSUART_LOG_0(INFO,"-hsuart_transmit");

   return HSUART_SUCCESS;
}

/*==================================================================================================

FUNCTION: hsuart_deinitialize

DESCRIPTION:

==================================================================================================*/
RETURN_STATUS EFIAPI
hsuart_deinitialize(void)
{
   hsuart_result result;
   
   result = hsuart_close(hsuart_port_handle);
   
   if (result == HSUART_SUCCESS)
   {
       return RETURN_SUCCESS;
   }
   else
   {
       return RETURN_DEVICE_ERROR;
   }
}

/*==================================================================================================

FUNCTION: hsuart_initialize

DESCRIPTION:

==================================================================================================*/
RETURN_STATUS EFIAPI
hsuart_initialize(void)
{
   hsuart_open_config c;

   c.baud_rate = 4000000;
   c.parity_mode = HSUART_NO_PARITY;
   c.num_stop_bits = HSUART_1_0_STOP_BITS;
   c.bits_per_char = HSUART_8_BITS_PER_CHAR;
   c.enable_loopback = FALSE;
   c.tx_cb_isr = NULL;
   c.rx_cb_isr = NULL;

   hsuart_open(&hsuart_port_handle, HSUART_DEBUG_PORT, &c);

   return RETURN_SUCCESS;
}

/*==================================================================================================

FUNCTION: hsuart_poll

DESCRIPTION:

==================================================================================================*/
BOOLEAN EFIAPI
hsuart_poll(void)
{
   volatile uint32  base = ((hsuart_context*)hsuart_port_handle)->properties->uart_base;
 //  if (props == NULL) { props = get_properties(); }
  // if (props == NULL) { return FALSE; }

  // service_rx_fifo();
   uint32 s_irq_status = REG_IN(base + GENI4_DATA, GENI_S_IRQ_STATUS);

   if( (s_irq_status & (RX_LAST_IRQ | RX_FIFO_WATERMARK_IRQ)) || ( BYTES_IN_RING() > 0 ) )
   {
      return  1;
   }
   return 0;
}
/*==================================================================================================

FUNCTION: hsuart_read

DESCRIPTION:

==================================================================================================*/
UINTN EFIAPI
hsuart_read(OUT UINT8 *user_buffer, IN UINTN bytes_requested)
{
   UINT8 *ptr = user_buffer;
   UINT32 room_avail = bytes_requested;
   UINT32 bytes_copied = 0;
   UINT32 bytes_read = 0;
   UINT32 bytes_avail = 0;
   UINT32 num_bytes;
   UINT32 i;

   //if (props == NULL) { props = get_properties(); }
   //if (props == NULL) { return 0; }

   while (1)  // loop until user buffer is full or no more bytes are available
   {
      if (read_index == write_index)
      {
         read_index = write_index = 0;
      }
      if (RING_SIZE - write_index > 4)
      {
         hsuart_receive(hsuart_port_handle, (char*)(ring_buffer + write_index), RING_SIZE - write_index, &bytes_read);
         write_index += bytes_read;
      }
      if(read_index < write_index)
      {
         bytes_avail = write_index - read_index;
      }
      else
      {
         bytes_avail = 0;
      }


      num_bytes = (room_avail < bytes_avail) ? (room_avail) : (bytes_avail);
      if (num_bytes == 0) { break; }
      for (i = 0; i < num_bytes; i++) { *ptr++ = RING_GET_BYTE(); }
      bytes_copied += num_bytes;
      room_avail -= num_bytes;
   }

   return(bytes_copied);

}

/*==================================================================================================

FUNCTION: hsuart_write

DESCRIPTION:

==================================================================================================*/
UINTN EFIAPI
hsuart_write(IN UINT8 *user_buffer, IN UINTN bytes_to_send)
{
   volatile uint32  base = ((hsuart_context*)hsuart_port_handle)->properties->uart_base;
   UINT32 bytes_sent = 0;
   UINT32 total_bytes_sent = 0;
   INT64 total_bytes_to_send = bytes_to_send;
   uint32 temp;

   if (tx_fifo_size == 0)
   {
      temp = REG_IN(base + QUPV3_SE_DMA, SE_HW_PARAM_0); // to get TXFIFO size

      tx_fifo_size = ((temp & TX_FIFO_DEPTH_MASK) >> TX_FIFO_DEPTH_SHIFT << 2); // It will be in words.
                                                                                // so << 2.
   }
   while (total_bytes_to_send > 0)
   {
      bytes_to_send = (bytes_to_send > tx_fifo_size) ? tx_fifo_size : bytes_to_send;
      hsuart_transmit(hsuart_port_handle, (char *)user_buffer, bytes_to_send, &bytes_sent);
      total_bytes_to_send -= bytes_sent;
      total_bytes_sent += bytes_sent;
   }
   return(total_bytes_sent);
}

/*==================================================================================================

FUNCTION: hsuart_is_cable_connected

DESCRIPTION:

==================================================================================================*/
BOOLEAN EFIAPI
hsuart_is_cable_connected(void)
{
   volatile uint32  base = ((hsuart_context*)hsuart_port_handle)->properties->uart_base;
   uint32 s_irq_status = REG_IN(base + GENI4_DATA, GENI_S_IRQ_STATUS);

   if ((s_irq_status & RX_BREAK_START) && !(s_irq_status & RX_BREAK_END))
   {
      return FALSE;
   }

   return TRUE;
}

/*==================================================================================================

FUNCTION: hsuart_power_off

DESCRIPTION:

==================================================================================================*/
BOOLEAN EFIAPI
hsuart_power_off(void)
{
   volatile uint32  base = ((hsuart_context*)hsuart_port_handle)->properties->uart_base;
   uint32 rx_irq_status;
   uint32 rx_discard_word;
   uint32 word_cnt;
   uint32 poll_cnt = 0;

   if(REG_IN(base + GENI4_CFG, GENI_STATUS) & 0x41000)
   {
      REG_OUT(base + GENI4_DATA, GENI_S_CMD_CTRL_REG, 0x4);

      do
      {
         rx_irq_status = REG_IN(base + GENI4_DATA, GENI_S_IRQ_STATUS);
         if(rx_irq_status & RX_LAST_IRQ)
         {
            REG_OUT(base + GENI4_DATA, GENI_S_IRQ_CLEAR, RX_LAST_IRQ );
            word_cnt = REG_IN(base + GENI4_DATA, GENI_RX_FIFO_STATUS) & RX_FIFO_WC;

            while(word_cnt--)
            {
              // uart_cancel_dummy_word_cnt++;  // Spurious bytes
               rx_discard_word = REG_INI(base + GENI4_DATA, GENI_RX_FIFOn, 0);
            }
         }

         if(rx_irq_status & S_CMD_CANCEL_IRQ)
         {
            REG_OUT(base + GENI4_DATA, GENI_S_IRQ_CLEAR, S_CMD_CANCEL_IRQ | S_CMD_DONE_IRQ);
            break;
         }
      }while(poll_cnt++ < 1000);
   }

   // Wait till all data from TX is flushed.
   while(REG_IN(base + GENI4_CFG, GENI_STATUS) & M_GENI_CMD_ACTIVE);

   return TRUE;
}

/*==================================================================================================

FUNCTION: hsuart_power_oon

DESCRIPTION:

==================================================================================================*/
BOOLEAN EFIAPI
hsuart_power_on(void)
{
   rx_transfer_start((hsuart_context*)hsuart_port_handle);
   return TRUE;
}

/*==================================================================================================

FUNCTION: hsuart_get_clock_param

DESCRIPTION:

==================================================================================================*/

static void hsuart_get_clock_param(uint32 bit_rate, uint32* freq, uint32* div)
{
   uint8 i=0;
   while (1)
   {
      if (bit_rate == baud_table[i].bit_rate)
      {
         *freq = baud_table[i].input_freq;
         *div = baud_table[i].divider;
         return;
      }
      if (baud_table[i].bit_rate == 0)
      {
         *freq = 0;
         *div = 0;
         return;
      }
      i++;
   }
}

/*==================================================================================================

FUNCTION: hsuart_get_clock_param

DESCRIPTION:

==================================================================================================*/

RETURN_STATUS EFIAPI
hsuart_set_baudrate(IN UINTN baud_rate)
{
   hsuart_result result;

   result = hsuart_set_baud_rate(hsuart_port_handle, baud_rate);

   if (result == HSUART_SUCCESS)
   {
       return RETURN_SUCCESS;
   }
   else
   {
       return RETURN_DEVICE_ERROR;
   }
}

/*==================================================================================================

FUNCTION: hsuart_set_baud_rate

DESCRIPTION:

==================================================================================================*/
hsuart_result hsuart_set_baud_rate(hsuart_handle h, uint32 baud_rate)
{
   volatile uint32 base = ((hsuart_context*)hsuart_port_handle)->properties->uart_base;
   uint32 reg_value;
   uint32 freq;
   uint32 div;
   RETURN_STATUS status = HSUART_SUCCESS;
   hsuart_context* hsuart_ctxt;
   uint32 old_baudrate;

   if (NULL == h)
   {
      DEBUG((EFI_D_ERROR,"Calling hsuart_set_baud_rate with a NULL handle."));
      return HSUART_ERROR;
   }

   hsuart_ctxt = (hsuart_context*)h;

   hsuart_get_clock_param(baud_rate, &freq, &div);

   if (!freq || !div )
   {
      status =  HSUART_ERROR;
	  goto exit;
   }
   
   // Once the baud rate is confirmed to be valid, stop/abort the RX engine
   REG_OUT(base + GENI4_DATA, GENI_S_CMD_CTRL_REG, 0x2);

   while ( (REG_IN(base + GENI4_DATA, GENI_S_CMD_CTRL_REG) & 0x2) );

   REG_OUT(base + GENI4_DATA, GENI_S_IRQ_CLEAR, 0xFFFFFFF);

   old_baudrate = hsuart_ctxt->open_config.baud_rate;

   // Update HSUart cotext with new baudrate
   hsuart_ctxt->open_config.baud_rate = baud_rate;

   if(HSUART_SUCCESS != clock_enable(hsuart_ctxt))
   {
      DEBUG((EFI_D_ERROR, "clock_enable failed."));
      status =  HSUART_ERROR;
      hsuart_ctxt->open_config.baud_rate = old_baudrate;
      goto exit;
   }

   reg_value = ((div/32) << 0x4) | 0x1;
   REG_OUT(base + GENI4_CFG, GENI_SER_M_CLK_CFG, reg_value);

   REG_OUT(base + GENI4_CFG, GENI_SER_S_CLK_CFG, reg_value);

exit:
   rx_transfer_start(hsuart_ctxt);
   return status;
}
