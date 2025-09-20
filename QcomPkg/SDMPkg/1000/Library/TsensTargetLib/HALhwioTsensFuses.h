#ifndef __HALHWIOTSENSFUSES_H__
#define __HALHWIOTSENSFUSES_H__
/*
===========================================================================
*/
/**
  @file HALhwioTsensFuses.h
  @brief Auto-generated HWIO interface include file.

  Reference chip release:
    SDM1000 (Poipu) [poipu_v1.0_p3q3r65_IPCAT]

  This file contains HWIO register definitions for the following modules:
    SECURITY_CONTROL_CORE

  'Include' filters applied: QFPROM_CORR_CALIB.*[SECURITY_CONTROL_CORE]

  Generation parameters:
  { u'filename': u'HALhwioTsensFuses.h',
    u'header': u'#include "msmhwiobase.h"',
    u'module-filter-exclude': { },
    u'module-filter-include': { u'SECURITY_CONTROL_CORE': [ u'QFPROM_CORR_CALIB.*']},
    u'modules': [u'SECURITY_CONTROL_CORE']}
*/
/*
  ===========================================================================

  Copyright (c) 2018 Qualcomm Technologies, Inc.
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

#include "msmhwiobase.h"

/*----------------------------------------------------------------------------
 * MODULE: SECURITY_CONTROL_CORE
 *--------------------------------------------------------------------------*/

#define SECURITY_CONTROL_CORE_REG_BASE                                                          (SECURITY_CONTROL_BASE      + 0x00000000)

#define HWIO_QFPROM_CORR_CALIB_ROW0_LSB_ADDR                                                    (SECURITY_CONTROL_CORE_REG_BASE      + 0x000041f8)
#define HWIO_QFPROM_CORR_CALIB_ROW0_LSB_RMSK                                                    0xffffffff
#define HWIO_QFPROM_CORR_CALIB_ROW0_LSB_IN          \
        in_dword_masked(HWIO_QFPROM_CORR_CALIB_ROW0_LSB_ADDR, HWIO_QFPROM_CORR_CALIB_ROW0_LSB_RMSK)
#define HWIO_QFPROM_CORR_CALIB_ROW0_LSB_INM(m)      \
        in_dword_masked(HWIO_QFPROM_CORR_CALIB_ROW0_LSB_ADDR, m)
#define HWIO_QFPROM_CORR_CALIB_ROW0_LSB_QUSB_PORT1_HSTX_TRIM_LSB_1_0_BMSK                       0xc0000000
#define HWIO_QFPROM_CORR_CALIB_ROW0_LSB_QUSB_PORT1_HSTX_TRIM_LSB_1_0_SHFT                             0x1e
#define HWIO_QFPROM_CORR_CALIB_ROW0_LSB_QUSB_PORT0_HS_REFCLK_SEL_BMSK                           0x20000000
#define HWIO_QFPROM_CORR_CALIB_ROW0_LSB_QUSB_PORT0_HS_REFCLK_SEL_SHFT                                 0x1d
#define HWIO_QFPROM_CORR_CALIB_ROW0_LSB_QUSB_PORT0_HSTX_TRIM_LSB_BMSK                           0x1e000000
#define HWIO_QFPROM_CORR_CALIB_ROW0_LSB_QUSB_PORT0_HSTX_TRIM_LSB_SHFT                                 0x19
#define HWIO_QFPROM_CORR_CALIB_ROW0_LSB_USB2PHY_RCAL_CODE_BMSK                                   0x1e00000
#define HWIO_QFPROM_CORR_CALIB_ROW0_LSB_USB2PHY_RCAL_CODE_SHFT                                        0x15
#define HWIO_QFPROM_CORR_CALIB_ROW0_LSB_SPARE0_BMSK                                               0x1c0000
#define HWIO_QFPROM_CORR_CALIB_ROW0_LSB_SPARE0_SHFT                                                   0x12
#define HWIO_QFPROM_CORR_CALIB_ROW0_LSB_MSS_Q6SSV_LDO_VREF_TRIM_BMSK                               0x3e000
#define HWIO_QFPROM_CORR_CALIB_ROW0_LSB_MSS_Q6SSV_LDO_VREF_TRIM_SHFT                                   0xd
#define HWIO_QFPROM_CORR_CALIB_ROW0_LSB_SPARE_R63_B12_BMSK                                          0x1000
#define HWIO_QFPROM_CORR_CALIB_ROW0_LSB_SPARE_R63_B12_SHFT                                             0xc
#define HWIO_QFPROM_CORR_CALIB_ROW0_LSB_TURING_Q6SS1_LDO_VREF_TRIM_BMSK                              0xf80
#define HWIO_QFPROM_CORR_CALIB_ROW0_LSB_TURING_Q6SS1_LDO_VREF_TRIM_SHFT                                0x7
#define HWIO_QFPROM_CORR_CALIB_ROW0_LSB_TURING_Q6SS1_LDO_ENABLE_NOM_BMSK                              0x40
#define HWIO_QFPROM_CORR_CALIB_ROW0_LSB_TURING_Q6SS1_LDO_ENABLE_NOM_SHFT                               0x6
#define HWIO_QFPROM_CORR_CALIB_ROW0_LSB_MSS_Q6SS0_LDO_VREF_TRIM_BMSK                                  0x3e
#define HWIO_QFPROM_CORR_CALIB_ROW0_LSB_MSS_Q6SS0_LDO_VREF_TRIM_SHFT                                   0x1
#define HWIO_QFPROM_CORR_CALIB_ROW0_LSB_MSS_Q6SS0_LDO_ENABLE_NOM_BMSK                                  0x1
#define HWIO_QFPROM_CORR_CALIB_ROW0_LSB_MSS_Q6SS0_LDO_ENABLE_NOM_SHFT                                  0x0

#define HWIO_QFPROM_CORR_CALIB_ROW0_MSB_ADDR                                                    (SECURITY_CONTROL_CORE_REG_BASE      + 0x000041fc)
#define HWIO_QFPROM_CORR_CALIB_ROW0_MSB_RMSK                                                    0xffffffff
#define HWIO_QFPROM_CORR_CALIB_ROW0_MSB_IN          \
        in_dword_masked(HWIO_QFPROM_CORR_CALIB_ROW0_MSB_ADDR, HWIO_QFPROM_CORR_CALIB_ROW0_MSB_RMSK)
#define HWIO_QFPROM_CORR_CALIB_ROW0_MSB_INM(m)      \
        in_dword_masked(HWIO_QFPROM_CORR_CALIB_ROW0_MSB_ADDR, m)
#define HWIO_QFPROM_CORR_CALIB_ROW0_MSB_SPARE1_BMSK                                             0xffffe000
#define HWIO_QFPROM_CORR_CALIB_ROW0_MSB_SPARE1_SHFT                                                    0xd
#define HWIO_QFPROM_CORR_CALIB_ROW0_MSB_TURING_Q6SS1_LDO_ENABLE_NOM_L1_BMSK                         0x1000
#define HWIO_QFPROM_CORR_CALIB_ROW0_MSB_TURING_Q6SS1_LDO_ENABLE_NOM_L1_SHFT                            0xc
#define HWIO_QFPROM_CORR_CALIB_ROW0_MSB_MSS_Q6SS0_LDO_ENABLE_NOM_L1_BMSK                             0x800
#define HWIO_QFPROM_CORR_CALIB_ROW0_MSB_MSS_Q6SS0_LDO_ENABLE_NOM_L1_SHFT                               0xb
#define HWIO_QFPROM_CORR_CALIB_ROW0_MSB_TURING_Q6SS1_LDO_ENABLE_MINSVS_BMSK                          0x400
#define HWIO_QFPROM_CORR_CALIB_ROW0_MSB_TURING_Q6SS1_LDO_ENABLE_MINSVS_SHFT                            0xa
#define HWIO_QFPROM_CORR_CALIB_ROW0_MSB_TURING_Q6SS1_LDO_ENABLE_LOWSVS_BMSK                          0x200
#define HWIO_QFPROM_CORR_CALIB_ROW0_MSB_TURING_Q6SS1_LDO_ENABLE_LOWSVS_SHFT                            0x9
#define HWIO_QFPROM_CORR_CALIB_ROW0_MSB_TURING_Q6SS1_LDO_ENABLE_SVS_BMSK                             0x100
#define HWIO_QFPROM_CORR_CALIB_ROW0_MSB_TURING_Q6SS1_LDO_ENABLE_SVS_SHFT                               0x8
#define HWIO_QFPROM_CORR_CALIB_ROW0_MSB_TURING_Q6SS1_LDO_ENABLE_SVS_L1_BMSK                           0x80
#define HWIO_QFPROM_CORR_CALIB_ROW0_MSB_TURING_Q6SS1_LDO_ENABLE_SVS_L1_SHFT                            0x7
#define HWIO_QFPROM_CORR_CALIB_ROW0_MSB_MSS_Q6SS0_LDO_ENABLE_MINSVS_BMSK                              0x40
#define HWIO_QFPROM_CORR_CALIB_ROW0_MSB_MSS_Q6SS0_LDO_ENABLE_MINSVS_SHFT                               0x6
#define HWIO_QFPROM_CORR_CALIB_ROW0_MSB_MSS_Q6SS0_LDO_ENABLE_LOWSVS_BMSK                              0x20
#define HWIO_QFPROM_CORR_CALIB_ROW0_MSB_MSS_Q6SS0_LDO_ENABLE_LOWSVS_SHFT                               0x5
#define HWIO_QFPROM_CORR_CALIB_ROW0_MSB_MSS_Q6SS0_LDO_ENABLE_SVS_BMSK                                 0x10
#define HWIO_QFPROM_CORR_CALIB_ROW0_MSB_MSS_Q6SS0_LDO_ENABLE_SVS_SHFT                                  0x4
#define HWIO_QFPROM_CORR_CALIB_ROW0_MSB_MSS_Q6SS0_LDO_ENABLE_SVS_L1_BMSK                               0x8
#define HWIO_QFPROM_CORR_CALIB_ROW0_MSB_MSS_Q6SS0_LDO_ENABLE_SVS_L1_SHFT                               0x3
#define HWIO_QFPROM_CORR_CALIB_ROW0_MSB_QUSB_PORT1_HS_REFCLK_SEL_BMSK                                  0x4
#define HWIO_QFPROM_CORR_CALIB_ROW0_MSB_QUSB_PORT1_HS_REFCLK_SEL_SHFT                                  0x2
#define HWIO_QFPROM_CORR_CALIB_ROW0_MSB_QUSB_PORT1_HSTX_TRIM_LSB_3_2_BMSK                              0x3
#define HWIO_QFPROM_CORR_CALIB_ROW0_MSB_QUSB_PORT1_HSTX_TRIM_LSB_3_2_SHFT                              0x0

#define HWIO_QFPROM_CORR_CALIB_ROW1_LSB_ADDR                                                    (SECURITY_CONTROL_CORE_REG_BASE      + 0x00004200)
#define HWIO_QFPROM_CORR_CALIB_ROW1_LSB_RMSK                                                    0xffffffff
#define HWIO_QFPROM_CORR_CALIB_ROW1_LSB_IN          \
        in_dword_masked(HWIO_QFPROM_CORR_CALIB_ROW1_LSB_ADDR, HWIO_QFPROM_CORR_CALIB_ROW1_LSB_RMSK)
#define HWIO_QFPROM_CORR_CALIB_ROW1_LSB_INM(m)      \
        in_dword_masked(HWIO_QFPROM_CORR_CALIB_ROW1_LSB_ADDR, m)
#define HWIO_QFPROM_CORR_CALIB_ROW1_LSB_REFGEN_EAST_BGV_TRIM_BMSK                               0xff000000
#define HWIO_QFPROM_CORR_CALIB_ROW1_LSB_REFGEN_EAST_BGV_TRIM_SHFT                                     0x18
#define HWIO_QFPROM_CORR_CALIB_ROW1_LSB_SPARE0_BMSK                                               0xffffff
#define HWIO_QFPROM_CORR_CALIB_ROW1_LSB_SPARE0_SHFT                                                    0x0

#define HWIO_QFPROM_CORR_CALIB_ROW1_MSB_ADDR                                                    (SECURITY_CONTROL_CORE_REG_BASE      + 0x00004204)
#define HWIO_QFPROM_CORR_CALIB_ROW1_MSB_RMSK                                                    0xffffffff
#define HWIO_QFPROM_CORR_CALIB_ROW1_MSB_IN          \
        in_dword_masked(HWIO_QFPROM_CORR_CALIB_ROW1_MSB_ADDR, HWIO_QFPROM_CORR_CALIB_ROW1_MSB_RMSK)
#define HWIO_QFPROM_CORR_CALIB_ROW1_MSB_INM(m)      \
        in_dword_masked(HWIO_QFPROM_CORR_CALIB_ROW1_MSB_ADDR, m)
#define HWIO_QFPROM_CORR_CALIB_ROW1_MSB_SPARE2_BMSK                                             0x80000000
#define HWIO_QFPROM_CORR_CALIB_ROW1_MSB_SPARE2_SHFT                                                   0x1f
#define HWIO_QFPROM_CORR_CALIB_ROW1_MSB_REFGEN_NORTH_BGV_TRIM_BMSK                              0x7f800000
#define HWIO_QFPROM_CORR_CALIB_ROW1_MSB_REFGEN_NORTH_BGV_TRIM_SHFT                                    0x17
#define HWIO_QFPROM_CORR_CALIB_ROW1_MSB_REFGEN_SOUTH_BGV_TRIM_BMSK                                0x7f8000
#define HWIO_QFPROM_CORR_CALIB_ROW1_MSB_REFGEN_SOUTH_BGV_TRIM_SHFT                                     0xf
#define HWIO_QFPROM_CORR_CALIB_ROW1_MSB_BANDGAP_TRIM_BMSK                                           0x7f00
#define HWIO_QFPROM_CORR_CALIB_ROW1_MSB_BANDGAP_TRIM_SHFT                                              0x8
#define HWIO_QFPROM_CORR_CALIB_ROW1_MSB_REFGEN_SOUTH1_BGV_TRIM_BMSK                                   0xff
#define HWIO_QFPROM_CORR_CALIB_ROW1_MSB_REFGEN_SOUTH1_BGV_TRIM_SHFT                                    0x0

#define HWIO_QFPROM_CORR_CALIB_ROW2_LSB_ADDR                                                    (SECURITY_CONTROL_CORE_REG_BASE      + 0x00004208)
#define HWIO_QFPROM_CORR_CALIB_ROW2_LSB_RMSK                                                    0xffffffff
#define HWIO_QFPROM_CORR_CALIB_ROW2_LSB_IN          \
        in_dword_masked(HWIO_QFPROM_CORR_CALIB_ROW2_LSB_ADDR, HWIO_QFPROM_CORR_CALIB_ROW2_LSB_RMSK)
#define HWIO_QFPROM_CORR_CALIB_ROW2_LSB_INM(m)      \
        in_dword_masked(HWIO_QFPROM_CORR_CALIB_ROW2_LSB_ADDR, m)
#define HWIO_QFPROM_CORR_CALIB_ROW2_LSB_CPR0_TARG_VOLT_OFFSET_SVS_BMSK                          0xf0000000
#define HWIO_QFPROM_CORR_CALIB_ROW2_LSB_CPR0_TARG_VOLT_OFFSET_SVS_SHFT                                0x1c
#define HWIO_QFPROM_CORR_CALIB_ROW2_LSB_CPR0_TARG_VOLT_OFFSET_NOM_BMSK                           0xf000000
#define HWIO_QFPROM_CORR_CALIB_ROW2_LSB_CPR0_TARG_VOLT_OFFSET_NOM_SHFT                                0x18
#define HWIO_QFPROM_CORR_CALIB_ROW2_LSB_CPR0_TARG_VOLT_OFFSET_TUR_BMSK                            0xf00000
#define HWIO_QFPROM_CORR_CALIB_ROW2_LSB_CPR0_TARG_VOLT_OFFSET_TUR_SHFT                                0x14
#define HWIO_QFPROM_CORR_CALIB_ROW2_LSB_CPR0_TARG_VOLT_SVS3_BMSK                                   0xf8000
#define HWIO_QFPROM_CORR_CALIB_ROW2_LSB_CPR0_TARG_VOLT_SVS3_SHFT                                       0xf
#define HWIO_QFPROM_CORR_CALIB_ROW2_LSB_CPR0_TARG_VOLT_SVS_BMSK                                     0x7c00
#define HWIO_QFPROM_CORR_CALIB_ROW2_LSB_CPR0_TARG_VOLT_SVS_SHFT                                        0xa
#define HWIO_QFPROM_CORR_CALIB_ROW2_LSB_CPR0_TARG_VOLT_NOM_BMSK                                      0x3e0
#define HWIO_QFPROM_CORR_CALIB_ROW2_LSB_CPR0_TARG_VOLT_NOM_SHFT                                        0x5
#define HWIO_QFPROM_CORR_CALIB_ROW2_LSB_CPR10_TARG_VOLT_TUR_BMSK                                      0x1f
#define HWIO_QFPROM_CORR_CALIB_ROW2_LSB_CPR10_TARG_VOLT_TUR_SHFT                                       0x0

#define HWIO_QFPROM_CORR_CALIB_ROW2_MSB_ADDR                                                    (SECURITY_CONTROL_CORE_REG_BASE      + 0x0000420c)
#define HWIO_QFPROM_CORR_CALIB_ROW2_MSB_RMSK                                                    0xffffffff
#define HWIO_QFPROM_CORR_CALIB_ROW2_MSB_IN          \
        in_dword_masked(HWIO_QFPROM_CORR_CALIB_ROW2_MSB_ADDR, HWIO_QFPROM_CORR_CALIB_ROW2_MSB_RMSK)
#define HWIO_QFPROM_CORR_CALIB_ROW2_MSB_INM(m)      \
        in_dword_masked(HWIO_QFPROM_CORR_CALIB_ROW2_MSB_ADDR, m)
#define HWIO_QFPROM_CORR_CALIB_ROW2_MSB_CPR1_TARG_VOLT_SVS3_BMSK                                0xf8000000
#define HWIO_QFPROM_CORR_CALIB_ROW2_MSB_CPR1_TARG_VOLT_SVS3_SHFT                                      0x1b
#define HWIO_QFPROM_CORR_CALIB_ROW2_MSB_CPR1_TARG_VOLT_SVS_BMSK                                  0x7c00000
#define HWIO_QFPROM_CORR_CALIB_ROW2_MSB_CPR1_TARG_VOLT_SVS_SHFT                                       0x16
#define HWIO_QFPROM_CORR_CALIB_ROW2_MSB_CPR1_TARG_VOLT_NOM_BMSK                                   0x3e0000
#define HWIO_QFPROM_CORR_CALIB_ROW2_MSB_CPR1_TARG_VOLT_NOM_SHFT                                       0x11
#define HWIO_QFPROM_CORR_CALIB_ROW2_MSB_CPR10_TARG_VOLT_NOM_BMSK                                   0x1f000
#define HWIO_QFPROM_CORR_CALIB_ROW2_MSB_CPR10_TARG_VOLT_NOM_SHFT                                       0xc
#define HWIO_QFPROM_CORR_CALIB_ROW2_MSB_CPR0_AGING_BMSK                                              0xff0
#define HWIO_QFPROM_CORR_CALIB_ROW2_MSB_CPR0_AGING_SHFT                                                0x4
#define HWIO_QFPROM_CORR_CALIB_ROW2_MSB_CPR0_TARG_VOLT_OFFSET_SVS2_BMSK                                0xf
#define HWIO_QFPROM_CORR_CALIB_ROW2_MSB_CPR0_TARG_VOLT_OFFSET_SVS2_SHFT                                0x0

#define HWIO_QFPROM_CORR_CALIB_ROW3_LSB_ADDR                                                    (SECURITY_CONTROL_CORE_REG_BASE      + 0x00004210)
#define HWIO_QFPROM_CORR_CALIB_ROW3_LSB_RMSK                                                    0xffffffff
#define HWIO_QFPROM_CORR_CALIB_ROW3_LSB_IN          \
        in_dword_masked(HWIO_QFPROM_CORR_CALIB_ROW3_LSB_ADDR, HWIO_QFPROM_CORR_CALIB_ROW3_LSB_RMSK)
#define HWIO_QFPROM_CORR_CALIB_ROW3_LSB_INM(m)      \
        in_dword_masked(HWIO_QFPROM_CORR_CALIB_ROW3_LSB_ADDR, m)
#define HWIO_QFPROM_CORR_CALIB_ROW3_LSB_CPR2_TARG_VOLT_NOM_2_0_BMSK                             0xe0000000
#define HWIO_QFPROM_CORR_CALIB_ROW3_LSB_CPR2_TARG_VOLT_NOM_2_0_SHFT                                   0x1d
#define HWIO_QFPROM_CORR_CALIB_ROW3_LSB_CPR10_TARG_VOLT_SVS2_BMSK                               0x1f000000
#define HWIO_QFPROM_CORR_CALIB_ROW3_LSB_CPR10_TARG_VOLT_SVS2_SHFT                                     0x18
#define HWIO_QFPROM_CORR_CALIB_ROW3_LSB_CPR1_AGING_BMSK                                           0xff0000
#define HWIO_QFPROM_CORR_CALIB_ROW3_LSB_CPR1_AGING_SHFT                                               0x10
#define HWIO_QFPROM_CORR_CALIB_ROW3_LSB_CPR1_TARG_VOLT_OFFSET_SVS2_BMSK                             0xf000
#define HWIO_QFPROM_CORR_CALIB_ROW3_LSB_CPR1_TARG_VOLT_OFFSET_SVS2_SHFT                                0xc
#define HWIO_QFPROM_CORR_CALIB_ROW3_LSB_CPR1_TARG_VOLT_OFFSET_SVS_BMSK                               0xf00
#define HWIO_QFPROM_CORR_CALIB_ROW3_LSB_CPR1_TARG_VOLT_OFFSET_SVS_SHFT                                 0x8
#define HWIO_QFPROM_CORR_CALIB_ROW3_LSB_CPR1_TARG_VOLT_OFFSET_NOM_BMSK                                0xf0
#define HWIO_QFPROM_CORR_CALIB_ROW3_LSB_CPR1_TARG_VOLT_OFFSET_NOM_SHFT                                 0x4
#define HWIO_QFPROM_CORR_CALIB_ROW3_LSB_CPR1_TARG_VOLT_OFFSET_TUR_BMSK                                 0xf
#define HWIO_QFPROM_CORR_CALIB_ROW3_LSB_CPR1_TARG_VOLT_OFFSET_TUR_SHFT                                 0x0

#define HWIO_QFPROM_CORR_CALIB_ROW3_MSB_ADDR                                                    (SECURITY_CONTROL_CORE_REG_BASE      + 0x00004214)
#define HWIO_QFPROM_CORR_CALIB_ROW3_MSB_RMSK                                                    0xffffffff
#define HWIO_QFPROM_CORR_CALIB_ROW3_MSB_IN          \
        in_dword_masked(HWIO_QFPROM_CORR_CALIB_ROW3_MSB_ADDR, HWIO_QFPROM_CORR_CALIB_ROW3_MSB_RMSK)
#define HWIO_QFPROM_CORR_CALIB_ROW3_MSB_INM(m)      \
        in_dword_masked(HWIO_QFPROM_CORR_CALIB_ROW3_MSB_ADDR, m)
#define HWIO_QFPROM_CORR_CALIB_ROW3_MSB_CPR2_AGING_3_0_BMSK                                     0xf0000000
#define HWIO_QFPROM_CORR_CALIB_ROW3_MSB_CPR2_AGING_3_0_SHFT                                           0x1c
#define HWIO_QFPROM_CORR_CALIB_ROW3_MSB_CPR2_TARG_VOLT_OFFSET_SVS2_BMSK                          0xf000000
#define HWIO_QFPROM_CORR_CALIB_ROW3_MSB_CPR2_TARG_VOLT_OFFSET_SVS2_SHFT                               0x18
#define HWIO_QFPROM_CORR_CALIB_ROW3_MSB_CPR2_TARG_VOLT_OFFSET_SVS_BMSK                            0xf00000
#define HWIO_QFPROM_CORR_CALIB_ROW3_MSB_CPR2_TARG_VOLT_OFFSET_SVS_SHFT                                0x14
#define HWIO_QFPROM_CORR_CALIB_ROW3_MSB_CPR2_TARG_VOLT_OFFSET_NOM_BMSK                             0xf0000
#define HWIO_QFPROM_CORR_CALIB_ROW3_MSB_CPR2_TARG_VOLT_OFFSET_NOM_SHFT                                0x10
#define HWIO_QFPROM_CORR_CALIB_ROW3_MSB_CPR2_TARG_VOLT_OFFSET_TUR_BMSK                              0xf000
#define HWIO_QFPROM_CORR_CALIB_ROW3_MSB_CPR2_TARG_VOLT_OFFSET_TUR_SHFT                                 0xc
#define HWIO_QFPROM_CORR_CALIB_ROW3_MSB_CPR2_TARG_VOLT_SVS3_BMSK                                     0xf80
#define HWIO_QFPROM_CORR_CALIB_ROW3_MSB_CPR2_TARG_VOLT_SVS3_SHFT                                       0x7
#define HWIO_QFPROM_CORR_CALIB_ROW3_MSB_CPR2_TARG_VOLT_SVS_BMSK                                       0x7c
#define HWIO_QFPROM_CORR_CALIB_ROW3_MSB_CPR2_TARG_VOLT_SVS_SHFT                                        0x2
#define HWIO_QFPROM_CORR_CALIB_ROW3_MSB_CPR2_TARG_VOLT_NOM_4_3_BMSK                                    0x3
#define HWIO_QFPROM_CORR_CALIB_ROW3_MSB_CPR2_TARG_VOLT_NOM_4_3_SHFT                                    0x0

