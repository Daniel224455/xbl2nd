/**************************************************************************
 * FILE: devprg_storage_ufs.c
 *
 * UFS device interface.
 *
 * Copyright (c) 2015-2018, 2020 Qualcomm Technologies, Inc.
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
2020-10-07   gk    Add Write Booster parameters
2018-12-14   jt    Move alloc units calculation to UFS driver
2018-09-21   wek   Remove unnecessary calculation for lun to grow.
2017-06-09   jt    Update copy cfg data to give alloc units in 4KB chunks
2017-04-20   wek   Include the start and end sectors in set io options.
2017-02-02   wek   Add io options to change how data is read/write.
2016-02-10   wek   Add the option to format whole dev or erase a portion.
2016-01-19   rh    Fix the allocation unit size if segment is not 4MB.
2016-01-07   wek   Get FW version from get info.
2015-12-04   wek   Create. (9-m)

===========================================================================*/

#ifdef FEATURE_DEVPRG_UFS

#include "devprg_storage.h"
#include "devprg_storage_ufs.h"
#include "devprg_log.h"
#include "ufs_api.h"
#include "stringl/stringl.h"
#include <string.h>
#include <stdint.h>

struct dp_storage_ufs
{
  struct ufs_handle *handle;
  char in_use;
  uint8 lun; /* LUN opened. */
};

#ifndef DEVPRG_MAX_UFS_PHY_PARTITIONS
#define DEVPRG_MAX_UFS_PHY_PARTITIONS   10
#endif

static struct dp_storage_ufs dp_ufs_data[DEVPRG_MAX_UFS_PHY_PARTITIONS];

#define DEVPRG_MAX_UFS_SLOTS   8
/* Slot initialized bitmap, bit set to 1 if the slot was initialized.  */
static uint8 slot_init_bitmap = 0;

static dp_res_t translate_ufs_error_simple(int32 error)
{
  dp_res_t e;

  switch(error)
  {
  case UFS_EOK:
    e = DEVPRG_SUCCESS;
    break;
  case UFS_EINVAL:
    e = DEVPRG_ERROR_INVAL_PARAM;
    break;
  case UFS_ENOENT:
  case UFS_EIO:
  case UFS_ENOMEM:
  case UFS_EBUSY:
  case UFS_ENODEV:
  case UFS_EPROTO:
  case UFS_ETIME:
  default:
    e = DEVPRG_ERROR_STORAGE;
    break;
  }

  return e;
}

static dp_res_t translate_ufs_error(int32 error)
{
  dp_res_t e;

  e = translate_ufs_error_simple(error);

  if(e != DEVPRG_SUCCESS)
  {
    DP_LOGE("UFS Error %d (%d)", error, e);
  }

  return e;
}

void devprg_storage_ufs_init(void)
{
  int i;

  for(i = 0; i < DEVPRG_MAX_UFS_PHY_PARTITIONS; i++)
  {
    dp_ufs_data[i].handle = NULL;
    dp_ufs_data[i].in_use = 0;
  }

  slot_init_bitmap = 0;
}

dp_res_t devprg_storage_ufs_deinit(void)
{
  return DEVPRG_SUCCESS;
}

