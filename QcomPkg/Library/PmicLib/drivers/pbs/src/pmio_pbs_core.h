#ifndef __PMIO_PBS_CORE_H__
#define __PMIO_PBS_CORE_H__
/*
===========================================================================
*/
/**
  @file pmio_pbs_core.h
  @brief Auto-generated PMIO interface include file.

  This file contains PMIO register definitions for the following modules:
    PBS_CORE

  'Include' filters applied: <none>
  'Exclude' filters applied: RESERVED DUMMY 
*/
/*
  ===========================================================================

  Copyright 2018 QUALCOMM Technologies Incorporated, All Rights Reserved
  QUALCOMM Proprietary and Confidential.

  ===========================================================================


  ===========================================================================
*/

/*----------------------------------------------------------------------------
 * PERIPHERAL: PBS_CORE
 *--------------------------------------------------------------------------*/

#define PBS_CORE_BASE                                                      0x00007000

#define PMIO_PBS_CORE_REVISION1_ADDR                                       (PBS_CORE_BASE      + 0x00000000)
#define PMIO_PBS_CORE_REVISION1_OFFS                                       0x00000000
#define PMIO_PBS_CORE_REVISION1_RMSK                                             0xff
#define PMIO_PBS_CORE_REVISION1_IN(ctxt, pmic, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_REVISION1_ADDR, PMIO_PBS_CORE_REVISION1_RMSK, 0, val)
#define PMIO_PBS_CORE_REVISION1_INF(ctxt, pmic, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_REVISION1_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_REVISION1_INN(ctxt, pmic, val, len)\
        in_pmio(ctxt, pmic, PMIO_PBS_CORE_REVISION1_ADDR, val, len)
#define PMIO_PBS_CORE_REVISION1_DIG_MINOR_BMSK                                   0xff
#define PMIO_PBS_CORE_REVISION1_DIG_MINOR_SHFT                                    0x0

#define PMIO_PBS_CORE_REVISION2_ADDR                                       (PBS_CORE_BASE      + 0x00000001)
#define PMIO_PBS_CORE_REVISION2_OFFS                                       0x00000001
#define PMIO_PBS_CORE_REVISION2_RMSK                                             0xff
#define PMIO_PBS_CORE_REVISION2_IN(ctxt, pmic, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_REVISION2_ADDR, PMIO_PBS_CORE_REVISION2_RMSK, 0, val)
#define PMIO_PBS_CORE_REVISION2_INF(ctxt, pmic, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_REVISION2_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_REVISION2_INN(ctxt, pmic, val, len)\
        in_pmio(ctxt, pmic, PMIO_PBS_CORE_REVISION2_ADDR, val, len)
#define PMIO_PBS_CORE_REVISION2_DIG_MAJOR_BMSK                                   0xff
#define PMIO_PBS_CORE_REVISION2_DIG_MAJOR_SHFT                                    0x0

#define PMIO_PBS_CORE_REVISION3_ADDR                                       (PBS_CORE_BASE      + 0x00000002)
#define PMIO_PBS_CORE_REVISION3_OFFS                                       0x00000002
#define PMIO_PBS_CORE_REVISION3_RMSK                                             0xff
#define PMIO_PBS_CORE_REVISION3_IN(ctxt, pmic, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_REVISION3_ADDR, PMIO_PBS_CORE_REVISION3_RMSK, 0, val)
#define PMIO_PBS_CORE_REVISION3_INF(ctxt, pmic, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_REVISION3_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_REVISION3_INN(ctxt, pmic, val, len)\
        in_pmio(ctxt, pmic, PMIO_PBS_CORE_REVISION3_ADDR, val, len)
#define PMIO_PBS_CORE_REVISION3_ANA_MINOR_BMSK                                   0xff
#define PMIO_PBS_CORE_REVISION3_ANA_MINOR_SHFT                                    0x0

#define PMIO_PBS_CORE_REVISION4_ADDR                                       (PBS_CORE_BASE      + 0x00000003)
#define PMIO_PBS_CORE_REVISION4_OFFS                                       0x00000003
#define PMIO_PBS_CORE_REVISION4_RMSK                                             0xff
#define PMIO_PBS_CORE_REVISION4_IN(ctxt, pmic, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_REVISION4_ADDR, PMIO_PBS_CORE_REVISION4_RMSK, 0, val)
#define PMIO_PBS_CORE_REVISION4_INF(ctxt, pmic, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_REVISION4_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_REVISION4_INN(ctxt, pmic, val, len)\
        in_pmio(ctxt, pmic, PMIO_PBS_CORE_REVISION4_ADDR, val, len)
#define PMIO_PBS_CORE_REVISION4_ANA_MAJOR_BMSK                                   0xff
#define PMIO_PBS_CORE_REVISION4_ANA_MAJOR_SHFT                                    0x0

#define PMIO_PBS_CORE_PERPH_TYPE_ADDR                                      (PBS_CORE_BASE      + 0x00000004)
#define PMIO_PBS_CORE_PERPH_TYPE_OFFS                                      0x00000004
#define PMIO_PBS_CORE_PERPH_TYPE_RMSK                                            0xff
#define PMIO_PBS_CORE_PERPH_TYPE_IN(ctxt, pmic, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_PERPH_TYPE_ADDR, PMIO_PBS_CORE_PERPH_TYPE_RMSK, 0, val)
#define PMIO_PBS_CORE_PERPH_TYPE_INF(ctxt, pmic, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_PERPH_TYPE_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_PERPH_TYPE_INN(ctxt, pmic, val, len)\
        in_pmio(ctxt, pmic, PMIO_PBS_CORE_PERPH_TYPE_ADDR, val, len)
#define PMIO_PBS_CORE_PERPH_TYPE_TYPE_BMSK                                       0xff
#define PMIO_PBS_CORE_PERPH_TYPE_TYPE_SHFT                                        0x0

#define PMIO_PBS_CORE_PERPH_SUBTYPE_ADDR                                   (PBS_CORE_BASE      + 0x00000005)
#define PMIO_PBS_CORE_PERPH_SUBTYPE_OFFS                                   0x00000005
#define PMIO_PBS_CORE_PERPH_SUBTYPE_RMSK                                         0xff
#define PMIO_PBS_CORE_PERPH_SUBTYPE_IN(ctxt, pmic, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_PERPH_SUBTYPE_ADDR, PMIO_PBS_CORE_PERPH_SUBTYPE_RMSK, 0, val)
#define PMIO_PBS_CORE_PERPH_SUBTYPE_INF(ctxt, pmic, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_PERPH_SUBTYPE_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_PERPH_SUBTYPE_INN(ctxt, pmic, val, len)\
        in_pmio(ctxt, pmic, PMIO_PBS_CORE_PERPH_SUBTYPE_ADDR, val, len)
#define PMIO_PBS_CORE_PERPH_SUBTYPE_SUBTYPE_BMSK                                 0xff
#define PMIO_PBS_CORE_PERPH_SUBTYPE_SUBTYPE_SHFT                                  0x0

#define PMIO_PBS_CORE_GEN_PMIC5_MEM_CONFIG_ADDR                               (PBS_CORE_BASE      + 0x00000006)
#define PMIO_PBS_CORE_GEN_PMIC5_MEM_CONFIG_OFFS                               0x00000006
#define PMIO_PBS_CORE_GEN_PMIC5_MEM_CONFIG_RMSK                                      0x3
#define PMIO_PBS_CORE_GEN_PMIC5_MEM_CONFIG_IN(ctxt, pmic, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_GEN_PMIC5_MEM_CONFIG_ADDR, PMIO_PBS_CORE_GEN_PMIC5_MEM_CONFIG_RMSK, 0, val)
#define PMIO_PBS_CORE_GEN_PMIC5_MEM_CONFIG_INF(ctxt, pmic, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_GEN_PMIC5_MEM_CONFIG_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_GEN_PMIC5_MEM_CONFIG_INN(ctxt, pmic, val, len)\
        in_pmio(ctxt, pmic, PMIO_PBS_CORE_GEN_PMIC5_MEM_CONFIG_ADDR, val, len)
#define PMIO_PBS_CORE_GEN_PMIC5_MEM_CONFIG_PMIC5_RAM_CONFIG_BMSK                     0x2
#define PMIO_PBS_CORE_GEN_PMIC5_MEM_CONFIG_PMIC5_RAM_CONFIG_SHFT                     0x1
#define PMIO_PBS_CORE_GEN_PMIC5_MEM_CONFIG_PMIC5_OTP_CONFIG_BMSK                     0x1
#define PMIO_PBS_CORE_GEN_PMIC5_MEM_CONFIG_PMIC5_OTP_CONFIG_SHFT                     0x0

#define PMIO_PBS_CORE_NUM_CLIENTS_ADDR                                        (PBS_CORE_BASE      + 0x00000007)
#define PMIO_PBS_CORE_NUM_CLIENTS_OFFS                                        0x00000007
#define PMIO_PBS_CORE_NUM_CLIENTS_RMSK                                              0xff
#define PMIO_PBS_CORE_NUM_CLIENTS_IN(ctxt, pmic, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_NUM_CLIENTS_ADDR, PMIO_PBS_CORE_NUM_CLIENTS_RMSK, 0, val)
#define PMIO_PBS_CORE_NUM_CLIENTS_INF(ctxt, pmic, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_NUM_CLIENTS_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_NUM_CLIENTS_INN(ctxt, pmic, val, len)\
        in_pmio(ctxt, pmic, PMIO_PBS_CORE_NUM_CLIENTS_ADDR, val, len)
#define PMIO_PBS_CORE_NUM_CLIENTS_NUM_CLIENTS_BMSK                                  0xff
#define PMIO_PBS_CORE_NUM_CLIENTS_NUM_CLIENTS_SHFT                                   0x0

#define PMIO_PBS_CORE_SEQ_STATUS0_ADDR                                     (PBS_CORE_BASE      + 0x00000008)
#define PMIO_PBS_CORE_SEQ_STATUS0_OFFS                                     0x00000008
#define PMIO_PBS_CORE_SEQ_STATUS0_RMSK                                           0x3f
#define PMIO_PBS_CORE_SEQ_STATUS0_IN(ctxt, pmic, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_SEQ_STATUS0_ADDR, PMIO_PBS_CORE_SEQ_STATUS0_RMSK, 0, val)
#define PMIO_PBS_CORE_SEQ_STATUS0_INF(ctxt, pmic, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_SEQ_STATUS0_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_SEQ_STATUS0_INN(ctxt, pmic, val, len)\
        in_pmio(ctxt, pmic, PMIO_PBS_CORE_SEQ_STATUS0_ADDR, val, len)
#define PMIO_PBS_CORE_SEQ_STATUS0_LAST_SEQ_COMP_STATUS_BMSK                      0x30
#define PMIO_PBS_CORE_SEQ_STATUS0_LAST_SEQ_COMP_STATUS_SHFT                       0x4
#define PMIO_PBS_CORE_SEQ_STATUS0_LAST_SEQ_COMP_BMSK                              0xf
#define PMIO_PBS_CORE_SEQ_STATUS0_LAST_SEQ_COMP_SHFT                              0x0

#define PMIO_PBS_CORE_NUM_TRIGCL_ADDR                                         (PBS_CORE_BASE      + 0x00000009)
#define PMIO_PBS_CORE_NUM_TRIGCL_OFFS                                         0x00000009
#define PMIO_PBS_CORE_NUM_TRIGCL_RMSK                                               0xff
#define PMIO_PBS_CORE_NUM_TRIGCL_IN(ctxt, pmic, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_NUM_TRIGCL_ADDR, PMIO_PBS_CORE_NUM_TRIGCL_RMSK, 0, val)
#define PMIO_PBS_CORE_NUM_TRIGCL_INF(ctxt, pmic, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_NUM_TRIGCL_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_NUM_TRIGCL_INN(ctxt, pmic, val, len)\
        in_pmio(ctxt, pmic, PMIO_PBS_CORE_NUM_TRIGCL_ADDR, val, len)
#define PMIO_PBS_CORE_NUM_TRIGCL_NUM_TRIGCL_BMSK                                    0xff
#define PMIO_PBS_CORE_NUM_TRIGCL_NUM_TRIGCL_SHFT                                     0x0

#define PMIO_PBS_CORE_SEQ_PC_STATUS0_ADDR                                  (PBS_CORE_BASE      + 0x0000000a)
#define PMIO_PBS_CORE_SEQ_PC_STATUS0_OFFS                                  0x0000000a
#define PMIO_PBS_CORE_SEQ_PC_STATUS0_RMSK                                        0xff
#define PMIO_PBS_CORE_SEQ_PC_STATUS0_IN(ctxt, pmic, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_SEQ_PC_STATUS0_ADDR, PMIO_PBS_CORE_SEQ_PC_STATUS0_RMSK, 0, val)
#define PMIO_PBS_CORE_SEQ_PC_STATUS0_INF(ctxt, pmic, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_SEQ_PC_STATUS0_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_SEQ_PC_STATUS0_INN(ctxt, pmic, val, len)\
        in_pmio(ctxt, pmic, PMIO_PBS_CORE_SEQ_PC_STATUS0_ADDR, val, len)
#define PMIO_PBS_CORE_SEQ_PC_STATUS0_PC_LSB_BMSK                                 0xff
#define PMIO_PBS_CORE_SEQ_PC_STATUS0_PC_LSB_SHFT                                  0x0

#define PMIO_PBS_CORE_SEQ_PC_STATUS1_ADDR                                  (PBS_CORE_BASE      + 0x0000000b)
#define PMIO_PBS_CORE_SEQ_PC_STATUS1_OFFS                                  0x0000000b
#define PMIO_PBS_CORE_SEQ_PC_STATUS1_RMSK                                        0xff
#define PMIO_PBS_CORE_SEQ_PC_STATUS1_IN(ctxt, pmic, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_SEQ_PC_STATUS1_ADDR, PMIO_PBS_CORE_SEQ_PC_STATUS1_RMSK, 0, val)
#define PMIO_PBS_CORE_SEQ_PC_STATUS1_INF(ctxt, pmic, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_SEQ_PC_STATUS1_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_SEQ_PC_STATUS1_INN(ctxt, pmic, val, len)\
        in_pmio(ctxt, pmic, PMIO_PBS_CORE_SEQ_PC_STATUS1_ADDR, val, len)
#define PMIO_PBS_CORE_SEQ_PC_STATUS1_PC_MSB_BMSK                                 0xff
#define PMIO_PBS_CORE_SEQ_PC_STATUS1_PC_MSB_SHFT                                  0x0

#define PMIO_PBS_CORE_SEQ_IR_STATUS0_ADDR                                  (PBS_CORE_BASE      + 0x0000000c)
#define PMIO_PBS_CORE_SEQ_IR_STATUS0_OFFS                                  0x0000000c
#define PMIO_PBS_CORE_SEQ_IR_STATUS0_RMSK                                        0xff
#define PMIO_PBS_CORE_SEQ_IR_STATUS0_IN(ctxt, pmic, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_SEQ_IR_STATUS0_ADDR, PMIO_PBS_CORE_SEQ_IR_STATUS0_RMSK, 0, val)
#define PMIO_PBS_CORE_SEQ_IR_STATUS0_INF(ctxt, pmic, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_SEQ_IR_STATUS0_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_SEQ_IR_STATUS0_INN(ctxt, pmic, val, len)\
        in_pmio(ctxt, pmic, PMIO_PBS_CORE_SEQ_IR_STATUS0_ADDR, val, len)
#define PMIO_PBS_CORE_SEQ_IR_STATUS0_IR_BYTE0_BMSK                               0xff
#define PMIO_PBS_CORE_SEQ_IR_STATUS0_IR_BYTE0_SHFT                                0x0

#define PMIO_PBS_CORE_SEQ_IR_STATUS1_ADDR                                  (PBS_CORE_BASE      + 0x0000000d)
#define PMIO_PBS_CORE_SEQ_IR_STATUS1_OFFS                                  0x0000000d
#define PMIO_PBS_CORE_SEQ_IR_STATUS1_RMSK                                        0xff
#define PMIO_PBS_CORE_SEQ_IR_STATUS1_IN(ctxt, pmic, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_SEQ_IR_STATUS1_ADDR, PMIO_PBS_CORE_SEQ_IR_STATUS1_RMSK, 0, val)
#define PMIO_PBS_CORE_SEQ_IR_STATUS1_INF(ctxt, pmic, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_SEQ_IR_STATUS1_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_SEQ_IR_STATUS1_INN(ctxt, pmic, val, len)\
        in_pmio(ctxt, pmic, PMIO_PBS_CORE_SEQ_IR_STATUS1_ADDR, val, len)
#define PMIO_PBS_CORE_SEQ_IR_STATUS1_IR_BYTE1_BMSK                               0xff
#define PMIO_PBS_CORE_SEQ_IR_STATUS1_IR_BYTE1_SHFT                                0x0

