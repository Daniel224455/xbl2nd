#ifndef __DDR_TARGET_H__
#define __DDR_TARGET_H__

/*=============================================================================
                              DDR HAL
                            Header File
GENERAL DESCRIPTION
This is the target header file for DDR HAL.

Copyright 2014 by Qualcomm Technologies, Inc.  All Rights Reserved.

===========================================================================

                            EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.


when       who     what, where, why
--------   ---     ------------------------------------------------------------
03/12/14   sr      Initial revision.
=============================================================================*/
/*==============================================================================
                                  INCLUDES
==============================================================================*/
#include "ddr_common.h"
#include "HALhwio.h"
#include "ChipInfoDefs.h"

//Intermediate training buffer is at the start of AOP Code RAM
#define INTERMEDIATE_TRAINING_BUF      0x0B000000
#define DDR_INTERLEAVE_CHANNEL_SIZE    512

void updatemsm_info(void);

typedef struct 
{
  DDR_CHIPSELECT rank_detected;
  uint64 remap_size;
  uint64 bimc_remapper_cs0; /* BIMC Remapper CS0 */
  uint64 bimc_remapper_cs1; /* BIMC Remapper CS1 */
} RemapperTable;

typedef struct 
{
  ChipInfoIdType chip_id;
  ChipInfoVersionType chip_version;
  unsigned int feature_sku;
} MSMinfo; 

/*==============================================================================
                                  MACROS
==============================================================================*/

/*==============================================================================
                                  FUNCTIONS
==============================================================================*/


/* define the maximum partition size for saving ddr training logs and result */
#define DDR_MAX_LOG_SIZE 1*1024*1024 /* 1 MB space */
#endif /* __DDR_TARGET_H__ */

