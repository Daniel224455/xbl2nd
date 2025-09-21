 /* ===========================================================================

                                 C C I D r i v e r

                                    (CCIResourceManager)

                               D e f i n i t i o n s
 *//** @file CCIResourceManager.c                                                                                                                                                                                          
 This file contains CCIResourceManager implementation
                                                                                                                                                                                                                                                                                                    
 @par EXTERNALIZED FUNCTIONS                                                                                                                                                                                           
 None                                                                                                                                                                                                                                                                                               
                                                                                                                                                                                                                                                                                                    
 @par INITIALIZATION AND SEQUENCING REQUIREMENTS                                                                                                                                                                       
 None                                                                                                                                                                                                                                                                                               
                                                                                                                                                                                                                                                                                                    
 Copyright (c) 2014-2017 Qualcomm Technologies, Incorporated. All Rights Reserved.                                                                                                                                                  
 Qualcomm Proprietary and Confidential.                                                                                                                                                                                                                                                             
 Export of this technology or software is regulated by the U.S. Government.                                                                                                                                                                                                                         
 Diversion contrary to U.S. law prohibited.                                   */                                                                                                                                                                                                                      
/* ===========================================================================                                                                                                                                        
**                         INCLUDE FILES FOR MODULE                                                                                                                                                                                                                                                
**=========================================================================== */                                                                                                                                                                                          
#include "CCIResourceManager.h"
#include "CCILoader.h"
#include "CCI_HAL.h"
#include "CameraPlatform.h"
#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>


/* ---------------------------------------------------------------------------                                                                                                                                                                                                                   
** Local Object Definitions                                                                                                                                                                                                                                                                    
** ------------------------------------------------------------------------ */                                                                                                                                                                                                                   
static struct _CCI CCICtxMemmory;
static CCICTX CCI = &CCICtxMemmory;

static CCIHWDetaisType CCIInfo = 
    { 
        #include "CCI_HAL_PLATFORM_HWIO.h"
    };

const static CCIReportCmdType reports[] = 
{
    /// place holder for callback
    { { .id = CCI_HAL_CMD_I2C_REPORT, .report_id = 0x0, .irq_enable = 0x1, .capture = 0x1 } },
    /// I2C Write
    { { .id = CCI_HAL_CMD_I2C_REPORT, .report_id = CAMSENSOR_MX_CALLBACK + 1, .irq_enable = 0x1, .capture = 0x0 } },
    /// Async I2C Write
    { { .id = CCI_HAL_CMD_I2C_REPORT, .report_id = CAMSENSOR_MX_CALLBACK + 2, .irq_enable = 0x1, .capture = 0x0 } },
    /// Async Queue Operation
    { { .id = CCI_HAL_CMD_I2C_REPORT, .report_id = CAMSENSOR_MX_CALLBACK + 0, .irq_enable = 0x1, .capture = 0x1 } },
    /// Async I2C Callback
    { { .id = CCI_HAL_CMD_GPIO_REPORT,.report_id = CAMSENSOR_ASYNC_I2C_CALLBACK, .irq_enable = 0x1, .capture = 0x1 } },
    /// GPIO Write 
    { { .id = CCI_HAL_CMD_GPIO_REPORT,.report_id = CAMSENSOR_MX_CALLBACK + 3, .irq_enable = 0x1, .capture = 0x0 } },
};
/* ===========================================================================
**                  Internal API Definitions 
** ======================================================================== */
/* ---------------------------------------------------------------------------
 *    FUNCTION        CCIRsm_AssignQueueId
 *    DESCRIPTION     Return queueId per operation 
 *    DEPENDENCIES    None                                           
 *    PARAMETERS      master and opertion defines type of resources user wants 
 *    RETURN VALUE    CCIStatus 
 *    SIDE EFFECTS    
 * ------------------------------------------------------------------------ */
static CCIQueueType CCIRsm_AssignQueueId(operationtype op, CCIMasterType master)
{
    switch(op)
    {
        case i2c_async_write:
            return (master == m0)? m0q0: m1q0;
        case i2c_read:
        case i2c_write:
        case i2c_queued_write:
            return (master == m0)? m0q1: m1q1;
        case i2c_async_irq:
            return gpioq2;
        case on_gpio_in:
            if (master == gpio)
                return gpioq0;
            else
                return (master == m0)? m0q0: m1q0;
        case gpio_write:
            return gpioq0;
        default: 
           break;
    }
    return queue_max;
}

