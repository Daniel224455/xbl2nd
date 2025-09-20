#ifndef __PDCHAL_HWIO_H__
#define __PDCHAL_HWIO_H__
/*
===========================================================================
*/
/**
  @file pdcHal_hwio.h
  @brief Auto-generated HWIO interface include file.

  Reference chip release:
    SC8180X (Poipu) [poipu_v1.0_p3q3r66_IPCAT]
 
  This file contains HWIO register definitions for the following modules:
    RPMH_PDC_DISPLAY_DISPLAY_PDC
    SDE_RSCC_RSCC_RSC


  Generation parameters: 
  { u'filename': u'pdcHal_hwio.h',
    u'header': u'#include "msmhwiobase.h"',
    u'module-filter-exclude': { },
    u'module-filter-include': { },
    u'modules': [u'RPMH_PDC_DISPLAY_DISPLAY_PDC', u'SDE_RSCC_RSCC_RSC']}
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
 * MODULE: RPMH_PDC_DISPLAY_DISPLAY_PDC
 *--------------------------------------------------------------------------*/

#define RPMH_PDC_DISPLAY_DISPLAY_PDC_REG_BASE                                                       (AOSS_BASE      + 0x00290000)
#define RPMH_PDC_DISPLAY_DISPLAY_PDC_REG_BASE_SIZE                                                  0x10000
#define RPMH_PDC_DISPLAY_DISPLAY_PDC_REG_BASE_USED                                                  0x56d4

#define HWIO_RPMH_PDC_DISPLAY_VOTE_CMP_DATA_DRVd_ADDR(d)                                            (RPMH_PDC_DISPLAY_DISPLAY_PDC_REG_BASE      + 0x00000000 + 0x10000 * (d))
#define HWIO_RPMH_PDC_DISPLAY_VOTE_CMP_DATA_DRVd_RMSK                                               0xffffffff
#define HWIO_RPMH_PDC_DISPLAY_VOTE_CMP_DATA_DRVd_MAXd                                                        0
#define HWIO_RPMH_PDC_DISPLAY_VOTE_CMP_DATA_DRVd_INI(d)        \
        in_dword_masked(HWIO_RPMH_PDC_DISPLAY_VOTE_CMP_DATA_DRVd_ADDR(d), HWIO_RPMH_PDC_DISPLAY_VOTE_CMP_DATA_DRVd_RMSK)
#define HWIO_RPMH_PDC_DISPLAY_VOTE_CMP_DATA_DRVd_INMI(d,mask)    \
        in_dword_masked(HWIO_RPMH_PDC_DISPLAY_VOTE_CMP_DATA_DRVd_ADDR(d), mask)
#define HWIO_RPMH_PDC_DISPLAY_VOTE_CMP_DATA_DRVd_OUTI(d,val)    \
        out_dword(HWIO_RPMH_PDC_DISPLAY_VOTE_CMP_DATA_DRVd_ADDR(d),val)
#define HWIO_RPMH_PDC_DISPLAY_VOTE_CMP_DATA_DRVd_OUTMI(d,mask,val) \
        out_dword_masked_ns(HWIO_RPMH_PDC_DISPLAY_VOTE_CMP_DATA_DRVd_ADDR(d),mask,val,HWIO_RPMH_PDC_DISPLAY_VOTE_CMP_DATA_DRVd_INI(d))
#define HWIO_RPMH_PDC_DISPLAY_VOTE_CMP_DATA_DRVd_VOTE_CMP_DATA_BMSK                                 0xffffffff
#define HWIO_RPMH_PDC_DISPLAY_VOTE_CMP_DATA_DRVd_VOTE_CMP_DATA_SHFT                                        0x0

#define HWIO_RPMH_PDC_DISPLAY_PDC_VERSION_DRVd_ADDR(d)                                              (RPMH_PDC_DISPLAY_DISPLAY_PDC_REG_BASE      + 0x00001000 + 0x10000 * (d))
#define HWIO_RPMH_PDC_DISPLAY_PDC_VERSION_DRVd_RMSK                                                   0xffffff
#define HWIO_RPMH_PDC_DISPLAY_PDC_VERSION_DRVd_MAXd                                                          0
#define HWIO_RPMH_PDC_DISPLAY_PDC_VERSION_DRVd_INI(d)        \
        in_dword_masked(HWIO_RPMH_PDC_DISPLAY_PDC_VERSION_DRVd_ADDR(d), HWIO_RPMH_PDC_DISPLAY_PDC_VERSION_DRVd_RMSK)
#define HWIO_RPMH_PDC_DISPLAY_PDC_VERSION_DRVd_INMI(d,mask)    \
        in_dword_masked(HWIO_RPMH_PDC_DISPLAY_PDC_VERSION_DRVd_ADDR(d), mask)
#define HWIO_RPMH_PDC_DISPLAY_PDC_VERSION_DRVd_MAJOR_VER_BMSK                                         0xff0000
#define HWIO_RPMH_PDC_DISPLAY_PDC_VERSION_DRVd_MAJOR_VER_SHFT                                             0x10
#define HWIO_RPMH_PDC_DISPLAY_PDC_VERSION_DRVd_MINOR_VER_BMSK                                           0xff00
#define HWIO_RPMH_PDC_DISPLAY_PDC_VERSION_DRVd_MINOR_VER_SHFT                                              0x8
#define HWIO_RPMH_PDC_DISPLAY_PDC_VERSION_DRVd_STEP_VER_BMSK                                              0xff
#define HWIO_RPMH_PDC_DISPLAY_PDC_VERSION_DRVd_STEP_VER_SHFT                                               0x0

#define HWIO_RPMH_PDC_DISPLAY_PDC_PARAM_RESOURCE_DRVd_ADDR(d)                                       (RPMH_PDC_DISPLAY_DISPLAY_PDC_REG_BASE      + 0x00001004 + 0x10000 * (d))
#define HWIO_RPMH_PDC_DISPLAY_PDC_PARAM_RESOURCE_DRVd_RMSK                                            0xffffff
#define HWIO_RPMH_PDC_DISPLAY_PDC_PARAM_RESOURCE_DRVd_MAXd                                                   0
#define HWIO_RPMH_PDC_DISPLAY_PDC_PARAM_RESOURCE_DRVd_INI(d)        \
        in_dword_masked(HWIO_RPMH_PDC_DISPLAY_PDC_PARAM_RESOURCE_DRVd_ADDR(d), HWIO_RPMH_PDC_DISPLAY_PDC_PARAM_RESOURCE_DRVd_RMSK)
#define HWIO_RPMH_PDC_DISPLAY_PDC_PARAM_RESOURCE_DRVd_INMI(d,mask)    \
        in_dword_masked(HWIO_RPMH_PDC_DISPLAY_PDC_PARAM_RESOURCE_DRVd_ADDR(d), mask)
#define HWIO_RPMH_PDC_DISPLAY_PDC_PARAM_RESOURCE_DRVd_BLK_NUM_TS_UNIT_BMSK                            0xff0000
#define HWIO_RPMH_PDC_DISPLAY_PDC_PARAM_RESOURCE_DRVd_BLK_NUM_TS_UNIT_SHFT                                0x10
#define HWIO_RPMH_PDC_DISPLAY_PDC_PARAM_RESOURCE_DRVd_BLK_NUM_PROFILING_UNIT_BMSK                       0xf000
#define HWIO_RPMH_PDC_DISPLAY_PDC_PARAM_RESOURCE_DRVd_BLK_NUM_PROFILING_UNIT_SHFT                          0xc
#define HWIO_RPMH_PDC_DISPLAY_PDC_PARAM_RESOURCE_DRVd_BLK_NUM_TCS_BMSK                                   0xf00
#define HWIO_RPMH_PDC_DISPLAY_PDC_PARAM_RESOURCE_DRVd_BLK_NUM_TCS_SHFT                                     0x8
#define HWIO_RPMH_PDC_DISPLAY_PDC_PARAM_RESOURCE_DRVd_BLK_NUM_TCS_CMDS_BMSK                               0xe0
#define HWIO_RPMH_PDC_DISPLAY_PDC_PARAM_RESOURCE_DRVd_BLK_NUM_TCS_CMDS_SHFT                                0x5
#define HWIO_RPMH_PDC_DISPLAY_PDC_PARAM_RESOURCE_DRVd_BLK_NUM_DRV_BMSK                                    0x1f
#define HWIO_RPMH_PDC_DISPLAY_PDC_PARAM_RESOURCE_DRVd_BLK_NUM_DRV_SHFT                                     0x0

#define HWIO_RPMH_PDC_DISPLAY_PDC_PARAM_SEQ_CONFIG_DRVd_ADDR(d)                                     (RPMH_PDC_DISPLAY_DISPLAY_PDC_REG_BASE      + 0x00001008 + 0x10000 * (d))
#define HWIO_RPMH_PDC_DISPLAY_PDC_PARAM_SEQ_CONFIG_DRVd_RMSK                                          0xff0fff
#define HWIO_RPMH_PDC_DISPLAY_PDC_PARAM_SEQ_CONFIG_DRVd_MAXd                                                 0
#define HWIO_RPMH_PDC_DISPLAY_PDC_PARAM_SEQ_CONFIG_DRVd_INI(d)        \
        in_dword_masked(HWIO_RPMH_PDC_DISPLAY_PDC_PARAM_SEQ_CONFIG_DRVd_ADDR(d), HWIO_RPMH_PDC_DISPLAY_PDC_PARAM_SEQ_CONFIG_DRVd_RMSK)
#define HWIO_RPMH_PDC_DISPLAY_PDC_PARAM_SEQ_CONFIG_DRVd_INMI(d,mask)    \
        in_dword_masked(HWIO_RPMH_PDC_DISPLAY_PDC_PARAM_SEQ_CONFIG_DRVd_ADDR(d), mask)
#define HWIO_RPMH_PDC_DISPLAY_PDC_PARAM_SEQ_CONFIG_DRVd_BLK_NUM_SEQ_CMD_WORDS_BMSK                    0xff0000
#define HWIO_RPMH_PDC_DISPLAY_PDC_PARAM_SEQ_CONFIG_DRVd_BLK_NUM_SEQ_CMD_WORDS_SHFT                        0x10
#define HWIO_RPMH_PDC_DISPLAY_PDC_PARAM_SEQ_CONFIG_DRVd_BLK_NUM_AOP_IRQ_BMSK                             0xf00
#define HWIO_RPMH_PDC_DISPLAY_PDC_PARAM_SEQ_CONFIG_DRVd_BLK_NUM_AOP_IRQ_SHFT                               0x8
#define HWIO_RPMH_PDC_DISPLAY_PDC_PARAM_SEQ_CONFIG_DRVd_BLK_NUM_ADDL_SEQ_PWR_CTRL_BMSK                    0xf0
#define HWIO_RPMH_PDC_DISPLAY_PDC_PARAM_SEQ_CONFIG_DRVd_BLK_NUM_ADDL_SEQ_PWR_CTRL_SHFT                     0x4
#define HWIO_RPMH_PDC_DISPLAY_PDC_PARAM_SEQ_CONFIG_DRVd_BLK_NUM_ADDL_SEQ_WAIT_EVNT_BMSK                    0xf
#define HWIO_RPMH_PDC_DISPLAY_PDC_PARAM_SEQ_CONFIG_DRVd_BLK_NUM_ADDL_SEQ_WAIT_EVNT_SHFT                    0x0

#define HWIO_RPMH_PDC_DISPLAY_PDC_PARAM_IRQ_CONFIG_DRVd_ADDR(d)                                     (RPMH_PDC_DISPLAY_DISPLAY_PDC_REG_BASE      + 0x0000100c + 0x10000 * (d))
#define HWIO_RPMH_PDC_DISPLAY_PDC_PARAM_IRQ_CONFIG_DRVd_RMSK                                         0x1ffffff
#define HWIO_RPMH_PDC_DISPLAY_PDC_PARAM_IRQ_CONFIG_DRVd_MAXd                                                 0
#define HWIO_RPMH_PDC_DISPLAY_PDC_PARAM_IRQ_CONFIG_DRVd_INI(d)        \
        in_dword_masked(HWIO_RPMH_PDC_DISPLAY_PDC_PARAM_IRQ_CONFIG_DRVd_ADDR(d), HWIO_RPMH_PDC_DISPLAY_PDC_PARAM_IRQ_CONFIG_DRVd_RMSK)
#define HWIO_RPMH_PDC_DISPLAY_PDC_PARAM_IRQ_CONFIG_DRVd_INMI(d,mask)    \
        in_dword_masked(HWIO_RPMH_PDC_DISPLAY_PDC_PARAM_IRQ_CONFIG_DRVd_ADDR(d), mask)
#define HWIO_RPMH_PDC_DISPLAY_PDC_PARAM_IRQ_CONFIG_DRVd_BLK_NUM_SEL_GP_IRQ_BMSK                      0x1ff0000
#define HWIO_RPMH_PDC_DISPLAY_PDC_PARAM_IRQ_CONFIG_DRVd_BLK_NUM_SEL_GP_IRQ_SHFT                           0x10
#define HWIO_RPMH_PDC_DISPLAY_PDC_PARAM_IRQ_CONFIG_DRVd_BLK_NUM_GP_IRQ_BMSK                             0xff00
#define HWIO_RPMH_PDC_DISPLAY_PDC_PARAM_IRQ_CONFIG_DRVd_BLK_NUM_GP_IRQ_SHFT                                0x8
#define HWIO_RPMH_PDC_DISPLAY_PDC_PARAM_IRQ_CONFIG_DRVd_BLK_NUM_IRQ_BMSK                                  0xff
#define HWIO_RPMH_PDC_DISPLAY_PDC_PARAM_IRQ_CONFIG_DRVd_BLK_NUM_IRQ_SHFT                                   0x0

#define HWIO_RPMH_PDC_DISPLAY_SEQ_BUSY_DRVd_ADDR(d)                                                 (RPMH_PDC_DISPLAY_DISPLAY_PDC_REG_BASE      + 0x00001020 + 0x10000 * (d))
#define HWIO_RPMH_PDC_DISPLAY_SEQ_BUSY_DRVd_RMSK                                                           0x1
#define HWIO_RPMH_PDC_DISPLAY_SEQ_BUSY_DRVd_MAXd                                                             0
#define HWIO_RPMH_PDC_DISPLAY_SEQ_BUSY_DRVd_INI(d)        \
        in_dword_masked(HWIO_RPMH_PDC_DISPLAY_SEQ_BUSY_DRVd_ADDR(d), HWIO_RPMH_PDC_DISPLAY_SEQ_BUSY_DRVd_RMSK)
#define HWIO_RPMH_PDC_DISPLAY_SEQ_BUSY_DRVd_INMI(d,mask)    \
        in_dword_masked(HWIO_RPMH_PDC_DISPLAY_SEQ_BUSY_DRVd_ADDR(d), mask)
#define HWIO_RPMH_PDC_DISPLAY_SEQ_BUSY_DRVd_BUSY_STAT_BMSK                                                 0x1
#define HWIO_RPMH_PDC_DISPLAY_SEQ_BUSY_DRVd_BUSY_STAT_SHFT                                                 0x0

#define HWIO_RPMH_PDC_DISPLAY_SEQ_DBG_PROGRAM_COUNTER_DRVd_ADDR(d)                                  (RPMH_PDC_DISPLAY_DISPLAY_PDC_REG_BASE      + 0x00001024 + 0x10000 * (d))
#define HWIO_RPMH_PDC_DISPLAY_SEQ_DBG_PROGRAM_COUNTER_DRVd_RMSK                                           0x7f
#define HWIO_RPMH_PDC_DISPLAY_SEQ_DBG_PROGRAM_COUNTER_DRVd_MAXd                                              0
#define HWIO_RPMH_PDC_DISPLAY_SEQ_DBG_PROGRAM_COUNTER_DRVd_INI(d)        \
        in_dword_masked(HWIO_RPMH_PDC_DISPLAY_SEQ_DBG_PROGRAM_COUNTER_DRVd_ADDR(d), HWIO_RPMH_PDC_DISPLAY_SEQ_DBG_PROGRAM_COUNTER_DRVd_RMSK)
#define HWIO_RPMH_PDC_DISPLAY_SEQ_DBG_PROGRAM_COUNTER_DRVd_INMI(d,mask)    \
        in_dword_masked(HWIO_RPMH_PDC_DISPLAY_SEQ_DBG_PROGRAM_COUNTER_DRVd_ADDR(d), mask)
#define HWIO_RPMH_PDC_DISPLAY_SEQ_DBG_PROGRAM_COUNTER_DRVd_CURRENT_INSTR_ADDR_BMSK                        0x7f
#define HWIO_RPMH_PDC_DISPLAY_SEQ_DBG_PROGRAM_COUNTER_DRVd_CURRENT_INSTR_ADDR_SHFT                         0x0

#define HWIO_RPMH_PDC_DISPLAY_SEQ_DBG_STAT_DRVd_ADDR(d)                                             (RPMH_PDC_DISPLAY_DISPLAY_PDC_REG_BASE      + 0x00001028 + 0x10000 * (d))
#define HWIO_RPMH_PDC_DISPLAY_SEQ_DBG_STAT_DRVd_RMSK                                                       0x1
#define HWIO_RPMH_PDC_DISPLAY_SEQ_DBG_STAT_DRVd_MAXd                                                         0
#define HWIO_RPMH_PDC_DISPLAY_SEQ_DBG_STAT_DRVd_INI(d)        \
        in_dword_masked(HWIO_RPMH_PDC_DISPLAY_SEQ_DBG_STAT_DRVd_ADDR(d), HWIO_RPMH_PDC_DISPLAY_SEQ_DBG_STAT_DRVd_RMSK)
#define HWIO_RPMH_PDC_DISPLAY_SEQ_DBG_STAT_DRVd_INMI(d,mask)    \
        in_dword_masked(HWIO_RPMH_PDC_DISPLAY_SEQ_DBG_STAT_DRVd_ADDR(d), mask)
#define HWIO_RPMH_PDC_DISPLAY_SEQ_DBG_STAT_DRVd_DBG_STEPPING_STATUS_BMSK                                   0x1
#define HWIO_RPMH_PDC_DISPLAY_SEQ_DBG_STAT_DRVd_DBG_STEPPING_STATUS_SHFT                                   0x0

#define HWIO_RPMH_PDC_DISPLAY_SEQ_DBG_WAIT_EVNT_DRVd_ADDR(d)                                        (RPMH_PDC_DISPLAY_DISPLAY_PDC_REG_BASE      + 0x0000102c + 0x10000 * (d))
#define HWIO_RPMH_PDC_DISPLAY_SEQ_DBG_WAIT_EVNT_DRVd_RMSK                                           0xffffffff
#define HWIO_RPMH_PDC_DISPLAY_SEQ_DBG_WAIT_EVNT_DRVd_MAXd                                                    0
#define HWIO_RPMH_PDC_DISPLAY_SEQ_DBG_WAIT_EVNT_DRVd_INI(d)        \
        in_dword_masked(HWIO_RPMH_PDC_DISPLAY_SEQ_DBG_WAIT_EVNT_DRVd_ADDR(d), HWIO_RPMH_PDC_DISPLAY_SEQ_DBG_WAIT_EVNT_DRVd_RMSK)
#define HWIO_RPMH_PDC_DISPLAY_SEQ_DBG_WAIT_EVNT_DRVd_INMI(d,mask)    \
        in_dword_masked(HWIO_RPMH_PDC_DISPLAY_SEQ_DBG_WAIT_EVNT_DRVd_ADDR(d), mask)
#define HWIO_RPMH_PDC_DISPLAY_SEQ_DBG_WAIT_EVNT_DRVd_SEQ_DBG_WAIT_EVNT_BMSK                         0xffffffff
#define HWIO_RPMH_PDC_DISPLAY_SEQ_DBG_WAIT_EVNT_DRVd_SEQ_DBG_WAIT_EVNT_SHFT                                0x0

#define HWIO_RPMH_PDC_DISPLAY_PDC_MODE_STATUS_DRVd_ADDR(d)                                          (RPMH_PDC_DISPLAY_DISPLAY_PDC_REG_BASE      + 0x00001030 + 0x10000 * (d))
#define HWIO_RPMH_PDC_DISPLAY_PDC_MODE_STATUS_DRVd_RMSK                                                    0x1
#define HWIO_RPMH_PDC_DISPLAY_PDC_MODE_STATUS_DRVd_MAXd                                                      0
#define HWIO_RPMH_PDC_DISPLAY_PDC_MODE_STATUS_DRVd_INI(d)        \
        in_dword_masked(HWIO_RPMH_PDC_DISPLAY_PDC_MODE_STATUS_DRVd_ADDR(d), HWIO_RPMH_PDC_DISPLAY_PDC_MODE_STATUS_DRVd_RMSK)
#define HWIO_RPMH_PDC_DISPLAY_PDC_MODE_STATUS_DRVd_INMI(d,mask)    \
        in_dword_masked(HWIO_RPMH_PDC_DISPLAY_PDC_MODE_STATUS_DRVd_ADDR(d), mask)
#define HWIO_RPMH_PDC_DISPLAY_PDC_MODE_STATUS_DRVd_PDC_MODE_STATUS_BMSK                                    0x1
#define HWIO_RPMH_PDC_DISPLAY_PDC_MODE_STATUS_DRVd_PDC_MODE_STATUS_SHFT                                    0x0

#define HWIO_RPMH_PDC_DISPLAY_DBG_EPCB_RX_ADDRd_ADDR(d)                                             (RPMH_PDC_DISPLAY_DISPLAY_PDC_REG_BASE      + 0x00001034 + 0x10000 * (d))
#define HWIO_RPMH_PDC_DISPLAY_DBG_EPCB_RX_ADDRd_RMSK                                                0xffffffff
#define HWIO_RPMH_PDC_DISPLAY_DBG_EPCB_RX_ADDRd_MAXd                                                         0
#define HWIO_RPMH_PDC_DISPLAY_DBG_EPCB_RX_ADDRd_INI(d)        \
        in_dword_masked(HWIO_RPMH_PDC_DISPLAY_DBG_EPCB_RX_ADDRd_ADDR(d), HWIO_RPMH_PDC_DISPLAY_DBG_EPCB_RX_ADDRd_RMSK)
#define HWIO_RPMH_PDC_DISPLAY_DBG_EPCB_RX_ADDRd_INMI(d,mask)    \
        in_dword_masked(HWIO_RPMH_PDC_DISPLAY_DBG_EPCB_RX_ADDRd_ADDR(d), mask)
#define HWIO_RPMH_PDC_DISPLAY_DBG_EPCB_RX_ADDRd_EPCB_DEBUG_RX_ADDR_BMSK                             0xffffffff
#define HWIO_RPMH_PDC_DISPLAY_DBG_EPCB_RX_ADDRd_EPCB_DEBUG_RX_ADDR_SHFT                                    0x0

#define HWIO_RPMH_PDC_DISPLAY_DBG_EPCB_RX_DATAd_ADDR(d)                                             (RPMH_PDC_DISPLAY_DISPLAY_PDC_REG_BASE      + 0x00001038 + 0x10000 * (d))
#define HWIO_RPMH_PDC_DISPLAY_DBG_EPCB_RX_DATAd_RMSK                                                0xffffffff
#define HWIO_RPMH_PDC_DISPLAY_DBG_EPCB_RX_DATAd_MAXd                                                         0
#define HWIO_RPMH_PDC_DISPLAY_DBG_EPCB_RX_DATAd_INI(d)        \
        in_dword_masked(HWIO_RPMH_PDC_DISPLAY_DBG_EPCB_RX_DATAd_ADDR(d), HWIO_RPMH_PDC_DISPLAY_DBG_EPCB_RX_DATAd_RMSK)
#define HWIO_RPMH_PDC_DISPLAY_DBG_EPCB_RX_DATAd_INMI(d,mask)    \
        in_dword_masked(HWIO_RPMH_PDC_DISPLAY_DBG_EPCB_RX_DATAd_ADDR(d), mask)
#define HWIO_RPMH_PDC_DISPLAY_DBG_EPCB_RX_DATAd_EPCB_DEBUG_RX_DATA_BMSK                             0xffffffff
#define HWIO_RPMH_PDC_DISPLAY_DBG_EPCB_RX_DATAd_EPCB_DEBUG_RX_DATA_SHFT                                    0x0

#define HWIO_RPMH_PDC_DISPLAY_PROFILING_UNITpf_DRVd_OUTPUT_ADDR(d,pf)                               (RPMH_PDC_DISPLAY_DISPLAY_PDC_REG_BASE      + 0x00001200 + 0x14 * (pf) + 0x10000 * (d))
#define HWIO_RPMH_PDC_DISPLAY_PROFILING_UNITpf_DRVd_OUTPUT_RMSK                                          0x101
#define HWIO_RPMH_PDC_DISPLAY_PROFILING_UNITpf_DRVd_OUTPUT_MAXd                                              0
#define HWIO_RPMH_PDC_DISPLAY_PROFILING_UNITpf_DRVd_OUTPUT_MAXpf                                             4
#define HWIO_RPMH_PDC_DISPLAY_PROFILING_UNITpf_DRVd_OUTPUT_INI2(d,pf)        \
        in_dword_masked(HWIO_RPMH_PDC_DISPLAY_PROFILING_UNITpf_DRVd_OUTPUT_ADDR(d,pf), HWIO_RPMH_PDC_DISPLAY_PROFILING_UNITpf_DRVd_OUTPUT_RMSK)
#define HWIO_RPMH_PDC_DISPLAY_PROFILING_UNITpf_DRVd_OUTPUT_INMI2(d,pf,mask)    \
        in_dword_masked(HWIO_RPMH_PDC_DISPLAY_PROFILING_UNITpf_DRVd_OUTPUT_ADDR(d,pf), mask)
#define HWIO_RPMH_PDC_DISPLAY_PROFILING_UNITpf_DRVd_OUTPUT_PROFILING_OVERFLOW_BMSK                       0x100
#define HWIO_RPMH_PDC_DISPLAY_PROFILING_UNITpf_DRVd_OUTPUT_PROFILING_OVERFLOW_SHFT                         0x8
#define HWIO_RPMH_PDC_DISPLAY_PROFILING_UNITpf_DRVd_OUTPUT_PROFILING_TS_VALID_BMSK                         0x1
#define HWIO_RPMH_PDC_DISPLAY_PROFILING_UNITpf_DRVd_OUTPUT_PROFILING_TS_VALID_SHFT                         0x0

#define HWIO_RPMH_PDC_DISPLAY_PROFILING_UNITpf_DRVd_TIMESTAMP_LO_ADDR(d,pf)                         (RPMH_PDC_DISPLAY_DISPLAY_PDC_REG_BASE      + 0x00001204 + 0x14 * (pf) + 0x10000 * (d))
#define HWIO_RPMH_PDC_DISPLAY_PROFILING_UNITpf_DRVd_TIMESTAMP_LO_RMSK                               0xffffffff
#define HWIO_RPMH_PDC_DISPLAY_PROFILING_UNITpf_DRVd_TIMESTAMP_LO_MAXd                                        0
#define HWIO_RPMH_PDC_DISPLAY_PROFILING_UNITpf_DRVd_TIMESTAMP_LO_MAXpf                                       4
#define HWIO_RPMH_PDC_DISPLAY_PROFILING_UNITpf_DRVd_TIMESTAMP_LO_INI2(d,pf)        \
        in_dword_masked(HWIO_RPMH_PDC_DISPLAY_PROFILING_UNITpf_DRVd_TIMESTAMP_LO_ADDR(d,pf), HWIO_RPMH_PDC_DISPLAY_PROFILING_UNITpf_DRVd_TIMESTAMP_LO_RMSK)
#define HWIO_RPMH_PDC_DISPLAY_PROFILING_UNITpf_DRVd_TIMESTAMP_LO_INMI2(d,pf,mask)    \
        in_dword_masked(HWIO_RPMH_PDC_DISPLAY_PROFILING_UNITpf_DRVd_TIMESTAMP_LO_ADDR(d,pf), mask)
#define HWIO_RPMH_PDC_DISPLAY_PROFILING_UNITpf_DRVd_TIMESTAMP_LO_PROFILING_TS_DATA_LO_BMSK          0xffffffff
#define HWIO_RPMH_PDC_DISPLAY_PROFILING_UNITpf_DRVd_TIMESTAMP_LO_PROFILING_TS_DATA_LO_SHFT                 0x0

#define HWIO_RPMH_PDC_DISPLAY_PROFILING_UNITpf_DRVd_TIMESTAMP_HI_ADDR(d,pf)                         (RPMH_PDC_DISPLAY_DISPLAY_PDC_REG_BASE      + 0x00001208 + 0x14 * (pf) + 0x10000 * (d))
#define HWIO_RPMH_PDC_DISPLAY_PROFILING_UNITpf_DRVd_TIMESTAMP_HI_RMSK                                 0xffffff
#define HWIO_RPMH_PDC_DISPLAY_PROFILING_UNITpf_DRVd_TIMESTAMP_HI_MAXd                                        0
#define HWIO_RPMH_PDC_DISPLAY_PROFILING_UNITpf_DRVd_TIMESTAMP_HI_MAXpf                                       4
#define HWIO_RPMH_PDC_DISPLAY_PROFILING_UNITpf_DRVd_TIMESTAMP_HI_INI2(d,pf)        \
        in_dword_masked(HWIO_RPMH_PDC_DISPLAY_PROFILING_UNITpf_DRVd_TIMESTAMP_HI_ADDR(d,pf), HWIO_RPMH_PDC_DISPLAY_PROFILING_UNITpf_DRVd_TIMESTAMP_HI_RMSK)
