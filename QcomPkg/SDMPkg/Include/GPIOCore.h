#ifndef __GPIOCORE_H__
#define __GPIOCORE_H__
/*==========================================================================

FILE:      GPIOCore.c

DESCRIPTION:
  This file contains internal type definitions of the GPIO Core driver.


============================================================================
Copyright (c) 2017 QUALCOMM Technologies Incorporated.
All Rights Reserved.
QUALCOMM Proprietary/GTDR
==========================================================================*/

/*==========================================================================

  EDIT HISTORY FOR FILE

  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.


  when       who     what, where, why
  --------   ---     -------------------------------------------------------
  03/02/17   dcf     Created.
  ========================================================================*/

/*==========================================================================

  INCLUDE FILES FOR MODULE

  ========================================================================*/

#include "GPIOTypes.h"


/*==========================================================================

                 INTERNAL MACROS AND DATA STRUCTURES

  ========================================================================*/


/*
 * Static array size for GPIO access information.  This array is a subset of
 * the total pins.  If exhausted, the size must be increased at a multiple of
 * 2^n to ensure sizing requirements with the GPIOPinInfo.Info.PinIdx field.
 * The GPIOPinInfo.Info.PinIdx field must also be expanded to account for the
 * increase such that the PinIdx field will not be able to be larger than the
 * total elements in the array.
 */
#define GPIO_MAX_ARR_SIZE  32


/*==========================================================================

                     INTERNAL DATA STRUCTURES

  =========================================================================*/

/*
 * GPIO Pin information structure.
 *
 * This structure describes the details for accessing TLMM HW as well as
 * validating information pertaining to the usage and accessibility of a
 * GPIO pin.
 */
typedef union
{
  struct
  {
    uint32 PinIdx   :  5;    /* Array index of this GPIO pin. */
    uint32 Pin      :  8;    /* Physical GPIO pin number.     */
    uint32 Id       :  8;    /* Unique identifier for this pin. */
    uint32 Access   :  1;    /* Access Type EXCLUSIVE or SHARED. */
    uint32 DeviceId :  3;    /* The device ID for this entry. */
    uint32 TileIdx  :  3;    /* Index of the TLMM Tile for this pin. */
    uint32 DescIdx  :  3;    /* Index of the hardware descriptor structure
                                for this pin. */
  }Info;
  GPIOKeyType All;            /* Used to copy to/from user supplied
                                 GPIOKeyType structure (uint32). */

}GPIOPinInfoType;



#endif /* __GPIOCORE_H__ */
