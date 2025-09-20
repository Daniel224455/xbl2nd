/*==================================================================================================

FILE: SpiDalProps.c

DESCRIPTION:
   The file implements functions to fetch device configuration properties 

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
#include "SpiDalProps.h"
#include "SpiDevice.h"
#include "DALDeviceId.h"
#include "DDIHWIO.h"
#include "SpiLog.h"
#include "com_dtypes.h"

#define SPI_BAM_THRESHOLD_BYTES 64 //size below which BAM mode will not be used.

static DalDeviceHandle *phDalHWIO;
static uint8 *uPeriphSsVirtAddr = NULL, *uPeriphSsPhysAddr = NULL, *uTcsrVirtAddr = NULL;

typedef struct SpiDevProp_DevTargetCfgLinkType SpiDevProp_DevTargetCfgLinkType;

struct SpiDevProp_DevTargetCfgLinkType
{
   SpiDevProp_DevTargetCfgLinkType *pNextLink;
   SpiDevProp_DevTargetCfgType devCfg;
};

static struct
{
   SpiDevProp_DevTargetCfgLinkType *pHead;
   uint32 numDevCfgs;
} SpiDevProp_DevTgtCfgList;

enum SpiDevProp_Error
{
   SPI_DEV_PROP_ERROR_DAL_GET_PROPERTY_HANDLE,
   SPI_DEV_PROP_ERROR_DAL_GET_PROPERTY_VALUE,
   SPI_DEV_PROP_ERROR_FAILED_TO_MAP_BLOCK_HWIO,
   SPI_DEV_PROP_ERROR_ATTACH_TO_DALHWIO,
   SPI_DEV_PROP_ERROR_REMOVE_DEVCFG_NOT_FOUND
};

static int32 SpiDevProp_GetDalProperties(SpiDevProp_DevTargetCfgType *pDev)
{
   DALSYS_PROPERTY_HANDLE_DECLARE(hProp);
   DALSYSPropertyVar PropVar;
   int32 res = 0;
   uint8 *pBasePtr;
   do
   {
      if (DAL_SUCCESS != DALSYS_GetDALPropertyHandle((uint32) (size_t) pDev->uOsDevId, hProp)) 
      {
         res = SPI_DEV_PROP_ERROR_DAL_GET_PROPERTY_HANDLE;
         break;
      }
      if (DAL_SUCCESS != DALSYS_GetPropertyValue(hProp, "CHIP_BUS_INDEX", 0, &PropVar)) 
      {
         break;
      }
      pDev->uQupCoreNum = (uint32)PropVar.Val.dwVal;

      if (DAL_SUCCESS != DALSYS_GetPropertyValue(hProp, "SW_USE_POLLING_MODE", 0, &PropVar))
      {
         break;
      }
      pDev->bInterruptBased = ((uint32)PropVar.Val.dwVal == 1) ? 0 : 1;

      if (DAL_SUCCESS != DALSYS_GetPropertyValue(hProp, "SW_ENABLE_BAM", 0, &PropVar))
      {
         break;
      }
      pDev->bBamSupported = ((uint32)PropVar.Val.dwVal == 1) ? 1 : 0;

      if (NULL == pDev->pQupAppClkName)
      {
         if (DAL_SUCCESS != DALSYS_GetPropertyValue(hProp, "CHIP_APP_CLK_NAME", 0, &PropVar)) 
         {
            res = SPI_DEV_PROP_ERROR_DAL_GET_PROPERTY_VALUE;
            break;
         }
         pDev->pQupAppClkName = PropVar.Val.pszVal;
      }

      if (NULL == pDev->pQupHClkName)
      {
         if (DAL_SUCCESS != DALSYS_GetPropertyValue(hProp, "CHIP_HCLK_NAME", 0, &PropVar)) 
         {
            res = SPI_DEV_PROP_ERROR_DAL_GET_PROPERTY_VALUE;
            break;
         }
         pDev->pQupHClkName = PropVar.Val.pszVal;
      }

      if (DAL_SUCCESS != DALSYS_GetPropertyValue(hProp, "CHIP_PERIPH_SS_BASE",0, &PropVar)) 
      {
         res = SPI_DEV_PROP_ERROR_DAL_GET_PROPERTY_VALUE;
         break;
      }
      uPeriphSsPhysAddr = (uint8 *) (size_t) PropVar.Val.dwVal;

      if (NULL == uPeriphSsVirtAddr)
      {
         if (DAL_SUCCESS != DALSYS_GetPropertyValue(hProp, "CHIP_PERIPH_SS_BASE_NAME",0, &PropVar)) 
         {
            res = SPI_DEV_PROP_ERROR_DAL_GET_PROPERTY_VALUE;
            break;
         }

         if (DAL_SUCCESS != DalHWIO_MapRegion(phDalHWIO, PropVar.Val.pszVal, &pBasePtr))
         {
            res = SPI_DEV_PROP_ERROR_FAILED_TO_MAP_BLOCK_HWIO;
            break;
         }
         uPeriphSsVirtAddr = pBasePtr;
      }

      if (DAL_SUCCESS != DALSYS_GetPropertyValue(hProp, "CHIP_BLOCK_OFFSET",0, &PropVar)) 
      {
         res = SPI_DEV_PROP_ERROR_DAL_GET_PROPERTY_VALUE;
         break;
      }
      pDev->qupVirtBlockAddr = (uint32)PropVar.Val.dwVal + uPeriphSsVirtAddr;
      pDev->qupPhysBlockAddr = (uint32)PropVar.Val.dwVal + uPeriphSsPhysAddr;

      if (DAL_SUCCESS == DALSYS_GetPropertyValue(hProp, "TCR_INTERRUPT_REQUIRED", 0, &PropVar))
      {
         if (1 == (uint32)PropVar.Val.dwVal)
         {
            pDev->bTcsrInterruptRequired = 1;
            if (0 == uTcsrVirtAddr)
            {

               if (DAL_SUCCESS != DALSYS_GetPropertyValue(hProp, "CHIP_TCSR_BASE_NAME",0, &PropVar))
               {
                  break;
               }
               if (DAL_SUCCESS != DalHWIO_MapRegion(phDalHWIO, PropVar.Val.pszVal, &pBasePtr))
               {
                  res = SPI_DEV_PROP_ERROR_FAILED_TO_MAP_BLOCK_HWIO;
                  break;
               }
               uTcsrVirtAddr = pBasePtr;
            }

            if (DAL_SUCCESS != DALSYS_GetPropertyValue(hProp, "CHIP_TCSR_BLOCK_OFFSET", 0, &PropVar)) 
            {
               break;
            }
            pDev->tcsrVirtBlockAddr = (uint32)PropVar.Val.dwVal + uTcsrVirtAddr;

            if (DAL_SUCCESS != DALSYS_GetPropertyValue(hProp, "CHIP_TCSR_ENABLE_MASK", 0, &PropVar)) 
            {
               break;
            }
            pDev->uTcsrInterruptBitMask = (uint32)PropVar.Val.dwVal;

            if (DAL_SUCCESS != DALSYS_GetPropertyValue(hProp, "CHIP_TCSR_ENABLE_SHIFT", 0, &PropVar))
            {
               break;
            }
            pDev->uTcsrInterruptBitShift = (uint32)PropVar.Val.dwVal;
         }
      }

      if (DAL_SUCCESS == DALSYS_GetPropertyValue(hProp, "USE_CMD_DESCRIPTORS", 0, &PropVar))
      {
         pDev->bUseCmdDesc = ((uint32)PropVar.Val.dwVal == 1) ? 1 : 0;
      } 

      /*PNOC settings*/
      if (DAL_SUCCESS == DALSYS_GetPropertyValue(hProp, "PNOC_VOTE_ENABLE", 0, &PropVar))
      {
         if (1 == (uint32)PropVar.Val.dwVal)
         {
            pDev->bPnocEnable = 1;

            if (DAL_SUCCESS != DALSYS_GetPropertyValue(hProp, "PNOC_IB_VAL", 0, &PropVar)) break;
            pDev->uPnocIBVal = (uint64)PropVar.Val.dwVal;

            if (DAL_SUCCESS != DALSYS_GetPropertyValue(hProp, "PNOC_AB_VAL", 0, &PropVar)) break;
            pDev->uPnocABVal = (uint64)PropVar.Val.dwVal;

            if (DAL_SUCCESS != DALSYS_GetPropertyValue(hProp, "PNOC_ARB_MASTER", 0, &PropVar)) break;
            pDev->uPnocMaster = PropVar.Val.dwVal;

            if (DAL_SUCCESS != DALSYS_GetPropertyValue(hProp, "PNOC_ARB_SLAVE", 0, &PropVar)) break;
            pDev->uPnocSlave = PropVar.Val.dwVal;
         }
      }

      if (DAL_SUCCESS == DALSYS_GetPropertyValue(hProp, "LOGGING_LEVEL", 0, &PropVar))
      {
         SpiLog_SetLogLevel(PropVar.Val.dwVal);
      }

      if (pDev->bBamSupported)
      {
         SpiDevProp_BamDevCfgType *devCfg = NULL;

         res = DALSYS_Malloc(sizeof(SpiDevProp_BamDevCfgType), (void **)&devCfg);

         SPIDEVICEPLAT_CHK_RESULT_BREAK(res);

         DALSYS_memset((void *)devCfg, 0, sizeof(SpiDevProp_BamDevCfgType));

         /* BAM related properties */
         if (DAL_SUCCESS != DALSYS_GetPropertyValue(hProp, "BAM_BLSP_ID", 0, &PropVar))
         {
            res = SPI_DEV_PROP_ERROR_DAL_GET_PROPERTY_VALUE;
            break;
         }
         devCfg->uBamDevId = (uint32)PropVar.Val.dwVal;

         if (DAL_SUCCESS != DALSYS_GetPropertyValue(hProp, "BAM_BLOCK_OFFSET", 0, &PropVar))
         {
            res = SPI_DEV_PROP_ERROR_DAL_GET_PROPERTY_VALUE;
            break;
         }
         devCfg->devBamConfig.bam_pa = (uint64) (uPeriphSsPhysAddr + (uint32)PropVar.Val.dwVal);

         if (DAL_SUCCESS != DALSYS_GetPropertyValue(hProp, "BAM_INTRERRUPT_ID", 0, &PropVar))
         {
            res = SPI_DEV_PROP_ERROR_DAL_GET_PROPERTY_VALUE;
            break;
         }
         devCfg->devBamConfig.bam_irq = (uint32)PropVar.Val.dwVal;

         if (DAL_SUCCESS != DALSYS_GetPropertyValue(hProp, "BAM_TX_PIPE_NUM", 0, &PropVar))
         {
            res = SPI_DEV_PROP_ERROR_DAL_GET_PROPERTY_VALUE;
            break;
         }
         devCfg->devBamTxPipeCtxt.devBamPipeNum = (uint32)PropVar.Val.dwVal;

         if (DAL_SUCCESS != DALSYS_GetPropertyValue(hProp, "BAM_RX_PIPE_NUM", 0, &PropVar))
         {
            res = SPI_DEV_PROP_ERROR_DAL_GET_PROPERTY_VALUE;
            break;
         }
         devCfg->devBamRxPipeCtxt.devBamPipeNum = (uint32)PropVar.Val.dwVal;
         devCfg->devBamCallbackCfg.func = NULL; // will be updated during call to IBUSPLAT_BamTransfer
         pDev->spiBamDevCfg = devCfg;
      } //end if (pDev->bBamSupported)

      res = SPIDEVICE_RES_SUCCESS;
   }
   while (0);

   if (res != SPIDEVICE_RES_SUCCESS)
      SPIDEVICE_LOG(ERROR, "SpiDevProp_GetDalProperties: Failure err 0x%x\n", (unsigned int)res);
   return res;
}

