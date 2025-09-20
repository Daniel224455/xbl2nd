/***********************************************************************
 * Coldplug FAT module.
 *
 * Coldplug
 * Copyright (C) 2017 QUALCOMM Technologies, Inc.
 *
 * Coldplug FAT module.
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

#ifndef __COLDPLUG_FAT_H__
#define __COLDPLUG_FAT_H__

#include "coldplug_config_i.h"
#include "coldplug_api.h"
#include "coldplug_device.h"
#include "coldplug_pcache.h"

int coldplug_fat_parse(struct coldplug_lun *clun,
	struct coldplug_pcache_data *data_cache);

#endif /* __COLDPLUG_FAT_H__ */
