#ifndef __GPIODIAG_H__
#define __GPIODIAG_H__
/*===========================================================================

P U B L I C   H E A D E R   F I L E   F O R   A C C E S S   T O   T H E

M A I N   T L M M / G P I O   D R I V E R

DESCRIPTION
Thils file contains public data structures, enuerations, and APIs for accessing
GPIO pins in the TLMM block.

REFERENCES

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

#include "GPIOTypes.h"
#include "GPIOCore.h"

#include <stdlib.h>


/* =========================================================================
**  Function : GPIO_GetInactiveConfig
** =========================================================================*/
/**
  Gets the saved inactive configuration for a GPIO pin.

  This function retrieves the saved inactive GPIO configuration for a
  particular GPIO pin, which would be used when a client no longer requires
  the pin in an active state.

  @param Handle    [in] The device handle (see GPIO_Attach).
  @param GPIOKey   [in] A valid GPIO pin key.
  @param pConfig   [in/out] The GPIO number to check.
  @param pValue    [in/out] The output value of the inactive GPIO configuration.


  @return
  TRUE  -- The GPIO is currently active.
  FALSE -- The GPIO is currently inactive.

  @dependencies
  None.
*/

GPIOResult GPIO_GetInactiveConfig
(
  GPIOClientHandleType Handle,
  GPIOKeyType          GPIOKey,
  GPIOConfigType      *pConfig,
  GPIOValueType       *pValue
);



/* =========================================================================
**  Function : GPIO_SetInactiveConfig
** =========================================================================*/
/**
  Changes the saved inactive configuration for a GPIO pin.  This is usually
  not necessary as the optimized low-power setting is usually applied in
  the boot-loader.

  This function changes the saved inactive GPIO configuration for a
  particular GPIO pin, which would be used when a client no longer requires
  the pin in an active state.

  @param Handle   [in] The device handle (see GPIO_Attach).
  @param GPIOKey  [in] A valid GPIO pin key.
  @param Config   [in] The GPIO number to check.
  @param Value    [in] The output value of the inactive GPIO configuration.


  @return
  TRUE  -- The GPIO is currently active.
  FALSE -- The GPIO is currently inactive.

  @dependencies
  None.
*/

GPIOResult GPIO_SetInactiveConfig
(
  GPIOClientHandleType Handle,
  GPIOKeyType          GPIOKey,
  GPIOConfigType       Config,
  GPIOValueType        Value
);



/* =========================================================================
**  Function : GPIO_IsPinActive
** =========================================================================*/
/**
  Checks if a GPIO pin is configured to an active state.

  This function will check if a GPIO pin is configured to an active, mission
  mode state or if it is configured as inactive (low power).

  @param Handle    [in] The device handle (see GPIO_Attach).
  @param GPIOKey   [in] A valid GPIO pin key.

  @return
  TRUE  -- The GPIO is currently active.
  FALSE -- The GPIO is currently inactive.

  @dependencies
  None.
*/

boolean GPIO_IsPinActive
(
  GPIOClientHandleType Handle,
  GPIOKeyType          GPIOKey
);



/* =========================================================================
**  Function : GPIO_GetOutput
** =========================================================================*/
/**
  Returns the output value programmed to the particular GPIO.

  This function reads the value of the output register and returns its state.
  It is for use with driving GPIO pins high or low (see GPIO_WritePin).

  @param Handle    [in] The device handle (see GPIO_Attach).
  @param GPIOKey   [in] A valid GPIO pin key.

  @return
  GPIO_HIGH -- The GPIO is currently programmed to drive high.
  GPIO_LOW  -- The GPIO is currently programmed to drive  low.

  @dependencies
  None.
*/

GPIOValueType GPIO_GetOutput
(
  GPIOClientHandleType Handle,
  GPIOKeyType          GPIOKey
);



/* =========================================================================
**  Function : GPIO_GetPinConfig
** =========================================================================*/
/**
  Retrieves the current GPIO pin configuration.

  This function returns the current GPIO configuration programmed to hardware.

  @param  Handle  [in] The device handle (see GPIO_Attach).
  @param  GPIOKey [in] A valid GPIO pin key.
  @param *Config  [in] Storage for the configuration retrieval.

  @return
  GPIO_SUCCESS  -- The GPIO configuration was retrieved.
  GPIO_ERROR    -- A parameter was invalid.

  @dependencies
  None.
*/

GPIOResult GPIO_GetPinConfig
(
  GPIOClientHandleType Handle,
  GPIOKeyType          GPIOKey,
  GPIOConfigType      *Config
);


/* =========================================================================
**  Function : GPIO_GetPinNumber
** =========================================================================*/
/**
  Retrieves the GPIO physical pin number for a GPIO ID.

  This function returns the physical GPIO pin number corresponding to a 
  GPIO ID.

  @param  GPIOKey [in] A valid GPIO pin key.

  @return
  uint32 - The GPIO number read from the GPIO ID.

  @dependencies
  None.
*/
uint32 GPIO_GetPinNumber( GPIOKeyType nGpioId );

#endif /* __GPIODIAG_H__ */
