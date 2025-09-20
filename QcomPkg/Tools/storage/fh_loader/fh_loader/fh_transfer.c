/**************************************************************************
 * FILE: fh_transfer.c
 *
 * Transfer abstraction layer for TX and RX. Includes VIP.
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

  $Header: //components/rel/boot.xf/3.0.1.c1/QcomPkg/Tools/storage/fh_loader/fh_loader/fh_transfer.c#1 $
  $DateTime: 2021/04/29 02:01:56 $
  $Author: pwbldsvc $

when         who   what, where, why
----------   ---   ---------------------------------------------------------
2019-05-16   wek   Create.

===========================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "stringl.h"


#include "fh_comdef.h"
#include "fh_log.h"
#include "fh_transfer.h"
#include "fh_transport.h"
#include "fh_utils.h"

#define VIP_CHAINED_TABLE_SIZE   8192

enum vip_state
{
  VIP_DISABLED,
  VIP_INIT, /* We need to send initial VIP table. */
  VIP_SEND_NEXT_TABLE,
  VIP_SEND_DATA,
  VIP_MAX,
};

struct fh_transfer_data
{
  enum vip_state state;
  FILE *signed_table;
  FILE *chained_table;
  size_t digests_per_table;
  size_t frame_sent;
  size_t frames_to_next_table;
  long chained_table_size;
};

struct fh_transfer_data static_fh_transfer_data = {0};


/* Return the size of the XML in the buffer when it ends with </data>. */
static size_t response_xml_parse(char *xml, size_t xml_size,
                                 char **tag, char **value)
{
  size_t offset, last_offset;
  /* If the buffer is too small to contain the string <?xml return. */
  if (xml_size < 12)
    return 0;

  offset = 6; /* Skip the inital <?xml?> */
  last_offset = xml_size - 7;
  while (offset < last_offset)
  {
    if (xml[offset++] == '<' &&
        xml[offset++] == 'd' &&
        xml[offset++] == 'a' &&
        xml[offset++] == 't' &&
        xml[offset++] == 'a' &&
        xml[offset++] == '>')
      break;
  }

  /* Offset moved past <data>, after the next < is the tag. */
  while (xml[offset] != '<')
  {
    offset++;
    if (offset > last_offset)
      return 0;
  }
  /* Found open < next character is the tag. */
  offset++;
  *tag = &xml[offset];

  /* Now search for the end of the tag, the white space. */
  while (xml[offset] != ' ')
  {
    offset++;
    if (offset > last_offset)
      return 0;
  }
  /* Move beyond the space. */
  offset++;
  *value = NULL;

  /* Find the closing tag "/>" */
  while (offset < last_offset)
  {
    if (memcmp (&xml[offset], "value=\"", 7) == 0)
    {
      offset += 7;
      *value = &xml[offset];
    }
    else if (xml[offset++] == '/' &&
             xml[offset++] == '>')
    {
      break;
    }
  }

  while (offset <= last_offset)
  {
    if (xml[offset++] == '<' &&
        xml[offset++] == '/' &&
        xml[offset++] == 'd' &&
        xml[offset++] == 'a' &&
        xml[offset++] == 't' &&
        xml[offset++] == 'a' &&
        xml[offset++] == '>')
      return offset;
  }

  return 0;
}


/* VIP related functions. */
static fh_res_t fh_vip_send_signed_table(void)
{
  struct fh_transfer_data *vip = &static_fh_transfer_data;
  long file_size_long;
  size_t file_size;
  size_t read_size, xml_size, bytes_read;
  char *buf;
  FILE *fp;
  fh_res_t res, result;
  char response_xml_buf[MAX_XML_SIZE];
  char *command;
  char *value;

  fp = vip->signed_table;
  if (fseek(fp, 0, SEEK_END) != 0)
  {
    FH_LOGERRNO("Can't seek signed table%c", '.');
    return FILE_IO;
  }

  file_size_long = ftell(fp);
  if (file_size_long <= 0)
  {
    FH_LOGERRNO("Can't get file size for signed table %ld", file_size_long);
    return FILE_IO;
  }
  else if (file_size_long > 16 * 1024) /* Lets put a limit of 16K because this will be allocated in the stack. */
  {
    FH_LOGE("Signed file size larger than expected > 16K %ld", file_size_long);
    return FILE_IO;
  }
  rewind (fp);
  file_size = (size_t)file_size_long;
  buf = malloc(file_size);
  if (buf == NULL)
  {
    FH_LOGE("Failed to allocate signed table buffer%c", '.');
    return NO_MEMORY;
  }

  bytes_read = 0;
  do
  {
    read_size = fread(buf + bytes_read, 1, file_size - bytes_read, fp);
    if (read_size == 0)
    {
      FH_LOGE("Can't read Signed file %ld", read_size);
      result = FILE_IO;
      goto done;
    }
    bytes_read += read_size;
  } while (bytes_read < file_size);

  res = fh_transport_tx_blocking (buf, bytes_read);
  if (res != SUCCESS)
  {
    FH_LOGE ("Failed to send digest table %ld", (long int) read_size);
    result = res;
    goto done;
  }

  /* Wait for ack. */
  res = fh_rx_blocking_response_xml (response_xml_buf, sizeof(response_xml_buf),
                                     &xml_size);
  if (res != SUCCESS)
  {
    FH_LOGE ("Failed to get response xml for VIP %ld", (long int) xml_size);
    result = res;
    goto done;
  }

  response_xml_parse(response_xml_buf, xml_size, &command, &value);
  if (value != NULL)
  {
    if (strncasecmp (value, "ACK", 3) == 0)
    {
      FH_LOGD("ACK for signed VIP table received %u", xml_size);
      result = SUCCESS;
      goto done;
    }
  }
  else
  {
    FH_LOGE("Didn't receive proper response XML %u", xml_size);
    fh_print_hex(response_xml_buf, xml_size);
  }
  result = TARGET_NAK;
done:
  free (buf);
  return result;
}


