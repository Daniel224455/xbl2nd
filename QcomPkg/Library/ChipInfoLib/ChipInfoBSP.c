/*==============================================================================
  @file ChipInfoBSP.c

  ChipInfo BSP data.

================================================================================
        Copyright (c) 2016-2017 Qualcomm Technologies, Inc.
               All Rights Reserved.
            QUALCOMM Proprietary/GTDR
================================================================================

  $Header: //components/rel/boot.xf/2.2/QcomPkg/Library/ChipInfoLib/ChipInfoBSP.c#2 $
  $DateTime: 2017/11/20 03:24:18 $
  $Author: pwbldsvc $

==============================================================================*/

/*==============================================================================
      Include Files
==============================================================================*/
#include "comdef.h"
#include "ChipInfoLocal.h"
#include "ChipInfoLoader.h"

/*==============================================================================
      Data Definitons
==============================================================================*/
#define ARRAY_SIZE(arr) (sizeof(arr)/sizeof(arr[0]))

/*==============================================================================
      Data Definitons
==============================================================================*/

/*
 * ChipInfoDataLUT
 *
 * Array of chip id strings, chip IDs, part numbers, and modem support.
 *
 */
static const ChipInfoDataLUTType ChipInfoDataLUT[] =
{
  { "UNKNOWN",    CHIPINFO_ID_UNKNOWN,       CHIPINFO_PARTNUM_UNKNOWN,      0 },
  { "SDM845",     CHIPINFO_ID_SDM845,        CHIPINFO_PARTNUM_SDM845,       CHIPINFO_MODEM_WWAN | CHIPINFO_MODEM_CGPS | CHIPINFO_MODEM_LTE },
  { "SDM830",     CHIPINFO_ID_SDM830,        CHIPINFO_PARTNUM_SDM830,       CHIPINFO_MODEM_WWAN | CHIPINFO_MODEM_CGPS | CHIPINFO_MODEM_LTE },
  { "SDC830",     CHIPINFO_ID_SDC830,        CHIPINFO_PARTNUM_SDC830,       CHIPINFO_MODEM_WWAN | CHIPINFO_MODEM_CGPS | CHIPINFO_MODEM_LTE },
  { "SDX24",      CHIPINFO_ID_SDX24,         CHIPINFO_PARTNUM_SDX24,        CHIPINFO_MODEM_WWAN | CHIPINFO_MODEM_CGPS | CHIPINFO_MODEM_LTE },
  { "SDX24M",     CHIPINFO_ID_SDX24M,        CHIPINFO_PARTNUM_SDX24M,       CHIPINFO_MODEM_WWAN | CHIPINFO_MODEM_CGPS | CHIPINFO_MODEM_LTE },
  { "SDM670",     CHIPINFO_ID_SDM670,        CHIPINFO_PARTNUM_SDM670,       CHIPINFO_MODEM_WWAN | CHIPINFO_MODEM_CGPS | CHIPINFO_MODEM_LTE },
  { "SDA670",     CHIPINFO_ID_SDA670,        CHIPINFO_PARTNUM_SDA670,       CHIPINFO_MODEM_WWAN | CHIPINFO_MODEM_CGPS | CHIPINFO_MODEM_LTE },
  { "SDM450",     CHIPINFO_ID_SDM450,        CHIPINFO_PARTNUM_SDM450,       CHIPINFO_MODEM_WWAN | CHIPINFO_MODEM_CGPS | CHIPINFO_MODEM_LTE },
  { "SDM855",     CHIPINFO_ID_SDM855,        CHIPINFO_PARTNUM_SDM855,       CHIPINFO_MODEM_WWAN | CHIPINFO_MODEM_CGPS | CHIPINFO_MODEM_LTE },
  { "SDM1000",    CHIPINFO_ID_SDM1000,       CHIPINFO_PARTNUM_SDM1000,      CHIPINFO_MODEM_WWAN | CHIPINFO_MODEM_CGPS | CHIPINFO_MODEM_LTE },
  { "SDA845",     CHIPINFO_ID_SDA845,        CHIPINFO_PARTNUM_SDA845,       CHIPINFO_MODEM_WWAN | CHIPINFO_MODEM_CGPS | CHIPINFO_MODEM_LTE },
  { "QCS605",     CHIPINFO_ID_QCS605,        CHIPINFO_PARTNUM_QCS605,       CHIPINFO_MODEM_WWAN | CHIPINFO_MODEM_CGPS | CHIPINFO_MODEM_LTE },
  { "SDM850",     CHIPINFO_ID_SDM850,        CHIPINFO_PARTNUM_SDM850,       CHIPINFO_MODEM_WWAN | CHIPINFO_MODEM_CGPS | CHIPINFO_MODEM_LTE },
};


static const ChipInfoArchFamilyDataType ChipInfoArchFamilyArray[] =
{
  { CHIPINFO_ARCH_FAMILY_UNKNOWN,     0,  CHIPINFO_FAMILY_UNKNOWN,  CHIPINFO_ID_UNKNOWN },

  { CHIPINFO_ARCH_FAMILY_HOYA,        0,  CHIPINFO_FAMILY_SDM845,   CHIPINFO_ID_SDM845  },
  { CHIPINFO_ARCH_FAMILY_HOYA,        1,  CHIPINFO_FAMILY_SDM830,   CHIPINFO_ID_SDM830  },
  { CHIPINFO_ARCH_FAMILY_HOYA,        2,  CHIPINFO_FAMILY_SDX24,    CHIPINFO_ID_SDX24   },
  { CHIPINFO_ARCH_FAMILY_HOYA,        3,  CHIPINFO_FAMILY_SDM855,   CHIPINFO_ID_SDM855  },
  { CHIPINFO_ARCH_FAMILY_HOYA,        4,  CHIPINFO_FAMILY_SDM670,   CHIPINFO_ID_SDM670  },
  { CHIPINFO_ARCH_FAMILY_HOYA,        5,  CHIPINFO_FAMILY_QCS605,   CHIPINFO_ID_QCS605  },
  { CHIPINFO_ARCH_FAMILY_HOYA,        6,  CHIPINFO_FAMILY_SDM1000,  CHIPINFO_ID_SDM1000 },
};

static ChipInfoBSPType ChipInfoBSP =
{
  .aLUT = ChipInfoDataLUT,
  .nNumLUTEntries = ARRAY_SIZE(ChipInfoDataLUT),
  .aArchFamilyData = ChipInfoArchFamilyArray,
  .nNumArchFamilyDataEntries = ARRAY_SIZE(ChipInfoArchFamilyArray),
};

/*==============================================================================
      Functions
==============================================================================*/

ChipInfoBSPType *ChipInfo_GetBSPData
(
  void
)
{
  return &ChipInfoBSP;
}
