/**********************************************************************
 * spinor_core.c
 *
 * Implementation of spinor driver
 *
 * Copyright (c) 2016-2017
 * Qualcomm Technologies Incorporated.
 * All Rights Reserved.
 * Qualcomm Confidential and Proprietary
 *
 **********************************************************************/
/*=======================================================================
                        Edit History


when        who       what, where, why
----------  -----     ---------------------------------------------------
2017-05-25  md        Initial Release


======================================================================*/
#include "spinor_api.h"
#include "spinor_core.h"
#include "quadspi.h"
#include "spinor_utils.h"
#include "spinor_osal.h"
#include "spinor_init_config.h"
#include "spinor_spial.h"

spinor_config_data spinor_core_config_data;

static SPINOR_CORE_STATUS enable_quad_mode_3 (void);
static SPINOR_CORE_STATUS enable_quad_mode_1_4_5 (void);
static SPINOR_CORE_STATUS enable_quad_mode_2 (void);
static SPINOR_CORE_STATUS vendor_error_check (uint8 operation_type);



/**********************************************************
 * Read the spinor device ID
 *
 * @param device_id [IN/OUT]
 *   spinor Device ID
 *
 * @param device_id_len [IN]
 *   Number of bytes to read
 *
 * @return int [OUT]
 *   Result of the operation.
 *
 **********************************************************/
 SPINOR_CORE_STATUS spinor_core_read_device_id (void *device_id, uint8 device_id_len)
 {
    SPINOR_CORE_STATUS status = SPINOR_CORE_DONE;

    memset(device_id, 0, device_id_len);
    status = read_spi(READ_ID_CMD, device_id_len, (uint8 *)device_id);
    return status;
 }

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
 SPINOR_CORE_STATUS spinor_core_erase (uint32 start_block, uint32 block_count)
 {
    uint32 address = 0;
    uint8 qspi_status = 0;
    SPINOR_CORE_STATUS status = SPINOR_CORE_FAIL;
    uint32 timeout = 0;
    uint32 status_polling_ms = 0;
    SPI_COMMAND_TYPE command_mode = qspi_erase_mode;
    
    status_polling_ms = spinor_dev.config_data->erase_status_polling_ms;
    address = start_block * spinor_dev.config_data->block_size_in_bytes;

    while (block_count)
    {
      status = write_enable (&qspi_status);
      if (status != SPINOR_CORE_DONE)
      {
        spinor_dev.debug_log.spinor_status = qspi_status;      
        spinor_log (&spinor_dev.debug_log, 0x200);
        goto spinor_core_erase_end;
      }

      if (SPINOR_CORE_DONE != spial_transfer(command_mode, address, NULL, 0,0))
      {
       status = SPINOR_CORE_FAIL;
       spinor_log (&spinor_dev.debug_log, 0x201);         
       goto spinor_core_erase_end;
      }

      status = SPINOR_CORE_FAIL;

      timeout = spinor_dev.erase_timeout;
      while (timeout > 0) 
      {
        /* delay between each status query */
        spinor_udelay (status_polling_ms * 1000);
        timeout -= (status_polling_ms * 1000);

        (void)read_spi(READ_STATUS_CMD, 1, &qspi_status);
        if ((qspi_status & PROG_ERASE_WRITE_BUSY_BMSK) == 0)
        { 
          status = vendor_error_check(ERASE_OPERATION);
          if (status == SPINOR_CORE_DONE)
          {
            /* No error. Break out of "while" loop */
            break;
          }
          else
          {
            spinor_dev.debug_log.spinor_status = qspi_status;
            spinor_log (&spinor_dev.debug_log, 0x202);
            goto spinor_core_erase_end;
          }
        }
      }

      if (status != SPINOR_CORE_DONE)
      {
        spinor_dev.debug_log.spinor_status = qspi_status;
        spinor_log (&spinor_dev.debug_log, 0x203);
        status = SPINOR_CORE_FAIL;
        goto spinor_core_erase_end;
      }

      address += spinor_dev.config_data->block_size_in_bytes;
      block_count--;
  }

    spinor_core_erase_end:
    return status;
}

