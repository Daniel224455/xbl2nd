#ifndef __GPIO_H__
#define __GPIO_H__
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
#include <stdlib.h>



/* =========================================================================
**  Function : GPIO_Attach
** =========================================================================*/
/**
  Attaches to a GPIO device and initializes a client handle.

  This function is required to be called first on a per-GPIO device basis.
  A client can attach to multiple GPIO devices, but must call this API for
  each required device.  The handle initialized in this function will be
  passed to all subsequent functions beginning with the GPIO_RegisterPin() or
  GPIO_RegisterPinExplicit().

  @param eDevice  [in] The GPIO HW device required by the client.
  @param pHandle  [in] A client handle to initialize.

  @return
  GPIO_SUCCESS -- A client handle was created.
  GPIO_ERROR   -- A QDI open call failed or the parameter is invalid.

  @dependencies
  None.
*/

GPIOResult GPIO_Attach
(
  GPIODeviceIdType      eDevice,
  GPIOClientHandleType *pHandle
);



/* =========================================================================
**  Function : GPIO_RegisterPin
** =========================================================================*/
/**
  Registers a GPIO signal with the GPIO driver for accessing additional APIs.

  This function must be called on a per-GPIO basis for a particular device
  (see GPIO_Attach).  Using pre-defined string signal name, clients will call
  this API to retrieve the GPIO pin key which will be passed into all subsequent
  functions and identifies the associated GPIO as valid and available.

  @param  Handle     [in] The device handle (see GPIO_Attach).
  @param *pszSignal  [in] The string signal name to retrieve a GPIO pin key for.
  @param  nStrLength [in] The size in bytes of the pszSignal string.
  @param *GPIOKey    [in] Storage for new GPIO pin key creation.
  @param AccessType  [in] The type of access desired - shared or exclusive
                          (see GPIOTypes.h).

  @return
  GPIO_SUCCESS -- The operation completed successfully.
  GPIO_ERROR   -- A QDI open call failed or one or more of the parameters is invalid.

  @dependencies
  None.
*/

GPIOResult GPIO_RegisterPin
(
  GPIOClientHandleType  Handle,
  const char           *pszSignal,
  GPIOAccessType        AccessType,
  GPIOConfigType       *pConfig,
  GPIOKeyType          *pGPIOKey
);



/* =========================================================================
**  Function : GPIO_RegisterPinExplicit
** =========================================================================*/
/**
  Registers a GPIO signal with the GPIO driver for accessing additional APIs
  for environments where the GPIO string signal name is not available.

  This function must be called on a per-GPIO basis for a particular device
  (see GPIO_Attach).  Using a GPIO configuration, clients will call
  this API to retrieve the GPIO pin key which will be passed into all subsequent
  functions and identifies the associated GPIO as valid and available.

  Note:  The GPIOConfigType Config structure must have the GPIO number
         initialized before calling this API as it will be used to register
         the pin.  The rest of the parameters are not used in this function.

  @param  Handle    [in] The device handle (see GPIO_Attach).
  @param nPinNumber [in] Address of a GPIOConfigType configuration.
  @param AccessType [in] The type of access desired - shared or exclusive
                         (see GPIOTypes.h).  Shared will allow multiple entities
                         to retrieve the same pin key, while exclusive will lock
                         the GPIO to the first caller.
  @param *GPIOKey   [in] Container for the GPIOKey to access GPIO APIs.

  @return
  GPIO_SUCCESS -- The operation completed successfully.
  GPIO_ERROR   -- A QDI open call failed or one or more of the parameters is invalid.

  @dependencies
  GPIO_Attach() must have been called prior to calling this API.
*/

GPIOResult GPIO_RegisterPinExplicit
(
  GPIOClientHandleType  Handle,
  uint32                nPinNumber,
  GPIOAccessType        AccessType,
  GPIOKeyType          *GPIOKey
);



/* =========================================================================
**  Function : GPIO_ConfigPin
** =========================================================================*/
/**
  Configures a GPIO pin.

  This function configures a GPIO pin to the setting specified by the calling
  client if the GPIO is properly registered (see GPIO_RegisterPin and
  GPIO_RegisterPinExplicit).

  @param Handle  [in] The device handle (see GPIO_Attach).
  @param GPIOKey [in] A valid GPIO pin key.
  @param Config  [in] The GPIO configuration to program.


  @return
  GPIO_SUCCESS -- The operation completed successfully.
  GPIO_ERROR   -- A QDI open call failed or one or more of the parameters is invalid.

  @dependencies
  None.
*/

GPIOResult GPIO_ConfigPin
(
  GPIOClientHandleType  Handle,
  GPIOKeyType           GPIOKey,
  GPIOConfigType        Config
);



/* =========================================================================
**  Function : GPIO_ConfigPinInactive
** =========================================================================*/
/**
  Configures a GPIO pin.

  This function configures a GPIO pin to a pre-determined low-power setting,
  which has been specified at boot time or otherwise stated.

  @param Handle  [in] The device handle (see GPIO_Attach).
  @param GPIOKey [in] A valid GPIO pin key.


  @return
  GPIO_SUCCESS -- The operation completed successfully.
  GPIO_ERROR   -- A QDI open call failed or one or more of the parameters is invalid.

  @dependencies
  None.
*/

GPIOResult GPIO_ConfigPinInactive
(
  GPIOClientHandleType  Handle,
  GPIOKeyType           GPIOKey
);



/* =========================================================================
**  Function : GPIO_ReadPin
** =========================================================================*/
/**
  Reads the input state of the GPIO line.

  This function can be used to read the input signal of a pin (high or low).

  @param Handle    [in] The device handle (see GPIO_Attach).
  @param GPIOKey   [in]  A valid GPIO pin key.


  @return
  GPIO_SUCCESS -- The operation completed successfully.
  GPIO_ERROR   -- A parameter is invalid.

  @dependencies
  None.
*/

GPIOResult GPIO_ReadPin
(
  GPIOClientHandleType Handle,
  GPIOKeyType          GPIOKey,
  GPIOValueType       *pValue
);



/* =========================================================================
**  Function : GPIO_WritePin
** =========================================================================*/
/**
  Reads the input state of the GPIO line.

  This function can be used to read the input signal of a pin (high or low).

  @param Handle    [in] The device handle (see GPIO_Attach).
  @param GPIOKey   [in] A valid GPIO pin key.
  @param PinNumber [in] The GPIO number to read from.
  @param bValue    [in] The value (high or low) to drive.  TRUE = high,
                        FALSE = low.


  @return
  GPIO_SUCCESS -- The operation completed successfully..
  GPIO_ERROR   -- The parameters are invalid or the pin key is not registered.

  @dependencies
  None.
*/

GPIOResult GPIO_WritePin
(
  GPIOClientHandleType  Handle,
  GPIOKeyType           GPIOKey,
  GPIOValueType         Value
);


#endif /* __GPIO_H__ */