void *devprg_storage_ufs_open(uint32 slot, uint32 partition, dp_res_t *error)
{
  struct dp_storage_ufs *dev;
  struct ufs_handle *handle;
  int i, slot_init;

  if(slot > DEVPRG_MAX_UFS_SLOTS)
  {
    DP_LOGE("Code supports a maximum of %d slots, tried to open slot %d",
            DEVPRG_MAX_UFS_SLOTS, slot);
    *error = DEVPRG_ERROR_INVAL_PARAM;
    return NULL;
  }

  /* find an empty handle. */
  dev = NULL;

  for(i = 0; i < DEVPRG_MAX_UFS_PHY_PARTITIONS; i++)
  {
    if(dp_ufs_data[i].in_use == 0)
    {
      dev = &dp_ufs_data[i];
      break;
    }
  }

  if(dev == NULL)
  {
    *error = DEVPRG_ERROR_NOMEM;
    DP_LOGE("Ran out of physical partitions to allocate %d",
            DEVPRG_MAX_UFS_PHY_PARTITIONS);
    DP_LOGI("Device type ufs slot %d, partition %d, error %d",
            slot, partition, *error);
    return NULL;
  }

  /* Check if the slot has been initialized. */
  slot_init = (slot_init_bitmap >> slot) & 0x01;

  if(slot_init == 0)
  {
    /* Initialize the slot. */
    handle = ufs_open(slot, UFS_WLUN_DEVICE);

    if(handle == NULL)
    {
      DP_LOGE("Failed to initialize (open whole lun) UFS Device slot %d partition %d",
              slot, partition);
      return NULL;
    }

    ufs_close(handle);
    slot_init_bitmap |= (1 << slot);
  }

  if(partition == DEVPRG_WHOLE_DEVICE_PARTITION)
    partition = UFS_WLUN_DEVICE;

  handle = ufs_open(slot, partition);

  if(handle == NULL)
  {
    DP_LOGE("Failed to open the UFS Device slot %d partition %d",
            slot, partition);
    return NULL;
  }

  dev->handle = handle;
  dev->in_use = 1;
  dev->lun = partition;

  return dev;
}

dp_res_t devprg_storage_ufs_close(void *device)
{
  struct dp_storage_ufs *dev = (struct dp_storage_ufs *)device;
  int32 result;

  result = ufs_close(dev->handle);

  if(result == UFS_EOK)
  {
    dev->in_use = 0;
    dev->handle = NULL;
  }

  return translate_ufs_error(result);
}

dp_res_t devprg_storage_ufs_io_options(void *device,
                                       struct devprg_io_options *io_options)
{
  int result = DEVPRG_SUCCESS;
  (void) device; /* UFS only supports default */
  if (io_options->skip_bb != BAD_BLOCK_SKIP)
  {
    DP_LOGE("UFS Does not support bad block options %d", io_options->skip_bb);
    result = DEVPRG_ERROR_INVAL_PARAM;
  }
  if (io_options->get_spare != DATA_ONLY)
  {
    DP_LOGE("UFS Does not support get spare options %d", io_options->get_spare);
    result = DEVPRG_ERROR_INVAL_PARAM;
  }
  if (io_options->ecc_state != ECC_ENABLED)
  {
    DP_LOGE("UFS Does not support ECC options %d", io_options->ecc_state);
    result = DEVPRG_ERROR_INVAL_PARAM;
  }

  return result;
}

dp_res_t devprg_storage_ufs_read(void *device, void *buffer,
                            uint64 start_sector, uint64 num_sectors)
{
  struct dp_storage_ufs *dev = (struct dp_storage_ufs *)device;
  int32 result = 0;

  result = ufs_read(dev->handle, buffer, start_sector, num_sectors);

  return translate_ufs_error_simple(result);

}

dp_res_t devprg_storage_ufs_write(void *device, void *buffer,
                                  uint64 start_sector, uint64 num_sectors)
{
  struct dp_storage_ufs *dev = (struct dp_storage_ufs *)device;
  int32 result;
  dp_res_t error;

  result = ufs_write(dev->handle, buffer, start_sector, num_sectors);

  if(result != UFS_EOK)
  {
    error = translate_ufs_error(result);
    return error;
  }

  return DEVPRG_SUCCESS;
}

dp_res_t devprg_storage_ufs_erase(void *device,
                             uint64 start_sector, uint64 num_sectors)
{
  struct dp_storage_ufs *dev = (struct dp_storage_ufs *)device;
  int32 result;
  dp_res_t error;

  result = ufs_unmap(dev->handle, start_sector, num_sectors);

  if(result != UFS_EOK)
  {
    error = translate_ufs_error(result);
    return error;
  }

  return DEVPRG_SUCCESS;
}