/* ---------------------------------------------------------------------------
 *    FUNCTION        CCIRsm_InquireCapability
 *    DESCRIPTION     Return CCI Capability 
 *    DEPENDENCIES    Resouce has to be acquired first 
 *    PARAMETERS              
 *    RETURN VALUE    CCIStatus 
 *    SIDE EFFECTS    
 * ------------------------------------------------------------------------ */
static void CCIRsm_InquireCapability(CCIRsm* self, CCIResourceType* resource)
{
    resource->capability.mx_cid = self->CCIInfo->mx_cid; 
    resource->capability.bContinousSupported = self->CCIInfo->bContinousModeSupported;
    resource->capability.mx_write_trnsf_unt = (INT16) self->CCIInfo->mx_write_trnsf_unt;
    resource->capability.mx_pltfm_write_trnsf_unt = (INT16) self->CCIInfo->mx_pltfm_write_trnsf_unt;
    resource->capability.mx_read_trnsf_unt = (INT16) self->CCIInfo->mx_read_fifo_len;
    resource->capability.cciclkfrq = self->nCurrentCCIClkFrq;
    resource->capability.mx_burt_write_transf_capability = (resource->queueInfo->maxQueueDepth / 3) *
    resource->capability.mx_write_trnsf_unt; 
    resource->capability.gpio_in_gpio_queue   = CCIRsm_AssignQueueId(on_gpio_in, gpio);
    resource->capability.gpio_in_m0_i2c_queue = CCIRsm_AssignQueueId(on_gpio_in, m0);
    resource->capability.gpio_in_m1_i2c_queue = CCIRsm_AssignQueueId(on_gpio_in, m1);
}

/* ===========================================================================
**                  External API Definitions
** ======================================================================== */
/* ---------------------------------------------------------------------------
*    FUNCTION        CCIRsm_CtrlInit
*    DESCRIPTION     Inits CCI Control Registers Struct
*    DEPENDENCIES
*    PARAMETERS
*    RETURN VALUE    void
*    SIDE EFFECTS    None
* ------------------------------------------------------------------------ */
static void CCIRsm_CtrlInit(CCIRsm* self)
{
    CCICtrlInit(CCI, self->CCIInfo);
}
/* ---------------------------------------------------------------------------
 *    FUNCTION        CCIRsm_SetCCIInfo
 *    DESCRIPTION     Set Platform specific info regarding CCI
 *    DEPENDENCIES    None                                           
 *    PARAMETERS      Pointer to CCI Resource Manager Object
 *    RETURN VALUE    None
 *    SIDE EFFECTS    
 * ------------------------------------------------------------------------ */
static void CCIRsm_SetCCIInfo(CCIRsm* self)
{
    self->CCIInfo = &CCIInfo;
}

/* ===========================================================================
                        EXTERNAL API DECLARATIONS
=========================================================================== */
/* ---------------------------------------------------------------------------
 *    FUNCTION        CCIRsm_Init
 *    DESCRIPTION     Initializes CCI Resource Manager
 *    DEPENDENCIES    None                                           
 *    PARAMETERS      Pointer to CCI Resource Manager Object
 *    RETURN VALUE    None
 *    SIDE EFFECTS    
 * ------------------------------------------------------------------------ */
void CCIRsm_Init(CCIRsm* self)
{ 
    
    UINT32 irq0 = 0;

    self->chipId = SDM845;
    self->nCurrentCCIClkFrq = CCIClkFrqVal_Normal;
    self->nfrequency_data[CCIClkFrqIdx_Normal] = CCIClkFrqVal_Normal;
    self->nfrequency_data[CCIClkFrqIdx_Turbo] = CCIClkFrqVal_Turbo;
    CCIRsm_SetCCIInfo(self);
    self->usageMap = 0;
    self->mastersBusFrq = 0;
    CCIRsm_CtrlInit(self);
    CCICtrl_SetMaskForInterrupt(CCI, i2c_protocol, irq_rst_done_ack);
    CCICtrl_Reset(CCI, 0x0FFFFFFF); 
    gBS->Stall(100000);
    irq0 = CCICtrl_GetInterruptStatus(CCI, i2c_protocol);
        

    // chekc if reset happened
    if(irq_rst_done_ack & irq0)
    {
        CCICtrl_ClearInterruptStatus(CCI, i2c_protocol, irq0);
        CCICtrl_DoInterruptGlobalClear(CCI);
    }
    else {
        AsciiPrint( "  CCI Reset failed\n");
        return;//    return CCI_Status_IRQ_EXPIRED;
    }
    CCICtrl_SetMaskForInterrupt(CCI, i2c_protocol, (irq_i2c_err | irq_i2c_ack));
    CCIRsm_SetFrequency(self, m0, CCIInfo.default_i2c_speed, TRUE);
    CCIRsm_SetFrequency(self, m1, CCIInfo.default_i2c_speed, TRUE);
}

