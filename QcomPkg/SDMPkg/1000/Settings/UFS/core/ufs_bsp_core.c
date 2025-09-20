/**********************************************************************
* ufs_bsp_core.c
*
* Board support file for XBL core
*
* Copyright (c) 2013-2019 Qualcomm Technologies, Inc.  All Rights Reserved.
* Qualcomm Technologies Proprietary and Confidential.
* 
*
**********************************************************************/

/*=======================================================================
                        Edit History


YYYY-MM-DD   who     what, where, why
----------------------------------------------------------------------
2019-09-24   jt      Add sleep voltage mode
2019-07-02   jt      Add ability to turn all regulators off 
2019-02-27   jt      Add UFS 3.0 support 
2018-11-29   jt      Add clock and voltage settings for secondary UFS
2018-09-24   jt      Move PHY settings to its own file 
2018-09-05   jt      Restructure PHY initialization 
2018-07-24   jt      HPG updates for Hana v2 
2018-06-21   jt      Only do cache management if CCA disabled
2018-06-04   jt      Add ability to turn clocks off/on
2018-05-30   jt      HPG v7.0 PHY updates for Hana/Poipu   
2018-05-14   jt      Add autoH8 function  
2018-02-13   jt      PHY init updates for Hana/Poipu 
2017-07-31   jt      Add log function 
2017-06-06   jt      Adapt for Hana
2017-04-08   jt      Turn on flags to skip actions in core  
2017-01-25   jt      Add UFS device reset 
2016-11-01   jt      Add reset function 
2016-10-21   jt      Enable HS for RUMI
2016-09-14   jt      AANG RUMI PHY init updates  
2016-08-22   jt      PHY init updates from 8998v2 
2016-07-22   jt      Adapted for Napali 
2015-06-15   rh      Disable the PHY from decoding the LCC sequence
2014-09-17   rh      Adapted for 8996 
2013-10-30   rh      Clock regime function cleanup
2013-10-30   rh      More accurate delay added
2013-06-19   rh      Initial creation
===========================================================================*/

#include <Uefi.h>
#include <HALhwio.h>
#include <Library/PcdLib.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/CacheMaintenanceLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UncachedMemoryAllocationLib.h>
#include <Library/SynchronizationLib.h>
#include <Library/TimerLib.h>
#include <Library/UefiCfgLib.h>
#include <Protocol/EFIHWIO.h>
#include <Protocol/EFIClock.h>
#include <Protocol/LoadedImage.h> 
#include <Protocol/EFIPlatformInfo.h> 
#include <Protocol/EFIHALIOMMUProtocol.h>

#include <Library/PcdLib.h>
#include <Library/ArmLib.h>

#include "DDIPlatformInfo.h"
#include "../inc/ufs_osal.h"
#include "../inc/ufs_bsp.h"
#include "../inc/ufs_phy_hwio_v2.h"
#include "../inc/ufs_phy_sequence.h"
#include "../inc/hwio_tlmm_ufs.h"
#include "../inc/aang_phy_hwio.h"
#include "../inc/hwio_ufs_base.h"
#include "HALbootHWIO.h"

#include "npa.h"
#include "pmapp_npa.h"

#define UFS_PHY_PCS_READY_TIMEOUT      0xffffff

EFI_CLOCK_PROTOCOL *hUFS_Clock = NULL;
DalPlatformInfoPlatformType g_platform;
enum hal_iommu_bypass_domain_type cca_flag = HAL_IOMMU_DOMAIN_TYPE_INVALID;

