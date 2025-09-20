/**************************************************************************
 * FILE: devprg_storage_nvme.c
 *
 * NVMe device interface.
 *
 * Copyright (c) 2015-2018 Qualcomm Technologies, Inc.
 * All Rights Reserved.
 * Qualcomm Proprietary
 *
 *************************************************************************/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.


when         who   what, where, why
----------   ---   ---------------------------------------------------------
2019-02-08   wek   Create (2-Y)

===========================================================================*/

#ifdef FEATURE_DEVPRG_NVME

#include "devprg_storage.h"
#include "devprg_storage_nvme.h"
#include "devprg_log.h"
#include "devprg_utils.h"
#include "nvme_api.h"
#include "stringl/stringl.h"
#include <string.h>
#include <stdint.h>

struct dp_storage_nvme
{
  struct nvme_handle *handle;
  char in_use;
  uint8 namespace; /* NAMESPACE opened. */
  uint8 slot;
};

#ifndef DEVPRG_MAX_NVME_PHY_PARTITIONS
#define DEVPRG_MAX_NVME_PHY_PARTITIONS   1
#endif

static struct dp_storage_nvme dp_nvme_data[DEVPRG_MAX_NVME_PHY_PARTITIONS];

#define DEVPRG_MAX_NVME_SLOTS   1
/* Slot initialized bitmap, bit set to 1 if the slot was initialized.  */

static dp_res_t translate_nvme_error_simple(int32 error)
{
  dp_res_t e;

  switch(error)
  {
  case NVME_SUCCESS:
    e = DEVPRG_SUCCESS;
    break;

  case NVME_SW_ENOMEM:
    e = DEVPRG_ERROR_NOMEM;
    break;
  case NVME_SW_ECOMQ_EMPTY:
  case NVME_SW_ECOMQ_TIMEOUT:
  case NVME_SW_ECOMQ_INVAL_SUBQ_HEAD:
  case NVME_SW_ECOMQ_RANGE_SUBQ_HEAD:
  case NVME_SW_ESUBQ_FULL:
    e = DEVPRG_ERROR_STORAGE;
    break;
  case NVME_SW_EINVAL_PARAM:
  case NVME_SW_EINVAL_NAMESPACE:
  case NVME_SW_EINVAL_LBA:
    e = DEVPRG_ERROR_INVAL_PARAM;
    break;

  case NVME_SW_EMAX_HANDLE:
  case NVME_SW_EBAD_HANDLE:

  case NVME_HW_ENVME_PWR:
  case NVME_HW_ENVME_PCIE:
  case NVME_HW_ENOT_NVME:
  case NVME_HW_ETIMEOUT_ENABLE:
  case NVME_HW_ETIMEOUT_DISABLE:
  case NVME_HW_EBAD_NS_INFO:
  default:
    e = DEVPRG_ERROR_STORAGE;
    break;
  }

  return e;
}

static dp_res_t translate_nvme_error(int32 error)
{
  dp_res_t e;

  e = translate_nvme_error_simple(error);

  if(e != DEVPRG_SUCCESS)
  {
    DP_LOGE("NVME Error %x (%d)", error, e);
  }

  return e;
}

void devprg_storage_nvme_init(void)
{
  int i;

  for(i = 0; i < DEVPRG_MAX_NVME_PHY_PARTITIONS; i++)
  {
    dp_nvme_data[i].handle = NULL;
    dp_nvme_data[i].in_use = 0;
  }

}

dp_res_t devprg_storage_nvme_deinit(void)
{
  return DEVPRG_SUCCESS;
}

void *devprg_storage_nvme_open(uint32 slot, uint32 partition, dp_res_t *error)
{
  struct dp_storage_nvme *dev;
  struct nvme_handle *handle;
  int i;
  uint32 nvme_error, nvme_ns;

  if(slot > DEVPRG_MAX_NVME_SLOTS)
  {
    DP_LOGE("Code supports a maximum of %d slots, tried to open slot %d",
            DEVPRG_MAX_NVME_SLOTS, slot);
    *error = DEVPRG_ERROR_INVAL_PARAM;
    return NULL;
  }

  /* find an empty handle. */
  dev = NULL;

  for(i = 0; i < DEVPRG_MAX_NVME_PHY_PARTITIONS; i++)
  {
    if(dp_nvme_data[i].in_use == 0)
    {
      dev = &dp_nvme_data[i];
      break;
    }
  }

  if(dev == NULL)
  {
    *error = DEVPRG_ERROR_NOMEM;
    DP_LOGE("Ran out of physical partitions to allocate %d",
            DEVPRG_MAX_NVME_PHY_PARTITIONS);
    DP_LOGI("Device type nvme slot %d, partition %d, error %d",
            slot, partition, *error);
    return NULL;
  }

  if(partition == DEVPRG_WHOLE_DEVICE_PARTITION)
    nvme_ns = 1;
  else
    nvme_ns = partition + 1;

  handle = nvme_open(slot, nvme_ns, &nvme_error);

  if(handle == NULL)
  {
    DP_LOGE("Failed to open the NVME Device slot %d partition %d error %d",
            slot, partition, nvme_error);
    *error = translate_nvme_error_simple(nvme_error);
    return NULL;
  }

  dev->handle = handle;
  dev->in_use = 1;
  dev->namespace = partition;
  dev->slot = slot;

  return dev;
}

