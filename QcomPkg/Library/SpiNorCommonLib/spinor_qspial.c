
/**********************************************************************
 * spionor_qspial.c
 *
 * This file implements function to interface spinor driver with qspi buses driver
 *
 * Copyright (c) 2015-2018
 * Qualcomm Technologies Incorporated.
 * All Rights Reserved.
 * Qualcomm Confidential and Proprietary
 *
 **********************************************************************/
/*=======================================================================
                        Edit History


when        who       what, where, why
----------  -----     ---------------------------------------------------
2018-03-21  wek       Update cache management logic.
2017-05-25  md        Initial Release


======================================================================*/
#include "spinor_utils.h"
#include "spinor_init_config.h"
#include "spinor_osal.h"
#include <api/systemdrivers/busywait.h>
#include "quadspi.h"
#include "spinor_core.h"
#include "spinor_spial.h"


static SPINOR_SPI_STATUS config_cmd_spi(QSPI_CMD *qspi_config_cmd, SPI_COMMAND_TYPE cmd_mode, uint8_t opcode);

SPINOR_SPI_STATUS spial_init(bool init_status)
{
  SPINOR_SPI_STATUS status = SPINOR_DEVICE_DONE;
  QSPI_MASTER_CONFIG qspi_cfg;

  /* Zero inialize memory. */
  memset(&qspi_cfg, 0, sizeof(qspi_cfg));

  /* Only 1 Chip Select */
  qspi_cfg.chip_select  = 0;
  /* This setting is for SPI mode */
  qspi_cfg.clk_polarity = FALSE;
  qspi_cfg.clk_phase    = FALSE;

  /* The QSPI frequency is controlled by the spinor driver via clock APIs */
  /* This clk_freq field is not used by the QSPI buses driver */
  if (init_status == FALSE)
  {
    qspi_cfg.clk_freq = 8000000;
  }
  else
  {
    qspi_cfg.clk_freq = spinor_dev.config_data->freq_khz * 1000;
  }
  (void) quadspi_init(&qspi_cfg);

  return status;
}


SPINOR_SPI_STATUS spial_deinit(void *handle)
{
  SPINOR_SPI_STATUS status = SPINOR_DEVICE_DONE;
  quadspi_deinit();
  return status;
}

static SPINOR_SPI_STATUS spial_enter_working_state(uint64 freq_khz)
{
  /* We don't do power management yet at this stage of boot. */
  return TRUE;
}

static SPINOR_SPI_STATUS spial_enter_low_power_state()
{
  /* We don't do power management yet at this stage of boot. */
  return TRUE;
}

SPINOR_SPI_STATUS spial_transfer(SPI_COMMAND_TYPE cmd_mode, uint32_t addr, uint8 *reg_value, uint32 len,uint8_t opcode )
{
  SPINOR_SPI_STATUS status = SPINOR_SPI_DONE;
  boolean transfer_mode = FALSE;
  uint8 *temp_buffer;
  uint32 transfer_size = 0;
  QSPI_CMD qspi_cmd;

  temp_buffer = reg_value;
  transfer_size = len;
  uint32 address= addr;

  if (cmd_mode == qspi_read_mode || cmd_mode == qspi_write_mode ||cmd_mode == qspi_erase_mode || cmd_mode == qspi_erase_bulk_mode ||cmd_mode == qspi_write_enable_mode)
  {
    transfer_mode = spinor_dev.config_data->dma_enable; //QSPI_DMA_INT_MODE : QSPI_DMA_POLL_MODE;
  }

  memset (&qspi_cmd, 0, sizeof(QSPI_CMD));

  if (status != config_cmd_spi(&qspi_cmd,cmd_mode, opcode))
  {
    status = SPINOR_SPI_FAIL;
    spinor_log (&spinor_dev.debug_log, 0x500);
    goto done;
  }

  if (spinor_dev.config_data->dma_enable == TRUE)
  {
    spinor_cache_op(temp_buffer, transfer_size, FLUSH);
  }

  if (spial_enter_working_state(spinor_dev.config_data->freq_khz) == FALSE)
  {
    status = SPINOR_SPI_FAIL;
    spinor_log (&spinor_dev.debug_log, 0x502);
    goto done;
  }

  if (!quadspi_cmd(&qspi_cmd, address, temp_buffer, transfer_size, transfer_mode))
  {
    status = SPINOR_SPI_FAIL;
    spinor_log (&spinor_dev.debug_log, 0x501);
  }

  if (spial_enter_low_power_state() == FALSE)
  {
    status = SPINOR_SPI_FAIL;
    spinor_log (&spinor_dev.debug_log, 0x503);
  }

  if (spinor_dev.config_data->dma_enable == TRUE)
  {
    spinor_cache_op(temp_buffer, transfer_size, INVAL);
  }

done:

  return status;
}


