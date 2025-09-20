/*============================================================================
  FILE:         AdcDxe.c

  OVERVIEW:     Implementation of the ADC EFI protocol interface

  DEPENDENCIES: None

                Copyright (c) 2011-2018 Qualcomm Technologies, Inc.
                All Rights Reserved.
                Qualcomm Technologies Confidential and Proprietary,
============================================================================*/
/*============================================================================
  EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.  Please
  use ISO format for dates.

  when        who  what, where, why
  ----------  ---  -----------------------------------------------------------
  2018-03-07  KS   Added an entry for cosmo pmic (pm600) in gaDALModDriverList.
  2016-05-03  jjo  Add fuel gauge threshold support.
  2015-07-13  al   Changing TPL_NOTIFY to TPL_CALLBACK.
  2015-01-23  jjo  Use DDIAdc.h.
  2014-09-04  jjo  Support updated AdcLib.
  2013-09-11  jjo  Raise / lower TPL during ADC calls for synchronization.
  2013-01-15  jjo  Added synchronous V-I API.
  2012-07-01  jjo  Moved the DAL code to ADC lib.
  2012-06-15  jjo  Removed the DAL ADC layer.
  2012-03-23  jjo  Klocwork fixes.
  2011-05-12  jdt  Initial revision.

============================================================================*/
/*----------------------------------------------------------------------------
 * Include Files
 * -------------------------------------------------------------------------*/
#include <Uefi.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Protocol/EFIAdc.h>
#include "DDIAdc.h"

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
static EFI_STATUS
EFI_Adc_GetChannel(
   IN CONST CHAR8 *pszInputName, IN UINT32 uInputNameSize,
   OUT EfiAdcDeviceChannelType *pChannel
   );

static EFI_STATUS
EFI_Adc_Read(
   IN EfiAdcDeviceChannelType *pEfiChannel,
   OUT EfiAdcResultType *pEfiAdcResult
   );

static EFI_STATUS
EFI_Adc_RecalibrateChannel(
   IN EfiAdcDeviceChannelType *pEfiChannel,
   OUT EfiAdcRecalibrateChannelResultType *pEfiAdcRecalChanResult
   );

static EFI_STATUS
EFI_Adc_SynchronousRead(
   IN EfiAdcDeviceChannelType *pVAdcChannel,
   IN EfiAdcDeviceChannelType *pIAdcChannel,
   OUT EfiAdcResultType *pVAdcResult,
   OUT EfiAdcResultType *pIAdcResult
   );

static EFI_STATUS
EFI_Adc_SetFuelGagueThreshold(
   IN EfiAdcFuelGaugeThresholdType eWhichThresh,
   IN INT32 nThreshold
   );

static EFI_STATUS
EFI_Adc_SetFuelGagueBattIdForced(
   IN BOOLEAN bEnable
   );

static EFI_STATUS
EFI_Adc_ProcessConversionResult(
   IN EfiAdcDeviceChannelType *pEfiChannel,
   IN UINT32 uAdcCode,
   OUT EfiAdcResultType *pEfiAdcResult
   );

static EFI_STATUS
EFI_Adc_ProcessConversionResultInverse(
   IN EfiAdcDeviceChannelType *pEfiChannel,
   IN INT32 nPhysical,
   OUT EfiAdcResultType *pEfiAdcResult
   );

/*----------------------------------------------------------------------------
 * Global Data Definitions
 * -------------------------------------------------------------------------*/
extern DALREG_DriverInfo DALVAdc_DriverInfo;

/*----------------------------------------------------------------------------
 * Static Variable Definitions
 * -------------------------------------------------------------------------*/
static DalDeviceHandle *phAdcDev;

