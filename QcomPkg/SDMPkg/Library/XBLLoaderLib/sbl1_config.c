/**
 * @file sbl1_config.c
 * @brief
 * Source file for bootloader configuration
 *
 *        Copyright (c) 2011-2018 QUALCOMM Technologies, Inc.
 *             All Rights Reserved.
 *        Qualcomm Technologies Inc. Proprietary and Confidential.
 *
 */

/*==========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.
        
when       who     what, where, why
--------   ---     ---------------------------------------------------------
05/30/18   vg      Added apt_sti_init params
04/16/18   vk      Add check for FORCE_DLOAD_TIMEOUT_RESET_WARM_BOOT
02/06/18   yps     Remove sbl1_ddr_set_default_params
01/31/18   vk      Refactor sbl1_config_table structure
01/18/18   vk      Add FlexSku 
08/04/17   naa     Added call to boot_ddi_entry
06/14/17   vk      Add loader procs
05/15/17   c_vkuppa Remove rollback protection from sbl1_config_table
05/11/17   kpa     remove unused api sbl1_restore_hash_segment_buffer
04/24/17   ds      xblconfig_init log message
04/20/17   kpa     remove pmic image loading
04/08/17   kpa     remove shrm image loading
03/08/17   kpa     enable xbl config init/deinit
03/07/17   ds	     added aop pre init call in aop preprocs
03/03/17   ds	     remove clobber prot
02/14/17   kpa     added xbl config init
01/27/17   digants assign the img whitelist pointer for each image
01/25/17   kpa     reorder AOP loading sequence
12/05/16   kpa     Load Pmic elf in dload mode
11/07/16   kpa     AOP SHRM Image loading sequence updates
09/12/16   kpa     remove rpm image loading logic
07/19/16   kpa     Load SHRM, AOP images
06/30/16   digant  Load APDP before QSEE devcfg
05/09/16   digant  STI image mmu mapping port and enablement  
10/18/11   dh      Initial Creation 

============================================================================
                     Copyright 2014-2015 Qualcomm Technologies Incorporated.
                            All Rights Reserved.
                    Qualcomm Confidential and Proprietary
===========================================================================*/

/*==========================================================================
                             INCLUDE FILES
===========================================================================*/
#include "boot_mc.h"
#include "boot_config.h"
#include "boot_flash_target.h"
#include "boot_flash_dev_if.h"
#include "boot_authenticator.h"
#include "boot_config_data.h"
#include "boot_dload.h"
#include "boot_loader.h"
#include "boot_ram_init.h"
#include "boot_logger.h"
#include "boot_page_table_armv8.h"
#include "boot_cache_mmu.h"
#include "boot_smem.h"
#include "boot_ram_partition.h"
#include "boot_ddr_info.h"
#include "sbl1_hw.h"
#include "sbl1_mc.h"
#include "boot_dload.h"
#include "boothw_target.h"
#include "boot_extern_clk_interface.h"
#include "boot_extern_ddr_interface.h"
#include "boot_shared_imem_cookie.h"
#include "boot_extern_pmic_interface.h"
#include "boot_extern_deviceprogrammer_interface.h"
#include "boot_extern_apt_interface.h"
#include "boot_extern_vsense_interface.h"
#include "boot_elf_header.h"
#include "sys_debug.h"
#include "boot_hash_buffer.h"
#include "boot_util.h"
#include "LoaderAptLib.h"
#include "boot_dload_dump.h"
#include "boot_fastcall_tz.h"
#include "boot_extern_platforminfo_interface.h"
#include "XBLConfig.h"
#include "boot_extern_ddi_interface.h"

/*=============================================================================

            LOCAL DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, types,
variables and other items needed by this module.

=============================================================================*/
#define QSEE_BOOT_LOG_STR        (uint8 *)"Loading QSEE Image"
#define QHEE_BOOT_LOG_STR        (uint8 *)"Loading QHEE Image"
#define APPSBL_BOOT_LOG_STR      (uint8 *)"Loading APPSBL Image"
#define QSEE_DEVCFG_BOOT_LOG_STR (uint8 *)"Loading QSEE Dev Config Image"
#define APDP_BOOT_LOG_STR        (uint8 *)"Loading APDP Image"
#define STI_BOOT_LOG_STR         (uint8 *)"Loading STI Image"
#define ABL_BOOT_LOG_STR         (uint8 *)"Loading ABL Image"
#define AOP_BOOT_LOG_STR         (uint8 *)"Loading AOP Image"
#define OEM_MISC_BOOT_LOG_STR    (uint8 *)"Loading OEM_MISC Image"
#define QTI_MISC_BOOT_LOG_STR    (uint8 *)"Loading QTI_MISC Image"
#define SEC_BOOT_LOG_STR         (uint8 *)"Loading SEC Image"

/*==========================================================================
                      DEFINE PARTITION IDs
===========================================================================*/
extern uint8 qsee_partition_id[];
extern uint8 qhee_partition_id[];
extern uint8 appsbl_partition_id[];
extern uint8 sbl1_partition_id[];
extern uint8 qsee_devcfg_image_partition_id[];
extern uint8 apdp_partition_id[];
extern uint8 sti_partition_id[];
extern uint8 abl_partition_id[];
extern uint8 shrm_partition_id[];
extern uint8 aop_partition_id[];
extern uint8 multi_image_partition_id[];
extern uint8 multi_image_qti_partition_id[];
extern uint8 secdata_partition_id[];
extern uint8 XBLConfig_partition_id[];

extern uint8 qsee_recovery_partition_id[];
extern uint8 qhee_recovery_partition_id[];
extern uint8 qsee_devcfg_recovery_partition_id[];
extern uint8 aop_recovery_partition_id[];
extern uint8 XBLConfig_recovery_partition_id[];



boot_configuration_table_entry sbl1_config_table[];

