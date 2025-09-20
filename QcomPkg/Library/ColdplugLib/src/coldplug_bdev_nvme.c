/***********************************************************************
 * Coldplug NVME Block Device.
 *
 * Coldplug
 * Copyright (C) 2018-2019, 2021 QUALCOMM Technologies, Inc.
 *
 * Coldplug NVME Block Device.
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
  2018-11-28   rp    Create

 ===========================================================================*/

#include "coldplug_config_i.h"

#ifdef COLDPLUG_NVME_ENABLE

#include "coldplug_bdev_ops.h"
#include "coldplug_device.h"
#include "coldplug_assert.h"
#include "coldplug_api.h"
#include "coldplug_util.h"
#include "coldplug_debug.h"

#include "nvme_api.h"

#include <string.h>

#ifndef COLDPLUG_BDEV_NVME_DEFAULT_SLOT_NO
#define COLDPLUG_BDEV_NVME_DEFAULT_SLOT_NO (0)
#endif

#ifndef COLDPLUG_BDEV_NVME_RESULT_DUMMY_VAL
#define COLDPLUG_BDEV_NVME_RESULT_DUMMY_VAL (0xA1B2C3D4)
#endif


#ifdef COLDPLUG_NVME_DEBUG_DISABLE

void static inline void coldplug_bdev_nvme_debug_add(
		enum coldplug_debug_id debug_id, uint32 nvme_res,
		uint32 val1, uint32 val2)
{
	(void) debug_id; (void) nvme_res; (void) val1; (void) val2;
}

#else

static void coldplug_bdev_nvme_debug_add(enum coldplug_debug_id debug_id,
		uint32 nvme_res, uint32 val1, uint32 val2)
{
	struct coldplug_debug_info_type debug_info;

	memset(&debug_info, 0, sizeof(debug_info));
	debug_info.debug_id = debug_id;
	debug_info.dev_type = COLDPLUG_DEVICE_NVME;
	debug_info.result = (int)nvme_res;
	debug_info.val1 = val1;
	debug_info.val2 = val2;

	coldplug_debug_add(&debug_info);
}

#endif

static int coldplug_bdev_nvme_ops_get_slot_and_lun_config(uint32 *max_slots,
		uint32 *max_luns, uint32 *boot_lun_no)
{
	struct nvme_device_info dev_info;
	uint32 slot_no;
	uint32 nvme_errno = COLDPLUG_BDEV_NVME_RESULT_DUMMY_VAL;

	COLDPLUG_ASSERT(max_slots != NULL);
	COLDPLUG_ASSERT(max_luns != NULL);
	COLDPLUG_ASSERT(boot_lun_no != NULL);

	memset(&dev_info, 0, sizeof(dev_info));

	slot_no = COLDPLUG_BDEV_NVME_DEFAULT_SLOT_NO;
	nvme_errno = nvme_get_device_info(slot_no, &dev_info);

	coldplug_bdev_nvme_debug_add(COLDPLUG_DEBUG_ID_GET_SLOT_CONFIG,
		nvme_errno, slot_no, dev_info.num_namespace);

	if (nvme_errno != NVME_SUCCESS)
		goto End;

	*max_slots = COLDPLUG_MAX_NVME_SLOTS;
	/* +1 because as per NVME spec namespace ID 0 is invalid, and valid
	 * namespace IDs are in the range 1 to max-namespace inclusive */
	*max_luns = (dev_info.num_namespace + 1);
	*boot_lun_no = -1;

End:
	return 0;
}

static int coldplug_bdev_nvme_ops_open(void **bdev_hdl, uint32 slot_no,
		uint32 lun_no)
{
	struct nvme_handle *nvme_hdl;
	uint32 nvme_errno = COLDPLUG_BDEV_NVME_RESULT_DUMMY_VAL;
	int result = COLDPLUG_RESULT_ENODEV;

	COLDPLUG_ASSERT(bdev_hdl != NULL);

	if (lun_no == 0)
		goto End; /* namespace 0 is invalid as per NVME spec*/

	nvme_hdl = nvme_open(slot_no, lun_no, &nvme_errno);
	if (nvme_hdl == NULL) {
		coldplug_bdev_nvme_debug_add(COLDPLUG_DEBUG_ID_OPEN,
			nvme_errno, slot_no, lun_no);
		goto End;
	}

	*bdev_hdl = (void *) nvme_hdl;

	result = COLDPLUG_RESULT_SUCCESS;

End:
	return result;
}

