/*
===========================================================================

FILE:   SpiDevicePlatSvc.c

DESCRIPTION:

===========================================================================

        Edit History


When     Who    What, where, why
-------- ---    -----------------------------------------------------------
10/03/13 vmk    BAM CMD descriptor usage is determined from properties
08/19/13 sg     Added explicit vote for PNOC clock.
05/30/12 ddk    Added support for correct deassert wait calculation.
05/26/12 ddk    Added support for GPIO initialisation.
04/20/12 ddk    Enabled Clock initialisation in driver.
04/20/12 ddk    Added updates to enable logging.
04/19/12 ddk    Added changes for avoiding symbol match with I2C library.
04/13/12 ddk    Added de-init changes.
03/28/12 ddk    Added QUP physical address info for BAM cmd desc creation.
03/27/12 ddk    Fixed DAL clock API usage.
03/27/12 ag     Added SPI BAM mode requirements.
09/26/11 ag     Created (Based on Spi implementation by LK)

===========================================================================
          Copyright c 2011 - 2014 Qualcomm Technologies Incorporated.
                              All Rights Reserved.
                           Qualcomm Proprietary/GTDR
===========================================================================
*/

#include "SpiDevicePlatSvc.h"
#include "SpiDeviceOsSvc.h"
#include "SpiDeviceError.h"
#include "SpiDevice.h"
#include "DALStdDef.h"
#include "DALSys.h"
#include "DDITlmm.h"
#include "SpiDalProps.h"
#include "SpiLog.h"

#define SPI_ICB_CLIENT_CNT              1
#define SPI_PNOC_MSTRSLV_PAIRS_NUM      1
#define SPI_DEVICE_BAM_DESC_SUM_THRESHOLD (32*1024)
#define SPI_DEVICE_BAM_MAX_DESC_SIZE     0x100

typedef enum IBUSCFG_Error
{
   IBUSPLAT_ERROR_ATTACH_TO_CLOCKS = SPIDEVICE_RES_ERROR_CLS_PLATFORM,
   IBUSPLAT_ERROR_GETTING_CLK_ID,
   IBUSPLAT_ERROR_INVALID_POWER_STATE,
   IBUSPLAT_ERROR_FAILED_TO_SET_APPCLK_FREQ,
   IBUSPLAT_ERROR_FAILED_TO_ENABLE_APPCLK,
   IBUSPLAT_ERROR_FAILED_TO_DISABLE_APPCLK,
   IBUSPLAT_ERROR_FAILED_TO_ENABLE_HCLK,
   IBUSPLAT_ERROR_FAILED_TO_DISABLE_HCLK,
   IBUSPLAT_ERROR_TLMM_ATTACH,
   IBUSPLAT_ERROR_TLMM_OPEN,
   IBUSPLAT_ERROR_NULL_PTR,
   IBUSPLAT_ERROR_FAILED_TO_CREATE_PNOC_CLIENT,
   IBUSPLAT_ERROR_FAILED_TO_ISSUE_PNOC_REQ,

   /*BAM errors*/
   IBUSPLAT_ERROR_BAM_INIT,
   IBUSPLAT_ERROR_BAM_RESET,
} IBUSCFG_Error;

#define QUPBUSOSSVC_NUM_DEVICES_MAX       12
int32 aQupDalDevIds [QUPBUSOSSVC_NUM_DEVICES_MAX] = { 
   DALDEVICEID_SPI_DEVICE_1, DALDEVICEID_SPI_DEVICE_2, DALDEVICEID_SPI_DEVICE_3,
   DALDEVICEID_SPI_DEVICE_4, DALDEVICEID_SPI_DEVICE_5, DALDEVICEID_SPI_DEVICE_6,
   DALDEVICEID_SPI_DEVICE_7, DALDEVICEID_SPI_DEVICE_8, DALDEVICEID_SPI_DEVICE_9,
   DALDEVICEID_SPI_DEVICE_10, DALDEVICEID_SPI_DEVICE_11, DALDEVICEID_SPI_DEVICE_12 };

typedef enum IBUSPLAT_PowerStates
{
   IBUSPLAT_POWER_STATE_0, /**< Bus and clocks off. */
   IBUSPLAT_POWER_STATE_1, /**< Bus on clocks off. */
   IBUSPLAT_POWER_STATE_2, /**< Bus on clocks on. */
} IBUSPLAT_PowerStates;

static DalDeviceHandle *pClkHandle;
static DalDeviceHandle *pTlmmHandle;

static uint32 IBUSPLAT_InitBam(SpiDevProp_DevTargetCfgType *);
static int32 IBUSPLAT_ClkDeInit(SpiDevProp_DevTargetCfgType *pDevTgtCfg);

