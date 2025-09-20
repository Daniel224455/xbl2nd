//------------------------------------------------------------------------------ 
//
//  Copyright (c) 2017, Qualcomm Technologies Inc. All rights reserved.
//
//------------------------------------------------------------------------------

//=============================================================================
//                              EDIT HISTORY
//
// 09/14/17   yg      Initial revision
//
//============================================================================


.text
.align 3


//  Returns the address of error or zero on successful verification
// UINT64* ReadVerifyAsm (UINT64* BasePtr, UINT64* EndPtr, UINT64 Pattern)
GCC_ASM_EXPORT (ReadVerifyAsm)

ReadVerifyAsm:
  sub	x8, x1, #0x40
  cmp	x8, x0
  b.ls	Done

#define USE_FP_QUAD_REGS
#ifdef USE_FP_QUAD_REGS
  dup v10.2D, x2
ReadStart:
  ldp	q11, q12, [x0]
  cmeq v11.2D, v10.2D, #0
  b.eq FailRet
  cmeq v12.2D ,v10.2D, #0
  b.eq FailRet
  add	x0, x0, #128
#else
ReadStart:
  ldp	x10, x11, [x0]
  eor	x3, x10, x2
  eor	x4, x11, x2
  orr	x5, x3, x4

//  ldp	x12, x13, [x0,#16]
//  eor	x3, x12, x2
//  eor	x4, x13, x2
//  orr	x5, x3, x4
//  ldp	x14, x15, [x0,#32]
//  eor	x3, x14, x2
//  eor	x4, x15, x2
//  orr	x5, x3, x4
//  ldp	x16, x17, [x0,#48]
//  eor	x3, x16, x2
//  eor	x4, x17, x2
//  orr	x5, x3, x4

  cbnz	x5, FailRet

//  prfm    PLDL1KEEP, [x0, #64]

//  eor	x7, x11, x10
//  eor	x1, x13, x12
//  orr	x3, x1, x2
//  eor	x4, x15, x14
//  orr	x5, x3, x4
//  eor	x6, x16, x17
//  orr	x7, x5, x6
//  cbnz	x7, FailRet

  add	x0, x0, #16
#endif

  cmp	x0, x8
  b.cc	ReadStart
Done:
  mov	x0, xzr
FailRet:
  ret

