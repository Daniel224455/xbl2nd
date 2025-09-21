/**
@file SmbiosLib.h
@brief SMBIOS chipset specific definitions/routines

This file contains the target specific information for SMBIOS

Copyright 2018 by Qualcomm Technologies, Inc.  All Rights Reserved

*/

/*=============================================================================
                              EDIT HISTORY
  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.


  $Header: //components/rel/boot.xf/2.2/QcomPkg/SDM845Pkg/Include/SmbiosLibTarget.h#1 $
  $DateTime: 2018/06/10 22:43:55 $
  $Author: pwbldsvc $

 when           who         what, where, why
 --------       ---         --------------------------------------------------
 2018/06/08     ltg         Initial version
=============================================================================*/

#ifndef SMBIOS_LIB_CHIPSET_H
#define SMBIOS_LIB_CHIPSET_H

#define NUM_SPEED_BINS 5
UINTN SPEED_BIN_FREQ_MHZ[NUM_SPEED_BINS] = { 2400, 2746, 2803, 2900, 2957 };

#endif
