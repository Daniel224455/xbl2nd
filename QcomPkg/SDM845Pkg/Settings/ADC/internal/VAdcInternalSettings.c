/*============================================================================
  FILE:         VAdcInternalSettings.c

  OVERVIEW:     Internal settings for VADC.

  DEPENDENCIES: None

                Copyright (c) 2016-2017 Qualcomm Technologies, Inc.
                All Rights Reserved.
                Qualcomm Technologies Proprietary and Confidential.
============================================================================*/
/*============================================================================
  EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.  Please
  use ISO format for dates.

  $Header: //components/rel/boot.xf/2.2/QcomPkg/SDM845Pkg/Settings/ADC/internal/VAdcInternalSettings.c#1 $

  when        who  what, where, why
  ----------  ---  -----------------------------------------------------------
  2016-08-12  jjo  Split BSP.
  2016-03-04  jjo  Initial revision.

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
extern const VAdcChannelConfigType gVAdcChannels[];
extern const uint32 uNumVAdcChannels;

/*----------------------------------------------------------------------------
 * Static Variable Definitions
 * -------------------------------------------------------------------------*/
/*
 * This table is a mapping of ADC channels to GPIO inputs.
 */
static const uint8 aucGpio8Channels[]  = {0x12, 0x32, 0x52, 0x72};
static const uint8 aucGpio9Channels[]  = {0x13, 0x33, 0x53, 0x73};
static const uint8 aucGpio10Channels[] = {0x14, 0x34, 0x54, 0x74};
static const uint8 aucGpio11Channels[] = {0x15, 0x35, 0x55, 0x75};
static const uint8 aucGpio12Channels[] = {0x16, 0x36, 0x56, 0x76};
static const uint8 aucGpio21Channels[] = {0x17, 0x37, 0x57, 0x77, 0x97};
static const uint8 aucGpio22Channels[] = {0x18, 0x38, 0x58, 0x78, 0x98};
static const uint8 aucGpio23Channels[] = {0x19, 0x39, 0x59, 0x79, 0x99};

static const VAdcGpioChannelMappingType vAdcGpioChannelMappings[] =
{
   {
      .paucChannels = aucGpio8Channels,
      .uNumChannels = ARRAY_LENGTH(aucGpio8Channels),
      .eGpio        = PM_GPIO_8,
   },

   {
      .paucChannels = aucGpio9Channels,
      .uNumChannels = ARRAY_LENGTH(aucGpio9Channels),
      .eGpio        = PM_GPIO_9,
   },

   {
      .paucChannels = aucGpio10Channels,
      .uNumChannels = ARRAY_LENGTH(aucGpio10Channels),
      .eGpio        = PM_GPIO_10,
   },

   {
      .paucChannels = aucGpio11Channels,
      .uNumChannels = ARRAY_LENGTH(aucGpio11Channels),
      .eGpio        = PM_GPIO_11,
   },

   {
      .paucChannels = aucGpio12Channels,
      .uNumChannels = ARRAY_LENGTH(aucGpio12Channels),
      .eGpio        = PM_GPIO_12,
   },

   {
      .paucChannels = aucGpio21Channels,
      .uNumChannels = ARRAY_LENGTH(aucGpio21Channels),
      .eGpio        = PM_GPIO_21,
   },

   {
      .paucChannels = aucGpio22Channels,
      .uNumChannels = ARRAY_LENGTH(aucGpio22Channels),
      .eGpio        = PM_GPIO_22,
   },

   {
      .paucChannels = aucGpio23Channels,
      .uNumChannels = ARRAY_LENGTH(aucGpio23Channels),
      .eGpio        = PM_GPIO_23,
   },
};

const VAdcBspType VAdcBsp[] =
{
   {
      .paChannels              = gVAdcChannels,
      .puNumChannels           = &uNumVAdcChannels,
      .paAveragedChannels      = NULL,
      .puNumAveragedChannels   = NULL,
      .paGpioChannelMappings   = vAdcGpioChannelMappings,
      .uNumGpioChannelMappings = ARRAY_LENGTH(vAdcGpioChannelMappings),
      .bUsesInterrupts         = FALSE,
      .uFullScale_code         = 0x4000,
      .uFullScale_uV           = 1875000,
      .uReadTimeout_us         = 500000,
      .uLDOSettlingTime_us     = 17,
      .uSlaveId                = 0,
      .uPeripheralId           = 0x31,
      .uMasterID               = 0,
      .eAccessPriority         = SPMI_BUS_ACCESS_PRIORITY_LOW,
      .ucPmicDevice            = 0,
      .usMinDigRev             = VADC_REVISION(3, 0),
      .usMinAnaRev             = VADC_REVISION(1, 0),
      .ucPerphType             = 0x8,
   }
};

