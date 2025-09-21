/** @file CCIService.c

Copyright (c) 2016-2017 QUALCOMM Technologies, Inc. All Rights Reserved.
Qualcomm Proprietary and Confidential.
Export of this technology or software is regulated by the U.S. Government.
Diversion contrary to U.S. law prohibited.

**/

#include "CCI_HAL.h"
#include "CCIService.h"
#include "CCIResourceManager.h"
#include "CCILoader.h"
#include "CCICmdBuilder.h"
#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>


extern CCICTX CCI;
extern CCIHWDetaisType CCIInfo;

/*===========================================================================
                            INTERNAL API DECLARATIONS
=========================================================================== */
/* ---------------------------------------------------------------------------
 *    FUNCTION        CCIService_AcquireResource
 *    DESCRIPTION     Acquires resources needed for CCIService opeartion
 *    DEPENDENCIES    None
 *    PARAMETERS      CCIService object
 *    RETURN VALUE    CCIStatus enum type
 *    SIDE EFFECTS    None
 * ------------------------------------------------------------------------ */
static CCIStatus CCIService_AcquireResource(CCIService* self, CCIServiceRequestType* rqst)
{
    CCIRsm_ConfigResource(self->rsm, &self->resource, self->op, rqst);
    /// acquries resource defined in resource config type
    self->eState = CCIRsm_AcquireResource(self->rsm, &self->resource);
    // init the cmd builder
    CCICmdBuilder_Init(&self->cmdbuilder, &self->resource.capability);
    return self->eState;
}

/*===========================================================================
                            EXTERNAL API DECLARATIONS
=========================================================================== */
/* ---------------------------------------------------------------------------
 *    FUNCTION        I2CSetParam
 *    DESCRIPTION     Load one I2CSetParam command CCI Queue
 *    DEPENDENCIES    
 *    PARAMETERS      
 *    RETURN VALUE    CCIStatus 
 *    SIDE EFFECTS    None
 * ------------------------------------------------------------------------ */
void I2CSetParam(CCIService* self, UINT8 ncid, UINT8 nslaveaddr)

{
    CCILoader_Load(&self->resource.loader, CCICmdBuilder_I2CSetParam(&self->cmdbuilder, 
        ncid, nslaveaddr), &self->cmdbuilder.state);
}

/* ---------------------------------------------------------------------------
 *    FUNCTION        I2CRead
 *    DESCRIPTION     Load one I2CRead command CCI Queue
 *    DEPENDENCIES    
 *    PARAMETERS      
 *    RETURN VALUE    CCIStatus 
 *    SIDE EFFECTS    None
 * ------------------------------------------------------------------------ */
void I2CRead(CCIService* self, UINT32 len)
{
    CCILoader_Load(&self->resource.loader, CCICmdBuilder_I2CRead(&self->cmdbuilder, len), 
        &self->cmdbuilder.state);
}
/* ---------------------------------------------------------------------------
 *    FUNCTION        I2CWrite
 *    DESCRIPTION     Load one I2CWrite command CCI Queue
 *    DEPENDENCIES    
 *    PARAMETERS      
 *    RETURN VALUE    CCIStatus 
 *    SIDE EFFECTS    None
 * ------------------------------------------------------------------------ */
void I2CWrite(CCIService* self,  CCII2CConfig* config)
{
    CCILoader_Load(&self->resource.loader, CCICmdBuilder_I2CWrite(&self->cmdbuilder, config), 
        &self->cmdbuilder.state);
    self->loader_state = CCILoader_GetState(&self->resource.loader);
}
/* ---------------------------------------------------------------------------
 *    FUNCTION        WriteToDevice
 *    DESCRIPTION     Write to Device
 *    DEPENDENCIES    
 *    PARAMETERS      
 *    RETURN VALUE    CCIStatus 
 *    SIDE EFFECTS    None
 * ------------------------------------------------------------------------ */
