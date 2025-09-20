#ifndef __CHIPINFOHWIO_H__
#define __CHIPINFOHWIO_H__
/*
===========================================================================
*/
/**
  @file ChipInfoHWIO.h
  @brief Auto-generated HWIO interface include file.

  Reference chip release:
    SDM1000 (Poipu) [poipu_v1.0_p3q3r64_MTO]
 
  This file contains HWIO register definitions for the following modules:
    TLMM_EAST
    TCSR_TCSR_REGS
    SECURITY_CONTROL_CORE

  'Include' filters applied: TLMM_HW_REVISION_NUM[TLMM_EAST] QFPROM_CORR_PTE_ROW1_MSB[SECURITY_CONTROL_CORE] QFPROM_CORR_FEAT_CONFIG_ROW2_MSB[SECURITY_CONTROL_CORE] QFPROM_CORR_FEAT_CONFIG_ROW4_MSB[SECURITY_CONTROL_CORE] SERIAL_NUM[SECURITY_CONTROL_CORE] CHIP_ID[SECURITY_CONTROL_CORE] TCSR_SOC_HW_VERSION[TCSR_TCSR_REGS] 
  'Exclude' filters applied: DUMMY RESERVED 

  Generation parameters: 
  { u'exclude-reserved': True,
    u'filename': u'ChipInfoHWIO.h',
    u'header': u'#include "msmhwiobase.h"\n\t \n#define CHIPINFO_TLMM_HW_REVISION_NUMBER  TLMM_HW_REVISION_NUMBER\n#define CHIPINFO_TCSR_SOC_HW_VERSION      TCSR_SOC_HW_VERSION\n\n#define CHIPINFO_FOUNDRY_ID_REG           QFPROM_CORR_FEAT_CONFIG_ROW4_MSB\n#define CHIPINFO_SERIAL_NUM_REG           SERIAL_NUM\n#define CHIPINFO_QFPROM_CHIP_ID_REG       CHIP_ID',
    u'module-filter-exclude': { },
    u'module-filter-include': { u'SECURITY_CONTROL_CORE': [ u'QFPROM_CORR_PTE_ROW1_MSB',
                                                            u'QFPROM_CORR_FEAT_CONFIG_ROW2_MSB',
                                                            u'QFPROM_CORR_FEAT_CONFIG_ROW4_MSB',
                                                            u'SERIAL_NUM',
                                                            u'CHIP_ID'],
                                u'TCSR_TCSR_REGS': [u'TCSR_SOC_HW_VERSION'],
                                u'TLMM_EAST': [u'TLMM_HW_REVISION_NUM']},
    u'modules': [u'TLMM_EAST', u'TCSR_TCSR_REGS', u'SECURITY_CONTROL_CORE']}
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
	 
#define CHIPINFO_TLMM_HW_REVISION_NUMBER  TLMM_HW_REVISION_NUMBER
#define CHIPINFO_TCSR_SOC_HW_VERSION      TCSR_SOC_HW_VERSION

#define CHIPINFO_FOUNDRY_ID_REG           QFPROM_CORR_FEAT_CONFIG_ROW4_MSB
#define CHIPINFO_SERIAL_NUM_REG           SERIAL_NUM
#define CHIPINFO_QFPROM_CHIP_ID_REG       CHIP_ID

/*----------------------------------------------------------------------------
 * MODULE: TLMM_EAST
 *--------------------------------------------------------------------------*/

#define TLMM_EAST_REG_BASE                                                 (TLMM_EAST_BASE      + 0x00000000)

#define HWIO_TLMM_HW_REVISION_NUMBER_ADDR                                  (TLMM_EAST_REG_BASE      + 0x000c0010)
#define HWIO_TLMM_HW_REVISION_NUMBER_RMSK                                  0xffffffff
#define HWIO_TLMM_HW_REVISION_NUMBER_IN          \
        in_dword_masked(HWIO_TLMM_HW_REVISION_NUMBER_ADDR, HWIO_TLMM_HW_REVISION_NUMBER_RMSK)
#define HWIO_TLMM_HW_REVISION_NUMBER_INM(m)      \
        in_dword_masked(HWIO_TLMM_HW_REVISION_NUMBER_ADDR, m)
