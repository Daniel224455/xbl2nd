/*===========================================================================
                              pdc_seq_cfg.c

DESCRIPTION:
  This file provides configuration data like sequences, delays, etc for
  various PDC sequencers handled on a given subsystem.

  Copyright (c) 2016-2018 Qualcomm Technologies, Inc.
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
===========================================================================*/

//===========================================================================
//                   Includes and Variable Definitions
//===========================================================================

//---------------------------------------------------------------------------
//  Include Files
//---------------------------------------------------------------------------

#include "pdc_seq.h"
#include "pdc_seq_internal.h"
#include "pdc_seq_hal.h"
#include "pdc_seq_hal_bsp.h"
#include "pdc_seq_hwio.h"

#define PDC_SEQ_ARRAY(arr) arr, (sizeof(arr)/sizeof(arr[0]))

//---------------------------------------------------------------------------
//  Global Variables
//---------------------------------------------------------------------------

/* Cx collapse with AOSS sleep */
pdc_seq_cmd_t cx_min_aoss[] =
{
  0xe1,    //CX_COL_AOSS: WAIT1    rx_fifo_empty         # Ensure RX FIFO empty from any previous SDE RSCC commands.
  0xa1,    //             SET1     pdc_mode_control      # Switch ePCB from "pass-throug mode" to PDC TCS logic,
  0x80,    //             SET0     timer_match_valid     # Enable use of PDC timer to not set from PDC_MATCH_VALUE above, CR QCTDD03836087
  0xbf,    //             SET1     seq_tcs_1_start       # Send TCS0 contents, that is, the "sleep set".
  0xff,    //             WAIT1    tcs_0_seq_cmp         # Wait for all commands to be sent. TCS0 setup with fire-and-forget.
  0xa5,    //             SET1      wfi_profiling_unit_3 # Trigger time stamp on profiling unit 3.
  0xa2,    //             SET1     soc_sleep_req         # Signal AOP that AOSS sleep is requested.
  0xa4,    //             SET1      dbg_resource_rel_req # To Enable AOP to release debug resources
  0xe0,    //             WAIT1    sequence_resume       # Wait for wake event of: PDC modem_pdc_irq_in, PDC timer, or
  0xa6,    //             SET1  generic_profiling_unit_5 # HW workaround for QCTDD03447497
  0x82,    //             SET0     soc_sleep_req         # Clear signal for AOSS sleep request.
  0x84,    //             SET0      dbg_resource_rel_req # To indicate to AOP to vote for debug resources
  0xe2,    //             WAIT1    soc_sleep_unlock      # Wait for AOP to signal AOSS sleep state exit.
  0xbd,    //             SET1     seq_tcs_2_start       # Send TCS2 contents, that is, the "wake set".
  0xfd,    //             WAIT1    tcs_2_seq_cmp         # Wait for all commands to be sent. TSC1 setup with wait-for-complete.
  0x80,    //             SET0     timer_match_valid     # Disable PDC timer.
  0x81,    //             SET0     pdc_mode_control      # Restore "pas-through mode".
  0x20,    //             RETURN                         # End of sequence
  0x20,    //             RETURN                         # Pad to word boundary
  0x20,    //             RETURN                         # Pad to word boundary
};

/* Branch addresses - dummy for testing */
uint8 branches[PDC_SEQ_HAL_BR_ADDR_REG_COUNT] = { 0x0 };

/* Delay values - dummy for testing */
uint32 delays[PDC_SEQ_HAL_DELAY_REG_COUNT] =  { 0x0 };

pdc_seq_hal_bsp_cfg_t pdc_cfg =
{
  PDC_SEQ_ARRAY (branches),
  PDC_SEQ_ARRAY (delays)
};

/* PDC supported low power mode */
pdc_seq_hal_bsp_mode_t pdc_modes[] =
{
  {
    PDC_SEQ_ARRAY (cx_min_aoss),
    1,
    NULL
  }
};

/* PDC sequencers handled on this Subsystem */
pdc_seq_t g_pdc_seq_instances[] =
{
  {
    "display",
    &pdc_cfg,
    PDC_SEQ_ARRAY (pdc_modes),
    DISPLAY_PDC_REG_BASE - AOSS_BASE,  //Get target base
    (uint8 *)DISPLAY_PDC_SEQ_MEM_REG_BASE,
    0
  },
};

pdc_seq_t g_pdc_seq_instancesV2[] =
{
  {
    "display",
    &pdc_cfg,
    PDC_SEQ_ARRAY (pdc_modes),
    DISPLAY_PDC_REG_BASE - AOSS_BASE + 0x10000,  //Get target base
    (uint8 *)DISPLAY_PDC_SEQ_MEM_REG_BASE + 0x10000,
    0
  },
};

uint32 g_pdc_seq_instance_count =
  sizeof (g_pdc_seq_instances) / sizeof (g_pdc_seq_instances[0]);
uint32 g_pdc_seq_instance_countV2 =
  sizeof (g_pdc_seq_instancesV2) / sizeof (g_pdc_seq_instances[0]);
  

  
