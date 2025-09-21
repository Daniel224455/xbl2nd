/*==================================================================================================

FILE: SpiDevice.c

DESCRIPTION:
   This file contains the implementation of the SPI common device layer.

GLOBAL FUNCTIONS:
   SpiDevice_Init
   SpiDevice_DeInit
   SpiDevice_WriteRead
   SpiDevice_Read
   SpiDevice_Write
   SpiDevice_AsyncWriteRead
   SpiDevice_AsyncRead
   SpiDevice_AsyncWrite
   SpiDevice_BatchTransfer
   SpiDevice_CancelTransfer
   SpiDevice_SetPowerState
   SpiDevice_GetHwInfo
   SpiDevice_IsrHook
   SpiDevice_IstHook
   SpiDevice_BamCallback

====================================================================================================

   Edit History

When      Who  What, where, why
--------  ---  ------------------------------------------------------------
06/24/14  sg   Removed redundant IBUSPLAT_SetPowerState() in SpiDevice_Init
10/03/13  vmk  BAM CMD descriptor usage is determined from properties
07/14/12  ag   Added updates to remove windows warnings and some interrupt
               Prototype changes.
05/24/12  ddk  Added updates to support common qup hal.
05/22/12  ddk  Added updates to calculate deassert waits.
04/20/12  ddk  Added updates to enable logging.
04/19/12  ddk  Added changes for avoiding symbol match with I2C library.
04/13/12  ddk  Fixed null pointer accesses in duplex transmissions.
03/29/12  ddk  Added deinit requriements for bam mode.
03/28/12  ddk  Added QUP physical address info for BAM cmd desc creation.
03/28/12  ddk  Fixed incorrect setting of SPI core mode.
03/27/12  ddk  Fixed some issues in driver related to Slave selection
               and BAM mode selection.
03/27/12  ag   Added SPI BAM mode updates and transfer requirements.
12/10/12  ag   Created.

====================================================================================================
                     Copyright (c) 2011-2014 Qualcomm Technologies Incorporated
                                        All Rights Reserved
                                     Qualcomm Proprietary/GTDR
==================================================================================================*/

#include "SpiDeviceTypes.h"
#include "SpiDevice.h"
#include "SpiDeviceBam.h"
#include "SpiDeviceTransfer.h"
#include "SpiDevicePlatSvc.h"
#include "SpiDeviceOsSvc.h"
#include "HALqupe.h"
#define SPIDEVICE_LOG_H
#include "SpiLog.h"

/*==================================================================================================
                                              CONSTANTS
==================================================================================================*/

#define QUP_RUN_STATE_CHANGE_TIME_OUT_US  100  // timeout during QUP state validity check
#define QUP_COMPLETION_TIME_OUT_US        400  // timeout while polling for transfer completion
#define QUP_BUSY_WAIT_STEP_US             5
#define QUP_BUSY_WAIT_POLL_STEP_US        1

/*==================================================================================================
                                               MACROS
==================================================================================================*/

#ifndef MAX
#define MAX(a, b)  ((a) > (b) ? (a) : (b))
#endif

/*==================================================================================================
                                              TYPEDEFS
==================================================================================================*/

typedef enum
{
   SPIDEVICE_INITSTATE_UNINITIALIZED     = 0x0,
   SPIDEVICE_INITSTATE_TARGET_INIT_DONE  = 0x1,
   SPIDEVICE_INITSTATE_BAM_INIT_DONE     = 0x2,
   SPIDEVICE_INITSTATE_EVENTS_DONE       = 0x4,
   SPIDEVICE_INITSTATE_REGISTER_ISR_DONE = 0x8,
} SpiDevice_InitState;

typedef struct
{
   SPI_DEV_ASYNC_CB      pCallbackFn;
   SpiDevice_AsyncResult asyncResult;
} SpiDevice_AsyncCallData;

typedef struct
{
   SpiDevice_Config         device_cfg;
   SpiDevice_InitState      init_state;
   IBUSPLAT_HANDLE          plat_handle;
   SpiDeviceBamType        *bam_handle;
   HAL_qupe_HwInfoType      hw_info;
   IBUSOS_EVENT_HANDLE      interrupt_event;
   SpiDevice_AsyncCallData  async_data;
   SpiDeviceTransferType    transfer_info;
   boolean                  transfer_complete;
} SpiDevice_DeviceCtxt;

typedef struct
{
   SpiDeviceInfoType *qup_config;
   boolean            bam_mode;
   void              *read_virtual;
   void              *read_physical;
   uint32             read_length;
   void              *write_virtual;
   void              *write_physical;
   uint32             write_length;
} SpiDevice_TransferDescriptor;

/*==================================================================================================
                                     LOCAL FUNCTION PROTOTYPES
==================================================================================================*/

static int32  build_descriptor_batch(SpiDeviceInfoType            *qup_config,
                                     boolean                       bam_mode,
                                     SpiLinkedTransferType        *linked,
                                     SpiDevice_TransferDescriptor *descriptor);
static int32  build_descriptor_single(SpiDeviceInfoType            *qup_config,
                                      boolean                       bam_mode,
                                      SpiDataAddrType              *read_buffer,
                                      uint32                        read_length,
                                      SpiDataAddrType              *write_buffer,
                                      uint32                        write_length,
                                      SpiDevice_TransferDescriptor *descriptor);
static void   change_state_to_reset(uint8 *base);
static uint32 configure_frequency(SpiDevice_DeviceCtxt *ctxt,
                                  SpiDevice_TransferDescriptor *descriptor, uint32 *new_frequency);
static uint32 hardware_init(SpiDevice_DeviceCtxt *ctxt);
static void   initialize_transfer_info(SpiDevice_DeviceCtxt *ctxt,
                                       SpiDevice_TransferDescriptor *descriptor,
                                       SpiLinkedTransferType *linked);
static uint32 num_transfers(uint32 num_bytes, uint8 word_size);
static void   read_block(SpiDevice_DeviceCtxt *ctxt);
static void   set_device_parameters(SpiDevice_TransferDescriptor *descriptor, uint32 clock_rate,
                                    HAL_QUPE_SPI_DeviceParametersType *hal_params);
static void   set_transfer_parameters(SpiDevice_TransferDescriptor *descriptor,
                                      HAL_QUPE_SPI_TransferParametersType *hal_params);
static void   signal_completion(SpiDevice_DeviceCtxt *ctxt, uint32 result);
static uint32 start_transfer(SpiDevice_DeviceCtxt *ctxt,
                             SpiDevice_TransferDescriptor *descriptor, boolean eot);
static uint32 validate_descriptor(SpiDevice_TransferDescriptor *descriptor);
static void   wait_for_completion(SpiDevice_DeviceCtxt *ctxt);
static uint32 wait_for_run_state_valid(uint8 *base);
static void   write_block(SpiDevice_DeviceCtxt *ctxt);