#define HWIO_TLMM_HW_REVISION_NUMBER_VERSION_ID_BMSK                       0xf0000000
#define HWIO_TLMM_HW_REVISION_NUMBER_VERSION_ID_SHFT                             0x1c
#define HWIO_TLMM_HW_REVISION_NUMBER_PRODUCT_DEVICE_ID_BMSK                 0xffff000
#define HWIO_TLMM_HW_REVISION_NUMBER_PRODUCT_DEVICE_ID_SHFT                       0xc
#define HWIO_TLMM_HW_REVISION_NUMBER_QUALCOMM_MFG_ID_BMSK                       0xffe
#define HWIO_TLMM_HW_REVISION_NUMBER_QUALCOMM_MFG_ID_SHFT                         0x1
#define HWIO_TLMM_HW_REVISION_NUMBER_START_BIT_BMSK                               0x1
#define HWIO_TLMM_HW_REVISION_NUMBER_START_BIT_SHFT                               0x0

/*----------------------------------------------------------------------------
 * MODULE: TCSR_TCSR_REGS
 *--------------------------------------------------------------------------*/

#define TCSR_TCSR_REGS_REG_BASE                                                                                      (CORE_TOP_CSR_BASE      + 0x000c0000)

#define HWIO_TCSR_SOC_HW_VERSION_ADDR                                                                                (TCSR_TCSR_REGS_REG_BASE      + 0x00008000)
#define HWIO_TCSR_SOC_HW_VERSION_RMSK                                                                                0xffffffff
#define HWIO_TCSR_SOC_HW_VERSION_IN          \
        in_dword_masked(HWIO_TCSR_SOC_HW_VERSION_ADDR, HWIO_TCSR_SOC_HW_VERSION_RMSK)
#define HWIO_TCSR_SOC_HW_VERSION_INM(m)      \
        in_dword_masked(HWIO_TCSR_SOC_HW_VERSION_ADDR, m)
#define HWIO_TCSR_SOC_HW_VERSION_FAMILY_NUMBER_BMSK                                                                  0xf0000000
#define HWIO_TCSR_SOC_HW_VERSION_FAMILY_NUMBER_SHFT                                                                        0x1c
#define HWIO_TCSR_SOC_HW_VERSION_DEVICE_NUMBER_BMSK                                                                   0xfff0000
#define HWIO_TCSR_SOC_HW_VERSION_DEVICE_NUMBER_SHFT                                                                        0x10
#define HWIO_TCSR_SOC_HW_VERSION_MAJOR_VERSION_BMSK                                                                      0xff00
#define HWIO_TCSR_SOC_HW_VERSION_MAJOR_VERSION_SHFT                                                                         0x8
#define HWIO_TCSR_SOC_HW_VERSION_MINOR_VERSION_BMSK                                                                        0xff
#define HWIO_TCSR_SOC_HW_VERSION_MINOR_VERSION_SHFT                                                                         0x0

/*----------------------------------------------------------------------------
 * MODULE: SECURITY_CONTROL_CORE
 *--------------------------------------------------------------------------*/

#define SECURITY_CONTROL_CORE_REG_BASE                                                          (SECURITY_CONTROL_BASE      + 0x00000000)

#define HWIO_QFPROM_CORR_PTE_ROW1_MSB_ADDR                                                      (SECURITY_CONTROL_CORE_REG_BASE      + 0x0000413c)
#define HWIO_QFPROM_CORR_PTE_ROW1_MSB_RMSK                                                      0xffffffff
#define HWIO_QFPROM_CORR_PTE_ROW1_MSB_IN          \
        in_dword_masked(HWIO_QFPROM_CORR_PTE_ROW1_MSB_ADDR, HWIO_QFPROM_CORR_PTE_ROW1_MSB_RMSK)
#define HWIO_QFPROM_CORR_PTE_ROW1_MSB_INM(m)      \
        in_dword_masked(HWIO_QFPROM_CORR_PTE_ROW1_MSB_ADDR, m)
#define HWIO_QFPROM_CORR_PTE_ROW1_MSB_PTE_DATA1_BMSK                                            0xffffffff
#define HWIO_QFPROM_CORR_PTE_ROW1_MSB_PTE_DATA1_SHFT                                                   0x0

#define HWIO_QFPROM_CORR_FEAT_CONFIG_ROW2_MSB_ADDR                                              (SECURITY_CONTROL_CORE_REG_BASE      + 0x000041b4)
#define HWIO_QFPROM_CORR_FEAT_CONFIG_ROW2_MSB_RMSK                                              0xffffffff
#define HWIO_QFPROM_CORR_FEAT_CONFIG_ROW2_MSB_IN          \
        in_dword_masked(HWIO_QFPROM_CORR_FEAT_CONFIG_ROW2_MSB_ADDR, HWIO_QFPROM_CORR_FEAT_CONFIG_ROW2_MSB_RMSK)
