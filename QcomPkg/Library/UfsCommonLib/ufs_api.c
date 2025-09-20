 /*****************************************************************************
 * ufs_api.c
 *
 * UFS device driver - public API
 *
 * This file implements the top level UFS driver APIs 
 *
 * Copyright (c) 2012-2020 Qualcomm Technologies, Inc.  All Rights Reserved.
 * Qualcomm Technologies Proprietary and Confidential.
 *
 *****************************************************************************/
/*=============================================================================
                        Edit History


when         who     what, where, why
----------   ---     --------------------------------------------------- 
2020-10-07   gk      Add Write Booster provisioning support
2019-09-24   jt      Lower Vcc to 0V for 2ms before switching to 2.5V 
2019-07-02   jt      Add voltage control ability 
2019-02-13   jt      Add UFS 3.0 support  
2018-12-12   jt      Modify UFS provisioning API  
2018-11-29   jt      Secondary UFS enablement for HLOS
2018-09-24   jt      Remove 1 core_id restriction 
2018-08-16   jt      Add clock gate/ungate APIs 
2018-06-04   jt      Add clock control ability 
2018-05-11   jt      Add autoH8 API  
2017-08-30   jt      Check whether LUN is valid before test unit ready 
2017-08-23   bb      Added profiling 
2017-05-22   jt      Add background initialization APIs
2016-09-21   jt      Get write protect flags  
2015-10-07   plc     Change RUMI flag to PRE_SI flag for presi execution
2015-09-22   rh      Adding the FFU interface API
2015-03-26   rh      Fix RPMB support
2015-02-18   rh      Add pre-init reading of WLUN_BOOT
2014-04-22   rh      Adding the pre-prog config option
2013-11-05   rh      Adding the format API
2013-10-22   rh      Move the gear switch to after NOP response
2013-09-25   rh      Allow ufs_open to pen UFS_WLUN_BOOT LU
2013-09-10   rh      Get the device descriptor first prior to open LUs
2013-09-09   rh      Enable setting of the bRefClkFreq
2013-08-12   rh      LU size calculation is not correct
2012-12-18   rh      Initial creation
=============================================================================*/

#include "ufs_osal.h"
#include "ufs_api.h"
#include "ufs_core.h"
#include "ufs_memory.h"
#include "ufs_errno.h"
#include "ufs_bsp.h"
#include "StorageCommon.h"

// Allocate an array to hand out handles
#define MAX_UFS_OPEN_HANDLES        20
#define UFS_HANDLE_MAGIC_ID         0x105189ab
#define UFS_INUSE_TRUE              0xF0

static ufs_handle_t ufs_handle_array[MAX_UFS_OPEN_HANDLES] = {{0}};

// Find the next free handle and allocate
static ufs_handle_t *ufs_get_handle ()
{
   uint32_t i;

   //ufs_osal_sync_enter(&g_ufs_drv_lock);
 
   for (i = 0; i < MAX_UFS_OPEN_HANDLES; i++) {
      if (ufs_handle_array[i].is_inuse) {
         continue;
      }
      else {
         // Set the inuse flag in the handle
         ufs_handle_array[i].is_inuse = UFS_INUSE_TRUE;
         ufs_handle_array[i].id_magic = UFS_HANDLE_MAGIC_ID;
         return &ufs_handle_array[i];
      }
   }

   //ufs_osal_sync_leave(&g_ufs_drv_lock);
  
   // Running out of handles, return NULL
   return NULL;
}


// Free the inuse handle
void ufs_free_handle (ufs_handle_t *handle)
{
   if (NULL == handle) {
      return;
   }
 
   handle->is_inuse = 0;
   handle->id_magic = 0;
}

// Beginning portion of UFS initialization (only until NOP). Partial initialization 
// is allowed only for non-primary instance 
static int32 ufs_phase1_init(ufs_handle_t *hufs, uint32 lun, boolean partial_init)
{
   int32 rc = EOK;

   // 1. Call memory initialization
   rc = ufs_mem_init(hufs);
   if (rc != EOK) {
      return rc; 
   }

   // For non-primary instance, we don't need to continue if we are only doing 
   // partial init
   if (partial_init && (hufs->ufs_core_id != 0)) {
      return rc; 
   }

   //1.5 Get PHY capabilities
   rc = ufs_get_phy_capabilities(hufs);
   if (rc != EOK) {
      return rc;  
   }

   // 2. Wait until device is responding to NOP UPIU
   rc =  ufs_wait_device_awake (hufs);
   if (rc != EOK) {
      return rc; 
   }

   return rc; 
}

static int32 ufs_phase2_init(ufs_handle_t *hufs)
{
   int32 rc = EOK;

   rc = ufs_set_fDeviceInit (hufs);
   if (rc != EOK) {
      return rc;  
   }

   rc = ufs_poll_fDeviceInit (hufs);
   if (rc != EOK) {
      return rc;
   }

   return rc; 
}

// Public API
int32 ufs_background_init_start(uint32 core_id, uint32 init_ctxt)
{
   (void) init_ctxt; 

   int32 rc = EOK;
   ufs_handle_t *hufs = NULL; 

   // First get a free handle
   hufs = ufs_get_handle();
   if (hufs == NULL) {
      // Out of available handles
      return UFS_EINVAL;
   }

   // Set the LUN, core_id used
   hufs->lun = UFS_WLUN_DEVICE;
   hufs->ufs_core_id = core_id; 

   // Do the 1st part of the initialization: PHY init, link startup, NOP OUT
   rc = ufs_phase1_init(hufs, UFS_WLUN_DEVICE, TRUE); 
   if (rc != EOK) {
      goto BGInitEnd; 
   }

   // For non-primary instance, we are done
   if (core_id != 0) {
      goto BGInitEnd; 
   }
  
   // Setting up a pending transfer
   rc = ufs_setup_pending_xfer (hufs);
   if (rc != EOK) {
      goto BGInitEnd; 
   }

   // Check to see if the device has fDeviceInit set, if not, set it. 
   // In this case, busy code is not an error. 
   rc = ufs_set_fDeviceInit (hufs);
   if (rc == -EBUSY) {
      rc = EOK; 
   }

BGInitEnd:
   if(hufs != NULL) {
      ufs_free_handle(hufs); 
   }
   return rc; 
}