#define PMIO_PBS_CORE_SEQ_IR_STATUS2_ADDR                                  (PBS_CORE_BASE      + 0x0000000e)
#define PMIO_PBS_CORE_SEQ_IR_STATUS2_OFFS                                  0x0000000e
#define PMIO_PBS_CORE_SEQ_IR_STATUS2_RMSK                                        0xff
#define PMIO_PBS_CORE_SEQ_IR_STATUS2_IN(ctxt, pmic, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_SEQ_IR_STATUS2_ADDR, PMIO_PBS_CORE_SEQ_IR_STATUS2_RMSK, 0, val)
#define PMIO_PBS_CORE_SEQ_IR_STATUS2_INF(ctxt, pmic, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_SEQ_IR_STATUS2_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_SEQ_IR_STATUS2_INN(ctxt, pmic, val, len)\
        in_pmio(ctxt, pmic, PMIO_PBS_CORE_SEQ_IR_STATUS2_ADDR, val, len)
#define PMIO_PBS_CORE_SEQ_IR_STATUS2_IR_BYTE2_BMSK                               0xff
#define PMIO_PBS_CORE_SEQ_IR_STATUS2_IR_BYTE2_SHFT                                0x0

#define PMIO_PBS_CORE_SEQ_IR_STATUS3_ADDR                                  (PBS_CORE_BASE      + 0x0000000f)
#define PMIO_PBS_CORE_SEQ_IR_STATUS3_OFFS                                  0x0000000f
#define PMIO_PBS_CORE_SEQ_IR_STATUS3_RMSK                                        0xff
#define PMIO_PBS_CORE_SEQ_IR_STATUS3_IN(ctxt, pmic, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_SEQ_IR_STATUS3_ADDR, PMIO_PBS_CORE_SEQ_IR_STATUS3_RMSK, 0, val)
#define PMIO_PBS_CORE_SEQ_IR_STATUS3_INF(ctxt, pmic, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_SEQ_IR_STATUS3_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_SEQ_IR_STATUS3_INN(ctxt, pmic, val, len)\
        in_pmio(ctxt, pmic, PMIO_PBS_CORE_SEQ_IR_STATUS3_ADDR, val, len)
#define PMIO_PBS_CORE_SEQ_IR_STATUS3_IR_BYTE3_BMSK                               0xff
#define PMIO_PBS_CORE_SEQ_IR_STATUS3_IR_BYTE3_SHFT                                0x0

#define PMIO_PBS_CORE_PMIC_CTL_CFG_ADDR                                       (PBS_CORE_BASE      + 0x00000030)
#define PMIO_PBS_CORE_PMIC_CTL_CFG_OFFS                                       0x00000030
#define PMIO_PBS_CORE_PMIC_CTL_CFG_RMSK                                             0x80
#define PMIO_PBS_CORE_PMIC_CTL_CFG_IN(ctxt, pmic, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_PMIC_CTL_CFG_ADDR, PMIO_PBS_CORE_PMIC_CTL_CFG_RMSK, 0, val)
#define PMIO_PBS_CORE_PMIC_CTL_CFG_INF(ctxt, pmic, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_PMIC_CTL_CFG_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_PMIC_CTL_CFG_INN(ctxt, pmic, val, len)\
        in_pmio(ctxt, pmic, PMIO_PBS_CORE_PMIC_CTL_CFG_ADDR, val, len)
#define PMIO_PBS_CORE_PMIC_CTL_CFG_OUT(ctxt, pmic, val)\
        out_pmio(ctxt, pmic, PMIO_PBS_CORE_PMIC_CTL_CFG_ADDR, val)
