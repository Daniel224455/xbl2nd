/* ===========================================================================

                               C C I D r i v e r

                                   (CCICtrl)

D e f i n i t i o n s
*//** @file CCICtrl.c 
This file contains accessor and setter methods to CCI core 

@par EXTERNALIZED FUNCTIONS
None

@par INITIALIZATION AND SEQUENCING REQUIREMENTS
None

Copyright (c) 2014-2017 QUALCOMM, Qualcomm Technologies, Inc. All Rights Reserved.
Qualcomm Proprietary and Confidential.
Export of this technology or software is regulated by the U.S. Government.
Diversion contrary to U.S. law prohibited.

*//*======================================================================= */

/* ===========================================================================
**
** =========================================================================== */

/* ===========================================================================
**                         INCLUDE FILES FOR MODULE
**=========================================================================== */
#include "CCI_HAL_HWIO.h"
#include "CCI_HAL_Util.h"

void(* LoadDATAFncPtr)(CCIQUEUE CCIQueue, UINT32 ndata);

/* ===========================================================================
**                  Internal API Definitions 
** ======================================================================== */
/* ---------------------------------------------------------------------------
 *    FUNCTION        CCICtrl_LoadDATA
 *    DESCRIPTION     
 *    DEPENDENCIES    
 *    PARAMETERS
 *    RETURN VALUE    void
 *    SIDE EFFECTS    None
 * ------------------------------------------------------------------------ */
static void LoadData(CCIQUEUE CCIQueue, UINT32 data)
{
    CCIQueue->LOAD_DATA = data;
}

/* ===========================================================================
**                  External API Definitions 
** ======================================================================== */
/* ---------------------------------------------------------------------------
 *    FUNCTION        CCICtrlInit
 *    DESCRIPTION     Inits CCI Control Registers Struct
 *    DEPENDENCIES    
 *    PARAMETERS
 *    RETURN VALUE    void
 *    SIDE EFFECTS    None
 * ------------------------------------------------------------------------ */
void CCICtrlInit(CCICTX CCI, CCIHWDetaisType* CCIInfo)
{
    UINT8 i = 0; 
    UINT32 nOffset = 0;

    CCI->CCIBase =(UINT8*) 0x0AC4A000;

    /// initializes CCIHWCtrl registers
    CCI->CCICMNCTRL = (CCIHWCTRL) CCI->CCIBase;

    CCI->GLBL_IRQ = (CCIIRQGlblControlType)(CCI->CCIBase + CCIInfo->irq_base);

    nOffset = CCIInfo->irq_set_base;
    for (i = ProtocolMin; i < ProtocolMax; i++)
    {
        CCI->IRQ[i] = (CCIIRQCTRL)(CCI->CCIBase + nOffset);
        nOffset += CCIInfo->irq_set_offset;
    }

    nOffset = CCIInfo->sync_timer_base;
    for (i = 0; i < CCIInfo->max_num_of_sync_timers; i++)
    {
        CCI->SYNC_TIMER[i] = (CCISyncTimerType)(CCI->CCIBase + nOffset);
        nOffset += CCIInfo->sync_timer_offset;
    }
    
    /// initializes CCIMasterCtrl registers
    nOffset = CCIInfo->master_base;
    for (i = MasterMin; i != CCIInfo->max_num_of_master; i++)
    {
        CCI->MASTERS[i] = (CCIMASTER) (CCI->CCIBase + nOffset);
        nOffset += CCIInfo->master_offset;
    }
    
    nOffset = CCIInfo->queue_base;
    /// initializes CCIQueueCtrl registers
    for (i = 0; i != CCIInfo->max_num_of_queues; i++)
    {
        CCI->QUEUES[i] = (CCIQUEUE) (CCI->CCIBase + nOffset);
        nOffset += CCIInfo->queue_offset;
    }

    LoadDATAFncPtr = LoadData;
    CCI->bExit = FALSE;
}


