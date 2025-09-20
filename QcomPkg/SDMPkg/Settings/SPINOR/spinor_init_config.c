/**********************************************************************
 * spinor_init_config.c
 *
 * Implementation of passing RAM data table from other boot environments
 * to spinor driver
 *
 * Copyright (c) 2017-2018, 2022
 * Qualcomm Technologies Incorporated.
 * All Rights Reserved.
 * Qualcomm Confidential and Proprietary
 *
 **********************************************************************/
/*=======================================================================
                        Edit History


when        who       what, where, why
----------  -----     ---------------------------------------------------
2022-05-18  vm        Add support for Winbond W25Q512NW-IM
2018-03-29  wek       Add support for Macronix MX25U51245G
2017-05-25  md        Initial Release


======================================================================*/
#include "spinor_init_config.h"
#include <string.h>


static spinor_config_data spinor_nor_entries[];


static spinor_init_config_table_type spinor_cfg_table =
{
  .magic_no = SPINOR_INIT_TABLE_MAGIC_NUMBER,
  .total_entries = 8,
  .version_no =SPINOR_INIT_TABLE_VERSION_NUMBER,
  .spinor_config_entries = spinor_nor_entries,

};


static spinor_config_data spinor_nor_entries[] =
{
  { // Winbond W25Q512NW-IM. 64MB
    .magic_number = 0xFACEECAF,
    .ddr_support = 0,
    .addr_bytes = 4,
    .read_mode = 2,
    .read_sdr_opcode = 0xEC,
    .read_ddr_opcode = 0,
    .read_sdr_wait_state = 6,
    .read_ddr_wait_state = 0,
    .write_mode = 1,
    .write_sdr_opcode = 0x34,
    .write_ddr_opcode = 0x00,
    .erase_4kb_opcode = 0x21,
    .bulk_erase_size_in_4KB = 16,
    .bulk_erase_opcode = 0xDC,
    .quad_enable_mode = 1,
    .erase_err_bmsk = 0,
    .erase_err_status_reg = 0,
    .write_err_bmsk = 0,
    .write_err_status_reg = 0,
    .erase_status_polling_ms = 5, /* not part related */
    .read_sleep_time_in_10us = 40, /* Not used in boot.xf */
    .write_sleep_time_in_10us = 40,/* Not used in boot.xf */
    .read_page_count_between_sleep = 255, /* Not used in boot.xf */
    .write_page_count_between_sleep = 64, /* Not used in boot.xf */
    .write_status_polling_usec = 100, /* not part related */
    .freq_khz = 32000,
    .read_max_transfer_in_pages = 0x20000,
    .density_in_blocks = 16384,
    .block_size_in_bytes = 4096,
    .pages_per_block = 16,
    .page_size_in_bytes = 256,
    .device_id = 0x002080EF,
    .spinor_client_ref_count = 0,
    .power_on_delay_in_100us = 8,
    .dma_enable = TRUE,
    .reserved_1 = 0,
    .reserved_3 = 0,
  },
  { // Winbond W25Q512NW-IQ/IN. 64MB
    .magic_number = 0xFACEECAF,
    .ddr_support = 0,
    .addr_bytes = 4,
    .read_mode = 2,
    .read_sdr_opcode = 0xEC,
    .read_ddr_opcode = 0,
    .read_sdr_wait_state = 6,
    .read_ddr_wait_state = 0,
    .write_mode = 1,
    .write_sdr_opcode = 0x34,
    .write_ddr_opcode = 0x00,
    .erase_4kb_opcode = 0x21,
    .bulk_erase_size_in_4KB = 16,
    .bulk_erase_opcode = 0xDC,
    .quad_enable_mode = 1,
    .erase_err_bmsk = 0,
    .erase_err_status_reg = 0,
    .write_err_bmsk = 0,
    .write_err_status_reg = 0,
    .erase_status_polling_ms = 5, /* not part related */
    .read_sleep_time_in_10us = 40, /* Not used in boot.xf */
    .write_sleep_time_in_10us = 40,/* Not used in boot.xf */
    .read_page_count_between_sleep = 255, /* Not used in boot.xf */
    .write_page_count_between_sleep = 64, /* Not used in boot.xf */
    .write_status_polling_usec = 100, /* not part related */
    .freq_khz = 32000,
    .read_max_transfer_in_pages = 0x20000,
    .density_in_blocks = 16384,
    .block_size_in_bytes = 4096,
    .pages_per_block = 16,
    .page_size_in_bytes = 256,
    .device_id = 0x002060EF,
    .spinor_client_ref_count = 0,
    .power_on_delay_in_100us = 8,
    .dma_enable = TRUE,
    .reserved_1 = 0,
    .reserved_3 = 0,
  },
  { /* Macronix MX25U51245G */
    .magic_number = 0xFACEECAF,
    .ddr_support = 0,
    .addr_bytes = 4,
    .read_mode = 2, /* READ_SDR_1_4_4_MODE 1 line for command 4 for address and data */
    .read_sdr_opcode = 0xEC,
    .read_ddr_opcode = 0,
    .read_sdr_wait_state = 6,
    .read_ddr_wait_state = 0,
    .write_mode = 2,
    .write_sdr_opcode = 0x3E,
    .write_ddr_opcode = 0x00,
    .erase_4kb_opcode = 0x21,
    .bulk_erase_size_in_4KB = 16,
    .bulk_erase_opcode = 0xDC,
    .quad_enable_mode = 2,
    .erase_err_bmsk = 0x40,
    .erase_err_status_reg = 0x2B,
    .write_err_bmsk = 0x20,
    .write_err_status_reg = 0x2B,
    .erase_status_polling_ms = 5, /* not part related */
    .read_sleep_time_in_10us = 40, /* Not used in boot.xf */
    .write_sleep_time_in_10us = 40,/* Not used in boot.xf */
    .read_page_count_between_sleep = 255, /* Not used in boot.xf */
    .write_page_count_between_sleep = 64, /* Not used in boot.xf */
    .write_status_polling_usec = 100, /* not part related */
    .freq_khz = 32000,
    .read_max_transfer_in_pages = 0x40000, /* Change to part size */
    .density_in_blocks = 16384, /* PART SIZE, should change for new part */
    .block_size_in_bytes = 4096,
    .pages_per_block = 16,
    .page_size_in_bytes = 256,
    .device_id = 0x003A25C2, /* This changes with each new part, command 9F*/
    .spinor_client_ref_count = 0,/* not part related */
    .power_on_delay_in_100us = 8,
    .dma_enable = TRUE,
    .reserved_1 = 0,
    .reserved_3 = 0,
  },
  { /* Winbond W25Q256JW. 32MB */
    .magic_number = 0xFACEECAF,
    .ddr_support = 0,
    .addr_bytes = 4,
    .read_mode = 2,
    .read_sdr_opcode = 0xEC,
    .read_ddr_opcode = 0,
    .read_sdr_wait_state = 6,
    .read_ddr_wait_state = 0,
    .write_mode = 1,
    .write_sdr_opcode = 0x34,
    .write_ddr_opcode = 0x00,
    .erase_4kb_opcode = 0x21,
    .bulk_erase_size_in_4KB = 16,
    .bulk_erase_opcode = 0xDC,
    .quad_enable_mode = 1,
    .erase_err_bmsk = 0,
    .erase_err_status_reg = 0,
    .write_err_bmsk = 0,
    .write_err_status_reg = 0,
    .erase_status_polling_ms = 5, /* not part related */
    .read_sleep_time_in_10us = 40, /* Not used in boot.xf */
    .write_sleep_time_in_10us = 40,/* Not used in boot.xf */
    .read_page_count_between_sleep = 255, /* Not used in boot.xf */
    .write_page_count_between_sleep = 64, /* Not used in boot.xf */
    .write_status_polling_usec = 100, /* not part related */
    .freq_khz = 32000,
    .read_max_transfer_in_pages = 0x20000,
    .density_in_blocks = 8192,
    .block_size_in_bytes = 4096,
    .pages_per_block = 16,
    .page_size_in_bytes = 256,
    .device_id = 0x001960EF,
    .spinor_client_ref_count = 0,
    .power_on_delay_in_100us = 8,
    .dma_enable = TRUE,
    .reserved_1 = 0,
    .reserved_3 = 0,
  },
  { /* Winbond W25Q256JW-IM. 32MB */
    .magic_number = 0xFACEECAF,
    .ddr_support = 0,
    .addr_bytes = 4,
    .read_mode = 2,
    .read_sdr_opcode = 0xEC,
    .read_ddr_opcode = 0,
    .read_sdr_wait_state = 6,
    .read_ddr_wait_state = 0,
    .write_mode = 1,
    .write_sdr_opcode = 0x34,
    .write_ddr_opcode = 0x00,
    .erase_4kb_opcode = 0x21,
    .bulk_erase_size_in_4KB = 16,
    .bulk_erase_opcode = 0xDC,
    .quad_enable_mode = 1,
    .erase_err_bmsk = 0,
    .erase_err_status_reg = 0,
    .write_err_bmsk = 0,
    .write_err_status_reg = 0,
    .erase_status_polling_ms = 5, /* not part related */
    .read_sleep_time_in_10us = 40, /* Not used in boot.xf */
    .write_sleep_time_in_10us = 40,/* Not used in boot.xf */
    .read_page_count_between_sleep = 255, /* Not used in boot.xf */
    .write_page_count_between_sleep = 64, /* Not used in boot.xf */
    .write_status_polling_usec = 100, /* not part related */
    .freq_khz = 32000,
    .read_max_transfer_in_pages = 0x20000,
    .density_in_blocks = 8192,
    .block_size_in_bytes = 4096,
    .pages_per_block = 16,
    .page_size_in_bytes = 256,
    .device_id = 0x001980EF,
    .spinor_client_ref_count = 0,
    .power_on_delay_in_100us = 8,
    .dma_enable = TRUE,
    .reserved_1 = 0,
    .reserved_3 = 0,
  },
  { /* Micron MT25QU256AB. 256MB */
    .magic_number = 0xFACEECAF,
    .ddr_support = 0,
    .addr_bytes = 4,
    .read_mode = 2,
    .read_sdr_opcode = 0xEB,
    .read_ddr_opcode = 0,
    .read_sdr_wait_state = 10,
    .read_ddr_wait_state = 0,
    .write_mode = 1,
    .write_sdr_opcode = 0x32,
    .write_ddr_opcode = 0x00,
    .erase_4kb_opcode = 0x20,
    .bulk_erase_size_in_4KB = 16,
    .bulk_erase_opcode = 0xD8,
    .quad_enable_mode = 0,
    .erase_err_bmsk = 0x7E,
    .erase_err_status_reg = 0x70,
    .write_err_bmsk = 0x7E,
    .write_err_status_reg = 0x70,
    .erase_status_polling_ms = 5,
    .read_sleep_time_in_10us = 40,
    .write_sleep_time_in_10us = 40,
    .read_page_count_between_sleep = 255,
    .write_page_count_between_sleep = 64,
    .write_status_polling_usec = 100,
    .freq_khz = 96000,
    .read_max_transfer_in_pages = 0x10000,
    .density_in_blocks = 0xFFFE,
    .block_size_in_bytes = 4096,
    .pages_per_block = 16,
    .page_size_in_bytes = 256,
    .device_id = 0x0020BB20,
    .spinor_client_ref_count = 0,
    .power_on_delay_in_100us = 8,
    .dma_enable = TRUE,
    .reserved_1 = 0,
    .reserved_3 = 0,
  },
  { /* Micron MT25QU256ABA8ESF. 32MB */
    .magic_number = 0xFACEECAF,
    .ddr_support = 0,
    .addr_bytes = 4,
    .read_mode = 2,
    .read_sdr_opcode = 0xEB,
    .read_ddr_opcode = 0,
    .read_sdr_wait_state = 10,
    .read_ddr_wait_state = 0,
    .write_mode = 1,
    .write_sdr_opcode = 0x32,
    .write_ddr_opcode = 0x00,
    .erase_4kb_opcode = 0x20,
    .bulk_erase_size_in_4KB = 16,
    .bulk_erase_opcode = 0xD8,
    .quad_enable_mode = 0,
    .erase_err_bmsk = 0x7E,
    .erase_err_status_reg = 0x70,
    .write_err_bmsk = 0x7E,
    .write_err_status_reg = 0x70,
    .erase_status_polling_ms = 5,
    .read_sleep_time_in_10us = 40,
    .write_sleep_time_in_10us = 40,
    .read_page_count_between_sleep = 255,
    .write_page_count_between_sleep = 64,
    .write_status_polling_usec = 100,
    .freq_khz = 96000,
    .read_max_transfer_in_pages = 0x10000,
    .density_in_blocks = 8192,
    .block_size_in_bytes = 4096,
    .pages_per_block = 16,
    .page_size_in_bytes = 256,
    .device_id = 0x0019BB20,
    .spinor_client_ref_count = 0,
    .power_on_delay_in_100us = 8,
    .dma_enable = TRUE,
    .reserved_1 = 0,
    .reserved_3 = 0,
  },
  { /* Micron N25Q128A11E40. 16MB */
    .magic_number = 0xFACEECAF,
    .ddr_support = 0,
    .addr_bytes = 3,
    .read_mode = 2,
    .read_sdr_opcode = 0xEB,
    .read_ddr_opcode = 0,
    .read_sdr_wait_state = 10,
    .read_ddr_wait_state = 0,
    .write_mode = 1,
    .write_sdr_opcode = 0x32,
    .write_ddr_opcode = 0x00,
    .erase_4kb_opcode = 0x20,
    .bulk_erase_size_in_4KB = 16,
    .bulk_erase_opcode = 0xD8,
    .quad_enable_mode = 0,
    .erase_err_bmsk = 0x7E,
    .erase_err_status_reg = 0x70,
    .write_err_bmsk = 0x7E,
    .write_err_status_reg = 0x70,
    .erase_status_polling_ms = 5,
    .read_sleep_time_in_10us = 40,
    .write_sleep_time_in_10us = 40,
    .read_page_count_between_sleep = 255,
    .write_page_count_between_sleep = 64,
    .write_status_polling_usec = 100,
    .freq_khz = 96000,
    .read_max_transfer_in_pages = 0x10000,
    .density_in_blocks = 4096,
    .block_size_in_bytes = 4096,
    .pages_per_block = 16,
    .page_size_in_bytes = 256,
    .device_id = 0x0018BB20,
    .spinor_client_ref_count = 0,
    .power_on_delay_in_100us = 8,
    .dma_enable = TRUE,
    .reserved_1 = 0,
    .reserved_3 = 0,
  },
  { /* Macronix MX25R6435FM2IL0 / MX25R6435FM2IH0 8MB */
    .magic_number = 0xFACEECAF,
    .ddr_support = 0,
    .addr_bytes = 3,
    .read_mode = 2,
    .read_sdr_opcode = 0xEB,
    .read_ddr_opcode = 0,
    .read_sdr_wait_state = 6,
    .read_ddr_wait_state = 0,
    .write_mode = 2,
    .write_sdr_opcode = 0x38,
    .write_ddr_opcode = 0x00,
    .erase_4kb_opcode = 0x20,
    .bulk_erase_size_in_4KB = 0,
    .bulk_erase_opcode = 0,
    .quad_enable_mode = 2,
    .erase_err_bmsk = 0x40,
    .erase_err_status_reg = 0x2B,
    .write_err_bmsk = 0x20,
    .write_err_status_reg = 0x2B,
    .erase_status_polling_ms = 5,
    .read_sleep_time_in_10us = 40,
    .write_sleep_time_in_10us = 40,
    .read_page_count_between_sleep = 255,
    .write_page_count_between_sleep = 64,
    .write_status_polling_usec = 100,
    .freq_khz = 32000,
    .read_max_transfer_in_pages = 0x8000,
    .density_in_blocks = 2048,
    .block_size_in_bytes = 4096,
    .pages_per_block = 16,
    .page_size_in_bytes = 256,
    .device_id = 0x001728C2,
    .spinor_client_ref_count = 0,
    .power_on_delay_in_100us = 8,
    .dma_enable = TRUE,
    .reserved_1 = 0,
    .reserved_3 = 0,
  },
  { /* Winbond W25Q16FWSSIQ. 8MB */
    .magic_number = 0xFACEECAF,
    .ddr_support = 0,
    .addr_bytes = 3,
    .read_mode = 2,
    .read_sdr_opcode = 0xEB,
    .read_ddr_opcode = 0,
    .read_sdr_wait_state = 6,
    .read_ddr_wait_state = 0,
    .write_mode = 1,
    .write_sdr_opcode = 0x32,
    .write_ddr_opcode = 0x00,
    .erase_4kb_opcode = 0x20,
    .bulk_erase_size_in_4KB = 16,
    .bulk_erase_opcode = 0xD8,
    .quad_enable_mode = 1,
    .erase_err_bmsk = 0,
    .erase_err_status_reg = 0,
    .write_err_bmsk = 0,
    .write_err_status_reg = 0,
    .erase_status_polling_ms = 5,
    .read_sleep_time_in_10us = 40,
    .write_sleep_time_in_10us = 40,
    .read_page_count_between_sleep = 255,
    .write_page_count_between_sleep = 64,
    .write_status_polling_usec = 100,
    .freq_khz = 96000,
    .read_max_transfer_in_pages = 0x2000,
    .density_in_blocks = 2048,
    .block_size_in_bytes = 4096,
    .pages_per_block = 16,
    .page_size_in_bytes = 256,
    .device_id = 0x001760EF,
    .spinor_client_ref_count = 0,
    .power_on_delay_in_100us = 8,
    .dma_enable = TRUE,
    .reserved_1 = 0,
    .reserved_3 = 0,
  },
  { /* Winbond W25Q16FWSSIQ. 2MB */
    .magic_number = 0xFACEECAF,
    .ddr_support = 0,
    .addr_bytes = 3,
    .read_mode = 2,
    .read_sdr_opcode = 0xEB,
    .read_ddr_opcode = 0,
    .read_sdr_wait_state = 6,
    .read_ddr_wait_state = 0,
    .write_mode = 1,
    .write_sdr_opcode = 0x32,
    .write_ddr_opcode = 0x00,
    .erase_4kb_opcode = 0x20,
    .bulk_erase_size_in_4KB = 16,
    .bulk_erase_opcode = 0xD8,
    .quad_enable_mode = 1,
    .erase_err_bmsk = 0,
    .erase_err_status_reg = 0,
    .write_err_bmsk = 0,
    .write_err_status_reg = 0,
    .erase_status_polling_ms = 5,
    .read_sleep_time_in_10us = 40,
    .write_sleep_time_in_10us = 40,
    .read_page_count_between_sleep = 255,
    .write_page_count_between_sleep = 64,
    .write_status_polling_usec = 100,
    .freq_khz = 96000,
    .read_max_transfer_in_pages = 0x2000,
    .density_in_blocks = 512,
    .block_size_in_bytes = 4096,
    .pages_per_block = 16,
    .page_size_in_bytes = 256,
    .device_id = 0x001560EF,
    .spinor_client_ref_count = 0,
    .power_on_delay_in_100us = 8,
    .dma_enable = TRUE,
    .reserved_1 = 0,
    .reserved_3 = 0,
  },
  { /* Macronix MX25R1635FM2IH1. 2MB */
    .magic_number = 0xFACEECAF,
    .ddr_support = 0,
    .addr_bytes = 3,
    .read_mode = 2,
    .read_sdr_opcode = 0xEB,
    .read_ddr_opcode = 0,
    .read_sdr_wait_state = 6,
    .read_ddr_wait_state = 0,
    .write_mode = 2,
    .write_sdr_opcode = 0x38,
    .write_ddr_opcode = 0x00,
    .erase_4kb_opcode = 0x20,
    .bulk_erase_size_in_4KB = 0,
    .bulk_erase_opcode = 0,
    .quad_enable_mode = 2,
    .erase_err_bmsk = 0x40,
    .erase_err_status_reg = 0x2B,
    .write_err_bmsk = 0x20,
    .write_err_status_reg = 0x2B,
    .erase_status_polling_ms = 5,
    .read_sleep_time_in_10us = 40,
    .write_sleep_time_in_10us = 40,
    .read_page_count_between_sleep = 255,
    .write_page_count_between_sleep = 64,
    .write_status_polling_usec = 100,
    .freq_khz = 64000,
    .read_max_transfer_in_pages = 0x2000,
    .density_in_blocks = 512,
    .block_size_in_bytes = 4096,
    .pages_per_block = 16,
    .page_size_in_bytes = 256,
    .device_id = 0x001528C2,
    .spinor_client_ref_count = 0,
    .power_on_delay_in_100us = 8,
    .dma_enable = TRUE,
    .reserved_1 = 0,
    .reserved_3 = 0,
  },
  /* { Spansion S25FL128S. 16MB *//**
     .magic_number = 0xFACEECAF,
     .ddr_support = 0,
     .addr_bytes = 3,
     .read_mode = 0,
     .read_sdr_opcode = 0x0B,
     .read_ddr_opcode = 0,
     .read_sdr_wait_state = 8,
     .read_ddr_wait_state = 0,
     .write_mode = 0,
     .write_sdr_opcode = 0x02,
     .write_ddr_opcode = 0x00,
     .erase_4kb_opcode = 0x20,
     .bulk_erase_size_in_4KB = 16,
     .bulk_erase_opcode = 0xD8,
     .quad_enable_mode = 1,
     .erase_err_bmsk = 0x60,
     .erase_err_status_reg = 0x05,
     .write_err_bmsk = 0x60,
     .write_err_status_reg = 0x05,
     .erase_status_polling_ms = 5,
     .read_sleep_time_in_10us = 40,
     .write_sleep_time_in_10us = 40,
     .read_page_count_between_sleep = 255,
     .write_page_count_between_sleep = 64,
     .write_status_polling_usec = 100,
     .freq_khz = 96000,
     .read_max_transfer_in_pages = 0x10000,
     .density_in_blocks = 4096,
     .block_size_in_bytes = 4096,
     .pages_per_block = 16,
     .page_size_in_bytes = 256,
     .device_id = 0x00200201,
     .spinor_client_ref_count = 0,
     .power_on_delay_in_100us = 8,
     .dma_enable = TRUE,
     .reserved_1 = 0,
     .reserved_3 = 0,
     },**/
  { /* Micron MT25QU512ABA. 512MB */
    .magic_number = 0xFACEECAF,
    .ddr_support = 0,
    .addr_bytes = 4,
    .read_mode = 2,
    .read_sdr_opcode = 0xEB,
    .read_ddr_opcode = 0,
    .read_sdr_wait_state = 10,
    .read_ddr_wait_state = 0,
    .write_mode = 1,
    .write_sdr_opcode = 0x32,
    .write_ddr_opcode = 0x00,
    .erase_4kb_opcode = 0x20,
    .bulk_erase_size_in_4KB = 16,
    .bulk_erase_opcode = 0xD8,
    .quad_enable_mode = 0,
    .erase_err_bmsk = 0x7E,
    .erase_err_status_reg = 0x70,
    .write_err_bmsk = 0x7E,
    .write_err_status_reg = 0x70,
    .erase_status_polling_ms = 5,
    .read_sleep_time_in_10us = 40,
    .write_sleep_time_in_10us = 40,
    .read_page_count_between_sleep = 255,
    .write_page_count_between_sleep = 64,
    .write_status_polling_usec = 100,
    .freq_khz = 96000,
    .read_max_transfer_in_pages = 0x10000,
    .density_in_blocks = 0xFFFE,
    .block_size_in_bytes = 4096,
    .pages_per_block = 16,
    .page_size_in_bytes = 256,
    .device_id = 0x0020BA20,
    .spinor_client_ref_count = 0,
    .power_on_delay_in_100us = 8,
    .dma_enable = TRUE,
    .reserved_1 = 0,
    .reserved_3 = 0,
  },
};



/**********************************************************
 * Get the pointer to the supported NOR devices and their parameters
 *
 * @return void* [OUT]
 *   Pointer to the table that contains the Flash's NOR config parameters
 *   needed for spinor operations
 *
 **********************************************************/
void* spinor_get_config_entries_struct(void)
{
  return (void *)&spinor_nor_entries[0];
}

/**********************************************************
 * Get the number of entries in the table
 *
 * @param total_nor_entries [OUT]
 *   Total number of entries in the table
 *
 * @return int [OUT]
 *   Result of the operation.
 *
 **********************************************************/
SPINOR_STATUS spinor_get_config_entries_count(uint32 *total_nor_entries)
{
  uint32 total_entries;

  total_entries = (sizeof(spinor_nor_entries) /
                   sizeof(spinor_nor_entries[0]));

  *total_nor_entries = total_entries;

  return SPINOR_DEVICE_DONE;
}


/**********************************************************
 * Get the pointer to the supported NOR devices and their parameters
 *
 * @return void* [OUT]
 *   Pointer to the table that contains the Flash's NOR config parameters
 *   needed for spinor operations
 *
 **********************************************************/
void* spinor_get_config_table_struct(void)
{
  return (void *)&spinor_cfg_table;
}