#define HWIO_QFPROM_CORR_CALIB_ROW4_LSB_ADDR                                                    (SECURITY_CONTROL_CORE_REG_BASE      + 0x00004218)
#define HWIO_QFPROM_CORR_CALIB_ROW4_LSB_RMSK                                                    0xffffffff
#define HWIO_QFPROM_CORR_CALIB_ROW4_LSB_IN          \
        in_dword_masked(HWIO_QFPROM_CORR_CALIB_ROW4_LSB_ADDR, HWIO_QFPROM_CORR_CALIB_ROW4_LSB_RMSK)
#define HWIO_QFPROM_CORR_CALIB_ROW4_LSB_INM(m)      \
        in_dword_masked(HWIO_QFPROM_CORR_CALIB_ROW4_LSB_ADDR, m)
#define HWIO_QFPROM_CORR_CALIB_ROW4_LSB_CPR4_TARG_VOLT_NOM_2_0_BMSK                             0xe0000000
#define HWIO_QFPROM_CORR_CALIB_ROW4_LSB_CPR4_TARG_VOLT_NOM_2_0_SHFT                                   0x1d
#define HWIO_QFPROM_CORR_CALIB_ROW4_LSB_CPR4_TARG_VOLT_TUR_BMSK                                 0x1f000000
#define HWIO_QFPROM_CORR_CALIB_ROW4_LSB_CPR4_TARG_VOLT_TUR_SHFT                                       0x18
#define HWIO_QFPROM_CORR_CALIB_ROW4_LSB_CPR3_TARG_VOLT_SVS3_BMSK                                  0xf80000
#define HWIO_QFPROM_CORR_CALIB_ROW4_LSB_CPR3_TARG_VOLT_SVS3_SHFT                                      0x13
#define HWIO_QFPROM_CORR_CALIB_ROW4_LSB_CPR3_TARG_VOLT_SVS_BMSK                                    0x7c000
#define HWIO_QFPROM_CORR_CALIB_ROW4_LSB_CPR3_TARG_VOLT_SVS_SHFT                                        0xe
#define HWIO_QFPROM_CORR_CALIB_ROW4_LSB_CPR3_TARG_VOLT_TUR_BMSK                                     0x3e00
#define HWIO_QFPROM_CORR_CALIB_ROW4_LSB_CPR3_TARG_VOLT_TUR_SHFT                                        0x9
#define HWIO_QFPROM_CORR_CALIB_ROW4_LSB_CPR3_TARG_VOLT_NOM_BMSK                                      0x1f0
#define HWIO_QFPROM_CORR_CALIB_ROW4_LSB_CPR3_TARG_VOLT_NOM_SHFT                                        0x4
#define HWIO_QFPROM_CORR_CALIB_ROW4_LSB_CPR2_AGING_7_4_BMSK                                            0xf
#define HWIO_QFPROM_CORR_CALIB_ROW4_LSB_CPR2_AGING_7_4_SHFT                                            0x0

#define HWIO_QFPROM_CORR_CALIB_ROW4_MSB_ADDR                                                    (SECURITY_CONTROL_CORE_REG_BASE      + 0x0000421c)
#define HWIO_QFPROM_CORR_CALIB_ROW4_MSB_RMSK                                                    0xffffffff
#define HWIO_QFPROM_CORR_CALIB_ROW4_MSB_IN          \
        in_dword_masked(HWIO_QFPROM_CORR_CALIB_ROW4_MSB_ADDR, HWIO_QFPROM_CORR_CALIB_ROW4_MSB_RMSK)
#define HWIO_QFPROM_CORR_CALIB_ROW4_MSB_INM(m)      \
        in_dword_masked(HWIO_QFPROM_CORR_CALIB_ROW4_MSB_ADDR, m)
#define HWIO_QFPROM_CORR_CALIB_ROW4_MSB_AON_TARG_VOLT_BMSK                                      0xf8000000
#define HWIO_QFPROM_CORR_CALIB_ROW4_MSB_AON_TARG_VOLT_SHFT                                            0x1b
#define HWIO_QFPROM_CORR_CALIB_ROW4_MSB_CPR5_TARG_VOLT_TUR_BMSK                                  0x7c00000
#define HWIO_QFPROM_CORR_CALIB_ROW4_MSB_CPR5_TARG_VOLT_TUR_SHFT                                       0x16
#define HWIO_QFPROM_CORR_CALIB_ROW4_MSB_CPR5_TARG_VOLT_NOM_BMSK                                   0x3e0000
#define HWIO_QFPROM_CORR_CALIB_ROW4_MSB_CPR5_TARG_VOLT_NOM_SHFT                                       0x11
#define HWIO_QFPROM_CORR_CALIB_ROW4_MSB_CPR_APC0_MODE_DISABLE_BMSK                                 0x10000
#define HWIO_QFPROM_CORR_CALIB_ROW4_MSB_CPR_APC0_MODE_DISABLE_SHFT                                    0x10
#define HWIO_QFPROM_CORR_CALIB_ROW4_MSB_CPR4_TARG_VOLT_OFFSET_NOM_BMSK                              0xf000
#define HWIO_QFPROM_CORR_CALIB_ROW4_MSB_CPR4_TARG_VOLT_OFFSET_NOM_SHFT                                 0xc
#define HWIO_QFPROM_CORR_CALIB_ROW4_MSB_CPR_APC1_MODE_DISABLE_BMSK                                   0x800
#define HWIO_QFPROM_CORR_CALIB_ROW4_MSB_CPR_APC1_MODE_DISABLE_SHFT                                     0xb
#define HWIO_QFPROM_CORR_CALIB_ROW4_MSB_CPR4_TARG_VOLT_OFFSET_TUR_BMSK                               0x780
#define HWIO_QFPROM_CORR_CALIB_ROW4_MSB_CPR4_TARG_VOLT_OFFSET_TUR_SHFT                                 0x7
#define HWIO_QFPROM_CORR_CALIB_ROW4_MSB_CPR10_TARG_VOLT_SVS_BMSK                                      0x7c
#define HWIO_QFPROM_CORR_CALIB_ROW4_MSB_CPR10_TARG_VOLT_SVS_SHFT                                       0x2
#define HWIO_QFPROM_CORR_CALIB_ROW4_MSB_CPR4_TARG_VOLT_NOM_4_3_BMSK                                    0x3
#define HWIO_QFPROM_CORR_CALIB_ROW4_MSB_CPR4_TARG_VOLT_NOM_4_3_SHFT                                    0x0

#define HWIO_QFPROM_CORR_CALIB_ROW5_LSB_ADDR                                                    (SECURITY_CONTROL_CORE_REG_BASE      + 0x00004220)
#define HWIO_QFPROM_CORR_CALIB_ROW5_LSB_RMSK                                                    0xffffffff
#define HWIO_QFPROM_CORR_CALIB_ROW5_LSB_IN          \
        in_dword_masked(HWIO_QFPROM_CORR_CALIB_ROW5_LSB_ADDR, HWIO_QFPROM_CORR_CALIB_ROW5_LSB_RMSK)
#define HWIO_QFPROM_CORR_CALIB_ROW5_LSB_INM(m)      \
        in_dword_masked(HWIO_QFPROM_CORR_CALIB_ROW5_LSB_ADDR, m)
#define HWIO_QFPROM_CORR_CALIB_ROW5_LSB_CPR8_LDO_TARG_VOLT_SVS_1_0_BMSK                         0xc0000000
#define HWIO_QFPROM_CORR_CALIB_ROW5_LSB_CPR8_LDO_TARG_VOLT_SVS_1_0_SHFT                               0x1e
#define HWIO_QFPROM_CORR_CALIB_ROW5_LSB_SPARE_R68_B29_BMSK                                      0x20000000
#define HWIO_QFPROM_CORR_CALIB_ROW5_LSB_SPARE_R68_B29_SHFT                                            0x1d
#define HWIO_QFPROM_CORR_CALIB_ROW5_LSB_CPR7_LDO_TARG_VOLT_SVS2_BMSK                            0x1e000000
#define HWIO_QFPROM_CORR_CALIB_ROW5_LSB_CPR7_LDO_TARG_VOLT_SVS2_SHFT                                  0x19
#define HWIO_QFPROM_CORR_CALIB_ROW5_LSB_SPARE_R68_B24_BMSK                                       0x1000000
#define HWIO_QFPROM_CORR_CALIB_ROW5_LSB_SPARE_R68_B24_SHFT                                            0x18
#define HWIO_QFPROM_CORR_CALIB_ROW5_LSB_CPR7_LDO_TARG_VOLT_SVSP_BMSK                              0xf00000
#define HWIO_QFPROM_CORR_CALIB_ROW5_LSB_CPR7_LDO_TARG_VOLT_SVSP_SHFT                                  0x14
#define HWIO_QFPROM_CORR_CALIB_ROW5_LSB_CPR6_TARG_VOLT_SVS3_BMSK                                   0xf8000
#define HWIO_QFPROM_CORR_CALIB_ROW5_LSB_CPR6_TARG_VOLT_SVS3_SHFT                                       0xf
#define HWIO_QFPROM_CORR_CALIB_ROW5_LSB_CPR6_TARG_VOLT_SVS_BMSK                                     0x7c00
#define HWIO_QFPROM_CORR_CALIB_ROW5_LSB_CPR6_TARG_VOLT_SVS_SHFT                                        0xa
#define HWIO_QFPROM_CORR_CALIB_ROW5_LSB_CPR6_TARG_VOLT_NOM_BMSK                                      0x3e0
#define HWIO_QFPROM_CORR_CALIB_ROW5_LSB_CPR6_TARG_VOLT_NOM_SHFT                                        0x5
#define HWIO_QFPROM_CORR_CALIB_ROW5_LSB_CPR6_TARG_VOLT_TUR_BMSK                                       0x1f
#define HWIO_QFPROM_CORR_CALIB_ROW5_LSB_CPR6_TARG_VOLT_TUR_SHFT                                        0x0

#define HWIO_QFPROM_CORR_CALIB_ROW5_MSB_ADDR                                                    (SECURITY_CONTROL_CORE_REG_BASE      + 0x00004224)
#define HWIO_QFPROM_CORR_CALIB_ROW5_MSB_RMSK                                                    0xffffffff
#define HWIO_QFPROM_CORR_CALIB_ROW5_MSB_IN          \
        in_dword_masked(HWIO_QFPROM_CORR_CALIB_ROW5_MSB_ADDR, HWIO_QFPROM_CORR_CALIB_ROW5_MSB_RMSK)
#define HWIO_QFPROM_CORR_CALIB_ROW5_MSB_INM(m)      \
        in_dword_masked(HWIO_QFPROM_CORR_CALIB_ROW5_MSB_ADDR, m)
#define HWIO_QFPROM_CORR_CALIB_ROW5_MSB_CPR11_TARG_VOLT_NOM_1_0_BMSK                            0xc0000000
#define HWIO_QFPROM_CORR_CALIB_ROW5_MSB_CPR11_TARG_VOLT_NOM_1_0_SHFT                                  0x1e
#define HWIO_QFPROM_CORR_CALIB_ROW5_MSB_CPR11_TARG_VOLT_TUR_BMSK                                0x3f000000
#define HWIO_QFPROM_CORR_CALIB_ROW5_MSB_CPR11_TARG_VOLT_TUR_SHFT                                      0x18
#define HWIO_QFPROM_CORR_CALIB_ROW5_MSB_CPR11_SVS2_ROSEL_BMSK                                     0xf00000
#define HWIO_QFPROM_CORR_CALIB_ROW5_MSB_CPR11_SVS2_ROSEL_SHFT                                         0x14
#define HWIO_QFPROM_CORR_CALIB_ROW5_MSB_CPR11_SVS_ROSEL_BMSK                                       0xf0000
#define HWIO_QFPROM_CORR_CALIB_ROW5_MSB_CPR11_SVS_ROSEL_SHFT                                          0x10
#define HWIO_QFPROM_CORR_CALIB_ROW5_MSB_CPR11_NOMINAL_ROSEL_BMSK                                    0xf000
#define HWIO_QFPROM_CORR_CALIB_ROW5_MSB_CPR11_NOMINAL_ROSEL_SHFT                                       0xc
#define HWIO_QFPROM_CORR_CALIB_ROW5_MSB_CPR11_TURBO_ROSEL_BMSK                                       0xf00
#define HWIO_QFPROM_CORR_CALIB_ROW5_MSB_CPR11_TURBO_ROSEL_SHFT                                         0x8
#define HWIO_QFPROM_CORR_CALIB_ROW5_MSB_SPARE_R68_B39_BMSK                                            0x80
#define HWIO_QFPROM_CORR_CALIB_ROW5_MSB_SPARE_R68_B39_SHFT                                             0x7
#define HWIO_QFPROM_CORR_CALIB_ROW5_MSB_CPR8_LDO_TARG_VOLT_SVS3_BMSK                                  0x78
#define HWIO_QFPROM_CORR_CALIB_ROW5_MSB_CPR8_LDO_TARG_VOLT_SVS3_SHFT                                   0x3
#define HWIO_QFPROM_CORR_CALIB_ROW5_MSB_SPARE_R68_B34_BMSK                                             0x4
#define HWIO_QFPROM_CORR_CALIB_ROW5_MSB_SPARE_R68_B34_SHFT                                             0x2
#define HWIO_QFPROM_CORR_CALIB_ROW5_MSB_CPR8_LDO_TARG_VOLT_SVS_3_2_BMSK                                0x3
#define HWIO_QFPROM_CORR_CALIB_ROW5_MSB_CPR8_LDO_TARG_VOLT_SVS_3_2_SHFT                                0x0

#define HWIO_QFPROM_CORR_CALIB_ROW6_LSB_ADDR                                                    (SECURITY_CONTROL_CORE_REG_BASE      + 0x00004228)
#define HWIO_QFPROM_CORR_CALIB_ROW6_LSB_RMSK                                                    0xffffffff
#define HWIO_QFPROM_CORR_CALIB_ROW6_LSB_IN          \
        in_dword_masked(HWIO_QFPROM_CORR_CALIB_ROW6_LSB_ADDR, HWIO_QFPROM_CORR_CALIB_ROW6_LSB_RMSK)
#define HWIO_QFPROM_CORR_CALIB_ROW6_LSB_INM(m)      \
        in_dword_masked(HWIO_QFPROM_CORR_CALIB_ROW6_LSB_ADDR, m)
#define HWIO_QFPROM_CORR_CALIB_ROW6_LSB_CPR11_NOMINAL_QUOT_VMIN_3_0_BMSK                        0xf0000000
#define HWIO_QFPROM_CORR_CALIB_ROW6_LSB_CPR11_NOMINAL_QUOT_VMIN_3_0_SHFT                              0x1c
#define HWIO_QFPROM_CORR_CALIB_ROW6_LSB_CPR11_TURBO_QUOT_VMIN_BMSK                               0xfff0000
#define HWIO_QFPROM_CORR_CALIB_ROW6_LSB_CPR11_TURBO_QUOT_VMIN_SHFT                                    0x10
#define HWIO_QFPROM_CORR_CALIB_ROW6_LSB_CPR11_TARG_VOLT_SVS2_BMSK                                   0xfc00
#define HWIO_QFPROM_CORR_CALIB_ROW6_LSB_CPR11_TARG_VOLT_SVS2_SHFT                                      0xa
#define HWIO_QFPROM_CORR_CALIB_ROW6_LSB_CPR11_TARG_VOLT_SVS_BMSK                                     0x3f0
#define HWIO_QFPROM_CORR_CALIB_ROW6_LSB_CPR11_TARG_VOLT_SVS_SHFT                                       0x4
#define HWIO_QFPROM_CORR_CALIB_ROW6_LSB_CPR11_TARG_VOLT_NOM_5_2_BMSK                                   0xf
#define HWIO_QFPROM_CORR_CALIB_ROW6_LSB_CPR11_TARG_VOLT_NOM_5_2_SHFT                                   0x0

#define HWIO_QFPROM_CORR_CALIB_ROW6_MSB_ADDR                                                    (SECURITY_CONTROL_CORE_REG_BASE      + 0x0000422c)
#define HWIO_QFPROM_CORR_CALIB_ROW6_MSB_RMSK                                                    0xffffffff
#define HWIO_QFPROM_CORR_CALIB_ROW6_MSB_IN          \
        in_dword_masked(HWIO_QFPROM_CORR_CALIB_ROW6_MSB_ADDR, HWIO_QFPROM_CORR_CALIB_ROW6_MSB_RMSK)
#define HWIO_QFPROM_CORR_CALIB_ROW6_MSB_INM(m)      \
        in_dword_masked(HWIO_QFPROM_CORR_CALIB_ROW6_MSB_ADDR, m)
#define HWIO_QFPROM_CORR_CALIB_ROW6_MSB_CPR11_SVS2_QUOT_VMIN_BMSK                               0xfff00000
#define HWIO_QFPROM_CORR_CALIB_ROW6_MSB_CPR11_SVS2_QUOT_VMIN_SHFT                                     0x14
#define HWIO_QFPROM_CORR_CALIB_ROW6_MSB_CPR11_SVS_QUOT_VMIN_BMSK                                   0xfff00
#define HWIO_QFPROM_CORR_CALIB_ROW6_MSB_CPR11_SVS_QUOT_VMIN_SHFT                                       0x8
#define HWIO_QFPROM_CORR_CALIB_ROW6_MSB_CPR11_NOMINAL_QUOT_VMIN_11_4_BMSK                             0xff
#define HWIO_QFPROM_CORR_CALIB_ROW6_MSB_CPR11_NOMINAL_QUOT_VMIN_11_4_SHFT                              0x0

#define HWIO_QFPROM_CORR_CALIB_ROW7_LSB_ADDR                                                    (SECURITY_CONTROL_CORE_REG_BASE      + 0x00004230)
#define HWIO_QFPROM_CORR_CALIB_ROW7_LSB_RMSK                                                    0xffffffff
#define HWIO_QFPROM_CORR_CALIB_ROW7_LSB_IN          \
        in_dword_masked(HWIO_QFPROM_CORR_CALIB_ROW7_LSB_ADDR, HWIO_QFPROM_CORR_CALIB_ROW7_LSB_RMSK)
#define HWIO_QFPROM_CORR_CALIB_ROW7_LSB_INM(m)      \
        in_dword_masked(HWIO_QFPROM_CORR_CALIB_ROW7_LSB_ADDR, m)
#define HWIO_QFPROM_CORR_CALIB_ROW7_LSB_CPR12_TARG_VOLT_TUR_2_0_BMSK                            0xe0000000
#define HWIO_QFPROM_CORR_CALIB_ROW7_LSB_CPR12_TARG_VOLT_TUR_2_0_SHFT                                  0x1d
#define HWIO_QFPROM_CORR_CALIB_ROW7_LSB_CPR11_AGING_BMSK                                        0x1fe00000
#define HWIO_QFPROM_CORR_CALIB_ROW7_LSB_CPR11_AGING_SHFT                                              0x15
#define HWIO_QFPROM_CORR_CALIB_ROW7_LSB_CPR11_QUOT_OFFSET_SVS_BMSK                                0x1fc000
#define HWIO_QFPROM_CORR_CALIB_ROW7_LSB_CPR11_QUOT_OFFSET_SVS_SHFT                                     0xe
#define HWIO_QFPROM_CORR_CALIB_ROW7_LSB_CPR11_QUOT_OFFSET_NOM_BMSK                                  0x3f80
#define HWIO_QFPROM_CORR_CALIB_ROW7_LSB_CPR11_QUOT_OFFSET_NOM_SHFT                                     0x7
#define HWIO_QFPROM_CORR_CALIB_ROW7_LSB_CPR_RESERVED_BMSK                                             0x7f
#define HWIO_QFPROM_CORR_CALIB_ROW7_LSB_CPR_RESERVED_SHFT                                              0x0

#define HWIO_QFPROM_CORR_CALIB_ROW7_MSB_ADDR                                                    (SECURITY_CONTROL_CORE_REG_BASE      + 0x00004234)
#define HWIO_QFPROM_CORR_CALIB_ROW7_MSB_RMSK                                                    0xffffffff
#define HWIO_QFPROM_CORR_CALIB_ROW7_MSB_IN          \
        in_dword_masked(HWIO_QFPROM_CORR_CALIB_ROW7_MSB_ADDR, HWIO_QFPROM_CORR_CALIB_ROW7_MSB_RMSK)
#define HWIO_QFPROM_CORR_CALIB_ROW7_MSB_INM(m)      \
        in_dword_masked(HWIO_QFPROM_CORR_CALIB_ROW7_MSB_ADDR, m)
#define HWIO_QFPROM_CORR_CALIB_ROW7_MSB_CPR12_TURBO_QUOT_VMIN_10_0_BMSK                         0xffe00000
#define HWIO_QFPROM_CORR_CALIB_ROW7_MSB_CPR12_TURBO_QUOT_VMIN_10_0_SHFT                               0x15
#define HWIO_QFPROM_CORR_CALIB_ROW7_MSB_CPR12_TARG_VOLT_SVS2_BMSK                                 0x1f8000
#define HWIO_QFPROM_CORR_CALIB_ROW7_MSB_CPR12_TARG_VOLT_SVS2_SHFT                                      0xf
#define HWIO_QFPROM_CORR_CALIB_ROW7_MSB_CPR12_TARG_VOLT_SVS_BMSK                                    0x7e00
#define HWIO_QFPROM_CORR_CALIB_ROW7_MSB_CPR12_TARG_VOLT_SVS_SHFT                                       0x9
#define HWIO_QFPROM_CORR_CALIB_ROW7_MSB_CPR12_TARG_VOLT_NOM_BMSK                                     0x1f8
#define HWIO_QFPROM_CORR_CALIB_ROW7_MSB_CPR12_TARG_VOLT_NOM_SHFT                                       0x3
#define HWIO_QFPROM_CORR_CALIB_ROW7_MSB_CPR12_TARG_VOLT_TUR_5_3_BMSK                                   0x7
#define HWIO_QFPROM_CORR_CALIB_ROW7_MSB_CPR12_TARG_VOLT_TUR_5_3_SHFT                                   0x0

#define HWIO_QFPROM_CORR_CALIB_ROW8_LSB_ADDR                                                    (SECURITY_CONTROL_CORE_REG_BASE      + 0x00004238)
#define HWIO_QFPROM_CORR_CALIB_ROW8_LSB_RMSK                                                    0xffffffff
#define HWIO_QFPROM_CORR_CALIB_ROW8_LSB_IN          \
        in_dword_masked(HWIO_QFPROM_CORR_CALIB_ROW8_LSB_ADDR, HWIO_QFPROM_CORR_CALIB_ROW8_LSB_RMSK)
#define HWIO_QFPROM_CORR_CALIB_ROW8_LSB_INM(m)      \
        in_dword_masked(HWIO_QFPROM_CORR_CALIB_ROW8_LSB_ADDR, m)
#define HWIO_QFPROM_CORR_CALIB_ROW8_LSB_CPR12_SVS2_QUOT_VMIN_6_0_BMSK                           0xfe000000
#define HWIO_QFPROM_CORR_CALIB_ROW8_LSB_CPR12_SVS2_QUOT_VMIN_6_0_SHFT                                 0x19
#define HWIO_QFPROM_CORR_CALIB_ROW8_LSB_CPR12_SVS_QUOT_VMIN_BMSK                                 0x1ffe000
#define HWIO_QFPROM_CORR_CALIB_ROW8_LSB_CPR12_SVS_QUOT_VMIN_SHFT                                       0xd
#define HWIO_QFPROM_CORR_CALIB_ROW8_LSB_CPR12_NOMINAL_QUOT_VMIN_BMSK                                0x1ffe
#define HWIO_QFPROM_CORR_CALIB_ROW8_LSB_CPR12_NOMINAL_QUOT_VMIN_SHFT                                   0x1
#define HWIO_QFPROM_CORR_CALIB_ROW8_LSB_CPR12_TURBO_QUOT_VMIN_11_BMSK                                  0x1
#define HWIO_QFPROM_CORR_CALIB_ROW8_LSB_CPR12_TURBO_QUOT_VMIN_11_SHFT                                  0x0

#define HWIO_QFPROM_CORR_CALIB_ROW8_MSB_ADDR                                                    (SECURITY_CONTROL_CORE_REG_BASE      + 0x0000423c)
#define HWIO_QFPROM_CORR_CALIB_ROW8_MSB_RMSK                                                    0xffffffff
#define HWIO_QFPROM_CORR_CALIB_ROW8_MSB_IN          \
        in_dword_masked(HWIO_QFPROM_CORR_CALIB_ROW8_MSB_ADDR, HWIO_QFPROM_CORR_CALIB_ROW8_MSB_RMSK)