// Public API
int32 ufs_background_init_finish(uint32 core_id, uint32 init_ctxt)
{
   struct ufs_handle *hdl;
   (void) init_ctxt; 

   int32 rc = EOK;
   ufs_handle_t *hufs;

   // First get a free handle
   hufs = ufs_get_handle();
   if (hufs == NULL) {
      // Out of available handles
      return UFS_EINVAL;
   }

   // Set the LUN, core_id used
   hufs->lun = UFS_WLUN_DEVICE;
   hufs->ufs_core_id = core_id; 

   // Finish 2nd initialization phase: poll for fDeviceInit completion
   rc = ufs_phase2_init(hufs); 
   if (rc != EOK) {
      goto BGInitEnd; 
   }

   hdl = ufs_open(core_id, UFS_WLUN_DEVICE);
   if (hdl == NULL) {
     return UFS_EINVAL;
   }

BGInitEnd:
   if(hufs != NULL) {
      ufs_free_handle(hufs); 
   }
   return rc; 
}


// Public API 
struct ufs_handle *ufs_open (uint32 core_id, uint32 lun)
{
   int32 rc = EOK;
   ufs_handle_t *hufs;

   STOR_PROFILE_START;

   // First get a free handle
   hufs = ufs_get_handle();
   if (hufs == NULL) {
      // Out of available handles
      return NULL;
   }

   // Set the LUN + core_id used
   hufs->lun = lun;
   hufs->ufs_core_id = core_id; 

   // Phase 1 Init: PHY init, link startup, NOP OUT
   rc = ufs_phase1_init(hufs, lun, FALSE); 
   if (rc != EOK) {
      ufs_free_handle (hufs);
      return NULL;
   }

   // Phase 2 Init: Check to see if the device has fDeviceInit set, if not, set it
   if (lun != UFS_WLUN_BOOT) {
      rc = ufs_phase2_init(hufs); 
      if (rc != EOK) {
         ufs_free_handle (hufs); 
         return NULL; 
      }
   }

   // Do not check for the DEVICE WLUN
   if (lun != UFS_WLUN_DEVICE && lun != UFS_WLUN_BOOT) {
      // Check to make sure the LUN is a valid LUN 
      rc = ufs_check_valid_lun (hufs, lun & 0xff);
      if (rc != EOK) {
         ufs_free_handle (hufs);
         return NULL;
      }
   }

   // Send TEST UNIT READY 
   rc = ufs_scsi_unit_rdy (hufs);
   if (rc != EOK) {
      ufs_free_handle (hufs);
      return NULL;
   }

   // If the call is for BOOT_WLUN, skip the rest of the operations
   if (lun == UFS_WLUN_BOOT) {
      return hufs;
   }

   if (lun != UFS_WLUN_DEVICE) {
     // Set gear
     rc =  ufs_set_gear (hufs);
     if (rc != EOK) {
        ufs_free_handle (hufs);
        return NULL;
      }
   }

   if (lun != UFS_WLUN_BOOT) {
      // Get the default device descriptor
      rc = ufs_get_descriptor_device (hufs);
      if (rc != EOK) {
         ufs_free_handle (hufs);
         return NULL;
      }

      // For UFS 3.0, we can lower the Vcc to 2.5V if low volt support is present
      if (hufs->phc->plat_cfg.low_volt_support) {
         if ((!hufs->phc->volt_sw_done) && (hufs->phc->desc_dev.wSpecVersion >= 0x300)) {
            rc = ufs_scsi_start_stop_unit (hufs, 2);
            if (rc != EOK) {
               ufs_free_handle (hufs); 
               return NULL;
            }

            // Lower the Vcc to 0V for 2ms before changing to 2.5V 
            ufs_bsp_volt_set(hufs->ufs_core_id, UFS_BSP_REGULATORS_SLEEP);
            ufs_bsp_busy_wait(2000); 
            ufs_bsp_volt_set(hufs->ufs_core_id, UFS_BSP_REGULATORS_UFS_30);

            rc = ufs_scsi_start_stop_unit (hufs, 1);
            if (rc != EOK) {
               ufs_free_handle (hufs); 
               return NULL;
            }
            hufs->phc->volt_sw_done = 1; 
         }
      }
   }
   
   // Send a signle REQUEST SENSE command to get any error code
   rc = ufs_scsi_req_sense (hufs);
   if (rc != EOK) {
      ufs_free_handle (hufs); 
      return NULL;
   }

   // Start the unit 
   if (lun == UFS_WLUN_DEVICE) {
      rc = ufs_scsi_start_stop_unit (hufs, 1);
      if (rc != EOK) {
          ufs_free_handle (hufs); 
          return NULL;
      }
   }

   if (lun != UFS_WLUN_BOOT) {
      rc = ufs_scsi_inquiry (hufs);
      if (rc != EOK) {
         ufs_free_handle (hufs);
         return NULL;
      }

      rc = ufs_get_descriptor_config (hufs);
      if (rc != EOK) {
         ufs_free_handle (hufs);
         return NULL;
      }
   }

   // Get the unit descriptor for normal storage LUNs
   if (lun <= 0x07) {
      rc = ufs_get_descriptor_unit (hufs, lun);
      if (rc != EOK) {
         ufs_free_handle (hufs);
         return NULL;
      }
   }

   if (lun == UFS_WLUN_RPMB) {
      rc = ufs_get_descriptor_rpmb (hufs);
      if (rc != EOK) {
         ufs_free_handle (hufs);
         return NULL;
      }
   }