#define HWIO_RPMH_PDC_DISPLAY_PROFILING_UNITpf_DRVd_TIMESTAMP_HI_INMI2(d,pf,mask)    \
        in_dword_masked(HWIO_RPMH_PDC_DISPLAY_PROFILING_UNITpf_DRVd_TIMESTAMP_HI_ADDR(d,pf), mask)
#define HWIO_RPMH_PDC_DISPLAY_PROFILING_UNITpf_DRVd_TIMESTAMP_HI_PROFILING_TS_DATA_HI_BMSK            0xffffff
#define HWIO_RPMH_PDC_DISPLAY_PROFILING_UNITpf_DRVd_TIMESTAMP_HI_PROFILING_TS_DATA_HI_SHFT                 0x0

#define HWIO_RPMH_PDC_DISPLAY_TCSt_DRVd_STATUS_ADDR(d,t)                                            (RPMH_PDC_DISPLAY_DISPLAY_PDC_REG_BASE      + 0x00001300 + 0xC8 * (t) + 0x10000 * (d))
#define HWIO_RPMH_PDC_DISPLAY_TCSt_DRVd_STATUS_RMSK                                                        0x1
#define HWIO_RPMH_PDC_DISPLAY_TCSt_DRVd_STATUS_MAXd                                                          0
#define HWIO_RPMH_PDC_DISPLAY_TCSt_DRVd_STATUS_MAXt                                                          2
#define HWIO_RPMH_PDC_DISPLAY_TCSt_DRVd_STATUS_INI2(d,t)        \
        in_dword_masked(HWIO_RPMH_PDC_DISPLAY_TCSt_DRVd_STATUS_ADDR(d,t), HWIO_RPMH_PDC_DISPLAY_TCSt_DRVd_STATUS_RMSK)
#define HWIO_RPMH_PDC_DISPLAY_TCSt_DRVd_STATUS_INMI2(d,t,mask)    \
        in_dword_masked(HWIO_RPMH_PDC_DISPLAY_TCSt_DRVd_STATUS_ADDR(d,t), mask)
#define HWIO_RPMH_PDC_DISPLAY_TCSt_DRVd_STATUS_CONTROLLER_IDLE_BMSK                                        0x1
#define HWIO_RPMH_PDC_DISPLAY_TCSt_DRVd_STATUS_CONTROLLER_IDLE_SHFT                                        0x0

#define HWIO_RPMH_PDC_DISPLAY_TCSt_CMDn_DRVd_STATUS_ADDR(d,t,n)                                     (RPMH_PDC_DISPLAY_DISPLAY_PDC_REG_BASE      + 0x00001304 + 0xC8 * (t) + 0x8 * (n) + 0x10000 * (d))
#define HWIO_RPMH_PDC_DISPLAY_TCSt_CMDn_DRVd_STATUS_RMSK                                               0x10101
#define HWIO_RPMH_PDC_DISPLAY_TCSt_CMDn_DRVd_STATUS_MAXd                                                     0
#define HWIO_RPMH_PDC_DISPLAY_TCSt_CMDn_DRVd_STATUS_MAXt                                                     2
#define HWIO_RPMH_PDC_DISPLAY_TCSt_CMDn_DRVd_STATUS_MAXn                                                     3
#define HWIO_RPMH_PDC_DISPLAY_TCSt_CMDn_DRVd_STATUS_INI3(d,t,n)        \
        in_dword_masked(HWIO_RPMH_PDC_DISPLAY_TCSt_CMDn_DRVd_STATUS_ADDR(d,t,n), HWIO_RPMH_PDC_DISPLAY_TCSt_CMDn_DRVd_STATUS_RMSK)
#define HWIO_RPMH_PDC_DISPLAY_TCSt_CMDn_DRVd_STATUS_INMI3(d,t,n,mask)    \
        in_dword_masked(HWIO_RPMH_PDC_DISPLAY_TCSt_CMDn_DRVd_STATUS_ADDR(d,t,n), mask)
#define HWIO_RPMH_PDC_DISPLAY_TCSt_CMDn_DRVd_STATUS_TCS_CMD_RESPONSE_RECD_BMSK                         0x10000
#define HWIO_RPMH_PDC_DISPLAY_TCSt_CMDn_DRVd_STATUS_TCS_CMD_RESPONSE_RECD_SHFT                            0x10
#define HWIO_RPMH_PDC_DISPLAY_TCSt_CMDn_DRVd_STATUS_TCS_CMD_ISSUED_BMSK                                  0x100
#define HWIO_RPMH_PDC_DISPLAY_TCSt_CMDn_DRVd_STATUS_TCS_CMD_ISSUED_SHFT                                    0x8
#define HWIO_RPMH_PDC_DISPLAY_TCSt_CMDn_DRVd_STATUS_TCS_CMD_TRIGGERED_BMSK                                 0x1
#define HWIO_RPMH_PDC_DISPLAY_TCSt_CMDn_DRVd_STATUS_TCS_CMD_TRIGGERED_SHFT                                 0x0

#define HWIO_RPMH_PDC_DISPLAY_ENABLE_PDC_ADDR                                                       (RPMH_PDC_DISPLAY_DISPLAY_PDC_REG_BASE      + 0x00004500)
#define HWIO_RPMH_PDC_DISPLAY_ENABLE_PDC_RMSK                                                       0x80000001
#define HWIO_RPMH_PDC_DISPLAY_ENABLE_PDC_IN          \
        in_dword_masked(HWIO_RPMH_PDC_DISPLAY_ENABLE_PDC_ADDR, HWIO_RPMH_PDC_DISPLAY_ENABLE_PDC_RMSK)
#define HWIO_RPMH_PDC_DISPLAY_ENABLE_PDC_INM(m)      \
        in_dword_masked(HWIO_RPMH_PDC_DISPLAY_ENABLE_PDC_ADDR, m)
#define HWIO_RPMH_PDC_DISPLAY_ENABLE_PDC_OUT(v)      \
        out_dword(HWIO_RPMH_PDC_DISPLAY_ENABLE_PDC_ADDR,v)
#define HWIO_RPMH_PDC_DISPLAY_ENABLE_PDC_OUTM(m,v) \
        out_dword_masked_ns(HWIO_RPMH_PDC_DISPLAY_ENABLE_PDC_ADDR,m,v,HWIO_RPMH_PDC_DISPLAY_ENABLE_PDC_IN)
#define HWIO_RPMH_PDC_DISPLAY_ENABLE_PDC_DISABLE_CLK_GATE_BMSK                                      0x80000000
#define HWIO_RPMH_PDC_DISPLAY_ENABLE_PDC_DISABLE_CLK_GATE_SHFT                                            0x1f
#define HWIO_RPMH_PDC_DISPLAY_ENABLE_PDC_ENABLE_PDC_BMSK                                                   0x1
#define HWIO_RPMH_PDC_DISPLAY_ENABLE_PDC_ENABLE_PDC_SHFT                                                   0x0

#define HWIO_RPMH_PDC_DISPLAY_TIMER_MATCH_VALUE_LO_ADDR                                             (RPMH_PDC_DISPLAY_DISPLAY_PDC_REG_BASE      + 0x00004510)
#define HWIO_RPMH_PDC_DISPLAY_TIMER_MATCH_VALUE_LO_RMSK                                             0xffffffff
#define HWIO_RPMH_PDC_DISPLAY_TIMER_MATCH_VALUE_LO_IN          \
        in_dword_masked(HWIO_RPMH_PDC_DISPLAY_TIMER_MATCH_VALUE_LO_ADDR, HWIO_RPMH_PDC_DISPLAY_TIMER_MATCH_VALUE_LO_RMSK)
#define HWIO_RPMH_PDC_DISPLAY_TIMER_MATCH_VALUE_LO_INM(m)      \
        in_dword_masked(HWIO_RPMH_PDC_DISPLAY_TIMER_MATCH_VALUE_LO_ADDR, m)
#define HWIO_RPMH_PDC_DISPLAY_TIMER_MATCH_VALUE_LO_OUT(v)      \
        out_dword(HWIO_RPMH_PDC_DISPLAY_TIMER_MATCH_VALUE_LO_ADDR,v)
#define HWIO_RPMH_PDC_DISPLAY_TIMER_MATCH_VALUE_LO_OUTM(m,v) \
        out_dword_masked_ns(HWIO_RPMH_PDC_DISPLAY_TIMER_MATCH_VALUE_LO_ADDR,m,v,HWIO_RPMH_PDC_DISPLAY_TIMER_MATCH_VALUE_LO_IN)
#define HWIO_RPMH_PDC_DISPLAY_TIMER_MATCH_VALUE_LO_DATA_LO_BMSK                                     0xffffffff
#define HWIO_RPMH_PDC_DISPLAY_TIMER_MATCH_VALUE_LO_DATA_LO_SHFT                                            0x0

#define HWIO_RPMH_PDC_DISPLAY_TIMER_MATCH_VALUE_HI_ADDR                                             (RPMH_PDC_DISPLAY_DISPLAY_PDC_REG_BASE      + 0x00004514)
#define HWIO_RPMH_PDC_DISPLAY_TIMER_MATCH_VALUE_HI_RMSK                                               0xffffff
#define HWIO_RPMH_PDC_DISPLAY_TIMER_MATCH_VALUE_HI_IN          \
        in_dword_masked(HWIO_RPMH_PDC_DISPLAY_TIMER_MATCH_VALUE_HI_ADDR, HWIO_RPMH_PDC_DISPLAY_TIMER_MATCH_VALUE_HI_RMSK)
#define HWIO_RPMH_PDC_DISPLAY_TIMER_MATCH_VALUE_HI_INM(m)      \
        in_dword_masked(HWIO_RPMH_PDC_DISPLAY_TIMER_MATCH_VALUE_HI_ADDR, m)
#define HWIO_RPMH_PDC_DISPLAY_TIMER_MATCH_VALUE_HI_OUT(v)      \
        out_dword(HWIO_RPMH_PDC_DISPLAY_TIMER_MATCH_VALUE_HI_ADDR,v)
#define HWIO_RPMH_PDC_DISPLAY_TIMER_MATCH_VALUE_HI_OUTM(m,v) \
        out_dword_masked_ns(HWIO_RPMH_PDC_DISPLAY_TIMER_MATCH_VALUE_HI_ADDR,m,v,HWIO_RPMH_PDC_DISPLAY_TIMER_MATCH_VALUE_HI_IN)
#define HWIO_RPMH_PDC_DISPLAY_TIMER_MATCH_VALUE_HI_DATA_HI_BMSK                                       0xffffff
#define HWIO_RPMH_PDC_DISPLAY_TIMER_MATCH_VALUE_HI_DATA_HI_SHFT                                            0x0

#define HWIO_RPMH_PDC_DISPLAY_TIMER_DISABLED_FOR_ARC_ADDR                                           (RPMH_PDC_DISPLAY_DISPLAY_PDC_REG_BASE      + 0x00004518)
#define HWIO_RPMH_PDC_DISPLAY_TIMER_DISABLED_FOR_ARC_RMSK                                                  0x1
#define HWIO_RPMH_PDC_DISPLAY_TIMER_DISABLED_FOR_ARC_IN          \
        in_dword_masked(HWIO_RPMH_PDC_DISPLAY_TIMER_DISABLED_FOR_ARC_ADDR, HWIO_RPMH_PDC_DISPLAY_TIMER_DISABLED_FOR_ARC_RMSK)
#define HWIO_RPMH_PDC_DISPLAY_TIMER_DISABLED_FOR_ARC_INM(m)      \
        in_dword_masked(HWIO_RPMH_PDC_DISPLAY_TIMER_DISABLED_FOR_ARC_ADDR, m)
#define HWIO_RPMH_PDC_DISPLAY_TIMER_DISABLED_FOR_ARC_OUT(v)      \
        out_dword(HWIO_RPMH_PDC_DISPLAY_TIMER_DISABLED_FOR_ARC_ADDR,v)
#define HWIO_RPMH_PDC_DISPLAY_TIMER_DISABLED_FOR_ARC_OUTM(m,v) \
        out_dword_masked_ns(HWIO_RPMH_PDC_DISPLAY_TIMER_DISABLED_FOR_ARC_ADDR,m,v,HWIO_RPMH_PDC_DISPLAY_TIMER_DISABLED_FOR_ARC_IN)
#define HWIO_RPMH_PDC_DISPLAY_TIMER_DISABLED_FOR_ARC_TIMER_DISABLED_BMSK                                   0x1
#define HWIO_RPMH_PDC_DISPLAY_TIMER_DISABLED_FOR_ARC_TIMER_DISABLED_SHFT                                   0x0

#define HWIO_RPMH_PDC_DISPLAY_SEQ_START_ADDR_ADDR                                                   (RPMH_PDC_DISPLAY_DISPLAY_PDC_REG_BASE      + 0x00004520)
#define HWIO_RPMH_PDC_DISPLAY_SEQ_START_ADDR_RMSK                                                         0x7f
#define HWIO_RPMH_PDC_DISPLAY_SEQ_START_ADDR_IN          \
        in_dword_masked(HWIO_RPMH_PDC_DISPLAY_SEQ_START_ADDR_ADDR, HWIO_RPMH_PDC_DISPLAY_SEQ_START_ADDR_RMSK)
#define HWIO_RPMH_PDC_DISPLAY_SEQ_START_ADDR_INM(m)      \
        in_dword_masked(HWIO_RPMH_PDC_DISPLAY_SEQ_START_ADDR_ADDR, m)
#define HWIO_RPMH_PDC_DISPLAY_SEQ_START_ADDR_OUT(v)      \
        out_dword(HWIO_RPMH_PDC_DISPLAY_SEQ_START_ADDR_ADDR,v)
#define HWIO_RPMH_PDC_DISPLAY_SEQ_START_ADDR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_RPMH_PDC_DISPLAY_SEQ_START_ADDR_ADDR,m,v,HWIO_RPMH_PDC_DISPLAY_SEQ_START_ADDR_IN)
#define HWIO_RPMH_PDC_DISPLAY_SEQ_START_ADDR_SEQ_START_ADDR_BMSK                                          0x7f
#define HWIO_RPMH_PDC_DISPLAY_SEQ_START_ADDR_SEQ_START_ADDR_SHFT                                           0x0

#define HWIO_RPMH_PDC_DISPLAY_SEQ_OVERRIDE_START_ADDR_ADDR                                          (RPMH_PDC_DISPLAY_DISPLAY_PDC_REG_BASE      + 0x00004524)
#define HWIO_RPMH_PDC_DISPLAY_SEQ_OVERRIDE_START_ADDR_RMSK                                          0x8000007f
#define HWIO_RPMH_PDC_DISPLAY_SEQ_OVERRIDE_START_ADDR_IN          \
        in_dword_masked(HWIO_RPMH_PDC_DISPLAY_SEQ_OVERRIDE_START_ADDR_ADDR, HWIO_RPMH_PDC_DISPLAY_SEQ_OVERRIDE_START_ADDR_RMSK)
#define HWIO_RPMH_PDC_DISPLAY_SEQ_OVERRIDE_START_ADDR_INM(m)      \
        in_dword_masked(HWIO_RPMH_PDC_DISPLAY_SEQ_OVERRIDE_START_ADDR_ADDR, m)
#define HWIO_RPMH_PDC_DISPLAY_SEQ_OVERRIDE_START_ADDR_OUT(v)      \
        out_dword(HWIO_RPMH_PDC_DISPLAY_SEQ_OVERRIDE_START_ADDR_ADDR,v)
#define HWIO_RPMH_PDC_DISPLAY_SEQ_OVERRIDE_START_ADDR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_RPMH_PDC_DISPLAY_SEQ_OVERRIDE_START_ADDR_ADDR,m,v,HWIO_RPMH_PDC_DISPLAY_SEQ_OVERRIDE_START_ADDR_IN)
#define HWIO_RPMH_PDC_DISPLAY_SEQ_OVERRIDE_START_ADDR_SEQ_OVERRIDE_VALID_BMSK                       0x80000000
#define HWIO_RPMH_PDC_DISPLAY_SEQ_OVERRIDE_START_ADDR_SEQ_OVERRIDE_VALID_SHFT                             0x1f
#define HWIO_RPMH_PDC_DISPLAY_SEQ_OVERRIDE_START_ADDR_SEQ_OVERRIDE_START_ADDR_BMSK                        0x7f
#define HWIO_RPMH_PDC_DISPLAY_SEQ_OVERRIDE_START_ADDR_SEQ_OVERRIDE_START_ADDR_SHFT                         0x0

#define HWIO_RPMH_PDC_DISPLAY_SEQ_OVERRIDE_TRIGGER_ADDR                                             (RPMH_PDC_DISPLAY_DISPLAY_PDC_REG_BASE      + 0x0000452c)
#define HWIO_RPMH_PDC_DISPLAY_SEQ_OVERRIDE_TRIGGER_RMSK                                                    0x1
#define HWIO_RPMH_PDC_DISPLAY_SEQ_OVERRIDE_TRIGGER_IN          \
        in_dword_masked(HWIO_RPMH_PDC_DISPLAY_SEQ_OVERRIDE_TRIGGER_ADDR, HWIO_RPMH_PDC_DISPLAY_SEQ_OVERRIDE_TRIGGER_RMSK)
#define HWIO_RPMH_PDC_DISPLAY_SEQ_OVERRIDE_TRIGGER_INM(m)      \
        in_dword_masked(HWIO_RPMH_PDC_DISPLAY_SEQ_OVERRIDE_TRIGGER_ADDR, m)
#define HWIO_RPMH_PDC_DISPLAY_SEQ_OVERRIDE_TRIGGER_OUT(v)      \
        out_dword(HWIO_RPMH_PDC_DISPLAY_SEQ_OVERRIDE_TRIGGER_ADDR,v)
#define HWIO_RPMH_PDC_DISPLAY_SEQ_OVERRIDE_TRIGGER_OUTM(m,v) \
        out_dword_masked_ns(HWIO_RPMH_PDC_DISPLAY_SEQ_OVERRIDE_TRIGGER_ADDR,m,v,HWIO_RPMH_PDC_DISPLAY_SEQ_OVERRIDE_TRIGGER_IN)
#define HWIO_RPMH_PDC_DISPLAY_SEQ_OVERRIDE_TRIGGER_SEQ_OVERRIDE_TRIGGER_BMSK                               0x1
#define HWIO_RPMH_PDC_DISPLAY_SEQ_OVERRIDE_TRIGGER_SEQ_OVERRIDE_TRIGGER_SHFT                               0x0

#define HWIO_RPMH_PDC_DISPLAY_SEQ_PWR_CTRL_OVERRIDE_MASK_ADDR                                       (RPMH_PDC_DISPLAY_DISPLAY_PDC_REG_BASE      + 0x00004530)
#define HWIO_RPMH_PDC_DISPLAY_SEQ_PWR_CTRL_OVERRIDE_MASK_RMSK                                       0xffffffff
#define HWIO_RPMH_PDC_DISPLAY_SEQ_PWR_CTRL_OVERRIDE_MASK_IN          \
        in_dword_masked(HWIO_RPMH_PDC_DISPLAY_SEQ_PWR_CTRL_OVERRIDE_MASK_ADDR, HWIO_RPMH_PDC_DISPLAY_SEQ_PWR_CTRL_OVERRIDE_MASK_RMSK)
#define HWIO_RPMH_PDC_DISPLAY_SEQ_PWR_CTRL_OVERRIDE_MASK_INM(m)      \
        in_dword_masked(HWIO_RPMH_PDC_DISPLAY_SEQ_PWR_CTRL_OVERRIDE_MASK_ADDR, m)
#define HWIO_RPMH_PDC_DISPLAY_SEQ_PWR_CTRL_OVERRIDE_MASK_OUT(v)      \
        out_dword(HWIO_RPMH_PDC_DISPLAY_SEQ_PWR_CTRL_OVERRIDE_MASK_ADDR,v)
#define HWIO_RPMH_PDC_DISPLAY_SEQ_PWR_CTRL_OVERRIDE_MASK_OUTM(m,v) \
        out_dword_masked_ns(HWIO_RPMH_PDC_DISPLAY_SEQ_PWR_CTRL_OVERRIDE_MASK_ADDR,m,v,HWIO_RPMH_PDC_DISPLAY_SEQ_PWR_CTRL_OVERRIDE_MASK_IN)
#define HWIO_RPMH_PDC_DISPLAY_SEQ_PWR_CTRL_OVERRIDE_MASK_PWR_CTRL_OVERRIDE_MASK_BMSK                0xffffffff
#define HWIO_RPMH_PDC_DISPLAY_SEQ_PWR_CTRL_OVERRIDE_MASK_PWR_CTRL_OVERRIDE_MASK_SHFT                       0x0

#define HWIO_RPMH_PDC_DISPLAY_SEQ_PWR_CTRL_OVERRIDE_VAL_ADDR                                        (RPMH_PDC_DISPLAY_DISPLAY_PDC_REG_BASE      + 0x00004534)
#define HWIO_RPMH_PDC_DISPLAY_SEQ_PWR_CTRL_OVERRIDE_VAL_RMSK                                        0xffffffff
#define HWIO_RPMH_PDC_DISPLAY_SEQ_PWR_CTRL_OVERRIDE_VAL_IN          \
        in_dword_masked(HWIO_RPMH_PDC_DISPLAY_SEQ_PWR_CTRL_OVERRIDE_VAL_ADDR, HWIO_RPMH_PDC_DISPLAY_SEQ_PWR_CTRL_OVERRIDE_VAL_RMSK)
#define HWIO_RPMH_PDC_DISPLAY_SEQ_PWR_CTRL_OVERRIDE_VAL_INM(m)      \
        in_dword_masked(HWIO_RPMH_PDC_DISPLAY_SEQ_PWR_CTRL_OVERRIDE_VAL_ADDR, m)
#define HWIO_RPMH_PDC_DISPLAY_SEQ_PWR_CTRL_OVERRIDE_VAL_OUT(v)      \
        out_dword(HWIO_RPMH_PDC_DISPLAY_SEQ_PWR_CTRL_OVERRIDE_VAL_ADDR,v)
#define HWIO_RPMH_PDC_DISPLAY_SEQ_PWR_CTRL_OVERRIDE_VAL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_RPMH_PDC_DISPLAY_SEQ_PWR_CTRL_OVERRIDE_VAL_ADDR,m,v,HWIO_RPMH_PDC_DISPLAY_SEQ_PWR_CTRL_OVERRIDE_VAL_IN)
#define HWIO_RPMH_PDC_DISPLAY_SEQ_PWR_CTRL_OVERRIDE_VAL_PWR_CTRL_OVERRIDE_VAL_BMSK                  0xffffffff
#define HWIO_RPMH_PDC_DISPLAY_SEQ_PWR_CTRL_OVERRIDE_VAL_PWR_CTRL_OVERRIDE_VAL_SHFT                         0x0

#define HWIO_RPMH_PDC_DISPLAY_SEQ_WAIT_EVENT_OVERRIDE_MASK_ADDR                                     (RPMH_PDC_DISPLAY_DISPLAY_PDC_REG_BASE      + 0x00004540)
#define HWIO_RPMH_PDC_DISPLAY_SEQ_WAIT_EVENT_OVERRIDE_MASK_RMSK                                     0xffffffff
#define HWIO_RPMH_PDC_DISPLAY_SEQ_WAIT_EVENT_OVERRIDE_MASK_IN          \
        in_dword_masked(HWIO_RPMH_PDC_DISPLAY_SEQ_WAIT_EVENT_OVERRIDE_MASK_ADDR, HWIO_RPMH_PDC_DISPLAY_SEQ_WAIT_EVENT_OVERRIDE_MASK_RMSK)
#define HWIO_RPMH_PDC_DISPLAY_SEQ_WAIT_EVENT_OVERRIDE_MASK_INM(m)      \
        in_dword_masked(HWIO_RPMH_PDC_DISPLAY_SEQ_WAIT_EVENT_OVERRIDE_MASK_ADDR, m)
#define HWIO_RPMH_PDC_DISPLAY_SEQ_WAIT_EVENT_OVERRIDE_MASK_OUT(v)      \
        out_dword(HWIO_RPMH_PDC_DISPLAY_SEQ_WAIT_EVENT_OVERRIDE_MASK_ADDR,v)
#define HWIO_RPMH_PDC_DISPLAY_SEQ_WAIT_EVENT_OVERRIDE_MASK_OUTM(m,v) \
        out_dword_masked_ns(HWIO_RPMH_PDC_DISPLAY_SEQ_WAIT_EVENT_OVERRIDE_MASK_ADDR,m,v,HWIO_RPMH_PDC_DISPLAY_SEQ_WAIT_EVENT_OVERRIDE_MASK_IN)
#define HWIO_RPMH_PDC_DISPLAY_SEQ_WAIT_EVENT_OVERRIDE_MASK_WAIT_EVENT_OVERRIDE_MASK_BMSK            0xffffffff
#define HWIO_RPMH_PDC_DISPLAY_SEQ_WAIT_EVENT_OVERRIDE_MASK_WAIT_EVENT_OVERRIDE_MASK_SHFT                   0x0

#define HWIO_RPMH_PDC_DISPLAY_SEQ_WAIT_EVENT_OVERRIDE_VAL_ADDR                                      (RPMH_PDC_DISPLAY_DISPLAY_PDC_REG_BASE      + 0x00004544)
#define HWIO_RPMH_PDC_DISPLAY_SEQ_WAIT_EVENT_OVERRIDE_VAL_RMSK                                      0xffffffff
#define HWIO_RPMH_PDC_DISPLAY_SEQ_WAIT_EVENT_OVERRIDE_VAL_IN          \
        in_dword_masked(HWIO_RPMH_PDC_DISPLAY_SEQ_WAIT_EVENT_OVERRIDE_VAL_ADDR, HWIO_RPMH_PDC_DISPLAY_SEQ_WAIT_EVENT_OVERRIDE_VAL_RMSK)
#define HWIO_RPMH_PDC_DISPLAY_SEQ_WAIT_EVENT_OVERRIDE_VAL_INM(m)      \
        in_dword_masked(HWIO_RPMH_PDC_DISPLAY_SEQ_WAIT_EVENT_OVERRIDE_VAL_ADDR, m)
#define HWIO_RPMH_PDC_DISPLAY_SEQ_WAIT_EVENT_OVERRIDE_VAL_OUT(v)      \
        out_dword(HWIO_RPMH_PDC_DISPLAY_SEQ_WAIT_EVENT_OVERRIDE_VAL_ADDR,v)
#define HWIO_RPMH_PDC_DISPLAY_SEQ_WAIT_EVENT_OVERRIDE_VAL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_RPMH_PDC_DISPLAY_SEQ_WAIT_EVENT_OVERRIDE_VAL_ADDR,m,v,HWIO_RPMH_PDC_DISPLAY_SEQ_WAIT_EVENT_OVERRIDE_VAL_IN)
#define HWIO_RPMH_PDC_DISPLAY_SEQ_WAIT_EVENT_OVERRIDE_VAL_WAIT_EVENT_OVERRIDE_VAL_BMSK              0xffffffff
#define HWIO_RPMH_PDC_DISPLAY_SEQ_WAIT_EVENT_OVERRIDE_VAL_WAIT_EVENT_OVERRIDE_VAL_SHFT                     0x0

#define HWIO_RPMH_PDC_DISPLAY_SEQ_BR_EVENT_OVERRIDE_MASK_ADDR                                       (RPMH_PDC_DISPLAY_DISPLAY_PDC_REG_BASE      + 0x00004550)
#define HWIO_RPMH_PDC_DISPLAY_SEQ_BR_EVENT_OVERRIDE_MASK_RMSK                                       0xffffffff
#define HWIO_RPMH_PDC_DISPLAY_SEQ_BR_EVENT_OVERRIDE_MASK_IN          \
        in_dword_masked(HWIO_RPMH_PDC_DISPLAY_SEQ_BR_EVENT_OVERRIDE_MASK_ADDR, HWIO_RPMH_PDC_DISPLAY_SEQ_BR_EVENT_OVERRIDE_MASK_RMSK)
#define HWIO_RPMH_PDC_DISPLAY_SEQ_BR_EVENT_OVERRIDE_MASK_INM(m)      \
        in_dword_masked(HWIO_RPMH_PDC_DISPLAY_SEQ_BR_EVENT_OVERRIDE_MASK_ADDR, m)
#define HWIO_RPMH_PDC_DISPLAY_SEQ_BR_EVENT_OVERRIDE_MASK_OUT(v)      \
        out_dword(HWIO_RPMH_PDC_DISPLAY_SEQ_BR_EVENT_OVERRIDE_MASK_ADDR,v)
#define HWIO_RPMH_PDC_DISPLAY_SEQ_BR_EVENT_OVERRIDE_MASK_OUTM(m,v) \
        out_dword_masked_ns(HWIO_RPMH_PDC_DISPLAY_SEQ_BR_EVENT_OVERRIDE_MASK_ADDR,m,v,HWIO_RPMH_PDC_DISPLAY_SEQ_BR_EVENT_OVERRIDE_MASK_IN)
#define HWIO_RPMH_PDC_DISPLAY_SEQ_BR_EVENT_OVERRIDE_MASK_BR_EVENT_OVERRIDE_MASK_BMSK                0xffffffff
#define HWIO_RPMH_PDC_DISPLAY_SEQ_BR_EVENT_OVERRIDE_MASK_BR_EVENT_OVERRIDE_MASK_SHFT                       0x0

