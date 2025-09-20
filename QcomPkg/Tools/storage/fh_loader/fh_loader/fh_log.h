/**************************************************************************
 * FILE: fh_log.h
 *
 * Provides logging to firehose loader.
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

  $Header: //components/rel/boot.xf/3.0.1.c1/QcomPkg/Tools/storage/fh_loader/fh_loader/fh_log.h#1 $
  $DateTime: 2021/04/29 02:01:56 $
  $Author: pwbldsvc $

when         who   what, where, why
----------   ---   ---------------------------------------------------------
2019-05-16   wek   Create.

===========================================================================*/

#ifndef __FH_LOG_H__
#define __FH_LOG_H__

#include <errno.h>
#include <string.h>
#include <stdarg.h>

#define LEN_ERRNO_LOG_STR      20 /* Length of buffers to hold the string for errornum. */

/* Define all the log macros as logMessage. This should actually map to a
 * specific function to raise or reduce the verbosity level. */
#define FH_LOGA(f, ...)  fh_log(LOG_ALWAYS, __FUNCTION__, __LINE__, f, __VA_ARGS__) /* Error */
#define FH_LOGE(f, ...)  fh_log(LOG_ERROR, __FUNCTION__, __LINE__, f, __VA_ARGS__) /* Error */
#define FH_LOGERRNO(f, ...)  do {                                       \
    char *errno_str;                                                    \
    errno_str = strerror (errno);                                       \
    fh_log(LOG_ERROR, __FUNCTION__, __LINE__, "errno(%d)%s - "f"", errno, errno_str, __VA_ARGS__); \
  } while(0)
#define FH_LOGW(f, ...)  fh_log(LOG_WARN, __FUNCTION__, __LINE__,  f, __VA_ARGS__) /* Warning */
#define FH_LOGI(f, ...)  fh_log(LOG_INFO, __FUNCTION__, __LINE__,  f, __VA_ARGS__) /* Info */
#define FH_LOGD(f, ...)  fh_log(LOG_DEBUG, __FUNCTION__, __LINE__, f, __VA_ARGS__) /* Debug, most verbose. */
#define FH_LOGFO(f, ...)  fh_log(LOG_DEBUG, __FUNCTION__, __LINE__, f, __VA_ARGS__) /* Log to file only. */

void MyLog (int log_level, const char *func_name, int line_number, const char *format, ...);
void printBuffer (const unsigned char *buffer, unsigned int length, unsigned int MaxLength, const char *func_name, int line_number);

void fh_print_hex(void *buffer, size_t len);

enum fh_log_lvl
{
  LOG_NONE, /* Disable all logs. */
  LOG_ALWAYS,
  LOG_ERROR,
  LOG_WARN,
  LOG_INFO,
  LOG_DEBUG,
  LOG_FILE_ONLY, /* Put this log into the log file, not the console. */
  LOG_MAX_LIMIT
};

#include <stdio.h> /* for vsnprintf, can be removed when fh_log is cleaned up */
static void fh_log (enum fh_log_lvl lvl, const char *func_name, int line_number, const char *log, ...)
{
  va_list args;
  char log_line[4096];
  va_start (args, log);
  vsnprintf (log_line, sizeof (log_line), log, args);
  MyLog(lvl, func_name, line_number, log_line);
  va_end(args);
}

#endif /* __FH_LOG_H__ */
