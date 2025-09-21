#ifndef __CCICMDBLDER_H_
#define __CCICMDBLDER_H_

 /* ===========================================================================

                                 C C I D r i v e r

                                    (CCICmdBuilder)

                               D e f i n i t i o n s
 @file CCICmdBuilder.h                                                                                                                                                                                          
 This file contains CCICmdBuilder implementation
                                                                                                                                                                                                                                                                                                    
 @par EXTERNALIZED FUNCTIONS                                                                                                                                                                                           
 None                                                                                                                                                                                                                                                                                               
                                                                                                                                                                                                                                                                                                    
 @par INITIALIZATION AND SEQUENCING REQUIREMENTS                                                                                                                                                                       
 None                                                                                                                                                                                                                                                                                               
                                                                                                                                                                                                                                                                                                    
 Copyright (c) 2014-2017 Qualcomm Technologies, Incorporated. All Rights Reserved.                                                                                                                                                  
 Qualcomm Proprietary and Confidential.                                                                                                                                                                                                                                                             
 Export of this technology or software is regulated by the U.S. Government.                                                                                                                                                                                                                         
 Diversion contrary to U.S. law prohibited.                                    */                                                                                                                                                                                                                     
/* ===========================================================================                                                                                                                                        
**                         INCLUDE FILES FOR MODULE                                                                                                                                                                                                                                                
**=========================================================================== */                                                                                                                                                                                          
#include "CameraCCITypes.h"
#include "CCICmd.h"
#include "CCIResourceType.h"

/* ===========================================================================
                        DATA DECLARATIONS
=========================================================================== */
/* ---------------------------------------------------------------------------
** Constant / Define Declarations
** ------------------------------------------------------------------------ */
/* ---------------------------------------------------------------------------
** Type Declarations
** ------------------------------------------------------------------------ */
typedef struct CCICmdBuilder CCICmdBuilder;
struct CCICmdBuilder
{
    CCICmd    cmd;
    CCICapability capability;
    CCIStatus state;
};

/* ===========================================================================
                        EXTERNAL API DECLARATIONS
=========================================================================== */
CCICmd* CCICmdBuilder_I2CSetParam(CCICmdBuilder* self, UINT8 ncid, UINT8 nslaveaddr);
CCICmd* CCICmdBuilder_I2CRead(CCICmdBuilder* self, UINT32 len);
CCICmd* CCICmdBuilder_I2CUnlock(CCICmdBuilder* self);
CCICmd* CCICmdBuilder_I2CLock(CCICmdBuilder* self);
CCICmd* CCICmdBuilder_I2CWrite(CCICmdBuilder* self, CCII2CConfig* config);

/* ---------------------------------------------------------------------------
*    FUNCTION        CCICmdBuilder_Init
*    DESCRIPTION     Initializes CCICmdBuilder
*    DEPENDENCIES
*    PARAMETERS
*    RETURN VALUE    CCIStatus
*    SIDE EFFECTS    None
* ------------------------------------------------------------------------ */
CCIStatus CCICmdBuilder_Init(CCICmdBuilder* self, CCICapability* capability);



#endif