/* TODO: _FIX_ for 8996. TZ cannot access shared data in L2 TCM since it disables mmu.
  If mmu disabling in TZ is postponed beyond shared data copy, a branch 'bl' instrn
  seemed to have issues. Hence temporarily 
*/
struct boot_sbl_qsee_interface sbl_qsee_interface_ddr SECTION(".bss.BOOT_DDR_ZI_DATA_ZONE");
uint32 appsbl_index = 0;

/* SYSTEM_DEBUG_QSEE_INFO is memory specified in the scatter file for SBL
   and system debug to share information about where QSEE was loaded and
   if it's a 32 bit or 64 bit image. */
extern uintnt Image$$SYSTEM_DEBUG_QSEE_INFO$$Base;

#ifdef FEATURE_DEVICEPROGRAMMER_IMAGE
extern uintnt  Image$$SBL1_DEVICEPROGRAMMER_USB_ZI$$Base[];
extern uintnt  Image$$SBL1_DEVICEPROGRAMMER_USB_ZI$$ZI$$Length[];
#endif


/*==========================================================================
                     ASSIGN IMAGE WHITELIST TABLES
===========================================================================*/
static whitelst_tbl_entry_type aop_img_whitelist[] = AOP_IMG_WHITELIST;
static whitelst_tbl_entry_type apdp_img_whitelist[] = APDP_IMG_WHITELIST;
static whitelst_tbl_entry_type oem_misc_img_whitelist[] = OEM_MISC_IMG_WHITELIST;
static whitelst_tbl_entry_type qti_misc_img_whitelist[] = QTI_MISC_IMG_WHITELIST;
static whitelst_tbl_entry_type devcfg_img_whitelist[] = DEVCFG_IMG_WHITELIST;
static whitelst_tbl_entry_type qsee_img_whitelist[] = QSEE_IMG_WHITELIST;
static whitelst_tbl_entry_type qhee_img_whitelist[] = QHEE_IMG_WHITELIST;
static whitelst_tbl_entry_type xbl_core_img_whitelist[] = XBL_CORE_IMG_WHITELIST;
//static whitelst_tbl_entry_type abl_img_whitelist[] = ABL_IMG_WHITELIST;
static whitelst_tbl_entry_type sti_img_whitelist[] = STI_IMG_WHITELIST;
static whitelst_tbl_entry_type sec_img_whitelist[] = SEC_IMG_WHITELIST;

/*==========================================================================
                     PRE/POST PROCEDURE DEFINITIONS 
===========================================================================*/

/* Wrapper function to hand control to deviceprogranmmer image */ 
static void sbl1_hand_control_to_devprog_ddr(bl_shared_data_type *bl_shared_data)
{
#ifdef FEATURE_DEVICEPROGRAMMER_IMAGE
  boot_boolean result = FALSE;
  struct mem_block devprg_mem_block;
  
  /* Update attributes for non-cacheable regions */
  devprg_mem_block.p_base = SCL_SBL1_OCIMEM_DATA_BASE;
  devprg_mem_block.v_base = SCL_SBL1_OCIMEM_DATA_BASE;
  devprg_mem_block.size_in_kbytes = SCL_SBL1_OCIMEM_DATA_SIZE >> 10;
  devprg_mem_block.memory_mapping = MMU_L3_SECTION_MAPPING;
  devprg_mem_block.access = MMU_PAGETABLE_MEM_READ_WRITE;
  devprg_mem_block.cachetype = MMU_PAGETABLE_MEM_NON_CACHEABLE;
  devprg_mem_block.executable = MMU_PAGETABLE_NON_EXECUTABLE_REGION;

  result =
    boot_mmu_page_table_map_single_mem_block((uint64*)mmu_get_page_table_base(),
                                             &devprg_mem_block);
  BL_VERIFY(result, BL_ERR_MMU_PGTBL_MAPPING_FAIL|BL_ERROR_GROUP_BOOT);

  devprg_mem_block.p_base = (uint32)Image$$SBL1_DEVICEPROGRAMMER_USB_ZI$$Base;
  devprg_mem_block.v_base = (uint32)Image$$SBL1_DEVICEPROGRAMMER_USB_ZI$$Base;
  devprg_mem_block.size_in_kbytes = ((uint32)Image$$SBL1_DEVICEPROGRAMMER_USB_ZI$$ZI$$Length >> 10);
  devprg_mem_block.cachetype = MMU_PAGETABLE_MEM_WRITE_BACK_CACHE;
  devprg_mem_block.memory_mapping = MMU_L2_SECTION_MAPPING;
  result =
    boot_mmu_page_table_map_single_mem_block((uint64*)mmu_get_page_table_base(),
                                             &devprg_mem_block);
  BL_VERIFY(result, BL_ERR_MMU_PGTBL_MAPPING_FAIL|BL_ERROR_GROUP_BOOT);
  mmu_flush_cache();
  mmu_invalidate_tlb();

  boot_hand_control_to_deviceprogrammer_ddr_main(bl_shared_data);
#endif /* FEATURE_DEVICEPROGRAMMER_IMAGE */
}



/*===========================================================================
**  Function :  sbl1_update_partition_ids
** ==========================================================================
*/
/*!
* 
* @brief
*  This function updates the image partiton id with the corresponding 
*  partition GUIDs for boot targets that are GUID based (eMMC/SD/UFS)
*
* @param[in] shared_data Pointer to shared data
*        
* @par Dependencies
*  None
* 
*/
static void sbl1_update_partition_ids(bl_shared_data_type *bl_shared_data)
{
 if(sbl1_hw_get_au_power_status())
 {
  BL_VERIFY( bl_shared_data != NULL, BL_ERR_NULL_PTR_PASSED );

  /* If boot from backup partition is set, then update partition names */
  if ((boot_check_backup_partition_cookie() == PARTITION_B))
  {

    qmemcpy(qsee_partition_id, qsee_recovery_partition_id, PARTITION_ID_MAX_SIZE);
    qmemcpy(qhee_partition_id, qhee_recovery_partition_id, PARTITION_ID_MAX_SIZE);
    qmemcpy(qsee_devcfg_image_partition_id, qsee_devcfg_recovery_partition_id, PARTITION_ID_MAX_SIZE);
    qmemcpy(aop_partition_id, aop_recovery_partition_id, PARTITION_ID_MAX_SIZE);
    qmemcpy(XBLConfig_partition_id, XBLConfig_recovery_partition_id, PARTITION_ID_MAX_SIZE);

    boot_log_message("Using boot chain B");
  } 
  else
  {
    boot_log_message("Using boot chain A");
  }
 } 
}