#define HWIO_RPMH_PDC_DISPLAY_SEQ_BR_EVENT_OVERRIDE_VAL_ADDR                                        (RPMH_PDC_DISPLAY_DISPLAY_PDC_REG_BASE      + 0x00004554)
#define HWIO_RPMH_PDC_DISPLAY_SEQ_BR_EVENT_OVERRIDE_VAL_RMSK                                        0xffffffff
#define HWIO_RPMH_PDC_DISPLAY_SEQ_BR_EVENT_OVERRIDE_VAL_IN          \
        in_dword_masked(HWIO_RPMH_PDC_DISPLAY_SEQ_BR_EVENT_OVERRIDE_VAL_ADDR, HWIO_RPMH_PDC_DISPLAY_SEQ_BR_EVENT_OVERRIDE_VAL_RMSK)
#define HWIO_RPMH_PDC_DISPLAY_SEQ_BR_EVENT_OVERRIDE_VAL_INM(m)      \
        in_dword_masked(HWIO_RPMH_PDC_DISPLAY_SEQ_BR_EVENT_OVERRIDE_VAL_ADDR, m)
#define HWIO_RPMH_PDC_DISPLAY_SEQ_BR_EVENT_OVERRIDE_VAL_OUT(v)      \
        out_dword(HWIO_RPMH_PDC_DISPLAY_SEQ_BR_EVENT_OVERRIDE_VAL_ADDR,v)
#define HWIO_RPMH_PDC_DISPLAY_SEQ_BR_EVENT_OVERRIDE_VAL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_RPMH_PDC_DISPLAY_SEQ_BR_EVENT_OVERRIDE_VAL_ADDR,m,v,HWIO_RPMH_PDC_DISPLAY_SEQ_BR_EVENT_OVERRIDE_VAL_IN)
#define HWIO_RPMH_PDC_DISPLAY_SEQ_BR_EVENT_OVERRIDE_VAL_BR_EVENT_OVERRIDE_VAL_BMSK                  0xffffffff
#define HWIO_RPMH_PDC_DISPLAY_SEQ_BR_EVENT_OVERRIDE_VAL_BR_EVENT_OVERRIDE_VAL_SHFT                         0x0

#define HWIO_RPMH_PDC_DISPLAY_SEQ_CFG_BR_ADDR_b_ADDR(b)                                             (RPMH_PDC_DISPLAY_DISPLAY_PDC_REG_BASE      + 0x00004590 + 0x4 * (b))
#define HWIO_RPMH_PDC_DISPLAY_SEQ_CFG_BR_ADDR_b_RMSK                                                      0x7f
#define HWIO_RPMH_PDC_DISPLAY_SEQ_CFG_BR_ADDR_b_MAXb                                                         3
#define HWIO_RPMH_PDC_DISPLAY_SEQ_CFG_BR_ADDR_b_INI(b)        \
        in_dword_masked(HWIO_RPMH_PDC_DISPLAY_SEQ_CFG_BR_ADDR_b_ADDR(b), HWIO_RPMH_PDC_DISPLAY_SEQ_CFG_BR_ADDR_b_RMSK)
#define HWIO_RPMH_PDC_DISPLAY_SEQ_CFG_BR_ADDR_b_INMI(b,mask)    \
        in_dword_masked(HWIO_RPMH_PDC_DISPLAY_SEQ_CFG_BR_ADDR_b_ADDR(b), mask)
#define HWIO_RPMH_PDC_DISPLAY_SEQ_CFG_BR_ADDR_b_OUTI(b,val)    \
        out_dword(HWIO_RPMH_PDC_DISPLAY_SEQ_CFG_BR_ADDR_b_ADDR(b),val)
#define HWIO_RPMH_PDC_DISPLAY_SEQ_CFG_BR_ADDR_b_OUTMI(b,mask,val) \
        out_dword_masked_ns(HWIO_RPMH_PDC_DISPLAY_SEQ_CFG_BR_ADDR_b_ADDR(b),mask,val,HWIO_RPMH_PDC_DISPLAY_SEQ_CFG_BR_ADDR_b_INI(b))
#define HWIO_RPMH_PDC_DISPLAY_SEQ_CFG_BR_ADDR_b_BR_ADDR_BMSK                                              0x7f
#define HWIO_RPMH_PDC_DISPLAY_SEQ_CFG_BR_ADDR_b_BR_ADDR_SHFT                                               0x0

#define HWIO_RPMH_PDC_DISPLAY_SEQ_CFG_DELAY_VAL_v_ADDR(v)                                           (RPMH_PDC_DISPLAY_DISPLAY_PDC_REG_BASE      + 0x000045a0 + 0x4 * (v))
#define HWIO_RPMH_PDC_DISPLAY_SEQ_CFG_DELAY_VAL_v_RMSK                                                 0xfffff
#define HWIO_RPMH_PDC_DISPLAY_SEQ_CFG_DELAY_VAL_v_MAXv                                                      15
#define HWIO_RPMH_PDC_DISPLAY_SEQ_CFG_DELAY_VAL_v_INI(v)        \
        in_dword_masked(HWIO_RPMH_PDC_DISPLAY_SEQ_CFG_DELAY_VAL_v_ADDR(v), HWIO_RPMH_PDC_DISPLAY_SEQ_CFG_DELAY_VAL_v_RMSK)
#define HWIO_RPMH_PDC_DISPLAY_SEQ_CFG_DELAY_VAL_v_INMI(v,mask)    \
        in_dword_masked(HWIO_RPMH_PDC_DISPLAY_SEQ_CFG_DELAY_VAL_v_ADDR(v), mask)
#define HWIO_RPMH_PDC_DISPLAY_SEQ_CFG_DELAY_VAL_v_OUTI(v,val)    \
        out_dword(HWIO_RPMH_PDC_DISPLAY_SEQ_CFG_DELAY_VAL_v_ADDR(v),val)
#define HWIO_RPMH_PDC_DISPLAY_SEQ_CFG_DELAY_VAL_v_OUTMI(v,mask,val) \
        out_dword_masked_ns(HWIO_RPMH_PDC_DISPLAY_SEQ_CFG_DELAY_VAL_v_ADDR(v),mask,val,HWIO_RPMH_PDC_DISPLAY_SEQ_CFG_DELAY_VAL_v_INI(v))
#define HWIO_RPMH_PDC_DISPLAY_SEQ_CFG_DELAY_VAL_v_DELAY_VAL_BMSK                                       0xfffff
#define HWIO_RPMH_PDC_DISPLAY_SEQ_CFG_DELAY_VAL_v_DELAY_VAL_SHFT                                           0x0

#define HWIO_RPMH_PDC_DISPLAY_SEQ_DBG_BREAKPOINT_ADDR_ADDR                                          (RPMH_PDC_DISPLAY_DISPLAY_PDC_REG_BASE      + 0x000045e0)
#define HWIO_RPMH_PDC_DISPLAY_SEQ_DBG_BREAKPOINT_ADDR_RMSK                                          0x8000007f
#define HWIO_RPMH_PDC_DISPLAY_SEQ_DBG_BREAKPOINT_ADDR_IN          \
        in_dword_masked(HWIO_RPMH_PDC_DISPLAY_SEQ_DBG_BREAKPOINT_ADDR_ADDR, HWIO_RPMH_PDC_DISPLAY_SEQ_DBG_BREAKPOINT_ADDR_RMSK)
#define HWIO_RPMH_PDC_DISPLAY_SEQ_DBG_BREAKPOINT_ADDR_INM(m)      \
        in_dword_masked(HWIO_RPMH_PDC_DISPLAY_SEQ_DBG_BREAKPOINT_ADDR_ADDR, m)
#define HWIO_RPMH_PDC_DISPLAY_SEQ_DBG_BREAKPOINT_ADDR_OUT(v)      \
        out_dword(HWIO_RPMH_PDC_DISPLAY_SEQ_DBG_BREAKPOINT_ADDR_ADDR,v)
#define HWIO_RPMH_PDC_DISPLAY_SEQ_DBG_BREAKPOINT_ADDR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_RPMH_PDC_DISPLAY_SEQ_DBG_BREAKPOINT_ADDR_ADDR,m,v,HWIO_RPMH_PDC_DISPLAY_SEQ_DBG_BREAKPOINT_ADDR_IN)
#define HWIO_RPMH_PDC_DISPLAY_SEQ_DBG_BREAKPOINT_ADDR_DBG_BREAKPOINT_VALID_BMSK                     0x80000000
#define HWIO_RPMH_PDC_DISPLAY_SEQ_DBG_BREAKPOINT_ADDR_DBG_BREAKPOINT_VALID_SHFT                           0x1f
#define HWIO_RPMH_PDC_DISPLAY_SEQ_DBG_BREAKPOINT_ADDR_DBG_BREAKPOINT_ADDR_BMSK                            0x7f
#define HWIO_RPMH_PDC_DISPLAY_SEQ_DBG_BREAKPOINT_ADDR_DBG_BREAKPOINT_ADDR_SHFT                             0x0

#define HWIO_RPMH_PDC_DISPLAY_SEQ_DBG_STEP_ADDR                                                     (RPMH_PDC_DISPLAY_DISPLAY_PDC_REG_BASE      + 0x000045e4)
#define HWIO_RPMH_PDC_DISPLAY_SEQ_DBG_STEP_RMSK                                                        0x10001
#define HWIO_RPMH_PDC_DISPLAY_SEQ_DBG_STEP_IN          \
        in_dword_masked(HWIO_RPMH_PDC_DISPLAY_SEQ_DBG_STEP_ADDR, HWIO_RPMH_PDC_DISPLAY_SEQ_DBG_STEP_RMSK)
#define HWIO_RPMH_PDC_DISPLAY_SEQ_DBG_STEP_INM(m)      \
        in_dword_masked(HWIO_RPMH_PDC_DISPLAY_SEQ_DBG_STEP_ADDR, m)
#define HWIO_RPMH_PDC_DISPLAY_SEQ_DBG_STEP_OUT(v)      \
        out_dword(HWIO_RPMH_PDC_DISPLAY_SEQ_DBG_STEP_ADDR,v)
#define HWIO_RPMH_PDC_DISPLAY_SEQ_DBG_STEP_OUTM(m,v) \
        out_dword_masked_ns(HWIO_RPMH_PDC_DISPLAY_SEQ_DBG_STEP_ADDR,m,v,HWIO_RPMH_PDC_DISPLAY_SEQ_DBG_STEP_IN)
#define HWIO_RPMH_PDC_DISPLAY_SEQ_DBG_STEP_SEQ_DBG_STEP_TRIGGER_BMSK                                   0x10000
#define HWIO_RPMH_PDC_DISPLAY_SEQ_DBG_STEP_SEQ_DBG_STEP_TRIGGER_SHFT                                      0x10
#define HWIO_RPMH_PDC_DISPLAY_SEQ_DBG_STEP_SEQ_DBG_STEP_CONTINUE_BMSK                                      0x1
#define HWIO_RPMH_PDC_DISPLAY_SEQ_DBG_STEP_SEQ_DBG_STEP_CONTINUE_SHFT                                      0x0

#define HWIO_RPMH_PDC_DISPLAY_PROFILING_UNITpf_EN_ADDR(pf)                                          (RPMH_PDC_DISPLAY_DISPLAY_PDC_REG_BASE      + 0x00005000 + 0x14 * (pf))
#define HWIO_RPMH_PDC_DISPLAY_PROFILING_UNITpf_EN_RMSK                                                     0x1
#define HWIO_RPMH_PDC_DISPLAY_PROFILING_UNITpf_EN_MAXpf                                                      4
#define HWIO_RPMH_PDC_DISPLAY_PROFILING_UNITpf_EN_INI(pf)        \
        in_dword_masked(HWIO_RPMH_PDC_DISPLAY_PROFILING_UNITpf_EN_ADDR(pf), HWIO_RPMH_PDC_DISPLAY_PROFILING_UNITpf_EN_RMSK)
#define HWIO_RPMH_PDC_DISPLAY_PROFILING_UNITpf_EN_INMI(pf,mask)    \
        in_dword_masked(HWIO_RPMH_PDC_DISPLAY_PROFILING_UNITpf_EN_ADDR(pf), mask)
#define HWIO_RPMH_PDC_DISPLAY_PROFILING_UNITpf_EN_OUTI(pf,val)    \
        out_dword(HWIO_RPMH_PDC_DISPLAY_PROFILING_UNITpf_EN_ADDR(pf),val)
#define HWIO_RPMH_PDC_DISPLAY_PROFILING_UNITpf_EN_OUTMI(pf,mask,val) \
        out_dword_masked_ns(HWIO_RPMH_PDC_DISPLAY_PROFILING_UNITpf_EN_ADDR(pf),mask,val,HWIO_RPMH_PDC_DISPLAY_PROFILING_UNITpf_EN_INI(pf))
#define HWIO_RPMH_PDC_DISPLAY_PROFILING_UNITpf_EN_PROFILING_UNIT_ENABLE_BMSK                               0x1
#define HWIO_RPMH_PDC_DISPLAY_PROFILING_UNITpf_EN_PROFILING_UNIT_ENABLE_SHFT                               0x0

#define HWIO_RPMH_PDC_DISPLAY_TCSt_CONTROL_ADDR(t)                                                  (RPMH_PDC_DISPLAY_DISPLAY_PDC_REG_BASE      + 0x00005500 + 0xC8 * (t))
#define HWIO_RPMH_PDC_DISPLAY_TCSt_CONTROL_RMSK                                                      0x1010000
#define HWIO_RPMH_PDC_DISPLAY_TCSt_CONTROL_MAXt                                                              2
#define HWIO_RPMH_PDC_DISPLAY_TCSt_CONTROL_INI(t)        \
        in_dword_masked(HWIO_RPMH_PDC_DISPLAY_TCSt_CONTROL_ADDR(t), HWIO_RPMH_PDC_DISPLAY_TCSt_CONTROL_RMSK)
#define HWIO_RPMH_PDC_DISPLAY_TCSt_CONTROL_INMI(t,mask)    \
        in_dword_masked(HWIO_RPMH_PDC_DISPLAY_TCSt_CONTROL_ADDR(t), mask)
#define HWIO_RPMH_PDC_DISPLAY_TCSt_CONTROL_OUTI(t,val)    \
        out_dword(HWIO_RPMH_PDC_DISPLAY_TCSt_CONTROL_ADDR(t),val)
#define HWIO_RPMH_PDC_DISPLAY_TCSt_CONTROL_OUTMI(t,mask,val) \
        out_dword_masked_ns(HWIO_RPMH_PDC_DISPLAY_TCSt_CONTROL_ADDR(t),mask,val,HWIO_RPMH_PDC_DISPLAY_TCSt_CONTROL_INI(t))
#define HWIO_RPMH_PDC_DISPLAY_TCSt_CONTROL_AMC_MODE_TRIGGER_BMSK                                     0x1000000
#define HWIO_RPMH_PDC_DISPLAY_TCSt_CONTROL_AMC_MODE_TRIGGER_SHFT                                          0x18
#define HWIO_RPMH_PDC_DISPLAY_TCSt_CONTROL_AMC_MODE_EN_BMSK                                            0x10000
#define HWIO_RPMH_PDC_DISPLAY_TCSt_CONTROL_AMC_MODE_EN_SHFT                                               0x10

#define HWIO_RPMH_PDC_DISPLAY_TCSt_CMD_ENABLE_BANK_ADDR(t)                                          (RPMH_PDC_DISPLAY_DISPLAY_PDC_REG_BASE      + 0x00005504 + 0xC8 * (t))
#define HWIO_RPMH_PDC_DISPLAY_TCSt_CMD_ENABLE_BANK_RMSK                                                    0xf
#define HWIO_RPMH_PDC_DISPLAY_TCSt_CMD_ENABLE_BANK_MAXt                                                      2
#define HWIO_RPMH_PDC_DISPLAY_TCSt_CMD_ENABLE_BANK_INI(t)        \
        in_dword_masked(HWIO_RPMH_PDC_DISPLAY_TCSt_CMD_ENABLE_BANK_ADDR(t), HWIO_RPMH_PDC_DISPLAY_TCSt_CMD_ENABLE_BANK_RMSK)
#define HWIO_RPMH_PDC_DISPLAY_TCSt_CMD_ENABLE_BANK_INMI(t,mask)    \
        in_dword_masked(HWIO_RPMH_PDC_DISPLAY_TCSt_CMD_ENABLE_BANK_ADDR(t), mask)
#define HWIO_RPMH_PDC_DISPLAY_TCSt_CMD_ENABLE_BANK_OUTI(t,val)    \
        out_dword(HWIO_RPMH_PDC_DISPLAY_TCSt_CMD_ENABLE_BANK_ADDR(t),val)
#define HWIO_RPMH_PDC_DISPLAY_TCSt_CMD_ENABLE_BANK_OUTMI(t,mask,val) \
        out_dword_masked_ns(HWIO_RPMH_PDC_DISPLAY_TCSt_CMD_ENABLE_BANK_ADDR(t),mask,val,HWIO_RPMH_PDC_DISPLAY_TCSt_CMD_ENABLE_BANK_INI(t))
#define HWIO_RPMH_PDC_DISPLAY_TCSt_CMD_ENABLE_BANK_INDIVIDUAL_CMD_ENABLE_MASK_BMSK                         0xf
#define HWIO_RPMH_PDC_DISPLAY_TCSt_CMD_ENABLE_BANK_INDIVIDUAL_CMD_ENABLE_MASK_SHFT                         0x0

#define HWIO_RPMH_PDC_DISPLAY_TCSt_CMD_WAIT_FOR_CMPL_BANK_ADDR(t)                                   (RPMH_PDC_DISPLAY_DISPLAY_PDC_REG_BASE      + 0x00005508 + 0xC8 * (t))
#define HWIO_RPMH_PDC_DISPLAY_TCSt_CMD_WAIT_FOR_CMPL_BANK_RMSK                                             0xf
#define HWIO_RPMH_PDC_DISPLAY_TCSt_CMD_WAIT_FOR_CMPL_BANK_MAXt                                               2
#define HWIO_RPMH_PDC_DISPLAY_TCSt_CMD_WAIT_FOR_CMPL_BANK_INI(t)        \
        in_dword_masked(HWIO_RPMH_PDC_DISPLAY_TCSt_CMD_WAIT_FOR_CMPL_BANK_ADDR(t), HWIO_RPMH_PDC_DISPLAY_TCSt_CMD_WAIT_FOR_CMPL_BANK_RMSK)
#define HWIO_RPMH_PDC_DISPLAY_TCSt_CMD_WAIT_FOR_CMPL_BANK_INMI(t,mask)    \
        in_dword_masked(HWIO_RPMH_PDC_DISPLAY_TCSt_CMD_WAIT_FOR_CMPL_BANK_ADDR(t), mask)
#define HWIO_RPMH_PDC_DISPLAY_TCSt_CMD_WAIT_FOR_CMPL_BANK_OUTI(t,val)    \
        out_dword(HWIO_RPMH_PDC_DISPLAY_TCSt_CMD_WAIT_FOR_CMPL_BANK_ADDR(t),val)
#define HWIO_RPMH_PDC_DISPLAY_TCSt_CMD_WAIT_FOR_CMPL_BANK_OUTMI(t,mask,val) \
        out_dword_masked_ns(HWIO_RPMH_PDC_DISPLAY_TCSt_CMD_WAIT_FOR_CMPL_BANK_ADDR(t),mask,val,HWIO_RPMH_PDC_DISPLAY_TCSt_CMD_WAIT_FOR_CMPL_BANK_INI(t))
#define HWIO_RPMH_PDC_DISPLAY_TCSt_CMD_WAIT_FOR_CMPL_BANK_CMD_WAIT_FOR_CMPL_BANK_BMSK                      0xf
#define HWIO_RPMH_PDC_DISPLAY_TCSt_CMD_WAIT_FOR_CMPL_BANK_CMD_WAIT_FOR_CMPL_BANK_SHFT                      0x0

#define HWIO_RPMH_PDC_DISPLAY_TCSt_CMDn_MSGID_ADDR(t,n)                                             (RPMH_PDC_DISPLAY_DISPLAY_PDC_REG_BASE      + 0x0000550c + 0xC8 * (t) + 0x10 * (n))
#define HWIO_RPMH_PDC_DISPLAY_TCSt_CMDn_MSGID_RMSK                                                     0x10100
#define HWIO_RPMH_PDC_DISPLAY_TCSt_CMDn_MSGID_MAXt                                                           2
#define HWIO_RPMH_PDC_DISPLAY_TCSt_CMDn_MSGID_MAXn                                                           3
#define HWIO_RPMH_PDC_DISPLAY_TCSt_CMDn_MSGID_INI2(t,n)        \
        in_dword_masked(HWIO_RPMH_PDC_DISPLAY_TCSt_CMDn_MSGID_ADDR(t,n), HWIO_RPMH_PDC_DISPLAY_TCSt_CMDn_MSGID_RMSK)
#define HWIO_RPMH_PDC_DISPLAY_TCSt_CMDn_MSGID_INMI2(t,n,mask)    \
        in_dword_masked(HWIO_RPMH_PDC_DISPLAY_TCSt_CMDn_MSGID_ADDR(t,n), mask)
#define HWIO_RPMH_PDC_DISPLAY_TCSt_CMDn_MSGID_OUTI2(t,n,val)    \
        out_dword(HWIO_RPMH_PDC_DISPLAY_TCSt_CMDn_MSGID_ADDR(t,n),val)
#define HWIO_RPMH_PDC_DISPLAY_TCSt_CMDn_MSGID_OUTMI2(t,n,mask,val) \
        out_dword_masked_ns(HWIO_RPMH_PDC_DISPLAY_TCSt_CMDn_MSGID_ADDR(t,n),mask,val,HWIO_RPMH_PDC_DISPLAY_TCSt_CMDn_MSGID_INI2(t,n))
#define HWIO_RPMH_PDC_DISPLAY_TCSt_CMDn_MSGID_READ_OR_WRITE_BMSK                                       0x10000
#define HWIO_RPMH_PDC_DISPLAY_TCSt_CMDn_MSGID_READ_OR_WRITE_SHFT                                          0x10
#define HWIO_RPMH_PDC_DISPLAY_TCSt_CMDn_MSGID_RES_REQ_BMSK                                               0x100
#define HWIO_RPMH_PDC_DISPLAY_TCSt_CMDn_MSGID_RES_REQ_SHFT                                                 0x8

#define HWIO_RPMH_PDC_DISPLAY_TCSt_CMDn_ADDR_ADDR(t,n)                                              (RPMH_PDC_DISPLAY_DISPLAY_PDC_REG_BASE      + 0x00005510 + 0xC8 * (t) + 0x10 * (n))
#define HWIO_RPMH_PDC_DISPLAY_TCSt_CMDn_ADDR_RMSK                                                      0x7fffc
#define HWIO_RPMH_PDC_DISPLAY_TCSt_CMDn_ADDR_MAXt                                                            2
#define HWIO_RPMH_PDC_DISPLAY_TCSt_CMDn_ADDR_MAXn                                                            3
#define HWIO_RPMH_PDC_DISPLAY_TCSt_CMDn_ADDR_INI2(t,n)        \
        in_dword_masked(HWIO_RPMH_PDC_DISPLAY_TCSt_CMDn_ADDR_ADDR(t,n), HWIO_RPMH_PDC_DISPLAY_TCSt_CMDn_ADDR_RMSK)
#define HWIO_RPMH_PDC_DISPLAY_TCSt_CMDn_ADDR_INMI2(t,n,mask)    \
        in_dword_masked(HWIO_RPMH_PDC_DISPLAY_TCSt_CMDn_ADDR_ADDR(t,n), mask)
#define HWIO_RPMH_PDC_DISPLAY_TCSt_CMDn_ADDR_OUTI2(t,n,val)    \
        out_dword(HWIO_RPMH_PDC_DISPLAY_TCSt_CMDn_ADDR_ADDR(t,n),val)
#define HWIO_RPMH_PDC_DISPLAY_TCSt_CMDn_ADDR_OUTMI2(t,n,mask,val) \
        out_dword_masked_ns(HWIO_RPMH_PDC_DISPLAY_TCSt_CMDn_ADDR_ADDR(t,n),mask,val,HWIO_RPMH_PDC_DISPLAY_TCSt_CMDn_ADDR_INI2(t,n))
#define HWIO_RPMH_PDC_DISPLAY_TCSt_CMDn_ADDR_SLV_ID_BMSK                                               0x70000
#define HWIO_RPMH_PDC_DISPLAY_TCSt_CMDn_ADDR_SLV_ID_SHFT                                                  0x10
#define HWIO_RPMH_PDC_DISPLAY_TCSt_CMDn_ADDR_OFFSET_BMSK                                                0xfffc
#define HWIO_RPMH_PDC_DISPLAY_TCSt_CMDn_ADDR_OFFSET_SHFT                                                   0x2

#define HWIO_RPMH_PDC_DISPLAY_TCSt_CMDn_DATA_ADDR(t,n)                                              (RPMH_PDC_DISPLAY_DISPLAY_PDC_REG_BASE      + 0x00005514 + 0xC8 * (t) + 0x10 * (n))
#define HWIO_RPMH_PDC_DISPLAY_TCSt_CMDn_DATA_RMSK                                                       0x1fff
#define HWIO_RPMH_PDC_DISPLAY_TCSt_CMDn_DATA_MAXt                                                            2
#define HWIO_RPMH_PDC_DISPLAY_TCSt_CMDn_DATA_MAXn                                                            3
#define HWIO_RPMH_PDC_DISPLAY_TCSt_CMDn_DATA_INI2(t,n)        \
        in_dword_masked(HWIO_RPMH_PDC_DISPLAY_TCSt_CMDn_DATA_ADDR(t,n), HWIO_RPMH_PDC_DISPLAY_TCSt_CMDn_DATA_RMSK)
#define HWIO_RPMH_PDC_DISPLAY_TCSt_CMDn_DATA_INMI2(t,n,mask)    \
        in_dword_masked(HWIO_RPMH_PDC_DISPLAY_TCSt_CMDn_DATA_ADDR(t,n), mask)
#define HWIO_RPMH_PDC_DISPLAY_TCSt_CMDn_DATA_OUTI2(t,n,val)    \
        out_dword(HWIO_RPMH_PDC_DISPLAY_TCSt_CMDn_DATA_ADDR(t,n),val)
#define HWIO_RPMH_PDC_DISPLAY_TCSt_CMDn_DATA_OUTMI2(t,n,mask,val) \
        out_dword_masked_ns(HWIO_RPMH_PDC_DISPLAY_TCSt_CMDn_DATA_ADDR(t,n),mask,val,HWIO_RPMH_PDC_DISPLAY_TCSt_CMDn_DATA_INI2(t,n))
#define HWIO_RPMH_PDC_DISPLAY_TCSt_CMDn_DATA_TCS_CMD_DATA_BMSK                                          0x1fff
#define HWIO_RPMH_PDC_DISPLAY_TCSt_CMDn_DATA_TCS_CMD_DATA_SHFT                                             0x0

/*----------------------------------------------------------------------------
 * MODULE: SDE_RSCC_RSCC_RSC
 *--------------------------------------------------------------------------*/

#define SDE_RSCC_RSCC_RSC_REG_BASE                                                                              (MDSS_BASE      + 0x00120000)
#define SDE_RSCC_RSCC_RSC_REG_BASE_SIZE                                                                         0x10000
#define SDE_RSCC_RSCC_RSC_REG_BASE_USED                                                                         0x254c

#define HWIO_SDE_RSCC_RSC_ID_DRVd_ADDR(d)                                                                       (SDE_RSCC_RSCC_RSC_REG_BASE      + 0x00000000 + 0x10000 * (d))
#define HWIO_SDE_RSCC_RSC_ID_DRVd_RMSK                                                                            0xffffff
#define HWIO_SDE_RSCC_RSC_ID_DRVd_MAXd                                                                                   0
#define HWIO_SDE_RSCC_RSC_ID_DRVd_INI(d)        \
        in_dword_masked(HWIO_SDE_RSCC_RSC_ID_DRVd_ADDR(d), HWIO_SDE_RSCC_RSC_ID_DRVd_RMSK)
#define HWIO_SDE_RSCC_RSC_ID_DRVd_INMI(d,mask)    \
        in_dword_masked(HWIO_SDE_RSCC_RSC_ID_DRVd_ADDR(d), mask)
#define HWIO_SDE_RSCC_RSC_ID_DRVd_MAJOR_VER_BMSK                                                                  0xff0000
#define HWIO_SDE_RSCC_RSC_ID_DRVd_MAJOR_VER_SHFT                                                                      0x10
#define HWIO_SDE_RSCC_RSC_ID_DRVd_MINOR_VER_BMSK                                                                    0xff00
#define HWIO_SDE_RSCC_RSC_ID_DRVd_MINOR_VER_SHFT                                                                       0x8
#define HWIO_SDE_RSCC_RSC_ID_DRVd_STEP_VER_BMSK                                                                       0xff
#define HWIO_SDE_RSCC_RSC_ID_DRVd_STEP_VER_SHFT                                                                        0x0

#define HWIO_SDE_RSCC_RSC_PARAM_SOLVER_CONFIG_DRVd_ADDR(d)                                                      (SDE_RSCC_RSCC_RSC_REG_BASE      + 0x00000004 + 0x10000 * (d))
#define HWIO_SDE_RSCC_RSC_PARAM_SOLVER_CONFIG_DRVd_RMSK                                                          0x71f1f1f
#define HWIO_SDE_RSCC_RSC_PARAM_SOLVER_CONFIG_DRVd_MAXd                                                                  0
#define HWIO_SDE_RSCC_RSC_PARAM_SOLVER_CONFIG_DRVd_INI(d)        \
        in_dword_masked(HWIO_SDE_RSCC_RSC_PARAM_SOLVER_CONFIG_DRVd_ADDR(d), HWIO_SDE_RSCC_RSC_PARAM_SOLVER_CONFIG_DRVd_RMSK)
