// ****************************************************************************
// QUALCOMM Proprietary Design Data
// Copyright (c) 2016, Qualcomm Technologies Incorporated. All rights reserved.
// ****************************************************************************
// ================================================================================
//                                 EDIT HISTORY
//
// ================================================================================
// when       who          what, where, why
// --------   ---          --------------------------------------------------------
// 11/07/16   pgw          Initial Revision
// ================================================================================

// -------------------
// MACRO: PushTwo
// -------------------
    .macro    PushTwo x0, x1
        stp     \x1, \x0, [sp, #-0x10]!
    .endm

// -------------------
// MACRO: PopTwo
// -------------------
    .macro    PopTwo x0, x1
        ldp     \x0, \x1, [sp], #0x10
    .endm

//=======================================================================
// UINT64 ArmSMCCall(UINT64 param1, UINT64 param2, UINT64 param3)
//
// Make ARM SMC call and return status in X0
//
  .global ArmSMCCall
ArmSMCCall:
   PushTwo x30, XZR
   SMC     0

   // Workaround for Errata #89. CR 933820
   // Clearing link stack only when EL2 MMU is disabled
  BL  SEQ1
SEQ1: BL SEQ2
SEQ2: BL SEQ3
SEQ3: BL SEQ4
SEQ4: BL SEQ5
SEQ5: BL SEQ6
SEQ6: BL SEQ7
SEQ7: BL SEQ8
SEQ8: BL SEQ9
SEQ9: BL SEQ10
SEQ10: BL SEQ11
SEQ11: BL SEQ12
SEQ12: BL SEQ13
SEQ13: BL SEQ14
SEQ14: BL SEQ15
SEQ15: BL SEQ16
SEQ16: NOP

   PopTwo  XZR, x30
   RET
