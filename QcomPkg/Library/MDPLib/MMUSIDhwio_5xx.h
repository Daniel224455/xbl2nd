#ifndef __MMUSIDHWIO_5XX_H__
#define __MMUSIDHWIO_5XX_H__
/*
===========================================================================
*/
/**
  @file MMUSIDhwio_5xx.h
  @brief Auto-generated HWIO interface include file.

  This file contains HWIO register definitions for the following modules:
    APPS_MMU500_SMMU_APP

  'Include' filters applied: CB0_SCTLR SMR[0-2]$ S2CR[0-2]$ CBAR0 CBA2R0 
  'Exclude' filters applied: RESERVED DUMMY 
*/
/*
  ===========================================================================

  Copyright (c) 2017 Qualcomm Technologies Incorporated.
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

/* HWIO base definitions */
#define SMMU_SS_APP_TCU_REG_BASE         0x15000000
#define SMMU_500_APPS_REG_WRAPPER_BASE   SMMU_SS_APP_TCU_REG_BASE


/*----------------------------------------------------------------------------
 * MODULE: APPS_MMU500_SMMU_APP
 *--------------------------------------------------------------------------*/

#define APPS_MMU500_SMMU_APP_REG_BASE                                                   (SMMU_500_APPS_REG_WRAPPER_BASE      + 0x00000000)
#define APPS_MMU500_SMMU_APP_REG_BASE_PHYS                                              (SMMU_500_APPS_REG_WRAPPER_BASE_PHYS + 0x00000000)
#define APPS_MMU500_SMMU_APP_REG_BASE_OFFS                                              0x00000000

#define HWIO_APPS_SMMU_SMR0_ADDR                                                        (APPS_MMU500_SMMU_APP_REG_BASE      + 0x00000800)
#define HWIO_APPS_SMMU_SMR0_PHYS                                                        (APPS_MMU500_SMMU_APP_REG_BASE_PHYS + 0x00000800)
#define HWIO_APPS_SMMU_SMR0_OFFS                                                        (APPS_MMU500_SMMU_APP_REG_BASE_OFFS + 0x00000800)
#define HWIO_APPS_SMMU_SMR0_RMSK                                                        0xffff7fff
#define HWIO_APPS_SMMU_SMR0_IN          \
        in_dword_masked(HWIO_APPS_SMMU_SMR0_ADDR, HWIO_APPS_SMMU_SMR0_RMSK)
#define HWIO_APPS_SMMU_SMR0_INM(m)      \
        in_dword_masked(HWIO_APPS_SMMU_SMR0_ADDR, m)
#define HWIO_APPS_SMMU_SMR0_OUT(v)      \
        out_dword(HWIO_APPS_SMMU_SMR0_ADDR,v)
#define HWIO_APPS_SMMU_SMR0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_APPS_SMMU_SMR0_ADDR,m,v,HWIO_APPS_SMMU_SMR0_IN)

#define HWIO_APPS_SMMU_SMR1_ADDR                                                        (APPS_MMU500_SMMU_APP_REG_BASE      + 0x00000804)
#define HWIO_APPS_SMMU_SMR1_PHYS                                                        (APPS_MMU500_SMMU_APP_REG_BASE_PHYS + 0x00000804)
#define HWIO_APPS_SMMU_SMR1_OFFS                                                        (APPS_MMU500_SMMU_APP_REG_BASE_OFFS + 0x00000804)
#define HWIO_APPS_SMMU_SMR1_RMSK                                                        0xffff7fff
#define HWIO_APPS_SMMU_SMR1_IN          \
        in_dword_masked(HWIO_APPS_SMMU_SMR1_ADDR, HWIO_APPS_SMMU_SMR1_RMSK)
#define HWIO_APPS_SMMU_SMR1_INM(m)      \
        in_dword_masked(HWIO_APPS_SMMU_SMR1_ADDR, m)
#define HWIO_APPS_SMMU_SMR1_OUT(v)      \
        out_dword(HWIO_APPS_SMMU_SMR1_ADDR,v)
#define HWIO_APPS_SMMU_SMR1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_APPS_SMMU_SMR1_ADDR,m,v,HWIO_APPS_SMMU_SMR1_IN)