/**********************************************************
 * Checks any vendor specific error
 *
 * @operation_type [IN]
 *   Status from reading the Status Register of prior operation
 *
 * @return int [OUT]
 *   Result of the operation.
 *
 **********************************************************/
 static SPINOR_CORE_STATUS vendor_error_check (uint8 operation_type)
 {
    uint8 status_mask = 0;
    uint8 err_status_register = 0;
    uint8 qspi_status = 0;   
    SPINOR_CORE_STATUS status = SPINOR_CORE_FAIL;

    switch (operation_type)
    {
      case WRITE_OPERATION:
      status_mask = spinor_dev.config_data->write_err_bmsk;  
      err_status_register = spinor_dev.config_data->write_err_status_reg; 
      break;

      case ERASE_OPERATION:
      status_mask = spinor_dev.config_data->erase_err_bmsk;  
      err_status_register = spinor_dev.config_data->erase_err_status_reg;   
      break;
      
      default:
      status = SPINOR_CORE_FAIL;
      spinor_log (&spinor_dev.debug_log, 0x210);
      goto vendor_error_check_done;  
    }

    if (err_status_register == 0)
    {
        /* No status error check required */
      status = SPINOR_CORE_DONE;    
      goto vendor_error_check_done;
    }

    status = read_spi(err_status_register, 1, &qspi_status);

      /* Check device status */
    if ((status == SPINOR_CORE_DONE) && (qspi_status & status_mask))
    {
      status = SPINOR_CORE_FAIL;
      spinor_dev.debug_log.vendor_status = (qspi_status & status_mask);
      spinor_log (&spinor_dev.debug_log, 0x211);
    }

    vendor_error_check_done:
    return status;
}

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
 SPINOR_CORE_STATUS spinor_core_write (uint32 address,
   uint32 byte_count, uint8 *buffer)
 {
    SPINOR_CORE_STATUS status = SPINOR_CORE_FAIL;
    int32 timeout = 0;
    uint32 transfer_size = 0;
    uint32 total_bytes_written = 0;  
    uint8 qspi_status = 0;
    uint32 limit = spinor_dev.config_data->page_size_in_bytes;
    uint32 status_polling_usec = spinor_dev.config_data->write_status_polling_usec;
    SPI_COMMAND_TYPE command_mode = qspi_write_mode;

    if (((uint32)buffer & 0x1F) || (byte_count & 0x3))
    {
      /* Keep a count of unalinged access for system analysis purpose */
      spinor_dev.debug_log.write_unaligned_count++;
    }

    while (byte_count)
    {
      /* In case the start address is not on page boundary, we write from the */
      /* start address to the end of the page as a separate write transaction */
      if (address % spinor_dev.config_data->page_size_in_bytes)
      {
        transfer_size = spinor_dev.config_data->page_size_in_bytes - (address % spinor_dev.config_data->page_size_in_bytes);
        if (transfer_size > byte_count)
        {
          transfer_size = byte_count;
        }
      }
      else
      {
        transfer_size = (byte_count > limit) ? (limit) : (byte_count);
      }
      
      status = write_enable (&qspi_status);
      if (status != SPINOR_CORE_DONE)
      {
        spinor_dev.debug_log.spinor_status = qspi_status;      
        spinor_log (&spinor_dev.debug_log, 0x221);
        goto spinor_core_write_end;      
      }

      /* Command | data addr | data buffer | length | opcode */    
      
      if (SPINOR_CORE_DONE != spial_transfer(command_mode, address, buffer, transfer_size,0))
      {
        status = SPINOR_CORE_FAIL;
        spinor_log (&spinor_dev.debug_log, 0x222);         
        goto spinor_core_write_end;
      }

      total_bytes_written += transfer_size;

      status = SPINOR_CORE_FAIL;

      timeout =  spinor_dev.write_timeout;
      while (timeout > 0)
      {
        read_spi(READ_STATUS_CMD, 1, &qspi_status);
        if ((qspi_status & PROG_ERASE_WRITE_BUSY_BMSK) == 0)
        {
          status = vendor_error_check(WRITE_OPERATION);   
          if (status == SPINOR_CORE_DONE)
          {
             /* No error. Break out of "while" loop */
            break;
          }

          spinor_dev.debug_log.spinor_status = qspi_status;
          spinor_log (&spinor_dev.debug_log, 0x223);
          goto spinor_core_write_end;
        }

        spinor_udelay(status_polling_usec);
        timeout -= status_polling_usec;
      }

      if (status != SPINOR_CORE_DONE)
      {
        spinor_dev.debug_log.spinor_status = qspi_status;
        spinor_log (&spinor_dev.debug_log, 0x224);
        status = SPINOR_CORE_FAIL;
        goto spinor_core_write_end;
      }

      address += transfer_size;
      buffer += transfer_size;
      byte_count -= transfer_size;
  }

    status = SPINOR_CORE_DONE;

    spinor_core_write_end:
    return status;
}

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
 SPINOR_CORE_STATUS spinor_core_read (uint32 address,
   uint32 byte_count, uint8 *buffer)
 {
    SPINOR_CORE_STATUS status = SPINOR_CORE_DONE;
    uint32 max_transfer_size = 0;
    uint32 transfer_size = 0;
    uint8 *temp_buffer;
    SPI_COMMAND_TYPE command_mode = qspi_read_mode;
    

    if (((uint32)buffer & 0x1F) || (byte_count & 0x3))
    {
      /* Keep a count of unalinged access for system analysis purpose */
      spinor_dev.debug_log.read_unaligned_count++;
    }

    /* The max transfer size is configurable by OEM when XIP is enabled. */
    /* When XIP is not enabled, read max transfer size is the memory size */
    max_transfer_size = PAGE_SIZE_IN_BYTES * spinor_dev.config_data->read_max_transfer_in_pages;

    temp_buffer = buffer;

    while (byte_count > 0)
    {
      transfer_size = (byte_count > max_transfer_size) ?
      max_transfer_size : byte_count;

    /* Command | data addr | data buffer | length | opcode */   

      if (SPINOR_CORE_DONE != spial_transfer(command_mode, address,temp_buffer,transfer_size,0))
      {
        status = SPINOR_CORE_FAIL;
        spinor_log (&spinor_dev.debug_log, 0x231);
        goto spinor_core_read_end;
      }    
      
      address += transfer_size;
      byte_count -= transfer_size;
      temp_buffer = temp_buffer + transfer_size;
    }
    spinor_core_read_end:
    return status;
 }

