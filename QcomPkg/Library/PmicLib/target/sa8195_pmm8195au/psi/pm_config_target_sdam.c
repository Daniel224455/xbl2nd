/*! \file  pm_config_target_sdam.c 
 *   
 *  \brief  File Contains the PMIC Set Mode Driver Implementation 
 *  \details Set Mode Driver implementation is responsible for setting and getting 
 *  all mode settings such as Register values, memory values, etc. 
 *   
 *    PMIC code generation Version: 1.0.0.0 
 *    Date: 11/14/2018 
 *    PMIC PSI Version: SM8150-SBL-b0x00_v0x66 - Approved 
 *    PBS RAM Version: PM8150_RAM_b0x00_v0x2C 
 *    PBS RAM Version: PM8150_2p0_RAM_b0x02_v0x0B 
 *    PBS RAM Version: PM8150_3p0_RAM_b0x06_v0x1A 
 *    PBS RAM Version: PM8150B_RAM_b0x02_v0x18 
 *    PBS RAM Version: PM8009_RAM_b0x02_v0x03 
 *    This file contains code for Target specific settings and modes. 
 *  
 *  &copy; Copyright 2018 Qualcomm Technologies Incorporated, All Rights Reserved
 */

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This document is created by a code generator, therefore this section will
  not contain comments describing changes made to the module.


when       who     what, where, why
--------   ---     ---------------------------------------------------------- 

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES 

===========================================================================*/

#include "pm_target_information.h"
#include "pm_config_sbl.h"

uint8
pm_sdam_seq [ ][SDAM_DATA_SIZE_MAX] =
{
  //Swire Map 0
  {0},
  //Swire Map 1
  {0},
  //Swire Map 2
  {0},
  //Custom Sdam Load
  {0},
};