/* ---------------------------------------------------------------------------
 *    FUNCTION        CCICtrl_Reset
 *    DESCRIPTION     
 *    DEPENDENCIES    
 *    PARAMETERS
 *    RETURN VALUE    void
 *    SIDE EFFECTS    None
 * ------------------------------------------------------------------------ */
void CCICtrl_Reset(CCICTX CCI, UINT32 cmd)
{
    CCI->CCICMNCTRL->RESET_CMD = cmd;
}

/* ---------------------------------------------------------------------------
 *    FUNCTION        CCIHWCtrl_QueueStart
 *    DESCRIPTION     
 *    DEPENDENCIES    
 *    PARAMETERS
 *    RETURN VALUE    void
 *    SIDE EFFECTS    None
 * ------------------------------------------------------------------------ */
void CCIHWCtrl_QueueStart(CCIHWCTRL CCI, UINT32 cmd)
{
    CCI->QUEUE_START = cmd;
}

/* ---------------------------------------------------------------------------
 *    FUNCTION        CCICtrl_SetSCL
 *    DESCRIPTION     
 *    DEPENDENCIES    
 *    PARAMETERS
 *    RETURN VALUE    void
 *    SIDE EFFECTS    None
 * ------------------------------------------------------------------------ */

void CCICtrl_SetSCL(CCICTX CCI, CCIMasterType masterId, UINT32 val)
{
    CCI->MASTERS[masterId]->SCL_CTL = val;
}

/* ---------------------------------------------------------------------------
 *    FUNCTION        CCICtrl_SetSDA0
 *    DESCRIPTION     Set I2C Control register0 
 *    DEPENDENCIES    
 *    PARAMETERS
 *    RETURN VALUE    void
 *    SIDE EFFECTS    None
 * ------------------------------------------------------------------------ */

void CCICtrl_SetSDA0(CCICTX CCI, CCIMasterType masterId, UINT32 val)
{
    CCI->MASTERS[masterId]->SDA_CTL_0 = val;
}

/* ---------------------------------------------------------------------------
 *    FUNCTION        CCICtrl_SetSDA1
 *    DESCRIPTION     Set I2C Control register1 
 *    DEPENDENCIES    
 *    PARAMETERS
 *    RETURN VALUE    void
 *    SIDE EFFECTS    None
 * ------------------------------------------------------------------------ */

void CCICtrl_SetSDA1(CCICTX CCI, CCIMasterType masterId, UINT32 val)
{
    CCI->MASTERS[masterId]->SDA_CTL_1 = val;
}

/* ---------------------------------------------------------------------------
 *    FUNCTION        CCICtrl_SetSDA2
 *    DESCRIPTION     Set I2C Control register2 
 *    DEPENDENCIES    
 *    PARAMETERS
 *    RETURN VALUE    void
 *    SIDE EFFECTS    None
 * ------------------------------------------------------------------------ */
void CCICtrl_SetSDA2(CCICTX CCI, CCIMasterType masterId, UINT32 val)
{
    CCI->MASTERS[masterId]->SDA_CTL_2 = val;
}
/* ---------------------------------------------------------------------------
 *    FUNCTION        CCICtrl_SetMiscCtl
 *    DESCRIPTION     
 *    DEPENDENCIES    
 *    PARAMETERS
 *    RETURN VALUE    void
 *    SIDE EFFECTS    None
 * ------------------------------------------------------------------------ */
void CCICtrl_SetMiscCtl(CCICTX CCI, CCIMasterType masterId, UINT32 val)
{
    CCI->MASTERS[masterId]->MISC_CTL = val;
}

/* ---------------------------------------------------------------------------
 *    FUNCTION        CCICtrl_GetI2CReadData
 *    DESCRIPTION     
 *    DEPENDENCIES    
 *    PARAMETERS
 *    RETURN VALUE    void
 *    SIDE EFFECTS    None
 * ------------------------------------------------------------------------ */