#define HWIO_APPS_SMMU_SMR2_ADDR                                                        (APPS_MMU500_SMMU_APP_REG_BASE      + 0x00000808)
#define HWIO_APPS_SMMU_SMR2_PHYS                                                        (APPS_MMU500_SMMU_APP_REG_BASE_PHYS + 0x00000808)
#define HWIO_APPS_SMMU_SMR2_OFFS                                                        (APPS_MMU500_SMMU_APP_REG_BASE_OFFS + 0x00000808)
#define HWIO_APPS_SMMU_SMR2_RMSK                                                        0xffff7fff
#define HWIO_APPS_SMMU_SMR2_IN          \
        in_dword_masked(HWIO_APPS_SMMU_SMR2_ADDR, HWIO_APPS_SMMU_SMR2_RMSK)
#define HWIO_APPS_SMMU_SMR2_INM(m)      \
        in_dword_masked(HWIO_APPS_SMMU_SMR2_ADDR, m)
#define HWIO_APPS_SMMU_SMR2_OUT(v)      \
        out_dword(HWIO_APPS_SMMU_SMR2_ADDR,v)
#define HWIO_APPS_SMMU_SMR2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_APPS_SMMU_SMR2_ADDR,m,v,HWIO_APPS_SMMU_SMR2_IN)

#define HWIO_APPS_SMMU_S2CR0_ADDR                                                       (APPS_MMU500_SMMU_APP_REG_BASE      + 0x00000c00)
#define HWIO_APPS_SMMU_S2CR0_PHYS                                                       (APPS_MMU500_SMMU_APP_REG_BASE_PHYS + 0x00000c00)
#define HWIO_APPS_SMMU_S2CR0_OFFS                                                       (APPS_MMU500_SMMU_APP_REG_BASE_OFFS + 0x00000c00)
#define HWIO_APPS_SMMU_S2CR0_RMSK                                                       0x3ffffbff
#define HWIO_APPS_SMMU_S2CR0_IN          \
        in_dword_masked(HWIO_APPS_SMMU_S2CR0_ADDR, HWIO_APPS_SMMU_S2CR0_RMSK)
#define HWIO_APPS_SMMU_S2CR0_INM(m)      \
        in_dword_masked(HWIO_APPS_SMMU_S2CR0_ADDR, m)
#define HWIO_APPS_SMMU_S2CR0_OUT(v)      \
        out_dword(HWIO_APPS_SMMU_S2CR0_ADDR,v)
#define HWIO_APPS_SMMU_S2CR0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_APPS_SMMU_S2CR0_ADDR,m,v,HWIO_APPS_SMMU_S2CR0_IN)

#define HWIO_APPS_SMMU_S2CR1_ADDR                                                       (APPS_MMU500_SMMU_APP_REG_BASE      + 0x00000c04)
#define HWIO_APPS_SMMU_S2CR1_PHYS                                                       (APPS_MMU500_SMMU_APP_REG_BASE_PHYS + 0x00000c04)
#define HWIO_APPS_SMMU_S2CR1_OFFS                                                       (APPS_MMU500_SMMU_APP_REG_BASE_OFFS + 0x00000c04)
#define HWIO_APPS_SMMU_S2CR1_RMSK                                                       0x3ffffbff
#define HWIO_APPS_SMMU_S2CR1_IN          \
        in_dword_masked(HWIO_APPS_SMMU_S2CR1_ADDR, HWIO_APPS_SMMU_S2CR1_RMSK)
#define HWIO_APPS_SMMU_S2CR1_INM(m)      \
        in_dword_masked(HWIO_APPS_SMMU_S2CR1_ADDR, m)
#define HWIO_APPS_SMMU_S2CR1_OUT(v)      \
        out_dword(HWIO_APPS_SMMU_S2CR1_ADDR,v)
#define HWIO_APPS_SMMU_S2CR1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_APPS_SMMU_S2CR1_ADDR,m,v,HWIO_APPS_SMMU_S2CR1_IN)

#define HWIO_APPS_SMMU_S2CR2_ADDR                                                       (APPS_MMU500_SMMU_APP_REG_BASE      + 0x00000c08)
#define HWIO_APPS_SMMU_S2CR2_PHYS                                                       (APPS_MMU500_SMMU_APP_REG_BASE_PHYS + 0x00000c08)
#define HWIO_APPS_SMMU_S2CR2_OFFS                                                       (APPS_MMU500_SMMU_APP_REG_BASE_OFFS + 0x00000c08)
#define HWIO_APPS_SMMU_S2CR2_RMSK                                                       0x3ffffbff
#define HWIO_APPS_SMMU_S2CR2_IN          \
        in_dword_masked(HWIO_APPS_SMMU_S2CR2_ADDR, HWIO_APPS_SMMU_S2CR2_RMSK)
#define HWIO_APPS_SMMU_S2CR2_INM(m)      \
        in_dword_masked(HWIO_APPS_SMMU_S2CR2_ADDR, m)
