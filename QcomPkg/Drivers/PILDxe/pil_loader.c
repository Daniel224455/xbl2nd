/*===============================================================================
  Copyright (c) 2017-2018 Qualcomm Technologies, Inc.  All Rights Reserved.
  
  FILE:         pil_loader.c
  DESCRIPTION:    
  
  REVISION HISTORY
  when       who         what, where, why
  --------   ---         --------------------------------------------------------
  06/14/17   yw         initial version

================================================================================*/

#include<pil_log.h>
#include<pil_loader.h>
#include<pil_config.h>
#include<Protocol/EFIScm.h>
#include<Library/UefiLib.h>
#include<Library/QcomLib.h>
#include<Library/TimerLib.h>
#include<Library/PrintLib.h>
#include<Library/QcomBaseLib.h>
#include<Library/GuidedFvLib.h>
#include<Library/BaseMemoryLib.h>
#include<Library/DxeServicesTableLib.h>
#include<Library/MemoryAllocationLib.h>
#include<Library/UefiBootServicesTableLib.h>
#include<Library/UncachedMemoryAllocationLib.h>

#include "LoaderUtils.h"

#define BIT(x)                 (1 << (x))
#define ALIGN_UP_SZ(x,y)    (((x) + ((y) - 1)) & ~((y) - 1))
#define ALIGN_DOWN_SZ(x,y)    ((x) & ~((y) - 1))

#define MASKOS_IS_SEGMENT_TYPE(flag, val)     (((flag) & (0x7 << 24)) == (val << 24))
#define MASKOS_SEGMENT_TYPE_VALUE_HASH        2

#define ELF_P_FLAGS_MASK (0x0F << 24)
#define ELF_P_FLAGS_HASH (0x02 << 24)
#define ELF_P_FLAGS_LOAD (0x08 << 24)

/* base address of image load info region in IMEM */
extern UINTN ImageLoadInfoBase;
/* size of image load info region in IMEM*/
extern UINT32 ImageLoadInfoSize;

extern UINT64 StorageSpentCnt, FvProcSpentCnt;

STATIC UINTN ConvertToVirtualAddress(UINTN paddr)
{
    return paddr;
}
/**
  validate ELF header

  @param[in] Ctx    ptr to private data of PIL, used for logging  
  @param[in] pEhdr    prt to ELF header to be validate
  
  @retval PIL_SUCCESS       validate successfully
  @retval other             invalid ELF header.

**/

STATIC PIL_STATUS PilValidateElfHeader(PIL_SUBSYS_CTX_TYPE* Ctx, Elf32_Ehdr* pEhdr)
{
    PIL_SUBSYS_CFG_TYPE* Cfg;
    PIL_SUBSYS_PRIV_TYPE* Priv;
    ASSERT(pEhdr != NULL);
    ASSERT_FAIL_PIL_RETURN(pEhdr != NULL, PIL_INVALID_PARAM, PIL_INVALID_PARAM);
    ASSERT(Ctx != NULL);
    ASSERT_FAIL_PIL_RETURN(Ctx != NULL, PIL_INVALID_PARAM, PIL_INVALID_PARAM);
    ASSERT((Cfg = Ctx->Cfg) != NULL);
    ASSERT_FAIL_PIL_RETURN(Cfg != NULL, PIL_INVALID_PARAM, PIL_INVALID_PARAM);
    ASSERT((Priv = Ctx->Priv) != NULL);
    ASSERT_FAIL_PIL_RETURN(Priv != NULL, PIL_INVALID_PARAM, PIL_INVALID_PARAM);
    
    if (CompareMem(pEhdr->e_ident, ELFMAG, SELFMAG)) {
        PIL_ERROR_CFG(Cfg, "Not an elf header\n");
        return PIL_INVALID_PARAM;
    }
    
    if (pEhdr->e_ident[EI_CLASS] != ELFCLASS32) {
        PIL_ERROR_CFG(Cfg, "Not 32bit ELF\n");
        return PIL_INVALID_PARAM;
    }
    
    if (pEhdr->e_ident[EI_DATA] != ELFDATA2LSB) {
        PIL_ERROR_CFG(Cfg, "image is not Little Endian type\n");
        return PIL_INVALID_PARAM;
    }
    
    if (pEhdr->e_ident[EI_VERSION] != EV_CURRENT) {
        PIL_ERROR_CFG(Cfg, "invalid ELF version\n");
        return PIL_INVALID_PARAM;
    }
    
    if (pEhdr->e_type != ET_EXEC) {
        PIL_ERROR_CFG(Cfg, "err: Not valide ELF type\n");
        return PIL_INVALID_PARAM;
    }

    if (pEhdr->e_phnum == 0) {
        PIL_ERROR_CFG(Cfg, "No loadable segments\n");
        return PIL_INVALID_PARAM;
    }
    
    return PIL_SUCCESS;
}


STATIC BOOLEAN SegmentIsRelocatable(Elf32_Phdr *p)
{
    return !!(p->p_flags & BIT(27));
}

/**
  validate single program header

  @param[in] Ctx    ptr to private data of PIL, used for logging  
  @param[in] pPhdr    prt to program header to be validate
  
  @retval PIL_SUCCESS       validate successfully
  @retval other             invalid ELF header.

**/

STATIC PIL_STATUS PilValidatePhdr(PIL_SUBSYS_CTX_TYPE* Ctx, Elf32_Phdr* pPhdr)
{
    PIL_SUBSYS_CFG_TYPE* Cfg;
    ASSERT(pPhdr != NULL);
    ASSERT_FAIL_PIL_RETURN(pPhdr != NULL, PIL_INVALID_PARAM, PIL_INVALID_PARAM);
    ASSERT(Ctx != NULL);
    ASSERT_FAIL_PIL_RETURN(Ctx != NULL, PIL_INVALID_PARAM, PIL_INVALID_PARAM);
    ASSERT((Cfg = Ctx->Cfg) != NULL);
    ASSERT_FAIL_PIL_RETURN(Cfg != NULL, PIL_INVALID_PARAM, PIL_INVALID_PARAM);

    PIL_INFO_CFG(Cfg, "type 0x%x off 0x%x vaddr 0x%x paddr 0x%x filesz 0x%x memsz 0x%x flg 0x%x ali 0x%x  ", \
                 pPhdr->p_type, pPhdr->p_offset, pPhdr->p_vaddr, pPhdr->p_paddr, \
                 pPhdr->p_filesz, pPhdr->p_memsz, pPhdr->p_flags, pPhdr->p_align);
    if (SegmentIsRelocatable(pPhdr)) {
        PIL_INFO("reloc\n");
    }
    else
        PIL_INFO("non-reloc\n");
    
    if (pPhdr->p_type == PT_NULL)
        return PIL_SUCCESS;

#if 0    
    // TODO also need to check if it's relocateable
    if (pPhdr->p_paddr == 0 && pPhdr->p_memsz > 0) {
        PIL_ERROR_CFG(Cfg, "err: pAddr of Phdr is 0 but elf header indicates that this image is fixed\n");
        return PIL_INVALID_PARAM;
    }
#endif
    
    if (pPhdr->p_memsz < pPhdr->p_filesz) {
        PIL_ERROR_CFG(Cfg, "err: memsz less than filesz\n");
        return PIL_INVALID_PARAM;
    }
    
    if (pPhdr->p_type != PT_DYNAMIC &&
        pPhdr->p_type != PT_LOAD &&
        pPhdr->p_type != PT_NULL) {
        PIL_ERROR_CFG(Cfg, "err: invalid program header type\n");
        return PIL_INVALID_PARAM;
    }
    
    if (pPhdr->p_filesz > pPhdr->p_memsz) {
        PIL_ERROR_CFG(Cfg, "err: program header filesz greater than memsz\n");
        return PIL_INVALID_PARAM;
    }
    return PIL_SUCCESS;
}

