/**************************************************************************
 * FILE: fh_transport_hsuart.c
 *
 * Implementation of the HSUART port transport layer.
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

  $Header: //components/rel/boot.xf/3.0.1.c1/QcomPkg/Tools/storage/fh_loader/fh_loader/fh_transport_hsuart.c#1 $
  $DateTime: 2021/04/29 02:01:56 $
  $Author: pwbldsvc $

when         who   what, where, why
----------   ---   ---------------------------------------------------------
2019-08-05   gp    Change of transport layer
2019-05-29   wek   Misc bug fixes.
2019-02-27   gp    Create.

===========================================================================*/

#include "fh_comdef.h"
#include "fh_hsuart_packet.h"
#include "fh_packet_id.h"
#include "fh_crc.h"
#include "fh_log.h"
#include "fh_transport.h"
#include "stringl.h"
#include <string.h>

#define BAUDRATE 3000000

#define READ_TIMEOUT_SECONDS 10

#define END_OF_PACKET 0x00
#define MAX_HSUART_PACKET_DATA_SIZE 4000

#define ONE_MEGA_BYTE (1024*1024)

static bool fh_transport_hsuart_is_target_in_read_mode = FALSE;
static uint32 fh_transport_hsuart_buffered_length = 0;
static char fh_transport_hsuart_buffered_data[ONE_MEGA_BYTE] = { 0 };
static char *fh_transport_hsuart_buffered_data_end_ptr =
  fh_transport_hsuart_buffered_data;
static char *fh_transport_hsuart_buffered_data_start_ptr =
  fh_transport_hsuart_buffered_data;

static bool send_nak(void);

static bool send_ack(void);

static bool wait_for_ack(void);

static bool read_hsuart_packet(unsigned char *buf, uint32* length,
                               bool* end_of_transfer);

static bool write_hsuart_packet(unsigned char *buf, uint32 length,
                                enum fh_packet_id_t packet_id);

static bool transmit_hsuart_packet(unsigned char *buf, uint32 length);

static bool receive_hsuart_packet(unsigned char *buf, uint32* length,
                                  bool waiting_for_ack);

static bool wait_for_ready_to_read_packet(void);

static uint32 get_data_from_buffer(unsigned char *buffer,
                                   uint32 requested_length);

static bool add_data_to_buffer(unsigned char *buffer, uint32 length);

static uint32 fh_transport_hsuart_read(void *read_buffer, uint32 read_len,
                                       uint32 *bytes_read);

#ifdef _MSC_VER /* Compiling under Windows */

#include <windows.h>
#include <time.h>
#include <strsafe.h>

#define sleep(x) Sleep(1000*x)

static HANDLE hsuart_port;


/******************************************************************************
 * Name: print_get_last_error
 *
 * Description:
 * This function prints the GetLastError and its message
 *
 *****************************************************************************/
static void print_get_last_error()
{
  LPVOID lpMsgBuf;
  DWORD dw = GetLastError();

  FormatMessage(
    FORMAT_MESSAGE_ALLOCATE_BUFFER |
    FORMAT_MESSAGE_FROM_SYSTEM |
    FORMAT_MESSAGE_IGNORE_INSERTS,
    NULL,
    dw,
    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
    (LPTSTR)& lpMsgBuf,
    0, NULL);

  FH_LOGE("GetLastError:%ld %s", dw, ((LPCTSTR)lpMsgBuf));
}

/******************************************************************************
 * Name: set_port_timeouts
 *
 * Description:
 * This function sets timeout settings to the provided COMM PORT
 *
 * Arguments:
 *    port      [IN]: COMM port
 *
 * Returns:
 *  TRUE or FALSE depending on successful operation
 *
 *****************************************************************************/
bool set_port_timeouts(void)
{
  COMMTIMEOUTS timeouts;
  /* Maximum number of milliseconds that can elapse between two characters
     without a timeout occurring                                           */
  timeouts.ReadIntervalTimeout = 20;
  /* For each read operation, this number is multiplied by the number of
     bytes that the read operation expects to receive.                     */
  timeouts.ReadTotalTimeoutMultiplier = 0;
  timeouts.ReadTotalTimeoutConstant = 100; /* Actual timeout in ms */
  timeouts.WriteTotalTimeoutMultiplier = 1;
  /*  Setting this to 10; seems to take 1 minute when HW is in a
      messed up state                                                      */
  timeouts.WriteTotalTimeoutConstant = 10;

  if(!SetCommTimeouts(hsuart_port, &timeouts))  // Error setting timeouts
  {
    FH_LOGE("Error setting com port timeouts");
    print_get_last_error();
    return FALSE;
  }

  return TRUE;
}

