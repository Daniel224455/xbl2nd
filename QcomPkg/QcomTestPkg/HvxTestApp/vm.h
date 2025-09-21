#ifndef VM_H
#define VM_H
/*=============================================================================

      Virtual Machine Manager

FILE:      vm.h

GENERAL DESCRIPTION
  This file contains data structures and exported API by SMMU LPAE translation.

EXTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS

===============================================================================
Copyright (c) 2014
Qualcomm Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary
=============================================================================*/
/*=============================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/boot.xf/2.2/QcomPkg/QcomTestPkg/HvxTestApp/vm.h#1 $
$DateTime: 2017/11/16 04:09:27 $
$Author: pwbldsvc $
$Change: 14860157 $

when       who     what, where, why
--------   ---     ------------------------------------------------------------
09/18/14   sho     Initial version
=============================================================================*/

/* ----------------------------------------------------------------------------
** Includes
** ------------------------------------------------------------------------- */
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ----------------------------------------------------------------------------
** Types
** ------------------------------------------------------------------------- */
// cache property
typedef enum
{

  // device non-gathering, non-reordering, no early write acknowledgement
  VM_CACHE_DEVICE_nGnRnE       = 0,   // b0000
  VM_CACHE_DEVICE_nGnRE        = 1,   // b0001
  VM_CACHE_DEVICE_nGRE         = 2,   // b0010
  VM_CACHE_DEVICE_GRE          = 3,   // b0011
  // outer non-cacheable, inner non-cacheable
  VM_CACHE_OUTER_NC_INNER_NC   = 5,   // b0101
  // outer non-cacheable, inner write-through
  VM_CACHE_OUTER_NC_INNER_WT   = 6,   // b0110
  // outer non-cacheable, inner writeback
  VM_CACHE_OUTER_NC_INNER_WB   = 7,   // b0111
  VM_CACHE_OUTER_WT_INNER_NC   = 9,   // b1001
  VM_CACHE_OUTER_WT_INNER_WT   = 10,  // b1010
  VM_CACHE_OUTER_WT_INNER_WB   = 11,  // b1011
  VM_CACHE_OUTER_WB_INNER_NC   = 13,  // b1101
  VM_CACHE_OUTER_WB_INNER_WT   = 14,  // b1110
  VM_CACHE_OUTER_WB_INNER_WB   = 15,  // b1111
  // use stage 1 or outer/inner WB
  VM_CACHE_DEFAULT             = 15,

  VM_CACHE_FORCE_ENUM_32_BIT   = 0x7FFFFFFF  /* Force to 32 bit enum */
}
VMCache_t;

// sharability
typedef enum

{
   VM_SHARE_NONE         = 0,
   VM_SHARE_OUTER        = 2,
   VM_SHARE_INNER        = 3,
   // use stage 1 or non-cached
   VM_SHARE_DEFAULT      = 0,

   VM_SHARE_FORCE_ENUM_32_BIT   = 0x7FFFFFFF   /* Force to 32 bit enum */
}
VMShare_t;

// permission
typedef enum
{
   VM_PERM_X      = 0x1,
   VM_PERM_W      = 0x2,
   VM_PERM_R      = 0x4,
   VM_PERM_RWX    = VM_PERM_R | VM_PERM_W | VM_PERM_X,
   VM_PERM_RW     = VM_PERM_R | VM_PERM_W,
   VM_PERM_RX     = VM_PERM_R | VM_PERM_X,
   VM_PERM_WX     = VM_PERM_W | VM_PERM_X,
   // user stage 1 or RWX
   VM_PERM_DEFAULT= VM_PERM_RWX,

   VM_PERM_FORCE_ENUM_32_BIT = 0x7FFFFFFF  /* Force to 32 bit enum */
}
VMPerm_t;

typedef enum
{
   VM_UNMAPPED,
   VM_MAPPED,
   VM_PARTIAL_MAPPED,
   VM_INVALID_MAP_STATUS,
   VM_MAPPED_NON_IDEM,
   VM_PARTIAL_MAPPED_NON_IDEM,
   VM_TABLE_OUT_OF_MEM
}
VMMapStatus_t;

// memory map attributes
typedef struct
{
  uint64_t     size;
  uint64_t     inputAddr;
  uint64_t     outputAddr;
  VMPerm_t     perm;
  VMShare_t    shareAttr;
  VMCache_t    cacheAttr;
}
VMMapAttr_t;

typedef struct
{
   uint32_t    inputBitSize;
   uint32_t    outputBitSize;
}
VMPageTableAttr_t;


#ifdef __cplusplus
}
#endif

#endif // VM_H
