#ifndef STORAGEBOOTCOMMON_H
#define STORAGEBOOTCOMMON_H

/*=============================================================================

                            Storage Boot Common APIs

GENERAL DESCRIPTION
  This file provides common API interface for different boot components.

Copyright 2018 by Qualcomm Technologies, Inc.  All Rights Reserved.
=============================================================================*/
/*===========================================================================

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.


YYYY-MM-DD   who     what, where, why
----------------------------------------------------------------------
2018-06-11   bb      Initial creation
===========================================================================*/

/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/

// Wrapper for UART logging
void boot_common_log_message(char *str);

// Wrapper for Time in us
UINT32 boot_common_get_time();

#endif /* STORAGEBOOTCOMMON_H */

