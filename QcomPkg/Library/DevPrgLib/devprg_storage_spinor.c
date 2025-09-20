/**************************************************************************
 * FILE: devprg_storage_spinor.c
 *
 * SPI-NOR Device implementation.
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
----------   ---   -----------------------------------------------------------
2018-07-31   wek   Add access denied error code returned by the driver.
2017-08-02   md    Replaced old spinor driver api calls with new version.
2017-04-20   wek   Include the start and end sectors in set io options.
2017-02-02   wek   Add io options to change how data is read/write.
2017-01-06   wek   Support the two types of SPI-NOR Flash APIs.
2016-03-09   wek   Reduce the amount of logging on erase.
2016-02-10   wek   Add the option to format whole dev or erase a portion.
2016-01-07   wek   Get FW version from get info.
2015-11-19   wek   Create.

==============================================================================*/

#include "devprg_storage.h"
#include "devprg_storage_spinor.h"
#include "devprg_log.h"

#ifdef FEATURE_DEVPRG_SPINOR
#include "spinor_api.h"
#include "spinor_init_config.h"
#include <string.h>
#include "stringl/stringl.h"

struct dp_storage_spinor
{
  spinor_handle_t handle;
  char in_use;
};

/* Simulated sector size for SPI-NOR. The sector size in SPI-NOR is
 * really 256bytes, the erase block size is 4K. To simplify the logic
 * we make the sector size as 4k, that way we don't have to re-compute
 * things on the PC based on a different sector size depending on the
 * device. */
#define DEVPRG_SPINOR_SIM_SECTOR_SIZE   4096

static struct dp_storage_spinor dp_spinor_data;

static dp_res_t translate_flash_error_simple(SPINOR_STATUS error)
{
  dp_res_t e;

  switch(error)
  {
  case SPINOR_DEVICE_DONE:
    e = DEVPRG_SUCCESS;
    break;
    break;
  case SPINOR_DEVICE_INVALID_PARAMETER:
    e = DEVPRG_ERROR_INVAL_PARAM;
    break;
  case SPINOR_ACCESS_DENIED:
    e = DEVPRG_ERROR_ACCESS_DENIED;
  case SPINOR_DEVICE_FAIL:
  default:
    e = DEVPRG_ERROR_STORAGE;
    break;
  }

  if(e != DEVPRG_SUCCESS)
  {
    DP_LOGE("SPINOR Error %d (%d)", error, e);
  }

  return e;
}

static dp_res_t translate_flash_error(SPINOR_STATUS error)
{
  dp_res_t e;

  e = translate_flash_error_simple(error);

  if(e != DEVPRG_SUCCESS)
  {
    DP_LOGE("SPINOR Error %d (%d)", error, e);
  }

  return e;
}

/* spinor_init() and spinor_deinit() are called in qspinor driver as part of spinor_open() and spinor_close() funtions. So commenting these 2 functions*/

void devprg_storage_spinor_init(void)
{
  dp_spinor_data.handle = NULL;
  dp_spinor_data.in_use = 0;
//  spinor_init();
}

dp_res_t devprg_storage_spinor_deinit(void)
{
  SPINOR_STATUS result = SPINOR_DEVICE_DONE;
//  result = spinor_deinit();
  return translate_flash_error(result);
}