void I2CLock(CCIService* self)
{
    CCILoader_Load(&self->resource.loader, CCICmdBuilder_I2CLock(&self->cmdbuilder), 
        &self->cmdbuilder.state);
}
/* ---------------------------------------------------------------------------
 *    FUNCTION        WriteToDevice
 *    DESCRIPTION     Write to Device
 *    DEPENDENCIES    
 *    PARAMETERS      
 *    RETURN VALUE    None 
 *    SIDE EFFECTS    None
 * ------------------------------------------------------------------------ */
 void I2CUnlock(CCIService* self)
{
    CCILoader_Load(&self->resource.loader, CCICmdBuilder_I2CUnlock(&self->cmdbuilder), 
        &self->cmdbuilder.state);
}

/* ---------------------------------------------------------------------------
 *    FUNCTION        WriteToDevice
 *    DESCRIPTION     Write to Device
 *    DEPENDENCIES    
 *    PARAMETERS      
 *    RETURN VALUE    CCIStatus 
 *    SIDE EFFECTS    None
 * ------------------------------------------------------------------------ */
CCIStatus WriteToDevice(CCIService* self)
{ 
    if (self->cmdbuilder.state)
        return self->cmdbuilder.state;
    if (self->eState)
        return self->eState;

    return self->eState = CCILoader_UnloadAll(&self->resource.loader, &self->cmdbuilder.state);
}

/* ---------------------------------------------------------------------------
 *    FUNCTION        CCIService_Init
 *    DESCRIPTION     CCIService implements and enforces the policy of how CCI 
                      top level operation. This function initializes CCIService 
                      object
 *    DEPENDENCIES    None
 *    PARAMETERS      CCIService object, Resource Manager, operation type
                      master type
 *    RETURN VALUE    None 
 *    SIDE EFFECTS    None
 * ------------------------------------------------------------------------ */
void CCIService_Init(CCIService* self, CCIRsm* rsm, operationtype op, CCIServiceRequestType* request)

{ 
    /// null ptr check
    if (!self || !rsm)
        return;

    self->rsm = rsm;
    self->eState = CCI_Status_OK;
    self->op = op;
    self->loader_state = CCILD_State_Unset;
    CCIRsm_ConfigResource(self->rsm, &self->resource, op, request);

    if (gpio != request->master)
        self->eState = CCIRsm_SetFrequency(rsm, request->master, request->eCCI_I2CBusSpeed, FALSE);
    // init the cmd builder
    CCICmdBuilder_Init(&self->cmdbuilder, &self->resource.capability);
}

/* ---------------------------------------------------------------------------
 *    FUNCTION        CCIService_Read
 *    DESCRIPTION     Receive bytes from slave devices through I2C protocol 
 *    DEPENDENCIES    None
 *    PARAMETERS      pointer to CCICmdbuilder, pointer to CCIUserRquestType  
 *    RETURN VALUE    None 
 *    SIDE EFFECTS    When fails, CCI H/W remains in error state 
 * ------------------------------------------------------------------------ */
void CCIService_Read(CCIService* self, CCIUserRequestType* rqst)
{
    if (CCIService_AcquireResource(self, &rqst->serviceRqst))
        return;

    I2CSetParam(self, rqst->ncid, rqst->nslaveaddr);
    I2CLock  (self);
    I2CWrite(self, &rqst->config);
    I2CRead  (self, rqst->config.ndatasz);
    I2CUnlock(self);
    int nTimeout = 20;
    if(CCI_Status_OK == WriteToDevice(self))
    {
        /// poll here
        while(CCICtrl_GetI2CReadBufLevel(CCI, rqst->serviceRqst.master) * CCIInfo.cciword_sz < rqst->config.ndatasz && nTimeout-- != 0)
        {
            AsciiPrint("  Read buffer is empty... \n" );
            gBS->Stall(100000);
        }

        if(nTimeout > 0){
            AsciiPrint("  Reading buffer ... \n" );
            CCI_HAL_Read_I2C_Data(rqst->queueInfo, rqst->rdbuff, rqst->config.ndatasz, &rqst->eStatus);
        }
        else{
            AsciiPrint("  Timed out ... \n" );
            rqst->eStatus = CCI_Status_GENERAL_ERROR;
        }
    }
    else{
        AsciiPrint("  WriteToDevice failed ... \n" );
        rqst->eStatus = CCI_Status_GENERAL_ERROR;
    }

 }

