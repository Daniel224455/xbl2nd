/*============================================================================
  FILE:         VAdcHal.c

  OVERVIEW:     Implementation of device HAL for VADC

  DEPENDENCIES: None

                Copyright (c) 2012, 2014-2015, 2017-2018 Qualcomm Technologies, Inc.
                All Rights Reserved.
                Qualcomm Technologies Proprietary and Confidential.
============================================================================*/
/*============================================================================
  EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.  Please
  use ISO format for dates.


  when        who  what, where, why
  ----------  ---  -----------------------------------------------------------
  2015-08-12  jjo  Updated for VADC refresh.

============================================================================*/
/*----------------------------------------------------------------------------
 * Include Files
 * -------------------------------------------------------------------------*/
#include "VAdcHal.h"
#include "VAdcHalPmio.h"
#include "pm_pmio.h"

/*----------------------------------------------------------------------------
 * Preprocessor Definitions and Constants
 * -------------------------------------------------------------------------*/
#define ARRAY_LENGTH(a) (sizeof(a) / sizeof((a)[0]))

/*----------------------------------------------------------------------------
 * Type Declarations
 * -------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 * Static Function Declarations
 * -------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 * Global Data Definitions
 * -------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 * Static Variable Definitions
 * -------------------------------------------------------------------------*/
static const uint8 gaucRegDumpAddresses[VADC_HAL_NUM_REGISTERS_TO_DUMP] =
{
   PMIO_ADDRX(0, REVISION1),
   PMIO_ADDRX(0, REVISION2),
   PMIO_ADDRX(0, REVISION3),
   PMIO_ADDRX(0, REVISION4),
   PMIO_ADDRX(0, PERPH_TYPE),
   PMIO_ADDRX(0, PERPH_SUBTYPE),
   PMIO_ADDRX(0, STATUS1),
   PMIO_ADDRX(0, STATUS2),
   PMIO_ADDRX(0, INT_RT_STS),
   PMIO_ADDRX(0, INT_SET_TYPE),
   PMIO_ADDRX(0, INT_POLARITY_HIGH),
   PMIO_ADDRX(0, INT_POLARITY_LOW),
   PMIO_ADDRX(0, INT_EN_SET),
   PMIO_ADDRX(0, INT_EN_CLR),
   PMIO_ADDRX(0, INT_LATCHED_STS),
   PMIO_ADDRX(0, INT_PENDING_STS),
   PMIO_ADDRX(0, INT_MID_SEL),
   PMIO_ADDRX(0, INT_PRIORITY),
   PMIO_ADDRX(0, ADC_DIG_PARAM),
   PMIO_ADDRX(0, FAST_AVG_CTL),
   PMIO_ADDRX(0, ADC_CH_SEL_CTL),
   PMIO_ADDRX(0, DELAY_CTL),
   PMIO_ADDRX(0, EN_CTL1),
   PMIO_ADDRX(0, DATA0),
   PMIO_ADDRX(0, DATA1),
};

/*----------------------------------------------------------------------------
 * Static Function Declarations and Definitions
 * -------------------------------------------------------------------------*/