/*==================================================================================================
                                          LOCAL FUNCTIONS
==================================================================================================*/
/*==================================================================================================

FUNCTION: build_descriptor_batch

DESCRIPTION:
   Build a transfer descriptor using parameters from a batch transfer request.

ARGUMENTS PASSED:
   qup_config - hardware configuration
   bam_mode   - flag that indicates whether to use BAM mode (TRUE) or PIO mode (FALSE)
   linked     - pointer to linked transfer structure (NULL for single buffer transfer)
   descriptor - pointer to the transfer descriptor we are building

RETURN VALUE:
   status - non-zero value indicates an error

==================================================================================================*/
static int32 build_descriptor_batch(SpiDeviceInfoType            *qup_config,
                                    boolean                       bam_mode,
                                    SpiLinkedTransferType        *linked,
                                    SpiDevice_TransferDescriptor *descriptor)
{
   boolean read, write;

   read = linked->pReadBuffVec &&
          linked->pReadBuffVec->pData &&
          linked->pReadBuffVec->pData->virtualAddr &&
          linked->pReadBuffVec->pData->physicalAddr &&
          linked->pReadBuffVec->uLen;

   write = linked->pWriteBuffVec &&
           linked->pWriteBuffVec->pData &&
           linked->pWriteBuffVec->pData->virtualAddr &&
           linked->pWriteBuffVec->pData->physicalAddr &&
           linked->pWriteBuffVec->uLen;

   descriptor->qup_config     = qup_config;
   descriptor->bam_mode       = bam_mode;
   descriptor->read_virtual   = read  ? linked->pReadBuffVec->pData->virtualAddr   : NULL;
   descriptor->read_physical  = read  ? linked->pReadBuffVec->pData->physicalAddr  : NULL;
   descriptor->read_length    = read  ? linked->pReadBuffVec->uLen                 : 0;
   descriptor->write_virtual  = write ? linked->pWriteBuffVec->pData->virtualAddr  : NULL;
   descriptor->write_physical = write ? linked->pWriteBuffVec->pData->physicalAddr : NULL;
   descriptor->write_length   = write ? linked->pWriteBuffVec->uLen                : 0;

   return validate_descriptor(descriptor);
}

/*==================================================================================================

FUNCTION: build_descriptor_single

DESCRIPTION:
   Build a transfer descriptor using parameters from a single transfer request.

ARGUMENTS PASSED:
   qup_config   - hardware configuration
   bam_mode     - flag that indicates whether to use BAM mode (TRUE) or PIO mode (FALSE)
   read_buffer  - pointer to structure that holds read buffer addresses
   read_length  - length (in bytes) of the read buffer
   write_buffer - pointer to structure that holds write buffer addresses
   write_length - length (in bytes) of the write buffer
   descriptor   - pointer to the descriptor we will build

RETURN VALUE:
   status - non-zero value indicates an error

==================================================================================================*/
static int32 build_descriptor_single(SpiDeviceInfoType            *qup_config,
                                     boolean                       bam_mode,
                                     SpiDataAddrType              *read_buffer,
                                     uint32                        read_length,
                                     SpiDataAddrType              *write_buffer,
                                     uint32                        write_length,
                                     SpiDevice_TransferDescriptor *descriptor)
{
   boolean read, write;

   read = read_buffer &&
          read_buffer->virtualAddr &&
          read_buffer->physicalAddr &&
          read_length;

   write = write_buffer &&
           write_buffer->virtualAddr &&
           write_buffer->physicalAddr &&
           write_length;

   descriptor->qup_config     = qup_config;
   descriptor->bam_mode       = bam_mode;
   descriptor->read_virtual   = read  ? read_buffer->virtualAddr   : NULL;
   descriptor->read_physical  = read  ? read_buffer->physicalAddr  : NULL;
   descriptor->read_length    = read  ? read_length                : 0;
   descriptor->write_virtual  = write ? write_buffer->virtualAddr  : NULL;
   descriptor->write_physical = write ? write_buffer->physicalAddr : NULL;
   descriptor->write_length   = write ? write_length               : 0;

   return validate_descriptor(descriptor);
}

/*==================================================================================================

FUNCTION: change_state_to_reset

DESCRIPTION:
   Change the QUP state to the reset state.

ARGUMENTS PASSED:
   base - QUP base address

==================================================================================================*/
static void change_state_to_reset(uint8 *base)
{
   HAL_qupe_SetRunState(base, HAL_QUPE_RUNSTATE_PAUSE);
   wait_for_run_state_valid(base);
   HAL_qupe_SetRunState(base, HAL_QUPE_RUNSTATE_RESET);
   wait_for_run_state_valid(base);
}

/*==================================================================================================

FUNCTION: configure_frequency

DESCRIPTION:
   Configure the SPI clock frequency.

ARGUMENTS PASSED:
   ctxt          - device context
   descriptor    - pointer to the transfer descriptor
   new_frequency - the new frequency the SPI core is running (if successful)

RETURN VALUE:
   status - non-zero value indicates an error

==================================================================================================*/
static uint32 configure_frequency(SpiDevice_DeviceCtxt *ctxt,
                                  SpiDevice_TransferDescriptor *descriptor, uint32 *new_frequency)
{
   uint32 res;
   uint32 max_frequency = descriptor->qup_config->deviceParameters.u32MaxSlaveFrequencyHz;
   uint32 min_frequency = descriptor->qup_config->deviceParameters.u32MinSlaveFrequencyHz;
   uint32 actual_frequency;

   res = IBUSPLAT_SetAppClkHz(ctxt->plat_handle, max_frequency, &actual_frequency);

   if (res != SPIDEVICE_RES_SUCCESS)
   {
      SPIDEVICE_LOG(ERROR, "configure_frequency: IBUSPLAT_SetAppClkHz failed (0x%lx)", res);
      return(res);
   }

   if ((min_frequency > 0) && (actual_frequency < min_frequency))
   {
      SPIDEVICE_LOG(ERROR, "failed to set frequency within specified parameters");
      return(SPIDEVICE_ERR_CLOCK_SETTING_FAILED);
   }

   *new_frequency = actual_frequency;
   return(SPIDEVICE_RES_SUCCESS);
}

/*==================================================================================================

FUNCTION: hardware_init

DESCRIPTION:
   Perform one-time initialization of the hardware.

ARGUMENTS PASSED:
   ctxt - device context

RETURN VALUE:
   status - non-zero value indicates an error

==================================================================================================*/
static uint32 hardware_init(SpiDevice_DeviceCtxt *ctxt)
{
   uint8 *base = (uint8 *)ctxt->device_cfg.QupVirtAddr;
   uint32 res;

   HAL_qupe_Init(base, NULL);
   res = wait_for_run_state_valid(base);

   if (res != SPIDEVICE_RES_SUCCESS)
   {
      SPIDEVICE_LOG(ERROR, "hardware_init: invalid state after reset (0x%lx)", res);
      return(res);
   }

   HAL_qupe_GetHwInfo(base, &ctxt->hw_info);
   HAL_qupe_SetSelectedMinicore(base, HAL_QUPE_MINI_CORE_SPI);

   if (ctxt->device_cfg.TcsrVirtAddr)
   {
      HAL_qupe_EnableTcsrInterrupt(ctxt->device_cfg.TcsrVirtAddr,
                                   ctxt->device_cfg.TcsrInterruptBitMask,
                                   ctxt->device_cfg.TcsrInterruptBitShift);
   }

   return(SPIDEVICE_RES_SUCCESS);
}

/*==================================================================================================

FUNCTION: initialize_transfer_info

DESCRIPTION:
   Initialize the transfer information structure.

ARGUMENTS PASSED:
   ctxt   - device context
   linked - pointer to linked transfer structure (NULL for single buffer transfer)

==================================================================================================*/
static void initialize_transfer_info(SpiDevice_DeviceCtxt *ctxt,
                                     SpiDevice_TransferDescriptor *descriptor,
                                     SpiLinkedTransferType *linked)
{
   if (linked)  // using linked buffers
   {
      SpiDeviceLinkedBufferTransferInit(&ctxt->transfer_info,
                                        linked->pReadBuffVec,
                                        linked->uNumReadVecs,
                                        linked->pWriteBuffVec,
                                        linked->uNumWriteVecs);
   }
   else  // using single buffers
   {
      SpiDeviceSingleBufferTransferInit(&ctxt->transfer_info,
                                        (uint8 *)descriptor->read_virtual,
                                        (uint8 *)descriptor->read_physical,
                                                 descriptor->read_length,
                                        (uint8 *)descriptor->write_virtual,
                                        (uint8 *)descriptor->write_physical,
                                                 descriptor->write_length);
   }

   ctxt->transfer_info.bWriteTransferDone = (descriptor->write_length) ? FALSE : TRUE;
   ctxt->transfer_info.bWriteBufferEmpty  = (descriptor->write_length) ? FALSE : TRUE;
   ctxt->transfer_info.bReadTransferDone  = (descriptor->read_length)  ? FALSE : TRUE;
   ctxt->transfer_info.bReadBufferFull    = (descriptor->read_length)  ? FALSE : TRUE;

   if (ctxt->transfer_info.uErrorFlagsNew)  // save copy of error flags before clearing them
   {
      ctxt->transfer_info.uErrorFlagsOld = ctxt->transfer_info.uErrorFlagsNew;
      ctxt->transfer_info.uErrorFlagsNew = 0;
   }
}

