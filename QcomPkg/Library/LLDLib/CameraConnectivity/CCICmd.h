#ifndef __CCICMD_H_
#define __CCICMD_H_

 /* ===========================================================================

                                 C C I D r i v e r

                                    (CCICmd)

                               D e f i n i t i o n s
 @file CCICmd.h                                                                                                                                                                                          
 This file contains CCICmd implementation
                                                                                                                                                                                                                                                                                                    
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

enum { max_cci_cmd_sz = 3 };

typedef struct CCICmd CCICmd;

struct CCICmd
{
    UINT32 ccmd[max_cci_cmd_sz];
    UINT32 idx;
    UINTN nSz;
};

/* ---------------------------------------------------------------------------
*    FUNCTION        ccicmd_get_size
*    DESCRIPTION     returns next ccicmd size
*    DEPENDENCIES
*    PARAMETERS
*    RETURN VALUE    None 
*    SIDE EFFECTS    None 
* ------------------------------------------------------------------------ */
__inline void ccicmd_get_size(CCICmd const* self, UINTN* nSz)
{
    *nSz = self->nSz;
}

/* ---------------------------------------------------------------------------
*    FUNCTION        ccicmd_init_with_value
*    DESCRIPTION     initializes ccicmd array with cci cmd 
*    DEPENDENCIES
*    PARAMETERS
*    RETURN VALUE    pointer to CCICmd 
*    SIDE EFFECTS    None
* ------------------------------------------------------------------------ */

__inline CCICmd* ccicmd_init_with_value(CCICmd* self, UINT32 input)
{ 
    self->idx = 0;
    self->ccmd[0] = input;
    self->nSz = 1;
    return self;
}

#endif
