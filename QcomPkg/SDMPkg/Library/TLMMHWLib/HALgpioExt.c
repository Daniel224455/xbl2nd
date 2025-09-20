/*===========================================================================

M A I N   T L M M / G P I O   D R I V E R   F I L E

DESCRIPTION

This file contains API implementations for the Top-Level Mode Multiplexor
 and General Purpose I/O Driver.


===========================================================================
Copyright (c) 2017 QUALCOMM Technologies Incorporated.
All Rights Reserved.
QUALCOMM Proprietary/GTDR
===========================================================================*/

/*===========================================================================

EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.


when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/02/17   dcf     Created.
===========================================================================*/

/*==========================================================================

                         INCLUDE FILES FOR MODULE

==========================================================================*/

#include "HALtlmm.h"
#include "HALgpio.h"
#include "HALhwio.h"


/*==========================================================================

                     MACRO DEFINITIONS FOR THIS FILE

==========================================================================*/

#define HAL_GPIO_CFG_REG_OFFS_BMSK           0x000000000
#define HAL_GPIO_IN_OUT_REG_OFFS_BMSK        0x000000004
#define HAL_GPIO_INTR_CFG_REG_OFFS_BMSK      0x000000008
#define HAL_GPIO_INTR_STATUS_REG_OFFS_BMSK   0x00000000C
#define HAL_GPIO_ID_STATUS_REG_OFFS_BMSK     0x000000010
#define HAL_GPIO_LP_CFG_REG_OFFS_BMSK        0x000000014




uint32 HAL_gpio_DriveCvtToLogical
(
  uint32 nHWVal
);



HAL_tlmm_GpioExtControlType HAL_gpio_ExtControl =
{
  .GetInactiveConfig = HAL_gpio_GetInactiveConfig,
  .SetInactiveConfig = HAL_gpio_SetInactiveConfig,
};



/*==========================================================================

                    APPLICATION PROGRAMMERS INTERFACE

==========================================================================*/

/*===============================================================================

  FUNCTION
    void HAL_gpio_GetInactiveConfig (*pBlock, nOffs nGPIO, *pConfig)

  DESCRIPTION
    Returns the low-power configuration of a pin where supported.

  PARAMETERS
    *pBlock  - Pointer to a hardware block descriptor.
     nOffs   - Register offset.
     nGPIO   - The physical GPIO pin number.
     pConfig - Storage for the GPIO configuration.
     pValue  - Storage for the GPIO value (high/low) read from the low-power setting.


  DEPENDENCIES
  None.

  RETURN VALUE
    The configuration register address of the specified GPIO pin.

  SIDE EFFECTS
    None.

  ===============================================================================*/

void HAL_gpio_GetInactiveConfig
(
  HALgpioBlockDescType *pBlock,
  uint32                nOffs,
  uint32                nGPIO,
  GPIOConfigType       *pConfig,
  GPIOValueType        *pValue
)
{
  uint32 nGPIOCfg = in_dword(pBlock->nBaseAddr + nOffs + (pBlock->nPinRegWidth * nGPIO) + HAL_GPIO_LP_CFG_REG_OFFS_BMSK);
  pConfig->func = HAL_GPIO_GET_FUNC(nGPIOCfg);
  pConfig->dir = HAL_GPIO_GET_DIR(nGPIOCfg);
  pConfig->pull = HAL_GPIO_GET_PULL(nGPIOCfg);
  pConfig->drive = HAL_gpio_DriveCvtToLogical(HAL_GPIO_GET_DRIVE(nGPIOCfg));
  *pValue = (GPIOValueType)HAL_GPIO_GET_LP_OUTPUT(nGPIOCfg);

} /* HAL_gpio_GetInactiveConfig */


/*===============================================================================

  FUNCTION
    void HAL_gpio_SetInactiveConfig (*pBlock, nOffs nGPIO, *pConfig)

  DESCRIPTION
    Returns the low-power configuration of a pin where supported.

  PARAMETERS
    *pBlock  - Pointer to a hardware block descriptor.
     nOffs   - Register offset.
     nGPIO   - The physical GPIO pin number.
     Config  - Storage for the GPIO configuration.
     Value   - Storage for the GPIO value (high/low) read from the low-power setting.


  DEPENDENCIES
  None.

  RETURN VALUE
    The configuration register address of the specified GPIO pin.

  SIDE EFFECTS
    None.

  ===============================================================================*/

void HAL_gpio_SetInactiveConfig
(
  HALgpioBlockDescType *pBlock,
  uint32                nOffs,
  uint32                nGPIO,
  GPIOConfigType        Config,
  GPIOValueType         Value
)
{
  uint32 nGPIOCfgVal = HAL_GPIO_CFG_LP_TO_REG(Config.func,
                                              Config.dir,
                                              Config.pull,
                                              Config.drive,
                                              Value);

  out_dword(pBlock->nBaseAddr + nOffs + (pBlock->nPinRegWidth * nGPIO) + HAL_GPIO_LP_CFG_REG_OFFS_BMSK, nGPIOCfgVal);
}