static fh_res_t fh_vip_send_chained_table(void)
{
  struct fh_transfer_data *vip = &static_fh_transfer_data;
  size_t read_size, xml_size, bytes_read;
  FILE *fp;
  fh_res_t res;
  char response_xml_buf[MAX_XML_SIZE];
  char *command;
  char *value;
  char next_table[VIP_CHAINED_TABLE_SIZE];
  size_t table_size = sizeof(next_table);
  long offset, remain_size;

  fp = vip->chained_table;
  offset = ftell(fp);
  if (offset < 0)
  {
    FH_LOGERRNO("Can't get file offset for chained table %ld", offset);
    return FILE_IO;
  }
  remain_size = vip->chained_table_size - offset;
  if (remain_size < (long)table_size)
    table_size = (size_t)remain_size;

  bytes_read = 0;
  do
  {
    read_size = fread(next_table + bytes_read, 1, table_size - bytes_read, fp);
    if (read_size == 0)
    {
      FH_LOGE("Can't read Signed file %ld", read_size);
      return FILE_IO;
    }
    bytes_read += read_size;
  } while (bytes_read < table_size);

  res = fh_transport_tx_blocking (next_table, table_size);
  if (res != SUCCESS)
  {
    FH_LOGE ("Failed to send digest table %ld", (long int) read_size);
    return res;
  }

  /* Wait for ack. */
  res = fh_rx_blocking_response_xml (response_xml_buf, sizeof(response_xml_buf),
                                     &xml_size);
  if (res != SUCCESS)
  {
    FH_LOGE ("Failed to get response xml for VIP %ld", (long int) xml_size);
    return res;
  }

  response_xml_parse(response_xml_buf, xml_size, &command, &value);
  if (value != NULL)
  {
    if (strncasecmp (value, "ACK", 3) == 0)
    {
      FH_LOGD("ACK for VIP chained table received %u", xml_size);
      return SUCCESS;
    }
  }
  else
  {
    FH_LOGE("Didn't receive proper response for chained table XML %u", xml_size);
    fh_print_hex(response_xml_buf, xml_size);
  }
  return TARGET_NAK;
}


fh_res_t fh_transfer_init(enum fh_transport_type transport_type)
{
  struct fh_transfer_data *vip = &static_fh_transfer_data;

  if (transport_type == FH_TRANSPORT_NONE || transport_type >= FH_TRANSPORT_MAX)
  {
    FH_LOGE("Invalid transport type %llu", (unsigned long long) transport_type);
    return INVALID_PARAMETER;
  }

  if (vip->state != VIP_INIT)
    vip->state = VIP_DISABLED;

  return fh_transport_init(transport_type);
}

enum fh_transport_type fh_transfer_get_type(void)
{
  return fh_transport_get_type();
}

fh_res_t fh_transfer_deinit(void)
{
  struct fh_transfer_data *vip = &static_fh_transfer_data;
  if (vip->state != VIP_DISABLED)
  {
    fclose(vip->signed_table);
    fclose(vip->chained_table);
  }
  return fh_transport_deinit();
}

fh_res_t fh_transfer_open(const char *path)
{
  return fh_transport_open(path);
}

static char fh_rx_buffer[4*1024];
static size_t fh_rx_buffer_bytes = 0;