/*==================================================================================================

FUNCTION: num_transfers

DESCRIPTION:
   This function calculates the number of transfers needed based on the value of N (word size).

ARGUMENTS PASSED:
   num_bytes - transfer length in bytes
   word_size - bits per transfer (N)

RETURN VALUE:
   number of transfers needed

==================================================================================================*/
static uint32 num_transfers(uint32 num_bytes, uint8 word_size)
{
   if      (word_size > 16) { return (num_bytes / sizeof(uint32)); }
   else if (word_size > 8)  { return (num_bytes / sizeof(uint16)); }
   else                     { return (num_bytes); }
}

/*==================================================================================================

FUNCTION: read_block

DESCRIPTION:
   Read one block of data from the input FIFO.

ARGUMENTS PASSED:
   ctxt - device context

==================================================================================================*/
static void read_block(SpiDevice_DeviceCtxt *ctxt)
{
   uint8 *base = (uint8 *)ctxt->device_cfg.QupVirtAddr;
   SpiDeviceTransferType *xfer_ptr = &ctxt->transfer_info;
   uint32 data_word, i;
   SpiDeviceTransferDataResult result;

   for (i = 0; i < ctxt->hw_info.uNumInputBlockSlots; i++)
   {
      HAL_qupe_GetWordFromInFifo(base, &data_word);
      result = xfer_ptr->setNextWord(xfer_ptr, &data_word);

      if (result == SPIDEVICE_TRANSFER_SET_DATA_NONE)
      {
         if (i == 0) { SPIDEVICE_LOG(ERROR, "insufficient room in read buffer"); }
      }
      else if (result == SPIDEVICE_TRANSFER_SET_DATA_DONE)
      {
         xfer_ptr->bReadBufferFull = TRUE;
      }
   }
}

/*==================================================================================================

FUNCTION: set_device_parameters

DESCRIPTION:
   This function populates the HAL device parameters structure.

ARGUMENTS PASSED:
   descriptor - transfer descriptor
   clock_rate - current SPI clock frequency
   hal_params - pointer to the HAL parameters that will be returned

==================================================================================================*/
static void set_device_parameters(SpiDevice_TransferDescriptor *descriptor, uint32 clock_rate,
                                  HAL_QUPE_SPI_DeviceParametersType *hal_params)
{
   SpiDeviceParametersType   *deviceParameters   = &descriptor->qup_config->deviceParameters;
   SpiDeviceBoardInfoType    *deviceBoardInfo    = &descriptor->qup_config->deviceBoardInfo;
   SpiTransferParametersType *transferParameters = &descriptor->qup_config->transferParameters;
   uint32 ns_per_tick;
   uint32 deassert_ticks;

   // Convert deassertion time from nanoseconds to clock ticks (rounding up).  Ticks must
   // be in the range from 1 to 64 (this is a hardware limitation).  In high speed mode
   // ticks should be at least 2.

   ns_per_tick = 1000000000 / clock_rate;
   deassert_ticks = (deviceParameters->u32DeassertionTime + (ns_per_tick - 1)) / ns_per_tick;

   if (deassert_ticks < 1)                              { deassert_ticks =  1; }
   if (deassert_ticks < 2 && deviceParameters->bHSMode) { deassert_ticks =  2; }
   if (deassert_ticks > 64)                             { deassert_ticks = 64; }

   hal_params->eClockPolarity = (deviceParameters->eClockPolarity == SPI_CLK_IDLE_HIGH) ?
                                (HAL_QUPE_SPI_CLK_IDLE_HIGH) : (HAL_QUPE_SPI_CLK_IDLE_LOW);

   hal_params->eTransferMode = (deviceParameters->eShiftMode == SPI_INPUT_FIRST_MODE) ?
                               (HAL_QUPE_SPI_INPUT_FIRST_MODE) : (HAL_QUPE_SPI_OUTPUT_FIRST_MODE);

   hal_params->bHighSpeedMode = deviceParameters->bHSMode;

   hal_params->bClockAlwaysOn = (deviceParameters->eClockMode == SPI_CLK_ALWAYS_ON) ?
                                (TRUE) : (FALSE);

   hal_params->bChipSelectAlwaysOn = (deviceParameters->eCSMode == SPI_CS_KEEP_ASSERTED) ?
                                     (TRUE) : (FALSE);

   hal_params->eCSPolarity = (deviceParameters->eCSPolarity == SPI_CS_ACTIVE_LOW) ?
                             (HAL_QUPE_SPI_CS_ACTIVE_LOW) : (HAL_QUPE_SPI_CS_ACTIVE_HIGH);

   hal_params->nChipSelectId = deviceBoardInfo->nSlaveNumber;

   hal_params->nDeassertionClockTicks = deassert_ticks;

   hal_params->eSpiCoreMode = (deviceBoardInfo->eCoreMode == SPI_CORE_MODE_SLAVE) ?
                              (HAL_QUPE_SPI_SLAVE) : (HAL_QUPE_SPI_MASTER);

   hal_params->eLoopbackMode = (transferParameters->eLoopbackMode == SPI_LOOPBACK_ENABLED) ?
                               (HAL_QUPE_SPI_LOOPBACK_ENABLED) : (HAL_QUPE_SPI_LOOPBACK_DISABLED);
}

/*==================================================================================================

FUNCTION: set_transfer_parameters

DESCRIPTION:
   This function populates the HAL transfer parameters structure.

ARGUMENTS PASSED:
   descriptor - transfer descriptor
   hal_params - pointer to the HAL parameters that will be returned

==================================================================================================*/
static void set_transfer_parameters(SpiDevice_TransferDescriptor *descriptor,
                                    HAL_QUPE_SPI_TransferParametersType *hal_params)
{
   uint8 num_bits = (uint8)descriptor->qup_config->transferParameters.nNumBits;
   uint32 read_length  = descriptor->read_length;
   uint32 write_length = descriptor->write_length;

   hal_params->nNumBits = num_bits;

   hal_params->bReceivedDataPacked = (num_bits <= 16) ? (TRUE) : (FALSE);
   hal_params->bTransmitDataPacked = (num_bits <= 16) ? (TRUE) : (FALSE);

   hal_params->bOutputBitShift = TRUE;

   hal_params->nNumInputTransfers  = num_transfers(read_length, num_bits);
   hal_params->nNumOutputTransfers = num_transfers(write_length, num_bits);

   hal_params->eTransferDirection = (read_length == 0)  ? (HAL_QUPE_SPI_OUTPUT_ONLY) :
                                    (write_length == 0) ? (HAL_QUPE_SPI_INPUT_ONLY) :
                                                          (HAL_QUPE_SPI_BI_DIRECTIONAL);

   hal_params->eIOMode = (descriptor->bam_mode) ? (HAL_QUPE_IO_MODE_BAM) :
                                                  (HAL_QUPE_IO_MODE_BLOCK);
}

