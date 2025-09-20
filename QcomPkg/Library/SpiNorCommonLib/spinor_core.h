#ifndef __SPINOR_CORE__H
#define __SPINOR_CORE__H
/**********************************************************************
 * spinor_core.h
 *
 * This file provides definitions for the spinor support functions
 *
 * Copyright (c) 2015-2017
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
/*#include "spinor_i.h"*/
#include "spinor_init_config.h"



/** Spinor Core operation status */
#define SPINOR_CORE_DONE                  0  /**< Operation passed */
#define SPINOR_CORE_FAIL                (-1) /**< Operation failed */
#define SPINOR_CORE_NOT_SUPPORTED       (-2) /**< Device/operation not supported */
#define SPINOR_CORE_INVALID_PARAMETER   (-3) /**< API parameters invalid */

typedef int SPINOR_CORE_STATUS;              /**< Error status values used in SPINOR driver */

/* SFDP's Dword 15th Bits[20:22]. QER Field.*/
#define  ENABLE_QUAD_MODE_0  0x0
#define  ENABLE_QUAD_MODE_1  0x1
#define  ENABLE_QUAD_MODE_2  0x2
#define  ENABLE_QUAD_MODE_3  0x3
#define  ENABLE_QUAD_MODE_4  0x4
#define  ENABLE_QUAD_MODE_5  0x5
#define  ENABLE_QUAD_MODE_6  0x6
#define  ENABLE_QUAD_MODE_7  0x7

#define PROG_ERASE_WRITE_BUSY_BMSK   0x01
#define READ_STATUS_BUSY_MASK        0x01
#define STATUS_WR_EN_MASK            0x02 /* Write Enable Latch status */

#define READ_SDR_1_1_1_MODE       0
#define READ_SDR_1_1_4_MODE       1
#define READ_SDR_1_4_4_MODE       2
#define READ_SDR_4_4_4_MODE       3
#define READ_DDR_1_1_4_MODE       4
#define READ_DDR_1_4_4_MODE       5
#define READ_DDR_4_4_4_MODE       6

#define WRITE_SDR_1_1_1_MODE       0
#define WRITE_SDR_1_1_4_MODE       1
#define WRITE_SDR_1_4_4_MODE       2
#define WRITE_SDR_4_4_4_MODE       3
#define WRITE_DDR_1_1_4_MODE       4
#define WRITE_DDR_1_4_4_MODE       5
#define WRITE_DDR_4_4_4_MODE       6

#define READ_STATUS_CMD       0x05
#define READ_STATUS_2_CMD     0x3F
#define WRITE_STATUS_2_CMD    0x3E
#define WRITE_ENABLE_CMD      0x06
#define READ_ID_CMD           0x9F
#define READ_SFDP_CMD         0x5A
#define READ_CFG1_CMD         0x35        /* Micron/Spansion: Enable QE */
#define READ_FLAG_STATUS_CMD  0x70
#define READ_SECURITY_CMD     0x2B
#define READ_SFDP_CMD         0x5A
#define WRITE_STATUS_CMD      0x01
#define ENTER_4B_ADDR_CMD     0xB7
#define READ_CFG_REG_CMD      0x15

#define MAX_PAGE_PER_WRITE    1
#define WRITE_TIMEOUT         5000000   /*usec unit */
#define ERASE_TIMEOUT         25000000   /*usec unit */
#define READ_TIMEOUT          5000000   /*usec unit */
#define READ_STATUS_TIMEOUT   5000000   
#define READ_STATUS_POLLING_USEC  20

#define DEVICE_ID_LEN_IN_BYTES   3

#define BIT_0  0x1
#define BIT_1  0x2
#define BIT_2  0x4
#define BIT_3  0x8
#define BIT_4  0x10
#define BIT_5  0x20
#define BIT_6  0x40
#define BIT_7  0x80

