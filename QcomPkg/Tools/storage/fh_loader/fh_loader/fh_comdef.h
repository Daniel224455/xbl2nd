/******************************************************************************
 * Common type definitions.
 *
 * Copyright (c) 2016,2019 Qualcomm Technologies, Inc.
 * All Rights Reserved.
 * Qualcomm Proprietary
 *
 *****************************************************************************/

/*=============================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/boot.xf/3.0.1.c1/QcomPkg/Tools/storage/fh_loader/fh_loader/fh_comdef.h#1 $
  $DateTime: 2021/04/29 02:01:56 $
  $Author: pwbldsvc $

when         who   what, where, why
----------   ---   ------------------------------------------------------------
2019-05-21   wek   Added Firehose error codes.
2016-01-15   wek   Create.

=============================================================================*/

#ifndef __COMDEF_H__
#define __COMDEF_H__

#include <stdint.h>
#include <stddef.h>

typedef int8_t        int8;
typedef int16_t       int16;
typedef int32_t       int32;
typedef int64_t       int64;
typedef uint8_t       uint8;
typedef uint16_t      uint16;
typedef uint32_t      uint32;
typedef uint64_t      uint64;
typedef int8_t        INT8;
typedef int16_t       INT16;
typedef int32_t       INT32;
typedef int64_t       INT64;
typedef uint8_t       UINT8;
typedef uint16_t      UINT16;
typedef uint32_t      UINT32;
typedef uint64_t      UINT64;

#define bool char

enum firehose_errors
{
  SUCCESS = 0,
  INVALID_PARAMETER,
  NOT_SUPPORTED,
  TRANSPORT_TIMEOUT,
  OPEN_PORT_FAILED,
  CLOSE_PORT_ERROR,
  READ_PORT_TIMEOUT,
  READ_PORT_ERROR,
  WRITE_PORT_TIMEOUT,
  WRITE_PORT_ERROR,
  NOT_INITALIZED,
  SET_TIMEOUT_ERROR,
  FILE_IO,
  NO_MEMORY,
  TARGET_NAK,
};

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

typedef enum firehose_errors fh_res_t;


#endif /* __COMDEF_H__ */