static UFS_CLOCK_LIST ufs_clocks_array[] = 
{
   {
      {"gcc_ufs_phy_axi_clk", 0, 0}, 
      {"gcc_ufs_phy_ice_core_clk", 0, 0}, 
      {"gcc_ufs_phy_unipro_core_clk", 0, 0},
      {"gcc_aggre_ufs_phy_axi_clk", 0, 0}, 
      {"gcc_ufs_phy_ahb_clk", 0, 0}, 
      {"gcc_ufs_phy_phy_aux_clk", 0, 0}, 
      {"gcc_ufs_phy_tx_symbol_0_clk", 0, 0},
      {"gcc_ufs_phy_rx_symbol_0_clk", 0, 0},
      {"gcc_ufs_phy_rx_symbol_1_clk", 0, 0}
   }, 
   {
      {"gcc_ufs_card_2_axi_clk", 0, 300000000}, 
      {"gcc_ufs_card_2_ice_core_clk", 0, 300000000}, 
      {"gcc_ufs_card_2_unipro_core_clk", 0, 300000000},
      {"gcc_aggre_ufs_card_2_axi_clk", 0, 300000000}, 
      {"gcc_ufs_card_2_ahb_clk", 0, 0}, 
      {"gcc_ufs_card_2_phy_aux_clk", 0, 0}, 
      {"gcc_ufs_card_2_tx_symbol_0_clk", 0, 0},
      {"gcc_ufs_card_2_rx_symbol_0_clk", 0, 0},
      {"gcc_ufs_card_2_rx_symbol_1_clk", 0, 0}
   }
};

void ufs_bsp_clk_set (uint8_t hostid, uint32_t mode)
{
   EFI_STATUS eResult = EFI_SUCCESS;
   UINT8 i = 0; 
   UINT8 num_clocks = sizeof(UFS_CLOCK_LIST) / sizeof(UFS_CLOCK_INFO);
   UFS_CLOCK_INFO *cur_clock = NULL; 
   UFS_CLOCK_INFO *clock_list = NULL; 

   if (NULL == hUFS_Clock)
   {
      eResult = gBS->LocateProtocol(&gEfiClockProtocolGuid, NULL, (VOID **)&hUFS_Clock);
      if ((EFI_SUCCESS != eResult) || (NULL == hUFS_Clock))
      {
          hUFS_Clock = NULL;
          ASSERT_EFI_ERROR(eResult);
      }
   }

   if (NULL != hUFS_Clock) 
   {
      clock_list = (UFS_CLOCK_INFO *)&ufs_clocks_array[hostid]; 
      for (i = 0; i < num_clocks; i++) 
      {
         cur_clock = clock_list + i; 
         if (cur_clock->clock_id == 0) 
         {
            eResult = hUFS_Clock->GetClockID(hUFS_Clock, cur_clock->clock_name,
                                             &cur_clock->clock_id);
            if (EFI_SUCCESS != eResult)
            {
               ASSERT_EFI_ERROR(eResult);
            }
         }

         if (UFS_CLOCK_MODE_OFF == mode) 
         {
            eResult = hUFS_Clock->DisableClock(hUFS_Clock, cur_clock->clock_id);
            if (EFI_SUCCESS != eResult)
            {
               ASSERT_EFI_ERROR(eResult);
            }
         }
         else if (UFS_CLOCK_MODE_ON == mode)
         {
            eResult = hUFS_Clock->EnableClock(hUFS_Clock, cur_clock->clock_id);          
            if (EFI_SUCCESS != eResult)
            {
               ASSERT_EFI_ERROR(eResult);
            }

            if (cur_clock->freq != 0) 
            {
               eResult = hUFS_Clock->SetClockFreqHz(hUFS_Clock, cur_clock->clock_id, cur_clock->freq, 
                                                    EFI_CLOCK_FREQUENCY_HZ_AT_MOST, &cur_clock->freq);          
               if (EFI_SUCCESS != eResult)
               {
                  ASSERT_EFI_ERROR(eResult);
               }
            }
         }
      }
   }
}    

void ufs_bsp_cache_op (void *addr, uint32_t len, uint32_t op) 
{
   if (cca_flag == HAL_IOMMU_DOMAIN_TYPE_INVALID) {
      if (GetConfigValue ("EnableUfsIOC", &cca_flag) != EFI_SUCCESS)
      {
         cca_flag = HAL_IOMMU_AARCH64_NON_CCA_DOMAIN_TYPE;
      }
   }

   if (cca_flag != HAL_IOMMU_AARCH64_CCA_DOMAIN_TYPE) {
      if (op & FLUSH) {
         WriteBackInvalidateDataCacheRange (addr, len);
      }

      if (op & INVAL) {
         InvalidateDataCacheRange (addr, len);
      }

      if (op & CLEAN) {
         WriteBackDataCacheRange (addr, len);
      }
   }
}