dp_res_t devprg_storage_nvme_close(void *device)
{
  struct dp_storage_nvme *dev = (struct dp_storage_nvme *)device;
  uint32 result;

  result = nvme_close(dev->handle);

  if(result == NVME_SUCCESS)
  {
    dev->in_use = 0;
    dev->handle = NULL;
  }

  return translate_nvme_error(result);
}

dp_res_t devprg_storage_nvme_io_options(void *device,
                                       struct devprg_io_options *io_options)
{
  dp_res_t result = DEVPRG_SUCCESS;
  (void) device; /* NVME only supports default */
  if (io_options->skip_bb != BAD_BLOCK_SKIP)
  {
    DP_LOGE("NVME Does not support bad block options %d", io_options->skip_bb);
    result = DEVPRG_ERROR_INVAL_PARAM;
  }
  if (io_options->get_spare != DATA_ONLY)
  {
    DP_LOGE("NVME Does not support get spare options %d", io_options->get_spare);
    result = DEVPRG_ERROR_INVAL_PARAM;
  }
  if (io_options->ecc_state != ECC_ENABLED)
  {
    DP_LOGE("NVME Does not support ECC options %d", io_options->ecc_state);
    result = DEVPRG_ERROR_INVAL_PARAM;
  }

  return result;
}

dp_res_t devprg_storage_nvme_read(void *device, void *buffer,
                            uint64 start_sector, uint64 num_sectors)
{
  struct dp_storage_nvme *dev = (struct dp_storage_nvme *)device;
  uint32 result;

  result = nvme_read(dev->handle, buffer, start_sector, num_sectors);

  return translate_nvme_error_simple(result);

}

dp_res_t devprg_storage_nvme_write(void *device, void *buffer,
                                  uint64 start_sector, uint64 num_sectors)
{
  struct dp_storage_nvme *dev = (struct dp_storage_nvme *)device;
  uint32 result;
  dp_res_t error;

  result = nvme_write(dev->handle, buffer, start_sector, num_sectors);

  if(result != NVME_SUCCESS)
  {
    error = translate_nvme_error(result);
    return error;
  }
  result = nvme_flush(dev->handle);
  if(result != NVME_SUCCESS)
  {
    error = translate_nvme_error(result);
    return error;
  }

  return DEVPRG_SUCCESS;
}

dp_res_t devprg_storage_nvme_erase(void *device,
                                   uint64 start_sector, uint64 num_sectors)
{
  struct nvme_namespace_sizes ns_sizes;
  uint32 result;
  dp_res_t status = DEVPRG_ERROR_NOMEM;
  struct dp_storage_nvme *dev = (struct dp_storage_nvme *)device;

  result = nvme_get_namespace_sizes(dev->handle, &ns_sizes);
  if(result != NVME_SUCCESS)
  {
    status = translate_nvme_error(result);
    return status;
  }

  result = nvme_write_zeroes(dev->handle, start_sector, num_sectors);
  if (result != NVME_SUCCESS)
    status = translate_nvme_error(result);
  else
    status = DEVPRG_SUCCESS;

  return status;
}

uint32 nvme_format (struct nvme_handle *handle, uint32 lba_size);

dp_res_t devprg_storage_nvme_format(void *device)
{
  uint32 result;
  dp_res_t status = DEVPRG_ERROR_NOMEM;
  struct dp_storage_nvme *dev = (struct dp_storage_nvme *)device;

  result = nvme_format(dev->handle, 512);
  if(result != NVME_SUCCESS)
    status = translate_nvme_error(result);
  else
    status = DEVPRG_SUCCESS;

  return status;
}

dp_res_t devprg_storage_nvme_set_bootable(void *device, int enable)
{
  (void) device;
  (void) enable;
  return DEVPRG_ERROR_NOT_SUPPORTED;
}

dp_res_t devprg_storage_nvme_get_info(void *device,
                                struct devprg_storage_info *dev_info)
{
  struct dp_storage_nvme *dev = (struct dp_storage_nvme *)device;
  uint32 result;
  dp_res_t error;
  struct nvme_device_info nvme_info;
  struct nvme_namespace_sizes ns_sizes;

  result = nvme_get_device_info(dev->slot, &nvme_info);
  if(result != NVME_SUCCESS)
  {
    error = translate_nvme_error(result);
    return error;
  }
  result = nvme_get_namespace_sizes(dev->handle, &ns_sizes);
  if(result != NVME_SUCCESS)
  {
    error = translate_nvme_error(result);
    return error;
  }

