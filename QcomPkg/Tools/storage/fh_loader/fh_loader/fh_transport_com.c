/**************************************************************************
 * FILE: fh_transport_com.h
 *
 * Implementation of the COM port transport layer. Used USB and PCIe.
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

  $Header: //components/rel/boot.xf/3.0.1.c1/QcomPkg/Tools/storage/fh_loader/fh_loader/fh_transport_com.c#1 $
  $DateTime: 2021/04/29 02:01:56 $
  $Author: pwbldsvc $

when         who   what, where, why
----------   ---   ---------------------------------------------------------
2019-05-29   wek   Create.

===========================================================================*/

#include "fh_comdef.h"
#include "fh_log.h"
#include "fh_transport.h"

#define NUM_RETRY_READ 100
#define NUM_RETRY_WRITE 100

#ifdef _MSC_VER /* Compiling under Windows */

#include <windows.h>

struct fh_windows_com_data
{
  int init;
  HANDLE com_fd;
};

static struct fh_windows_com_data windows_com_data = {0};

static fh_res_t fh_transport_com_open(const char *path)
{
  struct fh_windows_com_data *pdata = &windows_com_data;
  COMMTIMEOUTS timeouts;

  if (pdata->init != 0)
  {
    FH_LOGE("Opening the port without closing %d.\n", pdata->init);
    return INVALID_PARAMETER;
  }

  pdata->com_fd = CreateFileA((LPCSTR)path, GENERIC_READ | GENERIC_WRITE,
                              0,  NULL, OPEN_ALWAYS, 0, NULL);
  if (pdata->com_fd == INVALID_HANDLE_VALUE)
  {
    FH_LOGE("Failed to open port(%s)", path);
    return OPEN_PORT_FAILED;
  }
  /* maximum number of milliseconds that can elapse between two characters
     without a timeout occurring  */
  timeouts.ReadIntervalTimeout        = 20;
  /* For each read operation, this number is multiplied by the number of bytes
     that the read operation expects to receive.*/
  timeouts.ReadTotalTimeoutMultiplier = 0;
  timeouts.ReadTotalTimeoutConstant   = 100; /* actual timeout is this in ms */
  timeouts.WriteTotalTimeoutMultiplier = 1;
  /* 10 seems to take 1 minute when HW is in a messed up state */
  timeouts.WriteTotalTimeoutConstant  = 10;

  if (!SetCommTimeouts (pdata->com_fd, &timeouts) ) // Error setting time-outs
  {
    FH_LOGE ("Error setting com port timeouts");
    return OPEN_PORT_FAILED;
  }

  return SUCCESS;
}

static fh_res_t fh_transport_com_rx_blocking (void *data, size_t size, size_t *bytes_rx)
{
  struct fh_windows_com_data *pdata = &windows_com_data;
  int i;
  DWORD bytesRx;

  *bytes_rx = 0;

  for (i = 0; i < NUM_RETRY_READ; i++)
  {
    if (ReadFile(pdata->com_fd, data, size, &bytesRx, NULL))
    {
      break;
    }
    else
    {
      FH_LOGE("Error while reading data Will retry %llu",
              (unsigned long long) NUM_RETRY_READ - i);
    }
  }

  *bytes_rx = (size_t)bytesRx;

  return SUCCESS;
}

static fh_res_t fh_transport_com_tx_blocking(void *tx_data, size_t size)
{
  struct fh_windows_com_data *pdata = &windows_com_data;
  size_t remain;
  int num_retry = NUM_RETRY_WRITE;
  uint8 *data = (uint8 *)tx_data;
  DWORD written;
  FH_LOGD("Trying to write %llu bytes to the COM", (unsigned long long)size);
  if (size > SIZE_MAX/2)
  {
    FH_LOGE("TX size too large %llu", (unsigned long long) size);
    return INVALID_PARAMETER;
  }
  remain = size;
  while (remain > 0 && num_retry > 0)
  {
    if (WriteFile (pdata->com_fd, data, remain, &written, NULL))
    {
      remain -= (size_t)written;
      data += written;
    }
    else
    {
      FH_LOGE("Error while writing to port, may retry: %llu, %llu",
              (unsigned long long) remain, (unsigned long long) num_retry);
    }
    num_retry--;
  }

  if (remain > 0)
  {
    FH_LOGE("Failed to write all the data to the port %llu", (unsigned long long) remain);
    return WRITE_PORT_ERROR;
  }
  return SUCCESS;
}

static fh_res_t fh_transport_com_close(void)
{
  struct fh_windows_com_data *pdata = &windows_com_data;
  if (pdata->init != 1)
    return INVALID_PARAMETER;

  CloseHandle(pdata->com_fd);
  pdata->init = 0;

  return SUCCESS;
}

#else /* Asume Linux for else case.  */

#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>
#include <sys/select.h>
#include <limits.h>
#include "stringl.h"

struct fh_linux_com_data
{
  int init;
  int com_fd;
};

