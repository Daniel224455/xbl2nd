#ifndef __ICBCFG_HWIO_TARGET_H__
#define __ICBCFG_HWIO_TARGET_H__
/*
===========================================================================
*/
/**
  @file icbcfg_hwio_target.h
  @brief Auto-generated HWIO interface include file.

  Reference chip release:
    SDM1000 (Poipu) [poipu_v2.0_p3q3r65_MTO]
 
  This file contains HWIO register definitions for the following modules:
    LLCC0_BEAC
    LLCC1_BEAC
    LLCC2_BEAC
    LLCC3_BEAC
    LLCC4_BEAC
    LLCC5_BEAC
    LLCC6_BEAC
    LLCC7_BEAC
    TCSR_TCSR_REGS

  'Include' filters applied: ADDR_TRANSLATOR_CFG[LLCC3_BEAC] ADDR_TRANSLATOR_CFG[LLCC5_BEAC] ADDR_TRANSLATOR_CFG[LLCC6_BEAC] ADDR_TRANSLATOR_CFG[LLCC0_BEAC] ADDR_TRANSLATOR_CFG[LLCC7_BEAC] ADDR_TRANSLATOR_CFG[LLCC4_BEAC] ADDR_TRANSLATOR_CFG[LLCC2_BEAC] ADDR_TRANSLATOR_CFG[LLCC1_BEAC] 

  Generation parameters: 
  { u'filename': u'icbcfg_hwio_target.h',
    u'module-filter-exclude': { },
    u'module-filter-include': { u'LLCC0_BEAC': [u'ADDR_TRANSLATOR_CFG'],
                                u'LLCC1_BEAC': [u'ADDR_TRANSLATOR_CFG'],
                                u'LLCC2_BEAC': [u'ADDR_TRANSLATOR_CFG'],
                                u'LLCC3_BEAC': [u'ADDR_TRANSLATOR_CFG'],
                                u'LLCC4_BEAC': [u'ADDR_TRANSLATOR_CFG'],
                                u'LLCC5_BEAC': [u'ADDR_TRANSLATOR_CFG'],
                                u'LLCC6_BEAC': [u'ADDR_TRANSLATOR_CFG'],
                                u'LLCC7_BEAC': [u'ADDR_TRANSLATOR_CFG'],
                                u'TCSR_TCSR_REGS': [u'TIMEOUT_INTR_STATUS']},
    u'modules': [ u'LLCC0_BEAC',
                  u'LLCC1_BEAC',
                  u'LLCC2_BEAC',
                  u'LLCC3_BEAC',
                  u'LLCC4_BEAC',
                  u'LLCC5_BEAC',
                  u'LLCC6_BEAC',
                  u'LLCC7_BEAC',
                  u'TCSR_TCSR_REGS']}
*/
/*
  ===========================================================================

  Copyright (c) 2017 Qualcomm Technologies, Inc.
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


  ===========================================================================
*/

/*----------------------------------------------------------------------------
 * MODULE: LLCC0_BEAC
 *--------------------------------------------------------------------------*/

#define LLCC0_BEAC_REG_BASE                                                                  (DDR_SS_BASE      + 0x00248000)

#define HWIO_LLCC0_LLCC_BEAC_ADDR_TRANSLATOR_CFG_ADDR                                        (LLCC0_BEAC_REG_BASE      + 0x00000080)
#define HWIO_LLCC0_LLCC_BEAC_ADDR_TRANSLATOR_CFG_RMSK                                             0x3f1
#define HWIO_LLCC0_LLCC_BEAC_ADDR_TRANSLATOR_CFG_IN          \
        in_dword_masked(HWIO_LLCC0_LLCC_BEAC_ADDR_TRANSLATOR_CFG_ADDR, HWIO_LLCC0_LLCC_BEAC_ADDR_TRANSLATOR_CFG_RMSK)
#define HWIO_LLCC0_LLCC_BEAC_ADDR_TRANSLATOR_CFG_INM(m)      \
        in_dword_masked(HWIO_LLCC0_LLCC_BEAC_ADDR_TRANSLATOR_CFG_ADDR, m)
#define HWIO_LLCC0_LLCC_BEAC_ADDR_TRANSLATOR_CFG_OUT(v)      \
        out_dword(HWIO_LLCC0_LLCC_BEAC_ADDR_TRANSLATOR_CFG_ADDR,v)