/**
  validate metadata

  @param[in] Ctx    ptr to private data of PIL, used for logging  
  @param[in] fwbuf     start address of metadata
  
  @retval PIL_SUCCESS       validate successfully
  @retval other             invalid ELF header.

**/

PIL_STATUS PilValidateHdr(PIL_SUBSYS_CTX_TYPE* Ctx, UINT8* fwbuf) 
{
    Elf32_Ehdr *Ehdr;
    Elf32_Phdr *Phdr;
    PIL_SUBSYS_CFG_TYPE* Cfg;
    PIL_SUBSYS_PRIV_TYPE* Priv;
    UINT32         i, PhdrCnt;
    PIL_STATUS        result;
    
    Ehdr = (Elf32_Ehdr*)fwbuf;
    ASSERT(Ehdr != NULL);
    ASSERT_FAIL_PIL_RETURN(Ehdr != NULL, PIL_INVALID_PARAM, PIL_INVALID_PARAM);
    ASSERT(Ctx != NULL);
    ASSERT_FAIL_PIL_RETURN(Ctx != NULL, PIL_INVALID_PARAM, PIL_INVALID_PARAM);
    ASSERT((Cfg = Ctx->Cfg) != NULL);
    ASSERT_FAIL_PIL_RETURN(Cfg != NULL, PIL_INVALID_PARAM, PIL_INVALID_PARAM);
    ASSERT((Priv = Ctx->Priv) != NULL);
    ASSERT_FAIL_PIL_RETURN(Priv != NULL, PIL_INVALID_PARAM, PIL_INVALID_PARAM);
    
    result = PilValidateElfHeader(Ctx, Ehdr);
    if (result != PIL_SUCCESS) {
        PIL_ERROR_CFG(Cfg, "Elf header validation failed\n");
        return result;
    }
    
    // save Ehdr to private data structure
    Priv->Ehdr = AllocateZeroPool(sizeof(Elf32_Ehdr));
    if (Priv->Ehdr == NULL) {
        PIL_ERROR_CFG(Cfg, "Failed to alloc buffer to save Ehdr\n");
        return PIL_OUT_OF_MEMORY;
    }
    CopyMem(Priv->Ehdr, Ehdr, sizeof(Elf32_Ehdr));
    
    Phdr = (Elf32_Phdr*)((uint8*)fwbuf + Ehdr->e_phoff);
    ASSERT(Phdr != NULL);
    ASSERT_FAIL_PIL_RETURN(Phdr != NULL, PIL_INVALID_PARAM, PIL_INVALID_PARAM);
    PhdrCnt = Ehdr->e_phnum;
    
    for (i = 0; i < PhdrCnt; ++i)
    {
        result = PilValidatePhdr (Ctx, &Phdr[i]);
        if (result != PIL_SUCCESS) {
            PIL_ERROR_CFG(Cfg, "Elf program header %d validation failed\n", i);
            return result;
        }
    }
    
    /* save the phdr in private data */
    Priv->Phdr = AllocateZeroPool(sizeof(Elf32_Phdr)*PhdrCnt);
    if (NULL==Priv->Phdr) {
        PIL_ERROR_CFG(Cfg, "Failed to alloc phdr buffer in private data\n");
        return PIL_OUT_OF_MEMORY;
    }
    CopyMem(Priv->Phdr, Phdr, sizeof(Elf32_Phdr)*PhdrCnt);
    
    return PIL_SUCCESS;
}

/**
  send SMC to secure PIL  
     
  @param[in] ScmId           SMC ID, identify the function of the SMC
  @param[in] ParamId       identify the parameter request of the SMC
  
  @retval EFI_SUCCESS       success
  @retval other             failurel

**/
EFI_STATUS PilScmStdCall(PIL_SUBSYS_CTX_TYPE* Ctx, UINT32 ScmId, UINT32 ParamId,...)
{
    UINT32 ArgsNum = TZ_SYSCALL_NUM_ARGS(ParamId);
    UINT64 Param[SCM_MAX_NUM_PARAMETERS] = {0};
    UINT64 Results[SCM_MAX_NUM_RESULTS] = {0};
    UINT32 ScmIdOut, i;
    VA_LIST Ap;
    EFI_STATUS Status;
    UINT32 SmcStart, SmcEnd;
    PIL_SUBSYS_PRIV_TYPE* Priv;
    PIL_SUBSYS_CFG_TYPE* Cfg;
    QCOM_SCM_PROTOCOL *pScmProtocol = 0;
    
    ASSERT(Ctx != NULL);
    ASSERT_FAIL_PIL_RETURN(Ctx != NULL, PIL_INVALID_PARAM, PIL_INVALID_PARAM);
    ASSERT((Priv = Ctx->Priv) != NULL);
    ASSERT_FAIL_PIL_RETURN(Priv != NULL, PIL_INVALID_PARAM, PIL_INVALID_PARAM);
    ASSERT((Cfg = Ctx->Cfg) != NULL);
    ASSERT_FAIL_PIL_RETURN(Cfg != NULL, PIL_INVALID_PARAM, PIL_INVALID_PARAM);
    
    ScmIdOut = ScmId | (0x1ULL<<30); // standard SMC with ARM_SMC_SMC64

    if (ArgsNum > 0){
      VA_START(Ap, ParamId);
      for (i = 0; i < ArgsNum; i++){
         Param[i] = VA_ARG(Ap, UINTN);
      }
      VA_END(Ap);
    }
    
    Status = gBS->LocateProtocol (&gQcomScmProtocolGuid, NULL, (VOID**)&pScmProtocol); 
    if (EFI_ERROR(Status)) return Status;
    
    SmcStart = GetTimerCountus();
    Status = pScmProtocol->ScmSipSysCall(pScmProtocol, ScmIdOut, \
                                            ParamId, Param, Results);
    SmcEnd = GetTimerCountus();
    
    // save smc benchmark in the arry
    if (Priv->SmcCnt < SMC_MAX_CNT) {
        Priv->SmcBenchMarkus[Priv->SmcCnt].SmcId = ScmId;
        Priv->SmcBenchMarkus[Priv->SmcCnt].Timeus = SmcEnd - SmcStart;
        Priv->SmcCnt++;
    }
    
    return Status;
}


/**
  validate metadata from the ELF, inlcuding ELF header, program headers, 
  and hash segment.
     
  @param[in] Ctx    ptr to ctx data of PIL  
  
  @retval PIL_SUCCESS       success
  @retval other             failurel

**/
PIL_STATUS PilValidateMetadata(PIL_SUBSYS_CTX_TYPE* Ctx)
{
    PIL_SUBSYS_CFG_TYPE* Cfg;
    PIL_SUBSYS_PRIV_TYPE* Priv;
    EFI_STATUS        Status;
    PIL_STATUS        PilRet;
    
    ASSERT(Ctx != NULL);
    ASSERT_FAIL_PIL_RETURN(Ctx != NULL, PIL_INVALID_PARAM, PIL_INVALID_PARAM);
    ASSERT((Cfg = Ctx->Cfg) != NULL);
    ASSERT_FAIL_PIL_RETURN(Cfg != NULL, PIL_INVALID_PARAM, PIL_INVALID_PARAM);
    ASSERT((Priv = Ctx->Priv) != NULL);
    ASSERT_FAIL_PIL_RETURN(Priv != NULL, PIL_INVALID_PARAM, PIL_INVALID_PARAM);
    
    PilRet = PilValidateHdr(Ctx, Priv->MdtBuf);
    if (PIL_SUCCESS != PilRet) {
        PIL_ERROR_CFG(Cfg, "Failed to validate ELF header\n");
        return PilRet;
    }
    PIL_INFO_CFG(Cfg, "ELF header validation return OK\n");
    
    /* TODO, it the subsys with same id already start, rely on tz to return "started" err
       either remove private data or keep it with "not running" status
    */
    Status = PilScmStdCall(Ctx, TZ_PIL_INIT_ID, TZ_PIL_INIT_ID_PARAM_ID, Cfg->SubsysID, ConvertToPhysicalAddress(Priv->MdtBuf));
    if (EFI_ERROR(Status)) {
        PIL_ERROR_CFG(Cfg, "failed to validate firmware metadata return %r\n", Status);
        return PIL_SMC_ERR;
    }
    PIL_INFO_CFG(Cfg, "Successfully validate firmware metadata\n");
    
    return PIL_SUCCESS;
}



