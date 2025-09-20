/***********************************************************************
 * Coldplug GPT module.
 *
 * Coldplug
 * Copyright (C) 2017 QUALCOMM Technologies, Inc.
 *
 * Coldplug GPT module.
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

#ifndef __COLDPLUG_GPT_H__
#define __COLDPLUG_GPT_H__

#include "coldplug_config_i.h"
#include "coldplug_api.h"
#include "coldplug_device.h"
#include "coldplug_pcache.h"

int coldplug_gpt_parse(struct coldplug_lun *clun,
			struct coldplug_pcache_data *data_cache);

struct coldplug_handle* coldplug_open_gpt_partition_i(
	enum coldplug_device_type dev_type, int is_embedded_device,
	const struct coldplug_guid *guid);

int coldplug_gpt_partition_is_fat(struct coldplug_partition *cparti);

#endif /* __COLDPLUG_GPT_H__ */