static StringDevice gaDALModDriverList[] =
{
   {"/core/hwengines/adc/pmic_0/vadc", 0, 0, &DALVAdc_DriverInfo, 0, NULL},
   {"/vadc/pm855", 0, 0, &DALVAdc_DriverInfo, 0, NULL},
   {"/vadc/pm600", 0, 0, &DALVAdc_DriverInfo, 0, NULL},
   {"/vadc/pm640", 0, 0, &DALVAdc_DriverInfo, 0, NULL},
   {"/vadc/pm855b", 0, 0, &DALVAdc_DriverInfo, 0, NULL},
   {"/vadc/pm855a", 0, 0, &DALVAdc_DriverInfo, 0, NULL},
   {"/vadc/pm855l", 0, 0, &DALVAdc_DriverInfo, 0, NULL},
   {"/vadc/pmc1000_1", 0, 0, &DALVAdc_DriverInfo, 0, NULL},
   {"/vadc/pmc1000l", 0, 0, &DALVAdc_DriverInfo, 0, NULL},
   {"/vadc/pmc1000_2", 0, 0, &DALVAdc_DriverInfo, 0, NULL},
   {"/vadc/pm855au_1", 0, 0, &DALVAdc_DriverInfo, 0, NULL},
   {"/vadc/pm855au_2", 0, 0, &DALVAdc_DriverInfo, 0, NULL},
};

static DALSYSConfig gAdcDALSYSModConfig =
{
   // string device info
   {ARRAY_LENGTH(gaDALModDriverList), gaDALModDriverList},
   // numeric driver info
   {0, NULL},
};

static EFI_ADC_PROTOCOL AdcProtocol =
{
   EFI_ADC_PROTOCOL_REVISION,
   EFI_Adc_GetChannel,
   EFI_Adc_Read,
   EFI_Adc_RecalibrateChannel,
   EFI_Adc_SynchronousRead,
   EFI_Adc_SetFuelGagueThreshold,
   EFI_Adc_SetFuelGagueBattIdForced,
   EFI_Adc_ProcessConversionResult,
   EFI_Adc_ProcessConversionResultInverse,
};

/*----------------------------------------------------------------------------
 * Static Function Declarations and Definitions
 * -------------------------------------------------------------------------*/
static EfiAdcResultStatusType EFI_Adc_ConvertStatus(AdcResultStatusType eStatus)
{
   switch (eStatus)
   {
      case ADC_RESULT_VALID:
         return EFI_ADC_RESULT_VALID;
      case ADC_RESULT_TIMEOUT:
         return EFI_ADC_RESULT_TIMEOUT;
      case ADC_RESULT_STALE:
         return EFI_ADC_RESULT_STALE;
      default:
         return EFI_ADC_RESULT_INVALID;
   }
}

/**
  Gets an ADC channel using a logical ADC input string ID

  This function will look up the device-channel index using the string
  name passed in by the client

  @param  pszInputName    [in]         NULL-terminated ADC input ID string.
  @param  uInputNameSize  [in]         The size of the string (including the null-terminator).
  @param  pEfiChannel     [out]        The device-channel for the ADC input.

  @return EFI_SUCCESS              The function completed successfully.
  @return EFI_INVALID_PARAMETER    An invalid parameter was passed.
  @return EFI_UNSUPPORTED          Channel is not present.
**/
static EFI_STATUS
EFI_Adc_GetChannel(IN CONST CHAR8 *pszInputName, IN UINT32 uInputNameSize, OUT EfiAdcDeviceChannelType *pEfiChannel)
{
   AdcInputPropertiesType channel;
   DALResult ret;

   if (pszInputName == NULL || uInputNameSize == 0 || pEfiChannel == NULL)
   {
      return EFI_INVALID_PARAMETER;
   }

   ret = DalAdc_GetAdcInputProperties(phAdcDev, pszInputName, uInputNameSize, &channel);
   if (ret != DAL_SUCCESS)
   {
      return EFI_UNSUPPORTED;
   }

   pEfiChannel->nDeviceIdx = channel.nDeviceIdx;
   pEfiChannel->nChannelIdx = channel.nChannelIdx;

   return EFI_SUCCESS;
}

