#ifndef H_CCILOADER_H
#define H_CCILOADER_H

 /* ===========================================================================

                                 C C I D r i v e r

                                    (CCILoader)

                               D e f i n i t i o n s
 *//** @file CCILoader.h                                                                                                                                                                                          
 This file contains CCILoader implementation
                                                                                                                                                                                                                                                                                                    
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
#include "CCICmd.h"
#include "CCI_HAL_HWIO.h"
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

/* ===========================================================================
                        EXTERNAL API DECLARATIONS
=========================================================================== */
/* ---------------------------------------------------------------------------
 *    FUNCTION        CCILoader_Load
 *    DESCRIPTION     Loads one CCI command. CCI command size is either 1 or 3.
 *    DEPENDENCIES    
 *    PARAMETERS      eCCICmdSz is the size of CCI cmd
 *    RETURN VALUE    None
 *    SIDE EFFECTS    None
 * ------------------------------------------------------------------------ */
CCIStatus CCILoader_Load(CCILoader* self, CCICmd* cmd, CCIStatus* eStatus);
/* ---------------------------------------------------------------------------
 *    FUNCTION        CCILoader_LoadMultiple
 *    DESCRIPTION     
 *    DEPENDENCIES    
 *    PARAMETERS      
 *    RETURN VALUE    None
 *    SIDE EFFECTS    None
 * -------------------------------------------------------------------------- */
CCIStatus CCILoader_LoadMultiple(CCILoader* self, CCICmd* cmd, UINTN nSz, CCIStatus *eStatus);

/* ---------------------------------------------------------------------------
 *    FUNCTION        CCILoader_UnloadAll
 *    DESCRIPTION     
 *    DEPENDENCIES    
 *    PARAMETERS      
 *    RETURN VALUE    None
 *    SIDE EFFECTS    None
 * -------------------------------------------------------------------------- */
CCIStatus CCILoader_UnloadAll(CCILoader* loader, CCIStatus* eStatus);

/* ---------------------------------------------------------------------------
 *    FUNCTION        CCILoader_UnloadWithoutWait
 *    DESCRIPTION     
 *    DEPENDENCIES    
 *    PARAMETERS      
 *    RETURN VALUE    None
 *    SIDE EFFECTS    None
 * -------------------------------------------------------------------------- */
void CCILoader_UnloadWithoutWait(CCILoader* self);

/* ---------------------------------------------------------------------------
 *    FUNCTION        CCILoader_GetState
 *    DESCRIPTION     
 *    DEPENDENCIES    
 *    PARAMETERS      
 *    RETURN VALUE    None
 *    SIDE EFFECTS    None
 * -------------------------------------------------------------------------- */
CCILdState CCILoader_GetState(CCILoader* self);

/* ---------------------------------------------------------------------------
 *    FUNCTION        CCILoader_CheckAvailabilty
 *    DESCRIPTION     
 *    DEPENDENCIES    
 *    PARAMETERS      
 *    RETURN VALUE    None
 *    SIDE EFFECTS    None
 * -------------------------------------------------------------------------- */
UINTN CCILoader_CheckAvailabilty(CCILoader* self);

/* ---------------------------------------------------------------------------
 *    FUNCTION        CCILoader_Init
 *    DESCRIPTION     Initialize Loader
 *    DEPENDENCIES    
 *    PARAMETERS      
 *    RETURN VALUE    None
 *    SIDE EFFECTS    None
 * ------------------------------------------------------------------------ */
CCIStatus CCILoader_Init(CCILoader* self, CCIResourceType* resource, CCIResourceConfig* config);

/* ---------------------------------------------------------------------------
 *    FUNCTION        CCILoader_Deinit
 *    DESCRIPTION     Deinitialize Loader
 *    DEPENDENCIES    
 *    PARAMETERS      
 *    RETURN VALUE    None
 *    SIDE EFFECTS    None
 * ------------------------------------------------------------------------ */
CCIStatus CCILoader_Deinit(CCILoader* self);
#endif