static uint32 VAdcHalGetSettlingDelayUs(VAdcHalInterfaceType *piVAdcHal, VAdcSettlingDelay eSettlingDelay)
{
   if (piVAdcHal->revisionInfo.usDigitalRev < VADC_REVISION(5,3))
   {
      switch (eSettlingDelay)
      {
         case VADC_SETTLING_DELAY_100_US:
            return 100;
         case VADC_SETTLING_DELAY_200_US:
            return 200;
         case VADC_SETTLING_DELAY_300_US:
            return 300;
         case VADC_SETTLING_DELAY_400_US:
            return 400;
         case VADC_SETTLING_DELAY_500_US:
            return 500;
         case VADC_SETTLING_DELAY_600_US:
            return 600;
         case VADC_SETTLING_DELAY_700_US:
            return 700;
         case VADC_SETTLING_DELAY_800_US:
            return 800;
         case VADC_SETTLING_DELAY_900_US:
            return 900;
         case VADC_SETTLING_DELAY_1_MS:
            return 1000;
         case VADC_SETTLING_DELAY_2_MS:
            return 2000;
         case VADC_SETTLING_DELAY_4_MS:
            return 4000;
         case VADC_SETTLING_DELAY_6_MS:
            return 6000;
         case VADC_SETTLING_DELAY_8_MS:
            return 8000;
         case VADC_SETTLING_DELAY_10_MS:
         case VADC_SETTLING_DELAY_16_MS:
         case VADC_SETTLING_DELAY_32_MS:
         case VADC_SETTLING_DELAY_64_MS:
         case VADC_SETTLING_DELAY_128_MS:
            return 10000;
         default:
            return 50;
      }
   }
   else
   {
      switch (eSettlingDelay)
      {
         case VADC_SETTLING_DELAY_100_US:
            return 100;
         case VADC_SETTLING_DELAY_200_US:
            return 200;
         case VADC_SETTLING_DELAY_300_US:
            return 300;
         case VADC_SETTLING_DELAY_400_US:
            return 400;
         case VADC_SETTLING_DELAY_500_US:
            return 500;
         case VADC_SETTLING_DELAY_600_US:
            return 600;
         case VADC_SETTLING_DELAY_700_US:
            return 700;
         case VADC_SETTLING_DELAY_800_US:
         case VADC_SETTLING_DELAY_900_US:
         case VADC_SETTLING_DELAY_1_MS:
            return 1000;
         case VADC_SETTLING_DELAY_2_MS:
            return 2000;
         case VADC_SETTLING_DELAY_4_MS:
            return 4000;
         case VADC_SETTLING_DELAY_6_MS:
         case VADC_SETTLING_DELAY_8_MS:
            return 8000;
         case VADC_SETTLING_DELAY_10_MS:
         case VADC_SETTLING_DELAY_16_MS:
            return 16000;
         case VADC_SETTLING_DELAY_32_MS:
            return 32000;
         case VADC_SETTLING_DELAY_64_MS:
            return 64000;
         case VADC_SETTLING_DELAY_128_MS:
            return 128000;
         default:
            return 50;
      }
   }
}

static uint32 VAdcHalGetSampleTimeUs(VAdcDecimationRatioType eDecimationRatio)
{
   /*
    * These sample times are for PMIC5 with CONV_MODE == FULL_SINC2 and
    * DIG_BOOST_EN == 1
    */
   switch (eDecimationRatio)
   {
      case VADC_DECIMATION_RATIO_256:
         return 142;
      case VADC_DECIMATION_RATIO_512:
         return 283;
      case VADC_DECIMATION_RATIO_1024:
         return 567;
      default:
         return 567;
   }
}

static uint32 VAdcHalGetNumAverages(VAdcAverageModeType eAveragingMode)
{
   switch (eAveragingMode)
   {
      case VADC_AVERAGE_2_SAMPLES:
         return 2;
      case VADC_AVERAGE_4_SAMPLES:
         return 4;
      case VADC_AVERAGE_8_SAMPLES:
         return 8;
      case VADC_AVERAGE_16_SAMPLES:
         return 16;
      default:
         return 1;
   }
}

static uint32 VAdcHalGetConversionTime(VAdcHalInterfaceType *piVAdcHal, VAdcConversionParametersType *pParams)
{
   uint32 uSettlingDelay_us;
   uint32 uSampleTime_us;
   uint32 uNumAverages;

   uSettlingDelay_us = VAdcHalGetSettlingDelayUs(piVAdcHal, pParams->eSettlingDelay);
   uSampleTime_us = VAdcHalGetSampleTimeUs(pParams->eDecimationRatio);
   uNumAverages = VAdcHalGetNumAverages(pParams->eAverageMode);

   /*
    * For fast averaging, only the first sample takes the full sample time.
    * Remaining samples take half the time.
    */
   return uSettlingDelay_us + uSampleTime_us + ((uSampleTime_us * (uNumAverages - 1)) / 2);
}

/*----------------------------------------------------------------------------
 * Externalized Function Definitions
 * -------------------------------------------------------------------------*/
VAdcHalResultType VAdcHalGetRevisionInfo(VAdcHalInterfaceType *piVAdcHal, VAdcRevisionInfoType *pRevisionInfo)
{
   VAdcHalResultType result;
   uint8 aucData[6];

   result = piVAdcHal->pfnReadBytes(piVAdcHal->pCtxt,
                                    PMIO_ADDRX(0, REVISION1),
                                    aucData,
                                    ARRAY_LENGTH(aucData));
   if (result != VADC_HAL_SUCCESS)
   {
      return result;
   }

   pRevisionInfo->usDigitalRev = VADC_REVISION(aucData[1], aucData[0]);
   pRevisionInfo->usAnalogRev = VADC_REVISION(aucData[3], aucData[2]);
   pRevisionInfo->ucType = aucData[4];
   pRevisionInfo->ucSubType = aucData[5];

   return result;
}