/******************************************************************************
 * Name: purge_hsuart_buffer
 *
 * Description:
 * This function purges any data in target TX buffer by reading any data
 * available on the communication channel and discarding it. It is helpful in
 * scenarios when coomunication between the host and the target was broken
 * suddenly and the target still had some data to send. On a fresh run at the
 * host this function will clear out any data that is pending to be transferred
 * by the target
 *
 *****************************************************************************/
static void purge_hsuart_buffer()
{
  uint32 count = 0;
  char buf = 0;
  uint32 bytes_read = 0;

  while(1)
  {
    if(!ReadFile(hsuart_port, &buf, 1, (LPDWORD)&bytes_read, NULL))
    {
      bytes_read = 0;
      print_get_last_error();
    }

    if(bytes_read == 0)
    {
      count++;
    }

    if(count > 10)
    {
      break;
    }
  }
}

/******************************************************************************
 * Name: fh_uart_open
 *
 * Description:
 * This function opens the communication port for UART
 *
 * Arguments:
 *    *port_name_buf      [IN]: port name
 *
 * Returns:
 *  SUCCES on successful operation, error code otherwise
 *
 *****************************************************************************/
static fh_res_t fh_uart_open(const char *port_name_buf)
{
  DCB dcb;
  BOOL success;
  fh_res_t error = OPEN_PORT_FAILED;

  // Open the COM Port
  hsuart_port = CreateFileA((LPCSTR)port_name_buf,
                            GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, 0, NULL);

  if(INVALID_HANDLE_VALUE == hsuart_port)
  {
    FH_LOGE("Failed to open com port '%s'", port_name_buf);
    goto CloseHandleAndExit;
  }

  // Get the state of COM Port
  SecureZeroMemory(&dcb, sizeof(DCB));
  dcb.DCBlength = sizeof(DCB);
  success = GetCommState(hsuart_port, &dcb);

  if(!success)
  {
    FH_LOGE("GetCommState failed with error %d.\n", GetLastError());
    print_get_last_error();
    goto CloseHandleAndExit;
  }

  FH_LOGI(("COM Port properties\nBaudRate = %d, ByteSize = %d, Parity = %d, \
        StopBits = %d\n"), dcb.BaudRate, dcb.ByteSize, dcb.Parity,
          dcb.StopBits);

  dcb.BaudRate = BAUDRATE;
  dcb.fOutX = 0;
  dcb.fInX = 0;
  dcb.fOutxCtsFlow = 1;
  dcb.fOutxDsrFlow = 0;
  dcb.fRtsControl = RTS_CONTROL_HANDSHAKE;
  dcb.fDtrControl = DTR_CONTROL_ENABLE;
  dcb.fParity = 0;
  // Disabled parity since not enabled on target side
  //dcb.fParity = TRUE;
  //dcb.fParity = EVENPARITY;
  dcb.ByteSize = 8;
  success = SetCommState(hsuart_port, &dcb);

  if(!success)
  {
    FH_LOGE("SetCommState failed with error %d.\n", GetLastError());
    print_get_last_error();
    goto CloseHandleAndExit;
  }

  FH_LOGI(("COM Port properties\nBaudRate = %d, ByteSize = %d, Parity = %d, \
        StopBits = %d\n"), dcb.BaudRate, dcb.ByteSize, dcb.Parity,
          dcb.StopBits);


  success = PurgeComm(hsuart_port, PURGE_TXCLEAR);

  if(!success)
  {
    FH_LOGE("PurgeComm for tx clear failed with error %d.\n",
            GetLastError());
    print_get_last_error();
    goto CloseHandleAndExit;
  }

  success = PurgeComm(hsuart_port, PURGE_RXCLEAR);

  if(!success)
  {
    FH_LOGE("PurgeComm for rx clear failed with error %d.\n",
            GetLastError());
    print_get_last_error();
    goto CloseHandleAndExit;
  }

  FH_LOGI("port=0x%X\n", hsuart_port);

  if(!set_port_timeouts())
  {
    error = SET_TIMEOUT_ERROR;
    goto CloseHandleAndExit;
  }

  purge_hsuart_buffer();

  return SUCCESS;

CloseHandleAndExit:

  if(hsuart_port != NULL)
  {
    CloseHandle(hsuart_port);
    hsuart_port = NULL;
  }

  FH_LOGI("Transport layer initialisation failed!!\n");

  return error;

}

