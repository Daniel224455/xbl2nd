/*===========================================================================

M A I N   T L M M / G P I O   D R I V E R   F I L E

DESCRIPTION

This file contains API implementations for the Top-Level Mode Multiplexor
 and General Purpose I/O Driver.


===========================================================================
Copyright (c) 2017-2018 QUALCOMM Technologies Incorporated.
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

                INTERFACE  FUNCTION POINTER INITIALIZATION

==========================================================================*/

HAL_tlmm_GpioCoreControlType HAL_gpio_CoreControl = 
{
  .Init              = HAL_gpio_Init,
  .ConfigPin         = HAL_gpio_ConfigPin,
  .ReadPin           = HAL_gpio_ReadPin,
  .WritePin          = HAL_gpio_WritePin,
  .GetTileIndex      = HAL_gpio_GetTileIndex,
  .ReadPinOutput     = HAL_gpio_ReadPinOutput,
  .GetPinConfig      = HAL_gpio_GetPinConfig
};


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

/*==========================================================================

                    APPLICATION PROGRAMMERS INTERFACE

==========================================================================*/


/*===============================================================================

  FUNCTION
    boolean HAL_gpio_Init (*pBlock)

  DESCRIPTION
    Performs any common HW initialization of the TLMM block.

  PARAMETERS
    *pBlock  - Pointer to a hardware block descriptor.

  DEPENDENCIES
    None.

  RETURN VALUE
    TRUE  - Initialization occurred successfully.
    FALSE - Initialization could not complete successfully.

  SIDE EFFECTS
    None.

  ===============================================================================*/
  
boolean HAL_gpio_Init
(
  HALgpioBlockDescType *pBlock
)
{
  return TRUE;
  
} /* HAL_gpio_Init */


/*===============================================================================

  FUNCTION
    void HAL_gpio_ConfigPin (*pBlock, nOffs, nGPIO, Config)

  DESCRIPTION
    Configures a GPIO pin to a specified functionality.

  PARAMETERS
    *pBlock  - Pointer to a hardware block descriptor.
    nOffs    - The GPIO offset to use.
    nGPIO    - The pin to configure.
    Config   - The configuration to set to hardware.

  DEPENDENCIES
    None.

  RETURN VALUE
    The configuration register address of the specified GPIO pin.

  SIDE EFFECTS
    None.

  ===============================================================================*/

void HAL_gpio_ConfigPin
(
  HALgpioBlockDescType *pBlock,
  uint32                nOffs,
  uint32                nGPIO,
  GPIOConfigType        Config
)
{
  uint32 nRegVal = 0;

    nRegVal =
          HAL_GPIO_CFG_TO_REG(Config.func,
                              Config.dir,
                              Config.pull,
                              HAL_gpio_DriveCvtToHW(Config.drive));

  /*
   * Check for eGPIO and enable if required.
   */
  if ( in_dword(pBlock->nBaseAddr + nOffs + (pBlock->nPinRegWidth * nGPIO) + HAL_GPIO_CFG_REG_OFFS_BMSK) & HAL_GPIO_CFG_EGPIO_PRESENT_BMSK )
  {
    nRegVal |= HAL_GPIO_CFG_EGPIO_ENABLE_BMSK;
  }

  out_dword(pBlock->nBaseAddr + nOffs + (pBlock->nPinRegWidth * nGPIO) + HAL_GPIO_CFG_REG_OFFS_BMSK, nRegVal);

} /* HAL_gpio_ConfigPin */



/*===============================================================================

  FUNCTION
    void HAL_gpio_ReadPin (*pBlock, nOffs, nGPIO)

  DESCRIPTION
    Returns the value of the GPIO signal (HIGH or LOW) in boolean format.

  PARAMETERS
    *pBlock  - Pointer to a hardware block descriptor.
     nOffs   - The register bank offset.
     nGPIO   - The physical GPIO pin number.

  DEPENDENCIES
  None.

  RETURN VALUE
    The pin signal value read, high or low.

  SIDE EFFECTS
    None.

  ===============================================================================*/

GPIOValueType HAL_gpio_ReadPin
(
  HALgpioBlockDescType *pBlock,
  uint32                nOffs,
  uint32                nGPIO
)
{
  return (in_dword(pBlock->nBaseAddr + nOffs + (pBlock->nPinRegWidth * nGPIO) + HAL_GPIO_IN_OUT_REG_OFFS_BMSK) & 0x1) ? GPIO_HIGH : GPIO_LOW;

} /* HAL_gpio_ReadPin */



/*===============================================================================

  FUNCTION
    void HAL_gpio_WritePin (pBlock, nOffs, nGPIO, nValue)

  DESCRIPTION
    Programs the output value of a GPIO pin.

  PARAMETERS
    pBlock  - The HW block descriptor.
    nOffs   - The offset (tile) of a particular pin.
    nGPIO   - The GPIO pin number.
    nValue   - The value (HIGH/LOW) to write.

  DEPENDENCIES
    None.

  RETURN VALUE
    None.

  SIDE EFFECTS
    None.

  ===============================================================================*/

void HAL_gpio_WritePin
(
  HALgpioBlockDescType *pBlock,
  uint32                nOffs,
  uint32                nGPIO,
  GPIOValueType         nValue
)
{
  uint32 nVal = 0;

  if ( nValue == GPIO_HIGH )
  {
    nVal = 0x1;
  }
  out_dword(pBlock->nBaseAddr + nOffs +
        (pBlock->nPinRegWidth * nGPIO) + HAL_GPIO_IN_OUT_REG_OFFS_BMSK, (nVal & 0x1) << 1);

} /* HAL_gpio_WritePin */