#define HWIO_APPS_SMMU_S2CR2_OUT(v)      \
        out_dword(HWIO_APPS_SMMU_S2CR2_ADDR,v)
#define HWIO_APPS_SMMU_S2CR2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_APPS_SMMU_S2CR2_ADDR,m,v,HWIO_APPS_SMMU_S2CR2_IN)

#define HWIO_APPS_SMMU_CBAR0_ADDR                                                       (APPS_MMU500_SMMU_APP_REG_BASE      + 0x00001000)
#define HWIO_APPS_SMMU_CBAR0_PHYS                                                       (APPS_MMU500_SMMU_APP_REG_BASE_PHYS + 0x00001000)
#define HWIO_APPS_SMMU_CBAR0_OFFS                                                       (APPS_MMU500_SMMU_APP_REG_BASE_OFFS + 0x00001000)
#define HWIO_APPS_SMMU_CBAR0_RMSK                                                       0xffffffff
#define HWIO_APPS_SMMU_CBAR0_IN          \
        in_dword_masked(HWIO_APPS_SMMU_CBAR0_ADDR, HWIO_APPS_SMMU_CBAR0_RMSK)
#define HWIO_APPS_SMMU_CBAR0_INM(m)      \
        in_dword_masked(HWIO_APPS_SMMU_CBAR0_ADDR, m)
#define HWIO_APPS_SMMU_CBAR0_OUT(v)      \
        out_dword(HWIO_APPS_SMMU_CBAR0_ADDR,v)
#define HWIO_APPS_SMMU_CBAR0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_APPS_SMMU_CBAR0_ADDR,m,v,HWIO_APPS_SMMU_CBAR0_IN)
#define HWIO_APPS_SMMU_CBAR0_IRPTNDX_BMSK                                               0xff000000
#define HWIO_APPS_SMMU_CBAR0_IRPTNDX_SHFT                                                     0x18
#define HWIO_APPS_SMMU_CBAR0_WACFG_BMSK                                                   0xc00000
#define HWIO_APPS_SMMU_CBAR0_WACFG_SHFT                                                       0x16
#define HWIO_APPS_SMMU_CBAR0_RACFG_BMSK                                                   0x300000
#define HWIO_APPS_SMMU_CBAR0_RACFG_SHFT                                                       0x14
#define HWIO_APPS_SMMU_CBAR0_BSU_BMSK                                                      0xc0000
#define HWIO_APPS_SMMU_CBAR0_BSU_SHFT                                                         0x12
#define HWIO_APPS_SMMU_CBAR0_TYPE_BMSK                                                     0x30000
#define HWIO_APPS_SMMU_CBAR0_TYPE_SHFT                                                        0x10
#define HWIO_APPS_SMMU_CBAR0_MEMATTR_CBNDX_7_4_BMSK                                         0xf000
#define HWIO_APPS_SMMU_CBAR0_MEMATTR_CBNDX_7_4_SHFT                                            0xc
#define HWIO_APPS_SMMU_CBAR0_FB_CBNDX_3_BMSK                                                 0x800
#define HWIO_APPS_SMMU_CBAR0_FB_CBNDX_3_SHFT                                                   0xb
#define HWIO_APPS_SMMU_CBAR0_HYPC_CBNDX_2_BMSK                                               0x400
#define HWIO_APPS_SMMU_CBAR0_HYPC_CBNDX_2_SHFT                                                 0xa
#define HWIO_APPS_SMMU_CBAR0_BPSHCFG_CBNDX_1_0_BMSK                                          0x300
#define HWIO_APPS_SMMU_CBAR0_BPSHCFG_CBNDX_1_0_SHFT                                            0x8
#define HWIO_APPS_SMMU_CBAR0_VMID_BMSK                                                        0xff
#define HWIO_APPS_SMMU_CBAR0_VMID_SHFT                                                         0x0

#define HWIO_APPS_SMMU_CBA2R0_ADDR                                                      (APPS_MMU500_SMMU_APP_REG_BASE      + 0x00001800)
#define HWIO_APPS_SMMU_CBA2R0_PHYS                                                      (APPS_MMU500_SMMU_APP_REG_BASE_PHYS + 0x00001800)
#define HWIO_APPS_SMMU_CBA2R0_OFFS                                                      (APPS_MMU500_SMMU_APP_REG_BASE_OFFS + 0x00001800)
#define HWIO_APPS_SMMU_CBA2R0_RMSK                                                             0x3
#define HWIO_APPS_SMMU_CBA2R0_IN          \
        in_dword_masked(HWIO_APPS_SMMU_CBA2R0_ADDR, HWIO_APPS_SMMU_CBA2R0_RMSK)