/******************************************************************************
 * Name: fh_uart_read
 *
 * Description:
 * This function reads data from the UART communication port
 *
 * Arguments:
 *  *read_buf   [IN]: Pointer to buffer to store the data read
 *  read_len    [IN]: Length of the data to read
 *  *bytes_read [IN]: Length of the data actually read
 *
 * Returns:
 *  SUCCES on successful operation, error code otherwise
 *
 *****************************************************************************/
static fh_res_t fh_uart_read(void *read_buf, uint32 read_len,
                             uint32 *bytes_read)
{
  if(!ReadFile(hsuart_port, read_buf, read_len, (LPDWORD)bytes_read, NULL))
  {
    *bytes_read = 0;
    FH_LOGE("ReadFile API failed: Port (%x)", hsuart_port);
    print_get_last_error();
    return READ_PORT_ERROR;
  }

  return SUCCESS;
}

/******************************************************************************
 * Name: fh_uart_write
 *
 * Description:
 * This function writes data to the UART communication port
 *
 * Arguments:
 *  *write_buf      [IN]: Pointer to buffer to write the data from
 *  write_len       [IN]: Length of the data to write
 *  *bytes_written  [IN]: Length of the data actually written
 *
 * Returns:
 *  SUCCES on successful operation, error code otherwise
 *
 *****************************************************************************/
static fh_res_t fh_uart_write(void *write_buf, uint32 write_len,
                              uint32 *bytes_written)
{
  if(!WriteFile(hsuart_port, write_buf, write_len, (LPDWORD)bytes_written,
                NULL))
  {
    FH_LOGE("WriteFile API failed: Port (%x)", hsuart_port);
    print_get_last_error();
    return WRITE_PORT_ERROR;
  }

  return SUCCESS;
}

/******************************************************************************
 * Name: fh_uart_close
 *
 * Description:
 * This function closes the communication port for UART
 *
 * Returns:
 *  SUCCES on successful operation, error code otherwise
 *
 *****************************************************************************/
static fh_res_t fh_uart_close(void)
{
  if(hsuart_port != NULL)
  {
    CloseHandle(hsuart_port);
    hsuart_port = NULL;
  }

  return SUCCESS;

}

/******************************************************************************
 * Name: get_time_s
 *
 * Description:
 * Returns time in seconds
 *
 *****************************************************************************/
static long get_time_s(void)
{
  return GetTickCount() / 1000;
}

#elif FIREHOSE_SIMULATION

#include <unistd.h>
#include <time.h>
#include <string.h>


struct fh_transport *uart_sim = NULL;

/******************************************************************************
 * Name: fh_uart_open
 *
 * Description:
 * This function opens the communication port for UART
 *
 * Arguments:
 *    *port_name_buf      [IN]: port name
 *
 * Returns:
 *  SUCCES on successful operation, error code otherwise
 *
 *****************************************************************************/
static fh_res_t fh_uart_open(const char *path)
{
  if(uart_sim == NULL)
  {
    uart_sim = fh_transport_linux_pipe_init();
  }

  if(uart_sim != NULL)
  {
    return uart_sim->ops->open(path);
  }

  return NOT_SUPPORTED;
}

/******************************************************************************
 * Name: fh_uart_read
 *
 * Description:
 * This function reads data from the UART communication port
 *
 * Arguments:
 *  *read_buf   [IN]: Pointer to buffer to store the data read
 *  read_len    [IN]: Length of the data to read
 *  *bytes_read  [IN]: Length of the data actually read
 *
 * Returns:
 *  SUCCES on successful operation, error code otherwise
 *
 *****************************************************************************/
static fh_res_t fh_uart_read(void *read_buf, uint32 read_len,
                             uint32 *bytes_read)
{
  fh_res_t res;
  size_t bread;
  res = uart_sim->ops->rx_blocking(read_buf, read_len, &bread);
  *bytes_read = (uint32)bread;
  return res;
}