#define HWIO_SDE_RSCC_RSC_PARAM_SOLVER_CONFIG_DRVd_INMI(d,mask)    \
        in_dword_masked(HWIO_SDE_RSCC_RSC_PARAM_SOLVER_CONFIG_DRVd_ADDR(d), mask)
#define HWIO_SDE_RSCC_RSC_PARAM_SOLVER_CONFIG_DRVd_SEQ_EXTENDED_BMSK                                             0x4000000
#define HWIO_SDE_RSCC_RSC_PARAM_SOLVER_CONFIG_DRVd_SEQ_EXTENDED_SHFT                                                  0x1a
#define HWIO_SDE_RSCC_RSC_PARAM_SOLVER_CONFIG_DRVd_IS_CHILD_RSC_BMSK                                             0x2000000
#define HWIO_SDE_RSCC_RSC_PARAM_SOLVER_CONFIG_DRVd_IS_CHILD_RSC_SHFT                                                  0x19
#define HWIO_SDE_RSCC_RSC_PARAM_SOLVER_CONFIG_DRVd_HW_SOLVER_BMSK                                                0x1000000
#define HWIO_SDE_RSCC_RSC_PARAM_SOLVER_CONFIG_DRVd_HW_SOLVER_SHFT                                                     0x18
#define HWIO_SDE_RSCC_RSC_PARAM_SOLVER_CONFIG_DRVd_SOLVER_SLOTS_BMSK                                              0x1f0000
#define HWIO_SDE_RSCC_RSC_PARAM_SOLVER_CONFIG_DRVd_SOLVER_SLOTS_SHFT                                                  0x10
#define HWIO_SDE_RSCC_RSC_PARAM_SOLVER_CONFIG_DRVd_SOLVER_MODES_BMSK                                                0x1f00
#define HWIO_SDE_RSCC_RSC_PARAM_SOLVER_CONFIG_DRVd_SOLVER_MODES_SHFT                                                   0x8
#define HWIO_SDE_RSCC_RSC_PARAM_SOLVER_CONFIG_DRVd_NUM_TIMERS_BMSK                                                    0x1f
#define HWIO_SDE_RSCC_RSC_PARAM_SOLVER_CONFIG_DRVd_NUM_TIMERS_SHFT                                                     0x0

#define HWIO_SDE_RSCC_RSC_PARAM_RSC_CONFIG_DRVd_ADDR(d)                                                         (SDE_RSCC_RSCC_RSC_REG_BASE      + 0x00000008 + 0x10000 * (d))
#define HWIO_SDE_RSCC_RSC_PARAM_RSC_CONFIG_DRVd_RMSK                                                             0x7ff0f1f
#define HWIO_SDE_RSCC_RSC_PARAM_RSC_CONFIG_DRVd_MAXd                                                                     0
#define HWIO_SDE_RSCC_RSC_PARAM_RSC_CONFIG_DRVd_INI(d)        \
        in_dword_masked(HWIO_SDE_RSCC_RSC_PARAM_RSC_CONFIG_DRVd_ADDR(d), HWIO_SDE_RSCC_RSC_PARAM_RSC_CONFIG_DRVd_RMSK)
#define HWIO_SDE_RSCC_RSC_PARAM_RSC_CONFIG_DRVd_INMI(d,mask)    \
        in_dword_masked(HWIO_SDE_RSCC_RSC_PARAM_RSC_CONFIG_DRVd_ADDR(d), mask)
#define HWIO_SDE_RSCC_RSC_PARAM_RSC_CONFIG_DRVd_NUM_DRV_BMSK                                                     0x7000000
#define HWIO_SDE_RSCC_RSC_PARAM_RSC_CONFIG_DRVd_NUM_DRV_SHFT                                                          0x18
#define HWIO_SDE_RSCC_RSC_PARAM_RSC_CONFIG_DRVd_NUM_SEQ_CMD_WORDS_BMSK                                            0xff0000
#define HWIO_SDE_RSCC_RSC_PARAM_RSC_CONFIG_DRVd_NUM_SEQ_CMD_WORDS_SHFT                                                0x10
#define HWIO_SDE_RSCC_RSC_PARAM_RSC_CONFIG_DRVd_NUM_TS_EVENTS_BMSK                                                   0xf00
#define HWIO_SDE_RSCC_RSC_PARAM_RSC_CONFIG_DRVd_NUM_TS_EVENTS_SHFT                                                     0x8
#define HWIO_SDE_RSCC_RSC_PARAM_RSC_CONFIG_DRVd_DELAY_CNTR_BITWIDTH_BMSK                                              0x1f
#define HWIO_SDE_RSCC_RSC_PARAM_RSC_CONFIG_DRVd_DELAY_CNTR_BITWIDTH_SHFT                                               0x0

#define HWIO_SDE_RSCC_RSC_PARAM_RSC_PARENTCHILD_CONFIG_DRVd_ADDR(d)                                             (SDE_RSCC_RSCC_RSC_REG_BASE      + 0x0000000c + 0x10000 * (d))
#define HWIO_SDE_RSCC_RSC_PARAM_RSC_PARENTCHILD_CONFIG_DRVd_RMSK                                                0xf8ffffff
#define HWIO_SDE_RSCC_RSC_PARAM_RSC_PARENTCHILD_CONFIG_DRVd_MAXd                                                         0
#define HWIO_SDE_RSCC_RSC_PARAM_RSC_PARENTCHILD_CONFIG_DRVd_INI(d)        \
        in_dword_masked(HWIO_SDE_RSCC_RSC_PARAM_RSC_PARENTCHILD_CONFIG_DRVd_ADDR(d), HWIO_SDE_RSCC_RSC_PARAM_RSC_PARENTCHILD_CONFIG_DRVd_RMSK)
#define HWIO_SDE_RSCC_RSC_PARAM_RSC_PARENTCHILD_CONFIG_DRVd_INMI(d,mask)    \
        in_dword_masked(HWIO_SDE_RSCC_RSC_PARAM_RSC_PARENTCHILD_CONFIG_DRVd_ADDR(d), mask)
#define HWIO_SDE_RSCC_RSC_PARAM_RSC_PARENTCHILD_CONFIG_DRVd_NUM_CMDS_PER_TCS_BMSK                               0xf8000000
#define HWIO_SDE_RSCC_RSC_PARAM_RSC_PARENTCHILD_CONFIG_DRVd_NUM_CMDS_PER_TCS_SHFT                                     0x1b
#define HWIO_SDE_RSCC_RSC_PARAM_RSC_PARENTCHILD_CONFIG_DRVd_NUM_TCS_DRV3_BMSK                                     0xfc0000
#define HWIO_SDE_RSCC_RSC_PARAM_RSC_PARENTCHILD_CONFIG_DRVd_NUM_TCS_DRV3_SHFT                                         0x12
#define HWIO_SDE_RSCC_RSC_PARAM_RSC_PARENTCHILD_CONFIG_DRVd_NUM_TCS_DRV2_BMSK                                      0x3f000
#define HWIO_SDE_RSCC_RSC_PARAM_RSC_PARENTCHILD_CONFIG_DRVd_NUM_TCS_DRV2_SHFT                                          0xc
#define HWIO_SDE_RSCC_RSC_PARAM_RSC_PARENTCHILD_CONFIG_DRVd_NUM_TCS_DRV1_BMSK                                        0xfc0
#define HWIO_SDE_RSCC_RSC_PARAM_RSC_PARENTCHILD_CONFIG_DRVd_NUM_TCS_DRV1_SHFT                                          0x6
#define HWIO_SDE_RSCC_RSC_PARAM_RSC_PARENTCHILD_CONFIG_DRVd_NUM_TCS_DRV0_BMSK                                         0x3f
#define HWIO_SDE_RSCC_RSC_PARAM_RSC_PARENTCHILD_CONFIG_DRVd_NUM_TCS_DRV0_SHFT                                          0x0

#define HWIO_SDE_RSCC_RSC_STATUS0_DRVd_ADDR(d)                                                                  (SDE_RSCC_RSCC_RSC_REG_BASE      + 0x00000010 + 0x10000 * (d))
#define HWIO_SDE_RSCC_RSC_STATUS0_DRVd_RMSK                                                                      0x101ffff
#define HWIO_SDE_RSCC_RSC_STATUS0_DRVd_MAXd                                                                              0
#define HWIO_SDE_RSCC_RSC_STATUS0_DRVd_INI(d)        \
        in_dword_masked(HWIO_SDE_RSCC_RSC_STATUS0_DRVd_ADDR(d), HWIO_SDE_RSCC_RSC_STATUS0_DRVd_RMSK)
#define HWIO_SDE_RSCC_RSC_STATUS0_DRVd_INMI(d,mask)    \
        in_dword_masked(HWIO_SDE_RSCC_RSC_STATUS0_DRVd_ADDR(d), mask)
#define HWIO_SDE_RSCC_RSC_STATUS0_DRVd_OUTI(d,val)    \
        out_dword(HWIO_SDE_RSCC_RSC_STATUS0_DRVd_ADDR(d),val)
#define HWIO_SDE_RSCC_RSC_STATUS0_DRVd_OUTMI(d,mask,val) \
        out_dword_masked_ns(HWIO_SDE_RSCC_RSC_STATUS0_DRVd_ADDR(d),mask,val,HWIO_SDE_RSCC_RSC_STATUS0_DRVd_INI(d))
#define HWIO_SDE_RSCC_RSC_STATUS0_DRVd_CLK_GATE_DISABLE_BMSK                                                     0x1000000
#define HWIO_SDE_RSCC_RSC_STATUS0_DRVd_CLK_GATE_DISABLE_SHFT                                                          0x18
#define HWIO_SDE_RSCC_RSC_STATUS0_DRVd_STATUS_SLEEP_ENTRY_DONE_BMSK                                                0x10000
#define HWIO_SDE_RSCC_RSC_STATUS0_DRVd_STATUS_SLEEP_ENTRY_DONE_SHFT                                                   0x10
#define HWIO_SDE_RSCC_RSC_STATUS0_DRVd_STATUS_SLEEP_CTL_FSM_BMSK                                                    0xffff
#define HWIO_SDE_RSCC_RSC_STATUS0_DRVd_STATUS_SLEEP_CTL_FSM_SHFT                                                       0x0

#define HWIO_SDE_RSCC_RSC_STATUS1_DRVd_ADDR(d)                                                                  (SDE_RSCC_RSCC_RSC_REG_BASE      + 0x00000014 + 0x10000 * (d))
#define HWIO_SDE_RSCC_RSC_STATUS1_DRVd_RMSK                                                                     0xffffffff
#define HWIO_SDE_RSCC_RSC_STATUS1_DRVd_MAXd                                                                              0
#define HWIO_SDE_RSCC_RSC_STATUS1_DRVd_INI(d)        \
        in_dword_masked(HWIO_SDE_RSCC_RSC_STATUS1_DRVd_ADDR(d), HWIO_SDE_RSCC_RSC_STATUS1_DRVd_RMSK)
#define HWIO_SDE_RSCC_RSC_STATUS1_DRVd_INMI(d,mask)    \
        in_dword_masked(HWIO_SDE_RSCC_RSC_STATUS1_DRVd_ADDR(d), mask)
#define HWIO_SDE_RSCC_RSC_STATUS1_DRVd_PDC_MATCH_VALUE_LO_BMSK                                                  0xffffffff
#define HWIO_SDE_RSCC_RSC_STATUS1_DRVd_PDC_MATCH_VALUE_LO_SHFT                                                         0x0

#define HWIO_SDE_RSCC_RSC_STATUS2_DRVd_ADDR(d)                                                                  (SDE_RSCC_RSCC_RSC_REG_BASE      + 0x00000018 + 0x10000 * (d))
#define HWIO_SDE_RSCC_RSC_STATUS2_DRVd_RMSK                                                                       0xffffff
#define HWIO_SDE_RSCC_RSC_STATUS2_DRVd_MAXd                                                                              0
#define HWIO_SDE_RSCC_RSC_STATUS2_DRVd_INI(d)        \
        in_dword_masked(HWIO_SDE_RSCC_RSC_STATUS2_DRVd_ADDR(d), HWIO_SDE_RSCC_RSC_STATUS2_DRVd_RMSK)
#define HWIO_SDE_RSCC_RSC_STATUS2_DRVd_INMI(d,mask)    \
        in_dword_masked(HWIO_SDE_RSCC_RSC_STATUS2_DRVd_ADDR(d), mask)
#define HWIO_SDE_RSCC_RSC_STATUS2_DRVd_PDC_MATCH_VALUE_HI_BMSK                                                    0xffffff
#define HWIO_SDE_RSCC_RSC_STATUS2_DRVd_PDC_MATCH_VALUE_HI_SHFT                                                         0x0

#define HWIO_SDE_RSCC_HIDDEN_TCS_CTRL_DRV0_ADDR                                                                 (SDE_RSCC_RSCC_RSC_REG_BASE      + 0x0000001c)
#define HWIO_SDE_RSCC_HIDDEN_TCS_CTRL_DRV0_RMSK                                                                    0x10003
#define HWIO_SDE_RSCC_HIDDEN_TCS_CTRL_DRV0_IN          \
        in_dword_masked(HWIO_SDE_RSCC_HIDDEN_TCS_CTRL_DRV0_ADDR, HWIO_SDE_RSCC_HIDDEN_TCS_CTRL_DRV0_RMSK)
#define HWIO_SDE_RSCC_HIDDEN_TCS_CTRL_DRV0_INM(m)      \
        in_dword_masked(HWIO_SDE_RSCC_HIDDEN_TCS_CTRL_DRV0_ADDR, m)
#define HWIO_SDE_RSCC_HIDDEN_TCS_CTRL_DRV0_OUT(v)      \
        out_dword(HWIO_SDE_RSCC_HIDDEN_TCS_CTRL_DRV0_ADDR,v)
#define HWIO_SDE_RSCC_HIDDEN_TCS_CTRL_DRV0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_SDE_RSCC_HIDDEN_TCS_CTRL_DRV0_ADDR,m,v,HWIO_SDE_RSCC_HIDDEN_TCS_CTRL_DRV0_IN)
#define HWIO_SDE_RSCC_HIDDEN_TCS_CTRL_DRV0_HIDDEN_TCS_TRIGGER_BMSK                                                 0x10000
#define HWIO_SDE_RSCC_HIDDEN_TCS_CTRL_DRV0_HIDDEN_TCS_TRIGGER_SHFT                                                    0x10
#define HWIO_SDE_RSCC_HIDDEN_TCS_CTRL_DRV0_HIDDEN_TCS_PDC_TIMER_DRV_OWNER_BMSK                                         0x3
#define HWIO_SDE_RSCC_HIDDEN_TCS_CTRL_DRV0_HIDDEN_TCS_PDC_TIMER_DRV_OWNER_SHFT                                         0x0

#define HWIO_SDE_RSCC_PDC_SEQ_START_ADDR_REG_OFFSET_DRV0_ADDR                                                   (SDE_RSCC_RSCC_RSC_REG_BASE      + 0x00000020)
#define HWIO_SDE_RSCC_PDC_SEQ_START_ADDR_REG_OFFSET_DRV0_RMSK                                                       0xffff
#define HWIO_SDE_RSCC_PDC_SEQ_START_ADDR_REG_OFFSET_DRV0_IN          \
        in_dword_masked(HWIO_SDE_RSCC_PDC_SEQ_START_ADDR_REG_OFFSET_DRV0_ADDR, HWIO_SDE_RSCC_PDC_SEQ_START_ADDR_REG_OFFSET_DRV0_RMSK)
#define HWIO_SDE_RSCC_PDC_SEQ_START_ADDR_REG_OFFSET_DRV0_INM(m)      \
        in_dword_masked(HWIO_SDE_RSCC_PDC_SEQ_START_ADDR_REG_OFFSET_DRV0_ADDR, m)
#define HWIO_SDE_RSCC_PDC_SEQ_START_ADDR_REG_OFFSET_DRV0_OUT(v)      \
        out_dword(HWIO_SDE_RSCC_PDC_SEQ_START_ADDR_REG_OFFSET_DRV0_ADDR,v)
#define HWIO_SDE_RSCC_PDC_SEQ_START_ADDR_REG_OFFSET_DRV0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_SDE_RSCC_PDC_SEQ_START_ADDR_REG_OFFSET_DRV0_ADDR,m,v,HWIO_SDE_RSCC_PDC_SEQ_START_ADDR_REG_OFFSET_DRV0_IN)
#define HWIO_SDE_RSCC_PDC_SEQ_START_ADDR_REG_OFFSET_DRV0_PDC_SEQ_START_ADDR_REG_OFFSET_BMSK                         0xffff
#define HWIO_SDE_RSCC_PDC_SEQ_START_ADDR_REG_OFFSET_DRV0_PDC_SEQ_START_ADDR_REG_OFFSET_SHFT                            0x0

#define HWIO_SDE_RSCC_PDC_MATCH_VALUE_LO_REG_OFFSET_DRV0_ADDR                                                   (SDE_RSCC_RSCC_RSC_REG_BASE      + 0x00000024)
#define HWIO_SDE_RSCC_PDC_MATCH_VALUE_LO_REG_OFFSET_DRV0_RMSK                                                       0xffff
#define HWIO_SDE_RSCC_PDC_MATCH_VALUE_LO_REG_OFFSET_DRV0_IN          \
        in_dword_masked(HWIO_SDE_RSCC_PDC_MATCH_VALUE_LO_REG_OFFSET_DRV0_ADDR, HWIO_SDE_RSCC_PDC_MATCH_VALUE_LO_REG_OFFSET_DRV0_RMSK)
#define HWIO_SDE_RSCC_PDC_MATCH_VALUE_LO_REG_OFFSET_DRV0_INM(m)      \
        in_dword_masked(HWIO_SDE_RSCC_PDC_MATCH_VALUE_LO_REG_OFFSET_DRV0_ADDR, m)
#define HWIO_SDE_RSCC_PDC_MATCH_VALUE_LO_REG_OFFSET_DRV0_OUT(v)      \
        out_dword(HWIO_SDE_RSCC_PDC_MATCH_VALUE_LO_REG_OFFSET_DRV0_ADDR,v)
#define HWIO_SDE_RSCC_PDC_MATCH_VALUE_LO_REG_OFFSET_DRV0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_SDE_RSCC_PDC_MATCH_VALUE_LO_REG_OFFSET_DRV0_ADDR,m,v,HWIO_SDE_RSCC_PDC_MATCH_VALUE_LO_REG_OFFSET_DRV0_IN)
#define HWIO_SDE_RSCC_PDC_MATCH_VALUE_LO_REG_OFFSET_DRV0_PDC_MATCH_VALUE_LO_REG_OFFSET_BMSK                         0xffff
#define HWIO_SDE_RSCC_PDC_MATCH_VALUE_LO_REG_OFFSET_DRV0_PDC_MATCH_VALUE_LO_REG_OFFSET_SHFT                            0x0

#define HWIO_SDE_RSCC_PDC_MATCH_VALUE_HI_REG_OFFSET_DRV0_ADDR                                                   (SDE_RSCC_RSCC_RSC_REG_BASE      + 0x00000028)
#define HWIO_SDE_RSCC_PDC_MATCH_VALUE_HI_REG_OFFSET_DRV0_RMSK                                                       0xffff
#define HWIO_SDE_RSCC_PDC_MATCH_VALUE_HI_REG_OFFSET_DRV0_IN          \
        in_dword_masked(HWIO_SDE_RSCC_PDC_MATCH_VALUE_HI_REG_OFFSET_DRV0_ADDR, HWIO_SDE_RSCC_PDC_MATCH_VALUE_HI_REG_OFFSET_DRV0_RMSK)
#define HWIO_SDE_RSCC_PDC_MATCH_VALUE_HI_REG_OFFSET_DRV0_INM(m)      \
        in_dword_masked(HWIO_SDE_RSCC_PDC_MATCH_VALUE_HI_REG_OFFSET_DRV0_ADDR, m)
#define HWIO_SDE_RSCC_PDC_MATCH_VALUE_HI_REG_OFFSET_DRV0_OUT(v)      \
        out_dword(HWIO_SDE_RSCC_PDC_MATCH_VALUE_HI_REG_OFFSET_DRV0_ADDR,v)
#define HWIO_SDE_RSCC_PDC_MATCH_VALUE_HI_REG_OFFSET_DRV0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_SDE_RSCC_PDC_MATCH_VALUE_HI_REG_OFFSET_DRV0_ADDR,m,v,HWIO_SDE_RSCC_PDC_MATCH_VALUE_HI_REG_OFFSET_DRV0_IN)
#define HWIO_SDE_RSCC_PDC_MATCH_VALUE_HI_REG_OFFSET_DRV0_PDC_MATCH_VALUE_HI_REG_OFFSET_BMSK                         0xffff
#define HWIO_SDE_RSCC_PDC_MATCH_VALUE_HI_REG_OFFSET_DRV0_PDC_MATCH_VALUE_HI_REG_OFFSET_SHFT                            0x0

#define HWIO_SDE_RSCC_PDC_SLAVE_ID_DRV0_ADDR                                                                    (SDE_RSCC_RSCC_RSC_REG_BASE      + 0x0000002c)
#define HWIO_SDE_RSCC_PDC_SLAVE_ID_DRV0_RMSK                                                                           0x7
#define HWIO_SDE_RSCC_PDC_SLAVE_ID_DRV0_IN          \
        in_dword_masked(HWIO_SDE_RSCC_PDC_SLAVE_ID_DRV0_ADDR, HWIO_SDE_RSCC_PDC_SLAVE_ID_DRV0_RMSK)
#define HWIO_SDE_RSCC_PDC_SLAVE_ID_DRV0_INM(m)      \
        in_dword_masked(HWIO_SDE_RSCC_PDC_SLAVE_ID_DRV0_ADDR, m)
#define HWIO_SDE_RSCC_PDC_SLAVE_ID_DRV0_OUT(v)      \
        out_dword(HWIO_SDE_RSCC_PDC_SLAVE_ID_DRV0_ADDR,v)
#define HWIO_SDE_RSCC_PDC_SLAVE_ID_DRV0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_SDE_RSCC_PDC_SLAVE_ID_DRV0_ADDR,m,v,HWIO_SDE_RSCC_PDC_SLAVE_ID_DRV0_IN)
#define HWIO_SDE_RSCC_PDC_SLAVE_ID_DRV0_PDC_SLAVE_ID_BMSK                                                              0x7
#define HWIO_SDE_RSCC_PDC_SLAVE_ID_DRV0_PDC_SLAVE_ID_SHFT                                                              0x0

#define HWIO_SDE_RSCC_HIDDEN_TCS_STATUS_DRVd_ADDR(d)                                                            (SDE_RSCC_RSCC_RSC_REG_BASE      + 0x00000030 + 0x10000 * (d))
#define HWIO_SDE_RSCC_HIDDEN_TCS_STATUS_DRVd_RMSK                                                                  0x70707
#define HWIO_SDE_RSCC_HIDDEN_TCS_STATUS_DRVd_MAXd                                                                        0
#define HWIO_SDE_RSCC_HIDDEN_TCS_STATUS_DRVd_INI(d)        \
        in_dword_masked(HWIO_SDE_RSCC_HIDDEN_TCS_STATUS_DRVd_ADDR(d), HWIO_SDE_RSCC_HIDDEN_TCS_STATUS_DRVd_RMSK)
#define HWIO_SDE_RSCC_HIDDEN_TCS_STATUS_DRVd_INMI(d,mask)    \
        in_dword_masked(HWIO_SDE_RSCC_HIDDEN_TCS_STATUS_DRVd_ADDR(d), mask)
#define HWIO_SDE_RSCC_HIDDEN_TCS_STATUS_DRVd_COMPLETED_BMSK                                                        0x70000
#define HWIO_SDE_RSCC_HIDDEN_TCS_STATUS_DRVd_COMPLETED_SHFT                                                           0x10
#define HWIO_SDE_RSCC_HIDDEN_TCS_STATUS_DRVd_ISSUED_BMSK                                                             0x700
#define HWIO_SDE_RSCC_HIDDEN_TCS_STATUS_DRVd_ISSUED_SHFT                                                               0x8
#define HWIO_SDE_RSCC_HIDDEN_TCS_STATUS_DRVd_TRIGGERED_BMSK                                                            0x7
#define HWIO_SDE_RSCC_HIDDEN_TCS_STATUS_DRVd_TRIGGERED_SHFT                                                            0x0

#define HWIO_SDE_RSCC_HIDDEN_TCS_CMD0_ADDR_DRVd_ADDR(d)                                                         (SDE_RSCC_RSCC_RSC_REG_BASE      + 0x00000034 + 0x10000 * (d))
#define HWIO_SDE_RSCC_HIDDEN_TCS_CMD0_ADDR_DRVd_RMSK                                                            0xffffffff
#define HWIO_SDE_RSCC_HIDDEN_TCS_CMD0_ADDR_DRVd_MAXd                                                                     0
#define HWIO_SDE_RSCC_HIDDEN_TCS_CMD0_ADDR_DRVd_INI(d)        \
        in_dword_masked(HWIO_SDE_RSCC_HIDDEN_TCS_CMD0_ADDR_DRVd_ADDR(d), HWIO_SDE_RSCC_HIDDEN_TCS_CMD0_ADDR_DRVd_RMSK)
#define HWIO_SDE_RSCC_HIDDEN_TCS_CMD0_ADDR_DRVd_INMI(d,mask)    \
        in_dword_masked(HWIO_SDE_RSCC_HIDDEN_TCS_CMD0_ADDR_DRVd_ADDR(d), mask)
#define HWIO_SDE_RSCC_HIDDEN_TCS_CMD0_ADDR_DRVd_ADDR_BMSK                                                       0xffffffff
#define HWIO_SDE_RSCC_HIDDEN_TCS_CMD0_ADDR_DRVd_ADDR_SHFT                                                              0x0

#define HWIO_SDE_RSCC_HIDDEN_TCS_CMD0_DATA_DRVd_ADDR(d)                                                         (SDE_RSCC_RSCC_RSC_REG_BASE      + 0x00000038 + 0x10000 * (d))
#define HWIO_SDE_RSCC_HIDDEN_TCS_CMD0_DATA_DRVd_RMSK                                                            0x80ffffff
#define HWIO_SDE_RSCC_HIDDEN_TCS_CMD0_DATA_DRVd_MAXd                                                                     0
#define HWIO_SDE_RSCC_HIDDEN_TCS_CMD0_DATA_DRVd_INI(d)        \
        in_dword_masked(HWIO_SDE_RSCC_HIDDEN_TCS_CMD0_DATA_DRVd_ADDR(d), HWIO_SDE_RSCC_HIDDEN_TCS_CMD0_DATA_DRVd_RMSK)
#define HWIO_SDE_RSCC_HIDDEN_TCS_CMD0_DATA_DRVd_INMI(d,mask)    \
        in_dword_masked(HWIO_SDE_RSCC_HIDDEN_TCS_CMD0_DATA_DRVd_ADDR(d), mask)
#define HWIO_SDE_RSCC_HIDDEN_TCS_CMD0_DATA_DRVd_OUTI(d,val)    \
        out_dword(HWIO_SDE_RSCC_HIDDEN_TCS_CMD0_DATA_DRVd_ADDR(d),val)
#define HWIO_SDE_RSCC_HIDDEN_TCS_CMD0_DATA_DRVd_OUTMI(d,mask,val) \
        out_dword_masked_ns(HWIO_SDE_RSCC_HIDDEN_TCS_CMD0_DATA_DRVd_ADDR(d),mask,val,HWIO_SDE_RSCC_HIDDEN_TCS_CMD0_DATA_DRVd_INI(d))
#define HWIO_SDE_RSCC_HIDDEN_TCS_CMD0_DATA_DRVd_PDC_MATCH_VALUE_VALID_BMSK                                      0x80000000
#define HWIO_SDE_RSCC_HIDDEN_TCS_CMD0_DATA_DRVd_PDC_MATCH_VALUE_VALID_SHFT                                            0x1f
#define HWIO_SDE_RSCC_HIDDEN_TCS_CMD0_DATA_DRVd_PDC_MATCH_VALUE_HI_BMSK                                           0xffffff
#define HWIO_SDE_RSCC_HIDDEN_TCS_CMD0_DATA_DRVd_PDC_MATCH_VALUE_HI_SHFT                                                0x0

#define HWIO_SDE_RSCC_HIDDEN_TCS_CMD1_ADDR_DRVd_ADDR(d)                                                         (SDE_RSCC_RSCC_RSC_REG_BASE      + 0x0000003c + 0x10000 * (d))
#define HWIO_SDE_RSCC_HIDDEN_TCS_CMD1_ADDR_DRVd_RMSK                                                            0xffffffff
#define HWIO_SDE_RSCC_HIDDEN_TCS_CMD1_ADDR_DRVd_MAXd                                                                     0
#define HWIO_SDE_RSCC_HIDDEN_TCS_CMD1_ADDR_DRVd_INI(d)        \
        in_dword_masked(HWIO_SDE_RSCC_HIDDEN_TCS_CMD1_ADDR_DRVd_ADDR(d), HWIO_SDE_RSCC_HIDDEN_TCS_CMD1_ADDR_DRVd_RMSK)
#define HWIO_SDE_RSCC_HIDDEN_TCS_CMD1_ADDR_DRVd_INMI(d,mask)    \
        in_dword_masked(HWIO_SDE_RSCC_HIDDEN_TCS_CMD1_ADDR_DRVd_ADDR(d), mask)
