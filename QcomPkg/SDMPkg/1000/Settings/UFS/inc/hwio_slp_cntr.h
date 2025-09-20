#ifndef __HWIO_SLP_CNTR_H__
#define __HWIO_SLP_CNTR_H__
/*
===========================================================================
*/
/**
  @file hwio_slp_cntr.h
  @brief Auto-generated HWIO interface include file.

  Reference chip release:
    SDM855 (Hana) [hana_v1.0_p1q0r10]
 
  This file contains HWIO register definitions for the following modules:
    SLP_CNTR

  'Include' filters applied: 
  'Exclude' filters applied: 
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

/*----------------------------------------------------------------------------
 * MODULE: SLP_CNTR
 *--------------------------------------------------------------------------*/

#define SLP_CNTR_REG_BASE                                    (AOSS_BASE      + 0x01221000)

#define HWIO_MPM_SLEEP_TIMETICK_COUNT_VAL_ADDR               (SLP_CNTR_REG_BASE      + 0x00000000)
#define HWIO_MPM_SLEEP_TIMETICK_COUNT_VAL_RMSK               0xffffffff
#define HWIO_MPM_SLEEP_TIMETICK_COUNT_VAL_IN          \
        in_dword_masked(HWIO_MPM_SLEEP_TIMETICK_COUNT_VAL_ADDR, HWIO_MPM_SLEEP_TIMETICK_COUNT_VAL_RMSK)
#define HWIO_MPM_SLEEP_TIMETICK_COUNT_VAL_INM(m)      \
        in_dword_masked(HWIO_MPM_SLEEP_TIMETICK_COUNT_VAL_ADDR, m)
#define HWIO_MPM_SLEEP_TIMETICK_COUNT_VAL_DATA_BMSK          0xffffffff
#define HWIO_MPM_SLEEP_TIMETICK_COUNT_VAL_DATA_SHFT                 0x0


#endif /* __HWIO_SLP_CNTR_H__ */


