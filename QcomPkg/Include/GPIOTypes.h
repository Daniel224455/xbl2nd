#ifndef __GPIOTYPES_H__
#define __GPIOTYPES_H__
/*===========================================================================

P U B L I C   H E A D E R   F I L E   F O R   D A T A T Y P E S   U S E D

            I N   T H E   G P I O   D R I V E R   A P I S

DESCRIPTION
Thils file contains public data structures, and enuerations for accessing
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

#include "com_dtypes.h"


/*==========================================================================

                DATA STRUCTURES USED IN THIS MODULE

==========================================================================*/

/*
 * QDI TLMM/GPIO driver name.  This is generic.
 */
#define GPIO_DRV_NAME "/drv/gpio"



/*
 * The set upper limit of a GPIO signal name string length.
 */
#define GPIO_MAX_STRING_LENGTH 50


/*
 * QDI Commands for API routing.
 */
#define GPIO_CMD_ROUTER               (1011+QDI_PRIVATE)
#define GPIO_CMD_ATTACH               (1012+QDI_PRIVATE)
#define GPIO_CMD_REGISTER_PIN         (1013+QDI_PRIVATE)
#define GPIO_CMD_REGISTER_PIN_EX      (1014+QDI_PRIVATE)
#define GPIO_CMD_CONFIG_PIN           (1015+QDI_PRIVATE)
#define GPIO_CMD_CONFIG_PIN_INACTIVE  (1016+QDI_PRIVATE)
#define GPIO_CMD_IS_PIN_ACTIVE        (1017+QDI_PRIVATE)
#define GPIO_CMD_READ_PIN             (1018+QDI_PRIVATE)
#define GPIO_CMD_WRITE_PIN            (1019+QDI_PRIVATE)
#define GPIO_CMD_GET_PIN_OUTPUT       (1020+QDI_PRIVATE)
#define GPIO_CMD_GET_PIN_CONFIG       (1021+QDI_PRIVATE)
#define GPIO_CMD_SET_INACTIVE_CONFIG  (1022+QDI_PRIVATE)
#define GPIO_CMD_GET_INACTIVE_CONFIG  (1023+QDI_PRIVATE)


/*
 * GPIO Devices that can be accessed.
 */
typedef enum
{
  GPIO_DEVICE_TLMM     = 0,     /*<-- Chip-level TLMM > */
  GPIO_DEVICE_SSC_LPI  = 1,     /*<-- SSC TLMM        > */

  GPIO_DEVICE_MAX      = 2,

  GPIO_DEVICE_ID_FORCE32  = 0x7FFFFFFF

} GPIODeviceIdType;



/*
 * GPIO Return types used by the GPIO driver.
 */
typedef enum
{
  GPIO_SUCCESS               = 0,
  GPIO_ERROR                 = -1,
  GPIO_INVALID_PARAMETER     = 2,
  GPIO_INTERNAL_ERROR        = 3,
  GPIO_RESOURCE_UNAVAILABLE  = 4,
  GPIO_RESOURCE_NOT_FOUND    = 5,

  GPIO_RESULT_FORCE32        = 0x7FFFFFFF

}GPIOResult;



/*
 * GPIO Access Type.
 *
 * The following enumeration is used to specify if a GPIO ID should be
 * shared or exclusive to the registering driver.  If it is shared,
 * other drivers can retrieve the same GPIO ID.
 */
typedef enum
{
  GPIO_ACCESS_EXCLUSIVE = 0,  /**<-- Exclusive access of a GPIO  >*/
  GPIO_ACCESS_SHARED    = 1,  /**<-- Shared access of a GPIO     >*/

  GPIO_ACCESS_FORCE32 = 0x7FFFFFFF

} GPIOAccessType;



/*
 * GPIO state.
 *
 * Specifies the active or inactive (low-power) state of a GPIO pin.
 */
typedef enum
{
  GPIO_STATE_ACTIVE  = 0,   /**<-- GPIO State Active (POR)         >*/
  GPIO_STATE_INACTIVE = 1,  /**<-- GPIO State Inactive (low-power) >*/

  GPIO_STATE_FORCE32 = 0x7FFFFFFF

} GPIOStateType;



/*
 * GPIOPullType
 *
 * GPIO Pull type.  Used for input GPIOs to specify, pull-down, no-pull,
 * keeper, or pull-up.
 */
typedef enum
{
  GPIO_NP = 0,  /**<-- GPIO No Pull   >*/
  GPIO_PD,      /**<-- GPIO Pull Down >*/
  GPIO_KP,      /**<-- GPIO Keeper    >*/
  GPIO_PU,      /**<-- GPIO Pull Up   >*/

  GPIO_PULL_FORCE32 = 0x7FFFFFFF

}GPIOPullType;



/*
 * GPIODirType
 *
 * GPIO Direction type.  Specifies if a GPIO should be configured or input
 * or output.
 */
typedef enum
{
  GPIO_IN,   /**<-- GPIO Input   >*/
  GPIO_OUT,  /**<-- GPIO Output  >*/

  GPIO_DIR_FORCE32 = 0x7FFFFFFF

}GPIODirType;



/*
 * GPIOValuetype
 *
 * GPIO Value (HIGH/LOW) type.  Specifies whether a GPIO read is high or low
 * or whether an output pin should driver high or low.
 */
typedef enum
{
  GPIO_LOW,   /**<-- GPIO Low Value  >*/
  GPIO_HIGH,  /**<-- GPIO HighPath   >*/
  
  GPIO_VALUE_FORCE32 = 0x7FFFFFFF
  
}GPIOValueType;



/*
 * GPIOConfigType
 *
 * GPIO Configuration type.  This bit-field structure is used to configure a GPIO.
 */
typedef struct GPIOConfigType
{
  uint32 func    :  4;   /**<-- GPIO Function Select Value (uint32)  >*/
  uint32 dir     :  1;   /**<-- GPIO Direction Value (GPIODirType)   >*/
  uint32 pull    :  2;   /**<-- GPIO pull Value (GPIOPullType)       >*/
  uint32 drive   : 12;   /**<-- GPIO drive Value (GPIODirType)       >*/
  uint32 unused  : 13;   /**<-- Unused bits                          >*/
}GPIOConfigType;


/*
 * Externally available GPIO Key Type, also referenced as GPIO ID.
 */
typedef uint32 GPIOKeyType;


/*
 * GPIO Client Handle Type.  This handle is initialized by GPIO_Attach()
 * and passed into all GPIO functions.
 */
typedef struct
{
  uint32 DeviceId  : 32;
  int32  ClientId  : 32;

}GPIOClientHandleType;



#endif /* __GPIOTYPES_H__ */