/* ---------------------------------------------------------------------------
 *    FUNCTION        CCIRsm_getChipId
 *    DESCRIPTION     Return ChipId 
 *    DEPENDENCIES    None                                           
 *    PARAMETERS      Pointer to CCI Resource Manager Object, output parameter
 *    RETURN VALUE    None
 *    SIDE EFFECTS    
 * ------------------------------------------------------------------------ */
void CCIRsm_getChipId(CCIRsm* self, CameraPlatformChipIdType* chipId)
{ 
    *chipId = self->chipId;
}

/* ---------------------------------------------------------------------------
 *    FUNCTION        CCIRsm_ConfigResource
 *    DESCRIPTION     Configures Resource Type 
 *    DEPENDENCIES    None                                           
 *    PARAMETERS      Pointer to CCI Resource Manager Object, output parameter
 *    RETURN VALUE    None
 *    SIDE EFFECTS    
 * ------------------------------------------------------------------------ */
void CCIRsm_ConfigResource(CCIRsm* self, CCIResourceType* resource, operationtype op, 
    CCIServiceRequestType* rqst)
{
    (void)self;
    resource->config.bcontinuous = FALSE;

    switch (op)
    {
        case i2c_write:
        case i2c_queued_write:
            resource->config.signal = signal;
            resource->config.irq = reports[1].data;
            resource->config.eautorun = CCILD_AUTO;
            if ((self->chipId == MSM8994) && 
                (rqst->user_options & CAMSENSOR_I2C_FORCED_BURST_MODE))
            {
                resource->config.eautorun = CCILD_MANUAL;
                resource->config.bcontinuous = TRUE;
            }
            break;
        case i2c_read:
            resource->config.signal = signal;
            resource->config.irq = 0;
            resource->config.eautorun = CCILD_MANUAL;
            break;
        case i2c_async_write:
            resource->config.signal = signal_async;
            resource->config.irq = reports[2].data;
            resource->config.eautorun = CCILD_AUTO;
            if ((self->chipId == MSM8994) && 
                (rqst->user_options & CAMSENSOR_I2C_FORCED_BURST_MODE))
            {
                resource->config.eautorun = CCILD_MANUAL;
                resource->config.bcontinuous = TRUE;
            }
            break;
        case i2c_async_irq:
            resource->config.signal = signal_max;
            resource->config.irq = reports[4].data;
            resource->config.eautorun = CCILD_MANUAL;
            resource->config.bcontinuous = FALSE;
            break;
        case on_gpio_in:
            resource->config.signal = signal_max;
            resource->config.irq = 0;
            resource->config.eautorun = CCILD_MANUAL;
            resource->config.bcontinuous = FALSE;
            break;
        case gpio_write:
            resource->config.signal      = signal;
            resource->config.irq         = reports[5].data;
            resource->config.eautorun    = CCILD_MANUAL;
            resource->config.bcontinuous = FALSE;
            break;
        default:
            return;
    }

    resource->config.op = op;
    resource->config.master = rqst->master;
}
/* ---------------------------------------------------------------------------
 *    FUNCTION        CCIRsm_AcquireResource
 *    DESCRIPTION     Return CCI resource
 *    DEPENDENCIES    None                                           
 *    PARAMETERS      master and opertion defines type of resources user wants 
 *    RETURN VALUE    CCIStatus 
 *    SIDE EFFECTS    
 * ------------------------------------------------------------------------ */
CCIStatus CCIRsm_AcquireResource(CCIRsm* self, CCIResourceType* resource) 
{
    CCIQueueType queueId = queue_max;

    queueId = CCIRsm_AssignQueueId(resource->config.op, resource->config.master);

    if (queue_max == queueId)
        return CCI_Status_NO_RESOURCE;

    resource->queueInfo = &self->CCIInfo->queue_info_list[queueId]; 
    resource->Queue     = CCI->QUEUES[queueId];
    /// inquire cci capability
    CCIRsm_InquireCapability(self, resource);

    CCILoader_Init(&resource->loader, resource, &resource->config);
    self->usageMap |= (0x1 << queueId);
    return CCI_Status_OK;
}

/* ---------------------------------------------------------------------------
 *    FUNCTION        CCIRsm_ReleaseResource
 *    DESCRIPTION     Releases CCI resource
 *    DEPENDENCIES    None                                           
 *    PARAMETERS      resource that was acquired previously 
 *    RETURN VALUE    None
 *    SIDE EFFECTS    
 * ------------------------------------------------------------------------ */
