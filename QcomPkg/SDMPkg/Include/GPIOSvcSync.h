#ifndef __GPIOSVCSYNC_H__
#define __GPIOSVCSYNC_H__
/*===========================================================================

         H E A D E R   F I L E   F O R   A C C E S S   T O   T H E

         M A I N   G P I O   S Y N C H R O N I Z A T I O N   A P I

DESCRIPTION
  This file provides the function signatures for the GPIO synchronization
  object.

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

#include "com_dtypes.h"


/*==========================================================================

  APPLICATION PROGRAMMERS INTERFACE SIGNATURES

==========================================================================*/

/* =========================================================================
**  Function : GPIO_SvcSynInit
** =======================================================================*/
/**
  Initializes the associated synchronization object.

  This function is used to initialize the synchronization object.

  @return
  TRUE  -- The object was initialized.
  FALSE -- The object was not initialized.

  @dependencies
  None.
*/

boolean GPIO_SvcSynInit(void);



/* =========================================================================
**  Function : GPIO_SyncEnter
** =======================================================================*/
/**
  Enters synchronization, recursively where supported.

  This function is used to enter synchronization.  Where supported, the
  implementation should use recursive entry/exit.

  @dependencies
  None.
*/

void GPIO_SyncEnter(void);



/* =========================================================================
**  Function : GPIO_SyncExit
** =======================================================================*/
/**
  Exits synchronization, recursively where supported.

  This function is used to exit synchronization.  Where supported, the
  implementation should use recursive entry/exit.

  @dependencies
  None.
*/

void GPIO_SyncExit(void);


#endif /* __GPIOSVCSYNC_H__ */