STATIC INTN SegmentIsLoadable(Elf32_Phdr *p)
{
    return (p->p_type == PT_LOAD) && !MASKOS_IS_SEGMENT_TYPE(p->p_flags, MASKOS_SEGMENT_TYPE_VALUE_HASH) && p->p_memsz;
}


#define ULLONG_MAX      (~0ULL)
#define MAX_VALUE(x,y)    ((x)<=(y))?(y):(x)
#define MIN_VALUE(x,y)    ((x)<=(y))?(x):(y)


/**
  calculate the memoey range to load image 

  @param[in] Ctx    ptr to ctx data of PIL   
  
  @retval PIL_SUCCESS       success
  @retval other             failure

**/

PIL_STATUS PilSetupMemoryRange(PIL_SUBSYS_CTX_TYPE* Ctx)
{
    Elf32_Ehdr *Ehdr;
    Elf32_Phdr *Phdr;
    UINT32         i, PhdrCnt, Align=0;
    UINTN         MemMin, MemMax;
    UINTN       MemMinAligned, MemMaxAligned;
    UINTN       AlignedSize;
    VOID*        StartVaddr;
    UINT32       LoadableSegOff,LoadableSegSize;
    BOOLEAN     Reloc = FALSE;
    PIL_SUBSYS_PRIV_TYPE* Priv;
    PIL_SUBSYS_CFG_TYPE*  Cfg;
    EFI_STATUS   Status;
    
    ASSERT(Ctx != NULL);
    ASSERT_FAIL_PIL_RETURN(Ctx != NULL, PIL_INVALID_PARAM, PIL_INVALID_PARAM);
    Cfg  = Ctx->Cfg;
    Priv = Ctx->Priv;
    ASSERT(Cfg != NULL && Priv != NULL);
    ASSERT_FAIL_PIL_RETURN(Cfg != NULL && Priv != NULL, PIL_INVALID_PARAM,PIL_INVALID_PARAM);
    
    Ehdr = (Elf32_Ehdr*)(Priv->MdtBuf);
    ASSERT(Ehdr != NULL);
    ASSERT_FAIL_PIL_RETURN(Ehdr != NULL, PIL_INVALID_PARAM, PIL_INVALID_PARAM);
    Phdr = (Elf32_Phdr*)((uint8*)Ehdr + Ehdr->e_phoff);
    ASSERT(Phdr != NULL);
    ASSERT_FAIL_PIL_RETURN(Phdr != NULL, PIL_INVALID_PARAM, PIL_INVALID_PARAM);

    PhdrCnt = Ehdr->e_phnum;
    MemMin = (UINT64)ULLONG_MAX;
    MemMax = 0;
    LoadableSegOff = (UINT32)ULLONG_MAX;
    LoadableSegSize = 0;
    
    for (i = 0; i < PhdrCnt; ++i)
    {
        if (!SegmentIsLoadable(&Phdr[i]))
            continue;
        
        /* find the lowest and highest address of loadable segments */
        MemMax = MAX_VALUE(MemMax, Phdr[i].p_paddr + Phdr[i].p_memsz);
        MemMin = MIN_VALUE(MemMin, Phdr[i].p_paddr);
        
        /* find the offset of the 1st loadable segments, 
            and the total size of loadable segments */
        LoadableSegOff = MIN_VALUE(LoadableSegOff, Phdr[i].p_offset);
        LoadableSegSize += Phdr[i].p_filesz;
        
        if (SegmentIsRelocatable(&Phdr[i])) {
            Reloc = TRUE;
            //if (MemMin == Phdr[i].p_paddr)
            //    Align = Phdr[i].p_align;
        } 
        
        // TODO, moved out from above because ABL segment is not marked relocatable.
        if (MemMin == Phdr[i].p_paddr)
                Align = Phdr[i].p_align;
    }
    
    if (Priv->ElfLoadInfo.ElfBase != NULL) {
        /* if entire elf is loaded in memory, set the base and total size of loadable segments */
        Priv->ElfLoadInfo.SegBase = (Priv->ElfLoadInfo).ElfBase + LoadableSegOff;
        Priv->ElfLoadInfo.SegSize = LoadableSegSize;
        PIL_INFO_CFG(Cfg, "ELF is loaded in memory, SegBase=0x%x,SegSize=0x%x\n", \
                     Priv->ElfLoadInfo.SegBase, LoadableSegSize);
    }
    
    PIL_INFO_CFG(Cfg, "before relocation, MemMin=0x%x MemMax=0x%x\n", MemMin, MemMax);
    MemMaxAligned = ALIGN_UP_SZ(MemMax, SZ_4K);
    MemMinAligned = ALIGN_DOWN_SZ(MemMin, SZ_4K);
    AlignedSize = MemMaxAligned - MemMinAligned;
    PIL_INFO_CFG(Cfg, "after aligned MemMin=0x%x MemMax=0x%x size=0x%x\n", \
                 MemMinAligned, MemMaxAligned, AlignedSize);
    
    /*
    rules are: 
    1) if ELF_FV use the buffer which is already allocated by reading partition
    2) else (not ELF_FV) if have ELF address, check ipcate, if have ipcate make sure the ELF address
     fall in the ipcate region, load to ELF address
    3) if have ELF address and no ipcate, load to ELF address
    4) if have ELF address 0, assert relocate bit is set, if have ipcate load to ipcate address
    5) if have ELF adderss 0 and no ipcate, assert relocatable, allocate buffer  
    */
    if (Cfg->Type == ELF_FV)
    {
        /* entire ELF is already loaded in memory, just use the buffer we already loaded */
        /* TODO, require SegBase to be 4k alignment ? */
        Priv->AllocType = PIL_MEM_ALLOC_FV;
        Priv->RegionStart = (UINTN)(Priv->ElfLoadInfo).SegBase;
        Priv->RegionSize = (Priv->ElfLoadInfo).SegSize;
        Priv->BaseAddr = (UINTN)(Priv->ElfLoadInfo).SegBase;
    }
    else {
        if ( !Cfg->OverrideElfAddr && MemMinAligned != 0) {
            /* if we dont want to override loading addres from ELF which is non-zero, 
               consider it valid loading address */
            
            UINT32 RegionSize = (Cfg->ResvRegionSize==0)?AlignedSize:Cfg->ResvRegionSize;
            if (Cfg->ResvRegionStart != 0)
            {
                /* have valid ELF address and ipcate address, make sure ELF address
                fall in the ipcate region */
                if ( MemMinAligned < Cfg->ResvRegionStart || Cfg->ResvRegionStart + RegionSize < MemMaxAligned)
                {
                    PIL_ERROR_CFG(Cfg, "Resv start or ResvRegionSize does not match that from ELF\n");    
                    PIL_ERROR_CFG(Cfg, "Resv start 0x%x ELF start 0x%x\n", Cfg->ResvRegionStart, MemMinAligned);
                    PIL_ERROR_CFG(Cfg, "Resv ebd 0x%x ELF end 0x%x\n", \
                                  Cfg->ResvRegionStart + RegionSize, MemMaxAligned);
                    return PIL_INVALID_CFG;
                }
            }
            
            // since have ELF address, always use it
            Priv->RegionStart = MemMinAligned;
            Priv->RegionSize = RegionSize;
            Priv->BaseAddr = MemMinAligned;
            Priv->AllocType = PIL_MEM_RESV;
        }
        
        else {
            /* loading address in ELF is 0, which is definitely invalid address. 
               or OverrideElfAddr is set. Need to look for alternative loading address.
               first make sure relocate bit is set.
            */
            if (!Reloc) {
                PIL_ERROR_CFG(Cfg, "have invalide loading address from ELF but image is not relocatable\n");
                return PIL_ELF_ERR;
            }
            
            if (Cfg->ResvRegionStart != 0) {
                /* have invalid ELF address and valid ipcate address, use ipcate address */
                Priv->RegionStart = Cfg->ResvRegionStart;
                Priv->RegionSize = (Cfg->ResvRegionSize==0)?AlignedSize:Cfg->ResvRegionSize;
                Priv->BaseAddr = MemMinAligned;
                Priv->AllocType = PIL_MEM_RESV;
            }
            else {
                /* have invalid ELF address and invalid ipcate address, alloc buffer */
                Priv->AllocType = PIL_MEM_ALLOC;
            }
        }
    }
    
    // should have decision on AllocType
    ASSERT(Priv->AllocType != 0);
    ASSERT_FAIL_PIL_RETURN(Priv->AllocType != 0, PIL_LOAD_ERR, PIL_LOAD_ERR);
    
    if (Priv->AllocType == PIL_MEM_ALLOC) 
    {
        /* alloc buffer for image loading if we need to */
        UINT32 RegionSize = (Cfg->ResvRegionSize==0)?AlignedSize:Cfg->ResvRegionSize;
        StartVaddr = UncachedAllocateAlignedZeroPool(RegionSize, Align);
        if (StartVaddr==NULL) {
            PIL_ERROR_CFG(Cfg, "Failed to alloc buffer to load image size=0x%x\n", RegionSize);
            return PIL_OUT_OF_MEMORY;
        }
        
        Priv->RegionStart = ConvertToPhysicalAddress(StartVaddr);
        Priv->RegionSize = RegionSize;
        Priv->BaseAddr = MemMinAligned;
    }
    
    PIL_INFO_CFG(Cfg, "PIL memory setup start=0x%x size=0x%x\n", Priv->RegionStart, \
                 Priv->RegionSize);
    
    // make sure start loading address is aligned
    if ((Priv->RegionStart & (Align -1)) != 0)
    {
        PIL_ERROR_CFG(Cfg, "Start loading address is not aligned to 0x%x", Align);
        ASSERT(0);
        ASSERT_FAIL_PIL_RETURN(0, PIL_INVALID_CFG, PIL_INVALID_CFG);
    }
    
    Status = PilScmStdCall(Ctx, TZ_PIL_MEM_ID, TZ_PIL_MEM_ID_PARAM_ID, Cfg->SubsysID, Priv->RegionStart, Priv->RegionSize);
    if (EFI_ERROR(Status)) {
        PIL_ERROR_CFG(Cfg, "failed to setup memory for firmware image return %r\n", Status);
        return PIL_SMC_ERR;
    } 
    PIL_INFO_CFG(Cfg, "Successfully setup memory from 0x%x size 0x%x\n", Priv->RegionStart, Priv->RegionSize);

    return PIL_SUCCESS;
}