   rc = ufs_get_bBootLunEn (hufs);
   if (rc != EOK) {
      ufs_free_handle (hufs);
      return NULL;
   }

   rc = ufs_get_bConfigDescrLock (hufs);
   if (rc != EOK) {
      ufs_free_handle (hufs);
      return NULL;
   }

   if (lun != UFS_WLUN_BOOT) {
      rc = ufs_get_descriptor_geometry (hufs);
      if (rc != EOK) {
         ufs_free_handle (hufs);
         return NULL;
      }
   }

   STOR_PROFILE_END(UFS_PROFILE_INIT_BIT_MASK, "UFS_INIT", lun, 0);

   return hufs;
}


/* Public API */
int32 ufs_close (struct ufs_handle *handle)
{
   // Check input parameter
   if (handle == NULL) {
      return -EINVAL;
   }

   // Check inuse flag
   if (handle->is_inuse != UFS_INUSE_TRUE ||
       handle->id_magic != UFS_HANDLE_MAGIC_ID) {
      return -EINVAL;
   }

   // Make sure the host controlle interface is ready
   if (handle->phc == NULL) {
      return -EINVAL;
   }

   ufs_free_handle (handle);
   return EOK;
}


/* Public API */
int32 ufs_get_device_info (struct ufs_handle *handle, 
                           struct ufs_info_type *info)
{
   int32 rc = EOK;

   // Check input parameter
   if (handle == NULL) {
      return -EINVAL;
   }

   // Check inuse flag
   if (handle->is_inuse != UFS_INUSE_TRUE ||
       handle->id_magic != UFS_HANDLE_MAGIC_ID) {
      return -EINVAL;
   }

   // Make sure the host controlle interface is ready
   if (handle->phc == NULL) {
      return -EINVAL;
   }
   
   osal_memset (info, 0, sizeof (struct ufs_info_type));
   ufs_fill_in_device_info (handle, info);

   info->dLuTotalBlocks = info->qLogicalBlockCount;

   rc = ufs_get_flags(handle, info); 
   if (rc != EOK) {
      return rc;
   }

   return EOK;
}

static void ufs_convert_to_alloc_units (struct ufs_info_type *dev_info, uint64 size_in_kb, 
                                        uint64 blocks_per_alloc_unit, uint64 *alloc_units)
{
   uint64 num_alloc_units, size_in_4kb_chunk, size_in_blocks;

   size_in_4kb_chunk = (size_in_kb + 3) / 4;
   if (size_in_4kb_chunk > UINT32_MAX) {
      size_in_4kb_chunk = UINT32_MAX;
   }
   size_in_blocks = size_in_4kb_chunk / ((dev_info->bMinAddrBlockSize * 512) / 4096); 

   num_alloc_units = (size_in_blocks / blocks_per_alloc_unit);
   if (size_in_blocks % blocks_per_alloc_unit) {
      num_alloc_units++; 
   }

   *alloc_units = num_alloc_units; 
}
 
/* Public API */
int32 ufs_configure_device( struct ufs_handle *handle, 
                            struct ufs_lun_config_descr *cfg )
{
   char log_buffer[64] = {0};
   int32 rc = EOK;
   struct ufs_config_descr lun_cfg = {0}; 
   struct ufs_info_type dev_info; 
   uint8 i, lun_to_grow = 0, num_luns = 0;
   uint32 wb_buffer_count = 0, wb_supported = 0; 
   uint64 alloc_unit_in_512kb_chunk; 
   uint64 alloc_units_to_create = 0, alloc_units_lun = 0, alloc_units_total; 
   uint64 capacity_reduction = 0; 
   uint64 wb_units_to_create = 0, wb_alloc_units = 0; 
   uint64 blocks_per_alloc_unit; 
   uint64 enhanced1_units = 0, enhanced2_units = 0;

   // Check input parameter
   if (handle == NULL) {
      return -EINVAL;
   }

   // Check inuse flag
   if (handle->is_inuse != UFS_INUSE_TRUE ||
       handle->id_magic != UFS_HANDLE_MAGIC_ID) {
      return -EINVAL;
   }

   // Make sure the host controller interface is ready
   if (handle->phc == NULL) {
      return -EINVAL;
   }

   // Set the ref clock frequency- all current board design only support 19.2MHz ref clock 
   rc = ufs_set_bRefClkFreq (handle, 19200000);
   if (rc != EOK) {
      return rc;
   }

   rc = ufs_get_device_info (handle, &dev_info);
   if (rc != EOK) {
      snprintf(log_buffer, 64, "Get Dev Info Failed in Configure slot %d", 
               handle->ufs_core_id);
      goto ConfigLog; 
   }

   num_luns = sizeof(cfg->unit) / sizeof(cfg->unit[0]); 

   alloc_unit_in_512kb_chunk = (dev_info.dSegmentSize * dev_info.bAllocationUnitSize);

   blocks_per_alloc_unit = alloc_unit_in_512kb_chunk / dev_info.bMinAddrBlockSize; 
   if (blocks_per_alloc_unit == 0) {
      snprintf(log_buffer, 64, "Unsupported UFS memory type %d slot %d", dev_info.bMemoryType, handle->ufs_core_id); 
      rc = -EINVAL; 
      goto ConfigLog; 
   }

   if (dev_info.qTotalRawDeviceCapacity % alloc_unit_in_512kb_chunk) {
      snprintf(log_buffer, 64, "Raw capacity %d not a multiple of alloc unit size %d",
               dev_info.qTotalRawDeviceCapacity, alloc_unit_in_512kb_chunk); 
      rc = -EINVAL; 
      goto ConfigLog; 
   }
   alloc_units_total = dev_info.qTotalRawDeviceCapacity / alloc_unit_in_512kb_chunk; 

   // Check if write booster support is present 
   wb_supported = (dev_info.dExtendedUFSFeaturesSupport & 0x100) >> 8; 