VAdcHalResultType VAdcHalGetStatus(VAdcHalInterfaceType *piVAdcHal, VAdcStatusType *pStatus)
{
   VAdcHalResultType result;
   uint8 aucData[1];

   result = piVAdcHal->pfnReadBytes(piVAdcHal->pCtxt,
                                    PMIO_ADDRX(0, STATUS1),
                                    aucData,
                                    ARRAY_LENGTH(aucData));
   if (result != VADC_HAL_SUCCESS)
   {
      return result;
   }

   pStatus->eConversionStatus = (VAdcConversionStatusType)((PMIO_GET_FVAL(STATUS1, REQ_STS, aucData[0]) << 1) |
                                PMIO_GET_FVAL(STATUS1, EOC, aucData[0]));

   return VADC_HAL_SUCCESS;
}

VAdcHalResultType VAdcHalConfigInterrupts(VAdcHalInterfaceType *piVAdcHal, uint8 ucIntrMask, VAdcInterruptConfigType eConfig)
{
   VAdcHalResultType result;
   uint8 aucData[3];

   result = piVAdcHal->pfnReadBytes(piVAdcHal->pCtxt,
                                    PMIO_ADDRX(0, INT_SET_TYPE),
                                    aucData,
                                    ARRAY_LENGTH(aucData));
   if (result != VADC_HAL_SUCCESS)
   {
      return result;
   }

   switch (eConfig)
   {
      case VADC_INTERRUPT_CONFIG_LEVEL_HIGH:
         aucData[0] &= ~ucIntrMask;
         aucData[1] |= ucIntrMask;
         aucData[2] &= ~ucIntrMask;
         break;
      case VADC_INTERRUPT_CONFIG_LEVEL_LOW:
         aucData[0] &= ~ucIntrMask;
         aucData[1] &= ~ucIntrMask;
         aucData[2] |= ucIntrMask;
         break;
      case VADC_INTERRUPT_CONFIG_RISING_EDGE:
         aucData[0] |= ucIntrMask;
         aucData[1] |= ucIntrMask;
         aucData[2] &= ~ucIntrMask;
         break;
      case VADC_INTERRUPT_CONFIG_FALLING_EDGE:
         aucData[0] |= ucIntrMask;
         aucData[1] &= ~ucIntrMask;
         aucData[2] |= ucIntrMask;
         break;
      default:
         return VADC_HAL_ERROR;
   }

   return piVAdcHal->pfnWriteBytes(piVAdcHal->pCtxt,
                                   PMIO_ADDRX(0, INT_SET_TYPE),
                                   aucData,
                                   ARRAY_LENGTH(aucData));
}

VAdcHalResultType VAdcHalGetPendingInterrupts(VAdcHalInterfaceType *piVAdcHal, uint8 *pucIntrMask)
{
   VAdcHalResultType result;
   uint8 aucData[1];

   result = piVAdcHal->pfnReadBytes(piVAdcHal->pCtxt,
                                    PMIO_ADDRX(0, INT_LATCHED_STS),
                                    aucData,
                                    ARRAY_LENGTH(aucData));
   if (result != VADC_HAL_SUCCESS)
   {
      return result;
   }

   *pucIntrMask = aucData[0];

   return VADC_HAL_SUCCESS;
}

VAdcHalResultType VAdcHalClearInterrupts(VAdcHalInterfaceType *piVAdcHal, uint8 ucIntrMask)
{
   uint8 aucData[1];

   aucData[0] = ucIntrMask;

   return piVAdcHal->pfnWriteBytes(piVAdcHal->pCtxt,
                                   PMIO_ADDRX(0, INT_LATCHED_CLR),
                                   aucData,
                                   ARRAY_LENGTH(aucData));
}

VAdcHalResultType VAdcHalEnableInterrupts(VAdcHalInterfaceType *piVAdcHal, uint8 ucIntrMask)
{
   uint8 aucData[1];

   aucData[0] = ucIntrMask;

   return piVAdcHal->pfnWriteBytes(piVAdcHal->pCtxt,
                                   PMIO_ADDRX(0, INT_EN_SET),
                                   aucData,
                                   ARRAY_LENGTH(aucData));
}