#define HWIO_APPS_SMMU_CBA2R0_INM(m)      \
        in_dword_masked(HWIO_APPS_SMMU_CBA2R0_ADDR, m)
#define HWIO_APPS_SMMU_CBA2R0_OUT(v)      \
        out_dword(HWIO_APPS_SMMU_CBA2R0_ADDR,v)
#define HWIO_APPS_SMMU_CBA2R0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_APPS_SMMU_CBA2R0_ADDR,m,v,HWIO_APPS_SMMU_CBA2R0_IN)
#define HWIO_APPS_SMMU_CBA2R0_MONC_BMSK                                                        0x2
#define HWIO_APPS_SMMU_CBA2R0_MONC_SHFT                                                        0x1
#define HWIO_APPS_SMMU_CBA2R0_VA64_BMSK                                                        0x1
#define HWIO_APPS_SMMU_CBA2R0_VA64_SHFT                                                        0x0

#define HWIO_APPS_PMCGSMR0_ADDR                                                         (APPS_MMU500_SMMU_APP_REG_BASE      + 0x00003a00)
#define HWIO_APPS_PMCGSMR0_PHYS                                                         (APPS_MMU500_SMMU_APP_REG_BASE_PHYS + 0x00003a00)
#define HWIO_APPS_PMCGSMR0_OFFS                                                         (APPS_MMU500_SMMU_APP_REG_BASE_OFFS + 0x00003a00)
#define HWIO_APPS_PMCGSMR0_RMSK                                                          0x3ff03ff
#define HWIO_APPS_PMCGSMR0_IN          \
        in_dword_masked(HWIO_APPS_PMCGSMR0_ADDR, HWIO_APPS_PMCGSMR0_RMSK)
#define HWIO_APPS_PMCGSMR0_INM(m)      \
        in_dword_masked(HWIO_APPS_PMCGSMR0_ADDR, m)
#define HWIO_APPS_PMCGSMR0_OUT(v)      \
        out_dword(HWIO_APPS_PMCGSMR0_ADDR,v)
#define HWIO_APPS_PMCGSMR0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_APPS_PMCGSMR0_ADDR,m,v,HWIO_APPS_PMCGSMR0_IN)

#define HWIO_APPS_PMCGSMR1_ADDR                                                         (APPS_MMU500_SMMU_APP_REG_BASE      + 0x00003a04)
#define HWIO_APPS_PMCGSMR1_PHYS                                                         (APPS_MMU500_SMMU_APP_REG_BASE_PHYS + 0x00003a04)
#define HWIO_APPS_PMCGSMR1_OFFS                                                         (APPS_MMU500_SMMU_APP_REG_BASE_OFFS + 0x00003a04)
#define HWIO_APPS_PMCGSMR1_RMSK                                                          0x3ff03ff
#define HWIO_APPS_PMCGSMR1_IN          \
        in_dword_masked(HWIO_APPS_PMCGSMR1_ADDR, HWIO_APPS_PMCGSMR1_RMSK)
#define HWIO_APPS_PMCGSMR1_INM(m)      \
        in_dword_masked(HWIO_APPS_PMCGSMR1_ADDR, m)
#define HWIO_APPS_PMCGSMR1_OUT(v)      \
        out_dword(HWIO_APPS_PMCGSMR1_ADDR,v)
#define HWIO_APPS_PMCGSMR1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_APPS_PMCGSMR1_ADDR,m,v,HWIO_APPS_PMCGSMR1_IN)

#define HWIO_APPS_PMCGSMR2_ADDR                                                         (APPS_MMU500_SMMU_APP_REG_BASE      + 0x00003a08)
#define HWIO_APPS_PMCGSMR2_PHYS                                                         (APPS_MMU500_SMMU_APP_REG_BASE_PHYS + 0x00003a08)
#define HWIO_APPS_PMCGSMR2_OFFS                                                         (APPS_MMU500_SMMU_APP_REG_BASE_OFFS + 0x00003a08)
#define HWIO_APPS_PMCGSMR2_RMSK                                                          0x3ff03ff
#define HWIO_APPS_PMCGSMR2_IN          \
        in_dword_masked(HWIO_APPS_PMCGSMR2_ADDR, HWIO_APPS_PMCGSMR2_RMSK)
#define HWIO_APPS_PMCGSMR2_INM(m)      \
        in_dword_masked(HWIO_APPS_PMCGSMR2_ADDR, m)
