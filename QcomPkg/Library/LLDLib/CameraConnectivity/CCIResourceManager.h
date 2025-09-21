#ifndef H_CCI_RESOURCE_MANAGER_H
#define H_CCI_RESOURCE_MANAGER_H

 /* =========================================================================== 

                                 C C I D r i v e r

                                    (CCIResourceManager)

                               D e f i n i t i o n s
 *//** @file CCIResourceManager.h                                                                                                                                                                                          
 This file contains CCIResourceManager implementation
                                                                                                                                                                                                                                                                                                    
 @par EXTERNALIZED FUNCTIONS                                                                                                                                                                                           
 None                                                                                                                                                                                                                                                                                               
                                                                                                                                                                                                                                                                                                    
 @par INITIALIZATION AND SEQUENCING REQUIREMENTS                                                                                                                                                                       
 None                                                                                                                                                                                                                                                                                               
                                                                                                                                                                                                                                                                                                    
 Copyright (c) 2014 - 2017 Qualcomm Technologies, Inc. All Rights Reserved.                                                                                                                                                  
 Qualcomm Proprietary and Confidential.                                                                                                                                                                                                                                                             
 Export of this technology or software is regulated by the U.S. Government.                                                                                                                                                                                                                         
 Diversion contrary to U.S. law prohibited.                                                                                                                                                                                                                                                         
**===========================================================================                                                                                                                                        
**                         INCLUDE FILES FOR MODULE                                                                                                                                                                                                                                                
**=========================================================================== */                                                                                                                                                                                          
#include "CCIResourceType.h"

/* ==========================================================================
**                        DATA DECLARATIONS
** ========================================================================== */
/* --------------------------------------------------------------------------
** Constant / Define Declarations
** -------------------------------------------------------------------------- */
#define CCI_MX_FRQ_SZ 2
/* --------------------------------------------------------------------------
** Type Declarations
** -------------------------------------------------------------------------- */


typedef struct CCIRsm CCIRsm;       
struct CCIRsm
{
    CameraPlatformChipIdType chipId;
    CCIHWDetaisType* CCIInfo;
    UINT32 nCurrentCCIClkFrq;
    UINT32 nfrequency_data[CCI_MX_FRQ_SZ];
    UINT32 usageMap;
    UINT32 mastersBusFrq;
};

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
void CCIRsm_Init(CCIRsm* self);

/* ---------------------------------------------------------------------------
 *    FUNCTION        CCIRsm_ConfigResource
 *    DESCRIPTION     Configures Resource Type 
 *    DEPENDENCIES    None                                           
 *    PARAMETERS      Pointer to CCI Resource Manager Object, output parameter
 *    RETURN VALUE    None
 *    SIDE EFFECTS    
 * ------------------------------------------------------------------------ */
void CCIRsm_ConfigResource(CCIRsm* self, CCIResourceType* resource, operationtype op,
    CCIServiceRequestType* rqst);
/* ---------------------------------------------------------------------------
 *    FUNCTION        CCIRsm_AcquireResource
 *    DESCRIPTION     Return CCI resource
 *    DEPENDENCIES    None                                           
 *    PARAMETERS      master and opertion defines type of resources user wants 
 *    RETURN VALUE    CCIStatus 
 *    SIDE EFFECTS    
 * ------------------------------------------------------------------------ */
CCIStatus CCIRsm_AcquireResource(CCIRsm* self, CCIResourceType* resource);

/* ---------------------------------------------------------------------------
 *    FUNCTION        CCIRsm_SetFrequency
 *    DESCRIPTION     Set frequency for I2C bus 
 *    DEPENDENCIES    None                                           
 *    PARAMETERS      
 *    RETURN VALUE    CCIRsCapability 
 *    SIDE EFFECTS    
 * ------------------------------------------------------------------------ */
CCIStatus CCIRsm_SetFrequency(CCIRsm* self, CCIMasterType masterId,
    CameraSensorI2C_SpeedType efrequency, BOOLEAN bForced);

#endif
