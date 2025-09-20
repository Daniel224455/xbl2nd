#ifndef __HALTLMM_H__
#define __HALTLMM_H__
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


/*==========================================================================

MACRO DEFINITIONS FOR MODULE

==========================================================================*/


/*==========================================================================

DATA STRUCTURE DEFINITIONS FOR MODULE

==========================================================================*/

/*
 * Forward references to the API interface.
 */
typedef struct HAL_tlmm_GpioCoreControlType HAL_tlmm_GpioCoreControlType;
typedef struct HAL_tlmm_GpioExtControlType HAL_tlmm_GpioExtControlType;


typedef struct
{
  GPIODeviceIdType              DeviceId;      /* Type of TLMM HW */
  uint32                        nBaseAddr;     /* Top level TLMM base address. */
  uint32                        nPinRegWidth;  /* The amount of space between each GPIO pin configuration register. */
  uint32                        nNumTiles;     /* The total number of required TLMM HW tiles. */
  uint32                        aTiles[5];     /* Array of required of TLMM HW tiles. */
  uint32                        nMinPinNumber; /* nMinGPIO */
  uint32                        nMaxPinNumber; /* nMaxGPIO */
  uint32                        nHWVersion;    /* The supported HW version for this descriptor. */
  HAL_tlmm_GpioCoreControlType *pmCoreControl; /* Pointer to block-specific Core APIs. */
  HAL_tlmm_GpioExtControlType  *pmExtControl;  /* Pointer to block-specific Extended APIs. */
  void                         *pCustom;       /* Pointer to any required custom data. */

}HALgpioBlockDescType;


/*
 * HAL_tlmm_GpioCoreControlType
 *
 * Set of function pointers used to control a GPIO.
 *
 * Init              - Peforms any HW initialization required.
 * ConfigPin         - Configure a GPIO pin.
 * ReadPin           - Read the GPIO pin value.
 * WritePin          - Write a GPIO pin value.
 * GetTileIndex      - Finds the tile index used for this GPIO.
 * ReadPinOutput     - Read the SW programmed value of the pin output.
 * GetPinConfig      - Get the current pin configuration.
 */
struct HAL_tlmm_GpioCoreControlType
{
  boolean       (*Init)               ( HALgpioBlockDescType *pBlock );
  void          (*ConfigPin)          ( HALgpioBlockDescType *pBlock, uint32 nOffs, uint32 nGPIO, GPIOConfigType Config );
  GPIOValueType (*ReadPin)            ( HALgpioBlockDescType *pBlock, uint32 nOffs, uint32 nGPIO );
  void          (*WritePin)           ( HALgpioBlockDescType *pBlock, uint32 nOffs, uint32 nGPIO, GPIOValueType nValue );
  uint32        (*GetTileIndex)       ( HALgpioBlockDescType *pBlock, uint32 nGPIO );
  GPIOValueType (*ReadPinOutput)      ( HALgpioBlockDescType *pBlock, uint32 nOffs, uint32 nGPIO );
  void          (*GetPinConfig)       ( HALgpioBlockDescType *pBlock, uint32 nOffs, uint32 nGPIO, GPIOConfigType* pConfig );
};


/*
 * HAL_tlmm_GpioExtControlType
 *
 * Set of function pointers used to control a GPIO.
 *
 * GetInactiveConfig - Gets the low-power settting.
 * SetInactiveConfig - Sets the low-power settting.
 */
struct HAL_tlmm_GpioExtControlType
{
  void    (*GetInactiveConfig)  ( HALgpioBlockDescType *pBlock, uint32 nOffs, uint32 nGPIO, GPIOConfigType *pConfig, GPIOValueType *pValue );
  void    (*SetInactiveConfig)  ( HALgpioBlockDescType *pBlock, uint32 nOffs, uint32 nGPIO, GPIOConfigType pConfig, GPIOValueType Value );
};




/*==========================================================================

APPLICATION PROGRAMMERS INTERFACE PROTOTYPES

==========================================================================*/
boolean       HAL_tlmm_Init              (HALgpioBlockDescType *pBlock);
void          HAL_tlmm_ConfigPin         (HALgpioBlockDescType *pBlock, uint32 nOffs, uint32 nGPIO, GPIOConfigType Config);
uint32        HAL_tlmm_GetTileIndex      (HALgpioBlockDescType *pBlock, uint32 nGPIO);
void          HAL_tlmm_GetInactiveConfig (HALgpioBlockDescType *pBlock, uint32 nOffs, uint32 nGPIO, GPIOConfigType *pConfig, GPIOValueType *pValue);
void          HAL_tlmm_SetInactiveConfig (HALgpioBlockDescType *pBlock, uint32 nOffs, uint32 nGPIO, GPIOConfigType Config, GPIOValueType Value);
GPIOValueType HAL_tlmm_ReadPin           (HALgpioBlockDescType *pBlock, uint32 nOffs, uint32 nGPIO);
void          HAL_tlmm_WritePin          (HALgpioBlockDescType *pBlock, uint32 nOffs, uint32 nGPIO, GPIOValueType eValue);
GPIOValueType HAL_tlmm_ReadPinOutput     (HALgpioBlockDescType *pBlock, uint32 nOffs, uint32 nGPIO);
void          HAL_tlmm_GetPinConfig      (HALgpioBlockDescType *pBlock, uint32 nOffs, uint32 nGPIO, GPIOConfigType* Config);
uint32        HAL_tlmm_DriveCvtToLogical (HALgpioBlockDescType *pBlock, uint32 nHWVal);
uint32        HAL_tlmm_DriveCvtToHW      (HALgpioBlockDescType *pBlock, uint32 nLogicalVal);


/*===============================================================================

INTERFACES DOCUMENTATION - See source files for parameter descriptions.

=================================================================================

FUNCTION
  void HAL_tlmm_ConfigPin

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
  void HAL_tlmm_GetTileIndex

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
  void HAL_tlmm_GetInactiveConfig

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
  void HAL_tlmm_SetInactiveConfig

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
  void HAL_tlmm_ReadPin

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
  HAL_tlmm_WritePin

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
  void HAL_tlmm_ReadPinOutput

DESCRIPTION
  Reads the value of the GPIO output value

DEPENDENCIES
  None.  See also HAL_tlmm_WritePin.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=================================================================================

FUNCTION
  void HAL_tlmm_GetPinConfig

DESCRIPTION
  Returns the current configuration of a particular GPIO pin.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===============================================================================*/

#endif /* __HALTLMM_H__ */
