/** @file DDRInfoPartitionLib.h
  
  DDR Info interface

  Copyright (c) 2017-2018 by Qualcomm Technologies, Inc. 
  All Rights Reserved.
 
**/

/*=======================================================================
                        Edit History

when       who     what, where, why
--------   ----    ---------------------------------------------------
04/27/17   ddk      Initital Version
========================================================================*/

#include <Uefi.h>
#include <MemRegionInfo.h>
#include <DDRDetails.h>


/**
  Get DDR details from DDR info stored in shared memory
**/

EFI_STATUS
GetDDRDetails (IN OUT struct ddr_details_entry_info *ddr_detail);

/**
  Get DDR Frequency Table from DDR info stored in shared memory
**/
EFI_STATUS
GetDDRFreqTable (IN OUT struct ddr_freq_plan_entry_info *clk_plan_tbl);

/**
  Get DDR Frequency
**/
EFI_STATUS
GetDDRFreq (IN OUT UINT32    *ddr_freq);

/**
  Get DDR Max Enabled Frequency
**/
EFI_STATUS
GetDDRMaxEnabledFreq (IN OUT UINT32    *ddr_freq);

/**
  Set DDR Frequency
**/
EFI_STATUS
SetDDRFreq (IN UINT32    ddr_freq);

/**
  Switch DDR frequency
**/
UINT32 switch_ddr_freq(UINT32 freq_khz);

/**
  Remove DDR Request Client for DDR frequency switching
**/
void deinit_ddr_clk();