void CCIRsm_ReleaseResource(CCIRsm* self, CCIResourceType* resource)  
{
    if(resource && resource->queueInfo && 
            resource->queueInfo->queueId < self->CCIInfo->max_num_of_queues)
    {
        self->usageMap &= ~(0x1 << resource->queueInfo->queueId);
    }
}

/* ---------------------------------------------------------------------------
 *    FUNCTION        CCIRsm_VefifyResourceCapability
 *    DESCRIPTION     check if current resource can handle current request 
 *    DEPENDENCIES    None                                           
 *    PARAMETERS      
 *    RETURN VALUE    True if it can handle the current request 
 *    SIDE EFFECTS    
 * ------------------------------------------------------------------------ */
CCIRsCapabilityType CCIRsm_VefifyResourceCapability(CCIRsm* self, CCIResourceType* resource, UINT16 len)  
{
    UINTN capability, nbytes = 0;
    enum { write_cmd_sz = 3 };
    INTN idx = 0;
    if (!resource)
        return CCIRs_Cap_Impossible;

    (void)self;

    capability = CCILoader_CheckAvailabilty(&resource->loader);

    for (idx = 0; idx < 2; idx++)
    {
        /// lock unlock and report commands are needed to be accounted for
        capability -= 3; 

        nbytes = (capability / write_cmd_sz) * resource->capability.mx_write_trnsf_unt;

        if (capability % write_cmd_sz == 1)
            nbytes += 3;
        else if (capability % write_cmd_sz == 2)
            nbytes += 7;

        /// if it can't be handle it with current capacity
        /// check the maximum capacity
        if (nbytes >= len)
        {
            if (0 == idx)
                return CCIRs_Cap_Ok;
            else
                return CCIRs_Cap_After_Unload;
        }

        capability = resource->capability.mx_burt_write_transf_capability;
    }

    return CCIRs_Cap_Impossible;
}

/* ---------------------------------------------------------------------------
 *    FUNCTION        CCIRsm_SetFrequency
 *    DESCRIPTION     Set frequency for I2C bus 
 *    DEPENDENCIES    None                                           
 *    PARAMETERS      
 *    RETURN VALUE    CCIRsCapability 
 *    SIDE EFFECTS    
 * ------------------------------------------------------------------------ */
CCIStatus CCIRsm_SetFrequency(CCIRsm* self, CCIMasterType masterId, 
    CameraSensorI2C_SpeedType efrequency, BOOLEAN bForced)
{
    CCICLKFrqIdx frequencyIdx = CCIClkFrqIdx_Normal;
    CCIQueueType q0 = m0q0, q1 = m0q1;
    enum CCI_I2C_FRQCY ebus_frq = frqcy_400khz;
    CCIStatus eStatus = CCI_Status_OK;

    if (CCIInfo.max_num_of_master <= masterId)
        return CCI_Status_BAD_PARAM;

    if (masterId == m1)
    {
        q0 = m1q0;
        q1 = m1q1;
    }

    /// check if master is being shared by other client
    if(self->usageMap & (0x1 << q0) && self->usageMap & (0x1 << q1))
        return CCI_Status_NO_RESOURCE;

    switch (efrequency)
    {
        case CAMSENSOR_I2C_100KHZ:
            ebus_frq = frqcy_100khz;
            break;
        case CAMSENSOR_I2C_400KHZ:
            ebus_frq = frqcy_400khz;
            break;
        case CAMSENSOR_I2C_1000KHZ:
            ebus_frq = frqcy_1000khz;
            frequencyIdx = CCIClkFrqIdx_Turbo;
            break;
        default:
            return CCI_Status_BAD_PARAM;
    }

    self->nCurrentCCIClkFrq = self->nfrequency_data[frequencyIdx];
    /// if the requested frequency is different from current frequency 
     if (bForced || !(self->mastersBusFrq & ((0x1 << ebus_frq) << masterId * frqcy_max)))
    {
        eStatus = CCI_HAL_SetFrequency(masterId, efrequency);
        if (CCI_Status_OK != eStatus) 
            return eStatus;
        /// reset master's bus frequency
        self->mastersBusFrq &= ~(((0x1 << frqcy_max) - 1) << masterId * frqcy_max);
        /// set bit frequency bit
        self->mastersBusFrq |= ((0x1 << ebus_frq) << masterId * frqcy_max);
    }
    return eStatus;
}