#define HWIO_QFPROM_CORR_FEAT_CONFIG_ROW2_MSB_INM(m)      \
        in_dword_masked(HWIO_QFPROM_CORR_FEAT_CONFIG_ROW2_MSB_ADDR, m)
#define HWIO_QFPROM_CORR_FEAT_CONFIG_ROW2_MSB_SYS_CFG_APC1PLL_LVAL_BMSK                         0xff000000
#define HWIO_QFPROM_CORR_FEAT_CONFIG_ROW2_MSB_SYS_CFG_APC1PLL_LVAL_SHFT                               0x18
#define HWIO_QFPROM_CORR_FEAT_CONFIG_ROW2_MSB_EDP_EFUSE_DISABLE_BMSK                              0x800000
#define HWIO_QFPROM_CORR_FEAT_CONFIG_ROW2_MSB_EDP_EFUSE_DISABLE_SHFT                                  0x17
#define HWIO_QFPROM_CORR_FEAT_CONFIG_ROW2_MSB_SYS_CFG_L3_SIZE_RED_BMSK                            0x400000
#define HWIO_QFPROM_CORR_FEAT_CONFIG_ROW2_MSB_SYS_CFG_L3_SIZE_RED_SHFT                                0x16
#define HWIO_QFPROM_CORR_FEAT_CONFIG_ROW2_MSB_AUTO_CCI_RCG_CFG_DISABLE_BMSK                       0x200000
#define HWIO_QFPROM_CORR_FEAT_CONFIG_ROW2_MSB_AUTO_CCI_RCG_CFG_DISABLE_SHFT                           0x15
#define HWIO_QFPROM_CORR_FEAT_CONFIG_ROW2_MSB_APPS_BOOT_FSM_FUSE_BMSK                             0x1f8000
#define HWIO_QFPROM_CORR_FEAT_CONFIG_ROW2_MSB_APPS_BOOT_FSM_FUSE_SHFT                                  0xf
#define HWIO_QFPROM_CORR_FEAT_CONFIG_ROW2_MSB_SYS_APCSCFGAPMBOOTONMX_BMSK                           0x4000
#define HWIO_QFPROM_CORR_FEAT_CONFIG_ROW2_MSB_SYS_APCSCFGAPMBOOTONMX_SHFT                              0xe
#define HWIO_QFPROM_CORR_FEAT_CONFIG_ROW2_MSB_SYS_APCCCFGCPUPRESENT_N_BMSK                          0x3fc0
#define HWIO_QFPROM_CORR_FEAT_CONFIG_ROW2_MSB_SYS_APCCCFGCPUPRESENT_N_SHFT                             0x6
#define HWIO_QFPROM_CORR_FEAT_CONFIG_ROW2_MSB_TURING_Q6SS_PLL_L_MAX_15_10_BMSK                        0x3f
#define HWIO_QFPROM_CORR_FEAT_CONFIG_ROW2_MSB_TURING_Q6SS_PLL_L_MAX_15_10_SHFT                         0x0

#define HWIO_QFPROM_CORR_FEAT_CONFIG_ROW4_MSB_ADDR                                              (SECURITY_CONTROL_CORE_REG_BASE      + 0x000041c4)
#define HWIO_QFPROM_CORR_FEAT_CONFIG_ROW4_MSB_RMSK                                              0xffffffff
#define HWIO_QFPROM_CORR_FEAT_CONFIG_ROW4_MSB_IN          \
        in_dword_masked(HWIO_QFPROM_CORR_FEAT_CONFIG_ROW4_MSB_ADDR, HWIO_QFPROM_CORR_FEAT_CONFIG_ROW4_MSB_RMSK)
#define HWIO_QFPROM_CORR_FEAT_CONFIG_ROW4_MSB_INM(m)      \
        in_dword_masked(HWIO_QFPROM_CORR_FEAT_CONFIG_ROW4_MSB_ADDR, m)
