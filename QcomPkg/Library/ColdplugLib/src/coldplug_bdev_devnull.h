/***********************************************************************
 * Coldplug Null Block Device.
 *
 * Coldplug
 * Copyright (C) 2017 QUALCOMM Technologies, Inc.
 *
 * Coldplug null block device functions.
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

#ifndef __COLDPLUG_BDEV_DEVNULL__H__
#define __COLDPLUG_BDEV_DEVNULL__H__

#include "coldplug_config_i.h"
#include "coldplug_bdev_ops.h"

struct coldplug_bdev_ops* coldplug_bdev_devnull_get_ops(void);

#endif /* __COLDPLUG_BDEV_DEVNULL__H__ */
