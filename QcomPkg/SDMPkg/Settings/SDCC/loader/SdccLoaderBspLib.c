/** @file SdccLoaderBspLib.c

  SDCC driver BSP
  This file implements the SDCC driver BSP for the board in use

  Copyright (c) 2016-2019 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential

**/

/*=============================================================================
                              EDIT HISTORY

when         who     what, where, why
----------   ---     -----------------------------------------------------------
2019-06-26   jt      Add sdcc_bsp_get_platform_type() 
2018-07-13   jt      Add voltage switch functions  
2018-01-25   vk      Remove ArmLib
2017-10-30   jt      Add sdcc_bsp_busy_wait() 
2017-08-22   jt      Update TLMM addr for Hana  
2017-06-16   sb      Get platform specific speed config 
2017-06-12   jt      Update drive strength 
2017-01-17   jt      Update sdcc_bsp_get_max_supported_clk()
2016-12-08   jt      Update for Napali. Rename file.  
2016-01-18   jt      Initial version. Branched from 8996 XBL 

=============================================================================*/

#include "SdccBsp.h"
#include "msmhwiobase.h"
#include "HALhwio.h"
#include "Target_cust.h"
#include "XBLLoader/boot_cache_mmu.h"
#include "boot_cache.h"
#include "DDIPlatformInfo.h"
#include <api/systemdrivers/busywait.h>

#include "pm_ldo.h"
#include "pm_version.h"

/* Important Note: */
/* For every new target, verify with the SWI document. */
/* Ensure the SDCC base physical addresses are mapped correct */

#define HWIO_TLMM_SDC2_HDRV_PULL_CTL_ADDR  0x39B2000

#define HWIO_TCSR_SOC_EMULATION_TYPE       0x1FC8004

#define HWIO_SDC2_SDCC5_HC_ADDR            0x8804000
#define HWIO_SDC4_SDCC5_HC_ADDR            0x8844000

/******************************************************************************
                           S D C C    B S P
******************************************************************************/

/* Max number of slots supported by the Board */
#define SDCC_BSP_NUM_SLOTS         4
/* Todo: Query from pcd or allow query this info. */
/* Used for slot number validation*/
#define SDCC_BSP_INVALID_SLOT      SDCC_BSP_NUM_SLOTS
/* Cache line size */
#define SDCC_BSP_CACHE_LINE_SIZE   32

/* Slot configuration information for the board */
static sdcc_bsp_slot_type sdcc_bsp_slot_config [SDCC_BSP_NUM_SLOTS] = {
   SDCC_BSP_NO_SLOT,
   SDCC_BSP_SD_SLOT,
   SDCC_BSP_NO_SLOT,
   SDCC_BSP_NO_SLOT
};

/* Slot internal or external configuration information */
static sdcc_bsp_slot_access_type sdcc_bsp_slot_access_config [SDCC_BSP_NUM_SLOTS] = {
   SDCC_BSP_SLOT_INVALID,
   SDCC_BSP_SLOT_EXTERNAL,
   SDCC_BSP_SLOT_INVALID,
   SDCC_BSP_SLOT_INVALID
};

boolean busywait_inited = FALSE; 

/******************************************************************************
* FUNCTION      sdcc_bsp_target_cap_override
*
* DESCRIPTION   This function provides target specific cap overrides
*
* PARAMETERS    [IN]: Slot number
*
* RETURN VALUE  None
*
******************************************************************************/
VOID sdcc_bsp_target_cap_overrides (UINT32 slot_num) {
  return;
}

/*=============================================================================
 * FUNCTION      sdcc_bsp_get_base_addr
 *
 * DESCRIPTION   This function queries the SDCC register base addresses
 *
 * PARAMETERS    [OUT]: sdcc_base_addr_type pointer to retrieve the SDCC base addresses
 *
 * RETURN VALUE  sdcc_bsp_err_type
 *
 *===========================================================================*/
sdcc_bsp_err_type
sdcc_bsp_get_base_addr (sdcc_base_addr_type *sdcc_base_addr)
{
  sdcc_base_addr->sdc2_base_hc = HWIO_SDC2_SDCC5_HC_ADDR;
  sdcc_base_addr->sdc4_base_hc = HWIO_SDC4_SDCC5_HC_ADDR;
  
  return SDCC_BSP_NO_ERROR;
}

/*=============================================================================
 * FUNCTION      sdcc_bsp_get_slot_type
 *
 * DESCRIPTION   This function queries the slot to find out if the slot is designated
 *               for eMMC, SD or no device.
 *
 * PARAMETERS    [IN]: slot number
 *
 * RETURN VALUE  sdcc_bsp_slot_type
 *
 *===========================================================================*/
