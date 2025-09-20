//============================================================================
//
//                 MOR-PPI ASM FUNCTIONS
//
// Copyright (c) 2019 Copyright Qualcomm Technologies, Inc.  All Rights Reserved.
// Qualcomm Technologies Proprietary and Confidential.
//   
//============================================================================

//============================================================================
//
//                           EDIT HISTORY FOR FILE
//
// This section contains comments describing changes made to the module.
// Notice that changes are listed in reverse chronological order.
//
//
// when       who     what, where, why
// --------   ---     --------------------------------------------------------
// 04/09/19   pj      Initial version.
//============================================================================

.text
.align 3

#define temp      x9
#define tempw     w9

//============================================================================
//                             MODULE EXPORTS
//============================================================================

  GCC_ASM_EXPORT(is_morppi_memclear_512bytes_possible)
  GCC_ASM_EXPORT(morppi_memclear_cached_aligned512bytes)

   
//--------------------------------------------------------------------
// int is_morppi_memclear_512bytes_possible()
//--------------------------------------------------------------------
is_morppi_memclear_512bytes_possible:
   MRS     temp, DCZID_EL0
   TBNZ    tempw, #4, memclear_512bytes_not_possible
   CMP     tempw, 4
   B.NE    memclear_512bytes_not_possible
   MOV     x0, #1
   RET
   
memclear_512bytes_not_possible:
   MOV     x0, #0
   RET


//--------------------------------------------------------------------
// void morppi_memclear_cached_aligned512bytes(uint64 addr, uint64 size)
//--------------------------------------------------------------------
morppi_memclear_cached_aligned512bytes:
   ADD     x2, x0, #64
   ADD     x3, x2, #64
   ADD     x4, x3, #64
   ADD     x5, x4, #64
   ADD     x6, x5, #64
   ADD     x7, x6, #64
   ADD     x8, x7, #64
clearloop3:
   DC      ZVA, x0         // 64 byte write of zeros
   DC      ZVA, x2         // 64 byte write of zeros
   DC      ZVA, x3         // 64 byte write of zeros
   DC      ZVA, x4         // 64 byte write of zeros
   DC      ZVA, x5         // 64 byte write of zeros
   DC      ZVA, x6         // 64 byte write of zeros
   DC      ZVA, x7         // 64 byte write of zeros
   DC      ZVA, x8         // 64 byte write of zeros
   ADD     x0, x0, #512
   ADD     x2, x2, #512
   ADD     x3, x3, #512
   ADD     x4, x4, #512
   ADD     x5, x5, #512
   ADD     x6, x6, #512
   ADD     x7, x7, #512
   ADD     x8, x8, #512
   SUB     x1, x1, #512
   CBNZ    x1, clearloop3
   DSB     SY
   ISB
   RET