   if (wb_supported == 0) {
      if ((cfg->bWriteBoosterBufferType != 0xFF) || (cfg->bWriteBoosterBufferPreserveUserSpaceEn != 0xFF) ||
          (cfg->qSharedWriteBoosterBufferSizeInKb != 0)) {
         snprintf(log_buffer, 64, "Write Booster not supported for slot %d", handle->ufs_core_id);  
         rc = -EINVAL; 
         goto ConfigLog; 
      }
   }
   else {
      // Check if bWriteBoosterBufferPreserveUserSpaceEn has a valid value 
      if (cfg->bWriteBoosterBufferPreserveUserSpaceEn == 1) {
         if (dev_info.bSupportedWriteBoosterBufferUserSpaceReductionTypes == 0) {
            snprintf(log_buffer, 64, "Invalid value: bWriteBoosterBufferPreserveUserSpaceEn %d", 
                     cfg->bWriteBoosterBufferPreserveUserSpaceEn);
            rc = -EINVAL;
            goto ConfigLog; 
         }
      }
      else if (cfg->bWriteBoosterBufferPreserveUserSpaceEn == 0) {
         if (dev_info.bSupportedWriteBoosterBufferUserSpaceReductionTypes == 1) {
            snprintf(log_buffer, 64, "Invalid value: bWriteBoosterBufferPreserveUserSpaceEn %d", 
                     cfg->bWriteBoosterBufferPreserveUserSpaceEn);
            rc = -EINVAL;
            goto ConfigLog; 
         }
      }
      else {
         snprintf(log_buffer, 64, "Invalid value: bWriteBoosterBufferPreserveUserSpaceEn %d", 
                  cfg->bWriteBoosterBufferPreserveUserSpaceEn);
         rc = -EINVAL; 
         goto ConfigLog; 
      }

      // Check if bWriteBoosterBufferType has a valid value 
      if (cfg->bWriteBoosterBufferType == 1) {
         if (dev_info.bSupportedWriteBoosterBufferTypes == 0) {
            snprintf(log_buffer, 64, "Invalid value: bWriteBoosterBufferType %d", 
                     cfg->bWriteBoosterBufferType);
            rc = -EINVAL;
            goto ConfigLog; 
         }
      }
      else if (cfg->bWriteBoosterBufferType == 0) {
         if (dev_info.bSupportedWriteBoosterBufferTypes == 1) {
            snprintf(log_buffer, 64, "Invalid value: bWriteBoosterBufferType %d", 
                     cfg->bWriteBoosterBufferType);
            rc = -EINVAL;
            goto ConfigLog; 
         }
      }
      else {
         snprintf(log_buffer, 64, "Invalid value: bWriteBoosterBufferType %d", 
                  cfg->bWriteBoosterBufferType);
         rc = -EINVAL; 
         goto ConfigLog; 
      }

      lun_cfg.bWriteBoosterBufferPreserveUserSpaceEn = cfg->bWriteBoosterBufferPreserveUserSpaceEn; 
      lun_cfg.bWriteBoosterBufferType = cfg->bWriteBoosterBufferType; 

      if (cfg->qSharedWriteBoosterBufferSizeInKb) {
         if (wb_buffer_count == dev_info.bDeviceMaxWriteBoosterLUs) {
            snprintf(log_buffer, 64, "Maximum number of Write Booster Buffers (%d) reached", 
                     dev_info.bDeviceMaxWriteBoosterLUs);
            rc = -EINVAL; 
            goto ConfigLog; 
         }

         wb_buffer_count += 1; 
         ufs_convert_to_alloc_units(&dev_info, cfg->qSharedWriteBoosterBufferSizeInKb, 
                                    blocks_per_alloc_unit, &wb_alloc_units); 

         wb_units_to_create += wb_alloc_units; 

         // Reduce capacity if applicable 
         if (cfg->bWriteBoosterBufferPreserveUserSpaceEn == 0) {
            capacity_reduction = dev_info.bWriteBoosterBufferCapAdjFac * wb_alloc_units; 
            alloc_units_total = alloc_units_total - capacity_reduction;  
         }

         lun_cfg.dNumSharedWriteBoosterBufferAllocUnits = wb_alloc_units; 
      }
   }