PIL_STATUS PilDeleteMemoryRange(PIL_SUBSYS_CTX_TYPE* Ctx)
{
    PIL_SUBSYS_PRIV_TYPE* Priv;
    int Ret = 0;
    
    ASSERT(Ctx != NULL);
    ASSERT_FAIL_PIL_RETURN(Ctx != NULL, PIL_INVALID_PARAM, PIL_INVALID_PARAM);
    Priv = Ctx->Priv;
    ASSERT(Priv != NULL);
    ASSERT_FAIL_PIL_RETURN(Priv != NULL, PIL_INVALID_PARAM, PIL_INVALID_PARAM);
    
    switch (Priv->AllocType)
    {
    case PIL_MEM_RESV:
        break;
    case PIL_MEM_ALLOC:
        UncachedFreeAlignedPool((VOID*)Priv->RegionStart);
        break;
    default:
        Ret = PIL_GENERIC_ERR;
    }
    return Ret;
}


/**
   Calculate the metadata size of the ELF.
   
   @param  ElfBase[in]           base address of the elf file

   @return non-zero              size of metadata.
   @return zero                  error happens.
   
**/
STATIC UINT32 ElfGetMetaDataSize(VOID* ElfBase)
{
    UINT32 MdtSize=0,i;
    uint8 ElfFormate;
    Elf32_Ehdr* Ehdr32;
    Elf32_Phdr* Phdr32, *Phdr32Base;
    UINT64 StorageStart, StorageEnd;
    
    ASSERT(ElfBase!=NULL);
    ASSERT_FAIL_PIL_RETURN(ElfBase!=NULL, 0, PIL_LOAD_ERR);
    
    StorageStart = GetTimerCountus();
    
    ElfFormate =((Elf64_Ehdr *)ElfBase)->e_ident[EI_CLASS];
    if ( ElfFormate == ELFCLASS32 )
    {
        Ehdr32 = (Elf32_Ehdr*)ElfBase;
        MdtSize = sizeof(Elf32_Ehdr) + sizeof(Elf32_Phdr) * Ehdr32->e_phnum;
        Phdr32Base = (Elf32_Phdr*)((VOID*)Ehdr32 + Ehdr32->e_phoff);
        
        /* look for hash segment */
        for (i=0; i<Ehdr32->e_phnum; i++)
        {
            Phdr32 = &Phdr32Base[i];
            if ( (Phdr32->p_flags & ELF_P_FLAGS_MASK) == ELF_P_FLAGS_HASH)
            {
                // TODO, should use filzsz or memsz ?
                MdtSize += Phdr32->p_filesz;
                break;
            }
        }
    }
    
    StorageEnd = GetTimerCountus();
    StorageSpentCnt += (StorageEnd - StorageStart);
    return MdtSize;
}

/**
   copy metadata to the given buffer.
   
   @param  ElfBase[in]           base address of the elf file
   @param  MdtBuffer[in]         buffer to copy metadata
   @param  MdtSize[in]           size of the MdtBuffer

   @return PIL_SUCCESS           success.
   @return other                 error happens.
**/
STATIC PIL_STATUS ElfCopyMetaData(VOID* ElfBase, VOID* MdtBuffer, UINT32 BufSize)
{
    uint8 ElfFormate;
    UINT32 CopiedCnt=0,i;
    Elf32_Ehdr* Ehdr32;
    Elf32_Phdr* Phdr32, *Phdr32Base;
    
    ASSERT(ElfBase!=NULL);
    ASSERT_FAIL_PIL_RETURN(ElfBase!=NULL, 0, PIL_LOAD_ERR);
    
    ElfFormate =((Elf64_Ehdr *)ElfBase)->e_ident[EI_CLASS];
    if ( ElfFormate == ELFCLASS32 )
    {
        Ehdr32 = (Elf32_Ehdr*)ElfBase;
        Phdr32Base = (Elf32_Phdr*)((VOID*)Ehdr32 + Ehdr32->e_phoff);
        CopyMem(MdtBuffer, ElfBase, (CopiedCnt = sizeof(Elf32_Ehdr)));
        CopyMem(MdtBuffer + CopiedCnt, Phdr32Base, sizeof(Elf32_Phdr)* Ehdr32->e_phnum);
        CopiedCnt += sizeof(Elf32_Phdr)* Ehdr32->e_phnum;
        
        for (i=0; i<Ehdr32->e_phnum; i++)
        {
            Phdr32 = &Phdr32Base[i];
            if ( (Phdr32->p_flags & ELF_P_FLAGS_MASK) == ELF_P_FLAGS_HASH)
            {
                CopyMem(MdtBuffer + CopiedCnt, ElfBase + Phdr32->p_offset, Phdr32->p_filesz);
                CopiedCnt += Phdr32->p_filesz;
                break;
            }
        }
    }
    return (CopiedCnt<=BufSize)?PIL_SUCCESS:PIL_LOAD_ERR;
}

