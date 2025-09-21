#ifndef H_CCII2C_H
#define  H_CCII2C_H

 /* ===========================================================================

                                 C C I D r i v e r

                                    (CCIService)

                               D e f i n i t i o n s
 @file CCIService.h                                                                                                                                                                                          
 This file contains CCIService implementation
                                                                                                                                                                                                                                                                                                    
 @par EXTERNALIZED FUNCTIONS                                                                                                                                                                                           
 None                                                                                                                                                                                                                                                                                               
                                                                                                                                                                                                                                                                                                    
 @par INITIALIZATION AND SEQUENCING REQUIREMENTS                                                                                                                                                                       
 None                                                                                                                                                                                                                                                                                               
                                                                                                                                                                                                                                                                                                    
 Copyright (c) 2014-2017 Qualcomm Technologies, Incorporated. All Rights Reserved.                                                                                                                                                  
 Qualcomm Proprietary and Confidential.                                                                                                                                                                                                                                                             
 Export of this technology or software is regulated by the U.S. Government.                                                                                                                                                                                                                         
 Diversion contrary to U.S. law prohibited.                                     */                                                                                                                                                                                                                    
/* ===========================================================================                                                                                                                                        
**                         INCLUDE FILES FOR MODULE                                                                                                                                                                                                                                                
**=========================================================================== */                                                                                                                                                                                          
#include "CCI_HAL_HWIO.h"
#include "CCIResourceType.h"
#include "CCIResourceManager.h"
#include "CCICmdBuilder.h"
/* ===========================================================================
                        DATA DECLARATIONS
=========================================================================== */
/* ---------------------------------------------------------------------------
** Constant / Define Declarations
** ------------------------------------------------------------------------ */
/* ---------------------------------------------------------------------------
** Type Declarations
** ------------------------------------------------------------------------ */
typedef struct CCIService CCIService;

struct CCIService
{
    /// CCI resource manager
    CCIRsm* rsm;
    CCICmdBuilder cmdbuilder;
    CCIResourceType resource;
    CCILdState loader_state;
    CCIStatus eState;
    operationtype op;
};

/* ===========================================================================
                        EXTERNAL API DECLARATIONS
=========================================================================== */
/* ---------------------------------------------------------------------------
 *    FUNCTION        CCIService_Init
 *    DESCRIPTION     Initializes CCI Builder 
 *    DEPENDENCIES    None                                           
 *    PARAMETERS      Pointer to CCI Builder
 *    RETURN VALUE    None
 *    SIDE EFFECTS    
 * ------------------------------------------------------------------------ */
void CCIService_Init(CCIService* self, CCIRsm* rsm, operationtype op, CCIServiceRequestType* request);

/* ---------------------------------------------------------------------------
 *    FUNCTION        CCIService_Read
 *    DESCRIPTION     Receive bytes from slave devices through I2C protocol 
 *    DEPENDENCIES    None
 *    PARAMETERS      pointer to CCICmdbuilder, pointer to CCIUserRquestType  
 *    RETURN VALUE    None 
 *    SIDE EFFECTS    When fails, CCI H/W remains in error state 
 * ------------------------------------------------------------------------ */
void CCIService_Read(CCIService* self, CCIUserRequestType* rqst);
#endif