/* Initialize XBL config Lib.
  Note:  
  Dependencies: XBL config needs to be initialized after boot flash and heap
  initializations.
*/
static void sbl1_xblconfig_init(bl_shared_data_type *bl_shared_data )
{
  xblconfig_status_type status;
  boot_log_message("xblconfig_init, Start");
  boot_log_start_timer();
  status = xblconfig_init();
  BL_VERIFY(status == XBLCONFIG_SUCCESS, 
            BL_ERR_IMG_NOT_FOUND|BL_ERROR_GROUP_BOOT); 
  boot_log_stop_timer("xblconfig_init, Delta");
}

/*==========================================================================
              FUNCTIONS TO EXECUTE BEFORE APDP IMAGE LOADING
===========================================================================*/
boot_procedure_func_type apdp_pre_procs[] = 
{
  /* Save reset register logs */
  boot_save_reset_register_log,
  
  /* Initialize the flash device */
  boot_flash_init,


  sbl1_update_partition_ids,

  /* Initialize XBL config Lib */
  sbl1_xblconfig_init,
  
  /* Initialize the default CDT before reading CDT from flash */
  boot_config_data_table_default_init, 

  /* Copy the configure data table from flash */
  boot_config_data_table_init,
  
  /* Store platform id */
  sbl1_hw_platform_pre_ddr,
  
  /* Initialize PMIC and railway driver */
  sbl1_hw_pre_ddr_init,

  /* Check if forced dload timeout reset cookie is set */
  boot_dload_handle_forced_dload_timeout,

  /* Configure ddr parameters based on eeprom CDT table data. */
  sbl1_ddr_set_params,
  
  /* Initialize DDR */
  (boot_procedure_func_type)sbl1_ddr_init, 
  
  /* Train DDR if applicable */
  sbl1_do_ddr_training,

  /*----------------------------------------------------------------------
   Run deviceprogrammer if compiling the deviceprogrammer_ddr image.
   In XBL builds the function below is stubbed out (does nothing)
  ----------------------------------------------------------------------*/
  sbl1_hand_control_to_devprog_ddr,
  
#ifndef FEATURE_DEVICEPROGRAMMER_IMAGE

  /* Run DDI testing if tests exist */
  boot_ddi_entry,
  
  /* Initialize SBL1 DDR ZI region, relocate boot log to DDR */   
  sbl1_post_ddr_init,
  
  sbl1_hw_init_secondary, 
  
#endif /*FEATURE_DEVICEPROGRAMMER_IMAGE*/

  /* Last entry in the table. */
  NULL 
};


/*==========================================================================
   FUNCTIONS TO EXECUTE AFTER APDP LOADING
===========================================================================*/
boot_procedure_func_type apdp_post_procs[] = 
{
#ifndef FEATURE_DEVICEPROGRAMMER_IMAGE

/* Backup QSEE and QHEE region for ramdumps */  
  boot_dload_dump_security_regions,

  /* Check to see if DLOAD mode needs to be entered */
  boot_dload_check,

  /* Set the memory barrier pointer to shared memory */
  boot_cache_set_memory_barrier,

  /*----------------------------------------------------------------------
   Put SMEM in debug state such that smem_alloc() calls will return NULL. 
   The state is changed back to normal once smem_boot_init() is called.
   This call has to be made after setting the memory barrier.
  ----------------------------------------------------------------------*/
  boot_smem_debug_init,  
    
  /* Initialize shared memory after dload to preserve logs */
  boot_smem_init,

  /* Initialize SMEM minidump table */
  boot_smem_alloc_for_minidump,

 #ifndef FEATURE_RUMI_BOOT
  /* Stub out for rumi build. pmic api  pm_get_power_on_status gets 
     called from below api to get power on reason */ 
  /*----------------------------------------------------------------------
   Store Power on Status in SMEM. 
   Needs to be done after PMIC and SMEM initialization
  ----------------------------------------------------------------------*/
  boot_smem_store_pon_status,
#endif  

  /*----------------------------------------------------------------------
   Store the platform id to smem
  ----------------------------------------------------------------------*/
  sbl1_hw_platform_smem,

  (boot_procedure_func_type) boot_ddr_share_data_to_aop,

  (boot_procedure_func_type) boot_clock_init_rpm,

  boot_vsense_copy_to_smem,

  /*----------------------------------------------------------------------
   Get shared data out of the flash device module
  ----------------------------------------------------------------------*/
  boot_share_flash_data,
  
  /*----------------------------------------------------------------------
   populate the ram partition table
  ----------------------------------------------------------------------*/
  boot_populate_ram_partition_table,

  /*populate ddr details shared memory*/
  boot_populate_ddr_details_shared_table,


  /*----------------------------------------------------------------------
   Initialize GPIO for low power configuration
  ----------------------------------------------------------------------*/
  sbl1_tlmm_init,
  
  /*-----------------------------------------------------------------------
   Calls efs cookie handling api to perform efs backup/restore
  -----------------------------------------------------------------------*/  
  sbl1_efs_handle_cookies,
    
  /*-----------------------------------------------------------------------
   APT Security Test
   ----------------------------------------------------------------------*/
  (boot_procedure_func_type)boot_apt_test,

#endif /* FEATURE_DEVICEPROGRAMMER_IMAGE */

  /* Last entry in the table. */
  NULL 
};

