#ifndef _SPIDEVICEBAM_H_
#define _SPIDEVICEBAM_H_
/*==================================================================================================

FILE: SpiDeviceBam.h

DESCRIPTION:
   This file contains the definitions for the SPI BAM mode operation.

====================================================================================================

   Edit History

When      Who  What, where, why
--------  ---  ------------------------------------------------------------
10/03/13  vmk  BAM CMD descriptor usage is determined from properties
03/28/12  ddk  Added de-init API.
02/26/12  ag   Created

====================================================================================================
                     Copyright (c) 2012-2014 Qualcomm Technologies Incorporated
                                        All Rights Reserved
                                     Qualcomm Proprietary/GTDR
==================================================================================================*/

#include "SpiDeviceTypes.h"
#include "SpiDevicePlatSvc.h"
#include "SpiDeviceTransfer.h"
#include "HALqupe.h"

/*==================================================================================================
                                              TYPEDEFS
==================================================================================================*/

typedef enum
{
   SPIDEVICE_BAM_READ_TAG,
   SPIDEVICE_BAM_WRITE_TAG,
   SPIDEVICE_BAM_CMD_TAG,
} SpiDeviceBamTags;

typedef void* SpiDeviceBamCmdParms;

typedef void (*SPIDEVICE_BAM_TRANSFER_CB)(SpiDeviceBamResult *);

typedef struct
{
   void            *QupVirtAddr;  // QUP virtual address
   void            *QupPhysAddr;  // QUP physical address
   boolean          UseCmdDesc;   // TRUE to use command descriptors for register configuration
   IBUSPLAT_HANDLE  PlatHandle;   // platform services handle
} SpiDeviceBam_Config;

typedef struct
{
   void    *physMemHandle;
   void    *physAddr;
   void    *virtualAddr;
   uint32  cmd_element_index;
   uint32  size_in_bytes;
} SpiDeviceCmdDescInfo;

typedef struct SpiBamPlatCBArgs SpiBamPlatCBArgs;  // forward declaration due to circular reference
typedef struct SpiDeviceBamType SpiDeviceBamType;  // forward declaration due to circular reference

struct SpiBamPlatCBArgs
{
   uint32 bamTags;
   SpiDeviceBamType *spiBamObj;
};

struct SpiDeviceBamType
{
   SpiDeviceBam_Config bam_cfg;
   SpiDeviceTransferType *transfer;
   SPIDEVICE_BAM_TRANSFER_CB pCallbackFn;
   HAL_QUPE_SPI_TransferParametersType mHalTransferParameters;
   HAL_QUPE_SPI_DeviceParametersType mHalDeviceParameters;
   SpiDeviceBamResult result;
   SpiDeviceBamCmdParms *cmdParms;
   SpiBamPlatCBArgs readArgs;
   SpiBamPlatCBArgs writeArgs;
   SpiDeviceCmdDescInfo resetCmdDesc;
   SpiDeviceCmdDescInfo settingsCmdDesc;
   boolean eot;
   uint32 (*startBamTransfer)    (SpiDeviceBamType *spiBam, SpiDeviceTransferType *transfer,
                                  void *pUser, boolean eot);
   void (*cancelBamTransfer)     (SpiDeviceBamType *spiBam, SpiDeviceTransferType *transfer);
   void (*setBamTransferCallback)(SpiDeviceBamType *spiBam, SPIDEVICE_BAM_TRANSFER_CB pCallBackFn);
   void (*setBamTransferCmdParms)(SpiDeviceBamType *spiBam, SpiDeviceBamCmdParms *cmdParms);
   void (*setQupTransferParms)   (SpiDeviceBamType *spiBam,
                                  HAL_QUPE_SPI_TransferParametersType *mHalTransferParameters);
   void (*setQupDeviceParms)     (SpiDeviceBamType *spiBam,
                                  HAL_QUPE_SPI_DeviceParametersType *mHalDeviceParameters);
};

/*==================================================================================================
                                        FUNCTION PROTOTYPES
==================================================================================================*/

/*  @brief Initializes data structures and returns handle.
 *
 *  @param[in]  bam_cfg  pointer to BAM configuration
 *  @param[out] spiBam   pointer to BAM handle
 *  @return              error status
 */
uint32 SpiDeviceBamInit(SpiDeviceBam_Config  *bam_cfg,
                        SpiDeviceBamType    **spiBam);

/*  @brief Releases data structures.
 *
 *  @param[in] spiBam  BAM handle
 *  @return            error status
 */
uint32 SpiDeviceBamDeInit(SpiDeviceBamType *spiBam);

#endif  // _SPIDEVICEBAM_H_