#define HWIO_QFPROM_CORR_CALIB_ROW8_MSB_INM(m)      \
        in_dword_masked(HWIO_QFPROM_CORR_CALIB_ROW8_MSB_ADDR, m)
#define HWIO_QFPROM_CORR_CALIB_ROW8_MSB_SPARE_R71_B63_BMSK                                      0x80000000
#define HWIO_QFPROM_CORR_CALIB_ROW8_MSB_SPARE_R71_B63_SHFT                                            0x1f
#define HWIO_QFPROM_CORR_CALIB_ROW8_MSB_SPARE_R71_B62_BMSK                                      0x40000000
#define HWIO_QFPROM_CORR_CALIB_ROW8_MSB_SPARE_R71_B62_SHFT                                            0x1e
#define HWIO_QFPROM_CORR_CALIB_ROW8_MSB_CPR10_TARG_VOLT_OFFSET_TUR_BMSK                         0x3c000000
#define HWIO_QFPROM_CORR_CALIB_ROW8_MSB_CPR10_TARG_VOLT_OFFSET_TUR_SHFT                               0x1a
#define HWIO_QFPROM_CORR_CALIB_ROW8_MSB_CPR12_QUOT_OFFSET_SVS_BMSK                               0x3f80000
#define HWIO_QFPROM_CORR_CALIB_ROW8_MSB_CPR12_QUOT_OFFSET_SVS_SHFT                                    0x13
#define HWIO_QFPROM_CORR_CALIB_ROW8_MSB_CPR12_QUOT_OFFSET_NOM_BMSK                                 0x7f000
#define HWIO_QFPROM_CORR_CALIB_ROW8_MSB_CPR12_QUOT_OFFSET_NOM_SHFT                                     0xc
#define HWIO_QFPROM_CORR_CALIB_ROW8_MSB_CPR_RESERVED_BMSK                                            0xfe0
#define HWIO_QFPROM_CORR_CALIB_ROW8_MSB_CPR_RESERVED_SHFT                                              0x5
#define HWIO_QFPROM_CORR_CALIB_ROW8_MSB_CPR12_SVS2_QUOT_VMIN_11_7_BMSK                                0x1f
#define HWIO_QFPROM_CORR_CALIB_ROW8_MSB_CPR12_SVS2_QUOT_VMIN_11_7_SHFT                                 0x0

#define HWIO_QFPROM_CORR_CALIB_ROW9_LSB_ADDR                                                    (SECURITY_CONTROL_CORE_REG_BASE      + 0x00004240)
#define HWIO_QFPROM_CORR_CALIB_ROW9_LSB_RMSK                                                    0xffffffff
#define HWIO_QFPROM_CORR_CALIB_ROW9_LSB_IN          \
        in_dword_masked(HWIO_QFPROM_CORR_CALIB_ROW9_LSB_ADDR, HWIO_QFPROM_CORR_CALIB_ROW9_LSB_RMSK)
#define HWIO_QFPROM_CORR_CALIB_ROW9_LSB_INM(m)      \
        in_dword_masked(HWIO_QFPROM_CORR_CALIB_ROW9_LSB_ADDR, m)
#define HWIO_QFPROM_CORR_CALIB_ROW9_LSB_CPR13_TARG_VOLT_SVS_3_0_BMSK                            0xf0000000
#define HWIO_QFPROM_CORR_CALIB_ROW9_LSB_CPR13_TARG_VOLT_SVS_3_0_SHFT                                  0x1c
#define HWIO_QFPROM_CORR_CALIB_ROW9_LSB_CPR13_TARG_VOLT_NOM_BMSK                                 0xfc00000
#define HWIO_QFPROM_CORR_CALIB_ROW9_LSB_CPR13_TARG_VOLT_NOM_SHFT                                      0x16
#define HWIO_QFPROM_CORR_CALIB_ROW9_LSB_CPR13_TARG_VOLT_TUR_BMSK                                  0x3f0000
#define HWIO_QFPROM_CORR_CALIB_ROW9_LSB_CPR13_TARG_VOLT_TUR_SHFT                                      0x10
#define HWIO_QFPROM_CORR_CALIB_ROW9_LSB_CPR13_TARG_VOLT_SUT_BMSK                                    0xfc00
#define HWIO_QFPROM_CORR_CALIB_ROW9_LSB_CPR13_TARG_VOLT_SUT_SHFT                                       0xa
#define HWIO_QFPROM_CORR_CALIB_ROW9_LSB_CPR10_TARG_VOLT_OFFSET_SVS_BMSK                              0x3c0
#define HWIO_QFPROM_CORR_CALIB_ROW9_LSB_CPR10_TARG_VOLT_OFFSET_SVS_SHFT                                0x6
#define HWIO_QFPROM_CORR_CALIB_ROW9_LSB_CPR10_TARG_VOLT_OFFSET_NOM_BMSK                               0x3c
#define HWIO_QFPROM_CORR_CALIB_ROW9_LSB_CPR10_TARG_VOLT_OFFSET_NOM_SHFT                                0x2
#define HWIO_QFPROM_CORR_CALIB_ROW9_LSB_SPARE_R72_B1_BMSK                                              0x2
#define HWIO_QFPROM_CORR_CALIB_ROW9_LSB_SPARE_R72_B1_SHFT                                              0x1
#define HWIO_QFPROM_CORR_CALIB_ROW9_LSB_SPARE_R72_B0_BMSK                                              0x1
#define HWIO_QFPROM_CORR_CALIB_ROW9_LSB_SPARE_R72_B0_SHFT                                              0x0

#define HWIO_QFPROM_CORR_CALIB_ROW9_MSB_ADDR                                                    (SECURITY_CONTROL_CORE_REG_BASE      + 0x00004244)
#define HWIO_QFPROM_CORR_CALIB_ROW9_MSB_RMSK                                                    0xffffffff
#define HWIO_QFPROM_CORR_CALIB_ROW9_MSB_IN          \
        in_dword_masked(HWIO_QFPROM_CORR_CALIB_ROW9_MSB_ADDR, HWIO_QFPROM_CORR_CALIB_ROW9_MSB_RMSK)
#define HWIO_QFPROM_CORR_CALIB_ROW9_MSB_INM(m)      \
        in_dword_masked(HWIO_QFPROM_CORR_CALIB_ROW9_MSB_ADDR, m)
#define HWIO_QFPROM_CORR_CALIB_ROW9_MSB_CPR13_NOMINAL_QUOT_VMIN_5_0_BMSK                        0xfc000000
#define HWIO_QFPROM_CORR_CALIB_ROW9_MSB_CPR13_NOMINAL_QUOT_VMIN_5_0_SHFT                              0x1a
#define HWIO_QFPROM_CORR_CALIB_ROW9_MSB_CPR13_TURBO_QUOT_VMIN_BMSK                               0x3ffc000
#define HWIO_QFPROM_CORR_CALIB_ROW9_MSB_CPR13_TURBO_QUOT_VMIN_SHFT                                     0xe
#define HWIO_QFPROM_CORR_CALIB_ROW9_MSB_CPR13_SVS2_QUOT_VMIN_BMSK                                   0x3ffc
#define HWIO_QFPROM_CORR_CALIB_ROW9_MSB_CPR13_SVS2_QUOT_VMIN_SHFT                                      0x2
#define HWIO_QFPROM_CORR_CALIB_ROW9_MSB_CPR13_TARG_VOLT_SVS_5_4_BMSK                                   0x3
#define HWIO_QFPROM_CORR_CALIB_ROW9_MSB_CPR13_TARG_VOLT_SVS_5_4_SHFT                                   0x0

#define HWIO_QFPROM_CORR_CALIB_ROW10_LSB_ADDR                                                   (SECURITY_CONTROL_CORE_REG_BASE      + 0x00004248)
#define HWIO_QFPROM_CORR_CALIB_ROW10_LSB_RMSK                                                   0xffffffff
#define HWIO_QFPROM_CORR_CALIB_ROW10_LSB_IN          \
        in_dword_masked(HWIO_QFPROM_CORR_CALIB_ROW10_LSB_ADDR, HWIO_QFPROM_CORR_CALIB_ROW10_LSB_RMSK)
#define HWIO_QFPROM_CORR_CALIB_ROW10_LSB_INM(m)      \
        in_dword_masked(HWIO_QFPROM_CORR_CALIB_ROW10_LSB_ADDR, m)
#define HWIO_QFPROM_CORR_CALIB_ROW10_LSB_CPR13_QUOT_OFFSET_TUR_BMSK                             0xfe000000
#define HWIO_QFPROM_CORR_CALIB_ROW10_LSB_CPR13_QUOT_OFFSET_TUR_SHFT                                   0x19
#define HWIO_QFPROM_CORR_CALIB_ROW10_LSB_CPR13_QUOT_OFFSET_SVS_BMSK                              0x1fc0000
#define HWIO_QFPROM_CORR_CALIB_ROW10_LSB_CPR13_QUOT_OFFSET_SVS_SHFT                                   0x12
#define HWIO_QFPROM_CORR_CALIB_ROW10_LSB_CPR13_SVS_QUOT_VMIN_BMSK                                  0x3ffc0
#define HWIO_QFPROM_CORR_CALIB_ROW10_LSB_CPR13_SVS_QUOT_VMIN_SHFT                                      0x6
#define HWIO_QFPROM_CORR_CALIB_ROW10_LSB_CPR13_NOMINAL_QUOT_VMIN_11_6_BMSK                            0x3f
#define HWIO_QFPROM_CORR_CALIB_ROW10_LSB_CPR13_NOMINAL_QUOT_VMIN_11_6_SHFT                             0x0

#define HWIO_QFPROM_CORR_CALIB_ROW10_MSB_ADDR                                                   (SECURITY_CONTROL_CORE_REG_BASE      + 0x0000424c)
#define HWIO_QFPROM_CORR_CALIB_ROW10_MSB_RMSK                                                   0xffffffff
#define HWIO_QFPROM_CORR_CALIB_ROW10_MSB_IN          \
        in_dword_masked(HWIO_QFPROM_CORR_CALIB_ROW10_MSB_ADDR, HWIO_QFPROM_CORR_CALIB_ROW10_MSB_RMSK)
#define HWIO_QFPROM_CORR_CALIB_ROW10_MSB_INM(m)      \
        in_dword_masked(HWIO_QFPROM_CORR_CALIB_ROW10_MSB_ADDR, m)
#define HWIO_QFPROM_CORR_CALIB_ROW10_MSB_SPARE_R73_B63_BMSK                                     0x80000000
#define HWIO_QFPROM_CORR_CALIB_ROW10_MSB_SPARE_R73_B63_SHFT                                           0x1f
#define HWIO_QFPROM_CORR_CALIB_ROW10_MSB_SPARE_R73_B62_BMSK                                     0x40000000
#define HWIO_QFPROM_CORR_CALIB_ROW10_MSB_SPARE_R73_B62_SHFT                                           0x1e
#define HWIO_QFPROM_CORR_CALIB_ROW10_MSB_CPR_SSC_CX_MODE_DISABLE_BMSK                           0x38000000
#define HWIO_QFPROM_CORR_CALIB_ROW10_MSB_CPR_SSC_CX_MODE_DISABLE_SHFT                                 0x1b
#define HWIO_QFPROM_CORR_CALIB_ROW10_MSB_CPR_DDR_MODE_DISABLE_BMSK                               0x7000000
#define HWIO_QFPROM_CORR_CALIB_ROW10_MSB_CPR_DDR_MODE_DISABLE_SHFT                                    0x18
#define HWIO_QFPROM_CORR_CALIB_ROW10_MSB_CPR_GFX_MODE_DISABLE_BMSK                                0xe00000
#define HWIO_QFPROM_CORR_CALIB_ROW10_MSB_CPR_GFX_MODE_DISABLE_SHFT                                    0x15
#define HWIO_QFPROM_CORR_CALIB_ROW10_MSB_CPR_MSS_MODE_DISABLE_BMSK                                0x1c0000
#define HWIO_QFPROM_CORR_CALIB_ROW10_MSB_CPR_MSS_MODE_DISABLE_SHFT                                    0x12
#define HWIO_QFPROM_CORR_CALIB_ROW10_MSB_CPR_CX_MODE_DISABLE_BMSK                                  0x38000
#define HWIO_QFPROM_CORR_CALIB_ROW10_MSB_CPR_CX_MODE_DISABLE_SHFT                                      0xf
#define HWIO_QFPROM_CORR_CALIB_ROW10_MSB_CPR13_AGING_BMSK                                           0x7f80
#define HWIO_QFPROM_CORR_CALIB_ROW10_MSB_CPR13_AGING_SHFT                                              0x7
#define HWIO_QFPROM_CORR_CALIB_ROW10_MSB_CPR13_QUOT_OFFSET_NOM_BMSK                                   0x7f
#define HWIO_QFPROM_CORR_CALIB_ROW10_MSB_CPR13_QUOT_OFFSET_NOM_SHFT                                    0x0

#define HWIO_QFPROM_CORR_CALIB_ROW11_LSB_ADDR                                                   (SECURITY_CONTROL_CORE_REG_BASE      + 0x00004250)
#define HWIO_QFPROM_CORR_CALIB_ROW11_LSB_RMSK                                                   0xffffffff
#define HWIO_QFPROM_CORR_CALIB_ROW11_LSB_IN          \
        in_dword_masked(HWIO_QFPROM_CORR_CALIB_ROW11_LSB_ADDR, HWIO_QFPROM_CORR_CALIB_ROW11_LSB_RMSK)
#define HWIO_QFPROM_CORR_CALIB_ROW11_LSB_INM(m)      \
        in_dword_masked(HWIO_QFPROM_CORR_CALIB_ROW11_LSB_ADDR, m)
#define HWIO_QFPROM_CORR_CALIB_ROW11_LSB_CPR1_TARG_VOLT_OFFSET_SVS3_0_BMSK                      0x80000000
#define HWIO_QFPROM_CORR_CALIB_ROW11_LSB_CPR1_TARG_VOLT_OFFSET_SVS3_0_SHFT                            0x1f
#define HWIO_QFPROM_CORR_CALIB_ROW11_LSB_CPR0_TARG_VOLT_OFFSET_SVS3_BMSK                        0x78000000
#define HWIO_QFPROM_CORR_CALIB_ROW11_LSB_CPR0_TARG_VOLT_OFFSET_SVS3_SHFT                              0x1b
#define HWIO_QFPROM_CORR_CALIB_ROW11_LSB_CPR10_AGING_BMSK                                        0x7f80000
#define HWIO_QFPROM_CORR_CALIB_ROW11_LSB_CPR10_AGING_SHFT                                             0x13
#define HWIO_QFPROM_CORR_CALIB_ROW11_LSB_CPR12_QUOT_OFFSET_TUR_BMSK                                0x7f800
#define HWIO_QFPROM_CORR_CALIB_ROW11_LSB_CPR12_QUOT_OFFSET_TUR_SHFT                                    0xb
#define HWIO_QFPROM_CORR_CALIB_ROW11_LSB_CPR11_QUOT_OFFSET_TUR_BMSK                                  0x7f8
#define HWIO_QFPROM_CORR_CALIB_ROW11_LSB_CPR11_QUOT_OFFSET_TUR_SHFT                                    0x3
#define HWIO_QFPROM_CORR_CALIB_ROW11_LSB_CPR_LDO_RC_BMSK                                               0x7
#define HWIO_QFPROM_CORR_CALIB_ROW11_LSB_CPR_LDO_RC_SHFT                                               0x0

#define HWIO_QFPROM_CORR_CALIB_ROW11_MSB_ADDR                                                   (SECURITY_CONTROL_CORE_REG_BASE      + 0x00004254)
#define HWIO_QFPROM_CORR_CALIB_ROW11_MSB_RMSK                                                    0xfffffff
#define HWIO_QFPROM_CORR_CALIB_ROW11_MSB_IN          \
        in_dword_masked(HWIO_QFPROM_CORR_CALIB_ROW11_MSB_ADDR, HWIO_QFPROM_CORR_CALIB_ROW11_MSB_RMSK)
#define HWIO_QFPROM_CORR_CALIB_ROW11_MSB_INM(m)      \
        in_dword_masked(HWIO_QFPROM_CORR_CALIB_ROW11_MSB_ADDR, m)
#define HWIO_QFPROM_CORR_CALIB_ROW11_MSB_CPR9_TARG_VOLT_NOM_BMSK                                 0xf800000
#define HWIO_QFPROM_CORR_CALIB_ROW11_MSB_CPR9_TARG_VOLT_NOM_SHFT                                      0x17
#define HWIO_QFPROM_CORR_CALIB_ROW11_MSB_AON_AGING_BMSK                                           0x7f8000
#define HWIO_QFPROM_CORR_CALIB_ROW11_MSB_AON_AGING_SHFT                                                0xf
#define HWIO_QFPROM_CORR_CALIB_ROW11_MSB_CPR10_TARG_VOLT_OFFSET_SVS2_BMSK                           0x7800
#define HWIO_QFPROM_CORR_CALIB_ROW11_MSB_CPR10_TARG_VOLT_OFFSET_SVS2_SHFT                              0xb
#define HWIO_QFPROM_CORR_CALIB_ROW11_MSB_SPARE_R74_B42_BMSK                                          0x400
#define HWIO_QFPROM_CORR_CALIB_ROW11_MSB_SPARE_R74_B42_SHFT                                            0xa
#define HWIO_QFPROM_CORR_CALIB_ROW11_MSB_SPARE_R74_B41_BMSK                                          0x200
#define HWIO_QFPROM_CORR_CALIB_ROW11_MSB_SPARE_R74_B41_SHFT                                            0x9
#define HWIO_QFPROM_CORR_CALIB_ROW11_MSB_SPARE_R74_B40_BMSK                                          0x100
#define HWIO_QFPROM_CORR_CALIB_ROW11_MSB_SPARE_R74_B40_SHFT                                            0x8
#define HWIO_QFPROM_CORR_CALIB_ROW11_MSB_SPARE_R74_B39_BMSK                                           0x80
#define HWIO_QFPROM_CORR_CALIB_ROW11_MSB_SPARE_R74_B39_SHFT                                            0x7
#define HWIO_QFPROM_CORR_CALIB_ROW11_MSB_CPR2_TARG_VOLT_OFFSET_SVS3_BMSK                              0x78
#define HWIO_QFPROM_CORR_CALIB_ROW11_MSB_CPR2_TARG_VOLT_OFFSET_SVS3_SHFT                               0x3
#define HWIO_QFPROM_CORR_CALIB_ROW11_MSB_CPR1_TARG_VOLT_OFFSET_SVS3_3_1_BMSK                           0x7
#define HWIO_QFPROM_CORR_CALIB_ROW11_MSB_CPR1_TARG_VOLT_OFFSET_SVS3_3_1_SHFT                           0x0

#define HWIO_QFPROM_CORR_CALIB_ROW12_LSB_ADDR                                                   (SECURITY_CONTROL_CORE_REG_BASE      + 0x00004258)
#define HWIO_QFPROM_CORR_CALIB_ROW12_LSB_RMSK                                                   0xffffffff
#define HWIO_QFPROM_CORR_CALIB_ROW12_LSB_IN          \
        in_dword_masked(HWIO_QFPROM_CORR_CALIB_ROW12_LSB_ADDR, HWIO_QFPROM_CORR_CALIB_ROW12_LSB_RMSK)
#define HWIO_QFPROM_CORR_CALIB_ROW12_LSB_INM(m)      \
        in_dword_masked(HWIO_QFPROM_CORR_CALIB_ROW12_LSB_ADDR, m)
#define HWIO_QFPROM_CORR_CALIB_ROW12_LSB_ISENSE_PMOS_COMP_BMSK                                  0xf0000000
#define HWIO_QFPROM_CORR_CALIB_ROW12_LSB_ISENSE_PMOS_COMP_SHFT                                        0x1c
#define HWIO_QFPROM_CORR_CALIB_ROW12_LSB_SPARE_R73_B27_BMSK                                      0x8000000
#define HWIO_QFPROM_CORR_CALIB_ROW12_LSB_SPARE_R73_B27_SHFT                                           0x1b
#define HWIO_QFPROM_CORR_CALIB_ROW12_LSB_SPARE_R73_B26_BMSK                                      0x4000000
#define HWIO_QFPROM_CORR_CALIB_ROW12_LSB_SPARE_R73_B26_SHFT                                           0x1a
#define HWIO_QFPROM_CORR_CALIB_ROW12_LSB_SPARE_R73_B25_BMSK                                      0x2000000
#define HWIO_QFPROM_CORR_CALIB_ROW12_LSB_SPARE_R73_B25_SHFT                                           0x19
#define HWIO_QFPROM_CORR_CALIB_ROW12_LSB_SRAM_AGING_SENSOR_1_BMSK                                0x1fe0000
#define HWIO_QFPROM_CORR_CALIB_ROW12_LSB_SRAM_AGING_SENSOR_1_SHFT                                     0x11
#define HWIO_QFPROM_CORR_CALIB_ROW12_LSB_SRAM_AGING_SENSOR_0_BMSK                                  0x1fe00
#define HWIO_QFPROM_CORR_CALIB_ROW12_LSB_SRAM_AGING_SENSOR_0_SHFT                                      0x9
#define HWIO_QFPROM_CORR_CALIB_ROW12_LSB_SPARE_73_8_BMSK                                             0x100
#define HWIO_QFPROM_CORR_CALIB_ROW12_LSB_SPARE_73_8_SHFT                                               0x8
#define HWIO_QFPROM_CORR_CALIB_ROW12_LSB_SPARE_73_7_BMSK                                              0x80
#define HWIO_QFPROM_CORR_CALIB_ROW12_LSB_SPARE_73_7_SHFT                                               0x7
#define HWIO_QFPROM_CORR_CALIB_ROW12_LSB_SPARE_73_6_BMSK                                              0x40
#define HWIO_QFPROM_CORR_CALIB_ROW12_LSB_SPARE_73_6_SHFT                                               0x6
#define HWIO_QFPROM_CORR_CALIB_ROW12_LSB_CPR_LOCAL_RC_BMSK                                            0x38
#define HWIO_QFPROM_CORR_CALIB_ROW12_LSB_CPR_LOCAL_RC_SHFT                                             0x3
#define HWIO_QFPROM_CORR_CALIB_ROW12_LSB_CPR_GLOBAL_RC_BMSK                                            0x7
#define HWIO_QFPROM_CORR_CALIB_ROW12_LSB_CPR_GLOBAL_RC_SHFT                                            0x0

#define HWIO_QFPROM_CORR_CALIB_ROW12_MSB_ADDR                                                   (SECURITY_CONTROL_CORE_REG_BASE      + 0x0000425c)
#define HWIO_QFPROM_CORR_CALIB_ROW12_MSB_RMSK                                                   0xffffffff
#define HWIO_QFPROM_CORR_CALIB_ROW12_MSB_IN          \
        in_dword_masked(HWIO_QFPROM_CORR_CALIB_ROW12_MSB_ADDR, HWIO_QFPROM_CORR_CALIB_ROW12_MSB_RMSK)
#define HWIO_QFPROM_CORR_CALIB_ROW12_MSB_INM(m)      \
        in_dword_masked(HWIO_QFPROM_CORR_CALIB_ROW12_MSB_ADDR, m)
#define HWIO_QFPROM_CORR_CALIB_ROW12_MSB_TSENS1_BASE1_1_0_BMSK                                  0xc0000000
#define HWIO_QFPROM_CORR_CALIB_ROW12_MSB_TSENS1_BASE1_1_0_SHFT                                        0x1e
#define HWIO_QFPROM_CORR_CALIB_ROW12_MSB_TSENS1_BASE0_BMSK                                      0x3ff00000
#define HWIO_QFPROM_CORR_CALIB_ROW12_MSB_TSENS1_BASE0_SHFT                                            0x14
#define HWIO_QFPROM_CORR_CALIB_ROW12_MSB_TSENS0_BASE1_BMSK                                         0xffc00
#define HWIO_QFPROM_CORR_CALIB_ROW12_MSB_TSENS0_BASE1_SHFT                                             0xa
#define HWIO_QFPROM_CORR_CALIB_ROW12_MSB_TSENS0_BASE0_BMSK                                           0x3ff
#define HWIO_QFPROM_CORR_CALIB_ROW12_MSB_TSENS0_BASE0_SHFT                                             0x0

#define HWIO_QFPROM_CORR_CALIB_ROW13_LSB_ADDR                                                   (SECURITY_CONTROL_CORE_REG_BASE      + 0x00004260)
#define HWIO_QFPROM_CORR_CALIB_ROW13_LSB_RMSK                                                   0xffffffff
#define HWIO_QFPROM_CORR_CALIB_ROW13_LSB_IN          \
        in_dword_masked(HWIO_QFPROM_CORR_CALIB_ROW13_LSB_ADDR, HWIO_QFPROM_CORR_CALIB_ROW13_LSB_RMSK)