/*==========================================================================
                  FUNCTIONS TO EXECUTE BEFORE AOP LOADING
===========================================================================*/
boot_procedure_func_type aop_pre_procs[] = 
{  
  /* do pmic pre-aop init */
  sbl1_pm_aop_pre_init_wrapper,

  /* Last entry in the table */ 
  NULL 
};

/*==========================================================================
   FUNCTIONS TO EXECUTE AFTER QSEE LOADING
===========================================================================*/
boot_procedure_func_type qsee_post_procs[] = 
{
  /* Play power on vibration */
  sbl1_hw_play_vibr,

 /* Last entry in the table. */
  NULL 
};

/*==========================================================================
        List of SBL1 procedures to execute prior to dload entry
===========================================================================*/
boot_procedure_func_type sbl1_pre_dload_procs[] =
{
  /*-----------------------------------------------------------------------
   * Initialization functions for dload. This has to be the first function 
   * called before Dload entry
   *----------------------------------------------------------------------*/
  boot_init_for_dload,
  
  /*-----------------------------------------------------------------------
   * Setup clocks for ram dump
   *----------------------------------------------------------------------*/
  sbl1_hw_dload_init,
  
  /*-----------------------------------------------------------------------
   * NULL pointer indicates the end of the list
   *-----------------------------------------------------------------------*/
  NULL
};


/*==========================================================================
        List of SBL1 procedures to execute after APPSBL is loaded
===========================================================================*/
boot_procedure_func_type appsbl_post_procs[] =
{
  /*-----------------------------------------------------------------------
   * Determine APPSBL architecture and update SBL - QSEE interface
   *----------------------------------------------------------------------*/
  sbl1_appsbl_arch_determination,
  
  /*-----------------------------------------------------------------------
   * NULL pointer indicates the end of the list
   *-----------------------------------------------------------------------*/
  NULL
};

boot_boolean appsbl_check_uefi_loading()
{
  xblconfig_status_type xblcfg_err_flag = XBLCONFIG_SUCCESS;
  uint32 xbl_cfg_h = 0;
  uint32 xbl_cfg_size = 0;
  uint32 uefi_load_config[4] = {0,0,0,0};  // uefi load config is only 4Byte to get if UEFI needs to be loaded or not
  static boot_boolean uefi_loading_check_done = FALSE;
  
  if (uefi_loading_check_done == FALSE)
  {
    xblcfg_err_flag = xblconfig_open("/uefi_load_config.bin", &xbl_cfg_h, &xbl_cfg_size);
    if(xblcfg_err_flag == XBLCONFIG_SUCCESS)
    {
	    xblconfig_read(xbl_cfg_h, 0, xbl_cfg_size, (void*)&uefi_load_config);
	    xblconfig_close(xbl_cfg_h);
	    uefi_loading_check_done = TRUE;
    }
  }
  return uefi_loading_check_done;
}

/*==========================================================================
                     CONFIG FUNCTION DEFINITIONS 
===========================================================================*/

/* Executing QSEE from SBL1 */ 
static void qsee_jump_func(bl_shared_data_type *bl_shared_data )
{

  sys_debug_qsee_info_type * qsee_info =
    (sys_debug_qsee_info_type *)&Image$$SYSTEM_DEBUG_QSEE_INFO$$Base;
  xblconfig_status_type status;


  qsee_info->entry_point = (uint64)NULL;
  qsee_info->elf_class = (uint32)NULL;

  /* Flush the cache before entering QSEE */
  dcache_flush_region((void *)SCL_SBL1_IMAGE_BASE, SCL_SBL1_IMAGE_SIZE);  
  dcache_flush_region((void *)SCL_SBL1_DDR_DSF_ROM_BASE, SCL_SBL1_DDR_DSF_SEGMENT_SIZE);  
  dcache_flush_region((void *)SCL_SBL1_OCIMEM_DATA_BASE, SCL_SBL1_OCIMEM_DATA_SIZE);  
  dcache_flush_region((void *)SCL_SBL1_DDR_BASE, SCL_SBL1_DDR_SIZE);  

  boot_pm_vib_off();
  
  /* De-Initialize the boot logger */
  sbl1_boot_logger_deinit();
  
  /*Deinitialize XBL Config Lib*/
  status = xblconfig_deinit();
  BL_VERIFY(status == XBLCONFIG_SUCCESS, 
            BL_ERR_IMG_NOT_FOUND|BL_ERROR_GROUP_BOOT); 
  
  /* Search image entries in QSEE interface for QSEE and get entry point.
     The entry point and elf type of QSEE must be stored so if an abnormal
     reset occurs the system debug image knows how to get to QSEE.  A small
     section has been carved out in the system debug image memory for this
     data. 
  */
  qsee_info->entry_point =
    boot_sbl_qsee_interface_get_entry_point(&(bl_shared_data->sbl_qsee_interface),
                                            SECBOOT_QSEE_SW_TYPE);
  qsee_info->elf_class =
    boot_sbl_qsee_interface_get_eident(&(bl_shared_data->sbl_qsee_interface),
                                       SECBOOT_QSEE_SW_TYPE);

  /* Verify entry point is not null */
  BL_VERIFY(qsee_info->entry_point != (uint64)NULL,
            BL_ERR_NULL_PTR_PASSED|BL_ERROR_GROUP_BOOT);
  
  /* Verify QSEE ELF class is valid */
  BL_VERIFY((qsee_info->elf_class == ELF_CLASS_64) ||
            (qsee_info->elf_class == ELF_CLASS_32),
            BL_ERR_ELF_CLASS_INVALID|BL_ERROR_GROUP_BOOT);


  /* Store the index of appsbl image entry in sbl qsee interface for TZ to fetch
    the appsbl entry point. Needed as image ID can no longer be used to identify
    appsbl image.
  */
  (bl_shared_data->sbl_qsee_interface).appsbl_entry_index = 
                      sbl1_config_table[appsbl_index].sbl_qsee_interface_index ;
                      
  qmemcpy(&sbl_qsee_interface_ddr, &(bl_shared_data->sbl_qsee_interface),
            sizeof(boot_sbl_qsee_interface));
   
  dcache_flush_region((void *)&sbl_qsee_interface_ddr, sizeof(boot_sbl_qsee_interface));            
   
  /* Deinit hardware.  This must be done AFTER sbl1_boot_logger_deinit() 
    This is also used to turn off VMEM memory clocks. So ensure the api code/buffers
    are not in memory if it is being used.
  */  
  sbl1_hw_deinit();
  
 /* clearing the continuous boot failure count which is stored in IMEM */
  if (boot_shared_imem_cookie_ptr != NULL)
  {
	boot_shared_imem_cookie_ptr->boot_fail_count = 0;
  }
  
  /* "Exit" bootloader and enter QSEE context.
    */
    
  if (sbl1_get_current_el() == EL_1)
  {
     boot_fastcall_tz_no_rsp(TZ_SBL_END_MILESTONE,
                             TZ_SBL_END_MILESTONE_PARAM_ID,
                             (uint32)(&sbl_qsee_interface_ddr),
                             (uint32)(sizeof(sbl_qsee_interface_ddr)),
                             0,0);
  }

  /*Must not reach here*/
  while(1);
}