/*Default 4KB Block, 256 bytes per page */
#define PAGES_PER_BLOCK  16 
#define PAGE_SIZE_IN_BYTES   256
#define BLOCK_SIZE_IN_BYTES  4096

/* Check for integer overflows */
#define INT_OVERFLOW(x,y) (x + y < x)

#define SPINOR_16MB_IN_BYTES   0x1000000
#define SPINOR_16MB_IN_BLOCKS  0x1000
#define SPINOR_DEVICE_MEM_START_ADDR 0x000

/* Error Check operation types */
#define ERASE_OPERATION   1
#define WRITE_OPERATION   2

#define DEFAULT_QSPI_SDR_8MHZ   8000
#define SPINOR_QSPI_SDR_96MHZ    96000
#define SPINOR_QSPI_DDR_64MHZ    64000

#define SPINOR_ERR_LOG_SIZE  16

/* Flash Error Log */
typedef struct
{
  uint8  spinor_err_index;                        
  uint16  spinor_err_codes[SPINOR_ERR_LOG_SIZE];
  uint32 read_unaligned_count;
  uint32 write_unaligned_count;
  uint32 spinor_status;
  uint32 vendor_status;
} spinor_logs;

typedef struct
{
  boolean nor_init_done;
  uint32  erase_timeout;
  uint32  write_timeout;
  uint32  read_status_timeout;
  uint16  status_polling_usec;
  boolean dma_enable;
  uint8 ref_count;
  spinor_logs debug_log;
  spinor_config_data *config_data;  
}spinor_drv_info;

extern spinor_drv_info spinor_dev;

#ifndef TRUE
#define TRUE   1   /* Boolean true value. */
#endif

#ifndef FALSE
#define FALSE  0   /* Boolean false value. */
#endif

#ifndef NULL
#define NULL  0
#endif

/**********************************************************
 * Look up RAM table to find the spinor config info. 
 * Save the data in spinor's RAM AON
 *
 * @param spinor_config_tbl [IN]
 *   RAM table containing config data for supported spinor devices
 *
 * @return int [OUT]
 *   Result of the operation.
 *
 **********************************************************/
 SPINOR_STATUS spinor_device_lookup (void *spinor_config_tbl);

/**********************************************************
 * Issues WRITE_ENABLE command to SPI NOR device
 *
 * @result [OUT]
 *   returns the Read Status Register content after WRITE_ENABLE 
 *
 * @return int [OUT]
 *   Result of the operation.
 *
 **********************************************************/
 SPINOR_CORE_STATUS write_enable (uint8 *result);

/**********************************************************
 * Enables Quad Mode
 *
 * @qer_mode [IN]
 *   Mode is queried from the sfdp's 15th DWORD. Depending on the mode,
 *   different registers are used to enable the Quad Mode.
 *   Refer to the Standard JEDEC 216A Section 6.4.15 for details.
 *
 * @reg_value [OUT]
 *   Register Value
 *
 * @return int [OUT]
 *   Result of the operation.
 *
 **********************************************************/
 SPINOR_CORE_STATUS enable_quad_mode (uint8 qer_mode);

/**********************************************************
 * Sets 4-byte address mode
 *
 * @return int [OUT]
 *   Result of the operation.
 *
 **********************************************************/
 SPINOR_CORE_STATUS set_4byte_address_mode(void);

/**********************************************************
 * Read the Flash device ID
 *
 * @param device_id [IN/OUT]
 *   Flash NOR Device ID
 *
 * @param device_id_len [IN]
 *   Number of bytes to read
 *
 * @return int [OUT]
 *   Result of the operation.
 *
 **********************************************************/
 SPINOR_CORE_STATUS spinor_core_read_device_id (void *device_id, uint8 device_id_len);

/**********************************************************
 * Erases given spinor blocks
 *
 * @param start_block [IN]
 *   A starting block for a number of blocks to erase.
 *
 * @param block_count [IN]
 *   Number of blocks to erase
 *
 * @return int [OUT]
 *   Result of the operation.
 *
 **********************************************************/
 SPINOR_CORE_STATUS spinor_core_erase (uint32 start_block, uint32 block_count);