#define HWIO_QFPROM_CORR_CALIB_ROW13_LSB_INM(m)      \
        in_dword_masked(HWIO_QFPROM_CORR_CALIB_ROW13_LSB_ADDR, m)
#define HWIO_QFPROM_CORR_CALIB_ROW13_LSB_TSENS4_OFFSET_3_0_BMSK                                 0xf0000000
#define HWIO_QFPROM_CORR_CALIB_ROW13_LSB_TSENS4_OFFSET_3_0_SHFT                                       0x1c
#define HWIO_QFPROM_CORR_CALIB_ROW13_LSB_TSENS3_OFFSET_BMSK                                      0xf800000
#define HWIO_QFPROM_CORR_CALIB_ROW13_LSB_TSENS3_OFFSET_SHFT                                           0x17
#define HWIO_QFPROM_CORR_CALIB_ROW13_LSB_TSENS2_OFFSET_BMSK                                       0x7c0000
#define HWIO_QFPROM_CORR_CALIB_ROW13_LSB_TSENS2_OFFSET_SHFT                                           0x12
#define HWIO_QFPROM_CORR_CALIB_ROW13_LSB_TSENS1_OFFSET_BMSK                                        0x3e000
#define HWIO_QFPROM_CORR_CALIB_ROW13_LSB_TSENS1_OFFSET_SHFT                                            0xd
#define HWIO_QFPROM_CORR_CALIB_ROW13_LSB_TSENS0_OFFSET_BMSK                                         0x1f00
#define HWIO_QFPROM_CORR_CALIB_ROW13_LSB_TSENS0_OFFSET_SHFT                                            0x8
#define HWIO_QFPROM_CORR_CALIB_ROW13_LSB_TSENS1_BASE1_9_2_BMSK                                        0xff
#define HWIO_QFPROM_CORR_CALIB_ROW13_LSB_TSENS1_BASE1_9_2_SHFT                                         0x0

#define HWIO_QFPROM_CORR_CALIB_ROW13_MSB_ADDR                                                   (SECURITY_CONTROL_CORE_REG_BASE      + 0x00004264)
#define HWIO_QFPROM_CORR_CALIB_ROW13_MSB_RMSK                                                   0xffffffff
#define HWIO_QFPROM_CORR_CALIB_ROW13_MSB_IN          \
        in_dword_masked(HWIO_QFPROM_CORR_CALIB_ROW13_MSB_ADDR, HWIO_QFPROM_CORR_CALIB_ROW13_MSB_RMSK)
#define HWIO_QFPROM_CORR_CALIB_ROW13_MSB_INM(m)      \
        in_dword_masked(HWIO_QFPROM_CORR_CALIB_ROW13_MSB_ADDR, m)
#define HWIO_QFPROM_CORR_CALIB_ROW13_MSB_TSENS11_OFFSET_0_BMSK                                  0x80000000
#define HWIO_QFPROM_CORR_CALIB_ROW13_MSB_TSENS11_OFFSET_0_SHFT                                        0x1f
#define HWIO_QFPROM_CORR_CALIB_ROW13_MSB_TSENS10_OFFSET_BMSK                                    0x7c000000
#define HWIO_QFPROM_CORR_CALIB_ROW13_MSB_TSENS10_OFFSET_SHFT                                          0x1a
#define HWIO_QFPROM_CORR_CALIB_ROW13_MSB_TSENS9_OFFSET_BMSK                                      0x3e00000
#define HWIO_QFPROM_CORR_CALIB_ROW13_MSB_TSENS9_OFFSET_SHFT                                           0x15
#define HWIO_QFPROM_CORR_CALIB_ROW13_MSB_TSENS8_OFFSET_BMSK                                       0x1f0000
#define HWIO_QFPROM_CORR_CALIB_ROW13_MSB_TSENS8_OFFSET_SHFT                                           0x10
#define HWIO_QFPROM_CORR_CALIB_ROW13_MSB_TSENS7_OFFSET_BMSK                                         0xf800
#define HWIO_QFPROM_CORR_CALIB_ROW13_MSB_TSENS7_OFFSET_SHFT                                            0xb
#define HWIO_QFPROM_CORR_CALIB_ROW13_MSB_TSENS6_OFFSET_BMSK                                          0x7c0
#define HWIO_QFPROM_CORR_CALIB_ROW13_MSB_TSENS6_OFFSET_SHFT                                            0x6
#define HWIO_QFPROM_CORR_CALIB_ROW13_MSB_TSENS5_OFFSET_BMSK                                           0x3e
#define HWIO_QFPROM_CORR_CALIB_ROW13_MSB_TSENS5_OFFSET_SHFT                                            0x1
#define HWIO_QFPROM_CORR_CALIB_ROW13_MSB_TSENS4_OFFSET_4_BMSK                                          0x1
#define HWIO_QFPROM_CORR_CALIB_ROW13_MSB_TSENS4_OFFSET_4_SHFT                                          0x0

#define HWIO_QFPROM_CORR_CALIB_ROW14_LSB_ADDR                                                   (SECURITY_CONTROL_CORE_REG_BASE      + 0x00004268)
#define HWIO_QFPROM_CORR_CALIB_ROW14_LSB_RMSK                                                   0xffffffff
#define HWIO_QFPROM_CORR_CALIB_ROW14_LSB_IN          \
        in_dword_masked(HWIO_QFPROM_CORR_CALIB_ROW14_LSB_ADDR, HWIO_QFPROM_CORR_CALIB_ROW14_LSB_RMSK)
#define HWIO_QFPROM_CORR_CALIB_ROW14_LSB_INM(m)      \
        in_dword_masked(HWIO_QFPROM_CORR_CALIB_ROW14_LSB_ADDR, m)
#define HWIO_QFPROM_CORR_CALIB_ROW14_LSB_SPARE_75_31_BMSK                                       0x80000000
#define HWIO_QFPROM_CORR_CALIB_ROW14_LSB_SPARE_75_31_SHFT                                             0x1f
#define HWIO_QFPROM_CORR_CALIB_ROW14_LSB_TSENS_CAL_SEL_BMSK                                     0x70000000
#define HWIO_QFPROM_CORR_CALIB_ROW14_LSB_TSENS_CAL_SEL_SHFT                                           0x1c
#define HWIO_QFPROM_CORR_CALIB_ROW14_LSB_SPARE_R77_B27_BMSK                                      0x8000000
#define HWIO_QFPROM_CORR_CALIB_ROW14_LSB_SPARE_R77_B27_SHFT                                           0x1b
#define HWIO_QFPROM_CORR_CALIB_ROW14_LSB_SPARE_R77_B26_BMSK                                      0x4000000
#define HWIO_QFPROM_CORR_CALIB_ROW14_LSB_SPARE_R77_B26_SHFT                                           0x1a
#define HWIO_QFPROM_CORR_CALIB_ROW14_LSB_SPARE_R77_B25_BMSK                                      0x2000000
#define HWIO_QFPROM_CORR_CALIB_ROW14_LSB_SPARE_R77_B25_SHFT                                           0x19
#define HWIO_QFPROM_CORR_CALIB_ROW14_LSB_SPARE_R77_B24_BMSK                                      0x1000000
#define HWIO_QFPROM_CORR_CALIB_ROW14_LSB_SPARE_R77_B24_SHFT                                           0x18
#define HWIO_QFPROM_CORR_CALIB_ROW14_LSB_TSENS15_OFFSET_BMSK                                      0xf80000
#define HWIO_QFPROM_CORR_CALIB_ROW14_LSB_TSENS15_OFFSET_SHFT                                          0x13
#define HWIO_QFPROM_CORR_CALIB_ROW14_LSB_TSENS14_OFFSET_BMSK                                       0x7c000
#define HWIO_QFPROM_CORR_CALIB_ROW14_LSB_TSENS14_OFFSET_SHFT                                           0xe
#define HWIO_QFPROM_CORR_CALIB_ROW14_LSB_TSENS13_OFFSET_BMSK                                        0x3e00
#define HWIO_QFPROM_CORR_CALIB_ROW14_LSB_TSENS13_OFFSET_SHFT                                           0x9
#define HWIO_QFPROM_CORR_CALIB_ROW14_LSB_TSENS12_OFFSET_BMSK                                         0x1f0
#define HWIO_QFPROM_CORR_CALIB_ROW14_LSB_TSENS12_OFFSET_SHFT                                           0x4
#define HWIO_QFPROM_CORR_CALIB_ROW14_LSB_TSENS11_OFFSET_4_1_BMSK                                       0xf
#define HWIO_QFPROM_CORR_CALIB_ROW14_LSB_TSENS11_OFFSET_4_1_SHFT                                       0x0

#define HWIO_QFPROM_CORR_CALIB_ROW14_MSB_ADDR                                                   (SECURITY_CONTROL_CORE_REG_BASE      + 0x0000426c)
#define HWIO_QFPROM_CORR_CALIB_ROW14_MSB_RMSK                                                   0xffffffff
#define HWIO_QFPROM_CORR_CALIB_ROW14_MSB_IN          \
        in_dword_masked(HWIO_QFPROM_CORR_CALIB_ROW14_MSB_ADDR, HWIO_QFPROM_CORR_CALIB_ROW14_MSB_RMSK)
#define HWIO_QFPROM_CORR_CALIB_ROW14_MSB_INM(m)      \
        in_dword_masked(HWIO_QFPROM_CORR_CALIB_ROW14_MSB_ADDR, m)
#define HWIO_QFPROM_CORR_CALIB_ROW14_MSB_SPARE_75_63_BMSK                                       0x80000000
#define HWIO_QFPROM_CORR_CALIB_ROW14_MSB_SPARE_75_63_SHFT                                             0x1f
#define HWIO_QFPROM_CORR_CALIB_ROW14_MSB_SPARE_75_62_BMSK                                       0x40000000
#define HWIO_QFPROM_CORR_CALIB_ROW14_MSB_SPARE_75_62_SHFT                                             0x1e
#define HWIO_QFPROM_CORR_CALIB_ROW14_MSB_SPARE_75_61_BMSK                                       0x20000000
#define HWIO_QFPROM_CORR_CALIB_ROW14_MSB_SPARE_75_61_SHFT                                             0x1d
#define HWIO_QFPROM_CORR_CALIB_ROW14_MSB_SPARE_75_60_BMSK                                       0x10000000
#define HWIO_QFPROM_CORR_CALIB_ROW14_MSB_SPARE_75_60_SHFT                                             0x1c
#define HWIO_QFPROM_CORR_CALIB_ROW14_MSB_SPARE_75_59_BMSK                                        0x8000000
#define HWIO_QFPROM_CORR_CALIB_ROW14_MSB_SPARE_75_59_SHFT                                             0x1b
#define HWIO_QFPROM_CORR_CALIB_ROW14_MSB_SPARE_75_58_BMSK                                        0x4000000
#define HWIO_QFPROM_CORR_CALIB_ROW14_MSB_SPARE_75_58_SHFT                                             0x1a
#define HWIO_QFPROM_CORR_CALIB_ROW14_MSB_SPARE_75_57_BMSK                                        0x2000000
#define HWIO_QFPROM_CORR_CALIB_ROW14_MSB_SPARE_75_57_SHFT                                             0x19
#define HWIO_QFPROM_CORR_CALIB_ROW14_MSB_SPARE_75_56_BMSK                                        0x1000000
#define HWIO_QFPROM_CORR_CALIB_ROW14_MSB_SPARE_75_56_SHFT                                             0x18
#define HWIO_QFPROM_CORR_CALIB_ROW14_MSB_SPARE_75_55_BMSK                                         0x800000
#define HWIO_QFPROM_CORR_CALIB_ROW14_MSB_SPARE_75_55_SHFT                                             0x17
#define HWIO_QFPROM_CORR_CALIB_ROW14_MSB_SPARE_75_54_BMSK                                         0x400000
#define HWIO_QFPROM_CORR_CALIB_ROW14_MSB_SPARE_75_54_SHFT                                             0x16
#define HWIO_QFPROM_CORR_CALIB_ROW14_MSB_SPARE_75_53_BMSK                                         0x200000
#define HWIO_QFPROM_CORR_CALIB_ROW14_MSB_SPARE_75_53_SHFT                                             0x15
#define HWIO_QFPROM_CORR_CALIB_ROW14_MSB_SPARE_75_52_BMSK                                         0x100000
#define HWIO_QFPROM_CORR_CALIB_ROW14_MSB_SPARE_75_52_SHFT                                             0x14
#define HWIO_QFPROM_CORR_CALIB_ROW14_MSB_SPARE_75_51_BMSK                                          0x80000
#define HWIO_QFPROM_CORR_CALIB_ROW14_MSB_SPARE_75_51_SHFT                                             0x13
#define HWIO_QFPROM_CORR_CALIB_ROW14_MSB_SPARE_75_50_BMSK                                          0x40000
#define HWIO_QFPROM_CORR_CALIB_ROW14_MSB_SPARE_75_50_SHFT                                             0x12
#define HWIO_QFPROM_CORR_CALIB_ROW14_MSB_SPARE_75_49_BMSK                                          0x20000
#define HWIO_QFPROM_CORR_CALIB_ROW14_MSB_SPARE_75_49_SHFT                                             0x11
#define HWIO_QFPROM_CORR_CALIB_ROW14_MSB_SPARE_75_48_BMSK                                          0x10000
#define HWIO_QFPROM_CORR_CALIB_ROW14_MSB_SPARE_75_48_SHFT                                             0x10
#define HWIO_QFPROM_CORR_CALIB_ROW14_MSB_SPARE_75_47_BMSK                                           0x8000
#define HWIO_QFPROM_CORR_CALIB_ROW14_MSB_SPARE_75_47_SHFT                                              0xf
#define HWIO_QFPROM_CORR_CALIB_ROW14_MSB_SPARE_75_46_BMSK                                           0x4000
#define HWIO_QFPROM_CORR_CALIB_ROW14_MSB_SPARE_75_46_SHFT                                              0xe
#define HWIO_QFPROM_CORR_CALIB_ROW14_MSB_SPARE_75_45_BMSK                                           0x2000
#define HWIO_QFPROM_CORR_CALIB_ROW14_MSB_SPARE_75_45_SHFT                                              0xd
#define HWIO_QFPROM_CORR_CALIB_ROW14_MSB_SPARE_75_44_BMSK                                           0x1000
#define HWIO_QFPROM_CORR_CALIB_ROW14_MSB_SPARE_75_44_SHFT                                              0xc
#define HWIO_QFPROM_CORR_CALIB_ROW14_MSB_SPARE_75_43_BMSK                                            0x800
#define HWIO_QFPROM_CORR_CALIB_ROW14_MSB_SPARE_75_43_SHFT                                              0xb
#define HWIO_QFPROM_CORR_CALIB_ROW14_MSB_SPARE_75_42_BMSK                                            0x400
#define HWIO_QFPROM_CORR_CALIB_ROW14_MSB_SPARE_75_42_SHFT                                              0xa
#define HWIO_QFPROM_CORR_CALIB_ROW14_MSB_SPARE_75_41_BMSK                                            0x200
#define HWIO_QFPROM_CORR_CALIB_ROW14_MSB_SPARE_75_41_SHFT                                              0x9
#define HWIO_QFPROM_CORR_CALIB_ROW14_MSB_SPARE_75_40_BMSK                                            0x100
#define HWIO_QFPROM_CORR_CALIB_ROW14_MSB_SPARE_75_40_SHFT                                              0x8
#define HWIO_QFPROM_CORR_CALIB_ROW14_MSB_SPARE_75_39_BMSK                                             0x80
#define HWIO_QFPROM_CORR_CALIB_ROW14_MSB_SPARE_75_39_SHFT                                              0x7
#define HWIO_QFPROM_CORR_CALIB_ROW14_MSB_SPARE_75_38_BMSK                                             0x40
#define HWIO_QFPROM_CORR_CALIB_ROW14_MSB_SPARE_75_38_SHFT                                              0x6
#define HWIO_QFPROM_CORR_CALIB_ROW14_MSB_SPARE_75_37_BMSK                                             0x20
#define HWIO_QFPROM_CORR_CALIB_ROW14_MSB_SPARE_75_37_SHFT                                              0x5
#define HWIO_QFPROM_CORR_CALIB_ROW14_MSB_SPARE_75_36_BMSK                                             0x10
#define HWIO_QFPROM_CORR_CALIB_ROW14_MSB_SPARE_75_36_SHFT                                              0x4
#define HWIO_QFPROM_CORR_CALIB_ROW14_MSB_SPARE_75_35_BMSK                                              0x8
#define HWIO_QFPROM_CORR_CALIB_ROW14_MSB_SPARE_75_35_SHFT                                              0x3
#define HWIO_QFPROM_CORR_CALIB_ROW14_MSB_SPARE_75_34_BMSK                                              0x4
#define HWIO_QFPROM_CORR_CALIB_ROW14_MSB_SPARE_75_34_SHFT                                              0x2
#define HWIO_QFPROM_CORR_CALIB_ROW14_MSB_SPARE_75_33_BMSK                                              0x2
#define HWIO_QFPROM_CORR_CALIB_ROW14_MSB_SPARE_75_33_SHFT                                              0x1
#define HWIO_QFPROM_CORR_CALIB_ROW14_MSB_SPARE_75_32_BMSK                                              0x1
#define HWIO_QFPROM_CORR_CALIB_ROW14_MSB_SPARE_75_32_SHFT                                              0x0

#define HWIO_QFPROM_CORR_CALIB_ROW15_LSB_ADDR                                                   (SECURITY_CONTROL_CORE_REG_BASE      + 0x00004270)
#define HWIO_QFPROM_CORR_CALIB_ROW15_LSB_RMSK                                                   0xffffffff
#define HWIO_QFPROM_CORR_CALIB_ROW15_LSB_IN          \
        in_dword_masked(HWIO_QFPROM_CORR_CALIB_ROW15_LSB_ADDR, HWIO_QFPROM_CORR_CALIB_ROW15_LSB_RMSK)
#define HWIO_QFPROM_CORR_CALIB_ROW15_LSB_INM(m)      \
        in_dword_masked(HWIO_QFPROM_CORR_CALIB_ROW15_LSB_ADDR, m)
#define HWIO_QFPROM_CORR_CALIB_ROW15_LSB_VSENSE_FUSE1_S5_BMSK                                   0xe0000000
#define HWIO_QFPROM_CORR_CALIB_ROW15_LSB_VSENSE_FUSE1_S5_SHFT                                         0x1d
#define HWIO_QFPROM_CORR_CALIB_ROW15_LSB_VSENSE_FUSE1_S4_BMSK                                   0x1c000000
#define HWIO_QFPROM_CORR_CALIB_ROW15_LSB_VSENSE_FUSE1_S4_SHFT                                         0x1a
#define HWIO_QFPROM_CORR_CALIB_ROW15_LSB_VSENSE_FUSE1_S3_BMSK                                    0x3800000
#define HWIO_QFPROM_CORR_CALIB_ROW15_LSB_VSENSE_FUSE1_S3_SHFT                                         0x17
#define HWIO_QFPROM_CORR_CALIB_ROW15_LSB_VSENSE_FUSE1_S2_BMSK                                     0x700000
#define HWIO_QFPROM_CORR_CALIB_ROW15_LSB_VSENSE_FUSE1_S2_SHFT                                         0x14
#define HWIO_QFPROM_CORR_CALIB_ROW15_LSB_VSENSE_FUSE1_S1_BMSK                                      0xe0000
#define HWIO_QFPROM_CORR_CALIB_ROW15_LSB_VSENSE_FUSE1_S1_SHFT                                         0x11
#define HWIO_QFPROM_CORR_CALIB_ROW15_LSB_VSENSE_FUSE1_S0_BMSK                                      0x1c000
#define HWIO_QFPROM_CORR_CALIB_ROW15_LSB_VSENSE_FUSE1_S0_SHFT                                          0xe
#define HWIO_QFPROM_CORR_CALIB_ROW15_LSB_VSENSE_FUSE2_BMSK                                          0x3f00
#define HWIO_QFPROM_CORR_CALIB_ROW15_LSB_VSENSE_FUSE2_SHFT                                             0x8
#define HWIO_QFPROM_CORR_CALIB_ROW15_LSB_VSENSE_FUSE1_BMSK                                            0xff
#define HWIO_QFPROM_CORR_CALIB_ROW15_LSB_VSENSE_FUSE1_SHFT                                             0x0

#define HWIO_QFPROM_CORR_CALIB_ROW15_MSB_ADDR                                                   (SECURITY_CONTROL_CORE_REG_BASE      + 0x00004274)
#define HWIO_QFPROM_CORR_CALIB_ROW15_MSB_RMSK                                                   0xffffffff
#define HWIO_QFPROM_CORR_CALIB_ROW15_MSB_IN          \
        in_dword_masked(HWIO_QFPROM_CORR_CALIB_ROW15_MSB_ADDR, HWIO_QFPROM_CORR_CALIB_ROW15_MSB_RMSK)
#define HWIO_QFPROM_CORR_CALIB_ROW15_MSB_INM(m)      \
        in_dword_masked(HWIO_QFPROM_CORR_CALIB_ROW15_MSB_ADDR, m)
#define HWIO_QFPROM_CORR_CALIB_ROW15_MSB_VSENSE_FUSE2_S6_BMSK                                   0xc0000000
#define HWIO_QFPROM_CORR_CALIB_ROW15_MSB_VSENSE_FUSE2_S6_SHFT                                         0x1e
#define HWIO_QFPROM_CORR_CALIB_ROW15_MSB_VSENSE_FUSE2_S5_BMSK                                   0x30000000
#define HWIO_QFPROM_CORR_CALIB_ROW15_MSB_VSENSE_FUSE2_S5_SHFT                                         0x1c
#define HWIO_QFPROM_CORR_CALIB_ROW15_MSB_VSENSE_FUSE2_S4_BMSK                                    0xc000000
#define HWIO_QFPROM_CORR_CALIB_ROW15_MSB_VSENSE_FUSE2_S4_SHFT                                         0x1a
#define HWIO_QFPROM_CORR_CALIB_ROW15_MSB_VSENSE_FUSE2_S3_BMSK                                    0x3000000
#define HWIO_QFPROM_CORR_CALIB_ROW15_MSB_VSENSE_FUSE2_S3_SHFT                                         0x18
#define HWIO_QFPROM_CORR_CALIB_ROW15_MSB_VSENSE_FUSE2_S2_BMSK                                     0xc00000
#define HWIO_QFPROM_CORR_CALIB_ROW15_MSB_VSENSE_FUSE2_S2_SHFT                                         0x16
#define HWIO_QFPROM_CORR_CALIB_ROW15_MSB_VSENSE_FUSE2_S1_BMSK                                     0x300000
#define HWIO_QFPROM_CORR_CALIB_ROW15_MSB_VSENSE_FUSE2_S1_SHFT                                         0x14
#define HWIO_QFPROM_CORR_CALIB_ROW15_MSB_VSENSE_FUSE2_S0_BMSK                                      0xc0000
#define HWIO_QFPROM_CORR_CALIB_ROW15_MSB_VSENSE_FUSE2_S0_SHFT                                         0x12
#define HWIO_QFPROM_CORR_CALIB_ROW15_MSB_VSENSE_FUSE1_S11_BMSK                                     0x38000
#define HWIO_QFPROM_CORR_CALIB_ROW15_MSB_VSENSE_FUSE1_S11_SHFT                                         0xf
#define HWIO_QFPROM_CORR_CALIB_ROW15_MSB_VSENSE_FUSE1_S10_BMSK                                      0x7000
#define HWIO_QFPROM_CORR_CALIB_ROW15_MSB_VSENSE_FUSE1_S10_SHFT                                         0xc
#define HWIO_QFPROM_CORR_CALIB_ROW15_MSB_VSENSE_FUSE1_S9_BMSK                                        0xe00
#define HWIO_QFPROM_CORR_CALIB_ROW15_MSB_VSENSE_FUSE1_S9_SHFT                                          0x9
#define HWIO_QFPROM_CORR_CALIB_ROW15_MSB_VSENSE_FUSE1_S8_BMSK                                        0x1c0
#define HWIO_QFPROM_CORR_CALIB_ROW15_MSB_VSENSE_FUSE1_S8_SHFT                                          0x6
#define HWIO_QFPROM_CORR_CALIB_ROW15_MSB_VSENSE_FUSE1_S7_BMSK                                         0x38
#define HWIO_QFPROM_CORR_CALIB_ROW15_MSB_VSENSE_FUSE1_S7_SHFT                                          0x3
#define HWIO_QFPROM_CORR_CALIB_ROW15_MSB_VSENSE_FUSE1_S6_BMSK                                          0x7
#define HWIO_QFPROM_CORR_CALIB_ROW15_MSB_VSENSE_FUSE1_S6_SHFT                                          0x0

#define HWIO_QFPROM_CORR_CALIB_ROW16_LSB_ADDR                                                   (SECURITY_CONTROL_CORE_REG_BASE      + 0x00004278)
#define HWIO_QFPROM_CORR_CALIB_ROW16_LSB_RMSK                                                   0xffffffff
#define HWIO_QFPROM_CORR_CALIB_ROW16_LSB_IN          \
        in_dword_masked(HWIO_QFPROM_CORR_CALIB_ROW16_LSB_ADDR, HWIO_QFPROM_CORR_CALIB_ROW16_LSB_RMSK)
