#ifndef __STORAGECOMMON_H__
#define __STORAGECOMMON_H__

/** @file StorageCommon.h

  Implementation of Storage Common Library Functions like profiling.

  Copyright (c) 2016, Qualcomm Technologies Inc.
  All Rights Reserved.
  Qualcomm Confidential and Proprietary.

**/

/*=============================================================================
                              EDIT HISTORY


 when       who     what, where, why
 --------   ---     -----------------------------------------------------------
 08/30/18   bb      Initial version

=============================================================================*/

#include "com_dtypes.h"

/* Limitation : Nested profiling is not supported */

/* Define to enable profiling */
// #define STOR_PROFILE_ENABLE

/* Profile enable masks for each driver */
#define SDCC_PROFILE_INIT_BIT_MASK       (1<<0)
#define SDCC_PROFILE_READ_BIT_MASK       (1<<1)
#define SDCC_PROFILE_WRITE_BIT_MASK      (1<<2)
#define SDCC_PROFILE_ERASE_BIT_MASK      (1<<3)
#define UFS_PROFILE_INIT_BIT_MASK        (1<<4)
#define UFS_PROFILE_READ_BIT_MASK        (1<<5)
#define UFS_PROFILE_WRITE_BIT_MASK       (1<<6)
#define UFS_PROFILE_ERASE_BIT_MASK       (1<<7)
#define NOR_PROFILE_INIT_BIT_MASK        (1<<8)
#define NOR_PROFILE_READ_BIT_MASK        (1<<9)
#define NOR_PROFILE_WRITE_BIT_MASK       (1<<10)
#define NAND_PROFILE_INIT_BIT_MASK       (1<<11)
#define NAND_PROFILE_READ_BIT_MASK       (1<<12)
#define NAND_PROFILE_WRITE_BIT_MASK      (1<<13)
#define STOR_PROFILE_BENCHMARK_BIT_MASK  (1<<14)

#ifdef STOR_PROFILE_ENABLE

  /* Events that are enabled for profiling - Modify based on the need */
  #define STOR_PROFILE_BIT_MASK    (UFS_PROFILE_INIT_BIT_MASK|UFS_PROFILE_READ_BIT_MASK|UFS_PROFILE_WRITE_BIT_MASK|UFS_PROFILE_ERASE_BIT_MASK)

  void StorageProfileStart (void);
  void StorageProfileEnd (UINT32 opMask, char *op, UINT64 address, UINT32 size);

  #define STOR_PROFILE_START StorageProfileStart()
  #define STOR_PROFILE_END(a,b,c,d) StorageProfileEnd(a,b,c,d)

#else

  #define STOR_PROFILE_BIT_MASK    0
  #define STOR_PROFILE_START
  #define STOR_PROFILE_END(a,b,c,d)

#endif // STOR_PROFILE_ENABLE

#endif  /* __STORAGECOMMON_H__ */