/*===============================================================================

  FUNCTION
    void HAL_gpio_DriveCvtToLogical (uint32 nHWVal)

  DESCRIPTION
    Converts a HW register value of the drive strength to a SW logical value.
    The logical value is in hundredths of milliamps.

  PARAMETERS
    nHWVal - The HW register value for drive strength.

  DEPENDENCIES
    None.

  RETURN VALUE
    Returns the logical drive strength in hundredths of milliamps.

  SIDE EFFECTS
    None.

  ===============================================================================*/

uint32 HAL_gpio_DriveCvtToLogical
(
  uint32 nHWVal
)
{
  return ((nHWVal + 1) * 200);

} /* HAL_gpio_DriveCvtToLogical */



/*===============================================================================

  FUNCTION
    void HAL_gpio_DriveCvtToHW (uint32 nLogicalVal)

  DESCRIPTION
    Converts a SW logical drive strength value in hundredths of milliamps to a
    HW register value to program drive strength for a SW controlled output pin.

  PARAMETERS
    nLogicalVal - The logical drive strength value to retrieve the HW setting for.

  DEPENDENCIES
    None.

  RETURN VALUE
    Returns the physical register value of the drive strength for this device.

  SIDE EFFECTS
    None.

  ===============================================================================*/

uint32 HAL_gpio_DriveCvtToHW
(
  uint32 nLogicalVal
)
{
  if (nLogicalVal <=200)  { return 0;}
  if (nLogicalVal <=400)  { return 1;}
  if (nLogicalVal <=600)  { return 2;}
  if (nLogicalVal <=800)  { return 3;}
  if (nLogicalVal <=1000) { return 4;}
  if (nLogicalVal <=1200) { return 5;}
  if (nLogicalVal <=1400) { return 6;}

  return 7;

} /* HAL_gpio_DriveCvtToHW */


/*===============================================================================

  FUNCTION
    void HAL_gpio_GetTileIndex (pBlock, nGPIO)

  DESCRIPTION
    Retrieves the array index for the tile associated with the GPIO pin.

  PARAMETERS
    pBlock  - The HW block descriptor.
    nGPIO   - The GPIO pin number.

  DEPENDENCIES
    None.

  RETURN VALUE
    Returns the correponding index or 0 if there are zero or 1 indices.

  SIDE EFFECTS
    None.

  ===============================================================================*/

uint32 HAL_gpio_GetTileIndex
(
  HALgpioBlockDescType *pBlock,
  uint32 nGPIO
)
{
  uint32 nIdx;
  uint32 nValue;

  if ( pBlock->nPinRegWidth < 2 )
  {
    return 0;
  }

  for (nIdx = 0; nIdx < pBlock->nNumTiles; nIdx++)
  {
    nValue = pBlock->nBaseAddr + pBlock->aTiles[nIdx] + (pBlock->nPinRegWidth * nGPIO) + HAL_GPIO_ID_STATUS_REG_OFFS_BMSK;
    if (in_dword(nValue) & 0x1)
    {
      return (nIdx);
    }
  }
  return 0;

} /* HAL_gpio_GetTileIndex */



/*===============================================================================

  FUNCTION
    GPIOValueType HAL_gpio_ReadPinOutput (pBlock, nOffs, nGPIO)

  DESCRIPTION
    Reads the programmed output value of a GPIO pin.

  PARAMETERS
    pBlock  - The HW block descriptor.
    nOffs   - The offset (tile) of a particular pin.
    nGPIO   - The GPIO pin number.

  DEPENDENCIES
    None.

  RETURN VALUE
    Returns the value (HIGH/LOW) in GPIOValueType format.

  SIDE EFFECTS
    None.

  ===============================================================================*/

GPIOValueType HAL_gpio_ReadPinOutput
(
  HALgpioBlockDescType *pBlock,
  uint32                nOffs,
  uint32                nGPIO
)
{
  if ( ((in_dword(pBlock->nBaseAddr + nOffs +
       (pBlock->nPinRegWidth * nGPIO) + HAL_GPIO_IN_OUT_REG_OFFS_BMSK) & 0x2) >> 1) & 0x1 )
  {
   return GPIO_HIGH;                          
  }
  return GPIO_LOW;

} /* HAL_gpio_ReadPinOutput */



/*===============================================================================

  FUNCTION
    void HAL_gpio_GetPinConfig (pBlock, nOffs, nGPIO, *Config)

  DESCRIPTION
    Reads the current GPIO configuration from hardware.

  PARAMETERS
    pBlock  - The HW block descriptor.
    nOffs   - The offset (tile) of a particular pin.
    nGPIO   - The GPIO pin number.
    *Config - Storage for the configuration to retrieve.

  DEPENDENCIES
    None.

  RETURN VALUE
    None.

  SIDE EFFECTS
    None.

  ===============================================================================*/

void HAL_gpio_GetPinConfig
(
  HALgpioBlockDescType *pBlock,
  uint32                nOffs,
  uint32                nGPIO,
  GPIOConfigType       *Config
)
{
  uint32 nRegVal = in_dword(pBlock->nBaseAddr + nOffs + (pBlock->nPinRegWidth * nGPIO) + HAL_GPIO_CFG_REG_OFFS_BMSK);

  Config->func   = HAL_GPIO_GET_FUNC(nRegVal);
  Config->dir    = HAL_GPIO_GET_DIR(nRegVal);
  Config->pull   = HAL_GPIO_GET_PULL(nRegVal);
  Config->drive  = HAL_gpio_DriveCvtToLogical(HAL_GPIO_GET_DRIVE(nRegVal));

} /* HAL_gpio_GetPinConfig */