   for (i = 0; ((i < num_luns) && (alloc_units_to_create <= alloc_units_total) && 
                (wb_units_to_create <= dev_info.dWriteBoosterBufferMaxNAllocUnits)); i++) {
      if (cfg->unit[i].qWriteBoosterBufferSizeInKb) {
         if (wb_supported == 0) {
            snprintf(log_buffer, 64, "Write Booster not supported for slot %d", handle->ufs_core_id); 
            rc = -EINVAL; 
            goto ConfigLog; 
         }

         if (wb_buffer_count == dev_info.bDeviceMaxWriteBoosterLUs) {
            snprintf(log_buffer, 64, "Maximum number of Write Booster Buffers (%d) reached", 
                     dev_info.bDeviceMaxWriteBoosterLUs);
            rc = -EINVAL; 
            goto ConfigLog; 
         }

         // Write booster only allowed for LUN with bMemoryType = 0
         if (cfg->unit[i].bMemoryType != 0) {
            snprintf(log_buffer, 64, "Write Booster not supported for memory type %d", cfg->unit[i].bMemoryType);
            rc = -EINVAL; 
            goto ConfigLog; 
         }

         // Write booster only allowed for LUN with bBootLunID = 0
         if (cfg->unit[i].bBootLunID != 0) {
            snprintf(log_buffer, 64, "Write Booster not supported for Boot LUN id %d", cfg->unit[i].bBootLunID);
            rc = -EINVAL; 
            goto ConfigLog; 
         }

         wb_buffer_count += 1; 
         ufs_convert_to_alloc_units(&dev_info, cfg->unit[i].qWriteBoosterBufferSizeInKb, 
                                    blocks_per_alloc_unit, &wb_alloc_units); 

         wb_units_to_create += wb_alloc_units; 

         // Reduce capacity if applicable 
         if (cfg->bWriteBoosterBufferPreserveUserSpaceEn == 0) {
            capacity_reduction = dev_info.bWriteBoosterBufferCapAdjFac * wb_alloc_units; 
            alloc_units_total = alloc_units_total - capacity_reduction;  
         }

         lun_cfg.unit[i].dLUNumWriteBoosterBufferAllocUnits = wb_alloc_units; 
         lun_cfg.bWriteBoosterBufferPreserveUserSpaceEn = cfg->bWriteBoosterBufferPreserveUserSpaceEn; 
         lun_cfg.bWriteBoosterBufferType = cfg->bWriteBoosterBufferType; 
      }

      ufs_convert_to_alloc_units (&dev_info, cfg->unit[i].qSizeInKb, 
                                  blocks_per_alloc_unit, &alloc_units_lun); 

      if (cfg->unit[i].bMemoryType == 3) {
         alloc_units_lun = alloc_units_lun * (dev_info.wEnhanced1CapAdjFac / 0x100); 
         enhanced1_units += alloc_units_lun; 
      }
      else if (cfg->unit[i].bMemoryType == 4) {
         alloc_units_lun = alloc_units_lun * (dev_info.wEnhanced2CapAdjFac / 0x100);
         enhanced2_units += alloc_units_lun; 
      }
      else if (cfg->unit[i].bMemoryType != 0) {
         snprintf(log_buffer, 64, "Unsupported UFS memory type %d", cfg->unit[i].bMemoryType);  
         rc = -EINVAL; 
         goto ConfigLog; 
      }

      alloc_units_to_create += alloc_units_lun; 

      lun_cfg.unit[i].dNumAllocUnits = alloc_units_lun; 
      lun_cfg.unit[i].bLUEnable = cfg->unit[i].bLUEnable; 
      lun_cfg.unit[i].bBootLunID = cfg->unit[i].bBootLunID; 
      lun_cfg.unit[i].bLUWriteProtect = cfg->unit[i].bLUWriteProtect; 
      lun_cfg.unit[i].bMemoryType = cfg->unit[i].bMemoryType; 
      lun_cfg.unit[i].bDataReliability = cfg->unit[i].bDataReliability; 
      lun_cfg.unit[i].bLogicalBlockSize = cfg->unit[i].bLogicalBlockSize; 
      lun_cfg.unit[i].bProvisioningType = cfg->unit[i].bProvisioningType; 
      lun_cfg.unit[i].wContextCapabilities = cfg->unit[i].wContextCapabilities;  
   }

   // Check if we are over allocating for the enhanced regions
   if (enhanced1_units > dev_info.dEnhanced1MaxNAllocU) {
      snprintf(log_buffer, 64, "Size %d exceeds max enhanced1 area size %d",
               enhanced1_units, dev_info.dEnhanced1MaxNAllocU); 
      rc = -EINVAL; 
      goto ConfigLog; 
   }

   if (enhanced2_units > dev_info.dEnhanced2MaxNAllocU) {
      snprintf(log_buffer, 64, "Size %d exceeds max enhanced2 area size %d",
               enhanced2_units, dev_info.dEnhanced2MaxNAllocU); 
      rc = -EINVAL; 
      goto ConfigLog; 
   }

   // Check if we are over allocating the maximum write booster buffer size
   if (wb_units_to_create > dev_info.dWriteBoosterBufferMaxNAllocUnits) {
      snprintf(log_buffer, 64, 
               "Specified write booster size %d exceeds maximum write booster size %d",
               wb_units_to_create, dev_info.dWriteBoosterBufferMaxNAllocUnits); 
      rc = -EINVAL; 
      goto ConfigLog; 
   }

   // Check if we are over allocating the total allocation units 
   if (alloc_units_to_create > alloc_units_total) {
      snprintf(log_buffer, 64, "Specified size %d exceeds device size %d",
               alloc_units_to_create, alloc_units_total); 
      rc = -EINVAL; 
      goto ConfigLog; 
   }

   lun_to_grow = cfg->lun_to_grow;
   if ((0 <= lun_to_grow) && (lun_to_grow < UFS_MAX_LUNS)) {
      lun_cfg.unit[lun_to_grow].dNumAllocUnits += (alloc_units_total - alloc_units_to_create);
   }

   lun_cfg.bBootEnable = cfg->bBootEnable; 
   lun_cfg.bDescrAccessEn = cfg->bDescrAccessEn; 
   lun_cfg.bInitPowerMode = cfg->bInitPowerMode; 
   lun_cfg.bHighPriorityLUN = cfg->bHighPriorityLUN; 
   lun_cfg.bSecureRemovalType = cfg->bSecureRemovalType; 
   lun_cfg.bInitActiveICCLevel = cfg->bInitActiveICCLevel; 
   lun_cfg.wPeriodicRTCUpdate = cfg->wPeriodicRTCUpdate; 
   lun_cfg.bConfigDescrLock = cfg->bConfigDescrLock; 
   lun_cfg.qVendorConfigCode = cfg->qVendorConfigCode;

   rc = ufs_do_config_device (handle, &lun_cfg);
   if (rc != EOK) {
      return rc;
   }

   // Disable pre-program mode if it is requested
   if (cfg->qVendorConfigCode != 0) {
      rc = ufs_vend_pprog_ctrl (handle, cfg->qVendorConfigCode);
   }

ConfigLog:
   if (rc != EOK) {
      ufs_bsp_log_error(log_buffer); 
   }
   return rc;
}