#define HWIO_QFPROM_CORR_FEAT_CONFIG_ROW4_MSB_APPS_BOOT_TRIGGER_DISABLE_BMSK                    0x80000000
#define HWIO_QFPROM_CORR_FEAT_CONFIG_ROW4_MSB_APPS_BOOT_TRIGGER_DISABLE_SHFT                          0x1f
#define HWIO_QFPROM_CORR_FEAT_CONFIG_ROW4_MSB_PBL_QSEE_BOOT_FLOW_DISABLE_BMSK                   0x40000000
#define HWIO_QFPROM_CORR_FEAT_CONFIG_ROW4_MSB_PBL_QSEE_BOOT_FLOW_DISABLE_SHFT                         0x1e
#define HWIO_QFPROM_CORR_FEAT_CONFIG_ROW4_MSB_XBL_SEC_AUTH_DISABLE_BMSK                         0x20000000
#define HWIO_QFPROM_CORR_FEAT_CONFIG_ROW4_MSB_XBL_SEC_AUTH_DISABLE_SHFT                               0x1d
#define HWIO_QFPROM_CORR_FEAT_CONFIG_ROW4_MSB_MSM_PKG_TYPE_BMSK                                 0x10000000
#define HWIO_QFPROM_CORR_FEAT_CONFIG_ROW4_MSB_MSM_PKG_TYPE_SHFT                                       0x1c
#define HWIO_QFPROM_CORR_FEAT_CONFIG_ROW4_MSB_SPSS_CONFIG_MODE_BMSK                              0xc000000
#define HWIO_QFPROM_CORR_FEAT_CONFIG_ROW4_MSB_SPSS_CONFIG_MODE_SHFT                                   0x1a
#define HWIO_QFPROM_CORR_FEAT_CONFIG_ROW4_MSB_PERIPH_DRV_STRENGTH_SETTING_BMSK                   0x3800000
#define HWIO_QFPROM_CORR_FEAT_CONFIG_ROW4_MSB_PERIPH_DRV_STRENGTH_SETTING_SHFT                        0x17
#define HWIO_QFPROM_CORR_FEAT_CONFIG_ROW4_MSB_RSVD3_BMSK                                          0x400000
#define HWIO_QFPROM_CORR_FEAT_CONFIG_ROW4_MSB_RSVD3_SHFT                                              0x16
#define HWIO_QFPROM_CORR_FEAT_CONFIG_ROW4_MSB_FOUNDRY_ID_BMSK                                     0x3c0000
#define HWIO_QFPROM_CORR_FEAT_CONFIG_ROW4_MSB_FOUNDRY_ID_SHFT                                         0x12
#define HWIO_QFPROM_CORR_FEAT_CONFIG_ROW4_MSB_PLL_CFG_BMSK                                         0x3fff0
#define HWIO_QFPROM_CORR_FEAT_CONFIG_ROW4_MSB_PLL_CFG_SHFT                                             0x4
#define HWIO_QFPROM_CORR_FEAT_CONFIG_ROW4_MSB_APPS_PBL_PLL_CTRL_BMSK                                   0xf
#define HWIO_QFPROM_CORR_FEAT_CONFIG_ROW4_MSB_APPS_PBL_PLL_CTRL_SHFT                                   0x0

#define HWIO_SERIAL_NUM_ADDR                                                                    (SECURITY_CONTROL_CORE_REG_BASE      + 0x00006134)
#define HWIO_SERIAL_NUM_RMSK                                                                    0xffffffff
#define HWIO_SERIAL_NUM_IN          \
        in_dword_masked(HWIO_SERIAL_NUM_ADDR, HWIO_SERIAL_NUM_RMSK)
#define HWIO_SERIAL_NUM_INM(m)      \
        in_dword_masked(HWIO_SERIAL_NUM_ADDR, m)
#define HWIO_SERIAL_NUM_SERIAL_NUM_BMSK                                                         0xffffffff
#define HWIO_SERIAL_NUM_SERIAL_NUM_SHFT                                                                0x0

#define HWIO_CHIP_ID_ADDR                                                                       (SECURITY_CONTROL_CORE_REG_BASE      + 0x00006144)
#define HWIO_CHIP_ID_RMSK                                                                           0xffff
#define HWIO_CHIP_ID_IN          \
        in_dword_masked(HWIO_CHIP_ID_ADDR, HWIO_CHIP_ID_RMSK)
#define HWIO_CHIP_ID_INM(m)      \
        in_dword_masked(HWIO_CHIP_ID_ADDR, m)
#define HWIO_CHIP_ID_CHIP_ID_BMSK                                                                   0xffff
#define HWIO_CHIP_ID_CHIP_ID_SHFT                                                                      0x0


#endif /* __CHIPINFOHWIO_H__ */