static DALResult IBUSPLAT_InitSpiGpios (SpiDevProp_DevTargetCfgType *pDev)
{
   DALSYS_PROPERTY_HANDLE_DECLARE(hProp);
   DALSYSPropertyVar PropVar;
   uint32 clkSig, mosiSig, misoSig;
   DALSYSPropertyVar csSig;
   DALBOOL bCsGPIOUsed = FALSE;
   csSig.dwType = DALSYS_PROP_TYPE_UINT32_PTR;
   csSig.dwLen = 0;

   if (NULL == pTlmmHandle)
   {
      SPIDEVICE_LOG(ERROR, "spi pTlmmHandle not initialised");
      return DAL_ERROR;
   }

   if (DAL_SUCCESS == DALSYS_GetDALPropertyHandle((uint32) (UINTN) pDev->uOsDevId, hProp))
   {
      if (DAL_SUCCESS == DALSYS_GetPropertyValue (hProp, "gpio_sig_spi_clk", 0,&PropVar))
      {
         clkSig = PropVar.Val.dwVal;
      }
      else
      {
         SPIDEVICE_LOG(ERROR, "spi fail to get clk gpio number");
         return DAL_ERROR;
      }

      if (DAL_SUCCESS == DALSYS_GetPropertyValue (hProp, "gpio_sig_spi_cs", 0, &PropVar))
      {
         csSig = PropVar;
         bCsGPIOUsed = TRUE;
      }
      else
      {
         SPIDEVICE_LOG(ERROR, "No GPIO assigned for spi cs");

         /* We do not fail here. Not having a GPIO for CS maybe intentional.
          * For example, 3 wire config.
          */
      }

      if (DAL_SUCCESS == DALSYS_GetPropertyValue (hProp, "gpio_sig_spi_mosi", 0, &PropVar))
      {
         mosiSig = PropVar.Val.dwVal;
      }
      else
      {
         SPIDEVICE_LOG(ERROR, "spi fail to get mosi gpio number");
         return DAL_ERROR;
      }

      if (DAL_SUCCESS == DALSYS_GetPropertyValue (hProp, "gpio_sig_spi_miso", 0, &PropVar))
      {
         misoSig = PropVar.Val.dwVal;
      }
      else
      {
         SPIDEVICE_LOG(ERROR, "spi fail to get miso gpio number");
         return DAL_ERROR;
      }
   }
   else
   {
      SPIDEVICE_LOG(ERROR, "spi ConfigGPIO fail to get property handle");
      return DAL_ERROR;
   }
   /* ready to enable gpio line */
   if (DAL_SUCCESS != DalTlmm_ConfigGpio(pTlmmHandle, (DALGpioSignalType)clkSig, DAL_TLMM_GPIO_ENABLE))
   {
      SPIDEVICE_LOG(ERROR, "spi ConfigGPIO fail on clkSig");
      return DAL_ERROR;
   }

   if (bCsGPIOUsed)
   {
      uint32 uIdx;
      if (csSig.dwLen > 0)
      {
         if (csSig.dwType != DALSYS_PROP_TYPE_UINT32_PTR)
         {
            SPIDEVICE_LOG(ERROR, "spi ConfigGPIO fail on csSig");
            return DAL_ERROR;
         }
         /* configure all the GPIOs for CS */
         for (uIdx = 0; uIdx < csSig.dwLen; ++uIdx)
         {
            if (DAL_SUCCESS != DalTlmm_ConfigGpio(pTlmmHandle, (DALGpioSignalType)csSig.Val.pdwVal[uIdx], DAL_TLMM_GPIO_ENABLE))
            {
               SPIDEVICE_LOG(ERROR, "spi ConfigGPIO fail on csSig");
               return DAL_ERROR;
            }
         }
      }
   }

   if (DAL_SUCCESS != DalTlmm_ConfigGpio(pTlmmHandle, (DALGpioSignalType)mosiSig, DAL_TLMM_GPIO_ENABLE))
   {
      SPIDEVICE_LOG(ERROR, "spi ConfigGPIO fail on mosiSig");
      return DAL_ERROR;
   }

   if (DAL_SUCCESS != DalTlmm_ConfigGpio(pTlmmHandle, (DALGpioSignalType)misoSig, DAL_TLMM_GPIO_ENABLE))
   {
      SPIDEVICE_LOG(ERROR, "spi ConfigGPIO fail on misoSig");
      return DAL_ERROR;
   }

   return DAL_SUCCESS;
}


