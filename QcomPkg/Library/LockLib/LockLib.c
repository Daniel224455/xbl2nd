/** @file
  Lock lib to make API's re-entrant
   
  Copyright (c) 2017 Qualcomm Technologies Inc. All rights reserved.
  
**/

/*=============================================================================
                              EDIT HISTORY


 when       who     what, where, why
 --------   ---     -----------------------------------------------------------
 07/24/17   yg      Initial version

=============================================================================*/

#include <Uefi.h>
#include <PiDxe.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/HobLib.h>
#include <Library/LockLib.h>
#include <Library/KernelLib.h>

#define FLAG_RECURSIVE_LOCK   (1  << 0)

typedef struct 
{
   UINT32     Sig;
   UINT32     Flags;
   Mutex     *MutexPtr;
   UINTN      NestLevel;
   UINT32     MaxNest;
   UINT32     InactivityCnt;
} 
LockType;

extern BOOLEAN KernelInterfaceInit (VOID);

BOOLEAN 
InitLockLib (VOID)
{
  return KernelInterfaceInit ();
}

VOID* 
InitLock (OPTIONAL CONST CHAR8 *LockStrID)
{
  LockType *LockPtr;
  UINT64 LockID;
  Mutex* MLockPtr;
  CONST CHAR8* Sptr = LockStrID;

  LockID = 0;

  if (LockStrID)
  {
    UINTN i;
    for (i=0; i<8; ++i)
    {
      LockID = LockID | (((UINT64)(*Sptr)) << (i * 8));
      if (*Sptr == 0)
        break;
      ++Sptr;
    }
  }

  MLockPtr = MutexInit (LockID);

  if (MLockPtr == NULL)
    return NULL;

  LockPtr = AllocatePool (sizeof(*LockPtr));

  if (LockPtr == 0)
    return LockPtr;

  SetMem (LockPtr, sizeof(*LockPtr), 0);

  LockPtr->Sig = 'L' | ('o' << 8) | ('c' << 16) | ('k' << 24);

  LockPtr->MutexPtr = MLockPtr;

  return LockPtr;
}

VOID* 
InitRecursiveLock (OPTIONAL CONST CHAR8 *LockStrID)
{
  LockType *LockPtr;

  LockPtr = InitLock (LockStrID);

  if (LockPtr)
    LockPtr->Flags = LockPtr->Flags | FLAG_RECURSIVE_LOCK;

  return LockPtr;
}

VOID 
Lock (VOID* Handle)
{
  LockType *LockPtr = (LockType *)Handle;

  if (Handle == 0)
    return;

  if ((LockPtr->Flags & FLAG_RECURSIVE_LOCK) == 0)
  {
    MutexAcquire (LockPtr->MutexPtr);
    return;
  }

  if (IsMutexHeld(LockPtr->MutexPtr))
  {
    if (LockPtr->NestLevel == 0)
      LockPtr->NestLevel = 1;

    LockPtr->NestLevel++;

    if (LockPtr->MaxNest < LockPtr->NestLevel)
      LockPtr->MaxNest = LockPtr->NestLevel;

    if (LockPtr->NestLevel > 1)
    {
      ++LockPtr->InactivityCnt;
      return;
    }
  }

  MutexAcquire (LockPtr->MutexPtr);
  LockPtr->NestLevel = 1;
}

VOID 
UnLock (VOID* Handle)
{
  LockType *LockPtr = (LockType *)Handle;

  if (Handle == 0)
    return;

  if ((LockPtr->Flags & FLAG_RECURSIVE_LOCK) == 0)
  {
    MutexRelease (LockPtr->MutexPtr);
    return;
  }

  if (IsMutexHeld(LockPtr->MutexPtr))
  {
    if (LockPtr->NestLevel == 0)
      while (1);

    --LockPtr->NestLevel;
    if (LockPtr->NestLevel > 0)
      return;
  }
  else
  {
    /* Shouldn't have happened */
    while (1);
  }
  MutexRelease (LockPtr->MutexPtr);
}

  