/**
  Reads an ADC channel

  This function will read an ADC channel

  @param  pEfiChannel    [in]      The ADC channel to read instance.
  @param  pEfiAdcResult  [out]     The result of the ADC reading.

  @return EFI_SUCCESS              The function completed successfully.
  @return EFI_INVALID_PARAMETER    An invalid parameter was passed.
  @return EFI_UNSUPPORTED          The function is not supported in this context.
**/
static EFI_STATUS
EFI_Adc_Read(IN EfiAdcDeviceChannelType *pEfiChannel, OUT EfiAdcResultType *pEfiAdcResult)
{
   EFI_TPL prevTPL;
   AdcInputPropertiesType channel;
   AdcResultType result;
   DALResult ret;

   if (pEfiChannel == NULL || pEfiAdcResult == NULL)
   {
      return EFI_INVALID_PARAMETER;
   }

   channel.nDeviceIdx = pEfiChannel->nDeviceIdx;
   channel.nChannelIdx = pEfiChannel->nChannelIdx;

   prevTPL = gBS->RaiseTPL(TPL_CALLBACK);

   ret = DalAdc_Read(phAdcDev, &channel, &result);

   gBS->RestoreTPL(prevTPL);

   if (ret != DAL_SUCCESS)
   {
      pEfiAdcResult->eStatus = EFI_ADC_RESULT_INVALID;

      return EFI_UNSUPPORTED;
   }
   else
   {
      pEfiAdcResult->eStatus = EFI_Adc_ConvertStatus(result.eStatus);
      pEfiAdcResult->nPhysical = result.nPhysical;
      pEfiAdcResult->nPercent = result.nPercent;
      pEfiAdcResult->nMicrovolts = result.nMicrovolts;
      pEfiAdcResult->nCode = result.nCode;

      return EFI_SUCCESS;
   }
}

/**
  Recalibrates an ADC channel

  This function will recalibrate an ADC channel

  @param  pEfiChannel             [in]  The ADC channel to recalibrate instance.
  @param  pEfiAdcRecalChanResult  [out]  The result of the ADC recalibrate

  @return EFI_SUCCESS              The function completed successfully.
  @return EFI_INVALID_PARAMETER    An invalid parameter was passed.
  @return EFI_UNSUPPORTED          The function is not supported in this context.
**/
static EFI_STATUS
EFI_Adc_RecalibrateChannel(IN EfiAdcDeviceChannelType *pEfiChannel, OUT EfiAdcRecalibrateChannelResultType *pEfiAdcRecalChanResult)
{
   EFI_TPL prevTPL;
   AdcInputPropertiesType channel;
   AdcRecalibrationResultType recal;
   DALResult ret;

   if (pEfiChannel == NULL || pEfiAdcRecalChanResult == NULL)
   {
      return EFI_INVALID_PARAMETER;
   }

   channel.nDeviceIdx = pEfiChannel->nDeviceIdx;
   channel.nChannelIdx = pEfiChannel->nChannelIdx;

   prevTPL = gBS->RaiseTPL(TPL_CALLBACK);

   ret = DalAdc_Recalibrate(phAdcDev, &channel, &recal);

   gBS->RestoreTPL(prevTPL);

   if (ret != DAL_SUCCESS)
   {
      pEfiAdcRecalChanResult->eStatus = EFI_ADC_RESULT_INVALID;

      return EFI_UNSUPPORTED;
   }
   else
   {
      pEfiAdcRecalChanResult->eStatus = EFI_Adc_ConvertStatus(recal.eStatus);
      pEfiAdcRecalChanResult->nPhysical1_uV = recal.nPhysical1_uV;
      pEfiAdcRecalChanResult->nPhysical2_uV = recal.nPhysical2_uV;
      pEfiAdcRecalChanResult->uCode1 = recal.uCode1;
      pEfiAdcRecalChanResult->uCode2 = recal.uCode2;

      return EFI_SUCCESS;
   }
}

/**
  Performs a synchronous VADC & IADC measurement.

  @param  pVAdcChannel   [in]      The VADC channel to read.
  @param  pIAdcChannel   [in]      The IADC channel to read.
  @param  pVAdcResult    [out]     The VADC result.
  @param  pIAdcResult    [out]     The IADC result.

  @return EFI_SUCCESS              The function completed successfully.
  @return EFI_INVALID_PARAMETER    An invalid parameter was passed.
  @return EFI_UNSUPPORTED          The function is not supported in this context.
*/
static EFI_STATUS
EFI_Adc_SynchronousRead(
   IN EfiAdcDeviceChannelType *pVAdcChannel,
   IN EfiAdcDeviceChannelType *pIAdcChannel,
   OUT EfiAdcResultType *pVAdcResult,
   OUT EfiAdcResultType *pIAdcResult
   )
{
   return EFI_UNSUPPORTED;
}

