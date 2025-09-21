 /* ===========================================================================

                                 C C I L o a d e r

                                    (CCILoader)

                               D e f i n i t i o n s
 *//** @file CCILoader.c                                                                                                                                                                                          
 This file contains CCI Loader implementation for CCI driver.                                                                                                                                                                                                                       
                                                                                                                                                                                                                                                                                                    
 @par EXTERNALIZED FUNCTIONS                                                                                                                                                                                           
 None                                                                                                                                                                                                                                                                                               
                                                                                                                                                                                                                                                                                                    
 @par INITIALIZATION AND SEQUENCING REQUIREMENTS                                                                                                                                                                       
 None                                                                                                                                                                                                                                                                                               
                                                                                                                                                                                                                                                                                                    
 Copyright (c) 2014 - 2017 Qualcomm Technologies, Inc. All Rights Reserved.                                                                                                                                                  
 Qualcomm Proprietary and Confidential.                                                                                                                                                                                                                                                             
 Export of this technology or software is regulated by the U.S. Government.                                                                                                                                                                                                                         
 Diversion contrary to U.S. law prohibited.                                                                                                                                                                                                                                                         
                                                                                                                                                                                                                                                                                                    
** =========================================================================== */                                                                                                                                        
                                                                                                                                                                                                                                                                                                   
/* ===========================================================================                                                                                                                                        
**                                                                                                                                                                                                                                                                                                 
** =========================================================================== */                                                                                                                                                                                                                  
                                                                                                                                                                                                                                                                                                   
/* ===========================================================================                                                                                                                                        
**                         INCLUDE FILES FOR MODULE                                                                                                                                                                                                                                                
**============================================================================ */                                                                                                                                                                                          

#include "CCI_HAL_HWIO.h"
#include "CCICmd.h"
#include "CCILoader.h"
#include "CCI_HAL_Util.h"
#include "CCI_HAL.h"
#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>

/* ---------------------------------------------------------------------------
 *    FUNCTION        CCILoader_Init
 *    DESCRIPTION     Initialize Loader
 *    DEPENDENCIES    
 *    PARAMETERS      
 *    RETURN VALUE    None
 *    SIDE EFFECTS    None
 * ------------------------------------------------------------------------ */
CCIStatus CCILoader_Init(CCILoader* self, CCIResourceType* resource, CCIResourceConfig* config)
{
    self->nSz = 0;
    self->bAutoRunEnabled = (CCILD_AUTO == config->eautorun) ? TRUE: FALSE;
    self->signal = config->signal;
    self->irq = config->irq;
    self->reportcnts = 0;
    self->queueInfo = resource->queueInfo;
    self->CCI = resource->Queue;
    self->CCI_CTRL_REG = CCI_HAL_Get_CTRLReg();
    self->state = CCILD_State_Unset;
    self->nCapacity = self->queueInfo->maxQueueDepth;
    self->nFutureCapacity = 0;
    self->nLogicalCapacity = self->nCapacity / 2;
    self->nPresetLogicalCapacity = self->nLogicalCapacity;
    return CCI_Status_OK;
}
/* ---------------------------------------------------------------------------
 *    FUNCTION        CCILoader_Deinit
 *    DESCRIPTION     De-initialize Loader
 *    DEPENDENCIES    
 *    PARAMETERS      
 *    RETURN VALUE    None
 *    SIDE EFFECTS    None
 * ------------------------------------------------------------------------ */
CCIStatus CCILoader_Deinit(CCILoader* self) 
{
    self->nSz = 0;
    self->bAutoRunEnabled = FALSE;
    self->reportcnts = 0;
    self->signal = signal_max;
    self->irq = 0;
    self->queueInfo = NULL;
    self->CCI = NULL;
    self->CCI_CTRL_REG = NULL;
    self->state = CCILD_State_Unset;
    self->nCapacity = 0;
    self->nFutureCapacity = 0;
    self->nLogicalCapacity = 0;
    self->nPresetLogicalCapacity = 0;
    return CCI_Status_OK;
}

/* ---------------------------------------------------------------------------
 *    FUNCTION        CCILoader_Unload
 *    DESCRIPTION     Unload contents of queues upto report command 
 *    DEPENDENCIES    report command must be present 
 *    PARAMETERS      
 *    RETURN VALUE    None
 *    SIDE EFFECTS    None
 * -------------------------------------------------------------------------- */