VAdcHalResultType VAdcHalSetInterruptMid(VAdcHalInterfaceType *piVAdcHal, uint32 uIntrMid)
{
   uint8 aucData[1];

   aucData[0] = PMIO_FVAL(INT_MID_SEL, INT_MID_SEL, (uint8)uIntrMid);

   return piVAdcHal->pfnWriteBytes(piVAdcHal->pCtxt,
                                   PMIO_ADDRX(0, INT_MID_SEL),
                                   aucData,
                                   ARRAY_LENGTH(aucData));
}

VAdcHalResultType VAdcHalSetEnable(VAdcHalInterfaceType *piVAdcHal, VAdcEnableType eEnable)
{
   uint8 aucData[1];

   if (eEnable == VADC_DISABLE)
   {
      aucData[0] = PMIO_FVAL(EN_CTL1, ADC_EN, 0);
   }
   else
   {
      aucData[0] = PMIO_FVAL(EN_CTL1, ADC_EN, 1);
   }

   return piVAdcHal->pfnWriteBytes(piVAdcHal->pCtxt,
                                   PMIO_ADDRX(0, EN_CTL1),
                                   aucData,
                                   ARRAY_LENGTH(aucData));
}

VAdcHalResultType VAdcHalRequestConversion(VAdcHalInterfaceType *piVAdcHal, VAdcConversionParametersType *pParams, uint32 *puConvTimeUs)
{
   VAdcHalResultType result;
   uint8 aucData[6];
   uint8 uHwSettle;

   aucData[0] = PMIO_FVAL(ADC_DIG_PARAM, CAL_VAL, PMIO_VAL(ADC_DIG_PARAM, CAL_VAL, TIMER_CAL)) |
                PMIO_FVAL(ADC_DIG_PARAM, CAL_SEL, (uint8)pParams->eCalMethod) |
                PMIO_FVAL(ADC_DIG_PARAM, DEC_RATIO_SEL, (uint8)pParams->eDecimationRatio);

   aucData[1] = PMIO_FVAL(FAST_AVG_CTL, FAST_AVG_EN, PMIO_VAL(FAST_AVG_CTL, FAST_AVG_EN, FAST_AVG_ENABLED)) |
                PMIO_FVAL(FAST_AVG_CTL, FAST_AVG_SAMPLES, (uint8)pParams->eAverageMode);

   aucData[2] = PMIO_FVAL(ADC_CH_SEL_CTL, ADC_CH_SEL, (uint8)pParams->uChannel);

   if (piVAdcHal->revisionInfo.usDigitalRev < VADC_REVISION(5,3))
   {
      switch (pParams->eSettlingDelay)
      {
         case VADC_SETTLING_DELAY_0_US:
            uHwSettle = 0;
            break;
         case VADC_SETTLING_DELAY_100_US:
            uHwSettle = 1;
            break;
         case VADC_SETTLING_DELAY_200_US:
            uHwSettle = 2;
            break;
         case VADC_SETTLING_DELAY_300_US:
            uHwSettle = 3;
            break;
         case VADC_SETTLING_DELAY_400_US:
            uHwSettle = 4;
            break;
         case VADC_SETTLING_DELAY_500_US:
            uHwSettle = 5;
            break;
         case VADC_SETTLING_DELAY_600_US:
            uHwSettle = 6;
            break;
         case VADC_SETTLING_DELAY_700_US:
            uHwSettle = 7;
            break;
         case VADC_SETTLING_DELAY_800_US:
            uHwSettle = 8;
            break;
         case VADC_SETTLING_DELAY_900_US:
            uHwSettle = 9;
            break;
         case VADC_SETTLING_DELAY_1_MS:
            uHwSettle = 10;
            break;
         case VADC_SETTLING_DELAY_2_MS:
            uHwSettle = 11;
            break;
         case VADC_SETTLING_DELAY_4_MS:
            uHwSettle = 12;
            break;
         case VADC_SETTLING_DELAY_6_MS:
            uHwSettle = 13;
            break;
         case VADC_SETTLING_DELAY_8_MS:
            uHwSettle = 14;
            break;
         case VADC_SETTLING_DELAY_10_MS:
         case VADC_SETTLING_DELAY_16_MS:
         case VADC_SETTLING_DELAY_32_MS:
         case VADC_SETTLING_DELAY_64_MS:
         case VADC_SETTLING_DELAY_128_MS:
            uHwSettle = 15;
            break;
         default:
            uHwSettle = 0;
      }
   }
   else
   {
      switch (pParams->eSettlingDelay)
      {
         case VADC_SETTLING_DELAY_0_US:
            uHwSettle = 0;
            break;
         case VADC_SETTLING_DELAY_100_US:
            uHwSettle = 1;
            break;
         case VADC_SETTLING_DELAY_200_US:
            uHwSettle = 2;
            break;
         case VADC_SETTLING_DELAY_300_US:
            uHwSettle = 3;
            break;
         case VADC_SETTLING_DELAY_400_US:
            uHwSettle = 4;
            break;
         case VADC_SETTLING_DELAY_500_US:
            uHwSettle = 5;
            break;
         case VADC_SETTLING_DELAY_600_US:
            uHwSettle = 6;
            break;
         case VADC_SETTLING_DELAY_700_US:
            uHwSettle = 7;
            break;
         case VADC_SETTLING_DELAY_800_US:
         case VADC_SETTLING_DELAY_900_US:
         case VADC_SETTLING_DELAY_1_MS:
            uHwSettle = 8;
            break;
         case VADC_SETTLING_DELAY_2_MS:
            uHwSettle = 9;
            break;
         case VADC_SETTLING_DELAY_4_MS:
            uHwSettle = 10;
            break;
         case VADC_SETTLING_DELAY_6_MS:
         case VADC_SETTLING_DELAY_8_MS:
            uHwSettle = 11;
            break;
         case VADC_SETTLING_DELAY_10_MS:
         case VADC_SETTLING_DELAY_16_MS:
            uHwSettle = 12;
            break;
         case VADC_SETTLING_DELAY_32_MS:
            uHwSettle = 13;
            break;
         case VADC_SETTLING_DELAY_64_MS:
            uHwSettle = 14;
            break;
         case VADC_SETTLING_DELAY_128_MS:
            uHwSettle = 15;
            break;
         default:
            uHwSettle = 0;
      }
   }
   aucData[3] = PMIO_FVAL(DELAY_CTL, HW_SETTLE_DELAY, uHwSettle);

   aucData[4] = PMIO_FVAL(EN_CTL1, ADC_EN, 1);

   aucData[5] = PMIO_FVAL(CONV_REQ, REQ, 1);

   result = piVAdcHal->pfnWriteBytes(piVAdcHal->pCtxt,
                                     PMIO_ADDRX(0, ADC_DIG_PARAM),
                                     aucData,
                                     ARRAY_LENGTH(aucData));
   if (result != VADC_HAL_SUCCESS)
   {
      return result;
   }

   *puConvTimeUs = VAdcHalGetConversionTime(piVAdcHal, pParams);

   return VADC_HAL_SUCCESS;
}

