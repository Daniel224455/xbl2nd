/*
==============================================================================

FILE:         TLMMBSP.c

DESCRIPTION:
  This file contains the TLMM driver hardware BSP data.


==============================================================================


==============================================================================
            Copyright (c) 2017 QUALCOMM Technologies Incorporated.
                    All Rights Reserved.
                  QUALCOMM Proprietary/GTDR
==============================================================================

*/


/*=========================================================================
      Include Files
==========================================================================*/

#include "msmhwiobase.h"
#include "GPIOTypes.h"
#include "HALtlmm.h"

/*=========================================================================
      Externs
==========================================================================*/

extern HAL_tlmm_GpioCoreControlType HAL_gpio_CoreControl;
extern HAL_tlmm_GpioExtControlType HAL_gpio_ExtControl;

/*=========================================================================
      Data Declarations
==========================================================================*/

HALgpioBlockDescType GPIOBlockDescriptors[] =
{
  {
    GPIO_DEVICE_TLMM,
    0,
    0x1000,
    5,
    {
      TLMM_EAST_BASE,
      TLMM_SOUTH_BASE,
      TLMM_WEST_BASE,
      TLMM_EAST_BASE + 0x00400000,
      TLMM_SOUTH_BASE + 0x0001E000,
    },
    0,
    189,
    GPIO_HW_VERSION(1, 0, 0xFF, 0xFF),
    &HAL_gpio_CoreControl,
    &HAL_gpio_ExtControl
  },
  
#ifdef ENABLE_AUTO_PLAT
  {
    GPIO_DEVICE_SSC_LPI,
    SSC_BASE,
    0x1000,
    1,
    {
      0x00B40000,
      0
    },
    0,
    13,
    GPIO_HW_VERSION(1, 0, 0xFF, 0xFF),
    &HAL_gpio_CoreControl,
    &HAL_gpio_ExtControl
  }, 
#endif

};

uint32 gnNumBlockDescs = sizeof(GPIOBlockDescriptors)/sizeof(HALgpioBlockDescType);

uint32 nHWRevisionAddr = (CORE_TOP_CSR_BASE + 0xC8000);
