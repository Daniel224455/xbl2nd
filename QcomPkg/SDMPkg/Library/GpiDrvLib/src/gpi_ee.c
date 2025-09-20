/**
  @file gpi_ee.c
  @brief
  This file contains implementation of interfaces that provide a thin
  abstraction to various OS-specific APIs used by the GPI driver.
*/

/*
===============================================================================

                             Edit History


when       who     what, where, why
--------   ---     ------------------------------------------------------------
01/31/17   ts      file ported to UEFI.
02/19/16   ah      Created

===============================================================================
                   Copyright (c) 2016-2017 Qualcomm Technologies Incorporated.
                          All Rights Reserved.
                        Qualcomm Confidential and Proprietary.
===============================================================================
*/

#include "gpi_utils.h"

static char  gpii[GPII_HEAP_SIZE];
static uint8 gpii_idx = 0;

static char  evt_rings_mem[EVT_RINGS_HEAP_SIZE]__attribute__((aligned(EVT_RING_SIZE)));
static uint8 er_idx;

/**
 * Copies memory of a specofied size to the destination.
 *
 * @param[in] dst Pointer to the destination memory address
 * @param[in] src Pointer to the source memory address
 * @param[in] size Number of bytes to copy
 *
 * @return   None.
 */
void gpi_memcpy(void *dst, void *src, uint32 size)
{
  gBS->CopyMem(dst, src, (UINTN)size);

}

/**
 *  Sets memory of specified size to specified value.
 *
 *  @param[in] mem Pointer to the block of memory to fill
 *  @param[in] val Value of byte to fill the memory with
 *  @param[in] size Number of bytes to fill
 *
 *  @return   None.
 */
void gpi_memset(void *mem, uint8 val, uint32 size)
{
  gBS->SetMem(mem, (UINTN)size, val);
}

/**
 * SDC EE needs to translate local to system addresses.
 * For SDC this function is defined in system driver.
 *
 * @param[in]   addr    Address to translate
 *
 * @return      Translated address.
 */
U_LONG get_system_address(U_LONG addr)
{
   return addr;
}

/**
 * SDC EE needs to translate system to local addresses.
 * For SDC this function is defined in system driver.
 *
 * @param[in]   addr    Address to translate
 *
 * @return      Translated address.
 */
U_LONG get_local_address(U_LONG addr)
{
   return addr;
}

/**
 * Performs a cache maintanence operation.
 *
 * @param[in]  cache_op    Cache Operation to perform (clean/flush/invalidate)
 * @param[in]  address     Memory for which cache maintanence to be performed.
 * @param[in]  size        Size of the memory
 *
 * @return   None.
 */
void gpi_cachecmd(gpi_cache_op_type cache_op, U_LONG address, uint32 size)
{
  WriteBackInvalidateDataCacheRange((void *)address, size);

}

/**
 * Performs a processor specific memory barrier operation.
 *
 * @param    None
 *
 * @return   None.
 */
void gpi_memorybarrier(void)
{
  MemoryFence();
}

/**
 * @brief      Get physical address of a given virtual address
 *
 * @param[in]  buf - Virtual address
 *
 * @return     Physical address
 */
void* gpi_va_to_pa(void *buf)
{
  return buf;
}

/**
  Sleep function.

  @param[in]  wait    Sleep amount (usec for Q6)

  @return  None.
*/
void gpi_sleep(uint32 wait)
{
  gBS->Stall(wait);
}

/**
  Dynamically allocate GPII memory.

  @param[in]  None

  @return  None.
*/
void* gpii_malloc(void)
{
  void *ptr = NULL;

  if (((gpii_idx + 1) * sizeof(gpii_info)) > GPII_HEAP_SIZE)
  {
    gpi_log(GPI_ERROR, 0, "gpii_malloc - out of memory!\n");
    return ptr;
  }

  ptr = (void *)&gpii[gpii_idx * sizeof(gpii_info)];
  gpii_idx++;

  return ptr;
}

/**
  Dynamically allocate event ring memory.

  @param[in]  None

  @return  Pointer to allocated memory.
*/
void* gpi_er_malloc(void)
{
   void *ptr = NULL;

   if (((gpii_idx + 1) * EVT_RING_SIZE) > EVT_RINGS_HEAP_SIZE)
   {
      gpi_log(GPI_ERROR, 0, "er_malloc - out of ER memory!\n");
      return ptr;
   }

   ptr = (void *)&evt_rings_mem[er_idx * EVT_RING_SIZE];
   er_idx++;
   return ptr;
}