boolean SpiDevProp_SearchDevCfg (void *devId, SpiDevProp_DevTargetCfgType **ppDevCfg)
{
   SpiDevProp_DevTargetCfgLinkType *pLink;

   if (NULL == SpiDevProp_DevTgtCfgList.pHead)
   {
      return FALSE;
   }
   pLink = SpiDevProp_DevTgtCfgList.pHead;
   do
   {
      if (pLink->devCfg.uOsDevId == devId)
      {
         *ppDevCfg = &pLink->devCfg;
         return TRUE;
      }
      pLink = pLink->pNextLink;
   }
   while (NULL != pLink);

   return FALSE;
}

static int32 SpiDevProp_AddDevCfg (void *uOsDevId, SpiDevProp_DevTargetCfgType **ppDevCfg)
{
   int32 res;
   SpiDevProp_DevTargetCfgLinkType *pLink;

   res = DALSYS_Malloc(sizeof(SpiDevProp_DevTargetCfgLinkType), (void **)&pLink);

   if (SPIDEVICE_RES_SUCCESS != res)
   {
      SPIDEVICE_LOG(ERROR, "SpiDevProp_AddDevCfg: Malloc failure\n");
      return res;
   }
   DALSYS_memset((void *)pLink, 0, sizeof(SpiDevProp_DevTargetCfgLinkType));

   pLink->pNextLink = SpiDevProp_DevTgtCfgList.pHead;
   SpiDevProp_DevTgtCfgList.pHead = pLink;
   pLink->devCfg.uOsDevId = uOsDevId;
   *ppDevCfg = &pLink->devCfg;

   return SPIDEVICE_RES_SUCCESS;
}

