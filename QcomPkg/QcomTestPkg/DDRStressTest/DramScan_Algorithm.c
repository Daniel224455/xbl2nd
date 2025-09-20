/** @file MemTest.c
   
  Application to test Multi core/threading in UEFI env.
  
  Copyright (c) 2018 Qualcomm Technologies, Inc. All rights reserved.
   
**/

/*=============================================================================
                              EDIT HISTORY


  when         who     what, where, why
  ----------   ---     -----------------------------------------------------------
  06/16/2017   yg      Initial version
=============================================================================*/

#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/QcomLib.h>
#include <Library/QcomUtilsLib.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UncachedMemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Protocol/LoadedImage.h>
#include <Library/QcomBaseLib.h>
#include <DDRStressTest.h>


/* ============================================================================
**  Function : VerifyError
** ============================================================================
*/
/*!
*   @brief
*   This function is used to inform if the verify pattern fails
*
*   @param
*   BasePtr     - Address of DDR where read verify failed.
*   Pattern     - Pattern expected.
*
*   @par Dependencies
*
*   @par Side Effects
*   None
*
*   @retval  
*   None
*/

VOID VerifyError (UINT64* Loc, UINT64 Pattern)
{
  AsciiPrint ("Data error at %X, expected %X, Read:\n", Loc, Pattern);
  AsciiPrint ("%X %X\n", *(Loc + 0), *(Loc + 1));
  AsciiPrint ("%X %X\n", *(Loc + 2), *(Loc + 3));
  AsciiPrint ("%X %X\n", *(Loc + 4), *(Loc + 5));
  AsciiPrint ("%X %X\n", *(Loc + 6), *(Loc + 7));
}


/* ============================================================================
**  Function : ReadVerify
** ============================================================================
*/
/*!
*   @brief
*   This function is called to verify the written pattern
*
*   @param
*   BasePtr     - Address of DDR to verify
*   EndPtr      - DDR memory verify size
*   Pattern     - Pattern to verify
*
*   @par Dependencies
*
*   @par Side Effects
*   None
*
*   @retval  
*   None
*/

UINT64* ReadVerify (UINT64* BasePtr, UINT64* EndPtr, UINT64 Pattern)
#ifndef USE_CACHED_BUFFER
{
  __uint128_t p0, p1, cm, *ptr;
  
  cm = Pattern | ((__uint128_t)Pattern << 64);

  while (BasePtr < EndPtr)
  {
    ptr = (__uint128_t *)BasePtr;

    p0 = *(ptr + 0);
    p1 = *(ptr + 1);

    if ((p0 != cm) || (p1 != cm))
      return BasePtr;

    BasePtr += 4;
  }
  return 0;
}
#else
{
  while (BasePtr < EndPtr)
  {
    if (*BasePtr != Pattern)
      return BasePtr;
    BasePtr++;
  }
  return 0;
}
#endif

/* ============================================================================
**  Function : FillPattern
** ============================================================================
*/
/*!
*   @brief
*   This function is called to free allocated maximum available contiguous free memory block
*
*   @param
*   Pattern   - Pattern to write
*   Base      - Address of DDR to write
*   size      - DDR memory write size
*
*   @par Dependencies
*
*   @par Side Effects
*   None
*
*   @retval  
*   None
*/

EFI_STATUS FillPattern (UINT64 Pattern, UINT64* Base, UINT64 size, UINT64 time_ms)
{
  UINT64* EndPtr, *BasePtr;
  UINT64 start, end, total=0;
  while(1)
  {
      Base = (UINT64*)(((UINT64)Base) & (0xFFFFFFFFFFFFFFC0ULL));
      EndPtr = Base + (size/sizeof(*Base));
      BasePtr = Base;
      start =GetTimerCountms ();
      
      while (BasePtr < EndPtr)
      {
        *(BasePtr + 0) = Pattern;
        *(BasePtr + 1) = Pattern;
        *(BasePtr + 2) = Pattern;
        *(BasePtr + 3) = Pattern;
        *(BasePtr + 4) = Pattern;
        *(BasePtr + 5) = Pattern;
        *(BasePtr + 6) = Pattern;
        *(BasePtr + 7) = Pattern;

        BasePtr += 8;
      }
    #if 0
      BasePtr = ReadVerifyAsm (Base, EndPtr, Pattern);
    #else
      BasePtr = ReadVerify (Base, EndPtr, Pattern);
    #endif

      if (BasePtr)
      {
        VerifyError (BasePtr, Pattern);
        return EFI_VOLUME_CORRUPTED;
      }
      end = GetTimerCountms ();
      total = total + (end-start);
      if(total > time_ms)
      {
        break;
      }
  }  
  return EFI_SUCCESS;
}


/* ============================================================================
**  Function : memory_test
** ============================================================================
*/
/*!
*   @brief
*   This function is called to test memory by writing pattern
*
*   @param
*   ptr - Memory Descriptor containing start address and size
*
*   @par Dependencies
*
*   @par Side Effects
*   None
*
*   @retval  None
*
*/
VOID memory_test (void *ptr)
{
  struct ddr_mem_descriptor *mem_descriptor = (struct ddr_mem_descriptor *)ptr;
  FillPattern (0x5A5A5A5A5A5A5A5A, mem_descriptor->AllocBase, mem_descriptor->WriteSize, mem_descriptor->time_ms); 
}