#define HWIO_LLCC0_LLCC_BEAC_ADDR_TRANSLATOR_CFG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_LLCC0_LLCC_BEAC_ADDR_TRANSLATOR_CFG_ADDR,m,v,HWIO_LLCC0_LLCC_BEAC_ADDR_TRANSLATOR_CFG_IN)
#define HWIO_LLCC0_LLCC_BEAC_ADDR_TRANSLATOR_CFG_ERROR_BMSK                                       0x3f0
#define HWIO_LLCC0_LLCC_BEAC_ADDR_TRANSLATOR_CFG_ERROR_SHFT                                         0x4
#define HWIO_LLCC0_LLCC_BEAC_ADDR_TRANSLATOR_CFG_ENABLE_BMSK                                        0x1
#define HWIO_LLCC0_LLCC_BEAC_ADDR_TRANSLATOR_CFG_ENABLE_SHFT                                        0x0

/*----------------------------------------------------------------------------
 * MODULE: LLCC1_BEAC
 *--------------------------------------------------------------------------*/

#define LLCC1_BEAC_REG_BASE                                                                  (DDR_SS_BASE      + 0x002c8000)

#define HWIO_LLCC1_LLCC_BEAC_ADDR_TRANSLATOR_CFG_ADDR                                        (LLCC1_BEAC_REG_BASE      + 0x00000080)
#define HWIO_LLCC1_LLCC_BEAC_ADDR_TRANSLATOR_CFG_RMSK                                             0x3f1
#define HWIO_LLCC1_LLCC_BEAC_ADDR_TRANSLATOR_CFG_IN          \
        in_dword_masked(HWIO_LLCC1_LLCC_BEAC_ADDR_TRANSLATOR_CFG_ADDR, HWIO_LLCC1_LLCC_BEAC_ADDR_TRANSLATOR_CFG_RMSK)
#define HWIO_LLCC1_LLCC_BEAC_ADDR_TRANSLATOR_CFG_INM(m)      \
        in_dword_masked(HWIO_LLCC1_LLCC_BEAC_ADDR_TRANSLATOR_CFG_ADDR, m)
#define HWIO_LLCC1_LLCC_BEAC_ADDR_TRANSLATOR_CFG_OUT(v)      \
        out_dword(HWIO_LLCC1_LLCC_BEAC_ADDR_TRANSLATOR_CFG_ADDR,v)
#define HWIO_LLCC1_LLCC_BEAC_ADDR_TRANSLATOR_CFG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_LLCC1_LLCC_BEAC_ADDR_TRANSLATOR_CFG_ADDR,m,v,HWIO_LLCC1_LLCC_BEAC_ADDR_TRANSLATOR_CFG_IN)
#define HWIO_LLCC1_LLCC_BEAC_ADDR_TRANSLATOR_CFG_ERROR_BMSK                                       0x3f0
#define HWIO_LLCC1_LLCC_BEAC_ADDR_TRANSLATOR_CFG_ERROR_SHFT                                         0x4
#define HWIO_LLCC1_LLCC_BEAC_ADDR_TRANSLATOR_CFG_ENABLE_BMSK                                        0x1
#define HWIO_LLCC1_LLCC_BEAC_ADDR_TRANSLATOR_CFG_ENABLE_SHFT                                        0x0

/*----------------------------------------------------------------------------
 * MODULE: LLCC2_BEAC
 *--------------------------------------------------------------------------*/

#define LLCC2_BEAC_REG_BASE                                                                  (DDR_SS_BASE      + 0x00348000)

#define HWIO_LLCC2_LLCC_BEAC_ADDR_TRANSLATOR_CFG_ADDR                                        (LLCC2_BEAC_REG_BASE      + 0x00000080)
#define HWIO_LLCC2_LLCC_BEAC_ADDR_TRANSLATOR_CFG_RMSK                                             0x3f1
#define HWIO_LLCC2_LLCC_BEAC_ADDR_TRANSLATOR_CFG_IN          \
        in_dword_masked(HWIO_LLCC2_LLCC_BEAC_ADDR_TRANSLATOR_CFG_ADDR, HWIO_LLCC2_LLCC_BEAC_ADDR_TRANSLATOR_CFG_RMSK)