/* Public API */
int32 ufs_set_bootable (struct ufs_handle *handle, uint32 lun)
{
   int32 rc = EOK;
   // Check input parameter
   if (handle == NULL) {
      return -EINVAL;
   }

   // Check inuse flag
   if (handle->is_inuse != UFS_INUSE_TRUE ||
       handle->id_magic != UFS_HANDLE_MAGIC_ID) {
      return -EINVAL;
   }

   // Make sure the host controlle interface is ready
   if (handle->phc == NULL) {
      return -EINVAL;
   }

   // Allow only LUN 0 - Disable, 1 - Boot A or 2 - Boot B
   if (lun > UFS_LUN_BOOT_B) {
      return -EINVAL;
   }  

   // Set the boot enable flag in the attributes bBootLunEn
   rc = ufs_set_bBootLunEn (handle, lun);
   if (rc != EOK) {
      return -EIO;
   }

   return rc;
}


/* Public API */
int32 ufs_set_pon_write_protect (struct ufs_handle *handle)
{
   int32 rc = EOK;

   // Check input parameter
   if (handle == NULL) {
      return -EINVAL;
   }

   // Check inuse flag
   if (handle->is_inuse != UFS_INUSE_TRUE ||
       handle->id_magic != UFS_HANDLE_MAGIC_ID) {
      return -EINVAL;
   }

   // Make sure the host controlle interface is ready
   if (handle->phc == NULL) {
      return -EINVAL;
   }

   rc = ufs_set_fPowerOnWPEn (handle);
   return rc;
}


/* Public API */
int32 ufs_purge_and_wait( struct ufs_handle *handle)
{
   int32 rc = EOK;

   // Check input parameter
   if (handle == NULL) {
      return -EINVAL;
   }

   // Check inuse flag
   if (handle->is_inuse != UFS_INUSE_TRUE ||
       handle->id_magic != UFS_HANDLE_MAGIC_ID) {
      return -EINVAL;
   }

   // Make sure the host controlle interface is ready
   if (handle->phc == NULL) {
      return -EINVAL;
   }

   rc = ufs_set_n_wait_fPurgeEnable (handle);
   return rc;
}

/* Public API */
int32 ufs_purge_nonblock_setup (struct ufs_handle *handle)
{
   int32 rc = EOK;

   // Check input parameter
   if (handle == NULL) {
      return -EINVAL;
   }

   // Check inuse flag
   if (handle->is_inuse != UFS_INUSE_TRUE ||
       handle->id_magic != UFS_HANDLE_MAGIC_ID) {
      return -EINVAL;
   }

   // Make sure the host controlle interface is ready
   if (handle->phc == NULL) {
      return -EINVAL;
   }

   // Setup a pending operation
   rc = ufs_setup_pending_xfer (handle);
   if (rc != EOK) {
      return rc;
   }

   rc = ufs_set_n_wait_fPurgeEnable (handle);
   return rc;
}

/* Public API */
int32 ufs_read (struct ufs_handle *handle,
                uint8           *buff,
                uint64           lba,
                uint32           blocks)
{
   int32 rc = EOK;

   STOR_PROFILE_START;

   // Check input parameter
   if (handle == NULL) {
      return -EINVAL;
   }

   // Check inuse flag
   if (handle->is_inuse != UFS_INUSE_TRUE ||
       handle->id_magic != UFS_HANDLE_MAGIC_ID) {
      return -EINVAL;
   }

   // Make sure the host controlle interface is ready
   if (handle->phc == NULL) {
      return -EINVAL;
   }

   // Call the SCSI command building function
   rc = ufs_scsi_read (handle, buff, lba, blocks);

   STOR_PROFILE_END(UFS_PROFILE_READ_BIT_MASK, "UFS_READ", lba, blocks);

   return rc;
}


/* Public API */
int32 ufs_write (struct ufs_handle *handle,
                 uint8             *buff,
                 uint64             lba,
                 uint32             blocks)
{
   int32 rc = EOK;

   STOR_PROFILE_START;

   // Check input parameter
   if (handle == NULL) {
      return -EINVAL;
   }

   // Check inuse flag
   if (handle->is_inuse != UFS_INUSE_TRUE ||
       handle->id_magic != UFS_HANDLE_MAGIC_ID) {
      return -EINVAL;
   }

   // Make sure the host controlle interface is ready
   if (handle->phc == NULL) {
      return -EINVAL;
   }

   // Call the SCSI command building function
   rc = ufs_scsi_write (handle, buff, lba, blocks);

   STOR_PROFILE_END(UFS_PROFILE_WRITE_BIT_MASK, "UFS_WRITE", lba, blocks);

   return rc;
}

/* Public API */
int32 ufs_read_ex (struct ufs_handle *handle,
                   uint8             *buff,
                   uint64             lba,
                   uint32             blocks)
{
   int32 rc = EOK;

   // Check input parameter
   if (handle == NULL) {
      return -EINVAL;
   }

   // Check inuse flag
   if (handle->is_inuse != UFS_INUSE_TRUE ||
       handle->id_magic != UFS_HANDLE_MAGIC_ID) {
      return -EINVAL;
   }

   // Make sure the host controlle interface is ready
   if (handle->phc == NULL) {
      return -EINVAL;
   }

   // Setting up a pending transfer
   rc = ufs_setup_pending_xfer (handle);
   if (rc != EOK) {
      return rc;
   }

   // Call the SCSI command building function
   rc = ufs_scsi_read (handle, buff, lba, blocks);
   return rc;
}


/* Public API */
int32 ufs_write_ex (struct ufs_handle *handle,
                    uint8             *buff,
                    uint64             lba,
                    uint32             blocks)
{
   int32 rc = EOK;
   // Check input parameter
   if (handle == NULL) {
      return -EINVAL;
   }

   // Check inuse flag
   if (handle->is_inuse != UFS_INUSE_TRUE ||
       handle->id_magic != UFS_HANDLE_MAGIC_ID) {
      return -EINVAL;
   }

   // Make sure the host controlle interface is ready
   if (handle->phc == NULL) {
      return -EINVAL;
   }

   // Setting up a pending transfer
   rc = ufs_setup_pending_xfer (handle);
   if (rc != EOK) {
      return rc;
   }

   // Call the SCSI command building function
   rc = ufs_scsi_write (handle, buff, lba, blocks);
   return rc;
}