#define PMIO_PBS_CORE_PMIC_CTL_CFG_OUTF(ctxt, pmic, mask, shift, val)\
        out_pmio_field(ctxt, pmic, PMIO_PBS_CORE_PMIC_CTL_CFG_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_PMIC_CTL_CFG_OUTN(ctxt, pmic, val, len)\
        out_pmio_array(ctxt, pmic, PMIO_PBS_CORE_PMIC_CTL_CFG_ADDR, val, len)
#define PMIO_PBS_CORE_PMIC_CTL_CFG_LOOPBACK_MODE_BMSK                               0x80
#define PMIO_PBS_CORE_PMIC_CTL_CFG_LOOPBACK_MODE_SHFT                                0x7

#define PMIO_PBS_CORE_PBS_WDOG_CTL_ADDR                                       (PBS_CORE_BASE      + 0x00000031)
#define PMIO_PBS_CORE_PBS_WDOG_CTL_OFFS                                       0x00000031
#define PMIO_PBS_CORE_PBS_WDOG_CTL_RMSK                                             0x80
#define PMIO_PBS_CORE_PBS_WDOG_CTL_IN(ctxt, pmic, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_PBS_WDOG_CTL_ADDR, PMIO_PBS_CORE_PBS_WDOG_CTL_RMSK, 0, val)
#define PMIO_PBS_CORE_PBS_WDOG_CTL_INF(ctxt, pmic, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_PBS_WDOG_CTL_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_PBS_WDOG_CTL_INN(ctxt, pmic, val, len)\
        in_pmio(ctxt, pmic, PMIO_PBS_CORE_PBS_WDOG_CTL_ADDR, val, len)
#define PMIO_PBS_CORE_PBS_WDOG_CTL_OUT(ctxt, pmic, val)\
        out_pmio(ctxt, pmic, PMIO_PBS_CORE_PBS_WDOG_CTL_ADDR, val)
#define PMIO_PBS_CORE_PBS_WDOG_CTL_OUTF(ctxt, pmic, mask, shift, val)\
        out_pmio_field(ctxt, pmic, PMIO_PBS_CORE_PBS_WDOG_CTL_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_PBS_WDOG_CTL_OUTN(ctxt, pmic, val, len)\
        out_pmio_array(ctxt, pmic, PMIO_PBS_CORE_PBS_WDOG_CTL_ADDR, val, len)
#define PMIO_PBS_CORE_PBS_WDOG_CTL_ENABLE_BMSK                                      0x80
#define PMIO_PBS_CORE_PBS_WDOG_CTL_ENABLE_SHFT                                       0x7

#define PMIO_PBS_CORE_PBS_WDOG_PET_ADDR                                       (PBS_CORE_BASE      + 0x00000032)
#define PMIO_PBS_CORE_PBS_WDOG_PET_OFFS                                       0x00000032
#define PMIO_PBS_CORE_PBS_WDOG_PET_RMSK                                             0x80
#define PMIO_PBS_CORE_PBS_WDOG_PET_OUT(ctxt, pmic, val)\
        out_pmio(ctxt, pmic, PMIO_PBS_CORE_PBS_WDOG_PET_ADDR, val)
#define PMIO_PBS_CORE_PBS_WDOG_PET_OUTN(ctxt, pmic, val, len)\
        out_pmio_array(ctxt, pmic, PMIO_PBS_CORE_PBS_WDOG_PET_ADDR, val, len)
#define PMIO_PBS_CORE_PBS_WDOG_PET_PET_BMSK                                         0x80
#define PMIO_PBS_CORE_PBS_WDOG_PET_PET_SHFT                                          0x7

#define PMIO_PBS_CORE_MEM_INTF_CFG_ADDR                                    (PBS_CORE_BASE      + 0x00000040)
#define PMIO_PBS_CORE_MEM_INTF_CFG_OFFS                                    0x00000040
#define PMIO_PBS_CORE_MEM_INTF_CFG_RMSK                                          0x80
#define PMIO_PBS_CORE_MEM_INTF_CFG_IN(ctxt, pmic, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_MEM_INTF_CFG_ADDR, PMIO_PBS_CORE_MEM_INTF_CFG_RMSK, 0, val)
#define PMIO_PBS_CORE_MEM_INTF_CFG_INF(ctxt, pmic, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_MEM_INTF_CFG_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_MEM_INTF_CFG_INN(ctxt, pmic, val, len)\
        in_pmio(ctxt, pmic, PMIO_PBS_CORE_MEM_INTF_CFG_ADDR, val, len)
#define PMIO_PBS_CORE_MEM_INTF_CFG_OUT(ctxt, pmic, val)\
        out_pmio(ctxt, pmic, PMIO_PBS_CORE_MEM_INTF_CFG_ADDR, val)
#define PMIO_PBS_CORE_MEM_INTF_CFG_OUTF(ctxt, pmic, mask, shift, val)\
        out_pmio_field(ctxt, pmic, PMIO_PBS_CORE_MEM_INTF_CFG_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_MEM_INTF_CFG_OUTN(ctxt, pmic, val, len)\
        out_pmio_array(ctxt, pmic, PMIO_PBS_CORE_MEM_INTF_CFG_ADDR, val, len)
#define PMIO_PBS_CORE_MEM_INTF_CFG_RIF_MEM_ACCESS_EN_BMSK                        0x80
#define PMIO_PBS_CORE_MEM_INTF_CFG_RIF_MEM_ACCESS_EN_SHFT                         0x7

#define PMIO_PBS_CORE_MEM_INTF_CTL_ADDR                                    (PBS_CORE_BASE      + 0x00000041)
#define PMIO_PBS_CORE_MEM_INTF_CTL_OFFS                                    0x00000041
#define PMIO_PBS_CORE_MEM_INTF_CTL_RMSK                                          0xc0
#define PMIO_PBS_CORE_MEM_INTF_CTL_IN(ctxt, pmic, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_MEM_INTF_CTL_ADDR, PMIO_PBS_CORE_MEM_INTF_CTL_RMSK, 0, val)
#define PMIO_PBS_CORE_MEM_INTF_CTL_INF(ctxt, pmic, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_MEM_INTF_CTL_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_MEM_INTF_CTL_INN(ctxt, pmic, val, len)\
        in_pmio(ctxt, pmic, PMIO_PBS_CORE_MEM_INTF_CTL_ADDR, val, len)
#define PMIO_PBS_CORE_MEM_INTF_CTL_OUT(ctxt, pmic, val)\
        out_pmio(ctxt, pmic, PMIO_PBS_CORE_MEM_INTF_CTL_ADDR, val)
#define PMIO_PBS_CORE_MEM_INTF_CTL_OUTF(ctxt, pmic, mask, shift, val)\
        out_pmio_field(ctxt, pmic, PMIO_PBS_CORE_MEM_INTF_CTL_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_MEM_INTF_CTL_OUTN(ctxt, pmic, val, len)\
        out_pmio_array(ctxt, pmic, PMIO_PBS_CORE_MEM_INTF_CTL_ADDR, val, len)
#define PMIO_PBS_CORE_MEM_INTF_CTL_BURST_BMSK                                    0x80
#define PMIO_PBS_CORE_MEM_INTF_CTL_BURST_SHFT                                     0x7
#define PMIO_PBS_CORE_MEM_INTF_CTL_WR_EN_BMSK                                    0x40
#define PMIO_PBS_CORE_MEM_INTF_CTL_WR_EN_SHFT                                     0x6

#define PMIO_PBS_CORE_MEM_INTF_ADDR_LSB_ADDR                               (PBS_CORE_BASE      + 0x00000042)
#define PMIO_PBS_CORE_MEM_INTF_ADDR_LSB_OFFS                               0x00000042
#define PMIO_PBS_CORE_MEM_INTF_ADDR_LSB_RMSK                                     0xff
#define PMIO_PBS_CORE_MEM_INTF_ADDR_LSB_IN(ctxt, pmic, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_MEM_INTF_ADDR_LSB_ADDR, PMIO_PBS_CORE_MEM_INTF_ADDR_LSB_RMSK, 0, val)
#define PMIO_PBS_CORE_MEM_INTF_ADDR_LSB_INF(ctxt, pmic, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_MEM_INTF_ADDR_LSB_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_MEM_INTF_ADDR_LSB_INN(ctxt, pmic, val, len)\
        in_pmio(ctxt, pmic, PMIO_PBS_CORE_MEM_INTF_ADDR_LSB_ADDR, val, len)
#define PMIO_PBS_CORE_MEM_INTF_ADDR_LSB_OUT(ctxt, pmic, val)\
        out_pmio(ctxt, pmic, PMIO_PBS_CORE_MEM_INTF_ADDR_LSB_ADDR, val)
#define PMIO_PBS_CORE_MEM_INTF_ADDR_LSB_OUTF(ctxt, pmic, mask, shift, val)\
        out_pmio_field(ctxt, pmic, PMIO_PBS_CORE_MEM_INTF_ADDR_LSB_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_MEM_INTF_ADDR_LSB_OUTN(ctxt, pmic, val, len)\
        out_pmio_array(ctxt, pmic, PMIO_PBS_CORE_MEM_INTF_ADDR_LSB_ADDR, val, len)
#define PMIO_PBS_CORE_MEM_INTF_ADDR_LSB_MEM_INTF_ADDR_LSB_BMSK                   0xff
#define PMIO_PBS_CORE_MEM_INTF_ADDR_LSB_MEM_INTF_ADDR_LSB_SHFT                    0x0

#define PMIO_PBS_CORE_MEM_INTF_ADDR_MSB_ADDR                               (PBS_CORE_BASE      + 0x00000043)
#define PMIO_PBS_CORE_MEM_INTF_ADDR_MSB_OFFS                               0x00000043
#define PMIO_PBS_CORE_MEM_INTF_ADDR_MSB_RMSK                                     0xff
#define PMIO_PBS_CORE_MEM_INTF_ADDR_MSB_IN(ctxt, pmic, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_MEM_INTF_ADDR_MSB_ADDR, PMIO_PBS_CORE_MEM_INTF_ADDR_MSB_RMSK, 0, val)
#define PMIO_PBS_CORE_MEM_INTF_ADDR_MSB_INF(ctxt, pmic, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_MEM_INTF_ADDR_MSB_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_MEM_INTF_ADDR_MSB_INN(ctxt, pmic, val, len)\
        in_pmio(ctxt, pmic, PMIO_PBS_CORE_MEM_INTF_ADDR_MSB_ADDR, val, len)
#define PMIO_PBS_CORE_MEM_INTF_ADDR_MSB_OUT(ctxt, pmic, val)\
        out_pmio(ctxt, pmic, PMIO_PBS_CORE_MEM_INTF_ADDR_MSB_ADDR, val)
#define PMIO_PBS_CORE_MEM_INTF_ADDR_MSB_OUTF(ctxt, pmic, mask, shift, val)\
        out_pmio_field(ctxt, pmic, PMIO_PBS_CORE_MEM_INTF_ADDR_MSB_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_MEM_INTF_ADDR_MSB_OUTN(ctxt, pmic, val, len)\
        out_pmio_array(ctxt, pmic, PMIO_PBS_CORE_MEM_INTF_ADDR_MSB_ADDR, val, len)
#define PMIO_PBS_CORE_MEM_INTF_ADDR_MSB_MEM_INTF_ADDR_MSB_BMSK                   0xff
#define PMIO_PBS_CORE_MEM_INTF_ADDR_MSB_MEM_INTF_ADDR_MSB_SHFT                    0x0

#define PMIO_PBS_CORE_MEM_INTF_WR_DATA0_ADDR                               (PBS_CORE_BASE      + 0x00000048)
#define PMIO_PBS_CORE_MEM_INTF_WR_DATA0_OFFS                               0x00000048
#define PMIO_PBS_CORE_MEM_INTF_WR_DATA0_RMSK                                     0xff
#define PMIO_PBS_CORE_MEM_INTF_WR_DATA0_IN(ctxt, pmic, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_MEM_INTF_WR_DATA0_ADDR, PMIO_PBS_CORE_MEM_INTF_WR_DATA0_RMSK, 0, val)
#define PMIO_PBS_CORE_MEM_INTF_WR_DATA0_INF(ctxt, pmic, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_MEM_INTF_WR_DATA0_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_MEM_INTF_WR_DATA0_INN(ctxt, pmic, val, len)\
        in_pmio(ctxt, pmic, PMIO_PBS_CORE_MEM_INTF_WR_DATA0_ADDR, val, len)
#define PMIO_PBS_CORE_MEM_INTF_WR_DATA0_OUT(ctxt, pmic, val)\
        out_pmio(ctxt, pmic, PMIO_PBS_CORE_MEM_INTF_WR_DATA0_ADDR, val)
#define PMIO_PBS_CORE_MEM_INTF_WR_DATA0_OUTF(ctxt, pmic, mask, shift, val)\
        out_pmio_field(ctxt, pmic, PMIO_PBS_CORE_MEM_INTF_WR_DATA0_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_MEM_INTF_WR_DATA0_OUTN(ctxt, pmic, val, len)\
        out_pmio_array(ctxt, pmic, PMIO_PBS_CORE_MEM_INTF_WR_DATA0_ADDR, val, len)
#define PMIO_PBS_CORE_MEM_INTF_WR_DATA0_MEM_INTF_WR_DATA_0_BMSK                  0xff
#define PMIO_PBS_CORE_MEM_INTF_WR_DATA0_MEM_INTF_WR_DATA_0_SHFT                   0x0

#define PMIO_PBS_CORE_MEM_INTF_WR_DATA1_ADDR                               (PBS_CORE_BASE      + 0x00000049)
#define PMIO_PBS_CORE_MEM_INTF_WR_DATA1_OFFS                               0x00000049
#define PMIO_PBS_CORE_MEM_INTF_WR_DATA1_RMSK                                     0xff
#define PMIO_PBS_CORE_MEM_INTF_WR_DATA1_IN(ctxt, pmic, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_MEM_INTF_WR_DATA1_ADDR, PMIO_PBS_CORE_MEM_INTF_WR_DATA1_RMSK, 0, val)
#define PMIO_PBS_CORE_MEM_INTF_WR_DATA1_INF(ctxt, pmic, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_MEM_INTF_WR_DATA1_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_MEM_INTF_WR_DATA1_INN(ctxt, pmic, val, len)\
        in_pmio(ctxt, pmic, PMIO_PBS_CORE_MEM_INTF_WR_DATA1_ADDR, val, len)
#define PMIO_PBS_CORE_MEM_INTF_WR_DATA1_OUT(ctxt, pmic, val)\
        out_pmio(ctxt, pmic, PMIO_PBS_CORE_MEM_INTF_WR_DATA1_ADDR, val)
#define PMIO_PBS_CORE_MEM_INTF_WR_DATA1_OUTF(ctxt, pmic, mask, shift, val)\
        out_pmio_field(ctxt, pmic, PMIO_PBS_CORE_MEM_INTF_WR_DATA1_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_MEM_INTF_WR_DATA1_OUTN(ctxt, pmic, val, len)\
        out_pmio_array(ctxt, pmic, PMIO_PBS_CORE_MEM_INTF_WR_DATA1_ADDR, val, len)
#define PMIO_PBS_CORE_MEM_INTF_WR_DATA1_MEM_INTF_WR_DATA_1_BMSK                  0xff
#define PMIO_PBS_CORE_MEM_INTF_WR_DATA1_MEM_INTF_WR_DATA_1_SHFT                   0x0

#define PMIO_PBS_CORE_MEM_INTF_WR_DATA2_ADDR                               (PBS_CORE_BASE      + 0x0000004a)
#define PMIO_PBS_CORE_MEM_INTF_WR_DATA2_OFFS                               0x0000004a
#define PMIO_PBS_CORE_MEM_INTF_WR_DATA2_RMSK                                     0xff
#define PMIO_PBS_CORE_MEM_INTF_WR_DATA2_IN(ctxt, pmic, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_MEM_INTF_WR_DATA2_ADDR, PMIO_PBS_CORE_MEM_INTF_WR_DATA2_RMSK, 0, val)
#define PMIO_PBS_CORE_MEM_INTF_WR_DATA2_INF(ctxt, pmic, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_MEM_INTF_WR_DATA2_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_MEM_INTF_WR_DATA2_INN(ctxt, pmic, val, len)\
        in_pmio(ctxt, pmic, PMIO_PBS_CORE_MEM_INTF_WR_DATA2_ADDR, val, len)
#define PMIO_PBS_CORE_MEM_INTF_WR_DATA2_OUT(ctxt, pmic, val)\
        out_pmio(ctxt, pmic, PMIO_PBS_CORE_MEM_INTF_WR_DATA2_ADDR, val)
#define PMIO_PBS_CORE_MEM_INTF_WR_DATA2_OUTF(ctxt, pmic, mask, shift, val)\
        out_pmio_field(ctxt, pmic, PMIO_PBS_CORE_MEM_INTF_WR_DATA2_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_MEM_INTF_WR_DATA2_OUTN(ctxt, pmic, val, len)\
        out_pmio_array(ctxt, pmic, PMIO_PBS_CORE_MEM_INTF_WR_DATA2_ADDR, val, len)
#define PMIO_PBS_CORE_MEM_INTF_WR_DATA2_MEM_INTF_WR_DATA_2_BMSK                  0xff
#define PMIO_PBS_CORE_MEM_INTF_WR_DATA2_MEM_INTF_WR_DATA_2_SHFT                   0x0

#define PMIO_PBS_CORE_MEM_INTF_WR_DATA3_ADDR                               (PBS_CORE_BASE      + 0x0000004b)
#define PMIO_PBS_CORE_MEM_INTF_WR_DATA3_OFFS                               0x0000004b
#define PMIO_PBS_CORE_MEM_INTF_WR_DATA3_RMSK                                     0xff
#define PMIO_PBS_CORE_MEM_INTF_WR_DATA3_IN(ctxt, pmic, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_MEM_INTF_WR_DATA3_ADDR, PMIO_PBS_CORE_MEM_INTF_WR_DATA3_RMSK, 0, val)
#define PMIO_PBS_CORE_MEM_INTF_WR_DATA3_INF(ctxt, pmic, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_MEM_INTF_WR_DATA3_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_MEM_INTF_WR_DATA3_INN(ctxt, pmic, val, len)\
        in_pmio(ctxt, pmic, PMIO_PBS_CORE_MEM_INTF_WR_DATA3_ADDR, val, len)
#define PMIO_PBS_CORE_MEM_INTF_WR_DATA3_OUT(ctxt, pmic, val)\
        out_pmio(ctxt, pmic, PMIO_PBS_CORE_MEM_INTF_WR_DATA3_ADDR, val)
#define PMIO_PBS_CORE_MEM_INTF_WR_DATA3_OUTF(ctxt, pmic, mask, shift, val)\
        out_pmio_field(ctxt, pmic, PMIO_PBS_CORE_MEM_INTF_WR_DATA3_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_MEM_INTF_WR_DATA3_OUTN(ctxt, pmic, val, len)\
        out_pmio_array(ctxt, pmic, PMIO_PBS_CORE_MEM_INTF_WR_DATA3_ADDR, val, len)
#define PMIO_PBS_CORE_MEM_INTF_WR_DATA3_MEM_INTF_WR_DATA_3_BMSK                  0xff
#define PMIO_PBS_CORE_MEM_INTF_WR_DATA3_MEM_INTF_WR_DATA_3_SHFT                   0x0

#define PMIO_PBS_CORE_MEM_INTF_RD_DATA0_ADDR                               (PBS_CORE_BASE      + 0x0000004c)
#define PMIO_PBS_CORE_MEM_INTF_RD_DATA0_OFFS                               0x0000004c
#define PMIO_PBS_CORE_MEM_INTF_RD_DATA0_RMSK                                     0xff
#define PMIO_PBS_CORE_MEM_INTF_RD_DATA0_IN(ctxt, pmic, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_MEM_INTF_RD_DATA0_ADDR, PMIO_PBS_CORE_MEM_INTF_RD_DATA0_RMSK, 0, val)
#define PMIO_PBS_CORE_MEM_INTF_RD_DATA0_INF(ctxt, pmic, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_MEM_INTF_RD_DATA0_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_MEM_INTF_RD_DATA0_INN(ctxt, pmic, val, len)\
        in_pmio(ctxt, pmic, PMIO_PBS_CORE_MEM_INTF_RD_DATA0_ADDR, val, len)
#define PMIO_PBS_CORE_MEM_INTF_RD_DATA0_MEM_INTF_RD_DATA_0_BMSK                  0xff
#define PMIO_PBS_CORE_MEM_INTF_RD_DATA0_MEM_INTF_RD_DATA_0_SHFT                   0x0

#define PMIO_PBS_CORE_MEM_INTF_RD_DATA1_ADDR                               (PBS_CORE_BASE      + 0x0000004d)
#define PMIO_PBS_CORE_MEM_INTF_RD_DATA1_OFFS                               0x0000004d
#define PMIO_PBS_CORE_MEM_INTF_RD_DATA1_RMSK                                     0xff
#define PMIO_PBS_CORE_MEM_INTF_RD_DATA1_IN(ctxt, pmic, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_MEM_INTF_RD_DATA1_ADDR, PMIO_PBS_CORE_MEM_INTF_RD_DATA1_RMSK, 0, val)
#define PMIO_PBS_CORE_MEM_INTF_RD_DATA1_INF(ctxt, pmic, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_MEM_INTF_RD_DATA1_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_MEM_INTF_RD_DATA1_INN(ctxt, pmic, val, len)\
        in_pmio(ctxt, pmic, PMIO_PBS_CORE_MEM_INTF_RD_DATA1_ADDR, val, len)
#define PMIO_PBS_CORE_MEM_INTF_RD_DATA1_MEM_INTF_RD_DATA_1_BMSK                  0xff
#define PMIO_PBS_CORE_MEM_INTF_RD_DATA1_MEM_INTF_RD_DATA_1_SHFT                   0x0

#define PMIO_PBS_CORE_MEM_INTF_RD_DATA2_ADDR                               (PBS_CORE_BASE      + 0x0000004e)
#define PMIO_PBS_CORE_MEM_INTF_RD_DATA2_OFFS                               0x0000004e
#define PMIO_PBS_CORE_MEM_INTF_RD_DATA2_RMSK                                     0xff
#define PMIO_PBS_CORE_MEM_INTF_RD_DATA2_IN(ctxt, pmic, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_MEM_INTF_RD_DATA2_ADDR, PMIO_PBS_CORE_MEM_INTF_RD_DATA2_RMSK, 0, val)
#define PMIO_PBS_CORE_MEM_INTF_RD_DATA2_INF(ctxt, pmic, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_MEM_INTF_RD_DATA2_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_MEM_INTF_RD_DATA2_INN(ctxt, pmic, val, len)\
        in_pmio(ctxt, pmic, PMIO_PBS_CORE_MEM_INTF_RD_DATA2_ADDR, val, len)
#define PMIO_PBS_CORE_MEM_INTF_RD_DATA2_MEM_INTF_RD_DATA_2_BMSK                  0xff
#define PMIO_PBS_CORE_MEM_INTF_RD_DATA2_MEM_INTF_RD_DATA_2_SHFT                   0x0

#define PMIO_PBS_CORE_MEM_INTF_RD_DATA3_ADDR                               (PBS_CORE_BASE      + 0x0000004f)
#define PMIO_PBS_CORE_MEM_INTF_RD_DATA3_OFFS                               0x0000004f
#define PMIO_PBS_CORE_MEM_INTF_RD_DATA3_RMSK                                     0xff
#define PMIO_PBS_CORE_MEM_INTF_RD_DATA3_IN(ctxt, pmic, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_MEM_INTF_RD_DATA3_ADDR, PMIO_PBS_CORE_MEM_INTF_RD_DATA3_RMSK, 0, val)
#define PMIO_PBS_CORE_MEM_INTF_RD_DATA3_INF(ctxt, pmic, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_MEM_INTF_RD_DATA3_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_MEM_INTF_RD_DATA3_INN(ctxt, pmic, val, len)\
        in_pmio(ctxt, pmic, PMIO_PBS_CORE_MEM_INTF_RD_DATA3_ADDR, val, len)
#define PMIO_PBS_CORE_MEM_INTF_RD_DATA3_MEM_INTF_RD_DATA_3_BMSK                  0xff
#define PMIO_PBS_CORE_MEM_INTF_RD_DATA3_MEM_INTF_RD_DATA_3_SHFT                   0x0

#define PMIO_PBS_CORE_TRIG_START_ADDR_LSB_0_ADDR                              (PBS_CORE_BASE      + 0x00000050)
#define PMIO_PBS_CORE_TRIG_START_ADDR_LSB_0_OFFS                              0x00000050
#define PMIO_PBS_CORE_TRIG_START_ADDR_LSB_0_RMSK                                    0xff
#define PMIO_PBS_CORE_TRIG_START_ADDR_LSB_0_IN(ctxt, pmic, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_TRIG_START_ADDR_LSB_0_ADDR, PMIO_PBS_CORE_TRIG_START_ADDR_LSB_0_RMSK, 0, val)
#define PMIO_PBS_CORE_TRIG_START_ADDR_LSB_0_INF(ctxt, pmic, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_TRIG_START_ADDR_LSB_0_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_TRIG_START_ADDR_LSB_0_INN(ctxt, pmic, val, len)\
        in_pmio(ctxt, pmic, PMIO_PBS_CORE_TRIG_START_ADDR_LSB_0_ADDR, val, len)
#define PMIO_PBS_CORE_TRIG_START_ADDR_LSB_0_OUT(ctxt, pmic, val)\
        out_pmio(ctxt, pmic, PMIO_PBS_CORE_TRIG_START_ADDR_LSB_0_ADDR, val)
#define PMIO_PBS_CORE_TRIG_START_ADDR_LSB_0_OUTF(ctxt, pmic, mask, shift, val)\
        out_pmio_field(ctxt, pmic, PMIO_PBS_CORE_TRIG_START_ADDR_LSB_0_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_TRIG_START_ADDR_LSB_0_OUTN(ctxt, pmic, val, len)\
        out_pmio_array(ctxt, pmic, PMIO_PBS_CORE_TRIG_START_ADDR_LSB_0_ADDR, val, len)
#define PMIO_PBS_CORE_TRIG_START_ADDR_LSB_0_SEQ_ADDR_LSB_BMSK                       0xff
#define PMIO_PBS_CORE_TRIG_START_ADDR_LSB_0_SEQ_ADDR_LSB_SHFT                        0x0

#define PMIO_PBS_CORE_TRIG_START_ADDR_LSB_1_ADDR                              (PBS_CORE_BASE      + 0x00000054)
#define PMIO_PBS_CORE_TRIG_START_ADDR_LSB_1_OFFS                              0x00000054
#define PMIO_PBS_CORE_TRIG_START_ADDR_LSB_1_RMSK                                    0xff
#define PMIO_PBS_CORE_TRIG_START_ADDR_LSB_1_IN(ctxt, pmic, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_TRIG_START_ADDR_LSB_1_ADDR, PMIO_PBS_CORE_TRIG_START_ADDR_LSB_1_RMSK, 0, val)
#define PMIO_PBS_CORE_TRIG_START_ADDR_LSB_1_INF(ctxt, pmic, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_TRIG_START_ADDR_LSB_1_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_TRIG_START_ADDR_LSB_1_INN(ctxt, pmic, val, len)\
        in_pmio(ctxt, pmic, PMIO_PBS_CORE_TRIG_START_ADDR_LSB_1_ADDR, val, len)
#define PMIO_PBS_CORE_TRIG_START_ADDR_LSB_1_OUT(ctxt, pmic, val)\
        out_pmio(ctxt, pmic, PMIO_PBS_CORE_TRIG_START_ADDR_LSB_1_ADDR, val)
#define PMIO_PBS_CORE_TRIG_START_ADDR_LSB_1_OUTF(ctxt, pmic, mask, shift, val)\
        out_pmio_field(ctxt, pmic, PMIO_PBS_CORE_TRIG_START_ADDR_LSB_1_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_TRIG_START_ADDR_LSB_1_OUTN(ctxt, pmic, val, len)\
        out_pmio_array(ctxt, pmic, PMIO_PBS_CORE_TRIG_START_ADDR_LSB_1_ADDR, val, len)
#define PMIO_PBS_CORE_TRIG_START_ADDR_LSB_1_SEQ_ADDR_LSB_BMSK                       0xff
#define PMIO_PBS_CORE_TRIG_START_ADDR_LSB_1_SEQ_ADDR_LSB_SHFT                        0x0

#define PMIO_PBS_CORE_TRIG_START_ADDR_MSB_0_ADDR                              (PBS_CORE_BASE      + 0x00000051)
#define PMIO_PBS_CORE_TRIG_START_ADDR_MSB_0_OFFS                              0x00000051
#define PMIO_PBS_CORE_TRIG_START_ADDR_MSB_0_RMSK                                    0xff
#define PMIO_PBS_CORE_TRIG_START_ADDR_MSB_0_IN(ctxt, pmic, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_TRIG_START_ADDR_MSB_0_ADDR, PMIO_PBS_CORE_TRIG_START_ADDR_MSB_0_RMSK, 0, val)
#define PMIO_PBS_CORE_TRIG_START_ADDR_MSB_0_INF(ctxt, pmic, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_TRIG_START_ADDR_MSB_0_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_TRIG_START_ADDR_MSB_0_INN(ctxt, pmic, val, len)\
        in_pmio(ctxt, pmic, PMIO_PBS_CORE_TRIG_START_ADDR_MSB_0_ADDR, val, len)
#define PMIO_PBS_CORE_TRIG_START_ADDR_MSB_0_OUT(ctxt, pmic, val)\
        out_pmio(ctxt, pmic, PMIO_PBS_CORE_TRIG_START_ADDR_MSB_0_ADDR, val)
#define PMIO_PBS_CORE_TRIG_START_ADDR_MSB_0_OUTF(ctxt, pmic, mask, shift, val)\
        out_pmio_field(ctxt, pmic, PMIO_PBS_CORE_TRIG_START_ADDR_MSB_0_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_TRIG_START_ADDR_MSB_0_OUTN(ctxt, pmic, val, len)\
        out_pmio_array(ctxt, pmic, PMIO_PBS_CORE_TRIG_START_ADDR_MSB_0_ADDR, val, len)
#define PMIO_PBS_CORE_TRIG_START_ADDR_MSB_0_SEQ_ADDR_MSB_BMSK                       0xff
#define PMIO_PBS_CORE_TRIG_START_ADDR_MSB_0_SEQ_ADDR_MSB_SHFT                        0x0

#define PMIO_PBS_CORE_TRIG_START_ADDR_MSB_1_ADDR                              (PBS_CORE_BASE      + 0x00000055)
#define PMIO_PBS_CORE_TRIG_START_ADDR_MSB_1_OFFS                              0x00000055
#define PMIO_PBS_CORE_TRIG_START_ADDR_MSB_1_RMSK                                    0xff
#define PMIO_PBS_CORE_TRIG_START_ADDR_MSB_1_IN(ctxt, pmic, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_TRIG_START_ADDR_MSB_1_ADDR, PMIO_PBS_CORE_TRIG_START_ADDR_MSB_1_RMSK, 0, val)
#define PMIO_PBS_CORE_TRIG_START_ADDR_MSB_1_INF(ctxt, pmic, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_TRIG_START_ADDR_MSB_1_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_TRIG_START_ADDR_MSB_1_INN(ctxt, pmic, val, len)\
        in_pmio(ctxt, pmic, PMIO_PBS_CORE_TRIG_START_ADDR_MSB_1_ADDR, val, len)
#define PMIO_PBS_CORE_TRIG_START_ADDR_MSB_1_OUT(ctxt, pmic, val)\
        out_pmio(ctxt, pmic, PMIO_PBS_CORE_TRIG_START_ADDR_MSB_1_ADDR, val)
#define PMIO_PBS_CORE_TRIG_START_ADDR_MSB_1_OUTF(ctxt, pmic, mask, shift, val)\
        out_pmio_field(ctxt, pmic, PMIO_PBS_CORE_TRIG_START_ADDR_MSB_1_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_TRIG_START_ADDR_MSB_1_OUTN(ctxt, pmic, val, len)\
        out_pmio_array(ctxt, pmic, PMIO_PBS_CORE_TRIG_START_ADDR_MSB_1_ADDR, val, len)
#define PMIO_PBS_CORE_TRIG_START_ADDR_MSB_1_SEQ_ADDR_MSB_BMSK                       0xff
#define PMIO_PBS_CORE_TRIG_START_ADDR_MSB_1_SEQ_ADDR_MSB_SHFT                        0x0

#define PMIO_PBS_CORE_SEQ_STATUS1_ADDR                                     (PBS_CORE_BASE      + 0x00000090)
#define PMIO_PBS_CORE_SEQ_STATUS1_OFFS                                     0x00000090
#define PMIO_PBS_CORE_SEQ_STATUS1_RMSK                                           0xff
#define PMIO_PBS_CORE_SEQ_STATUS1_IN(ctxt, pmic, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_SEQ_STATUS1_ADDR, PMIO_PBS_CORE_SEQ_STATUS1_RMSK, 0, val)
#define PMIO_PBS_CORE_SEQ_STATUS1_INF(ctxt, pmic, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_SEQ_STATUS1_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_SEQ_STATUS1_INN(ctxt, pmic, val, len)\
        in_pmio(ctxt, pmic, PMIO_PBS_CORE_SEQ_STATUS1_ADDR, val, len)
#define PMIO_PBS_CORE_SEQ_STATUS1_SEQ_IN_SERVICE_BMSK                            0xf0
#define PMIO_PBS_CORE_SEQ_STATUS1_SEQ_IN_SERVICE_SHFT                             0x4
#define PMIO_PBS_CORE_SEQ_STATUS1_LAST_SEQ_NORMAL_BMSK                            0xf
#define PMIO_PBS_CORE_SEQ_STATUS1_LAST_SEQ_NORMAL_SHFT                            0x0

#define PMIO_PBS_CORE_SEQ_STATUS2_ADDR                                     (PBS_CORE_BASE      + 0x00000091)
#define PMIO_PBS_CORE_SEQ_STATUS2_OFFS                                     0x00000091
#define PMIO_PBS_CORE_SEQ_STATUS2_RMSK                                           0xff
#define PMIO_PBS_CORE_SEQ_STATUS2_IN(ctxt, pmic, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_SEQ_STATUS2_ADDR, PMIO_PBS_CORE_SEQ_STATUS2_RMSK, 0, val)
#define PMIO_PBS_CORE_SEQ_STATUS2_INF(ctxt, pmic, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_SEQ_STATUS2_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_SEQ_STATUS2_INN(ctxt, pmic, val, len)\
        in_pmio(ctxt, pmic, PMIO_PBS_CORE_SEQ_STATUS2_ADDR, val, len)
#define PMIO_PBS_CORE_SEQ_STATUS2_LAST_SEQ_ABORTED_BMSK                          0xf0
#define PMIO_PBS_CORE_SEQ_STATUS2_LAST_SEQ_ABORTED_SHFT                           0x4
#define PMIO_PBS_CORE_SEQ_STATUS2_LAST_SEQ_ERROR_BMSK                             0xf
#define PMIO_PBS_CORE_SEQ_STATUS2_LAST_SEQ_ERROR_SHFT                             0x0

#define PMIO_PBS_CORE_SEQ_ERR_PC_STATUS0_ADDR                              (PBS_CORE_BASE      + 0x00000092)
#define PMIO_PBS_CORE_SEQ_ERR_PC_STATUS0_OFFS                              0x00000092
#define PMIO_PBS_CORE_SEQ_ERR_PC_STATUS0_RMSK                                    0xff
#define PMIO_PBS_CORE_SEQ_ERR_PC_STATUS0_IN(ctxt, pmic, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_SEQ_ERR_PC_STATUS0_ADDR, PMIO_PBS_CORE_SEQ_ERR_PC_STATUS0_RMSK, 0, val)
#define PMIO_PBS_CORE_SEQ_ERR_PC_STATUS0_INF(ctxt, pmic, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_SEQ_ERR_PC_STATUS0_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_SEQ_ERR_PC_STATUS0_INN(ctxt, pmic, val, len)\
        in_pmio(ctxt, pmic, PMIO_PBS_CORE_SEQ_ERR_PC_STATUS0_ADDR, val, len)
#define PMIO_PBS_CORE_SEQ_ERR_PC_STATUS0_ERR_PC_LSB_BMSK                         0xff
#define PMIO_PBS_CORE_SEQ_ERR_PC_STATUS0_ERR_PC_LSB_SHFT                          0x0

#define PMIO_PBS_CORE_SEQ_ERR_PC_STATUS1_ADDR                              (PBS_CORE_BASE      + 0x00000093)
#define PMIO_PBS_CORE_SEQ_ERR_PC_STATUS1_OFFS                              0x00000093
#define PMIO_PBS_CORE_SEQ_ERR_PC_STATUS1_RMSK                                    0xff
#define PMIO_PBS_CORE_SEQ_ERR_PC_STATUS1_IN(ctxt, pmic, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_SEQ_ERR_PC_STATUS1_ADDR, PMIO_PBS_CORE_SEQ_ERR_PC_STATUS1_RMSK, 0, val)
#define PMIO_PBS_CORE_SEQ_ERR_PC_STATUS1_INF(ctxt, pmic, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_SEQ_ERR_PC_STATUS1_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_SEQ_ERR_PC_STATUS1_INN(ctxt, pmic, val, len)\
        in_pmio(ctxt, pmic, PMIO_PBS_CORE_SEQ_ERR_PC_STATUS1_ADDR, val, len)
#define PMIO_PBS_CORE_SEQ_ERR_PC_STATUS1_ERR_PC_MSB_BMSK                         0xff
#define PMIO_PBS_CORE_SEQ_ERR_PC_STATUS1_ERR_PC_MSB_SHFT                          0x0

#define PMIO_PBS_CORE_SEQ_ERR_IR_STATUS0_ADDR                              (PBS_CORE_BASE      + 0x00000094)
#define PMIO_PBS_CORE_SEQ_ERR_IR_STATUS0_OFFS                              0x00000094
#define PMIO_PBS_CORE_SEQ_ERR_IR_STATUS0_RMSK                                    0xff
#define PMIO_PBS_CORE_SEQ_ERR_IR_STATUS0_IN(ctxt, pmic, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_SEQ_ERR_IR_STATUS0_ADDR, PMIO_PBS_CORE_SEQ_ERR_IR_STATUS0_RMSK, 0, val)
#define PMIO_PBS_CORE_SEQ_ERR_IR_STATUS0_INF(ctxt, pmic, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_SEQ_ERR_IR_STATUS0_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_SEQ_ERR_IR_STATUS0_INN(ctxt, pmic, val, len)\
        in_pmio(ctxt, pmic, PMIO_PBS_CORE_SEQ_ERR_IR_STATUS0_ADDR, val, len)
#define PMIO_PBS_CORE_SEQ_ERR_IR_STATUS0_ERR_IR_BYTE0_BMSK                       0xff
#define PMIO_PBS_CORE_SEQ_ERR_IR_STATUS0_ERR_IR_BYTE0_SHFT                        0x0

#define PMIO_PBS_CORE_SEQ_ERR_IR_STATUS1_ADDR                              (PBS_CORE_BASE      + 0x00000095)
#define PMIO_PBS_CORE_SEQ_ERR_IR_STATUS1_OFFS                              0x00000095
#define PMIO_PBS_CORE_SEQ_ERR_IR_STATUS1_RMSK                                    0xff
#define PMIO_PBS_CORE_SEQ_ERR_IR_STATUS1_IN(ctxt, pmic, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_SEQ_ERR_IR_STATUS1_ADDR, PMIO_PBS_CORE_SEQ_ERR_IR_STATUS1_RMSK, 0, val)
#define PMIO_PBS_CORE_SEQ_ERR_IR_STATUS1_INF(ctxt, pmic, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_SEQ_ERR_IR_STATUS1_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_SEQ_ERR_IR_STATUS1_INN(ctxt, pmic, val, len)\
        in_pmio(ctxt, pmic, PMIO_PBS_CORE_SEQ_ERR_IR_STATUS1_ADDR, val, len)
#define PMIO_PBS_CORE_SEQ_ERR_IR_STATUS1_ERR_IR_BYTE1_BMSK                       0xff
#define PMIO_PBS_CORE_SEQ_ERR_IR_STATUS1_ERR_IR_BYTE1_SHFT                        0x0

#define PMIO_PBS_CORE_SEQ_ERR_IR_STATUS2_ADDR                              (PBS_CORE_BASE      + 0x00000096)
#define PMIO_PBS_CORE_SEQ_ERR_IR_STATUS2_OFFS                              0x00000096
#define PMIO_PBS_CORE_SEQ_ERR_IR_STATUS2_RMSK                                    0xff
#define PMIO_PBS_CORE_SEQ_ERR_IR_STATUS2_IN(ctxt, pmic, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_SEQ_ERR_IR_STATUS2_ADDR, PMIO_PBS_CORE_SEQ_ERR_IR_STATUS2_RMSK, 0, val)
#define PMIO_PBS_CORE_SEQ_ERR_IR_STATUS2_INF(ctxt, pmic, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_SEQ_ERR_IR_STATUS2_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_SEQ_ERR_IR_STATUS2_INN(ctxt, pmic, val, len)\
        in_pmio(ctxt, pmic, PMIO_PBS_CORE_SEQ_ERR_IR_STATUS2_ADDR, val, len)
#define PMIO_PBS_CORE_SEQ_ERR_IR_STATUS2_ERR_IR_BYTE2_BMSK                       0xff
#define PMIO_PBS_CORE_SEQ_ERR_IR_STATUS2_ERR_IR_BYTE2_SHFT                        0x0

#define PMIO_PBS_CORE_SEQ_ERR_IR_STATUS3_ADDR                              (PBS_CORE_BASE      + 0x00000097)
#define PMIO_PBS_CORE_SEQ_ERR_IR_STATUS3_OFFS                              0x00000097
#define PMIO_PBS_CORE_SEQ_ERR_IR_STATUS3_RMSK                                    0xff
#define PMIO_PBS_CORE_SEQ_ERR_IR_STATUS3_IN(ctxt, pmic, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_SEQ_ERR_IR_STATUS3_ADDR, PMIO_PBS_CORE_SEQ_ERR_IR_STATUS3_RMSK, 0, val)
#define PMIO_PBS_CORE_SEQ_ERR_IR_STATUS3_INF(ctxt, pmic, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_SEQ_ERR_IR_STATUS3_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_SEQ_ERR_IR_STATUS3_INN(ctxt, pmic, val, len)\
        in_pmio(ctxt, pmic, PMIO_PBS_CORE_SEQ_ERR_IR_STATUS3_ADDR, val, len)
#define PMIO_PBS_CORE_SEQ_ERR_IR_STATUS3_ERR_IR_BYTE3_BMSK                       0xff
#define PMIO_PBS_CORE_SEQ_ERR_IR_STATUS3_ERR_IR_BYTE3_SHFT                        0x0

#define PMIO_PBS_CORE_TRIG_STATUS0_ADDR                                       (PBS_CORE_BASE      + 0x00000098)
#define PMIO_PBS_CORE_TRIG_STATUS0_OFFS                                       0x00000098
#define PMIO_PBS_CORE_TRIG_STATUS0_RMSK                                              0xf
#define PMIO_PBS_CORE_TRIG_STATUS0_IN(ctxt, pmic, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_TRIG_STATUS0_ADDR, PMIO_PBS_CORE_TRIG_STATUS0_RMSK, 0, val)
#define PMIO_PBS_CORE_TRIG_STATUS0_INF(ctxt, pmic, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_TRIG_STATUS0_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_TRIG_STATUS0_INN(ctxt, pmic, val, len)\
        in_pmio(ctxt, pmic, PMIO_PBS_CORE_TRIG_STATUS0_ADDR, val, len)
#define PMIO_PBS_CORE_TRIG_STATUS0_LAST_TRIG_COMP_BMSK                               0xf
#define PMIO_PBS_CORE_TRIG_STATUS0_LAST_TRIG_COMP_SHFT                               0x0

#define PMIO_PBS_CORE_TRIG_STATUS1_ADDR                                       (PBS_CORE_BASE      + 0x00000099)
#define PMIO_PBS_CORE_TRIG_STATUS1_OFFS                                       0x00000099
#define PMIO_PBS_CORE_TRIG_STATUS1_RMSK                                              0xf
#define PMIO_PBS_CORE_TRIG_STATUS1_IN(ctxt, pmic, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_TRIG_STATUS1_ADDR, PMIO_PBS_CORE_TRIG_STATUS1_RMSK, 0, val)
#define PMIO_PBS_CORE_TRIG_STATUS1_INF(ctxt, pmic, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_TRIG_STATUS1_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_TRIG_STATUS1_INN(ctxt, pmic, val, len)\
        in_pmio(ctxt, pmic, PMIO_PBS_CORE_TRIG_STATUS1_ADDR, val, len)
#define PMIO_PBS_CORE_TRIG_STATUS1_LAST_TRIG_NORMAL_BMSK                             0xf
#define PMIO_PBS_CORE_TRIG_STATUS1_LAST_TRIG_NORMAL_SHFT                             0x0

#define PMIO_PBS_CORE_TRIG_STATUS2_ADDR                                       (PBS_CORE_BASE      + 0x0000009a)
#define PMIO_PBS_CORE_TRIG_STATUS2_OFFS                                       0x0000009a
#define PMIO_PBS_CORE_TRIG_STATUS2_RMSK                                              0xf
#define PMIO_PBS_CORE_TRIG_STATUS2_IN(ctxt, pmic, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_TRIG_STATUS2_ADDR, PMIO_PBS_CORE_TRIG_STATUS2_RMSK, 0, val)
#define PMIO_PBS_CORE_TRIG_STATUS2_INF(ctxt, pmic, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_TRIG_STATUS2_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_TRIG_STATUS2_INN(ctxt, pmic, val, len)\
        in_pmio(ctxt, pmic, PMIO_PBS_CORE_TRIG_STATUS2_ADDR, val, len)
#define PMIO_PBS_CORE_TRIG_STATUS2_TRIG_IN_SERVICE_BMSK                              0xf
#define PMIO_PBS_CORE_TRIG_STATUS2_TRIG_IN_SERVICE_SHFT                              0x0

#define PMIO_PBS_CORE_TRIG_STATUS3_ADDR                                       (PBS_CORE_BASE      + 0x0000009b)
#define PMIO_PBS_CORE_TRIG_STATUS3_OFFS                                       0x0000009b
#define PMIO_PBS_CORE_TRIG_STATUS3_RMSK                                              0xf
#define PMIO_PBS_CORE_TRIG_STATUS3_IN(ctxt, pmic, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_TRIG_STATUS3_ADDR, PMIO_PBS_CORE_TRIG_STATUS3_RMSK, 0, val)
#define PMIO_PBS_CORE_TRIG_STATUS3_INF(ctxt, pmic, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_TRIG_STATUS3_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_TRIG_STATUS3_INN(ctxt, pmic, val, len)\
        in_pmio(ctxt, pmic, PMIO_PBS_CORE_TRIG_STATUS3_ADDR, val, len)
#define PMIO_PBS_CORE_TRIG_STATUS3_LAST_TRIG_ERROR_BMSK                              0xf
#define PMIO_PBS_CORE_TRIG_STATUS3_LAST_TRIG_ERROR_SHFT                              0x0

#define PMIO_PBS_CORE_TRIG_STATUS4_ADDR                                       (PBS_CORE_BASE      + 0x0000009c)
#define PMIO_PBS_CORE_TRIG_STATUS4_OFFS                                       0x0000009c
#define PMIO_PBS_CORE_TRIG_STATUS4_RMSK                                              0xf
#define PMIO_PBS_CORE_TRIG_STATUS4_IN(ctxt, pmic, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_TRIG_STATUS4_ADDR, PMIO_PBS_CORE_TRIG_STATUS4_RMSK, 0, val)
#define PMIO_PBS_CORE_TRIG_STATUS4_INF(ctxt, pmic, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_TRIG_STATUS4_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_TRIG_STATUS4_INN(ctxt, pmic, val, len)\
        in_pmio(ctxt, pmic, PMIO_PBS_CORE_TRIG_STATUS4_ADDR, val, len)
#define PMIO_PBS_CORE_TRIG_STATUS4_LAST_TRIG_ABORTED_BMSK                            0xf
#define PMIO_PBS_CORE_TRIG_STATUS4_LAST_TRIG_ABORTED_SHFT                            0x0

#define PMIO_PBS_CORE_PCE_ENABLE_DONE_STATUS_ADDR                             (PBS_CORE_BASE      + 0x0000009d)
#define PMIO_PBS_CORE_PCE_ENABLE_DONE_STATUS_OFFS                             0x0000009d
#define PMIO_PBS_CORE_PCE_ENABLE_DONE_STATUS_RMSK                                    0x1
#define PMIO_PBS_CORE_PCE_ENABLE_DONE_STATUS_IN(ctxt, pmic, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_PCE_ENABLE_DONE_STATUS_ADDR, PMIO_PBS_CORE_PCE_ENABLE_DONE_STATUS_RMSK, 0, val)
#define PMIO_PBS_CORE_PCE_ENABLE_DONE_STATUS_INF(ctxt, pmic, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_PCE_ENABLE_DONE_STATUS_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_PCE_ENABLE_DONE_STATUS_INN(ctxt, pmic, val, len)\
        in_pmio(ctxt, pmic, PMIO_PBS_CORE_PCE_ENABLE_DONE_STATUS_ADDR, val, len)
#define PMIO_PBS_CORE_PCE_ENABLE_DONE_STATUS_ENABLE_DONE_STATUS_BMSK                 0x1
#define PMIO_PBS_CORE_PCE_ENABLE_DONE_STATUS_ENABLE_DONE_STATUS_SHFT                 0x0

#define PMIO_PBS_CORE_RIF_ACCESS_DONE_STATUS_ADDR                             (PBS_CORE_BASE      + 0x0000009e)
#define PMIO_PBS_CORE_RIF_ACCESS_DONE_STATUS_OFFS                             0x0000009e
#define PMIO_PBS_CORE_RIF_ACCESS_DONE_STATUS_RMSK                                    0x1
#define PMIO_PBS_CORE_RIF_ACCESS_DONE_STATUS_IN(ctxt, pmic, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_RIF_ACCESS_DONE_STATUS_ADDR, PMIO_PBS_CORE_RIF_ACCESS_DONE_STATUS_RMSK, 0, val)
#define PMIO_PBS_CORE_RIF_ACCESS_DONE_STATUS_INF(ctxt, pmic, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_RIF_ACCESS_DONE_STATUS_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_RIF_ACCESS_DONE_STATUS_INN(ctxt, pmic, val, len)\
        in_pmio(ctxt, pmic, PMIO_PBS_CORE_RIF_ACCESS_DONE_STATUS_ADDR, val, len)
#define PMIO_PBS_CORE_RIF_ACCESS_DONE_STATUS_ACCESS_DONE_STATUS_BMSK                 0x1
#define PMIO_PBS_CORE_RIF_ACCESS_DONE_STATUS_ACCESS_DONE_STATUS_SHFT                 0x0

#define PMIO_PBS_CORE_RAM_CFG1_ADDR                                           (PBS_CORE_BASE      + 0x000000a0)
#define PMIO_PBS_CORE_RAM_CFG1_OFFS                                           0x000000a0
#define PMIO_PBS_CORE_RAM_CFG1_RMSK                                                 0xff
#define PMIO_PBS_CORE_RAM_CFG1_IN(ctxt, pmic, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_RAM_CFG1_ADDR, PMIO_PBS_CORE_RAM_CFG1_RMSK, 0, val)
#define PMIO_PBS_CORE_RAM_CFG1_INF(ctxt, pmic, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_RAM_CFG1_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_RAM_CFG1_INN(ctxt, pmic, val, len)\
        in_pmio(ctxt, pmic, PMIO_PBS_CORE_RAM_CFG1_ADDR, val, len)
#define PMIO_PBS_CORE_RAM_CFG1_SIZE_BMSK                                            0xff
#define PMIO_PBS_CORE_RAM_CFG1_SIZE_SHFT                                             0x0

#define PMIO_PBS_CORE_RAM_CFG2_ADDR                                           (PBS_CORE_BASE      + 0x000000a1)
#define PMIO_PBS_CORE_RAM_CFG2_OFFS                                           0x000000a1
#define PMIO_PBS_CORE_RAM_CFG2_RMSK                                                 0x60
#define PMIO_PBS_CORE_RAM_CFG2_IN(ctxt, pmic, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_RAM_CFG2_ADDR, PMIO_PBS_CORE_RAM_CFG2_RMSK, 0, val)
#define PMIO_PBS_CORE_RAM_CFG2_INF(ctxt, pmic, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_RAM_CFG2_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_RAM_CFG2_INN(ctxt, pmic, val, len)\
        in_pmio(ctxt, pmic, PMIO_PBS_CORE_RAM_CFG2_ADDR, val, len)
#define PMIO_PBS_CORE_RAM_CFG2_MTYPE_BMSK                                           0x60
#define PMIO_PBS_CORE_RAM_CFG2_MTYPE_SHFT                                            0x5

#define PMIO_PBS_CORE_NVM_CFG1_ADDR                                           (PBS_CORE_BASE      + 0x000000a2)
#define PMIO_PBS_CORE_NVM_CFG1_OFFS                                           0x000000a2
#define PMIO_PBS_CORE_NVM_CFG1_RMSK                                                 0xff
#define PMIO_PBS_CORE_NVM_CFG1_IN(ctxt, pmic, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_NVM_CFG1_ADDR, PMIO_PBS_CORE_NVM_CFG1_RMSK, 0, val)
#define PMIO_PBS_CORE_NVM_CFG1_INF(ctxt, pmic, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_NVM_CFG1_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_NVM_CFG1_INN(ctxt, pmic, val, len)\
        in_pmio(ctxt, pmic, PMIO_PBS_CORE_NVM_CFG1_ADDR, val, len)
#define PMIO_PBS_CORE_NVM_CFG1_SIZE_BMSK                                            0xff
#define PMIO_PBS_CORE_NVM_CFG1_SIZE_SHFT                                             0x0

#define PMIO_PBS_CORE_NVM_CFG2_ADDR                                           (PBS_CORE_BASE      + 0x000000a3)
#define PMIO_PBS_CORE_NVM_CFG2_OFFS                                           0x000000a3
#define PMIO_PBS_CORE_NVM_CFG2_RMSK                                                 0x73
#define PMIO_PBS_CORE_NVM_CFG2_IN(ctxt, pmic, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_NVM_CFG2_ADDR, PMIO_PBS_CORE_NVM_CFG2_RMSK, 0, val)
#define PMIO_PBS_CORE_NVM_CFG2_INF(ctxt, pmic, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_NVM_CFG2_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_NVM_CFG2_INN(ctxt, pmic, val, len)\
        in_pmio(ctxt, pmic, PMIO_PBS_CORE_NVM_CFG2_ADDR, val, len)
#define PMIO_PBS_CORE_NVM_CFG2_MTYPE_BMSK                                           0x60
#define PMIO_PBS_CORE_NVM_CFG2_MTYPE_SHFT                                            0x5
#define PMIO_PBS_CORE_NVM_CFG2_TYPE_BMSK                                            0x10
#define PMIO_PBS_CORE_NVM_CFG2_TYPE_SHFT                                             0x4
#define PMIO_PBS_CORE_NVM_CFG2_ACCESS_BMSK                                           0x3
#define PMIO_PBS_CORE_NVM_CFG2_ACCESS_SHFT                                           0x0

#define PMIO_PBS_CORE_PARAM_SETTINGS_ADDR                                     (PBS_CORE_BASE      + 0x000000a4)
#define PMIO_PBS_CORE_PARAM_SETTINGS_OFFS                                     0x000000a4
#define PMIO_PBS_CORE_PARAM_SETTINGS_RMSK                                           0x8f
#define PMIO_PBS_CORE_PARAM_SETTINGS_IN(ctxt, pmic, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_PARAM_SETTINGS_ADDR, PMIO_PBS_CORE_PARAM_SETTINGS_RMSK, 0, val)
#define PMIO_PBS_CORE_PARAM_SETTINGS_INF(ctxt, pmic, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_PARAM_SETTINGS_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_PARAM_SETTINGS_INN(ctxt, pmic, val, len)\
        in_pmio(ctxt, pmic, PMIO_PBS_CORE_PARAM_SETTINGS_ADDR, val, len)
#define PMIO_PBS_CORE_PARAM_SETTINGS_FAST_PBS_BMSK                                  0x80
#define PMIO_PBS_CORE_PARAM_SETTINGS_FAST_PBS_SHFT                                   0x7
#define PMIO_PBS_CORE_PARAM_SETTINGS_NUM_MACHINE_REGS_BMSK                           0xf
#define PMIO_PBS_CORE_PARAM_SETTINGS_NUM_MACHINE_REGS_SHFT                           0x0

#define PMIO_PBS_CORE_SEQ_R0_STATUS_ADDR                                      (PBS_CORE_BASE      + 0x000000b0)
#define PMIO_PBS_CORE_SEQ_R0_STATUS_OFFS                                      0x000000b0
#define PMIO_PBS_CORE_SEQ_R0_STATUS_RMSK                                            0xff
#define PMIO_PBS_CORE_SEQ_R0_STATUS_IN(ctxt, pmic, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_SEQ_R0_STATUS_ADDR, PMIO_PBS_CORE_SEQ_R0_STATUS_RMSK, 0, val)
#define PMIO_PBS_CORE_SEQ_R0_STATUS_INF(ctxt, pmic, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_SEQ_R0_STATUS_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_SEQ_R0_STATUS_INN(ctxt, pmic, val, len)\
        in_pmio(ctxt, pmic, PMIO_PBS_CORE_SEQ_R0_STATUS_ADDR, val, len)
#define PMIO_PBS_CORE_SEQ_R0_STATUS_R0_STATUS_BMSK                                  0xff
#define PMIO_PBS_CORE_SEQ_R0_STATUS_R0_STATUS_SHFT                                   0x0

#define PMIO_PBS_CORE_SEQ_R1_STATUS_ADDR                                      (PBS_CORE_BASE      + 0x000000b1)
#define PMIO_PBS_CORE_SEQ_R1_STATUS_OFFS                                      0x000000b1
#define PMIO_PBS_CORE_SEQ_R1_STATUS_RMSK                                            0xff
#define PMIO_PBS_CORE_SEQ_R1_STATUS_IN(ctxt, pmic, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_SEQ_R1_STATUS_ADDR, PMIO_PBS_CORE_SEQ_R1_STATUS_RMSK, 0, val)
#define PMIO_PBS_CORE_SEQ_R1_STATUS_INF(ctxt, pmic, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_SEQ_R1_STATUS_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_SEQ_R1_STATUS_INN(ctxt, pmic, val, len)\
        in_pmio(ctxt, pmic, PMIO_PBS_CORE_SEQ_R1_STATUS_ADDR, val, len)
#define PMIO_PBS_CORE_SEQ_R1_STATUS_R1_STATUS_BMSK                                  0xff
#define PMIO_PBS_CORE_SEQ_R1_STATUS_R1_STATUS_SHFT                                   0x0

#define PMIO_PBS_CORE_SEQ_R2_STATUS_ADDR                                      (PBS_CORE_BASE      + 0x000000b2)
#define PMIO_PBS_CORE_SEQ_R2_STATUS_OFFS                                      0x000000b2
#define PMIO_PBS_CORE_SEQ_R2_STATUS_RMSK                                            0xff
#define PMIO_PBS_CORE_SEQ_R2_STATUS_IN(ctxt, pmic, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_SEQ_R2_STATUS_ADDR, PMIO_PBS_CORE_SEQ_R2_STATUS_RMSK, 0, val)
#define PMIO_PBS_CORE_SEQ_R2_STATUS_INF(ctxt, pmic, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_SEQ_R2_STATUS_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_SEQ_R2_STATUS_INN(ctxt, pmic, val, len)\
        in_pmio(ctxt, pmic, PMIO_PBS_CORE_SEQ_R2_STATUS_ADDR, val, len)
#define PMIO_PBS_CORE_SEQ_R2_STATUS_R2_STATUS_BMSK                                  0xff
#define PMIO_PBS_CORE_SEQ_R2_STATUS_R2_STATUS_SHFT                                   0x0

#define PMIO_PBS_CORE_SEQ_R3_STATUS_ADDR                                      (PBS_CORE_BASE      + 0x000000b3)
#define PMIO_PBS_CORE_SEQ_R3_STATUS_OFFS                                      0x000000b3
#define PMIO_PBS_CORE_SEQ_R3_STATUS_RMSK                                            0xff
#define PMIO_PBS_CORE_SEQ_R3_STATUS_IN(ctxt, pmic, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_SEQ_R3_STATUS_ADDR, PMIO_PBS_CORE_SEQ_R3_STATUS_RMSK, 0, val)
#define PMIO_PBS_CORE_SEQ_R3_STATUS_INF(ctxt, pmic, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_SEQ_R3_STATUS_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_SEQ_R3_STATUS_INN(ctxt, pmic, val, len)\
        in_pmio(ctxt, pmic, PMIO_PBS_CORE_SEQ_R3_STATUS_ADDR, val, len)
#define PMIO_PBS_CORE_SEQ_R3_STATUS_R3_STATUS_BMSK                                  0xff
#define PMIO_PBS_CORE_SEQ_R3_STATUS_R3_STATUS_SHFT                                   0x0

#define PMIO_PBS_CORE_SEQ_R4_STATUS_ADDR                                      (PBS_CORE_BASE      + 0x000000b4)
#define PMIO_PBS_CORE_SEQ_R4_STATUS_OFFS                                      0x000000b4
#define PMIO_PBS_CORE_SEQ_R4_STATUS_RMSK                                            0xff
#define PMIO_PBS_CORE_SEQ_R4_STATUS_IN(ctxt, pmic, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_SEQ_R4_STATUS_ADDR, PMIO_PBS_CORE_SEQ_R4_STATUS_RMSK, 0, val)
#define PMIO_PBS_CORE_SEQ_R4_STATUS_INF(ctxt, pmic, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_SEQ_R4_STATUS_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_SEQ_R4_STATUS_INN(ctxt, pmic, val, len)\
        in_pmio(ctxt, pmic, PMIO_PBS_CORE_SEQ_R4_STATUS_ADDR, val, len)
#define PMIO_PBS_CORE_SEQ_R4_STATUS_R4_STATUS_BMSK                                  0xff
#define PMIO_PBS_CORE_SEQ_R4_STATUS_R4_STATUS_SHFT                                   0x0

#define PMIO_PBS_CORE_SEQ_R5_STATUS_ADDR                                      (PBS_CORE_BASE      + 0x000000b5)
#define PMIO_PBS_CORE_SEQ_R5_STATUS_OFFS                                      0x000000b5
#define PMIO_PBS_CORE_SEQ_R5_STATUS_RMSK                                            0xff
#define PMIO_PBS_CORE_SEQ_R5_STATUS_IN(ctxt, pmic, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_SEQ_R5_STATUS_ADDR, PMIO_PBS_CORE_SEQ_R5_STATUS_RMSK, 0, val)
#define PMIO_PBS_CORE_SEQ_R5_STATUS_INF(ctxt, pmic, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_SEQ_R5_STATUS_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_SEQ_R5_STATUS_INN(ctxt, pmic, val, len)\
        in_pmio(ctxt, pmic, PMIO_PBS_CORE_SEQ_R5_STATUS_ADDR, val, len)
#define PMIO_PBS_CORE_SEQ_R5_STATUS_R5_STATUS_BMSK                                  0xff
#define PMIO_PBS_CORE_SEQ_R5_STATUS_R5_STATUS_SHFT                                   0x0

#define PMIO_PBS_CORE_SEQ_R6_STATUS_ADDR                                      (PBS_CORE_BASE      + 0x000000b6)
#define PMIO_PBS_CORE_SEQ_R6_STATUS_OFFS                                      0x000000b6
#define PMIO_PBS_CORE_SEQ_R6_STATUS_RMSK                                            0xff
#define PMIO_PBS_CORE_SEQ_R6_STATUS_IN(ctxt, pmic, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_SEQ_R6_STATUS_ADDR, PMIO_PBS_CORE_SEQ_R6_STATUS_RMSK, 0, val)
#define PMIO_PBS_CORE_SEQ_R6_STATUS_INF(ctxt, pmic, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_SEQ_R6_STATUS_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_SEQ_R6_STATUS_INN(ctxt, pmic, val, len)\
        in_pmio(ctxt, pmic, PMIO_PBS_CORE_SEQ_R6_STATUS_ADDR, val, len)
#define PMIO_PBS_CORE_SEQ_R6_STATUS_R6_STATUS_BMSK                                  0xff
#define PMIO_PBS_CORE_SEQ_R6_STATUS_R6_STATUS_SHFT                                   0x0

#define PMIO_PBS_CORE_SEQ_R7_STATUS_ADDR                                      (PBS_CORE_BASE      + 0x000000b7)
#define PMIO_PBS_CORE_SEQ_R7_STATUS_OFFS                                      0x000000b7
#define PMIO_PBS_CORE_SEQ_R7_STATUS_RMSK                                            0xff
#define PMIO_PBS_CORE_SEQ_R7_STATUS_IN(ctxt, pmic, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_SEQ_R7_STATUS_ADDR, PMIO_PBS_CORE_SEQ_R7_STATUS_RMSK, 0, val)
#define PMIO_PBS_CORE_SEQ_R7_STATUS_INF(ctxt, pmic, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_SEQ_R7_STATUS_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_SEQ_R7_STATUS_INN(ctxt, pmic, val, len)\
        in_pmio(ctxt, pmic, PMIO_PBS_CORE_SEQ_R7_STATUS_ADDR, val, len)
#define PMIO_PBS_CORE_SEQ_R7_STATUS_R7_STATUS_BMSK                                  0xff
#define PMIO_PBS_CORE_SEQ_R7_STATUS_R7_STATUS_SHFT                                   0x0

#define PMIO_PBS_CORE_SEQ_R8_STATUS_ADDR                                      (PBS_CORE_BASE      + 0x000000b8)
#define PMIO_PBS_CORE_SEQ_R8_STATUS_OFFS                                      0x000000b8
#define PMIO_PBS_CORE_SEQ_R8_STATUS_RMSK                                            0xff
#define PMIO_PBS_CORE_SEQ_R8_STATUS_IN(ctxt, pmic, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_SEQ_R8_STATUS_ADDR, PMIO_PBS_CORE_SEQ_R8_STATUS_RMSK, 0, val)
#define PMIO_PBS_CORE_SEQ_R8_STATUS_INF(ctxt, pmic, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_SEQ_R8_STATUS_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_SEQ_R8_STATUS_INN(ctxt, pmic, val, len)\
        in_pmio(ctxt, pmic, PMIO_PBS_CORE_SEQ_R8_STATUS_ADDR, val, len)
#define PMIO_PBS_CORE_SEQ_R8_STATUS_R8_STATUS_BMSK                                  0xff
#define PMIO_PBS_CORE_SEQ_R8_STATUS_R8_STATUS_SHFT                                   0x0

#define PMIO_PBS_CORE_SEQ_R9_STATUS_ADDR                                      (PBS_CORE_BASE      + 0x000000b9)
#define PMIO_PBS_CORE_SEQ_R9_STATUS_OFFS                                      0x000000b9
#define PMIO_PBS_CORE_SEQ_R9_STATUS_RMSK                                            0xff
#define PMIO_PBS_CORE_SEQ_R9_STATUS_IN(ctxt, pmic, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_SEQ_R9_STATUS_ADDR, PMIO_PBS_CORE_SEQ_R9_STATUS_RMSK, 0, val)
#define PMIO_PBS_CORE_SEQ_R9_STATUS_INF(ctxt, pmic, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_SEQ_R9_STATUS_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_SEQ_R9_STATUS_INN(ctxt, pmic, val, len)\
        in_pmio(ctxt, pmic, PMIO_PBS_CORE_SEQ_R9_STATUS_ADDR, val, len)
#define PMIO_PBS_CORE_SEQ_R9_STATUS_R9_STATUS_BMSK                                  0xff
#define PMIO_PBS_CORE_SEQ_R9_STATUS_R9_STATUS_SHFT                                   0x0

#define PMIO_PBS_CORE_SEQ_RA_STATUS_ADDR                                      (PBS_CORE_BASE      + 0x000000ba)
#define PMIO_PBS_CORE_SEQ_RA_STATUS_OFFS                                      0x000000ba
#define PMIO_PBS_CORE_SEQ_RA_STATUS_RMSK                                            0xff
#define PMIO_PBS_CORE_SEQ_RA_STATUS_IN(ctxt, pmic, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_SEQ_RA_STATUS_ADDR, PMIO_PBS_CORE_SEQ_RA_STATUS_RMSK, 0, val)
#define PMIO_PBS_CORE_SEQ_RA_STATUS_INF(ctxt, pmic, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_SEQ_RA_STATUS_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_SEQ_RA_STATUS_INN(ctxt, pmic, val, len)\
        in_pmio(ctxt, pmic, PMIO_PBS_CORE_SEQ_RA_STATUS_ADDR, val, len)
#define PMIO_PBS_CORE_SEQ_RA_STATUS_RA_STATUS_BMSK                                  0xff
#define PMIO_PBS_CORE_SEQ_RA_STATUS_RA_STATUS_SHFT                                   0x0

#define PMIO_PBS_CORE_SEQ_RB_STATUS_ADDR                                      (PBS_CORE_BASE      + 0x000000bb)
#define PMIO_PBS_CORE_SEQ_RB_STATUS_OFFS                                      0x000000bb
#define PMIO_PBS_CORE_SEQ_RB_STATUS_RMSK                                            0xff
#define PMIO_PBS_CORE_SEQ_RB_STATUS_IN(ctxt, pmic, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_SEQ_RB_STATUS_ADDR, PMIO_PBS_CORE_SEQ_RB_STATUS_RMSK, 0, val)
#define PMIO_PBS_CORE_SEQ_RB_STATUS_INF(ctxt, pmic, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_SEQ_RB_STATUS_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_SEQ_RB_STATUS_INN(ctxt, pmic, val, len)\
        in_pmio(ctxt, pmic, PMIO_PBS_CORE_SEQ_RB_STATUS_ADDR, val, len)
#define PMIO_PBS_CORE_SEQ_RB_STATUS_RB_STATUS_BMSK                                  0xff
#define PMIO_PBS_CORE_SEQ_RB_STATUS_RB_STATUS_SHFT                                   0x0

#define PMIO_PBS_CORE_SEQ_RC_STATUS_ADDR                                      (PBS_CORE_BASE      + 0x000000bc)
#define PMIO_PBS_CORE_SEQ_RC_STATUS_OFFS                                      0x000000bc
#define PMIO_PBS_CORE_SEQ_RC_STATUS_RMSK                                            0xff
#define PMIO_PBS_CORE_SEQ_RC_STATUS_IN(ctxt, pmic, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_SEQ_RC_STATUS_ADDR, PMIO_PBS_CORE_SEQ_RC_STATUS_RMSK, 0, val)
#define PMIO_PBS_CORE_SEQ_RC_STATUS_INF(ctxt, pmic, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_SEQ_RC_STATUS_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_SEQ_RC_STATUS_INN(ctxt, pmic, val, len)\
        in_pmio(ctxt, pmic, PMIO_PBS_CORE_SEQ_RC_STATUS_ADDR, val, len)
#define PMIO_PBS_CORE_SEQ_RC_STATUS_RC_STATUS_BMSK                                  0xff
#define PMIO_PBS_CORE_SEQ_RC_STATUS_RC_STATUS_SHFT                                   0x0

#define PMIO_PBS_CORE_SEQ_RD_STATUS_ADDR                                      (PBS_CORE_BASE      + 0x000000bd)
#define PMIO_PBS_CORE_SEQ_RD_STATUS_OFFS                                      0x000000bd
#define PMIO_PBS_CORE_SEQ_RD_STATUS_RMSK                                            0xff
#define PMIO_PBS_CORE_SEQ_RD_STATUS_IN(ctxt, pmic, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_SEQ_RD_STATUS_ADDR, PMIO_PBS_CORE_SEQ_RD_STATUS_RMSK, 0, val)
#define PMIO_PBS_CORE_SEQ_RD_STATUS_INF(ctxt, pmic, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_SEQ_RD_STATUS_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_SEQ_RD_STATUS_INN(ctxt, pmic, val, len)\
        in_pmio(ctxt, pmic, PMIO_PBS_CORE_SEQ_RD_STATUS_ADDR, val, len)
#define PMIO_PBS_CORE_SEQ_RD_STATUS_RD_STATUS_BMSK                                  0xff
#define PMIO_PBS_CORE_SEQ_RD_STATUS_RD_STATUS_SHFT                                   0x0

#define PMIO_PBS_CORE_SEQ_RE_STATUS_ADDR                                      (PBS_CORE_BASE      + 0x000000be)
#define PMIO_PBS_CORE_SEQ_RE_STATUS_OFFS                                      0x000000be
#define PMIO_PBS_CORE_SEQ_RE_STATUS_RMSK                                            0xff
#define PMIO_PBS_CORE_SEQ_RE_STATUS_IN(ctxt, pmic, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_SEQ_RE_STATUS_ADDR, PMIO_PBS_CORE_SEQ_RE_STATUS_RMSK, 0, val)
#define PMIO_PBS_CORE_SEQ_RE_STATUS_INF(ctxt, pmic, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_SEQ_RE_STATUS_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_SEQ_RE_STATUS_INN(ctxt, pmic, val, len)\
        in_pmio(ctxt, pmic, PMIO_PBS_CORE_SEQ_RE_STATUS_ADDR, val, len)
#define PMIO_PBS_CORE_SEQ_RE_STATUS_RE_STATUS_BMSK                                  0xff
#define PMIO_PBS_CORE_SEQ_RE_STATUS_RE_STATUS_SHFT                                   0x0

#define PMIO_PBS_CORE_SEQ_RF_STATUS_ADDR                                      (PBS_CORE_BASE      + 0x000000bf)
#define PMIO_PBS_CORE_SEQ_RF_STATUS_OFFS                                      0x000000bf
#define PMIO_PBS_CORE_SEQ_RF_STATUS_RMSK                                            0xff
#define PMIO_PBS_CORE_SEQ_RF_STATUS_IN(ctxt, pmic, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_SEQ_RF_STATUS_ADDR, PMIO_PBS_CORE_SEQ_RF_STATUS_RMSK, 0, val)
#define PMIO_PBS_CORE_SEQ_RF_STATUS_INF(ctxt, pmic, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_SEQ_RF_STATUS_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_SEQ_RF_STATUS_INN(ctxt, pmic, val, len)\
        in_pmio(ctxt, pmic, PMIO_PBS_CORE_SEQ_RF_STATUS_ADDR, val, len)
#define PMIO_PBS_CORE_SEQ_RF_STATUS_RF_STATUS_BMSK                                  0xff
#define PMIO_PBS_CORE_SEQ_RF_STATUS_RF_STATUS_SHFT                                   0x0

#define PMIO_PBS_CORE_LOCKBIT_D1_ADDR                                         (PBS_CORE_BASE      + 0x000000d1)
#define PMIO_PBS_CORE_LOCKBIT_D1_OFFS                                         0x000000d1
#define PMIO_PBS_CORE_LOCKBIT_D1_RMSK                                                0x3
#define PMIO_PBS_CORE_LOCKBIT_D1_IN(ctxt, pmic, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_LOCKBIT_D1_ADDR, PMIO_PBS_CORE_LOCKBIT_D1_RMSK, 0, val)
#define PMIO_PBS_CORE_LOCKBIT_D1_INF(ctxt, pmic, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_LOCKBIT_D1_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_LOCKBIT_D1_INN(ctxt, pmic, val, len)\
        in_pmio(ctxt, pmic, PMIO_PBS_CORE_LOCKBIT_D1_ADDR, val, len)
#define PMIO_PBS_CORE_LOCKBIT_D1_OUT(ctxt, pmic, val)\
        out_pmio(ctxt, pmic, PMIO_PBS_CORE_LOCKBIT_D1_ADDR, val)
#define PMIO_PBS_CORE_LOCKBIT_D1_OUTF(ctxt, pmic, mask, shift, val)\
        out_pmio_field(ctxt, pmic, PMIO_PBS_CORE_LOCKBIT_D1_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_LOCKBIT_D1_OUTN(ctxt, pmic, val, len)\
        out_pmio_array(ctxt, pmic, PMIO_PBS_CORE_LOCKBIT_D1_ADDR, val, len)
#define PMIO_PBS_CORE_LOCKBIT_D1_LOCKBIT_D1_BMSK                                     0x3
#define PMIO_PBS_CORE_LOCKBIT_D1_LOCKBIT_D1_SHFT                                     0x0

#define PMIO_PBS_CORE_PERPH_RESET_CTL2_ADDR                                (PBS_CORE_BASE      + 0x000000d9)
#define PMIO_PBS_CORE_PERPH_RESET_CTL2_OFFS                                0x000000d9
#define PMIO_PBS_CORE_PERPH_RESET_CTL2_RMSK                                       0x1
#define PMIO_PBS_CORE_PERPH_RESET_CTL2_IN(ctxt, pmic, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_PERPH_RESET_CTL2_ADDR, PMIO_PBS_CORE_PERPH_RESET_CTL2_RMSK, 0, val)
#define PMIO_PBS_CORE_PERPH_RESET_CTL2_INF(ctxt, pmic, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_PERPH_RESET_CTL2_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_PERPH_RESET_CTL2_INN(ctxt, pmic, val, len)\
        in_pmio(ctxt, pmic, PMIO_PBS_CORE_PERPH_RESET_CTL2_ADDR, val, len)
#define PMIO_PBS_CORE_PERPH_RESET_CTL2_OUT(ctxt, pmic, val)\
        out_pmio(ctxt, pmic, PMIO_PBS_CORE_PERPH_RESET_CTL2_ADDR, val)
#define PMIO_PBS_CORE_PERPH_RESET_CTL2_OUTF(ctxt, pmic, mask, shift, val)\
        out_pmio_field(ctxt, pmic, PMIO_PBS_CORE_PERPH_RESET_CTL2_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_PERPH_RESET_CTL2_OUTN(ctxt, pmic, val, len)\
        out_pmio_array(ctxt, pmic, PMIO_PBS_CORE_PERPH_RESET_CTL2_ADDR, val, len)
#define PMIO_PBS_CORE_PERPH_RESET_CTL2_FOLLOW_GLOBAL_SOFT_RB_BMSK                 0x1
#define PMIO_PBS_CORE_PERPH_RESET_CTL2_FOLLOW_GLOBAL_SOFT_RB_SHFT                 0x0

#define PMIO_PBS_CORE_PERPH_RESET_CTL3_ADDR                                (PBS_CORE_BASE      + 0x000000da)
#define PMIO_PBS_CORE_PERPH_RESET_CTL3_OFFS                                0x000000da
#define PMIO_PBS_CORE_PERPH_RESET_CTL3_RMSK                                          0x7
#define PMIO_PBS_CORE_PERPH_RESET_CTL3_IN(ctxt, pmic, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_PERPH_RESET_CTL3_ADDR, PMIO_PBS_CORE_PERPH_RESET_CTL3_RMSK, 0, val)
#define PMIO_PBS_CORE_PERPH_RESET_CTL3_INF(ctxt, pmic, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_PERPH_RESET_CTL3_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_PERPH_RESET_CTL3_INN(ctxt, pmic, val, len)\
        in_pmio(ctxt, pmic, PMIO_PBS_CORE_PERPH_RESET_CTL3_ADDR, val, len)
#define PMIO_PBS_CORE_PERPH_RESET_CTL3_OUT(ctxt, pmic, val)\
        out_pmio(ctxt, pmic, PMIO_PBS_CORE_PERPH_RESET_CTL3_ADDR, val)
#define PMIO_PBS_CORE_PERPH_RESET_CTL3_OUTF(ctxt, pmic, mask, shift, val)\
        out_pmio_field(ctxt, pmic, PMIO_PBS_CORE_PERPH_RESET_CTL3_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_PERPH_RESET_CTL3_OUTN(ctxt, pmic, val, len)\
        out_pmio_array(ctxt, pmic, PMIO_PBS_CORE_PERPH_RESET_CTL3_ADDR, val, len)
#define PMIO_PBS_CORE_PERPH_RESET_CTL3_FOLLOW_WARM_RB_BMSK                        0x4
#define PMIO_PBS_CORE_PERPH_RESET_CTL3_FOLLOW_WARM_RB_SHFT                        0x2
#define PMIO_PBS_CORE_PERPH_RESET_CTL3_FOLLOW_SHUTDOWN1_RB_BMSK                   0x2
#define PMIO_PBS_CORE_PERPH_RESET_CTL3_FOLLOW_SHUTDOWN1_RB_SHFT                   0x1
#define PMIO_PBS_CORE_PERPH_RESET_CTL3_FOLLOW_SHUTDOWN2_RB_BMSK                   0x1
#define PMIO_PBS_CORE_PERPH_RESET_CTL3_FOLLOW_SHUTDOWN2_RB_SHFT                   0x0

#define PMIO_PBS_CORE_PERPH_RESET_CTL4_ADDR                                (PBS_CORE_BASE      + 0x000000db)
#define PMIO_PBS_CORE_PERPH_RESET_CTL4_OFFS                                0x000000db
#define PMIO_PBS_CORE_PERPH_RESET_CTL4_RMSK                                       0x1
#define PMIO_PBS_CORE_PERPH_RESET_CTL4_IN(ctxt, pmic, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_PERPH_RESET_CTL4_ADDR, PMIO_PBS_CORE_PERPH_RESET_CTL4_RMSK, 0, val)
#define PMIO_PBS_CORE_PERPH_RESET_CTL4_INF(ctxt, pmic, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_PERPH_RESET_CTL4_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_PERPH_RESET_CTL4_INN(ctxt, pmic, val, len)\
        in_pmio(ctxt, pmic, PMIO_PBS_CORE_PERPH_RESET_CTL4_ADDR, val, len)
#define PMIO_PBS_CORE_PERPH_RESET_CTL4_OUT(ctxt, pmic, val)\
        out_pmio(ctxt, pmic, PMIO_PBS_CORE_PERPH_RESET_CTL4_ADDR, val)
#define PMIO_PBS_CORE_PERPH_RESET_CTL4_OUTF(ctxt, pmic, mask, shift, val)\
        out_pmio_field(ctxt, pmic, PMIO_PBS_CORE_PERPH_RESET_CTL4_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_PERPH_RESET_CTL4_OUTN(ctxt, pmic, val, len)\
        out_pmio_array(ctxt, pmic, PMIO_PBS_CORE_PERPH_RESET_CTL4_ADDR, val, len)
#define PMIO_PBS_CORE_PERPH_RESET_CTL4_LOCAL_SOFT_RESET_BMSK                      0x1
#define PMIO_PBS_CORE_PERPH_RESET_CTL4_LOCAL_SOFT_RESET_SHFT                      0x0

#define PMIO_PBS_CORE_SEQ_DEBUG_CFG_ADDR                                      (PBS_CORE_BASE      + 0x000000e0)
#define PMIO_PBS_CORE_SEQ_DEBUG_CFG_OFFS                                      0x000000e0
#define PMIO_PBS_CORE_SEQ_DEBUG_CFG_RMSK                                             0x3
#define PMIO_PBS_CORE_SEQ_DEBUG_CFG_IN(ctxt, pmic, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_SEQ_DEBUG_CFG_ADDR, PMIO_PBS_CORE_SEQ_DEBUG_CFG_RMSK, 0, val)
#define PMIO_PBS_CORE_SEQ_DEBUG_CFG_INF(ctxt, pmic, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_SEQ_DEBUG_CFG_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_SEQ_DEBUG_CFG_INN(ctxt, pmic, val, len)\
        in_pmio(ctxt, pmic, PMIO_PBS_CORE_SEQ_DEBUG_CFG_ADDR, val, len)
#define PMIO_PBS_CORE_SEQ_DEBUG_CFG_OUT(ctxt, pmic, val)\
        out_pmio(ctxt, pmic, PMIO_PBS_CORE_SEQ_DEBUG_CFG_ADDR, val)
#define PMIO_PBS_CORE_SEQ_DEBUG_CFG_OUTF(ctxt, pmic, mask, shift, val)\
        out_pmio_field(ctxt, pmic, PMIO_PBS_CORE_SEQ_DEBUG_CFG_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_SEQ_DEBUG_CFG_OUTN(ctxt, pmic, val, len)\
        out_pmio_array(ctxt, pmic, PMIO_PBS_CORE_SEQ_DEBUG_CFG_ADDR, val, len)
#define PMIO_PBS_CORE_SEQ_DEBUG_CFG_DEBUG_DTEST_BMSK                                 0x2
#define PMIO_PBS_CORE_SEQ_DEBUG_CFG_DEBUG_DTEST_SHFT                                 0x1
#define PMIO_PBS_CORE_SEQ_DEBUG_CFG_DEBUG_EN_BMSK                                    0x1
#define PMIO_PBS_CORE_SEQ_DEBUG_CFG_DEBUG_EN_SHFT                                    0x0

#define PMIO_PBS_CORE_SEQ_DEBUG_NEXT_ADDR                                     (PBS_CORE_BASE      + 0x000000e1)
#define PMIO_PBS_CORE_SEQ_DEBUG_NEXT_OFFS                                     0x000000e1
#define PMIO_PBS_CORE_SEQ_DEBUG_NEXT_RMSK                                            0x1
#define PMIO_PBS_CORE_SEQ_DEBUG_NEXT_OUT(ctxt, pmic, val)\
        out_pmio(ctxt, pmic, PMIO_PBS_CORE_SEQ_DEBUG_NEXT_ADDR, val)
#define PMIO_PBS_CORE_SEQ_DEBUG_NEXT_OUTN(ctxt, pmic, val, len)\
        out_pmio_array(ctxt, pmic, PMIO_PBS_CORE_SEQ_DEBUG_NEXT_ADDR, val, len)
#define PMIO_PBS_CORE_SEQ_DEBUG_NEXT_NEXT_CMD_BMSK                                   0x1
#define PMIO_PBS_CORE_SEQ_DEBUG_NEXT_NEXT_CMD_SHFT                                   0x0

#define PMIO_PBS_CORE_OTP_CFG1_ADDR                                           (PBS_CORE_BASE      + 0x000000e2)
#define PMIO_PBS_CORE_OTP_CFG1_OFFS                                           0x000000e2
#define PMIO_PBS_CORE_OTP_CFG1_RMSK                                                  0xf
#define PMIO_PBS_CORE_OTP_CFG1_IN(ctxt, pmic, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_OTP_CFG1_ADDR, PMIO_PBS_CORE_OTP_CFG1_RMSK, 0, val)
#define PMIO_PBS_CORE_OTP_CFG1_INF(ctxt, pmic, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_OTP_CFG1_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_OTP_CFG1_INN(ctxt, pmic, val, len)\
        in_pmio(ctxt, pmic, PMIO_PBS_CORE_OTP_CFG1_ADDR, val, len)
#define PMIO_PBS_CORE_OTP_CFG1_OUT(ctxt, pmic, val)\
        out_pmio(ctxt, pmic, PMIO_PBS_CORE_OTP_CFG1_ADDR, val)
#define PMIO_PBS_CORE_OTP_CFG1_OUTF(ctxt, pmic, mask, shift, val)\
        out_pmio_field(ctxt, pmic, PMIO_PBS_CORE_OTP_CFG1_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_OTP_CFG1_OUTN(ctxt, pmic, val, len)\
        out_pmio_array(ctxt, pmic, PMIO_PBS_CORE_OTP_CFG1_ADDR, val, len)
#define PMIO_PBS_CORE_OTP_CFG1_VPP_SEL_BMSK                                          0x8
#define PMIO_PBS_CORE_OTP_CFG1_VPP_SEL_SHFT                                          0x3
#define PMIO_PBS_CORE_OTP_CFG1_PPROG_BMSK                                            0x4
#define PMIO_PBS_CORE_OTP_CFG1_PPROG_SHFT                                            0x2
#define PMIO_PBS_CORE_OTP_CFG1_PTM_BMSK                                              0x3
#define PMIO_PBS_CORE_OTP_CFG1_PTM_SHFT                                              0x0

#define PMIO_PBS_CORE_OTP_CFG2_ADDR                                           (PBS_CORE_BASE      + 0x000000e3)
#define PMIO_PBS_CORE_OTP_CFG2_OFFS                                           0x000000e3
#define PMIO_PBS_CORE_OTP_CFG2_RMSK                                                 0x1f
#define PMIO_PBS_CORE_OTP_CFG2_IN(ctxt, pmic, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_OTP_CFG2_ADDR, PMIO_PBS_CORE_OTP_CFG2_RMSK, 0, val)
#define PMIO_PBS_CORE_OTP_CFG2_INF(ctxt, pmic, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_OTP_CFG2_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_OTP_CFG2_INN(ctxt, pmic, val, len)\
        in_pmio(ctxt, pmic, PMIO_PBS_CORE_OTP_CFG2_ADDR, val, len)
#define PMIO_PBS_CORE_OTP_CFG2_OUT(ctxt, pmic, val)\
        out_pmio(ctxt, pmic, PMIO_PBS_CORE_OTP_CFG2_ADDR, val)
#define PMIO_PBS_CORE_OTP_CFG2_OUTF(ctxt, pmic, mask, shift, val)\
        out_pmio_field(ctxt, pmic, PMIO_PBS_CORE_OTP_CFG2_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_OTP_CFG2_OUTN(ctxt, pmic, val, len)\
        out_pmio_array(ctxt, pmic, PMIO_PBS_CORE_OTP_CFG2_ADDR, val, len)
#define PMIO_PBS_CORE_OTP_CFG2_MARGIN_RD_WS_BMSK                                    0x1f
#define PMIO_PBS_CORE_OTP_CFG2_MARGIN_RD_WS_SHFT                                     0x0

#define PMIO_PBS_CORE_TEST1_ADDR                                              (PBS_CORE_BASE      + 0x000000e4)
#define PMIO_PBS_CORE_TEST1_OFFS                                              0x000000e4
#define PMIO_PBS_CORE_TEST1_RMSK                                                    0xff
#define PMIO_PBS_CORE_TEST1_IN(ctxt, pmic, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_TEST1_ADDR, PMIO_PBS_CORE_TEST1_RMSK, 0, val)
#define PMIO_PBS_CORE_TEST1_INF(ctxt, pmic, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_TEST1_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_TEST1_INN(ctxt, pmic, val, len)\
        in_pmio(ctxt, pmic, PMIO_PBS_CORE_TEST1_ADDR, val, len)
#define PMIO_PBS_CORE_TEST1_OUT(ctxt, pmic, val)\
        out_pmio(ctxt, pmic, PMIO_PBS_CORE_TEST1_ADDR, val)
#define PMIO_PBS_CORE_TEST1_OUTF(ctxt, pmic, mask, shift, val)\
        out_pmio_field(ctxt, pmic, PMIO_PBS_CORE_TEST1_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_TEST1_OUTN(ctxt, pmic, val, len)\
        out_pmio_array(ctxt, pmic, PMIO_PBS_CORE_TEST1_ADDR, val, len)
#define PMIO_PBS_CORE_TEST1_DTEST4_EN_BMSK                                          0x80
#define PMIO_PBS_CORE_TEST1_DTEST4_EN_SHFT                                           0x7
#define PMIO_PBS_CORE_TEST1_DTEST3_EN_BMSK                                          0x40
#define PMIO_PBS_CORE_TEST1_DTEST3_EN_SHFT                                           0x6
#define PMIO_PBS_CORE_TEST1_DTEST2_EN_BMSK                                          0x20
#define PMIO_PBS_CORE_TEST1_DTEST2_EN_SHFT                                           0x5
#define PMIO_PBS_CORE_TEST1_DTEST1_EN_BMSK                                          0x10
#define PMIO_PBS_CORE_TEST1_DTEST1_EN_SHFT                                           0x4
#define PMIO_PBS_CORE_TEST1_DTEST_SEL_BMSK                                           0xf
#define PMIO_PBS_CORE_TEST1_DTEST_SEL_SHFT                                           0x0

#define PMIO_PBS_CORE_SEQ_DEBUG_DTEST_ADDR                                    (PBS_CORE_BASE      + 0x000000e5)
#define PMIO_PBS_CORE_SEQ_DEBUG_DTEST_OFFS                                    0x000000e5
#define PMIO_PBS_CORE_SEQ_DEBUG_DTEST_RMSK                                           0x7
#define PMIO_PBS_CORE_SEQ_DEBUG_DTEST_IN(ctxt, pmic, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_SEQ_DEBUG_DTEST_ADDR, PMIO_PBS_CORE_SEQ_DEBUG_DTEST_RMSK, 0, val)
#define PMIO_PBS_CORE_SEQ_DEBUG_DTEST_INF(ctxt, pmic, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_SEQ_DEBUG_DTEST_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_SEQ_DEBUG_DTEST_INN(ctxt, pmic, val, len)\
        in_pmio(ctxt, pmic, PMIO_PBS_CORE_SEQ_DEBUG_DTEST_ADDR, val, len)
#define PMIO_PBS_CORE_SEQ_DEBUG_DTEST_OUT(ctxt, pmic, val)\
        out_pmio(ctxt, pmic, PMIO_PBS_CORE_SEQ_DEBUG_DTEST_ADDR, val)
#define PMIO_PBS_CORE_SEQ_DEBUG_DTEST_OUTF(ctxt, pmic, mask, shift, val)\
        out_pmio_field(ctxt, pmic, PMIO_PBS_CORE_SEQ_DEBUG_DTEST_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_SEQ_DEBUG_DTEST_OUTN(ctxt, pmic, val, len)\
        out_pmio_array(ctxt, pmic, PMIO_PBS_CORE_SEQ_DEBUG_DTEST_ADDR, val, len)
#define PMIO_PBS_CORE_SEQ_DEBUG_DTEST_DEBUG_DTEST_SEL_BMSK                           0x7
#define PMIO_PBS_CORE_SEQ_DEBUG_DTEST_DEBUG_DTEST_SEL_SHFT                           0x0

#define PMIO_PBS_CORE_PMIC5_RAM_RD_WR_MARGIN_CFG_ADDR                         (PBS_CORE_BASE      + 0x000000e6)
#define PMIO_PBS_CORE_PMIC5_RAM_RD_WR_MARGIN_CFG_OFFS                         0x000000e6
#define PMIO_PBS_CORE_PMIC5_RAM_RD_WR_MARGIN_CFG_RMSK                               0x1f
#define PMIO_PBS_CORE_PMIC5_RAM_RD_WR_MARGIN_CFG_IN(ctxt, pmic, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_PMIC5_RAM_RD_WR_MARGIN_CFG_ADDR, PMIO_PBS_CORE_PMIC5_RAM_RD_WR_MARGIN_CFG_RMSK, 0, val)
#define PMIO_PBS_CORE_PMIC5_RAM_RD_WR_MARGIN_CFG_INF(ctxt, pmic, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_PMIC5_RAM_RD_WR_MARGIN_CFG_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_PMIC5_RAM_RD_WR_MARGIN_CFG_INN(ctxt, pmic, val, len)\
        in_pmio(ctxt, pmic, PMIO_PBS_CORE_PMIC5_RAM_RD_WR_MARGIN_CFG_ADDR, val, len)
#define PMIO_PBS_CORE_PMIC5_RAM_RD_WR_MARGIN_CFG_OUT(ctxt, pmic, val)\
        out_pmio(ctxt, pmic, PMIO_PBS_CORE_PMIC5_RAM_RD_WR_MARGIN_CFG_ADDR, val)
#define PMIO_PBS_CORE_PMIC5_RAM_RD_WR_MARGIN_CFG_OUTF(ctxt, pmic, mask, shift, val)\
        out_pmio_field(ctxt, pmic, PMIO_PBS_CORE_PMIC5_RAM_RD_WR_MARGIN_CFG_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_PMIC5_RAM_RD_WR_MARGIN_CFG_OUTN(ctxt, pmic, val, len)\
        out_pmio_array(ctxt, pmic, PMIO_PBS_CORE_PMIC5_RAM_RD_WR_MARGIN_CFG_ADDR, val, len)
#define PMIO_PBS_CORE_PMIC5_RAM_RD_WR_MARGIN_CFG_MS_BMSK                            0x1e
#define PMIO_PBS_CORE_PMIC5_RAM_RD_WR_MARGIN_CFG_MS_SHFT                             0x1
#define PMIO_PBS_CORE_PMIC5_RAM_RD_WR_MARGIN_CFG_MSE_BMSK                            0x1
#define PMIO_PBS_CORE_PMIC5_RAM_RD_WR_MARGIN_CFG_MSE_SHFT                            0x0

#define PMIO_PBS_CORE_PMIC5_OTP_CFG1_ADDR                                     (PBS_CORE_BASE      + 0x000000e7)
#define PMIO_PBS_CORE_PMIC5_OTP_CFG1_OFFS                                     0x000000e7
#define PMIO_PBS_CORE_PMIC5_OTP_CFG1_RMSK                                           0x7f
#define PMIO_PBS_CORE_PMIC5_OTP_CFG1_IN(ctxt, pmic, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_PMIC5_OTP_CFG1_ADDR, PMIO_PBS_CORE_PMIC5_OTP_CFG1_RMSK, 0, val)
#define PMIO_PBS_CORE_PMIC5_OTP_CFG1_INF(ctxt, pmic, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_PMIC5_OTP_CFG1_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_PMIC5_OTP_CFG1_INN(ctxt, pmic, val, len)\
        in_pmio(ctxt, pmic, PMIO_PBS_CORE_PMIC5_OTP_CFG1_ADDR, val, len)
#define PMIO_PBS_CORE_PMIC5_OTP_CFG1_OUT(ctxt, pmic, val)\
        out_pmio(ctxt, pmic, PMIO_PBS_CORE_PMIC5_OTP_CFG1_ADDR, val)
#define PMIO_PBS_CORE_PMIC5_OTP_CFG1_OUTF(ctxt, pmic, mask, shift, val)\
        out_pmio_field(ctxt, pmic, PMIO_PBS_CORE_PMIC5_OTP_CFG1_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_PMIC5_OTP_CFG1_OUTN(ctxt, pmic, val, len)\
        out_pmio_array(ctxt, pmic, PMIO_PBS_CORE_PMIC5_OTP_CFG1_ADDR, val, len)
#define PMIO_PBS_CORE_PMIC5_OTP_CFG1_PTM_BMSK                                       0x70
#define PMIO_PBS_CORE_PMIC5_OTP_CFG1_PTM_SHFT                                        0x4
#define PMIO_PBS_CORE_PMIC5_OTP_CFG1_PPROG_BMSK                                      0x8
#define PMIO_PBS_CORE_PMIC5_OTP_CFG1_PPROG_SHFT                                      0x3
#define PMIO_PBS_CORE_PMIC5_OTP_CFG1_PAS_BMSK                                        0x4
#define PMIO_PBS_CORE_PMIC5_OTP_CFG1_PAS_SHFT                                        0x2
#define PMIO_PBS_CORE_PMIC5_OTP_CFG1_PTC_BMSK                                        0x2
#define PMIO_PBS_CORE_PMIC5_OTP_CFG1_PTC_SHFT                                        0x1
#define PMIO_PBS_CORE_PMIC5_OTP_CFG1_PTR_BMSK                                        0x1
#define PMIO_PBS_CORE_PMIC5_OTP_CFG1_PTR_SHFT                                        0x0

#define PMIO_PBS_CORE_PMIC5_OTP_CFG2_ADDR                                     (PBS_CORE_BASE      + 0x000000e8)
#define PMIO_PBS_CORE_PMIC5_OTP_CFG2_OFFS                                     0x000000e8
#define PMIO_PBS_CORE_PMIC5_OTP_CFG2_RMSK                                            0x1
#define PMIO_PBS_CORE_PMIC5_OTP_CFG2_IN(ctxt, pmic, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_PMIC5_OTP_CFG2_ADDR, PMIO_PBS_CORE_PMIC5_OTP_CFG2_RMSK, 0, val)
#define PMIO_PBS_CORE_PMIC5_OTP_CFG2_INF(ctxt, pmic, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_PMIC5_OTP_CFG2_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_PMIC5_OTP_CFG2_INN(ctxt, pmic, val, len)\
        in_pmio(ctxt, pmic, PMIO_PBS_CORE_PMIC5_OTP_CFG2_ADDR, val, len)
#define PMIO_PBS_CORE_PMIC5_OTP_CFG2_OUT(ctxt, pmic, val)\
        out_pmio(ctxt, pmic, PMIO_PBS_CORE_PMIC5_OTP_CFG2_ADDR, val)
#define PMIO_PBS_CORE_PMIC5_OTP_CFG2_OUTF(ctxt, pmic, mask, shift, val)\
        out_pmio_field(ctxt, pmic, PMIO_PBS_CORE_PMIC5_OTP_CFG2_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_PMIC5_OTP_CFG2_OUTN(ctxt, pmic, val, len)\
        out_pmio_array(ctxt, pmic, PMIO_PBS_CORE_PMIC5_OTP_CFG2_ADDR, val, len)
#define PMIO_PBS_CORE_PMIC5_OTP_CFG2_PDSTB_BMSK                                      0x1
#define PMIO_PBS_CORE_PMIC5_OTP_CFG2_PDSTB_SHFT                                      0x0

#define PMIO_PBS_CORE_PMIC5_OTP_PCE_FORCE_ON_ADDR                             (PBS_CORE_BASE      + 0x000000e9)
#define PMIO_PBS_CORE_PMIC5_OTP_PCE_FORCE_ON_OFFS                             0x000000e9
#define PMIO_PBS_CORE_PMIC5_OTP_PCE_FORCE_ON_RMSK                                    0x1
#define PMIO_PBS_CORE_PMIC5_OTP_PCE_FORCE_ON_IN(ctxt, pmic, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_PMIC5_OTP_PCE_FORCE_ON_ADDR, PMIO_PBS_CORE_PMIC5_OTP_PCE_FORCE_ON_RMSK, 0, val)
#define PMIO_PBS_CORE_PMIC5_OTP_PCE_FORCE_ON_INF(ctxt, pmic, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_PMIC5_OTP_PCE_FORCE_ON_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_PMIC5_OTP_PCE_FORCE_ON_INN(ctxt, pmic, val, len)\
        in_pmio(ctxt, pmic, PMIO_PBS_CORE_PMIC5_OTP_PCE_FORCE_ON_ADDR, val, len)
#define PMIO_PBS_CORE_PMIC5_OTP_PCE_FORCE_ON_OUT(ctxt, pmic, val)\
        out_pmio(ctxt, pmic, PMIO_PBS_CORE_PMIC5_OTP_PCE_FORCE_ON_ADDR, val)
#define PMIO_PBS_CORE_PMIC5_OTP_PCE_FORCE_ON_OUTF(ctxt, pmic, mask, shift, val)\
        out_pmio_field(ctxt, pmic, PMIO_PBS_CORE_PMIC5_OTP_PCE_FORCE_ON_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_PMIC5_OTP_PCE_FORCE_ON_OUTN(ctxt, pmic, val, len)\
        out_pmio_array(ctxt, pmic, PMIO_PBS_CORE_PMIC5_OTP_PCE_FORCE_ON_ADDR, val, len)
#define PMIO_PBS_CORE_PMIC5_OTP_PCE_FORCE_ON_PCE_FORCE_ON_BMSK                       0x1
#define PMIO_PBS_CORE_PMIC5_OTP_PCE_FORCE_ON_PCE_FORCE_ON_SHFT                       0x0

#define PMIO_PBS_CORE_PMIC5_OTP_PCE_FORCE_OFF_ADDR                            (PBS_CORE_BASE      + 0x000000ea)
#define PMIO_PBS_CORE_PMIC5_OTP_PCE_FORCE_OFF_OFFS                            0x000000ea
#define PMIO_PBS_CORE_PMIC5_OTP_PCE_FORCE_OFF_RMSK                                   0x1
#define PMIO_PBS_CORE_PMIC5_OTP_PCE_FORCE_OFF_IN(ctxt, pmic, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_PMIC5_OTP_PCE_FORCE_OFF_ADDR, PMIO_PBS_CORE_PMIC5_OTP_PCE_FORCE_OFF_RMSK, 0, val)
#define PMIO_PBS_CORE_PMIC5_OTP_PCE_FORCE_OFF_INF(ctxt, pmic, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_PMIC5_OTP_PCE_FORCE_OFF_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_PMIC5_OTP_PCE_FORCE_OFF_INN(ctxt, pmic, val, len)\
        in_pmio(ctxt, pmic, PMIO_PBS_CORE_PMIC5_OTP_PCE_FORCE_OFF_ADDR, val, len)
#define PMIO_PBS_CORE_PMIC5_OTP_PCE_FORCE_OFF_OUT(ctxt, pmic, val)\
        out_pmio(ctxt, pmic, PMIO_PBS_CORE_PMIC5_OTP_PCE_FORCE_OFF_ADDR, val)
#define PMIO_PBS_CORE_PMIC5_OTP_PCE_FORCE_OFF_OUTF(ctxt, pmic, mask, shift, val)\
        out_pmio_field(ctxt, pmic, PMIO_PBS_CORE_PMIC5_OTP_PCE_FORCE_OFF_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_PMIC5_OTP_PCE_FORCE_OFF_OUTN(ctxt, pmic, val, len)\
        out_pmio_array(ctxt, pmic, PMIO_PBS_CORE_PMIC5_OTP_PCE_FORCE_OFF_ADDR, val, len)
#define PMIO_PBS_CORE_PMIC5_OTP_PCE_FORCE_OFF_PCE_FORCE_OFF_BMSK                     0x1
#define PMIO_PBS_CORE_PMIC5_OTP_PCE_FORCE_OFF_PCE_FORCE_OFF_SHFT                     0x0

#define PMIO_PBS_CORE_SEQ_BREAKPOINT_CFG_ADDR                                 (PBS_CORE_BASE      + 0x000000ec)
#define PMIO_PBS_CORE_SEQ_BREAKPOINT_CFG_OFFS                                 0x000000ec
#define PMIO_PBS_CORE_SEQ_BREAKPOINT_CFG_RMSK                                        0x1
#define PMIO_PBS_CORE_SEQ_BREAKPOINT_CFG_IN(ctxt, pmic, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_SEQ_BREAKPOINT_CFG_ADDR, PMIO_PBS_CORE_SEQ_BREAKPOINT_CFG_RMSK, 0, val)
#define PMIO_PBS_CORE_SEQ_BREAKPOINT_CFG_INF(ctxt, pmic, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_SEQ_BREAKPOINT_CFG_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_SEQ_BREAKPOINT_CFG_INN(ctxt, pmic, val, len)\
        in_pmio(ctxt, pmic, PMIO_PBS_CORE_SEQ_BREAKPOINT_CFG_ADDR, val, len)
#define PMIO_PBS_CORE_SEQ_BREAKPOINT_CFG_OUT(ctxt, pmic, val)\
        out_pmio(ctxt, pmic, PMIO_PBS_CORE_SEQ_BREAKPOINT_CFG_ADDR, val)
#define PMIO_PBS_CORE_SEQ_BREAKPOINT_CFG_OUTF(ctxt, pmic, mask, shift, val)\
        out_pmio_field(ctxt, pmic, PMIO_PBS_CORE_SEQ_BREAKPOINT_CFG_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_SEQ_BREAKPOINT_CFG_OUTN(ctxt, pmic, val, len)\
        out_pmio_array(ctxt, pmic, PMIO_PBS_CORE_SEQ_BREAKPOINT_CFG_ADDR, val, len)
#define PMIO_PBS_CORE_SEQ_BREAKPOINT_CFG_BP_EN_BMSK                                  0x1
#define PMIO_PBS_CORE_SEQ_BREAKPOINT_CFG_BP_EN_SHFT                                  0x0

#define PMIO_PBS_CORE_SEQ_BP_CTL_ADDR                                         (PBS_CORE_BASE      + 0x000000ed)
#define PMIO_PBS_CORE_SEQ_BP_CTL_OFFS                                         0x000000ed
#define PMIO_PBS_CORE_SEQ_BP_CTL_RMSK                                                0x1
#define PMIO_PBS_CORE_SEQ_BP_CTL_OUT(ctxt, pmic, val)\
        out_pmio(ctxt, pmic, PMIO_PBS_CORE_SEQ_BP_CTL_ADDR, val)
#define PMIO_PBS_CORE_SEQ_BP_CTL_OUTN(ctxt, pmic, val, len)\
        out_pmio_array(ctxt, pmic, PMIO_PBS_CORE_SEQ_BP_CTL_ADDR, val, len)
#define PMIO_PBS_CORE_SEQ_BP_CTL_BP_CONTINUE_BMSK                                    0x1
#define PMIO_PBS_CORE_SEQ_BP_CTL_BP_CONTINUE_SHFT                                    0x0

#define PMIO_PBS_CORE_SEQ_BP_ADDR_LSB_ADDR                                    (PBS_CORE_BASE      + 0x000000ee)
#define PMIO_PBS_CORE_SEQ_BP_ADDR_LSB_OFFS                                    0x000000ee
#define PMIO_PBS_CORE_SEQ_BP_ADDR_LSB_RMSK                                          0xff
#define PMIO_PBS_CORE_SEQ_BP_ADDR_LSB_IN(ctxt, pmic, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_SEQ_BP_ADDR_LSB_ADDR, PMIO_PBS_CORE_SEQ_BP_ADDR_LSB_RMSK, 0, val)
#define PMIO_PBS_CORE_SEQ_BP_ADDR_LSB_INF(ctxt, pmic, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_SEQ_BP_ADDR_LSB_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_SEQ_BP_ADDR_LSB_INN(ctxt, pmic, val, len)\
        in_pmio(ctxt, pmic, PMIO_PBS_CORE_SEQ_BP_ADDR_LSB_ADDR, val, len)
#define PMIO_PBS_CORE_SEQ_BP_ADDR_LSB_OUT(ctxt, pmic, val)\
        out_pmio(ctxt, pmic, PMIO_PBS_CORE_SEQ_BP_ADDR_LSB_ADDR, val)
#define PMIO_PBS_CORE_SEQ_BP_ADDR_LSB_OUTF(ctxt, pmic, mask, shift, val)\
        out_pmio_field(ctxt, pmic, PMIO_PBS_CORE_SEQ_BP_ADDR_LSB_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_SEQ_BP_ADDR_LSB_OUTN(ctxt, pmic, val, len)\
        out_pmio_array(ctxt, pmic, PMIO_PBS_CORE_SEQ_BP_ADDR_LSB_ADDR, val, len)
#define PMIO_PBS_CORE_SEQ_BP_ADDR_LSB_BP_ADDR_LSB_BMSK                              0xff
#define PMIO_PBS_CORE_SEQ_BP_ADDR_LSB_BP_ADDR_LSB_SHFT                               0x0

#define PMIO_PBS_CORE_SEQ_BP_ADDR_MSB_ADDR                                    (PBS_CORE_BASE      + 0x000000ef)
#define PMIO_PBS_CORE_SEQ_BP_ADDR_MSB_OFFS                                    0x000000ef
#define PMIO_PBS_CORE_SEQ_BP_ADDR_MSB_RMSK                                          0xff
#define PMIO_PBS_CORE_SEQ_BP_ADDR_MSB_IN(ctxt, pmic, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_SEQ_BP_ADDR_MSB_ADDR, PMIO_PBS_CORE_SEQ_BP_ADDR_MSB_RMSK, 0, val)
#define PMIO_PBS_CORE_SEQ_BP_ADDR_MSB_INF(ctxt, pmic, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBS_CORE_SEQ_BP_ADDR_MSB_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_SEQ_BP_ADDR_MSB_INN(ctxt, pmic, val, len)\
        in_pmio(ctxt, pmic, PMIO_PBS_CORE_SEQ_BP_ADDR_MSB_ADDR, val, len)
#define PMIO_PBS_CORE_SEQ_BP_ADDR_MSB_OUT(ctxt, pmic, val)\
        out_pmio(ctxt, pmic, PMIO_PBS_CORE_SEQ_BP_ADDR_MSB_ADDR, val)
#define PMIO_PBS_CORE_SEQ_BP_ADDR_MSB_OUTF(ctxt, pmic, mask, shift, val)\
        out_pmio_field(ctxt, pmic, PMIO_PBS_CORE_SEQ_BP_ADDR_MSB_ADDR, mask, shift, val)
#define PMIO_PBS_CORE_SEQ_BP_ADDR_MSB_OUTN(ctxt, pmic, val, len)\
        out_pmio_array(ctxt, pmic, PMIO_PBS_CORE_SEQ_BP_ADDR_MSB_ADDR, val, len)
#define PMIO_PBS_CORE_SEQ_BP_ADDR_MSB_BP_ADDR_MSB_BMSK                              0xff
#define PMIO_PBS_CORE_SEQ_BP_ADDR_MSB_BP_ADDR_MSB_SHFT                               0x0


#endif /* __PMIO_PBS_CORE_H__ */