#define HWIO_LLCC2_LLCC_BEAC_ADDR_TRANSLATOR_CFG_INM(m)      \
        in_dword_masked(HWIO_LLCC2_LLCC_BEAC_ADDR_TRANSLATOR_CFG_ADDR, m)
#define HWIO_LLCC2_LLCC_BEAC_ADDR_TRANSLATOR_CFG_OUT(v)      \
        out_dword(HWIO_LLCC2_LLCC_BEAC_ADDR_TRANSLATOR_CFG_ADDR,v)
#define HWIO_LLCC2_LLCC_BEAC_ADDR_TRANSLATOR_CFG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_LLCC2_LLCC_BEAC_ADDR_TRANSLATOR_CFG_ADDR,m,v,HWIO_LLCC2_LLCC_BEAC_ADDR_TRANSLATOR_CFG_IN)
#define HWIO_LLCC2_LLCC_BEAC_ADDR_TRANSLATOR_CFG_ERROR_BMSK                                       0x3f0
#define HWIO_LLCC2_LLCC_BEAC_ADDR_TRANSLATOR_CFG_ERROR_SHFT                                         0x4
#define HWIO_LLCC2_LLCC_BEAC_ADDR_TRANSLATOR_CFG_ENABLE_BMSK                                        0x1
#define HWIO_LLCC2_LLCC_BEAC_ADDR_TRANSLATOR_CFG_ENABLE_SHFT                                        0x0

/*----------------------------------------------------------------------------
 * MODULE: LLCC3_BEAC
 *--------------------------------------------------------------------------*/

#define LLCC3_BEAC_REG_BASE                                                                  (DDR_SS_BASE      + 0x003c8000)

#define HWIO_LLCC3_LLCC_BEAC_ADDR_TRANSLATOR_CFG_ADDR                                        (LLCC3_BEAC_REG_BASE      + 0x00000080)
#define HWIO_LLCC3_LLCC_BEAC_ADDR_TRANSLATOR_CFG_RMSK                                             0x3f1
#define HWIO_LLCC3_LLCC_BEAC_ADDR_TRANSLATOR_CFG_IN          \
        in_dword_masked(HWIO_LLCC3_LLCC_BEAC_ADDR_TRANSLATOR_CFG_ADDR, HWIO_LLCC3_LLCC_BEAC_ADDR_TRANSLATOR_CFG_RMSK)
#define HWIO_LLCC3_LLCC_BEAC_ADDR_TRANSLATOR_CFG_INM(m)      \
        in_dword_masked(HWIO_LLCC3_LLCC_BEAC_ADDR_TRANSLATOR_CFG_ADDR, m)
#define HWIO_LLCC3_LLCC_BEAC_ADDR_TRANSLATOR_CFG_OUT(v)      \
        out_dword(HWIO_LLCC3_LLCC_BEAC_ADDR_TRANSLATOR_CFG_ADDR,v)
#define HWIO_LLCC3_LLCC_BEAC_ADDR_TRANSLATOR_CFG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_LLCC3_LLCC_BEAC_ADDR_TRANSLATOR_CFG_ADDR,m,v,HWIO_LLCC3_LLCC_BEAC_ADDR_TRANSLATOR_CFG_IN)
#define HWIO_LLCC3_LLCC_BEAC_ADDR_TRANSLATOR_CFG_ERROR_BMSK                                       0x3f0
#define HWIO_LLCC3_LLCC_BEAC_ADDR_TRANSLATOR_CFG_ERROR_SHFT                                         0x4
#define HWIO_LLCC3_LLCC_BEAC_ADDR_TRANSLATOR_CFG_ENABLE_BMSK                                        0x1
#define HWIO_LLCC3_LLCC_BEAC_ADDR_TRANSLATOR_CFG_ENABLE_SHFT                                        0x0

/*----------------------------------------------------------------------------
 * MODULE: LLCC4_BEAC
 *--------------------------------------------------------------------------*/

#define LLCC4_BEAC_REG_BASE                                                                  (DDR_SS_BASE      + 0x00448000)

