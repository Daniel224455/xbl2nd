/**************************************************************************
 * FILE: devprg_firehose.h
 *
 * Firehose protocol handler.
 *
 * Copyright (c) 2016-2018 Qualcomm Technologies, Inc.
 * All Rights Reserved.
 * Qualcomm Proprietary
 *
 *************************************************************************/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.


when         who   what, where, why
----------   ---   ---------------------------------------------------------
2018-10-04   wek   Moved power/reset modes to header file.
2016-11-04   wek   Create. (20-m)

===========================================================================*/

#ifndef __DEVPRG_FIREHOSE_H__
#define __DEVPRG_FIREHOSE_H__

#include "stddef.h"
#include "comdef.h"

enum devprg_status
{
  DEVPRG_SUCCESS = 0,
  DEVPRG_ERROR_NOMEM = 1,
  DEVPRG_ERROR_INVAL_PARAM,
  DEVPRG_ERROR_STORAGE, /* Untranslated error from storage. */
  DEVPRG_ERROR_NOT_SUPPORTED, /* Operation not supported. */
  DEVPRG_ERROR_TRANSMIT,
  DEVPRG_ERROR_RECEIVE,
  DEVPRG_ERROR_AUTHENTICATION,
  DEVPRG_ERROR_NOT_INIT,
  DEVPRG_ERROR_OUT_OF_GOOD_BLOCKS,
  DEVPRG_ERROR_ACCESS_DENIED,
  DEVPRG_ERROR_NOT_READY,
};

typedef enum devprg_status dp_res_t; /* Device Programmer result type, maps to enum status */

#define DEVPRG_MAX_TRANSFER_BUFFERS  2 /* 2 to ping-pong on write. */
#define DEVPRG_BUFFER_ALIGNMENT 128 /* To what byte boundary to align the buffers*/
#define DEVPRG_MAX_LOG_SIZE 512  /* Maximum size of a single log line */

int handle_xml(char *xml_buffer, size_t size);

enum devprg_reboot_type
{
  DEVPRG_REBOOT_NONE,      /* Do nothing, don't reboot the device*/
  DEVPRG_REBOOT_RESET,     /* Reset/reboot the device. */
  DEVPRG_REBOOT_POWER_OFF, /* Power Off the device. */
  DEVPRG_REBOOT_EDL,       /* Reboot into EDL. */
  DEVPRG_REBOOT_MAX,       /* Max value of ENUM, to bound check. */
};

/* Get the buffer that is intended to be used to receive XML commands. */
char *devprg_get_xml_buffer(size_t *size);
/* Get the buffer that is intended to be used to receive Log through USB. */
char *devprg_get_log_buffer(size_t *size);
/* Get the buffer that is intended to be used to receive VIP table. */
char *devprg_get_vip_buffer(size_t *size);
/* Get one transfer buffer. */
char *devprg_get_transfer_buffer(size_t *size);
/* Free one of the buffers requested on devprg_get_transfer_buffer(). */
void devprg_free_transfer_buffer(char *buffer_to_free);
/* Initialize any data structures associated with the buffers. */
void devprg_buffers_init(void);
/* Calculate CRC32 over a buffer. Used only for Patch. */
uint32 devprg_calc_crc32(const uint8 *data, uint32 nbytes);

void main_loop(void);

#endif /*  __DEVPRG_FIREHOSE_H__ */