void ufs_bsp_busy_wait(uint32_t us)
{
   gBS->Stall(us);
}

void ufs_bsp_memory_barrier ()
{
   ArmDataMemoryBarrier();
}

uint32_t ufs_bsp_get_mphy_init_item_table (struct ufs_mphy_init_item **itm)
{
   if (g_platform == DALPLATFORMINFO_TYPE_RUMI) {
      *itm = ufs_bsp_mphy_init_table_rumi;
      return sizeof(ufs_bsp_mphy_init_table_rumi)/sizeof(struct ufs_mphy_init_item);
   }
   else {
      *itm = ufs_bsp_mphy_init_table;
      return sizeof(ufs_bsp_mphy_init_table)/sizeof(struct ufs_mphy_init_item);
   }
}

uint32_t ufs_bsp_get_mphy_init_2_lane_item_table (struct ufs_mphy_init_item **itm)
{
   if (g_platform == DALPLATFORMINFO_TYPE_RUMI) {
      *itm = ufs_bsp_mphy_2_lane_init_table_rumi;
      return sizeof(ufs_bsp_mphy_2_lane_init_table_rumi)/sizeof(struct ufs_mphy_init_item);
   }
   else {
      *itm = ufs_bsp_mphy_2_lane_init_table;
      return sizeof(ufs_bsp_mphy_2_lane_init_table)/sizeof(struct ufs_mphy_init_item);
   }
}

uint32_t ufs_bsp_get_mphy_init_end_item_table (struct ufs_mphy_init_item **itm)
{
   if (g_platform == DALPLATFORMINFO_TYPE_RUMI) {
      return 0; 
   }
   else {
      *itm = ufs_bsp_mphy_init_end_table;
      return sizeof(ufs_bsp_mphy_init_end_table)/sizeof(struct ufs_mphy_init_item);
   }
}

// Start the PHY and wait for the PHY to be ready 
ufs_bsp_err_type ufs_bsp_mphy_start (uint8_t hostid)
{
   int32_t tout = UFS_PHY_PCS_READY_TIMEOUT;
   uint32_t base_addr = ufs_bsp_get_reg_baseaddress(hostid); 

   if (g_platform == DALPLATFORMINFO_TYPE_RUMI) {
      out_dword(base_addr + HWIO_UFS_PHY_PHY_START_OFFS, 1);
      ufs_bsp_busy_wait (10);

      while (in_dword(base_addr + HWIO_QSERDES_COM_RESET_SM_OFFS) != 0x3d) {
         if (tout-- == 0) {
            return UFS_BSP_DEVICE_ERROR;
         }
         ufs_bsp_busy_wait (1);
      }
   }
   else
   {
      HWIO_REG_OUTM (base_addr, UFS_MEM_UFS_PHY_PHY_START_OFFS, 1, 1);
      ufs_bsp_busy_wait (10);

      while (!(HWIO_REG_IN(base_addr, UFS_MEM_UFS_PHY_PCS_READY_STATUS_OFFS) & 0x01)) {
         if (tout-- == 0) {
            return UFS_BSP_DEVICE_ERROR;
         }
         ufs_bsp_busy_wait (1);
      }
   }

   return UFS_BSP_NO_ERROR;
}

