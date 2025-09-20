#ifndef _SPIDEVICETRANSFER_H
#define _SPIDEVICETRANSFER_H
/*==================================================================================================

FILE: SpiDeviceTransfer.h

DESCRIPTION:

====================================================================================================

   Edit History

When      Who  What, where, why
--------  ---  -----------------------------------------------------------
12/10/12  ag   Created.

====================================================================================================
                     Copyright (c) 2011-2014 Qualcomm Technologies Incorporated
                                        All Rights Reserved
                                     Qualcomm Proprietary/GTDR
==================================================================================================*/

#include "SpiDeviceTypes.h"
#include "SpiDevice.h"

/*==================================================================================================
                                              TYPEDEFS
==================================================================================================*/

typedef enum
{
   SPIDEVICE_TRANSFER_SINGLE_BUFF_DATA,
   SPIDEVICE_TRANSFER_LINKED_BUFF_DATA,
} SpiTransferDataType;

typedef enum
{
   SPIDEVICE_TRANSFER_GET_DATA_SUCCESS,
   SPIDEVICE_TRANSFER_GET_DATA_PARTIAL,
   SPIDEVICE_TRANSFER_GET_DATA_NONE,
   SPIDEVICE_TRANSFER_GET_DATA_DONE,
   SPIDEVICE_TRANSFER_SET_DATA_SUCCESS,
   SPIDEVICE_TRANSFER_SET_DATA_PARTIAL,
   SPIDEVICE_TRANSFER_SET_DATA_NONE,
   SPIDEVICE_TRANSFER_SET_DATA_DONE,
   SPIDEVICE_TRANSFER_BUFF_AVAILABLE,
   SPIDEVICE_TRANSFER_BUFF_LAST,
   SPIDEVICE_TRANSFER_BUFF_NONE,
} SpiDeviceTransferDataResult;

typedef struct
{
   uint8 *curReadPtr;
   uint8 *curReadPtrPhys;
   uint32 uReadAvail;
   uint8 *curWritePtr;
   uint8 *curWritePtrPhys;
   uint32 uWriteAvail;
} SpiDeviceSingleBuffData;

typedef struct
{
   SpiBufferType *curReadVector;
   uint8 *curReadPtr;
   uint8 *curReadPtrPhys;
   uint32 uCurReadAvail;
   uint32 uReadVectorAvail;
   SpiBufferType *curWriteVector;
   uint8 *curWritePtr;
   uint8 *curWritePtrPhys;
   uint32 uCurWriteAvail;
   uint32 uWriteVectorAvail;
} SpiDeviceLinkedBuffData;

// forward declaration for self reference
typedef struct SpiDeviceTransferType SpiDeviceTransferType;
struct SpiDeviceTransferType
{
   SpiTransferDataType eDataType;
   boolean bWriteTransferDone;  // flag set to TRUE when the output transfer is complete by HW
   boolean bWriteBufferEmpty;   // flag set to TRUE when ALL data from user buffer is read
   boolean bReadTransferDone;   // flag set to TRUE when the input transfer is complete by HW
   boolean bReadBufferFull;     // flag set to TRUE when input data buffer is full
   uint32 uErrorFlagsOld;       // QUP_ERROR_FLAGS from a previous transfer  (for debug purposes)
   uint32 uErrorFlagsNew;       // QUP_ERROR_FLAGS from the current transfer (for debug purposes)
   uint32 transferResult;       // the status/result of the transaction

   union 
   {
      SpiDeviceSingleBuffData sData;
      SpiDeviceLinkedBuffData lData;
   } data;

   /* Meant for block and FIFO operations */
   SpiDeviceTransferDataResult (*getNextWord)(SpiDeviceTransferType *transfer, uint32 *buf);
   SpiDeviceTransferDataResult (*setNextWord)(SpiDeviceTransferType *transfer, uint32 *buf);
   SpiDeviceTransferDataResult (*getNextByte)(SpiDeviceTransferType *transfer, uint8 *buf);
   SpiDeviceTransferDataResult (*setNextByte)(SpiDeviceTransferType *transfer, uint8 *buf);

   /* Meant for BAM or DM operations */
   SpiDeviceTransferDataResult (*getReadBuf)     (SpiDeviceTransferType *transfer, uint8 **buf);
   SpiDeviceTransferDataResult (*getReadBufPhys) (SpiDeviceTransferType *transfer, uint8 **buf);
   uint32                      (*getReadBufLen)  (SpiDeviceTransferType *transfer);
   SpiDeviceTransferDataResult (*moveReadNext)   (SpiDeviceTransferType *transfer);
   SpiDeviceTransferDataResult (*getWriteBuf)    (SpiDeviceTransferType *transfer, uint8 **buf);
   SpiDeviceTransferDataResult (*getWriteBufPhys)(SpiDeviceTransferType *transfer, uint8 **buf);
   SpiDeviceTransferDataResult (*moveWriteNext)  (SpiDeviceTransferType *transfer);
   uint32                      (*getWriteBufLen) (SpiDeviceTransferType *transfer);
};

/*==================================================================================================
                                        FUNCTION PROTOTYPES
==================================================================================================*/

/*  @brief Initializes a single buffer transfer.
 *
 *  @param[out] transfer        pointer to transfer structure
 *  @param[in]  readBuf         read buffer virtual address
 *  @param[in]  readBufPhys     read buffer physical address
 *  @param[in]  uTotalReadLen   read buffer size
 *  @param[in]  writeBuf        write buffer virtual address
 *  @param[in]  writeBufPhys    write buffer physical address
 *  @param[in]  uTotalWriteLen  write buffer size
 *  @return                     void
 */
void SpiDeviceSingleBufferTransferInit(SpiDeviceTransferType *transfer, 
                                       uint8                 *readBuf, 
                                       uint8                 *readBufPhys, 
                                       uint32                 uTotalReadLen, 
                                       uint8                 *writeBuf, 
                                       uint8                 *writeBufPhys, 
                                       uint32                 uTotalWriteLen);

/*  @brief Initializes a linked buffer transfer.
 *
 *  @param[out] transfer       pointer to transfer structure
 *  @param[in]  readVector     vector of read buffers
 *  @param[in]  uNumReadVecs   number of buffers in the read vector
 *  @param[in]  writeVector    vector of write buffers
 *  @param[in]  uNumWriteVecs  number of buffers in the write vector
 *  @return                    void
 */
void SpiDeviceLinkedBufferTransferInit(SpiDeviceTransferType *transfer, 
                                       SpiBufferType         *readVector,
                                       uint32                 uNumReadVecs,
                                       SpiBufferType         *writeVector,
                                       uint32                 uNumWriteVecs);

#endif  // _SPIDEVICETRANSFER_H
