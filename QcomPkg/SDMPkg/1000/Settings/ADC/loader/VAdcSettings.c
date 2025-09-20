/*============================================================================
  FILE:         VAdcSettings.c

  OVERVIEW:     Settings for VADC.

  DEPENDENCIES: None

                Copyright (c) 2017-2018 Qualcomm Technologies, Inc.
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
  2017-11-10  jjo  Initial revision.

============================================================================*/
/*----------------------------------------------------------------------------
 * Include Files
 * -------------------------------------------------------------------------*/
#include "VAdcBsp.h"
#include "AdcInputs.h"

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

/*----------------------------------------------------------------------------
 * PMC1000_1 Settings
 * -------------------------------------------------------------------------*/
/*
 * VAdc channel configuration.
 */
const VAdcChannelConfigType gVAdcChannelsPMC1000_1[] =
{
   /* VPH_PWR (VPH_PWR pin) */
   {
      .pszName                   = ADC_INPUT_VPH_PWR,
      .uAdcHardwareChannel       = 0x83,
      .eSettlingDelay            = VADC_SETTLING_DELAY_0_US,
      .eAverageMode              = VADC_AVERAGE_1_SAMPLE,
      .eDecimationRatio          = VADC_DECIMATION_RATIO_1024,
      .eCalMethod                = VADC_CAL_METHOD_ABSOLUTE,
      .scalingFactor             = {1, 3},  /* {num, den} */
      .eScalingMethod            = VADC_SCALE_TO_MILLIVOLTS,
      .uPullUp                   = 0,
      .pIntTable                 = NULL,
   },

   /* VCOIN (VCOIN pin) */
   {
      .pszName                   = ADC_INPUT_VCOIN,
      .uAdcHardwareChannel       = 0x85,
      .eSettlingDelay            = VADC_SETTLING_DELAY_0_US,
      .eAverageMode              = VADC_AVERAGE_1_SAMPLE,
      .eDecimationRatio          = VADC_DECIMATION_RATIO_1024,
      .eCalMethod                = VADC_CAL_METHOD_ABSOLUTE,
      .scalingFactor             = {1, 3},  /* {num, den} */
      .eScalingMethod            = VADC_SCALE_TO_MILLIVOLTS,
      .uPullUp                   = 0,
      .pIntTable                 = NULL,
   },

   /* PMIC_TEMP1 (internal sensor) */
   {
      .pszName                   = ADC_INPUT_PMIC_TEMP1,
      .uAdcHardwareChannel       = 0x6,
      .eSettlingDelay            = VADC_SETTLING_DELAY_0_US,
      .eAverageMode              = VADC_AVERAGE_1_SAMPLE,
      .eDecimationRatio          = VADC_DECIMATION_RATIO_1024,
      .eCalMethod                = VADC_CAL_METHOD_ABSOLUTE,
      .scalingFactor             = {1, 1},  /* {num, den} */
      .eScalingMethod            = VADC_SCALE_DIE_TEMP_TO_MILLIDEGREES,
      .uPullUp                   = 0,
      .pIntTable                 = NULL,
   },
};

const uint32 guNumVAdcChannelsPMC1000_1 = ARRAY_LENGTH(gVAdcChannelsPMC1000_1);

/*----------------------------------------------------------------------------
 * PMC1000L Settings
 * -------------------------------------------------------------------------*/
/*
 * VAdc channel configuration.
 */
const VAdcChannelConfigType gVAdcChannelsPMC1000L[] =
{
   /* VPH_PWR (VPH_PWR pin) */
   {
      .pszName                   = ADC_INPUT_VPH_PWR,
      .uAdcHardwareChannel       = 0x83,
      .eSettlingDelay            = VADC_SETTLING_DELAY_0_US,
      .eAverageMode              = VADC_AVERAGE_1_SAMPLE,
      .eDecimationRatio          = VADC_DECIMATION_RATIO_1024,
      .eCalMethod                = VADC_CAL_METHOD_ABSOLUTE,
      .scalingFactor             = {1, 3},  /* {num, den} */
      .eScalingMethod            = VADC_SCALE_TO_MILLIVOLTS,
      .uPullUp                   = 0,
      .pIntTable                 = NULL,
   },

   /* PMIC_TEMP2 (internal sensor) */
   {
      .pszName                   = ADC_INPUT_PMIC_TEMP2,
      .uAdcHardwareChannel       = 0x6,
      .eSettlingDelay            = VADC_SETTLING_DELAY_0_US,
      .eAverageMode              = VADC_AVERAGE_1_SAMPLE,
      .eDecimationRatio          = VADC_DECIMATION_RATIO_1024,
      .eCalMethod                = VADC_CAL_METHOD_ABSOLUTE,
      .scalingFactor             = {1, 1},  /* {num, den} */
      .eScalingMethod            = VADC_SCALE_DIE_TEMP_TO_MILLIDEGREES,
      .uPullUp                   = 0,
      .pIntTable                 = NULL,
   },
};

const uint32 guNumVAdcChannelsPMC1000L = ARRAY_LENGTH(gVAdcChannelsPMC1000L);

/*----------------------------------------------------------------------------
 * PMC1000_2 Settings
 * -------------------------------------------------------------------------*/
/*
 * VAdc channel configuration.
 */
const VAdcChannelConfigType gVAdcChannelsPMC1000_2[] =
{
   /* VPH_PWR (VPH_PWR pin) */
   {
      .pszName                   = ADC_INPUT_VPH_PWR,
      .uAdcHardwareChannel       = 0x83,
      .eSettlingDelay            = VADC_SETTLING_DELAY_0_US,
      .eAverageMode              = VADC_AVERAGE_1_SAMPLE,
      .eDecimationRatio          = VADC_DECIMATION_RATIO_1024,
      .eCalMethod                = VADC_CAL_METHOD_ABSOLUTE,
      .scalingFactor             = {1, 3},  /* {num, den} */
      .eScalingMethod            = VADC_SCALE_TO_MILLIVOLTS,
      .uPullUp                   = 0,
      .pIntTable                 = NULL,
   },

   /* PMIC_TEMP3 (internal sensor) */
   {
      .pszName                   = ADC_INPUT_PMIC_TEMP3,
      .uAdcHardwareChannel       = 0x6,
      .eSettlingDelay            = VADC_SETTLING_DELAY_0_US,
      .eAverageMode              = VADC_AVERAGE_1_SAMPLE,
      .eDecimationRatio          = VADC_DECIMATION_RATIO_1024,
      .eCalMethod                = VADC_CAL_METHOD_ABSOLUTE,
      .scalingFactor             = {1, 1},  /* {num, den} */
      .eScalingMethod            = VADC_SCALE_DIE_TEMP_TO_MILLIDEGREES,
      .uPullUp                   = 0,
      .pIntTable                 = NULL,
   },
};

const uint32 guNumVAdcChannelsPMC1000_2 = ARRAY_LENGTH(gVAdcChannelsPMC1000_2);