static void CCILoader_Unload(CCILoader* self, CCIStatus *eStatus)
{
    *eStatus = CCI_Status_OK;
    INT8 nTimeout = 20;

    if (!self)
    {
        *eStatus = CCI_Status_NULL_PTR;
        return;
    }

    // if empty return
    if (!self->nSz)
        return;

    /// if there are report command loaded or irq triggers by outside source
    if(self->reportcnts || 0 == self->irq)
    {
        CCIQueueCtrl_SetExecWordCnt(self->CCI, (UINT32) self->nSz);

        self->nSz = 0;

        /// this is o.k, but eventually, mutex is needed
        CCIHWCtrl_QueueStart(self->CCI_CTRL_REG, self->queueInfo->queue_start_id);

        //poll Here
        while(self->CCI->CUR_WORD_CNT && nTimeout--){
           gBS->Stall(75000);

        }
        if (nTimeout < 0 ) {
           *eStatus = CCI_Status_BAD_STATE;  
        }
        if(self->reportcnts)
           --self->reportcnts;
    } 
}
BOOLEAN ccicmd_get_next_token(CCICmd* self, UINT32* buffer)
{ 
    if (self->idx >= self->nSz)
        return FALSE;
    *buffer = self->ccmd[self->idx++];
    return TRUE;
}

/* ---------------------------------------------------------------------------
 *    FUNCTION        CCILoader_Load
 *    DESCRIPTION     Loads one CCI command. CCI command size is either 1 or 3.
 *    DEPENDENCIES    
 *    PARAMETERS      eCCICmdSz is the size of CCI cmd
 *    RETURN VALUE    None
 *    SIDE EFFECTS    None
 * ------------------------------------------------------------------------ */
CCIStatus CCILoader_Load(CCILoader* self, CCICmd* cmd, CCIStatus *eStatus)
{
    UINTN nSz = 0;
    UINT32 command_token = 0;

    if (!self || !cmd) 
        return *eStatus =CCI_Status_GENERAL_ERROR;

    // get the size of cmd
    ccicmd_get_size(cmd, &nSz);

    // check if the current request can be honored
    if (!self->bAutoRunEnabled && self->nCapacity < self->nSz + nSz)
        return *eStatus = CCI_Status_BAD_PARAM;

    self->state = CCILD_State_Unset;

    while (ccicmd_get_next_token(cmd, &command_token))
    { 
        CCIQueueCtrl_LoadDATA(self->CCI, command_token); 
        ++self->nSz;
    } 

    return *eStatus = CCI_Status_OK;
}

/* ---------------------------------------------------------------------------
 *    FUNCTION        CCILoader_UnloadAll
 *    DESCRIPTION     
 *    DEPENDENCIES    
 *    PARAMETERS      
 *    RETURN VALUE    None
 *    SIDE EFFECTS    None
 * -------------------------------------------------------------------------- */
CCIStatus CCILoader_UnloadAll(CCILoader* self, CCIStatus *eStatus)
{
    CCILoader_Unload(self, eStatus);

    if (*eStatus)
        return *eStatus;

    if (CCI_Status_OK == *eStatus)
        self->state = CCILD_State_Complete;

    return *eStatus;
}

/* ---------------------------------------------------------------------------
 *    FUNCTION        CCILoader_GetState
 *    DESCRIPTION     
 *    DEPENDENCIES    
 *    PARAMETERS      
 *    RETURN VALUE    None
 *    SIDE EFFECTS    None
 * -------------------------------------------------------------------------- */
CCILdState CCILoader_GetState(CCILoader* self)
{
    return self->state;
}

/* ---------------------------------------------------------------------------
 *    FUNCTION        CCILoader_CheckAvailabilty
 *    DESCRIPTION     
 *    DEPENDENCIES    
 *    PARAMETERS      
 *    RETURN VALUE    None
 *    SIDE EFFECTS    None
 * -------------------------------------------------------------------------- */
UINTN CCILoader_CheckAvailabilty(CCILoader* self)
{
    return self->nCapacity - self->nSz;
}
