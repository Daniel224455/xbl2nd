/*============================================================================
  FILE:         AdcInternalSettings.c

  OVERVIEW:     Internal settings for ADC.

  DEPENDENCIES: None

                Copyright (c) 2017 Qualcomm Technologies, Inc.
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
  2017-11-16  jjo  Initial version.

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
 * 1000_PMC1000_PMC1000L
 */
static const AdcPhysicalDeviceType adcPhysicalDevices_1000_PMC1000_PMC1000L[] =
{
   {
      .pszDevName = "/vadc/pmc1000_1",
   },

   {
      .pszDevName = "/vadc/pmc1000l",
   },

   {
      .pszDevName = "/vadc/pmc1000_2",
   },
};

const AdcBspType AdcBsp_1000_PMC1000_PMC1000L[] =
{
   {
      .paAdcPhysicalDevices = adcPhysicalDevices_1000_PMC1000_PMC1000L,
      .uNumDevices          = ARRAY_LENGTH(adcPhysicalDevices_1000_PMC1000_PMC1000L)
   }
};

