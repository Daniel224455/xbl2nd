#ifndef __DALITLMM_H__
#define __DALITLMM_H__
/**
  @file DDITlmm.h 
  
  Public interface include file for accessing the TLMM DAL device.
  
  The DDITlmm.h file is the public API interface to the Top Level Mode Multiplexor 
  (TLMM) Device Access Library (DAL).
*/
/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.


when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/19/14   na      Remove configuration requests for remote processor (pre A-family)
07/05/10   sw      (Tech Pubs) edited Doxygen comments and markup; modified
                   Doxygen markup to meet Living Docs standards.
06/17/09   dcf     Fix for embedded output.
02/23/09   dcf     Added comments and versioning.
=============================================================================
Copyright (c) 2010 Qualcomm Technologies, Incorporated.  All Rights Reserved.  
QUALCOMM Proprietary and Confidential. 
===========================================================================*/



/*==========================================================================

                     INCLUDE FILES FOR MODULE

==========================================================================*/

#include "DalDevice.h"
#include "DALDeviceId.h"
#include "DALStdErr.h"
#include "com_dtypes.h"
#include "TlmmDefs.h"


/*==========================================================================
  Function pointer table for TLMM glue code.  This is part of the DAL 
  framework glue code.
==========================================================================*/
/** @cond */
typedef struct DalTlmm DalTlmm;
struct DalTlmm
{
   struct DalDevice DalDevice;
   DALResult (*ConfigGpio)(DalDeviceHandle * _h, DALGpioSignalType gpio_config, DALGpioEnableType enable);
   DALResult (*ConfigGpioGroup)(DalDeviceHandle * _h, DALGpioEnableType enable, DALGpioSignalType* gpio_group, uint32 size);
   DALResult (*GetGpioNumber)(DalDeviceHandle* _h, DALGpioSignalType gpio_config, uint32 *  number);
   DALResult (*GpioIn)(DalDeviceHandle * _h, DALGpioSignalType gpio_config, DALGpioValueType*  value);
   DALResult (*GpioOut)(DalDeviceHandle * _h, DALGpioSignalType gpio_config, DALGpioValueType value);
   DALResult (*GpioOutGroup)(DalDeviceHandle * _h, DALGpioSignalType* gpio_config, uint32 size, DALGpioValueType value);
   DALResult (*SetInactiveConfig)(DalDeviceHandle * _h, uint32 gpio_number, DALGpioSignalType config);
   DALResult (*GetOutput)(DalDeviceHandle * _h, uint32 gpio_number, DALGpioValueType* value);
};


typedef struct DalTlmmHandle DalTlmmHandle; 
struct DalTlmmHandle 
{
   uint32 dwDalHandleId;
   const DalTlmm * pVtbl;
   void * pClientCtxt;
};

#define DAL_TlmmDeviceAttach(DevId,hDalDevice)\
        DAL_DeviceAttachEx(NULL,DevId,DALTLMM_INTERFACE_VERSION,hDalDevice)

/** @endcond */



/*==========================================================================

                     APPLICATION PROGRAMMER'S INTERFACE

==========================================================================*/
/** 
  @addtogroup driver_apis
  @{
*/


/* ============================================================================
**  Function : DalTlmm_ConfigGpio
** ============================================================================*/
/**
  Configures a GPIO.
 
  This function configures a general purpose I/O or a hardware function 
  multiplexed on a particular GPIO pin. The configuration is based in part on 
  the value for the enable parameter.
  - If this parameter is set to DAL_TLMM_GPIO_ENABLE, the configuration 
    associated with the gpio_config parameter is programmed to hardware.
  - If DAL_TLMM_GPIO_DISABLE is specified, the GPIO number is extracted from 
    the gpio_config parameter and used to index an internal table to retrieve 
    the inactive (or low-power) configuration associated with the particular GPIO.
 
  @param *_h         [in] -- Handle to the TLMM DAL device.
  @param gpio_config [in] -- Configuration to use whenn programming a GPIO.
  @param enable      [in] -- DAL_TLMM_GPIO_ENABLE or DAL_TLMM_GPIO_DISABLE.
    
  @return
  DAL_SUCCESS -- GPIO was successfully configured as requested.\n
  DAL_ERROR -- Either an invalid GPIO number in the configuration or ownership 
  cannot be obtained from a secure root.
    
  @dependencies
  None.

  @sideeffects
  Ownership for the specified GPIO can be changed, if allowed 
  and required. This may be necessary to provide visibility to 
  generic I/O on certain multiprocessor systems. 
    
  @sa
  DAL_GPIO_CFG, DALGpioSignalType
*/ 

