#ifndef UFS_BSP_H
#define UFS_BSP_H
 /**********************************************************************
 * ufs_bsp.h
 *
 * UFS driver BSP.
 *
 * Copyright (c) 2008-2019
 * Qualcomm Technologies Incorporated.
 * All Rights Reserved.
 * Qualcomm Confidential and Proprietary
 *
 **********************************************************************

                             Edit History


when         who     what, where, why
----------   ---     -------------------------------------------------------------- 
2019-09-24   jt      Add sleep voltage mode 
2019-02-01   jt      Add UFS 3.0 support  
2018-11-29   jt      Secondary UFS enablement changes 
2018-09-24   jt      Modify ufs_mphy_init_item
2018-09-10   jt      Add flags to control PHY initialization  
2018-07-30   jt      Add controller specific PHY init capability 
2018-06-04   jt      Add clock info struct 
2017-07-31   jt      Add logging function   
2017-06-02   jt      Add function for non-HSG4 PHY init 
2016-11-01   jt      Add reset function 
2016-08-22   jt      Update for 8998v2  
2016-08-18   jt      Add DAL config   
2015-06-15   rh      Disable the PHY from decoding the LCC sequence
2013-06-20   rh      Initial version.
-----------------------------------------------------------------------------*/

#include "hwio_ufs_v1.h"
#include "ufs_osal.h"

// Define the number of HCI controller
#define MAX_UFS_HCI_ID        2

#define UFS_CLOCK_MODE_OFF    0
#define UFS_CLOCK_MODE_ON     1

#define FLUSH                 1
#define INVAL                 2
#define CLEAN                 4

#define MPHY_INIT_BUSY_WAIT   0x00000001

/* Flags for controlling PHY init entries in ufs_mphy_init_item
 
   Bits 0-1: rate_modes
   Bits 2-3: gear_modes
   Bits 4-5: controller_ver
   Bits 6-14: reserved
   Bit  15: stop- no more entries
*/

#define STOP_CONDITION 0x8000

#define RATE_SHFT 0x0
#define RATE_MASK 0x00000003

#define GEAR_SHFT 0x2
#define GEAR_MASK 0x0000000c

#define VER_SHFT  0x4
#define VER_MASK  0x00000030

#define CONDITIONAL_FLAG(rate, gear, ver) ((rate << RATE_SHFT) | (gear << GEAR_SHFT) | (ver << VER_SHFT))

typedef enum {
   RATE_A = 0x1, 
   RATE_B = 0x2
} rate_modes;

typedef enum {
   DEFAULT_GEAR = 0x1,  /* represents HS-G3 and below */
   HS_G4 = 0x2
} gear_modes;

typedef enum {
   CONTROLLER_VER_4_0 = 0x1,  /* represents major ver = 4, minor ver = 0 */
   CONTROLLER_VER_4_1 = 0x2   /* represents major ver = 4, minor ver = 1 */
} controller_ver;

// Data structure used to initialize the UFS phy
struct ufs_mphy_init_item {
   uint16_t flags; 
   uint16_t offset; 
   uint8_t  val;
}; 

/* UFS Error Types */
typedef enum
{
   UFS_BSP_NO_ERROR,
   UFS_BSP_DEVICE_ERROR, 
   UFS_BSP_ERR_INVALID_PARAM,
   UFS_BSP_ERR_FEATURE_UNSUPPORTED, 
   UFS_BSP_ERR_PMIC
} ufs_bsp_err_type;

/* UFS voltage modes */
typedef enum
{
   UFS_BSP_REGULATORS_OFF, 
   UFS_BSP_REGULATORS_ON, 
   UFS_BSP_REGULATORS_SLEEP,
   UFS_BSP_REGULATORS_UFS_30
} ufs_bsp_volt_mode;

/* Cock busy wait function, unit in uS*/
void ufs_bsp_busy_wait(uint32_t us);

/* Configuration of all clock sources */
void ufs_bsp_clk_set (uint8_t hostid, uint32_t mode);

void ufs_bsp_reset (uint8_t hostid); 
void ufs_bsp_device_reset (uint8_t hostid);

void ufs_bsp_enable_symbol_clk (void);

/* Get the data structure for the UFS PHY init table */
uint32_t ufs_bsp_get_mphy_init_item_table (struct ufs_mphy_init_item **itm);
uint32_t ufs_bsp_get_mphy_init_2_lane_item_table (struct ufs_mphy_init_item **itm);
uint32_t ufs_bsp_get_mphy_init_end_item_table (struct ufs_mphy_init_item **itm);

uint32_t ufs_bsp_mphy_start (uint8_t hostid);
void ufs_bsp_post_link_init (uint8_t hostid);

/* Cache management wrapper */
void ufs_bsp_cache_op (void *addr, uint32_t len, uint32_t op);

/* Basic memory barrier function */
void ufs_bsp_memory_barrier (void);

/* Functions to manage transfer descriptor buffer */
void *ufs_bsp_allocate_xfer_buf (size_t size);
void  ufs_bsp_free_xfer_buf (void *pmem);

/* Map register into accessable memory region */
uintptr_t ufs_bsp_get_reg_baseaddress (uint8_t hostid);

/* Settings on each platform */
typedef struct {
   uint8_t     slow_xfer;
   uint8_t     core_skip; 
   uint8_t     g4_support[MAX_UFS_HCI_ID];
   uint8_t     low_volt_support; 
} ufs_bsp_config_item;

/* DAL config settings */
typedef struct {
   uint8_t     enable_hs;
   uint8_t     gear;
   uint8_t     num_lanes;
} ufs_dal_config_item;

/* UFS clock information */
typedef struct _UFS_CLOCK_INFO
{
   CHAR8* const clock_name;
   UINTN        clock_id;
   UINT32       freq; 
} UFS_CLOCK_INFO;

typedef struct _UFS_CLOCK_LIST
{
   UFS_CLOCK_INFO phy_axi_clk; 
   UFS_CLOCK_INFO ice_core_clk;
   UFS_CLOCK_INFO unipro_core_clk; 
   UFS_CLOCK_INFO aggre_phy_axi_clk; 
   UFS_CLOCK_INFO phy_ahb_clk; 
   UFS_CLOCK_INFO phy_aux_clk; 
   UFS_CLOCK_INFO tx_symbol_0_clk; 
   UFS_CLOCK_INFO rx_symbol_0_clk; 
   UFS_CLOCK_INFO rx_symbol_1_clk; 
} UFS_CLOCK_LIST;

void ufs_bsp_get_platform_config (ufs_bsp_config_item *itm);
boolean ufs_bsp_get_dal_config (ufs_dal_config_item *itm); 

/* Log to serial function */ 
void ufs_bsp_log_error(char *str);

ufs_bsp_err_type ufs_bsp_volt_set(uint8_t hostid, ufs_bsp_volt_mode mode); 

#endif /* #ifndef UFS_BSP_H */