/*==================================================================================================

FUNCTION: signal_completion

DESCRIPTION:
   Signal transfer completion via callback, event, or flag.

ARGUMENTS PASSED:
   ctxt   - device context
   result - completion result

==================================================================================================*/
static void signal_completion(SpiDevice_DeviceCtxt *ctxt, uint32 result)
{
   SpiDeviceTransferType *xfer_ptr = &ctxt->transfer_info;

   xfer_ptr->transferResult = result;

   if (ctxt->async_data.pCallbackFn)  // signal via callback
   {
      ctxt->async_data.asyncResult.nTransferStatus = result;
      ctxt->async_data.pCallbackFn(&ctxt->async_data.asyncResult);
   }
   else if (ctxt->device_cfg.InterruptBased)  // signal via event
   {
      IBUSOS_SetEvent(ctxt->interrupt_event);
   }
   else  // signal via flag
   {
      ctxt->transfer_complete = TRUE;
   }
}

/*==================================================================================================

FUNCTION: start_transfer

DESCRIPTION:
   This function starts the transfer described in the descriptor.  The transfer parameters
   are programmed into the hardware registers and the core is transitioned to the RUN state.

ARGUMENTS PASSED:
   ctxt       - device context
   descriptor - transfer descriptor
   eot        - flag that indicates "end of transfer" (used in BAM mode only)

RETURN VALUE:
   status - non-zero value indicates an error

==================================================================================================*/
static uint32 start_transfer(SpiDevice_DeviceCtxt *ctxt,
                             SpiDevice_TransferDescriptor *descriptor, boolean eot)
{
   uint8 *base = (uint8 *)ctxt->device_cfg.QupVirtAddr;
   SpiDeviceBamType *bam_handle = ctxt->bam_handle;
   uint32 clock_rate;
   HAL_QUPE_SPI_DeviceParametersType   hal_device_parameters;
   HAL_QUPE_SPI_TransferParametersType hal_transfer_parameters;
   uint32 res;

   if (ctxt->device_cfg.InterruptBased)
   {
      res = IBUSOS_ClearEvent(ctxt->interrupt_event);

      if (res != SPIDEVICE_RES_SUCCESS)
      {
         SPIDEVICE_LOG(ERROR, "start_transfer: IBUSOS_ClearEvent failed (0x%lx)", res);
         return(res);
      }
   }

   res = configure_frequency(ctxt, descriptor, &clock_rate);
   if (res != SPIDEVICE_RES_SUCCESS) { return(res); }

   set_device_parameters(descriptor, clock_rate, &hal_device_parameters);
   set_transfer_parameters(descriptor, &hal_transfer_parameters);

   if (descriptor->bam_mode && ctxt->device_cfg.UseCmdDesc)
   {
      // configure HW using BAM descriptors

      bam_handle->setQupDeviceParms  (bam_handle, &hal_device_parameters);
      bam_handle->setQupTransferParms(bam_handle, &hal_transfer_parameters);
   }
   else
   {
      // configure HW via direct register access

      HAL_qupe_SetRunState(base, HAL_QUPE_RUNSTATE_RESET);
      res = wait_for_run_state_valid(base);
      if (res != SPIDEVICE_RES_SUCCESS) { return(res); }

      HAL_qupe_Spi_SetDeviceParameters  (base, &hal_device_parameters);
      HAL_qupe_Spi_SetTransferParameters(base, &hal_transfer_parameters);

      HAL_qupe_SetRunState(base, HAL_QUPE_RUNSTATE_RUN);

      if (descriptor->bam_mode)
      {
         res = wait_for_run_state_valid(base);
         if (res != SPIDEVICE_RES_SUCCESS) { return(res); }
      }
   }

   if (descriptor->bam_mode)
   {
      bam_handle->setBamTransferCallback(bam_handle, SpiDevice_BamCallback);
      res = bam_handle->startBamTransfer(bam_handle, &ctxt->transfer_info, (void *)ctxt, eot);
      if (res != SPIDEVICE_RES_SUCCESS) { return(res); }
   }

   return(SPIDEVICE_RES_SUCCESS);
}

/*==================================================================================================

FUNCTION: validate_descriptor

DESCRIPTION:
   This function validates a transfer descriptor.

ARGUMENTS PASSED:
   descriptor - transfer descriptor

RETURN VALUE:
   status - non-zero value indicates an error

==================================================================================================*/
static uint32 validate_descriptor(SpiDevice_TransferDescriptor *descriptor)
{
   SpiDeviceInfoType *qup_config = descriptor->qup_config;
   uint32 read_len               = descriptor->read_length;
   uint32 write_len              = descriptor->write_length;
   uint32 num_bits;

   if (qup_config == NULL)
   {
      SPIDEVICE_LOG(ERROR, "qup_config cannot be NULL");
      return(SPIDEVICE_ERR_INVALID_CONFIG_POINTER);
   }

   if ( (qup_config->deviceParameters.eClockMode == SPI_CLK_ALWAYS_ON) &&
        (qup_config->deviceParameters.eCSMode == SPI_CS_KEEP_ASSERTED) )
   {
      SPIDEVICE_LOG(ERROR, "SPI_CLK_ALWAYS_ON and SPI_CS_KEEP_ASSERTED cannot both be selected");
      return(SPIDEVICE_ERR_INVALID_DEV_PARAMETERS);
   }

   if (!read_len && !write_len)
   {
      SPIDEVICE_LOG(ERROR, "read and write parameters cannot both be NULL");
      return(SPIDEVICE_ERR_INVALID_BUFFER_POINTER);
   }

   num_bits = qup_config->transferParameters.nNumBits;

   if (num_bits > 16)
   {
      if (write_len % sizeof(uint32))
      {
         SPIDEVICE_LOG(ERROR, "incompatible N (%d) and buffer length (%d)", num_bits, write_len);
         return(SPIDEVICE_ERR_INVALID_BUFFER_LEN);
      }

      if (read_len % sizeof(uint32))
      {
         SPIDEVICE_LOG(ERROR, "incompatible N (%d) and buffer length (%d)", num_bits, read_len);
         return(SPIDEVICE_ERR_INVALID_BUFFER_LEN);
      }
   }
   else if (num_bits > 8)
   {
      if (write_len % sizeof(uint16))
      {
         SPIDEVICE_LOG(ERROR, "incompatible N (%d) and buffer length (%d)", num_bits, write_len);
         return(SPIDEVICE_ERR_INVALID_BUFFER_LEN);
      }

      if (read_len % sizeof(uint16))
      {
         SPIDEVICE_LOG(ERROR, "incompatible N (%d) and buffer length (%d)", num_bits, read_len);
         return(SPIDEVICE_ERR_INVALID_BUFFER_LEN);
      }
   }

   return(SPIDEVICE_RES_SUCCESS);
}

/*==================================================================================================

FUNCTION: wait_for_completion

DESCRIPTION:
   Waits for transfer completion (via an event or polling).

ARGUMENTS PASSED:
   ctxt - device context

==================================================================================================*/
static void wait_for_completion(SpiDevice_DeviceCtxt *ctxt)
{
   uint8 *base = (uint8 *)ctxt->device_cfg.QupVirtAddr;
   uint32 time_elapsed = 0;
   uint32 res;

   if (ctxt->device_cfg.InterruptBased)  // interrupt based wait
   {
      res = IBUSOS_WaitEvent(ctxt->interrupt_event, 1000);

      if (res == SPIDEVICE_RES_SUCCESS)
      {
         ctxt->transfer_info.transferResult = SPIDEVICE_RES_SUCCESS;
      }
      else if (res == IBUSOS_ERROR_TIMEOUT)
      {
         SPIDEVICE_LOG(ERROR, "wait_for_completion: timeout waiting for event");
         ctxt->transfer_info.transferResult = SPIDEVICE_TIMEOUT;
      }
      else
      {
         ctxt->transfer_info.transferResult = res;
      }
   }
   else  // polling mode
   {
      ctxt->transfer_complete = FALSE;

      for (;;)
      {
         SpiDevice_IsrHook((void *)ctxt);

         if (ctxt->transfer_complete)
         {
            ctxt->transfer_info.transferResult = SPIDEVICE_RES_SUCCESS;
            break;
         }

         IBUSOS_BusyWait(QUP_BUSY_WAIT_POLL_STEP_US);
         time_elapsed += QUP_BUSY_WAIT_POLL_STEP_US;

         if (time_elapsed >= QUP_COMPLETION_TIME_OUT_US)
         {
            SPIDEVICE_LOG(ERROR, "wait_for_completion: timeout while polling");
            ctxt->transfer_info.transferResult = SPIDEVICE_TIMEOUT;
            break;
         }
      }
   }

   if (ctxt->transfer_info.transferResult != SPIDEVICE_RES_SUCCESS)
   {
      ctxt->transfer_info.transferResult |= HAL_qupe_Pack28_OperationalState(base);

      SPIDEVICE_LOG(ERROR, "wait_for_completion: transferResult = 0x%lx",
                    ctxt->transfer_info.transferResult);

      change_state_to_reset(base);
   }
}