/**********************************************************
 * Checks Status of the operation by reading the Status register
 *
 * @result [OUT]
 *   Status Register Value
 *
 * @return int [OUT]
 *   Result of the operation.
 *
 **********************************************************/
 SPINOR_CORE_STATUS check_operation_status (uint8 *result);

/**********************************************************
 * De-initializes the spinor nor driver
 *
 * @return int [OUT]
 *   Result of the operation.
 *
 **********************************************************/
 SPINOR_CORE_STATUS spinor_core_deinit (void);

/**********************************************************
 * Erases given spinor blocks
 *
 * @param start_block [IN]
 *   A starting block for a number of blocks to erase.
 *
 * @param block_count [IN]
 *   Number of blocks to erase
 *
 * @return int [OUT]
 *   Result of the operation.
 *
 **********************************************************/
 SPINOR_CORE_STATUS spinor_core_erase(uint32 start_block, uint32 block_count);

/**********************************************************
 * Erases given spinor blocks in bulk erase blocks size (32KB, 64KB, 512KB ect.
 * or whichever the memory device supports)
 * The following requirments must be met.
 * - The start_block must align to the bulk erase blocks size.
 * - The block_count must align to the bulk erase blocks size.
 *
 * @param start_block [IN]
 *   A starting block for a number of blocks to erase.
 *
 * @param block_count [IN]
 *   Number of blocks to erase
 *
 * @return int [OUT]
 *   Result of the operation.
 *
 **********************************************************/
 SPINOR_CORE_STATUS spinor_core_erase_bulk(uint32 start_block, uint32 block_count);

/**********************************************************
 * Allows write data to the spinor.
 *
 * @param address [IN]
 *   Physical location of the start of the write
 *
 * @param byte_count [IN]
 *   Number of bytes to write data.
 *
 * @buffer [IN]
 *   Data buffer for spinor write operation.
 *
 * @return int [OUT]
 *   Result of the operation.
 *
 **********************************************************/
 SPINOR_CORE_STATUS spinor_core_write(uint32 address, uint32 byte_count, uint8 *buffer);

/**********************************************************
 * Allows read data from the spinor.
 *
 * @param address [IN]
 *   Physical location of the start of the read
 *
 * @param byte_count [IN]
 *   Number of bytes to read data.
 *
 * @buffer [IN]
 *   Data buffer for spinor write operation.
 *
 * @return int [OUT]
 *   Result of the operation.
 *
 **********************************************************/
 SPINOR_CORE_STATUS spinor_core_read(uint32 address, uint32 byte_count, uint8 *buffer);

/**********************************************************
 * Initializes the spinor driver info structure.
 *
 * @return int [OUT]
 *   Result of the operation.
 *
 *********************************************************/
 SPINOR_STATUS spinor_drv_info_init (void);

/**********************************************************
 * Initializes the spinor controller HW
 *
 * @return int [OUT]
 *   Result of the operation.
 *
 **********************************************************/
 SPINOR_CORE_STATUS spinor_controller_init (void);

/**********************************************************
 * Initializes the spinor driver.
 * Invokes spinor_core_init() to initialize spinor sub modules
 * 
 * @param spinor_config_table [IN]
 *   RAM table containing config data for supported spinor devices
 *  
 * @return int [OUT]
 *   Result of the operation.
 *
 **********************************************************/
 SPINOR_STATUS spinor_init(void *spinor_config_table);

/**********************************************************
 * De-initializes the spinor driver.
 * Free up resources used by the spinor driver
 *
 * @return int [OUT]
 *   Result of the operation.
 *
**********************************************************/
 SPINOR_STATUS spinor_deinit (void);






#endif /* __SPINOR_CORE__H */