static void shift_fh_rx_buffer_left (size_t bytes)
{
  size_t buf_remain = fh_rx_buffer_bytes - bytes;
  /* Move the data to the start of the buffer. */
  memsmove (fh_rx_buffer, sizeof(fh_rx_buffer),
            &fh_rx_buffer[bytes], buf_remain);
  /* Clear the data that was just moved. */
  memset (&fh_rx_buffer[buf_remain], 0, bytes);
  fh_rx_buffer_bytes = buf_remain;
}

static fh_res_t read_more_data(void)
{
  size_t offset = fh_rx_buffer_bytes;
  size_t bytes_read;
  size_t remain = sizeof(fh_rx_buffer) - offset;
  fh_res_t res;

  res = fh_transport_rx_blocking(&fh_rx_buffer[offset], remain, &bytes_read);
  if (res == SUCCESS)
  {
    fh_rx_buffer_bytes += bytes_read;
  }
  return res;
}

/* Make sure the buffer starts with <?xml.
 * Returns 0 when the buffer starts with XML, other values means more data is needed. */
static int start_buf_with_xml(void)
{
  size_t offset;
  /* If the buffer is too small to contain the string <?xml return. */
  if (fh_rx_buffer_bytes < 6)
    return 1;

  for (offset = 0; offset < fh_rx_buffer_bytes; offset++)
  {
    if (fh_rx_buffer[offset] == '<' &&
        fh_rx_buffer[offset+1] == '?' &&
        fh_rx_buffer[offset+2] == 'x' &&
        fh_rx_buffer[offset+3] == 'm' &&
        fh_rx_buffer[offset+4] == 'l')
      break;
  }

  if (offset == 0)
    return 0;

  FH_LOGE ("Unexpected data while reading XML, discarding it %u.", offset);
  fh_print_hex(fh_rx_buffer, offset);

  if (offset < fh_rx_buffer_bytes)
  {
    shift_fh_rx_buffer_left (offset);
    return 0;
  }
  else
  {
    memset(fh_rx_buffer, 0, fh_rx_buffer_bytes);
    fh_rx_buffer_bytes = 0;
    return -1;
  }
}

#define RX_TIMEOUT_SECONDS 120

fh_res_t fh_rx_blocking_response_xml(void *data, size_t size, size_t *bytes_read)
{
  fh_res_t res;
  struct timeval start_time;
  double elapsed_time = 0;
  size_t xml_size;
  char *command;
  char *value;
  *bytes_read = 0;

  gettimeofday (&start_time, NULL);

  do
  {
    while (start_buf_with_xml() != 0)
    {
      elapsed_time = fh_get_delta_seconds(&start_time);
      if (elapsed_time > RX_TIMEOUT_SECONDS)
        return TRANSPORT_TIMEOUT;

      res = read_more_data();
      if (res != SUCCESS)
        return res;
    }

    xml_size = response_xml_parse(fh_rx_buffer, fh_rx_buffer_bytes,
                                  &command, &value);
    while (xml_size == 0)
    {
      elapsed_time = fh_get_delta_seconds(&start_time);
      if (elapsed_time > RX_TIMEOUT_SECONDS)
        return TRANSPORT_TIMEOUT;

      res = read_more_data();
      if (res != SUCCESS)
        return res;

    xml_size = response_xml_parse(fh_rx_buffer, fh_rx_buffer_bytes,
                                  &command, &value);
    }

    if (memcmp (command, "log", 3) == 0)
    {
      char *end = value;
      /* It is a log, find the closing quote*/
      if (end == NULL)
      {
        FH_LOGE("Target sent a log xml with no value/string%c",'.');
        fh_print_hex(fh_rx_buffer, fh_rx_buffer_bytes);
      }
      else
      {
        /* Find the closing quote. */
        while (end < &fh_rx_buffer[xml_size-7]) /* -7 for </data>*/
        {
          if (*end == '"')
          {
            /* terminate the string */
            *end = '\0';
            break;
          }
          end++;
        }
        if (*end == '\0')
        {
          FH_LOGI("TARGET SAID: '%s'", value);
        }

        /* We got a valid LOG, reset the timeout. */
        gettimeofday (&start_time, NULL);
      }
      shift_fh_rx_buffer_left (xml_size);
    }
    else if (memcmp (command, "response", 8) == 0)
    {
      size_t copied;
      copied = memscpy (data, size, fh_rx_buffer, xml_size);
      if (copied < xml_size)
      {
        FH_LOGE("Input buffer is too small %u %u", size, xml_size);
        return INVALID_PARAMETER;
      }
      shift_fh_rx_buffer_left (xml_size);
      *bytes_read = xml_size;
      return SUCCESS;
    }

    elapsed_time = fh_get_delta_seconds(&start_time);
  }while (elapsed_time < RX_TIMEOUT_SECONDS);

  return TRANSPORT_TIMEOUT;
}

