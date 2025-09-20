/** @file
  Lock lib to make API's re-entrant
   
  Copyright (c) 2017 - 2018 Qualcomm Technologies Inc. All rights reserved.
  
**/

/*=============================================================================
                              EDIT HISTORY


 when       who     what, where, why
 --------   ---     -----------------------------------------------------------
 07/24/17   yg      Initial version

=============================================================================*/

#include <Uefi.h>

#include <Library/BaseLib.h>

#ifndef ___LOCK_LIB_H__
#define ___LOCK_LIB_H__

/* Initialize lock lib, not a must, but the modules that would like to see
 * if the locks are indeed functional can call this API to see if the framework
 * is available.
 *
 * Returns TRUE  : If lock framework is available
 *         FALSE : If the framework is not available, all API's become stubs */
BOOLEAN InitLockLib (VOID);

/*  NOTE for LockID:
 *
 *  LockStrID of 0 results always into a new private lock. ie lib linked to multiple
 *  images will have NO relation between them, they will have their own each
 *  lock. 0 should be used only for the case where the lock is linked to a lib
 *  that's already guaranteed to have just 1 instance in the system.
 *
 *  LockStrID of non 0 value results into fetching the existing lock in the pool with
 *  the same ID. If a lock with that ID doesn't exist, it will be created, and
 *  any subsequent lock init with same ID will return that lock. This way of
 *  non 0 ID can be used in Lib's that get linked to multiple drivers, thus have
 *  multiple copies of libs in the system, but need a common lock for a shared
 *  resource protection.
 *
 * */

/* Use for any non recursive functions. If the thread tries to lock when it
 * already has locked, would result into crash */
VOID* InitLock (OPTIONAL CONST CHAR8 *LockStrID);

/* Use for any recursive functions. Lock can be locked multiple times by the
 * same thread even if its already locked. Should make sure that lock/unlock
 * nesting is even and count should match. Otherwise the underlying lock is
 * never released. Its NOT ok to Unlock more times than Lock, this would 
 * result into crash */
VOID* InitRecursiveLock (OPTIONAL CONST CHAR8 *LockStrID);

VOID  Lock (VOID* Handle);
VOID  UnLock (VOID* Handle);

#endif  /* ___LOCK_LIB_H__ */