dp_res_t devprg_storage_ufs_format(void *device)
{
  struct dp_storage_ufs *dev = (struct dp_storage_ufs *)device;
  int32 result;
  dp_res_t error;

  DP_LOGD("UFS Format whole LUN %s", " ");
  result = ufs_format(dev->handle);

  if(result != UFS_EOK)
  {
    error = translate_ufs_error(result);
    return error;
  }

  return DEVPRG_SUCCESS;
}

dp_res_t devprg_storage_ufs_set_bootable(void *device, int enable)
{
  struct dp_storage_ufs *dev = (struct dp_storage_ufs *)device;
  int32 result;
  dp_res_t error;
  uint32 lun = dev->lun;

  if(enable == 0)
  {
    lun = 0; /* Set the LUN to zero to disable. */
  }
  else
  {
    /* Get the currently open LUN. */
    struct ufs_info_type ufs_info;
    result = ufs_get_device_info(dev->handle, &ufs_info);

    if(result != UFS_EOK)
    {
      error = translate_ufs_error(result);
      DP_LOGE("Get info Failed for set bootable %s", " ");
      return error;
    }

    lun = ufs_info.bLUN;
  }

  result = ufs_set_bootable(dev->handle, lun);

  if(result != UFS_EOK)
  {
    error = translate_ufs_error(result);
    return error;
  }

  return DEVPRG_SUCCESS;
}

dp_res_t devprg_storage_ufs_get_info(void *device,
                                struct devprg_storage_info *dev_info)
{
  struct dp_storage_ufs *dev = (struct dp_storage_ufs *)device;
  int32 result;
  dp_res_t error;
  struct ufs_info_type info;
  uint64 serial;

  result = ufs_get_device_info(dev->handle, &info);

  if(result != UFS_EOK)
  {
    error = translate_ufs_error(result);
    return error;
  }

  /* Unalined access is not allowed by some cores and the compiler
   * does not correct for this on this case. Read the serial number
   * byte by byte. */
  serial = 0;
  serial += info.inquiry_str[info.iSerialNumber] << 24;
  serial += info.inquiry_str[info.iSerialNumber+1] << 16;
  serial += info.inquiry_str[info.iSerialNumber+2] << 8;
  serial += info.inquiry_str[info.iSerialNumber+3];

  dev_info->page_size       = info.bMinAddrBlockSize * 512;
  dev_info->block_size      = info.bMinAddrBlockSize * 512;
  dev_info->total_blocks    = info.dLuTotalBlocks;
  dev_info->manufacturer_id = info.wManufacturerID;
  dev_info->serial_num      = serial;
  dev_info->num_physical    = info.bNumberLu;

  /* According to the UFS Spec, Table 11.3 and 11.4 the version number is 4
     bytes with any ASCII value at the end of the inquiry string. */
  strlcpy(dev_info->fw_version, (char *)&info.inquiry_str[25],
          sizeof(dev_info->fw_version));
  strlcpy(dev_info->memory_type, "UFS", sizeof(dev_info->memory_type));
  strlcpy(dev_info->product_name, (char *)&info.inquiry_str[8],
          sizeof(dev_info->product_name));

  return DEVPRG_SUCCESS;
}

static void devprg_ufs_copy_cfg_data(struct ufs_lun_config_descr *config,
                                     struct devprg_ufs_config_descr *configure)

{
  unsigned int i, number_of_luns;

