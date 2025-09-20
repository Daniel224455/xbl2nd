/*===========================================================================

FILE:      GPIOSvcSync.c

DESCRIPTION:
  This file contains the GPIO abstraction of the synchronization objects.
  It is expected to be image-specific where implementations differ across
  images.

PUBLIC CLASSES:
  GPIO_SvcSynInit
  GPIO_SyncEnter
  GPIO_SyncExit

===========================================================================
Copyright (c) 2018 QUALCOMM Technologies Incorporated.
All Rights Reserved.
QUALCOMM Proprietary/GTDR
========================================================================================================*/

/*========================================================================================================

  EDIT HISTORY FOR FILE

  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.


  when       who     what, where, why
  --------   ---     -------------------------------------------------------------------------------------
  03/02/17   dcf     Created.
  ======================================================================================================*/

/*=======================================================================================================

  INCLUDE FILES FOR MODULE

  =====================================================================================================*/

//#include <DALStdErr.h>
//#include <DALFramework.h>
//#include <DALSys.h>
#include "GPIOSvcSync.h"
#include "HALhwio.h"


/*=======================================================================================================

  INTERNAL DATA STRUCTURES

  =====================================================================================================*/


#define CHIP_MAJOR_BMSK  0xFF00
#define CHIP_MINOR_BMSK  0x00FF

/*
 * Boolean flag for specifying initialization.
 */
static boolean bInitialized = FALSE;

extern uint32 nHWRevisionAddr;

/*=======================================================================================================

                            APPLICATION PROGRAMMERS INTERFACE FUNCTIONS

  =====================================================================================================*/


/*========================================================================================================

 FUNCTION    GPIO_SvcInitChipVersion

 DESCRIPTION  Initializes the internal chip revision variable.

========================================================================================================*/

boolean GPIO_SvcInitChipVersion(uint32 *pChipVersion)
{
  uint32 nVersionVal = 0;
  
  if ( (pChipVersion != NULL) && (nHWRevisionAddr != 0) )
  {
    if ( *pChipVersion == 0 )
    {
      nVersionVal = in_dword(nHWRevisionAddr);
      *pChipVersion = (uint32)(((nVersionVal & CHIP_MAJOR_BMSK) << 8) | (nVersionVal & CHIP_MINOR_BMSK));        
    }
    return TRUE;
  }
  
  return FALSE;
  
} /* GPIO_SvcInitChipVersion */
  

/*========================================================================================================

 FUNCTION    GPIO_SvcSynInit

 DESCRIPTION  See GPIOSrvSync.h

========================================================================================================*/

boolean GPIO_SvcSynInit(void)
{
  if ( bInitialized == FALSE )
  {
      bInitialized = TRUE;
  }
  return bInitialized;
  
} /* GPIO_SvcSynInit */


/*========================================================================================================

 FUNCTION    GPIO_SyncEnter

 DESCRIPTION  See GPIOSrvSync.h

========================================================================================================*/

void GPIO_SyncEnter(void)
{
  if ( bInitialized != TRUE )
  {
    GPIO_SvcSynInit();
  }
 // (void)DALSYS_SyncEnter(TLMMSync.hTlmmSync);
 
} /* GPIO_SyncEnter */


/*========================================================================================================

 FUNCTION    GPIO_SyncEnter

 DESCRIPTION  See GPIOSrvSync.h

========================================================================================================*/

void GPIO_SyncExit(void)
{
  if ( bInitialized != TRUE )
  {
    GPIO_SvcSynInit();
  }
 // (void)DALSYS_SyncLeave(TLMMSync.hTlmmSync);
 
} /* GPIO_SyncExit */