/* Local functions */

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
 SPINOR_CORE_STATUS spinor_core_erase_bulk(uint32 start_block, uint32 block_count)
 {
    uint32 address = 0;
    uint8 qspi_status = 0;
    SPINOR_CORE_STATUS status = SPINOR_CORE_FAIL;
    int32 timeout = 0;
    uint8 timeout_status = 0;
    uint32 status_polling_ms = 0;
    SPI_COMMAND_TYPE command_mode = qspi_erase_bulk_mode;
    
    if ((spinor_dev.config_data->bulk_erase_opcode == 0) ||
      (spinor_dev.config_data->bulk_erase_size_in_4KB == 0))
    {
      spinor_log (&spinor_dev.debug_log, 0x240);    
      return SPINOR_CORE_INVALID_PARAMETER;
    }

    /* physical starting address must align to the bulk erase blocks size */
    /* block_count must be a multiple of bulk erase blocks size */
    if ((start_block % spinor_dev.config_data->bulk_erase_size_in_4KB) || 
      (block_count % spinor_dev.config_data->bulk_erase_size_in_4KB))
    {
      spinor_log (&spinor_dev.debug_log, 0x241);    
      return SPINOR_CORE_INVALID_PARAMETER;
    }

    status_polling_ms = spinor_dev.config_data->erase_status_polling_ms;
    
    address = start_block * spinor_dev.config_data->block_size_in_bytes;

    while (block_count)
    {
      status = write_enable (&qspi_status);
      if (status != SPINOR_CORE_DONE)
      {
        spinor_dev.debug_log.spinor_status = qspi_status;      
        spinor_log (&spinor_dev.debug_log, 0x242);
        goto spinor_core_erase_bulk_end;
      }
      /* Command | data addr | data buffer | length | opcode */   
      if (SPINOR_CORE_DONE != spial_transfer(command_mode, address, NULL, 0,0))
      {
        status = SPINOR_CORE_FAIL;
        spinor_log (&spinor_dev.debug_log, 0x243);     
        goto spinor_core_erase_bulk_end;
      }

      status = SPINOR_CORE_FAIL;

      timeout = spinor_dev.erase_timeout;
      while (timeout > 0) 
      {
        /* delay between each status query */
        spinor_udelay (status_polling_ms * 1000);
        timeout -= (status_polling_ms * 1000);

        (void)read_spi(READ_STATUS_CMD, 1, &qspi_status);
        if ((qspi_status & PROG_ERASE_WRITE_BUSY_BMSK) == 0)
        { 
          status = vendor_error_check(ERASE_OPERATION);
          if (status == SPINOR_CORE_DONE)
          {
            /* No error. Break out of "while" loop */
            break;
          }
          else
          {
            spinor_dev.debug_log.spinor_status = qspi_status;
            spinor_log (&spinor_dev.debug_log, 0x244);
            goto spinor_core_erase_bulk_end;
          }
        }
      } /* (timeout > 0) */
      if (timeout == 0)
        {
          timeout_status = 1;
        }
        if (status != SPINOR_CORE_DONE)
        {
          spinor_dev.debug_log.spinor_status = qspi_status;
          spinor_log (&spinor_dev.debug_log, 0x245);
          status = SPINOR_CORE_FAIL;
          goto spinor_core_erase_bulk_end;
        }

      address += (spinor_dev.config_data->bulk_erase_size_in_4KB * spinor_dev.config_data->block_size_in_bytes);
      block_count -= spinor_dev.config_data->bulk_erase_size_in_4KB;
    } /* (block_count > 0) */

    spinor_core_erase_bulk_end:
    return status;
 }

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
 SPINOR_CORE_STATUS write_enable (uint8 *result)
 {
    SPINOR_CORE_STATUS status = SPINOR_CORE_FAIL;
    uint32 timeout = 0;
    SPI_COMMAND_TYPE command_mode = qspi_write_enable_mode;
    
    
    if (SPINOR_CORE_DONE != spial_transfer(command_mode, 0, NULL, 0,0))
    {
     status = SPINOR_CORE_FAIL;
     spinor_log (&spinor_dev.debug_log, 0x250);      
     goto spinor_write_enable_end;
    }

   status = SPINOR_CORE_FAIL;

   timeout = spinor_dev.read_status_timeout;
   while (timeout > 0)
   {
      /* Read 1 byte Status register. command 0x5 */
      status = read_spi(READ_STATUS_CMD, 1, result);
      if ((status == SPINOR_CORE_DONE) && (*result & STATUS_WR_EN_MASK))
      {   
        break;
      }

      spinor_udelay(spinor_dev.status_polling_usec);    
      timeout -= spinor_dev.status_polling_usec;
   }

   if (status != SPINOR_CORE_DONE)
   {
     spinor_log (&spinor_dev.debug_log, 0x251);
   }
   spinor_write_enable_end:
   return status;
}

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
 SPINOR_CORE_STATUS enable_quad_mode (uint8 qer_mode)
 {
    SPINOR_CORE_STATUS status = SPINOR_CORE_DONE;

    switch (qer_mode)
    {
      case ENABLE_QUAD_MODE_0:
        /* Device does not have QE bit. Device detects 1-1-4 and 1-4-4 modes */
        /* based on instruction */
      break;

      case ENABLE_QUAD_MODE_1:
      case ENABLE_QUAD_MODE_4:
      case ENABLE_QUAD_MODE_5:
        /* Break big funtion into smaller ones */
      status = enable_quad_mode_1_4_5();
      break;
      
      case ENABLE_QUAD_MODE_2:
      status = enable_quad_mode_2();
      break;

      case ENABLE_QUAD_MODE_3:
      status = enable_quad_mode_3();
      break;

      case ENABLE_QUAD_MODE_6:
      case ENABLE_QUAD_MODE_7:
      default:
      status = SPINOR_CORE_FAIL;
      break;
    }

    return status;
  }