sdcc_bsp_slot_type
sdcc_bsp_get_slot_type (UINT32 slot_num)
{
   sdcc_bsp_slot_type slot_type = SDCC_BSP_NO_SLOT;

   if (slot_num >= SDCC_BSP_NUM_SLOTS)
   {
      /* Return No slot available*/
      slot_type = SDCC_BSP_NO_SLOT;
   }
   else
   {
      slot_type = sdcc_bsp_slot_config [slot_num];
   }

   return slot_type;
}

/*=============================================================================
 * FUNCTION      sdcc_bsp_get_slot_access_type
 *
 * DESCRIPTION   This function queries the slot to find out if the slot is designated
 *               as internal or external memory device. Hotplug needs this info
 *               to determine which memory device to boot from.
 *
 * PARAMETERS    [IN]: slot number
 *
 * RETURN VALUE  sdcc_bsp_slot_access_type
 *
 *===========================================================================*/
sdcc_bsp_slot_access_type
sdcc_bsp_get_slot_access_type (UINT32 slot_num)
{
   sdcc_bsp_slot_access_type slot_access_type = SDCC_BSP_SLOT_INVALID;

   if (slot_num >= SDCC_BSP_NUM_SLOTS)
   {
      /* Return No slot available*/
      slot_access_type = SDCC_BSP_SLOT_INVALID;
   }
   else
   {
      slot_access_type = sdcc_bsp_slot_access_config [slot_num];
   }

   return slot_access_type;
}

/*=============================================================================
 * FUNCTION      sdcc_bsp_gpio_control
 *
 * DESCRIPTION   This function enables/disables the GPIO interface to the
 *               card slot.
 *
 * PARAMETERS    [IN]: slot number
 *               [IN]: GPIO ON/OFF
 *
 * RETURN VALUE  sdcc_bsp_err_type.
 *
 *===========================================================================*/
sdcc_bsp_err_type
sdcc_bsp_gpio_control (UINT32 slot_num, UINT32 state)
{
   if(slot_num >= SDCC_BSP_NUM_SLOTS)
   {
      return SDCC_BSP_ERR_INVALID_PARAM;
   }

   if(SDCC_BSP_NO_SLOT == sdcc_bsp_slot_config [slot_num])
   {
      return SDCC_BSP_ERR_INVALID_PARAM;
   }

   switch(slot_num)
   {
   case SDCC_BSP_SLOT_1:
      out_dword (HWIO_TLMM_SDC2_HDRV_PULL_CTL_ADDR, 0x1FE4);
      break;
   default:
      return SDCC_BSP_ERR_INVALID_PARAM;
   }

   return SDCC_BSP_NO_ERROR;
}

/******************************************************************************
* FUNCTION      sdcc_bsp_get_max_supported_mclk_freq
*
* DESCRIPTION   Get the maximum mclock frequency supported
*
* PARAMETERS    [IN]: drive number
*
* RETURN VALUE  Clock frequency supported
*
******************************************************************************/
sdcc_bsp_clk_type
sdcc_bsp_get_max_supported_clk (UINT32 driveno)
{   
   switch(driveno)
   {
   case SDCC_BSP_SLOT_1:
      return SDCC_BSP_CLK_200_MHZ;
   case SDCC_BSP_SLOT_3:
      return SDCC_BSP_CLK_100_MHZ;
   default:
      return SDCC_BSP_CLK_INVALID;
   }
}

/******************************************************************************
* FUNCTION      sdcc_bsp_get_hs200_supported_freq
*
* DESCRIPTION   Get the supported frequency for HS200 mode
*
* PARAMETERS    [IN]: drive number
*
* RETURN VALUE  Clock frequency supported
*
******************************************************************************/
sdcc_bsp_clk_type
sdcc_bsp_get_hs200_supported_freq (UINT32 driveno)
{
   return SDCC_BSP_CLK_192_MHZ;
}

/******************************************************************************
* FUNCTION      sdcc_bsp_get_hs400_supported_freq
*
* DESCRIPTION   Get the supported frequency for HS400 mode
*
* PARAMETERS    [IN]: drive number
*
* RETURN VALUE  Clock frequency supported
*
******************************************************************************/
sdcc_bsp_clk_type
sdcc_bsp_get_hs400_supported_freq (UINT32 driveno)
{
   return SDCC_BSP_CLK_384_MHZ;
}

/******************************************************************************
* FUNCTION      sdcc_bsp_busy_wait
*
* DESCRIPTION   Waits for the specified amount of us
*
* PARAMETERS    [IN]: us to wait 
*
* RETURN VALUE  None
*
******************************************************************************/
void sdcc_bsp_busy_wait(UINT32 us)
{
   if (FALSE == busywait_inited)
   {
      busywait_init(); 
      busywait_inited = TRUE; 
   }
   busywait(us); 
}

