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


/*==========================================================================

                     MACRO DEFINITIONS FOR THIS FILE

==========================================================================*/

/*==========================================================================

                    APPLICATION PROGRAMMERS INTERFACE

==========================================================================*/

/*===============================================================================

  FUNCTION
    void HAL_tlmm_GetInactiveConfig (*pBlock, nOffs nGPIO, *pConfig)

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

void HAL_tlmm_GetInactiveConfig
(
  HALgpioBlockDescType *pBlock,
  uint32                nOffs,
  uint32                nGPIO,
  GPIOConfigType       *pConfig,
  GPIOValueType        *pValue
)
{
  if ( pBlock                                  != NULL &&
       pBlock->pmExtControl                    != NULL &&
       pBlock->pmExtControl->GetInactiveConfig != NULL )
  {
    pBlock->pmExtControl->GetInactiveConfig(pBlock, nOffs, nGPIO, pConfig, pValue);    
  }

} /* HAL_tlmm_GetInactiveConfig */


/*===============================================================================

  FUNCTION
    void HAL_tlmm_SetInactiveConfig (*pBlock, nOffs nGPIO, *pConfig)

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

void HAL_tlmm_SetInactiveConfig
(
  HALgpioBlockDescType *pBlock,
  uint32                nOffs,
  uint32                nGPIO,
  GPIOConfigType        Config,
  GPIOValueType         Value
)
{
  if ( pBlock                                   != NULL &&
       pBlock->pmExtControl                     != NULL &&
       pBlock->pmExtControl->SetInactiveConfig  != NULL )
  {
    pBlock->pmExtControl->SetInactiveConfig(pBlock, nOffs, nGPIO, Config, Value);
  }
}