void *devprg_storage_spinor_open(uint32 slot, uint32 partition, dp_res_t *error)
{
  struct dp_storage_spinor *dev = &dp_spinor_data;
  spinor_handle_t handle =NULL;
  SPINOR_STATUS result;
  struct spinor_info info;
  uint32 erase_block_size;

  if(slot != 0)
  {
    DP_LOGE("Flash API does not support more than one device %d", slot);
    *error = DEVPRG_ERROR_INVAL_PARAM;
    return NULL;
  }

  if(partition != 0)
  {
    DP_LOGE("Flash API does not support multiple physical partitions %d",
            partition);
    *error = DEVPRG_ERROR_INVAL_PARAM;
    return NULL;
  }

  /* Device has already been open, return it. */
  if(dev->in_use == 1)
  {
    DP_LOGD("SPINOR was previously open, returning existing handle %d", 0);
    return dev;
  }

  result = spinor_open(&handle);

  if(result != SPINOR_DEVICE_DONE || handle == NULL)
  {
    *error = translate_flash_error(result);
    DP_LOGE("Failed to open the SPI NOR Device %d (%d)", result, *error);
    return NULL;
  }

	memset(&info, 0, sizeof(info));
  result = spinor_get_info(handle, &info);

  if(result != SPINOR_DEVICE_DONE)
  {
    spinor_close(handle);
    *error = translate_flash_error(result);
    DP_LOGE("Failed to get info the SPI NOR Device %d (%d)", result, *error);
    return NULL;
  }

  erase_block_size = info.page_size_bytes * info.pages_per_block;

  if(erase_block_size != DEVPRG_SPINOR_SIM_SECTOR_SIZE)
  {
    spinor_close(handle);
    DP_LOGE("Can't support storage part with erase block size of %d != %d",
            erase_block_size, DEVPRG_SPINOR_SIM_SECTOR_SIZE);
    DP_LOGI("page_size=%d, pages_per_block=%d, block_count=%d, maker_id=%d, device_id=%d",
            info.page_size_bytes, info.pages_per_block, info.block_count,
            info.maker_id, info.device_id);
    return NULL;
  }

  dev->handle = handle;
  dev->in_use = 1;

  return dev;
}

dp_res_t devprg_storage_spinor_close(void *device)
{
  struct dp_storage_spinor *dev = (struct dp_storage_spinor *)device;
  SPINOR_STATUS result;

  result = spinor_close(dev->handle);

  if(result == SPINOR_DEVICE_DONE)
  {
    dev->in_use = 0;
    dev->handle = NULL;
  }

  return translate_flash_error(result);
}

dp_res_t devprg_storage_spinor_io_options(void *device,
                                          struct devprg_io_options *io_options)
{
  int result = DEVPRG_SUCCESS;
  (void) device; /* SPINOR only supports default */
  if (io_options->skip_bb != BAD_BLOCK_SKIP)
  {
    DP_LOGE("SPINOR Does not support bad block options %d", io_options->skip_bb);
    result = DEVPRG_ERROR_INVAL_PARAM;
  }
  if (io_options->get_spare != DATA_ONLY)
  {
    DP_LOGE("SPINOR Does not support get spare options %d", io_options->get_spare);
    result = DEVPRG_ERROR_INVAL_PARAM;
  }
  if (io_options->ecc_state != ECC_ENABLED)
  {
    DP_LOGE("SPINOR Does not support ECC options %d", io_options->ecc_state);
    result = DEVPRG_ERROR_INVAL_PARAM;
  }

  return result;
}


dp_res_t devprg_storage_spinor_read(void *device, void *buffer,
                                    uint64 start_sector, uint64 num_sectors)
{
  struct dp_storage_spinor *dev = (struct dp_storage_spinor *)device;
  SPINOR_STATUS result;
  uint32 offset, count;

  offset = start_sector * DEVPRG_SPINOR_SIM_SECTOR_SIZE;
  count = num_sectors * DEVPRG_SPINOR_SIM_SECTOR_SIZE;

  result = spinor_read(dev->handle, offset, count, buffer);

  return translate_flash_error_simple(result);

}

/* Check if the data inside the buffer is erased, AKA filled with 0xFF.
 *
 * return 1 When filled with 0xFF, 0 otherwise. */
static int check_erase(void *buffer_in, uint32 size)
{
  uint32 i;
  uint8 *buf = (uint8 *)buffer_in;

  for(i = 0; i < size; i++)
  {
    if(buf[i] != 0xFF)
      return 0;
  }
  return 1;
}

#define INVALID_BLOCK_NUMBER  0xFFFFFFFF

