/**************************************************************************
 * FILE: devprg_storage_nvme.h
 *
 * NVMe Device interface.
 *
 * Copyright (c) 2015-2017 Qualcomm Technologies, Inc.
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

#ifndef __DEVPRG_STORAGE_NVME_H__
#define __DEVPRG_STORAGE_NVME_H__

#include "devprg_storage.h"

/* For documentation see devprg_storage.h */
void devprg_storage_nvme_init(void);
dp_res_t devprg_storage_nvme_deinit(void);
void *devprg_storage_nvme_open(uint32 slot, uint32 partition, dp_res_t *error);
dp_res_t devprg_storage_nvme_close(void *device);
dp_res_t devprg_storage_nvme_io_options(void *device,
                                       struct devprg_io_options *io_options);
dp_res_t devprg_storage_nvme_read(void *device, void *buffer,
                            uint64 start_sector, uint64 num_sectors);
dp_res_t devprg_storage_nvme_write(void *device, void *buffer,
                                  uint64 start_sector, uint64 num_sectors);
dp_res_t devprg_storage_nvme_erase(void *device,
                             uint64 start_sector, uint64 num_sectors);
dp_res_t devprg_storage_nvme_format(void *device);
dp_res_t devprg_storage_nvme_set_bootable(void *device, int enable);
dp_res_t devprg_storage_nvme_get_info(void *device,
                                struct devprg_storage_info *dev_info);
dp_res_t devprg_storage_nvme_configure(uint32 slot,
                                 struct devprg_storage_cfg_data *config);
dp_res_t devprg_storage_nvme_fw_update(uint32 slot, void *buffer,
                                 uint32 size_in_bytes);
dp_res_t devprg_storage_nvme_info_raw_size(void *device, uint32 *size);
dp_res_t devprg_storage_nvme_info_raw(void *device, void *buffer,
                                uint32 *size);
dp_res_t devprg_storage_nvme_info_raw_print(void *device, void *buffer,
                                      uint32 *size);

#endif /* __DEVPRG_STORAGE_NVME_H__ */
