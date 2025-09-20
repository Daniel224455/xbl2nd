#ifndef __DDR_STRESS_TEST_H__
#define __DDR_STRESS_TEST_H__

/** @file DDRStressTest.h

#  Copyright (c) 2018 Qualcomm Technologies, Inc. All rights reserved.
   
**/

struct ddr_mem_descriptor
{
    UINT64 *AllocBase;
    UINT64 WriteSize;
    UINT64 time_ms;
};

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

VOID memory_test (void*);

/* ============================================================================
**  Function : get_available_memory
** ============================================================================
*/
/*!
*   @brief
*   This function is called to allocate maximum available contiguous free memory block
*
*   @param
*   FreeSize - number of allocated contiguous free 4K pages.
*
*   @par Dependencies
*
*   @par Side Effects
*   None
*
*   @retval  Return address of allocated contiguous memory 
*
*/
UINT64* get_available_memory (UINT64 *FreeSize);

/* ============================================================================
**  Function : free_memory
** ============================================================================
*/
/*!
*   @brief
*   This function is called to free allocated maximum available contiguous free memory block
*
*   @param
*   AllocBase - Address of allocated memory block to be freed
*   FreeSize  - Number of allocated pages to be freed.
*
*   @par Dependencies
*
*   @par Side Effects
*   None
*
*   @retval  
*   None
*/
void free_memory (UINT64* AllocBase,UINT64 FreeSize);

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
UINT64* ReadVerifyAsm (UINT64* BasePtr, UINT64* EndPtr, UINT64 Pattern);

/* ============================================================================
**  Function : RunDDRStressTest
** ============================================================================
*/
/*!
*   @brief
*   This function is used to run DDR write/read tests and launch it in multiple cores.
*
*   @param
*
*   @par Dependencies
*
*   @par Side Effects
*   None
*
*   @retval  
*   None
*/

EFI_STATUS RunDDRStressTest (UINTN Argc, UINT8* core_numbers, UINT64 time_sec, UINT8 clock_index);

#endif