#define HWIO_QFPROM_CORR_CALIB_ROW16_LSB_INM(m)      \
        in_dword_masked(HWIO_QFPROM_CORR_CALIB_ROW16_LSB_ADDR, m)
#define HWIO_QFPROM_CORR_CALIB_ROW16_LSB_VSENSE_RESERVED_21_0_BMSK                              0xfffffc00
#define HWIO_QFPROM_CORR_CALIB_ROW16_LSB_VSENSE_RESERVED_21_0_SHFT                                     0xa
#define HWIO_QFPROM_CORR_CALIB_ROW16_LSB_VSENSE_FUSE2_BMSK                                           0x300
#define HWIO_QFPROM_CORR_CALIB_ROW16_LSB_VSENSE_FUSE2_SHFT                                             0x8
#define HWIO_QFPROM_CORR_CALIB_ROW16_LSB_VSENSE_FUSE2_S10_BMSK                                        0xc0
#define HWIO_QFPROM_CORR_CALIB_ROW16_LSB_VSENSE_FUSE2_S10_SHFT                                         0x6
#define HWIO_QFPROM_CORR_CALIB_ROW16_LSB_VSENSE_FUSE2_S9_BMSK                                         0x30
#define HWIO_QFPROM_CORR_CALIB_ROW16_LSB_VSENSE_FUSE2_S9_SHFT                                          0x4
#define HWIO_QFPROM_CORR_CALIB_ROW16_LSB_VSENSE_FUSE2_S8_BMSK                                          0xc
#define HWIO_QFPROM_CORR_CALIB_ROW16_LSB_VSENSE_FUSE2_S8_SHFT                                          0x2
#define HWIO_QFPROM_CORR_CALIB_ROW16_LSB_VSENSE_FUSE2_S7_BMSK                                          0x3
#define HWIO_QFPROM_CORR_CALIB_ROW16_LSB_VSENSE_FUSE2_S7_SHFT                                          0x0

#define HWIO_QFPROM_CORR_CALIB_ROW16_MSB_ADDR                                                   (SECURITY_CONTROL_CORE_REG_BASE      + 0x0000427c)
#define HWIO_QFPROM_CORR_CALIB_ROW16_MSB_RMSK                                                   0xffffffff
#define HWIO_QFPROM_CORR_CALIB_ROW16_MSB_IN          \
        in_dword_masked(HWIO_QFPROM_CORR_CALIB_ROW16_MSB_ADDR, HWIO_QFPROM_CORR_CALIB_ROW16_MSB_RMSK)
#define HWIO_QFPROM_CORR_CALIB_ROW16_MSB_INM(m)      \
        in_dword_masked(HWIO_QFPROM_CORR_CALIB_ROW16_MSB_ADDR, m)
#define HWIO_QFPROM_CORR_CALIB_ROW16_MSB_SPARE_77_63_BMSK                                       0x80000000
#define HWIO_QFPROM_CORR_CALIB_ROW16_MSB_SPARE_77_63_SHFT                                             0x1f
#define HWIO_QFPROM_CORR_CALIB_ROW16_MSB_SPARE_77_62_BMSK                                       0x40000000
#define HWIO_QFPROM_CORR_CALIB_ROW16_MSB_SPARE_77_62_SHFT                                             0x1e
#define HWIO_QFPROM_CORR_CALIB_ROW16_MSB_SPARE_77_61_BMSK                                       0x20000000
#define HWIO_QFPROM_CORR_CALIB_ROW16_MSB_SPARE_77_61_SHFT                                             0x1d
#define HWIO_QFPROM_CORR_CALIB_ROW16_MSB_SPARE_77_60_BMSK                                       0x10000000
#define HWIO_QFPROM_CORR_CALIB_ROW16_MSB_SPARE_77_60_SHFT                                             0x1c
#define HWIO_QFPROM_CORR_CALIB_ROW16_MSB_SPARE_77_59_BMSK                                        0x8000000
#define HWIO_QFPROM_CORR_CALIB_ROW16_MSB_SPARE_77_59_SHFT                                             0x1b
#define HWIO_QFPROM_CORR_CALIB_ROW16_MSB_SPARE_77_58_BMSK                                        0x4000000
#define HWIO_QFPROM_CORR_CALIB_ROW16_MSB_SPARE_77_58_SHFT                                             0x1a
#define HWIO_QFPROM_CORR_CALIB_ROW16_MSB_SPARE_77_57_BMSK                                        0x2000000
#define HWIO_QFPROM_CORR_CALIB_ROW16_MSB_SPARE_77_57_SHFT                                             0x19
#define HWIO_QFPROM_CORR_CALIB_ROW16_MSB_SPARE_77_56_BMSK                                        0x1000000
#define HWIO_QFPROM_CORR_CALIB_ROW16_MSB_SPARE_77_56_SHFT                                             0x18
#define HWIO_QFPROM_CORR_CALIB_ROW16_MSB_SPARE_77_55_BMSK                                         0x800000
#define HWIO_QFPROM_CORR_CALIB_ROW16_MSB_SPARE_77_55_SHFT                                             0x17
#define HWIO_QFPROM_CORR_CALIB_ROW16_MSB_SPARE_77_54_BMSK                                         0x400000
#define HWIO_QFPROM_CORR_CALIB_ROW16_MSB_SPARE_77_54_SHFT                                             0x16
#define HWIO_QFPROM_CORR_CALIB_ROW16_MSB_SPARE_77_53_BMSK                                         0x200000
#define HWIO_QFPROM_CORR_CALIB_ROW16_MSB_SPARE_77_53_SHFT                                             0x15
#define HWIO_QFPROM_CORR_CALIB_ROW16_MSB_SPARE_77_52_BMSK                                         0x100000
#define HWIO_QFPROM_CORR_CALIB_ROW16_MSB_SPARE_77_52_SHFT                                             0x14
#define HWIO_QFPROM_CORR_CALIB_ROW16_MSB_SPARE_77_51_BMSK                                          0x80000
#define HWIO_QFPROM_CORR_CALIB_ROW16_MSB_SPARE_77_51_SHFT                                             0x13
#define HWIO_QFPROM_CORR_CALIB_ROW16_MSB_SPARE_77_50_BMSK                                          0x40000
#define HWIO_QFPROM_CORR_CALIB_ROW16_MSB_SPARE_77_50_SHFT                                             0x12
#define HWIO_QFPROM_CORR_CALIB_ROW16_MSB_SPARE_77_49_BMSK                                          0x20000
#define HWIO_QFPROM_CORR_CALIB_ROW16_MSB_SPARE_77_49_SHFT                                             0x11
#define HWIO_QFPROM_CORR_CALIB_ROW16_MSB_SPARE_77_48_BMSK                                          0x10000
#define HWIO_QFPROM_CORR_CALIB_ROW16_MSB_SPARE_77_48_SHFT                                             0x10
#define HWIO_QFPROM_CORR_CALIB_ROW16_MSB_SPARE_77_47_BMSK                                           0x8000
#define HWIO_QFPROM_CORR_CALIB_ROW16_MSB_SPARE_77_47_SHFT                                              0xf
#define HWIO_QFPROM_CORR_CALIB_ROW16_MSB_SPARE_77_46_BMSK                                           0x4000
#define HWIO_QFPROM_CORR_CALIB_ROW16_MSB_SPARE_77_46_SHFT                                              0xe
#define HWIO_QFPROM_CORR_CALIB_ROW16_MSB_SPARE_77_45_BMSK                                           0x2000
#define HWIO_QFPROM_CORR_CALIB_ROW16_MSB_SPARE_77_45_SHFT                                              0xd
#define HWIO_QFPROM_CORR_CALIB_ROW16_MSB_SPARE_77_44_BMSK                                           0x1000
#define HWIO_QFPROM_CORR_CALIB_ROW16_MSB_SPARE_77_44_SHFT                                              0xc
#define HWIO_QFPROM_CORR_CALIB_ROW16_MSB_SPARE_77_43_BMSK                                            0x800
#define HWIO_QFPROM_CORR_CALIB_ROW16_MSB_SPARE_77_43_SHFT                                              0xb
#define HWIO_QFPROM_CORR_CALIB_ROW16_MSB_SPARE_77_42_BMSK                                            0x400
#define HWIO_QFPROM_CORR_CALIB_ROW16_MSB_SPARE_77_42_SHFT                                              0xa
#define HWIO_QFPROM_CORR_CALIB_ROW16_MSB_SPARE_77_41_BMSK                                            0x200
#define HWIO_QFPROM_CORR_CALIB_ROW16_MSB_SPARE_77_41_SHFT                                              0x9
#define HWIO_QFPROM_CORR_CALIB_ROW16_MSB_SPARE_77_40_BMSK                                            0x100
#define HWIO_QFPROM_CORR_CALIB_ROW16_MSB_SPARE_77_40_SHFT                                              0x8
#define HWIO_QFPROM_CORR_CALIB_ROW16_MSB_VSENSE_RESERVED_29_22_BMSK                                   0xff
#define HWIO_QFPROM_CORR_CALIB_ROW16_MSB_VSENSE_RESERVED_29_22_SHFT                                    0x0

#define HWIO_QFPROM_CORR_CALIB_ROW17_LSB_ADDR                                                   (SECURITY_CONTROL_CORE_REG_BASE      + 0x00004280)
#define HWIO_QFPROM_CORR_CALIB_ROW17_LSB_RMSK                                                   0xffffffff
#define HWIO_QFPROM_CORR_CALIB_ROW17_LSB_IN          \
        in_dword_masked(HWIO_QFPROM_CORR_CALIB_ROW17_LSB_ADDR, HWIO_QFPROM_CORR_CALIB_ROW17_LSB_RMSK)
#define HWIO_QFPROM_CORR_CALIB_ROW17_LSB_INM(m)      \
        in_dword_masked(HWIO_QFPROM_CORR_CALIB_ROW17_LSB_ADDR, m)
#define HWIO_QFPROM_CORR_CALIB_ROW17_LSB_CPR4_TARG_VOLT_SUT_BMSK                                0xf8000000
#define HWIO_QFPROM_CORR_CALIB_ROW17_LSB_CPR4_TARG_VOLT_SUT_SHFT                                      0x1b
#define HWIO_QFPROM_CORR_CALIB_ROW17_LSB_CPR2_TARG_VOLT_OFFSET_SUT_BMSK                          0x7800000
#define HWIO_QFPROM_CORR_CALIB_ROW17_LSB_CPR2_TARG_VOLT_OFFSET_SUT_SHFT                               0x17
#define HWIO_QFPROM_CORR_CALIB_ROW17_LSB_CPR2_TARG_VOLT_SUT_BMSK                                  0x7c0000
#define HWIO_QFPROM_CORR_CALIB_ROW17_LSB_CPR2_TARG_VOLT_SUT_SHFT                                      0x12
#define HWIO_QFPROM_CORR_CALIB_ROW17_LSB_CPR1_TARG_VOLT_OFFSET_SUT_BMSK                            0x3c000
#define HWIO_QFPROM_CORR_CALIB_ROW17_LSB_CPR1_TARG_VOLT_OFFSET_SUT_SHFT                                0xe
#define HWIO_QFPROM_CORR_CALIB_ROW17_LSB_CPR1_TARG_VOLT_SUT_BMSK                                    0x3e00
#define HWIO_QFPROM_CORR_CALIB_ROW17_LSB_CPR1_TARG_VOLT_SUT_SHFT                                       0x9
#define HWIO_QFPROM_CORR_CALIB_ROW17_LSB_CPR0_TARG_VOLT_OFFSET_SUT_BMSK                              0x1e0
#define HWIO_QFPROM_CORR_CALIB_ROW17_LSB_CPR0_TARG_VOLT_OFFSET_SUT_SHFT                                0x5
#define HWIO_QFPROM_CORR_CALIB_ROW17_LSB_CPR0_TARG_VOLT_SUT_BMSK                                      0x1f
#define HWIO_QFPROM_CORR_CALIB_ROW17_LSB_CPR0_TARG_VOLT_SUT_SHFT                                       0x0

#define HWIO_QFPROM_CORR_CALIB_ROW17_MSB_ADDR                                                   (SECURITY_CONTROL_CORE_REG_BASE      + 0x00004284)
#define HWIO_QFPROM_CORR_CALIB_ROW17_MSB_RMSK                                                   0xffffffff
#define HWIO_QFPROM_CORR_CALIB_ROW17_MSB_IN          \
        in_dword_masked(HWIO_QFPROM_CORR_CALIB_ROW17_MSB_ADDR, HWIO_QFPROM_CORR_CALIB_ROW17_MSB_RMSK)
#define HWIO_QFPROM_CORR_CALIB_ROW17_MSB_INM(m)      \
        in_dword_masked(HWIO_QFPROM_CORR_CALIB_ROW17_MSB_ADDR, m)
#define HWIO_QFPROM_CORR_CALIB_ROW17_MSB_CPR13_NOMINAL_ROSEL_2_0_BMSK                           0xe0000000
#define HWIO_QFPROM_CORR_CALIB_ROW17_MSB_CPR13_NOMINAL_ROSEL_2_0_SHFT                                 0x1d
#define HWIO_QFPROM_CORR_CALIB_ROW17_MSB_CPR13_TURBO_ROSEL_BMSK                                 0x1e000000
#define HWIO_QFPROM_CORR_CALIB_ROW17_MSB_CPR13_TURBO_ROSEL_SHFT                                       0x19
#define HWIO_QFPROM_CORR_CALIB_ROW17_MSB_CPR13_STURBO_ROSEL_BMSK                                 0x1e00000
#define HWIO_QFPROM_CORR_CALIB_ROW17_MSB_CPR13_STURBO_ROSEL_SHFT                                      0x15
#define HWIO_QFPROM_CORR_CALIB_ROW17_MSB_CPR8_LDO_TARG_VOLT_NOM_BMSK                              0x1e0000
#define HWIO_QFPROM_CORR_CALIB_ROW17_MSB_CPR8_LDO_TARG_VOLT_NOM_SHFT                                  0x11
#define HWIO_QFPROM_CORR_CALIB_ROW17_MSB_CPR7_LDO_TARG_VOLT_SVS3_BMSK                              0x1e000
#define HWIO_QFPROM_CORR_CALIB_ROW17_MSB_CPR7_LDO_TARG_VOLT_SVS3_SHFT                                  0xd
#define HWIO_QFPROM_CORR_CALIB_ROW17_MSB_CPR4_TARG_VOLT_OFFSET_SVSP_BMSK                            0x1e00
#define HWIO_QFPROM_CORR_CALIB_ROW17_MSB_CPR4_TARG_VOLT_OFFSET_SVSP_SHFT                               0x9
#define HWIO_QFPROM_CORR_CALIB_ROW17_MSB_CPR4_TARG_VOLT_SVSP_BMSK                                    0x1f0
#define HWIO_QFPROM_CORR_CALIB_ROW17_MSB_CPR4_TARG_VOLT_SVSP_SHFT                                      0x4
#define HWIO_QFPROM_CORR_CALIB_ROW17_MSB_CPR4_TARG_VOLT_OFFSET_SUT_BMSK                                0xf
#define HWIO_QFPROM_CORR_CALIB_ROW17_MSB_CPR4_TARG_VOLT_OFFSET_SUT_SHFT                                0x0

#define HWIO_QFPROM_CORR_CALIB_ROW18_LSB_ADDR                                                   (SECURITY_CONTROL_CORE_REG_BASE      + 0x00004288)
#define HWIO_QFPROM_CORR_CALIB_ROW18_LSB_RMSK                                                   0xffffffff
#define HWIO_QFPROM_CORR_CALIB_ROW18_LSB_IN          \
        in_dword_masked(HWIO_QFPROM_CORR_CALIB_ROW18_LSB_ADDR, HWIO_QFPROM_CORR_CALIB_ROW18_LSB_RMSK)
#define HWIO_QFPROM_CORR_CALIB_ROW18_LSB_INM(m)      \
        in_dword_masked(HWIO_QFPROM_CORR_CALIB_ROW18_LSB_ADDR, m)
#define HWIO_QFPROM_CORR_CALIB_ROW18_LSB_SPARE_R81_B31_BMSK                                     0x80000000
#define HWIO_QFPROM_CORR_CALIB_ROW18_LSB_SPARE_R81_B31_SHFT                                           0x1f
#define HWIO_QFPROM_CORR_CALIB_ROW18_LSB_SPARE_R81_B30_BMSK                                     0x40000000
#define HWIO_QFPROM_CORR_CALIB_ROW18_LSB_SPARE_R81_B30_SHFT                                           0x1e
#define HWIO_QFPROM_CORR_CALIB_ROW18_LSB_CPR13_SUT_QUOT_VMIN_BMSK                               0x3ffc0000
#define HWIO_QFPROM_CORR_CALIB_ROW18_LSB_CPR13_SUT_QUOT_VMIN_SHFT                                     0x12
#define HWIO_QFPROM_CORR_CALIB_ROW18_LSB_CPR13_QUOT_OFFSET_TUR_BMSK                                0x20000
#define HWIO_QFPROM_CORR_CALIB_ROW18_LSB_CPR13_QUOT_OFFSET_TUR_SHFT                                   0x11
#define HWIO_QFPROM_CORR_CALIB_ROW18_LSB_CPR13_QUOT_OFFSET_SUT_BMSK                                0x1fe00
#define HWIO_QFPROM_CORR_CALIB_ROW18_LSB_CPR13_QUOT_OFFSET_SUT_SHFT                                    0x9
#define HWIO_QFPROM_CORR_CALIB_ROW18_LSB_CPR13_SVS2_ROSEL_BMSK                                       0x1e0
#define HWIO_QFPROM_CORR_CALIB_ROW18_LSB_CPR13_SVS2_ROSEL_SHFT                                         0x5
#define HWIO_QFPROM_CORR_CALIB_ROW18_LSB_CPR13_SVS_ROSEL_BMSK                                         0x1e
#define HWIO_QFPROM_CORR_CALIB_ROW18_LSB_CPR13_SVS_ROSEL_SHFT                                          0x1
#define HWIO_QFPROM_CORR_CALIB_ROW18_LSB_CPR13_NOMINAL_ROSEL_3_BMSK                                    0x1
#define HWIO_QFPROM_CORR_CALIB_ROW18_LSB_CPR13_NOMINAL_ROSEL_3_SHFT                                    0x0

#define HWIO_QFPROM_CORR_CALIB_ROW18_MSB_ADDR                                                   (SECURITY_CONTROL_CORE_REG_BASE      + 0x0000428c)
#define HWIO_QFPROM_CORR_CALIB_ROW18_MSB_RMSK                                                   0xffffffff
#define HWIO_QFPROM_CORR_CALIB_ROW18_MSB_IN          \
        in_dword_masked(HWIO_QFPROM_CORR_CALIB_ROW18_MSB_ADDR, HWIO_QFPROM_CORR_CALIB_ROW18_MSB_RMSK)
#define HWIO_QFPROM_CORR_CALIB_ROW18_MSB_INM(m)      \
        in_dword_masked(HWIO_QFPROM_CORR_CALIB_ROW18_MSB_ADDR, m)
#define HWIO_QFPROM_CORR_CALIB_ROW18_MSB_CPR_RESERVED_BMSK                                      0xffff8000
#define HWIO_QFPROM_CORR_CALIB_ROW18_MSB_CPR_RESERVED_SHFT                                             0xf
#define HWIO_QFPROM_CORR_CALIB_ROW18_MSB_CPR_LPI_MX_MODE_DISABLE_BMSK                               0x7000
#define HWIO_QFPROM_CORR_CALIB_ROW18_MSB_CPR_LPI_MX_MODE_DISABLE_SHFT                                  0xc
#define HWIO_QFPROM_CORR_CALIB_ROW18_MSB_CPR_MM_MODE_DISABLE_BMSK                                    0xe00
#define HWIO_QFPROM_CORR_CALIB_ROW18_MSB_CPR_MM_MODE_DISABLE_SHFT                                      0x9
#define HWIO_QFPROM_CORR_CALIB_ROW18_MSB_CPR_MX_MODE_DISABLE_BMSK                                    0x1c0
#define HWIO_QFPROM_CORR_CALIB_ROW18_MSB_CPR_MX_MODE_DISABLE_SHFT                                      0x6
#define HWIO_QFPROM_CORR_CALIB_ROW18_MSB_CPR13_TARG_VOLT_SVS2_BMSK                                    0x3f
#define HWIO_QFPROM_CORR_CALIB_ROW18_MSB_CPR13_TARG_VOLT_SVS2_SHFT                                     0x0

#define HWIO_QFPROM_CORR_CALIB_ROW19_LSB_ADDR                                                   (SECURITY_CONTROL_CORE_REG_BASE      + 0x00004290)
#define HWIO_QFPROM_CORR_CALIB_ROW19_LSB_RMSK                                                   0xffffffff
#define HWIO_QFPROM_CORR_CALIB_ROW19_LSB_IN          \
        in_dword_masked(HWIO_QFPROM_CORR_CALIB_ROW19_LSB_ADDR, HWIO_QFPROM_CORR_CALIB_ROW19_LSB_RMSK)
#define HWIO_QFPROM_CORR_CALIB_ROW19_LSB_INM(m)      \
        in_dword_masked(HWIO_QFPROM_CORR_CALIB_ROW19_LSB_ADDR, m)
#define HWIO_QFPROM_CORR_CALIB_ROW19_LSB_TSENS22_OFFSET_1_0_BMSK                                0xc0000000
#define HWIO_QFPROM_CORR_CALIB_ROW19_LSB_TSENS22_OFFSET_1_0_SHFT                                      0x1e
#define HWIO_QFPROM_CORR_CALIB_ROW19_LSB_TSENS21_OFFSET_BMSK                                    0x3e000000
#define HWIO_QFPROM_CORR_CALIB_ROW19_LSB_TSENS21_OFFSET_SHFT                                          0x19
#define HWIO_QFPROM_CORR_CALIB_ROW19_LSB_TSENS20_OFFSET_BMSK                                     0x1f00000
#define HWIO_QFPROM_CORR_CALIB_ROW19_LSB_TSENS20_OFFSET_SHFT                                          0x14
#define HWIO_QFPROM_CORR_CALIB_ROW19_LSB_TSENS19_OFFSET_BMSK                                       0xf8000
#define HWIO_QFPROM_CORR_CALIB_ROW19_LSB_TSENS19_OFFSET_SHFT                                           0xf
#define HWIO_QFPROM_CORR_CALIB_ROW19_LSB_TSENS18_OFFSET_BMSK                                        0x7c00
#define HWIO_QFPROM_CORR_CALIB_ROW19_LSB_TSENS18_OFFSET_SHFT                                           0xa
#define HWIO_QFPROM_CORR_CALIB_ROW19_LSB_TSENS17_OFFSET_BMSK                                         0x3e0
#define HWIO_QFPROM_CORR_CALIB_ROW19_LSB_TSENS17_OFFSET_SHFT                                           0x5
#define HWIO_QFPROM_CORR_CALIB_ROW19_LSB_TSENS16_OFFSET_BMSK                                          0x1f
#define HWIO_QFPROM_CORR_CALIB_ROW19_LSB_TSENS16_OFFSET_SHFT                                           0x0

#define HWIO_QFPROM_CORR_CALIB_ROW19_MSB_ADDR                                                   (SECURITY_CONTROL_CORE_REG_BASE      + 0x00004294)
#define HWIO_QFPROM_CORR_CALIB_ROW19_MSB_RMSK                                                   0xffffffff
#define HWIO_QFPROM_CORR_CALIB_ROW19_MSB_IN          \
        in_dword_masked(HWIO_QFPROM_CORR_CALIB_ROW19_MSB_ADDR, HWIO_QFPROM_CORR_CALIB_ROW19_MSB_RMSK)
#define HWIO_QFPROM_CORR_CALIB_ROW19_MSB_INM(m)      \
        in_dword_masked(HWIO_QFPROM_CORR_CALIB_ROW19_MSB_ADDR, m)
#define HWIO_QFPROM_CORR_CALIB_ROW19_MSB_TSENS28_OFFSET_3_0_BMSK                                0xf0000000
#define HWIO_QFPROM_CORR_CALIB_ROW19_MSB_TSENS28_OFFSET_3_0_SHFT                                      0x1c
#define HWIO_QFPROM_CORR_CALIB_ROW19_MSB_TSENS27_OFFSET_BMSK                                     0xf800000
#define HWIO_QFPROM_CORR_CALIB_ROW19_MSB_TSENS27_OFFSET_SHFT                                          0x17
#define HWIO_QFPROM_CORR_CALIB_ROW19_MSB_TSENS26_OFFSET_BMSK                                      0x7c0000
#define HWIO_QFPROM_CORR_CALIB_ROW19_MSB_TSENS26_OFFSET_SHFT                                          0x12
#define HWIO_QFPROM_CORR_CALIB_ROW19_MSB_TSENS25_OFFSET_BMSK                                       0x3e000
#define HWIO_QFPROM_CORR_CALIB_ROW19_MSB_TSENS25_OFFSET_SHFT                                           0xd
#define HWIO_QFPROM_CORR_CALIB_ROW19_MSB_TSENS24_OFFSET_BMSK                                        0x1f00
#define HWIO_QFPROM_CORR_CALIB_ROW19_MSB_TSENS24_OFFSET_SHFT                                           0x8
#define HWIO_QFPROM_CORR_CALIB_ROW19_MSB_TSENS23_OFFSET_BMSK                                          0xf8
#define HWIO_QFPROM_CORR_CALIB_ROW19_MSB_TSENS23_OFFSET_SHFT                                           0x3
#define HWIO_QFPROM_CORR_CALIB_ROW19_MSB_TSENS22_OFFSET_4_2_BMSK                                       0x7
#define HWIO_QFPROM_CORR_CALIB_ROW19_MSB_TSENS22_OFFSET_4_2_SHFT                                       0x0

