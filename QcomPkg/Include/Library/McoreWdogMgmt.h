/** @file
 *
 *  Interface for Multi core Watch Dog Mgmt in UEFI
 *  
 * Copyright (c) 2018 Qualcomm Technologies, Inc. All rights reserved.
 * 
**/

/*=============================================================================
                              EDIT HISTORY


 when       who     what, where, why
 --------   ---     -----------------------------------------------------------
 09/24/18   yg      Initial version

=============================================================================*/

#ifndef __MCORE_WDOG_MGMT_LIB_H__
#define __MCORE_WDOG_MGMT_LIB_H__

#include <Uefi.h>

#define DEFAULT_WDOG_BITE_TIMEOUT_MSEC        24000

VOID WDogMonitorInit (VOID);
EFI_STATUS WDogMonitorStart (VOID);
VOID WDogMonitorStop (VOID);
EFI_STATUS WDogMonitorSetBiteTimeout (UINT32 WDogTimeoutms);
VOID WDogMonitorForcePet (VOID);
VOID WDogMonitorForceBite (VOID);
EFI_STATUS WDogMonitorSetPetTimerPeriod (UINT32 TimerPeriodms);

#endif // __MCORE_WDOG_MGMT_LIB_H__