  /* Copy the configuration items to the internal data structure, the two
   * structures are basically the same but the offset/address of each member
   * may be different due to alignment. */
  config->bNumberLU = configure->bNumberLU;
  config->bBootEnable = configure->bBootEnable;
  config->bDescrAccessEn = configure->bDescrAccessEn;
  config->bInitPowerMode = configure->bInitPowerMode;
  config->bHighPriorityLUN = configure->bHighPriorityLUN;
  config->bSecureRemovalType = configure->bSecureRemovalType;
  config->bInitActiveICCLevel = configure->bInitActiveICCLevel;
  config->bConfigDescrLock = configure->bConfigDescrLock;
  config->wPeriodicRTCUpdate = configure->wPeriodicRTCUpdate;
  config->bWriteBoosterBufferPreserveUserSpaceEn = configure->bWriteBoosterBufferPreserveUserSpaceEn; 
  config->bWriteBoosterBufferType = configure->bWriteBoosterBufferType;
  config->qSharedWriteBoosterBufferSizeInKb = configure->qSharedWriteBoosterBufferSizeInKb; 
  config->qVendorConfigCode = configure->qVendorConfigCode;
  config->lun_to_grow = configure->lun_to_grow; 

  number_of_luns = sizeof(configure->unit) / sizeof(struct devprg_ufs_unit_descr);

  for(i = 0; i < number_of_luns; i++)
  {
    struct ufs_lun_descr *dest = &config->unit[i];
    struct devprg_ufs_unit_descr *src = &configure->unit[i];

    dest->bLUEnable = src->bLUEnable;
    dest->bBootLunID = src->bBootLunID;
    dest->bLUWriteProtect = src->bLUWriteProtect;
    dest->bMemoryType = src->bMemoryType;
    dest->qSizeInKb = src->qSizeInKb; 
    dest->bDataReliability = src->bDataReliability;
    dest->bLogicalBlockSize = src->bLogicalBlockSize;
    dest->bProvisioningType = src->bProvisioningType;
    dest->wContextCapabilities = src->wContextCapabilities;
    dest->qWriteBoosterBufferSizeInKb = src->qWriteBoosterBufferSizeInKb; 
  }
}

dp_res_t devprg_storage_ufs_configure(uint32 slot,
                                      struct devprg_storage_cfg_data *configure)
{
  int32 result;
  dp_res_t error = DEVPRG_SUCCESS;
  struct ufs_handle *dev;
  struct ufs_lun_config_descr ufs_config; 

  devprg_ufs_copy_cfg_data(&ufs_config, &configure->u.ufs_config);

  dev = ufs_open(slot, UFS_WLUN_DEVICE);
  if(dev == NULL)
  {
    DP_LOGE("ufs_open(%d, WLUN_DEV) failed for configure", slot);
    return DEVPRG_ERROR_STORAGE;
  }

  result = ufs_configure_device(dev, &ufs_config);
  if(result != 0)
  {
    error = translate_ufs_error(result);
  }

  ufs_close(dev);

  return error;
}

dp_res_t devprg_storage_ufs_fw_update(uint32 slot, void *buffer,
                                 uint32 size_in_bytes)
{
  int result = UFS_ENOENT;
  struct ufs_handle *hsdev_handle = NULL;
  struct ufs_handle *hswdev_handle = NULL;
  dp_res_t error = DEVPRG_SUCCESS;

  hswdev_handle = ufs_open(slot, UFS_WLUN_DEVICE);
  hsdev_handle = ufs_open(slot, 0);

  if(hsdev_handle == NULL)
  {
    DP_LOGE("Open Failed LUN 0 for slot %d. Check card.", slot);
    return DEVPRG_ERROR_STORAGE;
  }

  result = ufs_fw_update_write (hsdev_handle, size_in_bytes, 0, buffer);
  if(result != 0)
    error = translate_ufs_error(result);
  else
    DP_LOGI("FW Update Successful. %s", " ");

  ufs_close(hswdev_handle);
  ufs_close(hsdev_handle);

  return error;
}

dp_res_t devprg_storage_ufs_info_raw_size(void *device, uint32 *size)
{
  (void) device;
  *size = sizeof(struct ufs_info_type);
  return DEVPRG_SUCCESS;
}