/******************************************************************************
 * Name: fh_uart_write
 *
 * Description:
 * This function writes data to the UART communication port
 *
 * Arguments:
 *  *write_buf      [IN]: Pointer to buffer to write the data from
 *  write_len       [IN]: Length of the data to write
 *  *bytes_written  [IN]: Length of the data actually written
 *
 * Returns:
 *  SUCCES on successful operation, error code otherwise
 *
 *****************************************************************************/
static fh_res_t fh_uart_write(void *write_buf, uint32 write_len,
                              uint32 *bytes_written)
{
  *bytes_written = write_len;
  return uart_sim->ops->tx_blocking(write_buf, write_len);
}

/******************************************************************************
 * Name: fh_uart_close
 *
 * Description:
 * This function closes the communication port for UART
 *
 * Returns:
 *  SUCCES on successful operation, error code otherwise
 *
 *****************************************************************************/
static fh_res_t fh_uart_close(void)
{
  return uart_sim->ops->close();
}

/******************************************************************************
 * Name: get_time_s
 *
 * Description:
 * Returns time in seconds
 *
 *****************************************************************************/
static long get_time_s(void)
{
  time_t seconds;
  seconds = time(NULL);
  return seconds;
}
#else

#include <unistd.h>
#include <time.h>
#include <string.h>

struct fh_transport *uart_sim = NULL;

static fh_res_t fh_uart_open(const char *path)
{
  return NOT_SUPPORTED;
}

static fh_res_t fh_uart_read(void *read_buf, uint32 read_len,
                             uint32 *bytes_read)
{
  return NOT_SUPPORTED;
}

static fh_res_t fh_uart_write(void *write_buf, uint32 write_len,
                              uint32 *bytes_written)
{

  return NOT_SUPPORTED;
}

static fh_res_t fh_uart_close(void)
{
  return NOT_SUPPORTED;
}

static long get_time_s(void)
{
  time_t seconds;
  seconds = time(NULL);
  return seconds;
}

#endif


/******************************************************************************
 * Name: send_nak
 *
 * Description:
 * This function sends NAK for the last received packet
 *
 * Returns:
 *  TRUE or FALSE depending on successful operation
 *
 *****************************************************************************/
static bool send_nak()
{
  unsigned char buffer[1] = { 0 };
  enum fh_hsuart_packet_error_t hsuart_packet_error;
  uint32 nak_result = fh_hsuart_packet_encode(NAK_PACKET, 0, 0, buffer, 1,
                      &hsuart_packet_error);

  if(hsuart_packet_error != HSUART_PACKET_SUCCESS)
  {
    return FALSE;
  }

  return transmit_hsuart_packet(buffer, nak_result);
}

/******************************************************************************
 * Name: send_nak
 *
 * Description:
 * This function sends ACK for the last received packet
 *
 * Returns:
 *  TRUE or FALSE depending on successful operation
 *
 *****************************************************************************/
static bool send_ack()
{
  unsigned char buffer[1] = { 0 };
  enum fh_hsuart_packet_error_t hsuart_packet_error;
  uint32 ack_result = fh_hsuart_packet_encode(ACK_PACKET, 0, 0, buffer, 1,
                      &hsuart_packet_error);

  if(hsuart_packet_error != HSUART_PACKET_SUCCESS)
  {
    return FALSE;
  }

  return transmit_hsuart_packet(buffer, ack_result);
}

/******************************************************************************
 * Name: wait_for_ack
 *
 * Description:
 * Waits for ACK packet for last sent HSUART packet
 *
 * Returns:
 * return TRUE if ACK was received else FALSE
 *
 *****************************************************************************/