void ufs_bsp_reset (uint8_t hostid)
{
   if (g_platform == DALPLATFORMINFO_TYPE_RUMI) {

      // Reset and sleep 50 ms
      out_dword(HWIO_GE_DUT_RESET_REG_ADDR, 0x5f); 
      ufs_bsp_busy_wait (50000);

      // Release reset and sleep 50 ms
      out_dword(HWIO_GE_DUT_RESET_REG_ADDR, 0x0);
      ufs_bsp_busy_wait (50000);

      // Clock frequencies used
      out_dword(HWIO_GE_DUT_CLK_STATUS_SEL_REG_ADDR, 0x1);

      // Reset ICE fuses
      out_dword(HWIO_GE_DUT_UFS_ICE_CTRL_REG_ADDR, 0x0);
   }
   else
   {
      if (hostid == 0) {
         out_dword(HWIO_GCC_UFS_PHY_BCR_ADDR, 1);
         ufs_bsp_busy_wait(500);
         out_dword(HWIO_GCC_UFS_PHY_BCR_ADDR, 0); 
      }
      else if (hostid == 1) {
         out_dword(HWIO_GCC_UFS_CARD_2_BCR_ADDR, 1); 
         ufs_bsp_busy_wait(500); 
         out_dword(HWIO_GCC_UFS_CARD_2_BCR_ADDR, 0); 
      }
   }
}

void ufs_bsp_post_link_init (uint8_t hostid)
{
   if (g_platform == DALPLATFORMINFO_TYPE_RUMI) {
      return;
   }

   HWIO_REG_OUT(ufs_bsp_get_reg_baseaddress(hostid), UFS_MEM_UFS_PHY_LINECFG_DISABLE_OFFS, 0x02);   
}

void ufs_bsp_device_reset (uint8_t hostid)
{
   if (g_platform == DALPLATFORMINFO_TYPE_RUMI) {
      return;
   }

   if (hostid == 0) {
      out_dword(HWIO_TLMM_UFS_RESET_ADDR, 0); 
      ufs_bsp_busy_wait(10); 
      out_dword(HWIO_TLMM_UFS_RESET_ADDR, 1); 
      ufs_bsp_busy_wait(10); 
   }
   else if (hostid == 1) {
      out_dword(HWIO_TLMM_UFS0_RESET_ADDR, 0); 
      ufs_bsp_busy_wait(10); 
      out_dword(HWIO_TLMM_UFS0_RESET_ADDR, 1); 
      ufs_bsp_busy_wait(10); 
   }
}

void ufs_bsp_autoH8(uint8_t hostid)
{
   /* Enable HW clock control for UFS clocks */
   if (hostid == 0) {
      HWIO_GCC_AGGRE_UFS_PHY_AXI_CBCR_OUTM(HWIO_GCC_AGGRE_UFS_PHY_AXI_CBCR_HW_CTL_BMSK, 
                                           (0x1 << HWIO_GCC_AGGRE_UFS_PHY_AXI_CBCR_HW_CTL_SHFT));
      HWIO_GCC_UFS_PHY_AXI_CBCR_OUTM(HWIO_GCC_UFS_PHY_AXI_CBCR_HW_CTL_BMSK, 
                                     (0x1 << HWIO_GCC_UFS_PHY_AXI_CBCR_HW_CTL_SHFT));
      HWIO_GCC_UFS_PHY_UNIPRO_CORE_CBCR_OUTM(HWIO_GCC_UFS_PHY_UNIPRO_CORE_CBCR_HW_CTL_BMSK, 
                                             (0x1 << HWIO_GCC_UFS_PHY_UNIPRO_CORE_CBCR_HW_CTL_SHFT)); 
      HWIO_GCC_UFS_PHY_ICE_CORE_CBCR_OUTM(HWIO_GCC_UFS_PHY_ICE_CORE_CBCR_HW_CTL_BMSK, 
                                          (0x1 << HWIO_GCC_UFS_PHY_ICE_CORE_CBCR_HW_CTL_SHFT)); 
      HWIO_GCC_UFS_PHY_PHY_AUX_CBCR_OUTM(HWIO_GCC_UFS_PHY_PHY_AUX_CBCR_HW_CTL_BMSK, 
                                         (0x1 << HWIO_GCC_UFS_PHY_PHY_AUX_CBCR_HW_CTL_SHFT)); 
   }
   else if (hostid == 1) {
      HWIO_GCC_AGGRE_UFS_PHY_AXI_CBCR_OUTM(HWIO_GCC_AGGRE_UFS_CARD_2_AXI_CBCR_HW_CTL_BMSK, 
                                           (0x1 << HWIO_GCC_AGGRE_UFS_CARD_2_AXI_CBCR_HW_CTL_SHFT));
      HWIO_GCC_UFS_PHY_AXI_CBCR_OUTM(HWIO_GCC_UFS_CARD_2_AXI_CBCR_HW_CTL_BMSK, 
                                     (0x1 << HWIO_GCC_UFS_CARD_2_AXI_CBCR_HW_CTL_SHFT));
      HWIO_GCC_UFS_PHY_UNIPRO_CORE_CBCR_OUTM(HWIO_GCC_UFS_CARD_2_UNIPRO_CORE_CBCR_HW_CTL_BMSK, 
                                             (0x1 << HWIO_GCC_UFS_CARD_2_UNIPRO_CORE_CBCR_HW_CTL_SHFT)); 
      HWIO_GCC_UFS_PHY_ICE_CORE_CBCR_OUTM(HWIO_GCC_UFS_CARD_2_ICE_CORE_CBCR_HW_CTL_BMSK, 
                                          (0x1 << HWIO_GCC_UFS_CARD_2_ICE_CORE_CBCR_HW_CTL_SHFT)); 
      HWIO_GCC_UFS_PHY_PHY_AUX_CBCR_OUTM(HWIO_GCC_UFS_CARD_2_PHY_AUX_CBCR_HW_CTL_BMSK, 
                                         (0x1 << HWIO_GCC_UFS_CARD_2_PHY_AUX_CBCR_HW_CTL_SHFT)); 
   }

   /* Set AH8_CFG to enable the clock request feature */
   HWIO_REG_OUT(ufs_bsp_get_reg_baseaddress(hostid), UFS_MEM_AH8_CFG_OFFS, 0x3c);  

   /* Set AH8 after 1ms */
   HWIO_REG_OUT(ufs_bsp_get_reg_baseaddress(hostid), UFS_MEM_AHIT_OFFS, 0xc01); 
}