/**********************************************************
 * Enables Quad Mode. Mode 1,4,5 
 *
 * @return int [OUT]
 *   Result of the operation.
 *
 **********************************************************/
 static SPINOR_CORE_STATUS enable_quad_mode_1_4_5 (void)
 {
    uint8 status_reg1 = 0, status_reg2 = 0;
    uint8 temp[2];
    SPINOR_CORE_STATUS status = SPINOR_CORE_DONE;

    /* Read/modify/write operation.*/ 
    (void)read_spi(READ_STATUS_CMD, 1, &status_reg1);
    (void)read_spi(READ_CFG1_CMD, 1, &status_reg2);

    if (status_reg2 & BIT_1)
    {
      /* Quad Mode is already enabled */
      goto enable_quad_mode_1_4_5_end; 
    }

    /* Set Bit1 of status register2 to enable Quad mode (QER Bit) */
    /* Read/Modify/Write 2 bytes back */
    status_reg2 |= BIT_1;
    temp[0] = status_reg1;
    temp[1] = status_reg2;

    status = write_spi(WRITE_STATUS_CMD, 2, &temp[0]);
    if (status != SPINOR_CORE_DONE)
    {
      spinor_log (&spinor_dev.debug_log, 0x260);
      goto enable_quad_mode_1_4_5_end;        
    }
    
    status = check_operation_status(&temp[0]);
    if (status != SPINOR_CORE_DONE)
    {
      spinor_dev.debug_log.spinor_status = temp[0];
      spinor_log (&spinor_dev.debug_log, 0x261);
      goto enable_quad_mode_1_4_5_end;        
    }

    enable_quad_mode_1_4_5_end:
    return status;
}