#define HWIO_QFPROM_CORR_CALIB_ROW20_LSB_ADDR                                                   (SECURITY_CONTROL_CORE_REG_BASE      + 0x00004298)
#define HWIO_QFPROM_CORR_CALIB_ROW20_LSB_RMSK                                                   0xffffffff
#define HWIO_QFPROM_CORR_CALIB_ROW20_LSB_IN          \
        in_dword_masked(HWIO_QFPROM_CORR_CALIB_ROW20_LSB_ADDR, HWIO_QFPROM_CORR_CALIB_ROW20_LSB_RMSK)
#define HWIO_QFPROM_CORR_CALIB_ROW20_LSB_INM(m)      \
        in_dword_masked(HWIO_QFPROM_CORR_CALIB_ROW20_LSB_ADDR, m)
#define HWIO_QFPROM_CORR_CALIB_ROW20_LSB_VSENS_RAIL1_OFFSET2_1_0_BMSK                           0xc0000000
#define HWIO_QFPROM_CORR_CALIB_ROW20_LSB_VSENS_RAIL1_OFFSET2_1_0_SHFT                                 0x1e
#define HWIO_QFPROM_CORR_CALIB_ROW20_LSB_VSENS_RAIL1_OFFSET1_BMSK                               0x3f000000
#define HWIO_QFPROM_CORR_CALIB_ROW20_LSB_VSENS_RAIL1_OFFSET1_SHFT                                     0x18
#define HWIO_QFPROM_CORR_CALIB_ROW20_LSB_VSENS_RAIL1_BASE_BMSK                                    0xff0000
#define HWIO_QFPROM_CORR_CALIB_ROW20_LSB_VSENS_RAIL1_BASE_SHFT                                        0x10
#define HWIO_QFPROM_CORR_CALIB_ROW20_LSB_TSENS31_OFFSET_BMSK                                        0xf800
#define HWIO_QFPROM_CORR_CALIB_ROW20_LSB_TSENS31_OFFSET_SHFT                                           0xb
#define HWIO_QFPROM_CORR_CALIB_ROW20_LSB_TSENS30_OFFSET_BMSK                                         0x7c0
#define HWIO_QFPROM_CORR_CALIB_ROW20_LSB_TSENS30_OFFSET_SHFT                                           0x6
#define HWIO_QFPROM_CORR_CALIB_ROW20_LSB_TSENS29_OFFSET_BMSK                                          0x3e
#define HWIO_QFPROM_CORR_CALIB_ROW20_LSB_TSENS29_OFFSET_SHFT                                           0x1
#define HWIO_QFPROM_CORR_CALIB_ROW20_LSB_TSENS28_OFFSET_4_BMSK                                         0x1
#define HWIO_QFPROM_CORR_CALIB_ROW20_LSB_TSENS28_OFFSET_4_SHFT                                         0x0

#define HWIO_QFPROM_CORR_CALIB_ROW20_MSB_ADDR                                                   (SECURITY_CONTROL_CORE_REG_BASE      + 0x0000429c)
#define HWIO_QFPROM_CORR_CALIB_ROW20_MSB_RMSK                                                   0xffffffff
#define HWIO_QFPROM_CORR_CALIB_ROW20_MSB_IN          \
        in_dword_masked(HWIO_QFPROM_CORR_CALIB_ROW20_MSB_ADDR, HWIO_QFPROM_CORR_CALIB_ROW20_MSB_RMSK)
#define HWIO_QFPROM_CORR_CALIB_ROW20_MSB_INM(m)      \
        in_dword_masked(HWIO_QFPROM_CORR_CALIB_ROW20_MSB_ADDR, m)
#define HWIO_QFPROM_CORR_CALIB_ROW20_MSB_SPARE_R83_B63_BMSK                                     0x80000000
#define HWIO_QFPROM_CORR_CALIB_ROW20_MSB_SPARE_R83_B63_SHFT                                           0x1f
#define HWIO_QFPROM_CORR_CALIB_ROW20_MSB_SPARE_R83_B62_BMSK                                     0x40000000
#define HWIO_QFPROM_CORR_CALIB_ROW20_MSB_SPARE_R83_B62_SHFT                                           0x1e
#define HWIO_QFPROM_CORR_CALIB_ROW20_MSB_SPARE_R83_B61_BMSK                                     0x20000000
#define HWIO_QFPROM_CORR_CALIB_ROW20_MSB_SPARE_R83_B61_SHFT                                           0x1d
#define HWIO_QFPROM_CORR_CALIB_ROW20_MSB_SPARE_R83_B60_BMSK                                     0x10000000
#define HWIO_QFPROM_CORR_CALIB_ROW20_MSB_SPARE_R83_B60_SHFT                                           0x1c
#define HWIO_QFPROM_CORR_CALIB_ROW20_MSB_SPARE_R83_B59_BMSK                                      0x8000000
#define HWIO_QFPROM_CORR_CALIB_ROW20_MSB_SPARE_R83_B59_SHFT                                           0x1b
#define HWIO_QFPROM_CORR_CALIB_ROW20_MSB_SPARE_R83_B58_BMSK                                      0x4000000
#define HWIO_QFPROM_CORR_CALIB_ROW20_MSB_SPARE_R83_B58_SHFT                                           0x1a
#define HWIO_QFPROM_CORR_CALIB_ROW20_MSB_SPARE_R83_B57_BMSK                                      0x2000000
#define HWIO_QFPROM_CORR_CALIB_ROW20_MSB_SPARE_R83_B57_SHFT                                           0x19
#define HWIO_QFPROM_CORR_CALIB_ROW20_MSB_SPARE_R83_B56_BMSK                                      0x1000000
#define HWIO_QFPROM_CORR_CALIB_ROW20_MSB_SPARE_R83_B56_SHFT                                           0x18
#define HWIO_QFPROM_CORR_CALIB_ROW20_MSB_VSENS_RAIL2_OFFSET2_BMSK                                 0xfc0000
#define HWIO_QFPROM_CORR_CALIB_ROW20_MSB_VSENS_RAIL2_OFFSET2_SHFT                                     0x12
#define HWIO_QFPROM_CORR_CALIB_ROW20_MSB_VSENS_RAIL2_OFFSET1_BMSK                                  0x3f000
#define HWIO_QFPROM_CORR_CALIB_ROW20_MSB_VSENS_RAIL2_OFFSET1_SHFT                                      0xc
#define HWIO_QFPROM_CORR_CALIB_ROW20_MSB_VSENS_RAIL2_BASE_BMSK                                       0xff0
#define HWIO_QFPROM_CORR_CALIB_ROW20_MSB_VSENS_RAIL2_BASE_SHFT                                         0x4
#define HWIO_QFPROM_CORR_CALIB_ROW20_MSB_VSENS_RAIL1_OFFSET2_5_2_BMSK                                  0xf
#define HWIO_QFPROM_CORR_CALIB_ROW20_MSB_VSENS_RAIL1_OFFSET2_5_2_SHFT                                  0x0

#define HWIO_QFPROM_CORR_CALIB_ROW21_LSB_ADDR                                                   (SECURITY_CONTROL_CORE_REG_BASE      + 0x000042a0)
#define HWIO_QFPROM_CORR_CALIB_ROW21_LSB_RMSK                                                   0xffffffff
#define HWIO_QFPROM_CORR_CALIB_ROW21_LSB_IN          \
        in_dword_masked(HWIO_QFPROM_CORR_CALIB_ROW21_LSB_ADDR, HWIO_QFPROM_CORR_CALIB_ROW21_LSB_RMSK)
#define HWIO_QFPROM_CORR_CALIB_ROW21_LSB_INM(m)      \
        in_dword_masked(HWIO_QFPROM_CORR_CALIB_ROW21_LSB_ADDR, m)
#define HWIO_QFPROM_CORR_CALIB_ROW21_LSB_SPARE_R84_B31_BMSK                                     0x80000000
#define HWIO_QFPROM_CORR_CALIB_ROW21_LSB_SPARE_R84_B31_SHFT                                           0x1f
#define HWIO_QFPROM_CORR_CALIB_ROW21_LSB_SPARE_R84_B30_BMSK                                     0x40000000
#define HWIO_QFPROM_CORR_CALIB_ROW21_LSB_SPARE_R84_B30_SHFT                                           0x1e
#define HWIO_QFPROM_CORR_CALIB_ROW21_LSB_SPARE_R84_B29_BMSK                                     0x20000000
#define HWIO_QFPROM_CORR_CALIB_ROW21_LSB_SPARE_R84_B29_SHFT                                           0x1d
#define HWIO_QFPROM_CORR_CALIB_ROW21_LSB_SPARE_R84_B28_BMSK                                     0x10000000
#define HWIO_QFPROM_CORR_CALIB_ROW21_LSB_SPARE_R84_B28_SHFT                                           0x1c
#define HWIO_QFPROM_CORR_CALIB_ROW21_LSB_SPARE_R84_B27_BMSK                                      0x8000000
#define HWIO_QFPROM_CORR_CALIB_ROW21_LSB_SPARE_R84_B27_SHFT                                           0x1b
#define HWIO_QFPROM_CORR_CALIB_ROW21_LSB_SPARE_R84_B26_BMSK                                      0x4000000
#define HWIO_QFPROM_CORR_CALIB_ROW21_LSB_SPARE_R84_B26_SHFT                                           0x1a
#define HWIO_QFPROM_CORR_CALIB_ROW21_LSB_LDO_ZURIL_REF_TC_CTRL_BMSK                              0x3c00000
#define HWIO_QFPROM_CORR_CALIB_ROW21_LSB_LDO_ZURIL_REF_TC_CTRL_SHFT                                   0x16
#define HWIO_QFPROM_CORR_CALIB_ROW21_LSB_LDO_ZURIL_RCO_CALIB_BMSK                                 0x3fff80
#define HWIO_QFPROM_CORR_CALIB_ROW21_LSB_LDO_ZURIL_RCO_CALIB_SHFT                                      0x7
#define HWIO_QFPROM_CORR_CALIB_ROW21_LSB_LDO_ZURIL_LP_IDLE_VOLTAGE_CONFIG_BMSK                        0x40
#define HWIO_QFPROM_CORR_CALIB_ROW21_LSB_LDO_ZURIL_LP_IDLE_VOLTAGE_CONFIG_SHFT                         0x6
#define HWIO_QFPROM_CORR_CALIB_ROW21_LSB_LDO_ZURIL_CMP_TRIM_CFG_BMSK                                  0x3f
#define HWIO_QFPROM_CORR_CALIB_ROW21_LSB_LDO_ZURIL_CMP_TRIM_CFG_SHFT                                   0x0

#define HWIO_QFPROM_CORR_CALIB_ROW21_MSB_ADDR                                                   (SECURITY_CONTROL_CORE_REG_BASE      + 0x000042a4)
#define HWIO_QFPROM_CORR_CALIB_ROW21_MSB_RMSK                                                   0xffffffff
#define HWIO_QFPROM_CORR_CALIB_ROW21_MSB_IN          \
        in_dword_masked(HWIO_QFPROM_CORR_CALIB_ROW21_MSB_ADDR, HWIO_QFPROM_CORR_CALIB_ROW21_MSB_RMSK)
#define HWIO_QFPROM_CORR_CALIB_ROW21_MSB_INM(m)      \
        in_dword_masked(HWIO_QFPROM_CORR_CALIB_ROW21_MSB_ADDR, m)
#define HWIO_QFPROM_CORR_CALIB_ROW21_MSB_SPARE_R84_B63_BMSK                                     0x80000000
#define HWIO_QFPROM_CORR_CALIB_ROW21_MSB_SPARE_R84_B63_SHFT                                           0x1f
#define HWIO_QFPROM_CORR_CALIB_ROW21_MSB_SPARE_R84_B62_BMSK                                     0x40000000
#define HWIO_QFPROM_CORR_CALIB_ROW21_MSB_SPARE_R84_B62_SHFT                                           0x1e
#define HWIO_QFPROM_CORR_CALIB_ROW21_MSB_SPARE_R84_B61_BMSK                                     0x20000000
#define HWIO_QFPROM_CORR_CALIB_ROW21_MSB_SPARE_R84_B61_SHFT                                           0x1d
#define HWIO_QFPROM_CORR_CALIB_ROW21_MSB_SPARE_R84_B60_BMSK                                     0x10000000
#define HWIO_QFPROM_CORR_CALIB_ROW21_MSB_SPARE_R84_B60_SHFT                                           0x1c
#define HWIO_QFPROM_CORR_CALIB_ROW21_MSB_SPARE_R84_B59_BMSK                                      0x8000000
#define HWIO_QFPROM_CORR_CALIB_ROW21_MSB_SPARE_R84_B59_SHFT                                           0x1b
#define HWIO_QFPROM_CORR_CALIB_ROW21_MSB_SPARE_R84_B58_BMSK                                      0x4000000
#define HWIO_QFPROM_CORR_CALIB_ROW21_MSB_SPARE_R84_B58_SHFT                                           0x1a
#define HWIO_QFPROM_CORR_CALIB_ROW21_MSB_SPARE_R84_B57_BMSK                                      0x2000000
#define HWIO_QFPROM_CORR_CALIB_ROW21_MSB_SPARE_R84_B57_SHFT                                           0x19
#define HWIO_QFPROM_CORR_CALIB_ROW21_MSB_SPARE_R84_B56_BMSK                                      0x1000000
#define HWIO_QFPROM_CORR_CALIB_ROW21_MSB_SPARE_R84_B56_SHFT                                           0x18
#define HWIO_QFPROM_CORR_CALIB_ROW21_MSB_SPARE_R84_B55_BMSK                                       0x800000
#define HWIO_QFPROM_CORR_CALIB_ROW21_MSB_SPARE_R84_B55_SHFT                                           0x17
#define HWIO_QFPROM_CORR_CALIB_ROW21_MSB_SPARE_R84_B54_BMSK                                       0x400000
#define HWIO_QFPROM_CORR_CALIB_ROW21_MSB_SPARE_R84_B54_SHFT                                           0x16
#define HWIO_QFPROM_CORR_CALIB_ROW21_MSB_SPARE_R84_B53_BMSK                                       0x200000
#define HWIO_QFPROM_CORR_CALIB_ROW21_MSB_SPARE_R84_B53_SHFT                                           0x15
#define HWIO_QFPROM_CORR_CALIB_ROW21_MSB_SPARE_R84_B52_BMSK                                       0x100000
#define HWIO_QFPROM_CORR_CALIB_ROW21_MSB_SPARE_R84_B52_SHFT                                           0x14
#define HWIO_QFPROM_CORR_CALIB_ROW21_MSB_SPARE_R84_B51_BMSK                                        0x80000
#define HWIO_QFPROM_CORR_CALIB_ROW21_MSB_SPARE_R84_B51_SHFT                                           0x13
#define HWIO_QFPROM_CORR_CALIB_ROW21_MSB_SPARE_R84_B50_BMSK                                        0x40000
#define HWIO_QFPROM_CORR_CALIB_ROW21_MSB_SPARE_R84_B50_SHFT                                           0x12
#define HWIO_QFPROM_CORR_CALIB_ROW21_MSB_SPARE_R84_B49_BMSK                                        0x20000
#define HWIO_QFPROM_CORR_CALIB_ROW21_MSB_SPARE_R84_B49_SHFT                                           0x11
#define HWIO_QFPROM_CORR_CALIB_ROW21_MSB_SPARE_R84_B48_BMSK                                        0x10000
#define HWIO_QFPROM_CORR_CALIB_ROW21_MSB_SPARE_R84_B48_SHFT                                           0x10
#define HWIO_QFPROM_CORR_CALIB_ROW21_MSB_SPARE_R84_B47_BMSK                                         0x8000
#define HWIO_QFPROM_CORR_CALIB_ROW21_MSB_SPARE_R84_B47_SHFT                                            0xf
#define HWIO_QFPROM_CORR_CALIB_ROW21_MSB_SPARE_R84_B46_BMSK                                         0x4000
#define HWIO_QFPROM_CORR_CALIB_ROW21_MSB_SPARE_R84_B46_SHFT                                            0xe
#define HWIO_QFPROM_CORR_CALIB_ROW21_MSB_SPARE_R84_B45_BMSK                                         0x2000
#define HWIO_QFPROM_CORR_CALIB_ROW21_MSB_SPARE_R84_B45_SHFT                                            0xd
#define HWIO_QFPROM_CORR_CALIB_ROW21_MSB_SPARE_R84_B44_BMSK                                         0x1000
#define HWIO_QFPROM_CORR_CALIB_ROW21_MSB_SPARE_R84_B44_SHFT                                            0xc
#define HWIO_QFPROM_CORR_CALIB_ROW21_MSB_SPARE_R84_B43_BMSK                                          0x800
#define HWIO_QFPROM_CORR_CALIB_ROW21_MSB_SPARE_R84_B43_SHFT                                            0xb
#define HWIO_QFPROM_CORR_CALIB_ROW21_MSB_SPARE_R84_B42_BMSK                                          0x400
#define HWIO_QFPROM_CORR_CALIB_ROW21_MSB_SPARE_R84_B42_SHFT                                            0xa
#define HWIO_QFPROM_CORR_CALIB_ROW21_MSB_SPARE_R84_B41_BMSK                                          0x200
#define HWIO_QFPROM_CORR_CALIB_ROW21_MSB_SPARE_R84_B41_SHFT                                            0x9
#define HWIO_QFPROM_CORR_CALIB_ROW21_MSB_SPARE_R84_B40_BMSK                                          0x100
#define HWIO_QFPROM_CORR_CALIB_ROW21_MSB_SPARE_R84_B40_SHFT                                            0x8
#define HWIO_QFPROM_CORR_CALIB_ROW21_MSB_SPARE_R84_B39_BMSK                                           0x80
#define HWIO_QFPROM_CORR_CALIB_ROW21_MSB_SPARE_R84_B39_SHFT                                            0x7
#define HWIO_QFPROM_CORR_CALIB_ROW21_MSB_SPARE_R84_B38_BMSK                                           0x40
#define HWIO_QFPROM_CORR_CALIB_ROW21_MSB_SPARE_R84_B38_SHFT                                            0x6
#define HWIO_QFPROM_CORR_CALIB_ROW21_MSB_SPARE_R84_B37_BMSK                                           0x20
#define HWIO_QFPROM_CORR_CALIB_ROW21_MSB_SPARE_R84_B37_SHFT                                            0x5
#define HWIO_QFPROM_CORR_CALIB_ROW21_MSB_SPARE_R84_B36_BMSK                                           0x10
#define HWIO_QFPROM_CORR_CALIB_ROW21_MSB_SPARE_R84_B36_SHFT                                            0x4
#define HWIO_QFPROM_CORR_CALIB_ROW21_MSB_SPARE_R84_B35_BMSK                                            0x8
#define HWIO_QFPROM_CORR_CALIB_ROW21_MSB_SPARE_R84_B35_SHFT                                            0x3
#define HWIO_QFPROM_CORR_CALIB_ROW21_MSB_SPARE_R84_B34_BMSK                                            0x4
#define HWIO_QFPROM_CORR_CALIB_ROW21_MSB_SPARE_R84_B34_SHFT                                            0x2
#define HWIO_QFPROM_CORR_CALIB_ROW21_MSB_SPARE_R84_B33_BMSK                                            0x2
#define HWIO_QFPROM_CORR_CALIB_ROW21_MSB_SPARE_R84_B33_SHFT                                            0x1
#define HWIO_QFPROM_CORR_CALIB_ROW21_MSB_SPARE_R84_B32_BMSK                                            0x1
#define HWIO_QFPROM_CORR_CALIB_ROW21_MSB_SPARE_R84_B32_SHFT                                            0x0

#define HWIO_QFPROM_CORR_CALIB_ROW22_LSB_ADDR                                                   (SECURITY_CONTROL_CORE_REG_BASE      + 0x000042a8)
#define HWIO_QFPROM_CORR_CALIB_ROW22_LSB_RMSK                                                   0xffffffff
#define HWIO_QFPROM_CORR_CALIB_ROW22_LSB_IN          \
        in_dword_masked(HWIO_QFPROM_CORR_CALIB_ROW22_LSB_ADDR, HWIO_QFPROM_CORR_CALIB_ROW22_LSB_RMSK)
#define HWIO_QFPROM_CORR_CALIB_ROW22_LSB_INM(m)      \
        in_dword_masked(HWIO_QFPROM_CORR_CALIB_ROW22_LSB_ADDR, m)
