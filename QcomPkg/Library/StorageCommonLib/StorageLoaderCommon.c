/**********************************************************************
* boot_common_loader.c
*
* Common services for XBL Loader
*
* Copyright (c) 2018 Qualcomm Technologies, Inc.  All Rights Reserved.
* Qualcomm Technologies Proprietary and Confidential.
*
*
**********************************************************************/

/*=======================================================================
                        Edit History


YYYY-MM-DD   who     what, where, why
----------------------------------------------------------------------
2018-06-11   bb      Initial creation
===========================================================================*/

#include "boot_logger.h"
#include "boot_logger_timer.h"
#include "StorageBootCommon.h"

// Wrapper for UART logging
void boot_common_log_message(char *str)
{
   boot_log_message(str);
}

// Wrapper for Time in us
uint32 boot_common_get_time()
{
  return boot_get_time_count();
}