static int32 IBUSPLAT_ClkInit(SpiDevProp_DevTargetCfgType *pDevTgtCfg)
{
   DALResult dalRes;
   int32 res = SPIDEVICE_RES_SUCCESS;

   do
   {
      dalRes = DalClock_GetClockId(pClkHandle, pDevTgtCfg->pQupAppClkName, &pDevTgtCfg->QupAppClkId);

      if (DAL_SUCCESS != dalRes)
      {
         res = IBUSPLAT_ERROR_GETTING_CLK_ID;
         break;
      }

      dalRes = DalClock_GetClockId(pClkHandle, pDevTgtCfg->pQupHClkName, &pDevTgtCfg->QupHClkId);

      if (DAL_SUCCESS != dalRes)
      {
         res = IBUSPLAT_ERROR_GETTING_CLK_ID;
         break;
      }


      /* Enable QUP Clock */
      dalRes = DalClock_EnableClock(pClkHandle, pDevTgtCfg->QupAppClkId);
      if (DAL_SUCCESS != dalRes)
      {
         res = IBUSPLAT_ERROR_FAILED_TO_ENABLE_APPCLK;
         break;
      }

      /* Enable HCLK */
      dalRes = DalClock_EnableClock(pClkHandle, pDevTgtCfg->QupHClkId);
      if (DAL_SUCCESS != dalRes)
      {
         res = IBUSPLAT_ERROR_FAILED_TO_ENABLE_HCLK;
         break;
      }
   }
   while (0);

   if (res != SPIDEVICE_RES_SUCCESS)
   {
      SPIDEVICE_LOG(ERROR, "IBUSPLAT_ClkInit: Failure err 0x%x\n", (unsigned int)dalRes);
   }

   return res;
}



static uint32 IBUSPLAT_AddPNOCVote (SpiDevProp_DevTargetCfgType *pDev)
{
   return SPIDEVICE_RES_SUCCESS;
}

static uint32 IBUSPLAT_RemovePNOCVote(SpiDevProp_DevTargetCfgType *pDev)
{
   return SPIDEVICE_RES_SUCCESS;
}

uint32 IBUSPLAT_InitTarget(void *QupHandle, IBUSPLAT_HANDLE *phPlat)
{
   DALResult dalRes;
   SpiDevProp_DevTargetCfgType *pDev = NULL;
   int32 res = SPIDEVICE_RES_SUCCESS;

   do
   {
      if (FALSE == SpiDevProp_SearchDevCfg(QupHandle, &pDev))
      {
         break;
      }

      if (NULL == pClkHandle)
      {
         dalRes = DAL_ClockDeviceAttach(DALDEVICEID_CLOCK, &pClkHandle);
         if ((DAL_SUCCESS != dalRes) || (NULL == pClkHandle))
         {
            res = IBUSPLAT_ERROR_ATTACH_TO_CLOCKS;
            break;
         }
      }
      if (NULL == pTlmmHandle)
      {
         if (DAL_SUCCESS != DAL_DeviceAttachEx(NULL, DALDEVICEID_TLMM,
                                               DALTLMM_INTERFACE_VERSION, &pTlmmHandle))
         {
            res = IBUSPLAT_ERROR_TLMM_ATTACH;
            break;
         }

         if (DAL_SUCCESS != DalDevice_Open(pTlmmHandle, DAL_OPEN_SHARED))
         {
            DAL_DeviceDetach(pTlmmHandle);
            pTlmmHandle = NULL;
            res = IBUSPLAT_ERROR_TLMM_OPEN;
            break;
         }
      }

      if (SPIDEVICE_RES_SUCCESS != (res = IBUSPLAT_ClkInit(pDev)))
      {
         break;
      }

      if (SPIDEVICE_RES_SUCCESS != (res = IBUSPLAT_InitSpiGpios(pDev)))
      {
         break;
      }

      if (pDev->bBamSupported)
      {
         IBUSPLAT_InitBam(pDev);
      }

      if (TRUE == pDev->bPnocEnable)
      {
         if (SPIDEVICE_RES_SUCCESS != (res = IBUSPLAT_AddPNOCVote(pDev)))
         {
            break;
         }
      }
   }
   while (0);

   if (res == SPIDEVICE_RES_SUCCESS)
   {
      *phPlat = (IBUSPLAT_HANDLE)pDev;
      pDev->uActiveDevCount++;
      SPIDEVICE_LOG(INFO,"IBUSPLAT_InitTarget: uActiveDevCount 0x%x\n",(unsigned int)pDev->uActiveDevCount);
   } 
   else
   {
      SPIDEVICE_LOG(ERROR,"IBUSPLAT_InitTarget: Failure err 0x%x\n",(unsigned int)res);
   }
   return res;
}

