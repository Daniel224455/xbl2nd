/*===========================================================================

          Shared Memory MSM (2MB SMEM) Device Configuration Structures

=============================================================================

  @file
    smem_partitions.c

    Contains structures to be used in SMEM Device Configuration.

  Copyright (c) 2013-2016 Qualcomm Technologies, Inc. 
  All rights reserved.
  Qualcomm Confidential and Proprietary
===========================================================================*/

/*===========================================================================

                           EDIT HISTORY FOR FILE


when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/14/16   vr      SMEM Version 0x000C0000 changes,
                   Added APPS-CDSP partition.
07/26/16   vr      Added MPSS-HYP partition.
08/26/15   an      Added SPSS partitions.
06/07/15   rs      Moved MPSS partitions next to legacy one to reduce XPU usage
04/07/15   rs      + Ordering partitions such that Modems are stacked together 
                     to miinize XPU usage
                   + Added support for legacy partition protections
09/05/14   sm      Compiler warnings fixes.
11/01/13   bt      Enable partitions on Apps edges.
10/17/13   bt      Re-enable non-Apps SMEM partitions.
09/19/13   bt      Enable partitions on non-Apps edges.
05/16/13   pa      Enabling this to be part of packed builds.
08/22/13   bt      Initial revision.  All partitions disabled (size = 0).
===========================================================================*/

/*===========================================================================
                        INCLUDE FILES
===========================================================================*/
#include "Library/SmemLib/inc/smem_toc.h"

/*===========================================================================
                        DATA DECLARATIONS
===========================================================================*/
const smem_toc_config_type smem_toc_partition_entries[] =
{
  /* Command DB partition. */
  {
    128 * 1024,
    SMEM_TOC_ENTRY_FLAGS_IGNORE_PARTITION,
    SMEM_CMDDB,
    SMEM_CMDDB,
    32,
    {0, 0, 0, 0}
  },
  
  /* Apps<->ADSP partition. */
  {
    192 * 1024,
    SMEM_TOC_ENTRY_FLAGS_ENABLE_RW_PROTECTION,
    SMEM_APPS,
    SMEM_ADSP,
    32,
    {0, 0, 0, 0}
  },
  
  /* Apps<->CDSP partition. */
  {
    128 * 1024,
    SMEM_TOC_ENTRY_FLAGS_ENABLE_RW_PROTECTION,
    SMEM_APPS,
    SMEM_CDSP,
    32,
    {0, 0, 0, 0}
  },
  
  /* Apps<->Sensors partition. */
  {
    192 * 1024,
    SMEM_TOC_ENTRY_FLAGS_ENABLE_RW_PROTECTION,
    SMEM_APPS,
    SMEM_SSC,
    32,
    {0, 0, 0, 0}
  },
  
  /* ADSP<->Sensors partition. */
  {
    48 * 1024,
    SMEM_TOC_ENTRY_FLAGS_ENABLE_RW_PROTECTION,
    SMEM_ADSP,
    SMEM_SSC,
    32,
    {0, 0, 0, 0}
  },

  /* Apps<->SP partition. */
  {
    16 * 1024,
    SMEM_TOC_ENTRY_FLAGS_ENABLE_RW_PROTECTION,
    SMEM_APPS,
    SMEM_SPSS,
    32,
    {0, 0, 0, 0}
  },

  /* TZ<->SP partition. */
  {
    16 * 1024,
    SMEM_TOC_ENTRY_FLAGS_ENABLE_RW_PROTECTION,
    SMEM_TZ,
    SMEM_SPSS,
    32,
    {0, 0, 0, 0}
  },
  
  /* TZ<->Modem partition. */
  {
    8 * 1024,   /* Size of the partition  (must be 4kB multiple). */
    SMEM_TOC_ENTRY_FLAGS_ENABLE_RW_PROTECTION, /* flags */
    SMEM_TZ,    /* proc0 with RW access to the partition. */
    SMEM_MODEM,   /* proc1 with RW access to the partition. */
    32,           /* The cacheline common multiple, in Bytes. */
    {0, 0, 0, 0}  /* Exclusion regions' sizes within this partition. */
  },    

  /* Modem<->SP partition. */
  {
    16 * 1024,
    SMEM_TOC_ENTRY_FLAGS_ENABLE_RW_PROTECTION,
    SMEM_MODEM,
    SMEM_SPSS,
    32,
    {0, 0, 0, 0}
  },
  
  /* Modem<->CDSP partition. */
  {
    64 * 1024,
    SMEM_TOC_ENTRY_FLAGS_ENABLE_RW_PROTECTION,
    SMEM_MODEM,
    SMEM_CDSP,
    32,
    {0, 0, 0, 0}
  },    

  /* Apps<->Modem partition. */
  {
    496 * 1024,   /* Size of the partition  (must be 4kB multiple). */
    SMEM_TOC_ENTRY_FLAGS_ENABLE_RW_PROTECTION, /* flags */
    SMEM_APPS,    /* proc0 with RW access to the partition. */
    SMEM_MODEM,   /* proc1 with RW access to the partition. */
    32,           /* The cacheline common multiple, in Bytes. */
    {0, 0, 0, 0}  /* Exclusion regions' sizes within this partition. */
  },

  /* Modem<->ADSP partition. */
  {
    128 * 1024,
    SMEM_TOC_ENTRY_FLAGS_ENABLE_RW_PROTECTION,
    SMEM_MODEM,
    SMEM_ADSP,
    32,
    {0, 0, 0, 0}
  },

  /* Modem<->Sensor partition. */
  {
    64 * 1024,
    SMEM_TOC_ENTRY_FLAGS_ENABLE_RW_PROTECTION,
    SMEM_MODEM,
    SMEM_SSC,
    32,
    {0, 0, 0, 0}
  },

  /* Last entry uses invalid hosts and no protections to signify the end. */
  {
    0,
    0,
    SMEM_INVALID_HOST,
    SMEM_INVALID_HOST,
    0,
    {0, 0, 0, 0}
  }
};
