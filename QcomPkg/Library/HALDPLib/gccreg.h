#ifndef __GCCREG__
#define __GCCREG__
/*
===========================================================================
*/
/**
  @file gccreg
  @brief Auto-generated HWIO interface include file.

  Reference chip release:
    SC8180X (Poipu) v2 [poipu_v2.0_p3q3r67_MTO]
 
  This file contains HWIO register definitions for the following modules:
    GCC_CLK_CTL_REG

  'Include' filters applied: .*USB.*[GCC_CLK_CTL_REG] 
  'Exclude' filters applied: DUMMY RESERVED 

  Generation parameters: 
  { 'exclude-no-doc': True,
    'exclude-reserved': True,
    'filename': 'gccreg',
    'module-filter-exclude': {},
    'module-filter-include': {'GCC_CLK_CTL_REG': ['.*USB.*']},
    'modules': ['GCC_CLK_CTL_REG'],
    'output-fvals': True,
    'output-offsets': True,
    'output-phys': True}
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

  $Header: //components/rel/boot.xf/3.0.1.c1/QcomPkg/Library/HALDPLib/gccreg.h#1 $
  $DateTime: 2021/04/29 02:01:56 $
  $Author: pwbldsvc $

  ===========================================================================
*/

/*----------------------------------------------------------------------------
 * MODULE: GCC_CLK_CTL_REG
 *--------------------------------------------------------------------------*/

#define CLK_CTL_BASE                                                                               0x00100000
#define CLK_CTL_BASE_SIZE                                                                          0x00200000
#define CLK_CTL_BASE_PHYS                                                                          0x00100000

#define GCC_CLK_CTL_REG_REG_BASE                                                                   (CLK_CTL_BASE      + 0x00000000)
#define GCC_CLK_CTL_REG_REG_BASE_SIZE                                                              0x1f0000
#define GCC_CLK_CTL_REG_REG_BASE_USED                                                              0xc0004
#define GCC_CLK_CTL_REG_REG_BASE_PHYS                                                              (CLK_CTL_BASE_PHYS + 0x00000000)
#define GCC_CLK_CTL_REG_REG_BASE_OFFS                                                              0x00000000

#define HWIO_GCC_CFG_NOC_USB3_PRIM_AXI_CBCR_ADDR                                                   (GCC_CLK_CTL_REG_REG_BASE      + 0x0000f078)
#define HWIO_GCC_CFG_NOC_USB3_PRIM_AXI_CBCR_PHYS                                                   (GCC_CLK_CTL_REG_REG_BASE_PHYS + 0x0000f078)
#define HWIO_GCC_CFG_NOC_USB3_PRIM_AXI_CBCR_OFFS                                                   (GCC_CLK_CTL_REG_REG_BASE_OFFS + 0x0000f078)
#define HWIO_GCC_CFG_NOC_USB3_PRIM_AXI_CBCR_RMSK                                                   0x80000005
#define HWIO_GCC_CFG_NOC_USB3_PRIM_AXI_CBCR_IN          \
        in_dword(HWIO_GCC_CFG_NOC_USB3_PRIM_AXI_CBCR_ADDR)
#define HWIO_GCC_CFG_NOC_USB3_PRIM_AXI_CBCR_INM(m)      \
        in_dword_masked(HWIO_GCC_CFG_NOC_USB3_PRIM_AXI_CBCR_ADDR, m)
#define HWIO_GCC_CFG_NOC_USB3_PRIM_AXI_CBCR_OUT(v)      \
        out_dword(HWIO_GCC_CFG_NOC_USB3_PRIM_AXI_CBCR_ADDR,v)
#define HWIO_GCC_CFG_NOC_USB3_PRIM_AXI_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GCC_CFG_NOC_USB3_PRIM_AXI_CBCR_ADDR,m,v,HWIO_GCC_CFG_NOC_USB3_PRIM_AXI_CBCR_IN)
#define HWIO_GCC_CFG_NOC_USB3_PRIM_AXI_CBCR_CLK_OFF_BMSK                                           0x80000000
#define HWIO_GCC_CFG_NOC_USB3_PRIM_AXI_CBCR_CLK_OFF_SHFT                                                 0x1f
#define HWIO_GCC_CFG_NOC_USB3_PRIM_AXI_CBCR_CLK_ARES_BMSK                                                 0x4
#define HWIO_GCC_CFG_NOC_USB3_PRIM_AXI_CBCR_CLK_ARES_SHFT                                                 0x2
#define HWIO_GCC_CFG_NOC_USB3_PRIM_AXI_CBCR_CLK_ARES_NO_RESET_FVAL                                        0x0
#define HWIO_GCC_CFG_NOC_USB3_PRIM_AXI_CBCR_CLK_ARES_RESET_FVAL                                           0x1
#define HWIO_GCC_CFG_NOC_USB3_PRIM_AXI_CBCR_CLK_ENABLE_BMSK                                               0x1
#define HWIO_GCC_CFG_NOC_USB3_PRIM_AXI_CBCR_CLK_ENABLE_SHFT                                               0x0
#define HWIO_GCC_CFG_NOC_USB3_PRIM_AXI_CBCR_CLK_ENABLE_DISABLE_FVAL                                       0x0
#define HWIO_GCC_CFG_NOC_USB3_PRIM_AXI_CBCR_CLK_ENABLE_ENABLE_FVAL                                        0x1

#define HWIO_GCC_CFG_NOC_USB3_SEC_AXI_CBCR_ADDR                                                    (GCC_CLK_CTL_REG_REG_BASE      + 0x00010078)
#define HWIO_GCC_CFG_NOC_USB3_SEC_AXI_CBCR_PHYS                                                    (GCC_CLK_CTL_REG_REG_BASE_PHYS + 0x00010078)
#define HWIO_GCC_CFG_NOC_USB3_SEC_AXI_CBCR_OFFS                                                    (GCC_CLK_CTL_REG_REG_BASE_OFFS + 0x00010078)
#define HWIO_GCC_CFG_NOC_USB3_SEC_AXI_CBCR_RMSK                                                    0x80000005
#define HWIO_GCC_CFG_NOC_USB3_SEC_AXI_CBCR_IN          \
        in_dword(HWIO_GCC_CFG_NOC_USB3_SEC_AXI_CBCR_ADDR)
#define HWIO_GCC_CFG_NOC_USB3_SEC_AXI_CBCR_INM(m)      \
        in_dword_masked(HWIO_GCC_CFG_NOC_USB3_SEC_AXI_CBCR_ADDR, m)
#define HWIO_GCC_CFG_NOC_USB3_SEC_AXI_CBCR_OUT(v)      \
        out_dword(HWIO_GCC_CFG_NOC_USB3_SEC_AXI_CBCR_ADDR,v)
#define HWIO_GCC_CFG_NOC_USB3_SEC_AXI_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GCC_CFG_NOC_USB3_SEC_AXI_CBCR_ADDR,m,v,HWIO_GCC_CFG_NOC_USB3_SEC_AXI_CBCR_IN)
#define HWIO_GCC_CFG_NOC_USB3_SEC_AXI_CBCR_CLK_OFF_BMSK                                            0x80000000
#define HWIO_GCC_CFG_NOC_USB3_SEC_AXI_CBCR_CLK_OFF_SHFT                                                  0x1f
#define HWIO_GCC_CFG_NOC_USB3_SEC_AXI_CBCR_CLK_ARES_BMSK                                                  0x4
#define HWIO_GCC_CFG_NOC_USB3_SEC_AXI_CBCR_CLK_ARES_SHFT                                                  0x2
#define HWIO_GCC_CFG_NOC_USB3_SEC_AXI_CBCR_CLK_ARES_NO_RESET_FVAL                                         0x0
#define HWIO_GCC_CFG_NOC_USB3_SEC_AXI_CBCR_CLK_ARES_RESET_FVAL                                            0x1
#define HWIO_GCC_CFG_NOC_USB3_SEC_AXI_CBCR_CLK_ENABLE_BMSK                                                0x1
#define HWIO_GCC_CFG_NOC_USB3_SEC_AXI_CBCR_CLK_ENABLE_SHFT                                                0x0
#define HWIO_GCC_CFG_NOC_USB3_SEC_AXI_CBCR_CLK_ENABLE_DISABLE_FVAL                                        0x0
#define HWIO_GCC_CFG_NOC_USB3_SEC_AXI_CBCR_CLK_ENABLE_ENABLE_FVAL                                         0x1

#define HWIO_GCC_CFG_NOC_USB3_MP_AXI_CBCR_ADDR                                                     (GCC_CLK_CTL_REG_REG_BASE      + 0x000a609c)
#define HWIO_GCC_CFG_NOC_USB3_MP_AXI_CBCR_PHYS                                                     (GCC_CLK_CTL_REG_REG_BASE_PHYS + 0x000a609c)
#define HWIO_GCC_CFG_NOC_USB3_MP_AXI_CBCR_OFFS                                                     (GCC_CLK_CTL_REG_REG_BASE_OFFS + 0x000a609c)
#define HWIO_GCC_CFG_NOC_USB3_MP_AXI_CBCR_RMSK                                                     0x80000005
#define HWIO_GCC_CFG_NOC_USB3_MP_AXI_CBCR_IN          \
        in_dword(HWIO_GCC_CFG_NOC_USB3_MP_AXI_CBCR_ADDR)
#define HWIO_GCC_CFG_NOC_USB3_MP_AXI_CBCR_INM(m)      \
        in_dword_masked(HWIO_GCC_CFG_NOC_USB3_MP_AXI_CBCR_ADDR, m)
#define HWIO_GCC_CFG_NOC_USB3_MP_AXI_CBCR_OUT(v)      \
        out_dword(HWIO_GCC_CFG_NOC_USB3_MP_AXI_CBCR_ADDR,v)
#define HWIO_GCC_CFG_NOC_USB3_MP_AXI_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GCC_CFG_NOC_USB3_MP_AXI_CBCR_ADDR,m,v,HWIO_GCC_CFG_NOC_USB3_MP_AXI_CBCR_IN)
#define HWIO_GCC_CFG_NOC_USB3_MP_AXI_CBCR_CLK_OFF_BMSK                                             0x80000000
#define HWIO_GCC_CFG_NOC_USB3_MP_AXI_CBCR_CLK_OFF_SHFT                                                   0x1f
#define HWIO_GCC_CFG_NOC_USB3_MP_AXI_CBCR_CLK_ARES_BMSK                                                   0x4
#define HWIO_GCC_CFG_NOC_USB3_MP_AXI_CBCR_CLK_ARES_SHFT                                                   0x2
#define HWIO_GCC_CFG_NOC_USB3_MP_AXI_CBCR_CLK_ARES_NO_RESET_FVAL                                          0x0
#define HWIO_GCC_CFG_NOC_USB3_MP_AXI_CBCR_CLK_ARES_RESET_FVAL                                             0x1
#define HWIO_GCC_CFG_NOC_USB3_MP_AXI_CBCR_CLK_ENABLE_BMSK                                                 0x1
#define HWIO_GCC_CFG_NOC_USB3_MP_AXI_CBCR_CLK_ENABLE_SHFT                                                 0x0
#define HWIO_GCC_CFG_NOC_USB3_MP_AXI_CBCR_CLK_ENABLE_DISABLE_FVAL                                         0x0
#define HWIO_GCC_CFG_NOC_USB3_MP_AXI_CBCR_CLK_ENABLE_ENABLE_FVAL                                          0x1

#define HWIO_GCC_QDSS_ETR_USB_CBCR_ADDR                                                            (GCC_CLK_CTL_REG_REG_BASE      + 0x0000c01c)
#define HWIO_GCC_QDSS_ETR_USB_CBCR_PHYS                                                            (GCC_CLK_CTL_REG_REG_BASE_PHYS + 0x0000c01c)
#define HWIO_GCC_QDSS_ETR_USB_CBCR_OFFS                                                            (GCC_CLK_CTL_REG_REG_BASE_OFFS + 0x0000c01c)
#define HWIO_GCC_QDSS_ETR_USB_CBCR_RMSK                                                            0x80100005
#define HWIO_GCC_QDSS_ETR_USB_CBCR_IN          \
        in_dword(HWIO_GCC_QDSS_ETR_USB_CBCR_ADDR)
#define HWIO_GCC_QDSS_ETR_USB_CBCR_INM(m)      \
        in_dword_masked(HWIO_GCC_QDSS_ETR_USB_CBCR_ADDR, m)
#define HWIO_GCC_QDSS_ETR_USB_CBCR_OUT(v)      \
        out_dword(HWIO_GCC_QDSS_ETR_USB_CBCR_ADDR,v)
#define HWIO_GCC_QDSS_ETR_USB_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GCC_QDSS_ETR_USB_CBCR_ADDR,m,v,HWIO_GCC_QDSS_ETR_USB_CBCR_IN)
#define HWIO_GCC_QDSS_ETR_USB_CBCR_CLK_OFF_BMSK                                                    0x80000000
#define HWIO_GCC_QDSS_ETR_USB_CBCR_CLK_OFF_SHFT                                                          0x1f
#define HWIO_GCC_QDSS_ETR_USB_CBCR_IGNORE_RPMH_CLK_DIS_BMSK                                          0x100000
#define HWIO_GCC_QDSS_ETR_USB_CBCR_IGNORE_RPMH_CLK_DIS_SHFT                                              0x14
#define HWIO_GCC_QDSS_ETR_USB_CBCR_CLK_ARES_BMSK                                                          0x4
#define HWIO_GCC_QDSS_ETR_USB_CBCR_CLK_ARES_SHFT                                                          0x2
#define HWIO_GCC_QDSS_ETR_USB_CBCR_CLK_ARES_NO_RESET_FVAL                                                 0x0
#define HWIO_GCC_QDSS_ETR_USB_CBCR_CLK_ARES_RESET_FVAL                                                    0x1
#define HWIO_GCC_QDSS_ETR_USB_CBCR_CLK_ENABLE_BMSK                                                        0x1
#define HWIO_GCC_QDSS_ETR_USB_CBCR_CLK_ENABLE_SHFT                                                        0x0
#define HWIO_GCC_QDSS_ETR_USB_CBCR_CLK_ENABLE_DISABLE_FVAL                                                0x0
#define HWIO_GCC_QDSS_ETR_USB_CBCR_CLK_ENABLE_ENABLE_FVAL                                                 0x1

#define HWIO_GCC_USB30_PRIM_BCR_ADDR                                                               (GCC_CLK_CTL_REG_REG_BASE      + 0x0000f000)
#define HWIO_GCC_USB30_PRIM_BCR_PHYS                                                               (GCC_CLK_CTL_REG_REG_BASE_PHYS + 0x0000f000)
#define HWIO_GCC_USB30_PRIM_BCR_OFFS                                                               (GCC_CLK_CTL_REG_REG_BASE_OFFS + 0x0000f000)
#define HWIO_GCC_USB30_PRIM_BCR_RMSK                                                                      0x1
#define HWIO_GCC_USB30_PRIM_BCR_IN          \
        in_dword(HWIO_GCC_USB30_PRIM_BCR_ADDR)
#define HWIO_GCC_USB30_PRIM_BCR_INM(m)      \
        in_dword_masked(HWIO_GCC_USB30_PRIM_BCR_ADDR, m)
#define HWIO_GCC_USB30_PRIM_BCR_OUT(v)      \
        out_dword(HWIO_GCC_USB30_PRIM_BCR_ADDR,v)
#define HWIO_GCC_USB30_PRIM_BCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GCC_USB30_PRIM_BCR_ADDR,m,v,HWIO_GCC_USB30_PRIM_BCR_IN)
#define HWIO_GCC_USB30_PRIM_BCR_BLK_ARES_BMSK                                                             0x1
#define HWIO_GCC_USB30_PRIM_BCR_BLK_ARES_SHFT                                                             0x0
#define HWIO_GCC_USB30_PRIM_BCR_BLK_ARES_DISABLE_FVAL                                                     0x0
#define HWIO_GCC_USB30_PRIM_BCR_BLK_ARES_ENABLE_FVAL                                                      0x1

#define HWIO_GCC_USB30_PRIM_GDSCR_ADDR                                                             (GCC_CLK_CTL_REG_REG_BASE      + 0x0000f004)
#define HWIO_GCC_USB30_PRIM_GDSCR_PHYS                                                             (GCC_CLK_CTL_REG_REG_BASE_PHYS + 0x0000f004)
#define HWIO_GCC_USB30_PRIM_GDSCR_OFFS                                                             (GCC_CLK_CTL_REG_REG_BASE_OFFS + 0x0000f004)
#define HWIO_GCC_USB30_PRIM_GDSCR_RMSK                                                             0xf8ffffff
#define HWIO_GCC_USB30_PRIM_GDSCR_IN          \
        in_dword(HWIO_GCC_USB30_PRIM_GDSCR_ADDR)
#define HWIO_GCC_USB30_PRIM_GDSCR_INM(m)      \
        in_dword_masked(HWIO_GCC_USB30_PRIM_GDSCR_ADDR, m)
#define HWIO_GCC_USB30_PRIM_GDSCR_OUT(v)      \
        out_dword(HWIO_GCC_USB30_PRIM_GDSCR_ADDR,v)
#define HWIO_GCC_USB30_PRIM_GDSCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GCC_USB30_PRIM_GDSCR_ADDR,m,v,HWIO_GCC_USB30_PRIM_GDSCR_IN)
#define HWIO_GCC_USB30_PRIM_GDSCR_PWR_ON_BMSK                                                      0x80000000
#define HWIO_GCC_USB30_PRIM_GDSCR_PWR_ON_SHFT                                                            0x1f
#define HWIO_GCC_USB30_PRIM_GDSCR_GDSC_STATE_BMSK                                                  0x78000000
#define HWIO_GCC_USB30_PRIM_GDSCR_GDSC_STATE_SHFT                                                        0x1b
#define HWIO_GCC_USB30_PRIM_GDSCR_EN_REST_WAIT_BMSK                                                  0xf00000
#define HWIO_GCC_USB30_PRIM_GDSCR_EN_REST_WAIT_SHFT                                                      0x14
#define HWIO_GCC_USB30_PRIM_GDSCR_EN_FEW_WAIT_BMSK                                                    0xf0000
#define HWIO_GCC_USB30_PRIM_GDSCR_EN_FEW_WAIT_SHFT                                                       0x10
#define HWIO_GCC_USB30_PRIM_GDSCR_CLK_DIS_WAIT_BMSK                                                    0xf000
#define HWIO_GCC_USB30_PRIM_GDSCR_CLK_DIS_WAIT_SHFT                                                       0xc
#define HWIO_GCC_USB30_PRIM_GDSCR_RETAIN_FF_ENABLE_BMSK                                                 0x800
#define HWIO_GCC_USB30_PRIM_GDSCR_RETAIN_FF_ENABLE_SHFT                                                   0xb
#define HWIO_GCC_USB30_PRIM_GDSCR_RETAIN_FF_ENABLE_DISABLE_FVAL                                           0x0
#define HWIO_GCC_USB30_PRIM_GDSCR_RETAIN_FF_ENABLE_ENABLE_FVAL                                            0x1
#define HWIO_GCC_USB30_PRIM_GDSCR_RESTORE_BMSK                                                          0x400
#define HWIO_GCC_USB30_PRIM_GDSCR_RESTORE_SHFT                                                            0xa
#define HWIO_GCC_USB30_PRIM_GDSCR_RESTORE_DISABLE_FVAL                                                    0x0
#define HWIO_GCC_USB30_PRIM_GDSCR_RESTORE_ENABLE_FVAL                                                     0x1
#define HWIO_GCC_USB30_PRIM_GDSCR_SAVE_BMSK                                                             0x200
#define HWIO_GCC_USB30_PRIM_GDSCR_SAVE_SHFT                                                               0x9
#define HWIO_GCC_USB30_PRIM_GDSCR_SAVE_DISABLE_FVAL                                                       0x0
#define HWIO_GCC_USB30_PRIM_GDSCR_SAVE_ENABLE_FVAL                                                        0x1
#define HWIO_GCC_USB30_PRIM_GDSCR_RETAIN_BMSK                                                           0x100
#define HWIO_GCC_USB30_PRIM_GDSCR_RETAIN_SHFT                                                             0x8
#define HWIO_GCC_USB30_PRIM_GDSCR_RETAIN_DISABLE_FVAL                                                     0x0
#define HWIO_GCC_USB30_PRIM_GDSCR_RETAIN_ENABLE_FVAL                                                      0x1
#define HWIO_GCC_USB30_PRIM_GDSCR_EN_REST_BMSK                                                           0x80
#define HWIO_GCC_USB30_PRIM_GDSCR_EN_REST_SHFT                                                            0x7
#define HWIO_GCC_USB30_PRIM_GDSCR_EN_REST_DISABLE_FVAL                                                    0x0
#define HWIO_GCC_USB30_PRIM_GDSCR_EN_REST_ENABLE_FVAL                                                     0x1
#define HWIO_GCC_USB30_PRIM_GDSCR_EN_FEW_BMSK                                                            0x40
#define HWIO_GCC_USB30_PRIM_GDSCR_EN_FEW_SHFT                                                             0x6
#define HWIO_GCC_USB30_PRIM_GDSCR_EN_FEW_DISABLE_FVAL                                                     0x0
#define HWIO_GCC_USB30_PRIM_GDSCR_EN_FEW_ENABLE_FVAL                                                      0x1
#define HWIO_GCC_USB30_PRIM_GDSCR_CLAMP_IO_BMSK                                                          0x20
#define HWIO_GCC_USB30_PRIM_GDSCR_CLAMP_IO_SHFT                                                           0x5
#define HWIO_GCC_USB30_PRIM_GDSCR_CLAMP_IO_DISABLE_FVAL                                                   0x0
#define HWIO_GCC_USB30_PRIM_GDSCR_CLAMP_IO_ENABLE_FVAL                                                    0x1
#define HWIO_GCC_USB30_PRIM_GDSCR_CLK_DISABLE_BMSK                                                       0x10
#define HWIO_GCC_USB30_PRIM_GDSCR_CLK_DISABLE_SHFT                                                        0x4
#define HWIO_GCC_USB30_PRIM_GDSCR_CLK_DISABLE_CLK_NOT_DISABLE_FVAL                                        0x0
#define HWIO_GCC_USB30_PRIM_GDSCR_CLK_DISABLE_CLK_DISABLE_FVAL                                            0x1
#define HWIO_GCC_USB30_PRIM_GDSCR_PD_ARES_BMSK                                                            0x8
#define HWIO_GCC_USB30_PRIM_GDSCR_PD_ARES_SHFT                                                            0x3
#define HWIO_GCC_USB30_PRIM_GDSCR_PD_ARES_NO_RESET_FVAL                                                   0x0
#define HWIO_GCC_USB30_PRIM_GDSCR_PD_ARES_RESET_FVAL                                                      0x1
#define HWIO_GCC_USB30_PRIM_GDSCR_SW_OVERRIDE_BMSK                                                        0x4
#define HWIO_GCC_USB30_PRIM_GDSCR_SW_OVERRIDE_SHFT                                                        0x2
#define HWIO_GCC_USB30_PRIM_GDSCR_SW_OVERRIDE_DISABLE_FVAL                                                0x0
#define HWIO_GCC_USB30_PRIM_GDSCR_SW_OVERRIDE_ENABLE_FVAL                                                 0x1
#define HWIO_GCC_USB30_PRIM_GDSCR_HW_CONTROL_BMSK                                                         0x2
#define HWIO_GCC_USB30_PRIM_GDSCR_HW_CONTROL_SHFT                                                         0x1
#define HWIO_GCC_USB30_PRIM_GDSCR_HW_CONTROL_DISABLE_FVAL                                                 0x0
#define HWIO_GCC_USB30_PRIM_GDSCR_HW_CONTROL_ENABLE_FVAL                                                  0x1
#define HWIO_GCC_USB30_PRIM_GDSCR_SW_COLLAPSE_BMSK                                                        0x1
#define HWIO_GCC_USB30_PRIM_GDSCR_SW_COLLAPSE_SHFT                                                        0x0
#define HWIO_GCC_USB30_PRIM_GDSCR_SW_COLLAPSE_DISABLE_FVAL                                                0x0
#define HWIO_GCC_USB30_PRIM_GDSCR_SW_COLLAPSE_ENABLE_FVAL                                                 0x1

#define HWIO_GCC_USB30_PRIM_CFG_GDSCR_ADDR                                                         (GCC_CLK_CTL_REG_REG_BASE      + 0x0000f008)
#define HWIO_GCC_USB30_PRIM_CFG_GDSCR_PHYS                                                         (GCC_CLK_CTL_REG_REG_BASE_PHYS + 0x0000f008)
#define HWIO_GCC_USB30_PRIM_CFG_GDSCR_OFFS                                                         (GCC_CLK_CTL_REG_REG_BASE_OFFS + 0x0000f008)
#define HWIO_GCC_USB30_PRIM_CFG_GDSCR_RMSK                                                         0xffffffff
#define HWIO_GCC_USB30_PRIM_CFG_GDSCR_IN          \
        in_dword(HWIO_GCC_USB30_PRIM_CFG_GDSCR_ADDR)
#define HWIO_GCC_USB30_PRIM_CFG_GDSCR_INM(m)      \
        in_dword_masked(HWIO_GCC_USB30_PRIM_CFG_GDSCR_ADDR, m)
#define HWIO_GCC_USB30_PRIM_CFG_GDSCR_OUT(v)      \
        out_dword(HWIO_GCC_USB30_PRIM_CFG_GDSCR_ADDR,v)
#define HWIO_GCC_USB30_PRIM_CFG_GDSCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GCC_USB30_PRIM_CFG_GDSCR_ADDR,m,v,HWIO_GCC_USB30_PRIM_CFG_GDSCR_IN)
#define HWIO_GCC_USB30_PRIM_CFG_GDSCR_GDSC_SPARE_CTRL_IN_BMSK                                      0xf0000000
#define HWIO_GCC_USB30_PRIM_CFG_GDSCR_GDSC_SPARE_CTRL_IN_SHFT                                            0x1c
#define HWIO_GCC_USB30_PRIM_CFG_GDSCR_GDSC_SPARE_CTRL_OUT_BMSK                                      0xc000000
#define HWIO_GCC_USB30_PRIM_CFG_GDSCR_GDSC_SPARE_CTRL_OUT_SHFT                                           0x1a
#define HWIO_GCC_USB30_PRIM_CFG_GDSCR_GDSC_PWR_DWN_START_BMSK                                       0x2000000
#define HWIO_GCC_USB30_PRIM_CFG_GDSCR_GDSC_PWR_DWN_START_SHFT                                            0x19
#define HWIO_GCC_USB30_PRIM_CFG_GDSCR_GDSC_PWR_UP_START_BMSK                                        0x1000000
#define HWIO_GCC_USB30_PRIM_CFG_GDSCR_GDSC_PWR_UP_START_SHFT                                             0x18
#define HWIO_GCC_USB30_PRIM_CFG_GDSCR_GDSC_CFG_FSM_STATE_STATUS_BMSK                                 0xf00000
#define HWIO_GCC_USB30_PRIM_CFG_GDSCR_GDSC_CFG_FSM_STATE_STATUS_SHFT                                     0x14
#define HWIO_GCC_USB30_PRIM_CFG_GDSCR_GDSC_MEM_PWR_ACK_STATUS_BMSK                                    0x80000
#define HWIO_GCC_USB30_PRIM_CFG_GDSCR_GDSC_MEM_PWR_ACK_STATUS_SHFT                                       0x13
#define HWIO_GCC_USB30_PRIM_CFG_GDSCR_GDSC_ENR_ACK_STATUS_BMSK                                        0x40000
#define HWIO_GCC_USB30_PRIM_CFG_GDSCR_GDSC_ENR_ACK_STATUS_SHFT                                           0x12
#define HWIO_GCC_USB30_PRIM_CFG_GDSCR_GDSC_ENF_ACK_STATUS_BMSK                                        0x20000
#define HWIO_GCC_USB30_PRIM_CFG_GDSCR_GDSC_ENF_ACK_STATUS_SHFT                                           0x11
#define HWIO_GCC_USB30_PRIM_CFG_GDSCR_GDSC_POWER_UP_COMPLETE_BMSK                                     0x10000
#define HWIO_GCC_USB30_PRIM_CFG_GDSCR_GDSC_POWER_UP_COMPLETE_SHFT                                        0x10
#define HWIO_GCC_USB30_PRIM_CFG_GDSCR_GDSC_POWER_DOWN_COMPLETE_BMSK                                    0x8000
#define HWIO_GCC_USB30_PRIM_CFG_GDSCR_GDSC_POWER_DOWN_COMPLETE_SHFT                                       0xf
#define HWIO_GCC_USB30_PRIM_CFG_GDSCR_SOFTWARE_CONTROL_OVERRIDE_BMSK                                   0x7800
#define HWIO_GCC_USB30_PRIM_CFG_GDSCR_SOFTWARE_CONTROL_OVERRIDE_SHFT                                      0xb
#define HWIO_GCC_USB30_PRIM_CFG_GDSCR_GDSC_HANDSHAKE_DIS_BMSK                                           0x400
#define HWIO_GCC_USB30_PRIM_CFG_GDSCR_GDSC_HANDSHAKE_DIS_SHFT                                             0xa
#define HWIO_GCC_USB30_PRIM_CFG_GDSCR_GDSC_MEM_PERI_FORCE_IN_SW_BMSK                                    0x200
#define HWIO_GCC_USB30_PRIM_CFG_GDSCR_GDSC_MEM_PERI_FORCE_IN_SW_SHFT                                      0x9
#define HWIO_GCC_USB30_PRIM_CFG_GDSCR_GDSC_MEM_CORE_FORCE_IN_SW_BMSK                                    0x100
#define HWIO_GCC_USB30_PRIM_CFG_GDSCR_GDSC_MEM_CORE_FORCE_IN_SW_SHFT                                      0x8
#define HWIO_GCC_USB30_PRIM_CFG_GDSCR_GDSC_PHASE_RESET_EN_SW_BMSK                                        0x80
#define HWIO_GCC_USB30_PRIM_CFG_GDSCR_GDSC_PHASE_RESET_EN_SW_SHFT                                         0x7
#define HWIO_GCC_USB30_PRIM_CFG_GDSCR_GDSC_PHASE_RESET_DELAY_COUNT_SW_BMSK                               0x60
#define HWIO_GCC_USB30_PRIM_CFG_GDSCR_GDSC_PHASE_RESET_DELAY_COUNT_SW_SHFT                                0x5
#define HWIO_GCC_USB30_PRIM_CFG_GDSCR_GDSC_PSCBC_PWR_DWN_SW_BMSK                                         0x10
#define HWIO_GCC_USB30_PRIM_CFG_GDSCR_GDSC_PSCBC_PWR_DWN_SW_SHFT                                          0x4
#define HWIO_GCC_USB30_PRIM_CFG_GDSCR_UNCLAMP_IO_SOFTWARE_OVERRIDE_BMSK                                   0x8
#define HWIO_GCC_USB30_PRIM_CFG_GDSCR_UNCLAMP_IO_SOFTWARE_OVERRIDE_SHFT                                   0x3
#define HWIO_GCC_USB30_PRIM_CFG_GDSCR_SAVE_RESTORE_SOFTWARE_OVERRIDE_BMSK                                 0x4
#define HWIO_GCC_USB30_PRIM_CFG_GDSCR_SAVE_RESTORE_SOFTWARE_OVERRIDE_SHFT                                 0x2
#define HWIO_GCC_USB30_PRIM_CFG_GDSCR_CLAMP_IO_SOFTWARE_OVERRIDE_BMSK                                     0x2
#define HWIO_GCC_USB30_PRIM_CFG_GDSCR_CLAMP_IO_SOFTWARE_OVERRIDE_SHFT                                     0x1
#define HWIO_GCC_USB30_PRIM_CFG_GDSCR_DISABLE_CLK_SOFTWARE_OVERRIDE_BMSK                                  0x1
#define HWIO_GCC_USB30_PRIM_CFG_GDSCR_DISABLE_CLK_SOFTWARE_OVERRIDE_SHFT                                  0x0

#define HWIO_GCC_USB30_PRIM_CFG2_GDSCR_ADDR                                                        (GCC_CLK_CTL_REG_REG_BASE      + 0x0000f00c)
#define HWIO_GCC_USB30_PRIM_CFG2_GDSCR_PHYS                                                        (GCC_CLK_CTL_REG_REG_BASE_PHYS + 0x0000f00c)
#define HWIO_GCC_USB30_PRIM_CFG2_GDSCR_OFFS                                                        (GCC_CLK_CTL_REG_REG_BASE_OFFS + 0x0000f00c)
#define HWIO_GCC_USB30_PRIM_CFG2_GDSCR_RMSK                                                           0x1ffff
#define HWIO_GCC_USB30_PRIM_CFG2_GDSCR_IN          \
        in_dword(HWIO_GCC_USB30_PRIM_CFG2_GDSCR_ADDR)
#define HWIO_GCC_USB30_PRIM_CFG2_GDSCR_INM(m)      \
        in_dword_masked(HWIO_GCC_USB30_PRIM_CFG2_GDSCR_ADDR, m)
#define HWIO_GCC_USB30_PRIM_CFG2_GDSCR_OUT(v)      \
        out_dword(HWIO_GCC_USB30_PRIM_CFG2_GDSCR_ADDR,v)
#define HWIO_GCC_USB30_PRIM_CFG2_GDSCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GCC_USB30_PRIM_CFG2_GDSCR_ADDR,m,v,HWIO_GCC_USB30_PRIM_CFG2_GDSCR_IN)
#define HWIO_GCC_USB30_PRIM_CFG2_GDSCR_GDSC_CLAMP_MEM_SW_BMSK                                         0x10000
#define HWIO_GCC_USB30_PRIM_CFG2_GDSCR_GDSC_CLAMP_MEM_SW_SHFT                                            0x10
#define HWIO_GCC_USB30_PRIM_CFG2_GDSCR_DLY_MEM_PWR_UP_BMSK                                             0xf000
#define HWIO_GCC_USB30_PRIM_CFG2_GDSCR_DLY_MEM_PWR_UP_SHFT                                                0xc
#define HWIO_GCC_USB30_PRIM_CFG2_GDSCR_DLY_DEASSERT_CLAMP_MEM_BMSK                                      0xf00
#define HWIO_GCC_USB30_PRIM_CFG2_GDSCR_DLY_DEASSERT_CLAMP_MEM_SHFT                                        0x8
#define HWIO_GCC_USB30_PRIM_CFG2_GDSCR_DLY_ASSERT_CLAMP_MEM_BMSK                                         0xf0
#define HWIO_GCC_USB30_PRIM_CFG2_GDSCR_DLY_ASSERT_CLAMP_MEM_SHFT                                          0x4
#define HWIO_GCC_USB30_PRIM_CFG2_GDSCR_MEM_PWR_DWN_TIMEOUT_BMSK                                           0xf
#define HWIO_GCC_USB30_PRIM_CFG2_GDSCR_MEM_PWR_DWN_TIMEOUT_SHFT                                           0x0

#define HWIO_GCC_USB30_PRIM_MASTER_CBCR_ADDR                                                       (GCC_CLK_CTL_REG_REG_BASE      + 0x0000f010)
#define HWIO_GCC_USB30_PRIM_MASTER_CBCR_PHYS                                                       (GCC_CLK_CTL_REG_REG_BASE_PHYS + 0x0000f010)
#define HWIO_GCC_USB30_PRIM_MASTER_CBCR_OFFS                                                       (GCC_CLK_CTL_REG_REG_BASE_OFFS + 0x0000f010)
#define HWIO_GCC_USB30_PRIM_MASTER_CBCR_RMSK                                                       0x80007ff5
#define HWIO_GCC_USB30_PRIM_MASTER_CBCR_IN          \
        in_dword(HWIO_GCC_USB30_PRIM_MASTER_CBCR_ADDR)
#define HWIO_GCC_USB30_PRIM_MASTER_CBCR_INM(m)      \
        in_dword_masked(HWIO_GCC_USB30_PRIM_MASTER_CBCR_ADDR, m)
#define HWIO_GCC_USB30_PRIM_MASTER_CBCR_OUT(v)      \
        out_dword(HWIO_GCC_USB30_PRIM_MASTER_CBCR_ADDR,v)
#define HWIO_GCC_USB30_PRIM_MASTER_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GCC_USB30_PRIM_MASTER_CBCR_ADDR,m,v,HWIO_GCC_USB30_PRIM_MASTER_CBCR_IN)
#define HWIO_GCC_USB30_PRIM_MASTER_CBCR_CLK_OFF_BMSK                                               0x80000000
#define HWIO_GCC_USB30_PRIM_MASTER_CBCR_CLK_OFF_SHFT                                                     0x1f
#define HWIO_GCC_USB30_PRIM_MASTER_CBCR_FORCE_MEM_CORE_ON_BMSK                                         0x4000
#define HWIO_GCC_USB30_PRIM_MASTER_CBCR_FORCE_MEM_CORE_ON_SHFT                                            0xe
#define HWIO_GCC_USB30_PRIM_MASTER_CBCR_FORCE_MEM_CORE_ON_FORCE_DISABLE_FVAL                              0x0
#define HWIO_GCC_USB30_PRIM_MASTER_CBCR_FORCE_MEM_CORE_ON_FORCE_ENABLE_FVAL                               0x1
#define HWIO_GCC_USB30_PRIM_MASTER_CBCR_FORCE_MEM_PERIPH_ON_BMSK                                       0x2000
#define HWIO_GCC_USB30_PRIM_MASTER_CBCR_FORCE_MEM_PERIPH_ON_SHFT                                          0xd
#define HWIO_GCC_USB30_PRIM_MASTER_CBCR_FORCE_MEM_PERIPH_ON_FORCE_DISABLE_FVAL                            0x0
#define HWIO_GCC_USB30_PRIM_MASTER_CBCR_FORCE_MEM_PERIPH_ON_FORCE_ENABLE_FVAL                             0x1
#define HWIO_GCC_USB30_PRIM_MASTER_CBCR_FORCE_MEM_PERIPH_OFF_BMSK                                      0x1000
#define HWIO_GCC_USB30_PRIM_MASTER_CBCR_FORCE_MEM_PERIPH_OFF_SHFT                                         0xc
#define HWIO_GCC_USB30_PRIM_MASTER_CBCR_FORCE_MEM_PERIPH_OFF_FORCE_DISABLE_FVAL                           0x0
#define HWIO_GCC_USB30_PRIM_MASTER_CBCR_FORCE_MEM_PERIPH_OFF_FORCE_ENABLE_FVAL                            0x1
#define HWIO_GCC_USB30_PRIM_MASTER_CBCR_WAKEUP_BMSK                                                     0xf00
#define HWIO_GCC_USB30_PRIM_MASTER_CBCR_WAKEUP_SHFT                                                       0x8
#define HWIO_GCC_USB30_PRIM_MASTER_CBCR_WAKEUP_CLOCK0_FVAL                                                0x0
#define HWIO_GCC_USB30_PRIM_MASTER_CBCR_WAKEUP_CLOCK1_FVAL                                                0x1
#define HWIO_GCC_USB30_PRIM_MASTER_CBCR_WAKEUP_CLOCK2_FVAL                                                0x2
#define HWIO_GCC_USB30_PRIM_MASTER_CBCR_WAKEUP_CLOCK3_FVAL                                                0x3
#define HWIO_GCC_USB30_PRIM_MASTER_CBCR_WAKEUP_CLOCK4_FVAL                                                0x4
#define HWIO_GCC_USB30_PRIM_MASTER_CBCR_WAKEUP_CLOCK5_FVAL                                                0x5
#define HWIO_GCC_USB30_PRIM_MASTER_CBCR_WAKEUP_CLOCK6_FVAL                                                0x6
#define HWIO_GCC_USB30_PRIM_MASTER_CBCR_WAKEUP_CLOCK7_FVAL                                                0x7
#define HWIO_GCC_USB30_PRIM_MASTER_CBCR_WAKEUP_CLOCK8_FVAL                                                0x8
#define HWIO_GCC_USB30_PRIM_MASTER_CBCR_WAKEUP_CLOCK9_FVAL                                                0x9
#define HWIO_GCC_USB30_PRIM_MASTER_CBCR_WAKEUP_CLOCK10_FVAL                                               0xa
#define HWIO_GCC_USB30_PRIM_MASTER_CBCR_WAKEUP_CLOCK11_FVAL                                               0xb
#define HWIO_GCC_USB30_PRIM_MASTER_CBCR_WAKEUP_CLOCK12_FVAL                                               0xc
#define HWIO_GCC_USB30_PRIM_MASTER_CBCR_WAKEUP_CLOCK13_FVAL                                               0xd
#define HWIO_GCC_USB30_PRIM_MASTER_CBCR_WAKEUP_CLOCK14_FVAL                                               0xe
#define HWIO_GCC_USB30_PRIM_MASTER_CBCR_WAKEUP_CLOCK15_FVAL                                               0xf
#define HWIO_GCC_USB30_PRIM_MASTER_CBCR_SLEEP_BMSK                                                       0xf0
#define HWIO_GCC_USB30_PRIM_MASTER_CBCR_SLEEP_SHFT                                                        0x4
#define HWIO_GCC_USB30_PRIM_MASTER_CBCR_SLEEP_CLOCK0_FVAL                                                 0x0
#define HWIO_GCC_USB30_PRIM_MASTER_CBCR_SLEEP_CLOCK1_FVAL                                                 0x1
#define HWIO_GCC_USB30_PRIM_MASTER_CBCR_SLEEP_CLOCK2_FVAL                                                 0x2
#define HWIO_GCC_USB30_PRIM_MASTER_CBCR_SLEEP_CLOCK3_FVAL                                                 0x3
#define HWIO_GCC_USB30_PRIM_MASTER_CBCR_SLEEP_CLOCK4_FVAL                                                 0x4
#define HWIO_GCC_USB30_PRIM_MASTER_CBCR_SLEEP_CLOCK5_FVAL                                                 0x5
#define HWIO_GCC_USB30_PRIM_MASTER_CBCR_SLEEP_CLOCK6_FVAL                                                 0x6
#define HWIO_GCC_USB30_PRIM_MASTER_CBCR_SLEEP_CLOCK7_FVAL                                                 0x7
#define HWIO_GCC_USB30_PRIM_MASTER_CBCR_SLEEP_CLOCK8_FVAL                                                 0x8
#define HWIO_GCC_USB30_PRIM_MASTER_CBCR_SLEEP_CLOCK9_FVAL                                                 0x9
#define HWIO_GCC_USB30_PRIM_MASTER_CBCR_SLEEP_CLOCK10_FVAL                                                0xa
#define HWIO_GCC_USB30_PRIM_MASTER_CBCR_SLEEP_CLOCK11_FVAL                                                0xb
#define HWIO_GCC_USB30_PRIM_MASTER_CBCR_SLEEP_CLOCK12_FVAL                                                0xc
#define HWIO_GCC_USB30_PRIM_MASTER_CBCR_SLEEP_CLOCK13_FVAL                                                0xd
#define HWIO_GCC_USB30_PRIM_MASTER_CBCR_SLEEP_CLOCK14_FVAL                                                0xe
#define HWIO_GCC_USB30_PRIM_MASTER_CBCR_SLEEP_CLOCK15_FVAL                                                0xf
#define HWIO_GCC_USB30_PRIM_MASTER_CBCR_CLK_ARES_BMSK                                                     0x4
#define HWIO_GCC_USB30_PRIM_MASTER_CBCR_CLK_ARES_SHFT                                                     0x2
#define HWIO_GCC_USB30_PRIM_MASTER_CBCR_CLK_ARES_NO_RESET_FVAL                                            0x0
#define HWIO_GCC_USB30_PRIM_MASTER_CBCR_CLK_ARES_RESET_FVAL                                               0x1
#define HWIO_GCC_USB30_PRIM_MASTER_CBCR_CLK_ENABLE_BMSK                                                   0x1
#define HWIO_GCC_USB30_PRIM_MASTER_CBCR_CLK_ENABLE_SHFT                                                   0x0
#define HWIO_GCC_USB30_PRIM_MASTER_CBCR_CLK_ENABLE_DISABLE_FVAL                                           0x0
#define HWIO_GCC_USB30_PRIM_MASTER_CBCR_CLK_ENABLE_ENABLE_FVAL                                            0x1

#define HWIO_GCC_USB30_PRIM_SLEEP_CBCR_ADDR                                                        (GCC_CLK_CTL_REG_REG_BASE      + 0x0000f014)
#define HWIO_GCC_USB30_PRIM_SLEEP_CBCR_PHYS                                                        (GCC_CLK_CTL_REG_REG_BASE_PHYS + 0x0000f014)
#define HWIO_GCC_USB30_PRIM_SLEEP_CBCR_OFFS                                                        (GCC_CLK_CTL_REG_REG_BASE_OFFS + 0x0000f014)
#define HWIO_GCC_USB30_PRIM_SLEEP_CBCR_RMSK                                                        0x80000005
#define HWIO_GCC_USB30_PRIM_SLEEP_CBCR_IN          \
        in_dword(HWIO_GCC_USB30_PRIM_SLEEP_CBCR_ADDR)
#define HWIO_GCC_USB30_PRIM_SLEEP_CBCR_INM(m)      \
        in_dword_masked(HWIO_GCC_USB30_PRIM_SLEEP_CBCR_ADDR, m)
#define HWIO_GCC_USB30_PRIM_SLEEP_CBCR_OUT(v)      \
        out_dword(HWIO_GCC_USB30_PRIM_SLEEP_CBCR_ADDR,v)
#define HWIO_GCC_USB30_PRIM_SLEEP_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GCC_USB30_PRIM_SLEEP_CBCR_ADDR,m,v,HWIO_GCC_USB30_PRIM_SLEEP_CBCR_IN)
#define HWIO_GCC_USB30_PRIM_SLEEP_CBCR_CLK_OFF_BMSK                                                0x80000000
#define HWIO_GCC_USB30_PRIM_SLEEP_CBCR_CLK_OFF_SHFT                                                      0x1f
#define HWIO_GCC_USB30_PRIM_SLEEP_CBCR_CLK_ARES_BMSK                                                      0x4
#define HWIO_GCC_USB30_PRIM_SLEEP_CBCR_CLK_ARES_SHFT                                                      0x2
#define HWIO_GCC_USB30_PRIM_SLEEP_CBCR_CLK_ARES_NO_RESET_FVAL                                             0x0
#define HWIO_GCC_USB30_PRIM_SLEEP_CBCR_CLK_ARES_RESET_FVAL                                                0x1
#define HWIO_GCC_USB30_PRIM_SLEEP_CBCR_CLK_ENABLE_BMSK                                                    0x1
#define HWIO_GCC_USB30_PRIM_SLEEP_CBCR_CLK_ENABLE_SHFT                                                    0x0
#define HWIO_GCC_USB30_PRIM_SLEEP_CBCR_CLK_ENABLE_DISABLE_FVAL                                            0x0
#define HWIO_GCC_USB30_PRIM_SLEEP_CBCR_CLK_ENABLE_ENABLE_FVAL                                             0x1

#define HWIO_GCC_USB30_PRIM_MOCK_UTMI_CBCR_ADDR                                                    (GCC_CLK_CTL_REG_REG_BASE      + 0x0000f018)
#define HWIO_GCC_USB30_PRIM_MOCK_UTMI_CBCR_PHYS                                                    (GCC_CLK_CTL_REG_REG_BASE_PHYS + 0x0000f018)
#define HWIO_GCC_USB30_PRIM_MOCK_UTMI_CBCR_OFFS                                                    (GCC_CLK_CTL_REG_REG_BASE_OFFS + 0x0000f018)
#define HWIO_GCC_USB30_PRIM_MOCK_UTMI_CBCR_RMSK                                                    0x80000005
#define HWIO_GCC_USB30_PRIM_MOCK_UTMI_CBCR_IN          \
        in_dword(HWIO_GCC_USB30_PRIM_MOCK_UTMI_CBCR_ADDR)
#define HWIO_GCC_USB30_PRIM_MOCK_UTMI_CBCR_INM(m)      \
        in_dword_masked(HWIO_GCC_USB30_PRIM_MOCK_UTMI_CBCR_ADDR, m)
#define HWIO_GCC_USB30_PRIM_MOCK_UTMI_CBCR_OUT(v)      \
        out_dword(HWIO_GCC_USB30_PRIM_MOCK_UTMI_CBCR_ADDR,v)
#define HWIO_GCC_USB30_PRIM_MOCK_UTMI_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GCC_USB30_PRIM_MOCK_UTMI_CBCR_ADDR,m,v,HWIO_GCC_USB30_PRIM_MOCK_UTMI_CBCR_IN)
#define HWIO_GCC_USB30_PRIM_MOCK_UTMI_CBCR_CLK_OFF_BMSK                                            0x80000000
#define HWIO_GCC_USB30_PRIM_MOCK_UTMI_CBCR_CLK_OFF_SHFT                                                  0x1f
#define HWIO_GCC_USB30_PRIM_MOCK_UTMI_CBCR_CLK_ARES_BMSK                                                  0x4
#define HWIO_GCC_USB30_PRIM_MOCK_UTMI_CBCR_CLK_ARES_SHFT                                                  0x2
#define HWIO_GCC_USB30_PRIM_MOCK_UTMI_CBCR_CLK_ARES_NO_RESET_FVAL                                         0x0
#define HWIO_GCC_USB30_PRIM_MOCK_UTMI_CBCR_CLK_ARES_RESET_FVAL                                            0x1
#define HWIO_GCC_USB30_PRIM_MOCK_UTMI_CBCR_CLK_ENABLE_BMSK                                                0x1
#define HWIO_GCC_USB30_PRIM_MOCK_UTMI_CBCR_CLK_ENABLE_SHFT                                                0x0
#define HWIO_GCC_USB30_PRIM_MOCK_UTMI_CBCR_CLK_ENABLE_DISABLE_FVAL                                        0x0
#define HWIO_GCC_USB30_PRIM_MOCK_UTMI_CBCR_CLK_ENABLE_ENABLE_FVAL                                         0x1

#define HWIO_GCC_USB30_PRIM_MASTER_CMD_RCGR_ADDR                                                   (GCC_CLK_CTL_REG_REG_BASE      + 0x0000f01c)
#define HWIO_GCC_USB30_PRIM_MASTER_CMD_RCGR_PHYS                                                   (GCC_CLK_CTL_REG_REG_BASE_PHYS + 0x0000f01c)
#define HWIO_GCC_USB30_PRIM_MASTER_CMD_RCGR_OFFS                                                   (GCC_CLK_CTL_REG_REG_BASE_OFFS + 0x0000f01c)
#define HWIO_GCC_USB30_PRIM_MASTER_CMD_RCGR_RMSK                                                   0x800000f3
#define HWIO_GCC_USB30_PRIM_MASTER_CMD_RCGR_IN          \
        in_dword(HWIO_GCC_USB30_PRIM_MASTER_CMD_RCGR_ADDR)
#define HWIO_GCC_USB30_PRIM_MASTER_CMD_RCGR_INM(m)      \
        in_dword_masked(HWIO_GCC_USB30_PRIM_MASTER_CMD_RCGR_ADDR, m)
#define HWIO_GCC_USB30_PRIM_MASTER_CMD_RCGR_OUT(v)      \
        out_dword(HWIO_GCC_USB30_PRIM_MASTER_CMD_RCGR_ADDR,v)
#define HWIO_GCC_USB30_PRIM_MASTER_CMD_RCGR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GCC_USB30_PRIM_MASTER_CMD_RCGR_ADDR,m,v,HWIO_GCC_USB30_PRIM_MASTER_CMD_RCGR_IN)
#define HWIO_GCC_USB30_PRIM_MASTER_CMD_RCGR_ROOT_OFF_BMSK                                          0x80000000
#define HWIO_GCC_USB30_PRIM_MASTER_CMD_RCGR_ROOT_OFF_SHFT                                                0x1f
#define HWIO_GCC_USB30_PRIM_MASTER_CMD_RCGR_DIRTY_D_BMSK                                                 0x80
#define HWIO_GCC_USB30_PRIM_MASTER_CMD_RCGR_DIRTY_D_SHFT                                                  0x7
#define HWIO_GCC_USB30_PRIM_MASTER_CMD_RCGR_DIRTY_N_BMSK                                                 0x40
#define HWIO_GCC_USB30_PRIM_MASTER_CMD_RCGR_DIRTY_N_SHFT                                                  0x6
#define HWIO_GCC_USB30_PRIM_MASTER_CMD_RCGR_DIRTY_M_BMSK                                                 0x20
#define HWIO_GCC_USB30_PRIM_MASTER_CMD_RCGR_DIRTY_M_SHFT                                                  0x5
#define HWIO_GCC_USB30_PRIM_MASTER_CMD_RCGR_DIRTY_CFG_RCGR_BMSK                                          0x10
#define HWIO_GCC_USB30_PRIM_MASTER_CMD_RCGR_DIRTY_CFG_RCGR_SHFT                                           0x4
#define HWIO_GCC_USB30_PRIM_MASTER_CMD_RCGR_ROOT_EN_BMSK                                                  0x2
#define HWIO_GCC_USB30_PRIM_MASTER_CMD_RCGR_ROOT_EN_SHFT                                                  0x1
#define HWIO_GCC_USB30_PRIM_MASTER_CMD_RCGR_ROOT_EN_DISABLE_FVAL                                          0x0
#define HWIO_GCC_USB30_PRIM_MASTER_CMD_RCGR_ROOT_EN_ENABLE_FVAL                                           0x1
#define HWIO_GCC_USB30_PRIM_MASTER_CMD_RCGR_UPDATE_BMSK                                                   0x1
#define HWIO_GCC_USB30_PRIM_MASTER_CMD_RCGR_UPDATE_SHFT                                                   0x0
#define HWIO_GCC_USB30_PRIM_MASTER_CMD_RCGR_UPDATE_DISABLE_FVAL                                           0x0
#define HWIO_GCC_USB30_PRIM_MASTER_CMD_RCGR_UPDATE_ENABLE_FVAL                                            0x1

#define HWIO_GCC_USB30_PRIM_MASTER_CFG_RCGR_ADDR                                                   (GCC_CLK_CTL_REG_REG_BASE      + 0x0000f020)
#define HWIO_GCC_USB30_PRIM_MASTER_CFG_RCGR_PHYS                                                   (GCC_CLK_CTL_REG_REG_BASE_PHYS + 0x0000f020)
#define HWIO_GCC_USB30_PRIM_MASTER_CFG_RCGR_OFFS                                                   (GCC_CLK_CTL_REG_REG_BASE_OFFS + 0x0000f020)
#define HWIO_GCC_USB30_PRIM_MASTER_CFG_RCGR_RMSK                                                     0x10371f
#define HWIO_GCC_USB30_PRIM_MASTER_CFG_RCGR_IN          \
        in_dword(HWIO_GCC_USB30_PRIM_MASTER_CFG_RCGR_ADDR)
#define HWIO_GCC_USB30_PRIM_MASTER_CFG_RCGR_INM(m)      \
        in_dword_masked(HWIO_GCC_USB30_PRIM_MASTER_CFG_RCGR_ADDR, m)
#define HWIO_GCC_USB30_PRIM_MASTER_CFG_RCGR_OUT(v)      \
        out_dword(HWIO_GCC_USB30_PRIM_MASTER_CFG_RCGR_ADDR,v)
#define HWIO_GCC_USB30_PRIM_MASTER_CFG_RCGR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GCC_USB30_PRIM_MASTER_CFG_RCGR_ADDR,m,v,HWIO_GCC_USB30_PRIM_MASTER_CFG_RCGR_IN)
#define HWIO_GCC_USB30_PRIM_MASTER_CFG_RCGR_HW_CLK_CONTROL_BMSK                                      0x100000
#define HWIO_GCC_USB30_PRIM_MASTER_CFG_RCGR_HW_CLK_CONTROL_SHFT                                          0x14
#define HWIO_GCC_USB30_PRIM_MASTER_CFG_RCGR_HW_CLK_CONTROL_DISABLE_FVAL                                   0x0
#define HWIO_GCC_USB30_PRIM_MASTER_CFG_RCGR_HW_CLK_CONTROL_ENABLE_FVAL                                    0x1
#define HWIO_GCC_USB30_PRIM_MASTER_CFG_RCGR_MODE_BMSK                                                  0x3000
#define HWIO_GCC_USB30_PRIM_MASTER_CFG_RCGR_MODE_SHFT                                                     0xc
#define HWIO_GCC_USB30_PRIM_MASTER_CFG_RCGR_MODE_BYPASS_FVAL                                              0x0
#define HWIO_GCC_USB30_PRIM_MASTER_CFG_RCGR_MODE_SWALLOW_FVAL                                             0x1
#define HWIO_GCC_USB30_PRIM_MASTER_CFG_RCGR_MODE_DUAL_EDGE_FVAL                                           0x2
#define HWIO_GCC_USB30_PRIM_MASTER_CFG_RCGR_MODE_SINGLE_EDGE_FVAL                                         0x3
#define HWIO_GCC_USB30_PRIM_MASTER_CFG_RCGR_SRC_SEL_BMSK                                                0x700
#define HWIO_GCC_USB30_PRIM_MASTER_CFG_RCGR_SRC_SEL_SHFT                                                  0x8
#define HWIO_GCC_USB30_PRIM_MASTER_CFG_RCGR_SRC_SEL_SRC0_FVAL                                             0x0
#define HWIO_GCC_USB30_PRIM_MASTER_CFG_RCGR_SRC_SEL_SRC1_FVAL                                             0x1
#define HWIO_GCC_USB30_PRIM_MASTER_CFG_RCGR_SRC_SEL_SRC2_FVAL                                             0x2
#define HWIO_GCC_USB30_PRIM_MASTER_CFG_RCGR_SRC_SEL_SRC3_FVAL                                             0x3
#define HWIO_GCC_USB30_PRIM_MASTER_CFG_RCGR_SRC_SEL_SRC4_FVAL                                             0x4
#define HWIO_GCC_USB30_PRIM_MASTER_CFG_RCGR_SRC_SEL_SRC5_FVAL                                             0x5
#define HWIO_GCC_USB30_PRIM_MASTER_CFG_RCGR_SRC_SEL_SRC6_FVAL                                             0x6
#define HWIO_GCC_USB30_PRIM_MASTER_CFG_RCGR_SRC_SEL_SRC7_FVAL                                             0x7
#define HWIO_GCC_USB30_PRIM_MASTER_CFG_RCGR_SRC_DIV_BMSK                                                 0x1f
#define HWIO_GCC_USB30_PRIM_MASTER_CFG_RCGR_SRC_DIV_SHFT                                                  0x0
#define HWIO_GCC_USB30_PRIM_MASTER_CFG_RCGR_SRC_DIV_BYPASS_FVAL                                           0x0
#define HWIO_GCC_USB30_PRIM_MASTER_CFG_RCGR_SRC_DIV_DIV1_FVAL                                             0x1
#define HWIO_GCC_USB30_PRIM_MASTER_CFG_RCGR_SRC_DIV_DIV1_5_FVAL                                           0x2
#define HWIO_GCC_USB30_PRIM_MASTER_CFG_RCGR_SRC_DIV_DIV2_FVAL                                             0x3
#define HWIO_GCC_USB30_PRIM_MASTER_CFG_RCGR_SRC_DIV_DIV2_5_FVAL                                           0x4
#define HWIO_GCC_USB30_PRIM_MASTER_CFG_RCGR_SRC_DIV_DIV3_FVAL                                             0x5
#define HWIO_GCC_USB30_PRIM_MASTER_CFG_RCGR_SRC_DIV_DIV3_5_FVAL                                           0x6
#define HWIO_GCC_USB30_PRIM_MASTER_CFG_RCGR_SRC_DIV_DIV4_FVAL                                             0x7
#define HWIO_GCC_USB30_PRIM_MASTER_CFG_RCGR_SRC_DIV_DIV4_5_FVAL                                           0x8
#define HWIO_GCC_USB30_PRIM_MASTER_CFG_RCGR_SRC_DIV_DIV5_FVAL                                             0x9
#define HWIO_GCC_USB30_PRIM_MASTER_CFG_RCGR_SRC_DIV_DIV5_5_FVAL                                           0xa
#define HWIO_GCC_USB30_PRIM_MASTER_CFG_RCGR_SRC_DIV_DIV6_FVAL                                             0xb
#define HWIO_GCC_USB30_PRIM_MASTER_CFG_RCGR_SRC_DIV_DIV6_5_FVAL                                           0xc
#define HWIO_GCC_USB30_PRIM_MASTER_CFG_RCGR_SRC_DIV_DIV7_FVAL                                             0xd
#define HWIO_GCC_USB30_PRIM_MASTER_CFG_RCGR_SRC_DIV_DIV7_5_FVAL                                           0xe
#define HWIO_GCC_USB30_PRIM_MASTER_CFG_RCGR_SRC_DIV_DIV8_FVAL                                             0xf
#define HWIO_GCC_USB30_PRIM_MASTER_CFG_RCGR_SRC_DIV_DIV8_5_FVAL                                          0x10
#define HWIO_GCC_USB30_PRIM_MASTER_CFG_RCGR_SRC_DIV_DIV9_FVAL                                            0x11
#define HWIO_GCC_USB30_PRIM_MASTER_CFG_RCGR_SRC_DIV_DIV9_5_FVAL                                          0x12
#define HWIO_GCC_USB30_PRIM_MASTER_CFG_RCGR_SRC_DIV_DIV10_FVAL                                           0x13
#define HWIO_GCC_USB30_PRIM_MASTER_CFG_RCGR_SRC_DIV_DIV10_5_FVAL                                         0x14
#define HWIO_GCC_USB30_PRIM_MASTER_CFG_RCGR_SRC_DIV_DIV11_FVAL                                           0x15
#define HWIO_GCC_USB30_PRIM_MASTER_CFG_RCGR_SRC_DIV_DIV11_5_FVAL                                         0x16
#define HWIO_GCC_USB30_PRIM_MASTER_CFG_RCGR_SRC_DIV_DIV12_FVAL                                           0x17
#define HWIO_GCC_USB30_PRIM_MASTER_CFG_RCGR_SRC_DIV_DIV12_5_FVAL                                         0x18
#define HWIO_GCC_USB30_PRIM_MASTER_CFG_RCGR_SRC_DIV_DIV13_FVAL                                           0x19
#define HWIO_GCC_USB30_PRIM_MASTER_CFG_RCGR_SRC_DIV_DIV13_5_FVAL                                         0x1a
#define HWIO_GCC_USB30_PRIM_MASTER_CFG_RCGR_SRC_DIV_DIV14_FVAL                                           0x1b
#define HWIO_GCC_USB30_PRIM_MASTER_CFG_RCGR_SRC_DIV_DIV14_5_FVAL                                         0x1c
#define HWIO_GCC_USB30_PRIM_MASTER_CFG_RCGR_SRC_DIV_DIV15_FVAL                                           0x1d
#define HWIO_GCC_USB30_PRIM_MASTER_CFG_RCGR_SRC_DIV_DIV15_5_FVAL                                         0x1e
#define HWIO_GCC_USB30_PRIM_MASTER_CFG_RCGR_SRC_DIV_DIV16_FVAL                                           0x1f

#define HWIO_GCC_USB30_PRIM_MASTER_M_ADDR                                                          (GCC_CLK_CTL_REG_REG_BASE      + 0x0000f024)
#define HWIO_GCC_USB30_PRIM_MASTER_M_PHYS                                                          (GCC_CLK_CTL_REG_REG_BASE_PHYS + 0x0000f024)
#define HWIO_GCC_USB30_PRIM_MASTER_M_OFFS                                                          (GCC_CLK_CTL_REG_REG_BASE_OFFS + 0x0000f024)
#define HWIO_GCC_USB30_PRIM_MASTER_M_RMSK                                                                0xff
#define HWIO_GCC_USB30_PRIM_MASTER_M_IN          \
        in_dword(HWIO_GCC_USB30_PRIM_MASTER_M_ADDR)
#define HWIO_GCC_USB30_PRIM_MASTER_M_INM(m)      \
        in_dword_masked(HWIO_GCC_USB30_PRIM_MASTER_M_ADDR, m)
#define HWIO_GCC_USB30_PRIM_MASTER_M_OUT(v)      \
        out_dword(HWIO_GCC_USB30_PRIM_MASTER_M_ADDR,v)
#define HWIO_GCC_USB30_PRIM_MASTER_M_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GCC_USB30_PRIM_MASTER_M_ADDR,m,v,HWIO_GCC_USB30_PRIM_MASTER_M_IN)
#define HWIO_GCC_USB30_PRIM_MASTER_M_M_BMSK                                                              0xff
#define HWIO_GCC_USB30_PRIM_MASTER_M_M_SHFT                                                               0x0

#define HWIO_GCC_USB30_PRIM_MASTER_N_ADDR                                                          (GCC_CLK_CTL_REG_REG_BASE      + 0x0000f028)
#define HWIO_GCC_USB30_PRIM_MASTER_N_PHYS                                                          (GCC_CLK_CTL_REG_REG_BASE_PHYS + 0x0000f028)
#define HWIO_GCC_USB30_PRIM_MASTER_N_OFFS                                                          (GCC_CLK_CTL_REG_REG_BASE_OFFS + 0x0000f028)
#define HWIO_GCC_USB30_PRIM_MASTER_N_RMSK                                                                0xff
#define HWIO_GCC_USB30_PRIM_MASTER_N_IN          \
        in_dword(HWIO_GCC_USB30_PRIM_MASTER_N_ADDR)
#define HWIO_GCC_USB30_PRIM_MASTER_N_INM(m)      \
        in_dword_masked(HWIO_GCC_USB30_PRIM_MASTER_N_ADDR, m)
#define HWIO_GCC_USB30_PRIM_MASTER_N_OUT(v)      \
        out_dword(HWIO_GCC_USB30_PRIM_MASTER_N_ADDR,v)
#define HWIO_GCC_USB30_PRIM_MASTER_N_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GCC_USB30_PRIM_MASTER_N_ADDR,m,v,HWIO_GCC_USB30_PRIM_MASTER_N_IN)
#define HWIO_GCC_USB30_PRIM_MASTER_N_NOT_N_MINUS_M_BMSK                                                  0xff
#define HWIO_GCC_USB30_PRIM_MASTER_N_NOT_N_MINUS_M_SHFT                                                   0x0

#define HWIO_GCC_USB30_PRIM_MASTER_D_ADDR                                                          (GCC_CLK_CTL_REG_REG_BASE      + 0x0000f02c)
#define HWIO_GCC_USB30_PRIM_MASTER_D_PHYS                                                          (GCC_CLK_CTL_REG_REG_BASE_PHYS + 0x0000f02c)
#define HWIO_GCC_USB30_PRIM_MASTER_D_OFFS                                                          (GCC_CLK_CTL_REG_REG_BASE_OFFS + 0x0000f02c)
#define HWIO_GCC_USB30_PRIM_MASTER_D_RMSK                                                                0xff
#define HWIO_GCC_USB30_PRIM_MASTER_D_IN          \
        in_dword(HWIO_GCC_USB30_PRIM_MASTER_D_ADDR)
#define HWIO_GCC_USB30_PRIM_MASTER_D_INM(m)      \
        in_dword_masked(HWIO_GCC_USB30_PRIM_MASTER_D_ADDR, m)
#define HWIO_GCC_USB30_PRIM_MASTER_D_OUT(v)      \
        out_dword(HWIO_GCC_USB30_PRIM_MASTER_D_ADDR,v)
#define HWIO_GCC_USB30_PRIM_MASTER_D_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GCC_USB30_PRIM_MASTER_D_ADDR,m,v,HWIO_GCC_USB30_PRIM_MASTER_D_IN)
#define HWIO_GCC_USB30_PRIM_MASTER_D_NOT_2D_BMSK                                                         0xff
#define HWIO_GCC_USB30_PRIM_MASTER_D_NOT_2D_SHFT                                                          0x0

#define HWIO_GCC_USB30_PRIM_MOCK_UTMI_CMD_RCGR_ADDR                                                (GCC_CLK_CTL_REG_REG_BASE      + 0x0000f034)
#define HWIO_GCC_USB30_PRIM_MOCK_UTMI_CMD_RCGR_PHYS                                                (GCC_CLK_CTL_REG_REG_BASE_PHYS + 0x0000f034)
#define HWIO_GCC_USB30_PRIM_MOCK_UTMI_CMD_RCGR_OFFS                                                (GCC_CLK_CTL_REG_REG_BASE_OFFS + 0x0000f034)
#define HWIO_GCC_USB30_PRIM_MOCK_UTMI_CMD_RCGR_RMSK                                                0x80000013
#define HWIO_GCC_USB30_PRIM_MOCK_UTMI_CMD_RCGR_IN          \
        in_dword(HWIO_GCC_USB30_PRIM_MOCK_UTMI_CMD_RCGR_ADDR)
#define HWIO_GCC_USB30_PRIM_MOCK_UTMI_CMD_RCGR_INM(m)      \
        in_dword_masked(HWIO_GCC_USB30_PRIM_MOCK_UTMI_CMD_RCGR_ADDR, m)
#define HWIO_GCC_USB30_PRIM_MOCK_UTMI_CMD_RCGR_OUT(v)      \
        out_dword(HWIO_GCC_USB30_PRIM_MOCK_UTMI_CMD_RCGR_ADDR,v)
#define HWIO_GCC_USB30_PRIM_MOCK_UTMI_CMD_RCGR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GCC_USB30_PRIM_MOCK_UTMI_CMD_RCGR_ADDR,m,v,HWIO_GCC_USB30_PRIM_MOCK_UTMI_CMD_RCGR_IN)
#define HWIO_GCC_USB30_PRIM_MOCK_UTMI_CMD_RCGR_ROOT_OFF_BMSK                                       0x80000000
#define HWIO_GCC_USB30_PRIM_MOCK_UTMI_CMD_RCGR_ROOT_OFF_SHFT                                             0x1f
#define HWIO_GCC_USB30_PRIM_MOCK_UTMI_CMD_RCGR_DIRTY_CFG_RCGR_BMSK                                       0x10
#define HWIO_GCC_USB30_PRIM_MOCK_UTMI_CMD_RCGR_DIRTY_CFG_RCGR_SHFT                                        0x4
#define HWIO_GCC_USB30_PRIM_MOCK_UTMI_CMD_RCGR_ROOT_EN_BMSK                                               0x2
#define HWIO_GCC_USB30_PRIM_MOCK_UTMI_CMD_RCGR_ROOT_EN_SHFT                                               0x1
#define HWIO_GCC_USB30_PRIM_MOCK_UTMI_CMD_RCGR_ROOT_EN_DISABLE_FVAL                                       0x0
#define HWIO_GCC_USB30_PRIM_MOCK_UTMI_CMD_RCGR_ROOT_EN_ENABLE_FVAL                                        0x1
#define HWIO_GCC_USB30_PRIM_MOCK_UTMI_CMD_RCGR_UPDATE_BMSK                                                0x1
#define HWIO_GCC_USB30_PRIM_MOCK_UTMI_CMD_RCGR_UPDATE_SHFT                                                0x0
#define HWIO_GCC_USB30_PRIM_MOCK_UTMI_CMD_RCGR_UPDATE_DISABLE_FVAL                                        0x0
#define HWIO_GCC_USB30_PRIM_MOCK_UTMI_CMD_RCGR_UPDATE_ENABLE_FVAL                                         0x1

#define HWIO_GCC_USB30_PRIM_MOCK_UTMI_CFG_RCGR_ADDR                                                (GCC_CLK_CTL_REG_REG_BASE      + 0x0000f038)
#define HWIO_GCC_USB30_PRIM_MOCK_UTMI_CFG_RCGR_PHYS                                                (GCC_CLK_CTL_REG_REG_BASE_PHYS + 0x0000f038)
#define HWIO_GCC_USB30_PRIM_MOCK_UTMI_CFG_RCGR_OFFS                                                (GCC_CLK_CTL_REG_REG_BASE_OFFS + 0x0000f038)
#define HWIO_GCC_USB30_PRIM_MOCK_UTMI_CFG_RCGR_RMSK                                                  0x11071f
#define HWIO_GCC_USB30_PRIM_MOCK_UTMI_CFG_RCGR_IN          \
        in_dword(HWIO_GCC_USB30_PRIM_MOCK_UTMI_CFG_RCGR_ADDR)
#define HWIO_GCC_USB30_PRIM_MOCK_UTMI_CFG_RCGR_INM(m)      \
        in_dword_masked(HWIO_GCC_USB30_PRIM_MOCK_UTMI_CFG_RCGR_ADDR, m)
#define HWIO_GCC_USB30_PRIM_MOCK_UTMI_CFG_RCGR_OUT(v)      \
        out_dword(HWIO_GCC_USB30_PRIM_MOCK_UTMI_CFG_RCGR_ADDR,v)
#define HWIO_GCC_USB30_PRIM_MOCK_UTMI_CFG_RCGR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GCC_USB30_PRIM_MOCK_UTMI_CFG_RCGR_ADDR,m,v,HWIO_GCC_USB30_PRIM_MOCK_UTMI_CFG_RCGR_IN)
#define HWIO_GCC_USB30_PRIM_MOCK_UTMI_CFG_RCGR_HW_CLK_CONTROL_BMSK                                   0x100000
#define HWIO_GCC_USB30_PRIM_MOCK_UTMI_CFG_RCGR_HW_CLK_CONTROL_SHFT                                       0x14
#define HWIO_GCC_USB30_PRIM_MOCK_UTMI_CFG_RCGR_HW_CLK_CONTROL_DISABLE_FVAL                                0x0
#define HWIO_GCC_USB30_PRIM_MOCK_UTMI_CFG_RCGR_HW_CLK_CONTROL_ENABLE_FVAL                                 0x1
#define HWIO_GCC_USB30_PRIM_MOCK_UTMI_CFG_RCGR_RCGLITE_DISABLE_BMSK                                   0x10000
#define HWIO_GCC_USB30_PRIM_MOCK_UTMI_CFG_RCGR_RCGLITE_DISABLE_SHFT                                      0x10
#define HWIO_GCC_USB30_PRIM_MOCK_UTMI_CFG_RCGR_RCGLITE_DISABLE_RCGLITE_ENABLED_FVAL                       0x0
#define HWIO_GCC_USB30_PRIM_MOCK_UTMI_CFG_RCGR_RCGLITE_DISABLE_RCGLITE_DISABLED_FVAL                      0x1
#define HWIO_GCC_USB30_PRIM_MOCK_UTMI_CFG_RCGR_SRC_SEL_BMSK                                             0x700
#define HWIO_GCC_USB30_PRIM_MOCK_UTMI_CFG_RCGR_SRC_SEL_SHFT                                               0x8
#define HWIO_GCC_USB30_PRIM_MOCK_UTMI_CFG_RCGR_SRC_SEL_SRC0_FVAL                                          0x0
#define HWIO_GCC_USB30_PRIM_MOCK_UTMI_CFG_RCGR_SRC_SEL_SRC1_FVAL                                          0x1
#define HWIO_GCC_USB30_PRIM_MOCK_UTMI_CFG_RCGR_SRC_SEL_SRC2_FVAL                                          0x2
#define HWIO_GCC_USB30_PRIM_MOCK_UTMI_CFG_RCGR_SRC_SEL_SRC3_FVAL                                          0x3
#define HWIO_GCC_USB30_PRIM_MOCK_UTMI_CFG_RCGR_SRC_SEL_SRC4_FVAL                                          0x4
#define HWIO_GCC_USB30_PRIM_MOCK_UTMI_CFG_RCGR_SRC_SEL_SRC5_FVAL                                          0x5
#define HWIO_GCC_USB30_PRIM_MOCK_UTMI_CFG_RCGR_SRC_SEL_SRC6_FVAL                                          0x6
#define HWIO_GCC_USB30_PRIM_MOCK_UTMI_CFG_RCGR_SRC_SEL_SRC7_FVAL                                          0x7
#define HWIO_GCC_USB30_PRIM_MOCK_UTMI_CFG_RCGR_SRC_DIV_BMSK                                              0x1f
#define HWIO_GCC_USB30_PRIM_MOCK_UTMI_CFG_RCGR_SRC_DIV_SHFT                                               0x0
#define HWIO_GCC_USB30_PRIM_MOCK_UTMI_CFG_RCGR_SRC_DIV_BYPASS_FVAL                                        0x0
#define HWIO_GCC_USB30_PRIM_MOCK_UTMI_CFG_RCGR_SRC_DIV_DIV1_FVAL                                          0x1
#define HWIO_GCC_USB30_PRIM_MOCK_UTMI_CFG_RCGR_SRC_DIV_DIV1_5_FVAL                                        0x2
#define HWIO_GCC_USB30_PRIM_MOCK_UTMI_CFG_RCGR_SRC_DIV_DIV2_FVAL                                          0x3
#define HWIO_GCC_USB30_PRIM_MOCK_UTMI_CFG_RCGR_SRC_DIV_DIV2_5_FVAL                                        0x4
#define HWIO_GCC_USB30_PRIM_MOCK_UTMI_CFG_RCGR_SRC_DIV_DIV3_FVAL                                          0x5
#define HWIO_GCC_USB30_PRIM_MOCK_UTMI_CFG_RCGR_SRC_DIV_DIV3_5_FVAL                                        0x6
#define HWIO_GCC_USB30_PRIM_MOCK_UTMI_CFG_RCGR_SRC_DIV_DIV4_FVAL                                          0x7
#define HWIO_GCC_USB30_PRIM_MOCK_UTMI_CFG_RCGR_SRC_DIV_DIV4_5_FVAL                                        0x8
#define HWIO_GCC_USB30_PRIM_MOCK_UTMI_CFG_RCGR_SRC_DIV_DIV5_FVAL                                          0x9
#define HWIO_GCC_USB30_PRIM_MOCK_UTMI_CFG_RCGR_SRC_DIV_DIV5_5_FVAL                                        0xa
#define HWIO_GCC_USB30_PRIM_MOCK_UTMI_CFG_RCGR_SRC_DIV_DIV6_FVAL                                          0xb
#define HWIO_GCC_USB30_PRIM_MOCK_UTMI_CFG_RCGR_SRC_DIV_DIV6_5_FVAL                                        0xc
#define HWIO_GCC_USB30_PRIM_MOCK_UTMI_CFG_RCGR_SRC_DIV_DIV7_FVAL                                          0xd
#define HWIO_GCC_USB30_PRIM_MOCK_UTMI_CFG_RCGR_SRC_DIV_DIV7_5_FVAL                                        0xe
#define HWIO_GCC_USB30_PRIM_MOCK_UTMI_CFG_RCGR_SRC_DIV_DIV8_FVAL                                          0xf
#define HWIO_GCC_USB30_PRIM_MOCK_UTMI_CFG_RCGR_SRC_DIV_DIV8_5_FVAL                                       0x10
#define HWIO_GCC_USB30_PRIM_MOCK_UTMI_CFG_RCGR_SRC_DIV_DIV9_FVAL                                         0x11
#define HWIO_GCC_USB30_PRIM_MOCK_UTMI_CFG_RCGR_SRC_DIV_DIV9_5_FVAL                                       0x12
#define HWIO_GCC_USB30_PRIM_MOCK_UTMI_CFG_RCGR_SRC_DIV_DIV10_FVAL                                        0x13
#define HWIO_GCC_USB30_PRIM_MOCK_UTMI_CFG_RCGR_SRC_DIV_DIV10_5_FVAL                                      0x14
#define HWIO_GCC_USB30_PRIM_MOCK_UTMI_CFG_RCGR_SRC_DIV_DIV11_FVAL                                        0x15
#define HWIO_GCC_USB30_PRIM_MOCK_UTMI_CFG_RCGR_SRC_DIV_DIV11_5_FVAL                                      0x16
#define HWIO_GCC_USB30_PRIM_MOCK_UTMI_CFG_RCGR_SRC_DIV_DIV12_FVAL                                        0x17
#define HWIO_GCC_USB30_PRIM_MOCK_UTMI_CFG_RCGR_SRC_DIV_DIV12_5_FVAL                                      0x18
#define HWIO_GCC_USB30_PRIM_MOCK_UTMI_CFG_RCGR_SRC_DIV_DIV13_FVAL                                        0x19
#define HWIO_GCC_USB30_PRIM_MOCK_UTMI_CFG_RCGR_SRC_DIV_DIV13_5_FVAL                                      0x1a
#define HWIO_GCC_USB30_PRIM_MOCK_UTMI_CFG_RCGR_SRC_DIV_DIV14_FVAL                                        0x1b
#define HWIO_GCC_USB30_PRIM_MOCK_UTMI_CFG_RCGR_SRC_DIV_DIV14_5_FVAL                                      0x1c
#define HWIO_GCC_USB30_PRIM_MOCK_UTMI_CFG_RCGR_SRC_DIV_DIV15_FVAL                                        0x1d
#define HWIO_GCC_USB30_PRIM_MOCK_UTMI_CFG_RCGR_SRC_DIV_DIV15_5_FVAL                                      0x1e
#define HWIO_GCC_USB30_PRIM_MOCK_UTMI_CFG_RCGR_SRC_DIV_DIV16_FVAL                                        0x1f

#define HWIO_GCC_USB30_PRIM_MOCK_UTMI_POSTDIV_CDIVR_ADDR                                           (GCC_CLK_CTL_REG_REG_BASE      + 0x0000f04c)
#define HWIO_GCC_USB30_PRIM_MOCK_UTMI_POSTDIV_CDIVR_PHYS                                           (GCC_CLK_CTL_REG_REG_BASE_PHYS + 0x0000f04c)
#define HWIO_GCC_USB30_PRIM_MOCK_UTMI_POSTDIV_CDIVR_OFFS                                           (GCC_CLK_CTL_REG_REG_BASE_OFFS + 0x0000f04c)
#define HWIO_GCC_USB30_PRIM_MOCK_UTMI_POSTDIV_CDIVR_RMSK                                                  0x3
#define HWIO_GCC_USB30_PRIM_MOCK_UTMI_POSTDIV_CDIVR_IN          \
        in_dword(HWIO_GCC_USB30_PRIM_MOCK_UTMI_POSTDIV_CDIVR_ADDR)
#define HWIO_GCC_USB30_PRIM_MOCK_UTMI_POSTDIV_CDIVR_INM(m)      \
        in_dword_masked(HWIO_GCC_USB30_PRIM_MOCK_UTMI_POSTDIV_CDIVR_ADDR, m)
#define HWIO_GCC_USB30_PRIM_MOCK_UTMI_POSTDIV_CDIVR_OUT(v)      \
        out_dword(HWIO_GCC_USB30_PRIM_MOCK_UTMI_POSTDIV_CDIVR_ADDR,v)
#define HWIO_GCC_USB30_PRIM_MOCK_UTMI_POSTDIV_CDIVR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GCC_USB30_PRIM_MOCK_UTMI_POSTDIV_CDIVR_ADDR,m,v,HWIO_GCC_USB30_PRIM_MOCK_UTMI_POSTDIV_CDIVR_IN)
#define HWIO_GCC_USB30_PRIM_MOCK_UTMI_POSTDIV_CDIVR_CLK_DIV_BMSK                                          0x3
#define HWIO_GCC_USB30_PRIM_MOCK_UTMI_POSTDIV_CDIVR_CLK_DIV_SHFT                                          0x0

#define HWIO_GCC_USB3_PRIM_PHY_AUX_CBCR_ADDR                                                       (GCC_CLK_CTL_REG_REG_BASE      + 0x0000f050)
#define HWIO_GCC_USB3_PRIM_PHY_AUX_CBCR_PHYS                                                       (GCC_CLK_CTL_REG_REG_BASE_PHYS + 0x0000f050)
#define HWIO_GCC_USB3_PRIM_PHY_AUX_CBCR_OFFS                                                       (GCC_CLK_CTL_REG_REG_BASE_OFFS + 0x0000f050)
#define HWIO_GCC_USB3_PRIM_PHY_AUX_CBCR_RMSK                                                       0x80000005
#define HWIO_GCC_USB3_PRIM_PHY_AUX_CBCR_IN          \
        in_dword(HWIO_GCC_USB3_PRIM_PHY_AUX_CBCR_ADDR)
#define HWIO_GCC_USB3_PRIM_PHY_AUX_CBCR_INM(m)      \
        in_dword_masked(HWIO_GCC_USB3_PRIM_PHY_AUX_CBCR_ADDR, m)
#define HWIO_GCC_USB3_PRIM_PHY_AUX_CBCR_OUT(v)      \
        out_dword(HWIO_GCC_USB3_PRIM_PHY_AUX_CBCR_ADDR,v)
#define HWIO_GCC_USB3_PRIM_PHY_AUX_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GCC_USB3_PRIM_PHY_AUX_CBCR_ADDR,m,v,HWIO_GCC_USB3_PRIM_PHY_AUX_CBCR_IN)
#define HWIO_GCC_USB3_PRIM_PHY_AUX_CBCR_CLK_OFF_BMSK                                               0x80000000
#define HWIO_GCC_USB3_PRIM_PHY_AUX_CBCR_CLK_OFF_SHFT                                                     0x1f
#define HWIO_GCC_USB3_PRIM_PHY_AUX_CBCR_CLK_ARES_BMSK                                                     0x4
#define HWIO_GCC_USB3_PRIM_PHY_AUX_CBCR_CLK_ARES_SHFT                                                     0x2
#define HWIO_GCC_USB3_PRIM_PHY_AUX_CBCR_CLK_ARES_NO_RESET_FVAL                                            0x0
#define HWIO_GCC_USB3_PRIM_PHY_AUX_CBCR_CLK_ARES_RESET_FVAL                                               0x1
#define HWIO_GCC_USB3_PRIM_PHY_AUX_CBCR_CLK_ENABLE_BMSK                                                   0x1
#define HWIO_GCC_USB3_PRIM_PHY_AUX_CBCR_CLK_ENABLE_SHFT                                                   0x0
#define HWIO_GCC_USB3_PRIM_PHY_AUX_CBCR_CLK_ENABLE_DISABLE_FVAL                                           0x0
#define HWIO_GCC_USB3_PRIM_PHY_AUX_CBCR_CLK_ENABLE_ENABLE_FVAL                                            0x1

#define HWIO_GCC_USB3_PRIM_PHY_COM_AUX_CBCR_ADDR                                                   (GCC_CLK_CTL_REG_REG_BASE      + 0x0000f054)
#define HWIO_GCC_USB3_PRIM_PHY_COM_AUX_CBCR_PHYS                                                   (GCC_CLK_CTL_REG_REG_BASE_PHYS + 0x0000f054)
#define HWIO_GCC_USB3_PRIM_PHY_COM_AUX_CBCR_OFFS                                                   (GCC_CLK_CTL_REG_REG_BASE_OFFS + 0x0000f054)
#define HWIO_GCC_USB3_PRIM_PHY_COM_AUX_CBCR_RMSK                                                   0x80000005
#define HWIO_GCC_USB3_PRIM_PHY_COM_AUX_CBCR_IN          \
        in_dword(HWIO_GCC_USB3_PRIM_PHY_COM_AUX_CBCR_ADDR)
#define HWIO_GCC_USB3_PRIM_PHY_COM_AUX_CBCR_INM(m)      \
        in_dword_masked(HWIO_GCC_USB3_PRIM_PHY_COM_AUX_CBCR_ADDR, m)
#define HWIO_GCC_USB3_PRIM_PHY_COM_AUX_CBCR_OUT(v)      \
        out_dword(HWIO_GCC_USB3_PRIM_PHY_COM_AUX_CBCR_ADDR,v)
#define HWIO_GCC_USB3_PRIM_PHY_COM_AUX_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GCC_USB3_PRIM_PHY_COM_AUX_CBCR_ADDR,m,v,HWIO_GCC_USB3_PRIM_PHY_COM_AUX_CBCR_IN)
#define HWIO_GCC_USB3_PRIM_PHY_COM_AUX_CBCR_CLK_OFF_BMSK                                           0x80000000
#define HWIO_GCC_USB3_PRIM_PHY_COM_AUX_CBCR_CLK_OFF_SHFT                                                 0x1f
#define HWIO_GCC_USB3_PRIM_PHY_COM_AUX_CBCR_CLK_ARES_BMSK                                                 0x4
#define HWIO_GCC_USB3_PRIM_PHY_COM_AUX_CBCR_CLK_ARES_SHFT                                                 0x2
#define HWIO_GCC_USB3_PRIM_PHY_COM_AUX_CBCR_CLK_ARES_NO_RESET_FVAL                                        0x0
#define HWIO_GCC_USB3_PRIM_PHY_COM_AUX_CBCR_CLK_ARES_RESET_FVAL                                           0x1
#define HWIO_GCC_USB3_PRIM_PHY_COM_AUX_CBCR_CLK_ENABLE_BMSK                                               0x1
#define HWIO_GCC_USB3_PRIM_PHY_COM_AUX_CBCR_CLK_ENABLE_SHFT                                               0x0
#define HWIO_GCC_USB3_PRIM_PHY_COM_AUX_CBCR_CLK_ENABLE_DISABLE_FVAL                                       0x0
#define HWIO_GCC_USB3_PRIM_PHY_COM_AUX_CBCR_CLK_ENABLE_ENABLE_FVAL                                        0x1

#define HWIO_GCC_USB3_PRIM_PHY_PIPE_CBCR_ADDR                                                      (GCC_CLK_CTL_REG_REG_BASE      + 0x0000f058)
#define HWIO_GCC_USB3_PRIM_PHY_PIPE_CBCR_PHYS                                                      (GCC_CLK_CTL_REG_REG_BASE_PHYS + 0x0000f058)
#define HWIO_GCC_USB3_PRIM_PHY_PIPE_CBCR_OFFS                                                      (GCC_CLK_CTL_REG_REG_BASE_OFFS + 0x0000f058)
#define HWIO_GCC_USB3_PRIM_PHY_PIPE_CBCR_RMSK                                                      0x80000005
#define HWIO_GCC_USB3_PRIM_PHY_PIPE_CBCR_IN          \
        in_dword(HWIO_GCC_USB3_PRIM_PHY_PIPE_CBCR_ADDR)
#define HWIO_GCC_USB3_PRIM_PHY_PIPE_CBCR_INM(m)      \
        in_dword_masked(HWIO_GCC_USB3_PRIM_PHY_PIPE_CBCR_ADDR, m)
#define HWIO_GCC_USB3_PRIM_PHY_PIPE_CBCR_OUT(v)      \
        out_dword(HWIO_GCC_USB3_PRIM_PHY_PIPE_CBCR_ADDR,v)
#define HWIO_GCC_USB3_PRIM_PHY_PIPE_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GCC_USB3_PRIM_PHY_PIPE_CBCR_ADDR,m,v,HWIO_GCC_USB3_PRIM_PHY_PIPE_CBCR_IN)
#define HWIO_GCC_USB3_PRIM_PHY_PIPE_CBCR_CLK_OFF_BMSK                                              0x80000000
#define HWIO_GCC_USB3_PRIM_PHY_PIPE_CBCR_CLK_OFF_SHFT                                                    0x1f
#define HWIO_GCC_USB3_PRIM_PHY_PIPE_CBCR_CLK_ARES_BMSK                                                    0x4
#define HWIO_GCC_USB3_PRIM_PHY_PIPE_CBCR_CLK_ARES_SHFT                                                    0x2
#define HWIO_GCC_USB3_PRIM_PHY_PIPE_CBCR_CLK_ARES_NO_RESET_FVAL                                           0x0
#define HWIO_GCC_USB3_PRIM_PHY_PIPE_CBCR_CLK_ARES_RESET_FVAL                                              0x1
#define HWIO_GCC_USB3_PRIM_PHY_PIPE_CBCR_CLK_ENABLE_BMSK                                                  0x1
#define HWIO_GCC_USB3_PRIM_PHY_PIPE_CBCR_CLK_ENABLE_SHFT                                                  0x0
#define HWIO_GCC_USB3_PRIM_PHY_PIPE_CBCR_CLK_ENABLE_DISABLE_FVAL                                          0x0
#define HWIO_GCC_USB3_PRIM_PHY_PIPE_CBCR_CLK_ENABLE_ENABLE_FVAL                                           0x1

#define HWIO_GCC_USB3_PRIM_PHY_AUX_CMD_RCGR_ADDR                                                   (GCC_CLK_CTL_REG_REG_BASE      + 0x0000f060)
#define HWIO_GCC_USB3_PRIM_PHY_AUX_CMD_RCGR_PHYS                                                   (GCC_CLK_CTL_REG_REG_BASE_PHYS + 0x0000f060)
#define HWIO_GCC_USB3_PRIM_PHY_AUX_CMD_RCGR_OFFS                                                   (GCC_CLK_CTL_REG_REG_BASE_OFFS + 0x0000f060)
#define HWIO_GCC_USB3_PRIM_PHY_AUX_CMD_RCGR_RMSK                                                   0x80000013
#define HWIO_GCC_USB3_PRIM_PHY_AUX_CMD_RCGR_IN          \
        in_dword(HWIO_GCC_USB3_PRIM_PHY_AUX_CMD_RCGR_ADDR)
#define HWIO_GCC_USB3_PRIM_PHY_AUX_CMD_RCGR_INM(m)      \
        in_dword_masked(HWIO_GCC_USB3_PRIM_PHY_AUX_CMD_RCGR_ADDR, m)
#define HWIO_GCC_USB3_PRIM_PHY_AUX_CMD_RCGR_OUT(v)      \
        out_dword(HWIO_GCC_USB3_PRIM_PHY_AUX_CMD_RCGR_ADDR,v)
#define HWIO_GCC_USB3_PRIM_PHY_AUX_CMD_RCGR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GCC_USB3_PRIM_PHY_AUX_CMD_RCGR_ADDR,m,v,HWIO_GCC_USB3_PRIM_PHY_AUX_CMD_RCGR_IN)
#define HWIO_GCC_USB3_PRIM_PHY_AUX_CMD_RCGR_ROOT_OFF_BMSK                                          0x80000000
#define HWIO_GCC_USB3_PRIM_PHY_AUX_CMD_RCGR_ROOT_OFF_SHFT                                                0x1f
#define HWIO_GCC_USB3_PRIM_PHY_AUX_CMD_RCGR_DIRTY_CFG_RCGR_BMSK                                          0x10
#define HWIO_GCC_USB3_PRIM_PHY_AUX_CMD_RCGR_DIRTY_CFG_RCGR_SHFT                                           0x4
#define HWIO_GCC_USB3_PRIM_PHY_AUX_CMD_RCGR_ROOT_EN_BMSK                                                  0x2
#define HWIO_GCC_USB3_PRIM_PHY_AUX_CMD_RCGR_ROOT_EN_SHFT                                                  0x1
#define HWIO_GCC_USB3_PRIM_PHY_AUX_CMD_RCGR_ROOT_EN_DISABLE_FVAL                                          0x0
#define HWIO_GCC_USB3_PRIM_PHY_AUX_CMD_RCGR_ROOT_EN_ENABLE_FVAL                                           0x1
#define HWIO_GCC_USB3_PRIM_PHY_AUX_CMD_RCGR_UPDATE_BMSK                                                   0x1
#define HWIO_GCC_USB3_PRIM_PHY_AUX_CMD_RCGR_UPDATE_SHFT                                                   0x0
#define HWIO_GCC_USB3_PRIM_PHY_AUX_CMD_RCGR_UPDATE_DISABLE_FVAL                                           0x0
#define HWIO_GCC_USB3_PRIM_PHY_AUX_CMD_RCGR_UPDATE_ENABLE_FVAL                                            0x1

#define HWIO_GCC_USB3_PRIM_PHY_AUX_CFG_RCGR_ADDR                                                   (GCC_CLK_CTL_REG_REG_BASE      + 0x0000f064)
#define HWIO_GCC_USB3_PRIM_PHY_AUX_CFG_RCGR_PHYS                                                   (GCC_CLK_CTL_REG_REG_BASE_PHYS + 0x0000f064)
#define HWIO_GCC_USB3_PRIM_PHY_AUX_CFG_RCGR_OFFS                                                   (GCC_CLK_CTL_REG_REG_BASE_OFFS + 0x0000f064)
#define HWIO_GCC_USB3_PRIM_PHY_AUX_CFG_RCGR_RMSK                                                     0x11071f
#define HWIO_GCC_USB3_PRIM_PHY_AUX_CFG_RCGR_IN          \
        in_dword(HWIO_GCC_USB3_PRIM_PHY_AUX_CFG_RCGR_ADDR)
#define HWIO_GCC_USB3_PRIM_PHY_AUX_CFG_RCGR_INM(m)      \
        in_dword_masked(HWIO_GCC_USB3_PRIM_PHY_AUX_CFG_RCGR_ADDR, m)
#define HWIO_GCC_USB3_PRIM_PHY_AUX_CFG_RCGR_OUT(v)      \
        out_dword(HWIO_GCC_USB3_PRIM_PHY_AUX_CFG_RCGR_ADDR,v)
#define HWIO_GCC_USB3_PRIM_PHY_AUX_CFG_RCGR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GCC_USB3_PRIM_PHY_AUX_CFG_RCGR_ADDR,m,v,HWIO_GCC_USB3_PRIM_PHY_AUX_CFG_RCGR_IN)
#define HWIO_GCC_USB3_PRIM_PHY_AUX_CFG_RCGR_HW_CLK_CONTROL_BMSK                                      0x100000
#define HWIO_GCC_USB3_PRIM_PHY_AUX_CFG_RCGR_HW_CLK_CONTROL_SHFT                                          0x14
#define HWIO_GCC_USB3_PRIM_PHY_AUX_CFG_RCGR_HW_CLK_CONTROL_DISABLE_FVAL                                   0x0
#define HWIO_GCC_USB3_PRIM_PHY_AUX_CFG_RCGR_HW_CLK_CONTROL_ENABLE_FVAL                                    0x1
#define HWIO_GCC_USB3_PRIM_PHY_AUX_CFG_RCGR_RCGLITE_DISABLE_BMSK                                      0x10000
#define HWIO_GCC_USB3_PRIM_PHY_AUX_CFG_RCGR_RCGLITE_DISABLE_SHFT                                         0x10
#define HWIO_GCC_USB3_PRIM_PHY_AUX_CFG_RCGR_RCGLITE_DISABLE_RCGLITE_ENABLED_FVAL                          0x0
#define HWIO_GCC_USB3_PRIM_PHY_AUX_CFG_RCGR_RCGLITE_DISABLE_RCGLITE_DISABLED_FVAL                         0x1
#define HWIO_GCC_USB3_PRIM_PHY_AUX_CFG_RCGR_SRC_SEL_BMSK                                                0x700
#define HWIO_GCC_USB3_PRIM_PHY_AUX_CFG_RCGR_SRC_SEL_SHFT                                                  0x8
#define HWIO_GCC_USB3_PRIM_PHY_AUX_CFG_RCGR_SRC_SEL_SRC0_FVAL                                             0x0
#define HWIO_GCC_USB3_PRIM_PHY_AUX_CFG_RCGR_SRC_SEL_SRC1_FVAL                                             0x1
#define HWIO_GCC_USB3_PRIM_PHY_AUX_CFG_RCGR_SRC_SEL_SRC2_FVAL                                             0x2
#define HWIO_GCC_USB3_PRIM_PHY_AUX_CFG_RCGR_SRC_SEL_SRC3_FVAL                                             0x3
#define HWIO_GCC_USB3_PRIM_PHY_AUX_CFG_RCGR_SRC_SEL_SRC4_FVAL                                             0x4
#define HWIO_GCC_USB3_PRIM_PHY_AUX_CFG_RCGR_SRC_SEL_SRC5_FVAL                                             0x5
#define HWIO_GCC_USB3_PRIM_PHY_AUX_CFG_RCGR_SRC_SEL_SRC6_FVAL                                             0x6
#define HWIO_GCC_USB3_PRIM_PHY_AUX_CFG_RCGR_SRC_SEL_SRC7_FVAL                                             0x7
#define HWIO_GCC_USB3_PRIM_PHY_AUX_CFG_RCGR_SRC_DIV_BMSK                                                 0x1f
#define HWIO_GCC_USB3_PRIM_PHY_AUX_CFG_RCGR_SRC_DIV_SHFT                                                  0x0
#define HWIO_GCC_USB3_PRIM_PHY_AUX_CFG_RCGR_SRC_DIV_BYPASS_FVAL                                           0x0
#define HWIO_GCC_USB3_PRIM_PHY_AUX_CFG_RCGR_SRC_DIV_DIV1_FVAL                                             0x1
#define HWIO_GCC_USB3_PRIM_PHY_AUX_CFG_RCGR_SRC_DIV_DIV1_5_FVAL                                           0x2
#define HWIO_GCC_USB3_PRIM_PHY_AUX_CFG_RCGR_SRC_DIV_DIV2_FVAL                                             0x3
#define HWIO_GCC_USB3_PRIM_PHY_AUX_CFG_RCGR_SRC_DIV_DIV2_5_FVAL                                           0x4
#define HWIO_GCC_USB3_PRIM_PHY_AUX_CFG_RCGR_SRC_DIV_DIV3_FVAL                                             0x5
#define HWIO_GCC_USB3_PRIM_PHY_AUX_CFG_RCGR_SRC_DIV_DIV3_5_FVAL                                           0x6
#define HWIO_GCC_USB3_PRIM_PHY_AUX_CFG_RCGR_SRC_DIV_DIV4_FVAL                                             0x7
#define HWIO_GCC_USB3_PRIM_PHY_AUX_CFG_RCGR_SRC_DIV_DIV4_5_FVAL                                           0x8
#define HWIO_GCC_USB3_PRIM_PHY_AUX_CFG_RCGR_SRC_DIV_DIV5_FVAL                                             0x9
#define HWIO_GCC_USB3_PRIM_PHY_AUX_CFG_RCGR_SRC_DIV_DIV5_5_FVAL                                           0xa
#define HWIO_GCC_USB3_PRIM_PHY_AUX_CFG_RCGR_SRC_DIV_DIV6_FVAL                                             0xb
#define HWIO_GCC_USB3_PRIM_PHY_AUX_CFG_RCGR_SRC_DIV_DIV6_5_FVAL                                           0xc
#define HWIO_GCC_USB3_PRIM_PHY_AUX_CFG_RCGR_SRC_DIV_DIV7_FVAL                                             0xd
#define HWIO_GCC_USB3_PRIM_PHY_AUX_CFG_RCGR_SRC_DIV_DIV7_5_FVAL                                           0xe
#define HWIO_GCC_USB3_PRIM_PHY_AUX_CFG_RCGR_SRC_DIV_DIV8_FVAL                                             0xf
#define HWIO_GCC_USB3_PRIM_PHY_AUX_CFG_RCGR_SRC_DIV_DIV8_5_FVAL                                          0x10
#define HWIO_GCC_USB3_PRIM_PHY_AUX_CFG_RCGR_SRC_DIV_DIV9_FVAL                                            0x11
#define HWIO_GCC_USB3_PRIM_PHY_AUX_CFG_RCGR_SRC_DIV_DIV9_5_FVAL                                          0x12
#define HWIO_GCC_USB3_PRIM_PHY_AUX_CFG_RCGR_SRC_DIV_DIV10_FVAL                                           0x13
#define HWIO_GCC_USB3_PRIM_PHY_AUX_CFG_RCGR_SRC_DIV_DIV10_5_FVAL                                         0x14
#define HWIO_GCC_USB3_PRIM_PHY_AUX_CFG_RCGR_SRC_DIV_DIV11_FVAL                                           0x15
#define HWIO_GCC_USB3_PRIM_PHY_AUX_CFG_RCGR_SRC_DIV_DIV11_5_FVAL                                         0x16
#define HWIO_GCC_USB3_PRIM_PHY_AUX_CFG_RCGR_SRC_DIV_DIV12_FVAL                                           0x17
#define HWIO_GCC_USB3_PRIM_PHY_AUX_CFG_RCGR_SRC_DIV_DIV12_5_FVAL                                         0x18
#define HWIO_GCC_USB3_PRIM_PHY_AUX_CFG_RCGR_SRC_DIV_DIV13_FVAL                                           0x19
#define HWIO_GCC_USB3_PRIM_PHY_AUX_CFG_RCGR_SRC_DIV_DIV13_5_FVAL                                         0x1a
#define HWIO_GCC_USB3_PRIM_PHY_AUX_CFG_RCGR_SRC_DIV_DIV14_FVAL                                           0x1b
#define HWIO_GCC_USB3_PRIM_PHY_AUX_CFG_RCGR_SRC_DIV_DIV14_5_FVAL                                         0x1c
#define HWIO_GCC_USB3_PRIM_PHY_AUX_CFG_RCGR_SRC_DIV_DIV15_FVAL                                           0x1d
#define HWIO_GCC_USB3_PRIM_PHY_AUX_CFG_RCGR_SRC_DIV_DIV15_5_FVAL                                         0x1e
#define HWIO_GCC_USB3_PRIM_PHY_AUX_CFG_RCGR_SRC_DIV_DIV16_FVAL                                           0x1f

#define HWIO_GCC_USB30_SEC_BCR_ADDR                                                                (GCC_CLK_CTL_REG_REG_BASE      + 0x00010000)
#define HWIO_GCC_USB30_SEC_BCR_PHYS                                                                (GCC_CLK_CTL_REG_REG_BASE_PHYS + 0x00010000)
#define HWIO_GCC_USB30_SEC_BCR_OFFS                                                                (GCC_CLK_CTL_REG_REG_BASE_OFFS + 0x00010000)
#define HWIO_GCC_USB30_SEC_BCR_RMSK                                                                       0x1
#define HWIO_GCC_USB30_SEC_BCR_IN          \
        in_dword(HWIO_GCC_USB30_SEC_BCR_ADDR)
#define HWIO_GCC_USB30_SEC_BCR_INM(m)      \
        in_dword_masked(HWIO_GCC_USB30_SEC_BCR_ADDR, m)
#define HWIO_GCC_USB30_SEC_BCR_OUT(v)      \
        out_dword(HWIO_GCC_USB30_SEC_BCR_ADDR,v)
#define HWIO_GCC_USB30_SEC_BCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GCC_USB30_SEC_BCR_ADDR,m,v,HWIO_GCC_USB30_SEC_BCR_IN)
#define HWIO_GCC_USB30_SEC_BCR_BLK_ARES_BMSK                                                              0x1
#define HWIO_GCC_USB30_SEC_BCR_BLK_ARES_SHFT                                                              0x0
#define HWIO_GCC_USB30_SEC_BCR_BLK_ARES_DISABLE_FVAL                                                      0x0
#define HWIO_GCC_USB30_SEC_BCR_BLK_ARES_ENABLE_FVAL                                                       0x1

#define HWIO_GCC_USB30_SEC_GDSCR_ADDR                                                              (GCC_CLK_CTL_REG_REG_BASE      + 0x00010004)
#define HWIO_GCC_USB30_SEC_GDSCR_PHYS                                                              (GCC_CLK_CTL_REG_REG_BASE_PHYS + 0x00010004)
#define HWIO_GCC_USB30_SEC_GDSCR_OFFS                                                              (GCC_CLK_CTL_REG_REG_BASE_OFFS + 0x00010004)
#define HWIO_GCC_USB30_SEC_GDSCR_RMSK                                                              0xf8ffffff
#define HWIO_GCC_USB30_SEC_GDSCR_IN          \
        in_dword(HWIO_GCC_USB30_SEC_GDSCR_ADDR)
#define HWIO_GCC_USB30_SEC_GDSCR_INM(m)      \
        in_dword_masked(HWIO_GCC_USB30_SEC_GDSCR_ADDR, m)
#define HWIO_GCC_USB30_SEC_GDSCR_OUT(v)      \
        out_dword(HWIO_GCC_USB30_SEC_GDSCR_ADDR,v)
#define HWIO_GCC_USB30_SEC_GDSCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GCC_USB30_SEC_GDSCR_ADDR,m,v,HWIO_GCC_USB30_SEC_GDSCR_IN)
#define HWIO_GCC_USB30_SEC_GDSCR_PWR_ON_BMSK                                                       0x80000000
#define HWIO_GCC_USB30_SEC_GDSCR_PWR_ON_SHFT                                                             0x1f
#define HWIO_GCC_USB30_SEC_GDSCR_GDSC_STATE_BMSK                                                   0x78000000
#define HWIO_GCC_USB30_SEC_GDSCR_GDSC_STATE_SHFT                                                         0x1b
#define HWIO_GCC_USB30_SEC_GDSCR_EN_REST_WAIT_BMSK                                                   0xf00000
#define HWIO_GCC_USB30_SEC_GDSCR_EN_REST_WAIT_SHFT                                                       0x14
#define HWIO_GCC_USB30_SEC_GDSCR_EN_FEW_WAIT_BMSK                                                     0xf0000
#define HWIO_GCC_USB30_SEC_GDSCR_EN_FEW_WAIT_SHFT                                                        0x10
#define HWIO_GCC_USB30_SEC_GDSCR_CLK_DIS_WAIT_BMSK                                                     0xf000
#define HWIO_GCC_USB30_SEC_GDSCR_CLK_DIS_WAIT_SHFT                                                        0xc
#define HWIO_GCC_USB30_SEC_GDSCR_RETAIN_FF_ENABLE_BMSK                                                  0x800
#define HWIO_GCC_USB30_SEC_GDSCR_RETAIN_FF_ENABLE_SHFT                                                    0xb
#define HWIO_GCC_USB30_SEC_GDSCR_RETAIN_FF_ENABLE_DISABLE_FVAL                                            0x0
#define HWIO_GCC_USB30_SEC_GDSCR_RETAIN_FF_ENABLE_ENABLE_FVAL                                             0x1
#define HWIO_GCC_USB30_SEC_GDSCR_RESTORE_BMSK                                                           0x400
#define HWIO_GCC_USB30_SEC_GDSCR_RESTORE_SHFT                                                             0xa
#define HWIO_GCC_USB30_SEC_GDSCR_RESTORE_DISABLE_FVAL                                                     0x0
#define HWIO_GCC_USB30_SEC_GDSCR_RESTORE_ENABLE_FVAL                                                      0x1
#define HWIO_GCC_USB30_SEC_GDSCR_SAVE_BMSK                                                              0x200
#define HWIO_GCC_USB30_SEC_GDSCR_SAVE_SHFT                                                                0x9
#define HWIO_GCC_USB30_SEC_GDSCR_SAVE_DISABLE_FVAL                                                        0x0
#define HWIO_GCC_USB30_SEC_GDSCR_SAVE_ENABLE_FVAL                                                         0x1
#define HWIO_GCC_USB30_SEC_GDSCR_RETAIN_BMSK                                                            0x100
#define HWIO_GCC_USB30_SEC_GDSCR_RETAIN_SHFT                                                              0x8
#define HWIO_GCC_USB30_SEC_GDSCR_RETAIN_DISABLE_FVAL                                                      0x0
#define HWIO_GCC_USB30_SEC_GDSCR_RETAIN_ENABLE_FVAL                                                       0x1
#define HWIO_GCC_USB30_SEC_GDSCR_EN_REST_BMSK                                                            0x80
#define HWIO_GCC_USB30_SEC_GDSCR_EN_REST_SHFT                                                             0x7
#define HWIO_GCC_USB30_SEC_GDSCR_EN_REST_DISABLE_FVAL                                                     0x0
#define HWIO_GCC_USB30_SEC_GDSCR_EN_REST_ENABLE_FVAL                                                      0x1
#define HWIO_GCC_USB30_SEC_GDSCR_EN_FEW_BMSK                                                             0x40
#define HWIO_GCC_USB30_SEC_GDSCR_EN_FEW_SHFT                                                              0x6
#define HWIO_GCC_USB30_SEC_GDSCR_EN_FEW_DISABLE_FVAL                                                      0x0
#define HWIO_GCC_USB30_SEC_GDSCR_EN_FEW_ENABLE_FVAL                                                       0x1
#define HWIO_GCC_USB30_SEC_GDSCR_CLAMP_IO_BMSK                                                           0x20
#define HWIO_GCC_USB30_SEC_GDSCR_CLAMP_IO_SHFT                                                            0x5
#define HWIO_GCC_USB30_SEC_GDSCR_CLAMP_IO_DISABLE_FVAL                                                    0x0
#define HWIO_GCC_USB30_SEC_GDSCR_CLAMP_IO_ENABLE_FVAL                                                     0x1
#define HWIO_GCC_USB30_SEC_GDSCR_CLK_DISABLE_BMSK                                                        0x10
#define HWIO_GCC_USB30_SEC_GDSCR_CLK_DISABLE_SHFT                                                         0x4
#define HWIO_GCC_USB30_SEC_GDSCR_CLK_DISABLE_CLK_NOT_DISABLE_FVAL                                         0x0
#define HWIO_GCC_USB30_SEC_GDSCR_CLK_DISABLE_CLK_DISABLE_FVAL                                             0x1
#define HWIO_GCC_USB30_SEC_GDSCR_PD_ARES_BMSK                                                             0x8
#define HWIO_GCC_USB30_SEC_GDSCR_PD_ARES_SHFT                                                             0x3
#define HWIO_GCC_USB30_SEC_GDSCR_PD_ARES_NO_RESET_FVAL                                                    0x0
#define HWIO_GCC_USB30_SEC_GDSCR_PD_ARES_RESET_FVAL                                                       0x1
#define HWIO_GCC_USB30_SEC_GDSCR_SW_OVERRIDE_BMSK                                                         0x4
#define HWIO_GCC_USB30_SEC_GDSCR_SW_OVERRIDE_SHFT                                                         0x2
#define HWIO_GCC_USB30_SEC_GDSCR_SW_OVERRIDE_DISABLE_FVAL                                                 0x0
#define HWIO_GCC_USB30_SEC_GDSCR_SW_OVERRIDE_ENABLE_FVAL                                                  0x1
#define HWIO_GCC_USB30_SEC_GDSCR_HW_CONTROL_BMSK                                                          0x2
#define HWIO_GCC_USB30_SEC_GDSCR_HW_CONTROL_SHFT                                                          0x1
#define HWIO_GCC_USB30_SEC_GDSCR_HW_CONTROL_DISABLE_FVAL                                                  0x0
#define HWIO_GCC_USB30_SEC_GDSCR_HW_CONTROL_ENABLE_FVAL                                                   0x1
#define HWIO_GCC_USB30_SEC_GDSCR_SW_COLLAPSE_BMSK                                                         0x1
#define HWIO_GCC_USB30_SEC_GDSCR_SW_COLLAPSE_SHFT                                                         0x0
#define HWIO_GCC_USB30_SEC_GDSCR_SW_COLLAPSE_DISABLE_FVAL                                                 0x0
#define HWIO_GCC_USB30_SEC_GDSCR_SW_COLLAPSE_ENABLE_FVAL                                                  0x1

#define HWIO_GCC_USB30_SEC_CFG_GDSCR_ADDR                                                          (GCC_CLK_CTL_REG_REG_BASE      + 0x00010008)
#define HWIO_GCC_USB30_SEC_CFG_GDSCR_PHYS                                                          (GCC_CLK_CTL_REG_REG_BASE_PHYS + 0x00010008)
#define HWIO_GCC_USB30_SEC_CFG_GDSCR_OFFS                                                          (GCC_CLK_CTL_REG_REG_BASE_OFFS + 0x00010008)
#define HWIO_GCC_USB30_SEC_CFG_GDSCR_RMSK                                                          0xffffffff
#define HWIO_GCC_USB30_SEC_CFG_GDSCR_IN          \
        in_dword(HWIO_GCC_USB30_SEC_CFG_GDSCR_ADDR)
#define HWIO_GCC_USB30_SEC_CFG_GDSCR_INM(m)      \
        in_dword_masked(HWIO_GCC_USB30_SEC_CFG_GDSCR_ADDR, m)
#define HWIO_GCC_USB30_SEC_CFG_GDSCR_OUT(v)      \
        out_dword(HWIO_GCC_USB30_SEC_CFG_GDSCR_ADDR,v)
#define HWIO_GCC_USB30_SEC_CFG_GDSCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GCC_USB30_SEC_CFG_GDSCR_ADDR,m,v,HWIO_GCC_USB30_SEC_CFG_GDSCR_IN)
#define HWIO_GCC_USB30_SEC_CFG_GDSCR_GDSC_SPARE_CTRL_IN_BMSK                                       0xf0000000
#define HWIO_GCC_USB30_SEC_CFG_GDSCR_GDSC_SPARE_CTRL_IN_SHFT                                             0x1c
#define HWIO_GCC_USB30_SEC_CFG_GDSCR_GDSC_SPARE_CTRL_OUT_BMSK                                       0xc000000
#define HWIO_GCC_USB30_SEC_CFG_GDSCR_GDSC_SPARE_CTRL_OUT_SHFT                                            0x1a
#define HWIO_GCC_USB30_SEC_CFG_GDSCR_GDSC_PWR_DWN_START_BMSK                                        0x2000000
#define HWIO_GCC_USB30_SEC_CFG_GDSCR_GDSC_PWR_DWN_START_SHFT                                             0x19
#define HWIO_GCC_USB30_SEC_CFG_GDSCR_GDSC_PWR_UP_START_BMSK                                         0x1000000
#define HWIO_GCC_USB30_SEC_CFG_GDSCR_GDSC_PWR_UP_START_SHFT                                              0x18
#define HWIO_GCC_USB30_SEC_CFG_GDSCR_GDSC_CFG_FSM_STATE_STATUS_BMSK                                  0xf00000
#define HWIO_GCC_USB30_SEC_CFG_GDSCR_GDSC_CFG_FSM_STATE_STATUS_SHFT                                      0x14
#define HWIO_GCC_USB30_SEC_CFG_GDSCR_GDSC_MEM_PWR_ACK_STATUS_BMSK                                     0x80000
#define HWIO_GCC_USB30_SEC_CFG_GDSCR_GDSC_MEM_PWR_ACK_STATUS_SHFT                                        0x13
#define HWIO_GCC_USB30_SEC_CFG_GDSCR_GDSC_ENR_ACK_STATUS_BMSK                                         0x40000
#define HWIO_GCC_USB30_SEC_CFG_GDSCR_GDSC_ENR_ACK_STATUS_SHFT                                            0x12
#define HWIO_GCC_USB30_SEC_CFG_GDSCR_GDSC_ENF_ACK_STATUS_BMSK                                         0x20000
#define HWIO_GCC_USB30_SEC_CFG_GDSCR_GDSC_ENF_ACK_STATUS_SHFT                                            0x11
#define HWIO_GCC_USB30_SEC_CFG_GDSCR_GDSC_POWER_UP_COMPLETE_BMSK                                      0x10000
#define HWIO_GCC_USB30_SEC_CFG_GDSCR_GDSC_POWER_UP_COMPLETE_SHFT                                         0x10
#define HWIO_GCC_USB30_SEC_CFG_GDSCR_GDSC_POWER_DOWN_COMPLETE_BMSK                                     0x8000
#define HWIO_GCC_USB30_SEC_CFG_GDSCR_GDSC_POWER_DOWN_COMPLETE_SHFT                                        0xf
#define HWIO_GCC_USB30_SEC_CFG_GDSCR_SOFTWARE_CONTROL_OVERRIDE_BMSK                                    0x7800
#define HWIO_GCC_USB30_SEC_CFG_GDSCR_SOFTWARE_CONTROL_OVERRIDE_SHFT                                       0xb
#define HWIO_GCC_USB30_SEC_CFG_GDSCR_GDSC_HANDSHAKE_DIS_BMSK                                            0x400
#define HWIO_GCC_USB30_SEC_CFG_GDSCR_GDSC_HANDSHAKE_DIS_SHFT                                              0xa
#define HWIO_GCC_USB30_SEC_CFG_GDSCR_GDSC_MEM_PERI_FORCE_IN_SW_BMSK                                     0x200
#define HWIO_GCC_USB30_SEC_CFG_GDSCR_GDSC_MEM_PERI_FORCE_IN_SW_SHFT                                       0x9
#define HWIO_GCC_USB30_SEC_CFG_GDSCR_GDSC_MEM_CORE_FORCE_IN_SW_BMSK                                     0x100
#define HWIO_GCC_USB30_SEC_CFG_GDSCR_GDSC_MEM_CORE_FORCE_IN_SW_SHFT                                       0x8
#define HWIO_GCC_USB30_SEC_CFG_GDSCR_GDSC_PHASE_RESET_EN_SW_BMSK                                         0x80
#define HWIO_GCC_USB30_SEC_CFG_GDSCR_GDSC_PHASE_RESET_EN_SW_SHFT                                          0x7
#define HWIO_GCC_USB30_SEC_CFG_GDSCR_GDSC_PHASE_RESET_DELAY_COUNT_SW_BMSK                                0x60
#define HWIO_GCC_USB30_SEC_CFG_GDSCR_GDSC_PHASE_RESET_DELAY_COUNT_SW_SHFT                                 0x5
#define HWIO_GCC_USB30_SEC_CFG_GDSCR_GDSC_PSCBC_PWR_DWN_SW_BMSK                                          0x10
#define HWIO_GCC_USB30_SEC_CFG_GDSCR_GDSC_PSCBC_PWR_DWN_SW_SHFT                                           0x4
#define HWIO_GCC_USB30_SEC_CFG_GDSCR_UNCLAMP_IO_SOFTWARE_OVERRIDE_BMSK                                    0x8
#define HWIO_GCC_USB30_SEC_CFG_GDSCR_UNCLAMP_IO_SOFTWARE_OVERRIDE_SHFT                                    0x3
#define HWIO_GCC_USB30_SEC_CFG_GDSCR_SAVE_RESTORE_SOFTWARE_OVERRIDE_BMSK                                  0x4
#define HWIO_GCC_USB30_SEC_CFG_GDSCR_SAVE_RESTORE_SOFTWARE_OVERRIDE_SHFT                                  0x2
#define HWIO_GCC_USB30_SEC_CFG_GDSCR_CLAMP_IO_SOFTWARE_OVERRIDE_BMSK                                      0x2
#define HWIO_GCC_USB30_SEC_CFG_GDSCR_CLAMP_IO_SOFTWARE_OVERRIDE_SHFT                                      0x1
#define HWIO_GCC_USB30_SEC_CFG_GDSCR_DISABLE_CLK_SOFTWARE_OVERRIDE_BMSK                                   0x1
#define HWIO_GCC_USB30_SEC_CFG_GDSCR_DISABLE_CLK_SOFTWARE_OVERRIDE_SHFT                                   0x0

#define HWIO_GCC_USB30_SEC_CFG2_GDSCR_ADDR                                                         (GCC_CLK_CTL_REG_REG_BASE      + 0x0001000c)
#define HWIO_GCC_USB30_SEC_CFG2_GDSCR_PHYS                                                         (GCC_CLK_CTL_REG_REG_BASE_PHYS + 0x0001000c)
#define HWIO_GCC_USB30_SEC_CFG2_GDSCR_OFFS                                                         (GCC_CLK_CTL_REG_REG_BASE_OFFS + 0x0001000c)
#define HWIO_GCC_USB30_SEC_CFG2_GDSCR_RMSK                                                            0x1ffff
#define HWIO_GCC_USB30_SEC_CFG2_GDSCR_IN          \
        in_dword(HWIO_GCC_USB30_SEC_CFG2_GDSCR_ADDR)
#define HWIO_GCC_USB30_SEC_CFG2_GDSCR_INM(m)      \
        in_dword_masked(HWIO_GCC_USB30_SEC_CFG2_GDSCR_ADDR, m)
#define HWIO_GCC_USB30_SEC_CFG2_GDSCR_OUT(v)      \
        out_dword(HWIO_GCC_USB30_SEC_CFG2_GDSCR_ADDR,v)
#define HWIO_GCC_USB30_SEC_CFG2_GDSCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GCC_USB30_SEC_CFG2_GDSCR_ADDR,m,v,HWIO_GCC_USB30_SEC_CFG2_GDSCR_IN)
#define HWIO_GCC_USB30_SEC_CFG2_GDSCR_GDSC_CLAMP_MEM_SW_BMSK                                          0x10000
#define HWIO_GCC_USB30_SEC_CFG2_GDSCR_GDSC_CLAMP_MEM_SW_SHFT                                             0x10
#define HWIO_GCC_USB30_SEC_CFG2_GDSCR_DLY_MEM_PWR_UP_BMSK                                              0xf000
#define HWIO_GCC_USB30_SEC_CFG2_GDSCR_DLY_MEM_PWR_UP_SHFT                                                 0xc
#define HWIO_GCC_USB30_SEC_CFG2_GDSCR_DLY_DEASSERT_CLAMP_MEM_BMSK                                       0xf00
#define HWIO_GCC_USB30_SEC_CFG2_GDSCR_DLY_DEASSERT_CLAMP_MEM_SHFT                                         0x8
#define HWIO_GCC_USB30_SEC_CFG2_GDSCR_DLY_ASSERT_CLAMP_MEM_BMSK                                          0xf0
#define HWIO_GCC_USB30_SEC_CFG2_GDSCR_DLY_ASSERT_CLAMP_MEM_SHFT                                           0x4
#define HWIO_GCC_USB30_SEC_CFG2_GDSCR_MEM_PWR_DWN_TIMEOUT_BMSK                                            0xf
#define HWIO_GCC_USB30_SEC_CFG2_GDSCR_MEM_PWR_DWN_TIMEOUT_SHFT                                            0x0

#define HWIO_GCC_USB30_SEC_MASTER_CBCR_ADDR                                                        (GCC_CLK_CTL_REG_REG_BASE      + 0x00010010)
#define HWIO_GCC_USB30_SEC_MASTER_CBCR_PHYS                                                        (GCC_CLK_CTL_REG_REG_BASE_PHYS + 0x00010010)
#define HWIO_GCC_USB30_SEC_MASTER_CBCR_OFFS                                                        (GCC_CLK_CTL_REG_REG_BASE_OFFS + 0x00010010)
#define HWIO_GCC_USB30_SEC_MASTER_CBCR_RMSK                                                        0x80007ff5
#define HWIO_GCC_USB30_SEC_MASTER_CBCR_IN          \
        in_dword(HWIO_GCC_USB30_SEC_MASTER_CBCR_ADDR)
#define HWIO_GCC_USB30_SEC_MASTER_CBCR_INM(m)      \
        in_dword_masked(HWIO_GCC_USB30_SEC_MASTER_CBCR_ADDR, m)
#define HWIO_GCC_USB30_SEC_MASTER_CBCR_OUT(v)      \
        out_dword(HWIO_GCC_USB30_SEC_MASTER_CBCR_ADDR,v)
#define HWIO_GCC_USB30_SEC_MASTER_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GCC_USB30_SEC_MASTER_CBCR_ADDR,m,v,HWIO_GCC_USB30_SEC_MASTER_CBCR_IN)
#define HWIO_GCC_USB30_SEC_MASTER_CBCR_CLK_OFF_BMSK                                                0x80000000
#define HWIO_GCC_USB30_SEC_MASTER_CBCR_CLK_OFF_SHFT                                                      0x1f
#define HWIO_GCC_USB30_SEC_MASTER_CBCR_FORCE_MEM_CORE_ON_BMSK                                          0x4000
#define HWIO_GCC_USB30_SEC_MASTER_CBCR_FORCE_MEM_CORE_ON_SHFT                                             0xe
#define HWIO_GCC_USB30_SEC_MASTER_CBCR_FORCE_MEM_CORE_ON_FORCE_DISABLE_FVAL                               0x0
#define HWIO_GCC_USB30_SEC_MASTER_CBCR_FORCE_MEM_CORE_ON_FORCE_ENABLE_FVAL                                0x1
#define HWIO_GCC_USB30_SEC_MASTER_CBCR_FORCE_MEM_PERIPH_ON_BMSK                                        0x2000
#define HWIO_GCC_USB30_SEC_MASTER_CBCR_FORCE_MEM_PERIPH_ON_SHFT                                           0xd
#define HWIO_GCC_USB30_SEC_MASTER_CBCR_FORCE_MEM_PERIPH_ON_FORCE_DISABLE_FVAL                             0x0
#define HWIO_GCC_USB30_SEC_MASTER_CBCR_FORCE_MEM_PERIPH_ON_FORCE_ENABLE_FVAL                              0x1
#define HWIO_GCC_USB30_SEC_MASTER_CBCR_FORCE_MEM_PERIPH_OFF_BMSK                                       0x1000
#define HWIO_GCC_USB30_SEC_MASTER_CBCR_FORCE_MEM_PERIPH_OFF_SHFT                                          0xc
#define HWIO_GCC_USB30_SEC_MASTER_CBCR_FORCE_MEM_PERIPH_OFF_FORCE_DISABLE_FVAL                            0x0
#define HWIO_GCC_USB30_SEC_MASTER_CBCR_FORCE_MEM_PERIPH_OFF_FORCE_ENABLE_FVAL                             0x1
#define HWIO_GCC_USB30_SEC_MASTER_CBCR_WAKEUP_BMSK                                                      0xf00
#define HWIO_GCC_USB30_SEC_MASTER_CBCR_WAKEUP_SHFT                                                        0x8
#define HWIO_GCC_USB30_SEC_MASTER_CBCR_WAKEUP_CLOCK0_FVAL                                                 0x0
#define HWIO_GCC_USB30_SEC_MASTER_CBCR_WAKEUP_CLOCK1_FVAL                                                 0x1
#define HWIO_GCC_USB30_SEC_MASTER_CBCR_WAKEUP_CLOCK2_FVAL                                                 0x2
#define HWIO_GCC_USB30_SEC_MASTER_CBCR_WAKEUP_CLOCK3_FVAL                                                 0x3
#define HWIO_GCC_USB30_SEC_MASTER_CBCR_WAKEUP_CLOCK4_FVAL                                                 0x4
#define HWIO_GCC_USB30_SEC_MASTER_CBCR_WAKEUP_CLOCK5_FVAL                                                 0x5
#define HWIO_GCC_USB30_SEC_MASTER_CBCR_WAKEUP_CLOCK6_FVAL                                                 0x6
#define HWIO_GCC_USB30_SEC_MASTER_CBCR_WAKEUP_CLOCK7_FVAL                                                 0x7
#define HWIO_GCC_USB30_SEC_MASTER_CBCR_WAKEUP_CLOCK8_FVAL                                                 0x8
#define HWIO_GCC_USB30_SEC_MASTER_CBCR_WAKEUP_CLOCK9_FVAL                                                 0x9
#define HWIO_GCC_USB30_SEC_MASTER_CBCR_WAKEUP_CLOCK10_FVAL                                                0xa
#define HWIO_GCC_USB30_SEC_MASTER_CBCR_WAKEUP_CLOCK11_FVAL                                                0xb
#define HWIO_GCC_USB30_SEC_MASTER_CBCR_WAKEUP_CLOCK12_FVAL                                                0xc
#define HWIO_GCC_USB30_SEC_MASTER_CBCR_WAKEUP_CLOCK13_FVAL                                                0xd
#define HWIO_GCC_USB30_SEC_MASTER_CBCR_WAKEUP_CLOCK14_FVAL                                                0xe
#define HWIO_GCC_USB30_SEC_MASTER_CBCR_WAKEUP_CLOCK15_FVAL                                                0xf
#define HWIO_GCC_USB30_SEC_MASTER_CBCR_SLEEP_BMSK                                                        0xf0
#define HWIO_GCC_USB30_SEC_MASTER_CBCR_SLEEP_SHFT                                                         0x4
#define HWIO_GCC_USB30_SEC_MASTER_CBCR_SLEEP_CLOCK0_FVAL                                                  0x0
#define HWIO_GCC_USB30_SEC_MASTER_CBCR_SLEEP_CLOCK1_FVAL                                                  0x1
#define HWIO_GCC_USB30_SEC_MASTER_CBCR_SLEEP_CLOCK2_FVAL                                                  0x2
#define HWIO_GCC_USB30_SEC_MASTER_CBCR_SLEEP_CLOCK3_FVAL                                                  0x3
#define HWIO_GCC_USB30_SEC_MASTER_CBCR_SLEEP_CLOCK4_FVAL                                                  0x4
#define HWIO_GCC_USB30_SEC_MASTER_CBCR_SLEEP_CLOCK5_FVAL                                                  0x5
#define HWIO_GCC_USB30_SEC_MASTER_CBCR_SLEEP_CLOCK6_FVAL                                                  0x6
#define HWIO_GCC_USB30_SEC_MASTER_CBCR_SLEEP_CLOCK7_FVAL                                                  0x7
#define HWIO_GCC_USB30_SEC_MASTER_CBCR_SLEEP_CLOCK8_FVAL                                                  0x8
#define HWIO_GCC_USB30_SEC_MASTER_CBCR_SLEEP_CLOCK9_FVAL                                                  0x9
#define HWIO_GCC_USB30_SEC_MASTER_CBCR_SLEEP_CLOCK10_FVAL                                                 0xa
#define HWIO_GCC_USB30_SEC_MASTER_CBCR_SLEEP_CLOCK11_FVAL                                                 0xb
#define HWIO_GCC_USB30_SEC_MASTER_CBCR_SLEEP_CLOCK12_FVAL                                                 0xc
#define HWIO_GCC_USB30_SEC_MASTER_CBCR_SLEEP_CLOCK13_FVAL                                                 0xd
#define HWIO_GCC_USB30_SEC_MASTER_CBCR_SLEEP_CLOCK14_FVAL                                                 0xe
#define HWIO_GCC_USB30_SEC_MASTER_CBCR_SLEEP_CLOCK15_FVAL                                                 0xf
#define HWIO_GCC_USB30_SEC_MASTER_CBCR_CLK_ARES_BMSK                                                      0x4
#define HWIO_GCC_USB30_SEC_MASTER_CBCR_CLK_ARES_SHFT                                                      0x2
#define HWIO_GCC_USB30_SEC_MASTER_CBCR_CLK_ARES_NO_RESET_FVAL                                             0x0
#define HWIO_GCC_USB30_SEC_MASTER_CBCR_CLK_ARES_RESET_FVAL                                                0x1
#define HWIO_GCC_USB30_SEC_MASTER_CBCR_CLK_ENABLE_BMSK                                                    0x1
#define HWIO_GCC_USB30_SEC_MASTER_CBCR_CLK_ENABLE_SHFT                                                    0x0
#define HWIO_GCC_USB30_SEC_MASTER_CBCR_CLK_ENABLE_DISABLE_FVAL                                            0x0
#define HWIO_GCC_USB30_SEC_MASTER_CBCR_CLK_ENABLE_ENABLE_FVAL                                             0x1

#define HWIO_GCC_USB30_SEC_SLEEP_CBCR_ADDR                                                         (GCC_CLK_CTL_REG_REG_BASE      + 0x00010014)
#define HWIO_GCC_USB30_SEC_SLEEP_CBCR_PHYS                                                         (GCC_CLK_CTL_REG_REG_BASE_PHYS + 0x00010014)
#define HWIO_GCC_USB30_SEC_SLEEP_CBCR_OFFS                                                         (GCC_CLK_CTL_REG_REG_BASE_OFFS + 0x00010014)
#define HWIO_GCC_USB30_SEC_SLEEP_CBCR_RMSK                                                         0x80000005
#define HWIO_GCC_USB30_SEC_SLEEP_CBCR_IN          \
        in_dword(HWIO_GCC_USB30_SEC_SLEEP_CBCR_ADDR)
#define HWIO_GCC_USB30_SEC_SLEEP_CBCR_INM(m)      \
        in_dword_masked(HWIO_GCC_USB30_SEC_SLEEP_CBCR_ADDR, m)
#define HWIO_GCC_USB30_SEC_SLEEP_CBCR_OUT(v)      \
        out_dword(HWIO_GCC_USB30_SEC_SLEEP_CBCR_ADDR,v)
#define HWIO_GCC_USB30_SEC_SLEEP_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GCC_USB30_SEC_SLEEP_CBCR_ADDR,m,v,HWIO_GCC_USB30_SEC_SLEEP_CBCR_IN)
#define HWIO_GCC_USB30_SEC_SLEEP_CBCR_CLK_OFF_BMSK                                                 0x80000000
#define HWIO_GCC_USB30_SEC_SLEEP_CBCR_CLK_OFF_SHFT                                                       0x1f
#define HWIO_GCC_USB30_SEC_SLEEP_CBCR_CLK_ARES_BMSK                                                       0x4
#define HWIO_GCC_USB30_SEC_SLEEP_CBCR_CLK_ARES_SHFT                                                       0x2
#define HWIO_GCC_USB30_SEC_SLEEP_CBCR_CLK_ARES_NO_RESET_FVAL                                              0x0
#define HWIO_GCC_USB30_SEC_SLEEP_CBCR_CLK_ARES_RESET_FVAL                                                 0x1
#define HWIO_GCC_USB30_SEC_SLEEP_CBCR_CLK_ENABLE_BMSK                                                     0x1
#define HWIO_GCC_USB30_SEC_SLEEP_CBCR_CLK_ENABLE_SHFT                                                     0x0
#define HWIO_GCC_USB30_SEC_SLEEP_CBCR_CLK_ENABLE_DISABLE_FVAL                                             0x0
#define HWIO_GCC_USB30_SEC_SLEEP_CBCR_CLK_ENABLE_ENABLE_FVAL                                              0x1

#define HWIO_GCC_USB30_SEC_MOCK_UTMI_CBCR_ADDR                                                     (GCC_CLK_CTL_REG_REG_BASE      + 0x00010018)
#define HWIO_GCC_USB30_SEC_MOCK_UTMI_CBCR_PHYS                                                     (GCC_CLK_CTL_REG_REG_BASE_PHYS + 0x00010018)
#define HWIO_GCC_USB30_SEC_MOCK_UTMI_CBCR_OFFS                                                     (GCC_CLK_CTL_REG_REG_BASE_OFFS + 0x00010018)
#define HWIO_GCC_USB30_SEC_MOCK_UTMI_CBCR_RMSK                                                     0x80000005
#define HWIO_GCC_USB30_SEC_MOCK_UTMI_CBCR_IN          \
        in_dword(HWIO_GCC_USB30_SEC_MOCK_UTMI_CBCR_ADDR)
#define HWIO_GCC_USB30_SEC_MOCK_UTMI_CBCR_INM(m)      \
        in_dword_masked(HWIO_GCC_USB30_SEC_MOCK_UTMI_CBCR_ADDR, m)
#define HWIO_GCC_USB30_SEC_MOCK_UTMI_CBCR_OUT(v)      \
        out_dword(HWIO_GCC_USB30_SEC_MOCK_UTMI_CBCR_ADDR,v)
#define HWIO_GCC_USB30_SEC_MOCK_UTMI_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GCC_USB30_SEC_MOCK_UTMI_CBCR_ADDR,m,v,HWIO_GCC_USB30_SEC_MOCK_UTMI_CBCR_IN)
#define HWIO_GCC_USB30_SEC_MOCK_UTMI_CBCR_CLK_OFF_BMSK                                             0x80000000
#define HWIO_GCC_USB30_SEC_MOCK_UTMI_CBCR_CLK_OFF_SHFT                                                   0x1f
#define HWIO_GCC_USB30_SEC_MOCK_UTMI_CBCR_CLK_ARES_BMSK                                                   0x4
#define HWIO_GCC_USB30_SEC_MOCK_UTMI_CBCR_CLK_ARES_SHFT                                                   0x2
#define HWIO_GCC_USB30_SEC_MOCK_UTMI_CBCR_CLK_ARES_NO_RESET_FVAL                                          0x0
#define HWIO_GCC_USB30_SEC_MOCK_UTMI_CBCR_CLK_ARES_RESET_FVAL                                             0x1
#define HWIO_GCC_USB30_SEC_MOCK_UTMI_CBCR_CLK_ENABLE_BMSK                                                 0x1
#define HWIO_GCC_USB30_SEC_MOCK_UTMI_CBCR_CLK_ENABLE_SHFT                                                 0x0
#define HWIO_GCC_USB30_SEC_MOCK_UTMI_CBCR_CLK_ENABLE_DISABLE_FVAL                                         0x0
#define HWIO_GCC_USB30_SEC_MOCK_UTMI_CBCR_CLK_ENABLE_ENABLE_FVAL                                          0x1

#define HWIO_GCC_USB30_SEC_MASTER_CMD_RCGR_ADDR                                                    (GCC_CLK_CTL_REG_REG_BASE      + 0x0001001c)
#define HWIO_GCC_USB30_SEC_MASTER_CMD_RCGR_PHYS                                                    (GCC_CLK_CTL_REG_REG_BASE_PHYS + 0x0001001c)
#define HWIO_GCC_USB30_SEC_MASTER_CMD_RCGR_OFFS                                                    (GCC_CLK_CTL_REG_REG_BASE_OFFS + 0x0001001c)
#define HWIO_GCC_USB30_SEC_MASTER_CMD_RCGR_RMSK                                                    0x800000f3
#define HWIO_GCC_USB30_SEC_MASTER_CMD_RCGR_IN          \
        in_dword(HWIO_GCC_USB30_SEC_MASTER_CMD_RCGR_ADDR)
#define HWIO_GCC_USB30_SEC_MASTER_CMD_RCGR_INM(m)      \
        in_dword_masked(HWIO_GCC_USB30_SEC_MASTER_CMD_RCGR_ADDR, m)
#define HWIO_GCC_USB30_SEC_MASTER_CMD_RCGR_OUT(v)      \
        out_dword(HWIO_GCC_USB30_SEC_MASTER_CMD_RCGR_ADDR,v)
#define HWIO_GCC_USB30_SEC_MASTER_CMD_RCGR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GCC_USB30_SEC_MASTER_CMD_RCGR_ADDR,m,v,HWIO_GCC_USB30_SEC_MASTER_CMD_RCGR_IN)
#define HWIO_GCC_USB30_SEC_MASTER_CMD_RCGR_ROOT_OFF_BMSK                                           0x80000000
#define HWIO_GCC_USB30_SEC_MASTER_CMD_RCGR_ROOT_OFF_SHFT                                                 0x1f
#define HWIO_GCC_USB30_SEC_MASTER_CMD_RCGR_DIRTY_D_BMSK                                                  0x80
#define HWIO_GCC_USB30_SEC_MASTER_CMD_RCGR_DIRTY_D_SHFT                                                   0x7
#define HWIO_GCC_USB30_SEC_MASTER_CMD_RCGR_DIRTY_N_BMSK                                                  0x40
#define HWIO_GCC_USB30_SEC_MASTER_CMD_RCGR_DIRTY_N_SHFT                                                   0x6
#define HWIO_GCC_USB30_SEC_MASTER_CMD_RCGR_DIRTY_M_BMSK                                                  0x20
#define HWIO_GCC_USB30_SEC_MASTER_CMD_RCGR_DIRTY_M_SHFT                                                   0x5
#define HWIO_GCC_USB30_SEC_MASTER_CMD_RCGR_DIRTY_CFG_RCGR_BMSK                                           0x10
#define HWIO_GCC_USB30_SEC_MASTER_CMD_RCGR_DIRTY_CFG_RCGR_SHFT                                            0x4
#define HWIO_GCC_USB30_SEC_MASTER_CMD_RCGR_ROOT_EN_BMSK                                                   0x2
#define HWIO_GCC_USB30_SEC_MASTER_CMD_RCGR_ROOT_EN_SHFT                                                   0x1
#define HWIO_GCC_USB30_SEC_MASTER_CMD_RCGR_ROOT_EN_DISABLE_FVAL                                           0x0
#define HWIO_GCC_USB30_SEC_MASTER_CMD_RCGR_ROOT_EN_ENABLE_FVAL                                            0x1
#define HWIO_GCC_USB30_SEC_MASTER_CMD_RCGR_UPDATE_BMSK                                                    0x1
#define HWIO_GCC_USB30_SEC_MASTER_CMD_RCGR_UPDATE_SHFT                                                    0x0
#define HWIO_GCC_USB30_SEC_MASTER_CMD_RCGR_UPDATE_DISABLE_FVAL                                            0x0
#define HWIO_GCC_USB30_SEC_MASTER_CMD_RCGR_UPDATE_ENABLE_FVAL                                             0x1

#define HWIO_GCC_USB30_SEC_MASTER_CFG_RCGR_ADDR                                                    (GCC_CLK_CTL_REG_REG_BASE      + 0x00010020)
#define HWIO_GCC_USB30_SEC_MASTER_CFG_RCGR_PHYS                                                    (GCC_CLK_CTL_REG_REG_BASE_PHYS + 0x00010020)
#define HWIO_GCC_USB30_SEC_MASTER_CFG_RCGR_OFFS                                                    (GCC_CLK_CTL_REG_REG_BASE_OFFS + 0x00010020)
#define HWIO_GCC_USB30_SEC_MASTER_CFG_RCGR_RMSK                                                      0x10371f
#define HWIO_GCC_USB30_SEC_MASTER_CFG_RCGR_IN          \
        in_dword(HWIO_GCC_USB30_SEC_MASTER_CFG_RCGR_ADDR)
#define HWIO_GCC_USB30_SEC_MASTER_CFG_RCGR_INM(m)      \
        in_dword_masked(HWIO_GCC_USB30_SEC_MASTER_CFG_RCGR_ADDR, m)
#define HWIO_GCC_USB30_SEC_MASTER_CFG_RCGR_OUT(v)      \
        out_dword(HWIO_GCC_USB30_SEC_MASTER_CFG_RCGR_ADDR,v)
#define HWIO_GCC_USB30_SEC_MASTER_CFG_RCGR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GCC_USB30_SEC_MASTER_CFG_RCGR_ADDR,m,v,HWIO_GCC_USB30_SEC_MASTER_CFG_RCGR_IN)
#define HWIO_GCC_USB30_SEC_MASTER_CFG_RCGR_HW_CLK_CONTROL_BMSK                                       0x100000
#define HWIO_GCC_USB30_SEC_MASTER_CFG_RCGR_HW_CLK_CONTROL_SHFT                                           0x14
#define HWIO_GCC_USB30_SEC_MASTER_CFG_RCGR_HW_CLK_CONTROL_DISABLE_FVAL                                    0x0
#define HWIO_GCC_USB30_SEC_MASTER_CFG_RCGR_HW_CLK_CONTROL_ENABLE_FVAL                                     0x1
#define HWIO_GCC_USB30_SEC_MASTER_CFG_RCGR_MODE_BMSK                                                   0x3000
#define HWIO_GCC_USB30_SEC_MASTER_CFG_RCGR_MODE_SHFT                                                      0xc
#define HWIO_GCC_USB30_SEC_MASTER_CFG_RCGR_MODE_BYPASS_FVAL                                               0x0
#define HWIO_GCC_USB30_SEC_MASTER_CFG_RCGR_MODE_SWALLOW_FVAL                                              0x1
#define HWIO_GCC_USB30_SEC_MASTER_CFG_RCGR_MODE_DUAL_EDGE_FVAL                                            0x2
#define HWIO_GCC_USB30_SEC_MASTER_CFG_RCGR_MODE_SINGLE_EDGE_FVAL                                          0x3
#define HWIO_GCC_USB30_SEC_MASTER_CFG_RCGR_SRC_SEL_BMSK                                                 0x700
#define HWIO_GCC_USB30_SEC_MASTER_CFG_RCGR_SRC_SEL_SHFT                                                   0x8
#define HWIO_GCC_USB30_SEC_MASTER_CFG_RCGR_SRC_SEL_SRC0_FVAL                                              0x0
#define HWIO_GCC_USB30_SEC_MASTER_CFG_RCGR_SRC_SEL_SRC1_FVAL                                              0x1
#define HWIO_GCC_USB30_SEC_MASTER_CFG_RCGR_SRC_SEL_SRC2_FVAL                                              0x2
#define HWIO_GCC_USB30_SEC_MASTER_CFG_RCGR_SRC_SEL_SRC3_FVAL                                              0x3
#define HWIO_GCC_USB30_SEC_MASTER_CFG_RCGR_SRC_SEL_SRC4_FVAL                                              0x4
#define HWIO_GCC_USB30_SEC_MASTER_CFG_RCGR_SRC_SEL_SRC5_FVAL                                              0x5
#define HWIO_GCC_USB30_SEC_MASTER_CFG_RCGR_SRC_SEL_SRC6_FVAL                                              0x6
#define HWIO_GCC_USB30_SEC_MASTER_CFG_RCGR_SRC_SEL_SRC7_FVAL                                              0x7
#define HWIO_GCC_USB30_SEC_MASTER_CFG_RCGR_SRC_DIV_BMSK                                                  0x1f
#define HWIO_GCC_USB30_SEC_MASTER_CFG_RCGR_SRC_DIV_SHFT                                                   0x0
#define HWIO_GCC_USB30_SEC_MASTER_CFG_RCGR_SRC_DIV_BYPASS_FVAL                                            0x0
#define HWIO_GCC_USB30_SEC_MASTER_CFG_RCGR_SRC_DIV_DIV1_FVAL                                              0x1
#define HWIO_GCC_USB30_SEC_MASTER_CFG_RCGR_SRC_DIV_DIV1_5_FVAL                                            0x2
#define HWIO_GCC_USB30_SEC_MASTER_CFG_RCGR_SRC_DIV_DIV2_FVAL                                              0x3
#define HWIO_GCC_USB30_SEC_MASTER_CFG_RCGR_SRC_DIV_DIV2_5_FVAL                                            0x4
#define HWIO_GCC_USB30_SEC_MASTER_CFG_RCGR_SRC_DIV_DIV3_FVAL                                              0x5
#define HWIO_GCC_USB30_SEC_MASTER_CFG_RCGR_SRC_DIV_DIV3_5_FVAL                                            0x6
#define HWIO_GCC_USB30_SEC_MASTER_CFG_RCGR_SRC_DIV_DIV4_FVAL                                              0x7
#define HWIO_GCC_USB30_SEC_MASTER_CFG_RCGR_SRC_DIV_DIV4_5_FVAL                                            0x8
#define HWIO_GCC_USB30_SEC_MASTER_CFG_RCGR_SRC_DIV_DIV5_FVAL                                              0x9
#define HWIO_GCC_USB30_SEC_MASTER_CFG_RCGR_SRC_DIV_DIV5_5_FVAL                                            0xa
#define HWIO_GCC_USB30_SEC_MASTER_CFG_RCGR_SRC_DIV_DIV6_FVAL                                              0xb
#define HWIO_GCC_USB30_SEC_MASTER_CFG_RCGR_SRC_DIV_DIV6_5_FVAL                                            0xc
#define HWIO_GCC_USB30_SEC_MASTER_CFG_RCGR_SRC_DIV_DIV7_FVAL                                              0xd
#define HWIO_GCC_USB30_SEC_MASTER_CFG_RCGR_SRC_DIV_DIV7_5_FVAL                                            0xe
#define HWIO_GCC_USB30_SEC_MASTER_CFG_RCGR_SRC_DIV_DIV8_FVAL                                              0xf
#define HWIO_GCC_USB30_SEC_MASTER_CFG_RCGR_SRC_DIV_DIV8_5_FVAL                                           0x10
#define HWIO_GCC_USB30_SEC_MASTER_CFG_RCGR_SRC_DIV_DIV9_FVAL                                             0x11
#define HWIO_GCC_USB30_SEC_MASTER_CFG_RCGR_SRC_DIV_DIV9_5_FVAL                                           0x12
#define HWIO_GCC_USB30_SEC_MASTER_CFG_RCGR_SRC_DIV_DIV10_FVAL                                            0x13
#define HWIO_GCC_USB30_SEC_MASTER_CFG_RCGR_SRC_DIV_DIV10_5_FVAL                                          0x14
#define HWIO_GCC_USB30_SEC_MASTER_CFG_RCGR_SRC_DIV_DIV11_FVAL                                            0x15
#define HWIO_GCC_USB30_SEC_MASTER_CFG_RCGR_SRC_DIV_DIV11_5_FVAL                                          0x16
#define HWIO_GCC_USB30_SEC_MASTER_CFG_RCGR_SRC_DIV_DIV12_FVAL                                            0x17
#define HWIO_GCC_USB30_SEC_MASTER_CFG_RCGR_SRC_DIV_DIV12_5_FVAL                                          0x18
#define HWIO_GCC_USB30_SEC_MASTER_CFG_RCGR_SRC_DIV_DIV13_FVAL                                            0x19
#define HWIO_GCC_USB30_SEC_MASTER_CFG_RCGR_SRC_DIV_DIV13_5_FVAL                                          0x1a
#define HWIO_GCC_USB30_SEC_MASTER_CFG_RCGR_SRC_DIV_DIV14_FVAL                                            0x1b
#define HWIO_GCC_USB30_SEC_MASTER_CFG_RCGR_SRC_DIV_DIV14_5_FVAL                                          0x1c
#define HWIO_GCC_USB30_SEC_MASTER_CFG_RCGR_SRC_DIV_DIV15_FVAL                                            0x1d
#define HWIO_GCC_USB30_SEC_MASTER_CFG_RCGR_SRC_DIV_DIV15_5_FVAL                                          0x1e
#define HWIO_GCC_USB30_SEC_MASTER_CFG_RCGR_SRC_DIV_DIV16_FVAL                                            0x1f

#define HWIO_GCC_USB30_SEC_MASTER_M_ADDR                                                           (GCC_CLK_CTL_REG_REG_BASE      + 0x00010024)
#define HWIO_GCC_USB30_SEC_MASTER_M_PHYS                                                           (GCC_CLK_CTL_REG_REG_BASE_PHYS + 0x00010024)
#define HWIO_GCC_USB30_SEC_MASTER_M_OFFS                                                           (GCC_CLK_CTL_REG_REG_BASE_OFFS + 0x00010024)
#define HWIO_GCC_USB30_SEC_MASTER_M_RMSK                                                                 0xff
#define HWIO_GCC_USB30_SEC_MASTER_M_IN          \
        in_dword(HWIO_GCC_USB30_SEC_MASTER_M_ADDR)
#define HWIO_GCC_USB30_SEC_MASTER_M_INM(m)      \
        in_dword_masked(HWIO_GCC_USB30_SEC_MASTER_M_ADDR, m)
#define HWIO_GCC_USB30_SEC_MASTER_M_OUT(v)      \
        out_dword(HWIO_GCC_USB30_SEC_MASTER_M_ADDR,v)
#define HWIO_GCC_USB30_SEC_MASTER_M_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GCC_USB30_SEC_MASTER_M_ADDR,m,v,HWIO_GCC_USB30_SEC_MASTER_M_IN)
#define HWIO_GCC_USB30_SEC_MASTER_M_M_BMSK                                                               0xff
#define HWIO_GCC_USB30_SEC_MASTER_M_M_SHFT                                                                0x0

#define HWIO_GCC_USB30_SEC_MASTER_N_ADDR                                                           (GCC_CLK_CTL_REG_REG_BASE      + 0x00010028)
#define HWIO_GCC_USB30_SEC_MASTER_N_PHYS                                                           (GCC_CLK_CTL_REG_REG_BASE_PHYS + 0x00010028)
#define HWIO_GCC_USB30_SEC_MASTER_N_OFFS                                                           (GCC_CLK_CTL_REG_REG_BASE_OFFS + 0x00010028)
#define HWIO_GCC_USB30_SEC_MASTER_N_RMSK                                                                 0xff
#define HWIO_GCC_USB30_SEC_MASTER_N_IN          \
        in_dword(HWIO_GCC_USB30_SEC_MASTER_N_ADDR)
#define HWIO_GCC_USB30_SEC_MASTER_N_INM(m)      \
        in_dword_masked(HWIO_GCC_USB30_SEC_MASTER_N_ADDR, m)
#define HWIO_GCC_USB30_SEC_MASTER_N_OUT(v)      \
        out_dword(HWIO_GCC_USB30_SEC_MASTER_N_ADDR,v)
#define HWIO_GCC_USB30_SEC_MASTER_N_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GCC_USB30_SEC_MASTER_N_ADDR,m,v,HWIO_GCC_USB30_SEC_MASTER_N_IN)
#define HWIO_GCC_USB30_SEC_MASTER_N_NOT_N_MINUS_M_BMSK                                                   0xff
#define HWIO_GCC_USB30_SEC_MASTER_N_NOT_N_MINUS_M_SHFT                                                    0x0

#define HWIO_GCC_USB30_SEC_MASTER_D_ADDR                                                           (GCC_CLK_CTL_REG_REG_BASE      + 0x0001002c)
#define HWIO_GCC_USB30_SEC_MASTER_D_PHYS                                                           (GCC_CLK_CTL_REG_REG_BASE_PHYS + 0x0001002c)
#define HWIO_GCC_USB30_SEC_MASTER_D_OFFS                                                           (GCC_CLK_CTL_REG_REG_BASE_OFFS + 0x0001002c)
#define HWIO_GCC_USB30_SEC_MASTER_D_RMSK                                                                 0xff
#define HWIO_GCC_USB30_SEC_MASTER_D_IN          \
        in_dword(HWIO_GCC_USB30_SEC_MASTER_D_ADDR)
#define HWIO_GCC_USB30_SEC_MASTER_D_INM(m)      \
        in_dword_masked(HWIO_GCC_USB30_SEC_MASTER_D_ADDR, m)
#define HWIO_GCC_USB30_SEC_MASTER_D_OUT(v)      \
        out_dword(HWIO_GCC_USB30_SEC_MASTER_D_ADDR,v)
#define HWIO_GCC_USB30_SEC_MASTER_D_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GCC_USB30_SEC_MASTER_D_ADDR,m,v,HWIO_GCC_USB30_SEC_MASTER_D_IN)
#define HWIO_GCC_USB30_SEC_MASTER_D_NOT_2D_BMSK                                                          0xff
#define HWIO_GCC_USB30_SEC_MASTER_D_NOT_2D_SHFT                                                           0x0

#define HWIO_GCC_USB30_SEC_MOCK_UTMI_CMD_RCGR_ADDR                                                 (GCC_CLK_CTL_REG_REG_BASE      + 0x00010034)
#define HWIO_GCC_USB30_SEC_MOCK_UTMI_CMD_RCGR_PHYS                                                 (GCC_CLK_CTL_REG_REG_BASE_PHYS + 0x00010034)
#define HWIO_GCC_USB30_SEC_MOCK_UTMI_CMD_RCGR_OFFS                                                 (GCC_CLK_CTL_REG_REG_BASE_OFFS + 0x00010034)
#define HWIO_GCC_USB30_SEC_MOCK_UTMI_CMD_RCGR_RMSK                                                 0x80000013
#define HWIO_GCC_USB30_SEC_MOCK_UTMI_CMD_RCGR_IN          \
        in_dword(HWIO_GCC_USB30_SEC_MOCK_UTMI_CMD_RCGR_ADDR)
#define HWIO_GCC_USB30_SEC_MOCK_UTMI_CMD_RCGR_INM(m)      \
        in_dword_masked(HWIO_GCC_USB30_SEC_MOCK_UTMI_CMD_RCGR_ADDR, m)
#define HWIO_GCC_USB30_SEC_MOCK_UTMI_CMD_RCGR_OUT(v)      \
        out_dword(HWIO_GCC_USB30_SEC_MOCK_UTMI_CMD_RCGR_ADDR,v)
#define HWIO_GCC_USB30_SEC_MOCK_UTMI_CMD_RCGR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GCC_USB30_SEC_MOCK_UTMI_CMD_RCGR_ADDR,m,v,HWIO_GCC_USB30_SEC_MOCK_UTMI_CMD_RCGR_IN)
#define HWIO_GCC_USB30_SEC_MOCK_UTMI_CMD_RCGR_ROOT_OFF_BMSK                                        0x80000000
#define HWIO_GCC_USB30_SEC_MOCK_UTMI_CMD_RCGR_ROOT_OFF_SHFT                                              0x1f
#define HWIO_GCC_USB30_SEC_MOCK_UTMI_CMD_RCGR_DIRTY_CFG_RCGR_BMSK                                        0x10
#define HWIO_GCC_USB30_SEC_MOCK_UTMI_CMD_RCGR_DIRTY_CFG_RCGR_SHFT                                         0x4
#define HWIO_GCC_USB30_SEC_MOCK_UTMI_CMD_RCGR_ROOT_EN_BMSK                                                0x2
#define HWIO_GCC_USB30_SEC_MOCK_UTMI_CMD_RCGR_ROOT_EN_SHFT                                                0x1
#define HWIO_GCC_USB30_SEC_MOCK_UTMI_CMD_RCGR_ROOT_EN_DISABLE_FVAL                                        0x0
#define HWIO_GCC_USB30_SEC_MOCK_UTMI_CMD_RCGR_ROOT_EN_ENABLE_FVAL                                         0x1
#define HWIO_GCC_USB30_SEC_MOCK_UTMI_CMD_RCGR_UPDATE_BMSK                                                 0x1
#define HWIO_GCC_USB30_SEC_MOCK_UTMI_CMD_RCGR_UPDATE_SHFT                                                 0x0
#define HWIO_GCC_USB30_SEC_MOCK_UTMI_CMD_RCGR_UPDATE_DISABLE_FVAL                                         0x0
#define HWIO_GCC_USB30_SEC_MOCK_UTMI_CMD_RCGR_UPDATE_ENABLE_FVAL                                          0x1

#define HWIO_GCC_USB30_SEC_MOCK_UTMI_CFG_RCGR_ADDR                                                 (GCC_CLK_CTL_REG_REG_BASE      + 0x00010038)
#define HWIO_GCC_USB30_SEC_MOCK_UTMI_CFG_RCGR_PHYS                                                 (GCC_CLK_CTL_REG_REG_BASE_PHYS + 0x00010038)
#define HWIO_GCC_USB30_SEC_MOCK_UTMI_CFG_RCGR_OFFS                                                 (GCC_CLK_CTL_REG_REG_BASE_OFFS + 0x00010038)
#define HWIO_GCC_USB30_SEC_MOCK_UTMI_CFG_RCGR_RMSK                                                   0x11071f
#define HWIO_GCC_USB30_SEC_MOCK_UTMI_CFG_RCGR_IN          \
        in_dword(HWIO_GCC_USB30_SEC_MOCK_UTMI_CFG_RCGR_ADDR)
#define HWIO_GCC_USB30_SEC_MOCK_UTMI_CFG_RCGR_INM(m)      \
        in_dword_masked(HWIO_GCC_USB30_SEC_MOCK_UTMI_CFG_RCGR_ADDR, m)
#define HWIO_GCC_USB30_SEC_MOCK_UTMI_CFG_RCGR_OUT(v)      \
        out_dword(HWIO_GCC_USB30_SEC_MOCK_UTMI_CFG_RCGR_ADDR,v)
#define HWIO_GCC_USB30_SEC_MOCK_UTMI_CFG_RCGR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GCC_USB30_SEC_MOCK_UTMI_CFG_RCGR_ADDR,m,v,HWIO_GCC_USB30_SEC_MOCK_UTMI_CFG_RCGR_IN)
#define HWIO_GCC_USB30_SEC_MOCK_UTMI_CFG_RCGR_HW_CLK_CONTROL_BMSK                                    0x100000
#define HWIO_GCC_USB30_SEC_MOCK_UTMI_CFG_RCGR_HW_CLK_CONTROL_SHFT                                        0x14
#define HWIO_GCC_USB30_SEC_MOCK_UTMI_CFG_RCGR_HW_CLK_CONTROL_DISABLE_FVAL                                 0x0
#define HWIO_GCC_USB30_SEC_MOCK_UTMI_CFG_RCGR_HW_CLK_CONTROL_ENABLE_FVAL                                  0x1
#define HWIO_GCC_USB30_SEC_MOCK_UTMI_CFG_RCGR_RCGLITE_DISABLE_BMSK                                    0x10000
#define HWIO_GCC_USB30_SEC_MOCK_UTMI_CFG_RCGR_RCGLITE_DISABLE_SHFT                                       0x10
#define HWIO_GCC_USB30_SEC_MOCK_UTMI_CFG_RCGR_RCGLITE_DISABLE_RCGLITE_ENABLED_FVAL                        0x0
#define HWIO_GCC_USB30_SEC_MOCK_UTMI_CFG_RCGR_RCGLITE_DISABLE_RCGLITE_DISABLED_FVAL                       0x1
#define HWIO_GCC_USB30_SEC_MOCK_UTMI_CFG_RCGR_SRC_SEL_BMSK                                              0x700
#define HWIO_GCC_USB30_SEC_MOCK_UTMI_CFG_RCGR_SRC_SEL_SHFT                                                0x8
#define HWIO_GCC_USB30_SEC_MOCK_UTMI_CFG_RCGR_SRC_SEL_SRC0_FVAL                                           0x0
#define HWIO_GCC_USB30_SEC_MOCK_UTMI_CFG_RCGR_SRC_SEL_SRC1_FVAL                                           0x1
#define HWIO_GCC_USB30_SEC_MOCK_UTMI_CFG_RCGR_SRC_SEL_SRC2_FVAL                                           0x2
#define HWIO_GCC_USB30_SEC_MOCK_UTMI_CFG_RCGR_SRC_SEL_SRC3_FVAL                                           0x3
#define HWIO_GCC_USB30_SEC_MOCK_UTMI_CFG_RCGR_SRC_SEL_SRC4_FVAL                                           0x4
#define HWIO_GCC_USB30_SEC_MOCK_UTMI_CFG_RCGR_SRC_SEL_SRC5_FVAL                                           0x5
#define HWIO_GCC_USB30_SEC_MOCK_UTMI_CFG_RCGR_SRC_SEL_SRC6_FVAL                                           0x6
#define HWIO_GCC_USB30_SEC_MOCK_UTMI_CFG_RCGR_SRC_SEL_SRC7_FVAL                                           0x7
#define HWIO_GCC_USB30_SEC_MOCK_UTMI_CFG_RCGR_SRC_DIV_BMSK                                               0x1f
#define HWIO_GCC_USB30_SEC_MOCK_UTMI_CFG_RCGR_SRC_DIV_SHFT                                                0x0
#define HWIO_GCC_USB30_SEC_MOCK_UTMI_CFG_RCGR_SRC_DIV_BYPASS_FVAL                                         0x0
#define HWIO_GCC_USB30_SEC_MOCK_UTMI_CFG_RCGR_SRC_DIV_DIV1_FVAL                                           0x1
#define HWIO_GCC_USB30_SEC_MOCK_UTMI_CFG_RCGR_SRC_DIV_DIV1_5_FVAL                                         0x2
#define HWIO_GCC_USB30_SEC_MOCK_UTMI_CFG_RCGR_SRC_DIV_DIV2_FVAL                                           0x3
#define HWIO_GCC_USB30_SEC_MOCK_UTMI_CFG_RCGR_SRC_DIV_DIV2_5_FVAL                                         0x4
#define HWIO_GCC_USB30_SEC_MOCK_UTMI_CFG_RCGR_SRC_DIV_DIV3_FVAL                                           0x5
#define HWIO_GCC_USB30_SEC_MOCK_UTMI_CFG_RCGR_SRC_DIV_DIV3_5_FVAL                                         0x6
#define HWIO_GCC_USB30_SEC_MOCK_UTMI_CFG_RCGR_SRC_DIV_DIV4_FVAL                                           0x7
#define HWIO_GCC_USB30_SEC_MOCK_UTMI_CFG_RCGR_SRC_DIV_DIV4_5_FVAL                                         0x8
#define HWIO_GCC_USB30_SEC_MOCK_UTMI_CFG_RCGR_SRC_DIV_DIV5_FVAL                                           0x9
#define HWIO_GCC_USB30_SEC_MOCK_UTMI_CFG_RCGR_SRC_DIV_DIV5_5_FVAL                                         0xa
#define HWIO_GCC_USB30_SEC_MOCK_UTMI_CFG_RCGR_SRC_DIV_DIV6_FVAL                                           0xb
#define HWIO_GCC_USB30_SEC_MOCK_UTMI_CFG_RCGR_SRC_DIV_DIV6_5_FVAL                                         0xc
#define HWIO_GCC_USB30_SEC_MOCK_UTMI_CFG_RCGR_SRC_DIV_DIV7_FVAL                                           0xd
#define HWIO_GCC_USB30_SEC_MOCK_UTMI_CFG_RCGR_SRC_DIV_DIV7_5_FVAL                                         0xe
#define HWIO_GCC_USB30_SEC_MOCK_UTMI_CFG_RCGR_SRC_DIV_DIV8_FVAL                                           0xf
#define HWIO_GCC_USB30_SEC_MOCK_UTMI_CFG_RCGR_SRC_DIV_DIV8_5_FVAL                                        0x10
#define HWIO_GCC_USB30_SEC_MOCK_UTMI_CFG_RCGR_SRC_DIV_DIV9_FVAL                                          0x11
#define HWIO_GCC_USB30_SEC_MOCK_UTMI_CFG_RCGR_SRC_DIV_DIV9_5_FVAL                                        0x12
#define HWIO_GCC_USB30_SEC_MOCK_UTMI_CFG_RCGR_SRC_DIV_DIV10_FVAL                                         0x13
#define HWIO_GCC_USB30_SEC_MOCK_UTMI_CFG_RCGR_SRC_DIV_DIV10_5_FVAL                                       0x14
#define HWIO_GCC_USB30_SEC_MOCK_UTMI_CFG_RCGR_SRC_DIV_DIV11_FVAL                                         0x15
#define HWIO_GCC_USB30_SEC_MOCK_UTMI_CFG_RCGR_SRC_DIV_DIV11_5_FVAL                                       0x16
#define HWIO_GCC_USB30_SEC_MOCK_UTMI_CFG_RCGR_SRC_DIV_DIV12_FVAL                                         0x17
#define HWIO_GCC_USB30_SEC_MOCK_UTMI_CFG_RCGR_SRC_DIV_DIV12_5_FVAL                                       0x18
#define HWIO_GCC_USB30_SEC_MOCK_UTMI_CFG_RCGR_SRC_DIV_DIV13_FVAL                                         0x19
#define HWIO_GCC_USB30_SEC_MOCK_UTMI_CFG_RCGR_SRC_DIV_DIV13_5_FVAL                                       0x1a
#define HWIO_GCC_USB30_SEC_MOCK_UTMI_CFG_RCGR_SRC_DIV_DIV14_FVAL                                         0x1b
#define HWIO_GCC_USB30_SEC_MOCK_UTMI_CFG_RCGR_SRC_DIV_DIV14_5_FVAL                                       0x1c
#define HWIO_GCC_USB30_SEC_MOCK_UTMI_CFG_RCGR_SRC_DIV_DIV15_FVAL                                         0x1d
#define HWIO_GCC_USB30_SEC_MOCK_UTMI_CFG_RCGR_SRC_DIV_DIV15_5_FVAL                                       0x1e
#define HWIO_GCC_USB30_SEC_MOCK_UTMI_CFG_RCGR_SRC_DIV_DIV16_FVAL                                         0x1f

#define HWIO_GCC_USB30_SEC_MOCK_UTMI_POSTDIV_CDIVR_ADDR                                            (GCC_CLK_CTL_REG_REG_BASE      + 0x0001004c)
#define HWIO_GCC_USB30_SEC_MOCK_UTMI_POSTDIV_CDIVR_PHYS                                            (GCC_CLK_CTL_REG_REG_BASE_PHYS + 0x0001004c)
#define HWIO_GCC_USB30_SEC_MOCK_UTMI_POSTDIV_CDIVR_OFFS                                            (GCC_CLK_CTL_REG_REG_BASE_OFFS + 0x0001004c)
#define HWIO_GCC_USB30_SEC_MOCK_UTMI_POSTDIV_CDIVR_RMSK                                                   0x3
#define HWIO_GCC_USB30_SEC_MOCK_UTMI_POSTDIV_CDIVR_IN          \
        in_dword(HWIO_GCC_USB30_SEC_MOCK_UTMI_POSTDIV_CDIVR_ADDR)
#define HWIO_GCC_USB30_SEC_MOCK_UTMI_POSTDIV_CDIVR_INM(m)      \
        in_dword_masked(HWIO_GCC_USB30_SEC_MOCK_UTMI_POSTDIV_CDIVR_ADDR, m)
#define HWIO_GCC_USB30_SEC_MOCK_UTMI_POSTDIV_CDIVR_OUT(v)      \
        out_dword(HWIO_GCC_USB30_SEC_MOCK_UTMI_POSTDIV_CDIVR_ADDR,v)
#define HWIO_GCC_USB30_SEC_MOCK_UTMI_POSTDIV_CDIVR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GCC_USB30_SEC_MOCK_UTMI_POSTDIV_CDIVR_ADDR,m,v,HWIO_GCC_USB30_SEC_MOCK_UTMI_POSTDIV_CDIVR_IN)
#define HWIO_GCC_USB30_SEC_MOCK_UTMI_POSTDIV_CDIVR_CLK_DIV_BMSK                                           0x3
#define HWIO_GCC_USB30_SEC_MOCK_UTMI_POSTDIV_CDIVR_CLK_DIV_SHFT                                           0x0

#define HWIO_GCC_USB3_SEC_PHY_AUX_CBCR_ADDR                                                        (GCC_CLK_CTL_REG_REG_BASE      + 0x00010050)
#define HWIO_GCC_USB3_SEC_PHY_AUX_CBCR_PHYS                                                        (GCC_CLK_CTL_REG_REG_BASE_PHYS + 0x00010050)
#define HWIO_GCC_USB3_SEC_PHY_AUX_CBCR_OFFS                                                        (GCC_CLK_CTL_REG_REG_BASE_OFFS + 0x00010050)
#define HWIO_GCC_USB3_SEC_PHY_AUX_CBCR_RMSK                                                        0x80000005
#define HWIO_GCC_USB3_SEC_PHY_AUX_CBCR_IN          \
        in_dword(HWIO_GCC_USB3_SEC_PHY_AUX_CBCR_ADDR)
#define HWIO_GCC_USB3_SEC_PHY_AUX_CBCR_INM(m)      \
        in_dword_masked(HWIO_GCC_USB3_SEC_PHY_AUX_CBCR_ADDR, m)
#define HWIO_GCC_USB3_SEC_PHY_AUX_CBCR_OUT(v)      \
        out_dword(HWIO_GCC_USB3_SEC_PHY_AUX_CBCR_ADDR,v)
#define HWIO_GCC_USB3_SEC_PHY_AUX_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GCC_USB3_SEC_PHY_AUX_CBCR_ADDR,m,v,HWIO_GCC_USB3_SEC_PHY_AUX_CBCR_IN)
#define HWIO_GCC_USB3_SEC_PHY_AUX_CBCR_CLK_OFF_BMSK                                                0x80000000
#define HWIO_GCC_USB3_SEC_PHY_AUX_CBCR_CLK_OFF_SHFT                                                      0x1f
#define HWIO_GCC_USB3_SEC_PHY_AUX_CBCR_CLK_ARES_BMSK                                                      0x4
#define HWIO_GCC_USB3_SEC_PHY_AUX_CBCR_CLK_ARES_SHFT                                                      0x2
#define HWIO_GCC_USB3_SEC_PHY_AUX_CBCR_CLK_ARES_NO_RESET_FVAL                                             0x0
#define HWIO_GCC_USB3_SEC_PHY_AUX_CBCR_CLK_ARES_RESET_FVAL                                                0x1
#define HWIO_GCC_USB3_SEC_PHY_AUX_CBCR_CLK_ENABLE_BMSK                                                    0x1
#define HWIO_GCC_USB3_SEC_PHY_AUX_CBCR_CLK_ENABLE_SHFT                                                    0x0
#define HWIO_GCC_USB3_SEC_PHY_AUX_CBCR_CLK_ENABLE_DISABLE_FVAL                                            0x0
#define HWIO_GCC_USB3_SEC_PHY_AUX_CBCR_CLK_ENABLE_ENABLE_FVAL                                             0x1

#define HWIO_GCC_USB3_SEC_PHY_COM_AUX_CBCR_ADDR                                                    (GCC_CLK_CTL_REG_REG_BASE      + 0x00010054)
#define HWIO_GCC_USB3_SEC_PHY_COM_AUX_CBCR_PHYS                                                    (GCC_CLK_CTL_REG_REG_BASE_PHYS + 0x00010054)
#define HWIO_GCC_USB3_SEC_PHY_COM_AUX_CBCR_OFFS                                                    (GCC_CLK_CTL_REG_REG_BASE_OFFS + 0x00010054)
#define HWIO_GCC_USB3_SEC_PHY_COM_AUX_CBCR_RMSK                                                    0x80000005
#define HWIO_GCC_USB3_SEC_PHY_COM_AUX_CBCR_IN          \
        in_dword(HWIO_GCC_USB3_SEC_PHY_COM_AUX_CBCR_ADDR)
#define HWIO_GCC_USB3_SEC_PHY_COM_AUX_CBCR_INM(m)      \
        in_dword_masked(HWIO_GCC_USB3_SEC_PHY_COM_AUX_CBCR_ADDR, m)
#define HWIO_GCC_USB3_SEC_PHY_COM_AUX_CBCR_OUT(v)      \
        out_dword(HWIO_GCC_USB3_SEC_PHY_COM_AUX_CBCR_ADDR,v)
#define HWIO_GCC_USB3_SEC_PHY_COM_AUX_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GCC_USB3_SEC_PHY_COM_AUX_CBCR_ADDR,m,v,HWIO_GCC_USB3_SEC_PHY_COM_AUX_CBCR_IN)
#define HWIO_GCC_USB3_SEC_PHY_COM_AUX_CBCR_CLK_OFF_BMSK                                            0x80000000
#define HWIO_GCC_USB3_SEC_PHY_COM_AUX_CBCR_CLK_OFF_SHFT                                                  0x1f
#define HWIO_GCC_USB3_SEC_PHY_COM_AUX_CBCR_CLK_ARES_BMSK                                                  0x4
#define HWIO_GCC_USB3_SEC_PHY_COM_AUX_CBCR_CLK_ARES_SHFT                                                  0x2
#define HWIO_GCC_USB3_SEC_PHY_COM_AUX_CBCR_CLK_ARES_NO_RESET_FVAL                                         0x0
#define HWIO_GCC_USB3_SEC_PHY_COM_AUX_CBCR_CLK_ARES_RESET_FVAL                                            0x1
#define HWIO_GCC_USB3_SEC_PHY_COM_AUX_CBCR_CLK_ENABLE_BMSK                                                0x1
#define HWIO_GCC_USB3_SEC_PHY_COM_AUX_CBCR_CLK_ENABLE_SHFT                                                0x0
#define HWIO_GCC_USB3_SEC_PHY_COM_AUX_CBCR_CLK_ENABLE_DISABLE_FVAL                                        0x0
#define HWIO_GCC_USB3_SEC_PHY_COM_AUX_CBCR_CLK_ENABLE_ENABLE_FVAL                                         0x1

#define HWIO_GCC_USB3_SEC_PHY_PIPE_CBCR_ADDR                                                       (GCC_CLK_CTL_REG_REG_BASE      + 0x00010058)
#define HWIO_GCC_USB3_SEC_PHY_PIPE_CBCR_PHYS                                                       (GCC_CLK_CTL_REG_REG_BASE_PHYS + 0x00010058)
#define HWIO_GCC_USB3_SEC_PHY_PIPE_CBCR_OFFS                                                       (GCC_CLK_CTL_REG_REG_BASE_OFFS + 0x00010058)
#define HWIO_GCC_USB3_SEC_PHY_PIPE_CBCR_RMSK                                                       0x80000005
#define HWIO_GCC_USB3_SEC_PHY_PIPE_CBCR_IN          \
        in_dword(HWIO_GCC_USB3_SEC_PHY_PIPE_CBCR_ADDR)
#define HWIO_GCC_USB3_SEC_PHY_PIPE_CBCR_INM(m)      \
        in_dword_masked(HWIO_GCC_USB3_SEC_PHY_PIPE_CBCR_ADDR, m)
#define HWIO_GCC_USB3_SEC_PHY_PIPE_CBCR_OUT(v)      \
        out_dword(HWIO_GCC_USB3_SEC_PHY_PIPE_CBCR_ADDR,v)
#define HWIO_GCC_USB3_SEC_PHY_PIPE_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GCC_USB3_SEC_PHY_PIPE_CBCR_ADDR,m,v,HWIO_GCC_USB3_SEC_PHY_PIPE_CBCR_IN)
#define HWIO_GCC_USB3_SEC_PHY_PIPE_CBCR_CLK_OFF_BMSK                                               0x80000000
#define HWIO_GCC_USB3_SEC_PHY_PIPE_CBCR_CLK_OFF_SHFT                                                     0x1f
#define HWIO_GCC_USB3_SEC_PHY_PIPE_CBCR_CLK_ARES_BMSK                                                     0x4
#define HWIO_GCC_USB3_SEC_PHY_PIPE_CBCR_CLK_ARES_SHFT                                                     0x2
#define HWIO_GCC_USB3_SEC_PHY_PIPE_CBCR_CLK_ARES_NO_RESET_FVAL                                            0x0
#define HWIO_GCC_USB3_SEC_PHY_PIPE_CBCR_CLK_ARES_RESET_FVAL                                               0x1
#define HWIO_GCC_USB3_SEC_PHY_PIPE_CBCR_CLK_ENABLE_BMSK                                                   0x1
#define HWIO_GCC_USB3_SEC_PHY_PIPE_CBCR_CLK_ENABLE_SHFT                                                   0x0
#define HWIO_GCC_USB3_SEC_PHY_PIPE_CBCR_CLK_ENABLE_DISABLE_FVAL                                           0x0
#define HWIO_GCC_USB3_SEC_PHY_PIPE_CBCR_CLK_ENABLE_ENABLE_FVAL                                            0x1

#define HWIO_GCC_USB3_SEC_PHY_AUX_CMD_RCGR_ADDR                                                    (GCC_CLK_CTL_REG_REG_BASE      + 0x00010060)
#define HWIO_GCC_USB3_SEC_PHY_AUX_CMD_RCGR_PHYS                                                    (GCC_CLK_CTL_REG_REG_BASE_PHYS + 0x00010060)
#define HWIO_GCC_USB3_SEC_PHY_AUX_CMD_RCGR_OFFS                                                    (GCC_CLK_CTL_REG_REG_BASE_OFFS + 0x00010060)
#define HWIO_GCC_USB3_SEC_PHY_AUX_CMD_RCGR_RMSK                                                    0x80000013
#define HWIO_GCC_USB3_SEC_PHY_AUX_CMD_RCGR_IN          \
        in_dword(HWIO_GCC_USB3_SEC_PHY_AUX_CMD_RCGR_ADDR)
#define HWIO_GCC_USB3_SEC_PHY_AUX_CMD_RCGR_INM(m)      \
        in_dword_masked(HWIO_GCC_USB3_SEC_PHY_AUX_CMD_RCGR_ADDR, m)
#define HWIO_GCC_USB3_SEC_PHY_AUX_CMD_RCGR_OUT(v)      \
        out_dword(HWIO_GCC_USB3_SEC_PHY_AUX_CMD_RCGR_ADDR,v)
#define HWIO_GCC_USB3_SEC_PHY_AUX_CMD_RCGR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GCC_USB3_SEC_PHY_AUX_CMD_RCGR_ADDR,m,v,HWIO_GCC_USB3_SEC_PHY_AUX_CMD_RCGR_IN)
#define HWIO_GCC_USB3_SEC_PHY_AUX_CMD_RCGR_ROOT_OFF_BMSK                                           0x80000000
#define HWIO_GCC_USB3_SEC_PHY_AUX_CMD_RCGR_ROOT_OFF_SHFT                                                 0x1f
#define HWIO_GCC_USB3_SEC_PHY_AUX_CMD_RCGR_DIRTY_CFG_RCGR_BMSK                                           0x10
#define HWIO_GCC_USB3_SEC_PHY_AUX_CMD_RCGR_DIRTY_CFG_RCGR_SHFT                                            0x4
#define HWIO_GCC_USB3_SEC_PHY_AUX_CMD_RCGR_ROOT_EN_BMSK                                                   0x2
#define HWIO_GCC_USB3_SEC_PHY_AUX_CMD_RCGR_ROOT_EN_SHFT                                                   0x1
#define HWIO_GCC_USB3_SEC_PHY_AUX_CMD_RCGR_ROOT_EN_DISABLE_FVAL                                           0x0
#define HWIO_GCC_USB3_SEC_PHY_AUX_CMD_RCGR_ROOT_EN_ENABLE_FVAL                                            0x1
#define HWIO_GCC_USB3_SEC_PHY_AUX_CMD_RCGR_UPDATE_BMSK                                                    0x1
#define HWIO_GCC_USB3_SEC_PHY_AUX_CMD_RCGR_UPDATE_SHFT                                                    0x0
#define HWIO_GCC_USB3_SEC_PHY_AUX_CMD_RCGR_UPDATE_DISABLE_FVAL                                            0x0
#define HWIO_GCC_USB3_SEC_PHY_AUX_CMD_RCGR_UPDATE_ENABLE_FVAL                                             0x1

#define HWIO_GCC_USB3_SEC_PHY_AUX_CFG_RCGR_ADDR                                                    (GCC_CLK_CTL_REG_REG_BASE      + 0x00010064)
#define HWIO_GCC_USB3_SEC_PHY_AUX_CFG_RCGR_PHYS                                                    (GCC_CLK_CTL_REG_REG_BASE_PHYS + 0x00010064)
#define HWIO_GCC_USB3_SEC_PHY_AUX_CFG_RCGR_OFFS                                                    (GCC_CLK_CTL_REG_REG_BASE_OFFS + 0x00010064)
#define HWIO_GCC_USB3_SEC_PHY_AUX_CFG_RCGR_RMSK                                                      0x11071f
#define HWIO_GCC_USB3_SEC_PHY_AUX_CFG_RCGR_IN          \
        in_dword(HWIO_GCC_USB3_SEC_PHY_AUX_CFG_RCGR_ADDR)
#define HWIO_GCC_USB3_SEC_PHY_AUX_CFG_RCGR_INM(m)      \
        in_dword_masked(HWIO_GCC_USB3_SEC_PHY_AUX_CFG_RCGR_ADDR, m)
#define HWIO_GCC_USB3_SEC_PHY_AUX_CFG_RCGR_OUT(v)      \
        out_dword(HWIO_GCC_USB3_SEC_PHY_AUX_CFG_RCGR_ADDR,v)
#define HWIO_GCC_USB3_SEC_PHY_AUX_CFG_RCGR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GCC_USB3_SEC_PHY_AUX_CFG_RCGR_ADDR,m,v,HWIO_GCC_USB3_SEC_PHY_AUX_CFG_RCGR_IN)
#define HWIO_GCC_USB3_SEC_PHY_AUX_CFG_RCGR_HW_CLK_CONTROL_BMSK                                       0x100000
#define HWIO_GCC_USB3_SEC_PHY_AUX_CFG_RCGR_HW_CLK_CONTROL_SHFT                                           0x14
#define HWIO_GCC_USB3_SEC_PHY_AUX_CFG_RCGR_HW_CLK_CONTROL_DISABLE_FVAL                                    0x0
#define HWIO_GCC_USB3_SEC_PHY_AUX_CFG_RCGR_HW_CLK_CONTROL_ENABLE_FVAL                                     0x1
#define HWIO_GCC_USB3_SEC_PHY_AUX_CFG_RCGR_RCGLITE_DISABLE_BMSK                                       0x10000
#define HWIO_GCC_USB3_SEC_PHY_AUX_CFG_RCGR_RCGLITE_DISABLE_SHFT                                          0x10
#define HWIO_GCC_USB3_SEC_PHY_AUX_CFG_RCGR_RCGLITE_DISABLE_RCGLITE_ENABLED_FVAL                           0x0
#define HWIO_GCC_USB3_SEC_PHY_AUX_CFG_RCGR_RCGLITE_DISABLE_RCGLITE_DISABLED_FVAL                          0x1
#define HWIO_GCC_USB3_SEC_PHY_AUX_CFG_RCGR_SRC_SEL_BMSK                                                 0x700
#define HWIO_GCC_USB3_SEC_PHY_AUX_CFG_RCGR_SRC_SEL_SHFT                                                   0x8
#define HWIO_GCC_USB3_SEC_PHY_AUX_CFG_RCGR_SRC_SEL_SRC0_FVAL                                              0x0
#define HWIO_GCC_USB3_SEC_PHY_AUX_CFG_RCGR_SRC_SEL_SRC1_FVAL                                              0x1
#define HWIO_GCC_USB3_SEC_PHY_AUX_CFG_RCGR_SRC_SEL_SRC2_FVAL                                              0x2
#define HWIO_GCC_USB3_SEC_PHY_AUX_CFG_RCGR_SRC_SEL_SRC3_FVAL                                              0x3
#define HWIO_GCC_USB3_SEC_PHY_AUX_CFG_RCGR_SRC_SEL_SRC4_FVAL                                              0x4
#define HWIO_GCC_USB3_SEC_PHY_AUX_CFG_RCGR_SRC_SEL_SRC5_FVAL                                              0x5
#define HWIO_GCC_USB3_SEC_PHY_AUX_CFG_RCGR_SRC_SEL_SRC6_FVAL                                              0x6
#define HWIO_GCC_USB3_SEC_PHY_AUX_CFG_RCGR_SRC_SEL_SRC7_FVAL                                              0x7
#define HWIO_GCC_USB3_SEC_PHY_AUX_CFG_RCGR_SRC_DIV_BMSK                                                  0x1f
#define HWIO_GCC_USB3_SEC_PHY_AUX_CFG_RCGR_SRC_DIV_SHFT                                                   0x0
#define HWIO_GCC_USB3_SEC_PHY_AUX_CFG_RCGR_SRC_DIV_BYPASS_FVAL                                            0x0
#define HWIO_GCC_USB3_SEC_PHY_AUX_CFG_RCGR_SRC_DIV_DIV1_FVAL                                              0x1
#define HWIO_GCC_USB3_SEC_PHY_AUX_CFG_RCGR_SRC_DIV_DIV1_5_FVAL                                            0x2
#define HWIO_GCC_USB3_SEC_PHY_AUX_CFG_RCGR_SRC_DIV_DIV2_FVAL                                              0x3
#define HWIO_GCC_USB3_SEC_PHY_AUX_CFG_RCGR_SRC_DIV_DIV2_5_FVAL                                            0x4
#define HWIO_GCC_USB3_SEC_PHY_AUX_CFG_RCGR_SRC_DIV_DIV3_FVAL                                              0x5
#define HWIO_GCC_USB3_SEC_PHY_AUX_CFG_RCGR_SRC_DIV_DIV3_5_FVAL                                            0x6
#define HWIO_GCC_USB3_SEC_PHY_AUX_CFG_RCGR_SRC_DIV_DIV4_FVAL                                              0x7
#define HWIO_GCC_USB3_SEC_PHY_AUX_CFG_RCGR_SRC_DIV_DIV4_5_FVAL                                            0x8
#define HWIO_GCC_USB3_SEC_PHY_AUX_CFG_RCGR_SRC_DIV_DIV5_FVAL                                              0x9
#define HWIO_GCC_USB3_SEC_PHY_AUX_CFG_RCGR_SRC_DIV_DIV5_5_FVAL                                            0xa
#define HWIO_GCC_USB3_SEC_PHY_AUX_CFG_RCGR_SRC_DIV_DIV6_FVAL                                              0xb
#define HWIO_GCC_USB3_SEC_PHY_AUX_CFG_RCGR_SRC_DIV_DIV6_5_FVAL                                            0xc
#define HWIO_GCC_USB3_SEC_PHY_AUX_CFG_RCGR_SRC_DIV_DIV7_FVAL                                              0xd
#define HWIO_GCC_USB3_SEC_PHY_AUX_CFG_RCGR_SRC_DIV_DIV7_5_FVAL                                            0xe
#define HWIO_GCC_USB3_SEC_PHY_AUX_CFG_RCGR_SRC_DIV_DIV8_FVAL                                              0xf
#define HWIO_GCC_USB3_SEC_PHY_AUX_CFG_RCGR_SRC_DIV_DIV8_5_FVAL                                           0x10
#define HWIO_GCC_USB3_SEC_PHY_AUX_CFG_RCGR_SRC_DIV_DIV9_FVAL                                             0x11
#define HWIO_GCC_USB3_SEC_PHY_AUX_CFG_RCGR_SRC_DIV_DIV9_5_FVAL                                           0x12
#define HWIO_GCC_USB3_SEC_PHY_AUX_CFG_RCGR_SRC_DIV_DIV10_FVAL                                            0x13
#define HWIO_GCC_USB3_SEC_PHY_AUX_CFG_RCGR_SRC_DIV_DIV10_5_FVAL                                          0x14
#define HWIO_GCC_USB3_SEC_PHY_AUX_CFG_RCGR_SRC_DIV_DIV11_FVAL                                            0x15
#define HWIO_GCC_USB3_SEC_PHY_AUX_CFG_RCGR_SRC_DIV_DIV11_5_FVAL                                          0x16
#define HWIO_GCC_USB3_SEC_PHY_AUX_CFG_RCGR_SRC_DIV_DIV12_FVAL                                            0x17
#define HWIO_GCC_USB3_SEC_PHY_AUX_CFG_RCGR_SRC_DIV_DIV12_5_FVAL                                          0x18
#define HWIO_GCC_USB3_SEC_PHY_AUX_CFG_RCGR_SRC_DIV_DIV13_FVAL                                            0x19
#define HWIO_GCC_USB3_SEC_PHY_AUX_CFG_RCGR_SRC_DIV_DIV13_5_FVAL                                          0x1a
#define HWIO_GCC_USB3_SEC_PHY_AUX_CFG_RCGR_SRC_DIV_DIV14_FVAL                                            0x1b
#define HWIO_GCC_USB3_SEC_PHY_AUX_CFG_RCGR_SRC_DIV_DIV14_5_FVAL                                          0x1c
#define HWIO_GCC_USB3_SEC_PHY_AUX_CFG_RCGR_SRC_DIV_DIV15_FVAL                                            0x1d
#define HWIO_GCC_USB3_SEC_PHY_AUX_CFG_RCGR_SRC_DIV_DIV15_5_FVAL                                          0x1e
#define HWIO_GCC_USB3_SEC_PHY_AUX_CFG_RCGR_SRC_DIV_DIV16_FVAL                                            0x1f

#define HWIO_GCC_USB30_MP_BCR_ADDR                                                                 (GCC_CLK_CTL_REG_REG_BASE      + 0x000a6000)
#define HWIO_GCC_USB30_MP_BCR_PHYS                                                                 (GCC_CLK_CTL_REG_REG_BASE_PHYS + 0x000a6000)
#define HWIO_GCC_USB30_MP_BCR_OFFS                                                                 (GCC_CLK_CTL_REG_REG_BASE_OFFS + 0x000a6000)
#define HWIO_GCC_USB30_MP_BCR_RMSK                                                                        0x1
#define HWIO_GCC_USB30_MP_BCR_IN          \
        in_dword(HWIO_GCC_USB30_MP_BCR_ADDR)
#define HWIO_GCC_USB30_MP_BCR_INM(m)      \
        in_dword_masked(HWIO_GCC_USB30_MP_BCR_ADDR, m)
#define HWIO_GCC_USB30_MP_BCR_OUT(v)      \
        out_dword(HWIO_GCC_USB30_MP_BCR_ADDR,v)
#define HWIO_GCC_USB30_MP_BCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GCC_USB30_MP_BCR_ADDR,m,v,HWIO_GCC_USB30_MP_BCR_IN)
#define HWIO_GCC_USB30_MP_BCR_BLK_ARES_BMSK                                                               0x1
#define HWIO_GCC_USB30_MP_BCR_BLK_ARES_SHFT                                                               0x0
#define HWIO_GCC_USB30_MP_BCR_BLK_ARES_DISABLE_FVAL                                                       0x0
#define HWIO_GCC_USB30_MP_BCR_BLK_ARES_ENABLE_FVAL                                                        0x1

#define HWIO_GCC_USB30_MP_GDSCR_ADDR                                                               (GCC_CLK_CTL_REG_REG_BASE      + 0x000a6004)
#define HWIO_GCC_USB30_MP_GDSCR_PHYS                                                               (GCC_CLK_CTL_REG_REG_BASE_PHYS + 0x000a6004)
#define HWIO_GCC_USB30_MP_GDSCR_OFFS                                                               (GCC_CLK_CTL_REG_REG_BASE_OFFS + 0x000a6004)
#define HWIO_GCC_USB30_MP_GDSCR_RMSK                                                               0xf8ffffff
#define HWIO_GCC_USB30_MP_GDSCR_IN          \
        in_dword(HWIO_GCC_USB30_MP_GDSCR_ADDR)
#define HWIO_GCC_USB30_MP_GDSCR_INM(m)      \
        in_dword_masked(HWIO_GCC_USB30_MP_GDSCR_ADDR, m)
#define HWIO_GCC_USB30_MP_GDSCR_OUT(v)      \
        out_dword(HWIO_GCC_USB30_MP_GDSCR_ADDR,v)
#define HWIO_GCC_USB30_MP_GDSCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GCC_USB30_MP_GDSCR_ADDR,m,v,HWIO_GCC_USB30_MP_GDSCR_IN)
#define HWIO_GCC_USB30_MP_GDSCR_PWR_ON_BMSK                                                        0x80000000
#define HWIO_GCC_USB30_MP_GDSCR_PWR_ON_SHFT                                                              0x1f
#define HWIO_GCC_USB30_MP_GDSCR_GDSC_STATE_BMSK                                                    0x78000000
#define HWIO_GCC_USB30_MP_GDSCR_GDSC_STATE_SHFT                                                          0x1b
#define HWIO_GCC_USB30_MP_GDSCR_EN_REST_WAIT_BMSK                                                    0xf00000
#define HWIO_GCC_USB30_MP_GDSCR_EN_REST_WAIT_SHFT                                                        0x14
#define HWIO_GCC_USB30_MP_GDSCR_EN_FEW_WAIT_BMSK                                                      0xf0000
#define HWIO_GCC_USB30_MP_GDSCR_EN_FEW_WAIT_SHFT                                                         0x10
#define HWIO_GCC_USB30_MP_GDSCR_CLK_DIS_WAIT_BMSK                                                      0xf000
#define HWIO_GCC_USB30_MP_GDSCR_CLK_DIS_WAIT_SHFT                                                         0xc
#define HWIO_GCC_USB30_MP_GDSCR_RETAIN_FF_ENABLE_BMSK                                                   0x800
#define HWIO_GCC_USB30_MP_GDSCR_RETAIN_FF_ENABLE_SHFT                                                     0xb
#define HWIO_GCC_USB30_MP_GDSCR_RETAIN_FF_ENABLE_DISABLE_FVAL                                             0x0
#define HWIO_GCC_USB30_MP_GDSCR_RETAIN_FF_ENABLE_ENABLE_FVAL                                              0x1
#define HWIO_GCC_USB30_MP_GDSCR_RESTORE_BMSK                                                            0x400
#define HWIO_GCC_USB30_MP_GDSCR_RESTORE_SHFT                                                              0xa
#define HWIO_GCC_USB30_MP_GDSCR_RESTORE_DISABLE_FVAL                                                      0x0
#define HWIO_GCC_USB30_MP_GDSCR_RESTORE_ENABLE_FVAL                                                       0x1
#define HWIO_GCC_USB30_MP_GDSCR_SAVE_BMSK                                                               0x200
#define HWIO_GCC_USB30_MP_GDSCR_SAVE_SHFT                                                                 0x9
#define HWIO_GCC_USB30_MP_GDSCR_SAVE_DISABLE_FVAL                                                         0x0
#define HWIO_GCC_USB30_MP_GDSCR_SAVE_ENABLE_FVAL                                                          0x1
#define HWIO_GCC_USB30_MP_GDSCR_RETAIN_BMSK                                                             0x100
#define HWIO_GCC_USB30_MP_GDSCR_RETAIN_SHFT                                                               0x8
#define HWIO_GCC_USB30_MP_GDSCR_RETAIN_DISABLE_FVAL                                                       0x0
#define HWIO_GCC_USB30_MP_GDSCR_RETAIN_ENABLE_FVAL                                                        0x1
#define HWIO_GCC_USB30_MP_GDSCR_EN_REST_BMSK                                                             0x80
#define HWIO_GCC_USB30_MP_GDSCR_EN_REST_SHFT                                                              0x7
#define HWIO_GCC_USB30_MP_GDSCR_EN_REST_DISABLE_FVAL                                                      0x0
#define HWIO_GCC_USB30_MP_GDSCR_EN_REST_ENABLE_FVAL                                                       0x1
#define HWIO_GCC_USB30_MP_GDSCR_EN_FEW_BMSK                                                              0x40
#define HWIO_GCC_USB30_MP_GDSCR_EN_FEW_SHFT                                                               0x6
#define HWIO_GCC_USB30_MP_GDSCR_EN_FEW_DISABLE_FVAL                                                       0x0
#define HWIO_GCC_USB30_MP_GDSCR_EN_FEW_ENABLE_FVAL                                                        0x1
#define HWIO_GCC_USB30_MP_GDSCR_CLAMP_IO_BMSK                                                            0x20
#define HWIO_GCC_USB30_MP_GDSCR_CLAMP_IO_SHFT                                                             0x5
#define HWIO_GCC_USB30_MP_GDSCR_CLAMP_IO_DISABLE_FVAL                                                     0x0
#define HWIO_GCC_USB30_MP_GDSCR_CLAMP_IO_ENABLE_FVAL                                                      0x1
#define HWIO_GCC_USB30_MP_GDSCR_CLK_DISABLE_BMSK                                                         0x10
#define HWIO_GCC_USB30_MP_GDSCR_CLK_DISABLE_SHFT                                                          0x4
#define HWIO_GCC_USB30_MP_GDSCR_CLK_DISABLE_CLK_NOT_DISABLE_FVAL                                          0x0
#define HWIO_GCC_USB30_MP_GDSCR_CLK_DISABLE_CLK_DISABLE_FVAL                                              0x1
#define HWIO_GCC_USB30_MP_GDSCR_PD_ARES_BMSK                                                              0x8
#define HWIO_GCC_USB30_MP_GDSCR_PD_ARES_SHFT                                                              0x3
#define HWIO_GCC_USB30_MP_GDSCR_PD_ARES_NO_RESET_FVAL                                                     0x0
#define HWIO_GCC_USB30_MP_GDSCR_PD_ARES_RESET_FVAL                                                        0x1
#define HWIO_GCC_USB30_MP_GDSCR_SW_OVERRIDE_BMSK                                                          0x4
#define HWIO_GCC_USB30_MP_GDSCR_SW_OVERRIDE_SHFT                                                          0x2
#define HWIO_GCC_USB30_MP_GDSCR_SW_OVERRIDE_DISABLE_FVAL                                                  0x0
#define HWIO_GCC_USB30_MP_GDSCR_SW_OVERRIDE_ENABLE_FVAL                                                   0x1
#define HWIO_GCC_USB30_MP_GDSCR_HW_CONTROL_BMSK                                                           0x2
#define HWIO_GCC_USB30_MP_GDSCR_HW_CONTROL_SHFT                                                           0x1
#define HWIO_GCC_USB30_MP_GDSCR_HW_CONTROL_DISABLE_FVAL                                                   0x0
#define HWIO_GCC_USB30_MP_GDSCR_HW_CONTROL_ENABLE_FVAL                                                    0x1
#define HWIO_GCC_USB30_MP_GDSCR_SW_COLLAPSE_BMSK                                                          0x1
#define HWIO_GCC_USB30_MP_GDSCR_SW_COLLAPSE_SHFT                                                          0x0
#define HWIO_GCC_USB30_MP_GDSCR_SW_COLLAPSE_DISABLE_FVAL                                                  0x0
#define HWIO_GCC_USB30_MP_GDSCR_SW_COLLAPSE_ENABLE_FVAL                                                   0x1

#define HWIO_GCC_USB30_MP_CFG_GDSCR_ADDR                                                           (GCC_CLK_CTL_REG_REG_BASE      + 0x000a6008)
#define HWIO_GCC_USB30_MP_CFG_GDSCR_PHYS                                                           (GCC_CLK_CTL_REG_REG_BASE_PHYS + 0x000a6008)
#define HWIO_GCC_USB30_MP_CFG_GDSCR_OFFS                                                           (GCC_CLK_CTL_REG_REG_BASE_OFFS + 0x000a6008)
#define HWIO_GCC_USB30_MP_CFG_GDSCR_RMSK                                                           0xffffffff
#define HWIO_GCC_USB30_MP_CFG_GDSCR_IN          \
        in_dword(HWIO_GCC_USB30_MP_CFG_GDSCR_ADDR)
#define HWIO_GCC_USB30_MP_CFG_GDSCR_INM(m)      \
        in_dword_masked(HWIO_GCC_USB30_MP_CFG_GDSCR_ADDR, m)
#define HWIO_GCC_USB30_MP_CFG_GDSCR_OUT(v)      \
        out_dword(HWIO_GCC_USB30_MP_CFG_GDSCR_ADDR,v)
#define HWIO_GCC_USB30_MP_CFG_GDSCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GCC_USB30_MP_CFG_GDSCR_ADDR,m,v,HWIO_GCC_USB30_MP_CFG_GDSCR_IN)
#define HWIO_GCC_USB30_MP_CFG_GDSCR_GDSC_SPARE_CTRL_IN_BMSK                                        0xf0000000
#define HWIO_GCC_USB30_MP_CFG_GDSCR_GDSC_SPARE_CTRL_IN_SHFT                                              0x1c
#define HWIO_GCC_USB30_MP_CFG_GDSCR_GDSC_SPARE_CTRL_OUT_BMSK                                        0xc000000
#define HWIO_GCC_USB30_MP_CFG_GDSCR_GDSC_SPARE_CTRL_OUT_SHFT                                             0x1a
#define HWIO_GCC_USB30_MP_CFG_GDSCR_GDSC_PWR_DWN_START_BMSK                                         0x2000000
#define HWIO_GCC_USB30_MP_CFG_GDSCR_GDSC_PWR_DWN_START_SHFT                                              0x19
#define HWIO_GCC_USB30_MP_CFG_GDSCR_GDSC_PWR_UP_START_BMSK                                          0x1000000
#define HWIO_GCC_USB30_MP_CFG_GDSCR_GDSC_PWR_UP_START_SHFT                                               0x18
#define HWIO_GCC_USB30_MP_CFG_GDSCR_GDSC_CFG_FSM_STATE_STATUS_BMSK                                   0xf00000
#define HWIO_GCC_USB30_MP_CFG_GDSCR_GDSC_CFG_FSM_STATE_STATUS_SHFT                                       0x14
#define HWIO_GCC_USB30_MP_CFG_GDSCR_GDSC_MEM_PWR_ACK_STATUS_BMSK                                      0x80000
#define HWIO_GCC_USB30_MP_CFG_GDSCR_GDSC_MEM_PWR_ACK_STATUS_SHFT                                         0x13
#define HWIO_GCC_USB30_MP_CFG_GDSCR_GDSC_ENR_ACK_STATUS_BMSK                                          0x40000
#define HWIO_GCC_USB30_MP_CFG_GDSCR_GDSC_ENR_ACK_STATUS_SHFT                                             0x12
#define HWIO_GCC_USB30_MP_CFG_GDSCR_GDSC_ENF_ACK_STATUS_BMSK                                          0x20000
#define HWIO_GCC_USB30_MP_CFG_GDSCR_GDSC_ENF_ACK_STATUS_SHFT                                             0x11
#define HWIO_GCC_USB30_MP_CFG_GDSCR_GDSC_POWER_UP_COMPLETE_BMSK                                       0x10000
#define HWIO_GCC_USB30_MP_CFG_GDSCR_GDSC_POWER_UP_COMPLETE_SHFT                                          0x10
#define HWIO_GCC_USB30_MP_CFG_GDSCR_GDSC_POWER_DOWN_COMPLETE_BMSK                                      0x8000
#define HWIO_GCC_USB30_MP_CFG_GDSCR_GDSC_POWER_DOWN_COMPLETE_SHFT                                         0xf
#define HWIO_GCC_USB30_MP_CFG_GDSCR_SOFTWARE_CONTROL_OVERRIDE_BMSK                                     0x7800
#define HWIO_GCC_USB30_MP_CFG_GDSCR_SOFTWARE_CONTROL_OVERRIDE_SHFT                                        0xb
#define HWIO_GCC_USB30_MP_CFG_GDSCR_GDSC_HANDSHAKE_DIS_BMSK                                             0x400
#define HWIO_GCC_USB30_MP_CFG_GDSCR_GDSC_HANDSHAKE_DIS_SHFT                                               0xa
#define HWIO_GCC_USB30_MP_CFG_GDSCR_GDSC_MEM_PERI_FORCE_IN_SW_BMSK                                      0x200
#define HWIO_GCC_USB30_MP_CFG_GDSCR_GDSC_MEM_PERI_FORCE_IN_SW_SHFT                                        0x9
#define HWIO_GCC_USB30_MP_CFG_GDSCR_GDSC_MEM_CORE_FORCE_IN_SW_BMSK                                      0x100
#define HWIO_GCC_USB30_MP_CFG_GDSCR_GDSC_MEM_CORE_FORCE_IN_SW_SHFT                                        0x8
#define HWIO_GCC_USB30_MP_CFG_GDSCR_GDSC_PHASE_RESET_EN_SW_BMSK                                          0x80
#define HWIO_GCC_USB30_MP_CFG_GDSCR_GDSC_PHASE_RESET_EN_SW_SHFT                                           0x7
#define HWIO_GCC_USB30_MP_CFG_GDSCR_GDSC_PHASE_RESET_DELAY_COUNT_SW_BMSK                                 0x60
#define HWIO_GCC_USB30_MP_CFG_GDSCR_GDSC_PHASE_RESET_DELAY_COUNT_SW_SHFT                                  0x5
#define HWIO_GCC_USB30_MP_CFG_GDSCR_GDSC_PSCBC_PWR_DWN_SW_BMSK                                           0x10
#define HWIO_GCC_USB30_MP_CFG_GDSCR_GDSC_PSCBC_PWR_DWN_SW_SHFT                                            0x4
#define HWIO_GCC_USB30_MP_CFG_GDSCR_UNCLAMP_IO_SOFTWARE_OVERRIDE_BMSK                                     0x8
#define HWIO_GCC_USB30_MP_CFG_GDSCR_UNCLAMP_IO_SOFTWARE_OVERRIDE_SHFT                                     0x3
#define HWIO_GCC_USB30_MP_CFG_GDSCR_SAVE_RESTORE_SOFTWARE_OVERRIDE_BMSK                                   0x4
#define HWIO_GCC_USB30_MP_CFG_GDSCR_SAVE_RESTORE_SOFTWARE_OVERRIDE_SHFT                                   0x2
#define HWIO_GCC_USB30_MP_CFG_GDSCR_CLAMP_IO_SOFTWARE_OVERRIDE_BMSK                                       0x2
#define HWIO_GCC_USB30_MP_CFG_GDSCR_CLAMP_IO_SOFTWARE_OVERRIDE_SHFT                                       0x1
#define HWIO_GCC_USB30_MP_CFG_GDSCR_DISABLE_CLK_SOFTWARE_OVERRIDE_BMSK                                    0x1
#define HWIO_GCC_USB30_MP_CFG_GDSCR_DISABLE_CLK_SOFTWARE_OVERRIDE_SHFT                                    0x0

#define HWIO_GCC_USB30_MP_CFG2_GDSCR_ADDR                                                          (GCC_CLK_CTL_REG_REG_BASE      + 0x000a600c)
#define HWIO_GCC_USB30_MP_CFG2_GDSCR_PHYS                                                          (GCC_CLK_CTL_REG_REG_BASE_PHYS + 0x000a600c)
#define HWIO_GCC_USB30_MP_CFG2_GDSCR_OFFS                                                          (GCC_CLK_CTL_REG_REG_BASE_OFFS + 0x000a600c)
#define HWIO_GCC_USB30_MP_CFG2_GDSCR_RMSK                                                             0x1ffff
#define HWIO_GCC_USB30_MP_CFG2_GDSCR_IN          \
        in_dword(HWIO_GCC_USB30_MP_CFG2_GDSCR_ADDR)
#define HWIO_GCC_USB30_MP_CFG2_GDSCR_INM(m)      \
        in_dword_masked(HWIO_GCC_USB30_MP_CFG2_GDSCR_ADDR, m)
#define HWIO_GCC_USB30_MP_CFG2_GDSCR_OUT(v)      \
        out_dword(HWIO_GCC_USB30_MP_CFG2_GDSCR_ADDR,v)
#define HWIO_GCC_USB30_MP_CFG2_GDSCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GCC_USB30_MP_CFG2_GDSCR_ADDR,m,v,HWIO_GCC_USB30_MP_CFG2_GDSCR_IN)
#define HWIO_GCC_USB30_MP_CFG2_GDSCR_GDSC_CLAMP_MEM_SW_BMSK                                           0x10000
#define HWIO_GCC_USB30_MP_CFG2_GDSCR_GDSC_CLAMP_MEM_SW_SHFT                                              0x10
#define HWIO_GCC_USB30_MP_CFG2_GDSCR_DLY_MEM_PWR_UP_BMSK                                               0xf000
#define HWIO_GCC_USB30_MP_CFG2_GDSCR_DLY_MEM_PWR_UP_SHFT                                                  0xc
#define HWIO_GCC_USB30_MP_CFG2_GDSCR_DLY_DEASSERT_CLAMP_MEM_BMSK                                        0xf00
#define HWIO_GCC_USB30_MP_CFG2_GDSCR_DLY_DEASSERT_CLAMP_MEM_SHFT                                          0x8
#define HWIO_GCC_USB30_MP_CFG2_GDSCR_DLY_ASSERT_CLAMP_MEM_BMSK                                           0xf0
#define HWIO_GCC_USB30_MP_CFG2_GDSCR_DLY_ASSERT_CLAMP_MEM_SHFT                                            0x4
#define HWIO_GCC_USB30_MP_CFG2_GDSCR_MEM_PWR_DWN_TIMEOUT_BMSK                                             0xf
#define HWIO_GCC_USB30_MP_CFG2_GDSCR_MEM_PWR_DWN_TIMEOUT_SHFT                                             0x0

#define HWIO_GCC_USB30_MP_MASTER_CBCR_ADDR                                                         (GCC_CLK_CTL_REG_REG_BASE      + 0x000a6010)
#define HWIO_GCC_USB30_MP_MASTER_CBCR_PHYS                                                         (GCC_CLK_CTL_REG_REG_BASE_PHYS + 0x000a6010)
#define HWIO_GCC_USB30_MP_MASTER_CBCR_OFFS                                                         (GCC_CLK_CTL_REG_REG_BASE_OFFS + 0x000a6010)
#define HWIO_GCC_USB30_MP_MASTER_CBCR_RMSK                                                         0x80007ff5
#define HWIO_GCC_USB30_MP_MASTER_CBCR_IN          \
        in_dword(HWIO_GCC_USB30_MP_MASTER_CBCR_ADDR)
#define HWIO_GCC_USB30_MP_MASTER_CBCR_INM(m)      \
        in_dword_masked(HWIO_GCC_USB30_MP_MASTER_CBCR_ADDR, m)
#define HWIO_GCC_USB30_MP_MASTER_CBCR_OUT(v)      \
        out_dword(HWIO_GCC_USB30_MP_MASTER_CBCR_ADDR,v)
#define HWIO_GCC_USB30_MP_MASTER_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GCC_USB30_MP_MASTER_CBCR_ADDR,m,v,HWIO_GCC_USB30_MP_MASTER_CBCR_IN)
#define HWIO_GCC_USB30_MP_MASTER_CBCR_CLK_OFF_BMSK                                                 0x80000000
#define HWIO_GCC_USB30_MP_MASTER_CBCR_CLK_OFF_SHFT                                                       0x1f
#define HWIO_GCC_USB30_MP_MASTER_CBCR_FORCE_MEM_CORE_ON_BMSK                                           0x4000
#define HWIO_GCC_USB30_MP_MASTER_CBCR_FORCE_MEM_CORE_ON_SHFT                                              0xe
#define HWIO_GCC_USB30_MP_MASTER_CBCR_FORCE_MEM_CORE_ON_FORCE_DISABLE_FVAL                                0x0
#define HWIO_GCC_USB30_MP_MASTER_CBCR_FORCE_MEM_CORE_ON_FORCE_ENABLE_FVAL                                 0x1
#define HWIO_GCC_USB30_MP_MASTER_CBCR_FORCE_MEM_PERIPH_ON_BMSK                                         0x2000
#define HWIO_GCC_USB30_MP_MASTER_CBCR_FORCE_MEM_PERIPH_ON_SHFT                                            0xd
#define HWIO_GCC_USB30_MP_MASTER_CBCR_FORCE_MEM_PERIPH_ON_FORCE_DISABLE_FVAL                              0x0
#define HWIO_GCC_USB30_MP_MASTER_CBCR_FORCE_MEM_PERIPH_ON_FORCE_ENABLE_FVAL                               0x1
#define HWIO_GCC_USB30_MP_MASTER_CBCR_FORCE_MEM_PERIPH_OFF_BMSK                                        0x1000
#define HWIO_GCC_USB30_MP_MASTER_CBCR_FORCE_MEM_PERIPH_OFF_SHFT                                           0xc
#define HWIO_GCC_USB30_MP_MASTER_CBCR_FORCE_MEM_PERIPH_OFF_FORCE_DISABLE_FVAL                             0x0
#define HWIO_GCC_USB30_MP_MASTER_CBCR_FORCE_MEM_PERIPH_OFF_FORCE_ENABLE_FVAL                              0x1
#define HWIO_GCC_USB30_MP_MASTER_CBCR_WAKEUP_BMSK                                                       0xf00
#define HWIO_GCC_USB30_MP_MASTER_CBCR_WAKEUP_SHFT                                                         0x8
#define HWIO_GCC_USB30_MP_MASTER_CBCR_WAKEUP_CLOCK0_FVAL                                                  0x0
#define HWIO_GCC_USB30_MP_MASTER_CBCR_WAKEUP_CLOCK1_FVAL                                                  0x1
#define HWIO_GCC_USB30_MP_MASTER_CBCR_WAKEUP_CLOCK2_FVAL                                                  0x2
#define HWIO_GCC_USB30_MP_MASTER_CBCR_WAKEUP_CLOCK3_FVAL                                                  0x3
#define HWIO_GCC_USB30_MP_MASTER_CBCR_WAKEUP_CLOCK4_FVAL                                                  0x4
#define HWIO_GCC_USB30_MP_MASTER_CBCR_WAKEUP_CLOCK5_FVAL                                                  0x5
#define HWIO_GCC_USB30_MP_MASTER_CBCR_WAKEUP_CLOCK6_FVAL                                                  0x6
#define HWIO_GCC_USB30_MP_MASTER_CBCR_WAKEUP_CLOCK7_FVAL                                                  0x7
#define HWIO_GCC_USB30_MP_MASTER_CBCR_WAKEUP_CLOCK8_FVAL                                                  0x8
#define HWIO_GCC_USB30_MP_MASTER_CBCR_WAKEUP_CLOCK9_FVAL                                                  0x9
#define HWIO_GCC_USB30_MP_MASTER_CBCR_WAKEUP_CLOCK10_FVAL                                                 0xa
#define HWIO_GCC_USB30_MP_MASTER_CBCR_WAKEUP_CLOCK11_FVAL                                                 0xb
#define HWIO_GCC_USB30_MP_MASTER_CBCR_WAKEUP_CLOCK12_FVAL                                                 0xc
#define HWIO_GCC_USB30_MP_MASTER_CBCR_WAKEUP_CLOCK13_FVAL                                                 0xd
#define HWIO_GCC_USB30_MP_MASTER_CBCR_WAKEUP_CLOCK14_FVAL                                                 0xe
#define HWIO_GCC_USB30_MP_MASTER_CBCR_WAKEUP_CLOCK15_FVAL                                                 0xf
#define HWIO_GCC_USB30_MP_MASTER_CBCR_SLEEP_BMSK                                                         0xf0
#define HWIO_GCC_USB30_MP_MASTER_CBCR_SLEEP_SHFT                                                          0x4
#define HWIO_GCC_USB30_MP_MASTER_CBCR_SLEEP_CLOCK0_FVAL                                                   0x0
#define HWIO_GCC_USB30_MP_MASTER_CBCR_SLEEP_CLOCK1_FVAL                                                   0x1
#define HWIO_GCC_USB30_MP_MASTER_CBCR_SLEEP_CLOCK2_FVAL                                                   0x2
#define HWIO_GCC_USB30_MP_MASTER_CBCR_SLEEP_CLOCK3_FVAL                                                   0x3
#define HWIO_GCC_USB30_MP_MASTER_CBCR_SLEEP_CLOCK4_FVAL                                                   0x4
#define HWIO_GCC_USB30_MP_MASTER_CBCR_SLEEP_CLOCK5_FVAL                                                   0x5
#define HWIO_GCC_USB30_MP_MASTER_CBCR_SLEEP_CLOCK6_FVAL                                                   0x6
#define HWIO_GCC_USB30_MP_MASTER_CBCR_SLEEP_CLOCK7_FVAL                                                   0x7
#define HWIO_GCC_USB30_MP_MASTER_CBCR_SLEEP_CLOCK8_FVAL                                                   0x8
#define HWIO_GCC_USB30_MP_MASTER_CBCR_SLEEP_CLOCK9_FVAL                                                   0x9
#define HWIO_GCC_USB30_MP_MASTER_CBCR_SLEEP_CLOCK10_FVAL                                                  0xa
#define HWIO_GCC_USB30_MP_MASTER_CBCR_SLEEP_CLOCK11_FVAL                                                  0xb
#define HWIO_GCC_USB30_MP_MASTER_CBCR_SLEEP_CLOCK12_FVAL                                                  0xc
#define HWIO_GCC_USB30_MP_MASTER_CBCR_SLEEP_CLOCK13_FVAL                                                  0xd
#define HWIO_GCC_USB30_MP_MASTER_CBCR_SLEEP_CLOCK14_FVAL                                                  0xe
#define HWIO_GCC_USB30_MP_MASTER_CBCR_SLEEP_CLOCK15_FVAL                                                  0xf
#define HWIO_GCC_USB30_MP_MASTER_CBCR_CLK_ARES_BMSK                                                       0x4
#define HWIO_GCC_USB30_MP_MASTER_CBCR_CLK_ARES_SHFT                                                       0x2
#define HWIO_GCC_USB30_MP_MASTER_CBCR_CLK_ARES_NO_RESET_FVAL                                              0x0
#define HWIO_GCC_USB30_MP_MASTER_CBCR_CLK_ARES_RESET_FVAL                                                 0x1
#define HWIO_GCC_USB30_MP_MASTER_CBCR_CLK_ENABLE_BMSK                                                     0x1
#define HWIO_GCC_USB30_MP_MASTER_CBCR_CLK_ENABLE_SHFT                                                     0x0
#define HWIO_GCC_USB30_MP_MASTER_CBCR_CLK_ENABLE_DISABLE_FVAL                                             0x0
#define HWIO_GCC_USB30_MP_MASTER_CBCR_CLK_ENABLE_ENABLE_FVAL                                              0x1

#define HWIO_GCC_USB30_MP_SLEEP_CBCR_ADDR                                                          (GCC_CLK_CTL_REG_REG_BASE      + 0x000a6014)
#define HWIO_GCC_USB30_MP_SLEEP_CBCR_PHYS                                                          (GCC_CLK_CTL_REG_REG_BASE_PHYS + 0x000a6014)
#define HWIO_GCC_USB30_MP_SLEEP_CBCR_OFFS                                                          (GCC_CLK_CTL_REG_REG_BASE_OFFS + 0x000a6014)
#define HWIO_GCC_USB30_MP_SLEEP_CBCR_RMSK                                                          0x80000005
#define HWIO_GCC_USB30_MP_SLEEP_CBCR_IN          \
        in_dword(HWIO_GCC_USB30_MP_SLEEP_CBCR_ADDR)
#define HWIO_GCC_USB30_MP_SLEEP_CBCR_INM(m)      \
        in_dword_masked(HWIO_GCC_USB30_MP_SLEEP_CBCR_ADDR, m)
#define HWIO_GCC_USB30_MP_SLEEP_CBCR_OUT(v)      \
        out_dword(HWIO_GCC_USB30_MP_SLEEP_CBCR_ADDR,v)
#define HWIO_GCC_USB30_MP_SLEEP_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GCC_USB30_MP_SLEEP_CBCR_ADDR,m,v,HWIO_GCC_USB30_MP_SLEEP_CBCR_IN)
#define HWIO_GCC_USB30_MP_SLEEP_CBCR_CLK_OFF_BMSK                                                  0x80000000
#define HWIO_GCC_USB30_MP_SLEEP_CBCR_CLK_OFF_SHFT                                                        0x1f
#define HWIO_GCC_USB30_MP_SLEEP_CBCR_CLK_ARES_BMSK                                                        0x4
#define HWIO_GCC_USB30_MP_SLEEP_CBCR_CLK_ARES_SHFT                                                        0x2
#define HWIO_GCC_USB30_MP_SLEEP_CBCR_CLK_ARES_NO_RESET_FVAL                                               0x0
#define HWIO_GCC_USB30_MP_SLEEP_CBCR_CLK_ARES_RESET_FVAL                                                  0x1
#define HWIO_GCC_USB30_MP_SLEEP_CBCR_CLK_ENABLE_BMSK                                                      0x1
#define HWIO_GCC_USB30_MP_SLEEP_CBCR_CLK_ENABLE_SHFT                                                      0x0
#define HWIO_GCC_USB30_MP_SLEEP_CBCR_CLK_ENABLE_DISABLE_FVAL                                              0x0
#define HWIO_GCC_USB30_MP_SLEEP_CBCR_CLK_ENABLE_ENABLE_FVAL                                               0x1

#define HWIO_GCC_USB30_MP_MOCK_UTMI_CBCR_ADDR                                                      (GCC_CLK_CTL_REG_REG_BASE      + 0x000a6018)
#define HWIO_GCC_USB30_MP_MOCK_UTMI_CBCR_PHYS                                                      (GCC_CLK_CTL_REG_REG_BASE_PHYS + 0x000a6018)
#define HWIO_GCC_USB30_MP_MOCK_UTMI_CBCR_OFFS                                                      (GCC_CLK_CTL_REG_REG_BASE_OFFS + 0x000a6018)
#define HWIO_GCC_USB30_MP_MOCK_UTMI_CBCR_RMSK                                                      0x80000005
#define HWIO_GCC_USB30_MP_MOCK_UTMI_CBCR_IN          \
        in_dword(HWIO_GCC_USB30_MP_MOCK_UTMI_CBCR_ADDR)
#define HWIO_GCC_USB30_MP_MOCK_UTMI_CBCR_INM(m)      \
        in_dword_masked(HWIO_GCC_USB30_MP_MOCK_UTMI_CBCR_ADDR, m)
#define HWIO_GCC_USB30_MP_MOCK_UTMI_CBCR_OUT(v)      \
        out_dword(HWIO_GCC_USB30_MP_MOCK_UTMI_CBCR_ADDR,v)
#define HWIO_GCC_USB30_MP_MOCK_UTMI_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GCC_USB30_MP_MOCK_UTMI_CBCR_ADDR,m,v,HWIO_GCC_USB30_MP_MOCK_UTMI_CBCR_IN)
#define HWIO_GCC_USB30_MP_MOCK_UTMI_CBCR_CLK_OFF_BMSK                                              0x80000000
#define HWIO_GCC_USB30_MP_MOCK_UTMI_CBCR_CLK_OFF_SHFT                                                    0x1f
#define HWIO_GCC_USB30_MP_MOCK_UTMI_CBCR_CLK_ARES_BMSK                                                    0x4
#define HWIO_GCC_USB30_MP_MOCK_UTMI_CBCR_CLK_ARES_SHFT                                                    0x2
#define HWIO_GCC_USB30_MP_MOCK_UTMI_CBCR_CLK_ARES_NO_RESET_FVAL                                           0x0
#define HWIO_GCC_USB30_MP_MOCK_UTMI_CBCR_CLK_ARES_RESET_FVAL                                              0x1
#define HWIO_GCC_USB30_MP_MOCK_UTMI_CBCR_CLK_ENABLE_BMSK                                                  0x1
#define HWIO_GCC_USB30_MP_MOCK_UTMI_CBCR_CLK_ENABLE_SHFT                                                  0x0
#define HWIO_GCC_USB30_MP_MOCK_UTMI_CBCR_CLK_ENABLE_DISABLE_FVAL                                          0x0
#define HWIO_GCC_USB30_MP_MOCK_UTMI_CBCR_CLK_ENABLE_ENABLE_FVAL                                           0x1

#define HWIO_GCC_USB30_MP_MASTER_CMD_RCGR_ADDR                                                     (GCC_CLK_CTL_REG_REG_BASE      + 0x000a601c)
#define HWIO_GCC_USB30_MP_MASTER_CMD_RCGR_PHYS                                                     (GCC_CLK_CTL_REG_REG_BASE_PHYS + 0x000a601c)
#define HWIO_GCC_USB30_MP_MASTER_CMD_RCGR_OFFS                                                     (GCC_CLK_CTL_REG_REG_BASE_OFFS + 0x000a601c)
#define HWIO_GCC_USB30_MP_MASTER_CMD_RCGR_RMSK                                                     0x800000f3
#define HWIO_GCC_USB30_MP_MASTER_CMD_RCGR_IN          \
        in_dword(HWIO_GCC_USB30_MP_MASTER_CMD_RCGR_ADDR)
#define HWIO_GCC_USB30_MP_MASTER_CMD_RCGR_INM(m)      \
        in_dword_masked(HWIO_GCC_USB30_MP_MASTER_CMD_RCGR_ADDR, m)
#define HWIO_GCC_USB30_MP_MASTER_CMD_RCGR_OUT(v)      \
        out_dword(HWIO_GCC_USB30_MP_MASTER_CMD_RCGR_ADDR,v)
#define HWIO_GCC_USB30_MP_MASTER_CMD_RCGR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GCC_USB30_MP_MASTER_CMD_RCGR_ADDR,m,v,HWIO_GCC_USB30_MP_MASTER_CMD_RCGR_IN)
#define HWIO_GCC_USB30_MP_MASTER_CMD_RCGR_ROOT_OFF_BMSK                                            0x80000000
#define HWIO_GCC_USB30_MP_MASTER_CMD_RCGR_ROOT_OFF_SHFT                                                  0x1f
#define HWIO_GCC_USB30_MP_MASTER_CMD_RCGR_DIRTY_D_BMSK                                                   0x80
#define HWIO_GCC_USB30_MP_MASTER_CMD_RCGR_DIRTY_D_SHFT                                                    0x7
#define HWIO_GCC_USB30_MP_MASTER_CMD_RCGR_DIRTY_N_BMSK                                                   0x40
#define HWIO_GCC_USB30_MP_MASTER_CMD_RCGR_DIRTY_N_SHFT                                                    0x6
#define HWIO_GCC_USB30_MP_MASTER_CMD_RCGR_DIRTY_M_BMSK                                                   0x20
#define HWIO_GCC_USB30_MP_MASTER_CMD_RCGR_DIRTY_M_SHFT                                                    0x5
#define HWIO_GCC_USB30_MP_MASTER_CMD_RCGR_DIRTY_CFG_RCGR_BMSK                                            0x10
#define HWIO_GCC_USB30_MP_MASTER_CMD_RCGR_DIRTY_CFG_RCGR_SHFT                                             0x4
#define HWIO_GCC_USB30_MP_MASTER_CMD_RCGR_ROOT_EN_BMSK                                                    0x2
#define HWIO_GCC_USB30_MP_MASTER_CMD_RCGR_ROOT_EN_SHFT                                                    0x1
#define HWIO_GCC_USB30_MP_MASTER_CMD_RCGR_ROOT_EN_DISABLE_FVAL                                            0x0
#define HWIO_GCC_USB30_MP_MASTER_CMD_RCGR_ROOT_EN_ENABLE_FVAL                                             0x1
#define HWIO_GCC_USB30_MP_MASTER_CMD_RCGR_UPDATE_BMSK                                                     0x1
#define HWIO_GCC_USB30_MP_MASTER_CMD_RCGR_UPDATE_SHFT                                                     0x0
#define HWIO_GCC_USB30_MP_MASTER_CMD_RCGR_UPDATE_DISABLE_FVAL                                             0x0
#define HWIO_GCC_USB30_MP_MASTER_CMD_RCGR_UPDATE_ENABLE_FVAL                                              0x1

#define HWIO_GCC_USB30_MP_MASTER_CFG_RCGR_ADDR                                                     (GCC_CLK_CTL_REG_REG_BASE      + 0x000a6020)
#define HWIO_GCC_USB30_MP_MASTER_CFG_RCGR_PHYS                                                     (GCC_CLK_CTL_REG_REG_BASE_PHYS + 0x000a6020)
#define HWIO_GCC_USB30_MP_MASTER_CFG_RCGR_OFFS                                                     (GCC_CLK_CTL_REG_REG_BASE_OFFS + 0x000a6020)
#define HWIO_GCC_USB30_MP_MASTER_CFG_RCGR_RMSK                                                       0x10371f
#define HWIO_GCC_USB30_MP_MASTER_CFG_RCGR_IN          \
        in_dword(HWIO_GCC_USB30_MP_MASTER_CFG_RCGR_ADDR)
#define HWIO_GCC_USB30_MP_MASTER_CFG_RCGR_INM(m)      \
        in_dword_masked(HWIO_GCC_USB30_MP_MASTER_CFG_RCGR_ADDR, m)
#define HWIO_GCC_USB30_MP_MASTER_CFG_RCGR_OUT(v)      \
        out_dword(HWIO_GCC_USB30_MP_MASTER_CFG_RCGR_ADDR,v)
#define HWIO_GCC_USB30_MP_MASTER_CFG_RCGR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GCC_USB30_MP_MASTER_CFG_RCGR_ADDR,m,v,HWIO_GCC_USB30_MP_MASTER_CFG_RCGR_IN)
#define HWIO_GCC_USB30_MP_MASTER_CFG_RCGR_HW_CLK_CONTROL_BMSK                                        0x100000
#define HWIO_GCC_USB30_MP_MASTER_CFG_RCGR_HW_CLK_CONTROL_SHFT                                            0x14
#define HWIO_GCC_USB30_MP_MASTER_CFG_RCGR_HW_CLK_CONTROL_DISABLE_FVAL                                     0x0
#define HWIO_GCC_USB30_MP_MASTER_CFG_RCGR_HW_CLK_CONTROL_ENABLE_FVAL                                      0x1
#define HWIO_GCC_USB30_MP_MASTER_CFG_RCGR_MODE_BMSK                                                    0x3000
#define HWIO_GCC_USB30_MP_MASTER_CFG_RCGR_MODE_SHFT                                                       0xc
#define HWIO_GCC_USB30_MP_MASTER_CFG_RCGR_MODE_BYPASS_FVAL                                                0x0
#define HWIO_GCC_USB30_MP_MASTER_CFG_RCGR_MODE_SWALLOW_FVAL                                               0x1
#define HWIO_GCC_USB30_MP_MASTER_CFG_RCGR_MODE_DUAL_EDGE_FVAL                                             0x2
#define HWIO_GCC_USB30_MP_MASTER_CFG_RCGR_MODE_SINGLE_EDGE_FVAL                                           0x3
#define HWIO_GCC_USB30_MP_MASTER_CFG_RCGR_SRC_SEL_BMSK                                                  0x700
#define HWIO_GCC_USB30_MP_MASTER_CFG_RCGR_SRC_SEL_SHFT                                                    0x8
#define HWIO_GCC_USB30_MP_MASTER_CFG_RCGR_SRC_SEL_SRC0_FVAL                                               0x0
#define HWIO_GCC_USB30_MP_MASTER_CFG_RCGR_SRC_SEL_SRC1_FVAL                                               0x1
#define HWIO_GCC_USB30_MP_MASTER_CFG_RCGR_SRC_SEL_SRC2_FVAL                                               0x2
#define HWIO_GCC_USB30_MP_MASTER_CFG_RCGR_SRC_SEL_SRC3_FVAL                                               0x3
#define HWIO_GCC_USB30_MP_MASTER_CFG_RCGR_SRC_SEL_SRC4_FVAL                                               0x4
#define HWIO_GCC_USB30_MP_MASTER_CFG_RCGR_SRC_SEL_SRC5_FVAL                                               0x5
#define HWIO_GCC_USB30_MP_MASTER_CFG_RCGR_SRC_SEL_SRC6_FVAL                                               0x6
#define HWIO_GCC_USB30_MP_MASTER_CFG_RCGR_SRC_SEL_SRC7_FVAL                                               0x7
#define HWIO_GCC_USB30_MP_MASTER_CFG_RCGR_SRC_DIV_BMSK                                                   0x1f
#define HWIO_GCC_USB30_MP_MASTER_CFG_RCGR_SRC_DIV_SHFT                                                    0x0
#define HWIO_GCC_USB30_MP_MASTER_CFG_RCGR_SRC_DIV_BYPASS_FVAL                                             0x0
#define HWIO_GCC_USB30_MP_MASTER_CFG_RCGR_SRC_DIV_DIV1_FVAL                                               0x1
#define HWIO_GCC_USB30_MP_MASTER_CFG_RCGR_SRC_DIV_DIV1_5_FVAL                                             0x2
#define HWIO_GCC_USB30_MP_MASTER_CFG_RCGR_SRC_DIV_DIV2_FVAL                                               0x3
#define HWIO_GCC_USB30_MP_MASTER_CFG_RCGR_SRC_DIV_DIV2_5_FVAL                                             0x4
#define HWIO_GCC_USB30_MP_MASTER_CFG_RCGR_SRC_DIV_DIV3_FVAL                                               0x5
#define HWIO_GCC_USB30_MP_MASTER_CFG_RCGR_SRC_DIV_DIV3_5_FVAL                                             0x6
#define HWIO_GCC_USB30_MP_MASTER_CFG_RCGR_SRC_DIV_DIV4_FVAL                                               0x7
#define HWIO_GCC_USB30_MP_MASTER_CFG_RCGR_SRC_DIV_DIV4_5_FVAL                                             0x8
#define HWIO_GCC_USB30_MP_MASTER_CFG_RCGR_SRC_DIV_DIV5_FVAL                                               0x9
#define HWIO_GCC_USB30_MP_MASTER_CFG_RCGR_SRC_DIV_DIV5_5_FVAL                                             0xa
#define HWIO_GCC_USB30_MP_MASTER_CFG_RCGR_SRC_DIV_DIV6_FVAL                                               0xb
#define HWIO_GCC_USB30_MP_MASTER_CFG_RCGR_SRC_DIV_DIV6_5_FVAL                                             0xc
#define HWIO_GCC_USB30_MP_MASTER_CFG_RCGR_SRC_DIV_DIV7_FVAL                                               0xd
#define HWIO_GCC_USB30_MP_MASTER_CFG_RCGR_SRC_DIV_DIV7_5_FVAL                                             0xe
#define HWIO_GCC_USB30_MP_MASTER_CFG_RCGR_SRC_DIV_DIV8_FVAL                                               0xf
#define HWIO_GCC_USB30_MP_MASTER_CFG_RCGR_SRC_DIV_DIV8_5_FVAL                                            0x10
#define HWIO_GCC_USB30_MP_MASTER_CFG_RCGR_SRC_DIV_DIV9_FVAL                                              0x11
#define HWIO_GCC_USB30_MP_MASTER_CFG_RCGR_SRC_DIV_DIV9_5_FVAL                                            0x12
#define HWIO_GCC_USB30_MP_MASTER_CFG_RCGR_SRC_DIV_DIV10_FVAL                                             0x13
#define HWIO_GCC_USB30_MP_MASTER_CFG_RCGR_SRC_DIV_DIV10_5_FVAL                                           0x14
#define HWIO_GCC_USB30_MP_MASTER_CFG_RCGR_SRC_DIV_DIV11_FVAL                                             0x15
#define HWIO_GCC_USB30_MP_MASTER_CFG_RCGR_SRC_DIV_DIV11_5_FVAL                                           0x16
#define HWIO_GCC_USB30_MP_MASTER_CFG_RCGR_SRC_DIV_DIV12_FVAL                                             0x17
#define HWIO_GCC_USB30_MP_MASTER_CFG_RCGR_SRC_DIV_DIV12_5_FVAL                                           0x18
#define HWIO_GCC_USB30_MP_MASTER_CFG_RCGR_SRC_DIV_DIV13_FVAL                                             0x19
#define HWIO_GCC_USB30_MP_MASTER_CFG_RCGR_SRC_DIV_DIV13_5_FVAL                                           0x1a
#define HWIO_GCC_USB30_MP_MASTER_CFG_RCGR_SRC_DIV_DIV14_FVAL                                             0x1b
#define HWIO_GCC_USB30_MP_MASTER_CFG_RCGR_SRC_DIV_DIV14_5_FVAL                                           0x1c
#define HWIO_GCC_USB30_MP_MASTER_CFG_RCGR_SRC_DIV_DIV15_FVAL                                             0x1d
#define HWIO_GCC_USB30_MP_MASTER_CFG_RCGR_SRC_DIV_DIV15_5_FVAL                                           0x1e
#define HWIO_GCC_USB30_MP_MASTER_CFG_RCGR_SRC_DIV_DIV16_FVAL                                             0x1f

#define HWIO_GCC_USB30_MP_MASTER_M_ADDR                                                            (GCC_CLK_CTL_REG_REG_BASE      + 0x000a6024)
#define HWIO_GCC_USB30_MP_MASTER_M_PHYS                                                            (GCC_CLK_CTL_REG_REG_BASE_PHYS + 0x000a6024)
#define HWIO_GCC_USB30_MP_MASTER_M_OFFS                                                            (GCC_CLK_CTL_REG_REG_BASE_OFFS + 0x000a6024)
#define HWIO_GCC_USB30_MP_MASTER_M_RMSK                                                                  0xff
#define HWIO_GCC_USB30_MP_MASTER_M_IN          \
        in_dword(HWIO_GCC_USB30_MP_MASTER_M_ADDR)
#define HWIO_GCC_USB30_MP_MASTER_M_INM(m)      \
        in_dword_masked(HWIO_GCC_USB30_MP_MASTER_M_ADDR, m)
#define HWIO_GCC_USB30_MP_MASTER_M_OUT(v)      \
        out_dword(HWIO_GCC_USB30_MP_MASTER_M_ADDR,v)
#define HWIO_GCC_USB30_MP_MASTER_M_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GCC_USB30_MP_MASTER_M_ADDR,m,v,HWIO_GCC_USB30_MP_MASTER_M_IN)
#define HWIO_GCC_USB30_MP_MASTER_M_M_BMSK                                                                0xff
#define HWIO_GCC_USB30_MP_MASTER_M_M_SHFT                                                                 0x0

#define HWIO_GCC_USB30_MP_MASTER_N_ADDR                                                            (GCC_CLK_CTL_REG_REG_BASE      + 0x000a6028)
#define HWIO_GCC_USB30_MP_MASTER_N_PHYS                                                            (GCC_CLK_CTL_REG_REG_BASE_PHYS + 0x000a6028)
#define HWIO_GCC_USB30_MP_MASTER_N_OFFS                                                            (GCC_CLK_CTL_REG_REG_BASE_OFFS + 0x000a6028)
#define HWIO_GCC_USB30_MP_MASTER_N_RMSK                                                                  0xff
#define HWIO_GCC_USB30_MP_MASTER_N_IN          \
        in_dword(HWIO_GCC_USB30_MP_MASTER_N_ADDR)
#define HWIO_GCC_USB30_MP_MASTER_N_INM(m)      \
        in_dword_masked(HWIO_GCC_USB30_MP_MASTER_N_ADDR, m)
#define HWIO_GCC_USB30_MP_MASTER_N_OUT(v)      \
        out_dword(HWIO_GCC_USB30_MP_MASTER_N_ADDR,v)
#define HWIO_GCC_USB30_MP_MASTER_N_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GCC_USB30_MP_MASTER_N_ADDR,m,v,HWIO_GCC_USB30_MP_MASTER_N_IN)
#define HWIO_GCC_USB30_MP_MASTER_N_NOT_N_MINUS_M_BMSK                                                    0xff
#define HWIO_GCC_USB30_MP_MASTER_N_NOT_N_MINUS_M_SHFT                                                     0x0

#define HWIO_GCC_USB30_MP_MASTER_D_ADDR                                                            (GCC_CLK_CTL_REG_REG_BASE      + 0x000a602c)
#define HWIO_GCC_USB30_MP_MASTER_D_PHYS                                                            (GCC_CLK_CTL_REG_REG_BASE_PHYS + 0x000a602c)
#define HWIO_GCC_USB30_MP_MASTER_D_OFFS                                                            (GCC_CLK_CTL_REG_REG_BASE_OFFS + 0x000a602c)
#define HWIO_GCC_USB30_MP_MASTER_D_RMSK                                                                  0xff
#define HWIO_GCC_USB30_MP_MASTER_D_IN          \
        in_dword(HWIO_GCC_USB30_MP_MASTER_D_ADDR)
#define HWIO_GCC_USB30_MP_MASTER_D_INM(m)      \
        in_dword_masked(HWIO_GCC_USB30_MP_MASTER_D_ADDR, m)
#define HWIO_GCC_USB30_MP_MASTER_D_OUT(v)      \
        out_dword(HWIO_GCC_USB30_MP_MASTER_D_ADDR,v)
#define HWIO_GCC_USB30_MP_MASTER_D_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GCC_USB30_MP_MASTER_D_ADDR,m,v,HWIO_GCC_USB30_MP_MASTER_D_IN)
#define HWIO_GCC_USB30_MP_MASTER_D_NOT_2D_BMSK                                                           0xff
#define HWIO_GCC_USB30_MP_MASTER_D_NOT_2D_SHFT                                                            0x0

#define HWIO_GCC_USB30_MP_MOCK_UTMI_CMD_RCGR_ADDR                                                  (GCC_CLK_CTL_REG_REG_BASE      + 0x000a6034)
#define HWIO_GCC_USB30_MP_MOCK_UTMI_CMD_RCGR_PHYS                                                  (GCC_CLK_CTL_REG_REG_BASE_PHYS + 0x000a6034)
#define HWIO_GCC_USB30_MP_MOCK_UTMI_CMD_RCGR_OFFS                                                  (GCC_CLK_CTL_REG_REG_BASE_OFFS + 0x000a6034)
#define HWIO_GCC_USB30_MP_MOCK_UTMI_CMD_RCGR_RMSK                                                  0x80000013
#define HWIO_GCC_USB30_MP_MOCK_UTMI_CMD_RCGR_IN          \
        in_dword(HWIO_GCC_USB30_MP_MOCK_UTMI_CMD_RCGR_ADDR)
#define HWIO_GCC_USB30_MP_MOCK_UTMI_CMD_RCGR_INM(m)      \
        in_dword_masked(HWIO_GCC_USB30_MP_MOCK_UTMI_CMD_RCGR_ADDR, m)
#define HWIO_GCC_USB30_MP_MOCK_UTMI_CMD_RCGR_OUT(v)      \
        out_dword(HWIO_GCC_USB30_MP_MOCK_UTMI_CMD_RCGR_ADDR,v)
#define HWIO_GCC_USB30_MP_MOCK_UTMI_CMD_RCGR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GCC_USB30_MP_MOCK_UTMI_CMD_RCGR_ADDR,m,v,HWIO_GCC_USB30_MP_MOCK_UTMI_CMD_RCGR_IN)
#define HWIO_GCC_USB30_MP_MOCK_UTMI_CMD_RCGR_ROOT_OFF_BMSK                                         0x80000000
#define HWIO_GCC_USB30_MP_MOCK_UTMI_CMD_RCGR_ROOT_OFF_SHFT                                               0x1f
#define HWIO_GCC_USB30_MP_MOCK_UTMI_CMD_RCGR_DIRTY_CFG_RCGR_BMSK                                         0x10
#define HWIO_GCC_USB30_MP_MOCK_UTMI_CMD_RCGR_DIRTY_CFG_RCGR_SHFT                                          0x4
#define HWIO_GCC_USB30_MP_MOCK_UTMI_CMD_RCGR_ROOT_EN_BMSK                                                 0x2
#define HWIO_GCC_USB30_MP_MOCK_UTMI_CMD_RCGR_ROOT_EN_SHFT                                                 0x1
#define HWIO_GCC_USB30_MP_MOCK_UTMI_CMD_RCGR_ROOT_EN_DISABLE_FVAL                                         0x0
#define HWIO_GCC_USB30_MP_MOCK_UTMI_CMD_RCGR_ROOT_EN_ENABLE_FVAL                                          0x1
#define HWIO_GCC_USB30_MP_MOCK_UTMI_CMD_RCGR_UPDATE_BMSK                                                  0x1
#define HWIO_GCC_USB30_MP_MOCK_UTMI_CMD_RCGR_UPDATE_SHFT                                                  0x0
#define HWIO_GCC_USB30_MP_MOCK_UTMI_CMD_RCGR_UPDATE_DISABLE_FVAL                                          0x0
#define HWIO_GCC_USB30_MP_MOCK_UTMI_CMD_RCGR_UPDATE_ENABLE_FVAL                                           0x1

#define HWIO_GCC_USB30_MP_MOCK_UTMI_CFG_RCGR_ADDR                                                  (GCC_CLK_CTL_REG_REG_BASE      + 0x000a6038)
#define HWIO_GCC_USB30_MP_MOCK_UTMI_CFG_RCGR_PHYS                                                  (GCC_CLK_CTL_REG_REG_BASE_PHYS + 0x000a6038)
#define HWIO_GCC_USB30_MP_MOCK_UTMI_CFG_RCGR_OFFS                                                  (GCC_CLK_CTL_REG_REG_BASE_OFFS + 0x000a6038)
#define HWIO_GCC_USB30_MP_MOCK_UTMI_CFG_RCGR_RMSK                                                    0x11071f
#define HWIO_GCC_USB30_MP_MOCK_UTMI_CFG_RCGR_IN          \
        in_dword(HWIO_GCC_USB30_MP_MOCK_UTMI_CFG_RCGR_ADDR)
#define HWIO_GCC_USB30_MP_MOCK_UTMI_CFG_RCGR_INM(m)      \
        in_dword_masked(HWIO_GCC_USB30_MP_MOCK_UTMI_CFG_RCGR_ADDR, m)
#define HWIO_GCC_USB30_MP_MOCK_UTMI_CFG_RCGR_OUT(v)      \
        out_dword(HWIO_GCC_USB30_MP_MOCK_UTMI_CFG_RCGR_ADDR,v)
#define HWIO_GCC_USB30_MP_MOCK_UTMI_CFG_RCGR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GCC_USB30_MP_MOCK_UTMI_CFG_RCGR_ADDR,m,v,HWIO_GCC_USB30_MP_MOCK_UTMI_CFG_RCGR_IN)
#define HWIO_GCC_USB30_MP_MOCK_UTMI_CFG_RCGR_HW_CLK_CONTROL_BMSK                                     0x100000
#define HWIO_GCC_USB30_MP_MOCK_UTMI_CFG_RCGR_HW_CLK_CONTROL_SHFT                                         0x14
#define HWIO_GCC_USB30_MP_MOCK_UTMI_CFG_RCGR_HW_CLK_CONTROL_DISABLE_FVAL                                  0x0
#define HWIO_GCC_USB30_MP_MOCK_UTMI_CFG_RCGR_HW_CLK_CONTROL_ENABLE_FVAL                                   0x1
#define HWIO_GCC_USB30_MP_MOCK_UTMI_CFG_RCGR_RCGLITE_DISABLE_BMSK                                     0x10000
#define HWIO_GCC_USB30_MP_MOCK_UTMI_CFG_RCGR_RCGLITE_DISABLE_SHFT                                        0x10
#define HWIO_GCC_USB30_MP_MOCK_UTMI_CFG_RCGR_RCGLITE_DISABLE_RCGLITE_ENABLED_FVAL                         0x0
#define HWIO_GCC_USB30_MP_MOCK_UTMI_CFG_RCGR_RCGLITE_DISABLE_RCGLITE_DISABLED_FVAL                        0x1
#define HWIO_GCC_USB30_MP_MOCK_UTMI_CFG_RCGR_SRC_SEL_BMSK                                               0x700
#define HWIO_GCC_USB30_MP_MOCK_UTMI_CFG_RCGR_SRC_SEL_SHFT                                                 0x8
#define HWIO_GCC_USB30_MP_MOCK_UTMI_CFG_RCGR_SRC_SEL_SRC0_FVAL                                            0x0
#define HWIO_GCC_USB30_MP_MOCK_UTMI_CFG_RCGR_SRC_SEL_SRC1_FVAL                                            0x1
#define HWIO_GCC_USB30_MP_MOCK_UTMI_CFG_RCGR_SRC_SEL_SRC2_FVAL                                            0x2
#define HWIO_GCC_USB30_MP_MOCK_UTMI_CFG_RCGR_SRC_SEL_SRC3_FVAL                                            0x3
#define HWIO_GCC_USB30_MP_MOCK_UTMI_CFG_RCGR_SRC_SEL_SRC4_FVAL                                            0x4
#define HWIO_GCC_USB30_MP_MOCK_UTMI_CFG_RCGR_SRC_SEL_SRC5_FVAL                                            0x5
#define HWIO_GCC_USB30_MP_MOCK_UTMI_CFG_RCGR_SRC_SEL_SRC6_FVAL                                            0x6
#define HWIO_GCC_USB30_MP_MOCK_UTMI_CFG_RCGR_SRC_SEL_SRC7_FVAL                                            0x7
#define HWIO_GCC_USB30_MP_MOCK_UTMI_CFG_RCGR_SRC_DIV_BMSK                                                0x1f
#define HWIO_GCC_USB30_MP_MOCK_UTMI_CFG_RCGR_SRC_DIV_SHFT                                                 0x0
#define HWIO_GCC_USB30_MP_MOCK_UTMI_CFG_RCGR_SRC_DIV_BYPASS_FVAL                                          0x0
#define HWIO_GCC_USB30_MP_MOCK_UTMI_CFG_RCGR_SRC_DIV_DIV1_FVAL                                            0x1
#define HWIO_GCC_USB30_MP_MOCK_UTMI_CFG_RCGR_SRC_DIV_DIV1_5_FVAL                                          0x2
#define HWIO_GCC_USB30_MP_MOCK_UTMI_CFG_RCGR_SRC_DIV_DIV2_FVAL                                            0x3
#define HWIO_GCC_USB30_MP_MOCK_UTMI_CFG_RCGR_SRC_DIV_DIV2_5_FVAL                                          0x4
#define HWIO_GCC_USB30_MP_MOCK_UTMI_CFG_RCGR_SRC_DIV_DIV3_FVAL                                            0x5
#define HWIO_GCC_USB30_MP_MOCK_UTMI_CFG_RCGR_SRC_DIV_DIV3_5_FVAL                                          0x6
#define HWIO_GCC_USB30_MP_MOCK_UTMI_CFG_RCGR_SRC_DIV_DIV4_FVAL                                            0x7
#define HWIO_GCC_USB30_MP_MOCK_UTMI_CFG_RCGR_SRC_DIV_DIV4_5_FVAL                                          0x8
#define HWIO_GCC_USB30_MP_MOCK_UTMI_CFG_RCGR_SRC_DIV_DIV5_FVAL                                            0x9
#define HWIO_GCC_USB30_MP_MOCK_UTMI_CFG_RCGR_SRC_DIV_DIV5_5_FVAL                                          0xa
#define HWIO_GCC_USB30_MP_MOCK_UTMI_CFG_RCGR_SRC_DIV_DIV6_FVAL                                            0xb
#define HWIO_GCC_USB30_MP_MOCK_UTMI_CFG_RCGR_SRC_DIV_DIV6_5_FVAL                                          0xc
#define HWIO_GCC_USB30_MP_MOCK_UTMI_CFG_RCGR_SRC_DIV_DIV7_FVAL                                            0xd
#define HWIO_GCC_USB30_MP_MOCK_UTMI_CFG_RCGR_SRC_DIV_DIV7_5_FVAL                                          0xe
#define HWIO_GCC_USB30_MP_MOCK_UTMI_CFG_RCGR_SRC_DIV_DIV8_FVAL                                            0xf
#define HWIO_GCC_USB30_MP_MOCK_UTMI_CFG_RCGR_SRC_DIV_DIV8_5_FVAL                                         0x10
#define HWIO_GCC_USB30_MP_MOCK_UTMI_CFG_RCGR_SRC_DIV_DIV9_FVAL                                           0x11
#define HWIO_GCC_USB30_MP_MOCK_UTMI_CFG_RCGR_SRC_DIV_DIV9_5_FVAL                                         0x12
#define HWIO_GCC_USB30_MP_MOCK_UTMI_CFG_RCGR_SRC_DIV_DIV10_FVAL                                          0x13
#define HWIO_GCC_USB30_MP_MOCK_UTMI_CFG_RCGR_SRC_DIV_DIV10_5_FVAL                                        0x14
#define HWIO_GCC_USB30_MP_MOCK_UTMI_CFG_RCGR_SRC_DIV_DIV11_FVAL                                          0x15
#define HWIO_GCC_USB30_MP_MOCK_UTMI_CFG_RCGR_SRC_DIV_DIV11_5_FVAL                                        0x16
#define HWIO_GCC_USB30_MP_MOCK_UTMI_CFG_RCGR_SRC_DIV_DIV12_FVAL                                          0x17
#define HWIO_GCC_USB30_MP_MOCK_UTMI_CFG_RCGR_SRC_DIV_DIV12_5_FVAL                                        0x18
#define HWIO_GCC_USB30_MP_MOCK_UTMI_CFG_RCGR_SRC_DIV_DIV13_FVAL                                          0x19
#define HWIO_GCC_USB30_MP_MOCK_UTMI_CFG_RCGR_SRC_DIV_DIV13_5_FVAL                                        0x1a
#define HWIO_GCC_USB30_MP_MOCK_UTMI_CFG_RCGR_SRC_DIV_DIV14_FVAL                                          0x1b
#define HWIO_GCC_USB30_MP_MOCK_UTMI_CFG_RCGR_SRC_DIV_DIV14_5_FVAL                                        0x1c
#define HWIO_GCC_USB30_MP_MOCK_UTMI_CFG_RCGR_SRC_DIV_DIV15_FVAL                                          0x1d
#define HWIO_GCC_USB30_MP_MOCK_UTMI_CFG_RCGR_SRC_DIV_DIV15_5_FVAL                                        0x1e
#define HWIO_GCC_USB30_MP_MOCK_UTMI_CFG_RCGR_SRC_DIV_DIV16_FVAL                                          0x1f

#define HWIO_GCC_USB30_MP_MOCK_UTMI_POSTDIV_CDIVR_ADDR                                             (GCC_CLK_CTL_REG_REG_BASE      + 0x000a604c)
#define HWIO_GCC_USB30_MP_MOCK_UTMI_POSTDIV_CDIVR_PHYS                                             (GCC_CLK_CTL_REG_REG_BASE_PHYS + 0x000a604c)
#define HWIO_GCC_USB30_MP_MOCK_UTMI_POSTDIV_CDIVR_OFFS                                             (GCC_CLK_CTL_REG_REG_BASE_OFFS + 0x000a604c)
#define HWIO_GCC_USB30_MP_MOCK_UTMI_POSTDIV_CDIVR_RMSK                                                    0x3
#define HWIO_GCC_USB30_MP_MOCK_UTMI_POSTDIV_CDIVR_IN          \
        in_dword(HWIO_GCC_USB30_MP_MOCK_UTMI_POSTDIV_CDIVR_ADDR)
#define HWIO_GCC_USB30_MP_MOCK_UTMI_POSTDIV_CDIVR_INM(m)      \
        in_dword_masked(HWIO_GCC_USB30_MP_MOCK_UTMI_POSTDIV_CDIVR_ADDR, m)
#define HWIO_GCC_USB30_MP_MOCK_UTMI_POSTDIV_CDIVR_OUT(v)      \
        out_dword(HWIO_GCC_USB30_MP_MOCK_UTMI_POSTDIV_CDIVR_ADDR,v)
#define HWIO_GCC_USB30_MP_MOCK_UTMI_POSTDIV_CDIVR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GCC_USB30_MP_MOCK_UTMI_POSTDIV_CDIVR_ADDR,m,v,HWIO_GCC_USB30_MP_MOCK_UTMI_POSTDIV_CDIVR_IN)
#define HWIO_GCC_USB30_MP_MOCK_UTMI_POSTDIV_CDIVR_CLK_DIV_BMSK                                            0x3
#define HWIO_GCC_USB30_MP_MOCK_UTMI_POSTDIV_CDIVR_CLK_DIV_SHFT                                            0x0

#define HWIO_GCC_USB3_MP_PHY_AUX_CBCR_ADDR                                                         (GCC_CLK_CTL_REG_REG_BASE      + 0x000a6050)
#define HWIO_GCC_USB3_MP_PHY_AUX_CBCR_PHYS                                                         (GCC_CLK_CTL_REG_REG_BASE_PHYS + 0x000a6050)
#define HWIO_GCC_USB3_MP_PHY_AUX_CBCR_OFFS                                                         (GCC_CLK_CTL_REG_REG_BASE_OFFS + 0x000a6050)
#define HWIO_GCC_USB3_MP_PHY_AUX_CBCR_RMSK                                                         0x80000005
#define HWIO_GCC_USB3_MP_PHY_AUX_CBCR_IN          \
        in_dword(HWIO_GCC_USB3_MP_PHY_AUX_CBCR_ADDR)
#define HWIO_GCC_USB3_MP_PHY_AUX_CBCR_INM(m)      \
        in_dword_masked(HWIO_GCC_USB3_MP_PHY_AUX_CBCR_ADDR, m)
#define HWIO_GCC_USB3_MP_PHY_AUX_CBCR_OUT(v)      \
        out_dword(HWIO_GCC_USB3_MP_PHY_AUX_CBCR_ADDR,v)
#define HWIO_GCC_USB3_MP_PHY_AUX_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GCC_USB3_MP_PHY_AUX_CBCR_ADDR,m,v,HWIO_GCC_USB3_MP_PHY_AUX_CBCR_IN)
#define HWIO_GCC_USB3_MP_PHY_AUX_CBCR_CLK_OFF_BMSK                                                 0x80000000
#define HWIO_GCC_USB3_MP_PHY_AUX_CBCR_CLK_OFF_SHFT                                                       0x1f
#define HWIO_GCC_USB3_MP_PHY_AUX_CBCR_CLK_ARES_BMSK                                                       0x4
#define HWIO_GCC_USB3_MP_PHY_AUX_CBCR_CLK_ARES_SHFT                                                       0x2
#define HWIO_GCC_USB3_MP_PHY_AUX_CBCR_CLK_ARES_NO_RESET_FVAL                                              0x0
#define HWIO_GCC_USB3_MP_PHY_AUX_CBCR_CLK_ARES_RESET_FVAL                                                 0x1
#define HWIO_GCC_USB3_MP_PHY_AUX_CBCR_CLK_ENABLE_BMSK                                                     0x1
#define HWIO_GCC_USB3_MP_PHY_AUX_CBCR_CLK_ENABLE_SHFT                                                     0x0
#define HWIO_GCC_USB3_MP_PHY_AUX_CBCR_CLK_ENABLE_DISABLE_FVAL                                             0x0
#define HWIO_GCC_USB3_MP_PHY_AUX_CBCR_CLK_ENABLE_ENABLE_FVAL                                              0x1

#define HWIO_GCC_USB3_MP_PHY_COM_AUX_CBCR_ADDR                                                     (GCC_CLK_CTL_REG_REG_BASE      + 0x000a6054)
#define HWIO_GCC_USB3_MP_PHY_COM_AUX_CBCR_PHYS                                                     (GCC_CLK_CTL_REG_REG_BASE_PHYS + 0x000a6054)
#define HWIO_GCC_USB3_MP_PHY_COM_AUX_CBCR_OFFS                                                     (GCC_CLK_CTL_REG_REG_BASE_OFFS + 0x000a6054)
#define HWIO_GCC_USB3_MP_PHY_COM_AUX_CBCR_RMSK                                                     0x80000005
#define HWIO_GCC_USB3_MP_PHY_COM_AUX_CBCR_IN          \
        in_dword(HWIO_GCC_USB3_MP_PHY_COM_AUX_CBCR_ADDR)
#define HWIO_GCC_USB3_MP_PHY_COM_AUX_CBCR_INM(m)      \
        in_dword_masked(HWIO_GCC_USB3_MP_PHY_COM_AUX_CBCR_ADDR, m)
#define HWIO_GCC_USB3_MP_PHY_COM_AUX_CBCR_OUT(v)      \
        out_dword(HWIO_GCC_USB3_MP_PHY_COM_AUX_CBCR_ADDR,v)
#define HWIO_GCC_USB3_MP_PHY_COM_AUX_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GCC_USB3_MP_PHY_COM_AUX_CBCR_ADDR,m,v,HWIO_GCC_USB3_MP_PHY_COM_AUX_CBCR_IN)
#define HWIO_GCC_USB3_MP_PHY_COM_AUX_CBCR_CLK_OFF_BMSK                                             0x80000000
#define HWIO_GCC_USB3_MP_PHY_COM_AUX_CBCR_CLK_OFF_SHFT                                                   0x1f
#define HWIO_GCC_USB3_MP_PHY_COM_AUX_CBCR_CLK_ARES_BMSK                                                   0x4
#define HWIO_GCC_USB3_MP_PHY_COM_AUX_CBCR_CLK_ARES_SHFT                                                   0x2
#define HWIO_GCC_USB3_MP_PHY_COM_AUX_CBCR_CLK_ARES_NO_RESET_FVAL                                          0x0
#define HWIO_GCC_USB3_MP_PHY_COM_AUX_CBCR_CLK_ARES_RESET_FVAL                                             0x1
#define HWIO_GCC_USB3_MP_PHY_COM_AUX_CBCR_CLK_ENABLE_BMSK                                                 0x1
#define HWIO_GCC_USB3_MP_PHY_COM_AUX_CBCR_CLK_ENABLE_SHFT                                                 0x0
#define HWIO_GCC_USB3_MP_PHY_COM_AUX_CBCR_CLK_ENABLE_DISABLE_FVAL                                         0x0
#define HWIO_GCC_USB3_MP_PHY_COM_AUX_CBCR_CLK_ENABLE_ENABLE_FVAL                                          0x1

#define HWIO_GCC_USB3_MP_PHY_PIPE_0_CBCR_ADDR                                                      (GCC_CLK_CTL_REG_REG_BASE      + 0x000a6058)
#define HWIO_GCC_USB3_MP_PHY_PIPE_0_CBCR_PHYS                                                      (GCC_CLK_CTL_REG_REG_BASE_PHYS + 0x000a6058)
#define HWIO_GCC_USB3_MP_PHY_PIPE_0_CBCR_OFFS                                                      (GCC_CLK_CTL_REG_REG_BASE_OFFS + 0x000a6058)
#define HWIO_GCC_USB3_MP_PHY_PIPE_0_CBCR_RMSK                                                      0x80000005
#define HWIO_GCC_USB3_MP_PHY_PIPE_0_CBCR_IN          \
        in_dword(HWIO_GCC_USB3_MP_PHY_PIPE_0_CBCR_ADDR)
#define HWIO_GCC_USB3_MP_PHY_PIPE_0_CBCR_INM(m)      \
        in_dword_masked(HWIO_GCC_USB3_MP_PHY_PIPE_0_CBCR_ADDR, m)
#define HWIO_GCC_USB3_MP_PHY_PIPE_0_CBCR_OUT(v)      \
        out_dword(HWIO_GCC_USB3_MP_PHY_PIPE_0_CBCR_ADDR,v)
#define HWIO_GCC_USB3_MP_PHY_PIPE_0_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GCC_USB3_MP_PHY_PIPE_0_CBCR_ADDR,m,v,HWIO_GCC_USB3_MP_PHY_PIPE_0_CBCR_IN)
#define HWIO_GCC_USB3_MP_PHY_PIPE_0_CBCR_CLK_OFF_BMSK                                              0x80000000
#define HWIO_GCC_USB3_MP_PHY_PIPE_0_CBCR_CLK_OFF_SHFT                                                    0x1f
#define HWIO_GCC_USB3_MP_PHY_PIPE_0_CBCR_CLK_ARES_BMSK                                                    0x4
#define HWIO_GCC_USB3_MP_PHY_PIPE_0_CBCR_CLK_ARES_SHFT                                                    0x2
#define HWIO_GCC_USB3_MP_PHY_PIPE_0_CBCR_CLK_ARES_NO_RESET_FVAL                                           0x0
#define HWIO_GCC_USB3_MP_PHY_PIPE_0_CBCR_CLK_ARES_RESET_FVAL                                              0x1
#define HWIO_GCC_USB3_MP_PHY_PIPE_0_CBCR_CLK_ENABLE_BMSK                                                  0x1
#define HWIO_GCC_USB3_MP_PHY_PIPE_0_CBCR_CLK_ENABLE_SHFT                                                  0x0
#define HWIO_GCC_USB3_MP_PHY_PIPE_0_CBCR_CLK_ENABLE_DISABLE_FVAL                                          0x0
#define HWIO_GCC_USB3_MP_PHY_PIPE_0_CBCR_CLK_ENABLE_ENABLE_FVAL                                           0x1

#define HWIO_GCC_USB3_MP_PHY_PIPE_1_CBCR_ADDR                                                      (GCC_CLK_CTL_REG_REG_BASE      + 0x000a605c)
#define HWIO_GCC_USB3_MP_PHY_PIPE_1_CBCR_PHYS                                                      (GCC_CLK_CTL_REG_REG_BASE_PHYS + 0x000a605c)
#define HWIO_GCC_USB3_MP_PHY_PIPE_1_CBCR_OFFS                                                      (GCC_CLK_CTL_REG_REG_BASE_OFFS + 0x000a605c)
#define HWIO_GCC_USB3_MP_PHY_PIPE_1_CBCR_RMSK                                                      0x80000005
#define HWIO_GCC_USB3_MP_PHY_PIPE_1_CBCR_IN          \
        in_dword(HWIO_GCC_USB3_MP_PHY_PIPE_1_CBCR_ADDR)
#define HWIO_GCC_USB3_MP_PHY_PIPE_1_CBCR_INM(m)      \
        in_dword_masked(HWIO_GCC_USB3_MP_PHY_PIPE_1_CBCR_ADDR, m)
#define HWIO_GCC_USB3_MP_PHY_PIPE_1_CBCR_OUT(v)      \
        out_dword(HWIO_GCC_USB3_MP_PHY_PIPE_1_CBCR_ADDR,v)
#define HWIO_GCC_USB3_MP_PHY_PIPE_1_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GCC_USB3_MP_PHY_PIPE_1_CBCR_ADDR,m,v,HWIO_GCC_USB3_MP_PHY_PIPE_1_CBCR_IN)
#define HWIO_GCC_USB3_MP_PHY_PIPE_1_CBCR_CLK_OFF_BMSK                                              0x80000000
#define HWIO_GCC_USB3_MP_PHY_PIPE_1_CBCR_CLK_OFF_SHFT                                                    0x1f
#define HWIO_GCC_USB3_MP_PHY_PIPE_1_CBCR_CLK_ARES_BMSK                                                    0x4
#define HWIO_GCC_USB3_MP_PHY_PIPE_1_CBCR_CLK_ARES_SHFT                                                    0x2
#define HWIO_GCC_USB3_MP_PHY_PIPE_1_CBCR_CLK_ARES_NO_RESET_FVAL                                           0x0
#define HWIO_GCC_USB3_MP_PHY_PIPE_1_CBCR_CLK_ARES_RESET_FVAL                                              0x1
#define HWIO_GCC_USB3_MP_PHY_PIPE_1_CBCR_CLK_ENABLE_BMSK                                                  0x1
#define HWIO_GCC_USB3_MP_PHY_PIPE_1_CBCR_CLK_ENABLE_SHFT                                                  0x0
#define HWIO_GCC_USB3_MP_PHY_PIPE_1_CBCR_CLK_ENABLE_DISABLE_FVAL                                          0x0
#define HWIO_GCC_USB3_MP_PHY_PIPE_1_CBCR_CLK_ENABLE_ENABLE_FVAL                                           0x1

#define HWIO_GCC_USB3_MP_PHY_AUX_CMD_RCGR_ADDR                                                     (GCC_CLK_CTL_REG_REG_BASE      + 0x000a6068)
#define HWIO_GCC_USB3_MP_PHY_AUX_CMD_RCGR_PHYS                                                     (GCC_CLK_CTL_REG_REG_BASE_PHYS + 0x000a6068)
#define HWIO_GCC_USB3_MP_PHY_AUX_CMD_RCGR_OFFS                                                     (GCC_CLK_CTL_REG_REG_BASE_OFFS + 0x000a6068)
#define HWIO_GCC_USB3_MP_PHY_AUX_CMD_RCGR_RMSK                                                     0x80000013
#define HWIO_GCC_USB3_MP_PHY_AUX_CMD_RCGR_IN          \
        in_dword(HWIO_GCC_USB3_MP_PHY_AUX_CMD_RCGR_ADDR)
#define HWIO_GCC_USB3_MP_PHY_AUX_CMD_RCGR_INM(m)      \
        in_dword_masked(HWIO_GCC_USB3_MP_PHY_AUX_CMD_RCGR_ADDR, m)
#define HWIO_GCC_USB3_MP_PHY_AUX_CMD_RCGR_OUT(v)      \
        out_dword(HWIO_GCC_USB3_MP_PHY_AUX_CMD_RCGR_ADDR,v)
#define HWIO_GCC_USB3_MP_PHY_AUX_CMD_RCGR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GCC_USB3_MP_PHY_AUX_CMD_RCGR_ADDR,m,v,HWIO_GCC_USB3_MP_PHY_AUX_CMD_RCGR_IN)
#define HWIO_GCC_USB3_MP_PHY_AUX_CMD_RCGR_ROOT_OFF_BMSK                                            0x80000000
#define HWIO_GCC_USB3_MP_PHY_AUX_CMD_RCGR_ROOT_OFF_SHFT                                                  0x1f
#define HWIO_GCC_USB3_MP_PHY_AUX_CMD_RCGR_DIRTY_CFG_RCGR_BMSK                                            0x10
#define HWIO_GCC_USB3_MP_PHY_AUX_CMD_RCGR_DIRTY_CFG_RCGR_SHFT                                             0x4
#define HWIO_GCC_USB3_MP_PHY_AUX_CMD_RCGR_ROOT_EN_BMSK                                                    0x2
#define HWIO_GCC_USB3_MP_PHY_AUX_CMD_RCGR_ROOT_EN_SHFT                                                    0x1
#define HWIO_GCC_USB3_MP_PHY_AUX_CMD_RCGR_ROOT_EN_DISABLE_FVAL                                            0x0
#define HWIO_GCC_USB3_MP_PHY_AUX_CMD_RCGR_ROOT_EN_ENABLE_FVAL                                             0x1
#define HWIO_GCC_USB3_MP_PHY_AUX_CMD_RCGR_UPDATE_BMSK                                                     0x1
#define HWIO_GCC_USB3_MP_PHY_AUX_CMD_RCGR_UPDATE_SHFT                                                     0x0
#define HWIO_GCC_USB3_MP_PHY_AUX_CMD_RCGR_UPDATE_DISABLE_FVAL                                             0x0
#define HWIO_GCC_USB3_MP_PHY_AUX_CMD_RCGR_UPDATE_ENABLE_FVAL                                              0x1

#define HWIO_GCC_USB3_MP_PHY_AUX_CFG_RCGR_ADDR                                                     (GCC_CLK_CTL_REG_REG_BASE      + 0x000a606c)
#define HWIO_GCC_USB3_MP_PHY_AUX_CFG_RCGR_PHYS                                                     (GCC_CLK_CTL_REG_REG_BASE_PHYS + 0x000a606c)
#define HWIO_GCC_USB3_MP_PHY_AUX_CFG_RCGR_OFFS                                                     (GCC_CLK_CTL_REG_REG_BASE_OFFS + 0x000a606c)
#define HWIO_GCC_USB3_MP_PHY_AUX_CFG_RCGR_RMSK                                                       0x11071f
#define HWIO_GCC_USB3_MP_PHY_AUX_CFG_RCGR_IN          \
        in_dword(HWIO_GCC_USB3_MP_PHY_AUX_CFG_RCGR_ADDR)
#define HWIO_GCC_USB3_MP_PHY_AUX_CFG_RCGR_INM(m)      \
        in_dword_masked(HWIO_GCC_USB3_MP_PHY_AUX_CFG_RCGR_ADDR, m)
#define HWIO_GCC_USB3_MP_PHY_AUX_CFG_RCGR_OUT(v)      \
        out_dword(HWIO_GCC_USB3_MP_PHY_AUX_CFG_RCGR_ADDR,v)
#define HWIO_GCC_USB3_MP_PHY_AUX_CFG_RCGR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GCC_USB3_MP_PHY_AUX_CFG_RCGR_ADDR,m,v,HWIO_GCC_USB3_MP_PHY_AUX_CFG_RCGR_IN)
#define HWIO_GCC_USB3_MP_PHY_AUX_CFG_RCGR_HW_CLK_CONTROL_BMSK                                        0x100000
#define HWIO_GCC_USB3_MP_PHY_AUX_CFG_RCGR_HW_CLK_CONTROL_SHFT                                            0x14
#define HWIO_GCC_USB3_MP_PHY_AUX_CFG_RCGR_HW_CLK_CONTROL_DISABLE_FVAL                                     0x0
#define HWIO_GCC_USB3_MP_PHY_AUX_CFG_RCGR_HW_CLK_CONTROL_ENABLE_FVAL                                      0x1
#define HWIO_GCC_USB3_MP_PHY_AUX_CFG_RCGR_RCGLITE_DISABLE_BMSK                                        0x10000
#define HWIO_GCC_USB3_MP_PHY_AUX_CFG_RCGR_RCGLITE_DISABLE_SHFT                                           0x10
#define HWIO_GCC_USB3_MP_PHY_AUX_CFG_RCGR_RCGLITE_DISABLE_RCGLITE_ENABLED_FVAL                            0x0
#define HWIO_GCC_USB3_MP_PHY_AUX_CFG_RCGR_RCGLITE_DISABLE_RCGLITE_DISABLED_FVAL                           0x1
#define HWIO_GCC_USB3_MP_PHY_AUX_CFG_RCGR_SRC_SEL_BMSK                                                  0x700
#define HWIO_GCC_USB3_MP_PHY_AUX_CFG_RCGR_SRC_SEL_SHFT                                                    0x8
#define HWIO_GCC_USB3_MP_PHY_AUX_CFG_RCGR_SRC_SEL_SRC0_FVAL                                               0x0
#define HWIO_GCC_USB3_MP_PHY_AUX_CFG_RCGR_SRC_SEL_SRC1_FVAL                                               0x1
#define HWIO_GCC_USB3_MP_PHY_AUX_CFG_RCGR_SRC_SEL_SRC2_FVAL                                               0x2
#define HWIO_GCC_USB3_MP_PHY_AUX_CFG_RCGR_SRC_SEL_SRC3_FVAL                                               0x3
#define HWIO_GCC_USB3_MP_PHY_AUX_CFG_RCGR_SRC_SEL_SRC4_FVAL                                               0x4
#define HWIO_GCC_USB3_MP_PHY_AUX_CFG_RCGR_SRC_SEL_SRC5_FVAL                                               0x5
#define HWIO_GCC_USB3_MP_PHY_AUX_CFG_RCGR_SRC_SEL_SRC6_FVAL                                               0x6
#define HWIO_GCC_USB3_MP_PHY_AUX_CFG_RCGR_SRC_SEL_SRC7_FVAL                                               0x7
#define HWIO_GCC_USB3_MP_PHY_AUX_CFG_RCGR_SRC_DIV_BMSK                                                   0x1f
#define HWIO_GCC_USB3_MP_PHY_AUX_CFG_RCGR_SRC_DIV_SHFT                                                    0x0
#define HWIO_GCC_USB3_MP_PHY_AUX_CFG_RCGR_SRC_DIV_BYPASS_FVAL                                             0x0
#define HWIO_GCC_USB3_MP_PHY_AUX_CFG_RCGR_SRC_DIV_DIV1_FVAL                                               0x1
#define HWIO_GCC_USB3_MP_PHY_AUX_CFG_RCGR_SRC_DIV_DIV1_5_FVAL                                             0x2
#define HWIO_GCC_USB3_MP_PHY_AUX_CFG_RCGR_SRC_DIV_DIV2_FVAL                                               0x3
#define HWIO_GCC_USB3_MP_PHY_AUX_CFG_RCGR_SRC_DIV_DIV2_5_FVAL                                             0x4
#define HWIO_GCC_USB3_MP_PHY_AUX_CFG_RCGR_SRC_DIV_DIV3_FVAL                                               0x5
#define HWIO_GCC_USB3_MP_PHY_AUX_CFG_RCGR_SRC_DIV_DIV3_5_FVAL                                             0x6
#define HWIO_GCC_USB3_MP_PHY_AUX_CFG_RCGR_SRC_DIV_DIV4_FVAL                                               0x7
#define HWIO_GCC_USB3_MP_PHY_AUX_CFG_RCGR_SRC_DIV_DIV4_5_FVAL                                             0x8
#define HWIO_GCC_USB3_MP_PHY_AUX_CFG_RCGR_SRC_DIV_DIV5_FVAL                                               0x9
#define HWIO_GCC_USB3_MP_PHY_AUX_CFG_RCGR_SRC_DIV_DIV5_5_FVAL                                             0xa
#define HWIO_GCC_USB3_MP_PHY_AUX_CFG_RCGR_SRC_DIV_DIV6_FVAL                                               0xb
#define HWIO_GCC_USB3_MP_PHY_AUX_CFG_RCGR_SRC_DIV_DIV6_5_FVAL                                             0xc
#define HWIO_GCC_USB3_MP_PHY_AUX_CFG_RCGR_SRC_DIV_DIV7_FVAL                                               0xd
#define HWIO_GCC_USB3_MP_PHY_AUX_CFG_RCGR_SRC_DIV_DIV7_5_FVAL                                             0xe
#define HWIO_GCC_USB3_MP_PHY_AUX_CFG_RCGR_SRC_DIV_DIV8_FVAL                                               0xf
#define HWIO_GCC_USB3_MP_PHY_AUX_CFG_RCGR_SRC_DIV_DIV8_5_FVAL                                            0x10
#define HWIO_GCC_USB3_MP_PHY_AUX_CFG_RCGR_SRC_DIV_DIV9_FVAL                                              0x11
#define HWIO_GCC_USB3_MP_PHY_AUX_CFG_RCGR_SRC_DIV_DIV9_5_FVAL                                            0x12
#define HWIO_GCC_USB3_MP_PHY_AUX_CFG_RCGR_SRC_DIV_DIV10_FVAL                                             0x13
#define HWIO_GCC_USB3_MP_PHY_AUX_CFG_RCGR_SRC_DIV_DIV10_5_FVAL                                           0x14
#define HWIO_GCC_USB3_MP_PHY_AUX_CFG_RCGR_SRC_DIV_DIV11_FVAL                                             0x15
#define HWIO_GCC_USB3_MP_PHY_AUX_CFG_RCGR_SRC_DIV_DIV11_5_FVAL                                           0x16
#define HWIO_GCC_USB3_MP_PHY_AUX_CFG_RCGR_SRC_DIV_DIV12_FVAL                                             0x17
#define HWIO_GCC_USB3_MP_PHY_AUX_CFG_RCGR_SRC_DIV_DIV12_5_FVAL                                           0x18
#define HWIO_GCC_USB3_MP_PHY_AUX_CFG_RCGR_SRC_DIV_DIV13_FVAL                                             0x19
#define HWIO_GCC_USB3_MP_PHY_AUX_CFG_RCGR_SRC_DIV_DIV13_5_FVAL                                           0x1a
#define HWIO_GCC_USB3_MP_PHY_AUX_CFG_RCGR_SRC_DIV_DIV14_FVAL                                             0x1b
#define HWIO_GCC_USB3_MP_PHY_AUX_CFG_RCGR_SRC_DIV_DIV14_5_FVAL                                           0x1c
#define HWIO_GCC_USB3_MP_PHY_AUX_CFG_RCGR_SRC_DIV_DIV15_FVAL                                             0x1d
#define HWIO_GCC_USB3_MP_PHY_AUX_CFG_RCGR_SRC_DIV_DIV15_5_FVAL                                           0x1e
#define HWIO_GCC_USB3_MP_PHY_AUX_CFG_RCGR_SRC_DIV_DIV16_FVAL                                             0x1f

#define HWIO_GCC_USB3_PHY_PRIM_SP0_BCR_ADDR                                                        (GCC_CLK_CTL_REG_REG_BASE      + 0x00050000)
#define HWIO_GCC_USB3_PHY_PRIM_SP0_BCR_PHYS                                                        (GCC_CLK_CTL_REG_REG_BASE_PHYS + 0x00050000)
#define HWIO_GCC_USB3_PHY_PRIM_SP0_BCR_OFFS                                                        (GCC_CLK_CTL_REG_REG_BASE_OFFS + 0x00050000)
#define HWIO_GCC_USB3_PHY_PRIM_SP0_BCR_RMSK                                                               0x1
#define HWIO_GCC_USB3_PHY_PRIM_SP0_BCR_IN          \
        in_dword(HWIO_GCC_USB3_PHY_PRIM_SP0_BCR_ADDR)
#define HWIO_GCC_USB3_PHY_PRIM_SP0_BCR_INM(m)      \
        in_dword_masked(HWIO_GCC_USB3_PHY_PRIM_SP0_BCR_ADDR, m)
#define HWIO_GCC_USB3_PHY_PRIM_SP0_BCR_OUT(v)      \
        out_dword(HWIO_GCC_USB3_PHY_PRIM_SP0_BCR_ADDR,v)
#define HWIO_GCC_USB3_PHY_PRIM_SP0_BCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GCC_USB3_PHY_PRIM_SP0_BCR_ADDR,m,v,HWIO_GCC_USB3_PHY_PRIM_SP0_BCR_IN)
#define HWIO_GCC_USB3_PHY_PRIM_SP0_BCR_BLK_ARES_BMSK                                                      0x1
#define HWIO_GCC_USB3_PHY_PRIM_SP0_BCR_BLK_ARES_SHFT                                                      0x0
#define HWIO_GCC_USB3_PHY_PRIM_SP0_BCR_BLK_ARES_DISABLE_FVAL                                              0x0
#define HWIO_GCC_USB3_PHY_PRIM_SP0_BCR_BLK_ARES_ENABLE_FVAL                                               0x1

#define HWIO_GCC_USB3_PHY_PRIM_SP1_BCR_ADDR                                                        (GCC_CLK_CTL_REG_REG_BASE      + 0x00050004)
#define HWIO_GCC_USB3_PHY_PRIM_SP1_BCR_PHYS                                                        (GCC_CLK_CTL_REG_REG_BASE_PHYS + 0x00050004)
#define HWIO_GCC_USB3_PHY_PRIM_SP1_BCR_OFFS                                                        (GCC_CLK_CTL_REG_REG_BASE_OFFS + 0x00050004)
#define HWIO_GCC_USB3_PHY_PRIM_SP1_BCR_RMSK                                                               0x1
#define HWIO_GCC_USB3_PHY_PRIM_SP1_BCR_IN          \
        in_dword(HWIO_GCC_USB3_PHY_PRIM_SP1_BCR_ADDR)
#define HWIO_GCC_USB3_PHY_PRIM_SP1_BCR_INM(m)      \
        in_dword_masked(HWIO_GCC_USB3_PHY_PRIM_SP1_BCR_ADDR, m)
#define HWIO_GCC_USB3_PHY_PRIM_SP1_BCR_OUT(v)      \
        out_dword(HWIO_GCC_USB3_PHY_PRIM_SP1_BCR_ADDR,v)
#define HWIO_GCC_USB3_PHY_PRIM_SP1_BCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GCC_USB3_PHY_PRIM_SP1_BCR_ADDR,m,v,HWIO_GCC_USB3_PHY_PRIM_SP1_BCR_IN)
#define HWIO_GCC_USB3_PHY_PRIM_SP1_BCR_BLK_ARES_BMSK                                                      0x1
#define HWIO_GCC_USB3_PHY_PRIM_SP1_BCR_BLK_ARES_SHFT                                                      0x0
#define HWIO_GCC_USB3_PHY_PRIM_SP1_BCR_BLK_ARES_DISABLE_FVAL                                              0x0
#define HWIO_GCC_USB3_PHY_PRIM_SP1_BCR_BLK_ARES_ENABLE_FVAL                                               0x1

#define HWIO_GCC_USB3PHY_PHY_PRIM_SP0_BCR_ADDR                                                     (GCC_CLK_CTL_REG_REG_BASE      + 0x00050008)
#define HWIO_GCC_USB3PHY_PHY_PRIM_SP0_BCR_PHYS                                                     (GCC_CLK_CTL_REG_REG_BASE_PHYS + 0x00050008)
#define HWIO_GCC_USB3PHY_PHY_PRIM_SP0_BCR_OFFS                                                     (GCC_CLK_CTL_REG_REG_BASE_OFFS + 0x00050008)
#define HWIO_GCC_USB3PHY_PHY_PRIM_SP0_BCR_RMSK                                                            0x1
#define HWIO_GCC_USB3PHY_PHY_PRIM_SP0_BCR_IN          \
        in_dword(HWIO_GCC_USB3PHY_PHY_PRIM_SP0_BCR_ADDR)
#define HWIO_GCC_USB3PHY_PHY_PRIM_SP0_BCR_INM(m)      \
        in_dword_masked(HWIO_GCC_USB3PHY_PHY_PRIM_SP0_BCR_ADDR, m)
#define HWIO_GCC_USB3PHY_PHY_PRIM_SP0_BCR_OUT(v)      \
        out_dword(HWIO_GCC_USB3PHY_PHY_PRIM_SP0_BCR_ADDR,v)
#define HWIO_GCC_USB3PHY_PHY_PRIM_SP0_BCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GCC_USB3PHY_PHY_PRIM_SP0_BCR_ADDR,m,v,HWIO_GCC_USB3PHY_PHY_PRIM_SP0_BCR_IN)
#define HWIO_GCC_USB3PHY_PHY_PRIM_SP0_BCR_BLK_ARES_BMSK                                                   0x1
#define HWIO_GCC_USB3PHY_PHY_PRIM_SP0_BCR_BLK_ARES_SHFT                                                   0x0
#define HWIO_GCC_USB3PHY_PHY_PRIM_SP0_BCR_BLK_ARES_DISABLE_FVAL                                           0x0
#define HWIO_GCC_USB3PHY_PHY_PRIM_SP0_BCR_BLK_ARES_ENABLE_FVAL                                            0x1

#define HWIO_GCC_USB3PHY_PHY_PRIM_SP1_BCR_ADDR                                                     (GCC_CLK_CTL_REG_REG_BASE      + 0x0005000c)
#define HWIO_GCC_USB3PHY_PHY_PRIM_SP1_BCR_PHYS                                                     (GCC_CLK_CTL_REG_REG_BASE_PHYS + 0x0005000c)
#define HWIO_GCC_USB3PHY_PHY_PRIM_SP1_BCR_OFFS                                                     (GCC_CLK_CTL_REG_REG_BASE_OFFS + 0x0005000c)
#define HWIO_GCC_USB3PHY_PHY_PRIM_SP1_BCR_RMSK                                                            0x1
#define HWIO_GCC_USB3PHY_PHY_PRIM_SP1_BCR_IN          \
        in_dword(HWIO_GCC_USB3PHY_PHY_PRIM_SP1_BCR_ADDR)
#define HWIO_GCC_USB3PHY_PHY_PRIM_SP1_BCR_INM(m)      \
        in_dword_masked(HWIO_GCC_USB3PHY_PHY_PRIM_SP1_BCR_ADDR, m)
#define HWIO_GCC_USB3PHY_PHY_PRIM_SP1_BCR_OUT(v)      \
        out_dword(HWIO_GCC_USB3PHY_PHY_PRIM_SP1_BCR_ADDR,v)
#define HWIO_GCC_USB3PHY_PHY_PRIM_SP1_BCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GCC_USB3PHY_PHY_PRIM_SP1_BCR_ADDR,m,v,HWIO_GCC_USB3PHY_PHY_PRIM_SP1_BCR_IN)
#define HWIO_GCC_USB3PHY_PHY_PRIM_SP1_BCR_BLK_ARES_BMSK                                                   0x1
#define HWIO_GCC_USB3PHY_PHY_PRIM_SP1_BCR_BLK_ARES_SHFT                                                   0x0
#define HWIO_GCC_USB3PHY_PHY_PRIM_SP1_BCR_BLK_ARES_DISABLE_FVAL                                           0x0
#define HWIO_GCC_USB3PHY_PHY_PRIM_SP1_BCR_BLK_ARES_ENABLE_FVAL                                            0x1

#define HWIO_GCC_USB3_DP_PHY_PRIM_SP0_BCR_ADDR                                                     (GCC_CLK_CTL_REG_REG_BASE      + 0x00050010)
#define HWIO_GCC_USB3_DP_PHY_PRIM_SP0_BCR_PHYS                                                     (GCC_CLK_CTL_REG_REG_BASE_PHYS + 0x00050010)
#define HWIO_GCC_USB3_DP_PHY_PRIM_SP0_BCR_OFFS                                                     (GCC_CLK_CTL_REG_REG_BASE_OFFS + 0x00050010)
#define HWIO_GCC_USB3_DP_PHY_PRIM_SP0_BCR_RMSK                                                            0x1
#define HWIO_GCC_USB3_DP_PHY_PRIM_SP0_BCR_IN          \
        in_dword(HWIO_GCC_USB3_DP_PHY_PRIM_SP0_BCR_ADDR)
#define HWIO_GCC_USB3_DP_PHY_PRIM_SP0_BCR_INM(m)      \
        in_dword_masked(HWIO_GCC_USB3_DP_PHY_PRIM_SP0_BCR_ADDR, m)
#define HWIO_GCC_USB3_DP_PHY_PRIM_SP0_BCR_OUT(v)      \
        out_dword(HWIO_GCC_USB3_DP_PHY_PRIM_SP0_BCR_ADDR,v)
#define HWIO_GCC_USB3_DP_PHY_PRIM_SP0_BCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GCC_USB3_DP_PHY_PRIM_SP0_BCR_ADDR,m,v,HWIO_GCC_USB3_DP_PHY_PRIM_SP0_BCR_IN)
#define HWIO_GCC_USB3_DP_PHY_PRIM_SP0_BCR_BLK_ARES_BMSK                                                   0x1
#define HWIO_GCC_USB3_DP_PHY_PRIM_SP0_BCR_BLK_ARES_SHFT                                                   0x0
#define HWIO_GCC_USB3_DP_PHY_PRIM_SP0_BCR_BLK_ARES_DISABLE_FVAL                                           0x0
#define HWIO_GCC_USB3_DP_PHY_PRIM_SP0_BCR_BLK_ARES_ENABLE_FVAL                                            0x1

#define HWIO_GCC_USB3_DP_PHY_PRIM_SP1_BCR_ADDR                                                     (GCC_CLK_CTL_REG_REG_BASE      + 0x00050014)
#define HWIO_GCC_USB3_DP_PHY_PRIM_SP1_BCR_PHYS                                                     (GCC_CLK_CTL_REG_REG_BASE_PHYS + 0x00050014)
#define HWIO_GCC_USB3_DP_PHY_PRIM_SP1_BCR_OFFS                                                     (GCC_CLK_CTL_REG_REG_BASE_OFFS + 0x00050014)
#define HWIO_GCC_USB3_DP_PHY_PRIM_SP1_BCR_RMSK                                                            0x1
#define HWIO_GCC_USB3_DP_PHY_PRIM_SP1_BCR_IN          \
        in_dword(HWIO_GCC_USB3_DP_PHY_PRIM_SP1_BCR_ADDR)
#define HWIO_GCC_USB3_DP_PHY_PRIM_SP1_BCR_INM(m)      \
        in_dword_masked(HWIO_GCC_USB3_DP_PHY_PRIM_SP1_BCR_ADDR, m)
#define HWIO_GCC_USB3_DP_PHY_PRIM_SP1_BCR_OUT(v)      \
        out_dword(HWIO_GCC_USB3_DP_PHY_PRIM_SP1_BCR_ADDR,v)
#define HWIO_GCC_USB3_DP_PHY_PRIM_SP1_BCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GCC_USB3_DP_PHY_PRIM_SP1_BCR_ADDR,m,v,HWIO_GCC_USB3_DP_PHY_PRIM_SP1_BCR_IN)
#define HWIO_GCC_USB3_DP_PHY_PRIM_SP1_BCR_BLK_ARES_BMSK                                                   0x1
#define HWIO_GCC_USB3_DP_PHY_PRIM_SP1_BCR_BLK_ARES_SHFT                                                   0x0
#define HWIO_GCC_USB3_DP_PHY_PRIM_SP1_BCR_BLK_ARES_DISABLE_FVAL                                           0x0
#define HWIO_GCC_USB3_DP_PHY_PRIM_SP1_BCR_BLK_ARES_ENABLE_FVAL                                            0x1

#define HWIO_GCC_USB3_PHY_SEC_BCR_ADDR                                                             (GCC_CLK_CTL_REG_REG_BASE      + 0x00050018)
#define HWIO_GCC_USB3_PHY_SEC_BCR_PHYS                                                             (GCC_CLK_CTL_REG_REG_BASE_PHYS + 0x00050018)
#define HWIO_GCC_USB3_PHY_SEC_BCR_OFFS                                                             (GCC_CLK_CTL_REG_REG_BASE_OFFS + 0x00050018)
#define HWIO_GCC_USB3_PHY_SEC_BCR_RMSK                                                                    0x1
#define HWIO_GCC_USB3_PHY_SEC_BCR_IN          \
        in_dword(HWIO_GCC_USB3_PHY_SEC_BCR_ADDR)
#define HWIO_GCC_USB3_PHY_SEC_BCR_INM(m)      \
        in_dword_masked(HWIO_GCC_USB3_PHY_SEC_BCR_ADDR, m)
#define HWIO_GCC_USB3_PHY_SEC_BCR_OUT(v)      \
        out_dword(HWIO_GCC_USB3_PHY_SEC_BCR_ADDR,v)
#define HWIO_GCC_USB3_PHY_SEC_BCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GCC_USB3_PHY_SEC_BCR_ADDR,m,v,HWIO_GCC_USB3_PHY_SEC_BCR_IN)
#define HWIO_GCC_USB3_PHY_SEC_BCR_BLK_ARES_BMSK                                                           0x1
#define HWIO_GCC_USB3_PHY_SEC_BCR_BLK_ARES_SHFT                                                           0x0
#define HWIO_GCC_USB3_PHY_SEC_BCR_BLK_ARES_DISABLE_FVAL                                                   0x0
#define HWIO_GCC_USB3_PHY_SEC_BCR_BLK_ARES_ENABLE_FVAL                                                    0x1

#define HWIO_GCC_USB3PHY_PHY_SEC_BCR_ADDR                                                          (GCC_CLK_CTL_REG_REG_BASE      + 0x0005001c)
#define HWIO_GCC_USB3PHY_PHY_SEC_BCR_PHYS                                                          (GCC_CLK_CTL_REG_REG_BASE_PHYS + 0x0005001c)
#define HWIO_GCC_USB3PHY_PHY_SEC_BCR_OFFS                                                          (GCC_CLK_CTL_REG_REG_BASE_OFFS + 0x0005001c)
#define HWIO_GCC_USB3PHY_PHY_SEC_BCR_RMSK                                                                 0x1
#define HWIO_GCC_USB3PHY_PHY_SEC_BCR_IN          \
        in_dword(HWIO_GCC_USB3PHY_PHY_SEC_BCR_ADDR)
#define HWIO_GCC_USB3PHY_PHY_SEC_BCR_INM(m)      \
        in_dword_masked(HWIO_GCC_USB3PHY_PHY_SEC_BCR_ADDR, m)
#define HWIO_GCC_USB3PHY_PHY_SEC_BCR_OUT(v)      \
        out_dword(HWIO_GCC_USB3PHY_PHY_SEC_BCR_ADDR,v)
#define HWIO_GCC_USB3PHY_PHY_SEC_BCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GCC_USB3PHY_PHY_SEC_BCR_ADDR,m,v,HWIO_GCC_USB3PHY_PHY_SEC_BCR_IN)
#define HWIO_GCC_USB3PHY_PHY_SEC_BCR_BLK_ARES_BMSK                                                        0x1
#define HWIO_GCC_USB3PHY_PHY_SEC_BCR_BLK_ARES_SHFT                                                        0x0
#define HWIO_GCC_USB3PHY_PHY_SEC_BCR_BLK_ARES_DISABLE_FVAL                                                0x0
#define HWIO_GCC_USB3PHY_PHY_SEC_BCR_BLK_ARES_ENABLE_FVAL                                                 0x1

#define HWIO_GCC_USB3_DP_PHY_SEC_BCR_ADDR                                                          (GCC_CLK_CTL_REG_REG_BASE      + 0x00050020)
#define HWIO_GCC_USB3_DP_PHY_SEC_BCR_PHYS                                                          (GCC_CLK_CTL_REG_REG_BASE_PHYS + 0x00050020)
#define HWIO_GCC_USB3_DP_PHY_SEC_BCR_OFFS                                                          (GCC_CLK_CTL_REG_REG_BASE_OFFS + 0x00050020)
#define HWIO_GCC_USB3_DP_PHY_SEC_BCR_RMSK                                                                 0x1
#define HWIO_GCC_USB3_DP_PHY_SEC_BCR_IN          \
        in_dword(HWIO_GCC_USB3_DP_PHY_SEC_BCR_ADDR)
#define HWIO_GCC_USB3_DP_PHY_SEC_BCR_INM(m)      \
        in_dword_masked(HWIO_GCC_USB3_DP_PHY_SEC_BCR_ADDR, m)
#define HWIO_GCC_USB3_DP_PHY_SEC_BCR_OUT(v)      \
        out_dword(HWIO_GCC_USB3_DP_PHY_SEC_BCR_ADDR,v)
#define HWIO_GCC_USB3_DP_PHY_SEC_BCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GCC_USB3_DP_PHY_SEC_BCR_ADDR,m,v,HWIO_GCC_USB3_DP_PHY_SEC_BCR_IN)
#define HWIO_GCC_USB3_DP_PHY_SEC_BCR_BLK_ARES_BMSK                                                        0x1
#define HWIO_GCC_USB3_DP_PHY_SEC_BCR_BLK_ARES_SHFT                                                        0x0
#define HWIO_GCC_USB3_DP_PHY_SEC_BCR_BLK_ARES_DISABLE_FVAL                                                0x0
#define HWIO_GCC_USB3_DP_PHY_SEC_BCR_BLK_ARES_ENABLE_FVAL                                                 0x1

#define HWIO_GCC_USB3_UNIPHY_MP0_BCR_ADDR                                                          (GCC_CLK_CTL_REG_REG_BASE      + 0x00050024)
#define HWIO_GCC_USB3_UNIPHY_MP0_BCR_PHYS                                                          (GCC_CLK_CTL_REG_REG_BASE_PHYS + 0x00050024)
#define HWIO_GCC_USB3_UNIPHY_MP0_BCR_OFFS                                                          (GCC_CLK_CTL_REG_REG_BASE_OFFS + 0x00050024)
#define HWIO_GCC_USB3_UNIPHY_MP0_BCR_RMSK                                                                 0x1
#define HWIO_GCC_USB3_UNIPHY_MP0_BCR_IN          \
        in_dword(HWIO_GCC_USB3_UNIPHY_MP0_BCR_ADDR)
#define HWIO_GCC_USB3_UNIPHY_MP0_BCR_INM(m)      \
        in_dword_masked(HWIO_GCC_USB3_UNIPHY_MP0_BCR_ADDR, m)
#define HWIO_GCC_USB3_UNIPHY_MP0_BCR_OUT(v)      \
        out_dword(HWIO_GCC_USB3_UNIPHY_MP0_BCR_ADDR,v)
#define HWIO_GCC_USB3_UNIPHY_MP0_BCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GCC_USB3_UNIPHY_MP0_BCR_ADDR,m,v,HWIO_GCC_USB3_UNIPHY_MP0_BCR_IN)
#define HWIO_GCC_USB3_UNIPHY_MP0_BCR_BLK_ARES_BMSK                                                        0x1
#define HWIO_GCC_USB3_UNIPHY_MP0_BCR_BLK_ARES_SHFT                                                        0x0
#define HWIO_GCC_USB3_UNIPHY_MP0_BCR_BLK_ARES_DISABLE_FVAL                                                0x0
#define HWIO_GCC_USB3_UNIPHY_MP0_BCR_BLK_ARES_ENABLE_FVAL                                                 0x1

#define HWIO_GCC_USB3_UNIPHY_MP1_BCR_ADDR                                                          (GCC_CLK_CTL_REG_REG_BASE      + 0x00050028)
#define HWIO_GCC_USB3_UNIPHY_MP1_BCR_PHYS                                                          (GCC_CLK_CTL_REG_REG_BASE_PHYS + 0x00050028)
#define HWIO_GCC_USB3_UNIPHY_MP1_BCR_OFFS                                                          (GCC_CLK_CTL_REG_REG_BASE_OFFS + 0x00050028)
#define HWIO_GCC_USB3_UNIPHY_MP1_BCR_RMSK                                                                 0x1
#define HWIO_GCC_USB3_UNIPHY_MP1_BCR_IN          \
        in_dword(HWIO_GCC_USB3_UNIPHY_MP1_BCR_ADDR)
#define HWIO_GCC_USB3_UNIPHY_MP1_BCR_INM(m)      \
        in_dword_masked(HWIO_GCC_USB3_UNIPHY_MP1_BCR_ADDR, m)
#define HWIO_GCC_USB3_UNIPHY_MP1_BCR_OUT(v)      \
        out_dword(HWIO_GCC_USB3_UNIPHY_MP1_BCR_ADDR,v)
#define HWIO_GCC_USB3_UNIPHY_MP1_BCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GCC_USB3_UNIPHY_MP1_BCR_ADDR,m,v,HWIO_GCC_USB3_UNIPHY_MP1_BCR_IN)
#define HWIO_GCC_USB3_UNIPHY_MP1_BCR_BLK_ARES_BMSK                                                        0x1
#define HWIO_GCC_USB3_UNIPHY_MP1_BCR_BLK_ARES_SHFT                                                        0x0
#define HWIO_GCC_USB3_UNIPHY_MP1_BCR_BLK_ARES_DISABLE_FVAL                                                0x0
#define HWIO_GCC_USB3_UNIPHY_MP1_BCR_BLK_ARES_ENABLE_FVAL                                                 0x1

#define HWIO_GCC_USB3UNIPHY_PHY_MP0_BCR_ADDR                                                       (GCC_CLK_CTL_REG_REG_BASE      + 0x0005002c)
#define HWIO_GCC_USB3UNIPHY_PHY_MP0_BCR_PHYS                                                       (GCC_CLK_CTL_REG_REG_BASE_PHYS + 0x0005002c)
#define HWIO_GCC_USB3UNIPHY_PHY_MP0_BCR_OFFS                                                       (GCC_CLK_CTL_REG_REG_BASE_OFFS + 0x0005002c)
#define HWIO_GCC_USB3UNIPHY_PHY_MP0_BCR_RMSK                                                              0x1
#define HWIO_GCC_USB3UNIPHY_PHY_MP0_BCR_IN          \
        in_dword(HWIO_GCC_USB3UNIPHY_PHY_MP0_BCR_ADDR)
#define HWIO_GCC_USB3UNIPHY_PHY_MP0_BCR_INM(m)      \
        in_dword_masked(HWIO_GCC_USB3UNIPHY_PHY_MP0_BCR_ADDR, m)
#define HWIO_GCC_USB3UNIPHY_PHY_MP0_BCR_OUT(v)      \
        out_dword(HWIO_GCC_USB3UNIPHY_PHY_MP0_BCR_ADDR,v)
#define HWIO_GCC_USB3UNIPHY_PHY_MP0_BCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GCC_USB3UNIPHY_PHY_MP0_BCR_ADDR,m,v,HWIO_GCC_USB3UNIPHY_PHY_MP0_BCR_IN)
#define HWIO_GCC_USB3UNIPHY_PHY_MP0_BCR_BLK_ARES_BMSK                                                     0x1
#define HWIO_GCC_USB3UNIPHY_PHY_MP0_BCR_BLK_ARES_SHFT                                                     0x0
#define HWIO_GCC_USB3UNIPHY_PHY_MP0_BCR_BLK_ARES_DISABLE_FVAL                                             0x0
#define HWIO_GCC_USB3UNIPHY_PHY_MP0_BCR_BLK_ARES_ENABLE_FVAL                                              0x1

#define HWIO_GCC_USB3UNIPHY_PHY_MP1_BCR_ADDR                                                       (GCC_CLK_CTL_REG_REG_BASE      + 0x00050030)
#define HWIO_GCC_USB3UNIPHY_PHY_MP1_BCR_PHYS                                                       (GCC_CLK_CTL_REG_REG_BASE_PHYS + 0x00050030)
#define HWIO_GCC_USB3UNIPHY_PHY_MP1_BCR_OFFS                                                       (GCC_CLK_CTL_REG_REG_BASE_OFFS + 0x00050030)
#define HWIO_GCC_USB3UNIPHY_PHY_MP1_BCR_RMSK                                                              0x1
#define HWIO_GCC_USB3UNIPHY_PHY_MP1_BCR_IN          \
        in_dword(HWIO_GCC_USB3UNIPHY_PHY_MP1_BCR_ADDR)
#define HWIO_GCC_USB3UNIPHY_PHY_MP1_BCR_INM(m)      \
        in_dword_masked(HWIO_GCC_USB3UNIPHY_PHY_MP1_BCR_ADDR, m)
#define HWIO_GCC_USB3UNIPHY_PHY_MP1_BCR_OUT(v)      \
        out_dword(HWIO_GCC_USB3UNIPHY_PHY_MP1_BCR_ADDR,v)
#define HWIO_GCC_USB3UNIPHY_PHY_MP1_BCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GCC_USB3UNIPHY_PHY_MP1_BCR_ADDR,m,v,HWIO_GCC_USB3UNIPHY_PHY_MP1_BCR_IN)
#define HWIO_GCC_USB3UNIPHY_PHY_MP1_BCR_BLK_ARES_BMSK                                                     0x1
#define HWIO_GCC_USB3UNIPHY_PHY_MP1_BCR_BLK_ARES_SHFT                                                     0x0
#define HWIO_GCC_USB3UNIPHY_PHY_MP1_BCR_BLK_ARES_DISABLE_FVAL                                             0x0
#define HWIO_GCC_USB3UNIPHY_PHY_MP1_BCR_BLK_ARES_ENABLE_FVAL                                              0x1

#define HWIO_GCC_QUSB2PHY_PRIM_BCR_ADDR                                                            (GCC_CLK_CTL_REG_REG_BASE      + 0x00012000)
#define HWIO_GCC_QUSB2PHY_PRIM_BCR_PHYS                                                            (GCC_CLK_CTL_REG_REG_BASE_PHYS + 0x00012000)
#define HWIO_GCC_QUSB2PHY_PRIM_BCR_OFFS                                                            (GCC_CLK_CTL_REG_REG_BASE_OFFS + 0x00012000)
#define HWIO_GCC_QUSB2PHY_PRIM_BCR_RMSK                                                                   0x1
#define HWIO_GCC_QUSB2PHY_PRIM_BCR_IN          \
        in_dword(HWIO_GCC_QUSB2PHY_PRIM_BCR_ADDR)
#define HWIO_GCC_QUSB2PHY_PRIM_BCR_INM(m)      \
        in_dword_masked(HWIO_GCC_QUSB2PHY_PRIM_BCR_ADDR, m)
#define HWIO_GCC_QUSB2PHY_PRIM_BCR_OUT(v)      \
        out_dword(HWIO_GCC_QUSB2PHY_PRIM_BCR_ADDR,v)
#define HWIO_GCC_QUSB2PHY_PRIM_BCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GCC_QUSB2PHY_PRIM_BCR_ADDR,m,v,HWIO_GCC_QUSB2PHY_PRIM_BCR_IN)
#define HWIO_GCC_QUSB2PHY_PRIM_BCR_BLK_ARES_BMSK                                                          0x1
#define HWIO_GCC_QUSB2PHY_PRIM_BCR_BLK_ARES_SHFT                                                          0x0
#define HWIO_GCC_QUSB2PHY_PRIM_BCR_BLK_ARES_DISABLE_FVAL                                                  0x0
#define HWIO_GCC_QUSB2PHY_PRIM_BCR_BLK_ARES_ENABLE_FVAL                                                   0x1

#define HWIO_GCC_QUSB2PHY_SEC_BCR_ADDR                                                             (GCC_CLK_CTL_REG_REG_BASE      + 0x00012004)
#define HWIO_GCC_QUSB2PHY_SEC_BCR_PHYS                                                             (GCC_CLK_CTL_REG_REG_BASE_PHYS + 0x00012004)
#define HWIO_GCC_QUSB2PHY_SEC_BCR_OFFS                                                             (GCC_CLK_CTL_REG_REG_BASE_OFFS + 0x00012004)
#define HWIO_GCC_QUSB2PHY_SEC_BCR_RMSK                                                                    0x1
#define HWIO_GCC_QUSB2PHY_SEC_BCR_IN          \
        in_dword(HWIO_GCC_QUSB2PHY_SEC_BCR_ADDR)
#define HWIO_GCC_QUSB2PHY_SEC_BCR_INM(m)      \
        in_dword_masked(HWIO_GCC_QUSB2PHY_SEC_BCR_ADDR, m)
#define HWIO_GCC_QUSB2PHY_SEC_BCR_OUT(v)      \
        out_dword(HWIO_GCC_QUSB2PHY_SEC_BCR_ADDR,v)
#define HWIO_GCC_QUSB2PHY_SEC_BCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GCC_QUSB2PHY_SEC_BCR_ADDR,m,v,HWIO_GCC_QUSB2PHY_SEC_BCR_IN)
#define HWIO_GCC_QUSB2PHY_SEC_BCR_BLK_ARES_BMSK                                                           0x1
#define HWIO_GCC_QUSB2PHY_SEC_BCR_BLK_ARES_SHFT                                                           0x0
#define HWIO_GCC_QUSB2PHY_SEC_BCR_BLK_ARES_DISABLE_FVAL                                                   0x0
#define HWIO_GCC_QUSB2PHY_SEC_BCR_BLK_ARES_ENABLE_FVAL                                                    0x1

#define HWIO_GCC_QUSB2PHY_MP0_BCR_ADDR                                                             (GCC_CLK_CTL_REG_REG_BASE      + 0x00012008)
#define HWIO_GCC_QUSB2PHY_MP0_BCR_PHYS                                                             (GCC_CLK_CTL_REG_REG_BASE_PHYS + 0x00012008)
#define HWIO_GCC_QUSB2PHY_MP0_BCR_OFFS                                                             (GCC_CLK_CTL_REG_REG_BASE_OFFS + 0x00012008)
#define HWIO_GCC_QUSB2PHY_MP0_BCR_RMSK                                                                    0x1
#define HWIO_GCC_QUSB2PHY_MP0_BCR_IN          \
        in_dword(HWIO_GCC_QUSB2PHY_MP0_BCR_ADDR)
#define HWIO_GCC_QUSB2PHY_MP0_BCR_INM(m)      \
        in_dword_masked(HWIO_GCC_QUSB2PHY_MP0_BCR_ADDR, m)
#define HWIO_GCC_QUSB2PHY_MP0_BCR_OUT(v)      \
        out_dword(HWIO_GCC_QUSB2PHY_MP0_BCR_ADDR,v)
#define HWIO_GCC_QUSB2PHY_MP0_BCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GCC_QUSB2PHY_MP0_BCR_ADDR,m,v,HWIO_GCC_QUSB2PHY_MP0_BCR_IN)
#define HWIO_GCC_QUSB2PHY_MP0_BCR_BLK_ARES_BMSK                                                           0x1
#define HWIO_GCC_QUSB2PHY_MP0_BCR_BLK_ARES_SHFT                                                           0x0
#define HWIO_GCC_QUSB2PHY_MP0_BCR_BLK_ARES_DISABLE_FVAL                                                   0x0
#define HWIO_GCC_QUSB2PHY_MP0_BCR_BLK_ARES_ENABLE_FVAL                                                    0x1

#define HWIO_GCC_QUSB2PHY_MP1_BCR_ADDR                                                             (GCC_CLK_CTL_REG_REG_BASE      + 0x0001200c)
#define HWIO_GCC_QUSB2PHY_MP1_BCR_PHYS                                                             (GCC_CLK_CTL_REG_REG_BASE_PHYS + 0x0001200c)
#define HWIO_GCC_QUSB2PHY_MP1_BCR_OFFS                                                             (GCC_CLK_CTL_REG_REG_BASE_OFFS + 0x0001200c)
#define HWIO_GCC_QUSB2PHY_MP1_BCR_RMSK                                                                    0x1
#define HWIO_GCC_QUSB2PHY_MP1_BCR_IN          \
        in_dword(HWIO_GCC_QUSB2PHY_MP1_BCR_ADDR)
#define HWIO_GCC_QUSB2PHY_MP1_BCR_INM(m)      \
        in_dword_masked(HWIO_GCC_QUSB2PHY_MP1_BCR_ADDR, m)
#define HWIO_GCC_QUSB2PHY_MP1_BCR_OUT(v)      \
        out_dword(HWIO_GCC_QUSB2PHY_MP1_BCR_ADDR,v)
#define HWIO_GCC_QUSB2PHY_MP1_BCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GCC_QUSB2PHY_MP1_BCR_ADDR,m,v,HWIO_GCC_QUSB2PHY_MP1_BCR_IN)
#define HWIO_GCC_QUSB2PHY_MP1_BCR_BLK_ARES_BMSK                                                           0x1
#define HWIO_GCC_QUSB2PHY_MP1_BCR_BLK_ARES_SHFT                                                           0x0
#define HWIO_GCC_QUSB2PHY_MP1_BCR_BLK_ARES_DISABLE_FVAL                                                   0x0
#define HWIO_GCC_QUSB2PHY_MP1_BCR_BLK_ARES_ENABLE_FVAL                                                    0x1

#define HWIO_GCC_QUSB2PHY_5_BCR_ADDR                                                               (GCC_CLK_CTL_REG_REG_BASE      + 0x00012010)
#define HWIO_GCC_QUSB2PHY_5_BCR_PHYS                                                               (GCC_CLK_CTL_REG_REG_BASE_PHYS + 0x00012010)
#define HWIO_GCC_QUSB2PHY_5_BCR_OFFS                                                               (GCC_CLK_CTL_REG_REG_BASE_OFFS + 0x00012010)
#define HWIO_GCC_QUSB2PHY_5_BCR_RMSK                                                                      0x1
#define HWIO_GCC_QUSB2PHY_5_BCR_IN          \
        in_dword(HWIO_GCC_QUSB2PHY_5_BCR_ADDR)
#define HWIO_GCC_QUSB2PHY_5_BCR_INM(m)      \
        in_dword_masked(HWIO_GCC_QUSB2PHY_5_BCR_ADDR, m)
#define HWIO_GCC_QUSB2PHY_5_BCR_OUT(v)      \
        out_dword(HWIO_GCC_QUSB2PHY_5_BCR_ADDR,v)
#define HWIO_GCC_QUSB2PHY_5_BCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GCC_QUSB2PHY_5_BCR_ADDR,m,v,HWIO_GCC_QUSB2PHY_5_BCR_IN)
#define HWIO_GCC_QUSB2PHY_5_BCR_BLK_ARES_BMSK                                                             0x1
#define HWIO_GCC_QUSB2PHY_5_BCR_BLK_ARES_SHFT                                                             0x0
#define HWIO_GCC_QUSB2PHY_5_BCR_BLK_ARES_DISABLE_FVAL                                                     0x0
#define HWIO_GCC_QUSB2PHY_5_BCR_BLK_ARES_ENABLE_FVAL                                                      0x1

#define HWIO_GCC_USB_PHY_CFG_AHB2PHY_BCR_ADDR                                                      (GCC_CLK_CTL_REG_REG_BASE      + 0x0006a000)
#define HWIO_GCC_USB_PHY_CFG_AHB2PHY_BCR_PHYS                                                      (GCC_CLK_CTL_REG_REG_BASE_PHYS + 0x0006a000)
#define HWIO_GCC_USB_PHY_CFG_AHB2PHY_BCR_OFFS                                                      (GCC_CLK_CTL_REG_REG_BASE_OFFS + 0x0006a000)
#define HWIO_GCC_USB_PHY_CFG_AHB2PHY_BCR_RMSK                                                             0x1
#define HWIO_GCC_USB_PHY_CFG_AHB2PHY_BCR_IN          \
        in_dword(HWIO_GCC_USB_PHY_CFG_AHB2PHY_BCR_ADDR)
#define HWIO_GCC_USB_PHY_CFG_AHB2PHY_BCR_INM(m)      \
        in_dword_masked(HWIO_GCC_USB_PHY_CFG_AHB2PHY_BCR_ADDR, m)
#define HWIO_GCC_USB_PHY_CFG_AHB2PHY_BCR_OUT(v)      \
        out_dword(HWIO_GCC_USB_PHY_CFG_AHB2PHY_BCR_ADDR,v)
#define HWIO_GCC_USB_PHY_CFG_AHB2PHY_BCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GCC_USB_PHY_CFG_AHB2PHY_BCR_ADDR,m,v,HWIO_GCC_USB_PHY_CFG_AHB2PHY_BCR_IN)
#define HWIO_GCC_USB_PHY_CFG_AHB2PHY_BCR_BLK_ARES_BMSK                                                    0x1
#define HWIO_GCC_USB_PHY_CFG_AHB2PHY_BCR_BLK_ARES_SHFT                                                    0x0
#define HWIO_GCC_USB_PHY_CFG_AHB2PHY_BCR_BLK_ARES_DISABLE_FVAL                                            0x0
#define HWIO_GCC_USB_PHY_CFG_AHB2PHY_BCR_BLK_ARES_ENABLE_FVAL                                             0x1

#define HWIO_GCC_AGGRE_USB3_PRIM_AXI_CBCR_ADDR                                                     (GCC_CLK_CTL_REG_REG_BASE      + 0x0000f07c)
#define HWIO_GCC_AGGRE_USB3_PRIM_AXI_CBCR_PHYS                                                     (GCC_CLK_CTL_REG_REG_BASE_PHYS + 0x0000f07c)
#define HWIO_GCC_AGGRE_USB3_PRIM_AXI_CBCR_OFFS                                                     (GCC_CLK_CTL_REG_REG_BASE_OFFS + 0x0000f07c)
#define HWIO_GCC_AGGRE_USB3_PRIM_AXI_CBCR_RMSK                                                     0x80000005
#define HWIO_GCC_AGGRE_USB3_PRIM_AXI_CBCR_IN          \
        in_dword(HWIO_GCC_AGGRE_USB3_PRIM_AXI_CBCR_ADDR)
#define HWIO_GCC_AGGRE_USB3_PRIM_AXI_CBCR_INM(m)      \
        in_dword_masked(HWIO_GCC_AGGRE_USB3_PRIM_AXI_CBCR_ADDR, m)
#define HWIO_GCC_AGGRE_USB3_PRIM_AXI_CBCR_OUT(v)      \
        out_dword(HWIO_GCC_AGGRE_USB3_PRIM_AXI_CBCR_ADDR,v)
#define HWIO_GCC_AGGRE_USB3_PRIM_AXI_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GCC_AGGRE_USB3_PRIM_AXI_CBCR_ADDR,m,v,HWIO_GCC_AGGRE_USB3_PRIM_AXI_CBCR_IN)
#define HWIO_GCC_AGGRE_USB3_PRIM_AXI_CBCR_CLK_OFF_BMSK                                             0x80000000
#define HWIO_GCC_AGGRE_USB3_PRIM_AXI_CBCR_CLK_OFF_SHFT                                                   0x1f
#define HWIO_GCC_AGGRE_USB3_PRIM_AXI_CBCR_CLK_ARES_BMSK                                                   0x4
#define HWIO_GCC_AGGRE_USB3_PRIM_AXI_CBCR_CLK_ARES_SHFT                                                   0x2
#define HWIO_GCC_AGGRE_USB3_PRIM_AXI_CBCR_CLK_ARES_NO_RESET_FVAL                                          0x0
#define HWIO_GCC_AGGRE_USB3_PRIM_AXI_CBCR_CLK_ARES_RESET_FVAL                                             0x1
#define HWIO_GCC_AGGRE_USB3_PRIM_AXI_CBCR_CLK_ENABLE_BMSK                                                 0x1
#define HWIO_GCC_AGGRE_USB3_PRIM_AXI_CBCR_CLK_ENABLE_SHFT                                                 0x0
#define HWIO_GCC_AGGRE_USB3_PRIM_AXI_CBCR_CLK_ENABLE_DISABLE_FVAL                                         0x0
#define HWIO_GCC_AGGRE_USB3_PRIM_AXI_CBCR_CLK_ENABLE_ENABLE_FVAL                                          0x1

#define HWIO_GCC_AGGRE_USB3_SEC_AXI_CBCR_ADDR                                                      (GCC_CLK_CTL_REG_REG_BASE      + 0x0001007c)
#define HWIO_GCC_AGGRE_USB3_SEC_AXI_CBCR_PHYS                                                      (GCC_CLK_CTL_REG_REG_BASE_PHYS + 0x0001007c)
#define HWIO_GCC_AGGRE_USB3_SEC_AXI_CBCR_OFFS                                                      (GCC_CLK_CTL_REG_REG_BASE_OFFS + 0x0001007c)
#define HWIO_GCC_AGGRE_USB3_SEC_AXI_CBCR_RMSK                                                      0x80000005
#define HWIO_GCC_AGGRE_USB3_SEC_AXI_CBCR_IN          \
        in_dword(HWIO_GCC_AGGRE_USB3_SEC_AXI_CBCR_ADDR)
#define HWIO_GCC_AGGRE_USB3_SEC_AXI_CBCR_INM(m)      \
        in_dword_masked(HWIO_GCC_AGGRE_USB3_SEC_AXI_CBCR_ADDR, m)
#define HWIO_GCC_AGGRE_USB3_SEC_AXI_CBCR_OUT(v)      \
        out_dword(HWIO_GCC_AGGRE_USB3_SEC_AXI_CBCR_ADDR,v)
#define HWIO_GCC_AGGRE_USB3_SEC_AXI_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GCC_AGGRE_USB3_SEC_AXI_CBCR_ADDR,m,v,HWIO_GCC_AGGRE_USB3_SEC_AXI_CBCR_IN)
#define HWIO_GCC_AGGRE_USB3_SEC_AXI_CBCR_CLK_OFF_BMSK                                              0x80000000
#define HWIO_GCC_AGGRE_USB3_SEC_AXI_CBCR_CLK_OFF_SHFT                                                    0x1f
#define HWIO_GCC_AGGRE_USB3_SEC_AXI_CBCR_CLK_ARES_BMSK                                                    0x4
#define HWIO_GCC_AGGRE_USB3_SEC_AXI_CBCR_CLK_ARES_SHFT                                                    0x2
#define HWIO_GCC_AGGRE_USB3_SEC_AXI_CBCR_CLK_ARES_NO_RESET_FVAL                                           0x0
#define HWIO_GCC_AGGRE_USB3_SEC_AXI_CBCR_CLK_ARES_RESET_FVAL                                              0x1
#define HWIO_GCC_AGGRE_USB3_SEC_AXI_CBCR_CLK_ENABLE_BMSK                                                  0x1
#define HWIO_GCC_AGGRE_USB3_SEC_AXI_CBCR_CLK_ENABLE_SHFT                                                  0x0
#define HWIO_GCC_AGGRE_USB3_SEC_AXI_CBCR_CLK_ENABLE_DISABLE_FVAL                                          0x0
#define HWIO_GCC_AGGRE_USB3_SEC_AXI_CBCR_CLK_ENABLE_ENABLE_FVAL                                           0x1

#define HWIO_GCC_AGGRE_USB3_MP_AXI_CBCR_ADDR                                                       (GCC_CLK_CTL_REG_REG_BASE      + 0x000a6084)
#define HWIO_GCC_AGGRE_USB3_MP_AXI_CBCR_PHYS                                                       (GCC_CLK_CTL_REG_REG_BASE_PHYS + 0x000a6084)
#define HWIO_GCC_AGGRE_USB3_MP_AXI_CBCR_OFFS                                                       (GCC_CLK_CTL_REG_REG_BASE_OFFS + 0x000a6084)
#define HWIO_GCC_AGGRE_USB3_MP_AXI_CBCR_RMSK                                                       0x80000005
#define HWIO_GCC_AGGRE_USB3_MP_AXI_CBCR_IN          \
        in_dword(HWIO_GCC_AGGRE_USB3_MP_AXI_CBCR_ADDR)
#define HWIO_GCC_AGGRE_USB3_MP_AXI_CBCR_INM(m)      \
        in_dword_masked(HWIO_GCC_AGGRE_USB3_MP_AXI_CBCR_ADDR, m)
#define HWIO_GCC_AGGRE_USB3_MP_AXI_CBCR_OUT(v)      \
        out_dword(HWIO_GCC_AGGRE_USB3_MP_AXI_CBCR_ADDR,v)
#define HWIO_GCC_AGGRE_USB3_MP_AXI_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GCC_AGGRE_USB3_MP_AXI_CBCR_ADDR,m,v,HWIO_GCC_AGGRE_USB3_MP_AXI_CBCR_IN)
#define HWIO_GCC_AGGRE_USB3_MP_AXI_CBCR_CLK_OFF_BMSK                                               0x80000000
#define HWIO_GCC_AGGRE_USB3_MP_AXI_CBCR_CLK_OFF_SHFT                                                     0x1f
#define HWIO_GCC_AGGRE_USB3_MP_AXI_CBCR_CLK_ARES_BMSK                                                     0x4
#define HWIO_GCC_AGGRE_USB3_MP_AXI_CBCR_CLK_ARES_SHFT                                                     0x2
#define HWIO_GCC_AGGRE_USB3_MP_AXI_CBCR_CLK_ARES_NO_RESET_FVAL                                            0x0
#define HWIO_GCC_AGGRE_USB3_MP_AXI_CBCR_CLK_ARES_RESET_FVAL                                               0x1
#define HWIO_GCC_AGGRE_USB3_MP_AXI_CBCR_CLK_ENABLE_BMSK                                                   0x1
#define HWIO_GCC_AGGRE_USB3_MP_AXI_CBCR_CLK_ENABLE_SHFT                                                   0x0
#define HWIO_GCC_AGGRE_USB3_MP_AXI_CBCR_CLK_ENABLE_DISABLE_FVAL                                           0x0
#define HWIO_GCC_AGGRE_USB3_MP_AXI_CBCR_CLK_ENABLE_ENABLE_FVAL                                            0x1

#define HWIO_GCC_TZ_USB30_PRIM_SGDSCR_ADDR                                                         (GCC_CLK_CTL_REG_REG_BASE      + 0x0003b000)
#define HWIO_GCC_TZ_USB30_PRIM_SGDSCR_PHYS                                                         (GCC_CLK_CTL_REG_REG_BASE_PHYS + 0x0003b000)
#define HWIO_GCC_TZ_USB30_PRIM_SGDSCR_OFFS                                                         (GCC_CLK_CTL_REG_REG_BASE_OFFS + 0x0003b000)
#define HWIO_GCC_TZ_USB30_PRIM_SGDSCR_RMSK                                                                0x3
#define HWIO_GCC_TZ_USB30_PRIM_SGDSCR_IN          \
        in_dword(HWIO_GCC_TZ_USB30_PRIM_SGDSCR_ADDR)
#define HWIO_GCC_TZ_USB30_PRIM_SGDSCR_INM(m)      \
        in_dword_masked(HWIO_GCC_TZ_USB30_PRIM_SGDSCR_ADDR, m)
#define HWIO_GCC_TZ_USB30_PRIM_SGDSCR_OUT(v)      \
        out_dword(HWIO_GCC_TZ_USB30_PRIM_SGDSCR_ADDR,v)
#define HWIO_GCC_TZ_USB30_PRIM_SGDSCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GCC_TZ_USB30_PRIM_SGDSCR_ADDR,m,v,HWIO_GCC_TZ_USB30_PRIM_SGDSCR_IN)
#define HWIO_GCC_TZ_USB30_PRIM_SGDSCR_RETAIN_FF_ENABLE_BMSK                                               0x2
#define HWIO_GCC_TZ_USB30_PRIM_SGDSCR_RETAIN_FF_ENABLE_SHFT                                               0x1
#define HWIO_GCC_TZ_USB30_PRIM_SGDSCR_RETAIN_FF_ENABLE_DISABLE_FVAL                                       0x0
#define HWIO_GCC_TZ_USB30_PRIM_SGDSCR_RETAIN_FF_ENABLE_ENABLE_FVAL                                        0x1
#define HWIO_GCC_TZ_USB30_PRIM_SGDSCR_SW_OVERRIDE_BMSK                                                    0x1
#define HWIO_GCC_TZ_USB30_PRIM_SGDSCR_SW_OVERRIDE_SHFT                                                    0x0
#define HWIO_GCC_TZ_USB30_PRIM_SGDSCR_SW_OVERRIDE_DISABLE_FVAL                                            0x0
#define HWIO_GCC_TZ_USB30_PRIM_SGDSCR_SW_OVERRIDE_ENABLE_FVAL                                             0x1

#define HWIO_GCC_TZ_USB30_MP_SGDSCR_ADDR                                                           (GCC_CLK_CTL_REG_REG_BASE      + 0x0003b004)
#define HWIO_GCC_TZ_USB30_MP_SGDSCR_PHYS                                                           (GCC_CLK_CTL_REG_REG_BASE_PHYS + 0x0003b004)
#define HWIO_GCC_TZ_USB30_MP_SGDSCR_OFFS                                                           (GCC_CLK_CTL_REG_REG_BASE_OFFS + 0x0003b004)
#define HWIO_GCC_TZ_USB30_MP_SGDSCR_RMSK                                                                  0x3
#define HWIO_GCC_TZ_USB30_MP_SGDSCR_IN          \
        in_dword(HWIO_GCC_TZ_USB30_MP_SGDSCR_ADDR)
#define HWIO_GCC_TZ_USB30_MP_SGDSCR_INM(m)      \
        in_dword_masked(HWIO_GCC_TZ_USB30_MP_SGDSCR_ADDR, m)
#define HWIO_GCC_TZ_USB30_MP_SGDSCR_OUT(v)      \
        out_dword(HWIO_GCC_TZ_USB30_MP_SGDSCR_ADDR,v)
#define HWIO_GCC_TZ_USB30_MP_SGDSCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GCC_TZ_USB30_MP_SGDSCR_ADDR,m,v,HWIO_GCC_TZ_USB30_MP_SGDSCR_IN)
#define HWIO_GCC_TZ_USB30_MP_SGDSCR_RETAIN_FF_ENABLE_BMSK                                                 0x2
#define HWIO_GCC_TZ_USB30_MP_SGDSCR_RETAIN_FF_ENABLE_SHFT                                                 0x1
#define HWIO_GCC_TZ_USB30_MP_SGDSCR_RETAIN_FF_ENABLE_DISABLE_FVAL                                         0x0
#define HWIO_GCC_TZ_USB30_MP_SGDSCR_RETAIN_FF_ENABLE_ENABLE_FVAL                                          0x1
#define HWIO_GCC_TZ_USB30_MP_SGDSCR_SW_OVERRIDE_BMSK                                                      0x1
#define HWIO_GCC_TZ_USB30_MP_SGDSCR_SW_OVERRIDE_SHFT                                                      0x0
#define HWIO_GCC_TZ_USB30_MP_SGDSCR_SW_OVERRIDE_DISABLE_FVAL                                              0x0
#define HWIO_GCC_TZ_USB30_MP_SGDSCR_SW_OVERRIDE_ENABLE_FVAL                                               0x1

#define HWIO_GCC_TZ_USB30_SEC_SGDSCR_ADDR                                                          (GCC_CLK_CTL_REG_REG_BASE      + 0x0003b030)
#define HWIO_GCC_TZ_USB30_SEC_SGDSCR_PHYS                                                          (GCC_CLK_CTL_REG_REG_BASE_PHYS + 0x0003b030)
#define HWIO_GCC_TZ_USB30_SEC_SGDSCR_OFFS                                                          (GCC_CLK_CTL_REG_REG_BASE_OFFS + 0x0003b030)
#define HWIO_GCC_TZ_USB30_SEC_SGDSCR_RMSK                                                                 0x3
#define HWIO_GCC_TZ_USB30_SEC_SGDSCR_IN          \
        in_dword(HWIO_GCC_TZ_USB30_SEC_SGDSCR_ADDR)
#define HWIO_GCC_TZ_USB30_SEC_SGDSCR_INM(m)      \
        in_dword_masked(HWIO_GCC_TZ_USB30_SEC_SGDSCR_ADDR, m)
#define HWIO_GCC_TZ_USB30_SEC_SGDSCR_OUT(v)      \
        out_dword(HWIO_GCC_TZ_USB30_SEC_SGDSCR_ADDR,v)
#define HWIO_GCC_TZ_USB30_SEC_SGDSCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GCC_TZ_USB30_SEC_SGDSCR_ADDR,m,v,HWIO_GCC_TZ_USB30_SEC_SGDSCR_IN)
#define HWIO_GCC_TZ_USB30_SEC_SGDSCR_RETAIN_FF_ENABLE_BMSK                                                0x2
#define HWIO_GCC_TZ_USB30_SEC_SGDSCR_RETAIN_FF_ENABLE_SHFT                                                0x1
#define HWIO_GCC_TZ_USB30_SEC_SGDSCR_RETAIN_FF_ENABLE_DISABLE_FVAL                                        0x0
#define HWIO_GCC_TZ_USB30_SEC_SGDSCR_RETAIN_FF_ENABLE_ENABLE_FVAL                                         0x1
#define HWIO_GCC_TZ_USB30_SEC_SGDSCR_SW_OVERRIDE_BMSK                                                     0x1
#define HWIO_GCC_TZ_USB30_SEC_SGDSCR_SW_OVERRIDE_SHFT                                                     0x0
#define HWIO_GCC_TZ_USB30_SEC_SGDSCR_SW_OVERRIDE_DISABLE_FVAL                                             0x0
#define HWIO_GCC_TZ_USB30_SEC_SGDSCR_SW_OVERRIDE_ENABLE_FVAL                                              0x1

#define HWIO_GCC_MSS_USB30_PRIM_SGDSCR_ADDR                                                        (GCC_CLK_CTL_REG_REG_BASE      + 0x0003e000)
#define HWIO_GCC_MSS_USB30_PRIM_SGDSCR_PHYS                                                        (GCC_CLK_CTL_REG_REG_BASE_PHYS + 0x0003e000)
#define HWIO_GCC_MSS_USB30_PRIM_SGDSCR_OFFS                                                        (GCC_CLK_CTL_REG_REG_BASE_OFFS + 0x0003e000)
#define HWIO_GCC_MSS_USB30_PRIM_SGDSCR_RMSK                                                               0x3
#define HWIO_GCC_MSS_USB30_PRIM_SGDSCR_IN          \
        in_dword(HWIO_GCC_MSS_USB30_PRIM_SGDSCR_ADDR)
#define HWIO_GCC_MSS_USB30_PRIM_SGDSCR_INM(m)      \
        in_dword_masked(HWIO_GCC_MSS_USB30_PRIM_SGDSCR_ADDR, m)
#define HWIO_GCC_MSS_USB30_PRIM_SGDSCR_OUT(v)      \
        out_dword(HWIO_GCC_MSS_USB30_PRIM_SGDSCR_ADDR,v)
#define HWIO_GCC_MSS_USB30_PRIM_SGDSCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GCC_MSS_USB30_PRIM_SGDSCR_ADDR,m,v,HWIO_GCC_MSS_USB30_PRIM_SGDSCR_IN)
#define HWIO_GCC_MSS_USB30_PRIM_SGDSCR_RETAIN_FF_ENABLE_BMSK                                              0x2
#define HWIO_GCC_MSS_USB30_PRIM_SGDSCR_RETAIN_FF_ENABLE_SHFT                                              0x1
#define HWIO_GCC_MSS_USB30_PRIM_SGDSCR_RETAIN_FF_ENABLE_DISABLE_FVAL                                      0x0
#define HWIO_GCC_MSS_USB30_PRIM_SGDSCR_RETAIN_FF_ENABLE_ENABLE_FVAL                                       0x1
#define HWIO_GCC_MSS_USB30_PRIM_SGDSCR_SW_OVERRIDE_BMSK                                                   0x1
#define HWIO_GCC_MSS_USB30_PRIM_SGDSCR_SW_OVERRIDE_SHFT                                                   0x0
#define HWIO_GCC_MSS_USB30_PRIM_SGDSCR_SW_OVERRIDE_DISABLE_FVAL                                           0x0
#define HWIO_GCC_MSS_USB30_PRIM_SGDSCR_SW_OVERRIDE_ENABLE_FVAL                                            0x1

#define HWIO_GCC_MSS_USB30_MP_SGDSCR_ADDR                                                          (GCC_CLK_CTL_REG_REG_BASE      + 0x0003e004)
#define HWIO_GCC_MSS_USB30_MP_SGDSCR_PHYS                                                          (GCC_CLK_CTL_REG_REG_BASE_PHYS + 0x0003e004)
#define HWIO_GCC_MSS_USB30_MP_SGDSCR_OFFS                                                          (GCC_CLK_CTL_REG_REG_BASE_OFFS + 0x0003e004)
#define HWIO_GCC_MSS_USB30_MP_SGDSCR_RMSK                                                                 0x3
#define HWIO_GCC_MSS_USB30_MP_SGDSCR_IN          \
        in_dword(HWIO_GCC_MSS_USB30_MP_SGDSCR_ADDR)
#define HWIO_GCC_MSS_USB30_MP_SGDSCR_INM(m)      \
        in_dword_masked(HWIO_GCC_MSS_USB30_MP_SGDSCR_ADDR, m)
#define HWIO_GCC_MSS_USB30_MP_SGDSCR_OUT(v)      \
        out_dword(HWIO_GCC_MSS_USB30_MP_SGDSCR_ADDR,v)
#define HWIO_GCC_MSS_USB30_MP_SGDSCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GCC_MSS_USB30_MP_SGDSCR_ADDR,m,v,HWIO_GCC_MSS_USB30_MP_SGDSCR_IN)
#define HWIO_GCC_MSS_USB30_MP_SGDSCR_RETAIN_FF_ENABLE_BMSK                                                0x2
#define HWIO_GCC_MSS_USB30_MP_SGDSCR_RETAIN_FF_ENABLE_SHFT                                                0x1
#define HWIO_GCC_MSS_USB30_MP_SGDSCR_RETAIN_FF_ENABLE_DISABLE_FVAL                                        0x0
#define HWIO_GCC_MSS_USB30_MP_SGDSCR_RETAIN_FF_ENABLE_ENABLE_FVAL                                         0x1
#define HWIO_GCC_MSS_USB30_MP_SGDSCR_SW_OVERRIDE_BMSK                                                     0x1
#define HWIO_GCC_MSS_USB30_MP_SGDSCR_SW_OVERRIDE_SHFT                                                     0x0
#define HWIO_GCC_MSS_USB30_MP_SGDSCR_SW_OVERRIDE_DISABLE_FVAL                                             0x0
#define HWIO_GCC_MSS_USB30_MP_SGDSCR_SW_OVERRIDE_ENABLE_FVAL                                              0x1

#define HWIO_GCC_MSS_USB30_SEC_SGDSCR_ADDR                                                         (GCC_CLK_CTL_REG_REG_BASE      + 0x0003e030)
#define HWIO_GCC_MSS_USB30_SEC_SGDSCR_PHYS                                                         (GCC_CLK_CTL_REG_REG_BASE_PHYS + 0x0003e030)
#define HWIO_GCC_MSS_USB30_SEC_SGDSCR_OFFS                                                         (GCC_CLK_CTL_REG_REG_BASE_OFFS + 0x0003e030)
#define HWIO_GCC_MSS_USB30_SEC_SGDSCR_RMSK                                                                0x3
#define HWIO_GCC_MSS_USB30_SEC_SGDSCR_IN          \
        in_dword(HWIO_GCC_MSS_USB30_SEC_SGDSCR_ADDR)
#define HWIO_GCC_MSS_USB30_SEC_SGDSCR_INM(m)      \
        in_dword_masked(HWIO_GCC_MSS_USB30_SEC_SGDSCR_ADDR, m)
#define HWIO_GCC_MSS_USB30_SEC_SGDSCR_OUT(v)      \
        out_dword(HWIO_GCC_MSS_USB30_SEC_SGDSCR_ADDR,v)
#define HWIO_GCC_MSS_USB30_SEC_SGDSCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GCC_MSS_USB30_SEC_SGDSCR_ADDR,m,v,HWIO_GCC_MSS_USB30_SEC_SGDSCR_IN)
#define HWIO_GCC_MSS_USB30_SEC_SGDSCR_RETAIN_FF_ENABLE_BMSK                                               0x2
#define HWIO_GCC_MSS_USB30_SEC_SGDSCR_RETAIN_FF_ENABLE_SHFT                                               0x1
#define HWIO_GCC_MSS_USB30_SEC_SGDSCR_RETAIN_FF_ENABLE_DISABLE_FVAL                                       0x0
#define HWIO_GCC_MSS_USB30_SEC_SGDSCR_RETAIN_FF_ENABLE_ENABLE_FVAL                                        0x1
#define HWIO_GCC_MSS_USB30_SEC_SGDSCR_SW_OVERRIDE_BMSK                                                    0x1
#define HWIO_GCC_MSS_USB30_SEC_SGDSCR_SW_OVERRIDE_SHFT                                                    0x0
#define HWIO_GCC_MSS_USB30_SEC_SGDSCR_SW_OVERRIDE_DISABLE_FVAL                                            0x0
#define HWIO_GCC_MSS_USB30_SEC_SGDSCR_SW_OVERRIDE_ENABLE_FVAL                                             0x1

#define HWIO_GCC_SP_USB30_PRIM_SGDSCR_ADDR                                                         (GCC_CLK_CTL_REG_REG_BASE      + 0x0003f000)
#define HWIO_GCC_SP_USB30_PRIM_SGDSCR_PHYS                                                         (GCC_CLK_CTL_REG_REG_BASE_PHYS + 0x0003f000)
#define HWIO_GCC_SP_USB30_PRIM_SGDSCR_OFFS                                                         (GCC_CLK_CTL_REG_REG_BASE_OFFS + 0x0003f000)
#define HWIO_GCC_SP_USB30_PRIM_SGDSCR_RMSK                                                                0x3
#define HWIO_GCC_SP_USB30_PRIM_SGDSCR_IN          \
        in_dword(HWIO_GCC_SP_USB30_PRIM_SGDSCR_ADDR)
#define HWIO_GCC_SP_USB30_PRIM_SGDSCR_INM(m)      \
        in_dword_masked(HWIO_GCC_SP_USB30_PRIM_SGDSCR_ADDR, m)
#define HWIO_GCC_SP_USB30_PRIM_SGDSCR_OUT(v)      \
        out_dword(HWIO_GCC_SP_USB30_PRIM_SGDSCR_ADDR,v)
#define HWIO_GCC_SP_USB30_PRIM_SGDSCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GCC_SP_USB30_PRIM_SGDSCR_ADDR,m,v,HWIO_GCC_SP_USB30_PRIM_SGDSCR_IN)
#define HWIO_GCC_SP_USB30_PRIM_SGDSCR_RETAIN_FF_ENABLE_BMSK                                               0x2
#define HWIO_GCC_SP_USB30_PRIM_SGDSCR_RETAIN_FF_ENABLE_SHFT                                               0x1
#define HWIO_GCC_SP_USB30_PRIM_SGDSCR_RETAIN_FF_ENABLE_DISABLE_FVAL                                       0x0
#define HWIO_GCC_SP_USB30_PRIM_SGDSCR_RETAIN_FF_ENABLE_ENABLE_FVAL                                        0x1
#define HWIO_GCC_SP_USB30_PRIM_SGDSCR_SW_OVERRIDE_BMSK                                                    0x1
#define HWIO_GCC_SP_USB30_PRIM_SGDSCR_SW_OVERRIDE_SHFT                                                    0x0
#define HWIO_GCC_SP_USB30_PRIM_SGDSCR_SW_OVERRIDE_DISABLE_FVAL                                            0x0
#define HWIO_GCC_SP_USB30_PRIM_SGDSCR_SW_OVERRIDE_ENABLE_FVAL                                             0x1

#define HWIO_GCC_SP_USB30_MP_SGDSCR_ADDR                                                           (GCC_CLK_CTL_REG_REG_BASE      + 0x0003f004)
#define HWIO_GCC_SP_USB30_MP_SGDSCR_PHYS                                                           (GCC_CLK_CTL_REG_REG_BASE_PHYS + 0x0003f004)
#define HWIO_GCC_SP_USB30_MP_SGDSCR_OFFS                                                           (GCC_CLK_CTL_REG_REG_BASE_OFFS + 0x0003f004)
#define HWIO_GCC_SP_USB30_MP_SGDSCR_RMSK                                                                  0x3
#define HWIO_GCC_SP_USB30_MP_SGDSCR_IN          \
        in_dword(HWIO_GCC_SP_USB30_MP_SGDSCR_ADDR)
#define HWIO_GCC_SP_USB30_MP_SGDSCR_INM(m)      \
        in_dword_masked(HWIO_GCC_SP_USB30_MP_SGDSCR_ADDR, m)
#define HWIO_GCC_SP_USB30_MP_SGDSCR_OUT(v)      \
        out_dword(HWIO_GCC_SP_USB30_MP_SGDSCR_ADDR,v)
#define HWIO_GCC_SP_USB30_MP_SGDSCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GCC_SP_USB30_MP_SGDSCR_ADDR,m,v,HWIO_GCC_SP_USB30_MP_SGDSCR_IN)
#define HWIO_GCC_SP_USB30_MP_SGDSCR_RETAIN_FF_ENABLE_BMSK                                                 0x2
#define HWIO_GCC_SP_USB30_MP_SGDSCR_RETAIN_FF_ENABLE_SHFT                                                 0x1
#define HWIO_GCC_SP_USB30_MP_SGDSCR_RETAIN_FF_ENABLE_DISABLE_FVAL                                         0x0
#define HWIO_GCC_SP_USB30_MP_SGDSCR_RETAIN_FF_ENABLE_ENABLE_FVAL                                          0x1
#define HWIO_GCC_SP_USB30_MP_SGDSCR_SW_OVERRIDE_BMSK                                                      0x1
#define HWIO_GCC_SP_USB30_MP_SGDSCR_SW_OVERRIDE_SHFT                                                      0x0
#define HWIO_GCC_SP_USB30_MP_SGDSCR_SW_OVERRIDE_DISABLE_FVAL                                              0x0
#define HWIO_GCC_SP_USB30_MP_SGDSCR_SW_OVERRIDE_ENABLE_FVAL                                               0x1

#define HWIO_GCC_SP_USB30_SEC_SGDSCR_ADDR                                                          (GCC_CLK_CTL_REG_REG_BASE      + 0x0003f030)
#define HWIO_GCC_SP_USB30_SEC_SGDSCR_PHYS                                                          (GCC_CLK_CTL_REG_REG_BASE_PHYS + 0x0003f030)
#define HWIO_GCC_SP_USB30_SEC_SGDSCR_OFFS                                                          (GCC_CLK_CTL_REG_REG_BASE_OFFS + 0x0003f030)
#define HWIO_GCC_SP_USB30_SEC_SGDSCR_RMSK                                                                 0x3
#define HWIO_GCC_SP_USB30_SEC_SGDSCR_IN          \
        in_dword(HWIO_GCC_SP_USB30_SEC_SGDSCR_ADDR)
#define HWIO_GCC_SP_USB30_SEC_SGDSCR_INM(m)      \
        in_dword_masked(HWIO_GCC_SP_USB30_SEC_SGDSCR_ADDR, m)
#define HWIO_GCC_SP_USB30_SEC_SGDSCR_OUT(v)      \
        out_dword(HWIO_GCC_SP_USB30_SEC_SGDSCR_ADDR,v)
#define HWIO_GCC_SP_USB30_SEC_SGDSCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GCC_SP_USB30_SEC_SGDSCR_ADDR,m,v,HWIO_GCC_SP_USB30_SEC_SGDSCR_IN)
#define HWIO_GCC_SP_USB30_SEC_SGDSCR_RETAIN_FF_ENABLE_BMSK                                                0x2
#define HWIO_GCC_SP_USB30_SEC_SGDSCR_RETAIN_FF_ENABLE_SHFT                                                0x1
#define HWIO_GCC_SP_USB30_SEC_SGDSCR_RETAIN_FF_ENABLE_DISABLE_FVAL                                        0x0
#define HWIO_GCC_SP_USB30_SEC_SGDSCR_RETAIN_FF_ENABLE_ENABLE_FVAL                                         0x1
#define HWIO_GCC_SP_USB30_SEC_SGDSCR_SW_OVERRIDE_BMSK                                                     0x1
#define HWIO_GCC_SP_USB30_SEC_SGDSCR_SW_OVERRIDE_SHFT                                                     0x0
#define HWIO_GCC_SP_USB30_SEC_SGDSCR_SW_OVERRIDE_DISABLE_FVAL                                             0x0
#define HWIO_GCC_SP_USB30_SEC_SGDSCR_SW_OVERRIDE_ENABLE_FVAL                                              0x1

#define HWIO_GCC_USB3_PRIM_PHY_PIPE_MUXR_ADDR                                                      (GCC_CLK_CTL_REG_REG_BASE      + 0x0000f05c)
#define HWIO_GCC_USB3_PRIM_PHY_PIPE_MUXR_PHYS                                                      (GCC_CLK_CTL_REG_REG_BASE_PHYS + 0x0000f05c)
#define HWIO_GCC_USB3_PRIM_PHY_PIPE_MUXR_OFFS                                                      (GCC_CLK_CTL_REG_REG_BASE_OFFS + 0x0000f05c)
#define HWIO_GCC_USB3_PRIM_PHY_PIPE_MUXR_RMSK                                                             0x3
#define HWIO_GCC_USB3_PRIM_PHY_PIPE_MUXR_IN          \
        in_dword(HWIO_GCC_USB3_PRIM_PHY_PIPE_MUXR_ADDR)
#define HWIO_GCC_USB3_PRIM_PHY_PIPE_MUXR_INM(m)      \
        in_dword_masked(HWIO_GCC_USB3_PRIM_PHY_PIPE_MUXR_ADDR, m)
#define HWIO_GCC_USB3_PRIM_PHY_PIPE_MUXR_OUT(v)      \
        out_dword(HWIO_GCC_USB3_PRIM_PHY_PIPE_MUXR_ADDR,v)
#define HWIO_GCC_USB3_PRIM_PHY_PIPE_MUXR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GCC_USB3_PRIM_PHY_PIPE_MUXR_ADDR,m,v,HWIO_GCC_USB3_PRIM_PHY_PIPE_MUXR_IN)
#define HWIO_GCC_USB3_PRIM_PHY_PIPE_MUXR_MUX_SEL_BMSK                                                     0x3
#define HWIO_GCC_USB3_PRIM_PHY_PIPE_MUXR_MUX_SEL_SHFT                                                     0x0

#define HWIO_GCC_USB3_SEC_PHY_PIPE_MUXR_ADDR                                                       (GCC_CLK_CTL_REG_REG_BASE      + 0x0001005c)
#define HWIO_GCC_USB3_SEC_PHY_PIPE_MUXR_PHYS                                                       (GCC_CLK_CTL_REG_REG_BASE_PHYS + 0x0001005c)
#define HWIO_GCC_USB3_SEC_PHY_PIPE_MUXR_OFFS                                                       (GCC_CLK_CTL_REG_REG_BASE_OFFS + 0x0001005c)
#define HWIO_GCC_USB3_SEC_PHY_PIPE_MUXR_RMSK                                                              0x3
#define HWIO_GCC_USB3_SEC_PHY_PIPE_MUXR_IN          \
        in_dword(HWIO_GCC_USB3_SEC_PHY_PIPE_MUXR_ADDR)
#define HWIO_GCC_USB3_SEC_PHY_PIPE_MUXR_INM(m)      \
        in_dword_masked(HWIO_GCC_USB3_SEC_PHY_PIPE_MUXR_ADDR, m)
#define HWIO_GCC_USB3_SEC_PHY_PIPE_MUXR_OUT(v)      \
        out_dword(HWIO_GCC_USB3_SEC_PHY_PIPE_MUXR_ADDR,v)
#define HWIO_GCC_USB3_SEC_PHY_PIPE_MUXR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GCC_USB3_SEC_PHY_PIPE_MUXR_ADDR,m,v,HWIO_GCC_USB3_SEC_PHY_PIPE_MUXR_IN)
#define HWIO_GCC_USB3_SEC_PHY_PIPE_MUXR_MUX_SEL_BMSK                                                      0x3
#define HWIO_GCC_USB3_SEC_PHY_PIPE_MUXR_MUX_SEL_SHFT                                                      0x0

#define HWIO_GCC_USB3_MP_PHY_PIPE_0_MUXR_ADDR                                                      (GCC_CLK_CTL_REG_REG_BASE      + 0x000a6060)
#define HWIO_GCC_USB3_MP_PHY_PIPE_0_MUXR_PHYS                                                      (GCC_CLK_CTL_REG_REG_BASE_PHYS + 0x000a6060)
#define HWIO_GCC_USB3_MP_PHY_PIPE_0_MUXR_OFFS                                                      (GCC_CLK_CTL_REG_REG_BASE_OFFS + 0x000a6060)
#define HWIO_GCC_USB3_MP_PHY_PIPE_0_MUXR_RMSK                                                             0x3
#define HWIO_GCC_USB3_MP_PHY_PIPE_0_MUXR_IN          \
        in_dword(HWIO_GCC_USB3_MP_PHY_PIPE_0_MUXR_ADDR)
#define HWIO_GCC_USB3_MP_PHY_PIPE_0_MUXR_INM(m)      \
        in_dword_masked(HWIO_GCC_USB3_MP_PHY_PIPE_0_MUXR_ADDR, m)
#define HWIO_GCC_USB3_MP_PHY_PIPE_0_MUXR_OUT(v)      \
        out_dword(HWIO_GCC_USB3_MP_PHY_PIPE_0_MUXR_ADDR,v)
#define HWIO_GCC_USB3_MP_PHY_PIPE_0_MUXR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GCC_USB3_MP_PHY_PIPE_0_MUXR_ADDR,m,v,HWIO_GCC_USB3_MP_PHY_PIPE_0_MUXR_IN)
#define HWIO_GCC_USB3_MP_PHY_PIPE_0_MUXR_MUX_SEL_BMSK                                                     0x3
#define HWIO_GCC_USB3_MP_PHY_PIPE_0_MUXR_MUX_SEL_SHFT                                                     0x0

#define HWIO_GCC_USB3_MP_PHY_PIPE_1_MUXR_ADDR                                                      (GCC_CLK_CTL_REG_REG_BASE      + 0x000a6064)
#define HWIO_GCC_USB3_MP_PHY_PIPE_1_MUXR_PHYS                                                      (GCC_CLK_CTL_REG_REG_BASE_PHYS + 0x000a6064)
#define HWIO_GCC_USB3_MP_PHY_PIPE_1_MUXR_OFFS                                                      (GCC_CLK_CTL_REG_REG_BASE_OFFS + 0x000a6064)
#define HWIO_GCC_USB3_MP_PHY_PIPE_1_MUXR_RMSK                                                             0x3
#define HWIO_GCC_USB3_MP_PHY_PIPE_1_MUXR_IN          \
        in_dword(HWIO_GCC_USB3_MP_PHY_PIPE_1_MUXR_ADDR)
#define HWIO_GCC_USB3_MP_PHY_PIPE_1_MUXR_INM(m)      \
        in_dword_masked(HWIO_GCC_USB3_MP_PHY_PIPE_1_MUXR_ADDR, m)
#define HWIO_GCC_USB3_MP_PHY_PIPE_1_MUXR_OUT(v)      \
        out_dword(HWIO_GCC_USB3_MP_PHY_PIPE_1_MUXR_ADDR,v)
#define HWIO_GCC_USB3_MP_PHY_PIPE_1_MUXR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GCC_USB3_MP_PHY_PIPE_1_MUXR_ADDR,m,v,HWIO_GCC_USB3_MP_PHY_PIPE_1_MUXR_IN)
#define HWIO_GCC_USB3_MP_PHY_PIPE_1_MUXR_MUX_SEL_BMSK                                                     0x3
#define HWIO_GCC_USB3_MP_PHY_PIPE_1_MUXR_MUX_SEL_SHFT                                                     0x0

#define HWIO_GCC_USB_BOOT_CLOCK_CTL_ADDR                                                           (GCC_CLK_CTL_REG_REG_BASE      + 0x00068000)
#define HWIO_GCC_USB_BOOT_CLOCK_CTL_PHYS                                                           (GCC_CLK_CTL_REG_REG_BASE_PHYS + 0x00068000)
#define HWIO_GCC_USB_BOOT_CLOCK_CTL_OFFS                                                           (GCC_CLK_CTL_REG_REG_BASE_OFFS + 0x00068000)
#define HWIO_GCC_USB_BOOT_CLOCK_CTL_RMSK                                                                  0x1
#define HWIO_GCC_USB_BOOT_CLOCK_CTL_IN          \
        in_dword(HWIO_GCC_USB_BOOT_CLOCK_CTL_ADDR)
#define HWIO_GCC_USB_BOOT_CLOCK_CTL_INM(m)      \
        in_dword_masked(HWIO_GCC_USB_BOOT_CLOCK_CTL_ADDR, m)
#define HWIO_GCC_USB_BOOT_CLOCK_CTL_OUT(v)      \
        out_dword(HWIO_GCC_USB_BOOT_CLOCK_CTL_ADDR,v)
#define HWIO_GCC_USB_BOOT_CLOCK_CTL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GCC_USB_BOOT_CLOCK_CTL_ADDR,m,v,HWIO_GCC_USB_BOOT_CLOCK_CTL_IN)
#define HWIO_GCC_USB_BOOT_CLOCK_CTL_CLK_ENABLE_BMSK                                                       0x1
#define HWIO_GCC_USB_BOOT_CLOCK_CTL_CLK_ENABLE_SHFT                                                       0x0
#define HWIO_GCC_USB_BOOT_CLOCK_CTL_CLK_ENABLE_DISABLE_FVAL                                               0x0
#define HWIO_GCC_USB_BOOT_CLOCK_CTL_CLK_ENABLE_ENABLE_FVAL                                                0x1

#define HWIO_GCC_USB3_MP0_CLKREF_EN_ADDR                                                           (GCC_CLK_CTL_REG_REG_BASE      + 0x0008c000)
#define HWIO_GCC_USB3_MP0_CLKREF_EN_PHYS                                                           (GCC_CLK_CTL_REG_REG_BASE_PHYS + 0x0008c000)
#define HWIO_GCC_USB3_MP0_CLKREF_EN_OFFS                                                           (GCC_CLK_CTL_REG_REG_BASE_OFFS + 0x0008c000)
#define HWIO_GCC_USB3_MP0_CLKREF_EN_RMSK                                                           0x80000001
#define HWIO_GCC_USB3_MP0_CLKREF_EN_IN          \
        in_dword(HWIO_GCC_USB3_MP0_CLKREF_EN_ADDR)
#define HWIO_GCC_USB3_MP0_CLKREF_EN_INM(m)      \
        in_dword_masked(HWIO_GCC_USB3_MP0_CLKREF_EN_ADDR, m)
#define HWIO_GCC_USB3_MP0_CLKREF_EN_OUT(v)      \
        out_dword(HWIO_GCC_USB3_MP0_CLKREF_EN_ADDR,v)
#define HWIO_GCC_USB3_MP0_CLKREF_EN_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GCC_USB3_MP0_CLKREF_EN_ADDR,m,v,HWIO_GCC_USB3_MP0_CLKREF_EN_IN)
#define HWIO_GCC_USB3_MP0_CLKREF_EN_USB3_STATUS_BMSK                                               0x80000000
#define HWIO_GCC_USB3_MP0_CLKREF_EN_USB3_STATUS_SHFT                                                     0x1f
#define HWIO_GCC_USB3_MP0_CLKREF_EN_USB3_ENABLE_BMSK                                                      0x1
#define HWIO_GCC_USB3_MP0_CLKREF_EN_USB3_ENABLE_SHFT                                                      0x0
#define HWIO_GCC_USB3_MP0_CLKREF_EN_USB3_ENABLE_DISABLE_FVAL                                              0x0
#define HWIO_GCC_USB3_MP0_CLKREF_EN_USB3_ENABLE_ENABLE_FVAL                                               0x1

#define HWIO_GCC_USB3_MP1_CLKREF_EN_ADDR                                                           (GCC_CLK_CTL_REG_REG_BASE      + 0x0008c004)
#define HWIO_GCC_USB3_MP1_CLKREF_EN_PHYS                                                           (GCC_CLK_CTL_REG_REG_BASE_PHYS + 0x0008c004)
#define HWIO_GCC_USB3_MP1_CLKREF_EN_OFFS                                                           (GCC_CLK_CTL_REG_REG_BASE_OFFS + 0x0008c004)
#define HWIO_GCC_USB3_MP1_CLKREF_EN_RMSK                                                           0x80000001
#define HWIO_GCC_USB3_MP1_CLKREF_EN_IN          \
        in_dword(HWIO_GCC_USB3_MP1_CLKREF_EN_ADDR)
#define HWIO_GCC_USB3_MP1_CLKREF_EN_INM(m)      \
        in_dword_masked(HWIO_GCC_USB3_MP1_CLKREF_EN_ADDR, m)
#define HWIO_GCC_USB3_MP1_CLKREF_EN_OUT(v)      \
        out_dword(HWIO_GCC_USB3_MP1_CLKREF_EN_ADDR,v)
#define HWIO_GCC_USB3_MP1_CLKREF_EN_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GCC_USB3_MP1_CLKREF_EN_ADDR,m,v,HWIO_GCC_USB3_MP1_CLKREF_EN_IN)
#define HWIO_GCC_USB3_MP1_CLKREF_EN_USB3_STATUS_BMSK                                               0x80000000
#define HWIO_GCC_USB3_MP1_CLKREF_EN_USB3_STATUS_SHFT                                                     0x1f
#define HWIO_GCC_USB3_MP1_CLKREF_EN_USB3_ENABLE_BMSK                                                      0x1
#define HWIO_GCC_USB3_MP1_CLKREF_EN_USB3_ENABLE_SHFT                                                      0x0
#define HWIO_GCC_USB3_MP1_CLKREF_EN_USB3_ENABLE_DISABLE_FVAL                                              0x0
#define HWIO_GCC_USB3_MP1_CLKREF_EN_USB3_ENABLE_ENABLE_FVAL                                               0x1

#define HWIO_GCC_USB3_PRIM_CLKREF_EN_ADDR                                                          (GCC_CLK_CTL_REG_REG_BASE      + 0x0008c008)
#define HWIO_GCC_USB3_PRIM_CLKREF_EN_PHYS                                                          (GCC_CLK_CTL_REG_REG_BASE_PHYS + 0x0008c008)
#define HWIO_GCC_USB3_PRIM_CLKREF_EN_OFFS                                                          (GCC_CLK_CTL_REG_REG_BASE_OFFS + 0x0008c008)
#define HWIO_GCC_USB3_PRIM_CLKREF_EN_RMSK                                                          0x80000001
#define HWIO_GCC_USB3_PRIM_CLKREF_EN_IN          \
        in_dword(HWIO_GCC_USB3_PRIM_CLKREF_EN_ADDR)
#define HWIO_GCC_USB3_PRIM_CLKREF_EN_INM(m)      \
        in_dword_masked(HWIO_GCC_USB3_PRIM_CLKREF_EN_ADDR, m)
#define HWIO_GCC_USB3_PRIM_CLKREF_EN_OUT(v)      \
        out_dword(HWIO_GCC_USB3_PRIM_CLKREF_EN_ADDR,v)
#define HWIO_GCC_USB3_PRIM_CLKREF_EN_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GCC_USB3_PRIM_CLKREF_EN_ADDR,m,v,HWIO_GCC_USB3_PRIM_CLKREF_EN_IN)
#define HWIO_GCC_USB3_PRIM_CLKREF_EN_USB3_STATUS_BMSK                                              0x80000000
#define HWIO_GCC_USB3_PRIM_CLKREF_EN_USB3_STATUS_SHFT                                                    0x1f
#define HWIO_GCC_USB3_PRIM_CLKREF_EN_USB3_ENABLE_BMSK                                                     0x1
#define HWIO_GCC_USB3_PRIM_CLKREF_EN_USB3_ENABLE_SHFT                                                     0x0
#define HWIO_GCC_USB3_PRIM_CLKREF_EN_USB3_ENABLE_DISABLE_FVAL                                             0x0
#define HWIO_GCC_USB3_PRIM_CLKREF_EN_USB3_ENABLE_ENABLE_FVAL                                              0x1

#define HWIO_GCC_USB3_SEC_CLKREF_EN_ADDR                                                           (GCC_CLK_CTL_REG_REG_BASE      + 0x0008c028)
#define HWIO_GCC_USB3_SEC_CLKREF_EN_PHYS                                                           (GCC_CLK_CTL_REG_REG_BASE_PHYS + 0x0008c028)
#define HWIO_GCC_USB3_SEC_CLKREF_EN_OFFS                                                           (GCC_CLK_CTL_REG_REG_BASE_OFFS + 0x0008c028)
#define HWIO_GCC_USB3_SEC_CLKREF_EN_RMSK                                                           0x80000001
#define HWIO_GCC_USB3_SEC_CLKREF_EN_IN          \
        in_dword(HWIO_GCC_USB3_SEC_CLKREF_EN_ADDR)
#define HWIO_GCC_USB3_SEC_CLKREF_EN_INM(m)      \
        in_dword_masked(HWIO_GCC_USB3_SEC_CLKREF_EN_ADDR, m)
#define HWIO_GCC_USB3_SEC_CLKREF_EN_OUT(v)      \
        out_dword(HWIO_GCC_USB3_SEC_CLKREF_EN_ADDR,v)
#define HWIO_GCC_USB3_SEC_CLKREF_EN_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GCC_USB3_SEC_CLKREF_EN_ADDR,m,v,HWIO_GCC_USB3_SEC_CLKREF_EN_IN)
#define HWIO_GCC_USB3_SEC_CLKREF_EN_USB3_STATUS_BMSK                                               0x80000000
#define HWIO_GCC_USB3_SEC_CLKREF_EN_USB3_STATUS_SHFT                                                     0x1f
#define HWIO_GCC_USB3_SEC_CLKREF_EN_USB3_ENABLE_BMSK                                                      0x1
#define HWIO_GCC_USB3_SEC_CLKREF_EN_USB3_ENABLE_SHFT                                                      0x0
#define HWIO_GCC_USB3_SEC_CLKREF_EN_USB3_ENABLE_DISABLE_FVAL                                              0x0
#define HWIO_GCC_USB3_SEC_CLKREF_EN_USB3_ENABLE_ENABLE_FVAL                                               0x1


#endif /* __GCCREG__ */