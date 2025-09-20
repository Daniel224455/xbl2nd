#ifndef __ICBCFG_COMMON_H__
#define __ICBCFG_COMMON_H__
/*============================================================================

FILE:      icbcfg_common.h

DESCRIPTION: Common type definitions and includes for icbcfg.

PUBLIC CLASSES:  Not Applicable

INITIALIZATION AND SEQUENCING REQUIREMENTS:  N/A
 
Edit History

$Change: 30389851 $ 

When        Who    What, where, why
----------  ---    ----------------------------------------------------------- 
2017/10/26  sds    Increase maximum number of channels to 8 to support sdm1000
2016/10/11  sds    Created

         Copyright (c) 2016-2017 QUALCOMM Technologies Incorporated.
                          All Rights Reserved.
                       QUALCOMM Proprietary/GTDR
============================================================================*/
#include "comdef.h"
#include <stdbool.h>

/*============================================================================
                          DEFINEs/MACROs
============================================================================*/

/*============================================================================
                          TYPE DEFINITION
============================================================================*/
/**
 * ICBCFG Error types.
 */ 
typedef enum
{
  ICBCFG_SUCCESS             = 0,          /**< Success */
  ICBCFG_ERROR_INVALID_PARAM,              /**< Invalid Parameters */
  ICBCFG_ERROR_INIT_FAILURE,               /**< Initialization Failure */
  ICBCFG_ERROR_EXHAUSTED_SEGMENTS,         /**< Out of slave segments. */
  ICBCFG_ERROR_UNSUPPORTED,                /**< This operation unsupported on this target. */
  ICBCFG_ERROR_INVALID_ADDRESS,            /**< Unable to find an address translation. */
  /* Add new errors here */         

  ICBCFG_ERROR_SIZE = 0x7FFFFFFF, /* force 32-bit enum */
  ICBCFG_ERROR = -1
}icbcfg_error_type;

/**
 * ICBCFG memory region types
*/
typedef struct
{
  uint64 base_addr;
  uint64 size;
  bool   interleaved;
} region_type;

/* DDR channel definitions */
#define MAX_REGIONS 6
typedef struct
{
  region_type regions[MAX_REGIONS];
} regions_type;

/* DDR slave region configuration */
#define MAX_CHANNELS 8
typedef struct
{
  regions_type channels[MAX_CHANNELS];
} icb_mem_map_type;

typedef struct
{
  uint64 start_addr;
  uint64 end_addr;
} icbcfg_mem_region_type;

#endif /* __ICBCFG_COMMON_H__ */