/*==================================================================================================

FUNCTION: wait_for_run_state_valid

DESCRIPTION:
   Wait for the run state to go valid or time out.

ARGUMENTS PASSED:
   base - QUP base address

RETURN VALUE:
   status - non-zero value indicates an error

==================================================================================================*/
static uint32 wait_for_run_state_valid(uint8 *base)
{
   uint32 time_elapsed = 0;

   while ( !HAL_qupe_RunStateIsValid(base) )
   {
      IBUSOS_BusyWait(QUP_BUSY_WAIT_STEP_US);
      time_elapsed += QUP_BUSY_WAIT_STEP_US;

      if (time_elapsed >= QUP_RUN_STATE_CHANGE_TIME_OUT_US)
      {
         SPIDEVICE_LOG(ERROR, "timeout waiting for run state to become valid");
         return(SPIDEVICE_ERR_INVALID_RUNSTATE);
      }
   }

   return(SPIDEVICE_RES_SUCCESS);
}

/*==================================================================================================

FUNCTION: write_block

DESCRIPTION:
   Write one block of data to the output FIFO.

ARGUMENTS PASSED:
   ctxt - device context

==================================================================================================*/
static void write_block(SpiDevice_DeviceCtxt *ctxt)
{
   uint8 *base = (uint8 *)ctxt->device_cfg.QupVirtAddr;
   SpiDeviceTransferType *xfer_ptr = &ctxt->transfer_info;
   uint32 data_word, i;
   SpiDeviceTransferDataResult result;

   for (i = 0; i < ctxt->hw_info.uNumOutputBlockSlots; i++)
   {
      result = xfer_ptr->getNextWord(xfer_ptr, &data_word);

      if (result == SPIDEVICE_TRANSFER_GET_DATA_NONE)
      {
         if (i == 0) { SPIDEVICE_LOG(ERROR, "insufficient data in write buffer"); }

         data_word = 0;  // write zeroes until we get to the block boundary
      }
      else if (result == SPIDEVICE_TRANSFER_GET_DATA_DONE)
      {
         xfer_ptr->bWriteBufferEmpty = TRUE;
      }

      HAL_qupe_PutWordIntoOutFifo(base, data_word);
   }
}

/*==================================================================================================
                                          GLOBAL FUNCTIONS
==================================================================================================*/
/*==================================================================================================

FUNCTION: SpiDevice_Init

DESCRIPTION:
   This function initializes the device and creates the necessary data structures
   to support other calls into the device.

ARGUMENTS PASSED:
   pConfig - pointer to device configuration
   phDev   - pointer to device handle

RETURNS:
   status - SPIDEVICE_RES_SUCCESS on success, error code on error

==================================================================================================*/
uint32 SpiDevice_Init(SpiDevice_Config *pConfig, SPIDEVICE_HANDLE *phDev)
{
   SpiDevice_DeviceCtxt *ctxt;
   SpiDeviceBam_Config bam_cfg;
   uint32 res;

   if (pConfig == NULL || phDev == NULL)
   {
      SPIDEVICE_LOG(ERROR, "SpiDevice_Init: NULL pointer passed");
      return(SPIDEVICE_ERR_INVALID_DEV_HANDLE);
   }

   *phDev = NULL;  // in case of error below

   res = IBUSOS_Malloc((void **)&ctxt, sizeof(SpiDevice_DeviceCtxt));

   if (res != SPIDEVICE_RES_SUCCESS)
   {
      SPIDEVICE_LOG(ERROR, "SpiDevice_Init: IBUSOS_Malloc failed (0x%lx)", res);
      return(res);
   }

   IBUSOS_Memset((void *)ctxt, 0, sizeof(SpiDevice_DeviceCtxt));
   IBUSOS_Memcpy(&ctxt->device_cfg, pConfig, sizeof(SpiDevice_Config));
   ctxt->init_state = SPIDEVICE_INITSTATE_UNINITIALIZED;

   res = IBUSPLAT_InitTarget(ctxt->device_cfg.QupHandle, &ctxt->plat_handle);

   if (res != SPIDEVICE_RES_SUCCESS)
   {
      SPIDEVICE_LOG(ERROR, "SpiDevice_Init: IBUSPLAT_InitTarget failed (0x%lx)", res);
      SpiDevice_DeInit(ctxt);
      return(res);
   }

   ctxt->init_state |= SPIDEVICE_INITSTATE_TARGET_INIT_DONE;

   if (ctxt->device_cfg.BamSupported)
   {
      bam_cfg.QupVirtAddr = ctxt->device_cfg.QupVirtAddr;
      bam_cfg.QupPhysAddr = ctxt->device_cfg.QupPhysAddr;
      bam_cfg.UseCmdDesc  = ctxt->device_cfg.UseCmdDesc;
      bam_cfg.PlatHandle  = ctxt->plat_handle;

      res = SpiDeviceBamInit(&bam_cfg, &ctxt->bam_handle);

      if (res != SPIDEVICE_RES_SUCCESS)
      {   
         SPIDEVICE_LOG(ERROR, "SpiDevice_Init: SpiDeviceBamInit failed (0x%lx)", res);
         SpiDevice_DeInit(ctxt);
         return(res);
      }

      ctxt->init_state |= SPIDEVICE_INITSTATE_BAM_INIT_DONE;
   }

   res = hardware_init(ctxt);

   if (res != SPIDEVICE_RES_SUCCESS)
   {
      SpiDevice_DeInit(ctxt);
      return(res);
   }

   if (ctxt->device_cfg.InterruptBased)
   {
      res = IBUSOS_CreateEvent(&ctxt->interrupt_event);

      if (res != SPIDEVICE_RES_SUCCESS)
      {
         SPIDEVICE_LOG(ERROR, "SpiDevice_Init: IBUSOS_CreateEvent failed (0x%lx)", res);
         SpiDevice_DeInit(ctxt);
         return(res);
      }

      ctxt->init_state |= SPIDEVICE_INITSTATE_EVENTS_DONE;

      res = IBUSOS_RegisterISTHooks(ctxt->device_cfg.QupHandle, SpiDevice_IsrHook, (void *)ctxt,
                                                                SpiDevice_IstHook, (void *)ctxt);

      if (res != SPIDEVICE_RES_SUCCESS)
      {
         SPIDEVICE_LOG(ERROR, "SpiDevice_Init: IBUSOS_RegisterISTHooks failed (0x%lx)", res);
         SpiDevice_DeInit(ctxt);
         return(res);
      }

      ctxt->init_state |= SPIDEVICE_INITSTATE_REGISTER_ISR_DONE;
   }

   *phDev = ctxt;
   return(SPIDEVICE_RES_SUCCESS);
}

