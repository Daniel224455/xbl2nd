/**********************************************************************
  nvme_loader_bsp.c

  NVMe driver BSP
  This file implements the NVMe driver BSP for the board in use

  Copyright (c) 2018 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential

**********************************************************************/

/*===========================================================================
                                 Edit History

when         who   what, where, why
----------   ---   ----------------------------------------------------------
2018-05-07   wek   Initial version.

===========================================================================*/

#include "DDIPlatformInfo.h"
#include "XBLLoader/boot_cache_mmu.h"
#include <api/systemdrivers/busywait.h>
#include "boot_logger.h"



/** Wait for the specified amount of us
 *
 * param us  microseconds to wait
 *
 * return   None.
 *
 **/
void nvme_bsp_busy_wait(uint32 us)
{
  static boolean busywait_inited = FALSE;
  if (FALSE == busywait_inited)
  {
    busywait_init();
    busywait_inited = TRUE;
  }
  busywait(us);
}

/** Flush the memory region in the CPU cache to main memory.
 *
 * param addr   start address of the memory region
 * param length length of the memory region
 *
 * return   None
 *
 */
void nvme_bsp_cache_flush(void *addr, uint32 length)
{
  dcache_flush_region (addr, length);
}

/** Invalidate the memory region in the CPU cache to read new data from
 *   main memory.
 *
 * param addr   start address of the memory region
 * param length length of the memory region
 *
 * return   None
 *
 */
void nvme_bsp_cache_inval(void *addr, uint32 length)
{
  dcache_inval_region (addr, length);
}



/** Force memory operations to be completed before executing this code.
 */
void nvme_bsp_memory_barrier(void)
{
  data_mem_barrier();
}

/* LOG error messages. */
void nvme_bsp_log_error(char *str)
{
  boot_log_message(str);
}
