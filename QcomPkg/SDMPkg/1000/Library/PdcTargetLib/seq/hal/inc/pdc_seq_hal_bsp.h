#ifndef __PDC_SEQ_HAL_BSP_H__
#define __PDC_SEQ_HAL_BSP_H__
/*===========================================================================
                            pdc_seq_hal_bsp.h

DESCRIPTION:
  This file defines various types for target specific data. Usually they
  are based on underlying HW block.

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
//                      Includes and Type Definitions
//===========================================================================

//---------------------------------------------------------------------------
//  Include Files
//---------------------------------------------------------------------------

#include "comdef.h"


//---------------------------------------------------------------------------
//  Type Declarations
//---------------------------------------------------------------------------
/* typedef for sequencer command type */
typedef uint8 pdc_seq_cmd_t;

/* Sequencer override configuration for a low power mode.
 *
 * The fields represent overridable properties of the sequencer for a low
 * power mode. */
typedef struct pdc_seq_hal_bsp_override_s
{
  uint32 pwr_ctl_mask;       /* Mask - one bit per applicable pwr_ctl */
  uint32 pwr_ctl_val;        /* Override value - one bit per pwr_ctl */
  uint32 wait_event_mask;    /* Mask - one bit per applicable wait_event */
  uint32 wait_event_val;     /* Value - one bit per wait_event */
  uint32 br_event_mask;      /* Mask - one bit per applicable branch_event */
  uint32 br_event_val;       /* Value - one bit per branch_event */
} pdc_seq_hal_bsp_override_t;

/* Structure containing one time configuration of PDC sequencer */
typedef struct pdc_seq_hal_bsp_cfg_s
{
  uint8* br_addr;            /* Branch addresses for sequences */
  uint32 br_count;           /* Number of branch addresses */
  uint32* delay;             /* Delays for sequences */
  uint32 delay_count;        /* Number of delays */
} pdc_seq_hal_bsp_cfg_t;

/* Type containing information on a low power mode from PDC sequencer
 * point of view. */
typedef struct pdc_seq_hal_bsp_mode_s
{
  pdc_seq_cmd_t *cmds;
  uint16 length;
  uint16 mode_id;
  pdc_seq_hal_bsp_override_t *override;
  int16 start_addr;          /* Internal fields */
} pdc_seq_hal_bsp_mode_t;

#endif // __PDC_SEQ_HAL_BSP_H__