/*==================================================================================================

FUNCTION: SpiDevice_DeInit

DESCRIPTION:
   This function deinitializes the device and releases resources acquired during init.

ARGUMENTS PASSED:
   hDev - device handle

RETURNS:
   status - SPIDEVICE_RES_SUCCESS on success, error code on error

==================================================================================================*/
uint32 SpiDevice_DeInit(SPIDEVICE_HANDLE hDev)
{
   SpiDevice_DeviceCtxt *ctxt = (SpiDevice_DeviceCtxt *)hDev;
   uint32 ret_val = SPIDEVICE_RES_SUCCESS;
   uint32 res;

   if (ctxt == NULL)
   {
      SPIDEVICE_LOG(ERROR, "SpiDevice_DeInit: NULL pointer passed");
      return(SPIDEVICE_ERR_INVALID_DEV_HANDLE);
   }

   if (ctxt->init_state & SPIDEVICE_INITSTATE_REGISTER_ISR_DONE)
   {
      res = IBUSOS_UnRegisterISTHooks(ctxt->device_cfg.QupHandle);

      if (res != SPIDEVICE_RES_SUCCESS)
      {
         SPIDEVICE_LOG(ERROR, "SpiDevice_DeInit: IBUSOS_UnRegisterISTHooks failed (0x%lx)", res);
         if (ret_val == SPIDEVICE_RES_SUCCESS) { ret_val = res; }  // return first error
      }
   }

   if (ctxt->init_state & SPIDEVICE_INITSTATE_EVENTS_DONE)
   {
      res = IBUSOS_CloseHandle(ctxt->interrupt_event);

      if (res != SPIDEVICE_RES_SUCCESS)
      {
         SPIDEVICE_LOG(ERROR, "SpiDevice_DeInit: IBUSOS_CloseHandle failed (0x%lx)", res);
         if (ret_val == SPIDEVICE_RES_SUCCESS) { ret_val = res; }  // return first error
      }
   }

   if (ctxt->init_state & SPIDEVICE_INITSTATE_BAM_INIT_DONE)
   {
      res = SpiDeviceBamDeInit(ctxt->bam_handle);

      if (res != SPIDEVICE_RES_SUCCESS)
      {
         SPIDEVICE_LOG(ERROR, "SpiDevice_DeInit: SpiDeviceBamDeInit failed (0x%lx)", res);
         if (ret_val == SPIDEVICE_RES_SUCCESS) { ret_val = res; }  // return first error
      }
   }

   if (ctxt->init_state & SPIDEVICE_INITSTATE_TARGET_INIT_DONE)
   {
      res = IBUSPLAT_DeInitTarget(ctxt->plat_handle);

      if (res != SPIDEVICE_RES_SUCCESS)
      {
         SPIDEVICE_LOG(ERROR, "SpiDevice_DeInit: IBUSPLAT_DeInitTarget failed (0x%lx)", res);
         if (ret_val == SPIDEVICE_RES_SUCCESS) { ret_val = res; }  // return first error
      }
   }

   res = IBUSOS_Free(ctxt);

   if (res != SPIDEVICE_RES_SUCCESS)
   {
      SPIDEVICE_LOG(ERROR, "SpiDevice_DeInit: IBUSOS_Free failed (0x%lx)", res);
      if (ret_val == SPIDEVICE_RES_SUCCESS) { ret_val = res; }  // return first error
   }

   return(ret_val);
}

/*==================================================================================================

FUNCTION: SpiDevice_WriteRead

DESCRIPTION:
   Perform a bi-directional transfer.  Read a buffer from SPI device and/or Write a buffer
   of data to a SPI device.

ARGUMENTS PASSED:
   hDev         - device handle
   pQupConfig   - pointer to the QUP config for data transfer
   pReadBuffer  - pointer where incoming data will be stored
   uReadBufLen  - number of bytes of valid data to be stored in pReadbuffer
   pWriteBuffer - pointer where outgoing data is stored
   uWriteBufLen - number of bytes of valid data stored in pWritebuffer

RETURNS:
   status - SPIDEVICE_RES_SUCCESS on success, error code on error

==================================================================================================*/
uint32 SpiDevice_WriteRead(SPIDEVICE_HANDLE   hDev,
                           SpiDeviceInfoType *pQupConfig,
                           SpiDataAddrType   *pReadBuffer,
                           uint32             uReadBufLen,
                           SpiDataAddrType   *pWriteBuffer,
                           uint32             uWriteBufLen)
{
   return SpiDevice_AsyncWriteRead(hDev, pQupConfig, pReadBuffer, uReadBufLen,
                                   pWriteBuffer, uWriteBufLen, NULL, NULL);
}

/*==================================================================================================

FUNCTION: SpiDevice_Read

DESCRIPTION:
   Read a buffer from spi device.

ARGUMENTS PASSED:
   hDev         - device handle
   pQupConfig   - pointer to the QUP config for data transfer
   pReadBuffer  - pointer where incoming data will be stored
   uReadBufLen  - number of bytes of valid data to be stored in pReadbuffer

RETURNS:
   status - SPIDEVICE_RES_SUCCESS on success, error code on error

==================================================================================================*/
uint32 SpiDevice_Read(SPIDEVICE_HANDLE   hDev,
                      SpiDeviceInfoType *pQupConfig,
                      SpiDataAddrType   *pReadBuffer,
                      uint32             uReadBufLen)
{
   return SpiDevice_AsyncWriteRead(hDev, pQupConfig, pReadBuffer, uReadBufLen,
                                   NULL, 0, NULL, NULL);
}

/*==================================================================================================

FUNCTION: SpiDevice_Write

DESCRIPTION:
   Write a buffer to spi device.

ARGUMENTS PASSED:
   hDev         - device handle
   pQupConfig   - pointer to the QUP config for data transfer
   pWriteBuffer - pointer where outgoing data is stored
   uWriteBufLen - number of bytes of valid data stored in pWritebuffer

RETURNS:
   status - SPIDEVICE_RES_SUCCESS on success, error code on error

==================================================================================================*/
uint32 SpiDevice_Write(SPIDEVICE_HANDLE   hDev,
                       SpiDeviceInfoType *pQupConfig,
                       SpiDataAddrType   *pWriteBuffer,
                       uint32             uWriteBufLen)
{
   return SpiDevice_AsyncWriteRead(hDev, pQupConfig, NULL, 0,
                                   pWriteBuffer, uWriteBufLen, NULL, NULL);
}