#define HWIO_LLCC4_LLCC_BEAC_ADDR_TRANSLATOR_CFG_ADDR                                        (LLCC4_BEAC_REG_BASE      + 0x00000080)
#define HWIO_LLCC4_LLCC_BEAC_ADDR_TRANSLATOR_CFG_RMSK                                             0x3f1
#define HWIO_LLCC4_LLCC_BEAC_ADDR_TRANSLATOR_CFG_IN          \
        in_dword_masked(HWIO_LLCC4_LLCC_BEAC_ADDR_TRANSLATOR_CFG_ADDR, HWIO_LLCC4_LLCC_BEAC_ADDR_TRANSLATOR_CFG_RMSK)
#define HWIO_LLCC4_LLCC_BEAC_ADDR_TRANSLATOR_CFG_INM(m)      \
        in_dword_masked(HWIO_LLCC4_LLCC_BEAC_ADDR_TRANSLATOR_CFG_ADDR, m)
#define HWIO_LLCC4_LLCC_BEAC_ADDR_TRANSLATOR_CFG_OUT(v)      \
        out_dword(HWIO_LLCC4_LLCC_BEAC_ADDR_TRANSLATOR_CFG_ADDR,v)
#define HWIO_LLCC4_LLCC_BEAC_ADDR_TRANSLATOR_CFG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_LLCC4_LLCC_BEAC_ADDR_TRANSLATOR_CFG_ADDR,m,v,HWIO_LLCC4_LLCC_BEAC_ADDR_TRANSLATOR_CFG_IN)
#define HWIO_LLCC4_LLCC_BEAC_ADDR_TRANSLATOR_CFG_ERROR_BMSK                                       0x3f0
#define HWIO_LLCC4_LLCC_BEAC_ADDR_TRANSLATOR_CFG_ERROR_SHFT                                         0x4
#define HWIO_LLCC4_LLCC_BEAC_ADDR_TRANSLATOR_CFG_ENABLE_BMSK                                        0x1
#define HWIO_LLCC4_LLCC_BEAC_ADDR_TRANSLATOR_CFG_ENABLE_SHFT                                        0x0

/*----------------------------------------------------------------------------
 * MODULE: LLCC5_BEAC
 *--------------------------------------------------------------------------*/

#define LLCC5_BEAC_REG_BASE                                                                  (DDR_SS_BASE      + 0x004c8000)

#define HWIO_LLCC5_LLCC_BEAC_ADDR_TRANSLATOR_CFG_ADDR                                        (LLCC5_BEAC_REG_BASE      + 0x00000080)
#define HWIO_LLCC5_LLCC_BEAC_ADDR_TRANSLATOR_CFG_RMSK                                             0x3f1
#define HWIO_LLCC5_LLCC_BEAC_ADDR_TRANSLATOR_CFG_IN          \
        in_dword_masked(HWIO_LLCC5_LLCC_BEAC_ADDR_TRANSLATOR_CFG_ADDR, HWIO_LLCC5_LLCC_BEAC_ADDR_TRANSLATOR_CFG_RMSK)
#define HWIO_LLCC5_LLCC_BEAC_ADDR_TRANSLATOR_CFG_INM(m)      \
        in_dword_masked(HWIO_LLCC5_LLCC_BEAC_ADDR_TRANSLATOR_CFG_ADDR, m)
#define HWIO_LLCC5_LLCC_BEAC_ADDR_TRANSLATOR_CFG_OUT(v)      \
        out_dword(HWIO_LLCC5_LLCC_BEAC_ADDR_TRANSLATOR_CFG_ADDR,v)
#define HWIO_LLCC5_LLCC_BEAC_ADDR_TRANSLATOR_CFG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_LLCC5_LLCC_BEAC_ADDR_TRANSLATOR_CFG_ADDR,m,v,HWIO_LLCC5_LLCC_BEAC_ADDR_TRANSLATOR_CFG_IN)
#define HWIO_LLCC5_LLCC_BEAC_ADDR_TRANSLATOR_CFG_ERROR_BMSK                                       0x3f0
#define HWIO_LLCC5_LLCC_BEAC_ADDR_TRANSLATOR_CFG_ERROR_SHFT                                         0x4
#define HWIO_LLCC5_LLCC_BEAC_ADDR_TRANSLATOR_CFG_ENABLE_BMSK                                        0x1
#define HWIO_LLCC5_LLCC_BEAC_ADDR_TRANSLATOR_CFG_ENABLE_SHFT                                        0x0