/**
  load ELF image into memory if it's not in split. Save the address and size 
  in ElfBase and Elfize.

  @param[in] Ctx    ptr to ctx data of PIL   
  
  @retval PIL_SUCCESS       success
  @retval other             failure

**/
PIL_STATUS PilLoadElfFile(PIL_SUBSYS_CTX_TYPE* Ctx)
{
    PIL_SUBSYS_PRIV_TYPE* Priv;
    PIL_SUBSYS_CFG_TYPE* Cfg;
    CHAR16   ElfFilePath[NAME_MAX_LENGTH];
    EFI_STATUS Status;
    UINTN       ElfSize = 0;
    UINT8*      ElfBuffer = NULL;
    EFI_GUID*   RootGuid;
    EFI_GUID*   PartiGuid;
    CHAR16*     PartiLabel;
    UINT64      StorageStart, StorageEnd;
    
    ASSERT(Ctx != NULL);
    ASSERT_FAIL_PIL_RETURN(Ctx != NULL, PIL_INVALID_PARAM, PIL_INVALID_PARAM);
    ASSERT(NULL != (Priv = Ctx->Priv));
    ASSERT_FAIL_PIL_RETURN(Priv != NULL, PIL_INVALID_PARAM, PIL_INVALID_PARAM);
    ASSERT(NULL != (Cfg = Ctx->Cfg));
    ASSERT_FAIL_PIL_RETURN(Cfg != NULL, PIL_INVALID_PARAM, PIL_INVALID_PARAM);
    
    StorageStart = GetPerformanceCounter();
    
    Status = EFI_LOAD_ERROR;
    if (Cfg->Type == ELF_FV) 
    {
        if (!IsZeroGuid((CONST GUID*)&Cfg->PartiGuid))
            Status = ReadFromPartitionByGuid(&Cfg->PartiGuid, (VOID**)&ElfBuffer, &ElfSize);
        else if (Cfg->PartiLabel[0] != 0) 
            Status = ReadFromPartition(Cfg->PartiLabel, (VOID**)&ElfBuffer, &ElfSize);
        else {
            PIL_ERROR_CFG(Cfg, "Need partition GUID or label to load image\n");
            return PIL_INVALID_PARAM;
        }
        
        if (EFI_ERROR(Status)) {
            PIL_ERROR_CFG(Cfg, "Failed to read from partition\n");
            return PIL_LOAD_ERR;
        }
    }
    else if (Cfg->Type == ELF_SINGLE)
    {
        do {
        /* use parti guid if have one, otherwise use label */
        RootGuid = IsZeroGuid((CONST GUID*)&Cfg->PartiRootGuid)?NULL:&Cfg->PartiRootGuid;
        PartiGuid = IsZeroGuid((CONST GUID*)&Cfg->PartiGuid)?NULL:&Cfg->PartiGuid;
        PartiLabel = (PartiGuid == NULL)?Cfg->PartiLabel:NULL;
#if 0
        UINT32      BlkIOAttrib=0;
        BlkIOAttrib = BLK_IO_SEL_PARTITIONED_MBR;
        BlkIOAttrib |= BLK_IO_SEL_MEDIA_TYPE_REMOVABLE;
        BlkIOAttrib |= BLK_IO_SEL_SELECT_MOUNTED_FILESYSTEM;
        BlkIOAttrib |= BLK_IO_SEL_PARTITIONED_GPT;
        BlkIOAttrib |= BLK_IO_SEL_STRING_CASE_INSENSITIVE;
          
        UnicodeSPrint(ElfFilePath, sizeof(ElfFilePath), L"%s.mbn", Cfg->ImagePath);
        Status = LoadFileFromPartition(BlkIOAttrib,
                                ElfFilePath,
                                RootGuid,
                                PartiGuid,
                                PartiLabel,
                                &ElfBuffer, 
                                &ElfSize);
#endif

        UnicodeSPrint(ElfFilePath, sizeof(ElfFilePath), L"%s.mbn", Cfg->ImagePath);
        Status = GetFileSize(ElfFilePath,
                        RootGuid,
                        PartiGuid,
                        PartiLabel,
                        TRUE,
                        NULL,
                        (UINT32*)&ElfSize);
        if (EFI_ERROR(Status) || ElfSize==0) break;
        
        UINT32 ElfSizeAligned = ALIGN_UP_SZ(ElfSize, EFI_PAGE_SIZE);
        ElfBuffer = (UINT8*)AllocatePages(ElfSizeAligned/EFI_PAGE_SIZE);
        if (NULL == ElfBuffer) {
            PIL_ERROR_CFG(Cfg, "Failed to alloc uncached pool for MDT buffer\n");
            Status = EFI_OUT_OF_RESOURCES;
            break;
        }
        
        UINTN ElfSizeRead = ElfSize;
        Status = ReadFile ( ElfFilePath,
                        RootGuid,
                        PartiGuid,
                        PartiLabel,
                        TRUE,
                        NULL,
                        &ElfSizeRead,
                        0,
                        ElfBuffer,
                        ElfSize);
           
        PIL_INFO_CFG(Cfg, "ReadFile %s return %r metadata read_size = 0x%x\n", ElfFilePath, Status, ElfSizeRead);
        if (EFI_ERROR(Status)) break;
    
        } while (0);
        
        if(EFI_ERROR(Status) || ElfSize == 0) {
            PIL_ERROR_CFG(Cfg, "Failed to load %s from %s\n partition", ElfFilePath, Cfg->PartiLabel);
            return PIL_LOAD_ERR;
        }
    }
    
    PIL_INFO_CFG(Cfg, "ELF is loaded in memory, ElfBase=0x%x ElfSize=0x%x\n", ElfBuffer, ElfSize);
    (Priv->ElfLoadInfo).ElfBase = ElfBuffer;
    (Priv->ElfLoadInfo).ElfSize = ElfSize;
    
    StorageEnd = GetPerformanceCounter();
    StorageSpentCnt += (StorageEnd - StorageStart);
    return PIL_SUCCESS;
}