/**********************************************************
 * Enables Quad Mode. Mode 2
 *
 * @return int [OUT]
 *   Result of the operation.
 *
 **********************************************************/
 static SPINOR_CORE_STATUS enable_quad_mode_2 (void)
 {
    uint8 temp = 0;
    uint8 status_reg = 0;
    SPINOR_CORE_STATUS status = SPINOR_CORE_DONE;

    (void)read_spi(READ_STATUS_CMD, 1, &status_reg);

    /* In Mode 2, Bit6 of status register is used to enable Quad mode */
    if (status_reg & BIT_6)
    {
      /* Quad Mode is already enabled */
      goto enable_quad_mode_2_end; 
    }

    /* Set Bit6 of status register to enable Quad mode (QER Bit) */
    status_reg |= BIT_6;

    status = write_spi(WRITE_STATUS_CMD, 1, &status_reg);
    if (status != SPINOR_CORE_DONE)
    {
      spinor_log (&spinor_dev.debug_log, 0x270);
      goto enable_quad_mode_2_end;        
    }
    
    status = check_operation_status(&temp);
    if (status != SPINOR_CORE_DONE)
    {
      spinor_dev.debug_log.spinor_status = temp;
      spinor_log (&spinor_dev.debug_log, 0x271);
      goto enable_quad_mode_2_end;        
    }

    enable_quad_mode_2_end:
    return status;
}

/**********************************************************
 * Enables Quad Mode for Mode 3
 *
 * @return int [OUT]
 *   Result of the operation.
 *
 **********************************************************/
 static SPINOR_CORE_STATUS enable_quad_mode_3 (void)
 {
    uint8 temp = 0;
    uint8 status2_reg = 0;
    SPINOR_CORE_STATUS status = SPINOR_CORE_DONE;

    /* Read 1 byte Status2 register with command 0x3F */
    (void)read_spi(READ_STATUS_2_CMD, 1, &status2_reg);

    /* In Mode 3, Bit7 of Status2 register is used to enable Quad mode */
    if (status2_reg & BIT_7)
    {
      /* Quad Mode is already enabled */
      goto enable_quad_mode_3_end; 
    }

    status = write_enable(&temp);
    if (status != SPINOR_CORE_DONE)
    {
      spinor_dev.debug_log.spinor_status = temp;      
      spinor_log (&spinor_dev.debug_log, 0x280);
      goto enable_quad_mode_3_end;
    }

    /* Set Bit7 of Status2 to enable Quad mode (QER Bit) */
    status2_reg |= BIT_7;

    /* Read/Modify/Write Status2 Register */
    status = write_spi(WRITE_STATUS_2_CMD, 1, &status2_reg);
    if (status != SPINOR_CORE_DONE)
    {
      spinor_log (&spinor_dev.debug_log, 0x281);
      goto enable_quad_mode_3_end;        
    }
    
    status = check_operation_status(&temp);
    if (status != SPINOR_CORE_DONE)
    {
      spinor_dev.debug_log.spinor_status = temp;
      spinor_log (&spinor_dev.debug_log, 0x282);
      goto enable_quad_mode_3_end;        
    }

    enable_quad_mode_3_end:
    return status;
}