#define HWIO_APPS_PMCGSMR2_OUT(v)      \
        out_dword(HWIO_APPS_PMCGSMR2_ADDR,v)
#define HWIO_APPS_PMCGSMR2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_APPS_PMCGSMR2_ADDR,m,v,HWIO_APPS_PMCGSMR2_IN)

#define HWIO_APPS_SMMU_CB0_SCTLR_ADDR                                                   (APPS_MMU500_SMMU_APP_REG_BASE      + 0x00080000)
#define HWIO_APPS_SMMU_CB0_SCTLR_PHYS                                                   (APPS_MMU500_SMMU_APP_REG_BASE_PHYS + 0x00080000)
#define HWIO_APPS_SMMU_CB0_SCTLR_OFFS                                                   (APPS_MMU500_SMMU_APP_REG_BASE_OFFS + 0x00080000)
#define HWIO_APPS_SMMU_CB0_SCTLR_RMSK                                                   0x3ffff7ff
#define HWIO_APPS_SMMU_CB0_SCTLR_IN          \
        in_dword_masked(HWIO_APPS_SMMU_CB0_SCTLR_ADDR, HWIO_APPS_SMMU_CB0_SCTLR_RMSK)
#define HWIO_APPS_SMMU_CB0_SCTLR_INM(m)      \
        in_dword_masked(HWIO_APPS_SMMU_CB0_SCTLR_ADDR, m)
#define HWIO_APPS_SMMU_CB0_SCTLR_OUT(v)      \
        out_dword(HWIO_APPS_SMMU_CB0_SCTLR_ADDR,v)
#define HWIO_APPS_SMMU_CB0_SCTLR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_APPS_SMMU_CB0_SCTLR_ADDR,m,v,HWIO_APPS_SMMU_CB0_SCTLR_IN)
#define HWIO_APPS_SMMU_CB0_SCTLR_NSCFG_BMSK                                             0x30000000
#define HWIO_APPS_SMMU_CB0_SCTLR_NSCFG_SHFT                                                   0x1c
#define HWIO_APPS_SMMU_CB0_SCTLR_WACFG_BMSK                                              0xc000000
#define HWIO_APPS_SMMU_CB0_SCTLR_WACFG_SHFT                                                   0x1a
#define HWIO_APPS_SMMU_CB0_SCTLR_RACFG_BMSK                                              0x3000000
#define HWIO_APPS_SMMU_CB0_SCTLR_RACFG_SHFT                                                   0x18
#define HWIO_APPS_SMMU_CB0_SCTLR_SHCFG_BMSK                                               0xc00000
#define HWIO_APPS_SMMU_CB0_SCTLR_SHCFG_SHFT                                                   0x16
#define HWIO_APPS_SMMU_CB0_SCTLR_FB_BMSK                                                  0x200000
#define HWIO_APPS_SMMU_CB0_SCTLR_FB_SHFT                                                      0x15
#define HWIO_APPS_SMMU_CB0_SCTLR_MTCFG_BMSK                                               0x100000
#define HWIO_APPS_SMMU_CB0_SCTLR_MTCFG_SHFT                                                   0x14
#define HWIO_APPS_SMMU_CB0_SCTLR_MEMATTR_BMSK                                              0xf0000
#define HWIO_APPS_SMMU_CB0_SCTLR_MEMATTR_SHFT                                                 0x10
#define HWIO_APPS_SMMU_CB0_SCTLR_TRANSIENTCFG_BMSK                                          0xc000
#define HWIO_APPS_SMMU_CB0_SCTLR_TRANSIENTCFG_SHFT                                             0xe
#define HWIO_APPS_SMMU_CB0_SCTLR_PTW_BMSK                                                   0x2000
#define HWIO_APPS_SMMU_CB0_SCTLR_PTW_SHFT                                                      0xd
#define HWIO_APPS_SMMU_CB0_SCTLR_ASIDPNE_BMSK                                               0x1000
#define HWIO_APPS_SMMU_CB0_SCTLR_ASIDPNE_SHFT                                                  0xc
#define HWIO_APPS_SMMU_CB0_SCTLR_UWXN_BMSK                                                   0x400
#define HWIO_APPS_SMMU_CB0_SCTLR_UWXN_SHFT                                                     0xa
#define HWIO_APPS_SMMU_CB0_SCTLR_WXN_BMSK                                                    0x200
#define HWIO_APPS_SMMU_CB0_SCTLR_WXN_SHFT                                                      0x9
#define HWIO_APPS_SMMU_CB0_SCTLR_HUPCF_BMSK                                                  0x100
#define HWIO_APPS_SMMU_CB0_SCTLR_HUPCF_SHFT                                                    0x8
#define HWIO_APPS_SMMU_CB0_SCTLR_CFCFG_BMSK                                                   0x80
#define HWIO_APPS_SMMU_CB0_SCTLR_CFCFG_SHFT                                                    0x7
#define HWIO_APPS_SMMU_CB0_SCTLR_CFIE_BMSK                                                    0x40
#define HWIO_APPS_SMMU_CB0_SCTLR_CFIE_SHFT                                                     0x6
#define HWIO_APPS_SMMU_CB0_SCTLR_CFRE_BMSK                                                    0x20
#define HWIO_APPS_SMMU_CB0_SCTLR_CFRE_SHFT                                                     0x5
#define HWIO_APPS_SMMU_CB0_SCTLR_E_BMSK                                                       0x10
#define HWIO_APPS_SMMU_CB0_SCTLR_E_SHFT                                                        0x4
#define HWIO_APPS_SMMU_CB0_SCTLR_AFFD_BMSK                                                     0x8
#define HWIO_APPS_SMMU_CB0_SCTLR_AFFD_SHFT                                                     0x3
#define HWIO_APPS_SMMU_CB0_SCTLR_AFE_BMSK                                                      0x4
#define HWIO_APPS_SMMU_CB0_SCTLR_AFE_SHFT                                                      0x2
#define HWIO_APPS_SMMU_CB0_SCTLR_TRE_BMSK                                                      0x2
#define HWIO_APPS_SMMU_CB0_SCTLR_TRE_SHFT                                                      0x1
#define HWIO_APPS_SMMU_CB0_SCTLR_M_BMSK                                                        0x1
#define HWIO_APPS_SMMU_CB0_SCTLR_M_SHFT                                                        0x0

