/******************************************************************//**
 * @file StackCanryCore.c
 *
 * @brief StackCanryCore functions
 *
 * Copyright (c) 2016-2017 by Qualcomm Technologies, Inc.
 * All Rights Reserved.
 *
 *********************************************************************/
/*=======================================================================
                        Edit History

when       who     what, where, why
--------   ----    --------------------------------------------------- 
11/13/17   bd      Save stack canary into system table
07/06/16   vk      Initial version
========================================================================*/
#include <Uefi/UefiBaseType.h>
#include <Library/RandomStackCanaryLib.h>

extern void* __stack_chk_guard;

EFI_GUID gEfiStackCanaryGuid = {0xb898d8dc, 0x080a, 0x40f7, {0x99,0xe3,0x31,0x62,0x7b,0x80,0x6a,0x5a}};

VOID InitStackCanary (VOID);

/**
 * Initialize stack check canary
 */
VOID InitStackCanary (VOID)
{
#ifndef PRE_SIL
  __stack_chk_guard = (VOID*) GetRandomStackCanary();
#endif
}