static bool wait_for_ack()
{
  unsigned char receive_buffer[MAX_HSUART_PACKET_SIZE] = { 0 };
  unsigned char decode_buffer[MAX_HSUART_PACKET_SIZE] = { 0 };
  enum fh_hsuart_packet_error_t hsuart_packet_error;
  uint32 length = 0;
  uint32 result;
  enum fh_packet_id_t packet_id;

  if(receive_hsuart_packet(receive_buffer, &length, TRUE))
  {
    result = fh_hsuart_packet_decode(&packet_id, receive_buffer, length,
                                     decode_buffer, MAX_HSUART_PACKET_SIZE, &hsuart_packet_error);

    if(hsuart_packet_error != HSUART_PACKET_SUCCESS)
    {
      return FALSE;
    }

    if(result != 0)
    {
      FH_LOGI("Invalid packet id received %02x", packet_id);
    }

    if(ACK_PACKET != packet_id)
    {
      if(NAK_PACKET != packet_id)
      {
        FH_LOGI("Invalid packet id received %02x", packet_id);
      }
      else
      {
        FH_LOGI("%s", "NAK received");
      }
    }

    return ACK_PACKET == packet_id;
  }

  return FALSE;
}

/******************************************************************************
 * Name: receive_hsuart_packet
 *
 * Description:
 * This function receives single HSUART packet
 *
 * Arguments:
 *  *buf            [IN]: Pointer to buffer to store payload
 *  *length         [OUT]: Length of payload
 *  waiting_for_ack [IN]:if waiting for ACK
 *
 * Returns:
 *  TRUE or FALSE depending on if packet was received successfully
 *
 *****************************************************************************/
static bool receive_hsuart_packet(unsigned char *buf, uint32* length,
                                  bool waiting_for_ack)
{
  uint32 bytes_read = 0;
  uint32 total_bytes_read = 0;
  long start_time, read_time;
  start_time = get_time_s();

  while(1)
  {
    if(fh_uart_read(buf + total_bytes_read,
                    (waiting_for_ack ? PACKET_ID_SIZE : MAX_HSUART_PACKET_SIZE),
                    &bytes_read) == SUCCESS)
    {
      total_bytes_read += bytes_read;;
    }

    if(bytes_read == 0)
    {
      sleep(1);
    }

    if(total_bytes_read > 0)
    {
      if((END_OF_PACKET == *(buf + total_bytes_read - 1))
          || waiting_for_ack)
      {
        break;
      }
      else
      {
        FH_LOGI("Received %d bytes, reading more", total_bytes_read);
      }
    }

    read_time = get_time_s();

    if(read_time - start_time > READ_TIMEOUT_SECONDS)
    {
      FH_LOGE("Read failed due to timeout: %ld", read_time - start_time);
      break;
    }
  }

  *length = total_bytes_read;
  return TRUE;
}

/******************************************************************************
 * Name: read_hsuart_packet
 *
 * Description:
 * This function receives, decodes and validate single HSUART packet and sends
 * the corresponding response
 *
 * Arguments:
 *  *buf            [IN]: Pointer to buffer to store payload
 *  *length         [OUT]: Length of payload
 *  end_of_transfer [OUT]: if END_OF_TRANSFER packet
 *
 * Returns:
 *  TRUE or FALSE depending on if packet was received and parsed successfully
 *
 *****************************************************************************/
static bool read_hsuart_packet(unsigned char *buf, uint32* length,
                               bool* end_of_transfer)
{
  unsigned char read_buffer[MAX_HSUART_PACKET_SIZE] = { 0 };
  unsigned char receive_buffer[MAX_HSUART_PACKET_SIZE] = { 0 };
  uint32 received_packet_length = 0;
  enum fh_packet_id_t pkt_id;
  enum fh_hsuart_packet_error_t hsuart_packet_error;
  uint32 decode_result;

  if(end_of_transfer != NULL)
  {
    *end_of_transfer = FALSE;
  }

  if(receive_hsuart_packet(receive_buffer, &received_packet_length, FALSE))
  {
    decode_result = fh_hsuart_packet_decode(&pkt_id, receive_buffer,
                                            received_packet_length, read_buffer, MAX_HSUART_PACKET_SIZE,
                                            &hsuart_packet_error);

    switch(pkt_id)
    {
    case END_OF_TRANSFER_PACKET:
      if(end_of_transfer != NULL)
      {
        *end_of_transfer = TRUE;
      }

      // Intentionally missing break here as both END_OF_TRANSFER_PACKET
      // and PROTOCOL_PACKET has some common execution
    case PROTOCOL_PACKET:
      if(HSUART_PACKET_SUCCESS == hsuart_packet_error)
      {
        memscpy(buf, decode_result, read_buffer, decode_result);
        *length = decode_result;
      }

      if(hsuart_packet_error != HSUART_INVALID_PARAM_ERROR)
      {
        goto SendResponse;
      }

      break;

    case READY_TO_READ_PACKET:
      if(hsuart_packet_error != HSUART_INVALID_PARAM_ERROR)
      {
        fh_transport_hsuart_is_target_in_read_mode = TRUE;
      }

      // Intentionally missing break here as both READY_TO_READ_PACKET
      // and VERSION_PACKET has some common execution
    case VERSION_PACKET:
      if(hsuart_packet_error != HSUART_INVALID_PARAM_ERROR)
      {
        goto SendResponse;
      }

      break;

    default:
      FH_LOGE("Invalid packet id %02x", pkt_id);
    }

    return FALSE;
SendResponse:

    if(hsuart_packet_error != HSUART_PACKET_SUCCESS)
    {
      if(!send_nak())
      {
        FH_LOGE("%s", "Failed to send NAK.");
      }

      return FALSE;
    }
    else
    {
      return send_ack();
    }
  }

  return FALSE;
}

