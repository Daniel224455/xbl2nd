/***********************************************************************
 * coldplug
 *
 * Coldplug Block Device
 * Copyright (C) 2017-2019, 2021 QUALCOMM Technologies, Inc.
 *
 * Coldplug Block Device Operations module
 *
 ***********************************************************************/

/*===========================================================================

  EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  when         who   what, where, why
  ----------   ---   ---------------------------------------------------------
  2021-03-18   gk    Add set and get bootable partition support from Makena
  2019-01-09   rp    Add flush support.
  2017-05-18   rp    Create

  ===========================================================================*/

#ifndef __COLDPLUG_BDEV_OPS_H__
#define __COLDPLUG_BDEV_OPS_H__

#include "coldplug_config_i.h"
#include "coldplug_api.h"

struct coldplug_bdev_ops {
	int (*get_slot_and_lun_config) (uint32 *max_slots, uint32 *max_luns,
					uint32 *boot_lun_no);

	int (*open) (void **bdev_hdl, uint32 slot_no, uint32 lun_no);
	int (*close) (void *bdev_hdl);
	int (*read) (void *bdev_hdl, uint64 lba, uint64 count, uint8 *buff);
	int (*write) (void *bdev_hdl, uint64 lba, uint64 count, uint8 *buff);
	int (*flush) (void *bdev_hdl);

	int (*get_size) (void *bdev_hdl, uint32 *lba_size, uint64 *total_lba);
	int (*get_info) (void *bdev_hdl,
			struct coldplug_device_info *device_info);

	int (*set_pon_wp) (void *bdev_hdl, uint64 start_lba, uint64 total_lba);
	int (*erase) (void *bdev_hdl, uint64 start_lba, uint64 total_lba);
	int (*format) (void *bdev_hdl);
	int (*set_bootable_partition) (uint32 slot_no, uint32 lun_no);
	int (*get_bootable_partition) (uint32 slot_no, uint32 *out_lun_no);

	int (*background_init_start)(uint32 slot_no, uint32 init_context);
	int (*background_init_finish)(uint32 slot_no, uint32 init_context);
};

struct coldplug_bdev_ops* coldplug_bdev_ops_get(
		enum coldplug_device_type dev_type);

#endif /* __COLDPLUG_BDEV_OPS_H__ */
