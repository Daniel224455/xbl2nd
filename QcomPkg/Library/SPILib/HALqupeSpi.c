/*
===========================================================================

FILE:   HALqupeSpi.c

DESCRIPTION:
    This file implements a qup hal specific to SPI related operations

===========================================================================

        Edit History


When     Who    What, where, why
-------- ---    -----------------------------------------------------------
10/03/13 vmk    BAM CMD descriptor usage is determined from properties
06/06/12 ddk    Created

===========================================================================
        Copyright (c) 2012-2013 Qualcomm Technologies Incorporated
                            All Rights Reserved
                         Qualcomm Proprietary/GTDR
===========================================================================
*/

#include "HALqupe.h"
#include "HALqupeTarget.h"
#define HAL_QUPE_SPI_LOG
#include "QupLog.h"

/*==================================================================================================

FUNCTION: HAL_qupe_Spi_ConfigureChipSelect

DESCRIPTION:
   This function configures a SPI chip select line as manual control (forced high or low)
   or automatic control by the SPI core.

ARGUMENTS PASSED:
   base         - QUP base address
   chip_select  - chip select line (0 - 3)
   force_assert - TRUE to force assertion of the chip select
   active_low   - TRUE if the chip select is active low

==================================================================================================*/
void HAL_qupe_Spi_ConfigureChipSelect(uint8 *base, uint32 chip_select,
                                      boolean force_assert, boolean active_low)
{
   uint32 mask;
   uint32 force_cs, cs_n_polarity;
   uint32 spi_io_control;

   mask = QUPE_FMSK(SPI_IO_CONTROL, FORCE_CS) |
          (1 << QUPE_SHFT(SPI_IO_CONTROL, CS_N_POLARITY) << chip_select);

   force_cs      = (force_assert) ? (1) : (0);
   cs_n_polarity = (active_low)   ? (0) : (1);

   spi_io_control = QUPE_IN(base, SPI_IO_CONTROL);
   spi_io_control &= ~mask;
   spi_io_control |= force_cs      << QUPE_SHFT(SPI_IO_CONTROL, FORCE_CS);
   spi_io_control |= cs_n_polarity << QUPE_SHFT(SPI_IO_CONTROL, CS_N_POLARITY)
                                   << chip_select;

   QUPE_OUT(base, SPI_IO_CONTROL, spi_io_control);
}

