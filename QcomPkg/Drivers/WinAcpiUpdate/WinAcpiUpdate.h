/*============================================================================
  @file WINACPIUPDATE_H

AcpiPlatfrom Callback functions for chipinfo 
               Copyright (c) 2016-2018,2023 by Qualcomm Technologies, Inc. All Rights Reserved.
============================================================================*/

/*=============================================================================
                              EDIT HISTORY


 when       who     what, where, why
 --------   ---     -----------------------------------------------------------
 03/21/23  vbu      Porting partial binning changes of MK to address security issues. Driver is currently disabled for Rennell.
 02/27/18  ltg      Added devices for Poipu
 08/4/16   vahidd   Intial Draft for Patching MADT tables.
=============================================================================*/


#ifndef WINACPIUPDATE_H
#define WINACPIUPDATE_H

#include <HALhwio.h>
#include "PlatformInfoHWIO.h"

/**
Device node information.
*/
#define EFI_DEVICE_CPU "NULL"
#define EFI_DEVICE_GPU "\\_SB.GPU0._STA"
#define EFI_DEVICE_MMU1 "\\_SB.MMU1._STA"
#define EFI_DEVICE_IMM1 "\\_SB.IMM1._STA"
#define EFI_DEVICE_NSP1 "NULL"
#define EFI_DEVICE_NSP0 "\\_SB.NSP0._STA"
#define EFI_DEVICE_SENSORS "\\_SB.SCSS._STA"
#define EFI_DEVICE_SPSS "\\_SB.SPSS._STA"

char * DEVICE_LIST[EFI_WIN_ACPI_PART_MAX] = {
    EFI_DEVICE_CPU,
    EFI_DEVICE_GPU,
    EFI_DEVICE_MMU1,
    EFI_DEVICE_IMM1,
    EFI_DEVICE_NSP1,
    EFI_DEVICE_NSP0,
    EFI_DEVICE_SENSORS,
    EFI_DEVICE_SPSS
};

#define GOLD_PLUS_BIN_MASK 0xF0
#define CPU_FUSE_MASK 0x1
#define GPU_FUSE_MASK 0x2
#define NSP1_FUSE_MASK 0x4
#define NSP_BOTH_FUSE_MASK 0x8
#define SCSS_FUSE_MASK 0x10
#define SPSS_FUSE_MASK 0x10

#define SOC_RUMI_AND_NON_GPU 0x40000001
#define SOC_PLATFORM_AND_GPU_MASK 0x50000003

#define RUMI_NO_GPU_CHECK() ((HWIO_IN(TCSR_SOC_EMULATION_TYPE) & SOC_PLATFORM_AND_GPU_MASK) == SOC_RUMI_AND_NON_GPU)

/**
Global variables
*/
static EFI_ACPI_SDT_PROTOCOL *gAcpiSdt = NULL;
static EFI_CHIPINFO_PROTOCOL *pChipInfoProtocol = NULL;

static UINT8 defectivePartsBitMask = 0x0;
static UINT8 defectivePartsFuseMask = 0x0;

#endif