/*----------------------------------------------------------------------------
 * MODULE: LLCC6_BEAC
 *--------------------------------------------------------------------------*/

#define LLCC6_BEAC_REG_BASE                                                                  (DDR_SS_BASE      + 0x00548000)

#define HWIO_LLCC6_LLCC_BEAC_ADDR_TRANSLATOR_CFG_ADDR                                        (LLCC6_BEAC_REG_BASE      + 0x00000080)
#define HWIO_LLCC6_LLCC_BEAC_ADDR_TRANSLATOR_CFG_RMSK                                             0x3f1
#define HWIO_LLCC6_LLCC_BEAC_ADDR_TRANSLATOR_CFG_IN          \
        in_dword_masked(HWIO_LLCC6_LLCC_BEAC_ADDR_TRANSLATOR_CFG_ADDR, HWIO_LLCC6_LLCC_BEAC_ADDR_TRANSLATOR_CFG_RMSK)
#define HWIO_LLCC6_LLCC_BEAC_ADDR_TRANSLATOR_CFG_INM(m)      \
        in_dword_masked(HWIO_LLCC6_LLCC_BEAC_ADDR_TRANSLATOR_CFG_ADDR, m)
#define HWIO_LLCC6_LLCC_BEAC_ADDR_TRANSLATOR_CFG_OUT(v)      \
        out_dword(HWIO_LLCC6_LLCC_BEAC_ADDR_TRANSLATOR_CFG_ADDR,v)
#define HWIO_LLCC6_LLCC_BEAC_ADDR_TRANSLATOR_CFG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_LLCC6_LLCC_BEAC_ADDR_TRANSLATOR_CFG_ADDR,m,v,HWIO_LLCC6_LLCC_BEAC_ADDR_TRANSLATOR_CFG_IN)
#define HWIO_LLCC6_LLCC_BEAC_ADDR_TRANSLATOR_CFG_ERROR_BMSK                                       0x3f0
#define HWIO_LLCC6_LLCC_BEAC_ADDR_TRANSLATOR_CFG_ERROR_SHFT                                         0x4
#define HWIO_LLCC6_LLCC_BEAC_ADDR_TRANSLATOR_CFG_ENABLE_BMSK                                        0x1
#define HWIO_LLCC6_LLCC_BEAC_ADDR_TRANSLATOR_CFG_ENABLE_SHFT                                        0x0

/*----------------------------------------------------------------------------
 * MODULE: LLCC7_BEAC
 *--------------------------------------------------------------------------*/

#define LLCC7_BEAC_REG_BASE                                                                  (DDR_SS_BASE      + 0x005c8000)

#define HWIO_LLCC7_LLCC_BEAC_ADDR_TRANSLATOR_CFG_ADDR                                        (LLCC7_BEAC_REG_BASE      + 0x00000080)
#define HWIO_LLCC7_LLCC_BEAC_ADDR_TRANSLATOR_CFG_RMSK                                             0x3f1
#define HWIO_LLCC7_LLCC_BEAC_ADDR_TRANSLATOR_CFG_IN          \
        in_dword_masked(HWIO_LLCC7_LLCC_BEAC_ADDR_TRANSLATOR_CFG_ADDR, HWIO_LLCC7_LLCC_BEAC_ADDR_TRANSLATOR_CFG_RMSK)
#define HWIO_LLCC7_LLCC_BEAC_ADDR_TRANSLATOR_CFG_INM(m)      \
        in_dword_masked(HWIO_LLCC7_LLCC_BEAC_ADDR_TRANSLATOR_CFG_ADDR, m)
#define HWIO_LLCC7_LLCC_BEAC_ADDR_TRANSLATOR_CFG_OUT(v)      \
        out_dword(HWIO_LLCC7_LLCC_BEAC_ADDR_TRANSLATOR_CFG_ADDR,v)