/******************************************************************************
 * Name: transmit_hsuart_packet
 *
 * Description:
 * This function transmits single HSUART packet
 *
 * Arguments:
 *  *buf            [IN]: Pointer to buffer to transmit payload
 *  *length         [IN]: Length of payload
 *
 * Returns:
 *  TRUE or FALSE depending on if packet was transmitted successfully
 *
 *****************************************************************************/
static bool transmit_hsuart_packet(unsigned char *buf, uint32 length)
{
  uint32 bytes_written = 0;
  uint32 total_bytes_written = 0;

  while(1)
  {
    if(fh_uart_write(buf + total_bytes_written,
                     length - total_bytes_written, &bytes_written) != SUCCESS)
    {
      return FALSE;
    }

    if(bytes_written > 0)
    {
      total_bytes_written += bytes_written;
    }

    if(length == total_bytes_written)
    {
      break;
    }
  }

  return TRUE;
}

/******************************************************************************
 * Name: write_hsuart_packet
 *
 * Description:
 * This function encodes and transmits single HSUART packet
 *
 * Arguments:
 *  *buf            [IN]: Pointer to buffer to transmit payload
 *  *length         [IN]: Length of payload
 *  *packet_id      [IN]: Packet Id of HSUART packet
 *
 * Returns:
 *  TRUE or FALSE depending on if packet was encoded and transmitted
 *  successfully
 *
 *****************************************************************************/
static bool write_hsuart_packet(unsigned char *buf, uint32 length,
                                enum fh_packet_id_t packet_id)
{

  uint32 encode_result;
  enum fh_hsuart_packet_error_t hsuart_packet_error;
  unsigned char write_buffer[MAX_HSUART_PACKET_SIZE] = { 0 };
  encode_result = fh_hsuart_packet_encode(packet_id, buf, length,
                                          write_buffer, MAX_HSUART_PACKET_SIZE, &hsuart_packet_error);

  if(HSUART_PACKET_SUCCESS == hsuart_packet_error)
  {
    do
    {
      if(!transmit_hsuart_packet(write_buffer, encode_result))
      {
        return FALSE;
      }
    }
    while(!wait_for_ack());

    return TRUE;
  }

  return FALSE;
}

/******************************************************************************
 * Name: fh_transport_hsuart_open
 *
 * Description:
 * This function opens a connection to the provided COMM PORT through UART and
 * then exchanges version packet to complete the initialization of UART
 * transport layer
 *
 * Arguments:
 *    *path  [IN]: Pointer to buffer having name of COMM port.
 *
 * Returns:
 *    SUCCESS when open was successfull, error otherwise.
 *
 ******************************************************************************/
static fh_res_t fh_transport_hsuart_open(const char *path)
{

  if(fh_uart_open(path) != SUCCESS)
  {
    goto Exit;
  }

  // Version hand shake between the transport layer at target as well as
  // the host side
  if(write_hsuart_packet(0, 0, VERSION_PACKET))
  {
    if(read_hsuart_packet(0, 0, NULL))
    {
      FH_LOGI("Transport layer initialisation successful!!\n", 0);
    }
    else
    {
      FH_LOGI("Failed to receive version packet\n", 0);
      goto CloseHandleAndExit;
    }
  }
  else
  {
    FH_LOGI("Failed to send version packet\n", 0);
    goto CloseHandleAndExit;
  }

  return SUCCESS;

CloseHandleAndExit:
  fh_uart_close();
Exit:
  FH_LOGE("Transport layer initialisation failed!!\n", 0);
  return OPEN_PORT_FAILED;
}

