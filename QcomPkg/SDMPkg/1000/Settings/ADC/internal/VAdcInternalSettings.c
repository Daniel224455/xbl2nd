/*============================================================================
  FILE:         VAdcInternalSettings.c

  OVERVIEW:     Internal settings for VADC.

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
extern const VAdcChannelConfigType gVAdcChannelsPMC1000_1[];
extern const VAdcChannelConfigType gVAdcChannelsPMC1000L[];
extern const VAdcChannelConfigType gVAdcChannelsPMC1000_2[];
extern const uint32 guNumVAdcChannelsPMC1000_1;
extern const uint32 guNumVAdcChannelsPMC1000L;
extern const uint32 guNumVAdcChannelsPMC1000_2;

/*----------------------------------------------------------------------------
 * Static Variable Definitions
 * -------------------------------------------------------------------------*/
/*
 * This table is a mapping of ADC channels to GPIO inputs.
 */
static const uint8 aucAmux1GpioChans[] = {0x12, 0x32, 0x52, 0x72};
static const uint8 aucAmux2GpioChans[] = {0x13, 0x33, 0x53, 0x73};
static const uint8 aucAmux3GpioChans[] = {0x14, 0x34, 0x54, 0x74, 0x94};
static const uint8 aucAmux4GpioChans[] = {0x15, 0x35, 0x55, 0x75, 0x95};

/*----------------------------------------------------------------------------
 * PMC1000_1 Settings
 * -------------------------------------------------------------------------*/
static const VAdcGpioChannelMappingType vAdcGpioChannelMappingsPMC1000[] =
{
   {
      .paucChannels = aucAmux1GpioChans,
      .uNumChannels = ARRAY_LENGTH(aucAmux1GpioChans),
      .eGpio        = PM_GPIO_3,
   },

   {
      .paucChannels = aucAmux2GpioChans,
      .uNumChannels = ARRAY_LENGTH(aucAmux2GpioChans),
      .eGpio        = PM_GPIO_4,
   },

   {
      .paucChannels = aucAmux3GpioChans,
      .uNumChannels = ARRAY_LENGTH(aucAmux3GpioChans),
      .eGpio        = PM_GPIO_5,
   },

   {
      .paucChannels = aucAmux4GpioChans,
      .uNumChannels = ARRAY_LENGTH(aucAmux4GpioChans),
      .eGpio        = PM_GPIO_2,    /* Driver remaps to PM_GPIO_6 for < rev 3.0 */
   },
};

const VAdcBspType VAdcBspPMC1000_1[] =
{
   {
      .paChannels              = gVAdcChannelsPMC1000_1,
      .puNumChannels           = &guNumVAdcChannelsPMC1000_1,
      .paAveragedChannels      = NULL,
      .puNumAveragedChannels   = NULL,
      .paGpioChannelMappings   = vAdcGpioChannelMappingsPMC1000,
      .uNumGpioChannelMappings = ARRAY_LENGTH(vAdcGpioChannelMappingsPMC1000),
      .bUsesInterrupts         = FALSE,
      .uFullScale_code         = 0x70E4,
      .uFullScale_uV           = 1875000,
      .uReadTimeout_us         = 500000,
      .uLDOSettlingTime_us     = 67,
      .uSlaveId                = 0,
      .uPeripheralId           = 0x31,
      .uMasterID               = 0,
      .eAccessPriority         = SPMI_BUS_ACCESS_PRIORITY_LOW,
      .ucPmicDevice            = 0,
   }
};

/*----------------------------------------------------------------------------
 * PMC1000L Settings
 * -------------------------------------------------------------------------*/
static const VAdcGpioChannelMappingType vAdcGpioChannelMappingsPMC1000L[] =
{
   {
      .paucChannels = aucAmux1GpioChans,
      .uNumChannels = ARRAY_LENGTH(aucAmux1GpioChans),
      .eGpio        = PM_GPIO_5,
   },

   {
      .paucChannels = aucAmux2GpioChans,
      .uNumChannels = ARRAY_LENGTH(aucAmux2GpioChans),
      .eGpio        = PM_GPIO_6,
   },

   {
      .paucChannels = aucAmux3GpioChans,
      .uNumChannels = ARRAY_LENGTH(aucAmux3GpioChans),
      .eGpio        = PM_GPIO_7,
   },

   {
      .paucChannels = aucAmux4GpioChans,
      .uNumChannels = ARRAY_LENGTH(aucAmux4GpioChans),
      .eGpio        = PM_GPIO_10,
   },
};

const VAdcBspType VAdcBspPMC1000L[] =
{
   {
      .paChannels              = gVAdcChannelsPMC1000L,
      .puNumChannels           = &guNumVAdcChannelsPMC1000L,
      .paAveragedChannels      = NULL,
      .puNumAveragedChannels   = NULL,
      .paGpioChannelMappings   = vAdcGpioChannelMappingsPMC1000L,
      .uNumGpioChannelMappings = ARRAY_LENGTH(vAdcGpioChannelMappingsPMC1000L),
      .bUsesInterrupts         = FALSE,
      .uFullScale_code         = 0x70E4,
      .uFullScale_uV           = 1875000,
      .uReadTimeout_us         = 500000,
      .uLDOSettlingTime_us     = 67,
      .uSlaveId                = 4,
      .uPeripheralId           = 0x31,
      .uMasterID               = 0,
      .eAccessPriority         = SPMI_BUS_ACCESS_PRIORITY_LOW,
      .ucPmicDevice            = 2,
   }
};

/*----------------------------------------------------------------------------
 * PMC1000_2 Settings
 * -------------------------------------------------------------------------*/
const VAdcBspType VAdcBspPMC1000_2[] =
{
   {
      .paChannels              = gVAdcChannelsPMC1000_2,
      .puNumChannels           = &guNumVAdcChannelsPMC1000_2,
      .paAveragedChannels      = NULL,
      .puNumAveragedChannels   = NULL,
      .paGpioChannelMappings   = vAdcGpioChannelMappingsPMC1000,
      .uNumGpioChannelMappings = ARRAY_LENGTH(vAdcGpioChannelMappingsPMC1000),
      .bUsesInterrupts         = FALSE,
      .uFullScale_code         = 0x70E4,
      .uFullScale_uV           = 1875000,
      .uReadTimeout_us         = 500000,
      .uLDOSettlingTime_us     = 67,
      .uSlaveId                = 8,
      .uPeripheralId           = 0x31,
      .uMasterID               = 0,
      .eAccessPriority         = SPMI_BUS_ACCESS_PRIORITY_LOW,
      .ucPmicDevice            = 4,
   }
};

