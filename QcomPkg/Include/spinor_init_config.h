#ifndef __SPINOR_INIT_CONFIG_H__
#define __SPINOR_INIT_CONFIG_H__

/**********************************************************************
 * spinor_init_config.h
 *
 * This file provides definitions for the spinor init config implementations 
 *
 * Copyright (c) 2017
 * Qualcomm Technologies Incorporated.
 * All Rights Reserved.
 * Qualcomm Confidential and Proprietary
 *
 **********************************************************************/
/*======================================================================

                        EDIT HISTORY FOR MODULE


when         who      what, where, why
----------   ------   ----------------------------------------------- 
2017-05-25  md        Initial Release

==================================================================*/
#include "spinor_api.h"

#define SPINOR_INIT_TABLE_MAGIC_NUMBER 0x46474643

#define SPINOR_INIT_TABLE_VERSION_NUMBER 0x1

typedef struct __attribute__((__packed__))
{
  uint32  magic_number;                   /* Magic number used to indicate the structure is valid */
  uint8   ddr_support;                    /* Is DDR Supported */
  uint8   addr_bytes;                     /* 3 or 4 Bytes Addressing Mode */
  uint8   read_mode;                      /* Read Mode: Command-Address-Data */
                                          /* READ_SDR_1_1_1_MODE = 0. READ_SDR_1_1_4_MODE = 1. READ_SDR_1_4_4_MODE = 2 */
                                          /* READ_SDR_4_4_4_MODE = 3. READ_DDR_1_1_4_MODE = 4. READ_DDR_1_4_4_MODE = 5. READ_DDR_4_4_4_MODE = 6 */
  uint8   read_sdr_opcode;                /* Read SDR mode opcode */
  uint8   read_ddr_opcode;                /* Read DDR mode opcode. Set to 0 if DDR is not supported */
  uint8   read_sdr_wait_state;            /* Read SDR Mode. Wait_state is the total of dummy cycles to wait */
  uint8   read_ddr_wait_state;            /* Read DDR mode. Wait state is the total of dummy cycles to wait */
  uint8   write_mode;                     /* Write mode: Command-Address-Data Mode */
                                          /* WRITE_SDR_1_1_1_MODE = 0. WRITE_SDR_1_1_4_MODE = 1. WRITE_SDR_1_4_4_MODE = 2 */
                                          /* WRITE_SDR_4_4_4_MODE = 3. WRITE_DDR_1_1_4_MODE = 4. WRITE_DDR_1_4_4_MODE = 5. WRITE_DDR_4_4_4_MODE = 6 */
  uint8   write_sdr_opcode;               /* Opcode used for Write in SDR mode */
  uint8   write_ddr_opcode;               /* Opcode used for Write in DDR mode */
  uint8   erase_4kb_opcode;               /* Opcode used for 4KB Block erase */
  uint8   bulk_erase_size_in_4KB;         /* Bulk erase size. Unit is 4KB */
  uint8   bulk_erase_opcode;              /* The opcode for the supported bulk erase size (larger than 4KB block erase). If bulk erase is not supported, set to 0 */
  uint8   quad_enable_mode;               /* Quad Enable Requirements (QER) as defined in the JEDEC Standard No. 216A Document */

  uint8   erase_err_bmsk;                 /* Status BIT(s) in the erase_err_status_reg Register indicating if there's erase error condition */
  uint8   erase_err_status_reg;           /* register address used for polling the erase status */
  uint8   write_err_bmsk;                 /* Status BIT(s) in the write_err_status_reg Register indicating if there's write error condition */
  uint8   write_err_status_reg;           /* register address used for polling the write status */
  uint8   erase_status_polling_ms;        /* erase_err_status_reg Register status polling intervals in ms unit */
  uint8   read_sleep_time_in_10us;        /* total sleep time between reads to yeild CPU to other threads. Unit is 10us */
  uint8   write_sleep_time_in_10us;       /* total sleep time between writes to yeild CPU to other threads. Uint is 10us */     
                                          /* Software will break a large read transfer into smaller transfers of this size in page unit */ 
  uint16  read_page_count_between_sleep;  /* total pages read before yielding CPU to other threads */  
  uint16  write_page_count_between_sleep; /* total pages write before yielding CPU to other threads */  
  uint16  write_status_polling_usec;      /* write_err_status_reg Register status polling intervals in micro second unit */
  uint32  freq_khz;                       /* Max supported Frequency */                      
  uint32  read_max_transfer_in_pages;     /* This field indicates the maximum Read transfer size in unit of page (256 bytes per page) */   
  uint32  density_in_blocks;              /* Device density in unit of Blocks */
  uint32  block_size_in_bytes ;           /* Block density in bytes */
  uint32  pages_per_block;                /* No of pages per block */
  uint32  page_size_in_bytes;             /* No of bytes per page */
  uint32  device_id;                      /* Device ID when query with Device Read ID command 0x9F */  
  uint8   spinor_client_ref_count;        /* Keep track of how many clients have called spinor_init() */
  uint8   power_on_delay_in_100us;        /* Power On Reset delay in 100us unit */
  boolean dma_enable;
  uint32  reserved_1; 
  uint32  reserved_3; 
} spinor_config_data;

typedef struct __attribute__((__packed__))
{
  uint32 magic_no;
  uint32 total_entries;
  uint32 version_no;
  void *spinor_config_entries;
} spinor_init_config_table_type;

void* spinor_get_config_entries_struct(void);
SPINOR_STATUS spinor_get_config_entries_count(uint32 *total_nor_entries);
void* spinor_get_config_table_struct(void);
#endif /* __SPINOR_INIT_CONFIG_H__ */