/******************************************************************************
 * Name: get_data_from_buffer
 *
 * Description:
 * This function fetches data that was buffer before receiving READY_TO_READ
 * packet
 *
 * Arguments:
 *  *buffer             [IN]: Pointer to buffer to copy data to
 *  requested_length    [IN]: Length of data to fetch
 *
 * Returns:
 *  Length of data actually fetched
 *
 *****************************************************************************/
static uint32 get_data_from_buffer(unsigned char *buffer,
                                   uint32 requested_length)
{
  uint32 length = 0;

  if(requested_length < fh_transport_hsuart_buffered_length)
  {
    memscpy(buffer, requested_length,
            fh_transport_hsuart_buffered_data_start_ptr, requested_length);
    fh_transport_hsuart_buffered_data_start_ptr += requested_length;
    fh_transport_hsuart_buffered_length -= requested_length;
    length = requested_length;
  }
  else
  {
    memscpy(buffer, fh_transport_hsuart_buffered_length,
            fh_transport_hsuart_buffered_data_start_ptr,
            fh_transport_hsuart_buffered_length);
    fh_transport_hsuart_buffered_data_start_ptr =
      fh_transport_hsuart_buffered_data;
    fh_transport_hsuart_buffered_data_end_ptr =
      fh_transport_hsuart_buffered_data;
    length = fh_transport_hsuart_buffered_length;
    fh_transport_hsuart_buffered_length = 0;
  }

  return length;
}

/******************************************************************************
* Name: fh_transport_hsuart_read
*
* Description:
* Reads multiple HSUART packets and stitch them together untill END_OF_TRANSFER
* packet Id is received
*
* Arguments:
* *read_buffer    [IN]: Pointer to buffer storing the data.
* read_len      [IN]: Requested length of data
* *bytes_read     [OUT]: Bytes actually read
*
* Returns:
*    TRUE:  Data read successfully
*    FALSE: Failed to read any data
*
******************************************************************************/
static uint32 fh_transport_hsuart_read(void *read_buffer,
                                       uint32 read_len, uint32 *bytes_read)
{
  unsigned char buffer[MAX_HSUART_PACKET_SIZE] = { 0 };
  uint32 read_length = 0;
  bool end_of_transfer = FALSE;

  if(fh_transport_hsuart_buffered_length != 0)
  {
    *bytes_read = get_data_from_buffer(read_buffer, read_len);
  }
  else
  {
    *bytes_read = 0;

    while(!end_of_transfer)
    {
      memset(buffer, 0, MAX_HSUART_PACKET_SIZE);
      read_length = 0;

      if(read_hsuart_packet(buffer, &read_length, &end_of_transfer))
      {
        memscpy(((unsigned char*)read_buffer) + *bytes_read,
                read_length, buffer, read_length);
        *bytes_read += read_length;
      }
      else
      {
        break;
      }
    }
  }

  return (*bytes_read > 0);
}

/******************************************************************************
 * Name: add_data_to_buffer
 *
 * Description:
 * This function stores data that was buffered before receiving READY_TO_READ
 * packet
 *
 * Arguments:
 *  *buffer             [IN]: Pointer to buffer to copy data to
 *  requested_length    [IN]: Length of data to store
 *
 * Returns:
 *  TRUE if data was stored successfully, FALSE otherwise
 *
 *****************************************************************************/
static bool add_data_to_buffer(unsigned char *buffer, uint32 length)
{
  if(length < (ONE_MEGA_BYTE - fh_transport_hsuart_buffered_length -
               (fh_transport_hsuart_buffered_data_end_ptr -
                fh_transport_hsuart_buffered_data)))
  {
    memscpy(fh_transport_hsuart_buffered_data_end_ptr, length, buffer,
            length);
    fh_transport_hsuart_buffered_data_end_ptr += length;
    fh_transport_hsuart_buffered_length += length;
  }
  else
  {
    return FALSE;
  }

  return TRUE;
}