/*==================================================================================================

FUNCTION: SpiDevice_AsyncWriteRead

DESCRIPTION:
   Perform a bi-directional transfer.  Schedules a read/write/read-write(bi-directional)
   transfer.  Once the transfer is complete or an error occurs the callback will be called.

ARGUMENTS PASSED:
   hDev         - device handle
   pQupConfig   - pointer to the QUP config for data transfer
   pReadBuffer  - pointer where incoming data will be stored
   uReadBufLen  - number of bytes of valid data to be stored in pReadbuffer
   pWriteBuffer - pointer where outgoing data is stored
   uWriteBufLen - number of bytes of valid data stored in pWritebuffer
   pCallbackFn  - pointer to a callback function to be called when transfer finishes or aborts
   pArg         - pointer to be passed to the callback function

RETURNS:
   status - SPIDEVICE_RES_SUCCESS on success, error code on error

==================================================================================================*/
uint32 SpiDevice_AsyncWriteRead(SPIDEVICE_HANDLE   hDev,
                                SpiDeviceInfoType *pQupConfig,
                                SpiDataAddrType   *pReadBuffer,
                                uint32             uReadBufLen,
                                SpiDataAddrType   *pWriteBuffer,
                                uint32             uWriteBufLen,
                                SPI_DEV_ASYNC_CB   pCallbackFn,
                                void              *pArg)
{
   SpiDevice_DeviceCtxt *ctxt = (SpiDevice_DeviceCtxt *)hDev;
   SpiDevice_TransferDescriptor descriptor;
   boolean bam_mode;
   uint32 res;

   if (ctxt == NULL) { return SPIDEVICE_ERR_INVALID_DEV_HANDLE; }

   ctxt->async_data.pCallbackFn      = pCallbackFn;
   ctxt->async_data.asyncResult.pArg = pArg;

   bam_mode = (ctxt->device_cfg.BamSupported) &&
              (MAX(uReadBufLen, uWriteBufLen) >= ctxt->device_cfg.BamThreshold);

   res = build_descriptor_single(pQupConfig, bam_mode, pReadBuffer, uReadBufLen,
                                 pWriteBuffer, uWriteBufLen, &descriptor);

   if (res != SPIDEVICE_RES_SUCCESS) { return(res); }

   initialize_transfer_info(ctxt, &descriptor, NULL);

   res = start_transfer(ctxt, &descriptor, TRUE);

   if (res != SPIDEVICE_RES_SUCCESS) { return(res); }

   if (pCallbackFn == NULL)
   {
      wait_for_completion(ctxt);
      return(ctxt->transfer_info.transferResult);
   }
   else
   {
      return(SPIDEVICE_RES_SUCCESS);
   }
}

/*==================================================================================================

FUNCTION: SpiDevice_AsyncRead

DESCRIPTION:
   Schedules a buffer read from spi device.  Once the read is complete or an error occurs
   the callback will be called.

ARGUMENTS PASSED:
   hDev         - device handle
   pQupConfig   - pointer to the QUP config for data transfer
   pReadBuffer  - pointer where incoming data will be stored
   uReadBufLen  - number of bytes of valid data to be stored in pReadbuffer
   pCallbackFn  - pointer to a callback function to be called when transfer finishes or aborts
   pArg         - pointer to be passed to the callback function

RETURNS:
   status - SPIDEVICE_RES_SUCCESS on success, error code on error

==================================================================================================*/
uint32 SpiDevice_AsyncRead(SPIDEVICE_HANDLE   hDev,
                           SpiDeviceInfoType *pQupConfig,
                           SpiDataAddrType   *pReadBuffer,
                           uint32             uReadBufLen,
                           SPI_DEV_ASYNC_CB   pCallbackFn,
                           void              *pArg)
{
   return SpiDevice_AsyncWriteRead(hDev, pQupConfig, pReadBuffer, uReadBufLen,
                                   NULL, 0, pCallbackFn, pArg);
}

/*==================================================================================================

FUNCTION: SpiDevice_AsyncWrite

DESCRIPTION:
   Schedules a buffer write to spi device. Once the write is complete or an error occurs
   the callback will be called.

ARGUMENTS PASSED:
   hDev         - device handle
   pQupConfig   - pointer to the QUP config for data transfer
   pWriteBuffer - pointer where outgoing data is stored
   uWriteBufLen - number of bytes of valid data stored in pWritebuffer
   pCallbackFn  - pointer to a callback function to be called when transfer finishes or aborts
   pArg         - pointer to be passed to the callback function

RETURNS:
   status - SPIDEVICE_RES_SUCCESS on success, error code on error

==================================================================================================*/
uint32 SpiDevice_AsyncWrite(SPIDEVICE_HANDLE   hDev,
                            SpiDeviceInfoType *pQupConfig,
                            SpiDataAddrType   *pWriteBuffer,
                            uint32             uWriteBufLen,
                            SPI_DEV_ASYNC_CB   pCallbackFn,
                            void              *pArg)
{
   return SpiDevice_AsyncWriteRead(hDev, pQupConfig, NULL, 0,
                                   pWriteBuffer, uWriteBufLen, pCallbackFn, pArg);
}

/*==================================================================================================

FUNCTION: SpiDevice_BatchTransfer

DESCRIPTION:
   Does a batch of transfers in a sequence.

   NOTE: This function has not had a use case for a while,
         therefore it's not been tested as the code has evolved.
         This needs to be tested thoroughly as we move forward as it maybe
         broken.

ARGUMENTS PASSED:
   hDev             - device handle
   hTransfer        - array of transfers
   uNumTransferVecs - number of elements in the array
   pQupConfig       - pointer to the QUP config for data transfer
   puNumCompleted   - pointer to return number of completed transfers

RETURNS:
   status - SPIDEVICE_RES_SUCCESS on success, error code on error

==================================================================================================*/
uint32 SpiDevice_BatchTransfer(SPIDEVICE_HANDLE       hDev,
                               SpiLinkedTransferType *pTransfer,
                               uint32                 uNumTransferVecs,
                               SpiDeviceInfoType     *pQupConfig,
                               uint32                *puNumCompleted)
{
   SpiDevice_DeviceCtxt *ctxt = (SpiDevice_DeviceCtxt *)hDev;
   SpiDevice_TransferDescriptor descriptor;
   uint32 i, total_bytes = 0;
   boolean bam_mode, eot;
   uint32 res;

   if (ctxt == NULL)           { return SPIDEVICE_ERR_INVALID_DEV_HANDLE; }
   if (pTransfer == NULL)      { return SPIDEVICE_ERR_INVALID_PTR; }
   if (uNumTransferVecs == 0)  { return SPIDEVICE_ERR_INVALID_NUM_BATCH_TRANSFER_VECS; }
   if (puNumCompleted == NULL) { return SPIDEVICE_ERR_INVALID_PTR; }

   for (i = 0; i < uNumTransferVecs; i++)
   {
      total_bytes += MAX(pTransfer[i].pReadBuffVec->uLen, pTransfer[i].pWriteBuffVec->uLen);
   }

   bam_mode = (ctxt->device_cfg.BamSupported) &&
              (total_bytes >= ctxt->device_cfg.BamThreshold);

   *puNumCompleted = 0;

   for (i = 0; i < uNumTransferVecs; i++)
   {
      eot = (bam_mode && ctxt->device_cfg.UseCmdDesc && (i != uNumTransferVecs - 1)) ?
            (FALSE) : (TRUE);

      res = build_descriptor_batch(pQupConfig, bam_mode, pTransfer + i, &descriptor);

      if (res != SPIDEVICE_RES_SUCCESS) { break; }

      initialize_transfer_info(ctxt, &descriptor, pTransfer + i);

      res = start_transfer(ctxt, &descriptor, eot);

      if (res != SPIDEVICE_RES_SUCCESS) { break; }

      if (eot)
      {
         wait_for_completion(ctxt);
         res = ctxt->transfer_info.transferResult;
         if (res != SPIDEVICE_RES_SUCCESS) { break; }
      }

      (*puNumCompleted)++;
   }

   if (bam_mode && (res != SPIDEVICE_RES_SUCCESS))
   {
      ctxt->bam_handle->cancelBamTransfer(ctxt->bam_handle, &ctxt->transfer_info);
   }

   return res;
}

/*==================================================================================================

FUNCTION: SpiDevice_CancelTransfer

DESCRIPTION:
   Cancels ongoing transfer by resetting the core.

ARGUMENTS PASSED:
   hDev - device handle

==================================================================================================*/
void SpiDevice_CancelTransfer(SPIDEVICE_HANDLE hDev)
{
   SpiDevice_DeviceCtxt *ctxt = (SpiDevice_DeviceCtxt *)hDev;
   uint8 *base = (uint8 *)ctxt->device_cfg.QupVirtAddr;

   HAL_qupe_Reset(base);
   wait_for_run_state_valid(base);
   HAL_qupe_SetSelectedMinicore(base, HAL_QUPE_MINI_CORE_SPI);
   ctxt->transfer_info.transferResult = SPIDEVICE_TRANSFER_CANCELED;
}