/**
  after the non-split ELF loaded in memory, copy the metadata to 
  an uncached buffer. save the pointer to Priv->MdtBuf.

  @param[in] Ctx    ptr to ctx data of PIL   
  
  @retval PIL_SUCCESS       success
  @retval other             failure

**/
PIL_STATUS CopyMetaDataFromLoadedElf(PIL_SUBSYS_CTX_TYPE* Ctx)
{
    UINT32 MetaDataSize, MetaDataSizeAligned;
    UINT8* MetaDataBuffer;
    VOID*  ElfBuffer;
    PIL_SUBSYS_PRIV_TYPE* Priv;
    PIL_SUBSYS_CFG_TYPE* Cfg;
    PIL_STATUS PilRet;
    
    ASSERT(Ctx != NULL);
    ASSERT_FAIL_PIL_RETURN(Ctx != NULL, PIL_INVALID_PARAM, PIL_INVALID_PARAM);
    ASSERT(NULL != (Priv = Ctx->Priv));
    ASSERT_FAIL_PIL_RETURN(Priv != NULL, PIL_INVALID_PARAM, PIL_INVALID_PARAM);
    ASSERT(NULL != (Cfg = Ctx->Cfg));
    ASSERT_FAIL_PIL_RETURN(Cfg != NULL, PIL_INVALID_PARAM, PIL_INVALID_PARAM);
    
    ElfBuffer = (Priv->ElfLoadInfo).ElfBase;
    MetaDataSize = ElfGetMetaDataSize(ElfBuffer);
    if (MetaDataSize==0) {
        PIL_ERROR_CFG(Cfg, "Failed to get metadata size\n");
        return PIL_LOAD_ERR;
    }
    
    MetaDataSizeAligned = ALIGN_UP_SZ(MetaDataSize, SZ_4K);
    MetaDataBuffer = (UINT8*)UncachedAllocateAlignedZeroPool(MetaDataSizeAligned, SZ_4K);
    if (MetaDataBuffer==NULL) {
        PIL_ERROR_CFG(Cfg, "Failed to alloc metadata buffer\n");
        return PIL_OUT_OF_MEMORY;
    }
    PilRet = ElfCopyMetaData(ElfBuffer, MetaDataBuffer, MetaDataSizeAligned);
    if (PilRet!=PIL_SUCCESS) {
        PIL_ERROR_CFG(Cfg, "Failed to copy metadata to buffer\n");
        return PIL_LOAD_ERR;
    }
           
    Priv->MdtBuf = MetaDataBuffer;
    return PIL_SUCCESS;
}

/**
  for split ELF, load the metadata file to uncached buffer, the pointer is
  saved to Priv->MdtBuf.

  @param[in] Ctx    ptr to ctx data of PIL   
  
  @retval PIL_SUCCESS       success
  @retval other             failure
**/

PIL_STATUS PilLoadMdtFile(PIL_SUBSYS_CTX_TYPE* Ctx)
{
    PIL_SUBSYS_PRIV_TYPE* Priv;
    PIL_SUBSYS_CFG_TYPE* Cfg;
    CHAR16       MetaDataFilePath[NAME_MAX_LENGTH];
    EFI_STATUS  Status   = EFI_SUCCESS;
    UINT32      MetaDataSize = 0, MetaDataSizeAligned;
    UINTN        MetaDataSizeRead;
    UINT8*       MetaDataBuffer = NULL;
    EFI_GUID*   RootGuid;
    EFI_GUID*   PartiGuid;
    CHAR16*     PartiLabel;
    UINT64      StorageStart, StorageEnd;
    
    ASSERT(Ctx != NULL);
    ASSERT_FAIL_PIL_RETURN(Ctx != NULL, PIL_INVALID_PARAM, PIL_INVALID_PARAM);
    ASSERT(NULL != (Priv = Ctx->Priv));
    ASSERT_FAIL_PIL_RETURN(Priv != NULL, PIL_INVALID_PARAM, PIL_INVALID_PARAM);
    ASSERT(NULL != (Cfg = Ctx->Cfg));
    ASSERT_FAIL_PIL_RETURN(Cfg != NULL, PIL_INVALID_PARAM, PIL_INVALID_PARAM);
    
    if (Cfg->ImagePath[0] == 0) {
        PIL_ERROR_CFG(Cfg, "%r type must have valid ImagePath\n", Cfg->Type);
        return PIL_INVALID_CFG;
    }
            
    /* cannot have PartiGuid and PartiLabel both invalid */
    if (IsZeroGuid((CONST GUID*)&Cfg->PartiGuid) && \
        Cfg->PartiLabel[0] == 0 ) {
        PIL_ERROR_CFG(Cfg, "%r type must have PartiGuid or PartiLabel\n", Cfg->Type);
        return PIL_INVALID_CFG;
    }
    
    StorageStart = GetPerformanceCounter();
    
    /* use parti guid if have one, otherwise use label */
    RootGuid = IsZeroGuid((CONST GUID*)&Cfg->PartiRootGuid)?NULL:&Cfg->PartiRootGuid;
    PartiGuid = IsZeroGuid((CONST GUID*)&Cfg->PartiGuid)?NULL:&Cfg->PartiGuid;
    PartiLabel = (PartiGuid == NULL)?Cfg->PartiLabel:NULL;
          
    UnicodeSPrint(MetaDataFilePath, sizeof(MetaDataFilePath), L"%s.mdt", Cfg->ImagePath);
           
    Status = GetFileSize(MetaDataFilePath,
                        RootGuid,
                        PartiGuid,
                        PartiLabel,
                        TRUE,
                        NULL,
                        &MetaDataSize);
            
    PIL_INFO_CFG(Cfg, "GetFileSize %s return %r MetaDataSize = 0x%x\n", MetaDataFilePath, Status, MetaDataSize);
    if(EFI_ERROR(Status) || MetaDataSize == 0) {
        PIL_ERROR_CFG(Cfg, "Failed to get MDT size\n");
        return PIL_LOAD_ERR;
    }

    MetaDataSizeAligned = ALIGN_UP_SZ(MetaDataSize, SZ_4K);
    MetaDataBuffer = (UINT8*)UncachedAllocateAlignedZeroPool(MetaDataSizeAligned, SZ_4K);
    if (NULL == MetaDataBuffer) {
        PIL_ERROR_CFG(Cfg, "Failed to alloc uncached pool for MDT buffer\n");
        return PIL_OUT_OF_MEMORY;
    }
            
    MetaDataSizeRead = MetaDataSize;
    Status = ReadFile ( MetaDataFilePath,
                        RootGuid,
                        PartiGuid,
                        PartiLabel,
                        TRUE,
                        NULL,
                        &MetaDataSizeRead,
                        0,
                        MetaDataBuffer,
                        MetaDataSize);
            
    PIL_INFO_CFG(Cfg, "ReadFile %s return %r metadata read_size = 0x%x\n", MetaDataFilePath, Status, MetaDataSizeRead);
    if(EFI_ERROR(Status)) {
        PIL_ERROR_CFG(Cfg, "Failed to read mdt file\n");
        return PIL_LOAD_ERR;
    }
            
    Priv->MdtBuf = MetaDataBuffer;
    
    StorageEnd = GetPerformanceCounter();
    StorageSpentCnt += (StorageEnd - StorageStart);
    return PIL_SUCCESS;
}


/**
  load segments into memory

  @param[in] Ctx    ptr to ctx data of PIL   
  
  @retval PIL_SUCCESS       load segments sucessfully
  @retval other             load fail

**/

