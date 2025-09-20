/***********************************************************************
 * Coldplug Null Block Device.
 *
 * Coldplug
 * Copyright (C) 2015 QUALCOMM Technologies, Inc.
 *
 * Coldplug null block device functions.
 *
 ***********************************************************************/

/*===========================================================================

  EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/boot.xf/2.0/QcomPkg/Library/ColdplugLib/src/coldplug_bdev_devnull.h#1 $ $DateTime: 2017/06/30 21:09:49 $ $Author: pwbldsvc $

  when         who   what, where, why
  ----------   ---   ---------------------------------------------------------
  2015-06-15   rp    Create

 ===========================================================================*/

#ifndef __COLDPLUG_BDEV_DEVNULL__H__
#define __COLDPLUG_BDEV_DEVNULL__H__

#include "coldplug_config_i.h"
#include "coldplug_bdev_ops.h"

struct coldplug_bdev_ops* coldplug_bdev_devnull_get_ops(void);

#endif /* __COLDPLUG_BDEV_DEVNULL__H__ */
