/*============================================================================
  FILE:         TsensLoaderLibNull.c

  OVERVIEW:     Implementation of TSENS supporting XBL loader.

  DEPENDENCIES: None

                Copyright (c) 2014-2018 Qualcomm Technologies, Inc.
                All Rights Reserved.
                Qualcomm Technologies Proprietary and Confidential.
============================================================================*/
/*============================================================================
  EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.  Please
  use ISO format for dates.

  when        who  what, where, why
  ----------  ---  -----------------------------------------------------------
  2018-02-27  vk   Initial revision.

============================================================================*/
/*----------------------------------------------------------------------------
 * Include Files
 * -------------------------------------------------------------------------*/
#include "TsensLib.h"

/*----------------------------------------------------------------------------
 * Preprocessor Definitions and Constants
 * -------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 * Type Declarations
 * -------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 * Static Function Declarations
 * -------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 * Global Data Definitions
 * -------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 * Static Variable Definitions
 * -------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 * Externalized Function Definitions
 * -------------------------------------------------------------------------*/
TsensResultType Tsens_Init(void)
{
  return 0;
}

TsensResultType Tsens_GetNumSensors(uint32 *puNumSensors)
{
  return 0;
}
TsensResultType Tsens_GetTemp(uint32 uSensor, int32 *pnDeciDegC)
{
  return 0;
}
TsensResultType Tsens_GetMaxTemp(int32 *pnMaxDeciDegC)
{
  return 0;
}