/**
  Sets a fuel gauge HW threshold.

  @param  eWhichThresh   [in]      Which threshold.
  @param  nThreshold     [in]      Threshold in physical units.

  @return EFI_SUCCESS              The function completed successfully.
  @return EFI_INVALID_PARAMETER    An invalid parameter was passed.
  @return EFI_UNSUPPORTED          The function is not supported in this context.
*/
static EFI_STATUS
EFI_Adc_SetFuelGagueThreshold(
   IN EfiAdcFuelGaugeThresholdType eWhichThresh,
   IN INT32 nThreshold
   )
{
   return EFI_UNSUPPORTED;
}

static EFI_STATUS
EFI_Adc_SetFuelGagueBattIdForced(
   IN BOOLEAN bEnable
   )
{
   return EFI_UNSUPPORTED;
}

static EFI_STATUS
EFI_Adc_ProcessConversionResult(
   IN EfiAdcDeviceChannelType *pEfiChannel,
   IN UINT32 uAdcCode,
   OUT EfiAdcResultType *pEfiAdcResult
   )
{
   AdcInputPropertiesType channel;
   AdcResultType result;
   DALResult ret;

   if (pEfiChannel == NULL || pEfiAdcResult == NULL)
   {
      return EFI_INVALID_PARAMETER;
   }

   channel.nDeviceIdx = pEfiChannel->nDeviceIdx;
   channel.nChannelIdx = pEfiChannel->nChannelIdx;

   ret = DalAdc_ProcessConversionResult(phAdcDev, &channel, uAdcCode, &result);
   if (ret != DAL_SUCCESS)
   {
      pEfiAdcResult->eStatus = EFI_ADC_RESULT_INVALID;

      return EFI_UNSUPPORTED;
   }
   else
   {
      pEfiAdcResult->eStatus = EFI_Adc_ConvertStatus(result.eStatus);
      pEfiAdcResult->nPhysical = result.nPhysical;
      pEfiAdcResult->nPercent = result.nPercent;
      pEfiAdcResult->nMicrovolts = result.nMicrovolts;
      pEfiAdcResult->nCode = result.nCode;

      return EFI_SUCCESS;
   }
}

static EFI_STATUS
EFI_Adc_ProcessConversionResultInverse(
   IN EfiAdcDeviceChannelType *pEfiChannel,
   IN INT32 nPhysical,
   OUT EfiAdcResultType *pEfiAdcResult
   )
{
   AdcInputPropertiesType channel;
   AdcResultType result;
   DALResult ret;

   if (pEfiChannel == NULL || pEfiAdcResult == NULL)
   {
      return EFI_INVALID_PARAMETER;
   }

   channel.nDeviceIdx = pEfiChannel->nDeviceIdx;
   channel.nChannelIdx = pEfiChannel->nChannelIdx;

   ret = DalAdc_ProcessConversionResultInverse(phAdcDev, &channel, nPhysical, &result);
   if (ret != DAL_SUCCESS)
   {
      pEfiAdcResult->eStatus = EFI_ADC_RESULT_INVALID;

      return EFI_UNSUPPORTED;
   }
   else
   {
      pEfiAdcResult->eStatus = EFI_Adc_ConvertStatus(result.eStatus);
      pEfiAdcResult->nPhysical = result.nPhysical;
      pEfiAdcResult->nPercent = result.nPercent;
      pEfiAdcResult->nMicrovolts = result.nMicrovolts;
      pEfiAdcResult->nCode = result.nCode;

      return EFI_SUCCESS;
   }
}

/*----------------------------------------------------------------------------
 * Externalized Function Definitions
 * -------------------------------------------------------------------------*/
EFI_STATUS
EFIAPI
AdcEntryPoint(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE *SystemTable)
{
   EFI_HANDLE handle = NULL;
   EFI_STATUS status;
   DALResult ret;

   DALSYS_InitMod(&gAdcDALSYSModConfig);

   ret = DAL_AdcDeviceAttach(DALDEVICEID_ADC, &phAdcDev);
   if (ret != DAL_SUCCESS)
   {
      return EFI_UNSUPPORTED;
   }

   status = gBS->InstallMultipleProtocolInterfaces(&handle,
                                                   &gEfiAdcProtocolGuid,
                                                   (void **)&AdcProtocol,
                                                   NULL,
                                                   NULL,
                                                   NULL);

   return status;
}

