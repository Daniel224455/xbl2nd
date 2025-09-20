/*===============================================================================
  Copyright (c) 2018 Qualcomm Technologies, Inc.  All Rights Reserved.
  
  FILE:         pil_ctx.h
  DESCRIPTION:    
  
  REVISION HISTORY
  when       who         what, where, why
  --------   ---         --------------------------------------------------------
  03/12/18   yw         initial version

================================================================================*/

#ifndef __PIL_CTX_H__
#define __PIL_CTX_H__

#include<elf.h>
#include<pil_config.h>
#include<pil_log.h>
#include<Library/BaseLib.h>

#define PIL_SUBSYS_MAX_NUM        32
#define SMC_MAX_CNT               10

typedef enum {
    PIL_PROC_DOWN,
    PIL_PROC_RUNNING,
} PIL_PROC_STATUS;

typedef enum {
    PIL_MEM_RESV = 1,
    PIL_MEM_ALLOC,
    PIL_MEM_ALLOC_FV,
} PIL_MEMORY_ALLOC_TYPE;

typedef struct _SMC_BENCHMARK_TYPE_
{
    UINT32 SmcId;
    UINT32 Timeus;
} SMC_BENCHMARK_TYPE;

typedef struct _PIL_ELF_LOAD_INFO_TYPE_
{
    VOID*    ElfBase;               /* Elf base address if entire elf loaded into memory */
    UINT32  ElfSize;               /* Elf size if entire elf loaded into memory */
    VOID*   SegBase;               /* base address of loadable segments */
    UINT32  SegSize;               /* size of loadable segments */
} PIL_ELF_LOAD_INFO_TYPE;

typedef struct _PIL_SUBSYS_PRIV_TYPE_
{
    UINTN    RegionStart;                  /* actual start address of load region */
    UINTN   RegionSize;                   /* actual size of the load region */
    UINTN   BaseAddr;                     /* load address in ELF */
    Elf32_Phdr* Phdr;                    /* program headers*/
    Elf32_Ehdr* Ehdr;                      /* ELF header */
    PIL_MEMORY_ALLOC_TYPE AllocType;      /* how the load region is allocated */
    PIL_PROC_STATUS ProcStatus;           /* status of subsys */
    UINT8*    MdtBuf;                     /* buffer of meta data */
    PIL_ELF_LOAD_INFO_TYPE ElfLoadInfo;     /* info of FV loading */
    UINT32  SmcCnt;                       /* smc count saved in smc benchmark array */
    SMC_BENCHMARK_TYPE  SmcBenchMarkus[SMC_MAX_CNT];    /* array of SMC benchmark  */
} PIL_SUBSYS_PRIV_TYPE;


typedef struct _PIL_SUBSYS_CTX_TYPE_
{
    PIL_SUBSYS_CFG_TYPE*    Cfg;
    PIL_SUBSYS_PRIV_TYPE*     Priv;
} PIL_SUBSYS_CTX_TYPE;

PIL_STATUS PilInitSubsys(PIL_SUBSYS_CFG_TYPE*, PIL_SUBSYS_CTX_TYPE**);

#endif