/**********************************************************
 * Sets 4-byte address mode
 *
 * @return int [OUT]
 *   Result of the operation.
 *
 **********************************************************/
 SPINOR_CORE_STATUS set_4byte_address_mode(void)
 {
    SPINOR_CORE_STATUS status = SPINOR_CORE_DONE;
    uint8 temp = 0;

    status = write_enable(&temp);
    if (status != SPINOR_CORE_DONE)
    {
      spinor_dev.debug_log.spinor_status = temp;      
      spinor_log (&spinor_dev.debug_log, 0x290);
      goto set_4byte_address_mode_end;
    }

    /* This command has no data phase. Only write the command */
    status = write_spi(ENTER_4B_ADDR_CMD, 0, &temp);
    if (status != SPINOR_CORE_DONE)
    {
      spinor_log (&spinor_dev.debug_log, 0x291);
      goto set_4byte_address_mode_end;        
    }

    set_4byte_address_mode_end:
    return status;
}

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
 SPINOR_CORE_STATUS check_operation_status (uint8 *result)
 {
    int32 timeout = 0;
    SPINOR_CORE_STATUS status = SPINOR_CORE_FAIL;

    timeout = spinor_dev.read_status_timeout;
    while (timeout > 0)
    {
      /* Read 1 byte Status register */
      (void)read_spi(READ_STATUS_CMD, 1, result);
      if (!(*result & READ_STATUS_BUSY_MASK))
      {
        status = SPINOR_CORE_DONE;
        goto check_operation_status_end;
      }

      timeout -= spinor_dev.status_polling_usec;
      spinor_udelay(spinor_dev.status_polling_usec);
    }

    if (status != SPINOR_CORE_DONE)
    {
      spinor_log (&spinor_dev.debug_log, 0x2A0);
    }

    check_operation_status_end:
    return status;
}

