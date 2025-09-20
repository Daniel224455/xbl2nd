#ifndef __PLATFORMINFOHWIO_H__
#define __PLATFORMINFOHWIO_H__
/*
===========================================================================
*/
/**
  @file PlatformInfoHWIO.h
  @brief Auto-generated HWIO interface include file.

  Reference chip release:
    SC8180X (Poipu) [poipu_v1.0_p3q3r65_IPCAT]
 
  This file contains HWIO register definitions for the following modules:
    TCSR_TCSR_REGS

  'Include' filters applied: TCSR_SOC_EMULATION_TYPE[TCSR_TCSR_REGS] 
  'Exclude' filters applied: DUMMY RESERVED 

  Generation parameters: 
  { u'exclude-reserved': True,
    u'filename': u'PlatformInfoHWIO.h',
    u'header': u'#include "msmhwiobase.h"\n\n#define PLATFORMINFO_TCSR_SOC_EMULATION_TYPE    TCSR_SOC_EMULATION_TYPE\n',
    u'module-filter-exclude': { },
    u'module-filter-include': { u'TCSR_TCSR_REGS': [u'TCSR_SOC_EMULATION_TYPE']},
    u'modules': [u'TCSR_TCSR_REGS']}
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

#define PLATFORMINFO_TCSR_SOC_EMULATION_TYPE    TCSR_SOC_EMULATION_TYPE


/*----------------------------------------------------------------------------
 * MODULE: TCSR_TCSR_REGS
 *--------------------------------------------------------------------------*/

#define TCSR_TCSR_REGS_REG_BASE                                                                                      (CORE_TOP_CSR_BASE      + 0x000c0000)
#define TCSR_TCSR_REGS_REG_BASE_SIZE                                                                                 0x30000
#define TCSR_TCSR_REGS_REG_BASE_USED                                                                                 0x2c004

#define HWIO_TCSR_SOC_EMULATION_TYPE_ADDR                                                                            (TCSR_TCSR_REGS_REG_BASE      + 0x00008004)
#define HWIO_TCSR_SOC_EMULATION_TYPE_RMSK                                                                                   0x3
#define HWIO_TCSR_SOC_EMULATION_TYPE_IN          \
        in_dword_masked(HWIO_TCSR_SOC_EMULATION_TYPE_ADDR, HWIO_TCSR_SOC_EMULATION_TYPE_RMSK)
#define HWIO_TCSR_SOC_EMULATION_TYPE_INM(m)      \
        in_dword_masked(HWIO_TCSR_SOC_EMULATION_TYPE_ADDR, m)
#define HWIO_TCSR_SOC_EMULATION_TYPE_SOC_EMULATION_TYPE_BMSK                                                                0x3
#define HWIO_TCSR_SOC_EMULATION_TYPE_SOC_EMULATION_TYPE_SHFT                                                                0x0


#endif /* __PLATFORMINFOHWIO_H__ */
