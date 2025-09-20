#ifndef __SPINOR_OSAL__H
#define __SPINOR_OSAL__H
/**********************************************************************
 *spinor_osal.h
 *
 * This file provides definitions for the spinor platform specific functions
 *
 * Copyright (c) 2017
 * Qualcomm Technologies Incorporated.
 * All Rights Reserved.
 * Qualcomm Confidential and Proprietary
 *
 **********************************************************************/
/*======================================================================

                        EDIT HISTORY FOR MODULE


when         who      what, where, why
----------   ------   ----------------------------------------------- 
2017-05-25  md        Initial Release

==================================================================*/
#include <stdlib.h>
#include "com_dtypes.h"
#include <string.h>
#include "stringl/stringl.h"
#include "LoaderUtils.h"
#include "boot_cache.h"
#include "spinor_spial.h"



#ifndef PACKED
  #define PACKED
#endif

#ifndef PACKED_POST
  #define PACKED_POST __attribute__((__packed__))
#endif

#define FLUSH                 1
#define INVAL                 2
#define CLEAN                 4

/* Cache management wrapper */

void spinor_cache_op (void *addr, uint32_t len, uint32_t op);

#endif /* __SPINOR_OSAL__H */

