/*==================================================================================================

FILE: SpiDalProps.h

DESCRIPTION:
   The file declares structures and functions to fetch device configuration properties 

   ====================================================================================================

  Edit History
   

When      Who  What, where, why
--------  ---  ------------------------------------------------------------
04/01/14  vmk  Initial version

====================================================================================================
                     Copyright (c) 2014 Qualcomm Technologies Incorporated
                                        All Rights Reserved
                                     Qualcomm Proprietary/GTDR
==================================================================================================*/

#include "SpiDeviceTypes.h"
#include "com_dtypes.h"

typedef struct SpiDevProp_DevTargetCfgType
{
   char *pQupHClkName;
   char *pQupAppClkName;
   void *uOsDevId;
   uint32 uQupCoreNum;
   uint8  *qupVirtBlockAddr;
   uint8  *qupPhysBlockAddr;
   uint8  *tcsrVirtBlockAddr;
   boolean bInterruptBased;
   boolean bBamSupported;
   boolean bTcsrInterruptRequired;
   boolean bUseCmdDesc;
   uint32 uTcsrInterruptBitMask;
   uint32 uTcsrInterruptBitShift;
   uint32 uActiveDevCount;
   boolean bPnocEnable;
   uint64 uPnocIBVal;
   uint64 uPnocABVal;
   uint32 uPnocMaster;
   uint32 uPnocSlave;
}SpiDevProp_DevTargetCfgType;

#define SPIDEVICEPLAT_CHK_RESULT_BREAK(res) if (SPIDEVICE_RES_SUCCESS !=(res)) break;

uint32 SpiDevProp_GetConfig(void *QupHandle, void *conf);
boolean SpiDevProp_SearchDevCfg (void *devId, SpiDevProp_DevTargetCfgType **ppDevCfg);
int32 SpiDevProp_RemoveDevCfg (void *uOsDevId);
