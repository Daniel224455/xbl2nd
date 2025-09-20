/***********************************************************************
 * Coldplug Log module.
 *
 * Coldplug
 * Copyright (C) 2017 QUALCOMM Technologies, Inc.
 *
 * Coldplug Log module functions.
 *
 ***********************************************************************/

/*===========================================================================

  EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  when         who   what, where, why
  ----------   ---   ---------------------------------------------------------
  2017-05-18   rp    Create

 ===========================================================================*/

#include "coldplug_config_i.h"
#include "coldplug_log.h"

#ifdef COLDPLUG_LOG_ENABLE

#include "boot_logger.h"

void coldplug_log(const char *filename, int line_no, char *msg)
{
	(void) filename; (void) line_no;
	boot_log_message(msg);
}

#endif /* COLDPLUG_LOG_DISABLE */
