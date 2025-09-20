#ifndef __DRE_HWIO_H__
#define __DRE_HWIO_H__
/*
===========================================================================
*/
/**
  @file dre_test.h
  @brief Auto-generated HWIO interface include file.

  Reference chip release:
    SM8150 (Hana) v2 [hana_v2.1_p3q2r59]
 
  This file contains HWIO register definitions for the following modules:
    DDR_TG0_TG
    DPC_REG_DPCC_DPCC_DPCC_REG
    DRE_REGS
    DRE_DRE_DRE_ECC


  Generation parameters: 
  { u'filename': u'dre_test.h',
    u'module-filter-exclude': { },
    u'module-filter-include': { },
    u'modules': [ u'DDR_TG0_TG',
                  u'DPC_REG_DPCC_DPCC_DPCC_REG',
                  u'DRE_REGS',
                  u'DRE_DRE_DRE_ECC'],
    u'output-fvals': True,
    u'output-offsets': True,
    u'output-phys': True}
*/
/*
  ===========================================================================

  Copyright (c) 2020 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies, Inc. and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of Qualcomm Technologies, Inc.

  ===========================================================================
  when       who        what, where, why
  --------   ---        -----------------------------------------------------
  07/13/2020 adawar     Added DRE change to poipuAU from hanaAU
  ===========================================================================

  $Header: //components/rel/boot.xf/3.0.1.c1/QcomPkg/SDMPkg/1000/Library/DRETargetLib/dre_hwio.h#1 $
  $DateTime: 2021/04/29 02:01:56 $
  $Author: pwbldsvc $

  ===========================================================================
*/

#include "msmhwiobase.h"
#include "HALhwio.h"

/*----------------------------------------------------------------------------
 * MODULE: DDR_TG0_TG
 *--------------------------------------------------------------------------*/

#define DDR_TG0_TG_REG_BASE                                                          (DDR_SS_BASE      + 0x000c2000)
#define DDR_TG0_TG_REG_BASE_SIZE                                                     0x100
#define DDR_TG0_TG_REG_BASE_USED                                                     0xd0
#define DDR_TG0_TG_REG_BASE_PHYS                                                     (DDR_SS_BASE_PHYS + 0x000c2000)
#define DDR_TG0_TG_REG_BASE_OFFS                                                     0x000c2000

#define HWIO_DDR_TG0_TG_STREAM_WR_OPERATION_ADDR                                     (DDR_TG0_TG_REG_BASE      + 0x00000000)
#define HWIO_DDR_TG0_TG_STREAM_WR_OPERATION_PHYS                                     (DDR_TG0_TG_REG_BASE_PHYS + 0x00000000)
#define HWIO_DDR_TG0_TG_STREAM_WR_OPERATION_OFFS                                     (DDR_TG0_TG_REG_BASE_OFFS + 0x00000000)
#define HWIO_DDR_TG0_TG_STREAM_WR_OPERATION_RMSK                                            0x1
#define HWIO_DDR_TG0_TG_STREAM_WR_OPERATION_IN          \
        in_dword_masked(HWIO_DDR_TG0_TG_STREAM_WR_OPERATION_ADDR, HWIO_DDR_TG0_TG_STREAM_WR_OPERATION_RMSK)
#define HWIO_DDR_TG0_TG_STREAM_WR_OPERATION_INM(m)      \
        in_dword_masked(HWIO_DDR_TG0_TG_STREAM_WR_OPERATION_ADDR, m)
#define HWIO_DDR_TG0_TG_STREAM_WR_OPERATION_OUT(v)      \
        out_dword(HWIO_DDR_TG0_TG_STREAM_WR_OPERATION_ADDR,v)
#define HWIO_DDR_TG0_TG_STREAM_WR_OPERATION_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DDR_TG0_TG_STREAM_WR_OPERATION_ADDR,m,v,HWIO_DDR_TG0_TG_STREAM_WR_OPERATION_IN)
#define HWIO_DDR_TG0_TG_STREAM_WR_OPERATION_TRIGGER_BMSK                                    0x1
#define HWIO_DDR_TG0_TG_STREAM_WR_OPERATION_TRIGGER_SHFT                                    0x0

#define HWIO_DDR_TG0_TG_STREAM_WR_CFG0_ADDR                                          (DDR_TG0_TG_REG_BASE      + 0x00000004)
#define HWIO_DDR_TG0_TG_STREAM_WR_CFG0_PHYS                                          (DDR_TG0_TG_REG_BASE_PHYS + 0x00000004)
#define HWIO_DDR_TG0_TG_STREAM_WR_CFG0_OFFS                                          (DDR_TG0_TG_REG_BASE_OFFS + 0x00000004)
#define HWIO_DDR_TG0_TG_STREAM_WR_CFG0_RMSK                                          0x7ffffff3
#define HWIO_DDR_TG0_TG_STREAM_WR_CFG0_IN          \
        in_dword_masked(HWIO_DDR_TG0_TG_STREAM_WR_CFG0_ADDR, HWIO_DDR_TG0_TG_STREAM_WR_CFG0_RMSK)
#define HWIO_DDR_TG0_TG_STREAM_WR_CFG0_INM(m)      \
        in_dword_masked(HWIO_DDR_TG0_TG_STREAM_WR_CFG0_ADDR, m)
#define HWIO_DDR_TG0_TG_STREAM_WR_CFG0_OUT(v)      \
        out_dword(HWIO_DDR_TG0_TG_STREAM_WR_CFG0_ADDR,v)
#define HWIO_DDR_TG0_TG_STREAM_WR_CFG0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DDR_TG0_TG_STREAM_WR_CFG0_ADDR,m,v,HWIO_DDR_TG0_TG_STREAM_WR_CFG0_IN)
#define HWIO_DDR_TG0_TG_STREAM_WR_CFG0_STOP_ON_ERROR_BMSK                            0x40000000
#define HWIO_DDR_TG0_TG_STREAM_WR_CFG0_STOP_ON_ERROR_SHFT                                  0x1e
#define HWIO_DDR_TG0_TG_STREAM_WR_CFG0_ILOOP_BMSK                                    0x20000000
#define HWIO_DDR_TG0_TG_STREAM_WR_CFG0_ILOOP_SHFT                                          0x1d
#define HWIO_DDR_TG0_TG_STREAM_WR_CFG0_OTCNT_BMSK                                    0x1f000000
#define HWIO_DDR_TG0_TG_STREAM_WR_CFG0_OTCNT_SHFT                                          0x18
#define HWIO_DDR_TG0_TG_STREAM_WR_CFG0_IAG_BMSK                                        0xff0000
#define HWIO_DDR_TG0_TG_STREAM_WR_CFG0_IAG_SHFT                                            0x10
#define HWIO_DDR_TG0_TG_STREAM_WR_CFG0_STRIDE_BMSK                                       0xff00
#define HWIO_DDR_TG0_TG_STREAM_WR_CFG0_STRIDE_SHFT                                          0x8
#define HWIO_DDR_TG0_TG_STREAM_WR_CFG0_DATA_PAT_BMSK                                       0xc0
#define HWIO_DDR_TG0_TG_STREAM_WR_CFG0_DATA_PAT_SHFT                                        0x6
#define HWIO_DDR_TG0_TG_STREAM_WR_CFG0_ADDRESS_PAT_BMSK                                    0x30
#define HWIO_DDR_TG0_TG_STREAM_WR_CFG0_ADDRESS_PAT_SHFT                                     0x4
#define HWIO_DDR_TG0_TG_STREAM_WR_CFG0_OPERATION_BMSK                                       0x3
#define HWIO_DDR_TG0_TG_STREAM_WR_CFG0_OPERATION_SHFT                                       0x0

#define HWIO_DDR_TG0_TG_STREAM_WR_CFG1_ADDR                                          (DDR_TG0_TG_REG_BASE      + 0x00000008)
#define HWIO_DDR_TG0_TG_STREAM_WR_CFG1_PHYS                                          (DDR_TG0_TG_REG_BASE_PHYS + 0x00000008)
#define HWIO_DDR_TG0_TG_STREAM_WR_CFG1_OFFS                                          (DDR_TG0_TG_REG_BASE_OFFS + 0x00000008)
#define HWIO_DDR_TG0_TG_STREAM_WR_CFG1_RMSK                                          0xffffffff
#define HWIO_DDR_TG0_TG_STREAM_WR_CFG1_IN          \
        in_dword_masked(HWIO_DDR_TG0_TG_STREAM_WR_CFG1_ADDR, HWIO_DDR_TG0_TG_STREAM_WR_CFG1_RMSK)
#define HWIO_DDR_TG0_TG_STREAM_WR_CFG1_INM(m)      \
        in_dword_masked(HWIO_DDR_TG0_TG_STREAM_WR_CFG1_ADDR, m)
#define HWIO_DDR_TG0_TG_STREAM_WR_CFG1_OUT(v)      \
        out_dword(HWIO_DDR_TG0_TG_STREAM_WR_CFG1_ADDR,v)
#define HWIO_DDR_TG0_TG_STREAM_WR_CFG1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DDR_TG0_TG_STREAM_WR_CFG1_ADDR,m,v,HWIO_DDR_TG0_TG_STREAM_WR_CFG1_IN)
#define HWIO_DDR_TG0_TG_STREAM_WR_CFG1_OLCNT_BMSK                                    0xffff0000
#define HWIO_DDR_TG0_TG_STREAM_WR_CFG1_OLCNT_SHFT                                          0x10
#define HWIO_DDR_TG0_TG_STREAM_WR_CFG1_ILCNT_BMSK                                        0xffff
#define HWIO_DDR_TG0_TG_STREAM_WR_CFG1_ILCNT_SHFT                                           0x0

#define HWIO_DDR_TG0_TG_STREAM_WR_CFG2_ADDR                                          (DDR_TG0_TG_REG_BASE      + 0x0000000c)
#define HWIO_DDR_TG0_TG_STREAM_WR_CFG2_PHYS                                          (DDR_TG0_TG_REG_BASE_PHYS + 0x0000000c)
#define HWIO_DDR_TG0_TG_STREAM_WR_CFG2_OFFS                                          (DDR_TG0_TG_REG_BASE_OFFS + 0x0000000c)
#define HWIO_DDR_TG0_TG_STREAM_WR_CFG2_RMSK                                          0xffffffff
#define HWIO_DDR_TG0_TG_STREAM_WR_CFG2_IN          \
        in_dword_masked(HWIO_DDR_TG0_TG_STREAM_WR_CFG2_ADDR, HWIO_DDR_TG0_TG_STREAM_WR_CFG2_RMSK)
#define HWIO_DDR_TG0_TG_STREAM_WR_CFG2_INM(m)      \
        in_dword_masked(HWIO_DDR_TG0_TG_STREAM_WR_CFG2_ADDR, m)
#define HWIO_DDR_TG0_TG_STREAM_WR_CFG2_OUT(v)      \
        out_dword(HWIO_DDR_TG0_TG_STREAM_WR_CFG2_ADDR,v)
#define HWIO_DDR_TG0_TG_STREAM_WR_CFG2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DDR_TG0_TG_STREAM_WR_CFG2_ADDR,m,v,HWIO_DDR_TG0_TG_STREAM_WR_CFG2_IN)
#define HWIO_DDR_TG0_TG_STREAM_WR_CFG2_SADDR_LOW_BMSK                                0xffffffff
#define HWIO_DDR_TG0_TG_STREAM_WR_CFG2_SADDR_LOW_SHFT                                       0x0

#define HWIO_DDR_TG0_TG_STREAM_WR_CFG3_ADDR                                          (DDR_TG0_TG_REG_BASE      + 0x00000010)
#define HWIO_DDR_TG0_TG_STREAM_WR_CFG3_PHYS                                          (DDR_TG0_TG_REG_BASE_PHYS + 0x00000010)
#define HWIO_DDR_TG0_TG_STREAM_WR_CFG3_OFFS                                          (DDR_TG0_TG_REG_BASE_OFFS + 0x00000010)
#define HWIO_DDR_TG0_TG_STREAM_WR_CFG3_RMSK                                              0xff0f
#define HWIO_DDR_TG0_TG_STREAM_WR_CFG3_IN          \
        in_dword_masked(HWIO_DDR_TG0_TG_STREAM_WR_CFG3_ADDR, HWIO_DDR_TG0_TG_STREAM_WR_CFG3_RMSK)
#define HWIO_DDR_TG0_TG_STREAM_WR_CFG3_INM(m)      \
        in_dword_masked(HWIO_DDR_TG0_TG_STREAM_WR_CFG3_ADDR, m)
#define HWIO_DDR_TG0_TG_STREAM_WR_CFG3_OUT(v)      \
        out_dword(HWIO_DDR_TG0_TG_STREAM_WR_CFG3_ADDR,v)
#define HWIO_DDR_TG0_TG_STREAM_WR_CFG3_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DDR_TG0_TG_STREAM_WR_CFG3_ADDR,m,v,HWIO_DDR_TG0_TG_STREAM_WR_CFG3_IN)
#define HWIO_DDR_TG0_TG_STREAM_WR_CFG3_STREAM_ID_BMSK                                    0xff00
#define HWIO_DDR_TG0_TG_STREAM_WR_CFG3_STREAM_ID_SHFT                                       0x8
#define HWIO_DDR_TG0_TG_STREAM_WR_CFG3_SADDR_HIGH_BMSK                                      0xf
#define HWIO_DDR_TG0_TG_STREAM_WR_CFG3_SADDR_HIGH_SHFT                                      0x0

#define HWIO_DDR_TG0_TG_STREAM_WR_CFG4_ADDR                                          (DDR_TG0_TG_REG_BASE      + 0x00000014)
#define HWIO_DDR_TG0_TG_STREAM_WR_CFG4_PHYS                                          (DDR_TG0_TG_REG_BASE_PHYS + 0x00000014)
#define HWIO_DDR_TG0_TG_STREAM_WR_CFG4_OFFS                                          (DDR_TG0_TG_REG_BASE_OFFS + 0x00000014)
#define HWIO_DDR_TG0_TG_STREAM_WR_CFG4_RMSK                                          0xffffffff
#define HWIO_DDR_TG0_TG_STREAM_WR_CFG4_IN          \
        in_dword_masked(HWIO_DDR_TG0_TG_STREAM_WR_CFG4_ADDR, HWIO_DDR_TG0_TG_STREAM_WR_CFG4_RMSK)
#define HWIO_DDR_TG0_TG_STREAM_WR_CFG4_INM(m)      \
        in_dword_masked(HWIO_DDR_TG0_TG_STREAM_WR_CFG4_ADDR, m)
#define HWIO_DDR_TG0_TG_STREAM_WR_CFG4_OUT(v)      \
        out_dword(HWIO_DDR_TG0_TG_STREAM_WR_CFG4_ADDR,v)
#define HWIO_DDR_TG0_TG_STREAM_WR_CFG4_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DDR_TG0_TG_STREAM_WR_CFG4_ADDR,m,v,HWIO_DDR_TG0_TG_STREAM_WR_CFG4_IN)
#define HWIO_DDR_TG0_TG_STREAM_WR_CFG4_DSEED_BMSK                                    0xffffffff
#define HWIO_DDR_TG0_TG_STREAM_WR_CFG4_DSEED_SHFT                                           0x0

#define HWIO_DDR_TG0_TG_STREAM_WR_QSB_CFG0_ADDR                                      (DDR_TG0_TG_REG_BASE      + 0x00000018)
#define HWIO_DDR_TG0_TG_STREAM_WR_QSB_CFG0_PHYS                                      (DDR_TG0_TG_REG_BASE_PHYS + 0x00000018)
#define HWIO_DDR_TG0_TG_STREAM_WR_QSB_CFG0_OFFS                                      (DDR_TG0_TG_REG_BASE_OFFS + 0x00000018)
#define HWIO_DDR_TG0_TG_STREAM_WR_QSB_CFG0_RMSK                                      0x3fffffff
#define HWIO_DDR_TG0_TG_STREAM_WR_QSB_CFG0_IN          \
        in_dword_masked(HWIO_DDR_TG0_TG_STREAM_WR_QSB_CFG0_ADDR, HWIO_DDR_TG0_TG_STREAM_WR_QSB_CFG0_RMSK)
#define HWIO_DDR_TG0_TG_STREAM_WR_QSB_CFG0_INM(m)      \
        in_dword_masked(HWIO_DDR_TG0_TG_STREAM_WR_QSB_CFG0_ADDR, m)
#define HWIO_DDR_TG0_TG_STREAM_WR_QSB_CFG0_OUT(v)      \
        out_dword(HWIO_DDR_TG0_TG_STREAM_WR_QSB_CFG0_ADDR,v)
#define HWIO_DDR_TG0_TG_STREAM_WR_QSB_CFG0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DDR_TG0_TG_STREAM_WR_QSB_CFG0_ADDR,m,v,HWIO_DDR_TG0_TG_STREAM_WR_QSB_CFG0_IN)
#define HWIO_DDR_TG0_TG_STREAM_WR_QSB_CFG0_AREQPRIORITY_BMSK                         0x38000000
#define HWIO_DDR_TG0_TG_STREAM_WR_QSB_CFG0_AREQPRIORITY_SHFT                               0x1b
#define HWIO_DDR_TG0_TG_STREAM_WR_QSB_CFG0_APROTNS_BMSK                               0x4000000
#define HWIO_DDR_TG0_TG_STREAM_WR_QSB_CFG0_APROTNS_SHFT                                    0x1a
#define HWIO_DDR_TG0_TG_STREAM_WR_QSB_CFG0_AMSSSELFAUTH_BMSK                          0x2000000
#define HWIO_DDR_TG0_TG_STREAM_WR_QSB_CFG0_AMSSSELFAUTH_SHFT                               0x19
#define HWIO_DDR_TG0_TG_STREAM_WR_QSB_CFG0_ANOALLOCATE_BMSK                           0x1000000
#define HWIO_DDR_TG0_TG_STREAM_WR_QSB_CFG0_ANOALLOCATE_SHFT                                0x18
#define HWIO_DDR_TG0_TG_STREAM_WR_QSB_CFG0_AWRITETHROUGH_BMSK                          0x800000
#define HWIO_DDR_TG0_TG_STREAM_WR_QSB_CFG0_AWRITETHROUGH_SHFT                              0x17
#define HWIO_DDR_TG0_TG_STREAM_WR_QSB_CFG0_AINNERCACHEABLE_BMSK                        0x400000
#define HWIO_DDR_TG0_TG_STREAM_WR_QSB_CFG0_AINNERCACHEABLE_SHFT                            0x16
#define HWIO_DDR_TG0_TG_STREAM_WR_QSB_CFG0_ASHARED_BMSK                                0x200000
#define HWIO_DDR_TG0_TG_STREAM_WR_QSB_CFG0_ASHARED_SHFT                                    0x15
#define HWIO_DDR_TG0_TG_STREAM_WR_QSB_CFG0_AINNERSHARED_BMSK                           0x100000
#define HWIO_DDR_TG0_TG_STREAM_WR_QSB_CFG0_AINNERSHARED_SHFT                               0x14
#define HWIO_DDR_TG0_TG_STREAM_WR_QSB_CFG0_ADEVICETYPE_BMSK                             0x80000
#define HWIO_DDR_TG0_TG_STREAM_WR_QSB_CFG0_ADEVICETYPE_SHFT                                0x13
#define HWIO_DDR_TG0_TG_STREAM_WR_QSB_CFG0_ADEVICE_BMSK                                 0x40000
#define HWIO_DDR_TG0_TG_STREAM_WR_QSB_CFG0_ADEVICE_SHFT                                    0x12
#define HWIO_DDR_TG0_TG_STREAM_WR_QSB_CFG0_AEXCLUSIVE_BMSK                              0x20000
#define HWIO_DDR_TG0_TG_STREAM_WR_QSB_CFG0_AEXCLUSIVE_SHFT                                 0x11
#define HWIO_DDR_TG0_TG_STREAM_WR_QSB_CFG0_ASIZE_BMSK                                   0x1c000
#define HWIO_DDR_TG0_TG_STREAM_WR_QSB_CFG0_ASIZE_SHFT                                       0xe
#define HWIO_DDR_TG0_TG_STREAM_WR_QSB_CFG0_ALEN_BMSK                                     0x3c00
#define HWIO_DDR_TG0_TG_STREAM_WR_QSB_CFG0_ALEN_SHFT                                        0xa
#define HWIO_DDR_TG0_TG_STREAM_WR_QSB_CFG0_AGFO_BMSK                                      0x200
#define HWIO_DDR_TG0_TG_STREAM_WR_QSB_CFG0_AGFO_SHFT                                        0x9
#define HWIO_DDR_TG0_TG_STREAM_WR_QSB_CFG0_ASCID_BMSK                                     0x1f0
#define HWIO_DDR_TG0_TG_STREAM_WR_QSB_CFG0_ASCID_SHFT                                       0x4
#define HWIO_DDR_TG0_TG_STREAM_WR_QSB_CFG0_ACACHEOPTYPE_BMSK                                0xf
#define HWIO_DDR_TG0_TG_STREAM_WR_QSB_CFG0_ACACHEOPTYPE_SHFT                                0x0

#define HWIO_DDR_TG0_TG_STREAM_WR_QSB_CFG1_ADDR                                      (DDR_TG0_TG_REG_BASE      + 0x0000001c)
#define HWIO_DDR_TG0_TG_STREAM_WR_QSB_CFG1_PHYS                                      (DDR_TG0_TG_REG_BASE_PHYS + 0x0000001c)
#define HWIO_DDR_TG0_TG_STREAM_WR_QSB_CFG1_OFFS                                      (DDR_TG0_TG_REG_BASE_OFFS + 0x0000001c)
#define HWIO_DDR_TG0_TG_STREAM_WR_QSB_CFG1_RMSK                                             0x3
#define HWIO_DDR_TG0_TG_STREAM_WR_QSB_CFG1_IN          \
        in_dword_masked(HWIO_DDR_TG0_TG_STREAM_WR_QSB_CFG1_ADDR, HWIO_DDR_TG0_TG_STREAM_WR_QSB_CFG1_RMSK)
#define HWIO_DDR_TG0_TG_STREAM_WR_QSB_CFG1_INM(m)      \
        in_dword_masked(HWIO_DDR_TG0_TG_STREAM_WR_QSB_CFG1_ADDR, m)
#define HWIO_DDR_TG0_TG_STREAM_WR_QSB_CFG1_OUT(v)      \
        out_dword(HWIO_DDR_TG0_TG_STREAM_WR_QSB_CFG1_ADDR,v)
#define HWIO_DDR_TG0_TG_STREAM_WR_QSB_CFG1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DDR_TG0_TG_STREAM_WR_QSB_CFG1_ADDR,m,v,HWIO_DDR_TG0_TG_STREAM_WR_QSB_CFG1_IN)
#define HWIO_DDR_TG0_TG_STREAM_WR_QSB_CFG1_ACACHEABLE_BMSK                                  0x2
#define HWIO_DDR_TG0_TG_STREAM_WR_QSB_CFG1_ACACHEABLE_SHFT                                  0x1
#define HWIO_DDR_TG0_TG_STREAM_WR_QSB_CFG1_ATRANSIENT_BMSK                                  0x1
#define HWIO_DDR_TG0_TG_STREAM_WR_QSB_CFG1_ATRANSIENT_SHFT                                  0x0

#define HWIO_DDR_TG0_TG_STREAM_WR_STATUS0_ADDR                                       (DDR_TG0_TG_REG_BASE      + 0x00000024)
#define HWIO_DDR_TG0_TG_STREAM_WR_STATUS0_PHYS                                       (DDR_TG0_TG_REG_BASE_PHYS + 0x00000024)
#define HWIO_DDR_TG0_TG_STREAM_WR_STATUS0_OFFS                                       (DDR_TG0_TG_REG_BASE_OFFS + 0x00000024)
#define HWIO_DDR_TG0_TG_STREAM_WR_STATUS0_RMSK                                              0x1
#define HWIO_DDR_TG0_TG_STREAM_WR_STATUS0_IN          \
        in_dword_masked(HWIO_DDR_TG0_TG_STREAM_WR_STATUS0_ADDR, HWIO_DDR_TG0_TG_STREAM_WR_STATUS0_RMSK)
#define HWIO_DDR_TG0_TG_STREAM_WR_STATUS0_INM(m)      \
        in_dword_masked(HWIO_DDR_TG0_TG_STREAM_WR_STATUS0_ADDR, m)
#define HWIO_DDR_TG0_TG_STREAM_WR_STATUS0_DONE_BMSK                                         0x1
#define HWIO_DDR_TG0_TG_STREAM_WR_STATUS0_DONE_SHFT                                         0x0

#define HWIO_DDR_TG0_TG_STREAM_RD_OPERATION_ADDR                                     (DDR_TG0_TG_REG_BASE      + 0x00000028)
#define HWIO_DDR_TG0_TG_STREAM_RD_OPERATION_PHYS                                     (DDR_TG0_TG_REG_BASE_PHYS + 0x00000028)
#define HWIO_DDR_TG0_TG_STREAM_RD_OPERATION_OFFS                                     (DDR_TG0_TG_REG_BASE_OFFS + 0x00000028)
#define HWIO_DDR_TG0_TG_STREAM_RD_OPERATION_RMSK                                            0x1
#define HWIO_DDR_TG0_TG_STREAM_RD_OPERATION_IN          \
        in_dword_masked(HWIO_DDR_TG0_TG_STREAM_RD_OPERATION_ADDR, HWIO_DDR_TG0_TG_STREAM_RD_OPERATION_RMSK)
#define HWIO_DDR_TG0_TG_STREAM_RD_OPERATION_INM(m)      \
        in_dword_masked(HWIO_DDR_TG0_TG_STREAM_RD_OPERATION_ADDR, m)
#define HWIO_DDR_TG0_TG_STREAM_RD_OPERATION_OUT(v)      \
        out_dword(HWIO_DDR_TG0_TG_STREAM_RD_OPERATION_ADDR,v)
#define HWIO_DDR_TG0_TG_STREAM_RD_OPERATION_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DDR_TG0_TG_STREAM_RD_OPERATION_ADDR,m,v,HWIO_DDR_TG0_TG_STREAM_RD_OPERATION_IN)
#define HWIO_DDR_TG0_TG_STREAM_RD_OPERATION_TRIGGER_BMSK                                    0x1
#define HWIO_DDR_TG0_TG_STREAM_RD_OPERATION_TRIGGER_SHFT                                    0x0

#define HWIO_DDR_TG0_TG_STREAM_RD_CFG0_ADDR                                          (DDR_TG0_TG_REG_BASE      + 0x0000002c)
#define HWIO_DDR_TG0_TG_STREAM_RD_CFG0_PHYS                                          (DDR_TG0_TG_REG_BASE_PHYS + 0x0000002c)
#define HWIO_DDR_TG0_TG_STREAM_RD_CFG0_OFFS                                          (DDR_TG0_TG_REG_BASE_OFFS + 0x0000002c)
#define HWIO_DDR_TG0_TG_STREAM_RD_CFG0_RMSK                                          0xfffffff3
#define HWIO_DDR_TG0_TG_STREAM_RD_CFG0_IN          \
        in_dword_masked(HWIO_DDR_TG0_TG_STREAM_RD_CFG0_ADDR, HWIO_DDR_TG0_TG_STREAM_RD_CFG0_RMSK)
#define HWIO_DDR_TG0_TG_STREAM_RD_CFG0_INM(m)      \
        in_dword_masked(HWIO_DDR_TG0_TG_STREAM_RD_CFG0_ADDR, m)
#define HWIO_DDR_TG0_TG_STREAM_RD_CFG0_OUT(v)      \
        out_dword(HWIO_DDR_TG0_TG_STREAM_RD_CFG0_ADDR,v)
#define HWIO_DDR_TG0_TG_STREAM_RD_CFG0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DDR_TG0_TG_STREAM_RD_CFG0_ADDR,m,v,HWIO_DDR_TG0_TG_STREAM_RD_CFG0_IN)
#define HWIO_DDR_TG0_TG_STREAM_RD_CFG0_MISR_RD_EN_BMSK                               0x80000000
#define HWIO_DDR_TG0_TG_STREAM_RD_CFG0_MISR_RD_EN_SHFT                                     0x1f
#define HWIO_DDR_TG0_TG_STREAM_RD_CFG0_STOP_ON_ERROR_BMSK                            0x40000000
#define HWIO_DDR_TG0_TG_STREAM_RD_CFG0_STOP_ON_ERROR_SHFT                                  0x1e
#define HWIO_DDR_TG0_TG_STREAM_RD_CFG0_ILOOP_BMSK                                    0x20000000
#define HWIO_DDR_TG0_TG_STREAM_RD_CFG0_ILOOP_SHFT                                          0x1d
#define HWIO_DDR_TG0_TG_STREAM_RD_CFG0_OTCNT_BMSK                                    0x1f000000
#define HWIO_DDR_TG0_TG_STREAM_RD_CFG0_OTCNT_SHFT                                          0x18
#define HWIO_DDR_TG0_TG_STREAM_RD_CFG0_IAG_BMSK                                        0xff0000
#define HWIO_DDR_TG0_TG_STREAM_RD_CFG0_IAG_SHFT                                            0x10
#define HWIO_DDR_TG0_TG_STREAM_RD_CFG0_STRIDE_BMSK                                       0xff00
#define HWIO_DDR_TG0_TG_STREAM_RD_CFG0_STRIDE_SHFT                                          0x8
#define HWIO_DDR_TG0_TG_STREAM_RD_CFG0_DATA_PAT_BMSK                                       0xc0
#define HWIO_DDR_TG0_TG_STREAM_RD_CFG0_DATA_PAT_SHFT                                        0x6
#define HWIO_DDR_TG0_TG_STREAM_RD_CFG0_ADDRESS_PAT_BMSK                                    0x30
#define HWIO_DDR_TG0_TG_STREAM_RD_CFG0_ADDRESS_PAT_SHFT                                     0x4
#define HWIO_DDR_TG0_TG_STREAM_RD_CFG0_OPERATION_BMSK                                       0x3
#define HWIO_DDR_TG0_TG_STREAM_RD_CFG0_OPERATION_SHFT                                       0x0

#define HWIO_DDR_TG0_TG_STREAM_RD_CFG1_ADDR                                          (DDR_TG0_TG_REG_BASE      + 0x00000030)
#define HWIO_DDR_TG0_TG_STREAM_RD_CFG1_PHYS                                          (DDR_TG0_TG_REG_BASE_PHYS + 0x00000030)
#define HWIO_DDR_TG0_TG_STREAM_RD_CFG1_OFFS                                          (DDR_TG0_TG_REG_BASE_OFFS + 0x00000030)
#define HWIO_DDR_TG0_TG_STREAM_RD_CFG1_RMSK                                          0xffffffff
#define HWIO_DDR_TG0_TG_STREAM_RD_CFG1_IN          \
        in_dword_masked(HWIO_DDR_TG0_TG_STREAM_RD_CFG1_ADDR, HWIO_DDR_TG0_TG_STREAM_RD_CFG1_RMSK)
#define HWIO_DDR_TG0_TG_STREAM_RD_CFG1_INM(m)      \
        in_dword_masked(HWIO_DDR_TG0_TG_STREAM_RD_CFG1_ADDR, m)
#define HWIO_DDR_TG0_TG_STREAM_RD_CFG1_OUT(v)      \
        out_dword(HWIO_DDR_TG0_TG_STREAM_RD_CFG1_ADDR,v)
#define HWIO_DDR_TG0_TG_STREAM_RD_CFG1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DDR_TG0_TG_STREAM_RD_CFG1_ADDR,m,v,HWIO_DDR_TG0_TG_STREAM_RD_CFG1_IN)
#define HWIO_DDR_TG0_TG_STREAM_RD_CFG1_OLCNT_BMSK                                    0xffff0000
#define HWIO_DDR_TG0_TG_STREAM_RD_CFG1_OLCNT_SHFT                                          0x10
#define HWIO_DDR_TG0_TG_STREAM_RD_CFG1_ILCNT_BMSK                                        0xffff
#define HWIO_DDR_TG0_TG_STREAM_RD_CFG1_ILCNT_SHFT                                           0x0

#define HWIO_DDR_TG0_TG_STREAM_RD_CFG2_ADDR                                          (DDR_TG0_TG_REG_BASE      + 0x00000034)
#define HWIO_DDR_TG0_TG_STREAM_RD_CFG2_PHYS                                          (DDR_TG0_TG_REG_BASE_PHYS + 0x00000034)
#define HWIO_DDR_TG0_TG_STREAM_RD_CFG2_OFFS                                          (DDR_TG0_TG_REG_BASE_OFFS + 0x00000034)
#define HWIO_DDR_TG0_TG_STREAM_RD_CFG2_RMSK                                          0xffffffff
#define HWIO_DDR_TG0_TG_STREAM_RD_CFG2_IN          \
        in_dword_masked(HWIO_DDR_TG0_TG_STREAM_RD_CFG2_ADDR, HWIO_DDR_TG0_TG_STREAM_RD_CFG2_RMSK)
#define HWIO_DDR_TG0_TG_STREAM_RD_CFG2_INM(m)      \
        in_dword_masked(HWIO_DDR_TG0_TG_STREAM_RD_CFG2_ADDR, m)
#define HWIO_DDR_TG0_TG_STREAM_RD_CFG2_OUT(v)      \
        out_dword(HWIO_DDR_TG0_TG_STREAM_RD_CFG2_ADDR,v)
#define HWIO_DDR_TG0_TG_STREAM_RD_CFG2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DDR_TG0_TG_STREAM_RD_CFG2_ADDR,m,v,HWIO_DDR_TG0_TG_STREAM_RD_CFG2_IN)
#define HWIO_DDR_TG0_TG_STREAM_RD_CFG2_SADDR_LOW_BMSK                                0xffffffff
#define HWIO_DDR_TG0_TG_STREAM_RD_CFG2_SADDR_LOW_SHFT                                       0x0

#define HWIO_DDR_TG0_TG_STREAM_RD_CFG3_ADDR                                          (DDR_TG0_TG_REG_BASE      + 0x00000038)
#define HWIO_DDR_TG0_TG_STREAM_RD_CFG3_PHYS                                          (DDR_TG0_TG_REG_BASE_PHYS + 0x00000038)
#define HWIO_DDR_TG0_TG_STREAM_RD_CFG3_OFFS                                          (DDR_TG0_TG_REG_BASE_OFFS + 0x00000038)
#define HWIO_DDR_TG0_TG_STREAM_RD_CFG3_RMSK                                              0xff0f
#define HWIO_DDR_TG0_TG_STREAM_RD_CFG3_IN          \
        in_dword_masked(HWIO_DDR_TG0_TG_STREAM_RD_CFG3_ADDR, HWIO_DDR_TG0_TG_STREAM_RD_CFG3_RMSK)
#define HWIO_DDR_TG0_TG_STREAM_RD_CFG3_INM(m)      \
        in_dword_masked(HWIO_DDR_TG0_TG_STREAM_RD_CFG3_ADDR, m)
#define HWIO_DDR_TG0_TG_STREAM_RD_CFG3_OUT(v)      \
        out_dword(HWIO_DDR_TG0_TG_STREAM_RD_CFG3_ADDR,v)
#define HWIO_DDR_TG0_TG_STREAM_RD_CFG3_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DDR_TG0_TG_STREAM_RD_CFG3_ADDR,m,v,HWIO_DDR_TG0_TG_STREAM_RD_CFG3_IN)
#define HWIO_DDR_TG0_TG_STREAM_RD_CFG3_STREAM_ID_BMSK                                    0xff00
#define HWIO_DDR_TG0_TG_STREAM_RD_CFG3_STREAM_ID_SHFT                                       0x8
#define HWIO_DDR_TG0_TG_STREAM_RD_CFG3_SADDR_HIGH_BMSK                                      0xf
#define HWIO_DDR_TG0_TG_STREAM_RD_CFG3_SADDR_HIGH_SHFT                                      0x0

#define HWIO_DDR_TG0_TG_STREAM_RD_CFG4_ADDR                                          (DDR_TG0_TG_REG_BASE      + 0x0000003c)
#define HWIO_DDR_TG0_TG_STREAM_RD_CFG4_PHYS                                          (DDR_TG0_TG_REG_BASE_PHYS + 0x0000003c)
#define HWIO_DDR_TG0_TG_STREAM_RD_CFG4_OFFS                                          (DDR_TG0_TG_REG_BASE_OFFS + 0x0000003c)
#define HWIO_DDR_TG0_TG_STREAM_RD_CFG4_RMSK                                          0xffffffff
#define HWIO_DDR_TG0_TG_STREAM_RD_CFG4_IN          \
        in_dword_masked(HWIO_DDR_TG0_TG_STREAM_RD_CFG4_ADDR, HWIO_DDR_TG0_TG_STREAM_RD_CFG4_RMSK)
#define HWIO_DDR_TG0_TG_STREAM_RD_CFG4_INM(m)      \
        in_dword_masked(HWIO_DDR_TG0_TG_STREAM_RD_CFG4_ADDR, m)
#define HWIO_DDR_TG0_TG_STREAM_RD_CFG4_OUT(v)      \
        out_dword(HWIO_DDR_TG0_TG_STREAM_RD_CFG4_ADDR,v)
#define HWIO_DDR_TG0_TG_STREAM_RD_CFG4_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DDR_TG0_TG_STREAM_RD_CFG4_ADDR,m,v,HWIO_DDR_TG0_TG_STREAM_RD_CFG4_IN)
#define HWIO_DDR_TG0_TG_STREAM_RD_CFG4_DSEED_BMSK                                    0xffffffff
#define HWIO_DDR_TG0_TG_STREAM_RD_CFG4_DSEED_SHFT                                           0x0

#define HWIO_DDR_TG0_TG_STREAM_RD_QSB_CFG0_ADDR                                      (DDR_TG0_TG_REG_BASE      + 0x00000040)
#define HWIO_DDR_TG0_TG_STREAM_RD_QSB_CFG0_PHYS                                      (DDR_TG0_TG_REG_BASE_PHYS + 0x00000040)
#define HWIO_DDR_TG0_TG_STREAM_RD_QSB_CFG0_OFFS                                      (DDR_TG0_TG_REG_BASE_OFFS + 0x00000040)
#define HWIO_DDR_TG0_TG_STREAM_RD_QSB_CFG0_RMSK                                      0x3fffffff
#define HWIO_DDR_TG0_TG_STREAM_RD_QSB_CFG0_IN          \
        in_dword_masked(HWIO_DDR_TG0_TG_STREAM_RD_QSB_CFG0_ADDR, HWIO_DDR_TG0_TG_STREAM_RD_QSB_CFG0_RMSK)
#define HWIO_DDR_TG0_TG_STREAM_RD_QSB_CFG0_INM(m)      \
        in_dword_masked(HWIO_DDR_TG0_TG_STREAM_RD_QSB_CFG0_ADDR, m)
#define HWIO_DDR_TG0_TG_STREAM_RD_QSB_CFG0_OUT(v)      \
        out_dword(HWIO_DDR_TG0_TG_STREAM_RD_QSB_CFG0_ADDR,v)
#define HWIO_DDR_TG0_TG_STREAM_RD_QSB_CFG0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DDR_TG0_TG_STREAM_RD_QSB_CFG0_ADDR,m,v,HWIO_DDR_TG0_TG_STREAM_RD_QSB_CFG0_IN)
#define HWIO_DDR_TG0_TG_STREAM_RD_QSB_CFG0_AREQPRIORITY_BMSK                         0x38000000
#define HWIO_DDR_TG0_TG_STREAM_RD_QSB_CFG0_AREQPRIORITY_SHFT                               0x1b
#define HWIO_DDR_TG0_TG_STREAM_RD_QSB_CFG0_APROTNS_BMSK                               0x4000000
#define HWIO_DDR_TG0_TG_STREAM_RD_QSB_CFG0_APROTNS_SHFT                                    0x1a
#define HWIO_DDR_TG0_TG_STREAM_RD_QSB_CFG0_AMSSSELFAUTH_BMSK                          0x2000000
#define HWIO_DDR_TG0_TG_STREAM_RD_QSB_CFG0_AMSSSELFAUTH_SHFT                               0x19
#define HWIO_DDR_TG0_TG_STREAM_RD_QSB_CFG0_ANOALLOCATE_BMSK                           0x1000000
#define HWIO_DDR_TG0_TG_STREAM_RD_QSB_CFG0_ANOALLOCATE_SHFT                                0x18
#define HWIO_DDR_TG0_TG_STREAM_RD_QSB_CFG0_AWRITETHROUGH_BMSK                          0x800000
#define HWIO_DDR_TG0_TG_STREAM_RD_QSB_CFG0_AWRITETHROUGH_SHFT                              0x17
#define HWIO_DDR_TG0_TG_STREAM_RD_QSB_CFG0_AINNERCACHEABLE_BMSK                        0x400000
#define HWIO_DDR_TG0_TG_STREAM_RD_QSB_CFG0_AINNERCACHEABLE_SHFT                            0x16
#define HWIO_DDR_TG0_TG_STREAM_RD_QSB_CFG0_ASHARED_BMSK                                0x200000
#define HWIO_DDR_TG0_TG_STREAM_RD_QSB_CFG0_ASHARED_SHFT                                    0x15
#define HWIO_DDR_TG0_TG_STREAM_RD_QSB_CFG0_AINNERSHARED_BMSK                           0x100000
#define HWIO_DDR_TG0_TG_STREAM_RD_QSB_CFG0_AINNERSHARED_SHFT                               0x14
#define HWIO_DDR_TG0_TG_STREAM_RD_QSB_CFG0_ADEVICETYPE_BMSK                             0x80000
#define HWIO_DDR_TG0_TG_STREAM_RD_QSB_CFG0_ADEVICETYPE_SHFT                                0x13
#define HWIO_DDR_TG0_TG_STREAM_RD_QSB_CFG0_ADEVICE_BMSK                                 0x40000
#define HWIO_DDR_TG0_TG_STREAM_RD_QSB_CFG0_ADEVICE_SHFT                                    0x12
#define HWIO_DDR_TG0_TG_STREAM_RD_QSB_CFG0_AEXCLUSIVE_BMSK                              0x20000
#define HWIO_DDR_TG0_TG_STREAM_RD_QSB_CFG0_AEXCLUSIVE_SHFT                                 0x11
#define HWIO_DDR_TG0_TG_STREAM_RD_QSB_CFG0_ASIZE_BMSK                                   0x1c000
#define HWIO_DDR_TG0_TG_STREAM_RD_QSB_CFG0_ASIZE_SHFT                                       0xe
#define HWIO_DDR_TG0_TG_STREAM_RD_QSB_CFG0_ALEN_BMSK                                     0x3c00
#define HWIO_DDR_TG0_TG_STREAM_RD_QSB_CFG0_ALEN_SHFT                                        0xa
#define HWIO_DDR_TG0_TG_STREAM_RD_QSB_CFG0_AGFO_BMSK                                      0x200
#define HWIO_DDR_TG0_TG_STREAM_RD_QSB_CFG0_AGFO_SHFT                                        0x9
#define HWIO_DDR_TG0_TG_STREAM_RD_QSB_CFG0_ASCID_BMSK                                     0x1f0
#define HWIO_DDR_TG0_TG_STREAM_RD_QSB_CFG0_ASCID_SHFT                                       0x4
#define HWIO_DDR_TG0_TG_STREAM_RD_QSB_CFG0_ACACHEOPTYPE_BMSK                                0xf
#define HWIO_DDR_TG0_TG_STREAM_RD_QSB_CFG0_ACACHEOPTYPE_SHFT                                0x0

#define HWIO_DDR_TG0_TG_STREAM_RD_QSB_CFG1_ADDR                                      (DDR_TG0_TG_REG_BASE      + 0x00000044)
#define HWIO_DDR_TG0_TG_STREAM_RD_QSB_CFG1_PHYS                                      (DDR_TG0_TG_REG_BASE_PHYS + 0x00000044)
#define HWIO_DDR_TG0_TG_STREAM_RD_QSB_CFG1_OFFS                                      (DDR_TG0_TG_REG_BASE_OFFS + 0x00000044)
#define HWIO_DDR_TG0_TG_STREAM_RD_QSB_CFG1_RMSK                                             0x3
#define HWIO_DDR_TG0_TG_STREAM_RD_QSB_CFG1_IN          \
        in_dword_masked(HWIO_DDR_TG0_TG_STREAM_RD_QSB_CFG1_ADDR, HWIO_DDR_TG0_TG_STREAM_RD_QSB_CFG1_RMSK)
#define HWIO_DDR_TG0_TG_STREAM_RD_QSB_CFG1_INM(m)      \
        in_dword_masked(HWIO_DDR_TG0_TG_STREAM_RD_QSB_CFG1_ADDR, m)
#define HWIO_DDR_TG0_TG_STREAM_RD_QSB_CFG1_OUT(v)      \
        out_dword(HWIO_DDR_TG0_TG_STREAM_RD_QSB_CFG1_ADDR,v)
#define HWIO_DDR_TG0_TG_STREAM_RD_QSB_CFG1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DDR_TG0_TG_STREAM_RD_QSB_CFG1_ADDR,m,v,HWIO_DDR_TG0_TG_STREAM_RD_QSB_CFG1_IN)
#define HWIO_DDR_TG0_TG_STREAM_RD_QSB_CFG1_ACACHEABLE_BMSK                                  0x2
#define HWIO_DDR_TG0_TG_STREAM_RD_QSB_CFG1_ACACHEABLE_SHFT                                  0x1
#define HWIO_DDR_TG0_TG_STREAM_RD_QSB_CFG1_ATRANSIENT_BMSK                                  0x1
#define HWIO_DDR_TG0_TG_STREAM_RD_QSB_CFG1_ATRANSIENT_SHFT                                  0x0

#define HWIO_DDR_TG0_TG_STREAM_RD_STATUS0_ADDR                                       (DDR_TG0_TG_REG_BASE      + 0x00000048)
#define HWIO_DDR_TG0_TG_STREAM_RD_STATUS0_PHYS                                       (DDR_TG0_TG_REG_BASE_PHYS + 0x00000048)
#define HWIO_DDR_TG0_TG_STREAM_RD_STATUS0_OFFS                                       (DDR_TG0_TG_REG_BASE_OFFS + 0x00000048)
#define HWIO_DDR_TG0_TG_STREAM_RD_STATUS0_RMSK                                              0x1
#define HWIO_DDR_TG0_TG_STREAM_RD_STATUS0_IN          \
        in_dword_masked(HWIO_DDR_TG0_TG_STREAM_RD_STATUS0_ADDR, HWIO_DDR_TG0_TG_STREAM_RD_STATUS0_RMSK)
#define HWIO_DDR_TG0_TG_STREAM_RD_STATUS0_INM(m)      \
        in_dword_masked(HWIO_DDR_TG0_TG_STREAM_RD_STATUS0_ADDR, m)
#define HWIO_DDR_TG0_TG_STREAM_RD_STATUS0_DONE_BMSK                                         0x1
#define HWIO_DDR_TG0_TG_STREAM_RD_STATUS0_DONE_SHFT                                         0x0

#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS0_ADDR                                  (DDR_TG0_TG_REG_BASE      + 0x0000004c)
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS0_PHYS                                  (DDR_TG0_TG_REG_BASE_PHYS + 0x0000004c)
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS0_OFFS                                  (DDR_TG0_TG_REG_BASE_OFFS + 0x0000004c)
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS0_RMSK                                   0x1ff3f0f
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS0_IN          \
        in_dword_masked(HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS0_ADDR, HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS0_RMSK)
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS0_INM(m)      \
        in_dword_masked(HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS0_ADDR, m)
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS0_RD_FAIL_BMSK                           0x1000000
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS0_RD_FAIL_SHFT                                0x18
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS0_RD_FAIL_MID_BMSK                        0xff0000
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS0_RD_FAIL_MID_SHFT                            0x10
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS0_RD_FAIL_TID_BMSK                          0x3f00
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS0_RD_FAIL_TID_SHFT                             0x8
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS0_RD_FAIL_BITS_BMSK                            0xf
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS0_RD_FAIL_BITS_SHFT                            0x0

#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS1_ADDR                                  (DDR_TG0_TG_REG_BASE      + 0x00000050)
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS1_PHYS                                  (DDR_TG0_TG_REG_BASE_PHYS + 0x00000050)
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS1_OFFS                                  (DDR_TG0_TG_REG_BASE_OFFS + 0x00000050)
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS1_RMSK                                  0xffffffff
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS1_IN          \
        in_dword_masked(HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS1_ADDR, HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS1_RMSK)
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS1_INM(m)      \
        in_dword_masked(HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS1_ADDR, m)
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS1_RD_DATA_EXP_31_0_BMSK                 0xffffffff
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS1_RD_DATA_EXP_31_0_SHFT                        0x0

#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS2_ADDR                                  (DDR_TG0_TG_REG_BASE      + 0x00000054)
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS2_PHYS                                  (DDR_TG0_TG_REG_BASE_PHYS + 0x00000054)
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS2_OFFS                                  (DDR_TG0_TG_REG_BASE_OFFS + 0x00000054)
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS2_RMSK                                  0xffffffff
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS2_IN          \
        in_dword_masked(HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS2_ADDR, HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS2_RMSK)
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS2_INM(m)      \
        in_dword_masked(HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS2_ADDR, m)
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS2_RD_DATA_EXP_63_32_BMSK                0xffffffff
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS2_RD_DATA_EXP_63_32_SHFT                       0x0

#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS3_ADDR                                  (DDR_TG0_TG_REG_BASE      + 0x00000058)
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS3_PHYS                                  (DDR_TG0_TG_REG_BASE_PHYS + 0x00000058)
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS3_OFFS                                  (DDR_TG0_TG_REG_BASE_OFFS + 0x00000058)
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS3_RMSK                                  0xffffffff
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS3_IN          \
        in_dword_masked(HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS3_ADDR, HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS3_RMSK)
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS3_INM(m)      \
        in_dword_masked(HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS3_ADDR, m)
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS3_RD_DATA_EXP_95_64_BMSK                0xffffffff
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS3_RD_DATA_EXP_95_64_SHFT                       0x0

#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS4_ADDR                                  (DDR_TG0_TG_REG_BASE      + 0x0000005c)
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS4_PHYS                                  (DDR_TG0_TG_REG_BASE_PHYS + 0x0000005c)
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS4_OFFS                                  (DDR_TG0_TG_REG_BASE_OFFS + 0x0000005c)
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS4_RMSK                                  0xffffffff
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS4_IN          \
        in_dword_masked(HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS4_ADDR, HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS4_RMSK)
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS4_INM(m)      \
        in_dword_masked(HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS4_ADDR, m)
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS4_RD_DATA_EXP_127_96_BMSK               0xffffffff
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS4_RD_DATA_EXP_127_96_SHFT                      0x0

#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS5_ADDR                                  (DDR_TG0_TG_REG_BASE      + 0x00000060)
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS5_PHYS                                  (DDR_TG0_TG_REG_BASE_PHYS + 0x00000060)
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS5_OFFS                                  (DDR_TG0_TG_REG_BASE_OFFS + 0x00000060)
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS5_RMSK                                  0xffffffff
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS5_IN          \
        in_dword_masked(HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS5_ADDR, HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS5_RMSK)
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS5_INM(m)      \
        in_dword_masked(HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS5_ADDR, m)
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS5_RD_DATA_EXP_159_128_BMSK              0xffffffff
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS5_RD_DATA_EXP_159_128_SHFT                     0x0

#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS6_ADDR                                  (DDR_TG0_TG_REG_BASE      + 0x00000064)
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS6_PHYS                                  (DDR_TG0_TG_REG_BASE_PHYS + 0x00000064)
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS6_OFFS                                  (DDR_TG0_TG_REG_BASE_OFFS + 0x00000064)
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS6_RMSK                                  0xffffffff
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS6_IN          \
        in_dword_masked(HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS6_ADDR, HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS6_RMSK)
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS6_INM(m)      \
        in_dword_masked(HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS6_ADDR, m)
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS6_RD_DATA_EXP_191_160_BMSK              0xffffffff
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS6_RD_DATA_EXP_191_160_SHFT                     0x0

#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS7_ADDR                                  (DDR_TG0_TG_REG_BASE      + 0x00000068)
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS7_PHYS                                  (DDR_TG0_TG_REG_BASE_PHYS + 0x00000068)
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS7_OFFS                                  (DDR_TG0_TG_REG_BASE_OFFS + 0x00000068)
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS7_RMSK                                  0xffffffff
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS7_IN          \
        in_dword_masked(HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS7_ADDR, HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS7_RMSK)
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS7_INM(m)      \
        in_dword_masked(HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS7_ADDR, m)
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS7_RD_DATA_EXP_223_192_BMSK              0xffffffff
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS7_RD_DATA_EXP_223_192_SHFT                     0x0

#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS8_ADDR                                  (DDR_TG0_TG_REG_BASE      + 0x0000006c)
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS8_PHYS                                  (DDR_TG0_TG_REG_BASE_PHYS + 0x0000006c)
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS8_OFFS                                  (DDR_TG0_TG_REG_BASE_OFFS + 0x0000006c)
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS8_RMSK                                  0xffffffff
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS8_IN          \
        in_dword_masked(HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS8_ADDR, HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS8_RMSK)
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS8_INM(m)      \
        in_dword_masked(HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS8_ADDR, m)
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS8_RD_DATA_EXP_255_224_BMSK              0xffffffff
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS8_RD_DATA_EXP_255_224_SHFT                     0x0

#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS9_ADDR                                  (DDR_TG0_TG_REG_BASE      + 0x00000070)
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS9_PHYS                                  (DDR_TG0_TG_REG_BASE_PHYS + 0x00000070)
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS9_OFFS                                  (DDR_TG0_TG_REG_BASE_OFFS + 0x00000070)
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS9_RMSK                                  0xffffffff
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS9_IN          \
        in_dword_masked(HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS9_ADDR, HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS9_RMSK)
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS9_INM(m)      \
        in_dword_masked(HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS9_ADDR, m)
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS9_RD_DATA_RCV_31_0_BMSK                 0xffffffff
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS9_RD_DATA_RCV_31_0_SHFT                        0x0

#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS10_ADDR                                 (DDR_TG0_TG_REG_BASE      + 0x00000074)
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS10_PHYS                                 (DDR_TG0_TG_REG_BASE_PHYS + 0x00000074)
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS10_OFFS                                 (DDR_TG0_TG_REG_BASE_OFFS + 0x00000074)
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS10_RMSK                                 0xffffffff
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS10_IN          \
        in_dword_masked(HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS10_ADDR, HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS10_RMSK)
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS10_INM(m)      \
        in_dword_masked(HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS10_ADDR, m)
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS10_RD_DATA_RCV_63_32_BMSK               0xffffffff
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS10_RD_DATA_RCV_63_32_SHFT                      0x0

#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS11_ADDR                                 (DDR_TG0_TG_REG_BASE      + 0x00000078)
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS11_PHYS                                 (DDR_TG0_TG_REG_BASE_PHYS + 0x00000078)
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS11_OFFS                                 (DDR_TG0_TG_REG_BASE_OFFS + 0x00000078)
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS11_RMSK                                 0xffffffff
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS11_IN          \
        in_dword_masked(HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS11_ADDR, HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS11_RMSK)
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS11_INM(m)      \
        in_dword_masked(HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS11_ADDR, m)
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS11_RD_DATA_RCV_95_64_BMSK               0xffffffff
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS11_RD_DATA_RCV_95_64_SHFT                      0x0

#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS12_ADDR                                 (DDR_TG0_TG_REG_BASE      + 0x0000007c)
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS12_PHYS                                 (DDR_TG0_TG_REG_BASE_PHYS + 0x0000007c)
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS12_OFFS                                 (DDR_TG0_TG_REG_BASE_OFFS + 0x0000007c)
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS12_RMSK                                 0xffffffff
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS12_IN          \
        in_dword_masked(HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS12_ADDR, HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS12_RMSK)
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS12_INM(m)      \
        in_dword_masked(HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS12_ADDR, m)
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS12_RD_DATA_RCV_127_96_BMSK              0xffffffff
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS12_RD_DATA_RCV_127_96_SHFT                     0x0

#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS13_ADDR                                 (DDR_TG0_TG_REG_BASE      + 0x00000080)
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS13_PHYS                                 (DDR_TG0_TG_REG_BASE_PHYS + 0x00000080)
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS13_OFFS                                 (DDR_TG0_TG_REG_BASE_OFFS + 0x00000080)
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS13_RMSK                                 0xffffffff
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS13_IN          \
        in_dword_masked(HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS13_ADDR, HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS13_RMSK)
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS13_INM(m)      \
        in_dword_masked(HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS13_ADDR, m)
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS13_RD_DATA_RCV_159_128_BMSK             0xffffffff
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS13_RD_DATA_RCV_159_128_SHFT                    0x0

#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS14_ADDR                                 (DDR_TG0_TG_REG_BASE      + 0x00000084)
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS14_PHYS                                 (DDR_TG0_TG_REG_BASE_PHYS + 0x00000084)
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS14_OFFS                                 (DDR_TG0_TG_REG_BASE_OFFS + 0x00000084)
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS14_RMSK                                 0xffffffff
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS14_IN          \
        in_dword_masked(HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS14_ADDR, HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS14_RMSK)
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS14_INM(m)      \
        in_dword_masked(HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS14_ADDR, m)
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS14_RD_DATA_RCV_191_160_BMSK             0xffffffff
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS14_RD_DATA_RCV_191_160_SHFT                    0x0

#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS15_ADDR                                 (DDR_TG0_TG_REG_BASE      + 0x00000088)
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS15_PHYS                                 (DDR_TG0_TG_REG_BASE_PHYS + 0x00000088)
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS15_OFFS                                 (DDR_TG0_TG_REG_BASE_OFFS + 0x00000088)
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS15_RMSK                                 0xffffffff
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS15_IN          \
        in_dword_masked(HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS15_ADDR, HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS15_RMSK)
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS15_INM(m)      \
        in_dword_masked(HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS15_ADDR, m)
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS15_RD_DATA_RCV_223_192_BMSK             0xffffffff
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS15_RD_DATA_RCV_223_192_SHFT                    0x0

#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS16_ADDR                                 (DDR_TG0_TG_REG_BASE      + 0x0000008c)
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS16_PHYS                                 (DDR_TG0_TG_REG_BASE_PHYS + 0x0000008c)
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS16_OFFS                                 (DDR_TG0_TG_REG_BASE_OFFS + 0x0000008c)
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS16_RMSK                                 0xffffffff
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS16_IN          \
        in_dword_masked(HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS16_ADDR, HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS16_RMSK)
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS16_INM(m)      \
        in_dword_masked(HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS16_ADDR, m)
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS16_RD_DATA_RCV_255_224_BMSK             0xffffffff
#define HWIO_DDR_TG0_TG_STREAM_RD_FAIL_STATUS16_RD_DATA_RCV_255_224_SHFT                    0x0

#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_RCV_STATUS0_ADDR                              (DDR_TG0_TG_REG_BASE      + 0x00000090)
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_RCV_STATUS0_PHYS                              (DDR_TG0_TG_REG_BASE_PHYS + 0x00000090)
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_RCV_STATUS0_OFFS                              (DDR_TG0_TG_REG_BASE_OFFS + 0x00000090)
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_RCV_STATUS0_RMSK                              0xffffffff
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_RCV_STATUS0_IN          \
        in_dword_masked(HWIO_DDR_TG0_TG_STREAM_MISR_RD_RCV_STATUS0_ADDR, HWIO_DDR_TG0_TG_STREAM_MISR_RD_RCV_STATUS0_RMSK)
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_RCV_STATUS0_INM(m)      \
        in_dword_masked(HWIO_DDR_TG0_TG_STREAM_MISR_RD_RCV_STATUS0_ADDR, m)
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_RCV_STATUS0_RD_DATA_RCV_31_0_BMSK             0xffffffff
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_RCV_STATUS0_RD_DATA_RCV_31_0_SHFT                    0x0

#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_RCV_STATUS1_ADDR                              (DDR_TG0_TG_REG_BASE      + 0x00000094)
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_RCV_STATUS1_PHYS                              (DDR_TG0_TG_REG_BASE_PHYS + 0x00000094)
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_RCV_STATUS1_OFFS                              (DDR_TG0_TG_REG_BASE_OFFS + 0x00000094)
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_RCV_STATUS1_RMSK                              0xffffffff
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_RCV_STATUS1_IN          \
        in_dword_masked(HWIO_DDR_TG0_TG_STREAM_MISR_RD_RCV_STATUS1_ADDR, HWIO_DDR_TG0_TG_STREAM_MISR_RD_RCV_STATUS1_RMSK)
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_RCV_STATUS1_INM(m)      \
        in_dword_masked(HWIO_DDR_TG0_TG_STREAM_MISR_RD_RCV_STATUS1_ADDR, m)
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_RCV_STATUS1_RD_DATA_RCV_63_32_BMSK            0xffffffff
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_RCV_STATUS1_RD_DATA_RCV_63_32_SHFT                   0x0

#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_RCV_STATUS2_ADDR                              (DDR_TG0_TG_REG_BASE      + 0x00000098)
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_RCV_STATUS2_PHYS                              (DDR_TG0_TG_REG_BASE_PHYS + 0x00000098)
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_RCV_STATUS2_OFFS                              (DDR_TG0_TG_REG_BASE_OFFS + 0x00000098)
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_RCV_STATUS2_RMSK                              0xffffffff
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_RCV_STATUS2_IN          \
        in_dword_masked(HWIO_DDR_TG0_TG_STREAM_MISR_RD_RCV_STATUS2_ADDR, HWIO_DDR_TG0_TG_STREAM_MISR_RD_RCV_STATUS2_RMSK)
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_RCV_STATUS2_INM(m)      \
        in_dword_masked(HWIO_DDR_TG0_TG_STREAM_MISR_RD_RCV_STATUS2_ADDR, m)
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_RCV_STATUS2_RD_DATA_RCV_95_64_BMSK            0xffffffff
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_RCV_STATUS2_RD_DATA_RCV_95_64_SHFT                   0x0

#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_RCV_STATUS3_ADDR                              (DDR_TG0_TG_REG_BASE      + 0x0000009c)
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_RCV_STATUS3_PHYS                              (DDR_TG0_TG_REG_BASE_PHYS + 0x0000009c)
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_RCV_STATUS3_OFFS                              (DDR_TG0_TG_REG_BASE_OFFS + 0x0000009c)
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_RCV_STATUS3_RMSK                              0xffffffff
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_RCV_STATUS3_IN          \
        in_dword_masked(HWIO_DDR_TG0_TG_STREAM_MISR_RD_RCV_STATUS3_ADDR, HWIO_DDR_TG0_TG_STREAM_MISR_RD_RCV_STATUS3_RMSK)
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_RCV_STATUS3_INM(m)      \
        in_dword_masked(HWIO_DDR_TG0_TG_STREAM_MISR_RD_RCV_STATUS3_ADDR, m)
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_RCV_STATUS3_RD_DATA_RCV_127_96_BMSK           0xffffffff
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_RCV_STATUS3_RD_DATA_RCV_127_96_SHFT                  0x0

#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_RCV_STATUS4_ADDR                              (DDR_TG0_TG_REG_BASE      + 0x000000a0)
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_RCV_STATUS4_PHYS                              (DDR_TG0_TG_REG_BASE_PHYS + 0x000000a0)
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_RCV_STATUS4_OFFS                              (DDR_TG0_TG_REG_BASE_OFFS + 0x000000a0)
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_RCV_STATUS4_RMSK                              0xffffffff
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_RCV_STATUS4_IN          \
        in_dword_masked(HWIO_DDR_TG0_TG_STREAM_MISR_RD_RCV_STATUS4_ADDR, HWIO_DDR_TG0_TG_STREAM_MISR_RD_RCV_STATUS4_RMSK)
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_RCV_STATUS4_INM(m)      \
        in_dword_masked(HWIO_DDR_TG0_TG_STREAM_MISR_RD_RCV_STATUS4_ADDR, m)
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_RCV_STATUS4_RD_DATA_RCV_159_128_BMSK          0xffffffff
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_RCV_STATUS4_RD_DATA_RCV_159_128_SHFT                 0x0

#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_RCV_STATUS5_ADDR                              (DDR_TG0_TG_REG_BASE      + 0x000000a4)
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_RCV_STATUS5_PHYS                              (DDR_TG0_TG_REG_BASE_PHYS + 0x000000a4)
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_RCV_STATUS5_OFFS                              (DDR_TG0_TG_REG_BASE_OFFS + 0x000000a4)
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_RCV_STATUS5_RMSK                              0xffffffff
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_RCV_STATUS5_IN          \
        in_dword_masked(HWIO_DDR_TG0_TG_STREAM_MISR_RD_RCV_STATUS5_ADDR, HWIO_DDR_TG0_TG_STREAM_MISR_RD_RCV_STATUS5_RMSK)
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_RCV_STATUS5_INM(m)      \
        in_dword_masked(HWIO_DDR_TG0_TG_STREAM_MISR_RD_RCV_STATUS5_ADDR, m)
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_RCV_STATUS5_RD_DATA_RCV_191_160_BMSK          0xffffffff
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_RCV_STATUS5_RD_DATA_RCV_191_160_SHFT                 0x0

#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_RCV_STATUS6_ADDR                              (DDR_TG0_TG_REG_BASE      + 0x000000a8)
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_RCV_STATUS6_PHYS                              (DDR_TG0_TG_REG_BASE_PHYS + 0x000000a8)
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_RCV_STATUS6_OFFS                              (DDR_TG0_TG_REG_BASE_OFFS + 0x000000a8)
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_RCV_STATUS6_RMSK                              0xffffffff
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_RCV_STATUS6_IN          \
        in_dword_masked(HWIO_DDR_TG0_TG_STREAM_MISR_RD_RCV_STATUS6_ADDR, HWIO_DDR_TG0_TG_STREAM_MISR_RD_RCV_STATUS6_RMSK)
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_RCV_STATUS6_INM(m)      \
        in_dword_masked(HWIO_DDR_TG0_TG_STREAM_MISR_RD_RCV_STATUS6_ADDR, m)
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_RCV_STATUS6_RD_DATA_RCV_223_192_BMSK          0xffffffff
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_RCV_STATUS6_RD_DATA_RCV_223_192_SHFT                 0x0

#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_RCV_STATUS7_ADDR                              (DDR_TG0_TG_REG_BASE      + 0x000000ac)
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_RCV_STATUS7_PHYS                              (DDR_TG0_TG_REG_BASE_PHYS + 0x000000ac)
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_RCV_STATUS7_OFFS                              (DDR_TG0_TG_REG_BASE_OFFS + 0x000000ac)
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_RCV_STATUS7_RMSK                              0xffffffff
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_RCV_STATUS7_IN          \
        in_dword_masked(HWIO_DDR_TG0_TG_STREAM_MISR_RD_RCV_STATUS7_ADDR, HWIO_DDR_TG0_TG_STREAM_MISR_RD_RCV_STATUS7_RMSK)
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_RCV_STATUS7_INM(m)      \
        in_dword_masked(HWIO_DDR_TG0_TG_STREAM_MISR_RD_RCV_STATUS7_ADDR, m)
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_RCV_STATUS7_RD_DATA_RCV_255_224_BMSK          0xffffffff
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_RCV_STATUS7_RD_DATA_RCV_255_224_SHFT                 0x0

#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS0_ADDR                              (DDR_TG0_TG_REG_BASE      + 0x000000b0)
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS0_PHYS                              (DDR_TG0_TG_REG_BASE_PHYS + 0x000000b0)
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS0_OFFS                              (DDR_TG0_TG_REG_BASE_OFFS + 0x000000b0)
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS0_RMSK                              0xffffffff
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS0_IN          \
        in_dword_masked(HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS0_ADDR, HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS0_RMSK)
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS0_INM(m)      \
        in_dword_masked(HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS0_ADDR, m)
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS0_OUT(v)      \
        out_dword(HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS0_ADDR,v)
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS0_ADDR,m,v,HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS0_IN)
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS0_RD_DATA_EXP_31_0_BMSK             0xffffffff
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS0_RD_DATA_EXP_31_0_SHFT                    0x0

#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS1_ADDR                              (DDR_TG0_TG_REG_BASE      + 0x000000b4)
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS1_PHYS                              (DDR_TG0_TG_REG_BASE_PHYS + 0x000000b4)
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS1_OFFS                              (DDR_TG0_TG_REG_BASE_OFFS + 0x000000b4)
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS1_RMSK                              0xffffffff
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS1_IN          \
        in_dword_masked(HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS1_ADDR, HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS1_RMSK)
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS1_INM(m)      \
        in_dword_masked(HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS1_ADDR, m)
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS1_OUT(v)      \
        out_dword(HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS1_ADDR,v)
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS1_ADDR,m,v,HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS1_IN)
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS1_RD_DATA_EXP_63_32_BMSK            0xffffffff
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS1_RD_DATA_EXP_63_32_SHFT                   0x0

#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS2_ADDR                              (DDR_TG0_TG_REG_BASE      + 0x000000b8)
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS2_PHYS                              (DDR_TG0_TG_REG_BASE_PHYS + 0x000000b8)
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS2_OFFS                              (DDR_TG0_TG_REG_BASE_OFFS + 0x000000b8)
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS2_RMSK                              0xffffffff
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS2_IN          \
        in_dword_masked(HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS2_ADDR, HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS2_RMSK)
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS2_INM(m)      \
        in_dword_masked(HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS2_ADDR, m)
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS2_OUT(v)      \
        out_dword(HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS2_ADDR,v)
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS2_ADDR,m,v,HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS2_IN)
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS2_RD_DATA_EXP_95_64_BMSK            0xffffffff
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS2_RD_DATA_EXP_95_64_SHFT                   0x0

#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS3_ADDR                              (DDR_TG0_TG_REG_BASE      + 0x000000bc)
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS3_PHYS                              (DDR_TG0_TG_REG_BASE_PHYS + 0x000000bc)
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS3_OFFS                              (DDR_TG0_TG_REG_BASE_OFFS + 0x000000bc)
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS3_RMSK                              0xffffffff
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS3_IN          \
        in_dword_masked(HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS3_ADDR, HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS3_RMSK)
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS3_INM(m)      \
        in_dword_masked(HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS3_ADDR, m)
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS3_OUT(v)      \
        out_dword(HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS3_ADDR,v)
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS3_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS3_ADDR,m,v,HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS3_IN)
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS3_RD_DATA_EXP_127_96_BMSK           0xffffffff
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS3_RD_DATA_EXP_127_96_SHFT                  0x0

#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS4_ADDR                              (DDR_TG0_TG_REG_BASE      + 0x000000c0)
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS4_PHYS                              (DDR_TG0_TG_REG_BASE_PHYS + 0x000000c0)
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS4_OFFS                              (DDR_TG0_TG_REG_BASE_OFFS + 0x000000c0)
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS4_RMSK                              0xffffffff
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS4_IN          \
        in_dword_masked(HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS4_ADDR, HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS4_RMSK)
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS4_INM(m)      \
        in_dword_masked(HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS4_ADDR, m)
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS4_OUT(v)      \
        out_dword(HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS4_ADDR,v)
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS4_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS4_ADDR,m,v,HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS4_IN)
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS4_RD_DATA_EXP_159_128_BMSK          0xffffffff
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS4_RD_DATA_EXP_159_128_SHFT                 0x0

#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS5_ADDR                              (DDR_TG0_TG_REG_BASE      + 0x000000c4)
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS5_PHYS                              (DDR_TG0_TG_REG_BASE_PHYS + 0x000000c4)
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS5_OFFS                              (DDR_TG0_TG_REG_BASE_OFFS + 0x000000c4)
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS5_RMSK                              0xffffffff
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS5_IN          \
        in_dword_masked(HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS5_ADDR, HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS5_RMSK)
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS5_INM(m)      \
        in_dword_masked(HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS5_ADDR, m)
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS5_OUT(v)      \
        out_dword(HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS5_ADDR,v)
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS5_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS5_ADDR,m,v,HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS5_IN)
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS5_RD_DATA_EXP_191_160_BMSK          0xffffffff
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS5_RD_DATA_EXP_191_160_SHFT                 0x0

#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS6_ADDR                              (DDR_TG0_TG_REG_BASE      + 0x000000c8)
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS6_PHYS                              (DDR_TG0_TG_REG_BASE_PHYS + 0x000000c8)
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS6_OFFS                              (DDR_TG0_TG_REG_BASE_OFFS + 0x000000c8)
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS6_RMSK                              0xffffffff
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS6_IN          \
        in_dword_masked(HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS6_ADDR, HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS6_RMSK)
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS6_INM(m)      \
        in_dword_masked(HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS6_ADDR, m)
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS6_OUT(v)      \
        out_dword(HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS6_ADDR,v)
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS6_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS6_ADDR,m,v,HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS6_IN)
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS6_RD_DATA_EXP_223_192_BMSK          0xffffffff
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS6_RD_DATA_EXP_223_192_SHFT                 0x0

#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS7_ADDR                              (DDR_TG0_TG_REG_BASE      + 0x000000cc)
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS7_PHYS                              (DDR_TG0_TG_REG_BASE_PHYS + 0x000000cc)
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS7_OFFS                              (DDR_TG0_TG_REG_BASE_OFFS + 0x000000cc)
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS7_RMSK                              0xffffffff
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS7_IN          \
        in_dword_masked(HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS7_ADDR, HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS7_RMSK)
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS7_INM(m)      \
        in_dword_masked(HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS7_ADDR, m)
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS7_OUT(v)      \
        out_dword(HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS7_ADDR,v)
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS7_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS7_ADDR,m,v,HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS7_IN)
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS7_RD_DATA_EXP_255_224_BMSK          0xffffffff
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_EXP_STATUS7_RD_DATA_EXP_255_224_SHFT                 0x0

#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_CMP_STATUS_ADDR                               (DDR_TG0_TG_REG_BASE      + 0x000000d0)
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_CMP_STATUS_PHYS                               (DDR_TG0_TG_REG_BASE_PHYS + 0x000000d0)
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_CMP_STATUS_OFFS                               (DDR_TG0_TG_REG_BASE_OFFS + 0x000000d0)
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_CMP_STATUS_RMSK                                      0x1
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_CMP_STATUS_IN          \
        in_dword_masked(HWIO_DDR_TG0_TG_STREAM_MISR_RD_CMP_STATUS_ADDR, HWIO_DDR_TG0_TG_STREAM_MISR_RD_CMP_STATUS_RMSK)
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_CMP_STATUS_INM(m)      \
        in_dword_masked(HWIO_DDR_TG0_TG_STREAM_MISR_RD_CMP_STATUS_ADDR, m)
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_CMP_STATUS_RD_DATA_CMP_FAIL_BMSK                     0x1
#define HWIO_DDR_TG0_TG_STREAM_MISR_RD_CMP_STATUS_RD_DATA_CMP_FAIL_SHFT                     0x0

/*----------------------------------------------------------------------------
 * MODULE: DPC_REG_DPCC_DPCC_DPCC_REG
 *--------------------------------------------------------------------------*/

#define DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE                                            (DDR_SS_BASE      + 0x000c8000)
#define DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_SIZE                                       0x1000
#define DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_USED                                       0x128
#define DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_PHYS                                       (DDR_SS_BASE_PHYS + 0x000c8000)
#define DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_OFFS                                       0x000c8000

#define HWIO_DPC_REG_DPCC_DEBUG_DIV_CDIVR_ADDR                                         (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE      + 0x00000004)
#define HWIO_DPC_REG_DPCC_DEBUG_DIV_CDIVR_PHYS                                         (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_PHYS + 0x00000004)
#define HWIO_DPC_REG_DPCC_DEBUG_DIV_CDIVR_OFFS                                         (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_OFFS + 0x00000004)
#define HWIO_DPC_REG_DPCC_DEBUG_DIV_CDIVR_RMSK                                                0x3
#define HWIO_DPC_REG_DPCC_DEBUG_DIV_CDIVR_IN          \
        in_dword_masked(HWIO_DPC_REG_DPCC_DEBUG_DIV_CDIVR_ADDR, HWIO_DPC_REG_DPCC_DEBUG_DIV_CDIVR_RMSK)
#define HWIO_DPC_REG_DPCC_DEBUG_DIV_CDIVR_INM(m)      \
        in_dword_masked(HWIO_DPC_REG_DPCC_DEBUG_DIV_CDIVR_ADDR, m)
#define HWIO_DPC_REG_DPCC_DEBUG_DIV_CDIVR_OUT(v)      \
        out_dword(HWIO_DPC_REG_DPCC_DEBUG_DIV_CDIVR_ADDR,v)
#define HWIO_DPC_REG_DPCC_DEBUG_DIV_CDIVR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DPC_REG_DPCC_DEBUG_DIV_CDIVR_ADDR,m,v,HWIO_DPC_REG_DPCC_DEBUG_DIV_CDIVR_IN)
#define HWIO_DPC_REG_DPCC_DEBUG_DIV_CDIVR_CLK_DIV_BMSK                                        0x3
#define HWIO_DPC_REG_DPCC_DEBUG_DIV_CDIVR_CLK_DIV_SHFT                                        0x0

#define HWIO_DPC_REG_DPCC_DEBUG_CBCR_ADDR                                              (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE      + 0x00000008)
#define HWIO_DPC_REG_DPCC_DEBUG_CBCR_PHYS                                              (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_PHYS + 0x00000008)
#define HWIO_DPC_REG_DPCC_DEBUG_CBCR_OFFS                                              (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_OFFS + 0x00000008)
#define HWIO_DPC_REG_DPCC_DEBUG_CBCR_RMSK                                              0x80000005
#define HWIO_DPC_REG_DPCC_DEBUG_CBCR_IN          \
        in_dword_masked(HWIO_DPC_REG_DPCC_DEBUG_CBCR_ADDR, HWIO_DPC_REG_DPCC_DEBUG_CBCR_RMSK)
#define HWIO_DPC_REG_DPCC_DEBUG_CBCR_INM(m)      \
        in_dword_masked(HWIO_DPC_REG_DPCC_DEBUG_CBCR_ADDR, m)
#define HWIO_DPC_REG_DPCC_DEBUG_CBCR_OUT(v)      \
        out_dword(HWIO_DPC_REG_DPCC_DEBUG_CBCR_ADDR,v)
#define HWIO_DPC_REG_DPCC_DEBUG_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DPC_REG_DPCC_DEBUG_CBCR_ADDR,m,v,HWIO_DPC_REG_DPCC_DEBUG_CBCR_IN)
#define HWIO_DPC_REG_DPCC_DEBUG_CBCR_CLK_OFF_BMSK                                      0x80000000
#define HWIO_DPC_REG_DPCC_DEBUG_CBCR_CLK_OFF_SHFT                                            0x1f
#define HWIO_DPC_REG_DPCC_DEBUG_CBCR_CLK_ARES_BMSK                                            0x4
#define HWIO_DPC_REG_DPCC_DEBUG_CBCR_CLK_ARES_SHFT                                            0x2
#define HWIO_DPC_REG_DPCC_DEBUG_CBCR_CLK_ARES_NO_RESET_FVAL                                   0x0
#define HWIO_DPC_REG_DPCC_DEBUG_CBCR_CLK_ARES_RESET_FVAL                                      0x1
#define HWIO_DPC_REG_DPCC_DEBUG_CBCR_CLK_ENABLE_BMSK                                          0x1
#define HWIO_DPC_REG_DPCC_DEBUG_CBCR_CLK_ENABLE_SHFT                                          0x0
#define HWIO_DPC_REG_DPCC_DEBUG_CBCR_CLK_ENABLE_DISABLE_FVAL                                  0x0
#define HWIO_DPC_REG_DPCC_DEBUG_CBCR_CLK_ENABLE_ENABLE_FVAL                                   0x1

#define HWIO_DPC_REG_DPCC_PLL_TEST_DIV_CDIVR_ADDR                                      (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE      + 0x00000010)
#define HWIO_DPC_REG_DPCC_PLL_TEST_DIV_CDIVR_PHYS                                      (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_PHYS + 0x00000010)
#define HWIO_DPC_REG_DPCC_PLL_TEST_DIV_CDIVR_OFFS                                      (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_OFFS + 0x00000010)
#define HWIO_DPC_REG_DPCC_PLL_TEST_DIV_CDIVR_RMSK                                             0x3
#define HWIO_DPC_REG_DPCC_PLL_TEST_DIV_CDIVR_IN          \
        in_dword_masked(HWIO_DPC_REG_DPCC_PLL_TEST_DIV_CDIVR_ADDR, HWIO_DPC_REG_DPCC_PLL_TEST_DIV_CDIVR_RMSK)
#define HWIO_DPC_REG_DPCC_PLL_TEST_DIV_CDIVR_INM(m)      \
        in_dword_masked(HWIO_DPC_REG_DPCC_PLL_TEST_DIV_CDIVR_ADDR, m)
#define HWIO_DPC_REG_DPCC_PLL_TEST_DIV_CDIVR_OUT(v)      \
        out_dword(HWIO_DPC_REG_DPCC_PLL_TEST_DIV_CDIVR_ADDR,v)
#define HWIO_DPC_REG_DPCC_PLL_TEST_DIV_CDIVR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DPC_REG_DPCC_PLL_TEST_DIV_CDIVR_ADDR,m,v,HWIO_DPC_REG_DPCC_PLL_TEST_DIV_CDIVR_IN)
#define HWIO_DPC_REG_DPCC_PLL_TEST_DIV_CDIVR_CLK_DIV_BMSK                                     0x3
#define HWIO_DPC_REG_DPCC_PLL_TEST_DIV_CDIVR_CLK_DIV_SHFT                                     0x0

#define HWIO_DPC_REG_DPCC_PLL_TEST_CBCR_ADDR                                           (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE      + 0x00000014)
#define HWIO_DPC_REG_DPCC_PLL_TEST_CBCR_PHYS                                           (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_PHYS + 0x00000014)
#define HWIO_DPC_REG_DPCC_PLL_TEST_CBCR_OFFS                                           (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_OFFS + 0x00000014)
#define HWIO_DPC_REG_DPCC_PLL_TEST_CBCR_RMSK                                           0x80000005
#define HWIO_DPC_REG_DPCC_PLL_TEST_CBCR_IN          \
        in_dword_masked(HWIO_DPC_REG_DPCC_PLL_TEST_CBCR_ADDR, HWIO_DPC_REG_DPCC_PLL_TEST_CBCR_RMSK)
#define HWIO_DPC_REG_DPCC_PLL_TEST_CBCR_INM(m)      \
        in_dword_masked(HWIO_DPC_REG_DPCC_PLL_TEST_CBCR_ADDR, m)
#define HWIO_DPC_REG_DPCC_PLL_TEST_CBCR_OUT(v)      \
        out_dword(HWIO_DPC_REG_DPCC_PLL_TEST_CBCR_ADDR,v)
#define HWIO_DPC_REG_DPCC_PLL_TEST_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DPC_REG_DPCC_PLL_TEST_CBCR_ADDR,m,v,HWIO_DPC_REG_DPCC_PLL_TEST_CBCR_IN)
#define HWIO_DPC_REG_DPCC_PLL_TEST_CBCR_CLK_OFF_BMSK                                   0x80000000
#define HWIO_DPC_REG_DPCC_PLL_TEST_CBCR_CLK_OFF_SHFT                                         0x1f
#define HWIO_DPC_REG_DPCC_PLL_TEST_CBCR_CLK_ARES_BMSK                                         0x4
#define HWIO_DPC_REG_DPCC_PLL_TEST_CBCR_CLK_ARES_SHFT                                         0x2
#define HWIO_DPC_REG_DPCC_PLL_TEST_CBCR_CLK_ARES_NO_RESET_FVAL                                0x0
#define HWIO_DPC_REG_DPCC_PLL_TEST_CBCR_CLK_ARES_RESET_FVAL                                   0x1
#define HWIO_DPC_REG_DPCC_PLL_TEST_CBCR_CLK_ENABLE_BMSK                                       0x1
#define HWIO_DPC_REG_DPCC_PLL_TEST_CBCR_CLK_ENABLE_SHFT                                       0x0
#define HWIO_DPC_REG_DPCC_PLL_TEST_CBCR_CLK_ENABLE_DISABLE_FVAL                               0x0
#define HWIO_DPC_REG_DPCC_PLL_TEST_CBCR_CLK_ENABLE_ENABLE_FVAL                                0x1

#define HWIO_DPC_REG_DPCC_SHUB_BCR_ADDR                                                (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE      + 0x00000018)
#define HWIO_DPC_REG_DPCC_SHUB_BCR_PHYS                                                (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_PHYS + 0x00000018)
#define HWIO_DPC_REG_DPCC_SHUB_BCR_OFFS                                                (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_OFFS + 0x00000018)
#define HWIO_DPC_REG_DPCC_SHUB_BCR_RMSK                                                       0x1
#define HWIO_DPC_REG_DPCC_SHUB_BCR_IN          \
        in_dword_masked(HWIO_DPC_REG_DPCC_SHUB_BCR_ADDR, HWIO_DPC_REG_DPCC_SHUB_BCR_RMSK)
#define HWIO_DPC_REG_DPCC_SHUB_BCR_INM(m)      \
        in_dword_masked(HWIO_DPC_REG_DPCC_SHUB_BCR_ADDR, m)
#define HWIO_DPC_REG_DPCC_SHUB_BCR_OUT(v)      \
        out_dword(HWIO_DPC_REG_DPCC_SHUB_BCR_ADDR,v)
#define HWIO_DPC_REG_DPCC_SHUB_BCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DPC_REG_DPCC_SHUB_BCR_ADDR,m,v,HWIO_DPC_REG_DPCC_SHUB_BCR_IN)
#define HWIO_DPC_REG_DPCC_SHUB_BCR_BLK_ARES_BMSK                                              0x1
#define HWIO_DPC_REG_DPCC_SHUB_BCR_BLK_ARES_SHFT                                              0x0
#define HWIO_DPC_REG_DPCC_SHUB_BCR_BLK_ARES_DISABLE_FVAL                                      0x0
#define HWIO_DPC_REG_DPCC_SHUB_BCR_BLK_ARES_ENABLE_FVAL                                       0x1

#define HWIO_DPC_REG_DPCC_SHUB_GDSCR_ADDR                                              (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE      + 0x0000001c)
#define HWIO_DPC_REG_DPCC_SHUB_GDSCR_PHYS                                              (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_PHYS + 0x0000001c)
#define HWIO_DPC_REG_DPCC_SHUB_GDSCR_OFFS                                              (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_OFFS + 0x0000001c)
#define HWIO_DPC_REG_DPCC_SHUB_GDSCR_RMSK                                              0xf8ffffff
#define HWIO_DPC_REG_DPCC_SHUB_GDSCR_IN          \
        in_dword_masked(HWIO_DPC_REG_DPCC_SHUB_GDSCR_ADDR, HWIO_DPC_REG_DPCC_SHUB_GDSCR_RMSK)
#define HWIO_DPC_REG_DPCC_SHUB_GDSCR_INM(m)      \
        in_dword_masked(HWIO_DPC_REG_DPCC_SHUB_GDSCR_ADDR, m)
#define HWIO_DPC_REG_DPCC_SHUB_GDSCR_OUT(v)      \
        out_dword(HWIO_DPC_REG_DPCC_SHUB_GDSCR_ADDR,v)
#define HWIO_DPC_REG_DPCC_SHUB_GDSCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DPC_REG_DPCC_SHUB_GDSCR_ADDR,m,v,HWIO_DPC_REG_DPCC_SHUB_GDSCR_IN)
#define HWIO_DPC_REG_DPCC_SHUB_GDSCR_PWR_ON_BMSK                                       0x80000000
#define HWIO_DPC_REG_DPCC_SHUB_GDSCR_PWR_ON_SHFT                                             0x1f
#define HWIO_DPC_REG_DPCC_SHUB_GDSCR_GDSC_STATE_BMSK                                   0x78000000
#define HWIO_DPC_REG_DPCC_SHUB_GDSCR_GDSC_STATE_SHFT                                         0x1b
#define HWIO_DPC_REG_DPCC_SHUB_GDSCR_EN_REST_WAIT_BMSK                                   0xf00000
#define HWIO_DPC_REG_DPCC_SHUB_GDSCR_EN_REST_WAIT_SHFT                                       0x14
#define HWIO_DPC_REG_DPCC_SHUB_GDSCR_EN_FEW_WAIT_BMSK                                     0xf0000
#define HWIO_DPC_REG_DPCC_SHUB_GDSCR_EN_FEW_WAIT_SHFT                                        0x10
#define HWIO_DPC_REG_DPCC_SHUB_GDSCR_CLK_DIS_WAIT_BMSK                                     0xf000
#define HWIO_DPC_REG_DPCC_SHUB_GDSCR_CLK_DIS_WAIT_SHFT                                        0xc
#define HWIO_DPC_REG_DPCC_SHUB_GDSCR_RETAIN_FF_ENABLE_BMSK                                  0x800
#define HWIO_DPC_REG_DPCC_SHUB_GDSCR_RETAIN_FF_ENABLE_SHFT                                    0xb
#define HWIO_DPC_REG_DPCC_SHUB_GDSCR_RETAIN_FF_ENABLE_DISABLE_FVAL                            0x0
#define HWIO_DPC_REG_DPCC_SHUB_GDSCR_RETAIN_FF_ENABLE_ENABLE_FVAL                             0x1
#define HWIO_DPC_REG_DPCC_SHUB_GDSCR_RESTORE_BMSK                                           0x400
#define HWIO_DPC_REG_DPCC_SHUB_GDSCR_RESTORE_SHFT                                             0xa
#define HWIO_DPC_REG_DPCC_SHUB_GDSCR_RESTORE_DISABLE_FVAL                                     0x0
#define HWIO_DPC_REG_DPCC_SHUB_GDSCR_RESTORE_ENABLE_FVAL                                      0x1
#define HWIO_DPC_REG_DPCC_SHUB_GDSCR_SAVE_BMSK                                              0x200
#define HWIO_DPC_REG_DPCC_SHUB_GDSCR_SAVE_SHFT                                                0x9
#define HWIO_DPC_REG_DPCC_SHUB_GDSCR_SAVE_DISABLE_FVAL                                        0x0
#define HWIO_DPC_REG_DPCC_SHUB_GDSCR_SAVE_ENABLE_FVAL                                         0x1
#define HWIO_DPC_REG_DPCC_SHUB_GDSCR_RETAIN_BMSK                                            0x100
#define HWIO_DPC_REG_DPCC_SHUB_GDSCR_RETAIN_SHFT                                              0x8
#define HWIO_DPC_REG_DPCC_SHUB_GDSCR_RETAIN_DISABLE_FVAL                                      0x0
#define HWIO_DPC_REG_DPCC_SHUB_GDSCR_RETAIN_ENABLE_FVAL                                       0x1
#define HWIO_DPC_REG_DPCC_SHUB_GDSCR_EN_REST_BMSK                                            0x80
#define HWIO_DPC_REG_DPCC_SHUB_GDSCR_EN_REST_SHFT                                             0x7
#define HWIO_DPC_REG_DPCC_SHUB_GDSCR_EN_REST_DISABLE_FVAL                                     0x0
#define HWIO_DPC_REG_DPCC_SHUB_GDSCR_EN_REST_ENABLE_FVAL                                      0x1
#define HWIO_DPC_REG_DPCC_SHUB_GDSCR_EN_FEW_BMSK                                             0x40
#define HWIO_DPC_REG_DPCC_SHUB_GDSCR_EN_FEW_SHFT                                              0x6
#define HWIO_DPC_REG_DPCC_SHUB_GDSCR_EN_FEW_DISABLE_FVAL                                      0x0
#define HWIO_DPC_REG_DPCC_SHUB_GDSCR_EN_FEW_ENABLE_FVAL                                       0x1
#define HWIO_DPC_REG_DPCC_SHUB_GDSCR_CLAMP_IO_BMSK                                           0x20
#define HWIO_DPC_REG_DPCC_SHUB_GDSCR_CLAMP_IO_SHFT                                            0x5
#define HWIO_DPC_REG_DPCC_SHUB_GDSCR_CLAMP_IO_DISABLE_FVAL                                    0x0
#define HWIO_DPC_REG_DPCC_SHUB_GDSCR_CLAMP_IO_ENABLE_FVAL                                     0x1
#define HWIO_DPC_REG_DPCC_SHUB_GDSCR_CLK_DISABLE_BMSK                                        0x10
#define HWIO_DPC_REG_DPCC_SHUB_GDSCR_CLK_DISABLE_SHFT                                         0x4
#define HWIO_DPC_REG_DPCC_SHUB_GDSCR_CLK_DISABLE_CLK_NOT_DISABLE_FVAL                         0x0
#define HWIO_DPC_REG_DPCC_SHUB_GDSCR_CLK_DISABLE_CLK_DISABLE_FVAL                             0x1
#define HWIO_DPC_REG_DPCC_SHUB_GDSCR_PD_ARES_BMSK                                             0x8
#define HWIO_DPC_REG_DPCC_SHUB_GDSCR_PD_ARES_SHFT                                             0x3
#define HWIO_DPC_REG_DPCC_SHUB_GDSCR_PD_ARES_NO_RESET_FVAL                                    0x0
#define HWIO_DPC_REG_DPCC_SHUB_GDSCR_PD_ARES_RESET_FVAL                                       0x1
#define HWIO_DPC_REG_DPCC_SHUB_GDSCR_SW_OVERRIDE_BMSK                                         0x4
#define HWIO_DPC_REG_DPCC_SHUB_GDSCR_SW_OVERRIDE_SHFT                                         0x2
#define HWIO_DPC_REG_DPCC_SHUB_GDSCR_SW_OVERRIDE_DISABLE_FVAL                                 0x0
#define HWIO_DPC_REG_DPCC_SHUB_GDSCR_SW_OVERRIDE_ENABLE_FVAL                                  0x1
#define HWIO_DPC_REG_DPCC_SHUB_GDSCR_HW_CONTROL_BMSK                                          0x2
#define HWIO_DPC_REG_DPCC_SHUB_GDSCR_HW_CONTROL_SHFT                                          0x1
#define HWIO_DPC_REG_DPCC_SHUB_GDSCR_HW_CONTROL_DISABLE_FVAL                                  0x0
#define HWIO_DPC_REG_DPCC_SHUB_GDSCR_HW_CONTROL_ENABLE_FVAL                                   0x1
#define HWIO_DPC_REG_DPCC_SHUB_GDSCR_SW_COLLAPSE_BMSK                                         0x1
#define HWIO_DPC_REG_DPCC_SHUB_GDSCR_SW_COLLAPSE_SHFT                                         0x0
#define HWIO_DPC_REG_DPCC_SHUB_GDSCR_SW_COLLAPSE_DISABLE_FVAL                                 0x0
#define HWIO_DPC_REG_DPCC_SHUB_GDSCR_SW_COLLAPSE_ENABLE_FVAL                                  0x1

#define HWIO_DPC_REG_DPCC_SHUB_CFG_GDSCR_ADDR                                          (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE      + 0x00000020)
#define HWIO_DPC_REG_DPCC_SHUB_CFG_GDSCR_PHYS                                          (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_PHYS + 0x00000020)
#define HWIO_DPC_REG_DPCC_SHUB_CFG_GDSCR_OFFS                                          (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_OFFS + 0x00000020)
#define HWIO_DPC_REG_DPCC_SHUB_CFG_GDSCR_RMSK                                          0xffffffff
#define HWIO_DPC_REG_DPCC_SHUB_CFG_GDSCR_IN          \
        in_dword_masked(HWIO_DPC_REG_DPCC_SHUB_CFG_GDSCR_ADDR, HWIO_DPC_REG_DPCC_SHUB_CFG_GDSCR_RMSK)
#define HWIO_DPC_REG_DPCC_SHUB_CFG_GDSCR_INM(m)      \
        in_dword_masked(HWIO_DPC_REG_DPCC_SHUB_CFG_GDSCR_ADDR, m)
#define HWIO_DPC_REG_DPCC_SHUB_CFG_GDSCR_OUT(v)      \
        out_dword(HWIO_DPC_REG_DPCC_SHUB_CFG_GDSCR_ADDR,v)
#define HWIO_DPC_REG_DPCC_SHUB_CFG_GDSCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DPC_REG_DPCC_SHUB_CFG_GDSCR_ADDR,m,v,HWIO_DPC_REG_DPCC_SHUB_CFG_GDSCR_IN)
#define HWIO_DPC_REG_DPCC_SHUB_CFG_GDSCR_GDSC_SPARE_CTRL_IN_BMSK                       0xf0000000
#define HWIO_DPC_REG_DPCC_SHUB_CFG_GDSCR_GDSC_SPARE_CTRL_IN_SHFT                             0x1c
#define HWIO_DPC_REG_DPCC_SHUB_CFG_GDSCR_GDSC_SPARE_CTRL_OUT_BMSK                       0xc000000
#define HWIO_DPC_REG_DPCC_SHUB_CFG_GDSCR_GDSC_SPARE_CTRL_OUT_SHFT                            0x1a
#define HWIO_DPC_REG_DPCC_SHUB_CFG_GDSCR_GDSC_PWR_DWN_START_BMSK                        0x2000000
#define HWIO_DPC_REG_DPCC_SHUB_CFG_GDSCR_GDSC_PWR_DWN_START_SHFT                             0x19
#define HWIO_DPC_REG_DPCC_SHUB_CFG_GDSCR_GDSC_PWR_UP_START_BMSK                         0x1000000
#define HWIO_DPC_REG_DPCC_SHUB_CFG_GDSCR_GDSC_PWR_UP_START_SHFT                              0x18
#define HWIO_DPC_REG_DPCC_SHUB_CFG_GDSCR_GDSC_CFG_FSM_STATE_STATUS_BMSK                  0xf00000
#define HWIO_DPC_REG_DPCC_SHUB_CFG_GDSCR_GDSC_CFG_FSM_STATE_STATUS_SHFT                      0x14
#define HWIO_DPC_REG_DPCC_SHUB_CFG_GDSCR_GDSC_MEM_PWR_ACK_STATUS_BMSK                     0x80000
#define HWIO_DPC_REG_DPCC_SHUB_CFG_GDSCR_GDSC_MEM_PWR_ACK_STATUS_SHFT                        0x13
#define HWIO_DPC_REG_DPCC_SHUB_CFG_GDSCR_GDSC_ENR_ACK_STATUS_BMSK                         0x40000
#define HWIO_DPC_REG_DPCC_SHUB_CFG_GDSCR_GDSC_ENR_ACK_STATUS_SHFT                            0x12
#define HWIO_DPC_REG_DPCC_SHUB_CFG_GDSCR_GDSC_ENF_ACK_STATUS_BMSK                         0x20000
#define HWIO_DPC_REG_DPCC_SHUB_CFG_GDSCR_GDSC_ENF_ACK_STATUS_SHFT                            0x11
#define HWIO_DPC_REG_DPCC_SHUB_CFG_GDSCR_GDSC_POWER_UP_COMPLETE_BMSK                      0x10000
#define HWIO_DPC_REG_DPCC_SHUB_CFG_GDSCR_GDSC_POWER_UP_COMPLETE_SHFT                         0x10
#define HWIO_DPC_REG_DPCC_SHUB_CFG_GDSCR_GDSC_POWER_DOWN_COMPLETE_BMSK                     0x8000
#define HWIO_DPC_REG_DPCC_SHUB_CFG_GDSCR_GDSC_POWER_DOWN_COMPLETE_SHFT                        0xf
#define HWIO_DPC_REG_DPCC_SHUB_CFG_GDSCR_SOFTWARE_CONTROL_OVERRIDE_BMSK                    0x7800
#define HWIO_DPC_REG_DPCC_SHUB_CFG_GDSCR_SOFTWARE_CONTROL_OVERRIDE_SHFT                       0xb
#define HWIO_DPC_REG_DPCC_SHUB_CFG_GDSCR_GDSC_HANDSHAKE_DIS_BMSK                            0x400
#define HWIO_DPC_REG_DPCC_SHUB_CFG_GDSCR_GDSC_HANDSHAKE_DIS_SHFT                              0xa
#define HWIO_DPC_REG_DPCC_SHUB_CFG_GDSCR_GDSC_MEM_PERI_FORCE_IN_SW_BMSK                     0x200
#define HWIO_DPC_REG_DPCC_SHUB_CFG_GDSCR_GDSC_MEM_PERI_FORCE_IN_SW_SHFT                       0x9
#define HWIO_DPC_REG_DPCC_SHUB_CFG_GDSCR_GDSC_MEM_CORE_FORCE_IN_SW_BMSK                     0x100
#define HWIO_DPC_REG_DPCC_SHUB_CFG_GDSCR_GDSC_MEM_CORE_FORCE_IN_SW_SHFT                       0x8
#define HWIO_DPC_REG_DPCC_SHUB_CFG_GDSCR_GDSC_PHASE_RESET_EN_SW_BMSK                         0x80
#define HWIO_DPC_REG_DPCC_SHUB_CFG_GDSCR_GDSC_PHASE_RESET_EN_SW_SHFT                          0x7
#define HWIO_DPC_REG_DPCC_SHUB_CFG_GDSCR_GDSC_PHASE_RESET_DELAY_COUNT_SW_BMSK                0x60
#define HWIO_DPC_REG_DPCC_SHUB_CFG_GDSCR_GDSC_PHASE_RESET_DELAY_COUNT_SW_SHFT                 0x5
#define HWIO_DPC_REG_DPCC_SHUB_CFG_GDSCR_GDSC_PSCBC_PWR_DWN_SW_BMSK                          0x10
#define HWIO_DPC_REG_DPCC_SHUB_CFG_GDSCR_GDSC_PSCBC_PWR_DWN_SW_SHFT                           0x4
#define HWIO_DPC_REG_DPCC_SHUB_CFG_GDSCR_UNCLAMP_IO_SOFTWARE_OVERRIDE_BMSK                    0x8
#define HWIO_DPC_REG_DPCC_SHUB_CFG_GDSCR_UNCLAMP_IO_SOFTWARE_OVERRIDE_SHFT                    0x3
#define HWIO_DPC_REG_DPCC_SHUB_CFG_GDSCR_SAVE_RESTORE_SOFTWARE_OVERRIDE_BMSK                  0x4
#define HWIO_DPC_REG_DPCC_SHUB_CFG_GDSCR_SAVE_RESTORE_SOFTWARE_OVERRIDE_SHFT                  0x2
#define HWIO_DPC_REG_DPCC_SHUB_CFG_GDSCR_CLAMP_IO_SOFTWARE_OVERRIDE_BMSK                      0x2
#define HWIO_DPC_REG_DPCC_SHUB_CFG_GDSCR_CLAMP_IO_SOFTWARE_OVERRIDE_SHFT                      0x1
#define HWIO_DPC_REG_DPCC_SHUB_CFG_GDSCR_DISABLE_CLK_SOFTWARE_OVERRIDE_BMSK                   0x1
#define HWIO_DPC_REG_DPCC_SHUB_CFG_GDSCR_DISABLE_CLK_SOFTWARE_OVERRIDE_SHFT                   0x0

#define HWIO_DPC_REG_DPCC_SHUB_CFG2_GDSCR_ADDR                                         (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE      + 0x00000024)
#define HWIO_DPC_REG_DPCC_SHUB_CFG2_GDSCR_PHYS                                         (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_PHYS + 0x00000024)
#define HWIO_DPC_REG_DPCC_SHUB_CFG2_GDSCR_OFFS                                         (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_OFFS + 0x00000024)
#define HWIO_DPC_REG_DPCC_SHUB_CFG2_GDSCR_RMSK                                            0x1ffff
#define HWIO_DPC_REG_DPCC_SHUB_CFG2_GDSCR_IN          \
        in_dword_masked(HWIO_DPC_REG_DPCC_SHUB_CFG2_GDSCR_ADDR, HWIO_DPC_REG_DPCC_SHUB_CFG2_GDSCR_RMSK)
#define HWIO_DPC_REG_DPCC_SHUB_CFG2_GDSCR_INM(m)      \
        in_dword_masked(HWIO_DPC_REG_DPCC_SHUB_CFG2_GDSCR_ADDR, m)
#define HWIO_DPC_REG_DPCC_SHUB_CFG2_GDSCR_OUT(v)      \
        out_dword(HWIO_DPC_REG_DPCC_SHUB_CFG2_GDSCR_ADDR,v)
#define HWIO_DPC_REG_DPCC_SHUB_CFG2_GDSCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DPC_REG_DPCC_SHUB_CFG2_GDSCR_ADDR,m,v,HWIO_DPC_REG_DPCC_SHUB_CFG2_GDSCR_IN)
#define HWIO_DPC_REG_DPCC_SHUB_CFG2_GDSCR_GDSC_CLAMP_MEM_SW_BMSK                          0x10000
#define HWIO_DPC_REG_DPCC_SHUB_CFG2_GDSCR_GDSC_CLAMP_MEM_SW_SHFT                             0x10
#define HWIO_DPC_REG_DPCC_SHUB_CFG2_GDSCR_DLY_MEM_PWR_UP_BMSK                              0xf000
#define HWIO_DPC_REG_DPCC_SHUB_CFG2_GDSCR_DLY_MEM_PWR_UP_SHFT                                 0xc
#define HWIO_DPC_REG_DPCC_SHUB_CFG2_GDSCR_DLY_DEASSERT_CLAMP_MEM_BMSK                       0xf00
#define HWIO_DPC_REG_DPCC_SHUB_CFG2_GDSCR_DLY_DEASSERT_CLAMP_MEM_SHFT                         0x8
#define HWIO_DPC_REG_DPCC_SHUB_CFG2_GDSCR_DLY_ASSERT_CLAMP_MEM_BMSK                          0xf0
#define HWIO_DPC_REG_DPCC_SHUB_CFG2_GDSCR_DLY_ASSERT_CLAMP_MEM_SHFT                           0x4
#define HWIO_DPC_REG_DPCC_SHUB_CFG2_GDSCR_MEM_PWR_DWN_TIMEOUT_BMSK                            0xf
#define HWIO_DPC_REG_DPCC_SHUB_CFG2_GDSCR_MEM_PWR_DWN_TIMEOUT_SHFT                            0x0

#define HWIO_DPC_REG_DPCC_LLCC0_XO_CBCR_ADDR                                           (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE      + 0x00000028)
#define HWIO_DPC_REG_DPCC_LLCC0_XO_CBCR_PHYS                                           (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_PHYS + 0x00000028)
#define HWIO_DPC_REG_DPCC_LLCC0_XO_CBCR_OFFS                                           (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_OFFS + 0x00000028)
#define HWIO_DPC_REG_DPCC_LLCC0_XO_CBCR_RMSK                                           0x80000005
#define HWIO_DPC_REG_DPCC_LLCC0_XO_CBCR_IN          \
        in_dword_masked(HWIO_DPC_REG_DPCC_LLCC0_XO_CBCR_ADDR, HWIO_DPC_REG_DPCC_LLCC0_XO_CBCR_RMSK)
#define HWIO_DPC_REG_DPCC_LLCC0_XO_CBCR_INM(m)      \
        in_dword_masked(HWIO_DPC_REG_DPCC_LLCC0_XO_CBCR_ADDR, m)
#define HWIO_DPC_REG_DPCC_LLCC0_XO_CBCR_OUT(v)      \
        out_dword(HWIO_DPC_REG_DPCC_LLCC0_XO_CBCR_ADDR,v)
#define HWIO_DPC_REG_DPCC_LLCC0_XO_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DPC_REG_DPCC_LLCC0_XO_CBCR_ADDR,m,v,HWIO_DPC_REG_DPCC_LLCC0_XO_CBCR_IN)
#define HWIO_DPC_REG_DPCC_LLCC0_XO_CBCR_CLK_OFF_BMSK                                   0x80000000
#define HWIO_DPC_REG_DPCC_LLCC0_XO_CBCR_CLK_OFF_SHFT                                         0x1f
#define HWIO_DPC_REG_DPCC_LLCC0_XO_CBCR_CLK_ARES_BMSK                                         0x4
#define HWIO_DPC_REG_DPCC_LLCC0_XO_CBCR_CLK_ARES_SHFT                                         0x2
#define HWIO_DPC_REG_DPCC_LLCC0_XO_CBCR_CLK_ARES_NO_RESET_FVAL                                0x0
#define HWIO_DPC_REG_DPCC_LLCC0_XO_CBCR_CLK_ARES_RESET_FVAL                                   0x1
#define HWIO_DPC_REG_DPCC_LLCC0_XO_CBCR_CLK_ENABLE_BMSK                                       0x1
#define HWIO_DPC_REG_DPCC_LLCC0_XO_CBCR_CLK_ENABLE_SHFT                                       0x0
#define HWIO_DPC_REG_DPCC_LLCC0_XO_CBCR_CLK_ENABLE_DISABLE_FVAL                               0x0
#define HWIO_DPC_REG_DPCC_LLCC0_XO_CBCR_CLK_ENABLE_ENABLE_FVAL                                0x1

#define HWIO_DPC_REG_DPCC_LLCC1_XO_CBCR_ADDR                                           (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE      + 0x0000002c)
#define HWIO_DPC_REG_DPCC_LLCC1_XO_CBCR_PHYS                                           (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_PHYS + 0x0000002c)
#define HWIO_DPC_REG_DPCC_LLCC1_XO_CBCR_OFFS                                           (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_OFFS + 0x0000002c)
#define HWIO_DPC_REG_DPCC_LLCC1_XO_CBCR_RMSK                                           0x80000005
#define HWIO_DPC_REG_DPCC_LLCC1_XO_CBCR_IN          \
        in_dword_masked(HWIO_DPC_REG_DPCC_LLCC1_XO_CBCR_ADDR, HWIO_DPC_REG_DPCC_LLCC1_XO_CBCR_RMSK)
#define HWIO_DPC_REG_DPCC_LLCC1_XO_CBCR_INM(m)      \
        in_dword_masked(HWIO_DPC_REG_DPCC_LLCC1_XO_CBCR_ADDR, m)
#define HWIO_DPC_REG_DPCC_LLCC1_XO_CBCR_OUT(v)      \
        out_dword(HWIO_DPC_REG_DPCC_LLCC1_XO_CBCR_ADDR,v)
#define HWIO_DPC_REG_DPCC_LLCC1_XO_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DPC_REG_DPCC_LLCC1_XO_CBCR_ADDR,m,v,HWIO_DPC_REG_DPCC_LLCC1_XO_CBCR_IN)
#define HWIO_DPC_REG_DPCC_LLCC1_XO_CBCR_CLK_OFF_BMSK                                   0x80000000
#define HWIO_DPC_REG_DPCC_LLCC1_XO_CBCR_CLK_OFF_SHFT                                         0x1f
#define HWIO_DPC_REG_DPCC_LLCC1_XO_CBCR_CLK_ARES_BMSK                                         0x4
#define HWIO_DPC_REG_DPCC_LLCC1_XO_CBCR_CLK_ARES_SHFT                                         0x2
#define HWIO_DPC_REG_DPCC_LLCC1_XO_CBCR_CLK_ARES_NO_RESET_FVAL                                0x0
#define HWIO_DPC_REG_DPCC_LLCC1_XO_CBCR_CLK_ARES_RESET_FVAL                                   0x1
#define HWIO_DPC_REG_DPCC_LLCC1_XO_CBCR_CLK_ENABLE_BMSK                                       0x1
#define HWIO_DPC_REG_DPCC_LLCC1_XO_CBCR_CLK_ENABLE_SHFT                                       0x0
#define HWIO_DPC_REG_DPCC_LLCC1_XO_CBCR_CLK_ENABLE_DISABLE_FVAL                               0x0
#define HWIO_DPC_REG_DPCC_LLCC1_XO_CBCR_CLK_ENABLE_ENABLE_FVAL                                0x1

#define HWIO_DPC_REG_DPCC_LLCC2_XO_CBCR_ADDR                                           (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE      + 0x00000030)
#define HWIO_DPC_REG_DPCC_LLCC2_XO_CBCR_PHYS                                           (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_PHYS + 0x00000030)
#define HWIO_DPC_REG_DPCC_LLCC2_XO_CBCR_OFFS                                           (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_OFFS + 0x00000030)
#define HWIO_DPC_REG_DPCC_LLCC2_XO_CBCR_RMSK                                           0x80000005
#define HWIO_DPC_REG_DPCC_LLCC2_XO_CBCR_IN          \
        in_dword_masked(HWIO_DPC_REG_DPCC_LLCC2_XO_CBCR_ADDR, HWIO_DPC_REG_DPCC_LLCC2_XO_CBCR_RMSK)
#define HWIO_DPC_REG_DPCC_LLCC2_XO_CBCR_INM(m)      \
        in_dword_masked(HWIO_DPC_REG_DPCC_LLCC2_XO_CBCR_ADDR, m)
#define HWIO_DPC_REG_DPCC_LLCC2_XO_CBCR_OUT(v)      \
        out_dword(HWIO_DPC_REG_DPCC_LLCC2_XO_CBCR_ADDR,v)
#define HWIO_DPC_REG_DPCC_LLCC2_XO_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DPC_REG_DPCC_LLCC2_XO_CBCR_ADDR,m,v,HWIO_DPC_REG_DPCC_LLCC2_XO_CBCR_IN)
#define HWIO_DPC_REG_DPCC_LLCC2_XO_CBCR_CLK_OFF_BMSK                                   0x80000000
#define HWIO_DPC_REG_DPCC_LLCC2_XO_CBCR_CLK_OFF_SHFT                                         0x1f
#define HWIO_DPC_REG_DPCC_LLCC2_XO_CBCR_CLK_ARES_BMSK                                         0x4
#define HWIO_DPC_REG_DPCC_LLCC2_XO_CBCR_CLK_ARES_SHFT                                         0x2
#define HWIO_DPC_REG_DPCC_LLCC2_XO_CBCR_CLK_ARES_NO_RESET_FVAL                                0x0
#define HWIO_DPC_REG_DPCC_LLCC2_XO_CBCR_CLK_ARES_RESET_FVAL                                   0x1
#define HWIO_DPC_REG_DPCC_LLCC2_XO_CBCR_CLK_ENABLE_BMSK                                       0x1
#define HWIO_DPC_REG_DPCC_LLCC2_XO_CBCR_CLK_ENABLE_SHFT                                       0x0
#define HWIO_DPC_REG_DPCC_LLCC2_XO_CBCR_CLK_ENABLE_DISABLE_FVAL                               0x0
#define HWIO_DPC_REG_DPCC_LLCC2_XO_CBCR_CLK_ENABLE_ENABLE_FVAL                                0x1

#define HWIO_DPC_REG_DPCC_LLCC3_XO_CBCR_ADDR                                           (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE      + 0x00000034)
#define HWIO_DPC_REG_DPCC_LLCC3_XO_CBCR_PHYS                                           (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_PHYS + 0x00000034)
#define HWIO_DPC_REG_DPCC_LLCC3_XO_CBCR_OFFS                                           (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_OFFS + 0x00000034)
#define HWIO_DPC_REG_DPCC_LLCC3_XO_CBCR_RMSK                                           0x80000005
#define HWIO_DPC_REG_DPCC_LLCC3_XO_CBCR_IN          \
        in_dword_masked(HWIO_DPC_REG_DPCC_LLCC3_XO_CBCR_ADDR, HWIO_DPC_REG_DPCC_LLCC3_XO_CBCR_RMSK)
#define HWIO_DPC_REG_DPCC_LLCC3_XO_CBCR_INM(m)      \
        in_dword_masked(HWIO_DPC_REG_DPCC_LLCC3_XO_CBCR_ADDR, m)
#define HWIO_DPC_REG_DPCC_LLCC3_XO_CBCR_OUT(v)      \
        out_dword(HWIO_DPC_REG_DPCC_LLCC3_XO_CBCR_ADDR,v)
#define HWIO_DPC_REG_DPCC_LLCC3_XO_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DPC_REG_DPCC_LLCC3_XO_CBCR_ADDR,m,v,HWIO_DPC_REG_DPCC_LLCC3_XO_CBCR_IN)
#define HWIO_DPC_REG_DPCC_LLCC3_XO_CBCR_CLK_OFF_BMSK                                   0x80000000
#define HWIO_DPC_REG_DPCC_LLCC3_XO_CBCR_CLK_OFF_SHFT                                         0x1f
#define HWIO_DPC_REG_DPCC_LLCC3_XO_CBCR_CLK_ARES_BMSK                                         0x4
#define HWIO_DPC_REG_DPCC_LLCC3_XO_CBCR_CLK_ARES_SHFT                                         0x2
#define HWIO_DPC_REG_DPCC_LLCC3_XO_CBCR_CLK_ARES_NO_RESET_FVAL                                0x0
#define HWIO_DPC_REG_DPCC_LLCC3_XO_CBCR_CLK_ARES_RESET_FVAL                                   0x1
#define HWIO_DPC_REG_DPCC_LLCC3_XO_CBCR_CLK_ENABLE_BMSK                                       0x1
#define HWIO_DPC_REG_DPCC_LLCC3_XO_CBCR_CLK_ENABLE_SHFT                                       0x0
#define HWIO_DPC_REG_DPCC_LLCC3_XO_CBCR_CLK_ENABLE_DISABLE_FVAL                               0x0
#define HWIO_DPC_REG_DPCC_LLCC3_XO_CBCR_CLK_ENABLE_ENABLE_FVAL                                0x1

#define HWIO_DPC_REG_DPCC_GEMNOC_XO_CBCR_ADDR                                          (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE      + 0x00000038)
#define HWIO_DPC_REG_DPCC_GEMNOC_XO_CBCR_PHYS                                          (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_PHYS + 0x00000038)
#define HWIO_DPC_REG_DPCC_GEMNOC_XO_CBCR_OFFS                                          (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_OFFS + 0x00000038)
#define HWIO_DPC_REG_DPCC_GEMNOC_XO_CBCR_RMSK                                          0x80000005
#define HWIO_DPC_REG_DPCC_GEMNOC_XO_CBCR_IN          \
        in_dword_masked(HWIO_DPC_REG_DPCC_GEMNOC_XO_CBCR_ADDR, HWIO_DPC_REG_DPCC_GEMNOC_XO_CBCR_RMSK)
#define HWIO_DPC_REG_DPCC_GEMNOC_XO_CBCR_INM(m)      \
        in_dword_masked(HWIO_DPC_REG_DPCC_GEMNOC_XO_CBCR_ADDR, m)
#define HWIO_DPC_REG_DPCC_GEMNOC_XO_CBCR_OUT(v)      \
        out_dword(HWIO_DPC_REG_DPCC_GEMNOC_XO_CBCR_ADDR,v)
#define HWIO_DPC_REG_DPCC_GEMNOC_XO_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DPC_REG_DPCC_GEMNOC_XO_CBCR_ADDR,m,v,HWIO_DPC_REG_DPCC_GEMNOC_XO_CBCR_IN)
#define HWIO_DPC_REG_DPCC_GEMNOC_XO_CBCR_CLK_OFF_BMSK                                  0x80000000
#define HWIO_DPC_REG_DPCC_GEMNOC_XO_CBCR_CLK_OFF_SHFT                                        0x1f
#define HWIO_DPC_REG_DPCC_GEMNOC_XO_CBCR_CLK_ARES_BMSK                                        0x4
#define HWIO_DPC_REG_DPCC_GEMNOC_XO_CBCR_CLK_ARES_SHFT                                        0x2
#define HWIO_DPC_REG_DPCC_GEMNOC_XO_CBCR_CLK_ARES_NO_RESET_FVAL                               0x0
#define HWIO_DPC_REG_DPCC_GEMNOC_XO_CBCR_CLK_ARES_RESET_FVAL                                  0x1
#define HWIO_DPC_REG_DPCC_GEMNOC_XO_CBCR_CLK_ENABLE_BMSK                                      0x1
#define HWIO_DPC_REG_DPCC_GEMNOC_XO_CBCR_CLK_ENABLE_SHFT                                      0x0
#define HWIO_DPC_REG_DPCC_GEMNOC_XO_CBCR_CLK_ENABLE_DISABLE_FVAL                              0x0
#define HWIO_DPC_REG_DPCC_GEMNOC_XO_CBCR_CLK_ENABLE_ENABLE_FVAL                               0x1

#define HWIO_DPC_REG_DPCC_LLCC0_SHUB_CBCR_ADDR                                         (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE      + 0x0000003c)
#define HWIO_DPC_REG_DPCC_LLCC0_SHUB_CBCR_PHYS                                         (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_PHYS + 0x0000003c)
#define HWIO_DPC_REG_DPCC_LLCC0_SHUB_CBCR_OFFS                                         (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_OFFS + 0x0000003c)
#define HWIO_DPC_REG_DPCC_LLCC0_SHUB_CBCR_RMSK                                         0x80000005
#define HWIO_DPC_REG_DPCC_LLCC0_SHUB_CBCR_IN          \
        in_dword_masked(HWIO_DPC_REG_DPCC_LLCC0_SHUB_CBCR_ADDR, HWIO_DPC_REG_DPCC_LLCC0_SHUB_CBCR_RMSK)
#define HWIO_DPC_REG_DPCC_LLCC0_SHUB_CBCR_INM(m)      \
        in_dword_masked(HWIO_DPC_REG_DPCC_LLCC0_SHUB_CBCR_ADDR, m)
#define HWIO_DPC_REG_DPCC_LLCC0_SHUB_CBCR_OUT(v)      \
        out_dword(HWIO_DPC_REG_DPCC_LLCC0_SHUB_CBCR_ADDR,v)
#define HWIO_DPC_REG_DPCC_LLCC0_SHUB_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DPC_REG_DPCC_LLCC0_SHUB_CBCR_ADDR,m,v,HWIO_DPC_REG_DPCC_LLCC0_SHUB_CBCR_IN)
#define HWIO_DPC_REG_DPCC_LLCC0_SHUB_CBCR_CLK_OFF_BMSK                                 0x80000000
#define HWIO_DPC_REG_DPCC_LLCC0_SHUB_CBCR_CLK_OFF_SHFT                                       0x1f
#define HWIO_DPC_REG_DPCC_LLCC0_SHUB_CBCR_CLK_ARES_BMSK                                       0x4
#define HWIO_DPC_REG_DPCC_LLCC0_SHUB_CBCR_CLK_ARES_SHFT                                       0x2
#define HWIO_DPC_REG_DPCC_LLCC0_SHUB_CBCR_CLK_ARES_NO_RESET_FVAL                              0x0
#define HWIO_DPC_REG_DPCC_LLCC0_SHUB_CBCR_CLK_ARES_RESET_FVAL                                 0x1
#define HWIO_DPC_REG_DPCC_LLCC0_SHUB_CBCR_CLK_ENABLE_BMSK                                     0x1
#define HWIO_DPC_REG_DPCC_LLCC0_SHUB_CBCR_CLK_ENABLE_SHFT                                     0x0
#define HWIO_DPC_REG_DPCC_LLCC0_SHUB_CBCR_CLK_ENABLE_DISABLE_FVAL                             0x0
#define HWIO_DPC_REG_DPCC_LLCC0_SHUB_CBCR_CLK_ENABLE_ENABLE_FVAL                              0x1

#define HWIO_DPC_REG_DPCC_LLCC1_SHUB_CBCR_ADDR                                         (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE      + 0x00000040)
#define HWIO_DPC_REG_DPCC_LLCC1_SHUB_CBCR_PHYS                                         (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_PHYS + 0x00000040)
#define HWIO_DPC_REG_DPCC_LLCC1_SHUB_CBCR_OFFS                                         (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_OFFS + 0x00000040)
#define HWIO_DPC_REG_DPCC_LLCC1_SHUB_CBCR_RMSK                                         0x80000005
#define HWIO_DPC_REG_DPCC_LLCC1_SHUB_CBCR_IN          \
        in_dword_masked(HWIO_DPC_REG_DPCC_LLCC1_SHUB_CBCR_ADDR, HWIO_DPC_REG_DPCC_LLCC1_SHUB_CBCR_RMSK)
#define HWIO_DPC_REG_DPCC_LLCC1_SHUB_CBCR_INM(m)      \
        in_dword_masked(HWIO_DPC_REG_DPCC_LLCC1_SHUB_CBCR_ADDR, m)
#define HWIO_DPC_REG_DPCC_LLCC1_SHUB_CBCR_OUT(v)      \
        out_dword(HWIO_DPC_REG_DPCC_LLCC1_SHUB_CBCR_ADDR,v)
#define HWIO_DPC_REG_DPCC_LLCC1_SHUB_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DPC_REG_DPCC_LLCC1_SHUB_CBCR_ADDR,m,v,HWIO_DPC_REG_DPCC_LLCC1_SHUB_CBCR_IN)
#define HWIO_DPC_REG_DPCC_LLCC1_SHUB_CBCR_CLK_OFF_BMSK                                 0x80000000
#define HWIO_DPC_REG_DPCC_LLCC1_SHUB_CBCR_CLK_OFF_SHFT                                       0x1f
#define HWIO_DPC_REG_DPCC_LLCC1_SHUB_CBCR_CLK_ARES_BMSK                                       0x4
#define HWIO_DPC_REG_DPCC_LLCC1_SHUB_CBCR_CLK_ARES_SHFT                                       0x2
#define HWIO_DPC_REG_DPCC_LLCC1_SHUB_CBCR_CLK_ARES_NO_RESET_FVAL                              0x0
#define HWIO_DPC_REG_DPCC_LLCC1_SHUB_CBCR_CLK_ARES_RESET_FVAL                                 0x1
#define HWIO_DPC_REG_DPCC_LLCC1_SHUB_CBCR_CLK_ENABLE_BMSK                                     0x1
#define HWIO_DPC_REG_DPCC_LLCC1_SHUB_CBCR_CLK_ENABLE_SHFT                                     0x0
#define HWIO_DPC_REG_DPCC_LLCC1_SHUB_CBCR_CLK_ENABLE_DISABLE_FVAL                             0x0
#define HWIO_DPC_REG_DPCC_LLCC1_SHUB_CBCR_CLK_ENABLE_ENABLE_FVAL                              0x1

#define HWIO_DPC_REG_DPCC_LLCC2_SHUB_CBCR_ADDR                                         (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE      + 0x00000044)
#define HWIO_DPC_REG_DPCC_LLCC2_SHUB_CBCR_PHYS                                         (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_PHYS + 0x00000044)
#define HWIO_DPC_REG_DPCC_LLCC2_SHUB_CBCR_OFFS                                         (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_OFFS + 0x00000044)
#define HWIO_DPC_REG_DPCC_LLCC2_SHUB_CBCR_RMSK                                         0x80000005
#define HWIO_DPC_REG_DPCC_LLCC2_SHUB_CBCR_IN          \
        in_dword_masked(HWIO_DPC_REG_DPCC_LLCC2_SHUB_CBCR_ADDR, HWIO_DPC_REG_DPCC_LLCC2_SHUB_CBCR_RMSK)
#define HWIO_DPC_REG_DPCC_LLCC2_SHUB_CBCR_INM(m)      \
        in_dword_masked(HWIO_DPC_REG_DPCC_LLCC2_SHUB_CBCR_ADDR, m)
#define HWIO_DPC_REG_DPCC_LLCC2_SHUB_CBCR_OUT(v)      \
        out_dword(HWIO_DPC_REG_DPCC_LLCC2_SHUB_CBCR_ADDR,v)
#define HWIO_DPC_REG_DPCC_LLCC2_SHUB_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DPC_REG_DPCC_LLCC2_SHUB_CBCR_ADDR,m,v,HWIO_DPC_REG_DPCC_LLCC2_SHUB_CBCR_IN)
#define HWIO_DPC_REG_DPCC_LLCC2_SHUB_CBCR_CLK_OFF_BMSK                                 0x80000000
#define HWIO_DPC_REG_DPCC_LLCC2_SHUB_CBCR_CLK_OFF_SHFT                                       0x1f
#define HWIO_DPC_REG_DPCC_LLCC2_SHUB_CBCR_CLK_ARES_BMSK                                       0x4
#define HWIO_DPC_REG_DPCC_LLCC2_SHUB_CBCR_CLK_ARES_SHFT                                       0x2
#define HWIO_DPC_REG_DPCC_LLCC2_SHUB_CBCR_CLK_ARES_NO_RESET_FVAL                              0x0
#define HWIO_DPC_REG_DPCC_LLCC2_SHUB_CBCR_CLK_ARES_RESET_FVAL                                 0x1
#define HWIO_DPC_REG_DPCC_LLCC2_SHUB_CBCR_CLK_ENABLE_BMSK                                     0x1
#define HWIO_DPC_REG_DPCC_LLCC2_SHUB_CBCR_CLK_ENABLE_SHFT                                     0x0
#define HWIO_DPC_REG_DPCC_LLCC2_SHUB_CBCR_CLK_ENABLE_DISABLE_FVAL                             0x0
#define HWIO_DPC_REG_DPCC_LLCC2_SHUB_CBCR_CLK_ENABLE_ENABLE_FVAL                              0x1

#define HWIO_DPC_REG_DPCC_LLCC3_SHUB_CBCR_ADDR                                         (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE      + 0x00000048)
#define HWIO_DPC_REG_DPCC_LLCC3_SHUB_CBCR_PHYS                                         (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_PHYS + 0x00000048)
#define HWIO_DPC_REG_DPCC_LLCC3_SHUB_CBCR_OFFS                                         (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_OFFS + 0x00000048)
#define HWIO_DPC_REG_DPCC_LLCC3_SHUB_CBCR_RMSK                                         0x80000005
#define HWIO_DPC_REG_DPCC_LLCC3_SHUB_CBCR_IN          \
        in_dword_masked(HWIO_DPC_REG_DPCC_LLCC3_SHUB_CBCR_ADDR, HWIO_DPC_REG_DPCC_LLCC3_SHUB_CBCR_RMSK)
#define HWIO_DPC_REG_DPCC_LLCC3_SHUB_CBCR_INM(m)      \
        in_dword_masked(HWIO_DPC_REG_DPCC_LLCC3_SHUB_CBCR_ADDR, m)
#define HWIO_DPC_REG_DPCC_LLCC3_SHUB_CBCR_OUT(v)      \
        out_dword(HWIO_DPC_REG_DPCC_LLCC3_SHUB_CBCR_ADDR,v)
#define HWIO_DPC_REG_DPCC_LLCC3_SHUB_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DPC_REG_DPCC_LLCC3_SHUB_CBCR_ADDR,m,v,HWIO_DPC_REG_DPCC_LLCC3_SHUB_CBCR_IN)
#define HWIO_DPC_REG_DPCC_LLCC3_SHUB_CBCR_CLK_OFF_BMSK                                 0x80000000
#define HWIO_DPC_REG_DPCC_LLCC3_SHUB_CBCR_CLK_OFF_SHFT                                       0x1f
#define HWIO_DPC_REG_DPCC_LLCC3_SHUB_CBCR_CLK_ARES_BMSK                                       0x4
#define HWIO_DPC_REG_DPCC_LLCC3_SHUB_CBCR_CLK_ARES_SHFT                                       0x2
#define HWIO_DPC_REG_DPCC_LLCC3_SHUB_CBCR_CLK_ARES_NO_RESET_FVAL                              0x0
#define HWIO_DPC_REG_DPCC_LLCC3_SHUB_CBCR_CLK_ARES_RESET_FVAL                                 0x1
#define HWIO_DPC_REG_DPCC_LLCC3_SHUB_CBCR_CLK_ENABLE_BMSK                                     0x1
#define HWIO_DPC_REG_DPCC_LLCC3_SHUB_CBCR_CLK_ENABLE_SHFT                                     0x0
#define HWIO_DPC_REG_DPCC_LLCC3_SHUB_CBCR_CLK_ENABLE_DISABLE_FVAL                             0x0
#define HWIO_DPC_REG_DPCC_LLCC3_SHUB_CBCR_CLK_ENABLE_ENABLE_FVAL                              0x1

#define HWIO_DPC_REG_DPCC_GLADIATOR_SHUB_CBCR_ADDR                                     (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE      + 0x0000004c)
#define HWIO_DPC_REG_DPCC_GLADIATOR_SHUB_CBCR_PHYS                                     (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_PHYS + 0x0000004c)
#define HWIO_DPC_REG_DPCC_GLADIATOR_SHUB_CBCR_OFFS                                     (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_OFFS + 0x0000004c)
#define HWIO_DPC_REG_DPCC_GLADIATOR_SHUB_CBCR_RMSK                                     0x80000005
#define HWIO_DPC_REG_DPCC_GLADIATOR_SHUB_CBCR_IN          \
        in_dword_masked(HWIO_DPC_REG_DPCC_GLADIATOR_SHUB_CBCR_ADDR, HWIO_DPC_REG_DPCC_GLADIATOR_SHUB_CBCR_RMSK)
#define HWIO_DPC_REG_DPCC_GLADIATOR_SHUB_CBCR_INM(m)      \
        in_dword_masked(HWIO_DPC_REG_DPCC_GLADIATOR_SHUB_CBCR_ADDR, m)
#define HWIO_DPC_REG_DPCC_GLADIATOR_SHUB_CBCR_OUT(v)      \
        out_dword(HWIO_DPC_REG_DPCC_GLADIATOR_SHUB_CBCR_ADDR,v)
#define HWIO_DPC_REG_DPCC_GLADIATOR_SHUB_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DPC_REG_DPCC_GLADIATOR_SHUB_CBCR_ADDR,m,v,HWIO_DPC_REG_DPCC_GLADIATOR_SHUB_CBCR_IN)
#define HWIO_DPC_REG_DPCC_GLADIATOR_SHUB_CBCR_CLK_OFF_BMSK                             0x80000000
#define HWIO_DPC_REG_DPCC_GLADIATOR_SHUB_CBCR_CLK_OFF_SHFT                                   0x1f
#define HWIO_DPC_REG_DPCC_GLADIATOR_SHUB_CBCR_CLK_ARES_BMSK                                   0x4
#define HWIO_DPC_REG_DPCC_GLADIATOR_SHUB_CBCR_CLK_ARES_SHFT                                   0x2
#define HWIO_DPC_REG_DPCC_GLADIATOR_SHUB_CBCR_CLK_ARES_NO_RESET_FVAL                          0x0
#define HWIO_DPC_REG_DPCC_GLADIATOR_SHUB_CBCR_CLK_ARES_RESET_FVAL                             0x1
#define HWIO_DPC_REG_DPCC_GLADIATOR_SHUB_CBCR_CLK_ENABLE_BMSK                                 0x1
#define HWIO_DPC_REG_DPCC_GLADIATOR_SHUB_CBCR_CLK_ENABLE_SHFT                                 0x0
#define HWIO_DPC_REG_DPCC_GLADIATOR_SHUB_CBCR_CLK_ENABLE_DISABLE_FVAL                         0x0
#define HWIO_DPC_REG_DPCC_GLADIATOR_SHUB_CBCR_CLK_ENABLE_ENABLE_FVAL                          0x1

#define HWIO_DPC_REG_DPCC_GEMNOC_SHUB_CBCR_ADDR                                        (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE      + 0x00000050)
#define HWIO_DPC_REG_DPCC_GEMNOC_SHUB_CBCR_PHYS                                        (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_PHYS + 0x00000050)
#define HWIO_DPC_REG_DPCC_GEMNOC_SHUB_CBCR_OFFS                                        (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_OFFS + 0x00000050)
#define HWIO_DPC_REG_DPCC_GEMNOC_SHUB_CBCR_RMSK                                        0x80000005
#define HWIO_DPC_REG_DPCC_GEMNOC_SHUB_CBCR_IN          \
        in_dword_masked(HWIO_DPC_REG_DPCC_GEMNOC_SHUB_CBCR_ADDR, HWIO_DPC_REG_DPCC_GEMNOC_SHUB_CBCR_RMSK)
#define HWIO_DPC_REG_DPCC_GEMNOC_SHUB_CBCR_INM(m)      \
        in_dword_masked(HWIO_DPC_REG_DPCC_GEMNOC_SHUB_CBCR_ADDR, m)
#define HWIO_DPC_REG_DPCC_GEMNOC_SHUB_CBCR_OUT(v)      \
        out_dword(HWIO_DPC_REG_DPCC_GEMNOC_SHUB_CBCR_ADDR,v)
#define HWIO_DPC_REG_DPCC_GEMNOC_SHUB_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DPC_REG_DPCC_GEMNOC_SHUB_CBCR_ADDR,m,v,HWIO_DPC_REG_DPCC_GEMNOC_SHUB_CBCR_IN)
#define HWIO_DPC_REG_DPCC_GEMNOC_SHUB_CBCR_CLK_OFF_BMSK                                0x80000000
#define HWIO_DPC_REG_DPCC_GEMNOC_SHUB_CBCR_CLK_OFF_SHFT                                      0x1f
#define HWIO_DPC_REG_DPCC_GEMNOC_SHUB_CBCR_CLK_ARES_BMSK                                      0x4
#define HWIO_DPC_REG_DPCC_GEMNOC_SHUB_CBCR_CLK_ARES_SHFT                                      0x2
#define HWIO_DPC_REG_DPCC_GEMNOC_SHUB_CBCR_CLK_ARES_NO_RESET_FVAL                             0x0
#define HWIO_DPC_REG_DPCC_GEMNOC_SHUB_CBCR_CLK_ARES_RESET_FVAL                                0x1
#define HWIO_DPC_REG_DPCC_GEMNOC_SHUB_CBCR_CLK_ENABLE_BMSK                                    0x1
#define HWIO_DPC_REG_DPCC_GEMNOC_SHUB_CBCR_CLK_ENABLE_SHFT                                    0x0
#define HWIO_DPC_REG_DPCC_GEMNOC_SHUB_CBCR_CLK_ENABLE_DISABLE_FVAL                            0x0
#define HWIO_DPC_REG_DPCC_GEMNOC_SHUB_CBCR_CLK_ENABLE_ENABLE_FVAL                             0x1

#define HWIO_DPC_REG_DPCC_CDSP_CBCR_ADDR                                               (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE      + 0x00000054)
#define HWIO_DPC_REG_DPCC_CDSP_CBCR_PHYS                                               (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_PHYS + 0x00000054)
#define HWIO_DPC_REG_DPCC_CDSP_CBCR_OFFS                                               (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_OFFS + 0x00000054)
#define HWIO_DPC_REG_DPCC_CDSP_CBCR_RMSK                                               0x80000005
#define HWIO_DPC_REG_DPCC_CDSP_CBCR_IN          \
        in_dword_masked(HWIO_DPC_REG_DPCC_CDSP_CBCR_ADDR, HWIO_DPC_REG_DPCC_CDSP_CBCR_RMSK)
#define HWIO_DPC_REG_DPCC_CDSP_CBCR_INM(m)      \
        in_dword_masked(HWIO_DPC_REG_DPCC_CDSP_CBCR_ADDR, m)
#define HWIO_DPC_REG_DPCC_CDSP_CBCR_OUT(v)      \
        out_dword(HWIO_DPC_REG_DPCC_CDSP_CBCR_ADDR,v)
#define HWIO_DPC_REG_DPCC_CDSP_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DPC_REG_DPCC_CDSP_CBCR_ADDR,m,v,HWIO_DPC_REG_DPCC_CDSP_CBCR_IN)
#define HWIO_DPC_REG_DPCC_CDSP_CBCR_CLK_OFF_BMSK                                       0x80000000
#define HWIO_DPC_REG_DPCC_CDSP_CBCR_CLK_OFF_SHFT                                             0x1f
#define HWIO_DPC_REG_DPCC_CDSP_CBCR_CLK_ARES_BMSK                                             0x4
#define HWIO_DPC_REG_DPCC_CDSP_CBCR_CLK_ARES_SHFT                                             0x2
#define HWIO_DPC_REG_DPCC_CDSP_CBCR_CLK_ARES_NO_RESET_FVAL                                    0x0
#define HWIO_DPC_REG_DPCC_CDSP_CBCR_CLK_ARES_RESET_FVAL                                       0x1
#define HWIO_DPC_REG_DPCC_CDSP_CBCR_CLK_ENABLE_BMSK                                           0x1
#define HWIO_DPC_REG_DPCC_CDSP_CBCR_CLK_ENABLE_SHFT                                           0x0
#define HWIO_DPC_REG_DPCC_CDSP_CBCR_CLK_ENABLE_DISABLE_FVAL                                   0x0
#define HWIO_DPC_REG_DPCC_CDSP_CBCR_CLK_ENABLE_ENABLE_FVAL                                    0x1

#define HWIO_DPC_REG_DPCC_GPU_CBCR_ADDR                                                (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE      + 0x00000058)
#define HWIO_DPC_REG_DPCC_GPU_CBCR_PHYS                                                (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_PHYS + 0x00000058)
#define HWIO_DPC_REG_DPCC_GPU_CBCR_OFFS                                                (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_OFFS + 0x00000058)
#define HWIO_DPC_REG_DPCC_GPU_CBCR_RMSK                                                0x80000005
#define HWIO_DPC_REG_DPCC_GPU_CBCR_IN          \
        in_dword_masked(HWIO_DPC_REG_DPCC_GPU_CBCR_ADDR, HWIO_DPC_REG_DPCC_GPU_CBCR_RMSK)
#define HWIO_DPC_REG_DPCC_GPU_CBCR_INM(m)      \
        in_dword_masked(HWIO_DPC_REG_DPCC_GPU_CBCR_ADDR, m)
#define HWIO_DPC_REG_DPCC_GPU_CBCR_OUT(v)      \
        out_dword(HWIO_DPC_REG_DPCC_GPU_CBCR_ADDR,v)
#define HWIO_DPC_REG_DPCC_GPU_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DPC_REG_DPCC_GPU_CBCR_ADDR,m,v,HWIO_DPC_REG_DPCC_GPU_CBCR_IN)
#define HWIO_DPC_REG_DPCC_GPU_CBCR_CLK_OFF_BMSK                                        0x80000000
#define HWIO_DPC_REG_DPCC_GPU_CBCR_CLK_OFF_SHFT                                              0x1f
#define HWIO_DPC_REG_DPCC_GPU_CBCR_CLK_ARES_BMSK                                              0x4
#define HWIO_DPC_REG_DPCC_GPU_CBCR_CLK_ARES_SHFT                                              0x2
#define HWIO_DPC_REG_DPCC_GPU_CBCR_CLK_ARES_NO_RESET_FVAL                                     0x0
#define HWIO_DPC_REG_DPCC_GPU_CBCR_CLK_ARES_RESET_FVAL                                        0x1
#define HWIO_DPC_REG_DPCC_GPU_CBCR_CLK_ENABLE_BMSK                                            0x1
#define HWIO_DPC_REG_DPCC_GPU_CBCR_CLK_ENABLE_SHFT                                            0x0
#define HWIO_DPC_REG_DPCC_GPU_CBCR_CLK_ENABLE_DISABLE_FVAL                                    0x0
#define HWIO_DPC_REG_DPCC_GPU_CBCR_CLK_ENABLE_ENABLE_FVAL                                     0x1

#define HWIO_DPC_REG_DPCC_MDSP_CBCR_ADDR                                               (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE      + 0x0000005c)
#define HWIO_DPC_REG_DPCC_MDSP_CBCR_PHYS                                               (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_PHYS + 0x0000005c)
#define HWIO_DPC_REG_DPCC_MDSP_CBCR_OFFS                                               (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_OFFS + 0x0000005c)
#define HWIO_DPC_REG_DPCC_MDSP_CBCR_RMSK                                               0x80000005
#define HWIO_DPC_REG_DPCC_MDSP_CBCR_IN          \
        in_dword_masked(HWIO_DPC_REG_DPCC_MDSP_CBCR_ADDR, HWIO_DPC_REG_DPCC_MDSP_CBCR_RMSK)
#define HWIO_DPC_REG_DPCC_MDSP_CBCR_INM(m)      \
        in_dword_masked(HWIO_DPC_REG_DPCC_MDSP_CBCR_ADDR, m)
#define HWIO_DPC_REG_DPCC_MDSP_CBCR_OUT(v)      \
        out_dword(HWIO_DPC_REG_DPCC_MDSP_CBCR_ADDR,v)
#define HWIO_DPC_REG_DPCC_MDSP_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DPC_REG_DPCC_MDSP_CBCR_ADDR,m,v,HWIO_DPC_REG_DPCC_MDSP_CBCR_IN)
#define HWIO_DPC_REG_DPCC_MDSP_CBCR_CLK_OFF_BMSK                                       0x80000000
#define HWIO_DPC_REG_DPCC_MDSP_CBCR_CLK_OFF_SHFT                                             0x1f
#define HWIO_DPC_REG_DPCC_MDSP_CBCR_CLK_ARES_BMSK                                             0x4
#define HWIO_DPC_REG_DPCC_MDSP_CBCR_CLK_ARES_SHFT                                             0x2
#define HWIO_DPC_REG_DPCC_MDSP_CBCR_CLK_ARES_NO_RESET_FVAL                                    0x0
#define HWIO_DPC_REG_DPCC_MDSP_CBCR_CLK_ARES_RESET_FVAL                                       0x1
#define HWIO_DPC_REG_DPCC_MDSP_CBCR_CLK_ENABLE_BMSK                                           0x1
#define HWIO_DPC_REG_DPCC_MDSP_CBCR_CLK_ENABLE_SHFT                                           0x0
#define HWIO_DPC_REG_DPCC_MDSP_CBCR_CLK_ENABLE_DISABLE_FVAL                                   0x0
#define HWIO_DPC_REG_DPCC_MDSP_CBCR_CLK_ENABLE_ENABLE_FVAL                                    0x1

#define HWIO_DPC_REG_DPCC_MNOC_HF_CBCR_ADDR                                            (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE      + 0x00000060)
#define HWIO_DPC_REG_DPCC_MNOC_HF_CBCR_PHYS                                            (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_PHYS + 0x00000060)
#define HWIO_DPC_REG_DPCC_MNOC_HF_CBCR_OFFS                                            (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_OFFS + 0x00000060)
#define HWIO_DPC_REG_DPCC_MNOC_HF_CBCR_RMSK                                            0x80000005
#define HWIO_DPC_REG_DPCC_MNOC_HF_CBCR_IN          \
        in_dword_masked(HWIO_DPC_REG_DPCC_MNOC_HF_CBCR_ADDR, HWIO_DPC_REG_DPCC_MNOC_HF_CBCR_RMSK)
#define HWIO_DPC_REG_DPCC_MNOC_HF_CBCR_INM(m)      \
        in_dword_masked(HWIO_DPC_REG_DPCC_MNOC_HF_CBCR_ADDR, m)
#define HWIO_DPC_REG_DPCC_MNOC_HF_CBCR_OUT(v)      \
        out_dword(HWIO_DPC_REG_DPCC_MNOC_HF_CBCR_ADDR,v)
#define HWIO_DPC_REG_DPCC_MNOC_HF_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DPC_REG_DPCC_MNOC_HF_CBCR_ADDR,m,v,HWIO_DPC_REG_DPCC_MNOC_HF_CBCR_IN)
#define HWIO_DPC_REG_DPCC_MNOC_HF_CBCR_CLK_OFF_BMSK                                    0x80000000
#define HWIO_DPC_REG_DPCC_MNOC_HF_CBCR_CLK_OFF_SHFT                                          0x1f
#define HWIO_DPC_REG_DPCC_MNOC_HF_CBCR_CLK_ARES_BMSK                                          0x4
#define HWIO_DPC_REG_DPCC_MNOC_HF_CBCR_CLK_ARES_SHFT                                          0x2
#define HWIO_DPC_REG_DPCC_MNOC_HF_CBCR_CLK_ARES_NO_RESET_FVAL                                 0x0
#define HWIO_DPC_REG_DPCC_MNOC_HF_CBCR_CLK_ARES_RESET_FVAL                                    0x1
#define HWIO_DPC_REG_DPCC_MNOC_HF_CBCR_CLK_ENABLE_BMSK                                        0x1
#define HWIO_DPC_REG_DPCC_MNOC_HF_CBCR_CLK_ENABLE_SHFT                                        0x0
#define HWIO_DPC_REG_DPCC_MNOC_HF_CBCR_CLK_ENABLE_DISABLE_FVAL                                0x0
#define HWIO_DPC_REG_DPCC_MNOC_HF_CBCR_CLK_ENABLE_ENABLE_FVAL                                 0x1

#define HWIO_DPC_REG_DPCC_MNOC_SF_CBCR_ADDR                                            (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE      + 0x00000064)
#define HWIO_DPC_REG_DPCC_MNOC_SF_CBCR_PHYS                                            (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_PHYS + 0x00000064)
#define HWIO_DPC_REG_DPCC_MNOC_SF_CBCR_OFFS                                            (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_OFFS + 0x00000064)
#define HWIO_DPC_REG_DPCC_MNOC_SF_CBCR_RMSK                                            0x80000005
#define HWIO_DPC_REG_DPCC_MNOC_SF_CBCR_IN          \
        in_dword_masked(HWIO_DPC_REG_DPCC_MNOC_SF_CBCR_ADDR, HWIO_DPC_REG_DPCC_MNOC_SF_CBCR_RMSK)
#define HWIO_DPC_REG_DPCC_MNOC_SF_CBCR_INM(m)      \
        in_dword_masked(HWIO_DPC_REG_DPCC_MNOC_SF_CBCR_ADDR, m)
#define HWIO_DPC_REG_DPCC_MNOC_SF_CBCR_OUT(v)      \
        out_dword(HWIO_DPC_REG_DPCC_MNOC_SF_CBCR_ADDR,v)
#define HWIO_DPC_REG_DPCC_MNOC_SF_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DPC_REG_DPCC_MNOC_SF_CBCR_ADDR,m,v,HWIO_DPC_REG_DPCC_MNOC_SF_CBCR_IN)
#define HWIO_DPC_REG_DPCC_MNOC_SF_CBCR_CLK_OFF_BMSK                                    0x80000000
#define HWIO_DPC_REG_DPCC_MNOC_SF_CBCR_CLK_OFF_SHFT                                          0x1f
#define HWIO_DPC_REG_DPCC_MNOC_SF_CBCR_CLK_ARES_BMSK                                          0x4
#define HWIO_DPC_REG_DPCC_MNOC_SF_CBCR_CLK_ARES_SHFT                                          0x2
#define HWIO_DPC_REG_DPCC_MNOC_SF_CBCR_CLK_ARES_NO_RESET_FVAL                                 0x0
#define HWIO_DPC_REG_DPCC_MNOC_SF_CBCR_CLK_ARES_RESET_FVAL                                    0x1
#define HWIO_DPC_REG_DPCC_MNOC_SF_CBCR_CLK_ENABLE_BMSK                                        0x1
#define HWIO_DPC_REG_DPCC_MNOC_SF_CBCR_CLK_ENABLE_SHFT                                        0x0
#define HWIO_DPC_REG_DPCC_MNOC_SF_CBCR_CLK_ENABLE_DISABLE_FVAL                                0x0
#define HWIO_DPC_REG_DPCC_MNOC_SF_CBCR_CLK_ENABLE_ENABLE_FVAL                                 0x1

#define HWIO_DPC_REG_DPCC_SNOC_CBCR_ADDR                                               (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE      + 0x00000068)
#define HWIO_DPC_REG_DPCC_SNOC_CBCR_PHYS                                               (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_PHYS + 0x00000068)
#define HWIO_DPC_REG_DPCC_SNOC_CBCR_OFFS                                               (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_OFFS + 0x00000068)
#define HWIO_DPC_REG_DPCC_SNOC_CBCR_RMSK                                               0x80000005
#define HWIO_DPC_REG_DPCC_SNOC_CBCR_IN          \
        in_dword_masked(HWIO_DPC_REG_DPCC_SNOC_CBCR_ADDR, HWIO_DPC_REG_DPCC_SNOC_CBCR_RMSK)
#define HWIO_DPC_REG_DPCC_SNOC_CBCR_INM(m)      \
        in_dword_masked(HWIO_DPC_REG_DPCC_SNOC_CBCR_ADDR, m)
#define HWIO_DPC_REG_DPCC_SNOC_CBCR_OUT(v)      \
        out_dword(HWIO_DPC_REG_DPCC_SNOC_CBCR_ADDR,v)
#define HWIO_DPC_REG_DPCC_SNOC_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DPC_REG_DPCC_SNOC_CBCR_ADDR,m,v,HWIO_DPC_REG_DPCC_SNOC_CBCR_IN)
#define HWIO_DPC_REG_DPCC_SNOC_CBCR_CLK_OFF_BMSK                                       0x80000000
#define HWIO_DPC_REG_DPCC_SNOC_CBCR_CLK_OFF_SHFT                                             0x1f
#define HWIO_DPC_REG_DPCC_SNOC_CBCR_CLK_ARES_BMSK                                             0x4
#define HWIO_DPC_REG_DPCC_SNOC_CBCR_CLK_ARES_SHFT                                             0x2
#define HWIO_DPC_REG_DPCC_SNOC_CBCR_CLK_ARES_NO_RESET_FVAL                                    0x0
#define HWIO_DPC_REG_DPCC_SNOC_CBCR_CLK_ARES_RESET_FVAL                                       0x1
#define HWIO_DPC_REG_DPCC_SNOC_CBCR_CLK_ENABLE_BMSK                                           0x1
#define HWIO_DPC_REG_DPCC_SNOC_CBCR_CLK_ENABLE_SHFT                                           0x0
#define HWIO_DPC_REG_DPCC_SNOC_CBCR_CLK_ENABLE_DISABLE_FVAL                                   0x0
#define HWIO_DPC_REG_DPCC_SNOC_CBCR_CLK_ENABLE_ENABLE_FVAL                                    0x1

#define HWIO_DPC_REG_DPCC_SNOC_GC_CBCR_ADDR                                            (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE      + 0x0000006c)
#define HWIO_DPC_REG_DPCC_SNOC_GC_CBCR_PHYS                                            (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_PHYS + 0x0000006c)
#define HWIO_DPC_REG_DPCC_SNOC_GC_CBCR_OFFS                                            (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_OFFS + 0x0000006c)
#define HWIO_DPC_REG_DPCC_SNOC_GC_CBCR_RMSK                                            0x80000005
#define HWIO_DPC_REG_DPCC_SNOC_GC_CBCR_IN          \
        in_dword_masked(HWIO_DPC_REG_DPCC_SNOC_GC_CBCR_ADDR, HWIO_DPC_REG_DPCC_SNOC_GC_CBCR_RMSK)
#define HWIO_DPC_REG_DPCC_SNOC_GC_CBCR_INM(m)      \
        in_dword_masked(HWIO_DPC_REG_DPCC_SNOC_GC_CBCR_ADDR, m)
#define HWIO_DPC_REG_DPCC_SNOC_GC_CBCR_OUT(v)      \
        out_dword(HWIO_DPC_REG_DPCC_SNOC_GC_CBCR_ADDR,v)
#define HWIO_DPC_REG_DPCC_SNOC_GC_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DPC_REG_DPCC_SNOC_GC_CBCR_ADDR,m,v,HWIO_DPC_REG_DPCC_SNOC_GC_CBCR_IN)
#define HWIO_DPC_REG_DPCC_SNOC_GC_CBCR_CLK_OFF_BMSK                                    0x80000000
#define HWIO_DPC_REG_DPCC_SNOC_GC_CBCR_CLK_OFF_SHFT                                          0x1f
#define HWIO_DPC_REG_DPCC_SNOC_GC_CBCR_CLK_ARES_BMSK                                          0x4
#define HWIO_DPC_REG_DPCC_SNOC_GC_CBCR_CLK_ARES_SHFT                                          0x2
#define HWIO_DPC_REG_DPCC_SNOC_GC_CBCR_CLK_ARES_NO_RESET_FVAL                                 0x0
#define HWIO_DPC_REG_DPCC_SNOC_GC_CBCR_CLK_ARES_RESET_FVAL                                    0x1
#define HWIO_DPC_REG_DPCC_SNOC_GC_CBCR_CLK_ENABLE_BMSK                                        0x1
#define HWIO_DPC_REG_DPCC_SNOC_GC_CBCR_CLK_ENABLE_SHFT                                        0x0
#define HWIO_DPC_REG_DPCC_SNOC_GC_CBCR_CLK_ENABLE_DISABLE_FVAL                                0x0
#define HWIO_DPC_REG_DPCC_SNOC_GC_CBCR_CLK_ENABLE_ENABLE_FVAL                                 0x1

#define HWIO_DPC_REG_DPCC_SNOC_HS_CBCR_ADDR                                            (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE      + 0x00000070)
#define HWIO_DPC_REG_DPCC_SNOC_HS_CBCR_PHYS                                            (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_PHYS + 0x00000070)
#define HWIO_DPC_REG_DPCC_SNOC_HS_CBCR_OFFS                                            (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_OFFS + 0x00000070)
#define HWIO_DPC_REG_DPCC_SNOC_HS_CBCR_RMSK                                            0x80000005
#define HWIO_DPC_REG_DPCC_SNOC_HS_CBCR_IN          \
        in_dword_masked(HWIO_DPC_REG_DPCC_SNOC_HS_CBCR_ADDR, HWIO_DPC_REG_DPCC_SNOC_HS_CBCR_RMSK)
#define HWIO_DPC_REG_DPCC_SNOC_HS_CBCR_INM(m)      \
        in_dword_masked(HWIO_DPC_REG_DPCC_SNOC_HS_CBCR_ADDR, m)
#define HWIO_DPC_REG_DPCC_SNOC_HS_CBCR_OUT(v)      \
        out_dword(HWIO_DPC_REG_DPCC_SNOC_HS_CBCR_ADDR,v)
#define HWIO_DPC_REG_DPCC_SNOC_HS_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DPC_REG_DPCC_SNOC_HS_CBCR_ADDR,m,v,HWIO_DPC_REG_DPCC_SNOC_HS_CBCR_IN)
#define HWIO_DPC_REG_DPCC_SNOC_HS_CBCR_CLK_OFF_BMSK                                    0x80000000
#define HWIO_DPC_REG_DPCC_SNOC_HS_CBCR_CLK_OFF_SHFT                                          0x1f
#define HWIO_DPC_REG_DPCC_SNOC_HS_CBCR_CLK_ARES_BMSK                                          0x4
#define HWIO_DPC_REG_DPCC_SNOC_HS_CBCR_CLK_ARES_SHFT                                          0x2
#define HWIO_DPC_REG_DPCC_SNOC_HS_CBCR_CLK_ARES_NO_RESET_FVAL                                 0x0
#define HWIO_DPC_REG_DPCC_SNOC_HS_CBCR_CLK_ARES_RESET_FVAL                                    0x1
#define HWIO_DPC_REG_DPCC_SNOC_HS_CBCR_CLK_ENABLE_BMSK                                        0x1
#define HWIO_DPC_REG_DPCC_SNOC_HS_CBCR_CLK_ENABLE_SHFT                                        0x0
#define HWIO_DPC_REG_DPCC_SNOC_HS_CBCR_CLK_ENABLE_DISABLE_FVAL                                0x0
#define HWIO_DPC_REG_DPCC_SNOC_HS_CBCR_CLK_ENABLE_ENABLE_FVAL                                 0x1

#define HWIO_DPC_REG_DPCC_TCU_CBCR_ADDR                                                (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE      + 0x00000074)
#define HWIO_DPC_REG_DPCC_TCU_CBCR_PHYS                                                (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_PHYS + 0x00000074)
#define HWIO_DPC_REG_DPCC_TCU_CBCR_OFFS                                                (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_OFFS + 0x00000074)
#define HWIO_DPC_REG_DPCC_TCU_CBCR_RMSK                                                0x80000005
#define HWIO_DPC_REG_DPCC_TCU_CBCR_IN          \
        in_dword_masked(HWIO_DPC_REG_DPCC_TCU_CBCR_ADDR, HWIO_DPC_REG_DPCC_TCU_CBCR_RMSK)
#define HWIO_DPC_REG_DPCC_TCU_CBCR_INM(m)      \
        in_dword_masked(HWIO_DPC_REG_DPCC_TCU_CBCR_ADDR, m)
#define HWIO_DPC_REG_DPCC_TCU_CBCR_OUT(v)      \
        out_dword(HWIO_DPC_REG_DPCC_TCU_CBCR_ADDR,v)
#define HWIO_DPC_REG_DPCC_TCU_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DPC_REG_DPCC_TCU_CBCR_ADDR,m,v,HWIO_DPC_REG_DPCC_TCU_CBCR_IN)
#define HWIO_DPC_REG_DPCC_TCU_CBCR_CLK_OFF_BMSK                                        0x80000000
#define HWIO_DPC_REG_DPCC_TCU_CBCR_CLK_OFF_SHFT                                              0x1f
#define HWIO_DPC_REG_DPCC_TCU_CBCR_CLK_ARES_BMSK                                              0x4
#define HWIO_DPC_REG_DPCC_TCU_CBCR_CLK_ARES_SHFT                                              0x2
#define HWIO_DPC_REG_DPCC_TCU_CBCR_CLK_ARES_NO_RESET_FVAL                                     0x0
#define HWIO_DPC_REG_DPCC_TCU_CBCR_CLK_ARES_RESET_FVAL                                        0x1
#define HWIO_DPC_REG_DPCC_TCU_CBCR_CLK_ENABLE_BMSK                                            0x1
#define HWIO_DPC_REG_DPCC_TCU_CBCR_CLK_ENABLE_SHFT                                            0x0
#define HWIO_DPC_REG_DPCC_TCU_CBCR_CLK_ENABLE_DISABLE_FVAL                                    0x0
#define HWIO_DPC_REG_DPCC_TCU_CBCR_CLK_ENABLE_ENABLE_FVAL                                     0x1

#define HWIO_DPC_REG_DPCC_SHUB_CFG_CBCR_ADDR                                           (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE      + 0x00000078)
#define HWIO_DPC_REG_DPCC_SHUB_CFG_CBCR_PHYS                                           (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_PHYS + 0x00000078)
#define HWIO_DPC_REG_DPCC_SHUB_CFG_CBCR_OFFS                                           (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_OFFS + 0x00000078)
#define HWIO_DPC_REG_DPCC_SHUB_CFG_CBCR_RMSK                                           0x80000005
#define HWIO_DPC_REG_DPCC_SHUB_CFG_CBCR_IN          \
        in_dword_masked(HWIO_DPC_REG_DPCC_SHUB_CFG_CBCR_ADDR, HWIO_DPC_REG_DPCC_SHUB_CFG_CBCR_RMSK)
#define HWIO_DPC_REG_DPCC_SHUB_CFG_CBCR_INM(m)      \
        in_dword_masked(HWIO_DPC_REG_DPCC_SHUB_CFG_CBCR_ADDR, m)
#define HWIO_DPC_REG_DPCC_SHUB_CFG_CBCR_OUT(v)      \
        out_dword(HWIO_DPC_REG_DPCC_SHUB_CFG_CBCR_ADDR,v)
#define HWIO_DPC_REG_DPCC_SHUB_CFG_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DPC_REG_DPCC_SHUB_CFG_CBCR_ADDR,m,v,HWIO_DPC_REG_DPCC_SHUB_CFG_CBCR_IN)
#define HWIO_DPC_REG_DPCC_SHUB_CFG_CBCR_CLK_OFF_BMSK                                   0x80000000
#define HWIO_DPC_REG_DPCC_SHUB_CFG_CBCR_CLK_OFF_SHFT                                         0x1f
#define HWIO_DPC_REG_DPCC_SHUB_CFG_CBCR_CLK_ARES_BMSK                                         0x4
#define HWIO_DPC_REG_DPCC_SHUB_CFG_CBCR_CLK_ARES_SHFT                                         0x2
#define HWIO_DPC_REG_DPCC_SHUB_CFG_CBCR_CLK_ARES_NO_RESET_FVAL                                0x0
#define HWIO_DPC_REG_DPCC_SHUB_CFG_CBCR_CLK_ARES_RESET_FVAL                                   0x1
#define HWIO_DPC_REG_DPCC_SHUB_CFG_CBCR_CLK_ENABLE_BMSK                                       0x1
#define HWIO_DPC_REG_DPCC_SHUB_CFG_CBCR_CLK_ENABLE_SHFT                                       0x0
#define HWIO_DPC_REG_DPCC_SHUB_CFG_CBCR_CLK_ENABLE_DISABLE_FVAL                               0x0
#define HWIO_DPC_REG_DPCC_SHUB_CFG_CBCR_CLK_ENABLE_ENABLE_FVAL                                0x1

#define HWIO_DPC_REG_DPCC_SHUB_ATB_CBCR_ADDR                                           (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE      + 0x0000007c)
#define HWIO_DPC_REG_DPCC_SHUB_ATB_CBCR_PHYS                                           (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_PHYS + 0x0000007c)
#define HWIO_DPC_REG_DPCC_SHUB_ATB_CBCR_OFFS                                           (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_OFFS + 0x0000007c)
#define HWIO_DPC_REG_DPCC_SHUB_ATB_CBCR_RMSK                                           0x80000005
#define HWIO_DPC_REG_DPCC_SHUB_ATB_CBCR_IN          \
        in_dword_masked(HWIO_DPC_REG_DPCC_SHUB_ATB_CBCR_ADDR, HWIO_DPC_REG_DPCC_SHUB_ATB_CBCR_RMSK)
#define HWIO_DPC_REG_DPCC_SHUB_ATB_CBCR_INM(m)      \
        in_dword_masked(HWIO_DPC_REG_DPCC_SHUB_ATB_CBCR_ADDR, m)
#define HWIO_DPC_REG_DPCC_SHUB_ATB_CBCR_OUT(v)      \
        out_dword(HWIO_DPC_REG_DPCC_SHUB_ATB_CBCR_ADDR,v)
#define HWIO_DPC_REG_DPCC_SHUB_ATB_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DPC_REG_DPCC_SHUB_ATB_CBCR_ADDR,m,v,HWIO_DPC_REG_DPCC_SHUB_ATB_CBCR_IN)
#define HWIO_DPC_REG_DPCC_SHUB_ATB_CBCR_CLK_OFF_BMSK                                   0x80000000
#define HWIO_DPC_REG_DPCC_SHUB_ATB_CBCR_CLK_OFF_SHFT                                         0x1f
#define HWIO_DPC_REG_DPCC_SHUB_ATB_CBCR_CLK_ARES_BMSK                                         0x4
#define HWIO_DPC_REG_DPCC_SHUB_ATB_CBCR_CLK_ARES_SHFT                                         0x2
#define HWIO_DPC_REG_DPCC_SHUB_ATB_CBCR_CLK_ARES_NO_RESET_FVAL                                0x0
#define HWIO_DPC_REG_DPCC_SHUB_ATB_CBCR_CLK_ARES_RESET_FVAL                                   0x1
#define HWIO_DPC_REG_DPCC_SHUB_ATB_CBCR_CLK_ENABLE_BMSK                                       0x1
#define HWIO_DPC_REG_DPCC_SHUB_ATB_CBCR_CLK_ENABLE_SHFT                                       0x0
#define HWIO_DPC_REG_DPCC_SHUB_ATB_CBCR_CLK_ENABLE_DISABLE_FVAL                               0x0
#define HWIO_DPC_REG_DPCC_SHUB_ATB_CBCR_CLK_ENABLE_ENABLE_FVAL                                0x1

#define HWIO_DPC_REG_DPCC_PCIE_SF_CBCR_ADDR                                            (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE      + 0x00000080)
#define HWIO_DPC_REG_DPCC_PCIE_SF_CBCR_PHYS                                            (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_PHYS + 0x00000080)
#define HWIO_DPC_REG_DPCC_PCIE_SF_CBCR_OFFS                                            (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_OFFS + 0x00000080)
#define HWIO_DPC_REG_DPCC_PCIE_SF_CBCR_RMSK                                            0x80000005
#define HWIO_DPC_REG_DPCC_PCIE_SF_CBCR_IN          \
        in_dword_masked(HWIO_DPC_REG_DPCC_PCIE_SF_CBCR_ADDR, HWIO_DPC_REG_DPCC_PCIE_SF_CBCR_RMSK)
#define HWIO_DPC_REG_DPCC_PCIE_SF_CBCR_INM(m)      \
        in_dword_masked(HWIO_DPC_REG_DPCC_PCIE_SF_CBCR_ADDR, m)
#define HWIO_DPC_REG_DPCC_PCIE_SF_CBCR_OUT(v)      \
        out_dword(HWIO_DPC_REG_DPCC_PCIE_SF_CBCR_ADDR,v)
#define HWIO_DPC_REG_DPCC_PCIE_SF_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DPC_REG_DPCC_PCIE_SF_CBCR_ADDR,m,v,HWIO_DPC_REG_DPCC_PCIE_SF_CBCR_IN)
#define HWIO_DPC_REG_DPCC_PCIE_SF_CBCR_CLK_OFF_BMSK                                    0x80000000
#define HWIO_DPC_REG_DPCC_PCIE_SF_CBCR_CLK_OFF_SHFT                                          0x1f
#define HWIO_DPC_REG_DPCC_PCIE_SF_CBCR_CLK_ARES_BMSK                                          0x4
#define HWIO_DPC_REG_DPCC_PCIE_SF_CBCR_CLK_ARES_SHFT                                          0x2
#define HWIO_DPC_REG_DPCC_PCIE_SF_CBCR_CLK_ARES_NO_RESET_FVAL                                 0x0
#define HWIO_DPC_REG_DPCC_PCIE_SF_CBCR_CLK_ARES_RESET_FVAL                                    0x1
#define HWIO_DPC_REG_DPCC_PCIE_SF_CBCR_CLK_ENABLE_BMSK                                        0x1
#define HWIO_DPC_REG_DPCC_PCIE_SF_CBCR_CLK_ENABLE_SHFT                                        0x0
#define HWIO_DPC_REG_DPCC_PCIE_SF_CBCR_CLK_ENABLE_DISABLE_FVAL                                0x0
#define HWIO_DPC_REG_DPCC_PCIE_SF_CBCR_CLK_ENABLE_ENABLE_FVAL                                 0x1

#define HWIO_DPC_REG_DPCC_GEMNOC_DRE_CBCR_ADDR                                         (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE      + 0x00000084)
#define HWIO_DPC_REG_DPCC_GEMNOC_DRE_CBCR_PHYS                                         (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_PHYS + 0x00000084)
#define HWIO_DPC_REG_DPCC_GEMNOC_DRE_CBCR_OFFS                                         (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_OFFS + 0x00000084)
#define HWIO_DPC_REG_DPCC_GEMNOC_DRE_CBCR_RMSK                                         0x80000005
#define HWIO_DPC_REG_DPCC_GEMNOC_DRE_CBCR_IN          \
        in_dword_masked(HWIO_DPC_REG_DPCC_GEMNOC_DRE_CBCR_ADDR, HWIO_DPC_REG_DPCC_GEMNOC_DRE_CBCR_RMSK)
#define HWIO_DPC_REG_DPCC_GEMNOC_DRE_CBCR_INM(m)      \
        in_dword_masked(HWIO_DPC_REG_DPCC_GEMNOC_DRE_CBCR_ADDR, m)
#define HWIO_DPC_REG_DPCC_GEMNOC_DRE_CBCR_OUT(v)      \
        out_dword(HWIO_DPC_REG_DPCC_GEMNOC_DRE_CBCR_ADDR,v)
#define HWIO_DPC_REG_DPCC_GEMNOC_DRE_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DPC_REG_DPCC_GEMNOC_DRE_CBCR_ADDR,m,v,HWIO_DPC_REG_DPCC_GEMNOC_DRE_CBCR_IN)
#define HWIO_DPC_REG_DPCC_GEMNOC_DRE_CBCR_CLK_OFF_BMSK                                 0x80000000
#define HWIO_DPC_REG_DPCC_GEMNOC_DRE_CBCR_CLK_OFF_SHFT                                       0x1f
#define HWIO_DPC_REG_DPCC_GEMNOC_DRE_CBCR_CLK_ARES_BMSK                                       0x4
#define HWIO_DPC_REG_DPCC_GEMNOC_DRE_CBCR_CLK_ARES_SHFT                                       0x2
#define HWIO_DPC_REG_DPCC_GEMNOC_DRE_CBCR_CLK_ARES_NO_RESET_FVAL                              0x0
#define HWIO_DPC_REG_DPCC_GEMNOC_DRE_CBCR_CLK_ARES_RESET_FVAL                                 0x1
#define HWIO_DPC_REG_DPCC_GEMNOC_DRE_CBCR_CLK_ENABLE_BMSK                                     0x1
#define HWIO_DPC_REG_DPCC_GEMNOC_DRE_CBCR_CLK_ENABLE_SHFT                                     0x0
#define HWIO_DPC_REG_DPCC_GEMNOC_DRE_CBCR_CLK_ENABLE_DISABLE_FVAL                             0x0
#define HWIO_DPC_REG_DPCC_GEMNOC_DRE_CBCR_CLK_ENABLE_ENABLE_FVAL                              0x1

#define HWIO_DPC_REG_DPCC_DRE_BCR_ADDR                                                 (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE      + 0x00000088)
#define HWIO_DPC_REG_DPCC_DRE_BCR_PHYS                                                 (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_PHYS + 0x00000088)
#define HWIO_DPC_REG_DPCC_DRE_BCR_OFFS                                                 (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_OFFS + 0x00000088)
#define HWIO_DPC_REG_DPCC_DRE_BCR_RMSK                                                        0x1
#define HWIO_DPC_REG_DPCC_DRE_BCR_IN          \
        in_dword_masked(HWIO_DPC_REG_DPCC_DRE_BCR_ADDR, HWIO_DPC_REG_DPCC_DRE_BCR_RMSK)
#define HWIO_DPC_REG_DPCC_DRE_BCR_INM(m)      \
        in_dword_masked(HWIO_DPC_REG_DPCC_DRE_BCR_ADDR, m)
#define HWIO_DPC_REG_DPCC_DRE_BCR_OUT(v)      \
        out_dword(HWIO_DPC_REG_DPCC_DRE_BCR_ADDR,v)
#define HWIO_DPC_REG_DPCC_DRE_BCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DPC_REG_DPCC_DRE_BCR_ADDR,m,v,HWIO_DPC_REG_DPCC_DRE_BCR_IN)
#define HWIO_DPC_REG_DPCC_DRE_BCR_BLK_ARES_BMSK                                               0x1
#define HWIO_DPC_REG_DPCC_DRE_BCR_BLK_ARES_SHFT                                               0x0
#define HWIO_DPC_REG_DPCC_DRE_BCR_BLK_ARES_DISABLE_FVAL                                       0x0
#define HWIO_DPC_REG_DPCC_DRE_BCR_BLK_ARES_ENABLE_FVAL                                        0x1

#define HWIO_DPC_REG_DPCC_DRE_GDSCR_ADDR                                               (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE      + 0x0000008c)
#define HWIO_DPC_REG_DPCC_DRE_GDSCR_PHYS                                               (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_PHYS + 0x0000008c)
#define HWIO_DPC_REG_DPCC_DRE_GDSCR_OFFS                                               (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_OFFS + 0x0000008c)
#define HWIO_DPC_REG_DPCC_DRE_GDSCR_RMSK                                               0xf8ffffff
#define HWIO_DPC_REG_DPCC_DRE_GDSCR_IN          \
        in_dword_masked(HWIO_DPC_REG_DPCC_DRE_GDSCR_ADDR, HWIO_DPC_REG_DPCC_DRE_GDSCR_RMSK)
#define HWIO_DPC_REG_DPCC_DRE_GDSCR_INM(m)      \
        in_dword_masked(HWIO_DPC_REG_DPCC_DRE_GDSCR_ADDR, m)
#define HWIO_DPC_REG_DPCC_DRE_GDSCR_OUT(v)      \
        out_dword(HWIO_DPC_REG_DPCC_DRE_GDSCR_ADDR,v)
#define HWIO_DPC_REG_DPCC_DRE_GDSCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DPC_REG_DPCC_DRE_GDSCR_ADDR,m,v,HWIO_DPC_REG_DPCC_DRE_GDSCR_IN)
#define HWIO_DPC_REG_DPCC_DRE_GDSCR_PWR_ON_BMSK                                        0x80000000
#define HWIO_DPC_REG_DPCC_DRE_GDSCR_PWR_ON_SHFT                                              0x1f
#define HWIO_DPC_REG_DPCC_DRE_GDSCR_GDSC_STATE_BMSK                                    0x78000000
#define HWIO_DPC_REG_DPCC_DRE_GDSCR_GDSC_STATE_SHFT                                          0x1b
#define HWIO_DPC_REG_DPCC_DRE_GDSCR_EN_REST_WAIT_BMSK                                    0xf00000
#define HWIO_DPC_REG_DPCC_DRE_GDSCR_EN_REST_WAIT_SHFT                                        0x14
#define HWIO_DPC_REG_DPCC_DRE_GDSCR_EN_FEW_WAIT_BMSK                                      0xf0000
#define HWIO_DPC_REG_DPCC_DRE_GDSCR_EN_FEW_WAIT_SHFT                                         0x10
#define HWIO_DPC_REG_DPCC_DRE_GDSCR_CLK_DIS_WAIT_BMSK                                      0xf000
#define HWIO_DPC_REG_DPCC_DRE_GDSCR_CLK_DIS_WAIT_SHFT                                         0xc
#define HWIO_DPC_REG_DPCC_DRE_GDSCR_RETAIN_FF_ENABLE_BMSK                                   0x800
#define HWIO_DPC_REG_DPCC_DRE_GDSCR_RETAIN_FF_ENABLE_SHFT                                     0xb
#define HWIO_DPC_REG_DPCC_DRE_GDSCR_RETAIN_FF_ENABLE_DISABLE_FVAL                             0x0
#define HWIO_DPC_REG_DPCC_DRE_GDSCR_RETAIN_FF_ENABLE_ENABLE_FVAL                              0x1
#define HWIO_DPC_REG_DPCC_DRE_GDSCR_RESTORE_BMSK                                            0x400
#define HWIO_DPC_REG_DPCC_DRE_GDSCR_RESTORE_SHFT                                              0xa
#define HWIO_DPC_REG_DPCC_DRE_GDSCR_RESTORE_DISABLE_FVAL                                      0x0
#define HWIO_DPC_REG_DPCC_DRE_GDSCR_RESTORE_ENABLE_FVAL                                       0x1
#define HWIO_DPC_REG_DPCC_DRE_GDSCR_SAVE_BMSK                                               0x200
#define HWIO_DPC_REG_DPCC_DRE_GDSCR_SAVE_SHFT                                                 0x9
#define HWIO_DPC_REG_DPCC_DRE_GDSCR_SAVE_DISABLE_FVAL                                         0x0
#define HWIO_DPC_REG_DPCC_DRE_GDSCR_SAVE_ENABLE_FVAL                                          0x1
#define HWIO_DPC_REG_DPCC_DRE_GDSCR_RETAIN_BMSK                                             0x100
#define HWIO_DPC_REG_DPCC_DRE_GDSCR_RETAIN_SHFT                                               0x8
#define HWIO_DPC_REG_DPCC_DRE_GDSCR_RETAIN_DISABLE_FVAL                                       0x0
#define HWIO_DPC_REG_DPCC_DRE_GDSCR_RETAIN_ENABLE_FVAL                                        0x1
#define HWIO_DPC_REG_DPCC_DRE_GDSCR_EN_REST_BMSK                                             0x80
#define HWIO_DPC_REG_DPCC_DRE_GDSCR_EN_REST_SHFT                                              0x7
#define HWIO_DPC_REG_DPCC_DRE_GDSCR_EN_REST_DISABLE_FVAL                                      0x0
#define HWIO_DPC_REG_DPCC_DRE_GDSCR_EN_REST_ENABLE_FVAL                                       0x1
#define HWIO_DPC_REG_DPCC_DRE_GDSCR_EN_FEW_BMSK                                              0x40
#define HWIO_DPC_REG_DPCC_DRE_GDSCR_EN_FEW_SHFT                                               0x6
#define HWIO_DPC_REG_DPCC_DRE_GDSCR_EN_FEW_DISABLE_FVAL                                       0x0
#define HWIO_DPC_REG_DPCC_DRE_GDSCR_EN_FEW_ENABLE_FVAL                                        0x1
#define HWIO_DPC_REG_DPCC_DRE_GDSCR_CLAMP_IO_BMSK                                            0x20
#define HWIO_DPC_REG_DPCC_DRE_GDSCR_CLAMP_IO_SHFT                                             0x5
#define HWIO_DPC_REG_DPCC_DRE_GDSCR_CLAMP_IO_DISABLE_FVAL                                     0x0
#define HWIO_DPC_REG_DPCC_DRE_GDSCR_CLAMP_IO_ENABLE_FVAL                                      0x1
#define HWIO_DPC_REG_DPCC_DRE_GDSCR_CLK_DISABLE_BMSK                                         0x10
#define HWIO_DPC_REG_DPCC_DRE_GDSCR_CLK_DISABLE_SHFT                                          0x4
#define HWIO_DPC_REG_DPCC_DRE_GDSCR_CLK_DISABLE_CLK_NOT_DISABLE_FVAL                          0x0
#define HWIO_DPC_REG_DPCC_DRE_GDSCR_CLK_DISABLE_CLK_DISABLE_FVAL                              0x1
#define HWIO_DPC_REG_DPCC_DRE_GDSCR_PD_ARES_BMSK                                              0x8
#define HWIO_DPC_REG_DPCC_DRE_GDSCR_PD_ARES_SHFT                                              0x3
#define HWIO_DPC_REG_DPCC_DRE_GDSCR_PD_ARES_NO_RESET_FVAL                                     0x0
#define HWIO_DPC_REG_DPCC_DRE_GDSCR_PD_ARES_RESET_FVAL                                        0x1
#define HWIO_DPC_REG_DPCC_DRE_GDSCR_SW_OVERRIDE_BMSK                                          0x4
#define HWIO_DPC_REG_DPCC_DRE_GDSCR_SW_OVERRIDE_SHFT                                          0x2
#define HWIO_DPC_REG_DPCC_DRE_GDSCR_SW_OVERRIDE_DISABLE_FVAL                                  0x0
#define HWIO_DPC_REG_DPCC_DRE_GDSCR_SW_OVERRIDE_ENABLE_FVAL                                   0x1
#define HWIO_DPC_REG_DPCC_DRE_GDSCR_HW_CONTROL_BMSK                                           0x2
#define HWIO_DPC_REG_DPCC_DRE_GDSCR_HW_CONTROL_SHFT                                           0x1
#define HWIO_DPC_REG_DPCC_DRE_GDSCR_HW_CONTROL_DISABLE_FVAL                                   0x0
#define HWIO_DPC_REG_DPCC_DRE_GDSCR_HW_CONTROL_ENABLE_FVAL                                    0x1
#define HWIO_DPC_REG_DPCC_DRE_GDSCR_SW_COLLAPSE_BMSK                                          0x1
#define HWIO_DPC_REG_DPCC_DRE_GDSCR_SW_COLLAPSE_SHFT                                          0x0
#define HWIO_DPC_REG_DPCC_DRE_GDSCR_SW_COLLAPSE_DISABLE_FVAL                                  0x0
#define HWIO_DPC_REG_DPCC_DRE_GDSCR_SW_COLLAPSE_ENABLE_FVAL                                   0x1

#define HWIO_DPC_REG_DPCC_DRE_CFG_GDSCR_ADDR                                           (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE      + 0x00000090)
#define HWIO_DPC_REG_DPCC_DRE_CFG_GDSCR_PHYS                                           (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_PHYS + 0x00000090)
#define HWIO_DPC_REG_DPCC_DRE_CFG_GDSCR_OFFS                                           (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_OFFS + 0x00000090)
#define HWIO_DPC_REG_DPCC_DRE_CFG_GDSCR_RMSK                                           0xffffffff
#define HWIO_DPC_REG_DPCC_DRE_CFG_GDSCR_IN          \
        in_dword_masked(HWIO_DPC_REG_DPCC_DRE_CFG_GDSCR_ADDR, HWIO_DPC_REG_DPCC_DRE_CFG_GDSCR_RMSK)
#define HWIO_DPC_REG_DPCC_DRE_CFG_GDSCR_INM(m)      \
        in_dword_masked(HWIO_DPC_REG_DPCC_DRE_CFG_GDSCR_ADDR, m)
#define HWIO_DPC_REG_DPCC_DRE_CFG_GDSCR_OUT(v)      \
        out_dword(HWIO_DPC_REG_DPCC_DRE_CFG_GDSCR_ADDR,v)
#define HWIO_DPC_REG_DPCC_DRE_CFG_GDSCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DPC_REG_DPCC_DRE_CFG_GDSCR_ADDR,m,v,HWIO_DPC_REG_DPCC_DRE_CFG_GDSCR_IN)
#define HWIO_DPC_REG_DPCC_DRE_CFG_GDSCR_GDSC_SPARE_CTRL_IN_BMSK                        0xf0000000
#define HWIO_DPC_REG_DPCC_DRE_CFG_GDSCR_GDSC_SPARE_CTRL_IN_SHFT                              0x1c
#define HWIO_DPC_REG_DPCC_DRE_CFG_GDSCR_GDSC_SPARE_CTRL_OUT_BMSK                        0xc000000
#define HWIO_DPC_REG_DPCC_DRE_CFG_GDSCR_GDSC_SPARE_CTRL_OUT_SHFT                             0x1a
#define HWIO_DPC_REG_DPCC_DRE_CFG_GDSCR_GDSC_PWR_DWN_START_BMSK                         0x2000000
#define HWIO_DPC_REG_DPCC_DRE_CFG_GDSCR_GDSC_PWR_DWN_START_SHFT                              0x19
#define HWIO_DPC_REG_DPCC_DRE_CFG_GDSCR_GDSC_PWR_UP_START_BMSK                          0x1000000
#define HWIO_DPC_REG_DPCC_DRE_CFG_GDSCR_GDSC_PWR_UP_START_SHFT                               0x18
#define HWIO_DPC_REG_DPCC_DRE_CFG_GDSCR_GDSC_CFG_FSM_STATE_STATUS_BMSK                   0xf00000
#define HWIO_DPC_REG_DPCC_DRE_CFG_GDSCR_GDSC_CFG_FSM_STATE_STATUS_SHFT                       0x14
#define HWIO_DPC_REG_DPCC_DRE_CFG_GDSCR_GDSC_MEM_PWR_ACK_STATUS_BMSK                      0x80000
#define HWIO_DPC_REG_DPCC_DRE_CFG_GDSCR_GDSC_MEM_PWR_ACK_STATUS_SHFT                         0x13
#define HWIO_DPC_REG_DPCC_DRE_CFG_GDSCR_GDSC_ENR_ACK_STATUS_BMSK                          0x40000
#define HWIO_DPC_REG_DPCC_DRE_CFG_GDSCR_GDSC_ENR_ACK_STATUS_SHFT                             0x12
#define HWIO_DPC_REG_DPCC_DRE_CFG_GDSCR_GDSC_ENF_ACK_STATUS_BMSK                          0x20000
#define HWIO_DPC_REG_DPCC_DRE_CFG_GDSCR_GDSC_ENF_ACK_STATUS_SHFT                             0x11
#define HWIO_DPC_REG_DPCC_DRE_CFG_GDSCR_GDSC_POWER_UP_COMPLETE_BMSK                       0x10000
#define HWIO_DPC_REG_DPCC_DRE_CFG_GDSCR_GDSC_POWER_UP_COMPLETE_SHFT                          0x10
#define HWIO_DPC_REG_DPCC_DRE_CFG_GDSCR_GDSC_POWER_DOWN_COMPLETE_BMSK                      0x8000
#define HWIO_DPC_REG_DPCC_DRE_CFG_GDSCR_GDSC_POWER_DOWN_COMPLETE_SHFT                         0xf
#define HWIO_DPC_REG_DPCC_DRE_CFG_GDSCR_SOFTWARE_CONTROL_OVERRIDE_BMSK                     0x7800
#define HWIO_DPC_REG_DPCC_DRE_CFG_GDSCR_SOFTWARE_CONTROL_OVERRIDE_SHFT                        0xb
#define HWIO_DPC_REG_DPCC_DRE_CFG_GDSCR_GDSC_HANDSHAKE_DIS_BMSK                             0x400
#define HWIO_DPC_REG_DPCC_DRE_CFG_GDSCR_GDSC_HANDSHAKE_DIS_SHFT                               0xa
#define HWIO_DPC_REG_DPCC_DRE_CFG_GDSCR_GDSC_MEM_PERI_FORCE_IN_SW_BMSK                      0x200
#define HWIO_DPC_REG_DPCC_DRE_CFG_GDSCR_GDSC_MEM_PERI_FORCE_IN_SW_SHFT                        0x9
#define HWIO_DPC_REG_DPCC_DRE_CFG_GDSCR_GDSC_MEM_CORE_FORCE_IN_SW_BMSK                      0x100
#define HWIO_DPC_REG_DPCC_DRE_CFG_GDSCR_GDSC_MEM_CORE_FORCE_IN_SW_SHFT                        0x8
#define HWIO_DPC_REG_DPCC_DRE_CFG_GDSCR_GDSC_PHASE_RESET_EN_SW_BMSK                          0x80
#define HWIO_DPC_REG_DPCC_DRE_CFG_GDSCR_GDSC_PHASE_RESET_EN_SW_SHFT                           0x7
#define HWIO_DPC_REG_DPCC_DRE_CFG_GDSCR_GDSC_PHASE_RESET_DELAY_COUNT_SW_BMSK                 0x60
#define HWIO_DPC_REG_DPCC_DRE_CFG_GDSCR_GDSC_PHASE_RESET_DELAY_COUNT_SW_SHFT                  0x5
#define HWIO_DPC_REG_DPCC_DRE_CFG_GDSCR_GDSC_PSCBC_PWR_DWN_SW_BMSK                           0x10
#define HWIO_DPC_REG_DPCC_DRE_CFG_GDSCR_GDSC_PSCBC_PWR_DWN_SW_SHFT                            0x4
#define HWIO_DPC_REG_DPCC_DRE_CFG_GDSCR_UNCLAMP_IO_SOFTWARE_OVERRIDE_BMSK                     0x8
#define HWIO_DPC_REG_DPCC_DRE_CFG_GDSCR_UNCLAMP_IO_SOFTWARE_OVERRIDE_SHFT                     0x3
#define HWIO_DPC_REG_DPCC_DRE_CFG_GDSCR_SAVE_RESTORE_SOFTWARE_OVERRIDE_BMSK                   0x4
#define HWIO_DPC_REG_DPCC_DRE_CFG_GDSCR_SAVE_RESTORE_SOFTWARE_OVERRIDE_SHFT                   0x2
#define HWIO_DPC_REG_DPCC_DRE_CFG_GDSCR_CLAMP_IO_SOFTWARE_OVERRIDE_BMSK                       0x2
#define HWIO_DPC_REG_DPCC_DRE_CFG_GDSCR_CLAMP_IO_SOFTWARE_OVERRIDE_SHFT                       0x1
#define HWIO_DPC_REG_DPCC_DRE_CFG_GDSCR_DISABLE_CLK_SOFTWARE_OVERRIDE_BMSK                    0x1
#define HWIO_DPC_REG_DPCC_DRE_CFG_GDSCR_DISABLE_CLK_SOFTWARE_OVERRIDE_SHFT                    0x0

#define HWIO_DPC_REG_DPCC_DRE_CFG2_GDSCR_ADDR                                          (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE      + 0x00000094)
#define HWIO_DPC_REG_DPCC_DRE_CFG2_GDSCR_PHYS                                          (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_PHYS + 0x00000094)
#define HWIO_DPC_REG_DPCC_DRE_CFG2_GDSCR_OFFS                                          (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_OFFS + 0x00000094)
#define HWIO_DPC_REG_DPCC_DRE_CFG2_GDSCR_RMSK                                             0x1ffff
#define HWIO_DPC_REG_DPCC_DRE_CFG2_GDSCR_IN          \
        in_dword_masked(HWIO_DPC_REG_DPCC_DRE_CFG2_GDSCR_ADDR, HWIO_DPC_REG_DPCC_DRE_CFG2_GDSCR_RMSK)
#define HWIO_DPC_REG_DPCC_DRE_CFG2_GDSCR_INM(m)      \
        in_dword_masked(HWIO_DPC_REG_DPCC_DRE_CFG2_GDSCR_ADDR, m)
#define HWIO_DPC_REG_DPCC_DRE_CFG2_GDSCR_OUT(v)      \
        out_dword(HWIO_DPC_REG_DPCC_DRE_CFG2_GDSCR_ADDR,v)
#define HWIO_DPC_REG_DPCC_DRE_CFG2_GDSCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DPC_REG_DPCC_DRE_CFG2_GDSCR_ADDR,m,v,HWIO_DPC_REG_DPCC_DRE_CFG2_GDSCR_IN)
#define HWIO_DPC_REG_DPCC_DRE_CFG2_GDSCR_GDSC_CLAMP_MEM_SW_BMSK                           0x10000
#define HWIO_DPC_REG_DPCC_DRE_CFG2_GDSCR_GDSC_CLAMP_MEM_SW_SHFT                              0x10
#define HWIO_DPC_REG_DPCC_DRE_CFG2_GDSCR_DLY_MEM_PWR_UP_BMSK                               0xf000
#define HWIO_DPC_REG_DPCC_DRE_CFG2_GDSCR_DLY_MEM_PWR_UP_SHFT                                  0xc
#define HWIO_DPC_REG_DPCC_DRE_CFG2_GDSCR_DLY_DEASSERT_CLAMP_MEM_BMSK                        0xf00
#define HWIO_DPC_REG_DPCC_DRE_CFG2_GDSCR_DLY_DEASSERT_CLAMP_MEM_SHFT                          0x8
#define HWIO_DPC_REG_DPCC_DRE_CFG2_GDSCR_DLY_ASSERT_CLAMP_MEM_BMSK                           0xf0
#define HWIO_DPC_REG_DPCC_DRE_CFG2_GDSCR_DLY_ASSERT_CLAMP_MEM_SHFT                            0x4
#define HWIO_DPC_REG_DPCC_DRE_CFG2_GDSCR_MEM_PWR_DWN_TIMEOUT_BMSK                             0xf
#define HWIO_DPC_REG_DPCC_DRE_CFG2_GDSCR_MEM_PWR_DWN_TIMEOUT_SHFT                             0x0

#define HWIO_DPC_REG_DPCC_DRE_CBCR_ADDR                                                (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE      + 0x00000098)
#define HWIO_DPC_REG_DPCC_DRE_CBCR_PHYS                                                (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_PHYS + 0x00000098)
#define HWIO_DPC_REG_DPCC_DRE_CBCR_OFFS                                                (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_OFFS + 0x00000098)
#define HWIO_DPC_REG_DPCC_DRE_CBCR_RMSK                                                0x80000005
#define HWIO_DPC_REG_DPCC_DRE_CBCR_IN          \
        in_dword_masked(HWIO_DPC_REG_DPCC_DRE_CBCR_ADDR, HWIO_DPC_REG_DPCC_DRE_CBCR_RMSK)
#define HWIO_DPC_REG_DPCC_DRE_CBCR_INM(m)      \
        in_dword_masked(HWIO_DPC_REG_DPCC_DRE_CBCR_ADDR, m)
#define HWIO_DPC_REG_DPCC_DRE_CBCR_OUT(v)      \
        out_dword(HWIO_DPC_REG_DPCC_DRE_CBCR_ADDR,v)
#define HWIO_DPC_REG_DPCC_DRE_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DPC_REG_DPCC_DRE_CBCR_ADDR,m,v,HWIO_DPC_REG_DPCC_DRE_CBCR_IN)
#define HWIO_DPC_REG_DPCC_DRE_CBCR_CLK_OFF_BMSK                                        0x80000000
#define HWIO_DPC_REG_DPCC_DRE_CBCR_CLK_OFF_SHFT                                              0x1f
#define HWIO_DPC_REG_DPCC_DRE_CBCR_CLK_ARES_BMSK                                              0x4
#define HWIO_DPC_REG_DPCC_DRE_CBCR_CLK_ARES_SHFT                                              0x2
#define HWIO_DPC_REG_DPCC_DRE_CBCR_CLK_ARES_NO_RESET_FVAL                                     0x0
#define HWIO_DPC_REG_DPCC_DRE_CBCR_CLK_ARES_RESET_FVAL                                        0x1
#define HWIO_DPC_REG_DPCC_DRE_CBCR_CLK_ENABLE_BMSK                                            0x1
#define HWIO_DPC_REG_DPCC_DRE_CBCR_CLK_ENABLE_SHFT                                            0x0
#define HWIO_DPC_REG_DPCC_DRE_CBCR_CLK_ENABLE_DISABLE_FVAL                                    0x0
#define HWIO_DPC_REG_DPCC_DRE_CBCR_CLK_ENABLE_ENABLE_FVAL                                     0x1

#define HWIO_DPC_REG_DPCC_DRE_CFG_CBCR_ADDR                                            (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE      + 0x0000009c)
#define HWIO_DPC_REG_DPCC_DRE_CFG_CBCR_PHYS                                            (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_PHYS + 0x0000009c)
#define HWIO_DPC_REG_DPCC_DRE_CFG_CBCR_OFFS                                            (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_OFFS + 0x0000009c)
#define HWIO_DPC_REG_DPCC_DRE_CFG_CBCR_RMSK                                            0x80000005
#define HWIO_DPC_REG_DPCC_DRE_CFG_CBCR_IN          \
        in_dword_masked(HWIO_DPC_REG_DPCC_DRE_CFG_CBCR_ADDR, HWIO_DPC_REG_DPCC_DRE_CFG_CBCR_RMSK)
#define HWIO_DPC_REG_DPCC_DRE_CFG_CBCR_INM(m)      \
        in_dword_masked(HWIO_DPC_REG_DPCC_DRE_CFG_CBCR_ADDR, m)
#define HWIO_DPC_REG_DPCC_DRE_CFG_CBCR_OUT(v)      \
        out_dword(HWIO_DPC_REG_DPCC_DRE_CFG_CBCR_ADDR,v)
#define HWIO_DPC_REG_DPCC_DRE_CFG_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DPC_REG_DPCC_DRE_CFG_CBCR_ADDR,m,v,HWIO_DPC_REG_DPCC_DRE_CFG_CBCR_IN)
#define HWIO_DPC_REG_DPCC_DRE_CFG_CBCR_CLK_OFF_BMSK                                    0x80000000
#define HWIO_DPC_REG_DPCC_DRE_CFG_CBCR_CLK_OFF_SHFT                                          0x1f
#define HWIO_DPC_REG_DPCC_DRE_CFG_CBCR_CLK_ARES_BMSK                                          0x4
#define HWIO_DPC_REG_DPCC_DRE_CFG_CBCR_CLK_ARES_SHFT                                          0x2
#define HWIO_DPC_REG_DPCC_DRE_CFG_CBCR_CLK_ARES_NO_RESET_FVAL                                 0x0
#define HWIO_DPC_REG_DPCC_DRE_CFG_CBCR_CLK_ARES_RESET_FVAL                                    0x1
#define HWIO_DPC_REG_DPCC_DRE_CFG_CBCR_CLK_ENABLE_BMSK                                        0x1
#define HWIO_DPC_REG_DPCC_DRE_CFG_CBCR_CLK_ENABLE_SHFT                                        0x0
#define HWIO_DPC_REG_DPCC_DRE_CFG_CBCR_CLK_ENABLE_DISABLE_FVAL                                0x0
#define HWIO_DPC_REG_DPCC_DRE_CFG_CBCR_CLK_ENABLE_ENABLE_FVAL                                 0x1

#define HWIO_DPC_REG_DPCC_DRE_ATB_CBCR_ADDR                                            (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE      + 0x00000128)
#define HWIO_DPC_REG_DPCC_DRE_ATB_CBCR_PHYS                                            (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_PHYS + 0x00000128)
#define HWIO_DPC_REG_DPCC_DRE_ATB_CBCR_OFFS                                            (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_OFFS + 0x00000128)
#define HWIO_DPC_REG_DPCC_DRE_ATB_CBCR_RMSK                                            0x80000005
#define HWIO_DPC_REG_DPCC_DRE_ATB_CBCR_IN          \
        in_dword_masked(HWIO_DPC_REG_DPCC_DRE_ATB_CBCR_ADDR, HWIO_DPC_REG_DPCC_DRE_ATB_CBCR_RMSK)
#define HWIO_DPC_REG_DPCC_DRE_ATB_CBCR_INM(m)      \
        in_dword_masked(HWIO_DPC_REG_DPCC_DRE_ATB_CBCR_ADDR, m)
#define HWIO_DPC_REG_DPCC_DRE_ATB_CBCR_OUT(v)      \
        out_dword(HWIO_DPC_REG_DPCC_DRE_ATB_CBCR_ADDR,v)
#define HWIO_DPC_REG_DPCC_DRE_ATB_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DPC_REG_DPCC_DRE_ATB_CBCR_ADDR,m,v,HWIO_DPC_REG_DPCC_DRE_ATB_CBCR_IN)
#define HWIO_DPC_REG_DPCC_DRE_ATB_CBCR_CLK_OFF_BMSK                                    0x80000000
#define HWIO_DPC_REG_DPCC_DRE_ATB_CBCR_CLK_OFF_SHFT                                          0x1f
#define HWIO_DPC_REG_DPCC_DRE_ATB_CBCR_CLK_ARES_BMSK                                          0x4
#define HWIO_DPC_REG_DPCC_DRE_ATB_CBCR_CLK_ARES_SHFT                                          0x2
#define HWIO_DPC_REG_DPCC_DRE_ATB_CBCR_CLK_ARES_NO_RESET_FVAL                                 0x0
#define HWIO_DPC_REG_DPCC_DRE_ATB_CBCR_CLK_ARES_RESET_FVAL                                    0x1
#define HWIO_DPC_REG_DPCC_DRE_ATB_CBCR_CLK_ENABLE_BMSK                                        0x1
#define HWIO_DPC_REG_DPCC_DRE_ATB_CBCR_CLK_ENABLE_SHFT                                        0x0
#define HWIO_DPC_REG_DPCC_DRE_ATB_CBCR_CLK_ENABLE_DISABLE_FVAL                                0x0
#define HWIO_DPC_REG_DPCC_DRE_ATB_CBCR_CLK_ENABLE_ENABLE_FVAL                                 0x1

#define HWIO_DPC_REG_DPCC_MC_BCR_ADDR                                                  (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE      + 0x000000a0)
#define HWIO_DPC_REG_DPCC_MC_BCR_PHYS                                                  (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_PHYS + 0x000000a0)
#define HWIO_DPC_REG_DPCC_MC_BCR_OFFS                                                  (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_OFFS + 0x000000a0)
#define HWIO_DPC_REG_DPCC_MC_BCR_RMSK                                                         0x1
#define HWIO_DPC_REG_DPCC_MC_BCR_IN          \
        in_dword_masked(HWIO_DPC_REG_DPCC_MC_BCR_ADDR, HWIO_DPC_REG_DPCC_MC_BCR_RMSK)
#define HWIO_DPC_REG_DPCC_MC_BCR_INM(m)      \
        in_dword_masked(HWIO_DPC_REG_DPCC_MC_BCR_ADDR, m)
#define HWIO_DPC_REG_DPCC_MC_BCR_OUT(v)      \
        out_dword(HWIO_DPC_REG_DPCC_MC_BCR_ADDR,v)
#define HWIO_DPC_REG_DPCC_MC_BCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DPC_REG_DPCC_MC_BCR_ADDR,m,v,HWIO_DPC_REG_DPCC_MC_BCR_IN)
#define HWIO_DPC_REG_DPCC_MC_BCR_BLK_ARES_BMSK                                                0x1
#define HWIO_DPC_REG_DPCC_MC_BCR_BLK_ARES_SHFT                                                0x0
#define HWIO_DPC_REG_DPCC_MC_BCR_BLK_ARES_DISABLE_FVAL                                        0x0
#define HWIO_DPC_REG_DPCC_MC_BCR_BLK_ARES_ENABLE_FVAL                                         0x1

#define HWIO_DPC_REG_DPCC_MC_GDSCR_ADDR                                                (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE      + 0x000000a4)
#define HWIO_DPC_REG_DPCC_MC_GDSCR_PHYS                                                (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_PHYS + 0x000000a4)
#define HWIO_DPC_REG_DPCC_MC_GDSCR_OFFS                                                (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_OFFS + 0x000000a4)
#define HWIO_DPC_REG_DPCC_MC_GDSCR_RMSK                                                0xf8ffffff
#define HWIO_DPC_REG_DPCC_MC_GDSCR_IN          \
        in_dword_masked(HWIO_DPC_REG_DPCC_MC_GDSCR_ADDR, HWIO_DPC_REG_DPCC_MC_GDSCR_RMSK)
#define HWIO_DPC_REG_DPCC_MC_GDSCR_INM(m)      \
        in_dword_masked(HWIO_DPC_REG_DPCC_MC_GDSCR_ADDR, m)
#define HWIO_DPC_REG_DPCC_MC_GDSCR_OUT(v)      \
        out_dword(HWIO_DPC_REG_DPCC_MC_GDSCR_ADDR,v)
#define HWIO_DPC_REG_DPCC_MC_GDSCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DPC_REG_DPCC_MC_GDSCR_ADDR,m,v,HWIO_DPC_REG_DPCC_MC_GDSCR_IN)
#define HWIO_DPC_REG_DPCC_MC_GDSCR_PWR_ON_BMSK                                         0x80000000
#define HWIO_DPC_REG_DPCC_MC_GDSCR_PWR_ON_SHFT                                               0x1f
#define HWIO_DPC_REG_DPCC_MC_GDSCR_GDSC_STATE_BMSK                                     0x78000000
#define HWIO_DPC_REG_DPCC_MC_GDSCR_GDSC_STATE_SHFT                                           0x1b
#define HWIO_DPC_REG_DPCC_MC_GDSCR_EN_REST_WAIT_BMSK                                     0xf00000
#define HWIO_DPC_REG_DPCC_MC_GDSCR_EN_REST_WAIT_SHFT                                         0x14
#define HWIO_DPC_REG_DPCC_MC_GDSCR_EN_FEW_WAIT_BMSK                                       0xf0000
#define HWIO_DPC_REG_DPCC_MC_GDSCR_EN_FEW_WAIT_SHFT                                          0x10
#define HWIO_DPC_REG_DPCC_MC_GDSCR_CLK_DIS_WAIT_BMSK                                       0xf000
#define HWIO_DPC_REG_DPCC_MC_GDSCR_CLK_DIS_WAIT_SHFT                                          0xc
#define HWIO_DPC_REG_DPCC_MC_GDSCR_RETAIN_FF_ENABLE_BMSK                                    0x800
#define HWIO_DPC_REG_DPCC_MC_GDSCR_RETAIN_FF_ENABLE_SHFT                                      0xb
#define HWIO_DPC_REG_DPCC_MC_GDSCR_RETAIN_FF_ENABLE_DISABLE_FVAL                              0x0
#define HWIO_DPC_REG_DPCC_MC_GDSCR_RETAIN_FF_ENABLE_ENABLE_FVAL                               0x1
#define HWIO_DPC_REG_DPCC_MC_GDSCR_RESTORE_BMSK                                             0x400
#define HWIO_DPC_REG_DPCC_MC_GDSCR_RESTORE_SHFT                                               0xa
#define HWIO_DPC_REG_DPCC_MC_GDSCR_RESTORE_DISABLE_FVAL                                       0x0
#define HWIO_DPC_REG_DPCC_MC_GDSCR_RESTORE_ENABLE_FVAL                                        0x1
#define HWIO_DPC_REG_DPCC_MC_GDSCR_SAVE_BMSK                                                0x200
#define HWIO_DPC_REG_DPCC_MC_GDSCR_SAVE_SHFT                                                  0x9
#define HWIO_DPC_REG_DPCC_MC_GDSCR_SAVE_DISABLE_FVAL                                          0x0
#define HWIO_DPC_REG_DPCC_MC_GDSCR_SAVE_ENABLE_FVAL                                           0x1
#define HWIO_DPC_REG_DPCC_MC_GDSCR_RETAIN_BMSK                                              0x100
#define HWIO_DPC_REG_DPCC_MC_GDSCR_RETAIN_SHFT                                                0x8
#define HWIO_DPC_REG_DPCC_MC_GDSCR_RETAIN_DISABLE_FVAL                                        0x0
#define HWIO_DPC_REG_DPCC_MC_GDSCR_RETAIN_ENABLE_FVAL                                         0x1
#define HWIO_DPC_REG_DPCC_MC_GDSCR_EN_REST_BMSK                                              0x80
#define HWIO_DPC_REG_DPCC_MC_GDSCR_EN_REST_SHFT                                               0x7
#define HWIO_DPC_REG_DPCC_MC_GDSCR_EN_REST_DISABLE_FVAL                                       0x0
#define HWIO_DPC_REG_DPCC_MC_GDSCR_EN_REST_ENABLE_FVAL                                        0x1
#define HWIO_DPC_REG_DPCC_MC_GDSCR_EN_FEW_BMSK                                               0x40
#define HWIO_DPC_REG_DPCC_MC_GDSCR_EN_FEW_SHFT                                                0x6
#define HWIO_DPC_REG_DPCC_MC_GDSCR_EN_FEW_DISABLE_FVAL                                        0x0
#define HWIO_DPC_REG_DPCC_MC_GDSCR_EN_FEW_ENABLE_FVAL                                         0x1
#define HWIO_DPC_REG_DPCC_MC_GDSCR_CLAMP_IO_BMSK                                             0x20
#define HWIO_DPC_REG_DPCC_MC_GDSCR_CLAMP_IO_SHFT                                              0x5
#define HWIO_DPC_REG_DPCC_MC_GDSCR_CLAMP_IO_DISABLE_FVAL                                      0x0
#define HWIO_DPC_REG_DPCC_MC_GDSCR_CLAMP_IO_ENABLE_FVAL                                       0x1
#define HWIO_DPC_REG_DPCC_MC_GDSCR_CLK_DISABLE_BMSK                                          0x10
#define HWIO_DPC_REG_DPCC_MC_GDSCR_CLK_DISABLE_SHFT                                           0x4
#define HWIO_DPC_REG_DPCC_MC_GDSCR_CLK_DISABLE_CLK_NOT_DISABLE_FVAL                           0x0
#define HWIO_DPC_REG_DPCC_MC_GDSCR_CLK_DISABLE_CLK_DISABLE_FVAL                               0x1
#define HWIO_DPC_REG_DPCC_MC_GDSCR_PD_ARES_BMSK                                               0x8
#define HWIO_DPC_REG_DPCC_MC_GDSCR_PD_ARES_SHFT                                               0x3
#define HWIO_DPC_REG_DPCC_MC_GDSCR_PD_ARES_NO_RESET_FVAL                                      0x0
#define HWIO_DPC_REG_DPCC_MC_GDSCR_PD_ARES_RESET_FVAL                                         0x1
#define HWIO_DPC_REG_DPCC_MC_GDSCR_SW_OVERRIDE_BMSK                                           0x4
#define HWIO_DPC_REG_DPCC_MC_GDSCR_SW_OVERRIDE_SHFT                                           0x2
#define HWIO_DPC_REG_DPCC_MC_GDSCR_SW_OVERRIDE_DISABLE_FVAL                                   0x0
#define HWIO_DPC_REG_DPCC_MC_GDSCR_SW_OVERRIDE_ENABLE_FVAL                                    0x1
#define HWIO_DPC_REG_DPCC_MC_GDSCR_HW_CONTROL_BMSK                                            0x2
#define HWIO_DPC_REG_DPCC_MC_GDSCR_HW_CONTROL_SHFT                                            0x1
#define HWIO_DPC_REG_DPCC_MC_GDSCR_HW_CONTROL_DISABLE_FVAL                                    0x0
#define HWIO_DPC_REG_DPCC_MC_GDSCR_HW_CONTROL_ENABLE_FVAL                                     0x1
#define HWIO_DPC_REG_DPCC_MC_GDSCR_SW_COLLAPSE_BMSK                                           0x1
#define HWIO_DPC_REG_DPCC_MC_GDSCR_SW_COLLAPSE_SHFT                                           0x0
#define HWIO_DPC_REG_DPCC_MC_GDSCR_SW_COLLAPSE_DISABLE_FVAL                                   0x0
#define HWIO_DPC_REG_DPCC_MC_GDSCR_SW_COLLAPSE_ENABLE_FVAL                                    0x1

#define HWIO_DPC_REG_DPCC_MC_CFG_GDSCR_ADDR                                            (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE      + 0x000000a8)
#define HWIO_DPC_REG_DPCC_MC_CFG_GDSCR_PHYS                                            (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_PHYS + 0x000000a8)
#define HWIO_DPC_REG_DPCC_MC_CFG_GDSCR_OFFS                                            (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_OFFS + 0x000000a8)
#define HWIO_DPC_REG_DPCC_MC_CFG_GDSCR_RMSK                                            0xffffffff
#define HWIO_DPC_REG_DPCC_MC_CFG_GDSCR_IN          \
        in_dword_masked(HWIO_DPC_REG_DPCC_MC_CFG_GDSCR_ADDR, HWIO_DPC_REG_DPCC_MC_CFG_GDSCR_RMSK)
#define HWIO_DPC_REG_DPCC_MC_CFG_GDSCR_INM(m)      \
        in_dword_masked(HWIO_DPC_REG_DPCC_MC_CFG_GDSCR_ADDR, m)
#define HWIO_DPC_REG_DPCC_MC_CFG_GDSCR_OUT(v)      \
        out_dword(HWIO_DPC_REG_DPCC_MC_CFG_GDSCR_ADDR,v)
#define HWIO_DPC_REG_DPCC_MC_CFG_GDSCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DPC_REG_DPCC_MC_CFG_GDSCR_ADDR,m,v,HWIO_DPC_REG_DPCC_MC_CFG_GDSCR_IN)
#define HWIO_DPC_REG_DPCC_MC_CFG_GDSCR_GDSC_SPARE_CTRL_IN_BMSK                         0xf0000000
#define HWIO_DPC_REG_DPCC_MC_CFG_GDSCR_GDSC_SPARE_CTRL_IN_SHFT                               0x1c
#define HWIO_DPC_REG_DPCC_MC_CFG_GDSCR_GDSC_SPARE_CTRL_OUT_BMSK                         0xc000000
#define HWIO_DPC_REG_DPCC_MC_CFG_GDSCR_GDSC_SPARE_CTRL_OUT_SHFT                              0x1a
#define HWIO_DPC_REG_DPCC_MC_CFG_GDSCR_GDSC_PWR_DWN_START_BMSK                          0x2000000
#define HWIO_DPC_REG_DPCC_MC_CFG_GDSCR_GDSC_PWR_DWN_START_SHFT                               0x19
#define HWIO_DPC_REG_DPCC_MC_CFG_GDSCR_GDSC_PWR_UP_START_BMSK                           0x1000000
#define HWIO_DPC_REG_DPCC_MC_CFG_GDSCR_GDSC_PWR_UP_START_SHFT                                0x18
#define HWIO_DPC_REG_DPCC_MC_CFG_GDSCR_GDSC_CFG_FSM_STATE_STATUS_BMSK                    0xf00000
#define HWIO_DPC_REG_DPCC_MC_CFG_GDSCR_GDSC_CFG_FSM_STATE_STATUS_SHFT                        0x14
#define HWIO_DPC_REG_DPCC_MC_CFG_GDSCR_GDSC_MEM_PWR_ACK_STATUS_BMSK                       0x80000
#define HWIO_DPC_REG_DPCC_MC_CFG_GDSCR_GDSC_MEM_PWR_ACK_STATUS_SHFT                          0x13
#define HWIO_DPC_REG_DPCC_MC_CFG_GDSCR_GDSC_ENR_ACK_STATUS_BMSK                           0x40000
#define HWIO_DPC_REG_DPCC_MC_CFG_GDSCR_GDSC_ENR_ACK_STATUS_SHFT                              0x12
#define HWIO_DPC_REG_DPCC_MC_CFG_GDSCR_GDSC_ENF_ACK_STATUS_BMSK                           0x20000
#define HWIO_DPC_REG_DPCC_MC_CFG_GDSCR_GDSC_ENF_ACK_STATUS_SHFT                              0x11
#define HWIO_DPC_REG_DPCC_MC_CFG_GDSCR_GDSC_POWER_UP_COMPLETE_BMSK                        0x10000
#define HWIO_DPC_REG_DPCC_MC_CFG_GDSCR_GDSC_POWER_UP_COMPLETE_SHFT                           0x10
#define HWIO_DPC_REG_DPCC_MC_CFG_GDSCR_GDSC_POWER_DOWN_COMPLETE_BMSK                       0x8000
#define HWIO_DPC_REG_DPCC_MC_CFG_GDSCR_GDSC_POWER_DOWN_COMPLETE_SHFT                          0xf
#define HWIO_DPC_REG_DPCC_MC_CFG_GDSCR_SOFTWARE_CONTROL_OVERRIDE_BMSK                      0x7800
#define HWIO_DPC_REG_DPCC_MC_CFG_GDSCR_SOFTWARE_CONTROL_OVERRIDE_SHFT                         0xb
#define HWIO_DPC_REG_DPCC_MC_CFG_GDSCR_GDSC_HANDSHAKE_DIS_BMSK                              0x400
#define HWIO_DPC_REG_DPCC_MC_CFG_GDSCR_GDSC_HANDSHAKE_DIS_SHFT                                0xa
#define HWIO_DPC_REG_DPCC_MC_CFG_GDSCR_GDSC_MEM_PERI_FORCE_IN_SW_BMSK                       0x200
#define HWIO_DPC_REG_DPCC_MC_CFG_GDSCR_GDSC_MEM_PERI_FORCE_IN_SW_SHFT                         0x9
#define HWIO_DPC_REG_DPCC_MC_CFG_GDSCR_GDSC_MEM_CORE_FORCE_IN_SW_BMSK                       0x100
#define HWIO_DPC_REG_DPCC_MC_CFG_GDSCR_GDSC_MEM_CORE_FORCE_IN_SW_SHFT                         0x8
#define HWIO_DPC_REG_DPCC_MC_CFG_GDSCR_GDSC_PHASE_RESET_EN_SW_BMSK                           0x80
#define HWIO_DPC_REG_DPCC_MC_CFG_GDSCR_GDSC_PHASE_RESET_EN_SW_SHFT                            0x7
#define HWIO_DPC_REG_DPCC_MC_CFG_GDSCR_GDSC_PHASE_RESET_DELAY_COUNT_SW_BMSK                  0x60
#define HWIO_DPC_REG_DPCC_MC_CFG_GDSCR_GDSC_PHASE_RESET_DELAY_COUNT_SW_SHFT                   0x5
#define HWIO_DPC_REG_DPCC_MC_CFG_GDSCR_GDSC_PSCBC_PWR_DWN_SW_BMSK                            0x10
#define HWIO_DPC_REG_DPCC_MC_CFG_GDSCR_GDSC_PSCBC_PWR_DWN_SW_SHFT                             0x4
#define HWIO_DPC_REG_DPCC_MC_CFG_GDSCR_UNCLAMP_IO_SOFTWARE_OVERRIDE_BMSK                      0x8
#define HWIO_DPC_REG_DPCC_MC_CFG_GDSCR_UNCLAMP_IO_SOFTWARE_OVERRIDE_SHFT                      0x3
#define HWIO_DPC_REG_DPCC_MC_CFG_GDSCR_SAVE_RESTORE_SOFTWARE_OVERRIDE_BMSK                    0x4
#define HWIO_DPC_REG_DPCC_MC_CFG_GDSCR_SAVE_RESTORE_SOFTWARE_OVERRIDE_SHFT                    0x2
#define HWIO_DPC_REG_DPCC_MC_CFG_GDSCR_CLAMP_IO_SOFTWARE_OVERRIDE_BMSK                        0x2
#define HWIO_DPC_REG_DPCC_MC_CFG_GDSCR_CLAMP_IO_SOFTWARE_OVERRIDE_SHFT                        0x1
#define HWIO_DPC_REG_DPCC_MC_CFG_GDSCR_DISABLE_CLK_SOFTWARE_OVERRIDE_BMSK                     0x1
#define HWIO_DPC_REG_DPCC_MC_CFG_GDSCR_DISABLE_CLK_SOFTWARE_OVERRIDE_SHFT                     0x0

#define HWIO_DPC_REG_DPCC_MC_CFG2_GDSCR_ADDR                                           (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE      + 0x000000ac)
#define HWIO_DPC_REG_DPCC_MC_CFG2_GDSCR_PHYS                                           (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_PHYS + 0x000000ac)
#define HWIO_DPC_REG_DPCC_MC_CFG2_GDSCR_OFFS                                           (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_OFFS + 0x000000ac)
#define HWIO_DPC_REG_DPCC_MC_CFG2_GDSCR_RMSK                                              0x1ffff
#define HWIO_DPC_REG_DPCC_MC_CFG2_GDSCR_IN          \
        in_dword_masked(HWIO_DPC_REG_DPCC_MC_CFG2_GDSCR_ADDR, HWIO_DPC_REG_DPCC_MC_CFG2_GDSCR_RMSK)
#define HWIO_DPC_REG_DPCC_MC_CFG2_GDSCR_INM(m)      \
        in_dword_masked(HWIO_DPC_REG_DPCC_MC_CFG2_GDSCR_ADDR, m)
#define HWIO_DPC_REG_DPCC_MC_CFG2_GDSCR_OUT(v)      \
        out_dword(HWIO_DPC_REG_DPCC_MC_CFG2_GDSCR_ADDR,v)
#define HWIO_DPC_REG_DPCC_MC_CFG2_GDSCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DPC_REG_DPCC_MC_CFG2_GDSCR_ADDR,m,v,HWIO_DPC_REG_DPCC_MC_CFG2_GDSCR_IN)
#define HWIO_DPC_REG_DPCC_MC_CFG2_GDSCR_GDSC_CLAMP_MEM_SW_BMSK                            0x10000
#define HWIO_DPC_REG_DPCC_MC_CFG2_GDSCR_GDSC_CLAMP_MEM_SW_SHFT                               0x10
#define HWIO_DPC_REG_DPCC_MC_CFG2_GDSCR_DLY_MEM_PWR_UP_BMSK                                0xf000
#define HWIO_DPC_REG_DPCC_MC_CFG2_GDSCR_DLY_MEM_PWR_UP_SHFT                                   0xc
#define HWIO_DPC_REG_DPCC_MC_CFG2_GDSCR_DLY_DEASSERT_CLAMP_MEM_BMSK                         0xf00
#define HWIO_DPC_REG_DPCC_MC_CFG2_GDSCR_DLY_DEASSERT_CLAMP_MEM_SHFT                           0x8
#define HWIO_DPC_REG_DPCC_MC_CFG2_GDSCR_DLY_ASSERT_CLAMP_MEM_BMSK                            0xf0
#define HWIO_DPC_REG_DPCC_MC_CFG2_GDSCR_DLY_ASSERT_CLAMP_MEM_SHFT                             0x4
#define HWIO_DPC_REG_DPCC_MC_CFG2_GDSCR_MEM_PWR_DWN_TIMEOUT_BMSK                              0xf
#define HWIO_DPC_REG_DPCC_MC_CFG2_GDSCR_MEM_PWR_DWN_TIMEOUT_SHFT                              0x0

#define HWIO_DPC_REG_DPCC_MC0_XO_CBCR_ADDR                                             (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE      + 0x000000b0)
#define HWIO_DPC_REG_DPCC_MC0_XO_CBCR_PHYS                                             (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_PHYS + 0x000000b0)
#define HWIO_DPC_REG_DPCC_MC0_XO_CBCR_OFFS                                             (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_OFFS + 0x000000b0)
#define HWIO_DPC_REG_DPCC_MC0_XO_CBCR_RMSK                                             0x80000005
#define HWIO_DPC_REG_DPCC_MC0_XO_CBCR_IN          \
        in_dword_masked(HWIO_DPC_REG_DPCC_MC0_XO_CBCR_ADDR, HWIO_DPC_REG_DPCC_MC0_XO_CBCR_RMSK)
#define HWIO_DPC_REG_DPCC_MC0_XO_CBCR_INM(m)      \
        in_dword_masked(HWIO_DPC_REG_DPCC_MC0_XO_CBCR_ADDR, m)
#define HWIO_DPC_REG_DPCC_MC0_XO_CBCR_OUT(v)      \
        out_dword(HWIO_DPC_REG_DPCC_MC0_XO_CBCR_ADDR,v)
#define HWIO_DPC_REG_DPCC_MC0_XO_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DPC_REG_DPCC_MC0_XO_CBCR_ADDR,m,v,HWIO_DPC_REG_DPCC_MC0_XO_CBCR_IN)
#define HWIO_DPC_REG_DPCC_MC0_XO_CBCR_CLK_OFF_BMSK                                     0x80000000
#define HWIO_DPC_REG_DPCC_MC0_XO_CBCR_CLK_OFF_SHFT                                           0x1f
#define HWIO_DPC_REG_DPCC_MC0_XO_CBCR_CLK_ARES_BMSK                                           0x4
#define HWIO_DPC_REG_DPCC_MC0_XO_CBCR_CLK_ARES_SHFT                                           0x2
#define HWIO_DPC_REG_DPCC_MC0_XO_CBCR_CLK_ARES_NO_RESET_FVAL                                  0x0
#define HWIO_DPC_REG_DPCC_MC0_XO_CBCR_CLK_ARES_RESET_FVAL                                     0x1
#define HWIO_DPC_REG_DPCC_MC0_XO_CBCR_CLK_ENABLE_BMSK                                         0x1
#define HWIO_DPC_REG_DPCC_MC0_XO_CBCR_CLK_ENABLE_SHFT                                         0x0
#define HWIO_DPC_REG_DPCC_MC0_XO_CBCR_CLK_ENABLE_DISABLE_FVAL                                 0x0
#define HWIO_DPC_REG_DPCC_MC0_XO_CBCR_CLK_ENABLE_ENABLE_FVAL                                  0x1

#define HWIO_DPC_REG_DPCC_MC1_XO_CBCR_ADDR                                             (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE      + 0x000000b4)
#define HWIO_DPC_REG_DPCC_MC1_XO_CBCR_PHYS                                             (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_PHYS + 0x000000b4)
#define HWIO_DPC_REG_DPCC_MC1_XO_CBCR_OFFS                                             (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_OFFS + 0x000000b4)
#define HWIO_DPC_REG_DPCC_MC1_XO_CBCR_RMSK                                             0x80000005
#define HWIO_DPC_REG_DPCC_MC1_XO_CBCR_IN          \
        in_dword_masked(HWIO_DPC_REG_DPCC_MC1_XO_CBCR_ADDR, HWIO_DPC_REG_DPCC_MC1_XO_CBCR_RMSK)
#define HWIO_DPC_REG_DPCC_MC1_XO_CBCR_INM(m)      \
        in_dword_masked(HWIO_DPC_REG_DPCC_MC1_XO_CBCR_ADDR, m)
#define HWIO_DPC_REG_DPCC_MC1_XO_CBCR_OUT(v)      \
        out_dword(HWIO_DPC_REG_DPCC_MC1_XO_CBCR_ADDR,v)
#define HWIO_DPC_REG_DPCC_MC1_XO_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DPC_REG_DPCC_MC1_XO_CBCR_ADDR,m,v,HWIO_DPC_REG_DPCC_MC1_XO_CBCR_IN)
#define HWIO_DPC_REG_DPCC_MC1_XO_CBCR_CLK_OFF_BMSK                                     0x80000000
#define HWIO_DPC_REG_DPCC_MC1_XO_CBCR_CLK_OFF_SHFT                                           0x1f
#define HWIO_DPC_REG_DPCC_MC1_XO_CBCR_CLK_ARES_BMSK                                           0x4
#define HWIO_DPC_REG_DPCC_MC1_XO_CBCR_CLK_ARES_SHFT                                           0x2
#define HWIO_DPC_REG_DPCC_MC1_XO_CBCR_CLK_ARES_NO_RESET_FVAL                                  0x0
#define HWIO_DPC_REG_DPCC_MC1_XO_CBCR_CLK_ARES_RESET_FVAL                                     0x1
#define HWIO_DPC_REG_DPCC_MC1_XO_CBCR_CLK_ENABLE_BMSK                                         0x1
#define HWIO_DPC_REG_DPCC_MC1_XO_CBCR_CLK_ENABLE_SHFT                                         0x0
#define HWIO_DPC_REG_DPCC_MC1_XO_CBCR_CLK_ENABLE_DISABLE_FVAL                                 0x0
#define HWIO_DPC_REG_DPCC_MC1_XO_CBCR_CLK_ENABLE_ENABLE_FVAL                                  0x1

#define HWIO_DPC_REG_DPCC_MC2_XO_CBCR_ADDR                                             (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE      + 0x000000b8)
#define HWIO_DPC_REG_DPCC_MC2_XO_CBCR_PHYS                                             (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_PHYS + 0x000000b8)
#define HWIO_DPC_REG_DPCC_MC2_XO_CBCR_OFFS                                             (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_OFFS + 0x000000b8)
#define HWIO_DPC_REG_DPCC_MC2_XO_CBCR_RMSK                                             0x80000005
#define HWIO_DPC_REG_DPCC_MC2_XO_CBCR_IN          \
        in_dword_masked(HWIO_DPC_REG_DPCC_MC2_XO_CBCR_ADDR, HWIO_DPC_REG_DPCC_MC2_XO_CBCR_RMSK)
#define HWIO_DPC_REG_DPCC_MC2_XO_CBCR_INM(m)      \
        in_dword_masked(HWIO_DPC_REG_DPCC_MC2_XO_CBCR_ADDR, m)
#define HWIO_DPC_REG_DPCC_MC2_XO_CBCR_OUT(v)      \
        out_dword(HWIO_DPC_REG_DPCC_MC2_XO_CBCR_ADDR,v)
#define HWIO_DPC_REG_DPCC_MC2_XO_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DPC_REG_DPCC_MC2_XO_CBCR_ADDR,m,v,HWIO_DPC_REG_DPCC_MC2_XO_CBCR_IN)
#define HWIO_DPC_REG_DPCC_MC2_XO_CBCR_CLK_OFF_BMSK                                     0x80000000
#define HWIO_DPC_REG_DPCC_MC2_XO_CBCR_CLK_OFF_SHFT                                           0x1f
#define HWIO_DPC_REG_DPCC_MC2_XO_CBCR_CLK_ARES_BMSK                                           0x4
#define HWIO_DPC_REG_DPCC_MC2_XO_CBCR_CLK_ARES_SHFT                                           0x2
#define HWIO_DPC_REG_DPCC_MC2_XO_CBCR_CLK_ARES_NO_RESET_FVAL                                  0x0
#define HWIO_DPC_REG_DPCC_MC2_XO_CBCR_CLK_ARES_RESET_FVAL                                     0x1
#define HWIO_DPC_REG_DPCC_MC2_XO_CBCR_CLK_ENABLE_BMSK                                         0x1
#define HWIO_DPC_REG_DPCC_MC2_XO_CBCR_CLK_ENABLE_SHFT                                         0x0
#define HWIO_DPC_REG_DPCC_MC2_XO_CBCR_CLK_ENABLE_DISABLE_FVAL                                 0x0
#define HWIO_DPC_REG_DPCC_MC2_XO_CBCR_CLK_ENABLE_ENABLE_FVAL                                  0x1

#define HWIO_DPC_REG_DPCC_MC3_XO_CBCR_ADDR                                             (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE      + 0x000000bc)
#define HWIO_DPC_REG_DPCC_MC3_XO_CBCR_PHYS                                             (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_PHYS + 0x000000bc)
#define HWIO_DPC_REG_DPCC_MC3_XO_CBCR_OFFS                                             (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_OFFS + 0x000000bc)
#define HWIO_DPC_REG_DPCC_MC3_XO_CBCR_RMSK                                             0x80000005
#define HWIO_DPC_REG_DPCC_MC3_XO_CBCR_IN          \
        in_dword_masked(HWIO_DPC_REG_DPCC_MC3_XO_CBCR_ADDR, HWIO_DPC_REG_DPCC_MC3_XO_CBCR_RMSK)
#define HWIO_DPC_REG_DPCC_MC3_XO_CBCR_INM(m)      \
        in_dword_masked(HWIO_DPC_REG_DPCC_MC3_XO_CBCR_ADDR, m)
#define HWIO_DPC_REG_DPCC_MC3_XO_CBCR_OUT(v)      \
        out_dword(HWIO_DPC_REG_DPCC_MC3_XO_CBCR_ADDR,v)
#define HWIO_DPC_REG_DPCC_MC3_XO_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DPC_REG_DPCC_MC3_XO_CBCR_ADDR,m,v,HWIO_DPC_REG_DPCC_MC3_XO_CBCR_IN)
#define HWIO_DPC_REG_DPCC_MC3_XO_CBCR_CLK_OFF_BMSK                                     0x80000000
#define HWIO_DPC_REG_DPCC_MC3_XO_CBCR_CLK_OFF_SHFT                                           0x1f
#define HWIO_DPC_REG_DPCC_MC3_XO_CBCR_CLK_ARES_BMSK                                           0x4
#define HWIO_DPC_REG_DPCC_MC3_XO_CBCR_CLK_ARES_SHFT                                           0x2
#define HWIO_DPC_REG_DPCC_MC3_XO_CBCR_CLK_ARES_NO_RESET_FVAL                                  0x0
#define HWIO_DPC_REG_DPCC_MC3_XO_CBCR_CLK_ARES_RESET_FVAL                                     0x1
#define HWIO_DPC_REG_DPCC_MC3_XO_CBCR_CLK_ENABLE_BMSK                                         0x1
#define HWIO_DPC_REG_DPCC_MC3_XO_CBCR_CLK_ENABLE_SHFT                                         0x0
#define HWIO_DPC_REG_DPCC_MC3_XO_CBCR_CLK_ENABLE_DISABLE_FVAL                                 0x0
#define HWIO_DPC_REG_DPCC_MC3_XO_CBCR_CLK_ENABLE_ENABLE_FVAL                                  0x1

#define HWIO_DPC_REG_DPCC_MC0_SLEEP_CBCR_ADDR                                          (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE      + 0x000000c0)
#define HWIO_DPC_REG_DPCC_MC0_SLEEP_CBCR_PHYS                                          (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_PHYS + 0x000000c0)
#define HWIO_DPC_REG_DPCC_MC0_SLEEP_CBCR_OFFS                                          (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_OFFS + 0x000000c0)
#define HWIO_DPC_REG_DPCC_MC0_SLEEP_CBCR_RMSK                                          0x80000005
#define HWIO_DPC_REG_DPCC_MC0_SLEEP_CBCR_IN          \
        in_dword_masked(HWIO_DPC_REG_DPCC_MC0_SLEEP_CBCR_ADDR, HWIO_DPC_REG_DPCC_MC0_SLEEP_CBCR_RMSK)
#define HWIO_DPC_REG_DPCC_MC0_SLEEP_CBCR_INM(m)      \
        in_dword_masked(HWIO_DPC_REG_DPCC_MC0_SLEEP_CBCR_ADDR, m)
#define HWIO_DPC_REG_DPCC_MC0_SLEEP_CBCR_OUT(v)      \
        out_dword(HWIO_DPC_REG_DPCC_MC0_SLEEP_CBCR_ADDR,v)
#define HWIO_DPC_REG_DPCC_MC0_SLEEP_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DPC_REG_DPCC_MC0_SLEEP_CBCR_ADDR,m,v,HWIO_DPC_REG_DPCC_MC0_SLEEP_CBCR_IN)
#define HWIO_DPC_REG_DPCC_MC0_SLEEP_CBCR_CLK_OFF_BMSK                                  0x80000000
#define HWIO_DPC_REG_DPCC_MC0_SLEEP_CBCR_CLK_OFF_SHFT                                        0x1f
#define HWIO_DPC_REG_DPCC_MC0_SLEEP_CBCR_CLK_ARES_BMSK                                        0x4
#define HWIO_DPC_REG_DPCC_MC0_SLEEP_CBCR_CLK_ARES_SHFT                                        0x2
#define HWIO_DPC_REG_DPCC_MC0_SLEEP_CBCR_CLK_ARES_NO_RESET_FVAL                               0x0
#define HWIO_DPC_REG_DPCC_MC0_SLEEP_CBCR_CLK_ARES_RESET_FVAL                                  0x1
#define HWIO_DPC_REG_DPCC_MC0_SLEEP_CBCR_CLK_ENABLE_BMSK                                      0x1
#define HWIO_DPC_REG_DPCC_MC0_SLEEP_CBCR_CLK_ENABLE_SHFT                                      0x0
#define HWIO_DPC_REG_DPCC_MC0_SLEEP_CBCR_CLK_ENABLE_DISABLE_FVAL                              0x0
#define HWIO_DPC_REG_DPCC_MC0_SLEEP_CBCR_CLK_ENABLE_ENABLE_FVAL                               0x1

#define HWIO_DPC_REG_DPCC_MC1_SLEEP_CBCR_ADDR                                          (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE      + 0x000000c4)
#define HWIO_DPC_REG_DPCC_MC1_SLEEP_CBCR_PHYS                                          (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_PHYS + 0x000000c4)
#define HWIO_DPC_REG_DPCC_MC1_SLEEP_CBCR_OFFS                                          (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_OFFS + 0x000000c4)
#define HWIO_DPC_REG_DPCC_MC1_SLEEP_CBCR_RMSK                                          0x80000005
#define HWIO_DPC_REG_DPCC_MC1_SLEEP_CBCR_IN          \
        in_dword_masked(HWIO_DPC_REG_DPCC_MC1_SLEEP_CBCR_ADDR, HWIO_DPC_REG_DPCC_MC1_SLEEP_CBCR_RMSK)
#define HWIO_DPC_REG_DPCC_MC1_SLEEP_CBCR_INM(m)      \
        in_dword_masked(HWIO_DPC_REG_DPCC_MC1_SLEEP_CBCR_ADDR, m)
#define HWIO_DPC_REG_DPCC_MC1_SLEEP_CBCR_OUT(v)      \
        out_dword(HWIO_DPC_REG_DPCC_MC1_SLEEP_CBCR_ADDR,v)
#define HWIO_DPC_REG_DPCC_MC1_SLEEP_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DPC_REG_DPCC_MC1_SLEEP_CBCR_ADDR,m,v,HWIO_DPC_REG_DPCC_MC1_SLEEP_CBCR_IN)
#define HWIO_DPC_REG_DPCC_MC1_SLEEP_CBCR_CLK_OFF_BMSK                                  0x80000000
#define HWIO_DPC_REG_DPCC_MC1_SLEEP_CBCR_CLK_OFF_SHFT                                        0x1f
#define HWIO_DPC_REG_DPCC_MC1_SLEEP_CBCR_CLK_ARES_BMSK                                        0x4
#define HWIO_DPC_REG_DPCC_MC1_SLEEP_CBCR_CLK_ARES_SHFT                                        0x2
#define HWIO_DPC_REG_DPCC_MC1_SLEEP_CBCR_CLK_ARES_NO_RESET_FVAL                               0x0
#define HWIO_DPC_REG_DPCC_MC1_SLEEP_CBCR_CLK_ARES_RESET_FVAL                                  0x1
#define HWIO_DPC_REG_DPCC_MC1_SLEEP_CBCR_CLK_ENABLE_BMSK                                      0x1
#define HWIO_DPC_REG_DPCC_MC1_SLEEP_CBCR_CLK_ENABLE_SHFT                                      0x0
#define HWIO_DPC_REG_DPCC_MC1_SLEEP_CBCR_CLK_ENABLE_DISABLE_FVAL                              0x0
#define HWIO_DPC_REG_DPCC_MC1_SLEEP_CBCR_CLK_ENABLE_ENABLE_FVAL                               0x1

#define HWIO_DPC_REG_DPCC_MC2_SLEEP_CBCR_ADDR                                          (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE      + 0x000000c8)
#define HWIO_DPC_REG_DPCC_MC2_SLEEP_CBCR_PHYS                                          (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_PHYS + 0x000000c8)
#define HWIO_DPC_REG_DPCC_MC2_SLEEP_CBCR_OFFS                                          (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_OFFS + 0x000000c8)
#define HWIO_DPC_REG_DPCC_MC2_SLEEP_CBCR_RMSK                                          0x80000005
#define HWIO_DPC_REG_DPCC_MC2_SLEEP_CBCR_IN          \
        in_dword_masked(HWIO_DPC_REG_DPCC_MC2_SLEEP_CBCR_ADDR, HWIO_DPC_REG_DPCC_MC2_SLEEP_CBCR_RMSK)
#define HWIO_DPC_REG_DPCC_MC2_SLEEP_CBCR_INM(m)      \
        in_dword_masked(HWIO_DPC_REG_DPCC_MC2_SLEEP_CBCR_ADDR, m)
#define HWIO_DPC_REG_DPCC_MC2_SLEEP_CBCR_OUT(v)      \
        out_dword(HWIO_DPC_REG_DPCC_MC2_SLEEP_CBCR_ADDR,v)
#define HWIO_DPC_REG_DPCC_MC2_SLEEP_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DPC_REG_DPCC_MC2_SLEEP_CBCR_ADDR,m,v,HWIO_DPC_REG_DPCC_MC2_SLEEP_CBCR_IN)
#define HWIO_DPC_REG_DPCC_MC2_SLEEP_CBCR_CLK_OFF_BMSK                                  0x80000000
#define HWIO_DPC_REG_DPCC_MC2_SLEEP_CBCR_CLK_OFF_SHFT                                        0x1f
#define HWIO_DPC_REG_DPCC_MC2_SLEEP_CBCR_CLK_ARES_BMSK                                        0x4
#define HWIO_DPC_REG_DPCC_MC2_SLEEP_CBCR_CLK_ARES_SHFT                                        0x2
#define HWIO_DPC_REG_DPCC_MC2_SLEEP_CBCR_CLK_ARES_NO_RESET_FVAL                               0x0
#define HWIO_DPC_REG_DPCC_MC2_SLEEP_CBCR_CLK_ARES_RESET_FVAL                                  0x1
#define HWIO_DPC_REG_DPCC_MC2_SLEEP_CBCR_CLK_ENABLE_BMSK                                      0x1
#define HWIO_DPC_REG_DPCC_MC2_SLEEP_CBCR_CLK_ENABLE_SHFT                                      0x0
#define HWIO_DPC_REG_DPCC_MC2_SLEEP_CBCR_CLK_ENABLE_DISABLE_FVAL                              0x0
#define HWIO_DPC_REG_DPCC_MC2_SLEEP_CBCR_CLK_ENABLE_ENABLE_FVAL                               0x1

#define HWIO_DPC_REG_DPCC_MC3_SLEEP_CBCR_ADDR                                          (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE      + 0x000000cc)
#define HWIO_DPC_REG_DPCC_MC3_SLEEP_CBCR_PHYS                                          (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_PHYS + 0x000000cc)
#define HWIO_DPC_REG_DPCC_MC3_SLEEP_CBCR_OFFS                                          (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_OFFS + 0x000000cc)
#define HWIO_DPC_REG_DPCC_MC3_SLEEP_CBCR_RMSK                                          0x80000005
#define HWIO_DPC_REG_DPCC_MC3_SLEEP_CBCR_IN          \
        in_dword_masked(HWIO_DPC_REG_DPCC_MC3_SLEEP_CBCR_ADDR, HWIO_DPC_REG_DPCC_MC3_SLEEP_CBCR_RMSK)
#define HWIO_DPC_REG_DPCC_MC3_SLEEP_CBCR_INM(m)      \
        in_dword_masked(HWIO_DPC_REG_DPCC_MC3_SLEEP_CBCR_ADDR, m)
#define HWIO_DPC_REG_DPCC_MC3_SLEEP_CBCR_OUT(v)      \
        out_dword(HWIO_DPC_REG_DPCC_MC3_SLEEP_CBCR_ADDR,v)
#define HWIO_DPC_REG_DPCC_MC3_SLEEP_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DPC_REG_DPCC_MC3_SLEEP_CBCR_ADDR,m,v,HWIO_DPC_REG_DPCC_MC3_SLEEP_CBCR_IN)
#define HWIO_DPC_REG_DPCC_MC3_SLEEP_CBCR_CLK_OFF_BMSK                                  0x80000000
#define HWIO_DPC_REG_DPCC_MC3_SLEEP_CBCR_CLK_OFF_SHFT                                        0x1f
#define HWIO_DPC_REG_DPCC_MC3_SLEEP_CBCR_CLK_ARES_BMSK                                        0x4
#define HWIO_DPC_REG_DPCC_MC3_SLEEP_CBCR_CLK_ARES_SHFT                                        0x2
#define HWIO_DPC_REG_DPCC_MC3_SLEEP_CBCR_CLK_ARES_NO_RESET_FVAL                               0x0
#define HWIO_DPC_REG_DPCC_MC3_SLEEP_CBCR_CLK_ARES_RESET_FVAL                                  0x1
#define HWIO_DPC_REG_DPCC_MC3_SLEEP_CBCR_CLK_ENABLE_BMSK                                      0x1
#define HWIO_DPC_REG_DPCC_MC3_SLEEP_CBCR_CLK_ENABLE_SHFT                                      0x0
#define HWIO_DPC_REG_DPCC_MC3_SLEEP_CBCR_CLK_ENABLE_DISABLE_FVAL                              0x0
#define HWIO_DPC_REG_DPCC_MC3_SLEEP_CBCR_CLK_ENABLE_ENABLE_FVAL                               0x1

#define HWIO_DPC_REG_DPCC_MC0_SHUB_CBCR_ADDR                                           (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE      + 0x000000d0)
#define HWIO_DPC_REG_DPCC_MC0_SHUB_CBCR_PHYS                                           (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_PHYS + 0x000000d0)
#define HWIO_DPC_REG_DPCC_MC0_SHUB_CBCR_OFFS                                           (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_OFFS + 0x000000d0)
#define HWIO_DPC_REG_DPCC_MC0_SHUB_CBCR_RMSK                                           0x80000005
#define HWIO_DPC_REG_DPCC_MC0_SHUB_CBCR_IN          \
        in_dword_masked(HWIO_DPC_REG_DPCC_MC0_SHUB_CBCR_ADDR, HWIO_DPC_REG_DPCC_MC0_SHUB_CBCR_RMSK)
#define HWIO_DPC_REG_DPCC_MC0_SHUB_CBCR_INM(m)      \
        in_dword_masked(HWIO_DPC_REG_DPCC_MC0_SHUB_CBCR_ADDR, m)
#define HWIO_DPC_REG_DPCC_MC0_SHUB_CBCR_OUT(v)      \
        out_dword(HWIO_DPC_REG_DPCC_MC0_SHUB_CBCR_ADDR,v)
#define HWIO_DPC_REG_DPCC_MC0_SHUB_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DPC_REG_DPCC_MC0_SHUB_CBCR_ADDR,m,v,HWIO_DPC_REG_DPCC_MC0_SHUB_CBCR_IN)
#define HWIO_DPC_REG_DPCC_MC0_SHUB_CBCR_CLK_OFF_BMSK                                   0x80000000
#define HWIO_DPC_REG_DPCC_MC0_SHUB_CBCR_CLK_OFF_SHFT                                         0x1f
#define HWIO_DPC_REG_DPCC_MC0_SHUB_CBCR_CLK_ARES_BMSK                                         0x4
#define HWIO_DPC_REG_DPCC_MC0_SHUB_CBCR_CLK_ARES_SHFT                                         0x2
#define HWIO_DPC_REG_DPCC_MC0_SHUB_CBCR_CLK_ARES_NO_RESET_FVAL                                0x0
#define HWIO_DPC_REG_DPCC_MC0_SHUB_CBCR_CLK_ARES_RESET_FVAL                                   0x1
#define HWIO_DPC_REG_DPCC_MC0_SHUB_CBCR_CLK_ENABLE_BMSK                                       0x1
#define HWIO_DPC_REG_DPCC_MC0_SHUB_CBCR_CLK_ENABLE_SHFT                                       0x0
#define HWIO_DPC_REG_DPCC_MC0_SHUB_CBCR_CLK_ENABLE_DISABLE_FVAL                               0x0
#define HWIO_DPC_REG_DPCC_MC0_SHUB_CBCR_CLK_ENABLE_ENABLE_FVAL                                0x1

#define HWIO_DPC_REG_DPCC_MC1_SHUB_CBCR_ADDR                                           (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE      + 0x000000d4)
#define HWIO_DPC_REG_DPCC_MC1_SHUB_CBCR_PHYS                                           (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_PHYS + 0x000000d4)
#define HWIO_DPC_REG_DPCC_MC1_SHUB_CBCR_OFFS                                           (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_OFFS + 0x000000d4)
#define HWIO_DPC_REG_DPCC_MC1_SHUB_CBCR_RMSK                                           0x80000005
#define HWIO_DPC_REG_DPCC_MC1_SHUB_CBCR_IN          \
        in_dword_masked(HWIO_DPC_REG_DPCC_MC1_SHUB_CBCR_ADDR, HWIO_DPC_REG_DPCC_MC1_SHUB_CBCR_RMSK)
#define HWIO_DPC_REG_DPCC_MC1_SHUB_CBCR_INM(m)      \
        in_dword_masked(HWIO_DPC_REG_DPCC_MC1_SHUB_CBCR_ADDR, m)
#define HWIO_DPC_REG_DPCC_MC1_SHUB_CBCR_OUT(v)      \
        out_dword(HWIO_DPC_REG_DPCC_MC1_SHUB_CBCR_ADDR,v)
#define HWIO_DPC_REG_DPCC_MC1_SHUB_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DPC_REG_DPCC_MC1_SHUB_CBCR_ADDR,m,v,HWIO_DPC_REG_DPCC_MC1_SHUB_CBCR_IN)
#define HWIO_DPC_REG_DPCC_MC1_SHUB_CBCR_CLK_OFF_BMSK                                   0x80000000
#define HWIO_DPC_REG_DPCC_MC1_SHUB_CBCR_CLK_OFF_SHFT                                         0x1f
#define HWIO_DPC_REG_DPCC_MC1_SHUB_CBCR_CLK_ARES_BMSK                                         0x4
#define HWIO_DPC_REG_DPCC_MC1_SHUB_CBCR_CLK_ARES_SHFT                                         0x2
#define HWIO_DPC_REG_DPCC_MC1_SHUB_CBCR_CLK_ARES_NO_RESET_FVAL                                0x0
#define HWIO_DPC_REG_DPCC_MC1_SHUB_CBCR_CLK_ARES_RESET_FVAL                                   0x1
#define HWIO_DPC_REG_DPCC_MC1_SHUB_CBCR_CLK_ENABLE_BMSK                                       0x1
#define HWIO_DPC_REG_DPCC_MC1_SHUB_CBCR_CLK_ENABLE_SHFT                                       0x0
#define HWIO_DPC_REG_DPCC_MC1_SHUB_CBCR_CLK_ENABLE_DISABLE_FVAL                               0x0
#define HWIO_DPC_REG_DPCC_MC1_SHUB_CBCR_CLK_ENABLE_ENABLE_FVAL                                0x1

#define HWIO_DPC_REG_DPCC_MC2_SHUB_CBCR_ADDR                                           (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE      + 0x000000d8)
#define HWIO_DPC_REG_DPCC_MC2_SHUB_CBCR_PHYS                                           (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_PHYS + 0x000000d8)
#define HWIO_DPC_REG_DPCC_MC2_SHUB_CBCR_OFFS                                           (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_OFFS + 0x000000d8)
#define HWIO_DPC_REG_DPCC_MC2_SHUB_CBCR_RMSK                                           0x80000005
#define HWIO_DPC_REG_DPCC_MC2_SHUB_CBCR_IN          \
        in_dword_masked(HWIO_DPC_REG_DPCC_MC2_SHUB_CBCR_ADDR, HWIO_DPC_REG_DPCC_MC2_SHUB_CBCR_RMSK)
#define HWIO_DPC_REG_DPCC_MC2_SHUB_CBCR_INM(m)      \
        in_dword_masked(HWIO_DPC_REG_DPCC_MC2_SHUB_CBCR_ADDR, m)
#define HWIO_DPC_REG_DPCC_MC2_SHUB_CBCR_OUT(v)      \
        out_dword(HWIO_DPC_REG_DPCC_MC2_SHUB_CBCR_ADDR,v)
#define HWIO_DPC_REG_DPCC_MC2_SHUB_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DPC_REG_DPCC_MC2_SHUB_CBCR_ADDR,m,v,HWIO_DPC_REG_DPCC_MC2_SHUB_CBCR_IN)
#define HWIO_DPC_REG_DPCC_MC2_SHUB_CBCR_CLK_OFF_BMSK                                   0x80000000
#define HWIO_DPC_REG_DPCC_MC2_SHUB_CBCR_CLK_OFF_SHFT                                         0x1f
#define HWIO_DPC_REG_DPCC_MC2_SHUB_CBCR_CLK_ARES_BMSK                                         0x4
#define HWIO_DPC_REG_DPCC_MC2_SHUB_CBCR_CLK_ARES_SHFT                                         0x2
#define HWIO_DPC_REG_DPCC_MC2_SHUB_CBCR_CLK_ARES_NO_RESET_FVAL                                0x0
#define HWIO_DPC_REG_DPCC_MC2_SHUB_CBCR_CLK_ARES_RESET_FVAL                                   0x1
#define HWIO_DPC_REG_DPCC_MC2_SHUB_CBCR_CLK_ENABLE_BMSK                                       0x1
#define HWIO_DPC_REG_DPCC_MC2_SHUB_CBCR_CLK_ENABLE_SHFT                                       0x0
#define HWIO_DPC_REG_DPCC_MC2_SHUB_CBCR_CLK_ENABLE_DISABLE_FVAL                               0x0
#define HWIO_DPC_REG_DPCC_MC2_SHUB_CBCR_CLK_ENABLE_ENABLE_FVAL                                0x1

#define HWIO_DPC_REG_DPCC_MC3_SHUB_CBCR_ADDR                                           (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE      + 0x000000dc)
#define HWIO_DPC_REG_DPCC_MC3_SHUB_CBCR_PHYS                                           (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_PHYS + 0x000000dc)
#define HWIO_DPC_REG_DPCC_MC3_SHUB_CBCR_OFFS                                           (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_OFFS + 0x000000dc)
#define HWIO_DPC_REG_DPCC_MC3_SHUB_CBCR_RMSK                                           0x80000005
#define HWIO_DPC_REG_DPCC_MC3_SHUB_CBCR_IN          \
        in_dword_masked(HWIO_DPC_REG_DPCC_MC3_SHUB_CBCR_ADDR, HWIO_DPC_REG_DPCC_MC3_SHUB_CBCR_RMSK)
#define HWIO_DPC_REG_DPCC_MC3_SHUB_CBCR_INM(m)      \
        in_dword_masked(HWIO_DPC_REG_DPCC_MC3_SHUB_CBCR_ADDR, m)
#define HWIO_DPC_REG_DPCC_MC3_SHUB_CBCR_OUT(v)      \
        out_dword(HWIO_DPC_REG_DPCC_MC3_SHUB_CBCR_ADDR,v)
#define HWIO_DPC_REG_DPCC_MC3_SHUB_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DPC_REG_DPCC_MC3_SHUB_CBCR_ADDR,m,v,HWIO_DPC_REG_DPCC_MC3_SHUB_CBCR_IN)
#define HWIO_DPC_REG_DPCC_MC3_SHUB_CBCR_CLK_OFF_BMSK                                   0x80000000
#define HWIO_DPC_REG_DPCC_MC3_SHUB_CBCR_CLK_OFF_SHFT                                         0x1f
#define HWIO_DPC_REG_DPCC_MC3_SHUB_CBCR_CLK_ARES_BMSK                                         0x4
#define HWIO_DPC_REG_DPCC_MC3_SHUB_CBCR_CLK_ARES_SHFT                                         0x2
#define HWIO_DPC_REG_DPCC_MC3_SHUB_CBCR_CLK_ARES_NO_RESET_FVAL                                0x0
#define HWIO_DPC_REG_DPCC_MC3_SHUB_CBCR_CLK_ARES_RESET_FVAL                                   0x1
#define HWIO_DPC_REG_DPCC_MC3_SHUB_CBCR_CLK_ENABLE_BMSK                                       0x1
#define HWIO_DPC_REG_DPCC_MC3_SHUB_CBCR_CLK_ENABLE_SHFT                                       0x0
#define HWIO_DPC_REG_DPCC_MC3_SHUB_CBCR_CLK_ENABLE_DISABLE_FVAL                               0x0
#define HWIO_DPC_REG_DPCC_MC3_SHUB_CBCR_CLK_ENABLE_ENABLE_FVAL                                0x1

#define HWIO_DPC_REG_DPCC_MC_CFG_CBCR_ADDR                                             (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE      + 0x000000e0)
#define HWIO_DPC_REG_DPCC_MC_CFG_CBCR_PHYS                                             (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_PHYS + 0x000000e0)
#define HWIO_DPC_REG_DPCC_MC_CFG_CBCR_OFFS                                             (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_OFFS + 0x000000e0)
#define HWIO_DPC_REG_DPCC_MC_CFG_CBCR_RMSK                                             0x80000005
#define HWIO_DPC_REG_DPCC_MC_CFG_CBCR_IN          \
        in_dword_masked(HWIO_DPC_REG_DPCC_MC_CFG_CBCR_ADDR, HWIO_DPC_REG_DPCC_MC_CFG_CBCR_RMSK)
#define HWIO_DPC_REG_DPCC_MC_CFG_CBCR_INM(m)      \
        in_dword_masked(HWIO_DPC_REG_DPCC_MC_CFG_CBCR_ADDR, m)
#define HWIO_DPC_REG_DPCC_MC_CFG_CBCR_OUT(v)      \
        out_dword(HWIO_DPC_REG_DPCC_MC_CFG_CBCR_ADDR,v)
#define HWIO_DPC_REG_DPCC_MC_CFG_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DPC_REG_DPCC_MC_CFG_CBCR_ADDR,m,v,HWIO_DPC_REG_DPCC_MC_CFG_CBCR_IN)
#define HWIO_DPC_REG_DPCC_MC_CFG_CBCR_CLK_OFF_BMSK                                     0x80000000
#define HWIO_DPC_REG_DPCC_MC_CFG_CBCR_CLK_OFF_SHFT                                           0x1f
#define HWIO_DPC_REG_DPCC_MC_CFG_CBCR_CLK_ARES_BMSK                                           0x4
#define HWIO_DPC_REG_DPCC_MC_CFG_CBCR_CLK_ARES_SHFT                                           0x2
#define HWIO_DPC_REG_DPCC_MC_CFG_CBCR_CLK_ARES_NO_RESET_FVAL                                  0x0
#define HWIO_DPC_REG_DPCC_MC_CFG_CBCR_CLK_ARES_RESET_FVAL                                     0x1
#define HWIO_DPC_REG_DPCC_MC_CFG_CBCR_CLK_ENABLE_BMSK                                         0x1
#define HWIO_DPC_REG_DPCC_MC_CFG_CBCR_CLK_ENABLE_SHFT                                         0x0
#define HWIO_DPC_REG_DPCC_MC_CFG_CBCR_CLK_ENABLE_DISABLE_FVAL                                 0x0
#define HWIO_DPC_REG_DPCC_MC_CFG_CBCR_CLK_ENABLE_ENABLE_FVAL                                  0x1

#define HWIO_DPC_REG_DPCC_MC_ATB_CBCR_ADDR                                             (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE      + 0x000000e4)
#define HWIO_DPC_REG_DPCC_MC_ATB_CBCR_PHYS                                             (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_PHYS + 0x000000e4)
#define HWIO_DPC_REG_DPCC_MC_ATB_CBCR_OFFS                                             (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_OFFS + 0x000000e4)
#define HWIO_DPC_REG_DPCC_MC_ATB_CBCR_RMSK                                             0x80000005
#define HWIO_DPC_REG_DPCC_MC_ATB_CBCR_IN          \
        in_dword_masked(HWIO_DPC_REG_DPCC_MC_ATB_CBCR_ADDR, HWIO_DPC_REG_DPCC_MC_ATB_CBCR_RMSK)
#define HWIO_DPC_REG_DPCC_MC_ATB_CBCR_INM(m)      \
        in_dword_masked(HWIO_DPC_REG_DPCC_MC_ATB_CBCR_ADDR, m)
#define HWIO_DPC_REG_DPCC_MC_ATB_CBCR_OUT(v)      \
        out_dword(HWIO_DPC_REG_DPCC_MC_ATB_CBCR_ADDR,v)
#define HWIO_DPC_REG_DPCC_MC_ATB_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DPC_REG_DPCC_MC_ATB_CBCR_ADDR,m,v,HWIO_DPC_REG_DPCC_MC_ATB_CBCR_IN)
#define HWIO_DPC_REG_DPCC_MC_ATB_CBCR_CLK_OFF_BMSK                                     0x80000000
#define HWIO_DPC_REG_DPCC_MC_ATB_CBCR_CLK_OFF_SHFT                                           0x1f
#define HWIO_DPC_REG_DPCC_MC_ATB_CBCR_CLK_ARES_BMSK                                           0x4
#define HWIO_DPC_REG_DPCC_MC_ATB_CBCR_CLK_ARES_SHFT                                           0x2
#define HWIO_DPC_REG_DPCC_MC_ATB_CBCR_CLK_ARES_NO_RESET_FVAL                                  0x0
#define HWIO_DPC_REG_DPCC_MC_ATB_CBCR_CLK_ARES_RESET_FVAL                                     0x1
#define HWIO_DPC_REG_DPCC_MC_ATB_CBCR_CLK_ENABLE_BMSK                                         0x1
#define HWIO_DPC_REG_DPCC_MC_ATB_CBCR_CLK_ENABLE_SHFT                                         0x0
#define HWIO_DPC_REG_DPCC_MC_ATB_CBCR_CLK_ENABLE_DISABLE_FVAL                                 0x0
#define HWIO_DPC_REG_DPCC_MC_ATB_CBCR_CLK_ENABLE_ENABLE_FVAL                                  0x1

#define HWIO_DPC_REG_DPCC_PHY_BCR_ADDR                                                 (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE      + 0x000000e8)
#define HWIO_DPC_REG_DPCC_PHY_BCR_PHYS                                                 (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_PHYS + 0x000000e8)
#define HWIO_DPC_REG_DPCC_PHY_BCR_OFFS                                                 (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_OFFS + 0x000000e8)
#define HWIO_DPC_REG_DPCC_PHY_BCR_RMSK                                                        0x1
#define HWIO_DPC_REG_DPCC_PHY_BCR_IN          \
        in_dword_masked(HWIO_DPC_REG_DPCC_PHY_BCR_ADDR, HWIO_DPC_REG_DPCC_PHY_BCR_RMSK)
#define HWIO_DPC_REG_DPCC_PHY_BCR_INM(m)      \
        in_dword_masked(HWIO_DPC_REG_DPCC_PHY_BCR_ADDR, m)
#define HWIO_DPC_REG_DPCC_PHY_BCR_OUT(v)      \
        out_dword(HWIO_DPC_REG_DPCC_PHY_BCR_ADDR,v)
#define HWIO_DPC_REG_DPCC_PHY_BCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DPC_REG_DPCC_PHY_BCR_ADDR,m,v,HWIO_DPC_REG_DPCC_PHY_BCR_IN)
#define HWIO_DPC_REG_DPCC_PHY_BCR_BLK_ARES_BMSK                                               0x1
#define HWIO_DPC_REG_DPCC_PHY_BCR_BLK_ARES_SHFT                                               0x0
#define HWIO_DPC_REG_DPCC_PHY_BCR_BLK_ARES_DISABLE_FVAL                                       0x0
#define HWIO_DPC_REG_DPCC_PHY_BCR_BLK_ARES_ENABLE_FVAL                                        0x1

#define HWIO_DPC_REG_DPCC_PHY_GDSCR_ADDR                                               (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE      + 0x000000ec)
#define HWIO_DPC_REG_DPCC_PHY_GDSCR_PHYS                                               (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_PHYS + 0x000000ec)
#define HWIO_DPC_REG_DPCC_PHY_GDSCR_OFFS                                               (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_OFFS + 0x000000ec)
#define HWIO_DPC_REG_DPCC_PHY_GDSCR_RMSK                                               0xf8ffffff
#define HWIO_DPC_REG_DPCC_PHY_GDSCR_IN          \
        in_dword_masked(HWIO_DPC_REG_DPCC_PHY_GDSCR_ADDR, HWIO_DPC_REG_DPCC_PHY_GDSCR_RMSK)
#define HWIO_DPC_REG_DPCC_PHY_GDSCR_INM(m)      \
        in_dword_masked(HWIO_DPC_REG_DPCC_PHY_GDSCR_ADDR, m)
#define HWIO_DPC_REG_DPCC_PHY_GDSCR_OUT(v)      \
        out_dword(HWIO_DPC_REG_DPCC_PHY_GDSCR_ADDR,v)
#define HWIO_DPC_REG_DPCC_PHY_GDSCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DPC_REG_DPCC_PHY_GDSCR_ADDR,m,v,HWIO_DPC_REG_DPCC_PHY_GDSCR_IN)
#define HWIO_DPC_REG_DPCC_PHY_GDSCR_PWR_ON_BMSK                                        0x80000000
#define HWIO_DPC_REG_DPCC_PHY_GDSCR_PWR_ON_SHFT                                              0x1f
#define HWIO_DPC_REG_DPCC_PHY_GDSCR_GDSC_STATE_BMSK                                    0x78000000
#define HWIO_DPC_REG_DPCC_PHY_GDSCR_GDSC_STATE_SHFT                                          0x1b
#define HWIO_DPC_REG_DPCC_PHY_GDSCR_EN_REST_WAIT_BMSK                                    0xf00000
#define HWIO_DPC_REG_DPCC_PHY_GDSCR_EN_REST_WAIT_SHFT                                        0x14
#define HWIO_DPC_REG_DPCC_PHY_GDSCR_EN_FEW_WAIT_BMSK                                      0xf0000
#define HWIO_DPC_REG_DPCC_PHY_GDSCR_EN_FEW_WAIT_SHFT                                         0x10
#define HWIO_DPC_REG_DPCC_PHY_GDSCR_CLK_DIS_WAIT_BMSK                                      0xf000
#define HWIO_DPC_REG_DPCC_PHY_GDSCR_CLK_DIS_WAIT_SHFT                                         0xc
#define HWIO_DPC_REG_DPCC_PHY_GDSCR_RETAIN_FF_ENABLE_BMSK                                   0x800
#define HWIO_DPC_REG_DPCC_PHY_GDSCR_RETAIN_FF_ENABLE_SHFT                                     0xb
#define HWIO_DPC_REG_DPCC_PHY_GDSCR_RETAIN_FF_ENABLE_DISABLE_FVAL                             0x0
#define HWIO_DPC_REG_DPCC_PHY_GDSCR_RETAIN_FF_ENABLE_ENABLE_FVAL                              0x1
#define HWIO_DPC_REG_DPCC_PHY_GDSCR_RESTORE_BMSK                                            0x400
#define HWIO_DPC_REG_DPCC_PHY_GDSCR_RESTORE_SHFT                                              0xa
#define HWIO_DPC_REG_DPCC_PHY_GDSCR_RESTORE_DISABLE_FVAL                                      0x0
#define HWIO_DPC_REG_DPCC_PHY_GDSCR_RESTORE_ENABLE_FVAL                                       0x1
#define HWIO_DPC_REG_DPCC_PHY_GDSCR_SAVE_BMSK                                               0x200
#define HWIO_DPC_REG_DPCC_PHY_GDSCR_SAVE_SHFT                                                 0x9
#define HWIO_DPC_REG_DPCC_PHY_GDSCR_SAVE_DISABLE_FVAL                                         0x0
#define HWIO_DPC_REG_DPCC_PHY_GDSCR_SAVE_ENABLE_FVAL                                          0x1
#define HWIO_DPC_REG_DPCC_PHY_GDSCR_RETAIN_BMSK                                             0x100
#define HWIO_DPC_REG_DPCC_PHY_GDSCR_RETAIN_SHFT                                               0x8
#define HWIO_DPC_REG_DPCC_PHY_GDSCR_RETAIN_DISABLE_FVAL                                       0x0
#define HWIO_DPC_REG_DPCC_PHY_GDSCR_RETAIN_ENABLE_FVAL                                        0x1
#define HWIO_DPC_REG_DPCC_PHY_GDSCR_EN_REST_BMSK                                             0x80
#define HWIO_DPC_REG_DPCC_PHY_GDSCR_EN_REST_SHFT                                              0x7
#define HWIO_DPC_REG_DPCC_PHY_GDSCR_EN_REST_DISABLE_FVAL                                      0x0
#define HWIO_DPC_REG_DPCC_PHY_GDSCR_EN_REST_ENABLE_FVAL                                       0x1
#define HWIO_DPC_REG_DPCC_PHY_GDSCR_EN_FEW_BMSK                                              0x40
#define HWIO_DPC_REG_DPCC_PHY_GDSCR_EN_FEW_SHFT                                               0x6
#define HWIO_DPC_REG_DPCC_PHY_GDSCR_EN_FEW_DISABLE_FVAL                                       0x0
#define HWIO_DPC_REG_DPCC_PHY_GDSCR_EN_FEW_ENABLE_FVAL                                        0x1
#define HWIO_DPC_REG_DPCC_PHY_GDSCR_CLAMP_IO_BMSK                                            0x20
#define HWIO_DPC_REG_DPCC_PHY_GDSCR_CLAMP_IO_SHFT                                             0x5
#define HWIO_DPC_REG_DPCC_PHY_GDSCR_CLAMP_IO_DISABLE_FVAL                                     0x0
#define HWIO_DPC_REG_DPCC_PHY_GDSCR_CLAMP_IO_ENABLE_FVAL                                      0x1
#define HWIO_DPC_REG_DPCC_PHY_GDSCR_CLK_DISABLE_BMSK                                         0x10
#define HWIO_DPC_REG_DPCC_PHY_GDSCR_CLK_DISABLE_SHFT                                          0x4
#define HWIO_DPC_REG_DPCC_PHY_GDSCR_CLK_DISABLE_CLK_NOT_DISABLE_FVAL                          0x0
#define HWIO_DPC_REG_DPCC_PHY_GDSCR_CLK_DISABLE_CLK_DISABLE_FVAL                              0x1
#define HWIO_DPC_REG_DPCC_PHY_GDSCR_PD_ARES_BMSK                                              0x8
#define HWIO_DPC_REG_DPCC_PHY_GDSCR_PD_ARES_SHFT                                              0x3
#define HWIO_DPC_REG_DPCC_PHY_GDSCR_PD_ARES_NO_RESET_FVAL                                     0x0
#define HWIO_DPC_REG_DPCC_PHY_GDSCR_PD_ARES_RESET_FVAL                                        0x1
#define HWIO_DPC_REG_DPCC_PHY_GDSCR_SW_OVERRIDE_BMSK                                          0x4
#define HWIO_DPC_REG_DPCC_PHY_GDSCR_SW_OVERRIDE_SHFT                                          0x2
#define HWIO_DPC_REG_DPCC_PHY_GDSCR_SW_OVERRIDE_DISABLE_FVAL                                  0x0
#define HWIO_DPC_REG_DPCC_PHY_GDSCR_SW_OVERRIDE_ENABLE_FVAL                                   0x1
#define HWIO_DPC_REG_DPCC_PHY_GDSCR_HW_CONTROL_BMSK                                           0x2
#define HWIO_DPC_REG_DPCC_PHY_GDSCR_HW_CONTROL_SHFT                                           0x1
#define HWIO_DPC_REG_DPCC_PHY_GDSCR_HW_CONTROL_DISABLE_FVAL                                   0x0
#define HWIO_DPC_REG_DPCC_PHY_GDSCR_HW_CONTROL_ENABLE_FVAL                                    0x1
#define HWIO_DPC_REG_DPCC_PHY_GDSCR_SW_COLLAPSE_BMSK                                          0x1
#define HWIO_DPC_REG_DPCC_PHY_GDSCR_SW_COLLAPSE_SHFT                                          0x0
#define HWIO_DPC_REG_DPCC_PHY_GDSCR_SW_COLLAPSE_DISABLE_FVAL                                  0x0
#define HWIO_DPC_REG_DPCC_PHY_GDSCR_SW_COLLAPSE_ENABLE_FVAL                                   0x1

#define HWIO_DPC_REG_DPCC_PHY_CFG_GDSCR_ADDR                                           (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE      + 0x000000f0)
#define HWIO_DPC_REG_DPCC_PHY_CFG_GDSCR_PHYS                                           (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_PHYS + 0x000000f0)
#define HWIO_DPC_REG_DPCC_PHY_CFG_GDSCR_OFFS                                           (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_OFFS + 0x000000f0)
#define HWIO_DPC_REG_DPCC_PHY_CFG_GDSCR_RMSK                                           0xffffffff
#define HWIO_DPC_REG_DPCC_PHY_CFG_GDSCR_IN          \
        in_dword_masked(HWIO_DPC_REG_DPCC_PHY_CFG_GDSCR_ADDR, HWIO_DPC_REG_DPCC_PHY_CFG_GDSCR_RMSK)
#define HWIO_DPC_REG_DPCC_PHY_CFG_GDSCR_INM(m)      \
        in_dword_masked(HWIO_DPC_REG_DPCC_PHY_CFG_GDSCR_ADDR, m)
#define HWIO_DPC_REG_DPCC_PHY_CFG_GDSCR_OUT(v)      \
        out_dword(HWIO_DPC_REG_DPCC_PHY_CFG_GDSCR_ADDR,v)
#define HWIO_DPC_REG_DPCC_PHY_CFG_GDSCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DPC_REG_DPCC_PHY_CFG_GDSCR_ADDR,m,v,HWIO_DPC_REG_DPCC_PHY_CFG_GDSCR_IN)
#define HWIO_DPC_REG_DPCC_PHY_CFG_GDSCR_GDSC_SPARE_CTRL_IN_BMSK                        0xf0000000
#define HWIO_DPC_REG_DPCC_PHY_CFG_GDSCR_GDSC_SPARE_CTRL_IN_SHFT                              0x1c
#define HWIO_DPC_REG_DPCC_PHY_CFG_GDSCR_GDSC_SPARE_CTRL_OUT_BMSK                        0xc000000
#define HWIO_DPC_REG_DPCC_PHY_CFG_GDSCR_GDSC_SPARE_CTRL_OUT_SHFT                             0x1a
#define HWIO_DPC_REG_DPCC_PHY_CFG_GDSCR_GDSC_PWR_DWN_START_BMSK                         0x2000000
#define HWIO_DPC_REG_DPCC_PHY_CFG_GDSCR_GDSC_PWR_DWN_START_SHFT                              0x19
#define HWIO_DPC_REG_DPCC_PHY_CFG_GDSCR_GDSC_PWR_UP_START_BMSK                          0x1000000
#define HWIO_DPC_REG_DPCC_PHY_CFG_GDSCR_GDSC_PWR_UP_START_SHFT                               0x18
#define HWIO_DPC_REG_DPCC_PHY_CFG_GDSCR_GDSC_CFG_FSM_STATE_STATUS_BMSK                   0xf00000
#define HWIO_DPC_REG_DPCC_PHY_CFG_GDSCR_GDSC_CFG_FSM_STATE_STATUS_SHFT                       0x14
#define HWIO_DPC_REG_DPCC_PHY_CFG_GDSCR_GDSC_MEM_PWR_ACK_STATUS_BMSK                      0x80000
#define HWIO_DPC_REG_DPCC_PHY_CFG_GDSCR_GDSC_MEM_PWR_ACK_STATUS_SHFT                         0x13
#define HWIO_DPC_REG_DPCC_PHY_CFG_GDSCR_GDSC_ENR_ACK_STATUS_BMSK                          0x40000
#define HWIO_DPC_REG_DPCC_PHY_CFG_GDSCR_GDSC_ENR_ACK_STATUS_SHFT                             0x12
#define HWIO_DPC_REG_DPCC_PHY_CFG_GDSCR_GDSC_ENF_ACK_STATUS_BMSK                          0x20000
#define HWIO_DPC_REG_DPCC_PHY_CFG_GDSCR_GDSC_ENF_ACK_STATUS_SHFT                             0x11
#define HWIO_DPC_REG_DPCC_PHY_CFG_GDSCR_GDSC_POWER_UP_COMPLETE_BMSK                       0x10000
#define HWIO_DPC_REG_DPCC_PHY_CFG_GDSCR_GDSC_POWER_UP_COMPLETE_SHFT                          0x10
#define HWIO_DPC_REG_DPCC_PHY_CFG_GDSCR_GDSC_POWER_DOWN_COMPLETE_BMSK                      0x8000
#define HWIO_DPC_REG_DPCC_PHY_CFG_GDSCR_GDSC_POWER_DOWN_COMPLETE_SHFT                         0xf
#define HWIO_DPC_REG_DPCC_PHY_CFG_GDSCR_SOFTWARE_CONTROL_OVERRIDE_BMSK                     0x7800
#define HWIO_DPC_REG_DPCC_PHY_CFG_GDSCR_SOFTWARE_CONTROL_OVERRIDE_SHFT                        0xb
#define HWIO_DPC_REG_DPCC_PHY_CFG_GDSCR_GDSC_HANDSHAKE_DIS_BMSK                             0x400
#define HWIO_DPC_REG_DPCC_PHY_CFG_GDSCR_GDSC_HANDSHAKE_DIS_SHFT                               0xa
#define HWIO_DPC_REG_DPCC_PHY_CFG_GDSCR_GDSC_MEM_PERI_FORCE_IN_SW_BMSK                      0x200
#define HWIO_DPC_REG_DPCC_PHY_CFG_GDSCR_GDSC_MEM_PERI_FORCE_IN_SW_SHFT                        0x9
#define HWIO_DPC_REG_DPCC_PHY_CFG_GDSCR_GDSC_MEM_CORE_FORCE_IN_SW_BMSK                      0x100
#define HWIO_DPC_REG_DPCC_PHY_CFG_GDSCR_GDSC_MEM_CORE_FORCE_IN_SW_SHFT                        0x8
#define HWIO_DPC_REG_DPCC_PHY_CFG_GDSCR_GDSC_PHASE_RESET_EN_SW_BMSK                          0x80
#define HWIO_DPC_REG_DPCC_PHY_CFG_GDSCR_GDSC_PHASE_RESET_EN_SW_SHFT                           0x7
#define HWIO_DPC_REG_DPCC_PHY_CFG_GDSCR_GDSC_PHASE_RESET_DELAY_COUNT_SW_BMSK                 0x60
#define HWIO_DPC_REG_DPCC_PHY_CFG_GDSCR_GDSC_PHASE_RESET_DELAY_COUNT_SW_SHFT                  0x5
#define HWIO_DPC_REG_DPCC_PHY_CFG_GDSCR_GDSC_PSCBC_PWR_DWN_SW_BMSK                           0x10
#define HWIO_DPC_REG_DPCC_PHY_CFG_GDSCR_GDSC_PSCBC_PWR_DWN_SW_SHFT                            0x4
#define HWIO_DPC_REG_DPCC_PHY_CFG_GDSCR_UNCLAMP_IO_SOFTWARE_OVERRIDE_BMSK                     0x8
#define HWIO_DPC_REG_DPCC_PHY_CFG_GDSCR_UNCLAMP_IO_SOFTWARE_OVERRIDE_SHFT                     0x3
#define HWIO_DPC_REG_DPCC_PHY_CFG_GDSCR_SAVE_RESTORE_SOFTWARE_OVERRIDE_BMSK                   0x4
#define HWIO_DPC_REG_DPCC_PHY_CFG_GDSCR_SAVE_RESTORE_SOFTWARE_OVERRIDE_SHFT                   0x2
#define HWIO_DPC_REG_DPCC_PHY_CFG_GDSCR_CLAMP_IO_SOFTWARE_OVERRIDE_BMSK                       0x2
#define HWIO_DPC_REG_DPCC_PHY_CFG_GDSCR_CLAMP_IO_SOFTWARE_OVERRIDE_SHFT                       0x1
#define HWIO_DPC_REG_DPCC_PHY_CFG_GDSCR_DISABLE_CLK_SOFTWARE_OVERRIDE_BMSK                    0x1
#define HWIO_DPC_REG_DPCC_PHY_CFG_GDSCR_DISABLE_CLK_SOFTWARE_OVERRIDE_SHFT                    0x0

#define HWIO_DPC_REG_DPCC_PHY_CFG2_GDSCR_ADDR                                          (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE      + 0x000000f4)
#define HWIO_DPC_REG_DPCC_PHY_CFG2_GDSCR_PHYS                                          (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_PHYS + 0x000000f4)
#define HWIO_DPC_REG_DPCC_PHY_CFG2_GDSCR_OFFS                                          (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_OFFS + 0x000000f4)
#define HWIO_DPC_REG_DPCC_PHY_CFG2_GDSCR_RMSK                                             0x1ffff
#define HWIO_DPC_REG_DPCC_PHY_CFG2_GDSCR_IN          \
        in_dword_masked(HWIO_DPC_REG_DPCC_PHY_CFG2_GDSCR_ADDR, HWIO_DPC_REG_DPCC_PHY_CFG2_GDSCR_RMSK)
#define HWIO_DPC_REG_DPCC_PHY_CFG2_GDSCR_INM(m)      \
        in_dword_masked(HWIO_DPC_REG_DPCC_PHY_CFG2_GDSCR_ADDR, m)
#define HWIO_DPC_REG_DPCC_PHY_CFG2_GDSCR_OUT(v)      \
        out_dword(HWIO_DPC_REG_DPCC_PHY_CFG2_GDSCR_ADDR,v)
#define HWIO_DPC_REG_DPCC_PHY_CFG2_GDSCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DPC_REG_DPCC_PHY_CFG2_GDSCR_ADDR,m,v,HWIO_DPC_REG_DPCC_PHY_CFG2_GDSCR_IN)
#define HWIO_DPC_REG_DPCC_PHY_CFG2_GDSCR_GDSC_CLAMP_MEM_SW_BMSK                           0x10000
#define HWIO_DPC_REG_DPCC_PHY_CFG2_GDSCR_GDSC_CLAMP_MEM_SW_SHFT                              0x10
#define HWIO_DPC_REG_DPCC_PHY_CFG2_GDSCR_DLY_MEM_PWR_UP_BMSK                               0xf000
#define HWIO_DPC_REG_DPCC_PHY_CFG2_GDSCR_DLY_MEM_PWR_UP_SHFT                                  0xc
#define HWIO_DPC_REG_DPCC_PHY_CFG2_GDSCR_DLY_DEASSERT_CLAMP_MEM_BMSK                        0xf00
#define HWIO_DPC_REG_DPCC_PHY_CFG2_GDSCR_DLY_DEASSERT_CLAMP_MEM_SHFT                          0x8
#define HWIO_DPC_REG_DPCC_PHY_CFG2_GDSCR_DLY_ASSERT_CLAMP_MEM_BMSK                           0xf0
#define HWIO_DPC_REG_DPCC_PHY_CFG2_GDSCR_DLY_ASSERT_CLAMP_MEM_SHFT                            0x4
#define HWIO_DPC_REG_DPCC_PHY_CFG2_GDSCR_MEM_PWR_DWN_TIMEOUT_BMSK                             0xf
#define HWIO_DPC_REG_DPCC_PHY_CFG2_GDSCR_MEM_PWR_DWN_TIMEOUT_SHFT                             0x0

#define HWIO_DPC_REG_DPCC_PHY_XO_CBCR_ADDR                                             (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE      + 0x000000f8)
#define HWIO_DPC_REG_DPCC_PHY_XO_CBCR_PHYS                                             (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_PHYS + 0x000000f8)
#define HWIO_DPC_REG_DPCC_PHY_XO_CBCR_OFFS                                             (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_OFFS + 0x000000f8)
#define HWIO_DPC_REG_DPCC_PHY_XO_CBCR_RMSK                                             0x80000005
#define HWIO_DPC_REG_DPCC_PHY_XO_CBCR_IN          \
        in_dword_masked(HWIO_DPC_REG_DPCC_PHY_XO_CBCR_ADDR, HWIO_DPC_REG_DPCC_PHY_XO_CBCR_RMSK)
#define HWIO_DPC_REG_DPCC_PHY_XO_CBCR_INM(m)      \
        in_dword_masked(HWIO_DPC_REG_DPCC_PHY_XO_CBCR_ADDR, m)
#define HWIO_DPC_REG_DPCC_PHY_XO_CBCR_OUT(v)      \
        out_dword(HWIO_DPC_REG_DPCC_PHY_XO_CBCR_ADDR,v)
#define HWIO_DPC_REG_DPCC_PHY_XO_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DPC_REG_DPCC_PHY_XO_CBCR_ADDR,m,v,HWIO_DPC_REG_DPCC_PHY_XO_CBCR_IN)
#define HWIO_DPC_REG_DPCC_PHY_XO_CBCR_CLK_OFF_BMSK                                     0x80000000
#define HWIO_DPC_REG_DPCC_PHY_XO_CBCR_CLK_OFF_SHFT                                           0x1f
#define HWIO_DPC_REG_DPCC_PHY_XO_CBCR_CLK_ARES_BMSK                                           0x4
#define HWIO_DPC_REG_DPCC_PHY_XO_CBCR_CLK_ARES_SHFT                                           0x2
#define HWIO_DPC_REG_DPCC_PHY_XO_CBCR_CLK_ARES_NO_RESET_FVAL                                  0x0
#define HWIO_DPC_REG_DPCC_PHY_XO_CBCR_CLK_ARES_RESET_FVAL                                     0x1
#define HWIO_DPC_REG_DPCC_PHY_XO_CBCR_CLK_ENABLE_BMSK                                         0x1
#define HWIO_DPC_REG_DPCC_PHY_XO_CBCR_CLK_ENABLE_SHFT                                         0x0
#define HWIO_DPC_REG_DPCC_PHY_XO_CBCR_CLK_ENABLE_DISABLE_FVAL                                 0x0
#define HWIO_DPC_REG_DPCC_PHY_XO_CBCR_CLK_ENABLE_ENABLE_FVAL                                  0x1

#define HWIO_DPC_REG_DPCC_PHY_SLEEP_CBCR_ADDR                                          (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE      + 0x000000fc)
#define HWIO_DPC_REG_DPCC_PHY_SLEEP_CBCR_PHYS                                          (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_PHYS + 0x000000fc)
#define HWIO_DPC_REG_DPCC_PHY_SLEEP_CBCR_OFFS                                          (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_OFFS + 0x000000fc)
#define HWIO_DPC_REG_DPCC_PHY_SLEEP_CBCR_RMSK                                          0x80000005
#define HWIO_DPC_REG_DPCC_PHY_SLEEP_CBCR_IN          \
        in_dword_masked(HWIO_DPC_REG_DPCC_PHY_SLEEP_CBCR_ADDR, HWIO_DPC_REG_DPCC_PHY_SLEEP_CBCR_RMSK)
#define HWIO_DPC_REG_DPCC_PHY_SLEEP_CBCR_INM(m)      \
        in_dword_masked(HWIO_DPC_REG_DPCC_PHY_SLEEP_CBCR_ADDR, m)
#define HWIO_DPC_REG_DPCC_PHY_SLEEP_CBCR_OUT(v)      \
        out_dword(HWIO_DPC_REG_DPCC_PHY_SLEEP_CBCR_ADDR,v)
#define HWIO_DPC_REG_DPCC_PHY_SLEEP_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DPC_REG_DPCC_PHY_SLEEP_CBCR_ADDR,m,v,HWIO_DPC_REG_DPCC_PHY_SLEEP_CBCR_IN)
#define HWIO_DPC_REG_DPCC_PHY_SLEEP_CBCR_CLK_OFF_BMSK                                  0x80000000
#define HWIO_DPC_REG_DPCC_PHY_SLEEP_CBCR_CLK_OFF_SHFT                                        0x1f
#define HWIO_DPC_REG_DPCC_PHY_SLEEP_CBCR_CLK_ARES_BMSK                                        0x4
#define HWIO_DPC_REG_DPCC_PHY_SLEEP_CBCR_CLK_ARES_SHFT                                        0x2
#define HWIO_DPC_REG_DPCC_PHY_SLEEP_CBCR_CLK_ARES_NO_RESET_FVAL                               0x0
#define HWIO_DPC_REG_DPCC_PHY_SLEEP_CBCR_CLK_ARES_RESET_FVAL                                  0x1
#define HWIO_DPC_REG_DPCC_PHY_SLEEP_CBCR_CLK_ENABLE_BMSK                                      0x1
#define HWIO_DPC_REG_DPCC_PHY_SLEEP_CBCR_CLK_ENABLE_SHFT                                      0x0
#define HWIO_DPC_REG_DPCC_PHY_SLEEP_CBCR_CLK_ENABLE_DISABLE_FVAL                              0x0
#define HWIO_DPC_REG_DPCC_PHY_SLEEP_CBCR_CLK_ENABLE_ENABLE_FVAL                               0x1

#define HWIO_DPC_REG_DPCC_SHRM_BCR_ADDR                                                (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE      + 0x00000100)
#define HWIO_DPC_REG_DPCC_SHRM_BCR_PHYS                                                (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_PHYS + 0x00000100)
#define HWIO_DPC_REG_DPCC_SHRM_BCR_OFFS                                                (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_OFFS + 0x00000100)
#define HWIO_DPC_REG_DPCC_SHRM_BCR_RMSK                                                       0x1
#define HWIO_DPC_REG_DPCC_SHRM_BCR_IN          \
        in_dword_masked(HWIO_DPC_REG_DPCC_SHRM_BCR_ADDR, HWIO_DPC_REG_DPCC_SHRM_BCR_RMSK)
#define HWIO_DPC_REG_DPCC_SHRM_BCR_INM(m)      \
        in_dword_masked(HWIO_DPC_REG_DPCC_SHRM_BCR_ADDR, m)
#define HWIO_DPC_REG_DPCC_SHRM_BCR_OUT(v)      \
        out_dword(HWIO_DPC_REG_DPCC_SHRM_BCR_ADDR,v)
#define HWIO_DPC_REG_DPCC_SHRM_BCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DPC_REG_DPCC_SHRM_BCR_ADDR,m,v,HWIO_DPC_REG_DPCC_SHRM_BCR_IN)
#define HWIO_DPC_REG_DPCC_SHRM_BCR_BLK_ARES_BMSK                                              0x1
#define HWIO_DPC_REG_DPCC_SHRM_BCR_BLK_ARES_SHFT                                              0x0
#define HWIO_DPC_REG_DPCC_SHRM_BCR_BLK_ARES_DISABLE_FVAL                                      0x0
#define HWIO_DPC_REG_DPCC_SHRM_BCR_BLK_ARES_ENABLE_FVAL                                       0x1

#define HWIO_DPC_REG_DPCC_SHRM_XO_CBCR_ADDR                                            (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE      + 0x00000104)
#define HWIO_DPC_REG_DPCC_SHRM_XO_CBCR_PHYS                                            (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_PHYS + 0x00000104)
#define HWIO_DPC_REG_DPCC_SHRM_XO_CBCR_OFFS                                            (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_OFFS + 0x00000104)
#define HWIO_DPC_REG_DPCC_SHRM_XO_CBCR_RMSK                                            0x80000005
#define HWIO_DPC_REG_DPCC_SHRM_XO_CBCR_IN          \
        in_dword_masked(HWIO_DPC_REG_DPCC_SHRM_XO_CBCR_ADDR, HWIO_DPC_REG_DPCC_SHRM_XO_CBCR_RMSK)
#define HWIO_DPC_REG_DPCC_SHRM_XO_CBCR_INM(m)      \
        in_dword_masked(HWIO_DPC_REG_DPCC_SHRM_XO_CBCR_ADDR, m)
#define HWIO_DPC_REG_DPCC_SHRM_XO_CBCR_OUT(v)      \
        out_dword(HWIO_DPC_REG_DPCC_SHRM_XO_CBCR_ADDR,v)
#define HWIO_DPC_REG_DPCC_SHRM_XO_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DPC_REG_DPCC_SHRM_XO_CBCR_ADDR,m,v,HWIO_DPC_REG_DPCC_SHRM_XO_CBCR_IN)
#define HWIO_DPC_REG_DPCC_SHRM_XO_CBCR_CLK_OFF_BMSK                                    0x80000000
#define HWIO_DPC_REG_DPCC_SHRM_XO_CBCR_CLK_OFF_SHFT                                          0x1f
#define HWIO_DPC_REG_DPCC_SHRM_XO_CBCR_CLK_ARES_BMSK                                          0x4
#define HWIO_DPC_REG_DPCC_SHRM_XO_CBCR_CLK_ARES_SHFT                                          0x2
#define HWIO_DPC_REG_DPCC_SHRM_XO_CBCR_CLK_ARES_NO_RESET_FVAL                                 0x0
#define HWIO_DPC_REG_DPCC_SHRM_XO_CBCR_CLK_ARES_RESET_FVAL                                    0x1
#define HWIO_DPC_REG_DPCC_SHRM_XO_CBCR_CLK_ENABLE_BMSK                                        0x1
#define HWIO_DPC_REG_DPCC_SHRM_XO_CBCR_CLK_ENABLE_SHFT                                        0x0
#define HWIO_DPC_REG_DPCC_SHRM_XO_CBCR_CLK_ENABLE_DISABLE_FVAL                                0x0
#define HWIO_DPC_REG_DPCC_SHRM_XO_CBCR_CLK_ENABLE_ENABLE_FVAL                                 0x1

#define HWIO_DPC_REG_DPCC_SHRM_CBCR_ADDR                                               (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE      + 0x00000108)
#define HWIO_DPC_REG_DPCC_SHRM_CBCR_PHYS                                               (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_PHYS + 0x00000108)
#define HWIO_DPC_REG_DPCC_SHRM_CBCR_OFFS                                               (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_OFFS + 0x00000108)
#define HWIO_DPC_REG_DPCC_SHRM_CBCR_RMSK                                               0x80000005
#define HWIO_DPC_REG_DPCC_SHRM_CBCR_IN          \
        in_dword_masked(HWIO_DPC_REG_DPCC_SHRM_CBCR_ADDR, HWIO_DPC_REG_DPCC_SHRM_CBCR_RMSK)
#define HWIO_DPC_REG_DPCC_SHRM_CBCR_INM(m)      \
        in_dword_masked(HWIO_DPC_REG_DPCC_SHRM_CBCR_ADDR, m)
#define HWIO_DPC_REG_DPCC_SHRM_CBCR_OUT(v)      \
        out_dword(HWIO_DPC_REG_DPCC_SHRM_CBCR_ADDR,v)
#define HWIO_DPC_REG_DPCC_SHRM_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DPC_REG_DPCC_SHRM_CBCR_ADDR,m,v,HWIO_DPC_REG_DPCC_SHRM_CBCR_IN)
#define HWIO_DPC_REG_DPCC_SHRM_CBCR_CLK_OFF_BMSK                                       0x80000000
#define HWIO_DPC_REG_DPCC_SHRM_CBCR_CLK_OFF_SHFT                                             0x1f
#define HWIO_DPC_REG_DPCC_SHRM_CBCR_CLK_ARES_BMSK                                             0x4
#define HWIO_DPC_REG_DPCC_SHRM_CBCR_CLK_ARES_SHFT                                             0x2
#define HWIO_DPC_REG_DPCC_SHRM_CBCR_CLK_ARES_NO_RESET_FVAL                                    0x0
#define HWIO_DPC_REG_DPCC_SHRM_CBCR_CLK_ARES_RESET_FVAL                                       0x1
#define HWIO_DPC_REG_DPCC_SHRM_CBCR_CLK_ENABLE_BMSK                                           0x1
#define HWIO_DPC_REG_DPCC_SHRM_CBCR_CLK_ENABLE_SHFT                                           0x0
#define HWIO_DPC_REG_DPCC_SHRM_CBCR_CLK_ENABLE_DISABLE_FVAL                                   0x0
#define HWIO_DPC_REG_DPCC_SHRM_CBCR_CLK_ENABLE_ENABLE_FVAL                                    0x1

#define HWIO_DPC_REG_DPCC_MCCC_BCR_ADDR                                                (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE      + 0x0000010c)
#define HWIO_DPC_REG_DPCC_MCCC_BCR_PHYS                                                (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_PHYS + 0x0000010c)
#define HWIO_DPC_REG_DPCC_MCCC_BCR_OFFS                                                (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_OFFS + 0x0000010c)
#define HWIO_DPC_REG_DPCC_MCCC_BCR_RMSK                                                       0x1
#define HWIO_DPC_REG_DPCC_MCCC_BCR_IN          \
        in_dword_masked(HWIO_DPC_REG_DPCC_MCCC_BCR_ADDR, HWIO_DPC_REG_DPCC_MCCC_BCR_RMSK)
#define HWIO_DPC_REG_DPCC_MCCC_BCR_INM(m)      \
        in_dword_masked(HWIO_DPC_REG_DPCC_MCCC_BCR_ADDR, m)
#define HWIO_DPC_REG_DPCC_MCCC_BCR_OUT(v)      \
        out_dword(HWIO_DPC_REG_DPCC_MCCC_BCR_ADDR,v)
#define HWIO_DPC_REG_DPCC_MCCC_BCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DPC_REG_DPCC_MCCC_BCR_ADDR,m,v,HWIO_DPC_REG_DPCC_MCCC_BCR_IN)
#define HWIO_DPC_REG_DPCC_MCCC_BCR_BLK_ARES_BMSK                                              0x1
#define HWIO_DPC_REG_DPCC_MCCC_BCR_BLK_ARES_SHFT                                              0x0
#define HWIO_DPC_REG_DPCC_MCCC_BCR_BLK_ARES_DISABLE_FVAL                                      0x0
#define HWIO_DPC_REG_DPCC_MCCC_BCR_BLK_ARES_ENABLE_FVAL                                       0x1

#define HWIO_DPC_REG_DPCC_MCCC_CBCR_ADDR                                               (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE      + 0x00000110)
#define HWIO_DPC_REG_DPCC_MCCC_CBCR_PHYS                                               (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_PHYS + 0x00000110)
#define HWIO_DPC_REG_DPCC_MCCC_CBCR_OFFS                                               (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_OFFS + 0x00000110)
#define HWIO_DPC_REG_DPCC_MCCC_CBCR_RMSK                                               0x80000005
#define HWIO_DPC_REG_DPCC_MCCC_CBCR_IN          \
        in_dword_masked(HWIO_DPC_REG_DPCC_MCCC_CBCR_ADDR, HWIO_DPC_REG_DPCC_MCCC_CBCR_RMSK)
#define HWIO_DPC_REG_DPCC_MCCC_CBCR_INM(m)      \
        in_dword_masked(HWIO_DPC_REG_DPCC_MCCC_CBCR_ADDR, m)
#define HWIO_DPC_REG_DPCC_MCCC_CBCR_OUT(v)      \
        out_dword(HWIO_DPC_REG_DPCC_MCCC_CBCR_ADDR,v)
#define HWIO_DPC_REG_DPCC_MCCC_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DPC_REG_DPCC_MCCC_CBCR_ADDR,m,v,HWIO_DPC_REG_DPCC_MCCC_CBCR_IN)
#define HWIO_DPC_REG_DPCC_MCCC_CBCR_CLK_OFF_BMSK                                       0x80000000
#define HWIO_DPC_REG_DPCC_MCCC_CBCR_CLK_OFF_SHFT                                             0x1f
#define HWIO_DPC_REG_DPCC_MCCC_CBCR_CLK_ARES_BMSK                                             0x4
#define HWIO_DPC_REG_DPCC_MCCC_CBCR_CLK_ARES_SHFT                                             0x2
#define HWIO_DPC_REG_DPCC_MCCC_CBCR_CLK_ARES_NO_RESET_FVAL                                    0x0
#define HWIO_DPC_REG_DPCC_MCCC_CBCR_CLK_ARES_RESET_FVAL                                       0x1
#define HWIO_DPC_REG_DPCC_MCCC_CBCR_CLK_ENABLE_BMSK                                           0x1
#define HWIO_DPC_REG_DPCC_MCCC_CBCR_CLK_ENABLE_SHFT                                           0x0
#define HWIO_DPC_REG_DPCC_MCCC_CBCR_CLK_ENABLE_DISABLE_FVAL                                   0x0
#define HWIO_DPC_REG_DPCC_MCCC_CBCR_CLK_ENABLE_ENABLE_FVAL                                    0x1

#define HWIO_DPC_REG_DPCC_DDRSS_TOP_BCR_ADDR                                           (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE      + 0x00000114)
#define HWIO_DPC_REG_DPCC_DDRSS_TOP_BCR_PHYS                                           (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_PHYS + 0x00000114)
#define HWIO_DPC_REG_DPCC_DDRSS_TOP_BCR_OFFS                                           (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_OFFS + 0x00000114)
#define HWIO_DPC_REG_DPCC_DDRSS_TOP_BCR_RMSK                                                  0x1
#define HWIO_DPC_REG_DPCC_DDRSS_TOP_BCR_IN          \
        in_dword_masked(HWIO_DPC_REG_DPCC_DDRSS_TOP_BCR_ADDR, HWIO_DPC_REG_DPCC_DDRSS_TOP_BCR_RMSK)
#define HWIO_DPC_REG_DPCC_DDRSS_TOP_BCR_INM(m)      \
        in_dword_masked(HWIO_DPC_REG_DPCC_DDRSS_TOP_BCR_ADDR, m)
#define HWIO_DPC_REG_DPCC_DDRSS_TOP_BCR_OUT(v)      \
        out_dword(HWIO_DPC_REG_DPCC_DDRSS_TOP_BCR_ADDR,v)
#define HWIO_DPC_REG_DPCC_DDRSS_TOP_BCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DPC_REG_DPCC_DDRSS_TOP_BCR_ADDR,m,v,HWIO_DPC_REG_DPCC_DDRSS_TOP_BCR_IN)
#define HWIO_DPC_REG_DPCC_DDRSS_TOP_BCR_BLK_ARES_BMSK                                         0x1
#define HWIO_DPC_REG_DPCC_DDRSS_TOP_BCR_BLK_ARES_SHFT                                         0x0
#define HWIO_DPC_REG_DPCC_DDRSS_TOP_BCR_BLK_ARES_DISABLE_FVAL                                 0x0
#define HWIO_DPC_REG_DPCC_DDRSS_TOP_BCR_BLK_ARES_ENABLE_FVAL                                  0x1

#define HWIO_DPC_REG_DPCC_DDRSS_TOP_XO_CBCR_ADDR                                       (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE      + 0x00000118)
#define HWIO_DPC_REG_DPCC_DDRSS_TOP_XO_CBCR_PHYS                                       (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_PHYS + 0x00000118)
#define HWIO_DPC_REG_DPCC_DDRSS_TOP_XO_CBCR_OFFS                                       (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_OFFS + 0x00000118)
#define HWIO_DPC_REG_DPCC_DDRSS_TOP_XO_CBCR_RMSK                                       0x80000005
#define HWIO_DPC_REG_DPCC_DDRSS_TOP_XO_CBCR_IN          \
        in_dword_masked(HWIO_DPC_REG_DPCC_DDRSS_TOP_XO_CBCR_ADDR, HWIO_DPC_REG_DPCC_DDRSS_TOP_XO_CBCR_RMSK)
#define HWIO_DPC_REG_DPCC_DDRSS_TOP_XO_CBCR_INM(m)      \
        in_dword_masked(HWIO_DPC_REG_DPCC_DDRSS_TOP_XO_CBCR_ADDR, m)
#define HWIO_DPC_REG_DPCC_DDRSS_TOP_XO_CBCR_OUT(v)      \
        out_dword(HWIO_DPC_REG_DPCC_DDRSS_TOP_XO_CBCR_ADDR,v)
#define HWIO_DPC_REG_DPCC_DDRSS_TOP_XO_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DPC_REG_DPCC_DDRSS_TOP_XO_CBCR_ADDR,m,v,HWIO_DPC_REG_DPCC_DDRSS_TOP_XO_CBCR_IN)
#define HWIO_DPC_REG_DPCC_DDRSS_TOP_XO_CBCR_CLK_OFF_BMSK                               0x80000000
#define HWIO_DPC_REG_DPCC_DDRSS_TOP_XO_CBCR_CLK_OFF_SHFT                                     0x1f
#define HWIO_DPC_REG_DPCC_DDRSS_TOP_XO_CBCR_CLK_ARES_BMSK                                     0x4
#define HWIO_DPC_REG_DPCC_DDRSS_TOP_XO_CBCR_CLK_ARES_SHFT                                     0x2
#define HWIO_DPC_REG_DPCC_DDRSS_TOP_XO_CBCR_CLK_ARES_NO_RESET_FVAL                            0x0
#define HWIO_DPC_REG_DPCC_DDRSS_TOP_XO_CBCR_CLK_ARES_RESET_FVAL                               0x1
#define HWIO_DPC_REG_DPCC_DDRSS_TOP_XO_CBCR_CLK_ENABLE_BMSK                                   0x1
#define HWIO_DPC_REG_DPCC_DDRSS_TOP_XO_CBCR_CLK_ENABLE_SHFT                                   0x0
#define HWIO_DPC_REG_DPCC_DDRSS_TOP_XO_CBCR_CLK_ENABLE_DISABLE_FVAL                           0x0
#define HWIO_DPC_REG_DPCC_DDRSS_TOP_XO_CBCR_CLK_ENABLE_ENABLE_FVAL                            0x1

#define HWIO_DPC_REG_DPCC_TGU_XO_CBCR_ADDR                                             (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE      + 0x0000011c)
#define HWIO_DPC_REG_DPCC_TGU_XO_CBCR_PHYS                                             (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_PHYS + 0x0000011c)
#define HWIO_DPC_REG_DPCC_TGU_XO_CBCR_OFFS                                             (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_OFFS + 0x0000011c)
#define HWIO_DPC_REG_DPCC_TGU_XO_CBCR_RMSK                                             0x80000005
#define HWIO_DPC_REG_DPCC_TGU_XO_CBCR_IN          \
        in_dword_masked(HWIO_DPC_REG_DPCC_TGU_XO_CBCR_ADDR, HWIO_DPC_REG_DPCC_TGU_XO_CBCR_RMSK)
#define HWIO_DPC_REG_DPCC_TGU_XO_CBCR_INM(m)      \
        in_dword_masked(HWIO_DPC_REG_DPCC_TGU_XO_CBCR_ADDR, m)
#define HWIO_DPC_REG_DPCC_TGU_XO_CBCR_OUT(v)      \
        out_dword(HWIO_DPC_REG_DPCC_TGU_XO_CBCR_ADDR,v)
#define HWIO_DPC_REG_DPCC_TGU_XO_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DPC_REG_DPCC_TGU_XO_CBCR_ADDR,m,v,HWIO_DPC_REG_DPCC_TGU_XO_CBCR_IN)
#define HWIO_DPC_REG_DPCC_TGU_XO_CBCR_CLK_OFF_BMSK                                     0x80000000
#define HWIO_DPC_REG_DPCC_TGU_XO_CBCR_CLK_OFF_SHFT                                           0x1f
#define HWIO_DPC_REG_DPCC_TGU_XO_CBCR_CLK_ARES_BMSK                                           0x4
#define HWIO_DPC_REG_DPCC_TGU_XO_CBCR_CLK_ARES_SHFT                                           0x2
#define HWIO_DPC_REG_DPCC_TGU_XO_CBCR_CLK_ARES_NO_RESET_FVAL                                  0x0
#define HWIO_DPC_REG_DPCC_TGU_XO_CBCR_CLK_ARES_RESET_FVAL                                     0x1
#define HWIO_DPC_REG_DPCC_TGU_XO_CBCR_CLK_ENABLE_BMSK                                         0x1
#define HWIO_DPC_REG_DPCC_TGU_XO_CBCR_CLK_ENABLE_SHFT                                         0x0
#define HWIO_DPC_REG_DPCC_TGU_XO_CBCR_CLK_ENABLE_DISABLE_FVAL                                 0x0
#define HWIO_DPC_REG_DPCC_TGU_XO_CBCR_CLK_ENABLE_ENABLE_FVAL                                  0x1

#define HWIO_DPC_REG_DPCC_DDRSS_TOP_SLEEP_CBCR_ADDR                                    (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE      + 0x00000120)
#define HWIO_DPC_REG_DPCC_DDRSS_TOP_SLEEP_CBCR_PHYS                                    (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_PHYS + 0x00000120)
#define HWIO_DPC_REG_DPCC_DDRSS_TOP_SLEEP_CBCR_OFFS                                    (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_OFFS + 0x00000120)
#define HWIO_DPC_REG_DPCC_DDRSS_TOP_SLEEP_CBCR_RMSK                                    0x80000005
#define HWIO_DPC_REG_DPCC_DDRSS_TOP_SLEEP_CBCR_IN          \
        in_dword_masked(HWIO_DPC_REG_DPCC_DDRSS_TOP_SLEEP_CBCR_ADDR, HWIO_DPC_REG_DPCC_DDRSS_TOP_SLEEP_CBCR_RMSK)
#define HWIO_DPC_REG_DPCC_DDRSS_TOP_SLEEP_CBCR_INM(m)      \
        in_dword_masked(HWIO_DPC_REG_DPCC_DDRSS_TOP_SLEEP_CBCR_ADDR, m)
#define HWIO_DPC_REG_DPCC_DDRSS_TOP_SLEEP_CBCR_OUT(v)      \
        out_dword(HWIO_DPC_REG_DPCC_DDRSS_TOP_SLEEP_CBCR_ADDR,v)
#define HWIO_DPC_REG_DPCC_DDRSS_TOP_SLEEP_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DPC_REG_DPCC_DDRSS_TOP_SLEEP_CBCR_ADDR,m,v,HWIO_DPC_REG_DPCC_DDRSS_TOP_SLEEP_CBCR_IN)
#define HWIO_DPC_REG_DPCC_DDRSS_TOP_SLEEP_CBCR_CLK_OFF_BMSK                            0x80000000
#define HWIO_DPC_REG_DPCC_DDRSS_TOP_SLEEP_CBCR_CLK_OFF_SHFT                                  0x1f
#define HWIO_DPC_REG_DPCC_DDRSS_TOP_SLEEP_CBCR_CLK_ARES_BMSK                                  0x4
#define HWIO_DPC_REG_DPCC_DDRSS_TOP_SLEEP_CBCR_CLK_ARES_SHFT                                  0x2
#define HWIO_DPC_REG_DPCC_DDRSS_TOP_SLEEP_CBCR_CLK_ARES_NO_RESET_FVAL                         0x0
#define HWIO_DPC_REG_DPCC_DDRSS_TOP_SLEEP_CBCR_CLK_ARES_RESET_FVAL                            0x1
#define HWIO_DPC_REG_DPCC_DDRSS_TOP_SLEEP_CBCR_CLK_ENABLE_BMSK                                0x1
#define HWIO_DPC_REG_DPCC_DDRSS_TOP_SLEEP_CBCR_CLK_ENABLE_SHFT                                0x0
#define HWIO_DPC_REG_DPCC_DDRSS_TOP_SLEEP_CBCR_CLK_ENABLE_DISABLE_FVAL                        0x0
#define HWIO_DPC_REG_DPCC_DDRSS_TOP_SLEEP_CBCR_CLK_ENABLE_ENABLE_FVAL                         0x1

#define HWIO_DPC_REG_DPCC_DEBUG_MUX_MUXR_ADDR                                          (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE      + 0x00000000)
#define HWIO_DPC_REG_DPCC_DEBUG_MUX_MUXR_PHYS                                          (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_PHYS + 0x00000000)
#define HWIO_DPC_REG_DPCC_DEBUG_MUX_MUXR_OFFS                                          (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_OFFS + 0x00000000)
#define HWIO_DPC_REG_DPCC_DEBUG_MUX_MUXR_RMSK                                                0x3f
#define HWIO_DPC_REG_DPCC_DEBUG_MUX_MUXR_IN          \
        in_dword_masked(HWIO_DPC_REG_DPCC_DEBUG_MUX_MUXR_ADDR, HWIO_DPC_REG_DPCC_DEBUG_MUX_MUXR_RMSK)
#define HWIO_DPC_REG_DPCC_DEBUG_MUX_MUXR_INM(m)      \
        in_dword_masked(HWIO_DPC_REG_DPCC_DEBUG_MUX_MUXR_ADDR, m)
#define HWIO_DPC_REG_DPCC_DEBUG_MUX_MUXR_OUT(v)      \
        out_dword(HWIO_DPC_REG_DPCC_DEBUG_MUX_MUXR_ADDR,v)
#define HWIO_DPC_REG_DPCC_DEBUG_MUX_MUXR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DPC_REG_DPCC_DEBUG_MUX_MUXR_ADDR,m,v,HWIO_DPC_REG_DPCC_DEBUG_MUX_MUXR_IN)
#define HWIO_DPC_REG_DPCC_DEBUG_MUX_MUXR_MUX_SEL_BMSK                                        0x3f
#define HWIO_DPC_REG_DPCC_DEBUG_MUX_MUXR_MUX_SEL_SHFT                                         0x0

#define HWIO_DPC_REG_DPCC_PLL_TEST_MUX_MUXR_ADDR                                       (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE      + 0x0000000c)
#define HWIO_DPC_REG_DPCC_PLL_TEST_MUX_MUXR_PHYS                                       (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_PHYS + 0x0000000c)
#define HWIO_DPC_REG_DPCC_PLL_TEST_MUX_MUXR_OFFS                                       (DPC_REG_DPCC_DPCC_DPCC_REG_REG_BASE_OFFS + 0x0000000c)
#define HWIO_DPC_REG_DPCC_PLL_TEST_MUX_MUXR_RMSK                                             0x3f
#define HWIO_DPC_REG_DPCC_PLL_TEST_MUX_MUXR_IN          \
        in_dword_masked(HWIO_DPC_REG_DPCC_PLL_TEST_MUX_MUXR_ADDR, HWIO_DPC_REG_DPCC_PLL_TEST_MUX_MUXR_RMSK)
#define HWIO_DPC_REG_DPCC_PLL_TEST_MUX_MUXR_INM(m)      \
        in_dword_masked(HWIO_DPC_REG_DPCC_PLL_TEST_MUX_MUXR_ADDR, m)
#define HWIO_DPC_REG_DPCC_PLL_TEST_MUX_MUXR_OUT(v)      \
        out_dword(HWIO_DPC_REG_DPCC_PLL_TEST_MUX_MUXR_ADDR,v)
#define HWIO_DPC_REG_DPCC_PLL_TEST_MUX_MUXR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DPC_REG_DPCC_PLL_TEST_MUX_MUXR_ADDR,m,v,HWIO_DPC_REG_DPCC_PLL_TEST_MUX_MUXR_IN)
#define HWIO_DPC_REG_DPCC_PLL_TEST_MUX_MUXR_MUX_SEL_BMSK                                     0x3f
#define HWIO_DPC_REG_DPCC_PLL_TEST_MUX_MUXR_MUX_SEL_SHFT                                      0x0

/*----------------------------------------------------------------------------
 * MODULE: DRE_REGS
 *--------------------------------------------------------------------------*/

#define DRE_REGS_REG_BASE                                                                     (DDR_SS_BASE      + 0x00140800)
#define DRE_REGS_REG_BASE_SIZE                                                                0x800
#define DRE_REGS_REG_BASE_USED                                                                0x48c
#define DRE_REGS_REG_BASE_PHYS                                                                (DDR_SS_BASE_PHYS + 0x00140800)
#define DRE_REGS_REG_BASE_OFFS                                                                0x00140800

#define HWIO_DRE_DRE_HW_INFO_ADDR                                                             (DRE_REGS_REG_BASE      + 0x00000000)
#define HWIO_DRE_DRE_HW_INFO_PHYS                                                             (DRE_REGS_REG_BASE_PHYS + 0x00000000)
#define HWIO_DRE_DRE_HW_INFO_OFFS                                                             (DRE_REGS_REG_BASE_OFFS + 0x00000000)
#define HWIO_DRE_DRE_HW_INFO_RMSK                                                             0xffffffff
#define HWIO_DRE_DRE_HW_INFO_IN          \
        in_dword_masked(HWIO_DRE_DRE_HW_INFO_ADDR, HWIO_DRE_DRE_HW_INFO_RMSK)
#define HWIO_DRE_DRE_HW_INFO_INM(m)      \
        in_dword_masked(HWIO_DRE_DRE_HW_INFO_ADDR, m)
#define HWIO_DRE_DRE_HW_INFO_MAJOR_REVISION_BMSK                                              0xff000000
#define HWIO_DRE_DRE_HW_INFO_MAJOR_REVISION_SHFT                                                    0x18
#define HWIO_DRE_DRE_HW_INFO_BRANCH_REVISION_BMSK                                               0xff0000
#define HWIO_DRE_DRE_HW_INFO_BRANCH_REVISION_SHFT                                                   0x10
#define HWIO_DRE_DRE_HW_INFO_MINOR_REVISION_BMSK                                                  0xff00
#define HWIO_DRE_DRE_HW_INFO_MINOR_REVISION_SHFT                                                     0x8
#define HWIO_DRE_DRE_HW_INFO_ECO_REVISION_BMSK                                                      0xff
#define HWIO_DRE_DRE_HW_INFO_ECO_REVISION_SHFT                                                       0x0

#define HWIO_DRE_DRE_HW_VERSION_ADDR                                                          (DRE_REGS_REG_BASE      + 0x00000004)
#define HWIO_DRE_DRE_HW_VERSION_PHYS                                                          (DRE_REGS_REG_BASE_PHYS + 0x00000004)
#define HWIO_DRE_DRE_HW_VERSION_OFFS                                                          (DRE_REGS_REG_BASE_OFFS + 0x00000004)
#define HWIO_DRE_DRE_HW_VERSION_RMSK                                                          0xffffffff
#define HWIO_DRE_DRE_HW_VERSION_IN          \
        in_dword_masked(HWIO_DRE_DRE_HW_VERSION_ADDR, HWIO_DRE_DRE_HW_VERSION_RMSK)
#define HWIO_DRE_DRE_HW_VERSION_INM(m)      \
        in_dword_masked(HWIO_DRE_DRE_HW_VERSION_ADDR, m)
#define HWIO_DRE_DRE_HW_VERSION_MAJOR_REVISION_BMSK                                           0xff000000
#define HWIO_DRE_DRE_HW_VERSION_MAJOR_REVISION_SHFT                                                 0x18
#define HWIO_DRE_DRE_HW_VERSION_BRANCH_REVISION_BMSK                                            0xff0000
#define HWIO_DRE_DRE_HW_VERSION_BRANCH_REVISION_SHFT                                                0x10
#define HWIO_DRE_DRE_HW_VERSION_MINOR_REVISION_BMSK                                               0xff00
#define HWIO_DRE_DRE_HW_VERSION_MINOR_REVISION_SHFT                                                  0x8
#define HWIO_DRE_DRE_HW_VERSION_ECO_REVISION_BMSK                                                   0xff
#define HWIO_DRE_DRE_HW_VERSION_ECO_REVISION_SHFT                                                    0x0

#define HWIO_DRE_DRE_BYPASS_MODE_ADDR                                                         (DRE_REGS_REG_BASE      + 0x00000010)
#define HWIO_DRE_DRE_BYPASS_MODE_PHYS                                                         (DRE_REGS_REG_BASE_PHYS + 0x00000010)
#define HWIO_DRE_DRE_BYPASS_MODE_OFFS                                                         (DRE_REGS_REG_BASE_OFFS + 0x00000010)
#define HWIO_DRE_DRE_BYPASS_MODE_RMSK                                                                0x1
#define HWIO_DRE_DRE_BYPASS_MODE_IN          \
        in_dword_masked(HWIO_DRE_DRE_BYPASS_MODE_ADDR, HWIO_DRE_DRE_BYPASS_MODE_RMSK)
#define HWIO_DRE_DRE_BYPASS_MODE_INM(m)      \
        in_dword_masked(HWIO_DRE_DRE_BYPASS_MODE_ADDR, m)
#define HWIO_DRE_DRE_BYPASS_MODE_OUT(v)      \
        out_dword(HWIO_DRE_DRE_BYPASS_MODE_ADDR,v)
#define HWIO_DRE_DRE_BYPASS_MODE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DRE_DRE_BYPASS_MODE_ADDR,m,v,HWIO_DRE_DRE_BYPASS_MODE_IN)
#define HWIO_DRE_DRE_BYPASS_MODE_BYPASS_MODE_BMSK                                                    0x1
#define HWIO_DRE_DRE_BYPASS_MODE_BYPASS_MODE_SHFT                                                    0x0

#define HWIO_DRE_DRE_SYNDROME_REGION_BASE_LOWER_ADDR                                          (DRE_REGS_REG_BASE      + 0x00000020)
#define HWIO_DRE_DRE_SYNDROME_REGION_BASE_LOWER_PHYS                                          (DRE_REGS_REG_BASE_PHYS + 0x00000020)
#define HWIO_DRE_DRE_SYNDROME_REGION_BASE_LOWER_OFFS                                          (DRE_REGS_REG_BASE_OFFS + 0x00000020)
#define HWIO_DRE_DRE_SYNDROME_REGION_BASE_LOWER_RMSK                                          0xfffff000
#define HWIO_DRE_DRE_SYNDROME_REGION_BASE_LOWER_IN          \
        in_dword_masked(HWIO_DRE_DRE_SYNDROME_REGION_BASE_LOWER_ADDR, HWIO_DRE_DRE_SYNDROME_REGION_BASE_LOWER_RMSK)
#define HWIO_DRE_DRE_SYNDROME_REGION_BASE_LOWER_INM(m)      \
        in_dword_masked(HWIO_DRE_DRE_SYNDROME_REGION_BASE_LOWER_ADDR, m)
#define HWIO_DRE_DRE_SYNDROME_REGION_BASE_LOWER_OUT(v)      \
        out_dword(HWIO_DRE_DRE_SYNDROME_REGION_BASE_LOWER_ADDR,v)
#define HWIO_DRE_DRE_SYNDROME_REGION_BASE_LOWER_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DRE_DRE_SYNDROME_REGION_BASE_LOWER_ADDR,m,v,HWIO_DRE_DRE_SYNDROME_REGION_BASE_LOWER_IN)
#define HWIO_DRE_DRE_SYNDROME_REGION_BASE_LOWER_BASE_31_12_BMSK                               0xfffff000
#define HWIO_DRE_DRE_SYNDROME_REGION_BASE_LOWER_BASE_31_12_SHFT                                      0xc

#define HWIO_DRE_DRE_SYNDROME_REGION_BASE_UPPER_ADDR                                          (DRE_REGS_REG_BASE      + 0x00000024)
#define HWIO_DRE_DRE_SYNDROME_REGION_BASE_UPPER_PHYS                                          (DRE_REGS_REG_BASE_PHYS + 0x00000024)
#define HWIO_DRE_DRE_SYNDROME_REGION_BASE_UPPER_OFFS                                          (DRE_REGS_REG_BASE_OFFS + 0x00000024)
#define HWIO_DRE_DRE_SYNDROME_REGION_BASE_UPPER_RMSK                                                 0xf
#define HWIO_DRE_DRE_SYNDROME_REGION_BASE_UPPER_IN          \
        in_dword_masked(HWIO_DRE_DRE_SYNDROME_REGION_BASE_UPPER_ADDR, HWIO_DRE_DRE_SYNDROME_REGION_BASE_UPPER_RMSK)
#define HWIO_DRE_DRE_SYNDROME_REGION_BASE_UPPER_INM(m)      \
        in_dword_masked(HWIO_DRE_DRE_SYNDROME_REGION_BASE_UPPER_ADDR, m)
#define HWIO_DRE_DRE_SYNDROME_REGION_BASE_UPPER_OUT(v)      \
        out_dword(HWIO_DRE_DRE_SYNDROME_REGION_BASE_UPPER_ADDR,v)
#define HWIO_DRE_DRE_SYNDROME_REGION_BASE_UPPER_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DRE_DRE_SYNDROME_REGION_BASE_UPPER_ADDR,m,v,HWIO_DRE_DRE_SYNDROME_REGION_BASE_UPPER_IN)
#define HWIO_DRE_DRE_SYNDROME_REGION_BASE_UPPER_BASE_35_32_BMSK                                      0xf
#define HWIO_DRE_DRE_SYNDROME_REGION_BASE_UPPER_BASE_35_32_SHFT                                      0x0

#define HWIO_DRE_DRE_SYNDROME_ADDR_MASK_LOWER_ADDR                                            (DRE_REGS_REG_BASE      + 0x00000028)
#define HWIO_DRE_DRE_SYNDROME_ADDR_MASK_LOWER_PHYS                                            (DRE_REGS_REG_BASE_PHYS + 0x00000028)
#define HWIO_DRE_DRE_SYNDROME_ADDR_MASK_LOWER_OFFS                                            (DRE_REGS_REG_BASE_OFFS + 0x00000028)
#define HWIO_DRE_DRE_SYNDROME_ADDR_MASK_LOWER_RMSK                                            0xfffff000
#define HWIO_DRE_DRE_SYNDROME_ADDR_MASK_LOWER_IN          \
        in_dword_masked(HWIO_DRE_DRE_SYNDROME_ADDR_MASK_LOWER_ADDR, HWIO_DRE_DRE_SYNDROME_ADDR_MASK_LOWER_RMSK)
#define HWIO_DRE_DRE_SYNDROME_ADDR_MASK_LOWER_INM(m)      \
        in_dword_masked(HWIO_DRE_DRE_SYNDROME_ADDR_MASK_LOWER_ADDR, m)
#define HWIO_DRE_DRE_SYNDROME_ADDR_MASK_LOWER_OUT(v)      \
        out_dword(HWIO_DRE_DRE_SYNDROME_ADDR_MASK_LOWER_ADDR,v)
#define HWIO_DRE_DRE_SYNDROME_ADDR_MASK_LOWER_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DRE_DRE_SYNDROME_ADDR_MASK_LOWER_ADDR,m,v,HWIO_DRE_DRE_SYNDROME_ADDR_MASK_LOWER_IN)
#define HWIO_DRE_DRE_SYNDROME_ADDR_MASK_LOWER_MASK_31_12_BMSK                                 0xfffff000
#define HWIO_DRE_DRE_SYNDROME_ADDR_MASK_LOWER_MASK_31_12_SHFT                                        0xc

#define HWIO_DRE_DRE_SYNDROME_ADDR_MASK_UPPER_ADDR                                            (DRE_REGS_REG_BASE      + 0x0000002c)
#define HWIO_DRE_DRE_SYNDROME_ADDR_MASK_UPPER_PHYS                                            (DRE_REGS_REG_BASE_PHYS + 0x0000002c)
#define HWIO_DRE_DRE_SYNDROME_ADDR_MASK_UPPER_OFFS                                            (DRE_REGS_REG_BASE_OFFS + 0x0000002c)
#define HWIO_DRE_DRE_SYNDROME_ADDR_MASK_UPPER_RMSK                                                   0xf
#define HWIO_DRE_DRE_SYNDROME_ADDR_MASK_UPPER_IN          \
        in_dword_masked(HWIO_DRE_DRE_SYNDROME_ADDR_MASK_UPPER_ADDR, HWIO_DRE_DRE_SYNDROME_ADDR_MASK_UPPER_RMSK)
#define HWIO_DRE_DRE_SYNDROME_ADDR_MASK_UPPER_INM(m)      \
        in_dword_masked(HWIO_DRE_DRE_SYNDROME_ADDR_MASK_UPPER_ADDR, m)
#define HWIO_DRE_DRE_SYNDROME_ADDR_MASK_UPPER_OUT(v)      \
        out_dword(HWIO_DRE_DRE_SYNDROME_ADDR_MASK_UPPER_ADDR,v)
#define HWIO_DRE_DRE_SYNDROME_ADDR_MASK_UPPER_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DRE_DRE_SYNDROME_ADDR_MASK_UPPER_ADDR,m,v,HWIO_DRE_DRE_SYNDROME_ADDR_MASK_UPPER_IN)
#define HWIO_DRE_DRE_SYNDROME_ADDR_MASK_UPPER_MASK_35_32_BMSK                                        0xf
#define HWIO_DRE_DRE_SYNDROME_ADDR_MASK_UPPER_MASK_35_32_SHFT                                        0x0

#define HWIO_DRE_DRE_ARB_PRIORITY_EN_ADDR                                                     (DRE_REGS_REG_BASE      + 0x00000030)
#define HWIO_DRE_DRE_ARB_PRIORITY_EN_PHYS                                                     (DRE_REGS_REG_BASE_PHYS + 0x00000030)
#define HWIO_DRE_DRE_ARB_PRIORITY_EN_OFFS                                                     (DRE_REGS_REG_BASE_OFFS + 0x00000030)
#define HWIO_DRE_DRE_ARB_PRIORITY_EN_RMSK                                                        0x10101
#define HWIO_DRE_DRE_ARB_PRIORITY_EN_IN          \
        in_dword_masked(HWIO_DRE_DRE_ARB_PRIORITY_EN_ADDR, HWIO_DRE_DRE_ARB_PRIORITY_EN_RMSK)
#define HWIO_DRE_DRE_ARB_PRIORITY_EN_INM(m)      \
        in_dword_masked(HWIO_DRE_DRE_ARB_PRIORITY_EN_ADDR, m)
#define HWIO_DRE_DRE_ARB_PRIORITY_EN_OUT(v)      \
        out_dword(HWIO_DRE_DRE_ARB_PRIORITY_EN_ADDR,v)
#define HWIO_DRE_DRE_ARB_PRIORITY_EN_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DRE_DRE_ARB_PRIORITY_EN_ADDR,m,v,HWIO_DRE_DRE_ARB_PRIORITY_EN_IN)
#define HWIO_DRE_DRE_ARB_PRIORITY_EN_SYNDROME_RD_BUFFER_ARB_PRIORITY_EN_BMSK                     0x10000
#define HWIO_DRE_DRE_ARB_PRIORITY_EN_SYNDROME_RD_BUFFER_ARB_PRIORITY_EN_SHFT                        0x10
#define HWIO_DRE_DRE_ARB_PRIORITY_EN_RCH_ARB_PRIORITY_EN_BMSK                                      0x100
#define HWIO_DRE_DRE_ARB_PRIORITY_EN_RCH_ARB_PRIORITY_EN_SHFT                                        0x8
#define HWIO_DRE_DRE_ARB_PRIORITY_EN_CCU_ARB_PRIORITY_EN_BMSK                                        0x1
#define HWIO_DRE_DRE_ARB_PRIORITY_EN_CCU_ARB_PRIORITY_EN_SHFT                                        0x0

#define HWIO_DRE_DRE_PRIORITYLVL_EN_ADDR                                                      (DRE_REGS_REG_BASE      + 0x00000034)
#define HWIO_DRE_DRE_PRIORITYLVL_EN_PHYS                                                      (DRE_REGS_REG_BASE_PHYS + 0x00000034)
#define HWIO_DRE_DRE_PRIORITYLVL_EN_OFFS                                                      (DRE_REGS_REG_BASE_OFFS + 0x00000034)
#define HWIO_DRE_DRE_PRIORITYLVL_EN_RMSK                                                             0x1
#define HWIO_DRE_DRE_PRIORITYLVL_EN_IN          \
        in_dword_masked(HWIO_DRE_DRE_PRIORITYLVL_EN_ADDR, HWIO_DRE_DRE_PRIORITYLVL_EN_RMSK)
#define HWIO_DRE_DRE_PRIORITYLVL_EN_INM(m)      \
        in_dword_masked(HWIO_DRE_DRE_PRIORITYLVL_EN_ADDR, m)
#define HWIO_DRE_DRE_PRIORITYLVL_EN_OUT(v)      \
        out_dword(HWIO_DRE_DRE_PRIORITYLVL_EN_ADDR,v)
#define HWIO_DRE_DRE_PRIORITYLVL_EN_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DRE_DRE_PRIORITYLVL_EN_ADDR,m,v,HWIO_DRE_DRE_PRIORITYLVL_EN_IN)
#define HWIO_DRE_DRE_PRIORITYLVL_EN_CMD_BUF_TO_SYND_PL_EN_BMSK                                       0x1
#define HWIO_DRE_DRE_PRIORITYLVL_EN_CMD_BUF_TO_SYND_PL_EN_SHFT                                       0x0

#define HWIO_DRE_DRE_QSB_INTERRUPT_STATUS_ADDR                                                (DRE_REGS_REG_BASE      + 0x00000040)
#define HWIO_DRE_DRE_QSB_INTERRUPT_STATUS_PHYS                                                (DRE_REGS_REG_BASE_PHYS + 0x00000040)
#define HWIO_DRE_DRE_QSB_INTERRUPT_STATUS_OFFS                                                (DRE_REGS_REG_BASE_OFFS + 0x00000040)
#define HWIO_DRE_DRE_QSB_INTERRUPT_STATUS_RMSK                                                    0x1f07
#define HWIO_DRE_DRE_QSB_INTERRUPT_STATUS_IN          \
        in_dword_masked(HWIO_DRE_DRE_QSB_INTERRUPT_STATUS_ADDR, HWIO_DRE_DRE_QSB_INTERRUPT_STATUS_RMSK)
#define HWIO_DRE_DRE_QSB_INTERRUPT_STATUS_INM(m)      \
        in_dword_masked(HWIO_DRE_DRE_QSB_INTERRUPT_STATUS_ADDR, m)
#define HWIO_DRE_DRE_QSB_INTERRUPT_STATUS_BESP_QSB_ERR_BMSK                                       0x1f00
#define HWIO_DRE_DRE_QSB_INTERRUPT_STATUS_BESP_QSB_ERR_SHFT                                          0x8
#define HWIO_DRE_DRE_QSB_INTERRUPT_STATUS_BEDP_QSB_ERR_BMSK                                          0x7
#define HWIO_DRE_DRE_QSB_INTERRUPT_STATUS_BEDP_QSB_ERR_SHFT                                          0x0

#define HWIO_DRE_DRE_QSB_INTERRUPT_CLR_ADDR                                                   (DRE_REGS_REG_BASE      + 0x00000044)
#define HWIO_DRE_DRE_QSB_INTERRUPT_CLR_PHYS                                                   (DRE_REGS_REG_BASE_PHYS + 0x00000044)
#define HWIO_DRE_DRE_QSB_INTERRUPT_CLR_OFFS                                                   (DRE_REGS_REG_BASE_OFFS + 0x00000044)
#define HWIO_DRE_DRE_QSB_INTERRUPT_CLR_RMSK                                                       0x1f07
#define HWIO_DRE_DRE_QSB_INTERRUPT_CLR_OUT(v)      \
        out_dword(HWIO_DRE_DRE_QSB_INTERRUPT_CLR_ADDR,v)
#define HWIO_DRE_DRE_QSB_INTERRUPT_CLR_BESP_QSB_ERR_BMSK                                          0x1f00
#define HWIO_DRE_DRE_QSB_INTERRUPT_CLR_BESP_QSB_ERR_SHFT                                             0x8
#define HWIO_DRE_DRE_QSB_INTERRUPT_CLR_BEDP_QSB_ERR_BMSK                                             0x7
#define HWIO_DRE_DRE_QSB_INTERRUPT_CLR_BEDP_QSB_ERR_SHFT                                             0x0

#define HWIO_DRE_DRE_QSB_INTERRUPT_ENABLE_ADDR                                                (DRE_REGS_REG_BASE      + 0x00000048)
#define HWIO_DRE_DRE_QSB_INTERRUPT_ENABLE_PHYS                                                (DRE_REGS_REG_BASE_PHYS + 0x00000048)
#define HWIO_DRE_DRE_QSB_INTERRUPT_ENABLE_OFFS                                                (DRE_REGS_REG_BASE_OFFS + 0x00000048)
#define HWIO_DRE_DRE_QSB_INTERRUPT_ENABLE_RMSK                                                    0x1f07
#define HWIO_DRE_DRE_QSB_INTERRUPT_ENABLE_IN          \
        in_dword_masked(HWIO_DRE_DRE_QSB_INTERRUPT_ENABLE_ADDR, HWIO_DRE_DRE_QSB_INTERRUPT_ENABLE_RMSK)
#define HWIO_DRE_DRE_QSB_INTERRUPT_ENABLE_INM(m)      \
        in_dword_masked(HWIO_DRE_DRE_QSB_INTERRUPT_ENABLE_ADDR, m)
#define HWIO_DRE_DRE_QSB_INTERRUPT_ENABLE_OUT(v)      \
        out_dword(HWIO_DRE_DRE_QSB_INTERRUPT_ENABLE_ADDR,v)
#define HWIO_DRE_DRE_QSB_INTERRUPT_ENABLE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DRE_DRE_QSB_INTERRUPT_ENABLE_ADDR,m,v,HWIO_DRE_DRE_QSB_INTERRUPT_ENABLE_IN)
#define HWIO_DRE_DRE_QSB_INTERRUPT_ENABLE_BESP_QSB_ERR_BMSK                                       0x1f00
#define HWIO_DRE_DRE_QSB_INTERRUPT_ENABLE_BESP_QSB_ERR_SHFT                                          0x8
#define HWIO_DRE_DRE_QSB_INTERRUPT_ENABLE_BEDP_QSB_ERR_BMSK                                          0x7
#define HWIO_DRE_DRE_QSB_INTERRUPT_ENABLE_BEDP_QSB_ERR_SHFT                                          0x0

#define HWIO_DRE_DRE_INT_BEDP_QSB_BRESP_TX_ID_ADDR                                            (DRE_REGS_REG_BASE      + 0x00000050)
#define HWIO_DRE_DRE_INT_BEDP_QSB_BRESP_TX_ID_PHYS                                            (DRE_REGS_REG_BASE_PHYS + 0x00000050)
#define HWIO_DRE_DRE_INT_BEDP_QSB_BRESP_TX_ID_OFFS                                            (DRE_REGS_REG_BASE_OFFS + 0x00000050)
#define HWIO_DRE_DRE_INT_BEDP_QSB_BRESP_TX_ID_RMSK                                            0xffffffff
#define HWIO_DRE_DRE_INT_BEDP_QSB_BRESP_TX_ID_IN          \
        in_dword_masked(HWIO_DRE_DRE_INT_BEDP_QSB_BRESP_TX_ID_ADDR, HWIO_DRE_DRE_INT_BEDP_QSB_BRESP_TX_ID_RMSK)
#define HWIO_DRE_DRE_INT_BEDP_QSB_BRESP_TX_ID_INM(m)      \
        in_dword_masked(HWIO_DRE_DRE_INT_BEDP_QSB_BRESP_TX_ID_ADDR, m)
#define HWIO_DRE_DRE_INT_BEDP_QSB_BRESP_TX_ID_ID_BMSK                                         0xffffffff
#define HWIO_DRE_DRE_INT_BEDP_QSB_BRESP_TX_ID_ID_SHFT                                                0x0

#define HWIO_DRE_DRE_INT_BESP_QSB_BRESP_TX_ID_ADDR                                            (DRE_REGS_REG_BASE      + 0x00000054)
#define HWIO_DRE_DRE_INT_BESP_QSB_BRESP_TX_ID_PHYS                                            (DRE_REGS_REG_BASE_PHYS + 0x00000054)
#define HWIO_DRE_DRE_INT_BESP_QSB_BRESP_TX_ID_OFFS                                            (DRE_REGS_REG_BASE_OFFS + 0x00000054)
#define HWIO_DRE_DRE_INT_BESP_QSB_BRESP_TX_ID_RMSK                                            0xffffffff
#define HWIO_DRE_DRE_INT_BESP_QSB_BRESP_TX_ID_IN          \
        in_dword_masked(HWIO_DRE_DRE_INT_BESP_QSB_BRESP_TX_ID_ADDR, HWIO_DRE_DRE_INT_BESP_QSB_BRESP_TX_ID_RMSK)
#define HWIO_DRE_DRE_INT_BESP_QSB_BRESP_TX_ID_INM(m)      \
        in_dword_masked(HWIO_DRE_DRE_INT_BESP_QSB_BRESP_TX_ID_ADDR, m)
#define HWIO_DRE_DRE_INT_BESP_QSB_BRESP_TX_ID_ID_BMSK                                         0xffffffff
#define HWIO_DRE_DRE_INT_BESP_QSB_BRESP_TX_ID_ID_SHFT                                                0x0

#define HWIO_DRE_DRE_INT_BESP_QSB_RRESP_TX_ID_ADDR                                            (DRE_REGS_REG_BASE      + 0x00000058)
#define HWIO_DRE_DRE_INT_BESP_QSB_RRESP_TX_ID_PHYS                                            (DRE_REGS_REG_BASE_PHYS + 0x00000058)
#define HWIO_DRE_DRE_INT_BESP_QSB_RRESP_TX_ID_OFFS                                            (DRE_REGS_REG_BASE_OFFS + 0x00000058)
#define HWIO_DRE_DRE_INT_BESP_QSB_RRESP_TX_ID_RMSK                                            0xffffffff
#define HWIO_DRE_DRE_INT_BESP_QSB_RRESP_TX_ID_IN          \
        in_dword_masked(HWIO_DRE_DRE_INT_BESP_QSB_RRESP_TX_ID_ADDR, HWIO_DRE_DRE_INT_BESP_QSB_RRESP_TX_ID_RMSK)
#define HWIO_DRE_DRE_INT_BESP_QSB_RRESP_TX_ID_INM(m)      \
        in_dword_masked(HWIO_DRE_DRE_INT_BESP_QSB_RRESP_TX_ID_ADDR, m)
#define HWIO_DRE_DRE_INT_BESP_QSB_RRESP_TX_ID_ID_BMSK                                         0xffffffff
#define HWIO_DRE_DRE_INT_BESP_QSB_RRESP_TX_ID_ID_SHFT                                                0x0

#define HWIO_DRE_DRE_SYNDROME_CFLUSH_CTRL_0_ADDR                                              (DRE_REGS_REG_BASE      + 0x00000060)
#define HWIO_DRE_DRE_SYNDROME_CFLUSH_CTRL_0_PHYS                                              (DRE_REGS_REG_BASE_PHYS + 0x00000060)
#define HWIO_DRE_DRE_SYNDROME_CFLUSH_CTRL_0_OFFS                                              (DRE_REGS_REG_BASE_OFFS + 0x00000060)
#define HWIO_DRE_DRE_SYNDROME_CFLUSH_CTRL_0_RMSK                                                     0x1
#define HWIO_DRE_DRE_SYNDROME_CFLUSH_CTRL_0_OUT(v)      \
        out_dword(HWIO_DRE_DRE_SYNDROME_CFLUSH_CTRL_0_ADDR,v)
#define HWIO_DRE_DRE_SYNDROME_CFLUSH_CTRL_0_TRIG_BMSK                                                0x1
#define HWIO_DRE_DRE_SYNDROME_CFLUSH_CTRL_0_TRIG_SHFT                                                0x0

#define HWIO_DRE_DRE_SYNDROME_CFLUSH_CTRL_1_ADDR                                              (DRE_REGS_REG_BASE      + 0x00000064)
#define HWIO_DRE_DRE_SYNDROME_CFLUSH_CTRL_1_PHYS                                              (DRE_REGS_REG_BASE_PHYS + 0x00000064)
#define HWIO_DRE_DRE_SYNDROME_CFLUSH_CTRL_1_OFFS                                              (DRE_REGS_REG_BASE_OFFS + 0x00000064)
#define HWIO_DRE_DRE_SYNDROME_CFLUSH_CTRL_1_RMSK                                                     0x1
#define HWIO_DRE_DRE_SYNDROME_CFLUSH_CTRL_1_IN          \
        in_dword_masked(HWIO_DRE_DRE_SYNDROME_CFLUSH_CTRL_1_ADDR, HWIO_DRE_DRE_SYNDROME_CFLUSH_CTRL_1_RMSK)
#define HWIO_DRE_DRE_SYNDROME_CFLUSH_CTRL_1_INM(m)      \
        in_dword_masked(HWIO_DRE_DRE_SYNDROME_CFLUSH_CTRL_1_ADDR, m)
#define HWIO_DRE_DRE_SYNDROME_CFLUSH_CTRL_1_OUT(v)      \
        out_dword(HWIO_DRE_DRE_SYNDROME_CFLUSH_CTRL_1_ADDR,v)
#define HWIO_DRE_DRE_SYNDROME_CFLUSH_CTRL_1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DRE_DRE_SYNDROME_CFLUSH_CTRL_1_ADDR,m,v,HWIO_DRE_DRE_SYNDROME_CFLUSH_CTRL_1_IN)
#define HWIO_DRE_DRE_SYNDROME_CFLUSH_CTRL_1_OP_CLEAN_BMSK                                            0x1
#define HWIO_DRE_DRE_SYNDROME_CFLUSH_CTRL_1_OP_CLEAN_SHFT                                            0x0

#define HWIO_DRE_DRE_SYNDROME_CFLUSH_STATUS_ADDR                                              (DRE_REGS_REG_BASE      + 0x00000068)
#define HWIO_DRE_DRE_SYNDROME_CFLUSH_STATUS_PHYS                                              (DRE_REGS_REG_BASE_PHYS + 0x00000068)
#define HWIO_DRE_DRE_SYNDROME_CFLUSH_STATUS_OFFS                                              (DRE_REGS_REG_BASE_OFFS + 0x00000068)
#define HWIO_DRE_DRE_SYNDROME_CFLUSH_STATUS_RMSK                                                   0x107
#define HWIO_DRE_DRE_SYNDROME_CFLUSH_STATUS_IN          \
        in_dword_masked(HWIO_DRE_DRE_SYNDROME_CFLUSH_STATUS_ADDR, HWIO_DRE_DRE_SYNDROME_CFLUSH_STATUS_RMSK)
#define HWIO_DRE_DRE_SYNDROME_CFLUSH_STATUS_INM(m)      \
        in_dword_masked(HWIO_DRE_DRE_SYNDROME_CFLUSH_STATUS_ADDR, m)
#define HWIO_DRE_DRE_SYNDROME_CFLUSH_STATUS_DIRTY_BMSK                                             0x100
#define HWIO_DRE_DRE_SYNDROME_CFLUSH_STATUS_DIRTY_SHFT                                               0x8
#define HWIO_DRE_DRE_SYNDROME_CFLUSH_STATUS_WAIT_BMSK                                                0x4
#define HWIO_DRE_DRE_SYNDROME_CFLUSH_STATUS_WAIT_SHFT                                                0x2
#define HWIO_DRE_DRE_SYNDROME_CFLUSH_STATUS_BUSY_BMSK                                                0x2
#define HWIO_DRE_DRE_SYNDROME_CFLUSH_STATUS_BUSY_SHFT                                                0x1
#define HWIO_DRE_DRE_SYNDROME_CFLUSH_STATUS_DONE_BMSK                                                0x1
#define HWIO_DRE_DRE_SYNDROME_CFLUSH_STATUS_DONE_SHFT                                                0x0

#define HWIO_DRE_DRE_LOAD_CONFIG_ADDR                                                         (DRE_REGS_REG_BASE      + 0x00000070)
#define HWIO_DRE_DRE_LOAD_CONFIG_PHYS                                                         (DRE_REGS_REG_BASE_PHYS + 0x00000070)
#define HWIO_DRE_DRE_LOAD_CONFIG_OFFS                                                         (DRE_REGS_REG_BASE_OFFS + 0x00000070)
#define HWIO_DRE_DRE_LOAD_CONFIG_RMSK                                                                0x1
#define HWIO_DRE_DRE_LOAD_CONFIG_OUT(v)      \
        out_dword(HWIO_DRE_DRE_LOAD_CONFIG_ADDR,v)
#define HWIO_DRE_DRE_LOAD_CONFIG_LOAD_CONFIG_BMSK                                                    0x1
#define HWIO_DRE_DRE_LOAD_CONFIG_LOAD_CONFIG_SHFT                                                    0x0

#define HWIO_DRE_DRE_AHB_QACTIVE_CTRL_ADDR                                                    (DRE_REGS_REG_BASE      + 0x00000080)
#define HWIO_DRE_DRE_AHB_QACTIVE_CTRL_PHYS                                                    (DRE_REGS_REG_BASE_PHYS + 0x00000080)
#define HWIO_DRE_DRE_AHB_QACTIVE_CTRL_OFFS                                                    (DRE_REGS_REG_BASE_OFFS + 0x00000080)
#define HWIO_DRE_DRE_AHB_QACTIVE_CTRL_RMSK                                                         0xff0
#define HWIO_DRE_DRE_AHB_QACTIVE_CTRL_IN          \
        in_dword_masked(HWIO_DRE_DRE_AHB_QACTIVE_CTRL_ADDR, HWIO_DRE_DRE_AHB_QACTIVE_CTRL_RMSK)
#define HWIO_DRE_DRE_AHB_QACTIVE_CTRL_INM(m)      \
        in_dword_masked(HWIO_DRE_DRE_AHB_QACTIVE_CTRL_ADDR, m)
#define HWIO_DRE_DRE_AHB_QACTIVE_CTRL_OUT(v)      \
        out_dword(HWIO_DRE_DRE_AHB_QACTIVE_CTRL_ADDR,v)
#define HWIO_DRE_DRE_AHB_QACTIVE_CTRL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DRE_DRE_AHB_QACTIVE_CTRL_ADDR,m,v,HWIO_DRE_DRE_AHB_QACTIVE_CTRL_IN)
#define HWIO_DRE_DRE_AHB_QACTIVE_CTRL_AHB_QACTIVE_HYSTERESIS_BMSK                                  0xff0
#define HWIO_DRE_DRE_AHB_QACTIVE_CTRL_AHB_QACTIVE_HYSTERESIS_SHFT                                    0x4

#define HWIO_DRE_DRE_RCG_CTRL_ADDR                                                            (DRE_REGS_REG_BASE      + 0x00000084)
#define HWIO_DRE_DRE_RCG_CTRL_PHYS                                                            (DRE_REGS_REG_BASE_PHYS + 0x00000084)
#define HWIO_DRE_DRE_RCG_CTRL_OFFS                                                            (DRE_REGS_REG_BASE_OFFS + 0x00000084)
#define HWIO_DRE_DRE_RCG_CTRL_RMSK                                                            0x8fff3333
#define HWIO_DRE_DRE_RCG_CTRL_IN          \
        in_dword_masked(HWIO_DRE_DRE_RCG_CTRL_ADDR, HWIO_DRE_DRE_RCG_CTRL_RMSK)
#define HWIO_DRE_DRE_RCG_CTRL_INM(m)      \
        in_dword_masked(HWIO_DRE_DRE_RCG_CTRL_ADDR, m)
#define HWIO_DRE_DRE_RCG_CTRL_OUT(v)      \
        out_dword(HWIO_DRE_DRE_RCG_CTRL_ADDR,v)
#define HWIO_DRE_DRE_RCG_CTRL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DRE_DRE_RCG_CTRL_ADDR,m,v,HWIO_DRE_DRE_RCG_CTRL_IN)
#define HWIO_DRE_DRE_RCG_CTRL_ENABLE_RCG_FSM_BMSK                                             0x80000000
#define HWIO_DRE_DRE_RCG_CTRL_ENABLE_RCG_FSM_SHFT                                                   0x1f
#define HWIO_DRE_DRE_RCG_CTRL_RCG_DECISION_TIMER_VALUE_BMSK                                    0xf000000
#define HWIO_DRE_DRE_RCG_CTRL_RCG_DECISION_TIMER_VALUE_SHFT                                         0x18
#define HWIO_DRE_DRE_RCG_CTRL_RCG_QACTIVE_HYSTERESIS_BMSK                                       0xff0000
#define HWIO_DRE_DRE_RCG_CTRL_RCG_QACTIVE_HYSTERESIS_SHFT                                           0x10
#define HWIO_DRE_DRE_RCG_CTRL_RCG_OVERRIDE_QACTIVE_BMSK                                           0x2000
#define HWIO_DRE_DRE_RCG_CTRL_RCG_OVERRIDE_QACTIVE_SHFT                                              0xd
#define HWIO_DRE_DRE_RCG_CTRL_RCG_QACTIVE_VALUE_BMSK                                              0x1000
#define HWIO_DRE_DRE_RCG_CTRL_RCG_QACTIVE_VALUE_SHFT                                                 0xc
#define HWIO_DRE_DRE_RCG_CTRL_RCG_OVERRIDE_QACCEPT_BMSK                                            0x200
#define HWIO_DRE_DRE_RCG_CTRL_RCG_OVERRIDE_QACCEPT_SHFT                                              0x9
#define HWIO_DRE_DRE_RCG_CTRL_RCG_QACCEPT_N_VALUE_BMSK                                             0x100
#define HWIO_DRE_DRE_RCG_CTRL_RCG_QACCEPT_N_VALUE_SHFT                                               0x8
#define HWIO_DRE_DRE_RCG_CTRL_RCG_OVERRIDE_QDENY_BMSK                                               0x20
#define HWIO_DRE_DRE_RCG_CTRL_RCG_OVERRIDE_QDENY_SHFT                                                0x5
#define HWIO_DRE_DRE_RCG_CTRL_RCG_QDENY_VALUE_BMSK                                                  0x10
#define HWIO_DRE_DRE_RCG_CTRL_RCG_QDENY_VALUE_SHFT                                                   0x4
#define HWIO_DRE_DRE_RCG_CTRL_RCG_OVERRIDE_FENCE_EN_BMSK                                             0x2
#define HWIO_DRE_DRE_RCG_CTRL_RCG_OVERRIDE_FENCE_EN_SHFT                                             0x1
#define HWIO_DRE_DRE_RCG_CTRL_RCG_FENCE_EN_VALUE_BMSK                                                0x1
#define HWIO_DRE_DRE_RCG_CTRL_RCG_FENCE_EN_VALUE_SHFT                                                0x0

#define HWIO_DRE_DRE_RCG_IDLE_ADDR                                                            (DRE_REGS_REG_BASE      + 0x00000088)
#define HWIO_DRE_DRE_RCG_IDLE_PHYS                                                            (DRE_REGS_REG_BASE_PHYS + 0x00000088)
#define HWIO_DRE_DRE_RCG_IDLE_OFFS                                                            (DRE_REGS_REG_BASE_OFFS + 0x00000088)
#define HWIO_DRE_DRE_RCG_IDLE_RMSK                                                                   0x1
#define HWIO_DRE_DRE_RCG_IDLE_IN          \
        in_dword_masked(HWIO_DRE_DRE_RCG_IDLE_ADDR, HWIO_DRE_DRE_RCG_IDLE_RMSK)
#define HWIO_DRE_DRE_RCG_IDLE_INM(m)      \
        in_dword_masked(HWIO_DRE_DRE_RCG_IDLE_ADDR, m)
#define HWIO_DRE_DRE_RCG_IDLE_DRE_IDLE_BMSK                                                          0x1
#define HWIO_DRE_DRE_RCG_IDLE_DRE_IDLE_SHFT                                                          0x0

#define HWIO_DRE_DRE_RCG_RCG_STATUS_ADDR                                                      (DRE_REGS_REG_BASE      + 0x0000008c)
#define HWIO_DRE_DRE_RCG_RCG_STATUS_PHYS                                                      (DRE_REGS_REG_BASE_PHYS + 0x0000008c)
#define HWIO_DRE_DRE_RCG_RCG_STATUS_OFFS                                                      (DRE_REGS_REG_BASE_OFFS + 0x0000008c)
#define HWIO_DRE_DRE_RCG_RCG_STATUS_RMSK                                                             0x7
#define HWIO_DRE_DRE_RCG_RCG_STATUS_IN          \
        in_dword_masked(HWIO_DRE_DRE_RCG_RCG_STATUS_ADDR, HWIO_DRE_DRE_RCG_RCG_STATUS_RMSK)
#define HWIO_DRE_DRE_RCG_RCG_STATUS_INM(m)      \
        in_dword_masked(HWIO_DRE_DRE_RCG_RCG_STATUS_ADDR, m)
#define HWIO_DRE_DRE_RCG_RCG_STATUS_RCG_FSM_QSTATE_BMSK                                              0x6
#define HWIO_DRE_DRE_RCG_RCG_STATUS_RCG_FSM_QSTATE_SHFT                                              0x1
#define HWIO_DRE_DRE_RCG_RCG_STATUS_QREQ_N_SYNC_BMSK                                                 0x1
#define HWIO_DRE_DRE_RCG_RCG_STATUS_QREQ_N_SYNC_SHFT                                                 0x0

#define HWIO_DRE_DRE_RCG_CLOCK_CTRL_ADDR                                                      (DRE_REGS_REG_BASE      + 0x00000090)
#define HWIO_DRE_DRE_RCG_CLOCK_CTRL_PHYS                                                      (DRE_REGS_REG_BASE_PHYS + 0x00000090)
#define HWIO_DRE_DRE_RCG_CLOCK_CTRL_OFFS                                                      (DRE_REGS_REG_BASE_OFFS + 0x00000090)
#define HWIO_DRE_DRE_RCG_CLOCK_CTRL_RMSK                                                             0x1
#define HWIO_DRE_DRE_RCG_CLOCK_CTRL_IN          \
        in_dword_masked(HWIO_DRE_DRE_RCG_CLOCK_CTRL_ADDR, HWIO_DRE_DRE_RCG_CLOCK_CTRL_RMSK)
#define HWIO_DRE_DRE_RCG_CLOCK_CTRL_INM(m)      \
        in_dword_masked(HWIO_DRE_DRE_RCG_CLOCK_CTRL_ADDR, m)
#define HWIO_DRE_DRE_RCG_CLOCK_CTRL_OUT(v)      \
        out_dword(HWIO_DRE_DRE_RCG_CLOCK_CTRL_ADDR,v)
#define HWIO_DRE_DRE_RCG_CLOCK_CTRL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DRE_DRE_RCG_CLOCK_CTRL_ADDR,m,v,HWIO_DRE_DRE_RCG_CLOCK_CTRL_IN)
#define HWIO_DRE_DRE_RCG_CLOCK_CTRL_PROF_CLK_EN_BMSK                                                 0x1
#define HWIO_DRE_DRE_RCG_CLOCK_CTRL_PROF_CLK_EN_SHFT                                                 0x0

#define HWIO_DRE_DRE_RCG_QACTIVE_STATUS_ADDR                                                  (DRE_REGS_REG_BASE      + 0x00000094)
#define HWIO_DRE_DRE_RCG_QACTIVE_STATUS_PHYS                                                  (DRE_REGS_REG_BASE_PHYS + 0x00000094)
#define HWIO_DRE_DRE_RCG_QACTIVE_STATUS_OFFS                                                  (DRE_REGS_REG_BASE_OFFS + 0x00000094)
#define HWIO_DRE_DRE_RCG_QACTIVE_STATUS_RMSK                                                     0x7ff03
#define HWIO_DRE_DRE_RCG_QACTIVE_STATUS_IN          \
        in_dword_masked(HWIO_DRE_DRE_RCG_QACTIVE_STATUS_ADDR, HWIO_DRE_DRE_RCG_QACTIVE_STATUS_RMSK)
#define HWIO_DRE_DRE_RCG_QACTIVE_STATUS_INM(m)      \
        in_dword_masked(HWIO_DRE_DRE_RCG_QACTIVE_STATUS_ADDR, m)
#define HWIO_DRE_DRE_RCG_QACTIVE_STATUS_DRE_BLOCKS_QACTIVE_BMSK                                  0x7ff00
#define HWIO_DRE_DRE_RCG_QACTIVE_STATUS_DRE_BLOCKS_QACTIVE_SHFT                                      0x8
#define HWIO_DRE_DRE_RCG_QACTIVE_STATUS_REF_QACTIVE_BMSK                                             0x2
#define HWIO_DRE_DRE_RCG_QACTIVE_STATUS_REF_QACTIVE_SHFT                                             0x1
#define HWIO_DRE_DRE_RCG_QACTIVE_STATUS_CFG_QACTIVE_BMSK                                             0x1
#define HWIO_DRE_DRE_RCG_QACTIVE_STATUS_CFG_QACTIVE_SHFT                                             0x0

#define HWIO_DRE_DRE_FENCE_EN_ADDR                                                            (DRE_REGS_REG_BASE      + 0x00000098)
#define HWIO_DRE_DRE_FENCE_EN_PHYS                                                            (DRE_REGS_REG_BASE_PHYS + 0x00000098)
#define HWIO_DRE_DRE_FENCE_EN_OFFS                                                            (DRE_REGS_REG_BASE_OFFS + 0x00000098)
#define HWIO_DRE_DRE_FENCE_EN_RMSK                                                                 0x101
#define HWIO_DRE_DRE_FENCE_EN_IN          \
        in_dword_masked(HWIO_DRE_DRE_FENCE_EN_ADDR, HWIO_DRE_DRE_FENCE_EN_RMSK)
#define HWIO_DRE_DRE_FENCE_EN_INM(m)      \
        in_dword_masked(HWIO_DRE_DRE_FENCE_EN_ADDR, m)
#define HWIO_DRE_DRE_FENCE_EN_OUT(v)      \
        out_dword(HWIO_DRE_DRE_FENCE_EN_ADDR,v)
#define HWIO_DRE_DRE_FENCE_EN_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DRE_DRE_FENCE_EN_ADDR,m,v,HWIO_DRE_DRE_FENCE_EN_IN)
#define HWIO_DRE_DRE_FENCE_EN_CTI_FENCE_ENABLE_BMSK                                                0x100
#define HWIO_DRE_DRE_FENCE_EN_CTI_FENCE_ENABLE_SHFT                                                  0x8
#define HWIO_DRE_DRE_FENCE_EN_RCG_FENCE_ENABLE_BMSK                                                  0x1
#define HWIO_DRE_DRE_FENCE_EN_RCG_FENCE_ENABLE_SHFT                                                  0x0

#define HWIO_DRE_DRE_GLOBAL_MON_CFG0_ADDR                                                     (DRE_REGS_REG_BASE      + 0x000000a0)
#define HWIO_DRE_DRE_GLOBAL_MON_CFG0_PHYS                                                     (DRE_REGS_REG_BASE_PHYS + 0x000000a0)
#define HWIO_DRE_DRE_GLOBAL_MON_CFG0_OFFS                                                     (DRE_REGS_REG_BASE_OFFS + 0x000000a0)
#define HWIO_DRE_DRE_GLOBAL_MON_CFG0_RMSK                                                         0x1113
#define HWIO_DRE_DRE_GLOBAL_MON_CFG0_IN          \
        in_dword_masked(HWIO_DRE_DRE_GLOBAL_MON_CFG0_ADDR, HWIO_DRE_DRE_GLOBAL_MON_CFG0_RMSK)
#define HWIO_DRE_DRE_GLOBAL_MON_CFG0_INM(m)      \
        in_dword_masked(HWIO_DRE_DRE_GLOBAL_MON_CFG0_ADDR, m)
#define HWIO_DRE_DRE_GLOBAL_MON_CFG0_OUT(v)      \
        out_dword(HWIO_DRE_DRE_GLOBAL_MON_CFG0_ADDR,v)
#define HWIO_DRE_DRE_GLOBAL_MON_CFG0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DRE_DRE_GLOBAL_MON_CFG0_ADDR,m,v,HWIO_DRE_DRE_GLOBAL_MON_CFG0_IN)
#define HWIO_DRE_DRE_GLOBAL_MON_CFG0_CLR_EXMON_EN_BMSK                                            0x1000
#define HWIO_DRE_DRE_GLOBAL_MON_CFG0_CLR_EXMON_EN_SHFT                                               0xc
#define HWIO_DRE_DRE_GLOBAL_MON_CFG0_EXMON_QSB2_1_EN_BMSK                                          0x100
#define HWIO_DRE_DRE_GLOBAL_MON_CFG0_EXMON_QSB2_1_EN_SHFT                                            0x8
#define HWIO_DRE_DRE_GLOBAL_MON_CFG0_EXRD_RESP_BMSK                                                 0x10
#define HWIO_DRE_DRE_GLOBAL_MON_CFG0_EXRD_RESP_SHFT                                                  0x4
#define HWIO_DRE_DRE_GLOBAL_MON_CFG0_EXCMD_ADDR_GRAN_BMSK                                            0x3
#define HWIO_DRE_DRE_GLOBAL_MON_CFG0_EXCMD_ADDR_GRAN_SHFT                                            0x0

#define HWIO_DRE_DRE_CCU_GLOBAL_MON_STATUS_ADDR                                               (DRE_REGS_REG_BASE      + 0x000000a4)
#define HWIO_DRE_DRE_CCU_GLOBAL_MON_STATUS_PHYS                                               (DRE_REGS_REG_BASE_PHYS + 0x000000a4)
#define HWIO_DRE_DRE_CCU_GLOBAL_MON_STATUS_OFFS                                               (DRE_REGS_REG_BASE_OFFS + 0x000000a4)
#define HWIO_DRE_DRE_CCU_GLOBAL_MON_STATUS_RMSK                                                0x7ffffff
#define HWIO_DRE_DRE_CCU_GLOBAL_MON_STATUS_IN          \
        in_dword_masked(HWIO_DRE_DRE_CCU_GLOBAL_MON_STATUS_ADDR, HWIO_DRE_DRE_CCU_GLOBAL_MON_STATUS_RMSK)
#define HWIO_DRE_DRE_CCU_GLOBAL_MON_STATUS_INM(m)      \
        in_dword_masked(HWIO_DRE_DRE_CCU_GLOBAL_MON_STATUS_ADDR, m)
#define HWIO_DRE_DRE_CCU_GLOBAL_MON_STATUS_PEN_CLREXMON_EVENT_BMSK                             0x4000000
#define HWIO_DRE_DRE_CCU_GLOBAL_MON_STATUS_PEN_CLREXMON_EVENT_SHFT                                  0x1a
#define HWIO_DRE_DRE_CCU_GLOBAL_MON_STATUS_CLREXMONACK_BMSK                                    0x2000000
#define HWIO_DRE_DRE_CCU_GLOBAL_MON_STATUS_CLREXMONACK_SHFT                                         0x19
#define HWIO_DRE_DRE_CCU_GLOBAL_MON_STATUS_CLREXMONREQ_BMSK                                    0x1000000
#define HWIO_DRE_DRE_CCU_GLOBAL_MON_STATUS_CLREXMONREQ_SHFT                                         0x18
#define HWIO_DRE_DRE_CCU_GLOBAL_MON_STATUS_GLOBAL_MONS_IN_USE_BMSK                              0xffffff
#define HWIO_DRE_DRE_CCU_GLOBAL_MON_STATUS_GLOBAL_MONS_IN_USE_SHFT                                   0x0

#define HWIO_DRE_DRE_CCU_CMO_CONTROL_ADDR                                                     (DRE_REGS_REG_BASE      + 0x000000a8)
#define HWIO_DRE_DRE_CCU_CMO_CONTROL_PHYS                                                     (DRE_REGS_REG_BASE_PHYS + 0x000000a8)
#define HWIO_DRE_DRE_CCU_CMO_CONTROL_OFFS                                                     (DRE_REGS_REG_BASE_OFFS + 0x000000a8)
#define HWIO_DRE_DRE_CCU_CMO_CONTROL_RMSK                                                            0x3
#define HWIO_DRE_DRE_CCU_CMO_CONTROL_IN          \
        in_dword_masked(HWIO_DRE_DRE_CCU_CMO_CONTROL_ADDR, HWIO_DRE_DRE_CCU_CMO_CONTROL_RMSK)
#define HWIO_DRE_DRE_CCU_CMO_CONTROL_INM(m)      \
        in_dword_masked(HWIO_DRE_DRE_CCU_CMO_CONTROL_ADDR, m)
#define HWIO_DRE_DRE_CCU_CMO_CONTROL_OUT(v)      \
        out_dword(HWIO_DRE_DRE_CCU_CMO_CONTROL_ADDR,v)
#define HWIO_DRE_DRE_CCU_CMO_CONTROL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DRE_DRE_CCU_CMO_CONTROL_ADDR,m,v,HWIO_DRE_DRE_CCU_CMO_CONTROL_IN)
#define HWIO_DRE_DRE_CCU_CMO_CONTROL_BLOCK_EWD_BMSK                                                  0x2
#define HWIO_DRE_DRE_CCU_CMO_CONTROL_BLOCK_EWD_SHFT                                                  0x1
#define HWIO_DRE_DRE_CCU_CMO_CONTROL_OVERRIDE_RWI_WITH_RWE_OR_RD_BMSK                                0x1
#define HWIO_DRE_DRE_CCU_CMO_CONTROL_OVERRIDE_RWI_WITH_RWE_OR_RD_SHFT                                0x0

#define HWIO_DRE_DRE_DBG_CFG_ADDR                                                             (DRE_REGS_REG_BASE      + 0x000000b0)
#define HWIO_DRE_DRE_DBG_CFG_PHYS                                                             (DRE_REGS_REG_BASE_PHYS + 0x000000b0)
#define HWIO_DRE_DRE_DBG_CFG_OFFS                                                             (DRE_REGS_REG_BASE_OFFS + 0x000000b0)
#define HWIO_DRE_DRE_DBG_CFG_RMSK                                                                    0xf
#define HWIO_DRE_DRE_DBG_CFG_IN          \
        in_dword_masked(HWIO_DRE_DRE_DBG_CFG_ADDR, HWIO_DRE_DRE_DBG_CFG_RMSK)
#define HWIO_DRE_DRE_DBG_CFG_INM(m)      \
        in_dword_masked(HWIO_DRE_DRE_DBG_CFG_ADDR, m)
#define HWIO_DRE_DRE_DBG_CFG_OUT(v)      \
        out_dword(HWIO_DRE_DRE_DBG_CFG_ADDR,v)
#define HWIO_DRE_DRE_DBG_CFG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DRE_DRE_DBG_CFG_ADDR,m,v,HWIO_DRE_DRE_DBG_CFG_IN)
#define HWIO_DRE_DRE_DBG_CFG_GLOBAL_MON_BITS_SEL_BMSK                                                0xf
#define HWIO_DRE_DRE_DBG_CFG_GLOBAL_MON_BITS_SEL_SHFT                                                0x0

#define HWIO_DRE_DRE_CCU_CLK_EN_COUNT_ADDR                                                    (DRE_REGS_REG_BASE      + 0x000000b4)
#define HWIO_DRE_DRE_CCU_CLK_EN_COUNT_PHYS                                                    (DRE_REGS_REG_BASE_PHYS + 0x000000b4)
#define HWIO_DRE_DRE_CCU_CLK_EN_COUNT_OFFS                                                    (DRE_REGS_REG_BASE_OFFS + 0x000000b4)
#define HWIO_DRE_DRE_CCU_CLK_EN_COUNT_RMSK                                                          0xff
#define HWIO_DRE_DRE_CCU_CLK_EN_COUNT_IN          \
        in_dword_masked(HWIO_DRE_DRE_CCU_CLK_EN_COUNT_ADDR, HWIO_DRE_DRE_CCU_CLK_EN_COUNT_RMSK)
#define HWIO_DRE_DRE_CCU_CLK_EN_COUNT_INM(m)      \
        in_dword_masked(HWIO_DRE_DRE_CCU_CLK_EN_COUNT_ADDR, m)
#define HWIO_DRE_DRE_CCU_CLK_EN_COUNT_OUT(v)      \
        out_dword(HWIO_DRE_DRE_CCU_CLK_EN_COUNT_ADDR,v)
#define HWIO_DRE_DRE_CCU_CLK_EN_COUNT_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DRE_DRE_CCU_CLK_EN_COUNT_ADDR,m,v,HWIO_DRE_DRE_CCU_CLK_EN_COUNT_IN)
#define HWIO_DRE_DRE_CCU_CLK_EN_COUNT_XPU_CLK_EN_COUNT_BMSK                                         0xff
#define HWIO_DRE_DRE_CCU_CLK_EN_COUNT_XPU_CLK_EN_COUNT_SHFT                                          0x0

#define HWIO_DRE_DRE_CLK_EN_CFG_ADDR                                                          (DRE_REGS_REG_BASE      + 0x000000c0)
#define HWIO_DRE_DRE_CLK_EN_CFG_PHYS                                                          (DRE_REGS_REG_BASE_PHYS + 0x000000c0)
#define HWIO_DRE_DRE_CLK_EN_CFG_OFFS                                                          (DRE_REGS_REG_BASE_OFFS + 0x000000c0)
#define HWIO_DRE_DRE_CLK_EN_CFG_RMSK                                                               0x1ff
#define HWIO_DRE_DRE_CLK_EN_CFG_IN          \
        in_dword_masked(HWIO_DRE_DRE_CLK_EN_CFG_ADDR, HWIO_DRE_DRE_CLK_EN_CFG_RMSK)
#define HWIO_DRE_DRE_CLK_EN_CFG_INM(m)      \
        in_dword_masked(HWIO_DRE_DRE_CLK_EN_CFG_ADDR, m)
#define HWIO_DRE_DRE_CLK_EN_CFG_OUT(v)      \
        out_dword(HWIO_DRE_DRE_CLK_EN_CFG_ADDR,v)
#define HWIO_DRE_DRE_CLK_EN_CFG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DRE_DRE_CLK_EN_CFG_ADDR,m,v,HWIO_DRE_DRE_CLK_EN_CFG_IN)
#define HWIO_DRE_DRE_CLK_EN_CFG_CCU_CLK_EN_BMSK                                                    0x100
#define HWIO_DRE_DRE_CLK_EN_CFG_CCU_CLK_EN_SHFT                                                      0x8
#define HWIO_DRE_DRE_CLK_EN_CFG_RCH_RD_BUF_CLK_EN_BMSK                                              0x80
#define HWIO_DRE_DRE_CLK_EN_CFG_RCH_RD_BUF_CLK_EN_SHFT                                               0x7
#define HWIO_DRE_DRE_CLK_EN_CFG_SYND_RD_BUF_CLK_EN_BMSK                                             0x40
#define HWIO_DRE_DRE_CLK_EN_CFG_SYND_RD_BUF_CLK_EN_SHFT                                              0x6
#define HWIO_DRE_DRE_CLK_EN_CFG_SYND_WR_BUF_CLK_EN_BMSK                                             0x20
#define HWIO_DRE_DRE_CLK_EN_CFG_SYND_WR_BUF_CLK_EN_SHFT                                              0x5
#define HWIO_DRE_DRE_CLK_EN_CFG_SYND_EVICT_FIFO_CLK_EN_BMSK                                         0x10
#define HWIO_DRE_DRE_CLK_EN_CFG_SYND_EVICT_FIFO_CLK_EN_SHFT                                          0x4
#define HWIO_DRE_DRE_CLK_EN_CFG_SYND_CACHE_CLK_EN_BMSK                                               0x8
#define HWIO_DRE_DRE_CLK_EN_CFG_SYND_CACHE_CLK_EN_SHFT                                               0x3
#define HWIO_DRE_DRE_CLK_EN_CFG_SYND_TAG_CLK_EN_BMSK                                                 0x4
#define HWIO_DRE_DRE_CLK_EN_CFG_SYND_TAG_CLK_EN_SHFT                                                 0x2
#define HWIO_DRE_DRE_CLK_EN_CFG_WCH_FIFO_CLK_EN_BMSK                                                 0x2
#define HWIO_DRE_DRE_CLK_EN_CFG_WCH_FIFO_CLK_EN_SHFT                                                 0x1
#define HWIO_DRE_DRE_CLK_EN_CFG_WR_BUF_CLK_EN_BMSK                                                   0x1
#define HWIO_DRE_DRE_CLK_EN_CFG_WR_BUF_CLK_EN_SHFT                                                   0x0

#define HWIO_DRE_DRE_COMMON_CGC_CFG_ADDR                                                      (DRE_REGS_REG_BASE      + 0x000000c4)
#define HWIO_DRE_DRE_COMMON_CGC_CFG_PHYS                                                      (DRE_REGS_REG_BASE_PHYS + 0x000000c4)
#define HWIO_DRE_DRE_COMMON_CGC_CFG_OFFS                                                      (DRE_REGS_REG_BASE_OFFS + 0x000000c4)
#define HWIO_DRE_DRE_COMMON_CGC_CFG_RMSK                                                             0x2
#define HWIO_DRE_DRE_COMMON_CGC_CFG_IN          \
        in_dword_masked(HWIO_DRE_DRE_COMMON_CGC_CFG_ADDR, HWIO_DRE_DRE_COMMON_CGC_CFG_RMSK)
#define HWIO_DRE_DRE_COMMON_CGC_CFG_INM(m)      \
        in_dword_masked(HWIO_DRE_DRE_COMMON_CGC_CFG_ADDR, m)
#define HWIO_DRE_DRE_COMMON_CGC_CFG_OUT(v)      \
        out_dword(HWIO_DRE_DRE_COMMON_CGC_CFG_ADDR,v)
#define HWIO_DRE_DRE_COMMON_CGC_CFG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DRE_DRE_COMMON_CGC_CFG_ADDR,m,v,HWIO_DRE_DRE_COMMON_CGC_CFG_IN)
#define HWIO_DRE_DRE_COMMON_CGC_CFG_DRE_FBG_CLK_ENABLE_BMSK                                          0x2
#define HWIO_DRE_DRE_COMMON_CGC_CFG_DRE_FBG_CLK_ENABLE_SHFT                                          0x1

#define HWIO_DRE_DRE_SYNDROME_QSB_BID_PID_MID_OVERRIDE_ADDR                                   (DRE_REGS_REG_BASE      + 0x00000100)
#define HWIO_DRE_DRE_SYNDROME_QSB_BID_PID_MID_OVERRIDE_PHYS                                   (DRE_REGS_REG_BASE_PHYS + 0x00000100)
#define HWIO_DRE_DRE_SYNDROME_QSB_BID_PID_MID_OVERRIDE_OFFS                                   (DRE_REGS_REG_BASE_OFFS + 0x00000100)
#define HWIO_DRE_DRE_SYNDROME_QSB_BID_PID_MID_OVERRIDE_RMSK                                     0xffff07
#define HWIO_DRE_DRE_SYNDROME_QSB_BID_PID_MID_OVERRIDE_IN          \
        in_dword_masked(HWIO_DRE_DRE_SYNDROME_QSB_BID_PID_MID_OVERRIDE_ADDR, HWIO_DRE_DRE_SYNDROME_QSB_BID_PID_MID_OVERRIDE_RMSK)
#define HWIO_DRE_DRE_SYNDROME_QSB_BID_PID_MID_OVERRIDE_INM(m)      \
        in_dword_masked(HWIO_DRE_DRE_SYNDROME_QSB_BID_PID_MID_OVERRIDE_ADDR, m)
#define HWIO_DRE_DRE_SYNDROME_QSB_BID_PID_MID_OVERRIDE_OUT(v)      \
        out_dword(HWIO_DRE_DRE_SYNDROME_QSB_BID_PID_MID_OVERRIDE_ADDR,v)
#define HWIO_DRE_DRE_SYNDROME_QSB_BID_PID_MID_OVERRIDE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DRE_DRE_SYNDROME_QSB_BID_PID_MID_OVERRIDE_ADDR,m,v,HWIO_DRE_DRE_SYNDROME_QSB_BID_PID_MID_OVERRIDE_IN)
#define HWIO_DRE_DRE_SYNDROME_QSB_BID_PID_MID_OVERRIDE_BID_BMSK                                 0xe00000
#define HWIO_DRE_DRE_SYNDROME_QSB_BID_PID_MID_OVERRIDE_BID_SHFT                                     0x15
#define HWIO_DRE_DRE_SYNDROME_QSB_BID_PID_MID_OVERRIDE_PID_BMSK                                 0x1f0000
#define HWIO_DRE_DRE_SYNDROME_QSB_BID_PID_MID_OVERRIDE_PID_SHFT                                     0x10
#define HWIO_DRE_DRE_SYNDROME_QSB_BID_PID_MID_OVERRIDE_MID_BMSK                                   0xff00
#define HWIO_DRE_DRE_SYNDROME_QSB_BID_PID_MID_OVERRIDE_MID_SHFT                                      0x8
#define HWIO_DRE_DRE_SYNDROME_QSB_BID_PID_MID_OVERRIDE_OVERRIDE_BID_BMSK                             0x4
#define HWIO_DRE_DRE_SYNDROME_QSB_BID_PID_MID_OVERRIDE_OVERRIDE_BID_SHFT                             0x2
#define HWIO_DRE_DRE_SYNDROME_QSB_BID_PID_MID_OVERRIDE_OVERRIDE_PID_BMSK                             0x2
#define HWIO_DRE_DRE_SYNDROME_QSB_BID_PID_MID_OVERRIDE_OVERRIDE_PID_SHFT                             0x1
#define HWIO_DRE_DRE_SYNDROME_QSB_BID_PID_MID_OVERRIDE_OVERRIDE_MID_BMSK                             0x1
#define HWIO_DRE_DRE_SYNDROME_QSB_BID_PID_MID_OVERRIDE_OVERRIDE_MID_SHFT                             0x0

#define HWIO_DRE_DRE_SYNDROME_QSB_WRITE_REQ_OVERRIDE_0_ADDR                                   (DRE_REGS_REG_BASE      + 0x00000108)
#define HWIO_DRE_DRE_SYNDROME_QSB_WRITE_REQ_OVERRIDE_0_PHYS                                   (DRE_REGS_REG_BASE_PHYS + 0x00000108)
#define HWIO_DRE_DRE_SYNDROME_QSB_WRITE_REQ_OVERRIDE_0_OFFS                                   (DRE_REGS_REG_BASE_OFFS + 0x00000108)
#define HWIO_DRE_DRE_SYNDROME_QSB_WRITE_REQ_OVERRIDE_0_RMSK                                   0x7fff8fff
#define HWIO_DRE_DRE_SYNDROME_QSB_WRITE_REQ_OVERRIDE_0_IN          \
        in_dword_masked(HWIO_DRE_DRE_SYNDROME_QSB_WRITE_REQ_OVERRIDE_0_ADDR, HWIO_DRE_DRE_SYNDROME_QSB_WRITE_REQ_OVERRIDE_0_RMSK)
#define HWIO_DRE_DRE_SYNDROME_QSB_WRITE_REQ_OVERRIDE_0_INM(m)      \
        in_dword_masked(HWIO_DRE_DRE_SYNDROME_QSB_WRITE_REQ_OVERRIDE_0_ADDR, m)
#define HWIO_DRE_DRE_SYNDROME_QSB_WRITE_REQ_OVERRIDE_0_OUT(v)      \
        out_dword(HWIO_DRE_DRE_SYNDROME_QSB_WRITE_REQ_OVERRIDE_0_ADDR,v)
#define HWIO_DRE_DRE_SYNDROME_QSB_WRITE_REQ_OVERRIDE_0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DRE_DRE_SYNDROME_QSB_WRITE_REQ_OVERRIDE_0_ADDR,m,v,HWIO_DRE_DRE_SYNDROME_QSB_WRITE_REQ_OVERRIDE_0_IN)
#define HWIO_DRE_DRE_SYNDROME_QSB_WRITE_REQ_OVERRIDE_0_AREQPRIORITY_BMSK                      0x70000000
#define HWIO_DRE_DRE_SYNDROME_QSB_WRITE_REQ_OVERRIDE_0_AREQPRIORITY_SHFT                            0x1c
#define HWIO_DRE_DRE_SYNDROME_QSB_WRITE_REQ_OVERRIDE_0_ASCID_BMSK                              0xf800000
#define HWIO_DRE_DRE_SYNDROME_QSB_WRITE_REQ_OVERRIDE_0_ASCID_SHFT                                   0x17
#define HWIO_DRE_DRE_SYNDROME_QSB_WRITE_REQ_OVERRIDE_0_AWRITETHROUGH_BMSK                       0x400000
#define HWIO_DRE_DRE_SYNDROME_QSB_WRITE_REQ_OVERRIDE_0_AWRITETHROUGH_SHFT                           0x16
#define HWIO_DRE_DRE_SYNDROME_QSB_WRITE_REQ_OVERRIDE_0_ANOALLOCATE_BMSK                         0x200000
#define HWIO_DRE_DRE_SYNDROME_QSB_WRITE_REQ_OVERRIDE_0_ANOALLOCATE_SHFT                             0x15
#define HWIO_DRE_DRE_SYNDROME_QSB_WRITE_REQ_OVERRIDE_0_ASHARED_BMSK                             0x100000
#define HWIO_DRE_DRE_SYNDROME_QSB_WRITE_REQ_OVERRIDE_0_ASHARED_SHFT                                 0x14
#define HWIO_DRE_DRE_SYNDROME_QSB_WRITE_REQ_OVERRIDE_0_AINNERSHARED_BMSK                         0x80000
#define HWIO_DRE_DRE_SYNDROME_QSB_WRITE_REQ_OVERRIDE_0_AINNERSHARED_SHFT                            0x13
#define HWIO_DRE_DRE_SYNDROME_QSB_WRITE_REQ_OVERRIDE_0_AINNERCACHEABLE_BMSK                      0x40000
#define HWIO_DRE_DRE_SYNDROME_QSB_WRITE_REQ_OVERRIDE_0_AINNERCACHEABLE_SHFT                         0x12
#define HWIO_DRE_DRE_SYNDROME_QSB_WRITE_REQ_OVERRIDE_0_ACACHEABLE_BMSK                           0x20000
#define HWIO_DRE_DRE_SYNDROME_QSB_WRITE_REQ_OVERRIDE_0_ACACHEABLE_SHFT                              0x11
#define HWIO_DRE_DRE_SYNDROME_QSB_WRITE_REQ_OVERRIDE_0_APRIV_BMSK                                0x10000
#define HWIO_DRE_DRE_SYNDROME_QSB_WRITE_REQ_OVERRIDE_0_APRIV_SHFT                                   0x10
#define HWIO_DRE_DRE_SYNDROME_QSB_WRITE_REQ_OVERRIDE_0_AINST_BMSK                                 0x8000
#define HWIO_DRE_DRE_SYNDROME_QSB_WRITE_REQ_OVERRIDE_0_AINST_SHFT                                    0xf
#define HWIO_DRE_DRE_SYNDROME_QSB_WRITE_REQ_OVERRIDE_0_ATRANSIENT_BMSK                             0x800
#define HWIO_DRE_DRE_SYNDROME_QSB_WRITE_REQ_OVERRIDE_0_ATRANSIENT_SHFT                               0xb
#define HWIO_DRE_DRE_SYNDROME_QSB_WRITE_REQ_OVERRIDE_0_OVERRIDE_AREQPRIORITY_BMSK                  0x400
#define HWIO_DRE_DRE_SYNDROME_QSB_WRITE_REQ_OVERRIDE_0_OVERRIDE_AREQPRIORITY_SHFT                    0xa
#define HWIO_DRE_DRE_SYNDROME_QSB_WRITE_REQ_OVERRIDE_0_OVERRIDE_ASCID_BMSK                         0x200
#define HWIO_DRE_DRE_SYNDROME_QSB_WRITE_REQ_OVERRIDE_0_OVERRIDE_ASCID_SHFT                           0x9
#define HWIO_DRE_DRE_SYNDROME_QSB_WRITE_REQ_OVERRIDE_0_OVERRIDE_AWRITETHROUGH_BMSK                 0x100
#define HWIO_DRE_DRE_SYNDROME_QSB_WRITE_REQ_OVERRIDE_0_OVERRIDE_AWRITETHROUGH_SHFT                   0x8
#define HWIO_DRE_DRE_SYNDROME_QSB_WRITE_REQ_OVERRIDE_0_OVERRIDE_ANOALLOCATE_BMSK                    0x80
#define HWIO_DRE_DRE_SYNDROME_QSB_WRITE_REQ_OVERRIDE_0_OVERRIDE_ANOALLOCATE_SHFT                     0x7
#define HWIO_DRE_DRE_SYNDROME_QSB_WRITE_REQ_OVERRIDE_0_OVERRIDE_ASHARED_BMSK                        0x40
#define HWIO_DRE_DRE_SYNDROME_QSB_WRITE_REQ_OVERRIDE_0_OVERRIDE_ASHARED_SHFT                         0x6
#define HWIO_DRE_DRE_SYNDROME_QSB_WRITE_REQ_OVERRIDE_0_OVERRIDE_AINNERSHARED_BMSK                   0x20
#define HWIO_DRE_DRE_SYNDROME_QSB_WRITE_REQ_OVERRIDE_0_OVERRIDE_AINNERSHARED_SHFT                    0x5
#define HWIO_DRE_DRE_SYNDROME_QSB_WRITE_REQ_OVERRIDE_0_OVERRIDE_AINNERCACHEABLE_BMSK                0x10
#define HWIO_DRE_DRE_SYNDROME_QSB_WRITE_REQ_OVERRIDE_0_OVERRIDE_AINNERCACHEABLE_SHFT                 0x4
#define HWIO_DRE_DRE_SYNDROME_QSB_WRITE_REQ_OVERRIDE_0_OVERRIDE_ACACHEABLE_BMSK                      0x8
#define HWIO_DRE_DRE_SYNDROME_QSB_WRITE_REQ_OVERRIDE_0_OVERRIDE_ACACHEABLE_SHFT                      0x3
#define HWIO_DRE_DRE_SYNDROME_QSB_WRITE_REQ_OVERRIDE_0_OVERRIDE_APRIV_BMSK                           0x4
#define HWIO_DRE_DRE_SYNDROME_QSB_WRITE_REQ_OVERRIDE_0_OVERRIDE_APRIV_SHFT                           0x2
#define HWIO_DRE_DRE_SYNDROME_QSB_WRITE_REQ_OVERRIDE_0_OVERRIDE_AINST_BMSK                           0x2
#define HWIO_DRE_DRE_SYNDROME_QSB_WRITE_REQ_OVERRIDE_0_OVERRIDE_AINST_SHFT                           0x1
#define HWIO_DRE_DRE_SYNDROME_QSB_WRITE_REQ_OVERRIDE_0_OVERRIDE_ATRANSIENT_BMSK                      0x1
#define HWIO_DRE_DRE_SYNDROME_QSB_WRITE_REQ_OVERRIDE_0_OVERRIDE_ATRANSIENT_SHFT                      0x0

#define HWIO_DRE_DRE_SYNDROME_QSB_WRITE_REQ_OVERRIDE_1_ADDR                                   (DRE_REGS_REG_BASE      + 0x0000010c)
#define HWIO_DRE_DRE_SYNDROME_QSB_WRITE_REQ_OVERRIDE_1_PHYS                                   (DRE_REGS_REG_BASE_PHYS + 0x0000010c)
#define HWIO_DRE_DRE_SYNDROME_QSB_WRITE_REQ_OVERRIDE_1_OFFS                                   (DRE_REGS_REG_BASE_OFFS + 0x0000010c)
#define HWIO_DRE_DRE_SYNDROME_QSB_WRITE_REQ_OVERRIDE_1_RMSK                                   0xffffffff
#define HWIO_DRE_DRE_SYNDROME_QSB_WRITE_REQ_OVERRIDE_1_IN          \
        in_dword_masked(HWIO_DRE_DRE_SYNDROME_QSB_WRITE_REQ_OVERRIDE_1_ADDR, HWIO_DRE_DRE_SYNDROME_QSB_WRITE_REQ_OVERRIDE_1_RMSK)
#define HWIO_DRE_DRE_SYNDROME_QSB_WRITE_REQ_OVERRIDE_1_INM(m)      \
        in_dword_masked(HWIO_DRE_DRE_SYNDROME_QSB_WRITE_REQ_OVERRIDE_1_ADDR, m)
#define HWIO_DRE_DRE_SYNDROME_QSB_WRITE_REQ_OVERRIDE_1_OUT(v)      \
        out_dword(HWIO_DRE_DRE_SYNDROME_QSB_WRITE_REQ_OVERRIDE_1_ADDR,v)
#define HWIO_DRE_DRE_SYNDROME_QSB_WRITE_REQ_OVERRIDE_1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DRE_DRE_SYNDROME_QSB_WRITE_REQ_OVERRIDE_1_ADDR,m,v,HWIO_DRE_DRE_SYNDROME_QSB_WRITE_REQ_OVERRIDE_1_IN)
#define HWIO_DRE_DRE_SYNDROME_QSB_WRITE_REQ_OVERRIDE_1_RSVDB_BMSK                             0xffe00000
#define HWIO_DRE_DRE_SYNDROME_QSB_WRITE_REQ_OVERRIDE_1_RSVDB_SHFT                                   0x15
#define HWIO_DRE_DRE_SYNDROME_QSB_WRITE_REQ_OVERRIDE_1_ADEVICE_BMSK                             0x100000
#define HWIO_DRE_DRE_SYNDROME_QSB_WRITE_REQ_OVERRIDE_1_ADEVICE_SHFT                                 0x14
#define HWIO_DRE_DRE_SYNDROME_QSB_WRITE_REQ_OVERRIDE_1_ACACHEOPTYPE_BMSK                         0xf0000
#define HWIO_DRE_DRE_SYNDROME_QSB_WRITE_REQ_OVERRIDE_1_ACACHEOPTYPE_SHFT                            0x10
#define HWIO_DRE_DRE_SYNDROME_QSB_WRITE_REQ_OVERRIDE_1_RSVDA_BMSK                                 0xfffc
#define HWIO_DRE_DRE_SYNDROME_QSB_WRITE_REQ_OVERRIDE_1_RSVDA_SHFT                                    0x2
#define HWIO_DRE_DRE_SYNDROME_QSB_WRITE_REQ_OVERRIDE_1_OVERRIDE_ADEVICE_BMSK                         0x2
#define HWIO_DRE_DRE_SYNDROME_QSB_WRITE_REQ_OVERRIDE_1_OVERRIDE_ADEVICE_SHFT                         0x1
#define HWIO_DRE_DRE_SYNDROME_QSB_WRITE_REQ_OVERRIDE_1_OVERRIDE_ACACHEOPTYPE_BMSK                    0x1
#define HWIO_DRE_DRE_SYNDROME_QSB_WRITE_REQ_OVERRIDE_1_OVERRIDE_ACACHEOPTYPE_SHFT                    0x0

#define HWIO_DRE_DRE_SYNDROME_QSB_READ_REQ_OVERRIDE_0_ADDR                                    (DRE_REGS_REG_BASE      + 0x00000110)
#define HWIO_DRE_DRE_SYNDROME_QSB_READ_REQ_OVERRIDE_0_PHYS                                    (DRE_REGS_REG_BASE_PHYS + 0x00000110)
#define HWIO_DRE_DRE_SYNDROME_QSB_READ_REQ_OVERRIDE_0_OFFS                                    (DRE_REGS_REG_BASE_OFFS + 0x00000110)
#define HWIO_DRE_DRE_SYNDROME_QSB_READ_REQ_OVERRIDE_0_RMSK                                    0x7fff8fff
#define HWIO_DRE_DRE_SYNDROME_QSB_READ_REQ_OVERRIDE_0_IN          \
        in_dword_masked(HWIO_DRE_DRE_SYNDROME_QSB_READ_REQ_OVERRIDE_0_ADDR, HWIO_DRE_DRE_SYNDROME_QSB_READ_REQ_OVERRIDE_0_RMSK)
#define HWIO_DRE_DRE_SYNDROME_QSB_READ_REQ_OVERRIDE_0_INM(m)      \
        in_dword_masked(HWIO_DRE_DRE_SYNDROME_QSB_READ_REQ_OVERRIDE_0_ADDR, m)
#define HWIO_DRE_DRE_SYNDROME_QSB_READ_REQ_OVERRIDE_0_OUT(v)      \
        out_dword(HWIO_DRE_DRE_SYNDROME_QSB_READ_REQ_OVERRIDE_0_ADDR,v)
#define HWIO_DRE_DRE_SYNDROME_QSB_READ_REQ_OVERRIDE_0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DRE_DRE_SYNDROME_QSB_READ_REQ_OVERRIDE_0_ADDR,m,v,HWIO_DRE_DRE_SYNDROME_QSB_READ_REQ_OVERRIDE_0_IN)
#define HWIO_DRE_DRE_SYNDROME_QSB_READ_REQ_OVERRIDE_0_AREQPRIORITY_BMSK                       0x70000000
#define HWIO_DRE_DRE_SYNDROME_QSB_READ_REQ_OVERRIDE_0_AREQPRIORITY_SHFT                             0x1c
#define HWIO_DRE_DRE_SYNDROME_QSB_READ_REQ_OVERRIDE_0_ASCID_BMSK                               0xf800000
#define HWIO_DRE_DRE_SYNDROME_QSB_READ_REQ_OVERRIDE_0_ASCID_SHFT                                    0x17
#define HWIO_DRE_DRE_SYNDROME_QSB_READ_REQ_OVERRIDE_0_AWRITETHROUGH_BMSK                        0x400000
#define HWIO_DRE_DRE_SYNDROME_QSB_READ_REQ_OVERRIDE_0_AWRITETHROUGH_SHFT                            0x16
#define HWIO_DRE_DRE_SYNDROME_QSB_READ_REQ_OVERRIDE_0_ANOALLOCATE_BMSK                          0x200000
#define HWIO_DRE_DRE_SYNDROME_QSB_READ_REQ_OVERRIDE_0_ANOALLOCATE_SHFT                              0x15
#define HWIO_DRE_DRE_SYNDROME_QSB_READ_REQ_OVERRIDE_0_ASHARED_BMSK                              0x100000
#define HWIO_DRE_DRE_SYNDROME_QSB_READ_REQ_OVERRIDE_0_ASHARED_SHFT                                  0x14
#define HWIO_DRE_DRE_SYNDROME_QSB_READ_REQ_OVERRIDE_0_AINNERSHARED_BMSK                          0x80000
#define HWIO_DRE_DRE_SYNDROME_QSB_READ_REQ_OVERRIDE_0_AINNERSHARED_SHFT                             0x13
#define HWIO_DRE_DRE_SYNDROME_QSB_READ_REQ_OVERRIDE_0_AINNERCACHEABLE_BMSK                       0x40000
#define HWIO_DRE_DRE_SYNDROME_QSB_READ_REQ_OVERRIDE_0_AINNERCACHEABLE_SHFT                          0x12
#define HWIO_DRE_DRE_SYNDROME_QSB_READ_REQ_OVERRIDE_0_ACACHEABLE_BMSK                            0x20000
#define HWIO_DRE_DRE_SYNDROME_QSB_READ_REQ_OVERRIDE_0_ACACHEABLE_SHFT                               0x11
#define HWIO_DRE_DRE_SYNDROME_QSB_READ_REQ_OVERRIDE_0_APRIV_BMSK                                 0x10000
#define HWIO_DRE_DRE_SYNDROME_QSB_READ_REQ_OVERRIDE_0_APRIV_SHFT                                    0x10
#define HWIO_DRE_DRE_SYNDROME_QSB_READ_REQ_OVERRIDE_0_AINST_BMSK                                  0x8000
#define HWIO_DRE_DRE_SYNDROME_QSB_READ_REQ_OVERRIDE_0_AINST_SHFT                                     0xf
#define HWIO_DRE_DRE_SYNDROME_QSB_READ_REQ_OVERRIDE_0_ATRANSIENT_BMSK                              0x800
#define HWIO_DRE_DRE_SYNDROME_QSB_READ_REQ_OVERRIDE_0_ATRANSIENT_SHFT                                0xb
#define HWIO_DRE_DRE_SYNDROME_QSB_READ_REQ_OVERRIDE_0_OVERRIDE_AREQPRIORITY_BMSK                   0x400
#define HWIO_DRE_DRE_SYNDROME_QSB_READ_REQ_OVERRIDE_0_OVERRIDE_AREQPRIORITY_SHFT                     0xa
#define HWIO_DRE_DRE_SYNDROME_QSB_READ_REQ_OVERRIDE_0_OVERRIDE_ASCID_BMSK                          0x200
#define HWIO_DRE_DRE_SYNDROME_QSB_READ_REQ_OVERRIDE_0_OVERRIDE_ASCID_SHFT                            0x9
#define HWIO_DRE_DRE_SYNDROME_QSB_READ_REQ_OVERRIDE_0_OVERRIDE_AWRITETHROUGH_BMSK                  0x100
#define HWIO_DRE_DRE_SYNDROME_QSB_READ_REQ_OVERRIDE_0_OVERRIDE_AWRITETHROUGH_SHFT                    0x8
#define HWIO_DRE_DRE_SYNDROME_QSB_READ_REQ_OVERRIDE_0_OVERRIDE_ANOALLOCATE_BMSK                     0x80
#define HWIO_DRE_DRE_SYNDROME_QSB_READ_REQ_OVERRIDE_0_OVERRIDE_ANOALLOCATE_SHFT                      0x7
#define HWIO_DRE_DRE_SYNDROME_QSB_READ_REQ_OVERRIDE_0_OVERRIDE_ASHARED_BMSK                         0x40
#define HWIO_DRE_DRE_SYNDROME_QSB_READ_REQ_OVERRIDE_0_OVERRIDE_ASHARED_SHFT                          0x6
#define HWIO_DRE_DRE_SYNDROME_QSB_READ_REQ_OVERRIDE_0_OVERRIDE_AINNERSHARED_BMSK                    0x20
#define HWIO_DRE_DRE_SYNDROME_QSB_READ_REQ_OVERRIDE_0_OVERRIDE_AINNERSHARED_SHFT                     0x5
#define HWIO_DRE_DRE_SYNDROME_QSB_READ_REQ_OVERRIDE_0_OVERRIDE_AINNERCACHEABLE_BMSK                 0x10
#define HWIO_DRE_DRE_SYNDROME_QSB_READ_REQ_OVERRIDE_0_OVERRIDE_AINNERCACHEABLE_SHFT                  0x4
#define HWIO_DRE_DRE_SYNDROME_QSB_READ_REQ_OVERRIDE_0_OVERRIDE_ACACHEABLE_BMSK                       0x8
#define HWIO_DRE_DRE_SYNDROME_QSB_READ_REQ_OVERRIDE_0_OVERRIDE_ACACHEABLE_SHFT                       0x3
#define HWIO_DRE_DRE_SYNDROME_QSB_READ_REQ_OVERRIDE_0_OVERRIDE_APRIV_BMSK                            0x4
#define HWIO_DRE_DRE_SYNDROME_QSB_READ_REQ_OVERRIDE_0_OVERRIDE_APRIV_SHFT                            0x2
#define HWIO_DRE_DRE_SYNDROME_QSB_READ_REQ_OVERRIDE_0_OVERRIDE_AINST_BMSK                            0x2
#define HWIO_DRE_DRE_SYNDROME_QSB_READ_REQ_OVERRIDE_0_OVERRIDE_AINST_SHFT                            0x1
#define HWIO_DRE_DRE_SYNDROME_QSB_READ_REQ_OVERRIDE_0_OVERRIDE_ATRANSIENT_BMSK                       0x1
#define HWIO_DRE_DRE_SYNDROME_QSB_READ_REQ_OVERRIDE_0_OVERRIDE_ATRANSIENT_SHFT                       0x0

#define HWIO_DRE_DRE_SYNDROME_QSB_READ_REQ_OVERRIDE_1_ADDR                                    (DRE_REGS_REG_BASE      + 0x00000114)
#define HWIO_DRE_DRE_SYNDROME_QSB_READ_REQ_OVERRIDE_1_PHYS                                    (DRE_REGS_REG_BASE_PHYS + 0x00000114)
#define HWIO_DRE_DRE_SYNDROME_QSB_READ_REQ_OVERRIDE_1_OFFS                                    (DRE_REGS_REG_BASE_OFFS + 0x00000114)
#define HWIO_DRE_DRE_SYNDROME_QSB_READ_REQ_OVERRIDE_1_RMSK                                    0xffffffff
#define HWIO_DRE_DRE_SYNDROME_QSB_READ_REQ_OVERRIDE_1_IN          \
        in_dword_masked(HWIO_DRE_DRE_SYNDROME_QSB_READ_REQ_OVERRIDE_1_ADDR, HWIO_DRE_DRE_SYNDROME_QSB_READ_REQ_OVERRIDE_1_RMSK)
#define HWIO_DRE_DRE_SYNDROME_QSB_READ_REQ_OVERRIDE_1_INM(m)      \
        in_dword_masked(HWIO_DRE_DRE_SYNDROME_QSB_READ_REQ_OVERRIDE_1_ADDR, m)
#define HWIO_DRE_DRE_SYNDROME_QSB_READ_REQ_OVERRIDE_1_OUT(v)      \
        out_dword(HWIO_DRE_DRE_SYNDROME_QSB_READ_REQ_OVERRIDE_1_ADDR,v)
#define HWIO_DRE_DRE_SYNDROME_QSB_READ_REQ_OVERRIDE_1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DRE_DRE_SYNDROME_QSB_READ_REQ_OVERRIDE_1_ADDR,m,v,HWIO_DRE_DRE_SYNDROME_QSB_READ_REQ_OVERRIDE_1_IN)
#define HWIO_DRE_DRE_SYNDROME_QSB_READ_REQ_OVERRIDE_1_RSVDB_BMSK                              0xffe00000
#define HWIO_DRE_DRE_SYNDROME_QSB_READ_REQ_OVERRIDE_1_RSVDB_SHFT                                    0x15
#define HWIO_DRE_DRE_SYNDROME_QSB_READ_REQ_OVERRIDE_1_ADEVICE_BMSK                              0x100000
#define HWIO_DRE_DRE_SYNDROME_QSB_READ_REQ_OVERRIDE_1_ADEVICE_SHFT                                  0x14
#define HWIO_DRE_DRE_SYNDROME_QSB_READ_REQ_OVERRIDE_1_ACACHEOPTYPE_BMSK                          0xf0000
#define HWIO_DRE_DRE_SYNDROME_QSB_READ_REQ_OVERRIDE_1_ACACHEOPTYPE_SHFT                             0x10
#define HWIO_DRE_DRE_SYNDROME_QSB_READ_REQ_OVERRIDE_1_RSVDA_BMSK                                  0xfffc
#define HWIO_DRE_DRE_SYNDROME_QSB_READ_REQ_OVERRIDE_1_RSVDA_SHFT                                     0x2
#define HWIO_DRE_DRE_SYNDROME_QSB_READ_REQ_OVERRIDE_1_OVERRIDE_ADEVICE_BMSK                          0x2
#define HWIO_DRE_DRE_SYNDROME_QSB_READ_REQ_OVERRIDE_1_OVERRIDE_ADEVICE_SHFT                          0x1
#define HWIO_DRE_DRE_SYNDROME_QSB_READ_REQ_OVERRIDE_1_OVERRIDE_ACACHEOPTYPE_BMSK                     0x1
#define HWIO_DRE_DRE_SYNDROME_QSB_READ_REQ_OVERRIDE_1_OVERRIDE_ACACHEOPTYPE_SHFT                     0x0

#define HWIO_DRE_DRE_DATA_QSB_REQ_OVERRIDE_ADDR                                               (DRE_REGS_REG_BASE      + 0x00000118)
#define HWIO_DRE_DRE_DATA_QSB_REQ_OVERRIDE_PHYS                                               (DRE_REGS_REG_BASE_PHYS + 0x00000118)
#define HWIO_DRE_DRE_DATA_QSB_REQ_OVERRIDE_OFFS                                               (DRE_REGS_REG_BASE_OFFS + 0x00000118)
#define HWIO_DRE_DRE_DATA_QSB_REQ_OVERRIDE_RMSK                                               0xffffffff
#define HWIO_DRE_DRE_DATA_QSB_REQ_OVERRIDE_IN          \
        in_dword_masked(HWIO_DRE_DRE_DATA_QSB_REQ_OVERRIDE_ADDR, HWIO_DRE_DRE_DATA_QSB_REQ_OVERRIDE_RMSK)
#define HWIO_DRE_DRE_DATA_QSB_REQ_OVERRIDE_INM(m)      \
        in_dword_masked(HWIO_DRE_DRE_DATA_QSB_REQ_OVERRIDE_ADDR, m)
#define HWIO_DRE_DRE_DATA_QSB_REQ_OVERRIDE_OUT(v)      \
        out_dword(HWIO_DRE_DRE_DATA_QSB_REQ_OVERRIDE_ADDR,v)
#define HWIO_DRE_DRE_DATA_QSB_REQ_OVERRIDE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DRE_DRE_DATA_QSB_REQ_OVERRIDE_ADDR,m,v,HWIO_DRE_DRE_DATA_QSB_REQ_OVERRIDE_IN)
#define HWIO_DRE_DRE_DATA_QSB_REQ_OVERRIDE_RSVDB_BMSK                                         0xf0000000
#define HWIO_DRE_DRE_DATA_QSB_REQ_OVERRIDE_RSVDB_SHFT                                               0x1c
#define HWIO_DRE_DRE_DATA_QSB_REQ_OVERRIDE_PRIORITYLVL_BMSK                                    0xe000000
#define HWIO_DRE_DRE_DATA_QSB_REQ_OVERRIDE_PRIORITYLVL_SHFT                                         0x19
#define HWIO_DRE_DRE_DATA_QSB_REQ_OVERRIDE_AREQPRIORITY_BMSK                                   0x1c00000
#define HWIO_DRE_DRE_DATA_QSB_REQ_OVERRIDE_AREQPRIORITY_SHFT                                        0x16
#define HWIO_DRE_DRE_DATA_QSB_REQ_OVERRIDE_AINNERCACHEABLE_BMSK                                 0x200000
#define HWIO_DRE_DRE_DATA_QSB_REQ_OVERRIDE_AINNERCACHEABLE_SHFT                                     0x15
#define HWIO_DRE_DRE_DATA_QSB_REQ_OVERRIDE_ASCID_BMSK                                           0x1f0000
#define HWIO_DRE_DRE_DATA_QSB_REQ_OVERRIDE_ASCID_SHFT                                               0x10
#define HWIO_DRE_DRE_DATA_QSB_REQ_OVERRIDE_RSVDA_BMSK                                             0xfff0
#define HWIO_DRE_DRE_DATA_QSB_REQ_OVERRIDE_RSVDA_SHFT                                                0x4
#define HWIO_DRE_DRE_DATA_QSB_REQ_OVERRIDE_OVERRIDE_PRIORITYLVL_BMSK                                 0x8
#define HWIO_DRE_DRE_DATA_QSB_REQ_OVERRIDE_OVERRIDE_PRIORITYLVL_SHFT                                 0x3
#define HWIO_DRE_DRE_DATA_QSB_REQ_OVERRIDE_OVERRIDE_AREQPRIORITY_BMSK                                0x4
#define HWIO_DRE_DRE_DATA_QSB_REQ_OVERRIDE_OVERRIDE_AREQPRIORITY_SHFT                                0x2
#define HWIO_DRE_DRE_DATA_QSB_REQ_OVERRIDE_OVERRIDE_AINNERCACHEABLE_BMSK                             0x2
#define HWIO_DRE_DRE_DATA_QSB_REQ_OVERRIDE_OVERRIDE_AINNERCACHEABLE_SHFT                             0x1
#define HWIO_DRE_DRE_DATA_QSB_REQ_OVERRIDE_OVERRIDE_ASCID_BMSK                                       0x1
#define HWIO_DRE_DRE_DATA_QSB_REQ_OVERRIDE_OVERRIDE_ASCID_SHFT                                       0x0

#define HWIO_DRE_DRE_ECC_NOC_DBG_ADDR                                                         (DRE_REGS_REG_BASE      + 0x00000120)
#define HWIO_DRE_DRE_ECC_NOC_DBG_PHYS                                                         (DRE_REGS_REG_BASE_PHYS + 0x00000120)
#define HWIO_DRE_DRE_ECC_NOC_DBG_OFFS                                                         (DRE_REGS_REG_BASE_OFFS + 0x00000120)
#define HWIO_DRE_DRE_ECC_NOC_DBG_RMSK                                                          0x1ff01ff
#define HWIO_DRE_DRE_ECC_NOC_DBG_IN          \
        in_dword_masked(HWIO_DRE_DRE_ECC_NOC_DBG_ADDR, HWIO_DRE_DRE_ECC_NOC_DBG_RMSK)
#define HWIO_DRE_DRE_ECC_NOC_DBG_INM(m)      \
        in_dword_masked(HWIO_DRE_DRE_ECC_NOC_DBG_ADDR, m)
#define HWIO_DRE_DRE_ECC_NOC_DBG_QNM_NOC_TRPENDING_BMSK                                        0x1000000
#define HWIO_DRE_DRE_ECC_NOC_DBG_QNM_NOC_TRPENDING_SHFT                                             0x18
#define HWIO_DRE_DRE_ECC_NOC_DBG_QNM_NOC_RSP_WAIT_BMSK                                          0x800000
#define HWIO_DRE_DRE_ECC_NOC_DBG_QNM_NOC_RSP_WAIT_SHFT                                              0x17
#define HWIO_DRE_DRE_ECC_NOC_DBG_QNM_NOC_RSP_PBUSY_BMSK                                         0x400000
#define HWIO_DRE_DRE_ECC_NOC_DBG_QNM_NOC_RSP_PBUSY_SHFT                                             0x16
#define HWIO_DRE_DRE_ECC_NOC_DBG_QNM_NOC_RSP_IDLE_BMSK                                          0x200000
#define HWIO_DRE_DRE_ECC_NOC_DBG_QNM_NOC_RSP_IDLE_SHFT                                              0x15
#define HWIO_DRE_DRE_ECC_NOC_DBG_QNM_NOC_RSP_BUSY_BMSK                                          0x100000
#define HWIO_DRE_DRE_ECC_NOC_DBG_QNM_NOC_RSP_BUSY_SHFT                                              0x14
#define HWIO_DRE_DRE_ECC_NOC_DBG_QNM_NOC_REQ_WAIT_BMSK                                           0x80000
#define HWIO_DRE_DRE_ECC_NOC_DBG_QNM_NOC_REQ_WAIT_SHFT                                              0x13
#define HWIO_DRE_DRE_ECC_NOC_DBG_QNM_NOC_REQ_PBUSY_BMSK                                          0x40000
#define HWIO_DRE_DRE_ECC_NOC_DBG_QNM_NOC_REQ_PBUSY_SHFT                                             0x12
#define HWIO_DRE_DRE_ECC_NOC_DBG_QNM_NOC_REQ_IDLE_BMSK                                           0x20000
#define HWIO_DRE_DRE_ECC_NOC_DBG_QNM_NOC_REQ_IDLE_SHFT                                              0x11
#define HWIO_DRE_DRE_ECC_NOC_DBG_QNM_NOC_REQ_BUSY_BMSK                                           0x10000
#define HWIO_DRE_DRE_ECC_NOC_DBG_QNM_NOC_REQ_BUSY_SHFT                                              0x10
#define HWIO_DRE_DRE_ECC_NOC_DBG_QXS_ECC_TRPENDING_BMSK                                            0x100
#define HWIO_DRE_DRE_ECC_NOC_DBG_QXS_ECC_TRPENDING_SHFT                                              0x8
#define HWIO_DRE_DRE_ECC_NOC_DBG_QXS_ECC_RSP_WAIT_BMSK                                              0x80
#define HWIO_DRE_DRE_ECC_NOC_DBG_QXS_ECC_RSP_WAIT_SHFT                                               0x7
#define HWIO_DRE_DRE_ECC_NOC_DBG_QXS_ECC_RSP_PBUSY_BMSK                                             0x40
#define HWIO_DRE_DRE_ECC_NOC_DBG_QXS_ECC_RSP_PBUSY_SHFT                                              0x6
#define HWIO_DRE_DRE_ECC_NOC_DBG_QXS_ECC_RSP_IDLE_BMSK                                              0x20
#define HWIO_DRE_DRE_ECC_NOC_DBG_QXS_ECC_RSP_IDLE_SHFT                                               0x5
#define HWIO_DRE_DRE_ECC_NOC_DBG_QXS_ECC_RSP_BUSY_BMSK                                              0x10
#define HWIO_DRE_DRE_ECC_NOC_DBG_QXS_ECC_RSP_BUSY_SHFT                                               0x4
#define HWIO_DRE_DRE_ECC_NOC_DBG_QXS_ECC_REQ_WAIT_BMSK                                               0x8
#define HWIO_DRE_DRE_ECC_NOC_DBG_QXS_ECC_REQ_WAIT_SHFT                                               0x3
#define HWIO_DRE_DRE_ECC_NOC_DBG_QXS_ECC_REQ_PBUSY_BMSK                                              0x4
#define HWIO_DRE_DRE_ECC_NOC_DBG_QXS_ECC_REQ_PBUSY_SHFT                                              0x2
#define HWIO_DRE_DRE_ECC_NOC_DBG_QXS_ECC_REQ_IDLE_BMSK                                               0x2
#define HWIO_DRE_DRE_ECC_NOC_DBG_QXS_ECC_REQ_IDLE_SHFT                                               0x1
#define HWIO_DRE_DRE_ECC_NOC_DBG_QXS_ECC_REQ_BUSY_BMSK                                               0x1
#define HWIO_DRE_DRE_ECC_NOC_DBG_QXS_ECC_REQ_BUSY_SHFT                                               0x0

#define HWIO_DRE_DRE_DBG_INTERFACE_STATUS_ADDR                                                (DRE_REGS_REG_BASE      + 0x00000124)
#define HWIO_DRE_DRE_DBG_INTERFACE_STATUS_PHYS                                                (DRE_REGS_REG_BASE_PHYS + 0x00000124)
#define HWIO_DRE_DRE_DBG_INTERFACE_STATUS_OFFS                                                (DRE_REGS_REG_BASE_OFFS + 0x00000124)
#define HWIO_DRE_DRE_DBG_INTERFACE_STATUS_RMSK                                                    0xffff
#define HWIO_DRE_DRE_DBG_INTERFACE_STATUS_IN          \
        in_dword_masked(HWIO_DRE_DRE_DBG_INTERFACE_STATUS_ADDR, HWIO_DRE_DRE_DBG_INTERFACE_STATUS_RMSK)
#define HWIO_DRE_DRE_DBG_INTERFACE_STATUS_INM(m)      \
        in_dword_masked(HWIO_DRE_DRE_DBG_INTERFACE_STATUS_ADDR, m)
#define HWIO_DRE_DRE_DBG_INTERFACE_STATUS_CCU2BEDP_AVALID_BMSK                                    0x8000
#define HWIO_DRE_DRE_DBG_INTERFACE_STATUS_CCU2BEDP_AVALID_SHFT                                       0xf
#define HWIO_DRE_DRE_DBG_INTERFACE_STATUS_CCU2BEDP_AREADY_BMSK                                    0x4000
#define HWIO_DRE_DRE_DBG_INTERFACE_STATUS_CCU2BEDP_AREADY_SHFT                                       0xe
#define HWIO_DRE_DRE_DBG_INTERFACE_STATUS_CCU2SYND_AVALID_BMSK                                    0x2000
#define HWIO_DRE_DRE_DBG_INTERFACE_STATUS_CCU2SYND_AVALID_SHFT                                       0xd
#define HWIO_DRE_DRE_DBG_INTERFACE_STATUS_CCU2SYND_AREADY_BMSK                                    0x1000
#define HWIO_DRE_DRE_DBG_INTERFACE_STATUS_CCU2SYND_AREADY_SHFT                                       0xc
#define HWIO_DRE_DRE_DBG_INTERFACE_STATUS_CCU2BEDP_WREADY_BMSK                                     0x800
#define HWIO_DRE_DRE_DBG_INTERFACE_STATUS_CCU2BEDP_WREADY_SHFT                                       0xb
#define HWIO_DRE_DRE_DBG_INTERFACE_STATUS_WCH2BEDP_WVALID_BMSK                                     0x400
#define HWIO_DRE_DRE_DBG_INTERFACE_STATUS_WCH2BEDP_WVALID_SHFT                                       0xa
#define HWIO_DRE_DRE_DBG_INTERFACE_STATUS_SYND2BESP_AVALID_BMSK                                    0x200
#define HWIO_DRE_DRE_DBG_INTERFACE_STATUS_SYND2BESP_AVALID_SHFT                                      0x9
#define HWIO_DRE_DRE_DBG_INTERFACE_STATUS_SYND2BESP_AREADY_BMSK                                    0x100
#define HWIO_DRE_DRE_DBG_INTERFACE_STATUS_SYND2BESP_AREADY_SHFT                                      0x8
#define HWIO_DRE_DRE_DBG_INTERFACE_STATUS_SYND2BESP_WVALID_BMSK                                     0x80
#define HWIO_DRE_DRE_DBG_INTERFACE_STATUS_SYND2BESP_WVALID_SHFT                                      0x7
#define HWIO_DRE_DRE_DBG_INTERFACE_STATUS_SYND2BESP_WREADY_BMSK                                     0x40
#define HWIO_DRE_DRE_DBG_INTERFACE_STATUS_SYND2BESP_WREADY_SHFT                                      0x6
#define HWIO_DRE_DRE_DBG_INTERFACE_STATUS_SYND2BESP_RVALID_BMSK                                     0x20
#define HWIO_DRE_DRE_DBG_INTERFACE_STATUS_SYND2BESP_RVALID_SHFT                                      0x5
#define HWIO_DRE_DRE_DBG_INTERFACE_STATUS_SYND2BESP_RREADY_BMSK                                     0x10
#define HWIO_DRE_DRE_DBG_INTERFACE_STATUS_SYND2BESP_RREADY_SHFT                                      0x4
#define HWIO_DRE_DRE_DBG_INTERFACE_STATUS_BEDP2RCH_RVALID_BMSK                                       0x8
#define HWIO_DRE_DRE_DBG_INTERFACE_STATUS_BEDP2RCH_RVALID_SHFT                                       0x3
#define HWIO_DRE_DRE_DBG_INTERFACE_STATUS_BEDP2RCH_RREADY_BMSK                                       0x4
#define HWIO_DRE_DRE_DBG_INTERFACE_STATUS_BEDP2RCH_RREADY_SHFT                                       0x2
#define HWIO_DRE_DRE_DBG_INTERFACE_STATUS_SYND2RCH_RVALID_BMSK                                       0x2
#define HWIO_DRE_DRE_DBG_INTERFACE_STATUS_SYND2RCH_RVALID_SHFT                                       0x1
#define HWIO_DRE_DRE_DBG_INTERFACE_STATUS_SYND2RCH_RREADY_BMSK                                       0x1
#define HWIO_DRE_DRE_DBG_INTERFACE_STATUS_SYND2RCH_RREADY_SHFT                                       0x0

#define HWIO_DRE_DRE_DBG_CCU_CMD_BUF_STATUS_ADDR                                              (DRE_REGS_REG_BASE      + 0x00000130)
#define HWIO_DRE_DRE_DBG_CCU_CMD_BUF_STATUS_PHYS                                              (DRE_REGS_REG_BASE_PHYS + 0x00000130)
#define HWIO_DRE_DRE_DBG_CCU_CMD_BUF_STATUS_OFFS                                              (DRE_REGS_REG_BASE_OFFS + 0x00000130)
#define HWIO_DRE_DRE_DBG_CCU_CMD_BUF_STATUS_RMSK                                              0xffffffff
#define HWIO_DRE_DRE_DBG_CCU_CMD_BUF_STATUS_IN          \
        in_dword_masked(HWIO_DRE_DRE_DBG_CCU_CMD_BUF_STATUS_ADDR, HWIO_DRE_DRE_DBG_CCU_CMD_BUF_STATUS_RMSK)
#define HWIO_DRE_DRE_DBG_CCU_CMD_BUF_STATUS_INM(m)      \
        in_dword_masked(HWIO_DRE_DRE_DBG_CCU_CMD_BUF_STATUS_ADDR, m)
#define HWIO_DRE_DRE_DBG_CCU_CMD_BUF_STATUS_VALID_BMSK                                        0xffffffff
#define HWIO_DRE_DRE_DBG_CCU_CMD_BUF_STATUS_VALID_SHFT                                               0x0

#define HWIO_DRE_DRE_DBG_CCU_RD_BUF_INDEX_STATUS_ADDR                                         (DRE_REGS_REG_BASE      + 0x00000134)
#define HWIO_DRE_DRE_DBG_CCU_RD_BUF_INDEX_STATUS_PHYS                                         (DRE_REGS_REG_BASE_PHYS + 0x00000134)
#define HWIO_DRE_DRE_DBG_CCU_RD_BUF_INDEX_STATUS_OFFS                                         (DRE_REGS_REG_BASE_OFFS + 0x00000134)
#define HWIO_DRE_DRE_DBG_CCU_RD_BUF_INDEX_STATUS_RMSK                                         0xffffffff
#define HWIO_DRE_DRE_DBG_CCU_RD_BUF_INDEX_STATUS_IN          \
        in_dword_masked(HWIO_DRE_DRE_DBG_CCU_RD_BUF_INDEX_STATUS_ADDR, HWIO_DRE_DRE_DBG_CCU_RD_BUF_INDEX_STATUS_RMSK)
#define HWIO_DRE_DRE_DBG_CCU_RD_BUF_INDEX_STATUS_INM(m)      \
        in_dword_masked(HWIO_DRE_DRE_DBG_CCU_RD_BUF_INDEX_STATUS_ADDR, m)
#define HWIO_DRE_DRE_DBG_CCU_RD_BUF_INDEX_STATUS_VALID_BMSK                                   0xffffffff
#define HWIO_DRE_DRE_DBG_CCU_RD_BUF_INDEX_STATUS_VALID_SHFT                                          0x0

#define HWIO_DRE_DRE_DBG_WCH_BUF_STATUS_ADDR                                                  (DRE_REGS_REG_BASE      + 0x00000138)
#define HWIO_DRE_DRE_DBG_WCH_BUF_STATUS_PHYS                                                  (DRE_REGS_REG_BASE_PHYS + 0x00000138)
#define HWIO_DRE_DRE_DBG_WCH_BUF_STATUS_OFFS                                                  (DRE_REGS_REG_BASE_OFFS + 0x00000138)
#define HWIO_DRE_DRE_DBG_WCH_BUF_STATUS_RMSK                                                  0xffffffff
#define HWIO_DRE_DRE_DBG_WCH_BUF_STATUS_IN          \
        in_dword_masked(HWIO_DRE_DRE_DBG_WCH_BUF_STATUS_ADDR, HWIO_DRE_DRE_DBG_WCH_BUF_STATUS_RMSK)
#define HWIO_DRE_DRE_DBG_WCH_BUF_STATUS_INM(m)      \
        in_dword_masked(HWIO_DRE_DRE_DBG_WCH_BUF_STATUS_ADDR, m)
#define HWIO_DRE_DRE_DBG_WCH_BUF_STATUS_VALID_BMSK                                            0xffffffff
#define HWIO_DRE_DRE_DBG_WCH_BUF_STATUS_VALID_SHFT                                                   0x0

#define HWIO_DRE_DRE_DBG_RCH_DATA_BUF_STATUS_ADDR                                             (DRE_REGS_REG_BASE      + 0x0000013c)
#define HWIO_DRE_DRE_DBG_RCH_DATA_BUF_STATUS_PHYS                                             (DRE_REGS_REG_BASE_PHYS + 0x0000013c)
#define HWIO_DRE_DRE_DBG_RCH_DATA_BUF_STATUS_OFFS                                             (DRE_REGS_REG_BASE_OFFS + 0x0000013c)
#define HWIO_DRE_DRE_DBG_RCH_DATA_BUF_STATUS_RMSK                                             0xffffffff
#define HWIO_DRE_DRE_DBG_RCH_DATA_BUF_STATUS_IN          \
        in_dword_masked(HWIO_DRE_DRE_DBG_RCH_DATA_BUF_STATUS_ADDR, HWIO_DRE_DRE_DBG_RCH_DATA_BUF_STATUS_RMSK)
#define HWIO_DRE_DRE_DBG_RCH_DATA_BUF_STATUS_INM(m)      \
        in_dword_masked(HWIO_DRE_DRE_DBG_RCH_DATA_BUF_STATUS_ADDR, m)
#define HWIO_DRE_DRE_DBG_RCH_DATA_BUF_STATUS_VALID_BMSK                                       0xffffffff
#define HWIO_DRE_DRE_DBG_RCH_DATA_BUF_STATUS_VALID_SHFT                                              0x0

#define HWIO_DRE_DRE_DBG_RCH_SYND_BUF_STATUS_ADDR                                             (DRE_REGS_REG_BASE      + 0x00000140)
#define HWIO_DRE_DRE_DBG_RCH_SYND_BUF_STATUS_PHYS                                             (DRE_REGS_REG_BASE_PHYS + 0x00000140)
#define HWIO_DRE_DRE_DBG_RCH_SYND_BUF_STATUS_OFFS                                             (DRE_REGS_REG_BASE_OFFS + 0x00000140)
#define HWIO_DRE_DRE_DBG_RCH_SYND_BUF_STATUS_RMSK                                             0xffffffff
#define HWIO_DRE_DRE_DBG_RCH_SYND_BUF_STATUS_IN          \
        in_dword_masked(HWIO_DRE_DRE_DBG_RCH_SYND_BUF_STATUS_ADDR, HWIO_DRE_DRE_DBG_RCH_SYND_BUF_STATUS_RMSK)
#define HWIO_DRE_DRE_DBG_RCH_SYND_BUF_STATUS_INM(m)      \
        in_dword_masked(HWIO_DRE_DRE_DBG_RCH_SYND_BUF_STATUS_ADDR, m)
#define HWIO_DRE_DRE_DBG_RCH_SYND_BUF_STATUS_VALID_BMSK                                       0xffffffff
#define HWIO_DRE_DRE_DBG_RCH_SYND_BUF_STATUS_VALID_SHFT                                              0x0

#define HWIO_DRE_DRE_DBG_SYND_WR_BUF_STATUS_ADDR                                              (DRE_REGS_REG_BASE      + 0x00000144)
#define HWIO_DRE_DRE_DBG_SYND_WR_BUF_STATUS_PHYS                                              (DRE_REGS_REG_BASE_PHYS + 0x00000144)
#define HWIO_DRE_DRE_DBG_SYND_WR_BUF_STATUS_OFFS                                              (DRE_REGS_REG_BASE_OFFS + 0x00000144)
#define HWIO_DRE_DRE_DBG_SYND_WR_BUF_STATUS_RMSK                                              0xffffffff
#define HWIO_DRE_DRE_DBG_SYND_WR_BUF_STATUS_IN          \
        in_dword_masked(HWIO_DRE_DRE_DBG_SYND_WR_BUF_STATUS_ADDR, HWIO_DRE_DRE_DBG_SYND_WR_BUF_STATUS_RMSK)
#define HWIO_DRE_DRE_DBG_SYND_WR_BUF_STATUS_INM(m)      \
        in_dword_masked(HWIO_DRE_DRE_DBG_SYND_WR_BUF_STATUS_ADDR, m)
#define HWIO_DRE_DRE_DBG_SYND_WR_BUF_STATUS_VALID_BMSK                                        0xffffffff
#define HWIO_DRE_DRE_DBG_SYND_WR_BUF_STATUS_VALID_SHFT                                               0x0

#define HWIO_DRE_DRE_DBG_SYND_RD_BUF_STATUS_ADDR                                              (DRE_REGS_REG_BASE      + 0x00000148)
#define HWIO_DRE_DRE_DBG_SYND_RD_BUF_STATUS_PHYS                                              (DRE_REGS_REG_BASE_PHYS + 0x00000148)
#define HWIO_DRE_DRE_DBG_SYND_RD_BUF_STATUS_OFFS                                              (DRE_REGS_REG_BASE_OFFS + 0x00000148)
#define HWIO_DRE_DRE_DBG_SYND_RD_BUF_STATUS_RMSK                                              0xffffffff
#define HWIO_DRE_DRE_DBG_SYND_RD_BUF_STATUS_IN          \
        in_dword_masked(HWIO_DRE_DRE_DBG_SYND_RD_BUF_STATUS_ADDR, HWIO_DRE_DRE_DBG_SYND_RD_BUF_STATUS_RMSK)
#define HWIO_DRE_DRE_DBG_SYND_RD_BUF_STATUS_INM(m)      \
        in_dword_masked(HWIO_DRE_DRE_DBG_SYND_RD_BUF_STATUS_ADDR, m)
#define HWIO_DRE_DRE_DBG_SYND_RD_BUF_STATUS_VALID_BMSK                                        0xffffffff
#define HWIO_DRE_DRE_DBG_SYND_RD_BUF_STATUS_VALID_SHFT                                               0x0

#define HWIO_DRE_DRE_DBG_QSB_OT_STATUS_ADDR                                                   (DRE_REGS_REG_BASE      + 0x0000014c)
#define HWIO_DRE_DRE_DBG_QSB_OT_STATUS_PHYS                                                   (DRE_REGS_REG_BASE_PHYS + 0x0000014c)
#define HWIO_DRE_DRE_DBG_QSB_OT_STATUS_OFFS                                                   (DRE_REGS_REG_BASE_OFFS + 0x0000014c)
#define HWIO_DRE_DRE_DBG_QSB_OT_STATUS_RMSK                                                   0x3f3f3f3f
#define HWIO_DRE_DRE_DBG_QSB_OT_STATUS_IN          \
        in_dword_masked(HWIO_DRE_DRE_DBG_QSB_OT_STATUS_ADDR, HWIO_DRE_DRE_DBG_QSB_OT_STATUS_RMSK)
#define HWIO_DRE_DRE_DBG_QSB_OT_STATUS_INM(m)      \
        in_dword_masked(HWIO_DRE_DRE_DBG_QSB_OT_STATUS_ADDR, m)
#define HWIO_DRE_DRE_DBG_QSB_OT_STATUS_BESP_WR_COUNT_BMSK                                     0x3f000000
#define HWIO_DRE_DRE_DBG_QSB_OT_STATUS_BESP_WR_COUNT_SHFT                                           0x18
#define HWIO_DRE_DRE_DBG_QSB_OT_STATUS_BESP_RD_COUNT_BMSK                                       0x3f0000
#define HWIO_DRE_DRE_DBG_QSB_OT_STATUS_BESP_RD_COUNT_SHFT                                           0x10
#define HWIO_DRE_DRE_DBG_QSB_OT_STATUS_BEDP_WR_COUNT_BMSK                                         0x3f00
#define HWIO_DRE_DRE_DBG_QSB_OT_STATUS_BEDP_WR_COUNT_SHFT                                            0x8
#define HWIO_DRE_DRE_DBG_QSB_OT_STATUS_BEDP_RD_COUNT_BMSK                                           0x3f
#define HWIO_DRE_DRE_DBG_QSB_OT_STATUS_BEDP_RD_COUNT_SHFT                                            0x0

#define HWIO_DRE_DRE_SPARE_REG0_CFG_ADDR                                                      (DRE_REGS_REG_BASE      + 0x00000150)
#define HWIO_DRE_DRE_SPARE_REG0_CFG_PHYS                                                      (DRE_REGS_REG_BASE_PHYS + 0x00000150)
#define HWIO_DRE_DRE_SPARE_REG0_CFG_OFFS                                                      (DRE_REGS_REG_BASE_OFFS + 0x00000150)
#define HWIO_DRE_DRE_SPARE_REG0_CFG_RMSK                                                      0xffffffff
#define HWIO_DRE_DRE_SPARE_REG0_CFG_IN          \
        in_dword_masked(HWIO_DRE_DRE_SPARE_REG0_CFG_ADDR, HWIO_DRE_DRE_SPARE_REG0_CFG_RMSK)
#define HWIO_DRE_DRE_SPARE_REG0_CFG_INM(m)      \
        in_dword_masked(HWIO_DRE_DRE_SPARE_REG0_CFG_ADDR, m)
#define HWIO_DRE_DRE_SPARE_REG0_CFG_OUT(v)      \
        out_dword(HWIO_DRE_DRE_SPARE_REG0_CFG_ADDR,v)
#define HWIO_DRE_DRE_SPARE_REG0_CFG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DRE_DRE_SPARE_REG0_CFG_ADDR,m,v,HWIO_DRE_DRE_SPARE_REG0_CFG_IN)
#define HWIO_DRE_DRE_SPARE_REG0_CFG_SR_31_0_BMSK                                              0xffffffff
#define HWIO_DRE_DRE_SPARE_REG0_CFG_SR_31_0_SHFT                                                     0x0

#define HWIO_DRE_DRE_SPARE_REG1_CFG_ADDR                                                      (DRE_REGS_REG_BASE      + 0x00000154)
#define HWIO_DRE_DRE_SPARE_REG1_CFG_PHYS                                                      (DRE_REGS_REG_BASE_PHYS + 0x00000154)
#define HWIO_DRE_DRE_SPARE_REG1_CFG_OFFS                                                      (DRE_REGS_REG_BASE_OFFS + 0x00000154)
#define HWIO_DRE_DRE_SPARE_REG1_CFG_RMSK                                                      0xffffffff
#define HWIO_DRE_DRE_SPARE_REG1_CFG_IN          \
        in_dword_masked(HWIO_DRE_DRE_SPARE_REG1_CFG_ADDR, HWIO_DRE_DRE_SPARE_REG1_CFG_RMSK)
#define HWIO_DRE_DRE_SPARE_REG1_CFG_INM(m)      \
        in_dword_masked(HWIO_DRE_DRE_SPARE_REG1_CFG_ADDR, m)
#define HWIO_DRE_DRE_SPARE_REG1_CFG_OUT(v)      \
        out_dword(HWIO_DRE_DRE_SPARE_REG1_CFG_ADDR,v)
#define HWIO_DRE_DRE_SPARE_REG1_CFG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DRE_DRE_SPARE_REG1_CFG_ADDR,m,v,HWIO_DRE_DRE_SPARE_REG1_CFG_IN)
#define HWIO_DRE_DRE_SPARE_REG1_CFG_SR_31_0_BMSK                                              0xffffffff
#define HWIO_DRE_DRE_SPARE_REG1_CFG_SR_31_0_SHFT                                                     0x0

#define HWIO_DRE_DRE_SPARE_REG2_CORE_ADDR                                                     (DRE_REGS_REG_BASE      + 0x00000158)
#define HWIO_DRE_DRE_SPARE_REG2_CORE_PHYS                                                     (DRE_REGS_REG_BASE_PHYS + 0x00000158)
#define HWIO_DRE_DRE_SPARE_REG2_CORE_OFFS                                                     (DRE_REGS_REG_BASE_OFFS + 0x00000158)
#define HWIO_DRE_DRE_SPARE_REG2_CORE_RMSK                                                     0xffffffff
#define HWIO_DRE_DRE_SPARE_REG2_CORE_IN          \
        in_dword_masked(HWIO_DRE_DRE_SPARE_REG2_CORE_ADDR, HWIO_DRE_DRE_SPARE_REG2_CORE_RMSK)
#define HWIO_DRE_DRE_SPARE_REG2_CORE_INM(m)      \
        in_dword_masked(HWIO_DRE_DRE_SPARE_REG2_CORE_ADDR, m)
#define HWIO_DRE_DRE_SPARE_REG2_CORE_OUT(v)      \
        out_dword(HWIO_DRE_DRE_SPARE_REG2_CORE_ADDR,v)
#define HWIO_DRE_DRE_SPARE_REG2_CORE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DRE_DRE_SPARE_REG2_CORE_ADDR,m,v,HWIO_DRE_DRE_SPARE_REG2_CORE_IN)
#define HWIO_DRE_DRE_SPARE_REG2_CORE_SR_31_0_BMSK                                             0xffffffff
#define HWIO_DRE_DRE_SPARE_REG2_CORE_SR_31_0_SHFT                                                    0x0

#define HWIO_DRE_DRE_SPARE_REG3_CORE_ADDR                                                     (DRE_REGS_REG_BASE      + 0x0000015c)
#define HWIO_DRE_DRE_SPARE_REG3_CORE_PHYS                                                     (DRE_REGS_REG_BASE_PHYS + 0x0000015c)
#define HWIO_DRE_DRE_SPARE_REG3_CORE_OFFS                                                     (DRE_REGS_REG_BASE_OFFS + 0x0000015c)
#define HWIO_DRE_DRE_SPARE_REG3_CORE_RMSK                                                     0xffffffff
#define HWIO_DRE_DRE_SPARE_REG3_CORE_IN          \
        in_dword_masked(HWIO_DRE_DRE_SPARE_REG3_CORE_ADDR, HWIO_DRE_DRE_SPARE_REG3_CORE_RMSK)
#define HWIO_DRE_DRE_SPARE_REG3_CORE_INM(m)      \
        in_dword_masked(HWIO_DRE_DRE_SPARE_REG3_CORE_ADDR, m)
#define HWIO_DRE_DRE_SPARE_REG3_CORE_OUT(v)      \
        out_dword(HWIO_DRE_DRE_SPARE_REG3_CORE_ADDR,v)
#define HWIO_DRE_DRE_SPARE_REG3_CORE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DRE_DRE_SPARE_REG3_CORE_ADDR,m,v,HWIO_DRE_DRE_SPARE_REG3_CORE_IN)
#define HWIO_DRE_DRE_SPARE_REG3_CORE_SR_31_0_BMSK                                             0xffffffff
#define HWIO_DRE_DRE_SPARE_REG3_CORE_SR_31_0_SHFT                                                    0x0

#define HWIO_DRE_DRE_DBG_CTRL_ADDR                                                            (DRE_REGS_REG_BASE      + 0x00000160)
#define HWIO_DRE_DRE_DBG_CTRL_PHYS                                                            (DRE_REGS_REG_BASE_PHYS + 0x00000160)
#define HWIO_DRE_DRE_DBG_CTRL_OFFS                                                            (DRE_REGS_REG_BASE_OFFS + 0x00000160)
#define HWIO_DRE_DRE_DBG_CTRL_RMSK                                                             0x1070303
#define HWIO_DRE_DRE_DBG_CTRL_IN          \
        in_dword_masked(HWIO_DRE_DRE_DBG_CTRL_ADDR, HWIO_DRE_DRE_DBG_CTRL_RMSK)
#define HWIO_DRE_DRE_DBG_CTRL_INM(m)      \
        in_dword_masked(HWIO_DRE_DRE_DBG_CTRL_ADDR, m)
#define HWIO_DRE_DRE_DBG_CTRL_OUT(v)      \
        out_dword(HWIO_DRE_DRE_DBG_CTRL_ADDR,v)
#define HWIO_DRE_DRE_DBG_CTRL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DRE_DRE_DBG_CTRL_ADDR,m,v,HWIO_DRE_DRE_DBG_CTRL_IN)
#define HWIO_DRE_DRE_DBG_CTRL_DBG_BUS_READ_EN_BMSK                                             0x1000000
#define HWIO_DRE_DRE_DBG_CTRL_DBG_BUS_READ_EN_SHFT                                                  0x18
#define HWIO_DRE_DRE_DBG_CTRL_DBG_BLOCK_SEL_BMSK                                                 0x70000
#define HWIO_DRE_DRE_DBG_CTRL_DBG_BLOCK_SEL_SHFT                                                    0x10
#define HWIO_DRE_DRE_DBG_CTRL_DBG_BUS_SEL_BMSK                                                     0x300
#define HWIO_DRE_DRE_DBG_CTRL_DBG_BUS_SEL_SHFT                                                       0x8
#define HWIO_DRE_DRE_DBG_CTRL_INTERNAL_DBG_BITS_SEL_BMSK                                             0x3
#define HWIO_DRE_DRE_DBG_CTRL_INTERNAL_DBG_BITS_SEL_SHFT                                             0x0

#define HWIO_DRE_DRE_DEBUG_TESTBUS_ADDR                                                       (DRE_REGS_REG_BASE      + 0x00000164)
#define HWIO_DRE_DRE_DEBUG_TESTBUS_PHYS                                                       (DRE_REGS_REG_BASE_PHYS + 0x00000164)
#define HWIO_DRE_DRE_DEBUG_TESTBUS_OFFS                                                       (DRE_REGS_REG_BASE_OFFS + 0x00000164)
#define HWIO_DRE_DRE_DEBUG_TESTBUS_RMSK                                                       0xffffffff
#define HWIO_DRE_DRE_DEBUG_TESTBUS_IN          \
        in_dword_masked(HWIO_DRE_DRE_DEBUG_TESTBUS_ADDR, HWIO_DRE_DRE_DEBUG_TESTBUS_RMSK)
#define HWIO_DRE_DRE_DEBUG_TESTBUS_INM(m)      \
        in_dword_masked(HWIO_DRE_DRE_DEBUG_TESTBUS_ADDR, m)
#define HWIO_DRE_DRE_DEBUG_TESTBUS_TESTBUS_DATA_BMSK                                          0xffffffff
#define HWIO_DRE_DRE_DEBUG_TESTBUS_TESTBUS_DATA_SHFT                                                 0x0

#define HWIO_DRE_DRE_WCH_FIFO_STATUS_ADDR                                                     (DRE_REGS_REG_BASE      + 0x00000168)
#define HWIO_DRE_DRE_WCH_FIFO_STATUS_PHYS                                                     (DRE_REGS_REG_BASE_PHYS + 0x00000168)
#define HWIO_DRE_DRE_WCH_FIFO_STATUS_OFFS                                                     (DRE_REGS_REG_BASE_OFFS + 0x00000168)
#define HWIO_DRE_DRE_WCH_FIFO_STATUS_RMSK                                                       0x111111
#define HWIO_DRE_DRE_WCH_FIFO_STATUS_IN          \
        in_dword_masked(HWIO_DRE_DRE_WCH_FIFO_STATUS_ADDR, HWIO_DRE_DRE_WCH_FIFO_STATUS_RMSK)
#define HWIO_DRE_DRE_WCH_FIFO_STATUS_INM(m)      \
        in_dword_masked(HWIO_DRE_DRE_WCH_FIFO_STATUS_ADDR, m)
#define HWIO_DRE_DRE_WCH_FIFO_STATUS_PCH_FULL_BMSK                                              0x100000
#define HWIO_DRE_DRE_WCH_FIFO_STATUS_PCH_FULL_SHFT                                                  0x14
#define HWIO_DRE_DRE_WCH_FIFO_STATUS_PCH_EMPTY_BMSK                                              0x10000
#define HWIO_DRE_DRE_WCH_FIFO_STATUS_PCH_EMPTY_SHFT                                                 0x10
#define HWIO_DRE_DRE_WCH_FIFO_STATUS_BCH_FULL_BMSK                                                0x1000
#define HWIO_DRE_DRE_WCH_FIFO_STATUS_BCH_FULL_SHFT                                                   0xc
#define HWIO_DRE_DRE_WCH_FIFO_STATUS_BCH_EMPTY_BMSK                                                0x100
#define HWIO_DRE_DRE_WCH_FIFO_STATUS_BCH_EMPTY_SHFT                                                  0x8
#define HWIO_DRE_DRE_WCH_FIFO_STATUS_WCH_FULL_BMSK                                                  0x10
#define HWIO_DRE_DRE_WCH_FIFO_STATUS_WCH_FULL_SHFT                                                   0x4
#define HWIO_DRE_DRE_WCH_FIFO_STATUS_WCH_EMPTY_BMSK                                                  0x1
#define HWIO_DRE_DRE_WCH_FIFO_STATUS_WCH_EMPTY_SHFT                                                  0x0

#define HWIO_DRE_DRE_QXM_ERROR_STATUS_ADDR                                                    (DRE_REGS_REG_BASE      + 0x0000016c)
#define HWIO_DRE_DRE_QXM_ERROR_STATUS_PHYS                                                    (DRE_REGS_REG_BASE_PHYS + 0x0000016c)
#define HWIO_DRE_DRE_QXM_ERROR_STATUS_OFFS                                                    (DRE_REGS_REG_BASE_OFFS + 0x0000016c)
#define HWIO_DRE_DRE_QXM_ERROR_STATUS_RMSK                                                          0x3f
#define HWIO_DRE_DRE_QXM_ERROR_STATUS_IN          \
        in_dword_masked(HWIO_DRE_DRE_QXM_ERROR_STATUS_ADDR, HWIO_DRE_DRE_QXM_ERROR_STATUS_RMSK)
#define HWIO_DRE_DRE_QXM_ERROR_STATUS_INM(m)      \
        in_dword_masked(HWIO_DRE_DRE_QXM_ERROR_STATUS_ADDR, m)
#define HWIO_DRE_DRE_QXM_ERROR_STATUS_QXM_ERROR_BMSK                                                0x3f
#define HWIO_DRE_DRE_QXM_ERROR_STATUS_QXM_ERROR_SHFT                                                 0x0

#define HWIO_DRE_DRE_PF_CLOCK_CTRL_ADDR                                                       (DRE_REGS_REG_BASE      + 0x00000200)
#define HWIO_DRE_DRE_PF_CLOCK_CTRL_PHYS                                                       (DRE_REGS_REG_BASE_PHYS + 0x00000200)
#define HWIO_DRE_DRE_PF_CLOCK_CTRL_OFFS                                                       (DRE_REGS_REG_BASE_OFFS + 0x00000200)
#define HWIO_DRE_DRE_PF_CLOCK_CTRL_RMSK                                                              0x1
#define HWIO_DRE_DRE_PF_CLOCK_CTRL_IN          \
        in_dword_masked(HWIO_DRE_DRE_PF_CLOCK_CTRL_ADDR, HWIO_DRE_DRE_PF_CLOCK_CTRL_RMSK)
#define HWIO_DRE_DRE_PF_CLOCK_CTRL_INM(m)      \
        in_dword_masked(HWIO_DRE_DRE_PF_CLOCK_CTRL_ADDR, m)
#define HWIO_DRE_DRE_PF_CLOCK_CTRL_OUT(v)      \
        out_dword(HWIO_DRE_DRE_PF_CLOCK_CTRL_ADDR,v)
#define HWIO_DRE_DRE_PF_CLOCK_CTRL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DRE_DRE_PF_CLOCK_CTRL_ADDR,m,v,HWIO_DRE_DRE_PF_CLOCK_CTRL_IN)
#define HWIO_DRE_DRE_PF_CLOCK_CTRL_EVENT_CLOCK_GATING_EN_BMSK                                        0x1
#define HWIO_DRE_DRE_PF_CLOCK_CTRL_EVENT_CLOCK_GATING_EN_SHFT                                        0x0

#define HWIO_DRE_DRE_PF_MODE_ADDR                                                             (DRE_REGS_REG_BASE      + 0x00000210)
#define HWIO_DRE_DRE_PF_MODE_PHYS                                                             (DRE_REGS_REG_BASE_PHYS + 0x00000210)
#define HWIO_DRE_DRE_PF_MODE_OFFS                                                             (DRE_REGS_REG_BASE_OFFS + 0x00000210)
#define HWIO_DRE_DRE_PF_MODE_RMSK                                                                   0x31
#define HWIO_DRE_DRE_PF_MODE_IN          \
        in_dword_masked(HWIO_DRE_DRE_PF_MODE_ADDR, HWIO_DRE_DRE_PF_MODE_RMSK)
#define HWIO_DRE_DRE_PF_MODE_INM(m)      \
        in_dword_masked(HWIO_DRE_DRE_PF_MODE_ADDR, m)
#define HWIO_DRE_DRE_PF_MODE_OUT(v)      \
        out_dword(HWIO_DRE_DRE_PF_MODE_ADDR,v)
#define HWIO_DRE_DRE_PF_MODE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DRE_DRE_PF_MODE_ADDR,m,v,HWIO_DRE_DRE_PF_MODE_IN)
#define HWIO_DRE_DRE_PF_MODE_BYTE_SCALING_BMSK                                                      0x20
#define HWIO_DRE_DRE_PF_MODE_BYTE_SCALING_SHFT                                                       0x5
#define HWIO_DRE_DRE_PF_MODE_BYTE_SCALING_SCALE_BY_BUS_WIDTH_FVAL                                    0x0
#define HWIO_DRE_DRE_PF_MODE_BYTE_SCALING_SCALE_BY_512_FVAL                                          0x1
#define HWIO_DRE_DRE_PF_MODE_BEAT_SCALING_BMSK                                                      0x10
#define HWIO_DRE_DRE_PF_MODE_BEAT_SCALING_SHFT                                                       0x4
#define HWIO_DRE_DRE_PF_MODE_BEAT_SCALING_SCALE_BY_1_FVAL                                            0x0
#define HWIO_DRE_DRE_PF_MODE_BEAT_SCALING_SCALE_BY_32_FVAL                                           0x1
#define HWIO_DRE_DRE_PF_MODE_PROFILING_EN_BMSK                                                       0x1
#define HWIO_DRE_DRE_PF_MODE_PROFILING_EN_SHFT                                                       0x0

#define HWIO_DRE_DRE_PF_MID_MASK_ADDR                                                         (DRE_REGS_REG_BASE      + 0x00000230)
#define HWIO_DRE_DRE_PF_MID_MASK_PHYS                                                         (DRE_REGS_REG_BASE_PHYS + 0x00000230)
#define HWIO_DRE_DRE_PF_MID_MASK_OFFS                                                         (DRE_REGS_REG_BASE_OFFS + 0x00000230)
#define HWIO_DRE_DRE_PF_MID_MASK_RMSK                                                             0xffff
#define HWIO_DRE_DRE_PF_MID_MASK_IN          \
        in_dword_masked(HWIO_DRE_DRE_PF_MID_MASK_ADDR, HWIO_DRE_DRE_PF_MID_MASK_RMSK)
#define HWIO_DRE_DRE_PF_MID_MASK_INM(m)      \
        in_dword_masked(HWIO_DRE_DRE_PF_MID_MASK_ADDR, m)
#define HWIO_DRE_DRE_PF_MID_MASK_OUT(v)      \
        out_dword(HWIO_DRE_DRE_PF_MID_MASK_ADDR,v)
#define HWIO_DRE_DRE_PF_MID_MASK_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DRE_DRE_PF_MID_MASK_ADDR,m,v,HWIO_DRE_DRE_PF_MID_MASK_IN)
#define HWIO_DRE_DRE_PF_MID_MASK_MID_MASK_BMSK                                                    0xffff
#define HWIO_DRE_DRE_PF_MID_MASK_MID_MASK_SHFT                                                       0x0

#define HWIO_DRE_DRE_PF_MID_MATCH_ADDR                                                        (DRE_REGS_REG_BASE      + 0x00000234)
#define HWIO_DRE_DRE_PF_MID_MATCH_PHYS                                                        (DRE_REGS_REG_BASE_PHYS + 0x00000234)
#define HWIO_DRE_DRE_PF_MID_MATCH_OFFS                                                        (DRE_REGS_REG_BASE_OFFS + 0x00000234)
#define HWIO_DRE_DRE_PF_MID_MATCH_RMSK                                                            0xffff
#define HWIO_DRE_DRE_PF_MID_MATCH_IN          \
        in_dword_masked(HWIO_DRE_DRE_PF_MID_MATCH_ADDR, HWIO_DRE_DRE_PF_MID_MATCH_RMSK)
#define HWIO_DRE_DRE_PF_MID_MATCH_INM(m)      \
        in_dword_masked(HWIO_DRE_DRE_PF_MID_MATCH_ADDR, m)
#define HWIO_DRE_DRE_PF_MID_MATCH_OUT(v)      \
        out_dword(HWIO_DRE_DRE_PF_MID_MATCH_ADDR,v)
#define HWIO_DRE_DRE_PF_MID_MATCH_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DRE_DRE_PF_MID_MATCH_ADDR,m,v,HWIO_DRE_DRE_PF_MID_MATCH_IN)
#define HWIO_DRE_DRE_PF_MID_MATCH_MID_MATCH_BMSK                                                  0xffff
#define HWIO_DRE_DRE_PF_MID_MATCH_MID_MATCH_SHFT                                                     0x0

#define HWIO_DRE_DRE_PF_MID_INV_MATCH_ADDR                                                    (DRE_REGS_REG_BASE      + 0x00000238)
#define HWIO_DRE_DRE_PF_MID_INV_MATCH_PHYS                                                    (DRE_REGS_REG_BASE_PHYS + 0x00000238)
#define HWIO_DRE_DRE_PF_MID_INV_MATCH_OFFS                                                    (DRE_REGS_REG_BASE_OFFS + 0x00000238)
#define HWIO_DRE_DRE_PF_MID_INV_MATCH_RMSK                                                           0x1
#define HWIO_DRE_DRE_PF_MID_INV_MATCH_IN          \
        in_dword_masked(HWIO_DRE_DRE_PF_MID_INV_MATCH_ADDR, HWIO_DRE_DRE_PF_MID_INV_MATCH_RMSK)
#define HWIO_DRE_DRE_PF_MID_INV_MATCH_INM(m)      \
        in_dword_masked(HWIO_DRE_DRE_PF_MID_INV_MATCH_ADDR, m)
#define HWIO_DRE_DRE_PF_MID_INV_MATCH_OUT(v)      \
        out_dword(HWIO_DRE_DRE_PF_MID_INV_MATCH_ADDR,v)
#define HWIO_DRE_DRE_PF_MID_INV_MATCH_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DRE_DRE_PF_MID_INV_MATCH_ADDR,m,v,HWIO_DRE_DRE_PF_MID_INV_MATCH_IN)
#define HWIO_DRE_DRE_PF_MID_INV_MATCH_MID_INV_MATCH_BMSK                                             0x1
#define HWIO_DRE_DRE_PF_MID_INV_MATCH_MID_INV_MATCH_SHFT                                             0x0

#define HWIO_DRE_DRE_PF_ADDR_MASK_LOWER_ADDR                                                  (DRE_REGS_REG_BASE      + 0x00000240)
#define HWIO_DRE_DRE_PF_ADDR_MASK_LOWER_PHYS                                                  (DRE_REGS_REG_BASE_PHYS + 0x00000240)
#define HWIO_DRE_DRE_PF_ADDR_MASK_LOWER_OFFS                                                  (DRE_REGS_REG_BASE_OFFS + 0x00000240)
#define HWIO_DRE_DRE_PF_ADDR_MASK_LOWER_RMSK                                                  0xffffffff
#define HWIO_DRE_DRE_PF_ADDR_MASK_LOWER_IN          \
        in_dword_masked(HWIO_DRE_DRE_PF_ADDR_MASK_LOWER_ADDR, HWIO_DRE_DRE_PF_ADDR_MASK_LOWER_RMSK)
#define HWIO_DRE_DRE_PF_ADDR_MASK_LOWER_INM(m)      \
        in_dword_masked(HWIO_DRE_DRE_PF_ADDR_MASK_LOWER_ADDR, m)
#define HWIO_DRE_DRE_PF_ADDR_MASK_LOWER_OUT(v)      \
        out_dword(HWIO_DRE_DRE_PF_ADDR_MASK_LOWER_ADDR,v)
#define HWIO_DRE_DRE_PF_ADDR_MASK_LOWER_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DRE_DRE_PF_ADDR_MASK_LOWER_ADDR,m,v,HWIO_DRE_DRE_PF_ADDR_MASK_LOWER_IN)
#define HWIO_DRE_DRE_PF_ADDR_MASK_LOWER_ADDR_MASK_LOWER_BMSK                                  0xffffffff
#define HWIO_DRE_DRE_PF_ADDR_MASK_LOWER_ADDR_MASK_LOWER_SHFT                                         0x0

#define HWIO_DRE_DRE_PF_ADDR_MASK_UPPER_ADDR                                                  (DRE_REGS_REG_BASE      + 0x00000244)
#define HWIO_DRE_DRE_PF_ADDR_MASK_UPPER_PHYS                                                  (DRE_REGS_REG_BASE_PHYS + 0x00000244)
#define HWIO_DRE_DRE_PF_ADDR_MASK_UPPER_OFFS                                                  (DRE_REGS_REG_BASE_OFFS + 0x00000244)
#define HWIO_DRE_DRE_PF_ADDR_MASK_UPPER_RMSK                                                         0xf
#define HWIO_DRE_DRE_PF_ADDR_MASK_UPPER_IN          \
        in_dword_masked(HWIO_DRE_DRE_PF_ADDR_MASK_UPPER_ADDR, HWIO_DRE_DRE_PF_ADDR_MASK_UPPER_RMSK)
#define HWIO_DRE_DRE_PF_ADDR_MASK_UPPER_INM(m)      \
        in_dword_masked(HWIO_DRE_DRE_PF_ADDR_MASK_UPPER_ADDR, m)
#define HWIO_DRE_DRE_PF_ADDR_MASK_UPPER_OUT(v)      \
        out_dword(HWIO_DRE_DRE_PF_ADDR_MASK_UPPER_ADDR,v)
#define HWIO_DRE_DRE_PF_ADDR_MASK_UPPER_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DRE_DRE_PF_ADDR_MASK_UPPER_ADDR,m,v,HWIO_DRE_DRE_PF_ADDR_MASK_UPPER_IN)
#define HWIO_DRE_DRE_PF_ADDR_MASK_UPPER_ADDR_MASK_UPPER_BMSK                                         0xf
#define HWIO_DRE_DRE_PF_ADDR_MASK_UPPER_ADDR_MASK_UPPER_SHFT                                         0x0

#define HWIO_DRE_DRE_PF_ADDR_MATCH_LOWER_ADDR                                                 (DRE_REGS_REG_BASE      + 0x00000248)
#define HWIO_DRE_DRE_PF_ADDR_MATCH_LOWER_PHYS                                                 (DRE_REGS_REG_BASE_PHYS + 0x00000248)
#define HWIO_DRE_DRE_PF_ADDR_MATCH_LOWER_OFFS                                                 (DRE_REGS_REG_BASE_OFFS + 0x00000248)
#define HWIO_DRE_DRE_PF_ADDR_MATCH_LOWER_RMSK                                                 0xffffffff
#define HWIO_DRE_DRE_PF_ADDR_MATCH_LOWER_IN          \
        in_dword_masked(HWIO_DRE_DRE_PF_ADDR_MATCH_LOWER_ADDR, HWIO_DRE_DRE_PF_ADDR_MATCH_LOWER_RMSK)
#define HWIO_DRE_DRE_PF_ADDR_MATCH_LOWER_INM(m)      \
        in_dword_masked(HWIO_DRE_DRE_PF_ADDR_MATCH_LOWER_ADDR, m)
#define HWIO_DRE_DRE_PF_ADDR_MATCH_LOWER_OUT(v)      \
        out_dword(HWIO_DRE_DRE_PF_ADDR_MATCH_LOWER_ADDR,v)
#define HWIO_DRE_DRE_PF_ADDR_MATCH_LOWER_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DRE_DRE_PF_ADDR_MATCH_LOWER_ADDR,m,v,HWIO_DRE_DRE_PF_ADDR_MATCH_LOWER_IN)
#define HWIO_DRE_DRE_PF_ADDR_MATCH_LOWER_ADDR_MATCH_LOWER_BMSK                                0xffffffff
#define HWIO_DRE_DRE_PF_ADDR_MATCH_LOWER_ADDR_MATCH_LOWER_SHFT                                       0x0

#define HWIO_DRE_DRE_PF_ADDR_MATCH_UPPER_ADDR                                                 (DRE_REGS_REG_BASE      + 0x0000024c)
#define HWIO_DRE_DRE_PF_ADDR_MATCH_UPPER_PHYS                                                 (DRE_REGS_REG_BASE_PHYS + 0x0000024c)
#define HWIO_DRE_DRE_PF_ADDR_MATCH_UPPER_OFFS                                                 (DRE_REGS_REG_BASE_OFFS + 0x0000024c)
#define HWIO_DRE_DRE_PF_ADDR_MATCH_UPPER_RMSK                                                        0xf
#define HWIO_DRE_DRE_PF_ADDR_MATCH_UPPER_IN          \
        in_dword_masked(HWIO_DRE_DRE_PF_ADDR_MATCH_UPPER_ADDR, HWIO_DRE_DRE_PF_ADDR_MATCH_UPPER_RMSK)
#define HWIO_DRE_DRE_PF_ADDR_MATCH_UPPER_INM(m)      \
        in_dword_masked(HWIO_DRE_DRE_PF_ADDR_MATCH_UPPER_ADDR, m)
#define HWIO_DRE_DRE_PF_ADDR_MATCH_UPPER_OUT(v)      \
        out_dword(HWIO_DRE_DRE_PF_ADDR_MATCH_UPPER_ADDR,v)
#define HWIO_DRE_DRE_PF_ADDR_MATCH_UPPER_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DRE_DRE_PF_ADDR_MATCH_UPPER_ADDR,m,v,HWIO_DRE_DRE_PF_ADDR_MATCH_UPPER_IN)
#define HWIO_DRE_DRE_PF_ADDR_MATCH_UPPER_ADDR_MATCH_UPPER_BMSK                                       0xf
#define HWIO_DRE_DRE_PF_ADDR_MATCH_UPPER_ADDR_MATCH_UPPER_SHFT                                       0x0

#define HWIO_DRE_DRE_PF_ADDR_INV_MATCH_ADDR                                                   (DRE_REGS_REG_BASE      + 0x00000250)
#define HWIO_DRE_DRE_PF_ADDR_INV_MATCH_PHYS                                                   (DRE_REGS_REG_BASE_PHYS + 0x00000250)
#define HWIO_DRE_DRE_PF_ADDR_INV_MATCH_OFFS                                                   (DRE_REGS_REG_BASE_OFFS + 0x00000250)
#define HWIO_DRE_DRE_PF_ADDR_INV_MATCH_RMSK                                                          0x1
#define HWIO_DRE_DRE_PF_ADDR_INV_MATCH_IN          \
        in_dword_masked(HWIO_DRE_DRE_PF_ADDR_INV_MATCH_ADDR, HWIO_DRE_DRE_PF_ADDR_INV_MATCH_RMSK)
#define HWIO_DRE_DRE_PF_ADDR_INV_MATCH_INM(m)      \
        in_dword_masked(HWIO_DRE_DRE_PF_ADDR_INV_MATCH_ADDR, m)
#define HWIO_DRE_DRE_PF_ADDR_INV_MATCH_OUT(v)      \
        out_dword(HWIO_DRE_DRE_PF_ADDR_INV_MATCH_ADDR,v)
#define HWIO_DRE_DRE_PF_ADDR_INV_MATCH_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DRE_DRE_PF_ADDR_INV_MATCH_ADDR,m,v,HWIO_DRE_DRE_PF_ADDR_INV_MATCH_IN)
#define HWIO_DRE_DRE_PF_ADDR_INV_MATCH_ADDR_INV_MATCH_BMSK                                           0x1
#define HWIO_DRE_DRE_PF_ADDR_INV_MATCH_ADDR_INV_MATCH_SHFT                                           0x0

#define HWIO_DRE_DRE_PF_ACH_MASK_0_ADDR                                                       (DRE_REGS_REG_BASE      + 0x00000260)
#define HWIO_DRE_DRE_PF_ACH_MASK_0_PHYS                                                       (DRE_REGS_REG_BASE_PHYS + 0x00000260)
#define HWIO_DRE_DRE_PF_ACH_MASK_0_OFFS                                                       (DRE_REGS_REG_BASE_OFFS + 0x00000260)
#define HWIO_DRE_DRE_PF_ACH_MASK_0_RMSK                                                        0x1010101
#define HWIO_DRE_DRE_PF_ACH_MASK_0_IN          \
        in_dword_masked(HWIO_DRE_DRE_PF_ACH_MASK_0_ADDR, HWIO_DRE_DRE_PF_ACH_MASK_0_RMSK)
#define HWIO_DRE_DRE_PF_ACH_MASK_0_INM(m)      \
        in_dword_masked(HWIO_DRE_DRE_PF_ACH_MASK_0_ADDR, m)
#define HWIO_DRE_DRE_PF_ACH_MASK_0_OUT(v)      \
        out_dword(HWIO_DRE_DRE_PF_ACH_MASK_0_ADDR,v)
#define HWIO_DRE_DRE_PF_ACH_MASK_0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DRE_DRE_PF_ACH_MASK_0_ADDR,m,v,HWIO_DRE_DRE_PF_ACH_MASK_0_IN)
#define HWIO_DRE_DRE_PF_ACH_MASK_0_AINNERSHARED_MASK_BMSK                                      0x1000000
#define HWIO_DRE_DRE_PF_ACH_MASK_0_AINNERSHARED_MASK_SHFT                                           0x18
#define HWIO_DRE_DRE_PF_ACH_MASK_0_AFULL_MASK_BMSK                                               0x10000
#define HWIO_DRE_DRE_PF_ACH_MASK_0_AFULL_MASK_SHFT                                                  0x10
#define HWIO_DRE_DRE_PF_ACH_MASK_0_AEXCLUSIVE_MASK_BMSK                                            0x100
#define HWIO_DRE_DRE_PF_ACH_MASK_0_AEXCLUSIVE_MASK_SHFT                                              0x8
#define HWIO_DRE_DRE_PF_ACH_MASK_0_ABURST_MASK_BMSK                                                  0x1
#define HWIO_DRE_DRE_PF_ACH_MASK_0_ABURST_MASK_SHFT                                                  0x0

#define HWIO_DRE_DRE_PF_ACH_MATCH_0_ADDR                                                      (DRE_REGS_REG_BASE      + 0x00000264)
#define HWIO_DRE_DRE_PF_ACH_MATCH_0_PHYS                                                      (DRE_REGS_REG_BASE_PHYS + 0x00000264)
#define HWIO_DRE_DRE_PF_ACH_MATCH_0_OFFS                                                      (DRE_REGS_REG_BASE_OFFS + 0x00000264)
#define HWIO_DRE_DRE_PF_ACH_MATCH_0_RMSK                                                       0x1010101
#define HWIO_DRE_DRE_PF_ACH_MATCH_0_IN          \
        in_dword_masked(HWIO_DRE_DRE_PF_ACH_MATCH_0_ADDR, HWIO_DRE_DRE_PF_ACH_MATCH_0_RMSK)
#define HWIO_DRE_DRE_PF_ACH_MATCH_0_INM(m)      \
        in_dword_masked(HWIO_DRE_DRE_PF_ACH_MATCH_0_ADDR, m)
#define HWIO_DRE_DRE_PF_ACH_MATCH_0_OUT(v)      \
        out_dword(HWIO_DRE_DRE_PF_ACH_MATCH_0_ADDR,v)
#define HWIO_DRE_DRE_PF_ACH_MATCH_0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DRE_DRE_PF_ACH_MATCH_0_ADDR,m,v,HWIO_DRE_DRE_PF_ACH_MATCH_0_IN)
#define HWIO_DRE_DRE_PF_ACH_MATCH_0_AINNERSHARED_MATCH_BMSK                                    0x1000000
#define HWIO_DRE_DRE_PF_ACH_MATCH_0_AINNERSHARED_MATCH_SHFT                                         0x18
#define HWIO_DRE_DRE_PF_ACH_MATCH_0_AFULL_MATCH_BMSK                                             0x10000
#define HWIO_DRE_DRE_PF_ACH_MATCH_0_AFULL_MATCH_SHFT                                                0x10
#define HWIO_DRE_DRE_PF_ACH_MATCH_0_AEXCLUSIVE_MATCH_BMSK                                          0x100
#define HWIO_DRE_DRE_PF_ACH_MATCH_0_AEXCLUSIVE_MATCH_SHFT                                            0x8
#define HWIO_DRE_DRE_PF_ACH_MATCH_0_ABURST_MATCH_BMSK                                                0x1
#define HWIO_DRE_DRE_PF_ACH_MATCH_0_ABURST_MATCH_SHFT                                                0x0

#define HWIO_DRE_DRE_PF_ACH_MASK_1_ADDR                                                       (DRE_REGS_REG_BASE      + 0x00000270)
#define HWIO_DRE_DRE_PF_ACH_MASK_1_PHYS                                                       (DRE_REGS_REG_BASE_PHYS + 0x00000270)
#define HWIO_DRE_DRE_PF_ACH_MASK_1_OFFS                                                       (DRE_REGS_REG_BASE_OFFS + 0x00000270)
#define HWIO_DRE_DRE_PF_ACH_MASK_1_RMSK                                                        0x13f0f01
#define HWIO_DRE_DRE_PF_ACH_MASK_1_IN          \
        in_dword_masked(HWIO_DRE_DRE_PF_ACH_MASK_1_ADDR, HWIO_DRE_DRE_PF_ACH_MASK_1_RMSK)
#define HWIO_DRE_DRE_PF_ACH_MASK_1_INM(m)      \
        in_dword_masked(HWIO_DRE_DRE_PF_ACH_MASK_1_ADDR, m)
#define HWIO_DRE_DRE_PF_ACH_MASK_1_OUT(v)      \
        out_dword(HWIO_DRE_DRE_PF_ACH_MASK_1_ADDR,v)
#define HWIO_DRE_DRE_PF_ACH_MASK_1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DRE_DRE_PF_ACH_MASK_1_ADDR,m,v,HWIO_DRE_DRE_PF_ACH_MASK_1_IN)
#define HWIO_DRE_DRE_PF_ACH_MASK_1_AMSSSELFAUTH_MASK_BMSK                                      0x1000000
#define HWIO_DRE_DRE_PF_ACH_MASK_1_AMSSSELFAUTH_MASK_SHFT                                           0x18
#define HWIO_DRE_DRE_PF_ACH_MASK_1_AMEMTYPE_MASK_BMSK                                           0x3f0000
#define HWIO_DRE_DRE_PF_ACH_MASK_1_AMEMTYPE_MASK_SHFT                                               0x10
#define HWIO_DRE_DRE_PF_ACH_MASK_1_ALEN_MASK_BMSK                                                  0xf00
#define HWIO_DRE_DRE_PF_ACH_MASK_1_ALEN_MASK_SHFT                                                    0x8
#define HWIO_DRE_DRE_PF_ACH_MASK_1_AINST_MASK_BMSK                                                   0x1
#define HWIO_DRE_DRE_PF_ACH_MASK_1_AINST_MASK_SHFT                                                   0x0

#define HWIO_DRE_DRE_PF_ACH_MATCH_1_ADDR                                                      (DRE_REGS_REG_BASE      + 0x00000274)
#define HWIO_DRE_DRE_PF_ACH_MATCH_1_PHYS                                                      (DRE_REGS_REG_BASE_PHYS + 0x00000274)
#define HWIO_DRE_DRE_PF_ACH_MATCH_1_OFFS                                                      (DRE_REGS_REG_BASE_OFFS + 0x00000274)
#define HWIO_DRE_DRE_PF_ACH_MATCH_1_RMSK                                                       0x13f0f01
#define HWIO_DRE_DRE_PF_ACH_MATCH_1_IN          \
        in_dword_masked(HWIO_DRE_DRE_PF_ACH_MATCH_1_ADDR, HWIO_DRE_DRE_PF_ACH_MATCH_1_RMSK)
#define HWIO_DRE_DRE_PF_ACH_MATCH_1_INM(m)      \
        in_dword_masked(HWIO_DRE_DRE_PF_ACH_MATCH_1_ADDR, m)
#define HWIO_DRE_DRE_PF_ACH_MATCH_1_OUT(v)      \
        out_dword(HWIO_DRE_DRE_PF_ACH_MATCH_1_ADDR,v)
#define HWIO_DRE_DRE_PF_ACH_MATCH_1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DRE_DRE_PF_ACH_MATCH_1_ADDR,m,v,HWIO_DRE_DRE_PF_ACH_MATCH_1_IN)
#define HWIO_DRE_DRE_PF_ACH_MATCH_1_AMSSSELFAUTH_MATCH_BMSK                                    0x1000000
#define HWIO_DRE_DRE_PF_ACH_MATCH_1_AMSSSELFAUTH_MATCH_SHFT                                         0x18
#define HWIO_DRE_DRE_PF_ACH_MATCH_1_AMEMTYPE_MATCH_BMSK                                         0x3f0000
#define HWIO_DRE_DRE_PF_ACH_MATCH_1_AMEMTYPE_MATCH_SHFT                                             0x10
#define HWIO_DRE_DRE_PF_ACH_MATCH_1_ALEN_MATCH_BMSK                                                0xf00
#define HWIO_DRE_DRE_PF_ACH_MATCH_1_ALEN_MATCH_SHFT                                                  0x8
#define HWIO_DRE_DRE_PF_ACH_MATCH_1_AINST_MATCH_BMSK                                                 0x1
#define HWIO_DRE_DRE_PF_ACH_MATCH_1_AINST_MATCH_SHFT                                                 0x0

#define HWIO_DRE_DRE_PF_ACH_INV_MATCH_1_ADDR                                                  (DRE_REGS_REG_BASE      + 0x00000278)
#define HWIO_DRE_DRE_PF_ACH_INV_MATCH_1_PHYS                                                  (DRE_REGS_REG_BASE_PHYS + 0x00000278)
#define HWIO_DRE_DRE_PF_ACH_INV_MATCH_1_OFFS                                                  (DRE_REGS_REG_BASE_OFFS + 0x00000278)
#define HWIO_DRE_DRE_PF_ACH_INV_MATCH_1_RMSK                                                     0x10100
#define HWIO_DRE_DRE_PF_ACH_INV_MATCH_1_IN          \
        in_dword_masked(HWIO_DRE_DRE_PF_ACH_INV_MATCH_1_ADDR, HWIO_DRE_DRE_PF_ACH_INV_MATCH_1_RMSK)
#define HWIO_DRE_DRE_PF_ACH_INV_MATCH_1_INM(m)      \
        in_dword_masked(HWIO_DRE_DRE_PF_ACH_INV_MATCH_1_ADDR, m)
#define HWIO_DRE_DRE_PF_ACH_INV_MATCH_1_OUT(v)      \
        out_dword(HWIO_DRE_DRE_PF_ACH_INV_MATCH_1_ADDR,v)
#define HWIO_DRE_DRE_PF_ACH_INV_MATCH_1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DRE_DRE_PF_ACH_INV_MATCH_1_ADDR,m,v,HWIO_DRE_DRE_PF_ACH_INV_MATCH_1_IN)
#define HWIO_DRE_DRE_PF_ACH_INV_MATCH_1_AMEMTYPE_INV_MATCH_BMSK                                  0x10000
#define HWIO_DRE_DRE_PF_ACH_INV_MATCH_1_AMEMTYPE_INV_MATCH_SHFT                                     0x10
#define HWIO_DRE_DRE_PF_ACH_INV_MATCH_1_ALEN_INV_MATCH_BMSK                                        0x100
#define HWIO_DRE_DRE_PF_ACH_INV_MATCH_1_ALEN_INV_MATCH_SHFT                                          0x8

#define HWIO_DRE_DRE_PF_ACH_MASK_2_ADDR                                                       (DRE_REGS_REG_BASE      + 0x00000280)
#define HWIO_DRE_DRE_PF_ACH_MASK_2_PHYS                                                       (DRE_REGS_REG_BASE_PHYS + 0x00000280)
#define HWIO_DRE_DRE_PF_ACH_MASK_2_OFFS                                                       (DRE_REGS_REG_BASE_OFFS + 0x00000280)
#define HWIO_DRE_DRE_PF_ACH_MASK_2_RMSK                                                        0x1010101
#define HWIO_DRE_DRE_PF_ACH_MASK_2_IN          \
        in_dword_masked(HWIO_DRE_DRE_PF_ACH_MASK_2_ADDR, HWIO_DRE_DRE_PF_ACH_MASK_2_RMSK)
#define HWIO_DRE_DRE_PF_ACH_MASK_2_INM(m)      \
        in_dword_masked(HWIO_DRE_DRE_PF_ACH_MASK_2_ADDR, m)
#define HWIO_DRE_DRE_PF_ACH_MASK_2_OUT(v)      \
        out_dword(HWIO_DRE_DRE_PF_ACH_MASK_2_ADDR,v)
#define HWIO_DRE_DRE_PF_ACH_MASK_2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DRE_DRE_PF_ACH_MASK_2_ADDR,m,v,HWIO_DRE_DRE_PF_ACH_MASK_2_IN)
#define HWIO_DRE_DRE_PF_ACH_MASK_2_APROTNS_MASK_BMSK                                           0x1000000
#define HWIO_DRE_DRE_PF_ACH_MASK_2_APROTNS_MASK_SHFT                                                0x18
#define HWIO_DRE_DRE_PF_ACH_MASK_2_APRIV_MASK_BMSK                                               0x10000
#define HWIO_DRE_DRE_PF_ACH_MASK_2_APRIV_MASK_SHFT                                                  0x10
#define HWIO_DRE_DRE_PF_ACH_MASK_2_APORTMREL_MASK_BMSK                                             0x100
#define HWIO_DRE_DRE_PF_ACH_MASK_2_APORTMREL_MASK_SHFT                                               0x8
#define HWIO_DRE_DRE_PF_ACH_MASK_2_AOOO_MASK_BMSK                                                    0x1
#define HWIO_DRE_DRE_PF_ACH_MASK_2_AOOO_MASK_SHFT                                                    0x0

#define HWIO_DRE_DRE_PF_ACH_MATCH_2_ADDR                                                      (DRE_REGS_REG_BASE      + 0x00000284)
#define HWIO_DRE_DRE_PF_ACH_MATCH_2_PHYS                                                      (DRE_REGS_REG_BASE_PHYS + 0x00000284)
#define HWIO_DRE_DRE_PF_ACH_MATCH_2_OFFS                                                      (DRE_REGS_REG_BASE_OFFS + 0x00000284)
#define HWIO_DRE_DRE_PF_ACH_MATCH_2_RMSK                                                       0x1010101
#define HWIO_DRE_DRE_PF_ACH_MATCH_2_IN          \
        in_dword_masked(HWIO_DRE_DRE_PF_ACH_MATCH_2_ADDR, HWIO_DRE_DRE_PF_ACH_MATCH_2_RMSK)
#define HWIO_DRE_DRE_PF_ACH_MATCH_2_INM(m)      \
        in_dword_masked(HWIO_DRE_DRE_PF_ACH_MATCH_2_ADDR, m)
#define HWIO_DRE_DRE_PF_ACH_MATCH_2_OUT(v)      \
        out_dword(HWIO_DRE_DRE_PF_ACH_MATCH_2_ADDR,v)
#define HWIO_DRE_DRE_PF_ACH_MATCH_2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DRE_DRE_PF_ACH_MATCH_2_ADDR,m,v,HWIO_DRE_DRE_PF_ACH_MATCH_2_IN)
#define HWIO_DRE_DRE_PF_ACH_MATCH_2_APROTNS_MATCH_BMSK                                         0x1000000
#define HWIO_DRE_DRE_PF_ACH_MATCH_2_APROTNS_MATCH_SHFT                                              0x18
#define HWIO_DRE_DRE_PF_ACH_MATCH_2_APRIV_MATCH_BMSK                                             0x10000
#define HWIO_DRE_DRE_PF_ACH_MATCH_2_APRIV_MATCH_SHFT                                                0x10
#define HWIO_DRE_DRE_PF_ACH_MATCH_2_APORTMREL_MATCH_BMSK                                           0x100
#define HWIO_DRE_DRE_PF_ACH_MATCH_2_APORTMREL_MATCH_SHFT                                             0x8
#define HWIO_DRE_DRE_PF_ACH_MATCH_2_AOOO_MATCH_BMSK                                                  0x1
#define HWIO_DRE_DRE_PF_ACH_MATCH_2_AOOO_MATCH_SHFT                                                  0x0

#define HWIO_DRE_DRE_PF_ACH_MASK_3_ADDR                                                       (DRE_REGS_REG_BASE      + 0x00000290)
#define HWIO_DRE_DRE_PF_ACH_MASK_3_PHYS                                                       (DRE_REGS_REG_BASE_PHYS + 0x00000290)
#define HWIO_DRE_DRE_PF_ACH_MASK_3_OFFS                                                       (DRE_REGS_REG_BASE_OFFS + 0x00000290)
#define HWIO_DRE_DRE_PF_ACH_MASK_3_RMSK                                                        0x1070101
#define HWIO_DRE_DRE_PF_ACH_MASK_3_IN          \
        in_dword_masked(HWIO_DRE_DRE_PF_ACH_MASK_3_ADDR, HWIO_DRE_DRE_PF_ACH_MASK_3_RMSK)
#define HWIO_DRE_DRE_PF_ACH_MASK_3_INM(m)      \
        in_dword_masked(HWIO_DRE_DRE_PF_ACH_MASK_3_ADDR, m)
#define HWIO_DRE_DRE_PF_ACH_MASK_3_OUT(v)      \
        out_dword(HWIO_DRE_DRE_PF_ACH_MASK_3_ADDR,v)
#define HWIO_DRE_DRE_PF_ACH_MASK_3_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DRE_DRE_PF_ACH_MASK_3_ADDR,m,v,HWIO_DRE_DRE_PF_ACH_MASK_3_IN)
#define HWIO_DRE_DRE_PF_ACH_MASK_3_ASHARED_MASK_BMSK                                           0x1000000
#define HWIO_DRE_DRE_PF_ACH_MASK_3_ASHARED_MASK_SHFT                                                0x18
#define HWIO_DRE_DRE_PF_ACH_MASK_3_AREQPRIORITY_MASK_BMSK                                        0x70000
#define HWIO_DRE_DRE_PF_ACH_MASK_3_AREQPRIORITY_MASK_SHFT                                           0x10
#define HWIO_DRE_DRE_PF_ACH_MASK_3_AREDIRECT_MASK_BMSK                                             0x100
#define HWIO_DRE_DRE_PF_ACH_MASK_3_AREDIRECT_MASK_SHFT                                               0x8
#define HWIO_DRE_DRE_PF_ACH_MASK_3_ARDBEATNDXEN_MASK_BMSK                                            0x1
#define HWIO_DRE_DRE_PF_ACH_MASK_3_ARDBEATNDXEN_MASK_SHFT                                            0x0

#define HWIO_DRE_DRE_PF_ACH_MATCH_3_ADDR                                                      (DRE_REGS_REG_BASE      + 0x00000294)
#define HWIO_DRE_DRE_PF_ACH_MATCH_3_PHYS                                                      (DRE_REGS_REG_BASE_PHYS + 0x00000294)
#define HWIO_DRE_DRE_PF_ACH_MATCH_3_OFFS                                                      (DRE_REGS_REG_BASE_OFFS + 0x00000294)
#define HWIO_DRE_DRE_PF_ACH_MATCH_3_RMSK                                                       0x1070101
#define HWIO_DRE_DRE_PF_ACH_MATCH_3_IN          \
        in_dword_masked(HWIO_DRE_DRE_PF_ACH_MATCH_3_ADDR, HWIO_DRE_DRE_PF_ACH_MATCH_3_RMSK)
#define HWIO_DRE_DRE_PF_ACH_MATCH_3_INM(m)      \
        in_dword_masked(HWIO_DRE_DRE_PF_ACH_MATCH_3_ADDR, m)
#define HWIO_DRE_DRE_PF_ACH_MATCH_3_OUT(v)      \
        out_dword(HWIO_DRE_DRE_PF_ACH_MATCH_3_ADDR,v)
#define HWIO_DRE_DRE_PF_ACH_MATCH_3_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DRE_DRE_PF_ACH_MATCH_3_ADDR,m,v,HWIO_DRE_DRE_PF_ACH_MATCH_3_IN)
#define HWIO_DRE_DRE_PF_ACH_MATCH_3_ASHARED_MATCH_BMSK                                         0x1000000
#define HWIO_DRE_DRE_PF_ACH_MATCH_3_ASHARED_MATCH_SHFT                                              0x18
#define HWIO_DRE_DRE_PF_ACH_MATCH_3_AREQPRIORITY_MATCH_BMSK                                      0x70000
#define HWIO_DRE_DRE_PF_ACH_MATCH_3_AREQPRIORITY_MATCH_SHFT                                         0x10
#define HWIO_DRE_DRE_PF_ACH_MATCH_3_AREDIRECT_MATCH_BMSK                                           0x100
#define HWIO_DRE_DRE_PF_ACH_MATCH_3_AREDIRECT_MATCH_SHFT                                             0x8
#define HWIO_DRE_DRE_PF_ACH_MATCH_3_ARDBEATNDXEN_MATCH_BMSK                                          0x1
#define HWIO_DRE_DRE_PF_ACH_MATCH_3_ARDBEATNDXEN_MATCH_SHFT                                          0x0

#define HWIO_DRE_DRE_PF_ACH_INV_MATCH_3_ADDR                                                  (DRE_REGS_REG_BASE      + 0x00000298)
#define HWIO_DRE_DRE_PF_ACH_INV_MATCH_3_PHYS                                                  (DRE_REGS_REG_BASE_PHYS + 0x00000298)
#define HWIO_DRE_DRE_PF_ACH_INV_MATCH_3_OFFS                                                  (DRE_REGS_REG_BASE_OFFS + 0x00000298)
#define HWIO_DRE_DRE_PF_ACH_INV_MATCH_3_RMSK                                                     0x10000
#define HWIO_DRE_DRE_PF_ACH_INV_MATCH_3_IN          \
        in_dword_masked(HWIO_DRE_DRE_PF_ACH_INV_MATCH_3_ADDR, HWIO_DRE_DRE_PF_ACH_INV_MATCH_3_RMSK)
#define HWIO_DRE_DRE_PF_ACH_INV_MATCH_3_INM(m)      \
        in_dword_masked(HWIO_DRE_DRE_PF_ACH_INV_MATCH_3_ADDR, m)
#define HWIO_DRE_DRE_PF_ACH_INV_MATCH_3_OUT(v)      \
        out_dword(HWIO_DRE_DRE_PF_ACH_INV_MATCH_3_ADDR,v)
#define HWIO_DRE_DRE_PF_ACH_INV_MATCH_3_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DRE_DRE_PF_ACH_INV_MATCH_3_ADDR,m,v,HWIO_DRE_DRE_PF_ACH_INV_MATCH_3_IN)
#define HWIO_DRE_DRE_PF_ACH_INV_MATCH_3_AREQPRIORITY_INV_MATCH_BMSK                              0x10000
#define HWIO_DRE_DRE_PF_ACH_INV_MATCH_3_AREQPRIORITY_INV_MATCH_SHFT                                 0x10

#define HWIO_DRE_DRE_PF_ACH_MASK_4_ADDR                                                       (DRE_REGS_REG_BASE      + 0x000002a0)
#define HWIO_DRE_DRE_PF_ACH_MASK_4_PHYS                                                       (DRE_REGS_REG_BASE_PHYS + 0x000002a0)
#define HWIO_DRE_DRE_PF_ACH_MASK_4_OFFS                                                       (DRE_REGS_REG_BASE_OFFS + 0x000002a0)
#define HWIO_DRE_DRE_PF_ACH_MASK_4_RMSK                                                       0xf11f0107
#define HWIO_DRE_DRE_PF_ACH_MASK_4_IN          \
        in_dword_masked(HWIO_DRE_DRE_PF_ACH_MASK_4_ADDR, HWIO_DRE_DRE_PF_ACH_MASK_4_RMSK)
#define HWIO_DRE_DRE_PF_ACH_MASK_4_INM(m)      \
        in_dword_masked(HWIO_DRE_DRE_PF_ACH_MASK_4_ADDR, m)
#define HWIO_DRE_DRE_PF_ACH_MASK_4_OUT(v)      \
        out_dword(HWIO_DRE_DRE_PF_ACH_MASK_4_ADDR,v)
#define HWIO_DRE_DRE_PF_ACH_MASK_4_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DRE_DRE_PF_ACH_MASK_4_ADDR,m,v,HWIO_DRE_DRE_PF_ACH_MASK_4_IN)
#define HWIO_DRE_DRE_PF_ACH_MASK_4_ACACHEOPTYPE_MASK_BMSK                                     0xf0000000
#define HWIO_DRE_DRE_PF_ACH_MASK_4_ACACHEOPTYPE_MASK_SHFT                                           0x1c
#define HWIO_DRE_DRE_PF_ACH_MASK_4_AWRITE_MASK_BMSK                                            0x1000000
#define HWIO_DRE_DRE_PF_ACH_MASK_4_AWRITE_MASK_SHFT                                                 0x18
#define HWIO_DRE_DRE_PF_ACH_MASK_4_AVMID_MASK_BMSK                                              0x1f0000
#define HWIO_DRE_DRE_PF_ACH_MASK_4_AVMID_MASK_SHFT                                                  0x10
#define HWIO_DRE_DRE_PF_ACH_MASK_4_ATRANSIENT_MASK_BMSK                                            0x100
#define HWIO_DRE_DRE_PF_ACH_MASK_4_ATRANSIENT_MASK_SHFT                                              0x8
#define HWIO_DRE_DRE_PF_ACH_MASK_4_ASIZE_MASK_BMSK                                                   0x7
#define HWIO_DRE_DRE_PF_ACH_MASK_4_ASIZE_MASK_SHFT                                                   0x0

#define HWIO_DRE_DRE_PF_ACH_MATCH_4_ADDR                                                      (DRE_REGS_REG_BASE      + 0x000002a4)
#define HWIO_DRE_DRE_PF_ACH_MATCH_4_PHYS                                                      (DRE_REGS_REG_BASE_PHYS + 0x000002a4)
#define HWIO_DRE_DRE_PF_ACH_MATCH_4_OFFS                                                      (DRE_REGS_REG_BASE_OFFS + 0x000002a4)
#define HWIO_DRE_DRE_PF_ACH_MATCH_4_RMSK                                                      0xf11f0107
#define HWIO_DRE_DRE_PF_ACH_MATCH_4_IN          \
        in_dword_masked(HWIO_DRE_DRE_PF_ACH_MATCH_4_ADDR, HWIO_DRE_DRE_PF_ACH_MATCH_4_RMSK)
#define HWIO_DRE_DRE_PF_ACH_MATCH_4_INM(m)      \
        in_dword_masked(HWIO_DRE_DRE_PF_ACH_MATCH_4_ADDR, m)
#define HWIO_DRE_DRE_PF_ACH_MATCH_4_OUT(v)      \
        out_dword(HWIO_DRE_DRE_PF_ACH_MATCH_4_ADDR,v)
#define HWIO_DRE_DRE_PF_ACH_MATCH_4_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DRE_DRE_PF_ACH_MATCH_4_ADDR,m,v,HWIO_DRE_DRE_PF_ACH_MATCH_4_IN)
#define HWIO_DRE_DRE_PF_ACH_MATCH_4_ACACHEOPTYPE_MATCH_BMSK                                   0xf0000000
#define HWIO_DRE_DRE_PF_ACH_MATCH_4_ACACHEOPTYPE_MATCH_SHFT                                         0x1c
#define HWIO_DRE_DRE_PF_ACH_MATCH_4_AWRITE_MATCH_BMSK                                          0x1000000
#define HWIO_DRE_DRE_PF_ACH_MATCH_4_AWRITE_MATCH_SHFT                                               0x18
#define HWIO_DRE_DRE_PF_ACH_MATCH_4_AVMID_MATCH_BMSK                                            0x1f0000
#define HWIO_DRE_DRE_PF_ACH_MATCH_4_AVMID_MATCH_SHFT                                                0x10
#define HWIO_DRE_DRE_PF_ACH_MATCH_4_ATRANSIENT_MATCH_BMSK                                          0x100
#define HWIO_DRE_DRE_PF_ACH_MATCH_4_ATRANSIENT_MATCH_SHFT                                            0x8
#define HWIO_DRE_DRE_PF_ACH_MATCH_4_ASIZE_MATCH_BMSK                                                 0x7
#define HWIO_DRE_DRE_PF_ACH_MATCH_4_ASIZE_MATCH_SHFT                                                 0x0

#define HWIO_DRE_DRE_PF_ACH_INV_MATCH_4_ADDR                                                  (DRE_REGS_REG_BASE      + 0x000002a8)
#define HWIO_DRE_DRE_PF_ACH_INV_MATCH_4_PHYS                                                  (DRE_REGS_REG_BASE_PHYS + 0x000002a8)
#define HWIO_DRE_DRE_PF_ACH_INV_MATCH_4_OFFS                                                  (DRE_REGS_REG_BASE_OFFS + 0x000002a8)
#define HWIO_DRE_DRE_PF_ACH_INV_MATCH_4_RMSK                                                  0x10010001
#define HWIO_DRE_DRE_PF_ACH_INV_MATCH_4_IN          \
        in_dword_masked(HWIO_DRE_DRE_PF_ACH_INV_MATCH_4_ADDR, HWIO_DRE_DRE_PF_ACH_INV_MATCH_4_RMSK)
#define HWIO_DRE_DRE_PF_ACH_INV_MATCH_4_INM(m)      \
        in_dword_masked(HWIO_DRE_DRE_PF_ACH_INV_MATCH_4_ADDR, m)
#define HWIO_DRE_DRE_PF_ACH_INV_MATCH_4_OUT(v)      \
        out_dword(HWIO_DRE_DRE_PF_ACH_INV_MATCH_4_ADDR,v)
#define HWIO_DRE_DRE_PF_ACH_INV_MATCH_4_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DRE_DRE_PF_ACH_INV_MATCH_4_ADDR,m,v,HWIO_DRE_DRE_PF_ACH_INV_MATCH_4_IN)
#define HWIO_DRE_DRE_PF_ACH_INV_MATCH_4_ACACHEOPTYPE_INV_MATCH_BMSK                           0x10000000
#define HWIO_DRE_DRE_PF_ACH_INV_MATCH_4_ACACHEOPTYPE_INV_MATCH_SHFT                                 0x1c
#define HWIO_DRE_DRE_PF_ACH_INV_MATCH_4_AVMID_INV_MATCH_BMSK                                     0x10000
#define HWIO_DRE_DRE_PF_ACH_INV_MATCH_4_AVMID_INV_MATCH_SHFT                                        0x10
#define HWIO_DRE_DRE_PF_ACH_INV_MATCH_4_ASIZE_INV_MATCH_BMSK                                         0x1
#define HWIO_DRE_DRE_PF_ACH_INV_MATCH_4_ASIZE_INV_MATCH_SHFT                                         0x0

#define HWIO_DRE_DRE_PF_ACH_MASK_5_ADDR                                                       (DRE_REGS_REG_BASE      + 0x000002b0)
#define HWIO_DRE_DRE_PF_ACH_MASK_5_PHYS                                                       (DRE_REGS_REG_BASE_PHYS + 0x000002b0)
#define HWIO_DRE_DRE_PF_ACH_MASK_5_OFFS                                                       (DRE_REGS_REG_BASE_OFFS + 0x000002b0)
#define HWIO_DRE_DRE_PF_ACH_MASK_5_RMSK                                                              0x1
#define HWIO_DRE_DRE_PF_ACH_MASK_5_IN          \
        in_dword_masked(HWIO_DRE_DRE_PF_ACH_MASK_5_ADDR, HWIO_DRE_DRE_PF_ACH_MASK_5_RMSK)
#define HWIO_DRE_DRE_PF_ACH_MASK_5_INM(m)      \
        in_dword_masked(HWIO_DRE_DRE_PF_ACH_MASK_5_ADDR, m)
#define HWIO_DRE_DRE_PF_ACH_MASK_5_OUT(v)      \
        out_dword(HWIO_DRE_DRE_PF_ACH_MASK_5_ADDR,v)
#define HWIO_DRE_DRE_PF_ACH_MASK_5_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DRE_DRE_PF_ACH_MASK_5_ADDR,m,v,HWIO_DRE_DRE_PF_ACH_MASK_5_IN)
#define HWIO_DRE_DRE_PF_ACH_MASK_5_ASECUREPROC_MASK_BMSK                                             0x1
#define HWIO_DRE_DRE_PF_ACH_MASK_5_ASECUREPROC_MASK_SHFT                                             0x0

#define HWIO_DRE_DRE_PF_ACH_MATCH_5_ADDR                                                      (DRE_REGS_REG_BASE      + 0x000002b4)
#define HWIO_DRE_DRE_PF_ACH_MATCH_5_PHYS                                                      (DRE_REGS_REG_BASE_PHYS + 0x000002b4)
#define HWIO_DRE_DRE_PF_ACH_MATCH_5_OFFS                                                      (DRE_REGS_REG_BASE_OFFS + 0x000002b4)
#define HWIO_DRE_DRE_PF_ACH_MATCH_5_RMSK                                                             0x1
#define HWIO_DRE_DRE_PF_ACH_MATCH_5_IN          \
        in_dword_masked(HWIO_DRE_DRE_PF_ACH_MATCH_5_ADDR, HWIO_DRE_DRE_PF_ACH_MATCH_5_RMSK)
#define HWIO_DRE_DRE_PF_ACH_MATCH_5_INM(m)      \
        in_dword_masked(HWIO_DRE_DRE_PF_ACH_MATCH_5_ADDR, m)
#define HWIO_DRE_DRE_PF_ACH_MATCH_5_OUT(v)      \
        out_dword(HWIO_DRE_DRE_PF_ACH_MATCH_5_ADDR,v)
#define HWIO_DRE_DRE_PF_ACH_MATCH_5_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DRE_DRE_PF_ACH_MATCH_5_ADDR,m,v,HWIO_DRE_DRE_PF_ACH_MATCH_5_IN)
#define HWIO_DRE_DRE_PF_ACH_MATCH_5_ASECUREPROC_MATCH_BMSK                                           0x1
#define HWIO_DRE_DRE_PF_ACH_MATCH_5_ASECUREPROC_MATCH_SHFT                                           0x0

#define HWIO_DRE_DRE_PF_BCH_MASK_ADDR                                                         (DRE_REGS_REG_BASE      + 0x00000300)
#define HWIO_DRE_DRE_PF_BCH_MASK_PHYS                                                         (DRE_REGS_REG_BASE_PHYS + 0x00000300)
#define HWIO_DRE_DRE_PF_BCH_MASK_OFFS                                                         (DRE_REGS_REG_BASE_OFFS + 0x00000300)
#define HWIO_DRE_DRE_PF_BCH_MASK_RMSK                                                                0x3
#define HWIO_DRE_DRE_PF_BCH_MASK_IN          \
        in_dword_masked(HWIO_DRE_DRE_PF_BCH_MASK_ADDR, HWIO_DRE_DRE_PF_BCH_MASK_RMSK)
#define HWIO_DRE_DRE_PF_BCH_MASK_INM(m)      \
        in_dword_masked(HWIO_DRE_DRE_PF_BCH_MASK_ADDR, m)
#define HWIO_DRE_DRE_PF_BCH_MASK_OUT(v)      \
        out_dword(HWIO_DRE_DRE_PF_BCH_MASK_ADDR,v)
#define HWIO_DRE_DRE_PF_BCH_MASK_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DRE_DRE_PF_BCH_MASK_ADDR,m,v,HWIO_DRE_DRE_PF_BCH_MASK_IN)
#define HWIO_DRE_DRE_PF_BCH_MASK_BRESP_MASK_BMSK                                                     0x3
#define HWIO_DRE_DRE_PF_BCH_MASK_BRESP_MASK_SHFT                                                     0x0

#define HWIO_DRE_DRE_PF_BCH_MATCH_ADDR                                                        (DRE_REGS_REG_BASE      + 0x00000304)
#define HWIO_DRE_DRE_PF_BCH_MATCH_PHYS                                                        (DRE_REGS_REG_BASE_PHYS + 0x00000304)
#define HWIO_DRE_DRE_PF_BCH_MATCH_OFFS                                                        (DRE_REGS_REG_BASE_OFFS + 0x00000304)
#define HWIO_DRE_DRE_PF_BCH_MATCH_RMSK                                                               0x3
#define HWIO_DRE_DRE_PF_BCH_MATCH_IN          \
        in_dword_masked(HWIO_DRE_DRE_PF_BCH_MATCH_ADDR, HWIO_DRE_DRE_PF_BCH_MATCH_RMSK)
#define HWIO_DRE_DRE_PF_BCH_MATCH_INM(m)      \
        in_dword_masked(HWIO_DRE_DRE_PF_BCH_MATCH_ADDR, m)
#define HWIO_DRE_DRE_PF_BCH_MATCH_OUT(v)      \
        out_dword(HWIO_DRE_DRE_PF_BCH_MATCH_ADDR,v)
#define HWIO_DRE_DRE_PF_BCH_MATCH_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DRE_DRE_PF_BCH_MATCH_ADDR,m,v,HWIO_DRE_DRE_PF_BCH_MATCH_IN)
#define HWIO_DRE_DRE_PF_BCH_MATCH_BRESP_MATCH_BMSK                                                   0x3
#define HWIO_DRE_DRE_PF_BCH_MATCH_BRESP_MATCH_SHFT                                                   0x0

#define HWIO_DRE_DRE_PF_BCH_INV_MATCH_ADDR                                                    (DRE_REGS_REG_BASE      + 0x00000308)
#define HWIO_DRE_DRE_PF_BCH_INV_MATCH_PHYS                                                    (DRE_REGS_REG_BASE_PHYS + 0x00000308)
#define HWIO_DRE_DRE_PF_BCH_INV_MATCH_OFFS                                                    (DRE_REGS_REG_BASE_OFFS + 0x00000308)
#define HWIO_DRE_DRE_PF_BCH_INV_MATCH_RMSK                                                           0x1
#define HWIO_DRE_DRE_PF_BCH_INV_MATCH_IN          \
        in_dword_masked(HWIO_DRE_DRE_PF_BCH_INV_MATCH_ADDR, HWIO_DRE_DRE_PF_BCH_INV_MATCH_RMSK)
#define HWIO_DRE_DRE_PF_BCH_INV_MATCH_INM(m)      \
        in_dword_masked(HWIO_DRE_DRE_PF_BCH_INV_MATCH_ADDR, m)
#define HWIO_DRE_DRE_PF_BCH_INV_MATCH_OUT(v)      \
        out_dword(HWIO_DRE_DRE_PF_BCH_INV_MATCH_ADDR,v)
#define HWIO_DRE_DRE_PF_BCH_INV_MATCH_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DRE_DRE_PF_BCH_INV_MATCH_ADDR,m,v,HWIO_DRE_DRE_PF_BCH_INV_MATCH_IN)
#define HWIO_DRE_DRE_PF_BCH_INV_MATCH_BRESP_INV_MATCH_BMSK                                           0x1
#define HWIO_DRE_DRE_PF_BCH_INV_MATCH_BRESP_INV_MATCH_SHFT                                           0x0

#define HWIO_DRE_DRE_PF_RCH_MASK_ADDR                                                         (DRE_REGS_REG_BASE      + 0x00000340)
#define HWIO_DRE_DRE_PF_RCH_MASK_PHYS                                                         (DRE_REGS_REG_BASE_PHYS + 0x00000340)
#define HWIO_DRE_DRE_PF_RCH_MASK_OFFS                                                         (DRE_REGS_REG_BASE_OFFS + 0x00000340)
#define HWIO_DRE_DRE_PF_RCH_MASK_RMSK                                                              0x103
#define HWIO_DRE_DRE_PF_RCH_MASK_IN          \
        in_dword_masked(HWIO_DRE_DRE_PF_RCH_MASK_ADDR, HWIO_DRE_DRE_PF_RCH_MASK_RMSK)
#define HWIO_DRE_DRE_PF_RCH_MASK_INM(m)      \
        in_dword_masked(HWIO_DRE_DRE_PF_RCH_MASK_ADDR, m)
#define HWIO_DRE_DRE_PF_RCH_MASK_OUT(v)      \
        out_dword(HWIO_DRE_DRE_PF_RCH_MASK_ADDR,v)
#define HWIO_DRE_DRE_PF_RCH_MASK_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DRE_DRE_PF_RCH_MASK_ADDR,m,v,HWIO_DRE_DRE_PF_RCH_MASK_IN)
#define HWIO_DRE_DRE_PF_RCH_MASK_RLAST_MASK_BMSK                                                   0x100
#define HWIO_DRE_DRE_PF_RCH_MASK_RLAST_MASK_SHFT                                                     0x8
#define HWIO_DRE_DRE_PF_RCH_MASK_RRESP_MASK_BMSK                                                     0x3
#define HWIO_DRE_DRE_PF_RCH_MASK_RRESP_MASK_SHFT                                                     0x0

#define HWIO_DRE_DRE_PF_RCH_MATCH_ADDR                                                        (DRE_REGS_REG_BASE      + 0x00000344)
#define HWIO_DRE_DRE_PF_RCH_MATCH_PHYS                                                        (DRE_REGS_REG_BASE_PHYS + 0x00000344)
#define HWIO_DRE_DRE_PF_RCH_MATCH_OFFS                                                        (DRE_REGS_REG_BASE_OFFS + 0x00000344)
#define HWIO_DRE_DRE_PF_RCH_MATCH_RMSK                                                             0x103
#define HWIO_DRE_DRE_PF_RCH_MATCH_IN          \
        in_dword_masked(HWIO_DRE_DRE_PF_RCH_MATCH_ADDR, HWIO_DRE_DRE_PF_RCH_MATCH_RMSK)
#define HWIO_DRE_DRE_PF_RCH_MATCH_INM(m)      \
        in_dword_masked(HWIO_DRE_DRE_PF_RCH_MATCH_ADDR, m)
#define HWIO_DRE_DRE_PF_RCH_MATCH_OUT(v)      \
        out_dword(HWIO_DRE_DRE_PF_RCH_MATCH_ADDR,v)
#define HWIO_DRE_DRE_PF_RCH_MATCH_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DRE_DRE_PF_RCH_MATCH_ADDR,m,v,HWIO_DRE_DRE_PF_RCH_MATCH_IN)
#define HWIO_DRE_DRE_PF_RCH_MATCH_RLAST_MATCH_BMSK                                                 0x100
#define HWIO_DRE_DRE_PF_RCH_MATCH_RLAST_MATCH_SHFT                                                   0x8
#define HWIO_DRE_DRE_PF_RCH_MATCH_RRESP_MATCH_BMSK                                                   0x3
#define HWIO_DRE_DRE_PF_RCH_MATCH_RRESP_MATCH_SHFT                                                   0x0

#define HWIO_DRE_DRE_PF_RCH_INV_MATCH_ADDR                                                    (DRE_REGS_REG_BASE      + 0x00000348)
#define HWIO_DRE_DRE_PF_RCH_INV_MATCH_PHYS                                                    (DRE_REGS_REG_BASE_PHYS + 0x00000348)
#define HWIO_DRE_DRE_PF_RCH_INV_MATCH_OFFS                                                    (DRE_REGS_REG_BASE_OFFS + 0x00000348)
#define HWIO_DRE_DRE_PF_RCH_INV_MATCH_RMSK                                                           0x1
#define HWIO_DRE_DRE_PF_RCH_INV_MATCH_IN          \
        in_dword_masked(HWIO_DRE_DRE_PF_RCH_INV_MATCH_ADDR, HWIO_DRE_DRE_PF_RCH_INV_MATCH_RMSK)
#define HWIO_DRE_DRE_PF_RCH_INV_MATCH_INM(m)      \
        in_dword_masked(HWIO_DRE_DRE_PF_RCH_INV_MATCH_ADDR, m)
#define HWIO_DRE_DRE_PF_RCH_INV_MATCH_OUT(v)      \
        out_dword(HWIO_DRE_DRE_PF_RCH_INV_MATCH_ADDR,v)
#define HWIO_DRE_DRE_PF_RCH_INV_MATCH_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DRE_DRE_PF_RCH_INV_MATCH_ADDR,m,v,HWIO_DRE_DRE_PF_RCH_INV_MATCH_IN)
#define HWIO_DRE_DRE_PF_RCH_INV_MATCH_RRESP_INV_MATCH_BMSK                                           0x1
#define HWIO_DRE_DRE_PF_RCH_INV_MATCH_RRESP_INV_MATCH_SHFT                                           0x0

#define HWIO_DRE_DRE_CCU_PROF_EVENT_n_CFG_ADDR(n)                                             (DRE_REGS_REG_BASE      + 0x00000400 + 0x4 * (n))
#define HWIO_DRE_DRE_CCU_PROF_EVENT_n_CFG_PHYS(n)                                             (DRE_REGS_REG_BASE_PHYS + 0x00000400 + 0x4 * (n))
#define HWIO_DRE_DRE_CCU_PROF_EVENT_n_CFG_OFFS(n)                                             (DRE_REGS_REG_BASE_OFFS + 0x00000400 + 0x4 * (n))
#define HWIO_DRE_DRE_CCU_PROF_EVENT_n_CFG_RMSK                                                0x8001003f
#define HWIO_DRE_DRE_CCU_PROF_EVENT_n_CFG_MAXn                                                        15
#define HWIO_DRE_DRE_CCU_PROF_EVENT_n_CFG_INI(n)        \
        in_dword_masked(HWIO_DRE_DRE_CCU_PROF_EVENT_n_CFG_ADDR(n), HWIO_DRE_DRE_CCU_PROF_EVENT_n_CFG_RMSK)
#define HWIO_DRE_DRE_CCU_PROF_EVENT_n_CFG_INMI(n,mask)    \
        in_dword_masked(HWIO_DRE_DRE_CCU_PROF_EVENT_n_CFG_ADDR(n), mask)
#define HWIO_DRE_DRE_CCU_PROF_EVENT_n_CFG_OUTI(n,val)    \
        out_dword(HWIO_DRE_DRE_CCU_PROF_EVENT_n_CFG_ADDR(n),val)
#define HWIO_DRE_DRE_CCU_PROF_EVENT_n_CFG_OUTMI(n,mask,val) \
        out_dword_masked_ns(HWIO_DRE_DRE_CCU_PROF_EVENT_n_CFG_ADDR(n),mask,val,HWIO_DRE_DRE_CCU_PROF_EVENT_n_CFG_INI(n))
#define HWIO_DRE_DRE_CCU_PROF_EVENT_n_CFG_FILTER_EN_BMSK                                      0x80000000
#define HWIO_DRE_DRE_CCU_PROF_EVENT_n_CFG_FILTER_EN_SHFT                                            0x1f
#define HWIO_DRE_DRE_CCU_PROF_EVENT_n_CFG_FILTER_SEL_BMSK                                        0x10000
#define HWIO_DRE_DRE_CCU_PROF_EVENT_n_CFG_FILTER_SEL_SHFT                                           0x10
#define HWIO_DRE_DRE_CCU_PROF_EVENT_n_CFG_EVENT_SEL_BMSK                                            0x3f
#define HWIO_DRE_DRE_CCU_PROF_EVENT_n_CFG_EVENT_SEL_SHFT                                             0x0

#define HWIO_DRE_DRE_SYND_PROF_EVENT_n_CFG_ADDR(n)                                            (DRE_REGS_REG_BASE      + 0x00000440 + 0x4 * (n))
#define HWIO_DRE_DRE_SYND_PROF_EVENT_n_CFG_PHYS(n)                                            (DRE_REGS_REG_BASE_PHYS + 0x00000440 + 0x4 * (n))
#define HWIO_DRE_DRE_SYND_PROF_EVENT_n_CFG_OFFS(n)                                            (DRE_REGS_REG_BASE_OFFS + 0x00000440 + 0x4 * (n))
#define HWIO_DRE_DRE_SYND_PROF_EVENT_n_CFG_RMSK                                               0x8001003f
#define HWIO_DRE_DRE_SYND_PROF_EVENT_n_CFG_MAXn                                                       15
#define HWIO_DRE_DRE_SYND_PROF_EVENT_n_CFG_INI(n)        \
        in_dword_masked(HWIO_DRE_DRE_SYND_PROF_EVENT_n_CFG_ADDR(n), HWIO_DRE_DRE_SYND_PROF_EVENT_n_CFG_RMSK)
#define HWIO_DRE_DRE_SYND_PROF_EVENT_n_CFG_INMI(n,mask)    \
        in_dword_masked(HWIO_DRE_DRE_SYND_PROF_EVENT_n_CFG_ADDR(n), mask)
#define HWIO_DRE_DRE_SYND_PROF_EVENT_n_CFG_OUTI(n,val)    \
        out_dword(HWIO_DRE_DRE_SYND_PROF_EVENT_n_CFG_ADDR(n),val)
#define HWIO_DRE_DRE_SYND_PROF_EVENT_n_CFG_OUTMI(n,mask,val) \
        out_dword_masked_ns(HWIO_DRE_DRE_SYND_PROF_EVENT_n_CFG_ADDR(n),mask,val,HWIO_DRE_DRE_SYND_PROF_EVENT_n_CFG_INI(n))
#define HWIO_DRE_DRE_SYND_PROF_EVENT_n_CFG_FILTER_EN_BMSK                                     0x80000000
#define HWIO_DRE_DRE_SYND_PROF_EVENT_n_CFG_FILTER_EN_SHFT                                           0x1f
#define HWIO_DRE_DRE_SYND_PROF_EVENT_n_CFG_FILTER_SEL_BMSK                                       0x10000
#define HWIO_DRE_DRE_SYND_PROF_EVENT_n_CFG_FILTER_SEL_SHFT                                          0x10
#define HWIO_DRE_DRE_SYND_PROF_EVENT_n_CFG_EVENT_SEL_BMSK                                           0x3f
#define HWIO_DRE_DRE_SYND_PROF_EVENT_n_CFG_EVENT_SEL_SHFT                                            0x0

#define HWIO_DRE_DRE_WCH_PROF_FILTER_CFG_ADDR                                                 (DRE_REGS_REG_BASE      + 0x00000480)
#define HWIO_DRE_DRE_WCH_PROF_FILTER_CFG_PHYS                                                 (DRE_REGS_REG_BASE_PHYS + 0x00000480)
#define HWIO_DRE_DRE_WCH_PROF_FILTER_CFG_OFFS                                                 (DRE_REGS_REG_BASE_OFFS + 0x00000480)
#define HWIO_DRE_DRE_WCH_PROF_FILTER_CFG_RMSK                                                     0xffff
#define HWIO_DRE_DRE_WCH_PROF_FILTER_CFG_IN          \
        in_dword_masked(HWIO_DRE_DRE_WCH_PROF_FILTER_CFG_ADDR, HWIO_DRE_DRE_WCH_PROF_FILTER_CFG_RMSK)
#define HWIO_DRE_DRE_WCH_PROF_FILTER_CFG_INM(m)      \
        in_dword_masked(HWIO_DRE_DRE_WCH_PROF_FILTER_CFG_ADDR, m)
#define HWIO_DRE_DRE_WCH_PROF_FILTER_CFG_OUT(v)      \
        out_dword(HWIO_DRE_DRE_WCH_PROF_FILTER_CFG_ADDR,v)
#define HWIO_DRE_DRE_WCH_PROF_FILTER_CFG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DRE_DRE_WCH_PROF_FILTER_CFG_ADDR,m,v,HWIO_DRE_DRE_WCH_PROF_FILTER_CFG_IN)
#define HWIO_DRE_DRE_WCH_PROF_FILTER_CFG_FILTER_EN_BMSK                                           0xffff
#define HWIO_DRE_DRE_WCH_PROF_FILTER_CFG_FILTER_EN_SHFT                                              0x0

#define HWIO_DRE_DRE_RCH_PROF_FILTER_CFG_ADDR                                                 (DRE_REGS_REG_BASE      + 0x00000484)
#define HWIO_DRE_DRE_RCH_PROF_FILTER_CFG_PHYS                                                 (DRE_REGS_REG_BASE_PHYS + 0x00000484)
#define HWIO_DRE_DRE_RCH_PROF_FILTER_CFG_OFFS                                                 (DRE_REGS_REG_BASE_OFFS + 0x00000484)
#define HWIO_DRE_DRE_RCH_PROF_FILTER_CFG_RMSK                                                     0xffff
#define HWIO_DRE_DRE_RCH_PROF_FILTER_CFG_IN          \
        in_dword_masked(HWIO_DRE_DRE_RCH_PROF_FILTER_CFG_ADDR, HWIO_DRE_DRE_RCH_PROF_FILTER_CFG_RMSK)
#define HWIO_DRE_DRE_RCH_PROF_FILTER_CFG_INM(m)      \
        in_dword_masked(HWIO_DRE_DRE_RCH_PROF_FILTER_CFG_ADDR, m)
#define HWIO_DRE_DRE_RCH_PROF_FILTER_CFG_OUT(v)      \
        out_dword(HWIO_DRE_DRE_RCH_PROF_FILTER_CFG_ADDR,v)
#define HWIO_DRE_DRE_RCH_PROF_FILTER_CFG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DRE_DRE_RCH_PROF_FILTER_CFG_ADDR,m,v,HWIO_DRE_DRE_RCH_PROF_FILTER_CFG_IN)
#define HWIO_DRE_DRE_RCH_PROF_FILTER_CFG_FILTER_EN_BMSK                                           0xffff
#define HWIO_DRE_DRE_RCH_PROF_FILTER_CFG_FILTER_EN_SHFT                                              0x0

#define HWIO_DRE_DRE_BEDP_PROF_FILTER_CFG_ADDR                                                (DRE_REGS_REG_BASE      + 0x00000488)
#define HWIO_DRE_DRE_BEDP_PROF_FILTER_CFG_PHYS                                                (DRE_REGS_REG_BASE_PHYS + 0x00000488)
#define HWIO_DRE_DRE_BEDP_PROF_FILTER_CFG_OFFS                                                (DRE_REGS_REG_BASE_OFFS + 0x00000488)
#define HWIO_DRE_DRE_BEDP_PROF_FILTER_CFG_RMSK                                                    0xffff
#define HWIO_DRE_DRE_BEDP_PROF_FILTER_CFG_IN          \
        in_dword_masked(HWIO_DRE_DRE_BEDP_PROF_FILTER_CFG_ADDR, HWIO_DRE_DRE_BEDP_PROF_FILTER_CFG_RMSK)
#define HWIO_DRE_DRE_BEDP_PROF_FILTER_CFG_INM(m)      \
        in_dword_masked(HWIO_DRE_DRE_BEDP_PROF_FILTER_CFG_ADDR, m)
#define HWIO_DRE_DRE_BEDP_PROF_FILTER_CFG_OUT(v)      \
        out_dword(HWIO_DRE_DRE_BEDP_PROF_FILTER_CFG_ADDR,v)
#define HWIO_DRE_DRE_BEDP_PROF_FILTER_CFG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DRE_DRE_BEDP_PROF_FILTER_CFG_ADDR,m,v,HWIO_DRE_DRE_BEDP_PROF_FILTER_CFG_IN)
#define HWIO_DRE_DRE_BEDP_PROF_FILTER_CFG_FILTER_EN_BMSK                                          0xffff
#define HWIO_DRE_DRE_BEDP_PROF_FILTER_CFG_FILTER_EN_SHFT                                             0x0

#define HWIO_DRE_DRE_BESP_PROF_FILTER_CFG_ADDR                                                (DRE_REGS_REG_BASE      + 0x0000048c)
#define HWIO_DRE_DRE_BESP_PROF_FILTER_CFG_PHYS                                                (DRE_REGS_REG_BASE_PHYS + 0x0000048c)
#define HWIO_DRE_DRE_BESP_PROF_FILTER_CFG_OFFS                                                (DRE_REGS_REG_BASE_OFFS + 0x0000048c)
#define HWIO_DRE_DRE_BESP_PROF_FILTER_CFG_RMSK                                                    0xffff
#define HWIO_DRE_DRE_BESP_PROF_FILTER_CFG_IN          \
        in_dword_masked(HWIO_DRE_DRE_BESP_PROF_FILTER_CFG_ADDR, HWIO_DRE_DRE_BESP_PROF_FILTER_CFG_RMSK)
#define HWIO_DRE_DRE_BESP_PROF_FILTER_CFG_INM(m)      \
        in_dword_masked(HWIO_DRE_DRE_BESP_PROF_FILTER_CFG_ADDR, m)
#define HWIO_DRE_DRE_BESP_PROF_FILTER_CFG_OUT(v)      \
        out_dword(HWIO_DRE_DRE_BESP_PROF_FILTER_CFG_ADDR,v)
#define HWIO_DRE_DRE_BESP_PROF_FILTER_CFG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DRE_DRE_BESP_PROF_FILTER_CFG_ADDR,m,v,HWIO_DRE_DRE_BESP_PROF_FILTER_CFG_IN)
#define HWIO_DRE_DRE_BESP_PROF_FILTER_CFG_FILTER_EN_BMSK                                          0xffff
#define HWIO_DRE_DRE_BESP_PROF_FILTER_CFG_FILTER_EN_SHFT                                             0x0

/*----------------------------------------------------------------------------
 * MODULE: DRE_DRE_DRE_ECC
 *--------------------------------------------------------------------------*/

#define DRE_DRE_DRE_ECC_REG_BASE                                                (DDR_SS_BASE      + 0x00140000)
#define DRE_DRE_DRE_ECC_REG_BASE_SIZE                                           0x200
#define DRE_DRE_DRE_ECC_REG_BASE_USED                                           0x94
#define DRE_DRE_DRE_ECC_REG_BASE_PHYS                                           (DDR_SS_BASE_PHYS + 0x00140000)
#define DRE_DRE_DRE_ECC_REG_BASE_OFFS                                           0x00140000

#define HWIO_DRE_DRE_ECC_FAULT_INJ_RESULT_ADDR                                  (DRE_DRE_DRE_ECC_REG_BASE      + 0x00000000)
#define HWIO_DRE_DRE_ECC_FAULT_INJ_RESULT_PHYS                                  (DRE_DRE_DRE_ECC_REG_BASE_PHYS + 0x00000000)
#define HWIO_DRE_DRE_ECC_FAULT_INJ_RESULT_OFFS                                  (DRE_DRE_DRE_ECC_REG_BASE_OFFS + 0x00000000)
#define HWIO_DRE_DRE_ECC_FAULT_INJ_RESULT_RMSK                                    0xffffff
#define HWIO_DRE_DRE_ECC_FAULT_INJ_RESULT_IN          \
        in_dword_masked(HWIO_DRE_DRE_ECC_FAULT_INJ_RESULT_ADDR, HWIO_DRE_DRE_ECC_FAULT_INJ_RESULT_RMSK)
#define HWIO_DRE_DRE_ECC_FAULT_INJ_RESULT_INM(m)      \
        in_dword_masked(HWIO_DRE_DRE_ECC_FAULT_INJ_RESULT_ADDR, m)
#define HWIO_DRE_DRE_ECC_FAULT_INJ_RESULT_ERROR_POSITION_DEC_BMSK                 0xffc000
#define HWIO_DRE_DRE_ECC_FAULT_INJ_RESULT_ERROR_POSITION_DEC_SHFT                      0xe
#define HWIO_DRE_DRE_ECC_FAULT_INJ_RESULT_ERROR_POSITION_SEC_BMSK                   0x3ff0
#define HWIO_DRE_DRE_ECC_FAULT_INJ_RESULT_ERROR_POSITION_SEC_SHFT                      0x4
#define HWIO_DRE_DRE_ECC_FAULT_INJ_RESULT_ERROR_STATUS_BMSK                            0xf
#define HWIO_DRE_DRE_ECC_FAULT_INJ_RESULT_ERROR_STATUS_SHFT                            0x0

#define HWIO_DRE_DRE_ECC_LOG_ERROR_i_ADDR_ADDR(i)                               (DRE_DRE_DRE_ECC_REG_BASE      + 0x00000004 + 0x4 * (i))
#define HWIO_DRE_DRE_ECC_LOG_ERROR_i_ADDR_PHYS(i)                               (DRE_DRE_DRE_ECC_REG_BASE_PHYS + 0x00000004 + 0x4 * (i))
#define HWIO_DRE_DRE_ECC_LOG_ERROR_i_ADDR_OFFS(i)                               (DRE_DRE_DRE_ECC_REG_BASE_OFFS + 0x00000004 + 0x4 * (i))
#define HWIO_DRE_DRE_ECC_LOG_ERROR_i_ADDR_RMSK                                  0xffffffff
#define HWIO_DRE_DRE_ECC_LOG_ERROR_i_ADDR_MAXi                                           7
#define HWIO_DRE_DRE_ECC_LOG_ERROR_i_ADDR_INI(i)        \
        in_dword_masked(HWIO_DRE_DRE_ECC_LOG_ERROR_i_ADDR_ADDR(i), HWIO_DRE_DRE_ECC_LOG_ERROR_i_ADDR_RMSK)
#define HWIO_DRE_DRE_ECC_LOG_ERROR_i_ADDR_INMI(i,mask)    \
        in_dword_masked(HWIO_DRE_DRE_ECC_LOG_ERROR_i_ADDR_ADDR(i), mask)
#define HWIO_DRE_DRE_ECC_LOG_ERROR_i_ADDR_ADDRESS_BMSK                          0xffffffff
#define HWIO_DRE_DRE_ECC_LOG_ERROR_i_ADDR_ADDRESS_SHFT                                 0x0

#define HWIO_DRE_DRE_ECC_LOG_ERROR_i_INFO_ADDR(i)                               (DRE_DRE_DRE_ECC_REG_BASE      + 0x00000024 + 0x4 * (i))
#define HWIO_DRE_DRE_ECC_LOG_ERROR_i_INFO_PHYS(i)                               (DRE_DRE_DRE_ECC_REG_BASE_PHYS + 0x00000024 + 0x4 * (i))
#define HWIO_DRE_DRE_ECC_LOG_ERROR_i_INFO_OFFS(i)                               (DRE_DRE_DRE_ECC_REG_BASE_OFFS + 0x00000024 + 0x4 * (i))
#define HWIO_DRE_DRE_ECC_LOG_ERROR_i_INFO_RMSK                                  0xfffffe01
#define HWIO_DRE_DRE_ECC_LOG_ERROR_i_INFO_MAXi                                           7
#define HWIO_DRE_DRE_ECC_LOG_ERROR_i_INFO_INI(i)        \
        in_dword_masked(HWIO_DRE_DRE_ECC_LOG_ERROR_i_INFO_ADDR(i), HWIO_DRE_DRE_ECC_LOG_ERROR_i_INFO_RMSK)
#define HWIO_DRE_DRE_ECC_LOG_ERROR_i_INFO_INMI(i,mask)    \
        in_dword_masked(HWIO_DRE_DRE_ECC_LOG_ERROR_i_INFO_ADDR(i), mask)
#define HWIO_DRE_DRE_ECC_LOG_ERROR_i_INFO_POSITION_DEC_BMSK                     0xffc00000
#define HWIO_DRE_DRE_ECC_LOG_ERROR_i_INFO_POSITION_DEC_SHFT                           0x16
#define HWIO_DRE_DRE_ECC_LOG_ERROR_i_INFO_POSITION_SEC_BMSK                       0x3ff000
#define HWIO_DRE_DRE_ECC_LOG_ERROR_i_INFO_POSITION_SEC_SHFT                            0xc
#define HWIO_DRE_DRE_ECC_LOG_ERROR_i_INFO_SEC_DED_BMSK                               0xe00
#define HWIO_DRE_DRE_ECC_LOG_ERROR_i_INFO_SEC_DED_SHFT                                 0x9
#define HWIO_DRE_DRE_ECC_LOG_ERROR_i_INFO_ENTRY_VALID_BMSK                             0x1
#define HWIO_DRE_DRE_ECC_LOG_ERROR_i_INFO_ENTRY_VALID_SHFT                             0x0

#define HWIO_DRE_DRE_ECC_READ_QUANTA_ADDR                                       (DRE_DRE_DRE_ECC_REG_BASE      + 0x00000044)
#define HWIO_DRE_DRE_ECC_READ_QUANTA_PHYS                                       (DRE_DRE_DRE_ECC_REG_BASE_PHYS + 0x00000044)
#define HWIO_DRE_DRE_ECC_READ_QUANTA_OFFS                                       (DRE_DRE_DRE_ECC_REG_BASE_OFFS + 0x00000044)
#define HWIO_DRE_DRE_ECC_READ_QUANTA_RMSK                                            0x3ff
#define HWIO_DRE_DRE_ECC_READ_QUANTA_IN          \
        in_dword_masked(HWIO_DRE_DRE_ECC_READ_QUANTA_ADDR, HWIO_DRE_DRE_ECC_READ_QUANTA_RMSK)
#define HWIO_DRE_DRE_ECC_READ_QUANTA_INM(m)      \
        in_dword_masked(HWIO_DRE_DRE_ECC_READ_QUANTA_ADDR, m)
#define HWIO_DRE_DRE_ECC_READ_QUANTA_ECC_QUANTA_BMSK                                 0x3ff
#define HWIO_DRE_DRE_ECC_READ_QUANTA_ECC_QUANTA_SHFT                                   0x0

#define HWIO_DRE_DRE_ECC_VERSION_INFO_ADDR                                      (DRE_DRE_DRE_ECC_REG_BASE      + 0x00000048)
#define HWIO_DRE_DRE_ECC_VERSION_INFO_PHYS                                      (DRE_DRE_DRE_ECC_REG_BASE_PHYS + 0x00000048)
#define HWIO_DRE_DRE_ECC_VERSION_INFO_OFFS                                      (DRE_DRE_DRE_ECC_REG_BASE_OFFS + 0x00000048)
#define HWIO_DRE_DRE_ECC_VERSION_INFO_RMSK                                      0xffffffff
#define HWIO_DRE_DRE_ECC_VERSION_INFO_IN          \
        in_dword_masked(HWIO_DRE_DRE_ECC_VERSION_INFO_ADDR, HWIO_DRE_DRE_ECC_VERSION_INFO_RMSK)
#define HWIO_DRE_DRE_ECC_VERSION_INFO_INM(m)      \
        in_dword_masked(HWIO_DRE_DRE_ECC_VERSION_INFO_ADDR, m)
#define HWIO_DRE_DRE_ECC_VERSION_INFO_ECC_IP_VERSION_BMSK                       0xffffffff
#define HWIO_DRE_DRE_ECC_VERSION_INFO_ECC_IP_VERSION_SHFT                              0x0

#define HWIO_DRE_DRE_ECC_AUTO_MEM_INIT_STATUS_ADDR                              (DRE_DRE_DRE_ECC_REG_BASE      + 0x0000004c)
#define HWIO_DRE_DRE_ECC_AUTO_MEM_INIT_STATUS_PHYS                              (DRE_DRE_DRE_ECC_REG_BASE_PHYS + 0x0000004c)
#define HWIO_DRE_DRE_ECC_AUTO_MEM_INIT_STATUS_OFFS                              (DRE_DRE_DRE_ECC_REG_BASE_OFFS + 0x0000004c)
#define HWIO_DRE_DRE_ECC_AUTO_MEM_INIT_STATUS_RMSK                                     0x1
#define HWIO_DRE_DRE_ECC_AUTO_MEM_INIT_STATUS_IN          \
        in_dword_masked(HWIO_DRE_DRE_ECC_AUTO_MEM_INIT_STATUS_ADDR, HWIO_DRE_DRE_ECC_AUTO_MEM_INIT_STATUS_RMSK)
#define HWIO_DRE_DRE_ECC_AUTO_MEM_INIT_STATUS_INM(m)      \
        in_dword_masked(HWIO_DRE_DRE_ECC_AUTO_MEM_INIT_STATUS_ADDR, m)
#define HWIO_DRE_DRE_ECC_AUTO_MEM_INIT_STATUS_ECC_AUTO_INIT_DONE_BMSK                  0x1
#define HWIO_DRE_DRE_ECC_AUTO_MEM_INIT_STATUS_ECC_AUTO_INIT_DONE_SHFT                  0x0

#define HWIO_DRE_DRE_ECC_CONTROL_ADDR                                           (DRE_DRE_DRE_ECC_REG_BASE      + 0x00000050)
#define HWIO_DRE_DRE_ECC_CONTROL_PHYS                                           (DRE_DRE_DRE_ECC_REG_BASE_PHYS + 0x00000050)
#define HWIO_DRE_DRE_ECC_CONTROL_OFFS                                           (DRE_DRE_DRE_ECC_REG_BASE_OFFS + 0x00000050)
#define HWIO_DRE_DRE_ECC_CONTROL_RMSK                                              0x3ffff
#define HWIO_DRE_DRE_ECC_CONTROL_IN          \
        in_dword_masked(HWIO_DRE_DRE_ECC_CONTROL_ADDR, HWIO_DRE_DRE_ECC_CONTROL_RMSK)
#define HWIO_DRE_DRE_ECC_CONTROL_INM(m)      \
        in_dword_masked(HWIO_DRE_DRE_ECC_CONTROL_ADDR, m)
#define HWIO_DRE_DRE_ECC_CONTROL_OUT(v)      \
        out_dword(HWIO_DRE_DRE_ECC_CONTROL_ADDR,v)
#define HWIO_DRE_DRE_ECC_CONTROL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DRE_DRE_ECC_CONTROL_ADDR,m,v,HWIO_DRE_DRE_ECC_CONTROL_IN)
#define HWIO_DRE_DRE_ECC_CONTROL_FAULT_INJ_IRQ_EN_BMSK                             0x20000
#define HWIO_DRE_DRE_ECC_CONTROL_FAULT_INJ_IRQ_EN_SHFT                                0x11
#define HWIO_DRE_DRE_ECC_CONTROL_FAULT_INJ_UPPER_LOWER_BMSK                        0x10000
#define HWIO_DRE_DRE_ECC_CONTROL_FAULT_INJ_UPPER_LOWER_SHFT                           0x10
#define HWIO_DRE_DRE_ECC_CONTROL_ENABLE_BUS_ERROR_GEN_BMSK                          0xf000
#define HWIO_DRE_DRE_ECC_CONTROL_ENABLE_BUS_ERROR_GEN_SHFT                             0xc
#define HWIO_DRE_DRE_ECC_CONTROL_ENABLE_WRITEBACK_BMSK                               0xf00
#define HWIO_DRE_DRE_ECC_CONTROL_ENABLE_WRITEBACK_SHFT                                 0x8
#define HWIO_DRE_DRE_ECC_CONTROL_ENABLE_FAULT_INJECT_BMSK                             0xf0
#define HWIO_DRE_DRE_ECC_CONTROL_ENABLE_FAULT_INJECT_SHFT                              0x4
#define HWIO_DRE_DRE_ECC_CONTROL_ENABLE_ECC_BMSK                                       0xf
#define HWIO_DRE_DRE_ECC_CONTROL_ENABLE_ECC_SHFT                                       0x0

#define HWIO_DRE_DRE_ECC_FAULT_INJ_i_DATA_ADDR(i)                               (DRE_DRE_DRE_ECC_REG_BASE      + 0x00000054 + 0x4 * (i))
#define HWIO_DRE_DRE_ECC_FAULT_INJ_i_DATA_PHYS(i)                               (DRE_DRE_DRE_ECC_REG_BASE_PHYS + 0x00000054 + 0x4 * (i))
#define HWIO_DRE_DRE_ECC_FAULT_INJ_i_DATA_OFFS(i)                               (DRE_DRE_DRE_ECC_REG_BASE_OFFS + 0x00000054 + 0x4 * (i))
#define HWIO_DRE_DRE_ECC_FAULT_INJ_i_DATA_RMSK                                  0xffffffff
#define HWIO_DRE_DRE_ECC_FAULT_INJ_i_DATA_MAXi                                           7
#define HWIO_DRE_DRE_ECC_FAULT_INJ_i_DATA_INI(i)        \
        in_dword_masked(HWIO_DRE_DRE_ECC_FAULT_INJ_i_DATA_ADDR(i), HWIO_DRE_DRE_ECC_FAULT_INJ_i_DATA_RMSK)
#define HWIO_DRE_DRE_ECC_FAULT_INJ_i_DATA_INMI(i,mask)    \
        in_dword_masked(HWIO_DRE_DRE_ECC_FAULT_INJ_i_DATA_ADDR(i), mask)
#define HWIO_DRE_DRE_ECC_FAULT_INJ_i_DATA_OUTI(i,val)    \
        out_dword(HWIO_DRE_DRE_ECC_FAULT_INJ_i_DATA_ADDR(i),val)
#define HWIO_DRE_DRE_ECC_FAULT_INJ_i_DATA_OUTMI(i,mask,val) \
        out_dword_masked_ns(HWIO_DRE_DRE_ECC_FAULT_INJ_i_DATA_ADDR(i),mask,val,HWIO_DRE_DRE_ECC_FAULT_INJ_i_DATA_INI(i))
#define HWIO_DRE_DRE_ECC_FAULT_INJ_i_DATA_DATA_BMSK                             0xffffffff
#define HWIO_DRE_DRE_ECC_FAULT_INJ_i_DATA_DATA_SHFT                                    0x0

#define HWIO_DRE_DRE_ECC_FAULT_INJ_SYNDROME_ADDR                                (DRE_DRE_DRE_ECC_REG_BASE      + 0x00000074)
#define HWIO_DRE_DRE_ECC_FAULT_INJ_SYNDROME_PHYS                                (DRE_DRE_DRE_ECC_REG_BASE_PHYS + 0x00000074)
#define HWIO_DRE_DRE_ECC_FAULT_INJ_SYNDROME_OFFS                                (DRE_DRE_DRE_ECC_REG_BASE_OFFS + 0x00000074)
#define HWIO_DRE_DRE_ECC_FAULT_INJ_SYNDROME_RMSK                                     0x7ff
#define HWIO_DRE_DRE_ECC_FAULT_INJ_SYNDROME_IN          \
        in_dword_masked(HWIO_DRE_DRE_ECC_FAULT_INJ_SYNDROME_ADDR, HWIO_DRE_DRE_ECC_FAULT_INJ_SYNDROME_RMSK)
#define HWIO_DRE_DRE_ECC_FAULT_INJ_SYNDROME_INM(m)      \
        in_dword_masked(HWIO_DRE_DRE_ECC_FAULT_INJ_SYNDROME_ADDR, m)
#define HWIO_DRE_DRE_ECC_FAULT_INJ_SYNDROME_OUT(v)      \
        out_dword(HWIO_DRE_DRE_ECC_FAULT_INJ_SYNDROME_ADDR,v)
#define HWIO_DRE_DRE_ECC_FAULT_INJ_SYNDROME_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DRE_DRE_ECC_FAULT_INJ_SYNDROME_ADDR,m,v,HWIO_DRE_DRE_ECC_FAULT_INJ_SYNDROME_IN)
#define HWIO_DRE_DRE_ECC_FAULT_INJ_SYNDROME_SYNDROME_PATTERN_MASK_BMSK               0x7ff
#define HWIO_DRE_DRE_ECC_FAULT_INJ_SYNDROME_SYNDROME_PATTERN_MASK_SHFT                 0x0

#define HWIO_DRE_DRE_ECC_IRQ_SEC_ADDR                                           (DRE_DRE_DRE_ECC_REG_BASE      + 0x00000078)
#define HWIO_DRE_DRE_ECC_IRQ_SEC_PHYS                                           (DRE_DRE_DRE_ECC_REG_BASE_PHYS + 0x00000078)
#define HWIO_DRE_DRE_ECC_IRQ_SEC_OFFS                                           (DRE_DRE_DRE_ECC_REG_BASE_OFFS + 0x00000078)
#define HWIO_DRE_DRE_ECC_IRQ_SEC_RMSK                                                0xfff
#define HWIO_DRE_DRE_ECC_IRQ_SEC_IN          \
        in_dword_masked(HWIO_DRE_DRE_ECC_IRQ_SEC_ADDR, HWIO_DRE_DRE_ECC_IRQ_SEC_RMSK)
#define HWIO_DRE_DRE_ECC_IRQ_SEC_INM(m)      \
        in_dword_masked(HWIO_DRE_DRE_ECC_IRQ_SEC_ADDR, m)
#define HWIO_DRE_DRE_ECC_IRQ_SEC_OUT(v)      \
        out_dword(HWIO_DRE_DRE_ECC_IRQ_SEC_ADDR,v)
#define HWIO_DRE_DRE_ECC_IRQ_SEC_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DRE_DRE_ECC_IRQ_SEC_ADDR,m,v,HWIO_DRE_DRE_ECC_IRQ_SEC_IN)
#define HWIO_DRE_DRE_ECC_IRQ_SEC_ENABLE_SEC_IRQ_REPEAT_BMSK                          0xf00
#define HWIO_DRE_DRE_ECC_IRQ_SEC_ENABLE_SEC_IRQ_REPEAT_SHFT                            0x8
#define HWIO_DRE_DRE_ECC_IRQ_SEC_ENABLE_SEC_THRESHOLD_BMSK                            0xf0
#define HWIO_DRE_DRE_ECC_IRQ_SEC_ENABLE_SEC_THRESHOLD_SHFT                             0x4
#define HWIO_DRE_DRE_ECC_IRQ_SEC_ENABLE_SEC_IRQ_BMSK                                   0xf
#define HWIO_DRE_DRE_ECC_IRQ_SEC_ENABLE_SEC_IRQ_SHFT                                   0x0

#define HWIO_DRE_DRE_ECC_CLEAR_COUNTER_ADDR                                     (DRE_DRE_DRE_ECC_REG_BASE      + 0x0000007c)
#define HWIO_DRE_DRE_ECC_CLEAR_COUNTER_PHYS                                     (DRE_DRE_DRE_ECC_REG_BASE_PHYS + 0x0000007c)
#define HWIO_DRE_DRE_ECC_CLEAR_COUNTER_OFFS                                     (DRE_DRE_DRE_ECC_REG_BASE_OFFS + 0x0000007c)
#define HWIO_DRE_DRE_ECC_CLEAR_COUNTER_RMSK                                           0xff
#define HWIO_DRE_DRE_ECC_CLEAR_COUNTER_IN          \
        in_dword_masked(HWIO_DRE_DRE_ECC_CLEAR_COUNTER_ADDR, HWIO_DRE_DRE_ECC_CLEAR_COUNTER_RMSK)
#define HWIO_DRE_DRE_ECC_CLEAR_COUNTER_INM(m)      \
        in_dword_masked(HWIO_DRE_DRE_ECC_CLEAR_COUNTER_ADDR, m)
#define HWIO_DRE_DRE_ECC_CLEAR_COUNTER_OUT(v)      \
        out_dword(HWIO_DRE_DRE_ECC_CLEAR_COUNTER_ADDR,v)
#define HWIO_DRE_DRE_ECC_CLEAR_COUNTER_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DRE_DRE_ECC_CLEAR_COUNTER_ADDR,m,v,HWIO_DRE_DRE_ECC_CLEAR_COUNTER_IN)
#define HWIO_DRE_DRE_ECC_CLEAR_COUNTER_CLEAR_DED_COUNTER_BMSK                         0xf0
#define HWIO_DRE_DRE_ECC_CLEAR_COUNTER_CLEAR_DED_COUNTER_SHFT                          0x4
#define HWIO_DRE_DRE_ECC_CLEAR_COUNTER_CLEAR_SEC_COUNTER_BMSK                          0xf
#define HWIO_DRE_DRE_ECC_CLEAR_COUNTER_CLEAR_SEC_COUNTER_SHFT                          0x0

#define HWIO_DRE_DRE_ECC_CLEAR_FIFO_ADDR                                        (DRE_DRE_DRE_ECC_REG_BASE      + 0x00000080)
#define HWIO_DRE_DRE_ECC_CLEAR_FIFO_PHYS                                        (DRE_DRE_DRE_ECC_REG_BASE_PHYS + 0x00000080)
#define HWIO_DRE_DRE_ECC_CLEAR_FIFO_OFFS                                        (DRE_DRE_DRE_ECC_REG_BASE_OFFS + 0x00000080)
#define HWIO_DRE_DRE_ECC_CLEAR_FIFO_RMSK                                               0xf
#define HWIO_DRE_DRE_ECC_CLEAR_FIFO_IN          \
        in_dword_masked(HWIO_DRE_DRE_ECC_CLEAR_FIFO_ADDR, HWIO_DRE_DRE_ECC_CLEAR_FIFO_RMSK)
#define HWIO_DRE_DRE_ECC_CLEAR_FIFO_INM(m)      \
        in_dword_masked(HWIO_DRE_DRE_ECC_CLEAR_FIFO_ADDR, m)
#define HWIO_DRE_DRE_ECC_CLEAR_FIFO_OUT(v)      \
        out_dword(HWIO_DRE_DRE_ECC_CLEAR_FIFO_ADDR,v)
#define HWIO_DRE_DRE_ECC_CLEAR_FIFO_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DRE_DRE_ECC_CLEAR_FIFO_ADDR,m,v,HWIO_DRE_DRE_ECC_CLEAR_FIFO_IN)
#define HWIO_DRE_DRE_ECC_CLEAR_FIFO_CLEAR_FIFO_BMSK                                    0xf
#define HWIO_DRE_DRE_ECC_CLEAR_FIFO_CLEAR_FIFO_SHFT                                    0x0

#define HWIO_DRE_DRE_ECC_FAULT_INJ_ADDR_ADDR                                    (DRE_DRE_DRE_ECC_REG_BASE      + 0x00000084)
#define HWIO_DRE_DRE_ECC_FAULT_INJ_ADDR_PHYS                                    (DRE_DRE_DRE_ECC_REG_BASE_PHYS + 0x00000084)
#define HWIO_DRE_DRE_ECC_FAULT_INJ_ADDR_OFFS                                    (DRE_DRE_DRE_ECC_REG_BASE_OFFS + 0x00000084)
#define HWIO_DRE_DRE_ECC_FAULT_INJ_ADDR_RMSK                                    0xffffffff
#define HWIO_DRE_DRE_ECC_FAULT_INJ_ADDR_IN          \
        in_dword_masked(HWIO_DRE_DRE_ECC_FAULT_INJ_ADDR_ADDR, HWIO_DRE_DRE_ECC_FAULT_INJ_ADDR_RMSK)
#define HWIO_DRE_DRE_ECC_FAULT_INJ_ADDR_INM(m)      \
        in_dword_masked(HWIO_DRE_DRE_ECC_FAULT_INJ_ADDR_ADDR, m)
#define HWIO_DRE_DRE_ECC_FAULT_INJ_ADDR_OUT(v)      \
        out_dword(HWIO_DRE_DRE_ECC_FAULT_INJ_ADDR_ADDR,v)
#define HWIO_DRE_DRE_ECC_FAULT_INJ_ADDR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DRE_DRE_ECC_FAULT_INJ_ADDR_ADDR,m,v,HWIO_DRE_DRE_ECC_FAULT_INJ_ADDR_IN)
#define HWIO_DRE_DRE_ECC_FAULT_INJ_ADDR_ADDRESS_BMSK                            0xffffffff
#define HWIO_DRE_DRE_ECC_FAULT_INJ_ADDR_ADDRESS_SHFT                                   0x0

#define HWIO_DRE_DRE_ECC_QUANTA_ADDR                                            (DRE_DRE_DRE_ECC_REG_BASE      + 0x00000088)
#define HWIO_DRE_DRE_ECC_QUANTA_PHYS                                            (DRE_DRE_DRE_ECC_REG_BASE_PHYS + 0x00000088)
#define HWIO_DRE_DRE_ECC_QUANTA_OFFS                                            (DRE_DRE_DRE_ECC_REG_BASE_OFFS + 0x00000088)
#define HWIO_DRE_DRE_ECC_QUANTA_RMSK                                                 0x3ff
#define HWIO_DRE_DRE_ECC_QUANTA_IN          \
        in_dword_masked(HWIO_DRE_DRE_ECC_QUANTA_ADDR, HWIO_DRE_DRE_ECC_QUANTA_RMSK)
#define HWIO_DRE_DRE_ECC_QUANTA_INM(m)      \
        in_dword_masked(HWIO_DRE_DRE_ECC_QUANTA_ADDR, m)
#define HWIO_DRE_DRE_ECC_QUANTA_OUT(v)      \
        out_dword(HWIO_DRE_DRE_ECC_QUANTA_ADDR,v)
#define HWIO_DRE_DRE_ECC_QUANTA_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DRE_DRE_ECC_QUANTA_ADDR,m,v,HWIO_DRE_DRE_ECC_QUANTA_IN)
#define HWIO_DRE_DRE_ECC_QUANTA_ECC_QUANTA_BMSK                                      0x3ff
#define HWIO_DRE_DRE_ECC_QUANTA_ECC_QUANTA_SHFT                                        0x0

#define HWIO_DRE_DRE_ECC_AUTO_MEM_INIT_ADDR                                     (DRE_DRE_DRE_ECC_REG_BASE      + 0x0000008c)
#define HWIO_DRE_DRE_ECC_AUTO_MEM_INIT_PHYS                                     (DRE_DRE_DRE_ECC_REG_BASE_PHYS + 0x0000008c)
#define HWIO_DRE_DRE_ECC_AUTO_MEM_INIT_OFFS                                     (DRE_DRE_DRE_ECC_REG_BASE_OFFS + 0x0000008c)
#define HWIO_DRE_DRE_ECC_AUTO_MEM_INIT_RMSK                                            0xf
#define HWIO_DRE_DRE_ECC_AUTO_MEM_INIT_IN          \
        in_dword_masked(HWIO_DRE_DRE_ECC_AUTO_MEM_INIT_ADDR, HWIO_DRE_DRE_ECC_AUTO_MEM_INIT_RMSK)
#define HWIO_DRE_DRE_ECC_AUTO_MEM_INIT_INM(m)      \
        in_dword_masked(HWIO_DRE_DRE_ECC_AUTO_MEM_INIT_ADDR, m)
#define HWIO_DRE_DRE_ECC_AUTO_MEM_INIT_OUT(v)      \
        out_dword(HWIO_DRE_DRE_ECC_AUTO_MEM_INIT_ADDR,v)
#define HWIO_DRE_DRE_ECC_AUTO_MEM_INIT_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DRE_DRE_ECC_AUTO_MEM_INIT_ADDR,m,v,HWIO_DRE_DRE_ECC_AUTO_MEM_INIT_IN)
#define HWIO_DRE_DRE_ECC_AUTO_MEM_INIT_AUTO_FILL_BMSK                                  0xf
#define HWIO_DRE_DRE_ECC_AUTO_MEM_INIT_AUTO_FILL_SHFT                                  0x0

#define HWIO_DRE_DRE_ECC_ERROR_COUNTER_ADDR                                     (DRE_DRE_DRE_ECC_REG_BASE      + 0x00000090)
#define HWIO_DRE_DRE_ECC_ERROR_COUNTER_PHYS                                     (DRE_DRE_DRE_ECC_REG_BASE_PHYS + 0x00000090)
#define HWIO_DRE_DRE_ECC_ERROR_COUNTER_OFFS                                     (DRE_DRE_DRE_ECC_REG_BASE_OFFS + 0x00000090)
#define HWIO_DRE_DRE_ECC_ERROR_COUNTER_RMSK                                         0xffff
#define HWIO_DRE_DRE_ECC_ERROR_COUNTER_IN          \
        in_dword_masked(HWIO_DRE_DRE_ECC_ERROR_COUNTER_ADDR, HWIO_DRE_DRE_ECC_ERROR_COUNTER_RMSK)
#define HWIO_DRE_DRE_ECC_ERROR_COUNTER_INM(m)      \
        in_dword_masked(HWIO_DRE_DRE_ECC_ERROR_COUNTER_ADDR, m)
#define HWIO_DRE_DRE_ECC_ERROR_COUNTER_DED_COUNTER_BMSK                             0xff00
#define HWIO_DRE_DRE_ECC_ERROR_COUNTER_DED_COUNTER_SHFT                                0x8
#define HWIO_DRE_DRE_ECC_ERROR_COUNTER_SEC_COUNTER_BMSK                               0xff
#define HWIO_DRE_DRE_ECC_ERROR_COUNTER_SEC_COUNTER_SHFT                                0x0

#define HWIO_DRE_DRE_ECC_INTEGRATION_INFO_ADDR                                  (DRE_DRE_DRE_ECC_REG_BASE      + 0x00000094)
#define HWIO_DRE_DRE_ECC_INTEGRATION_INFO_PHYS                                  (DRE_DRE_DRE_ECC_REG_BASE_PHYS + 0x00000094)
#define HWIO_DRE_DRE_ECC_INTEGRATION_INFO_OFFS                                  (DRE_DRE_DRE_ECC_REG_BASE_OFFS + 0x00000094)
#define HWIO_DRE_DRE_ECC_INTEGRATION_INFO_RMSK                                  0xffffffff
#define HWIO_DRE_DRE_ECC_INTEGRATION_INFO_IN          \
        in_dword_masked(HWIO_DRE_DRE_ECC_INTEGRATION_INFO_ADDR, HWIO_DRE_DRE_ECC_INTEGRATION_INFO_RMSK)
#define HWIO_DRE_DRE_ECC_INTEGRATION_INFO_INM(m)      \
        in_dword_masked(HWIO_DRE_DRE_ECC_INTEGRATION_INFO_ADDR, m)
#define HWIO_DRE_DRE_ECC_INTEGRATION_INFO_OUT(v)      \
        out_dword(HWIO_DRE_DRE_ECC_INTEGRATION_INFO_ADDR,v)
#define HWIO_DRE_DRE_ECC_INTEGRATION_INFO_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DRE_DRE_ECC_INTEGRATION_INFO_ADDR,m,v,HWIO_DRE_DRE_ECC_INTEGRATION_INFO_IN)
#define HWIO_DRE_DRE_ECC_INTEGRATION_INFO_INFO_BMSK                             0xffffffff
#define HWIO_DRE_DRE_ECC_INTEGRATION_INFO_INFO_SHFT                                    0x0


#endif /* __DRE_TEST_H__ */