/******************************************************************************
* FUNCTION      sdcc_bsp_cache_operation
*
* DESCRIPTION   Performs cache operations on the provided memory region.
*               Used for DMA related operations only.
*
* PARAMETERS    [IN]: cache operation type
*               [IN]: start address of the memory region
*               [IN]: length of the memory region
*
* RETURN VALUE  None
*
******************************************************************************/
void sdcc_bsp_cache_operation
(
   sdcc_bsp_cache_op_type op,
   void *addr,
   UINT32 length
)
{
   switch(op)
   {
      case SDCC_BSP_CACHE_FLUSH:
        dcache_flush_region (addr, length);
        break;

      case SDCC_BSP_CACHE_INVAL:
        dcache_inval_region (addr, length);
        break;

      default:
        break;
   }
}

/******************************************************************************
* FUNCTION      sdcc_bsp_get_vreg_switch_status
*
* DESCRIPTION   Check whether PMIC APIs work (for UHS-1) by enabling the VDD
*
* PARAMETERS    [IN]: driveno
*
* RETURN VALUE  sdcc_bsp_err_type
*
******************************************************************************/
sdcc_bsp_err_type sdcc_bsp_get_vreg_switch_status (UINT32 driveno)
{
   if (SDCC_BSP_SLOT_1 == driveno)
   {
      if (PM_ERR_FLAG_SUCCESS != pm_ldo_sw_enable(PMIC_C, PM_LDO_6, PM_ON, TRUE))
      {
         return SDCC_BSP_ERR_PMIC; 
      }
      else
      {
         return SDCC_BSP_NO_ERROR; 
      }
   }
   else
   {
      return SDCC_BSP_ERR_INVALID_PARAM;
   }
}

/******************************************************************************
* FUNCTION      sdcc_bsp_lower_volt_level
*
* DESCRIPTION   Lower the VDD to 1.8V for UHS1
*
* PARAMETERS    [IN]: driveno
*
* RETURN VALUE  sdcc_bsp_err_type
*
******************************************************************************/
sdcc_bsp_err_type sdcc_bsp_lower_volt_level (UINT32 driveno)
{
   if (SDCC_BSP_SLOT_1 == driveno)
   {
      if (PM_ERR_FLAG_SUCCESS != pm_ldo_volt_level(PMIC_C, PM_LDO_6, 1800 * 1000, TRUE))
      {
         return SDCC_BSP_ERR_PMIC; 
      }
      else
      {
         return SDCC_BSP_NO_ERROR; 
      }
   }
   else
   {
      return SDCC_BSP_ERR_INVALID_PARAM; 
   }
}

/******************************************************************************
* FUNCTION      sdcc_bsp_memory_barrier
*
* DESCRIPTION   Performs data memory barrier operation.
*
* PARAMETERS    None
*
* RETURN VALUE  None
*
******************************************************************************/
void sdcc_bsp_memory_barrier(void)
{
  data_mem_barrier(); 
}

/******************************************************************************
* FUNCTION      sdcc_bsp_get_slot_count
*
* DESCRIPTION   Get the maximum SDCC number of slots support
*
* PARAMETERS    [IN]: None
*
* RETURN VALUE  Maximum number of slots on the target
*
******************************************************************************/
UINT32 sdcc_bsp_get_slot_count (VOID)
{
   return SDCC_BSP_NUM_SLOTS;
}

/******************************************************************************
* FUNCTION      sdcc_bsp_get_platform_type
*
* DESCRIPTION   Get the platform type of the device
*
* PARAMETERS    [IN]: None
*
* RETURN VALUE  The platform type
*
******************************************************************************/
DalPlatformInfoPlatformType sdcc_bsp_get_platform_type(VOID)
{
   return DalPlatformInfo_Platform();
}

/******************************************************************************
* FUNCTION      sdcc_bsp_get_platform_speed_config
*
* DESCRIPTION   Get the speed config based on the platform
*
* PARAMETERS    [IN]: speed_mode
*
* RETURN VALUE  Speed config based on the platform
*
******************************************************************************/
UINT8
sdcc_bsp_get_platform_speed_config (UINT8 speed_mode)
{
   DalPlatformInfoPlatformType sdc_platform = DALPLATFORMINFO_TYPE_UNKNOWN;
   UINT32 soc_emulation_type = 0;
   
   sdc_platform = DalPlatformInfo_Platform();

   /* If DAL API doesn't work, query the SOC emulation type register */
   if (sdc_platform == DALPLATFORMINFO_TYPE_UNKNOWN) 
   {
      soc_emulation_type = in_dword(HWIO_TCSR_SOC_EMULATION_TYPE);

      /* 0 = Silicon, 1 = RUMI, 2 = Veloce, 3 = Virtio */
      if (1 == soc_emulation_type) 
      {
         sdc_platform = DALPLATFORMINFO_TYPE_RUMI; 
      }
   }

   if (sdc_platform == DALPLATFORMINFO_TYPE_RUMI)
   {
     return 0x3; //SDR
   }
   else
   {
     return speed_mode; 
   }
}