#define HWIO_QFPROM_CORR_CALIB_ROW22_LSB_SPARE_R85_B31_BMSK                                     0x80000000
#define HWIO_QFPROM_CORR_CALIB_ROW22_LSB_SPARE_R85_B31_SHFT                                           0x1f
#define HWIO_QFPROM_CORR_CALIB_ROW22_LSB_SPARE_R85_B30_BMSK                                     0x40000000
#define HWIO_QFPROM_CORR_CALIB_ROW22_LSB_SPARE_R85_B30_SHFT                                           0x1e
#define HWIO_QFPROM_CORR_CALIB_ROW22_LSB_SPARE_R85_B29_BMSK                                     0x20000000
#define HWIO_QFPROM_CORR_CALIB_ROW22_LSB_SPARE_R85_B29_SHFT                                           0x1d
#define HWIO_QFPROM_CORR_CALIB_ROW22_LSB_SPARE_R85_B28_BMSK                                     0x10000000
#define HWIO_QFPROM_CORR_CALIB_ROW22_LSB_SPARE_R85_B28_SHFT                                           0x1c
#define HWIO_QFPROM_CORR_CALIB_ROW22_LSB_SPARE_R85_B27_BMSK                                      0x8000000
#define HWIO_QFPROM_CORR_CALIB_ROW22_LSB_SPARE_R85_B27_SHFT                                           0x1b
#define HWIO_QFPROM_CORR_CALIB_ROW22_LSB_SPARE_R85_B26_BMSK                                      0x4000000
#define HWIO_QFPROM_CORR_CALIB_ROW22_LSB_SPARE_R85_B26_SHFT                                           0x1a
#define HWIO_QFPROM_CORR_CALIB_ROW22_LSB_SPARE_R85_B25_BMSK                                      0x2000000
#define HWIO_QFPROM_CORR_CALIB_ROW22_LSB_SPARE_R85_B25_SHFT                                           0x19
#define HWIO_QFPROM_CORR_CALIB_ROW22_LSB_SPARE_R85_B24_BMSK                                      0x1000000
#define HWIO_QFPROM_CORR_CALIB_ROW22_LSB_SPARE_R85_B24_SHFT                                           0x18
#define HWIO_QFPROM_CORR_CALIB_ROW22_LSB_SPARE_R85_B23_BMSK                                       0x800000
#define HWIO_QFPROM_CORR_CALIB_ROW22_LSB_SPARE_R85_B23_SHFT                                           0x17
#define HWIO_QFPROM_CORR_CALIB_ROW22_LSB_SPARE_R85_B22_BMSK                                       0x400000
#define HWIO_QFPROM_CORR_CALIB_ROW22_LSB_SPARE_R85_B22_SHFT                                           0x16
#define HWIO_QFPROM_CORR_CALIB_ROW22_LSB_SPARE_R85_B21_BMSK                                       0x200000
#define HWIO_QFPROM_CORR_CALIB_ROW22_LSB_SPARE_R85_B21_SHFT                                           0x15
#define HWIO_QFPROM_CORR_CALIB_ROW22_LSB_SPARE_R85_B20_BMSK                                       0x100000
#define HWIO_QFPROM_CORR_CALIB_ROW22_LSB_SPARE_R85_B20_SHFT                                           0x14
#define HWIO_QFPROM_CORR_CALIB_ROW22_LSB_SPARE_R85_B19_BMSK                                        0x80000
#define HWIO_QFPROM_CORR_CALIB_ROW22_LSB_SPARE_R85_B19_SHFT                                           0x13
#define HWIO_QFPROM_CORR_CALIB_ROW22_LSB_SPARE_R85_B18_BMSK                                        0x40000
#define HWIO_QFPROM_CORR_CALIB_ROW22_LSB_SPARE_R85_B18_SHFT                                           0x12
#define HWIO_QFPROM_CORR_CALIB_ROW22_LSB_SPARE_R85_B17_BMSK                                        0x20000
#define HWIO_QFPROM_CORR_CALIB_ROW22_LSB_SPARE_R85_B17_SHFT                                           0x11
#define HWIO_QFPROM_CORR_CALIB_ROW22_LSB_SPARE_R85_B16_BMSK                                        0x10000
#define HWIO_QFPROM_CORR_CALIB_ROW22_LSB_SPARE_R85_B16_SHFT                                           0x10
#define HWIO_QFPROM_CORR_CALIB_ROW22_LSB_SPARE_R85_B15_BMSK                                         0x8000
#define HWIO_QFPROM_CORR_CALIB_ROW22_LSB_SPARE_R85_B15_SHFT                                            0xf
#define HWIO_QFPROM_CORR_CALIB_ROW22_LSB_SPARE_R85_B14_BMSK                                         0x4000
#define HWIO_QFPROM_CORR_CALIB_ROW22_LSB_SPARE_R85_B14_SHFT                                            0xe
#define HWIO_QFPROM_CORR_CALIB_ROW22_LSB_SPARE_R85_B13_BMSK                                         0x2000
#define HWIO_QFPROM_CORR_CALIB_ROW22_LSB_SPARE_R85_B13_SHFT                                            0xd
#define HWIO_QFPROM_CORR_CALIB_ROW22_LSB_SPARE_R85_B12_BMSK                                         0x1000
#define HWIO_QFPROM_CORR_CALIB_ROW22_LSB_SPARE_R85_B12_SHFT                                            0xc
#define HWIO_QFPROM_CORR_CALIB_ROW22_LSB_SPARE_R85_B11_BMSK                                          0x800
#define HWIO_QFPROM_CORR_CALIB_ROW22_LSB_SPARE_R85_B11_SHFT                                            0xb
#define HWIO_QFPROM_CORR_CALIB_ROW22_LSB_SPARE_R85_B10_BMSK                                          0x400
#define HWIO_QFPROM_CORR_CALIB_ROW22_LSB_SPARE_R85_B10_SHFT                                            0xa
#define HWIO_QFPROM_CORR_CALIB_ROW22_LSB_SPARE_R85_B9_BMSK                                           0x200
#define HWIO_QFPROM_CORR_CALIB_ROW22_LSB_SPARE_R85_B9_SHFT                                             0x9
#define HWIO_QFPROM_CORR_CALIB_ROW22_LSB_SPARE_R85_B8_BMSK                                           0x100
#define HWIO_QFPROM_CORR_CALIB_ROW22_LSB_SPARE_R85_B8_SHFT                                             0x8
#define HWIO_QFPROM_CORR_CALIB_ROW22_LSB_SPARE_R85_B7_BMSK                                            0x80
#define HWIO_QFPROM_CORR_CALIB_ROW22_LSB_SPARE_R85_B7_SHFT                                             0x7
#define HWIO_QFPROM_CORR_CALIB_ROW22_LSB_SPARE_R85_B6_BMSK                                            0x40
#define HWIO_QFPROM_CORR_CALIB_ROW22_LSB_SPARE_R85_B6_SHFT                                             0x6
#define HWIO_QFPROM_CORR_CALIB_ROW22_LSB_SPARE_R85_B5_BMSK                                            0x20
#define HWIO_QFPROM_CORR_CALIB_ROW22_LSB_SPARE_R85_B5_SHFT                                             0x5
#define HWIO_QFPROM_CORR_CALIB_ROW22_LSB_SPARE_R85_B4_BMSK                                            0x10
#define HWIO_QFPROM_CORR_CALIB_ROW22_LSB_SPARE_R85_B4_SHFT                                             0x4
#define HWIO_QFPROM_CORR_CALIB_ROW22_LSB_SPARE_R85_B3_BMSK                                             0x8
#define HWIO_QFPROM_CORR_CALIB_ROW22_LSB_SPARE_R85_B3_SHFT                                             0x3
#define HWIO_QFPROM_CORR_CALIB_ROW22_LSB_SPARE_R85_B2_BMSK                                             0x4
#define HWIO_QFPROM_CORR_CALIB_ROW22_LSB_SPARE_R85_B2_SHFT                                             0x2
#define HWIO_QFPROM_CORR_CALIB_ROW22_LSB_SPARE_R85_B1_BMSK                                             0x2
#define HWIO_QFPROM_CORR_CALIB_ROW22_LSB_SPARE_R85_B1_SHFT                                             0x1
#define HWIO_QFPROM_CORR_CALIB_ROW22_LSB_SPARE_R85_B0_BMSK                                             0x1
#define HWIO_QFPROM_CORR_CALIB_ROW22_LSB_SPARE_R85_B0_SHFT                                             0x0

#define HWIO_QFPROM_CORR_CALIB_ROW22_MSB_ADDR                                                   (SECURITY_CONTROL_CORE_REG_BASE      + 0x000042ac)
#define HWIO_QFPROM_CORR_CALIB_ROW22_MSB_RMSK                                                   0xffffffff
#define HWIO_QFPROM_CORR_CALIB_ROW22_MSB_IN          \
        in_dword_masked(HWIO_QFPROM_CORR_CALIB_ROW22_MSB_ADDR, HWIO_QFPROM_CORR_CALIB_ROW22_MSB_RMSK)
#define HWIO_QFPROM_CORR_CALIB_ROW22_MSB_INM(m)      \
        in_dword_masked(HWIO_QFPROM_CORR_CALIB_ROW22_MSB_ADDR, m)
#define HWIO_QFPROM_CORR_CALIB_ROW22_MSB_SPARE_R85_B63_BMSK                                     0x80000000
#define HWIO_QFPROM_CORR_CALIB_ROW22_MSB_SPARE_R85_B63_SHFT                                           0x1f
#define HWIO_QFPROM_CORR_CALIB_ROW22_MSB_SPARE_R85_B62_BMSK                                     0x40000000
#define HWIO_QFPROM_CORR_CALIB_ROW22_MSB_SPARE_R85_B62_SHFT                                           0x1e
#define HWIO_QFPROM_CORR_CALIB_ROW22_MSB_SPARE_R85_B61_BMSK                                     0x20000000
#define HWIO_QFPROM_CORR_CALIB_ROW22_MSB_SPARE_R85_B61_SHFT                                           0x1d
#define HWIO_QFPROM_CORR_CALIB_ROW22_MSB_SPARE_R85_B60_BMSK                                     0x10000000
#define HWIO_QFPROM_CORR_CALIB_ROW22_MSB_SPARE_R85_B60_SHFT                                           0x1c
#define HWIO_QFPROM_CORR_CALIB_ROW22_MSB_SPARE_R85_B59_BMSK                                      0x8000000
#define HWIO_QFPROM_CORR_CALIB_ROW22_MSB_SPARE_R85_B59_SHFT                                           0x1b
#define HWIO_QFPROM_CORR_CALIB_ROW22_MSB_SPARE_R85_B58_BMSK                                      0x4000000
#define HWIO_QFPROM_CORR_CALIB_ROW22_MSB_SPARE_R85_B58_SHFT                                           0x1a
#define HWIO_QFPROM_CORR_CALIB_ROW22_MSB_SPARE_R85_B57_BMSK                                      0x2000000
#define HWIO_QFPROM_CORR_CALIB_ROW22_MSB_SPARE_R85_B57_SHFT                                           0x19
#define HWIO_QFPROM_CORR_CALIB_ROW22_MSB_SPARE_R85_B56_BMSK                                      0x1000000
#define HWIO_QFPROM_CORR_CALIB_ROW22_MSB_SPARE_R85_B56_SHFT                                           0x18
#define HWIO_QFPROM_CORR_CALIB_ROW22_MSB_SPARE_R85_B55_BMSK                                       0x800000
#define HWIO_QFPROM_CORR_CALIB_ROW22_MSB_SPARE_R85_B55_SHFT                                           0x17
#define HWIO_QFPROM_CORR_CALIB_ROW22_MSB_SPARE_R85_B54_BMSK                                       0x400000
#define HWIO_QFPROM_CORR_CALIB_ROW22_MSB_SPARE_R85_B54_SHFT                                           0x16
#define HWIO_QFPROM_CORR_CALIB_ROW22_MSB_SPARE_R85_B53_BMSK                                       0x200000
#define HWIO_QFPROM_CORR_CALIB_ROW22_MSB_SPARE_R85_B53_SHFT                                           0x15
#define HWIO_QFPROM_CORR_CALIB_ROW22_MSB_SPARE_R85_B52_BMSK                                       0x100000
#define HWIO_QFPROM_CORR_CALIB_ROW22_MSB_SPARE_R85_B52_SHFT                                           0x14
#define HWIO_QFPROM_CORR_CALIB_ROW22_MSB_SPARE_R85_B51_BMSK                                        0x80000
#define HWIO_QFPROM_CORR_CALIB_ROW22_MSB_SPARE_R85_B51_SHFT                                           0x13
#define HWIO_QFPROM_CORR_CALIB_ROW22_MSB_SPARE_R85_B50_BMSK                                        0x40000
#define HWIO_QFPROM_CORR_CALIB_ROW22_MSB_SPARE_R85_B50_SHFT                                           0x12
#define HWIO_QFPROM_CORR_CALIB_ROW22_MSB_SPARE_R85_B49_BMSK                                        0x20000
#define HWIO_QFPROM_CORR_CALIB_ROW22_MSB_SPARE_R85_B49_SHFT                                           0x11
#define HWIO_QFPROM_CORR_CALIB_ROW22_MSB_SPARE_R85_B48_BMSK                                        0x10000
#define HWIO_QFPROM_CORR_CALIB_ROW22_MSB_SPARE_R85_B48_SHFT                                           0x10
#define HWIO_QFPROM_CORR_CALIB_ROW22_MSB_SPARE_R85_B47_BMSK                                         0x8000
#define HWIO_QFPROM_CORR_CALIB_ROW22_MSB_SPARE_R85_B47_SHFT                                            0xf
#define HWIO_QFPROM_CORR_CALIB_ROW22_MSB_SPARE_R85_B46_BMSK                                         0x4000
#define HWIO_QFPROM_CORR_CALIB_ROW22_MSB_SPARE_R85_B46_SHFT                                            0xe
#define HWIO_QFPROM_CORR_CALIB_ROW22_MSB_SPARE_R85_B45_BMSK                                         0x2000
#define HWIO_QFPROM_CORR_CALIB_ROW22_MSB_SPARE_R85_B45_SHFT                                            0xd
#define HWIO_QFPROM_CORR_CALIB_ROW22_MSB_SPARE_R85_B44_BMSK                                         0x1000
#define HWIO_QFPROM_CORR_CALIB_ROW22_MSB_SPARE_R85_B44_SHFT                                            0xc
#define HWIO_QFPROM_CORR_CALIB_ROW22_MSB_SPARE_R85_B43_BMSK                                          0x800
#define HWIO_QFPROM_CORR_CALIB_ROW22_MSB_SPARE_R85_B43_SHFT                                            0xb
#define HWIO_QFPROM_CORR_CALIB_ROW22_MSB_SPARE_R85_B42_BMSK                                          0x400
#define HWIO_QFPROM_CORR_CALIB_ROW22_MSB_SPARE_R85_B42_SHFT                                            0xa
#define HWIO_QFPROM_CORR_CALIB_ROW22_MSB_SPARE_R85_B41_BMSK                                          0x200
#define HWIO_QFPROM_CORR_CALIB_ROW22_MSB_SPARE_R85_B41_SHFT                                            0x9
#define HWIO_QFPROM_CORR_CALIB_ROW22_MSB_SPARE_R85_B40_BMSK                                          0x100
#define HWIO_QFPROM_CORR_CALIB_ROW22_MSB_SPARE_R85_B40_SHFT                                            0x8
#define HWIO_QFPROM_CORR_CALIB_ROW22_MSB_SPARE_R85_B39_BMSK                                           0x80
#define HWIO_QFPROM_CORR_CALIB_ROW22_MSB_SPARE_R85_B39_SHFT                                            0x7
#define HWIO_QFPROM_CORR_CALIB_ROW22_MSB_SPARE_R85_B38_BMSK                                           0x40
#define HWIO_QFPROM_CORR_CALIB_ROW22_MSB_SPARE_R85_B38_SHFT                                            0x6
#define HWIO_QFPROM_CORR_CALIB_ROW22_MSB_SPARE_R85_B37_BMSK                                           0x20
#define HWIO_QFPROM_CORR_CALIB_ROW22_MSB_SPARE_R85_B37_SHFT                                            0x5
#define HWIO_QFPROM_CORR_CALIB_ROW22_MSB_SPARE_R85_B36_BMSK                                           0x10
#define HWIO_QFPROM_CORR_CALIB_ROW22_MSB_SPARE_R85_B36_SHFT                                            0x4
#define HWIO_QFPROM_CORR_CALIB_ROW22_MSB_SPARE_R85_B35_BMSK                                            0x8
#define HWIO_QFPROM_CORR_CALIB_ROW22_MSB_SPARE_R85_B35_SHFT                                            0x3
#define HWIO_QFPROM_CORR_CALIB_ROW22_MSB_SPARE_R85_B34_BMSK                                            0x4
#define HWIO_QFPROM_CORR_CALIB_ROW22_MSB_SPARE_R85_B34_SHFT                                            0x2
#define HWIO_QFPROM_CORR_CALIB_ROW22_MSB_SPARE_R85_B33_BMSK                                            0x2
#define HWIO_QFPROM_CORR_CALIB_ROW22_MSB_SPARE_R85_B33_SHFT                                            0x1
#define HWIO_QFPROM_CORR_CALIB_ROW22_MSB_SPARE_R85_B32_BMSK                                            0x1
#define HWIO_QFPROM_CORR_CALIB_ROW22_MSB_SPARE_R85_B32_SHFT                                            0x0

#define HWIO_QFPROM_CORR_CALIB_ROW23_LSB_ADDR                                                   (SECURITY_CONTROL_CORE_REG_BASE      + 0x000042b0)
#define HWIO_QFPROM_CORR_CALIB_ROW23_LSB_RMSK                                                   0xffffffff
#define HWIO_QFPROM_CORR_CALIB_ROW23_LSB_IN          \
        in_dword_masked(HWIO_QFPROM_CORR_CALIB_ROW23_LSB_ADDR, HWIO_QFPROM_CORR_CALIB_ROW23_LSB_RMSK)
#define HWIO_QFPROM_CORR_CALIB_ROW23_LSB_INM(m)      \
        in_dword_masked(HWIO_QFPROM_CORR_CALIB_ROW23_LSB_ADDR, m)
#define HWIO_QFPROM_CORR_CALIB_ROW23_LSB_SPARE_R86_B31_BMSK                                     0x80000000
#define HWIO_QFPROM_CORR_CALIB_ROW23_LSB_SPARE_R86_B31_SHFT                                           0x1f
#define HWIO_QFPROM_CORR_CALIB_ROW23_LSB_SPARE_R86_B30_BMSK                                     0x40000000
#define HWIO_QFPROM_CORR_CALIB_ROW23_LSB_SPARE_R86_B30_SHFT                                           0x1e
#define HWIO_QFPROM_CORR_CALIB_ROW23_LSB_SPARE_R86_B29_BMSK                                     0x20000000
#define HWIO_QFPROM_CORR_CALIB_ROW23_LSB_SPARE_R86_B29_SHFT                                           0x1d
#define HWIO_QFPROM_CORR_CALIB_ROW23_LSB_SPARE_R86_B28_BMSK                                     0x10000000
#define HWIO_QFPROM_CORR_CALIB_ROW23_LSB_SPARE_R86_B28_SHFT                                           0x1c
#define HWIO_QFPROM_CORR_CALIB_ROW23_LSB_SPARE_R86_B27_BMSK                                      0x8000000
#define HWIO_QFPROM_CORR_CALIB_ROW23_LSB_SPARE_R86_B27_SHFT                                           0x1b
#define HWIO_QFPROM_CORR_CALIB_ROW23_LSB_SPARE_R86_B26_BMSK                                      0x4000000
#define HWIO_QFPROM_CORR_CALIB_ROW23_LSB_SPARE_R86_B26_SHFT                                           0x1a
#define HWIO_QFPROM_CORR_CALIB_ROW23_LSB_SPARE_R86_B25_BMSK                                      0x2000000
#define HWIO_QFPROM_CORR_CALIB_ROW23_LSB_SPARE_R86_B25_SHFT                                           0x19
#define HWIO_QFPROM_CORR_CALIB_ROW23_LSB_SPARE_R86_B24_BMSK                                      0x1000000
#define HWIO_QFPROM_CORR_CALIB_ROW23_LSB_SPARE_R86_B24_SHFT                                           0x18
#define HWIO_QFPROM_CORR_CALIB_ROW23_LSB_SPARE_R86_B23_BMSK                                       0x800000
#define HWIO_QFPROM_CORR_CALIB_ROW23_LSB_SPARE_R86_B23_SHFT                                           0x17
#define HWIO_QFPROM_CORR_CALIB_ROW23_LSB_SPARE_R86_B22_BMSK                                       0x400000
#define HWIO_QFPROM_CORR_CALIB_ROW23_LSB_SPARE_R86_B22_SHFT                                           0x16
#define HWIO_QFPROM_CORR_CALIB_ROW23_LSB_SPARE_R86_B21_BMSK                                       0x200000
#define HWIO_QFPROM_CORR_CALIB_ROW23_LSB_SPARE_R86_B21_SHFT                                           0x15
#define HWIO_QFPROM_CORR_CALIB_ROW23_LSB_SPARE_R86_B20_BMSK                                       0x100000
#define HWIO_QFPROM_CORR_CALIB_ROW23_LSB_SPARE_R86_B20_SHFT                                           0x14
#define HWIO_QFPROM_CORR_CALIB_ROW23_LSB_SPARE_R86_B19_BMSK                                        0x80000
#define HWIO_QFPROM_CORR_CALIB_ROW23_LSB_SPARE_R86_B19_SHFT                                           0x13
#define HWIO_QFPROM_CORR_CALIB_ROW23_LSB_SPARE_R86_B18_BMSK                                        0x40000
#define HWIO_QFPROM_CORR_CALIB_ROW23_LSB_SPARE_R86_B18_SHFT                                           0x12
#define HWIO_QFPROM_CORR_CALIB_ROW23_LSB_SPARE_R86_B17_BMSK                                        0x20000
#define HWIO_QFPROM_CORR_CALIB_ROW23_LSB_SPARE_R86_B17_SHFT                                           0x11
#define HWIO_QFPROM_CORR_CALIB_ROW23_LSB_SPARE_R86_B16_BMSK                                        0x10000
#define HWIO_QFPROM_CORR_CALIB_ROW23_LSB_SPARE_R86_B16_SHFT                                           0x10
#define HWIO_QFPROM_CORR_CALIB_ROW23_LSB_SPARE_R86_B15_BMSK                                         0x8000
#define HWIO_QFPROM_CORR_CALIB_ROW23_LSB_SPARE_R86_B15_SHFT                                            0xf
#define HWIO_QFPROM_CORR_CALIB_ROW23_LSB_SPARE_R86_B14_BMSK                                         0x4000
#define HWIO_QFPROM_CORR_CALIB_ROW23_LSB_SPARE_R86_B14_SHFT                                            0xe
#define HWIO_QFPROM_CORR_CALIB_ROW23_LSB_SPARE_R86_B13_BMSK                                         0x2000
#define HWIO_QFPROM_CORR_CALIB_ROW23_LSB_SPARE_R86_B13_SHFT                                            0xd
#define HWIO_QFPROM_CORR_CALIB_ROW23_LSB_SPARE_R86_B12_BMSK                                         0x1000
#define HWIO_QFPROM_CORR_CALIB_ROW23_LSB_SPARE_R86_B12_SHFT                                            0xc
#define HWIO_QFPROM_CORR_CALIB_ROW23_LSB_SPARE_R86_B11_BMSK                                          0x800
#define HWIO_QFPROM_CORR_CALIB_ROW23_LSB_SPARE_R86_B11_SHFT                                            0xb
#define HWIO_QFPROM_CORR_CALIB_ROW23_LSB_SPARE_R86_B10_BMSK                                          0x400
#define HWIO_QFPROM_CORR_CALIB_ROW23_LSB_SPARE_R86_B10_SHFT                                            0xa
#define HWIO_QFPROM_CORR_CALIB_ROW23_LSB_SPARE_R86_B9_BMSK                                           0x200
#define HWIO_QFPROM_CORR_CALIB_ROW23_LSB_SPARE_R86_B9_SHFT                                             0x9
#define HWIO_QFPROM_CORR_CALIB_ROW23_LSB_SPARE_R86_B8_BMSK                                           0x100
#define HWIO_QFPROM_CORR_CALIB_ROW23_LSB_SPARE_R86_B8_SHFT                                             0x8
#define HWIO_QFPROM_CORR_CALIB_ROW23_LSB_SPARE_R86_B7_BMSK                                            0x80
#define HWIO_QFPROM_CORR_CALIB_ROW23_LSB_SPARE_R86_B7_SHFT                                             0x7
#define HWIO_QFPROM_CORR_CALIB_ROW23_LSB_SPARE_R86_B6_BMSK                                            0x40
#define HWIO_QFPROM_CORR_CALIB_ROW23_LSB_SPARE_R86_B6_SHFT                                             0x6
#define HWIO_QFPROM_CORR_CALIB_ROW23_LSB_SPARE_R86_B5_BMSK                                            0x20
#define HWIO_QFPROM_CORR_CALIB_ROW23_LSB_SPARE_R86_B5_SHFT                                             0x5
#define HWIO_QFPROM_CORR_CALIB_ROW23_LSB_SPARE_R86_B4_BMSK                                            0x10
#define HWIO_QFPROM_CORR_CALIB_ROW23_LSB_SPARE_R86_B4_SHFT                                             0x4
#define HWIO_QFPROM_CORR_CALIB_ROW23_LSB_SPARE_R86_B3_BMSK                                             0x8
#define HWIO_QFPROM_CORR_CALIB_ROW23_LSB_SPARE_R86_B3_SHFT                                             0x3
#define HWIO_QFPROM_CORR_CALIB_ROW23_LSB_SPARE_R86_B2_BMSK                                             0x4
#define HWIO_QFPROM_CORR_CALIB_ROW23_LSB_SPARE_R86_B2_SHFT                                             0x2
#define HWIO_QFPROM_CORR_CALIB_ROW23_LSB_SPARE_R86_B1_BMSK                                             0x2
#define HWIO_QFPROM_CORR_CALIB_ROW23_LSB_SPARE_R86_B1_SHFT                                             0x1
#define HWIO_QFPROM_CORR_CALIB_ROW23_LSB_SPARE_R86_B0_BMSK                                             0x1
#define HWIO_QFPROM_CORR_CALIB_ROW23_LSB_SPARE_R86_B0_SHFT                                             0x0

#define HWIO_QFPROM_CORR_CALIB_ROW23_MSB_ADDR                                                   (SECURITY_CONTROL_CORE_REG_BASE      + 0x000042b4)
#define HWIO_QFPROM_CORR_CALIB_ROW23_MSB_RMSK                                                   0xffffffff
#define HWIO_QFPROM_CORR_CALIB_ROW23_MSB_IN          \
        in_dword_masked(HWIO_QFPROM_CORR_CALIB_ROW23_MSB_ADDR, HWIO_QFPROM_CORR_CALIB_ROW23_MSB_RMSK)
#define HWIO_QFPROM_CORR_CALIB_ROW23_MSB_INM(m)      \
        in_dword_masked(HWIO_QFPROM_CORR_CALIB_ROW23_MSB_ADDR, m)