int coldplug_bdev_nvme_ops_close(void *bdev_hdl)
{
	struct nvme_handle *nvme_hdl = NULL;
	uint32 nvme_errno = COLDPLUG_BDEV_NVME_RESULT_DUMMY_VAL;
	int result = COLDPLUG_RESULT_ENODEV;

	COLDPLUG_ASSERT(bdev_hdl != NULL);
	nvme_hdl = (struct nvme_handle *) bdev_hdl;

	nvme_errno = nvme_close(nvme_hdl);
	if (nvme_errno != NVME_SUCCESS) {
		coldplug_bdev_nvme_debug_add(COLDPLUG_DEBUG_ID_CLOSE,
			nvme_errno, 0, 0);
		goto End;
	}

	result = COLDPLUG_RESULT_SUCCESS;

End:
	return result;
}

static int coldplug_bdev_nvme_ops_read(void *bdev_hdl, uint64 start_lba,
		uint64 count, uint8 *buff)
{
	struct nvme_handle *nvme_hdl = NULL;
	uint32 nvme_result;
	int result = COLDPLUG_RESULT_ENODEV;

	COLDPLUG_ASSERT(bdev_hdl != NULL);
	nvme_hdl = (struct nvme_handle *) bdev_hdl;

	COLDPLUG_ASSERT(buff != NULL);

	nvme_result = nvme_read(nvme_hdl, buff, start_lba, count);
	if (nvme_result != NVME_SUCCESS) {
		coldplug_bdev_nvme_debug_add(COLDPLUG_DEBUG_ID_READ,
			nvme_result, (uint32)start_lba, (uint32)count);
		goto End;
	}

	result = COLDPLUG_RESULT_SUCCESS;

End:
	return result;
}

static int coldplug_bdev_nvme_ops_write(void *bdev_hdl, uint64 start_lba,
                             uint64 count, uint8 *buff)
{
	struct nvme_handle *nvme_hdl = NULL;
	uint32 nvme_result;
	int result = COLDPLUG_RESULT_ENODEV;

	COLDPLUG_ASSERT(bdev_hdl != NULL);
	nvme_hdl = (struct nvme_handle *) bdev_hdl;

	COLDPLUG_ASSERT(buff != NULL);

	nvme_result = nvme_write(nvme_hdl, buff, start_lba, count);
	if (nvme_result != NVME_SUCCESS) {
		coldplug_bdev_nvme_debug_add(COLDPLUG_DEBUG_ID_WRITE,
			nvme_result, (uint32)start_lba, (uint32)count);
		goto End;
	}

	result = COLDPLUG_RESULT_SUCCESS;

End:
	return result;
}

static int coldplug_bdev_nvme_ops_flush(void *bdev_hdl)
{
	struct nvme_handle *nvme_hdl = NULL;
	uint32 nvme_result;
	int result = COLDPLUG_RESULT_ENODEV;

	COLDPLUG_ASSERT(bdev_hdl != NULL);
	nvme_hdl = (struct nvme_handle *) bdev_hdl;

	nvme_result = nvme_flush(nvme_hdl);
	if (nvme_result != NVME_SUCCESS) {
		coldplug_bdev_nvme_debug_add(COLDPLUG_DEBUG_ID_FLUSH,
			nvme_result, 0, 0);
		goto End;
	}

	result = COLDPLUG_RESULT_SUCCESS;
End:
	return result;
}


static int coldplug_bdev_nvme_ops_get_size(void *bdev_hdl, uint32 *lba_size,
		uint64 *total_lba)
{
	struct nvme_handle *nvme_hdl = NULL;
	struct nvme_namespace_sizes ns_info;
	uint32 nvme_result;
	int result = COLDPLUG_RESULT_ENODEV;

	COLDPLUG_ASSERT(bdev_hdl != NULL);
	nvme_hdl = (struct nvme_handle *) bdev_hdl;

	COLDPLUG_ASSERT(lba_size != NULL);
	COLDPLUG_ASSERT(total_lba != NULL);

	memset(&ns_info, 0, sizeof(ns_info));
	nvme_result = nvme_get_namespace_sizes(nvme_hdl, &ns_info);
	if (nvme_result != NVME_SUCCESS) {
		coldplug_bdev_nvme_debug_add(COLDPLUG_DEBUG_ID_GET_INFO,
			nvme_result, 0, 0);
		goto End;
	}

	*lba_size = ns_info.block_size;
	*total_lba = ns_info.size_blocks;

	result = COLDPLUG_RESULT_SUCCESS;

End:
	return result;
}

static int coldplug_bdev_nvme_ops_get_info(void *bdev_hdl,
		struct coldplug_device_info *dev_info)
{
	struct nvme_handle *nvme_hdl = NULL;
	struct nvme_device_info dev_info_i;
	struct nvme_namespace_sizes ns_info;
	uint32 nvme_errno;
	int result = COLDPLUG_RESULT_ENODEV;
	uint32 slot_no = COLDPLUG_BDEV_NVME_DEFAULT_SLOT_NO;