UINT32 CCICtrl_GetI2CReadData(CCICTX CCI, CCIMasterType masterId)
{
    return CCI->MASTERS[masterId]->READ_DATA;
}

/* ---------------------------------------------------------------------------
 *    FUNCTION        CCICtrl_GetI2CReadBufLevel
 *    DESCRIPTION     
 *    DEPENDENCIES    
 *    PARAMETERS
 *    RETURN VALUE    void
 *    SIDE EFFECTS    None
 * ------------------------------------------------------------------------ */
UINT32 CCICtrl_GetI2CReadBufLevel(CCICTX CCI, CCIMasterType masterId)
{
    return CCI->MASTERS[masterId]->READ_BUF_LEVEL;
}

/* ---------------------------------------------------------------------------
 *    FUNCTION        CCIQueueCtrl_SetExecWordCnt
 *    DESCRIPTION    
 *    DEPENDENCIES    
 *    PARAMETERS
 *    RETURN VALUE    void
 *    SIDE EFFECTS    None
 * ------------------------------------------------------------------------ */
void CCIQueueCtrl_SetExecWordCnt(CCIQUEUE CCIQueue, UINT32 cnts)
{
    CCIQueue->EXEC_WORD_CNT = cnts;
}

/* ---------------------------------------------------------------------------
 *    FUNCTION        CCIQueueCtrl_LoadDATA
 *    DESCRIPTION     
 *    DEPENDENCIES    
 *    PARAMETERS
 *    RETURN VALUE    void
 *    SIDE EFFECTS    None
 * ------------------------------------------------------------------------ */
void CCIQueueCtrl_LoadDATA(CCIQUEUE CCIQueue, UINT32 data)
{
    LoadDATAFncPtr(CCIQueue, data);
}
/* ---------------------------------------------------------------------------
 *    FUNCTION        CCICtrl_DoInterruptGlobalClear
 *    DESCRIPTION     
 *    DEPENDENCIES    
 *    PARAMETERS
 *    RETURN VALUE    void
 *    SIDE EFFECTS    None
 * ------------------------------------------------------------------------ */
 
void CCICtrl_DoInterruptGlobalClear(CCICTX CCI)
{
    CCI->GLBL_IRQ->GLOBAL_CLEAR_CMD = 0x1;
}

/* ---------------------------------------------------------------------------
 *    FUNCTION        CCICtrl_SetMaskForInterrupt
 *    DESCRIPTION      
 *    DEPENDENCIES    
 *    PARAMETERS
 *    RETURN VALUE    void
 *    SIDE EFFECTS    None
 * ------------------------------------------------------------------------ */
 
void CCICtrl_SetMaskForInterrupt(CCICTX CCI, CCIProtocolType protocol, UINT32 mask)
{
    CCI->IRQ[protocol]->MASK = mask;
}

/* ---------------------------------------------------------------------------
 *    FUNCTION        CCICtrl_ClearInterruptStatus
 *    DESCRIPTION     Clears interrupt status 
 *    DEPENDENCIES    
 *    PARAMETERS
 *    RETURN VALUE    void
 *    SIDE EFFECTS    None
 * ------------------------------------------------------------------------ */
 
void CCICtrl_ClearInterruptStatus(CCICTX CCI, CCIProtocolType protocol, UINT32 val)
{
    CCI->IRQ[protocol]->CLEAR = val;
}

/* ---------------------------------------------------------------------------
 *    FUNCTION        CCICtrl_GetInterruptStatus
 *    DESCRIPTION     Read Interrupt Status 
 *    DEPENDENCIES    
 *    PARAMETERS
 *    RETURN VALUE    void
 *    SIDE EFFECTS    None
 * ------------------------------------------------------------------------ */
 
UINT32 CCICtrl_GetInterruptStatus(CCICTX CCI, CCIProtocolType protocol)
{
    return CCI->IRQ[protocol]->STATUS;
}