#define HWIO_LLCC7_LLCC_BEAC_ADDR_TRANSLATOR_CFG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_LLCC7_LLCC_BEAC_ADDR_TRANSLATOR_CFG_ADDR,m,v,HWIO_LLCC7_LLCC_BEAC_ADDR_TRANSLATOR_CFG_IN)
#define HWIO_LLCC7_LLCC_BEAC_ADDR_TRANSLATOR_CFG_ERROR_BMSK                                       0x3f0
#define HWIO_LLCC7_LLCC_BEAC_ADDR_TRANSLATOR_CFG_ERROR_SHFT                                         0x4
#define HWIO_LLCC7_LLCC_BEAC_ADDR_TRANSLATOR_CFG_ENABLE_BMSK                                        0x1
#define HWIO_LLCC7_LLCC_BEAC_ADDR_TRANSLATOR_CFG_ENABLE_SHFT                                        0x0

/*----------------------------------------------------------------------------
 * MODULE: TCSR_TCSR_REGS
 *--------------------------------------------------------------------------*/

#define TCSR_TCSR_REGS_REG_BASE                                                                                      (CORE_TOP_CSR_BASE      + 0x000c0000)
#define TCSR_TCSR_REGS_REG_BASE_SIZE                                                                                 0x30000
#define TCSR_TCSR_REGS_REG_BASE_USED                                                                                 0x2c004

#define HWIO_TCSR_TIMEOUT_INTR_STATUS_ADDR                                                                           (TCSR_TCSR_REGS_REG_BASE      + 0x00008020)
#define HWIO_TCSR_TIMEOUT_INTR_STATUS_RMSK                                                                           0xffffffff
#define HWIO_TCSR_TIMEOUT_INTR_STATUS_IN          \
        in_dword_masked(HWIO_TCSR_TIMEOUT_INTR_STATUS_ADDR, HWIO_TCSR_TIMEOUT_INTR_STATUS_RMSK)
#define HWIO_TCSR_TIMEOUT_INTR_STATUS_INM(m)      \
        in_dword_masked(HWIO_TCSR_TIMEOUT_INTR_STATUS_ADDR, m)
