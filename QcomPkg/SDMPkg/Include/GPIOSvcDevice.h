#ifndef __GPIOSVCDEVICE_H__
#define __GPIOSVCDEVICE_H__
/*
===========================================================================

  @file GPIOSvcDevice.h

  This file declared the APIs for accessing the device configuration
  for GPIO settings, where available.

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


#include "com_dtypes.h"
#include "GPIOTypes.h"
#include "GPIOIFace.h"


/*==========================================================================

  APPLICATION PROGRAMMERS INTERFACE SIGNATURES

==========================================================================*/

/* =========================================================================
**  Function : GPIO_SvcDeviceInit
** =======================================================================*/
/**
  Initializes the associated device config connection.

  This function is used to initialize a connection to the device config
  object for the TLMM/GPIO driver.

  @return
  TRUE  -- The connection was initialized.
  FALSE -- The connection was not initialized.

  @dependencies
  None.
*/

boolean GPIO_SvcDeviceInit
(
  void
);



/* =========================================================================
**  Function : GPIO_SvcDeviceGetGpioId
** =======================================================================*/
/**
  Returns a GPIO ID for the the string-based signal name if supported.

  This function is used to retrieve a GPIO ID for a supported string-based
  signal name.

  @param pszString[in] -- String-based signal name to get ID for.
  @param pPin[inout]   -- Storage for the GPIO ID if found.

  @return
  TRUE  -- The string exists and is available.
  FALSE -- The string does not exist in device configuration.

  @dependencies
  None.
*/

GPIOIDType *GPIO_SvcDeviceGetGpioId
(
  const char *pszString
);

#endif /* __GPIOSVCDEVICE_H__ */