uint32 IBUSPLAT_SetPowerState(IBUSPLAT_HANDLE hPlat, uint32 state)
{

   SpiDevProp_DevTargetCfgType *pDev = (SpiDevProp_DevTargetCfgType *)hPlat;

   /* Bus state is OFF */
   if (IBUSPLAT_POWER_STATE_0 == state)
   {
      return IBUSPLAT_ClkDeInit(pDev);
   }
   /* Bus state is POWERED */
   else if (IBUSPLAT_POWER_STATE_1 == state)
   {
      return IBUSPLAT_ClkDeInit(pDev);
   }
   /* Bus state is ACTIVE */
   else if (IBUSPLAT_POWER_STATE_2 == state)
   {
      return IBUSPLAT_ClkInit(pDev);
   }
   else
   {
      return IBUSPLAT_ERROR_INVALID_POWER_STATE;
   }
}

uint32 IBUSPLAT_DeInitTarget(IBUSPLAT_HANDLE hPlat)
{
   int32 res = SPIDEVICE_RES_SUCCESS;
   SpiDevProp_DevTargetCfgType *pDev = (SpiDevProp_DevTargetCfgType *)hPlat;

   do
   {
      if (pDev == NULL)
      {
         res = IBUSPLAT_ERROR_NULL_PTR;
         break;
      }

      IBUSPLAT_SetPowerState(hPlat, IBUSPLAT_POWER_STATE_2);
      if (pDev->uActiveDevCount == 1)
      {
         if (TRUE == pDev->bPnocEnable)
         {
            res = IBUSPLAT_RemovePNOCVote(pDev);
            SPIDEVICEPLAT_CHK_RESULT_BREAK(res);
         }
      }
      IBUSPLAT_SetPowerState(hPlat, IBUSPLAT_POWER_STATE_0);

      res = SpiDevProp_RemoveDevCfg(pDev->uOsDevId);
      SPIDEVICEPLAT_CHK_RESULT_BREAK(res);

      pDev->uActiveDevCount--;
   }
   while (0);

   return res;
}

uint32 IBUSPLAT_SetAppClkHz (IBUSPLAT_HANDLE hPlat, uint32 reqFrequencyHz, uint32 *pFinalFrequencyHz)
{
   SpiDevProp_DevTargetCfgType *pDev = (SpiDevProp_DevTargetCfgType *)hPlat;
   DALResult dalRes;

   dalRes = DalClock_SetClockFrequency(pClkHandle,
                                       pDev->QupAppClkId,
                                       reqFrequencyHz,
                                       CLOCK_FREQUENCY_HZ_AT_MOST,
                                       pFinalFrequencyHz);

   if (DAL_SUCCESS != dalRes)
   {
      return IBUSPLAT_ERROR_FAILED_TO_SET_APPCLK_FREQ;
   }

   return SPIDEVICE_RES_SUCCESS;
}


static int32 IBUSPLAT_ClkDeInit(SpiDevProp_DevTargetCfgType *pDevTgtCfg)
{
   DALResult dalRes;
   int32 res = SPIDEVICE_RES_SUCCESS;

   /* Disable QUP Clock */
   dalRes = DalClock_DisableClock(pClkHandle, pDevTgtCfg->QupAppClkId);
   if (DAL_SUCCESS != dalRes)
   {
      res = IBUSPLAT_ERROR_FAILED_TO_DISABLE_APPCLK;
   }

   /* Disable HCLK */
   dalRes = DalClock_DisableClock(pClkHandle, pDevTgtCfg->QupHClkId);
   if (DAL_SUCCESS != dalRes)
   {
      res = IBUSPLAT_ERROR_FAILED_TO_DISABLE_HCLK;
   }

   return res;
}

static uint32 IBUSPLAT_InitBam(SpiDevProp_DevTargetCfgType *pDev)
{
   return SPIDEVICE_RES_SUCCESS;
}

uint32 IBUSPLAT_ResetBam(IBUSPLAT_HANDLE hPlat)
{
   return SPIDEVICE_RES_SUCCESS;
}

uint32 IBUSPLAT_BamTransfer(IBUSPLAT_HANDLE         hPlat,
                            boolean                 bWrite,
                            void                   *pAddr,
                            uint32                  uSize,
                            void                   *pUser,
                            uint32                  uFlags,
                            SPIDEVICE_BAM_CALLBACK  pCallBackFn)
{
   return SPIDEVICE_RES_SUCCESS;
}

uint32 IBUSPLAT_GetQupNum(void *QupHandle)
{
   SpiDevProp_DevTargetCfgType *pDev = NULL;
   if (FALSE == SpiDevProp_SearchDevCfg(QupHandle, &pDev))
   {
      return (uint32)(-1); 
   }
   return pDev->uQupCoreNum;
}