#define HWIO_SDE_RSCC_HIDDEN_TCS_CMD1_ADDR_DRVd_ADDR_BMSK                                                       0xffffffff
#define HWIO_SDE_RSCC_HIDDEN_TCS_CMD1_ADDR_DRVd_ADDR_SHFT                                                              0x0

#define HWIO_SDE_RSCC_HIDDEN_TCS_CMD1_DATA_DRVd_ADDR(d)                                                         (SDE_RSCC_RSCC_RSC_REG_BASE      + 0x00000040 + 0x10000 * (d))
#define HWIO_SDE_RSCC_HIDDEN_TCS_CMD1_DATA_DRVd_RMSK                                                            0xffffffff
#define HWIO_SDE_RSCC_HIDDEN_TCS_CMD1_DATA_DRVd_MAXd                                                                     0
#define HWIO_SDE_RSCC_HIDDEN_TCS_CMD1_DATA_DRVd_INI(d)        \
        in_dword_masked(HWIO_SDE_RSCC_HIDDEN_TCS_CMD1_DATA_DRVd_ADDR(d), HWIO_SDE_RSCC_HIDDEN_TCS_CMD1_DATA_DRVd_RMSK)
#define HWIO_SDE_RSCC_HIDDEN_TCS_CMD1_DATA_DRVd_INMI(d,mask)    \
        in_dword_masked(HWIO_SDE_RSCC_HIDDEN_TCS_CMD1_DATA_DRVd_ADDR(d), mask)
#define HWIO_SDE_RSCC_HIDDEN_TCS_CMD1_DATA_DRVd_OUTI(d,val)    \
        out_dword(HWIO_SDE_RSCC_HIDDEN_TCS_CMD1_DATA_DRVd_ADDR(d),val)
#define HWIO_SDE_RSCC_HIDDEN_TCS_CMD1_DATA_DRVd_OUTMI(d,mask,val) \
        out_dword_masked_ns(HWIO_SDE_RSCC_HIDDEN_TCS_CMD1_DATA_DRVd_ADDR(d),mask,val,HWIO_SDE_RSCC_HIDDEN_TCS_CMD1_DATA_DRVd_INI(d))
#define HWIO_SDE_RSCC_HIDDEN_TCS_CMD1_DATA_DRVd_PDC_MATCH_VALUE_LO_BMSK                                         0xffffffff
#define HWIO_SDE_RSCC_HIDDEN_TCS_CMD1_DATA_DRVd_PDC_MATCH_VALUE_LO_SHFT                                                0x0

#define HWIO_SDE_RSCC_HIDDEN_TCS_CMD2_ADDR_DRVd_ADDR(d)                                                         (SDE_RSCC_RSCC_RSC_REG_BASE      + 0x00000044 + 0x10000 * (d))
#define HWIO_SDE_RSCC_HIDDEN_TCS_CMD2_ADDR_DRVd_RMSK                                                            0xffffffff
#define HWIO_SDE_RSCC_HIDDEN_TCS_CMD2_ADDR_DRVd_MAXd                                                                     0
#define HWIO_SDE_RSCC_HIDDEN_TCS_CMD2_ADDR_DRVd_INI(d)        \
        in_dword_masked(HWIO_SDE_RSCC_HIDDEN_TCS_CMD2_ADDR_DRVd_ADDR(d), HWIO_SDE_RSCC_HIDDEN_TCS_CMD2_ADDR_DRVd_RMSK)
#define HWIO_SDE_RSCC_HIDDEN_TCS_CMD2_ADDR_DRVd_INMI(d,mask)    \
        in_dword_masked(HWIO_SDE_RSCC_HIDDEN_TCS_CMD2_ADDR_DRVd_ADDR(d), mask)
#define HWIO_SDE_RSCC_HIDDEN_TCS_CMD2_ADDR_DRVd_ADDR_BMSK                                                       0xffffffff
#define HWIO_SDE_RSCC_HIDDEN_TCS_CMD2_ADDR_DRVd_ADDR_SHFT                                                              0x0

#define HWIO_SDE_RSCC_HIDDEN_TCS_CMD2_DATA_DRV0_ADDR                                                            (SDE_RSCC_RSCC_RSC_REG_BASE      + 0x00000048)
#define HWIO_SDE_RSCC_HIDDEN_TCS_CMD2_DATA_DRV0_RMSK                                                            0x8000ffff
#define HWIO_SDE_RSCC_HIDDEN_TCS_CMD2_DATA_DRV0_IN          \
        in_dword_masked(HWIO_SDE_RSCC_HIDDEN_TCS_CMD2_DATA_DRV0_ADDR, HWIO_SDE_RSCC_HIDDEN_TCS_CMD2_DATA_DRV0_RMSK)
#define HWIO_SDE_RSCC_HIDDEN_TCS_CMD2_DATA_DRV0_INM(m)      \
        in_dword_masked(HWIO_SDE_RSCC_HIDDEN_TCS_CMD2_DATA_DRV0_ADDR, m)
#define HWIO_SDE_RSCC_HIDDEN_TCS_CMD2_DATA_DRV0_OUT(v)      \
        out_dword(HWIO_SDE_RSCC_HIDDEN_TCS_CMD2_DATA_DRV0_ADDR,v)
#define HWIO_SDE_RSCC_HIDDEN_TCS_CMD2_DATA_DRV0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_SDE_RSCC_HIDDEN_TCS_CMD2_DATA_DRV0_ADDR,m,v,HWIO_SDE_RSCC_HIDDEN_TCS_CMD2_DATA_DRV0_IN)
#define HWIO_SDE_RSCC_HIDDEN_TCS_CMD2_DATA_DRV0_PDC_SEQ_START_ADDR_VALID_BMSK                                   0x80000000
#define HWIO_SDE_RSCC_HIDDEN_TCS_CMD2_DATA_DRV0_PDC_SEQ_START_ADDR_VALID_SHFT                                         0x1f
#define HWIO_SDE_RSCC_HIDDEN_TCS_CMD2_DATA_DRV0_PDC_SEQ_START_ADDR_BMSK                                             0xffff
#define HWIO_SDE_RSCC_HIDDEN_TCS_CMD2_DATA_DRV0_PDC_SEQ_START_ADDR_SHFT                                                0x0

#define HWIO_SDE_RSCC_HW_EVENT_OWNER_DRV0_ADDR                                                                  (SDE_RSCC_RSCC_RSC_REG_BASE      + 0x0000004c)
#define HWIO_SDE_RSCC_HW_EVENT_OWNER_DRV0_RMSK                                                                         0x3
#define HWIO_SDE_RSCC_HW_EVENT_OWNER_DRV0_IN          \
        in_dword_masked(HWIO_SDE_RSCC_HW_EVENT_OWNER_DRV0_ADDR, HWIO_SDE_RSCC_HW_EVENT_OWNER_DRV0_RMSK)
#define HWIO_SDE_RSCC_HW_EVENT_OWNER_DRV0_INM(m)      \
        in_dword_masked(HWIO_SDE_RSCC_HW_EVENT_OWNER_DRV0_ADDR, m)
#define HWIO_SDE_RSCC_HW_EVENT_OWNER_DRV0_OUT(v)      \
        out_dword(HWIO_SDE_RSCC_HW_EVENT_OWNER_DRV0_ADDR,v)
#define HWIO_SDE_RSCC_HW_EVENT_OWNER_DRV0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_SDE_RSCC_HW_EVENT_OWNER_DRV0_ADDR,m,v,HWIO_SDE_RSCC_HW_EVENT_OWNER_DRV0_IN)
#define HWIO_SDE_RSCC_HW_EVENT_OWNER_DRV0_OWNER_BMSK                                                                   0x3
#define HWIO_SDE_RSCC_HW_EVENT_OWNER_DRV0_OWNER_SHFT                                                                   0x0

#define HWIO_SDE_RSCC_HW_EVENT_MUXm_SELECT_DRVd_ADDR(d,m)                                                       (SDE_RSCC_RSCC_RSC_REG_BASE      + 0x00000050 + 0x4 * (m) + 0x10000 * (d))
#define HWIO_SDE_RSCC_HW_EVENT_MUXm_SELECT_DRVd_RMSK                                                            0x80000007
#define HWIO_SDE_RSCC_HW_EVENT_MUXm_SELECT_DRVd_MAXd                                                                     0
#define HWIO_SDE_RSCC_HW_EVENT_MUXm_SELECT_DRVd_MAXm                                                                    31
#define HWIO_SDE_RSCC_HW_EVENT_MUXm_SELECT_DRVd_INI2(d,m)        \
        in_dword_masked(HWIO_SDE_RSCC_HW_EVENT_MUXm_SELECT_DRVd_ADDR(d,m), HWIO_SDE_RSCC_HW_EVENT_MUXm_SELECT_DRVd_RMSK)
#define HWIO_SDE_RSCC_HW_EVENT_MUXm_SELECT_DRVd_INMI2(d,m,mask)    \
        in_dword_masked(HWIO_SDE_RSCC_HW_EVENT_MUXm_SELECT_DRVd_ADDR(d,m), mask)
#define HWIO_SDE_RSCC_HW_EVENT_MUXm_SELECT_DRVd_OUTI2(d,m,val)    \
        out_dword(HWIO_SDE_RSCC_HW_EVENT_MUXm_SELECT_DRVd_ADDR(d,m),val)
#define HWIO_SDE_RSCC_HW_EVENT_MUXm_SELECT_DRVd_OUTMI2(d,m,mask,val) \
        out_dword_masked_ns(HWIO_SDE_RSCC_HW_EVENT_MUXm_SELECT_DRVd_ADDR(d,m),mask,val,HWIO_SDE_RSCC_HW_EVENT_MUXm_SELECT_DRVd_INI2(d,m))
#define HWIO_SDE_RSCC_HW_EVENT_MUXm_SELECT_DRVd_ENABLE_BMSK                                                     0x80000000
#define HWIO_SDE_RSCC_HW_EVENT_MUXm_SELECT_DRVd_ENABLE_SHFT                                                           0x1f
#define HWIO_SDE_RSCC_HW_EVENT_MUXm_SELECT_DRVd_EVENT_SELECT_BMSK                                                      0x7
#define HWIO_SDE_RSCC_HW_EVENT_MUXm_SELECT_DRVd_EVENT_SELECT_SHFT                                                      0x0

#define HWIO_SDE_RSCC_RSC_ERROR_IRQ_STATUS_DRV0_ADDR                                                            (SDE_RSCC_RSCC_RSC_REG_BASE      + 0x000000d0)
#define HWIO_SDE_RSCC_RSC_ERROR_IRQ_STATUS_DRV0_RMSK                                                                   0x1
#define HWIO_SDE_RSCC_RSC_ERROR_IRQ_STATUS_DRV0_IN          \
        in_dword_masked(HWIO_SDE_RSCC_RSC_ERROR_IRQ_STATUS_DRV0_ADDR, HWIO_SDE_RSCC_RSC_ERROR_IRQ_STATUS_DRV0_RMSK)
#define HWIO_SDE_RSCC_RSC_ERROR_IRQ_STATUS_DRV0_INM(m)      \
        in_dword_masked(HWIO_SDE_RSCC_RSC_ERROR_IRQ_STATUS_DRV0_ADDR, m)
#define HWIO_SDE_RSCC_RSC_ERROR_IRQ_STATUS_DRV0_EPCB_TIMEOUT_IRQ_BMSK                                                  0x1
#define HWIO_SDE_RSCC_RSC_ERROR_IRQ_STATUS_DRV0_EPCB_TIMEOUT_IRQ_SHFT                                                  0x0

#define HWIO_SDE_RSCC_RSC_ERROR_IRQ_CLEAR_DRV0_ADDR                                                             (SDE_RSCC_RSCC_RSC_REG_BASE      + 0x000000d4)
#define HWIO_SDE_RSCC_RSC_ERROR_IRQ_CLEAR_DRV0_RMSK                                                                    0x1
#define HWIO_SDE_RSCC_RSC_ERROR_IRQ_CLEAR_DRV0_OUT(v)      \
        out_dword(HWIO_SDE_RSCC_RSC_ERROR_IRQ_CLEAR_DRV0_ADDR,v)
#define HWIO_SDE_RSCC_RSC_ERROR_IRQ_CLEAR_DRV0_CLR_EPCB_TIMEOUT_IRQ_BMSK                                               0x1
#define HWIO_SDE_RSCC_RSC_ERROR_IRQ_CLEAR_DRV0_CLR_EPCB_TIMEOUT_IRQ_SHFT                                               0x0

#define HWIO_SDE_RSCC_RSC_ERROR_IRQ_ENABLE_DRV0_ADDR                                                            (SDE_RSCC_RSCC_RSC_REG_BASE      + 0x000000d8)
#define HWIO_SDE_RSCC_RSC_ERROR_IRQ_ENABLE_DRV0_RMSK                                                              0x10ffff
#define HWIO_SDE_RSCC_RSC_ERROR_IRQ_ENABLE_DRV0_IN          \
        in_dword_masked(HWIO_SDE_RSCC_RSC_ERROR_IRQ_ENABLE_DRV0_ADDR, HWIO_SDE_RSCC_RSC_ERROR_IRQ_ENABLE_DRV0_RMSK)
#define HWIO_SDE_RSCC_RSC_ERROR_IRQ_ENABLE_DRV0_INM(m)      \
        in_dword_masked(HWIO_SDE_RSCC_RSC_ERROR_IRQ_ENABLE_DRV0_ADDR, m)
#define HWIO_SDE_RSCC_RSC_ERROR_IRQ_ENABLE_DRV0_OUT(v)      \
        out_dword(HWIO_SDE_RSCC_RSC_ERROR_IRQ_ENABLE_DRV0_ADDR,v)
#define HWIO_SDE_RSCC_RSC_ERROR_IRQ_ENABLE_DRV0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_SDE_RSCC_RSC_ERROR_IRQ_ENABLE_DRV0_ADDR,m,v,HWIO_SDE_RSCC_RSC_ERROR_IRQ_ENABLE_DRV0_IN)
#define HWIO_SDE_RSCC_RSC_ERROR_IRQ_ENABLE_DRV0_EPCB_TIMEOUT_IRQ_EN_BMSK                                          0x100000
#define HWIO_SDE_RSCC_RSC_ERROR_IRQ_ENABLE_DRV0_EPCB_TIMEOUT_IRQ_EN_SHFT                                              0x14
#define HWIO_SDE_RSCC_RSC_ERROR_IRQ_ENABLE_DRV0_EPCB_TIMEOUT_THRESHOLD_BMSK                                         0xffff
#define HWIO_SDE_RSCC_RSC_ERROR_IRQ_ENABLE_DRV0_EPCB_TIMEOUT_THRESHOLD_SHFT                                            0x0

#define HWIO_SDE_RSCC_RSC_ERROR_RESP_CTRL_DRV0_ADDR                                                             (SDE_RSCC_RSCC_RSC_REG_BASE      + 0x00000100)
#define HWIO_SDE_RSCC_RSC_ERROR_RESP_CTRL_DRV0_RMSK                                                                    0x1
#define HWIO_SDE_RSCC_RSC_ERROR_RESP_CTRL_DRV0_IN          \
        in_dword_masked(HWIO_SDE_RSCC_RSC_ERROR_RESP_CTRL_DRV0_ADDR, HWIO_SDE_RSCC_RSC_ERROR_RESP_CTRL_DRV0_RMSK)
#define HWIO_SDE_RSCC_RSC_ERROR_RESP_CTRL_DRV0_INM(m)      \
        in_dword_masked(HWIO_SDE_RSCC_RSC_ERROR_RESP_CTRL_DRV0_ADDR, m)
#define HWIO_SDE_RSCC_RSC_ERROR_RESP_CTRL_DRV0_OUT(v)      \
        out_dword(HWIO_SDE_RSCC_RSC_ERROR_RESP_CTRL_DRV0_ADDR,v)
#define HWIO_SDE_RSCC_RSC_ERROR_RESP_CTRL_DRV0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_SDE_RSCC_RSC_ERROR_RESP_CTRL_DRV0_ADDR,m,v,HWIO_SDE_RSCC_RSC_ERROR_RESP_CTRL_DRV0_IN)
#define HWIO_SDE_RSCC_RSC_ERROR_RESP_CTRL_DRV0_ERROR_RESP_FOR_INVALID_ADDR_BMSK                                        0x1
#define HWIO_SDE_RSCC_RSC_ERROR_RESP_CTRL_DRV0_ERROR_RESP_FOR_INVALID_ADDR_SHFT                                        0x0

#define HWIO_SDE_RSCC_RSC_TIMESTAMP_UNIT_OWNER_DRV0_ADDR                                                        (SDE_RSCC_RSCC_RSC_REG_BASE      + 0x00000200)
#define HWIO_SDE_RSCC_RSC_TIMESTAMP_UNIT_OWNER_DRV0_RMSK                                                               0x3
#define HWIO_SDE_RSCC_RSC_TIMESTAMP_UNIT_OWNER_DRV0_IN          \
        in_dword_masked(HWIO_SDE_RSCC_RSC_TIMESTAMP_UNIT_OWNER_DRV0_ADDR, HWIO_SDE_RSCC_RSC_TIMESTAMP_UNIT_OWNER_DRV0_RMSK)
#define HWIO_SDE_RSCC_RSC_TIMESTAMP_UNIT_OWNER_DRV0_INM(m)      \
        in_dword_masked(HWIO_SDE_RSCC_RSC_TIMESTAMP_UNIT_OWNER_DRV0_ADDR, m)
#define HWIO_SDE_RSCC_RSC_TIMESTAMP_UNIT_OWNER_DRV0_OUT(v)      \
        out_dword(HWIO_SDE_RSCC_RSC_TIMESTAMP_UNIT_OWNER_DRV0_ADDR,v)
#define HWIO_SDE_RSCC_RSC_TIMESTAMP_UNIT_OWNER_DRV0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_SDE_RSCC_RSC_TIMESTAMP_UNIT_OWNER_DRV0_ADDR,m,v,HWIO_SDE_RSCC_RSC_TIMESTAMP_UNIT_OWNER_DRV0_IN)
#define HWIO_SDE_RSCC_RSC_TIMESTAMP_UNIT_OWNER_DRV0_OWNER_BMSK                                                         0x3
#define HWIO_SDE_RSCC_RSC_TIMESTAMP_UNIT_OWNER_DRV0_OWNER_SHFT                                                         0x0

#define HWIO_SDE_RSCC_RSC_TIMESTAMP_UNITm_EN_DRVd_ADDR(d,m)                                                     (SDE_RSCC_RSCC_RSC_REG_BASE      + 0x00000204 + 0x20 * (m) + 0x10000 * (d))
#define HWIO_SDE_RSCC_RSC_TIMESTAMP_UNITm_EN_DRVd_RMSK                                                                 0x1
#define HWIO_SDE_RSCC_RSC_TIMESTAMP_UNITm_EN_DRVd_MAXd                                                                   0
#define HWIO_SDE_RSCC_RSC_TIMESTAMP_UNITm_EN_DRVd_MAXm                                                                   5
#define HWIO_SDE_RSCC_RSC_TIMESTAMP_UNITm_EN_DRVd_INI2(d,m)        \
        in_dword_masked(HWIO_SDE_RSCC_RSC_TIMESTAMP_UNITm_EN_DRVd_ADDR(d,m), HWIO_SDE_RSCC_RSC_TIMESTAMP_UNITm_EN_DRVd_RMSK)
#define HWIO_SDE_RSCC_RSC_TIMESTAMP_UNITm_EN_DRVd_INMI2(d,m,mask)    \
        in_dword_masked(HWIO_SDE_RSCC_RSC_TIMESTAMP_UNITm_EN_DRVd_ADDR(d,m), mask)
#define HWIO_SDE_RSCC_RSC_TIMESTAMP_UNITm_EN_DRVd_OUTI2(d,m,val)    \
        out_dword(HWIO_SDE_RSCC_RSC_TIMESTAMP_UNITm_EN_DRVd_ADDR(d,m),val)
#define HWIO_SDE_RSCC_RSC_TIMESTAMP_UNITm_EN_DRVd_OUTMI2(d,m,mask,val) \
        out_dword_masked_ns(HWIO_SDE_RSCC_RSC_TIMESTAMP_UNITm_EN_DRVd_ADDR(d,m),mask,val,HWIO_SDE_RSCC_RSC_TIMESTAMP_UNITm_EN_DRVd_INI2(d,m))
#define HWIO_SDE_RSCC_RSC_TIMESTAMP_UNITm_EN_DRVd_ENABLE_BMSK                                                          0x1
#define HWIO_SDE_RSCC_RSC_TIMESTAMP_UNITm_EN_DRVd_ENABLE_SHFT                                                          0x0

#define HWIO_SDE_RSCC_RSC_TIMESTAMP_UNITm_TIMESTAMP_L_DRVd_ADDR(d,m)                                            (SDE_RSCC_RSCC_RSC_REG_BASE      + 0x00000208 + 0x20 * (m) + 0x10000 * (d))
#define HWIO_SDE_RSCC_RSC_TIMESTAMP_UNITm_TIMESTAMP_L_DRVd_RMSK                                                 0xffffffff
#define HWIO_SDE_RSCC_RSC_TIMESTAMP_UNITm_TIMESTAMP_L_DRVd_MAXd                                                          0
#define HWIO_SDE_RSCC_RSC_TIMESTAMP_UNITm_TIMESTAMP_L_DRVd_MAXm                                                          5
#define HWIO_SDE_RSCC_RSC_TIMESTAMP_UNITm_TIMESTAMP_L_DRVd_INI2(d,m)        \
        in_dword_masked(HWIO_SDE_RSCC_RSC_TIMESTAMP_UNITm_TIMESTAMP_L_DRVd_ADDR(d,m), HWIO_SDE_RSCC_RSC_TIMESTAMP_UNITm_TIMESTAMP_L_DRVd_RMSK)
#define HWIO_SDE_RSCC_RSC_TIMESTAMP_UNITm_TIMESTAMP_L_DRVd_INMI2(d,m,mask)    \
        in_dword_masked(HWIO_SDE_RSCC_RSC_TIMESTAMP_UNITm_TIMESTAMP_L_DRVd_ADDR(d,m), mask)
#define HWIO_SDE_RSCC_RSC_TIMESTAMP_UNITm_TIMESTAMP_L_DRVd_TS_DATA_L_BMSK                                       0xffffffff
#define HWIO_SDE_RSCC_RSC_TIMESTAMP_UNITm_TIMESTAMP_L_DRVd_TS_DATA_L_SHFT                                              0x0

#define HWIO_SDE_RSCC_RSC_TIMESTAMP_UNITm_TIMESTAMP_H_DRVd_ADDR(d,m)                                            (SDE_RSCC_RSCC_RSC_REG_BASE      + 0x0000020c + 0x20 * (m) + 0x10000 * (d))
#define HWIO_SDE_RSCC_RSC_TIMESTAMP_UNITm_TIMESTAMP_H_DRVd_RMSK                                                   0xffffff
#define HWIO_SDE_RSCC_RSC_TIMESTAMP_UNITm_TIMESTAMP_H_DRVd_MAXd                                                          0
#define HWIO_SDE_RSCC_RSC_TIMESTAMP_UNITm_TIMESTAMP_H_DRVd_MAXm                                                          5
#define HWIO_SDE_RSCC_RSC_TIMESTAMP_UNITm_TIMESTAMP_H_DRVd_INI2(d,m)        \
        in_dword_masked(HWIO_SDE_RSCC_RSC_TIMESTAMP_UNITm_TIMESTAMP_H_DRVd_ADDR(d,m), HWIO_SDE_RSCC_RSC_TIMESTAMP_UNITm_TIMESTAMP_H_DRVd_RMSK)
#define HWIO_SDE_RSCC_RSC_TIMESTAMP_UNITm_TIMESTAMP_H_DRVd_INMI2(d,m,mask)    \
        in_dword_masked(HWIO_SDE_RSCC_RSC_TIMESTAMP_UNITm_TIMESTAMP_H_DRVd_ADDR(d,m), mask)
#define HWIO_SDE_RSCC_RSC_TIMESTAMP_UNITm_TIMESTAMP_H_DRVd_TS_DATA_H_BMSK                                         0xffffff
#define HWIO_SDE_RSCC_RSC_TIMESTAMP_UNITm_TIMESTAMP_H_DRVd_TS_DATA_H_SHFT                                              0x0

#define HWIO_SDE_RSCC_RSC_TIMESTAMP_UNITm_OUTPUT_DRVd_ADDR(d,m)                                                 (SDE_RSCC_RSCC_RSC_REG_BASE      + 0x00000210 + 0x20 * (m) + 0x10000 * (d))
#define HWIO_SDE_RSCC_RSC_TIMESTAMP_UNITm_OUTPUT_DRVd_RMSK                                                           0x101
#define HWIO_SDE_RSCC_RSC_TIMESTAMP_UNITm_OUTPUT_DRVd_MAXd                                                               0
#define HWIO_SDE_RSCC_RSC_TIMESTAMP_UNITm_OUTPUT_DRVd_MAXm                                                               5
#define HWIO_SDE_RSCC_RSC_TIMESTAMP_UNITm_OUTPUT_DRVd_INI2(d,m)        \
        in_dword_masked(HWIO_SDE_RSCC_RSC_TIMESTAMP_UNITm_OUTPUT_DRVd_ADDR(d,m), HWIO_SDE_RSCC_RSC_TIMESTAMP_UNITm_OUTPUT_DRVd_RMSK)
#define HWIO_SDE_RSCC_RSC_TIMESTAMP_UNITm_OUTPUT_DRVd_INMI2(d,m,mask)    \
        in_dword_masked(HWIO_SDE_RSCC_RSC_TIMESTAMP_UNITm_OUTPUT_DRVd_ADDR(d,m), mask)
#define HWIO_SDE_RSCC_RSC_TIMESTAMP_UNITm_OUTPUT_DRVd_TS_OVERFLOW_BMSK                                               0x100
#define HWIO_SDE_RSCC_RSC_TIMESTAMP_UNITm_OUTPUT_DRVd_TS_OVERFLOW_SHFT                                                 0x8
#define HWIO_SDE_RSCC_RSC_TIMESTAMP_UNITm_OUTPUT_DRVd_TS_VALID_BMSK                                                    0x1
#define HWIO_SDE_RSCC_RSC_TIMESTAMP_UNITm_OUTPUT_DRVd_TS_VALID_SHFT                                                    0x0

#define HWIO_SDE_RSCC_RSC_TIMERS_CONSIDERED_DRVd_ADDR(d)                                                        (SDE_RSCC_RSCC_RSC_REG_BASE      + 0x00000c00 + 0x10000 * (d))
#define HWIO_SDE_RSCC_RSC_TIMERS_CONSIDERED_DRVd_RMSK                                                           0xffffffff
#define HWIO_SDE_RSCC_RSC_TIMERS_CONSIDERED_DRVd_MAXd                                                                    0
#define HWIO_SDE_RSCC_RSC_TIMERS_CONSIDERED_DRVd_INI(d)        \
        in_dword_masked(HWIO_SDE_RSCC_RSC_TIMERS_CONSIDERED_DRVd_ADDR(d), HWIO_SDE_RSCC_RSC_TIMERS_CONSIDERED_DRVd_RMSK)
#define HWIO_SDE_RSCC_RSC_TIMERS_CONSIDERED_DRVd_INMI(d,mask)    \
        in_dword_masked(HWIO_SDE_RSCC_RSC_TIMERS_CONSIDERED_DRVd_ADDR(d), mask)
#define HWIO_SDE_RSCC_RSC_TIMERS_CONSIDERED_DRVd_OUTI(d,val)    \
        out_dword(HWIO_SDE_RSCC_RSC_TIMERS_CONSIDERED_DRVd_ADDR(d),val)
#define HWIO_SDE_RSCC_RSC_TIMERS_CONSIDERED_DRVd_OUTMI(d,mask,val) \
        out_dword_masked_ns(HWIO_SDE_RSCC_RSC_TIMERS_CONSIDERED_DRVd_ADDR(d),mask,val,HWIO_SDE_RSCC_RSC_TIMERS_CONSIDERED_DRVd_INI(d))
#define HWIO_SDE_RSCC_RSC_TIMERS_CONSIDERED_DRVd_TIMERS_CONSIDERED_ENABLE_MASK_BMSK                             0xffffffff
#define HWIO_SDE_RSCC_RSC_TIMERS_CONSIDERED_DRVd_TIMERS_CONSIDERED_ENABLE_MASK_SHFT                                    0x0

#define HWIO_SDE_RSCC_RSC_SOFT_WAKEUP_TIME_LO_DRVd_ADDR(d)                                                      (SDE_RSCC_RSCC_RSC_REG_BASE      + 0x00000c04 + 0x10000 * (d))
#define HWIO_SDE_RSCC_RSC_SOFT_WAKEUP_TIME_LO_DRVd_RMSK                                                         0xffffffff
#define HWIO_SDE_RSCC_RSC_SOFT_WAKEUP_TIME_LO_DRVd_MAXd                                                                  0
#define HWIO_SDE_RSCC_RSC_SOFT_WAKEUP_TIME_LO_DRVd_INI(d)        \
        in_dword_masked(HWIO_SDE_RSCC_RSC_SOFT_WAKEUP_TIME_LO_DRVd_ADDR(d), HWIO_SDE_RSCC_RSC_SOFT_WAKEUP_TIME_LO_DRVd_RMSK)
#define HWIO_SDE_RSCC_RSC_SOFT_WAKEUP_TIME_LO_DRVd_INMI(d,mask)    \
        in_dword_masked(HWIO_SDE_RSCC_RSC_SOFT_WAKEUP_TIME_LO_DRVd_ADDR(d), mask)