VAdcHalResultType VAdcHalGetConversionCode(VAdcHalInterfaceType *piVAdcHal, uint32 *puCode)
{
   VAdcHalResultType result;
   uint8 aucData[2];

   result = piVAdcHal->pfnReadBytes(piVAdcHal->pCtxt,
                                    PMIO_ADDRX(0, DATA0),
                                    aucData,
                                    ARRAY_LENGTH(aucData));
   if (result != VADC_HAL_SUCCESS)
   {
      return result;
   }

   *puCode = (aucData[1] << 8) | aucData[0];

   return VADC_HAL_SUCCESS;
}

VAdcHalResultType VAdcHalDumpRegisters(VAdcHalInterfaceType *piVAdcHal, VAdcHalRegDumpType *pVAdcRegDump)
{
   VAdcHalResultType result;
   uint8 aucData[1];
   uint32 uOffset;
   uint32 uReg;

   for (uReg = 0; uReg < VADC_HAL_NUM_REGISTERS_TO_DUMP; uReg++)
   {
      uOffset = gaucRegDumpAddresses[uReg];

      result = piVAdcHal->pfnReadBytes(piVAdcHal->pCtxt,
                                       uOffset,
                                       aucData,
                                       ARRAY_LENGTH(aucData));
      if (result != VADC_HAL_SUCCESS)
      {
         return result;
      }

      pVAdcRegDump->aVAdcReg[uReg].ucOffset = (uint8)uOffset;
      pVAdcRegDump->aVAdcReg[uReg].ucVal = aucData[0];
   }

   return VADC_HAL_SUCCESS;
}