/*==================================================================================================

FUNCTION: SpiDevice_SetPowerState

DESCRIPTION:
   This function sets the power state for the device.

ARGUMENTS PASSED:
   hDev        - device handle
   ePowerState - new power state

RETURNS:
   status - SPIDEVICE_RES_SUCCESS on success, error code on error

==================================================================================================*/
uint32 SpiDevice_SetPowerState(SPIDEVICE_HANDLE hDev, SpiDevice_PowerStates ePowerState)
{
   SpiDevice_DeviceCtxt *ctxt = (SpiDevice_DeviceCtxt *)hDev;

   return (int32)IBUSPLAT_SetPowerState(ctxt->plat_handle, ePowerState);
}

/*==================================================================================================

FUNCTION: SpiDevice_GetHwInfo

DESCRIPTION:
   Gets the HW device information.

ARGUMENTS PASSED:
   hDev    - device handle
   spiInfo - SPI information

RETURNS:
   status - SPIDEVICE_RES_SUCCESS

==================================================================================================*/
uint32 SpiDevice_GetHwInfo(SPIDEVICE_HANDLE hDev, SpiInfoType *spiInfo)
{
   SpiDevice_DeviceCtxt *ctxt = (SpiDevice_DeviceCtxt *)hDev;

   spiInfo->uInputBlockSize  = ctxt->hw_info.uNumInputBlockSlots * 4;
   spiInfo->uOutputBlockSize = ctxt->hw_info.uNumOutputBlockSlots * 4;
   spiInfo->uInputFifoSize   = ctxt->hw_info.uNumInputBlockSlots * 4
                                  * ctxt->hw_info.uNumInputBlocks;
   spiInfo->uOutputFifoSize  = ctxt->hw_info.uNumOutputBlockSlots * 4
                                  * ctxt->hw_info.uNumOutputBlocks;

   return SPIDEVICE_RES_SUCCESS;
}

/*==================================================================================================

FUNCTION: SpiDevice_IsrHook

DESCRIPTION:
   QUP ISR (interrupt service routine).  This function should be called in interrupt
   context when a QUP interrupt is signaled.  It is used for block mode only.  It is
   responsible for moving data to/from the HW FIFOs and signaling when the transfer
   completes.

ARGUMENTS PASSED:
   pdata - device context

RETURNS:
   0

==================================================================================================*/
uint32 SpiDevice_IsrHook(void *pdata)
{
   SpiDevice_DeviceCtxt *ctxt = (SpiDevice_DeviceCtxt *)pdata;
   SpiDeviceTransferType *xfer_ptr = &ctxt->transfer_info;
   uint8 *base = (uint8 *)ctxt->device_cfg.QupVirtAddr;
   uint32 qup_operational;
   uint32 qup_error_flags;
   uint32 input_service;
   uint32 output_service;

   // According to the HW designer both the QUP error flags and the minicore
   // status register need to be read and then the interrupts can be cleared.
   // If status registers are not read then the interrupt will remain set.
   // I have seen this happen for the interrupts asserted by the minicore.

   for (;;)
   {
      qup_error_flags = HAL_qupe_GetErrStatusFlags(base);
      qup_operational = HAL_qupe_GetOpStatusFlags(base);
      output_service  = qup_operational & HAL_QUPE_OPSTATE_OUTPUT_SERVICE;
      input_service   = qup_operational & HAL_QUPE_OPSTATE_INPUT_SERVICE;

      if (qup_error_flags)
      {
         SPIDEVICE_LOG(ERROR, "QUP FIFO error detected (0x%08X)", qup_error_flags);
         xfer_ptr->uErrorFlagsNew = qup_error_flags;
         HAL_qupe_Reset(base);
         wait_for_run_state_valid(base);
         HAL_qupe_SetSelectedMinicore(base, HAL_QUPE_MINI_CORE_SPI);
         signal_completion(ctxt, SPIDEVICE_FIFO_ERROR);
         return 0;
      }

      if (!output_service && !input_service) { break; }

      // HW bug: We must clear the service flags before servicing the FIFOs.
      // If we service the FIFO first, and a large enough delay occurs before
      // clearing the service flag, then the HW will get into a bad state.
      // We will not get the next block request and the transfer will stall.

      HAL_qupe_ClearOpStatusFlags(base, output_service | input_service);

      if (qup_operational & HAL_QUPE_OPSTATE_OUT_BLOCK_WRITE_REQ) { write_block(ctxt); }
      if (qup_operational & HAL_QUPE_OPSTATE_IN_BLOCK_READ_REQ)   {  read_block(ctxt); }
   }

   if (qup_operational & HAL_QUPE_OPSTATE_MAX_OUTPUT_DONE) { xfer_ptr->bWriteTransferDone = 1; }
   if (qup_operational & HAL_QUPE_OPSTATE_MAX_INPUT_DONE)  { xfer_ptr->bReadTransferDone  = 1; }

   if (xfer_ptr->bWriteTransferDone && xfer_ptr->bReadTransferDone)
   {
      change_state_to_reset(base);
      signal_completion(ctxt, SPIDEVICE_RES_SUCCESS);
   }

   return 0;
}

/*==================================================================================================

FUNCTION: SpiDevice_IstHook

DESCRIPTION:
   QUP IST (interrupt service thread).  This function was used when interrupt servicing
   was split into two parts (one part to clear the interrupt and one part to read/write
   the FIFOs).  It is currently not used.

ARGUMENTS PASSED:
   pdata - device context

RETURNS:
   0

==================================================================================================*/
uint32 SpiDevice_IstHook(void *pdata)
{
   SPIDEVICE_UNREFERENCED_PARAMETER(pdata);  // suppress compiler warning
   return 0;
}

/*==================================================================================================

FUNCTION: SpiDevice_BamCallback

DESCRIPTION:
   This callback function is used to signal completion of a BAM transfer.

ARGUMENTS PASSED:
   pResult -

==================================================================================================*/
void SpiDevice_BamCallback(SpiDeviceBamResult *pResult)
{
   if (pResult)
   {
      signal_completion((SpiDevice_DeviceCtxt *)pResult->pUser, pResult->result);
   }
}

/*==================================================================================================

FUNCTION: SpiDevice_SetCSControl

DESCRIPTION:
  This function configures control of the chip select.  It can be controlled manually (forced
  high or low) or it can be controlled automatically by the core according to the SPI protocol.
  
ARGUMENTS PASSED:
   hDev        - device handle
   uChipSelect - chip select line (0 - 3)
   eControl    - selected control method (high/low/auto)

RETURNS:
   status - SPIDEVICE_RES_SUCCESS on success, error code on error

==================================================================================================*/
uint32 SpiDevice_SetCSControl(SPIDEVICE_HANDLE hDev, uint32 uChipSelect, SpiCSControl eControl)
{
   SpiDevice_DeviceCtxt *ctxt = (SpiDevice_DeviceCtxt *)hDev;
   uint8 *base = (uint8 *)ctxt->device_cfg.QupVirtAddr;
   boolean force_assert, active_low;
   uint32 res;

   switch (eControl)
   {
      case CS_HIGH: force_assert = TRUE;  active_low = FALSE; break;
      case CS_LOW:  force_assert = TRUE;  active_low = TRUE;  break;
      case CS_AUTO: force_assert = FALSE; active_low = TRUE;  break;
      default: return(SPIDEVICE_INVALID_PARAMETER);
   }

   HAL_qupe_SetRunState(base, HAL_QUPE_RUNSTATE_RESET);
   res = wait_for_run_state_valid(base);
   if (res != SPIDEVICE_RES_SUCCESS) { return(res); }

   HAL_qupe_Spi_ConfigureChipSelect(base, uChipSelect, force_assert, active_low);

   return(SPIDEVICE_RES_SUCCESS);
}