#define HWIO_QFPROM_CORR_CALIB_ROW23_MSB_SPARE_R86_B63_BMSK                                     0x80000000
#define HWIO_QFPROM_CORR_CALIB_ROW23_MSB_SPARE_R86_B63_SHFT                                           0x1f
#define HWIO_QFPROM_CORR_CALIB_ROW23_MSB_SPARE_R86_B62_BMSK                                     0x40000000
#define HWIO_QFPROM_CORR_CALIB_ROW23_MSB_SPARE_R86_B62_SHFT                                           0x1e
#define HWIO_QFPROM_CORR_CALIB_ROW23_MSB_SPARE_R86_B61_BMSK                                     0x20000000
#define HWIO_QFPROM_CORR_CALIB_ROW23_MSB_SPARE_R86_B61_SHFT                                           0x1d
#define HWIO_QFPROM_CORR_CALIB_ROW23_MSB_SPARE_R86_B60_BMSK                                     0x10000000
#define HWIO_QFPROM_CORR_CALIB_ROW23_MSB_SPARE_R86_B60_SHFT                                           0x1c
#define HWIO_QFPROM_CORR_CALIB_ROW23_MSB_SPARE_R86_B59_BMSK                                      0x8000000
#define HWIO_QFPROM_CORR_CALIB_ROW23_MSB_SPARE_R86_B59_SHFT                                           0x1b
#define HWIO_QFPROM_CORR_CALIB_ROW23_MSB_SPARE_R86_B58_BMSK                                      0x4000000
#define HWIO_QFPROM_CORR_CALIB_ROW23_MSB_SPARE_R86_B58_SHFT                                           0x1a
#define HWIO_QFPROM_CORR_CALIB_ROW23_MSB_SPARE_R86_B57_BMSK                                      0x2000000
#define HWIO_QFPROM_CORR_CALIB_ROW23_MSB_SPARE_R86_B57_SHFT                                           0x19
#define HWIO_QFPROM_CORR_CALIB_ROW23_MSB_SPARE_R86_B56_BMSK                                      0x1000000
#define HWIO_QFPROM_CORR_CALIB_ROW23_MSB_SPARE_R86_B56_SHFT                                           0x18
#define HWIO_QFPROM_CORR_CALIB_ROW23_MSB_SPARE_R86_B55_BMSK                                       0x800000
#define HWIO_QFPROM_CORR_CALIB_ROW23_MSB_SPARE_R86_B55_SHFT                                           0x17
#define HWIO_QFPROM_CORR_CALIB_ROW23_MSB_SPARE_R86_B54_BMSK                                       0x400000
#define HWIO_QFPROM_CORR_CALIB_ROW23_MSB_SPARE_R86_B54_SHFT                                           0x16
#define HWIO_QFPROM_CORR_CALIB_ROW23_MSB_SPARE_R86_B53_BMSK                                       0x200000
#define HWIO_QFPROM_CORR_CALIB_ROW23_MSB_SPARE_R86_B53_SHFT                                           0x15
#define HWIO_QFPROM_CORR_CALIB_ROW23_MSB_SPARE_R86_B52_BMSK                                       0x100000
#define HWIO_QFPROM_CORR_CALIB_ROW23_MSB_SPARE_R86_B52_SHFT                                           0x14
#define HWIO_QFPROM_CORR_CALIB_ROW23_MSB_SPARE_R86_B51_BMSK                                        0x80000
#define HWIO_QFPROM_CORR_CALIB_ROW23_MSB_SPARE_R86_B51_SHFT                                           0x13
#define HWIO_QFPROM_CORR_CALIB_ROW23_MSB_SPARE_R86_B50_BMSK                                        0x40000
#define HWIO_QFPROM_CORR_CALIB_ROW23_MSB_SPARE_R86_B50_SHFT                                           0x12
#define HWIO_QFPROM_CORR_CALIB_ROW23_MSB_SPARE_R86_B49_BMSK                                        0x20000
#define HWIO_QFPROM_CORR_CALIB_ROW23_MSB_SPARE_R86_B49_SHFT                                           0x11
#define HWIO_QFPROM_CORR_CALIB_ROW23_MSB_SPARE_R86_B48_BMSK                                        0x10000
#define HWIO_QFPROM_CORR_CALIB_ROW23_MSB_SPARE_R86_B48_SHFT                                           0x10
#define HWIO_QFPROM_CORR_CALIB_ROW23_MSB_SPARE_R86_B47_BMSK                                         0x8000
#define HWIO_QFPROM_CORR_CALIB_ROW23_MSB_SPARE_R86_B47_SHFT                                            0xf
#define HWIO_QFPROM_CORR_CALIB_ROW23_MSB_SPARE_R86_B46_BMSK                                         0x4000
#define HWIO_QFPROM_CORR_CALIB_ROW23_MSB_SPARE_R86_B46_SHFT                                            0xe
#define HWIO_QFPROM_CORR_CALIB_ROW23_MSB_SPARE_R86_B45_BMSK                                         0x2000
#define HWIO_QFPROM_CORR_CALIB_ROW23_MSB_SPARE_R86_B45_SHFT                                            0xd
#define HWIO_QFPROM_CORR_CALIB_ROW23_MSB_SPARE_R86_B44_BMSK                                         0x1000
#define HWIO_QFPROM_CORR_CALIB_ROW23_MSB_SPARE_R86_B44_SHFT                                            0xc
#define HWIO_QFPROM_CORR_CALIB_ROW23_MSB_SPARE_R86_B43_BMSK                                          0x800
#define HWIO_QFPROM_CORR_CALIB_ROW23_MSB_SPARE_R86_B43_SHFT                                            0xb
#define HWIO_QFPROM_CORR_CALIB_ROW23_MSB_SPARE_R86_B42_BMSK                                          0x400
#define HWIO_QFPROM_CORR_CALIB_ROW23_MSB_SPARE_R86_B42_SHFT                                            0xa
#define HWIO_QFPROM_CORR_CALIB_ROW23_MSB_SPARE_R86_B41_BMSK                                          0x200
#define HWIO_QFPROM_CORR_CALIB_ROW23_MSB_SPARE_R86_B41_SHFT                                            0x9
#define HWIO_QFPROM_CORR_CALIB_ROW23_MSB_SPARE_R86_B40_BMSK                                          0x100
#define HWIO_QFPROM_CORR_CALIB_ROW23_MSB_SPARE_R86_B40_SHFT                                            0x8
#define HWIO_QFPROM_CORR_CALIB_ROW23_MSB_SPARE_R86_B39_BMSK                                           0x80
#define HWIO_QFPROM_CORR_CALIB_ROW23_MSB_SPARE_R86_B39_SHFT                                            0x7
#define HWIO_QFPROM_CORR_CALIB_ROW23_MSB_SPARE_R86_B38_BMSK                                           0x40
#define HWIO_QFPROM_CORR_CALIB_ROW23_MSB_SPARE_R86_B38_SHFT                                            0x6
#define HWIO_QFPROM_CORR_CALIB_ROW23_MSB_SPARE_R86_B37_BMSK                                           0x20
#define HWIO_QFPROM_CORR_CALIB_ROW23_MSB_SPARE_R86_B37_SHFT                                            0x5
#define HWIO_QFPROM_CORR_CALIB_ROW23_MSB_SPARE_R86_B36_BMSK                                           0x10
#define HWIO_QFPROM_CORR_CALIB_ROW23_MSB_SPARE_R86_B36_SHFT                                            0x4
#define HWIO_QFPROM_CORR_CALIB_ROW23_MSB_SPARE_R86_B35_BMSK                                            0x8
#define HWIO_QFPROM_CORR_CALIB_ROW23_MSB_SPARE_R86_B35_SHFT                                            0x3
#define HWIO_QFPROM_CORR_CALIB_ROW23_MSB_SPARE_R86_B34_BMSK                                            0x4
#define HWIO_QFPROM_CORR_CALIB_ROW23_MSB_SPARE_R86_B34_SHFT                                            0x2
#define HWIO_QFPROM_CORR_CALIB_ROW23_MSB_SPARE_R86_B33_BMSK                                            0x2
#define HWIO_QFPROM_CORR_CALIB_ROW23_MSB_SPARE_R86_B33_SHFT                                            0x1
#define HWIO_QFPROM_CORR_CALIB_ROW23_MSB_SPARE_R86_B32_BMSK                                            0x1
#define HWIO_QFPROM_CORR_CALIB_ROW23_MSB_SPARE_R86_B32_SHFT                                            0x0

#define HWIO_QFPROM_CORR_CALIB_ROW24_LSB_ADDR                                                   (SECURITY_CONTROL_CORE_REG_BASE      + 0x000042b8)
#define HWIO_QFPROM_CORR_CALIB_ROW24_LSB_RMSK                                                   0xffffffff
#define HWIO_QFPROM_CORR_CALIB_ROW24_LSB_IN          \
        in_dword_masked(HWIO_QFPROM_CORR_CALIB_ROW24_LSB_ADDR, HWIO_QFPROM_CORR_CALIB_ROW24_LSB_RMSK)
#define HWIO_QFPROM_CORR_CALIB_ROW24_LSB_INM(m)      \
        in_dword_masked(HWIO_QFPROM_CORR_CALIB_ROW24_LSB_ADDR, m)
#define HWIO_QFPROM_CORR_CALIB_ROW24_LSB_SPARE_R87_B31_BMSK                                     0x80000000
#define HWIO_QFPROM_CORR_CALIB_ROW24_LSB_SPARE_R87_B31_SHFT                                           0x1f
#define HWIO_QFPROM_CORR_CALIB_ROW24_LSB_SPARE_R87_B30_BMSK                                     0x40000000
#define HWIO_QFPROM_CORR_CALIB_ROW24_LSB_SPARE_R87_B30_SHFT                                           0x1e
#define HWIO_QFPROM_CORR_CALIB_ROW24_LSB_SPARE_R87_B29_BMSK                                     0x20000000
#define HWIO_QFPROM_CORR_CALIB_ROW24_LSB_SPARE_R87_B29_SHFT                                           0x1d
#define HWIO_QFPROM_CORR_CALIB_ROW24_LSB_SPARE_R87_B28_BMSK                                     0x10000000
#define HWIO_QFPROM_CORR_CALIB_ROW24_LSB_SPARE_R87_B28_SHFT                                           0x1c
#define HWIO_QFPROM_CORR_CALIB_ROW24_LSB_SPARE_R87_B27_BMSK                                      0x8000000
#define HWIO_QFPROM_CORR_CALIB_ROW24_LSB_SPARE_R87_B27_SHFT                                           0x1b
#define HWIO_QFPROM_CORR_CALIB_ROW24_LSB_SPARE_R87_B26_BMSK                                      0x4000000
#define HWIO_QFPROM_CORR_CALIB_ROW24_LSB_SPARE_R87_B26_SHFT                                           0x1a
#define HWIO_QFPROM_CORR_CALIB_ROW24_LSB_SPARE_R87_B25_BMSK                                      0x2000000
#define HWIO_QFPROM_CORR_CALIB_ROW24_LSB_SPARE_R87_B25_SHFT                                           0x19
#define HWIO_QFPROM_CORR_CALIB_ROW24_LSB_SPARE_R87_B24_BMSK                                      0x1000000
#define HWIO_QFPROM_CORR_CALIB_ROW24_LSB_SPARE_R87_B24_SHFT                                           0x18
#define HWIO_QFPROM_CORR_CALIB_ROW24_LSB_SPARE_R87_B23_BMSK                                       0x800000
#define HWIO_QFPROM_CORR_CALIB_ROW24_LSB_SPARE_R87_B23_SHFT                                           0x17
#define HWIO_QFPROM_CORR_CALIB_ROW24_LSB_SPARE_R87_B22_BMSK                                       0x400000
#define HWIO_QFPROM_CORR_CALIB_ROW24_LSB_SPARE_R87_B22_SHFT                                           0x16
#define HWIO_QFPROM_CORR_CALIB_ROW24_LSB_SPARE_R87_B21_BMSK                                       0x200000
#define HWIO_QFPROM_CORR_CALIB_ROW24_LSB_SPARE_R87_B21_SHFT                                           0x15
#define HWIO_QFPROM_CORR_CALIB_ROW24_LSB_SPARE_R87_B20_BMSK                                       0x100000
#define HWIO_QFPROM_CORR_CALIB_ROW24_LSB_SPARE_R87_B20_SHFT                                           0x14
#define HWIO_QFPROM_CORR_CALIB_ROW24_LSB_SPARE_R87_B19_BMSK                                        0x80000
#define HWIO_QFPROM_CORR_CALIB_ROW24_LSB_SPARE_R87_B19_SHFT                                           0x13
#define HWIO_QFPROM_CORR_CALIB_ROW24_LSB_SPARE_R87_B18_BMSK                                        0x40000
#define HWIO_QFPROM_CORR_CALIB_ROW24_LSB_SPARE_R87_B18_SHFT                                           0x12
#define HWIO_QFPROM_CORR_CALIB_ROW24_LSB_SPARE_R87_B17_BMSK                                        0x20000
#define HWIO_QFPROM_CORR_CALIB_ROW24_LSB_SPARE_R87_B17_SHFT                                           0x11
#define HWIO_QFPROM_CORR_CALIB_ROW24_LSB_SPARE_R87_B16_BMSK                                        0x10000
#define HWIO_QFPROM_CORR_CALIB_ROW24_LSB_SPARE_R87_B16_SHFT                                           0x10
#define HWIO_QFPROM_CORR_CALIB_ROW24_LSB_SPARE_R87_B15_BMSK                                         0x8000
#define HWIO_QFPROM_CORR_CALIB_ROW24_LSB_SPARE_R87_B15_SHFT                                            0xf
#define HWIO_QFPROM_CORR_CALIB_ROW24_LSB_SPARE_R87_B14_BMSK                                         0x4000
#define HWIO_QFPROM_CORR_CALIB_ROW24_LSB_SPARE_R87_B14_SHFT                                            0xe
#define HWIO_QFPROM_CORR_CALIB_ROW24_LSB_SPARE_R87_B13_BMSK                                         0x2000
#define HWIO_QFPROM_CORR_CALIB_ROW24_LSB_SPARE_R87_B13_SHFT                                            0xd
#define HWIO_QFPROM_CORR_CALIB_ROW24_LSB_SPARE_R87_B12_BMSK                                         0x1000
#define HWIO_QFPROM_CORR_CALIB_ROW24_LSB_SPARE_R87_B12_SHFT                                            0xc
#define HWIO_QFPROM_CORR_CALIB_ROW24_LSB_SPARE_R87_B11_BMSK                                          0x800
#define HWIO_QFPROM_CORR_CALIB_ROW24_LSB_SPARE_R87_B11_SHFT                                            0xb
#define HWIO_QFPROM_CORR_CALIB_ROW24_LSB_SPARE_R87_B10_BMSK                                          0x400
#define HWIO_QFPROM_CORR_CALIB_ROW24_LSB_SPARE_R87_B10_SHFT                                            0xa
#define HWIO_QFPROM_CORR_CALIB_ROW24_LSB_SPARE_R87_B9_BMSK                                           0x200
#define HWIO_QFPROM_CORR_CALIB_ROW24_LSB_SPARE_R87_B9_SHFT                                             0x9
#define HWIO_QFPROM_CORR_CALIB_ROW24_LSB_SPARE_R87_B8_BMSK                                           0x100
#define HWIO_QFPROM_CORR_CALIB_ROW24_LSB_SPARE_R87_B8_SHFT                                             0x8
#define HWIO_QFPROM_CORR_CALIB_ROW24_LSB_SPARE_R87_B7_BMSK                                            0x80
#define HWIO_QFPROM_CORR_CALIB_ROW24_LSB_SPARE_R87_B7_SHFT                                             0x7
#define HWIO_QFPROM_CORR_CALIB_ROW24_LSB_SPARE_R87_B6_BMSK                                            0x40
#define HWIO_QFPROM_CORR_CALIB_ROW24_LSB_SPARE_R87_B6_SHFT                                             0x6
#define HWIO_QFPROM_CORR_CALIB_ROW24_LSB_SPARE_R87_B5_BMSK                                            0x20
#define HWIO_QFPROM_CORR_CALIB_ROW24_LSB_SPARE_R87_B5_SHFT                                             0x5
#define HWIO_QFPROM_CORR_CALIB_ROW24_LSB_SPARE_R87_B4_BMSK                                            0x10
#define HWIO_QFPROM_CORR_CALIB_ROW24_LSB_SPARE_R87_B4_SHFT                                             0x4
#define HWIO_QFPROM_CORR_CALIB_ROW24_LSB_SPARE_R87_B3_BMSK                                             0x8
#define HWIO_QFPROM_CORR_CALIB_ROW24_LSB_SPARE_R87_B3_SHFT                                             0x3
#define HWIO_QFPROM_CORR_CALIB_ROW24_LSB_SPARE_R87_B2_BMSK                                             0x4
#define HWIO_QFPROM_CORR_CALIB_ROW24_LSB_SPARE_R87_B2_SHFT                                             0x2
#define HWIO_QFPROM_CORR_CALIB_ROW24_LSB_SPARE_R87_B1_BMSK                                             0x2
#define HWIO_QFPROM_CORR_CALIB_ROW24_LSB_SPARE_R87_B1_SHFT                                             0x1
#define HWIO_QFPROM_CORR_CALIB_ROW24_LSB_SPARE_R87_B0_BMSK                                             0x1
#define HWIO_QFPROM_CORR_CALIB_ROW24_LSB_SPARE_R87_B0_SHFT                                             0x0

#define HWIO_QFPROM_CORR_CALIB_ROW24_MSB_ADDR                                                   (SECURITY_CONTROL_CORE_REG_BASE      + 0x000042bc)
#define HWIO_QFPROM_CORR_CALIB_ROW24_MSB_RMSK                                                   0xffffffff
#define HWIO_QFPROM_CORR_CALIB_ROW24_MSB_IN          \
        in_dword_masked(HWIO_QFPROM_CORR_CALIB_ROW24_MSB_ADDR, HWIO_QFPROM_CORR_CALIB_ROW24_MSB_RMSK)
#define HWIO_QFPROM_CORR_CALIB_ROW24_MSB_INM(m)      \
        in_dword_masked(HWIO_QFPROM_CORR_CALIB_ROW24_MSB_ADDR, m)
#define HWIO_QFPROM_CORR_CALIB_ROW24_MSB_SPARE_R87_B63_BMSK                                     0x80000000
#define HWIO_QFPROM_CORR_CALIB_ROW24_MSB_SPARE_R87_B63_SHFT                                           0x1f
#define HWIO_QFPROM_CORR_CALIB_ROW24_MSB_SPARE_R87_B62_BMSK                                     0x40000000
#define HWIO_QFPROM_CORR_CALIB_ROW24_MSB_SPARE_R87_B62_SHFT                                           0x1e
#define HWIO_QFPROM_CORR_CALIB_ROW24_MSB_SPARE_R87_B61_BMSK                                     0x20000000
#define HWIO_QFPROM_CORR_CALIB_ROW24_MSB_SPARE_R87_B61_SHFT                                           0x1d
#define HWIO_QFPROM_CORR_CALIB_ROW24_MSB_SPARE_R87_B60_BMSK                                     0x10000000
#define HWIO_QFPROM_CORR_CALIB_ROW24_MSB_SPARE_R87_B60_SHFT                                           0x1c
#define HWIO_QFPROM_CORR_CALIB_ROW24_MSB_SPARE_R87_B59_BMSK                                      0x8000000
#define HWIO_QFPROM_CORR_CALIB_ROW24_MSB_SPARE_R87_B59_SHFT                                           0x1b
#define HWIO_QFPROM_CORR_CALIB_ROW24_MSB_SPARE_R87_B58_BMSK                                      0x4000000
#define HWIO_QFPROM_CORR_CALIB_ROW24_MSB_SPARE_R87_B58_SHFT                                           0x1a
#define HWIO_QFPROM_CORR_CALIB_ROW24_MSB_SPARE_R87_B57_BMSK                                      0x2000000
#define HWIO_QFPROM_CORR_CALIB_ROW24_MSB_SPARE_R87_B57_SHFT                                           0x19
#define HWIO_QFPROM_CORR_CALIB_ROW24_MSB_SPARE_R87_B56_BMSK                                      0x1000000
#define HWIO_QFPROM_CORR_CALIB_ROW24_MSB_SPARE_R87_B56_SHFT                                           0x18
#define HWIO_QFPROM_CORR_CALIB_ROW24_MSB_SPARE_R87_B55_BMSK                                       0x800000
#define HWIO_QFPROM_CORR_CALIB_ROW24_MSB_SPARE_R87_B55_SHFT                                           0x17
#define HWIO_QFPROM_CORR_CALIB_ROW24_MSB_SPARE_R87_B54_BMSK                                       0x400000
#define HWIO_QFPROM_CORR_CALIB_ROW24_MSB_SPARE_R87_B54_SHFT                                           0x16
#define HWIO_QFPROM_CORR_CALIB_ROW24_MSB_SPARE_R87_B53_BMSK                                       0x200000
#define HWIO_QFPROM_CORR_CALIB_ROW24_MSB_SPARE_R87_B53_SHFT                                           0x15
#define HWIO_QFPROM_CORR_CALIB_ROW24_MSB_SPARE_R87_B52_BMSK                                       0x100000
#define HWIO_QFPROM_CORR_CALIB_ROW24_MSB_SPARE_R87_B52_SHFT                                           0x14
#define HWIO_QFPROM_CORR_CALIB_ROW24_MSB_SPARE_R87_B51_BMSK                                        0x80000
#define HWIO_QFPROM_CORR_CALIB_ROW24_MSB_SPARE_R87_B51_SHFT                                           0x13
#define HWIO_QFPROM_CORR_CALIB_ROW24_MSB_SPARE_R87_B50_BMSK                                        0x40000
#define HWIO_QFPROM_CORR_CALIB_ROW24_MSB_SPARE_R87_B50_SHFT                                           0x12
#define HWIO_QFPROM_CORR_CALIB_ROW24_MSB_SPARE_R87_B49_BMSK                                        0x20000
#define HWIO_QFPROM_CORR_CALIB_ROW24_MSB_SPARE_R87_B49_SHFT                                           0x11
#define HWIO_QFPROM_CORR_CALIB_ROW24_MSB_SPARE_R87_B48_BMSK                                        0x10000
#define HWIO_QFPROM_CORR_CALIB_ROW24_MSB_SPARE_R87_B48_SHFT                                           0x10
#define HWIO_QFPROM_CORR_CALIB_ROW24_MSB_SPARE_R87_B47_BMSK                                         0x8000
#define HWIO_QFPROM_CORR_CALIB_ROW24_MSB_SPARE_R87_B47_SHFT                                            0xf
#define HWIO_QFPROM_CORR_CALIB_ROW24_MSB_SPARE_R87_B46_BMSK                                         0x4000
#define HWIO_QFPROM_CORR_CALIB_ROW24_MSB_SPARE_R87_B46_SHFT                                            0xe
#define HWIO_QFPROM_CORR_CALIB_ROW24_MSB_SPARE_R87_B45_BMSK                                         0x2000
#define HWIO_QFPROM_CORR_CALIB_ROW24_MSB_SPARE_R87_B45_SHFT                                            0xd
#define HWIO_QFPROM_CORR_CALIB_ROW24_MSB_SPARE_R87_B44_BMSK                                         0x1000
#define HWIO_QFPROM_CORR_CALIB_ROW24_MSB_SPARE_R87_B44_SHFT                                            0xc
#define HWIO_QFPROM_CORR_CALIB_ROW24_MSB_SPARE_R87_B43_BMSK                                          0x800
#define HWIO_QFPROM_CORR_CALIB_ROW24_MSB_SPARE_R87_B43_SHFT                                            0xb
#define HWIO_QFPROM_CORR_CALIB_ROW24_MSB_SPARE_R87_B42_BMSK                                          0x400
#define HWIO_QFPROM_CORR_CALIB_ROW24_MSB_SPARE_R87_B42_SHFT                                            0xa
#define HWIO_QFPROM_CORR_CALIB_ROW24_MSB_SPARE_R87_B41_BMSK                                          0x200
#define HWIO_QFPROM_CORR_CALIB_ROW24_MSB_SPARE_R87_B41_SHFT                                            0x9
#define HWIO_QFPROM_CORR_CALIB_ROW24_MSB_SPARE_R87_B40_BMSK                                          0x100
#define HWIO_QFPROM_CORR_CALIB_ROW24_MSB_SPARE_R87_B40_SHFT                                            0x8
#define HWIO_QFPROM_CORR_CALIB_ROW24_MSB_SPARE_R87_B39_BMSK                                           0x80
#define HWIO_QFPROM_CORR_CALIB_ROW24_MSB_SPARE_R87_B39_SHFT                                            0x7
#define HWIO_QFPROM_CORR_CALIB_ROW24_MSB_SPARE_R87_B38_BMSK                                           0x40
#define HWIO_QFPROM_CORR_CALIB_ROW24_MSB_SPARE_R87_B38_SHFT                                            0x6
#define HWIO_QFPROM_CORR_CALIB_ROW24_MSB_SPARE_R87_B37_BMSK                                           0x20
#define HWIO_QFPROM_CORR_CALIB_ROW24_MSB_SPARE_R87_B37_SHFT                                            0x5
#define HWIO_QFPROM_CORR_CALIB_ROW24_MSB_SPARE_R87_B36_BMSK                                           0x10
#define HWIO_QFPROM_CORR_CALIB_ROW24_MSB_SPARE_R87_B36_SHFT                                            0x4
#define HWIO_QFPROM_CORR_CALIB_ROW24_MSB_SPARE_R87_B35_BMSK                                            0x8
#define HWIO_QFPROM_CORR_CALIB_ROW24_MSB_SPARE_R87_B35_SHFT                                            0x3
#define HWIO_QFPROM_CORR_CALIB_ROW24_MSB_SPARE_R87_B34_BMSK                                            0x4
#define HWIO_QFPROM_CORR_CALIB_ROW24_MSB_SPARE_R87_B34_SHFT                                            0x2
#define HWIO_QFPROM_CORR_CALIB_ROW24_MSB_SPARE_R87_B33_BMSK                                            0x2
#define HWIO_QFPROM_CORR_CALIB_ROW24_MSB_SPARE_R87_B33_SHFT                                            0x1
#define HWIO_QFPROM_CORR_CALIB_ROW24_MSB_SPARE_R87_B32_BMSK                                            0x1
#define HWIO_QFPROM_CORR_CALIB_ROW24_MSB_SPARE_R87_B32_SHFT                                            0x0


#endif /* __HALHWIOTSENSFUSES_H__ */