/* Buffer used to read data and check all bytes are 0xFF. */
uint8 erase_check_buffer[DEVPRG_SPINOR_SIM_SECTOR_SIZE];
/* Check if a chunk is erased, if it isn't, erases it. */
static SPINOR_STATUS spinor_check_erase(struct dp_storage_spinor *dev,
                                       uint32 start, uint32 count)
{
  SPINOR_STATUS result = SPINOR_DEVICE_DONE;
  uint32 offset, read_chunk;
  uint32 end_offset;

  if (count == 0)
    return SPINOR_DEVICE_DONE;

  offset = start * DEVPRG_SPINOR_SIM_SECTOR_SIZE;
  end_offset = offset + count * DEVPRG_SPINOR_SIM_SECTOR_SIZE;

  read_chunk = sizeof(erase_check_buffer);

  while(offset < end_offset)
  {
    result = spinor_read(dev->handle, offset, read_chunk, erase_check_buffer);
    if(result != SPINOR_DEVICE_DONE)
    {
      DP_LOGD("SPINOR Can't read offset %d chunk %d", offset, read_chunk);
      break;
    }

    /* Check if the buffer is filled with 0xFF, if it is not, needs erase. */
    if(check_erase(erase_check_buffer, read_chunk) == 0)
    {
      /* Erase the whole thing, it may be faster if chunk is large.
       * and spinor driver supports erasing large chunks. */
      result = spinor_erase(dev->handle, start, count);
      if(result == SPINOR_ACCESS_DENIED)
      {
        uint32 block_no, block_offset, first_failed_block = INVALID_BLOCK_NUMBER;
        DP_LOGD("SPINOR access denied to erase offset %d chunk %d", offset, read_chunk);
        /* In poipu there is regions that we can't program/erase. Try erasing each block instead. */
        for (block_offset = 0; block_offset < count; block_offset++)
        {
          block_no = start + block_offset;
          result = spinor_erase(dev->handle, block_no, 1);
          if(result != SPINOR_DEVICE_DONE)
          {
            if(first_failed_block > block_no)
              first_failed_block = block_no;
          }
          else if(first_failed_block != INVALID_BLOCK_NUMBER)
          {
            uint32 erase_chunk_size = block_no - first_failed_block + 1;
            DP_LOGI("SPINOR access denied to erase 4k offset %d 4k chunk %d", first_failed_block, erase_chunk_size);
            first_failed_block = INVALID_BLOCK_NUMBER;
          }
        }
        if(first_failed_block != INVALID_BLOCK_NUMBER)
        {
          uint32 erase_chunk_size = block_no - first_failed_block + 1;
          DP_LOGI("SPINOR access denied to erase 4k offset %d 4k chunk %d", first_failed_block, erase_chunk_size);
          first_failed_block = INVALID_BLOCK_NUMBER;
        }
        result = SPINOR_DEVICE_DONE;
      }
      else if(result != SPINOR_DEVICE_DONE)
      {
        DP_LOGD("SPINOR Can't Erase offset %d chunk %d", offset, read_chunk);
      }
      break;

    }
    offset += read_chunk;
  }

  return result;
}
/***This value was chosen to be larger than the largest bulk erase size for an SPI-NOR part, but small
enough to provide constant feedback to the user on the progress of the erase ***/

#define DEVPRG_ERASE_BLOCK_COUNT   32
/* Checks if the sectors are erased, if they are not, erase them. */
static dp_res_t devprg_storage_spinor_erase_log(void *device, uint64 start_sector,
                                                uint64 num_sectors, int log)
{
  struct dp_storage_spinor *dev = (struct dp_storage_spinor *)device;
  SPINOR_STATUS result = SPINOR_DEVICE_DONE;
  dp_res_t error;
  uint32 blk, blk_count, erase_size, total_count;

  blk = start_sector;
  total_count = blk_count = num_sectors;
  erase_size = DEVPRG_ERASE_BLOCK_COUNT;
  /* If the start block is not aligned, then the first erase will
   * align the start sector. */
  if(blk % DEVPRG_ERASE_BLOCK_COUNT != 0)
    erase_size = DEVPRG_ERASE_BLOCK_COUNT - (blk % DEVPRG_ERASE_BLOCK_COUNT);

  if(log)
    DP_LOGD("SPINOR Erasing sector %d Num blocks %d", blk, blk_count);

  while (blk_count >= erase_size && result == SPINOR_DEVICE_DONE)
  {
    result = spinor_check_erase(dev, blk, erase_size);
    blk += erase_size;
    blk_count -= erase_size;
    if(log)
      DP_LOGI("SPINOR Erase progress %d%% (%d of %d)",
              100 - (blk_count * 100) / total_count,
              total_count - blk_count, total_count);
    erase_size = DEVPRG_ERASE_BLOCK_COUNT;
  }

  if(blk_count < erase_size && result == SPINOR_DEVICE_DONE)
  {
    result = spinor_check_erase(dev, blk, blk_count);
    blk_count -= blk_count;
    if(log)
      DP_LOGI("SPINOR Erase progress %d%% (%d of %d).",
              100 - (blk_count * 100) / total_count,
              total_count - blk_count, total_count);
  }


  if(result != SPINOR_DEVICE_DONE)
  {
    error = translate_flash_error(result);
    return error;
  }

  return DEVPRG_SUCCESS;
}