/* Executing STI from SBL1 */ 
static void sti_jump_func(bl_shared_data_type *bl_shared_data )
{
  boot_boolean result = FALSE;
  struct mem_block sti_mem_block;

  sys_debug_qsee_info_type * sti_info =
    (sys_debug_qsee_info_type *)&Image$$SYSTEM_DEBUG_QSEE_INFO$$Base;

  /*  STI is apt's test image to verify loader. If STI image is loaded
    it is the final image that loader jumps to instead of QSEE image.
    The image is like of extention of xbl loader so the deinit api's
    need not be called.
  */  

  sti_info->entry_point = (uint64)NULL;
  sti_info->elf_class = (uint32)NULL;

  /* Flush the cache before entering STI */
  dcache_flush_region((void *)SCL_SBL1_IMAGE_BASE, SCL_SBL1_IMAGE_SIZE);  
  dcache_flush_region((void *)SCL_SBL1_DDR_DSF_ROM_BASE, SCL_SBL1_DDR_DSF_SEGMENT_SIZE);  
  dcache_flush_region((void *)SCL_SBL1_OCIMEM_DATA_BASE, SCL_SBL1_OCIMEM_DATA_SIZE);  
  dcache_flush_region((void *)SCL_SBL1_DDR_BASE, SCL_SBL1_DDR_SIZE);    
  
  /* De-Initialize the boot logger */
  sbl1_boot_logger_deinit();
  
  /* Search image entries in QSEE interface for STI and get entry point.
  */
  sti_info->entry_point =
    boot_sbl_qsee_interface_get_entry_point(&(bl_shared_data->sbl_qsee_interface),
                                            SECBOOT_WDT_SW_TYPE);
  sti_info->elf_class =
    boot_sbl_qsee_interface_get_eident(&(bl_shared_data->sbl_qsee_interface),
                                       SECBOOT_WDT_SW_TYPE);

  /* Verify entry point is not null */
  BL_VERIFY(sti_info->entry_point != (uint64)NULL,
            BL_ERR_NULL_PTR_PASSED|BL_ERROR_GROUP_BOOT);
  
  /* Verify STI ELF class is valid */
  BL_VERIFY((sti_info->elf_class == ELF_CLASS_64) ||
            (sti_info->elf_class == ELF_CLASS_32),
            BL_ERR_ELF_CLASS_INVALID|BL_ERROR_GROUP_BOOT);

  /* Flush the cache before entering STI */
  dcache_flush_region((void *)SCL_XBL_CORE_CODE_BASE, SCL_XBL_CORE_TOTAL_SIZE);
  dcache_flush_region((void *)sti_info, sizeof(sys_debug_qsee_info_type));

   /* Mark STI page table as executable */
	sti_mem_block.p_base = SCL_LOADERTEST_CODE_BASE;
	sti_mem_block.v_base = SCL_LOADERTEST_CODE_BASE;
	sti_mem_block.size_in_kbytes = SCL_LOADERTEST_CODE_SIZE >> 10;
	sti_mem_block.memory_mapping = MMU_L2_SECTION_MAPPING;
	sti_mem_block.access = MMU_PAGETABLE_MEM_READ_WRITE;
	sti_mem_block.cachetype = MMU_PAGETABLE_DEVICE_SHARED;
	sti_mem_block.executable = MMU_PAGETABLE_EXECUTABLE_REGION;

  result =
    boot_mmu_page_table_map_single_mem_block((uint64 *)(uintnt)bl_shared_data->sbl_shared_data->mmu_l1_pagetable_base,
                                             &sti_mem_block);
  BL_VERIFY(result, BL_ERR_MMU_PGTBL_MAPPING_FAIL|BL_ERROR_GROUP_BOOT);

  mmu_invalidate_tlb();

  boot_log_message("STI Image Entry");

  /* "Exit" bootloader and enter STI context.
     Note: This function should not return.
  */
   apt_sti_init(sti_info->entry_point, bl_shared_data);
  
/*Must not reach here*/
 while(1);
}