	COLDPLUG_ASSERT(bdev_hdl != NULL);
	nvme_hdl = (struct nvme_handle *) bdev_hdl;

	COLDPLUG_ASSERT(dev_info != NULL);
	memset(dev_info, 0, sizeof(*dev_info));

	memset(&dev_info_i, 0, sizeof(dev_info_i));
	nvme_errno = nvme_get_device_info(slot_no, &dev_info_i);
	if (nvme_errno != NVME_SUCCESS) {
		coldplug_bdev_nvme_debug_add(COLDPLUG_DEBUG_ID_GET_INFO,
			nvme_errno, slot_no, 0);
		goto End;
	}

	dev_info->is_embedded_device = 1;
	dev_info->oem_id = (uint32)dev_info_i.vendor_id;
	dev_info->manufacturer_id = (uint32)dev_info_i.subsys_vid;
	coldplug_memscpy(dev_info->product_serial_number_string,
		sizeof(dev_info->product_serial_number_string),
		dev_info_i.serial, sizeof(dev_info_i.serial));
	coldplug_memscpy(dev_info->product_name,
		sizeof(dev_info->product_name),
		dev_info_i.model_num, sizeof(dev_info->product_name));

	memset(&ns_info, 0, sizeof(ns_info));
	nvme_errno = nvme_get_namespace_sizes(nvme_hdl, &ns_info);
	if (nvme_errno != 0) {
		coldplug_bdev_nvme_debug_add(COLDPLUG_DEBUG_ID_GET_INFO,
			nvme_errno, 0, 0);
		goto End;
	}

	dev_info->bytes_per_lba = ns_info.block_size;
	dev_info->total_lba_count = ns_info.size_blocks;

	result = COLDPLUG_RESULT_SUCCESS;

End:
	return result;
}

static int coldplug_bdev_nvme_ops_set_pon_wp(void *bdev_hdl,
		uint64 start_lba, uint64 total_lba)
{
	(void) bdev_hdl; (void) start_lba; (void) total_lba;
	return COLDPLUG_RESULT_NOT_SUPPORTED;
}

static int coldplug_bdev_nvme_ops_erase(void *bdev_hdl, uint64 start_lba_arg,
		uint64 total_lba_arg)
{
	(void) bdev_hdl; (void) start_lba_arg; (void) total_lba_arg;
	return COLDPLUG_RESULT_NOT_SUPPORTED;
}

static int coldplug_bdev_nvme_ops_format(void *bdev_hdl)
{
	(void) bdev_hdl;
	return COLDPLUG_RESULT_NOT_SUPPORTED;
}

static int coldplug_bdev_nvme_ops_set_bootable_partition(uint32 slot_no, uint32 lun_no)
{
	(void) slot_no; (void) lun_no;
	return COLDPLUG_RESULT_NOT_SUPPORTED;
}

static int coldplug_bdev_nvme_ops_get_bootable_partition(uint32 slot_no, uint32 *out_lun_no)
{
	(void) slot_no; (void) out_lun_no;
	return COLDPLUG_RESULT_NOT_SUPPORTED;
}

static int coldplug_bdev_background_init_start(uint32 slot_no,
		uint32 init_context)
{
	(void) slot_no; (void) init_context;
	return COLDPLUG_RESULT_NOT_SUPPORTED;
}

static int coldplug_bdev_background_init_finish(uint32 slot_no,
		uint32 init_context)
{
	(void) slot_no; (void) init_context;
	return COLDPLUG_RESULT_NOT_SUPPORTED;
}

static struct coldplug_bdev_ops coldplug_bdev_nvme_ops = {
	coldplug_bdev_nvme_ops_get_slot_and_lun_config,
	coldplug_bdev_nvme_ops_open,
	coldplug_bdev_nvme_ops_close,
	coldplug_bdev_nvme_ops_read,
	coldplug_bdev_nvme_ops_write,
	coldplug_bdev_nvme_ops_flush,
	coldplug_bdev_nvme_ops_get_size,
	coldplug_bdev_nvme_ops_get_info,
	coldplug_bdev_nvme_ops_set_pon_wp,
	coldplug_bdev_nvme_ops_erase,
	coldplug_bdev_nvme_ops_format,
	coldplug_bdev_nvme_ops_set_bootable_partition,
	coldplug_bdev_nvme_ops_get_bootable_partition,
	coldplug_bdev_background_init_start,
	coldplug_bdev_background_init_finish,
};

struct coldplug_bdev_ops* coldplug_bdev_nvme_get_ops(void)
{
	return &coldplug_bdev_nvme_ops;
}

void coldplug_bdev_nvme_init(void)
{
}

#endif /* COLDPLUG_NVME_ENABLE */