#define HWIO_SDE_RSCC_RSC_SOFT_WAKEUP_TIME_LO_DRVd_OUTI(d,val)    \
        out_dword(HWIO_SDE_RSCC_RSC_SOFT_WAKEUP_TIME_LO_DRVd_ADDR(d),val)
#define HWIO_SDE_RSCC_RSC_SOFT_WAKEUP_TIME_LO_DRVd_OUTMI(d,mask,val) \
        out_dword_masked_ns(HWIO_SDE_RSCC_RSC_SOFT_WAKEUP_TIME_LO_DRVd_ADDR(d),mask,val,HWIO_SDE_RSCC_RSC_SOFT_WAKEUP_TIME_LO_DRVd_INI(d))
#define HWIO_SDE_RSCC_RSC_SOFT_WAKEUP_TIME_LO_DRVd_SOFT_WAKEUP_TIME_LO_BMSK                                     0xffffffff
#define HWIO_SDE_RSCC_RSC_SOFT_WAKEUP_TIME_LO_DRVd_SOFT_WAKEUP_TIME_LO_SHFT                                            0x0

#define HWIO_SDE_RSCC_RSC_SOFT_WAKEUP_TIME_HI_DRVd_ADDR(d)                                                      (SDE_RSCC_RSCC_RSC_REG_BASE      + 0x00000c08 + 0x10000 * (d))
#define HWIO_SDE_RSCC_RSC_SOFT_WAKEUP_TIME_HI_DRVd_RMSK                                                           0xffffff
#define HWIO_SDE_RSCC_RSC_SOFT_WAKEUP_TIME_HI_DRVd_MAXd                                                                  0
#define HWIO_SDE_RSCC_RSC_SOFT_WAKEUP_TIME_HI_DRVd_INI(d)        \
        in_dword_masked(HWIO_SDE_RSCC_RSC_SOFT_WAKEUP_TIME_HI_DRVd_ADDR(d), HWIO_SDE_RSCC_RSC_SOFT_WAKEUP_TIME_HI_DRVd_RMSK)
#define HWIO_SDE_RSCC_RSC_SOFT_WAKEUP_TIME_HI_DRVd_INMI(d,mask)    \
        in_dword_masked(HWIO_SDE_RSCC_RSC_SOFT_WAKEUP_TIME_HI_DRVd_ADDR(d), mask)
#define HWIO_SDE_RSCC_RSC_SOFT_WAKEUP_TIME_HI_DRVd_OUTI(d,val)    \
        out_dword(HWIO_SDE_RSCC_RSC_SOFT_WAKEUP_TIME_HI_DRVd_ADDR(d),val)
#define HWIO_SDE_RSCC_RSC_SOFT_WAKEUP_TIME_HI_DRVd_OUTMI(d,mask,val) \
        out_dword_masked_ns(HWIO_SDE_RSCC_RSC_SOFT_WAKEUP_TIME_HI_DRVd_ADDR(d),mask,val,HWIO_SDE_RSCC_RSC_SOFT_WAKEUP_TIME_HI_DRVd_INI(d))
#define HWIO_SDE_RSCC_RSC_SOFT_WAKEUP_TIME_HI_DRVd_SOFT_WAKEUP_TIME_HI_BMSK                                       0xffffff
#define HWIO_SDE_RSCC_RSC_SOFT_WAKEUP_TIME_HI_DRVd_SOFT_WAKEUP_TIME_HI_SHFT                                            0x0

#define HWIO_SDE_RSCC_RSC_MAX_IDLE_DURATION_DRVd_ADDR(d)                                                        (SDE_RSCC_RSCC_RSC_REG_BASE      + 0x00000c0c + 0x10000 * (d))
#define HWIO_SDE_RSCC_RSC_MAX_IDLE_DURATION_DRVd_RMSK                                                           0xffffffff
#define HWIO_SDE_RSCC_RSC_MAX_IDLE_DURATION_DRVd_MAXd                                                                    0
#define HWIO_SDE_RSCC_RSC_MAX_IDLE_DURATION_DRVd_INI(d)        \
        in_dword_masked(HWIO_SDE_RSCC_RSC_MAX_IDLE_DURATION_DRVd_ADDR(d), HWIO_SDE_RSCC_RSC_MAX_IDLE_DURATION_DRVd_RMSK)
#define HWIO_SDE_RSCC_RSC_MAX_IDLE_DURATION_DRVd_INMI(d,mask)    \
        in_dword_masked(HWIO_SDE_RSCC_RSC_MAX_IDLE_DURATION_DRVd_ADDR(d), mask)
#define HWIO_SDE_RSCC_RSC_MAX_IDLE_DURATION_DRVd_OUTI(d,val)    \
        out_dword(HWIO_SDE_RSCC_RSC_MAX_IDLE_DURATION_DRVd_ADDR(d),val)
#define HWIO_SDE_RSCC_RSC_MAX_IDLE_DURATION_DRVd_OUTMI(d,mask,val) \
        out_dword_masked_ns(HWIO_SDE_RSCC_RSC_MAX_IDLE_DURATION_DRVd_ADDR(d),mask,val,HWIO_SDE_RSCC_RSC_MAX_IDLE_DURATION_DRVd_INI(d))
#define HWIO_SDE_RSCC_RSC_MAX_IDLE_DURATION_DRVd_MAX_IDLE_DURATION_BMSK                                         0xffffffff
#define HWIO_SDE_RSCC_RSC_MAX_IDLE_DURATION_DRVd_MAX_IDLE_DURATION_SHFT                                                0x0

#define HWIO_SDE_RSCC_SOLVER_OVERRIDE_CTRL_DRV0_ADDR                                                            (SDE_RSCC_RSCC_RSC_REG_BASE      + 0x00000c14)
#define HWIO_SDE_RSCC_SOLVER_OVERRIDE_CTRL_DRV0_RMSK                                                               0x10001
#define HWIO_SDE_RSCC_SOLVER_OVERRIDE_CTRL_DRV0_IN          \
        in_dword_masked(HWIO_SDE_RSCC_SOLVER_OVERRIDE_CTRL_DRV0_ADDR, HWIO_SDE_RSCC_SOLVER_OVERRIDE_CTRL_DRV0_RMSK)
#define HWIO_SDE_RSCC_SOLVER_OVERRIDE_CTRL_DRV0_INM(m)      \
        in_dword_masked(HWIO_SDE_RSCC_SOLVER_OVERRIDE_CTRL_DRV0_ADDR, m)
#define HWIO_SDE_RSCC_SOLVER_OVERRIDE_CTRL_DRV0_OUT(v)      \
        out_dword(HWIO_SDE_RSCC_SOLVER_OVERRIDE_CTRL_DRV0_ADDR,v)
#define HWIO_SDE_RSCC_SOLVER_OVERRIDE_CTRL_DRV0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_SDE_RSCC_SOLVER_OVERRIDE_CTRL_DRV0_ADDR,m,v,HWIO_SDE_RSCC_SOLVER_OVERRIDE_CTRL_DRV0_IN)
#define HWIO_SDE_RSCC_SOLVER_OVERRIDE_CTRL_DRV0_ONETIME_BMSK                                                       0x10000
#define HWIO_SDE_RSCC_SOLVER_OVERRIDE_CTRL_DRV0_ONETIME_SHFT                                                          0x10
#define HWIO_SDE_RSCC_SOLVER_OVERRIDE_CTRL_DRV0_CONTINUOUS_BMSK                                                        0x1
#define HWIO_SDE_RSCC_SOLVER_OVERRIDE_CTRL_DRV0_CONTINUOUS_SHFT                                                        0x0

#define HWIO_SDE_RSCC_SOLVER_OVERRIDE_MODE_DRV0_ADDR                                                            (SDE_RSCC_RSCC_RSC_REG_BASE      + 0x00000c18)
#define HWIO_SDE_RSCC_SOLVER_OVERRIDE_MODE_DRV0_RMSK                                                                   0x7
#define HWIO_SDE_RSCC_SOLVER_OVERRIDE_MODE_DRV0_IN          \
        in_dword_masked(HWIO_SDE_RSCC_SOLVER_OVERRIDE_MODE_DRV0_ADDR, HWIO_SDE_RSCC_SOLVER_OVERRIDE_MODE_DRV0_RMSK)
#define HWIO_SDE_RSCC_SOLVER_OVERRIDE_MODE_DRV0_INM(m)      \
        in_dword_masked(HWIO_SDE_RSCC_SOLVER_OVERRIDE_MODE_DRV0_ADDR, m)
#define HWIO_SDE_RSCC_SOLVER_OVERRIDE_MODE_DRV0_OUT(v)      \
        out_dword(HWIO_SDE_RSCC_SOLVER_OVERRIDE_MODE_DRV0_ADDR,v)
#define HWIO_SDE_RSCC_SOLVER_OVERRIDE_MODE_DRV0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_SDE_RSCC_SOLVER_OVERRIDE_MODE_DRV0_ADDR,m,v,HWIO_SDE_RSCC_SOLVER_OVERRIDE_MODE_DRV0_IN)
#define HWIO_SDE_RSCC_SOLVER_OVERRIDE_MODE_DRV0_MODE_BMSK                                                              0x7
#define HWIO_SDE_RSCC_SOLVER_OVERRIDE_MODE_DRV0_MODE_SHFT                                                              0x0

#define HWIO_SDE_RSCC_SOLVER_OVERRIDE_IDLE_TIME_DRV0_ADDR                                                       (SDE_RSCC_RSCC_RSC_REG_BASE      + 0x00000c1c)
#define HWIO_SDE_RSCC_SOLVER_OVERRIDE_IDLE_TIME_DRV0_RMSK                                                       0xffffffff
#define HWIO_SDE_RSCC_SOLVER_OVERRIDE_IDLE_TIME_DRV0_IN          \
        in_dword_masked(HWIO_SDE_RSCC_SOLVER_OVERRIDE_IDLE_TIME_DRV0_ADDR, HWIO_SDE_RSCC_SOLVER_OVERRIDE_IDLE_TIME_DRV0_RMSK)
#define HWIO_SDE_RSCC_SOLVER_OVERRIDE_IDLE_TIME_DRV0_INM(m)      \
        in_dword_masked(HWIO_SDE_RSCC_SOLVER_OVERRIDE_IDLE_TIME_DRV0_ADDR, m)
#define HWIO_SDE_RSCC_SOLVER_OVERRIDE_IDLE_TIME_DRV0_OUT(v)      \
        out_dword(HWIO_SDE_RSCC_SOLVER_OVERRIDE_IDLE_TIME_DRV0_ADDR,v)
#define HWIO_SDE_RSCC_SOLVER_OVERRIDE_IDLE_TIME_DRV0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_SDE_RSCC_SOLVER_OVERRIDE_IDLE_TIME_DRV0_ADDR,m,v,HWIO_SDE_RSCC_SOLVER_OVERRIDE_IDLE_TIME_DRV0_IN)
#define HWIO_SDE_RSCC_SOLVER_OVERRIDE_IDLE_TIME_DRV0_IDLE_TIME_BMSK                                             0xffffffff
#define HWIO_SDE_RSCC_SOLVER_OVERRIDE_IDLE_TIME_DRV0_IDLE_TIME_SHFT                                                    0x0

#define HWIO_SDE_RSCC_SOLVER_SOLVER_MODES_ENABLED_DRVd_ADDR(d)                                                  (SDE_RSCC_RSCC_RSC_REG_BASE      + 0x00000c20 + 0x10000 * (d))
#define HWIO_SDE_RSCC_SOLVER_SOLVER_MODES_ENABLED_DRVd_RMSK                                                           0x1f
#define HWIO_SDE_RSCC_SOLVER_SOLVER_MODES_ENABLED_DRVd_MAXd                                                              0
#define HWIO_SDE_RSCC_SOLVER_SOLVER_MODES_ENABLED_DRVd_INI(d)        \
        in_dword_masked(HWIO_SDE_RSCC_SOLVER_SOLVER_MODES_ENABLED_DRVd_ADDR(d), HWIO_SDE_RSCC_SOLVER_SOLVER_MODES_ENABLED_DRVd_RMSK)
#define HWIO_SDE_RSCC_SOLVER_SOLVER_MODES_ENABLED_DRVd_INMI(d,mask)    \
        in_dword_masked(HWIO_SDE_RSCC_SOLVER_SOLVER_MODES_ENABLED_DRVd_ADDR(d), mask)
#define HWIO_SDE_RSCC_SOLVER_SOLVER_MODES_ENABLED_DRVd_OUTI(d,val)    \
        out_dword(HWIO_SDE_RSCC_SOLVER_SOLVER_MODES_ENABLED_DRVd_ADDR(d),val)
#define HWIO_SDE_RSCC_SOLVER_SOLVER_MODES_ENABLED_DRVd_OUTMI(d,mask,val) \
        out_dword_masked_ns(HWIO_SDE_RSCC_SOLVER_SOLVER_MODES_ENABLED_DRVd_ADDR(d),mask,val,HWIO_SDE_RSCC_SOLVER_SOLVER_MODES_ENABLED_DRVd_INI(d))
#define HWIO_SDE_RSCC_SOLVER_SOLVER_MODES_ENABLED_DRVd_MODE_ENABLE_MASK_BMSK                                          0x1e
#define HWIO_SDE_RSCC_SOLVER_SOLVER_MODES_ENABLED_DRVd_MODE_ENABLE_MASK_SHFT                                           0x1
#define HWIO_SDE_RSCC_SOLVER_SOLVER_MODES_ENABLED_DRVd_MODE_ENABLE_MASK_MODE0_BMSK                                     0x1
#define HWIO_SDE_RSCC_SOLVER_SOLVER_MODES_ENABLED_DRVd_MODE_ENABLE_MASK_MODE0_SHFT                                     0x0

#define HWIO_SDE_RSCC_SOLVER_STATUS0_DRVd_ADDR(d)                                                               (SDE_RSCC_RSCC_RSC_REG_BASE      + 0x00000c24 + 0x10000 * (d))
#define HWIO_SDE_RSCC_SOLVER_STATUS0_DRVd_RMSK                                                                         0x1
#define HWIO_SDE_RSCC_SOLVER_STATUS0_DRVd_MAXd                                                                           0
#define HWIO_SDE_RSCC_SOLVER_STATUS0_DRVd_INI(d)        \
        in_dword_masked(HWIO_SDE_RSCC_SOLVER_STATUS0_DRVd_ADDR(d), HWIO_SDE_RSCC_SOLVER_STATUS0_DRVd_RMSK)
#define HWIO_SDE_RSCC_SOLVER_STATUS0_DRVd_INMI(d,mask)    \
        in_dword_masked(HWIO_SDE_RSCC_SOLVER_STATUS0_DRVd_ADDR(d), mask)
#define HWIO_SDE_RSCC_SOLVER_STATUS0_DRVd_SOLVER_IN_OVERRIDE_BMSK                                                      0x1
#define HWIO_SDE_RSCC_SOLVER_STATUS0_DRVd_SOLVER_IN_OVERRIDE_SHFT                                                      0x0

#define HWIO_SDE_RSCC_SOLVER_STATUS1_DRVd_ADDR(d)                                                               (SDE_RSCC_RSCC_RSC_REG_BASE      + 0x00000c28 + 0x10000 * (d))
#define HWIO_SDE_RSCC_SOLVER_STATUS1_DRVd_RMSK                                                                         0x7
#define HWIO_SDE_RSCC_SOLVER_STATUS1_DRVd_MAXd                                                                           0
#define HWIO_SDE_RSCC_SOLVER_STATUS1_DRVd_INI(d)        \
        in_dword_masked(HWIO_SDE_RSCC_SOLVER_STATUS1_DRVd_ADDR(d), HWIO_SDE_RSCC_SOLVER_STATUS1_DRVd_RMSK)
#define HWIO_SDE_RSCC_SOLVER_STATUS1_DRVd_INMI(d,mask)    \
        in_dword_masked(HWIO_SDE_RSCC_SOLVER_STATUS1_DRVd_ADDR(d), mask)
#define HWIO_SDE_RSCC_SOLVER_STATUS1_DRVd_TIMESLOT_CHOSEN_BMSK                                                         0x7
#define HWIO_SDE_RSCC_SOLVER_STATUS1_DRVd_TIMESLOT_CHOSEN_SHFT                                                         0x0

#define HWIO_SDE_RSCC_SOLVER_STATUS2_DRVd_ADDR(d)                                                               (SDE_RSCC_RSCC_RSC_REG_BASE      + 0x00000c2c + 0x10000 * (d))
#define HWIO_SDE_RSCC_SOLVER_STATUS2_DRVd_RMSK                                                                         0x7
#define HWIO_SDE_RSCC_SOLVER_STATUS2_DRVd_MAXd                                                                           0
#define HWIO_SDE_RSCC_SOLVER_STATUS2_DRVd_INI(d)        \
        in_dword_masked(HWIO_SDE_RSCC_SOLVER_STATUS2_DRVd_ADDR(d), HWIO_SDE_RSCC_SOLVER_STATUS2_DRVd_RMSK)
#define HWIO_SDE_RSCC_SOLVER_STATUS2_DRVd_INMI(d,mask)    \
        in_dword_masked(HWIO_SDE_RSCC_SOLVER_STATUS2_DRVd_ADDR(d), mask)
#define HWIO_SDE_RSCC_SOLVER_STATUS2_DRVd_MODE_CHOSEN_BMSK                                                             0x7
#define HWIO_SDE_RSCC_SOLVER_STATUS2_DRVd_MODE_CHOSEN_SHFT                                                             0x0

#define HWIO_SDE_RSCC_SOLVER_MODE_PARAMETER0_DRV0_MODEm_ADDR(m)                                                 (SDE_RSCC_RSCC_RSC_REG_BASE      + 0x00000c30 + 0x1C * (m))
#define HWIO_SDE_RSCC_SOLVER_MODE_PARAMETER0_DRV0_MODEm_RMSK                                                         0x3ff
#define HWIO_SDE_RSCC_SOLVER_MODE_PARAMETER0_DRV0_MODEm_MAXm                                                             3
#define HWIO_SDE_RSCC_SOLVER_MODE_PARAMETER0_DRV0_MODEm_INI(m)        \
        in_dword_masked(HWIO_SDE_RSCC_SOLVER_MODE_PARAMETER0_DRV0_MODEm_ADDR(m), HWIO_SDE_RSCC_SOLVER_MODE_PARAMETER0_DRV0_MODEm_RMSK)
#define HWIO_SDE_RSCC_SOLVER_MODE_PARAMETER0_DRV0_MODEm_INMI(m,mask)    \
        in_dword_masked(HWIO_SDE_RSCC_SOLVER_MODE_PARAMETER0_DRV0_MODEm_ADDR(m), mask)
#define HWIO_SDE_RSCC_SOLVER_MODE_PARAMETER0_DRV0_MODEm_OUTI(m,val)    \
        out_dword(HWIO_SDE_RSCC_SOLVER_MODE_PARAMETER0_DRV0_MODEm_ADDR(m),val)
#define HWIO_SDE_RSCC_SOLVER_MODE_PARAMETER0_DRV0_MODEm_OUTMI(m,mask,val) \
        out_dword_masked_ns(HWIO_SDE_RSCC_SOLVER_MODE_PARAMETER0_DRV0_MODEm_ADDR(m),mask,val,HWIO_SDE_RSCC_SOLVER_MODE_PARAMETER0_DRV0_MODEm_INI(m))
#define HWIO_SDE_RSCC_SOLVER_MODE_PARAMETER0_DRV0_MODEm_RSC_SEQ_START_ADDR_BMSK                                      0x3ff
#define HWIO_SDE_RSCC_SOLVER_MODE_PARAMETER0_DRV0_MODEm_RSC_SEQ_START_ADDR_SHFT                                        0x0

#define HWIO_SDE_RSCC_SOLVER_MODE_PARAMETER1_DRV0_MODEm_ADDR(m)                                                 (SDE_RSCC_RSCC_RSC_REG_BASE      + 0x00000c34 + 0x1C * (m))
#define HWIO_SDE_RSCC_SOLVER_MODE_PARAMETER1_DRV0_MODEm_RMSK                                                    0xffffffff
#define HWIO_SDE_RSCC_SOLVER_MODE_PARAMETER1_DRV0_MODEm_MAXm                                                             3
#define HWIO_SDE_RSCC_SOLVER_MODE_PARAMETER1_DRV0_MODEm_INI(m)        \
        in_dword_masked(HWIO_SDE_RSCC_SOLVER_MODE_PARAMETER1_DRV0_MODEm_ADDR(m), HWIO_SDE_RSCC_SOLVER_MODE_PARAMETER1_DRV0_MODEm_RMSK)
#define HWIO_SDE_RSCC_SOLVER_MODE_PARAMETER1_DRV0_MODEm_INMI(m,mask)    \
        in_dword_masked(HWIO_SDE_RSCC_SOLVER_MODE_PARAMETER1_DRV0_MODEm_ADDR(m), mask)
#define HWIO_SDE_RSCC_SOLVER_MODE_PARAMETER1_DRV0_MODEm_OUTI(m,val)    \
        out_dword(HWIO_SDE_RSCC_SOLVER_MODE_PARAMETER1_DRV0_MODEm_ADDR(m),val)
#define HWIO_SDE_RSCC_SOLVER_MODE_PARAMETER1_DRV0_MODEm_OUTMI(m,mask,val) \
        out_dword_masked_ns(HWIO_SDE_RSCC_SOLVER_MODE_PARAMETER1_DRV0_MODEm_ADDR(m),mask,val,HWIO_SDE_RSCC_SOLVER_MODE_PARAMETER1_DRV0_MODEm_INI(m))
#define HWIO_SDE_RSCC_SOLVER_MODE_PARAMETER1_DRV0_MODEm_PDC_SEQ_VALID_BMSK                                      0x80000000
#define HWIO_SDE_RSCC_SOLVER_MODE_PARAMETER1_DRV0_MODEm_PDC_SEQ_VALID_SHFT                                            0x1f
#define HWIO_SDE_RSCC_SOLVER_MODE_PARAMETER1_DRV0_MODEm_PDC_TIMER_VALID_BMSK                                    0x40000000
#define HWIO_SDE_RSCC_SOLVER_MODE_PARAMETER1_DRV0_MODEm_PDC_TIMER_VALID_SHFT                                          0x1e
#define HWIO_SDE_RSCC_SOLVER_MODE_PARAMETER1_DRV0_MODEm_PDC_SEQ_START_ADDR_BMSK                                 0x3fffffff
#define HWIO_SDE_RSCC_SOLVER_MODE_PARAMETER1_DRV0_MODEm_PDC_SEQ_START_ADDR_SHFT                                        0x0

#define HWIO_SDE_RSCC_SOLVER_MODE_PARAMETER2_DRV0_MODEm_ADDR(m)                                                 (SDE_RSCC_RSCC_RSC_REG_BASE      + 0x00000c38 + 0x1C * (m))
#define HWIO_SDE_RSCC_SOLVER_MODE_PARAMETER2_DRV0_MODEm_RMSK                                                    0xffffffff
#define HWIO_SDE_RSCC_SOLVER_MODE_PARAMETER2_DRV0_MODEm_MAXm                                                             3
#define HWIO_SDE_RSCC_SOLVER_MODE_PARAMETER2_DRV0_MODEm_INI(m)        \
        in_dword_masked(HWIO_SDE_RSCC_SOLVER_MODE_PARAMETER2_DRV0_MODEm_ADDR(m), HWIO_SDE_RSCC_SOLVER_MODE_PARAMETER2_DRV0_MODEm_RMSK)
#define HWIO_SDE_RSCC_SOLVER_MODE_PARAMETER2_DRV0_MODEm_INMI(m,mask)    \
        in_dword_masked(HWIO_SDE_RSCC_SOLVER_MODE_PARAMETER2_DRV0_MODEm_ADDR(m), mask)
#define HWIO_SDE_RSCC_SOLVER_MODE_PARAMETER2_DRV0_MODEm_OUTI(m,val)    \
        out_dword(HWIO_SDE_RSCC_SOLVER_MODE_PARAMETER2_DRV0_MODEm_ADDR(m),val)
#define HWIO_SDE_RSCC_SOLVER_MODE_PARAMETER2_DRV0_MODEm_OUTMI(m,mask,val) \
        out_dword_masked_ns(HWIO_SDE_RSCC_SOLVER_MODE_PARAMETER2_DRV0_MODEm_ADDR(m),mask,val,HWIO_SDE_RSCC_SOLVER_MODE_PARAMETER2_DRV0_MODEm_INI(m))
#define HWIO_SDE_RSCC_SOLVER_MODE_PARAMETER2_DRV0_MODEm_RSC_BACKOFF_TIME_BMSK                                   0xffffffff
#define HWIO_SDE_RSCC_SOLVER_MODE_PARAMETER2_DRV0_MODEm_RSC_BACKOFF_TIME_SHFT                                          0x0

#define HWIO_SDE_RSCC_SOLVER_MODE_PARAMETER3_DRV0_MODEm_ADDR(m)                                                 (SDE_RSCC_RSCC_RSC_REG_BASE      + 0x00000c40 + 0x1C * (m))
#define HWIO_SDE_RSCC_SOLVER_MODE_PARAMETER3_DRV0_MODEm_RMSK                                                    0xffffffff
#define HWIO_SDE_RSCC_SOLVER_MODE_PARAMETER3_DRV0_MODEm_MAXm                                                             3
#define HWIO_SDE_RSCC_SOLVER_MODE_PARAMETER3_DRV0_MODEm_INI(m)        \
        in_dword_masked(HWIO_SDE_RSCC_SOLVER_MODE_PARAMETER3_DRV0_MODEm_ADDR(m), HWIO_SDE_RSCC_SOLVER_MODE_PARAMETER3_DRV0_MODEm_RMSK)
#define HWIO_SDE_RSCC_SOLVER_MODE_PARAMETER3_DRV0_MODEm_INMI(m,mask)    \
        in_dword_masked(HWIO_SDE_RSCC_SOLVER_MODE_PARAMETER3_DRV0_MODEm_ADDR(m), mask)
#define HWIO_SDE_RSCC_SOLVER_MODE_PARAMETER3_DRV0_MODEm_OUTI(m,val)    \
        out_dword(HWIO_SDE_RSCC_SOLVER_MODE_PARAMETER3_DRV0_MODEm_ADDR(m),val)
#define HWIO_SDE_RSCC_SOLVER_MODE_PARAMETER3_DRV0_MODEm_OUTMI(m,mask,val) \
        out_dword_masked_ns(HWIO_SDE_RSCC_SOLVER_MODE_PARAMETER3_DRV0_MODEm_ADDR(m),mask,val,HWIO_SDE_RSCC_SOLVER_MODE_PARAMETER3_DRV0_MODEm_INI(m))
#define HWIO_SDE_RSCC_SOLVER_MODE_PARAMETER3_DRV0_MODEm_PDC_BACKOFF_TIME_BMSK                                   0xffffffff
#define HWIO_SDE_RSCC_SOLVER_MODE_PARAMETER3_DRV0_MODEm_PDC_BACKOFF_TIME_SHFT                                          0x0

#define HWIO_SDE_RSCC_SOLVER_MODE_PARAMETER4_DRV0_MODEm_ADDR(m)                                                 (SDE_RSCC_RSCC_RSC_REG_BASE      + 0x00000c48 + 0x1C * (m))
#define HWIO_SDE_RSCC_SOLVER_MODE_PARAMETER4_DRV0_MODEm_RMSK                                                    0xffffffff
#define HWIO_SDE_RSCC_SOLVER_MODE_PARAMETER4_DRV0_MODEm_MAXm                                                             3
#define HWIO_SDE_RSCC_SOLVER_MODE_PARAMETER4_DRV0_MODEm_INI(m)        \
        in_dword_masked(HWIO_SDE_RSCC_SOLVER_MODE_PARAMETER4_DRV0_MODEm_ADDR(m), HWIO_SDE_RSCC_SOLVER_MODE_PARAMETER4_DRV0_MODEm_RMSK)
#define HWIO_SDE_RSCC_SOLVER_MODE_PARAMETER4_DRV0_MODEm_INMI(m,mask)    \
        in_dword_masked(HWIO_SDE_RSCC_SOLVER_MODE_PARAMETER4_DRV0_MODEm_ADDR(m), mask)
#define HWIO_SDE_RSCC_SOLVER_MODE_PARAMETER4_DRV0_MODEm_OUTI(m,val)    \
        out_dword(HWIO_SDE_RSCC_SOLVER_MODE_PARAMETER4_DRV0_MODEm_ADDR(m),val)
#define HWIO_SDE_RSCC_SOLVER_MODE_PARAMETER4_DRV0_MODEm_OUTMI(m,mask,val) \
        out_dword_masked_ns(HWIO_SDE_RSCC_SOLVER_MODE_PARAMETER4_DRV0_MODEm_ADDR(m),mask,val,HWIO_SDE_RSCC_SOLVER_MODE_PARAMETER4_DRV0_MODEm_INI(m))
#define HWIO_SDE_RSCC_SOLVER_MODE_PARAMETER4_DRV0_MODEm_MODE_PARAM_GENERAL_PURPOSE_SET0_BMSK                    0xffffffff
#define HWIO_SDE_RSCC_SOLVER_MODE_PARAMETER4_DRV0_MODEm_MODE_PARAM_GENERAL_PURPOSE_SET0_SHFT                           0x0

