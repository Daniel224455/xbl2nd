#ifndef __HALDSI_PHY_2_3_0_H__
#define __HALDSI_PHY_2_3_0_H__
/*
===========================================================================
*/
/**
  @file HALdsi_Phy_2_3_0.h
  @brief Auto-generated HWIO interface include file.

  This file contains HWIO register definitions for the following modules:
    DSI_0_PHY.*
    DSI_1_PHY.*

  'Include' filters applied: <none>
  'Exclude' filters applied: RESERVED DUMMY 
*/
/*
  ===========================================================================

  Copyright (c) 2020 Qualcomm Technologies Incorporated.
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


  ===========================================================================
*/

/*----------------------------------------------------------------------------
 * MODULE: DSI_0_PHY
 *--------------------------------------------------------------------------*/

#define DSI_0_PHY_REG_BASE   (MDSS_BASE      + 0x00094400)

/*----------------------------------------------------------------------------
 * MODULE: DSI_0_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON
 *--------------------------------------------------------------------------*/

#define DSI_0_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE                                            (MDSS_BASE      + 0x00094400)

#define HWIO_DSI_0_PHY_DSIPHY_CMN_REVISION_ID0_ADDR                                                    (DSI_0_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x00000000)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_REVISION_ID0_RMSK                                                          0xff
#define HWIO_DSI_0_PHY_DSIPHY_CMN_REVISION_ID0_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_REVISION_ID0_ADDR, HWIO_DSI_0_PHY_DSIPHY_CMN_REVISION_ID0_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_REVISION_ID0_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_REVISION_ID0_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_REVISION_ID0_MINOR_3_0_BMSK                                                0xf0
#define HWIO_DSI_0_PHY_DSIPHY_CMN_REVISION_ID0_MINOR_3_0_SHFT                                                 0x4
#define HWIO_DSI_0_PHY_DSIPHY_CMN_REVISION_ID0_MAJOR_BMSK                                                     0xf
#define HWIO_DSI_0_PHY_DSIPHY_CMN_REVISION_ID0_MAJOR_SHFT                                                     0x0

#define HWIO_DSI_0_PHY_DSIPHY_CMN_REVISION_ID1_ADDR                                                    (DSI_0_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x00000004)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_REVISION_ID1_RMSK                                                          0xff
#define HWIO_DSI_0_PHY_DSIPHY_CMN_REVISION_ID1_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_REVISION_ID1_ADDR, HWIO_DSI_0_PHY_DSIPHY_CMN_REVISION_ID1_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_REVISION_ID1_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_REVISION_ID1_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_REVISION_ID1_MINOR_11_4_BMSK                                               0xff
#define HWIO_DSI_0_PHY_DSIPHY_CMN_REVISION_ID1_MINOR_11_4_SHFT                                                0x0

#define HWIO_DSI_0_PHY_DSIPHY_CMN_REVISION_ID2_ADDR                                                    (DSI_0_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x00000008)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_REVISION_ID2_RMSK                                                          0xff
#define HWIO_DSI_0_PHY_DSIPHY_CMN_REVISION_ID2_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_REVISION_ID2_ADDR, HWIO_DSI_0_PHY_DSIPHY_CMN_REVISION_ID2_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_REVISION_ID2_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_REVISION_ID2_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_REVISION_ID2_STEP_7_0_BMSK                                                 0xff
#define HWIO_DSI_0_PHY_DSIPHY_CMN_REVISION_ID2_STEP_7_0_SHFT                                                  0x0

#define HWIO_DSI_0_PHY_DSIPHY_CMN_REVISION_ID3_ADDR                                                    (DSI_0_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x0000000c)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_REVISION_ID3_RMSK                                                          0xff
#define HWIO_DSI_0_PHY_DSIPHY_CMN_REVISION_ID3_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_REVISION_ID3_ADDR, HWIO_DSI_0_PHY_DSIPHY_CMN_REVISION_ID3_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_REVISION_ID3_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_REVISION_ID3_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_REVISION_ID3_STEP_15_8_BMSK                                                0xff
#define HWIO_DSI_0_PHY_DSIPHY_CMN_REVISION_ID3_STEP_15_8_SHFT                                                 0x0

#define HWIO_DSI_0_PHY_DSIPHY_CMN_CLK_CFG0_ADDR                                                        (DSI_0_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x00000010)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CLK_CFG0_RMSK                                                              0xff
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CLK_CFG0_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_CLK_CFG0_ADDR, HWIO_DSI_0_PHY_DSIPHY_CMN_CLK_CFG0_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CLK_CFG0_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_CLK_CFG0_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CLK_CFG0_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_CMN_CLK_CFG0_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CLK_CFG0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_CMN_CLK_CFG0_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_CMN_CLK_CFG0_IN)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CLK_CFG0_DIV_CTRL_7_4_BMSK                                                 0xf0
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CLK_CFG0_DIV_CTRL_7_4_SHFT                                                  0x4
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CLK_CFG0_DIV_CTRL_3_0_BMSK                                                  0xf
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CLK_CFG0_DIV_CTRL_3_0_SHFT                                                  0x0

#define HWIO_DSI_0_PHY_DSIPHY_CMN_CLK_CFG1_ADDR                                                        (DSI_0_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x00000014)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CLK_CFG1_RMSK                                                              0x7f
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CLK_CFG1_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_CLK_CFG1_ADDR, HWIO_DSI_0_PHY_DSIPHY_CMN_CLK_CFG1_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CLK_CFG1_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_CLK_CFG1_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CLK_CFG1_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_CMN_CLK_CFG1_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CLK_CFG1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_CMN_CLK_CFG1_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_CMN_CLK_CFG1_IN)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CLK_CFG1_BITCLK_R_OUT_EN_BMSK                                              0x40
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CLK_CFG1_BITCLK_R_OUT_EN_SHFT                                               0x6
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CLK_CFG1_CLK_EN_BMSK                                                       0x20
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CLK_CFG1_CLK_EN_SHFT                                                        0x5
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CLK_CFG1_CLK_EN_SEL_BMSK                                                   0x10
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CLK_CFG1_CLK_EN_SEL_SHFT                                                    0x4
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CLK_CFG1_BITCLK_SEL_BMSK                                                    0xc
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CLK_CFG1_BITCLK_SEL_SHFT                                                    0x2
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CLK_CFG1_DSICLK_SEL_BMSK                                                    0x3
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CLK_CFG1_DSICLK_SEL_SHFT                                                    0x0

#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_CTRL_ADDR                                                       (DSI_0_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x00000018)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_CTRL_RMSK                                                             0x5f
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_CTRL_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_CTRL_ADDR, HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_CTRL_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_CTRL_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_CTRL_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_CTRL_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_CTRL_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_CTRL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_CTRL_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_CTRL_IN)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_CTRL_PHY_MODE_BMSK                                                    0x40
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_CTRL_PHY_MODE_SHFT                                                     0x6
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_CTRL_BYTECLK_SEL_BMSK                                                 0x18
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_CTRL_BYTECLK_SEL_SHFT                                                  0x3
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_CTRL_POSTMAPPER_ENABLE_BMSK                                            0x4
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_CTRL_POSTMAPPER_ENABLE_SHFT                                            0x2
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_CTRL_TEST_ESCCLK_SEL_BMSK                                              0x2
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_CTRL_TEST_ESCCLK_SEL_SHFT                                              0x1
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_CTRL_TEST_BYTECLK_SEL_BMSK                                             0x1
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_CTRL_TEST_BYTECLK_SEL_SHFT                                             0x0

#define HWIO_DSI_0_PHY_DSIPHY_CMN_RBUF_CTRL_ADDR                                                       (DSI_0_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x0000001c)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_RBUF_CTRL_RMSK                                                              0x3
#define HWIO_DSI_0_PHY_DSIPHY_CMN_RBUF_CTRL_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_RBUF_CTRL_ADDR, HWIO_DSI_0_PHY_DSIPHY_CMN_RBUF_CTRL_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_RBUF_CTRL_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_RBUF_CTRL_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_RBUF_CTRL_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_CMN_RBUF_CTRL_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_RBUF_CTRL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_CMN_RBUF_CTRL_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_CMN_RBUF_CTRL_IN)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_RBUF_CTRL_FORCE_RETIME_BUFFER_RESYNC_BMSK                                   0x2
#define HWIO_DSI_0_PHY_DSIPHY_CMN_RBUF_CTRL_FORCE_RETIME_BUFFER_RESYNC_SHFT                                   0x1
#define HWIO_DSI_0_PHY_DSIPHY_CMN_RBUF_CTRL_PULSE_RETIME_BUFFER_RESYNC_BMSK                                   0x1
#define HWIO_DSI_0_PHY_DSIPHY_CMN_RBUF_CTRL_PULSE_RETIME_BUFFER_RESYNC_SHFT                                   0x0

#define HWIO_DSI_0_PHY_DSIPHY_CMN_VREG_CTRL_0_ADDR                                                     (DSI_0_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x00000020)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_VREG_CTRL_0_RMSK                                                           0xff
#define HWIO_DSI_0_PHY_DSIPHY_CMN_VREG_CTRL_0_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_VREG_CTRL_0_ADDR, HWIO_DSI_0_PHY_DSIPHY_CMN_VREG_CTRL_0_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_VREG_CTRL_0_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_VREG_CTRL_0_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_VREG_CTRL_0_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_CMN_VREG_CTRL_0_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_VREG_CTRL_0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_CMN_VREG_CTRL_0_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_CMN_VREG_CTRL_0_IN)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_VREG_CTRL_0_VREG_CTRL_0P4_BMSK                                             0xff
#define HWIO_DSI_0_PHY_DSIPHY_CMN_VREG_CTRL_0_VREG_CTRL_0P4_SHFT                                              0x0

#define HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_0_ADDR                                                          (DSI_0_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x00000024)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_0_RMSK                                                                0x7f
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_0_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_0_ADDR, HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_0_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_0_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_0_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_0_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_0_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_0_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_0_IN)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_0_DSIPHY_DIGTOP_PWRDN_B_BMSK                                          0x40
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_0_DSIPHY_DIGTOP_PWRDN_B_SHFT                                           0x6
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_0_CMN_PLL_SHUTDOWNB_BMSK                                              0x20
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_0_CMN_PLL_SHUTDOWNB_SHFT                                               0x5
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_0_DSIPHY_DLN3_SHUTDOWNB_BMSK                                          0x10
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_0_DSIPHY_DLN3_SHUTDOWNB_SHFT                                           0x4
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_0_DSIPHY_DLN2_SHUTDOWNB_BMSK                                           0x8
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_0_DSIPHY_DLN2_SHUTDOWNB_SHFT                                           0x3
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_0_DSIPHY_CLK_SHUTDOWNB_BMSK                                            0x4
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_0_DSIPHY_CLK_SHUTDOWNB_SHFT                                            0x2
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_0_DSIPHY_DLN1_SHUTDOWNB_BMSK                                           0x2
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_0_DSIPHY_DLN1_SHUTDOWNB_SHFT                                           0x1
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_0_DSIPHY_DLN0_SHUTDOWNB_BMSK                                           0x1
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_0_DSIPHY_DLN0_SHUTDOWNB_SHFT                                           0x0

#define HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_1_ADDR                                                          (DSI_0_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x00000028)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_1_RMSK                                                                0xff
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_1_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_1_ADDR, HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_1_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_1_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_1_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_1_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_1_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_1_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_1_IN)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_1_DSIPHY_PLL_CORE_CONTROLREG_RESET_BMSK                               0x80
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_1_DSIPHY_PLL_CORE_CONTROLREG_RESET_SHFT                                0x7
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_1_DSIPHY_SW_RESET_BMSK                                                0x40
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_1_DSIPHY_SW_RESET_SHFT                                                 0x6
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_1_DSIPHY_PLL_SW_RESET_BMSK                                            0x20
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_1_DSIPHY_PLL_SW_RESET_SHFT                                             0x5
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_1_DSIPHY_DLN3_SW_RESET_BMSK                                           0x10
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_1_DSIPHY_DLN3_SW_RESET_SHFT                                            0x4
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_1_DSIPHY_DLN2_SW_RESET_BMSK                                            0x8
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_1_DSIPHY_DLN2_SW_RESET_SHFT                                            0x3
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_1_DSIPHY_CLK_SW_RESET_BMSK                                             0x4
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_1_DSIPHY_CLK_SW_RESET_SHFT                                             0x2
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_1_DSIPHY_DLN1_SW_RESET_BMSK                                            0x2
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_1_DSIPHY_DLN1_SW_RESET_SHFT                                            0x1
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_1_DSIPHY_DLN0_SW_RESET_BMSK                                            0x1
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_1_DSIPHY_DLN0_SW_RESET_SHFT                                            0x0

#define HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_2_ADDR                                                          (DSI_0_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x0000002c)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_2_RMSK                                                                0xff
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_2_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_2_ADDR, HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_2_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_2_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_2_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_2_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_2_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_2_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_2_IN)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_2_DCTRL_BMSK                                                          0xff
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_2_DCTRL_SHFT                                                           0x0

#define HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_3_ADDR                                                          (DSI_0_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x00000030)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_3_RMSK                                                                 0x7
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_3_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_3_ADDR, HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_3_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_3_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_3_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_3_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_3_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_3_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_3_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_3_IN)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_3_SLEEP_B_BMSK                                                         0x4
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_3_SLEEP_B_SHFT                                                         0x2
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_3_NC_INTF_CLK_OUT_CLK_EN_BMSK                                          0x2
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_3_NC_INTF_CLK_OUT_CLK_EN_SHFT                                          0x1
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_3_DSIPHY_RQST_SEL_BMSK                                                 0x1
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_3_DSIPHY_RQST_SEL_SHFT                                                 0x0

#define HWIO_DSI_0_PHY_DSIPHY_CMN_LANE_CFG0_ADDR                                                       (DSI_0_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x00000034)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_LANE_CFG0_RMSK                                                             0xff
#define HWIO_DSI_0_PHY_DSIPHY_CMN_LANE_CFG0_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_LANE_CFG0_ADDR, HWIO_DSI_0_PHY_DSIPHY_CMN_LANE_CFG0_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_LANE_CFG0_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_LANE_CFG0_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_LANE_CFG0_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_CMN_LANE_CFG0_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_LANE_CFG0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_CMN_LANE_CFG0_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_CMN_LANE_CFG0_IN)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_LANE_CFG0_LOGICAL_LANE1_SEL_BMSK                                           0xf0
#define HWIO_DSI_0_PHY_DSIPHY_CMN_LANE_CFG0_LOGICAL_LANE1_SEL_SHFT                                            0x4
#define HWIO_DSI_0_PHY_DSIPHY_CMN_LANE_CFG0_LOGICAL_LANE0_SEL_BMSK                                            0xf
#define HWIO_DSI_0_PHY_DSIPHY_CMN_LANE_CFG0_LOGICAL_LANE0_SEL_SHFT                                            0x0

#define HWIO_DSI_0_PHY_DSIPHY_CMN_LANE_CFG1_ADDR                                                       (DSI_0_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x00000038)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_LANE_CFG1_RMSK                                                             0xff
#define HWIO_DSI_0_PHY_DSIPHY_CMN_LANE_CFG1_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_LANE_CFG1_ADDR, HWIO_DSI_0_PHY_DSIPHY_CMN_LANE_CFG1_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_LANE_CFG1_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_LANE_CFG1_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_LANE_CFG1_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_CMN_LANE_CFG1_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_LANE_CFG1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_CMN_LANE_CFG1_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_CMN_LANE_CFG1_IN)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_LANE_CFG1_LOGICAL_LANE3_SEL_BMSK                                           0xf0
#define HWIO_DSI_0_PHY_DSIPHY_CMN_LANE_CFG1_LOGICAL_LANE3_SEL_SHFT                                            0x4
#define HWIO_DSI_0_PHY_DSIPHY_CMN_LANE_CFG1_LOGICAL_LANE2_SEL_BMSK                                            0xf
#define HWIO_DSI_0_PHY_DSIPHY_CMN_LANE_CFG1_LOGICAL_LANE2_SEL_SHFT                                            0x0

#define HWIO_DSI_0_PHY_DSIPHY_CMN_PLL_CNTRL_ADDR                                                       (DSI_0_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x0000003c)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_PLL_CNTRL_RMSK                                                              0x7
#define HWIO_DSI_0_PHY_DSIPHY_CMN_PLL_CNTRL_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_PLL_CNTRL_ADDR, HWIO_DSI_0_PHY_DSIPHY_CMN_PLL_CNTRL_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_PLL_CNTRL_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_PLL_CNTRL_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_PLL_CNTRL_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_CMN_PLL_CNTRL_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_PLL_CNTRL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_CMN_PLL_CNTRL_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_CMN_PLL_CNTRL_IN)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_PLL_CNTRL_TSYNC_EN_BMSK                                                     0x4
#define HWIO_DSI_0_PHY_DSIPHY_CMN_PLL_CNTRL_TSYNC_EN_SHFT                                                     0x2
#define HWIO_DSI_0_PHY_DSIPHY_CMN_PLL_CNTRL_PLL_BIAS_EN_BMSK                                                  0x2
#define HWIO_DSI_0_PHY_DSIPHY_CMN_PLL_CNTRL_PLL_BIAS_EN_SHFT                                                  0x1
#define HWIO_DSI_0_PHY_DSIPHY_CMN_PLL_CNTRL_PLL_START_BMSK                                                    0x1
#define HWIO_DSI_0_PHY_DSIPHY_CMN_PLL_CNTRL_PLL_START_SHFT                                                    0x0

#define HWIO_DSI_0_PHY_DSIPHY_CMN_DPHY_SOT_ADDR                                                        (DSI_0_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x00000040)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_DPHY_SOT_RMSK                                                              0xff
#define HWIO_DSI_0_PHY_DSIPHY_CMN_DPHY_SOT_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_DPHY_SOT_ADDR, HWIO_DSI_0_PHY_DSIPHY_CMN_DPHY_SOT_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_DPHY_SOT_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_DPHY_SOT_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_DPHY_SOT_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_CMN_DPHY_SOT_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_DPHY_SOT_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_CMN_DPHY_SOT_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_CMN_DPHY_SOT_IN)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_DPHY_SOT_DPHY_SOT_BMSK                                                     0xff
#define HWIO_DSI_0_PHY_DSIPHY_CMN_DPHY_SOT_DPHY_SOT_SHFT                                                      0x0

#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PREAMBLE_FLIP_ADDR                                              (DSI_0_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x00000044)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PREAMBLE_FLIP_RMSK                                                    0x7f
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PREAMBLE_FLIP_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PREAMBLE_FLIP_ADDR, HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PREAMBLE_FLIP_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PREAMBLE_FLIP_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PREAMBLE_FLIP_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PREAMBLE_FLIP_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PREAMBLE_FLIP_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PREAMBLE_FLIP_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PREAMBLE_FLIP_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PREAMBLE_FLIP_IN)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PREAMBLE_FLIP_CPHY_PREAMBLE_FLIP_BMSK                                 0x7f
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PREAMBLE_FLIP_CPHY_PREAMBLE_FLIP_SHFT                                  0x0

#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PREAMBLE_ROTATE_ADDR                                            (DSI_0_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x00000048)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PREAMBLE_ROTATE_RMSK                                                  0x7f
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PREAMBLE_ROTATE_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PREAMBLE_ROTATE_ADDR, HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PREAMBLE_ROTATE_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PREAMBLE_ROTATE_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PREAMBLE_ROTATE_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PREAMBLE_ROTATE_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PREAMBLE_ROTATE_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PREAMBLE_ROTATE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PREAMBLE_ROTATE_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PREAMBLE_ROTATE_IN)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PREAMBLE_ROTATE_CPHY_PREAMBLE_ROTATE_BMSK                             0x7f
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PREAMBLE_ROTATE_CPHY_PREAMBLE_ROTATE_SHFT                              0x0

#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PREAMBLE_POLARITY_ADDR                                          (DSI_0_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x0000004c)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PREAMBLE_POLARITY_RMSK                                                0x7f
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PREAMBLE_POLARITY_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PREAMBLE_POLARITY_ADDR, HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PREAMBLE_POLARITY_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PREAMBLE_POLARITY_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PREAMBLE_POLARITY_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PREAMBLE_POLARITY_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PREAMBLE_POLARITY_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PREAMBLE_POLARITY_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PREAMBLE_POLARITY_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PREAMBLE_POLARITY_IN)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PREAMBLE_POLARITY_CPHY_PREAMBLE_POLARITY_BMSK                         0x7f
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PREAMBLE_POLARITY_CPHY_PREAMBLE_POLARITY_SHFT                          0x0

#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PROGSEQ_FLIP0_ADDR                                              (DSI_0_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x00000050)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PROGSEQ_FLIP0_RMSK                                                    0xff
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PROGSEQ_FLIP0_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PROGSEQ_FLIP0_ADDR, HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PROGSEQ_FLIP0_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PROGSEQ_FLIP0_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PROGSEQ_FLIP0_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PROGSEQ_FLIP0_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PROGSEQ_FLIP0_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PROGSEQ_FLIP0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PROGSEQ_FLIP0_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PROGSEQ_FLIP0_IN)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PROGSEQ_FLIP0_CPHY_PROGSEQ_FLIP_7_0_BMSK                              0xff
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PROGSEQ_FLIP0_CPHY_PROGSEQ_FLIP_7_0_SHFT                               0x0

#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PROGSEQ_ROTATE0_ADDR                                            (DSI_0_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x00000054)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PROGSEQ_ROTATE0_RMSK                                                  0xff
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PROGSEQ_ROTATE0_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PROGSEQ_ROTATE0_ADDR, HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PROGSEQ_ROTATE0_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PROGSEQ_ROTATE0_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PROGSEQ_ROTATE0_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PROGSEQ_ROTATE0_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PROGSEQ_ROTATE0_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PROGSEQ_ROTATE0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PROGSEQ_ROTATE0_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PROGSEQ_ROTATE0_IN)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PROGSEQ_ROTATE0_CPHY_PROGSEQ_ROTATE_7_0_BMSK                          0xff
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PROGSEQ_ROTATE0_CPHY_PROGSEQ_ROTATE_7_0_SHFT                           0x0

#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PROGSEQ_POLARITY0_ADDR                                          (DSI_0_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x00000058)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PROGSEQ_POLARITY0_RMSK                                                0xff
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PROGSEQ_POLARITY0_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PROGSEQ_POLARITY0_ADDR, HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PROGSEQ_POLARITY0_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PROGSEQ_POLARITY0_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PROGSEQ_POLARITY0_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PROGSEQ_POLARITY0_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PROGSEQ_POLARITY0_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PROGSEQ_POLARITY0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PROGSEQ_POLARITY0_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PROGSEQ_POLARITY0_IN)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PROGSEQ_POLARITY0_CPHY_PROGSEQ_POLARITY_7_0_BMSK                      0xff
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PROGSEQ_POLARITY0_CPHY_PROGSEQ_POLARITY_7_0_SHFT                       0x0

#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PROGSEQ_FLIP1_ADDR                                              (DSI_0_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x0000005c)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PROGSEQ_FLIP1_RMSK                                                    0x3f
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PROGSEQ_FLIP1_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PROGSEQ_FLIP1_ADDR, HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PROGSEQ_FLIP1_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PROGSEQ_FLIP1_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PROGSEQ_FLIP1_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PROGSEQ_FLIP1_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PROGSEQ_FLIP1_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PROGSEQ_FLIP1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PROGSEQ_FLIP1_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PROGSEQ_FLIP1_IN)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PROGSEQ_FLIP1_CPHY_PROGSEQ_FLIP_13_8_BMSK                             0x3f
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PROGSEQ_FLIP1_CPHY_PROGSEQ_FLIP_13_8_SHFT                              0x0

#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PROGSEQ_ROTATE1_ADDR                                            (DSI_0_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x00000060)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PROGSEQ_ROTATE1_RMSK                                                  0x3f
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PROGSEQ_ROTATE1_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PROGSEQ_ROTATE1_ADDR, HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PROGSEQ_ROTATE1_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PROGSEQ_ROTATE1_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PROGSEQ_ROTATE1_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PROGSEQ_ROTATE1_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PROGSEQ_ROTATE1_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PROGSEQ_ROTATE1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PROGSEQ_ROTATE1_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PROGSEQ_ROTATE1_IN)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PROGSEQ_ROTATE1_CPHY_PROGSEQ_ROTATE_13_8_BMSK                         0x3f
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PROGSEQ_ROTATE1_CPHY_PROGSEQ_ROTATE_13_8_SHFT                          0x0

#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PROGSEQ_POLARITY1_ADDR                                          (DSI_0_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x00000064)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PROGSEQ_POLARITY1_RMSK                                                0x3f
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PROGSEQ_POLARITY1_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PROGSEQ_POLARITY1_ADDR, HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PROGSEQ_POLARITY1_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PROGSEQ_POLARITY1_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PROGSEQ_POLARITY1_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PROGSEQ_POLARITY1_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PROGSEQ_POLARITY1_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PROGSEQ_POLARITY1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PROGSEQ_POLARITY1_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PROGSEQ_POLARITY1_IN)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PROGSEQ_POLARITY1_CPHY_PROGSEQ_POLARITY_13_8_BMSK                     0x3f
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_PROGSEQ_POLARITY1_CPHY_PROGSEQ_POLARITY_13_8_SHFT                      0x0

#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_SYNC_FLIP_ADDR                                                  (DSI_0_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x00000068)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_SYNC_FLIP_RMSK                                                        0x7f
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_SYNC_FLIP_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_SYNC_FLIP_ADDR, HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_SYNC_FLIP_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_SYNC_FLIP_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_SYNC_FLIP_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_SYNC_FLIP_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_SYNC_FLIP_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_SYNC_FLIP_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_SYNC_FLIP_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_SYNC_FLIP_IN)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_SYNC_FLIP_CPHY_SYNC_FLIP_BMSK                                         0x7f
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_SYNC_FLIP_CPHY_SYNC_FLIP_SHFT                                          0x0

#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_SYNC_ROTATE_ADDR                                                (DSI_0_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x0000006c)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_SYNC_ROTATE_RMSK                                                      0x7f
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_SYNC_ROTATE_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_SYNC_ROTATE_ADDR, HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_SYNC_ROTATE_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_SYNC_ROTATE_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_SYNC_ROTATE_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_SYNC_ROTATE_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_SYNC_ROTATE_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_SYNC_ROTATE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_SYNC_ROTATE_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_SYNC_ROTATE_IN)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_SYNC_ROTATE_CPHY_SYNC_ROTATE_BMSK                                     0x7f
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_SYNC_ROTATE_CPHY_SYNC_ROTATE_SHFT                                      0x0

#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_SYNC_POLARITY_ADDR                                              (DSI_0_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x00000070)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_SYNC_POLARITY_RMSK                                                    0x7f
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_SYNC_POLARITY_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_SYNC_POLARITY_ADDR, HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_SYNC_POLARITY_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_SYNC_POLARITY_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_SYNC_POLARITY_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_SYNC_POLARITY_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_SYNC_POLARITY_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_SYNC_POLARITY_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_SYNC_POLARITY_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_SYNC_POLARITY_IN)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_SYNC_POLARITY_CPHY_SYNC_POLARITY_BMSK                                 0x7f
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_SYNC_POLARITY_CPHY_SYNC_POLARITY_SHFT                                  0x0

#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_POST_FLIP_ADDR                                                  (DSI_0_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x00000074)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_POST_FLIP_RMSK                                                        0x7f
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_POST_FLIP_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_POST_FLIP_ADDR, HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_POST_FLIP_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_POST_FLIP_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_POST_FLIP_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_POST_FLIP_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_POST_FLIP_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_POST_FLIP_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_POST_FLIP_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_POST_FLIP_IN)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_POST_FLIP_CPHY_POST_FLIP_BMSK                                         0x7f
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_POST_FLIP_CPHY_POST_FLIP_SHFT                                          0x0

#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_POST_ROTATE_ADDR                                                (DSI_0_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x00000078)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_POST_ROTATE_RMSK                                                      0x7f
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_POST_ROTATE_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_POST_ROTATE_ADDR, HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_POST_ROTATE_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_POST_ROTATE_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_POST_ROTATE_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_POST_ROTATE_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_POST_ROTATE_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_POST_ROTATE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_POST_ROTATE_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_POST_ROTATE_IN)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_POST_ROTATE_CPHY_POST_ROTATE_BMSK                                     0x7f
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_POST_ROTATE_CPHY_POST_ROTATE_SHFT                                      0x0

#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_POST_POLARITY_ADDR                                              (DSI_0_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x0000007c)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_POST_POLARITY_RMSK                                                    0x7f
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_POST_POLARITY_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_POST_POLARITY_ADDR, HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_POST_POLARITY_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_POST_POLARITY_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_POST_POLARITY_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_POST_POLARITY_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_POST_POLARITY_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_POST_POLARITY_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_POST_POLARITY_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_POST_POLARITY_IN)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_POST_POLARITY_CPHY_POST_POLARITY_BMSK                                 0x7f
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_POST_POLARITY_CPHY_POST_POLARITY_SHFT                                  0x0

#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_LANE_CTRL0_ADDR                                                 (DSI_0_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x00000080)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_LANE_CTRL0_RMSK                                                        0x7
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_LANE_CTRL0_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_LANE_CTRL0_ADDR, HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_LANE_CTRL0_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_LANE_CTRL0_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_LANE_CTRL0_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_LANE_CTRL0_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_LANE_CTRL0_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_LANE_CTRL0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_LANE_CTRL0_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_LANE_CTRL0_IN)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_LANE_CTRL0_CPHY_DLN2_CONTINUOUS_CLK_MODE_BMSK                          0x4
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_LANE_CTRL0_CPHY_DLN2_CONTINUOUS_CLK_MODE_SHFT                          0x2
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_LANE_CTRL0_CPHY_DLN1_CONTINUOUS_CLK_MODE_BMSK                          0x2
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_LANE_CTRL0_CPHY_DLN1_CONTINUOUS_CLK_MODE_SHFT                          0x1
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_LANE_CTRL0_CPHY_DLN0_CONTINUOUS_CLK_MODE_BMSK                          0x1
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CPHY_LANE_CTRL0_CPHY_DLN0_CONTINUOUS_CLK_MODE_SHFT                          0x0

#define HWIO_DSI_0_PHY_DSIPHY_CMN_BIST_CTRL_ADDR                                                       (DSI_0_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x00000084)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_BIST_CTRL_RMSK                                                             0xff
#define HWIO_DSI_0_PHY_DSIPHY_CMN_BIST_CTRL_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_BIST_CTRL_ADDR, HWIO_DSI_0_PHY_DSIPHY_CMN_BIST_CTRL_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_BIST_CTRL_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_BIST_CTRL_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_BIST_CTRL_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_CMN_BIST_CTRL_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_BIST_CTRL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_CMN_BIST_CTRL_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_CMN_BIST_CTRL_IN)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_BIST_CTRL_FORCE_CNCK_TX_REQUEST_HS_BMSK                                    0x80
#define HWIO_DSI_0_PHY_DSIPHY_CMN_BIST_CTRL_FORCE_CNCK_TX_REQUEST_HS_SHFT                                     0x7
#define HWIO_DSI_0_PHY_DSIPHY_CMN_BIST_CTRL_LP_BIST_PRBS_POLY_BMSK                                           0x60
#define HWIO_DSI_0_PHY_DSIPHY_CMN_BIST_CTRL_LP_BIST_PRBS_POLY_SHFT                                            0x5
#define HWIO_DSI_0_PHY_DSIPHY_CMN_BIST_CTRL_LP_BISTGEN_ENABLE_BMSK                                           0x10
#define HWIO_DSI_0_PHY_DSIPHY_CMN_BIST_CTRL_LP_BISTGEN_ENABLE_SHFT                                            0x4
#define HWIO_DSI_0_PHY_DSIPHY_CMN_BIST_CTRL_BIST_MODE_BMSK                                                    0x8
#define HWIO_DSI_0_PHY_DSIPHY_CMN_BIST_CTRL_BIST_MODE_SHFT                                                    0x3
#define HWIO_DSI_0_PHY_DSIPHY_CMN_BIST_CTRL_BISTGEN_ERROR_INJECT_BMSK                                         0x4
#define HWIO_DSI_0_PHY_DSIPHY_CMN_BIST_CTRL_BISTGEN_ERROR_INJECT_SHFT                                         0x2
#define HWIO_DSI_0_PHY_DSIPHY_CMN_BIST_CTRL_HS_BISTGEN_ENABLE_BMSK                                            0x2
#define HWIO_DSI_0_PHY_DSIPHY_CMN_BIST_CTRL_HS_BISTGEN_ENABLE_SHFT                                            0x1
#define HWIO_DSI_0_PHY_DSIPHY_CMN_BIST_CTRL_HS_BISTGEN_CROSS_BMSK                                             0x1
#define HWIO_DSI_0_PHY_DSIPHY_CMN_BIST_CTRL_HS_BISTGEN_CROSS_SHFT                                             0x0

#define HWIO_DSI_0_PHY_DSIPHY_CMN_BIST_SEED0_ADDR                                                      (DSI_0_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x00000088)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_BIST_SEED0_RMSK                                                            0xff
#define HWIO_DSI_0_PHY_DSIPHY_CMN_BIST_SEED0_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_BIST_SEED0_ADDR, HWIO_DSI_0_PHY_DSIPHY_CMN_BIST_SEED0_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_BIST_SEED0_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_BIST_SEED0_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_BIST_SEED0_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_CMN_BIST_SEED0_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_BIST_SEED0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_CMN_BIST_SEED0_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_CMN_BIST_SEED0_IN)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_BIST_SEED0_BIST_PRBS_SEED_7_0_BMSK                                         0xff
#define HWIO_DSI_0_PHY_DSIPHY_CMN_BIST_SEED0_BIST_PRBS_SEED_7_0_SHFT                                          0x0

#define HWIO_DSI_0_PHY_DSIPHY_CMN_BIST_SEED1_ADDR                                                      (DSI_0_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x0000008c)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_BIST_SEED1_RMSK                                                            0xff
#define HWIO_DSI_0_PHY_DSIPHY_CMN_BIST_SEED1_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_BIST_SEED1_ADDR, HWIO_DSI_0_PHY_DSIPHY_CMN_BIST_SEED1_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_BIST_SEED1_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_BIST_SEED1_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_BIST_SEED1_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_CMN_BIST_SEED1_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_BIST_SEED1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_CMN_BIST_SEED1_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_CMN_BIST_SEED1_IN)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_BIST_SEED1_BIST_PRBS_SEED_15_8_BMSK                                        0xff
#define HWIO_DSI_0_PHY_DSIPHY_CMN_BIST_SEED1_BIST_PRBS_SEED_15_8_SHFT                                         0x0

#define HWIO_DSI_0_PHY_DSIPHY_CMN_BIST_SEED2_ADDR                                                      (DSI_0_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x00000090)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_BIST_SEED2_RMSK                                                            0xff
#define HWIO_DSI_0_PHY_DSIPHY_CMN_BIST_SEED2_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_BIST_SEED2_ADDR, HWIO_DSI_0_PHY_DSIPHY_CMN_BIST_SEED2_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_BIST_SEED2_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_BIST_SEED2_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_BIST_SEED2_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_CMN_BIST_SEED2_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_BIST_SEED2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_CMN_BIST_SEED2_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_CMN_BIST_SEED2_IN)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_BIST_SEED2_BIST_PRBS_SEED_23_16_BMSK                                       0xff
#define HWIO_DSI_0_PHY_DSIPHY_CMN_BIST_SEED2_BIST_PRBS_SEED_23_16_SHFT                                        0x0

#define HWIO_DSI_0_PHY_DSIPHY_CMN_BIST_POLY_ADDR                                                       (DSI_0_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x00000094)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_BIST_POLY_RMSK                                                             0xff
#define HWIO_DSI_0_PHY_DSIPHY_CMN_BIST_POLY_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_BIST_POLY_ADDR, HWIO_DSI_0_PHY_DSIPHY_CMN_BIST_POLY_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_BIST_POLY_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_BIST_POLY_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_BIST_POLY_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_CMN_BIST_POLY_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_BIST_POLY_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_CMN_BIST_POLY_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_CMN_BIST_POLY_IN)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_BIST_POLY_BIST_PRBS_POLY_BMSK                                              0xff
#define HWIO_DSI_0_PHY_DSIPHY_CMN_BIST_POLY_BIST_PRBS_POLY_SHFT                                               0x0

#define HWIO_DSI_0_PHY_DSIPHY_CMN_MISR_CTRL_ADDR                                                       (DSI_0_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x00000098)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_MISR_CTRL_RMSK                                                             0x3f
#define HWIO_DSI_0_PHY_DSIPHY_CMN_MISR_CTRL_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_MISR_CTRL_ADDR, HWIO_DSI_0_PHY_DSIPHY_CMN_MISR_CTRL_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_MISR_CTRL_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_MISR_CTRL_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_MISR_CTRL_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_CMN_MISR_CTRL_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_MISR_CTRL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_CMN_MISR_CTRL_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_CMN_MISR_CTRL_IN)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_MISR_CTRL_DIGTOP_TEST_CLK_DEBUG_SEL_BMSK                                   0x3c
#define HWIO_DSI_0_PHY_DSIPHY_CMN_MISR_CTRL_DIGTOP_TEST_CLK_DEBUG_SEL_SHFT                                    0x2
#define HWIO_DSI_0_PHY_DSIPHY_CMN_MISR_CTRL_MISR_CLEAR_BMSK                                                   0x2
#define HWIO_DSI_0_PHY_DSIPHY_CMN_MISR_CTRL_MISR_CLEAR_SHFT                                                   0x1
#define HWIO_DSI_0_PHY_DSIPHY_CMN_MISR_CTRL_MISR_EN_BMSK                                                      0x1
#define HWIO_DSI_0_PHY_DSIPHY_CMN_MISR_CTRL_MISR_EN_SHFT                                                      0x0

#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_DEBUG_SEL_ADDR                                           (DSI_0_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x0000009c)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_DEBUG_SEL_RMSK                                                 0xff
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_DEBUG_SEL_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_DEBUG_SEL_ADDR, HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_DEBUG_SEL_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_DEBUG_SEL_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_DEBUG_SEL_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_DEBUG_SEL_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_DEBUG_SEL_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_DEBUG_SEL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_DEBUG_SEL_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_DEBUG_SEL_IN)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_DEBUG_SEL_DIGTOP_AMUX_SEL_BMSK                                 0xc0
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_DEBUG_SEL_DIGTOP_AMUX_SEL_SHFT                                  0x6
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_DEBUG_SEL_DIGTOP_DEBUG_SEL_BMSK                                0x3f
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_DEBUG_SEL_DIGTOP_DEBUG_SEL_SHFT                                 0x0

#define HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL0_ADDR                                                  (DSI_0_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x000000a0)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL0_RMSK                                                        0x1f
#define HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL0_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL0_ADDR, HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL0_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL0_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL0_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL0_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL0_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL0_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL0_IN)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL0_CLKLN_EN_BMSK                                               0x10
#define HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL0_CLKLN_EN_SHFT                                                0x4
#define HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL0_DLN3_EN_BMSK                                                 0x8
#define HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL0_DLN3_EN_SHFT                                                 0x3
#define HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL0_DLN2_EN_BMSK                                                 0x4
#define HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL0_DLN2_EN_SHFT                                                 0x2
#define HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL0_DLN1_EN_BMSK                                                 0x2
#define HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL0_DLN1_EN_SHFT                                                 0x1
#define HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL0_DLN0_EN_BMSK                                                 0x1
#define HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL0_DLN0_EN_SHFT                                                 0x0

#define HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL1_ADDR                                                  (DSI_0_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x000000a4)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL1_RMSK                                                        0x7f
#define HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL1_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL1_ADDR, HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL1_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL1_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL1_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL1_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL1_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL1_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL1_IN)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL1_DPHY_CONTNSCLKMODE_CNCK_TX_RQST_HS_BMSK                     0x40
#define HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL1_DPHY_CONTNSCLKMODE_CNCK_TX_RQST_HS_SHFT                      0x6
#define HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL1_DPHY_CONTINUOUS_CLK_MODE_BMSK                               0x20
#define HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL1_DPHY_CONTINUOUS_CLK_MODE_SHFT                                0x5
#define HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL1_CLKLN_ULPS_REQUEST_BMSK                                     0x10
#define HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL1_CLKLN_ULPS_REQUEST_SHFT                                      0x4
#define HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL1_DLN3_ULPS_REQUEST_BMSK                                       0x8
#define HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL1_DLN3_ULPS_REQUEST_SHFT                                       0x3
#define HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL1_DLN2_ULPS_REQUEST_BMSK                                       0x4
#define HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL1_DLN2_ULPS_REQUEST_SHFT                                       0x2
#define HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL1_DLN1_ULPS_REQUEST_BMSK                                       0x2
#define HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL1_DLN1_ULPS_REQUEST_SHFT                                       0x1
#define HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL1_DLN0_ULPS_REQUEST_BMSK                                       0x1
#define HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL1_DLN0_ULPS_REQUEST_SHFT                                       0x0

#define HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL2_ADDR                                                  (DSI_0_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x000000a8)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL2_RMSK                                                        0x1f
#define HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL2_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL2_ADDR, HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL2_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL2_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL2_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL2_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL2_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL2_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL2_IN)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL2_CLKLN_ULPS_EXIT_BMSK                                        0x10
#define HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL2_CLKLN_ULPS_EXIT_SHFT                                         0x4
#define HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL2_DLN3_ULPS_EXIT_BMSK                                          0x8
#define HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL2_DLN3_ULPS_EXIT_SHFT                                          0x3
#define HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL2_DLN2_ULPS_EXIT_BMSK                                          0x4
#define HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL2_DLN2_ULPS_EXIT_SHFT                                          0x2
#define HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL2_DLN1_ULPS_EXIT_BMSK                                          0x2
#define HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL2_DLN1_ULPS_EXIT_SHFT                                          0x1
#define HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL2_DLN0_ULPS_EXIT_BMSK                                          0x1
#define HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL2_DLN0_ULPS_EXIT_SHFT                                          0x0

#define HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL3_ADDR                                                  (DSI_0_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x000000ac)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL3_RMSK                                                        0x1f
#define HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL3_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL3_ADDR, HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL3_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL3_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL3_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL3_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL3_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL3_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL3_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL3_IN)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL3_CLKLN_FORCE_TX_STOP_BMSK                                    0x10
#define HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL3_CLKLN_FORCE_TX_STOP_SHFT                                     0x4
#define HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL3_DLN3_FORCE_TX_STOP_BMSK                                      0x8
#define HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL3_DLN3_FORCE_TX_STOP_SHFT                                      0x3
#define HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL3_DLN2_FORCE_TX_STOP_BMSK                                      0x4
#define HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL3_DLN2_FORCE_TX_STOP_SHFT                                      0x2
#define HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL3_DLN1_FORCE_TX_STOP_BMSK                                      0x2
#define HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL3_DLN1_FORCE_TX_STOP_SHFT                                      0x1
#define HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL3_DLN0_FORCE_TX_STOP_BMSK                                      0x1
#define HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL3_DLN0_FORCE_TX_STOP_SHFT                                      0x0

#define HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL4_ADDR                                                  (DSI_0_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x000000b0)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL4_RMSK                                                        0xff
#define HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL4_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL4_ADDR, HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL4_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL4_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL4_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL4_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL4_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL4_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL4_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL4_IN)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL4_DIGTOP_DEBUG_CLK_DEBUG_SEL_BMSK                             0xf0
#define HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL4_DIGTOP_DEBUG_CLK_DEBUG_SEL_SHFT                              0x4
#define HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL4_TRIGGER_BMSK                                                 0xf
#define HWIO_DSI_0_PHY_DSIPHY_CMN_DSI_LANE_CTRL4_TRIGGER_SHFT                                                 0x0

#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_0_ADDR                                                   (DSI_0_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x000000b4)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_0_RMSK                                                          0x3
#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_0_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_0_ADDR, HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_0_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_0_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_0_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_0_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_0_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_0_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_0_IN)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_0_HALFBYTECLK_CLKLN_EN_BMSK                                     0x2
#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_0_HALFBYTECLK_CLKLN_EN_SHFT                                     0x1
#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_0_HALFBYTECLK_DATALN_EN_BMSK                                    0x1
#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_0_HALFBYTECLK_DATALN_EN_SHFT                                    0x0

#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_1_ADDR                                                   (DSI_0_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x000000b8)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_1_RMSK                                                         0xff
#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_1_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_1_ADDR, HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_1_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_1_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_1_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_1_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_1_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_1_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_1_IN)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_1_DSIPHY_T_CLK_ZERO_BMSK                                       0xff
#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_1_DSIPHY_T_CLK_ZERO_SHFT                                        0x0

#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_2_ADDR                                                   (DSI_0_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x000000bc)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_2_RMSK                                                         0xff
#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_2_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_2_ADDR, HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_2_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_2_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_2_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_2_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_2_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_2_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_2_IN)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_2_DSIPHY_T_CLK_PREPARE_BMSK                                    0xff
#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_2_DSIPHY_T_CLK_PREPARE_SHFT                                     0x0

#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_3_ADDR                                                   (DSI_0_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x000000c0)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_3_RMSK                                                         0xff
#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_3_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_3_ADDR, HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_3_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_3_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_3_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_3_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_3_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_3_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_3_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_3_IN)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_3_DSIPHY_T_CLK_TRAIL_BMSK                                      0xff
#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_3_DSIPHY_T_CLK_TRAIL_SHFT                                       0x0

#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_4_ADDR                                                   (DSI_0_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x000000c4)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_4_RMSK                                                         0xff
#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_4_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_4_ADDR, HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_4_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_4_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_4_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_4_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_4_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_4_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_4_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_4_IN)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_4_DSIPHY_T_HS_EXIT_BMSK                                        0xff
#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_4_DSIPHY_T_HS_EXIT_SHFT                                         0x0

#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_5_ADDR                                                   (DSI_0_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x000000c8)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_5_RMSK                                                         0xff
#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_5_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_5_ADDR, HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_5_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_5_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_5_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_5_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_5_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_5_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_5_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_5_IN)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_5_DSIPHY_T_HS_ZERO_BMSK                                        0xff
#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_5_DSIPHY_T_HS_ZERO_SHFT                                         0x0

#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_6_ADDR                                                   (DSI_0_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x000000cc)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_6_RMSK                                                         0xff
#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_6_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_6_ADDR, HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_6_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_6_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_6_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_6_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_6_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_6_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_6_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_6_IN)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_6_DSIPHY_T_HS_PREPARE_BMSK                                     0xff
#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_6_DSIPHY_T_HS_PREPARE_SHFT                                      0x0

#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_7_ADDR                                                   (DSI_0_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x000000d0)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_7_RMSK                                                         0xff
#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_7_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_7_ADDR, HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_7_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_7_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_7_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_7_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_7_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_7_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_7_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_7_IN)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_7_DSIPHY_T_HS_TRAIL_BMSK                                       0xff
#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_7_DSIPHY_T_HS_TRAIL_SHFT                                        0x0

#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_8_ADDR                                                   (DSI_0_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x000000d4)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_8_RMSK                                                         0xff
#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_8_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_8_ADDR, HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_8_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_8_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_8_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_8_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_8_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_8_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_8_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_8_IN)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_8_DSIPHY_T_HS_RQST_BMSK                                        0xff
#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_8_DSIPHY_T_HS_RQST_SHFT                                         0x0

#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_9_ADDR                                                   (DSI_0_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x000000d8)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_9_RMSK                                                         0x3f
#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_9_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_9_ADDR, HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_9_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_9_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_9_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_9_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_9_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_9_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_9_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_9_IN)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_9_DSIPHY_T_TA_SURE_BMSK                                        0x38
#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_9_DSIPHY_T_TA_SURE_SHFT                                         0x3
#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_9_DSIPHY_T_TA_GO_BMSK                                           0x7
#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_9_DSIPHY_T_TA_GO_SHFT                                           0x0

#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_10_ADDR                                                  (DSI_0_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x000000dc)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_10_RMSK                                                         0x7
#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_10_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_10_ADDR, HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_10_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_10_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_10_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_10_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_10_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_10_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_10_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_10_IN)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_10_DSIPHY_T_TA_GET_BMSK                                         0x7
#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_10_DSIPHY_T_TA_GET_SHFT                                         0x0

#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_11_ADDR                                                  (DSI_0_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x000000e0)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_11_RMSK                                                        0xff
#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_11_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_11_ADDR, HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_11_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_11_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_11_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_11_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_11_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_11_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_11_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_11_IN)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_11_DSIPHY_TRIG3_CMD_BMSK                                       0xff
#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_11_DSIPHY_TRIG3_CMD_SHFT                                        0x0

#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_12_ADDR                                                  (DSI_0_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x000000e4)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_12_RMSK                                                        0xff
#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_12_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_12_ADDR, HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_12_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_12_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_12_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_12_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_12_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_12_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_12_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_12_IN)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_12_DSIPHY_T_CLK_PRE_BMSK                                       0xff
#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_12_DSIPHY_T_CLK_PRE_SHFT                                        0x0

#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_13_ADDR                                                  (DSI_0_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x000000e8)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_13_RMSK                                                        0xff
#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_13_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_13_ADDR, HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_13_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_13_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_13_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_13_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_13_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_13_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_13_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_13_IN)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_13_DSIPHY_T_CLK_POST_BMSK                                      0xff
#define HWIO_DSI_0_PHY_DSIPHY_CMN_TIMING_CTRL_13_DSIPHY_T_CLK_POST_SHFT                                       0x0

#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_HSTX_STR_CTRL_0_ADDR                                            (DSI_0_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x000000ec)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_HSTX_STR_CTRL_0_RMSK                                                  0xff
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_HSTX_STR_CTRL_0_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_HSTX_STR_CTRL_0_ADDR, HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_HSTX_STR_CTRL_0_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_HSTX_STR_CTRL_0_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_HSTX_STR_CTRL_0_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_HSTX_STR_CTRL_0_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_HSTX_STR_CTRL_0_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_HSTX_STR_CTRL_0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_HSTX_STR_CTRL_0_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_HSTX_STR_CTRL_0_IN)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_HSTX_STR_CTRL_0_DSIPHY_HSTX_STR_HSTOP_BMSK                            0xf0
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_HSTX_STR_CTRL_0_DSIPHY_HSTX_STR_HSTOP_SHFT                             0x4
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_HSTX_STR_CTRL_0_DSIPHY_HSTX_STR_HSBOT_BMSK                             0xf
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_HSTX_STR_CTRL_0_DSIPHY_HSTX_STR_HSBOT_SHFT                             0x0

#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_HSTX_STR_CTRL_1_ADDR                                            (DSI_0_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x000000f0)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_HSTX_STR_CTRL_1_RMSK                                                   0xf
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_HSTX_STR_CTRL_1_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_HSTX_STR_CTRL_1_ADDR, HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_HSTX_STR_CTRL_1_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_HSTX_STR_CTRL_1_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_HSTX_STR_CTRL_1_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_HSTX_STR_CTRL_1_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_HSTX_STR_CTRL_1_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_HSTX_STR_CTRL_1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_HSTX_STR_CTRL_1_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_HSTX_STR_CTRL_1_IN)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_HSTX_STR_CTRL_1_DSIPHY_HSTX_STR_HSMID_BMSK                             0xf
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_HSTX_STR_CTRL_1_DSIPHY_HSTX_STR_HSMID_SHFT                             0x0

#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_RESCODE_OFFSET_TOP_CTRL_ADDR                                    (DSI_0_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x000000f4)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_RESCODE_OFFSET_TOP_CTRL_RMSK                                          0x3f
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_RESCODE_OFFSET_TOP_CTRL_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_RESCODE_OFFSET_TOP_CTRL_ADDR, HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_RESCODE_OFFSET_TOP_CTRL_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_RESCODE_OFFSET_TOP_CTRL_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_RESCODE_OFFSET_TOP_CTRL_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_RESCODE_OFFSET_TOP_CTRL_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_RESCODE_OFFSET_TOP_CTRL_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_RESCODE_OFFSET_TOP_CTRL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_RESCODE_OFFSET_TOP_CTRL_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_RESCODE_OFFSET_TOP_CTRL_IN)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_RESCODE_OFFSET_TOP_CTRL_DSIPHY_RESCODE_OFFSET_TOP_BMSK                0x3f
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_RESCODE_OFFSET_TOP_CTRL_DSIPHY_RESCODE_OFFSET_TOP_SHFT                 0x0

#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_RESCODE_OFFSET_BOT_CTRL_ADDR                                    (DSI_0_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x000000f8)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_RESCODE_OFFSET_BOT_CTRL_RMSK                                          0x3f
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_RESCODE_OFFSET_BOT_CTRL_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_RESCODE_OFFSET_BOT_CTRL_ADDR, HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_RESCODE_OFFSET_BOT_CTRL_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_RESCODE_OFFSET_BOT_CTRL_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_RESCODE_OFFSET_BOT_CTRL_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_RESCODE_OFFSET_BOT_CTRL_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_RESCODE_OFFSET_BOT_CTRL_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_RESCODE_OFFSET_BOT_CTRL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_RESCODE_OFFSET_BOT_CTRL_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_RESCODE_OFFSET_BOT_CTRL_IN)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_RESCODE_OFFSET_BOT_CTRL_DSIPHY_RESCODE_OFFSET_BOT_BMSK                0x3f
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_RESCODE_OFFSET_BOT_CTRL_DSIPHY_RESCODE_OFFSET_BOT_SHFT                 0x0

#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_RESCODE_OFFSET_MID_CTRL_ADDR                                    (DSI_0_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x000000fc)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_RESCODE_OFFSET_MID_CTRL_RMSK                                          0x3f
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_RESCODE_OFFSET_MID_CTRL_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_RESCODE_OFFSET_MID_CTRL_ADDR, HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_RESCODE_OFFSET_MID_CTRL_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_RESCODE_OFFSET_MID_CTRL_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_RESCODE_OFFSET_MID_CTRL_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_RESCODE_OFFSET_MID_CTRL_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_RESCODE_OFFSET_MID_CTRL_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_RESCODE_OFFSET_MID_CTRL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_RESCODE_OFFSET_MID_CTRL_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_RESCODE_OFFSET_MID_CTRL_IN)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_RESCODE_OFFSET_MID_CTRL_DSIPHY_RESCODE_OFFSET_MID_BMSK                0x3f
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_RESCODE_OFFSET_MID_CTRL_DSIPHY_RESCODE_OFFSET_MID_SHFT                 0x0

#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_LPTX_STR_CTRL_ADDR                                              (DSI_0_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x00000100)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_LPTX_STR_CTRL_RMSK                                                    0xff
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_LPTX_STR_CTRL_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_LPTX_STR_CTRL_ADDR, HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_LPTX_STR_CTRL_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_LPTX_STR_CTRL_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_LPTX_STR_CTRL_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_LPTX_STR_CTRL_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_LPTX_STR_CTRL_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_LPTX_STR_CTRL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_LPTX_STR_CTRL_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_LPTX_STR_CTRL_IN)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_LPTX_STR_CTRL_DSIPHY_STR_LP_N_BMSK                                    0xf0
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_LPTX_STR_CTRL_DSIPHY_STR_LP_N_SHFT                                     0x4
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_LPTX_STR_CTRL_DSIPHY_STR_LP_P_BMSK                                     0xf
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_LPTX_STR_CTRL_DSIPHY_STR_LP_P_SHFT                                     0x0

#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_PEMPH_CTRL_0_ADDR                                               (DSI_0_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x00000104)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_PEMPH_CTRL_0_RMSK                                                     0xff
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_PEMPH_CTRL_0_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_PEMPH_CTRL_0_ADDR, HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_PEMPH_CTRL_0_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_PEMPH_CTRL_0_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_PEMPH_CTRL_0_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_PEMPH_CTRL_0_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_PEMPH_CTRL_0_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_PEMPH_CTRL_0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_PEMPH_CTRL_0_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_PEMPH_CTRL_0_IN)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_PEMPH_CTRL_0_DSIPHY_PEMPH_STRBOT_BMSK                                 0xf0
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_PEMPH_CTRL_0_DSIPHY_PEMPH_STRBOT_SHFT                                  0x4
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_PEMPH_CTRL_0_DSIPHY_PEMPH_STRTOP_BMSK                                  0xf
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_PEMPH_CTRL_0_DSIPHY_PEMPH_STRTOP_SHFT                                  0x0

#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_PEMPH_CTRL_1_ADDR                                               (DSI_0_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x00000108)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_PEMPH_CTRL_1_RMSK                                                      0xf
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_PEMPH_CTRL_1_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_PEMPH_CTRL_1_ADDR, HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_PEMPH_CTRL_1_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_PEMPH_CTRL_1_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_PEMPH_CTRL_1_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_PEMPH_CTRL_1_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_PEMPH_CTRL_1_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_PEMPH_CTRL_1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_PEMPH_CTRL_1_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_PEMPH_CTRL_1_IN)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_PEMPH_CTRL_1_DSIPHY_PEMPH_STRMID_BMSK                                  0xf
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_PEMPH_CTRL_1_DSIPHY_PEMPH_STRMID_SHFT                                  0x0

#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_STR_SWI_CAL_SEL_CTRL_ADDR                                       (DSI_0_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x0000010c)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_STR_SWI_CAL_SEL_CTRL_RMSK                                              0x7
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_STR_SWI_CAL_SEL_CTRL_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_STR_SWI_CAL_SEL_CTRL_ADDR, HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_STR_SWI_CAL_SEL_CTRL_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_STR_SWI_CAL_SEL_CTRL_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_STR_SWI_CAL_SEL_CTRL_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_STR_SWI_CAL_SEL_CTRL_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_STR_SWI_CAL_SEL_CTRL_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_STR_SWI_CAL_SEL_CTRL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_STR_SWI_CAL_SEL_CTRL_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_STR_SWI_CAL_SEL_CTRL_IN)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_STR_SWI_CAL_SEL_CTRL_DSIPHY_DEEMPH_SEL_BMSK                            0x4
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_STR_SWI_CAL_SEL_CTRL_DSIPHY_DEEMPH_SEL_SHFT                            0x2
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_STR_SWI_CAL_SEL_CTRL_DSIPHY_EQ_VALUE_OVERRIDE_BMSK                     0x2
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_STR_SWI_CAL_SEL_CTRL_DSIPHY_EQ_VALUE_OVERRIDE_SHFT                     0x1
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_STR_SWI_CAL_SEL_CTRL_DSIPHY_STR_VALUE_OVERRIDE_BMSK                    0x1
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_STR_SWI_CAL_SEL_CTRL_DSIPHY_STR_VALUE_OVERRIDE_SHFT                    0x0

#define HWIO_DSI_0_PHY_DSIPHY_CMN_VREG_CTRL_1_ADDR                                                     (DSI_0_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x00000110)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_VREG_CTRL_1_RMSK                                                           0xff
#define HWIO_DSI_0_PHY_DSIPHY_CMN_VREG_CTRL_1_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_VREG_CTRL_1_ADDR, HWIO_DSI_0_PHY_DSIPHY_CMN_VREG_CTRL_1_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_VREG_CTRL_1_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_VREG_CTRL_1_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_VREG_CTRL_1_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_CMN_VREG_CTRL_1_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_VREG_CTRL_1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_CMN_VREG_CTRL_1_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_CMN_VREG_CTRL_1_IN)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_VREG_CTRL_1_VREG_CTRL_0P2_BMSK                                             0xff
#define HWIO_DSI_0_PHY_DSIPHY_CMN_VREG_CTRL_1_VREG_CTRL_0P2_SHFT                                              0x0

#define HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_4_ADDR                                                          (DSI_0_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x00000114)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_4_RMSK                                                                0xff
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_4_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_4_ADDR, HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_4_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_4_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_4_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_4_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_4_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_4_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_4_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_4_IN)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_4_DCTRL_GLBL_2_BMSK                                                   0xff
#define HWIO_DSI_0_PHY_DSIPHY_CMN_CTRL_4_DCTRL_GLBL_2_SHFT                                                    0x0

#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE0_ADDR                                              (DSI_0_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x00000118)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE0_RMSK                                                    0xff
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE0_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE0_ADDR, HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE0_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE0_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE0_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE0_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE0_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE0_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE0_IN)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE0_DIGTOP_SPARE_7_0_BMSK                                   0xff
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE0_DIGTOP_SPARE_7_0_SHFT                                    0x0

#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE1_ADDR                                              (DSI_0_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x0000011c)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE1_RMSK                                                    0xff
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE1_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE1_ADDR, HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE1_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE1_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE1_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE1_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE1_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE1_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE1_IN)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE1_DIGTOP_SPARE_15_8_BMSK                                  0xff
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE1_DIGTOP_SPARE_15_8_SHFT                                   0x0

#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE2_ADDR                                              (DSI_0_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x00000120)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE2_RMSK                                                    0xff
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE2_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE2_ADDR, HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE2_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE2_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE2_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE2_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE2_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE2_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE2_IN)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE2_DIGTOP_SPARE_23_16_BMSK                                 0xff
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE2_DIGTOP_SPARE_23_16_SHFT                                  0x0

#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE3_ADDR                                              (DSI_0_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x00000124)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE3_RMSK                                                    0xff
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE3_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE3_ADDR, HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE3_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE3_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE3_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE3_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE3_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE3_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE3_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE3_IN)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE3_DIGTOP_SPARE_31_24_BMSK                                 0xff
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE3_DIGTOP_SPARE_31_24_SHFT                                  0x0

#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE4_ADDR                                              (DSI_0_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x00000128)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE4_RMSK                                                    0xff
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE4_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE4_ADDR, HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE4_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE4_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE4_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE4_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE4_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE4_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE4_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE4_IN)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE4_DIGTOP_SPARE_39_32_BMSK                                 0xff
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE4_DIGTOP_SPARE_39_32_SHFT                                  0x0

#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE5_ADDR                                              (DSI_0_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x0000012c)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE5_RMSK                                                    0xff
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE5_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE5_ADDR, HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE5_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE5_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE5_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE5_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE5_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE5_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE5_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE5_IN)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE5_DIGTOP_SPARE_47_40_BMSK                                 0xff
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE5_DIGTOP_SPARE_47_40_SHFT                                  0x0

#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE6_ADDR                                              (DSI_0_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x00000130)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE6_RMSK                                                    0xff
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE6_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE6_ADDR, HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE6_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE6_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE6_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE6_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE6_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE6_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE6_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE6_IN)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE6_DIGTOP_SPARE_55_48_BMSK                                 0xff
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE6_DIGTOP_SPARE_55_48_SHFT                                  0x0

#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE7_ADDR                                              (DSI_0_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x00000134)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE7_RMSK                                                    0xff
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE7_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE7_ADDR, HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE7_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE7_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE7_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE7_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE7_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE7_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE7_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE7_IN)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE7_DIGTOP_SPARE_63_56_BMSK                                 0xff
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE7_DIGTOP_SPARE_63_56_SHFT                                  0x0

#define HWIO_DSI_0_PHY_DSIPHY_CMN_HSTX_STR_STATUS0_ADDR                                                (DSI_0_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x00000138)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_HSTX_STR_STATUS0_RMSK                                                      0xff
#define HWIO_DSI_0_PHY_DSIPHY_CMN_HSTX_STR_STATUS0_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_HSTX_STR_STATUS0_ADDR, HWIO_DSI_0_PHY_DSIPHY_CMN_HSTX_STR_STATUS0_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_HSTX_STR_STATUS0_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_HSTX_STR_STATUS0_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_HSTX_STR_STATUS0_DSIPHY_HSTX_STR_HSTOP_STATUS_BMSK                         0xf0
#define HWIO_DSI_0_PHY_DSIPHY_CMN_HSTX_STR_STATUS0_DSIPHY_HSTX_STR_HSTOP_STATUS_SHFT                          0x4
#define HWIO_DSI_0_PHY_DSIPHY_CMN_HSTX_STR_STATUS0_DSIPHY_HSTX_STR_HSBOT_STATUS_BMSK                          0xf
#define HWIO_DSI_0_PHY_DSIPHY_CMN_HSTX_STR_STATUS0_DSIPHY_HSTX_STR_HSBOT_STATUS_SHFT                          0x0

#define HWIO_DSI_0_PHY_DSIPHY_CMN_HSTX_STR_STATUS1_ADDR                                                (DSI_0_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x0000013c)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_HSTX_STR_STATUS1_RMSK                                                       0xf
#define HWIO_DSI_0_PHY_DSIPHY_CMN_HSTX_STR_STATUS1_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_HSTX_STR_STATUS1_ADDR, HWIO_DSI_0_PHY_DSIPHY_CMN_HSTX_STR_STATUS1_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_HSTX_STR_STATUS1_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_HSTX_STR_STATUS1_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_HSTX_STR_STATUS1_DSIPHY_HSTX_STR_HSMID_STATUS_BMSK                          0xf
#define HWIO_DSI_0_PHY_DSIPHY_CMN_HSTX_STR_STATUS1_DSIPHY_HSTX_STR_HSMID_STATUS_SHFT                          0x0

#define HWIO_DSI_0_PHY_DSIPHY_CMN_PHY_STATUS_ADDR                                                      (DSI_0_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x00000140)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_PHY_STATUS_RMSK                                                             0x7
#define HWIO_DSI_0_PHY_DSIPHY_CMN_PHY_STATUS_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_PHY_STATUS_ADDR, HWIO_DSI_0_PHY_DSIPHY_CMN_PHY_STATUS_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_PHY_STATUS_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_PHY_STATUS_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_PHY_STATUS_PHY_READY_BMSK                                                   0x4
#define HWIO_DSI_0_PHY_DSIPHY_CMN_PHY_STATUS_PHY_READY_SHFT                                                   0x2
#define HWIO_DSI_0_PHY_DSIPHY_CMN_PHY_STATUS_PLL_READY_BMSK                                                   0x2
#define HWIO_DSI_0_PHY_DSIPHY_CMN_PHY_STATUS_PLL_READY_SHFT                                                   0x1
#define HWIO_DSI_0_PHY_DSIPHY_CMN_PHY_STATUS_REFGEN_READY_BMSK                                                0x1
#define HWIO_DSI_0_PHY_DSIPHY_CMN_PHY_STATUS_REFGEN_READY_SHFT                                                0x0

#define HWIO_DSI_0_PHY_DSIPHY_CMN_RESCODE_STATUS_ADDR                                                  (DSI_0_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x00000144)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_RESCODE_STATUS_RMSK                                                        0x7f
#define HWIO_DSI_0_PHY_DSIPHY_CMN_RESCODE_STATUS_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_RESCODE_STATUS_ADDR, HWIO_DSI_0_PHY_DSIPHY_CMN_RESCODE_STATUS_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_RESCODE_STATUS_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_RESCODE_STATUS_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_RESCODE_STATUS_RESCODE_BMSK                                                0x7f
#define HWIO_DSI_0_PHY_DSIPHY_CMN_RESCODE_STATUS_RESCODE_SHFT                                                 0x0

#define HWIO_DSI_0_PHY_DSIPHY_CMN_LANE_STATUS0_ADDR                                                    (DSI_0_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x00000148)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_LANE_STATUS0_RMSK                                                          0x1f
#define HWIO_DSI_0_PHY_DSIPHY_CMN_LANE_STATUS0_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_LANE_STATUS0_ADDR, HWIO_DSI_0_PHY_DSIPHY_CMN_LANE_STATUS0_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_LANE_STATUS0_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_LANE_STATUS0_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_LANE_STATUS0_CLKLN_ULPS_ACTIVE_NOT_BMSK                                    0x10
#define HWIO_DSI_0_PHY_DSIPHY_CMN_LANE_STATUS0_CLKLN_ULPS_ACTIVE_NOT_SHFT                                     0x4
#define HWIO_DSI_0_PHY_DSIPHY_CMN_LANE_STATUS0_DLN3_ULPS_ACTIVE_NOT_BMSK                                      0x8
#define HWIO_DSI_0_PHY_DSIPHY_CMN_LANE_STATUS0_DLN3_ULPS_ACTIVE_NOT_SHFT                                      0x3
#define HWIO_DSI_0_PHY_DSIPHY_CMN_LANE_STATUS0_DLN2_ULPS_ACTIVE_NOT_BMSK                                      0x4
#define HWIO_DSI_0_PHY_DSIPHY_CMN_LANE_STATUS0_DLN2_ULPS_ACTIVE_NOT_SHFT                                      0x2
#define HWIO_DSI_0_PHY_DSIPHY_CMN_LANE_STATUS0_DLN1_ULPS_ACTIVE_NOT_BMSK                                      0x2
#define HWIO_DSI_0_PHY_DSIPHY_CMN_LANE_STATUS0_DLN1_ULPS_ACTIVE_NOT_SHFT                                      0x1
#define HWIO_DSI_0_PHY_DSIPHY_CMN_LANE_STATUS0_DLN0_ULPS_ACTIVE_NOT_BMSK                                      0x1
#define HWIO_DSI_0_PHY_DSIPHY_CMN_LANE_STATUS0_DLN0_ULPS_ACTIVE_NOT_SHFT                                      0x0

#define HWIO_DSI_0_PHY_DSIPHY_CMN_LANE_STATUS1_ADDR                                                    (DSI_0_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x0000014c)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_LANE_STATUS1_RMSK                                                          0x1f
#define HWIO_DSI_0_PHY_DSIPHY_CMN_LANE_STATUS1_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_LANE_STATUS1_ADDR, HWIO_DSI_0_PHY_DSIPHY_CMN_LANE_STATUS1_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_LANE_STATUS1_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_LANE_STATUS1_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_LANE_STATUS1_CLKLN_STOPSTATE_BMSK                                          0x10
#define HWIO_DSI_0_PHY_DSIPHY_CMN_LANE_STATUS1_CLKLN_STOPSTATE_SHFT                                           0x4
#define HWIO_DSI_0_PHY_DSIPHY_CMN_LANE_STATUS1_DLN3_STOPSTATE_BMSK                                            0x8
#define HWIO_DSI_0_PHY_DSIPHY_CMN_LANE_STATUS1_DLN3_STOPSTATE_SHFT                                            0x3
#define HWIO_DSI_0_PHY_DSIPHY_CMN_LANE_STATUS1_DLN2_STOPSTATE_BMSK                                            0x4
#define HWIO_DSI_0_PHY_DSIPHY_CMN_LANE_STATUS1_DLN2_STOPSTATE_SHFT                                            0x2
#define HWIO_DSI_0_PHY_DSIPHY_CMN_LANE_STATUS1_DLN1_STOPSTATE_BMSK                                            0x2
#define HWIO_DSI_0_PHY_DSIPHY_CMN_LANE_STATUS1_DLN1_STOPSTATE_SHFT                                            0x1
#define HWIO_DSI_0_PHY_DSIPHY_CMN_LANE_STATUS1_DLN0_STOPSTATE_BMSK                                            0x1
#define HWIO_DSI_0_PHY_DSIPHY_CMN_LANE_STATUS1_DLN0_STOPSTATE_SHFT                                            0x0

#define HWIO_DSI_0_PHY_DSIPHY_CMN_MISR_STATUS00_ADDR                                                   (DSI_0_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x00000150)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_MISR_STATUS00_RMSK                                                         0xff
#define HWIO_DSI_0_PHY_DSIPHY_CMN_MISR_STATUS00_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_MISR_STATUS00_ADDR, HWIO_DSI_0_PHY_DSIPHY_CMN_MISR_STATUS00_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_MISR_STATUS00_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_MISR_STATUS00_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_MISR_STATUS00_MISR_SIGNATURE0_7_0_BMSK                                     0xff
#define HWIO_DSI_0_PHY_DSIPHY_CMN_MISR_STATUS00_MISR_SIGNATURE0_7_0_SHFT                                      0x0

#define HWIO_DSI_0_PHY_DSIPHY_CMN_MISR_STATUS01_ADDR                                                   (DSI_0_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x00000154)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_MISR_STATUS01_RMSK                                                         0xff
#define HWIO_DSI_0_PHY_DSIPHY_CMN_MISR_STATUS01_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_MISR_STATUS01_ADDR, HWIO_DSI_0_PHY_DSIPHY_CMN_MISR_STATUS01_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_MISR_STATUS01_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_MISR_STATUS01_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_MISR_STATUS01_MISR_SIGNATURE0_15_8_BMSK                                    0xff
#define HWIO_DSI_0_PHY_DSIPHY_CMN_MISR_STATUS01_MISR_SIGNATURE0_15_8_SHFT                                     0x0

#define HWIO_DSI_0_PHY_DSIPHY_CMN_MISR_STATUS10_ADDR                                                   (DSI_0_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x00000158)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_MISR_STATUS10_RMSK                                                         0xff
#define HWIO_DSI_0_PHY_DSIPHY_CMN_MISR_STATUS10_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_MISR_STATUS10_ADDR, HWIO_DSI_0_PHY_DSIPHY_CMN_MISR_STATUS10_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_MISR_STATUS10_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_MISR_STATUS10_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_MISR_STATUS10_MISR_SIGNATURE1_7_0_BMSK                                     0xff
#define HWIO_DSI_0_PHY_DSIPHY_CMN_MISR_STATUS10_MISR_SIGNATURE1_7_0_SHFT                                      0x0

#define HWIO_DSI_0_PHY_DSIPHY_CMN_MISR_STATUS11_ADDR                                                   (DSI_0_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x0000015c)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_MISR_STATUS11_RMSK                                                         0xff
#define HWIO_DSI_0_PHY_DSIPHY_CMN_MISR_STATUS11_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_MISR_STATUS11_ADDR, HWIO_DSI_0_PHY_DSIPHY_CMN_MISR_STATUS11_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_MISR_STATUS11_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_MISR_STATUS11_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_MISR_STATUS11_MISR_SIGNATURE1_15_8_BMSK                                    0xff
#define HWIO_DSI_0_PHY_DSIPHY_CMN_MISR_STATUS11_MISR_SIGNATURE1_15_8_SHFT                                     0x0

#define HWIO_DSI_0_PHY_DSIPHY_CMN_MISR_STATUS20_ADDR                                                   (DSI_0_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x00000160)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_MISR_STATUS20_RMSK                                                         0xff
#define HWIO_DSI_0_PHY_DSIPHY_CMN_MISR_STATUS20_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_MISR_STATUS20_ADDR, HWIO_DSI_0_PHY_DSIPHY_CMN_MISR_STATUS20_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_MISR_STATUS20_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_MISR_STATUS20_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_MISR_STATUS20_MISR_SIGNATURE2_7_0_BMSK                                     0xff
#define HWIO_DSI_0_PHY_DSIPHY_CMN_MISR_STATUS20_MISR_SIGNATURE2_7_0_SHFT                                      0x0

#define HWIO_DSI_0_PHY_DSIPHY_CMN_MISR_STATUS21_ADDR                                                   (DSI_0_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x00000164)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_MISR_STATUS21_RMSK                                                         0xff
#define HWIO_DSI_0_PHY_DSIPHY_CMN_MISR_STATUS21_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_MISR_STATUS21_ADDR, HWIO_DSI_0_PHY_DSIPHY_CMN_MISR_STATUS21_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_MISR_STATUS21_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_MISR_STATUS21_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_MISR_STATUS21_MISR_SIGNATURE2_15_8_BMSK                                    0xff
#define HWIO_DSI_0_PHY_DSIPHY_CMN_MISR_STATUS21_MISR_SIGNATURE2_15_8_SHFT                                     0x0

#define HWIO_DSI_0_PHY_DSIPHY_CMN_MISR_STATUS30_ADDR                                                   (DSI_0_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x00000168)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_MISR_STATUS30_RMSK                                                         0xff
#define HWIO_DSI_0_PHY_DSIPHY_CMN_MISR_STATUS30_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_MISR_STATUS30_ADDR, HWIO_DSI_0_PHY_DSIPHY_CMN_MISR_STATUS30_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_MISR_STATUS30_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_MISR_STATUS30_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_MISR_STATUS30_MISR_SIGNATURE3_7_0_BMSK                                     0xff
#define HWIO_DSI_0_PHY_DSIPHY_CMN_MISR_STATUS30_MISR_SIGNATURE3_7_0_SHFT                                      0x0

#define HWIO_DSI_0_PHY_DSIPHY_CMN_MISR_STATUS31_ADDR                                                   (DSI_0_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x0000016c)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_MISR_STATUS31_RMSK                                                         0xff
#define HWIO_DSI_0_PHY_DSIPHY_CMN_MISR_STATUS31_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_MISR_STATUS31_ADDR, HWIO_DSI_0_PHY_DSIPHY_CMN_MISR_STATUS31_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_MISR_STATUS31_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_MISR_STATUS31_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_MISR_STATUS31_MISR_SIGNATURE3_15_8_BMSK                                    0xff
#define HWIO_DSI_0_PHY_DSIPHY_CMN_MISR_STATUS31_MISR_SIGNATURE3_15_8_SHFT                                     0x0

#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DEBUG_BUS0_ADDR                                                 (DSI_0_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x00000170)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DEBUG_BUS0_RMSK                                                       0xff
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DEBUG_BUS0_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DEBUG_BUS0_ADDR, HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DEBUG_BUS0_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DEBUG_BUS0_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DEBUG_BUS0_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DEBUG_BUS0_DEBUG_BUS_7_0_BMSK                                         0xff
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DEBUG_BUS0_DEBUG_BUS_7_0_SHFT                                          0x0

#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DEBUG_BUS1_ADDR                                                 (DSI_0_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x00000174)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DEBUG_BUS1_RMSK                                                       0xff
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DEBUG_BUS1_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DEBUG_BUS1_ADDR, HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DEBUG_BUS1_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DEBUG_BUS1_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DEBUG_BUS1_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DEBUG_BUS1_DEBUG_BUS_15_8_BMSK                                        0xff
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DEBUG_BUS1_DEBUG_BUS_15_8_SHFT                                         0x0

#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DEBUG_BUS2_ADDR                                                 (DSI_0_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x00000178)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DEBUG_BUS2_RMSK                                                       0xff
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DEBUG_BUS2_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DEBUG_BUS2_ADDR, HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DEBUG_BUS2_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DEBUG_BUS2_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DEBUG_BUS2_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DEBUG_BUS2_DEBUG_BUS_23_16_BMSK                                       0xff
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DEBUG_BUS2_DEBUG_BUS_23_16_SHFT                                        0x0

#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DEBUG_BUS3_ADDR                                                 (DSI_0_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x0000017c)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DEBUG_BUS3_RMSK                                                       0xff
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DEBUG_BUS3_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DEBUG_BUS3_ADDR, HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DEBUG_BUS3_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DEBUG_BUS3_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DEBUG_BUS3_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DEBUG_BUS3_DEBUG_BUS_31_24_BMSK                                       0xff
#define HWIO_DSI_0_PHY_DSIPHY_CMN_GLBL_DEBUG_BUS3_DEBUG_BUS_31_24_SHFT                                        0x0

/*----------------------------------------------------------------------------
 * MODULE: DSI_0_PHY_DSIPHY_DLN0_DSIPHY_DLN0_DSI_DATALN
 *--------------------------------------------------------------------------*/

#define DSI_0_PHY_DSIPHY_DLN0_DSIPHY_DLN0_DSI_DATALN_REG_BASE                          (MDSS_BASE      + 0x00094600)

#define HWIO_DSI_0_PHY_DSIPHY_DLN0_CFG0_ADDR                                           (DSI_0_PHY_DSIPHY_DLN0_DSIPHY_DLN0_DSI_DATALN_REG_BASE      + 0x00000000)
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_CFG0_RMSK                                                 0x3f
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_CFG0_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN0_CFG0_ADDR, HWIO_DSI_0_PHY_DSIPHY_DLN0_CFG0_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_CFG0_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN0_CFG0_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_CFG0_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_DLN0_CFG0_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_CFG0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_DLN0_CFG0_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_DLN0_CFG0_IN)
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_CFG0_DSIPHY_HSTX_PGM_DLY_5_4_BMSK                         0x30
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_CFG0_DSIPHY_HSTX_PGM_DLY_5_4_SHFT                          0x4
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_CFG0_DSIPHY_HSTX_PGM_DLY_3_0_BMSK                          0xf
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_CFG0_DSIPHY_HSTX_PGM_DLY_3_0_SHFT                          0x0

#define HWIO_DSI_0_PHY_DSIPHY_DLN0_CFG1_ADDR                                           (DSI_0_PHY_DSIPHY_DLN0_DSIPHY_DLN0_DSI_DATALN_REG_BASE      + 0x00000004)
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_CFG1_RMSK                                                 0x1f
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_CFG1_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN0_CFG1_ADDR, HWIO_DSI_0_PHY_DSIPHY_DLN0_CFG1_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_CFG1_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN0_CFG1_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_CFG1_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_DLN0_CFG1_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_CFG1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_DLN0_CFG1_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_DLN0_CFG1_IN)
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_CFG1_PROGSEQ_EN_BMSK                                      0x10
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_CFG1_PROGSEQ_EN_SHFT                                       0x4
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_CFG1_DSIPHY_HSTX_SLEW_RATE_BMSK                            0xf
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_CFG1_DSIPHY_HSTX_SLEW_RATE_SHFT                            0x0

#define HWIO_DSI_0_PHY_DSIPHY_DLN0_CFG2_ADDR                                           (DSI_0_PHY_DSIPHY_DLN0_DSIPHY_DLN0_DSI_DATALN_REG_BASE      + 0x00000008)
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_CFG2_RMSK                                                 0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_CFG2_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN0_CFG2_ADDR, HWIO_DSI_0_PHY_DSIPHY_DLN0_CFG2_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_CFG2_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN0_CFG2_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_CFG2_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_DLN0_CFG2_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_CFG2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_DLN0_CFG2_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_DLN0_CFG2_IN)
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_CFG2_IS_CKLANE_BMSK                                       0x80
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_CFG2_IS_CKLANE_SHFT                                        0x7
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_CFG2_CLK_PATTERN_HSTX_INV_BMSK                            0x40
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_CFG2_CLK_PATTERN_HSTX_INV_SHFT                             0x6
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_CFG2_TEST_MODE_LPTX_DATAN_BMSK                            0x20
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_CFG2_TEST_MODE_LPTX_DATAN_SHFT                             0x5
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_CFG2_TEST_MODE_LPTX_DATAP_BMSK                            0x10
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_CFG2_TEST_MODE_LPTX_DATAP_SHFT                             0x4
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_CFG2_DSIPHY_LPRX_DLY_BMSK                                  0xf
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_CFG2_DSIPHY_LPRX_DLY_SHFT                                  0x0

#define HWIO_DSI_0_PHY_DSIPHY_DLN0_TEST_DATAPATH_ADDR                                  (DSI_0_PHY_DSIPHY_DLN0_DSIPHY_DLN0_DSI_DATALN_REG_BASE      + 0x0000000c)
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_TEST_DATAPATH_RMSK                                        0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_TEST_DATAPATH_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN0_TEST_DATAPATH_ADDR, HWIO_DSI_0_PHY_DSIPHY_DLN0_TEST_DATAPATH_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_TEST_DATAPATH_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN0_TEST_DATAPATH_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_TEST_DATAPATH_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_DLN0_TEST_DATAPATH_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_TEST_DATAPATH_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_DLN0_TEST_DATAPATH_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_DLN0_TEST_DATAPATH_IN)
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_TEST_DATAPATH_TEST_MODE_LPTX_EN_BMSK                      0x80
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_TEST_DATAPATH_TEST_MODE_LPTX_EN_SHFT                       0x7
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_TEST_DATAPATH_ANE_BYPASS_EN_BMSK                          0x40
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_TEST_DATAPATH_ANE_BYPASS_EN_SHFT                           0x6
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_TEST_DATAPATH_TEST_MODE_HSTX_DATA_BMSK                    0x20
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_TEST_DATAPATH_TEST_MODE_HSTX_DATA_SHFT                     0x5
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_TEST_DATAPATH_DMUX_LP_SEL_BMSK                            0x18
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_TEST_DATAPATH_DMUX_LP_SEL_SHFT                             0x3
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_TEST_DATAPATH_DMUX_HS_SEL_BMSK                             0x4
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_TEST_DATAPATH_DMUX_HS_SEL_SHFT                             0x2
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_TEST_DATAPATH_TEST_MODE_HSTX_EN_BMSK                       0x2
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_TEST_DATAPATH_TEST_MODE_HSTX_EN_SHFT                       0x1
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_TEST_DATAPATH_FORCE_TEST_MODE_BMSK                         0x1
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_TEST_DATAPATH_FORCE_TEST_MODE_SHFT                         0x0

#define HWIO_DSI_0_PHY_DSIPHY_DLN0_PIN_SWAP_ADDR                                       (DSI_0_PHY_DSIPHY_DLN0_DSIPHY_DLN0_DSI_DATALN_REG_BASE      + 0x00000010)
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_PIN_SWAP_RMSK                                              0xf
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_PIN_SWAP_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN0_PIN_SWAP_ADDR, HWIO_DSI_0_PHY_DSIPHY_DLN0_PIN_SWAP_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_PIN_SWAP_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN0_PIN_SWAP_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_PIN_SWAP_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_DLN0_PIN_SWAP_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_PIN_SWAP_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_DLN0_PIN_SWAP_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_DLN0_PIN_SWAP_IN)
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_PIN_SWAP_ABC_SWAP_BMSK                                     0xe
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_PIN_SWAP_ABC_SWAP_SHFT                                     0x1
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_PIN_SWAP_PN_SWAP_BMSK                                      0x1
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_PIN_SWAP_PN_SWAP_SHFT                                      0x0

#define HWIO_DSI_0_PHY_DSIPHY_DLN0_LPRX_CTRL_ADDR                                      (DSI_0_PHY_DSIPHY_DLN0_DSIPHY_DLN0_DSI_DATALN_REG_BASE      + 0x00000014)
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_LPRX_CTRL_RMSK                                             0x7
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_LPRX_CTRL_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN0_LPRX_CTRL_ADDR, HWIO_DSI_0_PHY_DSIPHY_DLN0_LPRX_CTRL_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_LPRX_CTRL_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN0_LPRX_CTRL_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_LPRX_CTRL_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_DLN0_LPRX_CTRL_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_LPRX_CTRL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_DLN0_LPRX_CTRL_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_DLN0_LPRX_CTRL_IN)
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_LPRX_CTRL_HS_CONTENTION_DIS_BMSK                           0x4
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_LPRX_CTRL_HS_CONTENTION_DIS_SHFT                           0x2
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_LPRX_CTRL_DSIPHY_LPRX_EN_BMSK                              0x2
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_LPRX_CTRL_DSIPHY_LPRX_EN_SHFT                              0x1
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_LPRX_CTRL_DSIPHY_CDRX_EN_BMSK                              0x1
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_LPRX_CTRL_DSIPHY_CDRX_EN_SHFT                              0x0

#define HWIO_DSI_0_PHY_DSIPHY_DLN0_TX_DCTRL_ADDR                                       (DSI_0_PHY_DSIPHY_DLN0_DSIPHY_DLN0_DSI_DATALN_REG_BASE      + 0x00000018)
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_TX_DCTRL_RMSK                                             0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_TX_DCTRL_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN0_TX_DCTRL_ADDR, HWIO_DSI_0_PHY_DSIPHY_DLN0_TX_DCTRL_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_TX_DCTRL_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN0_TX_DCTRL_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_TX_DCTRL_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_DLN0_TX_DCTRL_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_TX_DCTRL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_DLN0_TX_DCTRL_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_DLN0_TX_DCTRL_IN)
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_TX_DCTRL_DCTRL_BMSK                                       0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_TX_DCTRL_DCTRL_SHFT                                        0x0

#define HWIO_DSI_0_PHY_DSIPHY_DLN0_BIST_CTRL0_ADDR                                     (DSI_0_PHY_DSIPHY_DLN0_DSIPHY_DLN0_DSI_DATALN_REG_BASE      + 0x0000001c)
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_BIST_CTRL0_RMSK                                           0x3f
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_BIST_CTRL0_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN0_BIST_CTRL0_ADDR, HWIO_DSI_0_PHY_DSIPHY_DLN0_BIST_CTRL0_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_BIST_CTRL0_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN0_BIST_CTRL0_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_BIST_CTRL0_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_DLN0_BIST_CTRL0_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_BIST_CTRL0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_DLN0_BIST_CTRL0_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_DLN0_BIST_CTRL0_IN)
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_BIST_CTRL0_LP_BISTCHK_ENABLE_BMSK                         0x20
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_BIST_CTRL0_LP_BISTCHK_ENABLE_SHFT                          0x5
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_BIST_CTRL0_BISTCHK_CAPTURE_BMSK                           0x10
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_BIST_CTRL0_BISTCHK_CAPTURE_SHFT                            0x4
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_BIST_CTRL0_BISTCHK_CLR_ERROR_CNT_BMSK                      0x8
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_BIST_CTRL0_BISTCHK_CLR_ERROR_CNT_SHFT                      0x3
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_BIST_CTRL0_BISTCHK_MODE_BMSK                               0x4
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_BIST_CTRL0_BISTCHK_MODE_SHFT                               0x2
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_BIST_CTRL0_BISTCHK_CROSS_BMSK                              0x2
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_BIST_CTRL0_BISTCHK_CROSS_SHFT                              0x1
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_BIST_CTRL0_BISTCHK_ENABLE_BMSK                             0x1
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_BIST_CTRL0_BISTCHK_ENABLE_SHFT                             0x0

#define HWIO_DSI_0_PHY_DSIPHY_DLN0_BIST_CTRL1_ADDR                                     (DSI_0_PHY_DSIPHY_DLN0_DSIPHY_DLN0_DSI_DATALN_REG_BASE      + 0x00000020)
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_BIST_CTRL1_RMSK                                           0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_BIST_CTRL1_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN0_BIST_CTRL1_ADDR, HWIO_DSI_0_PHY_DSIPHY_DLN0_BIST_CTRL1_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_BIST_CTRL1_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN0_BIST_CTRL1_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_BIST_CTRL1_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_DLN0_BIST_CTRL1_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_BIST_CTRL1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_DLN0_BIST_CTRL1_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_DLN0_BIST_CTRL1_IN)
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_BIST_CTRL1_BIST_WORD_CNT_7_0_BMSK                         0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_BIST_CTRL1_BIST_WORD_CNT_7_0_SHFT                          0x0

#define HWIO_DSI_0_PHY_DSIPHY_DLN0_BIST_CTRL2_ADDR                                     (DSI_0_PHY_DSIPHY_DLN0_DSIPHY_DLN0_DSI_DATALN_REG_BASE      + 0x00000024)
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_BIST_CTRL2_RMSK                                           0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_BIST_CTRL2_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN0_BIST_CTRL2_ADDR, HWIO_DSI_0_PHY_DSIPHY_DLN0_BIST_CTRL2_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_BIST_CTRL2_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN0_BIST_CTRL2_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_BIST_CTRL2_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_DLN0_BIST_CTRL2_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_BIST_CTRL2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_DLN0_BIST_CTRL2_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_DLN0_BIST_CTRL2_IN)
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_BIST_CTRL2_BIST_WORD_CNT_15_8_BMSK                        0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_BIST_CTRL2_BIST_WORD_CNT_15_8_SHFT                         0x0

#define HWIO_DSI_0_PHY_DSIPHY_DLN0_MISR_CTRL_ADDR                                      (DSI_0_PHY_DSIPHY_DLN0_DSIPHY_DLN0_DSI_DATALN_REG_BASE      + 0x00000028)
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_MISR_CTRL_RMSK                                            0x1f
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_MISR_CTRL_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN0_MISR_CTRL_ADDR, HWIO_DSI_0_PHY_DSIPHY_DLN0_MISR_CTRL_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_MISR_CTRL_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN0_MISR_CTRL_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_MISR_CTRL_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_DLN0_MISR_CTRL_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_MISR_CTRL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_DLN0_MISR_CTRL_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_DLN0_MISR_CTRL_IN)
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_MISR_CTRL_DEBUG_CLK_DEBUG_SEL_BMSK                        0x1c
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_MISR_CTRL_DEBUG_CLK_DEBUG_SEL_SHFT                         0x2
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_MISR_CTRL_MISR_SIG_CLEAR_BMSK                              0x2
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_MISR_CTRL_MISR_SIG_CLEAR_SHFT                              0x1
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_MISR_CTRL_MISR_ENABLE_BMSK                                 0x1
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_MISR_CTRL_MISR_ENABLE_SHFT                                 0x0

#define HWIO_DSI_0_PHY_DSIPHY_DLN0_DBG_SEL_ADDR                                        (DSI_0_PHY_DSIPHY_DLN0_DSIPHY_DLN0_DSI_DATALN_REG_BASE      + 0x0000002c)
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_DBG_SEL_RMSK                                              0x7f
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_DBG_SEL_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN0_DBG_SEL_ADDR, HWIO_DSI_0_PHY_DSIPHY_DLN0_DBG_SEL_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_DBG_SEL_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN0_DBG_SEL_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_DBG_SEL_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_DLN0_DBG_SEL_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_DBG_SEL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_DLN0_DBG_SEL_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_DLN0_DBG_SEL_IN)
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_DBG_SEL_TEST_CLK_DEBUG_SEL_BMSK                           0x70
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_DBG_SEL_TEST_CLK_DEBUG_SEL_SHFT                            0x4
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_DBG_SEL_DEBUG_SEL_BMSK                                     0xf
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_DBG_SEL_DEBUG_SEL_SHFT                                     0x0

#define HWIO_DSI_0_PHY_DSIPHY_DLN0_SWI_DBG_BUS_CHECK_ADDR                              (DSI_0_PHY_DSIPHY_DLN0_DSIPHY_DLN0_DSI_DATALN_REG_BASE      + 0x00000030)
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_SWI_DBG_BUS_CHECK_RMSK                                    0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_SWI_DBG_BUS_CHECK_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN0_SWI_DBG_BUS_CHECK_ADDR, HWIO_DSI_0_PHY_DSIPHY_DLN0_SWI_DBG_BUS_CHECK_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_SWI_DBG_BUS_CHECK_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN0_SWI_DBG_BUS_CHECK_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_SWI_DBG_BUS_CHECK_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_DLN0_SWI_DBG_BUS_CHECK_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_SWI_DBG_BUS_CHECK_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_DLN0_SWI_DBG_BUS_CHECK_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_DLN0_SWI_DBG_BUS_CHECK_IN)
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_SWI_DBG_BUS_CHECK_LANE_DEBUG_BUS_BMSK                     0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_SWI_DBG_BUS_CHECK_LANE_DEBUG_BUS_SHFT                      0x0

#define HWIO_DSI_0_PHY_DSIPHY_DLN0_BIST_STATUS0_ADDR                                   (DSI_0_PHY_DSIPHY_DLN0_DSIPHY_DLN0_DSI_DATALN_REG_BASE      + 0x00000034)
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_BIST_STATUS0_RMSK                                         0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_BIST_STATUS0_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN0_BIST_STATUS0_ADDR, HWIO_DSI_0_PHY_DSIPHY_DLN0_BIST_STATUS0_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_BIST_STATUS0_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN0_BIST_STATUS0_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_BIST_STATUS0_BISTCHK_ONE_BIT_ERROR_BMSK                   0x80
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_BIST_STATUS0_BISTCHK_ONE_BIT_ERROR_SHFT                    0x7
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_BIST_STATUS0_BISTCHK_SYNC_ERROR_BMSK                      0x40
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_BIST_STATUS0_BISTCHK_SYNC_ERROR_SHFT                       0x6
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_BIST_STATUS0_BISTCHK_SEED_ERROR_BMSK                      0x20
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_BIST_STATUS0_BISTCHK_SEED_ERROR_SHFT                       0x5
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_BIST_STATUS0_BISTCHK_PRBS_ERROR_BMSK                      0x10
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_BIST_STATUS0_BISTCHK_PRBS_ERROR_SHFT                       0x4
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_BIST_STATUS0_BISTCHK_STATUS_BMSK                           0xe
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_BIST_STATUS0_BISTCHK_STATUS_SHFT                           0x1
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_BIST_STATUS0_BISTCHK_DONE_BMSK                             0x1
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_BIST_STATUS0_BISTCHK_DONE_SHFT                             0x0

#define HWIO_DSI_0_PHY_DSIPHY_DLN0_BIST_STATUS1_ADDR                                   (DSI_0_PHY_DSIPHY_DLN0_DSIPHY_DLN0_DSI_DATALN_REG_BASE      + 0x00000038)
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_BIST_STATUS1_RMSK                                         0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_BIST_STATUS1_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN0_BIST_STATUS1_ADDR, HWIO_DSI_0_PHY_DSIPHY_DLN0_BIST_STATUS1_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_BIST_STATUS1_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN0_BIST_STATUS1_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_BIST_STATUS1_BISTCHK_ERROR_CNT_7_0_BMSK                   0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_BIST_STATUS1_BISTCHK_ERROR_CNT_7_0_SHFT                    0x0

#define HWIO_DSI_0_PHY_DSIPHY_DLN0_BIST_STATUS2_ADDR                                   (DSI_0_PHY_DSIPHY_DLN0_DSIPHY_DLN0_DSI_DATALN_REG_BASE      + 0x0000003c)
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_BIST_STATUS2_RMSK                                         0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_BIST_STATUS2_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN0_BIST_STATUS2_ADDR, HWIO_DSI_0_PHY_DSIPHY_DLN0_BIST_STATUS2_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_BIST_STATUS2_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN0_BIST_STATUS2_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_BIST_STATUS2_BISTCHK_ERROR_CNT_15_8_BMSK                  0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_BIST_STATUS2_BISTCHK_ERROR_CNT_15_8_SHFT                   0x0

#define HWIO_DSI_0_PHY_DSIPHY_DLN0_BIST_STATUS3_ADDR                                   (DSI_0_PHY_DSIPHY_DLN0_DSIPHY_DLN0_DSI_DATALN_REG_BASE      + 0x00000040)
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_BIST_STATUS3_RMSK                                         0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_BIST_STATUS3_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN0_BIST_STATUS3_ADDR, HWIO_DSI_0_PHY_DSIPHY_DLN0_BIST_STATUS3_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_BIST_STATUS3_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN0_BIST_STATUS3_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_BIST_STATUS3_BISTCHK_WORD_CNT_7_0_BMSK                    0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_BIST_STATUS3_BISTCHK_WORD_CNT_7_0_SHFT                     0x0

#define HWIO_DSI_0_PHY_DSIPHY_DLN0_BIST_STATUS4_ADDR                                   (DSI_0_PHY_DSIPHY_DLN0_DSIPHY_DLN0_DSI_DATALN_REG_BASE      + 0x00000044)
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_BIST_STATUS4_RMSK                                         0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_BIST_STATUS4_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN0_BIST_STATUS4_ADDR, HWIO_DSI_0_PHY_DSIPHY_DLN0_BIST_STATUS4_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_BIST_STATUS4_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN0_BIST_STATUS4_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_BIST_STATUS4_BISTCHK_WORD_CNT_15_8_BMSK                   0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_BIST_STATUS4_BISTCHK_WORD_CNT_15_8_SHFT                    0x0

#define HWIO_DSI_0_PHY_DSIPHY_DLN0_LP_BIST_STATUS0_ADDR                                (DSI_0_PHY_DSIPHY_DLN0_DSIPHY_DLN0_DSI_DATALN_REG_BASE      + 0x00000048)
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_LP_BIST_STATUS0_RMSK                                       0x3
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_LP_BIST_STATUS0_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN0_LP_BIST_STATUS0_ADDR, HWIO_DSI_0_PHY_DSIPHY_DLN0_LP_BIST_STATUS0_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_LP_BIST_STATUS0_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN0_LP_BIST_STATUS0_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_LP_BIST_STATUS0_LP_BISTCHK_PRBS_ERROR_BMSK                 0x2
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_LP_BIST_STATUS0_LP_BISTCHK_PRBS_ERROR_SHFT                 0x1
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_LP_BIST_STATUS0_LP_BISTCHK_DONE_BMSK                       0x1
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_LP_BIST_STATUS0_LP_BISTCHK_DONE_SHFT                       0x0

#define HWIO_DSI_0_PHY_DSIPHY_DLN0_LP_BIST_STATUS1_ADDR                                (DSI_0_PHY_DSIPHY_DLN0_DSIPHY_DLN0_DSI_DATALN_REG_BASE      + 0x0000004c)
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_LP_BIST_STATUS1_RMSK                                      0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_LP_BIST_STATUS1_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN0_LP_BIST_STATUS1_ADDR, HWIO_DSI_0_PHY_DSIPHY_DLN0_LP_BIST_STATUS1_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_LP_BIST_STATUS1_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN0_LP_BIST_STATUS1_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_LP_BIST_STATUS1_LP_BISTCHK_ERROR_CNT_BMSK                 0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_LP_BIST_STATUS1_LP_BISTCHK_ERROR_CNT_SHFT                  0x0

#define HWIO_DSI_0_PHY_DSIPHY_DLN0_LP_BIST_STATUS2_ADDR                                (DSI_0_PHY_DSIPHY_DLN0_DSIPHY_DLN0_DSI_DATALN_REG_BASE      + 0x00000050)
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_LP_BIST_STATUS2_RMSK                                      0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_LP_BIST_STATUS2_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN0_LP_BIST_STATUS2_ADDR, HWIO_DSI_0_PHY_DSIPHY_DLN0_LP_BIST_STATUS2_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_LP_BIST_STATUS2_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN0_LP_BIST_STATUS2_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_LP_BIST_STATUS2_LP_BISTCHK_WORD_CNT_BMSK                  0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_LP_BIST_STATUS2_LP_BISTCHK_WORD_CNT_SHFT                   0x0

#define HWIO_DSI_0_PHY_DSIPHY_DLN0_MISR_STATUS0_ADDR                                   (DSI_0_PHY_DSIPHY_DLN0_DSIPHY_DLN0_DSI_DATALN_REG_BASE      + 0x00000054)
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_MISR_STATUS0_RMSK                                         0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_MISR_STATUS0_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN0_MISR_STATUS0_ADDR, HWIO_DSI_0_PHY_DSIPHY_DLN0_MISR_STATUS0_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_MISR_STATUS0_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN0_MISR_STATUS0_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_MISR_STATUS0_BISTCHK_MISR_SIG_7_0_BMSK                    0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_MISR_STATUS0_BISTCHK_MISR_SIG_7_0_SHFT                     0x0

#define HWIO_DSI_0_PHY_DSIPHY_DLN0_MISR_STATUS1_ADDR                                   (DSI_0_PHY_DSIPHY_DLN0_DSIPHY_DLN0_DSI_DATALN_REG_BASE      + 0x00000058)
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_MISR_STATUS1_RMSK                                         0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_MISR_STATUS1_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN0_MISR_STATUS1_ADDR, HWIO_DSI_0_PHY_DSIPHY_DLN0_MISR_STATUS1_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_MISR_STATUS1_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN0_MISR_STATUS1_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_MISR_STATUS1_BISTCHK_MISR_SIG_15_8_BMSK                   0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_MISR_STATUS1_BISTCHK_MISR_SIG_15_8_SHFT                    0x0

#define HWIO_DSI_0_PHY_DSIPHY_DLN0_DEBUG_BUS0_ADDR                                     (DSI_0_PHY_DSIPHY_DLN0_DSIPHY_DLN0_DSI_DATALN_REG_BASE      + 0x0000005c)
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_DEBUG_BUS0_RMSK                                           0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_DEBUG_BUS0_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN0_DEBUG_BUS0_ADDR, HWIO_DSI_0_PHY_DSIPHY_DLN0_DEBUG_BUS0_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_DEBUG_BUS0_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN0_DEBUG_BUS0_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_DEBUG_BUS0_DEBUG_BUS_7_0_BMSK                             0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_DEBUG_BUS0_DEBUG_BUS_7_0_SHFT                              0x0

#define HWIO_DSI_0_PHY_DSIPHY_DLN0_DEBUG_BUS1_ADDR                                     (DSI_0_PHY_DSIPHY_DLN0_DSIPHY_DLN0_DSI_DATALN_REG_BASE      + 0x00000060)
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_DEBUG_BUS1_RMSK                                           0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_DEBUG_BUS1_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN0_DEBUG_BUS1_ADDR, HWIO_DSI_0_PHY_DSIPHY_DLN0_DEBUG_BUS1_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_DEBUG_BUS1_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN0_DEBUG_BUS1_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_DEBUG_BUS1_DEBUG_BUS_15_8_BMSK                            0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_DEBUG_BUS1_DEBUG_BUS_15_8_SHFT                             0x0

#define HWIO_DSI_0_PHY_DSIPHY_DLN0_DEBUG_BUS2_ADDR                                     (DSI_0_PHY_DSIPHY_DLN0_DSIPHY_DLN0_DSI_DATALN_REG_BASE      + 0x00000064)
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_DEBUG_BUS2_RMSK                                           0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_DEBUG_BUS2_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN0_DEBUG_BUS2_ADDR, HWIO_DSI_0_PHY_DSIPHY_DLN0_DEBUG_BUS2_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_DEBUG_BUS2_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN0_DEBUG_BUS2_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_DEBUG_BUS2_DEBUG_BUS_23_16_BMSK                           0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_DEBUG_BUS2_DEBUG_BUS_23_16_SHFT                            0x0

#define HWIO_DSI_0_PHY_DSIPHY_DLN0_DEBUG_BUS3_ADDR                                     (DSI_0_PHY_DSIPHY_DLN0_DSIPHY_DLN0_DSI_DATALN_REG_BASE      + 0x00000068)
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_DEBUG_BUS3_RMSK                                           0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_DEBUG_BUS3_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN0_DEBUG_BUS3_ADDR, HWIO_DSI_0_PHY_DSIPHY_DLN0_DEBUG_BUS3_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_DEBUG_BUS3_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN0_DEBUG_BUS3_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_DEBUG_BUS3_DEBUG_BUS_31_24_BMSK                           0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN0_DEBUG_BUS3_DEBUG_BUS_31_24_SHFT                            0x0

/*----------------------------------------------------------------------------
 * MODULE: DSI_0_PHY_DSIPHY_DLN1_DSIPHY_DLN1_DSI_DATALN
 *--------------------------------------------------------------------------*/

#define DSI_0_PHY_DSIPHY_DLN1_DSIPHY_DLN1_DSI_DATALN_REG_BASE                          (MDSS_BASE      + 0x00094680)

#define HWIO_DSI_0_PHY_DSIPHY_DLN1_CFG0_ADDR                                           (DSI_0_PHY_DSIPHY_DLN1_DSIPHY_DLN1_DSI_DATALN_REG_BASE      + 0x00000000)
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_CFG0_RMSK                                                 0x3f
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_CFG0_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN1_CFG0_ADDR, HWIO_DSI_0_PHY_DSIPHY_DLN1_CFG0_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_CFG0_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN1_CFG0_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_CFG0_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_DLN1_CFG0_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_CFG0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_DLN1_CFG0_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_DLN1_CFG0_IN)
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_CFG0_DSIPHY_HSTX_PGM_DLY_5_4_BMSK                         0x30
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_CFG0_DSIPHY_HSTX_PGM_DLY_5_4_SHFT                          0x4
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_CFG0_DSIPHY_HSTX_PGM_DLY_3_0_BMSK                          0xf
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_CFG0_DSIPHY_HSTX_PGM_DLY_3_0_SHFT                          0x0

#define HWIO_DSI_0_PHY_DSIPHY_DLN1_CFG1_ADDR                                           (DSI_0_PHY_DSIPHY_DLN1_DSIPHY_DLN1_DSI_DATALN_REG_BASE      + 0x00000004)
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_CFG1_RMSK                                                 0x1f
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_CFG1_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN1_CFG1_ADDR, HWIO_DSI_0_PHY_DSIPHY_DLN1_CFG1_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_CFG1_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN1_CFG1_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_CFG1_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_DLN1_CFG1_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_CFG1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_DLN1_CFG1_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_DLN1_CFG1_IN)
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_CFG1_PROGSEQ_EN_BMSK                                      0x10
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_CFG1_PROGSEQ_EN_SHFT                                       0x4
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_CFG1_DSIPHY_HSTX_SLEW_RATE_BMSK                            0xf
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_CFG1_DSIPHY_HSTX_SLEW_RATE_SHFT                            0x0

#define HWIO_DSI_0_PHY_DSIPHY_DLN1_CFG2_ADDR                                           (DSI_0_PHY_DSIPHY_DLN1_DSIPHY_DLN1_DSI_DATALN_REG_BASE      + 0x00000008)
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_CFG2_RMSK                                                 0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_CFG2_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN1_CFG2_ADDR, HWIO_DSI_0_PHY_DSIPHY_DLN1_CFG2_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_CFG2_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN1_CFG2_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_CFG2_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_DLN1_CFG2_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_CFG2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_DLN1_CFG2_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_DLN1_CFG2_IN)
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_CFG2_IS_CKLANE_BMSK                                       0x80
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_CFG2_IS_CKLANE_SHFT                                        0x7
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_CFG2_CLK_PATTERN_HSTX_INV_BMSK                            0x40
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_CFG2_CLK_PATTERN_HSTX_INV_SHFT                             0x6
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_CFG2_TEST_MODE_LPTX_DATAN_BMSK                            0x20
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_CFG2_TEST_MODE_LPTX_DATAN_SHFT                             0x5
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_CFG2_TEST_MODE_LPTX_DATAP_BMSK                            0x10
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_CFG2_TEST_MODE_LPTX_DATAP_SHFT                             0x4
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_CFG2_DSIPHY_LPRX_DLY_BMSK                                  0xf
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_CFG2_DSIPHY_LPRX_DLY_SHFT                                  0x0

#define HWIO_DSI_0_PHY_DSIPHY_DLN1_TEST_DATAPATH_ADDR                                  (DSI_0_PHY_DSIPHY_DLN1_DSIPHY_DLN1_DSI_DATALN_REG_BASE      + 0x0000000c)
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_TEST_DATAPATH_RMSK                                        0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_TEST_DATAPATH_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN1_TEST_DATAPATH_ADDR, HWIO_DSI_0_PHY_DSIPHY_DLN1_TEST_DATAPATH_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_TEST_DATAPATH_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN1_TEST_DATAPATH_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_TEST_DATAPATH_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_DLN1_TEST_DATAPATH_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_TEST_DATAPATH_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_DLN1_TEST_DATAPATH_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_DLN1_TEST_DATAPATH_IN)
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_TEST_DATAPATH_TEST_MODE_LPTX_EN_BMSK                      0x80
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_TEST_DATAPATH_TEST_MODE_LPTX_EN_SHFT                       0x7
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_TEST_DATAPATH_ANE_BYPASS_EN_BMSK                          0x40
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_TEST_DATAPATH_ANE_BYPASS_EN_SHFT                           0x6
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_TEST_DATAPATH_TEST_MODE_HSTX_DATA_BMSK                    0x20
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_TEST_DATAPATH_TEST_MODE_HSTX_DATA_SHFT                     0x5
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_TEST_DATAPATH_DMUX_LP_SEL_BMSK                            0x18
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_TEST_DATAPATH_DMUX_LP_SEL_SHFT                             0x3
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_TEST_DATAPATH_DMUX_HS_SEL_BMSK                             0x4
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_TEST_DATAPATH_DMUX_HS_SEL_SHFT                             0x2
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_TEST_DATAPATH_TEST_MODE_HSTX_EN_BMSK                       0x2
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_TEST_DATAPATH_TEST_MODE_HSTX_EN_SHFT                       0x1
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_TEST_DATAPATH_FORCE_TEST_MODE_BMSK                         0x1
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_TEST_DATAPATH_FORCE_TEST_MODE_SHFT                         0x0

#define HWIO_DSI_0_PHY_DSIPHY_DLN1_PIN_SWAP_ADDR                                       (DSI_0_PHY_DSIPHY_DLN1_DSIPHY_DLN1_DSI_DATALN_REG_BASE      + 0x00000010)
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_PIN_SWAP_RMSK                                              0xf
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_PIN_SWAP_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN1_PIN_SWAP_ADDR, HWIO_DSI_0_PHY_DSIPHY_DLN1_PIN_SWAP_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_PIN_SWAP_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN1_PIN_SWAP_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_PIN_SWAP_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_DLN1_PIN_SWAP_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_PIN_SWAP_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_DLN1_PIN_SWAP_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_DLN1_PIN_SWAP_IN)
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_PIN_SWAP_ABC_SWAP_BMSK                                     0xe
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_PIN_SWAP_ABC_SWAP_SHFT                                     0x1
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_PIN_SWAP_PN_SWAP_BMSK                                      0x1
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_PIN_SWAP_PN_SWAP_SHFT                                      0x0

#define HWIO_DSI_0_PHY_DSIPHY_DLN1_LPRX_CTRL_ADDR                                      (DSI_0_PHY_DSIPHY_DLN1_DSIPHY_DLN1_DSI_DATALN_REG_BASE      + 0x00000014)
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_LPRX_CTRL_RMSK                                             0x7
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_LPRX_CTRL_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN1_LPRX_CTRL_ADDR, HWIO_DSI_0_PHY_DSIPHY_DLN1_LPRX_CTRL_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_LPRX_CTRL_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN1_LPRX_CTRL_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_LPRX_CTRL_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_DLN1_LPRX_CTRL_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_LPRX_CTRL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_DLN1_LPRX_CTRL_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_DLN1_LPRX_CTRL_IN)
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_LPRX_CTRL_HS_CONTENTION_DIS_BMSK                           0x4
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_LPRX_CTRL_HS_CONTENTION_DIS_SHFT                           0x2
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_LPRX_CTRL_DSIPHY_LPRX_EN_BMSK                              0x2
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_LPRX_CTRL_DSIPHY_LPRX_EN_SHFT                              0x1
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_LPRX_CTRL_DSIPHY_CDRX_EN_BMSK                              0x1
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_LPRX_CTRL_DSIPHY_CDRX_EN_SHFT                              0x0

#define HWIO_DSI_0_PHY_DSIPHY_DLN1_TX_DCTRL_ADDR                                       (DSI_0_PHY_DSIPHY_DLN1_DSIPHY_DLN1_DSI_DATALN_REG_BASE      + 0x00000018)
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_TX_DCTRL_RMSK                                             0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_TX_DCTRL_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN1_TX_DCTRL_ADDR, HWIO_DSI_0_PHY_DSIPHY_DLN1_TX_DCTRL_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_TX_DCTRL_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN1_TX_DCTRL_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_TX_DCTRL_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_DLN1_TX_DCTRL_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_TX_DCTRL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_DLN1_TX_DCTRL_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_DLN1_TX_DCTRL_IN)
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_TX_DCTRL_DCTRL_BMSK                                       0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_TX_DCTRL_DCTRL_SHFT                                        0x0

#define HWIO_DSI_0_PHY_DSIPHY_DLN1_BIST_CTRL0_ADDR                                     (DSI_0_PHY_DSIPHY_DLN1_DSIPHY_DLN1_DSI_DATALN_REG_BASE      + 0x0000001c)
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_BIST_CTRL0_RMSK                                           0x3f
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_BIST_CTRL0_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN1_BIST_CTRL0_ADDR, HWIO_DSI_0_PHY_DSIPHY_DLN1_BIST_CTRL0_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_BIST_CTRL0_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN1_BIST_CTRL0_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_BIST_CTRL0_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_DLN1_BIST_CTRL0_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_BIST_CTRL0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_DLN1_BIST_CTRL0_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_DLN1_BIST_CTRL0_IN)
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_BIST_CTRL0_LP_BISTCHK_ENABLE_BMSK                         0x20
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_BIST_CTRL0_LP_BISTCHK_ENABLE_SHFT                          0x5
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_BIST_CTRL0_BISTCHK_CAPTURE_BMSK                           0x10
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_BIST_CTRL0_BISTCHK_CAPTURE_SHFT                            0x4
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_BIST_CTRL0_BISTCHK_CLR_ERROR_CNT_BMSK                      0x8
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_BIST_CTRL0_BISTCHK_CLR_ERROR_CNT_SHFT                      0x3
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_BIST_CTRL0_BISTCHK_MODE_BMSK                               0x4
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_BIST_CTRL0_BISTCHK_MODE_SHFT                               0x2
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_BIST_CTRL0_BISTCHK_CROSS_BMSK                              0x2
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_BIST_CTRL0_BISTCHK_CROSS_SHFT                              0x1
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_BIST_CTRL0_BISTCHK_ENABLE_BMSK                             0x1
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_BIST_CTRL0_BISTCHK_ENABLE_SHFT                             0x0

#define HWIO_DSI_0_PHY_DSIPHY_DLN1_BIST_CTRL1_ADDR                                     (DSI_0_PHY_DSIPHY_DLN1_DSIPHY_DLN1_DSI_DATALN_REG_BASE      + 0x00000020)
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_BIST_CTRL1_RMSK                                           0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_BIST_CTRL1_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN1_BIST_CTRL1_ADDR, HWIO_DSI_0_PHY_DSIPHY_DLN1_BIST_CTRL1_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_BIST_CTRL1_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN1_BIST_CTRL1_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_BIST_CTRL1_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_DLN1_BIST_CTRL1_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_BIST_CTRL1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_DLN1_BIST_CTRL1_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_DLN1_BIST_CTRL1_IN)
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_BIST_CTRL1_BIST_WORD_CNT_7_0_BMSK                         0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_BIST_CTRL1_BIST_WORD_CNT_7_0_SHFT                          0x0

#define HWIO_DSI_0_PHY_DSIPHY_DLN1_BIST_CTRL2_ADDR                                     (DSI_0_PHY_DSIPHY_DLN1_DSIPHY_DLN1_DSI_DATALN_REG_BASE      + 0x00000024)
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_BIST_CTRL2_RMSK                                           0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_BIST_CTRL2_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN1_BIST_CTRL2_ADDR, HWIO_DSI_0_PHY_DSIPHY_DLN1_BIST_CTRL2_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_BIST_CTRL2_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN1_BIST_CTRL2_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_BIST_CTRL2_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_DLN1_BIST_CTRL2_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_BIST_CTRL2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_DLN1_BIST_CTRL2_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_DLN1_BIST_CTRL2_IN)
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_BIST_CTRL2_BIST_WORD_CNT_15_8_BMSK                        0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_BIST_CTRL2_BIST_WORD_CNT_15_8_SHFT                         0x0

#define HWIO_DSI_0_PHY_DSIPHY_DLN1_MISR_CTRL_ADDR                                      (DSI_0_PHY_DSIPHY_DLN1_DSIPHY_DLN1_DSI_DATALN_REG_BASE      + 0x00000028)
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_MISR_CTRL_RMSK                                            0x1f
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_MISR_CTRL_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN1_MISR_CTRL_ADDR, HWIO_DSI_0_PHY_DSIPHY_DLN1_MISR_CTRL_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_MISR_CTRL_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN1_MISR_CTRL_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_MISR_CTRL_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_DLN1_MISR_CTRL_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_MISR_CTRL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_DLN1_MISR_CTRL_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_DLN1_MISR_CTRL_IN)
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_MISR_CTRL_DEBUG_CLK_DEBUG_SEL_BMSK                        0x1c
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_MISR_CTRL_DEBUG_CLK_DEBUG_SEL_SHFT                         0x2
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_MISR_CTRL_MISR_SIG_CLEAR_BMSK                              0x2
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_MISR_CTRL_MISR_SIG_CLEAR_SHFT                              0x1
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_MISR_CTRL_MISR_ENABLE_BMSK                                 0x1
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_MISR_CTRL_MISR_ENABLE_SHFT                                 0x0

#define HWIO_DSI_0_PHY_DSIPHY_DLN1_DBG_SEL_ADDR                                        (DSI_0_PHY_DSIPHY_DLN1_DSIPHY_DLN1_DSI_DATALN_REG_BASE      + 0x0000002c)
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_DBG_SEL_RMSK                                              0x7f
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_DBG_SEL_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN1_DBG_SEL_ADDR, HWIO_DSI_0_PHY_DSIPHY_DLN1_DBG_SEL_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_DBG_SEL_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN1_DBG_SEL_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_DBG_SEL_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_DLN1_DBG_SEL_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_DBG_SEL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_DLN1_DBG_SEL_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_DLN1_DBG_SEL_IN)
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_DBG_SEL_TEST_CLK_DEBUG_SEL_BMSK                           0x70
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_DBG_SEL_TEST_CLK_DEBUG_SEL_SHFT                            0x4
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_DBG_SEL_DEBUG_SEL_BMSK                                     0xf
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_DBG_SEL_DEBUG_SEL_SHFT                                     0x0

#define HWIO_DSI_0_PHY_DSIPHY_DLN1_SWI_DBG_BUS_CHECK_ADDR                              (DSI_0_PHY_DSIPHY_DLN1_DSIPHY_DLN1_DSI_DATALN_REG_BASE      + 0x00000030)
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_SWI_DBG_BUS_CHECK_RMSK                                    0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_SWI_DBG_BUS_CHECK_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN1_SWI_DBG_BUS_CHECK_ADDR, HWIO_DSI_0_PHY_DSIPHY_DLN1_SWI_DBG_BUS_CHECK_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_SWI_DBG_BUS_CHECK_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN1_SWI_DBG_BUS_CHECK_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_SWI_DBG_BUS_CHECK_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_DLN1_SWI_DBG_BUS_CHECK_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_SWI_DBG_BUS_CHECK_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_DLN1_SWI_DBG_BUS_CHECK_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_DLN1_SWI_DBG_BUS_CHECK_IN)
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_SWI_DBG_BUS_CHECK_LANE_DEBUG_BUS_BMSK                     0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_SWI_DBG_BUS_CHECK_LANE_DEBUG_BUS_SHFT                      0x0

#define HWIO_DSI_0_PHY_DSIPHY_DLN1_BIST_STATUS0_ADDR                                   (DSI_0_PHY_DSIPHY_DLN1_DSIPHY_DLN1_DSI_DATALN_REG_BASE      + 0x00000034)
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_BIST_STATUS0_RMSK                                         0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_BIST_STATUS0_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN1_BIST_STATUS0_ADDR, HWIO_DSI_0_PHY_DSIPHY_DLN1_BIST_STATUS0_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_BIST_STATUS0_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN1_BIST_STATUS0_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_BIST_STATUS0_BISTCHK_ONE_BIT_ERROR_BMSK                   0x80
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_BIST_STATUS0_BISTCHK_ONE_BIT_ERROR_SHFT                    0x7
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_BIST_STATUS0_BISTCHK_SYNC_ERROR_BMSK                      0x40
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_BIST_STATUS0_BISTCHK_SYNC_ERROR_SHFT                       0x6
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_BIST_STATUS0_BISTCHK_SEED_ERROR_BMSK                      0x20
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_BIST_STATUS0_BISTCHK_SEED_ERROR_SHFT                       0x5
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_BIST_STATUS0_BISTCHK_PRBS_ERROR_BMSK                      0x10
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_BIST_STATUS0_BISTCHK_PRBS_ERROR_SHFT                       0x4
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_BIST_STATUS0_BISTCHK_STATUS_BMSK                           0xe
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_BIST_STATUS0_BISTCHK_STATUS_SHFT                           0x1
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_BIST_STATUS0_BISTCHK_DONE_BMSK                             0x1
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_BIST_STATUS0_BISTCHK_DONE_SHFT                             0x0

#define HWIO_DSI_0_PHY_DSIPHY_DLN1_BIST_STATUS1_ADDR                                   (DSI_0_PHY_DSIPHY_DLN1_DSIPHY_DLN1_DSI_DATALN_REG_BASE      + 0x00000038)
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_BIST_STATUS1_RMSK                                         0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_BIST_STATUS1_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN1_BIST_STATUS1_ADDR, HWIO_DSI_0_PHY_DSIPHY_DLN1_BIST_STATUS1_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_BIST_STATUS1_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN1_BIST_STATUS1_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_BIST_STATUS1_BISTCHK_ERROR_CNT_7_0_BMSK                   0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_BIST_STATUS1_BISTCHK_ERROR_CNT_7_0_SHFT                    0x0

#define HWIO_DSI_0_PHY_DSIPHY_DLN1_BIST_STATUS2_ADDR                                   (DSI_0_PHY_DSIPHY_DLN1_DSIPHY_DLN1_DSI_DATALN_REG_BASE      + 0x0000003c)
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_BIST_STATUS2_RMSK                                         0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_BIST_STATUS2_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN1_BIST_STATUS2_ADDR, HWIO_DSI_0_PHY_DSIPHY_DLN1_BIST_STATUS2_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_BIST_STATUS2_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN1_BIST_STATUS2_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_BIST_STATUS2_BISTCHK_ERROR_CNT_15_8_BMSK                  0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_BIST_STATUS2_BISTCHK_ERROR_CNT_15_8_SHFT                   0x0

#define HWIO_DSI_0_PHY_DSIPHY_DLN1_BIST_STATUS3_ADDR                                   (DSI_0_PHY_DSIPHY_DLN1_DSIPHY_DLN1_DSI_DATALN_REG_BASE      + 0x00000040)
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_BIST_STATUS3_RMSK                                         0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_BIST_STATUS3_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN1_BIST_STATUS3_ADDR, HWIO_DSI_0_PHY_DSIPHY_DLN1_BIST_STATUS3_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_BIST_STATUS3_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN1_BIST_STATUS3_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_BIST_STATUS3_BISTCHK_WORD_CNT_7_0_BMSK                    0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_BIST_STATUS3_BISTCHK_WORD_CNT_7_0_SHFT                     0x0

#define HWIO_DSI_0_PHY_DSIPHY_DLN1_BIST_STATUS4_ADDR                                   (DSI_0_PHY_DSIPHY_DLN1_DSIPHY_DLN1_DSI_DATALN_REG_BASE      + 0x00000044)
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_BIST_STATUS4_RMSK                                         0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_BIST_STATUS4_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN1_BIST_STATUS4_ADDR, HWIO_DSI_0_PHY_DSIPHY_DLN1_BIST_STATUS4_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_BIST_STATUS4_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN1_BIST_STATUS4_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_BIST_STATUS4_BISTCHK_WORD_CNT_15_8_BMSK                   0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_BIST_STATUS4_BISTCHK_WORD_CNT_15_8_SHFT                    0x0

#define HWIO_DSI_0_PHY_DSIPHY_DLN1_LP_BIST_STATUS0_ADDR                                (DSI_0_PHY_DSIPHY_DLN1_DSIPHY_DLN1_DSI_DATALN_REG_BASE      + 0x00000048)
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_LP_BIST_STATUS0_RMSK                                       0x3
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_LP_BIST_STATUS0_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN1_LP_BIST_STATUS0_ADDR, HWIO_DSI_0_PHY_DSIPHY_DLN1_LP_BIST_STATUS0_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_LP_BIST_STATUS0_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN1_LP_BIST_STATUS0_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_LP_BIST_STATUS0_LP_BISTCHK_PRBS_ERROR_BMSK                 0x2
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_LP_BIST_STATUS0_LP_BISTCHK_PRBS_ERROR_SHFT                 0x1
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_LP_BIST_STATUS0_LP_BISTCHK_DONE_BMSK                       0x1
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_LP_BIST_STATUS0_LP_BISTCHK_DONE_SHFT                       0x0

#define HWIO_DSI_0_PHY_DSIPHY_DLN1_LP_BIST_STATUS1_ADDR                                (DSI_0_PHY_DSIPHY_DLN1_DSIPHY_DLN1_DSI_DATALN_REG_BASE      + 0x0000004c)
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_LP_BIST_STATUS1_RMSK                                      0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_LP_BIST_STATUS1_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN1_LP_BIST_STATUS1_ADDR, HWIO_DSI_0_PHY_DSIPHY_DLN1_LP_BIST_STATUS1_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_LP_BIST_STATUS1_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN1_LP_BIST_STATUS1_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_LP_BIST_STATUS1_LP_BISTCHK_ERROR_CNT_BMSK                 0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_LP_BIST_STATUS1_LP_BISTCHK_ERROR_CNT_SHFT                  0x0

#define HWIO_DSI_0_PHY_DSIPHY_DLN1_LP_BIST_STATUS2_ADDR                                (DSI_0_PHY_DSIPHY_DLN1_DSIPHY_DLN1_DSI_DATALN_REG_BASE      + 0x00000050)
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_LP_BIST_STATUS2_RMSK                                      0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_LP_BIST_STATUS2_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN1_LP_BIST_STATUS2_ADDR, HWIO_DSI_0_PHY_DSIPHY_DLN1_LP_BIST_STATUS2_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_LP_BIST_STATUS2_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN1_LP_BIST_STATUS2_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_LP_BIST_STATUS2_LP_BISTCHK_WORD_CNT_BMSK                  0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_LP_BIST_STATUS2_LP_BISTCHK_WORD_CNT_SHFT                   0x0

#define HWIO_DSI_0_PHY_DSIPHY_DLN1_MISR_STATUS0_ADDR                                   (DSI_0_PHY_DSIPHY_DLN1_DSIPHY_DLN1_DSI_DATALN_REG_BASE      + 0x00000054)
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_MISR_STATUS0_RMSK                                         0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_MISR_STATUS0_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN1_MISR_STATUS0_ADDR, HWIO_DSI_0_PHY_DSIPHY_DLN1_MISR_STATUS0_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_MISR_STATUS0_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN1_MISR_STATUS0_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_MISR_STATUS0_BISTCHK_MISR_SIG_7_0_BMSK                    0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_MISR_STATUS0_BISTCHK_MISR_SIG_7_0_SHFT                     0x0

#define HWIO_DSI_0_PHY_DSIPHY_DLN1_MISR_STATUS1_ADDR                                   (DSI_0_PHY_DSIPHY_DLN1_DSIPHY_DLN1_DSI_DATALN_REG_BASE      + 0x00000058)
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_MISR_STATUS1_RMSK                                         0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_MISR_STATUS1_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN1_MISR_STATUS1_ADDR, HWIO_DSI_0_PHY_DSIPHY_DLN1_MISR_STATUS1_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_MISR_STATUS1_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN1_MISR_STATUS1_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_MISR_STATUS1_BISTCHK_MISR_SIG_15_8_BMSK                   0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_MISR_STATUS1_BISTCHK_MISR_SIG_15_8_SHFT                    0x0

#define HWIO_DSI_0_PHY_DSIPHY_DLN1_DEBUG_BUS0_ADDR                                     (DSI_0_PHY_DSIPHY_DLN1_DSIPHY_DLN1_DSI_DATALN_REG_BASE      + 0x0000005c)
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_DEBUG_BUS0_RMSK                                           0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_DEBUG_BUS0_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN1_DEBUG_BUS0_ADDR, HWIO_DSI_0_PHY_DSIPHY_DLN1_DEBUG_BUS0_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_DEBUG_BUS0_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN1_DEBUG_BUS0_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_DEBUG_BUS0_DEBUG_BUS_7_0_BMSK                             0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_DEBUG_BUS0_DEBUG_BUS_7_0_SHFT                              0x0

#define HWIO_DSI_0_PHY_DSIPHY_DLN1_DEBUG_BUS1_ADDR                                     (DSI_0_PHY_DSIPHY_DLN1_DSIPHY_DLN1_DSI_DATALN_REG_BASE      + 0x00000060)
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_DEBUG_BUS1_RMSK                                           0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_DEBUG_BUS1_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN1_DEBUG_BUS1_ADDR, HWIO_DSI_0_PHY_DSIPHY_DLN1_DEBUG_BUS1_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_DEBUG_BUS1_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN1_DEBUG_BUS1_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_DEBUG_BUS1_DEBUG_BUS_15_8_BMSK                            0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_DEBUG_BUS1_DEBUG_BUS_15_8_SHFT                             0x0

#define HWIO_DSI_0_PHY_DSIPHY_DLN1_DEBUG_BUS2_ADDR                                     (DSI_0_PHY_DSIPHY_DLN1_DSIPHY_DLN1_DSI_DATALN_REG_BASE      + 0x00000064)
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_DEBUG_BUS2_RMSK                                           0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_DEBUG_BUS2_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN1_DEBUG_BUS2_ADDR, HWIO_DSI_0_PHY_DSIPHY_DLN1_DEBUG_BUS2_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_DEBUG_BUS2_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN1_DEBUG_BUS2_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_DEBUG_BUS2_DEBUG_BUS_23_16_BMSK                           0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_DEBUG_BUS2_DEBUG_BUS_23_16_SHFT                            0x0

#define HWIO_DSI_0_PHY_DSIPHY_DLN1_DEBUG_BUS3_ADDR                                     (DSI_0_PHY_DSIPHY_DLN1_DSIPHY_DLN1_DSI_DATALN_REG_BASE      + 0x00000068)
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_DEBUG_BUS3_RMSK                                           0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_DEBUG_BUS3_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN1_DEBUG_BUS3_ADDR, HWIO_DSI_0_PHY_DSIPHY_DLN1_DEBUG_BUS3_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_DEBUG_BUS3_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN1_DEBUG_BUS3_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_DEBUG_BUS3_DEBUG_BUS_31_24_BMSK                           0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN1_DEBUG_BUS3_DEBUG_BUS_31_24_SHFT                            0x0

/*----------------------------------------------------------------------------
 * MODULE: DSI_0_PHY_DSIPHY_DLN2_DSIPHY_DLN2_DSI_DATALN
 *--------------------------------------------------------------------------*/

#define DSI_0_PHY_DSIPHY_DLN2_DSIPHY_DLN2_DSI_DATALN_REG_BASE                          (MDSS_BASE      + 0x00094700)

#define HWIO_DSI_0_PHY_DSIPHY_DLN2_CFG0_ADDR                                           (DSI_0_PHY_DSIPHY_DLN2_DSIPHY_DLN2_DSI_DATALN_REG_BASE      + 0x00000000)
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_CFG0_RMSK                                                 0x3f
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_CFG0_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN2_CFG0_ADDR, HWIO_DSI_0_PHY_DSIPHY_DLN2_CFG0_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_CFG0_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN2_CFG0_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_CFG0_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_DLN2_CFG0_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_CFG0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_DLN2_CFG0_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_DLN2_CFG0_IN)
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_CFG0_DSIPHY_HSTX_PGM_DLY_5_4_BMSK                         0x30
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_CFG0_DSIPHY_HSTX_PGM_DLY_5_4_SHFT                          0x4
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_CFG0_DSIPHY_HSTX_PGM_DLY_3_0_BMSK                          0xf
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_CFG0_DSIPHY_HSTX_PGM_DLY_3_0_SHFT                          0x0

#define HWIO_DSI_0_PHY_DSIPHY_DLN2_CFG1_ADDR                                           (DSI_0_PHY_DSIPHY_DLN2_DSIPHY_DLN2_DSI_DATALN_REG_BASE      + 0x00000004)
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_CFG1_RMSK                                                 0x1f
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_CFG1_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN2_CFG1_ADDR, HWIO_DSI_0_PHY_DSIPHY_DLN2_CFG1_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_CFG1_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN2_CFG1_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_CFG1_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_DLN2_CFG1_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_CFG1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_DLN2_CFG1_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_DLN2_CFG1_IN)
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_CFG1_PROGSEQ_EN_BMSK                                      0x10
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_CFG1_PROGSEQ_EN_SHFT                                       0x4
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_CFG1_DSIPHY_HSTX_SLEW_RATE_BMSK                            0xf
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_CFG1_DSIPHY_HSTX_SLEW_RATE_SHFT                            0x0

#define HWIO_DSI_0_PHY_DSIPHY_DLN2_CFG2_ADDR                                           (DSI_0_PHY_DSIPHY_DLN2_DSIPHY_DLN2_DSI_DATALN_REG_BASE      + 0x00000008)
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_CFG2_RMSK                                                 0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_CFG2_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN2_CFG2_ADDR, HWIO_DSI_0_PHY_DSIPHY_DLN2_CFG2_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_CFG2_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN2_CFG2_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_CFG2_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_DLN2_CFG2_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_CFG2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_DLN2_CFG2_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_DLN2_CFG2_IN)
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_CFG2_IS_CKLANE_BMSK                                       0x80
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_CFG2_IS_CKLANE_SHFT                                        0x7
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_CFG2_CLK_PATTERN_HSTX_INV_BMSK                            0x40
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_CFG2_CLK_PATTERN_HSTX_INV_SHFT                             0x6
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_CFG2_TEST_MODE_LPTX_DATAN_BMSK                            0x20
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_CFG2_TEST_MODE_LPTX_DATAN_SHFT                             0x5
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_CFG2_TEST_MODE_LPTX_DATAP_BMSK                            0x10
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_CFG2_TEST_MODE_LPTX_DATAP_SHFT                             0x4
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_CFG2_DSIPHY_LPRX_DLY_BMSK                                  0xf
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_CFG2_DSIPHY_LPRX_DLY_SHFT                                  0x0

#define HWIO_DSI_0_PHY_DSIPHY_DLN2_TEST_DATAPATH_ADDR                                  (DSI_0_PHY_DSIPHY_DLN2_DSIPHY_DLN2_DSI_DATALN_REG_BASE      + 0x0000000c)
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_TEST_DATAPATH_RMSK                                        0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_TEST_DATAPATH_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN2_TEST_DATAPATH_ADDR, HWIO_DSI_0_PHY_DSIPHY_DLN2_TEST_DATAPATH_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_TEST_DATAPATH_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN2_TEST_DATAPATH_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_TEST_DATAPATH_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_DLN2_TEST_DATAPATH_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_TEST_DATAPATH_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_DLN2_TEST_DATAPATH_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_DLN2_TEST_DATAPATH_IN)
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_TEST_DATAPATH_TEST_MODE_LPTX_EN_BMSK                      0x80
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_TEST_DATAPATH_TEST_MODE_LPTX_EN_SHFT                       0x7
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_TEST_DATAPATH_ANE_BYPASS_EN_BMSK                          0x40
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_TEST_DATAPATH_ANE_BYPASS_EN_SHFT                           0x6
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_TEST_DATAPATH_TEST_MODE_HSTX_DATA_BMSK                    0x20
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_TEST_DATAPATH_TEST_MODE_HSTX_DATA_SHFT                     0x5
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_TEST_DATAPATH_DMUX_LP_SEL_BMSK                            0x18
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_TEST_DATAPATH_DMUX_LP_SEL_SHFT                             0x3
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_TEST_DATAPATH_DMUX_HS_SEL_BMSK                             0x4
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_TEST_DATAPATH_DMUX_HS_SEL_SHFT                             0x2
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_TEST_DATAPATH_TEST_MODE_HSTX_EN_BMSK                       0x2
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_TEST_DATAPATH_TEST_MODE_HSTX_EN_SHFT                       0x1
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_TEST_DATAPATH_FORCE_TEST_MODE_BMSK                         0x1
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_TEST_DATAPATH_FORCE_TEST_MODE_SHFT                         0x0

#define HWIO_DSI_0_PHY_DSIPHY_DLN2_PIN_SWAP_ADDR                                       (DSI_0_PHY_DSIPHY_DLN2_DSIPHY_DLN2_DSI_DATALN_REG_BASE      + 0x00000010)
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_PIN_SWAP_RMSK                                              0xf
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_PIN_SWAP_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN2_PIN_SWAP_ADDR, HWIO_DSI_0_PHY_DSIPHY_DLN2_PIN_SWAP_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_PIN_SWAP_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN2_PIN_SWAP_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_PIN_SWAP_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_DLN2_PIN_SWAP_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_PIN_SWAP_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_DLN2_PIN_SWAP_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_DLN2_PIN_SWAP_IN)
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_PIN_SWAP_ABC_SWAP_BMSK                                     0xe
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_PIN_SWAP_ABC_SWAP_SHFT                                     0x1
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_PIN_SWAP_PN_SWAP_BMSK                                      0x1
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_PIN_SWAP_PN_SWAP_SHFT                                      0x0

#define HWIO_DSI_0_PHY_DSIPHY_DLN2_LPRX_CTRL_ADDR                                      (DSI_0_PHY_DSIPHY_DLN2_DSIPHY_DLN2_DSI_DATALN_REG_BASE      + 0x00000014)
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_LPRX_CTRL_RMSK                                             0x7
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_LPRX_CTRL_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN2_LPRX_CTRL_ADDR, HWIO_DSI_0_PHY_DSIPHY_DLN2_LPRX_CTRL_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_LPRX_CTRL_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN2_LPRX_CTRL_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_LPRX_CTRL_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_DLN2_LPRX_CTRL_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_LPRX_CTRL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_DLN2_LPRX_CTRL_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_DLN2_LPRX_CTRL_IN)
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_LPRX_CTRL_HS_CONTENTION_DIS_BMSK                           0x4
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_LPRX_CTRL_HS_CONTENTION_DIS_SHFT                           0x2
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_LPRX_CTRL_DSIPHY_LPRX_EN_BMSK                              0x2
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_LPRX_CTRL_DSIPHY_LPRX_EN_SHFT                              0x1
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_LPRX_CTRL_DSIPHY_CDRX_EN_BMSK                              0x1
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_LPRX_CTRL_DSIPHY_CDRX_EN_SHFT                              0x0

#define HWIO_DSI_0_PHY_DSIPHY_DLN2_TX_DCTRL_ADDR                                       (DSI_0_PHY_DSIPHY_DLN2_DSIPHY_DLN2_DSI_DATALN_REG_BASE      + 0x00000018)
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_TX_DCTRL_RMSK                                             0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_TX_DCTRL_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN2_TX_DCTRL_ADDR, HWIO_DSI_0_PHY_DSIPHY_DLN2_TX_DCTRL_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_TX_DCTRL_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN2_TX_DCTRL_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_TX_DCTRL_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_DLN2_TX_DCTRL_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_TX_DCTRL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_DLN2_TX_DCTRL_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_DLN2_TX_DCTRL_IN)
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_TX_DCTRL_DCTRL_BMSK                                       0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_TX_DCTRL_DCTRL_SHFT                                        0x0

#define HWIO_DSI_0_PHY_DSIPHY_DLN2_BIST_CTRL0_ADDR                                     (DSI_0_PHY_DSIPHY_DLN2_DSIPHY_DLN2_DSI_DATALN_REG_BASE      + 0x0000001c)
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_BIST_CTRL0_RMSK                                           0x3f
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_BIST_CTRL0_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN2_BIST_CTRL0_ADDR, HWIO_DSI_0_PHY_DSIPHY_DLN2_BIST_CTRL0_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_BIST_CTRL0_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN2_BIST_CTRL0_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_BIST_CTRL0_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_DLN2_BIST_CTRL0_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_BIST_CTRL0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_DLN2_BIST_CTRL0_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_DLN2_BIST_CTRL0_IN)
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_BIST_CTRL0_LP_BISTCHK_ENABLE_BMSK                         0x20
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_BIST_CTRL0_LP_BISTCHK_ENABLE_SHFT                          0x5
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_BIST_CTRL0_BISTCHK_CAPTURE_BMSK                           0x10
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_BIST_CTRL0_BISTCHK_CAPTURE_SHFT                            0x4
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_BIST_CTRL0_BISTCHK_CLR_ERROR_CNT_BMSK                      0x8
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_BIST_CTRL0_BISTCHK_CLR_ERROR_CNT_SHFT                      0x3
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_BIST_CTRL0_BISTCHK_MODE_BMSK                               0x4
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_BIST_CTRL0_BISTCHK_MODE_SHFT                               0x2
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_BIST_CTRL0_BISTCHK_CROSS_BMSK                              0x2
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_BIST_CTRL0_BISTCHK_CROSS_SHFT                              0x1
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_BIST_CTRL0_BISTCHK_ENABLE_BMSK                             0x1
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_BIST_CTRL0_BISTCHK_ENABLE_SHFT                             0x0

#define HWIO_DSI_0_PHY_DSIPHY_DLN2_BIST_CTRL1_ADDR                                     (DSI_0_PHY_DSIPHY_DLN2_DSIPHY_DLN2_DSI_DATALN_REG_BASE      + 0x00000020)
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_BIST_CTRL1_RMSK                                           0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_BIST_CTRL1_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN2_BIST_CTRL1_ADDR, HWIO_DSI_0_PHY_DSIPHY_DLN2_BIST_CTRL1_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_BIST_CTRL1_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN2_BIST_CTRL1_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_BIST_CTRL1_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_DLN2_BIST_CTRL1_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_BIST_CTRL1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_DLN2_BIST_CTRL1_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_DLN2_BIST_CTRL1_IN)
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_BIST_CTRL1_BIST_WORD_CNT_7_0_BMSK                         0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_BIST_CTRL1_BIST_WORD_CNT_7_0_SHFT                          0x0

#define HWIO_DSI_0_PHY_DSIPHY_DLN2_BIST_CTRL2_ADDR                                     (DSI_0_PHY_DSIPHY_DLN2_DSIPHY_DLN2_DSI_DATALN_REG_BASE      + 0x00000024)
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_BIST_CTRL2_RMSK                                           0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_BIST_CTRL2_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN2_BIST_CTRL2_ADDR, HWIO_DSI_0_PHY_DSIPHY_DLN2_BIST_CTRL2_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_BIST_CTRL2_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN2_BIST_CTRL2_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_BIST_CTRL2_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_DLN2_BIST_CTRL2_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_BIST_CTRL2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_DLN2_BIST_CTRL2_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_DLN2_BIST_CTRL2_IN)
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_BIST_CTRL2_BIST_WORD_CNT_15_8_BMSK                        0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_BIST_CTRL2_BIST_WORD_CNT_15_8_SHFT                         0x0

#define HWIO_DSI_0_PHY_DSIPHY_DLN2_MISR_CTRL_ADDR                                      (DSI_0_PHY_DSIPHY_DLN2_DSIPHY_DLN2_DSI_DATALN_REG_BASE      + 0x00000028)
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_MISR_CTRL_RMSK                                            0x1f
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_MISR_CTRL_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN2_MISR_CTRL_ADDR, HWIO_DSI_0_PHY_DSIPHY_DLN2_MISR_CTRL_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_MISR_CTRL_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN2_MISR_CTRL_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_MISR_CTRL_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_DLN2_MISR_CTRL_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_MISR_CTRL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_DLN2_MISR_CTRL_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_DLN2_MISR_CTRL_IN)
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_MISR_CTRL_DEBUG_CLK_DEBUG_SEL_BMSK                        0x1c
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_MISR_CTRL_DEBUG_CLK_DEBUG_SEL_SHFT                         0x2
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_MISR_CTRL_MISR_SIG_CLEAR_BMSK                              0x2
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_MISR_CTRL_MISR_SIG_CLEAR_SHFT                              0x1
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_MISR_CTRL_MISR_ENABLE_BMSK                                 0x1
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_MISR_CTRL_MISR_ENABLE_SHFT                                 0x0

#define HWIO_DSI_0_PHY_DSIPHY_DLN2_DBG_SEL_ADDR                                        (DSI_0_PHY_DSIPHY_DLN2_DSIPHY_DLN2_DSI_DATALN_REG_BASE      + 0x0000002c)
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_DBG_SEL_RMSK                                              0x7f
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_DBG_SEL_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN2_DBG_SEL_ADDR, HWIO_DSI_0_PHY_DSIPHY_DLN2_DBG_SEL_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_DBG_SEL_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN2_DBG_SEL_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_DBG_SEL_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_DLN2_DBG_SEL_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_DBG_SEL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_DLN2_DBG_SEL_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_DLN2_DBG_SEL_IN)
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_DBG_SEL_TEST_CLK_DEBUG_SEL_BMSK                           0x70
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_DBG_SEL_TEST_CLK_DEBUG_SEL_SHFT                            0x4
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_DBG_SEL_DEBUG_SEL_BMSK                                     0xf
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_DBG_SEL_DEBUG_SEL_SHFT                                     0x0

#define HWIO_DSI_0_PHY_DSIPHY_DLN2_SWI_DBG_BUS_CHECK_ADDR                              (DSI_0_PHY_DSIPHY_DLN2_DSIPHY_DLN2_DSI_DATALN_REG_BASE      + 0x00000030)
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_SWI_DBG_BUS_CHECK_RMSK                                    0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_SWI_DBG_BUS_CHECK_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN2_SWI_DBG_BUS_CHECK_ADDR, HWIO_DSI_0_PHY_DSIPHY_DLN2_SWI_DBG_BUS_CHECK_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_SWI_DBG_BUS_CHECK_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN2_SWI_DBG_BUS_CHECK_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_SWI_DBG_BUS_CHECK_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_DLN2_SWI_DBG_BUS_CHECK_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_SWI_DBG_BUS_CHECK_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_DLN2_SWI_DBG_BUS_CHECK_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_DLN2_SWI_DBG_BUS_CHECK_IN)
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_SWI_DBG_BUS_CHECK_LANE_DEBUG_BUS_BMSK                     0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_SWI_DBG_BUS_CHECK_LANE_DEBUG_BUS_SHFT                      0x0

#define HWIO_DSI_0_PHY_DSIPHY_DLN2_BIST_STATUS0_ADDR                                   (DSI_0_PHY_DSIPHY_DLN2_DSIPHY_DLN2_DSI_DATALN_REG_BASE      + 0x00000034)
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_BIST_STATUS0_RMSK                                         0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_BIST_STATUS0_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN2_BIST_STATUS0_ADDR, HWIO_DSI_0_PHY_DSIPHY_DLN2_BIST_STATUS0_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_BIST_STATUS0_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN2_BIST_STATUS0_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_BIST_STATUS0_BISTCHK_ONE_BIT_ERROR_BMSK                   0x80
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_BIST_STATUS0_BISTCHK_ONE_BIT_ERROR_SHFT                    0x7
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_BIST_STATUS0_BISTCHK_SYNC_ERROR_BMSK                      0x40
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_BIST_STATUS0_BISTCHK_SYNC_ERROR_SHFT                       0x6
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_BIST_STATUS0_BISTCHK_SEED_ERROR_BMSK                      0x20
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_BIST_STATUS0_BISTCHK_SEED_ERROR_SHFT                       0x5
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_BIST_STATUS0_BISTCHK_PRBS_ERROR_BMSK                      0x10
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_BIST_STATUS0_BISTCHK_PRBS_ERROR_SHFT                       0x4
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_BIST_STATUS0_BISTCHK_STATUS_BMSK                           0xe
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_BIST_STATUS0_BISTCHK_STATUS_SHFT                           0x1
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_BIST_STATUS0_BISTCHK_DONE_BMSK                             0x1
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_BIST_STATUS0_BISTCHK_DONE_SHFT                             0x0

#define HWIO_DSI_0_PHY_DSIPHY_DLN2_BIST_STATUS1_ADDR                                   (DSI_0_PHY_DSIPHY_DLN2_DSIPHY_DLN2_DSI_DATALN_REG_BASE      + 0x00000038)
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_BIST_STATUS1_RMSK                                         0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_BIST_STATUS1_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN2_BIST_STATUS1_ADDR, HWIO_DSI_0_PHY_DSIPHY_DLN2_BIST_STATUS1_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_BIST_STATUS1_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN2_BIST_STATUS1_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_BIST_STATUS1_BISTCHK_ERROR_CNT_7_0_BMSK                   0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_BIST_STATUS1_BISTCHK_ERROR_CNT_7_0_SHFT                    0x0

#define HWIO_DSI_0_PHY_DSIPHY_DLN2_BIST_STATUS2_ADDR                                   (DSI_0_PHY_DSIPHY_DLN2_DSIPHY_DLN2_DSI_DATALN_REG_BASE      + 0x0000003c)
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_BIST_STATUS2_RMSK                                         0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_BIST_STATUS2_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN2_BIST_STATUS2_ADDR, HWIO_DSI_0_PHY_DSIPHY_DLN2_BIST_STATUS2_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_BIST_STATUS2_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN2_BIST_STATUS2_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_BIST_STATUS2_BISTCHK_ERROR_CNT_15_8_BMSK                  0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_BIST_STATUS2_BISTCHK_ERROR_CNT_15_8_SHFT                   0x0

#define HWIO_DSI_0_PHY_DSIPHY_DLN2_BIST_STATUS3_ADDR                                   (DSI_0_PHY_DSIPHY_DLN2_DSIPHY_DLN2_DSI_DATALN_REG_BASE      + 0x00000040)
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_BIST_STATUS3_RMSK                                         0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_BIST_STATUS3_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN2_BIST_STATUS3_ADDR, HWIO_DSI_0_PHY_DSIPHY_DLN2_BIST_STATUS3_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_BIST_STATUS3_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN2_BIST_STATUS3_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_BIST_STATUS3_BISTCHK_WORD_CNT_7_0_BMSK                    0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_BIST_STATUS3_BISTCHK_WORD_CNT_7_0_SHFT                     0x0

#define HWIO_DSI_0_PHY_DSIPHY_DLN2_BIST_STATUS4_ADDR                                   (DSI_0_PHY_DSIPHY_DLN2_DSIPHY_DLN2_DSI_DATALN_REG_BASE      + 0x00000044)
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_BIST_STATUS4_RMSK                                         0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_BIST_STATUS4_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN2_BIST_STATUS4_ADDR, HWIO_DSI_0_PHY_DSIPHY_DLN2_BIST_STATUS4_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_BIST_STATUS4_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN2_BIST_STATUS4_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_BIST_STATUS4_BISTCHK_WORD_CNT_15_8_BMSK                   0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_BIST_STATUS4_BISTCHK_WORD_CNT_15_8_SHFT                    0x0

#define HWIO_DSI_0_PHY_DSIPHY_DLN2_LP_BIST_STATUS0_ADDR                                (DSI_0_PHY_DSIPHY_DLN2_DSIPHY_DLN2_DSI_DATALN_REG_BASE      + 0x00000048)
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_LP_BIST_STATUS0_RMSK                                       0x3
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_LP_BIST_STATUS0_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN2_LP_BIST_STATUS0_ADDR, HWIO_DSI_0_PHY_DSIPHY_DLN2_LP_BIST_STATUS0_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_LP_BIST_STATUS0_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN2_LP_BIST_STATUS0_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_LP_BIST_STATUS0_LP_BISTCHK_PRBS_ERROR_BMSK                 0x2
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_LP_BIST_STATUS0_LP_BISTCHK_PRBS_ERROR_SHFT                 0x1
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_LP_BIST_STATUS0_LP_BISTCHK_DONE_BMSK                       0x1
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_LP_BIST_STATUS0_LP_BISTCHK_DONE_SHFT                       0x0

#define HWIO_DSI_0_PHY_DSIPHY_DLN2_LP_BIST_STATUS1_ADDR                                (DSI_0_PHY_DSIPHY_DLN2_DSIPHY_DLN2_DSI_DATALN_REG_BASE      + 0x0000004c)
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_LP_BIST_STATUS1_RMSK                                      0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_LP_BIST_STATUS1_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN2_LP_BIST_STATUS1_ADDR, HWIO_DSI_0_PHY_DSIPHY_DLN2_LP_BIST_STATUS1_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_LP_BIST_STATUS1_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN2_LP_BIST_STATUS1_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_LP_BIST_STATUS1_LP_BISTCHK_ERROR_CNT_BMSK                 0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_LP_BIST_STATUS1_LP_BISTCHK_ERROR_CNT_SHFT                  0x0

#define HWIO_DSI_0_PHY_DSIPHY_DLN2_LP_BIST_STATUS2_ADDR                                (DSI_0_PHY_DSIPHY_DLN2_DSIPHY_DLN2_DSI_DATALN_REG_BASE      + 0x00000050)
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_LP_BIST_STATUS2_RMSK                                      0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_LP_BIST_STATUS2_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN2_LP_BIST_STATUS2_ADDR, HWIO_DSI_0_PHY_DSIPHY_DLN2_LP_BIST_STATUS2_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_LP_BIST_STATUS2_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN2_LP_BIST_STATUS2_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_LP_BIST_STATUS2_LP_BISTCHK_WORD_CNT_BMSK                  0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_LP_BIST_STATUS2_LP_BISTCHK_WORD_CNT_SHFT                   0x0

#define HWIO_DSI_0_PHY_DSIPHY_DLN2_MISR_STATUS0_ADDR                                   (DSI_0_PHY_DSIPHY_DLN2_DSIPHY_DLN2_DSI_DATALN_REG_BASE      + 0x00000054)
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_MISR_STATUS0_RMSK                                         0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_MISR_STATUS0_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN2_MISR_STATUS0_ADDR, HWIO_DSI_0_PHY_DSIPHY_DLN2_MISR_STATUS0_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_MISR_STATUS0_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN2_MISR_STATUS0_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_MISR_STATUS0_BISTCHK_MISR_SIG_7_0_BMSK                    0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_MISR_STATUS0_BISTCHK_MISR_SIG_7_0_SHFT                     0x0

#define HWIO_DSI_0_PHY_DSIPHY_DLN2_MISR_STATUS1_ADDR                                   (DSI_0_PHY_DSIPHY_DLN2_DSIPHY_DLN2_DSI_DATALN_REG_BASE      + 0x00000058)
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_MISR_STATUS1_RMSK                                         0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_MISR_STATUS1_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN2_MISR_STATUS1_ADDR, HWIO_DSI_0_PHY_DSIPHY_DLN2_MISR_STATUS1_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_MISR_STATUS1_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN2_MISR_STATUS1_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_MISR_STATUS1_BISTCHK_MISR_SIG_15_8_BMSK                   0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_MISR_STATUS1_BISTCHK_MISR_SIG_15_8_SHFT                    0x0

#define HWIO_DSI_0_PHY_DSIPHY_DLN2_DEBUG_BUS0_ADDR                                     (DSI_0_PHY_DSIPHY_DLN2_DSIPHY_DLN2_DSI_DATALN_REG_BASE      + 0x0000005c)
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_DEBUG_BUS0_RMSK                                           0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_DEBUG_BUS0_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN2_DEBUG_BUS0_ADDR, HWIO_DSI_0_PHY_DSIPHY_DLN2_DEBUG_BUS0_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_DEBUG_BUS0_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN2_DEBUG_BUS0_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_DEBUG_BUS0_DEBUG_BUS_7_0_BMSK                             0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_DEBUG_BUS0_DEBUG_BUS_7_0_SHFT                              0x0

#define HWIO_DSI_0_PHY_DSIPHY_DLN2_DEBUG_BUS1_ADDR                                     (DSI_0_PHY_DSIPHY_DLN2_DSIPHY_DLN2_DSI_DATALN_REG_BASE      + 0x00000060)
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_DEBUG_BUS1_RMSK                                           0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_DEBUG_BUS1_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN2_DEBUG_BUS1_ADDR, HWIO_DSI_0_PHY_DSIPHY_DLN2_DEBUG_BUS1_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_DEBUG_BUS1_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN2_DEBUG_BUS1_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_DEBUG_BUS1_DEBUG_BUS_15_8_BMSK                            0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_DEBUG_BUS1_DEBUG_BUS_15_8_SHFT                             0x0

#define HWIO_DSI_0_PHY_DSIPHY_DLN2_DEBUG_BUS2_ADDR                                     (DSI_0_PHY_DSIPHY_DLN2_DSIPHY_DLN2_DSI_DATALN_REG_BASE      + 0x00000064)
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_DEBUG_BUS2_RMSK                                           0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_DEBUG_BUS2_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN2_DEBUG_BUS2_ADDR, HWIO_DSI_0_PHY_DSIPHY_DLN2_DEBUG_BUS2_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_DEBUG_BUS2_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN2_DEBUG_BUS2_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_DEBUG_BUS2_DEBUG_BUS_23_16_BMSK                           0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_DEBUG_BUS2_DEBUG_BUS_23_16_SHFT                            0x0

#define HWIO_DSI_0_PHY_DSIPHY_DLN2_DEBUG_BUS3_ADDR                                     (DSI_0_PHY_DSIPHY_DLN2_DSIPHY_DLN2_DSI_DATALN_REG_BASE      + 0x00000068)
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_DEBUG_BUS3_RMSK                                           0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_DEBUG_BUS3_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN2_DEBUG_BUS3_ADDR, HWIO_DSI_0_PHY_DSIPHY_DLN2_DEBUG_BUS3_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_DEBUG_BUS3_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN2_DEBUG_BUS3_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_DEBUG_BUS3_DEBUG_BUS_31_24_BMSK                           0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN2_DEBUG_BUS3_DEBUG_BUS_31_24_SHFT                            0x0

/*----------------------------------------------------------------------------
 * MODULE: DSI_0_PHY_DSIPHY_DLN3_DSIPHY_DLN3_DSI_DATALN
 *--------------------------------------------------------------------------*/

#define DSI_0_PHY_DSIPHY_DLN3_DSIPHY_DLN3_DSI_DATALN_REG_BASE                          (MDSS_BASE      + 0x00094780)

#define HWIO_DSI_0_PHY_DSIPHY_DLN3_CFG0_ADDR                                           (DSI_0_PHY_DSIPHY_DLN3_DSIPHY_DLN3_DSI_DATALN_REG_BASE      + 0x00000000)
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_CFG0_RMSK                                                 0x3f
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_CFG0_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN3_CFG0_ADDR, HWIO_DSI_0_PHY_DSIPHY_DLN3_CFG0_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_CFG0_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN3_CFG0_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_CFG0_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_DLN3_CFG0_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_CFG0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_DLN3_CFG0_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_DLN3_CFG0_IN)
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_CFG0_DSIPHY_HSTX_PGM_DLY_5_4_BMSK                         0x30
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_CFG0_DSIPHY_HSTX_PGM_DLY_5_4_SHFT                          0x4
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_CFG0_DSIPHY_HSTX_PGM_DLY_3_0_BMSK                          0xf
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_CFG0_DSIPHY_HSTX_PGM_DLY_3_0_SHFT                          0x0

#define HWIO_DSI_0_PHY_DSIPHY_DLN3_CFG1_ADDR                                           (DSI_0_PHY_DSIPHY_DLN3_DSIPHY_DLN3_DSI_DATALN_REG_BASE      + 0x00000004)
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_CFG1_RMSK                                                 0x1f
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_CFG1_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN3_CFG1_ADDR, HWIO_DSI_0_PHY_DSIPHY_DLN3_CFG1_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_CFG1_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN3_CFG1_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_CFG1_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_DLN3_CFG1_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_CFG1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_DLN3_CFG1_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_DLN3_CFG1_IN)
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_CFG1_PROGSEQ_EN_BMSK                                      0x10
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_CFG1_PROGSEQ_EN_SHFT                                       0x4
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_CFG1_DSIPHY_HSTX_SLEW_RATE_BMSK                            0xf
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_CFG1_DSIPHY_HSTX_SLEW_RATE_SHFT                            0x0

#define HWIO_DSI_0_PHY_DSIPHY_DLN3_CFG2_ADDR                                           (DSI_0_PHY_DSIPHY_DLN3_DSIPHY_DLN3_DSI_DATALN_REG_BASE      + 0x00000008)
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_CFG2_RMSK                                                 0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_CFG2_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN3_CFG2_ADDR, HWIO_DSI_0_PHY_DSIPHY_DLN3_CFG2_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_CFG2_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN3_CFG2_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_CFG2_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_DLN3_CFG2_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_CFG2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_DLN3_CFG2_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_DLN3_CFG2_IN)
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_CFG2_IS_CKLANE_BMSK                                       0x80
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_CFG2_IS_CKLANE_SHFT                                        0x7
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_CFG2_CLK_PATTERN_HSTX_INV_BMSK                            0x40
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_CFG2_CLK_PATTERN_HSTX_INV_SHFT                             0x6
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_CFG2_TEST_MODE_LPTX_DATAN_BMSK                            0x20
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_CFG2_TEST_MODE_LPTX_DATAN_SHFT                             0x5
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_CFG2_TEST_MODE_LPTX_DATAP_BMSK                            0x10
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_CFG2_TEST_MODE_LPTX_DATAP_SHFT                             0x4
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_CFG2_DSIPHY_LPRX_DLY_BMSK                                  0xf
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_CFG2_DSIPHY_LPRX_DLY_SHFT                                  0x0

#define HWIO_DSI_0_PHY_DSIPHY_DLN3_TEST_DATAPATH_ADDR                                  (DSI_0_PHY_DSIPHY_DLN3_DSIPHY_DLN3_DSI_DATALN_REG_BASE      + 0x0000000c)
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_TEST_DATAPATH_RMSK                                        0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_TEST_DATAPATH_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN3_TEST_DATAPATH_ADDR, HWIO_DSI_0_PHY_DSIPHY_DLN3_TEST_DATAPATH_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_TEST_DATAPATH_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN3_TEST_DATAPATH_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_TEST_DATAPATH_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_DLN3_TEST_DATAPATH_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_TEST_DATAPATH_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_DLN3_TEST_DATAPATH_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_DLN3_TEST_DATAPATH_IN)
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_TEST_DATAPATH_TEST_MODE_LPTX_EN_BMSK                      0x80
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_TEST_DATAPATH_TEST_MODE_LPTX_EN_SHFT                       0x7
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_TEST_DATAPATH_ANE_BYPASS_EN_BMSK                          0x40
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_TEST_DATAPATH_ANE_BYPASS_EN_SHFT                           0x6
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_TEST_DATAPATH_TEST_MODE_HSTX_DATA_BMSK                    0x20
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_TEST_DATAPATH_TEST_MODE_HSTX_DATA_SHFT                     0x5
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_TEST_DATAPATH_DMUX_LP_SEL_BMSK                            0x18
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_TEST_DATAPATH_DMUX_LP_SEL_SHFT                             0x3
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_TEST_DATAPATH_DMUX_HS_SEL_BMSK                             0x4
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_TEST_DATAPATH_DMUX_HS_SEL_SHFT                             0x2
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_TEST_DATAPATH_TEST_MODE_HSTX_EN_BMSK                       0x2
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_TEST_DATAPATH_TEST_MODE_HSTX_EN_SHFT                       0x1
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_TEST_DATAPATH_FORCE_TEST_MODE_BMSK                         0x1
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_TEST_DATAPATH_FORCE_TEST_MODE_SHFT                         0x0

#define HWIO_DSI_0_PHY_DSIPHY_DLN3_PIN_SWAP_ADDR                                       (DSI_0_PHY_DSIPHY_DLN3_DSIPHY_DLN3_DSI_DATALN_REG_BASE      + 0x00000010)
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_PIN_SWAP_RMSK                                              0xf
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_PIN_SWAP_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN3_PIN_SWAP_ADDR, HWIO_DSI_0_PHY_DSIPHY_DLN3_PIN_SWAP_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_PIN_SWAP_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN3_PIN_SWAP_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_PIN_SWAP_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_DLN3_PIN_SWAP_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_PIN_SWAP_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_DLN3_PIN_SWAP_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_DLN3_PIN_SWAP_IN)
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_PIN_SWAP_ABC_SWAP_BMSK                                     0xe
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_PIN_SWAP_ABC_SWAP_SHFT                                     0x1
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_PIN_SWAP_PN_SWAP_BMSK                                      0x1
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_PIN_SWAP_PN_SWAP_SHFT                                      0x0

#define HWIO_DSI_0_PHY_DSIPHY_DLN3_LPRX_CTRL_ADDR                                      (DSI_0_PHY_DSIPHY_DLN3_DSIPHY_DLN3_DSI_DATALN_REG_BASE      + 0x00000014)
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_LPRX_CTRL_RMSK                                             0x7
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_LPRX_CTRL_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN3_LPRX_CTRL_ADDR, HWIO_DSI_0_PHY_DSIPHY_DLN3_LPRX_CTRL_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_LPRX_CTRL_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN3_LPRX_CTRL_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_LPRX_CTRL_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_DLN3_LPRX_CTRL_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_LPRX_CTRL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_DLN3_LPRX_CTRL_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_DLN3_LPRX_CTRL_IN)
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_LPRX_CTRL_HS_CONTENTION_DIS_BMSK                           0x4
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_LPRX_CTRL_HS_CONTENTION_DIS_SHFT                           0x2
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_LPRX_CTRL_DSIPHY_LPRX_EN_BMSK                              0x2
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_LPRX_CTRL_DSIPHY_LPRX_EN_SHFT                              0x1
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_LPRX_CTRL_DSIPHY_CDRX_EN_BMSK                              0x1
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_LPRX_CTRL_DSIPHY_CDRX_EN_SHFT                              0x0

#define HWIO_DSI_0_PHY_DSIPHY_DLN3_TX_DCTRL_ADDR                                       (DSI_0_PHY_DSIPHY_DLN3_DSIPHY_DLN3_DSI_DATALN_REG_BASE      + 0x00000018)
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_TX_DCTRL_RMSK                                             0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_TX_DCTRL_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN3_TX_DCTRL_ADDR, HWIO_DSI_0_PHY_DSIPHY_DLN3_TX_DCTRL_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_TX_DCTRL_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN3_TX_DCTRL_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_TX_DCTRL_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_DLN3_TX_DCTRL_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_TX_DCTRL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_DLN3_TX_DCTRL_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_DLN3_TX_DCTRL_IN)
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_TX_DCTRL_DCTRL_BMSK                                       0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_TX_DCTRL_DCTRL_SHFT                                        0x0

#define HWIO_DSI_0_PHY_DSIPHY_DLN3_BIST_CTRL0_ADDR                                     (DSI_0_PHY_DSIPHY_DLN3_DSIPHY_DLN3_DSI_DATALN_REG_BASE      + 0x0000001c)
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_BIST_CTRL0_RMSK                                           0x3f
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_BIST_CTRL0_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN3_BIST_CTRL0_ADDR, HWIO_DSI_0_PHY_DSIPHY_DLN3_BIST_CTRL0_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_BIST_CTRL0_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN3_BIST_CTRL0_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_BIST_CTRL0_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_DLN3_BIST_CTRL0_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_BIST_CTRL0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_DLN3_BIST_CTRL0_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_DLN3_BIST_CTRL0_IN)
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_BIST_CTRL0_LP_BISTCHK_ENABLE_BMSK                         0x20
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_BIST_CTRL0_LP_BISTCHK_ENABLE_SHFT                          0x5
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_BIST_CTRL0_BISTCHK_CAPTURE_BMSK                           0x10
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_BIST_CTRL0_BISTCHK_CAPTURE_SHFT                            0x4
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_BIST_CTRL0_BISTCHK_CLR_ERROR_CNT_BMSK                      0x8
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_BIST_CTRL0_BISTCHK_CLR_ERROR_CNT_SHFT                      0x3
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_BIST_CTRL0_BISTCHK_MODE_BMSK                               0x4
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_BIST_CTRL0_BISTCHK_MODE_SHFT                               0x2
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_BIST_CTRL0_BISTCHK_CROSS_BMSK                              0x2
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_BIST_CTRL0_BISTCHK_CROSS_SHFT                              0x1
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_BIST_CTRL0_BISTCHK_ENABLE_BMSK                             0x1
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_BIST_CTRL0_BISTCHK_ENABLE_SHFT                             0x0

#define HWIO_DSI_0_PHY_DSIPHY_DLN3_BIST_CTRL1_ADDR                                     (DSI_0_PHY_DSIPHY_DLN3_DSIPHY_DLN3_DSI_DATALN_REG_BASE      + 0x00000020)
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_BIST_CTRL1_RMSK                                           0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_BIST_CTRL1_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN3_BIST_CTRL1_ADDR, HWIO_DSI_0_PHY_DSIPHY_DLN3_BIST_CTRL1_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_BIST_CTRL1_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN3_BIST_CTRL1_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_BIST_CTRL1_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_DLN3_BIST_CTRL1_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_BIST_CTRL1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_DLN3_BIST_CTRL1_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_DLN3_BIST_CTRL1_IN)
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_BIST_CTRL1_BIST_WORD_CNT_7_0_BMSK                         0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_BIST_CTRL1_BIST_WORD_CNT_7_0_SHFT                          0x0

#define HWIO_DSI_0_PHY_DSIPHY_DLN3_BIST_CTRL2_ADDR                                     (DSI_0_PHY_DSIPHY_DLN3_DSIPHY_DLN3_DSI_DATALN_REG_BASE      + 0x00000024)
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_BIST_CTRL2_RMSK                                           0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_BIST_CTRL2_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN3_BIST_CTRL2_ADDR, HWIO_DSI_0_PHY_DSIPHY_DLN3_BIST_CTRL2_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_BIST_CTRL2_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN3_BIST_CTRL2_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_BIST_CTRL2_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_DLN3_BIST_CTRL2_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_BIST_CTRL2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_DLN3_BIST_CTRL2_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_DLN3_BIST_CTRL2_IN)
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_BIST_CTRL2_BIST_WORD_CNT_15_8_BMSK                        0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_BIST_CTRL2_BIST_WORD_CNT_15_8_SHFT                         0x0

#define HWIO_DSI_0_PHY_DSIPHY_DLN3_MISR_CTRL_ADDR                                      (DSI_0_PHY_DSIPHY_DLN3_DSIPHY_DLN3_DSI_DATALN_REG_BASE      + 0x00000028)
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_MISR_CTRL_RMSK                                            0x1f
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_MISR_CTRL_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN3_MISR_CTRL_ADDR, HWIO_DSI_0_PHY_DSIPHY_DLN3_MISR_CTRL_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_MISR_CTRL_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN3_MISR_CTRL_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_MISR_CTRL_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_DLN3_MISR_CTRL_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_MISR_CTRL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_DLN3_MISR_CTRL_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_DLN3_MISR_CTRL_IN)
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_MISR_CTRL_DEBUG_CLK_DEBUG_SEL_BMSK                        0x1c
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_MISR_CTRL_DEBUG_CLK_DEBUG_SEL_SHFT                         0x2
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_MISR_CTRL_MISR_SIG_CLEAR_BMSK                              0x2
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_MISR_CTRL_MISR_SIG_CLEAR_SHFT                              0x1
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_MISR_CTRL_MISR_ENABLE_BMSK                                 0x1
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_MISR_CTRL_MISR_ENABLE_SHFT                                 0x0

#define HWIO_DSI_0_PHY_DSIPHY_DLN3_DBG_SEL_ADDR                                        (DSI_0_PHY_DSIPHY_DLN3_DSIPHY_DLN3_DSI_DATALN_REG_BASE      + 0x0000002c)
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_DBG_SEL_RMSK                                              0x7f
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_DBG_SEL_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN3_DBG_SEL_ADDR, HWIO_DSI_0_PHY_DSIPHY_DLN3_DBG_SEL_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_DBG_SEL_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN3_DBG_SEL_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_DBG_SEL_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_DLN3_DBG_SEL_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_DBG_SEL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_DLN3_DBG_SEL_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_DLN3_DBG_SEL_IN)
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_DBG_SEL_TEST_CLK_DEBUG_SEL_BMSK                           0x70
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_DBG_SEL_TEST_CLK_DEBUG_SEL_SHFT                            0x4
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_DBG_SEL_DEBUG_SEL_BMSK                                     0xf
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_DBG_SEL_DEBUG_SEL_SHFT                                     0x0

#define HWIO_DSI_0_PHY_DSIPHY_DLN3_SWI_DBG_BUS_CHECK_ADDR                              (DSI_0_PHY_DSIPHY_DLN3_DSIPHY_DLN3_DSI_DATALN_REG_BASE      + 0x00000030)
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_SWI_DBG_BUS_CHECK_RMSK                                    0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_SWI_DBG_BUS_CHECK_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN3_SWI_DBG_BUS_CHECK_ADDR, HWIO_DSI_0_PHY_DSIPHY_DLN3_SWI_DBG_BUS_CHECK_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_SWI_DBG_BUS_CHECK_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN3_SWI_DBG_BUS_CHECK_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_SWI_DBG_BUS_CHECK_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_DLN3_SWI_DBG_BUS_CHECK_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_SWI_DBG_BUS_CHECK_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_DLN3_SWI_DBG_BUS_CHECK_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_DLN3_SWI_DBG_BUS_CHECK_IN)
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_SWI_DBG_BUS_CHECK_LANE_DEBUG_BUS_BMSK                     0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_SWI_DBG_BUS_CHECK_LANE_DEBUG_BUS_SHFT                      0x0

#define HWIO_DSI_0_PHY_DSIPHY_DLN3_BIST_STATUS0_ADDR                                   (DSI_0_PHY_DSIPHY_DLN3_DSIPHY_DLN3_DSI_DATALN_REG_BASE      + 0x00000034)
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_BIST_STATUS0_RMSK                                         0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_BIST_STATUS0_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN3_BIST_STATUS0_ADDR, HWIO_DSI_0_PHY_DSIPHY_DLN3_BIST_STATUS0_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_BIST_STATUS0_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN3_BIST_STATUS0_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_BIST_STATUS0_BISTCHK_ONE_BIT_ERROR_BMSK                   0x80
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_BIST_STATUS0_BISTCHK_ONE_BIT_ERROR_SHFT                    0x7
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_BIST_STATUS0_BISTCHK_SYNC_ERROR_BMSK                      0x40
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_BIST_STATUS0_BISTCHK_SYNC_ERROR_SHFT                       0x6
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_BIST_STATUS0_BISTCHK_SEED_ERROR_BMSK                      0x20
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_BIST_STATUS0_BISTCHK_SEED_ERROR_SHFT                       0x5
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_BIST_STATUS0_BISTCHK_PRBS_ERROR_BMSK                      0x10
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_BIST_STATUS0_BISTCHK_PRBS_ERROR_SHFT                       0x4
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_BIST_STATUS0_BISTCHK_STATUS_BMSK                           0xe
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_BIST_STATUS0_BISTCHK_STATUS_SHFT                           0x1
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_BIST_STATUS0_BISTCHK_DONE_BMSK                             0x1
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_BIST_STATUS0_BISTCHK_DONE_SHFT                             0x0

#define HWIO_DSI_0_PHY_DSIPHY_DLN3_BIST_STATUS1_ADDR                                   (DSI_0_PHY_DSIPHY_DLN3_DSIPHY_DLN3_DSI_DATALN_REG_BASE      + 0x00000038)
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_BIST_STATUS1_RMSK                                         0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_BIST_STATUS1_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN3_BIST_STATUS1_ADDR, HWIO_DSI_0_PHY_DSIPHY_DLN3_BIST_STATUS1_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_BIST_STATUS1_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN3_BIST_STATUS1_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_BIST_STATUS1_BISTCHK_ERROR_CNT_7_0_BMSK                   0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_BIST_STATUS1_BISTCHK_ERROR_CNT_7_0_SHFT                    0x0

#define HWIO_DSI_0_PHY_DSIPHY_DLN3_BIST_STATUS2_ADDR                                   (DSI_0_PHY_DSIPHY_DLN3_DSIPHY_DLN3_DSI_DATALN_REG_BASE      + 0x0000003c)
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_BIST_STATUS2_RMSK                                         0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_BIST_STATUS2_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN3_BIST_STATUS2_ADDR, HWIO_DSI_0_PHY_DSIPHY_DLN3_BIST_STATUS2_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_BIST_STATUS2_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN3_BIST_STATUS2_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_BIST_STATUS2_BISTCHK_ERROR_CNT_15_8_BMSK                  0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_BIST_STATUS2_BISTCHK_ERROR_CNT_15_8_SHFT                   0x0

#define HWIO_DSI_0_PHY_DSIPHY_DLN3_BIST_STATUS3_ADDR                                   (DSI_0_PHY_DSIPHY_DLN3_DSIPHY_DLN3_DSI_DATALN_REG_BASE      + 0x00000040)
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_BIST_STATUS3_RMSK                                         0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_BIST_STATUS3_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN3_BIST_STATUS3_ADDR, HWIO_DSI_0_PHY_DSIPHY_DLN3_BIST_STATUS3_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_BIST_STATUS3_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN3_BIST_STATUS3_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_BIST_STATUS3_BISTCHK_WORD_CNT_7_0_BMSK                    0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_BIST_STATUS3_BISTCHK_WORD_CNT_7_0_SHFT                     0x0

#define HWIO_DSI_0_PHY_DSIPHY_DLN3_BIST_STATUS4_ADDR                                   (DSI_0_PHY_DSIPHY_DLN3_DSIPHY_DLN3_DSI_DATALN_REG_BASE      + 0x00000044)
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_BIST_STATUS4_RMSK                                         0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_BIST_STATUS4_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN3_BIST_STATUS4_ADDR, HWIO_DSI_0_PHY_DSIPHY_DLN3_BIST_STATUS4_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_BIST_STATUS4_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN3_BIST_STATUS4_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_BIST_STATUS4_BISTCHK_WORD_CNT_15_8_BMSK                   0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_BIST_STATUS4_BISTCHK_WORD_CNT_15_8_SHFT                    0x0

#define HWIO_DSI_0_PHY_DSIPHY_DLN3_LP_BIST_STATUS0_ADDR                                (DSI_0_PHY_DSIPHY_DLN3_DSIPHY_DLN3_DSI_DATALN_REG_BASE      + 0x00000048)
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_LP_BIST_STATUS0_RMSK                                       0x3
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_LP_BIST_STATUS0_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN3_LP_BIST_STATUS0_ADDR, HWIO_DSI_0_PHY_DSIPHY_DLN3_LP_BIST_STATUS0_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_LP_BIST_STATUS0_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN3_LP_BIST_STATUS0_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_LP_BIST_STATUS0_LP_BISTCHK_PRBS_ERROR_BMSK                 0x2
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_LP_BIST_STATUS0_LP_BISTCHK_PRBS_ERROR_SHFT                 0x1
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_LP_BIST_STATUS0_LP_BISTCHK_DONE_BMSK                       0x1
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_LP_BIST_STATUS0_LP_BISTCHK_DONE_SHFT                       0x0

#define HWIO_DSI_0_PHY_DSIPHY_DLN3_LP_BIST_STATUS1_ADDR                                (DSI_0_PHY_DSIPHY_DLN3_DSIPHY_DLN3_DSI_DATALN_REG_BASE      + 0x0000004c)
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_LP_BIST_STATUS1_RMSK                                      0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_LP_BIST_STATUS1_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN3_LP_BIST_STATUS1_ADDR, HWIO_DSI_0_PHY_DSIPHY_DLN3_LP_BIST_STATUS1_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_LP_BIST_STATUS1_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN3_LP_BIST_STATUS1_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_LP_BIST_STATUS1_LP_BISTCHK_ERROR_CNT_BMSK                 0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_LP_BIST_STATUS1_LP_BISTCHK_ERROR_CNT_SHFT                  0x0

#define HWIO_DSI_0_PHY_DSIPHY_DLN3_LP_BIST_STATUS2_ADDR                                (DSI_0_PHY_DSIPHY_DLN3_DSIPHY_DLN3_DSI_DATALN_REG_BASE      + 0x00000050)
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_LP_BIST_STATUS2_RMSK                                      0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_LP_BIST_STATUS2_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN3_LP_BIST_STATUS2_ADDR, HWIO_DSI_0_PHY_DSIPHY_DLN3_LP_BIST_STATUS2_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_LP_BIST_STATUS2_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN3_LP_BIST_STATUS2_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_LP_BIST_STATUS2_LP_BISTCHK_WORD_CNT_BMSK                  0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_LP_BIST_STATUS2_LP_BISTCHK_WORD_CNT_SHFT                   0x0

#define HWIO_DSI_0_PHY_DSIPHY_DLN3_MISR_STATUS0_ADDR                                   (DSI_0_PHY_DSIPHY_DLN3_DSIPHY_DLN3_DSI_DATALN_REG_BASE      + 0x00000054)
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_MISR_STATUS0_RMSK                                         0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_MISR_STATUS0_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN3_MISR_STATUS0_ADDR, HWIO_DSI_0_PHY_DSIPHY_DLN3_MISR_STATUS0_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_MISR_STATUS0_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN3_MISR_STATUS0_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_MISR_STATUS0_BISTCHK_MISR_SIG_7_0_BMSK                    0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_MISR_STATUS0_BISTCHK_MISR_SIG_7_0_SHFT                     0x0

#define HWIO_DSI_0_PHY_DSIPHY_DLN3_MISR_STATUS1_ADDR                                   (DSI_0_PHY_DSIPHY_DLN3_DSIPHY_DLN3_DSI_DATALN_REG_BASE      + 0x00000058)
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_MISR_STATUS1_RMSK                                         0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_MISR_STATUS1_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN3_MISR_STATUS1_ADDR, HWIO_DSI_0_PHY_DSIPHY_DLN3_MISR_STATUS1_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_MISR_STATUS1_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN3_MISR_STATUS1_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_MISR_STATUS1_BISTCHK_MISR_SIG_15_8_BMSK                   0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_MISR_STATUS1_BISTCHK_MISR_SIG_15_8_SHFT                    0x0

#define HWIO_DSI_0_PHY_DSIPHY_DLN3_DEBUG_BUS0_ADDR                                     (DSI_0_PHY_DSIPHY_DLN3_DSIPHY_DLN3_DSI_DATALN_REG_BASE      + 0x0000005c)
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_DEBUG_BUS0_RMSK                                           0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_DEBUG_BUS0_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN3_DEBUG_BUS0_ADDR, HWIO_DSI_0_PHY_DSIPHY_DLN3_DEBUG_BUS0_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_DEBUG_BUS0_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN3_DEBUG_BUS0_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_DEBUG_BUS0_DEBUG_BUS_7_0_BMSK                             0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_DEBUG_BUS0_DEBUG_BUS_7_0_SHFT                              0x0

#define HWIO_DSI_0_PHY_DSIPHY_DLN3_DEBUG_BUS1_ADDR                                     (DSI_0_PHY_DSIPHY_DLN3_DSIPHY_DLN3_DSI_DATALN_REG_BASE      + 0x00000060)
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_DEBUG_BUS1_RMSK                                           0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_DEBUG_BUS1_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN3_DEBUG_BUS1_ADDR, HWIO_DSI_0_PHY_DSIPHY_DLN3_DEBUG_BUS1_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_DEBUG_BUS1_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN3_DEBUG_BUS1_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_DEBUG_BUS1_DEBUG_BUS_15_8_BMSK                            0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_DEBUG_BUS1_DEBUG_BUS_15_8_SHFT                             0x0

#define HWIO_DSI_0_PHY_DSIPHY_DLN3_DEBUG_BUS2_ADDR                                     (DSI_0_PHY_DSIPHY_DLN3_DSIPHY_DLN3_DSI_DATALN_REG_BASE      + 0x00000064)
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_DEBUG_BUS2_RMSK                                           0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_DEBUG_BUS2_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN3_DEBUG_BUS2_ADDR, HWIO_DSI_0_PHY_DSIPHY_DLN3_DEBUG_BUS2_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_DEBUG_BUS2_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN3_DEBUG_BUS2_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_DEBUG_BUS2_DEBUG_BUS_23_16_BMSK                           0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_DEBUG_BUS2_DEBUG_BUS_23_16_SHFT                            0x0

#define HWIO_DSI_0_PHY_DSIPHY_DLN3_DEBUG_BUS3_ADDR                                     (DSI_0_PHY_DSIPHY_DLN3_DSIPHY_DLN3_DSI_DATALN_REG_BASE      + 0x00000068)
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_DEBUG_BUS3_RMSK                                           0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_DEBUG_BUS3_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN3_DEBUG_BUS3_ADDR, HWIO_DSI_0_PHY_DSIPHY_DLN3_DEBUG_BUS3_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_DEBUG_BUS3_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_DLN3_DEBUG_BUS3_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_DEBUG_BUS3_DEBUG_BUS_31_24_BMSK                           0xff
#define HWIO_DSI_0_PHY_DSIPHY_DLN3_DEBUG_BUS3_DEBUG_BUS_31_24_SHFT                            0x0

/*----------------------------------------------------------------------------
 * MODULE: DSI_0_PHY_DSIPHY_CKLN_DSIPHY_CKLN_DSI_DATALN
 *--------------------------------------------------------------------------*/

#define DSI_0_PHY_DSIPHY_CKLN_DSIPHY_CKLN_DSI_DATALN_REG_BASE                          (MDSS_BASE      + 0x00094800)

#define HWIO_DSI_0_PHY_DSIPHY_CKLN_CFG0_ADDR                                           (DSI_0_PHY_DSIPHY_CKLN_DSIPHY_CKLN_DSI_DATALN_REG_BASE      + 0x00000000)
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_CFG0_RMSK                                                 0x3f
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_CFG0_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CKLN_CFG0_ADDR, HWIO_DSI_0_PHY_DSIPHY_CKLN_CFG0_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_CFG0_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CKLN_CFG0_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_CFG0_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_CKLN_CFG0_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_CFG0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_CKLN_CFG0_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_CKLN_CFG0_IN)
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_CFG0_DSIPHY_HSTX_PGM_DLY_5_4_BMSK                         0x30
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_CFG0_DSIPHY_HSTX_PGM_DLY_5_4_SHFT                          0x4
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_CFG0_DSIPHY_HSTX_PGM_DLY_3_0_BMSK                          0xf
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_CFG0_DSIPHY_HSTX_PGM_DLY_3_0_SHFT                          0x0

#define HWIO_DSI_0_PHY_DSIPHY_CKLN_CFG1_ADDR                                           (DSI_0_PHY_DSIPHY_CKLN_DSIPHY_CKLN_DSI_DATALN_REG_BASE      + 0x00000004)
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_CFG1_RMSK                                                 0x1f
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_CFG1_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CKLN_CFG1_ADDR, HWIO_DSI_0_PHY_DSIPHY_CKLN_CFG1_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_CFG1_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CKLN_CFG1_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_CFG1_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_CKLN_CFG1_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_CFG1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_CKLN_CFG1_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_CKLN_CFG1_IN)
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_CFG1_PROGSEQ_EN_BMSK                                      0x10
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_CFG1_PROGSEQ_EN_SHFT                                       0x4
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_CFG1_DSIPHY_HSTX_SLEW_RATE_BMSK                            0xf
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_CFG1_DSIPHY_HSTX_SLEW_RATE_SHFT                            0x0

#define HWIO_DSI_0_PHY_DSIPHY_CKLN_CFG2_ADDR                                           (DSI_0_PHY_DSIPHY_CKLN_DSIPHY_CKLN_DSI_DATALN_REG_BASE      + 0x00000008)
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_CFG2_RMSK                                                 0xff
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_CFG2_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CKLN_CFG2_ADDR, HWIO_DSI_0_PHY_DSIPHY_CKLN_CFG2_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_CFG2_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CKLN_CFG2_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_CFG2_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_CKLN_CFG2_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_CFG2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_CKLN_CFG2_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_CKLN_CFG2_IN)
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_CFG2_IS_CKLANE_BMSK                                       0x80
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_CFG2_IS_CKLANE_SHFT                                        0x7
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_CFG2_CLK_PATTERN_HSTX_INV_BMSK                            0x40
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_CFG2_CLK_PATTERN_HSTX_INV_SHFT                             0x6
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_CFG2_TEST_MODE_LPTX_DATAN_BMSK                            0x20
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_CFG2_TEST_MODE_LPTX_DATAN_SHFT                             0x5
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_CFG2_TEST_MODE_LPTX_DATAP_BMSK                            0x10
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_CFG2_TEST_MODE_LPTX_DATAP_SHFT                             0x4
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_CFG2_DSIPHY_LPRX_DLY_BMSK                                  0xf
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_CFG2_DSIPHY_LPRX_DLY_SHFT                                  0x0

#define HWIO_DSI_0_PHY_DSIPHY_CKLN_TEST_DATAPATH_ADDR                                  (DSI_0_PHY_DSIPHY_CKLN_DSIPHY_CKLN_DSI_DATALN_REG_BASE      + 0x0000000c)
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_TEST_DATAPATH_RMSK                                        0xff
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_TEST_DATAPATH_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CKLN_TEST_DATAPATH_ADDR, HWIO_DSI_0_PHY_DSIPHY_CKLN_TEST_DATAPATH_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_TEST_DATAPATH_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CKLN_TEST_DATAPATH_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_TEST_DATAPATH_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_CKLN_TEST_DATAPATH_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_TEST_DATAPATH_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_CKLN_TEST_DATAPATH_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_CKLN_TEST_DATAPATH_IN)
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_TEST_DATAPATH_TEST_MODE_LPTX_EN_BMSK                      0x80
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_TEST_DATAPATH_TEST_MODE_LPTX_EN_SHFT                       0x7
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_TEST_DATAPATH_ANE_BYPASS_EN_BMSK                          0x40
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_TEST_DATAPATH_ANE_BYPASS_EN_SHFT                           0x6
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_TEST_DATAPATH_TEST_MODE_HSTX_DATA_BMSK                    0x20
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_TEST_DATAPATH_TEST_MODE_HSTX_DATA_SHFT                     0x5
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_TEST_DATAPATH_DMUX_LP_SEL_BMSK                            0x18
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_TEST_DATAPATH_DMUX_LP_SEL_SHFT                             0x3
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_TEST_DATAPATH_DMUX_HS_SEL_BMSK                             0x4
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_TEST_DATAPATH_DMUX_HS_SEL_SHFT                             0x2
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_TEST_DATAPATH_TEST_MODE_HSTX_EN_BMSK                       0x2
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_TEST_DATAPATH_TEST_MODE_HSTX_EN_SHFT                       0x1
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_TEST_DATAPATH_FORCE_TEST_MODE_BMSK                         0x1
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_TEST_DATAPATH_FORCE_TEST_MODE_SHFT                         0x0

#define HWIO_DSI_0_PHY_DSIPHY_CKLN_PIN_SWAP_ADDR                                       (DSI_0_PHY_DSIPHY_CKLN_DSIPHY_CKLN_DSI_DATALN_REG_BASE      + 0x00000010)
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_PIN_SWAP_RMSK                                              0xf
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_PIN_SWAP_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CKLN_PIN_SWAP_ADDR, HWIO_DSI_0_PHY_DSIPHY_CKLN_PIN_SWAP_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_PIN_SWAP_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CKLN_PIN_SWAP_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_PIN_SWAP_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_CKLN_PIN_SWAP_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_PIN_SWAP_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_CKLN_PIN_SWAP_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_CKLN_PIN_SWAP_IN)
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_PIN_SWAP_ABC_SWAP_BMSK                                     0xe
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_PIN_SWAP_ABC_SWAP_SHFT                                     0x1
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_PIN_SWAP_PN_SWAP_BMSK                                      0x1
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_PIN_SWAP_PN_SWAP_SHFT                                      0x0

#define HWIO_DSI_0_PHY_DSIPHY_CKLN_LPRX_CTRL_ADDR                                      (DSI_0_PHY_DSIPHY_CKLN_DSIPHY_CKLN_DSI_DATALN_REG_BASE      + 0x00000014)
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_LPRX_CTRL_RMSK                                             0x7
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_LPRX_CTRL_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CKLN_LPRX_CTRL_ADDR, HWIO_DSI_0_PHY_DSIPHY_CKLN_LPRX_CTRL_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_LPRX_CTRL_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CKLN_LPRX_CTRL_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_LPRX_CTRL_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_CKLN_LPRX_CTRL_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_LPRX_CTRL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_CKLN_LPRX_CTRL_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_CKLN_LPRX_CTRL_IN)
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_LPRX_CTRL_HS_CONTENTION_DIS_BMSK                           0x4
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_LPRX_CTRL_HS_CONTENTION_DIS_SHFT                           0x2
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_LPRX_CTRL_DSIPHY_LPRX_EN_BMSK                              0x2
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_LPRX_CTRL_DSIPHY_LPRX_EN_SHFT                              0x1
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_LPRX_CTRL_DSIPHY_CDRX_EN_BMSK                              0x1
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_LPRX_CTRL_DSIPHY_CDRX_EN_SHFT                              0x0

#define HWIO_DSI_0_PHY_DSIPHY_CKLN_TX_DCTRL_ADDR                                       (DSI_0_PHY_DSIPHY_CKLN_DSIPHY_CKLN_DSI_DATALN_REG_BASE      + 0x00000018)
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_TX_DCTRL_RMSK                                             0xff
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_TX_DCTRL_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CKLN_TX_DCTRL_ADDR, HWIO_DSI_0_PHY_DSIPHY_CKLN_TX_DCTRL_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_TX_DCTRL_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CKLN_TX_DCTRL_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_TX_DCTRL_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_CKLN_TX_DCTRL_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_TX_DCTRL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_CKLN_TX_DCTRL_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_CKLN_TX_DCTRL_IN)
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_TX_DCTRL_DCTRL_BMSK                                       0xff
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_TX_DCTRL_DCTRL_SHFT                                        0x0

#define HWIO_DSI_0_PHY_DSIPHY_CKLN_BIST_CTRL0_ADDR                                     (DSI_0_PHY_DSIPHY_CKLN_DSIPHY_CKLN_DSI_DATALN_REG_BASE      + 0x0000001c)
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_BIST_CTRL0_RMSK                                           0x3f
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_BIST_CTRL0_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CKLN_BIST_CTRL0_ADDR, HWIO_DSI_0_PHY_DSIPHY_CKLN_BIST_CTRL0_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_BIST_CTRL0_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CKLN_BIST_CTRL0_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_BIST_CTRL0_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_CKLN_BIST_CTRL0_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_BIST_CTRL0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_CKLN_BIST_CTRL0_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_CKLN_BIST_CTRL0_IN)
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_BIST_CTRL0_LP_BISTCHK_ENABLE_BMSK                         0x20
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_BIST_CTRL0_LP_BISTCHK_ENABLE_SHFT                          0x5
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_BIST_CTRL0_BISTCHK_CAPTURE_BMSK                           0x10
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_BIST_CTRL0_BISTCHK_CAPTURE_SHFT                            0x4
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_BIST_CTRL0_BISTCHK_CLR_ERROR_CNT_BMSK                      0x8
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_BIST_CTRL0_BISTCHK_CLR_ERROR_CNT_SHFT                      0x3
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_BIST_CTRL0_BISTCHK_MODE_BMSK                               0x4
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_BIST_CTRL0_BISTCHK_MODE_SHFT                               0x2
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_BIST_CTRL0_BISTCHK_CROSS_BMSK                              0x2
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_BIST_CTRL0_BISTCHK_CROSS_SHFT                              0x1
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_BIST_CTRL0_BISTCHK_ENABLE_BMSK                             0x1
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_BIST_CTRL0_BISTCHK_ENABLE_SHFT                             0x0

#define HWIO_DSI_0_PHY_DSIPHY_CKLN_BIST_CTRL1_ADDR                                     (DSI_0_PHY_DSIPHY_CKLN_DSIPHY_CKLN_DSI_DATALN_REG_BASE      + 0x00000020)
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_BIST_CTRL1_RMSK                                           0xff
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_BIST_CTRL1_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CKLN_BIST_CTRL1_ADDR, HWIO_DSI_0_PHY_DSIPHY_CKLN_BIST_CTRL1_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_BIST_CTRL1_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CKLN_BIST_CTRL1_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_BIST_CTRL1_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_CKLN_BIST_CTRL1_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_BIST_CTRL1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_CKLN_BIST_CTRL1_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_CKLN_BIST_CTRL1_IN)
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_BIST_CTRL1_BIST_WORD_CNT_7_0_BMSK                         0xff
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_BIST_CTRL1_BIST_WORD_CNT_7_0_SHFT                          0x0

#define HWIO_DSI_0_PHY_DSIPHY_CKLN_BIST_CTRL2_ADDR                                     (DSI_0_PHY_DSIPHY_CKLN_DSIPHY_CKLN_DSI_DATALN_REG_BASE      + 0x00000024)
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_BIST_CTRL2_RMSK                                           0xff
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_BIST_CTRL2_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CKLN_BIST_CTRL2_ADDR, HWIO_DSI_0_PHY_DSIPHY_CKLN_BIST_CTRL2_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_BIST_CTRL2_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CKLN_BIST_CTRL2_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_BIST_CTRL2_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_CKLN_BIST_CTRL2_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_BIST_CTRL2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_CKLN_BIST_CTRL2_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_CKLN_BIST_CTRL2_IN)
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_BIST_CTRL2_BIST_WORD_CNT_15_8_BMSK                        0xff
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_BIST_CTRL2_BIST_WORD_CNT_15_8_SHFT                         0x0

#define HWIO_DSI_0_PHY_DSIPHY_CKLN_MISR_CTRL_ADDR                                      (DSI_0_PHY_DSIPHY_CKLN_DSIPHY_CKLN_DSI_DATALN_REG_BASE      + 0x00000028)
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_MISR_CTRL_RMSK                                            0x1f
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_MISR_CTRL_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CKLN_MISR_CTRL_ADDR, HWIO_DSI_0_PHY_DSIPHY_CKLN_MISR_CTRL_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_MISR_CTRL_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CKLN_MISR_CTRL_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_MISR_CTRL_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_CKLN_MISR_CTRL_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_MISR_CTRL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_CKLN_MISR_CTRL_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_CKLN_MISR_CTRL_IN)
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_MISR_CTRL_DEBUG_CLK_DEBUG_SEL_BMSK                        0x1c
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_MISR_CTRL_DEBUG_CLK_DEBUG_SEL_SHFT                         0x2
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_MISR_CTRL_MISR_SIG_CLEAR_BMSK                              0x2
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_MISR_CTRL_MISR_SIG_CLEAR_SHFT                              0x1
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_MISR_CTRL_MISR_ENABLE_BMSK                                 0x1
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_MISR_CTRL_MISR_ENABLE_SHFT                                 0x0

#define HWIO_DSI_0_PHY_DSIPHY_CKLN_DBG_SEL_ADDR                                        (DSI_0_PHY_DSIPHY_CKLN_DSIPHY_CKLN_DSI_DATALN_REG_BASE      + 0x0000002c)
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_DBG_SEL_RMSK                                              0x7f
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_DBG_SEL_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CKLN_DBG_SEL_ADDR, HWIO_DSI_0_PHY_DSIPHY_CKLN_DBG_SEL_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_DBG_SEL_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CKLN_DBG_SEL_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_DBG_SEL_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_CKLN_DBG_SEL_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_DBG_SEL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_CKLN_DBG_SEL_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_CKLN_DBG_SEL_IN)
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_DBG_SEL_TEST_CLK_DEBUG_SEL_BMSK                           0x70
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_DBG_SEL_TEST_CLK_DEBUG_SEL_SHFT                            0x4
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_DBG_SEL_DEBUG_SEL_BMSK                                     0xf
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_DBG_SEL_DEBUG_SEL_SHFT                                     0x0

#define HWIO_DSI_0_PHY_DSIPHY_CKLN_SWI_DBG_BUS_CHECK_ADDR                              (DSI_0_PHY_DSIPHY_CKLN_DSIPHY_CKLN_DSI_DATALN_REG_BASE      + 0x00000030)
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_SWI_DBG_BUS_CHECK_RMSK                                    0xff
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_SWI_DBG_BUS_CHECK_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CKLN_SWI_DBG_BUS_CHECK_ADDR, HWIO_DSI_0_PHY_DSIPHY_CKLN_SWI_DBG_BUS_CHECK_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_SWI_DBG_BUS_CHECK_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CKLN_SWI_DBG_BUS_CHECK_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_SWI_DBG_BUS_CHECK_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_CKLN_SWI_DBG_BUS_CHECK_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_SWI_DBG_BUS_CHECK_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_CKLN_SWI_DBG_BUS_CHECK_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_CKLN_SWI_DBG_BUS_CHECK_IN)
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_SWI_DBG_BUS_CHECK_LANE_DEBUG_BUS_BMSK                     0xff
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_SWI_DBG_BUS_CHECK_LANE_DEBUG_BUS_SHFT                      0x0

#define HWIO_DSI_0_PHY_DSIPHY_CKLN_BIST_STATUS0_ADDR                                   (DSI_0_PHY_DSIPHY_CKLN_DSIPHY_CKLN_DSI_DATALN_REG_BASE      + 0x00000034)
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_BIST_STATUS0_RMSK                                         0xff
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_BIST_STATUS0_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CKLN_BIST_STATUS0_ADDR, HWIO_DSI_0_PHY_DSIPHY_CKLN_BIST_STATUS0_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_BIST_STATUS0_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CKLN_BIST_STATUS0_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_BIST_STATUS0_BISTCHK_ONE_BIT_ERROR_BMSK                   0x80
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_BIST_STATUS0_BISTCHK_ONE_BIT_ERROR_SHFT                    0x7
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_BIST_STATUS0_BISTCHK_SYNC_ERROR_BMSK                      0x40
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_BIST_STATUS0_BISTCHK_SYNC_ERROR_SHFT                       0x6
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_BIST_STATUS0_BISTCHK_SEED_ERROR_BMSK                      0x20
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_BIST_STATUS0_BISTCHK_SEED_ERROR_SHFT                       0x5
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_BIST_STATUS0_BISTCHK_PRBS_ERROR_BMSK                      0x10
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_BIST_STATUS0_BISTCHK_PRBS_ERROR_SHFT                       0x4
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_BIST_STATUS0_BISTCHK_STATUS_BMSK                           0xe
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_BIST_STATUS0_BISTCHK_STATUS_SHFT                           0x1
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_BIST_STATUS0_BISTCHK_DONE_BMSK                             0x1
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_BIST_STATUS0_BISTCHK_DONE_SHFT                             0x0

#define HWIO_DSI_0_PHY_DSIPHY_CKLN_BIST_STATUS1_ADDR                                   (DSI_0_PHY_DSIPHY_CKLN_DSIPHY_CKLN_DSI_DATALN_REG_BASE      + 0x00000038)
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_BIST_STATUS1_RMSK                                         0xff
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_BIST_STATUS1_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CKLN_BIST_STATUS1_ADDR, HWIO_DSI_0_PHY_DSIPHY_CKLN_BIST_STATUS1_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_BIST_STATUS1_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CKLN_BIST_STATUS1_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_BIST_STATUS1_BISTCHK_ERROR_CNT_7_0_BMSK                   0xff
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_BIST_STATUS1_BISTCHK_ERROR_CNT_7_0_SHFT                    0x0

#define HWIO_DSI_0_PHY_DSIPHY_CKLN_BIST_STATUS2_ADDR                                   (DSI_0_PHY_DSIPHY_CKLN_DSIPHY_CKLN_DSI_DATALN_REG_BASE      + 0x0000003c)
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_BIST_STATUS2_RMSK                                         0xff
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_BIST_STATUS2_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CKLN_BIST_STATUS2_ADDR, HWIO_DSI_0_PHY_DSIPHY_CKLN_BIST_STATUS2_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_BIST_STATUS2_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CKLN_BIST_STATUS2_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_BIST_STATUS2_BISTCHK_ERROR_CNT_15_8_BMSK                  0xff
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_BIST_STATUS2_BISTCHK_ERROR_CNT_15_8_SHFT                   0x0

#define HWIO_DSI_0_PHY_DSIPHY_CKLN_BIST_STATUS3_ADDR                                   (DSI_0_PHY_DSIPHY_CKLN_DSIPHY_CKLN_DSI_DATALN_REG_BASE      + 0x00000040)
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_BIST_STATUS3_RMSK                                         0xff
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_BIST_STATUS3_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CKLN_BIST_STATUS3_ADDR, HWIO_DSI_0_PHY_DSIPHY_CKLN_BIST_STATUS3_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_BIST_STATUS3_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CKLN_BIST_STATUS3_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_BIST_STATUS3_BISTCHK_WORD_CNT_7_0_BMSK                    0xff
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_BIST_STATUS3_BISTCHK_WORD_CNT_7_0_SHFT                     0x0

#define HWIO_DSI_0_PHY_DSIPHY_CKLN_BIST_STATUS4_ADDR                                   (DSI_0_PHY_DSIPHY_CKLN_DSIPHY_CKLN_DSI_DATALN_REG_BASE      + 0x00000044)
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_BIST_STATUS4_RMSK                                         0xff
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_BIST_STATUS4_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CKLN_BIST_STATUS4_ADDR, HWIO_DSI_0_PHY_DSIPHY_CKLN_BIST_STATUS4_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_BIST_STATUS4_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CKLN_BIST_STATUS4_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_BIST_STATUS4_BISTCHK_WORD_CNT_15_8_BMSK                   0xff
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_BIST_STATUS4_BISTCHK_WORD_CNT_15_8_SHFT                    0x0

#define HWIO_DSI_0_PHY_DSIPHY_CKLN_LP_BIST_STATUS0_ADDR                                (DSI_0_PHY_DSIPHY_CKLN_DSIPHY_CKLN_DSI_DATALN_REG_BASE      + 0x00000048)
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_LP_BIST_STATUS0_RMSK                                       0x3
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_LP_BIST_STATUS0_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CKLN_LP_BIST_STATUS0_ADDR, HWIO_DSI_0_PHY_DSIPHY_CKLN_LP_BIST_STATUS0_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_LP_BIST_STATUS0_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CKLN_LP_BIST_STATUS0_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_LP_BIST_STATUS0_LP_BISTCHK_PRBS_ERROR_BMSK                 0x2
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_LP_BIST_STATUS0_LP_BISTCHK_PRBS_ERROR_SHFT                 0x1
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_LP_BIST_STATUS0_LP_BISTCHK_DONE_BMSK                       0x1
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_LP_BIST_STATUS0_LP_BISTCHK_DONE_SHFT                       0x0

#define HWIO_DSI_0_PHY_DSIPHY_CKLN_LP_BIST_STATUS1_ADDR                                (DSI_0_PHY_DSIPHY_CKLN_DSIPHY_CKLN_DSI_DATALN_REG_BASE      + 0x0000004c)
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_LP_BIST_STATUS1_RMSK                                      0xff
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_LP_BIST_STATUS1_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CKLN_LP_BIST_STATUS1_ADDR, HWIO_DSI_0_PHY_DSIPHY_CKLN_LP_BIST_STATUS1_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_LP_BIST_STATUS1_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CKLN_LP_BIST_STATUS1_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_LP_BIST_STATUS1_LP_BISTCHK_ERROR_CNT_BMSK                 0xff
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_LP_BIST_STATUS1_LP_BISTCHK_ERROR_CNT_SHFT                  0x0

#define HWIO_DSI_0_PHY_DSIPHY_CKLN_LP_BIST_STATUS2_ADDR                                (DSI_0_PHY_DSIPHY_CKLN_DSIPHY_CKLN_DSI_DATALN_REG_BASE      + 0x00000050)
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_LP_BIST_STATUS2_RMSK                                      0xff
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_LP_BIST_STATUS2_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CKLN_LP_BIST_STATUS2_ADDR, HWIO_DSI_0_PHY_DSIPHY_CKLN_LP_BIST_STATUS2_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_LP_BIST_STATUS2_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CKLN_LP_BIST_STATUS2_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_LP_BIST_STATUS2_LP_BISTCHK_WORD_CNT_BMSK                  0xff
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_LP_BIST_STATUS2_LP_BISTCHK_WORD_CNT_SHFT                   0x0

#define HWIO_DSI_0_PHY_DSIPHY_CKLN_MISR_STATUS0_ADDR                                   (DSI_0_PHY_DSIPHY_CKLN_DSIPHY_CKLN_DSI_DATALN_REG_BASE      + 0x00000054)
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_MISR_STATUS0_RMSK                                         0xff
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_MISR_STATUS0_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CKLN_MISR_STATUS0_ADDR, HWIO_DSI_0_PHY_DSIPHY_CKLN_MISR_STATUS0_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_MISR_STATUS0_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CKLN_MISR_STATUS0_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_MISR_STATUS0_BISTCHK_MISR_SIG_7_0_BMSK                    0xff
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_MISR_STATUS0_BISTCHK_MISR_SIG_7_0_SHFT                     0x0

#define HWIO_DSI_0_PHY_DSIPHY_CKLN_MISR_STATUS1_ADDR                                   (DSI_0_PHY_DSIPHY_CKLN_DSIPHY_CKLN_DSI_DATALN_REG_BASE      + 0x00000058)
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_MISR_STATUS1_RMSK                                         0xff
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_MISR_STATUS1_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CKLN_MISR_STATUS1_ADDR, HWIO_DSI_0_PHY_DSIPHY_CKLN_MISR_STATUS1_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_MISR_STATUS1_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CKLN_MISR_STATUS1_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_MISR_STATUS1_BISTCHK_MISR_SIG_15_8_BMSK                   0xff
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_MISR_STATUS1_BISTCHK_MISR_SIG_15_8_SHFT                    0x0

#define HWIO_DSI_0_PHY_DSIPHY_CKLN_DEBUG_BUS0_ADDR                                     (DSI_0_PHY_DSIPHY_CKLN_DSIPHY_CKLN_DSI_DATALN_REG_BASE      + 0x0000005c)
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_DEBUG_BUS0_RMSK                                           0xff
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_DEBUG_BUS0_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CKLN_DEBUG_BUS0_ADDR, HWIO_DSI_0_PHY_DSIPHY_CKLN_DEBUG_BUS0_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_DEBUG_BUS0_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CKLN_DEBUG_BUS0_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_DEBUG_BUS0_DEBUG_BUS_7_0_BMSK                             0xff
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_DEBUG_BUS0_DEBUG_BUS_7_0_SHFT                              0x0

#define HWIO_DSI_0_PHY_DSIPHY_CKLN_DEBUG_BUS1_ADDR                                     (DSI_0_PHY_DSIPHY_CKLN_DSIPHY_CKLN_DSI_DATALN_REG_BASE      + 0x00000060)
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_DEBUG_BUS1_RMSK                                           0xff
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_DEBUG_BUS1_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CKLN_DEBUG_BUS1_ADDR, HWIO_DSI_0_PHY_DSIPHY_CKLN_DEBUG_BUS1_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_DEBUG_BUS1_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CKLN_DEBUG_BUS1_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_DEBUG_BUS1_DEBUG_BUS_15_8_BMSK                            0xff
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_DEBUG_BUS1_DEBUG_BUS_15_8_SHFT                             0x0

#define HWIO_DSI_0_PHY_DSIPHY_CKLN_DEBUG_BUS2_ADDR                                     (DSI_0_PHY_DSIPHY_CKLN_DSIPHY_CKLN_DSI_DATALN_REG_BASE      + 0x00000064)
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_DEBUG_BUS2_RMSK                                           0xff
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_DEBUG_BUS2_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CKLN_DEBUG_BUS2_ADDR, HWIO_DSI_0_PHY_DSIPHY_CKLN_DEBUG_BUS2_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_DEBUG_BUS2_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CKLN_DEBUG_BUS2_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_DEBUG_BUS2_DEBUG_BUS_23_16_BMSK                           0xff
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_DEBUG_BUS2_DEBUG_BUS_23_16_SHFT                            0x0

#define HWIO_DSI_0_PHY_DSIPHY_CKLN_DEBUG_BUS3_ADDR                                     (DSI_0_PHY_DSIPHY_CKLN_DSIPHY_CKLN_DSI_DATALN_REG_BASE      + 0x00000068)
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_DEBUG_BUS3_RMSK                                           0xff
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_DEBUG_BUS3_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CKLN_DEBUG_BUS3_ADDR, HWIO_DSI_0_PHY_DSIPHY_CKLN_DEBUG_BUS3_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_DEBUG_BUS3_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_CKLN_DEBUG_BUS3_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_DEBUG_BUS3_DEBUG_BUS_31_24_BMSK                           0xff
#define HWIO_DSI_0_PHY_DSIPHY_CKLN_DEBUG_BUS3_DEBUG_BUS_31_24_SHFT                            0x0

/*----------------------------------------------------------------------------
 * MODULE: DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL
 *--------------------------------------------------------------------------*/

#define DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE                                            (MDSS_BASE      + 0x00094900)

#define HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_ONE_ADDR                                               (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000000)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_ONE_RMSK                                                     0x7f
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_ONE_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_ONE_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_ONE_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_ONE_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_ONE_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_ONE_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_ONE_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_ONE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_ONE_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_ONE_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_ONE_DISABLE_CLKGATE_BMSK                                     0x40
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_ONE_DISABLE_CLKGATE_SHFT                                      0x6
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_ONE_CM_PLL_LOCK_BYP_BMSK                                     0x20
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_ONE_CM_PLL_LOCK_BYP_SHFT                                      0x5
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_ONE_CM_PLL_BYPASS_BMSK                                       0x10
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_ONE_CM_PLL_BYPASS_SHFT                                        0x4
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_ONE_CM_PLL_RST_MUX_BMSK                                       0x8
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_ONE_CM_PLL_RST_MUX_SHFT                                       0x3
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_ONE_CM_PLL_RST_BMSK                                           0x4
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_ONE_CM_PLL_RST_SHFT                                           0x2
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_ONE_CM_PLL_PWRDN_MUX_BMSK                                     0x2
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_ONE_CM_PLL_PWRDN_MUX_SHFT                                     0x1
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_ONE_CM_PLL_PWRDN_BMSK                                         0x1
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_ONE_CM_PLL_PWRDN_SHFT                                         0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_TWO_ADDR                                               (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000004)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_TWO_RMSK                                                      0xf
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_TWO_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_TWO_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_TWO_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_TWO_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_TWO_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_TWO_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_TWO_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_TWO_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_TWO_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_TWO_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_TWO_PWRDN_RESET_MUX_BMSK                                      0x8
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_TWO_PWRDN_RESET_MUX_SHFT                                      0x3
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_TWO_PWRDN_RESET_BMSK                                          0x4
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_TWO_PWRDN_RESET_SHFT                                          0x2
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_TWO_PWRDN_B_BIAS_BMSK                                         0x2
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_TWO_PWRDN_B_BIAS_SHFT                                         0x1
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_TWO_PWRDN_B_BMSK                                              0x1
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_TWO_PWRDN_B_SHFT                                              0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_INT_LOOP_SETTINGS_ADDR                                                 (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000008)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_INT_LOOP_SETTINGS_RMSK                                                       0x7f
#define HWIO_DSI_0_PHY_DSIPHY_PLL_INT_LOOP_SETTINGS_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_INT_LOOP_SETTINGS_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_INT_LOOP_SETTINGS_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_INT_LOOP_SETTINGS_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_INT_LOOP_SETTINGS_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_INT_LOOP_SETTINGS_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_INT_LOOP_SETTINGS_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_INT_LOOP_SETTINGS_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_INT_LOOP_SETTINGS_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_INT_LOOP_SETTINGS_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_INT_LOOP_SETTINGS_QIL_POLARITY_BMSK                                          0x40
#define HWIO_DSI_0_PHY_DSIPHY_PLL_INT_LOOP_SETTINGS_QIL_POLARITY_SHFT                                           0x6
#define HWIO_DSI_0_PHY_DSIPHY_PLL_INT_LOOP_SETTINGS_LFSR_SEED_BMSK                                             0x3f
#define HWIO_DSI_0_PHY_DSIPHY_PLL_INT_LOOP_SETTINGS_LFSR_SEED_SHFT                                              0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_INT_LOOP_SETTINGS_TWO_ADDR                                             (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x0000000c)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_INT_LOOP_SETTINGS_TWO_RMSK                                                   0x1f
#define HWIO_DSI_0_PHY_DSIPHY_PLL_INT_LOOP_SETTINGS_TWO_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_INT_LOOP_SETTINGS_TWO_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_INT_LOOP_SETTINGS_TWO_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_INT_LOOP_SETTINGS_TWO_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_INT_LOOP_SETTINGS_TWO_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_INT_LOOP_SETTINGS_TWO_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_INT_LOOP_SETTINGS_TWO_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_INT_LOOP_SETTINGS_TWO_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_INT_LOOP_SETTINGS_TWO_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_INT_LOOP_SETTINGS_TWO_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_INT_LOOP_SETTINGS_TWO_LFSR_EN_BMSK                                           0x10
#define HWIO_DSI_0_PHY_DSIPHY_PLL_INT_LOOP_SETTINGS_TWO_LFSR_EN_SHFT                                            0x4
#define HWIO_DSI_0_PHY_DSIPHY_PLL_INT_LOOP_SETTINGS_TWO_SECOND_ORDER_EN_BMSK                                    0x8
#define HWIO_DSI_0_PHY_DSIPHY_PLL_INT_LOOP_SETTINGS_TWO_SECOND_ORDER_EN_SHFT                                    0x3
#define HWIO_DSI_0_PHY_DSIPHY_PLL_INT_LOOP_SETTINGS_TWO_QIL_DEC_MUX_BMSK                                        0x4
#define HWIO_DSI_0_PHY_DSIPHY_PLL_INT_LOOP_SETTINGS_TWO_QIL_DEC_MUX_SHFT                                        0x2
#define HWIO_DSI_0_PHY_DSIPHY_PLL_INT_LOOP_SETTINGS_TWO_QIL_DEC_BMSK                                            0x3
#define HWIO_DSI_0_PHY_DSIPHY_PLL_INT_LOOP_SETTINGS_TWO_QIL_DEC_SHFT                                            0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_THREE_ADDR                                             (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000010)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_THREE_RMSK                                                   0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_THREE_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_THREE_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_THREE_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_THREE_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_THREE_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_THREE_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_THREE_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_THREE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_THREE_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_THREE_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_THREE_CM_PLL_BAND_SEL_BMSK                                   0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_THREE_CM_PLL_BAND_SEL_SHFT                                    0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FOUR_ADDR                                              (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000014)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FOUR_RMSK                                                    0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FOUR_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FOUR_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FOUR_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FOUR_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FOUR_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FOUR_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FOUR_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FOUR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FOUR_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FOUR_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FOUR_CM_PLL_ATB_SEL_BMSK                                     0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FOUR_CM_PLL_ATB_SEL_SHFT                                      0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FIVE_ADDR                                              (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000018)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FIVE_RMSK                                                     0x3
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FIVE_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FIVE_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FIVE_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FIVE_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FIVE_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FIVE_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FIVE_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FIVE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FIVE_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FIVE_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FIVE_CM_VCOCLK_DIV2EN_MSB_BMSK                                0x2
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FIVE_CM_VCOCLK_DIV2EN_MSB_SHFT                                0x1
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FIVE_CM_VCOCLK_DIV2EN_BMSK                                    0x1
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FIVE_CM_VCOCLK_DIV2EN_SHFT                                    0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_INT_LOOP_CONTROLS_ADDR                                                 (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x0000001c)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_INT_LOOP_CONTROLS_RMSK                                                       0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_INT_LOOP_CONTROLS_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_INT_LOOP_CONTROLS_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_INT_LOOP_CONTROLS_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_INT_LOOP_CONTROLS_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_INT_LOOP_CONTROLS_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_INT_LOOP_CONTROLS_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_INT_LOOP_CONTROLS_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_INT_LOOP_CONTROLS_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_INT_LOOP_CONTROLS_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_INT_LOOP_CONTROLS_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_INT_LOOP_CONTROLS_BYP_OUTPUT_REGS_BMSK                                       0x80
#define HWIO_DSI_0_PHY_DSIPHY_PLL_INT_LOOP_CONTROLS_BYP_OUTPUT_REGS_SHFT                                        0x7
#define HWIO_DSI_0_PHY_DSIPHY_PLL_INT_LOOP_CONTROLS_AVG_DEC_BMSK                                               0x40
#define HWIO_DSI_0_PHY_DSIPHY_PLL_INT_LOOP_CONTROLS_AVG_DEC_SHFT                                                0x6
#define HWIO_DSI_0_PHY_DSIPHY_PLL_INT_LOOP_CONTROLS_TDC_DECIMATE_BMSK                                          0x38
#define HWIO_DSI_0_PHY_DSIPHY_PLL_INT_LOOP_CONTROLS_TDC_DECIMATE_SHFT                                           0x3
#define HWIO_DSI_0_PHY_DSIPHY_PLL_INT_LOOP_CONTROLS_TDC_CLEAR_MUX_BMSK                                          0x4
#define HWIO_DSI_0_PHY_DSIPHY_PLL_INT_LOOP_CONTROLS_TDC_CLEAR_MUX_SHFT                                          0x2
#define HWIO_DSI_0_PHY_DSIPHY_PLL_INT_LOOP_CONTROLS_TDC_CLEAR_BMSK                                              0x2
#define HWIO_DSI_0_PHY_DSIPHY_PLL_INT_LOOP_CONTROLS_TDC_CLEAR_SHFT                                              0x1
#define HWIO_DSI_0_PHY_DSIPHY_PLL_INT_LOOP_CONTROLS_TDC_POLARITY_BMSK                                           0x1
#define HWIO_DSI_0_PHY_DSIPHY_PLL_INT_LOOP_CONTROLS_TDC_POLARITY_SHFT                                           0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_DSM_DIVIDER_ADDR                                                       (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000020)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_DSM_DIVIDER_RMSK                                                             0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_DSM_DIVIDER_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_DSM_DIVIDER_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_DSM_DIVIDER_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_DSM_DIVIDER_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_DSM_DIVIDER_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_DSM_DIVIDER_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_DSM_DIVIDER_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_DSM_DIVIDER_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_DSM_DIVIDER_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_DSM_DIVIDER_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_DSM_DIVIDER_CM_PLL_DSMDIV_BMSK                                               0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_DSM_DIVIDER_CM_PLL_DSMDIV_SHFT                                                0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_FEEDBACK_DIVIDER_ADDR                                                  (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000024)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FEEDBACK_DIVIDER_RMSK                                                        0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FEEDBACK_DIVIDER_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_FEEDBACK_DIVIDER_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_FEEDBACK_DIVIDER_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FEEDBACK_DIVIDER_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_FEEDBACK_DIVIDER_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FEEDBACK_DIVIDER_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_FEEDBACK_DIVIDER_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FEEDBACK_DIVIDER_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_FEEDBACK_DIVIDER_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_FEEDBACK_DIVIDER_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FEEDBACK_DIVIDER_CM_PLL_FBDIV_BMSK                                           0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FEEDBACK_DIVIDER_CM_PLL_FBDIV_SHFT                                            0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_SYSTEM_MUXES_ADDR                                                      (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000028)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SYSTEM_MUXES_RMSK                                                            0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SYSTEM_MUXES_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_SYSTEM_MUXES_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_SYSTEM_MUXES_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SYSTEM_MUXES_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_SYSTEM_MUXES_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SYSTEM_MUXES_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_SYSTEM_MUXES_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SYSTEM_MUXES_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_SYSTEM_MUXES_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_SYSTEM_MUXES_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SYSTEM_MUXES_CM_BIAS_EN_MUX_BMSK                                             0x80
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SYSTEM_MUXES_CM_BIAS_EN_MUX_SHFT                                              0x7
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SYSTEM_MUXES_CM_BIAS_EN_BMSK                                                 0x40
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SYSTEM_MUXES_CM_BIAS_EN_SHFT                                                  0x6
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SYSTEM_MUXES_CM_PLL_BAND_SEL_MUX_BMSK                                        0x20
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SYSTEM_MUXES_CM_PLL_BAND_SEL_MUX_SHFT                                         0x5
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SYSTEM_MUXES_CM_PLL_LOCKDET_MUX_BMSK                                         0x10
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SYSTEM_MUXES_CM_PLL_LOCKDET_MUX_SHFT                                          0x4
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SYSTEM_MUXES_CM_PLL_PROP_GAIN_MUX_BMSK                                        0x8
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SYSTEM_MUXES_CM_PLL_PROP_GAIN_MUX_SHFT                                        0x3
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SYSTEM_MUXES_CM_PLL_INT_GAIN_MUX_BMSK                                         0x4
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SYSTEM_MUXES_CM_PLL_INT_GAIN_MUX_SHFT                                         0x2
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SYSTEM_MUXES_CM_PLL_FBDIV_MUX_BMSK                                            0x2
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SYSTEM_MUXES_CM_PLL_FBDIV_MUX_SHFT                                            0x1
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SYSTEM_MUXES_CM_PLL_DSMDIV_MUX_BMSK                                           0x1
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SYSTEM_MUXES_CM_PLL_DSMDIV_MUX_SHFT                                           0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_UPDATE_CONTROL_OVERRIDES_ADDR                                     (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x0000002c)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_UPDATE_CONTROL_OVERRIDES_RMSK                                           0x3f
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_UPDATE_CONTROL_OVERRIDES_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_UPDATE_CONTROL_OVERRIDES_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_UPDATE_CONTROL_OVERRIDES_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_UPDATE_CONTROL_OVERRIDES_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_UPDATE_CONTROL_OVERRIDES_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_UPDATE_CONTROL_OVERRIDES_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_UPDATE_CONTROL_OVERRIDES_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_UPDATE_CONTROL_OVERRIDES_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_UPDATE_CONTROL_OVERRIDES_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_UPDATE_CONTROL_OVERRIDES_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_UPDATE_CONTROL_OVERRIDES_HOLD_SSC_OUTPUT_MUX_BMSK                       0x20
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_UPDATE_CONTROL_OVERRIDES_HOLD_SSC_OUTPUT_MUX_SHFT                        0x5
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_UPDATE_CONTROL_OVERRIDES_HOLD_SSC_OUTPUT_BMSK                           0x10
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_UPDATE_CONTROL_OVERRIDES_HOLD_SSC_OUTPUT_SHFT                            0x4
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_UPDATE_CONTROL_OVERRIDES_UPDATE_LOOP_MUX_BMSK                            0x8
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_UPDATE_CONTROL_OVERRIDES_UPDATE_LOOP_MUX_SHFT                            0x3
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_UPDATE_CONTROL_OVERRIDES_UPDATE_LOOP_BMSK                                0x4
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_UPDATE_CONTROL_OVERRIDES_UPDATE_LOOP_SHFT                                0x2
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_UPDATE_CONTROL_OVERRIDES_UPDATE_PARAMS_MUX_BMSK                          0x2
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_UPDATE_CONTROL_OVERRIDES_UPDATE_PARAMS_MUX_SHFT                          0x1
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_UPDATE_CONTROL_OVERRIDES_UPDATE_PARAMS_BMSK                              0x1
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_UPDATE_CONTROL_OVERRIDES_UPDATE_PARAMS_SHFT                              0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_CMODE_ADDR                                                             (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000030)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CMODE_RMSK                                                                   0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CMODE_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_CMODE_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_CMODE_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CMODE_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_CMODE_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CMODE_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_CMODE_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CMODE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_CMODE_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_CMODE_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CMODE_CM_CMODE_BMSK                                                          0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CMODE_CM_CMODE_SHFT                                                           0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_PSM_CTRL_ADDR                                                          (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000034)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PSM_CTRL_RMSK                                                                0x7f
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PSM_CTRL_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_PSM_CTRL_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_PSM_CTRL_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PSM_CTRL_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_PSM_CTRL_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PSM_CTRL_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_PSM_CTRL_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PSM_CTRL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_PSM_CTRL_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_PSM_CTRL_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PSM_CTRL_EN_PLL_LOCK_MUX_BMSK                                                0x40
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PSM_CTRL_EN_PLL_LOCK_MUX_SHFT                                                 0x6
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PSM_CTRL_EN_PLL_LOCK_BMSK                                                    0x20
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PSM_CTRL_EN_PLL_LOCK_SHFT                                                     0x5
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PSM_CTRL_PWR_STATE_BMSK                                                      0x1c
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PSM_CTRL_PWR_STATE_SHFT                                                       0x2
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PSM_CTRL_PWR_STATE_REQ_MUX_BMSK                                               0x2
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PSM_CTRL_PWR_STATE_REQ_MUX_SHFT                                               0x1
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PSM_CTRL_PWR_STATE_REQ_BMSK                                                   0x1
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PSM_CTRL_PWR_STATE_REQ_SHFT                                                   0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_RSM_CTRL_ADDR                                                          (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000038)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_RSM_CTRL_RMSK                                                                0x1f
#define HWIO_DSI_0_PHY_DSIPHY_PLL_RSM_CTRL_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_RSM_CTRL_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_RSM_CTRL_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_RSM_CTRL_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_RSM_CTRL_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_RSM_CTRL_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_RSM_CTRL_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_RSM_CTRL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_RSM_CTRL_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_RSM_CTRL_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_RSM_CTRL_RSM_LEGACY_MODE_BMSK                                                0x10
#define HWIO_DSI_0_PHY_DSIPHY_PLL_RSM_CTRL_RSM_LEGACY_MODE_SHFT                                                 0x4
#define HWIO_DSI_0_PHY_DSIPHY_PLL_RSM_CTRL_BYPASS_BS_MODE_BMSK                                                  0x8
#define HWIO_DSI_0_PHY_DSIPHY_PLL_RSM_CTRL_BYPASS_BS_MODE_SHFT                                                  0x3
#define HWIO_DSI_0_PHY_DSIPHY_PLL_RSM_CTRL_BYPASS_RSM_PLLLOCK_BMSK                                              0x4
#define HWIO_DSI_0_PHY_DSIPHY_PLL_RSM_CTRL_BYPASS_RSM_PLLLOCK_SHFT                                              0x2
#define HWIO_DSI_0_PHY_DSIPHY_PLL_RSM_CTRL_BYPASS_RSM_BIN_FGCAL_BMSK                                            0x2
#define HWIO_DSI_0_PHY_DSIPHY_PLL_RSM_CTRL_BYPASS_RSM_BIN_FGCAL_SHFT                                            0x1
#define HWIO_DSI_0_PHY_DSIPHY_PLL_RSM_CTRL_BYPASS_RSM_BIAS_BMSK                                                 0x1
#define HWIO_DSI_0_PHY_DSIPHY_PLL_RSM_CTRL_BYPASS_RSM_BIAS_SHFT                                                 0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_VCO_TUNE_MAP_ADDR                                                      (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x0000003c)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_VCO_TUNE_MAP_RMSK                                                             0x7
#define HWIO_DSI_0_PHY_DSIPHY_PLL_VCO_TUNE_MAP_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_VCO_TUNE_MAP_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_VCO_TUNE_MAP_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_VCO_TUNE_MAP_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_VCO_TUNE_MAP_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_VCO_TUNE_MAP_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_VCO_TUNE_MAP_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_VCO_TUNE_MAP_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_VCO_TUNE_MAP_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_VCO_TUNE_MAP_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_VCO_TUNE_MAP_DEFAULT_FREQ_MODE_BMSK                                           0x4
#define HWIO_DSI_0_PHY_DSIPHY_PLL_VCO_TUNE_MAP_DEFAULT_FREQ_MODE_SHFT                                           0x2
#define HWIO_DSI_0_PHY_DSIPHY_PLL_VCO_TUNE_MAP_PS1_FREQ_MODE_BMSK                                               0x2
#define HWIO_DSI_0_PHY_DSIPHY_PLL_VCO_TUNE_MAP_PS1_FREQ_MODE_SHFT                                               0x1
#define HWIO_DSI_0_PHY_DSIPHY_PLL_VCO_TUNE_MAP_PS0_FREQ_MODE_BMSK                                               0x1
#define HWIO_DSI_0_PHY_DSIPHY_PLL_VCO_TUNE_MAP_PS0_FREQ_MODE_SHFT                                               0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_CNTRL_ADDR                                                         (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000040)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_CNTRL_RMSK                                                               0x7f
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_CNTRL_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_CNTRL_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_CNTRL_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_CNTRL_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_CNTRL_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_CNTRL_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_CNTRL_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_CNTRL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_CNTRL_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_CNTRL_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_CNTRL_DONT_SKIP_WAIT_STATE_BMSK                                          0x40
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_CNTRL_DONT_SKIP_WAIT_STATE_SHFT                                           0x6
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_CNTRL_ROUND_UP_ENABLE_BMSK                                               0x20
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_CNTRL_ROUND_UP_ENABLE_SHFT                                                0x5
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_CNTRL_LOCKDET_CDC_FIX2_ENABLE_BMSK                                       0x10
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_CNTRL_LOCKDET_CDC_FIX2_ENABLE_SHFT                                        0x4
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_CNTRL_LOCKDET_CDC_FIX_ENABLE_BMSK                                         0x8
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_CNTRL_LOCKDET_CDC_FIX_ENABLE_SHFT                                         0x3
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_CNTRL_FIX_ENABLE_BMSK                                                     0x4
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_CNTRL_FIX_ENABLE_SHFT                                                     0x2
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_CNTRL_PSM_EN_PLL_MUX_BMSK                                                 0x2
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_CNTRL_PSM_EN_PLL_MUX_SHFT                                                 0x1
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_CNTRL_PSM_EN_PLL_BMSK                                                     0x1
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_CNTRL_PSM_EN_PLL_SHFT                                                     0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_CALIBRATION_SETTINGS_ADDR                                              (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000044)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CALIBRATION_SETTINGS_RMSK                                                    0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CALIBRATION_SETTINGS_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_CALIBRATION_SETTINGS_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_CALIBRATION_SETTINGS_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CALIBRATION_SETTINGS_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_CALIBRATION_SETTINGS_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CALIBRATION_SETTINGS_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_CALIBRATION_SETTINGS_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CALIBRATION_SETTINGS_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_CALIBRATION_SETTINGS_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_CALIBRATION_SETTINGS_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CALIBRATION_SETTINGS_BS_UPPER_MUX_BMSK                                       0x80
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CALIBRATION_SETTINGS_BS_UPPER_MUX_SHFT                                        0x7
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CALIBRATION_SETTINGS_BS_UPPER_BMSK                                           0x40
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CALIBRATION_SETTINGS_BS_UPPER_SHFT                                            0x6
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CALIBRATION_SETTINGS_USE_BAND_SEL_TIMER_BMSK                                 0x20
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CALIBRATION_SETTINGS_USE_BAND_SEL_TIMER_SHFT                                  0x5
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CALIBRATION_SETTINGS_BAND_SEL_CAL_EN_MUX_BMSK                                0x10
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CALIBRATION_SETTINGS_BAND_SEL_CAL_EN_MUX_SHFT                                 0x4
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CALIBRATION_SETTINGS_BAND_SEL_CAL_EN_BMSK                                     0x8
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CALIBRATION_SETTINGS_BAND_SEL_CAL_EN_SHFT                                     0x3
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CALIBRATION_SETTINGS_SKIP_BAND_SEL_CAL_BMSK                                   0x4
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CALIBRATION_SETTINGS_SKIP_BAND_SEL_CAL_SHFT                                   0x2
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CALIBRATION_SETTINGS_CM_PLL_CAL_MUX_BMSK                                      0x2
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CALIBRATION_SETTINGS_CM_PLL_CAL_MUX_SHFT                                      0x1
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CALIBRATION_SETTINGS_CM_PLL_CAL_BMSK                                          0x1
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CALIBRATION_SETTINGS_CM_PLL_CAL_SHFT                                          0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_TIMER_LOW_ADDR                                            (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000048)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_TIMER_LOW_RMSK                                                  0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_TIMER_LOW_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_TIMER_LOW_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_TIMER_LOW_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_TIMER_LOW_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_TIMER_LOW_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_TIMER_LOW_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_TIMER_LOW_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_TIMER_LOW_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_TIMER_LOW_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_TIMER_LOW_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_TIMER_LOW_BAND_SEL_CAL_TIMER_LOW_BMSK                           0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_TIMER_LOW_BAND_SEL_CAL_TIMER_LOW_SHFT                            0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_TIMER_HIGH_ADDR                                           (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x0000004c)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_TIMER_HIGH_RMSK                                                 0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_TIMER_HIGH_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_TIMER_HIGH_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_TIMER_HIGH_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_TIMER_HIGH_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_TIMER_HIGH_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_TIMER_HIGH_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_TIMER_HIGH_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_TIMER_HIGH_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_TIMER_HIGH_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_TIMER_HIGH_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_TIMER_HIGH_BAND_SEL_CAL_TIMER_HIGH_BMSK                         0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_TIMER_HIGH_BAND_SEL_CAL_TIMER_HIGH_SHFT                          0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_ADDR                                             (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000050)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_RMSK                                                   0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_BAND_SEL_CAL_INIT_BMSK                                 0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_BAND_SEL_CAL_INIT_SHFT                                  0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_MIN_ADDR                                                      (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000054)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_MIN_RMSK                                                            0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_MIN_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_MIN_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_MIN_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_MIN_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_MIN_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_MIN_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_MIN_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_MIN_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_MIN_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_MIN_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_MIN_BAND_SEL_CAL_MIN_BMSK                                           0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_MIN_BAND_SEL_CAL_MIN_SHFT                                            0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_MAX_ADDR                                                      (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000058)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_MAX_RMSK                                                            0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_MAX_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_MAX_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_MAX_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_MAX_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_MAX_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_MAX_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_MAX_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_MAX_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_MAX_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_MAX_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_MAX_BAND_SEL_CAL_MAX_BMSK                                           0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_MAX_BAND_SEL_CAL_MAX_SHFT                                            0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_PFILT_ADDR                                                    (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x0000005c)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_PFILT_RMSK                                                          0x1f
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_PFILT_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_PFILT_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_PFILT_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_PFILT_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_PFILT_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_PFILT_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_PFILT_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_PFILT_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_PFILT_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_PFILT_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_PFILT_BS_PFILT_BMSK                                                 0x1f
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_PFILT_BS_PFILT_SHFT                                                  0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_IFILT_ADDR                                                    (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000060)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_IFILT_RMSK                                                          0x1f
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_IFILT_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_IFILT_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_IFILT_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_IFILT_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_IFILT_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_IFILT_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_IFILT_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_IFILT_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_IFILT_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_IFILT_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_IFILT_BS_IFILT_BMSK                                                 0x1f
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_IFILT_BS_IFILT_SHFT                                                  0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_TWO_ADDR                                         (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000064)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_TWO_RMSK                                               0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_TWO_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_TWO_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_TWO_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_TWO_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_TWO_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_TWO_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_TWO_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_TWO_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_TWO_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_TWO_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_TWO_BS_CAL_DONE_MUX_BMSK                               0x80
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_TWO_BS_CAL_DONE_MUX_SHFT                                0x7
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_TWO_BS_CAL_DONE_BMSK                                   0x40
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_TWO_BS_CAL_DONE_SHFT                                    0x6
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_TWO_BS_STOP_ON_DONE_BMSK                               0x20
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_TWO_BS_STOP_ON_DONE_SHFT                                0x5
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_TWO_BS_LOCK_COUNT_BMSK                                 0x1c
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_TWO_BS_LOCK_COUNT_SHFT                                  0x2
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_TWO_BS_LOCK_THRESH_BMSK                                 0x3
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_TWO_BS_LOCK_THRESH_SHFT                                 0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_THREE_ADDR                                       (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000068)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_THREE_RMSK                                             0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_THREE_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_THREE_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_THREE_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_THREE_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_THREE_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_THREE_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_THREE_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_THREE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_THREE_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_THREE_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_THREE_USE_BS_CAL_CODE_BMSK                             0x80
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_THREE_USE_BS_CAL_CODE_SHFT                              0x7
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_THREE_BS_NO_SETTLE_BMSK                                0x40
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_THREE_BS_NO_SETTLE_SHFT                                 0x6
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_THREE_BS_GAIN_BMSK                                     0x3c
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_THREE_BS_GAIN_SHFT                                      0x2
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_THREE_BS_INIT_ON_REENABLE_BMSK                          0x2
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_THREE_BS_INIT_ON_REENABLE_SHFT                          0x1
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_THREE_BS_DEMET_ERROR_BMSK                               0x1
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_THREE_BS_DEMET_ERROR_SHFT                               0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_FOUR_ADDR                                        (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x0000006c)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_FOUR_RMSK                                              0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_FOUR_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_FOUR_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_FOUR_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_FOUR_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_FOUR_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_FOUR_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_FOUR_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_FOUR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_FOUR_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_FOUR_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_FOUR_BS_SETTLE_TIME_BMSK                               0xf0
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_FOUR_BS_SETTLE_TIME_SHFT                                0x4
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_FOUR_BS_INITIAL_TIME_BMSK                               0xf
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_FOUR_BS_INITIAL_TIME_SHFT                               0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_ICODE_HIGH_ADDR                                               (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000070)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_ICODE_HIGH_RMSK                                                     0x1f
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_ICODE_HIGH_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_ICODE_HIGH_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_ICODE_HIGH_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_ICODE_HIGH_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_ICODE_HIGH_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_ICODE_HIGH_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_ICODE_HIGH_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_ICODE_HIGH_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_ICODE_HIGH_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_ICODE_HIGH_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_ICODE_HIGH_BAND_SEL_2MSB_MUX_BMSK                                   0x10
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_ICODE_HIGH_BAND_SEL_2MSB_MUX_SHFT                                    0x4
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_ICODE_HIGH_BAND_SEL_2MSB_BMSK                                        0xc
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_ICODE_HIGH_BAND_SEL_2MSB_SHFT                                        0x2
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_ICODE_HIGH_BAND_SEL_ICODE_HIGH_BMSK                                  0x3
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_ICODE_HIGH_BAND_SEL_ICODE_HIGH_SHFT                                  0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_ICODE_LOW_ADDR                                                (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000074)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_ICODE_LOW_RMSK                                                      0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_ICODE_LOW_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_ICODE_LOW_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_ICODE_LOW_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_ICODE_LOW_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_ICODE_LOW_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_ICODE_LOW_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_ICODE_LOW_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_ICODE_LOW_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_ICODE_LOW_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_ICODE_LOW_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_ICODE_LOW_BAND_SEL_ICODE_LOW_BMSK                                   0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_ICODE_LOW_BAND_SEL_ICODE_LOW_SHFT                                    0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_DETECT_SETTINGS_ONE_ADDR                                          (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000078)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_DETECT_SETTINGS_ONE_RMSK                                                0x7f
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_DETECT_SETTINGS_ONE_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_DETECT_SETTINGS_ONE_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_DETECT_SETTINGS_ONE_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_DETECT_SETTINGS_ONE_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_DETECT_SETTINGS_ONE_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_DETECT_SETTINGS_ONE_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_DETECT_SETTINGS_ONE_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_DETECT_SETTINGS_ONE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_DETECT_SETTINGS_ONE_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_DETECT_SETTINGS_ONE_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_DETECT_SETTINGS_ONE_STICKY_UNLOCK_BMSK                                  0x40
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_DETECT_SETTINGS_ONE_STICKY_UNLOCK_SHFT                                   0x6
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_DETECT_SETTINGS_ONE_USE_FD_LOCK_BMSK                                    0x20
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_DETECT_SETTINGS_ONE_USE_FD_LOCK_SHFT                                     0x5
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_DETECT_SETTINGS_ONE_INVERT_DETECT_POLARITY_BMSK                         0x10
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_DETECT_SETTINGS_ONE_INVERT_DETECT_POLARITY_SHFT                          0x4
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_DETECT_SETTINGS_ONE_PFD_SOURCE_BMSK                                      0x8
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_DETECT_SETTINGS_ONE_PFD_SOURCE_SHFT                                      0x3
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_DETECT_SETTINGS_ONE_FD_MODE_BMSK                                         0x4
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_DETECT_SETTINGS_ONE_FD_MODE_SHFT                                         0x2
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_DETECT_SETTINGS_ONE_FD_ENABLE_MUX_BMSK                                   0x2
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_DETECT_SETTINGS_ONE_FD_ENABLE_MUX_SHFT                                   0x1
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_DETECT_SETTINGS_ONE_FD_ENABLE_BMSK                                       0x1
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_DETECT_SETTINGS_ONE_FD_ENABLE_SHFT                                       0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_DETECT_THRESH_ADDR                                                (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x0000007c)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_DETECT_THRESH_RMSK                                                      0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_DETECT_THRESH_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_DETECT_THRESH_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_DETECT_THRESH_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_DETECT_THRESH_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_DETECT_THRESH_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_DETECT_THRESH_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_DETECT_THRESH_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_DETECT_THRESH_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_DETECT_THRESH_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_DETECT_THRESH_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_DETECT_THRESH_FD_THRESH_BMSK                                            0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_DETECT_THRESH_FD_THRESH_SHFT                                             0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_DET_REFCLK_HIGH_ADDR                                              (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000080)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_DET_REFCLK_HIGH_RMSK                                                     0x3
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_DET_REFCLK_HIGH_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_DET_REFCLK_HIGH_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_DET_REFCLK_HIGH_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_DET_REFCLK_HIGH_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_DET_REFCLK_HIGH_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_DET_REFCLK_HIGH_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_DET_REFCLK_HIGH_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_DET_REFCLK_HIGH_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_DET_REFCLK_HIGH_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_DET_REFCLK_HIGH_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_DET_REFCLK_HIGH_FD_REF_HIGH_BMSK                                         0x3
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_DET_REFCLK_HIGH_FD_REF_HIGH_SHFT                                         0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_DET_REFCLK_LOW_ADDR                                               (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000084)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_DET_REFCLK_LOW_RMSK                                                     0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_DET_REFCLK_LOW_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_DET_REFCLK_LOW_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_DET_REFCLK_LOW_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_DET_REFCLK_LOW_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_DET_REFCLK_LOW_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_DET_REFCLK_LOW_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_DET_REFCLK_LOW_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_DET_REFCLK_LOW_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_DET_REFCLK_LOW_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_DET_REFCLK_LOW_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_DET_REFCLK_LOW_FD_REF_LOW_BMSK                                          0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_DET_REFCLK_LOW_FD_REF_LOW_SHFT                                           0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_DET_PLLCLK_HIGH_ADDR                                              (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000088)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_DET_PLLCLK_HIGH_RMSK                                                    0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_DET_PLLCLK_HIGH_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_DET_PLLCLK_HIGH_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_DET_PLLCLK_HIGH_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_DET_PLLCLK_HIGH_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_DET_PLLCLK_HIGH_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_DET_PLLCLK_HIGH_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_DET_PLLCLK_HIGH_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_DET_PLLCLK_HIGH_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_DET_PLLCLK_HIGH_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_DET_PLLCLK_HIGH_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_DET_PLLCLK_HIGH_FD_PLL_HIGH_BMSK                                        0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_DET_PLLCLK_HIGH_FD_PLL_HIGH_SHFT                                         0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_DET_PLLCLK_LOW_ADDR                                               (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x0000008c)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_DET_PLLCLK_LOW_RMSK                                                     0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_DET_PLLCLK_LOW_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_DET_PLLCLK_LOW_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_DET_PLLCLK_LOW_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_DET_PLLCLK_LOW_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_DET_PLLCLK_LOW_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_DET_PLLCLK_LOW_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_DET_PLLCLK_LOW_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_DET_PLLCLK_LOW_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_DET_PLLCLK_LOW_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_DET_PLLCLK_LOW_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_DET_PLLCLK_LOW_FD_PLL_LOW_BMSK                                          0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_DET_PLLCLK_LOW_FD_PLL_LOW_SHFT                                           0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_PFILT_ADDR                                                             (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000090)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PFILT_RMSK                                                                   0x3f
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PFILT_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_PFILT_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_PFILT_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PFILT_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_PFILT_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PFILT_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_PFILT_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PFILT_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_PFILT_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_PFILT_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PFILT_CM_PLL_PFILT_MUX_BMSK                                                  0x20
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PFILT_CM_PLL_PFILT_MUX_SHFT                                                   0x5
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PFILT_CM_PLL_PFILT_BMSK                                                      0x1f
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PFILT_CM_PLL_PFILT_SHFT                                                       0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_IFILT_ADDR                                                             (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000094)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_IFILT_RMSK                                                                   0x3f
#define HWIO_DSI_0_PHY_DSIPHY_PLL_IFILT_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_IFILT_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_IFILT_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_IFILT_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_IFILT_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_IFILT_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_IFILT_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_IFILT_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_IFILT_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_IFILT_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_IFILT_CM_PLL_IFILT_MUX_BMSK                                                  0x20
#define HWIO_DSI_0_PHY_DSIPHY_PLL_IFILT_CM_PLL_IFILT_MUX_SHFT                                                   0x5
#define HWIO_DSI_0_PHY_DSIPHY_PLL_IFILT_CM_PLL_IFILT_BMSK                                                      0x1f
#define HWIO_DSI_0_PHY_DSIPHY_PLL_IFILT_CM_PLL_IFILT_SHFT                                                       0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_GAIN_ADDR                                                          (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000098)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_GAIN_RMSK                                                                0x7f
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_GAIN_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_GAIN_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_GAIN_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_GAIN_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_GAIN_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_GAIN_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_GAIN_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_GAIN_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_GAIN_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_GAIN_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_GAIN_CM_PLL_PROP_GAIN_BMSK                                               0x78
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_GAIN_CM_PLL_PROP_GAIN_SHFT                                                0x3
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_GAIN_CM_PLL_INT_GAIN_BMSK                                                 0x7
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_GAIN_CM_PLL_INT_GAIN_SHFT                                                 0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_ICODE_LOW_ADDR                                                         (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x0000009c)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ICODE_LOW_RMSK                                                               0x3f
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ICODE_LOW_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_ICODE_LOW_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_ICODE_LOW_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ICODE_LOW_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_ICODE_LOW_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ICODE_LOW_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_ICODE_LOW_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ICODE_LOW_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_ICODE_LOW_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_ICODE_LOW_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ICODE_LOW_CM_PLL_ICODE_LOW_MUX_BMSK                                          0x20
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ICODE_LOW_CM_PLL_ICODE_LOW_MUX_SHFT                                           0x5
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ICODE_LOW_CM_PLL_ICODE_LOW_BMSK                                              0x1f
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ICODE_LOW_CM_PLL_ICODE_LOW_SHFT                                               0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_ICODE_HIGH_ADDR                                                        (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x000000a0)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ICODE_HIGH_RMSK                                                              0x5f
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ICODE_HIGH_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_ICODE_HIGH_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_ICODE_HIGH_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ICODE_HIGH_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_ICODE_HIGH_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ICODE_HIGH_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_ICODE_HIGH_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ICODE_HIGH_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_ICODE_HIGH_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_ICODE_HIGH_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ICODE_HIGH_CM_PLL_ICODE_HIGH_MUX_BMSK                                        0x40
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ICODE_HIGH_CM_PLL_ICODE_HIGH_MUX_SHFT                                         0x6
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ICODE_HIGH_CM_PLL_ICODE_HIGH_BMSK                                            0x1f
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ICODE_HIGH_CM_PLL_ICODE_HIGH_SHFT                                             0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_LOCKDET_ADDR                                                           (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x000000a4)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_LOCKDET_RMSK                                                                 0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_LOCKDET_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_LOCKDET_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_LOCKDET_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_LOCKDET_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_LOCKDET_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_LOCKDET_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_LOCKDET_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_LOCKDET_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_LOCKDET_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_LOCKDET_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_LOCKDET_CM_PLL_LOCKDET_BMSK                                                  0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_LOCKDET_CM_PLL_LOCKDET_SHFT                                                   0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_OUTDIV_ADDR                                                            (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x000000a8)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_OUTDIV_RMSK                                                                   0x7
#define HWIO_DSI_0_PHY_DSIPHY_PLL_OUTDIV_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_OUTDIV_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_OUTDIV_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_OUTDIV_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_OUTDIV_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_OUTDIV_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_OUTDIV_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_OUTDIV_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_OUTDIV_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_OUTDIV_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_OUTDIV_CM_PLL_OUTDIV_MUX_BMSK                                                 0x4
#define HWIO_DSI_0_PHY_DSIPHY_PLL_OUTDIV_CM_PLL_OUTDIV_MUX_SHFT                                                 0x2
#define HWIO_DSI_0_PHY_DSIPHY_PLL_OUTDIV_CM_PLL_OUTDIV_BMSK                                                     0x3
#define HWIO_DSI_0_PHY_DSIPHY_PLL_OUTDIV_CM_PLL_OUTDIV_SHFT                                                     0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_FASTLOCK_CONTROL_ADDR                                                  (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x000000ac)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FASTLOCK_CONTROL_RMSK                                                        0x3f
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FASTLOCK_CONTROL_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_FASTLOCK_CONTROL_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_FASTLOCK_CONTROL_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FASTLOCK_CONTROL_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_FASTLOCK_CONTROL_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FASTLOCK_CONTROL_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_FASTLOCK_CONTROL_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FASTLOCK_CONTROL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_FASTLOCK_CONTROL_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_FASTLOCK_CONTROL_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FASTLOCK_CONTROL_CM_PLL_FASTLOCK_EN_MUX_BMSK                                 0x20
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FASTLOCK_CONTROL_CM_PLL_FASTLOCK_EN_MUX_SHFT                                  0x5
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FASTLOCK_CONTROL_CM_PLL_FASTLOCK_EN_BMSK                                     0x10
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FASTLOCK_CONTROL_CM_PLL_FASTLOCK_EN_SHFT                                      0x4
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FASTLOCK_CONTROL_CM_PLL_INT_GAIN_FASTLOCK_MUX_BMSK                            0x8
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FASTLOCK_CONTROL_CM_PLL_INT_GAIN_FASTLOCK_MUX_SHFT                            0x3
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FASTLOCK_CONTROL_CM_PLL_INT_GAIN_FASTLOCK_BMSK                                0x7
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FASTLOCK_CONTROL_CM_PLL_INT_GAIN_FASTLOCK_SHFT                                0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_PASS_OUT_OVERRIDE_ONE_ADDR                                             (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x000000b0)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PASS_OUT_OVERRIDE_ONE_RMSK                                                   0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PASS_OUT_OVERRIDE_ONE_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_PASS_OUT_OVERRIDE_ONE_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_PASS_OUT_OVERRIDE_ONE_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PASS_OUT_OVERRIDE_ONE_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_PASS_OUT_OVERRIDE_ONE_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PASS_OUT_OVERRIDE_ONE_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_PASS_OUT_OVERRIDE_ONE_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PASS_OUT_OVERRIDE_ONE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_PASS_OUT_OVERRIDE_ONE_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_PASS_OUT_OVERRIDE_ONE_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PASS_OUT_OVERRIDE_ONE_PASS_RES_CODE_OUT_MUX_BMSK                             0x80
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PASS_OUT_OVERRIDE_ONE_PASS_RES_CODE_OUT_MUX_SHFT                              0x7
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PASS_OUT_OVERRIDE_ONE_PASS_RES_CODE_OUT_BMSK                                 0x78
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PASS_OUT_OVERRIDE_ONE_PASS_RES_CODE_OUT_SHFT                                  0x3
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PASS_OUT_OVERRIDE_ONE_PASS_BAND_OUT_MUX_BMSK                                  0x4
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PASS_OUT_OVERRIDE_ONE_PASS_BAND_OUT_MUX_SHFT                                  0x2
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PASS_OUT_OVERRIDE_ONE_PASS_BAND_OUT_BMSK                                      0x3
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PASS_OUT_OVERRIDE_ONE_PASS_BAND_OUT_SHFT                                      0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_PASS_OUT_OVERRIDE_TWO_ADDR                                             (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x000000b4)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PASS_OUT_OVERRIDE_TWO_RMSK                                                    0x3
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PASS_OUT_OVERRIDE_TWO_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_PASS_OUT_OVERRIDE_TWO_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_PASS_OUT_OVERRIDE_TWO_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PASS_OUT_OVERRIDE_TWO_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_PASS_OUT_OVERRIDE_TWO_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PASS_OUT_OVERRIDE_TWO_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_PASS_OUT_OVERRIDE_TWO_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PASS_OUT_OVERRIDE_TWO_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_PASS_OUT_OVERRIDE_TWO_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_PASS_OUT_OVERRIDE_TWO_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PASS_OUT_OVERRIDE_TWO_PASS_LS_EN_OUT_MUX_BMSK                                 0x2
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PASS_OUT_OVERRIDE_TWO_PASS_LS_EN_OUT_MUX_SHFT                                 0x1
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PASS_OUT_OVERRIDE_TWO_PASS_LS_EN_OUT_BMSK                                     0x1
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PASS_OUT_OVERRIDE_TWO_PASS_LS_EN_OUT_SHFT                                     0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_CORE_OVERRIDE_ADDR                                                     (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x000000b8)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CORE_OVERRIDE_RMSK                                                           0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CORE_OVERRIDE_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_CORE_OVERRIDE_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_CORE_OVERRIDE_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CORE_OVERRIDE_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_CORE_OVERRIDE_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CORE_OVERRIDE_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_CORE_OVERRIDE_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CORE_OVERRIDE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_CORE_OVERRIDE_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_CORE_OVERRIDE_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CORE_OVERRIDE_UNLOCK_READY_MUX_BMSK                                          0x80
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CORE_OVERRIDE_UNLOCK_READY_MUX_SHFT                                           0x7
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CORE_OVERRIDE_UNLOCK_READY_BMSK                                              0x40
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CORE_OVERRIDE_UNLOCK_READY_SHFT                                               0x6
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CORE_OVERRIDE_PASS_PLL_READY_MUX_BMSK                                        0x20
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CORE_OVERRIDE_PASS_PLL_READY_MUX_SHFT                                         0x5
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CORE_OVERRIDE_PASS_PLL_READY_BMSK                                            0x10
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CORE_OVERRIDE_PASS_PLL_READY_SHFT                                             0x4
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CORE_OVERRIDE_CORE_PLL_UNLOCK_MUX_BMSK                                        0x8
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CORE_OVERRIDE_CORE_PLL_UNLOCK_MUX_SHFT                                        0x3
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CORE_OVERRIDE_CORE_PLL_UNLOCK_BMSK                                            0x4
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CORE_OVERRIDE_CORE_PLL_UNLOCK_SHFT                                            0x2
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CORE_OVERRIDE_CORE_READY_MUX_BMSK                                             0x2
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CORE_OVERRIDE_CORE_READY_MUX_SHFT                                             0x1
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CORE_OVERRIDE_CORE_READY_BMSK                                                 0x1
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CORE_OVERRIDE_CORE_READY_SHFT                                                 0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_CORE_INPUT_OVERRIDE_ADDR                                               (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x000000bc)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CORE_INPUT_OVERRIDE_RMSK                                                     0x7f
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CORE_INPUT_OVERRIDE_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_CORE_INPUT_OVERRIDE_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_CORE_INPUT_OVERRIDE_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CORE_INPUT_OVERRIDE_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_CORE_INPUT_OVERRIDE_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CORE_INPUT_OVERRIDE_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_CORE_INPUT_OVERRIDE_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CORE_INPUT_OVERRIDE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_CORE_INPUT_OVERRIDE_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_CORE_INPUT_OVERRIDE_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CORE_INPUT_OVERRIDE_CORE_RESET_MUX_BMSK                                      0x40
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CORE_INPUT_OVERRIDE_CORE_RESET_MUX_SHFT                                       0x6
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CORE_INPUT_OVERRIDE_CORE_RESET_BMSK                                          0x20
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CORE_INPUT_OVERRIDE_CORE_RESET_SHFT                                           0x5
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CORE_INPUT_OVERRIDE_CORE_PLL_EN_FROM_RESET_BMSK                              0x10
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CORE_INPUT_OVERRIDE_CORE_PLL_EN_FROM_RESET_SHFT                               0x4
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CORE_INPUT_OVERRIDE_CORE_PLL_EN_MUX_BMSK                                      0x8
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CORE_INPUT_OVERRIDE_CORE_PLL_EN_MUX_SHFT                                      0x3
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CORE_INPUT_OVERRIDE_CORE_PLL_EN_BMSK                                          0x4
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CORE_INPUT_OVERRIDE_CORE_PLL_EN_SHFT                                          0x2
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CORE_INPUT_OVERRIDE_CORE_PLL_RATE_MUX_BMSK                                    0x2
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CORE_INPUT_OVERRIDE_CORE_PLL_RATE_MUX_SHFT                                    0x1
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CORE_INPUT_OVERRIDE_CORE_PLL_RATE_BMSK                                        0x1
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CORE_INPUT_OVERRIDE_CORE_PLL_RATE_SHFT                                        0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_RATE_CHANGE_ADDR                                                       (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x000000c0)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_RATE_CHANGE_RMSK                                                             0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_RATE_CHANGE_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_RATE_CHANGE_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_RATE_CHANGE_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_RATE_CHANGE_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_RATE_CHANGE_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_RATE_CHANGE_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_RATE_CHANGE_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_RATE_CHANGE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_RATE_CHANGE_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_RATE_CHANGE_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_RATE_CHANGE_PPM_UNLOCK_TIMER_LSB_BMSK                                        0xfe
#define HWIO_DSI_0_PHY_DSIPHY_PLL_RATE_CHANGE_PPM_UNLOCK_TIMER_LSB_SHFT                                         0x1
#define HWIO_DSI_0_PHY_DSIPHY_PLL_RATE_CHANGE_PLL_RATE_CHANGE_BMSK                                              0x1
#define HWIO_DSI_0_PHY_DSIPHY_PLL_RATE_CHANGE_PLL_RATE_CHANGE_SHFT                                              0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_DIGITAL_TIMERS_ADDR                                                (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x000000c4)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_DIGITAL_TIMERS_RMSK                                                      0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_DIGITAL_TIMERS_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_DIGITAL_TIMERS_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_DIGITAL_TIMERS_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_DIGITAL_TIMERS_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_DIGITAL_TIMERS_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_DIGITAL_TIMERS_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_DIGITAL_TIMERS_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_DIGITAL_TIMERS_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_DIGITAL_TIMERS_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_DIGITAL_TIMERS_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_DIGITAL_TIMERS_PPM_UNLOCK_TIMER_SEL_BMSK                                 0x80
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_DIGITAL_TIMERS_PPM_UNLOCK_TIMER_SEL_SHFT                                  0x7
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_DIGITAL_TIMERS_PPM_UNLOCK_TIMER_MSB_BMSK                                 0x70
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_DIGITAL_TIMERS_PPM_UNLOCK_TIMER_MSB_SHFT                                  0x4
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_DIGITAL_TIMERS_RATE_CHANGE_SETTLE_BMSK                                    0xf
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_DIGITAL_TIMERS_RATE_CHANGE_SETTLE_SHFT                                    0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_DIGITAL_TIMERS_TWO_ADDR                                            (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x000000c8)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_DIGITAL_TIMERS_TWO_RMSK                                                  0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_DIGITAL_TIMERS_TWO_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_DIGITAL_TIMERS_TWO_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_DIGITAL_TIMERS_TWO_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_DIGITAL_TIMERS_TWO_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_DIGITAL_TIMERS_TWO_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_DIGITAL_TIMERS_TWO_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_DIGITAL_TIMERS_TWO_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_DIGITAL_TIMERS_TWO_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_DIGITAL_TIMERS_TWO_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_DIGITAL_TIMERS_TWO_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_DIGITAL_TIMERS_TWO_PLL_EN_TO_RESET_TIME_BMSK                             0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_DIGITAL_TIMERS_TWO_PLL_EN_TO_RESET_TIME_SHFT                              0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_DECIMAL_DIV_START_ADDR                                                 (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x000000cc)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_DECIMAL_DIV_START_RMSK                                                       0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_DECIMAL_DIV_START_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_DECIMAL_DIV_START_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_DECIMAL_DIV_START_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_DECIMAL_DIV_START_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_DECIMAL_DIV_START_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_DECIMAL_DIV_START_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_DECIMAL_DIV_START_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_DECIMAL_DIV_START_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_DECIMAL_DIV_START_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_DECIMAL_DIV_START_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_DECIMAL_DIV_START_PLL_DIV_DEC_START_BMSK                                     0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_DECIMAL_DIV_START_PLL_DIV_DEC_START_SHFT                                      0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_LOW_ADDR                                                (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x000000d0)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_LOW_RMSK                                                      0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_LOW_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_LOW_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_LOW_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_LOW_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_LOW_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_LOW_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_LOW_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_LOW_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_LOW_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_LOW_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_LOW_PLL_DIV_FRAC_START_LOW_BMSK                               0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_LOW_PLL_DIV_FRAC_START_LOW_SHFT                                0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_MID_ADDR                                                (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x000000d4)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_MID_RMSK                                                      0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_MID_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_MID_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_MID_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_MID_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_MID_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_MID_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_MID_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_MID_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_MID_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_MID_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_MID_PLL_DIV_FRAC_START_MID_BMSK                               0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_MID_PLL_DIV_FRAC_START_MID_SHFT                                0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_HIGH_ADDR                                               (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x000000d8)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_HIGH_RMSK                                                      0x3
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_HIGH_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_HIGH_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_HIGH_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_HIGH_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_HIGH_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_HIGH_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_HIGH_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_HIGH_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_HIGH_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_HIGH_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_HIGH_PLL_DIV_FRAC_START_HIGH_BMSK                              0x3
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_HIGH_PLL_DIV_FRAC_START_HIGH_SHFT                              0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_DEC_FRAC_MUXES_ADDR                                                    (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x000000dc)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_DEC_FRAC_MUXES_RMSK                                                           0xf
#define HWIO_DSI_0_PHY_DSIPHY_PLL_DEC_FRAC_MUXES_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_DEC_FRAC_MUXES_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_DEC_FRAC_MUXES_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_DEC_FRAC_MUXES_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_DEC_FRAC_MUXES_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_DEC_FRAC_MUXES_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_DEC_FRAC_MUXES_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_DEC_FRAC_MUXES_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_DEC_FRAC_MUXES_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_DEC_FRAC_MUXES_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_DEC_FRAC_MUXES_PLL_DIV_FRAC_START_HIGH_MUX_BMSK                               0x8
#define HWIO_DSI_0_PHY_DSIPHY_PLL_DEC_FRAC_MUXES_PLL_DIV_FRAC_START_HIGH_MUX_SHFT                               0x3
#define HWIO_DSI_0_PHY_DSIPHY_PLL_DEC_FRAC_MUXES_PLL_DIV_FRAC_START_MID_MUX_BMSK                                0x4
#define HWIO_DSI_0_PHY_DSIPHY_PLL_DEC_FRAC_MUXES_PLL_DIV_FRAC_START_MID_MUX_SHFT                                0x2
#define HWIO_DSI_0_PHY_DSIPHY_PLL_DEC_FRAC_MUXES_PLL_DIV_FRAC_START_LOW_MUX_BMSK                                0x2
#define HWIO_DSI_0_PHY_DSIPHY_PLL_DEC_FRAC_MUXES_PLL_DIV_FRAC_START_LOW_MUX_SHFT                                0x1
#define HWIO_DSI_0_PHY_DSIPHY_PLL_DEC_FRAC_MUXES_PLL_DIV_DEC_START_MUX_BMSK                                     0x1
#define HWIO_DSI_0_PHY_DSIPHY_PLL_DEC_FRAC_MUXES_PLL_DIV_DEC_START_MUX_SHFT                                     0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_DECIMAL_DIV_START_1_ADDR                                               (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x000000e0)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_DECIMAL_DIV_START_1_RMSK                                                     0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_DECIMAL_DIV_START_1_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_DECIMAL_DIV_START_1_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_DECIMAL_DIV_START_1_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_DECIMAL_DIV_START_1_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_DECIMAL_DIV_START_1_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_DECIMAL_DIV_START_1_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_DECIMAL_DIV_START_1_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_DECIMAL_DIV_START_1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_DECIMAL_DIV_START_1_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_DECIMAL_DIV_START_1_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_DECIMAL_DIV_START_1_PLL_DIV_DEC_START_1_BMSK                                 0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_DECIMAL_DIV_START_1_PLL_DIV_DEC_START_1_SHFT                                  0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_LOW_1_ADDR                                              (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x000000e4)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_LOW_1_RMSK                                                    0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_LOW_1_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_LOW_1_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_LOW_1_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_LOW_1_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_LOW_1_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_LOW_1_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_LOW_1_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_LOW_1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_LOW_1_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_LOW_1_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_LOW_1_PLL_DIV_FRAC_START_LOW_1_BMSK                           0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_LOW_1_PLL_DIV_FRAC_START_LOW_1_SHFT                            0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_MID_1_ADDR                                              (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x000000e8)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_MID_1_RMSK                                                    0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_MID_1_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_MID_1_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_MID_1_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_MID_1_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_MID_1_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_MID_1_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_MID_1_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_MID_1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_MID_1_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_MID_1_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_MID_1_PLL_DIV_FRAC_START_MID_1_BMSK                           0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_MID_1_PLL_DIV_FRAC_START_MID_1_SHFT                            0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_HIGH_1_ADDR                                             (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x000000ec)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_HIGH_1_RMSK                                                    0x3
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_HIGH_1_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_HIGH_1_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_HIGH_1_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_HIGH_1_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_HIGH_1_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_HIGH_1_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_HIGH_1_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_HIGH_1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_HIGH_1_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_HIGH_1_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_HIGH_1_PLL_DIV_FRAC_START_HIGH_1_BMSK                          0x3
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_HIGH_1_PLL_DIV_FRAC_START_HIGH_1_SHFT                          0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_DECIMAL_DIV_START_2_ADDR                                               (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x000000f0)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_DECIMAL_DIV_START_2_RMSK                                                     0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_DECIMAL_DIV_START_2_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_DECIMAL_DIV_START_2_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_DECIMAL_DIV_START_2_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_DECIMAL_DIV_START_2_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_DECIMAL_DIV_START_2_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_DECIMAL_DIV_START_2_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_DECIMAL_DIV_START_2_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_DECIMAL_DIV_START_2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_DECIMAL_DIV_START_2_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_DECIMAL_DIV_START_2_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_DECIMAL_DIV_START_2_PLL_DIV_DEC_START_2_BMSK                                 0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_DECIMAL_DIV_START_2_PLL_DIV_DEC_START_2_SHFT                                  0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_LOW_2_ADDR                                              (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x000000f4)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_LOW_2_RMSK                                                    0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_LOW_2_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_LOW_2_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_LOW_2_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_LOW_2_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_LOW_2_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_LOW_2_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_LOW_2_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_LOW_2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_LOW_2_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_LOW_2_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_LOW_2_PLL_DIV_FRAC_START_LOW_2_BMSK                           0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_LOW_2_PLL_DIV_FRAC_START_LOW_2_SHFT                            0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_MID_2_ADDR                                              (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x000000f8)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_MID_2_RMSK                                                    0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_MID_2_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_MID_2_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_MID_2_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_MID_2_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_MID_2_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_MID_2_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_MID_2_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_MID_2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_MID_2_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_MID_2_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_MID_2_PLL_DIV_FRAC_START_MID_2_BMSK                           0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_MID_2_PLL_DIV_FRAC_START_MID_2_SHFT                            0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_HIGH_2_ADDR                                             (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x000000fc)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_HIGH_2_RMSK                                                    0x3
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_HIGH_2_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_HIGH_2_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_HIGH_2_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_HIGH_2_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_HIGH_2_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_HIGH_2_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_HIGH_2_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_HIGH_2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_HIGH_2_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_HIGH_2_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_HIGH_2_PLL_DIV_FRAC_START_HIGH_2_BMSK                          0x3
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FRAC_DIV_START_HIGH_2_PLL_DIV_FRAC_START_HIGH_2_SHFT                          0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_MASH_CONTROL_ADDR                                                      (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000100)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_MASH_CONTROL_RMSK                                                             0x3
#define HWIO_DSI_0_PHY_DSIPHY_PLL_MASH_CONTROL_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_MASH_CONTROL_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_MASH_CONTROL_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_MASH_CONTROL_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_MASH_CONTROL_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_MASH_CONTROL_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_MASH_CONTROL_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_MASH_CONTROL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_MASH_CONTROL_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_MASH_CONTROL_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_MASH_CONTROL_PLL_DIV_ORD_BMSK                                                 0x2
#define HWIO_DSI_0_PHY_DSIPHY_PLL_MASH_CONTROL_PLL_DIV_ORD_SHFT                                                 0x1
#define HWIO_DSI_0_PHY_DSIPHY_PLL_MASH_CONTROL_PLL_DIV_FFEN_BMSK                                                0x1
#define HWIO_DSI_0_PHY_DSIPHY_PLL_MASH_CONTROL_PLL_DIV_FFEN_SHFT                                                0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_STEPSIZE_LOW_ADDR                                                  (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000104)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_STEPSIZE_LOW_RMSK                                                        0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_STEPSIZE_LOW_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_STEPSIZE_LOW_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_STEPSIZE_LOW_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_STEPSIZE_LOW_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_STEPSIZE_LOW_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_STEPSIZE_LOW_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_STEPSIZE_LOW_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_STEPSIZE_LOW_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_STEPSIZE_LOW_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_STEPSIZE_LOW_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_STEPSIZE_LOW_PLL_DIV_STEPSIZE_LOW_BMSK                                   0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_STEPSIZE_LOW_PLL_DIV_STEPSIZE_LOW_SHFT                                    0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_STEPSIZE_HIGH_ADDR                                                 (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000108)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_STEPSIZE_HIGH_RMSK                                                       0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_STEPSIZE_HIGH_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_STEPSIZE_HIGH_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_STEPSIZE_HIGH_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_STEPSIZE_HIGH_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_STEPSIZE_HIGH_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_STEPSIZE_HIGH_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_STEPSIZE_HIGH_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_STEPSIZE_HIGH_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_STEPSIZE_HIGH_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_STEPSIZE_HIGH_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_STEPSIZE_HIGH_PLL_DIV_STEPSIZE_HIGH_BMSK                                 0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_STEPSIZE_HIGH_PLL_DIV_STEPSIZE_HIGH_SHFT                                  0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_DIV_PER_LOW_ADDR                                                   (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x0000010c)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_DIV_PER_LOW_RMSK                                                         0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_DIV_PER_LOW_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_DIV_PER_LOW_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_DIV_PER_LOW_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_DIV_PER_LOW_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_DIV_PER_LOW_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_DIV_PER_LOW_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_DIV_PER_LOW_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_DIV_PER_LOW_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_DIV_PER_LOW_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_DIV_PER_LOW_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_DIV_PER_LOW_PLL_DIV_PER_LOW_BMSK                                         0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_DIV_PER_LOW_PLL_DIV_PER_LOW_SHFT                                          0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_DIV_PER_HIGH_ADDR                                                  (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000110)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_DIV_PER_HIGH_RMSK                                                        0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_DIV_PER_HIGH_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_DIV_PER_HIGH_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_DIV_PER_HIGH_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_DIV_PER_HIGH_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_DIV_PER_HIGH_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_DIV_PER_HIGH_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_DIV_PER_HIGH_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_DIV_PER_HIGH_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_DIV_PER_HIGH_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_DIV_PER_HIGH_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_DIV_PER_HIGH_PLL_DIV_PER_HIGH_BMSK                                       0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_DIV_PER_HIGH_PLL_DIV_PER_HIGH_SHFT                                        0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_ADJPER_LOW_ADDR                                                    (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000114)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_ADJPER_LOW_RMSK                                                          0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_ADJPER_LOW_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_ADJPER_LOW_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_ADJPER_LOW_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_ADJPER_LOW_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_ADJPER_LOW_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_ADJPER_LOW_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_ADJPER_LOW_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_ADJPER_LOW_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_ADJPER_LOW_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_ADJPER_LOW_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_ADJPER_LOW_PLL_DIV_ADJPER_LOW_BMSK                                       0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_ADJPER_LOW_PLL_DIV_ADJPER_LOW_SHFT                                        0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_ADJPER_HIGH_ADDR                                                   (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000118)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_ADJPER_HIGH_RMSK                                                          0x3
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_ADJPER_HIGH_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_ADJPER_HIGH_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_ADJPER_HIGH_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_ADJPER_HIGH_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_ADJPER_HIGH_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_ADJPER_HIGH_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_ADJPER_HIGH_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_ADJPER_HIGH_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_ADJPER_HIGH_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_ADJPER_HIGH_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_ADJPER_HIGH_PLL_DIV_ADJPER_HIGH_BMSK                                      0x3
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_ADJPER_HIGH_PLL_DIV_ADJPER_HIGH_SHFT                                      0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_MUX_CONTROL_ADDR                                                   (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x0000011c)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_MUX_CONTROL_RMSK                                                         0x3f
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_MUX_CONTROL_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_MUX_CONTROL_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_MUX_CONTROL_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_MUX_CONTROL_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_MUX_CONTROL_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_MUX_CONTROL_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_MUX_CONTROL_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_MUX_CONTROL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_MUX_CONTROL_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_MUX_CONTROL_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_MUX_CONTROL_PLL_DIV_ADJPER_HIGH_MUX_BMSK                                 0x20
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_MUX_CONTROL_PLL_DIV_ADJPER_HIGH_MUX_SHFT                                  0x5
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_MUX_CONTROL_PLL_DIV_ADJPER_LOW_MUX_BMSK                                  0x10
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_MUX_CONTROL_PLL_DIV_ADJPER_LOW_MUX_SHFT                                   0x4
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_MUX_CONTROL_PLL_DIV_PER_HIGH_MUX_BMSK                                     0x8
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_MUX_CONTROL_PLL_DIV_PER_HIGH_MUX_SHFT                                     0x3
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_MUX_CONTROL_PLL_DIV_PER_LOW_MUX_BMSK                                      0x4
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_MUX_CONTROL_PLL_DIV_PER_LOW_MUX_SHFT                                      0x2
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_MUX_CONTROL_PLL_DIV_STEPSIZE_HIGH_MUX_BMSK                                0x2
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_MUX_CONTROL_PLL_DIV_STEPSIZE_HIGH_MUX_SHFT                                0x1
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_MUX_CONTROL_PLL_DIV_STEPSIZE_LOW_MUX_BMSK                                 0x1
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_MUX_CONTROL_PLL_DIV_STEPSIZE_LOW_MUX_SHFT                                 0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_STEPSIZE_LOW_1_ADDR                                                (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000120)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_STEPSIZE_LOW_1_RMSK                                                      0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_STEPSIZE_LOW_1_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_STEPSIZE_LOW_1_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_STEPSIZE_LOW_1_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_STEPSIZE_LOW_1_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_STEPSIZE_LOW_1_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_STEPSIZE_LOW_1_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_STEPSIZE_LOW_1_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_STEPSIZE_LOW_1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_STEPSIZE_LOW_1_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_STEPSIZE_LOW_1_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_STEPSIZE_LOW_1_PLL_DIV_STEPSIZE_LOW_1_BMSK                               0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_STEPSIZE_LOW_1_PLL_DIV_STEPSIZE_LOW_1_SHFT                                0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_STEPSIZE_HIGH_1_ADDR                                               (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000124)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_STEPSIZE_HIGH_1_RMSK                                                     0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_STEPSIZE_HIGH_1_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_STEPSIZE_HIGH_1_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_STEPSIZE_HIGH_1_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_STEPSIZE_HIGH_1_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_STEPSIZE_HIGH_1_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_STEPSIZE_HIGH_1_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_STEPSIZE_HIGH_1_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_STEPSIZE_HIGH_1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_STEPSIZE_HIGH_1_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_STEPSIZE_HIGH_1_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_STEPSIZE_HIGH_1_PLL_DIV_STEPSIZE_HIGH_1_BMSK                             0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_STEPSIZE_HIGH_1_PLL_DIV_STEPSIZE_HIGH_1_SHFT                              0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_DIV_PER_LOW_1_ADDR                                                 (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000128)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_DIV_PER_LOW_1_RMSK                                                       0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_DIV_PER_LOW_1_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_DIV_PER_LOW_1_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_DIV_PER_LOW_1_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_DIV_PER_LOW_1_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_DIV_PER_LOW_1_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_DIV_PER_LOW_1_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_DIV_PER_LOW_1_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_DIV_PER_LOW_1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_DIV_PER_LOW_1_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_DIV_PER_LOW_1_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_DIV_PER_LOW_1_PLL_DIV_PER_LOW_1_BMSK                                     0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_DIV_PER_LOW_1_PLL_DIV_PER_LOW_1_SHFT                                      0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_DIV_PER_HIGH_1_ADDR                                                (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x0000012c)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_DIV_PER_HIGH_1_RMSK                                                      0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_DIV_PER_HIGH_1_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_DIV_PER_HIGH_1_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_DIV_PER_HIGH_1_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_DIV_PER_HIGH_1_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_DIV_PER_HIGH_1_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_DIV_PER_HIGH_1_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_DIV_PER_HIGH_1_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_DIV_PER_HIGH_1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_DIV_PER_HIGH_1_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_DIV_PER_HIGH_1_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_DIV_PER_HIGH_1_PLL_DIV_PER_HIGH_1_BMSK                                   0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_DIV_PER_HIGH_1_PLL_DIV_PER_HIGH_1_SHFT                                    0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_ADJPER_LOW_1_ADDR                                                  (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000130)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_ADJPER_LOW_1_RMSK                                                        0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_ADJPER_LOW_1_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_ADJPER_LOW_1_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_ADJPER_LOW_1_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_ADJPER_LOW_1_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_ADJPER_LOW_1_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_ADJPER_LOW_1_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_ADJPER_LOW_1_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_ADJPER_LOW_1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_ADJPER_LOW_1_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_ADJPER_LOW_1_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_ADJPER_LOW_1_PLL_DIV_ADJPER_LOW_1_BMSK                                   0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_ADJPER_LOW_1_PLL_DIV_ADJPER_LOW_1_SHFT                                    0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_ADJPER_HIGH_1_ADDR                                                 (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000134)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_ADJPER_HIGH_1_RMSK                                                        0x3
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_ADJPER_HIGH_1_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_ADJPER_HIGH_1_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_ADJPER_HIGH_1_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_ADJPER_HIGH_1_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_ADJPER_HIGH_1_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_ADJPER_HIGH_1_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_ADJPER_HIGH_1_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_ADJPER_HIGH_1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_ADJPER_HIGH_1_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_ADJPER_HIGH_1_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_ADJPER_HIGH_1_PLL_DIV_ADJPER_HIGH_1_BMSK                                  0x3
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_ADJPER_HIGH_1_PLL_DIV_ADJPER_HIGH_1_SHFT                                  0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_STEPSIZE_LOW_2_ADDR                                                (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000138)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_STEPSIZE_LOW_2_RMSK                                                      0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_STEPSIZE_LOW_2_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_STEPSIZE_LOW_2_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_STEPSIZE_LOW_2_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_STEPSIZE_LOW_2_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_STEPSIZE_LOW_2_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_STEPSIZE_LOW_2_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_STEPSIZE_LOW_2_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_STEPSIZE_LOW_2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_STEPSIZE_LOW_2_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_STEPSIZE_LOW_2_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_STEPSIZE_LOW_2_PLL_DIV_STEPSIZE_LOW_2_BMSK                               0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_STEPSIZE_LOW_2_PLL_DIV_STEPSIZE_LOW_2_SHFT                                0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_STEPSIZE_HIGH_2_ADDR                                               (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x0000013c)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_STEPSIZE_HIGH_2_RMSK                                                     0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_STEPSIZE_HIGH_2_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_STEPSIZE_HIGH_2_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_STEPSIZE_HIGH_2_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_STEPSIZE_HIGH_2_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_STEPSIZE_HIGH_2_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_STEPSIZE_HIGH_2_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_STEPSIZE_HIGH_2_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_STEPSIZE_HIGH_2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_STEPSIZE_HIGH_2_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_STEPSIZE_HIGH_2_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_STEPSIZE_HIGH_2_PLL_DIV_STEPSIZE_HIGH_2_BMSK                             0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_STEPSIZE_HIGH_2_PLL_DIV_STEPSIZE_HIGH_2_SHFT                              0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_DIV_PER_LOW_2_ADDR                                                 (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000140)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_DIV_PER_LOW_2_RMSK                                                       0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_DIV_PER_LOW_2_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_DIV_PER_LOW_2_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_DIV_PER_LOW_2_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_DIV_PER_LOW_2_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_DIV_PER_LOW_2_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_DIV_PER_LOW_2_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_DIV_PER_LOW_2_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_DIV_PER_LOW_2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_DIV_PER_LOW_2_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_DIV_PER_LOW_2_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_DIV_PER_LOW_2_PLL_DIV_PER_LOW_2_BMSK                                     0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_DIV_PER_LOW_2_PLL_DIV_PER_LOW_2_SHFT                                      0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_DIV_PER_HIGH_2_ADDR                                                (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000144)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_DIV_PER_HIGH_2_RMSK                                                      0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_DIV_PER_HIGH_2_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_DIV_PER_HIGH_2_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_DIV_PER_HIGH_2_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_DIV_PER_HIGH_2_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_DIV_PER_HIGH_2_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_DIV_PER_HIGH_2_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_DIV_PER_HIGH_2_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_DIV_PER_HIGH_2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_DIV_PER_HIGH_2_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_DIV_PER_HIGH_2_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_DIV_PER_HIGH_2_PLL_DIV_PER_HIGH_2_BMSK                                   0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_DIV_PER_HIGH_2_PLL_DIV_PER_HIGH_2_SHFT                                    0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_ADJPER_LOW_2_ADDR                                                  (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000148)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_ADJPER_LOW_2_RMSK                                                        0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_ADJPER_LOW_2_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_ADJPER_LOW_2_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_ADJPER_LOW_2_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_ADJPER_LOW_2_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_ADJPER_LOW_2_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_ADJPER_LOW_2_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_ADJPER_LOW_2_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_ADJPER_LOW_2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_ADJPER_LOW_2_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_ADJPER_LOW_2_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_ADJPER_LOW_2_PLL_DIV_ADJPER_LOW_2_BMSK                                   0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_ADJPER_LOW_2_PLL_DIV_ADJPER_LOW_2_SHFT                                    0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_ADJPER_HIGH_2_ADDR                                                 (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x0000014c)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_ADJPER_HIGH_2_RMSK                                                        0x3
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_ADJPER_HIGH_2_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_ADJPER_HIGH_2_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_ADJPER_HIGH_2_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_ADJPER_HIGH_2_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_ADJPER_HIGH_2_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_ADJPER_HIGH_2_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_ADJPER_HIGH_2_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_ADJPER_HIGH_2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_ADJPER_HIGH_2_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_ADJPER_HIGH_2_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_ADJPER_HIGH_2_PLL_DIV_ADJPER_HIGH_2_BMSK                                  0x3
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_ADJPER_HIGH_2_PLL_DIV_ADJPER_HIGH_2_SHFT                                  0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_CONTROL_ADDR                                                       (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000150)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_CONTROL_RMSK                                                             0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_CONTROL_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_CONTROL_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_CONTROL_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_CONTROL_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_CONTROL_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_CONTROL_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_CONTROL_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_CONTROL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_CONTROL_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_CONTROL_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_CONTROL_SSCSTART_MUX_BMSK                                                0x80
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_CONTROL_SSCSTART_MUX_SHFT                                                 0x7
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_CONTROL_SSCSTART_BMSK                                                    0x40
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_CONTROL_SSCSTART_SHFT                                                     0x6
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_CONTROL_UPDATE_SSC_MUX_BMSK                                              0x20
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_CONTROL_UPDATE_SSC_MUX_SHFT                                               0x5
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_CONTROL_UPDATE_SSC_BMSK                                                  0x10
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_CONTROL_UPDATE_SSC_SHFT                                                   0x4
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_CONTROL_FREQ_UPDATE_MUX_BMSK                                              0x8
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_CONTROL_FREQ_UPDATE_MUX_SHFT                                              0x3
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_CONTROL_FREQ_UPDATE_BMSK                                                  0x4
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_CONTROL_FREQ_UPDATE_SHFT                                                  0x2
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_CONTROL_SSCEN_BMSK                                                        0x2
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_CONTROL_SSCEN_SHFT                                                        0x1
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_CONTROL_SSCCENTER_BMSK                                                    0x1
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SSC_CONTROL_SSCCENTER_SHFT                                                    0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_OUTDIV_RATE_ADDR                                                   (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000154)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_OUTDIV_RATE_RMSK                                                          0xf
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_OUTDIV_RATE_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_OUTDIV_RATE_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_OUTDIV_RATE_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_OUTDIV_RATE_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_OUTDIV_RATE_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_OUTDIV_RATE_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_OUTDIV_RATE_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_OUTDIV_RATE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_OUTDIV_RATE_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_OUTDIV_RATE_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_OUTDIV_RATE_PLL_OUTDIV_2_BMSK                                             0xc
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_OUTDIV_RATE_PLL_OUTDIV_2_SHFT                                             0x2
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_OUTDIV_RATE_PLL_OUTDIV_1_BMSK                                             0x3
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_OUTDIV_RATE_PLL_OUTDIV_1_SHFT                                             0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_LOCKDET_RATE_1_ADDR                                                (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000158)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_LOCKDET_RATE_1_RMSK                                                      0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_LOCKDET_RATE_1_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_LOCKDET_RATE_1_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_LOCKDET_RATE_1_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_LOCKDET_RATE_1_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_LOCKDET_RATE_1_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_LOCKDET_RATE_1_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_LOCKDET_RATE_1_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_LOCKDET_RATE_1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_LOCKDET_RATE_1_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_LOCKDET_RATE_1_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_LOCKDET_RATE_1_PLL_LOCKDET_1_BMSK                                        0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_LOCKDET_RATE_1_PLL_LOCKDET_1_SHFT                                         0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_LOCKDET_RATE_2_ADDR                                                (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x0000015c)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_LOCKDET_RATE_2_RMSK                                                      0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_LOCKDET_RATE_2_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_LOCKDET_RATE_2_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_LOCKDET_RATE_2_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_LOCKDET_RATE_2_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_LOCKDET_RATE_2_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_LOCKDET_RATE_2_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_LOCKDET_RATE_2_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_LOCKDET_RATE_2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_LOCKDET_RATE_2_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_LOCKDET_RATE_2_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_LOCKDET_RATE_2_PLL_LOCKDET_2_BMSK                                        0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_LOCKDET_RATE_2_PLL_LOCKDET_2_SHFT                                         0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_PROP_GAIN_RATE_1_ADDR                                              (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000160)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_PROP_GAIN_RATE_1_RMSK                                                     0xf
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_PROP_GAIN_RATE_1_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_PROP_GAIN_RATE_1_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_PROP_GAIN_RATE_1_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_PROP_GAIN_RATE_1_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_PROP_GAIN_RATE_1_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_PROP_GAIN_RATE_1_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_PROP_GAIN_RATE_1_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_PROP_GAIN_RATE_1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_PROP_GAIN_RATE_1_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_PROP_GAIN_RATE_1_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_PROP_GAIN_RATE_1_PLL_PROP_GAIN_1_BMSK                                     0xf
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_PROP_GAIN_RATE_1_PLL_PROP_GAIN_1_SHFT                                     0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_PROP_GAIN_RATE_2_ADDR                                              (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000164)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_PROP_GAIN_RATE_2_RMSK                                                     0xf
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_PROP_GAIN_RATE_2_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_PROP_GAIN_RATE_2_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_PROP_GAIN_RATE_2_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_PROP_GAIN_RATE_2_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_PROP_GAIN_RATE_2_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_PROP_GAIN_RATE_2_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_PROP_GAIN_RATE_2_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_PROP_GAIN_RATE_2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_PROP_GAIN_RATE_2_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_PROP_GAIN_RATE_2_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_PROP_GAIN_RATE_2_PLL_PROP_GAIN_2_BMSK                                     0xf
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_PROP_GAIN_RATE_2_PLL_PROP_GAIN_2_SHFT                                     0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_BAND_SEL_RATE_1_ADDR                                               (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000168)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_BAND_SEL_RATE_1_RMSK                                                     0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_BAND_SEL_RATE_1_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_BAND_SEL_RATE_1_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_BAND_SEL_RATE_1_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_BAND_SEL_RATE_1_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_BAND_SEL_RATE_1_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_BAND_SEL_RATE_1_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_BAND_SEL_RATE_1_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_BAND_SEL_RATE_1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_BAND_SEL_RATE_1_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_BAND_SEL_RATE_1_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_BAND_SEL_RATE_1_PLL_BAND_SEL_1_BMSK                                      0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_BAND_SEL_RATE_1_PLL_BAND_SEL_1_SHFT                                       0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_BAND_SEL_RATE_2_ADDR                                               (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x0000016c)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_BAND_SEL_RATE_2_RMSK                                                     0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_BAND_SEL_RATE_2_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_BAND_SEL_RATE_2_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_BAND_SEL_RATE_2_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_BAND_SEL_RATE_2_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_BAND_SEL_RATE_2_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_BAND_SEL_RATE_2_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_BAND_SEL_RATE_2_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_BAND_SEL_RATE_2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_BAND_SEL_RATE_2_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_BAND_SEL_RATE_2_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_BAND_SEL_RATE_2_PLL_BAND_SEL_2_BMSK                                      0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_BAND_SEL_RATE_2_PLL_BAND_SEL_2_SHFT                                       0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_INT_GAIN_IFILT_BAND_1_ADDR                                         (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000170)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_INT_GAIN_IFILT_BAND_1_RMSK                                               0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_INT_GAIN_IFILT_BAND_1_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_INT_GAIN_IFILT_BAND_1_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_INT_GAIN_IFILT_BAND_1_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_INT_GAIN_IFILT_BAND_1_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_INT_GAIN_IFILT_BAND_1_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_INT_GAIN_IFILT_BAND_1_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_INT_GAIN_IFILT_BAND_1_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_INT_GAIN_IFILT_BAND_1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_INT_GAIN_IFILT_BAND_1_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_INT_GAIN_IFILT_BAND_1_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_INT_GAIN_IFILT_BAND_1_PLL_IFILT_1_BMSK                                   0xf8
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_INT_GAIN_IFILT_BAND_1_PLL_IFILT_1_SHFT                                    0x3
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_INT_GAIN_IFILT_BAND_1_PLL_INT_GAIN_1_BMSK                                 0x7
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_INT_GAIN_IFILT_BAND_1_PLL_INT_GAIN_1_SHFT                                 0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_INT_GAIN_IFILT_BAND_2_ADDR                                         (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000174)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_INT_GAIN_IFILT_BAND_2_RMSK                                               0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_INT_GAIN_IFILT_BAND_2_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_INT_GAIN_IFILT_BAND_2_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_INT_GAIN_IFILT_BAND_2_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_INT_GAIN_IFILT_BAND_2_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_INT_GAIN_IFILT_BAND_2_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_INT_GAIN_IFILT_BAND_2_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_INT_GAIN_IFILT_BAND_2_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_INT_GAIN_IFILT_BAND_2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_INT_GAIN_IFILT_BAND_2_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_INT_GAIN_IFILT_BAND_2_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_INT_GAIN_IFILT_BAND_2_PLL_IFILT_2_BMSK                                   0xf8
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_INT_GAIN_IFILT_BAND_2_PLL_IFILT_2_SHFT                                    0x3
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_INT_GAIN_IFILT_BAND_2_PLL_INT_GAIN_2_BMSK                                 0x7
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_INT_GAIN_IFILT_BAND_2_PLL_INT_GAIN_2_SHFT                                 0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_FL_INT_GAIN_PFILT_BAND_1_ADDR                                      (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000178)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_FL_INT_GAIN_PFILT_BAND_1_RMSK                                            0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_FL_INT_GAIN_PFILT_BAND_1_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_FL_INT_GAIN_PFILT_BAND_1_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_FL_INT_GAIN_PFILT_BAND_1_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_FL_INT_GAIN_PFILT_BAND_1_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_FL_INT_GAIN_PFILT_BAND_1_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_FL_INT_GAIN_PFILT_BAND_1_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_FL_INT_GAIN_PFILT_BAND_1_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_FL_INT_GAIN_PFILT_BAND_1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_FL_INT_GAIN_PFILT_BAND_1_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_FL_INT_GAIN_PFILT_BAND_1_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_FL_INT_GAIN_PFILT_BAND_1_PLL_PFILT_1_BMSK                                0xf8
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_FL_INT_GAIN_PFILT_BAND_1_PLL_PFILT_1_SHFT                                 0x3
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_FL_INT_GAIN_PFILT_BAND_1_PLL_INT_GAIN_FASTLOCK_1_BMSK                     0x7
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_FL_INT_GAIN_PFILT_BAND_1_PLL_INT_GAIN_FASTLOCK_1_SHFT                     0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_FL_INT_GAIN_PFILT_BAND_2_ADDR                                      (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x0000017c)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_FL_INT_GAIN_PFILT_BAND_2_RMSK                                            0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_FL_INT_GAIN_PFILT_BAND_2_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_FL_INT_GAIN_PFILT_BAND_2_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_FL_INT_GAIN_PFILT_BAND_2_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_FL_INT_GAIN_PFILT_BAND_2_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_FL_INT_GAIN_PFILT_BAND_2_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_FL_INT_GAIN_PFILT_BAND_2_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_FL_INT_GAIN_PFILT_BAND_2_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_FL_INT_GAIN_PFILT_BAND_2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_FL_INT_GAIN_PFILT_BAND_2_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_FL_INT_GAIN_PFILT_BAND_2_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_FL_INT_GAIN_PFILT_BAND_2_PLL_PFILT_2_BMSK                                0xf8
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_FL_INT_GAIN_PFILT_BAND_2_PLL_PFILT_2_SHFT                                 0x3
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_FL_INT_GAIN_PFILT_BAND_2_PLL_INT_GAIN_FASTLOCK_2_BMSK                     0x7
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_FL_INT_GAIN_PFILT_BAND_2_PLL_INT_GAIN_FASTLOCK_2_SHFT                     0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_FASTLOCK_EN_BAND_ADDR                                              (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000180)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_FASTLOCK_EN_BAND_RMSK                                                     0x3
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_FASTLOCK_EN_BAND_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_FASTLOCK_EN_BAND_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_FASTLOCK_EN_BAND_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_FASTLOCK_EN_BAND_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_FASTLOCK_EN_BAND_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_FASTLOCK_EN_BAND_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_FASTLOCK_EN_BAND_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_FASTLOCK_EN_BAND_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_FASTLOCK_EN_BAND_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_FASTLOCK_EN_BAND_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_FASTLOCK_EN_BAND_PLL_FASTLOCK_EN_2_BMSK                                   0x2
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_FASTLOCK_EN_BAND_PLL_FASTLOCK_EN_2_SHFT                                   0x1
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_FASTLOCK_EN_BAND_PLL_FASTLOCK_EN_1_BMSK                                   0x1
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_FASTLOCK_EN_BAND_PLL_FASTLOCK_EN_1_SHFT                                   0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_TUNE_ACCUM_INIT_MID_ADDR                                          (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000184)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_TUNE_ACCUM_INIT_MID_RMSK                                                0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_TUNE_ACCUM_INIT_MID_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_TUNE_ACCUM_INIT_MID_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_TUNE_ACCUM_INIT_MID_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_TUNE_ACCUM_INIT_MID_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_TUNE_ACCUM_INIT_MID_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_TUNE_ACCUM_INIT_MID_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_TUNE_ACCUM_INIT_MID_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_TUNE_ACCUM_INIT_MID_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_TUNE_ACCUM_INIT_MID_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_TUNE_ACCUM_INIT_MID_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_TUNE_ACCUM_INIT_MID_FTUNE_CODE_MID_BMSK                                 0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_TUNE_ACCUM_INIT_MID_FTUNE_CODE_MID_SHFT                                  0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_TUNE_ACCUM_INIT_HIGH_ADDR                                         (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000188)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_TUNE_ACCUM_INIT_HIGH_RMSK                                               0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_TUNE_ACCUM_INIT_HIGH_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_TUNE_ACCUM_INIT_HIGH_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_TUNE_ACCUM_INIT_HIGH_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_TUNE_ACCUM_INIT_HIGH_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_TUNE_ACCUM_INIT_HIGH_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_TUNE_ACCUM_INIT_HIGH_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_TUNE_ACCUM_INIT_HIGH_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_TUNE_ACCUM_INIT_HIGH_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_TUNE_ACCUM_INIT_HIGH_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_TUNE_ACCUM_INIT_HIGH_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_TUNE_ACCUM_INIT_HIGH_FTUNE_CODE_HIGH_BMSK                               0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_TUNE_ACCUM_INIT_HIGH_FTUNE_CODE_HIGH_SHFT                                0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_TUNE_ACCUM_INIT_MUX_ADDR                                          (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x0000018c)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_TUNE_ACCUM_INIT_MUX_RMSK                                                 0x7
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_TUNE_ACCUM_INIT_MUX_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_TUNE_ACCUM_INIT_MUX_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_TUNE_ACCUM_INIT_MUX_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_TUNE_ACCUM_INIT_MUX_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_TUNE_ACCUM_INIT_MUX_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_TUNE_ACCUM_INIT_MUX_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_TUNE_ACCUM_INIT_MUX_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_TUNE_ACCUM_INIT_MUX_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_TUNE_ACCUM_INIT_MUX_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_TUNE_ACCUM_INIT_MUX_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_TUNE_ACCUM_INIT_MUX_FTUNE_CODE_HIGH_MUX_BMSK                             0x4
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_TUNE_ACCUM_INIT_MUX_FTUNE_CODE_HIGH_MUX_SHFT                             0x2
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_TUNE_ACCUM_INIT_MUX_FTUNE_CODE_MID_MUX_BMSK                              0x2
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_TUNE_ACCUM_INIT_MUX_FTUNE_CODE_MID_MUX_SHFT                              0x1
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_TUNE_ACCUM_INIT_MUX_RSVD_BMSK                                            0x1
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FREQ_TUNE_ACCUM_INIT_MUX_RSVD_SHFT                                            0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_LOCK_OVERRIDE_ADDR                                                 (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000190)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_LOCK_OVERRIDE_RMSK                                                       0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_LOCK_OVERRIDE_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_LOCK_OVERRIDE_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_LOCK_OVERRIDE_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_LOCK_OVERRIDE_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_LOCK_OVERRIDE_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_LOCK_OVERRIDE_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_LOCK_OVERRIDE_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_LOCK_OVERRIDE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_LOCK_OVERRIDE_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_LOCK_OVERRIDE_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_LOCK_OVERRIDE_STICKY_LOCK_BMSK                                           0x80
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_LOCK_OVERRIDE_STICKY_LOCK_SHFT                                            0x7
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_LOCK_OVERRIDE_CM_PLL_LOCK_MUX_BMSK                                       0x40
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_LOCK_OVERRIDE_CM_PLL_LOCK_MUX_SHFT                                        0x6
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_LOCK_OVERRIDE_CM_PLL_LOCK_BMSK                                           0x20
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_LOCK_OVERRIDE_CM_PLL_LOCK_SHFT                                            0x5
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_LOCK_OVERRIDE_PLL_LOCK_SELECT_BMSK                                       0x10
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_LOCK_OVERRIDE_PLL_LOCK_SELECT_SHFT                                        0x4
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_LOCK_OVERRIDE_PLL_RELOCK_MUX_BMSK                                         0x8
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_LOCK_OVERRIDE_PLL_RELOCK_MUX_SHFT                                         0x3
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_LOCK_OVERRIDE_PLL_RELOCK_BMSK                                             0x4
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_LOCK_OVERRIDE_PLL_RELOCK_SHFT                                             0x2
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_LOCK_OVERRIDE_CM_PLL_LOCKOUT_OUT_MUX_BMSK                                 0x2
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_LOCK_OVERRIDE_CM_PLL_LOCKOUT_OUT_MUX_SHFT                                 0x1
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_LOCK_OVERRIDE_CM_PLL_LOCKOUT_OUT_BMSK                                     0x1
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_LOCK_OVERRIDE_CM_PLL_LOCKOUT_OUT_SHFT                                     0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_LOCK_DELAY_ADDR                                                    (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000194)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_LOCK_DELAY_RMSK                                                          0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_LOCK_DELAY_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_LOCK_DELAY_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_LOCK_DELAY_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_LOCK_DELAY_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_LOCK_DELAY_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_LOCK_DELAY_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_LOCK_DELAY_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_LOCK_DELAY_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_LOCK_DELAY_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_LOCK_DELAY_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_LOCK_DELAY_LOCK_DELAY_BMSK                                               0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_LOCK_DELAY_LOCK_DELAY_SHFT                                                0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_LOCK_MIN_DELAY_ADDR                                                (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000198)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_LOCK_MIN_DELAY_RMSK                                                      0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_LOCK_MIN_DELAY_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_LOCK_MIN_DELAY_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_LOCK_MIN_DELAY_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_LOCK_MIN_DELAY_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_LOCK_MIN_DELAY_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_LOCK_MIN_DELAY_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_LOCK_MIN_DELAY_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_LOCK_MIN_DELAY_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_LOCK_MIN_DELAY_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_LOCK_MIN_DELAY_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_LOCK_MIN_DELAY_LOCK_MIN_DLY_BMSK                                         0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_LOCK_MIN_DELAY_LOCK_MIN_DLY_SHFT                                          0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_CLOCK_INVERTERS_ADDR                                                   (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x0000019c)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CLOCK_INVERTERS_RMSK                                                         0x7b
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CLOCK_INVERTERS_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_CLOCK_INVERTERS_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_CLOCK_INVERTERS_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CLOCK_INVERTERS_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_CLOCK_INVERTERS_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CLOCK_INVERTERS_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_CLOCK_INVERTERS_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CLOCK_INVERTERS_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_CLOCK_INVERTERS_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_CLOCK_INVERTERS_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CLOCK_INVERTERS_CM_OSCBIAS_ADJ_BMSK                                          0x60
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CLOCK_INVERTERS_CM_OSCBIAS_ADJ_SHFT                                           0x5
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CLOCK_INVERTERS_CM_OSCCAP_EN_BMSK                                            0x18
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CLOCK_INVERTERS_CM_OSCCAP_EN_SHFT                                             0x3
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CLOCK_INVERTERS_INVERT_DSMCLK_BMSK                                            0x2
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CLOCK_INVERTERS_INVERT_DSMCLK_SHFT                                            0x1
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CLOCK_INVERTERS_INVERT_FRACNCLK_BMSK                                          0x1
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CLOCK_INVERTERS_INVERT_FRACNCLK_SHFT                                          0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_SPARE_AND_JPC_OVERRIDES_ADDR                                           (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x000001a0)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SPARE_AND_JPC_OVERRIDES_RMSK                                                  0xf
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SPARE_AND_JPC_OVERRIDES_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_SPARE_AND_JPC_OVERRIDES_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_SPARE_AND_JPC_OVERRIDES_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SPARE_AND_JPC_OVERRIDES_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_SPARE_AND_JPC_OVERRIDES_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SPARE_AND_JPC_OVERRIDES_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_SPARE_AND_JPC_OVERRIDES_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SPARE_AND_JPC_OVERRIDES_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_SPARE_AND_JPC_OVERRIDES_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_SPARE_AND_JPC_OVERRIDES_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SPARE_AND_JPC_OVERRIDES_CM_PLL_SPARE_A_3_BMSK                                 0x8
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SPARE_AND_JPC_OVERRIDES_CM_PLL_SPARE_A_3_SHFT                                 0x3
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SPARE_AND_JPC_OVERRIDES_CM_PLL_SPARE_A_2_BMSK                                 0x4
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SPARE_AND_JPC_OVERRIDES_CM_PLL_SPARE_A_2_SHFT                                 0x2
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SPARE_AND_JPC_OVERRIDES_CM_PLL_SPARE_A_1_BMSK                                 0x2
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SPARE_AND_JPC_OVERRIDES_CM_PLL_SPARE_A_1_SHFT                                 0x1
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SPARE_AND_JPC_OVERRIDES_CM_PLL_SPARE_A_0_BMSK                                 0x1
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SPARE_AND_JPC_OVERRIDES_CM_PLL_SPARE_A_0_SHFT                                 0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_BIAS_CONTROL_1_ADDR                                                    (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x000001a4)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BIAS_CONTROL_1_RMSK                                                          0x7f
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BIAS_CONTROL_1_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_BIAS_CONTROL_1_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_BIAS_CONTROL_1_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BIAS_CONTROL_1_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_BIAS_CONTROL_1_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BIAS_CONTROL_1_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_BIAS_CONTROL_1_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BIAS_CONTROL_1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_BIAS_CONTROL_1_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_BIAS_CONTROL_1_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BIAS_CONTROL_1_CM_BIAS_IE_CTRL_BMSK                                          0x7e
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BIAS_CONTROL_1_CM_BIAS_IE_CTRL_SHFT                                           0x1
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BIAS_CONTROL_1_CM_BIAS_SEL_BMSK                                               0x1
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BIAS_CONTROL_1_CM_BIAS_SEL_SHFT                                               0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_BIAS_CONTROL_2_ADDR                                                    (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x000001a8)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BIAS_CONTROL_2_RMSK                                                          0x3f
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BIAS_CONTROL_2_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_BIAS_CONTROL_2_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_BIAS_CONTROL_2_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BIAS_CONTROL_2_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_BIAS_CONTROL_2_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BIAS_CONTROL_2_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_BIAS_CONTROL_2_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BIAS_CONTROL_2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_BIAS_CONTROL_2_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_BIAS_CONTROL_2_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BIAS_CONTROL_2_CM_BIAS_IP_CTRL_BMSK                                          0x3f
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BIAS_CONTROL_2_CM_BIAS_IP_CTRL_SHFT                                           0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_ALOG_OBSV_BUS_CTRL_1_ADDR                                              (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x000001ac)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ALOG_OBSV_BUS_CTRL_1_RMSK                                                    0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ALOG_OBSV_BUS_CTRL_1_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_ALOG_OBSV_BUS_CTRL_1_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_ALOG_OBSV_BUS_CTRL_1_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ALOG_OBSV_BUS_CTRL_1_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_ALOG_OBSV_BUS_CTRL_1_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ALOG_OBSV_BUS_CTRL_1_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_ALOG_OBSV_BUS_CTRL_1_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ALOG_OBSV_BUS_CTRL_1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_ALOG_OBSV_BUS_CTRL_1_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_ALOG_OBSV_BUS_CTRL_1_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ALOG_OBSV_BUS_CTRL_1_ALOG_OBSV_BUS_CTRL_1_SEL_BMSK                           0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ALOG_OBSV_BUS_CTRL_1_ALOG_OBSV_BUS_CTRL_1_SEL_SHFT                            0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_COMMON_STATUS_ONE_ADDR                                                 (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x000001b0)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_COMMON_STATUS_ONE_RMSK                                                       0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_COMMON_STATUS_ONE_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_COMMON_STATUS_ONE_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_COMMON_STATUS_ONE_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_COMMON_STATUS_ONE_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_COMMON_STATUS_ONE_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_COMMON_STATUS_ONE_UNLOCKED_STATUS_BMSK                                       0x80
#define HWIO_DSI_0_PHY_DSIPHY_PLL_COMMON_STATUS_ONE_UNLOCKED_STATUS_SHFT                                        0x7
#define HWIO_DSI_0_PHY_DSIPHY_PLL_COMMON_STATUS_ONE_PLL_PWRDN_STATUS_BMSK                                      0x40
#define HWIO_DSI_0_PHY_DSIPHY_PLL_COMMON_STATUS_ONE_PLL_PWRDN_STATUS_SHFT                                       0x6
#define HWIO_DSI_0_PHY_DSIPHY_PLL_COMMON_STATUS_ONE_PLL_RST_STATUS_BMSK                                        0x20
#define HWIO_DSI_0_PHY_DSIPHY_PLL_COMMON_STATUS_ONE_PLL_RST_STATUS_SHFT                                         0x5
#define HWIO_DSI_0_PHY_DSIPHY_PLL_COMMON_STATUS_ONE_PLL_LOCKOUT_STATUS_BMSK                                    0x10
#define HWIO_DSI_0_PHY_DSIPHY_PLL_COMMON_STATUS_ONE_PLL_LOCKOUT_STATUS_SHFT                                     0x4
#define HWIO_DSI_0_PHY_DSIPHY_PLL_COMMON_STATUS_ONE_CORE_PLL_RATE_STATUS_BMSK                                   0xc
#define HWIO_DSI_0_PHY_DSIPHY_PLL_COMMON_STATUS_ONE_CORE_PLL_RATE_STATUS_SHFT                                   0x2
#define HWIO_DSI_0_PHY_DSIPHY_PLL_COMMON_STATUS_ONE_CORE_PLL_EN_STATUS_BMSK                                     0x2
#define HWIO_DSI_0_PHY_DSIPHY_PLL_COMMON_STATUS_ONE_CORE_PLL_EN_STATUS_SHFT                                     0x1
#define HWIO_DSI_0_PHY_DSIPHY_PLL_COMMON_STATUS_ONE_CORE_READY_STATUS_BMSK                                      0x1
#define HWIO_DSI_0_PHY_DSIPHY_PLL_COMMON_STATUS_ONE_CORE_READY_STATUS_SHFT                                      0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_COMMON_STATUS_TWO_ADDR                                                 (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x000001b4)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_COMMON_STATUS_TWO_RMSK                                                       0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_COMMON_STATUS_TWO_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_COMMON_STATUS_TWO_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_COMMON_STATUS_TWO_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_COMMON_STATUS_TWO_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_COMMON_STATUS_TWO_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_COMMON_STATUS_TWO_PSM_PWR_STATE_INT_BMSK                                     0xe0
#define HWIO_DSI_0_PHY_DSIPHY_PLL_COMMON_STATUS_TWO_PSM_PWR_STATE_INT_SHFT                                      0x5
#define HWIO_DSI_0_PHY_DSIPHY_PLL_COMMON_STATUS_TWO_CALIBRATION_CNT_BMSK                                       0x18
#define HWIO_DSI_0_PHY_DSIPHY_PLL_COMMON_STATUS_TWO_CALIBRATION_CNT_SHFT                                        0x3
#define HWIO_DSI_0_PHY_DSIPHY_PLL_COMMON_STATUS_TWO_PLL_PWRDN_EXT_BMSK                                          0x4
#define HWIO_DSI_0_PHY_DSIPHY_PLL_COMMON_STATUS_TWO_PLL_PWRDN_EXT_SHFT                                          0x2
#define HWIO_DSI_0_PHY_DSIPHY_PLL_COMMON_STATUS_TWO_FD_LOCK_STATUS_BMSK                                         0x2
#define HWIO_DSI_0_PHY_DSIPHY_PLL_COMMON_STATUS_TWO_FD_LOCK_STATUS_SHFT                                         0x1
#define HWIO_DSI_0_PHY_DSIPHY_PLL_COMMON_STATUS_TWO_BAND_SEL_CAL_LOCKED_STATUS_BMSK                             0x1
#define HWIO_DSI_0_PHY_DSIPHY_PLL_COMMON_STATUS_TWO_BAND_SEL_CAL_LOCKED_STATUS_SHFT                             0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_ADDR                                                      (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x000001b8)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_RMSK                                                            0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_CALIBRATED_BAND_SEL_BMSK                                        0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_BAND_SEL_CAL_CALIBRATED_BAND_SEL_SHFT                                         0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_ICODE_ACCUM_STATUS_LOW_ADDR                                            (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x000001bc)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ICODE_ACCUM_STATUS_LOW_RMSK                                                  0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ICODE_ACCUM_STATUS_LOW_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_ICODE_ACCUM_STATUS_LOW_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_ICODE_ACCUM_STATUS_LOW_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ICODE_ACCUM_STATUS_LOW_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_ICODE_ACCUM_STATUS_LOW_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ICODE_ACCUM_STATUS_LOW_FTUNE_CODE_STATUS_7_0_BMSK                            0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ICODE_ACCUM_STATUS_LOW_FTUNE_CODE_STATUS_7_0_SHFT                             0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_ICODE_ACCUM_STATUS_HIGH_ADDR                                           (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x000001c0)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ICODE_ACCUM_STATUS_HIGH_RMSK                                                 0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ICODE_ACCUM_STATUS_HIGH_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_ICODE_ACCUM_STATUS_HIGH_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_ICODE_ACCUM_STATUS_HIGH_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ICODE_ACCUM_STATUS_HIGH_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_ICODE_ACCUM_STATUS_HIGH_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ICODE_ACCUM_STATUS_HIGH_FTUNE_CODE_STATUS_15_8_BMSK                          0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ICODE_ACCUM_STATUS_HIGH_FTUNE_CODE_STATUS_15_8_SHFT                           0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_FD_OUT_LOW_ADDR                                                        (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x000001c4)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FD_OUT_LOW_RMSK                                                              0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FD_OUT_LOW_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_FD_OUT_LOW_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_FD_OUT_LOW_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FD_OUT_LOW_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_FD_OUT_LOW_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FD_OUT_LOW_FD_OUT_7_0_BMSK                                                   0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FD_OUT_LOW_FD_OUT_7_0_SHFT                                                    0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_FD_OUT_HIGH_ADDR                                                       (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x000001c8)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FD_OUT_HIGH_RMSK                                                             0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FD_OUT_HIGH_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_FD_OUT_HIGH_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_FD_OUT_HIGH_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FD_OUT_HIGH_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_FD_OUT_HIGH_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FD_OUT_HIGH_FD_OUT_15_8_BMSK                                                 0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FD_OUT_HIGH_FD_OUT_15_8_SHFT                                                  0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_ALOG_OBSV_BUS_STATUS_1_ADDR                                            (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x000001cc)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ALOG_OBSV_BUS_STATUS_1_RMSK                                                  0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ALOG_OBSV_BUS_STATUS_1_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_ALOG_OBSV_BUS_STATUS_1_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_ALOG_OBSV_BUS_STATUS_1_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ALOG_OBSV_BUS_STATUS_1_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_ALOG_OBSV_BUS_STATUS_1_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ALOG_OBSV_BUS_STATUS_1_ALOG_OBSV_BUS_STATUS_1_BMSK                           0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ALOG_OBSV_BUS_STATUS_1_ALOG_OBSV_BUS_STATUS_1_SHFT                            0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_MISC_CONFIG_ADDR                                                   (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x000001d0)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_MISC_CONFIG_RMSK                                                         0x3f
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_MISC_CONFIG_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_MISC_CONFIG_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_MISC_CONFIG_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_MISC_CONFIG_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_MISC_CONFIG_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_MISC_CONFIG_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_MISC_CONFIG_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_MISC_CONFIG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_MISC_CONFIG_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_MISC_CONFIG_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_MISC_CONFIG_FLL_WAITTIME_FGCAL_BMSK                                      0x30
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_MISC_CONFIG_FLL_WAITTIME_FGCAL_SHFT                                       0x4
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_MISC_CONFIG_FLL_WAITTIME_BGCAL_BMSK                                       0xc
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_MISC_CONFIG_FLL_WAITTIME_BGCAL_SHFT                                       0x2
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_MISC_CONFIG_FLL_FREEZE_CODE_BMSK                                          0x2
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_MISC_CONFIG_FLL_FREEZE_CODE_SHFT                                          0x1
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_MISC_CONFIG_ICODE_PRECAL_EN_BMSK                                          0x1
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_MISC_CONFIG_ICODE_PRECAL_EN_SHFT                                          0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_FLL_CONFIG_ADDR                                                        (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x000001d4)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FLL_CONFIG_RMSK                                                              0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FLL_CONFIG_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_FLL_CONFIG_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_FLL_CONFIG_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FLL_CONFIG_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_FLL_CONFIG_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FLL_CONFIG_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_FLL_CONFIG_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FLL_CONFIG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_FLL_CONFIG_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_FLL_CONFIG_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FLL_CONFIG_FLL_CFG_MISC_BMSK                                                 0xe0
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FLL_CONFIG_FLL_CFG_MISC_SHFT                                                  0x5
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FLL_CONFIG_FLL_EN_CONT_CODE_UPDATE_BMSK                                      0x10
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FLL_CONFIG_FLL_EN_CONT_CODE_UPDATE_SHFT                                       0x4
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FLL_CONFIG_FLL_CNT_VAL_TOL_BMSK                                               0xe
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FLL_CONFIG_FLL_CNT_VAL_TOL_SHFT                                               0x1
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FLL_CONFIG_FLL_MODE_BMSK                                                      0x1
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FLL_CONFIG_FLL_MODE_SHFT                                                      0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_FLL_FREQ_ACQ_TIME_ADDR                                                 (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x000001d8)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FLL_FREQ_ACQ_TIME_RMSK                                                       0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FLL_FREQ_ACQ_TIME_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_FLL_FREQ_ACQ_TIME_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_FLL_FREQ_ACQ_TIME_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FLL_FREQ_ACQ_TIME_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_FLL_FREQ_ACQ_TIME_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FLL_FREQ_ACQ_TIME_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_FLL_FREQ_ACQ_TIME_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FLL_FREQ_ACQ_TIME_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_FLL_FREQ_ACQ_TIME_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_FLL_FREQ_ACQ_TIME_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FLL_FREQ_ACQ_TIME_FLL_FREQ_ACQ_CYC_BG_BMSK                                   0xf0
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FLL_FREQ_ACQ_TIME_FLL_FREQ_ACQ_CYC_BG_SHFT                                    0x4
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FLL_FREQ_ACQ_TIME_FLL_FREQ_ACQ_CYC_FG_BMSK                                    0xf
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FLL_FREQ_ACQ_TIME_FLL_FREQ_ACQ_CYC_FG_SHFT                                    0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_FLL_CODE0_ADDR                                                         (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x000001dc)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FLL_CODE0_RMSK                                                               0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FLL_CODE0_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_FLL_CODE0_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_FLL_CODE0_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FLL_CODE0_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_FLL_CODE0_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FLL_CODE0_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_FLL_CODE0_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FLL_CODE0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_FLL_CODE0_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_FLL_CODE0_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FLL_CODE0_FLL_CODE_7_0_BMSK                                                  0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FLL_CODE0_FLL_CODE_7_0_SHFT                                                   0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_FLL_CODE1_ADDR                                                         (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x000001e0)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FLL_CODE1_RMSK                                                               0x13
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FLL_CODE1_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_FLL_CODE1_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_FLL_CODE1_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FLL_CODE1_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_FLL_CODE1_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FLL_CODE1_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_FLL_CODE1_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FLL_CODE1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_FLL_CODE1_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_FLL_CODE1_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FLL_CODE1_FLL_CODE_MUX_BMSK                                                  0x10
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FLL_CODE1_FLL_CODE_MUX_SHFT                                                   0x4
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FLL_CODE1_FLL_CODE_9_8_BMSK                                                   0x3
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FLL_CODE1_FLL_CODE_9_8_SHFT                                                   0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_FLL_GAIN0_ADDR                                                         (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x000001e4)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FLL_GAIN0_RMSK                                                               0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FLL_GAIN0_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_FLL_GAIN0_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_FLL_GAIN0_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FLL_GAIN0_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_FLL_GAIN0_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FLL_GAIN0_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_FLL_GAIN0_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FLL_GAIN0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_FLL_GAIN0_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_FLL_GAIN0_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FLL_GAIN0_FLL_GAIN_7_0_BMSK                                                  0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FLL_GAIN0_FLL_GAIN_7_0_SHFT                                                   0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_FLL_GAIN1_ADDR                                                         (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x000001e8)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FLL_GAIN1_RMSK                                                                0xf
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FLL_GAIN1_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_FLL_GAIN1_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_FLL_GAIN1_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FLL_GAIN1_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_FLL_GAIN1_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FLL_GAIN1_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_FLL_GAIN1_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FLL_GAIN1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_FLL_GAIN1_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_FLL_GAIN1_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FLL_GAIN1_FLL_GAIN_11_8_BMSK                                                  0xf
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FLL_GAIN1_FLL_GAIN_11_8_SHFT                                                  0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_SW_RESET_ADDR                                                          (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x000001ec)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SW_RESET_RMSK                                                                 0x1
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SW_RESET_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_SW_RESET_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_SW_RESET_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SW_RESET_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_SW_RESET_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SW_RESET_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_SW_RESET_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SW_RESET_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_SW_RESET_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_SW_RESET_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SW_RESET_SW_RESET_BMSK                                                        0x1
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SW_RESET_SW_RESET_SHFT                                                        0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_FAST_PWRUP_ADDR                                                        (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x000001f0)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FAST_PWRUP_RMSK                                                              0x3f
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FAST_PWRUP_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_FAST_PWRUP_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_FAST_PWRUP_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FAST_PWRUP_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_FAST_PWRUP_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FAST_PWRUP_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_FAST_PWRUP_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FAST_PWRUP_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_FAST_PWRUP_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_FAST_PWRUP_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FAST_PWRUP_FLL_RUN_BGCAL_MUX_BMSK                                            0x20
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FAST_PWRUP_FLL_RUN_BGCAL_MUX_SHFT                                             0x5
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FAST_PWRUP_FLL_RUN_BGCAL_BMSK                                                0x10
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FAST_PWRUP_FLL_RUN_BGCAL_SHFT                                                 0x4
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FAST_PWRUP_FLL_RUN_FGCAL_MUX_BMSK                                             0x8
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FAST_PWRUP_FLL_RUN_FGCAL_MUX_SHFT                                             0x3
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FAST_PWRUP_FLL_RUN_FGCAL_BMSK                                                 0x4
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FAST_PWRUP_FLL_RUN_FGCAL_SHFT                                                 0x2
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FAST_PWRUP_FLL_CAL_EN_MUX_BMSK                                                0x2
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FAST_PWRUP_FLL_CAL_EN_MUX_SHFT                                                0x1
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FAST_PWRUP_FLL_CAL_EN_BMSK                                                    0x1
#define HWIO_DSI_0_PHY_DSIPHY_PLL_FAST_PWRUP_FLL_CAL_EN_SHFT                                                    0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_LOCKTIME0_ADDR                                                         (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x000001f4)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_LOCKTIME0_RMSK                                                               0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_LOCKTIME0_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_LOCKTIME0_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_LOCKTIME0_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_LOCKTIME0_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_LOCKTIME0_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_LOCKTIME0_LOCKTIME_7_0_BMSK                                                  0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_LOCKTIME0_LOCKTIME_7_0_SHFT                                                   0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_LOCKTIME1_ADDR                                                         (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x000001f8)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_LOCKTIME1_RMSK                                                               0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_LOCKTIME1_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_LOCKTIME1_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_LOCKTIME1_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_LOCKTIME1_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_LOCKTIME1_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_LOCKTIME1_LOCKTIME_15_8_BMSK                                                 0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_LOCKTIME1_LOCKTIME_15_8_SHFT                                                  0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_DEBUG_BUS_SEL_ADDR                                                     (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x000001fc)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_DEBUG_BUS_SEL_RMSK                                                            0xf
#define HWIO_DSI_0_PHY_DSIPHY_PLL_DEBUG_BUS_SEL_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_DEBUG_BUS_SEL_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_DEBUG_BUS_SEL_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_DEBUG_BUS_SEL_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_DEBUG_BUS_SEL_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_DEBUG_BUS_SEL_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_DEBUG_BUS_SEL_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_DEBUG_BUS_SEL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_DEBUG_BUS_SEL_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_DEBUG_BUS_SEL_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_DEBUG_BUS_SEL_DEBUG_BUS_SEL_BMSK                                              0xf
#define HWIO_DSI_0_PHY_DSIPHY_PLL_DEBUG_BUS_SEL_DEBUG_BUS_SEL_SHFT                                              0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_DEBUG_BUS0_ADDR                                                        (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000200)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_DEBUG_BUS0_RMSK                                                              0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_DEBUG_BUS0_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_DEBUG_BUS0_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_DEBUG_BUS0_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_DEBUG_BUS0_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_DEBUG_BUS0_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_DEBUG_BUS0_DEBUG_BUS_7_0_BMSK                                                0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_DEBUG_BUS0_DEBUG_BUS_7_0_SHFT                                                 0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_DEBUG_BUS1_ADDR                                                        (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000204)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_DEBUG_BUS1_RMSK                                                              0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_DEBUG_BUS1_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_DEBUG_BUS1_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_DEBUG_BUS1_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_DEBUG_BUS1_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_DEBUG_BUS1_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_DEBUG_BUS1_DEBUG_BUS_15_8_BMSK                                               0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_DEBUG_BUS1_DEBUG_BUS_15_8_SHFT                                                0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_DEBUG_BUS2_ADDR                                                        (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000208)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_DEBUG_BUS2_RMSK                                                              0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_DEBUG_BUS2_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_DEBUG_BUS2_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_DEBUG_BUS2_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_DEBUG_BUS2_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_DEBUG_BUS2_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_DEBUG_BUS2_DEBUG_BUS_23_16_BMSK                                              0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_DEBUG_BUS2_DEBUG_BUS_23_16_SHFT                                               0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_DEBUG_BUS3_ADDR                                                        (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x0000020c)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_DEBUG_BUS3_RMSK                                                              0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_DEBUG_BUS3_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_DEBUG_BUS3_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_DEBUG_BUS3_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_DEBUG_BUS3_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_DEBUG_BUS3_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_DEBUG_BUS3_DEBUG_BUS_31_24_BMSK                                              0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_DEBUG_BUS3_DEBUG_BUS_31_24_SHFT                                               0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_FLL_CONTROL_OVERRIDES_ADDR                                      (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000210)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_FLL_CONTROL_OVERRIDES_RMSK                                            0x77
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_FLL_CONTROL_OVERRIDES_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_FLL_CONTROL_OVERRIDES_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_FLL_CONTROL_OVERRIDES_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_FLL_CONTROL_OVERRIDES_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_FLL_CONTROL_OVERRIDES_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_FLL_CONTROL_OVERRIDES_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_FLL_CONTROL_OVERRIDES_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_FLL_CONTROL_OVERRIDES_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_FLL_CONTROL_OVERRIDES_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_FLL_CONTROL_OVERRIDES_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_FLL_CONTROL_OVERRIDES_FLL_MODE_JPC_MUX_BMSK                           0x40
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_FLL_CONTROL_OVERRIDES_FLL_MODE_JPC_MUX_SHFT                            0x6
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_FLL_CONTROL_OVERRIDES_FLL_DIV_RST_MUX_BMSK                            0x20
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_FLL_CONTROL_OVERRIDES_FLL_DIV_RST_MUX_SHFT                             0x5
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_FLL_CONTROL_OVERRIDES_FLL_PFD_TDC_RST_MUX_BMSK                        0x10
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_FLL_CONTROL_OVERRIDES_FLL_PFD_TDC_RST_MUX_SHFT                         0x4
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_FLL_CONTROL_OVERRIDES_FLL_MODE_JPC_BMSK                                0x4
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_FLL_CONTROL_OVERRIDES_FLL_MODE_JPC_SHFT                                0x2
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_FLL_CONTROL_OVERRIDES_FLL_DIV_RST_BMSK                                 0x2
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_FLL_CONTROL_OVERRIDES_FLL_DIV_RST_SHFT                                 0x1
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_FLL_CONTROL_OVERRIDES_FLL_PFD_TDC_RST_BMSK                             0x1
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_FLL_CONTROL_OVERRIDES_FLL_PFD_TDC_RST_SHFT                             0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_VCO_CONFIG_ADDR                                                        (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000214)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_VCO_CONFIG_RMSK                                                              0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_VCO_CONFIG_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_VCO_CONFIG_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_VCO_CONFIG_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_VCO_CONFIG_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_VCO_CONFIG_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_VCO_CONFIG_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_VCO_CONFIG_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_VCO_CONFIG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_VCO_CONFIG_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_VCO_CONFIG_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_VCO_CONFIG_VCO_CONFIG_BMSK                                                   0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_VCO_CONFIG_VCO_CONFIG_SHFT                                                    0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_VCO_CAL_CODE1_MODE0_STATUS_ADDR                                        (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000218)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_VCO_CAL_CODE1_MODE0_STATUS_RMSK                                              0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_VCO_CAL_CODE1_MODE0_STATUS_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_VCO_CAL_CODE1_MODE0_STATUS_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_VCO_CAL_CODE1_MODE0_STATUS_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_VCO_CAL_CODE1_MODE0_STATUS_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_VCO_CAL_CODE1_MODE0_STATUS_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_VCO_CAL_CODE1_MODE0_STATUS_PSM_VCOCAL_CODE_MODE0_7_0_BMSK                    0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_VCO_CAL_CODE1_MODE0_STATUS_PSM_VCOCAL_CODE_MODE0_7_0_SHFT                     0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_VCO_CAL_CODE1_MODE1_STATUS_ADDR                                        (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x0000021c)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_VCO_CAL_CODE1_MODE1_STATUS_RMSK                                              0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_VCO_CAL_CODE1_MODE1_STATUS_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_VCO_CAL_CODE1_MODE1_STATUS_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_VCO_CAL_CODE1_MODE1_STATUS_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_VCO_CAL_CODE1_MODE1_STATUS_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_VCO_CAL_CODE1_MODE1_STATUS_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_VCO_CAL_CODE1_MODE1_STATUS_PSM_VCOCAL_CODE_MODE1_7_0_BMSK                    0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_VCO_CAL_CODE1_MODE1_STATUS_PSM_VCOCAL_CODE_MODE1_7_0_SHFT                     0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_RESET_SM_STATUS_ADDR                                                   (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000220)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_RESET_SM_STATUS_RMSK                                                         0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_RESET_SM_STATUS_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_RESET_SM_STATUS_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_RESET_SM_STATUS_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_RESET_SM_STATUS_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_RESET_SM_STATUS_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_RESET_SM_STATUS_FIRST_DIR_BMSK                                               0x80
#define HWIO_DSI_0_PHY_DSIPHY_PLL_RESET_SM_STATUS_FIRST_DIR_SHFT                                                0x7
#define HWIO_DSI_0_PHY_DSIPHY_PLL_RESET_SM_STATUS_BAS_STATE_BMSK                                               0x70
#define HWIO_DSI_0_PHY_DSIPHY_PLL_RESET_SM_STATUS_BAS_STATE_SHFT                                                0x4
#define HWIO_DSI_0_PHY_DSIPHY_PLL_RESET_SM_STATUS_RESETSM_BMSK                                                  0xf
#define HWIO_DSI_0_PHY_DSIPHY_PLL_RESET_SM_STATUS_RESETSM_SHFT                                                  0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_TDC_OFFSET_ADDR                                                        (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000224)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_TDC_OFFSET_RMSK                                                              0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_TDC_OFFSET_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_TDC_OFFSET_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_TDC_OFFSET_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_TDC_OFFSET_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_TDC_OFFSET_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_TDC_OFFSET_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_TDC_OFFSET_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_TDC_OFFSET_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_TDC_OFFSET_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_TDC_OFFSET_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_TDC_OFFSET_TDC_OFFSET_BMSK                                                   0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_TDC_OFFSET_TDC_OFFSET_SHFT                                                    0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_PS3_PWRDOWN_CONTROLS_ADDR                                              (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000228)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PS3_PWRDOWN_CONTROLS_RMSK                                                    0x1f
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PS3_PWRDOWN_CONTROLS_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_PS3_PWRDOWN_CONTROLS_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_PS3_PWRDOWN_CONTROLS_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PS3_PWRDOWN_CONTROLS_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_PS3_PWRDOWN_CONTROLS_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PS3_PWRDOWN_CONTROLS_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_PS3_PWRDOWN_CONTROLS_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PS3_PWRDOWN_CONTROLS_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_PS3_PWRDOWN_CONTROLS_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_PS3_PWRDOWN_CONTROLS_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PS3_PWRDOWN_CONTROLS_PS3_CLK_DRV_GATEOFF_BMSK                                0x10
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PS3_PWRDOWN_CONTROLS_PS3_CLK_DRV_GATEOFF_SHFT                                 0x4
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PS3_PWRDOWN_CONTROLS_PS3_PLL_RST_BMSK                                         0x8
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PS3_PWRDOWN_CONTROLS_PS3_PLL_RST_SHFT                                         0x3
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PS3_PWRDOWN_CONTROLS_PS3_DLF_RST_BMSK                                         0x4
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PS3_PWRDOWN_CONTROLS_PS3_DLF_RST_SHFT                                         0x2
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PS3_PWRDOWN_CONTROLS_PS3_EN_PLL_BMSK                                          0x2
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PS3_PWRDOWN_CONTROLS_PS3_EN_PLL_SHFT                                          0x1
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PS3_PWRDOWN_CONTROLS_PS3_BIAS_EN_BMSK                                         0x1
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PS3_PWRDOWN_CONTROLS_PS3_BIAS_EN_SHFT                                         0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_PS4_PWRDOWN_CONTROLS_ADDR                                              (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x0000022c)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PS4_PWRDOWN_CONTROLS_RMSK                                                    0x1f
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PS4_PWRDOWN_CONTROLS_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_PS4_PWRDOWN_CONTROLS_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_PS4_PWRDOWN_CONTROLS_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PS4_PWRDOWN_CONTROLS_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_PS4_PWRDOWN_CONTROLS_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PS4_PWRDOWN_CONTROLS_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_PS4_PWRDOWN_CONTROLS_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PS4_PWRDOWN_CONTROLS_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_PS4_PWRDOWN_CONTROLS_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_PS4_PWRDOWN_CONTROLS_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PS4_PWRDOWN_CONTROLS_PS4_CLK_DRV_GATEOFF_BMSK                                0x10
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PS4_PWRDOWN_CONTROLS_PS4_CLK_DRV_GATEOFF_SHFT                                 0x4
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PS4_PWRDOWN_CONTROLS_PS4_PLL_RST_BMSK                                         0x8
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PS4_PWRDOWN_CONTROLS_PS4_PLL_RST_SHFT                                         0x3
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PS4_PWRDOWN_CONTROLS_PS4_DLF_RST_BMSK                                         0x4
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PS4_PWRDOWN_CONTROLS_PS4_DLF_RST_SHFT                                         0x2
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PS4_PWRDOWN_CONTROLS_PS4_EN_PLL_BMSK                                          0x2
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PS4_PWRDOWN_CONTROLS_PS4_EN_PLL_SHFT                                          0x1
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PS4_PWRDOWN_CONTROLS_PS4_BIAS_EN_BMSK                                         0x1
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PS4_PWRDOWN_CONTROLS_PS4_BIAS_EN_SHFT                                         0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_RST_CONTROLS_ADDR                                                  (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000230)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_RST_CONTROLS_RMSK                                                        0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_RST_CONTROLS_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_RST_CONTROLS_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_RST_CONTROLS_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_RST_CONTROLS_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_RST_CONTROLS_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_RST_CONTROLS_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_RST_CONTROLS_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_RST_CONTROLS_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_RST_CONTROLS_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_RST_CONTROLS_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_RST_CONTROLS_PLL_RST_RLS_DLY_BMSK                                        0xf0
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_RST_CONTROLS_PLL_RST_RLS_DLY_SHFT                                         0x4
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_RST_CONTROLS_PLL_PWRDN_RLS_DLY_BMSK                                       0xf
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PLL_RST_CONTROLS_PLL_PWRDN_RLS_DLY_SHFT                                       0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_GEAR_BAND_SELECT_CONTROLS_ADDR                                         (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000234)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_GEAR_BAND_SELECT_CONTROLS_RMSK                                               0x3f
#define HWIO_DSI_0_PHY_DSIPHY_PLL_GEAR_BAND_SELECT_CONTROLS_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_GEAR_BAND_SELECT_CONTROLS_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_GEAR_BAND_SELECT_CONTROLS_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_GEAR_BAND_SELECT_CONTROLS_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_GEAR_BAND_SELECT_CONTROLS_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_GEAR_BAND_SELECT_CONTROLS_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_GEAR_BAND_SELECT_CONTROLS_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_GEAR_BAND_SELECT_CONTROLS_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_GEAR_BAND_SELECT_CONTROLS_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_GEAR_BAND_SELECT_CONTROLS_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_GEAR_BAND_SELECT_CONTROLS_BAS_STOP_FIRST_LOCK_2STEPS_ADJ_BMSK                0x20
#define HWIO_DSI_0_PHY_DSIPHY_PLL_GEAR_BAND_SELECT_CONTROLS_BAS_STOP_FIRST_LOCK_2STEPS_ADJ_SHFT                 0x5
#define HWIO_DSI_0_PHY_DSIPHY_PLL_GEAR_BAND_SELECT_CONTROLS_BAS_STOP_FIRST_LOCK_1STEP_ADJ_BMSK                 0x10
#define HWIO_DSI_0_PHY_DSIPHY_PLL_GEAR_BAND_SELECT_CONTROLS_BAS_STOP_FIRST_LOCK_1STEP_ADJ_SHFT                  0x4
#define HWIO_DSI_0_PHY_DSIPHY_PLL_GEAR_BAND_SELECT_CONTROLS_BAS_2ND_LOOP_X2_BMSK                                0x8
#define HWIO_DSI_0_PHY_DSIPHY_PLL_GEAR_BAND_SELECT_CONTROLS_BAS_2ND_LOOP_X2_SHFT                                0x3
#define HWIO_DSI_0_PHY_DSIPHY_PLL_GEAR_BAND_SELECT_CONTROLS_BAS_OFFSET_DIV2_BMSK                                0x4
#define HWIO_DSI_0_PHY_DSIPHY_PLL_GEAR_BAND_SELECT_CONTROLS_BAS_OFFSET_DIV2_SHFT                                0x2
#define HWIO_DSI_0_PHY_DSIPHY_PLL_GEAR_BAND_SELECT_CONTROLS_BAS_GEAR_BS_ENABLE_BMSK                             0x2
#define HWIO_DSI_0_PHY_DSIPHY_PLL_GEAR_BAND_SELECT_CONTROLS_BAS_GEAR_BS_ENABLE_SHFT                             0x1
#define HWIO_DSI_0_PHY_DSIPHY_PLL_GEAR_BAND_SELECT_CONTROLS_LNS_GEAR_BS_ENABLE_BMSK                             0x1
#define HWIO_DSI_0_PHY_DSIPHY_PLL_GEAR_BAND_SELECT_CONTROLS_LNS_GEAR_BS_ENABLE_SHFT                             0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_PSM_CLK_CONTROLS_ADDR                                                  (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000238)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PSM_CLK_CONTROLS_RMSK                                                         0xf
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PSM_CLK_CONTROLS_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_PSM_CLK_CONTROLS_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_PSM_CLK_CONTROLS_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PSM_CLK_CONTROLS_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_PSM_CLK_CONTROLS_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PSM_CLK_CONTROLS_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_PSM_CLK_CONTROLS_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PSM_CLK_CONTROLS_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_PSM_CLK_CONTROLS_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_PSM_CLK_CONTROLS_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PSM_CLK_CONTROLS_USE_CMODE_CAL_CLK_CONTROL_BMSK                               0x8
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PSM_CLK_CONTROLS_USE_CMODE_CAL_CLK_CONTROL_SHFT                               0x3
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PSM_CLK_CONTROLS_PSM_CONTROL_CAL_CLK_MUX_BMSK                                 0x4
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PSM_CLK_CONTROLS_PSM_CONTROL_CAL_CLK_MUX_SHFT                                 0x2
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PSM_CLK_CONTROLS_PSM_CONTROL_CAL_CLK_BMSK                                     0x2
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PSM_CLK_CONTROLS_PSM_CONTROL_CAL_CLK_SHFT                                     0x1
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PSM_CLK_CONTROLS_PS2_CLK_DRV_GATEOFF_BMSK                                     0x1
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PSM_CLK_CONTROLS_PS2_CLK_DRV_GATEOFF_SHFT                                     0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_SYSTEM_MUXES_2_ADDR                                                    (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x0000023c)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SYSTEM_MUXES_2_RMSK                                                          0x1f
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SYSTEM_MUXES_2_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_SYSTEM_MUXES_2_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_SYSTEM_MUXES_2_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SYSTEM_MUXES_2_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_SYSTEM_MUXES_2_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SYSTEM_MUXES_2_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_SYSTEM_MUXES_2_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SYSTEM_MUXES_2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_SYSTEM_MUXES_2_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_SYSTEM_MUXES_2_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SYSTEM_MUXES_2_CM_VCOCLK_DIV2EN_MSB_MUX_BMSK                                 0x10
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SYSTEM_MUXES_2_CM_VCOCLK_DIV2EN_MSB_MUX_SHFT                                  0x4
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SYSTEM_MUXES_2_CM_VCOCLK_DIV2EN_MUX_BMSK                                      0x8
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SYSTEM_MUXES_2_CM_VCOCLK_DIV2EN_MUX_SHFT                                      0x3
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SYSTEM_MUXES_2_CM_PLL_CLOCK_INVERTERS_MUX_BMSK                                0x4
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SYSTEM_MUXES_2_CM_PLL_CLOCK_INVERTERS_MUX_SHFT                                0x2
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SYSTEM_MUXES_2_VCO_CONFIG_MUX_BMSK                                            0x2
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SYSTEM_MUXES_2_VCO_CONFIG_MUX_SHFT                                            0x1
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SYSTEM_MUXES_2_CM_CMODE_MUX_BMSK                                              0x1
#define HWIO_DSI_0_PHY_DSIPHY_PLL_SYSTEM_MUXES_2_CM_CMODE_MUX_SHFT                                              0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_VCO_CONFIG_1_ADDR                                                      (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000240)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_VCO_CONFIG_1_RMSK                                                            0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_VCO_CONFIG_1_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_VCO_CONFIG_1_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_VCO_CONFIG_1_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_VCO_CONFIG_1_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_VCO_CONFIG_1_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_VCO_CONFIG_1_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_VCO_CONFIG_1_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_VCO_CONFIG_1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_VCO_CONFIG_1_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_VCO_CONFIG_1_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_VCO_CONFIG_1_VCO_CONFIG_1_BMSK                                               0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_VCO_CONFIG_1_VCO_CONFIG_1_SHFT                                                0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_VCO_CONFIG_2_ADDR                                                      (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000244)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_VCO_CONFIG_2_RMSK                                                            0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_VCO_CONFIG_2_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_VCO_CONFIG_2_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_VCO_CONFIG_2_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_VCO_CONFIG_2_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_VCO_CONFIG_2_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_VCO_CONFIG_2_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_VCO_CONFIG_2_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_VCO_CONFIG_2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_VCO_CONFIG_2_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_VCO_CONFIG_2_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_VCO_CONFIG_2_VCO_CONFIG_2_BMSK                                               0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_VCO_CONFIG_2_VCO_CONFIG_2_SHFT                                                0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_CLOCK_INVERTERS_1_ADDR                                                 (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000248)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CLOCK_INVERTERS_1_RMSK                                                       0x7b
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CLOCK_INVERTERS_1_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_CLOCK_INVERTERS_1_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_CLOCK_INVERTERS_1_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CLOCK_INVERTERS_1_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_CLOCK_INVERTERS_1_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CLOCK_INVERTERS_1_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_CLOCK_INVERTERS_1_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CLOCK_INVERTERS_1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_CLOCK_INVERTERS_1_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_CLOCK_INVERTERS_1_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CLOCK_INVERTERS_1_CM_OSCBIAS_ADJ_1_BMSK                                      0x60
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CLOCK_INVERTERS_1_CM_OSCBIAS_ADJ_1_SHFT                                       0x5
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CLOCK_INVERTERS_1_CM_OSCCAP_EN_1_BMSK                                        0x18
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CLOCK_INVERTERS_1_CM_OSCCAP_EN_1_SHFT                                         0x3
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CLOCK_INVERTERS_1_INVERT_DSMCLK_1_BMSK                                        0x2
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CLOCK_INVERTERS_1_INVERT_DSMCLK_1_SHFT                                        0x1
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CLOCK_INVERTERS_1_INVERT_FRACNCLK_1_BMSK                                      0x1
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CLOCK_INVERTERS_1_INVERT_FRACNCLK_1_SHFT                                      0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_CLOCK_INVERTERS_2_ADDR                                                 (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x0000024c)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CLOCK_INVERTERS_2_RMSK                                                       0x7b
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CLOCK_INVERTERS_2_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_CLOCK_INVERTERS_2_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_CLOCK_INVERTERS_2_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CLOCK_INVERTERS_2_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_CLOCK_INVERTERS_2_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CLOCK_INVERTERS_2_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_CLOCK_INVERTERS_2_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CLOCK_INVERTERS_2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_CLOCK_INVERTERS_2_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_CLOCK_INVERTERS_2_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CLOCK_INVERTERS_2_CM_OSCBIAS_ADJ_2_BMSK                                      0x60
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CLOCK_INVERTERS_2_CM_OSCBIAS_ADJ_2_SHFT                                       0x5
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CLOCK_INVERTERS_2_CM_OSCCAP_EN_2_BMSK                                        0x18
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CLOCK_INVERTERS_2_CM_OSCCAP_EN_2_SHFT                                         0x3
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CLOCK_INVERTERS_2_INVERT_DSMCLK_2_BMSK                                        0x2
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CLOCK_INVERTERS_2_INVERT_DSMCLK_2_SHFT                                        0x1
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CLOCK_INVERTERS_2_INVERT_FRACNCLK_2_BMSK                                      0x1
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CLOCK_INVERTERS_2_INVERT_FRACNCLK_2_SHFT                                      0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_CMODE_1_ADDR                                                           (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000250)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CMODE_1_RMSK                                                                 0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CMODE_1_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_CMODE_1_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_CMODE_1_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CMODE_1_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_CMODE_1_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CMODE_1_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_CMODE_1_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CMODE_1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_CMODE_1_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_CMODE_1_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CMODE_1_CM_CMODE_1_BMSK                                                      0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CMODE_1_CM_CMODE_1_SHFT                                                       0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_CMODE_2_ADDR                                                           (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000254)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CMODE_2_RMSK                                                                 0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CMODE_2_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_CMODE_2_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_CMODE_2_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CMODE_2_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_CMODE_2_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CMODE_2_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_CMODE_2_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CMODE_2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_CMODE_2_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_CMODE_2_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CMODE_2_CM_CMODE_2_BMSK                                                      0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_CMODE_2_CM_CMODE_2_SHFT                                                       0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FIVE_1_ADDR                                            (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000258)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FIVE_1_RMSK                                                   0x3
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FIVE_1_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FIVE_1_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FIVE_1_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FIVE_1_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FIVE_1_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FIVE_1_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FIVE_1_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FIVE_1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FIVE_1_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FIVE_1_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FIVE_1_CM_VCOCLK_DIV2EN_MSB_1_BMSK                            0x2
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FIVE_1_CM_VCOCLK_DIV2EN_MSB_1_SHFT                            0x1
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FIVE_1_CM_VCOCLK_DIV2EN_1_BMSK                                0x1
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FIVE_1_CM_VCOCLK_DIV2EN_1_SHFT                                0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FIVE_2_ADDR                                            (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x0000025c)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FIVE_2_RMSK                                                   0x3
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FIVE_2_IN          \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FIVE_2_ADDR, HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FIVE_2_RMSK)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FIVE_2_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FIVE_2_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FIVE_2_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FIVE_2_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FIVE_2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FIVE_2_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FIVE_2_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FIVE_2_CM_VCOCLK_DIV2EN_MSB_2_BMSK                            0x2
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FIVE_2_CM_VCOCLK_DIV2EN_MSB_2_SHFT                            0x1
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FIVE_2_CM_VCOCLK_DIV2EN_2_BMSK                                0x1
#define HWIO_DSI_0_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FIVE_2_CM_VCOCLK_DIV2EN_2_SHFT                                0x0

#define HWIO_DSI_0_PHY_DSIPHY_PLL_PERF_OPTIMIZE_ADDR                                                     (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000260)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PERF_OPTIMIZE_PHYS                                                     (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE_PHYS + 0x00000260)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PERF_OPTIMIZE_OFFS                                                     (DSI_0_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE_OFFS + 0x00000260)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PERF_OPTIMIZE_RMSK                                                           0xff
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PERF_OPTIMIZE_IN          \
        in_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_PERF_OPTIMIZE_ADDR)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PERF_OPTIMIZE_INM(m)      \
        in_dword_masked(HWIO_DSI_0_PHY_DSIPHY_PLL_PERF_OPTIMIZE_ADDR, m)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PERF_OPTIMIZE_OUT(v)      \
        out_dword(HWIO_DSI_0_PHY_DSIPHY_PLL_PERF_OPTIMIZE_ADDR,v)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PERF_OPTIMIZE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_0_PHY_DSIPHY_PLL_PERF_OPTIMIZE_ADDR,m,v,HWIO_DSI_0_PHY_DSIPHY_PLL_PERF_OPTIMIZE_IN)
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PERF_OPTIMIZE_NEW_RATE_SETTLE_BMSK                                           0xf0
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PERF_OPTIMIZE_NEW_RATE_SETTLE_SHFT                                            0x4
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PERF_OPTIMIZE_ENABLE_DEBUG_CNTR_BMSK                                          0x8
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PERF_OPTIMIZE_ENABLE_DEBUG_CNTR_SHFT                                          0x3
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PERF_OPTIMIZE_NEW_RATE_SETTLE_SEL_BMSK                                        0x4
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PERF_OPTIMIZE_NEW_RATE_SETTLE_SEL_SHFT                                        0x2
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PERF_OPTIMIZE_LEGACY_DELAY_BMSK                                               0x2
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PERF_OPTIMIZE_LEGACY_DELAY_SHFT                                               0x1
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PERF_OPTIMIZE_LEGACY_MODE_PSM_ENABLE_BMSK                                     0x1
#define HWIO_DSI_0_PHY_DSIPHY_PLL_PERF_OPTIMIZE_LEGACY_MODE_PSM_ENABLE_SHFT                                     0x0

/*----------------------------------------------------------------------------
 * MODULE: DSI_1_PHY
 *--------------------------------------------------------------------------*/

#define DSI_1_PHY_REG_BASE   (MDSS_BASE      + 0x00096400)

/*----------------------------------------------------------------------------
 * MODULE: DSI_1_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON
 *--------------------------------------------------------------------------*/

#define DSI_1_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE                                            (MDSS_BASE      + 0x00096400)

#define HWIO_DSI_1_PHY_DSIPHY_CMN_REVISION_ID0_ADDR                                                    (DSI_1_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x00000000)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_REVISION_ID0_RMSK                                                          0xff
#define HWIO_DSI_1_PHY_DSIPHY_CMN_REVISION_ID0_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_REVISION_ID0_ADDR, HWIO_DSI_1_PHY_DSIPHY_CMN_REVISION_ID0_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_REVISION_ID0_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_REVISION_ID0_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_REVISION_ID0_MINOR_3_0_BMSK                                                0xf0
#define HWIO_DSI_1_PHY_DSIPHY_CMN_REVISION_ID0_MINOR_3_0_SHFT                                                 0x4
#define HWIO_DSI_1_PHY_DSIPHY_CMN_REVISION_ID0_MAJOR_BMSK                                                     0xf
#define HWIO_DSI_1_PHY_DSIPHY_CMN_REVISION_ID0_MAJOR_SHFT                                                     0x0

#define HWIO_DSI_1_PHY_DSIPHY_CMN_REVISION_ID1_ADDR                                                    (DSI_1_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x00000004)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_REVISION_ID1_RMSK                                                          0xff
#define HWIO_DSI_1_PHY_DSIPHY_CMN_REVISION_ID1_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_REVISION_ID1_ADDR, HWIO_DSI_1_PHY_DSIPHY_CMN_REVISION_ID1_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_REVISION_ID1_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_REVISION_ID1_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_REVISION_ID1_MINOR_11_4_BMSK                                               0xff
#define HWIO_DSI_1_PHY_DSIPHY_CMN_REVISION_ID1_MINOR_11_4_SHFT                                                0x0

#define HWIO_DSI_1_PHY_DSIPHY_CMN_REVISION_ID2_ADDR                                                    (DSI_1_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x00000008)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_REVISION_ID2_RMSK                                                          0xff
#define HWIO_DSI_1_PHY_DSIPHY_CMN_REVISION_ID2_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_REVISION_ID2_ADDR, HWIO_DSI_1_PHY_DSIPHY_CMN_REVISION_ID2_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_REVISION_ID2_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_REVISION_ID2_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_REVISION_ID2_STEP_7_0_BMSK                                                 0xff
#define HWIO_DSI_1_PHY_DSIPHY_CMN_REVISION_ID2_STEP_7_0_SHFT                                                  0x0

#define HWIO_DSI_1_PHY_DSIPHY_CMN_REVISION_ID3_ADDR                                                    (DSI_1_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x0000000c)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_REVISION_ID3_RMSK                                                          0xff
#define HWIO_DSI_1_PHY_DSIPHY_CMN_REVISION_ID3_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_REVISION_ID3_ADDR, HWIO_DSI_1_PHY_DSIPHY_CMN_REVISION_ID3_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_REVISION_ID3_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_REVISION_ID3_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_REVISION_ID3_STEP_15_8_BMSK                                                0xff
#define HWIO_DSI_1_PHY_DSIPHY_CMN_REVISION_ID3_STEP_15_8_SHFT                                                 0x0

#define HWIO_DSI_1_PHY_DSIPHY_CMN_CLK_CFG0_ADDR                                                        (DSI_1_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x00000010)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CLK_CFG0_RMSK                                                              0xff
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CLK_CFG0_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_CLK_CFG0_ADDR, HWIO_DSI_1_PHY_DSIPHY_CMN_CLK_CFG0_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CLK_CFG0_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_CLK_CFG0_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CLK_CFG0_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_CMN_CLK_CFG0_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CLK_CFG0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_CMN_CLK_CFG0_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_CMN_CLK_CFG0_IN)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CLK_CFG0_DIV_CTRL_7_4_BMSK                                                 0xf0
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CLK_CFG0_DIV_CTRL_7_4_SHFT                                                  0x4
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CLK_CFG0_DIV_CTRL_3_0_BMSK                                                  0xf
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CLK_CFG0_DIV_CTRL_3_0_SHFT                                                  0x0

#define HWIO_DSI_1_PHY_DSIPHY_CMN_CLK_CFG1_ADDR                                                        (DSI_1_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x00000014)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CLK_CFG1_RMSK                                                              0x7f
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CLK_CFG1_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_CLK_CFG1_ADDR, HWIO_DSI_1_PHY_DSIPHY_CMN_CLK_CFG1_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CLK_CFG1_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_CLK_CFG1_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CLK_CFG1_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_CMN_CLK_CFG1_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CLK_CFG1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_CMN_CLK_CFG1_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_CMN_CLK_CFG1_IN)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CLK_CFG1_BITCLK_R_OUT_EN_BMSK                                              0x40
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CLK_CFG1_BITCLK_R_OUT_EN_SHFT                                               0x6
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CLK_CFG1_CLK_EN_BMSK                                                       0x20
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CLK_CFG1_CLK_EN_SHFT                                                        0x5
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CLK_CFG1_CLK_EN_SEL_BMSK                                                   0x10
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CLK_CFG1_CLK_EN_SEL_SHFT                                                    0x4
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CLK_CFG1_BITCLK_SEL_BMSK                                                    0xc
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CLK_CFG1_BITCLK_SEL_SHFT                                                    0x2
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CLK_CFG1_DSICLK_SEL_BMSK                                                    0x3
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CLK_CFG1_DSICLK_SEL_SHFT                                                    0x0

#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_CTRL_ADDR                                                       (DSI_1_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x00000018)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_CTRL_RMSK                                                             0x5f
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_CTRL_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_CTRL_ADDR, HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_CTRL_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_CTRL_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_CTRL_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_CTRL_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_CTRL_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_CTRL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_CTRL_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_CTRL_IN)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_CTRL_PHY_MODE_BMSK                                                    0x40
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_CTRL_PHY_MODE_SHFT                                                     0x6
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_CTRL_BYTECLK_SEL_BMSK                                                 0x18
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_CTRL_BYTECLK_SEL_SHFT                                                  0x3
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_CTRL_POSTMAPPER_ENABLE_BMSK                                            0x4
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_CTRL_POSTMAPPER_ENABLE_SHFT                                            0x2
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_CTRL_TEST_ESCCLK_SEL_BMSK                                              0x2
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_CTRL_TEST_ESCCLK_SEL_SHFT                                              0x1
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_CTRL_TEST_BYTECLK_SEL_BMSK                                             0x1
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_CTRL_TEST_BYTECLK_SEL_SHFT                                             0x0

#define HWIO_DSI_1_PHY_DSIPHY_CMN_RBUF_CTRL_ADDR                                                       (DSI_1_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x0000001c)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_RBUF_CTRL_RMSK                                                              0x3
#define HWIO_DSI_1_PHY_DSIPHY_CMN_RBUF_CTRL_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_RBUF_CTRL_ADDR, HWIO_DSI_1_PHY_DSIPHY_CMN_RBUF_CTRL_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_RBUF_CTRL_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_RBUF_CTRL_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_RBUF_CTRL_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_CMN_RBUF_CTRL_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_RBUF_CTRL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_CMN_RBUF_CTRL_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_CMN_RBUF_CTRL_IN)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_RBUF_CTRL_FORCE_RETIME_BUFFER_RESYNC_BMSK                                   0x2
#define HWIO_DSI_1_PHY_DSIPHY_CMN_RBUF_CTRL_FORCE_RETIME_BUFFER_RESYNC_SHFT                                   0x1
#define HWIO_DSI_1_PHY_DSIPHY_CMN_RBUF_CTRL_PULSE_RETIME_BUFFER_RESYNC_BMSK                                   0x1
#define HWIO_DSI_1_PHY_DSIPHY_CMN_RBUF_CTRL_PULSE_RETIME_BUFFER_RESYNC_SHFT                                   0x0

#define HWIO_DSI_1_PHY_DSIPHY_CMN_VREG_CTRL_0_ADDR                                                     (DSI_1_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x00000020)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_VREG_CTRL_0_RMSK                                                           0xff
#define HWIO_DSI_1_PHY_DSIPHY_CMN_VREG_CTRL_0_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_VREG_CTRL_0_ADDR, HWIO_DSI_1_PHY_DSIPHY_CMN_VREG_CTRL_0_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_VREG_CTRL_0_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_VREG_CTRL_0_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_VREG_CTRL_0_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_CMN_VREG_CTRL_0_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_VREG_CTRL_0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_CMN_VREG_CTRL_0_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_CMN_VREG_CTRL_0_IN)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_VREG_CTRL_0_VREG_CTRL_0P4_BMSK                                             0xff
#define HWIO_DSI_1_PHY_DSIPHY_CMN_VREG_CTRL_0_VREG_CTRL_0P4_SHFT                                              0x0

#define HWIO_DSI_1_PHY_DSIPHY_CMN_CTRL_0_ADDR                                                          (DSI_1_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x00000024)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CTRL_0_RMSK                                                                0x7f
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CTRL_0_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_CTRL_0_ADDR, HWIO_DSI_1_PHY_DSIPHY_CMN_CTRL_0_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CTRL_0_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_CTRL_0_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CTRL_0_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_CMN_CTRL_0_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CTRL_0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_CMN_CTRL_0_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_CMN_CTRL_0_IN)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CTRL_0_DSIPHY_DIGTOP_PWRDN_B_BMSK                                          0x40
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CTRL_0_DSIPHY_DIGTOP_PWRDN_B_SHFT                                           0x6
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CTRL_0_CMN_PLL_SHUTDOWNB_BMSK                                              0x20
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CTRL_0_CMN_PLL_SHUTDOWNB_SHFT                                               0x5
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CTRL_0_DSIPHY_DLN3_SHUTDOWNB_BMSK                                          0x10
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CTRL_0_DSIPHY_DLN3_SHUTDOWNB_SHFT                                           0x4
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CTRL_0_DSIPHY_DLN2_SHUTDOWNB_BMSK                                           0x8
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CTRL_0_DSIPHY_DLN2_SHUTDOWNB_SHFT                                           0x3
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CTRL_0_DSIPHY_CLK_SHUTDOWNB_BMSK                                            0x4
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CTRL_0_DSIPHY_CLK_SHUTDOWNB_SHFT                                            0x2
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CTRL_0_DSIPHY_DLN1_SHUTDOWNB_BMSK                                           0x2
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CTRL_0_DSIPHY_DLN1_SHUTDOWNB_SHFT                                           0x1
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CTRL_0_DSIPHY_DLN0_SHUTDOWNB_BMSK                                           0x1
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CTRL_0_DSIPHY_DLN0_SHUTDOWNB_SHFT                                           0x0

#define HWIO_DSI_1_PHY_DSIPHY_CMN_CTRL_1_ADDR                                                          (DSI_1_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x00000028)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CTRL_1_RMSK                                                                0xff
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CTRL_1_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_CTRL_1_ADDR, HWIO_DSI_1_PHY_DSIPHY_CMN_CTRL_1_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CTRL_1_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_CTRL_1_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CTRL_1_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_CMN_CTRL_1_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CTRL_1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_CMN_CTRL_1_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_CMN_CTRL_1_IN)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CTRL_1_DSIPHY_PLL_CORE_CONTROLREG_RESET_BMSK                               0x80
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CTRL_1_DSIPHY_PLL_CORE_CONTROLREG_RESET_SHFT                                0x7
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CTRL_1_DSIPHY_SW_RESET_BMSK                                                0x40
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CTRL_1_DSIPHY_SW_RESET_SHFT                                                 0x6
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CTRL_1_DSIPHY_PLL_SW_RESET_BMSK                                            0x20
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CTRL_1_DSIPHY_PLL_SW_RESET_SHFT                                             0x5
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CTRL_1_DSIPHY_DLN3_SW_RESET_BMSK                                           0x10
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CTRL_1_DSIPHY_DLN3_SW_RESET_SHFT                                            0x4
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CTRL_1_DSIPHY_DLN2_SW_RESET_BMSK                                            0x8
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CTRL_1_DSIPHY_DLN2_SW_RESET_SHFT                                            0x3
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CTRL_1_DSIPHY_CLK_SW_RESET_BMSK                                             0x4
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CTRL_1_DSIPHY_CLK_SW_RESET_SHFT                                             0x2
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CTRL_1_DSIPHY_DLN1_SW_RESET_BMSK                                            0x2
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CTRL_1_DSIPHY_DLN1_SW_RESET_SHFT                                            0x1
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CTRL_1_DSIPHY_DLN0_SW_RESET_BMSK                                            0x1
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CTRL_1_DSIPHY_DLN0_SW_RESET_SHFT                                            0x0

#define HWIO_DSI_1_PHY_DSIPHY_CMN_CTRL_2_ADDR                                                          (DSI_1_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x0000002c)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CTRL_2_RMSK                                                                0xff
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CTRL_2_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_CTRL_2_ADDR, HWIO_DSI_1_PHY_DSIPHY_CMN_CTRL_2_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CTRL_2_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_CTRL_2_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CTRL_2_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_CMN_CTRL_2_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CTRL_2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_CMN_CTRL_2_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_CMN_CTRL_2_IN)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CTRL_2_DCTRL_BMSK                                                          0xff
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CTRL_2_DCTRL_SHFT                                                           0x0

#define HWIO_DSI_1_PHY_DSIPHY_CMN_CTRL_3_ADDR                                                          (DSI_1_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x00000030)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CTRL_3_RMSK                                                                 0x7
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CTRL_3_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_CTRL_3_ADDR, HWIO_DSI_1_PHY_DSIPHY_CMN_CTRL_3_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CTRL_3_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_CTRL_3_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CTRL_3_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_CMN_CTRL_3_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CTRL_3_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_CMN_CTRL_3_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_CMN_CTRL_3_IN)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CTRL_3_SLEEP_B_BMSK                                                         0x4
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CTRL_3_SLEEP_B_SHFT                                                         0x2
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CTRL_3_NC_INTF_CLK_OUT_CLK_EN_BMSK                                          0x2
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CTRL_3_NC_INTF_CLK_OUT_CLK_EN_SHFT                                          0x1
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CTRL_3_DSIPHY_RQST_SEL_BMSK                                                 0x1
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CTRL_3_DSIPHY_RQST_SEL_SHFT                                                 0x0

#define HWIO_DSI_1_PHY_DSIPHY_CMN_LANE_CFG0_ADDR                                                       (DSI_1_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x00000034)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_LANE_CFG0_RMSK                                                             0xff
#define HWIO_DSI_1_PHY_DSIPHY_CMN_LANE_CFG0_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_LANE_CFG0_ADDR, HWIO_DSI_1_PHY_DSIPHY_CMN_LANE_CFG0_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_LANE_CFG0_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_LANE_CFG0_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_LANE_CFG0_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_CMN_LANE_CFG0_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_LANE_CFG0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_CMN_LANE_CFG0_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_CMN_LANE_CFG0_IN)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_LANE_CFG0_LOGICAL_LANE1_SEL_BMSK                                           0xf0
#define HWIO_DSI_1_PHY_DSIPHY_CMN_LANE_CFG0_LOGICAL_LANE1_SEL_SHFT                                            0x4
#define HWIO_DSI_1_PHY_DSIPHY_CMN_LANE_CFG0_LOGICAL_LANE0_SEL_BMSK                                            0xf
#define HWIO_DSI_1_PHY_DSIPHY_CMN_LANE_CFG0_LOGICAL_LANE0_SEL_SHFT                                            0x0

#define HWIO_DSI_1_PHY_DSIPHY_CMN_LANE_CFG1_ADDR                                                       (DSI_1_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x00000038)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_LANE_CFG1_RMSK                                                             0xff
#define HWIO_DSI_1_PHY_DSIPHY_CMN_LANE_CFG1_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_LANE_CFG1_ADDR, HWIO_DSI_1_PHY_DSIPHY_CMN_LANE_CFG1_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_LANE_CFG1_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_LANE_CFG1_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_LANE_CFG1_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_CMN_LANE_CFG1_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_LANE_CFG1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_CMN_LANE_CFG1_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_CMN_LANE_CFG1_IN)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_LANE_CFG1_LOGICAL_LANE3_SEL_BMSK                                           0xf0
#define HWIO_DSI_1_PHY_DSIPHY_CMN_LANE_CFG1_LOGICAL_LANE3_SEL_SHFT                                            0x4
#define HWIO_DSI_1_PHY_DSIPHY_CMN_LANE_CFG1_LOGICAL_LANE2_SEL_BMSK                                            0xf
#define HWIO_DSI_1_PHY_DSIPHY_CMN_LANE_CFG1_LOGICAL_LANE2_SEL_SHFT                                            0x0

#define HWIO_DSI_1_PHY_DSIPHY_CMN_PLL_CNTRL_ADDR                                                       (DSI_1_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x0000003c)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_PLL_CNTRL_RMSK                                                              0x7
#define HWIO_DSI_1_PHY_DSIPHY_CMN_PLL_CNTRL_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_PLL_CNTRL_ADDR, HWIO_DSI_1_PHY_DSIPHY_CMN_PLL_CNTRL_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_PLL_CNTRL_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_PLL_CNTRL_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_PLL_CNTRL_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_CMN_PLL_CNTRL_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_PLL_CNTRL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_CMN_PLL_CNTRL_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_CMN_PLL_CNTRL_IN)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_PLL_CNTRL_TSYNC_EN_BMSK                                                     0x4
#define HWIO_DSI_1_PHY_DSIPHY_CMN_PLL_CNTRL_TSYNC_EN_SHFT                                                     0x2
#define HWIO_DSI_1_PHY_DSIPHY_CMN_PLL_CNTRL_PLL_BIAS_EN_BMSK                                                  0x2
#define HWIO_DSI_1_PHY_DSIPHY_CMN_PLL_CNTRL_PLL_BIAS_EN_SHFT                                                  0x1
#define HWIO_DSI_1_PHY_DSIPHY_CMN_PLL_CNTRL_PLL_START_BMSK                                                    0x1
#define HWIO_DSI_1_PHY_DSIPHY_CMN_PLL_CNTRL_PLL_START_SHFT                                                    0x0

#define HWIO_DSI_1_PHY_DSIPHY_CMN_DPHY_SOT_ADDR                                                        (DSI_1_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x00000040)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_DPHY_SOT_RMSK                                                              0xff
#define HWIO_DSI_1_PHY_DSIPHY_CMN_DPHY_SOT_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_DPHY_SOT_ADDR, HWIO_DSI_1_PHY_DSIPHY_CMN_DPHY_SOT_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_DPHY_SOT_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_DPHY_SOT_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_DPHY_SOT_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_CMN_DPHY_SOT_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_DPHY_SOT_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_CMN_DPHY_SOT_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_CMN_DPHY_SOT_IN)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_DPHY_SOT_DPHY_SOT_BMSK                                                     0xff
#define HWIO_DSI_1_PHY_DSIPHY_CMN_DPHY_SOT_DPHY_SOT_SHFT                                                      0x0

#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PREAMBLE_FLIP_ADDR                                              (DSI_1_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x00000044)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PREAMBLE_FLIP_RMSK                                                    0x7f
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PREAMBLE_FLIP_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PREAMBLE_FLIP_ADDR, HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PREAMBLE_FLIP_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PREAMBLE_FLIP_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PREAMBLE_FLIP_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PREAMBLE_FLIP_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PREAMBLE_FLIP_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PREAMBLE_FLIP_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PREAMBLE_FLIP_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PREAMBLE_FLIP_IN)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PREAMBLE_FLIP_CPHY_PREAMBLE_FLIP_BMSK                                 0x7f
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PREAMBLE_FLIP_CPHY_PREAMBLE_FLIP_SHFT                                  0x0

#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PREAMBLE_ROTATE_ADDR                                            (DSI_1_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x00000048)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PREAMBLE_ROTATE_RMSK                                                  0x7f
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PREAMBLE_ROTATE_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PREAMBLE_ROTATE_ADDR, HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PREAMBLE_ROTATE_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PREAMBLE_ROTATE_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PREAMBLE_ROTATE_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PREAMBLE_ROTATE_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PREAMBLE_ROTATE_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PREAMBLE_ROTATE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PREAMBLE_ROTATE_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PREAMBLE_ROTATE_IN)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PREAMBLE_ROTATE_CPHY_PREAMBLE_ROTATE_BMSK                             0x7f
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PREAMBLE_ROTATE_CPHY_PREAMBLE_ROTATE_SHFT                              0x0

#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PREAMBLE_POLARITY_ADDR                                          (DSI_1_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x0000004c)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PREAMBLE_POLARITY_RMSK                                                0x7f
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PREAMBLE_POLARITY_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PREAMBLE_POLARITY_ADDR, HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PREAMBLE_POLARITY_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PREAMBLE_POLARITY_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PREAMBLE_POLARITY_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PREAMBLE_POLARITY_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PREAMBLE_POLARITY_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PREAMBLE_POLARITY_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PREAMBLE_POLARITY_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PREAMBLE_POLARITY_IN)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PREAMBLE_POLARITY_CPHY_PREAMBLE_POLARITY_BMSK                         0x7f
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PREAMBLE_POLARITY_CPHY_PREAMBLE_POLARITY_SHFT                          0x0

#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PROGSEQ_FLIP0_ADDR                                              (DSI_1_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x00000050)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PROGSEQ_FLIP0_RMSK                                                    0xff
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PROGSEQ_FLIP0_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PROGSEQ_FLIP0_ADDR, HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PROGSEQ_FLIP0_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PROGSEQ_FLIP0_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PROGSEQ_FLIP0_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PROGSEQ_FLIP0_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PROGSEQ_FLIP0_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PROGSEQ_FLIP0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PROGSEQ_FLIP0_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PROGSEQ_FLIP0_IN)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PROGSEQ_FLIP0_CPHY_PROGSEQ_FLIP_7_0_BMSK                              0xff
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PROGSEQ_FLIP0_CPHY_PROGSEQ_FLIP_7_0_SHFT                               0x0

#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PROGSEQ_ROTATE0_ADDR                                            (DSI_1_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x00000054)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PROGSEQ_ROTATE0_RMSK                                                  0xff
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PROGSEQ_ROTATE0_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PROGSEQ_ROTATE0_ADDR, HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PROGSEQ_ROTATE0_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PROGSEQ_ROTATE0_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PROGSEQ_ROTATE0_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PROGSEQ_ROTATE0_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PROGSEQ_ROTATE0_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PROGSEQ_ROTATE0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PROGSEQ_ROTATE0_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PROGSEQ_ROTATE0_IN)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PROGSEQ_ROTATE0_CPHY_PROGSEQ_ROTATE_7_0_BMSK                          0xff
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PROGSEQ_ROTATE0_CPHY_PROGSEQ_ROTATE_7_0_SHFT                           0x0

#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PROGSEQ_POLARITY0_ADDR                                          (DSI_1_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x00000058)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PROGSEQ_POLARITY0_RMSK                                                0xff
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PROGSEQ_POLARITY0_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PROGSEQ_POLARITY0_ADDR, HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PROGSEQ_POLARITY0_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PROGSEQ_POLARITY0_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PROGSEQ_POLARITY0_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PROGSEQ_POLARITY0_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PROGSEQ_POLARITY0_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PROGSEQ_POLARITY0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PROGSEQ_POLARITY0_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PROGSEQ_POLARITY0_IN)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PROGSEQ_POLARITY0_CPHY_PROGSEQ_POLARITY_7_0_BMSK                      0xff
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PROGSEQ_POLARITY0_CPHY_PROGSEQ_POLARITY_7_0_SHFT                       0x0

#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PROGSEQ_FLIP1_ADDR                                              (DSI_1_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x0000005c)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PROGSEQ_FLIP1_RMSK                                                    0x3f
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PROGSEQ_FLIP1_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PROGSEQ_FLIP1_ADDR, HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PROGSEQ_FLIP1_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PROGSEQ_FLIP1_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PROGSEQ_FLIP1_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PROGSEQ_FLIP1_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PROGSEQ_FLIP1_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PROGSEQ_FLIP1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PROGSEQ_FLIP1_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PROGSEQ_FLIP1_IN)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PROGSEQ_FLIP1_CPHY_PROGSEQ_FLIP_13_8_BMSK                             0x3f
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PROGSEQ_FLIP1_CPHY_PROGSEQ_FLIP_13_8_SHFT                              0x0

#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PROGSEQ_ROTATE1_ADDR                                            (DSI_1_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x00000060)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PROGSEQ_ROTATE1_RMSK                                                  0x3f
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PROGSEQ_ROTATE1_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PROGSEQ_ROTATE1_ADDR, HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PROGSEQ_ROTATE1_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PROGSEQ_ROTATE1_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PROGSEQ_ROTATE1_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PROGSEQ_ROTATE1_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PROGSEQ_ROTATE1_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PROGSEQ_ROTATE1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PROGSEQ_ROTATE1_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PROGSEQ_ROTATE1_IN)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PROGSEQ_ROTATE1_CPHY_PROGSEQ_ROTATE_13_8_BMSK                         0x3f
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PROGSEQ_ROTATE1_CPHY_PROGSEQ_ROTATE_13_8_SHFT                          0x0

#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PROGSEQ_POLARITY1_ADDR                                          (DSI_1_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x00000064)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PROGSEQ_POLARITY1_RMSK                                                0x3f
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PROGSEQ_POLARITY1_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PROGSEQ_POLARITY1_ADDR, HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PROGSEQ_POLARITY1_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PROGSEQ_POLARITY1_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PROGSEQ_POLARITY1_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PROGSEQ_POLARITY1_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PROGSEQ_POLARITY1_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PROGSEQ_POLARITY1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PROGSEQ_POLARITY1_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PROGSEQ_POLARITY1_IN)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PROGSEQ_POLARITY1_CPHY_PROGSEQ_POLARITY_13_8_BMSK                     0x3f
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_PROGSEQ_POLARITY1_CPHY_PROGSEQ_POLARITY_13_8_SHFT                      0x0

#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_SYNC_FLIP_ADDR                                                  (DSI_1_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x00000068)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_SYNC_FLIP_RMSK                                                        0x7f
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_SYNC_FLIP_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_SYNC_FLIP_ADDR, HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_SYNC_FLIP_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_SYNC_FLIP_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_SYNC_FLIP_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_SYNC_FLIP_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_SYNC_FLIP_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_SYNC_FLIP_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_SYNC_FLIP_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_SYNC_FLIP_IN)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_SYNC_FLIP_CPHY_SYNC_FLIP_BMSK                                         0x7f
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_SYNC_FLIP_CPHY_SYNC_FLIP_SHFT                                          0x0

#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_SYNC_ROTATE_ADDR                                                (DSI_1_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x0000006c)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_SYNC_ROTATE_RMSK                                                      0x7f
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_SYNC_ROTATE_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_SYNC_ROTATE_ADDR, HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_SYNC_ROTATE_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_SYNC_ROTATE_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_SYNC_ROTATE_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_SYNC_ROTATE_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_SYNC_ROTATE_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_SYNC_ROTATE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_SYNC_ROTATE_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_SYNC_ROTATE_IN)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_SYNC_ROTATE_CPHY_SYNC_ROTATE_BMSK                                     0x7f
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_SYNC_ROTATE_CPHY_SYNC_ROTATE_SHFT                                      0x0

#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_SYNC_POLARITY_ADDR                                              (DSI_1_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x00000070)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_SYNC_POLARITY_RMSK                                                    0x7f
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_SYNC_POLARITY_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_SYNC_POLARITY_ADDR, HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_SYNC_POLARITY_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_SYNC_POLARITY_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_SYNC_POLARITY_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_SYNC_POLARITY_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_SYNC_POLARITY_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_SYNC_POLARITY_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_SYNC_POLARITY_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_SYNC_POLARITY_IN)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_SYNC_POLARITY_CPHY_SYNC_POLARITY_BMSK                                 0x7f
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_SYNC_POLARITY_CPHY_SYNC_POLARITY_SHFT                                  0x0

#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_POST_FLIP_ADDR                                                  (DSI_1_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x00000074)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_POST_FLIP_RMSK                                                        0x7f
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_POST_FLIP_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_POST_FLIP_ADDR, HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_POST_FLIP_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_POST_FLIP_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_POST_FLIP_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_POST_FLIP_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_POST_FLIP_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_POST_FLIP_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_POST_FLIP_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_POST_FLIP_IN)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_POST_FLIP_CPHY_POST_FLIP_BMSK                                         0x7f
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_POST_FLIP_CPHY_POST_FLIP_SHFT                                          0x0

#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_POST_ROTATE_ADDR                                                (DSI_1_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x00000078)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_POST_ROTATE_RMSK                                                      0x7f
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_POST_ROTATE_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_POST_ROTATE_ADDR, HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_POST_ROTATE_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_POST_ROTATE_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_POST_ROTATE_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_POST_ROTATE_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_POST_ROTATE_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_POST_ROTATE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_POST_ROTATE_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_POST_ROTATE_IN)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_POST_ROTATE_CPHY_POST_ROTATE_BMSK                                     0x7f
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_POST_ROTATE_CPHY_POST_ROTATE_SHFT                                      0x0

#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_POST_POLARITY_ADDR                                              (DSI_1_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x0000007c)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_POST_POLARITY_RMSK                                                    0x7f
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_POST_POLARITY_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_POST_POLARITY_ADDR, HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_POST_POLARITY_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_POST_POLARITY_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_POST_POLARITY_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_POST_POLARITY_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_POST_POLARITY_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_POST_POLARITY_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_POST_POLARITY_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_POST_POLARITY_IN)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_POST_POLARITY_CPHY_POST_POLARITY_BMSK                                 0x7f
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_POST_POLARITY_CPHY_POST_POLARITY_SHFT                                  0x0

#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_LANE_CTRL0_ADDR                                                 (DSI_1_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x00000080)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_LANE_CTRL0_RMSK                                                        0x7
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_LANE_CTRL0_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_LANE_CTRL0_ADDR, HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_LANE_CTRL0_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_LANE_CTRL0_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_LANE_CTRL0_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_LANE_CTRL0_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_LANE_CTRL0_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_LANE_CTRL0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_LANE_CTRL0_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_LANE_CTRL0_IN)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_LANE_CTRL0_CPHY_DLN2_CONTINUOUS_CLK_MODE_BMSK                          0x4
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_LANE_CTRL0_CPHY_DLN2_CONTINUOUS_CLK_MODE_SHFT                          0x2
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_LANE_CTRL0_CPHY_DLN1_CONTINUOUS_CLK_MODE_BMSK                          0x2
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_LANE_CTRL0_CPHY_DLN1_CONTINUOUS_CLK_MODE_SHFT                          0x1
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_LANE_CTRL0_CPHY_DLN0_CONTINUOUS_CLK_MODE_BMSK                          0x1
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CPHY_LANE_CTRL0_CPHY_DLN0_CONTINUOUS_CLK_MODE_SHFT                          0x0

#define HWIO_DSI_1_PHY_DSIPHY_CMN_BIST_CTRL_ADDR                                                       (DSI_1_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x00000084)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_BIST_CTRL_RMSK                                                             0xff
#define HWIO_DSI_1_PHY_DSIPHY_CMN_BIST_CTRL_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_BIST_CTRL_ADDR, HWIO_DSI_1_PHY_DSIPHY_CMN_BIST_CTRL_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_BIST_CTRL_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_BIST_CTRL_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_BIST_CTRL_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_CMN_BIST_CTRL_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_BIST_CTRL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_CMN_BIST_CTRL_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_CMN_BIST_CTRL_IN)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_BIST_CTRL_FORCE_CNCK_TX_REQUEST_HS_BMSK                                    0x80
#define HWIO_DSI_1_PHY_DSIPHY_CMN_BIST_CTRL_FORCE_CNCK_TX_REQUEST_HS_SHFT                                     0x7
#define HWIO_DSI_1_PHY_DSIPHY_CMN_BIST_CTRL_LP_BIST_PRBS_POLY_BMSK                                           0x60
#define HWIO_DSI_1_PHY_DSIPHY_CMN_BIST_CTRL_LP_BIST_PRBS_POLY_SHFT                                            0x5
#define HWIO_DSI_1_PHY_DSIPHY_CMN_BIST_CTRL_LP_BISTGEN_ENABLE_BMSK                                           0x10
#define HWIO_DSI_1_PHY_DSIPHY_CMN_BIST_CTRL_LP_BISTGEN_ENABLE_SHFT                                            0x4
#define HWIO_DSI_1_PHY_DSIPHY_CMN_BIST_CTRL_BIST_MODE_BMSK                                                    0x8
#define HWIO_DSI_1_PHY_DSIPHY_CMN_BIST_CTRL_BIST_MODE_SHFT                                                    0x3
#define HWIO_DSI_1_PHY_DSIPHY_CMN_BIST_CTRL_BISTGEN_ERROR_INJECT_BMSK                                         0x4
#define HWIO_DSI_1_PHY_DSIPHY_CMN_BIST_CTRL_BISTGEN_ERROR_INJECT_SHFT                                         0x2
#define HWIO_DSI_1_PHY_DSIPHY_CMN_BIST_CTRL_HS_BISTGEN_ENABLE_BMSK                                            0x2
#define HWIO_DSI_1_PHY_DSIPHY_CMN_BIST_CTRL_HS_BISTGEN_ENABLE_SHFT                                            0x1
#define HWIO_DSI_1_PHY_DSIPHY_CMN_BIST_CTRL_HS_BISTGEN_CROSS_BMSK                                             0x1
#define HWIO_DSI_1_PHY_DSIPHY_CMN_BIST_CTRL_HS_BISTGEN_CROSS_SHFT                                             0x0

#define HWIO_DSI_1_PHY_DSIPHY_CMN_BIST_SEED0_ADDR                                                      (DSI_1_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x00000088)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_BIST_SEED0_RMSK                                                            0xff
#define HWIO_DSI_1_PHY_DSIPHY_CMN_BIST_SEED0_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_BIST_SEED0_ADDR, HWIO_DSI_1_PHY_DSIPHY_CMN_BIST_SEED0_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_BIST_SEED0_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_BIST_SEED0_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_BIST_SEED0_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_CMN_BIST_SEED0_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_BIST_SEED0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_CMN_BIST_SEED0_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_CMN_BIST_SEED0_IN)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_BIST_SEED0_BIST_PRBS_SEED_7_0_BMSK                                         0xff
#define HWIO_DSI_1_PHY_DSIPHY_CMN_BIST_SEED0_BIST_PRBS_SEED_7_0_SHFT                                          0x0

#define HWIO_DSI_1_PHY_DSIPHY_CMN_BIST_SEED1_ADDR                                                      (DSI_1_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x0000008c)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_BIST_SEED1_RMSK                                                            0xff
#define HWIO_DSI_1_PHY_DSIPHY_CMN_BIST_SEED1_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_BIST_SEED1_ADDR, HWIO_DSI_1_PHY_DSIPHY_CMN_BIST_SEED1_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_BIST_SEED1_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_BIST_SEED1_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_BIST_SEED1_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_CMN_BIST_SEED1_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_BIST_SEED1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_CMN_BIST_SEED1_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_CMN_BIST_SEED1_IN)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_BIST_SEED1_BIST_PRBS_SEED_15_8_BMSK                                        0xff
#define HWIO_DSI_1_PHY_DSIPHY_CMN_BIST_SEED1_BIST_PRBS_SEED_15_8_SHFT                                         0x0

#define HWIO_DSI_1_PHY_DSIPHY_CMN_BIST_SEED2_ADDR                                                      (DSI_1_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x00000090)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_BIST_SEED2_RMSK                                                            0xff
#define HWIO_DSI_1_PHY_DSIPHY_CMN_BIST_SEED2_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_BIST_SEED2_ADDR, HWIO_DSI_1_PHY_DSIPHY_CMN_BIST_SEED2_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_BIST_SEED2_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_BIST_SEED2_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_BIST_SEED2_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_CMN_BIST_SEED2_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_BIST_SEED2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_CMN_BIST_SEED2_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_CMN_BIST_SEED2_IN)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_BIST_SEED2_BIST_PRBS_SEED_23_16_BMSK                                       0xff
#define HWIO_DSI_1_PHY_DSIPHY_CMN_BIST_SEED2_BIST_PRBS_SEED_23_16_SHFT                                        0x0

#define HWIO_DSI_1_PHY_DSIPHY_CMN_BIST_POLY_ADDR                                                       (DSI_1_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x00000094)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_BIST_POLY_RMSK                                                             0xff
#define HWIO_DSI_1_PHY_DSIPHY_CMN_BIST_POLY_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_BIST_POLY_ADDR, HWIO_DSI_1_PHY_DSIPHY_CMN_BIST_POLY_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_BIST_POLY_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_BIST_POLY_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_BIST_POLY_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_CMN_BIST_POLY_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_BIST_POLY_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_CMN_BIST_POLY_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_CMN_BIST_POLY_IN)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_BIST_POLY_BIST_PRBS_POLY_BMSK                                              0xff
#define HWIO_DSI_1_PHY_DSIPHY_CMN_BIST_POLY_BIST_PRBS_POLY_SHFT                                               0x0

#define HWIO_DSI_1_PHY_DSIPHY_CMN_MISR_CTRL_ADDR                                                       (DSI_1_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x00000098)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_MISR_CTRL_RMSK                                                             0x3f
#define HWIO_DSI_1_PHY_DSIPHY_CMN_MISR_CTRL_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_MISR_CTRL_ADDR, HWIO_DSI_1_PHY_DSIPHY_CMN_MISR_CTRL_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_MISR_CTRL_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_MISR_CTRL_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_MISR_CTRL_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_CMN_MISR_CTRL_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_MISR_CTRL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_CMN_MISR_CTRL_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_CMN_MISR_CTRL_IN)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_MISR_CTRL_DIGTOP_TEST_CLK_DEBUG_SEL_BMSK                                   0x3c
#define HWIO_DSI_1_PHY_DSIPHY_CMN_MISR_CTRL_DIGTOP_TEST_CLK_DEBUG_SEL_SHFT                                    0x2
#define HWIO_DSI_1_PHY_DSIPHY_CMN_MISR_CTRL_MISR_CLEAR_BMSK                                                   0x2
#define HWIO_DSI_1_PHY_DSIPHY_CMN_MISR_CTRL_MISR_CLEAR_SHFT                                                   0x1
#define HWIO_DSI_1_PHY_DSIPHY_CMN_MISR_CTRL_MISR_EN_BMSK                                                      0x1
#define HWIO_DSI_1_PHY_DSIPHY_CMN_MISR_CTRL_MISR_EN_SHFT                                                      0x0

#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_DEBUG_SEL_ADDR                                           (DSI_1_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x0000009c)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_DEBUG_SEL_RMSK                                                 0xff
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_DEBUG_SEL_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_DEBUG_SEL_ADDR, HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_DEBUG_SEL_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_DEBUG_SEL_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_DEBUG_SEL_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_DEBUG_SEL_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_DEBUG_SEL_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_DEBUG_SEL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_DEBUG_SEL_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_DEBUG_SEL_IN)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_DEBUG_SEL_DIGTOP_AMUX_SEL_BMSK                                 0xc0
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_DEBUG_SEL_DIGTOP_AMUX_SEL_SHFT                                  0x6
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_DEBUG_SEL_DIGTOP_DEBUG_SEL_BMSK                                0x3f
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_DEBUG_SEL_DIGTOP_DEBUG_SEL_SHFT                                 0x0

#define HWIO_DSI_1_PHY_DSIPHY_CMN_DSI_LANE_CTRL0_ADDR                                                  (DSI_1_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x000000a0)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_DSI_LANE_CTRL0_RMSK                                                        0x1f
#define HWIO_DSI_1_PHY_DSIPHY_CMN_DSI_LANE_CTRL0_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_DSI_LANE_CTRL0_ADDR, HWIO_DSI_1_PHY_DSIPHY_CMN_DSI_LANE_CTRL0_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_DSI_LANE_CTRL0_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_DSI_LANE_CTRL0_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_DSI_LANE_CTRL0_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_CMN_DSI_LANE_CTRL0_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_DSI_LANE_CTRL0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_CMN_DSI_LANE_CTRL0_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_CMN_DSI_LANE_CTRL0_IN)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_DSI_LANE_CTRL0_CLKLN_EN_BMSK                                               0x10
#define HWIO_DSI_1_PHY_DSIPHY_CMN_DSI_LANE_CTRL0_CLKLN_EN_SHFT                                                0x4
#define HWIO_DSI_1_PHY_DSIPHY_CMN_DSI_LANE_CTRL0_DLN3_EN_BMSK                                                 0x8
#define HWIO_DSI_1_PHY_DSIPHY_CMN_DSI_LANE_CTRL0_DLN3_EN_SHFT                                                 0x3
#define HWIO_DSI_1_PHY_DSIPHY_CMN_DSI_LANE_CTRL0_DLN2_EN_BMSK                                                 0x4
#define HWIO_DSI_1_PHY_DSIPHY_CMN_DSI_LANE_CTRL0_DLN2_EN_SHFT                                                 0x2
#define HWIO_DSI_1_PHY_DSIPHY_CMN_DSI_LANE_CTRL0_DLN1_EN_BMSK                                                 0x2
#define HWIO_DSI_1_PHY_DSIPHY_CMN_DSI_LANE_CTRL0_DLN1_EN_SHFT                                                 0x1
#define HWIO_DSI_1_PHY_DSIPHY_CMN_DSI_LANE_CTRL0_DLN0_EN_BMSK                                                 0x1
#define HWIO_DSI_1_PHY_DSIPHY_CMN_DSI_LANE_CTRL0_DLN0_EN_SHFT                                                 0x0

#define HWIO_DSI_1_PHY_DSIPHY_CMN_DSI_LANE_CTRL1_ADDR                                                  (DSI_1_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x000000a4)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_DSI_LANE_CTRL1_RMSK                                                        0x7f
#define HWIO_DSI_1_PHY_DSIPHY_CMN_DSI_LANE_CTRL1_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_DSI_LANE_CTRL1_ADDR, HWIO_DSI_1_PHY_DSIPHY_CMN_DSI_LANE_CTRL1_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_DSI_LANE_CTRL1_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_DSI_LANE_CTRL1_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_DSI_LANE_CTRL1_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_CMN_DSI_LANE_CTRL1_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_DSI_LANE_CTRL1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_CMN_DSI_LANE_CTRL1_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_CMN_DSI_LANE_CTRL1_IN)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_DSI_LANE_CTRL1_DPHY_CONTNSCLKMODE_CNCK_TX_RQST_HS_BMSK                     0x40
#define HWIO_DSI_1_PHY_DSIPHY_CMN_DSI_LANE_CTRL1_DPHY_CONTNSCLKMODE_CNCK_TX_RQST_HS_SHFT                      0x6
#define HWIO_DSI_1_PHY_DSIPHY_CMN_DSI_LANE_CTRL1_DPHY_CONTINUOUS_CLK_MODE_BMSK                               0x20
#define HWIO_DSI_1_PHY_DSIPHY_CMN_DSI_LANE_CTRL1_DPHY_CONTINUOUS_CLK_MODE_SHFT                                0x5
#define HWIO_DSI_1_PHY_DSIPHY_CMN_DSI_LANE_CTRL1_CLKLN_ULPS_REQUEST_BMSK                                     0x10
#define HWIO_DSI_1_PHY_DSIPHY_CMN_DSI_LANE_CTRL1_CLKLN_ULPS_REQUEST_SHFT                                      0x4
#define HWIO_DSI_1_PHY_DSIPHY_CMN_DSI_LANE_CTRL1_DLN3_ULPS_REQUEST_BMSK                                       0x8
#define HWIO_DSI_1_PHY_DSIPHY_CMN_DSI_LANE_CTRL1_DLN3_ULPS_REQUEST_SHFT                                       0x3
#define HWIO_DSI_1_PHY_DSIPHY_CMN_DSI_LANE_CTRL1_DLN2_ULPS_REQUEST_BMSK                                       0x4
#define HWIO_DSI_1_PHY_DSIPHY_CMN_DSI_LANE_CTRL1_DLN2_ULPS_REQUEST_SHFT                                       0x2
#define HWIO_DSI_1_PHY_DSIPHY_CMN_DSI_LANE_CTRL1_DLN1_ULPS_REQUEST_BMSK                                       0x2
#define HWIO_DSI_1_PHY_DSIPHY_CMN_DSI_LANE_CTRL1_DLN1_ULPS_REQUEST_SHFT                                       0x1
#define HWIO_DSI_1_PHY_DSIPHY_CMN_DSI_LANE_CTRL1_DLN0_ULPS_REQUEST_BMSK                                       0x1
#define HWIO_DSI_1_PHY_DSIPHY_CMN_DSI_LANE_CTRL1_DLN0_ULPS_REQUEST_SHFT                                       0x0

#define HWIO_DSI_1_PHY_DSIPHY_CMN_DSI_LANE_CTRL2_ADDR                                                  (DSI_1_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x000000a8)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_DSI_LANE_CTRL2_RMSK                                                        0x1f
#define HWIO_DSI_1_PHY_DSIPHY_CMN_DSI_LANE_CTRL2_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_DSI_LANE_CTRL2_ADDR, HWIO_DSI_1_PHY_DSIPHY_CMN_DSI_LANE_CTRL2_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_DSI_LANE_CTRL2_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_DSI_LANE_CTRL2_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_DSI_LANE_CTRL2_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_CMN_DSI_LANE_CTRL2_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_DSI_LANE_CTRL2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_CMN_DSI_LANE_CTRL2_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_CMN_DSI_LANE_CTRL2_IN)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_DSI_LANE_CTRL2_CLKLN_ULPS_EXIT_BMSK                                        0x10
#define HWIO_DSI_1_PHY_DSIPHY_CMN_DSI_LANE_CTRL2_CLKLN_ULPS_EXIT_SHFT                                         0x4
#define HWIO_DSI_1_PHY_DSIPHY_CMN_DSI_LANE_CTRL2_DLN3_ULPS_EXIT_BMSK                                          0x8
#define HWIO_DSI_1_PHY_DSIPHY_CMN_DSI_LANE_CTRL2_DLN3_ULPS_EXIT_SHFT                                          0x3
#define HWIO_DSI_1_PHY_DSIPHY_CMN_DSI_LANE_CTRL2_DLN2_ULPS_EXIT_BMSK                                          0x4
#define HWIO_DSI_1_PHY_DSIPHY_CMN_DSI_LANE_CTRL2_DLN2_ULPS_EXIT_SHFT                                          0x2
#define HWIO_DSI_1_PHY_DSIPHY_CMN_DSI_LANE_CTRL2_DLN1_ULPS_EXIT_BMSK                                          0x2
#define HWIO_DSI_1_PHY_DSIPHY_CMN_DSI_LANE_CTRL2_DLN1_ULPS_EXIT_SHFT                                          0x1
#define HWIO_DSI_1_PHY_DSIPHY_CMN_DSI_LANE_CTRL2_DLN0_ULPS_EXIT_BMSK                                          0x1
#define HWIO_DSI_1_PHY_DSIPHY_CMN_DSI_LANE_CTRL2_DLN0_ULPS_EXIT_SHFT                                          0x0

#define HWIO_DSI_1_PHY_DSIPHY_CMN_DSI_LANE_CTRL3_ADDR                                                  (DSI_1_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x000000ac)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_DSI_LANE_CTRL3_RMSK                                                        0x1f
#define HWIO_DSI_1_PHY_DSIPHY_CMN_DSI_LANE_CTRL3_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_DSI_LANE_CTRL3_ADDR, HWIO_DSI_1_PHY_DSIPHY_CMN_DSI_LANE_CTRL3_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_DSI_LANE_CTRL3_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_DSI_LANE_CTRL3_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_DSI_LANE_CTRL3_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_CMN_DSI_LANE_CTRL3_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_DSI_LANE_CTRL3_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_CMN_DSI_LANE_CTRL3_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_CMN_DSI_LANE_CTRL3_IN)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_DSI_LANE_CTRL3_CLKLN_FORCE_TX_STOP_BMSK                                    0x10
#define HWIO_DSI_1_PHY_DSIPHY_CMN_DSI_LANE_CTRL3_CLKLN_FORCE_TX_STOP_SHFT                                     0x4
#define HWIO_DSI_1_PHY_DSIPHY_CMN_DSI_LANE_CTRL3_DLN3_FORCE_TX_STOP_BMSK                                      0x8
#define HWIO_DSI_1_PHY_DSIPHY_CMN_DSI_LANE_CTRL3_DLN3_FORCE_TX_STOP_SHFT                                      0x3
#define HWIO_DSI_1_PHY_DSIPHY_CMN_DSI_LANE_CTRL3_DLN2_FORCE_TX_STOP_BMSK                                      0x4
#define HWIO_DSI_1_PHY_DSIPHY_CMN_DSI_LANE_CTRL3_DLN2_FORCE_TX_STOP_SHFT                                      0x2
#define HWIO_DSI_1_PHY_DSIPHY_CMN_DSI_LANE_CTRL3_DLN1_FORCE_TX_STOP_BMSK                                      0x2
#define HWIO_DSI_1_PHY_DSIPHY_CMN_DSI_LANE_CTRL3_DLN1_FORCE_TX_STOP_SHFT                                      0x1
#define HWIO_DSI_1_PHY_DSIPHY_CMN_DSI_LANE_CTRL3_DLN0_FORCE_TX_STOP_BMSK                                      0x1
#define HWIO_DSI_1_PHY_DSIPHY_CMN_DSI_LANE_CTRL3_DLN0_FORCE_TX_STOP_SHFT                                      0x0

#define HWIO_DSI_1_PHY_DSIPHY_CMN_DSI_LANE_CTRL4_ADDR                                                  (DSI_1_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x000000b0)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_DSI_LANE_CTRL4_RMSK                                                        0xff
#define HWIO_DSI_1_PHY_DSIPHY_CMN_DSI_LANE_CTRL4_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_DSI_LANE_CTRL4_ADDR, HWIO_DSI_1_PHY_DSIPHY_CMN_DSI_LANE_CTRL4_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_DSI_LANE_CTRL4_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_DSI_LANE_CTRL4_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_DSI_LANE_CTRL4_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_CMN_DSI_LANE_CTRL4_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_DSI_LANE_CTRL4_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_CMN_DSI_LANE_CTRL4_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_CMN_DSI_LANE_CTRL4_IN)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_DSI_LANE_CTRL4_DIGTOP_DEBUG_CLK_DEBUG_SEL_BMSK                             0xf0
#define HWIO_DSI_1_PHY_DSIPHY_CMN_DSI_LANE_CTRL4_DIGTOP_DEBUG_CLK_DEBUG_SEL_SHFT                              0x4
#define HWIO_DSI_1_PHY_DSIPHY_CMN_DSI_LANE_CTRL4_TRIGGER_BMSK                                                 0xf
#define HWIO_DSI_1_PHY_DSIPHY_CMN_DSI_LANE_CTRL4_TRIGGER_SHFT                                                 0x0

#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_0_ADDR                                                   (DSI_1_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x000000b4)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_0_RMSK                                                          0x3
#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_0_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_0_ADDR, HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_0_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_0_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_0_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_0_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_0_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_0_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_0_IN)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_0_HALFBYTECLK_CLKLN_EN_BMSK                                     0x2
#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_0_HALFBYTECLK_CLKLN_EN_SHFT                                     0x1
#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_0_HALFBYTECLK_DATALN_EN_BMSK                                    0x1
#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_0_HALFBYTECLK_DATALN_EN_SHFT                                    0x0

#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_1_ADDR                                                   (DSI_1_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x000000b8)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_1_RMSK                                                         0xff
#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_1_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_1_ADDR, HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_1_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_1_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_1_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_1_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_1_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_1_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_1_IN)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_1_DSIPHY_T_CLK_ZERO_BMSK                                       0xff
#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_1_DSIPHY_T_CLK_ZERO_SHFT                                        0x0

#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_2_ADDR                                                   (DSI_1_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x000000bc)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_2_RMSK                                                         0xff
#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_2_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_2_ADDR, HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_2_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_2_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_2_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_2_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_2_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_2_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_2_IN)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_2_DSIPHY_T_CLK_PREPARE_BMSK                                    0xff
#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_2_DSIPHY_T_CLK_PREPARE_SHFT                                     0x0

#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_3_ADDR                                                   (DSI_1_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x000000c0)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_3_RMSK                                                         0xff
#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_3_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_3_ADDR, HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_3_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_3_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_3_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_3_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_3_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_3_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_3_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_3_IN)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_3_DSIPHY_T_CLK_TRAIL_BMSK                                      0xff
#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_3_DSIPHY_T_CLK_TRAIL_SHFT                                       0x0

#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_4_ADDR                                                   (DSI_1_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x000000c4)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_4_RMSK                                                         0xff
#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_4_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_4_ADDR, HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_4_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_4_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_4_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_4_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_4_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_4_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_4_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_4_IN)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_4_DSIPHY_T_HS_EXIT_BMSK                                        0xff
#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_4_DSIPHY_T_HS_EXIT_SHFT                                         0x0

#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_5_ADDR                                                   (DSI_1_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x000000c8)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_5_RMSK                                                         0xff
#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_5_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_5_ADDR, HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_5_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_5_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_5_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_5_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_5_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_5_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_5_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_5_IN)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_5_DSIPHY_T_HS_ZERO_BMSK                                        0xff
#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_5_DSIPHY_T_HS_ZERO_SHFT                                         0x0

#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_6_ADDR                                                   (DSI_1_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x000000cc)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_6_RMSK                                                         0xff
#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_6_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_6_ADDR, HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_6_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_6_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_6_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_6_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_6_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_6_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_6_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_6_IN)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_6_DSIPHY_T_HS_PREPARE_BMSK                                     0xff
#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_6_DSIPHY_T_HS_PREPARE_SHFT                                      0x0

#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_7_ADDR                                                   (DSI_1_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x000000d0)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_7_RMSK                                                         0xff
#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_7_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_7_ADDR, HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_7_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_7_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_7_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_7_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_7_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_7_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_7_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_7_IN)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_7_DSIPHY_T_HS_TRAIL_BMSK                                       0xff
#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_7_DSIPHY_T_HS_TRAIL_SHFT                                        0x0

#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_8_ADDR                                                   (DSI_1_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x000000d4)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_8_RMSK                                                         0xff
#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_8_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_8_ADDR, HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_8_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_8_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_8_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_8_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_8_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_8_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_8_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_8_IN)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_8_DSIPHY_T_HS_RQST_BMSK                                        0xff
#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_8_DSIPHY_T_HS_RQST_SHFT                                         0x0

#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_9_ADDR                                                   (DSI_1_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x000000d8)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_9_RMSK                                                         0x3f
#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_9_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_9_ADDR, HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_9_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_9_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_9_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_9_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_9_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_9_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_9_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_9_IN)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_9_DSIPHY_T_TA_SURE_BMSK                                        0x38
#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_9_DSIPHY_T_TA_SURE_SHFT                                         0x3
#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_9_DSIPHY_T_TA_GO_BMSK                                           0x7
#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_9_DSIPHY_T_TA_GO_SHFT                                           0x0

#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_10_ADDR                                                  (DSI_1_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x000000dc)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_10_RMSK                                                         0x7
#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_10_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_10_ADDR, HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_10_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_10_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_10_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_10_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_10_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_10_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_10_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_10_IN)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_10_DSIPHY_T_TA_GET_BMSK                                         0x7
#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_10_DSIPHY_T_TA_GET_SHFT                                         0x0

#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_11_ADDR                                                  (DSI_1_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x000000e0)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_11_RMSK                                                        0xff
#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_11_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_11_ADDR, HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_11_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_11_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_11_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_11_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_11_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_11_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_11_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_11_IN)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_11_DSIPHY_TRIG3_CMD_BMSK                                       0xff
#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_11_DSIPHY_TRIG3_CMD_SHFT                                        0x0

#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_12_ADDR                                                  (DSI_1_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x000000e4)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_12_RMSK                                                        0xff
#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_12_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_12_ADDR, HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_12_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_12_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_12_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_12_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_12_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_12_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_12_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_12_IN)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_12_DSIPHY_T_CLK_PRE_BMSK                                       0xff
#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_12_DSIPHY_T_CLK_PRE_SHFT                                        0x0

#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_13_ADDR                                                  (DSI_1_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x000000e8)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_13_RMSK                                                        0xff
#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_13_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_13_ADDR, HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_13_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_13_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_13_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_13_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_13_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_13_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_13_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_13_IN)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_13_DSIPHY_T_CLK_POST_BMSK                                      0xff
#define HWIO_DSI_1_PHY_DSIPHY_CMN_TIMING_CTRL_13_DSIPHY_T_CLK_POST_SHFT                                       0x0

#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_HSTX_STR_CTRL_0_ADDR                                            (DSI_1_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x000000ec)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_HSTX_STR_CTRL_0_RMSK                                                  0xff
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_HSTX_STR_CTRL_0_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_HSTX_STR_CTRL_0_ADDR, HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_HSTX_STR_CTRL_0_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_HSTX_STR_CTRL_0_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_HSTX_STR_CTRL_0_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_HSTX_STR_CTRL_0_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_HSTX_STR_CTRL_0_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_HSTX_STR_CTRL_0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_HSTX_STR_CTRL_0_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_HSTX_STR_CTRL_0_IN)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_HSTX_STR_CTRL_0_DSIPHY_HSTX_STR_HSTOP_BMSK                            0xf0
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_HSTX_STR_CTRL_0_DSIPHY_HSTX_STR_HSTOP_SHFT                             0x4
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_HSTX_STR_CTRL_0_DSIPHY_HSTX_STR_HSBOT_BMSK                             0xf
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_HSTX_STR_CTRL_0_DSIPHY_HSTX_STR_HSBOT_SHFT                             0x0

#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_HSTX_STR_CTRL_1_ADDR                                            (DSI_1_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x000000f0)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_HSTX_STR_CTRL_1_RMSK                                                   0xf
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_HSTX_STR_CTRL_1_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_HSTX_STR_CTRL_1_ADDR, HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_HSTX_STR_CTRL_1_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_HSTX_STR_CTRL_1_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_HSTX_STR_CTRL_1_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_HSTX_STR_CTRL_1_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_HSTX_STR_CTRL_1_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_HSTX_STR_CTRL_1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_HSTX_STR_CTRL_1_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_HSTX_STR_CTRL_1_IN)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_HSTX_STR_CTRL_1_DSIPHY_HSTX_STR_HSMID_BMSK                             0xf
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_HSTX_STR_CTRL_1_DSIPHY_HSTX_STR_HSMID_SHFT                             0x0

#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_RESCODE_OFFSET_TOP_CTRL_ADDR                                    (DSI_1_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x000000f4)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_RESCODE_OFFSET_TOP_CTRL_RMSK                                          0x3f
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_RESCODE_OFFSET_TOP_CTRL_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_RESCODE_OFFSET_TOP_CTRL_ADDR, HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_RESCODE_OFFSET_TOP_CTRL_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_RESCODE_OFFSET_TOP_CTRL_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_RESCODE_OFFSET_TOP_CTRL_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_RESCODE_OFFSET_TOP_CTRL_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_RESCODE_OFFSET_TOP_CTRL_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_RESCODE_OFFSET_TOP_CTRL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_RESCODE_OFFSET_TOP_CTRL_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_RESCODE_OFFSET_TOP_CTRL_IN)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_RESCODE_OFFSET_TOP_CTRL_DSIPHY_RESCODE_OFFSET_TOP_BMSK                0x3f
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_RESCODE_OFFSET_TOP_CTRL_DSIPHY_RESCODE_OFFSET_TOP_SHFT                 0x0

#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_RESCODE_OFFSET_BOT_CTRL_ADDR                                    (DSI_1_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x000000f8)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_RESCODE_OFFSET_BOT_CTRL_RMSK                                          0x3f
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_RESCODE_OFFSET_BOT_CTRL_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_RESCODE_OFFSET_BOT_CTRL_ADDR, HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_RESCODE_OFFSET_BOT_CTRL_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_RESCODE_OFFSET_BOT_CTRL_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_RESCODE_OFFSET_BOT_CTRL_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_RESCODE_OFFSET_BOT_CTRL_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_RESCODE_OFFSET_BOT_CTRL_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_RESCODE_OFFSET_BOT_CTRL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_RESCODE_OFFSET_BOT_CTRL_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_RESCODE_OFFSET_BOT_CTRL_IN)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_RESCODE_OFFSET_BOT_CTRL_DSIPHY_RESCODE_OFFSET_BOT_BMSK                0x3f
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_RESCODE_OFFSET_BOT_CTRL_DSIPHY_RESCODE_OFFSET_BOT_SHFT                 0x0

#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_RESCODE_OFFSET_MID_CTRL_ADDR                                    (DSI_1_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x000000fc)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_RESCODE_OFFSET_MID_CTRL_RMSK                                          0x3f
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_RESCODE_OFFSET_MID_CTRL_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_RESCODE_OFFSET_MID_CTRL_ADDR, HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_RESCODE_OFFSET_MID_CTRL_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_RESCODE_OFFSET_MID_CTRL_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_RESCODE_OFFSET_MID_CTRL_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_RESCODE_OFFSET_MID_CTRL_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_RESCODE_OFFSET_MID_CTRL_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_RESCODE_OFFSET_MID_CTRL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_RESCODE_OFFSET_MID_CTRL_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_RESCODE_OFFSET_MID_CTRL_IN)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_RESCODE_OFFSET_MID_CTRL_DSIPHY_RESCODE_OFFSET_MID_BMSK                0x3f
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_RESCODE_OFFSET_MID_CTRL_DSIPHY_RESCODE_OFFSET_MID_SHFT                 0x0

#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_LPTX_STR_CTRL_ADDR                                              (DSI_1_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x00000100)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_LPTX_STR_CTRL_RMSK                                                    0xff
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_LPTX_STR_CTRL_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_LPTX_STR_CTRL_ADDR, HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_LPTX_STR_CTRL_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_LPTX_STR_CTRL_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_LPTX_STR_CTRL_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_LPTX_STR_CTRL_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_LPTX_STR_CTRL_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_LPTX_STR_CTRL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_LPTX_STR_CTRL_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_LPTX_STR_CTRL_IN)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_LPTX_STR_CTRL_DSIPHY_STR_LP_N_BMSK                                    0xf0
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_LPTX_STR_CTRL_DSIPHY_STR_LP_N_SHFT                                     0x4
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_LPTX_STR_CTRL_DSIPHY_STR_LP_P_BMSK                                     0xf
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_LPTX_STR_CTRL_DSIPHY_STR_LP_P_SHFT                                     0x0

#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_PEMPH_CTRL_0_ADDR                                               (DSI_1_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x00000104)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_PEMPH_CTRL_0_RMSK                                                     0xff
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_PEMPH_CTRL_0_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_PEMPH_CTRL_0_ADDR, HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_PEMPH_CTRL_0_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_PEMPH_CTRL_0_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_PEMPH_CTRL_0_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_PEMPH_CTRL_0_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_PEMPH_CTRL_0_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_PEMPH_CTRL_0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_PEMPH_CTRL_0_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_PEMPH_CTRL_0_IN)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_PEMPH_CTRL_0_DSIPHY_PEMPH_STRBOT_BMSK                                 0xf0
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_PEMPH_CTRL_0_DSIPHY_PEMPH_STRBOT_SHFT                                  0x4
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_PEMPH_CTRL_0_DSIPHY_PEMPH_STRTOP_BMSK                                  0xf
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_PEMPH_CTRL_0_DSIPHY_PEMPH_STRTOP_SHFT                                  0x0

#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_PEMPH_CTRL_1_ADDR                                               (DSI_1_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x00000108)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_PEMPH_CTRL_1_RMSK                                                      0xf
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_PEMPH_CTRL_1_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_PEMPH_CTRL_1_ADDR, HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_PEMPH_CTRL_1_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_PEMPH_CTRL_1_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_PEMPH_CTRL_1_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_PEMPH_CTRL_1_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_PEMPH_CTRL_1_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_PEMPH_CTRL_1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_PEMPH_CTRL_1_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_PEMPH_CTRL_1_IN)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_PEMPH_CTRL_1_DSIPHY_PEMPH_STRMID_BMSK                                  0xf
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_PEMPH_CTRL_1_DSIPHY_PEMPH_STRMID_SHFT                                  0x0

#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_STR_SWI_CAL_SEL_CTRL_ADDR                                       (DSI_1_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x0000010c)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_STR_SWI_CAL_SEL_CTRL_RMSK                                              0x7
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_STR_SWI_CAL_SEL_CTRL_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_STR_SWI_CAL_SEL_CTRL_ADDR, HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_STR_SWI_CAL_SEL_CTRL_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_STR_SWI_CAL_SEL_CTRL_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_STR_SWI_CAL_SEL_CTRL_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_STR_SWI_CAL_SEL_CTRL_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_STR_SWI_CAL_SEL_CTRL_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_STR_SWI_CAL_SEL_CTRL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_STR_SWI_CAL_SEL_CTRL_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_STR_SWI_CAL_SEL_CTRL_IN)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_STR_SWI_CAL_SEL_CTRL_DSIPHY_DEEMPH_SEL_BMSK                            0x4
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_STR_SWI_CAL_SEL_CTRL_DSIPHY_DEEMPH_SEL_SHFT                            0x2
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_STR_SWI_CAL_SEL_CTRL_DSIPHY_EQ_VALUE_OVERRIDE_BMSK                     0x2
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_STR_SWI_CAL_SEL_CTRL_DSIPHY_EQ_VALUE_OVERRIDE_SHFT                     0x1
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_STR_SWI_CAL_SEL_CTRL_DSIPHY_STR_VALUE_OVERRIDE_BMSK                    0x1
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_STR_SWI_CAL_SEL_CTRL_DSIPHY_STR_VALUE_OVERRIDE_SHFT                    0x0

#define HWIO_DSI_1_PHY_DSIPHY_CMN_VREG_CTRL_1_ADDR                                                     (DSI_1_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x00000110)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_VREG_CTRL_1_RMSK                                                           0xff
#define HWIO_DSI_1_PHY_DSIPHY_CMN_VREG_CTRL_1_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_VREG_CTRL_1_ADDR, HWIO_DSI_1_PHY_DSIPHY_CMN_VREG_CTRL_1_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_VREG_CTRL_1_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_VREG_CTRL_1_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_VREG_CTRL_1_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_CMN_VREG_CTRL_1_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_VREG_CTRL_1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_CMN_VREG_CTRL_1_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_CMN_VREG_CTRL_1_IN)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_VREG_CTRL_1_VREG_CTRL_0P2_BMSK                                             0xff
#define HWIO_DSI_1_PHY_DSIPHY_CMN_VREG_CTRL_1_VREG_CTRL_0P2_SHFT                                              0x0

#define HWIO_DSI_1_PHY_DSIPHY_CMN_CTRL_4_ADDR                                                          (DSI_1_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x00000114)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CTRL_4_RMSK                                                                0xff
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CTRL_4_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_CTRL_4_ADDR, HWIO_DSI_1_PHY_DSIPHY_CMN_CTRL_4_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CTRL_4_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_CTRL_4_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CTRL_4_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_CMN_CTRL_4_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CTRL_4_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_CMN_CTRL_4_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_CMN_CTRL_4_IN)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CTRL_4_DCTRL_GLBL_2_BMSK                                                   0xff
#define HWIO_DSI_1_PHY_DSIPHY_CMN_CTRL_4_DCTRL_GLBL_2_SHFT                                                    0x0

#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE0_ADDR                                              (DSI_1_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x00000118)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE0_RMSK                                                    0xff
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE0_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE0_ADDR, HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE0_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE0_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE0_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE0_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE0_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE0_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE0_IN)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE0_DIGTOP_SPARE_7_0_BMSK                                   0xff
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE0_DIGTOP_SPARE_7_0_SHFT                                    0x0

#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE1_ADDR                                              (DSI_1_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x0000011c)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE1_RMSK                                                    0xff
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE1_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE1_ADDR, HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE1_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE1_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE1_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE1_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE1_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE1_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE1_IN)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE1_DIGTOP_SPARE_15_8_BMSK                                  0xff
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE1_DIGTOP_SPARE_15_8_SHFT                                   0x0

#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE2_ADDR                                              (DSI_1_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x00000120)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE2_RMSK                                                    0xff
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE2_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE2_ADDR, HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE2_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE2_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE2_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE2_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE2_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE2_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE2_IN)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE2_DIGTOP_SPARE_23_16_BMSK                                 0xff
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE2_DIGTOP_SPARE_23_16_SHFT                                  0x0

#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE3_ADDR                                              (DSI_1_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x00000124)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE3_RMSK                                                    0xff
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE3_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE3_ADDR, HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE3_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE3_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE3_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE3_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE3_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE3_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE3_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE3_IN)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE3_DIGTOP_SPARE_31_24_BMSK                                 0xff
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE3_DIGTOP_SPARE_31_24_SHFT                                  0x0

#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE4_ADDR                                              (DSI_1_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x00000128)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE4_RMSK                                                    0xff
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE4_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE4_ADDR, HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE4_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE4_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE4_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE4_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE4_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE4_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE4_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE4_IN)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE4_DIGTOP_SPARE_39_32_BMSK                                 0xff
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE4_DIGTOP_SPARE_39_32_SHFT                                  0x0

#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE5_ADDR                                              (DSI_1_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x0000012c)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE5_RMSK                                                    0xff
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE5_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE5_ADDR, HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE5_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE5_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE5_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE5_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE5_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE5_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE5_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE5_IN)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE5_DIGTOP_SPARE_47_40_BMSK                                 0xff
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE5_DIGTOP_SPARE_47_40_SHFT                                  0x0

#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE6_ADDR                                              (DSI_1_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x00000130)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE6_RMSK                                                    0xff
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE6_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE6_ADDR, HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE6_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE6_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE6_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE6_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE6_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE6_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE6_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE6_IN)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE6_DIGTOP_SPARE_55_48_BMSK                                 0xff
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE6_DIGTOP_SPARE_55_48_SHFT                                  0x0

#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE7_ADDR                                              (DSI_1_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x00000134)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE7_RMSK                                                    0xff
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE7_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE7_ADDR, HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE7_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE7_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE7_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE7_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE7_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE7_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE7_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE7_IN)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE7_DIGTOP_SPARE_63_56_BMSK                                 0xff
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DIGTOP_SPARE7_DIGTOP_SPARE_63_56_SHFT                                  0x0

#define HWIO_DSI_1_PHY_DSIPHY_CMN_HSTX_STR_STATUS0_ADDR                                                (DSI_1_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x00000138)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_HSTX_STR_STATUS0_RMSK                                                      0xff
#define HWIO_DSI_1_PHY_DSIPHY_CMN_HSTX_STR_STATUS0_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_HSTX_STR_STATUS0_ADDR, HWIO_DSI_1_PHY_DSIPHY_CMN_HSTX_STR_STATUS0_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_HSTX_STR_STATUS0_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_HSTX_STR_STATUS0_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_HSTX_STR_STATUS0_DSIPHY_HSTX_STR_HSTOP_STATUS_BMSK                         0xf0
#define HWIO_DSI_1_PHY_DSIPHY_CMN_HSTX_STR_STATUS0_DSIPHY_HSTX_STR_HSTOP_STATUS_SHFT                          0x4
#define HWIO_DSI_1_PHY_DSIPHY_CMN_HSTX_STR_STATUS0_DSIPHY_HSTX_STR_HSBOT_STATUS_BMSK                          0xf
#define HWIO_DSI_1_PHY_DSIPHY_CMN_HSTX_STR_STATUS0_DSIPHY_HSTX_STR_HSBOT_STATUS_SHFT                          0x0

#define HWIO_DSI_1_PHY_DSIPHY_CMN_HSTX_STR_STATUS1_ADDR                                                (DSI_1_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x0000013c)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_HSTX_STR_STATUS1_RMSK                                                       0xf
#define HWIO_DSI_1_PHY_DSIPHY_CMN_HSTX_STR_STATUS1_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_HSTX_STR_STATUS1_ADDR, HWIO_DSI_1_PHY_DSIPHY_CMN_HSTX_STR_STATUS1_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_HSTX_STR_STATUS1_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_HSTX_STR_STATUS1_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_HSTX_STR_STATUS1_DSIPHY_HSTX_STR_HSMID_STATUS_BMSK                          0xf
#define HWIO_DSI_1_PHY_DSIPHY_CMN_HSTX_STR_STATUS1_DSIPHY_HSTX_STR_HSMID_STATUS_SHFT                          0x0

#define HWIO_DSI_1_PHY_DSIPHY_CMN_PHY_STATUS_ADDR                                                      (DSI_1_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x00000140)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_PHY_STATUS_RMSK                                                             0x7
#define HWIO_DSI_1_PHY_DSIPHY_CMN_PHY_STATUS_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_PHY_STATUS_ADDR, HWIO_DSI_1_PHY_DSIPHY_CMN_PHY_STATUS_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_PHY_STATUS_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_PHY_STATUS_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_PHY_STATUS_PHY_READY_BMSK                                                   0x4
#define HWIO_DSI_1_PHY_DSIPHY_CMN_PHY_STATUS_PHY_READY_SHFT                                                   0x2
#define HWIO_DSI_1_PHY_DSIPHY_CMN_PHY_STATUS_PLL_READY_BMSK                                                   0x2
#define HWIO_DSI_1_PHY_DSIPHY_CMN_PHY_STATUS_PLL_READY_SHFT                                                   0x1
#define HWIO_DSI_1_PHY_DSIPHY_CMN_PHY_STATUS_REFGEN_READY_BMSK                                                0x1
#define HWIO_DSI_1_PHY_DSIPHY_CMN_PHY_STATUS_REFGEN_READY_SHFT                                                0x0

#define HWIO_DSI_1_PHY_DSIPHY_CMN_RESCODE_STATUS_ADDR                                                  (DSI_1_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x00000144)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_RESCODE_STATUS_RMSK                                                        0x7f
#define HWIO_DSI_1_PHY_DSIPHY_CMN_RESCODE_STATUS_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_RESCODE_STATUS_ADDR, HWIO_DSI_1_PHY_DSIPHY_CMN_RESCODE_STATUS_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_RESCODE_STATUS_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_RESCODE_STATUS_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_RESCODE_STATUS_RESCODE_BMSK                                                0x7f
#define HWIO_DSI_1_PHY_DSIPHY_CMN_RESCODE_STATUS_RESCODE_SHFT                                                 0x0

#define HWIO_DSI_1_PHY_DSIPHY_CMN_LANE_STATUS0_ADDR                                                    (DSI_1_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x00000148)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_LANE_STATUS0_RMSK                                                          0x1f
#define HWIO_DSI_1_PHY_DSIPHY_CMN_LANE_STATUS0_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_LANE_STATUS0_ADDR, HWIO_DSI_1_PHY_DSIPHY_CMN_LANE_STATUS0_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_LANE_STATUS0_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_LANE_STATUS0_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_LANE_STATUS0_CLKLN_ULPS_ACTIVE_NOT_BMSK                                    0x10
#define HWIO_DSI_1_PHY_DSIPHY_CMN_LANE_STATUS0_CLKLN_ULPS_ACTIVE_NOT_SHFT                                     0x4
#define HWIO_DSI_1_PHY_DSIPHY_CMN_LANE_STATUS0_DLN3_ULPS_ACTIVE_NOT_BMSK                                      0x8
#define HWIO_DSI_1_PHY_DSIPHY_CMN_LANE_STATUS0_DLN3_ULPS_ACTIVE_NOT_SHFT                                      0x3
#define HWIO_DSI_1_PHY_DSIPHY_CMN_LANE_STATUS0_DLN2_ULPS_ACTIVE_NOT_BMSK                                      0x4
#define HWIO_DSI_1_PHY_DSIPHY_CMN_LANE_STATUS0_DLN2_ULPS_ACTIVE_NOT_SHFT                                      0x2
#define HWIO_DSI_1_PHY_DSIPHY_CMN_LANE_STATUS0_DLN1_ULPS_ACTIVE_NOT_BMSK                                      0x2
#define HWIO_DSI_1_PHY_DSIPHY_CMN_LANE_STATUS0_DLN1_ULPS_ACTIVE_NOT_SHFT                                      0x1
#define HWIO_DSI_1_PHY_DSIPHY_CMN_LANE_STATUS0_DLN0_ULPS_ACTIVE_NOT_BMSK                                      0x1
#define HWIO_DSI_1_PHY_DSIPHY_CMN_LANE_STATUS0_DLN0_ULPS_ACTIVE_NOT_SHFT                                      0x0

#define HWIO_DSI_1_PHY_DSIPHY_CMN_LANE_STATUS1_ADDR                                                    (DSI_1_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x0000014c)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_LANE_STATUS1_RMSK                                                          0x1f
#define HWIO_DSI_1_PHY_DSIPHY_CMN_LANE_STATUS1_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_LANE_STATUS1_ADDR, HWIO_DSI_1_PHY_DSIPHY_CMN_LANE_STATUS1_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_LANE_STATUS1_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_LANE_STATUS1_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_LANE_STATUS1_CLKLN_STOPSTATE_BMSK                                          0x10
#define HWIO_DSI_1_PHY_DSIPHY_CMN_LANE_STATUS1_CLKLN_STOPSTATE_SHFT                                           0x4
#define HWIO_DSI_1_PHY_DSIPHY_CMN_LANE_STATUS1_DLN3_STOPSTATE_BMSK                                            0x8
#define HWIO_DSI_1_PHY_DSIPHY_CMN_LANE_STATUS1_DLN3_STOPSTATE_SHFT                                            0x3
#define HWIO_DSI_1_PHY_DSIPHY_CMN_LANE_STATUS1_DLN2_STOPSTATE_BMSK                                            0x4
#define HWIO_DSI_1_PHY_DSIPHY_CMN_LANE_STATUS1_DLN2_STOPSTATE_SHFT                                            0x2
#define HWIO_DSI_1_PHY_DSIPHY_CMN_LANE_STATUS1_DLN1_STOPSTATE_BMSK                                            0x2
#define HWIO_DSI_1_PHY_DSIPHY_CMN_LANE_STATUS1_DLN1_STOPSTATE_SHFT                                            0x1
#define HWIO_DSI_1_PHY_DSIPHY_CMN_LANE_STATUS1_DLN0_STOPSTATE_BMSK                                            0x1
#define HWIO_DSI_1_PHY_DSIPHY_CMN_LANE_STATUS1_DLN0_STOPSTATE_SHFT                                            0x0

#define HWIO_DSI_1_PHY_DSIPHY_CMN_MISR_STATUS00_ADDR                                                   (DSI_1_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x00000150)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_MISR_STATUS00_RMSK                                                         0xff
#define HWIO_DSI_1_PHY_DSIPHY_CMN_MISR_STATUS00_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_MISR_STATUS00_ADDR, HWIO_DSI_1_PHY_DSIPHY_CMN_MISR_STATUS00_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_MISR_STATUS00_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_MISR_STATUS00_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_MISR_STATUS00_MISR_SIGNATURE0_7_0_BMSK                                     0xff
#define HWIO_DSI_1_PHY_DSIPHY_CMN_MISR_STATUS00_MISR_SIGNATURE0_7_0_SHFT                                      0x0

#define HWIO_DSI_1_PHY_DSIPHY_CMN_MISR_STATUS01_ADDR                                                   (DSI_1_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x00000154)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_MISR_STATUS01_RMSK                                                         0xff
#define HWIO_DSI_1_PHY_DSIPHY_CMN_MISR_STATUS01_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_MISR_STATUS01_ADDR, HWIO_DSI_1_PHY_DSIPHY_CMN_MISR_STATUS01_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_MISR_STATUS01_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_MISR_STATUS01_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_MISR_STATUS01_MISR_SIGNATURE0_15_8_BMSK                                    0xff
#define HWIO_DSI_1_PHY_DSIPHY_CMN_MISR_STATUS01_MISR_SIGNATURE0_15_8_SHFT                                     0x0

#define HWIO_DSI_1_PHY_DSIPHY_CMN_MISR_STATUS10_ADDR                                                   (DSI_1_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x00000158)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_MISR_STATUS10_RMSK                                                         0xff
#define HWIO_DSI_1_PHY_DSIPHY_CMN_MISR_STATUS10_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_MISR_STATUS10_ADDR, HWIO_DSI_1_PHY_DSIPHY_CMN_MISR_STATUS10_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_MISR_STATUS10_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_MISR_STATUS10_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_MISR_STATUS10_MISR_SIGNATURE1_7_0_BMSK                                     0xff
#define HWIO_DSI_1_PHY_DSIPHY_CMN_MISR_STATUS10_MISR_SIGNATURE1_7_0_SHFT                                      0x0

#define HWIO_DSI_1_PHY_DSIPHY_CMN_MISR_STATUS11_ADDR                                                   (DSI_1_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x0000015c)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_MISR_STATUS11_RMSK                                                         0xff
#define HWIO_DSI_1_PHY_DSIPHY_CMN_MISR_STATUS11_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_MISR_STATUS11_ADDR, HWIO_DSI_1_PHY_DSIPHY_CMN_MISR_STATUS11_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_MISR_STATUS11_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_MISR_STATUS11_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_MISR_STATUS11_MISR_SIGNATURE1_15_8_BMSK                                    0xff
#define HWIO_DSI_1_PHY_DSIPHY_CMN_MISR_STATUS11_MISR_SIGNATURE1_15_8_SHFT                                     0x0

#define HWIO_DSI_1_PHY_DSIPHY_CMN_MISR_STATUS20_ADDR                                                   (DSI_1_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x00000160)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_MISR_STATUS20_RMSK                                                         0xff
#define HWIO_DSI_1_PHY_DSIPHY_CMN_MISR_STATUS20_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_MISR_STATUS20_ADDR, HWIO_DSI_1_PHY_DSIPHY_CMN_MISR_STATUS20_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_MISR_STATUS20_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_MISR_STATUS20_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_MISR_STATUS20_MISR_SIGNATURE2_7_0_BMSK                                     0xff
#define HWIO_DSI_1_PHY_DSIPHY_CMN_MISR_STATUS20_MISR_SIGNATURE2_7_0_SHFT                                      0x0

#define HWIO_DSI_1_PHY_DSIPHY_CMN_MISR_STATUS21_ADDR                                                   (DSI_1_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x00000164)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_MISR_STATUS21_RMSK                                                         0xff
#define HWIO_DSI_1_PHY_DSIPHY_CMN_MISR_STATUS21_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_MISR_STATUS21_ADDR, HWIO_DSI_1_PHY_DSIPHY_CMN_MISR_STATUS21_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_MISR_STATUS21_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_MISR_STATUS21_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_MISR_STATUS21_MISR_SIGNATURE2_15_8_BMSK                                    0xff
#define HWIO_DSI_1_PHY_DSIPHY_CMN_MISR_STATUS21_MISR_SIGNATURE2_15_8_SHFT                                     0x0

#define HWIO_DSI_1_PHY_DSIPHY_CMN_MISR_STATUS30_ADDR                                                   (DSI_1_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x00000168)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_MISR_STATUS30_RMSK                                                         0xff
#define HWIO_DSI_1_PHY_DSIPHY_CMN_MISR_STATUS30_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_MISR_STATUS30_ADDR, HWIO_DSI_1_PHY_DSIPHY_CMN_MISR_STATUS30_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_MISR_STATUS30_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_MISR_STATUS30_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_MISR_STATUS30_MISR_SIGNATURE3_7_0_BMSK                                     0xff
#define HWIO_DSI_1_PHY_DSIPHY_CMN_MISR_STATUS30_MISR_SIGNATURE3_7_0_SHFT                                      0x0

#define HWIO_DSI_1_PHY_DSIPHY_CMN_MISR_STATUS31_ADDR                                                   (DSI_1_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x0000016c)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_MISR_STATUS31_RMSK                                                         0xff
#define HWIO_DSI_1_PHY_DSIPHY_CMN_MISR_STATUS31_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_MISR_STATUS31_ADDR, HWIO_DSI_1_PHY_DSIPHY_CMN_MISR_STATUS31_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_MISR_STATUS31_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_MISR_STATUS31_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_MISR_STATUS31_MISR_SIGNATURE3_15_8_BMSK                                    0xff
#define HWIO_DSI_1_PHY_DSIPHY_CMN_MISR_STATUS31_MISR_SIGNATURE3_15_8_SHFT                                     0x0

#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DEBUG_BUS0_ADDR                                                 (DSI_1_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x00000170)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DEBUG_BUS0_RMSK                                                       0xff
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DEBUG_BUS0_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DEBUG_BUS0_ADDR, HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DEBUG_BUS0_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DEBUG_BUS0_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DEBUG_BUS0_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DEBUG_BUS0_DEBUG_BUS_7_0_BMSK                                         0xff
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DEBUG_BUS0_DEBUG_BUS_7_0_SHFT                                          0x0

#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DEBUG_BUS1_ADDR                                                 (DSI_1_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x00000174)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DEBUG_BUS1_RMSK                                                       0xff
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DEBUG_BUS1_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DEBUG_BUS1_ADDR, HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DEBUG_BUS1_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DEBUG_BUS1_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DEBUG_BUS1_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DEBUG_BUS1_DEBUG_BUS_15_8_BMSK                                        0xff
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DEBUG_BUS1_DEBUG_BUS_15_8_SHFT                                         0x0

#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DEBUG_BUS2_ADDR                                                 (DSI_1_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x00000178)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DEBUG_BUS2_RMSK                                                       0xff
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DEBUG_BUS2_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DEBUG_BUS2_ADDR, HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DEBUG_BUS2_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DEBUG_BUS2_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DEBUG_BUS2_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DEBUG_BUS2_DEBUG_BUS_23_16_BMSK                                       0xff
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DEBUG_BUS2_DEBUG_BUS_23_16_SHFT                                        0x0

#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DEBUG_BUS3_ADDR                                                 (DSI_1_PHY_DSIPHY_CMN_DSIPHY_CMN_DSI_COMMON_REG_BASE      + 0x0000017c)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DEBUG_BUS3_RMSK                                                       0xff
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DEBUG_BUS3_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DEBUG_BUS3_ADDR, HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DEBUG_BUS3_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DEBUG_BUS3_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DEBUG_BUS3_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DEBUG_BUS3_DEBUG_BUS_31_24_BMSK                                       0xff
#define HWIO_DSI_1_PHY_DSIPHY_CMN_GLBL_DEBUG_BUS3_DEBUG_BUS_31_24_SHFT                                        0x0

/*----------------------------------------------------------------------------
 * MODULE: DSI_1_PHY_DSIPHY_DLN0_DSIPHY_DLN0_DSI_DATALN
 *--------------------------------------------------------------------------*/

#define DSI_1_PHY_DSIPHY_DLN0_DSIPHY_DLN0_DSI_DATALN_REG_BASE                          (MDSS_BASE      + 0x00096600)

#define HWIO_DSI_1_PHY_DSIPHY_DLN0_CFG0_ADDR                                           (DSI_1_PHY_DSIPHY_DLN0_DSIPHY_DLN0_DSI_DATALN_REG_BASE      + 0x00000000)
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_CFG0_RMSK                                                 0x3f
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_CFG0_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN0_CFG0_ADDR, HWIO_DSI_1_PHY_DSIPHY_DLN0_CFG0_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_CFG0_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN0_CFG0_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_CFG0_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_DLN0_CFG0_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_CFG0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_DLN0_CFG0_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_DLN0_CFG0_IN)
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_CFG0_DSIPHY_HSTX_PGM_DLY_5_4_BMSK                         0x30
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_CFG0_DSIPHY_HSTX_PGM_DLY_5_4_SHFT                          0x4
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_CFG0_DSIPHY_HSTX_PGM_DLY_3_0_BMSK                          0xf
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_CFG0_DSIPHY_HSTX_PGM_DLY_3_0_SHFT                          0x0

#define HWIO_DSI_1_PHY_DSIPHY_DLN0_CFG1_ADDR                                           (DSI_1_PHY_DSIPHY_DLN0_DSIPHY_DLN0_DSI_DATALN_REG_BASE      + 0x00000004)
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_CFG1_RMSK                                                 0x1f
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_CFG1_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN0_CFG1_ADDR, HWIO_DSI_1_PHY_DSIPHY_DLN0_CFG1_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_CFG1_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN0_CFG1_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_CFG1_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_DLN0_CFG1_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_CFG1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_DLN0_CFG1_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_DLN0_CFG1_IN)
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_CFG1_PROGSEQ_EN_BMSK                                      0x10
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_CFG1_PROGSEQ_EN_SHFT                                       0x4
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_CFG1_DSIPHY_HSTX_SLEW_RATE_BMSK                            0xf
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_CFG1_DSIPHY_HSTX_SLEW_RATE_SHFT                            0x0

#define HWIO_DSI_1_PHY_DSIPHY_DLN0_CFG2_ADDR                                           (DSI_1_PHY_DSIPHY_DLN0_DSIPHY_DLN0_DSI_DATALN_REG_BASE      + 0x00000008)
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_CFG2_RMSK                                                 0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_CFG2_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN0_CFG2_ADDR, HWIO_DSI_1_PHY_DSIPHY_DLN0_CFG2_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_CFG2_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN0_CFG2_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_CFG2_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_DLN0_CFG2_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_CFG2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_DLN0_CFG2_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_DLN0_CFG2_IN)
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_CFG2_IS_CKLANE_BMSK                                       0x80
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_CFG2_IS_CKLANE_SHFT                                        0x7
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_CFG2_CLK_PATTERN_HSTX_INV_BMSK                            0x40
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_CFG2_CLK_PATTERN_HSTX_INV_SHFT                             0x6
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_CFG2_TEST_MODE_LPTX_DATAN_BMSK                            0x20
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_CFG2_TEST_MODE_LPTX_DATAN_SHFT                             0x5
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_CFG2_TEST_MODE_LPTX_DATAP_BMSK                            0x10
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_CFG2_TEST_MODE_LPTX_DATAP_SHFT                             0x4
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_CFG2_DSIPHY_LPRX_DLY_BMSK                                  0xf
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_CFG2_DSIPHY_LPRX_DLY_SHFT                                  0x0

#define HWIO_DSI_1_PHY_DSIPHY_DLN0_TEST_DATAPATH_ADDR                                  (DSI_1_PHY_DSIPHY_DLN0_DSIPHY_DLN0_DSI_DATALN_REG_BASE      + 0x0000000c)
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_TEST_DATAPATH_RMSK                                        0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_TEST_DATAPATH_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN0_TEST_DATAPATH_ADDR, HWIO_DSI_1_PHY_DSIPHY_DLN0_TEST_DATAPATH_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_TEST_DATAPATH_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN0_TEST_DATAPATH_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_TEST_DATAPATH_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_DLN0_TEST_DATAPATH_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_TEST_DATAPATH_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_DLN0_TEST_DATAPATH_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_DLN0_TEST_DATAPATH_IN)
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_TEST_DATAPATH_TEST_MODE_LPTX_EN_BMSK                      0x80
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_TEST_DATAPATH_TEST_MODE_LPTX_EN_SHFT                       0x7
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_TEST_DATAPATH_ANE_BYPASS_EN_BMSK                          0x40
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_TEST_DATAPATH_ANE_BYPASS_EN_SHFT                           0x6
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_TEST_DATAPATH_TEST_MODE_HSTX_DATA_BMSK                    0x20
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_TEST_DATAPATH_TEST_MODE_HSTX_DATA_SHFT                     0x5
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_TEST_DATAPATH_DMUX_LP_SEL_BMSK                            0x18
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_TEST_DATAPATH_DMUX_LP_SEL_SHFT                             0x3
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_TEST_DATAPATH_DMUX_HS_SEL_BMSK                             0x4
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_TEST_DATAPATH_DMUX_HS_SEL_SHFT                             0x2
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_TEST_DATAPATH_TEST_MODE_HSTX_EN_BMSK                       0x2
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_TEST_DATAPATH_TEST_MODE_HSTX_EN_SHFT                       0x1
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_TEST_DATAPATH_FORCE_TEST_MODE_BMSK                         0x1
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_TEST_DATAPATH_FORCE_TEST_MODE_SHFT                         0x0

#define HWIO_DSI_1_PHY_DSIPHY_DLN0_PIN_SWAP_ADDR                                       (DSI_1_PHY_DSIPHY_DLN0_DSIPHY_DLN0_DSI_DATALN_REG_BASE      + 0x00000010)
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_PIN_SWAP_RMSK                                              0xf
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_PIN_SWAP_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN0_PIN_SWAP_ADDR, HWIO_DSI_1_PHY_DSIPHY_DLN0_PIN_SWAP_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_PIN_SWAP_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN0_PIN_SWAP_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_PIN_SWAP_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_DLN0_PIN_SWAP_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_PIN_SWAP_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_DLN0_PIN_SWAP_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_DLN0_PIN_SWAP_IN)
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_PIN_SWAP_ABC_SWAP_BMSK                                     0xe
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_PIN_SWAP_ABC_SWAP_SHFT                                     0x1
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_PIN_SWAP_PN_SWAP_BMSK                                      0x1
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_PIN_SWAP_PN_SWAP_SHFT                                      0x0

#define HWIO_DSI_1_PHY_DSIPHY_DLN0_LPRX_CTRL_ADDR                                      (DSI_1_PHY_DSIPHY_DLN0_DSIPHY_DLN0_DSI_DATALN_REG_BASE      + 0x00000014)
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_LPRX_CTRL_RMSK                                             0x7
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_LPRX_CTRL_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN0_LPRX_CTRL_ADDR, HWIO_DSI_1_PHY_DSIPHY_DLN0_LPRX_CTRL_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_LPRX_CTRL_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN0_LPRX_CTRL_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_LPRX_CTRL_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_DLN0_LPRX_CTRL_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_LPRX_CTRL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_DLN0_LPRX_CTRL_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_DLN0_LPRX_CTRL_IN)
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_LPRX_CTRL_HS_CONTENTION_DIS_BMSK                           0x4
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_LPRX_CTRL_HS_CONTENTION_DIS_SHFT                           0x2
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_LPRX_CTRL_DSIPHY_LPRX_EN_BMSK                              0x2
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_LPRX_CTRL_DSIPHY_LPRX_EN_SHFT                              0x1
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_LPRX_CTRL_DSIPHY_CDRX_EN_BMSK                              0x1
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_LPRX_CTRL_DSIPHY_CDRX_EN_SHFT                              0x0

#define HWIO_DSI_1_PHY_DSIPHY_DLN0_TX_DCTRL_ADDR                                       (DSI_1_PHY_DSIPHY_DLN0_DSIPHY_DLN0_DSI_DATALN_REG_BASE      + 0x00000018)
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_TX_DCTRL_RMSK                                             0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_TX_DCTRL_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN0_TX_DCTRL_ADDR, HWIO_DSI_1_PHY_DSIPHY_DLN0_TX_DCTRL_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_TX_DCTRL_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN0_TX_DCTRL_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_TX_DCTRL_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_DLN0_TX_DCTRL_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_TX_DCTRL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_DLN0_TX_DCTRL_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_DLN0_TX_DCTRL_IN)
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_TX_DCTRL_DCTRL_BMSK                                       0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_TX_DCTRL_DCTRL_SHFT                                        0x0

#define HWIO_DSI_1_PHY_DSIPHY_DLN0_BIST_CTRL0_ADDR                                     (DSI_1_PHY_DSIPHY_DLN0_DSIPHY_DLN0_DSI_DATALN_REG_BASE      + 0x0000001c)
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_BIST_CTRL0_RMSK                                           0x3f
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_BIST_CTRL0_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN0_BIST_CTRL0_ADDR, HWIO_DSI_1_PHY_DSIPHY_DLN0_BIST_CTRL0_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_BIST_CTRL0_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN0_BIST_CTRL0_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_BIST_CTRL0_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_DLN0_BIST_CTRL0_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_BIST_CTRL0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_DLN0_BIST_CTRL0_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_DLN0_BIST_CTRL0_IN)
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_BIST_CTRL0_LP_BISTCHK_ENABLE_BMSK                         0x20
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_BIST_CTRL0_LP_BISTCHK_ENABLE_SHFT                          0x5
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_BIST_CTRL0_BISTCHK_CAPTURE_BMSK                           0x10
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_BIST_CTRL0_BISTCHK_CAPTURE_SHFT                            0x4
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_BIST_CTRL0_BISTCHK_CLR_ERROR_CNT_BMSK                      0x8
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_BIST_CTRL0_BISTCHK_CLR_ERROR_CNT_SHFT                      0x3
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_BIST_CTRL0_BISTCHK_MODE_BMSK                               0x4
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_BIST_CTRL0_BISTCHK_MODE_SHFT                               0x2
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_BIST_CTRL0_BISTCHK_CROSS_BMSK                              0x2
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_BIST_CTRL0_BISTCHK_CROSS_SHFT                              0x1
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_BIST_CTRL0_BISTCHK_ENABLE_BMSK                             0x1
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_BIST_CTRL0_BISTCHK_ENABLE_SHFT                             0x0

#define HWIO_DSI_1_PHY_DSIPHY_DLN0_BIST_CTRL1_ADDR                                     (DSI_1_PHY_DSIPHY_DLN0_DSIPHY_DLN0_DSI_DATALN_REG_BASE      + 0x00000020)
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_BIST_CTRL1_RMSK                                           0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_BIST_CTRL1_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN0_BIST_CTRL1_ADDR, HWIO_DSI_1_PHY_DSIPHY_DLN0_BIST_CTRL1_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_BIST_CTRL1_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN0_BIST_CTRL1_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_BIST_CTRL1_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_DLN0_BIST_CTRL1_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_BIST_CTRL1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_DLN0_BIST_CTRL1_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_DLN0_BIST_CTRL1_IN)
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_BIST_CTRL1_BIST_WORD_CNT_7_0_BMSK                         0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_BIST_CTRL1_BIST_WORD_CNT_7_0_SHFT                          0x0

#define HWIO_DSI_1_PHY_DSIPHY_DLN0_BIST_CTRL2_ADDR                                     (DSI_1_PHY_DSIPHY_DLN0_DSIPHY_DLN0_DSI_DATALN_REG_BASE      + 0x00000024)
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_BIST_CTRL2_RMSK                                           0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_BIST_CTRL2_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN0_BIST_CTRL2_ADDR, HWIO_DSI_1_PHY_DSIPHY_DLN0_BIST_CTRL2_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_BIST_CTRL2_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN0_BIST_CTRL2_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_BIST_CTRL2_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_DLN0_BIST_CTRL2_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_BIST_CTRL2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_DLN0_BIST_CTRL2_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_DLN0_BIST_CTRL2_IN)
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_BIST_CTRL2_BIST_WORD_CNT_15_8_BMSK                        0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_BIST_CTRL2_BIST_WORD_CNT_15_8_SHFT                         0x0

#define HWIO_DSI_1_PHY_DSIPHY_DLN0_MISR_CTRL_ADDR                                      (DSI_1_PHY_DSIPHY_DLN0_DSIPHY_DLN0_DSI_DATALN_REG_BASE      + 0x00000028)
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_MISR_CTRL_RMSK                                            0x1f
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_MISR_CTRL_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN0_MISR_CTRL_ADDR, HWIO_DSI_1_PHY_DSIPHY_DLN0_MISR_CTRL_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_MISR_CTRL_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN0_MISR_CTRL_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_MISR_CTRL_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_DLN0_MISR_CTRL_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_MISR_CTRL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_DLN0_MISR_CTRL_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_DLN0_MISR_CTRL_IN)
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_MISR_CTRL_DEBUG_CLK_DEBUG_SEL_BMSK                        0x1c
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_MISR_CTRL_DEBUG_CLK_DEBUG_SEL_SHFT                         0x2
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_MISR_CTRL_MISR_SIG_CLEAR_BMSK                              0x2
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_MISR_CTRL_MISR_SIG_CLEAR_SHFT                              0x1
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_MISR_CTRL_MISR_ENABLE_BMSK                                 0x1
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_MISR_CTRL_MISR_ENABLE_SHFT                                 0x0

#define HWIO_DSI_1_PHY_DSIPHY_DLN0_DBG_SEL_ADDR                                        (DSI_1_PHY_DSIPHY_DLN0_DSIPHY_DLN0_DSI_DATALN_REG_BASE      + 0x0000002c)
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_DBG_SEL_RMSK                                              0x7f
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_DBG_SEL_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN0_DBG_SEL_ADDR, HWIO_DSI_1_PHY_DSIPHY_DLN0_DBG_SEL_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_DBG_SEL_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN0_DBG_SEL_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_DBG_SEL_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_DLN0_DBG_SEL_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_DBG_SEL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_DLN0_DBG_SEL_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_DLN0_DBG_SEL_IN)
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_DBG_SEL_TEST_CLK_DEBUG_SEL_BMSK                           0x70
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_DBG_SEL_TEST_CLK_DEBUG_SEL_SHFT                            0x4
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_DBG_SEL_DEBUG_SEL_BMSK                                     0xf
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_DBG_SEL_DEBUG_SEL_SHFT                                     0x0

#define HWIO_DSI_1_PHY_DSIPHY_DLN0_SWI_DBG_BUS_CHECK_ADDR                              (DSI_1_PHY_DSIPHY_DLN0_DSIPHY_DLN0_DSI_DATALN_REG_BASE      + 0x00000030)
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_SWI_DBG_BUS_CHECK_RMSK                                    0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_SWI_DBG_BUS_CHECK_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN0_SWI_DBG_BUS_CHECK_ADDR, HWIO_DSI_1_PHY_DSIPHY_DLN0_SWI_DBG_BUS_CHECK_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_SWI_DBG_BUS_CHECK_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN0_SWI_DBG_BUS_CHECK_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_SWI_DBG_BUS_CHECK_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_DLN0_SWI_DBG_BUS_CHECK_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_SWI_DBG_BUS_CHECK_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_DLN0_SWI_DBG_BUS_CHECK_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_DLN0_SWI_DBG_BUS_CHECK_IN)
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_SWI_DBG_BUS_CHECK_LANE_DEBUG_BUS_BMSK                     0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_SWI_DBG_BUS_CHECK_LANE_DEBUG_BUS_SHFT                      0x0

#define HWIO_DSI_1_PHY_DSIPHY_DLN0_BIST_STATUS0_ADDR                                   (DSI_1_PHY_DSIPHY_DLN0_DSIPHY_DLN0_DSI_DATALN_REG_BASE      + 0x00000034)
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_BIST_STATUS0_RMSK                                         0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_BIST_STATUS0_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN0_BIST_STATUS0_ADDR, HWIO_DSI_1_PHY_DSIPHY_DLN0_BIST_STATUS0_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_BIST_STATUS0_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN0_BIST_STATUS0_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_BIST_STATUS0_BISTCHK_ONE_BIT_ERROR_BMSK                   0x80
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_BIST_STATUS0_BISTCHK_ONE_BIT_ERROR_SHFT                    0x7
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_BIST_STATUS0_BISTCHK_SYNC_ERROR_BMSK                      0x40
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_BIST_STATUS0_BISTCHK_SYNC_ERROR_SHFT                       0x6
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_BIST_STATUS0_BISTCHK_SEED_ERROR_BMSK                      0x20
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_BIST_STATUS0_BISTCHK_SEED_ERROR_SHFT                       0x5
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_BIST_STATUS0_BISTCHK_PRBS_ERROR_BMSK                      0x10
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_BIST_STATUS0_BISTCHK_PRBS_ERROR_SHFT                       0x4
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_BIST_STATUS0_BISTCHK_STATUS_BMSK                           0xe
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_BIST_STATUS0_BISTCHK_STATUS_SHFT                           0x1
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_BIST_STATUS0_BISTCHK_DONE_BMSK                             0x1
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_BIST_STATUS0_BISTCHK_DONE_SHFT                             0x0

#define HWIO_DSI_1_PHY_DSIPHY_DLN0_BIST_STATUS1_ADDR                                   (DSI_1_PHY_DSIPHY_DLN0_DSIPHY_DLN0_DSI_DATALN_REG_BASE      + 0x00000038)
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_BIST_STATUS1_RMSK                                         0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_BIST_STATUS1_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN0_BIST_STATUS1_ADDR, HWIO_DSI_1_PHY_DSIPHY_DLN0_BIST_STATUS1_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_BIST_STATUS1_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN0_BIST_STATUS1_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_BIST_STATUS1_BISTCHK_ERROR_CNT_7_0_BMSK                   0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_BIST_STATUS1_BISTCHK_ERROR_CNT_7_0_SHFT                    0x0

#define HWIO_DSI_1_PHY_DSIPHY_DLN0_BIST_STATUS2_ADDR                                   (DSI_1_PHY_DSIPHY_DLN0_DSIPHY_DLN0_DSI_DATALN_REG_BASE      + 0x0000003c)
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_BIST_STATUS2_RMSK                                         0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_BIST_STATUS2_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN0_BIST_STATUS2_ADDR, HWIO_DSI_1_PHY_DSIPHY_DLN0_BIST_STATUS2_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_BIST_STATUS2_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN0_BIST_STATUS2_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_BIST_STATUS2_BISTCHK_ERROR_CNT_15_8_BMSK                  0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_BIST_STATUS2_BISTCHK_ERROR_CNT_15_8_SHFT                   0x0

#define HWIO_DSI_1_PHY_DSIPHY_DLN0_BIST_STATUS3_ADDR                                   (DSI_1_PHY_DSIPHY_DLN0_DSIPHY_DLN0_DSI_DATALN_REG_BASE      + 0x00000040)
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_BIST_STATUS3_RMSK                                         0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_BIST_STATUS3_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN0_BIST_STATUS3_ADDR, HWIO_DSI_1_PHY_DSIPHY_DLN0_BIST_STATUS3_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_BIST_STATUS3_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN0_BIST_STATUS3_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_BIST_STATUS3_BISTCHK_WORD_CNT_7_0_BMSK                    0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_BIST_STATUS3_BISTCHK_WORD_CNT_7_0_SHFT                     0x0

#define HWIO_DSI_1_PHY_DSIPHY_DLN0_BIST_STATUS4_ADDR                                   (DSI_1_PHY_DSIPHY_DLN0_DSIPHY_DLN0_DSI_DATALN_REG_BASE      + 0x00000044)
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_BIST_STATUS4_RMSK                                         0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_BIST_STATUS4_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN0_BIST_STATUS4_ADDR, HWIO_DSI_1_PHY_DSIPHY_DLN0_BIST_STATUS4_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_BIST_STATUS4_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN0_BIST_STATUS4_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_BIST_STATUS4_BISTCHK_WORD_CNT_15_8_BMSK                   0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_BIST_STATUS4_BISTCHK_WORD_CNT_15_8_SHFT                    0x0

#define HWIO_DSI_1_PHY_DSIPHY_DLN0_LP_BIST_STATUS0_ADDR                                (DSI_1_PHY_DSIPHY_DLN0_DSIPHY_DLN0_DSI_DATALN_REG_BASE      + 0x00000048)
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_LP_BIST_STATUS0_RMSK                                       0x3
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_LP_BIST_STATUS0_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN0_LP_BIST_STATUS0_ADDR, HWIO_DSI_1_PHY_DSIPHY_DLN0_LP_BIST_STATUS0_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_LP_BIST_STATUS0_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN0_LP_BIST_STATUS0_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_LP_BIST_STATUS0_LP_BISTCHK_PRBS_ERROR_BMSK                 0x2
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_LP_BIST_STATUS0_LP_BISTCHK_PRBS_ERROR_SHFT                 0x1
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_LP_BIST_STATUS0_LP_BISTCHK_DONE_BMSK                       0x1
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_LP_BIST_STATUS0_LP_BISTCHK_DONE_SHFT                       0x0

#define HWIO_DSI_1_PHY_DSIPHY_DLN0_LP_BIST_STATUS1_ADDR                                (DSI_1_PHY_DSIPHY_DLN0_DSIPHY_DLN0_DSI_DATALN_REG_BASE      + 0x0000004c)
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_LP_BIST_STATUS1_RMSK                                      0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_LP_BIST_STATUS1_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN0_LP_BIST_STATUS1_ADDR, HWIO_DSI_1_PHY_DSIPHY_DLN0_LP_BIST_STATUS1_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_LP_BIST_STATUS1_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN0_LP_BIST_STATUS1_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_LP_BIST_STATUS1_LP_BISTCHK_ERROR_CNT_BMSK                 0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_LP_BIST_STATUS1_LP_BISTCHK_ERROR_CNT_SHFT                  0x0

#define HWIO_DSI_1_PHY_DSIPHY_DLN0_LP_BIST_STATUS2_ADDR                                (DSI_1_PHY_DSIPHY_DLN0_DSIPHY_DLN0_DSI_DATALN_REG_BASE      + 0x00000050)
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_LP_BIST_STATUS2_RMSK                                      0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_LP_BIST_STATUS2_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN0_LP_BIST_STATUS2_ADDR, HWIO_DSI_1_PHY_DSIPHY_DLN0_LP_BIST_STATUS2_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_LP_BIST_STATUS2_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN0_LP_BIST_STATUS2_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_LP_BIST_STATUS2_LP_BISTCHK_WORD_CNT_BMSK                  0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_LP_BIST_STATUS2_LP_BISTCHK_WORD_CNT_SHFT                   0x0

#define HWIO_DSI_1_PHY_DSIPHY_DLN0_MISR_STATUS0_ADDR                                   (DSI_1_PHY_DSIPHY_DLN0_DSIPHY_DLN0_DSI_DATALN_REG_BASE      + 0x00000054)
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_MISR_STATUS0_RMSK                                         0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_MISR_STATUS0_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN0_MISR_STATUS0_ADDR, HWIO_DSI_1_PHY_DSIPHY_DLN0_MISR_STATUS0_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_MISR_STATUS0_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN0_MISR_STATUS0_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_MISR_STATUS0_BISTCHK_MISR_SIG_7_0_BMSK                    0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_MISR_STATUS0_BISTCHK_MISR_SIG_7_0_SHFT                     0x0

#define HWIO_DSI_1_PHY_DSIPHY_DLN0_MISR_STATUS1_ADDR                                   (DSI_1_PHY_DSIPHY_DLN0_DSIPHY_DLN0_DSI_DATALN_REG_BASE      + 0x00000058)
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_MISR_STATUS1_RMSK                                         0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_MISR_STATUS1_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN0_MISR_STATUS1_ADDR, HWIO_DSI_1_PHY_DSIPHY_DLN0_MISR_STATUS1_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_MISR_STATUS1_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN0_MISR_STATUS1_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_MISR_STATUS1_BISTCHK_MISR_SIG_15_8_BMSK                   0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_MISR_STATUS1_BISTCHK_MISR_SIG_15_8_SHFT                    0x0

#define HWIO_DSI_1_PHY_DSIPHY_DLN0_DEBUG_BUS0_ADDR                                     (DSI_1_PHY_DSIPHY_DLN0_DSIPHY_DLN0_DSI_DATALN_REG_BASE      + 0x0000005c)
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_DEBUG_BUS0_RMSK                                           0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_DEBUG_BUS0_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN0_DEBUG_BUS0_ADDR, HWIO_DSI_1_PHY_DSIPHY_DLN0_DEBUG_BUS0_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_DEBUG_BUS0_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN0_DEBUG_BUS0_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_DEBUG_BUS0_DEBUG_BUS_7_0_BMSK                             0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_DEBUG_BUS0_DEBUG_BUS_7_0_SHFT                              0x0

#define HWIO_DSI_1_PHY_DSIPHY_DLN0_DEBUG_BUS1_ADDR                                     (DSI_1_PHY_DSIPHY_DLN0_DSIPHY_DLN0_DSI_DATALN_REG_BASE      + 0x00000060)
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_DEBUG_BUS1_RMSK                                           0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_DEBUG_BUS1_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN0_DEBUG_BUS1_ADDR, HWIO_DSI_1_PHY_DSIPHY_DLN0_DEBUG_BUS1_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_DEBUG_BUS1_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN0_DEBUG_BUS1_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_DEBUG_BUS1_DEBUG_BUS_15_8_BMSK                            0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_DEBUG_BUS1_DEBUG_BUS_15_8_SHFT                             0x0

#define HWIO_DSI_1_PHY_DSIPHY_DLN0_DEBUG_BUS2_ADDR                                     (DSI_1_PHY_DSIPHY_DLN0_DSIPHY_DLN0_DSI_DATALN_REG_BASE      + 0x00000064)
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_DEBUG_BUS2_RMSK                                           0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_DEBUG_BUS2_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN0_DEBUG_BUS2_ADDR, HWIO_DSI_1_PHY_DSIPHY_DLN0_DEBUG_BUS2_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_DEBUG_BUS2_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN0_DEBUG_BUS2_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_DEBUG_BUS2_DEBUG_BUS_23_16_BMSK                           0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_DEBUG_BUS2_DEBUG_BUS_23_16_SHFT                            0x0

#define HWIO_DSI_1_PHY_DSIPHY_DLN0_DEBUG_BUS3_ADDR                                     (DSI_1_PHY_DSIPHY_DLN0_DSIPHY_DLN0_DSI_DATALN_REG_BASE      + 0x00000068)
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_DEBUG_BUS3_RMSK                                           0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_DEBUG_BUS3_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN0_DEBUG_BUS3_ADDR, HWIO_DSI_1_PHY_DSIPHY_DLN0_DEBUG_BUS3_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_DEBUG_BUS3_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN0_DEBUG_BUS3_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_DEBUG_BUS3_DEBUG_BUS_31_24_BMSK                           0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN0_DEBUG_BUS3_DEBUG_BUS_31_24_SHFT                            0x0

/*----------------------------------------------------------------------------
 * MODULE: DSI_1_PHY_DSIPHY_DLN1_DSIPHY_DLN1_DSI_DATALN
 *--------------------------------------------------------------------------*/

#define DSI_1_PHY_DSIPHY_DLN1_DSIPHY_DLN1_DSI_DATALN_REG_BASE                          (MDSS_BASE      + 0x00096680)

#define HWIO_DSI_1_PHY_DSIPHY_DLN1_CFG0_ADDR                                           (DSI_1_PHY_DSIPHY_DLN1_DSIPHY_DLN1_DSI_DATALN_REG_BASE      + 0x00000000)
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_CFG0_RMSK                                                 0x3f
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_CFG0_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN1_CFG0_ADDR, HWIO_DSI_1_PHY_DSIPHY_DLN1_CFG0_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_CFG0_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN1_CFG0_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_CFG0_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_DLN1_CFG0_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_CFG0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_DLN1_CFG0_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_DLN1_CFG0_IN)
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_CFG0_DSIPHY_HSTX_PGM_DLY_5_4_BMSK                         0x30
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_CFG0_DSIPHY_HSTX_PGM_DLY_5_4_SHFT                          0x4
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_CFG0_DSIPHY_HSTX_PGM_DLY_3_0_BMSK                          0xf
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_CFG0_DSIPHY_HSTX_PGM_DLY_3_0_SHFT                          0x0

#define HWIO_DSI_1_PHY_DSIPHY_DLN1_CFG1_ADDR                                           (DSI_1_PHY_DSIPHY_DLN1_DSIPHY_DLN1_DSI_DATALN_REG_BASE      + 0x00000004)
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_CFG1_RMSK                                                 0x1f
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_CFG1_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN1_CFG1_ADDR, HWIO_DSI_1_PHY_DSIPHY_DLN1_CFG1_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_CFG1_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN1_CFG1_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_CFG1_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_DLN1_CFG1_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_CFG1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_DLN1_CFG1_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_DLN1_CFG1_IN)
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_CFG1_PROGSEQ_EN_BMSK                                      0x10
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_CFG1_PROGSEQ_EN_SHFT                                       0x4
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_CFG1_DSIPHY_HSTX_SLEW_RATE_BMSK                            0xf
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_CFG1_DSIPHY_HSTX_SLEW_RATE_SHFT                            0x0

#define HWIO_DSI_1_PHY_DSIPHY_DLN1_CFG2_ADDR                                           (DSI_1_PHY_DSIPHY_DLN1_DSIPHY_DLN1_DSI_DATALN_REG_BASE      + 0x00000008)
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_CFG2_RMSK                                                 0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_CFG2_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN1_CFG2_ADDR, HWIO_DSI_1_PHY_DSIPHY_DLN1_CFG2_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_CFG2_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN1_CFG2_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_CFG2_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_DLN1_CFG2_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_CFG2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_DLN1_CFG2_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_DLN1_CFG2_IN)
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_CFG2_IS_CKLANE_BMSK                                       0x80
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_CFG2_IS_CKLANE_SHFT                                        0x7
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_CFG2_CLK_PATTERN_HSTX_INV_BMSK                            0x40
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_CFG2_CLK_PATTERN_HSTX_INV_SHFT                             0x6
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_CFG2_TEST_MODE_LPTX_DATAN_BMSK                            0x20
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_CFG2_TEST_MODE_LPTX_DATAN_SHFT                             0x5
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_CFG2_TEST_MODE_LPTX_DATAP_BMSK                            0x10
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_CFG2_TEST_MODE_LPTX_DATAP_SHFT                             0x4
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_CFG2_DSIPHY_LPRX_DLY_BMSK                                  0xf
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_CFG2_DSIPHY_LPRX_DLY_SHFT                                  0x0

#define HWIO_DSI_1_PHY_DSIPHY_DLN1_TEST_DATAPATH_ADDR                                  (DSI_1_PHY_DSIPHY_DLN1_DSIPHY_DLN1_DSI_DATALN_REG_BASE      + 0x0000000c)
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_TEST_DATAPATH_RMSK                                        0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_TEST_DATAPATH_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN1_TEST_DATAPATH_ADDR, HWIO_DSI_1_PHY_DSIPHY_DLN1_TEST_DATAPATH_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_TEST_DATAPATH_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN1_TEST_DATAPATH_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_TEST_DATAPATH_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_DLN1_TEST_DATAPATH_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_TEST_DATAPATH_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_DLN1_TEST_DATAPATH_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_DLN1_TEST_DATAPATH_IN)
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_TEST_DATAPATH_TEST_MODE_LPTX_EN_BMSK                      0x80
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_TEST_DATAPATH_TEST_MODE_LPTX_EN_SHFT                       0x7
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_TEST_DATAPATH_ANE_BYPASS_EN_BMSK                          0x40
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_TEST_DATAPATH_ANE_BYPASS_EN_SHFT                           0x6
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_TEST_DATAPATH_TEST_MODE_HSTX_DATA_BMSK                    0x20
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_TEST_DATAPATH_TEST_MODE_HSTX_DATA_SHFT                     0x5
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_TEST_DATAPATH_DMUX_LP_SEL_BMSK                            0x18
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_TEST_DATAPATH_DMUX_LP_SEL_SHFT                             0x3
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_TEST_DATAPATH_DMUX_HS_SEL_BMSK                             0x4
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_TEST_DATAPATH_DMUX_HS_SEL_SHFT                             0x2
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_TEST_DATAPATH_TEST_MODE_HSTX_EN_BMSK                       0x2
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_TEST_DATAPATH_TEST_MODE_HSTX_EN_SHFT                       0x1
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_TEST_DATAPATH_FORCE_TEST_MODE_BMSK                         0x1
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_TEST_DATAPATH_FORCE_TEST_MODE_SHFT                         0x0

#define HWIO_DSI_1_PHY_DSIPHY_DLN1_PIN_SWAP_ADDR                                       (DSI_1_PHY_DSIPHY_DLN1_DSIPHY_DLN1_DSI_DATALN_REG_BASE      + 0x00000010)
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_PIN_SWAP_RMSK                                              0xf
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_PIN_SWAP_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN1_PIN_SWAP_ADDR, HWIO_DSI_1_PHY_DSIPHY_DLN1_PIN_SWAP_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_PIN_SWAP_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN1_PIN_SWAP_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_PIN_SWAP_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_DLN1_PIN_SWAP_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_PIN_SWAP_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_DLN1_PIN_SWAP_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_DLN1_PIN_SWAP_IN)
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_PIN_SWAP_ABC_SWAP_BMSK                                     0xe
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_PIN_SWAP_ABC_SWAP_SHFT                                     0x1
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_PIN_SWAP_PN_SWAP_BMSK                                      0x1
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_PIN_SWAP_PN_SWAP_SHFT                                      0x0

#define HWIO_DSI_1_PHY_DSIPHY_DLN1_LPRX_CTRL_ADDR                                      (DSI_1_PHY_DSIPHY_DLN1_DSIPHY_DLN1_DSI_DATALN_REG_BASE      + 0x00000014)
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_LPRX_CTRL_RMSK                                             0x7
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_LPRX_CTRL_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN1_LPRX_CTRL_ADDR, HWIO_DSI_1_PHY_DSIPHY_DLN1_LPRX_CTRL_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_LPRX_CTRL_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN1_LPRX_CTRL_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_LPRX_CTRL_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_DLN1_LPRX_CTRL_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_LPRX_CTRL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_DLN1_LPRX_CTRL_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_DLN1_LPRX_CTRL_IN)
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_LPRX_CTRL_HS_CONTENTION_DIS_BMSK                           0x4
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_LPRX_CTRL_HS_CONTENTION_DIS_SHFT                           0x2
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_LPRX_CTRL_DSIPHY_LPRX_EN_BMSK                              0x2
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_LPRX_CTRL_DSIPHY_LPRX_EN_SHFT                              0x1
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_LPRX_CTRL_DSIPHY_CDRX_EN_BMSK                              0x1
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_LPRX_CTRL_DSIPHY_CDRX_EN_SHFT                              0x0

#define HWIO_DSI_1_PHY_DSIPHY_DLN1_TX_DCTRL_ADDR                                       (DSI_1_PHY_DSIPHY_DLN1_DSIPHY_DLN1_DSI_DATALN_REG_BASE      + 0x00000018)
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_TX_DCTRL_RMSK                                             0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_TX_DCTRL_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN1_TX_DCTRL_ADDR, HWIO_DSI_1_PHY_DSIPHY_DLN1_TX_DCTRL_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_TX_DCTRL_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN1_TX_DCTRL_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_TX_DCTRL_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_DLN1_TX_DCTRL_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_TX_DCTRL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_DLN1_TX_DCTRL_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_DLN1_TX_DCTRL_IN)
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_TX_DCTRL_DCTRL_BMSK                                       0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_TX_DCTRL_DCTRL_SHFT                                        0x0

#define HWIO_DSI_1_PHY_DSIPHY_DLN1_BIST_CTRL0_ADDR                                     (DSI_1_PHY_DSIPHY_DLN1_DSIPHY_DLN1_DSI_DATALN_REG_BASE      + 0x0000001c)
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_BIST_CTRL0_RMSK                                           0x3f
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_BIST_CTRL0_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN1_BIST_CTRL0_ADDR, HWIO_DSI_1_PHY_DSIPHY_DLN1_BIST_CTRL0_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_BIST_CTRL0_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN1_BIST_CTRL0_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_BIST_CTRL0_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_DLN1_BIST_CTRL0_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_BIST_CTRL0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_DLN1_BIST_CTRL0_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_DLN1_BIST_CTRL0_IN)
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_BIST_CTRL0_LP_BISTCHK_ENABLE_BMSK                         0x20
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_BIST_CTRL0_LP_BISTCHK_ENABLE_SHFT                          0x5
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_BIST_CTRL0_BISTCHK_CAPTURE_BMSK                           0x10
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_BIST_CTRL0_BISTCHK_CAPTURE_SHFT                            0x4
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_BIST_CTRL0_BISTCHK_CLR_ERROR_CNT_BMSK                      0x8
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_BIST_CTRL0_BISTCHK_CLR_ERROR_CNT_SHFT                      0x3
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_BIST_CTRL0_BISTCHK_MODE_BMSK                               0x4
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_BIST_CTRL0_BISTCHK_MODE_SHFT                               0x2
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_BIST_CTRL0_BISTCHK_CROSS_BMSK                              0x2
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_BIST_CTRL0_BISTCHK_CROSS_SHFT                              0x1
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_BIST_CTRL0_BISTCHK_ENABLE_BMSK                             0x1
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_BIST_CTRL0_BISTCHK_ENABLE_SHFT                             0x0

#define HWIO_DSI_1_PHY_DSIPHY_DLN1_BIST_CTRL1_ADDR                                     (DSI_1_PHY_DSIPHY_DLN1_DSIPHY_DLN1_DSI_DATALN_REG_BASE      + 0x00000020)
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_BIST_CTRL1_RMSK                                           0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_BIST_CTRL1_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN1_BIST_CTRL1_ADDR, HWIO_DSI_1_PHY_DSIPHY_DLN1_BIST_CTRL1_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_BIST_CTRL1_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN1_BIST_CTRL1_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_BIST_CTRL1_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_DLN1_BIST_CTRL1_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_BIST_CTRL1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_DLN1_BIST_CTRL1_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_DLN1_BIST_CTRL1_IN)
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_BIST_CTRL1_BIST_WORD_CNT_7_0_BMSK                         0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_BIST_CTRL1_BIST_WORD_CNT_7_0_SHFT                          0x0

#define HWIO_DSI_1_PHY_DSIPHY_DLN1_BIST_CTRL2_ADDR                                     (DSI_1_PHY_DSIPHY_DLN1_DSIPHY_DLN1_DSI_DATALN_REG_BASE      + 0x00000024)
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_BIST_CTRL2_RMSK                                           0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_BIST_CTRL2_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN1_BIST_CTRL2_ADDR, HWIO_DSI_1_PHY_DSIPHY_DLN1_BIST_CTRL2_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_BIST_CTRL2_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN1_BIST_CTRL2_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_BIST_CTRL2_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_DLN1_BIST_CTRL2_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_BIST_CTRL2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_DLN1_BIST_CTRL2_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_DLN1_BIST_CTRL2_IN)
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_BIST_CTRL2_BIST_WORD_CNT_15_8_BMSK                        0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_BIST_CTRL2_BIST_WORD_CNT_15_8_SHFT                         0x0

#define HWIO_DSI_1_PHY_DSIPHY_DLN1_MISR_CTRL_ADDR                                      (DSI_1_PHY_DSIPHY_DLN1_DSIPHY_DLN1_DSI_DATALN_REG_BASE      + 0x00000028)
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_MISR_CTRL_RMSK                                            0x1f
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_MISR_CTRL_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN1_MISR_CTRL_ADDR, HWIO_DSI_1_PHY_DSIPHY_DLN1_MISR_CTRL_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_MISR_CTRL_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN1_MISR_CTRL_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_MISR_CTRL_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_DLN1_MISR_CTRL_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_MISR_CTRL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_DLN1_MISR_CTRL_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_DLN1_MISR_CTRL_IN)
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_MISR_CTRL_DEBUG_CLK_DEBUG_SEL_BMSK                        0x1c
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_MISR_CTRL_DEBUG_CLK_DEBUG_SEL_SHFT                         0x2
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_MISR_CTRL_MISR_SIG_CLEAR_BMSK                              0x2
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_MISR_CTRL_MISR_SIG_CLEAR_SHFT                              0x1
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_MISR_CTRL_MISR_ENABLE_BMSK                                 0x1
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_MISR_CTRL_MISR_ENABLE_SHFT                                 0x0

#define HWIO_DSI_1_PHY_DSIPHY_DLN1_DBG_SEL_ADDR                                        (DSI_1_PHY_DSIPHY_DLN1_DSIPHY_DLN1_DSI_DATALN_REG_BASE      + 0x0000002c)
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_DBG_SEL_RMSK                                              0x7f
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_DBG_SEL_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN1_DBG_SEL_ADDR, HWIO_DSI_1_PHY_DSIPHY_DLN1_DBG_SEL_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_DBG_SEL_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN1_DBG_SEL_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_DBG_SEL_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_DLN1_DBG_SEL_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_DBG_SEL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_DLN1_DBG_SEL_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_DLN1_DBG_SEL_IN)
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_DBG_SEL_TEST_CLK_DEBUG_SEL_BMSK                           0x70
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_DBG_SEL_TEST_CLK_DEBUG_SEL_SHFT                            0x4
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_DBG_SEL_DEBUG_SEL_BMSK                                     0xf
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_DBG_SEL_DEBUG_SEL_SHFT                                     0x0

#define HWIO_DSI_1_PHY_DSIPHY_DLN1_SWI_DBG_BUS_CHECK_ADDR                              (DSI_1_PHY_DSIPHY_DLN1_DSIPHY_DLN1_DSI_DATALN_REG_BASE      + 0x00000030)
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_SWI_DBG_BUS_CHECK_RMSK                                    0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_SWI_DBG_BUS_CHECK_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN1_SWI_DBG_BUS_CHECK_ADDR, HWIO_DSI_1_PHY_DSIPHY_DLN1_SWI_DBG_BUS_CHECK_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_SWI_DBG_BUS_CHECK_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN1_SWI_DBG_BUS_CHECK_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_SWI_DBG_BUS_CHECK_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_DLN1_SWI_DBG_BUS_CHECK_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_SWI_DBG_BUS_CHECK_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_DLN1_SWI_DBG_BUS_CHECK_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_DLN1_SWI_DBG_BUS_CHECK_IN)
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_SWI_DBG_BUS_CHECK_LANE_DEBUG_BUS_BMSK                     0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_SWI_DBG_BUS_CHECK_LANE_DEBUG_BUS_SHFT                      0x0

#define HWIO_DSI_1_PHY_DSIPHY_DLN1_BIST_STATUS0_ADDR                                   (DSI_1_PHY_DSIPHY_DLN1_DSIPHY_DLN1_DSI_DATALN_REG_BASE      + 0x00000034)
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_BIST_STATUS0_RMSK                                         0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_BIST_STATUS0_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN1_BIST_STATUS0_ADDR, HWIO_DSI_1_PHY_DSIPHY_DLN1_BIST_STATUS0_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_BIST_STATUS0_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN1_BIST_STATUS0_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_BIST_STATUS0_BISTCHK_ONE_BIT_ERROR_BMSK                   0x80
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_BIST_STATUS0_BISTCHK_ONE_BIT_ERROR_SHFT                    0x7
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_BIST_STATUS0_BISTCHK_SYNC_ERROR_BMSK                      0x40
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_BIST_STATUS0_BISTCHK_SYNC_ERROR_SHFT                       0x6
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_BIST_STATUS0_BISTCHK_SEED_ERROR_BMSK                      0x20
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_BIST_STATUS0_BISTCHK_SEED_ERROR_SHFT                       0x5
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_BIST_STATUS0_BISTCHK_PRBS_ERROR_BMSK                      0x10
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_BIST_STATUS0_BISTCHK_PRBS_ERROR_SHFT                       0x4
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_BIST_STATUS0_BISTCHK_STATUS_BMSK                           0xe
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_BIST_STATUS0_BISTCHK_STATUS_SHFT                           0x1
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_BIST_STATUS0_BISTCHK_DONE_BMSK                             0x1
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_BIST_STATUS0_BISTCHK_DONE_SHFT                             0x0

#define HWIO_DSI_1_PHY_DSIPHY_DLN1_BIST_STATUS1_ADDR                                   (DSI_1_PHY_DSIPHY_DLN1_DSIPHY_DLN1_DSI_DATALN_REG_BASE      + 0x00000038)
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_BIST_STATUS1_RMSK                                         0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_BIST_STATUS1_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN1_BIST_STATUS1_ADDR, HWIO_DSI_1_PHY_DSIPHY_DLN1_BIST_STATUS1_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_BIST_STATUS1_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN1_BIST_STATUS1_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_BIST_STATUS1_BISTCHK_ERROR_CNT_7_0_BMSK                   0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_BIST_STATUS1_BISTCHK_ERROR_CNT_7_0_SHFT                    0x0

#define HWIO_DSI_1_PHY_DSIPHY_DLN1_BIST_STATUS2_ADDR                                   (DSI_1_PHY_DSIPHY_DLN1_DSIPHY_DLN1_DSI_DATALN_REG_BASE      + 0x0000003c)
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_BIST_STATUS2_RMSK                                         0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_BIST_STATUS2_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN1_BIST_STATUS2_ADDR, HWIO_DSI_1_PHY_DSIPHY_DLN1_BIST_STATUS2_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_BIST_STATUS2_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN1_BIST_STATUS2_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_BIST_STATUS2_BISTCHK_ERROR_CNT_15_8_BMSK                  0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_BIST_STATUS2_BISTCHK_ERROR_CNT_15_8_SHFT                   0x0

#define HWIO_DSI_1_PHY_DSIPHY_DLN1_BIST_STATUS3_ADDR                                   (DSI_1_PHY_DSIPHY_DLN1_DSIPHY_DLN1_DSI_DATALN_REG_BASE      + 0x00000040)
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_BIST_STATUS3_RMSK                                         0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_BIST_STATUS3_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN1_BIST_STATUS3_ADDR, HWIO_DSI_1_PHY_DSIPHY_DLN1_BIST_STATUS3_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_BIST_STATUS3_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN1_BIST_STATUS3_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_BIST_STATUS3_BISTCHK_WORD_CNT_7_0_BMSK                    0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_BIST_STATUS3_BISTCHK_WORD_CNT_7_0_SHFT                     0x0

#define HWIO_DSI_1_PHY_DSIPHY_DLN1_BIST_STATUS4_ADDR                                   (DSI_1_PHY_DSIPHY_DLN1_DSIPHY_DLN1_DSI_DATALN_REG_BASE      + 0x00000044)
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_BIST_STATUS4_RMSK                                         0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_BIST_STATUS4_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN1_BIST_STATUS4_ADDR, HWIO_DSI_1_PHY_DSIPHY_DLN1_BIST_STATUS4_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_BIST_STATUS4_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN1_BIST_STATUS4_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_BIST_STATUS4_BISTCHK_WORD_CNT_15_8_BMSK                   0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_BIST_STATUS4_BISTCHK_WORD_CNT_15_8_SHFT                    0x0

#define HWIO_DSI_1_PHY_DSIPHY_DLN1_LP_BIST_STATUS0_ADDR                                (DSI_1_PHY_DSIPHY_DLN1_DSIPHY_DLN1_DSI_DATALN_REG_BASE      + 0x00000048)
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_LP_BIST_STATUS0_RMSK                                       0x3
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_LP_BIST_STATUS0_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN1_LP_BIST_STATUS0_ADDR, HWIO_DSI_1_PHY_DSIPHY_DLN1_LP_BIST_STATUS0_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_LP_BIST_STATUS0_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN1_LP_BIST_STATUS0_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_LP_BIST_STATUS0_LP_BISTCHK_PRBS_ERROR_BMSK                 0x2
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_LP_BIST_STATUS0_LP_BISTCHK_PRBS_ERROR_SHFT                 0x1
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_LP_BIST_STATUS0_LP_BISTCHK_DONE_BMSK                       0x1
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_LP_BIST_STATUS0_LP_BISTCHK_DONE_SHFT                       0x0

#define HWIO_DSI_1_PHY_DSIPHY_DLN1_LP_BIST_STATUS1_ADDR                                (DSI_1_PHY_DSIPHY_DLN1_DSIPHY_DLN1_DSI_DATALN_REG_BASE      + 0x0000004c)
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_LP_BIST_STATUS1_RMSK                                      0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_LP_BIST_STATUS1_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN1_LP_BIST_STATUS1_ADDR, HWIO_DSI_1_PHY_DSIPHY_DLN1_LP_BIST_STATUS1_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_LP_BIST_STATUS1_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN1_LP_BIST_STATUS1_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_LP_BIST_STATUS1_LP_BISTCHK_ERROR_CNT_BMSK                 0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_LP_BIST_STATUS1_LP_BISTCHK_ERROR_CNT_SHFT                  0x0

#define HWIO_DSI_1_PHY_DSIPHY_DLN1_LP_BIST_STATUS2_ADDR                                (DSI_1_PHY_DSIPHY_DLN1_DSIPHY_DLN1_DSI_DATALN_REG_BASE      + 0x00000050)
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_LP_BIST_STATUS2_RMSK                                      0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_LP_BIST_STATUS2_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN1_LP_BIST_STATUS2_ADDR, HWIO_DSI_1_PHY_DSIPHY_DLN1_LP_BIST_STATUS2_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_LP_BIST_STATUS2_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN1_LP_BIST_STATUS2_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_LP_BIST_STATUS2_LP_BISTCHK_WORD_CNT_BMSK                  0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_LP_BIST_STATUS2_LP_BISTCHK_WORD_CNT_SHFT                   0x0

#define HWIO_DSI_1_PHY_DSIPHY_DLN1_MISR_STATUS0_ADDR                                   (DSI_1_PHY_DSIPHY_DLN1_DSIPHY_DLN1_DSI_DATALN_REG_BASE      + 0x00000054)
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_MISR_STATUS0_RMSK                                         0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_MISR_STATUS0_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN1_MISR_STATUS0_ADDR, HWIO_DSI_1_PHY_DSIPHY_DLN1_MISR_STATUS0_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_MISR_STATUS0_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN1_MISR_STATUS0_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_MISR_STATUS0_BISTCHK_MISR_SIG_7_0_BMSK                    0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_MISR_STATUS0_BISTCHK_MISR_SIG_7_0_SHFT                     0x0

#define HWIO_DSI_1_PHY_DSIPHY_DLN1_MISR_STATUS1_ADDR                                   (DSI_1_PHY_DSIPHY_DLN1_DSIPHY_DLN1_DSI_DATALN_REG_BASE      + 0x00000058)
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_MISR_STATUS1_RMSK                                         0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_MISR_STATUS1_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN1_MISR_STATUS1_ADDR, HWIO_DSI_1_PHY_DSIPHY_DLN1_MISR_STATUS1_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_MISR_STATUS1_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN1_MISR_STATUS1_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_MISR_STATUS1_BISTCHK_MISR_SIG_15_8_BMSK                   0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_MISR_STATUS1_BISTCHK_MISR_SIG_15_8_SHFT                    0x0

#define HWIO_DSI_1_PHY_DSIPHY_DLN1_DEBUG_BUS0_ADDR                                     (DSI_1_PHY_DSIPHY_DLN1_DSIPHY_DLN1_DSI_DATALN_REG_BASE      + 0x0000005c)
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_DEBUG_BUS0_RMSK                                           0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_DEBUG_BUS0_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN1_DEBUG_BUS0_ADDR, HWIO_DSI_1_PHY_DSIPHY_DLN1_DEBUG_BUS0_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_DEBUG_BUS0_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN1_DEBUG_BUS0_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_DEBUG_BUS0_DEBUG_BUS_7_0_BMSK                             0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_DEBUG_BUS0_DEBUG_BUS_7_0_SHFT                              0x0

#define HWIO_DSI_1_PHY_DSIPHY_DLN1_DEBUG_BUS1_ADDR                                     (DSI_1_PHY_DSIPHY_DLN1_DSIPHY_DLN1_DSI_DATALN_REG_BASE      + 0x00000060)
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_DEBUG_BUS1_RMSK                                           0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_DEBUG_BUS1_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN1_DEBUG_BUS1_ADDR, HWIO_DSI_1_PHY_DSIPHY_DLN1_DEBUG_BUS1_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_DEBUG_BUS1_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN1_DEBUG_BUS1_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_DEBUG_BUS1_DEBUG_BUS_15_8_BMSK                            0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_DEBUG_BUS1_DEBUG_BUS_15_8_SHFT                             0x0

#define HWIO_DSI_1_PHY_DSIPHY_DLN1_DEBUG_BUS2_ADDR                                     (DSI_1_PHY_DSIPHY_DLN1_DSIPHY_DLN1_DSI_DATALN_REG_BASE      + 0x00000064)
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_DEBUG_BUS2_RMSK                                           0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_DEBUG_BUS2_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN1_DEBUG_BUS2_ADDR, HWIO_DSI_1_PHY_DSIPHY_DLN1_DEBUG_BUS2_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_DEBUG_BUS2_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN1_DEBUG_BUS2_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_DEBUG_BUS2_DEBUG_BUS_23_16_BMSK                           0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_DEBUG_BUS2_DEBUG_BUS_23_16_SHFT                            0x0

#define HWIO_DSI_1_PHY_DSIPHY_DLN1_DEBUG_BUS3_ADDR                                     (DSI_1_PHY_DSIPHY_DLN1_DSIPHY_DLN1_DSI_DATALN_REG_BASE      + 0x00000068)
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_DEBUG_BUS3_RMSK                                           0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_DEBUG_BUS3_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN1_DEBUG_BUS3_ADDR, HWIO_DSI_1_PHY_DSIPHY_DLN1_DEBUG_BUS3_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_DEBUG_BUS3_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN1_DEBUG_BUS3_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_DEBUG_BUS3_DEBUG_BUS_31_24_BMSK                           0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN1_DEBUG_BUS3_DEBUG_BUS_31_24_SHFT                            0x0

/*----------------------------------------------------------------------------
 * MODULE: DSI_1_PHY_DSIPHY_DLN2_DSIPHY_DLN2_DSI_DATALN
 *--------------------------------------------------------------------------*/

#define DSI_1_PHY_DSIPHY_DLN2_DSIPHY_DLN2_DSI_DATALN_REG_BASE                          (MDSS_BASE      + 0x00096700)

#define HWIO_DSI_1_PHY_DSIPHY_DLN2_CFG0_ADDR                                           (DSI_1_PHY_DSIPHY_DLN2_DSIPHY_DLN2_DSI_DATALN_REG_BASE      + 0x00000000)
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_CFG0_RMSK                                                 0x3f
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_CFG0_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN2_CFG0_ADDR, HWIO_DSI_1_PHY_DSIPHY_DLN2_CFG0_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_CFG0_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN2_CFG0_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_CFG0_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_DLN2_CFG0_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_CFG0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_DLN2_CFG0_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_DLN2_CFG0_IN)
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_CFG0_DSIPHY_HSTX_PGM_DLY_5_4_BMSK                         0x30
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_CFG0_DSIPHY_HSTX_PGM_DLY_5_4_SHFT                          0x4
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_CFG0_DSIPHY_HSTX_PGM_DLY_3_0_BMSK                          0xf
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_CFG0_DSIPHY_HSTX_PGM_DLY_3_0_SHFT                          0x0

#define HWIO_DSI_1_PHY_DSIPHY_DLN2_CFG1_ADDR                                           (DSI_1_PHY_DSIPHY_DLN2_DSIPHY_DLN2_DSI_DATALN_REG_BASE      + 0x00000004)
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_CFG1_RMSK                                                 0x1f
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_CFG1_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN2_CFG1_ADDR, HWIO_DSI_1_PHY_DSIPHY_DLN2_CFG1_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_CFG1_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN2_CFG1_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_CFG1_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_DLN2_CFG1_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_CFG1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_DLN2_CFG1_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_DLN2_CFG1_IN)
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_CFG1_PROGSEQ_EN_BMSK                                      0x10
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_CFG1_PROGSEQ_EN_SHFT                                       0x4
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_CFG1_DSIPHY_HSTX_SLEW_RATE_BMSK                            0xf
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_CFG1_DSIPHY_HSTX_SLEW_RATE_SHFT                            0x0

#define HWIO_DSI_1_PHY_DSIPHY_DLN2_CFG2_ADDR                                           (DSI_1_PHY_DSIPHY_DLN2_DSIPHY_DLN2_DSI_DATALN_REG_BASE      + 0x00000008)
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_CFG2_RMSK                                                 0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_CFG2_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN2_CFG2_ADDR, HWIO_DSI_1_PHY_DSIPHY_DLN2_CFG2_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_CFG2_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN2_CFG2_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_CFG2_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_DLN2_CFG2_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_CFG2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_DLN2_CFG2_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_DLN2_CFG2_IN)
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_CFG2_IS_CKLANE_BMSK                                       0x80
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_CFG2_IS_CKLANE_SHFT                                        0x7
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_CFG2_CLK_PATTERN_HSTX_INV_BMSK                            0x40
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_CFG2_CLK_PATTERN_HSTX_INV_SHFT                             0x6
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_CFG2_TEST_MODE_LPTX_DATAN_BMSK                            0x20
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_CFG2_TEST_MODE_LPTX_DATAN_SHFT                             0x5
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_CFG2_TEST_MODE_LPTX_DATAP_BMSK                            0x10
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_CFG2_TEST_MODE_LPTX_DATAP_SHFT                             0x4
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_CFG2_DSIPHY_LPRX_DLY_BMSK                                  0xf
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_CFG2_DSIPHY_LPRX_DLY_SHFT                                  0x0

#define HWIO_DSI_1_PHY_DSIPHY_DLN2_TEST_DATAPATH_ADDR                                  (DSI_1_PHY_DSIPHY_DLN2_DSIPHY_DLN2_DSI_DATALN_REG_BASE      + 0x0000000c)
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_TEST_DATAPATH_RMSK                                        0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_TEST_DATAPATH_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN2_TEST_DATAPATH_ADDR, HWIO_DSI_1_PHY_DSIPHY_DLN2_TEST_DATAPATH_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_TEST_DATAPATH_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN2_TEST_DATAPATH_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_TEST_DATAPATH_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_DLN2_TEST_DATAPATH_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_TEST_DATAPATH_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_DLN2_TEST_DATAPATH_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_DLN2_TEST_DATAPATH_IN)
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_TEST_DATAPATH_TEST_MODE_LPTX_EN_BMSK                      0x80
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_TEST_DATAPATH_TEST_MODE_LPTX_EN_SHFT                       0x7
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_TEST_DATAPATH_ANE_BYPASS_EN_BMSK                          0x40
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_TEST_DATAPATH_ANE_BYPASS_EN_SHFT                           0x6
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_TEST_DATAPATH_TEST_MODE_HSTX_DATA_BMSK                    0x20
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_TEST_DATAPATH_TEST_MODE_HSTX_DATA_SHFT                     0x5
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_TEST_DATAPATH_DMUX_LP_SEL_BMSK                            0x18
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_TEST_DATAPATH_DMUX_LP_SEL_SHFT                             0x3
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_TEST_DATAPATH_DMUX_HS_SEL_BMSK                             0x4
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_TEST_DATAPATH_DMUX_HS_SEL_SHFT                             0x2
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_TEST_DATAPATH_TEST_MODE_HSTX_EN_BMSK                       0x2
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_TEST_DATAPATH_TEST_MODE_HSTX_EN_SHFT                       0x1
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_TEST_DATAPATH_FORCE_TEST_MODE_BMSK                         0x1
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_TEST_DATAPATH_FORCE_TEST_MODE_SHFT                         0x0

#define HWIO_DSI_1_PHY_DSIPHY_DLN2_PIN_SWAP_ADDR                                       (DSI_1_PHY_DSIPHY_DLN2_DSIPHY_DLN2_DSI_DATALN_REG_BASE      + 0x00000010)
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_PIN_SWAP_RMSK                                              0xf
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_PIN_SWAP_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN2_PIN_SWAP_ADDR, HWIO_DSI_1_PHY_DSIPHY_DLN2_PIN_SWAP_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_PIN_SWAP_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN2_PIN_SWAP_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_PIN_SWAP_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_DLN2_PIN_SWAP_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_PIN_SWAP_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_DLN2_PIN_SWAP_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_DLN2_PIN_SWAP_IN)
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_PIN_SWAP_ABC_SWAP_BMSK                                     0xe
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_PIN_SWAP_ABC_SWAP_SHFT                                     0x1
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_PIN_SWAP_PN_SWAP_BMSK                                      0x1
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_PIN_SWAP_PN_SWAP_SHFT                                      0x0

#define HWIO_DSI_1_PHY_DSIPHY_DLN2_LPRX_CTRL_ADDR                                      (DSI_1_PHY_DSIPHY_DLN2_DSIPHY_DLN2_DSI_DATALN_REG_BASE      + 0x00000014)
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_LPRX_CTRL_RMSK                                             0x7
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_LPRX_CTRL_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN2_LPRX_CTRL_ADDR, HWIO_DSI_1_PHY_DSIPHY_DLN2_LPRX_CTRL_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_LPRX_CTRL_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN2_LPRX_CTRL_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_LPRX_CTRL_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_DLN2_LPRX_CTRL_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_LPRX_CTRL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_DLN2_LPRX_CTRL_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_DLN2_LPRX_CTRL_IN)
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_LPRX_CTRL_HS_CONTENTION_DIS_BMSK                           0x4
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_LPRX_CTRL_HS_CONTENTION_DIS_SHFT                           0x2
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_LPRX_CTRL_DSIPHY_LPRX_EN_BMSK                              0x2
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_LPRX_CTRL_DSIPHY_LPRX_EN_SHFT                              0x1
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_LPRX_CTRL_DSIPHY_CDRX_EN_BMSK                              0x1
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_LPRX_CTRL_DSIPHY_CDRX_EN_SHFT                              0x0

#define HWIO_DSI_1_PHY_DSIPHY_DLN2_TX_DCTRL_ADDR                                       (DSI_1_PHY_DSIPHY_DLN2_DSIPHY_DLN2_DSI_DATALN_REG_BASE      + 0x00000018)
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_TX_DCTRL_RMSK                                             0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_TX_DCTRL_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN2_TX_DCTRL_ADDR, HWIO_DSI_1_PHY_DSIPHY_DLN2_TX_DCTRL_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_TX_DCTRL_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN2_TX_DCTRL_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_TX_DCTRL_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_DLN2_TX_DCTRL_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_TX_DCTRL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_DLN2_TX_DCTRL_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_DLN2_TX_DCTRL_IN)
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_TX_DCTRL_DCTRL_BMSK                                       0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_TX_DCTRL_DCTRL_SHFT                                        0x0

#define HWIO_DSI_1_PHY_DSIPHY_DLN2_BIST_CTRL0_ADDR                                     (DSI_1_PHY_DSIPHY_DLN2_DSIPHY_DLN2_DSI_DATALN_REG_BASE      + 0x0000001c)
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_BIST_CTRL0_RMSK                                           0x3f
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_BIST_CTRL0_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN2_BIST_CTRL0_ADDR, HWIO_DSI_1_PHY_DSIPHY_DLN2_BIST_CTRL0_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_BIST_CTRL0_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN2_BIST_CTRL0_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_BIST_CTRL0_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_DLN2_BIST_CTRL0_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_BIST_CTRL0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_DLN2_BIST_CTRL0_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_DLN2_BIST_CTRL0_IN)
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_BIST_CTRL0_LP_BISTCHK_ENABLE_BMSK                         0x20
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_BIST_CTRL0_LP_BISTCHK_ENABLE_SHFT                          0x5
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_BIST_CTRL0_BISTCHK_CAPTURE_BMSK                           0x10
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_BIST_CTRL0_BISTCHK_CAPTURE_SHFT                            0x4
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_BIST_CTRL0_BISTCHK_CLR_ERROR_CNT_BMSK                      0x8
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_BIST_CTRL0_BISTCHK_CLR_ERROR_CNT_SHFT                      0x3
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_BIST_CTRL0_BISTCHK_MODE_BMSK                               0x4
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_BIST_CTRL0_BISTCHK_MODE_SHFT                               0x2
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_BIST_CTRL0_BISTCHK_CROSS_BMSK                              0x2
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_BIST_CTRL0_BISTCHK_CROSS_SHFT                              0x1
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_BIST_CTRL0_BISTCHK_ENABLE_BMSK                             0x1
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_BIST_CTRL0_BISTCHK_ENABLE_SHFT                             0x0

#define HWIO_DSI_1_PHY_DSIPHY_DLN2_BIST_CTRL1_ADDR                                     (DSI_1_PHY_DSIPHY_DLN2_DSIPHY_DLN2_DSI_DATALN_REG_BASE      + 0x00000020)
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_BIST_CTRL1_RMSK                                           0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_BIST_CTRL1_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN2_BIST_CTRL1_ADDR, HWIO_DSI_1_PHY_DSIPHY_DLN2_BIST_CTRL1_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_BIST_CTRL1_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN2_BIST_CTRL1_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_BIST_CTRL1_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_DLN2_BIST_CTRL1_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_BIST_CTRL1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_DLN2_BIST_CTRL1_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_DLN2_BIST_CTRL1_IN)
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_BIST_CTRL1_BIST_WORD_CNT_7_0_BMSK                         0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_BIST_CTRL1_BIST_WORD_CNT_7_0_SHFT                          0x0

#define HWIO_DSI_1_PHY_DSIPHY_DLN2_BIST_CTRL2_ADDR                                     (DSI_1_PHY_DSIPHY_DLN2_DSIPHY_DLN2_DSI_DATALN_REG_BASE      + 0x00000024)
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_BIST_CTRL2_RMSK                                           0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_BIST_CTRL2_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN2_BIST_CTRL2_ADDR, HWIO_DSI_1_PHY_DSIPHY_DLN2_BIST_CTRL2_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_BIST_CTRL2_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN2_BIST_CTRL2_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_BIST_CTRL2_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_DLN2_BIST_CTRL2_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_BIST_CTRL2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_DLN2_BIST_CTRL2_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_DLN2_BIST_CTRL2_IN)
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_BIST_CTRL2_BIST_WORD_CNT_15_8_BMSK                        0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_BIST_CTRL2_BIST_WORD_CNT_15_8_SHFT                         0x0

#define HWIO_DSI_1_PHY_DSIPHY_DLN2_MISR_CTRL_ADDR                                      (DSI_1_PHY_DSIPHY_DLN2_DSIPHY_DLN2_DSI_DATALN_REG_BASE      + 0x00000028)
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_MISR_CTRL_RMSK                                            0x1f
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_MISR_CTRL_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN2_MISR_CTRL_ADDR, HWIO_DSI_1_PHY_DSIPHY_DLN2_MISR_CTRL_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_MISR_CTRL_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN2_MISR_CTRL_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_MISR_CTRL_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_DLN2_MISR_CTRL_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_MISR_CTRL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_DLN2_MISR_CTRL_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_DLN2_MISR_CTRL_IN)
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_MISR_CTRL_DEBUG_CLK_DEBUG_SEL_BMSK                        0x1c
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_MISR_CTRL_DEBUG_CLK_DEBUG_SEL_SHFT                         0x2
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_MISR_CTRL_MISR_SIG_CLEAR_BMSK                              0x2
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_MISR_CTRL_MISR_SIG_CLEAR_SHFT                              0x1
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_MISR_CTRL_MISR_ENABLE_BMSK                                 0x1
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_MISR_CTRL_MISR_ENABLE_SHFT                                 0x0

#define HWIO_DSI_1_PHY_DSIPHY_DLN2_DBG_SEL_ADDR                                        (DSI_1_PHY_DSIPHY_DLN2_DSIPHY_DLN2_DSI_DATALN_REG_BASE      + 0x0000002c)
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_DBG_SEL_RMSK                                              0x7f
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_DBG_SEL_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN2_DBG_SEL_ADDR, HWIO_DSI_1_PHY_DSIPHY_DLN2_DBG_SEL_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_DBG_SEL_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN2_DBG_SEL_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_DBG_SEL_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_DLN2_DBG_SEL_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_DBG_SEL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_DLN2_DBG_SEL_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_DLN2_DBG_SEL_IN)
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_DBG_SEL_TEST_CLK_DEBUG_SEL_BMSK                           0x70
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_DBG_SEL_TEST_CLK_DEBUG_SEL_SHFT                            0x4
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_DBG_SEL_DEBUG_SEL_BMSK                                     0xf
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_DBG_SEL_DEBUG_SEL_SHFT                                     0x0

#define HWIO_DSI_1_PHY_DSIPHY_DLN2_SWI_DBG_BUS_CHECK_ADDR                              (DSI_1_PHY_DSIPHY_DLN2_DSIPHY_DLN2_DSI_DATALN_REG_BASE      + 0x00000030)
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_SWI_DBG_BUS_CHECK_RMSK                                    0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_SWI_DBG_BUS_CHECK_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN2_SWI_DBG_BUS_CHECK_ADDR, HWIO_DSI_1_PHY_DSIPHY_DLN2_SWI_DBG_BUS_CHECK_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_SWI_DBG_BUS_CHECK_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN2_SWI_DBG_BUS_CHECK_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_SWI_DBG_BUS_CHECK_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_DLN2_SWI_DBG_BUS_CHECK_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_SWI_DBG_BUS_CHECK_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_DLN2_SWI_DBG_BUS_CHECK_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_DLN2_SWI_DBG_BUS_CHECK_IN)
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_SWI_DBG_BUS_CHECK_LANE_DEBUG_BUS_BMSK                     0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_SWI_DBG_BUS_CHECK_LANE_DEBUG_BUS_SHFT                      0x0

#define HWIO_DSI_1_PHY_DSIPHY_DLN2_BIST_STATUS0_ADDR                                   (DSI_1_PHY_DSIPHY_DLN2_DSIPHY_DLN2_DSI_DATALN_REG_BASE      + 0x00000034)
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_BIST_STATUS0_RMSK                                         0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_BIST_STATUS0_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN2_BIST_STATUS0_ADDR, HWIO_DSI_1_PHY_DSIPHY_DLN2_BIST_STATUS0_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_BIST_STATUS0_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN2_BIST_STATUS0_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_BIST_STATUS0_BISTCHK_ONE_BIT_ERROR_BMSK                   0x80
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_BIST_STATUS0_BISTCHK_ONE_BIT_ERROR_SHFT                    0x7
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_BIST_STATUS0_BISTCHK_SYNC_ERROR_BMSK                      0x40
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_BIST_STATUS0_BISTCHK_SYNC_ERROR_SHFT                       0x6
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_BIST_STATUS0_BISTCHK_SEED_ERROR_BMSK                      0x20
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_BIST_STATUS0_BISTCHK_SEED_ERROR_SHFT                       0x5
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_BIST_STATUS0_BISTCHK_PRBS_ERROR_BMSK                      0x10
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_BIST_STATUS0_BISTCHK_PRBS_ERROR_SHFT                       0x4
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_BIST_STATUS0_BISTCHK_STATUS_BMSK                           0xe
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_BIST_STATUS0_BISTCHK_STATUS_SHFT                           0x1
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_BIST_STATUS0_BISTCHK_DONE_BMSK                             0x1
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_BIST_STATUS0_BISTCHK_DONE_SHFT                             0x0

#define HWIO_DSI_1_PHY_DSIPHY_DLN2_BIST_STATUS1_ADDR                                   (DSI_1_PHY_DSIPHY_DLN2_DSIPHY_DLN2_DSI_DATALN_REG_BASE      + 0x00000038)
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_BIST_STATUS1_RMSK                                         0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_BIST_STATUS1_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN2_BIST_STATUS1_ADDR, HWIO_DSI_1_PHY_DSIPHY_DLN2_BIST_STATUS1_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_BIST_STATUS1_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN2_BIST_STATUS1_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_BIST_STATUS1_BISTCHK_ERROR_CNT_7_0_BMSK                   0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_BIST_STATUS1_BISTCHK_ERROR_CNT_7_0_SHFT                    0x0

#define HWIO_DSI_1_PHY_DSIPHY_DLN2_BIST_STATUS2_ADDR                                   (DSI_1_PHY_DSIPHY_DLN2_DSIPHY_DLN2_DSI_DATALN_REG_BASE      + 0x0000003c)
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_BIST_STATUS2_RMSK                                         0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_BIST_STATUS2_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN2_BIST_STATUS2_ADDR, HWIO_DSI_1_PHY_DSIPHY_DLN2_BIST_STATUS2_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_BIST_STATUS2_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN2_BIST_STATUS2_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_BIST_STATUS2_BISTCHK_ERROR_CNT_15_8_BMSK                  0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_BIST_STATUS2_BISTCHK_ERROR_CNT_15_8_SHFT                   0x0

#define HWIO_DSI_1_PHY_DSIPHY_DLN2_BIST_STATUS3_ADDR                                   (DSI_1_PHY_DSIPHY_DLN2_DSIPHY_DLN2_DSI_DATALN_REG_BASE      + 0x00000040)
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_BIST_STATUS3_RMSK                                         0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_BIST_STATUS3_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN2_BIST_STATUS3_ADDR, HWIO_DSI_1_PHY_DSIPHY_DLN2_BIST_STATUS3_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_BIST_STATUS3_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN2_BIST_STATUS3_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_BIST_STATUS3_BISTCHK_WORD_CNT_7_0_BMSK                    0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_BIST_STATUS3_BISTCHK_WORD_CNT_7_0_SHFT                     0x0

#define HWIO_DSI_1_PHY_DSIPHY_DLN2_BIST_STATUS4_ADDR                                   (DSI_1_PHY_DSIPHY_DLN2_DSIPHY_DLN2_DSI_DATALN_REG_BASE      + 0x00000044)
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_BIST_STATUS4_RMSK                                         0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_BIST_STATUS4_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN2_BIST_STATUS4_ADDR, HWIO_DSI_1_PHY_DSIPHY_DLN2_BIST_STATUS4_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_BIST_STATUS4_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN2_BIST_STATUS4_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_BIST_STATUS4_BISTCHK_WORD_CNT_15_8_BMSK                   0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_BIST_STATUS4_BISTCHK_WORD_CNT_15_8_SHFT                    0x0

#define HWIO_DSI_1_PHY_DSIPHY_DLN2_LP_BIST_STATUS0_ADDR                                (DSI_1_PHY_DSIPHY_DLN2_DSIPHY_DLN2_DSI_DATALN_REG_BASE      + 0x00000048)
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_LP_BIST_STATUS0_RMSK                                       0x3
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_LP_BIST_STATUS0_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN2_LP_BIST_STATUS0_ADDR, HWIO_DSI_1_PHY_DSIPHY_DLN2_LP_BIST_STATUS0_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_LP_BIST_STATUS0_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN2_LP_BIST_STATUS0_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_LP_BIST_STATUS0_LP_BISTCHK_PRBS_ERROR_BMSK                 0x2
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_LP_BIST_STATUS0_LP_BISTCHK_PRBS_ERROR_SHFT                 0x1
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_LP_BIST_STATUS0_LP_BISTCHK_DONE_BMSK                       0x1
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_LP_BIST_STATUS0_LP_BISTCHK_DONE_SHFT                       0x0

#define HWIO_DSI_1_PHY_DSIPHY_DLN2_LP_BIST_STATUS1_ADDR                                (DSI_1_PHY_DSIPHY_DLN2_DSIPHY_DLN2_DSI_DATALN_REG_BASE      + 0x0000004c)
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_LP_BIST_STATUS1_RMSK                                      0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_LP_BIST_STATUS1_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN2_LP_BIST_STATUS1_ADDR, HWIO_DSI_1_PHY_DSIPHY_DLN2_LP_BIST_STATUS1_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_LP_BIST_STATUS1_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN2_LP_BIST_STATUS1_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_LP_BIST_STATUS1_LP_BISTCHK_ERROR_CNT_BMSK                 0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_LP_BIST_STATUS1_LP_BISTCHK_ERROR_CNT_SHFT                  0x0

#define HWIO_DSI_1_PHY_DSIPHY_DLN2_LP_BIST_STATUS2_ADDR                                (DSI_1_PHY_DSIPHY_DLN2_DSIPHY_DLN2_DSI_DATALN_REG_BASE      + 0x00000050)
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_LP_BIST_STATUS2_RMSK                                      0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_LP_BIST_STATUS2_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN2_LP_BIST_STATUS2_ADDR, HWIO_DSI_1_PHY_DSIPHY_DLN2_LP_BIST_STATUS2_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_LP_BIST_STATUS2_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN2_LP_BIST_STATUS2_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_LP_BIST_STATUS2_LP_BISTCHK_WORD_CNT_BMSK                  0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_LP_BIST_STATUS2_LP_BISTCHK_WORD_CNT_SHFT                   0x0

#define HWIO_DSI_1_PHY_DSIPHY_DLN2_MISR_STATUS0_ADDR                                   (DSI_1_PHY_DSIPHY_DLN2_DSIPHY_DLN2_DSI_DATALN_REG_BASE      + 0x00000054)
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_MISR_STATUS0_RMSK                                         0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_MISR_STATUS0_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN2_MISR_STATUS0_ADDR, HWIO_DSI_1_PHY_DSIPHY_DLN2_MISR_STATUS0_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_MISR_STATUS0_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN2_MISR_STATUS0_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_MISR_STATUS0_BISTCHK_MISR_SIG_7_0_BMSK                    0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_MISR_STATUS0_BISTCHK_MISR_SIG_7_0_SHFT                     0x0

#define HWIO_DSI_1_PHY_DSIPHY_DLN2_MISR_STATUS1_ADDR                                   (DSI_1_PHY_DSIPHY_DLN2_DSIPHY_DLN2_DSI_DATALN_REG_BASE      + 0x00000058)
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_MISR_STATUS1_RMSK                                         0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_MISR_STATUS1_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN2_MISR_STATUS1_ADDR, HWIO_DSI_1_PHY_DSIPHY_DLN2_MISR_STATUS1_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_MISR_STATUS1_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN2_MISR_STATUS1_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_MISR_STATUS1_BISTCHK_MISR_SIG_15_8_BMSK                   0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_MISR_STATUS1_BISTCHK_MISR_SIG_15_8_SHFT                    0x0

#define HWIO_DSI_1_PHY_DSIPHY_DLN2_DEBUG_BUS0_ADDR                                     (DSI_1_PHY_DSIPHY_DLN2_DSIPHY_DLN2_DSI_DATALN_REG_BASE      + 0x0000005c)
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_DEBUG_BUS0_RMSK                                           0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_DEBUG_BUS0_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN2_DEBUG_BUS0_ADDR, HWIO_DSI_1_PHY_DSIPHY_DLN2_DEBUG_BUS0_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_DEBUG_BUS0_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN2_DEBUG_BUS0_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_DEBUG_BUS0_DEBUG_BUS_7_0_BMSK                             0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_DEBUG_BUS0_DEBUG_BUS_7_0_SHFT                              0x0

#define HWIO_DSI_1_PHY_DSIPHY_DLN2_DEBUG_BUS1_ADDR                                     (DSI_1_PHY_DSIPHY_DLN2_DSIPHY_DLN2_DSI_DATALN_REG_BASE      + 0x00000060)
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_DEBUG_BUS1_RMSK                                           0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_DEBUG_BUS1_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN2_DEBUG_BUS1_ADDR, HWIO_DSI_1_PHY_DSIPHY_DLN2_DEBUG_BUS1_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_DEBUG_BUS1_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN2_DEBUG_BUS1_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_DEBUG_BUS1_DEBUG_BUS_15_8_BMSK                            0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_DEBUG_BUS1_DEBUG_BUS_15_8_SHFT                             0x0

#define HWIO_DSI_1_PHY_DSIPHY_DLN2_DEBUG_BUS2_ADDR                                     (DSI_1_PHY_DSIPHY_DLN2_DSIPHY_DLN2_DSI_DATALN_REG_BASE      + 0x00000064)
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_DEBUG_BUS2_RMSK                                           0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_DEBUG_BUS2_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN2_DEBUG_BUS2_ADDR, HWIO_DSI_1_PHY_DSIPHY_DLN2_DEBUG_BUS2_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_DEBUG_BUS2_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN2_DEBUG_BUS2_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_DEBUG_BUS2_DEBUG_BUS_23_16_BMSK                           0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_DEBUG_BUS2_DEBUG_BUS_23_16_SHFT                            0x0

#define HWIO_DSI_1_PHY_DSIPHY_DLN2_DEBUG_BUS3_ADDR                                     (DSI_1_PHY_DSIPHY_DLN2_DSIPHY_DLN2_DSI_DATALN_REG_BASE      + 0x00000068)
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_DEBUG_BUS3_RMSK                                           0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_DEBUG_BUS3_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN2_DEBUG_BUS3_ADDR, HWIO_DSI_1_PHY_DSIPHY_DLN2_DEBUG_BUS3_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_DEBUG_BUS3_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN2_DEBUG_BUS3_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_DEBUG_BUS3_DEBUG_BUS_31_24_BMSK                           0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN2_DEBUG_BUS3_DEBUG_BUS_31_24_SHFT                            0x0

/*----------------------------------------------------------------------------
 * MODULE: DSI_1_PHY_DSIPHY_DLN3_DSIPHY_DLN3_DSI_DATALN
 *--------------------------------------------------------------------------*/

#define DSI_1_PHY_DSIPHY_DLN3_DSIPHY_DLN3_DSI_DATALN_REG_BASE                          (MDSS_BASE      + 0x00096780)

#define HWIO_DSI_1_PHY_DSIPHY_DLN3_CFG0_ADDR                                           (DSI_1_PHY_DSIPHY_DLN3_DSIPHY_DLN3_DSI_DATALN_REG_BASE      + 0x00000000)
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_CFG0_RMSK                                                 0x3f
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_CFG0_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN3_CFG0_ADDR, HWIO_DSI_1_PHY_DSIPHY_DLN3_CFG0_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_CFG0_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN3_CFG0_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_CFG0_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_DLN3_CFG0_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_CFG0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_DLN3_CFG0_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_DLN3_CFG0_IN)
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_CFG0_DSIPHY_HSTX_PGM_DLY_5_4_BMSK                         0x30
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_CFG0_DSIPHY_HSTX_PGM_DLY_5_4_SHFT                          0x4
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_CFG0_DSIPHY_HSTX_PGM_DLY_3_0_BMSK                          0xf
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_CFG0_DSIPHY_HSTX_PGM_DLY_3_0_SHFT                          0x0

#define HWIO_DSI_1_PHY_DSIPHY_DLN3_CFG1_ADDR                                           (DSI_1_PHY_DSIPHY_DLN3_DSIPHY_DLN3_DSI_DATALN_REG_BASE      + 0x00000004)
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_CFG1_RMSK                                                 0x1f
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_CFG1_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN3_CFG1_ADDR, HWIO_DSI_1_PHY_DSIPHY_DLN3_CFG1_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_CFG1_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN3_CFG1_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_CFG1_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_DLN3_CFG1_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_CFG1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_DLN3_CFG1_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_DLN3_CFG1_IN)
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_CFG1_PROGSEQ_EN_BMSK                                      0x10
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_CFG1_PROGSEQ_EN_SHFT                                       0x4
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_CFG1_DSIPHY_HSTX_SLEW_RATE_BMSK                            0xf
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_CFG1_DSIPHY_HSTX_SLEW_RATE_SHFT                            0x0

#define HWIO_DSI_1_PHY_DSIPHY_DLN3_CFG2_ADDR                                           (DSI_1_PHY_DSIPHY_DLN3_DSIPHY_DLN3_DSI_DATALN_REG_BASE      + 0x00000008)
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_CFG2_RMSK                                                 0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_CFG2_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN3_CFG2_ADDR, HWIO_DSI_1_PHY_DSIPHY_DLN3_CFG2_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_CFG2_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN3_CFG2_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_CFG2_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_DLN3_CFG2_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_CFG2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_DLN3_CFG2_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_DLN3_CFG2_IN)
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_CFG2_IS_CKLANE_BMSK                                       0x80
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_CFG2_IS_CKLANE_SHFT                                        0x7
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_CFG2_CLK_PATTERN_HSTX_INV_BMSK                            0x40
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_CFG2_CLK_PATTERN_HSTX_INV_SHFT                             0x6
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_CFG2_TEST_MODE_LPTX_DATAN_BMSK                            0x20
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_CFG2_TEST_MODE_LPTX_DATAN_SHFT                             0x5
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_CFG2_TEST_MODE_LPTX_DATAP_BMSK                            0x10
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_CFG2_TEST_MODE_LPTX_DATAP_SHFT                             0x4
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_CFG2_DSIPHY_LPRX_DLY_BMSK                                  0xf
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_CFG2_DSIPHY_LPRX_DLY_SHFT                                  0x0

#define HWIO_DSI_1_PHY_DSIPHY_DLN3_TEST_DATAPATH_ADDR                                  (DSI_1_PHY_DSIPHY_DLN3_DSIPHY_DLN3_DSI_DATALN_REG_BASE      + 0x0000000c)
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_TEST_DATAPATH_RMSK                                        0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_TEST_DATAPATH_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN3_TEST_DATAPATH_ADDR, HWIO_DSI_1_PHY_DSIPHY_DLN3_TEST_DATAPATH_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_TEST_DATAPATH_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN3_TEST_DATAPATH_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_TEST_DATAPATH_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_DLN3_TEST_DATAPATH_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_TEST_DATAPATH_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_DLN3_TEST_DATAPATH_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_DLN3_TEST_DATAPATH_IN)
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_TEST_DATAPATH_TEST_MODE_LPTX_EN_BMSK                      0x80
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_TEST_DATAPATH_TEST_MODE_LPTX_EN_SHFT                       0x7
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_TEST_DATAPATH_ANE_BYPASS_EN_BMSK                          0x40
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_TEST_DATAPATH_ANE_BYPASS_EN_SHFT                           0x6
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_TEST_DATAPATH_TEST_MODE_HSTX_DATA_BMSK                    0x20
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_TEST_DATAPATH_TEST_MODE_HSTX_DATA_SHFT                     0x5
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_TEST_DATAPATH_DMUX_LP_SEL_BMSK                            0x18
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_TEST_DATAPATH_DMUX_LP_SEL_SHFT                             0x3
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_TEST_DATAPATH_DMUX_HS_SEL_BMSK                             0x4
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_TEST_DATAPATH_DMUX_HS_SEL_SHFT                             0x2
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_TEST_DATAPATH_TEST_MODE_HSTX_EN_BMSK                       0x2
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_TEST_DATAPATH_TEST_MODE_HSTX_EN_SHFT                       0x1
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_TEST_DATAPATH_FORCE_TEST_MODE_BMSK                         0x1
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_TEST_DATAPATH_FORCE_TEST_MODE_SHFT                         0x0

#define HWIO_DSI_1_PHY_DSIPHY_DLN3_PIN_SWAP_ADDR                                       (DSI_1_PHY_DSIPHY_DLN3_DSIPHY_DLN3_DSI_DATALN_REG_BASE      + 0x00000010)
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_PIN_SWAP_RMSK                                              0xf
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_PIN_SWAP_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN3_PIN_SWAP_ADDR, HWIO_DSI_1_PHY_DSIPHY_DLN3_PIN_SWAP_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_PIN_SWAP_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN3_PIN_SWAP_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_PIN_SWAP_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_DLN3_PIN_SWAP_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_PIN_SWAP_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_DLN3_PIN_SWAP_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_DLN3_PIN_SWAP_IN)
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_PIN_SWAP_ABC_SWAP_BMSK                                     0xe
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_PIN_SWAP_ABC_SWAP_SHFT                                     0x1
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_PIN_SWAP_PN_SWAP_BMSK                                      0x1
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_PIN_SWAP_PN_SWAP_SHFT                                      0x0

#define HWIO_DSI_1_PHY_DSIPHY_DLN3_LPRX_CTRL_ADDR                                      (DSI_1_PHY_DSIPHY_DLN3_DSIPHY_DLN3_DSI_DATALN_REG_BASE      + 0x00000014)
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_LPRX_CTRL_RMSK                                             0x7
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_LPRX_CTRL_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN3_LPRX_CTRL_ADDR, HWIO_DSI_1_PHY_DSIPHY_DLN3_LPRX_CTRL_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_LPRX_CTRL_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN3_LPRX_CTRL_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_LPRX_CTRL_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_DLN3_LPRX_CTRL_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_LPRX_CTRL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_DLN3_LPRX_CTRL_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_DLN3_LPRX_CTRL_IN)
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_LPRX_CTRL_HS_CONTENTION_DIS_BMSK                           0x4
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_LPRX_CTRL_HS_CONTENTION_DIS_SHFT                           0x2
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_LPRX_CTRL_DSIPHY_LPRX_EN_BMSK                              0x2
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_LPRX_CTRL_DSIPHY_LPRX_EN_SHFT                              0x1
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_LPRX_CTRL_DSIPHY_CDRX_EN_BMSK                              0x1
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_LPRX_CTRL_DSIPHY_CDRX_EN_SHFT                              0x0

#define HWIO_DSI_1_PHY_DSIPHY_DLN3_TX_DCTRL_ADDR                                       (DSI_1_PHY_DSIPHY_DLN3_DSIPHY_DLN3_DSI_DATALN_REG_BASE      + 0x00000018)
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_TX_DCTRL_RMSK                                             0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_TX_DCTRL_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN3_TX_DCTRL_ADDR, HWIO_DSI_1_PHY_DSIPHY_DLN3_TX_DCTRL_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_TX_DCTRL_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN3_TX_DCTRL_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_TX_DCTRL_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_DLN3_TX_DCTRL_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_TX_DCTRL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_DLN3_TX_DCTRL_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_DLN3_TX_DCTRL_IN)
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_TX_DCTRL_DCTRL_BMSK                                       0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_TX_DCTRL_DCTRL_SHFT                                        0x0

#define HWIO_DSI_1_PHY_DSIPHY_DLN3_BIST_CTRL0_ADDR                                     (DSI_1_PHY_DSIPHY_DLN3_DSIPHY_DLN3_DSI_DATALN_REG_BASE      + 0x0000001c)
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_BIST_CTRL0_RMSK                                           0x3f
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_BIST_CTRL0_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN3_BIST_CTRL0_ADDR, HWIO_DSI_1_PHY_DSIPHY_DLN3_BIST_CTRL0_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_BIST_CTRL0_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN3_BIST_CTRL0_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_BIST_CTRL0_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_DLN3_BIST_CTRL0_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_BIST_CTRL0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_DLN3_BIST_CTRL0_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_DLN3_BIST_CTRL0_IN)
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_BIST_CTRL0_LP_BISTCHK_ENABLE_BMSK                         0x20
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_BIST_CTRL0_LP_BISTCHK_ENABLE_SHFT                          0x5
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_BIST_CTRL0_BISTCHK_CAPTURE_BMSK                           0x10
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_BIST_CTRL0_BISTCHK_CAPTURE_SHFT                            0x4
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_BIST_CTRL0_BISTCHK_CLR_ERROR_CNT_BMSK                      0x8
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_BIST_CTRL0_BISTCHK_CLR_ERROR_CNT_SHFT                      0x3
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_BIST_CTRL0_BISTCHK_MODE_BMSK                               0x4
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_BIST_CTRL0_BISTCHK_MODE_SHFT                               0x2
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_BIST_CTRL0_BISTCHK_CROSS_BMSK                              0x2
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_BIST_CTRL0_BISTCHK_CROSS_SHFT                              0x1
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_BIST_CTRL0_BISTCHK_ENABLE_BMSK                             0x1
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_BIST_CTRL0_BISTCHK_ENABLE_SHFT                             0x0

#define HWIO_DSI_1_PHY_DSIPHY_DLN3_BIST_CTRL1_ADDR                                     (DSI_1_PHY_DSIPHY_DLN3_DSIPHY_DLN3_DSI_DATALN_REG_BASE      + 0x00000020)
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_BIST_CTRL1_RMSK                                           0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_BIST_CTRL1_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN3_BIST_CTRL1_ADDR, HWIO_DSI_1_PHY_DSIPHY_DLN3_BIST_CTRL1_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_BIST_CTRL1_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN3_BIST_CTRL1_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_BIST_CTRL1_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_DLN3_BIST_CTRL1_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_BIST_CTRL1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_DLN3_BIST_CTRL1_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_DLN3_BIST_CTRL1_IN)
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_BIST_CTRL1_BIST_WORD_CNT_7_0_BMSK                         0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_BIST_CTRL1_BIST_WORD_CNT_7_0_SHFT                          0x0

#define HWIO_DSI_1_PHY_DSIPHY_DLN3_BIST_CTRL2_ADDR                                     (DSI_1_PHY_DSIPHY_DLN3_DSIPHY_DLN3_DSI_DATALN_REG_BASE      + 0x00000024)
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_BIST_CTRL2_RMSK                                           0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_BIST_CTRL2_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN3_BIST_CTRL2_ADDR, HWIO_DSI_1_PHY_DSIPHY_DLN3_BIST_CTRL2_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_BIST_CTRL2_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN3_BIST_CTRL2_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_BIST_CTRL2_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_DLN3_BIST_CTRL2_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_BIST_CTRL2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_DLN3_BIST_CTRL2_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_DLN3_BIST_CTRL2_IN)
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_BIST_CTRL2_BIST_WORD_CNT_15_8_BMSK                        0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_BIST_CTRL2_BIST_WORD_CNT_15_8_SHFT                         0x0

#define HWIO_DSI_1_PHY_DSIPHY_DLN3_MISR_CTRL_ADDR                                      (DSI_1_PHY_DSIPHY_DLN3_DSIPHY_DLN3_DSI_DATALN_REG_BASE      + 0x00000028)
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_MISR_CTRL_RMSK                                            0x1f
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_MISR_CTRL_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN3_MISR_CTRL_ADDR, HWIO_DSI_1_PHY_DSIPHY_DLN3_MISR_CTRL_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_MISR_CTRL_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN3_MISR_CTRL_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_MISR_CTRL_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_DLN3_MISR_CTRL_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_MISR_CTRL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_DLN3_MISR_CTRL_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_DLN3_MISR_CTRL_IN)
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_MISR_CTRL_DEBUG_CLK_DEBUG_SEL_BMSK                        0x1c
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_MISR_CTRL_DEBUG_CLK_DEBUG_SEL_SHFT                         0x2
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_MISR_CTRL_MISR_SIG_CLEAR_BMSK                              0x2
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_MISR_CTRL_MISR_SIG_CLEAR_SHFT                              0x1
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_MISR_CTRL_MISR_ENABLE_BMSK                                 0x1
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_MISR_CTRL_MISR_ENABLE_SHFT                                 0x0

#define HWIO_DSI_1_PHY_DSIPHY_DLN3_DBG_SEL_ADDR                                        (DSI_1_PHY_DSIPHY_DLN3_DSIPHY_DLN3_DSI_DATALN_REG_BASE      + 0x0000002c)
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_DBG_SEL_RMSK                                              0x7f
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_DBG_SEL_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN3_DBG_SEL_ADDR, HWIO_DSI_1_PHY_DSIPHY_DLN3_DBG_SEL_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_DBG_SEL_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN3_DBG_SEL_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_DBG_SEL_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_DLN3_DBG_SEL_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_DBG_SEL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_DLN3_DBG_SEL_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_DLN3_DBG_SEL_IN)
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_DBG_SEL_TEST_CLK_DEBUG_SEL_BMSK                           0x70
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_DBG_SEL_TEST_CLK_DEBUG_SEL_SHFT                            0x4
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_DBG_SEL_DEBUG_SEL_BMSK                                     0xf
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_DBG_SEL_DEBUG_SEL_SHFT                                     0x0

#define HWIO_DSI_1_PHY_DSIPHY_DLN3_SWI_DBG_BUS_CHECK_ADDR                              (DSI_1_PHY_DSIPHY_DLN3_DSIPHY_DLN3_DSI_DATALN_REG_BASE      + 0x00000030)
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_SWI_DBG_BUS_CHECK_RMSK                                    0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_SWI_DBG_BUS_CHECK_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN3_SWI_DBG_BUS_CHECK_ADDR, HWIO_DSI_1_PHY_DSIPHY_DLN3_SWI_DBG_BUS_CHECK_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_SWI_DBG_BUS_CHECK_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN3_SWI_DBG_BUS_CHECK_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_SWI_DBG_BUS_CHECK_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_DLN3_SWI_DBG_BUS_CHECK_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_SWI_DBG_BUS_CHECK_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_DLN3_SWI_DBG_BUS_CHECK_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_DLN3_SWI_DBG_BUS_CHECK_IN)
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_SWI_DBG_BUS_CHECK_LANE_DEBUG_BUS_BMSK                     0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_SWI_DBG_BUS_CHECK_LANE_DEBUG_BUS_SHFT                      0x0

#define HWIO_DSI_1_PHY_DSIPHY_DLN3_BIST_STATUS0_ADDR                                   (DSI_1_PHY_DSIPHY_DLN3_DSIPHY_DLN3_DSI_DATALN_REG_BASE      + 0x00000034)
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_BIST_STATUS0_RMSK                                         0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_BIST_STATUS0_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN3_BIST_STATUS0_ADDR, HWIO_DSI_1_PHY_DSIPHY_DLN3_BIST_STATUS0_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_BIST_STATUS0_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN3_BIST_STATUS0_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_BIST_STATUS0_BISTCHK_ONE_BIT_ERROR_BMSK                   0x80
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_BIST_STATUS0_BISTCHK_ONE_BIT_ERROR_SHFT                    0x7
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_BIST_STATUS0_BISTCHK_SYNC_ERROR_BMSK                      0x40
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_BIST_STATUS0_BISTCHK_SYNC_ERROR_SHFT                       0x6
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_BIST_STATUS0_BISTCHK_SEED_ERROR_BMSK                      0x20
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_BIST_STATUS0_BISTCHK_SEED_ERROR_SHFT                       0x5
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_BIST_STATUS0_BISTCHK_PRBS_ERROR_BMSK                      0x10
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_BIST_STATUS0_BISTCHK_PRBS_ERROR_SHFT                       0x4
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_BIST_STATUS0_BISTCHK_STATUS_BMSK                           0xe
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_BIST_STATUS0_BISTCHK_STATUS_SHFT                           0x1
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_BIST_STATUS0_BISTCHK_DONE_BMSK                             0x1
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_BIST_STATUS0_BISTCHK_DONE_SHFT                             0x0

#define HWIO_DSI_1_PHY_DSIPHY_DLN3_BIST_STATUS1_ADDR                                   (DSI_1_PHY_DSIPHY_DLN3_DSIPHY_DLN3_DSI_DATALN_REG_BASE      + 0x00000038)
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_BIST_STATUS1_RMSK                                         0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_BIST_STATUS1_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN3_BIST_STATUS1_ADDR, HWIO_DSI_1_PHY_DSIPHY_DLN3_BIST_STATUS1_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_BIST_STATUS1_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN3_BIST_STATUS1_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_BIST_STATUS1_BISTCHK_ERROR_CNT_7_0_BMSK                   0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_BIST_STATUS1_BISTCHK_ERROR_CNT_7_0_SHFT                    0x0

#define HWIO_DSI_1_PHY_DSIPHY_DLN3_BIST_STATUS2_ADDR                                   (DSI_1_PHY_DSIPHY_DLN3_DSIPHY_DLN3_DSI_DATALN_REG_BASE      + 0x0000003c)
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_BIST_STATUS2_RMSK                                         0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_BIST_STATUS2_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN3_BIST_STATUS2_ADDR, HWIO_DSI_1_PHY_DSIPHY_DLN3_BIST_STATUS2_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_BIST_STATUS2_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN3_BIST_STATUS2_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_BIST_STATUS2_BISTCHK_ERROR_CNT_15_8_BMSK                  0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_BIST_STATUS2_BISTCHK_ERROR_CNT_15_8_SHFT                   0x0

#define HWIO_DSI_1_PHY_DSIPHY_DLN3_BIST_STATUS3_ADDR                                   (DSI_1_PHY_DSIPHY_DLN3_DSIPHY_DLN3_DSI_DATALN_REG_BASE      + 0x00000040)
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_BIST_STATUS3_RMSK                                         0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_BIST_STATUS3_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN3_BIST_STATUS3_ADDR, HWIO_DSI_1_PHY_DSIPHY_DLN3_BIST_STATUS3_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_BIST_STATUS3_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN3_BIST_STATUS3_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_BIST_STATUS3_BISTCHK_WORD_CNT_7_0_BMSK                    0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_BIST_STATUS3_BISTCHK_WORD_CNT_7_0_SHFT                     0x0

#define HWIO_DSI_1_PHY_DSIPHY_DLN3_BIST_STATUS4_ADDR                                   (DSI_1_PHY_DSIPHY_DLN3_DSIPHY_DLN3_DSI_DATALN_REG_BASE      + 0x00000044)
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_BIST_STATUS4_RMSK                                         0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_BIST_STATUS4_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN3_BIST_STATUS4_ADDR, HWIO_DSI_1_PHY_DSIPHY_DLN3_BIST_STATUS4_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_BIST_STATUS4_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN3_BIST_STATUS4_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_BIST_STATUS4_BISTCHK_WORD_CNT_15_8_BMSK                   0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_BIST_STATUS4_BISTCHK_WORD_CNT_15_8_SHFT                    0x0

#define HWIO_DSI_1_PHY_DSIPHY_DLN3_LP_BIST_STATUS0_ADDR                                (DSI_1_PHY_DSIPHY_DLN3_DSIPHY_DLN3_DSI_DATALN_REG_BASE      + 0x00000048)
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_LP_BIST_STATUS0_RMSK                                       0x3
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_LP_BIST_STATUS0_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN3_LP_BIST_STATUS0_ADDR, HWIO_DSI_1_PHY_DSIPHY_DLN3_LP_BIST_STATUS0_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_LP_BIST_STATUS0_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN3_LP_BIST_STATUS0_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_LP_BIST_STATUS0_LP_BISTCHK_PRBS_ERROR_BMSK                 0x2
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_LP_BIST_STATUS0_LP_BISTCHK_PRBS_ERROR_SHFT                 0x1
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_LP_BIST_STATUS0_LP_BISTCHK_DONE_BMSK                       0x1
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_LP_BIST_STATUS0_LP_BISTCHK_DONE_SHFT                       0x0

#define HWIO_DSI_1_PHY_DSIPHY_DLN3_LP_BIST_STATUS1_ADDR                                (DSI_1_PHY_DSIPHY_DLN3_DSIPHY_DLN3_DSI_DATALN_REG_BASE      + 0x0000004c)
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_LP_BIST_STATUS1_RMSK                                      0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_LP_BIST_STATUS1_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN3_LP_BIST_STATUS1_ADDR, HWIO_DSI_1_PHY_DSIPHY_DLN3_LP_BIST_STATUS1_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_LP_BIST_STATUS1_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN3_LP_BIST_STATUS1_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_LP_BIST_STATUS1_LP_BISTCHK_ERROR_CNT_BMSK                 0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_LP_BIST_STATUS1_LP_BISTCHK_ERROR_CNT_SHFT                  0x0

#define HWIO_DSI_1_PHY_DSIPHY_DLN3_LP_BIST_STATUS2_ADDR                                (DSI_1_PHY_DSIPHY_DLN3_DSIPHY_DLN3_DSI_DATALN_REG_BASE      + 0x00000050)
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_LP_BIST_STATUS2_RMSK                                      0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_LP_BIST_STATUS2_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN3_LP_BIST_STATUS2_ADDR, HWIO_DSI_1_PHY_DSIPHY_DLN3_LP_BIST_STATUS2_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_LP_BIST_STATUS2_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN3_LP_BIST_STATUS2_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_LP_BIST_STATUS2_LP_BISTCHK_WORD_CNT_BMSK                  0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_LP_BIST_STATUS2_LP_BISTCHK_WORD_CNT_SHFT                   0x0

#define HWIO_DSI_1_PHY_DSIPHY_DLN3_MISR_STATUS0_ADDR                                   (DSI_1_PHY_DSIPHY_DLN3_DSIPHY_DLN3_DSI_DATALN_REG_BASE      + 0x00000054)
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_MISR_STATUS0_RMSK                                         0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_MISR_STATUS0_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN3_MISR_STATUS0_ADDR, HWIO_DSI_1_PHY_DSIPHY_DLN3_MISR_STATUS0_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_MISR_STATUS0_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN3_MISR_STATUS0_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_MISR_STATUS0_BISTCHK_MISR_SIG_7_0_BMSK                    0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_MISR_STATUS0_BISTCHK_MISR_SIG_7_0_SHFT                     0x0

#define HWIO_DSI_1_PHY_DSIPHY_DLN3_MISR_STATUS1_ADDR                                   (DSI_1_PHY_DSIPHY_DLN3_DSIPHY_DLN3_DSI_DATALN_REG_BASE      + 0x00000058)
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_MISR_STATUS1_RMSK                                         0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_MISR_STATUS1_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN3_MISR_STATUS1_ADDR, HWIO_DSI_1_PHY_DSIPHY_DLN3_MISR_STATUS1_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_MISR_STATUS1_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN3_MISR_STATUS1_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_MISR_STATUS1_BISTCHK_MISR_SIG_15_8_BMSK                   0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_MISR_STATUS1_BISTCHK_MISR_SIG_15_8_SHFT                    0x0

#define HWIO_DSI_1_PHY_DSIPHY_DLN3_DEBUG_BUS0_ADDR                                     (DSI_1_PHY_DSIPHY_DLN3_DSIPHY_DLN3_DSI_DATALN_REG_BASE      + 0x0000005c)
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_DEBUG_BUS0_RMSK                                           0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_DEBUG_BUS0_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN3_DEBUG_BUS0_ADDR, HWIO_DSI_1_PHY_DSIPHY_DLN3_DEBUG_BUS0_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_DEBUG_BUS0_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN3_DEBUG_BUS0_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_DEBUG_BUS0_DEBUG_BUS_7_0_BMSK                             0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_DEBUG_BUS0_DEBUG_BUS_7_0_SHFT                              0x0

#define HWIO_DSI_1_PHY_DSIPHY_DLN3_DEBUG_BUS1_ADDR                                     (DSI_1_PHY_DSIPHY_DLN3_DSIPHY_DLN3_DSI_DATALN_REG_BASE      + 0x00000060)
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_DEBUG_BUS1_RMSK                                           0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_DEBUG_BUS1_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN3_DEBUG_BUS1_ADDR, HWIO_DSI_1_PHY_DSIPHY_DLN3_DEBUG_BUS1_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_DEBUG_BUS1_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN3_DEBUG_BUS1_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_DEBUG_BUS1_DEBUG_BUS_15_8_BMSK                            0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_DEBUG_BUS1_DEBUG_BUS_15_8_SHFT                             0x0

#define HWIO_DSI_1_PHY_DSIPHY_DLN3_DEBUG_BUS2_ADDR                                     (DSI_1_PHY_DSIPHY_DLN3_DSIPHY_DLN3_DSI_DATALN_REG_BASE      + 0x00000064)
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_DEBUG_BUS2_RMSK                                           0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_DEBUG_BUS2_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN3_DEBUG_BUS2_ADDR, HWIO_DSI_1_PHY_DSIPHY_DLN3_DEBUG_BUS2_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_DEBUG_BUS2_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN3_DEBUG_BUS2_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_DEBUG_BUS2_DEBUG_BUS_23_16_BMSK                           0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_DEBUG_BUS2_DEBUG_BUS_23_16_SHFT                            0x0

#define HWIO_DSI_1_PHY_DSIPHY_DLN3_DEBUG_BUS3_ADDR                                     (DSI_1_PHY_DSIPHY_DLN3_DSIPHY_DLN3_DSI_DATALN_REG_BASE      + 0x00000068)
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_DEBUG_BUS3_RMSK                                           0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_DEBUG_BUS3_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN3_DEBUG_BUS3_ADDR, HWIO_DSI_1_PHY_DSIPHY_DLN3_DEBUG_BUS3_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_DEBUG_BUS3_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_DLN3_DEBUG_BUS3_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_DEBUG_BUS3_DEBUG_BUS_31_24_BMSK                           0xff
#define HWIO_DSI_1_PHY_DSIPHY_DLN3_DEBUG_BUS3_DEBUG_BUS_31_24_SHFT                            0x0

/*----------------------------------------------------------------------------
 * MODULE: DSI_1_PHY_DSIPHY_CKLN_DSIPHY_CKLN_DSI_DATALN
 *--------------------------------------------------------------------------*/

#define DSI_1_PHY_DSIPHY_CKLN_DSIPHY_CKLN_DSI_DATALN_REG_BASE                          (MDSS_BASE      + 0x00096800)

#define HWIO_DSI_1_PHY_DSIPHY_CKLN_CFG0_ADDR                                           (DSI_1_PHY_DSIPHY_CKLN_DSIPHY_CKLN_DSI_DATALN_REG_BASE      + 0x00000000)
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_CFG0_RMSK                                                 0x3f
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_CFG0_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CKLN_CFG0_ADDR, HWIO_DSI_1_PHY_DSIPHY_CKLN_CFG0_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_CFG0_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CKLN_CFG0_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_CFG0_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_CKLN_CFG0_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_CFG0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_CKLN_CFG0_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_CKLN_CFG0_IN)
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_CFG0_DSIPHY_HSTX_PGM_DLY_5_4_BMSK                         0x30
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_CFG0_DSIPHY_HSTX_PGM_DLY_5_4_SHFT                          0x4
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_CFG0_DSIPHY_HSTX_PGM_DLY_3_0_BMSK                          0xf
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_CFG0_DSIPHY_HSTX_PGM_DLY_3_0_SHFT                          0x0

#define HWIO_DSI_1_PHY_DSIPHY_CKLN_CFG1_ADDR                                           (DSI_1_PHY_DSIPHY_CKLN_DSIPHY_CKLN_DSI_DATALN_REG_BASE      + 0x00000004)
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_CFG1_RMSK                                                 0x1f
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_CFG1_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CKLN_CFG1_ADDR, HWIO_DSI_1_PHY_DSIPHY_CKLN_CFG1_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_CFG1_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CKLN_CFG1_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_CFG1_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_CKLN_CFG1_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_CFG1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_CKLN_CFG1_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_CKLN_CFG1_IN)
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_CFG1_PROGSEQ_EN_BMSK                                      0x10
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_CFG1_PROGSEQ_EN_SHFT                                       0x4
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_CFG1_DSIPHY_HSTX_SLEW_RATE_BMSK                            0xf
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_CFG1_DSIPHY_HSTX_SLEW_RATE_SHFT                            0x0

#define HWIO_DSI_1_PHY_DSIPHY_CKLN_CFG2_ADDR                                           (DSI_1_PHY_DSIPHY_CKLN_DSIPHY_CKLN_DSI_DATALN_REG_BASE      + 0x00000008)
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_CFG2_RMSK                                                 0xff
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_CFG2_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CKLN_CFG2_ADDR, HWIO_DSI_1_PHY_DSIPHY_CKLN_CFG2_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_CFG2_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CKLN_CFG2_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_CFG2_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_CKLN_CFG2_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_CFG2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_CKLN_CFG2_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_CKLN_CFG2_IN)
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_CFG2_IS_CKLANE_BMSK                                       0x80
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_CFG2_IS_CKLANE_SHFT                                        0x7
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_CFG2_CLK_PATTERN_HSTX_INV_BMSK                            0x40
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_CFG2_CLK_PATTERN_HSTX_INV_SHFT                             0x6
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_CFG2_TEST_MODE_LPTX_DATAN_BMSK                            0x20
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_CFG2_TEST_MODE_LPTX_DATAN_SHFT                             0x5
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_CFG2_TEST_MODE_LPTX_DATAP_BMSK                            0x10
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_CFG2_TEST_MODE_LPTX_DATAP_SHFT                             0x4
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_CFG2_DSIPHY_LPRX_DLY_BMSK                                  0xf
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_CFG2_DSIPHY_LPRX_DLY_SHFT                                  0x0

#define HWIO_DSI_1_PHY_DSIPHY_CKLN_TEST_DATAPATH_ADDR                                  (DSI_1_PHY_DSIPHY_CKLN_DSIPHY_CKLN_DSI_DATALN_REG_BASE      + 0x0000000c)
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_TEST_DATAPATH_RMSK                                        0xff
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_TEST_DATAPATH_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CKLN_TEST_DATAPATH_ADDR, HWIO_DSI_1_PHY_DSIPHY_CKLN_TEST_DATAPATH_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_TEST_DATAPATH_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CKLN_TEST_DATAPATH_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_TEST_DATAPATH_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_CKLN_TEST_DATAPATH_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_TEST_DATAPATH_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_CKLN_TEST_DATAPATH_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_CKLN_TEST_DATAPATH_IN)
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_TEST_DATAPATH_TEST_MODE_LPTX_EN_BMSK                      0x80
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_TEST_DATAPATH_TEST_MODE_LPTX_EN_SHFT                       0x7
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_TEST_DATAPATH_ANE_BYPASS_EN_BMSK                          0x40
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_TEST_DATAPATH_ANE_BYPASS_EN_SHFT                           0x6
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_TEST_DATAPATH_TEST_MODE_HSTX_DATA_BMSK                    0x20
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_TEST_DATAPATH_TEST_MODE_HSTX_DATA_SHFT                     0x5
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_TEST_DATAPATH_DMUX_LP_SEL_BMSK                            0x18
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_TEST_DATAPATH_DMUX_LP_SEL_SHFT                             0x3
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_TEST_DATAPATH_DMUX_HS_SEL_BMSK                             0x4
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_TEST_DATAPATH_DMUX_HS_SEL_SHFT                             0x2
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_TEST_DATAPATH_TEST_MODE_HSTX_EN_BMSK                       0x2
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_TEST_DATAPATH_TEST_MODE_HSTX_EN_SHFT                       0x1
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_TEST_DATAPATH_FORCE_TEST_MODE_BMSK                         0x1
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_TEST_DATAPATH_FORCE_TEST_MODE_SHFT                         0x0

#define HWIO_DSI_1_PHY_DSIPHY_CKLN_PIN_SWAP_ADDR                                       (DSI_1_PHY_DSIPHY_CKLN_DSIPHY_CKLN_DSI_DATALN_REG_BASE      + 0x00000010)
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_PIN_SWAP_RMSK                                              0xf
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_PIN_SWAP_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CKLN_PIN_SWAP_ADDR, HWIO_DSI_1_PHY_DSIPHY_CKLN_PIN_SWAP_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_PIN_SWAP_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CKLN_PIN_SWAP_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_PIN_SWAP_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_CKLN_PIN_SWAP_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_PIN_SWAP_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_CKLN_PIN_SWAP_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_CKLN_PIN_SWAP_IN)
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_PIN_SWAP_ABC_SWAP_BMSK                                     0xe
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_PIN_SWAP_ABC_SWAP_SHFT                                     0x1
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_PIN_SWAP_PN_SWAP_BMSK                                      0x1
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_PIN_SWAP_PN_SWAP_SHFT                                      0x0

#define HWIO_DSI_1_PHY_DSIPHY_CKLN_LPRX_CTRL_ADDR                                      (DSI_1_PHY_DSIPHY_CKLN_DSIPHY_CKLN_DSI_DATALN_REG_BASE      + 0x00000014)
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_LPRX_CTRL_RMSK                                             0x7
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_LPRX_CTRL_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CKLN_LPRX_CTRL_ADDR, HWIO_DSI_1_PHY_DSIPHY_CKLN_LPRX_CTRL_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_LPRX_CTRL_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CKLN_LPRX_CTRL_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_LPRX_CTRL_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_CKLN_LPRX_CTRL_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_LPRX_CTRL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_CKLN_LPRX_CTRL_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_CKLN_LPRX_CTRL_IN)
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_LPRX_CTRL_HS_CONTENTION_DIS_BMSK                           0x4
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_LPRX_CTRL_HS_CONTENTION_DIS_SHFT                           0x2
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_LPRX_CTRL_DSIPHY_LPRX_EN_BMSK                              0x2
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_LPRX_CTRL_DSIPHY_LPRX_EN_SHFT                              0x1
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_LPRX_CTRL_DSIPHY_CDRX_EN_BMSK                              0x1
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_LPRX_CTRL_DSIPHY_CDRX_EN_SHFT                              0x0

#define HWIO_DSI_1_PHY_DSIPHY_CKLN_TX_DCTRL_ADDR                                       (DSI_1_PHY_DSIPHY_CKLN_DSIPHY_CKLN_DSI_DATALN_REG_BASE      + 0x00000018)
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_TX_DCTRL_RMSK                                             0xff
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_TX_DCTRL_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CKLN_TX_DCTRL_ADDR, HWIO_DSI_1_PHY_DSIPHY_CKLN_TX_DCTRL_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_TX_DCTRL_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CKLN_TX_DCTRL_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_TX_DCTRL_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_CKLN_TX_DCTRL_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_TX_DCTRL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_CKLN_TX_DCTRL_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_CKLN_TX_DCTRL_IN)
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_TX_DCTRL_DCTRL_BMSK                                       0xff
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_TX_DCTRL_DCTRL_SHFT                                        0x0

#define HWIO_DSI_1_PHY_DSIPHY_CKLN_BIST_CTRL0_ADDR                                     (DSI_1_PHY_DSIPHY_CKLN_DSIPHY_CKLN_DSI_DATALN_REG_BASE      + 0x0000001c)
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_BIST_CTRL0_RMSK                                           0x3f
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_BIST_CTRL0_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CKLN_BIST_CTRL0_ADDR, HWIO_DSI_1_PHY_DSIPHY_CKLN_BIST_CTRL0_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_BIST_CTRL0_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CKLN_BIST_CTRL0_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_BIST_CTRL0_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_CKLN_BIST_CTRL0_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_BIST_CTRL0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_CKLN_BIST_CTRL0_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_CKLN_BIST_CTRL0_IN)
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_BIST_CTRL0_LP_BISTCHK_ENABLE_BMSK                         0x20
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_BIST_CTRL0_LP_BISTCHK_ENABLE_SHFT                          0x5
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_BIST_CTRL0_BISTCHK_CAPTURE_BMSK                           0x10
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_BIST_CTRL0_BISTCHK_CAPTURE_SHFT                            0x4
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_BIST_CTRL0_BISTCHK_CLR_ERROR_CNT_BMSK                      0x8
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_BIST_CTRL0_BISTCHK_CLR_ERROR_CNT_SHFT                      0x3
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_BIST_CTRL0_BISTCHK_MODE_BMSK                               0x4
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_BIST_CTRL0_BISTCHK_MODE_SHFT                               0x2
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_BIST_CTRL0_BISTCHK_CROSS_BMSK                              0x2
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_BIST_CTRL0_BISTCHK_CROSS_SHFT                              0x1
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_BIST_CTRL0_BISTCHK_ENABLE_BMSK                             0x1
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_BIST_CTRL0_BISTCHK_ENABLE_SHFT                             0x0

#define HWIO_DSI_1_PHY_DSIPHY_CKLN_BIST_CTRL1_ADDR                                     (DSI_1_PHY_DSIPHY_CKLN_DSIPHY_CKLN_DSI_DATALN_REG_BASE      + 0x00000020)
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_BIST_CTRL1_RMSK                                           0xff
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_BIST_CTRL1_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CKLN_BIST_CTRL1_ADDR, HWIO_DSI_1_PHY_DSIPHY_CKLN_BIST_CTRL1_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_BIST_CTRL1_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CKLN_BIST_CTRL1_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_BIST_CTRL1_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_CKLN_BIST_CTRL1_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_BIST_CTRL1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_CKLN_BIST_CTRL1_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_CKLN_BIST_CTRL1_IN)
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_BIST_CTRL1_BIST_WORD_CNT_7_0_BMSK                         0xff
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_BIST_CTRL1_BIST_WORD_CNT_7_0_SHFT                          0x0

#define HWIO_DSI_1_PHY_DSIPHY_CKLN_BIST_CTRL2_ADDR                                     (DSI_1_PHY_DSIPHY_CKLN_DSIPHY_CKLN_DSI_DATALN_REG_BASE      + 0x00000024)
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_BIST_CTRL2_RMSK                                           0xff
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_BIST_CTRL2_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CKLN_BIST_CTRL2_ADDR, HWIO_DSI_1_PHY_DSIPHY_CKLN_BIST_CTRL2_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_BIST_CTRL2_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CKLN_BIST_CTRL2_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_BIST_CTRL2_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_CKLN_BIST_CTRL2_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_BIST_CTRL2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_CKLN_BIST_CTRL2_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_CKLN_BIST_CTRL2_IN)
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_BIST_CTRL2_BIST_WORD_CNT_15_8_BMSK                        0xff
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_BIST_CTRL2_BIST_WORD_CNT_15_8_SHFT                         0x0

#define HWIO_DSI_1_PHY_DSIPHY_CKLN_MISR_CTRL_ADDR                                      (DSI_1_PHY_DSIPHY_CKLN_DSIPHY_CKLN_DSI_DATALN_REG_BASE      + 0x00000028)
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_MISR_CTRL_RMSK                                            0x1f
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_MISR_CTRL_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CKLN_MISR_CTRL_ADDR, HWIO_DSI_1_PHY_DSIPHY_CKLN_MISR_CTRL_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_MISR_CTRL_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CKLN_MISR_CTRL_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_MISR_CTRL_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_CKLN_MISR_CTRL_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_MISR_CTRL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_CKLN_MISR_CTRL_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_CKLN_MISR_CTRL_IN)
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_MISR_CTRL_DEBUG_CLK_DEBUG_SEL_BMSK                        0x1c
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_MISR_CTRL_DEBUG_CLK_DEBUG_SEL_SHFT                         0x2
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_MISR_CTRL_MISR_SIG_CLEAR_BMSK                              0x2
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_MISR_CTRL_MISR_SIG_CLEAR_SHFT                              0x1
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_MISR_CTRL_MISR_ENABLE_BMSK                                 0x1
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_MISR_CTRL_MISR_ENABLE_SHFT                                 0x0

#define HWIO_DSI_1_PHY_DSIPHY_CKLN_DBG_SEL_ADDR                                        (DSI_1_PHY_DSIPHY_CKLN_DSIPHY_CKLN_DSI_DATALN_REG_BASE      + 0x0000002c)
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_DBG_SEL_RMSK                                              0x7f
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_DBG_SEL_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CKLN_DBG_SEL_ADDR, HWIO_DSI_1_PHY_DSIPHY_CKLN_DBG_SEL_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_DBG_SEL_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CKLN_DBG_SEL_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_DBG_SEL_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_CKLN_DBG_SEL_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_DBG_SEL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_CKLN_DBG_SEL_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_CKLN_DBG_SEL_IN)
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_DBG_SEL_TEST_CLK_DEBUG_SEL_BMSK                           0x70
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_DBG_SEL_TEST_CLK_DEBUG_SEL_SHFT                            0x4
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_DBG_SEL_DEBUG_SEL_BMSK                                     0xf
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_DBG_SEL_DEBUG_SEL_SHFT                                     0x0

#define HWIO_DSI_1_PHY_DSIPHY_CKLN_SWI_DBG_BUS_CHECK_ADDR                              (DSI_1_PHY_DSIPHY_CKLN_DSIPHY_CKLN_DSI_DATALN_REG_BASE      + 0x00000030)
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_SWI_DBG_BUS_CHECK_RMSK                                    0xff
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_SWI_DBG_BUS_CHECK_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CKLN_SWI_DBG_BUS_CHECK_ADDR, HWIO_DSI_1_PHY_DSIPHY_CKLN_SWI_DBG_BUS_CHECK_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_SWI_DBG_BUS_CHECK_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CKLN_SWI_DBG_BUS_CHECK_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_SWI_DBG_BUS_CHECK_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_CKLN_SWI_DBG_BUS_CHECK_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_SWI_DBG_BUS_CHECK_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_CKLN_SWI_DBG_BUS_CHECK_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_CKLN_SWI_DBG_BUS_CHECK_IN)
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_SWI_DBG_BUS_CHECK_LANE_DEBUG_BUS_BMSK                     0xff
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_SWI_DBG_BUS_CHECK_LANE_DEBUG_BUS_SHFT                      0x0

#define HWIO_DSI_1_PHY_DSIPHY_CKLN_BIST_STATUS0_ADDR                                   (DSI_1_PHY_DSIPHY_CKLN_DSIPHY_CKLN_DSI_DATALN_REG_BASE      + 0x00000034)
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_BIST_STATUS0_RMSK                                         0xff
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_BIST_STATUS0_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CKLN_BIST_STATUS0_ADDR, HWIO_DSI_1_PHY_DSIPHY_CKLN_BIST_STATUS0_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_BIST_STATUS0_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CKLN_BIST_STATUS0_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_BIST_STATUS0_BISTCHK_ONE_BIT_ERROR_BMSK                   0x80
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_BIST_STATUS0_BISTCHK_ONE_BIT_ERROR_SHFT                    0x7
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_BIST_STATUS0_BISTCHK_SYNC_ERROR_BMSK                      0x40
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_BIST_STATUS0_BISTCHK_SYNC_ERROR_SHFT                       0x6
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_BIST_STATUS0_BISTCHK_SEED_ERROR_BMSK                      0x20
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_BIST_STATUS0_BISTCHK_SEED_ERROR_SHFT                       0x5
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_BIST_STATUS0_BISTCHK_PRBS_ERROR_BMSK                      0x10
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_BIST_STATUS0_BISTCHK_PRBS_ERROR_SHFT                       0x4
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_BIST_STATUS0_BISTCHK_STATUS_BMSK                           0xe
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_BIST_STATUS0_BISTCHK_STATUS_SHFT                           0x1
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_BIST_STATUS0_BISTCHK_DONE_BMSK                             0x1
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_BIST_STATUS0_BISTCHK_DONE_SHFT                             0x0

#define HWIO_DSI_1_PHY_DSIPHY_CKLN_BIST_STATUS1_ADDR                                   (DSI_1_PHY_DSIPHY_CKLN_DSIPHY_CKLN_DSI_DATALN_REG_BASE      + 0x00000038)
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_BIST_STATUS1_RMSK                                         0xff
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_BIST_STATUS1_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CKLN_BIST_STATUS1_ADDR, HWIO_DSI_1_PHY_DSIPHY_CKLN_BIST_STATUS1_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_BIST_STATUS1_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CKLN_BIST_STATUS1_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_BIST_STATUS1_BISTCHK_ERROR_CNT_7_0_BMSK                   0xff
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_BIST_STATUS1_BISTCHK_ERROR_CNT_7_0_SHFT                    0x0

#define HWIO_DSI_1_PHY_DSIPHY_CKLN_BIST_STATUS2_ADDR                                   (DSI_1_PHY_DSIPHY_CKLN_DSIPHY_CKLN_DSI_DATALN_REG_BASE      + 0x0000003c)
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_BIST_STATUS2_RMSK                                         0xff
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_BIST_STATUS2_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CKLN_BIST_STATUS2_ADDR, HWIO_DSI_1_PHY_DSIPHY_CKLN_BIST_STATUS2_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_BIST_STATUS2_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CKLN_BIST_STATUS2_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_BIST_STATUS2_BISTCHK_ERROR_CNT_15_8_BMSK                  0xff
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_BIST_STATUS2_BISTCHK_ERROR_CNT_15_8_SHFT                   0x0

#define HWIO_DSI_1_PHY_DSIPHY_CKLN_BIST_STATUS3_ADDR                                   (DSI_1_PHY_DSIPHY_CKLN_DSIPHY_CKLN_DSI_DATALN_REG_BASE      + 0x00000040)
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_BIST_STATUS3_RMSK                                         0xff
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_BIST_STATUS3_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CKLN_BIST_STATUS3_ADDR, HWIO_DSI_1_PHY_DSIPHY_CKLN_BIST_STATUS3_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_BIST_STATUS3_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CKLN_BIST_STATUS3_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_BIST_STATUS3_BISTCHK_WORD_CNT_7_0_BMSK                    0xff
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_BIST_STATUS3_BISTCHK_WORD_CNT_7_0_SHFT                     0x0

#define HWIO_DSI_1_PHY_DSIPHY_CKLN_BIST_STATUS4_ADDR                                   (DSI_1_PHY_DSIPHY_CKLN_DSIPHY_CKLN_DSI_DATALN_REG_BASE      + 0x00000044)
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_BIST_STATUS4_RMSK                                         0xff
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_BIST_STATUS4_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CKLN_BIST_STATUS4_ADDR, HWIO_DSI_1_PHY_DSIPHY_CKLN_BIST_STATUS4_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_BIST_STATUS4_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CKLN_BIST_STATUS4_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_BIST_STATUS4_BISTCHK_WORD_CNT_15_8_BMSK                   0xff
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_BIST_STATUS4_BISTCHK_WORD_CNT_15_8_SHFT                    0x0

#define HWIO_DSI_1_PHY_DSIPHY_CKLN_LP_BIST_STATUS0_ADDR                                (DSI_1_PHY_DSIPHY_CKLN_DSIPHY_CKLN_DSI_DATALN_REG_BASE      + 0x00000048)
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_LP_BIST_STATUS0_RMSK                                       0x3
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_LP_BIST_STATUS0_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CKLN_LP_BIST_STATUS0_ADDR, HWIO_DSI_1_PHY_DSIPHY_CKLN_LP_BIST_STATUS0_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_LP_BIST_STATUS0_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CKLN_LP_BIST_STATUS0_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_LP_BIST_STATUS0_LP_BISTCHK_PRBS_ERROR_BMSK                 0x2
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_LP_BIST_STATUS0_LP_BISTCHK_PRBS_ERROR_SHFT                 0x1
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_LP_BIST_STATUS0_LP_BISTCHK_DONE_BMSK                       0x1
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_LP_BIST_STATUS0_LP_BISTCHK_DONE_SHFT                       0x0

#define HWIO_DSI_1_PHY_DSIPHY_CKLN_LP_BIST_STATUS1_ADDR                                (DSI_1_PHY_DSIPHY_CKLN_DSIPHY_CKLN_DSI_DATALN_REG_BASE      + 0x0000004c)
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_LP_BIST_STATUS1_RMSK                                      0xff
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_LP_BIST_STATUS1_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CKLN_LP_BIST_STATUS1_ADDR, HWIO_DSI_1_PHY_DSIPHY_CKLN_LP_BIST_STATUS1_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_LP_BIST_STATUS1_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CKLN_LP_BIST_STATUS1_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_LP_BIST_STATUS1_LP_BISTCHK_ERROR_CNT_BMSK                 0xff
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_LP_BIST_STATUS1_LP_BISTCHK_ERROR_CNT_SHFT                  0x0

#define HWIO_DSI_1_PHY_DSIPHY_CKLN_LP_BIST_STATUS2_ADDR                                (DSI_1_PHY_DSIPHY_CKLN_DSIPHY_CKLN_DSI_DATALN_REG_BASE      + 0x00000050)
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_LP_BIST_STATUS2_RMSK                                      0xff
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_LP_BIST_STATUS2_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CKLN_LP_BIST_STATUS2_ADDR, HWIO_DSI_1_PHY_DSIPHY_CKLN_LP_BIST_STATUS2_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_LP_BIST_STATUS2_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CKLN_LP_BIST_STATUS2_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_LP_BIST_STATUS2_LP_BISTCHK_WORD_CNT_BMSK                  0xff
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_LP_BIST_STATUS2_LP_BISTCHK_WORD_CNT_SHFT                   0x0

#define HWIO_DSI_1_PHY_DSIPHY_CKLN_MISR_STATUS0_ADDR                                   (DSI_1_PHY_DSIPHY_CKLN_DSIPHY_CKLN_DSI_DATALN_REG_BASE      + 0x00000054)
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_MISR_STATUS0_RMSK                                         0xff
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_MISR_STATUS0_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CKLN_MISR_STATUS0_ADDR, HWIO_DSI_1_PHY_DSIPHY_CKLN_MISR_STATUS0_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_MISR_STATUS0_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CKLN_MISR_STATUS0_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_MISR_STATUS0_BISTCHK_MISR_SIG_7_0_BMSK                    0xff
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_MISR_STATUS0_BISTCHK_MISR_SIG_7_0_SHFT                     0x0

#define HWIO_DSI_1_PHY_DSIPHY_CKLN_MISR_STATUS1_ADDR                                   (DSI_1_PHY_DSIPHY_CKLN_DSIPHY_CKLN_DSI_DATALN_REG_BASE      + 0x00000058)
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_MISR_STATUS1_RMSK                                         0xff
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_MISR_STATUS1_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CKLN_MISR_STATUS1_ADDR, HWIO_DSI_1_PHY_DSIPHY_CKLN_MISR_STATUS1_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_MISR_STATUS1_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CKLN_MISR_STATUS1_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_MISR_STATUS1_BISTCHK_MISR_SIG_15_8_BMSK                   0xff
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_MISR_STATUS1_BISTCHK_MISR_SIG_15_8_SHFT                    0x0

#define HWIO_DSI_1_PHY_DSIPHY_CKLN_DEBUG_BUS0_ADDR                                     (DSI_1_PHY_DSIPHY_CKLN_DSIPHY_CKLN_DSI_DATALN_REG_BASE      + 0x0000005c)
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_DEBUG_BUS0_RMSK                                           0xff
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_DEBUG_BUS0_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CKLN_DEBUG_BUS0_ADDR, HWIO_DSI_1_PHY_DSIPHY_CKLN_DEBUG_BUS0_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_DEBUG_BUS0_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CKLN_DEBUG_BUS0_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_DEBUG_BUS0_DEBUG_BUS_7_0_BMSK                             0xff
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_DEBUG_BUS0_DEBUG_BUS_7_0_SHFT                              0x0

#define HWIO_DSI_1_PHY_DSIPHY_CKLN_DEBUG_BUS1_ADDR                                     (DSI_1_PHY_DSIPHY_CKLN_DSIPHY_CKLN_DSI_DATALN_REG_BASE      + 0x00000060)
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_DEBUG_BUS1_RMSK                                           0xff
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_DEBUG_BUS1_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CKLN_DEBUG_BUS1_ADDR, HWIO_DSI_1_PHY_DSIPHY_CKLN_DEBUG_BUS1_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_DEBUG_BUS1_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CKLN_DEBUG_BUS1_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_DEBUG_BUS1_DEBUG_BUS_15_8_BMSK                            0xff
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_DEBUG_BUS1_DEBUG_BUS_15_8_SHFT                             0x0

#define HWIO_DSI_1_PHY_DSIPHY_CKLN_DEBUG_BUS2_ADDR                                     (DSI_1_PHY_DSIPHY_CKLN_DSIPHY_CKLN_DSI_DATALN_REG_BASE      + 0x00000064)
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_DEBUG_BUS2_RMSK                                           0xff
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_DEBUG_BUS2_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CKLN_DEBUG_BUS2_ADDR, HWIO_DSI_1_PHY_DSIPHY_CKLN_DEBUG_BUS2_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_DEBUG_BUS2_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CKLN_DEBUG_BUS2_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_DEBUG_BUS2_DEBUG_BUS_23_16_BMSK                           0xff
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_DEBUG_BUS2_DEBUG_BUS_23_16_SHFT                            0x0

#define HWIO_DSI_1_PHY_DSIPHY_CKLN_DEBUG_BUS3_ADDR                                     (DSI_1_PHY_DSIPHY_CKLN_DSIPHY_CKLN_DSI_DATALN_REG_BASE      + 0x00000068)
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_DEBUG_BUS3_RMSK                                           0xff
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_DEBUG_BUS3_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CKLN_DEBUG_BUS3_ADDR, HWIO_DSI_1_PHY_DSIPHY_CKLN_DEBUG_BUS3_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_DEBUG_BUS3_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_CKLN_DEBUG_BUS3_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_DEBUG_BUS3_DEBUG_BUS_31_24_BMSK                           0xff
#define HWIO_DSI_1_PHY_DSIPHY_CKLN_DEBUG_BUS3_DEBUG_BUS_31_24_SHFT                            0x0

/*----------------------------------------------------------------------------
 * MODULE: DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL
 *--------------------------------------------------------------------------*/

#define DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE                                            (MDSS_BASE      + 0x00096900)

#define HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_ONE_ADDR                                               (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000000)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_ONE_RMSK                                                     0x7f
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_ONE_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_ONE_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_ONE_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_ONE_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_ONE_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_ONE_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_ONE_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_ONE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_ONE_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_ONE_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_ONE_DISABLE_CLKGATE_BMSK                                     0x40
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_ONE_DISABLE_CLKGATE_SHFT                                      0x6
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_ONE_CM_PLL_LOCK_BYP_BMSK                                     0x20
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_ONE_CM_PLL_LOCK_BYP_SHFT                                      0x5
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_ONE_CM_PLL_BYPASS_BMSK                                       0x10
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_ONE_CM_PLL_BYPASS_SHFT                                        0x4
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_ONE_CM_PLL_RST_MUX_BMSK                                       0x8
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_ONE_CM_PLL_RST_MUX_SHFT                                       0x3
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_ONE_CM_PLL_RST_BMSK                                           0x4
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_ONE_CM_PLL_RST_SHFT                                           0x2
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_ONE_CM_PLL_PWRDN_MUX_BMSK                                     0x2
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_ONE_CM_PLL_PWRDN_MUX_SHFT                                     0x1
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_ONE_CM_PLL_PWRDN_BMSK                                         0x1
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_ONE_CM_PLL_PWRDN_SHFT                                         0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_TWO_ADDR                                               (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000004)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_TWO_RMSK                                                      0xf
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_TWO_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_TWO_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_TWO_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_TWO_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_TWO_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_TWO_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_TWO_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_TWO_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_TWO_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_TWO_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_TWO_PWRDN_RESET_MUX_BMSK                                      0x8
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_TWO_PWRDN_RESET_MUX_SHFT                                      0x3
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_TWO_PWRDN_RESET_BMSK                                          0x4
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_TWO_PWRDN_RESET_SHFT                                          0x2
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_TWO_PWRDN_B_BIAS_BMSK                                         0x2
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_TWO_PWRDN_B_BIAS_SHFT                                         0x1
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_TWO_PWRDN_B_BMSK                                              0x1
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_TWO_PWRDN_B_SHFT                                              0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_INT_LOOP_SETTINGS_ADDR                                                 (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000008)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_INT_LOOP_SETTINGS_RMSK                                                       0x7f
#define HWIO_DSI_1_PHY_DSIPHY_PLL_INT_LOOP_SETTINGS_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_INT_LOOP_SETTINGS_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_INT_LOOP_SETTINGS_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_INT_LOOP_SETTINGS_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_INT_LOOP_SETTINGS_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_INT_LOOP_SETTINGS_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_INT_LOOP_SETTINGS_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_INT_LOOP_SETTINGS_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_INT_LOOP_SETTINGS_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_INT_LOOP_SETTINGS_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_INT_LOOP_SETTINGS_QIL_POLARITY_BMSK                                          0x40
#define HWIO_DSI_1_PHY_DSIPHY_PLL_INT_LOOP_SETTINGS_QIL_POLARITY_SHFT                                           0x6
#define HWIO_DSI_1_PHY_DSIPHY_PLL_INT_LOOP_SETTINGS_LFSR_SEED_BMSK                                             0x3f
#define HWIO_DSI_1_PHY_DSIPHY_PLL_INT_LOOP_SETTINGS_LFSR_SEED_SHFT                                              0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_INT_LOOP_SETTINGS_TWO_ADDR                                             (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x0000000c)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_INT_LOOP_SETTINGS_TWO_RMSK                                                   0x1f
#define HWIO_DSI_1_PHY_DSIPHY_PLL_INT_LOOP_SETTINGS_TWO_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_INT_LOOP_SETTINGS_TWO_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_INT_LOOP_SETTINGS_TWO_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_INT_LOOP_SETTINGS_TWO_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_INT_LOOP_SETTINGS_TWO_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_INT_LOOP_SETTINGS_TWO_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_INT_LOOP_SETTINGS_TWO_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_INT_LOOP_SETTINGS_TWO_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_INT_LOOP_SETTINGS_TWO_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_INT_LOOP_SETTINGS_TWO_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_INT_LOOP_SETTINGS_TWO_LFSR_EN_BMSK                                           0x10
#define HWIO_DSI_1_PHY_DSIPHY_PLL_INT_LOOP_SETTINGS_TWO_LFSR_EN_SHFT                                            0x4
#define HWIO_DSI_1_PHY_DSIPHY_PLL_INT_LOOP_SETTINGS_TWO_SECOND_ORDER_EN_BMSK                                    0x8
#define HWIO_DSI_1_PHY_DSIPHY_PLL_INT_LOOP_SETTINGS_TWO_SECOND_ORDER_EN_SHFT                                    0x3
#define HWIO_DSI_1_PHY_DSIPHY_PLL_INT_LOOP_SETTINGS_TWO_QIL_DEC_MUX_BMSK                                        0x4
#define HWIO_DSI_1_PHY_DSIPHY_PLL_INT_LOOP_SETTINGS_TWO_QIL_DEC_MUX_SHFT                                        0x2
#define HWIO_DSI_1_PHY_DSIPHY_PLL_INT_LOOP_SETTINGS_TWO_QIL_DEC_BMSK                                            0x3
#define HWIO_DSI_1_PHY_DSIPHY_PLL_INT_LOOP_SETTINGS_TWO_QIL_DEC_SHFT                                            0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_THREE_ADDR                                             (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000010)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_THREE_RMSK                                                   0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_THREE_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_THREE_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_THREE_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_THREE_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_THREE_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_THREE_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_THREE_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_THREE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_THREE_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_THREE_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_THREE_CM_PLL_BAND_SEL_BMSK                                   0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_THREE_CM_PLL_BAND_SEL_SHFT                                    0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FOUR_ADDR                                              (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000014)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FOUR_RMSK                                                    0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FOUR_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FOUR_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FOUR_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FOUR_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FOUR_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FOUR_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FOUR_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FOUR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FOUR_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FOUR_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FOUR_CM_PLL_ATB_SEL_BMSK                                     0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FOUR_CM_PLL_ATB_SEL_SHFT                                      0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FIVE_ADDR                                              (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000018)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FIVE_RMSK                                                     0x3
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FIVE_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FIVE_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FIVE_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FIVE_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FIVE_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FIVE_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FIVE_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FIVE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FIVE_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FIVE_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FIVE_CM_VCOCLK_DIV2EN_MSB_BMSK                                0x2
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FIVE_CM_VCOCLK_DIV2EN_MSB_SHFT                                0x1
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FIVE_CM_VCOCLK_DIV2EN_BMSK                                    0x1
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FIVE_CM_VCOCLK_DIV2EN_SHFT                                    0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_INT_LOOP_CONTROLS_ADDR                                                 (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x0000001c)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_INT_LOOP_CONTROLS_RMSK                                                       0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_INT_LOOP_CONTROLS_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_INT_LOOP_CONTROLS_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_INT_LOOP_CONTROLS_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_INT_LOOP_CONTROLS_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_INT_LOOP_CONTROLS_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_INT_LOOP_CONTROLS_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_INT_LOOP_CONTROLS_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_INT_LOOP_CONTROLS_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_INT_LOOP_CONTROLS_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_INT_LOOP_CONTROLS_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_INT_LOOP_CONTROLS_BYP_OUTPUT_REGS_BMSK                                       0x80
#define HWIO_DSI_1_PHY_DSIPHY_PLL_INT_LOOP_CONTROLS_BYP_OUTPUT_REGS_SHFT                                        0x7
#define HWIO_DSI_1_PHY_DSIPHY_PLL_INT_LOOP_CONTROLS_AVG_DEC_BMSK                                               0x40
#define HWIO_DSI_1_PHY_DSIPHY_PLL_INT_LOOP_CONTROLS_AVG_DEC_SHFT                                                0x6
#define HWIO_DSI_1_PHY_DSIPHY_PLL_INT_LOOP_CONTROLS_TDC_DECIMATE_BMSK                                          0x38
#define HWIO_DSI_1_PHY_DSIPHY_PLL_INT_LOOP_CONTROLS_TDC_DECIMATE_SHFT                                           0x3
#define HWIO_DSI_1_PHY_DSIPHY_PLL_INT_LOOP_CONTROLS_TDC_CLEAR_MUX_BMSK                                          0x4
#define HWIO_DSI_1_PHY_DSIPHY_PLL_INT_LOOP_CONTROLS_TDC_CLEAR_MUX_SHFT                                          0x2
#define HWIO_DSI_1_PHY_DSIPHY_PLL_INT_LOOP_CONTROLS_TDC_CLEAR_BMSK                                              0x2
#define HWIO_DSI_1_PHY_DSIPHY_PLL_INT_LOOP_CONTROLS_TDC_CLEAR_SHFT                                              0x1
#define HWIO_DSI_1_PHY_DSIPHY_PLL_INT_LOOP_CONTROLS_TDC_POLARITY_BMSK                                           0x1
#define HWIO_DSI_1_PHY_DSIPHY_PLL_INT_LOOP_CONTROLS_TDC_POLARITY_SHFT                                           0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_DSM_DIVIDER_ADDR                                                       (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000020)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_DSM_DIVIDER_RMSK                                                             0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_DSM_DIVIDER_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_DSM_DIVIDER_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_DSM_DIVIDER_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_DSM_DIVIDER_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_DSM_DIVIDER_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_DSM_DIVIDER_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_DSM_DIVIDER_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_DSM_DIVIDER_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_DSM_DIVIDER_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_DSM_DIVIDER_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_DSM_DIVIDER_CM_PLL_DSMDIV_BMSK                                               0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_DSM_DIVIDER_CM_PLL_DSMDIV_SHFT                                                0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_FEEDBACK_DIVIDER_ADDR                                                  (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000024)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FEEDBACK_DIVIDER_RMSK                                                        0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FEEDBACK_DIVIDER_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_FEEDBACK_DIVIDER_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_FEEDBACK_DIVIDER_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FEEDBACK_DIVIDER_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_FEEDBACK_DIVIDER_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FEEDBACK_DIVIDER_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_FEEDBACK_DIVIDER_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FEEDBACK_DIVIDER_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_FEEDBACK_DIVIDER_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_FEEDBACK_DIVIDER_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FEEDBACK_DIVIDER_CM_PLL_FBDIV_BMSK                                           0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FEEDBACK_DIVIDER_CM_PLL_FBDIV_SHFT                                            0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_SYSTEM_MUXES_ADDR                                                      (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000028)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SYSTEM_MUXES_RMSK                                                            0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SYSTEM_MUXES_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_SYSTEM_MUXES_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_SYSTEM_MUXES_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SYSTEM_MUXES_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_SYSTEM_MUXES_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SYSTEM_MUXES_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_SYSTEM_MUXES_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SYSTEM_MUXES_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_SYSTEM_MUXES_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_SYSTEM_MUXES_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SYSTEM_MUXES_CM_BIAS_EN_MUX_BMSK                                             0x80
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SYSTEM_MUXES_CM_BIAS_EN_MUX_SHFT                                              0x7
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SYSTEM_MUXES_CM_BIAS_EN_BMSK                                                 0x40
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SYSTEM_MUXES_CM_BIAS_EN_SHFT                                                  0x6
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SYSTEM_MUXES_CM_PLL_BAND_SEL_MUX_BMSK                                        0x20
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SYSTEM_MUXES_CM_PLL_BAND_SEL_MUX_SHFT                                         0x5
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SYSTEM_MUXES_CM_PLL_LOCKDET_MUX_BMSK                                         0x10
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SYSTEM_MUXES_CM_PLL_LOCKDET_MUX_SHFT                                          0x4
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SYSTEM_MUXES_CM_PLL_PROP_GAIN_MUX_BMSK                                        0x8
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SYSTEM_MUXES_CM_PLL_PROP_GAIN_MUX_SHFT                                        0x3
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SYSTEM_MUXES_CM_PLL_INT_GAIN_MUX_BMSK                                         0x4
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SYSTEM_MUXES_CM_PLL_INT_GAIN_MUX_SHFT                                         0x2
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SYSTEM_MUXES_CM_PLL_FBDIV_MUX_BMSK                                            0x2
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SYSTEM_MUXES_CM_PLL_FBDIV_MUX_SHFT                                            0x1
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SYSTEM_MUXES_CM_PLL_DSMDIV_MUX_BMSK                                           0x1
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SYSTEM_MUXES_CM_PLL_DSMDIV_MUX_SHFT                                           0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_UPDATE_CONTROL_OVERRIDES_ADDR                                     (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x0000002c)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_UPDATE_CONTROL_OVERRIDES_RMSK                                           0x3f
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_UPDATE_CONTROL_OVERRIDES_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_UPDATE_CONTROL_OVERRIDES_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_UPDATE_CONTROL_OVERRIDES_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_UPDATE_CONTROL_OVERRIDES_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_UPDATE_CONTROL_OVERRIDES_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_UPDATE_CONTROL_OVERRIDES_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_UPDATE_CONTROL_OVERRIDES_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_UPDATE_CONTROL_OVERRIDES_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_UPDATE_CONTROL_OVERRIDES_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_UPDATE_CONTROL_OVERRIDES_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_UPDATE_CONTROL_OVERRIDES_HOLD_SSC_OUTPUT_MUX_BMSK                       0x20
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_UPDATE_CONTROL_OVERRIDES_HOLD_SSC_OUTPUT_MUX_SHFT                        0x5
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_UPDATE_CONTROL_OVERRIDES_HOLD_SSC_OUTPUT_BMSK                           0x10
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_UPDATE_CONTROL_OVERRIDES_HOLD_SSC_OUTPUT_SHFT                            0x4
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_UPDATE_CONTROL_OVERRIDES_UPDATE_LOOP_MUX_BMSK                            0x8
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_UPDATE_CONTROL_OVERRIDES_UPDATE_LOOP_MUX_SHFT                            0x3
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_UPDATE_CONTROL_OVERRIDES_UPDATE_LOOP_BMSK                                0x4
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_UPDATE_CONTROL_OVERRIDES_UPDATE_LOOP_SHFT                                0x2
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_UPDATE_CONTROL_OVERRIDES_UPDATE_PARAMS_MUX_BMSK                          0x2
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_UPDATE_CONTROL_OVERRIDES_UPDATE_PARAMS_MUX_SHFT                          0x1
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_UPDATE_CONTROL_OVERRIDES_UPDATE_PARAMS_BMSK                              0x1
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_UPDATE_CONTROL_OVERRIDES_UPDATE_PARAMS_SHFT                              0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_CMODE_ADDR                                                             (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000030)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CMODE_RMSK                                                                   0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CMODE_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_CMODE_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_CMODE_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CMODE_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_CMODE_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CMODE_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_CMODE_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CMODE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_CMODE_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_CMODE_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CMODE_CM_CMODE_BMSK                                                          0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CMODE_CM_CMODE_SHFT                                                           0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_PSM_CTRL_ADDR                                                          (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000034)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PSM_CTRL_RMSK                                                                0x7f
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PSM_CTRL_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_PSM_CTRL_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_PSM_CTRL_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PSM_CTRL_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_PSM_CTRL_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PSM_CTRL_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_PSM_CTRL_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PSM_CTRL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_PSM_CTRL_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_PSM_CTRL_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PSM_CTRL_EN_PLL_LOCK_MUX_BMSK                                                0x40
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PSM_CTRL_EN_PLL_LOCK_MUX_SHFT                                                 0x6
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PSM_CTRL_EN_PLL_LOCK_BMSK                                                    0x20
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PSM_CTRL_EN_PLL_LOCK_SHFT                                                     0x5
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PSM_CTRL_PWR_STATE_BMSK                                                      0x1c
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PSM_CTRL_PWR_STATE_SHFT                                                       0x2
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PSM_CTRL_PWR_STATE_REQ_MUX_BMSK                                               0x2
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PSM_CTRL_PWR_STATE_REQ_MUX_SHFT                                               0x1
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PSM_CTRL_PWR_STATE_REQ_BMSK                                                   0x1
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PSM_CTRL_PWR_STATE_REQ_SHFT                                                   0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_RSM_CTRL_ADDR                                                          (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000038)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_RSM_CTRL_RMSK                                                                0x1f
#define HWIO_DSI_1_PHY_DSIPHY_PLL_RSM_CTRL_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_RSM_CTRL_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_RSM_CTRL_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_RSM_CTRL_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_RSM_CTRL_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_RSM_CTRL_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_RSM_CTRL_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_RSM_CTRL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_RSM_CTRL_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_RSM_CTRL_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_RSM_CTRL_RSM_LEGACY_MODE_BMSK                                                0x10
#define HWIO_DSI_1_PHY_DSIPHY_PLL_RSM_CTRL_RSM_LEGACY_MODE_SHFT                                                 0x4
#define HWIO_DSI_1_PHY_DSIPHY_PLL_RSM_CTRL_BYPASS_BS_MODE_BMSK                                                  0x8
#define HWIO_DSI_1_PHY_DSIPHY_PLL_RSM_CTRL_BYPASS_BS_MODE_SHFT                                                  0x3
#define HWIO_DSI_1_PHY_DSIPHY_PLL_RSM_CTRL_BYPASS_RSM_PLLLOCK_BMSK                                              0x4
#define HWIO_DSI_1_PHY_DSIPHY_PLL_RSM_CTRL_BYPASS_RSM_PLLLOCK_SHFT                                              0x2
#define HWIO_DSI_1_PHY_DSIPHY_PLL_RSM_CTRL_BYPASS_RSM_BIN_FGCAL_BMSK                                            0x2
#define HWIO_DSI_1_PHY_DSIPHY_PLL_RSM_CTRL_BYPASS_RSM_BIN_FGCAL_SHFT                                            0x1
#define HWIO_DSI_1_PHY_DSIPHY_PLL_RSM_CTRL_BYPASS_RSM_BIAS_BMSK                                                 0x1
#define HWIO_DSI_1_PHY_DSIPHY_PLL_RSM_CTRL_BYPASS_RSM_BIAS_SHFT                                                 0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_VCO_TUNE_MAP_ADDR                                                      (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x0000003c)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_VCO_TUNE_MAP_RMSK                                                             0x7
#define HWIO_DSI_1_PHY_DSIPHY_PLL_VCO_TUNE_MAP_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_VCO_TUNE_MAP_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_VCO_TUNE_MAP_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_VCO_TUNE_MAP_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_VCO_TUNE_MAP_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_VCO_TUNE_MAP_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_VCO_TUNE_MAP_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_VCO_TUNE_MAP_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_VCO_TUNE_MAP_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_VCO_TUNE_MAP_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_VCO_TUNE_MAP_DEFAULT_FREQ_MODE_BMSK                                           0x4
#define HWIO_DSI_1_PHY_DSIPHY_PLL_VCO_TUNE_MAP_DEFAULT_FREQ_MODE_SHFT                                           0x2
#define HWIO_DSI_1_PHY_DSIPHY_PLL_VCO_TUNE_MAP_PS1_FREQ_MODE_BMSK                                               0x2
#define HWIO_DSI_1_PHY_DSIPHY_PLL_VCO_TUNE_MAP_PS1_FREQ_MODE_SHFT                                               0x1
#define HWIO_DSI_1_PHY_DSIPHY_PLL_VCO_TUNE_MAP_PS0_FREQ_MODE_BMSK                                               0x1
#define HWIO_DSI_1_PHY_DSIPHY_PLL_VCO_TUNE_MAP_PS0_FREQ_MODE_SHFT                                               0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_CNTRL_ADDR                                                         (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000040)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_CNTRL_RMSK                                                               0x7f
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_CNTRL_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_CNTRL_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_CNTRL_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_CNTRL_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_CNTRL_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_CNTRL_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_CNTRL_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_CNTRL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_CNTRL_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_CNTRL_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_CNTRL_DONT_SKIP_WAIT_STATE_BMSK                                          0x40
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_CNTRL_DONT_SKIP_WAIT_STATE_SHFT                                           0x6
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_CNTRL_ROUND_UP_ENABLE_BMSK                                               0x20
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_CNTRL_ROUND_UP_ENABLE_SHFT                                                0x5
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_CNTRL_LOCKDET_CDC_FIX2_ENABLE_BMSK                                       0x10
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_CNTRL_LOCKDET_CDC_FIX2_ENABLE_SHFT                                        0x4
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_CNTRL_LOCKDET_CDC_FIX_ENABLE_BMSK                                         0x8
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_CNTRL_LOCKDET_CDC_FIX_ENABLE_SHFT                                         0x3
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_CNTRL_FIX_ENABLE_BMSK                                                     0x4
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_CNTRL_FIX_ENABLE_SHFT                                                     0x2
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_CNTRL_PSM_EN_PLL_MUX_BMSK                                                 0x2
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_CNTRL_PSM_EN_PLL_MUX_SHFT                                                 0x1
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_CNTRL_PSM_EN_PLL_BMSK                                                     0x1
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_CNTRL_PSM_EN_PLL_SHFT                                                     0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_CALIBRATION_SETTINGS_ADDR                                              (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000044)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CALIBRATION_SETTINGS_RMSK                                                    0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CALIBRATION_SETTINGS_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_CALIBRATION_SETTINGS_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_CALIBRATION_SETTINGS_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CALIBRATION_SETTINGS_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_CALIBRATION_SETTINGS_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CALIBRATION_SETTINGS_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_CALIBRATION_SETTINGS_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CALIBRATION_SETTINGS_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_CALIBRATION_SETTINGS_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_CALIBRATION_SETTINGS_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CALIBRATION_SETTINGS_BS_UPPER_MUX_BMSK                                       0x80
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CALIBRATION_SETTINGS_BS_UPPER_MUX_SHFT                                        0x7
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CALIBRATION_SETTINGS_BS_UPPER_BMSK                                           0x40
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CALIBRATION_SETTINGS_BS_UPPER_SHFT                                            0x6
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CALIBRATION_SETTINGS_USE_BAND_SEL_TIMER_BMSK                                 0x20
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CALIBRATION_SETTINGS_USE_BAND_SEL_TIMER_SHFT                                  0x5
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CALIBRATION_SETTINGS_BAND_SEL_CAL_EN_MUX_BMSK                                0x10
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CALIBRATION_SETTINGS_BAND_SEL_CAL_EN_MUX_SHFT                                 0x4
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CALIBRATION_SETTINGS_BAND_SEL_CAL_EN_BMSK                                     0x8
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CALIBRATION_SETTINGS_BAND_SEL_CAL_EN_SHFT                                     0x3
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CALIBRATION_SETTINGS_SKIP_BAND_SEL_CAL_BMSK                                   0x4
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CALIBRATION_SETTINGS_SKIP_BAND_SEL_CAL_SHFT                                   0x2
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CALIBRATION_SETTINGS_CM_PLL_CAL_MUX_BMSK                                      0x2
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CALIBRATION_SETTINGS_CM_PLL_CAL_MUX_SHFT                                      0x1
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CALIBRATION_SETTINGS_CM_PLL_CAL_BMSK                                          0x1
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CALIBRATION_SETTINGS_CM_PLL_CAL_SHFT                                          0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_TIMER_LOW_ADDR                                            (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000048)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_TIMER_LOW_RMSK                                                  0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_TIMER_LOW_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_TIMER_LOW_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_TIMER_LOW_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_TIMER_LOW_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_TIMER_LOW_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_TIMER_LOW_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_TIMER_LOW_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_TIMER_LOW_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_TIMER_LOW_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_TIMER_LOW_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_TIMER_LOW_BAND_SEL_CAL_TIMER_LOW_BMSK                           0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_TIMER_LOW_BAND_SEL_CAL_TIMER_LOW_SHFT                            0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_TIMER_HIGH_ADDR                                           (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x0000004c)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_TIMER_HIGH_RMSK                                                 0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_TIMER_HIGH_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_TIMER_HIGH_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_TIMER_HIGH_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_TIMER_HIGH_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_TIMER_HIGH_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_TIMER_HIGH_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_TIMER_HIGH_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_TIMER_HIGH_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_TIMER_HIGH_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_TIMER_HIGH_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_TIMER_HIGH_BAND_SEL_CAL_TIMER_HIGH_BMSK                         0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_TIMER_HIGH_BAND_SEL_CAL_TIMER_HIGH_SHFT                          0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_ADDR                                             (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000050)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_RMSK                                                   0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_BAND_SEL_CAL_INIT_BMSK                                 0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_BAND_SEL_CAL_INIT_SHFT                                  0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_MIN_ADDR                                                      (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000054)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_MIN_RMSK                                                            0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_MIN_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_MIN_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_MIN_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_MIN_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_MIN_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_MIN_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_MIN_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_MIN_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_MIN_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_MIN_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_MIN_BAND_SEL_CAL_MIN_BMSK                                           0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_MIN_BAND_SEL_CAL_MIN_SHFT                                            0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_MAX_ADDR                                                      (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000058)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_MAX_RMSK                                                            0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_MAX_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_MAX_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_MAX_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_MAX_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_MAX_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_MAX_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_MAX_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_MAX_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_MAX_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_MAX_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_MAX_BAND_SEL_CAL_MAX_BMSK                                           0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_MAX_BAND_SEL_CAL_MAX_SHFT                                            0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_PFILT_ADDR                                                    (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x0000005c)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_PFILT_RMSK                                                          0x1f
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_PFILT_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_PFILT_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_PFILT_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_PFILT_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_PFILT_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_PFILT_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_PFILT_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_PFILT_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_PFILT_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_PFILT_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_PFILT_BS_PFILT_BMSK                                                 0x1f
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_PFILT_BS_PFILT_SHFT                                                  0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_IFILT_ADDR                                                    (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000060)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_IFILT_RMSK                                                          0x1f
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_IFILT_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_IFILT_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_IFILT_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_IFILT_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_IFILT_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_IFILT_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_IFILT_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_IFILT_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_IFILT_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_IFILT_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_IFILT_BS_IFILT_BMSK                                                 0x1f
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_IFILT_BS_IFILT_SHFT                                                  0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_TWO_ADDR                                         (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000064)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_TWO_RMSK                                               0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_TWO_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_TWO_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_TWO_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_TWO_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_TWO_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_TWO_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_TWO_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_TWO_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_TWO_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_TWO_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_TWO_BS_CAL_DONE_MUX_BMSK                               0x80
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_TWO_BS_CAL_DONE_MUX_SHFT                                0x7
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_TWO_BS_CAL_DONE_BMSK                                   0x40
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_TWO_BS_CAL_DONE_SHFT                                    0x6
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_TWO_BS_STOP_ON_DONE_BMSK                               0x20
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_TWO_BS_STOP_ON_DONE_SHFT                                0x5
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_TWO_BS_LOCK_COUNT_BMSK                                 0x1c
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_TWO_BS_LOCK_COUNT_SHFT                                  0x2
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_TWO_BS_LOCK_THRESH_BMSK                                 0x3
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_TWO_BS_LOCK_THRESH_SHFT                                 0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_THREE_ADDR                                       (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000068)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_THREE_RMSK                                             0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_THREE_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_THREE_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_THREE_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_THREE_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_THREE_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_THREE_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_THREE_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_THREE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_THREE_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_THREE_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_THREE_USE_BS_CAL_CODE_BMSK                             0x80
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_THREE_USE_BS_CAL_CODE_SHFT                              0x7
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_THREE_BS_NO_SETTLE_BMSK                                0x40
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_THREE_BS_NO_SETTLE_SHFT                                 0x6
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_THREE_BS_GAIN_BMSK                                     0x3c
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_THREE_BS_GAIN_SHFT                                      0x2
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_THREE_BS_INIT_ON_REENABLE_BMSK                          0x2
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_THREE_BS_INIT_ON_REENABLE_SHFT                          0x1
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_THREE_BS_DEMET_ERROR_BMSK                               0x1
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_THREE_BS_DEMET_ERROR_SHFT                               0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_FOUR_ADDR                                        (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x0000006c)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_FOUR_RMSK                                              0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_FOUR_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_FOUR_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_FOUR_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_FOUR_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_FOUR_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_FOUR_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_FOUR_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_FOUR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_FOUR_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_FOUR_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_FOUR_BS_SETTLE_TIME_BMSK                               0xf0
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_FOUR_BS_SETTLE_TIME_SHFT                                0x4
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_FOUR_BS_INITIAL_TIME_BMSK                               0xf
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_SETTINGS_FOUR_BS_INITIAL_TIME_SHFT                               0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_ICODE_HIGH_ADDR                                               (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000070)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_ICODE_HIGH_RMSK                                                     0x1f
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_ICODE_HIGH_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_ICODE_HIGH_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_ICODE_HIGH_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_ICODE_HIGH_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_ICODE_HIGH_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_ICODE_HIGH_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_ICODE_HIGH_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_ICODE_HIGH_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_ICODE_HIGH_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_ICODE_HIGH_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_ICODE_HIGH_BAND_SEL_2MSB_MUX_BMSK                                   0x10
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_ICODE_HIGH_BAND_SEL_2MSB_MUX_SHFT                                    0x4
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_ICODE_HIGH_BAND_SEL_2MSB_BMSK                                        0xc
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_ICODE_HIGH_BAND_SEL_2MSB_SHFT                                        0x2
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_ICODE_HIGH_BAND_SEL_ICODE_HIGH_BMSK                                  0x3
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_ICODE_HIGH_BAND_SEL_ICODE_HIGH_SHFT                                  0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_ICODE_LOW_ADDR                                                (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000074)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_ICODE_LOW_RMSK                                                      0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_ICODE_LOW_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_ICODE_LOW_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_ICODE_LOW_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_ICODE_LOW_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_ICODE_LOW_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_ICODE_LOW_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_ICODE_LOW_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_ICODE_LOW_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_ICODE_LOW_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_ICODE_LOW_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_ICODE_LOW_BAND_SEL_ICODE_LOW_BMSK                                   0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_ICODE_LOW_BAND_SEL_ICODE_LOW_SHFT                                    0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_DETECT_SETTINGS_ONE_ADDR                                          (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000078)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_DETECT_SETTINGS_ONE_RMSK                                                0x7f
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_DETECT_SETTINGS_ONE_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_DETECT_SETTINGS_ONE_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_DETECT_SETTINGS_ONE_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_DETECT_SETTINGS_ONE_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_DETECT_SETTINGS_ONE_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_DETECT_SETTINGS_ONE_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_DETECT_SETTINGS_ONE_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_DETECT_SETTINGS_ONE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_DETECT_SETTINGS_ONE_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_DETECT_SETTINGS_ONE_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_DETECT_SETTINGS_ONE_STICKY_UNLOCK_BMSK                                  0x40
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_DETECT_SETTINGS_ONE_STICKY_UNLOCK_SHFT                                   0x6
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_DETECT_SETTINGS_ONE_USE_FD_LOCK_BMSK                                    0x20
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_DETECT_SETTINGS_ONE_USE_FD_LOCK_SHFT                                     0x5
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_DETECT_SETTINGS_ONE_INVERT_DETECT_POLARITY_BMSK                         0x10
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_DETECT_SETTINGS_ONE_INVERT_DETECT_POLARITY_SHFT                          0x4
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_DETECT_SETTINGS_ONE_PFD_SOURCE_BMSK                                      0x8
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_DETECT_SETTINGS_ONE_PFD_SOURCE_SHFT                                      0x3
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_DETECT_SETTINGS_ONE_FD_MODE_BMSK                                         0x4
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_DETECT_SETTINGS_ONE_FD_MODE_SHFT                                         0x2
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_DETECT_SETTINGS_ONE_FD_ENABLE_MUX_BMSK                                   0x2
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_DETECT_SETTINGS_ONE_FD_ENABLE_MUX_SHFT                                   0x1
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_DETECT_SETTINGS_ONE_FD_ENABLE_BMSK                                       0x1
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_DETECT_SETTINGS_ONE_FD_ENABLE_SHFT                                       0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_DETECT_THRESH_ADDR                                                (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x0000007c)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_DETECT_THRESH_RMSK                                                      0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_DETECT_THRESH_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_DETECT_THRESH_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_DETECT_THRESH_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_DETECT_THRESH_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_DETECT_THRESH_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_DETECT_THRESH_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_DETECT_THRESH_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_DETECT_THRESH_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_DETECT_THRESH_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_DETECT_THRESH_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_DETECT_THRESH_FD_THRESH_BMSK                                            0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_DETECT_THRESH_FD_THRESH_SHFT                                             0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_DET_REFCLK_HIGH_ADDR                                              (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000080)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_DET_REFCLK_HIGH_RMSK                                                     0x3
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_DET_REFCLK_HIGH_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_DET_REFCLK_HIGH_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_DET_REFCLK_HIGH_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_DET_REFCLK_HIGH_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_DET_REFCLK_HIGH_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_DET_REFCLK_HIGH_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_DET_REFCLK_HIGH_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_DET_REFCLK_HIGH_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_DET_REFCLK_HIGH_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_DET_REFCLK_HIGH_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_DET_REFCLK_HIGH_FD_REF_HIGH_BMSK                                         0x3
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_DET_REFCLK_HIGH_FD_REF_HIGH_SHFT                                         0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_DET_REFCLK_LOW_ADDR                                               (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000084)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_DET_REFCLK_LOW_RMSK                                                     0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_DET_REFCLK_LOW_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_DET_REFCLK_LOW_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_DET_REFCLK_LOW_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_DET_REFCLK_LOW_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_DET_REFCLK_LOW_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_DET_REFCLK_LOW_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_DET_REFCLK_LOW_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_DET_REFCLK_LOW_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_DET_REFCLK_LOW_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_DET_REFCLK_LOW_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_DET_REFCLK_LOW_FD_REF_LOW_BMSK                                          0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_DET_REFCLK_LOW_FD_REF_LOW_SHFT                                           0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_DET_PLLCLK_HIGH_ADDR                                              (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000088)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_DET_PLLCLK_HIGH_RMSK                                                    0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_DET_PLLCLK_HIGH_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_DET_PLLCLK_HIGH_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_DET_PLLCLK_HIGH_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_DET_PLLCLK_HIGH_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_DET_PLLCLK_HIGH_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_DET_PLLCLK_HIGH_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_DET_PLLCLK_HIGH_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_DET_PLLCLK_HIGH_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_DET_PLLCLK_HIGH_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_DET_PLLCLK_HIGH_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_DET_PLLCLK_HIGH_FD_PLL_HIGH_BMSK                                        0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_DET_PLLCLK_HIGH_FD_PLL_HIGH_SHFT                                         0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_DET_PLLCLK_LOW_ADDR                                               (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x0000008c)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_DET_PLLCLK_LOW_RMSK                                                     0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_DET_PLLCLK_LOW_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_DET_PLLCLK_LOW_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_DET_PLLCLK_LOW_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_DET_PLLCLK_LOW_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_DET_PLLCLK_LOW_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_DET_PLLCLK_LOW_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_DET_PLLCLK_LOW_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_DET_PLLCLK_LOW_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_DET_PLLCLK_LOW_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_DET_PLLCLK_LOW_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_DET_PLLCLK_LOW_FD_PLL_LOW_BMSK                                          0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_DET_PLLCLK_LOW_FD_PLL_LOW_SHFT                                           0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_PFILT_ADDR                                                             (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000090)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PFILT_RMSK                                                                   0x3f
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PFILT_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_PFILT_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_PFILT_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PFILT_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_PFILT_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PFILT_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_PFILT_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PFILT_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_PFILT_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_PFILT_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PFILT_CM_PLL_PFILT_MUX_BMSK                                                  0x20
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PFILT_CM_PLL_PFILT_MUX_SHFT                                                   0x5
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PFILT_CM_PLL_PFILT_BMSK                                                      0x1f
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PFILT_CM_PLL_PFILT_SHFT                                                       0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_IFILT_ADDR                                                             (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000094)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_IFILT_RMSK                                                                   0x3f
#define HWIO_DSI_1_PHY_DSIPHY_PLL_IFILT_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_IFILT_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_IFILT_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_IFILT_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_IFILT_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_IFILT_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_IFILT_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_IFILT_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_IFILT_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_IFILT_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_IFILT_CM_PLL_IFILT_MUX_BMSK                                                  0x20
#define HWIO_DSI_1_PHY_DSIPHY_PLL_IFILT_CM_PLL_IFILT_MUX_SHFT                                                   0x5
#define HWIO_DSI_1_PHY_DSIPHY_PLL_IFILT_CM_PLL_IFILT_BMSK                                                      0x1f
#define HWIO_DSI_1_PHY_DSIPHY_PLL_IFILT_CM_PLL_IFILT_SHFT                                                       0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_GAIN_ADDR                                                          (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000098)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_GAIN_RMSK                                                                0x7f
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_GAIN_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_GAIN_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_GAIN_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_GAIN_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_GAIN_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_GAIN_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_GAIN_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_GAIN_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_GAIN_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_GAIN_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_GAIN_CM_PLL_PROP_GAIN_BMSK                                               0x78
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_GAIN_CM_PLL_PROP_GAIN_SHFT                                                0x3
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_GAIN_CM_PLL_INT_GAIN_BMSK                                                 0x7
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_GAIN_CM_PLL_INT_GAIN_SHFT                                                 0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_ICODE_LOW_ADDR                                                         (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x0000009c)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ICODE_LOW_RMSK                                                               0x3f
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ICODE_LOW_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_ICODE_LOW_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_ICODE_LOW_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ICODE_LOW_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_ICODE_LOW_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ICODE_LOW_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_ICODE_LOW_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ICODE_LOW_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_ICODE_LOW_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_ICODE_LOW_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ICODE_LOW_CM_PLL_ICODE_LOW_MUX_BMSK                                          0x20
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ICODE_LOW_CM_PLL_ICODE_LOW_MUX_SHFT                                           0x5
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ICODE_LOW_CM_PLL_ICODE_LOW_BMSK                                              0x1f
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ICODE_LOW_CM_PLL_ICODE_LOW_SHFT                                               0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_ICODE_HIGH_ADDR                                                        (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x000000a0)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ICODE_HIGH_RMSK                                                              0x5f
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ICODE_HIGH_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_ICODE_HIGH_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_ICODE_HIGH_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ICODE_HIGH_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_ICODE_HIGH_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ICODE_HIGH_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_ICODE_HIGH_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ICODE_HIGH_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_ICODE_HIGH_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_ICODE_HIGH_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ICODE_HIGH_CM_PLL_ICODE_HIGH_MUX_BMSK                                        0x40
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ICODE_HIGH_CM_PLL_ICODE_HIGH_MUX_SHFT                                         0x6
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ICODE_HIGH_CM_PLL_ICODE_HIGH_BMSK                                            0x1f
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ICODE_HIGH_CM_PLL_ICODE_HIGH_SHFT                                             0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_LOCKDET_ADDR                                                           (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x000000a4)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_LOCKDET_RMSK                                                                 0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_LOCKDET_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_LOCKDET_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_LOCKDET_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_LOCKDET_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_LOCKDET_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_LOCKDET_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_LOCKDET_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_LOCKDET_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_LOCKDET_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_LOCKDET_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_LOCKDET_CM_PLL_LOCKDET_BMSK                                                  0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_LOCKDET_CM_PLL_LOCKDET_SHFT                                                   0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_OUTDIV_ADDR                                                            (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x000000a8)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_OUTDIV_RMSK                                                                   0x7
#define HWIO_DSI_1_PHY_DSIPHY_PLL_OUTDIV_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_OUTDIV_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_OUTDIV_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_OUTDIV_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_OUTDIV_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_OUTDIV_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_OUTDIV_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_OUTDIV_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_OUTDIV_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_OUTDIV_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_OUTDIV_CM_PLL_OUTDIV_MUX_BMSK                                                 0x4
#define HWIO_DSI_1_PHY_DSIPHY_PLL_OUTDIV_CM_PLL_OUTDIV_MUX_SHFT                                                 0x2
#define HWIO_DSI_1_PHY_DSIPHY_PLL_OUTDIV_CM_PLL_OUTDIV_BMSK                                                     0x3
#define HWIO_DSI_1_PHY_DSIPHY_PLL_OUTDIV_CM_PLL_OUTDIV_SHFT                                                     0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_FASTLOCK_CONTROL_ADDR                                                  (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x000000ac)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FASTLOCK_CONTROL_RMSK                                                        0x3f
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FASTLOCK_CONTROL_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_FASTLOCK_CONTROL_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_FASTLOCK_CONTROL_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FASTLOCK_CONTROL_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_FASTLOCK_CONTROL_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FASTLOCK_CONTROL_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_FASTLOCK_CONTROL_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FASTLOCK_CONTROL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_FASTLOCK_CONTROL_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_FASTLOCK_CONTROL_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FASTLOCK_CONTROL_CM_PLL_FASTLOCK_EN_MUX_BMSK                                 0x20
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FASTLOCK_CONTROL_CM_PLL_FASTLOCK_EN_MUX_SHFT                                  0x5
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FASTLOCK_CONTROL_CM_PLL_FASTLOCK_EN_BMSK                                     0x10
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FASTLOCK_CONTROL_CM_PLL_FASTLOCK_EN_SHFT                                      0x4
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FASTLOCK_CONTROL_CM_PLL_INT_GAIN_FASTLOCK_MUX_BMSK                            0x8
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FASTLOCK_CONTROL_CM_PLL_INT_GAIN_FASTLOCK_MUX_SHFT                            0x3
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FASTLOCK_CONTROL_CM_PLL_INT_GAIN_FASTLOCK_BMSK                                0x7
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FASTLOCK_CONTROL_CM_PLL_INT_GAIN_FASTLOCK_SHFT                                0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_PASS_OUT_OVERRIDE_ONE_ADDR                                             (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x000000b0)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PASS_OUT_OVERRIDE_ONE_RMSK                                                   0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PASS_OUT_OVERRIDE_ONE_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_PASS_OUT_OVERRIDE_ONE_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_PASS_OUT_OVERRIDE_ONE_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PASS_OUT_OVERRIDE_ONE_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_PASS_OUT_OVERRIDE_ONE_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PASS_OUT_OVERRIDE_ONE_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_PASS_OUT_OVERRIDE_ONE_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PASS_OUT_OVERRIDE_ONE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_PASS_OUT_OVERRIDE_ONE_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_PASS_OUT_OVERRIDE_ONE_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PASS_OUT_OVERRIDE_ONE_PASS_RES_CODE_OUT_MUX_BMSK                             0x80
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PASS_OUT_OVERRIDE_ONE_PASS_RES_CODE_OUT_MUX_SHFT                              0x7
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PASS_OUT_OVERRIDE_ONE_PASS_RES_CODE_OUT_BMSK                                 0x78
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PASS_OUT_OVERRIDE_ONE_PASS_RES_CODE_OUT_SHFT                                  0x3
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PASS_OUT_OVERRIDE_ONE_PASS_BAND_OUT_MUX_BMSK                                  0x4
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PASS_OUT_OVERRIDE_ONE_PASS_BAND_OUT_MUX_SHFT                                  0x2
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PASS_OUT_OVERRIDE_ONE_PASS_BAND_OUT_BMSK                                      0x3
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PASS_OUT_OVERRIDE_ONE_PASS_BAND_OUT_SHFT                                      0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_PASS_OUT_OVERRIDE_TWO_ADDR                                             (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x000000b4)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PASS_OUT_OVERRIDE_TWO_RMSK                                                    0x3
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PASS_OUT_OVERRIDE_TWO_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_PASS_OUT_OVERRIDE_TWO_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_PASS_OUT_OVERRIDE_TWO_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PASS_OUT_OVERRIDE_TWO_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_PASS_OUT_OVERRIDE_TWO_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PASS_OUT_OVERRIDE_TWO_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_PASS_OUT_OVERRIDE_TWO_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PASS_OUT_OVERRIDE_TWO_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_PASS_OUT_OVERRIDE_TWO_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_PASS_OUT_OVERRIDE_TWO_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PASS_OUT_OVERRIDE_TWO_PASS_LS_EN_OUT_MUX_BMSK                                 0x2
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PASS_OUT_OVERRIDE_TWO_PASS_LS_EN_OUT_MUX_SHFT                                 0x1
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PASS_OUT_OVERRIDE_TWO_PASS_LS_EN_OUT_BMSK                                     0x1
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PASS_OUT_OVERRIDE_TWO_PASS_LS_EN_OUT_SHFT                                     0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_CORE_OVERRIDE_ADDR                                                     (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x000000b8)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CORE_OVERRIDE_RMSK                                                           0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CORE_OVERRIDE_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_CORE_OVERRIDE_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_CORE_OVERRIDE_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CORE_OVERRIDE_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_CORE_OVERRIDE_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CORE_OVERRIDE_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_CORE_OVERRIDE_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CORE_OVERRIDE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_CORE_OVERRIDE_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_CORE_OVERRIDE_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CORE_OVERRIDE_UNLOCK_READY_MUX_BMSK                                          0x80
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CORE_OVERRIDE_UNLOCK_READY_MUX_SHFT                                           0x7
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CORE_OVERRIDE_UNLOCK_READY_BMSK                                              0x40
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CORE_OVERRIDE_UNLOCK_READY_SHFT                                               0x6
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CORE_OVERRIDE_PASS_PLL_READY_MUX_BMSK                                        0x20
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CORE_OVERRIDE_PASS_PLL_READY_MUX_SHFT                                         0x5
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CORE_OVERRIDE_PASS_PLL_READY_BMSK                                            0x10
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CORE_OVERRIDE_PASS_PLL_READY_SHFT                                             0x4
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CORE_OVERRIDE_CORE_PLL_UNLOCK_MUX_BMSK                                        0x8
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CORE_OVERRIDE_CORE_PLL_UNLOCK_MUX_SHFT                                        0x3
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CORE_OVERRIDE_CORE_PLL_UNLOCK_BMSK                                            0x4
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CORE_OVERRIDE_CORE_PLL_UNLOCK_SHFT                                            0x2
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CORE_OVERRIDE_CORE_READY_MUX_BMSK                                             0x2
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CORE_OVERRIDE_CORE_READY_MUX_SHFT                                             0x1
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CORE_OVERRIDE_CORE_READY_BMSK                                                 0x1
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CORE_OVERRIDE_CORE_READY_SHFT                                                 0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_CORE_INPUT_OVERRIDE_ADDR                                               (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x000000bc)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CORE_INPUT_OVERRIDE_RMSK                                                     0x7f
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CORE_INPUT_OVERRIDE_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_CORE_INPUT_OVERRIDE_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_CORE_INPUT_OVERRIDE_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CORE_INPUT_OVERRIDE_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_CORE_INPUT_OVERRIDE_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CORE_INPUT_OVERRIDE_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_CORE_INPUT_OVERRIDE_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CORE_INPUT_OVERRIDE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_CORE_INPUT_OVERRIDE_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_CORE_INPUT_OVERRIDE_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CORE_INPUT_OVERRIDE_CORE_RESET_MUX_BMSK                                      0x40
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CORE_INPUT_OVERRIDE_CORE_RESET_MUX_SHFT                                       0x6
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CORE_INPUT_OVERRIDE_CORE_RESET_BMSK                                          0x20
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CORE_INPUT_OVERRIDE_CORE_RESET_SHFT                                           0x5
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CORE_INPUT_OVERRIDE_CORE_PLL_EN_FROM_RESET_BMSK                              0x10
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CORE_INPUT_OVERRIDE_CORE_PLL_EN_FROM_RESET_SHFT                               0x4
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CORE_INPUT_OVERRIDE_CORE_PLL_EN_MUX_BMSK                                      0x8
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CORE_INPUT_OVERRIDE_CORE_PLL_EN_MUX_SHFT                                      0x3
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CORE_INPUT_OVERRIDE_CORE_PLL_EN_BMSK                                          0x4
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CORE_INPUT_OVERRIDE_CORE_PLL_EN_SHFT                                          0x2
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CORE_INPUT_OVERRIDE_CORE_PLL_RATE_MUX_BMSK                                    0x2
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CORE_INPUT_OVERRIDE_CORE_PLL_RATE_MUX_SHFT                                    0x1
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CORE_INPUT_OVERRIDE_CORE_PLL_RATE_BMSK                                        0x1
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CORE_INPUT_OVERRIDE_CORE_PLL_RATE_SHFT                                        0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_RATE_CHANGE_ADDR                                                       (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x000000c0)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_RATE_CHANGE_RMSK                                                             0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_RATE_CHANGE_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_RATE_CHANGE_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_RATE_CHANGE_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_RATE_CHANGE_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_RATE_CHANGE_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_RATE_CHANGE_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_RATE_CHANGE_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_RATE_CHANGE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_RATE_CHANGE_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_RATE_CHANGE_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_RATE_CHANGE_PPM_UNLOCK_TIMER_LSB_BMSK                                        0xfe
#define HWIO_DSI_1_PHY_DSIPHY_PLL_RATE_CHANGE_PPM_UNLOCK_TIMER_LSB_SHFT                                         0x1
#define HWIO_DSI_1_PHY_DSIPHY_PLL_RATE_CHANGE_PLL_RATE_CHANGE_BMSK                                              0x1
#define HWIO_DSI_1_PHY_DSIPHY_PLL_RATE_CHANGE_PLL_RATE_CHANGE_SHFT                                              0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_DIGITAL_TIMERS_ADDR                                                (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x000000c4)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_DIGITAL_TIMERS_RMSK                                                      0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_DIGITAL_TIMERS_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_DIGITAL_TIMERS_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_DIGITAL_TIMERS_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_DIGITAL_TIMERS_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_DIGITAL_TIMERS_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_DIGITAL_TIMERS_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_DIGITAL_TIMERS_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_DIGITAL_TIMERS_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_DIGITAL_TIMERS_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_DIGITAL_TIMERS_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_DIGITAL_TIMERS_PPM_UNLOCK_TIMER_SEL_BMSK                                 0x80
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_DIGITAL_TIMERS_PPM_UNLOCK_TIMER_SEL_SHFT                                  0x7
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_DIGITAL_TIMERS_PPM_UNLOCK_TIMER_MSB_BMSK                                 0x70
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_DIGITAL_TIMERS_PPM_UNLOCK_TIMER_MSB_SHFT                                  0x4
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_DIGITAL_TIMERS_RATE_CHANGE_SETTLE_BMSK                                    0xf
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_DIGITAL_TIMERS_RATE_CHANGE_SETTLE_SHFT                                    0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_DIGITAL_TIMERS_TWO_ADDR                                            (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x000000c8)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_DIGITAL_TIMERS_TWO_RMSK                                                  0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_DIGITAL_TIMERS_TWO_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_DIGITAL_TIMERS_TWO_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_DIGITAL_TIMERS_TWO_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_DIGITAL_TIMERS_TWO_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_DIGITAL_TIMERS_TWO_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_DIGITAL_TIMERS_TWO_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_DIGITAL_TIMERS_TWO_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_DIGITAL_TIMERS_TWO_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_DIGITAL_TIMERS_TWO_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_DIGITAL_TIMERS_TWO_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_DIGITAL_TIMERS_TWO_PLL_EN_TO_RESET_TIME_BMSK                             0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_DIGITAL_TIMERS_TWO_PLL_EN_TO_RESET_TIME_SHFT                              0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_DECIMAL_DIV_START_ADDR                                                 (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x000000cc)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_DECIMAL_DIV_START_RMSK                                                       0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_DECIMAL_DIV_START_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_DECIMAL_DIV_START_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_DECIMAL_DIV_START_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_DECIMAL_DIV_START_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_DECIMAL_DIV_START_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_DECIMAL_DIV_START_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_DECIMAL_DIV_START_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_DECIMAL_DIV_START_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_DECIMAL_DIV_START_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_DECIMAL_DIV_START_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_DECIMAL_DIV_START_PLL_DIV_DEC_START_BMSK                                     0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_DECIMAL_DIV_START_PLL_DIV_DEC_START_SHFT                                      0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_LOW_ADDR                                                (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x000000d0)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_LOW_RMSK                                                      0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_LOW_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_LOW_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_LOW_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_LOW_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_LOW_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_LOW_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_LOW_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_LOW_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_LOW_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_LOW_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_LOW_PLL_DIV_FRAC_START_LOW_BMSK                               0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_LOW_PLL_DIV_FRAC_START_LOW_SHFT                                0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_MID_ADDR                                                (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x000000d4)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_MID_RMSK                                                      0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_MID_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_MID_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_MID_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_MID_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_MID_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_MID_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_MID_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_MID_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_MID_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_MID_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_MID_PLL_DIV_FRAC_START_MID_BMSK                               0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_MID_PLL_DIV_FRAC_START_MID_SHFT                                0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_HIGH_ADDR                                               (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x000000d8)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_HIGH_RMSK                                                      0x3
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_HIGH_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_HIGH_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_HIGH_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_HIGH_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_HIGH_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_HIGH_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_HIGH_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_HIGH_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_HIGH_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_HIGH_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_HIGH_PLL_DIV_FRAC_START_HIGH_BMSK                              0x3
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_HIGH_PLL_DIV_FRAC_START_HIGH_SHFT                              0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_DEC_FRAC_MUXES_ADDR                                                    (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x000000dc)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_DEC_FRAC_MUXES_RMSK                                                           0xf
#define HWIO_DSI_1_PHY_DSIPHY_PLL_DEC_FRAC_MUXES_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_DEC_FRAC_MUXES_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_DEC_FRAC_MUXES_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_DEC_FRAC_MUXES_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_DEC_FRAC_MUXES_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_DEC_FRAC_MUXES_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_DEC_FRAC_MUXES_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_DEC_FRAC_MUXES_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_DEC_FRAC_MUXES_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_DEC_FRAC_MUXES_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_DEC_FRAC_MUXES_PLL_DIV_FRAC_START_HIGH_MUX_BMSK                               0x8
#define HWIO_DSI_1_PHY_DSIPHY_PLL_DEC_FRAC_MUXES_PLL_DIV_FRAC_START_HIGH_MUX_SHFT                               0x3
#define HWIO_DSI_1_PHY_DSIPHY_PLL_DEC_FRAC_MUXES_PLL_DIV_FRAC_START_MID_MUX_BMSK                                0x4
#define HWIO_DSI_1_PHY_DSIPHY_PLL_DEC_FRAC_MUXES_PLL_DIV_FRAC_START_MID_MUX_SHFT                                0x2
#define HWIO_DSI_1_PHY_DSIPHY_PLL_DEC_FRAC_MUXES_PLL_DIV_FRAC_START_LOW_MUX_BMSK                                0x2
#define HWIO_DSI_1_PHY_DSIPHY_PLL_DEC_FRAC_MUXES_PLL_DIV_FRAC_START_LOW_MUX_SHFT                                0x1
#define HWIO_DSI_1_PHY_DSIPHY_PLL_DEC_FRAC_MUXES_PLL_DIV_DEC_START_MUX_BMSK                                     0x1
#define HWIO_DSI_1_PHY_DSIPHY_PLL_DEC_FRAC_MUXES_PLL_DIV_DEC_START_MUX_SHFT                                     0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_DECIMAL_DIV_START_1_ADDR                                               (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x000000e0)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_DECIMAL_DIV_START_1_RMSK                                                     0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_DECIMAL_DIV_START_1_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_DECIMAL_DIV_START_1_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_DECIMAL_DIV_START_1_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_DECIMAL_DIV_START_1_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_DECIMAL_DIV_START_1_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_DECIMAL_DIV_START_1_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_DECIMAL_DIV_START_1_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_DECIMAL_DIV_START_1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_DECIMAL_DIV_START_1_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_DECIMAL_DIV_START_1_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_DECIMAL_DIV_START_1_PLL_DIV_DEC_START_1_BMSK                                 0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_DECIMAL_DIV_START_1_PLL_DIV_DEC_START_1_SHFT                                  0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_LOW_1_ADDR                                              (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x000000e4)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_LOW_1_RMSK                                                    0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_LOW_1_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_LOW_1_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_LOW_1_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_LOW_1_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_LOW_1_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_LOW_1_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_LOW_1_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_LOW_1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_LOW_1_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_LOW_1_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_LOW_1_PLL_DIV_FRAC_START_LOW_1_BMSK                           0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_LOW_1_PLL_DIV_FRAC_START_LOW_1_SHFT                            0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_MID_1_ADDR                                              (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x000000e8)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_MID_1_RMSK                                                    0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_MID_1_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_MID_1_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_MID_1_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_MID_1_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_MID_1_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_MID_1_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_MID_1_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_MID_1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_MID_1_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_MID_1_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_MID_1_PLL_DIV_FRAC_START_MID_1_BMSK                           0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_MID_1_PLL_DIV_FRAC_START_MID_1_SHFT                            0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_HIGH_1_ADDR                                             (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x000000ec)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_HIGH_1_RMSK                                                    0x3
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_HIGH_1_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_HIGH_1_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_HIGH_1_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_HIGH_1_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_HIGH_1_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_HIGH_1_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_HIGH_1_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_HIGH_1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_HIGH_1_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_HIGH_1_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_HIGH_1_PLL_DIV_FRAC_START_HIGH_1_BMSK                          0x3
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_HIGH_1_PLL_DIV_FRAC_START_HIGH_1_SHFT                          0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_DECIMAL_DIV_START_2_ADDR                                               (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x000000f0)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_DECIMAL_DIV_START_2_RMSK                                                     0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_DECIMAL_DIV_START_2_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_DECIMAL_DIV_START_2_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_DECIMAL_DIV_START_2_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_DECIMAL_DIV_START_2_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_DECIMAL_DIV_START_2_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_DECIMAL_DIV_START_2_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_DECIMAL_DIV_START_2_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_DECIMAL_DIV_START_2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_DECIMAL_DIV_START_2_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_DECIMAL_DIV_START_2_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_DECIMAL_DIV_START_2_PLL_DIV_DEC_START_2_BMSK                                 0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_DECIMAL_DIV_START_2_PLL_DIV_DEC_START_2_SHFT                                  0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_LOW_2_ADDR                                              (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x000000f4)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_LOW_2_RMSK                                                    0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_LOW_2_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_LOW_2_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_LOW_2_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_LOW_2_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_LOW_2_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_LOW_2_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_LOW_2_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_LOW_2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_LOW_2_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_LOW_2_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_LOW_2_PLL_DIV_FRAC_START_LOW_2_BMSK                           0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_LOW_2_PLL_DIV_FRAC_START_LOW_2_SHFT                            0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_MID_2_ADDR                                              (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x000000f8)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_MID_2_RMSK                                                    0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_MID_2_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_MID_2_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_MID_2_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_MID_2_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_MID_2_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_MID_2_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_MID_2_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_MID_2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_MID_2_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_MID_2_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_MID_2_PLL_DIV_FRAC_START_MID_2_BMSK                           0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_MID_2_PLL_DIV_FRAC_START_MID_2_SHFT                            0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_HIGH_2_ADDR                                             (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x000000fc)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_HIGH_2_RMSK                                                    0x3
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_HIGH_2_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_HIGH_2_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_HIGH_2_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_HIGH_2_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_HIGH_2_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_HIGH_2_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_HIGH_2_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_HIGH_2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_HIGH_2_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_HIGH_2_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_HIGH_2_PLL_DIV_FRAC_START_HIGH_2_BMSK                          0x3
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FRAC_DIV_START_HIGH_2_PLL_DIV_FRAC_START_HIGH_2_SHFT                          0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_MASH_CONTROL_ADDR                                                      (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000100)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_MASH_CONTROL_RMSK                                                             0x3
#define HWIO_DSI_1_PHY_DSIPHY_PLL_MASH_CONTROL_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_MASH_CONTROL_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_MASH_CONTROL_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_MASH_CONTROL_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_MASH_CONTROL_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_MASH_CONTROL_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_MASH_CONTROL_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_MASH_CONTROL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_MASH_CONTROL_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_MASH_CONTROL_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_MASH_CONTROL_PLL_DIV_ORD_BMSK                                                 0x2
#define HWIO_DSI_1_PHY_DSIPHY_PLL_MASH_CONTROL_PLL_DIV_ORD_SHFT                                                 0x1
#define HWIO_DSI_1_PHY_DSIPHY_PLL_MASH_CONTROL_PLL_DIV_FFEN_BMSK                                                0x1
#define HWIO_DSI_1_PHY_DSIPHY_PLL_MASH_CONTROL_PLL_DIV_FFEN_SHFT                                                0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_STEPSIZE_LOW_ADDR                                                  (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000104)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_STEPSIZE_LOW_RMSK                                                        0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_STEPSIZE_LOW_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_STEPSIZE_LOW_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_STEPSIZE_LOW_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_STEPSIZE_LOW_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_STEPSIZE_LOW_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_STEPSIZE_LOW_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_STEPSIZE_LOW_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_STEPSIZE_LOW_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_STEPSIZE_LOW_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_STEPSIZE_LOW_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_STEPSIZE_LOW_PLL_DIV_STEPSIZE_LOW_BMSK                                   0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_STEPSIZE_LOW_PLL_DIV_STEPSIZE_LOW_SHFT                                    0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_STEPSIZE_HIGH_ADDR                                                 (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000108)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_STEPSIZE_HIGH_RMSK                                                       0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_STEPSIZE_HIGH_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_STEPSIZE_HIGH_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_STEPSIZE_HIGH_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_STEPSIZE_HIGH_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_STEPSIZE_HIGH_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_STEPSIZE_HIGH_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_STEPSIZE_HIGH_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_STEPSIZE_HIGH_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_STEPSIZE_HIGH_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_STEPSIZE_HIGH_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_STEPSIZE_HIGH_PLL_DIV_STEPSIZE_HIGH_BMSK                                 0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_STEPSIZE_HIGH_PLL_DIV_STEPSIZE_HIGH_SHFT                                  0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_DIV_PER_LOW_ADDR                                                   (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x0000010c)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_DIV_PER_LOW_RMSK                                                         0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_DIV_PER_LOW_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_DIV_PER_LOW_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_DIV_PER_LOW_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_DIV_PER_LOW_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_DIV_PER_LOW_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_DIV_PER_LOW_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_DIV_PER_LOW_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_DIV_PER_LOW_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_DIV_PER_LOW_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_DIV_PER_LOW_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_DIV_PER_LOW_PLL_DIV_PER_LOW_BMSK                                         0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_DIV_PER_LOW_PLL_DIV_PER_LOW_SHFT                                          0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_DIV_PER_HIGH_ADDR                                                  (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000110)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_DIV_PER_HIGH_RMSK                                                        0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_DIV_PER_HIGH_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_DIV_PER_HIGH_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_DIV_PER_HIGH_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_DIV_PER_HIGH_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_DIV_PER_HIGH_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_DIV_PER_HIGH_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_DIV_PER_HIGH_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_DIV_PER_HIGH_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_DIV_PER_HIGH_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_DIV_PER_HIGH_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_DIV_PER_HIGH_PLL_DIV_PER_HIGH_BMSK                                       0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_DIV_PER_HIGH_PLL_DIV_PER_HIGH_SHFT                                        0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_ADJPER_LOW_ADDR                                                    (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000114)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_ADJPER_LOW_RMSK                                                          0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_ADJPER_LOW_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_ADJPER_LOW_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_ADJPER_LOW_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_ADJPER_LOW_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_ADJPER_LOW_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_ADJPER_LOW_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_ADJPER_LOW_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_ADJPER_LOW_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_ADJPER_LOW_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_ADJPER_LOW_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_ADJPER_LOW_PLL_DIV_ADJPER_LOW_BMSK                                       0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_ADJPER_LOW_PLL_DIV_ADJPER_LOW_SHFT                                        0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_ADJPER_HIGH_ADDR                                                   (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000118)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_ADJPER_HIGH_RMSK                                                          0x3
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_ADJPER_HIGH_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_ADJPER_HIGH_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_ADJPER_HIGH_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_ADJPER_HIGH_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_ADJPER_HIGH_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_ADJPER_HIGH_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_ADJPER_HIGH_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_ADJPER_HIGH_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_ADJPER_HIGH_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_ADJPER_HIGH_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_ADJPER_HIGH_PLL_DIV_ADJPER_HIGH_BMSK                                      0x3
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_ADJPER_HIGH_PLL_DIV_ADJPER_HIGH_SHFT                                      0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_MUX_CONTROL_ADDR                                                   (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x0000011c)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_MUX_CONTROL_RMSK                                                         0x3f
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_MUX_CONTROL_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_MUX_CONTROL_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_MUX_CONTROL_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_MUX_CONTROL_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_MUX_CONTROL_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_MUX_CONTROL_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_MUX_CONTROL_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_MUX_CONTROL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_MUX_CONTROL_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_MUX_CONTROL_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_MUX_CONTROL_PLL_DIV_ADJPER_HIGH_MUX_BMSK                                 0x20
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_MUX_CONTROL_PLL_DIV_ADJPER_HIGH_MUX_SHFT                                  0x5
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_MUX_CONTROL_PLL_DIV_ADJPER_LOW_MUX_BMSK                                  0x10
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_MUX_CONTROL_PLL_DIV_ADJPER_LOW_MUX_SHFT                                   0x4
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_MUX_CONTROL_PLL_DIV_PER_HIGH_MUX_BMSK                                     0x8
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_MUX_CONTROL_PLL_DIV_PER_HIGH_MUX_SHFT                                     0x3
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_MUX_CONTROL_PLL_DIV_PER_LOW_MUX_BMSK                                      0x4
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_MUX_CONTROL_PLL_DIV_PER_LOW_MUX_SHFT                                      0x2
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_MUX_CONTROL_PLL_DIV_STEPSIZE_HIGH_MUX_BMSK                                0x2
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_MUX_CONTROL_PLL_DIV_STEPSIZE_HIGH_MUX_SHFT                                0x1
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_MUX_CONTROL_PLL_DIV_STEPSIZE_LOW_MUX_BMSK                                 0x1
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_MUX_CONTROL_PLL_DIV_STEPSIZE_LOW_MUX_SHFT                                 0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_STEPSIZE_LOW_1_ADDR                                                (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000120)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_STEPSIZE_LOW_1_RMSK                                                      0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_STEPSIZE_LOW_1_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_STEPSIZE_LOW_1_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_STEPSIZE_LOW_1_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_STEPSIZE_LOW_1_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_STEPSIZE_LOW_1_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_STEPSIZE_LOW_1_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_STEPSIZE_LOW_1_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_STEPSIZE_LOW_1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_STEPSIZE_LOW_1_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_STEPSIZE_LOW_1_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_STEPSIZE_LOW_1_PLL_DIV_STEPSIZE_LOW_1_BMSK                               0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_STEPSIZE_LOW_1_PLL_DIV_STEPSIZE_LOW_1_SHFT                                0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_STEPSIZE_HIGH_1_ADDR                                               (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000124)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_STEPSIZE_HIGH_1_RMSK                                                     0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_STEPSIZE_HIGH_1_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_STEPSIZE_HIGH_1_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_STEPSIZE_HIGH_1_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_STEPSIZE_HIGH_1_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_STEPSIZE_HIGH_1_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_STEPSIZE_HIGH_1_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_STEPSIZE_HIGH_1_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_STEPSIZE_HIGH_1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_STEPSIZE_HIGH_1_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_STEPSIZE_HIGH_1_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_STEPSIZE_HIGH_1_PLL_DIV_STEPSIZE_HIGH_1_BMSK                             0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_STEPSIZE_HIGH_1_PLL_DIV_STEPSIZE_HIGH_1_SHFT                              0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_DIV_PER_LOW_1_ADDR                                                 (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000128)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_DIV_PER_LOW_1_RMSK                                                       0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_DIV_PER_LOW_1_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_DIV_PER_LOW_1_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_DIV_PER_LOW_1_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_DIV_PER_LOW_1_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_DIV_PER_LOW_1_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_DIV_PER_LOW_1_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_DIV_PER_LOW_1_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_DIV_PER_LOW_1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_DIV_PER_LOW_1_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_DIV_PER_LOW_1_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_DIV_PER_LOW_1_PLL_DIV_PER_LOW_1_BMSK                                     0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_DIV_PER_LOW_1_PLL_DIV_PER_LOW_1_SHFT                                      0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_DIV_PER_HIGH_1_ADDR                                                (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x0000012c)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_DIV_PER_HIGH_1_RMSK                                                      0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_DIV_PER_HIGH_1_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_DIV_PER_HIGH_1_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_DIV_PER_HIGH_1_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_DIV_PER_HIGH_1_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_DIV_PER_HIGH_1_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_DIV_PER_HIGH_1_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_DIV_PER_HIGH_1_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_DIV_PER_HIGH_1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_DIV_PER_HIGH_1_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_DIV_PER_HIGH_1_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_DIV_PER_HIGH_1_PLL_DIV_PER_HIGH_1_BMSK                                   0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_DIV_PER_HIGH_1_PLL_DIV_PER_HIGH_1_SHFT                                    0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_ADJPER_LOW_1_ADDR                                                  (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000130)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_ADJPER_LOW_1_RMSK                                                        0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_ADJPER_LOW_1_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_ADJPER_LOW_1_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_ADJPER_LOW_1_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_ADJPER_LOW_1_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_ADJPER_LOW_1_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_ADJPER_LOW_1_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_ADJPER_LOW_1_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_ADJPER_LOW_1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_ADJPER_LOW_1_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_ADJPER_LOW_1_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_ADJPER_LOW_1_PLL_DIV_ADJPER_LOW_1_BMSK                                   0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_ADJPER_LOW_1_PLL_DIV_ADJPER_LOW_1_SHFT                                    0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_ADJPER_HIGH_1_ADDR                                                 (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000134)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_ADJPER_HIGH_1_RMSK                                                        0x3
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_ADJPER_HIGH_1_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_ADJPER_HIGH_1_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_ADJPER_HIGH_1_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_ADJPER_HIGH_1_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_ADJPER_HIGH_1_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_ADJPER_HIGH_1_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_ADJPER_HIGH_1_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_ADJPER_HIGH_1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_ADJPER_HIGH_1_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_ADJPER_HIGH_1_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_ADJPER_HIGH_1_PLL_DIV_ADJPER_HIGH_1_BMSK                                  0x3
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_ADJPER_HIGH_1_PLL_DIV_ADJPER_HIGH_1_SHFT                                  0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_STEPSIZE_LOW_2_ADDR                                                (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000138)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_STEPSIZE_LOW_2_RMSK                                                      0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_STEPSIZE_LOW_2_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_STEPSIZE_LOW_2_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_STEPSIZE_LOW_2_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_STEPSIZE_LOW_2_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_STEPSIZE_LOW_2_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_STEPSIZE_LOW_2_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_STEPSIZE_LOW_2_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_STEPSIZE_LOW_2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_STEPSIZE_LOW_2_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_STEPSIZE_LOW_2_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_STEPSIZE_LOW_2_PLL_DIV_STEPSIZE_LOW_2_BMSK                               0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_STEPSIZE_LOW_2_PLL_DIV_STEPSIZE_LOW_2_SHFT                                0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_STEPSIZE_HIGH_2_ADDR                                               (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x0000013c)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_STEPSIZE_HIGH_2_RMSK                                                     0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_STEPSIZE_HIGH_2_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_STEPSIZE_HIGH_2_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_STEPSIZE_HIGH_2_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_STEPSIZE_HIGH_2_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_STEPSIZE_HIGH_2_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_STEPSIZE_HIGH_2_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_STEPSIZE_HIGH_2_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_STEPSIZE_HIGH_2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_STEPSIZE_HIGH_2_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_STEPSIZE_HIGH_2_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_STEPSIZE_HIGH_2_PLL_DIV_STEPSIZE_HIGH_2_BMSK                             0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_STEPSIZE_HIGH_2_PLL_DIV_STEPSIZE_HIGH_2_SHFT                              0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_DIV_PER_LOW_2_ADDR                                                 (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000140)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_DIV_PER_LOW_2_RMSK                                                       0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_DIV_PER_LOW_2_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_DIV_PER_LOW_2_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_DIV_PER_LOW_2_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_DIV_PER_LOW_2_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_DIV_PER_LOW_2_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_DIV_PER_LOW_2_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_DIV_PER_LOW_2_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_DIV_PER_LOW_2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_DIV_PER_LOW_2_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_DIV_PER_LOW_2_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_DIV_PER_LOW_2_PLL_DIV_PER_LOW_2_BMSK                                     0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_DIV_PER_LOW_2_PLL_DIV_PER_LOW_2_SHFT                                      0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_DIV_PER_HIGH_2_ADDR                                                (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000144)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_DIV_PER_HIGH_2_RMSK                                                      0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_DIV_PER_HIGH_2_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_DIV_PER_HIGH_2_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_DIV_PER_HIGH_2_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_DIV_PER_HIGH_2_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_DIV_PER_HIGH_2_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_DIV_PER_HIGH_2_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_DIV_PER_HIGH_2_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_DIV_PER_HIGH_2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_DIV_PER_HIGH_2_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_DIV_PER_HIGH_2_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_DIV_PER_HIGH_2_PLL_DIV_PER_HIGH_2_BMSK                                   0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_DIV_PER_HIGH_2_PLL_DIV_PER_HIGH_2_SHFT                                    0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_ADJPER_LOW_2_ADDR                                                  (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000148)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_ADJPER_LOW_2_RMSK                                                        0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_ADJPER_LOW_2_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_ADJPER_LOW_2_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_ADJPER_LOW_2_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_ADJPER_LOW_2_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_ADJPER_LOW_2_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_ADJPER_LOW_2_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_ADJPER_LOW_2_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_ADJPER_LOW_2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_ADJPER_LOW_2_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_ADJPER_LOW_2_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_ADJPER_LOW_2_PLL_DIV_ADJPER_LOW_2_BMSK                                   0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_ADJPER_LOW_2_PLL_DIV_ADJPER_LOW_2_SHFT                                    0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_ADJPER_HIGH_2_ADDR                                                 (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x0000014c)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_ADJPER_HIGH_2_RMSK                                                        0x3
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_ADJPER_HIGH_2_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_ADJPER_HIGH_2_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_ADJPER_HIGH_2_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_ADJPER_HIGH_2_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_ADJPER_HIGH_2_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_ADJPER_HIGH_2_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_ADJPER_HIGH_2_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_ADJPER_HIGH_2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_ADJPER_HIGH_2_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_ADJPER_HIGH_2_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_ADJPER_HIGH_2_PLL_DIV_ADJPER_HIGH_2_BMSK                                  0x3
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_ADJPER_HIGH_2_PLL_DIV_ADJPER_HIGH_2_SHFT                                  0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_CONTROL_ADDR                                                       (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000150)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_CONTROL_RMSK                                                             0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_CONTROL_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_CONTROL_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_CONTROL_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_CONTROL_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_CONTROL_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_CONTROL_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_CONTROL_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_CONTROL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_CONTROL_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_CONTROL_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_CONTROL_SSCSTART_MUX_BMSK                                                0x80
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_CONTROL_SSCSTART_MUX_SHFT                                                 0x7
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_CONTROL_SSCSTART_BMSK                                                    0x40
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_CONTROL_SSCSTART_SHFT                                                     0x6
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_CONTROL_UPDATE_SSC_MUX_BMSK                                              0x20
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_CONTROL_UPDATE_SSC_MUX_SHFT                                               0x5
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_CONTROL_UPDATE_SSC_BMSK                                                  0x10
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_CONTROL_UPDATE_SSC_SHFT                                                   0x4
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_CONTROL_FREQ_UPDATE_MUX_BMSK                                              0x8
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_CONTROL_FREQ_UPDATE_MUX_SHFT                                              0x3
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_CONTROL_FREQ_UPDATE_BMSK                                                  0x4
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_CONTROL_FREQ_UPDATE_SHFT                                                  0x2
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_CONTROL_SSCEN_BMSK                                                        0x2
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_CONTROL_SSCEN_SHFT                                                        0x1
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_CONTROL_SSCCENTER_BMSK                                                    0x1
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SSC_CONTROL_SSCCENTER_SHFT                                                    0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_OUTDIV_RATE_ADDR                                                   (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000154)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_OUTDIV_RATE_RMSK                                                          0xf
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_OUTDIV_RATE_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_OUTDIV_RATE_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_OUTDIV_RATE_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_OUTDIV_RATE_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_OUTDIV_RATE_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_OUTDIV_RATE_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_OUTDIV_RATE_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_OUTDIV_RATE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_OUTDIV_RATE_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_OUTDIV_RATE_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_OUTDIV_RATE_PLL_OUTDIV_2_BMSK                                             0xc
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_OUTDIV_RATE_PLL_OUTDIV_2_SHFT                                             0x2
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_OUTDIV_RATE_PLL_OUTDIV_1_BMSK                                             0x3
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_OUTDIV_RATE_PLL_OUTDIV_1_SHFT                                             0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_LOCKDET_RATE_1_ADDR                                                (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000158)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_LOCKDET_RATE_1_RMSK                                                      0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_LOCKDET_RATE_1_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_LOCKDET_RATE_1_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_LOCKDET_RATE_1_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_LOCKDET_RATE_1_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_LOCKDET_RATE_1_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_LOCKDET_RATE_1_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_LOCKDET_RATE_1_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_LOCKDET_RATE_1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_LOCKDET_RATE_1_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_LOCKDET_RATE_1_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_LOCKDET_RATE_1_PLL_LOCKDET_1_BMSK                                        0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_LOCKDET_RATE_1_PLL_LOCKDET_1_SHFT                                         0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_LOCKDET_RATE_2_ADDR                                                (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x0000015c)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_LOCKDET_RATE_2_RMSK                                                      0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_LOCKDET_RATE_2_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_LOCKDET_RATE_2_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_LOCKDET_RATE_2_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_LOCKDET_RATE_2_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_LOCKDET_RATE_2_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_LOCKDET_RATE_2_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_LOCKDET_RATE_2_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_LOCKDET_RATE_2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_LOCKDET_RATE_2_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_LOCKDET_RATE_2_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_LOCKDET_RATE_2_PLL_LOCKDET_2_BMSK                                        0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_LOCKDET_RATE_2_PLL_LOCKDET_2_SHFT                                         0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_PROP_GAIN_RATE_1_ADDR                                              (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000160)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_PROP_GAIN_RATE_1_RMSK                                                     0xf
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_PROP_GAIN_RATE_1_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_PROP_GAIN_RATE_1_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_PROP_GAIN_RATE_1_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_PROP_GAIN_RATE_1_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_PROP_GAIN_RATE_1_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_PROP_GAIN_RATE_1_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_PROP_GAIN_RATE_1_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_PROP_GAIN_RATE_1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_PROP_GAIN_RATE_1_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_PROP_GAIN_RATE_1_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_PROP_GAIN_RATE_1_PLL_PROP_GAIN_1_BMSK                                     0xf
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_PROP_GAIN_RATE_1_PLL_PROP_GAIN_1_SHFT                                     0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_PROP_GAIN_RATE_2_ADDR                                              (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000164)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_PROP_GAIN_RATE_2_RMSK                                                     0xf
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_PROP_GAIN_RATE_2_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_PROP_GAIN_RATE_2_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_PROP_GAIN_RATE_2_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_PROP_GAIN_RATE_2_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_PROP_GAIN_RATE_2_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_PROP_GAIN_RATE_2_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_PROP_GAIN_RATE_2_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_PROP_GAIN_RATE_2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_PROP_GAIN_RATE_2_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_PROP_GAIN_RATE_2_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_PROP_GAIN_RATE_2_PLL_PROP_GAIN_2_BMSK                                     0xf
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_PROP_GAIN_RATE_2_PLL_PROP_GAIN_2_SHFT                                     0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_BAND_SEL_RATE_1_ADDR                                               (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000168)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_BAND_SEL_RATE_1_RMSK                                                     0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_BAND_SEL_RATE_1_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_BAND_SEL_RATE_1_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_BAND_SEL_RATE_1_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_BAND_SEL_RATE_1_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_BAND_SEL_RATE_1_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_BAND_SEL_RATE_1_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_BAND_SEL_RATE_1_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_BAND_SEL_RATE_1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_BAND_SEL_RATE_1_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_BAND_SEL_RATE_1_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_BAND_SEL_RATE_1_PLL_BAND_SEL_1_BMSK                                      0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_BAND_SEL_RATE_1_PLL_BAND_SEL_1_SHFT                                       0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_BAND_SEL_RATE_2_ADDR                                               (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x0000016c)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_BAND_SEL_RATE_2_RMSK                                                     0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_BAND_SEL_RATE_2_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_BAND_SEL_RATE_2_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_BAND_SEL_RATE_2_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_BAND_SEL_RATE_2_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_BAND_SEL_RATE_2_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_BAND_SEL_RATE_2_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_BAND_SEL_RATE_2_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_BAND_SEL_RATE_2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_BAND_SEL_RATE_2_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_BAND_SEL_RATE_2_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_BAND_SEL_RATE_2_PLL_BAND_SEL_2_BMSK                                      0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_BAND_SEL_RATE_2_PLL_BAND_SEL_2_SHFT                                       0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_INT_GAIN_IFILT_BAND_1_ADDR                                         (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000170)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_INT_GAIN_IFILT_BAND_1_RMSK                                               0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_INT_GAIN_IFILT_BAND_1_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_INT_GAIN_IFILT_BAND_1_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_INT_GAIN_IFILT_BAND_1_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_INT_GAIN_IFILT_BAND_1_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_INT_GAIN_IFILT_BAND_1_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_INT_GAIN_IFILT_BAND_1_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_INT_GAIN_IFILT_BAND_1_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_INT_GAIN_IFILT_BAND_1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_INT_GAIN_IFILT_BAND_1_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_INT_GAIN_IFILT_BAND_1_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_INT_GAIN_IFILT_BAND_1_PLL_IFILT_1_BMSK                                   0xf8
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_INT_GAIN_IFILT_BAND_1_PLL_IFILT_1_SHFT                                    0x3
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_INT_GAIN_IFILT_BAND_1_PLL_INT_GAIN_1_BMSK                                 0x7
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_INT_GAIN_IFILT_BAND_1_PLL_INT_GAIN_1_SHFT                                 0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_INT_GAIN_IFILT_BAND_2_ADDR                                         (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000174)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_INT_GAIN_IFILT_BAND_2_RMSK                                               0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_INT_GAIN_IFILT_BAND_2_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_INT_GAIN_IFILT_BAND_2_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_INT_GAIN_IFILT_BAND_2_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_INT_GAIN_IFILT_BAND_2_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_INT_GAIN_IFILT_BAND_2_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_INT_GAIN_IFILT_BAND_2_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_INT_GAIN_IFILT_BAND_2_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_INT_GAIN_IFILT_BAND_2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_INT_GAIN_IFILT_BAND_2_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_INT_GAIN_IFILT_BAND_2_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_INT_GAIN_IFILT_BAND_2_PLL_IFILT_2_BMSK                                   0xf8
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_INT_GAIN_IFILT_BAND_2_PLL_IFILT_2_SHFT                                    0x3
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_INT_GAIN_IFILT_BAND_2_PLL_INT_GAIN_2_BMSK                                 0x7
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_INT_GAIN_IFILT_BAND_2_PLL_INT_GAIN_2_SHFT                                 0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_FL_INT_GAIN_PFILT_BAND_1_ADDR                                      (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000178)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_FL_INT_GAIN_PFILT_BAND_1_RMSK                                            0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_FL_INT_GAIN_PFILT_BAND_1_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_FL_INT_GAIN_PFILT_BAND_1_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_FL_INT_GAIN_PFILT_BAND_1_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_FL_INT_GAIN_PFILT_BAND_1_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_FL_INT_GAIN_PFILT_BAND_1_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_FL_INT_GAIN_PFILT_BAND_1_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_FL_INT_GAIN_PFILT_BAND_1_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_FL_INT_GAIN_PFILT_BAND_1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_FL_INT_GAIN_PFILT_BAND_1_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_FL_INT_GAIN_PFILT_BAND_1_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_FL_INT_GAIN_PFILT_BAND_1_PLL_PFILT_1_BMSK                                0xf8
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_FL_INT_GAIN_PFILT_BAND_1_PLL_PFILT_1_SHFT                                 0x3
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_FL_INT_GAIN_PFILT_BAND_1_PLL_INT_GAIN_FASTLOCK_1_BMSK                     0x7
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_FL_INT_GAIN_PFILT_BAND_1_PLL_INT_GAIN_FASTLOCK_1_SHFT                     0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_FL_INT_GAIN_PFILT_BAND_2_ADDR                                      (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x0000017c)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_FL_INT_GAIN_PFILT_BAND_2_RMSK                                            0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_FL_INT_GAIN_PFILT_BAND_2_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_FL_INT_GAIN_PFILT_BAND_2_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_FL_INT_GAIN_PFILT_BAND_2_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_FL_INT_GAIN_PFILT_BAND_2_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_FL_INT_GAIN_PFILT_BAND_2_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_FL_INT_GAIN_PFILT_BAND_2_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_FL_INT_GAIN_PFILT_BAND_2_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_FL_INT_GAIN_PFILT_BAND_2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_FL_INT_GAIN_PFILT_BAND_2_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_FL_INT_GAIN_PFILT_BAND_2_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_FL_INT_GAIN_PFILT_BAND_2_PLL_PFILT_2_BMSK                                0xf8
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_FL_INT_GAIN_PFILT_BAND_2_PLL_PFILT_2_SHFT                                 0x3
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_FL_INT_GAIN_PFILT_BAND_2_PLL_INT_GAIN_FASTLOCK_2_BMSK                     0x7
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_FL_INT_GAIN_PFILT_BAND_2_PLL_INT_GAIN_FASTLOCK_2_SHFT                     0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_FASTLOCK_EN_BAND_ADDR                                              (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000180)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_FASTLOCK_EN_BAND_RMSK                                                     0x3
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_FASTLOCK_EN_BAND_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_FASTLOCK_EN_BAND_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_FASTLOCK_EN_BAND_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_FASTLOCK_EN_BAND_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_FASTLOCK_EN_BAND_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_FASTLOCK_EN_BAND_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_FASTLOCK_EN_BAND_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_FASTLOCK_EN_BAND_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_FASTLOCK_EN_BAND_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_FASTLOCK_EN_BAND_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_FASTLOCK_EN_BAND_PLL_FASTLOCK_EN_2_BMSK                                   0x2
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_FASTLOCK_EN_BAND_PLL_FASTLOCK_EN_2_SHFT                                   0x1
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_FASTLOCK_EN_BAND_PLL_FASTLOCK_EN_1_BMSK                                   0x1
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_FASTLOCK_EN_BAND_PLL_FASTLOCK_EN_1_SHFT                                   0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_TUNE_ACCUM_INIT_MID_ADDR                                          (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000184)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_TUNE_ACCUM_INIT_MID_RMSK                                                0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_TUNE_ACCUM_INIT_MID_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_TUNE_ACCUM_INIT_MID_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_TUNE_ACCUM_INIT_MID_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_TUNE_ACCUM_INIT_MID_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_TUNE_ACCUM_INIT_MID_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_TUNE_ACCUM_INIT_MID_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_TUNE_ACCUM_INIT_MID_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_TUNE_ACCUM_INIT_MID_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_TUNE_ACCUM_INIT_MID_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_TUNE_ACCUM_INIT_MID_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_TUNE_ACCUM_INIT_MID_FTUNE_CODE_MID_BMSK                                 0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_TUNE_ACCUM_INIT_MID_FTUNE_CODE_MID_SHFT                                  0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_TUNE_ACCUM_INIT_HIGH_ADDR                                         (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000188)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_TUNE_ACCUM_INIT_HIGH_RMSK                                               0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_TUNE_ACCUM_INIT_HIGH_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_TUNE_ACCUM_INIT_HIGH_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_TUNE_ACCUM_INIT_HIGH_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_TUNE_ACCUM_INIT_HIGH_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_TUNE_ACCUM_INIT_HIGH_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_TUNE_ACCUM_INIT_HIGH_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_TUNE_ACCUM_INIT_HIGH_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_TUNE_ACCUM_INIT_HIGH_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_TUNE_ACCUM_INIT_HIGH_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_TUNE_ACCUM_INIT_HIGH_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_TUNE_ACCUM_INIT_HIGH_FTUNE_CODE_HIGH_BMSK                               0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_TUNE_ACCUM_INIT_HIGH_FTUNE_CODE_HIGH_SHFT                                0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_TUNE_ACCUM_INIT_MUX_ADDR                                          (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x0000018c)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_TUNE_ACCUM_INIT_MUX_RMSK                                                 0x7
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_TUNE_ACCUM_INIT_MUX_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_TUNE_ACCUM_INIT_MUX_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_TUNE_ACCUM_INIT_MUX_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_TUNE_ACCUM_INIT_MUX_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_TUNE_ACCUM_INIT_MUX_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_TUNE_ACCUM_INIT_MUX_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_TUNE_ACCUM_INIT_MUX_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_TUNE_ACCUM_INIT_MUX_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_TUNE_ACCUM_INIT_MUX_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_TUNE_ACCUM_INIT_MUX_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_TUNE_ACCUM_INIT_MUX_FTUNE_CODE_HIGH_MUX_BMSK                             0x4
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_TUNE_ACCUM_INIT_MUX_FTUNE_CODE_HIGH_MUX_SHFT                             0x2
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_TUNE_ACCUM_INIT_MUX_FTUNE_CODE_MID_MUX_BMSK                              0x2
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_TUNE_ACCUM_INIT_MUX_FTUNE_CODE_MID_MUX_SHFT                              0x1
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_TUNE_ACCUM_INIT_MUX_RSVD_BMSK                                            0x1
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FREQ_TUNE_ACCUM_INIT_MUX_RSVD_SHFT                                            0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_LOCK_OVERRIDE_ADDR                                                 (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000190)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_LOCK_OVERRIDE_RMSK                                                       0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_LOCK_OVERRIDE_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_LOCK_OVERRIDE_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_LOCK_OVERRIDE_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_LOCK_OVERRIDE_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_LOCK_OVERRIDE_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_LOCK_OVERRIDE_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_LOCK_OVERRIDE_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_LOCK_OVERRIDE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_LOCK_OVERRIDE_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_LOCK_OVERRIDE_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_LOCK_OVERRIDE_STICKY_LOCK_BMSK                                           0x80
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_LOCK_OVERRIDE_STICKY_LOCK_SHFT                                            0x7
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_LOCK_OVERRIDE_CM_PLL_LOCK_MUX_BMSK                                       0x40
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_LOCK_OVERRIDE_CM_PLL_LOCK_MUX_SHFT                                        0x6
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_LOCK_OVERRIDE_CM_PLL_LOCK_BMSK                                           0x20
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_LOCK_OVERRIDE_CM_PLL_LOCK_SHFT                                            0x5
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_LOCK_OVERRIDE_PLL_LOCK_SELECT_BMSK                                       0x10
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_LOCK_OVERRIDE_PLL_LOCK_SELECT_SHFT                                        0x4
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_LOCK_OVERRIDE_PLL_RELOCK_MUX_BMSK                                         0x8
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_LOCK_OVERRIDE_PLL_RELOCK_MUX_SHFT                                         0x3
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_LOCK_OVERRIDE_PLL_RELOCK_BMSK                                             0x4
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_LOCK_OVERRIDE_PLL_RELOCK_SHFT                                             0x2
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_LOCK_OVERRIDE_CM_PLL_LOCKOUT_OUT_MUX_BMSK                                 0x2
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_LOCK_OVERRIDE_CM_PLL_LOCKOUT_OUT_MUX_SHFT                                 0x1
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_LOCK_OVERRIDE_CM_PLL_LOCKOUT_OUT_BMSK                                     0x1
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_LOCK_OVERRIDE_CM_PLL_LOCKOUT_OUT_SHFT                                     0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_LOCK_DELAY_ADDR                                                    (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000194)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_LOCK_DELAY_RMSK                                                          0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_LOCK_DELAY_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_LOCK_DELAY_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_LOCK_DELAY_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_LOCK_DELAY_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_LOCK_DELAY_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_LOCK_DELAY_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_LOCK_DELAY_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_LOCK_DELAY_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_LOCK_DELAY_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_LOCK_DELAY_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_LOCK_DELAY_LOCK_DELAY_BMSK                                               0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_LOCK_DELAY_LOCK_DELAY_SHFT                                                0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_LOCK_MIN_DELAY_ADDR                                                (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000198)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_LOCK_MIN_DELAY_RMSK                                                      0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_LOCK_MIN_DELAY_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_LOCK_MIN_DELAY_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_LOCK_MIN_DELAY_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_LOCK_MIN_DELAY_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_LOCK_MIN_DELAY_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_LOCK_MIN_DELAY_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_LOCK_MIN_DELAY_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_LOCK_MIN_DELAY_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_LOCK_MIN_DELAY_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_LOCK_MIN_DELAY_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_LOCK_MIN_DELAY_LOCK_MIN_DLY_BMSK                                         0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_LOCK_MIN_DELAY_LOCK_MIN_DLY_SHFT                                          0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_CLOCK_INVERTERS_ADDR                                                   (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x0000019c)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CLOCK_INVERTERS_RMSK                                                         0x7b
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CLOCK_INVERTERS_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_CLOCK_INVERTERS_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_CLOCK_INVERTERS_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CLOCK_INVERTERS_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_CLOCK_INVERTERS_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CLOCK_INVERTERS_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_CLOCK_INVERTERS_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CLOCK_INVERTERS_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_CLOCK_INVERTERS_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_CLOCK_INVERTERS_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CLOCK_INVERTERS_CM_OSCBIAS_ADJ_BMSK                                          0x60
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CLOCK_INVERTERS_CM_OSCBIAS_ADJ_SHFT                                           0x5
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CLOCK_INVERTERS_CM_OSCCAP_EN_BMSK                                            0x18
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CLOCK_INVERTERS_CM_OSCCAP_EN_SHFT                                             0x3
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CLOCK_INVERTERS_INVERT_DSMCLK_BMSK                                            0x2
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CLOCK_INVERTERS_INVERT_DSMCLK_SHFT                                            0x1
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CLOCK_INVERTERS_INVERT_FRACNCLK_BMSK                                          0x1
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CLOCK_INVERTERS_INVERT_FRACNCLK_SHFT                                          0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_SPARE_AND_JPC_OVERRIDES_ADDR                                           (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x000001a0)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SPARE_AND_JPC_OVERRIDES_RMSK                                                  0xf
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SPARE_AND_JPC_OVERRIDES_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_SPARE_AND_JPC_OVERRIDES_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_SPARE_AND_JPC_OVERRIDES_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SPARE_AND_JPC_OVERRIDES_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_SPARE_AND_JPC_OVERRIDES_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SPARE_AND_JPC_OVERRIDES_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_SPARE_AND_JPC_OVERRIDES_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SPARE_AND_JPC_OVERRIDES_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_SPARE_AND_JPC_OVERRIDES_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_SPARE_AND_JPC_OVERRIDES_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SPARE_AND_JPC_OVERRIDES_CM_PLL_SPARE_A_3_BMSK                                 0x8
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SPARE_AND_JPC_OVERRIDES_CM_PLL_SPARE_A_3_SHFT                                 0x3
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SPARE_AND_JPC_OVERRIDES_CM_PLL_SPARE_A_2_BMSK                                 0x4
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SPARE_AND_JPC_OVERRIDES_CM_PLL_SPARE_A_2_SHFT                                 0x2
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SPARE_AND_JPC_OVERRIDES_CM_PLL_SPARE_A_1_BMSK                                 0x2
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SPARE_AND_JPC_OVERRIDES_CM_PLL_SPARE_A_1_SHFT                                 0x1
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SPARE_AND_JPC_OVERRIDES_CM_PLL_SPARE_A_0_BMSK                                 0x1
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SPARE_AND_JPC_OVERRIDES_CM_PLL_SPARE_A_0_SHFT                                 0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_BIAS_CONTROL_1_ADDR                                                    (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x000001a4)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BIAS_CONTROL_1_RMSK                                                          0x7f
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BIAS_CONTROL_1_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_BIAS_CONTROL_1_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_BIAS_CONTROL_1_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BIAS_CONTROL_1_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_BIAS_CONTROL_1_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BIAS_CONTROL_1_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_BIAS_CONTROL_1_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BIAS_CONTROL_1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_BIAS_CONTROL_1_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_BIAS_CONTROL_1_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BIAS_CONTROL_1_CM_BIAS_IE_CTRL_BMSK                                          0x7e
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BIAS_CONTROL_1_CM_BIAS_IE_CTRL_SHFT                                           0x1
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BIAS_CONTROL_1_CM_BIAS_SEL_BMSK                                               0x1
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BIAS_CONTROL_1_CM_BIAS_SEL_SHFT                                               0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_BIAS_CONTROL_2_ADDR                                                    (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x000001a8)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BIAS_CONTROL_2_RMSK                                                          0x3f
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BIAS_CONTROL_2_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_BIAS_CONTROL_2_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_BIAS_CONTROL_2_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BIAS_CONTROL_2_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_BIAS_CONTROL_2_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BIAS_CONTROL_2_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_BIAS_CONTROL_2_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BIAS_CONTROL_2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_BIAS_CONTROL_2_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_BIAS_CONTROL_2_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BIAS_CONTROL_2_CM_BIAS_IP_CTRL_BMSK                                          0x3f
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BIAS_CONTROL_2_CM_BIAS_IP_CTRL_SHFT                                           0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_ALOG_OBSV_BUS_CTRL_1_ADDR                                              (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x000001ac)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ALOG_OBSV_BUS_CTRL_1_RMSK                                                    0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ALOG_OBSV_BUS_CTRL_1_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_ALOG_OBSV_BUS_CTRL_1_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_ALOG_OBSV_BUS_CTRL_1_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ALOG_OBSV_BUS_CTRL_1_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_ALOG_OBSV_BUS_CTRL_1_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ALOG_OBSV_BUS_CTRL_1_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_ALOG_OBSV_BUS_CTRL_1_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ALOG_OBSV_BUS_CTRL_1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_ALOG_OBSV_BUS_CTRL_1_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_ALOG_OBSV_BUS_CTRL_1_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ALOG_OBSV_BUS_CTRL_1_ALOG_OBSV_BUS_CTRL_1_SEL_BMSK                           0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ALOG_OBSV_BUS_CTRL_1_ALOG_OBSV_BUS_CTRL_1_SEL_SHFT                            0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_COMMON_STATUS_ONE_ADDR                                                 (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x000001b0)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_COMMON_STATUS_ONE_RMSK                                                       0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_COMMON_STATUS_ONE_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_COMMON_STATUS_ONE_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_COMMON_STATUS_ONE_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_COMMON_STATUS_ONE_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_COMMON_STATUS_ONE_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_COMMON_STATUS_ONE_UNLOCKED_STATUS_BMSK                                       0x80
#define HWIO_DSI_1_PHY_DSIPHY_PLL_COMMON_STATUS_ONE_UNLOCKED_STATUS_SHFT                                        0x7
#define HWIO_DSI_1_PHY_DSIPHY_PLL_COMMON_STATUS_ONE_PLL_PWRDN_STATUS_BMSK                                      0x40
#define HWIO_DSI_1_PHY_DSIPHY_PLL_COMMON_STATUS_ONE_PLL_PWRDN_STATUS_SHFT                                       0x6
#define HWIO_DSI_1_PHY_DSIPHY_PLL_COMMON_STATUS_ONE_PLL_RST_STATUS_BMSK                                        0x20
#define HWIO_DSI_1_PHY_DSIPHY_PLL_COMMON_STATUS_ONE_PLL_RST_STATUS_SHFT                                         0x5
#define HWIO_DSI_1_PHY_DSIPHY_PLL_COMMON_STATUS_ONE_PLL_LOCKOUT_STATUS_BMSK                                    0x10
#define HWIO_DSI_1_PHY_DSIPHY_PLL_COMMON_STATUS_ONE_PLL_LOCKOUT_STATUS_SHFT                                     0x4
#define HWIO_DSI_1_PHY_DSIPHY_PLL_COMMON_STATUS_ONE_CORE_PLL_RATE_STATUS_BMSK                                   0xc
#define HWIO_DSI_1_PHY_DSIPHY_PLL_COMMON_STATUS_ONE_CORE_PLL_RATE_STATUS_SHFT                                   0x2
#define HWIO_DSI_1_PHY_DSIPHY_PLL_COMMON_STATUS_ONE_CORE_PLL_EN_STATUS_BMSK                                     0x2
#define HWIO_DSI_1_PHY_DSIPHY_PLL_COMMON_STATUS_ONE_CORE_PLL_EN_STATUS_SHFT                                     0x1
#define HWIO_DSI_1_PHY_DSIPHY_PLL_COMMON_STATUS_ONE_CORE_READY_STATUS_BMSK                                      0x1
#define HWIO_DSI_1_PHY_DSIPHY_PLL_COMMON_STATUS_ONE_CORE_READY_STATUS_SHFT                                      0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_COMMON_STATUS_TWO_ADDR                                                 (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x000001b4)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_COMMON_STATUS_TWO_RMSK                                                       0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_COMMON_STATUS_TWO_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_COMMON_STATUS_TWO_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_COMMON_STATUS_TWO_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_COMMON_STATUS_TWO_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_COMMON_STATUS_TWO_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_COMMON_STATUS_TWO_PSM_PWR_STATE_INT_BMSK                                     0xe0
#define HWIO_DSI_1_PHY_DSIPHY_PLL_COMMON_STATUS_TWO_PSM_PWR_STATE_INT_SHFT                                      0x5
#define HWIO_DSI_1_PHY_DSIPHY_PLL_COMMON_STATUS_TWO_CALIBRATION_CNT_BMSK                                       0x18
#define HWIO_DSI_1_PHY_DSIPHY_PLL_COMMON_STATUS_TWO_CALIBRATION_CNT_SHFT                                        0x3
#define HWIO_DSI_1_PHY_DSIPHY_PLL_COMMON_STATUS_TWO_PLL_PWRDN_EXT_BMSK                                          0x4
#define HWIO_DSI_1_PHY_DSIPHY_PLL_COMMON_STATUS_TWO_PLL_PWRDN_EXT_SHFT                                          0x2
#define HWIO_DSI_1_PHY_DSIPHY_PLL_COMMON_STATUS_TWO_FD_LOCK_STATUS_BMSK                                         0x2
#define HWIO_DSI_1_PHY_DSIPHY_PLL_COMMON_STATUS_TWO_FD_LOCK_STATUS_SHFT                                         0x1
#define HWIO_DSI_1_PHY_DSIPHY_PLL_COMMON_STATUS_TWO_BAND_SEL_CAL_LOCKED_STATUS_BMSK                             0x1
#define HWIO_DSI_1_PHY_DSIPHY_PLL_COMMON_STATUS_TWO_BAND_SEL_CAL_LOCKED_STATUS_SHFT                             0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_ADDR                                                      (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x000001b8)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_RMSK                                                            0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_CALIBRATED_BAND_SEL_BMSK                                        0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_BAND_SEL_CAL_CALIBRATED_BAND_SEL_SHFT                                         0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_ICODE_ACCUM_STATUS_LOW_ADDR                                            (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x000001bc)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ICODE_ACCUM_STATUS_LOW_RMSK                                                  0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ICODE_ACCUM_STATUS_LOW_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_ICODE_ACCUM_STATUS_LOW_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_ICODE_ACCUM_STATUS_LOW_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ICODE_ACCUM_STATUS_LOW_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_ICODE_ACCUM_STATUS_LOW_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ICODE_ACCUM_STATUS_LOW_FTUNE_CODE_STATUS_7_0_BMSK                            0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ICODE_ACCUM_STATUS_LOW_FTUNE_CODE_STATUS_7_0_SHFT                             0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_ICODE_ACCUM_STATUS_HIGH_ADDR                                           (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x000001c0)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ICODE_ACCUM_STATUS_HIGH_RMSK                                                 0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ICODE_ACCUM_STATUS_HIGH_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_ICODE_ACCUM_STATUS_HIGH_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_ICODE_ACCUM_STATUS_HIGH_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ICODE_ACCUM_STATUS_HIGH_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_ICODE_ACCUM_STATUS_HIGH_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ICODE_ACCUM_STATUS_HIGH_FTUNE_CODE_STATUS_15_8_BMSK                          0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ICODE_ACCUM_STATUS_HIGH_FTUNE_CODE_STATUS_15_8_SHFT                           0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_FD_OUT_LOW_ADDR                                                        (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x000001c4)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FD_OUT_LOW_RMSK                                                              0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FD_OUT_LOW_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_FD_OUT_LOW_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_FD_OUT_LOW_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FD_OUT_LOW_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_FD_OUT_LOW_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FD_OUT_LOW_FD_OUT_7_0_BMSK                                                   0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FD_OUT_LOW_FD_OUT_7_0_SHFT                                                    0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_FD_OUT_HIGH_ADDR                                                       (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x000001c8)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FD_OUT_HIGH_RMSK                                                             0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FD_OUT_HIGH_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_FD_OUT_HIGH_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_FD_OUT_HIGH_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FD_OUT_HIGH_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_FD_OUT_HIGH_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FD_OUT_HIGH_FD_OUT_15_8_BMSK                                                 0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FD_OUT_HIGH_FD_OUT_15_8_SHFT                                                  0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_ALOG_OBSV_BUS_STATUS_1_ADDR                                            (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x000001cc)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ALOG_OBSV_BUS_STATUS_1_RMSK                                                  0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ALOG_OBSV_BUS_STATUS_1_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_ALOG_OBSV_BUS_STATUS_1_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_ALOG_OBSV_BUS_STATUS_1_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ALOG_OBSV_BUS_STATUS_1_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_ALOG_OBSV_BUS_STATUS_1_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ALOG_OBSV_BUS_STATUS_1_ALOG_OBSV_BUS_STATUS_1_BMSK                           0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ALOG_OBSV_BUS_STATUS_1_ALOG_OBSV_BUS_STATUS_1_SHFT                            0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_MISC_CONFIG_ADDR                                                   (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x000001d0)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_MISC_CONFIG_RMSK                                                         0x3f
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_MISC_CONFIG_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_MISC_CONFIG_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_MISC_CONFIG_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_MISC_CONFIG_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_MISC_CONFIG_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_MISC_CONFIG_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_MISC_CONFIG_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_MISC_CONFIG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_MISC_CONFIG_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_MISC_CONFIG_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_MISC_CONFIG_FLL_WAITTIME_FGCAL_BMSK                                      0x30
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_MISC_CONFIG_FLL_WAITTIME_FGCAL_SHFT                                       0x4
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_MISC_CONFIG_FLL_WAITTIME_BGCAL_BMSK                                       0xc
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_MISC_CONFIG_FLL_WAITTIME_BGCAL_SHFT                                       0x2
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_MISC_CONFIG_FLL_FREEZE_CODE_BMSK                                          0x2
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_MISC_CONFIG_FLL_FREEZE_CODE_SHFT                                          0x1
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_MISC_CONFIG_ICODE_PRECAL_EN_BMSK                                          0x1
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_MISC_CONFIG_ICODE_PRECAL_EN_SHFT                                          0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_FLL_CONFIG_ADDR                                                        (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x000001d4)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FLL_CONFIG_RMSK                                                              0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FLL_CONFIG_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_FLL_CONFIG_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_FLL_CONFIG_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FLL_CONFIG_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_FLL_CONFIG_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FLL_CONFIG_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_FLL_CONFIG_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FLL_CONFIG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_FLL_CONFIG_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_FLL_CONFIG_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FLL_CONFIG_FLL_CFG_MISC_BMSK                                                 0xe0
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FLL_CONFIG_FLL_CFG_MISC_SHFT                                                  0x5
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FLL_CONFIG_FLL_EN_CONT_CODE_UPDATE_BMSK                                      0x10
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FLL_CONFIG_FLL_EN_CONT_CODE_UPDATE_SHFT                                       0x4
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FLL_CONFIG_FLL_CNT_VAL_TOL_BMSK                                               0xe
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FLL_CONFIG_FLL_CNT_VAL_TOL_SHFT                                               0x1
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FLL_CONFIG_FLL_MODE_BMSK                                                      0x1
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FLL_CONFIG_FLL_MODE_SHFT                                                      0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_FLL_FREQ_ACQ_TIME_ADDR                                                 (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x000001d8)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FLL_FREQ_ACQ_TIME_RMSK                                                       0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FLL_FREQ_ACQ_TIME_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_FLL_FREQ_ACQ_TIME_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_FLL_FREQ_ACQ_TIME_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FLL_FREQ_ACQ_TIME_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_FLL_FREQ_ACQ_TIME_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FLL_FREQ_ACQ_TIME_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_FLL_FREQ_ACQ_TIME_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FLL_FREQ_ACQ_TIME_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_FLL_FREQ_ACQ_TIME_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_FLL_FREQ_ACQ_TIME_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FLL_FREQ_ACQ_TIME_FLL_FREQ_ACQ_CYC_BG_BMSK                                   0xf0
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FLL_FREQ_ACQ_TIME_FLL_FREQ_ACQ_CYC_BG_SHFT                                    0x4
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FLL_FREQ_ACQ_TIME_FLL_FREQ_ACQ_CYC_FG_BMSK                                    0xf
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FLL_FREQ_ACQ_TIME_FLL_FREQ_ACQ_CYC_FG_SHFT                                    0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_FLL_CODE0_ADDR                                                         (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x000001dc)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FLL_CODE0_RMSK                                                               0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FLL_CODE0_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_FLL_CODE0_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_FLL_CODE0_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FLL_CODE0_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_FLL_CODE0_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FLL_CODE0_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_FLL_CODE0_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FLL_CODE0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_FLL_CODE0_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_FLL_CODE0_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FLL_CODE0_FLL_CODE_7_0_BMSK                                                  0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FLL_CODE0_FLL_CODE_7_0_SHFT                                                   0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_FLL_CODE1_ADDR                                                         (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x000001e0)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FLL_CODE1_RMSK                                                               0x13
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FLL_CODE1_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_FLL_CODE1_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_FLL_CODE1_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FLL_CODE1_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_FLL_CODE1_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FLL_CODE1_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_FLL_CODE1_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FLL_CODE1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_FLL_CODE1_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_FLL_CODE1_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FLL_CODE1_FLL_CODE_MUX_BMSK                                                  0x10
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FLL_CODE1_FLL_CODE_MUX_SHFT                                                   0x4
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FLL_CODE1_FLL_CODE_9_8_BMSK                                                   0x3
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FLL_CODE1_FLL_CODE_9_8_SHFT                                                   0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_FLL_GAIN0_ADDR                                                         (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x000001e4)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FLL_GAIN0_RMSK                                                               0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FLL_GAIN0_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_FLL_GAIN0_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_FLL_GAIN0_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FLL_GAIN0_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_FLL_GAIN0_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FLL_GAIN0_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_FLL_GAIN0_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FLL_GAIN0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_FLL_GAIN0_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_FLL_GAIN0_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FLL_GAIN0_FLL_GAIN_7_0_BMSK                                                  0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FLL_GAIN0_FLL_GAIN_7_0_SHFT                                                   0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_FLL_GAIN1_ADDR                                                         (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x000001e8)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FLL_GAIN1_RMSK                                                                0xf
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FLL_GAIN1_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_FLL_GAIN1_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_FLL_GAIN1_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FLL_GAIN1_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_FLL_GAIN1_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FLL_GAIN1_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_FLL_GAIN1_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FLL_GAIN1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_FLL_GAIN1_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_FLL_GAIN1_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FLL_GAIN1_FLL_GAIN_11_8_BMSK                                                  0xf
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FLL_GAIN1_FLL_GAIN_11_8_SHFT                                                  0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_SW_RESET_ADDR                                                          (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x000001ec)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SW_RESET_RMSK                                                                 0x1
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SW_RESET_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_SW_RESET_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_SW_RESET_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SW_RESET_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_SW_RESET_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SW_RESET_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_SW_RESET_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SW_RESET_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_SW_RESET_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_SW_RESET_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SW_RESET_SW_RESET_BMSK                                                        0x1
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SW_RESET_SW_RESET_SHFT                                                        0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_FAST_PWRUP_ADDR                                                        (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x000001f0)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FAST_PWRUP_RMSK                                                              0x3f
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FAST_PWRUP_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_FAST_PWRUP_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_FAST_PWRUP_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FAST_PWRUP_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_FAST_PWRUP_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FAST_PWRUP_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_FAST_PWRUP_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FAST_PWRUP_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_FAST_PWRUP_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_FAST_PWRUP_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FAST_PWRUP_FLL_RUN_BGCAL_MUX_BMSK                                            0x20
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FAST_PWRUP_FLL_RUN_BGCAL_MUX_SHFT                                             0x5
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FAST_PWRUP_FLL_RUN_BGCAL_BMSK                                                0x10
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FAST_PWRUP_FLL_RUN_BGCAL_SHFT                                                 0x4
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FAST_PWRUP_FLL_RUN_FGCAL_MUX_BMSK                                             0x8
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FAST_PWRUP_FLL_RUN_FGCAL_MUX_SHFT                                             0x3
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FAST_PWRUP_FLL_RUN_FGCAL_BMSK                                                 0x4
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FAST_PWRUP_FLL_RUN_FGCAL_SHFT                                                 0x2
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FAST_PWRUP_FLL_CAL_EN_MUX_BMSK                                                0x2
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FAST_PWRUP_FLL_CAL_EN_MUX_SHFT                                                0x1
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FAST_PWRUP_FLL_CAL_EN_BMSK                                                    0x1
#define HWIO_DSI_1_PHY_DSIPHY_PLL_FAST_PWRUP_FLL_CAL_EN_SHFT                                                    0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_LOCKTIME0_ADDR                                                         (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x000001f4)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_LOCKTIME0_RMSK                                                               0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_LOCKTIME0_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_LOCKTIME0_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_LOCKTIME0_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_LOCKTIME0_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_LOCKTIME0_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_LOCKTIME0_LOCKTIME_7_0_BMSK                                                  0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_LOCKTIME0_LOCKTIME_7_0_SHFT                                                   0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_LOCKTIME1_ADDR                                                         (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x000001f8)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_LOCKTIME1_RMSK                                                               0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_LOCKTIME1_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_LOCKTIME1_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_LOCKTIME1_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_LOCKTIME1_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_LOCKTIME1_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_LOCKTIME1_LOCKTIME_15_8_BMSK                                                 0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_LOCKTIME1_LOCKTIME_15_8_SHFT                                                  0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_DEBUG_BUS_SEL_ADDR                                                     (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x000001fc)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_DEBUG_BUS_SEL_RMSK                                                            0xf
#define HWIO_DSI_1_PHY_DSIPHY_PLL_DEBUG_BUS_SEL_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_DEBUG_BUS_SEL_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_DEBUG_BUS_SEL_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_DEBUG_BUS_SEL_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_DEBUG_BUS_SEL_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_DEBUG_BUS_SEL_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_DEBUG_BUS_SEL_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_DEBUG_BUS_SEL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_DEBUG_BUS_SEL_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_DEBUG_BUS_SEL_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_DEBUG_BUS_SEL_DEBUG_BUS_SEL_BMSK                                              0xf
#define HWIO_DSI_1_PHY_DSIPHY_PLL_DEBUG_BUS_SEL_DEBUG_BUS_SEL_SHFT                                              0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_DEBUG_BUS0_ADDR                                                        (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000200)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_DEBUG_BUS0_RMSK                                                              0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_DEBUG_BUS0_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_DEBUG_BUS0_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_DEBUG_BUS0_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_DEBUG_BUS0_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_DEBUG_BUS0_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_DEBUG_BUS0_DEBUG_BUS_7_0_BMSK                                                0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_DEBUG_BUS0_DEBUG_BUS_7_0_SHFT                                                 0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_DEBUG_BUS1_ADDR                                                        (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000204)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_DEBUG_BUS1_RMSK                                                              0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_DEBUG_BUS1_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_DEBUG_BUS1_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_DEBUG_BUS1_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_DEBUG_BUS1_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_DEBUG_BUS1_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_DEBUG_BUS1_DEBUG_BUS_15_8_BMSK                                               0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_DEBUG_BUS1_DEBUG_BUS_15_8_SHFT                                                0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_DEBUG_BUS2_ADDR                                                        (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000208)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_DEBUG_BUS2_RMSK                                                              0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_DEBUG_BUS2_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_DEBUG_BUS2_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_DEBUG_BUS2_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_DEBUG_BUS2_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_DEBUG_BUS2_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_DEBUG_BUS2_DEBUG_BUS_23_16_BMSK                                              0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_DEBUG_BUS2_DEBUG_BUS_23_16_SHFT                                               0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_DEBUG_BUS3_ADDR                                                        (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x0000020c)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_DEBUG_BUS3_RMSK                                                              0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_DEBUG_BUS3_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_DEBUG_BUS3_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_DEBUG_BUS3_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_DEBUG_BUS3_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_DEBUG_BUS3_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_DEBUG_BUS3_DEBUG_BUS_31_24_BMSK                                              0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_DEBUG_BUS3_DEBUG_BUS_31_24_SHFT                                               0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_FLL_CONTROL_OVERRIDES_ADDR                                      (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000210)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_FLL_CONTROL_OVERRIDES_RMSK                                            0x77
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_FLL_CONTROL_OVERRIDES_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_FLL_CONTROL_OVERRIDES_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_FLL_CONTROL_OVERRIDES_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_FLL_CONTROL_OVERRIDES_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_FLL_CONTROL_OVERRIDES_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_FLL_CONTROL_OVERRIDES_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_FLL_CONTROL_OVERRIDES_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_FLL_CONTROL_OVERRIDES_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_FLL_CONTROL_OVERRIDES_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_FLL_CONTROL_OVERRIDES_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_FLL_CONTROL_OVERRIDES_FLL_MODE_JPC_MUX_BMSK                           0x40
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_FLL_CONTROL_OVERRIDES_FLL_MODE_JPC_MUX_SHFT                            0x6
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_FLL_CONTROL_OVERRIDES_FLL_DIV_RST_MUX_BMSK                            0x20
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_FLL_CONTROL_OVERRIDES_FLL_DIV_RST_MUX_SHFT                             0x5
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_FLL_CONTROL_OVERRIDES_FLL_PFD_TDC_RST_MUX_BMSK                        0x10
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_FLL_CONTROL_OVERRIDES_FLL_PFD_TDC_RST_MUX_SHFT                         0x4
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_FLL_CONTROL_OVERRIDES_FLL_MODE_JPC_BMSK                                0x4
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_FLL_CONTROL_OVERRIDES_FLL_MODE_JPC_SHFT                                0x2
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_FLL_CONTROL_OVERRIDES_FLL_DIV_RST_BMSK                                 0x2
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_FLL_CONTROL_OVERRIDES_FLL_DIV_RST_SHFT                                 0x1
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_FLL_CONTROL_OVERRIDES_FLL_PFD_TDC_RST_BMSK                             0x1
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_FLL_CONTROL_OVERRIDES_FLL_PFD_TDC_RST_SHFT                             0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_VCO_CONFIG_ADDR                                                        (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000214)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_VCO_CONFIG_RMSK                                                              0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_VCO_CONFIG_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_VCO_CONFIG_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_VCO_CONFIG_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_VCO_CONFIG_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_VCO_CONFIG_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_VCO_CONFIG_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_VCO_CONFIG_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_VCO_CONFIG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_VCO_CONFIG_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_VCO_CONFIG_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_VCO_CONFIG_VCO_CONFIG_BMSK                                                   0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_VCO_CONFIG_VCO_CONFIG_SHFT                                                    0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_VCO_CAL_CODE1_MODE0_STATUS_ADDR                                        (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000218)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_VCO_CAL_CODE1_MODE0_STATUS_RMSK                                              0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_VCO_CAL_CODE1_MODE0_STATUS_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_VCO_CAL_CODE1_MODE0_STATUS_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_VCO_CAL_CODE1_MODE0_STATUS_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_VCO_CAL_CODE1_MODE0_STATUS_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_VCO_CAL_CODE1_MODE0_STATUS_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_VCO_CAL_CODE1_MODE0_STATUS_PSM_VCOCAL_CODE_MODE0_7_0_BMSK                    0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_VCO_CAL_CODE1_MODE0_STATUS_PSM_VCOCAL_CODE_MODE0_7_0_SHFT                     0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_VCO_CAL_CODE1_MODE1_STATUS_ADDR                                        (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x0000021c)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_VCO_CAL_CODE1_MODE1_STATUS_RMSK                                              0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_VCO_CAL_CODE1_MODE1_STATUS_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_VCO_CAL_CODE1_MODE1_STATUS_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_VCO_CAL_CODE1_MODE1_STATUS_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_VCO_CAL_CODE1_MODE1_STATUS_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_VCO_CAL_CODE1_MODE1_STATUS_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_VCO_CAL_CODE1_MODE1_STATUS_PSM_VCOCAL_CODE_MODE1_7_0_BMSK                    0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_VCO_CAL_CODE1_MODE1_STATUS_PSM_VCOCAL_CODE_MODE1_7_0_SHFT                     0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_RESET_SM_STATUS_ADDR                                                   (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000220)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_RESET_SM_STATUS_RMSK                                                         0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_RESET_SM_STATUS_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_RESET_SM_STATUS_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_RESET_SM_STATUS_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_RESET_SM_STATUS_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_RESET_SM_STATUS_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_RESET_SM_STATUS_FIRST_DIR_BMSK                                               0x80
#define HWIO_DSI_1_PHY_DSIPHY_PLL_RESET_SM_STATUS_FIRST_DIR_SHFT                                                0x7
#define HWIO_DSI_1_PHY_DSIPHY_PLL_RESET_SM_STATUS_BAS_STATE_BMSK                                               0x70
#define HWIO_DSI_1_PHY_DSIPHY_PLL_RESET_SM_STATUS_BAS_STATE_SHFT                                                0x4
#define HWIO_DSI_1_PHY_DSIPHY_PLL_RESET_SM_STATUS_RESETSM_BMSK                                                  0xf
#define HWIO_DSI_1_PHY_DSIPHY_PLL_RESET_SM_STATUS_RESETSM_SHFT                                                  0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_TDC_OFFSET_ADDR                                                        (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000224)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_TDC_OFFSET_RMSK                                                              0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_TDC_OFFSET_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_TDC_OFFSET_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_TDC_OFFSET_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_TDC_OFFSET_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_TDC_OFFSET_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_TDC_OFFSET_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_TDC_OFFSET_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_TDC_OFFSET_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_TDC_OFFSET_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_TDC_OFFSET_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_TDC_OFFSET_TDC_OFFSET_BMSK                                                   0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_TDC_OFFSET_TDC_OFFSET_SHFT                                                    0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_PS3_PWRDOWN_CONTROLS_ADDR                                              (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000228)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PS3_PWRDOWN_CONTROLS_RMSK                                                    0x1f
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PS3_PWRDOWN_CONTROLS_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_PS3_PWRDOWN_CONTROLS_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_PS3_PWRDOWN_CONTROLS_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PS3_PWRDOWN_CONTROLS_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_PS3_PWRDOWN_CONTROLS_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PS3_PWRDOWN_CONTROLS_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_PS3_PWRDOWN_CONTROLS_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PS3_PWRDOWN_CONTROLS_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_PS3_PWRDOWN_CONTROLS_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_PS3_PWRDOWN_CONTROLS_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PS3_PWRDOWN_CONTROLS_PS3_CLK_DRV_GATEOFF_BMSK                                0x10
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PS3_PWRDOWN_CONTROLS_PS3_CLK_DRV_GATEOFF_SHFT                                 0x4
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PS3_PWRDOWN_CONTROLS_PS3_PLL_RST_BMSK                                         0x8
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PS3_PWRDOWN_CONTROLS_PS3_PLL_RST_SHFT                                         0x3
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PS3_PWRDOWN_CONTROLS_PS3_DLF_RST_BMSK                                         0x4
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PS3_PWRDOWN_CONTROLS_PS3_DLF_RST_SHFT                                         0x2
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PS3_PWRDOWN_CONTROLS_PS3_EN_PLL_BMSK                                          0x2
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PS3_PWRDOWN_CONTROLS_PS3_EN_PLL_SHFT                                          0x1
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PS3_PWRDOWN_CONTROLS_PS3_BIAS_EN_BMSK                                         0x1
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PS3_PWRDOWN_CONTROLS_PS3_BIAS_EN_SHFT                                         0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_PS4_PWRDOWN_CONTROLS_ADDR                                              (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x0000022c)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PS4_PWRDOWN_CONTROLS_RMSK                                                    0x1f
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PS4_PWRDOWN_CONTROLS_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_PS4_PWRDOWN_CONTROLS_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_PS4_PWRDOWN_CONTROLS_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PS4_PWRDOWN_CONTROLS_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_PS4_PWRDOWN_CONTROLS_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PS4_PWRDOWN_CONTROLS_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_PS4_PWRDOWN_CONTROLS_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PS4_PWRDOWN_CONTROLS_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_PS4_PWRDOWN_CONTROLS_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_PS4_PWRDOWN_CONTROLS_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PS4_PWRDOWN_CONTROLS_PS4_CLK_DRV_GATEOFF_BMSK                                0x10
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PS4_PWRDOWN_CONTROLS_PS4_CLK_DRV_GATEOFF_SHFT                                 0x4
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PS4_PWRDOWN_CONTROLS_PS4_PLL_RST_BMSK                                         0x8
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PS4_PWRDOWN_CONTROLS_PS4_PLL_RST_SHFT                                         0x3
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PS4_PWRDOWN_CONTROLS_PS4_DLF_RST_BMSK                                         0x4
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PS4_PWRDOWN_CONTROLS_PS4_DLF_RST_SHFT                                         0x2
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PS4_PWRDOWN_CONTROLS_PS4_EN_PLL_BMSK                                          0x2
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PS4_PWRDOWN_CONTROLS_PS4_EN_PLL_SHFT                                          0x1
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PS4_PWRDOWN_CONTROLS_PS4_BIAS_EN_BMSK                                         0x1
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PS4_PWRDOWN_CONTROLS_PS4_BIAS_EN_SHFT                                         0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_RST_CONTROLS_ADDR                                                  (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000230)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_RST_CONTROLS_RMSK                                                        0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_RST_CONTROLS_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_RST_CONTROLS_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_RST_CONTROLS_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_RST_CONTROLS_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_RST_CONTROLS_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_RST_CONTROLS_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_RST_CONTROLS_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_RST_CONTROLS_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_RST_CONTROLS_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_RST_CONTROLS_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_RST_CONTROLS_PLL_RST_RLS_DLY_BMSK                                        0xf0
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_RST_CONTROLS_PLL_RST_RLS_DLY_SHFT                                         0x4
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_RST_CONTROLS_PLL_PWRDN_RLS_DLY_BMSK                                       0xf
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PLL_RST_CONTROLS_PLL_PWRDN_RLS_DLY_SHFT                                       0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_GEAR_BAND_SELECT_CONTROLS_ADDR                                         (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000234)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_GEAR_BAND_SELECT_CONTROLS_RMSK                                               0x3f
#define HWIO_DSI_1_PHY_DSIPHY_PLL_GEAR_BAND_SELECT_CONTROLS_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_GEAR_BAND_SELECT_CONTROLS_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_GEAR_BAND_SELECT_CONTROLS_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_GEAR_BAND_SELECT_CONTROLS_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_GEAR_BAND_SELECT_CONTROLS_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_GEAR_BAND_SELECT_CONTROLS_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_GEAR_BAND_SELECT_CONTROLS_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_GEAR_BAND_SELECT_CONTROLS_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_GEAR_BAND_SELECT_CONTROLS_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_GEAR_BAND_SELECT_CONTROLS_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_GEAR_BAND_SELECT_CONTROLS_BAS_STOP_FIRST_LOCK_2STEPS_ADJ_BMSK                0x20
#define HWIO_DSI_1_PHY_DSIPHY_PLL_GEAR_BAND_SELECT_CONTROLS_BAS_STOP_FIRST_LOCK_2STEPS_ADJ_SHFT                 0x5
#define HWIO_DSI_1_PHY_DSIPHY_PLL_GEAR_BAND_SELECT_CONTROLS_BAS_STOP_FIRST_LOCK_1STEP_ADJ_BMSK                 0x10
#define HWIO_DSI_1_PHY_DSIPHY_PLL_GEAR_BAND_SELECT_CONTROLS_BAS_STOP_FIRST_LOCK_1STEP_ADJ_SHFT                  0x4
#define HWIO_DSI_1_PHY_DSIPHY_PLL_GEAR_BAND_SELECT_CONTROLS_BAS_2ND_LOOP_X2_BMSK                                0x8
#define HWIO_DSI_1_PHY_DSIPHY_PLL_GEAR_BAND_SELECT_CONTROLS_BAS_2ND_LOOP_X2_SHFT                                0x3
#define HWIO_DSI_1_PHY_DSIPHY_PLL_GEAR_BAND_SELECT_CONTROLS_BAS_OFFSET_DIV2_BMSK                                0x4
#define HWIO_DSI_1_PHY_DSIPHY_PLL_GEAR_BAND_SELECT_CONTROLS_BAS_OFFSET_DIV2_SHFT                                0x2
#define HWIO_DSI_1_PHY_DSIPHY_PLL_GEAR_BAND_SELECT_CONTROLS_BAS_GEAR_BS_ENABLE_BMSK                             0x2
#define HWIO_DSI_1_PHY_DSIPHY_PLL_GEAR_BAND_SELECT_CONTROLS_BAS_GEAR_BS_ENABLE_SHFT                             0x1
#define HWIO_DSI_1_PHY_DSIPHY_PLL_GEAR_BAND_SELECT_CONTROLS_LNS_GEAR_BS_ENABLE_BMSK                             0x1
#define HWIO_DSI_1_PHY_DSIPHY_PLL_GEAR_BAND_SELECT_CONTROLS_LNS_GEAR_BS_ENABLE_SHFT                             0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_PSM_CLK_CONTROLS_ADDR                                                  (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000238)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PSM_CLK_CONTROLS_RMSK                                                         0xf
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PSM_CLK_CONTROLS_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_PSM_CLK_CONTROLS_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_PSM_CLK_CONTROLS_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PSM_CLK_CONTROLS_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_PSM_CLK_CONTROLS_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PSM_CLK_CONTROLS_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_PSM_CLK_CONTROLS_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PSM_CLK_CONTROLS_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_PSM_CLK_CONTROLS_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_PSM_CLK_CONTROLS_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PSM_CLK_CONTROLS_USE_CMODE_CAL_CLK_CONTROL_BMSK                               0x8
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PSM_CLK_CONTROLS_USE_CMODE_CAL_CLK_CONTROL_SHFT                               0x3
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PSM_CLK_CONTROLS_PSM_CONTROL_CAL_CLK_MUX_BMSK                                 0x4
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PSM_CLK_CONTROLS_PSM_CONTROL_CAL_CLK_MUX_SHFT                                 0x2
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PSM_CLK_CONTROLS_PSM_CONTROL_CAL_CLK_BMSK                                     0x2
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PSM_CLK_CONTROLS_PSM_CONTROL_CAL_CLK_SHFT                                     0x1
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PSM_CLK_CONTROLS_PS2_CLK_DRV_GATEOFF_BMSK                                     0x1
#define HWIO_DSI_1_PHY_DSIPHY_PLL_PSM_CLK_CONTROLS_PS2_CLK_DRV_GATEOFF_SHFT                                     0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_SYSTEM_MUXES_2_ADDR                                                    (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x0000023c)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SYSTEM_MUXES_2_RMSK                                                          0x1f
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SYSTEM_MUXES_2_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_SYSTEM_MUXES_2_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_SYSTEM_MUXES_2_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SYSTEM_MUXES_2_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_SYSTEM_MUXES_2_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SYSTEM_MUXES_2_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_SYSTEM_MUXES_2_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SYSTEM_MUXES_2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_SYSTEM_MUXES_2_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_SYSTEM_MUXES_2_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SYSTEM_MUXES_2_CM_VCOCLK_DIV2EN_MSB_MUX_BMSK                                 0x10
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SYSTEM_MUXES_2_CM_VCOCLK_DIV2EN_MSB_MUX_SHFT                                  0x4
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SYSTEM_MUXES_2_CM_VCOCLK_DIV2EN_MUX_BMSK                                      0x8
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SYSTEM_MUXES_2_CM_VCOCLK_DIV2EN_MUX_SHFT                                      0x3
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SYSTEM_MUXES_2_CM_PLL_CLOCK_INVERTERS_MUX_BMSK                                0x4
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SYSTEM_MUXES_2_CM_PLL_CLOCK_INVERTERS_MUX_SHFT                                0x2
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SYSTEM_MUXES_2_VCO_CONFIG_MUX_BMSK                                            0x2
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SYSTEM_MUXES_2_VCO_CONFIG_MUX_SHFT                                            0x1
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SYSTEM_MUXES_2_CM_CMODE_MUX_BMSK                                              0x1
#define HWIO_DSI_1_PHY_DSIPHY_PLL_SYSTEM_MUXES_2_CM_CMODE_MUX_SHFT                                              0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_VCO_CONFIG_1_ADDR                                                      (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000240)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_VCO_CONFIG_1_RMSK                                                            0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_VCO_CONFIG_1_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_VCO_CONFIG_1_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_VCO_CONFIG_1_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_VCO_CONFIG_1_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_VCO_CONFIG_1_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_VCO_CONFIG_1_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_VCO_CONFIG_1_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_VCO_CONFIG_1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_VCO_CONFIG_1_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_VCO_CONFIG_1_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_VCO_CONFIG_1_VCO_CONFIG_1_BMSK                                               0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_VCO_CONFIG_1_VCO_CONFIG_1_SHFT                                                0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_VCO_CONFIG_2_ADDR                                                      (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000244)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_VCO_CONFIG_2_RMSK                                                            0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_VCO_CONFIG_2_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_VCO_CONFIG_2_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_VCO_CONFIG_2_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_VCO_CONFIG_2_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_VCO_CONFIG_2_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_VCO_CONFIG_2_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_VCO_CONFIG_2_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_VCO_CONFIG_2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_VCO_CONFIG_2_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_VCO_CONFIG_2_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_VCO_CONFIG_2_VCO_CONFIG_2_BMSK                                               0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_VCO_CONFIG_2_VCO_CONFIG_2_SHFT                                                0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_CLOCK_INVERTERS_1_ADDR                                                 (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000248)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CLOCK_INVERTERS_1_RMSK                                                       0x7b
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CLOCK_INVERTERS_1_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_CLOCK_INVERTERS_1_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_CLOCK_INVERTERS_1_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CLOCK_INVERTERS_1_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_CLOCK_INVERTERS_1_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CLOCK_INVERTERS_1_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_CLOCK_INVERTERS_1_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CLOCK_INVERTERS_1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_CLOCK_INVERTERS_1_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_CLOCK_INVERTERS_1_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CLOCK_INVERTERS_1_CM_OSCBIAS_ADJ_1_BMSK                                      0x60
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CLOCK_INVERTERS_1_CM_OSCBIAS_ADJ_1_SHFT                                       0x5
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CLOCK_INVERTERS_1_CM_OSCCAP_EN_1_BMSK                                        0x18
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CLOCK_INVERTERS_1_CM_OSCCAP_EN_1_SHFT                                         0x3
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CLOCK_INVERTERS_1_INVERT_DSMCLK_1_BMSK                                        0x2
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CLOCK_INVERTERS_1_INVERT_DSMCLK_1_SHFT                                        0x1
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CLOCK_INVERTERS_1_INVERT_FRACNCLK_1_BMSK                                      0x1
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CLOCK_INVERTERS_1_INVERT_FRACNCLK_1_SHFT                                      0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_CLOCK_INVERTERS_2_ADDR                                                 (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x0000024c)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CLOCK_INVERTERS_2_RMSK                                                       0x7b
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CLOCK_INVERTERS_2_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_CLOCK_INVERTERS_2_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_CLOCK_INVERTERS_2_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CLOCK_INVERTERS_2_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_CLOCK_INVERTERS_2_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CLOCK_INVERTERS_2_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_CLOCK_INVERTERS_2_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CLOCK_INVERTERS_2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_CLOCK_INVERTERS_2_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_CLOCK_INVERTERS_2_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CLOCK_INVERTERS_2_CM_OSCBIAS_ADJ_2_BMSK                                      0x60
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CLOCK_INVERTERS_2_CM_OSCBIAS_ADJ_2_SHFT                                       0x5
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CLOCK_INVERTERS_2_CM_OSCCAP_EN_2_BMSK                                        0x18
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CLOCK_INVERTERS_2_CM_OSCCAP_EN_2_SHFT                                         0x3
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CLOCK_INVERTERS_2_INVERT_DSMCLK_2_BMSK                                        0x2
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CLOCK_INVERTERS_2_INVERT_DSMCLK_2_SHFT                                        0x1
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CLOCK_INVERTERS_2_INVERT_FRACNCLK_2_BMSK                                      0x1
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CLOCK_INVERTERS_2_INVERT_FRACNCLK_2_SHFT                                      0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_CMODE_1_ADDR                                                           (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000250)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CMODE_1_RMSK                                                                 0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CMODE_1_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_CMODE_1_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_CMODE_1_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CMODE_1_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_CMODE_1_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CMODE_1_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_CMODE_1_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CMODE_1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_CMODE_1_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_CMODE_1_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CMODE_1_CM_CMODE_1_BMSK                                                      0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CMODE_1_CM_CMODE_1_SHFT                                                       0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_CMODE_2_ADDR                                                           (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000254)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CMODE_2_RMSK                                                                 0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CMODE_2_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_CMODE_2_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_CMODE_2_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CMODE_2_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_CMODE_2_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CMODE_2_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_CMODE_2_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CMODE_2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_CMODE_2_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_CMODE_2_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CMODE_2_CM_CMODE_2_BMSK                                                      0xff
#define HWIO_DSI_1_PHY_DSIPHY_PLL_CMODE_2_CM_CMODE_2_SHFT                                                       0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FIVE_1_ADDR                                            (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x00000258)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FIVE_1_RMSK                                                   0x3
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FIVE_1_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FIVE_1_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FIVE_1_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FIVE_1_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FIVE_1_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FIVE_1_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FIVE_1_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FIVE_1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FIVE_1_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FIVE_1_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FIVE_1_CM_VCOCLK_DIV2EN_MSB_1_BMSK                            0x2
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FIVE_1_CM_VCOCLK_DIV2EN_MSB_1_SHFT                            0x1
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FIVE_1_CM_VCOCLK_DIV2EN_1_BMSK                                0x1
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FIVE_1_CM_VCOCLK_DIV2EN_1_SHFT                                0x0

#define HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FIVE_2_ADDR                                            (DSI_1_PHY_DSIPHY_PLL_DSIPHY_PLL_DSI_QDIG_PLL_REG_BASE      + 0x0000025c)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FIVE_2_RMSK                                                   0x3
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FIVE_2_IN          \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FIVE_2_ADDR, HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FIVE_2_RMSK)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FIVE_2_INM(m)      \
        in_dword_masked(HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FIVE_2_ADDR, m)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FIVE_2_OUT(v)      \
        out_dword(HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FIVE_2_ADDR,v)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FIVE_2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FIVE_2_ADDR,m,v,HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FIVE_2_IN)
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FIVE_2_CM_VCOCLK_DIV2EN_MSB_2_BMSK                            0x2
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FIVE_2_CM_VCOCLK_DIV2EN_MSB_2_SHFT                            0x1
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FIVE_2_CM_VCOCLK_DIV2EN_2_BMSK                                0x1
#define HWIO_DSI_1_PHY_DSIPHY_PLL_ANALOG_CONTROLS_FIVE_2_CM_VCOCLK_DIV2EN_2_SHFT                                0x0


#endif /* __HALDSI_PHY_2_3_0_H__ */
