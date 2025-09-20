/*===============================================================================
  Copyright (c) 2017-2018, 2021 Qualcomm Technologies, Inc.  All Rights Reserved.
  
  FILE:         pil_tz.c
  DESCRIPTION:    
  
  REVISION HISTORY

  when       who         what, where, why
  --------   ---         --------------------------------------------------------
  10/08/18   yg          Add lock to make thread safe
  06/14/17   yw          initial version

================================================================================*/
#include<pil_tz.h>
#include<pil_log.h>
#include<pil_loader.h>
#include<pil_config.h>
#include<Library/BaseLib.h>
#include<Library/UefiLib.h>
#include<Protocol/EFIClock.h>
#include<Library/QcomLib.h>
#include<Library/PrintLib.h>
#include<Library/TimerLib.h>
#include<Library/QcomBaseLib.h>
#include<Library/EfiFileLib.h>
#include<Library/BaseMemoryLib.h>
#include<Library/QcomTargetLib.h>
#include<Library/MemoryAllocationLib.h>
#include<Library/UefiBootServicesTableLib.h>
#include<Library/UncachedMemoryAllocationLib.h>
#include <Library/LockLib.h>

#define IS_ALPHABET(c)  (((c)>='a'&&(c)<='z')||((c)>='A'&&(c)<='Z')||((c)=='_'))

/*
    RetailList[] contains the list of SubsysID which is allowed 
    to be processed/loaded by PIL in retail product
*/
extern UINT32     RetailImageNum;
extern PIL_SUBSYS_CFG_TYPE*   RetailList[];

UINT64 StorageSpentCnt, AuthSpentUs, FvProcSpentCnt;

const PIL_METHODS_TYPE PilSplitElf = 
{
    .LoadMetaData = PilLoadMdtFile,
    .ValidateMetaData = PilValidateMetadata,
    .SetupMemoryRange = PilSetupMemoryRange,
    .LoadSegments = PilLoadSegments,
    .AuthAndReset = PilAuthAndReset,
    .PostLoad = PilPostLoad,
};

const PIL_METHODS_TYPE PilSingleElf = 
{
    .LoadEntireImage = PilLoadElfFile,
    .LoadMetaData = CopyMetaDataFromLoadedElf,
    .ValidateMetaData = PilValidateMetadata,
    .SetupMemoryRange = PilSetupMemoryRange,
    .LoadSegments = PilLoadSegments,
    .AuthAndReset = PilAuthAndReset,
    .PostLoad = PilPostLoad,
};

const PIL_METHODS_TYPE PilFv = 
{
    .LoadEntireImage = PilLoadElfFile,
    .LoadMetaData = CopyMetaDataFromLoadedElf,
    .ValidateMetaData = PilValidateMetadata,
    .SetupMemoryRange = PilSetupMemoryRange,
    .AuthAndReset = PilAuthAndReset,
    .PostLoad = PilPostLoad,
};