#define HWIO_SDE_RSCC_SOLVER_TIME_SLOT_TABLE_s_DRV0_ADDR(s)                                                     (SDE_RSCC_RSCC_RSC_REG_BASE      + 0x00001000 + 0x4 * (s))
#define HWIO_SDE_RSCC_SOLVER_TIME_SLOT_TABLE_s_DRV0_RMSK                                                        0xffffffff
#define HWIO_SDE_RSCC_SOLVER_TIME_SLOT_TABLE_s_DRV0_MAXs                                                                 3
#define HWIO_SDE_RSCC_SOLVER_TIME_SLOT_TABLE_s_DRV0_INI(s)        \
        in_dword_masked(HWIO_SDE_RSCC_SOLVER_TIME_SLOT_TABLE_s_DRV0_ADDR(s), HWIO_SDE_RSCC_SOLVER_TIME_SLOT_TABLE_s_DRV0_RMSK)
#define HWIO_SDE_RSCC_SOLVER_TIME_SLOT_TABLE_s_DRV0_INMI(s,mask)    \
        in_dword_masked(HWIO_SDE_RSCC_SOLVER_TIME_SLOT_TABLE_s_DRV0_ADDR(s), mask)
#define HWIO_SDE_RSCC_SOLVER_TIME_SLOT_TABLE_s_DRV0_OUTI(s,val)    \
        out_dword(HWIO_SDE_RSCC_SOLVER_TIME_SLOT_TABLE_s_DRV0_ADDR(s),val)
#define HWIO_SDE_RSCC_SOLVER_TIME_SLOT_TABLE_s_DRV0_OUTMI(s,mask,val) \
        out_dword_masked_ns(HWIO_SDE_RSCC_SOLVER_TIME_SLOT_TABLE_s_DRV0_ADDR(s),mask,val,HWIO_SDE_RSCC_SOLVER_TIME_SLOT_TABLE_s_DRV0_INI(s))
#define HWIO_SDE_RSCC_SOLVER_TIME_SLOT_TABLE_s_DRV0_MIN_TIME_SLOT_BMSK                                          0xffffffff
#define HWIO_SDE_RSCC_SOLVER_TIME_SLOT_TABLE_s_DRV0_MIN_TIME_SLOT_SHFT                                                 0x0

#define HWIO_SDE_RSCC_SOLVER_MODE_PRIORITY_TABLE_SLOTs_PRIORITYp_DRV0_ADDR(s,p)                                 (SDE_RSCC_RSCC_RSC_REG_BASE      + 0x00001080 + 0x80 * (s) + 0x4 * (p))
#define HWIO_SDE_RSCC_SOLVER_MODE_PRIORITY_TABLE_SLOTs_PRIORITYp_DRV0_RMSK                                            0x1f
#define HWIO_SDE_RSCC_SOLVER_MODE_PRIORITY_TABLE_SLOTs_PRIORITYp_DRV0_MAXs                                               3
#define HWIO_SDE_RSCC_SOLVER_MODE_PRIORITY_TABLE_SLOTs_PRIORITYp_DRV0_MAXp                                               3
#define HWIO_SDE_RSCC_SOLVER_MODE_PRIORITY_TABLE_SLOTs_PRIORITYp_DRV0_INI2(s,p)        \
        in_dword_masked(HWIO_SDE_RSCC_SOLVER_MODE_PRIORITY_TABLE_SLOTs_PRIORITYp_DRV0_ADDR(s,p), HWIO_SDE_RSCC_SOLVER_MODE_PRIORITY_TABLE_SLOTs_PRIORITYp_DRV0_RMSK)
#define HWIO_SDE_RSCC_SOLVER_MODE_PRIORITY_TABLE_SLOTs_PRIORITYp_DRV0_INMI2(s,p,mask)    \
        in_dword_masked(HWIO_SDE_RSCC_SOLVER_MODE_PRIORITY_TABLE_SLOTs_PRIORITYp_DRV0_ADDR(s,p), mask)
#define HWIO_SDE_RSCC_SOLVER_MODE_PRIORITY_TABLE_SLOTs_PRIORITYp_DRV0_OUTI2(s,p,val)    \
        out_dword(HWIO_SDE_RSCC_SOLVER_MODE_PRIORITY_TABLE_SLOTs_PRIORITYp_DRV0_ADDR(s,p),val)
#define HWIO_SDE_RSCC_SOLVER_MODE_PRIORITY_TABLE_SLOTs_PRIORITYp_DRV0_OUTMI2(s,p,mask,val) \
        out_dword_masked_ns(HWIO_SDE_RSCC_SOLVER_MODE_PRIORITY_TABLE_SLOTs_PRIORITYp_DRV0_ADDR(s,p),mask,val,HWIO_SDE_RSCC_SOLVER_MODE_PRIORITY_TABLE_SLOTs_PRIORITYp_DRV0_INI2(s,p))
#define HWIO_SDE_RSCC_SOLVER_MODE_PRIORITY_TABLE_SLOTs_PRIORITYp_DRV0_MODE_AT_PRIORITY_BMSK                           0x1f
#define HWIO_SDE_RSCC_SOLVER_MODE_PRIORITY_TABLE_SLOTs_PRIORITYp_DRV0_MODE_AT_PRIORITY_SHFT                            0x0

#define HWIO_SDE_RSCC_RSC_SEQ_OVERRIDE_START_ADDR_DRV0_ADDR                                                     (SDE_RSCC_RSCC_RSC_REG_BASE      + 0x00000400)
#define HWIO_SDE_RSCC_RSC_SEQ_OVERRIDE_START_ADDR_DRV0_RMSK                                                     0x800003ff
#define HWIO_SDE_RSCC_RSC_SEQ_OVERRIDE_START_ADDR_DRV0_IN          \
        in_dword_masked(HWIO_SDE_RSCC_RSC_SEQ_OVERRIDE_START_ADDR_DRV0_ADDR, HWIO_SDE_RSCC_RSC_SEQ_OVERRIDE_START_ADDR_DRV0_RMSK)
#define HWIO_SDE_RSCC_RSC_SEQ_OVERRIDE_START_ADDR_DRV0_INM(m)      \
        in_dword_masked(HWIO_SDE_RSCC_RSC_SEQ_OVERRIDE_START_ADDR_DRV0_ADDR, m)
#define HWIO_SDE_RSCC_RSC_SEQ_OVERRIDE_START_ADDR_DRV0_OUT(v)      \
        out_dword(HWIO_SDE_RSCC_RSC_SEQ_OVERRIDE_START_ADDR_DRV0_ADDR,v)
#define HWIO_SDE_RSCC_RSC_SEQ_OVERRIDE_START_ADDR_DRV0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_SDE_RSCC_RSC_SEQ_OVERRIDE_START_ADDR_DRV0_ADDR,m,v,HWIO_SDE_RSCC_RSC_SEQ_OVERRIDE_START_ADDR_DRV0_IN)
#define HWIO_SDE_RSCC_RSC_SEQ_OVERRIDE_START_ADDR_DRV0_VALID_BMSK                                               0x80000000
#define HWIO_SDE_RSCC_RSC_SEQ_OVERRIDE_START_ADDR_DRV0_VALID_SHFT                                                     0x1f
#define HWIO_SDE_RSCC_RSC_SEQ_OVERRIDE_START_ADDR_DRV0_ADDR_BMSK                                                     0x3ff
#define HWIO_SDE_RSCC_RSC_SEQ_OVERRIDE_START_ADDR_DRV0_ADDR_SHFT                                                       0x0

#define HWIO_SDE_RSCC_RSC_SEQ_BUSY_DRV0_ADDR                                                                    (SDE_RSCC_RSCC_RSC_REG_BASE      + 0x00000404)
#define HWIO_SDE_RSCC_RSC_SEQ_BUSY_DRV0_RMSK                                                                           0x1
#define HWIO_SDE_RSCC_RSC_SEQ_BUSY_DRV0_IN          \
        in_dword_masked(HWIO_SDE_RSCC_RSC_SEQ_BUSY_DRV0_ADDR, HWIO_SDE_RSCC_RSC_SEQ_BUSY_DRV0_RMSK)
#define HWIO_SDE_RSCC_RSC_SEQ_BUSY_DRV0_INM(m)      \
        in_dword_masked(HWIO_SDE_RSCC_RSC_SEQ_BUSY_DRV0_ADDR, m)
#define HWIO_SDE_RSCC_RSC_SEQ_BUSY_DRV0_BUSY_STAT_BMSK                                                                 0x1
#define HWIO_SDE_RSCC_RSC_SEQ_BUSY_DRV0_BUSY_STAT_SHFT                                                                 0x0

#define HWIO_SDE_RSCC_RSC_SEQ_PROGRAM_COUNTER_DRV0_ADDR                                                         (SDE_RSCC_RSCC_RSC_REG_BASE      + 0x00000408)
#define HWIO_SDE_RSCC_RSC_SEQ_PROGRAM_COUNTER_DRV0_RMSK                                                              0x3ff
#define HWIO_SDE_RSCC_RSC_SEQ_PROGRAM_COUNTER_DRV0_IN          \
        in_dword_masked(HWIO_SDE_RSCC_RSC_SEQ_PROGRAM_COUNTER_DRV0_ADDR, HWIO_SDE_RSCC_RSC_SEQ_PROGRAM_COUNTER_DRV0_RMSK)
#define HWIO_SDE_RSCC_RSC_SEQ_PROGRAM_COUNTER_DRV0_INM(m)      \
        in_dword_masked(HWIO_SDE_RSCC_RSC_SEQ_PROGRAM_COUNTER_DRV0_ADDR, m)
#define HWIO_SDE_RSCC_RSC_SEQ_PROGRAM_COUNTER_DRV0_CURRENT_INSTR_ADDR_BMSK                                           0x3ff
#define HWIO_SDE_RSCC_RSC_SEQ_PROGRAM_COUNTER_DRV0_CURRENT_INSTR_ADDR_SHFT                                             0x0

#define HWIO_SDE_RSCC_RSC_SEQ_CFG_BR_ADDR_i_DRV0_ADDR(i)                                                        (SDE_RSCC_RSCC_RSC_REG_BASE      + 0x00000410 + 0x4 * (i))
#define HWIO_SDE_RSCC_RSC_SEQ_CFG_BR_ADDR_i_DRV0_RMSK                                                                0x3ff
#define HWIO_SDE_RSCC_RSC_SEQ_CFG_BR_ADDR_i_DRV0_MAXi                                                                    3
#define HWIO_SDE_RSCC_RSC_SEQ_CFG_BR_ADDR_i_DRV0_INI(i)        \
        in_dword_masked(HWIO_SDE_RSCC_RSC_SEQ_CFG_BR_ADDR_i_DRV0_ADDR(i), HWIO_SDE_RSCC_RSC_SEQ_CFG_BR_ADDR_i_DRV0_RMSK)
#define HWIO_SDE_RSCC_RSC_SEQ_CFG_BR_ADDR_i_DRV0_INMI(i,mask)    \
        in_dword_masked(HWIO_SDE_RSCC_RSC_SEQ_CFG_BR_ADDR_i_DRV0_ADDR(i), mask)
#define HWIO_SDE_RSCC_RSC_SEQ_CFG_BR_ADDR_i_DRV0_OUTI(i,val)    \
        out_dword(HWIO_SDE_RSCC_RSC_SEQ_CFG_BR_ADDR_i_DRV0_ADDR(i),val)
#define HWIO_SDE_RSCC_RSC_SEQ_CFG_BR_ADDR_i_DRV0_OUTMI(i,mask,val) \
        out_dword_masked_ns(HWIO_SDE_RSCC_RSC_SEQ_CFG_BR_ADDR_i_DRV0_ADDR(i),mask,val,HWIO_SDE_RSCC_RSC_SEQ_CFG_BR_ADDR_i_DRV0_INI(i))
#define HWIO_SDE_RSCC_RSC_SEQ_CFG_BR_ADDR_i_DRV0_ADDR_BMSK                                                           0x3ff
#define HWIO_SDE_RSCC_RSC_SEQ_CFG_BR_ADDR_i_DRV0_ADDR_SHFT                                                             0x0

#define HWIO_SDE_RSCC_RSC_SEQ_CFG_DELAY_VAL_i_DRV0_ADDR(i)                                                      (SDE_RSCC_RSCC_RSC_REG_BASE      + 0x00000450 + 0x4 * (i))
#define HWIO_SDE_RSCC_RSC_SEQ_CFG_DELAY_VAL_i_DRV0_RMSK                                                            0xfffff
#define HWIO_SDE_RSCC_RSC_SEQ_CFG_DELAY_VAL_i_DRV0_MAXi                                                                  3
#define HWIO_SDE_RSCC_RSC_SEQ_CFG_DELAY_VAL_i_DRV0_INI(i)        \
        in_dword_masked(HWIO_SDE_RSCC_RSC_SEQ_CFG_DELAY_VAL_i_DRV0_ADDR(i), HWIO_SDE_RSCC_RSC_SEQ_CFG_DELAY_VAL_i_DRV0_RMSK)
#define HWIO_SDE_RSCC_RSC_SEQ_CFG_DELAY_VAL_i_DRV0_INMI(i,mask)    \
        in_dword_masked(HWIO_SDE_RSCC_RSC_SEQ_CFG_DELAY_VAL_i_DRV0_ADDR(i), mask)
#define HWIO_SDE_RSCC_RSC_SEQ_CFG_DELAY_VAL_i_DRV0_OUTI(i,val)    \
        out_dword(HWIO_SDE_RSCC_RSC_SEQ_CFG_DELAY_VAL_i_DRV0_ADDR(i),val)
#define HWIO_SDE_RSCC_RSC_SEQ_CFG_DELAY_VAL_i_DRV0_OUTMI(i,mask,val) \
        out_dword_masked_ns(HWIO_SDE_RSCC_RSC_SEQ_CFG_DELAY_VAL_i_DRV0_ADDR(i),mask,val,HWIO_SDE_RSCC_RSC_SEQ_CFG_DELAY_VAL_i_DRV0_INI(i))
#define HWIO_SDE_RSCC_RSC_SEQ_CFG_DELAY_VAL_i_DRV0_DELAY_VAL_BMSK                                                  0xfffff
#define HWIO_SDE_RSCC_RSC_SEQ_CFG_DELAY_VAL_i_DRV0_DELAY_VAL_SHFT                                                      0x0

#define HWIO_SDE_RSCC_RSC_SEQ_OVERRIDE_TRIGGER_DRV0_ADDR                                                        (SDE_RSCC_RSCC_RSC_REG_BASE      + 0x00000460)
#define HWIO_SDE_RSCC_RSC_SEQ_OVERRIDE_TRIGGER_DRV0_RMSK                                                               0x1
#define HWIO_SDE_RSCC_RSC_SEQ_OVERRIDE_TRIGGER_DRV0_IN          \
        in_dword_masked(HWIO_SDE_RSCC_RSC_SEQ_OVERRIDE_TRIGGER_DRV0_ADDR, HWIO_SDE_RSCC_RSC_SEQ_OVERRIDE_TRIGGER_DRV0_RMSK)
#define HWIO_SDE_RSCC_RSC_SEQ_OVERRIDE_TRIGGER_DRV0_INM(m)      \
        in_dword_masked(HWIO_SDE_RSCC_RSC_SEQ_OVERRIDE_TRIGGER_DRV0_ADDR, m)
#define HWIO_SDE_RSCC_RSC_SEQ_OVERRIDE_TRIGGER_DRV0_OUT(v)      \
        out_dword(HWIO_SDE_RSCC_RSC_SEQ_OVERRIDE_TRIGGER_DRV0_ADDR,v)
#define HWIO_SDE_RSCC_RSC_SEQ_OVERRIDE_TRIGGER_DRV0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_SDE_RSCC_RSC_SEQ_OVERRIDE_TRIGGER_DRV0_ADDR,m,v,HWIO_SDE_RSCC_RSC_SEQ_OVERRIDE_TRIGGER_DRV0_IN)
#define HWIO_SDE_RSCC_RSC_SEQ_OVERRIDE_TRIGGER_DRV0_SEQ_OVERRIDE_TRIGGER_BMSK                                          0x1
#define HWIO_SDE_RSCC_RSC_SEQ_OVERRIDE_TRIGGER_DRV0_SEQ_OVERRIDE_TRIGGER_SHFT                                          0x0

#define HWIO_SDE_RSCC_RSC_SEQ_OVERRIDE_TRIGGER_START_ADDRESS_DRV0_ADDR                                          (SDE_RSCC_RSCC_RSC_REG_BASE      + 0x00000464)
#define HWIO_SDE_RSCC_RSC_SEQ_OVERRIDE_TRIGGER_START_ADDRESS_DRV0_RMSK                                               0x3ff
#define HWIO_SDE_RSCC_RSC_SEQ_OVERRIDE_TRIGGER_START_ADDRESS_DRV0_IN          \
        in_dword_masked(HWIO_SDE_RSCC_RSC_SEQ_OVERRIDE_TRIGGER_START_ADDRESS_DRV0_ADDR, HWIO_SDE_RSCC_RSC_SEQ_OVERRIDE_TRIGGER_START_ADDRESS_DRV0_RMSK)
#define HWIO_SDE_RSCC_RSC_SEQ_OVERRIDE_TRIGGER_START_ADDRESS_DRV0_INM(m)      \
        in_dword_masked(HWIO_SDE_RSCC_RSC_SEQ_OVERRIDE_TRIGGER_START_ADDRESS_DRV0_ADDR, m)
#define HWIO_SDE_RSCC_RSC_SEQ_OVERRIDE_TRIGGER_START_ADDRESS_DRV0_OUT(v)      \
        out_dword(HWIO_SDE_RSCC_RSC_SEQ_OVERRIDE_TRIGGER_START_ADDRESS_DRV0_ADDR,v)
#define HWIO_SDE_RSCC_RSC_SEQ_OVERRIDE_TRIGGER_START_ADDRESS_DRV0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_SDE_RSCC_RSC_SEQ_OVERRIDE_TRIGGER_START_ADDRESS_DRV0_ADDR,m,v,HWIO_SDE_RSCC_RSC_SEQ_OVERRIDE_TRIGGER_START_ADDRESS_DRV0_IN)
#define HWIO_SDE_RSCC_RSC_SEQ_OVERRIDE_TRIGGER_START_ADDRESS_DRV0_SEQ_OVERRIDE_TRIGGER_START_ADDR_BMSK               0x3ff
#define HWIO_SDE_RSCC_RSC_SEQ_OVERRIDE_TRIGGER_START_ADDRESS_DRV0_SEQ_OVERRIDE_TRIGGER_START_ADDR_SHFT                 0x0

#define HWIO_SDE_RSCC_RSC_SEQ_DBG_BREAKPOINT_ADDR_DRV0_ADDR                                                     (SDE_RSCC_RSCC_RSC_REG_BASE      + 0x00000490)
#define HWIO_SDE_RSCC_RSC_SEQ_DBG_BREAKPOINT_ADDR_DRV0_RMSK                                                     0x800003ff
#define HWIO_SDE_RSCC_RSC_SEQ_DBG_BREAKPOINT_ADDR_DRV0_IN          \
        in_dword_masked(HWIO_SDE_RSCC_RSC_SEQ_DBG_BREAKPOINT_ADDR_DRV0_ADDR, HWIO_SDE_RSCC_RSC_SEQ_DBG_BREAKPOINT_ADDR_DRV0_RMSK)
#define HWIO_SDE_RSCC_RSC_SEQ_DBG_BREAKPOINT_ADDR_DRV0_INM(m)      \
        in_dword_masked(HWIO_SDE_RSCC_RSC_SEQ_DBG_BREAKPOINT_ADDR_DRV0_ADDR, m)
#define HWIO_SDE_RSCC_RSC_SEQ_DBG_BREAKPOINT_ADDR_DRV0_OUT(v)      \
        out_dword(HWIO_SDE_RSCC_RSC_SEQ_DBG_BREAKPOINT_ADDR_DRV0_ADDR,v)
#define HWIO_SDE_RSCC_RSC_SEQ_DBG_BREAKPOINT_ADDR_DRV0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_SDE_RSCC_RSC_SEQ_DBG_BREAKPOINT_ADDR_DRV0_ADDR,m,v,HWIO_SDE_RSCC_RSC_SEQ_DBG_BREAKPOINT_ADDR_DRV0_IN)
#define HWIO_SDE_RSCC_RSC_SEQ_DBG_BREAKPOINT_ADDR_DRV0_DBG_BREAKPOINT_VALID_BMSK                                0x80000000
#define HWIO_SDE_RSCC_RSC_SEQ_DBG_BREAKPOINT_ADDR_DRV0_DBG_BREAKPOINT_VALID_SHFT                                      0x1f
#define HWIO_SDE_RSCC_RSC_SEQ_DBG_BREAKPOINT_ADDR_DRV0_DBG_BREAKPOINT_ADDR_BMSK                                      0x3ff
#define HWIO_SDE_RSCC_RSC_SEQ_DBG_BREAKPOINT_ADDR_DRV0_DBG_BREAKPOINT_ADDR_SHFT                                        0x0

#define HWIO_SDE_RSCC_RSC_SEQ_DBG_STEP_DRV0_ADDR                                                                (SDE_RSCC_RSCC_RSC_REG_BASE      + 0x00000494)
#define HWIO_SDE_RSCC_RSC_SEQ_DBG_STEP_DRV0_RMSK                                                                       0x1
#define HWIO_SDE_RSCC_RSC_SEQ_DBG_STEP_DRV0_IN          \
        in_dword_masked(HWIO_SDE_RSCC_RSC_SEQ_DBG_STEP_DRV0_ADDR, HWIO_SDE_RSCC_RSC_SEQ_DBG_STEP_DRV0_RMSK)
#define HWIO_SDE_RSCC_RSC_SEQ_DBG_STEP_DRV0_INM(m)      \
        in_dword_masked(HWIO_SDE_RSCC_RSC_SEQ_DBG_STEP_DRV0_ADDR, m)
#define HWIO_SDE_RSCC_RSC_SEQ_DBG_STEP_DRV0_OUT(v)      \
        out_dword(HWIO_SDE_RSCC_RSC_SEQ_DBG_STEP_DRV0_ADDR,v)
#define HWIO_SDE_RSCC_RSC_SEQ_DBG_STEP_DRV0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_SDE_RSCC_RSC_SEQ_DBG_STEP_DRV0_ADDR,m,v,HWIO_SDE_RSCC_RSC_SEQ_DBG_STEP_DRV0_IN)
#define HWIO_SDE_RSCC_RSC_SEQ_DBG_STEP_DRV0_DBG_STEP_TRIGGER_BMSK                                                      0x1
#define HWIO_SDE_RSCC_RSC_SEQ_DBG_STEP_DRV0_DBG_STEP_TRIGGER_SHFT                                                      0x0

#define HWIO_SDE_RSCC_RSC_SEQ_DBG_CONTINUE_DRV0_ADDR                                                            (SDE_RSCC_RSCC_RSC_REG_BASE      + 0x00000498)
#define HWIO_SDE_RSCC_RSC_SEQ_DBG_CONTINUE_DRV0_RMSK                                                                   0x1
#define HWIO_SDE_RSCC_RSC_SEQ_DBG_CONTINUE_DRV0_IN          \
        in_dword_masked(HWIO_SDE_RSCC_RSC_SEQ_DBG_CONTINUE_DRV0_ADDR, HWIO_SDE_RSCC_RSC_SEQ_DBG_CONTINUE_DRV0_RMSK)
#define HWIO_SDE_RSCC_RSC_SEQ_DBG_CONTINUE_DRV0_INM(m)      \
        in_dword_masked(HWIO_SDE_RSCC_RSC_SEQ_DBG_CONTINUE_DRV0_ADDR, m)
#define HWIO_SDE_RSCC_RSC_SEQ_DBG_CONTINUE_DRV0_OUT(v)      \
        out_dword(HWIO_SDE_RSCC_RSC_SEQ_DBG_CONTINUE_DRV0_ADDR,v)
#define HWIO_SDE_RSCC_RSC_SEQ_DBG_CONTINUE_DRV0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_SDE_RSCC_RSC_SEQ_DBG_CONTINUE_DRV0_ADDR,m,v,HWIO_SDE_RSCC_RSC_SEQ_DBG_CONTINUE_DRV0_IN)
#define HWIO_SDE_RSCC_RSC_SEQ_DBG_CONTINUE_DRV0_DBG_STEP_CONTINUE_BMSK                                                 0x1
#define HWIO_SDE_RSCC_RSC_SEQ_DBG_CONTINUE_DRV0_DBG_STEP_CONTINUE_SHFT                                                 0x0

#define HWIO_SDE_RSCC_RSC_SEQ_DBG_STAT_DRV0_ADDR                                                                (SDE_RSCC_RSCC_RSC_REG_BASE      + 0x0000049c)
#define HWIO_SDE_RSCC_RSC_SEQ_DBG_STAT_DRV0_RMSK                                                                       0x1
#define HWIO_SDE_RSCC_RSC_SEQ_DBG_STAT_DRV0_IN          \
        in_dword_masked(HWIO_SDE_RSCC_RSC_SEQ_DBG_STAT_DRV0_ADDR, HWIO_SDE_RSCC_RSC_SEQ_DBG_STAT_DRV0_RMSK)
#define HWIO_SDE_RSCC_RSC_SEQ_DBG_STAT_DRV0_INM(m)      \
        in_dword_masked(HWIO_SDE_RSCC_RSC_SEQ_DBG_STAT_DRV0_ADDR, m)
#define HWIO_SDE_RSCC_RSC_SEQ_DBG_STAT_DRV0_DBG_STEPPING_STATUS_BMSK                                                   0x1
#define HWIO_SDE_RSCC_RSC_SEQ_DBG_STAT_DRV0_DBG_STEPPING_STATUS_SHFT                                                   0x0

#define HWIO_SDE_RSCC_RSC_SEQ_OVERRIDE_PWR_CNTRL_MASK_DRV0_ADDR                                                 (SDE_RSCC_RSCC_RSC_REG_BASE      + 0x000004a0)
#define HWIO_SDE_RSCC_RSC_SEQ_OVERRIDE_PWR_CNTRL_MASK_DRV0_RMSK                                                       0x3f
#define HWIO_SDE_RSCC_RSC_SEQ_OVERRIDE_PWR_CNTRL_MASK_DRV0_IN          \
        in_dword_masked(HWIO_SDE_RSCC_RSC_SEQ_OVERRIDE_PWR_CNTRL_MASK_DRV0_ADDR, HWIO_SDE_RSCC_RSC_SEQ_OVERRIDE_PWR_CNTRL_MASK_DRV0_RMSK)
#define HWIO_SDE_RSCC_RSC_SEQ_OVERRIDE_PWR_CNTRL_MASK_DRV0_INM(m)      \
        in_dword_masked(HWIO_SDE_RSCC_RSC_SEQ_OVERRIDE_PWR_CNTRL_MASK_DRV0_ADDR, m)
#define HWIO_SDE_RSCC_RSC_SEQ_OVERRIDE_PWR_CNTRL_MASK_DRV0_OUT(v)      \
        out_dword(HWIO_SDE_RSCC_RSC_SEQ_OVERRIDE_PWR_CNTRL_MASK_DRV0_ADDR,v)
#define HWIO_SDE_RSCC_RSC_SEQ_OVERRIDE_PWR_CNTRL_MASK_DRV0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_SDE_RSCC_RSC_SEQ_OVERRIDE_PWR_CNTRL_MASK_DRV0_ADDR,m,v,HWIO_SDE_RSCC_RSC_SEQ_OVERRIDE_PWR_CNTRL_MASK_DRV0_IN)
#define HWIO_SDE_RSCC_RSC_SEQ_OVERRIDE_PWR_CNTRL_MASK_DRV0_SEQ_OVERRIDE_PWR_CNTRL_MASK_BMSK                           0x3f
#define HWIO_SDE_RSCC_RSC_SEQ_OVERRIDE_PWR_CNTRL_MASK_DRV0_SEQ_OVERRIDE_PWR_CNTRL_MASK_SHFT                            0x0

#define HWIO_SDE_RSCC_RSC_SEQ_OVERRIDE_PWR_CNTRL_VAL_DRV0_ADDR                                                  (SDE_RSCC_RSCC_RSC_REG_BASE      + 0x000004a4)
#define HWIO_SDE_RSCC_RSC_SEQ_OVERRIDE_PWR_CNTRL_VAL_DRV0_RMSK                                                        0x3f
#define HWIO_SDE_RSCC_RSC_SEQ_OVERRIDE_PWR_CNTRL_VAL_DRV0_IN          \
        in_dword_masked(HWIO_SDE_RSCC_RSC_SEQ_OVERRIDE_PWR_CNTRL_VAL_DRV0_ADDR, HWIO_SDE_RSCC_RSC_SEQ_OVERRIDE_PWR_CNTRL_VAL_DRV0_RMSK)
#define HWIO_SDE_RSCC_RSC_SEQ_OVERRIDE_PWR_CNTRL_VAL_DRV0_INM(m)      \
        in_dword_masked(HWIO_SDE_RSCC_RSC_SEQ_OVERRIDE_PWR_CNTRL_VAL_DRV0_ADDR, m)
#define HWIO_SDE_RSCC_RSC_SEQ_OVERRIDE_PWR_CNTRL_VAL_DRV0_OUT(v)      \
        out_dword(HWIO_SDE_RSCC_RSC_SEQ_OVERRIDE_PWR_CNTRL_VAL_DRV0_ADDR,v)
