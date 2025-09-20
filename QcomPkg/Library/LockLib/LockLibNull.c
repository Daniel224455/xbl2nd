/** @file
  Stub Lock lib Implementation
   
  Copyright (c) 2017 Qualcomm Technologies Inc. All rights reserved.
  
**/

/*=============================================================================
                              EDIT HISTORY


 when       who     what, where, why
 --------   ---     -----------------------------------------------------------
 07/24/17   yg      Initial version

=============================================================================*/

#include <Uefi.h>

#include <Library/BaseLib.h>
#include <Library/LockLib.h>

VOID* 
InitLock (OPTIONAL CONST CHAR8 *LockStrID)
{
  return NULL;
}

VOID* 
InitRecursiveLock (OPTIONAL CONST CHAR8 *LockStrID)
{
  return NULL;
}

VOID 
Lock (VOID* Handle)
{
}

VOID 
UnLock (VOID* Handle)
{
}

BOOLEAN 
InitLockLib (VOID)
{
  return FALSE;
}