dp_res_t devprg_storage_ufs_info_raw(void *device, void *buffer,
                                uint32 *size)
{
  struct dp_storage_ufs *dev = (struct dp_storage_ufs *)device;
  int32 result;
  dp_res_t error;
  struct ufs_info_type *info;
  uint32 max_size;

  devprg_storage_ufs_info_raw_size(device, &max_size);

  if(*size < max_size)
  {
    DP_LOGE("UFS Get info raw size too small %d < %d", *size, max_size);
    return DEVPRG_ERROR_NOMEM;
  }

  info = (struct ufs_info_type *)buffer;
  result = ufs_get_device_info(dev->handle, info);

  if(result != UFS_EOK)
  {
    error = translate_ufs_error(result);
    return error;
  }

  *size = max_size;

  return DEVPRG_SUCCESS;
}

dp_res_t devprg_storage_ufs_info_raw_print(void *device, void *buffer,
                                      uint32 *size)
{
  dp_res_t error;

  error = devprg_storage_ufs_info_raw(device, buffer, size);
  if (error == DEVPRG_SUCCESS)
  {
    char buf[30];
    struct ufs_info_type *info;
    int i;
    info = (struct ufs_info_type*) buffer;
    DP_LOGI("UFS fInitialized: 0x%x", info->fInitialized);
    DP_LOGI("UFS Current LUN Number: = 0x%x", info->bLUN);
    DP_LOGI("UFS Total Active LU: 0x%x", info->bNumberLu);
    DP_LOGI("UFS wManufacturerID: 0x%x", info->wManufacturerID);
    DP_LOGI("UFS Boot Partition Enabled: 0x%x", info->bBootEnable);
    DP_LOGI("UFS Raw Device Capacity: = 0x%x", info->qTotalRawDeviceCapacity);
    DP_LOGI("UFS Min Block Size: 0x%x", info->bMinAddrBlockSize);
    DP_LOGI("UFS Erase Block Size: 0x%x", info->dSegmentSize);
    DP_LOGI("UFS Allocation Unit Size: 0x%x", info->bAllocationUnitSize);
    DP_LOGI("UFS RPMB ReadWrite Size: = 0x%x", info->bRPMB_ReadWriteSize);
    DP_LOGI("UFS Number of Allocation Uint for This LU: 0x%x", info->dNumAllocUnits);
    DP_LOGI("UFS Logical Block Size: 0x%x", info->bLogicalBlockSize);
    DP_LOGI("UFS Provisioning Type: 0x%x", info->bProvisioningType);
    DP_LOGI("UFS LU Write Protect: 0x%x", info->bLUWriteProtect);
    DP_LOGI("UFS Boot LUN ID: = 0x%x", info->bBootLunID);
    DP_LOGI("UFS Memory Type: 0x%x", info->bMemoryType);
    DP_LOGI("UFS LU Total Blocks: 0x%x", info->dLuTotalBlocks);
    DP_LOGI("UFS Supported Memory Types: 0x%x", info->wSupportedMemoryTypes);
    DP_LOGI("UFS dEnhanced1MaxNAllocU: 0x%x", info->dEnhanced1MaxNAllocU);
    DP_LOGI("UFS wEnhanced1CapAdjFac: 0x%x", info->wEnhanced1CapAdjFac);
    DP_LOGI("UFS dEnhanced2MaxNAllocU: = 0x%x", info->dEnhanced2MaxNAllocU);
    DP_LOGI("UFS wEnhanced2CapAdjFac: 0x%x", info->wEnhanced2CapAdjFac);
    DP_LOGI("UFS dEnhanced3MaxNAllocU: 0x%x", info->dEnhanced3MaxNAllocU);
    DP_LOGI("UFS wEnhanced3CapAdjFac: 0x%x", info->wEnhanced3CapAdjFac);
    DP_LOGI("UFS dEnhanced4MaxNAllocU: 0x%x", info->dEnhanced4MaxNAllocU);
    DP_LOGI("UFS wEnhanced4CapAdjFac: 0x%x", info->wEnhanced4CapAdjFac);
    DP_LOGI("UFS LUN Enable Bitmask: 0x%x", info->dLunEnabled);
    DP_LOGI("UFS Logical Block Count: 0x%x", info->qLogicalBlockCount);
    DP_LOGI("UFS bConfigDescrLock: 0x%x", info->bConfigDescrLock);
    DP_LOGI("UFS iManufacturerName String Index: 0x%x", info->iManufacturerName);
    DP_LOGI("UFS iProductName String Index: 0x%x", info->iProductName);
    DP_LOGI("UFS iSerialNumber String Index: 0x%x", info->iSerialNumber);
    DP_LOGI("UFS iOemID String Index: 0x%x", info->iOemID);

    /* Send Inquiry String */
    for (i = 0; i < 28; i++)
    {
      buf[i] = info->inquiry_str[i] != 0 ? info->inquiry_str[i] : ' ';
    }
    buf[28] = 0;
    buf[29] = 0;
    DP_LOGI("UFS Inquiry Command Output: %s ", buf);
  }

  return error;
}


