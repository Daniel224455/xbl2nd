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
#include <Library/RandomStackCanaryLib.h>
#include <Library/DebugLib.h>
#include <Library/SerialPortShLib.h>

void* __stack_chk_guard = (void*)0xc0c0c0c0;

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

 
/**
 * Callback if stack cananry is corrupted
 * */
void __stack_chk_fail (void)
{
  volatile UINT32 i = 1;
  /* Loop forever in case of stack overflow. Avoid
  calling into another api in case of stack corruption/overflow */
  DEBUG((EFI_D_ERROR,"__stack_chk_fail : Stack corruption detected \n"));
  SerialPortFlush();
  while (i);
}