/* Conditionally cancel APPSBL loading in SBL1 */
static boot_boolean appsbl_load_cancel(bl_shared_data_type *bl_shared_data)
{
  boot_boolean cancel = FALSE;
  uint32 index = 0;

#ifndef FEATURE_DEVICEPROGRAMMER_IMAGE
  /* Check for QNX config, if QNX target than no need to load UEFI image */
  if (appsbl_check_uefi_loading())
  {
    boot_log_message("Skip UEFI loading");
    qsee_jump_func(bl_shared_data);  // As Uefi is not loaded, directely call qsee jump function to start TZ
    while (1);  /*Must not reach here*/
    return TRUE;
  }
 #endif 
  
 /*
   Note:
   appsbl_load_cancel behaves a bit different than other load_cancel api's.
   The api modifies parameters to switch to loading XBL Core segments
   using XBL elf info passed by bootrom.  
 */
  
  do
  {
    /* Iterate over config table to find APPSBL entry */
    while(sbl1_config_table[index].host_img_id != (uintnt)NULL)
    {
       if(sbl1_config_table[index].target_img_sec_type 
          == SECBOOT_APPSBL_SW_TYPE)
       {
         break;
       }
       index++;
    }
    
    /* Verify Appsbl entry is found */
    BL_VERIFY(sbl1_config_table[index].host_img_id != (uint64)NULL,
            BL_ERR_NULL_PTR_PASSED|BL_ERROR_GROUP_BOOT);

    /* Store the index for future use */
    appsbl_index = index;  
  
    /*Update APPSBL entry to enable loading XBL core segments*/
    sbl1_config_table[index].target_img_type = CONFIG_IMG_ELF_SEG;
    sbl1_config_table[index].target_img_sec_type = SECBOOT_SBL_SW_TYPE;
    boot_elf_ziinit_structures(NULL);
	
    pbl_elf_img_share_info_type *elf_img_share_info_p = NULL;
    sec_img_auth_elf_info_type *elf_p = NULL;

    elf_img_share_info_p = (pbl_elf_img_share_info_type*)bl_shared_data->sbl_shared_data->pbl_shared_data->elf_img_shared_info;
    elf_p = (sec_img_auth_elf_info_type*)elf_img_share_info_p->elf_img_meta_info;

    boot_set_elf_and_prog_header( elf_p->elf_hdr, elf_p->prog_hdr);
    boot_set_hash_segment_buffer((void *)(elf_p->hash_seg_hdr));

    /* Configure the target image using custom partition ID information */ 
    boot_flash_configure_target_image(sbl1_partition_id);
  
  }while(0);
  
  return cancel;
}

/* Conditionally cancel ABL loading in XBL */
static boot_boolean abl_load_cancel(bl_shared_data_type *bl_shared_data)
{  
  boot_flash_trans_if_type *trans_if;    
  boot_boolean cancel = TRUE;
  uint32 index = 0;
  mi_boot_image_header_type abl_header;
  
  do
  {
    /* Iterate over config table to find ABL entry */
    while(sbl1_config_table[index].host_img_id!= (uintnt)NULL)
    {
       if(sbl1_config_table[index].target_img_sec_type 
          == SECBOOT_ABL_SW_TYPE)
       {
         break;
       }
       index++;
    }

    /* Do not load ABL if image not found in the config table */
    if(sbl1_config_table[index].host_img_id == (uintnt)NULL)
    {
       break;
    }

    /* Load only if ABL partition exists */
    boot_flash_configure_target_image(abl_partition_id);
    trans_if = boot_flash_dev_open_image(GEN_IMG);
    if(trans_if == NULL )
    {
       break;
    }
    boot_flash_dev_close_image( &trans_if );

	/* Validate the ELF header and load the image only if the header is valid */
	if (!boot_elf_verify_image_header(abl_partition_id, &abl_header))
	{
		/* Partition is not flashed */
		break;
	}      
    
    /* All conditions are satisfied, load ABL image */
    cancel = FALSE;
  } while(0);

  return cancel;
}

/* Conditionally cancel AOP Image loading in SBL1 (ie XBL Loader) */
static boot_boolean aop_load_cancel(bl_shared_data_type *bl_shared_data)
{
  DalPlatformInfoPlatformType platform;
  boot_boolean status = FALSE;

  platform = boot_DalPlatformInfo_Platform();

  if((boot_dload_is_dload_mode_set() == TRUE) ||
     (platform == DALPLATFORMINFO_TYPE_VIRTIO)
    )
  {
    status = TRUE;
  }
    /* Cancel loading if XBL loader is in dload mode */
  return status;
}

/* Conditionally cancel APDP loading in SBL1 */
static boot_boolean apdp_load_cancel(bl_shared_data_type *bl_shared_data)
{  
  boot_flash_trans_if_type *trans_if;    
  boot_boolean cancel = TRUE;
  uint32 index = 0;
  mi_boot_image_header_type apdp_header;
  
  do
  {
    /* Iterate over config table to find APDP entry */
    while(sbl1_config_table[index].host_img_id!= (uintnt)NULL)
    {
       if(sbl1_config_table[index].target_img_sec_type 
          == SECBOOT_APDP_SW_TYPE)
       {
         break;
       }
       index++;
    }

    /* Do not load APDP if image not found in the config table */
    if(sbl1_config_table[index].host_img_id == (uintnt)NULL)
    {
       break;
    }

    /* Load only if APDP partition exists */
    boot_flash_configure_target_image(apdp_partition_id);
    trans_if = boot_flash_dev_open_image(GEN_IMG);
    if(trans_if == NULL )
    {
       break;
    }
    boot_flash_dev_close_image( &trans_if );

    /* Validate the ELF header and load the APDP image only if the 
      header is valid 
    */
    if (!boot_elf_verify_image_header(apdp_partition_id, &apdp_header))
    {
      /* Partition is not flashed */
      break;
    }
    
    /* All conditions are satisfied, load APDP image */
    cancel = FALSE;
  } while(0);

  return cancel;
}

/* Conditionally cancel Qsee enumeration/ Dev config image loading in SBL1 */
static boot_boolean qsee_devcfg_load_cancel(bl_shared_data_type *bl_shared_data)
{
  boot_flash_trans_if_type *trans_if; 
  boot_boolean cancel = TRUE;

  do
  {
    /* Load only if Qsee enumeration/ dev config image partition exists */
    boot_flash_configure_target_image(qsee_devcfg_image_partition_id);
    trans_if = boot_flash_dev_open_image(GEN_IMG);

    if(trans_if != NULL )
    {
      boot_flash_dev_close_image( &trans_if );
      cancel = FALSE;
      break;
    }
  }while(0);
  
  return cancel;
}

