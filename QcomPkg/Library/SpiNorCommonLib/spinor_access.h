/**********************************************************************
 * spinor_access.h
 *
 * Get the regions that need protection (block IO Access)
 *
 * Copyright (c) 2018
 * Qualcomm Technologies Incorporated.
 * All Rights Reserved.
 * Qualcomm Confidential and Proprietary
 *
 **********************************************************************/
/*=======================================================================
                        Edit History


when        who   what, where, why
----------  ---   ---------------------------------------------------
2018-06-20  wek   Create.

======================================================================*/
#include "comdef.h"

#define MAX_PARTITIONS 2

typedef struct
{
   uint32 slot;
   uint32 partition;
   uint64 start_sector;
   uint64 num_sectors;
} spinor_partition_info;

/* Get the list of regions that need to be protected from IO. */
spinor_partition_info *spinor_devcfg_protected_regions(uint32 *count);
