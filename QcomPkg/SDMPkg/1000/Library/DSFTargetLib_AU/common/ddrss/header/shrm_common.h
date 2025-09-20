/*******************************************************************************
 QUALCOMM Proprietary Design Data
 Copyright (c) 2017-2018, Qualcomm Technologies Incorporated.
 All rights reserved.
 ******************************************************************************/
/*==============================================================================
================================================================================*/
#ifndef __SHRM_COMMON_H__
#define __SHRM_COMMON_H__

// Offset inside SHRM DRAM for SHRM dump region
#define SHRM_DUMP_OFFSET 0x5100

// Size of SHRM dump region
#define SHRM_DUMP_SIZE 0x1200 

// Absolute address to SHRM dump region
#ifdef DSF_COMPILING_SHRM
    #define SHRM_DUMP_ADDR (0x20000 + SHRM_DUMP_OFFSET) // for SHRM
#else
    #define SHRM_DUMP_ADDR (SEQ_DDR_SS_SHRM_MEM_SHRM_OFFSET + SHRM_DUMP_OFFSET) // for APPS
#endif

// Offset inside SHRM DRAM for SHRM MR dump region
#define SHRM_MR_DUMP_OFFSET 0x6C00

// Absolute address to SHRM MR dump region
#ifdef DSF_COMPILING_SHRM
    #define SHRM_MR_DUMP_ADDR (0x20000 + SHRM_MR_DUMP_OFFSET) // for SHRM
#else
    #define SHRM_MR_DUMP_ADDR (SEQ_DDR_SS_SHRM_MEM_SHRM_OFFSET + SHRM_MR_DUMP_OFFSET) // for APPS
#endif



// Offsets to compressed address lists and dumps in SHRM dump region
typedef struct
{
    uint16 addr_list;
    uint16 wdog_dump;
    uint16 mccc_addr_list;
    uint16 mccc_dump;
}
SHRM_DUMP_OFFSETS;

// Pointer to SHRM_DUMP_OFFSETS struct in SHRM dump region
#define SHRM_DUMP_OFFSETS_PTR ((SHRM_DUMP_OFFSETS*)SHRM_DUMP_ADDR)

#endif // __SHRM_COMMON_H__