SPINOR_SPI_STATUS config_cmd_spi(QSPI_CMD *qspi_config_cmd, SPI_COMMAND_TYPE cmd_mode, uint8_t opcode)
{
  QSPI_CMD *qspi_cmd;
  qspi_cmd = qspi_config_cmd;
  SPINOR_SPI_STATUS status = SPINOR_SPI_DONE;

  if (cmd_mode == qspi_read_mode)
  {
    qspi_cmd->opcode       = spinor_dev.config_data->read_sdr_opcode;
    qspi_cmd->addr_bytes   = spinor_dev.config_data->addr_bytes;
    qspi_cmd->dummy_clocks = spinor_dev.config_data->read_sdr_wait_state;
    qspi_cmd->cmd_mode     = SDR_1BIT;
    qspi_cmd->addr_mode    = SDR_1BIT;
    qspi_cmd->data_mode    = SDR_1BIT;
    qspi_cmd->write        = false;

    switch (spinor_dev.config_data->read_mode)
    {
    case READ_SDR_1_1_1_MODE:
      break;                                   /* default settings */

    case READ_SDR_1_1_4_MODE:
      qspi_cmd->data_mode    = SDR_4BIT;
      break;

    case READ_SDR_1_4_4_MODE:
      qspi_cmd->addr_mode    = SDR_4BIT;
      qspi_cmd->data_mode    = SDR_4BIT;
      break;

    case READ_SDR_4_4_4_MODE:
      qspi_cmd->cmd_mode  = SDR_4BIT;
      qspi_cmd->addr_mode = SDR_4BIT;
      qspi_cmd->data_mode = SDR_4BIT;
      break;

    case READ_DDR_1_1_4_MODE:
      qspi_cmd->data_mode    = DDR_4BIT;
      qspi_cmd->opcode       = spinor_dev.config_data->read_ddr_opcode;
      qspi_cmd->dummy_clocks = spinor_dev.config_data->read_ddr_wait_state;
      break;

    case READ_DDR_1_4_4_MODE:
      qspi_cmd->addr_mode    = DDR_4BIT;
      qspi_cmd->data_mode    = DDR_4BIT;
      qspi_cmd->opcode       = spinor_dev.config_data->read_ddr_opcode;
      qspi_cmd->dummy_clocks = spinor_dev.config_data->read_ddr_wait_state;
      break;

    default:
      status = SPINOR_SPI_FAIL;
      spinor_log (&spinor_dev.debug_log, 0x510);
      goto config_cmd_spi;
    }
  }
  else if (cmd_mode == qspi_write_mode)
  {
    qspi_cmd->opcode = spinor_dev.config_data->write_sdr_opcode;
    qspi_cmd->addr_bytes = spinor_dev.config_data->addr_bytes;
    qspi_cmd->dummy_clocks = 0;
    qspi_cmd->cmd_mode = SDR_1BIT;
    qspi_cmd->addr_mode = SDR_1BIT;
    qspi_cmd->data_mode = SDR_1BIT;
    qspi_cmd->write = true;

    switch (spinor_dev.config_data->write_mode)
    {
    case WRITE_SDR_1_1_1_MODE:
      break;								   /* default settings */

    case WRITE_SDR_1_1_4_MODE:
      qspi_cmd->data_mode = SDR_4BIT;
      break;

    case WRITE_SDR_1_4_4_MODE:
      qspi_cmd->addr_mode = SDR_4BIT;
      qspi_cmd->data_mode = SDR_4BIT;
      break;

    case WRITE_SDR_4_4_4_MODE:
      qspi_cmd->cmd_mode   = SDR_4BIT;
      qspi_cmd->addr_mode  = SDR_4BIT;
      qspi_cmd->data_mode  = SDR_4BIT;
      break;

    case WRITE_DDR_1_1_4_MODE:
      qspi_cmd->data_mode	 = DDR_4BIT;
      qspi_cmd->opcode		 = spinor_dev.config_data->write_ddr_opcode;
      break;

    case WRITE_DDR_1_4_4_MODE:
      qspi_cmd->addr_mode	 = DDR_4BIT;
      qspi_cmd->data_mode	 = DDR_4BIT;
      qspi_cmd->opcode		 = spinor_dev.config_data->write_ddr_opcode;
      break;

    default:
      status = SPINOR_SPI_FAIL;
      spinor_log (&spinor_dev.debug_log, 0x511);
      goto config_cmd_spi;
    }
  }
  else if (cmd_mode == qspi_erase_mode)
  {
    qspi_cmd->opcode = spinor_dev.config_data->erase_4kb_opcode;
    qspi_cmd->addr_bytes = spinor_dev.config_data->addr_bytes;
    qspi_cmd->dummy_clocks = 0;
    qspi_cmd->cmd_mode = SDR_1BIT;
    qspi_cmd->addr_mode = SDR_1BIT;
    qspi_cmd->data_mode = SDR_1BIT;
    qspi_cmd->write = false;
  }
  else if (cmd_mode == qspi_erase_bulk_mode)
  {
    qspi_cmd->opcode = spinor_dev.config_data->bulk_erase_opcode;
    qspi_cmd->addr_bytes = spinor_dev.config_data->addr_bytes;
    qspi_cmd->dummy_clocks = 0;
    qspi_cmd->cmd_mode = SDR_1BIT;
    qspi_cmd->addr_mode = SDR_1BIT;
    qspi_cmd->data_mode = SDR_1BIT;
    qspi_cmd->write = false;
  }
  else if (cmd_mode == qspi_write_enable_mode)
  {
    /* WRITE_ENABLE_CMD 0x06*/
    qspi_cmd->opcode = WRITE_ENABLE_CMD;
    qspi_cmd->addr_bytes = 0;
    qspi_cmd->dummy_clocks = 0;
    qspi_cmd->cmd_mode = SDR_1BIT;
    qspi_cmd->addr_mode = SDR_1BIT;
    qspi_cmd->data_mode = SDR_1BIT;
    qspi_cmd->write = false;
  }
  else if (cmd_mode == qspi_register_write)
  {
    qspi_cmd->opcode = opcode;
    qspi_cmd->addr_bytes = 0;
    qspi_cmd->dummy_clocks = 0;
    qspi_cmd->cmd_mode = SDR_1BIT;
    qspi_cmd->addr_mode = SDR_1BIT;
    qspi_cmd->data_mode = SDR_1BIT;
    qspi_cmd->write = true;
  }
  else if (cmd_mode == qspi_register_read)
  {
    qspi_cmd->opcode = opcode;
    qspi_cmd->addr_bytes = 0;
    qspi_cmd->dummy_clocks = 0;
    qspi_cmd->cmd_mode = SDR_1BIT;
    qspi_cmd->addr_mode = SDR_1BIT;
    qspi_cmd->data_mode = SDR_1BIT;
    qspi_cmd->write = false;

  }

config_cmd_spi:
  return status;
}


