#ifndef __HALGPIO_H__
#define __HALGPIO_H__
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

#include "HALcomdef.h"
#include "GPIOTypes.h"
#include "GPIOIFace.h"
#include "HALtlmm.h"

/*==========================================================================

MACRO DEFINITIONS FOR MODULE

==========================================================================*/

/*
 * GPIO_CFGx register format for configuring GPIO settings.
 */
#define HAL_GPIO_CFG_TO_REG(func, dir, pull, drive) \
              (((pull)  & 0x3)        | \
              (((func)  & 0xF) << 2)  | \
              (((dir)   & 0x1) << 9)  | \
              (((drive) & 0x7) << 6))

/*
 * GPIO_LP_CFGx register format for getting/setting low-power configurations.
 */
#define HAL_GPIO_CFG_LP_TO_REG(func, dir, pull, drive, op) \
              (((pull)  & 0x3)        | \
              (((func)  & 0xF) << 2)  | \
              (((dir)   & 0x1) << 9)  | \
              (((drive) & 0x7) << 6)  | \
              (((op)    & 0x1) << 10) | \
                (0x1 << 11) )


/*
 * Accessor macros for retrieving GPIO settings.
 */
#define HAL_GPIO_GET_FUNC(reg)      (((reg) >> 2) & 0xF)
#define HAL_GPIO_GET_DIR(reg)       (((reg) >> 9) & 0x1)
#define HAL_GPIO_GET_PULL(reg)       ((reg) & 0x3)
#define HAL_GPIO_GET_DRIVE(reg)     (((reg) >> 6) & 0x7)

#define HAL_GPIO_LP_CFG_IS_SET(reg) (((reg) >> 11) & 0x1)
#define HAL_GPIO_GET_LP_OUTPUT(reg) (((reg) >> 10) & 0x1)


/*
 * Register offset bitmasks used to access TLMM HW.
 */
#define HAL_GPIO_CFG_EGPIO_ENABLE_BMSK        0x00001000
#define HAL_GPIO_CFG_EGPIO_PRESENT_BMSK       0x00000800
#define HAL_GPIO_CFG_HIHYS_ENABLE_BMSK        0x00000400
#define HAL_GPIO_CFG_OE_BMSK                  0x00000200
#define HAL_GPIO_CFG_DRV_STRENGTH_BMSK        0x000001C0
#define HAL_GPIO_CFG_FUNC_SEL_BMSK            0x0000003C
#define HAL_GPIO_CFG_DIRECTION_BMSK           0x00000003


/*==========================================================================

DATA STRUCTURE DEFINITIONS FOR MODULE

==========================================================================*/



/*==========================================================================

APPLICATION PROGRAMMERS INTERFACE PROTOTYPES

==========================================================================*/

boolean       HAL_gpio_Init              (HALgpioBlockDescType *pBlock);
void          HAL_gpio_ConfigPin         (HALgpioBlockDescType *pBlock, uint32 nOffs, uint32 nGPIO, GPIOConfigType Config);
uint32        HAL_gpio_GetTileIndex      (HALgpioBlockDescType *pBlock, uint32 nGPIO);
void          HAL_gpio_GetInactiveConfig (HALgpioBlockDescType *pBlock, uint32 nOffs, uint32 nGPIO, GPIOConfigType *pConfig, GPIOValueType *pValue);
void          HAL_gpio_SetInactiveConfig (HALgpioBlockDescType *pBlock, uint32 nOffs, uint32 nGPIO, GPIOConfigType Config, GPIOValueType Value);
GPIOValueType HAL_gpio_ReadPin           (HALgpioBlockDescType *pBlock, uint32 nOffs, uint32 nGPIO);
void          HAL_gpio_WritePin          (HALgpioBlockDescType *pBlock, uint32 nOffs, uint32 nGPIO, GPIOValueType eValue);
GPIOValueType HAL_gpio_ReadPinOutput     (HALgpioBlockDescType *pBlock, uint32 nOffs, uint32 nGPIO);
void          HAL_gpio_GetPinConfig      (HALgpioBlockDescType *pBlock, uint32 nOffs, uint32 nGPIO, GPIOConfigType* Config);
uint32        HAL_gpio_DriveCvtToLogical (uint32 nHWVal);
uint32        HAL_gpio_DriveCvtToHW      (uint32 nLogicalVal);


/*===============================================================================

INTERFACES DOCUMENTATION - See source files for parameter descriptions.

=================================================================================

FUNCTION
  void HAL_gpio_ConfigPin

DESCRIPTION
  Configures a GPIO to a required operational state.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=================================================================================

FUNCTION
  void HAL_gpio_GetTileIndex

DESCRIPTION
  Retrieves the tile index of a particular GPIO from the descriptor block.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=================================================================================

FUNCTION
  void HAL_gpio_GetInactiveConfig

DESCRIPTION
  Retrieves the saved inactive (low-power) GPIO pin configuration.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=================================================================================

FUNCTION
  void HAL_gpio_SetInactiveConfig

DESCRIPTION
  Changes the saved inactive (low-power) configuration of a GPIO pin.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=================================================================================

FUNCTION
  void HAL_gpio_ReadPin

DESCRIPTION
  Reads the value (high/low) of a GPIO pin.

DEPENDENCIES
  None.

RETURN VALUE
  GPIO HIGH or GPIO_LOW.

SIDE EFFECTS
  None.

=================================================================================

FUNCTION
  HAL_gpio_WritePin

DESCRIPTION
  Drives the value (high/low) of a generic IO with output enable set.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=================================================================================

FUNCTION
  GPIOValueType HAL_gpio_ReadPinOutput

DESCRIPTION
  Reads the value of the GPIO output value

DEPENDENCIES
  None.  See also HAL_gpio_WritePin.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=================================================================================

FUNCTION
  void HAL_gpio_GetPinConfig

DESCRIPTION
  Returns the current configuration of a particular GPIO pin.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=================================================================================

FUNCTION
  uint32 HAL_gpio_DriveCvtToLogical

DESCRIPTION
  Returns the value of a conversion from HW register value to logical value for
  drive strength.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=================================================================================

FUNCTION
  uint32 HAL_gpio_DriveCvtToHW

DESCRIPTION
  Converts a logical drive strength value to a register value.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=================================================================================

===============================================================================*/

#endif /* __HALGPIO_H__ */