#define HWIO_APPS_SMMU_CBAR0_2_ADDR                                                     (APPS_MMU500_SMMU_APP_REG_BASE      + 0x00001000)
#define HWIO_APPS_SMMU_CBAR0_2_PHYS                                                     (APPS_MMU500_SMMU_APP_REG_BASE_PHYS + 0x00001000)
#define HWIO_APPS_SMMU_CBAR0_2_OFFS                                                     (APPS_MMU500_SMMU_APP_REG_BASE_OFFS + 0x00001000)
#define HWIO_APPS_SMMU_CBAR0_2_RMSK                                                     0xffffffff
#define HWIO_APPS_SMMU_CBAR0_2_IN          \
        in_dword_masked(HWIO_APPS_SMMU_CBAR0_2_ADDR, HWIO_APPS_SMMU_CBAR0_2_RMSK)
#define HWIO_APPS_SMMU_CBAR0_2_INM(m)      \
        in_dword_masked(HWIO_APPS_SMMU_CBAR0_2_ADDR, m)
#define HWIO_APPS_SMMU_CBAR0_2_OUT(v)      \
        out_dword(HWIO_APPS_SMMU_CBAR0_2_ADDR,v)
#define HWIO_APPS_SMMU_CBAR0_2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_APPS_SMMU_CBAR0_2_ADDR,m,v,HWIO_APPS_SMMU_CBAR0_2_IN)
#define HWIO_APPS_SMMU_CBAR0_2_IRPTNDX_BMSK                                             0xff000000
#define HWIO_APPS_SMMU_CBAR0_2_IRPTNDX_SHFT                                                   0x18
#define HWIO_APPS_SMMU_CBAR0_2_WACFG_BMSK                                                 0xc00000
#define HWIO_APPS_SMMU_CBAR0_2_WACFG_SHFT                                                     0x16
#define HWIO_APPS_SMMU_CBAR0_2_RACFG_BMSK                                                 0x300000
#define HWIO_APPS_SMMU_CBAR0_2_RACFG_SHFT                                                     0x14
#define HWIO_APPS_SMMU_CBAR0_2_BSU_BMSK                                                    0xc0000
#define HWIO_APPS_SMMU_CBAR0_2_BSU_SHFT                                                       0x12
#define HWIO_APPS_SMMU_CBAR0_2_TYPE_BMSK                                                   0x30000
#define HWIO_APPS_SMMU_CBAR0_2_TYPE_SHFT                                                      0x10
#define HWIO_APPS_SMMU_CBAR0_2_MEMATTR_CBNDX_7_4_BMSK                                       0xf000
#define HWIO_APPS_SMMU_CBAR0_2_MEMATTR_CBNDX_7_4_SHFT                                          0xc
#define HWIO_APPS_SMMU_CBAR0_2_FB_CBNDX_3_BMSK                                               0x800
#define HWIO_APPS_SMMU_CBAR0_2_FB_CBNDX_3_SHFT                                                 0xb
#define HWIO_APPS_SMMU_CBAR0_2_HYPC_CBNDX_2_BMSK                                             0x400
#define HWIO_APPS_SMMU_CBAR0_2_HYPC_CBNDX_2_SHFT                                               0xa
#define HWIO_APPS_SMMU_CBAR0_2_BPSHCFG_CBNDX_1_0_BMSK                                        0x300
#define HWIO_APPS_SMMU_CBAR0_2_BPSHCFG_CBNDX_1_0_SHFT                                          0x8
#define HWIO_APPS_SMMU_CBAR0_2_VMID_BMSK                                                      0xff
#define HWIO_APPS_SMMU_CBAR0_2_VMID_SHFT                                                       0x0

