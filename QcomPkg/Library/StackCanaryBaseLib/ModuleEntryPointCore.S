//------------------------------------------------------------------------------ 
//
//  Copyright (c) 2015-2018, Qualcomm Technologies Inc. All rights reserved.
//  Portions Copyright (c) 2011-2013, ARM Limited. All rights reserved.
//
//  This program and the accompanying materials
//  are licensed and made available under the terms and conditions of the BSD License
//  which accompanies this distribution.  The full text of the license may be found at
//  http://opensource.org/licenses/bsd-license.php
//
//  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
//  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
//
//------------------------------------------------------------------------------

//=============================================================================
//                              EDIT HISTORY
//
//
// when       who     what, where, why
// --------   ---     ---------------------------------------------------------
// 09/10/18   bd      Check for X1 at DxeCore stage
// 11/13/17   bd      Save stack canary into system table
// 06/28/16   vk      Enable TZ call
// 10/27/15   vk      Initial revision
//
//============================================================================


.text
.align 3

GCC_ASM_IMPORT (_ModuleEntryPoint)
GCC_ASM_IMPORT (InitStackCanary)
GCC_ASM_IMPORT (gEfiStackCanaryGuid)
GCC_ASM_IMPORT (__stack_chk_guard)

GCC_ASM_EXPORT (_QCModuleEntry)

ASM_PFX(_QCModuleEntry):
  cmp x0, #0
  beq EntryPoint  
  
  stp x3,  x2,  [sp, #-0x10]!
  stp x1,  x0,  [sp, #-0x10]!
  stp x9,  x8,  [sp, #-0x10]!
  stp x30, x29, [sp, #-0x10]!
  
  // We are here means we already crossed the SEC stage
  // x1 has System Table pointer, starting from EnvDxe onwards.
  // First member of the ST is signature, which will be "IBI SYST" in ASCII
  // If signature matches, that indicates we crossed DxeCore stage

  // Check if X1 has System Table address
  cmp x1, #0  
  beq DxeStage
  
  ldrb  w8, [x1]
  cmp   w8, #73
  b.ne  DxeStage
  ldrb  w8, [x1, #1]
  cmp   w8, #66
  b.ne  DxeStage
  ldrb  w8, [x1, #2]
  cmp   w8, #73
  beq   DxeStageOver

ASM_PFX(DxeStage):
  bl InitStackCanary
  b  JumpToEntryPoint

ASM_PFX(DxeStageOver):

  // Get the configTable pointer and search for StackCanaryGUID; If found read from it, if not found install the new one 
  ldr x2, [x1,#0x68]            //gST->NumberOfTableEntries
  ldr x3, [x1,#0x70]            //gST->ConfigurationTable
  
  ldr x9, =gEfiStackCanaryGuid
  ldr x9, [x9]  
  
ReadNextGuid: 
  cbz x2, StackCanaryGuidNotFound
  sub x2, x2, #0x01             // count--
  ldr x8, [x3],#0x18            // load GUID into x8; increment x3 by 0x18; sizeof(EFI_CONFIGURATION_TABLE)
  cmp x8, x9                    // compare with stack canary GUID
  bne ReadNextGuid
  
  // in case of a match, we fall through ; Read the GUID and use it
  sub x3, x3, #0x8              // substract 8-bytes to read VendorTable, which is nothing but stack canary our case
  ldr x3, [x3]
  
  ldr x1, =__stack_chk_guard    // Read and load the stack canary value from gST->ConfigurationTable[index].VendorTable
  str x3, [x1]
  b JumpToEntryPoint

StackCanaryGuidNotFound:  
  stp x1,  x0,  [sp, #-0x10]!
  bl InitStackCanary
  ldp x1,  x0,  [SP],#0x10
  
  add x3, x1,#0x60              //offset of BootServicesTable in SystemTable
  ldr x1, __stack_chk_guard     // read the value
  ldr x0, =gEfiStackCanaryGuid  
  ldr x2, [x3]
  add x2, x2,#0xC0              //offset of gBS->InstallConfigurationTable 
  ldr x3, [x2]
  blr x3
  
ASM_PFX(JumpToEntryPoint): 
  ldp x30, x29, [SP],#0x10
  ldp x9,  x8,  [sp],#0x10
  ldp x1,  x0,  [SP],#0x10
  ldp x3,  x2,  [SP],#0x10

ASM_PFX(EntryPoint): 
  b _ModuleEntryPoint
 
