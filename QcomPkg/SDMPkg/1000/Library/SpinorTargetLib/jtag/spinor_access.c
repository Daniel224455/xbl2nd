/**********************************************************************
 * FILE: spinor_access.c
 *
 * Get the regions that need protection (block IO Access)
 *
 * Copyright (c) 2018
 * Qualcomm Technologies Incorporated.
 * All Rights Reserved.
 * Qualcomm Confidential and Proprietary
 *
 **********************************************************************/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.


when         who   what, where, why
----------   ---   ---------------------------------------------------------
2018-06-20   wek   Create.

===========================================================================*/

#include "spinor_access.h"

typedef struct
{
   uint32 num_partitions;
   spinor_partition_info partition_entries[MAX_PARTITIONS];
} spinor_partition_record;

static spinor_partition_record partition_list = {0};

spinor_partition_info *devcfg_get_partition_info(uint32 *count)
{
  uint32 i = 0, num_partitions = 0;

  for (i = 0; i < MAX_PARTITIONS; i++)
  {
    num_partitions = partition_list.num_partitions;

    partition_list.partition_entries[num_partitions].slot = 0;
    partition_list.partition_entries[num_partitions].partition = 0;
    partition_list.partition_entries[num_partitions].start_sector = 0;
    partition_list.partition_entries[num_partitions].num_sectors = 0;
    partition_list.num_partitions++;
  }
  *count = MAX_PARTITIONS;
  return &partition_list.partition_entries[0];
}

spinor_partition_info *spinor_devcfg_protected_regions(uint32 *count)
{
  spinor_partition_info *part;
  if (partition_list.num_partitions == 0)
  {
    part = devcfg_get_partition_info(count);
  }
  else
  {
    part = &partition_list.partition_entries[0];
    *count = partition_list.num_partitions;
  }
  return part;
}

