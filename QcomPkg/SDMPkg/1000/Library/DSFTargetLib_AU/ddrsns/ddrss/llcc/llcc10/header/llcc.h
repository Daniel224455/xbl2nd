/*******************************************************************************
 QUALCOMM Proprietary Design Data
 Copyright (c) 2017-2018, Qualcomm Technologies Incorporated.
 All rights reserved.
 ******************************************************************************/
/*==============================================================================
================================================================================*/
/***************************************************************************************************
 Auto-generated sequence C codes from Autoseq 2.4.4 2/2/2016
 User Name:fengding
 Input File:llcc.blk
 *************************************************************************************************** 
 All data and information contained in or disclosed by this document are confidential and
 proprietary information of QUALCOMM Technologies Incorporated, and all rights therein are expressly
 reserved. By accepting this material, the recipient agrees that this material and the
 information contained therein are held in confidence and in trust and will not be used,
 copied, reproduced in whole or in part, nor its contents revealed in any manner to others
 without the express written permission of QUALCOMM Technologies Incorporated.
 
 This technology was exported from the United States in accordance with the Export
 Administration Regulations. Diversion contrary to U. S. law prohibited.
 ***************************************************************************************************/

#ifndef _LLCC_H
#define _LLCC_H

#ifndef _LLCC_SEQ_HEADER_
#define _LLCC_SEQ_HEADER_
#include "llcc_seq_hwiobase.h"
#include "llcc_seq_hwioreg.h"
#include "ddr_ss_seq_hwiobase.h"
#include "ddr_ss_seq_hwioreg.h"
#include "ddr_common.h"
#include "ddrss.h"
#include "llcc_common.h"
//#define DUMMY                                                        0

#define ACTIVITY_BASED_SLEEP_EN			0

#define NUMBER_OF_SCIDS                        32


void LLCC_config (BOOT_DDR_STRUCT *ddr, DDR_CHANNEL channel);
void LLCC_init (BOOT_DDR_STRUCT *ddr, DDR_CHANNEL channel);
void LLCC_initialize_tag_ram (DDR_CHANNEL channel);
void LLCC_configure_beac_credit (BOOT_DDR_STRUCT *ddr);
void LLCC_cacheable_mode_override_ctrl (CACHEABLE_MODE cachable_mode);
void LLCC_ram_init (BOOT_DDR_STRUCT *ddr);
void LLCC_PXI_flush (BOOT_DDR_STRUCT *ddr, DDR_CHANNEL channel, uint16 start_inx, uint16 end_inx, uint16 way_vec);
void LLCC_PXI_TCM_mode_ctrl (BOOT_DDR_STRUCT *ddr, DDR_CHANNEL channel, uint8 act_en);
void LLCC_magic_number(BOOT_DDR_STRUCT *ddr);

#endif
#endif