uint32 SpiDevProp_GetConfig(void *QupHandle, void *conf)
{
   SpiDevProp_DevTargetCfgType *pDev;
   int32 res = SPIDEVICE_RES_SUCCESS;
   void *uOsDevId = QupHandle;
   SpiDevice_Config *config = (SpiDevice_Config *)conf;
   do
   {
      if (SpiDevProp_SearchDevCfg(uOsDevId, &pDev))
      {
         res = SPIDEVICE_RES_SUCCESS;
         break;
      }
      if (SPIDEVICE_RES_SUCCESS != (res = SpiDevProp_AddDevCfg(uOsDevId, &pDev)))
      {
         break;
      }

      if (NULL == phDalHWIO)
      {
         if ((DAL_SUCCESS != DAL_DeviceAttach(DALDEVICEID_HWIO, &phDalHWIO)) || (NULL == phDalHWIO))
         {
            res = SPI_DEV_PROP_ERROR_ATTACH_TO_DALHWIO;
            break;
         }
      }

      if (SPIDEVICE_RES_SUCCESS != (res = SpiDevProp_GetDalProperties(pDev)))
      {
         break;
      }
      config->BamSupported = pDev->bBamSupported;
      config->BamThreshold = SPI_BAM_THRESHOLD_BYTES;
      config->InterruptBased = pDev->bInterruptBased;
      config->QupPhysAddr = pDev->qupPhysBlockAddr;
      config->QupVirtAddr = pDev->qupVirtBlockAddr;
      config->UseCmdDesc = pDev->bUseCmdDesc;
      config->TcsrVirtAddr = pDev->tcsrVirtBlockAddr;
      config->TcsrInterruptBitMask = pDev->uTcsrInterruptBitMask;
      config->TcsrInterruptBitShift = pDev->uTcsrInterruptBitShift;
   }
   while (0);
   return res;
}

