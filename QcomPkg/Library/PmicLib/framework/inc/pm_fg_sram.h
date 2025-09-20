#ifndef __PM_FG_SRAM_H__
#define __PM_FG_SRAM_H__

 /*! @file pm_fg_sram.h
 *
 * PMIC Battery Profile SRAM/OTP access functionalities 
 * FG OTP Starts at address 0x0.
 * 256x24 (3 banks of 256x8) byte addressable within a 32 bit word.
 * MSByte of each 32 bit word is invalid and will readback 0x00 and will not be programmed.
 * 
 * FG RAM Starts at address 0x400.
 * 128x32 byte addressable.
 *
 * Copyright (c) 2014-2018 Qualcomm Technologies, Inc.  All Rights Reserved. 
 * Qualcomm Technologies Proprietary and Confidential.
 */

/*=============================================================================
                              EDIT HISTORY


 when            who     what, where, why
 --------------------------------------------------------------------------------------
 12/22/17   pxm     Add SRAM access DMA support
 08/22/17   czq     Detect the bad conditions of FG SRAM (trim registers) and issue VDD hard reset on PM/PMi to recover
 07/10/17   pxm     Add function PmicFgSram_ImaWriteSingle
 09/22/14   aab     Updated to support FG SRAM configuration in SBL
 06/06/14   va      New file.
=============================================================================*/

/**
  PMIC Platform interface
 */

#include "com_dtypes.h"         /* Containse basic type definitions */
#include "pm_err_flags.h"     /* Containse the error definitions */
#include "pm_resources_and_types.h"



/*===========================================================================
                               MACROS
===========================================================================*/
// Start address included while End address excluded
#define FG_SRAM_START_ADDR     0
#define FG_SRAM_END_ADDR       480
#define FG_SRAM_ADDRESS_IS_INVALID(address) ((address) < FG_SRAM_START_ADDR || (address) >= FG_SRAM_END_ADDR)

#define NUM_BITS_IN_BYTE 8

#define FG_SRAM_RD_WR_BUS_WIDTH         4
//#define FG_SRAM_RD_WR_OFFSET_WIDTH      3

#define NUM_BYTE_IN_SRAM_DATA      (sizeof(uint16))
#define FG_SRAM_DMA_PARTITIONS     6
#define FG_SRAM_DMA_OFFSET         0x20

#define FG_SRAM_MAX_RETRY     10

// TODO: 0x4400 ~ 0x49FF is still not defined in go/swi, how can we know this range?
//#define FG_SRAM_DMA_PARTITIONS           6
#define FG_SRAM_START_DMA_ADDR           0x4400
#define FG_SRAM_END_DMA_ADDR             0x49FF

#define FG_SRAM_START_DMA_PROFILE_ADDR   0x4500
#define FG_SRAM_START_DMA_OFFSET 0x20
#define FG_SRAM_END_DMA_PROFILE_ADDR     0x46FF



/*=========================================================================
                            GLOBAL VARIABLES
===========================================================================*/
/**
  Sram State
*/
typedef enum {
  FG_SRAM_STATUS_INIT,
  FG_SRAM_STATUS_LOAD_PROFILE,
  FG_SRAM_STATUS_IN_USE,
  FG_SRAM_STATUS_POLLING,
  FG_SRAM_STATUS_AVAILABLE,
  FG_SRAM_STATUS_INVALID
} fg_sram_state_type;



typedef enum{
  FG_MEMORY_ACCESS_SLCT_DMA,
  FG_MEMORY_ACCESS_SLCT_IMA,
  FG_MEMORY_ACCESS_SLCT_TYPE_INVALID
}pm_fg_mem_if_memory_access_slct_type;


typedef enum{
  FG_MEMORY_ACCESS_INTF_CTL_MEM_ACS_SINGLE,
  FG_MEMORY_ACCESS_INTF_CTL_MEM_ACS_BURST,
  FG_MEMORY_ACCESS_INTF_CTL_BURST_INVALID
}pm_fg_mem_if_memory_access_intf_ctl_burst;



typedef enum{
  FG_MEMORY_ACCESS_INTF_CTL_READ_ACCESS,
  FG_MEMORY_ACCESS_INTF_CTL_WRITE_ACCESS,
  FG_MEMORY_ACCESS_INTF_CTL_INVALID
}pm_fg_mem_if_memory_access_intf_ctl_ima_wr_en;


typedef enum{
  FG_MEMORY_ACCESS_REQ_FALSE,
  FG_MEMORY_ACCESS_REQ_TRUE,
  FG_MEMORY_ACCESS_REQ_TYPE_INVALID
}pm_fg_mem_if_memory_access_req_type;

typedef struct {
   uint16 partition_start;
   uint16 partition_end;
   uint16 spmi_addr_base;
} pm_fg_dma_partition_type;



/*===========================================================================
                               FUNCTION DEFINITIONS
===========================================================================*/

pm_err_flag_type pm_fg_sram_init(uint32 pmic_index, pm_fg_mem_if_memory_access_slct_type access_type);

pm_err_flag_type pm_fg_sram_ima_read(uint32 pmic_index, uint16 address, uint16 *fg_memif_data);

pm_err_flag_type pm_fg_sram_ima_write_single(uint32 pmic_index, uint16 address, uint16 fg_memif_data);

pm_err_flag_type pm_fg_sram_dma_read_single(uint32 pmic_index, uint16 address, uint16 *fg_memif_data);

pm_err_flag_type pm_fg_sram_dma_write_single(uint32 pmic_index, uint16 address, uint16 fg_memif_data);

pm_err_flag_type pm_fg_sram_dma_read(uint32 pmic_index, uint16 sram_address, uint16 *data, uint16 length);

pm_err_flag_type pm_fg_sram_dma_write(uint32 pmic_index, uint16 sram_address, uint16 data);

#endif //__PM_FG_SRAM_H__


