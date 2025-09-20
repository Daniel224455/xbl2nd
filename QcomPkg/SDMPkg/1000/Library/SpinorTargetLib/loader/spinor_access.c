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

#include "DALFramework.h"
#include "spinor_access.h"

static char *partition_accesslist[MAX_PARTITIONS] = {
  "spinor/region1",
  "spinor/region2",
};

typedef struct
{
   uint32 num_partitions;
   spinor_partition_info partition_entries[MAX_PARTITIONS];
} spinor_partition_record;

static spinor_partition_record partition_list = {0};

spinor_partition_info *devcfg_get_partition_info(uint32 *count)
{
  DALSYSPropertyVar prop;
  DALSYS_PROPERTY_HANDLE_DECLARE(handle);
  uint32 i = 0, num_partitions = 0;

  for (i = 0; i < MAX_PARTITIONS; i++)
  {
    uint32 slot, partition, start_sector, num_sectors;
    partition = 0;
    slot = 0;
    start_sector = 0;
    num_sectors = 0;
    num_partitions = partition_list.num_partitions;

    if (DALSYS_GetDALPropertyHandleStr(partition_accesslist[i], handle) == DAL_SUCCESS)
    {
      if (DAL_SUCCESS == DALSYS_GetPropertyValue(handle, "slot", 0, &prop))
        slot = prop.Val.dwVal;

      if (DAL_SUCCESS == DALSYS_GetPropertyValue(handle, "partition", 0, &prop))
        partition = prop.Val.dwVal;

      if (DAL_SUCCESS == DALSYS_GetPropertyValue(handle, "start_sector", 0, &prop))
	start_sector = prop.Val.dwVal;

      if (DAL_SUCCESS == DALSYS_GetPropertyValue(handle, "num_sectors", 0, &prop))
        num_sectors = prop.Val.dwVal;

      if (partition == 0)
      {
        partition_list.partition_entries[num_partitions].slot = slot;
        partition_list.partition_entries[num_partitions].partition = partition;
        partition_list.partition_entries[num_partitions].start_sector = start_sector;
        partition_list.partition_entries[num_partitions].num_sectors = num_sectors;
        partition_list.num_partitions++;
      }
    }
  }
  *count = partition_list.num_partitions;
  if (*count == 0)
    return NULL;
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

