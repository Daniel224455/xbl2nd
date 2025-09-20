#ifndef __GPIOIFACE_H__
#define __GPIOIFACE_H__
/*
============================================================================

                 G P I O   I N T E R F A C E   P U B L I C
                             H E A D E R   F I L E

FILE:         GPIOIFace.h

DESCRIPTION:  
              This file contains definitions for data used in the device
              configuration as well as within the TLMM/GPIO driver.

============================================================================

                             Edit History


when       who     what, where, why
--------   ---     ---------------------------------------------------------
04/24/15   dcf     Latest updates for platform IO detection.
07/30/10   dcf     Created the initial revision. 
 
============================================================================
             Copyright (c) 2010-2017 QUALCOMM Technologies, Incorporated.
                    All Rights Reserved.
                  QUALCOMM Proprietary/GTDR
============================================================================
*/

/*==========================================================================

                     INCLUDE FILES FOR THIS MODULE

==========================================================================*/

#include "com_dtypes.h"
#include "PlatformInfoDefs.h"


/*==========================================================================

                     DEFINITIONS FOR THIS MODULE

==========================================================================*/

/*
 * Definitions used by device configurations and driver software to capture
 * the chip information.
*/

#define GPIO_HW_VERSION(MinMajor, MinMinor, MaxMajor, MaxMinor) \
                    (((MinMajor) << 24) | ((MinMinor) << 16) | ((MaxMajor) << 8) | (MaxMinor))


#define GPIO_HW_MINMAJOR(val) (((val) & 0xFF000000) >> 24)
#define GPIO_HW_MINMINOR(val) (((val) & 0x00FF0000) >> 16)  
#define GPIO_HW_MAXMAJOR(val) (((val) & 0x0000FF00) >> 8)
#define GPIO_HW_MAXMINOR(val)  ((val) & 0x000000FF)

#define GPIO_MIN_TO_CHIPINFO(val) \
              (uint32)((((val) & 0xFF000000) >> 8) | (((val) & 0x00FF0000) >> 16))

#define GPIO_MAX_TO_CHIPINFO(val) \
              (uint32)((((val) & 0x0000FF00) << 8) | ((val) & 0x000000FF))  

/** 
  TLMM port selection enumerations.
    
  The DALGpioPortType is a port configuration enumeration for publicly available 
  TLMM ports, which abstract miscellaneous TLMM registers and are used with the 
  DalTlmm_SetPort API.
  
  @note Additions to this enumeration and associated functionality are added 
  on a needed basis, and not every enumeration is available or defined on every 
  target). 
    
  @sa
  DalTlmm_SetPort
*/

/* ====================================================================
   New Ports must be added to the end of this enumeration (just before 
   DAL_TLMM_NUM_PORTS).
=====================================================================*/

#define TLMM_RFFE_CTL               0

#define TLMM_NUM_PORTS              1


/*
 * GpioIdType
 *
 * GPIO Identifier for configuring a GPIO based on device config data for
 * a particular platform.
 *
 *  pin:  GPIO Pin Number.
 *  func: The function select associated with this GPIO.
 */
typedef struct
{  
   uint32 pin;
   uint32 func;

}GPIOIDType;


/*
 * TLMMPlatformMapType
 *
 * Platform identifier map used to query and load the appropriate
 * platform GPIO configurations at runtime.
 *
 *  Platform:   The PlatformInfo descriptor.
 *  szPlatName: The string name corresponding to the Platform.
 */
typedef struct
{
  DalPlatformInfoPlatformInfoType Platform;
  uint32                          nKVPS;
  uint32                          nKVPSType;
  const char*                     pszPlatform;
  const char*                     pszPlatConfigs;

}TLMMPlatformMapType;



#endif /* __GPIOIFACE_H__ */