static struct fh_linux_com_data linux_com_data = {0};

static fh_res_t fh_transport_com_open(const char *path)
{
  struct fh_linux_com_data *pdata = &linux_com_data;
  struct termios tio;

  if (pdata->init != 0)
  {
    FH_LOGE("Opening the port without closing %d.\n", pdata->init);
    return INVALID_PARAMETER;
  }

  pdata->com_fd = open(path, O_RDWR | O_SYNC);
  if (pdata->com_fd == -1)
  {
    FH_LOGE("Failed to open port(%s): %s", path, strerror(errno));
    return OPEN_PORT_FAILED;
  }

  memset (&tio, 0, sizeof (tio) );
  tio.c_iflag = 0;
  tio.c_oflag = 0;
  tio.c_cflag = CS8 | CREAD | CLOCAL;
  tio.c_lflag = 0;
  tio.c_cc[VMIN] = 1;
  tio.c_cc[VTIME] = 5;
  cfsetospeed (&tio, B115200);
  cfsetispeed (&tio, B115200);
  tcsetattr (pdata->com_fd, TCSANOW, &tio);
  pdata->init = 1;

  return SUCCESS;
}

fh_res_t fh_transport_com_rx_blocking_fd (int fd, void *data, size_t size, size_t *bytes_rx)
{
  fd_set rfds;
  struct timeval tv;
  int result;
  ssize_t bytes_in;
  int i;

  tv.tv_sec = 50;
  tv.tv_usec = 0;
  FD_ZERO (&rfds);
  FD_SET (fd, &rfds);
  *bytes_rx = 0;


  for (i = 0; i < NUM_RETRY_READ; i++)
  {
    result = select (fd + 1, &rfds, NULL, NULL, &tv);
    if (result == 0)
    {
      FH_LOGI("Didn't receive data from port after %f, will retry %lld times",
              (double)tv.tv_sec + ((double)tv.tv_usec / 1000.0), (long long) NUM_RETRY_READ - i);
    }
    else
    {
      bytes_in = read (fd, data, size);
      if (bytes_in < 0)
      {
        FH_LOGE("Error while reading data: %s, Will retry %lld", strerror(errno),
                (long long) NUM_RETRY_READ - i);
      }
      else
      {
        break;
      }
    }
  }

  *bytes_rx = (size_t)bytes_in;

  return SUCCESS;
}

fh_res_t fh_transport_com_tx_blocking_fd (int fd, void *tx_data, size_t size)
{
  ssize_t tx_bytes;
  size_t remain;
  int num_retry = NUM_RETRY_WRITE;
  uint8 *data = (uint8 *)tx_data;
  FH_LOGD("Trying to write %llu bytes to the COM", (unsigned long long)size);
  if (size > SIZE_MAX/2)
  {
    FH_LOGE("TX size too large %llu", (unsigned long long) size);
    return INVALID_PARAMETER;
  }
  remain = size;
  while (remain > 0 && num_retry > 0)
  {
    tx_bytes = write (fd, data, remain);
    if (tx_bytes < 0)
    {
      FH_LOGE("Error while writing to port, may retry: %s %llu, %llu", strerror(errno),
              (unsigned long long) remain, (unsigned long long) num_retry);
    }
    else
    {
      remain -= (size_t)tx_bytes;
      data += tx_bytes;
    }
    num_retry--;
  }
  if (remain > 0)
  {
    FH_LOGE("Failed to write all the data to the port %llu", (unsigned long long) remain);
    return WRITE_PORT_ERROR;
  }
  return SUCCESS;
}

static fh_res_t fh_transport_com_rx_blocking (void *data, size_t size, size_t *bytes_rx)
{
  struct fh_linux_com_data *pdata = &linux_com_data;
  if (pdata->init != 1)
    return INVALID_PARAMETER;

  return fh_transport_com_rx_blocking_fd(pdata->com_fd, data, size, bytes_rx);
}

static fh_res_t fh_transport_com_tx_blocking (void *tx_data, size_t size)
{
  struct fh_linux_com_data *pdata = &linux_com_data;
  if (pdata->init != 1)
    return INVALID_PARAMETER;

  return fh_transport_com_tx_blocking_fd(pdata->com_fd, tx_data, size);
}


static fh_res_t fh_transport_com_close(void)
{
  struct fh_linux_com_data *pdata = &linux_com_data;
  if (pdata->init != 1)
    return INVALID_PARAMETER;

  close(pdata->com_fd);
  pdata->init = 0;

  return SUCCESS;
}

#endif

static fh_transport_ops fh_transport_com_ops =
{
  fh_transport_com_open,
  fh_transport_com_rx_blocking,
  fh_transport_com_tx_blocking,
  fh_transport_com_close,
};

static struct fh_transport fh_transport_com =
{
  FH_TRANSPORT_COM,
  &fh_transport_com_ops,
};

struct fh_transport *fh_transport_com_init(void)
{
  return &fh_transport_com;
}
