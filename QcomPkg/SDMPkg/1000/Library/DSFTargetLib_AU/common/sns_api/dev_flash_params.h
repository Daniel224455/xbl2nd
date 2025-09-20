#ifndef __DEV_FLASH_PARAMS_H__
#define __DEV_FLASH_PARAMS_H__

/*=============================================================================

                                DDR device Flash PARAMETERS
                                Header File
GENERAL DESCRIPTION
This file defines the DDR parameters stored in DDR Flash partition
=============================================================================*/
/*******************************************************************************
 QUALCOMM Proprietary Design Data
 Copyright (c) 2013-2018, Qualcomm Technologies Incorporated.
 All rights reserved.
 ******************************************************************************/
/*==============================================================================
================================================================================*/
/*==========================================================================
                               INCLUDE FILES
===========================================================================*/
#include "HALcomdef.h"
#include "phy_common.h"

/*==============================================================================
                                  TYPES & DEFINES
==============================================================================*/

#define TRAINING_DATA_SIZE_IN_BYTES sizeof(struct training_results)

/******************************************************/
/*** DDR Parameter partition datastructure ***/
/******************************************************/

struct ddr_params_partition
{
  uint32 magic_number;
  uint32 version;
  uint32 training_data_size;
  uint32 training_log_size;
  uint32 checksum;
  uint32 stored_ddr_fmax;
  uint32 boot_dit_factory_flag;
  uint16 boot_dit_runtime_count;
  uint16 boot_dit_count[NUM_CH][NUM_DIT_FREQS][NUM_CS][NUM_DIE_PCH];
  /* PHY code is allowed to restructure needed fields within the training_data array, as required */
  uint8 training_data[TRAINING_DATA_SIZE_IN_BYTES];  
};
#endif /* __DEV_FLASH_PARAMS_H__ */