#else /* If UFS Support is disabled (FEATURE_DEVPRG_UFS not defined)*/

#include "devprg_storage_stubs.h"
void devprg_storage_ufs_init(void)
{
  devprg_storage_stub_init();
}

dp_res_t devprg_storage_ufs_deinit(void)
{
  return devprg_storage_stub_deinit();
}

void *devprg_storage_ufs_open(uint32 slot, uint32 partition,
                              dp_res_t *error)
{
  return devprg_storage_stub_open(slot, partition, error);
}

dp_res_t devprg_storage_ufs_close(void *device)
{
  return devprg_storage_stub_close(device);
}

dp_res_t devprg_storage_ufs_io_options(void *device,
                                       struct devprg_io_options *io_options)
{
  return devprg_storage_stub_io_options(device, io_options);
}

dp_res_t devprg_storage_ufs_read(void *device, void *buffer,
                            uint64 start_sector, uint64 num_sectors)
{
  return devprg_storage_stub_read(device, buffer, start_sector, num_sectors);
}

dp_res_t devprg_storage_ufs_write(void *device, void *buffer,
                                  uint64 start_sector, uint64 num_sectors)
{
  return devprg_storage_stub_write(device, buffer, start_sector, num_sectors);
}

dp_res_t devprg_storage_ufs_erase(void *device, uint64 start_sector,
                                  uint64 num_sectors)
{
  return devprg_storage_stub_erase(device, start_sector, num_sectors);
}

dp_res_t devprg_storage_ufs_format(void *device)
{
  return devprg_storage_stub_format(device);
}

dp_res_t devprg_storage_ufs_set_bootable(void *device, int enable)
{
  return devprg_storage_stub_set_bootable(device, enable);
}

dp_res_t devprg_storage_ufs_get_info(void *device,
                                     struct devprg_storage_info *dev_info)
{
  return devprg_storage_stub_get_info(device, dev_info);
}

dp_res_t devprg_storage_ufs_configure(uint32 slot,
                                      struct devprg_storage_cfg_data *config)
{
  return devprg_storage_stub_configure(slot, config);
}

dp_res_t devprg_storage_ufs_fw_update(uint32 slot, void *buffer,
                                      uint32 size_in_bytes)
{
  return devprg_storage_stub_fw_update(slot, buffer, size_in_bytes);
}

dp_res_t devprg_storage_ufs_info_raw_size(void *device, uint32 *size)
{
  return devprg_storage_stub_info_raw_size(device, size);
}

dp_res_t devprg_storage_ufs_info_raw(void *device, void *buffer,
                                     uint32 *size)
{
  return devprg_storage_stub_info_raw(device, buffer, size);
}

dp_res_t devprg_storage_ufs_info_raw_print(void *device, void *buffer,
                                           uint32 *size)
{
  return devprg_storage_stub_info_raw_print(device, buffer, size);
}

#endif /*  FEATURE_DEVPRG_UFS */