static __inline DALResult
DalTlmm_ConfigGpio(DalDeviceHandle * _h, DALGpioSignalType gpio_config, DALGpioEnableType enable)
{
   return ((DalTlmmHandle *)_h)->pVtbl->ConfigGpio( _h, gpio_config, enable);
}


/* ============================================================================
**  Function : DalTlmm_ConfigGpioGroup
** ============================================================================*/
/**
  Configures a group of GPIOs.
 
  This function configures a group of GPIOs or a hardware function multiplexed 
  on a particular GPIO pin.
 
  @param *_h         [in] -- Handle to the TLMM DAL device.
  @param enable      [in] -- DAL_TLMM_GPIO_ENABLE or DAL_TLMM_GPIO_DISABLE.
  @param *gpio_group [in] -- Pointer to an array of GPIO configurations.
  @param size        [in] -- Number of GPIO configurations in gpio_group.
    
  @return
  If 1, DAL_SUCCESS -- All configurations were valid and any remote requests 
                       were successful.\n
  DAL_ERROR -- Invalid configurations; unsuccessful requests.
    
  @dependencies
  None.

  @sa
  DalTlmm_ConfigGpio
*/ 

static __inline DALResult
DalTlmm_ConfigGpioGroup
(
  DalDeviceHandle * _h, 
  DALGpioEnableType  enable,
  DALGpioSignalType* gpio_group, 
  uint32             size
)
{
   return 
     ((DalTlmmHandle *)_h)->pVtbl->ConfigGpioGroup( _h, enable, gpio_group, size);
}


/* ============================================================================
**  Function : DalTlmm_GetGpioNumber
** ============================================================================*/
/**
  Retrieves a GPIO number.
 
  Retrieves the GPIO number from a specified configuration. This function does 
  the same thing as the macro DAL_GPIO_NUMBER.
 
  @param *_h           [in] -- Handle to the TLMM DAL device.
  @param gpio_config   [in] -- Configuration from which to get the GPIO number.
  @param *gpio_number [out] -- GPIO number to return to the caller.
    
  @return
  Returns DAL_SUCCESS even if the GPIO number is invalid for the the current 
  target.
      
  @dependencies
  None.

  @sa
  DAL_GPIO_NUMBER
*/

static __inline DALResult
DalTlmm_GetGpioNumber
(
  DalDeviceHandle   *_h, 
  DALGpioSignalType gpio_config, 
  uint32            *gpio_number
)
{
   return ((DalTlmmHandle *)_h)->pVtbl->GetGpioNumber( _h, 
                                                     (DALGpioSignalType)gpio_config, 
                                                     gpio_number);
}


/* ============================================================================
**  Function : DalTlmm_GpioIn
** ============================================================================*/
/**
  Reads a GPIO value.
 
  Reads the value of a GPIO that has been configured as an INPUT.
 
  @param *_h         [in] -- Handle to the TLMM DAL device.
  @param gpio_config [in] -- Configuration from which to get the GPIO number.
  @param *value     [out] -- Value to be returned to the caller.
    
  @return
  DAL_SUCCESS -- GPIO number is valid.\n
  DAL_ERROR -- Invalid number.
    
  @dependencies
  None.
*/

static __inline DALResult
DalTlmm_GpioIn
(
  DalDeviceHandle *_h, 
  DALGpioSignalType gpio_config, 
  DALGpioValueType*value
)
{
   return ((DalTlmmHandle *)_h)->pVtbl->GpioIn( _h, gpio_config, value);
}


/* ============================================================================
**  Function : DalTlmm_GpioOut
** ============================================================================*/
/**
  Writes a GPIO value.
 
  Writes a value (high or low) to a GPIO that is configured as an OUTPUT, has a 
  function select set to 0, and has no pull. This type of configuration is known 
  as general purpose I/O or bit-banged I/O.
 
  @param *_h         [in] -- Handle to the TLMM DAL device.
  @param gpio_config [in] -- Configuration from which to get the GPIO number.
  @param value       [in] -- Value to write (DAL_GPIO_VALUE_HIGH or 
                             DAL_GPIO_VALUE_LOW).
    
  @return
  DAL_SUCCESS -- GPIO is valid.\n
  DAL_ERROR -- Invalid GPIO.

  @dependencies
  None.
    
  @sa
  DALGpioValueType
*/