/* Conditionally cancel STI loading in SBL1 */
static boot_boolean sti_load_cancel(bl_shared_data_type *bl_shared_data)
{
  boot_flash_trans_if_type *trans_if; 
  mi_boot_image_header_type sti_header;  
  boot_boolean cancel = TRUE;
  boot_boolean is_auth_enabled = TRUE;
  secboot_hw_etype status = E_SECBOOT_HW_FAILURE;
   
 /*
   Note:
   STI (APT's loader test image) to be loaded only if partition is present,
   image is flashed, authentication not enabled and debug disable fuses
   not blown   
 */
  
  do
  {
    /* Load only if STI partition exists */
    boot_flash_configure_target_image(sti_partition_id);
    trans_if = boot_flash_dev_open_image(GEN_IMG);
    if(trans_if == NULL )
    {
       break;
    }
    boot_flash_dev_close_image( &trans_if );

    /* Validate the ELF header and load the STI image only if the 
      header is valid 
    */
    if (!boot_elf_verify_image_header(sti_partition_id, &sti_header))
    {
      /* Partition is not flashed */
      break;
    }
    
    /* Do not load STI image if authentication is enabled or
      debug disable fuses are blown.
    */
#ifdef ENABLE_FOR_VERIFICATION    
    if(DEBUG_DISABLED_APPS)
    {
      break;
    }
#endif
    status = boot_is_auth_enabled(&is_auth_enabled);
    BL_VERIFY((status == E_SECBOOT_HW_SUCCESS), (uint16)status|BL_ERROR_GROUP_SECURITY); 

    if(is_auth_enabled == TRUE)
    {
      break;
    }
    
    /* All conditions are satisfied, load STI image */
    cancel = FALSE;
  
  }while(0);
  
  return cancel;
}

/* Conditionally cancel OEM_MISC loading in SBL1 */
static boot_boolean oem_misc_load_cancel(bl_shared_data_type *bl_shared_data)
{  
  boot_flash_trans_if_type *trans_if;    
  boot_boolean cancel = TRUE;
  uint32 index = 0;
  mi_boot_image_header_type oem_misc_header;
  
  do
  {
    /* Iterate over config table to find OEM_MISC entry */
    while(sbl1_config_table[index].host_img_id!= (uintnt)NULL)
    {
       if(sbl1_config_table[index].target_img_sec_type 
          == SECBOOT_OEM_MISC_SW_TYPE)
       {
         break;
       }
       index++;
    }

    /* Do not load OEM_MISC if image not found in the config table */
    if(sbl1_config_table[index].host_img_id == (uintnt)NULL)
    {
       break;
    }

    /* Load only if OEM_MISC partition exists */
    boot_flash_configure_target_image(multi_image_partition_id);
    trans_if = boot_flash_dev_open_image(GEN_IMG);
    if(trans_if == NULL )
    {
       break;
    }
    boot_flash_dev_close_image( &trans_if );

    /* Validate the ELF header and load the OEM_MISC image only if the 
      header is valid 
    */
    if (!boot_elf_verify_image_header(multi_image_partition_id, &oem_misc_header))
    {
      /* Partition is not flashed */
      break;
    }
    
    /* All conditions are satisfied, load OEM_MISC image */
    cancel = FALSE;
  } while(0);

  return cancel;
}

/* Conditionally cancel QTI_MISC loading in SBL1 */
static boot_boolean qti_misc_load_cancel(bl_shared_data_type *bl_shared_data)
{  
  boot_flash_trans_if_type *trans_if;    
  boot_boolean cancel = TRUE;
  uint32 index = 0;
  mi_boot_image_header_type qti_misc_header;
  
  do
  {
    /* Iterate over config table to find QTI_MISC entry */
    while(sbl1_config_table[index].host_img_id!= (uintnt)NULL)
    {
       if(sbl1_config_table[index].target_img_sec_type 
          == SECBOOT_QTI_MISC_SW_TYPE)
       {
         break;
       }
       index++;
    }

    /* Do not load QTI_MISC if image not found in the config table */
    if(sbl1_config_table[index].host_img_id == (uintnt)NULL)
    {
       break;
    }

    /* Load only if QTI_MISC partition exists */
    boot_flash_configure_target_image(multi_image_qti_partition_id);
    trans_if = boot_flash_dev_open_image(GEN_IMG);
    if(trans_if == NULL )
    {
       break;
    }
    boot_flash_dev_close_image( &trans_if );

    /* Validate the ELF header and load the QTI_MISC image only if the 
      header is valid 
    */
    if (!boot_elf_verify_image_header(multi_image_qti_partition_id, &qti_misc_header))
    {
      /* Partition is not flashed */
      break;
    }
    
    /* All conditions are satisfied, load QTI_MISC image */
    cancel = FALSE;
  } while(0);

  return cancel;
}


/* Conditionally cancel SEC image loading in SBL1 */
static boot_boolean sec_load_cancel(bl_shared_data_type *bl_shared_data)
{  
  boot_flash_trans_if_type *trans_if;    
  boot_boolean cancel = TRUE;
  uint32 index = 0;
  mi_boot_image_header_type sec_header;
  
  do
  {
    /* Iterate over config table to find SEC entry */
    while(sbl1_config_table[index].host_img_id!= (uintnt)NULL)
    {
       if(sbl1_config_table[index].target_img_sec_type 
          == SECBOOT_SEC_SW_TYPE)
       {
         break;
       }
       index++;
    }

    /* Do not load SEC if image not found in the config table */
    if(sbl1_config_table[index].host_img_id == (uintnt)NULL)
    {
       break;
    }

    /* Load only if SEC partition exists */
    boot_flash_configure_target_image(secdata_partition_id);
    trans_if = boot_flash_dev_open_image(GEN_IMG);
    if(trans_if == NULL )
    {
       break;
    }
    boot_flash_dev_close_image( &trans_if );

    /* Validate the ELF header and load the SEC image only if the 
      header is valid 
    */
    if (!boot_elf_verify_image_header(secdata_partition_id, &sec_header))
    {
      /* Partition is not flashed */
      break;
    }
    
    /* All conditions are satisfied, load SEC image */
    cancel = FALSE;
  } while(0);

  return cancel;
}

