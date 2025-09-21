/*============================================================================
  FILE:         AdcInternalSettings.c

  OVERVIEW:     Internal settings for ADC.

  DEPENDENCIES: None

                Copyright (c) 2014-2016 Qualcomm Technologies, Inc.
                All Rights Reserved.
                Qualcomm Technologies Confidential and Proprietary.
============================================================================*/
/*============================================================================
  EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.  Please
  use ISO format for dates.

  when        who  what, where, why
  ----------  ---  -----------------------------------------------------------
  2016-05-05  jjo  Add FG ADC.
  2015-05-05  jjo  Use designated initializers.
  2014-04-02  jjo  Initial version.

============================================================================*/
/*----------------------------------------------------------------------------
 * Include Files
 * -------------------------------------------------------------------------*/
#include "AdcBsp.h"

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

/*
 * 8998_PM8998
 */
static const AdcPhysicalDeviceType adcPhysicalDevices_8998_PM8998[] =
{
   {
      .pszDevName = "/core/hwengines/adc/pmic_0/vadc",
   },
};

const AdcBspType AdcBsp_8998_PM8998[] =
{
   {
      .paAdcPhysicalDevices = adcPhysicalDevices_8998_PM8998,
      .uNumDevices          = ARRAY_LENGTH(adcPhysicalDevices_8998_PM8998)
   }
};

/*
 * 8998_PM8998_PMI8998
 */
static const AdcPhysicalDeviceType adcPhysicalDevices_8998_PM8998_PMI8998[] =
{
   /* VADC PM8998 */
   {
      .pszDevName = "/core/hwengines/adc/pmic_0/vadc",
   },
   /* FGADC PMI8998 */
   {
      .pszDevName = "/core/hwengines/adc/pmic_1/fgadc",
   },
};

const AdcBspType AdcBsp_8998_PM8998_PMI8998[] =
{
   {
      .paAdcPhysicalDevices = adcPhysicalDevices_8998_PM8998_PMI8998,
      .uNumDevices          = ARRAY_LENGTH(adcPhysicalDevices_8998_PM8998_PMI8998)
   }
};