PIL_STATUS PilLoadSegments(PIL_SUBSYS_CTX_TYPE* Ctx)
{
    Elf32_Phdr *Phdr, *p_seg;
    UINT32 PhdrCnt;
    Elf32_Ehdr *Ehdr;
    UINT32        i;
    EFI_STATUS  Status   = EFI_SUCCESS;
    UINTN        SegStartPaddr, SegEndPaddr, SegSource;
    CHAR16        SegName[32];
    UINTN        SegSize;
    PIL_SUBSYS_PRIV_TYPE* Priv;
    PIL_SUBSYS_CFG_TYPE* Cfg;
    EFI_GUID*   RootGuid;
    EFI_GUID*   PartiGuid;
    CHAR16*     PartiLabel;
    UINT64      StorageStart, StorageEnd;
    
    ASSERT(Ctx != NULL);
    ASSERT_FAIL_PIL_RETURN(Ctx != NULL, PIL_INVALID_PARAM, PIL_INVALID_PARAM);
    ASSERT(NULL != (Priv = Ctx->Priv));
    ASSERT_FAIL_PIL_RETURN(Priv != NULL, PIL_INVALID_PARAM, PIL_INVALID_PARAM);
    ASSERT(NULL != (Cfg = Ctx->Cfg));
    ASSERT_FAIL_PIL_RETURN(Cfg != NULL, PIL_INVALID_PARAM, PIL_INVALID_PARAM);
    
    if (Cfg->Type != ELF_SPLIT && Cfg->Type != ELF_SINGLE)
        return PIL_SUBSYS_NOT_SUPPORTED;
    
    StorageStart = GetPerformanceCounter();
    
    Ehdr = (Elf32_Ehdr*)(Priv->MdtBuf);
    ASSERT(Ehdr != NULL);
    ASSERT_FAIL_PIL_RETURN(Ehdr != NULL, PIL_INVALID_PARAM, PIL_INVALID_PARAM);
    Phdr = (Elf32_Phdr*)((uint8*)Ehdr + Ehdr->e_phoff);
    PhdrCnt = Ehdr->e_phnum;
    
    for (i=0; i<PhdrCnt; i++)
    {
        p_seg = &Phdr[i];
        if ( !SegmentIsLoadable(p_seg) || p_seg->p_filesz == 0)
            continue;
        
        if ( SegmentIsRelocatable(p_seg) )
        {
            // RegionStart is the actual loading address -->
            //   either ELF (for nonrelocatable) or Buffer (for relocatable)
            SegStartPaddr = p_seg->p_paddr + Priv->RegionStart - Priv->BaseAddr;
        } else {
            SegStartPaddr = p_seg->p_paddr;
        }
        SegEndPaddr = SegStartPaddr + p_seg->p_memsz;
        
        if (Cfg->Type == ELF_SPLIT)
        {
            /*read file to the allocated buffer*/
            UnicodeSPrint(SegName, sizeof(SegName), L"%s.b%02d", Cfg->ImagePath, i);
            SegSize = p_seg->p_filesz;
            
            /* use parti guid if have one, otherwise use label */
            RootGuid = IsZeroGuid((CONST GUID*)&Cfg->PartiRootGuid)?NULL:&Cfg->PartiRootGuid;
            PartiGuid = IsZeroGuid((CONST GUID*)&Cfg->PartiGuid)?NULL:&Cfg->PartiGuid;
            PartiLabel = (PartiGuid == NULL)?Cfg->PartiLabel:NULL;
            
            Status = ReadFile ( SegName,
                            RootGuid,
                            PartiGuid,
                            PartiLabel,
                            TRUE,
                            NULL,
                            &SegSize,
                            0,
                            (UINT8*)SegStartPaddr,
                            p_seg->p_filesz);
        }
        else { 
            // if (Cfg->Type == ELF_SINGLE)
            SegSource = (UINTN)((Priv->ElfLoadInfo).ElfBase) + p_seg->p_offset;
            CopyMem((VOID*)SegStartPaddr, (VOID*)SegSource, p_seg->p_filesz);
            SegSize = p_seg->p_filesz;
        }

        PIL_INFO_CFG(Cfg, "load %s return %r Reloc %d start 0x%x end 0x%x memsz 0x%x filesz 0x%x read_sz 0x%x\n", \
                    SegName, Status, (UINT32)SegmentIsRelocatable(p_seg), SegStartPaddr, \
                    SegEndPaddr, p_seg->p_memsz, p_seg->p_filesz, SegSize);
                
        if(EFI_ERROR(Status) || SegSize != p_seg->p_filesz) {
            PIL_ERROR_CFG(Cfg, "Failed to read %s\n", SegName);
            return PIL_GENERIC_ERR;
        }
        else {
            UINTN Vaddr, Size;
            /* zero out tailing memory */
            Vaddr = SegStartPaddr + p_seg->p_filesz;
            Size = p_seg->p_memsz - p_seg->p_filesz;
            ZeroMem((VOID*)Vaddr, Size);
            PIL_INFO_CFG(Cfg, "Zero out tailing memory start 0x%x size 0x%x\n", Vaddr, Size);
        }
    }

    StorageEnd = GetPerformanceCounter();
    StorageSpentCnt += (StorageEnd - StorageStart);
    return PIL_SUCCESS;
}

/*
   do authentication and bring the subsystem out of reset
   
   @param[in] Ctx    ptr to ctx data of PIL   
  
   @retval PIL_SUCCESS       success
   @retval other             failure
*/
PIL_STATUS PilAuthAndReset(PIL_SUBSYS_CTX_TYPE* Ctx)
{
    PIL_SUBSYS_CFG_TYPE* Cfg;
    EFI_STATUS Status;
    
    ASSERT(Ctx != NULL);
    ASSERT_FAIL_PIL_RETURN(Ctx != NULL, PIL_INVALID_PARAM, PIL_INVALID_PARAM);
    ASSERT(NULL != (Cfg = Ctx->Cfg));
    ASSERT_FAIL_PIL_RETURN(Cfg != NULL, PIL_INVALID_PARAM, PIL_INVALID_PARAM);
    
    /*bring subsys out of reset*/
    Status = PilScmStdCall(Ctx, TZ_PIL_AUTH_RESET_ID, TZ_PIL_AUTH_RESET_ID_PARAM_ID, Cfg->SubsysID);
    if (EFI_ERROR(Status)) {
        PIL_ERROR_CFG(Cfg, "Failed to PAS authenticate and reset subsys %r\n", Status);
        return PIL_SMC_ERR;
    }
    PIL_INFO_CFG(Cfg, "Successfully PAS authenticate and reset subsys\n");

    return PIL_SUCCESS;
}

/**
    copy the image name from ImagePath to a given buffer
    
    @param[in] Ctx            ctx of the subsys
    @param[out] Buffer        buffer used to copy the image name
  
    @retval PIL_SUCCESS       success
    @retval other             failure
**/

STATIC PIL_STATUS CopyImageName(PIL_SUBSYS_CTX_TYPE* Ctx, CHAR8* Buffer)
{
    PIL_SUBSYS_CFG_TYPE* Cfg;
    PIL_SUBSYS_PRIV_TYPE* Priv;
    CHAR16* ptr, *Start;
    
    ASSERT(Ctx != NULL);
    ASSERT_FAIL_PIL_RETURN(Ctx != NULL, PIL_INVALID_PARAM, PIL_INVALID_PARAM);
    ASSERT((Cfg = Ctx->Cfg) != NULL);
    ASSERT_FAIL_PIL_RETURN(Cfg != NULL, PIL_INVALID_PARAM, PIL_INVALID_PARAM);
    ASSERT((Priv = Ctx->Priv) != NULL);
    ASSERT_FAIL_PIL_RETURN(Priv != NULL, PIL_INVALID_PARAM, PIL_INVALID_PARAM);
    
    // must have ImagePath
    if (Cfg->ImagePath[0] == 0)
        return PIL_INVALID_CFG;
    
    ptr = &Cfg->ImagePath[0];
    Start = ptr;
    while (*ptr != '\0')
    {
        if (*ptr == '\\')
            Start = ptr + 1;
        ptr++;
    }
    
    if (StrnLenS(Start, FWNAME_SHORT_LEN) >= FWNAME_SHORT_LEN)
        return PIL_INVALID_CFG;
    
    UnicodeStrToAsciiStr(Start, Buffer);
    return PIL_SUCCESS;
}