/* Public API */
int32 ufs_unmap (struct ufs_handle *handle,
                 uint64             lba,
                 uint32             blocks)
{
   int32 rc = EOK;

   STOR_PROFILE_START;

   // Check input parameter
   if (handle == NULL) {
      return -EINVAL;
   }

   // Check inuse flag
   if (handle->is_inuse != UFS_INUSE_TRUE ||
       handle->id_magic != UFS_HANDLE_MAGIC_ID) {
      return -EINVAL;
   }

   // Make sure the host controlle interface is ready
   if (handle->phc == NULL) {
      return -EINVAL;
   }

   // Call the SCSI command building function
   rc = ufs_scsi_unmap (handle, lba, blocks);

   STOR_PROFILE_END(UFS_PROFILE_ERASE_BIT_MASK, "UFS_UNMAP", lba, blocks);

   return rc;
}

/* Public API */
int32 ufs_format (struct ufs_handle *handle)
{
   int32 rc = EOK;
   // Check input parameter
   if (handle == NULL) {
      return -EINVAL;
   }

   // Check inuse flag
   if (handle->is_inuse != UFS_INUSE_TRUE ||
       handle->id_magic != UFS_HANDLE_MAGIC_ID) {
      return -EINVAL;
   }

   // Make sure the host controlle interface is ready
   if (handle->phc == NULL) {
      return -EINVAL;
   }

   // Call the SCSI command building function
   rc = ufs_scsi_format (handle);
   return rc;
}

/* Public API */
int32 ufs_rpmb_read_cnt_pkt (struct ufs_handle *handle, uint8 *resp)
{
   int32 rc = EOK;
   // Check input parameter
   if (handle == NULL) {
      return -EINVAL;
   }

   // Check inuse flag
   if (handle->is_inuse != UFS_INUSE_TRUE ||
       handle->id_magic != UFS_HANDLE_MAGIC_ID) {
      return -EINVAL;
   }

   // Make sure the host controlle interface is ready
   if (handle->phc == NULL) {
      return -EINVAL;
   }

   // This API only work with RPMB WLUN
   if (handle->lun != UFS_WLUN_RPMB) {
      return -EINVAL;
   }

   // Call the SCSI command building function
   rc = ufs_mem_rpmb_read_write_cnt (handle, resp);
   return rc;
}

/* Public API */
int32 ufs_rpmb_send_cmd_pkt (struct ufs_handle *handle, 
                             uint8 *cmd_pkt, 
                             uint8 *rsp_pkt)
{
   int32 rc = EOK;
   // Check input parameter
   if (handle == NULL) {
      return -EINVAL;
   }

   // Check inuse flag
   if (handle->is_inuse != UFS_INUSE_TRUE ||
       handle->id_magic != UFS_HANDLE_MAGIC_ID) {
      return -EINVAL;
   }

   // Make sure the host controlle interface is ready
   if (handle->phc == NULL) {
      return -EINVAL;
   }

   // This API only work with RPMB WLUN
   if (handle->lun != UFS_WLUN_RPMB) {
      return -EINVAL;
   }

   // Call the SCSI command building function
   rc = ufs_mem_rpmb_msg_out (handle, cmd_pkt);
   if (rc != EOK) {
      return rc;
   }

   rc = ufs_mem_rpmb_msg_in (handle, rsp_pkt);
   return rc;
}

/* Public API */
int32 ufs_rpmb_read_blocks (struct ufs_handle *handle, 
                            uint8 *cmd_pkt, 
                            uint32 pkt_cnt, 
                            uint8 *data_pkt)
{
   int32 rc = EOK;
   // Check input parameter
   if (handle == NULL) {
      return -EINVAL;
   }

   // Check inuse flag
   if (handle->is_inuse != UFS_INUSE_TRUE ||
       handle->id_magic != UFS_HANDLE_MAGIC_ID) {
      return -EINVAL;
   }

   // Make sure the host controlle interface is ready
   if (handle->phc == NULL) {
      return -EINVAL;
   }

   // This API only work with RPMB WLUN
   if (handle->lun != UFS_WLUN_RPMB) {
      return -EINVAL;
   }

   // Call the SCSI command building function
   rc = ufs_mem_rpmb_read_data (handle, pkt_cnt, cmd_pkt, data_pkt);
   return rc;
}

/* Public API */
int32 ufs_rpmb_write_blocks (struct ufs_handle *handle, 
                             uint8 *cmd_pkt, 
                             uint32 pkt_cnt, 
                             uint8 *resp_pkt)
{
   int32 rc = EOK;
   // Check input parameter
   if (handle == NULL) {
      return -EINVAL;
   }

   // Check inuse flag
   if (handle->is_inuse != UFS_INUSE_TRUE ||
       handle->id_magic != UFS_HANDLE_MAGIC_ID) {
      return -EINVAL;
   }

   // Make sure the host controlle interface is ready
   if (handle->phc == NULL) {
      return -EINVAL;
   }

   // This API only work with RPMB WLUN
   if (handle->lun != UFS_WLUN_RPMB) {
      return -EINVAL;
   }

   // Call the SCSI command building function
   rc = ufs_mem_rpmb_write_data (handle, pkt_cnt, cmd_pkt, resp_pkt);
   return rc;
}