  dev_info->page_size      = ns_sizes.block_size;
  dev_info->block_size     = ns_sizes.block_size;
  dev_info->total_blocks   = ns_sizes.size_blocks;
  dev_info->num_physical   = nvme_info.num_namespace;
  dev_info->manufacturer_id = nvme_info.vendor_id || (nvme_info.subsys_vid << 16);
  dev_info->serial_num     = 0xFFFFFFFF; /* TODO Fix the serial number for all devices. */
  strlcpy(dev_info->fw_version, (char *)nvme_info.fw_rev,
          sizeof(nvme_info.fw_rev));
  strlcpy(dev_info->memory_type, "NVMe", sizeof(dev_info->memory_type));
  strlcpy(dev_info->product_name, (char *)nvme_info.model_num,
          sizeof(dev_info->product_name));

  return DEVPRG_SUCCESS;
}

dp_res_t devprg_storage_nvme_configure(uint32 slot,
                                      struct devprg_storage_cfg_data *configure)
{
  (void) slot;
  (void) configure;
  return DEVPRG_ERROR_NOT_SUPPORTED;
}

dp_res_t devprg_storage_nvme_fw_update(uint32 slot, void *buffer,
                                 uint32 size_in_bytes)
{
  (void) slot;
  (void) buffer;
  (void) size_in_bytes;
  return DEVPRG_ERROR_NOT_SUPPORTED;
}

dp_res_t devprg_storage_nvme_info_raw_size(void *device, uint32 *size)
{
  (void) device;
  *size = 0;
  return DEVPRG_SUCCESS;
}

dp_res_t devprg_storage_nvme_info_raw(void *device, void *buffer,
                                uint32 *size)
{
  (void) device;
  (void) buffer;
  (void) size;
  return DEVPRG_ERROR_NOT_SUPPORTED;
}

dp_res_t devprg_storage_nvme_info_raw_print(void *device, void *buffer,
                                      uint32 *size)
{
  (void) device;
  (void) buffer;
  (void) size;
  return DEVPRG_ERROR_NOT_SUPPORTED;
}


#else /* If NVME Support is disabled (FEATURE_DEVPRG_NVME not defined)*/

#include "devprg_storage_stubs.h"
void devprg_storage_nvme_init(void)
{
  devprg_storage_stub_init();
}

dp_res_t devprg_storage_nvme_deinit(void)
{
  return devprg_storage_stub_deinit();
}

void *devprg_storage_nvme_open(uint32 slot, uint32 partition,
                              dp_res_t *error)
{
  return devprg_storage_stub_open(slot, partition, error);
}

dp_res_t devprg_storage_nvme_close(void *device)
{
  return devprg_storage_stub_close(device);
}

dp_res_t devprg_storage_nvme_io_options(void *device,
                                       struct devprg_io_options *io_options)
{
  return devprg_storage_stub_io_options(device, io_options);
}

dp_res_t devprg_storage_nvme_read(void *device, void *buffer,
                            uint64 start_sector, uint64 num_sectors)
{
  return devprg_storage_stub_read(device, buffer, start_sector, num_sectors);
}

dp_res_t devprg_storage_nvme_write(void *device, void *buffer,
                                  uint64 start_sector, uint64 num_sectors)
{
  return devprg_storage_stub_write(device, buffer, start_sector, num_sectors);
}

dp_res_t devprg_storage_nvme_erase(void *device, uint64 start_sector,
                                  uint64 num_sectors)
{
  return devprg_storage_stub_erase(device, start_sector, num_sectors);
}

dp_res_t devprg_storage_nvme_format(void *device)
{
  return devprg_storage_stub_format(device);
}

dp_res_t devprg_storage_nvme_set_bootable(void *device, int enable)
{
  return devprg_storage_stub_set_bootable(device, enable);
}

dp_res_t devprg_storage_nvme_get_info(void *device,
                                     struct devprg_storage_info *dev_info)
{
  return devprg_storage_stub_get_info(device, dev_info);
}

dp_res_t devprg_storage_nvme_configure(uint32 slot,
                                      struct devprg_storage_cfg_data *config)
{
  return devprg_storage_stub_configure(slot, config);
}

dp_res_t devprg_storage_nvme_fw_update(uint32 slot, void *buffer,
                                      uint32 size_in_bytes)
{
  return devprg_storage_stub_fw_update(slot, buffer, size_in_bytes);
}

dp_res_t devprg_storage_nvme_info_raw_size(void *device, uint32 *size)
{
  return devprg_storage_stub_info_raw_size(device, size);
}

dp_res_t devprg_storage_nvme_info_raw(void *device, void *buffer,
                                     uint32 *size)
{
  return devprg_storage_stub_info_raw(device, buffer, size);
}

dp_res_t devprg_storage_nvme_info_raw_print(void *device, void *buffer,
                                           uint32 *size)
{
  return devprg_storage_stub_info_raw_print(device, buffer, size);
}

#endif /*  FEATURE_DEVPRG_NVME */
