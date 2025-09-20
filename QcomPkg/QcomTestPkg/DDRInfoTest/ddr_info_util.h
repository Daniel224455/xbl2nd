/*
===========================================================================
*/
/**
  @file ddr_info_util.h

*/
/*
  ====================================================================

  Copyright (c) 2018 Qualcomm Technologies Incorporated.  All Rights Reserved.

  ====================================================================

  when       who     what, where, why
  --------   ---     -------------------------------------------------
  10/16/18   dais    Initial revision.

  ====================================================================
*/
#ifndef DDR_INFO_UTIL_H
#define DDR_INFO_UTIL_H
/*==============================================================================

  FUNCTION      ddr_uefi_display_init

  DESCRIPTION   Locate Display Control protocol

==============================================================================*/
UINT32 ddr_uefi_display_init();

/*==============================================================================

  FUNCTION      ddr_uefi_display_off

  DESCRIPTION   Function to change primary display state as OFF

==============================================================================*/
UINT32 ddr_uefi_display_off();

/*==============================================================================

  FUNCTION      ddr_uefi_display_on

  DESCRIPTION   Function to change primary display state as ON

==============================================================================*/
UINT32 ddr_uefi_display_on();

/*==============================================================================

  FUNCTION      ddr_setlowbandwidth

  DESCRIPTION   Function to set low bandwidth for DDR

==============================================================================*/
EFI_STATUS ddr_setlowbandwidth(void);

#endif