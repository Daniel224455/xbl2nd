/***********************************************************************
 * Coldplug NVME Block Device.
 *
 * Coldplug
 * Copyright (C) 2018 QUALCOMM Technologies, Inc.
 *
 * Coldplug NVME Device module.
 *
 ***********************************************************************/

/*===========================================================================

  EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  when         who   what, where, why
  ----------   ---   ---------------------------------------------------------
  2018-11-28   rp    Create

 ===========================================================================*/

#ifndef __COLDPLUG_BDEV_NVME__H__
#define __COLDPLUG_BDEV_NVME__H__

#include "coldplug_config_i.h"
#include "coldplug_bdev_ops.h"

#ifdef COLDPLUG_NVME_ENABLE

void coldplug_bdev_nvme_init(void);
struct coldplug_bdev_ops* coldplug_bdev_nvme_get_ops(void);

#else

static inline void coldplug_bdev_nvme_init(void)
{
}

static inline struct coldplug_bdev_ops* coldplug_bdev_nvme_get_ops(void)
{
	return NULL;
}

#endif /* COLDPLUG_NVME_ENABLE */

#endif /* __COLDPLUG_BDEV_NVME__H__ */