/* For Tools debugging, get device descriptor string */
/* Public API */
int32 ufs_get_device_info_str (struct ufs_handle *handle,
                               uint32             id,
                               char              *buf,
                               uint32             size)
{
   int32 rc = EOK;
   // Check input parameter
   if (handle == NULL || buf == NULL) {
      return -EINVAL;
   }

   // Check inuse flag
   if (handle->is_inuse != UFS_INUSE_TRUE ||
       handle->id_magic != UFS_HANDLE_MAGIC_ID) {
      return -EINVAL;
   }

   // Make sure the host controlle interface is ready
   if (handle->phc == NULL) {
      return -EINVAL;
   }

   // Call the SCSI command building function
   rc = ufs_get_descriptor_str (handle, id, buf, size);
   return rc;
}

int32 ufs_poll_xfer (struct ufs_handle *handle )
{
   int32 rc = EOK;
   // Check input parameter
   if (handle == NULL) {
      return -EINVAL;
   }

   // Check inuse flag
   if (handle->is_inuse != UFS_INUSE_TRUE ||
       handle->id_magic != UFS_HANDLE_MAGIC_ID) {
      return -EINVAL;
   }

   // Make sure the host controlle interface is ready
   if (handle->phc == NULL) {
      return -EINVAL;
   }

   rc = ufs_finish_pending_xfer (handle);
   return rc;
}

int32 ufs_fw_update_write (ufs_handle_t *handle, int32_t size, 
                           int32_t offset, uint8_t *buf)
{
   int32 rc = EOK;

   if (handle == NULL) {
      return -EINVAL;
   }

   // Check inuse flag
   if (handle->is_inuse != UFS_INUSE_TRUE ||
       handle->id_magic != UFS_HANDLE_MAGIC_ID) {
      return -EINVAL;
   }

   // Make sure the host controlle interface is ready
   if (handle->phc == NULL) {
      return -EINVAL;
   }

   rc = ufs_scsi_write_buf (handle, buf, 0x0e, 0x00, offset, size);
   return rc;
}

int32 ufs_autoH8(ufs_handle_t *handle)
{
   if (handle == NULL) {
      return -EINVAL;
   }

   // Check inuse flag
   if (handle->is_inuse != UFS_INUSE_TRUE ||
       handle->id_magic != UFS_HANDLE_MAGIC_ID) {
      return -EINVAL;
   }

   // Make sure the host controlle interface is ready
   if (handle->phc == NULL) {
      return -EINVAL;
   }

   ufs_enable_autoH8(handle); 
   return EOK; 
}

void ufs_clk_ctrl (uint8_t core_id, uint32_t mode)
{
   ufs_bsp_clk_set(core_id, mode); 
}

void ufs_volt_ctrl (uint8_t core_id, uint32_t mode)
{
   ufs_bsp_volt_set(core_id, mode); 
}

int32 ufs_clock_gate (struct ufs_handle *handle)
{
   int32 rc = EOK;

   // Check input parameter
   if (handle == NULL) {
      return -EINVAL;
   }

   // Check inuse flag
   if (handle->is_inuse != UFS_INUSE_TRUE ||
       handle->id_magic != UFS_HANDLE_MAGIC_ID) {
      return -EINVAL;
   }

   // Make sure the host controlle interface is ready
   if (handle->phc == NULL) {
      return -EINVAL;
   }

   rc = ufs_do_clock_gate (handle); 
   return rc; 
}

int32 ufs_clock_ungate (struct ufs_handle *handle)
{
   int32 rc = EOK;

   // Check input parameter
   if (handle == NULL) {
      return -EINVAL;
   }

   // Check inuse flag
   if (handle->is_inuse != UFS_INUSE_TRUE ||
       handle->id_magic != UFS_HANDLE_MAGIC_ID) {
      return -EINVAL;
   }

   // Make sure the host controlle interface is ready
   if (handle->phc == NULL) {
      return -EINVAL;
   }

   rc = ufs_do_clock_ungate (handle); 
   return rc; 
}

/* Test code for R/w profiling - Please note this may corrupt the images */
#if 0
void storage_ufs_test(void)
{
  ufs_handle_t * uh;
  uint8 *buf;
  UINT32 try_count = 0;
  UINT32 chunk_in_sectors[] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192};
  UINT32 i = 0, j = 0;
  UINT32 sector_size = 4096, sector_addr = 0;
  UINT32 offset = 25600; //Test starts @ 100MB

  /* IMPORTANT NOTE: DISABLE ALL OTHER PROFILE EVENTS WHEN
   *                 STOR_PROFILE_BENCHMARK_BIT_MASK IS ENABLED.
   *                 THE BUFFER (buf) ADDRESS NEEDS TO BE ADJUSTED
   *                 BASED ON TARGET MEMORY MAP. TAKE AN UNUSED 4MB AREA.
   *                 ALSO CALL THIS FUNCTION AFTER DDR INIT
   */

  buf = (uint8 *) 0x80080000;
  uh = ufs_open(0, 0);
  if (uh == NULL) return;

  for (j=0; j<=13;j++) // For different chunk sizes
  {
    /* Test 4MB data read */
    try_count = (4096*1024) / (chunk_in_sectors[j] * sector_size);

    STOR_PROFILE_START;

    for (i=0;i<try_count;i++)
    {
      sector_addr = offset + (i * chunk_in_sectors[j]);
      ufs_read(uh, buf, sector_addr, chunk_in_sectors[j]);
    }

    STOR_PROFILE_END(STOR_PROFILE_BENCHMARK_BIT_MASK, "UFS_READ",
      chunk_in_sectors[j],try_count);
  }

  for (j=0; j<=13;j++) // For different chunk sizes
  {
    /* Test 4MB data write */
    try_count = (4096*1024) / (chunk_in_sectors[j] * sector_size);

    STOR_PROFILE_START;

    for (i=0;i<try_count;i++)
    {
      sector_addr = offset + (i * chunk_in_sectors[j]);
      ufs_write(uh, buf, sector_addr, chunk_in_sectors[j]);
    }

    STOR_PROFILE_END(STOR_PROFILE_BENCHMARK_BIT_MASK, "UFS_WRITE",
      chunk_in_sectors[j], try_count);

  }
}
#endif