/**
  load and boot image by giving the subsys configure
     
  @param[in] cfg            configure of subsys to process/load  
  
  @retval PIL_SUCCESS       success
  @retval other             failurel

**/
PIL_STATUS PilProcessImageExt(PIL_SUBSYS_CFG_TYPE* CfgIn)
{
    EFI_STATUS  Status   = EFI_DEVICE_ERROR;
    PIL_SUBSYS_CTX_TYPE* Ctx;
    PIL_SUBSYS_CFG_TYPE* Cfg;
    PIL_SUBSYS_PRIV_TYPE* Priv;
    UINT64 LoadStart, LoadEnd;
    PIL_STATUS PilRet = PIL_SUCCESS;
    UINT32    Idx;
    const PIL_METHODS_TYPE*  PilMethod;
    
    StorageSpentCnt = AuthSpentUs = FvProcSpentCnt = 0;
    
    /* we need unique FwName to identify subsys, consider different
       subsys could have same ImagePath. */
    if (0 == CfgIn->FwName[0]) {
        PIL_ERROR("Need unique FwName\n");
        return PIL_INVALID_PARAM;
    }
    
    LoadStart = GetPerformanceCounter();
    
    /* look for cfg with the given name in cfg list, cfg file, and variables */
    Cfg = PilInitCfgByUnicodeName(CfgIn->FwName);
    if (Cfg != NULL) {
        /* found a cfg with the given name. if the founded cfg ptr is same as 
           the paramter cfg ptr, it means the caller is PilProcessImage(), just 
           go ahead using it, otherwise we have two cfgs with same name, confusing */
        if (Cfg != CfgIn) {
            PIL_ERROR_CFG(Cfg, "FwName or subsys name must be unique\n");
            return PIL_INVALID_PARAM;
        }
    }
    else {
        /* did not find cfg with given name, this subsys is not in cfg file or variables,
           need to duplicate the cfg in cfg list */
        Cfg = PilDuplicateCfg(CfgIn);
    }
        
    if (RETAIL) {
        for (Idx = 0; Idx<RetailImageNum; Idx++) {
            if (RetailList[Idx] == Cfg)
                break;
        }
        if (Idx == RetailImageNum) {
            PIL_ERROR_CFG(Cfg, "not supported in retail\n");
            return PIL_SUBSYS_NOT_SUPPORTED;
        }
    }

    PilRet = PilInitSubsys(Cfg, &Ctx);
    if (PilRet != PIL_SUCCESS)
    {
        if (PilRet == PIL_SUBSYS_RUNNING) {
            PIL_WARN_CFG(Cfg, "subsys is in running\n");
            return PIL_SUBSYS_RUNNING;
        }
        else if (PilRet == PIL_SUBSYS_EXIST) {
            PIL_WARN_CFG(Cfg, "Found ctx eists, continue to load\n");
        }
        else  {
            PIL_ERROR_CFG(Cfg, "Failed to create ctx\n");
            goto LoadErr;
        }
    }

    Cfg = Ctx->Cfg;
    Priv = Ctx->Priv;
    
    switch(Cfg->Type)
    {
        case ELF_FV:
            PilMethod = &PilFv;
            break;
        case ELF_SINGLE:
            PilMethod = &PilSingleElf;
            break;
        case ELF_SPLIT:
            PilMethod = &PilSplitElf;
            break;
        default:
            PIL_ERROR_CFG(Cfg, "%r is not supported\n", Cfg->Type);
            return PIL_SUBSYS_NOT_SUPPORTED;
    }

    if (Cfg->Type == ELF_FV) {
        PIL_INFO_CFG(Cfg, "Skip to mount FV partition\n");
    }
    else if (Cfg->Type == ELF_SPLIT || Cfg->Type == ELF_SINGLE) {
        EFI_GUID* RootGuid = IsZeroGuid((CONST GUID*)&Cfg->PartiRootGuid)?NULL:&Cfg->PartiRootGuid;
        EFI_GUID* PartiGuid = IsZeroGuid((CONST GUID*)&Cfg->PartiGuid)?NULL:&Cfg->PartiGuid;
        CHAR16*   PartiLabel = (PartiGuid == NULL)?Cfg->PartiLabel:NULL;
        Status = MountFatPartitionExt(RootGuid, PartiGuid, PartiLabel);
        
        if (EFI_ERROR(Status)) {
            PIL_ERROR_CFG(Cfg, "Mount partition return %r\n", Status);
            //return PIL_LOAD_ERR;
        } else {
            PIL_INFO_CFG(Cfg, "Mount partition return %r\n", Status);
        }
    }
    else {
        PIL_ERROR_CFG(Cfg, "No support type %r\n", Cfg->Type);
        return PIL_SUBSYS_NOT_SUPPORTED;
    }
    
    
    
    // load ELF file into memory for non-split image
    if (PilMethod->LoadEntireImage)
    {
        PilRet = PilMethod->LoadEntireImage(Ctx);
        if (PilRet != PIL_SUCCESS) {
            PIL_ERROR_CFG(Cfg, "Failed to load ELF file\n");
            goto LoadErr;
        }
    }

    // load metadata into memory
    if (PilMethod->LoadMetaData)
    {
        PilRet = PilMethod->LoadMetaData(Ctx);
        if (PilRet != PIL_SUCCESS) {
            PIL_ERROR_CFG(Cfg, "Failed to load metadata\n");
            goto LoadErr;
        }
    }
    else {
        PIL_ERROR_CFG(Cfg, "LoadMetaData cannot be NULL\n");
        PilRet = PIL_GENERIC_ERR;
        goto LoadErr;
    }

    // validate metadata including ELF and prgram headers, and hash segment
    if (PilMethod->ValidateMetaData)
    {
        PilRet = PilMethod->ValidateMetaData(Ctx);
        if (PilRet != PIL_SUCCESS) {
            PIL_ERROR_CFG(Cfg, "Failed to validate metadata\n");
            goto LoadErr;
        }
    }
    else {
        PIL_ERROR_CFG(Cfg, "ValidateMetaData cannot be NULL\n");
        PilRet = PIL_GENERIC_ERR;
        goto LoadErr;
    }
    
    // setup memory range and notify hyp&tz PIL service 
    if (PilMethod->SetupMemoryRange)
    {
        PilRet = PilMethod->SetupMemoryRange(Ctx);
        if (PilRet != PIL_SUCCESS) {
            PIL_ERROR_CFG(Cfg, "Failed to setup memory range\n");
            goto LoadErr;
        }
    }
    else {
        PIL_ERROR_CFG(Cfg, "SetupMemoryRange cannot be NULL\n");
        PilRet = PIL_GENERIC_ERR;
        goto LoadErr;
    }
    
    // load segments, not required for FV image
    if (PilMethod->LoadSegments)
    {
        PilRet = PilMethod->LoadSegments(Ctx);
        if (PilRet != PIL_SUCCESS) {
            PIL_ERROR_CFG(Cfg, "Failed to load segments\n");
            goto LoadErr;
        }
    }
    
    // bring subsys out of reset
    if (PilMethod->AuthAndReset)
    {
        PilRet = PilMethod->AuthAndReset(Ctx);
        if (PilRet != PIL_SUCCESS) {
            PIL_ERROR_CFG(Cfg, "Failed to bring proc out of reset\n");
            goto LoadErr;
        }
    }
    else {
        PIL_ERROR_CFG(Cfg, "AuthAndReset cannot be NULL\n");
        PilRet = PIL_GENERIC_ERR;
        goto LoadErr;
    }

    /* post load operations, including write loading info in IMEM, 
       unlcok is required, and process FV image
    */
    if (PilMethod->PostLoad)
    {
        PilRet = PilMethod->PostLoad(Ctx);
        if (PilRet != PIL_SUCCESS) {
            PIL_ERROR_CFG(Cfg, "Failed to do post-load operations\n");
            goto LoadErr;
        }
    }
    
    /* update subsys status */
    Priv->ProcStatus = PIL_PROC_RUNNING;
    
    LoadEnd = GetPerformanceCounter();
    for (Idx=0; Idx<Priv->SmcCnt; Idx++)
        AuthSpentUs += Priv->SmcBenchMarkus[Idx].Timeus;
    
    PIL_WARN_CFG(Cfg, "Done t=%dms s=%dms a=%dms f=%dms\n", ConvertTimerCountms(LoadEnd - LoadStart),
                 ConvertTimerCountms(StorageSpentCnt), DivU64x32(AuthSpentUs, 1000),
                 ConvertTimerCountms(FvProcSpentCnt));
    //PilCleanUp(Ctx);
    
    return PIL_SUCCESS;
LoadErr:
    return PilRet;
}


/**
  load and boot image by giving the subsys name
     
  @param[in] Subsys            name of subsys to process/load  
  
  @retval PIL_SUCCESS       success
  @retval other             failurel

**/
PIL_STATUS PilProcessImage(CHAR16* Subsys)
{
    PIL_STATUS  PStatus;
    PIL_SUBSYS_CFG_TYPE* Cfg;
    VOID* PilLock;
    
    if (!Subsys)
        return PIL_INVALID_PARAM;
    
    PilLock = InitLock ("PIL_Lock");
    Lock (PilLock);
    
    Cfg = PilInitCfgByUnicodeName(Subsys);
    if (Cfg == NULL) {
        PIL_WARN("No configuration found for %s\n", Subsys);
        PStatus = PIL_SUBSYS_NOT_SUPPORTED;
    }
    else
      PStatus = PilProcessImageExt(Cfg);
        
    UnLock (PilLock);
    return PStatus;
}