#define HWIO_APPS_SMMU_CBA2R0_2_ADDR                                                    (APPS_MMU500_SMMU_APP_REG_BASE      + 0x00001800)
#define HWIO_APPS_SMMU_CBA2R0_2_PHYS                                                    (APPS_MMU500_SMMU_APP_REG_BASE_PHYS + 0x00001800)
#define HWIO_APPS_SMMU_CBA2R0_2_OFFS                                                    (APPS_MMU500_SMMU_APP_REG_BASE_OFFS + 0x00001800)
#define HWIO_APPS_SMMU_CBA2R0_2_RMSK                                                           0x3
#define HWIO_APPS_SMMU_CBA2R0_2_IN          \
        in_dword_masked(HWIO_APPS_SMMU_CBA2R0_2_ADDR, HWIO_APPS_SMMU_CBA2R0_2_RMSK)
#define HWIO_APPS_SMMU_CBA2R0_2_INM(m)      \
        in_dword_masked(HWIO_APPS_SMMU_CBA2R0_2_ADDR, m)
#define HWIO_APPS_SMMU_CBA2R0_2_OUT(v)      \
        out_dword(HWIO_APPS_SMMU_CBA2R0_2_ADDR,v)
#define HWIO_APPS_SMMU_CBA2R0_2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_APPS_SMMU_CBA2R0_2_ADDR,m,v,HWIO_APPS_SMMU_CBA2R0_2_IN)
#define HWIO_APPS_SMMU_CBA2R0_2_MONC_BMSK                                                      0x2
#define HWIO_APPS_SMMU_CBA2R0_2_MONC_SHFT                                                      0x1
#define HWIO_APPS_SMMU_CBA2R0_2_VA64_BMSK                                                      0x1
#define HWIO_APPS_SMMU_CBA2R0_2_VA64_SHFT                                                      0x0

#define HWIO_APPS_SMMU_CB0_SCTLR_2_ADDR                                                 (APPS_MMU500_SMMU_APP_REG_BASE      + 0x00080000)
#define HWIO_APPS_SMMU_CB0_SCTLR_2_PHYS                                                 (APPS_MMU500_SMMU_APP_REG_BASE_PHYS + 0x00080000)
#define HWIO_APPS_SMMU_CB0_SCTLR_2_OFFS                                                 (APPS_MMU500_SMMU_APP_REG_BASE_OFFS + 0x00080000)
#define HWIO_APPS_SMMU_CB0_SCTLR_2_RMSK                                                 0x3ffff7ff
#define HWIO_APPS_SMMU_CB0_SCTLR_2_IN          \
        in_dword_masked(HWIO_APPS_SMMU_CB0_SCTLR_2_ADDR, HWIO_APPS_SMMU_CB0_SCTLR_2_RMSK)
#define HWIO_APPS_SMMU_CB0_SCTLR_2_INM(m)      \
        in_dword_masked(HWIO_APPS_SMMU_CB0_SCTLR_2_ADDR, m)
#define HWIO_APPS_SMMU_CB0_SCTLR_2_OUT(v)      \
        out_dword(HWIO_APPS_SMMU_CB0_SCTLR_2_ADDR,v)
