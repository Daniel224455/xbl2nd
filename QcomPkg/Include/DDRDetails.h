/** 
  @file  DDRDetails.h
  @brief DDR device details shared structure interface.
*/
/*=============================================================================
  Copyright (c) 2017 Qualcomm Technologies, Incorporated.
  All rights reserved.
  Qualcomm Technologies, Confidential and Proprietary.
=============================================================================*/

/*=============================================================================
                              EDIT HISTORY


 when       who       what, where, why
 --------   ---       -----------------------------------------------------------
 09/26/17   daisond   changed maximum size required for ddr params for all channels
 05/26/17   daisond   Initial Revision
=============================================================================*/

#ifndef DDRDETAILS_H
#define DDRDETAILS_H

#include <api/boot/ddr_common.h>
#include <api/boot/ddr_aop_params.h>

#define DDR_DETAILS_STRUCT_VERSION 0x0000000000030000


struct ddr_freq_table{
   UINT32 freq_khz;
   UINT8  enable;
};

typedef struct ddr_freq_plan_entry_info 
{
  struct ddr_freq_table ddr_freq[MAX_NUM_CLOCK_PLAN];
  UINT8  num_ddr_freqs;
  UINT32* clk_period_address;
}ddr_freq_plan_entry;

struct ddr_part_details
{

  UINT8 revision_id1[2];
  UINT8 revision_id2[2];
  UINT8 width[2];
  UINT8 density[2];
  
};

typedef struct ddr_details_entry_info 
{

  UINT8   manufacturer_id;
  UINT8   device_type;  
  struct  ddr_part_details ddr_params[MAX_IDX];
  ddr_freq_plan_entry     ddr_freq_tbl;
  UINT8   num_channels;
  
}ddr_details_entry;


#endif /* DDRDETAILS_H */