fh_res_t fh_rx_blocking_raw(void *data, size_t size, size_t *bytes_read)
{
  char *d = (char *)data;
  fh_res_t res = SUCCESS;
  size_t remain = size;
  *bytes_read = 0;
  if (fh_rx_buffer_bytes != 0)
  {
    size_t copied;
    copied = memscpy (d, size, fh_rx_buffer, fh_rx_buffer_bytes);

    d += copied;
    remain -= copied;
    *bytes_read = copied;

    if (copied < fh_rx_buffer_bytes)
    {
      size_t buf_remain = fh_rx_buffer_bytes - copied;
      /* Move the data to the start of the buffer. */
      memsmove (fh_rx_buffer, sizeof(fh_rx_buffer),
                &fh_rx_buffer[copied], buf_remain);
      /* Clear the data that was just moved. */
      memset (&fh_rx_buffer[buf_remain], 0, copied);
      fh_rx_buffer_bytes = buf_remain;
    }
    else
    {
      memset (fh_rx_buffer, 0, fh_rx_buffer_bytes);
      fh_rx_buffer_bytes = 0;
    }
  }

  if (remain > 0)
  {
    size_t read;
    res = fh_transport_rx_blocking(d, remain, &read);
    if (res == SUCCESS)
      *bytes_read += read;
  }
  return res;
}

fh_res_t fh_tx_blocking(void *data, size_t size)
{
  struct fh_transfer_data *vip = &static_fh_transfer_data;
  fh_res_t res;

  if (vip->state == VIP_DISABLED)
    return fh_transport_tx_blocking(data, size);

  if (vip->state == VIP_INIT)
  {
    /* Initalize VIP, send the initial table. */
    res = fh_vip_send_signed_table();
    if (res != SUCCESS)
    {
      FH_LOGE("Failed to send the signed VIP table%c",'.');
      return res;
    }
    vip->state = VIP_SEND_DATA;
    vip->frame_sent = 0;
    vip->frames_to_next_table = 53; /* The first table has only 54 hashes, 53 of
                                       payload and the last for the hash of the next table. */
  }

  if (vip->state == VIP_SEND_NEXT_TABLE)
  {

    res = fh_vip_send_chained_table();
    if (res != SUCCESS)
    {
      FH_LOGE("Failed to send the chained VIP table%c",'.');
      return res;
    }

    vip->state = VIP_SEND_DATA;
    vip->frame_sent = 0;
    /* We will have to send TABLE_SIZE - 1 frames before we we send the next table. */
    vip->frames_to_next_table = (VIP_CHAINED_TABLE_SIZE / 32) - 1; /* SHA 256 is 32 bytes long. */
  }


  vip->frame_sent++;
  if (vip->frame_sent >= vip->frames_to_next_table)
  {
    vip->state = VIP_SEND_NEXT_TABLE;
  }

  return fh_transport_tx_blocking(data, size);
}

fh_res_t fh_transfer_close()
{
  return fh_transport_close();
}


fh_res_t fh_transfer_vip(const char *signed_table, const char *chained_tables, size_t digests_per_table)
{
  struct fh_transfer_data *vip = &static_fh_transfer_data;
  FILE *handle;
  long file_size_long;

  if (digests_per_table > 256)
  {
    FH_LOGE("Value too large for digests per table, 256 is max, given %llu",
            (long long unsigned int) digests_per_table);
    return INVALID_PARAMETER;
  }
  vip->digests_per_table = digests_per_table;
  vip->frames_to_next_table = 53;
  vip->frame_sent = 0;

  handle = fopen(signed_table, "rb");
  if (handle == NULL)
  {
    FH_LOGE("Can't open signed digest file %s", signed_table);
    return INVALID_PARAMETER;
  }
  vip->signed_table = handle;

  handle = fopen(chained_tables, "rb");
  if (handle == NULL)
  {
    FH_LOGE("Can't open chained digest tables file %s", chained_tables);
    fclose (vip->signed_table);
    return INVALID_PARAMETER;
  }
  vip->chained_table = handle;

  if (fseek(handle, 0, SEEK_END) != 0)
  {
    FH_LOGERRNO("Can't seek chained table%c", '.');
    return FILE_IO;
  }

  file_size_long = ftell(handle);
  if (file_size_long < 0)
  {
    FH_LOGERRNO("Can't get file size for chained table %ld", file_size_long);
    return FILE_IO;
  }
  rewind (handle);

  vip->chained_table_size = file_size_long;
  vip->state = VIP_INIT;
  return SUCCESS;
}

fh_res_t fh_transfer_options(uint64 options)
{
  (void) options;
  /* Set the flag to hash every packet sent to this layer. */
  return SUCCESS;
}