int32 SpiDevProp_RemoveDevCfg (void *uOsDevId)
{
   int32 res = SPI_DEV_PROP_ERROR_REMOVE_DEVCFG_NOT_FOUND;
   SpiDevProp_DevTargetCfgLinkType *pLink;
   SpiDevProp_DevTargetCfgLinkType *oldpLink = NULL;

   if (NULL == SpiDevProp_DevTgtCfgList.pHead)
   {
      return res;
   }
   pLink = SpiDevProp_DevTgtCfgList.pHead;

   do
   {
      if (pLink->devCfg.uOsDevId == uOsDevId)
      {
         if (SpiDevProp_DevTgtCfgList.pHead == pLink)
         {
            SpiDevProp_DevTgtCfgList.pHead = pLink->pNextLink;
         } else
         {
            if (NULL != oldpLink)
            {
               oldpLink->pNextLink = pLink->pNextLink;
            }
         }
         SpiDevProp_DevTgtCfgList.numDevCfgs--;
         res = DALSYS_Free(pLink);
         break;
      }
      oldpLink = pLink;
      pLink = pLink->pNextLink;
   }
   while (NULL != pLink);


   if (SPIDEVICE_RES_SUCCESS != res)
   {
      SPIDEVICE_LOG(ERROR, "SpiDevProp_RemoveDevCfg: Free failure\n");
      return res;
   }

   return SPIDEVICE_RES_SUCCESS;
}
