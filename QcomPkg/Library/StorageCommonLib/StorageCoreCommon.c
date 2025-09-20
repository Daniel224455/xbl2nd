/**********************************************************************
* boot_common_core.c
*
* Common services for XBL core
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

#include <Uefi.h>
#include <Library/PcdLib.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/QcomBaseLib.h>


#include "StorageBootCommon.h"

// Wrapper for UART logging
void boot_common_log_message(char *str)
{
   DEBUG ((EFI_D_ERROR, "%a\n", str));
}

// Wrapper for Time in us
UINT32 boot_common_get_time()
{
  return GetTimerCountus();
}

