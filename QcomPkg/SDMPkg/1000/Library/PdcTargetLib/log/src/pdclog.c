/*============================================================================
  FILE:         pdclog.c

  OVERVIEW:     This file implements the functions for logging messages.

  DEPENDENCIES: None

  Copyright (c) 2016-2018 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies, Inc. and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of Qualcomm Technologies, Inc.
=============================================================================
============================================================================*/
#ifdef ENABLE_PDC_LOG

#include "comdef.h"
#include "CoreVerify.h"
#include "ULog.h"
#include "ULogFront.h"

/*===========================================================================
 *                            TYPES AND DEFINITIONS
 *===========================================================================*/
#define PDC_LOG_SIZE  2048

/*===========================================================================
 *                            INTERNAL VARIABLES
 *===========================================================================*/
static ULogHandle g_pdcLogHandle = NULL;

/*===========================================================================
 *                         GLOBAL FUNCTION DEFINITIONS
 *===========================================================================*/
/**
 * pdcLog_initialize
 *
 * @brief Initializes the log system for the driver
 *
 * @note called from RCINIT
 */
void pdcLog_initialize(void)
{
  ULOG_LOCK_TYPE log_lock = ULOG_LOCK_OS;

  if(NULL == g_pdcLogHandle)
  {
    /* Creating a log handle */
    CORE_VERIFY(ULOG_ERR_INITINCOMPLETE ==
                ULogFront_RealTimeInit(&g_pdcLogHandle, "PDC Log",
                                       0, ULOG_MEMORY_LOCAL, log_lock));

    /* Log header setup */
    ULogCore_HeaderSet(g_pdcLogHandle,
                       "Content-Type: text/pdc-driver-1.0; title=PDC Driver");

    /* Allocating required memory */
    ULogCore_Allocate(g_pdcLogHandle, PDC_LOG_SIZE);

    /* Enabling the log */
    ULogCore_Enable(g_pdcLogHandle);
  }

  return;
}

/*
 * pdcLog_printf
 */
void pdcLog_printf(int arg_count, const char *format, ...)
{
  va_list args;

  if(NULL != g_pdcLogHandle)
  {
    va_start(args, format);
    ULogFront_RealTimeVprintf(g_pdcLogHandle, arg_count, format, args);
    va_end(args);
  }

  return;
}

#else

/* Logging is not supported on this target */
void pdcLog_printf(int arg_count, const char *format, ...)
{
  return;
}

#endif /* ENABLE_PDC_LOG */