/*==========================================================================
                      DEFINE TARGET BOOT CONFIG TABLE
===========================================================================*/
boot_configuration_table_entry sbl1_config_table[] = 
{
/* host_img_id host_img_type target_img_id target_img_type target_img_sec_type        load   auth   exec   jump   exec_func jump_func   pre_procs       post_procs         load_cancel              target_img_partition_id         target_img_str            boot_ssa_enabled enable_xpu xpu_proc_id sbl_qsee_interface_index seg_elf_entry_point whitelist_ptr */
  {SBL1_IMG, CONFIG_IMG_QC, GEN_IMG, CONFIG_IMG_ELF,     SECBOOT_APDP_SW_TYPE,        TRUE,  TRUE,  FALSE, FALSE, NULL, NULL,           apdp_pre_procs, apdp_post_procs,   apdp_load_cancel,        apdp_partition_id,              APDP_BOOT_LOG_STR,        FALSE, FALSE, 0x0, 0x0, 0x0,                    apdp_img_whitelist    },
  {SBL1_IMG, CONFIG_IMG_QC, GEN_IMG, CONFIG_IMG_ELF,     SECBOOT_OEM_MISC_SW_TYPE,    TRUE,  TRUE,  FALSE, FALSE, NULL, NULL,           NULL,           NULL,              oem_misc_load_cancel,    multi_image_partition_id,       OEM_MISC_BOOT_LOG_STR,    FALSE, FALSE, 0x0, 0x0, 0x0,                    oem_misc_img_whitelist},
  {SBL1_IMG, CONFIG_IMG_QC, GEN_IMG, CONFIG_IMG_ELF,     SECBOOT_QTI_MISC_SW_TYPE,    TRUE,  TRUE,  FALSE, FALSE, NULL, NULL,           NULL,           NULL,              qti_misc_load_cancel,    multi_image_qti_partition_id,   QTI_MISC_BOOT_LOG_STR,    FALSE, FALSE, 0x0, 0x0, 0x0,                    qti_misc_img_whitelist},
  {SBL1_IMG, CONFIG_IMG_QC, GEN_IMG, CONFIG_IMG_ELF,     SECBOOT_AOP_SW_TYPE,         TRUE,  TRUE,  FALSE, FALSE, NULL, NULL,           aop_pre_procs,  NULL,              aop_load_cancel,         aop_partition_id,               AOP_BOOT_LOG_STR,         FALSE, FALSE, 0x0, 0x0, 0x0,                    aop_img_whitelist     },
  {SBL1_IMG, CONFIG_IMG_QC, GEN_IMG, CONFIG_IMG_ELF,     SECBOOT_QSEE_DEVCFG_SW_TYPE, TRUE,  TRUE,  FALSE, FALSE, NULL, NULL,           NULL,           NULL,              qsee_devcfg_load_cancel, qsee_devcfg_image_partition_id, QSEE_DEVCFG_BOOT_LOG_STR, FALSE, FALSE, 0x0, 0x0, 0x0,                    devcfg_img_whitelist  },
  {SBL1_IMG, CONFIG_IMG_QC, GEN_IMG, CONFIG_IMG_ELF,     SECBOOT_QSEE_SW_TYPE,        TRUE,  TRUE,  FALSE, FALSE, NULL, NULL,           NULL,           qsee_post_procs,   NULL,                    qsee_partition_id,              QSEE_BOOT_LOG_STR,        FALSE, FALSE, 0x0, 0x0, 0x0,                    qsee_img_whitelist    },
  {SBL1_IMG, CONFIG_IMG_QC, GEN_IMG, CONFIG_IMG_ELF,     SECBOOT_SEC_SW_TYPE,         TRUE,  TRUE,  FALSE, FALSE, NULL, NULL,           NULL,           NULL,              sec_load_cancel,         secdata_partition_id,           SEC_BOOT_LOG_STR,         FALSE, FALSE, 0x0, 0x0, 0x0,                    sec_img_whitelist     },
  {SBL1_IMG, CONFIG_IMG_QC, GEN_IMG, CONFIG_IMG_ELF,     SECBOOT_QHEE_SW_TYPE,        TRUE,  TRUE,  FALSE, FALSE, NULL, NULL,           NULL,           NULL,              NULL,                    qhee_partition_id,              QHEE_BOOT_LOG_STR,        FALSE, FALSE, 0x0, 0x0, 0x0,                    qhee_img_whitelist    },
  {SBL1_IMG, CONFIG_IMG_QC, GEN_IMG, CONFIG_IMG_ELF,     SECBOOT_WDT_SW_TYPE,         TRUE,  TRUE,  FALSE, TRUE,  NULL, sti_jump_func,  NULL,           NULL,              sti_load_cancel,         sti_partition_id,               STI_BOOT_LOG_STR,         FALSE, FALSE, 0x0, 0x0, 0x0,                    sti_img_whitelist     },
  {SBL1_IMG, CONFIG_IMG_QC, GEN_IMG, CONFIG_IMG_ELF,     SECBOOT_APPSBL_SW_TYPE,      TRUE,  TRUE,  FALSE, TRUE,  NULL, qsee_jump_func, NULL,           appsbl_post_procs, appsbl_load_cancel,      appsbl_partition_id,            APPSBL_BOOT_LOG_STR,      FALSE, FALSE, 0x0, 0x0, SCL_XBL_CORE_CODE_BASE, xbl_core_img_whitelist},
  {NONE_IMG, }
};
