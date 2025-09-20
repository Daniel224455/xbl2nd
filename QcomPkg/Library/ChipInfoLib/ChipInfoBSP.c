/*==============================================================================
  @file ChipInfoBSP.c

  ChipInfo BSP data.

================================================================================
        Copyright (c) 2016-2018 Qualcomm Technologies, Inc.
               All Rights Reserved.
            QUALCOMM Proprietary/GTDR
================================================================================


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
//#define ARRAY_SIZE(arr) (sizeof(arr)/sizeof(arr[0]))

/*==============================================================================
      Data Definitons
==============================================================================*/

/*
 * ChipInfoDataLUT
 *
 * Array of chip id strings, chip IDs, part numbers, and modem support.
 *
 * Various HLOS clients still expect to see the old naming convention for
 * Hana variants. Continue to report the legacy names for Hana.
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
  { "SDM855",     CHIPINFO_ID_SM8150,        CHIPINFO_PARTNUM_SM8150,       CHIPINFO_MODEM_WWAN | CHIPINFO_MODEM_CGPS | CHIPINFO_MODEM_LTE },
  { "SC8180X",    CHIPINFO_ID_SDM1000,       CHIPINFO_PARTNUM_SDM1000,      CHIPINFO_MODEM_WWAN | CHIPINFO_MODEM_CGPS | CHIPINFO_MODEM_LTE },
  { "SDA845",     CHIPINFO_ID_SDA845,        CHIPINFO_PARTNUM_SDA845,       CHIPINFO_MODEM_WWAN | CHIPINFO_MODEM_CGPS | CHIPINFO_MODEM_LTE },
  { "QCS605",     CHIPINFO_ID_QCS605,        CHIPINFO_PARTNUM_QCS605,       CHIPINFO_MODEM_WWAN | CHIPINFO_MODEM_CGPS | CHIPINFO_MODEM_LTE },
  { "SM6150",     CHIPINFO_ID_SM6150,        CHIPINFO_PARTNUM_SM6150,       CHIPINFO_MODEM_WWAN | CHIPINFO_MODEM_CGPS | CHIPINFO_MODEM_LTE },
  { "SDA865",     CHIPINFO_ID_SDA865,        CHIPINFO_PARTNUM_SDA865,       0 },
  { "SDX55",      CHIPINFO_ID_SDX55,         CHIPINFO_PARTNUM_SDX55,        CHIPINFO_MODEM_WWAN | CHIPINFO_MODEM_CGPS | CHIPINFO_MODEM_LTE },
  { "QCA6390",    CHIPINFO_ID_QCA6390,       CHIPINFO_PARTNUM_QCA6390,      CHIPINFO_MODEM_WWAN | CHIPINFO_MODEM_CGPS | CHIPINFO_MODEM_LTE },
  { "SDA855" ,    CHIPINFO_ID_SM8150P,       CHIPINFO_PARTNUM_SM8150P,      0 },
  { "SDM855A",    CHIPINFO_ID_SA8150,        CHIPINFO_PARTNUM_SDM855A,      CHIPINFO_MODEM_WWAN | CHIPINFO_MODEM_CGPS | CHIPINFO_MODEM_LTE }, // Same chip as SA8150, despite different JTAG IDs.
  { "SA8150",     CHIPINFO_ID_SA8150,        CHIPINFO_PARTNUM_SA8150,       CHIPINFO_MODEM_WWAN | CHIPINFO_MODEM_CGPS | CHIPINFO_MODEM_LTE },
  { "SM7150",     CHIPINFO_ID_SM7150,        CHIPINFO_PARTNUM_SM7150,       CHIPINFO_MODEM_WWAN | CHIPINFO_MODEM_CGPS | CHIPINFO_MODEM_LTE },
  { "SM7150P",    CHIPINFO_ID_SM7150P,       CHIPINFO_PARTNUM_SM7150P,      0 },
  { "SDA855A",    CHIPINFO_ID_SA8150P,       CHIPINFO_PARTNUM_SA8150P,      0 },
  { "SM6150P",    CHIPINFO_ID_SM6150P,       CHIPINFO_PARTNUM_SM6150P,      0 },
  { "SC8180XP",   CHIPINFO_ID_SC8180XP,      CHIPINFO_PARTNUM_SC8180XP,     0 },
  { "SA8195P",    CHIPINFO_ID_SA8195P,       CHIPINFO_PARTNUM_SA8195P,      0 },
};


static const ChipInfoArchFamilyDataType ChipInfoArchFamilyArray[] =
{
  { CHIPINFO_ARCH_FAMILY_UNKNOWN,     0,  CHIPINFO_FAMILY_UNKNOWN,  CHIPINFO_ID_UNKNOWN },

  { CHIPINFO_ARCH_FAMILY_QCA,         10, CHIPINFO_FAMILY_QCA639x,  CHIPINFO_ID_QCA6390 },

  { CHIPINFO_ARCH_FAMILY_HOYA,        0,  CHIPINFO_FAMILY_SDM845,   CHIPINFO_ID_SDM845  },
  { CHIPINFO_ARCH_FAMILY_HOYA,        1,  CHIPINFO_FAMILY_SDM830,   CHIPINFO_ID_SDM830  },
  { CHIPINFO_ARCH_FAMILY_HOYA,        2,  CHIPINFO_FAMILY_SDX24,    CHIPINFO_ID_SDX24   },
  { CHIPINFO_ARCH_FAMILY_HOYA,        3,  CHIPINFO_FAMILY_SM8150,   CHIPINFO_ID_SM8150  },
  { CHIPINFO_ARCH_FAMILY_HOYA,        4,  CHIPINFO_FAMILY_SDM670,   CHIPINFO_ID_SDM670  },
  { CHIPINFO_ARCH_FAMILY_HOYA,        5,  CHIPINFO_FAMILY_QCS605,   CHIPINFO_ID_QCS605  },
  { CHIPINFO_ARCH_FAMILY_HOYA,        6,  CHIPINFO_FAMILY_SDM1000,  CHIPINFO_ID_SDM1000 },
  { CHIPINFO_ARCH_FAMILY_HOYA,        7,  CHIPINFO_FAMILY_SM6150,   CHIPINFO_ID_SM6150  },
  { CHIPINFO_ARCH_FAMILY_HOYA,        8,  CHIPINFO_FAMILY_SDA865,   CHIPINFO_ID_SDA865  },
  { CHIPINFO_ARCH_FAMILY_HOYA,        11, CHIPINFO_FAMILY_SDX55,    CHIPINFO_ID_SDX55   },
  { CHIPINFO_ARCH_FAMILY_HOYA,        12, CHIPINFO_FAMILY_SM7150,   CHIPINFO_ID_SM7150  },
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
