/*===========================================================================
                             pdc_seq_target.c

DESCRIPTION:
  This file implements target specific functions for PDC sequencer driver.

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
#include "DDIChipInfo.h"

//---------------------------------------------------------------------------
//  Global and External Variables
//---------------------------------------------------------------------------

/* Pointer to the list of PDC sequencer handles and list size - must be set
 * up during target initialization */
pdc_seq_t *g_pdc_seqs = NULL;
uint32 g_pdc_seq_count = 0;

/* Externed variables for PDC sequencer configuration/handles for different
 * version of a chipset. At the moment we just have these for just one
 * version but if newer version requires different config, extern them
 * here */
extern pdc_seq_t g_pdc_seq_instances[];
extern pdc_seq_t g_pdc_seq_instancesV2[];
extern uint32 g_pdc_seq_instance_count;
extern uint32 g_pdc_seq_instance_countV2;
extern uint32 ChipVersion;

//===========================================================================
//                       Function Defintions
//===========================================================================

//---------------------------------------------------------------------------
//  Public Function Definitions
//---------------------------------------------------------------------------

/**
 * <!-- pdc_seq_target_init -->
 *
 * @brief Target specific initialization for the PDC sequencer driver.
 *
 * Usually this function is expected to be called as part of common driver
 * initialization and will obtain different PDCs driver will talk to.
 */
void pdc_seq_target_init (void)
{
  /* Get chip version if becomes necessary */
  DalChipInfoVersionType version = DalChipInfo_ChipVersion ();

  if(version < DALCHIPINFO_VERSION(2, 0))
  {
    g_pdc_seqs = g_pdc_seq_instances;
	g_pdc_seq_count = g_pdc_seq_instance_count;
  }
  else
  {
    g_pdc_seqs = g_pdc_seq_instancesV2;
	g_pdc_seq_count = g_pdc_seq_instance_countV2;
  }
}

/**
 * <!-- pdc_seq_handle_target_init -->
 *
 * @brief Performs any target specific initialization required for a given
 *        pdc handle e.g. disabling certain feature or updating some settings.
 *
 * This is an internal function expected to be called during PDC
 * handle initialization.
 *
 * @note No input validation as it is an internal function.
 *
 * @param handle: PDC handle to be initialized.
 *
 * @return PDC Sequencer error code (@see pdc_seq_result_t)
 */
pdc_seq_result_t pdc_seq_handle_target_init (pdc_seq_handle handle)
{
  /* Bug in ChipInfo driver that returns version 0 as success if SMEM
   * is not populated - That's typical for pre-sil environment. Disabling
   * version 0 check to avoid adding extra dependency on standalone
   * environment. Can be added post SoD */
  // if (!ChipVersions)
  // {
  //  return PDC_SEQ_ERROR;
  // }

  if (!PDC_SEQ_VALID_HANDLE (handle))
  {
    return PDC_SEQ_INVALID_HANDLE;
  }

  /* Fix for HW errata QCTDD03459865 on Napali V1 - disable clock gating */
  //if (ChipVersion < DALCHIPINFO_VERSION(2, 0))
  //{
  //  pdc_seq_hal_clk_gate_enable (handle->addr, 0);
  //}

  return PDC_SEQ_SUCCESS;
}