/*==================================================================================================

FUNCTION: HAL_qupe_Spi_SetDeviceParameters

DESCRIPTION:
   This function configures the SPI device-specific parameters before starting a SPI transfer.

ARGUMENTS PASSED:
   base   - QUP base address
   params - device configuration parameters

==================================================================================================*/
void HAL_qupe_Spi_SetDeviceParameters(uint8 *base, HAL_QUPE_SPI_DeviceParametersType *params)
{
   uint32 mask;
   uint32 spi_config, hs_mode, input_first, loop_back, slave_operation;
   uint32 spi_io_control, clk_idle_high, clk_always_on, mx_cs_mode, cs_n_polarity, cs_select;

   // Configure the SPI_CONFIG register.
   //
   // HS_MODE         - when set high speed mode is enabled
   // INPUT_FIRST     - when set spi_data_in is sampled on the leading clock edge
   // LOOP_BACK       - when set spi_data_out is looped back to spi_data_in
   // SLAVE_OPERATION - when set the SPI core is configured for slave operation

   mask = QUPE_FMSK(SPI_CONFIG, HS_MODE) |
          QUPE_FMSK(SPI_CONFIG, INPUT_FIRST) |
          QUPE_FMSK(SPI_CONFIG, LOOP_BACK) |
          QUPE_FMSK(SPI_CONFIG, SLAVE_OPERATION);

   hs_mode         = (params->bHighSpeedMode)                                 ? (1) : (0);
   input_first     = (params->eTransferMode == HAL_QUPE_SPI_INPUT_FIRST_MODE) ? (1) : (0);
   loop_back       = (params->eLoopbackMode == HAL_QUPE_SPI_LOOPBACK_ENABLED) ? (1) : (0);
   slave_operation = (params->eSpiCoreMode == HAL_QUPE_SPI_SLAVE)             ? (1) : (0);

   spi_config = QUPE_IN(base, SPI_CONFIG);
   spi_config &= ~mask;
   spi_config |= hs_mode         << QUPE_SHFT(SPI_CONFIG, HS_MODE);
   spi_config |= input_first     << QUPE_SHFT(SPI_CONFIG, INPUT_FIRST);
   spi_config |= loop_back       << QUPE_SHFT(SPI_CONFIG, LOOP_BACK);
   spi_config |= slave_operation << QUPE_SHFT(SPI_CONFIG, SLAVE_OPERATION);

   QUPE_OUT(base, SPI_CONFIG, spi_config);

   // Configure the SPI_IO_CONTROL register.
   //
   // FORCE_CS      - when set the chip selects are asserted unconditionally
   // CLK_IDLE_HIGH - when set the SPI clock idles high (the first clock edge is falling)
   // CLK_ALWAYS_ON - when set the clock continues to run between transactions (during IDLE)
   // MX_CS_MODE    - when set the chip select is kept asserted between words in a transfer
   // CS_N_POLARITY - (4 bits) when a bit is set the associated chip select is active high
   // CS_SELECT     - (2 bits) specifies which chip select to assert during transfers
   // TRISTATE_CS   - when set the chip selects are tri-stated
   // NO_TRI_STATE  - when set spi_data_out is not tri-stated when the chip select is deasserted

   mask = QUPE_FMSK(SPI_IO_CONTROL, CLK_IDLE_HIGH) |
          QUPE_FMSK(SPI_IO_CONTROL, CLK_ALWAYS_ON) |
          QUPE_FMSK(SPI_IO_CONTROL, MX_CS_MODE) |
          (1 << QUPE_SHFT(SPI_IO_CONTROL, CS_N_POLARITY) << params->nChipSelectId) |
          QUPE_FMSK(SPI_IO_CONTROL, CS_SELECT);

   clk_idle_high = (params->eClockPolarity == HAL_QUPE_SPI_CLK_IDLE_HIGH) ? (1) : (0);
   clk_always_on = (params->bClockAlwaysOn)                               ? (1) : (0);
   mx_cs_mode    = (params->bChipSelectAlwaysOn)                          ? (1) : (0);
   cs_n_polarity = (params->eCSPolarity == HAL_QUPE_SPI_CS_ACTIVE_HIGH)   ? (1) : (0);
   cs_select     =  params->nChipSelectId;

   spi_io_control = QUPE_IN(base, SPI_IO_CONTROL);

   if (spi_io_control & QUPE_FMSK(SPI_IO_CONTROL, FORCE_CS))
   {
      // If the chip select is currently forced do not change its polarity.
      cs_n_polarity = (spi_io_control >> QUPE_SHFT(SPI_IO_CONTROL, CS_N_POLARITY)
                                      >> params->nChipSelectId) & 1;
   }

   spi_io_control &= ~mask;
   spi_io_control |= clk_idle_high << QUPE_SHFT(SPI_IO_CONTROL, CLK_IDLE_HIGH);
   spi_io_control |= clk_always_on << QUPE_SHFT(SPI_IO_CONTROL, CLK_ALWAYS_ON);
   spi_io_control |= mx_cs_mode    << QUPE_SHFT(SPI_IO_CONTROL, MX_CS_MODE);
   spi_io_control |= cs_n_polarity << QUPE_SHFT(SPI_IO_CONTROL, CS_N_POLARITY)
                                   << params->nChipSelectId;
   spi_io_control |= cs_select     << QUPE_SHFT(SPI_IO_CONTROL, CS_SELECT);

   QUPE_OUT(base, SPI_IO_CONTROL, spi_io_control);

   // Configure the SPI_DEASSERT_WAIT register.
   //
   // DEASSERT_WAIT - (6 bits) chip select is deasserted for (DEASSERT_WAIT + 1)
   //                 SPI master clock ticks between transfers

   QUPE_OUT(base, SPI_DEASSERT_WAIT, params->nDeassertionClockTicks - 1);
}