/**
    update image load info in IMEM, the region start at ImageLoadInfoBase
    size is ImageLoadInfoSize
    
    @param[in] Ctx            ctx of the subsys
  
    @retval PIL_SUCCESS       success
    @retval other             failure
    
**/

STATIC PIL_STATUS UpdateImageLoadInfo(PIL_SUBSYS_CTX_TYPE* Ctx)
{
    STATIC UINT32 LoadInfoId = 0;
    PIL_IMAGE_INFO_TYPE *LoadInfo = (PIL_IMAGE_INFO_TYPE*)ImageLoadInfoBase;
    PIL_SUBSYS_PRIV_TYPE* Priv;
    PIL_SUBSYS_CFG_TYPE* Cfg;
    PIL_IMAGE_INFO_TYPE *ImemInfoAddr = NULL;
    CHAR8 ImageNameBuf[FWNAME_SHORT_LEN] = {0};
    UINT32 Idx;
    PIL_STATUS PilRet;
    
    ASSERT(Ctx != NULL);
    ASSERT_FAIL_PIL_RETURN(Ctx != NULL, PIL_INVALID_PARAM, PIL_INVALID_PARAM);
    ASSERT((Priv=Ctx->Priv) != NULL);
    ASSERT_FAIL_PIL_RETURN(Priv != NULL, PIL_INVALID_PARAM, PIL_INVALID_PARAM);
    ASSERT((Cfg=Ctx->Cfg) != NULL);
    ASSERT_FAIL_PIL_RETURN(Cfg != NULL, PIL_INVALID_PARAM, PIL_INVALID_PARAM);
    
    PilRet = CopyImageName(Ctx, ImageNameBuf);
    if (PilRet != PIL_SUCCESS)
        return PilRet;
    
    // search for ImageName in IMEM, check if it's already filled
    for (Idx=0; Idx<LoadInfoId; Idx++)
    {
        if (0 == AsciiStrnCmp(LoadInfo[Idx].Name, \
                        ImageNameBuf, FWNAME_SHORT_LEN))
            ImemInfoAddr = &LoadInfo[Idx];
    }
    
    if (ImemInfoAddr == NULL) 
    {
        if (LoadInfoId >=  ImageLoadInfoSize/sizeof(PIL_IMAGE_INFO_TYPE))
        {
            PIL_ERROR_CFG(Cfg, "Image load region in IMEM is full\n");
            return PIL_GENERIC_ERR;
        }
        
        // make ImemInfoAddr point to next availale slot
        ImemInfoAddr = (PIL_IMAGE_INFO_TYPE *)(ImageLoadInfoBase + \
                        sizeof(PIL_IMAGE_INFO_TYPE)*(LoadInfoId++));
    }
    
    // write image load info to the IMEM slot
    AsciiStrnCpyS(&ImemInfoAddr->Name[0], FWNAME_SHORT_LEN, ImageNameBuf, FWNAME_SHORT_LEN);
    WriteUnaligned64(&ImemInfoAddr->Start, (UINT64)(Priv->RegionStart));
    WriteUnaligned32(&ImemInfoAddr->Size, (UINT32)(Priv->RegionSize));
    PIL_INFO_CFG(Cfg, "Image load info is updated to 0x%x\n", ImemInfoAddr);
    return PIL_SUCCESS;
}

/**
    operations required after proc is brought out of reset
    
    @param[in] Ctx            ctx of the subsys
  
    @retval PIL_SUCCESS       success
    @retval other             failure
    
**/
PIL_STATUS PilPostLoad(PIL_SUBSYS_CTX_TYPE* Ctx)
{
    PIL_SUBSYS_CFG_TYPE* Cfg;
    PIL_SUBSYS_PRIV_TYPE* Priv;
    EFI_STATUS        Status;
    PIL_STATUS        PilRet;
    UINT64            FvProcStart, FvProcEnd;
    
    ASSERT(Ctx != NULL);
    ASSERT_FAIL_PIL_RETURN(Ctx != NULL, PIL_INVALID_PARAM, PIL_INVALID_PARAM);
    ASSERT((Cfg = Ctx->Cfg) != NULL);
    ASSERT_FAIL_PIL_RETURN(Cfg != NULL, PIL_INVALID_PARAM, PIL_INVALID_PARAM);
    ASSERT((Priv = Ctx->Priv) != NULL);
    ASSERT_FAIL_PIL_RETURN(Priv != NULL, PIL_INVALID_PARAM, PIL_INVALID_PARAM);
    
    if (Cfg->Unlock) 
    {
        Status = PilScmStdCall(Ctx, TZ_PIL_UNLOCK_XPU_ID, TZ_PIL_UNLOCK_XPU_ID_PARAM_ID, Cfg->SubsysID);
        if (EFI_ERROR(Status)) {
            PIL_ERROR_CFG(Cfg, "Failed to unlock subsys %r\n", Status);
            return PIL_SMC_ERR;
        }
        PIL_INFO_CFG(Cfg, "Successfully unlock subsys\n");
    }
    
    FvProcStart = GetPerformanceCounter();
    /* process fv images */
    if (Cfg->Type == ELF_FV) {
        Status = ProcessFvImageFile((VOID*)Priv->RegionStart);
        if (EFI_ERROR(Status)) {
            PIL_ERROR_CFG(Cfg, "Failed to process FV image\n");
            return PIL_GENERIC_ERR;
        }
        PIL_INFO_CFG(Cfg, "Successfully process Fv image\n");
    }
    FvProcEnd = GetPerformanceCounter();
    FvProcSpentCnt += (FvProcEnd - FvProcStart);
    
    // write image load info to IMEM which is assumed to have mapping
    if (Cfg->ImageLoadInfo) {
        PilRet = UpdateImageLoadInfo(Ctx);
        if (PilRet != PIL_SUCCESS) {
            PIL_WARN_CFG(Cfg, "Failed to update image load info im IMEM\n");
        }
    }
    
    return PIL_SUCCESS;
}

/*
   clean up the PIL buffer if subsystem loaded successfully. some of them should 
   be kept for debugging/analyzing purpose. ELF buffer will be freed if it's 
   non-split. metadata buffer will be cleaned up. all others are kept until next 
   time PIL is called. 
   this function should not be called in case PIL fail to load subsys and cause crash, 
   all data should be kept for dump and debugging.
   
   @param[in] Ctx    ptr to ctx data of PIL   
  
   @retval PIL_SUCCESS       success
   @retval other             failure
*/
PIL_STATUS PilCleanUp(PIL_SUBSYS_CTX_TYPE* Ctx)
{
    PIL_SUBSYS_PRIV_TYPE* Priv;
    
    ASSERT(Ctx != NULL);
    ASSERT_FAIL_PIL_RETURN(Ctx != NULL, PIL_INVALID_PARAM, PIL_INVALID_PARAM);
    ASSERT(NULL != (Priv = Ctx->Priv));
    ASSERT_FAIL_PIL_RETURN(Priv != NULL, PIL_INVALID_PARAM, PIL_INVALID_PARAM);

    if (Priv->MdtBuf != NULL) {
        UncachedFreePool(Priv->MdtBuf);
        Priv->MdtBuf = NULL;
    }
    
    if ((Priv->ElfLoadInfo).ElfBase != NULL)
    {
        FreePages((Priv->ElfLoadInfo).ElfBase, 
                  ALIGN_UP_SZ((Priv->ElfLoadInfo).ElfSize, EFI_PAGE_SIZE)/EFI_PAGE_SIZE);
        ZeroMem(&Priv->ElfLoadInfo, sizeof(PIL_ELF_LOAD_INFO_TYPE));
    }
    return PIL_SUCCESS;
}