#define HWIO_APPS_SMMU_CB0_SCTLR_2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_APPS_SMMU_CB0_SCTLR_2_ADDR,m,v,HWIO_APPS_SMMU_CB0_SCTLR_2_IN)
#define HWIO_APPS_SMMU_CB0_SCTLR_2_NSCFG_BMSK                                           0x30000000
#define HWIO_APPS_SMMU_CB0_SCTLR_2_NSCFG_SHFT                                                 0x1c
#define HWIO_APPS_SMMU_CB0_SCTLR_2_WACFG_BMSK                                            0xc000000
#define HWIO_APPS_SMMU_CB0_SCTLR_2_WACFG_SHFT                                                 0x1a
#define HWIO_APPS_SMMU_CB0_SCTLR_2_RACFG_BMSK                                            0x3000000
#define HWIO_APPS_SMMU_CB0_SCTLR_2_RACFG_SHFT                                                 0x18
#define HWIO_APPS_SMMU_CB0_SCTLR_2_SHCFG_BMSK                                             0xc00000
#define HWIO_APPS_SMMU_CB0_SCTLR_2_SHCFG_SHFT                                                 0x16
#define HWIO_APPS_SMMU_CB0_SCTLR_2_FB_BMSK                                                0x200000
#define HWIO_APPS_SMMU_CB0_SCTLR_2_FB_SHFT                                                    0x15
#define HWIO_APPS_SMMU_CB0_SCTLR_2_MTCFG_BMSK                                             0x100000
#define HWIO_APPS_SMMU_CB0_SCTLR_2_MTCFG_SHFT                                                 0x14
#define HWIO_APPS_SMMU_CB0_SCTLR_2_MEMATTR_BMSK                                            0xf0000
#define HWIO_APPS_SMMU_CB0_SCTLR_2_MEMATTR_SHFT                                               0x10
#define HWIO_APPS_SMMU_CB0_SCTLR_2_TRANSIENTCFG_BMSK                                        0xc000
#define HWIO_APPS_SMMU_CB0_SCTLR_2_TRANSIENTCFG_SHFT                                           0xe
#define HWIO_APPS_SMMU_CB0_SCTLR_2_PTW_BMSK                                                 0x2000
#define HWIO_APPS_SMMU_CB0_SCTLR_2_PTW_SHFT                                                    0xd
#define HWIO_APPS_SMMU_CB0_SCTLR_2_ASIDPNE_BMSK                                             0x1000
#define HWIO_APPS_SMMU_CB0_SCTLR_2_ASIDPNE_SHFT                                                0xc
#define HWIO_APPS_SMMU_CB0_SCTLR_2_UWXN_BMSK                                                 0x400
#define HWIO_APPS_SMMU_CB0_SCTLR_2_UWXN_SHFT                                                   0xa
#define HWIO_APPS_SMMU_CB0_SCTLR_2_WXN_BMSK                                                  0x200
#define HWIO_APPS_SMMU_CB0_SCTLR_2_WXN_SHFT                                                    0x9
#define HWIO_APPS_SMMU_CB0_SCTLR_2_HUPCF_BMSK                                                0x100
#define HWIO_APPS_SMMU_CB0_SCTLR_2_HUPCF_SHFT                                                  0x8
#define HWIO_APPS_SMMU_CB0_SCTLR_2_CFCFG_BMSK                                                 0x80
#define HWIO_APPS_SMMU_CB0_SCTLR_2_CFCFG_SHFT                                                  0x7
#define HWIO_APPS_SMMU_CB0_SCTLR_2_CFIE_BMSK                                                  0x40
#define HWIO_APPS_SMMU_CB0_SCTLR_2_CFIE_SHFT                                                   0x6
#define HWIO_APPS_SMMU_CB0_SCTLR_2_CFRE_BMSK                                                  0x20
#define HWIO_APPS_SMMU_CB0_SCTLR_2_CFRE_SHFT                                                   0x5
#define HWIO_APPS_SMMU_CB0_SCTLR_2_E_BMSK                                                     0x10
#define HWIO_APPS_SMMU_CB0_SCTLR_2_E_SHFT                                                      0x4
#define HWIO_APPS_SMMU_CB0_SCTLR_2_AFFD_BMSK                                                   0x8
#define HWIO_APPS_SMMU_CB0_SCTLR_2_AFFD_SHFT                                                   0x3
#define HWIO_APPS_SMMU_CB0_SCTLR_2_AFE_BMSK                                                    0x4
#define HWIO_APPS_SMMU_CB0_SCTLR_2_AFE_SHFT                                                    0x2
#define HWIO_APPS_SMMU_CB0_SCTLR_2_TRE_BMSK                                                    0x2
#define HWIO_APPS_SMMU_CB0_SCTLR_2_TRE_SHFT                                                    0x1
#define HWIO_APPS_SMMU_CB0_SCTLR_2_M_BMSK                                                      0x1
#define HWIO_APPS_SMMU_CB0_SCTLR_2_M_SHFT                                                      0x0


#endif /* __MMUSIDHWIO_5XX_H__ */
