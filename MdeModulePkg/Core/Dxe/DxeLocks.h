/** @file
  Dxe Lock lib
   
  Copyright (c) 2018 Qualcomm Technologies Inc. All rights reserved.
  
**/

/*=============================================================================
                              EDIT HISTORY


 when       who     what, where, why
 --------   ---     -----------------------------------------------------------
 03/01/18   yg      Initial version

=============================================================================*/

#include <Uefi.h>

#include "Library/LockLib.h"

#ifndef ___DXE_LOCK_LIB_H__
#define ___DXE_LOCK_LIB_H__
/* Services with API's utilizing Locks */
extern EFI_BOOT_SERVICES gBootServicesLk;
extern EFI_DXE_SERVICES gDxeServicesLk;

VOID DxeLockInit (VOID);
VOID DxeLock (VOID);
VOID DxeUnlock (VOID);

#endif  /* ___DXE_LOCK_LIB_H__ */