/**********************************************************
 * Initializes the spinor driver info structure.
 *
 * @return int [OUT]
 *   Result of the operation.
 *
 *********************************************************/
 SPINOR_STATUS spinor_drv_info_init (void)
 {
    spinor_dev.nor_init_done = FALSE;
    spinor_dev.erase_timeout = ERASE_TIMEOUT;
    spinor_dev.write_timeout = WRITE_TIMEOUT;
    spinor_dev.read_status_timeout = READ_STATUS_TIMEOUT;
    spinor_dev.status_polling_usec = READ_STATUS_POLLING_USEC;
    spinor_dev.config_data = &spinor_core_config_data;

    return SPINOR_DEVICE_DONE;
}


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
 SPINOR_STATUS spinor_init(void *spinor_config_table)
 {
    SPINOR_STATUS status = SPINOR_DEVICE_FAIL;
    SPINOR_CORE_STATUS nor_status = SPINOR_CORE_FAIL;
    bool qspi_init_status = FALSE;

    spinor_dev.config_data = &spinor_core_config_data;

    if (spinor_dev.config_data == NULL)
    {
      goto spinor_init_end;
    }

    if ((spinor_dev.config_data->magic_number == 0xFACEECAF) && 
      (spinor_dev.config_data->spinor_client_ref_count > 0) && 
      (spinor_dev.nor_init_done == TRUE))
    {
      /* In FOM mode, spinor_init has been done at least once */
      spinor_dev.config_data->spinor_client_ref_count++;
      status = SPINOR_DEVICE_DONE;
      goto spinor_init_end;
    }
    else
    {
      /* First time. Start keeping track of how many clients have called spinor_init() */
      spinor_dev.config_data->spinor_client_ref_count = 1;
    }

    memset(&spinor_dev, 0, sizeof(spinor_drv_info));
    
    /* Initialize all the spinor_dev structure, except client's handles info  */
    spinor_drv_info_init();

    if(spinor_dev.config_data == NULL)
    {
      spinor_log (&spinor_dev.debug_log, 0x2B0);  
      status = SPINOR_DEVICE_FAIL;
      goto spinor_init_end;
    }
    
    /* Give the spinor device POR reset delay before communicating with it */
    spinor_udelay(10000);

    /* Intialize the Buses QSPI driver */
    nor_status = spinor_controller_init();
    if (nor_status != SPINOR_CORE_DONE)
    {
      spinor_log (&spinor_dev.debug_log, 0x2B1);
      status = SPINOR_DEVICE_FAIL;
      goto spinor_init_end;
    }

    status = spinor_device_lookup(spinor_config_table);
    if (status != SPINOR_DEVICE_DONE)
    {
      spinor_log (&spinor_dev.debug_log, 0x2B2);
      goto spinor_init_end;     
    }

    qspi_init_status = TRUE;
    nor_status= spial_init (qspi_init_status);
    if (status != SPINOR_CORE_DONE)
    {
      spinor_log (&spinor_dev.debug_log, 0x2B3);
      goto spinor_init_end;     
    }

    if ((spinor_dev.config_data->read_mode != READ_SDR_1_1_1_MODE) ||
      (spinor_dev.config_data->write_mode != WRITE_SDR_1_1_1_MODE))
    {
      nor_status = enable_quad_mode(spinor_dev.config_data->quad_enable_mode);
      if (nor_status != SPINOR_CORE_DONE)
      {
        spinor_log (&spinor_dev.debug_log, 0x2B4);
        status = SPINOR_DEVICE_FAIL;
        goto spinor_init_end;    
      }
    }

    /* Enable 4-byte address mode when density is more than 16MB */
    if ((spinor_dev.config_data->addr_bytes == 4) && 
      ((spinor_dev.config_data->density_in_blocks * spinor_dev.config_data->block_size_in_bytes) > SPINOR_16MB_IN_BYTES))
    {
      nor_status = set_4byte_address_mode();
      if (nor_status != SPINOR_DEVICE_DONE)
      {     
        spinor_log (&spinor_dev.debug_log, 0x2B5);      
        status = SPINOR_DEVICE_FAIL;
        goto spinor_init_end;    
      }    
    }

    spinor_dev.nor_init_done = TRUE;
    status = SPINOR_DEVICE_DONE;

    spinor_init_end: 
    return status;
}


/**********************************************************
 * Initializes the spinor controller HW
 *
 * @return int [OUT]
 *   Result of the operation.
 *
 **********************************************************/
 SPINOR_CORE_STATUS spinor_controller_init (void)
 {
    SPINOR_CORE_STATUS status = SPINOR_DEVICE_DONE;
    bool qspi_init_status = FALSE;
    
    /* Initialize the QSPI buses driver */
    status = spial_init(qspi_init_status);
    if (status != SPINOR_CORE_DONE)
    {
      spinor_log (&spinor_dev.debug_log, 0x2C0);  
    }

    return status;
}

/**********************************************************
 * De-initializes the spinor driver.
 * Free up resources used by the spinor driver
 *
 * @return int [OUT]
 *   Result of the operation.
 *
**********************************************************/
 SPINOR_STATUS spinor_deinit (void)
 {
  /*uint32 i = 0; */ 
    SPINOR_STATUS status = SPINOR_DEVICE_FAIL;
    
    if ((spinor_dev.config_data->magic_number == 0xFACEECAF) &&
      (spinor_dev.nor_init_done == FALSE))
    {
      return SPINOR_DEVICE_FAIL;
    }

    /* check if multiple clients are using spinor */
    if (spinor_dev.config_data->spinor_client_ref_count >= 2)
    {
      /* Multiple clients are using spinor */
      spinor_dev.config_data->spinor_client_ref_count--;
      status = SPINOR_DEVICE_DONE;
      goto spinor_deinit_end;
    }
    
    if ((spinor_dev.ref_count) != 0)
    {
      spinor_log (&spinor_dev.debug_log, 0x2D0);
      status = SPINOR_DEVICE_FAIL;
      goto spinor_deinit_end;
    }  
    
    (void)spinor_core_deinit();

    memset(&spinor_dev, 0, sizeof(spinor_drv_info));

    status = SPINOR_DEVICE_DONE;

    spinor_deinit_end:  
    return status;
}