static __inline DALResult
DalTlmm_GpioOut
(
  DalDeviceHandle  *_h, 
  DALGpioSignalType gpio_config, 
  DALGpioValueType  value
)
{
   return ((DalTlmmHandle *)_h)->pVtbl->GpioOut( _h, gpio_config, value);
}


/* ============================================================================
**  Function : DalTlmm_GpioOutGroup
** ============================================================================*/
/**
  Writes an output value to a group of GPIOs.
 
  Writes a value (high or low) to an array of GPIOs that are configured as OUTPUT,
  have function selects set to 0, and have pull set to no-pull.
 
  @param *_h           [in] -- Handle to the TLMM DAL device.
  @param *gpio_group   [in] -- Array of GPIO configurations to which to write an
                               output value.
  @param size          [in] -- Number of elements in the gpio_group array.
  @param value         [in] -- Value to write (DAL_GPIO_VALUE_HIGH or
                               DAL_GPIO_VALUE_LOW).
    
  @return
  DAL_SUCCESS -- GPIO numbers are valid.\n
  DAL_ERROR -- Invalid numbers.

  @dependencies
  None.
    
  @sa
  DalTlmm_GpioOut, DALGpioValueType
*/

static __inline DALResult
DalTlmm_GpioOutGroup
(
  DalDeviceHandle   *_h, 
  DALGpioSignalType *gpio_group, 
  uint32            size, 
  DALGpioValueType  value
)
{
   return ((DalTlmmHandle *)_h)->pVtbl->GpioOutGroup( _h, 
                                                      gpio_group, 
                                                      size, value);
}


/* ============================================================================
**  Function : DalTlmm_SetInactiveConfig
** ============================================================================*/
/**
  Sets the inactive configuration of a GPIO.
 
  This function is used to modify the inactive or low-power configuration of a 
  GPIO. It modifies the configuration that is programmed when 
  DAL_TLMM_GPIO_DISABLE is passed in to the configuration functions.

  This function was originally provided for power debugging, but it can be used 
  elsewhere as necessary. It does not change the current configuration of a GPIO.
 
  @param *_h          [in] -- Handle to the TLMM DAL device.
  @param gpio_number  [in] -- GPIO number for which to retrieve the configuration.
  @param gpio_config [out] -- Configuration to use to replace the low-power
                              configuration in the table.
    
  @return
  DAL_SUCCESS -- GPIO number is valid.\n
  DAL_ERROR -- Invalid number.

  @dependencies
  None.
    
  @sa
  DalTlmm_GetInactiveConfig
*/

static __inline DALResult
DalTlmm_SetInactiveConfig
(
  DalDeviceHandle   *_h, 
  uint32            gpio_number, 
  DALGpioSignalType gpio_config
)
{
   return ((DalTlmmHandle *)_h)->pVtbl->SetInactiveConfig( _h, 
                                                           gpio_number, 
                                                           gpio_config);
}


/* ============================================================================
**  Function : DalTlmm_GetOutput
** ============================================================================*/
/**
  Retrieves the output value of a GPIO.
 
  This function retrieves the current output value of a GPIO that has been 
  configured as a generic I/O with output enabled.
 
  @param *_h         [in] -- Handle to the TLMM DAL device.
  @param gpio_number [in] -- GPIO number to use to obtain the output value.
  @param *value     [out] -- Current value of the output GPIO (DAL_GPIO_VALUE_LOW 
                             or DAL_GPIO_VALUE_HIGH).
    
  @return
  DAL_SUCCESS -- GPIO number is valid.\n
  DAL_ERROR -- Invalid number.
    
  @dependencies
  None.
*/

static __inline DALResult
DalTlmm_GetOutput(
  DalDeviceHandle   *_h, uint32 gpio_number, DALGpioValueType* value)
{
   return ((DalTlmmHandle *)_h)->pVtbl->GetOutput( _h, gpio_number, value);
}

/** 
  @}
*/ /* end_group driver_apis */


DALResult Tlmm_ConfigGpioGroup
(
  DALGpioEnableType  enable,
  DALGpioSignalType* gpio_group, 
  uint32             size
);


#endif