/******************************************************************************
 * Name: wait_for_ready_to_read_packet
 *
 * Description:
 * Waits for READY_TO_READ packet and buffers the data received earlier
 *
 * Returns:
 * return TRUE if READY_TO_READ packet was received else FALSE
 *
 *****************************************************************************/
static bool wait_for_ready_to_read_packet()
{
  unsigned char buffer[MAX_HSUART_PACKET_SIZE] = { 0 };
  uint32 read_length = 0;

  while(!fh_transport_hsuart_is_target_in_read_mode)
  {
    memset(buffer, 0, MAX_HSUART_PACKET_SIZE);
    read_length = 0;

    if(read_hsuart_packet(buffer, &read_length, NULL))
    {
      if(!add_data_to_buffer(buffer, read_length))
      {
        return FALSE;
      }
    }
    else
    {
      return FALSE;
    }
  }

  return TRUE;
}

/******************************************************************************
 * Name: fh_transport_hsuart_write
 *
 * Description:
 * Breaks the data into multiple small HSUART packets and transmits them over
 * the communication layer with appropriate packet Ids
 *
 * Arguments:
 *   *write_buffer  [IN]: Pointer to buffer having the data.
 *   write_len      [IN]: Length of the data
 *   *bytes_written [OUT]: Bytes actually written
 *
 * Returns:
*    TRUE:  Complete data written successfully
*    FALSE: Failed to write complete data
 *
 ******************************************************************************/
static uint32 fh_transport_hsuart_write(void *write_buffer,
                                        uint32 write_len, uint32 *bytes_written)
{
  uint32 packet_count = 0;
  uint32 packet_index = 0;
  uint32 transfer_size = MAX_HSUART_PACKET_DATA_SIZE;
  enum fh_packet_id_t packet_id = PROTOCOL_PACKET;

  if(wait_for_ready_to_read_packet())
  {
    packet_count = write_len / MAX_HSUART_PACKET_DATA_SIZE;

    if(write_len % MAX_HSUART_PACKET_DATA_SIZE)
    {
      packet_count++;
    }

    for(packet_index = 0; packet_index < packet_count; packet_index++)
    {
      if(packet_count - packet_index == 1)
      {
        transfer_size = write_len % MAX_HSUART_PACKET_DATA_SIZE;
        packet_id = END_OF_TRANSFER_PACKET;
      }

      if(!write_hsuart_packet((unsigned char*)write_buffer +
                              (packet_index * MAX_HSUART_PACKET_DATA_SIZE),
                              transfer_size, packet_id))
      {

      }
      else
      {
        *bytes_written += transfer_size;
      }
    }

    fh_transport_hsuart_is_target_in_read_mode = FALSE;
  }

  return *bytes_written > 0;
}

static fh_res_t fh_transport_hsuart_rx_blocking(void *data, size_t size,
    size_t *bytes_rx)
{
  int retry = 100;

  do
  {
    if(!fh_transport_hsuart_read(data, (uint32)size,
                                 (uint32 *)bytes_rx) != 0)
    {
      return READ_PORT_ERROR;
    }

    retry--;

    if(retry == 0)
    {
      return READ_PORT_ERROR;
    }
  }
  while(*bytes_rx == 0);

  return SUCCESS;
}

static fh_res_t fh_transport_hsuart_tx_blocking(void *tx_data, size_t size)
{
  uint32 bytes_written = 0;
  int retry = 100;

  do
  {
    if(!fh_transport_hsuart_write(tx_data, (uint32)size,
                                  &bytes_written) != 0)
    {
      return READ_PORT_ERROR;
    }

    retry--;

    if(retry == 0)
    {
      return READ_PORT_ERROR;
    }
  }
  while(bytes_written < size);

  return SUCCESS;
}

static fh_res_t fh_transport_hsuart_close(void)
{
  fh_uart_close();
  return SUCCESS;
}

static fh_transport_ops fh_transport_hsuart_ops =
{
  fh_transport_hsuart_open,
  fh_transport_hsuart_rx_blocking,
  fh_transport_hsuart_tx_blocking,
  fh_transport_hsuart_close,
};

static struct fh_transport fh_transport_hsuart =
{
  FH_TRANSPORT_HSUART,
  &fh_transport_hsuart_ops,
};

struct fh_transport *fh_transport_hsuart_init(void)
{
  return &fh_transport_hsuart;
}