dp_res_t devprg_storage_spinor_write(void *device, void *buffer,
                                     uint64 start_sector, uint64 num_sectors)
{
  struct dp_storage_spinor *dev = (struct dp_storage_spinor *)device;
  SPINOR_STATUS result;
  uint32 offset, count;
  dp_res_t error;

  error = devprg_storage_spinor_erase_log(device, start_sector,
                                          num_sectors, 0);
  if (error != DEVPRG_SUCCESS)
    return error;

  offset = start_sector * DEVPRG_SPINOR_SIM_SECTOR_SIZE;
  count = num_sectors * DEVPRG_SPINOR_SIM_SECTOR_SIZE;
  result = spinor_write(dev->handle, offset, count, buffer);

  if(result != SPINOR_DEVICE_DONE)
  {
    error = translate_flash_error(result);
    return error;
  }

  return DEVPRG_SUCCESS;
}

dp_res_t devprg_storage_spinor_erase(void *device, uint64 start_sector,
                                     uint64 num_sectors)
{
  return devprg_storage_spinor_erase_log(device, start_sector, num_sectors, 1);
}

dp_res_t devprg_storage_spinor_format(void *device)
{
  struct dp_storage_spinor *dev = (struct dp_storage_spinor *)device;
  SPINOR_STATUS result;
  struct spinor_info info;
  int error;

	memset(&info, 0, sizeof(info));
  result = spinor_get_info(dev->handle, &info);

  if(result != SPINOR_DEVICE_DONE)
  {
    error = translate_flash_error(result);
    DP_LOGE("Get Info Failed in format %s", " ");
    return error;
  }

  DP_LOGD("SPINOR Erasing whole disk of %d blocks", info.block_count);
  error = devprg_storage_spinor_erase(device, 0, info.block_count);

  return error;
}


dp_res_t devprg_storage_spinor_set_bootable(void *device, int enable)
{
  (void) device;
  (void) enable;
  return DEVPRG_ERROR_NOT_SUPPORTED;
}

dp_res_t devprg_storage_spinor_get_info(void *device,
                                        struct devprg_storage_info *dev_info)
{
  struct dp_storage_spinor *dev = (struct dp_storage_spinor *)device;
  SPINOR_STATUS result;
  struct spinor_info info;
  dp_res_t error;

  memset(&info, 0, sizeof(info));
  result = spinor_get_info(dev->handle, &info);

  if(result != SPINOR_DEVICE_DONE)
  {
    error = translate_flash_error(result);
    return error;
  }

  dev_info->page_size       = info.page_size_bytes;
  dev_info->block_size      = info.pages_per_block * info.page_size_bytes;
  dev_info->total_blocks    = info.block_count;
  dev_info->manufacturer_id = info.maker_id;
  dev_info->serial_num      = info.device_id;
  dev_info->num_physical    = 0;
  dev_info->fw_version[0]   = 0;
  strlcpy(dev_info->memory_type, "NOR", sizeof(dev_info->memory_type));
  dev_info->product_name[0] = 0;

  return DEVPRG_SUCCESS;
}