void *ufs_bsp_allocate_norm_buf (size_t size)
{
    VOID *mem;
    EFI_STATUS            status;
    status=gBS->AllocatePool(EfiBootServicesData,size,(VOID**)&mem);

    if (EFI_SUCCESS != status)
    {
        DEBUG((EFI_D_ERROR, "UFS: Allocate memory of size 0x%x     [FAILED]\n",size));
        return NULL;
    }
    return mem;
}

void  ufs_bsp_free_norm_buf (void *pmem)
{
    if ( pmem == NULL )
        return;

    gBS->FreePool(pmem);
}

void *ufs_bsp_allocate_xfer_buf (size_t size)
{
    return UncachedAllocatePool(size);
}

void  ufs_bsp_free_xfer_buf (void *pmem)
{
    if ( pmem == NULL )
        return;

    UncachedFreePool(pmem);
}

void ufs_bsp_get_platform_config (ufs_bsp_config_item *itm)
{
   EFI_STATUS                          eResult = EFI_SUCCESS;
   EFI_PLATFORMINFO_PROTOCOL           *hPlatformInfoProtocol;
   EFI_PLATFORMINFO_PLATFORM_INFO_TYPE PlatformInfo;
   uint32_t soc_emulation_type = 0;
   
   eResult = gBS->LocateProtocol(&gEfiPlatformInfoProtocolGuid, NULL,
     (VOID **) &hPlatformInfoProtocol);
   if (eResult != EFI_SUCCESS)
   {
     DEBUG((EFI_D_WARN, "Error: Failed to locate PlatformInfo protocol.\n"));
     goto ConfigEnd;
   }
   
   eResult = hPlatformInfoProtocol->GetPlatformInfo(hPlatformInfoProtocol,
     &PlatformInfo);
   if (eResult != EFI_SUCCESS)
   {
     DEBUG((EFI_D_WARN, "Error: GetPlatformInfo failed.\n"));
     goto ConfigEnd;
   }
   
   if (PlatformInfo.platform >= EFI_PLATFORMINFO_NUM_TYPES)
   {
     DEBUG((EFI_D_WARN, "Error: Unknown platform type (%d)\n", PlatformInfo.platform));
     eResult = EFI_PROTOCOL_ERROR;
     goto ConfigEnd; 
   }

   if (PlatformInfo.platform == EFI_PLATFORMINFO_TYPE_RUMI) {
   	g_platform = DALPLATFORMINFO_TYPE_RUMI;
   }

ConfigEnd:
   if (eResult != EFI_SUCCESS) {
      soc_emulation_type = in_dword(0x01fc8004);

      /* 0 = Silicon, 1 = RUMI, 2 = Veloce, 3 = Virtio */
      if (soc_emulation_type == 1) 
      {
         g_platform = DALPLATFORMINFO_TYPE_RUMI; 
      }
   }

   if (g_platform == DALPLATFORMINFO_TYPE_RUMI) {
      itm->slow_xfer = 1;
      itm->core_skip = 0; 
      itm->low_volt_support = 0; 
      itm->g4_support[0] = 0;
      itm->g4_support[1] = 0;
   }
   else {
      /* VIRTIO or Silicon */
      itm->slow_xfer = 0;
      itm->core_skip = 1; 
      itm->low_volt_support = 1; 
      itm->g4_support[0] = 0;
      itm->g4_support[1] = 0;
   }
}

