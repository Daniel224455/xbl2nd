#ifndef __HYP_HVX_COMMON_H__
#define __HYP_HVX_COMMON_H__
/*
===========================================================================
*/
/**
  @file hyp_hvx_common.h
  @brief Include file that defines common structs between UEFI/TZ

  */
/*
  ===========================================================================

  Copyright (c) 2016 Qualcomm Technologies Incorporated.
  All Rights Reserved.
  Qualcomm Confidential and Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of Qualcomm Technologies Incorporated.

  ===========================================================================

  $Header: //components/rel/boot.xf/2.2/QcomPkg/QcomTestPkg/HvxTestApp/hyp_hvx_common.h#2 $
  $DateTime: 2018/05/22 13:36:14 $
  $Author: pwbldsvc $

  ===========================================================================
  ===========================================================================
                             EDIT HISTORY

  when       who     what, where, why
  --------   ---     ----------------------------------------------------------
  05/21/18   pweber  Added Flags to SMC_MEMORY_MAPPING structure
  05/26/16   pgw     Initial revision
  ===========================================================================
*/
#include <stdint.h>
#include "errno.h"
#include <Uefi/UefiSpec.h>
#include "vm.h"

/**
   @ingroup secure_kernel_launch

   Authenticate SK Launcher, perform reloc fix-ups, extract jump address
   and return status.

   @smc_id
     0xC3000001

   @sys_call_params
     @param1 - LAUNCH_ATTRIBUTES pointer
     @param2 - ACTION bitmask
			0x1 - Authenticate
			0x2 - Authenticate and Launch

   @return
     E_SUCCESS on success; an error code otherwise.
*/
const uint32_t HYP_SECURE_KERNEL_LAUNCH_ID = 0xC3000001;

#define OEM_EL2_LAUNCH_MAJOR_VERSION 0x1
#define OEM_EL2_LAUNCH_MINOR_VERSION 0x0
#define PAGE_SIZE_4KB 0x1000
#define ROUND_TO_PAGE(x) ((x+PAGE_SIZE_4KB-1) & ~PAGE_SIZE_4KB)

#define SMC_MEMORY_MAPPING_FLAG_DMA       0x1
#define SMC_MEMORY_MAPPING_FLAG_EL2       0x2
#define SMC_MEMORY_MAPPING_FLAG_RESRV     0x4

/* Action bitmask */
typedef enum {
  HYP_SECURE_KERNEL_VERSION=0x1,
  HYP_SECURE_KERNEL_AUTHENTICATE=0x2,
  HYP_SECURE_KERNEL_MAP_MEMORY=0x3,
  HYP_SECURE_KERNEL_LAUNCH=0x4,
  HYP_SECURE_KERNEL_CANCEL=0x5,
  HYP_SECURE_KERNEL_INVALID=0x6
} SK_LAUNCH_ACTION;

typedef struct {
  UINT16 MajorVersion;
  UINT16 MinorVersion;
  UINT32 Size;
  UINT32 Action;
  UINT32 Reserved;
} SMC_TABLE_HEADER;

typedef struct {
  UINT64 VirtualAddress;
  UINT64 PhysicalAddress;
  UINT64 Size;
  VMPerm_t PermAttr;
  VMCache_t CacheAttr;
  UINT64 Flags;     // Added in V2
} SMC_MEMORY_MAPPING;

typedef struct {
  SMC_TABLE_HEADER Header;
  UINT64 SecLaunchImageBase;
  UINT64 SecLaunchImageSize;
  UINT64 TzAppDataBase;
  UINT64 TzAppDataSize;
} SMC_PARAMETERS_AUTHENTICATE;

typedef struct {
  SMC_TABLE_HEADER Header;
  UINT64 SecLaunchImageBase;
  UINT64 SecLaunchImageSize;
  UINT64 SecLaunchDataBase;
  UINT64 SecLaunchDataSize;
  UINT64 SecLaunchImageEntry;
} SMC_PARAMETERS_LAUNCH;

typedef struct {
  SMC_TABLE_HEADER Header;
  UINT32 MemoryMappingCount;
  UINT32 MemoryMappingSize;
  UINT32 MemoryMappingVersion;
  SMC_MEMORY_MAPPING *MemoryMappings;
} SMC_PARAMETERS_MAP_MEMORY;

typedef struct {
  SMC_TABLE_HEADER Header;
} SMC_PARAMETERS_CANCEL;

typedef struct {
  union {
    SMC_TABLE_HEADER Header;
    SMC_PARAMETERS_AUTHENTICATE Authenticate;
    SMC_PARAMETERS_MAP_MEMORY MapMemory;
    SMC_PARAMETERS_LAUNCH Launch;
    SMC_PARAMETERS_CANCEL Cancel;
  };
} SMC_PARAMETERS;

typedef struct {
  uint32_t Revision;
  uint32_t Size;
} SK_LAUNCH_TABLE_HEADER;

#endif // __HYP_HVX_COMMON_H__