dp_res_t devprg_storage_spinor_configure(uint32 slot,
                                         struct devprg_storage_cfg_data *config)
{
  (void) slot;
  (void) config;
  return DEVPRG_ERROR_NOT_SUPPORTED;
}

dp_res_t devprg_storage_spinor_fw_update(uint32 slot, void *buffer,
                                         uint32 size_in_bytes)
{
  (void) slot;
  (void) buffer;
  (void) size_in_bytes;
  return DEVPRG_ERROR_NOT_SUPPORTED;
}

dp_res_t devprg_storage_spinor_info_raw_size(void *device, uint32 *size)
{
  (void) device;
  *size = 0;
  return DEVPRG_SUCCESS;
}

dp_res_t devprg_storage_spinor_info_raw(void *device, void *buffer,
                                        uint32 *size)
{
  (void) device;
  (void) buffer;
  *size = 0;
  return DEVPRG_ERROR_NOT_SUPPORTED;
}

dp_res_t devprg_storage_spinor_info_raw_print(void *device, void *buffer,
                                              uint32 *size)
{
  (void) device;
  (void) buffer;
  *size = 0;
  return DEVPRG_SUCCESS;
}
#else /* If SPI NOR Support is disabled (FEATURE_DEVPRG_SPINOR not defined)*/

#include "devprg_storage_stubs.h"
void devprg_storage_spinor_init(void)
{
  devprg_storage_stub_init();
}

dp_res_t devprg_storage_spinor_deinit(void)
{
  return devprg_storage_stub_deinit();
}

void *devprg_storage_spinor_open(uint32 slot, uint32 partition,
                                 dp_res_t *error)
{
  return devprg_storage_stub_open(slot, partition, error);
}

dp_res_t devprg_storage_spinor_io_options(void *device,
                                          struct devprg_io_options *io_options)
{
  return devprg_storage_stub_io_options(device, io_options);
}

dp_res_t devprg_storage_spinor_close(void *device)
{
  return devprg_storage_stub_close(device);
}

dp_res_t devprg_storage_spinor_read(void *device, void *buffer,
                                    uint64 start_sector, uint64 num_sectors)
{
  return devprg_storage_stub_read(device, buffer, start_sector, num_sectors);
}

dp_res_t devprg_storage_spinor_write(void *device, void *buffer,
                                     uint64 start_sector, uint64 num_sectors)
{
  return devprg_storage_stub_write(device, buffer, start_sector, num_sectors);
}

dp_res_t devprg_storage_spinor_erase(void *device,
                                     uint64 start_sector, uint64 num_sectors)
{
  return devprg_storage_stub_erase(device, start_sector, num_sectors);
}

dp_res_t devprg_storage_spinor_format(void *device)
{
  return devprg_storage_stub_format(device);
}

dp_res_t devprg_storage_spinor_set_bootable(void *device, int enable)
{
  return devprg_storage_stub_set_bootable(device, enable);
}

dp_res_t devprg_storage_spinor_get_info(void *device,
                                        struct devprg_storage_info *dev_info)
{
  return devprg_storage_stub_get_info(device, dev_info);
}

dp_res_t devprg_storage_spinor_configure(uint32 slot,
                                         struct devprg_storage_cfg_data *config)
{
  return devprg_storage_stub_configure(slot, config);
}

dp_res_t devprg_storage_spinor_fw_update(uint32 slot, void *buffer,
                                         uint32 size_in_bytes)
{
  return devprg_storage_stub_fw_update(slot, buffer, size_in_bytes);
}

dp_res_t devprg_storage_spinor_info_raw_size(void *device, uint32 *size)
{
  return devprg_storage_stub_info_raw_size(device, size);
}

dp_res_t devprg_storage_spinor_info_raw(void *device, void *buffer,
                                        uint32 *size)
{
  return devprg_storage_stub_info_raw(device, buffer, size);
}
dp_res_t devprg_storage_spinor_info_raw_print(void *device, void *buffer,
                                              uint32 *size)
{
  return devprg_storage_stub_info_raw_print(device, buffer, size);
}


#endif /*  FEATURE_DEVPRG_SPINOR */
