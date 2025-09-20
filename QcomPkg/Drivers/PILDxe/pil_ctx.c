/*===============================================================================
  Copyright (c) 2018 Qualcomm Technologies, Inc.  All Rights Reserved.
  
  FILE:         pil_ctx.c
  DESCRIPTION:    
  
  REVISION HISTORY
  when       who         what, where, why
  --------   ---         --------------------------------------------------------
  03/12/18   yw         initial version

================================================================================*/

#include<pil_ctx.h>
#include<Library/MemoryAllocationLib.h>
#include<Library/UncachedMemoryAllocationLib.h>

/* number of valid subsys in the subsys array*/
STATIC UINT32 PilSubsysCtxNum = 0;
/* array to save ctx of all subsys */
STATIC PIL_SUBSYS_CTX_TYPE PilSubsysCtxList[PIL_SUBSYS_MAX_NUM] = { {0} }; 

/**
  search for subsys in ctx array, assume the input cfg is already in cfg list 
     
  @param[in] cfg           configure of the subsys to be found  
  
  @retval PIL_SUCCESS       success
  @retval other             failure

**/

PIL_SUBSYS_CTX_TYPE* LookUpSubsysCtx(PIL_SUBSYS_CFG_TYPE* Cfg)
{
    UINT32 i;
    PIL_SUBSYS_CTX_TYPE* Ctx = NULL;
    
    for (i=0; i<PilSubsysCtxNum; i++)
    {
        if (NULL != PilSubsysCtxList[i].Cfg)
        {
            if (PilSubsysCtxList[i].Cfg == Cfg)
            {
                Ctx = &PilSubsysCtxList[i];
                break;
            }
        }
    }
    return Ctx;
}



/**
  alloc and init data for subsys  

  @param[in] CfgIn            configure of subsys to be initilized    
  @param[out] CtxOut        return the ctx if found, create new one
  
  @retval PIL_SUCCESS                   ctx created
  @retval PIL_SUBSYS_RUNNING            ctx exists and subsys is in running status
  @retval PIL_SUBSYS_EXIST                ctx exusts and subsys is in shutdown status
  @retval PIL_OUT_OF_MEMORY                memory allocation fails

**/

PIL_STATUS PilInitSubsys(PIL_SUBSYS_CFG_TYPE* CfgIn, PIL_SUBSYS_CTX_TYPE** CtxOut)
{
    PIL_SUBSYS_CTX_TYPE* Ctx;
    PIL_SUBSYS_PRIV_TYPE* Priv;
    
    /* subsys ctx already created */
    if (NULL != (Ctx = LookUpSubsysCtx(CfgIn))) 
    {
        if (Ctx->Cfg != CfgIn) {
            PIL_ERROR_CFG(CfgIn, "Found cfg invalid\n");
            ASSERT(0);
            ASSERT_FAIL_PIL_RETURN(0, PIL_INVALID_CFG, PIL_INVALID_CFG);
        }
        
        if ((Priv = Ctx->Priv) != NULL) 
        {
            if (Priv->ProcStatus == PIL_PROC_RUNNING) {
                // do bothing if the subsys is in running status
			    *CtxOut = Ctx;
                return PIL_SUBSYS_RUNNING;
			}
            else {
                /* if somehow the private buffer is allocated but subsys in down, 
                we need to reset the private data for rebooting */
                // remove saved Phdr if there is valid buffer
                if (NULL!=Priv->Phdr)
                {
                    FreePool(Priv->Phdr);
                    Priv->Phdr = NULL;
                }
                
                // remove saved Ehdr if there is valid buffer
                if (NULL!=Priv->Ehdr) {
                    FreePool(Priv->Ehdr);
                    Priv->Ehdr = NULL;
                }
                
                // free mdt buffer if buffer allocated already
                if (NULL!=Priv->MdtBuf) {
                    UncachedFreePool(Priv->MdtBuf);
                    Priv->MdtBuf = NULL;
                }
				*CtxOut = Ctx;
                return PIL_SUBSYS_EXIST;
            }
        }
    }
    else {
        // create new ctx
        Ctx = &PilSubsysCtxList[PilSubsysCtxNum++];
        Ctx->Cfg = CfgIn;
    }
    
    // allocate buffer for private data
    Priv = AllocateZeroPool(sizeof(PIL_SUBSYS_PRIV_TYPE));
    if (Priv == NULL) 
        return PIL_OUT_OF_MEMORY;
    Ctx->Priv = Priv;
    
    *CtxOut = Ctx;
    return PIL_SUCCESS;
}


/**
  remove ctx data for subsys  

  @param[in] CfgIn            configure of subsys to be removed    
  
  @retval PIL_SUCCESS       success
  @retval other             failurel

**/
#if 0
PIL_STATUS PilDeinitSubsys(PIL_SUBSYS_CFG_TYPE* CfgIn)
{
    PIL_SUBSYS_CTX_TYPE* Ctx = NULL;
    uint32 i;
    
    if (NULL == (Ctx = FindSubsysCtx(CfgIn)))
        return PIL_SUBSYS_NOT_FOUND;

    ASSERT(Ctx->Priv != NULL && Ctx->Cfg != NULL);
    ASSERT_FAIL_PIL_RETURN(Ctx->Priv != NULL && Ctx->Cfg != NULL, PIL_INVALID_PARAM, PIL_INVALID_PARAM);
    FreePool(Ctx->Priv);
    FreePool(Ctx->Cfg);
    
    for (i=0; i<PIL_SUBSYS_MAX_NUM; i++) {
        if (PILSubsysCtxList[i] == Ctx) {
            PILSubsysCtxList[i] = NULL;
            break;
        }
    }
    FreePool(Ctx);
    return PIL_SUCCESS;
}
#endif
