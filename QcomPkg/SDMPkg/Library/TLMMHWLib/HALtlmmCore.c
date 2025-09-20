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


/*==========================================================================

                     MACRO DEFINITIONS FOR THIS FILE

==========================================================================*/

/*==========================================================================

                    APPLICATION PROGRAMMERS INTERFACE

==========================================================================*/

/*===============================================================================

  FUNCTION
    boolean HAL_tlmm_Init (*pBlock)

  DESCRIPTION
    Initializes the TLMM HW Block associated with the HW descriptor parameter.

  PARAMETERS
    *pBlock     - Pointer to a hardware block descriptor.

  DEPENDENCIES
    None.

  RETURN VALUE
    TRUE  - The HW was initialized successfully.
    FALSE - The HW initialization was not able to complete.

  SIDE EFFECTS
    None.

  ===============================================================================*/
boolean HAL_tlmm_Init
(
  HALgpioBlockDescType *pBlock
)
{
  if ( pBlock                      != NULL &&
       pBlock->pmCoreControl       != NULL &&
       pBlock->pmCoreControl->Init != NULL )
  {
    return pBlock->pmCoreControl->Init(pBlock);
  }
  return FALSE;
}

/*===============================================================================

  FUNCTION
    void HAL_tlmm_ConfigPin (*pBlock, nOffs, nGPIO, Config)

  DESCRIPTION
    Returns the HW address of a particular GPIO.

  PARAMETERS
    *pBlock     - Pointer to a hardware block descriptor.
    nGPIO       - The physical GPIO pin number.

  DEPENDENCIES
    None.

  RETURN VALUE
    The configuration register address of the specified GPIO pin.

  SIDE EFFECTS
    None.

  ===============================================================================*/

void HAL_tlmm_ConfigPin
(
  HALgpioBlockDescType *pBlock,
  uint32                nOffs,
  uint32                nGPIO,
  GPIOConfigType        Config
)
{
  if ( pBlock                           != NULL &&
       pBlock->pmCoreControl            != NULL &&
       pBlock->pmCoreControl->ConfigPin != NULL )
  {
    pBlock->pmCoreControl->ConfigPin(pBlock, nOffs, nGPIO, Config);
  }

} /* HAL_tlmm_ConfigPin */



/*===============================================================================

  FUNCTION
    void HAL_tlmm_ReadPin (*pBlock, nOffs, nGPIO)

  DESCRIPTION
    Returns the value of the GPIO signal (HIGH or LOW) in boolean format.

  PARAMETERS
    *pBlock  - Pointer to a hardware block descriptor.
     nOffs   - The register bank offset.
     nGPIO   - The physical GPIO pin number.

  DEPENDENCIES
  None.

  RETURN VALUE
    The configuration register address of the specified GPIO pin.

  SIDE EFFECTS
    None.

  ===============================================================================*/

GPIOValueType HAL_tlmm_ReadPin
(
  HALgpioBlockDescType *pBlock,
  uint32                nOffs,
  uint32                nGPIO
)
{
  if ( pBlock                         != NULL &&
       pBlock->pmCoreControl          != NULL &&
       pBlock->pmCoreControl->ReadPin != NULL )
  {
    return pBlock->pmCoreControl->ReadPin(pBlock, nOffs, nGPIO);
  }
  return GPIO_LOW;

} /* HAL_tlmm_ReadPin */


/*===============================================================================

  FUNCTION
    void HAL_tlmm_GetTileIndex (pBlock, nGPIO)

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

uint32 HAL_tlmm_GetTileIndex
(
  HALgpioBlockDescType *pBlock,
  uint32                nGPIO
)
{
  if ( pBlock                              != NULL &&
       pBlock->pmCoreControl               != NULL &&
       pBlock->pmCoreControl->GetTileIndex != NULL )
  {
    return pBlock->pmCoreControl->GetTileIndex(pBlock, nGPIO);
  }
  return 0;

} /* HAL_tlmm_GetTileIndex */


/*===============================================================================

  FUNCTION
    void HAL_tlmm_WritePin (pBlock, nOffs, nGPIO, nValue)

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

void HAL_tlmm_WritePin
(
  HALgpioBlockDescType *pBlock,
  uint32                nOffs,
  uint32                nGPIO,
  GPIOValueType         nValue
)
{
  if ( pBlock                          != NULL &&
       pBlock->pmCoreControl           != NULL &&
       pBlock->pmCoreControl->WritePin != NULL )
  {
    pBlock->pmCoreControl->WritePin(pBlock, nOffs, nGPIO, nValue);
  }

} /* HAL_tlmm_WritePin */



/*===============================================================================

  FUNCTION
    void HAL_tlmm_ReadPinOutput (pBlock, nOffs, nGPIO)

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

GPIOValueType HAL_tlmm_ReadPinOutput
(
  HALgpioBlockDescType *pBlock,
  uint32                nOffs,
  uint32                nGPIO
)
{
  if ( pBlock                               != NULL &&
       pBlock->pmCoreControl                != NULL &&
       pBlock->pmCoreControl->ReadPinOutput != NULL )
  {
    return pBlock->pmCoreControl->ReadPinOutput(pBlock, nOffs, nGPIO);
  }
  return 0;

} /* HAL_tlmm_ReadPinOutput */



/*===============================================================================

  FUNCTION
    void HAL_tlmm_GetPinConfig (pBlock, nOffs, nGPIO, *Config)

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

void HAL_tlmm_GetPinConfig
(
  HALgpioBlockDescType *pBlock,
  uint32                nOffs,
  uint32                nGPIO,
  GPIOConfigType       *Config
)
{
  if ( pBlock                              != NULL &&
       pBlock->pmCoreControl               != NULL &&
       pBlock->pmCoreControl->GetPinConfig != NULL )
  {
    pBlock->pmCoreControl->GetPinConfig(pBlock, nOffs, nGPIO, Config);
  }

} /* HAL_tlmm_GetPinConfig */