#define HWIO_SDE_RSCC_RSC_SEQ_OVERRIDE_PWR_CNTRL_VAL_DRV0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_SDE_RSCC_RSC_SEQ_OVERRIDE_PWR_CNTRL_VAL_DRV0_ADDR,m,v,HWIO_SDE_RSCC_RSC_SEQ_OVERRIDE_PWR_CNTRL_VAL_DRV0_IN)
#define HWIO_SDE_RSCC_RSC_SEQ_OVERRIDE_PWR_CNTRL_VAL_DRV0_SEQ_OVERRIDE_PWR_CNTRL_VAL_BMSK                             0x3f
#define HWIO_SDE_RSCC_RSC_SEQ_OVERRIDE_PWR_CNTRL_VAL_DRV0_SEQ_OVERRIDE_PWR_CNTRL_VAL_SHFT                              0x0

#define HWIO_SDE_RSCC_RSC_SEQ_OVERRIDE_WAIT_EVENT_MASK_DRV0_ADDR                                                (SDE_RSCC_RSCC_RSC_REG_BASE      + 0x000004a8)
#define HWIO_SDE_RSCC_RSC_SEQ_OVERRIDE_WAIT_EVENT_MASK_DRV0_RMSK                                                      0x3f
#define HWIO_SDE_RSCC_RSC_SEQ_OVERRIDE_WAIT_EVENT_MASK_DRV0_IN          \
        in_dword_masked(HWIO_SDE_RSCC_RSC_SEQ_OVERRIDE_WAIT_EVENT_MASK_DRV0_ADDR, HWIO_SDE_RSCC_RSC_SEQ_OVERRIDE_WAIT_EVENT_MASK_DRV0_RMSK)
#define HWIO_SDE_RSCC_RSC_SEQ_OVERRIDE_WAIT_EVENT_MASK_DRV0_INM(m)      \
        in_dword_masked(HWIO_SDE_RSCC_RSC_SEQ_OVERRIDE_WAIT_EVENT_MASK_DRV0_ADDR, m)
#define HWIO_SDE_RSCC_RSC_SEQ_OVERRIDE_WAIT_EVENT_MASK_DRV0_OUT(v)      \
        out_dword(HWIO_SDE_RSCC_RSC_SEQ_OVERRIDE_WAIT_EVENT_MASK_DRV0_ADDR,v)
#define HWIO_SDE_RSCC_RSC_SEQ_OVERRIDE_WAIT_EVENT_MASK_DRV0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_SDE_RSCC_RSC_SEQ_OVERRIDE_WAIT_EVENT_MASK_DRV0_ADDR,m,v,HWIO_SDE_RSCC_RSC_SEQ_OVERRIDE_WAIT_EVENT_MASK_DRV0_IN)
#define HWIO_SDE_RSCC_RSC_SEQ_OVERRIDE_WAIT_EVENT_MASK_DRV0_SEQ_OVERRIDE_WAIT_EVENT_MASK_BMSK                         0x3f
#define HWIO_SDE_RSCC_RSC_SEQ_OVERRIDE_WAIT_EVENT_MASK_DRV0_SEQ_OVERRIDE_WAIT_EVENT_MASK_SHFT                          0x0

#define HWIO_SDE_RSCC_RSC_SEQ_OVERRIDE_WAIT_EVENT_VAL_DRV0_ADDR                                                 (SDE_RSCC_RSCC_RSC_REG_BASE      + 0x000004ac)
#define HWIO_SDE_RSCC_RSC_SEQ_OVERRIDE_WAIT_EVENT_VAL_DRV0_RMSK                                                       0x3f
#define HWIO_SDE_RSCC_RSC_SEQ_OVERRIDE_WAIT_EVENT_VAL_DRV0_IN          \
        in_dword_masked(HWIO_SDE_RSCC_RSC_SEQ_OVERRIDE_WAIT_EVENT_VAL_DRV0_ADDR, HWIO_SDE_RSCC_RSC_SEQ_OVERRIDE_WAIT_EVENT_VAL_DRV0_RMSK)
#define HWIO_SDE_RSCC_RSC_SEQ_OVERRIDE_WAIT_EVENT_VAL_DRV0_INM(m)      \
        in_dword_masked(HWIO_SDE_RSCC_RSC_SEQ_OVERRIDE_WAIT_EVENT_VAL_DRV0_ADDR, m)
#define HWIO_SDE_RSCC_RSC_SEQ_OVERRIDE_WAIT_EVENT_VAL_DRV0_OUT(v)      \
        out_dword(HWIO_SDE_RSCC_RSC_SEQ_OVERRIDE_WAIT_EVENT_VAL_DRV0_ADDR,v)
#define HWIO_SDE_RSCC_RSC_SEQ_OVERRIDE_WAIT_EVENT_VAL_DRV0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_SDE_RSCC_RSC_SEQ_OVERRIDE_WAIT_EVENT_VAL_DRV0_ADDR,m,v,HWIO_SDE_RSCC_RSC_SEQ_OVERRIDE_WAIT_EVENT_VAL_DRV0_IN)
#define HWIO_SDE_RSCC_RSC_SEQ_OVERRIDE_WAIT_EVENT_VAL_DRV0_SEQ_OVERRIDE_WAIT_EVENT_VAL_BMSK                           0x3f
#define HWIO_SDE_RSCC_RSC_SEQ_OVERRIDE_WAIT_EVENT_VAL_DRV0_SEQ_OVERRIDE_WAIT_EVENT_VAL_SHFT                            0x0

#define HWIO_SDE_RSCC_SEQ_MEM_m_DRV0_ADDR(m)                                                                    (SDE_RSCC_RSCC_RSC_REG_BASE      + 0x00000600 + 0x4 * (m))
#define HWIO_SDE_RSCC_SEQ_MEM_m_DRV0_RMSK                                                                       0xffffffff
#define HWIO_SDE_RSCC_SEQ_MEM_m_DRV0_MAXm                                                                               31
#define HWIO_SDE_RSCC_SEQ_MEM_m_DRV0_INI(m)        \
        in_dword_masked(HWIO_SDE_RSCC_SEQ_MEM_m_DRV0_ADDR(m), HWIO_SDE_RSCC_SEQ_MEM_m_DRV0_RMSK)
#define HWIO_SDE_RSCC_SEQ_MEM_m_DRV0_INMI(m,mask)    \
        in_dword_masked(HWIO_SDE_RSCC_SEQ_MEM_m_DRV0_ADDR(m), mask)
#define HWIO_SDE_RSCC_SEQ_MEM_m_DRV0_OUTI(m,val)    \
        out_dword(HWIO_SDE_RSCC_SEQ_MEM_m_DRV0_ADDR(m),val)
#define HWIO_SDE_RSCC_SEQ_MEM_m_DRV0_OUTMI(m,mask,val) \
        out_dword_masked_ns(HWIO_SDE_RSCC_SEQ_MEM_m_DRV0_ADDR(m),mask,val,HWIO_SDE_RSCC_SEQ_MEM_m_DRV0_INI(m))
#define HWIO_SDE_RSCC_SEQ_MEM_m_DRV0_CMD_WORD_BMSK                                                              0xffffffff
#define HWIO_SDE_RSCC_SEQ_MEM_m_DRV0_CMD_WORD_SHFT                                                                     0x0

#define HWIO_SDE_RSCC_TCS_AMC_MODE_IRQ_ENABLE_DRV0_ADDR                                                         (SDE_RSCC_RSCC_RSC_REG_BASE      + 0x00001c00)
#define HWIO_SDE_RSCC_TCS_AMC_MODE_IRQ_ENABLE_DRV0_RMSK                                                                0xf
#define HWIO_SDE_RSCC_TCS_AMC_MODE_IRQ_ENABLE_DRV0_IN          \
        in_dword_masked(HWIO_SDE_RSCC_TCS_AMC_MODE_IRQ_ENABLE_DRV0_ADDR, HWIO_SDE_RSCC_TCS_AMC_MODE_IRQ_ENABLE_DRV0_RMSK)
#define HWIO_SDE_RSCC_TCS_AMC_MODE_IRQ_ENABLE_DRV0_INM(m)      \
        in_dword_masked(HWIO_SDE_RSCC_TCS_AMC_MODE_IRQ_ENABLE_DRV0_ADDR, m)
#define HWIO_SDE_RSCC_TCS_AMC_MODE_IRQ_ENABLE_DRV0_OUT(v)      \
        out_dword(HWIO_SDE_RSCC_TCS_AMC_MODE_IRQ_ENABLE_DRV0_ADDR,v)
#define HWIO_SDE_RSCC_TCS_AMC_MODE_IRQ_ENABLE_DRV0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_SDE_RSCC_TCS_AMC_MODE_IRQ_ENABLE_DRV0_ADDR,m,v,HWIO_SDE_RSCC_TCS_AMC_MODE_IRQ_ENABLE_DRV0_IN)
#define HWIO_SDE_RSCC_TCS_AMC_MODE_IRQ_ENABLE_DRV0_AMC_COMPLETION_IRQ_ENABLE_BMSK                                      0xf
#define HWIO_SDE_RSCC_TCS_AMC_MODE_IRQ_ENABLE_DRV0_AMC_COMPLETION_IRQ_ENABLE_SHFT                                      0x0

#define HWIO_SDE_RSCC_TCS_AMC_MODE_IRQ_STATUS_DRV0_ADDR                                                         (SDE_RSCC_RSCC_RSC_REG_BASE      + 0x00001c04)
#define HWIO_SDE_RSCC_TCS_AMC_MODE_IRQ_STATUS_DRV0_RMSK                                                                0xf
#define HWIO_SDE_RSCC_TCS_AMC_MODE_IRQ_STATUS_DRV0_IN          \
        in_dword_masked(HWIO_SDE_RSCC_TCS_AMC_MODE_IRQ_STATUS_DRV0_ADDR, HWIO_SDE_RSCC_TCS_AMC_MODE_IRQ_STATUS_DRV0_RMSK)
#define HWIO_SDE_RSCC_TCS_AMC_MODE_IRQ_STATUS_DRV0_INM(m)      \
        in_dword_masked(HWIO_SDE_RSCC_TCS_AMC_MODE_IRQ_STATUS_DRV0_ADDR, m)
#define HWIO_SDE_RSCC_TCS_AMC_MODE_IRQ_STATUS_DRV0_AMC_COMPLETION_IRQ_STATUS_BMSK                                      0xf
#define HWIO_SDE_RSCC_TCS_AMC_MODE_IRQ_STATUS_DRV0_AMC_COMPLETION_IRQ_STATUS_SHFT                                      0x0

#define HWIO_SDE_RSCC_TCS_AMC_MODE_IRQ_CLEAR_DRV0_ADDR                                                          (SDE_RSCC_RSCC_RSC_REG_BASE      + 0x00001c08)
#define HWIO_SDE_RSCC_TCS_AMC_MODE_IRQ_CLEAR_DRV0_RMSK                                                                 0xf
#define HWIO_SDE_RSCC_TCS_AMC_MODE_IRQ_CLEAR_DRV0_OUT(v)      \
        out_dword(HWIO_SDE_RSCC_TCS_AMC_MODE_IRQ_CLEAR_DRV0_ADDR,v)
#define HWIO_SDE_RSCC_TCS_AMC_MODE_IRQ_CLEAR_DRV0_AMC_COMPLETION_IRQ_CLEAR_BMSK                                        0xf
#define HWIO_SDE_RSCC_TCS_AMC_MODE_IRQ_CLEAR_DRV0_AMC_COMPLETION_IRQ_CLEAR_SHFT                                        0x0

#define HWIO_SDE_RSCC_TCSm_DRV0_CMD_WAIT_FOR_CMPL_ADDR(m)                                                       (SDE_RSCC_RSCC_RSC_REG_BASE      + 0x00001c10 + 0x2A0 * (m))
#define HWIO_SDE_RSCC_TCSm_DRV0_CMD_WAIT_FOR_CMPL_RMSK                                                              0xffff
#define HWIO_SDE_RSCC_TCSm_DRV0_CMD_WAIT_FOR_CMPL_MAXm                                                                   3
#define HWIO_SDE_RSCC_TCSm_DRV0_CMD_WAIT_FOR_CMPL_INI(m)        \
        in_dword_masked(HWIO_SDE_RSCC_TCSm_DRV0_CMD_WAIT_FOR_CMPL_ADDR(m), HWIO_SDE_RSCC_TCSm_DRV0_CMD_WAIT_FOR_CMPL_RMSK)
#define HWIO_SDE_RSCC_TCSm_DRV0_CMD_WAIT_FOR_CMPL_INMI(m,mask)    \
        in_dword_masked(HWIO_SDE_RSCC_TCSm_DRV0_CMD_WAIT_FOR_CMPL_ADDR(m), mask)
#define HWIO_SDE_RSCC_TCSm_DRV0_CMD_WAIT_FOR_CMPL_OUTI(m,val)    \
        out_dword(HWIO_SDE_RSCC_TCSm_DRV0_CMD_WAIT_FOR_CMPL_ADDR(m),val)
#define HWIO_SDE_RSCC_TCSm_DRV0_CMD_WAIT_FOR_CMPL_OUTMI(m,mask,val) \
        out_dword_masked_ns(HWIO_SDE_RSCC_TCSm_DRV0_CMD_WAIT_FOR_CMPL_ADDR(m),mask,val,HWIO_SDE_RSCC_TCSm_DRV0_CMD_WAIT_FOR_CMPL_INI(m))
#define HWIO_SDE_RSCC_TCSm_DRV0_CMD_WAIT_FOR_CMPL_CMD_WAIT_FOR_CMPL_BMSK                                            0xffff
#define HWIO_SDE_RSCC_TCSm_DRV0_CMD_WAIT_FOR_CMPL_CMD_WAIT_FOR_CMPL_SHFT                                               0x0

#define HWIO_SDE_RSCC_TCSm_DRV0_CONTROL_ADDR(m)                                                                 (SDE_RSCC_RSCC_RSC_REG_BASE      + 0x00001c14 + 0x2A0 * (m))
#define HWIO_SDE_RSCC_TCSm_DRV0_CONTROL_RMSK                                                                     0x1010000
#define HWIO_SDE_RSCC_TCSm_DRV0_CONTROL_MAXm                                                                             3
#define HWIO_SDE_RSCC_TCSm_DRV0_CONTROL_INI(m)        \
        in_dword_masked(HWIO_SDE_RSCC_TCSm_DRV0_CONTROL_ADDR(m), HWIO_SDE_RSCC_TCSm_DRV0_CONTROL_RMSK)
#define HWIO_SDE_RSCC_TCSm_DRV0_CONTROL_INMI(m,mask)    \
        in_dword_masked(HWIO_SDE_RSCC_TCSm_DRV0_CONTROL_ADDR(m), mask)
#define HWIO_SDE_RSCC_TCSm_DRV0_CONTROL_OUTI(m,val)    \
        out_dword(HWIO_SDE_RSCC_TCSm_DRV0_CONTROL_ADDR(m),val)
#define HWIO_SDE_RSCC_TCSm_DRV0_CONTROL_OUTMI(m,mask,val) \
        out_dword_masked_ns(HWIO_SDE_RSCC_TCSm_DRV0_CONTROL_ADDR(m),mask,val,HWIO_SDE_RSCC_TCSm_DRV0_CONTROL_INI(m))
#define HWIO_SDE_RSCC_TCSm_DRV0_CONTROL_AMC_MODE_TRIGGER_BMSK                                                    0x1000000
#define HWIO_SDE_RSCC_TCSm_DRV0_CONTROL_AMC_MODE_TRIGGER_SHFT                                                         0x18
#define HWIO_SDE_RSCC_TCSm_DRV0_CONTROL_AMC_MODE_EN_BMSK                                                           0x10000
#define HWIO_SDE_RSCC_TCSm_DRV0_CONTROL_AMC_MODE_EN_SHFT                                                              0x10

#define HWIO_SDE_RSCC_TCSm_DRV0_STATUS_ADDR(m)                                                                  (SDE_RSCC_RSCC_RSC_REG_BASE      + 0x00001c18 + 0x2A0 * (m))
#define HWIO_SDE_RSCC_TCSm_DRV0_STATUS_RMSK                                                                            0x1
#define HWIO_SDE_RSCC_TCSm_DRV0_STATUS_MAXm                                                                              3
#define HWIO_SDE_RSCC_TCSm_DRV0_STATUS_INI(m)        \
        in_dword_masked(HWIO_SDE_RSCC_TCSm_DRV0_STATUS_ADDR(m), HWIO_SDE_RSCC_TCSm_DRV0_STATUS_RMSK)
#define HWIO_SDE_RSCC_TCSm_DRV0_STATUS_INMI(m,mask)    \
        in_dword_masked(HWIO_SDE_RSCC_TCSm_DRV0_STATUS_ADDR(m), mask)
#define HWIO_SDE_RSCC_TCSm_DRV0_STATUS_CONTROLLER_IDLE_BMSK                                                            0x1
#define HWIO_SDE_RSCC_TCSm_DRV0_STATUS_CONTROLLER_IDLE_SHFT                                                            0x0

#define HWIO_SDE_RSCC_TCSm_DRV0_CMD_ENABLE_ADDR(m)                                                              (SDE_RSCC_RSCC_RSC_REG_BASE      + 0x00001c1c + 0x2A0 * (m))
#define HWIO_SDE_RSCC_TCSm_DRV0_CMD_ENABLE_RMSK                                                                     0xffff
#define HWIO_SDE_RSCC_TCSm_DRV0_CMD_ENABLE_MAXm                                                                          3
#define HWIO_SDE_RSCC_TCSm_DRV0_CMD_ENABLE_INI(m)        \
        in_dword_masked(HWIO_SDE_RSCC_TCSm_DRV0_CMD_ENABLE_ADDR(m), HWIO_SDE_RSCC_TCSm_DRV0_CMD_ENABLE_RMSK)
#define HWIO_SDE_RSCC_TCSm_DRV0_CMD_ENABLE_INMI(m,mask)    \
        in_dword_masked(HWIO_SDE_RSCC_TCSm_DRV0_CMD_ENABLE_ADDR(m), mask)
#define HWIO_SDE_RSCC_TCSm_DRV0_CMD_ENABLE_OUTI(m,val)    \
        out_dword(HWIO_SDE_RSCC_TCSm_DRV0_CMD_ENABLE_ADDR(m),val)
#define HWIO_SDE_RSCC_TCSm_DRV0_CMD_ENABLE_OUTMI(m,mask,val) \
        out_dword_masked_ns(HWIO_SDE_RSCC_TCSm_DRV0_CMD_ENABLE_ADDR(m),mask,val,HWIO_SDE_RSCC_TCSm_DRV0_CMD_ENABLE_INI(m))
#define HWIO_SDE_RSCC_TCSm_DRV0_CMD_ENABLE_INDIVIDUAL_CMD_ENABLE_MASK_BMSK                                          0xffff
#define HWIO_SDE_RSCC_TCSm_DRV0_CMD_ENABLE_INDIVIDUAL_CMD_ENABLE_MASK_SHFT                                             0x0

#define HWIO_SDE_RSCC_TCSm_CMDn_DRV0_MSGID_ADDR(m,n)                                                            (SDE_RSCC_RSCC_RSC_REG_BASE      + 0x00001c30 + 0x2A0 * (m) + 0x14 * (n))
#define HWIO_SDE_RSCC_TCSm_CMDn_DRV0_MSGID_RMSK                                                                    0x1010f
#define HWIO_SDE_RSCC_TCSm_CMDn_DRV0_MSGID_MAXm                                                                          3
#define HWIO_SDE_RSCC_TCSm_CMDn_DRV0_MSGID_MAXn                                                                         15
#define HWIO_SDE_RSCC_TCSm_CMDn_DRV0_MSGID_INI2(m,n)        \
        in_dword_masked(HWIO_SDE_RSCC_TCSm_CMDn_DRV0_MSGID_ADDR(m,n), HWIO_SDE_RSCC_TCSm_CMDn_DRV0_MSGID_RMSK)
#define HWIO_SDE_RSCC_TCSm_CMDn_DRV0_MSGID_INMI2(m,n,mask)    \
        in_dword_masked(HWIO_SDE_RSCC_TCSm_CMDn_DRV0_MSGID_ADDR(m,n), mask)
#define HWIO_SDE_RSCC_TCSm_CMDn_DRV0_MSGID_OUTI2(m,n,val)    \
        out_dword(HWIO_SDE_RSCC_TCSm_CMDn_DRV0_MSGID_ADDR(m,n),val)
#define HWIO_SDE_RSCC_TCSm_CMDn_DRV0_MSGID_OUTMI2(m,n,mask,val) \
        out_dword_masked_ns(HWIO_SDE_RSCC_TCSm_CMDn_DRV0_MSGID_ADDR(m,n),mask,val,HWIO_SDE_RSCC_TCSm_CMDn_DRV0_MSGID_INI2(m,n))
#define HWIO_SDE_RSCC_TCSm_CMDn_DRV0_MSGID_READ_OR_WRITE_BMSK                                                      0x10000
#define HWIO_SDE_RSCC_TCSm_CMDn_DRV0_MSGID_READ_OR_WRITE_SHFT                                                         0x10
#define HWIO_SDE_RSCC_TCSm_CMDn_DRV0_MSGID_RES_REQ_BMSK                                                              0x100
#define HWIO_SDE_RSCC_TCSm_CMDn_DRV0_MSGID_RES_REQ_SHFT                                                                0x8
#define HWIO_SDE_RSCC_TCSm_CMDn_DRV0_MSGID_MSG_LENGTH_BMSK                                                             0xf
#define HWIO_SDE_RSCC_TCSm_CMDn_DRV0_MSGID_MSG_LENGTH_SHFT                                                             0x0

#define HWIO_SDE_RSCC_TCSm_CMDn_DRV0_ADDR_ADDR(m,n)                                                             (SDE_RSCC_RSCC_RSC_REG_BASE      + 0x00001c34 + 0x2A0 * (m) + 0x14 * (n))
#define HWIO_SDE_RSCC_TCSm_CMDn_DRV0_ADDR_RMSK                                                                     0x7ffff
#define HWIO_SDE_RSCC_TCSm_CMDn_DRV0_ADDR_MAXm                                                                           3
#define HWIO_SDE_RSCC_TCSm_CMDn_DRV0_ADDR_MAXn                                                                          15
#define HWIO_SDE_RSCC_TCSm_CMDn_DRV0_ADDR_INI2(m,n)        \
        in_dword_masked(HWIO_SDE_RSCC_TCSm_CMDn_DRV0_ADDR_ADDR(m,n), HWIO_SDE_RSCC_TCSm_CMDn_DRV0_ADDR_RMSK)
#define HWIO_SDE_RSCC_TCSm_CMDn_DRV0_ADDR_INMI2(m,n,mask)    \
        in_dword_masked(HWIO_SDE_RSCC_TCSm_CMDn_DRV0_ADDR_ADDR(m,n), mask)
#define HWIO_SDE_RSCC_TCSm_CMDn_DRV0_ADDR_OUTI2(m,n,val)    \
        out_dword(HWIO_SDE_RSCC_TCSm_CMDn_DRV0_ADDR_ADDR(m,n),val)
#define HWIO_SDE_RSCC_TCSm_CMDn_DRV0_ADDR_OUTMI2(m,n,mask,val) \
        out_dword_masked_ns(HWIO_SDE_RSCC_TCSm_CMDn_DRV0_ADDR_ADDR(m,n),mask,val,HWIO_SDE_RSCC_TCSm_CMDn_DRV0_ADDR_INI2(m,n))
#define HWIO_SDE_RSCC_TCSm_CMDn_DRV0_ADDR_SLV_ID_BMSK                                                              0x70000
#define HWIO_SDE_RSCC_TCSm_CMDn_DRV0_ADDR_SLV_ID_SHFT                                                                 0x10
#define HWIO_SDE_RSCC_TCSm_CMDn_DRV0_ADDR_OFFSET_BMSK                                                               0xffff
#define HWIO_SDE_RSCC_TCSm_CMDn_DRV0_ADDR_OFFSET_SHFT                                                                  0x0

#define HWIO_SDE_RSCC_TCSm_CMDn_DRV0_DATA_ADDR(m,n)                                                             (SDE_RSCC_RSCC_RSC_REG_BASE      + 0x00001c38 + 0x2A0 * (m) + 0x14 * (n))
#define HWIO_SDE_RSCC_TCSm_CMDn_DRV0_DATA_RMSK                                                                  0xffffffff
#define HWIO_SDE_RSCC_TCSm_CMDn_DRV0_DATA_MAXm                                                                           3
#define HWIO_SDE_RSCC_TCSm_CMDn_DRV0_DATA_MAXn                                                                          15
#define HWIO_SDE_RSCC_TCSm_CMDn_DRV0_DATA_INI2(m,n)        \
        in_dword_masked(HWIO_SDE_RSCC_TCSm_CMDn_DRV0_DATA_ADDR(m,n), HWIO_SDE_RSCC_TCSm_CMDn_DRV0_DATA_RMSK)
#define HWIO_SDE_RSCC_TCSm_CMDn_DRV0_DATA_INMI2(m,n,mask)    \
        in_dword_masked(HWIO_SDE_RSCC_TCSm_CMDn_DRV0_DATA_ADDR(m,n), mask)
#define HWIO_SDE_RSCC_TCSm_CMDn_DRV0_DATA_OUTI2(m,n,val)    \
        out_dword(HWIO_SDE_RSCC_TCSm_CMDn_DRV0_DATA_ADDR(m,n),val)
#define HWIO_SDE_RSCC_TCSm_CMDn_DRV0_DATA_OUTMI2(m,n,mask,val) \
        out_dword_masked_ns(HWIO_SDE_RSCC_TCSm_CMDn_DRV0_DATA_ADDR(m,n),mask,val,HWIO_SDE_RSCC_TCSm_CMDn_DRV0_DATA_INI2(m,n))
#define HWIO_SDE_RSCC_TCSm_CMDn_DRV0_DATA_DATA_BMSK                                                             0xffffffff
#define HWIO_SDE_RSCC_TCSm_CMDn_DRV0_DATA_DATA_SHFT                                                                    0x0

#define HWIO_SDE_RSCC_TCSm_CMDn_DRV0_STATUS_ADDR(m,n)                                                           (SDE_RSCC_RSCC_RSC_REG_BASE      + 0x00001c3c + 0x2A0 * (m) + 0x14 * (n))
#define HWIO_SDE_RSCC_TCSm_CMDn_DRV0_STATUS_RMSK                                                                   0x10101
#define HWIO_SDE_RSCC_TCSm_CMDn_DRV0_STATUS_MAXm                                                                         3
#define HWIO_SDE_RSCC_TCSm_CMDn_DRV0_STATUS_MAXn                                                                        15
#define HWIO_SDE_RSCC_TCSm_CMDn_DRV0_STATUS_INI2(m,n)        \
        in_dword_masked(HWIO_SDE_RSCC_TCSm_CMDn_DRV0_STATUS_ADDR(m,n), HWIO_SDE_RSCC_TCSm_CMDn_DRV0_STATUS_RMSK)
#define HWIO_SDE_RSCC_TCSm_CMDn_DRV0_STATUS_INMI2(m,n,mask)    \
        in_dword_masked(HWIO_SDE_RSCC_TCSm_CMDn_DRV0_STATUS_ADDR(m,n), mask)
#define HWIO_SDE_RSCC_TCSm_CMDn_DRV0_STATUS_COMPLETED_BMSK                                                         0x10000
#define HWIO_SDE_RSCC_TCSm_CMDn_DRV0_STATUS_COMPLETED_SHFT                                                            0x10
#define HWIO_SDE_RSCC_TCSm_CMDn_DRV0_STATUS_ISSUED_BMSK                                                              0x100
#define HWIO_SDE_RSCC_TCSm_CMDn_DRV0_STATUS_ISSUED_SHFT                                                                0x8
#define HWIO_SDE_RSCC_TCSm_CMDn_DRV0_STATUS_TRIGGERED_BMSK                                                             0x1
#define HWIO_SDE_RSCC_TCSm_CMDn_DRV0_STATUS_TRIGGERED_SHFT                                                             0x0

#define HWIO_SDE_RSCC_TCSm_CMDn_DRV0_READ_RESPONSE_DATA_ADDR(m,n)                                               (SDE_RSCC_RSCC_RSC_REG_BASE      + 0x00001c40 + 0x2A0 * (m) + 0x14 * (n))
#define HWIO_SDE_RSCC_TCSm_CMDn_DRV0_READ_RESPONSE_DATA_RMSK                                                    0xffffffff
#define HWIO_SDE_RSCC_TCSm_CMDn_DRV0_READ_RESPONSE_DATA_MAXm                                                             3
#define HWIO_SDE_RSCC_TCSm_CMDn_DRV0_READ_RESPONSE_DATA_MAXn                                                            15
#define HWIO_SDE_RSCC_TCSm_CMDn_DRV0_READ_RESPONSE_DATA_INI2(m,n)        \
        in_dword_masked(HWIO_SDE_RSCC_TCSm_CMDn_DRV0_READ_RESPONSE_DATA_ADDR(m,n), HWIO_SDE_RSCC_TCSm_CMDn_DRV0_READ_RESPONSE_DATA_RMSK)
#define HWIO_SDE_RSCC_TCSm_CMDn_DRV0_READ_RESPONSE_DATA_INMI2(m,n,mask)    \
        in_dword_masked(HWIO_SDE_RSCC_TCSm_CMDn_DRV0_READ_RESPONSE_DATA_ADDR(m,n), mask)
#define HWIO_SDE_RSCC_TCSm_CMDn_DRV0_READ_RESPONSE_DATA_READ_RESPONSE_DATA_BMSK                                 0xffffffff
#define HWIO_SDE_RSCC_TCSm_CMDn_DRV0_READ_RESPONSE_DATA_READ_RESPONSE_DATA_SHFT                                        0x0


#endif /* __PDCHAL_HWIO_H__ */