uintptr_t ufs_bsp_get_reg_baseaddress (uint8_t hostid)
{
   if (0 == hostid) 
   {
      return (uintptr_t) 0x1d80000;
   }
   else if (1 == hostid) 
   {
      return (uintptr_t) 0x1d60000;
   }
   else
   {
      return 0; 
   }
}

boolean ufs_bsp_get_dal_config(ufs_dal_config_item *itm)
{
   if (g_platform == DALPLATFORMINFO_TYPE_RUMI) {
      itm->enable_hs = 1; 
      itm->gear = 1;
      itm->num_lanes = 1; 
   }

   return TRUE; 
}

void ufs_bsp_log_error(char *str)
{
   DEBUG ((EFI_D_ERROR, "%a\n", str)); 
}

ufs_bsp_err_type ufs_bsp_volt_set(uint8_t hostid, ufs_bsp_volt_mode mode)
{
   static npa_client_handle client_handle0 = NULL;
   static npa_client_handle client_handle1 = NULL; 
   npa_client_handle client_handle = NULL; 

   if (hostid == 0) {
      if (client_handle0 == NULL) {
         client_handle0 = npa_create_sync_client(PMIC_NPA_GROUP_ID_UFS1, "ufs1", NPA_CLIENT_REQUIRED); 
      }
      client_handle = client_handle0; 
   }
   else if (hostid == 1) {
      if (client_handle1 == NULL) {
         client_handle1 = npa_create_sync_client(PMIC_NPA_GROUP_ID_UFS0, "ufs0", NPA_CLIENT_REQUIRED);
      }
      client_handle = client_handle1; 
   }
   else {
      return UFS_BSP_ERR_INVALID_PARAM;
   }

   if (client_handle == NULL) {
      return UFS_BSP_ERR_PMIC;
   }

   /* Turn all UFS regulators ON */
   if (mode == UFS_BSP_REGULATORS_ON) {
      if (hostid == 1) {
         npa_issue_required_request(client_handle, PMIC_NPA_MODE_ID_GENERIC_HV);
      }
   }
   /* Lower Vcc to 2.5 V */
   else if (mode == UFS_BSP_REGULATORS_UFS_30) {
      npa_issue_required_request(client_handle, PMIC_NPA_MODE_ID_GENERIC_ACTIVE); 
   }
   /* Lower Vcc to 0V for UFS device sleep */
   else if (mode == UFS_BSP_REGULATORS_SLEEP) {
      npa_issue_required_request(client_handle, PMIC_NPA_MODE_ID_GENERIC_STANDBY);
   }
   /* Turn all UFS regulators OFF */
   else if (mode == UFS_BSP_REGULATORS_OFF) {
      npa_issue_required_request(client_handle, PMIC_NPA_MODE_ID_GENERIC_OFF);
   }
   else {
      return UFS_BSP_ERR_FEATURE_UNSUPPORTED; 
   }

   return UFS_BSP_NO_ERROR;
}
