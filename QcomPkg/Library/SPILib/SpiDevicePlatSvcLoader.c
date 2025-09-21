/*
===========================================================================

FILE:   SpiDevicePlatSvcLoader.c

DESCRIPTION:

===========================================================================

        Edit History


When     Who    What, where, why
-------- ---    -----------------------------------------------------------
03/19/15 unr    Created for XBL Loader

===========================================================================
              Copyright c 2015 Qualcomm Technologies Incorporated.
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
#include "HALtlmm.h"
#include "HALhwio.h"
#include "HALbootHWIO.h"
#include "SpiDalPropsLoader.h"
#include "SpiLog.h"
#include "DALDeviceId.h"
#include "api/systemdrivers/ClockBoot.h"

#define SPI_ICB_CLIENT_CNT              1
#define SPI_PNOC_MSTRSLV_PAIRS_NUM      1
#define SPI_DEVICE_BAM_DESC_SUM_THRESHOLD (32*1024)
#define SPI_DEVICE_BAM_MAX_DESC_SIZE     0x100

/* boot_gpio_configure:link issues in JtagProgrammer. copying implementation */
void plat_gpio_configure(uint32 gpio_number,
                         uint32 gpio_function_select,
                         HAL_tlmm_PullType gpio_pull_type,
                         HAL_tlmm_DriveType gpio_drive_strength,
                         HAL_tlmm_DirType gpio_output_enable)
{
  /* Set output enable */
  HWIO_TLMM_GPIO_CFGn_OUTMI(gpio_number,
                            HWIO_TLMM_GPIO_CFGn_GPIO_OE_BMSK,
                            gpio_output_enable << HWIO_TLMM_GPIO_CFGn_GPIO_OE_SHFT);

  /* Set the drive strength */
  HWIO_TLMM_GPIO_CFGn_OUTMI(gpio_number,
                            HWIO_TLMM_GPIO_CFGn_DRV_STRENGTH_BMSK,
                            gpio_drive_strength << HWIO_TLMM_GPIO_CFGn_DRV_STRENGTH_SHFT);

  /* Set the function select value */
  HWIO_TLMM_GPIO_CFGn_OUTMI(gpio_number,
                            HWIO_TLMM_GPIO_CFGn_FUNC_SEL_BMSK,
                            gpio_function_select << HWIO_TLMM_GPIO_CFGn_FUNC_SEL_SHFT);

  /* Set the pull type */
  HWIO_TLMM_GPIO_CFGn_OUTMI(gpio_number,
                            HWIO_TLMM_GPIO_CFGn_GPIO_PULL_BMSK,
                            gpio_pull_type << HWIO_TLMM_GPIO_CFGn_GPIO_PULL_SHFT);
}

typedef enum IBUSCFG_Error
{
   IBUSPLAT_ERROR_NULL_PTR = SPIDEVICE_RES_ERROR_CLS_PLATFORM

} IBUSCFG_Error;

static DALResult IBUSPLAT_InitSpiGpios (SpiDevProp_DevTargetCfgType *pDev)
{
   DALSYS_PROPERTY_HANDLE_DECLARE(hProp);
   DALSYSPropertyVar PropVar;
   uint32 clkSig, mosiSig, misoSig;
   DALSYSPropertyVar csSig;
   DALBOOL bCsGPIOUsed = FALSE;
   csSig.dwType = DALSYS_PROP_TYPE_UINT32_PTR;
   csSig.dwLen = 0;

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

   plat_gpio_configure(HAL_GPIO_NUMBER(clkSig),
                       HAL_FUNC_VAL(clkSig),
                       HAL_PULL_VAL(clkSig),
                       HAL_DRVSTR_VAL(clkSig),
                       HAL_DIR_VAL(clkSig));

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
            plat_gpio_configure(HAL_GPIO_NUMBER(csSig.Val.pdwVal[uIdx]),
                                HAL_FUNC_VAL(csSig.Val.pdwVal[uIdx]),
                                HAL_PULL_VAL(csSig.Val.pdwVal[uIdx]),
                                HAL_DRVSTR_VAL(csSig.Val.pdwVal[uIdx]),
                                HAL_DIR_VAL(csSig.Val.pdwVal[uIdx]));
         }
      }
   }

   plat_gpio_configure(HAL_GPIO_NUMBER(mosiSig),
                       HAL_FUNC_VAL(mosiSig),
                       HAL_PULL_VAL(mosiSig),
                       HAL_DRVSTR_VAL(mosiSig),
                       HAL_DIR_VAL(mosiSig));

   plat_gpio_configure(HAL_GPIO_NUMBER(misoSig),
                       HAL_FUNC_VAL(misoSig),
                       HAL_PULL_VAL(misoSig),
                       HAL_DRVSTR_VAL(misoSig),
                       HAL_DIR_VAL(misoSig));

   return DAL_SUCCESS;
}


static int32 IBUSPLAT_ClkInit(SpiDevProp_DevTargetCfgType *pDevTgtCfg)
{

   uint32 frequency;

   switch (pDevTgtCfg->uQupCoreNum)
   {
      case 0:
         frequency = Clock_SetSPIClockFrequency(CLK_BLSP0_QUP0_SPI_APPS, 20000);
         break;
      case 1:
         frequency = Clock_SetSPIClockFrequency(CLK_BLSP0_QUP1_SPI_APPS, 20000);
         break;
      case 2:
         frequency = Clock_SetSPIClockFrequency(CLK_BLSP3_QUP0_SPI_APPS, 20000);
         break;
      case 3:
         frequency = Clock_SetSPIClockFrequency(CLK_BLSP3_QUP1_SPI_APPS, 20000);
         break;
   }
      
   if (!frequency)
   { 
      return SPIDEVICE_ERR_CLOCK_SETTING_FAILED;
   }

   return SPIDEVICE_RES_SUCCESS;
}

static uint32 IBUSPLAT_AddPNOCVote (SpiDevProp_DevTargetCfgType *pDev)
{
   return SPIDEVICE_RES_SUCCESS;
}

uint32 IBUSPLAT_InitTarget(void *QupHandle, IBUSPLAT_HANDLE *phPlat)
{
   SpiDevProp_DevTargetCfgType *pDev = NULL;
   int32 res = SPIDEVICE_RES_SUCCESS;

   do
   {
      if (FALSE == SpiDevProp_SearchDevCfg(QupHandle, &pDev))
      {
         break;
      }

      if (SPIDEVICE_RES_SUCCESS != (res = IBUSPLAT_ClkInit(pDev)))
      {
         break;
      }

      if (SPIDEVICE_RES_SUCCESS != (res = IBUSPLAT_InitSpiGpios(pDev)))
      {
         break;
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
   return SPIDEVICE_RES_SUCCESS;
}

uint32 IBUSPLAT_DeInitTarget(IBUSPLAT_HANDLE hPlat)
{
   int32 res = SPIDEVICE_RES_SUCCESS;
   SpiDevProp_DevTargetCfgType *pDev = (SpiDevProp_DevTargetCfgType *)hPlat;

   if (pDev == NULL)
   {
      return IBUSPLAT_ERROR_NULL_PTR;
   }

   res = SpiDevProp_RemoveDevCfg(pDev->uOsDevId);

   pDev->uActiveDevCount--;

   return res;
}

uint32 IBUSPLAT_SetAppClkHz (IBUSPLAT_HANDLE hPlat, uint32 reqFrequencyHz, uint32 *pFinalFrequencyHz)
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
