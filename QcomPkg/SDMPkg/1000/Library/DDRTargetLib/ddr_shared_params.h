#ifndef __DDR_SHARED_PARAMS_H__
#define __DDR_SHARED_PARAMS_H__

/*=============================================================================
                              DDR HAL
                            Header File
GENERAL DESCRIPTION
This is the target header file for shared DDR params such as those shared with clock driver and AOP.

Copyright 2014-2018 by Qualcomm Technologies, Inc.  All Rights Reserved.

===========================================================================

                            EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.


when       who     what, where, why
--------   ---     ------------------------------------------------------------
06/04/18   daisond PASR table share with AOP
05/24/17   rp      Initial revision.
=============================================================================*/
/*==============================================================================
                                  INCLUDES
==============================================================================*/
#include "ddr_common.h"

typedef struct
{
  uint16 size;
  uint16 offset;
}shared_table_header_type;

typedef struct
{
  uint64 ddr_cs0_addr;
  uint64 ddr_cs1_addr;
  uint64 ddr_cs0_remapped_addr;
  uint64 ddr_cs1_remapped_addr;
  uint32 num_channels;
  uint8  per_ch_density_rank0;
  uint8  per_ch_density_rank1;	
  uint8  reserved1;
  uint8  reserved2;
}pasr_shared_ddr_info;

typedef enum
{
  MC_STATES = 0,
  SHUB_STATES,
  DEFAULT_ALC_POLICY,
  ALC_TIMINGS,
  PASR_DDR_INFO_TABLE,
  ALC_POLICY_INDEX,
  MAX_NUM_TABLES,
}smem_tables;

typedef struct
{
  uint16 major_version;
  uint16 minor_version;
  shared_table_header_type shared_table_header[MAX_NUM_TABLES];
}ddr_smem_info;

/*==============================================================================
                                  MACROS
==============================================================================*/
#define AOP_PARAMS_MAJOR_VERSION 0x1
#define AOP_PARAMS_MINOR_VERSION 0x2

/* With the new SMEM architecture, SMEM IDs need to be defined in individual driver files */
#define DDR_AOP_SMEM_ID 0x25C   //604

#endif /* __DDR_SHARED_PARAMS_H__ */