#define HWIO_TCSR_TIMEOUT_INTR_STATUS_RESERVE_31_TIMEOUT_SLAVE_INTR_BMSK                                             0x80000000
#define HWIO_TCSR_TIMEOUT_INTR_STATUS_RESERVE_31_TIMEOUT_SLAVE_INTR_SHFT                                                   0x1f
#define HWIO_TCSR_TIMEOUT_INTR_STATUS_RESERVE_30_TIMEOUT_SLAVE_INTR_BMSK                                             0x40000000
#define HWIO_TCSR_TIMEOUT_INTR_STATUS_RESERVE_30_TIMEOUT_SLAVE_INTR_SHFT                                                   0x1e
#define HWIO_TCSR_TIMEOUT_INTR_STATUS_RESERVE_29_TIMEOUT_SLAVE_INTR_BMSK                                             0x20000000
#define HWIO_TCSR_TIMEOUT_INTR_STATUS_RESERVE_29_TIMEOUT_SLAVE_INTR_SHFT                                                   0x1d
#define HWIO_TCSR_TIMEOUT_INTR_STATUS_RESERVE_28_TIMEOUT_SLAVE_INTR_BMSK                                             0x10000000
#define HWIO_TCSR_TIMEOUT_INTR_STATUS_RESERVE_28_TIMEOUT_SLAVE_INTR_SHFT                                                   0x1c
#define HWIO_TCSR_TIMEOUT_INTR_STATUS_RESERVE_27_TIMEOUT_SLAVE_INTR_BMSK                                              0x8000000
#define HWIO_TCSR_TIMEOUT_INTR_STATUS_RESERVE_27_TIMEOUT_SLAVE_INTR_SHFT                                                   0x1b
#define HWIO_TCSR_TIMEOUT_INTR_STATUS_RESERVE_26_TIMEOUT_SLAVE_INTR_BMSK                                              0x4000000
#define HWIO_TCSR_TIMEOUT_INTR_STATUS_RESERVE_26_TIMEOUT_SLAVE_INTR_SHFT                                                   0x1a
#define HWIO_TCSR_TIMEOUT_INTR_STATUS_RESERVE_25_TIMEOUT_SLAVE_INTR_BMSK                                              0x2000000
#define HWIO_TCSR_TIMEOUT_INTR_STATUS_RESERVE_25_TIMEOUT_SLAVE_INTR_SHFT                                                   0x19
#define HWIO_TCSR_TIMEOUT_INTR_STATUS_WCSS_ECAHB_TSLV_INTR_BMSK                                                       0x1000000
#define HWIO_TCSR_TIMEOUT_INTR_STATUS_WCSS_ECAHB_TSLV_INTR_SHFT                                                            0x18
#define HWIO_TCSR_TIMEOUT_INTR_STATUS_LPASS_IRQ_OUT_AHB_TIMEOUT2_BMSK                                                  0x800000
#define HWIO_TCSR_TIMEOUT_INTR_STATUS_LPASS_IRQ_OUT_AHB_TIMEOUT2_SHFT                                                      0x17
#define HWIO_TCSR_TIMEOUT_INTR_STATUS_LPASS_IRQ_OUT_AHB_TIMEOUT1_BMSK                                                  0x400000
#define HWIO_TCSR_TIMEOUT_INTR_STATUS_LPASS_IRQ_OUT_AHB_TIMEOUT1_SHFT                                                      0x16
#define HWIO_TCSR_TIMEOUT_INTR_STATUS_LPASS_IRQ_OUT_AHB_TIMEOUT0_BMSK                                                  0x200000
#define HWIO_TCSR_TIMEOUT_INTR_STATUS_LPASS_IRQ_OUT_AHB_TIMEOUT0_SHFT                                                      0x15
#define HWIO_TCSR_TIMEOUT_INTR_STATUS_WCSS_TCSR_WAHB_TSLV_INTR_BMSK                                                    0x100000
#define HWIO_TCSR_TIMEOUT_INTR_STATUS_WCSS_TCSR_WAHB_TSLV_INTR_SHFT                                                        0x14
#define HWIO_TCSR_TIMEOUT_INTR_STATUS_TURING_IRQ_OUT_AHB_TIMEOUT1_BMSK                                                  0x80000
#define HWIO_TCSR_TIMEOUT_INTR_STATUS_TURING_IRQ_OUT_AHB_TIMEOUT1_SHFT                                                     0x13
#define HWIO_TCSR_TIMEOUT_INTR_STATUS_TURING_IRQ_OUT_AHB_TIMEOUT0_BMSK                                                  0x40000
#define HWIO_TCSR_TIMEOUT_INTR_STATUS_TURING_IRQ_OUT_AHB_TIMEOUT0_SHFT                                                     0x12
#define HWIO_TCSR_TIMEOUT_INTR_STATUS_RESERVE_17_TIMEOUT_SLAVE_INTR_BMSK                                                0x20000
#define HWIO_TCSR_TIMEOUT_INTR_STATUS_RESERVE_17_TIMEOUT_SLAVE_INTR_SHFT                                                   0x11
#define HWIO_TCSR_TIMEOUT_INTR_STATUS_RESERVE_16_TIMEOUT_SLAVE_INTR_BMSK                                                0x10000
#define HWIO_TCSR_TIMEOUT_INTR_STATUS_RESERVE_16_TIMEOUT_SLAVE_INTR_SHFT                                                   0x10
#define HWIO_TCSR_TIMEOUT_INTR_STATUS_RESERVE_15_TIMEOUT_SLAVE_INTR_BMSK                                                 0x8000
#define HWIO_TCSR_TIMEOUT_INTR_STATUS_RESERVE_15_TIMEOUT_SLAVE_INTR_SHFT                                                    0xf
#define HWIO_TCSR_TIMEOUT_INTR_STATUS_RESERVE_14_TIMEOUT_SLAVE_INTR_BMSK                                                 0x4000
#define HWIO_TCSR_TIMEOUT_INTR_STATUS_RESERVE_14_TIMEOUT_SLAVE_INTR_SHFT                                                    0xe
#define HWIO_TCSR_TIMEOUT_INTR_STATUS_RESERVE_13_TIMEOUT_SLAVE_INTR_BMSK                                                 0x2000
#define HWIO_TCSR_TIMEOUT_INTR_STATUS_RESERVE_13_TIMEOUT_SLAVE_INTR_SHFT                                                    0xd
#define HWIO_TCSR_TIMEOUT_INTR_STATUS_RESERVE_12_TIMEOUT_SLAVE_INTR_BMSK                                                 0x1000
#define HWIO_TCSR_TIMEOUT_INTR_STATUS_RESERVE_12_TIMEOUT_SLAVE_INTR_SHFT                                                    0xc
#define HWIO_TCSR_TIMEOUT_INTR_STATUS_WCSS_TCSR_WAPB_TSLV_IRQ_BMSK                                                        0x800
#define HWIO_TCSR_TIMEOUT_INTR_STATUS_WCSS_TCSR_WAPB_TSLV_IRQ_SHFT                                                          0xb
#define HWIO_TCSR_TIMEOUT_INTR_STATUS_MSS_CONFIG_TIMEOUT_SLAVE_IRQ_BMSK                                                   0x400
#define HWIO_TCSR_TIMEOUT_INTR_STATUS_MSS_CONFIG_TIMEOUT_SLAVE_IRQ_SHFT                                                     0xa
#define HWIO_TCSR_TIMEOUT_INTR_STATUS_RESERVE_9_TIMEOUT_SLAVE_INTR_BMSK                                                   0x200
#define HWIO_TCSR_TIMEOUT_INTR_STATUS_RESERVE_9_TIMEOUT_SLAVE_INTR_SHFT                                                     0x9
#define HWIO_TCSR_TIMEOUT_INTR_STATUS_RESERVE_8_TIMEOUT_SLAVE_INTR_BMSK                                                   0x100
#define HWIO_TCSR_TIMEOUT_INTR_STATUS_RESERVE_8_TIMEOUT_SLAVE_INTR_SHFT                                                     0x8
#define HWIO_TCSR_TIMEOUT_INTR_STATUS_RESERVE_7_TIMEOUT_SLAVE_INTR_BMSK                                                    0x80
#define HWIO_TCSR_TIMEOUT_INTR_STATUS_RESERVE_7_TIMEOUT_SLAVE_INTR_SHFT                                                     0x7
#define HWIO_TCSR_TIMEOUT_INTR_STATUS_WCSS_TCSR_NOC_INTR_BMSK                                                              0x40
#define HWIO_TCSR_TIMEOUT_INTR_STATUS_WCSS_TCSR_NOC_INTR_SHFT                                                               0x6
#define HWIO_TCSR_TIMEOUT_INTR_STATUS_RESERVE_5_TIMEOUT_SLAVE_INTR_BMSK                                                    0x20
#define HWIO_TCSR_TIMEOUT_INTR_STATUS_RESERVE_5_TIMEOUT_SLAVE_INTR_SHFT                                                     0x5
#define HWIO_TCSR_TIMEOUT_INTR_STATUS_RESERVE_4_TIMEOUT_SLAVE_INTR_BMSK                                                    0x10
#define HWIO_TCSR_TIMEOUT_INTR_STATUS_RESERVE_4_TIMEOUT_SLAVE_INTR_SHFT                                                     0x4
#define HWIO_TCSR_TIMEOUT_INTR_STATUS_RESERVE_3_TIMEOUT_SLAVE_INTR_BMSK                                                     0x8
#define HWIO_TCSR_TIMEOUT_INTR_STATUS_RESERVE_3_TIMEOUT_SLAVE_INTR_SHFT                                                     0x3
#define HWIO_TCSR_TIMEOUT_INTR_STATUS_RESERVE_2_TIMEOUT_SLAVE_INTR_BMSK                                                     0x4
#define HWIO_TCSR_TIMEOUT_INTR_STATUS_RESERVE_2_TIMEOUT_SLAVE_INTR_SHFT                                                     0x2
#define HWIO_TCSR_TIMEOUT_INTR_STATUS_RESERVE_1_TIMEOUT_SLAVE_INTR_BMSK                                                     0x2
#define HWIO_TCSR_TIMEOUT_INTR_STATUS_RESERVE_1_TIMEOUT_SLAVE_INTR_SHFT                                                     0x1
#define HWIO_TCSR_TIMEOUT_INTR_STATUS_RESERVE_0_TIMEOUT_SLAVE_INTR_BMSK                                                     0x1
#define HWIO_TCSR_TIMEOUT_INTR_STATUS_RESERVE_0_TIMEOUT_SLAVE_INTR_SHFT                                                     0x0


#endif /* __ICBCFG_HWIO_TARGET_H__ */