/*==================================================================================================

FUNCTION: HAL_qupe_Spi_SetTransferParameters

DESCRIPTION:
   This function configures the SPI transfer-specific parameters before starting a SPI transfer.

ARGUMENTS PASSED:
   base   - QUP base address
   params - transfer parameters

==================================================================================================*/
void HAL_qupe_Spi_SetTransferParameters(uint8 *base, HAL_QUPE_SPI_TransferParametersType *params)
{
   uint32 mask;
   uint32 qup_config, no_input, no_output, n;
   uint32 qup_io_modes, output_bit_shift_en, pack_en, unpack_en, input_mode, output_mode;
   uint32 qup_operational_mask, input_service_mask, output_service_mask;
   uint32 qup_mx_output_count;
   uint32 qup_mx_input_count;

   // Configure the QUP_CONFIG register.
   //
   // EN_EXT_OUT_FLAG  - when set enable extended input/output service interrupt generation
   // FIFO_CLK_GATE_EN - when set FIFO clock is turned off dynamically
   // CORE_CLK_ON_EN   - when set dynamic clock on is driven high conditionally
   // APP_CLK_ON_EN    - when set dynamic clock on is driven high conditionally
   // MINI_CORE        - (4 bits) specifies whether to use the SPI core or the I2C core
   // NO_INPUT         - when set no data will be input during the next SPI transfer
   // NO_OUTPUT        - when set no data will be output during the next SPI transfer
   // N                - (5 bits) the SPI transfer word size is (N + 1) bits

   mask = QUPE_FMSK(QUP_CONFIG, NO_INPUT) |
          QUPE_FMSK(QUP_CONFIG, NO_OUTPUT) |
          QUPE_FMSK(QUP_CONFIG, N);

   no_input  = (params->eTransferDirection == HAL_QUPE_SPI_OUTPUT_ONLY) ? (1) : (0);
   no_output = (params->eTransferDirection == HAL_QUPE_SPI_INPUT_ONLY)  ? (1) : (0);
   n         =  params->nNumBits - 1;

   qup_config = QUPE_IN(base, QUP_CONFIG);
   qup_config &= ~mask;
   qup_config |= no_input  << QUPE_SHFT(QUP_CONFIG, NO_INPUT);
   qup_config |= no_output << QUPE_SHFT(QUP_CONFIG, NO_OUTPUT);
   qup_config |= n         << QUPE_SHFT(QUP_CONFIG, N);

   QUPE_OUT(base, QUP_CONFIG, qup_config);

   // Configure the QUP_IO_MODES register.
   //
   // OUTPUT_BIT_SHIFT_EN - when set bit shifting is done as data leaves the output FIFO
   // PACK_EN             - when set data is packed as it enters the input FIFO
   // UNPACK_EN           - when set data is unpacked as it leaves the output FIFO
   // INPUT_MODE          - (2 bits) select FIFO, block, or BAM mode for input
   // OUTPUT_MODE         - (2 bits) select FIFO, block, or BAM mode for output
   // INPUT_FIFO_SIZE     - (3 bits)(read only) input FIFO holds (2, 4, 8, or 16) blocks
   // INPUT_BLOCK_SIZE    - (2 bits)(read only) input block size is (4, 16, or 32) bytes
   // OUTPUT_FIFO_SIZE    - (3 bits)(read only) output FIFO holds (2, 4, 8, or 16) blocks
   // OUTPUT_BLOCK_SIZE   - (2 bits)(read only) output block size is (4, 16, or 32) bytes

   mask = QUPE_FMSK(QUP_IO_MODES, OUTPUT_BIT_SHIFT_EN) |
          QUPE_FMSK(QUP_IO_MODES, PACK_EN) |
          QUPE_FMSK(QUP_IO_MODES, UNPACK_EN) |
          QUPE_FMSK(QUP_IO_MODES, INPUT_MODE) |
          QUPE_FMSK(QUP_IO_MODES, OUTPUT_MODE);

   output_bit_shift_en = (params->bOutputBitShift)     ? (1) : (0);
   pack_en             = (params->bReceivedDataPacked) ? (1) : (0);
   unpack_en           = (params->bTransmitDataPacked) ? (1) : (0);
   input_mode          =  params->eIOMode;
   output_mode         =  params->eIOMode;

   qup_io_modes = QUPE_IN(base, QUP_IO_MODES);
   qup_io_modes &= ~mask;
   qup_io_modes |= output_bit_shift_en << QUPE_SHFT(QUP_IO_MODES, OUTPUT_BIT_SHIFT_EN);
   qup_io_modes |= pack_en             << QUPE_SHFT(QUP_IO_MODES, PACK_EN);
   qup_io_modes |= unpack_en           << QUPE_SHFT(QUP_IO_MODES, UNPACK_EN);
   qup_io_modes |= input_mode          << QUPE_SHFT(QUP_IO_MODES, INPUT_MODE);
   qup_io_modes |= output_mode         << QUPE_SHFT(QUP_IO_MODES, OUTPUT_MODE);

   QUPE_OUT(base, QUP_IO_MODES, qup_io_modes);

   // Configure the QUP_OPERATIONAL_MASK register.
   //
   // INPUT_SERVICE_MASK  - when set suppress INPUT_SERVICE_FLAG interrupts
   // OUTPUT_SERVICE_MASK - when set suppress OUTPUT_SERVICE_FLAG interrupts

   input_service_mask  = (params->eIOMode == HAL_QUPE_IO_MODE_BAM) ? (1) : (0);
   output_service_mask = (params->eIOMode == HAL_QUPE_IO_MODE_BAM) ? (1) : (0);

   qup_operational_mask =  input_service_mask  << QUPE_SHFT(QUP_OPERATIONAL_MASK,
                                                            INPUT_SERVICE_MASK);
   qup_operational_mask |= output_service_mask << QUPE_SHFT(QUP_OPERATIONAL_MASK,
                                                            OUTPUT_SERVICE_MASK);

   QUPE_OUT(base, QUP_OPERATIONAL_MASK, qup_operational_mask);

   // Configure the QUP_MX_OUTPUT_COUNT register.
   //
   // MX_OUTPUT_COUNT - (16 bits) number of N-sized words to write during a RUN_STATE

   qup_mx_output_count = (params->eIOMode == HAL_QUPE_IO_MODE_BLOCK) ?
                         (params->nNumOutputTransfers) : (0);

   QUPE_OUT(base, QUP_MX_OUTPUT_COUNT, qup_mx_output_count);

   // Configure the QUP_MX_INPUT_COUNT register.
   //
   // MX_INPUT_COUNT - (16 bits) number of N-sized words to read during a RUN_STATE

   if (params->eIOMode == HAL_QUPE_IO_MODE_BAM)
   {
      qup_mx_input_count = (params->nNumInputTransfers != params->nNumOutputTransfers) ?
                           (params->nNumInputTransfers) : (0);
   }
   else
   {
      qup_mx_input_count = params->nNumInputTransfers;
   }

   QUPE_OUT(base, QUP_MX_INPUT_COUNT, qup_mx_input_count);
}
